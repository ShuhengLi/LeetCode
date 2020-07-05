//check duplicate chars
bool isUnique(string s){
  if(s.empty()) return true;
  unsigned dic = 0;
  for(auto c:s){
    int pos = c - 'a';
    if((dic>>pos)&1==1){
      return false;
    }else{
      dic = dic | ( 1 << pos);
    }
  }
  return true;
}
