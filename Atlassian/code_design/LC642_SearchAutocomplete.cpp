/*
 * 642. Design Search Autocomplete System
 * Hard
 * [Atlassian Code Design Round — ✅ confirmed 2024/2025, Confluence search context]
 *
 * Design a search autocomplete system:
 *   AutocompleteSystem(sentences, times):
 *     Initialize with historical sentences and their hit counts.
 *
 *   input(c):
 *     - If c != '#': add c to current input, return top-3 matching sentences
 *       (sorted by: hit count desc, then lexicographic asc on ties).
 *     - If c == '#': save current input to history, reset current input.
 *       Return [].
 *
 * Example:
 *   AutocompleteSystem(["i love you","island","ironman","i love leetcode"], [5,3,2,2])
 *   input('i')  → ["i love you","island","i love leetcode"]  (top 3 by freq)
 *   input(' ')  → ["i love you","i love leetcode"]
 *   input('a')  → []
 *   input('#')  → []  (saves "i a" with count 1)
 *
 * ─────────────────────────────────────────────────────────────────
 * Approach: Trie + per-node top-3 cache
 *   TrieNode stores map<string, int> hotMap (sentence → count) ← top-3 for this prefix.
 *   (Alternative: store only sorted top-3 list for O(1) retrieval but O(k) update)
 *
 *   insert(sentence, times): walk Trie path, at each node update its hotMap.
 *
 *   input(c):
 *     - If c == '#': call insert(curInput, 1), reset curInput. Return [].
 *     - Else: append c to curInput, walk Trie to currentNode.
 *       If node exists: extract top-3 from node->hotMap.
 *       Else: return [].
 *
 *   Time: insert O(L × |top-k|), input O(1) retrieval from node
 *   Space: O(N × L × k) where N = sentences, L = avg length
 *
 * Simplified approach below (brute-force per node, easier to code in interview):
 *   Each TrieNode stores unordered_map<string, int>; top-3 extracted at query time.
 */
#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    unordered_map<string, int> counts;  // sentence → count for all sentences with this prefix
    unordered_map<char, TrieNode*> children;
};

class AutocompleteSystem {
    TrieNode* root;
    TrieNode* cur;        // pointer to node for current input prefix
    string curInput;

    void insert(TrieNode* node, const string& sentence, int times) {
        for (char c : sentence) {
            if (!node->children.count(c)) node->children[c] = new TrieNode();
            node = node->children[c];
            node->counts[sentence] += times;
        }
    }

    vector<string> topK(TrieNode* node, int k = 3) {
        // Extract top-k by (count desc, lexicographic asc)
        vector<pair<int,string>> vec;
        for (auto& [s, cnt] : node->counts) vec.push_back({cnt, s});
        sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
            return a.first != b.first ? a.first > b.first : a.second < b.second;
        });
        vector<string> res;
        for (int i = 0; i < min(k, (int)vec.size()); ++i) res.push_back(vec[i].second);
        return res;
    }

public:
    AutocompleteSystem(vector<string>& sentences, vector<int>& times) {
        root = new TrieNode();
        cur = root;
        curInput = "";
        for (int i = 0; i < (int)sentences.size(); ++i)
            insert(root, sentences[i], times[i]);
    }

    vector<string> input(char c) {
        if (c == '#') {
            insert(root, curInput, 1);
            cur = root;
            curInput = "";
            return {};
        }

        curInput += c;
        if (!cur || !cur->children.count(c)) {
            cur = nullptr;   // dead end, no completions
            return {};
        }
        cur = cur->children[c];
        return topK(cur);
    }
};

/*
 * ── Follow-up (Atlassian onsite) ────────────────────────────────
 * Q: Current approach sorts at query time — how to make it O(1)?
 * A: Store a sorted top-k list at each TrieNode.
 *    On insert: update each ancestor node's top-k (remove old entry, insert new).
 *    Using a size-k ordered set (e.g., std::set with custom comparator): O(log k) per node.
 *    Overall insert: O(L × log k). Query: O(k) to copy top-k list.
 *
 * Q: Scale: Confluence has millions of searches per second — how to scale?
 * A: Shard Trie by first character or hash. Use Redis Sorted Sets:
 *    ZADD prefix:{prefix} {count} {sentence} — O(log n) insert.
 *    ZREVRANGE prefix:{prefix} 0 2 — O(log n + k) top-3 retrieval.
 *    Much simpler to scale horizontally than in-memory Trie.
 *
 * Q: How to handle typos (fuzzy autocomplete)?
 * A: BK-Tree or Levenshtein automaton for approximate prefix matching.
 *    Or use edit-distance bounded DFS on Trie (expensive for large datasets).
 *    Production: Elasticsearch with fuzzy matching (max 2 edits).
 *
 * Q: How to persist the Trie across restarts?
 * A: Serialize Trie to disk (DFS, each node as a record).
 *    Or store raw (sentence, count) pairs in RocksDB/PostgreSQL and
 *    rebuild Trie on startup. Or use Redis sorted sets as primary store.
 *
 * Related: LC 208 Implement Trie, LC 211 Add and Search Word
 */
