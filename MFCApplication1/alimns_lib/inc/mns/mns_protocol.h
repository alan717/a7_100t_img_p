// Copyright (C) 2015, Alibaba Cloud Computing

#ifndef MNS_PROTOCOL_H
#define MNS_PROTOCOL_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <stdint.h>
#include "pugixml.hpp"
#include "mns_utils.h"

namespace mns
{
namespace sdk
{

enum NotifyStrategy
{
    UNKNOWN_STRATEGY,   // do not specify the strategy and use default value
    BACKOFF_RETRY,
    EXPONENTIAL_DECAY_RETRY
};

class NotifyStrategyHelper
{
public:
    static std::string NotifyStrategyToString(const NotifyStrategy notifyStrategy)
    {
        if (notifyStrategy == BACKOFF_RETRY)
        {
            return "BACKOFF_RETRY";
        }
        if (notifyStrategy == EXPONENTIAL_DECAY_RETRY)
        {
            return "EXPONENTIAL_DECAY_RETRY";
        }
        return "UNKNOWN_STRATEGY";
    }

    static NotifyStrategy StringToNotifyStrategy(const std::string& notifyStrategyStr)
    {
        if (notifyStrategyStr == "BACKOFF_RETRY")
        {
            return BACKOFF_RETRY;
        }
        if (notifyStrategyStr == "EXPONENTIAL_DECAY_RETRY")
        {
            return EXPONENTIAL_DECAY_RETRY;
        }
        return UNKNOWN_STRATEGY;
    }
};

enum ContentFormat
{
    UNKNOWN_FORMAT, // do not specify the format and use default value
    XML,
    SIMPLIFIED,
    JSON
};

class ContentFormatHelper
{
public:
    static std::string ContentFormatToString(const ContentFormat contentFormat)
    {
        switch (contentFormat)
        {
            case XML:
                return "XML";
            case SIMPLIFIED:
                return "SIMPLIFIED";
            case JSON:
                return "JSON";
            default:
                return "UNKNOWN_FORMAT";
        }
    }

    static ContentFormat StringToContentFormat(const std::string& contentFormatStr)
    {
        if (contentFormatStr == "XML")
        {
            return XML;
        }
        if (contentFormatStr == "SIMPLIFIED")
        {
            return SIMPLIFIED;
        }
        if (contentFormatStr == "JSON")
        {
            return JSON;
        }
        return UNKNOWN_FORMAT;
    }
};

enum SMSType
{
    SINGLE_SMS,
    BATCH_SMS
};

struct SendMessageItem
{
    SendMessageItem(const std::string& body = "",
                    const int32_t delaySec = -1,
                    const int32_t prio = -1)
        : messageBody(body)
        , delaySeconds(delaySec)
        , priority(prio)
    {
    }
    std::string messageBody;
    int32_t delaySeconds;
    int32_t priority;
};

struct SendMessageSucceedItem
{
    SendMessageSucceedItem()
        : messageId(""), messageBodyMD5("")
    {
    }
    std::string messageId;
    std::string messageBodyMD5;
};

struct DeleteMessageFailedItem
{
    DeleteMessageFailedItem()
        : errorCode("")
        , errorMessage("")
        , receiptHandle("")
    {
    }
    std::string errorCode;
    std::string errorMessage;
    std::string receiptHandle;
};

struct SendMessageFailedItem
{
    SendMessageFailedItem()
        : errorCode("")
        , errorMessage("")
    {
    }
    std::string errorCode;
    std::string errorMessage;
};

class MessageAttributes;

class MailAttributes
{
public:
    const std::string& getSubject();
    void setSubject(const std::string& subject);

    const std::string& getAccountName();
    void setAccountName(const std::string& accountName);

    int getAddressType();
    void setAddressType(int addressType);

    bool getReplyToAddress();
    void setReplyToAddress(bool replyToAddress);

    bool getIsHtml();
    void setIsHtml(bool isHtml);

    std::string toJson();

