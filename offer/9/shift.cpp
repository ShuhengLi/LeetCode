void shift(string s, int x){
  reverse(s.begin(), s.end());
  auto it = s.begin();
  reverse(it, it + x); 
  reverse(it + x s.end()); 
}
