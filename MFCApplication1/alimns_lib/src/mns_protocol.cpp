#include "mns_protocol.h"
#include "mns_utils.h"
#include "mns_exception.h"
#include "mns_network_tool.h"
#include "mns_common_tool.h"
#include "mns_exception.h"
#include "constants.h"
#include "mns_utils.h"

#include <iostream>

using namespace std;
using namespace pugi;
using namespace mns::sdk;


MailAttributes::MailAttributes()
    : mSubject("")
    , mAccountName("")
    , mAddressType(0)
    , mReplyToAddress(false)
    , mIsHtml(false)
{
}

std::string MailAttributes::toJson()
{
    return "{\"Subject\":\"" + MNSUtils::escapeJson(mSubject) 
        + "\", \"AccountName\":\"" + MNSUtils::escapeJson(mAccountName)
        + "\", \"AddressType\": \"" + StringTool::ToString(mAddressType) 
        + "\", \"ReplyToAddress\":\"" + (mReplyToAddress ? "1" : "0")
        + "\", \"IsHtml\": \"" + (mIsHtml ? "1" : "0") + "\"}";
}

const std::string& MailAttributes::getSubject()
{
    return mSubject;
}

void MailAttributes::setSubject(const std::string& subject)
{
    mSubject = subject;
}

const std::string& MailAttributes::getAccountName()
{
    return mAccountName;
}

void MailAttributes::setAccountName(const std::string& accountName)
{
    mAccountName = accountName;
}

int MailAttributes::getAddressType()
{
    return mAddressType;
}

void MailAttributes::setAddressType(int addressType)
{
    mAddressType = addressType;
}

bool MailAttributes::getReplyToAddress()
{
    return mReplyToAddress;
}

void MailAttributes::setReplyToAddress(bool replyToAddress)
{
    mReplyToAddress = replyToAddress;
}

bool MailAttributes::getIsHtml()
{
    return mIsHtml;
}

void MailAttributes::setIsHtml(bool isHtml)
{
    mIsHtml = isHtml;
}

SMSAttributes::SMSAttributes(const SMSType type)
{
    mType = type;
}

SMSType SMSAttributes::getType()
{
    return mType;
}

std::string SMSAttributes::toJson()
{
    string ret = "";
    ret.reserve(200);
    switch (mType)
    {
        case SINGLE_SMS:
            ret = "{\"FreeSignName\":\"" 
                + MNSUtils::escapeJson(mSignName) + "\",\"TemplateCode\":\""
                + MNSUtils::escapeJson(mTemplateCode) + "\",\"SmsParams\":\""
                + MNSUtils::escapeJson(MNSUtils::toJsonStr(mSingleParams)) + "\"";
            if (mReceiver != "")
            {
                ret += ",\"Receiver\":\"" + MNSUtils::escapeJson(mReceiver) + "\"";
            }
            ret += "}";
            return ret;
        case BATCH_SMS:
            ret = "{\"Type\":\"multiContent\",\"FreeSignName\":\"" 
                + MNSUtils::escapeJson(mSignName) + "\",\"TemplateCode\":\""
                + MNSUtils::escapeJson(mTemplateCode) + "\",\"SmsParams\":\"";
            string smsParamsStr = "{";
			std::map<std::string, std::map<std::string, std::string> >::reverse_iterator lastIter = mBatchParams.rbegin();
            for (std::map<std::string, std::map<std::string, std::string> >::iterator iter = mBatchParams.begin();
                    iter != mBatchParams.end(); iter++)
            {
                smsParamsStr += "\"" + MNSUtils::escapeJson(iter->first) + "\":" 
                                + MNSUtils::toJsonStr(iter->second);
                if (iter->first != lastIter->first)
                {
                    smsParamsStr += ",";
                }
            }
            smsParamsStr += "}";
            ret += MNSUtils::escapeJson(smsParamsStr) + "\"}";
            return ret;
    }
}

std::string SMSAttributes::getSignName()
{
    return mSignName;
}

void SMSAttributes::setSignName(const std::string& signName)
{
    mSignName = signName;
}

std::string SMSAttributes::getTemplateCode()
{
    return mTemplateCode;
}

void SMSAttributes::setTemplateCode(const std::string& templateCode)
{
    mTemplateCode = templateCode;
}

std::string SMSAttributes::getReceiver()
{
    return mReceiver;
}

void SMSAttributes::setReceiver(const std::string& receiver)
{
    mReceiver = receiver;
}

std::string SMSAttributes::getSingleParam(const std::string& key)
{
	std::map<std::string, std::string>::iterator iter = mSingleParams.find(key);
    if (iter != mSingleParams.end())
    {
        return iter->second;
    }
    return "";
}

void SMSAttributes::setSingleParam(const std::string& key, const std::string& value)
{
    mSingleParams[key] = value;
}

std::map<std::string, std::string> SMSAttributes::getBatchParam(const std::string& phone)
{
	std::map<std::string, std::map<std::string, std::string> >::iterator iter = mBatchParams.find(phone);
    if (iter != mBatchParams.end())
    {
        return iter->second;
    }
    std::map<std::string, std::string> ret;
    return ret;
}

std::string SMSAttributes::getBatchParam(const std::string& phone, const std::string& key)
{
	std::map<std::string, std::map<std::string, std::string> >::iterator iter = mBatchParams.find(phone);
    if (iter != mBatchParams.end())
    {
		std::map<std::string, std::string>::iterator iter2 = iter->second.find(key);
        if (iter2 != iter->second.end())
        {
            return iter2->second;
        }
    }
    return "";
}

void SMSAttributes::setBatchParam(const std::string& phone, const std::string& key, const std::string& value)
{
	std::map<std::string, std::map<std::string, std::string> >::iterator iter = mBatchParams.find(phone);
    if (iter != mBatchParams.end())
    {
        (iter->second)[key] = value;
    }
    else
    {
        std::map<std::string, std::string> tmp;
        tmp[key] = value;
        mBatchParams[phone] = tmp;
    }
}

void SMSAttributes::setBatchParam(const std::string& phone, const std::map<std::string, std::string>& params)
{
    mBatchParams[phone] = params;
}

QueueAttributes::QueueAttributes()
{
    clear();
}

void QueueAttributes::clear()
{
    mQueueName = "";
    mVisibilityTimeout = -1;
    mDelaySeconds = -1;
    mMaximumMessageSize = -1;
    mMessageRetentionPeriod = -1;
    mActiveMessages = -1;
    mInactiveMessages = -1;
    mDelayMessages = -1;
    mCreateTime = -1;
    mLastModifyTime = -1;
    mPollingWaitSeconds = -1;
    mLoggingEnabledSet = false;
    mLoggingEnabled = false;
}

void QueueAttributes::setLoggingEnabled(bool loggingEnabled)
{
    mLoggingEnabled = loggingEnabled;
    mLoggingEnabledSet = true;
}

bool QueueAttributes::isLoggingEnabled() const
{
    return mLoggingEnabled;
}

bool QueueAttributes::isLoggingEnabledSet() const
{
    return mLoggingEnabledSet;
}

int64_t QueueAttributes::getVisibilityTimeout() const
{
    return mVisibilityTimeout;
}

void QueueAttributes::setVisibilityTimeout(int64_t visibilityTimeout)
{
    mVisibilityTimeout = visibilityTimeout;
}

int64_t QueueAttributes::getDelaySeconds() const
{
    return mDelaySeconds;
}