    friend class MessageAttributes;
protected:
    MailAttributes();

protected:
    /*
     * Please Refer to https://help.aliyun.com/document_detail/directmail/api-reference/sendmail-related/SingleSendMail.html
     */
    std::string mSubject; 
    std::string mAccountName; 
    int mAddressType; 
    bool mReplyToAddress; 
    bool mIsHtml; 
};

class SMSAttributes
{
public:
    std::string toJson();

    SMSType getType();

    std::string getSignName();
    void setSignName(const std::string& signName);

    std::string getTemplateCode();
    void setTemplateCode(const std::string& templateCode);

    std::string getReceiver();
    void setReceiver(const std::string& receiver);

    std::string getSingleParam(const std::string& key);
    void setSingleParam(const std::string& key, const std::string& value);

    std::map<std::string, std::string> getBatchParam(const std::string& phone);
    std::string getBatchParam(const std::string& phone, const std::string& key);
    void setBatchParam(const std::string& phone, const std::string& key, const std::string& value);
    void setBatchParam(const std::string& phone, const std::map<std::string, std::string>& params);

    friend class MessageAttributes;
protected:
    SMSAttributes(const SMSType type);

private:
    SMSType mType;
    std::string mSignName;
    std::string mTemplateCode;
    std::string mReceiver;
    std::map<std::string, std::string> mSingleParams;
    std::map<std::string, std::map<std::string, std::string> > mBatchParams;
};

class PublishMessageRequest;

class MessageAttributes
{
public:
    MessageAttributes()
        : mpMailAttributes(NULL)
        , mpSMSAttributes(NULL)
    {}

    virtual ~MessageAttributes()
    {
        if (mpMailAttributes != NULL)
        {
            delete mpMailAttributes;
        }
        if (mpSMSAttributes != NULL) 
        {
            delete mpSMSAttributes;
        }
    }

    /** Get MailAttributes Ptr
     *
     * @param isCreate: if isCreate is true and mailAttributes does not exist,
     *                  the mailAttributes will be created.
     */
    MailAttributes* getMailAttributesPtr(bool isCreate = true)
    {
        if (mpMailAttributes == NULL && isCreate)
        {
            PTScopedLock lock(mMutex);
            mpMailAttributes = new MailAttributes();
        }
        return mpMailAttributes;
    }

    SMSAttributes* getSMSAttributesPtr(const SMSType smsType = BATCH_SMS, bool isCreate = true)
    {
        if (mpSMSAttributes == NULL && isCreate)
        {
            PTScopedLock lock(mMutex);
            mpSMSAttributes = new SMSAttributes(smsType);
        }
        return mpSMSAttributes;
    }

    friend class PublishMessageRequest;
protected:
    MailAttributes* getMailAttributesPtrNoCreate() const
    {
        return mpMailAttributes;
    }

    SMSAttributes* getSMSAttributesPtrNoCreate() const
    {
        return mpSMSAttributes;
    }

protected:
    MailAttributes* mpMailAttributes;
    SMSAttributes* mpSMSAttributes;
    PTMutex mMutex;
};

class ReceiveMessageResponse;
class BatchReceiveMessageResponse;

class Message
{
public:
    Message()
        : mMessageId("")
        , mReceiptHandle("")
        , mMessageBody("")
        , mMessageBodyMD5("")
        , mEnqueueTime(-1)
        , mNextVisibleTime(-1)
        , mFirstDequeueTime(-1)
        , mDequeueCount(-1)
        , mPriority(-1)
    {
    }

    const std::string& getMessageId()
    {
        return mMessageId;
    }

    const std::string& getReceiptHandle()
    {
        return mReceiptHandle;
    }

    const std::string& getMessageBody()
    {
        return mMessageBody;
    }

    const std::string& getMessageBodyMD5()
    {
        return mMessageBodyMD5;
    }

    int64_t getEnqueueTime()
    {
        return mEnqueueTime;
    }

    int64_t getFirstDequeueTime()
    {
        return mFirstDequeueTime;
    }

    int64_t getNextVisibleTime()
    {
        return mNextVisibleTime;
    }

    int32_t getDequeueCount()
    {
        return mDequeueCount;
    }

    int32_t getPriority()
    {
        return mPriority;
    }

