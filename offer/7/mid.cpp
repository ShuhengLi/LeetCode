//q1 resver LinkedList
ListNode* reverse(ListNode* head){
  ListNode* pre = NULL;
  while(head){
    auto temp = head -> next;
    head -> next = pre;
    pre = head;
    head = temp;
  }
  return pre;
}
ListNode* reverse(ListNode* head){
  if(!head || !head -> next) return head;
  auto newHead = reverse(head -> next);
  head -> next -> next = head;
  head -> next = NULL;
  return newHead;
}

//q2 isBST
bool isBST(TreeNode* root){
  return helper(root);
}
bool helper(TreeNode* root, long min = LONG_MIN, long max=LONG_MAX){
  if(!root) return true;
  if(root->val <= min || root->val >= max) return false;
  return helper(root->left, min, root->val) && helper(root->right, root->val, max);
}
//q3 permutation without dupilicate
vector<string> res;
vector<string> permute(string s){
  helper(s, 0);
  return res;
}
void helper(string s, int index){
  if(index == s.size()){
    res.push_back(string);
    return;
  }
  for(int i = index; i < s.size(); i++){
    swap(s[i], s[index]);
    helper(s, index+1);
    swap(s[i], s[index]);
  }
}
//q4 quickSort
void quickSort(vector<int>& nums){
  quickSort(nums, 0, nums.size() - 1);
}
void quickSort(vector<int>& nums, int l, int r){
  if(l >= r) return;
  int pivot = getPivot(nums, l, r);
  quickSort(nums, l, pivot - 1);
  quickSort(nums, pivot + 1, r);
}
int getPivot(vector<int>& nums, int l, int r){
  int index = randRange(l, r);
  int cur = nums[index];
  swap(nums[index], nums[r]);
  int i = l;
  int j = r - 1;
  while(i <= j){
    if(nums[i] < cur){
      i++;
    }else if(nums[j] >= cur){
      j--;
    }else{
      swap(nums[i++], nums[j--]);
    }
  }
  swap(nums[i], nums[r]);
  return i;
}
int randRange(int l, int r){
  int range = r - l + 1;
  return rand()%range + l;
}

