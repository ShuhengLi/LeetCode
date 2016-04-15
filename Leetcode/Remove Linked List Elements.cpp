/*
Remove all elements from a linked list of integers that have value val.

Example
Given: 1 --> 2 --> 6 --> 3 --> 4 --> 5 --> 6, val = 6
Return: 1 --> 2 --> 3 --> 4 --> 5

Author: SHuheng Li
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
	ListNode* removeElements(ListNode* head, int val) {
		if (!head) return head;
		ListNode* dummy = new ListNode(0);
		dummy->next = head;
		ListNode* cur = dummy;
		while (cur->next != NULL) {
			if (cur->next->val == val) {
				cur->next = cur->next->next;
			}
			else {
				cur = cur->next;
			}
		}
		return dummy->next;
	}
};