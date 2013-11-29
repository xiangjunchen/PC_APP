// TPTitleBar.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPTitleBar.h"
#include ".\tptitlebar.h"

// CTPTitleBar

IMPLEMENT_DYNAMIC(CTPTitleBar, CWnd)
CTPTitleBar::CTPTitleBar()
{
	m_bBaseWnd    = TRUE;
	m_uBoxAmount  = BOX_MIN;
	m_uState      = STATE_ACTIVATE;
	m_pMsgWnd     = NULL;

	m_pShowTips	  = NULL; //2006-03-14
}

CTPTitleBar::~CTPTitleBar()
{
}


BEGIN_MESSAGE_MAP(CTPTitleBar, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()													//2006-03-14
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0, 0xffff, OnToolTipNotify)	//2006-03-14
END_MESSAGE_MAP()



// CTPTitleBar 消息处理程序

int    CTPTitleBar::GetBarHeight()
{
	return m_pResources[BORDER_TOP_N]->Height();	
}
void CTPTitleBar::SetWindowText(LPCTSTR lpszString)
{
	m_sText = lpszString;
	Invalidate();
}

int CTPTitleBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_hBaseWnd = m_hWnd;
	SetSkin(TP_GetSkinPath() + _L("\\CommonCtrl\\Frame"));	
	FunSize();
	if(m_pMsgWnd == NULL) m_pMsgWnd = GetParent();

	// 2006-03-14
	m_pShowTips = new CTPToolTipCtrl();
	m_pShowTips->Create( this, TTS_ALWAYSTIP); 
	m_pShowTips->EnableToolTips( TRUE); 
	m_pShowTips->Activate( TRUE); 
	m_pShowTips->AddTool( this);
	EnableToolTips(TRUE);

	return 0;
}

void CTPTitleBar::OnDestroy() //2006-03-14
{
	if( m_pShowTips)
	{
		m_pShowTips ->DestroyToolTipCtrl();		
		m_pShowTips = NULL;
	}

	CWnd::OnDestroy();
}

BOOL CTPTitleBar::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult) //2005-03-14
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	if( !m_pShowTips) return FALSE;

	CPoint ptCursor;
	::GetCursorPos(&ptCursor); 
	ScreenToClient(&ptCursor);

	if( m_rtMinBox.PtInRect( ptCursor))
	{
		m_sToolTips = _T("minisize");
	}
	else if( m_rtMaxBox.PtInRect( ptCursor))
	{
		m_sToolTips = _T("maxsize");
	}
	else if( m_rtCloseBox.PtInRect( ptCursor))
	{
		m_sToolTips = _T("maxsize");
	}

	m_pShowTips ->m_sToolTips = m_sToolTips;
	return  TRUE;			
}

void CTPTitleBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnPaint()
	PaintFrameNS(&dc);
}

void CTPTitleBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_pShowTips) // by zhh on 2006-04-02
	{
		if( m_rtMinBox.PtInRect( point))
		{
			m_sToolTips = _T("minisize");
		}
		else if( m_rtMaxBox.PtInRect( point))
		{
			m_sToolTips = _T("maxsize");
		}
		else if( m_rtCloseBox.PtInRect( point))
		{
			m_sToolTips = _T("maxsize");
		}

		ClientToScreen( &point);
		point.x += 2;
		point.y += 2;
		m_pShowTips ->m_sToolTips = m_sToolTips;
		m_pShowTips ->Pop(); //?
	}

	__super::OnMouseMove(nFlags, point);
}

void CTPTitleBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_rtMinBox.PtInRect(point)) m_pMsgWnd ->PostMessage(WM_SYSCOMMAND,SC_MINIMIZE,NULL);
	__super::OnLButtonDown(nFlags, point);
}
