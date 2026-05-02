/*
 * 42. Trapping Rain Water
 * Hard
 * [Atlassian DSA - Medium Frequency, classic Hard]
 *
 * Given n non-negative integers representing an elevation map where the width
 * of each bar is 1, compute how much water it can trap after raining.
 *
 * Example:
 *   [0,1,0,2,1,0,1,3,2,1,2,1] -> 6
 *   [4,2,0,3,2,5]             -> 9
 *
 * ─────────────────────────────────────────────────────────────────
 * Key insight: water above index i = min(maxLeft[i], maxRight[i]) - height[i]
 *
 * Approach 1: prefix/suffix max arrays (clearest)
 *   - leftMax[i]  = max(height[0..i])
 *   - rightMax[i] = max(height[i..n-1])
 *   - sum(min(leftMax[i], rightMax[i]) - height[i])
 *   Time O(n), Space O(n)
 *
 * Approach 2: two pointers (O(1) space) — interview gold standard
 *   - Maintain leftMax, rightMax while two pointers converge.
 *   - At each step, the side with smaller max determines water level.
 *   Time O(n), Space O(1)
 *
 * Approach 3: monotonic stack (counts water in horizontal layers)
 *   - When current bar is taller than stack top, pop and compute trapped layer.
 *   Time O(n), Space O(n)
 * ─────────────────────────────────────────────────────────────────
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Approach 2: two pointers (recommended)
    int trap(vector<int>& height) {
        int l = 0, r = (int)height.size() - 1;
        int leftMax = 0, rightMax = 0;
        int water = 0;

        while (l < r) {
            if (height[l] < height[r]) {
                // Left side is the bottleneck — water level fixed by leftMax
                if (height[l] >= leftMax) leftMax = height[l];
                else water += leftMax - height[l];
                ++l;
            } else {
                if (height[r] >= rightMax) rightMax = height[r];
                else water += rightMax - height[r];
                --r;
            }
        }
        return water;
    }

    // Approach 1: prefix arrays
    int trapPrefix(vector<int>& height) {
        int n = height.size();
        if (n < 3) return 0;
        vector<int> leftMax(n), rightMax(n);
        leftMax[0] = height[0];
        for (int i = 1; i < n; ++i) leftMax[i] = max(leftMax[i-1], height[i]);
        rightMax[n-1] = height[n-1];
        for (int i = n-2; i >= 0; --i) rightMax[i] = max(rightMax[i+1], height[i]);
        int water = 0;
        for (int i = 0; i < n; ++i) water += min(leftMax[i], rightMax[i]) - height[i];
        return water;
    }

    // Approach 3: monotonic stack
    int trapStack(vector<int>& height) {
        stack<int> st;
        int water = 0;
        for (int i = 0; i < (int)height.size(); ++i) {
            while (!st.empty() && height[i] > height[st.top()]) {
                int bottom = st.top(); st.pop();
                if (st.empty()) break;
                int distance = i - st.top() - 1;
                int boundedHeight = min(height[i], height[st.top()]) - height[bottom];
                water += distance * boundedHeight;
            }
            st.push(i);
        }
        return water;
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: 2D version (LC 407 Trapping Rain Water II)?
 * A: BFS from boundary cells using min-heap; expand to neighbors using
 *    max(current_level, neighbor_height). Time O(mn log(mn)).
 *
 * Q: Can you solve in O(1) extra space and O(n)?
 * A: Yes — Approach 2 (two pointers).
 *
 * Q: How to verify? Why does two-pointer work?
 * A: At any step the smaller side's max bounds the water level. Moving the
 *    pointer on the smaller side is safe because increasing it can only
 *    raise its max (or stay), never beyond the other side's known max.
 */

int main() {
    Solution sol;
    vector<int> h1 = {0,1,0,2,1,0,1,3,2,1,2,1};
    cout << sol.trap(h1) << endl;        // 6
    cout << sol.trapPrefix(h1) << endl;  // 6
    cout << sol.trapStack(h1) << endl;   // 6

    vector<int> h2 = {4,2,0,3,2,5};
    cout << sol.trap(h2) << endl;        // 9
    return 0;
}