    friend class ReceiveMessageResponse;
    friend class BatchReceiveMessageResponse;
    friend class PeekMessageResponse;
    friend class BatchPeekMessageResponse;

protected:
    void initFromXml(const pugi::xml_node& messageNode);

protected:
    std::string mMessageId;
    std::string mReceiptHandle;
    std::string mMessageBody;
    std::string mMessageBodyMD5;
    int64_t mEnqueueTime;
    int64_t mNextVisibleTime;
    int64_t mFirstDequeueTime;
    int32_t mDequeueCount;
    int32_t mPriority;
};

class GetAccountAttributesResponse;
class SetAccountAttributesRequest;

class AccountAttributes
{
public:
    AccountAttributes()
        : mLoggingBucket("")
        , mLoggingBucketSet(false)
    {
    }

    void clear()
    {
        mLoggingBucket = "";
        mLoggingBucketSet = false;
    }

    void setLoggingBucket(const std::string& loggingBucket)
    {
        mLoggingBucket = loggingBucket;
        mLoggingBucketSet = true;
    }

    std::string getLoggingBucket() const
    {
        return mLoggingBucket;
    }

    friend class GetAccountAttributesResponse;
    friend class SetAccountAttributesRequest;

protected:
    bool isLoggingBucketSet() const
    {
        return mLoggingBucketSet;
    }

protected:
    std::string mLoggingBucket;
    bool mLoggingBucketSet;
};

class GetQueueAttributesResponse;
class SetQueueAttributesRequest;
class CreateQueueRequest;

class QueueAttributes
{
public:
    QueueAttributes();

    void clear();

    void setVisibilityTimeout(int64_t visibilityTimeout);
    void setDelaySeconds(int64_t delaySeconds);
    void setMaximumMessageSize(int64_t maximumMessageSize);
    void setMessageRetentionPeriod(int64_t messageRetentionPeriod);
    void setPollingWaitSeconds(int32_t pollingWaitSeconds);
    void setLoggingEnabled(bool loggingEnabled);

    std::string getQueueName() const;
    int32_t getPollingWaitSeconds() const;
    int64_t getVisibilityTimeout() const;
    int64_t getDelaySeconds() const;
    int64_t getMaximumMessageSize() const;
    int64_t getMessageRetentionPeriod() const;
    int64_t getActiveMessages() const;
    int64_t getInactiveMessages() const;
    int64_t getDelayMessages() const;
    int64_t getCreateTime() const;
    int64_t getLastModifyTime() const;
    bool isLoggingEnabled() const;

    friend class GetQueueAttributesResponse;
    friend class SetQueueAttributesRequest;
    friend class CreateQueueRequest;

protected:
    bool isLoggingEnabledSet() const;

protected:
    std::string mQueueName;
    int64_t mVisibilityTimeout;
    int64_t mDelaySeconds;
    int64_t mMaximumMessageSize;
    int64_t mMessageRetentionPeriod;
    int64_t mActiveMessages;
    int64_t mInactiveMessages;
    int64_t mDelayMessages;
    int64_t mCreateTime;
    int64_t mLastModifyTime;
    int32_t mPollingWaitSeconds;

    bool mLoggingEnabled;
    bool mLoggingEnabledSet;
};

class GetTopicAttributesResponse;
class SetTopicAttributesRequest;
class CreateTopicRequest;

class TopicAttributes
{
public:
    TopicAttributes();

    void clear();

    void setMaximumMessageSize(int64_t maximumMessageSize);
    void setLoggingEnabled(bool loggingEnabled);

    std::string getTopicName();
    int64_t getCreateTime() const;
    int64_t getLastModifyTime() const;
    int64_t getMaximumMessageSize() const;
    int64_t getMessageRetentionPeriod() const;
    int64_t getMessages() const;
    bool isLoggingEnabled() const;

    friend class GetTopicAttributesResponse;
    friend class SetTopicAttributesRequest;
    friend class CreateTopicRequest;

protected:
    bool isLoggingEnabledSet() const;

protected:
    std::string mTopicName;
    int64_t mCreateTime;
    int64_t mLastModifyTime;
    int64_t mMaximumMessageSize;
    int64_t mMessageRetentionPeriod;
    int64_t mMessages;  // count of messages in topic

