/*403. Frog Jump
 * A frog is crossing a river. The river is divided into x units and at each unit there may or may not exist a stone. The frog can jump on a stone, but it must not jump into the water.

Given a list of stones' positions (in units) in sorted ascending order, determine if the frog is able to cross the river by landing on the last stone. Initially, the frog is on the first stone and assume the first jump must be 1 unit.

If the frog's last jump was k units, then its next jump must be either k - 1, k, or k + 1 units. Note that the frog can only jump in the forward direction.
Example 1:

[0,1,3,5,6,8,12,17]

There are a total of 8 stones.
The first stone at the 0th unit, second stone at the 1st unit,
third stone at the 3rd unit, and so on...
The last stone at the 17th unit.

Return true. The frog can jump to the last stone by jumping
1 unit to the 2nd stone, then 2 units to the 3rd stone, then
2 units to the 4th stone, then 3 units to the 6th stone,
4 units to the 7th stone, and 5 units to the 8th stone.
*/
class Solution {
public:
    bool canCross(vector<int>& stones) {
        int n = stones.size();
        if(!n) return false;
        vector<set<int>> dp(n);
        dp[0].insert(1);
        for(int i = 1; i < n; i++){
            for(int j = 0; j < i; j++){
                int diff = stones[i] - stones[j];
                if(dp[j].find(diff)!=dp[j].end()){
                    dp[i].insert(diff-1);
                    dp[i].insert(diff);
                    dp[i].insert(diff+1);
                }
            }
        }
        return !dp[n-1].empty();
    }
};
