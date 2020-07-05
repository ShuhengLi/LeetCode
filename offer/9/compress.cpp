//abbbccc => ab3c3
class Solution {
public:
    int compress(vector<char>& chars) {
        if(chars.size() <= 1) return chars.size();
        int i = 1; 
        int count = 1;
        int number = 0;
        for(int j = 1; j < chars.size(); j++){
            if(chars[j] == chars[i-1]){
                count++;
            }else{
                if(count == 1) number++;
                if(count > 1){
                    auto temp = to_string(count);
                    for(auto c : temp){
                        chars[i++] = c;
                    }
                }
                count = 1;
                chars[i++] = chars[j];
            }
            if(j == chars.size()-1){
                if(count == 1) number++;
                if(count > 1){
                    auto temp = to_string(count);
                    for(auto c : temp){
                        chars[i++] = c;
                    }
                }
            }
        }
        //All above code will return abc3d4...
        chars.resize(i);
        int j = i -1;
        chars.resize(i + number);
        i = chars.size() -1;
        while(j >=0){
            if(!isdigit(chars[j])){
                chars[i--] = '1';
                chars[i--] = chars[j--];
                continue;
            }else{
               while(isdigit(chars[j])) {
                   chars[i--] = chars[j--];
               }
                chars[i--] = chars[j--];
            }
        }
        return chars.size(); 
        //now code will return a1b1c3d4...
    }
};
