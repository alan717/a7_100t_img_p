#pragma once
#include"CHistory.h"
#include"CInformationTechnology.h"
// CMySheet

class CMySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(CMySheet)

public:
	CMySheet();
	virtual ~CMySheet();
private:
	CHistory _historypage;
	CInformationTechnology _infopage;
	CImageList m_imagelist;
protected:
	DECLARE_MESSAGE_MAP()
};


