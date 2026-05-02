/*
 * File Collection / Top N Collections
 * [Atlassian Classic - Coding Design Round]
 *
 * Problem: You are given a list of file records, each described as:
 *   [fileName, fileSize, [collection1, collection2, ...]]
 *
 * A file can belong to zero or more collections.
 * Implement:
 *   1. totalSize() - Return the total size of all files in the system
 *   2. topNCollections(n) - Return the top N collections by total file size
 *
 * Follow-ups:
 *   1. Handle nested collections (a collection can contain sub-collections)
 *   2. What if files are added/removed dynamically?
 *   3. What if the same file appears in multiple collections - should we count it once or per collection?
 *
 * Example:
 *   Input:
 *     file1.txt, 100, [collectionA]
 *     file2.txt, 200, [collectionA, collectionB]
 *     file3.txt, 300, [collectionB]
 *     file4.txt, 400, []
 *
 *   totalSize() -> 1000
 *   topNCollections(1) -> [collectionB: 500]  (200 + 300)
 *   topNCollections(2) -> [collectionB: 500, collectionA: 300]
 */
#include <bits/stdc++.h>
using namespace std;

struct FileRecord {
    string name;
    long long size;
    vector<string> collections;

    FileRecord(string name, long long size, vector<string> collections = {})
        : name(move(name)), size(size), collections(move(collections)) {}
};

class FileCollection {
    unordered_map<string, long long> collectionSizes;
    long long total;

public:
    FileCollection(const vector<FileRecord>& files) : total(0) {
        for (const auto& file : files) {
            total += file.size;
            for (const auto& col : file.collections) {
                collectionSizes[col] += file.size;
            }
        }
    }

    long long totalSize() const { return total; }

    // Using min-heap for O(m log n) where m = total collections
    vector<pair<string, long long>> topNCollections(int n) {
        // min-heap: {size, name}
        priority_queue<pair<long long, string>,
                       vector<pair<long long, string>>,
                       greater<>> minHeap;

        for (const auto& [col, sz] : collectionSizes) {
            minHeap.push({sz, col});
            if ((int)minHeap.size() > n) minHeap.pop();
        }

        vector<pair<string, long long>> result;
        while (!minHeap.empty()) {
            auto [sz, col] = minHeap.top(); minHeap.pop();
            result.push_back({col, sz});
        }
        sort(result.begin(), result.end(),
             [](const auto& a, const auto& b) { return a.second > b.second; });
        return result;
    }
};

/*
 * Follow-up: Nested Collections
 * A collection can contain sub-collections. Size of a parent includes all descendants.
 */
class NestedFileCollection {
    unordered_map<string, long long> directSizes;
    unordered_map<string, vector<string>> children; // parent -> child collections
    unordered_map<string, long long> totalSizes;    // memoized
    long long totalFileSize;

public:
    NestedFileCollection(const vector<FileRecord>& files,
                         const unordered_map<string, vector<string>>& parentChildMap)
        : children(parentChildMap), totalFileSize(0) {
        for (const auto& file : files) {
            totalFileSize += file.size;
            for (const auto& col : file.collections) {
                directSizes[col] += file.size;
            }
        }
    }

    long long getCollectionSize(const string& collection) {
        auto it = totalSizes.find(collection);
        if (it != totalSizes.end()) return it->second;

        long long size = 0;
        auto dit = directSizes.find(collection);
        if (dit != directSizes.end()) size = dit->second;

        auto cit = children.find(collection);
        if (cit != children.end()) {
            for (const auto& child : cit->second) {
                size += getCollectionSize(child);
            }
        }

        totalSizes[collection] = size;
        return size;
    }

    vector<pair<string, long long>> topNCollections(int n) {
        for (const auto& [col, _] : directSizes) getCollectionSize(col);
        for (const auto& [col, _] : children) getCollectionSize(col);

        priority_queue<pair<long long, string>,
                       vector<pair<long long, string>>,
                       greater<>> minHeap;

        for (const auto& [col, sz] : totalSizes) {
            minHeap.push({sz, col});
            if ((int)minHeap.size() > n) minHeap.pop();
        }

        vector<pair<string, long long>> result;
        while (!minHeap.empty()) {
            auto [sz, col] = minHeap.top(); minHeap.pop();
            result.push_back({col, sz});
        }
        sort(result.begin(), result.end(),
             [](const auto& a, const auto& b) { return a.second > b.second; });
        return result;
    }
};
