/*
 * 208. Implement Trie (Prefix Tree)
 * Medium
 * [Atlassian DSA Round — ✅ confirmed Glassdoor 2024/2025, search autocomplete context]
 *
 * Implement a Trie with:
 *   - insert(word)       : insert word into trie
 *   - search(word)       : return true if word exists exactly
 *   - startsWith(prefix) : return true if any word has this prefix
 *
 * Example:
 *   insert("apple")
 *   search("apple")   → true
 *   search("app")     → false
 *   startsWith("app") → true
 *   insert("app")
 *   search("app")     → true
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach: children[26] array per node + isEnd flag
 *   - Each node has children[26] (for 'a'–'z')
 *   - insert: walk/create path, mark final node isEnd = true
 *   - search: walk path; return node != null && node->isEnd
 *   - startsWith: walk path; return node != null
 *
 *   Time per op: O(L), L = word/prefix length
 *   Space: O(N × 26), N = total chars inserted
 *
 * Extension (Atlassian Confluence search autocomplete):
 *   Store top-k results per node for O(L + k) autocomplete.
 *
 * Alternative: unordered_map<char, TrieNode*> for non-ASCII / memory efficiency
 */
#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool isEnd;
    TrieNode() : isEnd(false) { fill(children, children + 26, nullptr); }
};

class Trie {
    TrieNode* root;

    TrieNode* find(const string& s) {
        TrieNode* node = root;
        for (char c : s) {
            int idx = c - 'a';
            if (!node->children[idx]) return nullptr;
            node = node->children[idx];
        }
        return node;
    }

public:
    Trie() { root = new TrieNode(); }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx])
                node->children[idx] = new TrieNode();
            node = node->children[idx];
        }
        node->isEnd = true;
    }

    bool search(const string& word) {
        TrieNode* node = find(word);
        return node && node->isEnd;
    }

    bool startsWith(const string& prefix) {
        return find(prefix) != nullptr;
    }

    // ── Bonus: return all words with given prefix ────────────────
    vector<string> autocomplete(const string& prefix) {
        vector<string> result;
        TrieNode* node = find(prefix);
        if (!node) return result;
        string cur = prefix;
        dfs(node, cur, result);
        return result;
    }

private:
    void dfs(TrieNode* node, string& cur, vector<string>& result) {
        if (node->isEnd) result.push_back(cur);
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                cur.push_back('a' + i);
                dfs(node->children[i], cur, result);
                cur.pop_back();
            }
        }
    }
};

/*
 * ── Follow-up (Atlassian onsite) ────────────────────────────────
 * Q: children[26] vs unordered_map?
 * A: Array: O(1) access, fixed 26×8 = 208 bytes/node.
 *    Map: memory-efficient for sparse/Unicode, slower constant.
 *    For lowercase ASCII → array; for Unicode → map.
 *
 * Q: Wildcard search (LC 211 "." matches any char)?
 * A: DFS over all children when encountering '.'.
 *
 * Q: Word replacement (LC 648 Replace Words with Trie root)?
 * A: Build trie of roots; for each word in sentence, find shortest prefix match.
 *
 * Q: How does Confluence use Trie?
 * A: Each Trie node stores top-k hot pages (by access count).
 *    On autocomplete(prefix), directly return node->topK in O(L + k).
 */
