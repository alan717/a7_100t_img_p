// Copyright (C) 2015, Alibaba Cloud Computing

#ifndef MNS_SDK_COMMON_TOOL_H
#define MNS_SDK_COMMON_TOOL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

namespace mns
{
namespace sdk
{

class StringTool
{
public:
    template <typename T>
    static std::string ToString(const T& i)
    {
        std::ostringstream s;
        s << i;
        return s.str();
    };
    static std::string TrimString(const std::string& str, const char leftTrimChar = ' ', const char rightTrimChar = ' ');
    static std::string LeftTrimString(const std::string& str, const char trimChar = ' ');
    static std::string RightTrimString(const std::string& str, const char trimChar = ' ');
    static std::vector<std::string> StringToVector(const std::string& string, const std::string& delim = ",");
    static bool StartWith(const std::string& input, const std::string& pattern);
};

class Base64Tool
{
public:
    static void Base64Encoding(std::istream&, std::ostream&, char makeupChar = '=', const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    static void Base64Decoding(std::istream&, std::ostream&, char plus = '+', char slash = '/');
};

class TimeTool
{
public:
    static std::string GetDateTime();
};

}
}

#endif
