#include <time.h>
#ifdef _WIN32
#include "openssl/hmac.h"
#include "openssl/evp.h"
#include "openssl/bio.h"
#include "openssl/buffer.h"
#else
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#endif
#include <errno.h>
#include "pugixml.hpp"
#include "mns_common_tool.h"
#include "mns_network_tool.h"
#include "mns_exception.h"
#include "constants.h"
#include <ctime>
#include <iostream>

using namespace std;
using namespace mns::sdk;

static int32_t BUFFER_SIZE = 10240;

static size_t Stream_read(void *buffer, size_t size, size_t nmemb, void* stream)
{
    memcpy(buffer, (*(static_cast<string *>(stream))).data(), (*(static_cast<string *>(stream))).size());
    return (*(static_cast<string *>(stream))).size();
}

static size_t Stream_write(void *buffer, size_t size, size_t nmemb, void* stream)
{
    *(static_cast<string *>(stream)) += string(static_cast<char *>(buffer), size*nmemb);
    return size*nmemb;
}

void MNSNetworkTool::SendRequest(const std::string& endpoint,
                                 Request& req,
                                 Response& resp,
                                 MNSConnectionToolPtr mnsConTool)
{
    int retry = 3;
    while (true)
    {
        try
        {
            string receiveHeader;
            bool isLongConnection = true;
            CURL* curl = mnsConTool->InvokeCurlConnection(isLongConnection);
            curl_slist* header = NULL;
            const std::map<std::string, std::string> headers = req.getHeaders();
            for (std::map<std::string, std::string>::const_iterator iter = headers.begin();
                iter != headers.end(); iter++)
            {
                header = curl_slist_append(header, (iter->first + ":" + iter->second).c_str());
            }
            header = curl_slist_append(header, "Connection: keep-alive");

            std::string url = endpoint + req.getCanonicalizedResource();
            curl_easy_setopt( curl, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt( curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt( curl, CURLOPT_HTTPHEADER, header);
            curl_easy_setopt( curl, CURLOPT_BUFFERSIZE, BUFFER_SIZE);
            if (!isLongConnection)
                curl_easy_setopt( curl, CURLOPT_FORBID_REUSE, 1);
            curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, &Stream_write);
            resp.clearRawData();
            curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)(resp.getRawDataPtr()));
            curl_easy_setopt( curl, CURLOPT_WRITEHEADER, (void *)(&receiveHeader));
            if (req.getMethod() == "PUT" || req.getMethod() == "POST")
            {
                if (req.getMethod() == "PUT")
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
                else
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
                const std::string& requestBody = req.getRequestBody();
                if (requestBody != "")
                {
                    curl_easy_setopt( curl, CURLOPT_READFUNCTION, &Stream_read);
                    curl_easy_setopt( curl, CURLOPT_READDATA, (void *)(&requestBody));
                    curl_easy_setopt( curl, CURLOPT_INFILESIZE_LARGE, requestBody.size());
                    curl_easy_setopt( curl, CURLOPT_POSTFIELDS, requestBody.data());
                    curl_easy_setopt( curl, CURLOPT_POSTFIELDSIZE, requestBody.size());
                }
            }
            else if (req.getMethod() == "GET")
            {
                curl_easy_setopt( curl, CURLOPT_CUSTOMREQUEST, "GET");
            }
            else if ( req.getMethod() == "DELETE" )
            {
                curl_easy_setopt( curl, CURLOPT_CUSTOMREQUEST, "DELETE");
                const std::string& requestBody = req.getRequestBody();
                if (requestBody != "")
                {
                    curl_easy_setopt( curl, CURLOPT_READDATA, (void *)(&requestBody));
                    curl_easy_setopt( curl, CURLOPT_INFILESIZE_LARGE, requestBody.size());
                    curl_easy_setopt( curl, CURLOPT_POSTFIELDS, requestBody.data());
                    curl_easy_setopt( curl, CURLOPT_POSTFIELDSIZE, requestBody.size());
                }
            }

            CURLcode curlret;
            curlret = curl_easy_perform(curl);
            curl_slist_free_all(header);
            if (curlret != CURLE_OK)
            {
                string errMes = "Curl Send Request Fail, errorcode:" + StringTool::ToString(curlret) + " errno:" + StringTool::ToString(errno) + " errorStr:" + StringTool::ToString(curl_easy_strerror(curlret));
                mnsConTool->RevokeCurlConnection(curl, isLongConnection);
                MNS_THROW(MNSExceptionBase, errMes);
                return;
            }
            vector<string> tmpVec = StringTool::StringToVector(receiveHeader, "\n");
            for (vector<string>::iterator iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
            {
                if (iter->find(":") != string::npos)
                {
                    int pos = iter->find(":");
                    resp.setHeader(
                        StringTool::TrimString(iter->substr(0, pos)),
                        StringTool::TrimString(iter->substr(pos+1)));
                }
                else if (iter->find("HTTP") != string::npos)
                {
                    vector<string> v = StringTool::StringToVector(*iter, " ");
                    resp.setStatus(atoi(v[1].c_str()));
                }
            }
            mnsConTool->RevokeCurlConnection(curl, isLongConnection);
            resp.parseResponse();
            return;
        }
        catch (MNSExceptionBase& e)
        {
            if (retry > 0 && resp.getStatus() >= 500)
            {
                retry--;
                continue;
            }
            throw;
        }
    }
}

