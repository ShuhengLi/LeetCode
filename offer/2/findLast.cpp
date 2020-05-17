int findLast(vector<int> nums, int x){
  if(nums.empty()) return -1;
  int l = 0;
  int r = nums.size() -1;
  while(l + 1 < r){
    int m = l + (r-l)/2;
    if(nums[m] > x){
      r = m - 1;
    }else if(nums[m] < x){
      l = m + 1;
    }else{
      l = m;
    }
  }
  if(nums[r] == x) return r;
  if(nums[l] == x) return l;
  return -1;
}
