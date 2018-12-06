#include"UserSpi.h"
#include<iostream>


void MyHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,bool bIsLast) {
	printf("OnRspError:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
		pRspInfo->ErrorMsg);
	printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	// �ͻ�������д�����
	//{�ͻ��˵Ĵ�����}
}

void MyHandler::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	printf("OnRtnOrder:\n");
	printf("OrderSysID=[%s]\n", pOrder->OrderSysID);
}

// ����¼��Ӧ��
  void MyHandler::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	// �������¼����
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
		pRspInfo->ErrorMsg);
	// ֪ͨ����¼�����
	//SetEvent(g_hEvent);
};

void MyHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
  {
	  printf("OnRspUserLogin:\n");
	  printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
		  pRspInfo->ErrorMsg);
	  printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	  if (pRspInfo->ErrorID != 0) {
			// �˵�ʧ�ܣ��ͻ�������д�����
			printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain = %d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
			exit(-1);
	  }
	  // �˵ǳɹ�,��������¼������
	  CThostFtdcInputOrderField ord;
	  memset(&ord, 0, sizeof(ord));
	  //���͹�˾����
	  strcpy(ord.BrokerID, g_chBrokerID);
	  //Ͷ���ߴ���
	  strcpy(ord.InvestorID, "098306");
	  // ��Լ����
	  strcpy(ord.InstrumentID, "m1901");
	  ///��������
	  strcpy(ord.OrderRef, "000000000001");
	  // �û�����
	  strcpy(ord.UserID, g_chUserID);
	  // �����۸�����
	  ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	  // ��������
	  ord.Direction = THOST_FTDC_D_Buy;
	  // ��Ͽ�ƽ��־
	  strcpy(ord.CombOffsetFlag, "0");
	  // ���Ͷ���ױ���־
	  strcpy(ord.CombHedgeFlag, "1");
	  // �۸�
	  ord.LimitPrice = 50000;
	  // ����
	  ord.VolumeTotalOriginal = 10;
	  // ��Ч������
	  ord.TimeCondition = THOST_FTDC_TC_GFD;
	  // GTD����
	  strcpy(ord.GTDDate, "");
	  // �ɽ�������
	  ord.VolumeCondition = THOST_FTDC_VC_AV;
	  // ��С�ɽ���
	  ord.MinVolume = 0;
	  // ��������
	  ord.ContingentCondition = THOST_FTDC_CC_Immediately;
	  // ֹ���
	  ord.StopPrice = 0;
	  // ǿƽԭ��
	  ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	  // �Զ������־
	  ord.IsAutoSuspend = 0;
	  //m_pUserApi->ReqOrderInsert(&ord, 1);
 
	  CThostFtdcQrySettlementInfoField pQrySettlementInfo;
	  memset(&pQrySettlementInfo, 0, sizeof(pQrySettlementInfo));
	  strcpy(pQrySettlementInfo.BrokerID,g_chBrokerID);
	  strcpy(pQrySettlementInfo.InvestorID, g_chUserID);
	  strcpy(pQrySettlementInfo.TradingDay, "20181130");
	  //strcpy_s(pQrySettlementInfo.TradingDay, TradingDay(pRspUserLogin->TradingDay).prev_day().to_string().c_str());
	  //strcpy(pQrySettlementInfo.TradingDay,);
	  m_pUserApi->ReqQrySettlementInfo(&pQrySettlementInfo,1);
  }


  void MyHandler::OnFrontDisconnected(int nReason)
{
	// ��������������API���Զ��������ӣ��ͻ��˿ɲ�������
	printf("OnFrontDisconnected.\n");
}


void MyHandler::OnFrontConnected()
  {
	  CThostFtdcReqUserLoginField reqUserLogin;
	  // get BrokerID
	  printf("BrokerID:%s",g_chBrokerID);
	  //scanf("%s", &g_chBrokerID);
	  strcpy(reqUserLogin.BrokerID, g_chBrokerID);
	  // get userid
	  printf("userid:%s",g_chUserID);
	  //scanf("%s", &g_chUserID);
	  strcpy(reqUserLogin.UserID, g_chUserID);
	  // get password
	 
	  strncpy(reqUserLogin.Password, "12345678", sizeof(reqUserLogin.Password));
	  printf("password:%s",reqUserLogin.Password);
	  // ������½����
	  m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
  }

void MyHandler::OnRspSettlementInfoConfirm(
	CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{	
	printf("OnRspUserLogin:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
		pRspInfo->ErrorMsg);
	printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	if (pRspInfo->ErrorID != 0) {
		// �˵�ʧ�ܣ��ͻ�������д�����
		printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain = %d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
		exit(-1);
	}
}
void MyHandler::OnRspQrySettlementInfo(
	CThostFtdcSettlementInfoField *pSettlementInfo,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	printf("OnRspUserLogin:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
		pRspInfo->ErrorMsg);
	printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	printf("%s", pSettlementInfo->TradingDay);
	if (pRspInfo->ErrorID != 0) {
		// �˵�ʧ�ܣ��ͻ�������д�����
		printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain = %d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
		exit(-1);
	}
	CThostFtdcQrySettlementInfoConfirmField coir;
	memset(&coir,0,sizeof(coir));
	strcpy(coir.BrokerID,g_chBrokerID);
	strcpy(coir.InvestorID,g_chUserID);
	// strcpy(coir.ConfirmTime, pSettlementInfo->)
	//strcpy()
	m_pUserApi->ReqQrySettlementInfoConfirm(&coir,2);
}
