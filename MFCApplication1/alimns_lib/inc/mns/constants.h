// Copyright (C) 2015, Alibaba Cloud Computing

#ifndef MNS_CONSTANTS_H
#define MNS_CONSTANTS_H

namespace mns
{
namespace sdk
{

const char* const GET = "GET";
const char* const PUT = "PUT";
const char* const POST = "POST";
const char* const DELETE_METHOD = "DELETE";

const char* const HOST = "Host";
const char* const CURRENT_VERSION = "2015-06-06";
const char* const CONTENT_TYPE = "Content-Type";
const char* const CONTENT_LENGTH = "Content-Length";
const char* const SECURITY_TOKEN = "security-token";
const char* const DATE = "Date";
const char* const MNS_VERSION = "x-mns-version";
const char* const MNS_PREFIX = "x-mns-prefix";
const char* const MNS_MARKER = "x-mns-marker";
const char* const MNS_RETNUM = "x-mns-ret-number";
const char* const AUTHORIZATION = "Authorization";
const char* const CONTENT_MD5 = "Content-MD5";
const char* const LOCATION = "Location";
const char* const CANONICALIZED_MNS_HEADER_PREFIX = "x-mns-";
const char* const MNS_XML_NAMESPACE_V1 = "http://mns.aliyuncs.com/doc/v1";
const char* const DEFAULT_CONTENT_TYPE = "text/xml;charset=UTF-8";

//Error code
const char* const MESSAGE_NOT_EXIST = "MessageNotExist";
const char* const QUEUE_NOT_EXIST = "QueueNotExist";
const char* const TOPIC_NOT_EXIST = "TopicNotExist";
const char* const SUBSCRIPTION_NOT_EXIST = "SubscriptionNotExist";
const char* const QUEUE_ALREADY_EXIST = "QueueAlreadyExist";
const char* const TOPIC_ALREADY_EXIST = "TopicAlreadyExist";
const char* const SUBSCRIPTION_ALREADY_EXIST = "SubscriptionAlreadyExist";
const char* const STATE_CONFLICT = "StateConflict";
const char* const REQUEST_TIMEOUT = "RequestTimeout";

const char* const BEGIN_CURSOR = "begin";
const char* const NEXT_CURSOR = "next";

const char* const HTTP_PREFIX = "http://";
const char* const HTTPS_PREFIX = "https://";

const char* const NOTIFY_STRATEGY_BACKOFF_RETRY = "BACKOFF_RETRY";
const char* const NOTIFY_STRATEGY_EXPONENTIAL_DECAY_RETRY = "EXPONENTIAL_DECAY_RETRY";
const char* const SUBSCRIPTION_STATE_ACTIVE = "Active";
const char* const SUBSCRIPTION_STATE_INACTIVE = "Inactive";

const char* const ERROR_TAG = "Error";
const char* const QUEUE = "Queue";
const char* const QUEUES = "Queues";
const char* const TOPIC = "Topic";
const char* const TOPICS = "Topics";
const char* const SUBSCRIPTION = "Subscription";
const char* const SUBSCRIPTIONS = "Subscriptions";
const char* const QUEUE_URL = "QueueURL";
const char* const TOPIC_URL = "TopicURL";
const char* const SUBSCRIPTION_URL = "SubscriptionURL";
const char* const QUEUE_NAME = "QueueName";
const char* const MESSAGE = "Message";
const char* const REQUEST_ID = "RequestId";
const char* const HOST_ID = "HostId";
const char* const MESSAGE_ID = "MessageId";
const char* const MESSAGE_BODY = "MessageBody";
const char* const MESSAGE_BODY_MD5 = "MessageBodyMD5";
const char* const RECEIPT_HANDLE = "ReceiptHandle";
const char* const ENQUEUE_TIME = "EnqueueTime";
const char* const FIRST_DEQUEUE_TIME = "FirstDequeueTime";
const char* const NEXT_VISIBLE_TIME = "NextVisibleTime";
const char* const DEQUEUE_COUNT = "DequeueCount";
const char* const PRIORITY = "Priority";
const char* const CODE = "Code";
const char* const CREATE_TIME = "CreateTime";
const char* const LAST_MODIFY_TIME = "LastModifyTime";
const char* const VISIBILITY_TIMEOUT = "VisibilityTimeout";
const char* const DELAY_SECONDS = "DelaySeconds";
const char* const MAXIMUM_MESSAGE_SIZE = "MaximumMessageSize";
const char* const MESSAGE_RETENTION_PERIOD= "MessageRetentionPeriod";
const char* const ACTIVE_MESSAGES = "ActiveMessages";
const char* const INACTIVE_MESSAGES = "InactiveMessages";
const char* const DELAY_MESSAGES = "DelayMessages";
const char* const POLLING_WAIT_SECONDS = "PollingWaitSeconds";
const char* const ERROR_MESSAGE = "ErrorMessage";
const char* const ERROR_CODE = "ErrorCode";
const char* const NEXT_MARKER = "NextMarker";
const char* const MESSAGE_COUNT = "MessageCount";
const char* const LOGGING_BUCKET = "LoggingBucket";
const char* const LOGGING_ENABLED = "LoggingEnabled";

const char* const ENDPOINT = "Endpoint";
const char* const BACKOFF_RETRY_STR = "BACKOFF_RETRY";
const char* const EXPONENTIAL_DECAY_RETRY_STR = "EXPONENTIAL_DECAY_RETRY";
const char* const XML_STR = "XML";
const char* const SIMPLIFIED_STR = "SIMPLIFIED";
const char* const JSON_STR = "JSON";
const char* const NOTIFY_STRATEGY_STR = "NotifyStrategy";
const char* const CONTENT_FORMAT_STR = "NotifyContentFormat";
const char* const SUBSCRIPTION_NAME = "SubscriptionName";
const char* const TOPIC_OWNER = "TopicOwner";
const char* const TOPIC_NAME = "TopicName";
const char* const FILTER_TAG = "FilterTag";
}
}

#endif
