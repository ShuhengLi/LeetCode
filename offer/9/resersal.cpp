void reversal(string s){
  int i = 0, j = s.size() -1;
  while(i<=j){
    swap(s[i++], s[j--]);
  }
}
//recursion
void reversal(string s,int l, int r){
  if(l >= r) return;
  swap(s[l], s[r]);
  reversal(s, l+1, r-1);
}
