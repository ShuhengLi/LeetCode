void convertor(string s, int l, int r){
  if(l+1 <=r){
    return;
  }
  int size = r - l + 1;
  int m = l + size / 2;
  int l_m = l + size / 4;
  int r_m = l + size / 4 * 3;
  int old_m = l + (r - l) / 2;
  reverse(l_m , r_m - 1);
  reverse(l_m , m - 1);
  reverse(m , r_m - 1);

  convertor(s, l, old_m);
  convertor(s, old_m + 1, r);

}
