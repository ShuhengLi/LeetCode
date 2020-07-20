/*23. Merge k Sorted Lists
Merge k sorted linked lists and return it as one sorted list. Analyze and describe its complexity.

Example:

Input:
[
  1->4->5,
  1->3->4,
  2->6
]
Output: 1->1->2->3->4->4->5->6
*/
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if(lists.empty()) return NULL;
        vector<ListNode*> cur;
        for(int i = 0; i < lists.size(); i=i+2){
            if((i + 1) < lists.size()){
                auto merged = mergeTwoLists(lists[i], lists[i+1]);
                cur.push_back(merged);
            }else{
                cur.push_back(lists[i]);
            }
        }
        if(cur.size() != 1) return(mergeKLists(cur));
        return cur[0];

    }
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* newHead = new ListNode(-1);
        ListNode* dummy = newHead;
        while(l1 && l2){
            if(l1->val < l2->val){
                newHead -> next = l1;
                l1 = l1->next;
            }else{
                newHead -> next = l2;
                l2 = l2->next;
            }
            newHead = newHead->next;
        }
        newHead -> next = l1?l1 : l2;
        return dummy->next;
    }
};

