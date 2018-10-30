#include <iostream>

#include "MessageHandler.h"
#include "MessageTypes.h"
#include "Dispatcher.h"

#include <sstream>
#include <string>
#include <cstring>

 using namespace messages;

int main(int argc, char **argv)
{
    Dispatcher<MessageTypes> dispatcher;
    Handler<MessageTypes> handler;

    std::string streamstr;
    std::stringstream stream(streamstr);

    {
        MessageWrapper<MessageA, MessageTypes> msg;
        msg.serialize(stream);
        dispatcher.dispatch(stream, handler);
    }

    {
        MessageWrapper<MessageC, MessageTypes> msg;
        msg.serialize(stream);
        dispatcher.dispatch(stream, handler);
    }

    {
        MessageWrapper<MessageB, MessageTypes> msg;
        msg.serialize(stream);
        dispatcher.dispatch(stream, handler);
    }

    return 0;
}

