#include<iostream>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "UserMd.h"

using namespace std;
int main()
{
	CThostFtdcMdApi* pUserMdApi = CThostFtdcMdApi::CreateFtdcMdApi("",true);
	 
	return 0 ;
}