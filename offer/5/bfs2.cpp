void bfs2(vector<vector<int> graph, int source){
  int size = graph.size();
  vector<bool> visited(size, false);
  vector<int> dist(size, INT_MAX);
  prority_queue<pair<int,int>, vector<pair<int, int>>, greater<pair<int,int>>> q;
  q.push(make_pair(0, source));
  dist[source]=0;
  while(!q.empty()){
    int top = q.top();q.pop();
    int cur = top.second;
    if(visited[cur]) continue;
    visited[cur] = true;
    int cur_dist = top.first;
    for(int i = 0; i < graph[cur].size(); i++){
      int child = graph[cur][i].first;
      int child_weight = graph[cur][i].second;
      if(!visited[child]){
        if( dist[child] > cur_dist + child_weight){
          q.push(make_pair(cur_dist + child_weight, child));
          dist[child] = cur_dist + child_weight;
        }
      }
    }
  }

}
