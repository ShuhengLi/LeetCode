/*146. LRU Cache
 * Design and implement a data structure for Least Recently Used (LRU) cache. It should support the following operations: get and put.

get(key) - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
put(key, value) - Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the least recently used item before inserting a new item.

The cache is initialized with a positive capacity.

Follow up:
Could you do both operations in O(1) time complexity?

Example:

LRUCache cache = new LRUCache( 2 );

cache.put(1, 1);
cache.put(2, 2);
cache.get(1);       // returns 1
cache.put(3, 3);    // evicts key 2
cache.get(2);       // returns -1 (not found)
cache.put(4, 4);    // evicts key 1
cache.get(1);       // returns -1 (not found)
cache.get(3);       // returns 3
cache.get(4);       // returns 4
*/
class LRUCache {
public:
    unordered_map<int, int> m;
    unordered_map<int, list<int>::iterator> t;
    list<int> recent;
    int limit;
    LRUCache(int capacity) {
        limit = capacity;
        m.clear();
        t.clear();
        recent.clear();
    }
    
    int get(int key) {
        int res = -1;
        if(m.find(key)!=m.end()){
            recent.erase(t[key]);
            recent.push_front(key);
            t[key]=recent.begin();
            res = m[key];
        }
        return res;
        
    }
    
    void put(int key, int value) {
        if(m.find(key)!=m.end()){
            recent.erase(t[key]);
        }else{
            if(m.size()>=limit){
                m.erase(recent.back());
                t.erase(recent.back());
                recent.pop_back();
            }
        }
        recent.push_front(key);
        t[key]=recent.begin();
        m[key] = value;
    }
};

