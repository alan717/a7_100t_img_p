// Copyright (C) 2015, Alibaba Cloud Computing

#ifndef MNS_SDK_NETWORK_H
#define MNS_SDK_NETWORK_H

#include "mns_exception.h"
#include "mns_common_tool.h"
#include "mns_protocol.h"
#include "mns_utils.h"

#include <queue>
#ifdef _WIN32
#include "curl-win/curl.h"
#else
#include <curl/curl.h>
#endif

#include <iostream>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

namespace mns
{
namespace sdk
{

class MNSConnectionTool
{
public:
    MNSConnectionTool(const int32_t curlPoolSize, const int32_t connectTimeout,
        const int32_t timeout)
        : mConnectTimeout(connectTimeout)
        , mTimeout(timeout)
        , mCurlPoolSize(curlPoolSize)
        , mCurrentPoolSize(0)
    {
    }
    ~MNSConnectionTool();

    CURL* InvokeCurlConnection(bool& isLongConnection);
    void RevokeCurlConnection(CURL* curlConnection, const bool isLongConnection);

private:
    int32_t mConnectTimeout;
    int32_t mTimeout;
    int32_t mCurlPoolSize;
    int32_t mCurrentPoolSize;
    WaitObject mWaitObject;

    std::queue<CURL*> mCurlPool;
};
typedef std::tr1::shared_ptr<MNSConnectionTool> MNSConnectionToolPtr;

class MNSNetworkTool
{
public:
    static void SendRequest(const std::string& endpoint,
                            Request& req,
                            Response& resp,
                            MNSConnectionToolPtr mnsConnTool);

    static std::string Signature(const std::string& method,
                                 const std::string& canonicalizedResource,
                                 const std::string& accessId,
                                 const std::string& accessKey,
                                 const std::map<std::string, std::string>& headers);

protected:
    static std::string Sign(const char* data, const std::string& accessId, const std::string& accessKey);
    static std::string Base64(const char* input, unsigned int& length);
    static void Base64Encoding(std::istream&, std::ostream&, char makeupChar = '=',
                               const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
};

}
}

#endif
