/*341. Flatten Nested List Iterator
 * Given a nested list of integers, implement an iterator to flatten it.

Each element is either an integer, or a list -- whose elements may also be integers or other lists.

Example 1:

Input: [[1,1],2,[1,1]]
Output: [1,1,2,1,1]
Explanation: By calling next repeatedly until hasNext returns false,
             the order of elements returned by next should be: [1,1,2,1,1].
Example 2:

Input: [1,[4,[6]]]
Output: [1,4,6]
Explanation: By calling next repeatedly until hasNext returns false,
             the order of elements returned by next should be: [1,4,6].
             */
class NestedIterator {
public:
    vector<int> res;
    int cur;
    NestedIterator(vector<NestedInteger> &nestedList) {
        helper(nestedList);
        cur = 0;
    }
    void helper(vector<NestedInteger> &list) {
        for(int i = 0; i< list.size(); i++){
            if(list[i].isInteger()){
                res.push_back(list[i].getInteger());
            }else{
                helper(list[i].getList());
            }
        }
    }
    int next() {
        return res[cur++];
    }
    bool hasNext() {
        return cur < res.size();
    }
};
