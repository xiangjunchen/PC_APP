// TPPopMsgWnd.cpp : implementation file
//

#include "stdafx.h"
 
#include "TPPopMsgWnd.h"

CWnd   *CTPPopMsgWnd::m_pMsgSneWnd = NULL;
// CTPPopMsgWnd
IMPLEMENT_DYNAMIC(CTPPopMsgWnd, CWnd)

CTPPopMsgWnd::CTPPopMsgWnd()
{
	m_dwTimeToLive = 0;
	m_nIncrement   = 0;
	m_nWndHeight   = 0;
	m_nWndWidth	   = 0;
	m_nAlpha	   = 0;
	m_nStartPosX   = 0;
	m_nStartPosY   = 0;
	m_nCurPosX     = 0;
	m_nCurPosY     = 0;
	m_nWndState    = 0;
	m_sMessage     = _T("");
 	m_rtCloseBox   = CRect(0,0,0,0);
 	m_rtWnd        = CRect(0,0,0,0);
	m_sTitle       = _T("XpriEdit Message");
	m_bEnable      = TRUE;
}

CTPPopMsgWnd::~CTPPopMsgWnd()
{
	 
}

BEGIN_MESSAGE_MAP(CTPPopMsgWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ENABLE()
END_MESSAGE_MAP()

namespace _tp_extractrl
{
// CTPPopMsgWnd message handlers
CArray <CTPPopMsgWnd *,CTPPopMsgWnd *&> g_aMsgWnd;

CPoint                                  g_ptInitPos      = CPoint(0,0); //任务栏起市位置
long                                    g_lInitTime      = 200000;        //停留时间
DWORD                                   g_dwDirect        = FALSE;        //自动增长的方向
BOOL                                    g_bSingleMsg     = TRUE;
TPCommonRes                            *g_pPosMsgRes[2]  = {NULL,NULL}; 
CWnd*									g_pParentWnd     = NULL;
void TP_LoadPosMsgRes(BOOL bLoad)
{
	if(bLoad && g_pPosMsgRes[0] == NULL)
	{
		CString sSkinPath = TP_GetSkinPath();
		if(g_pPosMsgRes[0] == NULL)  g_pPosMsgRes[0] = TP_LoadBmpByte(sSkinPath + _L("\\Commontest\\media.bmp"));
		//if(g_pPosMsgRes[0] == NULL)  g_pPosMsgRes[0] = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\PopMsgWnd\\Popmsgbackground.bmp"));
		if(g_pPosMsgRes[1] == NULL)  g_pPosMsgRes[1] = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\PopMsgWnd\\close.bmp"));
	}
	else if(!bLoad)
	{
		if(g_pPosMsgRes[0]) TP_ReleaseCommonRes(g_pPosMsgRes[0]);g_pPosMsgRes[0] = NULL;
		if(g_pPosMsgRes[1]) TP_ReleaseCommonRes(g_pPosMsgRes[1]);g_pPosMsgRes[1] = NULL;
	}

}
void TP_SetPosMessageState(CPoint ptLeftTop,long lTime,DWORD dwDirect, CWnd* pParent, BOOL bSingle )
{
	g_ptInitPos    = ptLeftTop;
	if(lTime>0)
		g_lInitTime    = lTime;

	g_dwDirect   = dwDirect;
	g_bSingleMsg = bSingle;
	g_pParentWnd = pParent;
}

CPoint TP_GetPosMessagePos(DWORD dwDirect)
{
	CPoint ptStart = g_ptInitPos;
	INT nSize = g_aMsgWnd.GetSize();
	for(INT l=0; l < nSize; l++)
	{
		if(NULL==g_aMsgWnd[l]) continue;///应该 根据 m_rtWnd 来计算 
		if(TP_POPDIRECT_UP == dwDirect)
		{
			if(ptStart.y > g_aMsgWnd[l]->m_rtWnd.top) ptStart.y = g_aMsgWnd[l]->m_rtWnd.top - 5;					
		}
		else if(TP_POPDIRECT_DOWN == dwDirect)
		{
			if(ptStart.y < g_aMsgWnd[l]->m_rtWnd.bottom) ptStart.y = g_aMsgWnd[l]->m_rtWnd.bottom + 5;					
		}
		else if(TP_POPDIRECT_LEFT == dwDirect)
		{
			if(ptStart.x < g_aMsgWnd[l]->m_rtWnd.left) ptStart.y = g_aMsgWnd[l]->m_rtWnd.left - 5;					
		}
		else if(TP_POPDIRECT_RIGHT == dwDirect)
		{
			if(ptStart.y < g_aMsgWnd[l]->m_rtWnd.right) ptStart.y = g_aMsgWnd[l]->m_rtWnd.right + 5;					
		}
	}
	ptStart.x -= g_pPosMsgRes[0]->Width();
	return ptStart;
}

void TP_ShowPosMessage(LPCTSTR sMessage,CTPPopMsgWnd::tag_Poster *pUser,CPoint ptLeftTop,long lTime)
{

	TP_LoadPosMsgRes(TRUE);
	//有可能消息窗口已经存在
// 	if(pUser)
// 	{
// 		for(int l= 0;l<g_aMsgWnd.GetSize();l++)//重复使用以前曾经已经出来的窗口
// 		{
// 			if(g_aMsgWnd[l]->m_stuUser.sIP == pUser ->sIP )
// 			{
// 				g_aMsgWnd[l]->m_stuUser  = *pUser ;
// 				g_aMsgWnd[l]->Show(  sMessage);//重新设置消息
// 				return  ;
// 			}
// 		}
// 	}

	{
		if(lTime < 0)                        lTime      = g_lInitTime;
		if(ptLeftTop.x * ptLeftTop.y == 0)	 ptLeftTop  = TP_GetPosMessagePos(pUser->dwDirect); 

		CTPBaseFrame::m_uBaseFlag |= TP_BASESTATE_NOACTWND;
		//	HWND  hActWnd = ::GetActiveWindow();

		CTPPopMsgWnd* pPopMsgWnd = NULL;
		pPopMsgWnd = new CTPPopMsgWnd;
		if(!(pPopMsgWnd->Create(WS_CHILD,g_pParentWnd,0)))
		{	
			delete pPopMsgWnd;
			pPopMsgWnd = NULL;
			return;
		}
		if(pUser)	pPopMsgWnd->m_stuUser  = *pUser ;

		pPopMsgWnd->Show(sMessage, ptLeftTop, lTime);
		g_aMsgWnd.Add(pPopMsgWnd);

		//	if(::IsWindow(hActWnd)) ::SendMessage(hActWnd,WM_ACTIVATE,WA_ACTIVE, 0);
		CTPBaseFrame::m_uBaseFlag &= ~TP_BASESTATE_NOACTWND;
	}
}

void TP_ClosePosMessage(CTPPopMsgWnd* pPopMsgWnd)
{
	for(INT l=0;l<g_aMsgWnd.GetSize();l++)
	{
		if(g_aMsgWnd[l] == pPopMsgWnd)
		{
			g_aMsgWnd.RemoveAt(l);
			break;
		}
	}
	if(NULL != pPopMsgWnd)
	{
		pPopMsgWnd ->DestroyWindow();
		delete pPopMsgWnd;
		pPopMsgWnd = NULL;
	}
	if(g_aMsgWnd.GetSize()<=0)
		TP_LoadPosMsgRes(FALSE);
}

void TP_ClosePosMessage(CTPPopMsgWnd::tag_Poster *pUser)
{
	if(pUser  == NULL)
	{	
		for(INT l=0;l<g_aMsgWnd.GetSize();l++)
		{
			if(NULL != g_aMsgWnd[l])
			{
				g_aMsgWnd[l]  ->DestroyWindow();
				delete g_aMsgWnd[l];
				g_aMsgWnd[l] = NULL;	 
			}
		}
		g_aMsgWnd.RemoveAll();
	}
	else
	{
		for(INT l= g_aMsgWnd.GetSize()-1;l>=0;l--)
		{
			if(NULL != g_aMsgWnd[l] && (g_aMsgWnd[l] ->m_stuUser.sTitle == pUser ->sTitle || pUser ->sTitle == _T("")) && (g_aMsgWnd[l] ->m_stuUser.uState & pUser ->uState)!=0) 
			{
				g_aMsgWnd[l]  ->DestroyWindow();
				delete g_aMsgWnd[l];
				g_aMsgWnd[l] = NULL;
				g_aMsgWnd.RemoveAt(l);
			}
		}
	}
	if(g_aMsgWnd.GetSize()<=0)
		TP_LoadPosMsgRes(FALSE);
}
void TP_CloseAllPosMessage()
{
	for(INT l=0;l<g_aMsgWnd.GetSize();l++)
	{
		if(NULL != g_aMsgWnd[l])
		{
			g_aMsgWnd[l]  ->DestroyWindow();
			delete g_aMsgWnd[l];
			g_aMsgWnd[l] = NULL;	 
		}
	}
	g_aMsgWnd.RemoveAll();
	if(g_aMsgWnd.GetSize()<=0)
		TP_LoadPosMsgRes(FALSE);
}
};
int CTPPopMsgWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	LoadRes();
	return 0;
}

