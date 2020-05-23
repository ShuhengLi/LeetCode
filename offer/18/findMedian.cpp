int findMedian(vector<int> nums){
  min_heap;
  max_heap;
  if(min_heap.size() < max_heap.size()){
    if(cur < max_heap.top()){
      max_heap.push(temp);
      auto temp = max_heap.pop();
      min_heap.push(cur);
    }else{
      min_heap.push(cur);
    }
  } else if (min_heap.size() == max_heap.size()){
    if(cur < max_heap.top()){
      max_heap.push(temp);
    }else{
      min_heap.push(cur);
      auto temp = min_heap.pop();
      max_heap.push(cur);
    }
  }
}
