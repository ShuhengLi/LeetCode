//left big, right big
class Solution {
  public:
    int maxproduct(int  length) {
      if(!length || length == 1) return 0;
      vector<int> dp(length.size() + 1, 0);

      for(int i = 2; i < length;i++){
        for(int j = 1; j <= i/2; j++){
          int cur = max(dp[j], j) * max(dp[i - j], i - j);
          dp[i] = max(cur, dp[i]);
        }
      }
      return dp[length];
    }
};
//Time:O(n^2)


//left big, right small
class Solution {
  public:
    int maxproduct(int  length) {
      if(!length || length == 1) return 0;
      vector<int> dp(length.size() + 1, 0);

      for(int i = 2; i < length;i++){
        for(int j = 1; j < i; j++){
          int cur = max(dp[j], j) *  (i - j);
          dp[i] = max(cur, dp[i]);
        }
      }
      return dp[length];
    }
};

//recursion
class Solution {
  public:
    int maxProduct(int  n) {
      if(!n || n== 1) return 0;
      maxP = 0;

      for(int i = 1; i < length;i++){
        int best = max(n - i, maxProduct(n-i));
        maxp = max(maxp, best * i);
      }
      return maxp;
    }
};
