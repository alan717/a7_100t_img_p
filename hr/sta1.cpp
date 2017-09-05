#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

int main(int argc,char *argv[])
{
		vector<string> projects;
		for (int i=0;i<argc;++i)
			projects.push_back(string(argv[i]));
		for(vector<string>::iterator j=projects.begin();
				j!=projects.end();++j)
			cout<<*j<<endl;


		return 0;

}
