// TPCaseWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPCaseWnd.h"
#include ".\tpcasewnd.h"


// CTPCaseWnd

IMPLEMENT_DYNAMIC(CTPCaseWnd, CWnd)
CTPCaseWnd::CTPCaseWnd()
{
	m_bSubWindow   = FALSE;
	m_bCreate      = FALSE;
	m_uStyle       = 0;
	m_eBorderStyle = TP_GetBorderStyle();

	m_pSubWnd      = NULL;
	m_bAutoH       = FALSE;
	m_bAutoV       = FALSE;

	m_bSameH       = FALSE;
	m_bSameV       = FALSE;

	m_bAutoDel     = FALSE;

	m_pParentWnd   = NULL; 
	m_iBackColor   = COLOR_BASE_FRAME;

}

CTPCaseWnd::~CTPCaseWnd()
{
	SubWindow(FALSE);
}


BEGIN_MESSAGE_MAP(CTPCaseWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


void CTPCaseWnd::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	
		m_uStyle = CWnd::GetStyle(); 		
		CWnd::ShowScrollBar(SB_HORZ,FALSE);		
		CWnd::ShowScrollBar(SB_VERT,FALSE);
		TP_ReplaceWnd(this);
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
		TP_UnReplaceWnd(this);
	}
}
// CTPCaseWnd 消息处理程序


BOOL CTPCaseWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	m_bCreate = TRUE;
	return CWnd::PreCreateWindow(cs);
}

void CTPCaseWnd::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

int CTPCaseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CTPCaseWnd::OnDestroy()
{
	SubWindow(FALSE);

	if(m_pSubWnd && m_bAutoDel)
	{
		m_pSubWnd ->DestroyWindow();
		delete m_pSubWnd;
	}	
	else if(m_pSubWnd && m_pParentWnd)
	{
		m_pSubWnd ->SetParent(m_pParentWnd);
	}
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

BOOL CTPCaseWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void CTPCaseWnd::OnPaint()
{
	CPaintDC dc(this); 
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));
}

void CTPCaseWnd::OnNcPaint()
{
	CRect rtWnd,rtClient;
	GetWindowRect(&rtWnd);
	GetClientRect(&rtClient);
	ClientToScreen(&rtClient);
	rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
	rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
	if(rtWnd.bottom - rtClient.bottom >2)
	{
		CDC *pDC     = GetWindowDC();		
		CTPDCExchange stuTemp(pDC);
		pDC ->FillSolidRect(0,rtClient.right,rtWnd.right -rtClient.right,rtWnd.bottom -rtClient.bottom,TP_GetSysColor(m_iBackColor));		
		stuTemp.ReleaseDC();
		ReleaseDC(pDC);
	}

	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}

void CTPCaseWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);	
	if(m_pSubWnd && cy >0)
	{	
		CRect rtWnd,rtSub;
		GetWindowRect(&rtWnd);
		m_pSubWnd ->GetWindowRect(&rtSub);
		SCROLLINFO sInfo;
		sInfo.cbSize = sizeof(sizeof(SCROLLINFO));
		sInfo.fMask = SIF_RANGE | SIF_PAGE;
		sInfo.nPage = rtWnd.Height() - (m_bAutoH ? 20:0);
		sInfo.nMax  = rtSub.Height();
		sInfo.nMin  = 0; 
		if(m_bAutoV)
			CWnd::SetScrollInfo(SB_VERT,&sInfo,FALSE);
		sInfo.nPage = rtWnd.Width() - (m_bAutoV?20:0);
		sInfo.nMax  = rtSub.Width();
		if(m_bAutoH)
			CWnd::SetScrollInfo(SB_HORZ,&sInfo,TRUE);

		MoveSubWnd();
	}
}
void CTPCaseWnd::MoveSubWnd()
{
	if(m_pSubWnd)
	{	
		int iPosX = GetScrollPos(SB_HORZ);
		int iPosY = GetScrollPos(SB_VERT);
		CRect rtWnd,rtClient;
		GetClientRect(&rtClient);
		m_pSubWnd ->GetWindowRect(&rtWnd);
		ScreenToClient(&rtWnd);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
		rtWnd.OffsetRect(-iPosX,-iPosY);

		if(m_bSameH)
			rtWnd.right = rtWnd.left + rtClient.Width();
		if(m_bSameV)
			rtWnd.bottom = rtWnd.top + rtClient.Height();

		m_pSubWnd->MoveWindow(rtWnd);	
 

	}
}
void CTPCaseWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	if(pScrollBar)
	{
		((CTPScrollBar *)pScrollBar) ->SetScroll(nSBCode,nPos);
		MoveSubWnd();		
	}
}
void CTPCaseWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	if(pScrollBar )  
	{	 
		((CTPScrollBar *)pScrollBar) ->SetScroll(nSBCode,nPos);
		MoveSubWnd();
	}
}
void CTPCaseWnd::SetSubWnd(CWnd *pWnd,BOOL bAutoV,BOOL bAutoH,BOOL bAtuoDel,long lXOffset ,long lYOffset )
{
	m_pSubWnd      = pWnd;
	m_bAutoH       = bAutoH;
	m_bAutoV       = bAutoV;
	m_bAutoDel     = bAtuoDel;
	if(pWnd)
	{	
		m_pParentWnd   = pWnd ->GetParent();
		pWnd ->SetParent(this);
		CRect rtWnd,rtThis;
		pWnd ->GetWindowRect(&rtWnd);
		GetClientRect(&rtThis);		
		ScreenToClient(&rtWnd);
		if(m_bAutoH)
			CWnd::SetScrollRange(SB_HORZ,0,rtWnd.Width()  - rtThis.Width()  + 20+lXOffset);
		if(m_bAutoV)
			CWnd::SetScrollRange(SB_VERT,0,rtWnd.Height() - rtThis.Height() + 20+lYOffset);
	}
	else
	{
		 CWnd::ShowScrollBar(SB_HORZ,FALSE);
		 CWnd::ShowScrollBar(SB_VERT,FALSE);
	}
}
