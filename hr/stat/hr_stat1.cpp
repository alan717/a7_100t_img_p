/************************************************
*
* File Name : acc.cpp
* Purpose :
* Creation Date : 04-11-2017
* Last Modified : Thu 09 Nov 2017 10:48:23 PM HKT
* Created By :
* Description :
*
************************************************/
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
bool compare(double a,double b)
{
      return a<b;   //升序排列，如果改为return a>b，则为降序

}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int n,a;
    double sum=0,avg,mid,max;
     int i=0,index;
    cin>>n;
    vector<double> vc(n);
    uint fre[n];
//    cin>>a;
    for(i=0;i<n;i++)
    {
        cin>>vc[i];
    }
    for(i=0;i<n;i++)
    {
        sum+=vc[i];
    }
    avg=sum/n;
    
    sort(vc.begin(),vc.end(),compare);  
    if(n%2==1)
    {
        mid=vc[n/2+1];
    }
    else
    {
        mid=(vc[n/2]+vc[(n/2)-1]);
    }
//    vector<int>::iterator beg,ed;
//
    for(i=0;i<vc.size();i++)
    {
        fre[i]=count(&vc[0],&vc[n-1],vc[i]);
    }
    max=0;
    for(i=0;i<vc.size();i++)
    {
        if(max<fre[i])
        {
            max=fre[i];
            index=i;
        }

    }
//    sort(fre,fre+n,compare);
    cout<<avg<<endl;
    cout<<mid/2<<endl;
    cout<<vc[index]<<endl;
    return 0;
}

