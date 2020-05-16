class Solution {
public:
    vector<int> sortByStack(vector<int>& nums) {
        stack<int> s;
        stack<int> s1;
        stack<int> s2;
        for(auto c : nums){
            s.push(c);
        }
        if(nums.empty()) return nums;
        while(!s.empty()){
            pair<int, int> g_min = make_pair(INT_MAX, 1);
            while(!s.empty()){
                if(s.top() < g_min.first){
                    g_min.first =  s.top();
                    g_min.second = 1;
                }else if(s.top() == g_min.first){
                    g_min.second += 1;
                }
                s1.push(s.top());
                s.pop();
            }
            while(!s1.empty()){
                if(s1.top() == g_min.first ){
                    s1.pop();
                    continue;
                }
                s.push(s1.top());
                s1.pop();
            }
            for(int i = 0; i < g_min.second; i++){
                s2.push(g_min.first);
            }
        }
        nums.clear();
        while(!s2.empty()){
            nums.push_back(s2.top());
            s2.pop();
        }
        return nums;
    }
};
//Followup just one stack
class Solution {
public:
    vector<int> sortByStack(vector<int>& nums) {
        stack<int> s;
        stack<int> s1;
        for(auto c : nums){
            s.push(c);
        }
        if(nums.empty()) return nums;
        int size = nums.size();
        while(!s.empty()){
            pair<int, int> g_min = make_pair(INT_MAX, 1);
            while(!s.empty()){
                if(s.top() < g_min.first){
                    g_min.first =  s.top();
                    g_min.second = 1;
                }else if(s.top() == g_min.first){
                    g_min.second += 1;
                }
                s1.push(s.top());
                s.pop();
            }

            for(int i = 0; i < size; i++){
                if(s1.top() == g_min.first ){
                    s1.pop();
                    continue;
                }
                s.push(s1.top());
                s1.pop();
            }
            for(int i = 0; i < g_min.second; i++){
                s1.push(g_min.first);
            }
            size -= g_min.second;
        }
        nums.clear();
        while(!s1.empty()){
            nums.push_back(s1.top());
            s1.pop();
        }
        return nums;
    }
};