    bool mLoggingEnabled;
    bool mLoggingEnabledSet;
};

class GetSubscriptionAttributesResponse;

class SubscriptionAttributes
{
public:
    SubscriptionAttributes();

    void setNotifyStrategy(NotifyStrategy notifyStrategy);
    void setContentFormat(ContentFormat contentFormat);

    std::string getSubscriptionName() const;
    std::string getTopicOwner() const;
    std::string getTopicName() const;
    std::string getFilterTag() const;
    std::string getEndPoint() const;
    NotifyStrategy getNotifyStrategy() const;
    ContentFormat getContentFormat() const;
    int64_t getCreateTime() const;
    int64_t getLastModifyTime() const;

    friend class GetSubscriptionAttributesResponse;
protected:
    std::string mSubscriptionName;
    std::string mTopicOwner;
    std::string mTopicName;
    std::string mEndPoint;
    std::string mFilterTag;
    NotifyStrategy mNotifyStrategy;
    ContentFormat mContentFormat;
    int64_t mCreateTime;
    int64_t mLastModifyTime;
};

class Request
{
public:
    Request(const std::string& method)
        : mMethod(method), mCanonicalizedResource(""), mRequestBody("")
    {}
    virtual ~Request() {};

    const std::string& getMethod() const
    {
        return mMethod;
    }

    const std::string& getCanonicalizedResource()
    {
        if (mCanonicalizedResource == "")
        {
            return generateCanonicalizedResource();
        }
        return mCanonicalizedResource;
    }

    const std::string& generateCanonicalizedResource()
    {
        mCanonicalizedResource = getResourcePath();
        if (getQueryString() != "")
        {
            mCanonicalizedResource += "?" + getQueryString();
        }
        return mCanonicalizedResource;
    }

    const std::string& getRequestBody()
    {
        if (mRequestBody == "")
        {
            return generateRequestBody();
        }
        return mRequestBody;
    }

    virtual std::string getQueryString() = 0;

    virtual std::string getResourcePath() = 0;

    virtual const std::string& generateRequestBody() = 0;

    const std::map<std::string, std::string>& getHeaders()
    {
        return mHeaders;
    }

    std::string getHeader(const std::string& key)
    {
		std::map<std::string, std::string>::iterator iter = mHeaders.find(key);
        if (iter != mHeaders.end())
        {
            return iter->second;
        }
        return "";
    }

    void setHeader(std::string key, std::string value)
    {
        mHeaders[key] = value;
    }

protected:
    std::string mMethod;
    std::string mCanonicalizedResource;
    std::string mRequestBody;
    std::map<std::string, std::string> mHeaders;
};

class Response
{
public:
    Response();
    virtual ~Response() {};

    int getStatus()
    {
        return mStatus;
    }

    void setStatus(int32_t status)
    {
        mStatus = status;
    }

    const std::map<std::string, std::string>& getHeaders()
    {
        return mHeaders;
    }

    void setHeader(std::string key, std::string value)
    {
        mHeaders[key] = value;
    }

    std::string getHeader(std::string key)
    {
		std::map<std::string, std::string>::iterator iter = mHeaders.find(key);
        if (iter != mHeaders.end())
        {
            return iter->second;
        }
        return NULL;
    }

    std::string* getRawDataPtr()
    {
        return &mRawData;
    }

    void clearRawData()
    {
        mRawData.clear();
    }

    virtual bool isSuccess() = 0;
    virtual void parseResponse() = 0;

protected:
    pugi::xml_node toXML();
    void parseCommonError(const pugi::xml_node& rootNode);
    bool isCommonError(const pugi::xml_node& rootNode);

protected:
    pugi::xml_document mDoc;
    std::map<std::string, std::string> mHeaders;
    std::string mRawData;
    int32_t mStatus;
};

class MessageRequest : public Request
{
public:
    MessageRequest(const std::string& method)
        : Request(method), mQueueName(NULL)
    {
    }

    virtual ~MessageRequest() {}

