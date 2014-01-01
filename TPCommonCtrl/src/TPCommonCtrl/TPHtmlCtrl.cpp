// TPHtmlCtrl.cpp : implementation file
//

#include "stdafx.h"
 

// CTPHtmlCtrl

IMPLEMENT_DYNCREATE(CTPHtmlCtrl, CHtmlView)

CTPHtmlCtrl::CTPHtmlCtrl()
{
	m_pScrollBar = NULL;
	m_bSubWindow = FALSE;
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
void CTPHtmlCtrl::SubCtrl(BOOL bCreate)
{
 	//if(bCreate)
 	//{	
 	//	if(m_bSubWindow) return;
 	//	m_bSubWindow = TRUE;	
 	//	m_pScrollBar = TP_ReplaceWnd(this);
 	//	m_pScrollBar ->iScrollHeight = TPSCROLLBAR;
 	//	m_pScrollBar ->iScrollWidth  = TPSCROLLBAR;
 	//	m_pScrollBar ->m_pVScrollInfo = &m_sVScrollInfo;
 	//	m_pScrollBar ->m_pHScrollInfo = &m_sHScrollInfo;
 	//	SaveFont(m_hWnd);
 	//}
 	//else
 	//{
 	//	if(!m_bSubWindow) return;
 	//	m_bSubWindow = FALSE;	
 	//	TP_UnReplaceWnd(this);
 	//	m_pScrollBar = NULL;
 	//	RestoreFont(m_hWnd);
 	//}
}
BOOL CTPHtmlCtrl::Create(
					LPCTSTR lpszClassName,
					LPCTSTR lpszWindowName,
					DWORD dwStyle,
					const RECT& rect,
					CWnd* pParentWnd,
					UINT nID,
					CCreateContext* pContext  
					)
{
	BOOL bCreate = CHtmlView::Create(lpszClassName, lpszWindowName,dwStyle,rect,pParentWnd,nID,pContext);

	SubCtrl(TRUE);
	return bCreate;
}
void CTPHtmlCtrl::OnQuit( )
{
	ASSERT(0);
}

// CTPHtmlCtrl message handlers
