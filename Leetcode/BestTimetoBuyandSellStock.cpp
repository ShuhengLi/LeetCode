/*
Say you have an array for which the ith element is the price of a given stock on day i.

If you were only permitted to complete at most one transaction (ie, buy one and sell one share of the stock), design an algorithm to find the maximum profit.

Author: Shuheng Li
*/

class Solution {
public:
	int maxProfit(vector<int>& prices) {
		int minv = INT_MAX;
		int maxPro = 0;
		for (int i = 0; i< prices.size(); i++) {
			minv = min(minv, prices[i]);
			maxPro = max(maxPro, prices[i] - minv);
		}
		return maxPro;
	}
};