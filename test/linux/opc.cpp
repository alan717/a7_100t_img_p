/************************************************
*
* File Name : opc.cpp
* Purpose :
* Creation Date : 04-11-2017
* Last Modified : Sat 04 Nov 2017 11:55:36 AM HKT
* Created By :
* Description :
*
************************************************/
#include<iostream>

using namespace std;

int main()
{
    char a;
    int times=0;
    cin>>a;
    while(a!='\n')
    {
        cout<<a;
        cin>>a;
        times++;
    }
    cout<<times;
    return 0;
}
