#pragma once
#include "MessageTypes.h"

#include <iostream>

namespace messages
{
template <typename MessageTypes>
class Handler
{
public:
    void handle(MessageWrapper<MessageA, MessageTypes>)
    {
        ::std::cout << "MessageA" << ::std::endl;
    }
    void handle(MessageWrapper<MessageB, MessageTypes>)
    {
        ::std::cout << "MessageB" << ::std::endl;
    }
    void handle(MessageWrapper<MessageC, MessageTypes>)
    {
        ::std::cout << "MessageC" << ::std::endl;
    }
};
}
