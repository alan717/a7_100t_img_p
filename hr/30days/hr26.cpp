/************************************************
*
* File Name : hr26.cpp
* Purpose :
* Creation Date : 06-08-2017
* Last Modified :
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
typedef struct {
    int a;
    int b;
    int c;
}dat;
 int calc(dat fir,dat sec)
 {
     if(fir.c>sec.c)
     {
         return 10000;
     }else if(fir.c<sec.c)
	 {
		 return 0;
	 }else	 
	 {
		 if(fir.b>sec.b)
		 {
         	return (500*(fir.b-sec.b));
		 }else if (fir.b<sec.b)
		 {
			 return 0;
		 }else
		 {
			 if(fir.a>sec.a)
			 {
				 return (15*(fir.a-sec.a));
			 }else if(fir.a<sec.a)
			 {
				 return 0;
			 }else
			 {
				return 0;
			 }
		 }
     }
     
     
//     return 0;
 };
int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
    dat d1,d2;
    cin >> d1.a>>d1.b>>d1.c;
    cin >> d2.a>>d2.b>>d2.c;
    cout<<calc(d1,d2)<<endl;
    return 0;
}



