/*Given an integer array sorted in ascending order, write a function to search target in nums.  If target exists, then return its index, otherwise return -1. However, the array size is unknown to you. You may only access the array using an ArrayReader interface, where ArrayReader.get(k) returns the element of the array at index k (0-indexed).

You may assume all integers in the array are less than 10000, and if you access the array out of bounds, ArrayReader.get will return 2147483647.

 

Example 1:

Input: array = [-1,0,3,5,9,12], target = 9
Output: 4
Explanation: 9 exists in nums and its index is 4
Example 2:

Input: array = [-1,0,3,5,9,12], target = 2
Output: -1
Explanation: 2 does not exist in nums so return -1
*/
// Forward declaration of ArrayReader class.
class ArrayReader;

class Solution {
public:
    int search(const ArrayReader& reader, int target) {
      int index = getRange(reader, target);
      if(reader.get(index)==INT_MAX){
        for(int i = index /2; i < index; i++){
          if(reader.get(i)==target){
            return i;
          }
        }
      }else{
        int l = index / 2;
        int r = index;
        while (l <= r){
          int m = l + (r-l)/2;
          if(reader.get(m) == target){
            return m;
          }else if(reader.get(m) < target){
            l = m + 1;
          }else{
            r = m - 1;
          }
        }
        return -1;
    }
    int getRange(const ArrayReader& reader, int target) {
      int start = 1;
      while(reader.get(start)!=INT_MAX && reader.get(start) < target ){
        start *= 2;
      }
      return start;

    }
};
