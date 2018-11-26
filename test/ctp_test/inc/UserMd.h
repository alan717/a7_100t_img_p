#ifndef _H_USERMD
#include"ThostFtdcMdApi.h"
#include "ThostFtdcUserApiStruct.h"
class UserMd :public CThostFtdcMdSpi
{
public:
	virtual void OnFrontConnected();
	virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) ;
};
#endif // !_H_USERMD
