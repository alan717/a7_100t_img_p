/************************************************
*
* File Name : tree.cpp
* Purpose :
* Creation Date : 06-08-2017
* Last Modified :
* Created By :
* Description :
*
************************************************/

template <typename E>
class Position<E>
{
public:
	E& operator*();
	Position parent()const;
	Position children()const;
	bool isRoot()const;
	bool isExternal()const;
};


template <typename E>
class Tree<E>
{
public:
	class Position<E>;
	class PositionList;
	int size() const;
	bool empty() const;
	Positon root() const;
	PositionList positions() const;
};


