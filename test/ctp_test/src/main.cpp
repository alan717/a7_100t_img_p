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
	// ����˽����
	// TERT_RESTART:�ӱ������տ�ʼ�ش�
	// TERT_RESUME:���ϴ��յ�������
	// TERT_QUICK:ֻ���͵�¼��˽����������
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	pUserApi->SubscribePublicTopic(THOST_TERT_RESUME);
	pUserApi->RegisterFront("tcp://180.168.146.187:10030");
	pUserApi->Init();
	// �ͷ�APIʵ��
	pUserApi->Release();
	return 0 ;
}