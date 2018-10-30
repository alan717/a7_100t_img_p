//! Compile time type list.
//!
//! This file contains the tools to build and work with
//! compile time type lists.
#pragma once

#include <climits>

namespace types
{
namespace detail
{
class None { };
}

//! List item
//!
//! This is similar to how lists work in functional languages, it's
//! made up of a head and a tail.
//!
//! The list's head can be accessed in Cons::Head and the tail can
//! be accessed in Cons::Tail.
//!
//! \tparam H   The list head, it can be any type.
//! \tparam T   The list tail, it should be another Cons or None.
template <typename H, typename T = detail::None>
struct Cons
{
    typedef H Head;
    typedef T Tail;
};

//! Check if two types are equal.
//!
//! If T1 and T2 are the same type, AreEqual::value will be true,
//! else it will be false.
//!
//! \tparam T1  The first type to compare.
//! \tparam T2  The second type to compare.
template <typename T1, typename T2>
struct AreEqual
{
    static const bool value = false;
};

template <typename T>
struct AreEqual<T, T>
{
    static const bool value = true;
};

//! Select between two integers.
//!
//! If Cond is true, Select::value will contain I1, else
//! it will contain I2.
//!
//! \tparam Cond    The select condition.
//! \tparam I1      Value for true.
//! \tparam I2      Value for false.
template <bool Cond, int I1, int I2>
struct Select;

template <int I1, int I2>
struct Select<true, I1, I2>
{
    static const int value = I1;
};

template <int I1, int I2>
struct Select<false, I1, I2>
{
    static const int value = I2;
};

//! Index of type.
//!
//! Given a type list and a type, IndexOf::value will contain
//! the type's index in the list. IndexOf::value will be undefined
//! if T is not in the list.
//!
//! \tparam Cons    The type list to search.
//! \tparam T       Type to find.
template <typename Cons, typename T>
struct IndexOf;

template <typename Cons, typename T>
struct IndexOf
{
    static const int value = Select<AreEqual<typename Cons::Head, T>::value, 0, IndexOf<typename Cons::Tail, T>::value + 1>::value;
};

template <typename T>
struct IndexOf<detail::None, T>
{
    static const int value = INT_MIN;
};

//! List length.
//!
//! Given a type list, Count::value will contain it's length.
//!
//! \tparam Cons    Type list.
template <typename Cons>
struct Count;

template <typename Cons>
struct Count
{
    static const int value = Count<typename Cons::Tail>::value + 1;
};

template <>
struct Count<detail::None>
{
    static const int value = 0;
};
}
