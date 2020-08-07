/*218. The Skyline Problem
 * A city's skyline is the outer contour of the silhouette formed by all the buildings in that city when viewed from a distance. Now suppose you are given the locations and height of all the buildings as shown on a cityscape photo (Figure A), write a program to output the skyline formed by these buildings collectively (Figure B).

Buildings Skyline Contour
The geometric information of each building is represented by a triplet of integers [Li, Ri, Hi], where Li and Ri are the x coordinates of the left and right edge of the ith building, respectively, and Hi is its height. It is guaranteed that 0 ≤ Li, Ri ≤ INT_MAX, 0 < Hi ≤ INT_MAX, and Ri - Li > 0. You may assume all buildings are perfect rectangles grounded on an absolutely flat surface at height 0.

For instance, the dimensions of all buildings in Figure A are recorded as: [ [2 9 10], [3 7 15], [5 12 12], [15 20 10], [19 24 8] ] .

The output is a list of "key points" (red dots in Figure B) in the format of [ [x1,y1], [x2, y2], [x3, y3], ... ] that uniquely defines a skyline. A key point is the left endpoint of a horizontal line segment. Note that the last key point, where the rightmost building ends, is merely used to mark the termination of the skyline, and always has zero height. Also, the ground in between any two adjacent buildings should be considered part of the skyline contour.

For instance, the skyline in Figure B should be represented as:[ [2 10], [3 15], [7 12], [12 0], [15 10], [20 8], [24, 0] ].
*/
class Solution {
public:
    multiset<int> s;
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        vector<vector<int>> res;
        vector<pair<int, int>> entry;
        for(auto b:buildings){
            entry.push_back({b[0], b[2]});
            entry.push_back({b[1], -b[2]});
        }
        sort(entry.begin(), entry.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            if(a.first == b.first) return a.second > b.second;
            return a.first < b.first;
        });
        for(auto e:entry){
            bool entering = e.second > 0;
            int x = e.first;
            int h = abs(e.second);
            if(entering){
                if(h > maxH()){
                    res.push_back({x, h});
                }
                s.insert(h);
            }else{
                s.erase(s.lower_bound(h));
                if(h > maxH()){
                    res.push_back({x, maxH()});
                }
            }
        }
        return res;
    }
    int maxH(){
        if(s.empty()) return 0;
        return *s.rbegin();

    }
};