void QueueAttributes::setDelaySeconds(int64_t delaySeconds)
{
    mDelaySeconds = delaySeconds;
}

int64_t QueueAttributes::getMaximumMessageSize() const
{
    return mMaximumMessageSize;
}

void QueueAttributes::setMaximumMessageSize(int64_t maximumMessageSize)
{
    mMaximumMessageSize = maximumMessageSize;
}

int64_t QueueAttributes::getMessageRetentionPeriod() const
{
    return mMessageRetentionPeriod;
}

void QueueAttributes::setMessageRetentionPeriod(int64_t messageRetentionPeriod)
{
    mMessageRetentionPeriod = messageRetentionPeriod;
}

int32_t QueueAttributes::getPollingWaitSeconds() const
{
    return mPollingWaitSeconds;
}

void QueueAttributes::setPollingWaitSeconds(int32_t pollingWaitSeconds)
{
    mPollingWaitSeconds = pollingWaitSeconds;
}

std::string QueueAttributes::getQueueName() const
{
    return mQueueName;
}

int64_t QueueAttributes::getActiveMessages() const
{
    return mActiveMessages;
}

int64_t QueueAttributes::getInactiveMessages() const
{
    return mInactiveMessages;
}

int64_t QueueAttributes::getDelayMessages() const
{
    return mDelayMessages;
}

int64_t QueueAttributes::getCreateTime() const
{
    return mCreateTime;
}

int64_t QueueAttributes::getLastModifyTime() const
{
    return mLastModifyTime;
}

TopicAttributes::TopicAttributes()
{
    clear();
}

void TopicAttributes::clear()
{
    mTopicName = "";
    mMaximumMessageSize = -1;
    mMessageRetentionPeriod = -1;
    mMessages = -1;
    mCreateTime = -1;
    mLastModifyTime = -1;
    mLoggingEnabledSet = false;
    mLoggingEnabled = false;
}

void TopicAttributes::setLoggingEnabled(bool loggingEnabled)
{
    mLoggingEnabled = loggingEnabled;
    mLoggingEnabledSet = true;
}

bool TopicAttributes::isLoggingEnabled() const
{
    return mLoggingEnabled;
}

bool TopicAttributes::isLoggingEnabledSet() const
{
    return mLoggingEnabledSet;
}

std::string TopicAttributes::getTopicName()
{
    return mTopicName;
}

int64_t TopicAttributes::getCreateTime() const
{
    return mCreateTime;
}

int64_t TopicAttributes::getLastModifyTime() const
{
    return mLastModifyTime;
}

int64_t TopicAttributes::getMaximumMessageSize() const
{
    return mMaximumMessageSize;
}

void TopicAttributes::setMaximumMessageSize(int64_t maximumMessageSize)
{
    mMaximumMessageSize = maximumMessageSize;
}

int64_t TopicAttributes::getMessageRetentionPeriod() const
{
    return mMessageRetentionPeriod;
}

int64_t TopicAttributes::getMessages() const
{
    return mMessages;
}

SubscriptionAttributes::SubscriptionAttributes()
    : mSubscriptionName("")
    , mTopicOwner("")
    , mTopicName("")
    , mEndPoint("")
    , mNotifyStrategy(UNKNOWN_STRATEGY)
    , mContentFormat(UNKNOWN_FORMAT)
    , mCreateTime(-1)
    , mLastModifyTime(-1)
{
}

void SubscriptionAttributes::setNotifyStrategy(NotifyStrategy notifyStrategy)
{
    mNotifyStrategy = notifyStrategy;
}

void SubscriptionAttributes::setContentFormat(ContentFormat contentFormat)
{
    mContentFormat = contentFormat;
}

std::string SubscriptionAttributes::getSubscriptionName() const
{
    return mSubscriptionName;
}

std::string SubscriptionAttributes::getTopicOwner() const
{
    return mTopicOwner;
}

std::string SubscriptionAttributes::getTopicName() const
{
    return mTopicName;
}

std::string SubscriptionAttributes::getEndPoint() const
{
    return mEndPoint;
}

std::string SubscriptionAttributes::getFilterTag() const
{
    return mFilterTag;
}

NotifyStrategy SubscriptionAttributes::getNotifyStrategy() const
{
    return mNotifyStrategy;
}

ContentFormat SubscriptionAttributes::getContentFormat() const
{
    return mContentFormat;
}

int64_t SubscriptionAttributes::getCreateTime() const
{
    return mCreateTime;
}

int64_t SubscriptionAttributes::getLastModifyTime() const
{
    return mLastModifyTime;
}

void Message::initFromXml(const xml_node& messageNode)
{
    xml_node iterNode = messageNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(MESSAGE_ID, name))
        {
            mMessageId = iterNode.text().get();
        }
        else if (0 == strcmp(RECEIPT_HANDLE, name))
        {
            mReceiptHandle = iterNode.text().get();
        }
        else if (0 == strcmp(MESSAGE_BODY, name))
        {
            mMessageBody = iterNode.text().get();
        }
        else if (0 == strcmp(MESSAGE_BODY_MD5, name))
        {
            mMessageBodyMD5 = iterNode.text().get();
        }
        else if (0 == strcmp(ENQUEUE_TIME, name))
        {
            mEnqueueTime = atol(iterNode.text().get());
        }
        else if (0 == strcmp(FIRST_DEQUEUE_TIME, name))
        {
            mFirstDequeueTime = atol(iterNode.text().get());
        }
        else if (0 == strcmp(NEXT_VISIBLE_TIME, name))
        {
            mNextVisibleTime = atol(iterNode.text().get());
        }
        else if (0 == strcmp(DEQUEUE_COUNT, name))
        {
            mDequeueCount = atoi(iterNode.text().get());
        }
        else if (0 == strcmp(PRIORITY, name))
        {
            mPriority = atoi(iterNode.text().get());
        }

        iterNode = iterNode.next_sibling();
    }
}

Response::Response()
    : mRawData(""), mStatus(0)
{
}

bool Response::isCommonError(const pugi::xml_node& rootNode)
{
    const pugi::char_t* name = rootNode.name();
    return 0 == strcmp(ERROR_TAG, name);
}

pugi::xml_node Response::toXML()
{
    mDoc.reset();
    pugi::xml_parse_result result = mDoc.load_string(mRawData.c_str());
    if (!result)
    {
        MNS_THROW(MNSExceptionBase, mRawData);
    }
    xml_node iterNode = mDoc.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() == node_element) {
            return iterNode;
        }

        iterNode = iterNode.next_sibling();
    }
    MNS_THROW(MNSExceptionBase, mRawData);
}

void Response::parseCommonError(const pugi::xml_node& rootNode)
{
    if (!isCommonError(rootNode))
    {
        MNS_THROW(MNSExceptionBase, "Unknown CommonError XML: " + mRawData);
    }

    ErrorInfo errorInfo;
    errorInfo.httpStatus = mStatus;

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(CODE, name))
            errorInfo.code = iterNode.text().get();
        else if (0 == strcmp(MESSAGE, name))
            errorInfo.errorMessage = iterNode.text().get();
        else if (0 == strcmp(REQUEST_ID, name))
            errorInfo.requestId = iterNode.text().get();
        else if (0 == strcmp(HOST_ID, name))
            errorInfo.hostId = iterNode.text().get();

        iterNode = iterNode.next_sibling();
    }
    MNS_THROW(MNSServerException, errorInfo);
}

