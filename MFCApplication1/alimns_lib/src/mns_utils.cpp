#include "mns_utils.h"

#include <iomanip>

using namespace std;
using namespace mns::sdk;

string MNSUtils::getLastNodeFromURL(const string& inputURL)
{
    if (inputURL == "")
    {
        return "";
    }

    size_t pos = inputURL.find_last_of("/");
    if (pos == string::npos)
    {
        return inputURL;
    }

    return inputURL.substr(pos + 1);
}

string MNSUtils::escapeJson(const std::string &s) {
    std::ostringstream o;
    for (std::string::const_iterator c = s.begin(); c != s.end(); c++) {
        switch (*c) {
        case '"': o << "\\\""; break;
        case '\\': o << "\\\\"; break;
        case '\b': o << "\\b"; break;
        case '\f': o << "\\f"; break;
        case '\n': o << "\\n"; break;
        case '\r': o << "\\r"; break;
        case '\t': o << "\\t"; break;
        default:
            if ('\x00' <= *c && *c <= '\x1f') {
                o << "\\u"
                  << std::hex << std::setw(4) << std::setfill('0') << (int)*c;
            } else {
                o << *c;
            }
        }
    }
    return o.str();
}

std::string MNSUtils::toJsonStr(const std::map<std::string, std::string>& param)
{
    string str = "{";
	std::map<std::string, std::string>::const_reverse_iterator lastIter = param.rbegin();
    for (std::map<std::string, std::string>::const_iterator iter = param.begin();
            iter != param.end(); iter++)
    {
        str += "\"" + MNSUtils::escapeJson(iter->first) + "\":\"" 
                        + MNSUtils::escapeJson(iter->second) + "\"";
        if (iter->first != lastIter->first)
        {
            str += ",";
        }
    }
    str += "}";
    return str;
}
