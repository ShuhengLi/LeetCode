/*1235. Maximum Profit in Job Scheduling
 *We have n jobs, where every job is scheduled to be done from startTime[i] to endTime[i], obtaining a profit of profit[i].

You're given the startTime , endTime and profit arrays, you need to output the maximum profit you can take such that there are no 2 jobs in the subset with overlapping time range.

If you choose a job that ends at time X you will be able to start another job that starts at time X.

 

Example 1:



Input: startTime = [1,2,3,3], endTime = [3,4,5,6], profit = [50,10,40,70]
Output: 120
Explanation: The subset chosen is the first and fourth job. 
Time range [1-3]+[3-6] , we get profit of 120 = 50 + 70.
*/
class Solution {
public:
    static bool mycomp(pair<int,pair<int,int>>a,pair<int,pair<int,int>>b){

        return a.second.first<b.second.first;
    }
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {

        vector<pair<int,pair<int,int>>>v;
        if(startTime.size()==0)return 0;
        for(int i=0;i<startTime.size();i++){
            v.push_back({startTime[i],{endTime[i],profit[i]}});
        }

        sort(v.begin(),v.end(),mycomp);
        int m=v[0].second.second;
        vector<int>arr(profit.size(),0);
        arr[0]=m;
        for(int i=1;i<profit.size();i++){
            arr[i]=max(arr[i-1],v[i].second.second);
            m=max(m,arr[i]);
           for(int j=i-1;j>=0;j--){
              if(v[i].first>=v[j].second.first){
                    arr[i]= max(arr[i],arr[j]+v[i].second.second);
                  m=max(m,arr[i]);
                    break;
                }
           }
            }

                        return m;

    }
};
