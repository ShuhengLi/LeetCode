/*
 * 588. Design In-Memory File System
 * Hard
 * [Atlassian Code Design Round — ✅ confirmed Glassdoor/Blind 2024/2025]
 *
 * Implement an in-memory file system:
 *   - ls(path)              : list files/dirs at path (sorted)
 *   - mkdir(path)           : create directory (including parent dirs)
 *   - addContentToFile(path, content) : append content to file (create if needed)
 *   - readContentFromFile(path)       : read file content
 *
 * Example:
 *   mkdir("/a/b/c")
 *   addContentToFile("/a/b/c/d","hello")
 *   ls("/")           → ["a"]
 *   ls("/a/b/c")      → ["d"]
 *   readContentFromFile("/a/b/c/d") → "hello"
 *   addContentToFile("/a/b/c/d"," world")
 *   readContentFromFile("/a/b/c/d") → "hello world"
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach: Trie of directory nodes
 *   Each TrieNode represents a directory entry:
 *     - children: map<string, TrieNode*> (subdirs and files)
 *     - content:  string (non-empty only for files)
 *     - isFile:   bool
 *
 *   ls(path):
 *     Walk to node. If it's a file → return {filename}.
 *     If dir → return sorted list of children names.
 *
 *   mkdir(path): Walk/create nodes along path.
 *
 *   addContentToFile(path, content):
 *     Walk to parent dir, create/update file node, append content.
 *
 *   readContentFromFile(path): Walk to file node, return content.
 *
 *   Time: O(L) per operation where L = path depth
 *   Space: O(total chars in all file contents + path components)
 *
 * Atlassian context: Directly maps to Confluence page tree (Space/Page hierarchy),
 *   Bitbucket repository file tree, or any tree-structured content system.
 */
#include <bits/stdc++.h>
using namespace std;

struct Dir {
    map<string, Dir*> children;   // sorted map for ls()
    string content;
    bool isFile = false;
};

class FileSystem {
    Dir* root;

    vector<string> split(const string& path) {
        vector<string> parts;
        string token;
        for (char c : path) {
            if (c == '/') {
                if (!token.empty()) { parts.push_back(token); token.clear(); }
            } else {
                token += c;
            }
        }
        if (!token.empty()) parts.push_back(token);
        return parts;
    }

    Dir* walk(const vector<string>& parts, bool create = false) {
        Dir* cur = root;
        for (const string& p : parts) {
            if (!cur->children.count(p)) {
                if (!create) return nullptr;
                cur->children[p] = new Dir();
            }
            cur = cur->children[p];
        }
        return cur;
    }

public:
    FileSystem() { root = new Dir(); }

    vector<string> ls(const string& path) {
        vector<string> parts = split(path);
        Dir* node = walk(parts);
        if (!node) return {};
        // If it's a file, ls returns just the filename
        if (node->isFile) return {parts.back()};
        // Else list all children (map is already sorted)
        vector<string> res;
        for (auto& [name, _] : node->children) res.push_back(name);
        return res;
    }

    void mkdir(const string& path) {
        walk(split(path), true);
    }

    void addContentToFile(const string& filePath, const string& content) {
        vector<string> parts = split(filePath);
        Dir* node = walk(parts, true);
        node->isFile = true;
        node->content += content;
    }

    string readContentFromFile(const string& filePath) {
        Dir* node = walk(split(filePath));
        return node ? node->content : "";
    }
};

/*
 * ── Follow-up (Atlassian onsite) ────────────────────────────────
 * Q: Why use map<string, Dir*> instead of unordered_map?
 * A: ls() requires sorted output. map iterates in sorted order → O(k) listing.
 *    unordered_map would need sort → O(k log k) per ls. For ls-heavy workloads,
 *    map is better here.
 *
 * Q: How to support file deletion?
 * A: Walk to parent, erase child from parent's map, recursively free memory.
 *    mkdir -p equivalent: walk creates nodes. rmdir equivalent needs reference
 *    counting to handle shared paths.
 *
 * Q: How to support move/rename (mv)?
 * A: Walk to source node, record its Dir* pointer, erase from parent children,
 *    insert into destination parent children under new name. O(path_length).
 *
 * Q: How to implement search (find all files matching name pattern)?
 * A: DFS from root, collect all file nodes whose name matches.
 *    With Trie-based name indexing (separate Trie for filenames): O(L + results).
 *
 * Q: Confluence analogy?
 * A: Space = "/" root, sub-directories = page hierarchy, files = pages with content.
 *    addContentToFile ≈ editPage(append mode).
 *    This design directly models Confluence's tree structure.
 *
 * Related: LC 642 Design Search Autocomplete System, LC 208 Implement Trie
 */
