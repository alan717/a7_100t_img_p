// CMySheet.cpp: 实现文件
//

#include "stdafx.h"
#include "DIALOGS.h"
#include "CMySheet.h"


// CMySheet

IMPLEMENT_DYNAMIC(CMySheet, CMFCPropertySheet)

CMySheet::CMySheet()
{
	AddPage(&_historypage);
	AddPage(&_infopage);
	//	PropSheetLook_Tabs,
	//	PropSheetLook_OutlookBar,
	//	PropSheetLook_Tree,
	//	PropSheetLook_OneNoteTabs,
	//	PropSheetLook_List,
	SetLook(CMFCPropertySheet::PropSheetLook_Tabs);
	// out look style
	//m_imagelist.Create(IDR_MAINFRAME, 32, 1, RGB(128, 128, 128));
	//SetIconsList(m_imagelist);
	//SetLook(CMFCPropertySheet::PropSheetLook_OutlookBar);
}

CMySheet::~CMySheet()
{
}


BEGIN_MESSAGE_MAP(CMySheet, CMFCPropertySheet)
END_MESSAGE_MAP()



// CMySheet 消息处理程序


