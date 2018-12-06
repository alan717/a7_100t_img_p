#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include <iostream>
//TThostFtdcBrokerIDType brokerid;
class MyHandler : public CThostFtdcTraderSpi
{
private:
	CThostFtdcTraderApi *m_pUserApi;
	TThostFtdcBrokerIDType g_chBrokerID;//brokerid
	TThostFtdcUserIDType g_chUserID;
public:
	//
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///报单回报
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	// 针对用户请求的出错通知
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
		bool bIsLast);
	virtual void OnRspSettlementInfoConfirm(
		CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast);

	virtual void OnRspQrySettlementInfo(
		CThostFtdcSettlementInfoField *pSettlementInfo,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast);


	MyHandler(CThostFtdcTraderApi *pUserApi, TThostFtdcBrokerIDType brokerid, TThostFtdcUserIDType userid) :m_pUserApi(pUserApi)  {
		strcpy(g_chBrokerID,brokerid);
		strcpy(g_chUserID,userid);
	}
	~MyHandler() {}
};