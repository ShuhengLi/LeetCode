/*155. Min Stack
 * Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.

push(x) -- Push element x onto stack.
pop() -- Removes the element on top of the stack.
top() -- Get the top element.
getMin() -- Retrieve the minimum element in the stack.


Example 1:

Input
["MinStack","push","push","push","getMin","pop","top","getMin"]
[[],[-2],[0],[-3],[],[],[],[]]

Output
[null,null,null,null,-3,null,0,-2]

Explanation
MinStack minStack = new MinStack();
minStack.push(-2);
minStack.push(0);
minStack.push(-3);
minStack.getMin(); // return -3
minStack.pop();
minStack.top();    // return 0
minStack.getMin(); // return -2
*/
class MinStack {
public:
    /** initialize your data structure here. */
    stack<pair<int,int>> s2;
    stack<int> s1;
    int g_min;
    MinStack() {
    }

    void push(int x) {
        s1.push(x);
        if(s2.empty()){
            auto temp = make_pair(x, 1);
            s2.push(temp);
        }else{
            auto temp = s2.top();
            if(x == temp.first){
                temp.second++;
                s2.pop();
                s2.push(temp);
            }else if(x < temp.first){
                auto pair = make_pair(x, 1);
                s2.push(pair);
            }
        }


    }

    void pop() {
        int current = s1.top();
        s1.pop();
        auto pair = s2.top();
        if(current == pair.first){
            s2.pop();
            if(pair.second > 1){
                pair.second--;
                s2.push(pair);
            }
        }
    }

    int top() {
        return s1.top();
    }

    int getMin() {
        return s2.top().first;
    }
};
//Time: amoritize O(1)
//note: pair.first not pair->first  make_pair(a,b)
