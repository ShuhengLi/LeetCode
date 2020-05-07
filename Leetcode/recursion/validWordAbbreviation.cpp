/*408. Valid Word Abbreviation
 *Given a non-empty string s and an abbreviation abbr, return whether the string matches with the given abbreviation.
A string such as "word" contains only the following valid abbreviations:

Example 1:
Given s = "internationalization", abbr = "i12iz4n":

Return true.
Example 2:
Given s = "apple", abbr = "a2e":

Return false.
*/


//Note change the signature to pass word start index and abbr start index
class Solution {
public:
    bool validWordAbbreviation(string word, string abbr) {
      if(!word.size() && !abbr.size()){
        return true;
      }else if(!word.size() || !abbr.size()){
        return false;
      }

      if(isdigit(abbr[0]){
          int i = 0;
          if(word.size() < num) return false;
          int num = 0;
          while(i < abbr.size() && isdigit(abbr[i])){
            num = num * 10 + abbr[i++] - '0';
          }
          if(word.size() < num) return false;
          return validWordAbbreviation(word.substr(num), abbr.substr(i));
      }
      else{
          if(word[0] ! = abbr[0]) return false;
          return validWordAbbreviation(word.substr(1), abbr.substr(1));
      }
    }
};
