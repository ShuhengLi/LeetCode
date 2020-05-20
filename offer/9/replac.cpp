void replace(string s, string source, string target){
  if(target.size() > source.size()){
    int count = 0;
    for(int i = 0; i <= s.size()-source.size();i++){
      string substr = s.substr(i, source.size());
      if(substr == source) count++;
    }
    int j = s.size();
    int resize = s.size() + count * (target.size()- source.size();
    s.resize(resize);

    int i = s.size() -1;
    while(j >=0){
      if(s[j] == source.back()){
        int k = j;
        int t = source.size() -1;
        while(k >= 0 && t >= 0 && s[k] == source[t]){
            k--;
            t--;
        }
        if(t<0){
          j = j - source.size();
          for(int z = target.size() -1; z >=0; z--){
            s[i--] = target[z];
          }
        }else {
          s[i--] = s[j--];
        }
      }else{
        s[i--] = s[j--];
      }
      
    }
  }else{
    int i = 0, j = 0;
    while(j < s.size()){
      string substr = s.substr(j, source.size());
      if(substr !=  source){
        s[i++] = s[j++];
      }else{
        for(auto c : target){
          s[i++] = c;
          j++;
        }
      }
    }
  }

}

