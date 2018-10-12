// Copyright (C) 2015, Alibaba Cloud Computing

#ifndef MNS_CLIENT_H
#define MNS_CLIENT_H

#include "mns_protocol.h"
#include "mns_network_tool.h"
#include <map>
#include <stdint.h>
#include <vector>

#ifdef _WIN32
#include <memory>
#else
#include <tr1/memory>
#endif

namespace mns
{
namespace sdk
{

class Queue;
class Topic;
class MNSClient;

typedef std::tr1::shared_ptr<Queue> QueuePtr;
typedef std::tr1::shared_ptr<Topic> TopicPtr;

/*
 * use MNSClient to access MNS service
 *
 * CAUTION:
 *     Make sure to catch Exception when calling any function
 *     MNSClient throws MNSServerException when the request fails.
 *     MNSClient throws MNSExceptionBase for client errors
 */
class MNSClient
{
public:
    /* init the MNSClient for calling MNS service
     *
     * @param endpoint:
     *      http://{AccountId}.mns.cn-hangzhou.aliyuncs.com
     *      "cn-hangzhou" is the region place
     * @param accessId:  accessId from aliyun.com
     * @param accessKey: accessKey from aliyun.com
     * @param connPoolSize:
     *      MNSClient keeps a pool of connections and reuse them
     */
    MNSClient(const std::string& endpoint,
              const std::string& accessId,
              const std::string& accessKey,
              const int32_t connPoolSize = 200,
              const int32_t timeout = 35,
              const int32_t connectTimeout = 35);

    /* init the MNSClient for calling MNS service
    *
    * @param endpoint:
    *      http://{AccountId}.mns.cn-hangzhou.aliyuncs.com
    *      "cn-hangzhou" is the region place
    * @param accessId:  the sts accessId
    * @param accessKey: the sts accessKey
    * @param stsToken:  the sts token
    * @param connPoolSize:
    *      MNSClient keeps a pool of connections and reuse them
    */
    MNSClient(const std::string& endpoint,
              const std::string& accessId,
              const std::string& accessKey,
              const std::string& stsToken,
              const int32_t connPoolSize = 200,
              const int32_t timeout = 35,
              const int32_t connectTimeout = 35);

    /* update the AccessId/AccessKey
    *
    * @param accessId: accessId from aliyun.com
    * @param accessKey: accessKey from aliyun.com
    */
    void updateAccessId(const std::string& accessId,
                        const std::string& accessKey);

    /* update the AccessId/AccessKey/StsToken
    *
    * @param accessId:  the sts accessId
    * @param accessKey: the sts accessKey
    * @param stsToken:  the sts token
    */
    void updateAccessId(const std::string& accessId,
                        const std::string& accessKey,
                        const std::string& stsToken);

    virtual ~MNSClient() {}

    /* get Account attributes
     *
     * @param attributes: the returned Account attributes
     */
    void getAccountAttributes(AccountAttributes& attributes);

    /* set Account attributes
     *
     * @param attributes: the attributes to set
     */
    void setAccountAttributes(const AccountAttributes& attributes);

    /* Create Queue with specified name
     *
     * @param queueName: the queue name
     *
     * @return: the created queue instance
     * @throws: MNSServerException
     */
    QueuePtr createQueue(const std::string& queueName);

    /* Create Queue with specified name and attributes
     *
     * @param queueName: the queue name
     * @param attributes: the queue attributes
     *
     * @return: the created queue instance
     */
    QueuePtr createQueue(const std::string& queueName,
                         const QueueAttributes& attributes);

    /* init Queue instance with specified name without creating queue
     *
     * @param queueName: the queue name
     *
     * @return: the inited queue instance
     */
    QueuePtr getQueueRef(const std::string& queueName);

    /* delete queue with specified name
     *
     * @param queueName: the queue name
     */
    void deleteQueue(const std::string& queueName);

    /* list queues with retNum=1000 and marker=""
     *
     * @param prefix: the prefix of queueName
     *
     * @param queueNames: the response queueNames
     */
    void listQueue(const std::string& prefix,
        std::vector<std::string>& queueNames);

