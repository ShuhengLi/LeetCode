int largestSumSubArray.cpp(vector<int> nums ){
  if(nums.empty()) return 0;
  int start = 0;
  int res_s = 0;
  int res_e = 0;
  int last = nums[0];
  int res  = nums[0];
  for(int i = 1; i < nums.size(); i++){
    if(last < 0){
      last = nums[i];
      start = i;
    }else{
      last = nums[i] + last;
    }

    if(last > res){
      res = last;
      res_s = start;
      res_e = i;
    }
  }
  return res;
}
