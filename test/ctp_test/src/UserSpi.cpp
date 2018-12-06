#include"UserSpi.h"
#include<iostream>


void MyHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,bool bIsLast) {
	printf("OnRspError:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
		pRspInfo->ErrorMsg);
	printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	// 客户端需进行错误处理
	//{客户端的错误处理}
}

void MyHandler::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	printf("OnRtnOrder:\n");
	printf("OrderSysID=[%s]\n", pOrder->OrderSysID);
}

// 报单录入应答
  void MyHandler::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	// 输出报单录入结果
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
		pRspInfo->ErrorMsg);
	// 通知报单录入完成
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
			// 端登失败，客户端需进行错误处理
			printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain = %d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
			exit(-1);
	  }
	  // 端登成功,发出报单录入请求
	  CThostFtdcInputOrderField ord;
	  memset(&ord, 0, sizeof(ord));
	  //经纪公司代码
	  strcpy(ord.BrokerID, g_chBrokerID);
	  //投资者代码
	  strcpy(ord.InvestorID, "098306");
	  // 合约代码
	  strcpy(ord.InstrumentID, "m1901");
	  ///报单引用
	  strcpy(ord.OrderRef, "000000000001");
	  // 用户代码
	  strcpy(ord.UserID, g_chUserID);
	  // 报单价格条件
	  ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	  // 买卖方向
	  ord.Direction = THOST_FTDC_D_Buy;
	  // 组合开平标志
	  strcpy(ord.CombOffsetFlag, "0");
	  // 组合投机套保标志
	  strcpy(ord.CombHedgeFlag, "1");
	  // 价格
	  ord.LimitPrice = 50000;
	  // 数量
	  ord.VolumeTotalOriginal = 10;
	  // 有效期类型
	  ord.TimeCondition = THOST_FTDC_TC_GFD;
	  // GTD日期
	  strcpy(ord.GTDDate, "");
	  // 成交量类型
	  ord.VolumeCondition = THOST_FTDC_VC_AV;
	  // 最小成交量
	  ord.MinVolume = 0;
	  // 触发条件
	  ord.ContingentCondition = THOST_FTDC_CC_Immediately;
	  // 止损价
	  ord.StopPrice = 0;
	  // 强平原因
	  ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	  // 自动挂起标志
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
	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
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
	  // 发出登陆请求
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
		// 端登失败，客户端需进行错误处理
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
		// 端登失败，客户端需进行错误处理
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
