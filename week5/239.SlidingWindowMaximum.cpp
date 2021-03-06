/*239. Sliding Window Maximum
 * Given an array nums, there is a sliding window of size k which is moving from the very left of the array to the very right. You can only see the k numbers in the window. Each time the sliding window moves right by one position. Return the max sliding window.

Follow up:
Could you solve it in linear time?

Example:

Input: nums = [1,3,-1,-3,5,3,6,7], and k = 3
Output: [3,3,5,5,6,7]
Explanation:

Window position                Max
---------------               -----
[1  3  -1] -3  5  3  6  7       3
 1 [3  -1  -3] 5  3  6  7       3
 1  3 [-1  -3  5] 3  6  7       5
 1  3  -1 [-3  5  3] 6  7       5
 1  3  -1  -3 [5  3  6] 7       6
 1  3  -1  -3  5 [3  6  7]      7
 */
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        multiset<int> s;
        vector<int> res;
        for(int i = 0; i < k; i++){
            s.insert(nums[i]);
        }
        res.push_back(*(s.rbegin()));
        for(int i = k, slow = 0; i < nums.size(); i++){
            auto it = s.equal_range(nums[slow++]).first;
            s.erase(it);
            s.insert(nums[i]);
            res.push_back(*(s.rbegin()));
        }
        return res;
    }
};
//O(N)
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector <int> ans;
        int n=nums.size();
        deque <int> q; //only indexes are stored
        for(int i=0;i<n;i++)
        {   
            while(!q.empty() && i-q.front()>=k)
                q.pop_front(); //only window size of k is allowed
            while(!q.empty() && nums[q.back()]<nums[i])
                q.pop_back();
            q.push_back(i);
            if(i>=k-1)ans.push_back(nums[q.front()]); //our max value in O(1)
        }
        return ans;
    }
};
