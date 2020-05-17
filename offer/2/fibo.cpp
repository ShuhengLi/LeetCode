int fibo(int n){
  if (n ==0 || n == 1) return n;
  return fibo(n-2) + fibo(n-1);
}
//time: O(2^n)
