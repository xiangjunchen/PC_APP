// TPRichEditCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPRichEditCtrl.h"
#include ".\tpricheditctrl.h"


// CTPRichEditCtrl

IMPLEMENT_DYNAMIC(CTPRichEditCtrl, CRichEditCtrl)
CTPRichEditCtrl::CTPRichEditCtrl()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uStyle        = 0;
	m_eBorderStyle  = g_eBorderStyle;	
}

CTPRichEditCtrl::~CTPRichEditCtrl()
{
	SubWindow(FALSE);
}


BEGIN_MESSAGE_MAP(CTPRichEditCtrl, CRichEditCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	//ON_WM_PAINT()
	ON_WM_NCPAINT()
//	ON_WM_NCCREATE()
END_MESSAGE_MAP()

void CTPRichEditCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	
		m_uStyle = CRichEditCtrl::GetStyle(); 
		TPScrollBarData *pData = TP_ReplaceWnd(this);
		if(pData)
		{
			((CTPScrollBar *) pData ->pHScroll) -> m_bSetScrollInfo = TRUE;
			((CTPScrollBar *) pData ->pVScroll) -> m_bSetScrollInfo = TRUE;
		}
	} 
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
		TP_UnReplaceWnd(this);
	}
}


// CTPRichEditCtrl 消息处理程序


int CTPRichEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}


BOOL CTPRichEditCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CRichEditCtrl::PreCreateWindow(cs);
}

void CTPRichEditCtrl::PreSubclassWindow()
{
	CRichEditCtrl::PreSubclassWindow();
	if(!m_bCreate)	SubWindow(TRUE);
}
void CTPRichEditCtrl::OnDestroy()
{
	SubWindow(FALSE);
	CRichEditCtrl::OnDestroy();
}

HBRUSH CTPRichEditCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC ->SetBkMode(TRANSPARENT);
	pDC ->SetBkColor(TP_GetSysColor(COLOR_BASE_WINDOW));
	if(GetFocus() == this)	 	 pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
	else if(!IsWindowEnabled())	 pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));
	else                   	     pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));	

	return TP_GetSysBrush(BRUSH_WINDOW);

}

BOOL CTPRichEditCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CRichEditCtrl::OnEraseBkgnd(pDC);
}

void CTPRichEditCtrl::OnPaint()
{
	CPaintDC dc(this);
}

void CTPRichEditCtrl::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}

