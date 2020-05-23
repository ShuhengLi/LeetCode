/*Given a integer n, print out all possible if block for it
 if{
  if{// 2 space before the inner line
  }
}
*/
vector<vector<string>>  res;
map<int, string> m;
vector<vector<string>> block(int n){
  vector<string> cur;
  m[0]= "if{\n"
  m[1]= "}\n"
  block(n, 0, 0, cur);
  return res;
}
void block(int n, int l, int r, vector<string>& cur){
  if(l + r == 2*n){
    res.push_back(cur);
    return;
  }
  int diff = l - r;

  if(l < n){
    string temp = "";
    for(int i = 0; i< diff; i++){
      temp = "  " + temp;
    }
    cur.push_back(temp + m[0]);
    block(n, l+1, r, cur);
    cur.pop_back();
  }
  if(r < l){
    string temp = "";
    for(int i = 0; i< diff-1; i++){
      temp = "  " + temp;
    }
    cur.push_back(temp + m[1]);
    block(n, l, r+1, cur);
    cur.pop_back();
  }
}
