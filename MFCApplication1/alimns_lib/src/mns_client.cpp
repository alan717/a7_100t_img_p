#include "mns_client.h"
#include "mns_utils.h"
#include "mns_network_tool.h"
#include "mns_common_tool.h"
#include "constants.h"

#ifdef _WIN32
#include "curl-win/curl.h"
#else
#include <curl/curl.h>
#endif

#include <sstream>
#include <ctime>
#include <iostream>

using namespace std;
using namespace mns::sdk;

class MNSGlobalFlagInit
{
public:
    MNSGlobalFlagInit()
    {
        curl_global_init(CURL_GLOBAL_ALL);
    }
    ~MNSGlobalFlagInit()
    {
        curl_global_cleanup();
    }
};
static MNSGlobalFlagInit sMNSInit;

MNSClient::MNSClient(const string& endpoint,
                     const string& accessId,
                     const string& accessKey,
                     const int32_t connPoolSize,
                     const int32_t timeout,
                     const int32_t connectTimeout)
    : mAccessId(accessId)
    , mAccessKey(accessKey)
    , mStsToken("")
{
    mMnsConnTool.reset(new MNSConnectionTool(connPoolSize, connectTimeout, timeout));

    mEndPoint = StringTool::RightTrimString(endpoint);
    mEndPoint = StringTool::RightTrimString(mEndPoint, '/');
}

MNSClient::MNSClient(const std::string& endpoint,
          const std::string& accessId,
          const std::string& accessKey,
          const std::string& stsToken,
          const int32_t connPoolSize,
          const int32_t timeout,
          const int32_t connectTimeout)
    : mAccessId(accessId)
    , mAccessKey(accessKey)
    , mStsToken(stsToken)
{
    mMnsConnTool.reset(new MNSConnectionTool(connPoolSize, connectTimeout, timeout));

    mEndPoint = StringTool::RightTrimString(endpoint);
    mEndPoint = StringTool::RightTrimString(mEndPoint, '/');
}

void MNSClient::updateAccessId(const std::string& accessId,
                               const std::string& accessKey)
{
    mAccessId = accessId;
    mAccessKey = accessKey;
    mStsToken = "";
}

void MNSClient::updateAccessId(const std::string& accessId,
                               const std::string& accessKey,
                               const std::string& stsToken)
{
    mAccessId = accessId;
    mAccessKey = accessKey;
    mStsToken = stsToken;
}

void MNSClient::sendRequest(Request& request,
                            Response& response,
                            const std::string& endpoint,
                            const std::string& accessId,
                            const std::string& accessKey,
                            const std::string& stsToken,
                            MNSConnectionToolPtr mnsConnTool)
{
    MNSClient::signRequest(request, endpoint, accessId, accessKey, stsToken);
    MNSNetworkTool::SendRequest(endpoint,
                                request,
                                response,
                                mnsConnTool);
}

void MNSClient::signRequest(Request& req,
                            const std::string& endpoint,
                            const std::string& accessId,
                            const std::string& accessKey,
                            const std::string& stsToken)
{
    if (stsToken != "")
    {
        req.setHeader(SECURITY_TOKEN, stsToken);
    }
    const std::string& canonicalizedResource = req.generateCanonicalizedResource();
    const std::string& requestBody = req.generateRequestBody();
    size_t contentLength = strlen(requestBody.c_str());

    size_t pos = endpoint.find_first_of("//");
    req.setHeader(HOST, endpoint.substr(pos + 2));
    req.setHeader(CONTENT_TYPE, DEFAULT_CONTENT_TYPE);
    req.setHeader(DATE, TimeTool::GetDateTime());
    req.setHeader(MNS_VERSION, CURRENT_VERSION);
    if (contentLength > 0)
        req.setHeader(CONTENT_LENGTH, StringTool::ToString(contentLength));
    req.setHeader(AUTHORIZATION,
        MNSNetworkTool::Signature(req.getMethod(), canonicalizedResource,
            accessId, accessKey, req.getHeaders()));

}

void MNSClient::getAccountAttributes(AccountAttributes& attributes)
{
    GetAccountAttributesRequest req;
    GetAccountAttributesResponse resp(attributes);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void MNSClient::setAccountAttributes(const AccountAttributes& attributes)
{
    SetAccountAttributesRequest req(attributes);
    SetAccountAttributesResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

QueuePtr MNSClient::createQueue(const std::string& queueName)
{
    CreateQueueRequest req(queueName);
    CreateQueueResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);

    return QueuePtr(new Queue(queueName, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool));
}

QueuePtr MNSClient::createQueue(const std::string& queueName,
                  const QueueAttributes& attributes)
{
    CreateQueueRequest req(queueName, attributes);
    CreateQueueResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);

    return QueuePtr(new Queue(queueName, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool));
}

