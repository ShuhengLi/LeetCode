void wordReverse(string s){
  reverse(s, 0, s.size() -1);
  int i = 0, j = 0;
  while(j < s.size()){
    while(j < s.size() && s[j]!=' '){
      j++;
    }
    reverse(s, i, j-1);
    j++;
    i = j;
  }
  return s;
}
void reverse(string&s, int l, int r){
  if(l>=r) return;
  swap(s[l], s[r]);
  reverse(s, l+1, r-1);
}
