/*215. Kth Largest Element in an Array
 * Find the kth largest element in an unsorted array. Note that it is the kth largest element in the sorted order, not the kth distinct element.

Example 1:

Input: [3,2,1,5,6,4] and k = 2
Output: 5
Example 2:

Input: [3,2,3,1,2,4,5,5,6] and k = 4
Output: 4
*/
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        priority_queue<int,vector<int>, greater<int>> q;
        for(auto element :nums){
            if(q.empty()){
                q.push(element);
                continue;
            }
            if(q.size() < k){
                q.push(element);
            }else {
                if(q.top() < element){
                    q.pop();
                    q.push(element);
                }
            }
        }
        return q.top();
    }
};
