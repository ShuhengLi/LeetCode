/*24. Swap Nodes in Pairs
 * Given a linked list, swap every two adjacent nodes and return its head.

You may not modify the values in the list's nodes, only nodes itself may be changed.



Example:

Given 1->2->3->4, you should return the list as 2->1->4->3.
*/
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if(!head || !head->next ) return head;
        ListNode* next = head -> next;
        ListNode* newNode = swapPairs(next -> next);
        head -> next = newNode;
        next -> next = head;
        return next;
    }
};
