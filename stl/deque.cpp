/************************************************
*
* File Name : deque.cpp
* Purpose :
* Creation Date : 19-08-2017
* Last Modified :
* Created By :
* Description :
*
************************************************/

#include <iostream>
#include <deque>
#include <iterator>
#include <algorithm>

int main()
{
	std::deque<int> a_deck;
	a_deck.push_back(3);
	a_deck.push_front(1);
	a_deck.insert(a_deck.begin()+1,2);

	a_deck[2]=0;

	std::copy(a_deck.begin(),a_deck.end(),std::ostream_iterator<int> 
			(std::cout," "));

	return 0;
}
