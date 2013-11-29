// TPComboLBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPComboLBox.h"
#include ".\tpcombolbox.h"


// CTPComboLBox

IMPLEMENT_DYNAMIC(CTPComboLBox, CTPListBox)
CTPComboLBox::CTPComboLBox()
{
	m_pMouseScrollBar = NULL;
	m_pScrollBar      = NULL;
	m_bSimple         = FALSE;
	m_pComboBoxEx     = NULL;
	m_uState         |= LB_SUBCOMBOBOX;
	m_bFocus          = TRUE;
	m_pToolTips       = NULL;
}

CTPComboLBox::~CTPComboLBox()
{
}


BEGIN_MESSAGE_MAP(CTPComboLBox, CTPListBox)
	ON_WM_NCPAINT()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_PRINT,OnPrint)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff,       OnToolTipNotify )
	ON_MESSAGE(WM_USER+11,OnShowScrollBar)
END_MESSAGE_MAP()
LRESULT CTPComboLBox::OnShowScrollBar(WPARAM wp,LPARAM lp)
{
	if(m_pScrollBar && m_pScrollBar ->pVScroll && m_pScrollBar ->bShowVScrollBar)
	{		 
		CRect rcWinSelf;
		GetWindowRect(&rcWinSelf);
 		rcWinSelf.DeflateRect(1,1,0,1);//remove border
		rcWinSelf.left = rcWinSelf.right - m_pScrollBar->iScrollWidth;
		::SetWindowPos(m_pScrollBar->pVScroll->GetSafeHwnd(), HWND_TOP, rcWinSelf.left-1, rcWinSelf.top, rcWinSelf.Width(), rcWinSelf.Height(),  SWP_NOACTIVATE);
		//TP_GetPop(GetParent());
  		m_pScrollBar ->pVScroll->ShowWindow(SW_SHOW);	
	}
	return S_OK;
}
LRESULT CTPComboLBox::OnPrint(WPARAM wp,LPARAM lp)
{
	if(m_bForceNoDraw) return S_OK;
 	BOOL bDrawFrame = (lp & PRF_NONCLIENT) == PRF_NONCLIENT;			
	if(bDrawFrame && !m_bSimple)
	{
		CDC  *pDC = CDC::FromHandle((HDC)wp);		
		CRect rtWnd,rtClient;
		GetWindowRect(&rtWnd);
		GetClientRect(&rtClient);
		ClientToScreen(&rtClient);
		rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);	
		pDC ->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));			 		
		m_pfnSuper(m_hWnd,WM_PRINT,wp,lp);	
		if(rtClient.Width() < rtWnd.Width())
			pDC ->FillSolidRect(rtClient.right,rtWnd.top,rtWnd.right - rtClient.right,rtWnd.bottom - rtClient.top,TP_GetSysColor(COLOR_BASE_FRAME));			 
		pDC->Draw3dRect(rtWnd,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DRIGHT));
		return S_OK;
	}	
	else	return  DefWindowProc(WM_PRINT,wp,lp);		
}

void CTPComboLBox::OnNcLButtonDown(UINT nHitTest, CPoint point)
{	
	CPoint ptTest = point;
	if(m_pScrollBar)
	{
		CRect rtWnd;
		if(m_pScrollBar ->bShowHScrollBar)
		{
			m_pScrollBar ->pHScroll ->GetWindowRect(&rtWnd);
			if(rtWnd.PtInRect(ptTest))
			{
				m_pScrollBar ->pHScroll ->ScreenToClient(&ptTest);
				m_pScrollBar ->pHScroll ->SendMessage(WM_LBUTTONDOWN,nHitTest,MAKELONG(ptTest.x,ptTest.y));
				m_pMouseScrollBar = m_pScrollBar ->pHScroll;		
				return;
			}
		}
		if(m_pScrollBar ->bShowVScrollBar)
		{
			m_pScrollBar ->pVScroll ->GetWindowRect(&rtWnd);
			if(rtWnd.PtInRect(ptTest))
			{
				m_pScrollBar ->pVScroll ->ScreenToClient(&ptTest);
				m_pScrollBar ->pVScroll ->SendMessage(WM_LBUTTONDOWN,nHitTest,MAKELONG(ptTest.x,ptTest.y));
				m_pMouseScrollBar = m_pScrollBar ->pVScroll;		
				return;
			}
		}
	}
	else CTPListBox::OnNcLButtonDown(nHitTest, point);
}

void CTPComboLBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint ptTest = point;
	ClientToScreen(&ptTest);
	if(m_pMouseScrollBar)
	{	
		m_pMouseScrollBar ->ScreenToClient(&ptTest);
		m_pMouseScrollBar ->SendMessage(WM_MOUSEMOVE,nFlags,MAKELONG(ptTest.x,ptTest.y));
		return;
	}
	else if(m_pScrollBar)
	{
		CRect rtWnd;
		if(m_pScrollBar ->bShowHScrollBar)
		{
			m_pScrollBar ->pHScroll ->GetWindowRect(&rtWnd);
			if(rtWnd.PtInRect(ptTest))
			{
				m_pScrollBar ->pHScroll ->ScreenToClient(&ptTest);
				m_pScrollBar ->pHScroll ->SendMessage(WM_MOUSEMOVE,nFlags,MAKELONG(ptTest.x,ptTest.y));
				return;
			}
		}
		if(m_pScrollBar ->bShowVScrollBar)
		{
			m_pScrollBar ->pVScroll ->GetWindowRect(&rtWnd);
			if(rtWnd.PtInRect(ptTest))
			{
				m_pScrollBar ->pVScroll ->ScreenToClient(&ptTest);
				m_pScrollBar ->pVScroll ->SendMessage(WM_MOUSEMOVE,nFlags,MAKELONG(ptTest.x,ptTest.y));
				return;
			}
		}
	}
	CTPListBox::OnMouseMove(nFlags, point);
}

