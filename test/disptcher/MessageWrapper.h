#pragma once
#include "TypeList.h"

#include <iostream>

namespace messages
{
//! Return the message type for the next serialized message in the stream.
//!
//! \param  stream  Stream that holds the serialized messages.
//! \return Next message's type id.
inline const int peek_type(std::istream & stream)
{
    return stream.peek();
}

//! Returns Message's type id.
//!
//! \tparam Cons    Type list of all message types.
//! \tparam Message Message to search for.
//! \return Message's type id in Cons.
template <typename Cons, typename Message>
inline const int type_id()
{
    return types::IndexOf<Cons, Message>::value;
}

//! Message wrapper that helps with serialization.
//!
//! \tparam MessageImpl Message implementation.
//! \tparam MessageTypes    Type list of all message types.
template <typename MessageImpl, typename MessageTypes>
class MessageWrapper : public MessageImpl
{
public:
    //! Serialize the message to stream.
    //!
    //! \param  stream  Stream that holds the serialized message.
    void serialize(std::ostream & stream)
    {
        stream.put(type_id<MessageTypes, MessageImpl>());
        MessageImpl::serialize(stream);
    }

    //! Unserialize the message from stream.
    //!
    //! \param  stream  Stream that holds the serialized message.
    void unserialize(std::istream & stream)
    {
        if (stream.get() != type_id<MessageTypes, MessageImpl>())
        {
            throw "Mismatched message type";
        }

        MessageImpl::unserialize(stream);
    }
};
}