GetAccountAttributesRequest::GetAccountAttributesRequest()
    : Request("GET")
{
}

string GetAccountAttributesRequest::getQueryString()
{
    return "accountmeta=true";
}

string GetAccountAttributesRequest::getResourcePath()
{
    return "/";
}

const string& GetAccountAttributesRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

GetAccountAttributesResponse::GetAccountAttributesResponse(
    AccountAttributes& attributes)
    : mAccountAttributes(&attributes), Response()
{
}

bool GetAccountAttributesResponse::isSuccess()
{
    return mStatus == 200;
}

void GetAccountAttributesResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(LOGGING_BUCKET, name))
        {
            mAccountAttributes->mLoggingBucket = iterNode.text().get();
            mAccountAttributes->mLoggingBucketSet = true;
        }
        iterNode = iterNode.next_sibling();
    }
}

SetAccountAttributesRequest::SetAccountAttributesRequest(
    const AccountAttributes& accountAttributes)
    : Request("PUT")
    , mAccountAttributes(&accountAttributes)
{
}

string SetAccountAttributesRequest::getQueryString()
{
    return "accountmeta=true";
}

string SetAccountAttributesRequest::getResourcePath()
{
    return "/";
}

const string& SetAccountAttributesRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child("Queue");
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    if (mAccountAttributes->isLoggingBucketSet())
        node.append_child("LoggingBucket").append_child(
            pugi::node_pcdata).set_value((mAccountAttributes->getLoggingBucket()).c_str());

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

bool SetAccountAttributesResponse::isSuccess()
{
    return mStatus == 204;
}

void SetAccountAttributesResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

CreateQueueRequest::CreateQueueRequest(const string& queueName)
    : mQueueName(&queueName), Request("PUT"), mAttributes(NULL)
{
}

CreateQueueRequest::CreateQueueRequest(const string& queueName,
                                       const QueueAttributes& attributes)
    : mQueueName(&queueName), mAttributes(&attributes), Request("PUT")
{
}

std::string CreateQueueRequest::getQueryString()
{
    return "";
}

std::string CreateQueueRequest::getResourcePath()
{
    return "/queues/" + *mQueueName;
}

const string& CreateQueueRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child("Queue");
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    if (mAttributes != NULL)
    {
        string visibilityTimeoutStr = StringTool::ToString(
            mAttributes->getVisibilityTimeout());
        string maximumMessageSizeStr = StringTool::ToString(
            mAttributes->getMaximumMessageSize());
        string messageRetentionPeriodStr = StringTool::ToString(
            mAttributes->getMessageRetentionPeriod());
        string delaySecondsStr = StringTool::ToString(
            mAttributes->getDelaySeconds());
        string pollingWaitSecondsStr = StringTool::ToString(
            mAttributes->getPollingWaitSeconds());

        if (mAttributes->getVisibilityTimeout() > 0)
            node.append_child("VisibilityTimeout").append_child(
                pugi::node_pcdata).set_value(visibilityTimeoutStr.c_str());

        if (mAttributes->getMaximumMessageSize() > 0)
            node.append_child("MaximumMessageSize").append_child(
                pugi::node_pcdata).set_value(maximumMessageSizeStr.c_str());

        if (mAttributes->getMessageRetentionPeriod() > 0)
            node.append_child("MessageRetentionPeriod").append_child(
                pugi::node_pcdata).set_value(messageRetentionPeriodStr.c_str());

        if (mAttributes->getDelaySeconds() > 0)
            node.append_child("DelaySeconds").append_child(
                pugi::node_pcdata).set_value(delaySecondsStr.c_str());

        if (mAttributes->getPollingWaitSeconds() > 0)
            node.append_child("PollingWaitSeconds").append_child(
                pugi::node_pcdata).set_value(pollingWaitSecondsStr.c_str());

        if (mAttributes->isLoggingEnabledSet())
        {
            string loggingEnabledStr = mAttributes->isLoggingEnabled() ? "True" : "False";
            node.append_child("LoggingEnabled").append_child(
                pugi::node_pcdata).set_value(loggingEnabledStr.c_str());
        }
    }

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

bool CreateQueueResponse::isSuccess()
{
    return mStatus == 201 || mStatus == 204;
}

void CreateQueueResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

DeleteQueueRequest::DeleteQueueRequest(const string& queueName)
    :Request("DELETE"), mQueueName(&queueName)
{
}

string DeleteQueueRequest::getQueryString()
{
    return "";
}

string DeleteQueueRequest::getResourcePath()
{
    return "/queues/" + *mQueueName;
}

const string& DeleteQueueRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

bool DeleteQueueResponse::isSuccess()
{
    return mStatus == 204;
}

void DeleteQueueResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

ListQueueRequest::ListQueueRequest(const string& prefix,
                                   const string& marker,
                                   const int32_t retNum)
    : Request("GET")
{
    setMarker(marker);
    setPrefix(prefix);
    setRetNum(retNum);
}

void ListQueueRequest::setMarker(const string& marker)
{
    mMarker = marker;
    if (mMarker != "")
    {
        mHeaders["x-mns-marker"] = mMarker;
    }
    else
    {
        mHeaders.erase("x-mns-marker");
    }
}

void ListQueueRequest::setPrefix(const string& prefix)
{
    mPrefix = prefix;
    if (mPrefix != "")
    {
        mHeaders["x-mns-prefix"] = mPrefix;
    }
    else
    {
        mHeaders.erase("x-mns-prefix");
    }
}

void ListQueueRequest::setRetNum(const int32_t retNum)
{
    mRetNum = retNum;
    if (mRetNum > 0)
    {
        mHeaders["x-mns-ret-number"] = StringTool::ToString(mRetNum);
    }
    else
    {
        mHeaders.erase("x-mns-ret-number");
    }
}

string ListQueueRequest::getQueryString()
{
    return "";
}

string ListQueueRequest::getResourcePath()
{
    return "/queues";
}

const string& ListQueueRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

ListQueueResponse::ListQueueResponse(std::vector<std::string>& queueNames,
                                     std::string& nextMarker)
    : mQueueNames(&queueNames), mNextMarker(&nextMarker), Response()
{
}

bool ListQueueResponse::isSuccess()
{
    return mStatus == 200;
}

void ListQueueResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(QUEUE, name))
        {
            xml_node queueURLNode = iterNode.child(QUEUE_URL);
            string queueName = MNSUtils::getLastNodeFromURL(
                    queueURLNode.text().get());
            mQueueNames->push_back(queueName);
        }
        else if (0 == strcmp(NEXT_MARKER, name))
        {
            *mNextMarker = iterNode.text().get();
        }

        iterNode = iterNode.next_sibling();
    }
}

GetQueueAttributesRequest::GetQueueAttributesRequest(const string& queueName)
    :Request("GET"), mQueueName(&queueName)
{
}

string GetQueueAttributesRequest::getQueryString()
{
    return "";
}

string GetQueueAttributesRequest::getResourcePath()
{
    return "/queues/" + *mQueueName;
}

const string& GetQueueAttributesRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

GetQueueAttributesResponse::GetQueueAttributesResponse(
    QueueAttributes& attributes)
    : mQueueAttributes(&attributes), Response()
{
}

bool GetQueueAttributesResponse::isSuccess()
{
    return mStatus == 200;
}

void GetQueueAttributesResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    mQueueAttributes->clear();

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(QUEUE_NAME, name))
        {
            mQueueAttributes->mQueueName = iterNode.text().get();
        }
        else if (0 == strcmp(CREATE_TIME, name))
        {
            mQueueAttributes->mCreateTime = atol(iterNode.text().get());
        }
        else if (0 == strcmp(LAST_MODIFY_TIME, name))
        {
            mQueueAttributes->mLastModifyTime = atol(iterNode.text().get());
        }
        else if (0 == strcmp(VISIBILITY_TIMEOUT, name))
        {
            mQueueAttributes->mVisibilityTimeout = atol(iterNode.text().get());
        }
        else if (0 == strcmp(DELAY_SECONDS, name))
        {
            mQueueAttributes->mDelaySeconds = atol(iterNode.text().get());
        }
        else if (0 == strcmp(MAXIMUM_MESSAGE_SIZE, name))
        {
            mQueueAttributes->mMaximumMessageSize = atol(iterNode.text().get());
        }
        else if (0 == strcmp(MAXIMUM_MESSAGE_SIZE, name))
        {
            mQueueAttributes->mMessageRetentionPeriod = atol(iterNode.text().get());
        }
        else if (0 == strcmp(ACTIVE_MESSAGES, name))
        {
            mQueueAttributes->mActiveMessages = atol(iterNode.text().get());
        }
        else if (0 == strcmp(INACTIVE_MESSAGES, name))
        {
            mQueueAttributes->mInactiveMessages = atol(iterNode.text().get());
        }
        else if (0 == strcmp(DELAY_MESSAGES, name))
        {
            mQueueAttributes->mDelayMessages = atol(iterNode.text().get());
        }
        else if (0 == strcmp(POLLING_WAIT_SECONDS, name))
        {
            mQueueAttributes->mPollingWaitSeconds = atoi(iterNode.text().get());
        }
        else if (0 == strcmp(LOGGING_ENABLED, name))
        {
            mQueueAttributes->mLoggingEnabled = (0 == strcmp(iterNode.text().get(), "True")) ? true : false;
            mQueueAttributes->mLoggingEnabledSet = true;
        }

        iterNode = iterNode.next_sibling();
    }
}

SetQueueAttributesRequest::SetQueueAttributesRequest(
    const string& queueName,
    const QueueAttributes& queueAttributes)
    : Request("PUT")
    , mQueueName(&queueName)
    , mQueueAttributes(&queueAttributes)
{
}

string SetQueueAttributesRequest::getQueryString()
{
    return "metaoverride=true";
}

string SetQueueAttributesRequest::getResourcePath()
{
    return "/queues/" + *mQueueName;
}

const string& SetQueueAttributesRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child("Queue");
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    string visibilityTimeoutStr = StringTool::ToString(
        mQueueAttributes->getVisibilityTimeout());
    string maximumMessageSizeStr = StringTool::ToString(
        mQueueAttributes->getMaximumMessageSize());
    string messageRetentionPeriodStr = StringTool::ToString(
        mQueueAttributes->getMessageRetentionPeriod());
    string delaySecondsStr = StringTool::ToString(
        mQueueAttributes->getDelaySeconds());
    string pollingWaitSecondsStr = StringTool::ToString(
        mQueueAttributes->getPollingWaitSeconds());

    if (mQueueAttributes->getVisibilityTimeout() > 0)
        node.append_child("VisibilityTimeout").append_child(
            pugi::node_pcdata).set_value(visibilityTimeoutStr.c_str());

    if (mQueueAttributes->getMaximumMessageSize() > 0)
        node.append_child("MaximumMessageSize").append_child(
            pugi::node_pcdata).set_value(maximumMessageSizeStr.c_str());

    if (mQueueAttributes->getMessageRetentionPeriod() > 0)
        node.append_child("MessageRetentionPeriod").append_child(
            pugi::node_pcdata).set_value(messageRetentionPeriodStr.c_str());

    if (mQueueAttributes->getDelaySeconds() > 0)
        node.append_child("DelaySeconds").append_child(
            pugi::node_pcdata).set_value(delaySecondsStr.c_str());

    if (mQueueAttributes->getPollingWaitSeconds() > 0)
        node.append_child("PollingWaitSeconds").append_child(
            pugi::node_pcdata).set_value(pollingWaitSecondsStr.c_str());

    if (mQueueAttributes->isLoggingEnabledSet())
    {
        string loggingEnabledStr = mQueueAttributes->isLoggingEnabled() ? "True" : "False";
        node.append_child("LoggingEnabled").append_child(
            pugi::node_pcdata).set_value(loggingEnabledStr.c_str());
    }

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

bool SetQueueAttributesResponse::isSuccess()
{
    return mStatus == 204;
}

void SetQueueAttributesResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

SendMessageRequest::SendMessageRequest(const std::string& messageBody,
                                       const int32_t delaySeconds,
                                       const int32_t priority)
    : MessageRequest("POST"), mMessageBody(&messageBody),
      mDelaySeconds(delaySeconds), mPriority(priority)
{
}

string SendMessageRequest::getQueryString()
{
    return "";
}

const string& SendMessageRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child("Message");
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    node.append_child("MessageBody").append_child(
        pugi::node_pcdata).set_value(mMessageBody->c_str());

    string delaySecondsStr = StringTool::ToString(mDelaySeconds);
    string priorityStr = StringTool::ToString(mPriority);

    if (mDelaySeconds > 0)
        node.append_child("DelaySeconds").append_child(
            pugi::node_pcdata).set_value(delaySecondsStr.c_str());

    if (mPriority > 0)
        node.append_child("Priority").append_child(
            pugi::node_pcdata).set_value(priorityStr.c_str());

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

SendMessageResponse::SendMessageResponse()
    : mMessageId(""), mMessageBodyMD5("")
{
}

bool SendMessageResponse::isSuccess()
{
    return mStatus == 201;
}

void SendMessageResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(MESSAGE_ID, name))
        {
            mMessageId = iterNode.text().get();
        }
        else if (0 == strcmp(MESSAGE_BODY_MD5, name))
        {
            mMessageBodyMD5 = iterNode.text().get();
        }

        iterNode = iterNode.next_sibling();
    }
}

BatchSendMessageRequest::BatchSendMessageRequest(
    const std::vector<SendMessageItem>& sendMessageItems)
    : MessageRequest("POST"), mSendMessageItems(&sendMessageItems)
{
}

std::string BatchSendMessageRequest::getQueryString()
{
    return "";
}

const std::string& BatchSendMessageRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child("Messages");
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    for (std::vector<SendMessageItem>::const_iterator iter = mSendMessageItems->begin();
        iter != mSendMessageItems->end(); ++iter)
    {
        pugi::xml_node messgeNode = node.append_child("Message");

        messgeNode.append_child("MessageBody").append_child(
            pugi::node_pcdata).set_value(iter->messageBody.c_str());

        string delaySecondsStr = StringTool::ToString(iter->delaySeconds);
        string priorityStr = StringTool::ToString(iter->priority);

        if (iter->delaySeconds > 0)
            messgeNode.append_child("DelaySeconds").append_child(
                pugi::node_pcdata).set_value(delaySecondsStr.c_str());

        if (iter->priority > 0)
            messgeNode.append_child("Priority").append_child(
                pugi::node_pcdata).set_value(priorityStr.c_str());
    }

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

