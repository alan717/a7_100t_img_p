// MainDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "DIALOGS.h"
#include "MainDialog.h"
#include "afxdialogex.h"


// MainDialog 对话框

IMPLEMENT_DYNAMIC(MainDialog, CDialogEx)

MainDialog::MainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MainDialog, pParent)
{
	m_donghua = new CAnimateCtrl;
}

MainDialog::~MainDialog()
{
	delete m_donghua;
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BOOL MainDialog::OnInitDialog()
{
	RECT Recto = { 5, 5, 360, 360 };
	m_donghua->Create(WS_CHILD | WS_VISIBLE,
		Recto, this, 0x1884);
	m_donghua->Open(L"res\\clock.AVI");
	TRACE("SSSS");
	return true;
}

BEGIN_MESSAGE_MAP(MainDialog, CDialogEx)
END_MESSAGE_MAP()


// MainDialog 消息处理程序
