/*56. Merge Intervals
 * Given a collection of intervals, merge all overlapping intervals.

Example 1:

Input: intervals = [[1,3],[2,6],[8,10],[15,18]]
Output: [[1,6],[8,10],[15,18]]
Explanation: Since intervals [1,3] and [2,6] overlaps, merge them into [1,6].
Example 2:

Input: intervals = [[1,4],[4,5]]
Output: [[1,5]]
Explanation: Intervals [1,4] and [4,5] are considered overlapping.
*/
class Solution {
public:
    static bool mycompare(vector<int> a, vector<int> b){
        return a[0] < b[0];
    }
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        vector<vector<int>> res;
        int n = intervals.size();
        if(!n) return res;
        sort(intervals.begin(), intervals.end(), mycompare);
        res.push_back(intervals[0]);
        for(auto cur:intervals){
            auto pre = res.back();
            if(cur[0] <= pre[1]){
                res.back()[1] = max(cur[1], pre[1]);
            }else{
                res.push_back(cur);
            }
        }
        return res;
    }
};
