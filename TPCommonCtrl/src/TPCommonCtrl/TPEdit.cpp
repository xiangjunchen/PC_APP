extern // TPEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPEdit.h"
#include <Imm.h>
#include ".\tpedit.h"

extern CTPMenuEx         *g_pMenuCreate;

// CTPEdit

IMPLEMENT_DYNAMIC(CTPEdit, CEdit)
CTPEdit::CTPEdit()
{
	m_bClientRect   = FALSE;
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uStyle        = 0;
	m_eBorderStyle  = g_eBorderStyle;
	m_dwWindowLevel = TPWNDLEVEL1;
	m_bForceFocus   = FALSE;
	m_uPrivateStyle = 0;
	m_uState        = 0;
	m_pScrollBar    = NULL;

}

CTPEdit::~CTPEdit()
{	
	ASSERT(m_bSubWindow ==0);
}
BEGIN_MESSAGE_MAP(CTPEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_UNSUBCLASSWINDOW,OnMsgUnSubWindow)
	ON_MESSAGE(WM_SCROLLBAR,OnMsgScollBar)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

void CTPEdit::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	
		m_uStyle     = CEdit::GetStyle();
		SaveFont(m_hWnd);		
		if(m_uStyle &(WS_HSCROLL | WS_VSCROLL ))
		{
			m_pScrollBar = TP_ReplaceWnd(this);
			if(m_uStyle &(ES_AUTOHSCROLL|ES_AUTOVSCROLL)) 
			{
				SCROLLINFO si;
				ZeroMemory(&si,sizeof(SCROLLINFO));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask  = SIF_PAGE;			
				si.nPage  = 10;
				if(m_uStyle &ES_AUTOHSCROLL) SetScrollInfo(SB_HORZ,&si);
				if(m_uStyle &ES_AUTOVSCROLL) SetScrollInfo(SB_VERT,&si);
			}
			
		}
		else 	SetFont(TP_GetSysFont(m_iFontIndex));
		if(m_uStyle & WS_EX_CLIENTEDGE)
			m_bClientRect = TRUE;
	}
	else
	{
		if(!m_bSubWindow) return;		
		m_bSubWindow = FALSE;	
		RestoreFont(m_hWnd);		
		if(m_uStyle &(WS_HSCROLL | WS_VSCROLL ))
			TP_UnReplaceWnd(this);
		m_pScrollBar = NULL;	
	}
}
int CTPEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}

BOOL CTPEdit::PreCreateWindow(CREATESTRUCT& cs)
{	
	m_bCreate = TRUE;
	return CEdit::PreCreateWindow(cs);
}

void CTPEdit::PreSubclassWindow()
{
	CEdit::PreSubclassWindow();	
	if(!m_bCreate)	SubWindow(TRUE);
}

void CTPEdit::OnDestroy()
{
	SubWindow(FALSE);
	CEdit::OnDestroy();
}

HBRUSH CTPEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{	

#ifdef  TP_NS_EDIT
	pDC ->SetTextColor(RGB(0,0,0));

	if(m_uStyle & ES_READONLY)   
	{		
		pDC ->SetBkColor(TP_GetSysColor(COLOR_BASE_EDITREDONLY));
		return TP_GetSysBrush(BRUSH_EDITREDONLY);	  
	}
	else if((GetFocus() == this || m_bForceFocus || m_pScrollBar && GetFocus() == m_pScrollBar ->pVScroll) && m_bForceFocus !=0x7fff)	
	{	
		pDC ->SetBkColor(TP_GetSysColor(COLOR_BASE_EDITFOCUS));
		return TP_GetSysBrush(BRUSH_EDITFOCUS);	  		
	}
	else
	{			
		pDC ->SetBkColor(TP_GetSysColor(COLOR_BASE_EDIT));		
		return TP_GetSysBrush(BRUSH_EDIT);	  		
	}
#else 
	if(GetFocus() == this)	 	 pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
	else if(!IsWindowEnabled())	 pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));
	else                   	     pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));	

	if(m_uStyle & ES_READONLY) pDC ->SetBkColor(TP_GetSysColor(COLOR_BASE_FRAME));
	else                       pDC ->SetBkColor(TP_GetSysColor(COLOR_BASE_WINDOW));		

	if(m_dwWindowLevel == TPWNDLEVEL1)
	{
		pDC ->SetBkColor(TP_GetSysColor(COLOR_BASE_WINDOW));
		return TP_GetSysBrush(BRUSH_WINDOW);
	}
	else
	{
		pDC ->SetBkColor(TP_GetSysColor(COLOR_BASE_WINDOW2));
		return TP_GetSysBrush(BRUSH_WINDOW2);
	}