bool BatchSendMessageResponse::isSuccess()
{
    return mStatus == 201;
}

void BatchSendMessageResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (isSuccess())
    {
        parseBatchSendResponse(rootNode);
        return;
    }

    if (isCommonError(rootNode))
    {
        parseCommonError(rootNode);
    }
    else
    {
        parseBatchSendResponse(rootNode);
    }
}

void BatchSendMessageResponse::parseBatchSendResponse(xml_node& rootNode)
{
    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(MESSAGE, name))
        {
            SendMessageSucceedItem succeedItem;
            SendMessageFailedItem failedItem;

            xml_node iterMessageNode = iterNode.first_child();
            while (!iterMessageNode.empty())
            {
                if (iterMessageNode.type() != node_element)
                {
                    continue;
                }
                const pugi::char_t* iterName = iterMessageNode.name();
                if (0 == strcmp(MESSAGE_ID, iterName))
                {
                    succeedItem.messageId = iterMessageNode.text().get();
                }
                else if (0 == strcmp(MESSAGE_BODY_MD5, iterName))
                {
                    succeedItem.messageBodyMD5 = iterMessageNode.text().get();
                }
                else if (0 == strcmp(ERROR_MESSAGE, iterName))
                {
                    failedItem.errorMessage = iterMessageNode.text().get();
                }
                else if (0 == strcmp(ERROR_CODE, iterName))
                {
                    failedItem.errorCode = iterMessageNode.text().get();
                }

                iterMessageNode = iterMessageNode.next_sibling();
            }

            if (succeedItem.messageId != "")
            {
                mSendMessageSucceedItems.push_back(succeedItem);
            }
            else
            {
                mSendMessageFailedItems.push_back(failedItem);
            }
        }

        iterNode = iterNode.next_sibling();
    }
}

ReceiveMessageRequest::ReceiveMessageRequest(const int32_t waitSeconds)
    : MessageRequest("GET"), mWaitSeconds(waitSeconds)
{
}

std::string ReceiveMessageRequest::getQueryString()
{
    if (mWaitSeconds > 0)
    {
        return "waitseconds=" + StringTool::ToString(mWaitSeconds);
    }
    return "";
}

const std::string& ReceiveMessageRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

ReceiveMessageResponse::ReceiveMessageResponse(Message& message)
    : Response(), mMessage(&message)
{
}

void ReceiveMessageResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    mMessage->initFromXml(rootNode);
}

bool ReceiveMessageResponse::isSuccess()
{
    return mStatus == 200;
}

BatchReceiveMessageResponse::BatchReceiveMessageResponse(
    std::vector<Message>& messages)
    : Response(), mMessages(&messages)
{
}

BatchReceiveMessageRequest::BatchReceiveMessageRequest(
    const int32_t numOfMessages,
    const int32_t waitSeconds)
    : MessageRequest("GET"), mNumOfMessages(numOfMessages),
      mWaitSeconds(waitSeconds)
{
}

std::string BatchReceiveMessageRequest::getQueryString()
{
    string queryStr = "numOfMessages=" + StringTool::ToString(mNumOfMessages);
    if (mWaitSeconds > 0)
    {
        queryStr += "&waitseconds=" + StringTool::ToString(mWaitSeconds);
    }
    return queryStr;
}

const std::string& BatchReceiveMessageRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

void BatchReceiveMessageResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    int32_t index = 0;
    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(MESSAGE, name))
        {
            Message message;
            mMessages->push_back(message);
            mMessages->at(index).initFromXml(iterNode);
            index += 1;
        }

        iterNode = iterNode.next_sibling();
    }
}

bool BatchReceiveMessageResponse::isSuccess()
{
    return mStatus == 200;
}

DeleteMessageRequest::DeleteMessageRequest(const std::string& receiptHandle)
    : MessageRequest("DELETE"), mReceiptHandle(&receiptHandle)
{
}

std::string DeleteMessageRequest::getQueryString()
{
    return "ReceiptHandle=" + *mReceiptHandle;
}

const std::string& DeleteMessageRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

void DeleteMessageResponse::parseResponse()
{
    if (!isSuccess())
    {
        pugi::xml_node rootNode = toXML();
        parseCommonError(rootNode);
        return;
    }
}

bool DeleteMessageResponse::isSuccess()
{
    return mStatus == 204;
}

BatchDeleteMessageRequest::BatchDeleteMessageRequest(
    const std::vector<std::string>& receiptHandles)
    : MessageRequest("DELETE"), mReceiptHandles(&receiptHandles)
{
}

std::string BatchDeleteMessageRequest::getQueryString()
{
    return "";
}

const std::string& BatchDeleteMessageRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child("ReceiptHandles");
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    for (std::vector<std::string>::const_iterator iter = mReceiptHandles->begin();
        iter != mReceiptHandles->end(); ++iter)
    {
        node.append_child("ReceiptHandle").append_child(
            pugi::node_pcdata).set_value(iter->c_str());
    }

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

void BatchDeleteMessageResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();

    if (isCommonError(rootNode))
    {
        parseCommonError(rootNode);
        return;
    }

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(ERROR_TAG, name))
        {
            DeleteMessageFailedItem failedItem;

            xml_node iterErrorNode = iterNode.first_child();
            while (!iterErrorNode.empty())
            {
                if (iterErrorNode.type() != node_element)
                {
                    continue;
                }
                const pugi::char_t* itemName = iterErrorNode.name();
                if (0 == strcmp(ERROR_MESSAGE, itemName))
                {
                    failedItem.errorMessage = iterErrorNode.text().get();
                }
                else if (0 == strcmp(ERROR_CODE, itemName))
                {
                    failedItem.errorCode = iterErrorNode.text().get();
                }
                else if (0 == strcmp(RECEIPT_HANDLE, itemName))
                {
                    failedItem.receiptHandle = iterErrorNode.text().get();
                }

                iterErrorNode = iterErrorNode.next_sibling();
            }
            mDeleteMessageFailedItems.push_back(failedItem);
        }

        iterNode = iterNode.next_sibling();
    }
}

bool BatchDeleteMessageResponse::isSuccess()
{
    return mStatus == 204;
}

PeekMessageRequest::PeekMessageRequest()
    : MessageRequest("GET")
{
}

std::string PeekMessageRequest::getQueryString()
{
    return "peekonly=true";
}

const std::string& PeekMessageRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

PeekMessageResponse::PeekMessageResponse(Message& message)
    : Response(), mMessage(&message)
{
}

void PeekMessageResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    mMessage->initFromXml(rootNode);
}

bool PeekMessageResponse::isSuccess()
{
    return mStatus == 200;
}

BatchPeekMessageRequest::BatchPeekMessageRequest(
    const int32_t numOfMessages)
    : MessageRequest("GET"), mNumOfMessages(numOfMessages)
{
}

std::string BatchPeekMessageRequest::getQueryString()
{
    return "peekonly=true&numOfMessages="
        + StringTool::ToString(mNumOfMessages);
}

const std::string& BatchPeekMessageRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

BatchPeekMessageResponse::BatchPeekMessageResponse(
    std::vector<Message>& messages)
    : Response(), mMessages(&messages)
{
}

void BatchPeekMessageResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    int32_t index = 0;
    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(MESSAGE, name))
        {
            Message message;
            mMessages->push_back(message);
            mMessages->at(index).initFromXml(iterNode);
            index += 1;
        }

        iterNode = iterNode.next_sibling();
    }
}