std::string MNSNetworkTool::Signature(const std::string& method,
                                      const std::string& canonicalizedResource,
                                      const std::string& accessId,
                                      const std::string& accessKey,
                                      const std::map<string, string>& headers)
{
    ostringstream os;
	os << method << '\n';
#define APPEND_IF_EXIST(key, newline) \
    {\
        std::map<string, string>::const_iterator iter = headers.find(string(key)); \
        if (iter != headers.end()) \
        os << iter->second; \
        if (newline) \
        os << '\n'; \
    }
    APPEND_IF_EXIST(CONTENT_MD5, true);
    APPEND_IF_EXIST(CONTENT_TYPE, true);
    APPEND_IF_EXIST(DATE, true);
	std::map<string, string>::const_iterator iter = headers.lower_bound(CANONICALIZED_MNS_HEADER_PREFIX);
    while (iter != headers.end() && StringTool::StartWith(iter->first, CANONICALIZED_MNS_HEADER_PREFIX))
    {
        os << iter->first << ':' << iter->second << '\n';
        ++iter;
    }
#undef APPEND_IF_EXIST
    os << canonicalizedResource;
    string stringToSign = os.str();
    return Sign(stringToSign.c_str(), accessId, accessKey);
}

string MNSNetworkTool::Sign(const char* data, const std::string& accessId, const std::string& accessKey)
{
    char sha1sum[EVP_MAX_MD_SIZE];
    unsigned int sha1len;
    HMAC(EVP_sha1(), accessKey.c_str(), strlen(accessKey.c_str()), (const unsigned char*) data, strlen(data), (unsigned char*) sha1sum, &sha1len);
    string base64_str = Base64(sha1sum, sha1len);
    return "MNS " + accessId + ":" + base64_str;
}

string MNSNetworkTool::Base64(const char *input, unsigned int& length)
{
    istringstream eis;
    ostringstream eos;
    string str(input, length);
    eis.str(str);
    eos.str("");
    Base64Encoding(eis, eos);
    return eos.str();
}

MNSConnectionTool::~MNSConnectionTool()
{
    int retry = mCurlPoolSize + 10;
    do
    {
        PTScopedLock lock(mWaitObject);
        if (!mCurlPool.empty())
        {
            CURL* curl = mCurlPool.front();
            if (curl)
            {
                mCurlPool.pop();
                curl_easy_cleanup(curl);
                mCurrentPoolSize--;
            }
        }
        else
            mWaitObject.wait(1000);
    }while(retry-- > 0);
}

CURL* MNSConnectionTool::InvokeCurlConnection(bool& isLongConnection)
{
    int retry = 3;
    isLongConnection = true;
    do
    {
        PTScopedLock lock(mWaitObject);
        if (!mCurlPool.empty())
        {
            CURL* curl = mCurlPool.front();
            mCurlPool.pop();
            return curl;
        }
        else if (mCurrentPoolSize < mCurlPoolSize)
        {
            CURL* curl = curl_easy_init();
            if (curl)
                mCurrentPoolSize++;
            curl_easy_setopt( curl, CURLOPT_TIMEOUT, mTimeout);
            curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, mConnectTimeout);
            return curl;
        }
        if (--retry > 0)
            mWaitObject.wait(100);
        else
            break;
    }while(true);
    isLongConnection = false;
    CURL* curl = curl_easy_init();
    curl_easy_setopt( curl, CURLOPT_TIMEOUT, mTimeout);
    curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, mConnectTimeout);
    return curl;
}

void MNSConnectionTool::RevokeCurlConnection(CURL* curlConnection, const bool isLongConnection)
{
    if (isLongConnection)
    {
        PTScopedLock lock(mWaitObject);
        if (curlConnection)
        {
            mCurlPool.push(curlConnection);
            mWaitObject.signal();
        }
    }
    else
    {
        curl_easy_cleanup(curlConnection);
    }
}

void MNSNetworkTool::Base64Encoding(std::istream& is, std::ostream& os, char makeupChar, const char *alphabet)
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
