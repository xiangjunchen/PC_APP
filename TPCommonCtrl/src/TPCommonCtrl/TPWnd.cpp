// TPWnd.cpp : ʵ���ļ�
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

// CTPWnd ��Ϣ�������


//void CTPWnd::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: �ڴ˴������Ϣ����������
//	// ��Ϊ��ͼ��Ϣ���� CTabCtrl::OnPaint()
//}

LRESULT CTPWnd::OnNcHitTest(CPoint point)
{
	return HTCLIENT;
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CTabCtrl::OnNcHitTest(point);
}

LRESULT CTPWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CWnd::DefWindowProc(message, wParam, lParam);
}
BOOL CTPWnd::Create(DWORD dwStyle,const RECT &rtWnd,CWnd *pParent,UINT nID)
{
	return CWnd::Create(NULL,NULL,dwStyle,rtWnd,pParent,nID,NULL);
}