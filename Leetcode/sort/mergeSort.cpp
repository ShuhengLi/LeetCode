class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        if(nums.empty()) return nums;
        return mergeSort(nums, 0, nums.size() - 1);
    }
    vector<int> mergeSort(vector<int>& nums, int l, int r){
        vector<int> res;
        if(l >= r){
            res.push_back(nums[l]);
            return res;
        }
        int m = l + (r - l)/2;
        auto left = mergeSort(nums, l, m);
        auto right = mergeSort(nums, m + 1, r);
        res = combine(left, right);
        return res;
    }
    vector<int> combine(vector<int>& a, vector<int>& b){
        vector<int> res;
        int i = 0; 
        int j = 0; 
        while(i < a.size() && j < b.size()){
            if(a[i] <= b[j]){
                res.push_back(a[i++]);
            }else{
                res.push_back(b[j++]);
            }
        }
        while(i < a.size()){
            res.push_back(a[i++]);
        }
        while(j < b.size()){
            res.push_back(b[j++]);
        }
        return res;
    }
};

// return void
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        if(nums.empty()) return nums;
        mergeSort(nums, 0, nums.size() - 1);
        return nums;
    }
    void mergeSort(vector<int>& nums, int l, int r){
        if(l >= r){
            return;
        }
        int m = l + (r - l)/2;
        mergeSort(nums, l, m);
        mergeSort(nums, m + 1, r);
        combine(nums, l, m, r);
        return;
    }
    void combine(vector<int>& nums, int l, int m, int r){
        vector<int> res;
        int i = l; 
        int j = m+1; 
        while(i <= m && j <= r){
            if(nums[i] <= nums[j]){
                res.push_back(nums[i++]);
            }else{
                res.push_back(nums[j++]);
            }
        }
        while(i <= m){
            res.push_back(nums[i++]);
        }
        while(j <= r){
            res.push_back(nums[j++]);
        }
        for(int i = 0; i < res.size(); i++){
            nums[l + i] = res[i];
        }
        return;
    }
};