void CTPComboLBox::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint ptTest = point;
	ClientToScreen(&ptTest);
	if(m_pMouseScrollBar)
	{	
		m_pMouseScrollBar ->ScreenToClient(&ptTest);
		m_pMouseScrollBar ->SendMessage(WM_NCLBUTTONUP,nHitTest,MAKELONG(ptTest.x,ptTest.y));				
	}
	else CTPListBox::OnNcLButtonUp(nHitTest, point);
}

void CTPComboLBox::OnLButtonUp(UINT nFlags, CPoint point)
{	
	CPoint ptTest = point;
	ClientToScreen(&ptTest);
	BOOL bOut  = TRUE;
	int iIndex = ItemFromPoint(point,bOut);
	if(iIndex >=0 && iIndex <GetCount()){CString sText;GetText(iIndex,sText);if(sText.Find(_T("--------"))>=0) return;}
	if(m_pMouseScrollBar)
	{
		m_pMouseScrollBar ->ScreenToClient(&ptTest);
		m_pMouseScrollBar ->SendMessage(WM_LBUTTONUP,0,MAKELONG(ptTest.x,ptTest.y));				
		m_pMouseScrollBar = FALSE;	
	}
	else CTPListBox::OnLButtonUp(nFlags, point);
}

void CTPComboLBox::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	CTPListBox::PreSubclassWindow();	
	if(m_pScrollBar && !m_bSimple)
	{
		((CTPScrollBar *)m_pScrollBar ->pHScroll) ->m_bSetCapture = FALSE;
		((CTPScrollBar *)m_pScrollBar ->pVScroll) ->m_bSetCapture = FALSE;
	
		m_pScrollBar ->iScrollWidth     = ::GetSystemMetrics(SM_CXVSCROLL);
		m_pScrollBar ->iScrollHeight    = ::GetSystemMetrics(SM_CYHSCROLL);

	}
}
void CTPComboLBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(m_pComboBoxEx)
		((CTPComboBoxEx *)m_pComboBoxEx) ->PaintItem(lpDrawItemStruct);
	else CTPListBox::DrawItem(lpDrawItemStruct);
}
namespace _tp_commonctrl
{
extern void inline TP_CalCWindowPos( TPScrollBarData *pScrollBarData,BOOL bHShow,BOOL bVShow);
}
void CTPComboLBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if(bShow && m_pScrollBar && m_pScrollBar ->pVScroll && m_pScrollBar ->bShowVScrollBar)
		PostMessage(WM_USER+11,0,0);
	m_uBaseState &= ~TP_BASESTATE_SELCHANGE;
	CTPListBox::OnShowWindow(bShow, nStatus);
}


BOOL CTPComboLBox::PreTranslateMessage(MSG* pMsg)
{
	if(m_pToolTips) m_pToolTips ->RelayEvent(pMsg);
	if(pMsg ->message == WM_NCRBUTTONUP || pMsg ->message == WM_RBUTTONUP)
	{		
		return TRUE;
	}
	return CTPListBox::PreTranslateMessage(pMsg);
}
BOOL CTPComboLBox::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	if(m_uBaseState & TP_BASESTATE_ENABLETOOLTIP)
	{	
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;		
		CString  sToolTips;
		int iSel = GetCurSel();
		if(iSel>=0) GetText(iSel,sToolTips);		
		lstrcpyn( pTTT->szText,sToolTips,80);	
		if(m_pToolTips)
			m_pToolTips ->m_sToolTips = sToolTips;
		return  TRUE;			
	}
	else return FALSE;
}

void CTPComboLBox::EnableToolTips(BOOL bEnable)
{
	if(bEnable)
	{	
		m_uBaseState |= 	TP_BASESTATE_ENABLETOOLTIP;
		if(m_pToolTips == NULL)
		{				
			m_pToolTips = new CTPToolTipCtrl();		
			m_pToolTips->Create(this,TTS_ALWAYSTIP); 
			m_pToolTips->EnableToolTips(TRUE); 
			m_pToolTips->AddTool(this,LPSTR_TEXTCALLBACK,CRect(0,0,0,0),20000);
			m_pToolTips->Activate(TRUE); 

			CWnd::EnableToolTips(TRUE);
		}
	}
	else
	{
		m_uBaseState &= 	~TP_BASESTATE_ENABLETOOLTIP;
		if(m_pToolTips)
		{
			m_pToolTips->DestroyWindow();
			delete m_pToolTips;
			m_pToolTips = NULL;
			CWnd::EnableToolTips(FALSE);	
		}
	}
}

