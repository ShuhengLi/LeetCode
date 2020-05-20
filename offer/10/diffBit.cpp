// n1 n2 positive 
int diffBit(int n1, int n2){
  int res = 0;
  int diff = n1 ^ n2;
  unsigned bit = 1;
  for(int i = 0; i<32; i++){
    auto temp = diff >> i;
    if(bit & diff){
      res++
    }
  }
  return res;
}
