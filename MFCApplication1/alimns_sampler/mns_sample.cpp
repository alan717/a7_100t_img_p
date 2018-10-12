#include <iostream>
#include <vector>
#include <fstream>
#include "mns/mns_client.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace mns::sdk;

string trim(string& s)
{
    int n;
    for (n = s.size() - 1; n >= 0; n--)
    {
        if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n' && s[n] != '\r')
        {
            break;    
        }
    }
    return s.substr(0, n+1);
}

void readConfig(string& endpoint, string& accessId, string& accessKey, string& stsToken)
{
    stsToken = "";
    ifstream is_file("aliyun-mns.properties");
	if (!is_file.is_open())
	{
		cout << "Cannot find config: aliyun-mns.properties" << endl;
		return;
	}
    std::string line;
    while( std::getline(is_file, line) )
    {
        std::istringstream is_line(line);
        std::string key;
        if( std::getline(is_line, key, '=') )
        {
            std::string value;
            if( std::getline(is_line, value) )
            {
                if (key == "mns.accountendpoint")
                {
                    endpoint = trim(value);
                }
                else if (key == "mns.accesskeyid")
                {
                    accessId = trim(value);
                }
                else if (key == "mns.accesskeysecret")
                {
                    accessKey = trim(value);
                }
                else if (key == "mns.ststoken")
                {
                    stsToken = trim(value);
                }
            }
        }
    }
}

