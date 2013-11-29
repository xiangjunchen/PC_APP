// TPWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPWnd.h"
#include ".\tpwnd.h"


// CTPWnd

IMPLEMENT_DYNAMIC(CTPWnd, CTabCtrl)
CTPWnd::CTPWnd()
{
}

CTPWnd::~CTPWnd()
{
}


BEGIN_MESSAGE_MAP(CTPWnd, CTabCtrl)
//	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

void     CTPWnd::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_TABCONTROL,&wndClass);		
		wndClass.lpszClassName = WC_WND;
		wndClass.style  	  |= CS_HREDRAW|CS_VREDRAW;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_WND,AfxGetInstanceHandle());
}

// CTPWnd 消息处理程序


//void CTPWnd::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: 在此处添加消息处理程序代码
//	// 不为绘图消息调用 CTabCtrl::OnPaint()
//}

LRESULT CTPWnd::OnNcHitTest(CPoint point)
{
	return HTCLIENT;
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CTabCtrl::OnNcHitTest(point);
}

LRESULT CTPWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::DefWindowProc(message, wParam, lParam);
}
BOOL CTPWnd::Create(DWORD dwStyle,const RECT &rtWnd,CWnd *pParent,UINT nID)
{
	return CWnd::Create(NULL,NULL,dwStyle,rtWnd,pParent,nID,NULL);
}