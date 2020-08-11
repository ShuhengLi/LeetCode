/*1438. Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit
 * Given an array of integers nums and an integer limit, return the size of the longest non-empty subarray such that the absolute difference between any two elements of this subarray is less than or equal to limit.



Example 1:

Input: nums = [8,2,4,7], limit = 4
Output: 2
Explanation: All subarrays are:
[8] with maximum absolute diff |8-8| = 0 <= 4.
[8,2] with maximum absolute diff |8-2| = 6 > 4.
[8,2,4] with maximum absolute diff |8-2| = 6 > 4.
[8,2,4,7] with maximum absolute diff |8-2| = 6 > 4.
[2] with maximum absolute diff |2-2| = 0 <= 4.
[2,4] with maximum absolute diff |2-4| = 2 <= 4.
[2,4,7] with maximum absolute diff |2-7| = 5 > 4.
[4] with maximum absolute diff |4-4| = 0 <= 4.
[4,7] with maximum absolute diff |4-7| = 3 <= 4.
[7] with maximum absolute diff |7-7| = 0 <= 4.
Therefore, the size of the longest subarray is 2.
Example 2:

Input: nums = [10,1,2,4,7,2], limit = 5
Output: 4
Explanation: The subarray [2,4,7,2] is the longest since the maximum absolute diff is |2-7| = 5 <= 5.
*/
class Solution {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        int n = nums.size();
        multiset<int> s;
        multiset<int> temp;
        int res = 0;

            for(int i = 0; i < n; i++){
                s.insert(nums[i]);
            }
            if(*(s.rbegin()) - *(s.begin()) <= limit) return n;
        temp=s;
        for(int len = n-1; len >= 1; len--){
            s = temp;
            s.erase(s.lower_bound(nums[slow++]));
            for(int i = len, slow = 0; i  < n; i++){
                s.erase(s.lower_bound(nums[slow++]));
                s.insert(nums[i]);
                if(*(s.rbegin()) - *(s.begin()) <= limit) return len;
            }
        }
        return res;
    }
};

class Solution {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        int start = 0, end = 0, ans = 0, n = nums.size();
        deque<int> inc_q, dec_q;
        while(end<n)
        {
            while(!dec_q.empty() && nums[dec_q.back()]<=nums[end]) dec_q.pop_back(); dec_q.push_back(end);
            while(!inc_q.empty() && nums[inc_q.back()]>=nums[end]) inc_q.pop_back(); inc_q.push_back(end);

            while(!dec_q.empty() && !inc_q.empty() && (nums[dec_q.front()] - nums[inc_q.front()])>limit)
            {
                start++;
                while(!dec_q.empty() && dec_q.front()<start) dec_q.pop_front();
                while(!inc_q.empty() && inc_q.front()<start) inc_q.pop_front();

            }
            ans = max(ans, end - start + 1);
            end++;

        }
        return ans;

    }
};