    std::string getResourcePath()
    {
        if (mQueueName == NULL)
            return "";
        return "/queues/" + *mQueueName + "/messages";
    }

    /* QueueName will be automatically set when sending request. */
    void setQueueName(const std::string& queueName)
    {
        mQueueName = &queueName;
    }

protected:
    const std::string* mQueueName;
};

class SubscriptionRequest : public Request
{
public:
    SubscriptionRequest(const std::string& method,
                        const std::string& topicName,
                        const std::string& subscriptionName)
        : Request(method)
        , mTopicName(&topicName)
        , mSubscriptionName(&subscriptionName)
    {
    }

    virtual ~SubscriptionRequest() {}

    std::string getResourcePath()
    {
        if (mTopicName == NULL || mSubscriptionName == NULL)
            return "";
        return "/topics/" + *mTopicName + "/subscriptions/" + *mSubscriptionName;
    }

protected:
    const std::string* mTopicName;
    const std::string* mSubscriptionName;
};

class GetAccountAttributesRequest : public Request
{
public:
    GetAccountAttributesRequest();
    virtual ~GetAccountAttributesRequest() {}

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();
};

class GetAccountAttributesResponse : public Response
{
public:
    GetAccountAttributesResponse(AccountAttributes& attributes);
    virtual ~GetAccountAttributesResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    AccountAttributes* mAccountAttributes;
};

class SetAccountAttributesRequest : public Request
{
public:
    SetAccountAttributesRequest(const AccountAttributes& accountAttributes);
    virtual ~SetAccountAttributesRequest() {}

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();

protected:
    const AccountAttributes* mAccountAttributes;
};

class SetAccountAttributesResponse : public Response
{
public:
    bool isSuccess();
    void parseResponse();
};

class CreateQueueRequest : public Request
{
public:
    CreateQueueRequest(const std::string& queueName);
    CreateQueueRequest(const std::string& queueName,
        const QueueAttributes& attributes);

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();
protected:
    const std::string* mQueueName;
    const QueueAttributes* mAttributes;
};

class CreateQueueResponse : public Response
{
public:
    void parseResponse();
    bool isSuccess();
};

class DeleteQueueRequest : public Request
{
public:
    DeleteQueueRequest(const std::string& queueName);
    virtual ~DeleteQueueRequest() {}

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();
protected:
    const std::string* mQueueName;
};

class DeleteQueueResponse : public Response
{
public:
    void parseResponse();
    bool isSuccess();
};

class ListQueueRequest : public Request
{
public:
    ListQueueRequest(const std::string& prefix = "",
                     const std::string& marker = "",
                     const int32_t retNum = -1);
    virtual ~ListQueueRequest() {}

    void setMarker(const std::string& marker);
    void setPrefix(const std::string& prefix);
    void setRetNum(const int32_t retNum);

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();

protected:
    std::string mPrefix;
    std::string mMarker;
    int32_t mRetNum;
};

class ListQueueResponse : public Response
{
public:
    ListQueueResponse(std::vector<std::string>& queueNames,
        std::string& nextMarker);
    virtual ~ListQueueResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    std::vector<std::string>* mQueueNames;
    std::string* mNextMarker;
};

class GetQueueAttributesRequest : public Request
{
public:
    GetQueueAttributesRequest(const std::string& queueName);
    virtual ~GetQueueAttributesRequest() {}

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();

protected:
    const std::string* mQueueName;
};

class GetQueueAttributesResponse : public Response
{
public:
    GetQueueAttributesResponse(QueueAttributes& attributes);
    virtual ~GetQueueAttributesResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    QueueAttributes* mQueueAttributes;
};

class SetQueueAttributesRequest : public Request
{
public:
    SetQueueAttributesRequest(const std::string& queueName,
        const QueueAttributes& queueAttributes);
    virtual ~SetQueueAttributesRequest() {}

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();

protected:
    const std::string* mQueueName;
    const QueueAttributes* mQueueAttributes;
};

class SetQueueAttributesResponse : public Response
{
public:
    bool isSuccess();
    void parseResponse();
};

class SendMessageRequest : public MessageRequest
{
public:
    SendMessageRequest(const std::string& messageBody,
        const int32_t delaySeconds = -1, const int32_t priority = -1);
    virtual ~SendMessageRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    const std::string* mMessageBody;
    int32_t mDelaySeconds;
    int32_t mPriority;
};

class SendMessageResponse : public Response
{
public:
    SendMessageResponse();
    virtual ~SendMessageResponse() {}

