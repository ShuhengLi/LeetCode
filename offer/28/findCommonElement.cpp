/find commom element in 2 array
//
//assuption 1: sorted
//Solution 1: 2 pointer
vector<int> findCommon(string s1, string s2){
  vector<int> res;
  int i = 0, j = 0;
  while(i < s1.size() && j < s2.size()){
    if(s1[i] == s2[j]){
      res.push_back(s1[i]);
      i++;
      j++;
    }else if(s1[i] < s2[j]){
      i++;
    }else{
      j++;
    }
  }
  return res;
}
//time:O(m+n)
//
//Solution 2:set
vector<int> findCommon(string s1, string s2){
  if(s1.size() < s2.size()) return findCommon(s2, s1);
  map<int, int> m;
  for(auto n:s2){
    m[n]++;
  }
  vector<int> res;
  for(auto n:s1){
    if(m.find(n) != m.end() && m[n] > 0){
      res.push_back(n);
    }
    m[n]--;
  }
  return res;
}
//Solution 3:binary search
//Question 1.2: 3 sorted array
//solution 1: 3 pointer move the smallest or move 2 smallest
//solution 2: result1 and s3
//Question 1.3: find common elements k sorted array
//solution 1: min heap of size k O(k*n*log(K))
//solution 2: binary reduction O(kn)
//solution 3: iterative reduction O(kn)