bool BatchPeekMessageResponse::isSuccess()
{
    return mStatus == 200;
}

ChangeMessageVisibilityRequest::ChangeMessageVisibilityRequest(
    const std::string& receiptHandle,
    const int32_t visibilityTimeout)
    : MessageRequest("PUT")
    , mReceiptHandle(&receiptHandle)
    , mVisibilityTimeout(visibilityTimeout)
{
}

std::string ChangeMessageVisibilityRequest::getQueryString()
{
    return "receiptHandle=" + *mReceiptHandle
        + "&visibilityTimeout=" + StringTool::ToString(mVisibilityTimeout);
}

const std::string& ChangeMessageVisibilityRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

void ChangeMessageVisibilityResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(RECEIPT_HANDLE, name))
        {
            mReceiptHandle = iterNode.text().get();
        }
        else if (0 == strcmp(NEXT_VISIBLE_TIME, name))
        {
            mNextVisibleTime = atol(iterNode.text().get());
        }

        iterNode = iterNode.next_sibling();
    }
}

bool ChangeMessageVisibilityResponse::isSuccess()
{
	return mStatus == 200;
}

CreateTopicRequest::CreateTopicRequest(const string& topicName)
    : Request("PUT")
    , mTopicName(&topicName)
    , mAttributes(NULL)
{
}

CreateTopicRequest::CreateTopicRequest(const string& topicName,
                                       const TopicAttributes& attributes)
    : Request("PUT")
    , mTopicName(&topicName)
    , mAttributes(&attributes)
{
}

std::string CreateTopicRequest::getQueryString()
{
    return "";
}

std::string CreateTopicRequest::getResourcePath()
{
    return "/topics/" + *mTopicName;
}

const string& CreateTopicRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child("Topic");
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    if (mAttributes != NULL)
    {
        string maximumMessageSizeStr = StringTool::ToString(
            mAttributes->getMaximumMessageSize());

        if (mAttributes->getMaximumMessageSize() > 0)
            node.append_child("MaximumMessageSize").append_child(
                pugi::node_pcdata).set_value(maximumMessageSizeStr.c_str());

        if (mAttributes->isLoggingEnabledSet())
        {
            string loggingEnabledStr = mAttributes->isLoggingEnabled() ? "True" : "False";
            node.append_child("LoggingEnabled").append_child(
                pugi::node_pcdata).set_value(loggingEnabledStr.c_str());
        }
    }

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

CreateTopicResponse::CreateTopicResponse()
    : mTopicURL(""), Response()
{
}

bool CreateTopicResponse::isSuccess()
{
    return mStatus == 201 || mStatus == 204;
}

void CreateTopicResponse::parseResponse()
{
    if (isSuccess())
    {
        mTopicURL = getHeader("Location");
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

DeleteTopicRequest::DeleteTopicRequest(const string& topicName)
    : Request("DELETE")
    , mTopicName(&topicName)
{
}

string DeleteTopicRequest::getQueryString()
{
    return "";
}

string DeleteTopicRequest::getResourcePath()
{
    return "/topics/" + *mTopicName;
}

const string& DeleteTopicRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

bool DeleteTopicResponse::isSuccess()
{
    return mStatus == 204;
}

void DeleteTopicResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

ListTopicRequest::ListTopicRequest(const string& prefix,
                                   const string& marker,
                                   const int32_t retNum)
    : Request("GET")
{
    setMarker(marker);
    setPrefix(prefix);
    setRetNum(retNum);
}

void ListTopicRequest::setMarker(const string& marker)
{
    mMarker = marker;
    if (mMarker != "")
    {
        mHeaders["x-mns-marker"] = mMarker;
    }
    else
    {
        mHeaders.erase("x-mns-marker");
    }
}

void ListTopicRequest::setPrefix(const string& prefix)
{
    mPrefix = prefix;
    if (mPrefix != "")
    {
        mHeaders["x-mns-prefix"] = mPrefix;
    }
    else
    {
        mHeaders.erase("x-mns-prefix");
    }
}

void ListTopicRequest::setRetNum(const int32_t retNum)
{
    mRetNum = retNum;
    if (mRetNum > 0)
    {
        mHeaders["x-mns-ret-number"] = StringTool::ToString(mRetNum);
    }
    else
    {
        mHeaders.erase("x-mns-ret-number");
    }
}

string ListTopicRequest::getQueryString()
{
    return "";
}

string ListTopicRequest::getResourcePath()
{
    return "/topics";
}

const string& ListTopicRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

ListTopicResponse::ListTopicResponse(std::vector<std::string>& topicNames,
                                     std::string& nextMarker)
    : mTopicNames(&topicNames), mNextMarker(&nextMarker), Response()
{
}

bool ListTopicResponse::isSuccess()
{
    return mStatus == 200;
}

void ListTopicResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(TOPIC, name))
        {
            xml_node topicURLNode = iterNode.child(TOPIC_URL);
            string topicName = MNSUtils::getLastNodeFromURL(
                    topicURLNode.text().get());
            mTopicNames->push_back(topicName);
        }
        else if (0 == strcmp(NEXT_MARKER, name))
        {
            *mNextMarker = iterNode.text().get();
        }

        iterNode = iterNode.next_sibling();
    }
}

GetTopicAttributesRequest::GetTopicAttributesRequest(const string& topicName)
    : Request("GET"), mTopicName(&topicName)
{
}

string GetTopicAttributesRequest::getQueryString()
{
    return "";
}

string GetTopicAttributesRequest::getResourcePath()
{
    return "/topics/" + *mTopicName;
}

const string& GetTopicAttributesRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

GetTopicAttributesResponse::GetTopicAttributesResponse(
    TopicAttributes& attributes)
    : mAttributes(&attributes), Response()
{
}

bool GetTopicAttributesResponse::isSuccess()
{
    return mStatus == 200;
}

void GetTopicAttributesResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    mAttributes->clear();

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(TOPIC_NAME, name))
        {
            mAttributes->mTopicName = iterNode.text().get();
        }
        else if (0 == strcmp(CREATE_TIME, name))
        {
            mAttributes->mCreateTime = atol(iterNode.text().get());
        }
        else if (0 == strcmp(LAST_MODIFY_TIME, name))
        {
            mAttributes->mLastModifyTime = atol(iterNode.text().get());
        }
        else if (0 == strcmp(MAXIMUM_MESSAGE_SIZE, name))
        {
            mAttributes->mMaximumMessageSize = atol(iterNode.text().get());
        }
        else if (0 == strcmp(MAXIMUM_MESSAGE_SIZE, name))
        {
            mAttributes->mMessageRetentionPeriod = atol(iterNode.text().get());
        }
        else if (0 == strcmp(MESSAGE_COUNT, name))
        {
            mAttributes->mMessages = atol(iterNode.text().get());
        }
        else if (0 == strcmp(LOGGING_ENABLED, name))
        {
            mAttributes->mLoggingEnabled = (0 == strcmp(iterNode.text().get(), "True")) ? true : false;
            mAttributes->mLoggingEnabledSet = true;
        }

        iterNode = iterNode.next_sibling();
    }
}

SetTopicAttributesRequest::SetTopicAttributesRequest(
    const string& topicName,
    const TopicAttributes& attributes)
    : Request("PUT")
    , mTopicName(&topicName)
    , mAttributes(&attributes)
{
}

