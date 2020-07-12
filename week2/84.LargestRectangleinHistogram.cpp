/*84. Largest Rectangle in Histogram
Given n non-negative integers representing the histogram's bar height where the width of each bar is 1, find the area of largest rectangle in the histogram.

Example:

Input: [2,1,5,6,2,3]
Output: 10
*/
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int res = 0;
        int n = heights.size();
        if(n == 0) return res;
        heights.push_back(0);
        stack<int> s;
        for(int i = 0; i< heights.size(); i++){
            if(s.empty() || heights[i] > heights[s.top()]){
                s.push(i);
                continue;
            }else{
                while(!s.empty() && heights[i] < heights[s.top()]){
                    int index = s.top(); s.pop();
                    int h = heights[index];
                    int r = i;
                    int l = s.empty()? 0: (s.top() +1);
                    auto cur = h *(r-l);
                    res = max(res, cur);
                }
            }
            s.push(i);
        }
        return res;

    }
};
