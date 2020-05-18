int findKthMatrix(vector<vector<int>> nums, int k){
  if(nums.empty() || nums[0].empty()) return -1;
  int rows = nums.size();
  int columns = nums[0].size();
  vector<bool> temp (columns, false);
  vector<vector<bool>> visited;
  for(int i = 0; i< rows; i++){
    visited.push_back(temp);
  }
  if(a){

  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int,int>>> q;
  if(a){
  }
  int i = 0, j = 0;
  q.push(make_pair(nums[0][0], 0));
  while(!q.empty() && k - 1){
    auto top = q.top();q.pop();
    int r = top.second / columns;
    int c = top.second % columns;
    if(visited[r][c]) continue;
    visited[r][c]=true;
    k--;
    if(c+1 < clumns){
      q.push(make_pair(nums[r][c+1], r*columns + c +1));
    }
    
    if(r+1 < row){
      q.push(make_pair(nums[r+1][c], (r+1)*columns + c));
    }
  }
  return q.top();
}
