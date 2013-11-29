// TPSplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPSplitterBar.h"
#include ".\tpsplitterbar.h"

// CTPSplitterBar

IMPLEMENT_DYNAMIC(CTPSplitterBar, CWnd)

CTPSplitterBar::CTPSplitterBar()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bHorz     = FALSE;
	m_bDrag     = FALSE;
	m_ptDrag    = CPoint(0, 0);
	m_rtDragBar = CRect(0, 0, 0, 0);

	m_bSubWindow = FALSE;
	m_bCreate    = FALSE;	

	m_eBorderStyle = TP_GetBorderStyle();
	
	m_pResCursor = NULL;
	m_ptRange    = CPoint(0,0);
	m_iLineType  = 0;;
}

CTPSplitterBar::~CTPSplitterBar()
{
	TP_ReleaseCommonRes(m_pResCursor);
	SubWindow(FALSE);
}

BEGIN_MESSAGE_MAP(CTPSplitterBar, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

// CWnd 消息处理程序
int CTPSplitterBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SubWindow(TRUE);

	return 0;
}
BOOL  CTPSplitterBar::Create(BOOL bHorz, UINT nStyle, RECT &rtWnd, CWnd *pParent, UINT nID)
{
	m_bHorz = bHorz;

	return CWnd::Create(NULL, NULL, nStyle, rtWnd, pParent, nID);
}
void CTPSplitterBar::OnPaint()
{
	CPaintDC dc(this); 
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_FRAME));	
	if(m_iLineType == 1)
	{
		CPen *pOldPen;
		CPen pen1(PS_SOLID, 1, TP_GetSysColor(COLOR_BASE_3DRIGHT));
		CPen pen2(PS_SOLID, 1, TP_GetSysColor(COLOR_BASE_3DLEFT));
		if (!m_bHorz)
		{
			pOldPen = dc.SelectObject(&pen1);
			dc.MoveTo(rtClient.CenterPoint().x,rtClient.top);
			dc.LineTo(rtClient.CenterPoint().x,rtClient.bottom);
			dc.SelectObject(&pen2);
			dc.MoveTo(rtClient.CenterPoint().x+1,rtClient.top);
			dc.LineTo(rtClient.CenterPoint().x+1,rtClient.bottom);
			dc.SelectObject(pOldPen);
		}
		else
		{
			pOldPen = dc.SelectObject(&pen1);
			dc.MoveTo(rtClient.left, rtClient.CenterPoint().y);
			dc.LineTo(rtClient.right, rtClient.CenterPoint().y);
			dc.SelectObject(&pen2);
			dc.MoveTo(rtClient.left, rtClient.CenterPoint().y+1);
			dc.LineTo(rtClient.right, rtClient.CenterPoint().y+1);
			dc.SelectObject(pOldPen);
		}

	}
}

void CTPSplitterBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDrag      = TRUE;
	m_ptDrag     = point;
	m_ptDragSave = point;
	m_rtDragBar  = CRect(0, 0, 0, 0);
	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

