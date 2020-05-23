int trackN(int n){
  int num = rand%n;
  if(num == n - 1){
    return n;
  }
}

//track  k out of n logs
int trackN(int n, int k){
  int num = rand%n;
  if(num <= k){
    solu[num] = s[k];
    return n;
  }
}

//return random largest number's index
int trackN(vector<int> nums){
  int g_max = INT_MIN;
  int count = 0;
  int res = 0;
  for(auto n:nums){
    if(n > g_max){
      g_max = n;
      cout = 1;
      res = index;
    }else if(n == g_max){
      count++;
      int rand_n = rand()%count;
      if(rand_n == count-1){
        res = index;
      }
    }
  }
  return res
}