void CTPPopMsgWnd::LoadRes()
{
	m_nWndWidth   = g_pPosMsgRes[0]->Width();
	m_nWndHeight  = g_pPosMsgRes[0]->Height();
  
	m_nAlpha	   = 80; //控件起始透明度
	m_nIncrement   = 50; //移动增量
	m_rtCloseBox   = CRect(m_nWndWidth - 20,8,m_nWndWidth - 20 + g_pPosMsgRes[1]->Width() ,8+g_pPosMsgRes[1]->Height()); //背景图片上close的坐标
}

BOOL CTPPopMsgWnd::Create(DWORD uStyle,CWnd *pWnd,UINT nID)
{
	//uStyle |= WS_POPUP;
	uStyle &= ~WS_VISIBLE;
 	return  CWnd::CreateEx(/*WS_EX_LAYERED | WS_EX_TOPMOST*/ NULL,TP_GetRegisterClass(),NULL,uStyle  ,0,0,0,0,pWnd->m_hWnd,NULL);
}
void CTPPopMsgWnd::OnDestroy()
{
	CWnd::OnDestroy();
}

void	 CTPPopMsgWnd::CalcPopPos()
{
	if(TP_POPDIRECT_UP == g_dwDirect)
	{
		m_nCurPosY -= m_nIncrement;
	}
	else if(TP_POPDIRECT_DOWN == g_dwDirect)
	{
		m_nCurPosY += m_nIncrement;
	}
	else if(TP_POPDIRECT_LEFT == g_dwDirect)
	{
		m_nCurPosX -= m_nIncrement;
	}
	else if(TP_POPDIRECT_RIGHT == g_dwDirect)
	{
		m_nCurPosX += m_nIncrement;
	}
//  	m_nIncrement+=40;
//  	if(m_nIncrement > 150)	m_nIncrement = 150;
	return ;
}
void CTPPopMsgWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TP_TASKBARPOPWND_APPEARING:
		{
			CalcPopPos();
			if(abs(m_nCurPosY - m_nStartPosY) >= m_nWndHeight)	
			{
				KillTimer(TP_TASKBARPOPWND_APPEARING);
				SetTimer(TP_TASKBARPOPWND_SHOW,50,NULL);
			}
			if(abs(m_nCurPosX - m_nStartPosX) >= m_nWndWidth/2)	
			{
				KillTimer(TP_TASKBARPOPWND_APPEARING);
				SetTimer(TP_TASKBARPOPWND_SHOW,50,NULL);
			}
			m_nAlpha += m_nIncrement;
			if(m_nAlpha <=255)		SetLayeredWindowAttributes(0,m_nAlpha,2); 

			m_nWndState = TP_TASKBARPOPWND_APPEARING;
			SetWindowPos(NULL,m_nCurPosX,m_nCurPosY,m_nWndWidth,m_nWndHeight,SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;

	case TP_TASKBARPOPWND_SHOW:
		{
			m_nWndState = TP_TASKBARPOPWND_SHOW;
			KillTimer(TP_TASKBARPOPWND_SHOW);
			SetTimer(TP_TASKBARPOPWND_DISAPPEARING,m_dwTimeToLive,NULL);
		}
		break;

	case TP_TASKBARPOPWND_DISAPPEARING:
		{
			m_nWndState = TP_TASKBARPOPWND_DISAPPEARING;
			KillTimer(TP_TASKBARPOPWND_DISAPPEARING);
			ShowWindow(SW_HIDE);
			TP_ClosePosMessage(this);
			return ;
		}
		break;
	}

	CWnd::OnTimer(nIDEvent);
}
void CTPPopMsgWnd::Show(LPCTSTR sMessage)
{
	m_sMessage     = sMessage;
	if(m_nWndState == TP_TASKBARPOPWND_APPEARING)   {}//当前正在现实
	else  if(m_nWndState == TP_TASKBARPOPWND_SHOW) 
	{
		KillTimer(TP_TASKBARPOPWND_DISAPPEARING);	 
		m_nWndState = TP_TASKBARPOPWND_SHOW;
		SetTimer(TP_TASKBARPOPWND_DISAPPEARING,m_dwTimeToLive,NULL);
	}
	Invalidate();
	if(!m_bEnable) EnableWindow(TRUE);
}
void CTPPopMsgWnd::Show(LPCTSTR sMessage, CPoint ptLeftTop, DWORD dwTimeToLive)
{
	m_sMessage     = sMessage;
	m_dwTimeToLive = dwTimeToLive;
	m_nStartPosX   = ptLeftTop.x;
	m_nStartPosY   = ptLeftTop.y;
	m_nCurPosX     = m_nStartPosX;
	m_nCurPosY	   = m_nStartPosY;
	m_rtWnd        = CRect(m_nCurPosX,m_nCurPosY,m_nCurPosX+m_nWndWidth,m_nCurPosY+m_nWndHeight);
	if(m_stuUser.sTitle.GetLength()>0)
		m_sTitle       = m_stuUser.sTitle; 
	if(!m_bEnable) EnableWindow(TRUE);
	SetWindowPos(NULL,m_nCurPosX,m_nCurPosY,m_nWndWidth,m_nWndHeight,SWP_NOZORDER | SWP_NOACTIVATE|SWP_SHOWWINDOW);
	m_nWndState = TP_TASKBARPOPWND_APPEARING;
	SetTimer(TP_TASKBARPOPWND_APPEARING,100,NULL);
}

void CTPPopMsgWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	PaintDC(&dc);
}

void CTPPopMsgWnd::PaintDC(CDC *pDC)
{
	CRect rtClient, rtTitle, rtMessage,rtText = CRect(0,0,0,0);
	GetClientRect(&rtClient);
	rtTitle       = rtClient;
	rtMessage     = rtClient;
	rtTitle.top   = rtClient.top + 5;
	rtTitle.left  = rtClient.left + 5;
	rtMessage.top = rtTitle.top + 30;

	CTPMemoDC memDC(pDC,rtClient.Width(),rtClient.Height());	 
	CFont *pSaveFont =  memDC.SelectObject(TP_GetSysFont(FONT_BASE_TEXT));	
	memDC.SetBkMode(TRANSPARENT);
	TP_StretchDIBits(&memDC,g_pPosMsgRes[0]->GetByte(),g_pPosMsgRes[0]->Width(),g_pPosMsgRes[0]->Height(),rtClient);
	//TP_SetDIBitsToDevice(&memDC,g_pPosMsgRes[1],m_rtCloseBox);
	memDC.SetTextColor(TP_GetSysColor(COLOR_BASE_3DLEFT | COLOR_BASE_TEXTHIGHLIGHT));
	memDC.DrawText(m_sTitle, rtTitle, DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_END_ELLIPSIS);
	memDC.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
	memDC.DrawText(m_sMessage,rtText,DT_CALCRECT);  
	rtMessage.DeflateRect((rtMessage.Width()-rtText.Width())/2,0);
	memDC.DrawText(m_sMessage, rtMessage, DT_LEFT | DT_VCENTER |   DT_END_ELLIPSIS);
	pDC->BitBlt(0,0,m_nWndWidth,m_nWndHeight,&memDC,0,0,SRCCOPY);
	if(pSaveFont) memDC.SelectObject(pSaveFont);
}

void CTPPopMsgWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT t_MouseEvent;
	t_MouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
	t_MouseEvent.dwFlags     = TME_LEAVE | TME_HOVER;
	t_MouseEvent.hwndTrack   = m_hWnd;
	t_MouseEvent.dwHoverTime = 1;
	::_TrackMouseEvent(&t_MouseEvent);
	CWnd::OnMouseMove(nFlags, point);
}

void CTPPopMsgWnd::OnMouseHover(UINT nFlags, CPoint point)
{
	if (TP_TASKBARPOPWND_SHOW == m_nWndState)
	{
		KillTimer(TP_TASKBARPOPWND_DISAPPEARING);	 
	}
	CWnd::OnMouseHover(nFlags, point);
}

void CTPPopMsgWnd::OnMouseLeave()
{
	if (TP_TASKBARPOPWND_SHOW == m_nWndState)
	{
		SetTimer(TP_TASKBARPOPWND_SHOW,m_dwTimeToLive,NULL);
	}
	CWnd::OnMouseLeave();
}

BOOL CTPPopMsgWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (HTCLIENT == nHitTest && TP_TASKBARPOPWND_SHOW == m_nWndState)
	{
		CRect rtScreenClose = m_rtCloseBox;
		ClientToScreen(rtScreenClose);
		CPoint point;
		GetCursorPos(&point);		
		if(rtScreenClose.PtInRect(point))
		{
			::SetCursor(TP_GetCtrlCursor());
			return TRUE;
		}
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CTPPopMsgWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
	if((TP_TASKBARPOPWND_SHOW == m_nWndState) && m_rtCloseBox.PtInRect(point))
	{
		SetTimer(TP_TASKBARPOPWND_DISAPPEARING,10,NULL);
	}
	else  if((m_stuUser.uState  & 0x00000004) && m_stuUser.sTitle .GetLength()>4)
	{
		OnLButtonDblClk(0,CPoint(0,0));
	}
}

int CTPPopMsgWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_NOACTIVATE;
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
void CTPPopMsgWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if((m_stuUser.uState  & 0x00000004) && m_stuUser.sTitle .GetLength()>4)//消息窗口
	{
// 		tag_TPUser  stuUser;
// 
// 		stuUser.sUser        = m_stuUser.sUser; 
// 		stuUser.sComputer    = m_stuUser.sComputer;
// 		stuUser.sIP          = m_stuUser.sIP;
// 		stuUser.sMessage     = m_stuUser.sMessage;
// 
// 		stuUser.lParam1      = m_stuUser.lParam1;	 
// 		stuUser.lParam2      = 7;	 
// 
// 		if(m_pMsgSneWnd)
// 			m_pMsgSneWnd ->SendMessage(WM_TEAMUSER,TPTeamMsg_MessageString,(LPARAM)&stuUser);
		TP_ClosePosMessage(this);
		return;
	}
	else PostMessage(WM_CLOSE,0,0);
	CWnd::OnLButtonDblClk(nFlags, point);
}
void  CTPPopMsgWnd::OnEnable(BOOL bEnable)
{
	m_bEnable = bEnable;
}