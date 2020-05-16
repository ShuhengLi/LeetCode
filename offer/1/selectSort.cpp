class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        for(int i = 0; i < nums.size(); i++){
            for(int j = i; j < nums.size(); j++){
                if(nums[j] < nums[i]){
                    swap(nums[i], nums[j]);
                }
            }
        }
        return nums;
    }
};

//trach index
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        for(int i = 0; i < nums.size(); i++){
            int global = i
            for(int j = i; j < nums.size(); j++){
                if(nums[j] < nums[global]){
                  global = j;
                }
            }
            swap(nums[global], nums[i];
        }
        return nums;
    }
};
