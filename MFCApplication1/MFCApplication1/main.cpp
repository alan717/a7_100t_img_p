#include<afxwin.h>
#include"stdafx.h"
class CMyFrame : public CFrameWnd {
public:
	CMyFrame() {
		Create(NULL, _T("MFC Application Tutorial"), WS_SYSMENU,CRect(90,120,400,500),NULL);
	}
};

class CExample : public CWinApp {
	BOOL InitInstance() {
		CMyFrame *Frame = new CMyFrame();
		m_pMainWnd = Frame;

		Frame->ShowWindow(SW_NORMAL);
		Frame->UpdateWindow();
 		return TRUE;
	}
};

CExample theApp;