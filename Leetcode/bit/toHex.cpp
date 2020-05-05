/*405. Convert a Number to Hexadecimal
 * Given an integer, write an algorithm to convert it to hexadecimal. For negative integer, two’s complement method is used.

Note:

All letters in hexadecimal (a-f) must be in lowercase.
The hexadecimal string must not contain extra leading 0s. If the number is zero, it is represented by a single zero character '0'; otherwise, the first character in the hexadecimal string will not be the zero character.
The given number is guaranteed to fit within the range of a 32-bit signed integer.
You must not use any method provided by the library which converts/formats the number to hex directly.
Example 1:

Input:
26

Output:
"1a"
*/.
class Solution {
    map<int, char> m;
public:
    string toHex(int num2) {
        if(!num2) return "0";
        unsigned num = num2; //to handle negtive number
        m[10] = 'a';
        m[11] = 'b';
        m[12] = 'c';
        m[13] = 'd';
        m[14] = 'e';
        m[15] = 'f';
        string res;
        while(num != 0){
            int reminder = num % 16;
            if(reminder > 9){
                res = m[reminder] + res;
            }else{
                res = to_string(reminder) + res;
            }
            num = num / 16;
        }
        return res;
    }
};

//check each 4 bits
//
class Solution {
    map<int, char> m;
public:
    string toHex(int num) {
        m[10] = 'a';
        m[11] = 'b';
        m[12] = 'c';
        m[13] = 'd';
        m[14] = 'e';
        m[15] = 'f';
        string res;
        unsigned bit = 15;
        for(int i = 0; i < 8; i++){
            int current = num >> i * 4 & bit;
            int reminder = current;
            if(reminder > 9){
                res = m[reminder] + res;
            }else{
                res = to_string(reminder) + res;
            }
        }
        int i = 0; 
        int j = 0;
        if(res == "00000000") return "0"; //edge case
        if (res.size() > 1){
            while( i < res.size() && j < res.size()){
                if(j == 0){//pop leading 0
                while(res[j] == '0'){
                    j++;
                }
                }
                res[i++] = res[j++];
            }
        }
        res.resize(i);
        return res;
    }
};
