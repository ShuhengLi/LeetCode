/*
Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.

push(x) -- Push element x onto stack.
pop() -- Removes the element on top of the stack.
top() -- Get the top element.
getMin() -- Retrieve the minimum element in the stack.
Subscribe to see which companies asked this question

Author: Shuheng Li
Date: 04/27/2016
*/

class MinStack {
	stack<int> t1;
	stack<int> t2;
public:
	void push(int x) {
		t1.push(x);
		if (t2.empty() || x <= t2.top()) {
			t2.push(x);
		}
	}

	void pop() {
		if (t1.top() == t2.top()) t2.pop();
		t1.pop();
	}

	int top() {
		return t1.top();
	}

	int getMin() {
		return t2.top();
	}
};