/*19. Remove Nth Node From End of List
 * Given a linked list, remove the n-th node from the end of list and return its head.

Example:

Given linked list: 1->2->3->4->5, and n = 2.

After removing the second node from the end, the linked list becomes 1->2->3->5.
*/
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode* dummy = new ListNode(-1);
        dummy->next = head;
        ListNode* slow = dummy;
        ListNode* fast = dummy;
        while(n){
            fast = fast -> next;
            n--;
        }
        while(fast->next){
            slow = slow->next;
            fast = fast->next;
        }
        if(slow->next->next == NULL){
            slow->next = NULL;
        }else{
            auto temp = slow -> next;
            slow->next = temp->next;
            temp->next = NULL;
            delete(temp);
        }
        return dummy->next;
    }
};
