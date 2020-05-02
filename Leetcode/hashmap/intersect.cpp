/*Given two arrays, write a function to compute their intersection.

Example 1:

Input: nums1 = [1,2,2,1], nums2 = [2,2]
Output: [2,2]
Example 2:

Input: nums1 = [4,9,5], nums2 = [9,4,9,8,4]
Output: [4,9]
*/
class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        int size1 = nums1.size();
        int size2 = nums2.size();
        vector<int> res;
        unordered_map<int, int> m;
        for(auto num : nums2) {
            m[num]++;
        }
        for(auto num : nums1){
            if(m.find(num) != m.end() && m[num] > 0){
                res.push_back(num);
                m[num]--;
            }
        }
        return res;
    }
};
//Time: O(N)
//Space: O(N)
//folow up if sorted

class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
        sort(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());
        int i = 0;
        int j = 0;
        while(i < nums1.size() && j < nums2.size()){
            if(nums1[i] < nums2[j]){
                i++;
            }else if (nums1[i] > nums2[j]){
                j++;
            }else{
                res.push_back(nums1[i]);
                i++;
                j++;
            }
        }
        return res;
    }
};
//Time: O(N)
//Space: O(1)
