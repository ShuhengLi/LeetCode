/*1152. Analyze User Website Visit Pattern
We are given some website visits: the user with name username[i] visited the website website[i] at time timestamp[i].

A 3-sequence is a list of websites of length 3 sorted in ascending order by the time of their visits.  (The websites in a 3-sequence are not necessarily distinct.)

Find the 3-sequence visited by the largest number of users. If there is more than one solution, return the lexicographically smallest such 3-sequence.

Example 1:

Input: username = ["joe","joe","joe","james","james","james","james","mary","mary","mary"], timestamp = [1,2,3,4,5,6,7,8,9,10], website = ["home","about","career","home","cart","maps","home","home","about","career"]
Output: ["home","about","career"]
Explanation: 
The tuples in this example are:
["joe", 1, "home"]
["joe", 2, "about"]
["joe", 3, "career"]
["james", 4, "home"]
["james", 5, "cart"]
["james", 6, "maps"]
["james", 7, "home"]
["mary", 8, "home"]
["mary", 9, "about"]
["mary", 10, "career"]
The 3-sequence ("home", "about", "career") was visited at least once by 2 users.
The 3-sequence ("home", "cart", "maps") was visited at least once by 1 user.
The 3-sequence ("home", "cart", "home") was visited at least once by 1 user.
The 3-sequence ("home", "maps", "home") was visited at least once by 1 user.
The 3-sequence ("cart", "maps", "home") was visited at least once by 1 user.
*/
vector<string> mostVisitedPattern(vector<string>& username, vector<int>& timestamp, vector<string>& website) {
    int n=username.size();
    unordered_map<string,map<int,string>> data;
    for(int i=0;i<n;i++)
        data[username[i]][timestamp[i]]=website[i];
    map<vector<string>,unordered_map<string,int>> res;
    // cpp vector 不能unordered_map key
    // value 的map除去相同名字但是3string一样的
    for(auto m:data){
        if(m.second.size()<3) continue;
        for(auto i=m.second.begin();i!=m.second.end();i++){
            auto j=i;j++;
            for(;j!=m.second.end();j++){
                auto k=j; k++;
                for(;k!=m.second.end();k++){
                    res[{(*i).second,(*j).second,(*k).second}][m.first]++;
                }
            }
        }
    }
    vector<string> s;
    int ma=0;
    for(auto i:res){
        if(i.second.size()>ma){
            s=i.first;
            ma=i.second.size();
        }
    }
    return s;
}
© 2020 GitHub, Inc.
