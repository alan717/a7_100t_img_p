#pragma once

#include "TypeList.h"
#include "Messages.h"
#include "MessageWrapper.h"

namespace messages
{
    typedef types::Cons<MessageA,
            types::Cons<MessageB,
            types::Cons<MessageC
            > > >

            MessageTypes;
}