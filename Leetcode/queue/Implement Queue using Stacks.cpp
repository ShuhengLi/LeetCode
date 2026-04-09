/*
Implement the following operations of a queue using stacks.

push(x) -- Push element x to the back of queue.
pop() -- Removes the element from in front of queue.
peek() -- Get the front element.
empty() -- Return whether the queue is empty.

Author: Shuheng Li
*/

class Queue {
	stack<int> s1;
	stack<int> s2;
public:
	// Push element x to the back of queue.
	void push(int x) {
		s1.push(x);
	}

	// Removes the element from in front of queue.
	void pop(void) {
		if (s2.empty()) {
			while (!s1.empty()) {
				s2.push(s1.top());
				s1.pop();
			}
			s2.pop();
		}
		else {
			s2.pop();
		}

	}

	// Get the front element.
	int peek(void) {
		if (s2.empty()) {
			while (!s1.empty()) {
				s2.push(s1.top());
				s1.pop();
			}
			return s2.top();
		}
		else {
			return s2.top();
		}
	}

	// Return whether the queue is empty.
	bool empty(void) {
		return s1.empty() && s2.empty();
	}
};