/*315. Count of Smaller Numbers After Self
 * You are given an integer array nums and you have to return a new counts array. The counts array has the property where counts[i] is the number of smaller elements to the right of nums[i].



Example 1:

Input: nums = [5,2,6,1]
Output: [2,1,1,0]
Explanation:
To the right of 5 there are 2 smaller elements (2 and 1).
To the right of 2 there is only 1 smaller element (1).
To the right of 6 there is 1 smaller element (1).
To the right of 1 there is 0 smaller element.
*/
class Solution {
public:
    vector<int>result;
    vector<pair<int,int>>arr;
    void merge(int s,int mid, int e)
    {
        vector<pair<int,int>>temp(e-s+1);
        int i=s, j=mid+1, pos=0;
        int rcount=0;//To store the count of smaller elements comming from the right subarray
        while(i<=mid&&j<=e)
        {
            if(arr[j].first<arr[i].first)
            {
                rcount++;
                temp[pos++]=arr[j++];
            }
            else
            {
                result[arr[i].second]+=rcount;
                temp[pos++]=arr[i++];
            }
        }
        while(i<=mid)
        {
                result[arr[i].second]+=rcount;
                temp[pos++]=arr[i++];
        }
        while(j<=e)
        {
                temp[pos++]=arr[j++];
        }
        pos=0;
        for(int i=s;i<=e;i++) arr[i]=temp[pos++];
        return;
    }

    void mergesort(int s,int e)
    {
        if(s>=e) return;
        int mid=(s+e)/2;
        mergesort(s,mid);
        mergesort(mid+1,e);
        merge(s,mid, e);
        return;
    }

    vector<int> countSmaller(vector<int>& nums) {
     int n=nums.size();
     result.resize(n,0);
     arr.resize(n);
     for(int i=0;i<n;i++) arr[i]={nums[i],i};
     mergesort(0,n-1);
     return result;
    }
};
