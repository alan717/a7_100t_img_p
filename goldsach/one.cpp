/************************************************
*
* File Name : one.cpp
* Purpose :
* Creation Date : 20-08-2017
* Last Modified :
* Created By :
* Description :
*
************************************************/
#include <bits/stdc++.h>

using namespace std;

string feeOrUpfront(int n, int k, int x, int d, vector <int> p) {
    // Complete this function
}

int main() {
    int q;
    cin >> q;
    for(int a0 = 0; a0 < q; a0++){
        int n;
        int k;
        int x;
        int d;
        cin >> n >> k >> x >> d;
        vector<int> p(n);
        for(int p_i = 0; p_i < n; p_i++){
           cin >> p[p_i];
        }
        string result = feeOrUpfront(n, k, x, d, p);
        cout << result << endl;
    }
    return 0;
}
