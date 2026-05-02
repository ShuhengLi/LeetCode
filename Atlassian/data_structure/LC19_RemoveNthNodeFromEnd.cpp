/*
 * 19. Remove Nth Node From End of List
 * Medium
 *
 * Given the head of a linked list, remove the nth node from the end of the list
 * and return its head.
 *
 * Example 1:
 * Input: head = [1,2,3,4,5], n = 2
 * Output: [1,2,3,5]
 *
 * Example 2:
 * Input: head = [1], n = 1
 * Output: []
 *
 * Constraints:
 * - The number of nodes in the list is sz (1 <= sz <= 30)
 * - 0 <= Node.val <= 100
 * - 1 <= n <= sz
 *
 * Follow up: Could you do this in one pass?
 */
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    // Two pointers with dummy node - one pass
    // Time: O(n), Space: O(1)
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;

        // Move fast pointer n+1 steps ahead
        for (int i = 0; i <= n; i++) {
            fast = fast->next;
        }

        // Move both until fast reaches end
        while (fast != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }

        // Skip the target node
        ListNode* toDelete = slow->next;
        slow->next = slow->next->next;
        delete toDelete;

        return dummy.next;
    }
};
