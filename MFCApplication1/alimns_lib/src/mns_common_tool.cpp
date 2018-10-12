#include "mns_common_tool.h"
#include "mns_exception.h"

#include <string>
#include <map>
#ifdef _WIN32
#include <time.h>
#endif

using namespace std;
using namespace mns::sdk;

std::vector<std::string> StringTool::StringToVector(const std::string& str, const std::string& delim)
{
    if (str.size() == 0 || delim.size() == 0)
    {
        return std::vector<std::string>();
    }
    std::vector<std::string> part;
    size_t pos = 0;
    while (pos <= str.size())
    {
        size_t pos2 = str.find_first_of(delim, pos);
        if (pos2 == std::string::npos)
        {
            pos2 = str.size();
        }
        part.push_back(str.substr(pos, pos2-pos));
        pos = pos2 + 1;
    }
    return part;
}

std::string StringTool::LeftTrimString(const std::string& string, const char trimChar)
{
    size_t pos = 0;
    while (pos < string.size() && string[pos] == trimChar) ++pos;
    return string.substr(pos);
}

std::string StringTool::RightTrimString(const std::string& string, const char trimChar)
{
    size_t pos = string.size() - 1;
    while (pos != (size_t)-1  && string[pos] == trimChar) --pos;
    return string.substr(0, pos+1);
}

std::string StringTool::TrimString(
        const std::string& string,
        const char leftTrimChar,
        const char rightTrimChar)
{
    return LeftTrimString(RightTrimString(string, rightTrimChar), leftTrimChar);
}

bool StringTool::StartWith(const std::string& input, const std::string& pattern)
{
    if (input.length() < pattern.length())
        return false;

    size_t i = 0;
    while (i < pattern.length() && input[i] == pattern[i])
        i++;

    return i == pattern.length();
}


void Base64Tool::Base64Encoding(std::istream& is, std::ostream& os, char makeupChar, const char *alphabet)
{
    int out[4];
    int remain = 0;
    while (!is.eof())
    {
        int byte1 = is.get();
        if (byte1 < 0)
        {
            break;
        }
        int byte2 = is.get();
        int byte3;
        if (byte2 < 0)
        {
            byte2 = 0;
            byte3 = 0;
            remain = 1;
        }
        else
        {
            byte3 = is.get();
            if (byte3 < 0)
            {
                byte3 = 0;
                remain = 2;
            }
        }
        out[0] = static_cast<unsigned>(byte1) >> 2;
        out[1] = ((byte1 & 0x03) << 4) + (static_cast<unsigned>(byte2) >> 4);
        out[2] = ((byte2 & 0x0F) << 2) + (static_cast<unsigned>(byte3) >> 6);
        out[3] = byte3 & 0x3F;

        if (remain == 1)
        {
            os.put(out[0] = alphabet[out[0]]);
            os.put(out[1] = alphabet[out[1]]);
            os.put(makeupChar);
            os.put(makeupChar);
        }
        else if (remain == 2)
        {
            os.put(out[0] = alphabet[out[0]]);
            os.put(out[1] = alphabet[out[1]]);
            os.put(out[2] = alphabet[out[2]]);
            os.put(makeupChar);
        }
        else
        {
            os.put(out[0] = alphabet[out[0]]);
            os.put(out[1] = alphabet[out[1]]);
            os.put(out[2] = alphabet[out[2]]);
            os.put(out[3] = alphabet[out[3]]);
        }
    }
}

//Base64Decoding
void Base64Tool::Base64Decoding(std::istream& is, std::ostream& os, char plus, char slash)
{
    int out[3];
    int byte[4];
    int bTmp;
    int bTmpNext;
    const int numOfAlpha = 26;
    const int numOfDecimalNum = 10;
    const int numOfBase64 = 64;
    int index = -1;

    while (is.peek() >= 0)
    {
        byte[0] = byte[1] = byte[2] = byte[3] = 0;
        out[0] = out[1] = out[2] = 0;
        bTmp = 0;

        for (int i = 0; i < 4; i++)
        {
            bTmp = is.get();

            if (bTmp == '=')
            {
                index = i;
                break;
            }
            else if (bTmp >= 'A' && bTmp <= 'Z')
            {
                byte[i] = bTmp - 'A';
            }
            else if (bTmp >= 'a' && bTmp <= 'z')
            {
                byte[i] = bTmp - 'a' + numOfAlpha;
            }
            else if (bTmp >= '0' && bTmp <= '9')
            {
                byte[i] = bTmp + numOfAlpha * 2 - '0';
            }
            else if (bTmp == plus)
            {
                byte[i] = numOfAlpha * 2 + numOfDecimalNum;
            }
            else if (bTmp == slash)
            {
                byte[i] = numOfBase64 - 1;
            }
            else if (bTmp < 0)
            {
                MNS_THROW(MNSExceptionBase, "Bad Base64 Message Body");
            }
            else
            {
                MNS_THROW(MNSExceptionBase, "Bad Base64 Message Body, String should only contain::ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=");
            }
        }

        out[0] = (byte[0] << 2) + (static_cast<unsigned>(byte[1]) >> 4) ;
        out[1] = ((byte[1] & 0x0F) << 4) + (static_cast<unsigned>(byte[2]) >> 2);
        out[2] = ((byte[2] & 0x03) << 6) + byte[3];

        if (bTmp == '=')
        {
            if (index == 0 || index == 1)
            {
                MNS_THROW(MNSExceptionBase, "Bad Base64 Message Body, String should be the third or fourth place in the last four characters");
            }
            else if (index == 2)
            {
                bTmpNext = is.get();
                if (bTmpNext == '=')
                {
                    if (is.peek() < 0)
                    {
                        os.put(out[0]);
                    }
                    else
                    {
                        MNS_THROW(MNSExceptionBase, "Bad Base64 Message Body, String should not append any character after ==");
                    }
                }
                else
                {
                    MNS_THROW(MNSExceptionBase, "Bad Base64 Message Body, String should not append any character after = except =");
                }
            }
            else
            {
                if (is.peek() < 0)
                {
                    os.put(out[0]);
                    os.put(out[1]);
                }
                else
                {
                    MNS_THROW(MNSExceptionBase, "Bad Base64 Message Body, String should not append any character after the first =");
                }
            }
        }
        else
        {
            os.put(out[0]);
            os.put(out[1]);
            os.put(out[2]);
        }
    }
}

string TimeTool::GetDateTime()
{
    time_t now = time(NULL);
    char timeBuffer[80];
#ifdef _WIN32
	struct tm* utc_tm_ptr = gmtime(&now);
	if (utc_tm_ptr == NULL)
	{
		MNS_THROW(MNSExceptionBase, "Cannot get current tiem!");
}
#else
	struct tm utc_tm;
    struct tm* r= gmtime_r(&now, &utc_tm);
	if (r == NULL)
	{
		MNS_THROW(MNSExceptionBase, "Cannot get current tiem!");
	}
#endif
    
#ifdef _WIN32
	strftime(timeBuffer, sizeof(timeBuffer), "%a, %d %b %Y %H:%M:%S GMT", utc_tm_ptr);
#else
    strftime(timeBuffer, sizeof(timeBuffer), "%a, %d %b %Y %H:%M:%S GMT", &utc_tm);
#endif
    return timeBuffer;
}
