/*480. Sliding Window Median
 * Median is the middle value in an ordered integer list. If the size of the list is even, there is no middle value. So the median is the mean of the two middle value.

Examples:
[2,3,4] , the median is 3

[2,3], the median is (2 + 3) / 2 = 2.5

Given an array nums, there is a sliding window of size k which is moving from the very left of the array to the very right. You can only see the k numbers in the window. Each time the sliding window moves right by one position. Your job is to output the median array for each window in the original array.

For example,
Given nums = [1,3,-1,-3,5,3,6,7], and k = 3.

Window position                Median
---------------               -----
[1  3  -1] -3  5  3  6  7       1
 1 [3  -1  -3] 5  3  6  7       -1
 1  3 [-1  -3  5] 3  6  7       -1
 1  3  -1 [-3  5  3] 6  7       3
 1  3  -1  -3 [5  3  6] 7       5
 1  3  -1  -3  5 [3  6  7]      6
Therefore, return the median sliding window as [1,-1,-1,3,5,6].
*/
class Solution {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        vector<double> res;
        if(nums.size() < k) return res;
        multiset<int> small;
        multiset<int> large;
        for(int i = 0; i < k ; i++){
            add(small, large, nums[i]);
        }
        res.push_back(getMed(small, large));
        for(int i = k, slow = 0; i < nums.size(); i++){
            if(nums[slow] <= *(small.rbegin())) {
            cout << *(small.rbegin());
                small.erase(small.lower_bound(nums[slow]));//bug3: always make sure small's size bigger than large
                if(!large.empty()){//bug 4: only do it when large is not empty
                    auto it = large.begin();
                    int temp = *(it); large.erase(it);
                    small.insert(temp);
                }
            }
            else large.erase(large.lower_bound(nums[slow]));
            slow++;
            add(small, large, nums[i]);
            res.push_back(getMed(small, large));
        }
        return res;
    }
    double getMed(multiset<int>& s, multiset<int>& l){
        double res;
        bool odd = ((s.size() + l.size()) % 2) ;
        if(odd) res = *(s.rbegin());
        else {
            double a = *(s.rbegin());
            double b = *(l.begin());
            res = (a + b) / 2;//bug2: cast to double
        }
        return res;
    }
    void add(multiset<int>& small, multiset<int>& large, int n){
            if(small.size() == large.size()){
                large.insert(n);
                auto it = large.begin();
                int temp = *(it); large.erase(it);
                small.insert(temp);
            }else{
                small.insert(n);
                auto it = --small.end();//bug 1: cannot erase(*.rbegin()) use --*.end()
                int temp = *(it); small.erase(it);
                large.insert(temp);
            }
    }
};
