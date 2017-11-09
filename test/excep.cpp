/************************************************
*
* File Name : excep.cpp
* Purpose :
* Creation Date : 08-11-2017
* Last Modified : Wed 08 Nov 2017 10:21:39 PM HKT
* Created By :
* Description :
*
************************************************/
#include <iostream>
#include <typeinfo>
#include <exception>

class Polymorphic{
    virtual void member(){}
};

int main(){
    try
    {
    Polymorphic *pb=0;
    typeid(*pb);
    }
    catch (std::exception& e)
    {
        std::cerr<<"exception caught::"<<e.what()<<'\n';
    }

    return 0;

}
