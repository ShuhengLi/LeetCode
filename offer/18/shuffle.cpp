void shuffle(vector<int> nums){
  for(int i = 0; i< nums.size(); i++){
    int rand = i + rand()%(nums.size() - i);
    swap(nums[i], nums[rand]);
  }
}
