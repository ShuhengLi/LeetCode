/*2. Add Two Numbers
You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example:

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.
*/
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int carry = 0;
        ListNode* newhead = new ListNode(-1);
        ListNode* dummy = newhead;
        while(l1 || l2 || carry){
            int num = (l1?l1->val:0) + (l2?l2->val:0) + carry;
            carry = num / 10;
            ListNode* node = new ListNode(num % 10);
            newhead->next = node;
            newhead = newhead->next;
            if(l1) l1 = l1->next;
            if(l2) l2 = l2->next;
        }
        return dummy->next;
    }
};
