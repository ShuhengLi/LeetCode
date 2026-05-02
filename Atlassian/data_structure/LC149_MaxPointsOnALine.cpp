/*
 * 149. Max Points on a Line
 * Hard
 *
 * Given an array of points where points[i] = [xi, yi] represents a point on the X-Y plane,
 * return the maximum number of points that lie on the same straight line.
 *
 * Example 1:
 * Input: points = [[1,1],[2,2],[3,3]]
 * Output: 3
 *
 * Example 2:
 * Input: points = [[1,1],[3,2],[5,3],[4,1],[2,3],[1,4]]
 * Output: 4
 *
 * Constraints:
 * - 1 <= points.length <= 300
 * - points[i].length == 2
 * - -10^4 <= xi, yi <= 10^4
 * - All the points are unique
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // For each point, calculate slope to all other points
    // Use GCD to represent slope as reduced fraction to avoid floating point issues
    // Time: O(n^2), Space: O(n)
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        if (n <= 2) return n;

        int maxCount = 2;

        for (int i = 0; i < n; i++) {
            unordered_map<string, int> slopeCount;
            for (int j = i + 1; j < n; j++) {
                int dx = points[j][0] - points[i][0];
                int dy = points[j][1] - points[i][1];

                // Normalize the slope representation
                int g = gcd(abs(dx), abs(dy));
                if (g != 0) {
                    dx /= g;
                    dy /= g;
                }
                // Ensure consistent sign: dx is positive, or if dx==0, dy is positive
                if (dx < 0 || (dx == 0 && dy < 0)) {
                    dx = -dx;
                    dy = -dy;
                }

                string key = to_string(dx) + "/" + to_string(dy);
                slopeCount[key]++;
            }

            for (auto& [key, count] : slopeCount) {
                maxCount = max(maxCount, count + 1); // +1 for point i itself
            }
        }

        return maxCount;
    }

private:
    int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }
};
