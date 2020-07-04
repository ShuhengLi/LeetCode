/* 332. Reconstruct Itinerary
Given a list of airline tickets represented by pairs of departure and arrival airports [from, to], reconstruct the itinerary in order. All of the tickets belong to a man who departs from JFK. Thus, the itinerary must begin with JFK.

Note:

If there are multiple valid itineraries, you should return the itinerary that has the smallest lexical order when read as a single string. For example, the itinerary ["JFK", "LGA"] has a smaller lexical order than ["JFK", "LGB"].
All airports are represented by three capital letters (IATA code).
You may assume all tickets form at least one valid itinerary.
One must use all the tickets once and only once.
Example 1:

Input: [["MUC", "LHR"], ["JFK", "MUC"], ["SFO", "SJC"], ["LHR", "SFO"]]
Output: ["JFK", "MUC", "LHR", "SFO", "SJC"]
*/
class Solution {
public:
map<string, priority_queue<string, vector<string>, greater<string>>> targets;
vector<string> findItinerary(vector<vector<string>> tickets) {
    for (auto ticket : tickets)
        targets[ticket[0]].push(ticket[1]);
    visit("JFK");
    return vector<string>(route.rbegin(), route.rend());
}


vector<string> route;

void visit(string airport) {
    while (targets[airport].size()) {
        string next = targets[airport].top();
        targets[airport].pop();
        visit(next);
    }
    route.push_back(airport);
}
};
