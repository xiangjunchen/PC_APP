
// TPSpinButtonCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPSpinButtonCtrl.h"

enum SPINCTRL_INDEX
{
	SPINCTRL_1_NOR,
	SPINCTRL_1_DOWN,
	SPINCTRL_1_OVER,
	SPINCTRL_1_DIS,
	SPINCTRL_2_NOR,
	SPINCTRL_2_DOWN,
	SPINCTRL_2_OVER,
	SPINCTRL_2_DIS,
};
// CTPSpinButtonCtr
#define TP_SPINACC   0x10000000

IMPLEMENT_DYNAMIC(CTPSpinButtonCtrl, CSpinButtonCtrl)
CTPSpinButtonCtrl::CTPSpinButtonCtrl()
{
	m_dwWindowLevel   = TPWNDLEVEL1; 
	m_bSubWindow      = FALSE;
	m_bCreate         = FALSE; 
	m_uStyle          = 0;
	m_eBorderStyle    = g_eBorderStyle;

	m_iOver           = 0;
	m_iPressed        = 0;
	m_uTimer          = 0; 

	m_rtSpinButton[0] = CRect(0,0,0,0);
	m_rtSpinButton[1] = CRect(0,0,0,0);

	for(int l=0;l<8;l++)
		m_pButtonBmp[l] = NULL;

	m_pMsgDelPosWnd    = NULL;
	m_sSkinPath        = _T("");
	m_bForceBud        = TRUE; 
	m_iAccrCount       = 0;

}

CTPSpinButtonCtrl::~CTPSpinButtonCtrl()
{
	for(int l=0;l<8;l++)
		TP_ReleaseCommonRes(m_pButtonBmp[l]);
}


BEGIN_MESSAGE_MAP(CTPSpinButtonCtrl, CSpinButtonCtrl)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()

	ON_WM_TIMER()
	//	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CTPSpinButtonCtrl::LoadButtonBmp()
{
	CString s1Text,s2Text;
	if(m_uStyle & UDS_HORZ) 
	{
		s1Text = _L("Left");
		s2Text = _L("Right");
	}
	else
	{
		s1Text = _L("Top");
		s2Text = _L("Bottom");
	}
	if(m_sSkinPath.GetLength()<=0) m_sSkinPath = g_sSysSkinPath + _L("\\CommonCtrl\\SpinButtonCtrl");
	m_pButtonBmp[SPINCTRL_1_NOR]  = TP_LoadBmpByte(m_sSkinPath + _L("\\Spin_") + s1Text + _L("_Nor.Bmp"));
	m_pButtonBmp[SPINCTRL_1_DOWN] = TP_LoadBmpByte(m_sSkinPath + _L("\\Spin_") + s1Text + _L("_Down.Bmp"));
	m_pButtonBmp[SPINCTRL_1_OVER] = TP_LoadBmpByte(m_sSkinPath + _L("\\Spin_") + s1Text + _L("_Over.Bmp"));
	m_pButtonBmp[SPINCTRL_1_DIS]  = TP_LoadBmpByte(m_sSkinPath + _L("\\Spin_") + s1Text + _L("_Dis.Bmp"));

	m_pButtonBmp[SPINCTRL_2_NOR]  = TP_LoadBmpByte(m_sSkinPath + _L("\\Spin_") + s2Text + _L("_Nor.Bmp"));
	m_pButtonBmp[SPINCTRL_2_DOWN] = TP_LoadBmpByte(m_sSkinPath + _L("\\Spin_") + s2Text + _L("_Down.Bmp"));
	m_pButtonBmp[SPINCTRL_2_OVER] = TP_LoadBmpByte(m_sSkinPath + _L("\\Spin_") + s2Text + _L("_Over.Bmp"));
	m_pButtonBmp[SPINCTRL_2_DIS]  = TP_LoadBmpByte(m_sSkinPath + _L("\\Spin_") + s2Text + _L("_Dis.Bmp"));	

}
void CTPSpinButtonCtrl::SetSkinPath(CString sSkin)
{
	m_sSkinPath = sSkin;
}
void CTPSpinButtonCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		m_uStyle     = GetStyle();
		LoadButtonBmp();		

		m_pMsgDelPosWnd = GetBuddy();

	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;		
		if(m_uTimer) KillTimer(m_uTimer);
		m_uTimer = 0;
		KillTimer(1);
	}
}

BOOL CTPSpinButtonCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bCreate = TRUE;

	return CSpinButtonCtrl::PreCreateWindow(cs);
}

void CTPSpinButtonCtrl::PreSubclassWindow()
{
	CSpinButtonCtrl::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

void CTPSpinButtonCtrl::OnDestroy()
{
	SubWindow(FALSE);
	CSpinButtonCtrl::OnDestroy();
}

int CTPSpinButtonCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSpinButtonCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}
void CTPSpinButtonCtrl::EnableAcclet(BOOL bEnable)
{
	if(bEnable)	m_uStyle |= TP_SPINACC;
	else 	    m_uStyle &= ~TP_SPINACC;
}
void CTPSpinButtonCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{		
	if(m_rtSpinButton[0].PtInRect(point))      m_iPressed = 1;
	else if(m_rtSpinButton[1].PtInRect(point)) m_iPressed = 2;
	else                                       m_iPressed = 0;	
	if(m_pMsgDelPosWnd)
	{
		m_iAccrCount = 0;
		if(m_uStyle & TP_SPINACC) SetTimer(1,500,NULL);
		m_pMsgDelPosWnd ->SendMessage(WM_UDNDELTAPOS,m_iPressed,0);
	}
	CSpinButtonCtrl::OnLButtonDown(nFlags, point);
}

void CTPSpinButtonCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{	
	CSpinButtonCtrl::OnLButtonUp(nFlags, point);	
	if(m_pMsgDelPosWnd)
	{
		KillTimer(1);
		m_pMsgDelPosWnd ->SendMessage(WM_UDNDELTAPOS,m_iPressed,1);
	}
	m_iPressed = 0;
	if(m_iOver != 0) 
	{
		m_iOver = 0;
		Invalidate(FALSE);
	}
}

void CTPSpinButtonCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int iOver = -1;
	if(m_rtSpinButton[0].PtInRect(point))      iOver = 1;
	else if(m_rtSpinButton[1].PtInRect(point)) iOver = 2;
	else                                       iOver = 0;
	if(m_iOver != iOver) 
	{
		m_iOver = iOver;
		Invalidate(FALSE);
		if(m_uTimer ==0)
		{
			m_uTimer = SetTimer(200,100,NULL);
		}
	}
	CSpinButtonCtrl::OnMouseMove(nFlags, point);
}

void CTPSpinButtonCtrl::OnPaint()
{
	CPaintDC dc(this); 	
	CRect     rtClient,rtTemp;
	GetClientRect(&rtClient);
	BOOL bBuddy = GetBuddy() != NULL && m_bForceBud;
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());
	if(!bBuddy)	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_FRAME)); 
	else        cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW)); 
	rtTemp = rtClient;
	if(bBuddy && m_uStyle & (UDS_ALIGNLEFT |UDS_ALIGNRIGHT))	rtTemp.DeflateRect(2,2);
	if(m_uStyle & UDS_HORZ) 
	{
		m_rtSpinButton[0] = rtTemp;
		m_rtSpinButton[1] = rtTemp;
		m_rtSpinButton[0].right = rtTemp.CenterPoint().x;
		m_rtSpinButton[1].left  = rtTemp.CenterPoint().x ;
	}
	else
	{
		m_rtSpinButton[0] = rtTemp;
		m_rtSpinButton[1] = rtTemp;
		m_rtSpinButton[0].bottom = rtTemp.CenterPoint().y;
		m_rtSpinButton[1].top    = rtTemp.CenterPoint().y;
	}

	int iImage = 0;
	if(m_iOver == 1)
	{
		if(!IsWindowEnabled())  iImage = SPINCTRL_1_DIS;
		if(m_iPressed == 1)     iImage = SPINCTRL_1_DOWN;
		else                    iImage = SPINCTRL_1_OVER;
	}
	else iImage = SPINCTRL_1_NOR;

	TP_StretchDIBits(&cdcMemo,m_pButtonBmp[iImage] ->GetByte(),m_pButtonBmp[iImage]->Width(),m_pButtonBmp[iImage]->Height(),m_rtSpinButton[0]);

	if(m_iOver == 2)
	{
		if(!IsWindowEnabled())  iImage = SPINCTRL_2_DIS;
		if(m_iPressed == 2)     iImage = SPINCTRL_2_DOWN;
		else                    iImage = SPINCTRL_2_OVER;
	}
	else iImage = SPINCTRL_2_NOR;

	TP_StretchDIBits(&cdcMemo,m_pButtonBmp[iImage] ->GetByte(),m_pButtonBmp[iImage]->Width(),m_pButtonBmp[iImage]->Height(),m_rtSpinButton[1]);

	if(bBuddy)
	{
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		if(m_uStyle & UDS_ALIGNRIGHT) 	g_pRoundCorner ->PaintRect(m_eBorderStyle,&cdcMemo,rtClient,2,CORNER_LEFT);
		else if(m_uStyle & CORNER_LEFT) g_pRoundCorner ->PaintRect(m_eBorderStyle,&cdcMemo,rtClient,2,CORNER_RIGHT);
	}

	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	

}
void CTPSpinButtonCtrl::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{			
		if(m_iOver == m_iPressed && m_iPressed !=0)
		{
			m_iAccrCount ++;
			if(m_iAccrCount>10)
			{			
				if(m_pMsgDelPosWnd)
					m_pMsgDelPosWnd ->SendMessage(WM_UDNDELTAPOS,m_iPressed,2);
			}
		}		
		if(GetKeyState(VK_LBUTTON)>=0)
			KillTimer(1);
	}
	else if(nIDEvent == 200)
	{
		CRect rtClient;
		GetClientRect(&rtClient);
		CPoint ptCursor;
		GetCursorPos(&ptCursor); 
		ScreenToClient(&ptCursor);
		if(!rtClient.PtInRect(ptCursor) && GetKeyState(VK_LBUTTON)>=0) 
		{
			KillTimer(m_uTimer);
			m_uTimer = 0;
			m_iOver = 0;
			Invalidate();
		}
		return ;
	}
	// jyn changed 2007.10.31
	CSpinButtonCtrl::OnTimer(nIDEvent);
}
BOOL CTPSpinButtonCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CSpinButtonCtrl::OnEraseBkgnd(pDC);
}
HWND CTPSpinButtonCtrl::UnsubclassWindow()
{
	SubWindow(FALSE);
	return CSpinButtonCtrl::UnsubclassWindow();
}