/* 461. Hamming Distance
Add to List

Share
The Hamming distance between two integers is the number of positions at which the corresponding bits are different.

Given two integers x and y, calculate the Hamming distance.

Note:
0 ≤ x, y < 231.

Example:

Input: x = 1, y = 4

Output: 2

Explanation:
1   (0 0 0 1)
4   (0 1 0 0)
       ↑   ↑
*/
class Solution {
public:
    int hammingDistance(int x, int y) {
        unsigned bit = 1;
        int res = 0;
        for(int i = 0; i < 32; i++){
            auto a = x >> i;
            auto b = y >> i;
            if((a & bit) != (b & bit)){
                res++;
            }
        }
        return res;
    }
};

