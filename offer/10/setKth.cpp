//set kth to 1
void setKth(int n, int k){
  unsigned bit_set = 1;
  bit_set = bit_set << k;
  n = n | bit_set;
}
//set kth to 0
void setKth(int n, int k){
  unsigned bit_set = 1;
  bit_set = bit_set << k;
  bit_set = ~bit_set;
  n = n & bit_set;
}
