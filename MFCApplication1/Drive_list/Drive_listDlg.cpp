
// Drive_listDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Drive_list.h"
#include "Drive_listDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDrivelistDlg 对话框



CDrivelistDlg::CDrivelistDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRIVE_LIST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (m_hIcon);
		//SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)m_hIcon);
}

void CDrivelistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, btnWhatElse);
}

BEGIN_MESSAGE_MAP(CDrivelistDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDrivelistDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDrivelistDlg::OnBnClickedCancel)
	ON_MESSAGE(FUCK, &CDrivelistDlg::OnFuck)
	ON_MESSAGE(WM_USER_REFURBISH_TEST_RESULT,&CDrivelistDlg::OnFresh)
END_MESSAGE_MAP()


// CDrivelistDlg 消息处理程序

BOOL CDrivelistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//SetWindowLongPtr(m_hWnd, GWL_STYLE, ~WS_CAPTION);
	//SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_SYSMENU);
	// TODO: 在此添加额外的初始化代码
	SetBackgroundColor(278394);
	SetBackgroundImage(IDB_BITMAP1);
	
	edtTotal ->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(20, 30, 200, 50), this, 2002);
	//btnWhatElse = reinterpret_cast<CButton *>(GetDlgItem(IDOK));
	//btnWhatElse->EnableWindow(false);


	CListCtrl *lstCtrl = new CListCtrl;

	lstCtrl->Create(WS_CHILD | WS_VISIBLE,
		CRect(210, 100, 280,130 ), this, 0x285);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDrivelistDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDrivelistDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDrivelistDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDrivelistDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	PostQuitMessage(125);
}


void CDrivelistDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
	
	
	CString ss;
	edtTotal->GetWindowText(ss);
	if (ss.IsEmpty())
	{
		btnWhatElse.SetWindowText(L"FUCK");
		HANDLE SS =GETWINDOWTHEME();
	}
	else {
		btnWhatElse.SetWindowText(ss);
		//SetDlgItemText(edtTotal,Result);
		TRACE("WM_USER_THREADEND message \n");
		SendMessage(WM_USER_REFURBISH_TEST_RESULT, _E_STATUS_READY, NULL);
	}
	
	btnWhatElse.EnableWindow();
}


afx_msg LRESULT CDrivelistDlg::OnFuck(WPARAM wParam, LPARAM lParam)
{
	CDialogEx::OnCancel();
	
	return 0;
}
afx_msg LRESULT CDrivelistDlg::OnFresh(WPARAM wParam, LPARAM lParam)
{
	CString*p_str = (CString *)lParam;
	switch (wParam)
	{
	case _E_STATUS_PASS:
		//m_CtrlStaticResult.SetBkColor(gsColor[COLOR_LIME]);
		//m_CtrlStaticResult.SetWindowText(_T("PASS"));
		break;
	case _E_STATUS_FAIL:
		//m_CtrlStaticResult.SetBkColor(gsColor[COLOR_RED]);
		//m_CtrlStaticResult.SetWindowText(_T("FAIL"));
		break;
	case _E_STATUS_DOING:
		//m_CtrlStaticResult.SetBkColor(gsColor[COLOR_YELLOW]);
		//m_CtrlStaticResult.SetWindowText(*p_str);
		break;
	case _E_STATUS_READY:
		afxDump << "Hello World " << "fcuk\n" ;
		break;
	default:
		//m_CtrlStaticResult.SetBkColor(gsColor[COLOR_PINK]);
		//m_CtrlStaticResult.SetWindowText(_T("Ready"));
		break;
	}
	return 0;

 
}