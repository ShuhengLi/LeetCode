//____a____b___c____ => a_b_c
void removeSpace(string s){
  int i = 0, j = 0;
  int count = 0;
  while(j < s.size()){
    while(j < s.size() && s[j] == ' '){
      j++;
    }
    if(j==s.size()) break;
    if(count) s[i++] = ' ';
    while(j < s.size() && s[j] != ' '){
      s[i++] = s[j++];
    }
    count++;
  }
}
