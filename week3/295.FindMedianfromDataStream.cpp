/*295. Find Median from Data Stream
 * Median is the middle value in an ordered integer list. If the size of the list is even, there is no middle value. So the median is the mean of the two middle value.

For example,
[2,3,4], the median is 3

[2,3], the median is (2 + 3) / 2 = 2.5

Design a data structure that supports the following two operations:

void addNum(int num) - Add a integer number from the data stream to the data structure.
double findMedian() - Return the median of all elements so far.
Example:
addNum(1)
addNum(2)
findMedian() -> 1.5
addNum(3)
findMedian() -> 2
*/
class MedianFinder {
public:
    /** initialize your data structure here. */
    priority_queue<int> small;
    priority_queue<int, vector<int>, greater<int>> large;
    MedianFinder() {
    }

    void addNum(int num) {
        if(small.size() == large.size()){
          large.push(num);
          small.put(large.top());
          large.pop();
        }else if(small.size() > large.size()){
          small.push(num);
          large.push(small.top());
          small.pop();
        }
    }

    double findMedian() {
        if((small.size() + large.size())%2) return small.top();
        double a = small.top();
        double b = large.top();
        return (a + b )/2;
    }
};
