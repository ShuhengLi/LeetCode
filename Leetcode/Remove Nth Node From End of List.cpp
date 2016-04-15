/*
Given a linked list, remove the nth node from the end of list and return its head.

For example,

Given linked list: 1->2->3->4->5, and n = 2.

After removing the second node from the end, the linked list becomes 1->2->3->5.

Author; Shuheng Li
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
	ListNode* removeNthFromEnd(ListNode* head, int n) {
		if (n<1 || !head) return head;
		//        int b = help(head);
		//        n = n % b;

		ListNode* dummy = new ListNode(0);
		dummy->next = head;
		ListNode* slow = dummy;
		ListNode* fast = dummy;

		while (fast != NULL) {
			fast = fast->next;
			if (n<0) {
				slow = slow->next;
			}
			n--;
		}
		//ListNode *temp = slow->next;
		slow->next = slow->next->next;
		//        delete temp;
		return dummy->next;
	}

	/*   int help(ListNode *head){
	int res = 0;
	while(head!=NULL){
	res++;
	head = head -> next;
	}
	return res;
	}*/
};