QueuePtr MNSClient::getQueueRef(const std::string& queueName)
{
    return QueuePtr(new Queue(queueName, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool));
}

void MNSClient::deleteQueue(const std::string& queueName)
{
    DeleteQueueRequest req(queueName);
    DeleteQueueResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void MNSClient::listQueue(const std::string& prefix,
    std::vector<std::string>& queueNames)
{
    std::string nextMarker;
    ListQueueRequest req(prefix);
    ListQueueResponse resp(queueNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void MNSClient::listQueue(const std::string& prefix,
    std::vector<std::string>& queueNames,
    std::string& nextMarker)
{
    ListQueueRequest req(prefix);
    ListQueueResponse resp(queueNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void MNSClient::listQueue(ListQueueRequest& req,
                          std::vector<std::string>& queueNames)
{
    std::string nextMarker;
    ListQueueResponse resp(queueNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void MNSClient::listQueue(ListQueueRequest& req,
                          std::vector<std::string>& queueNames,
                          std::string& nextMarker)
{
    ListQueueResponse resp(queueNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

TopicPtr MNSClient::createTopic(const std::string& topicName)
{
    CreateTopicRequest req(topicName);
    CreateTopicResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);

    return TopicPtr(new Topic(topicName, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool));
}

TopicPtr MNSClient::createTopic(const std::string& topicName,
                                const TopicAttributes& attributes)
{
    CreateTopicRequest req(topicName, attributes);
    CreateTopicResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);

    return TopicPtr(new Topic(topicName, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool));
}

TopicPtr MNSClient::getTopicRef(const std::string& topicName)
{
    return TopicPtr(new Topic(topicName, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool));
}

void MNSClient::deleteTopic(const std::string& topicName)
{
    DeleteTopicRequest req(topicName);
    DeleteTopicResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void MNSClient::listTopic(const std::string& prefix,
                          std::vector<std::string>& topicNames)
{
    std::string nextMarker;
    ListTopicRequest req(prefix);
    ListTopicResponse resp(topicNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void MNSClient::listTopic(const std::string& prefix,
                          std::vector<std::string>& topicNames,
                          std::string& nextMarker)
{
    ListTopicRequest req(prefix);
    ListTopicResponse resp(topicNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void MNSClient::listTopic(ListTopicRequest& req,
                          std::vector<std::string>& topicNames)
{
    std::string nextMarker;
    ListTopicResponse resp(topicNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void MNSClient::listTopic(ListTopicRequest& req,
                          std::vector<std::string>& topicNames,
                          std::string& nextMarker)
{
    ListTopicResponse resp(topicNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

Queue::Queue(const std::string& queueName,
      const std::string& endpoint,
      const std::string& accessId,
      const std::string& accessKey,
      const std::string& stsToken,
      MNSConnectionToolPtr mnsConnTool)
    : mQueueName(queueName)
    , mEndPoint(endpoint)
    , mAccessId(accessId)
    , mAccessKey(accessKey)
    , mStsToken(stsToken)
    , mMnsConnTool(mnsConnTool)
{
}

void Queue::updateAccessId(const std::string& accessId,
                               const std::string& accessKey)
{
    mAccessId = accessId;
    mAccessKey = accessKey;
    mStsToken = "";
}

void Queue::updateAccessId(const std::string& accessId,
                               const std::string& accessKey,
                               const std::string& stsToken)
{
    mAccessId = accessId;
    mAccessKey = accessKey;
    mStsToken = stsToken;
}

void Queue::getAttributes(QueueAttributes& attributes)
{
    GetQueueAttributesRequest req(mQueueName);
    GetQueueAttributesResponse resp(attributes);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::setAttributes(const QueueAttributes& attributes)
{
    SetQueueAttributesRequest req(mQueueName, attributes);
    SetQueueAttributesResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::sendMessage(const std::string& messageBody,
                 SendMessageResponse& resp)
{
    SendMessageRequest req(messageBody, -1, -1);
    req.setQueueName(mQueueName);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::sendMessage(const std::string& messageBody,
                        const int32_t delaySeconds,
                        const int32_t priority,
                        SendMessageResponse& resp)
{
    SendMessageRequest req(messageBody, delaySeconds, priority);
    req.setQueueName(mQueueName);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::batchSendMessage(
    const std::vector<SendMessageItem>& sendMessageItems,
    BatchSendMessageResponse& resp)
{
    BatchSendMessageRequest req(sendMessageItems);
    req.setQueueName(mQueueName);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::peekMessage(Message& message)
{
    PeekMessageRequest req;
    req.setQueueName(mQueueName);
    PeekMessageResponse resp(message);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::batchPeekMessage(const int32_t numOfMessages,
    std::vector<Message>& messages)
{
    BatchPeekMessageRequest req(numOfMessages);
    req.setQueueName(mQueueName);
    BatchPeekMessageResponse resp(messages);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::receiveMessage(Message& message)
{
    ReceiveMessageRequest req;
    req.setQueueName(mQueueName);
    ReceiveMessageResponse resp(message);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::receiveMessage(const int32_t waitSeconds,
                           Message& message)
{
    ReceiveMessageRequest req(waitSeconds);
    req.setQueueName(mQueueName);
    ReceiveMessageResponse resp(message);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::batchReceiveMessage(const int32_t numOfMessages,
                                std::vector<Message>& messages)
{
    BatchReceiveMessageRequest req(numOfMessages);
    req.setQueueName(mQueueName);
    BatchReceiveMessageResponse resp(messages);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::batchReceiveMessage(const int32_t numOfMessages,
                                const int32_t waitSeconds,
                                std::vector<Message>& messages)
{
    BatchReceiveMessageRequest req(numOfMessages, waitSeconds);
    req.setQueueName(mQueueName);
    BatchReceiveMessageResponse resp(messages);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::deleteMessage(const std::string& receiptHandle)
{
    DeleteMessageRequest req(receiptHandle);
    req.setQueueName(mQueueName);
    DeleteMessageResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::batchDeleteMessage(const std::vector<std::string>& receiptHandles,
                               BatchDeleteMessageResponse& resp)
{
    BatchDeleteMessageRequest req(receiptHandles);
    req.setQueueName(mQueueName);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Queue::changeMessageVisibility(const std::string& receiptHandle,
                                    const int32_t visibilityTimeout,
                                    ChangeMessageVisibilityResponse& resp)
{
    ChangeMessageVisibilityRequest req(receiptHandle, visibilityTimeout);
    req.setQueueName(mQueueName);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

Topic::Topic(const std::string& topicName,
             const std::string& endpoint,
             const std::string& accessId,
             const std::string& accessKey,
             const std::string& stsToken,
             MNSConnectionToolPtr mnsConnTool)
    : mTopicName(topicName)
    , mEndPoint(endpoint)
    , mAccessId(accessId)
    , mAccessKey(accessKey)
    , mStsToken(stsToken)
    , mMnsConnTool(mnsConnTool)
{
    string host = StringTool::LeftTrimString(mEndPoint);
    if (StringTool::StartWith(host, "http://"))
    {
        host = host.substr(7);
    } 
    else if (StringTool::StartWith(host, "https://"))
    {
        host = host.substr(8);
    }

    vector<std::string> hostPieces = StringTool::StringToVector(host, ".");
    if (hostPieces.size() < 3)
    {
        MNS_THROW(MNSExceptionBase, "InvalidEndpoint");
    }
    mAccountId = hostPieces[0];
    mRegion = hostPieces[2];
    std::size_t pos = mRegion.find("-internal");
    if (pos != std::string::npos)
    {
        mRegion = mRegion.substr(0, pos);
    }
}

void Topic::updateAccessId(const std::string& accessId,
                           const std::string& accessKey)
{
    mAccessId = accessId;
    mAccessKey = accessKey;
    mStsToken = "";
}

void Topic::updateAccessId(const std::string& accessId,
                           const std::string& accessKey,
                           const std::string& stsToken)
{
    mAccessId = accessId;
    mAccessKey = accessKey;
    mStsToken = stsToken;
}

void Topic::getAttributes(TopicAttributes& attributes)
{
    GetTopicAttributesRequest req(mTopicName);
    GetTopicAttributesResponse resp(attributes);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::setAttributes(const TopicAttributes& attributes)
{
    SetTopicAttributesRequest req(mTopicName, attributes);
    SetTopicAttributesResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

std::string Topic::generateQueueEndpoint(const std::string& queueName)
{
    return "acs:mns:" + mRegion + ":" + mAccountId + ":queues/" + queueName;
}

std::string Topic::generateQueueEndpoint(const std::string& queueName, const std::string& region)
{
    return "acs:mns:" + region + ":" + mAccountId + ":queues/" + queueName;
}

std::string Topic::generateMailEndpoint(const std::string& mailAddress)
{
    return "mail:directmail:" + mailAddress;
}

std::string Topic::generateSMSEndpoint(const std::string& phone)
{
    if (!phone.empty())
        return "sms:directsms:" + phone;
    else
        return "sms:directsms:anonymous";
}

void Topic::publishMessage(const std::string& messageBody,
                           PublishMessageResponse& resp)
{
    PublishMessageRequest req(mTopicName, messageBody);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::publishMessage(const std::string& messageBody,
                           const MessageAttributes& messageAttributes,
                           PublishMessageResponse& resp)
{
    PublishMessageRequest req(mTopicName, messageBody, messageAttributes);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::publishMessage(const std::string& messageBody,
                           const std::string& messageTag,
                           PublishMessageResponse& resp)
{
    PublishMessageRequest req(mTopicName, messageBody, messageTag);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::publishMessage(const std::string& messageBody,
                           const std::string& messageTag,
                           const MessageAttributes& messageAttributes,
                           PublishMessageResponse& resp)
{
    PublishMessageRequest req(mTopicName, messageBody, messageTag, messageAttributes);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::subscribe(const std::string& subscriptionName,
                      const std::string& endPoint)
{
    SubscribeRequest req(mTopicName, subscriptionName, endPoint);
    SubscribeResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::subscribe(const std::string& subscriptionName,
                      const std::string& endPoint,
                      const SubscriptionAttributes& attributes)
{
    SubscribeRequest req(mTopicName, subscriptionName, endPoint, attributes);
    SubscribeResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::subscribe(const std::string& subscriptionName,
                      const std::string& filterTag,
                      const std::string& endPoint)
{
    SubscribeRequest req(mTopicName, subscriptionName, filterTag, endPoint);
    SubscribeResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::subscribe(const std::string& subscriptionName,
                      const std::string& filterTag,
                      const std::string& endPoint,
                      const SubscriptionAttributes& attributes)
{
    SubscribeRequest req(mTopicName, subscriptionName, filterTag, endPoint, attributes);
    SubscribeResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::setSubscriptionAttributes(const std::string& subscriptionName,
                                      const SubscriptionAttributes& attributes)
{
    SetSubscriptionAttributesRequest req(mTopicName,
        subscriptionName, attributes);
    SetSubscriptionAttributesResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::getSubscriptionAttributes(const std::string& subscriptionName,
                                      SubscriptionAttributes& attributes)
{
    GetSubscriptionAttributesRequest req(mTopicName, subscriptionName);
    GetSubscriptionAttributesResponse resp(attributes);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::unsubscribe(const std::string& subscriptionName)
{
    UnsubscribeRequest req(mTopicName, subscriptionName);
    UnsubscribeResponse resp;
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::listSubscription(const std::string& prefix,
                             std::vector<std::string>& subscriptionNames)
{
    std::string nextMarker;
    ListSubscriptionRequest req(prefix, "", -1);
    req.setTopicName(mTopicName);
    ListSubscriptionResponse resp(subscriptionNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::listSubscription(const std::string& prefix,
                             std::vector<std::string>& subscriptionNames,
                             std::string& nextMarker)
{
    ListSubscriptionRequest req(prefix, "", -1);
    req.setTopicName(mTopicName);
    ListSubscriptionResponse resp(subscriptionNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::listSubscription(ListSubscriptionRequest& req,
                             std::vector<std::string>& subscriptionNames)
{
    std::string nextMarker;
    req.setTopicName(mTopicName);
    ListSubscriptionResponse resp(subscriptionNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}

void Topic::listSubscription(ListSubscriptionRequest& req,
                             std::vector<std::string>& subscriptionNames,
                             std::string& nextMarker)
{
    req.setTopicName(mTopicName);
    ListSubscriptionResponse resp(subscriptionNames, nextMarker);
    MNSClient::sendRequest(req, resp, mEndPoint, mAccessId,
        mAccessKey, mStsToken, mMnsConnTool);
}
