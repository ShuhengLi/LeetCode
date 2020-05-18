/*349. Intersection of Two Arrays
 * Given two arrays, write a function to compute their intersection.

Example 1:

Input: nums1 = [1,2,2,1], nums2 = [2,2]
Output: [2]
*/
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
        set<int> s;
        for(auto n:nums1){
            s.insert(n);
        }
        for(auto n:nums2){
            if(s.find(n)!=s.end()){
                res.push_back(n);
                s.erase(n);
            }
        }
        return res;
    }
};


//With dupilicate 
/*350. Intersection of Two Arrays II
Given two arrays, write a function to compute their intersection.

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
        vector<int> res;
        map<int, int>m;
        for(auto n:nums1){
            m[n]++;
        }
        for(auto n:nums2){
            if(m.find(n)!= m.end() && m[n] > 0){
                res.push_back(n);
                m[n]--;
            }
        }
        return res;
    }
};

//sorted array
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
