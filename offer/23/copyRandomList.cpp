/*138. Copy List with Random Pointer
A linked list is given such that each node contains an additional random pointer which could point to any node in the list or null.

Return a deep copy of the list.

The Linked List is represented in the input/output as a list of n nodes. Each node is represented as a pair of [val, random_index] where:

val: an integer representing Node.val
random_index: the index of the node (range from 0 to n-1) where random pointer points to, or null if it does not point to any node.

Input: head = [[7,null],[13,0],[11,4],[10,2],[1,0]]
Output: [[7,null],[13,0],[11,4],[10,2],[1,0]]
*/
class Solution {
public:
    Node* copyRandomList(Node* head) {
        if(!head) return head;
        map<Node*, Node*> s;
        Node* newHead = new Node(head->val);
        s[head] = newHead;
        Node* dummy = newHead;
        while(head){
            auto r = head->random;
            if(!r){
                newHead->random = NULL;   
            }else if(s.find(r) != s.end()){
                newHead->random = s[r];
            }else{
                Node* newRandom = new Node(head->random->val);
                newHead->random = newRandom;
                s[head->random] = newRandom;
            }
            auto n = head->next;
            if(!n){
                newHead->next = NULL;   
            }else if(s.find(n) != s.end()){
                newHead->next = s[n];
            }else{
                Node* newNext = new Node(head->next->val);
                newHead->next= newNext;
                s[head->next] = newNext;
            }
            head = head -> next;
            newHead = newHead -> next;
        }
        return dummy;
    }
};