string SetTopicAttributesRequest::getQueryString()
{
    return "metaoverride=true";
}

string SetTopicAttributesRequest::getResourcePath()
{
    return "/topics/" + *mTopicName;
}

const string& SetTopicAttributesRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child("Topic");
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    string maximumMessageSizeStr = StringTool::ToString(
        mAttributes->getMaximumMessageSize());

    if (mAttributes->getMaximumMessageSize() > 0)
        node.append_child("MaximumMessageSize").append_child(
            pugi::node_pcdata).set_value(maximumMessageSizeStr.c_str());

    if (mAttributes->isLoggingEnabledSet())
    {
        string loggingEnabledStr = mAttributes->isLoggingEnabled() ? "True" : "False";
        node.append_child("LoggingEnabled").append_child(
            pugi::node_pcdata).set_value(loggingEnabledStr.c_str());
    }

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

bool SetTopicAttributesResponse::isSuccess()
{
    return mStatus == 204;
}

void SetTopicAttributesResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

PublishMessageRequest::PublishMessageRequest(const std::string& topicName,
                                             const std::string& messageBody)
    : Request("POST")
    , mTopicName(&topicName)
    , mMessageBody(&messageBody)
    , mMessageTag(NULL)
    , mpMessageAttributes(NULL)
{
}

PublishMessageRequest::PublishMessageRequest(const std::string& topicName,
                                             const std::string& messageBody,
                                             const MessageAttributes& messageAttributes)
    : Request("POST")
    , mTopicName(&topicName)
    , mMessageBody(&messageBody)
    , mMessageTag(NULL)
    , mpMessageAttributes(&messageAttributes)
{
}

PublishMessageRequest::PublishMessageRequest(const std::string& topicName,
                                             const std::string& messageBody,
                                             const std::string& messageTag)
    : Request("POST")
    , mTopicName(&topicName)
    , mMessageBody(&messageBody)
    , mMessageTag(&messageTag)
    , mpMessageAttributes(NULL)
{
}

PublishMessageRequest::PublishMessageRequest(const std::string& topicName,
                                             const std::string& messageBody,
                                             const std::string& messageTag,
                                             const MessageAttributes& messageAttributes)
    : Request("POST")
    , mTopicName(&topicName)
    , mMessageBody(&messageBody)
    , mMessageTag(&messageTag)
    , mpMessageAttributes(&messageAttributes)
{
}

string PublishMessageRequest::getQueryString()
{
    return "";
}

string PublishMessageRequest::getResourcePath()
{
    if (mTopicName == NULL)
        return "";
    return "/topics/" + *mTopicName + "/messages";
}

const string& PublishMessageRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child("Message");
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    node.append_child("MessageBody").append_child(
        pugi::node_pcdata).set_value(mMessageBody->c_str());

    if (mMessageTag != NULL)
    {
        node.append_child("MessageTag").append_child(
            pugi::node_pcdata).set_value(mMessageTag->c_str());
    }

    if (mpMessageAttributes != NULL
        && (mpMessageAttributes->getMailAttributesPtrNoCreate() != NULL
            || mpMessageAttributes->getSMSAttributesPtrNoCreate() != NULL))
    {
        pugi::xml_node attributesNode = node.append_child("MessageAttributes");
        if (mpMessageAttributes->getMailAttributesPtrNoCreate() != NULL)
        {
            const string& mailAttributesJsonStr =
                mpMessageAttributes->getMailAttributesPtrNoCreate()->toJson();
            attributesNode.append_child("DirectMail")
                .append_child(pugi::node_pcdata)
                .set_value(mailAttributesJsonStr.c_str());
        }
        if (mpMessageAttributes->getSMSAttributesPtrNoCreate() != NULL)
        {
            const string& smsAttributesJsonStr =
                mpMessageAttributes->getSMSAttributesPtrNoCreate()->toJson();
            attributesNode.append_child("DirectSMS")
                .append_child(pugi::node_pcdata)
                .set_value(smsAttributesJsonStr.c_str());
        }
    }

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

PublishMessageResponse::PublishMessageResponse()
    : mMessageId(""), mMessageBodyMD5("")
{
}

bool PublishMessageResponse::isSuccess()
{
    return mStatus == 201;
}

void PublishMessageResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(MESSAGE_ID, name))
        {
            mMessageId = iterNode.text().get();
        }
        else if (0 == strcmp(MESSAGE_BODY_MD5, name))
        {
            mMessageBodyMD5 = iterNode.text().get();
        }

        iterNode = iterNode.next_sibling();
    }
}

SubscribeRequest::SubscribeRequest(const string& topicName,
                                   const string& subscriptionName,
                                   const string& endPoint)
    : SubscriptionRequest("PUT", topicName, subscriptionName)
    , mFilterTag(NULL)
    , mEndPoint(&endPoint)
    , mAttributes(NULL)
{
}

SubscribeRequest::SubscribeRequest(const string& topicName,
                                   const string& subscriptionName,
                                   const string& endPoint,
                                   const SubscriptionAttributes& attributes)
    : SubscriptionRequest("PUT", topicName, subscriptionName)
    , mFilterTag(NULL)
    , mEndPoint(&endPoint)
    , mAttributes(&attributes)
{
}

SubscribeRequest::SubscribeRequest(const string& topicName,
                                   const string& subscriptionName,
                                   const string& filterTag,
                                   const string& endPoint)
    : SubscriptionRequest("PUT", topicName, subscriptionName)
    , mFilterTag(&filterTag)
    , mEndPoint(&endPoint)
    , mAttributes(NULL)
{
}

SubscribeRequest::SubscribeRequest(const string& topicName,
                                   const string& subscriptionName,
                                   const string& filterTag,
                                   const string& endPoint,
                                   const SubscriptionAttributes& attributes)
    : SubscriptionRequest("PUT", topicName, subscriptionName)
    , mFilterTag(&filterTag)
    , mEndPoint(&endPoint)
    , mAttributes(&attributes)
{
}

SubscribeRequest::~SubscribeRequest()
{
}

std::string SubscribeRequest::getQueryString()
{
    return "";
}

const std::string& SubscribeRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child(SUBSCRIPTION);
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    if (mFilterTag != NULL)
    {
        node.append_child("FilterTag").append_child(
            pugi::node_pcdata).set_value(mFilterTag->c_str());
    }

    node.append_child(ENDPOINT).append_child(
        pugi::node_pcdata).set_value(mEndPoint->c_str());

    if (mAttributes != NULL)
    {
        if (mAttributes->getNotifyStrategy() == BACKOFF_RETRY)
            node.append_child(NOTIFY_STRATEGY_STR).append_child(
                pugi::node_pcdata).set_value(BACKOFF_RETRY_STR);
        else if (mAttributes->getNotifyStrategy() == EXPONENTIAL_DECAY_RETRY)
            node.append_child(NOTIFY_STRATEGY_STR).append_child(
                pugi::node_pcdata).set_value(EXPONENTIAL_DECAY_RETRY_STR);

        if (mAttributes->getContentFormat() == XML)
            node.append_child(CONTENT_FORMAT_STR).append_child(
                pugi::node_pcdata).set_value(XML_STR);
        else if (mAttributes->getContentFormat() == SIMPLIFIED)
            node.append_child(CONTENT_FORMAT_STR).append_child(
                pugi::node_pcdata).set_value(SIMPLIFIED_STR);
        else if (mAttributes->getContentFormat() == JSON)
            node.append_child(CONTENT_FORMAT_STR).append_child(
                pugi::node_pcdata).set_value(JSON_STR);
    }

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

