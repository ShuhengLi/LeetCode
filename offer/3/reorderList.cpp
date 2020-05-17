/*143. Reorder List
 * Given a singly linked list L: L0→L1→…→Ln-1→Ln,
reorder it to: L0→Ln→L1→Ln-1→L2→Ln-2→…

You may not modify the values in the list's nodes, only nodes itself may be changed.

Example 1:

Given 1->2->3->4, reorder it to 1->4->2->3.
Example 2:

Given 1->2->3->4->5, reorder it to 1->5->2->4->3.
*/
class Solution {
public:
    void reorderList(ListNode* head) {
        if(!head) return ;
        ListNode* slow = head;
        ListNode* fast = head;
        while(fast -> next&& fast->next -> next){
            slow = slow -> next;
            fast= fast-> next;
            fast= fast-> next;
        }
        auto mid = slow->next;
        slow -> next = NULL;

        ListNode* pre = NULL;
        while(mid){
            auto temp = mid ->next;
            mid -> next = pre;
            pre = mid;
            mid = temp;
        }
        auto tail = pre;
        auto test = tail;
        while(test){
            cout << test-> val<< endl;
            test = test->next;
        }

        ListNode* res = new ListNode(0);
        ListNode* dummy= res;
        while(head && tail){
            res -> next = head;
            res = res->next;
            head = head -> next;
            res -> next = tail;
            res = res-> next;
            tail = tail -> next;
        }
        if(head!= NULL)res -> next = head;
        head = dummy->next;
        return;
    }
};
