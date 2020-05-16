class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        quickSort(nums, 0, nums.size()-1);
        return nums;
    }
    void quickSort(vector<int>& nums, int l, int r){
        if(l>=r) return;
        int r_index =  myrand(l, r);
        int partition = helper(nums, l, r_index, r);
        quickSort(nums, l, partition - 1);
        quickSort(nums, partition + 1, r);
    }
    int helper(vector<int>& nums, int l,int index, int r){
        swap(nums[index], nums[r]);
        int i = l;
        int j = r-1;
        while(i <= j){
            if(nums[i]<nums[r]){
                i++;
            }else if( nums[j] >= nums[r]){
                j--;
            }else{
                swap(nums[i++], nums[j--]);
            }
        }
        swap(nums[i], nums[r]);
        return i;
    }
    int myrand(int l, int r){
        int range = r - l + 1;
        return rand()%range + l;
    }
};
