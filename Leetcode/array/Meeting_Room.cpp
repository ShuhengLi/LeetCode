/*
Given an array of meeting time intervals consisting of start and end times [[s1,e1],[s2,e2],...] (si < ei), determine if a person could attend all meetings.
For example,
Given [[0, 30],[5, 10],[15, 20]],
return false.

Author: Shuheng Li
*/

/*
Given an array of meeting time intervals consisting of start and end times [[s1,e1],[s2,e2],...] (si < ei), determine if a person could attend all meetings.
For example,Given [[0, 30],[5, 10],[15, 20]],return false.

Author: Shuheng Li
*/

/**
* Definition for an interval.
* struct Interval {
*     int start;
*     int end;
*     Interval() : start(0), end(0) {}
*     Interval(int s, int e) : start(s), end(e) {}
* };
*/
class Solution {
public:
	bool canAttendMeetings(vector<Interval>& intervals) {
		int n = intervals.size();
		for (int i = 0; i < n; i++) {
			if (overLap(intervals[i], intervals[i + 1])) {
				return false;
			}
		}
		return true;
	}
private:
	bool overLap(Interval a, Interval b) {
		return a.end > b.start;
	}
};