int main()
{
    string endpoint;
    string accessId;
    string accessKey;
    string stsToken;
    readConfig(endpoint, accessId, accessKey, stsToken);
	if (endpoint == "")
	{
		cout << "Endpoint Empty!" << endl;
		return -1;
	}
    MNSClient mnsClient(endpoint, accessId, accessKey, stsToken);
    // set account attributes
    try
    {
        AccountAttributes attributes;
        mnsClient.getAccountAttributes(attributes);
        cout << "LoggingBucket: " << attributes.getLoggingBucket() << endl;

        AccountAttributes attributes2;
        attributes2.setLoggingBucket("Test");
        mnsClient.setAccountAttributes(attributes2);
        mnsClient.getAccountAttributes(attributes);
        cout << "LoggingBucket: " << attributes.getLoggingBucket() << endl;

        AccountAttributes attributes3;
        mnsClient.setAccountAttributes(attributes3);
        mnsClient.getAccountAttributes(attributes);
        cout << "LoggingBucket: " << attributes.getLoggingBucket() << endl;

        attributes2.setLoggingBucket("");
        mnsClient.setAccountAttributes(attributes2);
        mnsClient.getAccountAttributes(attributes);
        cout << "LoggingBucket: " << attributes.getLoggingBucket() << endl;
    }
    catch (MNSServerException& me)
    {
        cout << "getAccountAttributes Failed: " + me.GetErrorCode() << endl;
        // handle exception according to the errorcode
        return -1;
    }
    catch (MNSExceptionBase& mb)
    {
        cout << "getAccountAttributes Failed: " + mb.ToString() << endl;
        // log it and retry
        return -2;
    }

    QueuePtr queue;
    try
    {
        mnsClient.deleteQueue("TestCplusplusQ");
        queue = mnsClient.createQueue("TestCplusplusQ");
        cout << "QueueName: " << queue->getQueueName() << endl;
    }
    catch (MNSServerException& me)
    {
        cout << "CreateQueue Failed: " + me.GetErrorCode() << endl;
        // handle exception according to the errorcode
        return -1;
    }
    catch (MNSExceptionBase& mb)
    {
        cout << "CreateQueue Failed: " + mb.ToString() << endl;
        // log it and retry
        return -2;
    }

    // set logging enabled
    try
    {
        QueueAttributes attributes;
        queue->getAttributes(attributes);
        cout << "LoggingEnabled: "
             << attributes.isLoggingEnabled() << endl;

        QueueAttributes attributes2;
        attributes2.setLoggingEnabled(true);
        queue->setAttributes(attributes2);

        queue->getAttributes(attributes);
        cout << "LoggingEnabled: "
             << attributes.isLoggingEnabled() << endl;

        QueueAttributes attributes3;
        queue->setAttributes(attributes3);
        queue->getAttributes(attributes);
        cout << "LoggingEnabled: "
             << attributes.isLoggingEnabled() << endl;

        QueueAttributes attributes4;
        attributes4.setLoggingEnabled(false);
        queue->setAttributes(attributes4);
        queue->getAttributes(attributes);
        cout << "LoggingEnabled: "
             << attributes.isLoggingEnabled() << endl;
    }
    catch (MNSServerException& me)
    {
        cout << "setAttributes Failed: " + me.GetErrorCode() << endl;
        // handle exception according to the errorcode
        return -1;
    }
    catch (MNSExceptionBase& mb)
    {
        cout << "setAttributes Failed: " + mb.ToString() << endl;
        // log it and retry
        return -2;
    }

    try
    {
        mnsClient.createQueue("TestCplusplusQ1");
        mnsClient.createQueue("TestCplusplusQ2");
        mnsClient.createQueue("TestCplusplusQ3");
        mnsClient.createQueue("TestCplusplusQ4");
        mnsClient.createQueue("TestCplusplusQ5");
        ListQueueRequest req("TestCplusplus", "", 2);
        vector<string> queueNames;
        string nextMarker;
        mnsClient.listQueue(req, queueNames, nextMarker);
        for (int i = 0; i < queueNames.size(); ++i)
        {
            cout << queueNames[i] << endl;
        }
        cout << nextMarker << endl;
        mnsClient.deleteQueue("TestCplusplusQ1");
        mnsClient.deleteQueue("TestCplusplusQ2");
        mnsClient.deleteQueue("TestCplusplusQ3");
        mnsClient.deleteQueue("TestCplusplusQ4");
        mnsClient.deleteQueue("TestCplusplusQ5");

        queue = mnsClient.getQueueRef("TestCplusplusQ");

        QueueAttributes attributes;
        queue->getAttributes(attributes);
        cout << "VisibilityTimeout: "
             << attributes.getVisibilityTimeout() << endl;

        attributes.setVisibilityTimeout(20);
        queue->setAttributes(attributes);

        // single message
        SendMessageResponse smResp;
        queue->sendMessage("Test<HAHA>\"Messge", smResp);
        Message message1;
        queue->peekMessage(message1);
        cout << "PeekMessage: " << message1.getMessageBody() << endl;

        Message message2;
        queue->receiveMessage(message2);
        cout << "ReceiveMessage: " << message2.getMessageBody() << endl;

        ChangeMessageVisibilityResponse cmvResp;
        queue->changeMessageVisibility(message2.getReceiptHandle(), 30, cmvResp);
        queue->deleteMessage(cmvResp.getReceiptHandle());
        cout << "Message Deleted!" << endl;

        // delay message
        {
            SendMessageResponse smResp;
            cout << "Send Delay Message" << endl;
            queue->sendMessage("Test<HAHA>\"Messge", 3, 8, smResp);
            Message message1;
            cout << "Wait 5 seconds..." << endl;
#ifdef _WIN32
			Sleep(5000);
#else
            sleep(5);
#endif
            queue->peekMessage(message1);
            cout << "PeekMessage: " << message1.getMessageBody() << endl;
            Message message2;
            queue->receiveMessage(message2);
            cout << "ReceiveMessage: " << message2.getMessageBody() << endl;
            ChangeMessageVisibilityResponse cmvResp;
            queue->changeMessageVisibility(message2.getReceiptHandle(), 30, cmvResp);
            queue->deleteMessage(cmvResp.getReceiptHandle());
            cout << "Message Deleted!" << endl;
        }

        // now batch
        std::vector<SendMessageItem> sendMessageItems;
        SendMessageItem item("Test<HAHA>\"MessgeBatch");
        sendMessageItems.push_back(item);
        sendMessageItems.push_back(item);

        BatchSendMessageResponse bsmResp;
        queue->batchSendMessage(sendMessageItems, bsmResp);
        const std::vector<SendMessageSucceedItem>& succeedItems =
            bsmResp.getSendMessageSucceedItems();
        for (std::vector<SendMessageSucceedItem>::const_iterator iter = succeedItems.begin();
            iter != succeedItems.end(); ++iter)
        {
            cout << "BatchSendSucceedItem: " << iter->messageId << endl;
        }
        if (!bsmResp.isSuccess())
        {
            const std::vector<SendMessageFailedItem>& failedItems =
                bsmResp.getSendMessageFailedItems();
            for (std::vector<SendMessageFailedItem>::const_iterator iter = failedItems.begin();
                iter != failedItems.end(); ++iter)
            {
                cout << "BatchSendFailedItem: " << iter->errorCode << endl;
            }
        }

        std::vector<Message> messages;
        queue->batchPeekMessage(5, messages);
        cout << "BatchPeeked " << messages.size() << " Messages!" << endl;
        for (std::vector<Message>::iterator iter = messages.begin();
            iter != messages.end(); ++iter)
        {
            cout << "PeekdItem (MessageId: " << iter->getMessageId()
                 << "MessageBody: " << iter->getMessageBody() << ")" << endl;
        }

        messages.clear();
        queue->batchReceiveMessage(5, messages);
        cout << "BatchReceived " << messages.size() << " Messages!" << endl;

        std::vector<std::string> receiptHandles;
        for (std::vector<Message>::iterator iter = messages.begin();
            iter != messages.end(); ++iter)
        {
            cout << "ReceivedItem (MessageId: " << iter->getMessageId()
                 << "MessageBody: " << iter->getMessageBody() << ")" << endl;
            receiptHandles.push_back(iter->getReceiptHandle());
        }

        BatchDeleteMessageResponse bdmResp;
        queue->batchDeleteMessage(receiptHandles, bdmResp);
        if (!bdmResp.isSuccess())
        {
            const std::vector<DeleteMessageFailedItem>& failedItems =
                bdmResp.getDeleteMessageFailedItem();
            for (std::vector<DeleteMessageFailedItem>::const_iterator iter = failedItems.begin();
                iter != failedItems.end(); ++iter)
            {
                cout << "BatchDeleteFailedItem: " << iter->errorCode
                     << "  " << iter->receiptHandle << endl;
            }
        }
        cout << "BatchDeleted!" << endl;

        mnsClient.deleteQueue("TestCplusplusQ");
        return 0;
    }
    catch (MNSServerException& me)
    {
        cout << "Request Failed: " + me.GetErrorCode() << endl;
        // handle exception according to the errorcode
        return -1;
    }
    catch (MNSExceptionBase& mb)
    {
        cout << "Request Failed: " + mb.ToString() << endl;
        // log it and retry
        return -2;
    }
}
