// TPHtmlCtrl.cpp : implementation file
//

#include "stdafx.h"
 

// CTPHtmlCtrl

IMPLEMENT_DYNCREATE(CTPHtmlCtrl, CHtmlView)

CTPHtmlCtrl::CTPHtmlCtrl()
{

}

CTPHtmlCtrl::~CTPHtmlCtrl()
{
}

void CTPHtmlCtrl::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTPHtmlCtrl, CHtmlView)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CTPHtmlCtrl diagnostics
int CTPHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg)
{
	// bypass CView doc/frame stuff
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, msg);
}

void CTPHtmlCtrl::OnBeforeNavigate2( LPCTSTR lpszURL,
								  DWORD nFlags,
								  LPCTSTR lpszTargetFrameName,
								  CByteArray& baPostedData,
								  LPCTSTR lpszHeaders,
								  BOOL* pbCancel )
{
	//转向规则
	const TCHAR APP_PROTOCOL[] = _T("app:");
	int len = _tcslen(APP_PROTOCOL);
	if (_tcsnicmp(lpszURL, APP_PROTOCOL, len)==0) {
		OnAppCmd(lpszURL + len);
		*pbCancel = TRUE;
	}
}
void CTPHtmlCtrl::OnAppCmd(LPCTSTR lpszWhere)
{
	// default: do nothing
}






#ifdef _DEBUG
void CTPHtmlCtrl::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CTPHtmlCtrl::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CTPHtmlCtrl message handlers
