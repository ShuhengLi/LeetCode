/*518. Coin Change 2
 * You are given coins of different denominations and a total amount of money. Write a function to compute the number of combinations that make up that amount. You may assume that you have infinite number of each kind of coin.



Example 1:

Input: amount = 5, coins = [1, 2, 5]
Output: 4
Explanation: there are four ways to make up the amount:
5=5
5=2+2+1
5=2+1+1+1
5=1+1+1+1+1
Example 2:

Input: amount = 3, coins = [2]
Output: 0
Explanation: the amount of 3 cannot be made up just with coins of 2.
Example 3:

Input: amount = 10, coins = [10]
Output: 1
*/
//DFS exceed time limit
class Solution {
public:
    int res;
    int change(int amount, vector<int>& coins) {
        if(coins.empty() && amount == 0) return 1;
        if(coins.empty() ) return 0;
        res = 0;
        helper(amount, coins, coins.size()-1);
        return res;
    }
    void helper(int amount, vector<int>& coins, int index) {
        if(index == 0){
            if(amount % coins[index] == 0) res +=1;
            return;
        }
        for(int i = 0; i <= amount / coins[index]; i++){
            int cur = coins[index] * i;
            helper(amount - cur, coins, index - 1);
        }
    }
};
//DP 