    /* list queues with retNum=1000 and marker=""
     *
     * @param prefix: the prefix of queueName
     *
     * @param queueNames: the response queueNames
     * @param nextMarker: the page marker for next listQueue
     */
    void listQueue(const std::string& prefix,
        std::vector<std::string>& queueNames,
        std::string& nextMarker);

    /* list queues
     *
     * @param req: the ListQueueRequest containing filters
     * @param queueNames: the response queueNames
     */
    void listQueue(ListQueueRequest& req,
        std::vector<std::string>& queueNames);

    /* list queues
     *
     * @param req: the ListQueueRequest containing filters
     * @param queueNames: the response queueNames
     * @param nextMarker: the page marker for next listQueue
     */
    void listQueue(ListQueueRequest& req,
        std::vector<std::string>& queueNames,
        std::string& nextMarker);

    /* Create Topic with specified name
     *
     * @param topicName: the topic name
     *
     * @return: the created topic instance
     * @throws: MNSServerException
     */
    TopicPtr createTopic(const std::string& topicName);

    /* Create Topic with specified name and attributes
     *
     * @param topicName: the topic name
     * @param attributes: the topic attributes
     *
     * @return: the created topic instance
     */
    TopicPtr createTopic(const std::string& topicName,
                         const TopicAttributes& attributes);

    /* init Topic instance with specified name without creating topic
     *
     * @param topicName: the topic name
     *
     * @return: the inited topic instance
     */
    TopicPtr getTopicRef(const std::string& topicName);

    /* delete topic with specified name
     *
     * @param topicName: the topic name
     */
    void deleteTopic(const std::string& topicName);

    /* list topics with retNum=1000 and marker=""
     *
     * @param prefix: the prefix of topicName
     * @param topicNames: the response topicNames
     */
    void listTopic(const std::string& prefix,
                   std::vector<std::string>& topicNames);

    /* list topics with retNum=1000 and marker=""
     *
     * @param prefix: the prefix of topicName
     * @param topicNames: the response topicNames
     * @param nextMarker: the page marker for next listTopic
     */
    void listTopic(const std::string& prefix,
                   std::vector<std::string>& topicNames,
                   std::string& nextMarker);

    /* list topics
     *
     * @param req: the ListTopicRequest containing filters
     * @param topicNames: the response topicNames
     */
    void listTopic(ListTopicRequest& req,
                   std::vector<std::string>& topicNames);

    /* list topics
     *
     * @param req: the ListTopicRequest containing filters
     * @param topicNames: the response topicNames
     * @param nextMarker: the page marker for next listTopic
     */
    void listTopic(ListTopicRequest& req,
                   std::vector<std::string>& topicNames,
                   std::string& nextMarker);

    const std::string& getEndpoint() const
    {
        return mEndPoint;
    }
    const std::string& GetAccessId() const
    {
        return mAccessId;
    }
    const std::string& GetAccessKey() const
    {
        return mAccessKey;
    }
    const std::string& GetStsToken() const
    {
        return mStsToken;
    }

public:
    static void sendRequest(Request& req,
                            Response& response,
                            const std::string& endpoint,
                            const std::string& accessId,
                            const std::string& accessKey,
                            const std::string& stsToken,
                            MNSConnectionToolPtr mnsConnTool);

    static void signRequest(Request& req,
                            const std::string& endpoint,
                            const std::string& accessId,
                            const std::string& accessKey,
                            const std::string& stsToken);

protected:
    std::string mEndPoint;
    std::string mAccessId;
    std::string mAccessKey;
    std::string mStsToken;
    MNSConnectionToolPtr mMnsConnTool;
};

/*
 * access MNS Queue service
 *
 * CAUTION:
 *     Make sure to catch Exception when calling any function
 *     Queue throws MNSServerException when the request is failed.
 *     Queue throws MNSExceptionBase for client errors
 */
class Queue
{
public:
    virtual ~Queue() {}

    /* update the AccessId/AccessKey
    *
    * @param accessId: accessId from aliyun.com
    * @param accessKey: accessKey from aliyun.com
    */
    void updateAccessId(const std::string& accessId,
                        const std::string& accessKey);

