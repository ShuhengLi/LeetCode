/*merge 2 sorted  88. Merge Sorted Array
Given two sorted integer arrays nums1 and nums2, merge nums2 into nums1 as one sorted array.

Note:

The number of elements initialized in nums1 and nums2 are m and n respectively.
You may assume that nums1 has enough space (size that is greater or equal to m + n) to hold additional elements from nums2.
Example:

Input:
nums1 = [1,2,3,0,0,0], m = 3
nums2 = [2,5,6],       n = 3

Output: [1,2,2,3,5,6]
*/
//class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        if(!n) return;
        if(!m){
            for(int i = 0; i< n; i++){
                nums1[i] = nums2[i];
            }
            return;
        }
        int k = m + n - 1;
        int i = m  - 1;
        int j = n - 1;
        while(i >= 0 && j >= 0){
            if(nums1[i] > nums2[j]){
                nums1[k--] = nums1[i--];
            }else{
                nums1[k--] = nums2[j--];
            }
        }
        while(k >= 0 && j >= 0){
           nums1[k--] = nums2[j--];
        }
        return;
    }
};

//merge 2 linked-list
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if(!l1) return l2;
        if(!l2) return l1;
        ListNode* newHead = new ListNode(-1);
        ListNode* dummy = newHead;
        int count =0;
        while(l1 && l2){
            if(l1->val < l2 ->val) {
                newHead -> next = l1;
                l1 = l1 -> next;
            }else{
                newHead -> next = l2;
                l2 = l2->next;
            }
            newHead = newHead -> next;
        }
        if(l1){
            newHead -> next = l1;
        }
        if(l2){
            newHead -> next = l2;
        }
        return dummy->next;
    }
};


//Method 1:Iterateve A1A2 -> A12 A3 -> A13 A4 -> A14......
//Time: O(k^2*N)
//
//Method 2:Binary merge
//A1 A2 -> A12
//A3 A4 -> A34
//....
//Ak-1 Ak ->Ak
//Time: O(klogk*N)
//space:O(KN)
//
//Method 3: k pointer move smallest one
//min_heap<pair<index_of_arry, index_in_array, value> track pointer number
//Time: O(logK*K*N)
//space:O(K)
//how many times to read/write opearation
//For Binary merge: for each element read logk write logk
//For K pointer each element read write just once
//
/*
23. Merge k Sorted Lists
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
struct CustomCompare
{
    bool operator()(ListNode* a, ListNode* b) {
        return a->val > b->val;
    }
};
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if(lists.empty()) return NULL;
        int k = lists.size();
        if(k == 1) return lists[0];
        priority_queue<ListNode*, vector<ListNode*>, CustomCompare> q;
        for(int i = 0; i < k; i++){
            if(lists[i]){
                q.push(lists[i]);
            }
        }
        ListNode* newHead = new ListNode(-1);
        ListNode* dummy = newHead;
        while(!q.empty()){
            auto cur = q.top();q.pop();
            newHead -> next = cur;
            cur = cur->next;
            if(cur){
                q.push(cur);
            }
            newHead = newHead->next;
        }
        return dummy->next;
    }
};
