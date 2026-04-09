/*
Given a singly linked list, determine if it is a palindrome.

Auth
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
	bool isPalindrome(ListNode* head) {
		if (!head) return true;
		vector<int> v;
		while (head != NULL) {
			v.push_back(head->val);
			head = head->next;
		}
		int l = 0, r = v.size() - 1;
		while (l <= r) {
			if (v[l] != v[r]) return false;
			l++;
			r--;
		}
		return true;
	}
};