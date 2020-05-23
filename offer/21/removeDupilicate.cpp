//everything on the left of the slow, excluded slow
int removeDuplicate(vector<int> nums){
  if(nums.empty() || nums.size() ==1) return nums.size();
  int slow = 1;
  for(int i = 1; i < nums.size(); i++){
    if(nums[i] != nums[slow - 1]){
      nums[slow++] = nums[i];
    }
  }
  return slow;
}
//everything on the left of the slow, include slow
int removeDuplicate(vector<int> nums){
  if(nums.empty() || nums.size() ==1) return nums.size();
  int slow = 0;
  for(int i = 1; i < nums.size(); i++){
    if(nums[i] != nums[slow]){
      slow++;
      nums[slow] = nums[i];
    }
  }
  return slow+1;
}
//keep at most k duplicate excluding slow
int removeDuplicate(vector<int> nums, int k){
  if(nums.empty() || nums.size() <= k) return nums.size();
  int slow = k;
  for(int i = k; i < nums.size(); i++){
    if(nums[i] != nums[slow - k]){
      nums[slow++] = nums[i];
    }
  }
  return slow;
}

//Keep no duplicate at all
int removeDuplicate(vector<int> nums){
  if( nums.size() <= 1) return nums.size();
  int slow = 0;
  int last = 0;
  for(int i = 1; i < nums.size(); i++){
    if(nums[i] != nums[last]){
      if(i - fast == 1){
        nums[slow++] = nums[i];
      }else{
        last = i;
      }
    }
  }
  return slow;
}

//remove adjacent letters repeatedly
int removeDuplicate(string s){
  if( s.size() <= 1) return s.size();
  int slow = 1;
  int fast = 1;
  while(fast < s.size()){
    if(slow == 0 || s[slow-1] != s[fast]) {
      s[slow++] = s[fast++];
    }else{
      while(fast < s.size() && s[fast] == s[slow-1]){
        fast++;
      }
      slow--;
    }
  }
  s.resize(slow);
  return s;
}

//move zeroes to end, this method will mess up the order
void move(vector<int> nums){
  if( nums.size() <= 1) return; 
  int i = 0; 
  int j = nums.size() - 1;
  while(i <= j){
    if(nums[i]){
      i++;
    }else if(!nums[j]){
      j--;
    }else{
      swap(nums[i++], nums[j--]);
    }
  }
  return;
}

//move zeroes to end with original order
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        if( nums.size() <= 1) return;
        int i = 0;
        for(int j = 0; j < nums.size(); j++){
            if(nums[j]) nums[i++] = nums[j];
        }
        for(int j = i; j < nums.size(); j++){
            nums[j] = 0;
        }
        return;
    }
};
