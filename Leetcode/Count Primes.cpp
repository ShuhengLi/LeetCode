/*
Description:

Count the number of prime numbers less than a non-negative number, n.

Credits:
Special thanks to @mithmatt for adding this problem and creating all test cases.

Show Hint

Author: Shuheng Li
Date: 04/27/2016
*/

class Solution {
public:
	int countPrimes(int n) {
		if (n <= 2) return 0;
		vector<bool> passed(n, false);
		int sum = 1;
		int upper = sqrt(n);
		for (int i = 3; i<n; i += 2) {
			if (!passed[i]) {
				sum++;
				//avoid overflow
				if (i>upper) continue;
				for (int j = i*i; j<n; j += i) {
					passed[j] = true;
				}
			}
		}
		return sum;
	}
}; 