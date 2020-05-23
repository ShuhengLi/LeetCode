vector<vecot<int>> res;
vector<vecot<int>> NQueen(int n){
  vector<int> cur(n, -1);
  NQueen(n, 0, cur);
  return res;
}

void NQueen(int n,int level, vector<int> cur){
  if(level == n){
    res.push_back(cur);
    return
  }
  for(int i = 0; i< n ;i++){
    cur[level] = i;

    if(valid(cur, level)){
      NQueen(n, level + 1, cur);
    }
  }
}

bool valid(vector<int> nums, int level){
  int cur = nums[level];
  for(int i = 0; i< level; i++){
    if(nums[i] == cur) return false;
    int diff = level - i;
    if(nums[i] + diff < nums.size() && nums[i] + diff == cur)return false;
    if(nums[i] - diff >=  0 && nums[i] - diff == cur)return false;
  }
  return true;
}