#endif
	return NULL;
}


void CTPEdit::OnNcPaint()
{
	g_pRoundCorner ->m_iBackColor    = COLOR_BASE_WINDOW;
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}

void CTPEdit::OnPaint()
{
	CPaintDC dc(this); 
	CRect     rtClient;m_rtClient = rtClient;
	GetClientRect(&rtClient);	
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));	
	m_pfnSuper(m_hWnd,WM_PRINT,(WPARAM)cdcMemo.m_hDC,(LPARAM)(PRF_CLIENT|PRF_CHECKVISIBLE|PRF_ERASEBKGND));	 	
	if(m_bClientRect == 4) cdcMemo.Draw3dRect(rtClient,TP_GetSysColor(COLOR_BASE_3DLEFT),TP_GetSysColor(COLOR_BASE_3DLEFT));
	else if(m_bClientRect) cdcMemo.Draw3dRect(rtClient,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DLEFT));
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
}

void CTPEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	if(nChar == VK_RETURN &&  !TP_IsCtrlKeyDown() && !TP_IsShiftKeyDown() && !TP_IsAltKeyDown())
		GetParent() ->PostMessage(WM_TPEDIT,TPEDIT_RETURE,(LPARAM)m_hWnd);
}

LRESULT  CTPEdit::OnMsgScollBar(WPARAM wp,LPARAM lp)
{
	if(1 == wp)
	{
		InvalidateRect(NULL);
	}
	return S_OK;
}
LRESULT  CTPEdit::OnMsgUnSubWindow(WPARAM wp,LPARAM lp)
{
	SubWindow(FALSE);
	return S_OK;
}
int CTPEdit::GetInt()
{
	CString sText;
	GetWindowText(sText);
	return StrToInt(sText);
}
void CTPEdit::SetInt(int iSet)
{
	CString sText;
	sText.Format(_T("%d"),iSet);
	SetWindowText(sText);
}
extern CTPMenuEx *g_pPopMenu;
extern BOOL       g_bMenuExPopUp; 
extern HWND       g_hMenuCreate;
//extern void     (*g_pPopMenuOperate)(CTPMenuEx * pMenu);  
void TP_PopMenuOperateEdit(CTPMenuEx * pMenu)
{
	if(pMenu == NULL) return;
	int iPos = pMenu ->GetMenuItemPos(0x08001);
	if(iPos>=0)
	{
		pMenu ->DeleteMenu(iPos,MF_BYPOSITION);
		pMenu ->DeleteMenu(iPos,MF_BYPOSITION);
	}
}
void CTPEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{	
	CTPMenuEx *pMenu  = new CTPMenuEx();	
	g_pPopMenu        = pMenu;	
	g_bMenuExPopUp    = TRUE;
	//g_pPopMenuOperate = TP_PopMenuOperateEdit;
	CEdit::OnContextMenu(pWnd,point);		
	g_bMenuExPopUp    = FALSE;
	g_pPopMenu        = NULL;	
	//g_pPopMenuOperate = NULL;
	pMenu ->DestroyMenu();
	delete pMenu;		
}
HWND CTPEdit::UnsubclassWindow()
{
	SubWindow(FALSE);
	return CEdit::UnsubclassWindow();
}
BOOL CTPEdit::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	BOOL bRet = CWnd::ModifyStyle(dwRemove, dwAdd, nFlags);
	if (bRet) m_uStyle = GetStyle();
	return bRet;
}
void CTPEdit::PreTranslateFocus(UINT message, WPARAM wParam,LPARAM lParam)
{
	if(message == WM_COMMAND)
	{
		UINT nID = HIWORD(wParam);	
		if(nID == EN_SETFOCUS || nID == EN_KILLFOCUS || CBN_SETFOCUS == nID || CBN_KILLFOCUS == nID)	
			::InvalidateRect((HWND)lParam,NULL,FALSE);
	}
}

LRESULT CTPEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_COMMAND)
	{
		message = LOWORD(wParam);
	}
	else if(message == WM_CHAR && wParam == VK_RETURN && !(m_uStyle & ES_WANTRETURN))
		return S_OK;
	return __super::DefWindowProc(message, wParam, lParam);
}
void CTPEdit::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	g_pPopMenu ->SetMenuWndData(g_hMenuCreate,lpMeasureItemStruct->itemData);
	return g_pPopMenu ->MeasureItem(lpMeasureItemStruct);
	__super::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CTPEdit::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	g_hMenuCreate = NULL;	
	return g_pPopMenu ->DrawItem(lpDrawItemStruct);
	__super::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
