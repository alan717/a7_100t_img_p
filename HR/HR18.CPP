#include <iostream>

using namespace std;
#define MAX 1000
class Solution {
    //Write your code here
public:
    Solution(){
		top=-1;
		
	}
    ~Solution(){}
	bool pushCharacter(char x)
	{
		if(top>=max )
		{
			cout<<"stack overflow"<<endl;
			return false ;
			
		}
		else{
			a[++top]=x;
			return true;
		}
	}
	
	void enqueueCharacter(char x)
	{
		q.push(x);
	}
	char popCharacter()
	{
		if(top<0)
		{
			cout<<"underflow"<<endl;
			return false;
		}
		else{
			int x=a[top--];
			return x;
		}
		
	}
	char dequeueCharacter()
	{
		return q.pop();
	}
private:
    int top;
	char a[1000];
	queque<char> q;
    
    
};

int main() {
    // read the string s.
    string s;
    getline(cin, s);
    
  	// create the Solution class object p.
    Solution obj;
    
    // push/enqueue all the characters of string s to stack.
    for (int i = 0; i < s.length(); i++) {  
        obj.pushCharacter(s[i]);
        obj.enqueueCharacter(s[i]);
    }
    
    bool isPalindrome = true;
    
    // pop the top character from stack.
    // dequeue the first character from queue.
    // compare both the characters.
    for (int i = 0; i < s.length() / 2; i++) {
        if (obj.popCharacter() != obj.dequeueCharacter()) {
            isPalindrome = false;
            
            break;
        }
    }
    
    // finally print whether string s is palindrome or not.
    if (isPalindrome) {
        cout << "The word, " << s << ", is a palindrome.";
    } else {
        cout << "The word, " << s << ", is not a palindrome.";
    }
    
    return 0;
}