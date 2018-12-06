#include<iostream>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "UserMd.h"
#include "UserSpi.h"
using namespace std;
int main()
{
	//CThostFtdcMdApi* pUserMdApi = CThostFtdcMdApi::CreateFtdcMdApi("",true);
	CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	MyHandler mh(pUserApi,"9999","098306");
	pUserApi->RegisterSpi(&mh);
	// 订阅私有流
	// TERT_RESTART:从本交易日开始重传
	// TERT_RESUME:从上次收到的续传
	// TERT_QUICK:只传送登录后私有流的内容
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	pUserApi->SubscribePublicTopic(THOST_TERT_RESUME);
	pUserApi->RegisterFront("tcp://180.168.146.187:10030");
	pUserApi->Init();
	// 释放API实例
	pUserApi->Release();
	return 0 ;
}