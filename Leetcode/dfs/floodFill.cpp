/*
733. Flood Fill

An image is represented by an m x n integer grid image where image[i][j]
represents the pixel value of the image.

You are also given three integers sr, sc, and color. You should perform a flood
fill on the image starting from the pixel image[sr][sc].

To perform a flood fill, consider the starting pixel, plus any pixels connected
4-directionally to the starting pixel of the same color as the starting pixel,
plus any pixels connected 4-directionally to those pixels (also with the same
color), and so on. Replace the color of all of the aforementioned pixels with
color.

Return the modified image after performing the flood fill.

Example 1:
  Input: image = [[1,1,1],[1,1,0],[1,0,1]], sr = 1, sc = 1, color = 2
  Output: [[2,2,2],[2,2,0],[2,0,1]]
  Explanation: From the center pixel (1,1), all pixels connected by a path of
  the same color as the starting pixel are colored with the new color.

Example 2:
  Input: image = [[0,0,0],[0,0,0]], sr = 0, sc = 0, color = 0
  Output: [[0,0,0],[0,0,0]]
  Explanation: The starting pixel is already colored 0, so no changes are made.

Constraints:
  - m == image.length
  - n == image[i].length
  - 1 <= m, n <= 50
  - 0 <= image[i][j], color < 2^16
  - 0 <= sr < m
  - 0 <= sc < n
*/
// Simple DFS on grid - warm-up for harder grid DFS problems
// Time: O(m*n), Space: O(m*n)
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        if (image[sr][sc] == color) return image;
        dfs(image, sr, sc, image[sr][sc], color);
        return image;
    }

    void dfs(vector<vector<int>>& image, int i, int j, int oldColor, int newColor) {
        if (i < 0 || i >= image.size() || j < 0 || j >= image[0].size()) return;
        if (image[i][j] != oldColor) return;
        image[i][j] = newColor;
        dfs(image, i+1, j, oldColor, newColor);
        dfs(image, i-1, j, oldColor, newColor);
        dfs(image, i, j+1, oldColor, newColor);
        dfs(image, i, j-1, oldColor, newColor);
    }
};
