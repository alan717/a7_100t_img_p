#pragma once


// MainDialog 对话框

class MainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MainDialog)

public:
	MainDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MainDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainDialog };
#endif
private:
	CAnimateCtrl *m_donghua;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL MainDialog::OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
