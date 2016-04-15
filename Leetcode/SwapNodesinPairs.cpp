/*
Given a linked list, swap every two adjacent nodes and return its head.

For example,
Given 1->2->3->4, you should return the list as 2->1->4->3.

Your algorithm should use only constant space. You may not modify the values in the list, only nodes itself can be changed.

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
	ListNode* swapPairs(ListNode* head) {
		if (head == NULL || head->next == NULL) return head;
		ListNode* even = head->next;
		ListNode* dummy1 = new ListNode(0);
		ListNode* dummy2 = new ListNode(0);
		dummy1->next = head;
		dummy2->next = even;
		while (head != NULL && even != NULL) {
			head->next = even->next;
			head = head->next;
			if (head != NULL) {
				even->next = head->next;
				even = even->next;
			}
			else {
				break;
			}
		}
		head = dummy1->next;
		even = dummy2->next;
		while (head != NULL && even != NULL) {
			ListNode* t1 = head->next;
			ListNode* t2 = even->next;
			even->next = head;
			head->next = t2;
			if (t2 == NULL) {
				head->next = t1;
				break;

			}
			head = t1;
			even = t2;

		}

		//        if(head) even->next = head;


		return dummy2->next;
	}
};