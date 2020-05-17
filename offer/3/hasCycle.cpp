/*141. Linked List Cycle
Add to List

Share
Given a linked list, determine if it has a cycle in it.

To represent a cycle in the given linked list, we use an integer pos which represents the position (0-indexed) in the linked list where tail connects to. If pos is -1, then there is no cycle in the linked list.

 

Example 1:

Input: head = [3,2,0,-4], pos = 1
Output: true
Explanation: There is a cycle in the linked list, where tail connects to the second node.
*/
class Solution {
public:
    bool hasCycle(ListNode *head) {
        auto slow = head;
        auto fast = head;
        while(fast && fast -> next){
            slow = slow->next;
            fast = fast -> next;
            fast = fast -> next;
            if(fast == slow)  return true;
        }
        return false;
    }
};
