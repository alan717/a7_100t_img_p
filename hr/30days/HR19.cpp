#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
class AdvancedArithmetic{
    public:
        virtual int divisorSum(int n)=0;
};



//Write your code here
class Calculator:public AdvancedArithmetic
{
	
	
	
	
	
	int divisorSum(int n) override 
	{
		int sum = 1;

		for(int k = 2; k * k <= n; ++k)
		{
			int p = 1;
			while(n % k == 0)
			{
				p = p * k + 1;
					n /= k;	
			}

				sum *= p;
		}

		if(n > 1)
		sum *= 1 + n;
		return sum;
	}
	
};
int main(){
    int n;
    cin >> n;
    AdvancedArithmetic *myCalculator = new Calculator(); 
    int sum = myCalculator->divisorSum(n);
    cout << "I implemented: AdvancedArithmetic\n" << sum;
    return 0;
}0