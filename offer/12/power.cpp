long power(int a, int b){
  if(a==0) return 0;
  if(b==0) return 1;
  int flag = b < 0;
  double res = helper(a, b);
  if(flag && res) res = 1 / res;
  return res;
}
doubl helper(a, b){
  if(b==1 || b== -1){
    return a;
  }
  auto half = helper(a, b/2);
  double res = half * half;
  if(b%2) res = res * a;
  return res;
}
