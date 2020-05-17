void insertNode(ListNode* head, ListNode* target){
  if(!target) return head;
  ListNode* dummy = new ListNode(-1);
  auto cur = dummy;
  while(cur->next && cur->next->val < target->val){
    cur = cur -> next;
  }
  auto temp = cur -> next;
  cur -> next = target;
  target -> next = temp;
  return dummy->next;
}
