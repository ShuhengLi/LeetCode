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
    stack<int> s1;
    stack<int> s2;
    MinStack() {
    }

    void push(int x) {
        s1.push(x);
        if(s2.empty() || x <= s2.top()){
            s2.push(x);
        }
    }

    void pop() {
        int top = s1.top();
        s1.pop();
        if(top == s2.top()){
            s2.pop();
        }
    }

    int top() {
        return s1.top();
    }

    int getMin() {
        return s2.top();
    }
};

