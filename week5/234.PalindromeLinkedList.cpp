/*234. Palindrome Linked List
 * Given a singly linked list, determine if it is a palindrome.

Example 1:

Input: 1->2
Output: false
Example 2:

Input: 1->2->2->1
Output: true
*/
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if(!head) return true;

        ListNode* slow = head;
        ListNode* fast = head;
        while(fast->next && fast->next->next){
            slow = slow -> next;
            fast = fast -> next -> next;
        }
        ListNode* dummy = slow;
        ListNode* mid = dummy->next;
        dummy->next = NULL;

        ListNode* pre = NULL;

        while(mid){
            ListNode* temp = mid->next;
            mid->next = pre;
            pre = mid;
            mid = temp;
        }

        while(head && pre){
            if(head->val != pre->val) return false;
            head = head->next;
            pre = pre->next;
        }
        return true;
    }
};
