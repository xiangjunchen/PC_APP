#pragma once

#ifdef _WIN32_WCE
#error "CHtmlView is not supported for Windows CE."
#endif 

// CTPHtmlCtrl html view

class CTPHtmlCtrl : public CHtmlView
{
	DECLARE_DYNCREATE(CTPHtmlCtrl)

public:
	CTPHtmlCtrl();           // protected constructor used by dynamic creation
	virtual ~CTPHtmlCtrl();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


