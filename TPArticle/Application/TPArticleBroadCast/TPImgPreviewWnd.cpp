// TPImgPreviewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TPImgPreviewWnd.h"


// CTPImgPreviewWnd
#define WM_PREVIEWIMGMESSAGE WM_USER + 2400

#define TP_IMGPREVIEW_PREVIEW 0x0001

UINT TP_PreViewImgThread( LPVOID pParam ,BOOL *pbRun,CTPBaseThread *pThread);

TPCommonRes  *CTPImgPreviewWnd::m_pCommonRes[TP_IMGPREVIEWICON_MAX] = {NULL, NULL};

IMPLEMENT_DYNAMIC(CTPImgPreviewWnd, CWnd)

CTPImgPreviewWnd::CTPImgPreviewWnd()
{
	m_szDesWndSize = CSize(0,0);
	m_rtCloseBox = CRect(0,0,0,0);
	m_pMsgWnd   = NULL;
	m_ptStartPos = CPoint(0,0);
	m_ptCurPos   = CPoint(0,0);
	m_szSrcWndSize  = CSize(0,0);
	m_szIncrementSize = CSize(0,0);
}

CTPImgPreviewWnd::~CTPImgPreviewWnd()
{
}


BEGIN_MESSAGE_MAP(CTPImgPreviewWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_MESSAGE(WM_PREVIEWIMGMESSAGE,OnMessagePreviewImg)	
END_MESSAGE_MAP()



// CTPImgPreviewWnd message handlers


BOOL   CTPImgPreviewWnd::Create(DWORD uStyle, CWnd *pWnd, UINT nID)
{
	uStyle |= WS_POPUP;
	uStyle &= ~WS_VISIBLE;
	m_pMsgWnd = pWnd;
	return  CWnd::CreateEx(WS_EX_LAYERED /*| WS_EX_TOPMOST*/ ,TP_GetRegisterClass(),NULL,uStyle  ,0,0,230,120,pWnd->m_hWnd,NULL);//WS_EX_LAYERED 如果要设置透明再加这个属性
}

int CTPImgPreviewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CString sSkinPath = TP_GetSkinPath();
	m_pCommonRes[TP_IMGPREVIEWICON_CLOSE] = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\PopMsgWnd\\close.bmp"));
	m_rtCloseBox = CRect(m_szDesWndSize.cx - 20,8,m_szDesWndSize.cx - 20 + m_pCommonRes[TP_IMGPREVIEWICON_CLOSE]->Width() ,8+m_pCommonRes[TP_IMGPREVIEWICON_CLOSE]->Height()); //背景图片上close的坐标

	CenterWindow();
	return 0;
}

void CTPImgPreviewWnd::OnDestroy()
{
	m_stuUpdateThread.EndThread();
	for(INT l=0;l<TP_IMGPREVIEWICON_MAX;l++) 
	{
		TP_ReleaseCommonRes(m_pCommonRes[l]);
		m_pCommonRes[l] = NULL;
	}

	CWnd::OnDestroy();
	// TODO: Add your message handler code here
}

void CTPImgPreviewWnd::OnPaint()
{
	CPaintDC dc(this); 
	CRect rtClient,rtTitle;
	GetClientRect(&rtClient);
	rtTitle = rtClient;
	rtTitle.top += 5;

	CTPMemoDC memDC(&dc,rtClient.Width(),rtClient.Height());
	memDC.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BLACK));//RoundRect圆角矩形
	//CFont* pOldFont = memDC.SelectObject(m_pMsgWnd->m_pFont);
//	memDC.DrawText(_T("图片预览"),rtClient,DT_CENTER|DT_SINGLELINE);
	memDC.SetTextColor(TP_GetSysColor(COLOR_WHITE));
	memDC.DrawText(_T("图片预览"), rtTitle,  DT_CENTER | DT_TOP|DT_SINGLELINE | DT_END_ELLIPSIS);
//	TP_SetDIBitsToDevice(&memDC,m_pCommonRes[TP_IMGPREVIEWICON_CLOSE],m_rtCloseBox);
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&memDC,0,0,SRCCOPY);
}

void CTPImgPreviewWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnLButtonDown(nFlags, point);
	if(!m_rtCloseBox.PtInRect(point))
	{
		PostMessage (WM_NCLBUTTONDOWN , HTCAPTION , MAKELPARAM (point.x, point.y) );
	}
}

void CTPImgPreviewWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnLButtonUp(nFlags, point);
	if(m_rtCloseBox.PtInRect(point))
	{
		ShowWindow(SW_HIDE);
	}
}

void CTPImgPreviewWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TP_IMGPREVIEW_PREVIEW:
		{
			CRect rtMoveWindow = CRect(0,0,0,0);
			if(!GetMoveWindowRect(rtMoveWindow))
			{
				KillTimer(TP_IMGPREVIEW_PREVIEW);
			}
			SetWindowPos(NULL,rtMoveWindow.left,rtMoveWindow.top,rtMoveWindow.Width(),rtMoveWindow.Height(),SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
	CWnd::OnTimer(nIDEvent);
}

UINT TP_PreViewImgThread( LPVOID pParam ,BOOL *pbRun,CTPBaseThread *pThread)
{
	HANDLE hEvent   = ::CreateEvent(0,NULL,NULL,0);
	int nCount = 0;
	while (*pbRun)
	{
		Sleep(500);
		if(nCount++ < 6)	continue;
		//loadimgres
		CTPImgPreviewWnd *pImgPreview = (CTPImgPreviewWnd *)(CTPImgPreviewWnd*)(DWORD_PTR)pParam;
		::PostMessage(pImgPreview->GetSafeHwnd(),WM_PREVIEWIMGMESSAGE,0/*(WPARAM)&stuRfresh*/,(LPARAM)hEvent);
		::WaitForSingleObject(hEvent,INFINITE);
		break;
	}
	return 1;
}
BOOL CTPImgPreviewWnd::PreviewImg()
{
	if(m_stuUpdateThread.IsRun())
		m_stuUpdateThread.EndThread();
	m_stuUpdateThread.BeginThread(TP_PreViewImgThread,this);
	return TRUE;
}
BOOL CTPImgPreviewWnd::GetMoveWindowRect(CRect &rtWnd)
{
	INT nWndWidth = 0, nWndHeight = 0, nCurPosX = 0, nCurPosY = 0;
	m_ptCurPos.x += m_szIncrementSize.cx;
	m_ptCurPos.y += m_szIncrementSize.cy;
	if(m_ptCurPos.y >= (m_szDesWndSize.cy - m_szSrcWndSize.cy)/2)	
		m_ptCurPos.y = (m_szDesWndSize.cy - m_szSrcWndSize.cy)/2;
	if(m_ptCurPos.x >= (m_szDesWndSize.cx - m_szSrcWndSize.cx)/2)	
		m_ptCurPos.x = (m_szDesWndSize.cx - m_szSrcWndSize.cx)/2;
	nCurPosX = m_ptStartPos.x - m_ptCurPos.x;
	nCurPosY = m_ptStartPos.y - m_ptCurPos.y;
	nWndWidth =  m_szSrcWndSize.cx+2*m_ptCurPos.x;
	nWndHeight = m_szSrcWndSize.cy+2*m_ptCurPos.y;
	if(nWndWidth >= m_szDesWndSize.cx)		
		nWndWidth = m_szDesWndSize.cx;
	if(nWndHeight >= m_szDesWndSize.cy)	
		nWndHeight = m_szDesWndSize.cy;

	rtWnd.left = nCurPosX;
	rtWnd.top = nCurPosY;
	rtWnd.right = nCurPosX + nWndWidth;
	rtWnd.bottom = nCurPosY + nWndHeight;
	if(nWndWidth >= m_szDesWndSize.cx && nWndHeight >= m_szDesWndSize.cy)
	{
		return FALSE;
	}
	return TRUE;
}	

LRESULT CTPImgPreviewWnd::OnMessagePreviewImg(WPARAM wp,LPARAM lp)
{
	CRect rtWindow;
	GetWindowRect(rtWindow);
	m_ptStartPos   = CPoint(rtWindow.left,rtWindow.top);
	m_szDesWndSize = CSize(300,500);
	m_szSrcWndSize = CSize(rtWindow.Width(),rtWindow.Height());
	m_szIncrementSize = CSize(10,40);

	SetTimer(TP_IMGPREVIEW_PREVIEW,50,NULL);
	SetEvent((HANDLE)lp);
	return S_OK;
}
