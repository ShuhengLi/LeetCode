/*349. Intersection of Two Arrays
 * Given two arrays, write a function to compute their intersection.

Example 1:

Input: nums1 = [1,2,2,1], nums2 = [2,2]
Output: [2]
Example 2:

Input: nums1 = [4,9,5], nums2 = [9,4,9,8,4]
Output: [9,4]
*/
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        vector<int>  res;
        set<int> s;
        for( auto n: nums1){
            s.insert(n);
        }
        for(auto n:nums2){
            if(s.find(n) != s.end()){
                res.push_back(n);
                s.erase(n);
            }
        }
        return res;;
    }
};
