/*
Given a sorted linked list, delete all duplicates such that each element appear only once.

For example,
Given 1->1->2, return 1->2.
Given 1->1->2->3->3, return 1->2->3.

Author: Shuheng Li
*/

class Solution {
public:
	ListNode* deleteDuplicates(ListNode* head) {
		if (!head) return head;
		ListNode* cur = head;
		ListNode* dummy = new ListNode(0);
		dummy->next = head;
		while (cur != NULL && cur->next != NULL) {
			if (cur->val == cur->next->val) {
				cur = cur->next;
			}
			else {
				head->next = cur->next;
				head = head->next;
				cur = cur->next;
			}
		}
		head->next = NULL;
		return dummy->next;
	}
};