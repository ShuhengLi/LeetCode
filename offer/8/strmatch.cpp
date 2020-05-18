int strmatch(string s1, string s2){
  if(s1.empty() && s2.empty()) return 0;
  if(s1.empty() || s2.empty()) return -1;
  if(s1.size() < s2.size()) return strmatch(s2, s1);
  for (int i = 0; i <= s1.size() - s2.size(); i++){
    for(int j = 0; j < s2.size(); j++){
      if(s2[j] != s1[i+j]) break;
      if(j == s2.size() -1) return i;
    }
  }
  return -1;
}
