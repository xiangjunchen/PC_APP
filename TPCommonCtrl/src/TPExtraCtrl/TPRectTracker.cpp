

#include "stdafx.h"
#include "TPRectTracker.h"
#include ".\tprecttracker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTPRectTracker

CTPRectTracker::CTPRectTracker()
{
	m_nStyle        = CRectTracker::dottedLine;//|CRectTracker::resizeInside;	
	m_rect          = CRect(0,0,100,100);
	m_rtInit        = m_rect;
	m_dwWindowLevel = TPWNDLEVEL1;
	m_eBorderStyle  = TP_GetBorderStyle();
	m_bChangeSize   = FALSE; 

}

CTPRectTracker::~CTPRectTracker()
{
}
BEGIN_MESSAGE_MAP(CTPRectTracker, CTPWnd)
	//{{AFX_MSG_MAP(CTPRectTracker)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP	
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void     CTPRectTracker::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_TABCONTROL,&wndClass);		
		wndClass.lpszClassName = WC_TPRECTTRACKER;		
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_TPRECTTRACKER,AfxGetInstanceHandle());
}

/////////////////////////////////////////////////////////////////////////////
// CTPRectTracker message handlers
BOOL CTPRectTracker::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bResult = CTPWnd::Create(dwStyle, rect, pParentWnd,nID );
	m_rtRange = rect;
	m_rect    = rect;
	return bResult;
}
void CTPRectTracker::AdjustRect(int nHandle, LPRECT pRect)
{
//	CTPRectTracker::AdjustRect(nHandle, pRect);
	if(!m_bChangeSize)
	{
		pRect ->bottom = pRect ->top + m_rtInit.Height();
		pRect ->right  = pRect ->left + m_rtInit.Width();
	}

}
void CTPRectTracker::OnChangedRect(const CRect& rectOld)
{
	CRect rtClient; 
	CRect rtRange = m_rect;
	GetClientRect(&rtClient);

	rtRange.left   = m_rect.left * m_rtRange.Width()  / rtClient.Width(); 
	rtRange.right  = m_rect.right * m_rtRange.Width()/ rtClient.Width(); 
	rtRange.top    = m_rect.top * m_rtRange.Height()  / rtClient.Height(); 
	rtRange.bottom = m_rect.bottom * m_rtRange.Height()/ rtClient.Height(); 

	GetParent()->SendMessage(WM_TPRECTTRACKER,0,(LPARAM)&rtRange);
	Invalidate();
	UpdateWindow();
}

void CTPRectTracker::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//if(CRectTracker::HitTest(point) >= 0)
	//{
		//CRect rect;	
		//GetClientRect(&rect); zx
		//ClientToScreen(&rect);
		//ClipCursor(&rect);
		Track(this,point,TRUE);	
		//ClipCursor(NULL);//控件的问题!
	//}
	CTPWnd::OnLButtonDown(nFlags, point);
}

void CTPRectTracker::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTPWnd::OnLButtonUp(nFlags, point);
}
BOOL CTPRectTracker::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (pWnd == this && CRectTracker::SetCursor(this, nHitTest)) 	return TRUE;
	else return CTPWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CTPRectTracker::OnPaint() 
{
	CPaintDC dc(this); 	
	CRect rcClient;
	GetClientRect( &rcClient );
	CTPMemoDC dcMmeo(&dc,rcClient.Width(),rcClient.Height());	
	dcMmeo.FillSolidRect(&rcClient,TP_GetSysColor(COLOR_BASE_WINDOW));	
	Draw(&dcMmeo);
	dc.BitBlt(rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height(),&dcMmeo,0,0,SRCCOPY);
	
}

CRect CTPRectTracker::GetTrackRect()
{
	CRect rtClient; 
	CRect rtRange = m_rect;
	GetClientRect(&rtClient);

	rtRange.left   = m_rect.left * m_rtRange.Width()  / rtClient.Width(); 
	rtRange.right  = m_rect.right * m_rtRange.Width()/ rtClient.Width(); 
	rtRange.top    = m_rect.top * m_rtRange.Height()  / rtClient.Height(); 
	rtRange.bottom = m_rect.bottom * m_rtRange.Height()/ rtClient.Height(); 
	return rtRange;
}
void CTPRectTracker::SetTrackRect(POINT point,RECT rt)
{
	CRect rtClient; 
	CRect rtRange = m_rect;
	GetClientRect(&rtClient);

	rtRange.left   = rt.left   *  rtClient.Width()/m_rtRange.Width(); 
	rtRange.right  = rt.right  *  rtClient.Width()/m_rtRange.Width(); 
	rtRange.top    = rt.top    *  rtClient.Height()/m_rtRange.Height(); 
	rtRange.bottom = rt.bottom *  rtClient.Height()/m_rtRange.Height(); 

	rtRange.OffsetRect(point.x *  rtClient.Width()/m_rtRange.Width(),point.y*rtClient.Height()/m_rtRange.Height());

	m_rect = rtRange;
	m_rtInit = m_rect;
	Invalidate();
}
void CTPRectTracker::SetRectRange(CRect rtRange)
{
	m_rtRange = rtRange;
}

void CTPRectTracker::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}

BOOL CTPRectTracker::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return __super::OnEraseBkgnd(pDC);
}