    void parseResponse();
    bool isSuccess();

    std::string getMessageId()
    {
        return mMessageId;
    }

    std::string getMessageBodyMD5()
    {
        return mMessageBodyMD5;
    }
    
protected:
    std::string mMessageId;
    std::string mMessageBodyMD5;
};

class BatchSendMessageRequest : public MessageRequest
{
public:
    BatchSendMessageRequest(
        const std::vector<SendMessageItem>& sendMessageItems);
    virtual ~BatchSendMessageRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    const std::vector<SendMessageItem>* mSendMessageItems;
};

class BatchSendMessageResponse : public Response
{
public:
    void parseResponse();
    bool isSuccess();

    const std::vector<SendMessageSucceedItem>& getSendMessageSucceedItems()
    {
        return mSendMessageSucceedItems;
    }

    const std::vector<SendMessageFailedItem>& getSendMessageFailedItems()
    {
        return mSendMessageFailedItems;
    }

protected:
    void parseBatchSendResponse(pugi::xml_node& rootNode);

protected:
    std::vector<SendMessageSucceedItem> mSendMessageSucceedItems;
    std::vector<SendMessageFailedItem> mSendMessageFailedItems;
};

class ReceiveMessageRequest : public MessageRequest
{
public:
    ReceiveMessageRequest(const int32_t waitSeconds = -1);
    virtual ~ReceiveMessageRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    int32_t mWaitSeconds;
};

class ReceiveMessageResponse : public Response
{
public:
    ReceiveMessageResponse(Message& message);
    virtual ~ReceiveMessageResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    Message* mMessage;
};

class BatchReceiveMessageRequest : public MessageRequest
{
public:
    BatchReceiveMessageRequest(const int32_t numOfMessages,
        const int32_t waitSeconds = -1);
    virtual ~BatchReceiveMessageRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    int32_t mNumOfMessages;
    int32_t mWaitSeconds;
};

class BatchReceiveMessageResponse : public Response
{
public:
    BatchReceiveMessageResponse(std::vector<Message>& messages);
    virtual ~BatchReceiveMessageResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    std::vector<Message>* mMessages;
};

class DeleteMessageRequest : public MessageRequest
{
public:
    DeleteMessageRequest(const std::string& receiptHandle);
    virtual ~DeleteMessageRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    const std::string* mReceiptHandle;
};

class DeleteMessageResponse : public Response
{
public:
    void parseResponse();
    bool isSuccess();
};

class BatchDeleteMessageRequest : public MessageRequest
{
public:
    BatchDeleteMessageRequest(const std::vector<std::string>& receiptHandles);
    virtual ~BatchDeleteMessageRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    const std::vector<std::string>* mReceiptHandles;
};

class BatchDeleteMessageResponse : public Response
{
public:
    void parseResponse();
    bool isSuccess();

    const std::vector<DeleteMessageFailedItem>& getDeleteMessageFailedItem()
    {
        return mDeleteMessageFailedItems;
    }

protected:
    std::vector<DeleteMessageFailedItem> mDeleteMessageFailedItems;
};

class PeekMessageRequest : public MessageRequest
{
public:
    PeekMessageRequest();
    virtual ~PeekMessageRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();
};

class PeekMessageResponse : public Response
{
public:
    PeekMessageResponse(Message& message);
    virtual ~PeekMessageResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    Message* mMessage;
};

class BatchPeekMessageRequest : public MessageRequest
{
public:
    BatchPeekMessageRequest(const int32_t numOfMessages);
    virtual ~BatchPeekMessageRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    int32_t mNumOfMessages;
};

class BatchPeekMessageResponse : public Response
{
public:
    BatchPeekMessageResponse(std::vector<Message>& mMessages);
    virtual ~BatchPeekMessageResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    std::vector<Message>* mMessages;
};

class ChangeMessageVisibilityRequest : public MessageRequest
{
public:
    ChangeMessageVisibilityRequest(const std::string& receiptHandle,
        const int32_t visibilityTimeout);
    virtual ~ChangeMessageVisibilityRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    const std::string* mReceiptHandle;
    int32_t mVisibilityTimeout;
};

class ChangeMessageVisibilityResponse : public Response
{
public:
    void parseResponse();
    bool isSuccess();

