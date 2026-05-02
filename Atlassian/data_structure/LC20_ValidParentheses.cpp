/*
 * 20. Valid Parentheses
 * Easy
 * [Atlassian DSA - High Frequency]
 *
 * Given a string s containing just the characters '(', ')', '{', '}', '[' and ']',
 * determine if the input string is valid.
 *
 * Valid rules:
 *   1. Open brackets must be closed by the same type of brackets.
 *   2. Open brackets must be closed in the correct order.
 *   3. Every close bracket has a corresponding open bracket of the same type.
 *
 * Examples:
 *   "()"      -> true
 *   "()[]{}"  -> true
 *   "(]"      -> false
 *   "([)]"    -> false
 *   "{[]}"    -> true
 *
 * Approach: Stack
 *   - Push opens onto stack.
 *   - On close: stack must be non-empty and top must match the pair; pop.
 *   - At end: stack must be empty.
 *
 * Time:  O(n)
 * Space: O(n)
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isValid(string s) {
        stack<char> st;
        // Map close -> matching open
        unordered_map<char, char> match = {
            {')', '('}, {']', '['}, {'}', '{'}
        };

        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                st.push(c);
            } else {
                // close bracket
                if (st.empty() || st.top() != match[c]) return false;
                st.pop();
            }
        }
        return st.empty();
    }
};

/*
 * Follow-up Q&A (Atlassian-style):
 *
 * Q: What if the input has other characters mixed in?
 * A: Either skip non-bracket chars, or throw — clarify spec first.
 *
 * Q: What about the minimum number of insertions to make it valid? (LC 921)
 * A: Track unmatched open and unmatched close counters; total = open + close at end.
 *
 * Q: How to also tell which position is the first error?
 * A: Track index when pushing; report mismatch index on failure.
 *
 * Q: Can it be O(1) space?
 * A: For arbitrary input, no — you must remember opens. But for a known
 *    fixed bracket type, two counters suffice. Here we have 3 types,
 *    so O(n) stack is necessary.
 */

int main() {
    Solution sol;
    cout << boolalpha;
    cout << sol.isValid("()") << endl;       // true
    cout << sol.isValid("()[]{}") << endl;   // true
    cout << sol.isValid("(]") << endl;       // false
    cout << sol.isValid("([)]") << endl;     // false
    cout << sol.isValid("{[]}") << endl;     // true
    cout << sol.isValid("") << endl;         // true (empty is valid)
    cout << sol.isValid("(") << endl;        // false
    return 0;
}