    /* update the AccessId/AccessKey/StsToken
    *
    * @param accessId:  the sts accessId
    * @param accessKey: the sts accessKey
    * @param stsToken:  the sts token
    */
    void updateAccessId(const std::string& accessId,
                        const std::string& accessKey,
                        const std::string& stsToken);

    const std::string& getQueueName()
    {
        return mQueueName;
    }

    /* get queue attributes
     *
     * @param attributes: the returned queue attributes
     */
    void getAttributes(QueueAttributes& attributes);

    /* set queue attributes
     *
     * @param attributes: the attributes to set
     */
    void setAttributes(const QueueAttributes& attributes);

    /* send one message
     *
     * @param messageBody: the message body
     * @param resp: the Response containing MessageId and BodyMD5
     */
    void sendMessage(const std::string& messageBody,
                     SendMessageResponse& resp);

    /* send one message
     *
     * @param messageBody: the message body
     * @delaySeconds: the message cannot be received in DelaySeconds
     * @priority:
     *     whether this message has a higher/lower priority, default is 8
     * @param resp: the Response containing MessageId and BodyMD5
     */
    void sendMessage(const std::string& messageBody,
                     const int32_t delaySeconds,
                     const int32_t priority,
                     SendMessageResponse& resp);

    /* send a batch of messages
     *
     * @param sendMessageItems: the messages to send
     * @param resp: the Response containing MessageIds and BodyMD5s
     *     1. responses of sent messages are in mSendMessageSucceedItems
     *     2. check resp.isSuccess(), if not success, some messages are not sent
     *        the failInfos are recorded in mSendMessageFailedItems
     */
    void batchSendMessage(const std::vector<SendMessageItem>& sendMessageItems,
                          BatchSendMessageResponse& resp);

    /* peek one message, the message still can be received by others
     *
     * @param message: the peeked message
     */
    void peekMessage(Message& message);

    /* batch peek
     *
     * @param numOfMessages: the batch size
     * @param messages: the peeked messages
     */
    void batchPeekMessage(const int32_t numOfMessages,
                          std::vector<Message>& messages);

    /* receive one active message,
     *    the message is changed to be inactive.
     *    message will become active after the "MessageVisibilityTimeout"
     *    ChangeMessageVisibility could be used to change the "VisibilityTime"
     *
     * @param message: the received message
     */
    void receiveMessage(Message& message);

    /* receive one active message,
     *    the message is changed to be inactive.
     *    message will become active after the "MessageVisibilityTimeout"
     *    ChangeMessageVisibility could be used to change the "VisibilityTime"
     *
     * @param waitSeconds:
     *     if no message exist in the queue, the request will block for
     *         "waitSeconds" util timeout or any message coming.
     * @param message: the received message
     */
    void receiveMessage(const int32_t waitSeconds,
                        Message& message);

    /* receive active messages,
     *    the message is changed to be inactive.
     *    message will become active after the "MessageVisibilityTimeout"
     *    ChangeMessageVisibility could be used to change the "VisibilityTime"
     *
     * @param numOfMessages: the batch size
     * @param messages: received messages
     */
    void batchReceiveMessage(const int32_t numOfMessages,
                             std::vector<Message>& messages);

    /* receive active messages,
     *    the message is changed to be inactive.
     *    message will become active after the "MessageVisibilityTimeout"
     *    ChangeMessageVisibility could be used to change the "VisibilityTime"
     *
     * @param numOfMessages: the batch size
     * @param waitSeconds:
     *     if no message exist in the queue, the request will block for
     *         "waitSeconds" util timeout or any message coming.
     * @param messages: received messages
     */
    void batchReceiveMessage(const int32_t numOfMessages,
                             const int32_t waitSeconds,
                             std::vector<Message>& messages);

    /* delete one inactive message,
     *    after receiving the message, a ReceiptHandle is returned in Message
     *    this ReceiptHandle is valid util the "VisibilityTimeout"
     *    this ReceiptHandle could be used to delete the message
     *
     * @param receiptHandle: the ReceiptHandle
     */
    void deleteMessage(const std::string& receiptHandle);

