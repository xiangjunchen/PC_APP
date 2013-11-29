// TPFrameWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPFrameWnd.h"
#include ".\tpframewnd.h"

extern CString  g_sRegisterClass;
// CTPFrameWnd

IMPLEMENT_DYNCREATE(CTPFrameWnd, CMiniFrameWnd)

CTPFrameWnd::CTPFrameWnd()
   :CTPBaseFrame()
{	
	m_iBackColor             = COLOR_BASE_FRAME;
	m_pTranslateMessage      = NULL;
	m_pTranslateMessageParam = NULL;
	m_pAttachWnd             = NULL;
#ifdef TP_NS_EDIT
	m_iFontIndex  = FONT_BASE_TEXT3;
#endif
}

CTPFrameWnd::~CTPFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CTPFrameWnd, CWnd)
//	ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_NCACTIVATE()
ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CTPFrameWnd 消息处理程序

LRESULT CTPFrameWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CTPBaseFrame::m_hBaseWnd     = m_hWnd;
	CTPBaseFrame::m_pfnBaseSuper = m_pfnSuper; 

	if(g_pTaskBarCallBackFun)
	{
		switch(message)
		{
		case WM_SYSCOMMAND:
			{
				UINT nID = (UINT)wParam;
				if((nID & 0XFFF0) == SC_MINIMIZE)	
				{
					g_pTaskBarCallBackFun(this,message, wParam, lParam);
					LRESULT lr = CTPBaseFrame::DefWindowProc(message, wParam, lParam);
					ShowWindow(SW_HIDE);
					return lr;
				}
				else if((nID & 0XFFF0) == SC_MAXIMIZE)
				{
					ShowWindow(SW_SHOW);
				}
				else if((nID & 0XFFF0) == SC_RESTORE)
				{
					ShowWindow(SW_SHOW);
					g_pTaskBarCallBackFun(this,message, wParam, lParam);
				}
				else if((nID & 0XFFF0) == SC_CLOSE)
				{
					g_pTaskBarCallBackFun(this,message, wParam, lParam);
				}
			}
			break;
		case WM_SHOWWINDOW:
		case WM_CREATE:
		case WM_DESTROY:
		case WM_ACTIVATE:
		case WM_GETMINMAXINFO:
			g_pTaskBarCallBackFun(this,message, wParam, lParam);
			break;
		default:
			break;
		}
	}//}	 
	return CTPBaseFrame::DefWindowProc(message, wParam, lParam);		
}
BOOL CTPFrameWnd::Create(LPCTSTR lpClassName, LPCTSTR lpszWindowName,DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= WS_CLIPCHILDREN;
	if(lpClassName == NULL) lpClassName = g_sRegisterClass;
	return  __super::CreateEx(0,lpClassName,lpszWindowName,dwStyle,rect,pParentWnd,nID);	
}
BOOL CTPFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef TP_NS_EDIT
	if(CTPBaseFrame::PreTranslateMessageEx(pMsg)) return TRUE;	
#endif
	if(m_pTranslateMessage)
	{
		if(m_pTranslateMessage(pMsg,(LPARAM)m_pTranslateMessageParam)) return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CTPFrameWnd::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if(m_pAttachWnd) m_pAttachWnd ->MoveWindow(0,0,cx,cy);
}

INT_PTR CTPFrameWnd::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{	
#ifdef TP_NS_EDIT			
		pTI ->hwnd     = m_hWnd;	
		pTI ->lpszText = LPSTR_TEXTCALLBACK;
		pTI ->uId      = (UINT)(UINT_PTR)m_hWnd;
		pTI ->uFlags  |= TTF_IDISHWND;
		return  GetDlgCtrlID();		
#else
	return __super::OnToolHitTest(point, pTI);
#endif
}

BOOL CTPFrameWnd::OnNcActivate(BOOL bActive)
{
	return __super::OnNcActivate(bActive);
}
void CTPFrameWnd::OnSysCommand(UINT nID, LPARAM lParam)
{
	if((nID & 0xfff0) == SC_KEYMENU) return;
	CWnd::OnSysCommand(  nID,   lParam);
}