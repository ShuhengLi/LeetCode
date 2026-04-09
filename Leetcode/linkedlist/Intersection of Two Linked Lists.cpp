/*
Write a program to find the node at which the intersection of two singly linked lists begins.


For example, the following two linked lists:

A:          a1 ¡ú a2
                   ¨K
                     c1 ¡ú c2 ¡ú c3
                   ¨J            
B:     b1 ¡ú b2 ¡ú b3
begin to intersect at node c1.

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
	ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
		if (!headA || !headB) return NULL;
		int l1 = help(headA);
		int l2 = help(headB);
		int n = 0;

		if (l1 > l2) {
			n = l1 - l2;
			while (n>0) {
				headA = headA->next;
				n--;
			}
		}
		else {
			n = l2 - l1;
			while (n>0) {
				headB = headB->next;
				n--;
			}
		}
		while (headA != headB && headA && headB) {
			headA = headA->next;
			headB = headB->next;
		}

		if (!headA || !headB) return NULL;
		else return headA;

	}

	int help(ListNode *head) {
		int res = 0;
		while (head != NULL) {
			res++;
			head = head->next;
		}
		return res;
	}
};