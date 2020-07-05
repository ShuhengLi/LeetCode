//abcdef  a3d2 return true;
bool abbrevMatch(string s1, string s2, int start1, int start2){
  if(s1.empty() && s2.empty()) return true;
  if(s1.empty() || s2.empty()) return false;

  if(isdigit(s2[start2]){
      int  i = start2; 
      int  num = 0; 

      while(i < s2.size() && isdigit(s2[i]) ){
        num = num * 10 + s2[i] - '0';
        i++;
     }
     if(num > s1.size()) return false;
     return abbrevmatch(s1, s2, start1 + num, start2 + i);
  }else{
    if(s1[start1] !== s2[start2]) return false;
    return abbrevmatch(s1, s2, start1+1, start2+1);
  }
}
