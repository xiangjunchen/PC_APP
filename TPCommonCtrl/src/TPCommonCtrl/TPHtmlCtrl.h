#pragma once

#ifdef _WIN32_WCE
#error "CHtmlView is not supported for Windows CE."
#endif 
 
// CTPHtmlCtrl html view

class AFX_EXT_CLASS CTPHtmlCtrl : public CHtmlView,public CTPBaseCtrl
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


public:
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);
	// override to trap "app:" pseudo protocol
	virtual void OnBeforeNavigate2( LPCTSTR lpszURL,
		DWORD nFlags,
		LPCTSTR lpszTargetFrameName,
		CByteArray& baPostedData,
		LPCTSTR lpszHeaders,
		BOOL* pbCancel );

	// override to handle links to "app:mumble...". lpszWhere will be "mumble"
	virtual void OnAppCmd(LPCTSTR lpszWhere);
	virtual BOOL Create(
		LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName,
		DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd,
		UINT nID,
		CCreateContext* pContext = NULL 
		);
	virtual void OnQuit( );

protected:
	TPScrollBarData          *m_pScrollBar;
	SCROLLINFO                m_sVScrollInfo;
	SCROLLINFO                m_sHScrollInfo;
	CPoint                    m_ptScrollPos;
	BOOL					  m_bSubWindow;

	void SubCtrl(BOOL bCreate);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


