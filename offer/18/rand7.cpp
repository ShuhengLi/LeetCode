int rand7(){
  int row = rand5();
  int col = rand5();
  int res = row * 5 + col;
  if(res >= 21){
    res = rand7();
  }
  return res;
}

int rand1000000(){
  int dimensen = log5^n
}