    /* delete several inactive messages,
     *    after receiving the message, a ReceiptHandle is returned in Message
     *    this ReceiptHandle is valid util the "VisibilityTimeout"
     *    this ReceiptHandle could be used to delete the message
     *
     * @param receiptHandles: the ReceiptHandles
     */
    void batchDeleteMessage(const std::vector<std::string>& receiptHandles,
                            BatchDeleteMessageResponse& resp);

    /* change the visibilityTimeout for inactive message
     *    after receiving the message, a ReceiptHandle is returned in Message
     *    this ReceiptHandle is valid util the "VisibilityTimeout"
     *    a valid ReceiptHandle could be used to change the "VisibilityTimeout"
     *
     * @param receiptHandle: the ReceiptHandle
     * @param visibilityTimeout:
     *     the message will be active again after "visibilityTimeout" seconds
     */
    void changeMessageVisibility(const std::string& receiptHandle,
                                 const int32_t visibilityTimeout,
                                 ChangeMessageVisibilityResponse& resp);

    friend class MNSClient;

protected:
    Queue(const std::string& queueName,
          const std::string& endpoint,
          const std::string& accessId,
          const std::string& accessKey,
          const std::string& stsToken,
          MNSConnectionToolPtr mnsConnTool);

protected:
    std::string mQueueName;;
    std::string mEndPoint;
    std::string mAccessId;
    std::string mAccessKey;
    std::string mStsToken;
    MNSConnectionToolPtr mMnsConnTool;
};

/*
 * access MNS Topic service
 *
 * CAUTION:
 *     Make sure to catch Exception when calling any function
 *     Topic throws MNSServerException when the request is failed.
 *     Topic throws MNSExceptionBase for client errors
 */
class Topic
{
public:
    virtual ~Topic() {}

    /* update the AccessId/AccessKey
    *
    * @param accessId: accessId from aliyun.com
    * @param accessKey: accessKey from aliyun.com
    */
    void updateAccessId(const std::string& accessId,
                        const std::string& accessKey);

    /* update the AccessId/AccessKey/StsToken
    *
    * @param accessId:  the sts accessId
    * @param accessKey: the sts accessKey
    * @param stsToken:  the sts token
    */
    void updateAccessId(const std::string& accessId,
                        const std::string& accessKey,
                        const std::string& stsToken);

    const std::string& getTopicName()
    {
        return mTopicName;
    }

    /* get topic attributes
     *
     * @param attributes: the returned topic attributes
     */
    void getAttributes(TopicAttributes& attributes);

    /* set optic attributes
     *
     * @param attributes: the attributes to set
     */
    void setAttributes(const TopicAttributes& attributes);

    /* publish one message to topic
     *
     * @param messageBody: the message body
     * @param resp: the Response containing MessageId and BodyMD5
     */
    void publishMessage(const std::string& messageBody,
                        PublishMessageResponse& resp);

    /* publish one message to topic
     *
     * @param messageBody: the message body
     * @param messageAttributes: related message attributes
     * @param resp: the Response containing MessageId and BodyMD5
     */
    void publishMessage(const std::string& messageBody,
                        const MessageAttributes& messageAttributes,
                        PublishMessageResponse& resp);

    /* publish one message to topic with messageTag
     *
     * @param messageBody: the message body
     * @param messageTag: the message Tag
     * @param resp: the Response containing MessageId and BodyMD5
     */
    void publishMessage(const std::string& messageBody,
                        const std::string& messageTag,
                        PublishMessageResponse& resp);

    /* publish one message to topic with messageTag
     *
     * @param messageBody: the message body
     * @param messageTag: the message Tag
     * @param messageAttributes: related message attributes
     * @param resp: the Response containing MessageId and BodyMD5
     */
    void publishMessage(const std::string& messageBody,
                        const std::string& messageTag,
                        const MessageAttributes& messageAttributes,
                        PublishMessageResponse& resp);

    /** generate queue endpoint for subscription
     *
     * @param queueName: mns queue name
     * @return: mns queue endpoint
     */
    std::string generateQueueEndpoint(const std::string& queueName);

    /** generate queue endpoint for subscription
     *
     * @param queueName: mns queue name
     * @param region: mns region in endpoint url, such as cn-hangzhou, cn-shenzhen, cn-beijing, ...
     * @return: mns queue endpoint
     */
    std::string generateQueueEndpoint(const std::string& queueName, const std::string& region);