    const std::string& getReceiptHandle()
    {
        return mReceiptHandle;
    }

    int64_t getNextVisibleTime()
    {
        return mNextVisibleTime;
    }

protected:
    std::string mReceiptHandle;
    int64_t mNextVisibleTime;
};

class CreateTopicRequest : public Request
{
public:
    CreateTopicRequest(const std::string& topicName);
    CreateTopicRequest(const std::string& topicName,
        const TopicAttributes& attributes);

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();
protected:
    const std::string* mTopicName;
    const TopicAttributes* mAttributes;
};

class CreateTopicResponse : public Response
{
public:
    CreateTopicResponse();
    virtual ~CreateTopicResponse() {};

    std::string getTopicURL() const
    {
        return mTopicURL;
    }

    void setTopicURL(const std::string& topicURL)
    {
        mTopicURL = topicURL;
    }

    void parseResponse();
    bool isSuccess();

protected:
    std::string mTopicURL;
};

class DeleteTopicRequest : public Request
{
public:
    DeleteTopicRequest(const std::string& queueName);
    virtual ~DeleteTopicRequest() {}

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();
protected:
    const std::string* mTopicName;
};

class DeleteTopicResponse : public Response
{
public:
    void parseResponse();
    bool isSuccess();
};

class ListTopicRequest : public Request
{
public:
    ListTopicRequest(const std::string& prefix = "",
                     const std::string& marker = "",
                     const int32_t retNum = -1);
    virtual ~ListTopicRequest() {}

    void setMarker(const std::string& marker);
    void setPrefix(const std::string& prefix);
    void setRetNum(const int32_t retNum);

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();

protected:
    std::string mPrefix;
    std::string mMarker;
    int32_t mRetNum;
};

class ListTopicResponse : public Response
{
public:
    ListTopicResponse(std::vector<std::string>& queueNames,
        std::string& nextMarker);
    virtual ~ListTopicResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    std::vector<std::string>* mTopicNames;
    std::string* mNextMarker;
};

class GetTopicAttributesRequest : public Request
{
public:
    GetTopicAttributesRequest(const std::string& topicName);
    virtual ~GetTopicAttributesRequest() {}

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();

protected:
    const std::string* mTopicName;
};

class GetTopicAttributesResponse : public Response
{
public:
    GetTopicAttributesResponse(TopicAttributes& attributes);
    virtual ~GetTopicAttributesResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    TopicAttributes* mAttributes;
};

class SetTopicAttributesRequest : public Request
{
public:
    SetTopicAttributesRequest(const std::string& topicName,
        const TopicAttributes& attributes);
    virtual ~SetTopicAttributesRequest() {}

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();

protected:
    const std::string* mTopicName;
    const TopicAttributes* mAttributes;
};

class SetTopicAttributesResponse : public Response
{
public:
    bool isSuccess();
    void parseResponse();
};

class PublishMessageRequest : public Request
{
public:
    PublishMessageRequest(const std::string& topicName,
                          const std::string& messageBody);
    PublishMessageRequest(const std::string& topicName,
                          const std::string& messageBody,
                          const MessageAttributes& messageAttributes);
    PublishMessageRequest(const std::string& topicName,
                          const std::string& messageBody,
                          const std::string& messageTag);
    PublishMessageRequest(const std::string& topicName,
                          const std::string& messageBody,
                          const std::string& messageTag,
                          const MessageAttributes& messageAttributes);
    virtual ~PublishMessageRequest() {}

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();

protected:
    const std::string* mTopicName;
    const std::string* mMessageBody;
    const std::string* mMessageTag;
    const MessageAttributes* mpMessageAttributes;
};

class PublishMessageResponse : public Response
{
public:
    PublishMessageResponse();
    virtual ~PublishMessageResponse() {}

