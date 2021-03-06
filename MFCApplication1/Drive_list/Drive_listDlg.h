
// Drive_listDlg.h: 头文件
//

#pragma once

#define WM_USER_REFURBISH_TEST_RESULT		(WM_USER + 1001)
// CDrivelistDlg 对话框
class CDrivelistDlg : public CDialogEx
{
// 构造
public:
	CDrivelistDlg(CWnd* pParent = nullptr);	// 标准构造函数
	CButton btnWhatElse;
	CEdit* edtTotal = new CEdit;
	enum { _E_STATUS_PASS , _E_STATUS_FAIL,
		_E_STATUS_DOING, _E_STATUS_READY
	};
	void CDrivelistDlg::OnBnClickedLogicaldrivesBtn();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRIVE_LIST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnFresh(WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT OnFuck(WPARAM wParam, LPARAM lParam);
};
