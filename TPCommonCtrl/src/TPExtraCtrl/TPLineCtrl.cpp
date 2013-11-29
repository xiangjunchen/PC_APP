// TPLineCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPLineCtrl.h"
#include ".\tplinectrl.h"


// CTPLineCtrl

IMPLEMENT_DYNAMIC(CTPLineCtrl, CTPWnd)
CTPLineCtrl::CTPLineCtrl()
{
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_eBorderStyle  = TP_GetBorderStyle();
	m_dwWindowLevel = TPWNDLEVEL1;	
	m_iBackColor    = COLOR_BASE_FRAME;

	m_iCount        = 1;
	m_iSpace        = 0;
	m_bVert         = FALSE;

}

CTPLineCtrl::~CTPLineCtrl()
{
	SubWindow(FALSE);
}


BEGIN_MESSAGE_MAP(CTPLineCtrl, CTPWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void     CTPLineCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle() ,WC_WND,&wndClass);				
		wndClass.lpszClassName = WC_LINECTRL;	
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_LINECTRL,AfxGetInstanceHandle());	
}
void CTPLineCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;

		if(m_dwWindowLevel == TPWNDLEVEL1)	m_iBackColor   = COLOR_BASE_FRAME;
		else	                        	m_iBackColor   = COLOR_BASE_FRAME2;
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;		
	}
}

// CTPLineCtrl 消息处理程序


BOOL CTPLineCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bCreate = TRUE;

	return CTPWnd::PreCreateWindow(cs);
}

void CTPLineCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CTPWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

int CTPLineCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_bVert = lpCreateStruct ->style & SBS_VERT;
	SubWindow(TRUE);
	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CTPLineCtrl::OnDestroy()
{
	SubWindow(FALSE);
	CTPWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CTPLineCtrl::OnPaint()
{
	CPaintDC dc(this); 
	CRect rtClient;
	GetClientRect(&rtClient);
	CTPMemoDC memDC(&dc,rtClient.Width(),rtClient.Height());
	memDC.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));

	int iRightColor = COLOR_BASE_3DRIGHT,iLeftColor = COLOR_BASE_3DLEFT;
	if(m_dwWindowLevel == TPWNDLEVEL2)
	{
		iRightColor = COLOR_BASE_3DRIGHT2;iLeftColor = COLOR_BASE_3DLEFT;
	}
	if(m_bVert)
	{
		if(m_iCount == 1)
		{
			memDC.Draw3dRect(rtClient.left,rtClient.top,2,rtClient.Height(),TP_GetSysColor(iRightColor),TP_GetSysColor(iLeftColor));
		}
		else
		{
			for(INT l=0;l<m_iCount;l++)
			{
				memDC.Draw3dRect(rtClient.left,rtClient.top + (m_iSpace + 2) * l,2,rtClient.Height(),TP_GetSysColor(iRightColor),TP_GetSysColor(iLeftColor));
			}
		}
	}
	else
	{
		if(m_iCount == 1)
		{
			memDC.Draw3dRect(rtClient.left,rtClient.CenterPoint().y,rtClient.Width(),2,TP_GetSysColor(iRightColor),TP_GetSysColor(iLeftColor));
		}
		else
		{
			for(INT l=0;l<m_iCount;l++)
			{
				memDC.Draw3dRect(rtClient.left,rtClient.CenterPoint().y + (m_iSpace + 2) * l,rtClient.Width(),2,TP_GetSysColor(iRightColor),TP_GetSysColor(iLeftColor));
			}
		}
	}

	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&memDC,0,0,SRCCOPY);
}

BOOL CTPLineCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE ;//CTPWnd::OnEraseBkgnd(pDC);
}
BOOL  CTPLineCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CTPWnd::Create(dwStyle,rect,pParentWnd,nID);
}
void CTPLineCtrl::SetCount(int iCount,int iSpace)
{
	m_iCount = iCount;
	m_iSpace = iSpace;
}