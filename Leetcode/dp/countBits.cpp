/*338. Counting Bits
 * Given a non negative integer number num. For every numbers i in the range 0 ≤ i ≤ num calculate the number of 1's in their binary representation and return them as an array.

Example 1:

Input: 2
Output: [0,1,1]
*/
class Solution {
public:
    vector<int> countBits(int num) {
        long high = 1;
        while(high < num){
            high *=2;
        }
        vector<int> res;
        res.push_back(0);
        if(!num) return res;
        vector<int> last;
        last.push_back(1);
        res.insert(res.end(), last.begin(), last.end());
        while(high >=1){
            high = high / 2;
            vector<int> cur = last;
            cur.insert(cur.end(), last.begin(), last.end());
            for(int i = cur.size()  / 2; i < cur.size(); i++){
                cur[i] += 1;
            }
            res.insert(res.end(), cur.begin(), cur.end());
            last = cur;
        }
        res.resize(num+1);
        return res;
    }
};
