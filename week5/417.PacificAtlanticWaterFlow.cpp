/*417. Pacific Atlantic Water Flow
 * Given an m x n matrix of non-negative integers representing the height of each unit cell in a continent, the "Pacific ocean" touches the left and top edges of the matrix and the "Atlantic ocean" touches the right and bottom edges.

Water can only flow in four directions (up, down, left, or right) from a cell to another one with height equal or lower.

Find the list of grid coordinates where water can flow to both the Pacific and Atlantic ocean.

Note:

The order of returned grid coordinates does not matter.
Both m and n are less than 150.


Example:

Given the following 5x5 matrix:

  Pacific ~   ~   ~   ~   ~
       ~  1   2   2   3  (5) *
       ~  3   2   3  (4) (4) *
       ~  2   4  (5)  3   1  *
       ~ (6) (7)  1   4   5  *
       ~ (5)  1   1   2   4  *
          *   *   *   *   * Atlantic

Return:

[[0, 4], [1, 3], [1, 4], [2, 2], [3, 0], [3, 1], [4, 0]] (positions with parentheses in above matrix).*/
vector<vector<int>> pacificAtlantic(vector<vector<int>>& matrix) {
	if(matrix.empty()) return {};
	vector<vector<int>> res, seen(matrix.size(), vector<int>(matrix[0].size(), 0));

	// Pacific pass
	for(int i=0; i<matrix.size(); i++)
		dfs(matrix, seen, 1, 0, i, 0, res);
	for(int j=0; j<matrix[0].size(); j++)
		dfs(matrix, seen, 1, 0, 0, j, res);

	// Atlantic pass
	for(int i=0; i<matrix.size(); i++)
		dfs(matrix, seen, 2, 0, i, matrix[0].size()-1, res);
	for(int j=0; j<matrix[0].size(); j++)
		dfs(matrix, seen, 2, 0, matrix.size()-1, j, res);


	return res;
}

void dfs(vector<vector<int>>& grid, vector<vector<int>>& seen, int val, int prev, int i, int j, vector<vector<int>>& res){

	if(i<0||j<0||i==grid.size()||j==grid[0].size()||seen[i][j]==val||grid[i][j]<prev)
		return;

	if(seen[i][j]!=0)          //for "Atlantic" pass only
		res.push_back({i,j});

	seen[i][j]=val;

	dfs(grid, seen, val, grid[i][j], i+1, j, res);
	dfs(grid, seen, val, grid[i][j], i-1, j, res);
	dfs(grid, seen, val, grid[i][j], i, j+1, res);
	dfs(grid, seen, val, grid[i][j], i, j-1, res);

}
