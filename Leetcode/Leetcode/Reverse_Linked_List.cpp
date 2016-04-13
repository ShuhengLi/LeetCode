/*
Reverse a singly linked list.

Author: Shuheng Li
*/

/**
* Definition for singly-linked list.
* struct ListNode {
*     int val;
*     ListNode *next;
*     ListNode(int x) : val(x), next(NULL) {}
* };
*/
class Solution {
public:
	ListNode* reverseList(ListNode* head) {
		if (head == NULL) return head;

		ListNode* pre = NULL;

		while (head != NULL) {
			ListNode* n = head->next;
			head->next = pre;
			pre = head;
			head = n;
		}
		return pre;
	}
};