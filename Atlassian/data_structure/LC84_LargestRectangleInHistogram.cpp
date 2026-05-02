/*
 * 84. Largest Rectangle in Histogram
 * Hard
 *
 * Given an array of integers heights representing the histogram's bar height where
 * the width of each bar is 1, return the area of the largest rectangle in the histogram.
 *
 * Example 1:
 * Input: heights = [2,1,5,6,2,3]
 * Output: 10
 * Explanation: The largest rectangle has an area = 10 (from heights[2] and heights[3])
 *
 * Example 2:
 * Input: heights = [2,4]
 * Output: 4
 *
 * Constraints:
 * - 1 <= heights.length <= 10^5
 * - 0 <= heights[i] <= 10^4
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Monotonic stack - for each bar, find the largest rectangle with that bar as the shortest
    // Time: O(n), Space: O(n)
    int largestRectangleArea(vector<int>& heights) {
        int maxArea = 0;
        stack<int> st; // stores indices

        for (int i = 0; i <= (int)heights.size(); i++) {
            int h = (i == (int)heights.size()) ? 0 : heights[i];

            while (!st.empty() && h < heights[st.top()]) {
                int height = heights[st.top()];
                st.pop();
                int width = st.empty() ? i : i - st.top() - 1;
                maxArea = max(maxArea, height * width);
            }
            st.push(i);
        }

        return maxArea;
    }
};
