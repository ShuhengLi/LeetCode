/*42. Trapping Rain Water
 * Given n non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it is able to trap after raining.


The above elevation map is represented by array [0,1,0,2,1,0,1,3,2,1,2,1]. In this case, 6 units of rain water (blue section) are being trapped. Thanks Marcos for contributing this image!

Example:

Input: [0,1,0,2,1,0,1,3,2,1,2,1]
Output: 6
*/
*/
class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        if(!n) return 0;
        vector<int> l(n, 0);
        vector<int> r(n, 0);
        l[0] = height[0];
        r[n-1] = height[n-1];
        for(int i = 1; i < n; i++){
            l[i] = max(l[i-1], height[i]);
        }
        for(int i = n-2; i >= 0; i--){
            r[i] = max(r[i+1], height[i]);
        }
        int ans = 0;
        for(int i = 1; i < n-1; i++){
            ans += min(l[i], r[i]) - height[i];
        }
        return ans;
    }
};
