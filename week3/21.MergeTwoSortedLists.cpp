/*21. Merge Two Sorted Lists
 * Merge two sorted linked lists and return it as a new sorted list. The new list should be made by splicing together the nodes of the first two lists.

Example:

Input: 1->2->4, 1->3->4
Output: 1->1->2->3->4->4
*/
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* newHead = new ListNode(-1);
        ListNode* dummy = newHead;
        while(l1 && l2){
            if(l1->val < l2->val){
                newHead -> next = l1;
                l1 = l1->next;
            }else{
                newHead -> next = l2;
                l2 = l2->next;
            }
            newHead = newHead->next;
        }
        newHead -> next = l1 ? l1:l2;
        return dummy->next;
    }
};
