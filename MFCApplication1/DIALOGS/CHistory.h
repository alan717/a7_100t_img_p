#pragma once


// CHistory

class CHistory : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CHistory)
	//DIAGLOG DATA
	enum{IDD=IDD_CHistory};
public:
	CHistory();
	virtual ~CHistory();

protected:
	DECLARE_MESSAGE_MAP()
};


