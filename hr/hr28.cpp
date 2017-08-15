/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : hr28.c

* Purpose :

* Creation Date : 05-08-2017

* Last Modified : Sat 05 Aug 2017 11:04:08 PM HKT

* Created By :  

_._._._._._._._._._._._._._._._._._._._._.*/

#include <set>
#include <iostream>
using namespace std;

int main()
{
    int N;
    cin >> N;
    regex e(".+@gmail\\.com$");
    multiset<string> db;
    for(int a0 = 0; a0 < N; a0++)
    {
        string firstName;
        string emailID;
        cin >> firstName >> emailID;
        if(regex_match(emailID,e))
        {
            db.insert(firstName);
        }
    }
    // just a fancy and less robust way of iterating in 
    // C++11 (and later)
    for(auto& it : db) 
    { 
        cout << it << endl; 
    }
    return 0;
}
