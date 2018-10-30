#pragma once
#include <iostream>

namespace messages
{
class MessageA
{
protected:
    void serialize(std::ostream &) { }
    void unserialize(std::istream &) { }
};

class MessageB
{
protected:
    void serialize(std::ostream &) { }
    void unserialize(std::istream &) { }
};

class MessageC
{
protected:
    void serialize(std::ostream &) { }
    void unserialize(std::istream &) { }
};
}
