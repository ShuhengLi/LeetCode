class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if(nums.empty() || nums.size() == 1) return nums.size();
        int last = 1;
        int g_max = INT_MIN;
        for(int i = 1; i < nums.size(); i++){
            if(nums[i] > nums[i-1]){
                last = last + 1;
            }else{
                last = 1;
            }
            g_max = max(g_max, last);
        }
        return g_max;
    }
};
