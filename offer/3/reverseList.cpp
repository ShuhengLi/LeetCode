/*206. Reverse Linked List
 * Reverse a singly linked list.

Example:

Input: 1->2->3->4->5->NULL
Output: 5->4->3->2->1->NULL
*/
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode * pre = NULL;
        while(head){
            auto temp = head -> next;
            head -> next = pre;
            pre = head;
            head = temp;
        }
        return pre;
    }
};

//recurssion
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if(!head || !(head->next) return head;
        ListNode* newhead = reverseList(head->next);
        head -> next ->next = head;
        head -> next = NULL;
        return newhead;
    }
};