void CTPSplitterBar::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if (m_bDrag)
	{	
		GetParent() ->ModifyStyle(WS_CLIPCHILDREN, 0, 0);

		CWnd *pParent = GetParent();
		CDC  *pDC     = pParent ->GetDC();
		CRect rtTemp = m_rtDragBar;
		ClientToScreen(&rtTemp);
		pParent ->ScreenToClient(&rtTemp);
		pDC->InvertRect(rtTemp);
		pParent ->ReleaseDC(pDC);

		GetParent() ->ModifyStyle(0,WS_CLIPCHILDREN, 0);

		ReleaseCapture();		
		if (m_ptDragSave.x != point.x && !m_bHorz || m_ptDragSave.y != point.y && m_bHorz)
		{
			CRect rtWnd;
			GetParent() ->GetWindowRect(&rtWnd);			
			ClientToScreen(&point);

			if (!m_bHorz)	
			{
				point.x = max(point.x,rtWnd.left +m_ptRange.x);
				point.x = min(point.x,rtWnd.right-m_ptRange.y);
			}
			else
			{
				point.y = max(point.y,rtWnd.top   +m_ptRange.x);
				point.y = min(point.y,rtWnd.bottom-m_ptRange.y);
			}
			ScreenToClient(&point);

			ClientToScreen(&point);
			GetParent() ->SendMessage(WM_SPLITTERMOVE, MAKELONG(point.x, point.y), (LPARAM)this);
		}	
		m_bDrag = FALSE;
		m_rtDragBar = CRect(0, 0, 0, 0);
		GetParent() ->Invalidate();

		Invalidate();
	}

	CWnd::OnLButtonUp(nFlags, point);
}
void CTPSplitterBar::SetMoveRange(INT iMaxValue, INT iMinValue)
{
 	m_ptRange.x = iMinValue;
 	m_ptRange.y = iMaxValue;
}
void CTPSplitterBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDrag)
	{
		if (m_ptDrag.y != point.y && m_bHorz || m_ptDrag.x != point.x && !m_bHorz)
		{
			GetParent() ->ModifyStyle(WS_CLIPCHILDREN, 0, 0);
			m_ptDrag = point;
			CRect rtWnd;
			GetParent() ->GetWindowRect(&rtWnd);			
			ClientToScreen(&point);

			if (!m_bHorz)	
			{
				point.x = max(point.x,rtWnd.left +m_ptRange.x);
				point.x = min(point.x,rtWnd.right-m_ptRange.y);
			}
			else
			{
				point.y = max(point.y,rtWnd.top   +m_ptRange.x);
				point.y = min(point.y,rtWnd.bottom-m_ptRange.y);
			}
			
			ScreenToClient(&point);
			
			CWnd *pParent = GetParent();
			CDC  *pDC     = pParent ->GetDC();

			if (m_rtDragBar.Width() == 0)
			{
				GetClientRect(m_rtDragBar);
			}
			else
			{
				CRect rtTemp = m_rtDragBar;
				ClientToScreen(&rtTemp);
				pParent->ScreenToClient(&rtTemp);
				pDC->InvertRect(rtTemp);
			}

			CRect rtTemp = m_rtDragBar;

			if (!m_bHorz)	
			{
				rtTemp.left  = point.x - m_rtDragBar.Width()/2;
				rtTemp.right = rtTemp.left + m_rtDragBar.Width();
			}
			else
			{
				rtTemp.top    = point.y - m_rtDragBar.Height()/2;
				rtTemp.bottom = rtTemp.top + m_rtDragBar.Height();
			}

			m_rtDragBar = rtTemp;
			ClientToScreen(&rtTemp);
			pParent ->ScreenToClient(&rtTemp);
			pDC->InvertRect(rtTemp); // InvertRect:在某一矩形区域内反显现有颜色
			pParent ->ReleaseDC(pDC);

			GetParent() ->ModifyStyle(0,WS_CLIPCHILDREN, 0);

		}
	}
	else
	{
		CWnd::OnMouseMove(nFlags, point);
	}
}

BOOL CTPSplitterBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{	
	if (m_pResCursor)	
	{
		SetCursor(m_pResCursor->GetCursor());
		return TRUE;
	}
	else
	{
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}
}

void CTPSplitterBar::SetDirection(BOOL bHorz)
{
	TP_ReleaseCommonRes(m_pResCursor);
	m_bHorz = bHorz;

	if (!m_bHorz)
		m_pResCursor = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\Cursorwe.cur"));
	else 
		m_pResCursor = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\Cursorns.cur"));
}

void CTPSplitterBar::OnDestroy()
{
	SubWindow(FALSE);
	CWnd::OnDestroy();
}

BOOL CTPSplitterBar::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;

	return CWnd::PreCreateWindow(cs);
}

void CTPSplitterBar::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	if (!m_bCreate) 
		SubWindow(TRUE);
}

//registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function.
void CTPSplitterBar::RegisterClass(BOOL bRegister)
{
	if (bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(), WC_STATIC, &wndClass);		
		wndClass.lpszClassName = WC_SPLITTERBAR;
		::RegisterClass(&wndClass);
	}
	else
	{
		UnregisterClass(WC_SPLITTERBAR, AfxGetInstanceHandle());
	}
}

void CTPSplitterBar::SubWindow(BOOL bSub)
{
	if (bSub)
	{	
		if (m_bSubWindow) 
			return;

		m_bSubWindow = TRUE;
		TP_ReleaseCommonRes(m_pResCursor);

		if (!m_bHorz) 
#ifdef TP_NS_EDIT // li hui li 2007-09-19
			m_pResCursor = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\Cursorwe_ns.cur"));
#else
			m_pResCursor = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\Cursorwe.cur"));
#endif
		else 
#ifdef TP_NS_EDIT // li hui li 2007-09-19
			m_pResCursor = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\Cursorns_ns.cur"));
#else
			m_pResCursor = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\Cursorns.cur"));
#endif
	}
	else
	{
		if (!m_bSubWindow)
			return;

		m_bSubWindow = FALSE;	
	}
}

LRESULT CTPSplitterBar::OnNcHitTest(CPoint point)
{
	return HTCLIENT;
}

void CTPSplitterBar::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle, this);
	// 不为绘图消息调用 CWnd::OnNcPaint()
}
