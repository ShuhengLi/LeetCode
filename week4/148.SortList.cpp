/* 148 sort List
 * Sort a linked list in O(n log n) time using constant space complexity.

Example 1:

Input: 4->2->1->3
Output: 1->2->3->4
Example 2:

Input: -1->5->3->4->0
Output: -1->0->3->4->5
*/
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if(head == nullptr || head->next == nullptr) 
            return head;
        
        ListNode *p = head, *q = head, *pre = head;
        while(q != nullptr && q->next != nullptr) {
            pre = p;
            p = p->next;
            q = q->next->next;
        }
        pre->next = nullptr;
        
        ListNode* h1 = sortList(head);
        ListNode* h2 = sortList(p);
        
        // merge two sorted list
        ListNode* newHead = (h1->val < h2->val ? h1 : h2);
        newHead == h1 ? h1 = h1->next : h2 = h2->next;
        ListNode* nh = newHead;
        
        while(h1 != nullptr && h2 != nullptr) {
            if(h1->val < h2->val) {
                nh->next = h1;
                h1 = h1->next;
            }
            else {
                nh->next = h2;
                h2 = h2->next;
            }
            nh = nh->next;    
        }   
        nh->next = (h1 == nullptr ? h2 : h1);
        return newHead;
    }
};