bool SubscribeResponse::isSuccess()
{
    return mStatus == 201 || mStatus == 204;
}

void SubscribeResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

SetSubscriptionAttributesRequest::SetSubscriptionAttributesRequest(
    const string& topicName,
    const string& subscriptionName,
    const SubscriptionAttributes& attributes)
    : SubscriptionRequest("PUT", topicName, subscriptionName)
    , mAttributes(&attributes)
{
}

SetSubscriptionAttributesRequest::~SetSubscriptionAttributesRequest()
{
}

std::string SetSubscriptionAttributesRequest::getQueryString()
{
    return "metaoverride=true";
}

const std::string& SetSubscriptionAttributesRequest::generateRequestBody()
{
    pugi::xml_document doc;
    doc.load("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", pugi::parse_declaration);
    pugi::xml_node node = doc.append_child(SUBSCRIPTION);
    node.append_attribute("xmlns") = MNS_XML_NAMESPACE_V1;

    if (mAttributes->getNotifyStrategy() == BACKOFF_RETRY)
        node.append_child(NOTIFY_STRATEGY_STR).append_child(
            pugi::node_pcdata).set_value(BACKOFF_RETRY_STR);
    else if (mAttributes->getNotifyStrategy() == EXPONENTIAL_DECAY_RETRY)
        node.append_child(NOTIFY_STRATEGY_STR).append_child(
            pugi::node_pcdata).set_value(EXPONENTIAL_DECAY_RETRY_STR);

    ostringstream os;
    doc.save(os);
    mRequestBody = os.str();
    return mRequestBody;
}

bool SetSubscriptionAttributesResponse::isSuccess()
{
    return mStatus == 204;
}

void SetSubscriptionAttributesResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

GetSubscriptionAttributesRequest::GetSubscriptionAttributesRequest(
    const string& topicName,
    const string& subscriptionName)
    : SubscriptionRequest("GET", topicName, subscriptionName)
{
}

std::string GetSubscriptionAttributesRequest::getQueryString()
{
    return "";
}

const std::string& GetSubscriptionAttributesRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

GetSubscriptionAttributesResponse::GetSubscriptionAttributesResponse(
    SubscriptionAttributes& attributes)
    : mAttributes(&attributes)
{
}

bool GetSubscriptionAttributesResponse::isSuccess()
{
    return mStatus == 200;
}

void GetSubscriptionAttributesResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(SUBSCRIPTION_NAME, name))
        {
            mAttributes->mSubscriptionName = iterNode.text().get();
        }
        else if (0 == strcmp(TOPIC_OWNER, name))
        {
            mAttributes->mTopicOwner = iterNode.text().get();
        }
        else if (0 == strcmp(TOPIC_NAME, name))
        {
            mAttributes->mTopicName = iterNode.text().get();
        }
        else if (0 == strcmp(FILTER_TAG, name))
        {
            mAttributes->mFilterTag = iterNode.text().get();
        }
        else if (0 == strcmp(ENDPOINT, name))
        {
            mAttributes->mEndPoint = iterNode.text().get();
        }
        else if (0 == strcmp(NOTIFY_STRATEGY_STR, name))
        {
            mAttributes->mNotifyStrategy =
                NotifyStrategyHelper::StringToNotifyStrategy(iterNode.text().get());
        }
        else if (0 == strcmp(CONTENT_FORMAT_STR, name))
        {
            mAttributes->mContentFormat =
                ContentFormatHelper::StringToContentFormat(iterNode.text().get());
        }
        else if (0 == strcmp(CREATE_TIME, name))
        {
            mAttributes->mCreateTime = atol(iterNode.text().get());
        }
        else if (0 == strcmp(LAST_MODIFY_TIME, name))
        {
            mAttributes->mLastModifyTime = atol(iterNode.text().get());
        }

        iterNode = iterNode.next_sibling();
    }
}

UnsubscribeRequest::UnsubscribeRequest(const string& topicName,
                                       const string& subscriptionName)
    : SubscriptionRequest("DELETE", topicName, subscriptionName)
{
}

std::string UnsubscribeRequest::getQueryString()
{
    return "";
}

const std::string& UnsubscribeRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

bool UnsubscribeResponse::isSuccess()
{
    return mStatus == 204;
}

void UnsubscribeResponse::parseResponse()
{
    if (isSuccess())
    {
        return;
    }

    pugi::xml_node rootNode = toXML();
    parseCommonError(rootNode);
}

ListSubscriptionRequest::ListSubscriptionRequest(const string& prefix,
                                                 const string& marker,
                                                 const int32_t retNum)
    : Request("GET")
{
    setMarker(marker);
    setPrefix(prefix);
    setRetNum(retNum);
}

void ListSubscriptionRequest::setMarker(const string& marker)
{
    mMarker = marker;
    if (mMarker != "")
    {
        mHeaders["x-mns-marker"] = mMarker;
    }
    else
    {
        mHeaders.erase("x-mns-marker");
    }
}

void ListSubscriptionRequest::setPrefix(const string& prefix)
{
    mPrefix = prefix;
    if (mPrefix != "")
    {
        mHeaders["x-mns-prefix"] = mPrefix;
    }
    else
    {
        mHeaders.erase("x-mns-prefix");
    }
}

void ListSubscriptionRequest::setRetNum(const int32_t retNum)
{
    mRetNum = retNum;
    if (mRetNum > 0)
    {
        mHeaders["x-mns-ret-number"] = StringTool::ToString(mRetNum);
    }
    else
    {
        mHeaders.erase("x-mns-ret-number");
    }
}

void ListSubscriptionRequest::setTopicName(const string& topicName)
{
    mTopicName = &topicName;
}

string ListSubscriptionRequest::getQueryString()
{
    return "";
}

string ListSubscriptionRequest::getResourcePath()
{
    return "/topics/" + *mTopicName + "/subscriptions";
}

const string& ListSubscriptionRequest::generateRequestBody()
{
    mRequestBody = "";
    return mRequestBody;
}

ListSubscriptionResponse::ListSubscriptionResponse(
    std::vector<std::string>& subscriptionNames,
    std::string& nextMarker)
    : Response()
    , mSubscriptionNames(&subscriptionNames)
    , mNextMarker(&nextMarker)
{
}

bool ListSubscriptionResponse::isSuccess()
{
    return mStatus == 200;
}

void ListSubscriptionResponse::parseResponse()
{
    pugi::xml_node rootNode = toXML();
    if (!isSuccess())
    {
        parseCommonError(rootNode);
        return;
    }

    xml_node iterNode = rootNode.first_child();
    while (!iterNode.empty())
    {
        if (iterNode.type() != node_element)
        {
            continue;
        }
        const pugi::char_t* name = iterNode.name();
        if (0 == strcmp(SUBSCRIPTION, name))
        {
            xml_node subNode = iterNode.child(SUBSCRIPTION_URL);
            string subscriptionName = MNSUtils::getLastNodeFromURL(
                    subNode.text().get());
            mSubscriptionNames->push_back(subscriptionName);
        }
        else if (0 == strcmp(NEXT_MARKER, name))
        {
            *mNextMarker = iterNode.text().get();
        }

        iterNode = iterNode.next_sibling();
    }
}