    /** generate mail endpoint for subscription
     *
     * @param mailAddress: mail address for receiving mails
     * @return: mns mail endpoint
     */
    std::string generateMailEndpoint(const std::string& mailAddress);

    /** generate sms endpoint for subscription
     *
     * @param phone: phone number for receiving sms, could be empty 
     *   (empty phone means that the number could be specified in SMSAttributes)
     * @return: mns sms endpoint
     */
    std::string generateSMSEndpoint(const std::string& phone = "");

    /* subscribe to topic
     *
     * @param subscriptionName: the unique name for the subscription
     * @param endPoint: the subscription endpoint
     */
    void subscribe(const std::string& subscriptionName,
                   const std::string& endPoint);

    /* subscribe to topic
     *
     * @param subscriptionName: the unique name for the subscription
     * @param endPoint: the subscription endpoint
     * @param attributes: the subscription attributes
     */
    void subscribe(const std::string& subscriptionName,
                   const std::string& endPoint,
                   const SubscriptionAttributes& attributes);

    /* subscribe to topic with fiterTag
     *
     * @param subscriptionName: the unique name for the subscription
     * @param filterTag: the filterTag for the subscription
     * @param endPoint: the subscription endpoint
     */
    void subscribe(const std::string& subscriptionName,
                   const std::string& filterTag,
                   const std::string& endPoint);

    /* subscribe to topic with fiterTag
     *
     * @param subscriptionName: the unique name for the subscription
     * @param filterTag: the filterTag for the subscription
     * @param endPoint: the subscription endpoint
     * @param attributes: the subscription attributes
     */
    void subscribe(const std::string& subscriptionName,
                   const std::string& filterTag,
                   const std::string& endPoint,
                   const SubscriptionAttributes& attributes);

    /* change the subscription attributes
     *
     * @param subscriptionName: name of the subscription to change
     * @param attributes: the attributes to set
     */
    void setSubscriptionAttributes(const std::string& subscriptionName,
                                   const SubscriptionAttributes& attributes);

    /* get the subscription attributes
     *
     * @param subscriptionName: name of the subscription
     * @param attributes: the returned attributes
     */
    void getSubscriptionAttributes(const std::string& subscriptionName,
                                   SubscriptionAttributes& attributes);

    /* Unsubscribe
     *
     * @param subscriptionName: name of the subscription
     */
    void unsubscribe(const std::string& subscriptionName);

    /* List the subscriptions under current topic
     *
     * @param prefix: the prefix of subscriptionName
     * @param subscriptionNames: the subscription names
     */
    void listSubscription(const std::string& prefix,
                          std::vector<std::string>& subscriptionNames);

    /* List the subscriptions under current topic
     *
     * @param prefix: the prefix of subscriptionName
     * @param subscriptionNames: the subscription names
     * @param nextMarker: the page marker for next listSubscription
     */
    void listSubscription(const std::string& prefix,
                          std::vector<std::string>& subscriptionNames,
                          std::string& nextMarker);

    /* List the subscriptions under current topic
     *
     * @param prefix: the prefix of subscriptionName
     * @param subscriptionNames: the subscription names
     */
    void listSubscription(ListSubscriptionRequest& request,
                          std::vector<std::string>& subscriptionNames);

    /* List the subscriptions under current topic
     *
     * @param prefix: the prefix of subscriptionName
     * @param subscriptionNames: the subscription names
     * @param nextMarker: the page marker for next listSubscription
     */
    void listSubscription(ListSubscriptionRequest& request,
                          std::vector<std::string>& subscriptionNames,
                          std::string& nextMarker);

    friend class MNSClient;

protected:
    Topic(const std::string& topicName,
          const std::string& endpoint,
          const std::string& accessId,
          const std::string& accessKey,
          const std::string& stsToken,
          MNSConnectionToolPtr mnsConnTool);

protected:
    std::string mTopicName;
    std::string mEndPoint;
    std::string mAccessId;
    std::string mAccessKey;
    std::string mStsToken;
    MNSConnectionToolPtr mMnsConnTool;

    std::string mRegion;
    std::string mAccountId;
};

}
}

#endif
