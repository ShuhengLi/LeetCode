/*932. Beautiful Array
 * For some fixed N, an array A is beautiful if it is a permutation of the integers 1, 2, ..., N, such that:

For every i < j, there is no k with i < k < j such that A[k] * 2 = A[i] + A[j].

Given N, return any beautiful array A.  (It is guaranteed that one exists.)



Example 1:

Input: 4
Output: [2,1,4,3]
Example 2:

Input: 5
Output: [3,1,2,5,4]
*/
class Solution {
public:
    map<int, vector<int>>m;
    vector<int> beautifulArray(int N) {
        return helper(N);
    }
    vector<int> helper(int n) {
        if(m.find(n) != m.end()){
            return m[n];
        }

        vector<int> cur(n);
        if(n == 1){
            cur[0] = 1;
        }else{
            int index = 0;
            for(int i : helper((n+1) / 2)){
                cur[index++] = 2 * i - 1;
            }
            for(int i: helper((n/ 2))){
                cur[index++] = 2 * i;
            }
        }
        m[n] = cur;
        return cur;
    }
};
