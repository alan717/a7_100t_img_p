//! Message dispatcher.
//!
//! Implementation of a message dispatcher.
#pragma once
#include "MessageWrapper.h"
#include "TypeList.h"

#include <iostream>

namespace messages
{

//! Dispatcher implementation.
//!
//! \tparam Cons        Holds the type list during recursion.
//! \tparam OrigCons    Holds the original type list.
template <typename Cons, typename OrigCons>
class DispatcherImpl;

template <typename Cons, typename OrigCons>
class DispatcherImpl : public DispatcherImpl<typename Cons::Tail, OrigCons>
{
public:
    //! Unserialize and dispatch the message to the right handler.
    //!
    //! Recurse trough the type list and generate code to instantiate the
    //! right message type.
    //!
    //! \tparam Handler Handler type.
    //! \param  type_id     Id of message to unserialize.
    //! \param  stream      Data stream that contains the serialized message.
    //! \param  handler     Message handler instance.
    template <typename Handler>
    inline void do_dispatch(int type_id, std::istream & stream, Handler & handler)
    {
        if (type_id == messages::type_id<OrigCons, typename Cons::Head>())
        {
            MessageWrapper<typename Cons::Head, OrigCons> msg;
            msg.unserialize(stream);

            handler.handle(msg);
        }
        else
        {
            // Try the next dispatch method, with the type list's tail as a type list.
            DispatcherImpl<typename Cons::Tail, OrigCons>::
            do_dispatch(type_id, stream, handler);
        }
    }
};

template <typename OrigCons>
class DispatcherImpl<types::detail::None, OrigCons>
{
public:
    template <typename Handler>
    inline void do_dispatch(int, std::istream &, Handler &)
    {
        throw "Type not found";
    }
};

//! Message dispatcher.
//!
//! Unserialize and dispatch a message to the message handler.
//!
//! \tparam Cons    Type list that holds the messages to be unserialized.
template <typename Cons>
class Dispatcher : protected DispatcherImpl<Cons, Cons>
{
public:
    //! Dispatch the serialized message in stream to handler.
    //!
    //! \tparam Handler     Message handler type.
    //! \param  stream      Stream that holds the serialized message.
    //! \param  handler     Message handler instance.
    template <typename Handler>
    inline void dispatch(std::istream & stream, Handler & handler)
    {
        int type_id = peek_type(stream);

        this->do_dispatch(type_id, stream, handler);
    }
};
}

