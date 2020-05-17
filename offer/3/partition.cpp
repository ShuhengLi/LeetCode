/*86. Partition List
 * Given a linked list and a value x, partition it such that all nodes less than x come before nodes greater than or equal to x.

You should preserve the original relative order of the nodes in each of the two partitions.

Example:

Input: head = 1->4->3->2->5->2, x = 3
Output: 1->2->2->4->3->5
*/
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode* dummy1 = new ListNode(0);
        ListNode* dummy2 = new ListNode(0);
        auto small = dummy1;
        auto large = dummy2;
        while(head){
            if(head->val < x){
                small -> next = head;
                small = small -> next;
            }else{
                large -> next = head;
                large = large -> next;
            }
            auto temp = head -> next;
            head -> next = NULL;//KEY!!!!! cut all node 
            head = temp;
        }
        ListNode* res;
        if(dummy1->next){
            res = dummy1->next;
            small -> next = dummy2->next;
        }else{
            res = dummy2->next;
        }
        return res;
    }
};
