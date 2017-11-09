#include <iostream>
#include <string>


using namespace std;

















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





void func(string& ff,string& ss)
{
	string::iterator ii=ff.begin();
	string::iterator kk=ss.begin();
	int n=ss.size();
	for(;ii<ff.end();ii++)
	{
		while()
	}	
}
