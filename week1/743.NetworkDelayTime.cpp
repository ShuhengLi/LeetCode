/* 743. Network Delay Time
There are N network nodes, labelled 1 to N.

Given times, a list of travel times as directed edges times[i] = (u, v, w), where u is the source node, v is the target node, and w is the time it takes for a signal to travel from source to target.

Now, we send a signal from a certain node K. How long will it take for all nodes to receive the signal? If it is impossible, return -1.

 

Example 1:
Input: times = [[2,1,1],[2,3,1],[3,4,1]], N = 4, K = 2
Output: 2
*/
class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int N, int K) {
        int res = 0;
        vector<map<int,int>> g(N + 1);
        for(auto t:times){
            auto source = t[0];
            auto target = t[1];
            auto time = t[2];
            g[source][target] = time;
        }
        priority_queue<pair<int,int>,vector<pair<int,int>>, greater<pair<int,int>>> q;

        vector<int> dists(N+1, INT_MAX);
        set<int> s;
        dists[K] = 0;
        q.push(make_pair(0,K));
        while(!q.empty()){
            auto cur = q.top(); q.pop();
            int index = cur.second;
            int dist = cur.first;
            if(s.find(index)!=s.end()) continue;
            s.insert(index);
            for(auto it = g[index].begin(); it != g[index].end();it++){
                if(s.find(it->first) == s.end()){
                    dists[it->first] = min(dists[it->first], it->second + dist);
                    q.push(make_pair(dists[it->first], it->first));
                }
            }
        }
        for(auto d:dists){
            if(d!=INT_MAX){
                res = max(d, res);
            }
        }
        return s.size() == N ? res:-1;

    }
};