    void parseResponse();
    bool isSuccess();

    std::string getMessageId()
    {
        return mMessageId;
    }

    std::string getMessageBodyMD5()
    {
        return mMessageBodyMD5;
    }

protected:
    std::string mMessageId;
    std::string mMessageBodyMD5;
};

class SubscribeRequest : public SubscriptionRequest
{
public:
    SubscribeRequest(const std::string& topicName,
                     const std::string& subscriptionName,
                     const std::string& endPoint);

    SubscribeRequest(const std::string& topicName,
                     const std::string& subscriptionName,
                     const std::string& endPoint,
                     const SubscriptionAttributes& attributes);

    SubscribeRequest(const std::string& topicName,
                     const std::string& subscriptionName,
                     const std::string& filterTag,
                     const std::string& endPoint);

    SubscribeRequest(const std::string& topicName,
                     const std::string& subscriptionName,
                     const std::string& filterTag,
                     const std::string& endPoint,
                     const SubscriptionAttributes& attributes);
    virtual ~SubscribeRequest();

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    const std::string* mEndPoint;
    const std::string* mFilterTag;
    const SubscriptionAttributes* mAttributes;
};

class SubscribeResponse : public Response
{
public:
    bool isSuccess();
    void parseResponse();
};

class SetSubscriptionAttributesRequest : public SubscriptionRequest
{
public:
    SetSubscriptionAttributesRequest(const std::string& topicName,
                                     const std::string& subscriptionName,
                                     const SubscriptionAttributes& attributes);
    virtual ~SetSubscriptionAttributesRequest();

    std::string getQueryString();
    const std::string& generateRequestBody();

protected:
    const SubscriptionAttributes* mAttributes;
};

class SetSubscriptionAttributesResponse : public Response
{
public:
    bool isSuccess();
    void parseResponse();
};

class GetSubscriptionAttributesRequest : public SubscriptionRequest
{
public:
    GetSubscriptionAttributesRequest(const std::string& topicName,
                                     const std::string& subscriptionName);
    virtual ~GetSubscriptionAttributesRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();
};

class GetSubscriptionAttributesResponse : public Response
{
public:
    GetSubscriptionAttributesResponse(SubscriptionAttributes& attributes);
    virtual ~GetSubscriptionAttributesResponse() {}

    bool isSuccess();
    void parseResponse();

protected:
    SubscriptionAttributes* mAttributes;
};

class UnsubscribeRequest : public SubscriptionRequest
{
public:
    UnsubscribeRequest(const std::string& topicName,
                       const std::string& subscriptionName);
    virtual ~UnsubscribeRequest() {}

    std::string getQueryString();
    const std::string& generateRequestBody();
};

class UnsubscribeResponse : public Response
{
public:
    bool isSuccess();
    void parseResponse();
};

class ListSubscriptionRequest : public Request
{
public:
    ListSubscriptionRequest(const std::string& prefix = "",
                            const std::string& marker = "",
                            const int32_t retNum = -1);
    virtual ~ListSubscriptionRequest() {}

    void setMarker(const std::string& marker);
    void setPrefix(const std::string& prefix);
    void setRetNum(const int32_t retNum);

    // this is automatically set
    void setTopicName(const std::string& topicName);

    std::string getQueryString();
    std::string getResourcePath();
    const std::string& generateRequestBody();

protected:
    std::string mPrefix;
    std::string mMarker;
    int32_t mRetNum;
    const std::string* mTopicName;
};

class ListSubscriptionResponse : public Response
{
public:
    ListSubscriptionResponse(std::vector<std::string>& subscriptionNames,
                             std::string& nextMarker);
    virtual ~ListSubscriptionResponse() {}

    void parseResponse();
    bool isSuccess();

protected:
    std::vector<std::string>* mSubscriptionNames;
    std::string* mNextMarker;
};

}
}

#endif
