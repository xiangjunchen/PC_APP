//

#include "stdafx.h"
#include "TPScrollBar.h"
#include ".\tpscrollbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DRAG_SCROLL_NONE          0
#define DRAG_SCROLL_LEFT          1
#define DRAG_SCROLL_RIGHT         2
#define DRAG_SCROLL_MIDDLE        3
#define DRAG_SCROLL_MIDDLEOK      4
#define DRAG_SCROLL_MIDDLELEFT    5
#define DRAG_SCROLL_MIDDLERIGHT   6
#define DRAG_SCROLL_TOP           7
#define DRAG_SCROLL_BOTTOM        8
#define DRAG_SCROLL_MIDDLETOP     10
#define DRAG_SCROLL_MIDDLEBOTTOM  11

#define PRESSS_CROLL_ECHO_TIME		100
#define DRAGPENDING					2//(*PRESSS_CROLL_ECHO_TIME = 200)
#define TP_DIS                      0

enum    TP_SCROLLBAR_RES
{
	SCROLLBAR_TL_DOWN,
	SCROLLBAR_TL_NOR,
	SCROLLBAR_TL_OVER,
	SCROLLBAR_TL_DIS,

	SCROLLBAR_THUMB_DOWN,
	SCROLLBAR_THUMB_NOR,
	SCROLLBAR_THUMB_OVER,
	SCROLLBAR_THUMB_DIS,

	SCROLLBAR_T_DOWN,
	SCROLLBAR_T_NOR,
	SCROLLBAR_T_OVER,
	SCROLLBAR_T_DIS,

	SCROLLBAR_RB_DOWN,
	SCROLLBAR_RB_NOR,
	SCROLLBAR_RB_OVER,
	SCROLLBAR_RB_DIS,

	SCROLLBAR_BACK,
};

enum TL_RES_INDEX
{
	TL_RES_ZOOMLEFTDOWNV = 20,
	TL_RES_ZOOMLEFTUPV,
	TL_RES_ZOOMRIGHTDOWNV,
	TL_RES_ZOOMRIGHTUPV,
};

#define MOUSE_DELAY 0 
#define TP_SCROLLBAR_SETPAGE 0x00000001

CTPScrollBar::CTPScrollBar()
{
	m_bSetCapture    = TRUE;
	m_bSetScrollInfo = 	FALSE;
	m_pMsgWnd        = NULL;      
	m_bAttachBar     = FALSE;

	m_nMinVisual    = 0;        
	m_nMaxVisual    = 100;        
	m_iPageSize     = 100;         
	m_iCurPos       = 0;           
	m_iOldPos       = 0;           
	m_iDragPos      = 0;          

	m_iSetTimer    = 0;        
	m_hTimerHover  = 0;     
	m_hTimerAcce   = 0;
	m_bVert        = FALSE;       
	m_bHover       = FALSE;
	m_nArrowFlags  = ESB_ENABLE_BOTH;

	m_rtLeftTopArrow     = CRect(0,0,0,0);     
	m_rtRightBottomArrow = CRect(0,0,0,0); 
	m_rtScrolBar         = CRect(0,0,0,0);         
	m_rtClient           = CRect(0,0,0,0);           
	m_iBarPage           = 10;           


	m_iDragMode       = 0;          
	m_ptDragSave      = CPoint(0,0);         
	m_iDragLeftSave   = 0;      
	m_dwTimeTickSave  = 0;   
	m_uState          = TP_SCROLLBAR_SETPAGE;

	for(int l=0;l<TP_SCROLLBAR_MAX;l++)	
		m_aBmpres[l] = NULL;        


	m_bSubWindow  = FALSE;
	m_bCreate     = FALSE;
	m_bReDraw     = TRUE;
	m_ptDrag      = CPoint(0,0);
	m_uScrollMode = 0;
	m_iScollLine  = -1;


	m_pResBar     = NULL;
	m_pResBarBack = NULL;
	m_bTLFace     = NULL;


}
CTPScrollBar::~CTPScrollBar()
{
	for(int l=0;l<TP_SCROLLBAR_MAX;l++)
	{
		TP_ReleaseCommonRes(m_aBmpres[l]);
		m_aBmpres[l] = NULL;        
	}
	TP_ReleaseCommonRes(m_pResBar);     m_pResBar     = NULL;
	TP_ReleaseCommonRes(m_pResBarBack); m_pResBarBack = NULL;

	SubWindow(FALSE);
}


BEGIN_MESSAGE_MAP(CTPScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CTPScrollBar)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	//	ON_WM_SHOWWINDOW()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()


void CTPScrollBar::LoadThumb()
{
#ifdef TP_MINI_EDIT
	if(m_uStyle & SBS_VERT)	
	{
		m_aBmpres[SCROLLBAR_TL_DOWN]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_Top_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_NOR]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_Top_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_OVER]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_Top_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_DIS]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_Top_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_DOWN] = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_V_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_NOR]  = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_V_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_OVER] = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_V_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_DIS]  = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_V_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_DOWN]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_Bottom_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_NOR]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_Bottom_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_OVER]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_Bottom_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_DIS]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_Bottom_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_BACK]       = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_V_BACK.Bmp")); 
	}
	else  
	{
		m_aBmpres[SCROLLBAR_TL_DOWN]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_left_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_NOR]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_left_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_OVER]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_left_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_DIS]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_left_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_DOWN] = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_H_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_NOR]  = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_H_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_OVER] = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_H_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_DIS]  = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_H_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_DOWN]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_right_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_NOR]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_right_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_OVER]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_right_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_DIS]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_right_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_BACK]       = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\MiniScrollBar\\SCROLLBAR_H_BACK.Bmp")); 		
	}
#else
	if(m_uStyle & SBS_VERT)	
	{
		m_aBmpres[SCROLLBAR_TL_DOWN]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_Top_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_NOR]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_Top_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_OVER]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_Top_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_DIS]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_Top_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_DOWN] = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_V_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_NOR]  = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_V_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_OVER] = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_V_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_DIS]  = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_V_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_T_DOWN]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\scrollbar_t_V_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_T_NOR]      = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\scrollbar_t_V_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_T_OVER]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\scrollbar_t_V_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_T_DIS]      = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\scrollbar_t_V_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_DOWN]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_Bottom_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_NOR]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_Bottom_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_OVER]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_Bottom_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_DIS]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_Bottom_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_BACK]       = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_V_BACK.Bmp")); 
	}
	else  
	{
		m_aBmpres[SCROLLBAR_TL_DOWN]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_left_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_NOR]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_left_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_OVER]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_left_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_TL_DIS]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_left_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_DOWN] = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_H_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_NOR]  = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_H_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_OVER] = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_H_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_THUMB_DIS]  = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_H_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_T_DOWN]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\scrollbar_t_h_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_T_NOR]      = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\scrollbar_t_h_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_T_OVER]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\scrollbar_t_h_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_T_DIS]      = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\scrollbar_t_h_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_DOWN]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_right_DOWN.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_NOR]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_right_Nor.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_OVER]    = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_right_Over.Bmp")); 
		m_aBmpres[SCROLLBAR_RB_DIS]     = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_right_Dis.Bmp")); 
		m_aBmpres[SCROLLBAR_BACK]       = TP_LoadBmpByte(g_sSysSkinPath +  _L("\\CommonCtrl\\ScrollBar\\SCROLLBAR_H_BACK.Bmp")); 		
	}
#endif
}

void CTPScrollBar::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		m_uStyle = ::GetWindowLong(m_hWnd,GWL_STYLE);
		m_bVert     = BOOL((m_uStyle & SBS_VERT) ==  SBS_VERT);
		LoadThumb();
		if(m_pMsgWnd == NULL)  m_pMsgWnd = GetParent();		
		SetScrollRange(m_nMinVisual,m_nMaxVisual,FALSE);	
		GetClientRect(&m_rtClient);	
		EnableScrollBarCtrl(SB_BOTH,FALSE);
		//TP_ReplaceScrollBar(this);
	}
	else
	{
		//if(m_bSubWindow)  TP_UnReplaceScrollBar(this);
		m_bSubWindow = FALSE;
	}
}
int CTPScrollBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow();
	DWORD dwcStyle = GetClassLong(GetSafeHwnd(),GCL_STYLE);
	dwcStyle &= ~CS_DBLCLKS;
	SetClassLong(GetSafeHwnd(),GCL_STYLE, dwcStyle);

	// TODO:  在此添加您专用的创建代码

	return 0;
}
BOOL CTPScrollBar::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bCreate = TRUE;

	return CScrollBar::PreCreateWindow(cs);
}
void CTPScrollBar::PreSubclassWindow()
{	
	CScrollBar::PreSubclassWindow();
	if(!m_bCreate) SubWindow();
}

void  CTPScrollBar::ShowScrollBar(BOOL bShow)
{
	if(bShow)
	{		
		CScrollBar::ShowWindow(SW_SHOW);
		CScrollBar::EnableScrollBarCtrl(SB_BOTH,FALSE);	
	}
	else CScrollBar::ShowWindow(SW_HIDE);
}
int   CTPScrollBar::GetScrollPos() const
{
	return (int)m_iCurPos;
}
INT64   CTPScrollBar::SetScrollPos(INT64 nPos, BOOL bRedraw)
{
	if(nPos >m_nMaxVisual-m_iPageSize + TP_DIS)    nPos  = m_nMaxVisual-m_iPageSize + TP_DIS;
	if(nPos <m_nMinVisual)                         nPos  = m_nMinVisual;
	m_iCurPos  = nPos;
	m_iDragPos = nPos;
	if(bRedraw)
	{
		Invalidate();
		//UpdateWindow();
	}
	return nPos;
}
void  CTPScrollBar::GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const
{	
	*lpMinPos = (int)m_nMinVisual;
	*lpMaxPos = (int)m_nMaxVisual;
}
void  CTPScrollBar::SetScrollRange(INT64 nMinPos, INT64 nMaxPos, BOOL bRedraw)
{
	CRect rectClient;
	GetScrollBarRect(rectClient);
	if(nMinPos<0||nMaxPos<0)      {m_nMinVisual = 0;m_nMaxVisual = 0;            }
	else if(nMinPos > nMaxPos )   {m_nMinVisual = nMinPos;m_nMaxVisual = nMinPos;}
	else                          {m_nMinVisual = nMinPos;m_nMaxVisual = nMaxPos;}

	if(m_iCurPos >m_nMaxVisual-m_iPageSize+TP_DIS)    {m_iCurPos  = m_nMaxVisual-m_iPageSize+TP_DIS;}
	if(m_iCurPos <nMinPos)                             m_iCurPos  = nMinPos;
	if(m_iCurPos >nMaxPos)                             m_iCurPos  = nMaxPos;	
	

	if(m_bVert && rectClient.Height() >rectClient.Width()*2)
	{
		if(m_nMaxVisual>m_nMinVisual )
		{	
			m_iPageSize = max(1,m_iPageSize);
			m_iBarPage = TP_DoubleToInt((rectClient.Height()  - rectClient.Width()*2) *m_iPageSize *1.0/ (m_nMaxVisual - m_nMinVisual+0.00001) - 0.5);	
			if(m_iBarPage <10 && rectClient.Height()  - rectClient.Width()*2 >0) 
			{                
			//	m_iPageSize = (m_nMaxVisual - m_nMinVisual) *10/(rectClient.Height()  - rectClient.Width()*2);				
				m_iBarPage  = 10;//TP_DoubleToInt((rectClient.Height()  - rectClient.Width()*2) *m_iPageSize *1.0/ (m_nMaxVisual - m_nMinVisual) - 0.5);	
			}			
		}
		else m_iBarPage = rectClient.Height()  - rectClient.Width()*2;
	}
	else if(rectClient.Width() > rectClient.Height()*2)
	{	
		if(m_nMaxVisual> m_nMinVisual)
		{	
			m_iPageSize =max(1,m_iPageSize);
			m_iBarPage = TP_DoubleToInt((rectClient.Width () - rectClient.Height()*2) *m_iPageSize *1.0 / (m_nMaxVisual - m_nMinVisual+0.00001 )- 0.5);
			if(m_iBarPage <10)
			{
			//	m_iPageSize = (m_nMaxVisual - m_nMinVisual) *10/(rectClient.Width () - rectClient.Height()*2);
				m_iBarPage = 10;//TP_DoubleToInt((rectClient.Width () - rectClient.Height()*2) *m_iPageSize *1.0 / (m_nMaxVisual - m_nMinVisual)- 0.5);
			}
		}
		else m_iBarPage = rectClient.Width () - rectClient.Height()*2;	}

	if(bRedraw) InvalidateRect(rectClient);
}

BOOL  CTPScrollBar::EnableScrollBar(UINT nArrowFlags)
{	
	m_nArrowFlags = nArrowFlags;
	return TRUE;
}
BOOL  CTPScrollBar::SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	if(lpScrollInfo ->fMask &SIF_PAGE )
	{
		m_iPageSize  =  lpScrollInfo ->nPage;
		m_iPageSize  =  max(1,m_iPageSize);//XN00027159 qushaojie 20100104 m_iPageSize>=1
		m_uState    &= ~TP_SCROLLBAR_SETPAGE; 
	}
	if(lpScrollInfo ->fMask &SIF_RANGE  )	
	{		
		lpScrollInfo ->nMax = max(lpScrollInfo ->nMax,lpScrollInfo ->nMin);
		SetScrollRange(lpScrollInfo ->nMin,lpScrollInfo ->nMax+1,bRedraw);	
	}
	if(lpScrollInfo ->fMask &SIF_TRACKPOS)
	{
		m_iDragPos  = lpScrollInfo ->nTrackPos;
	}
	if(lpScrollInfo ->fMask &SIF_POS )
		SetScrollPos(lpScrollInfo ->nPos,bRedraw);


	return TRUE;
}
BOOL  CTPScrollBar::GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask)
{
	if(lpScrollInfo ->fMask &SIF_PAGE )
		lpScrollInfo -> nPage = (int)m_iPageSize;
	if(lpScrollInfo ->fMask &SIF_TRACKPOS)
	{
		m_iDragPos = min(m_iDragPos,m_nMaxVisual-m_iPageSize + TP_DIS);
		m_iDragPos = max(m_iDragPos,m_nMinVisual);		
		lpScrollInfo ->nTrackPos = (int)m_iDragPos;
	}
	if(lpScrollInfo ->fMask &SIF_POS)
	{
		m_iCurPos = min(m_iCurPos,m_nMaxVisual-m_iPageSize +TP_DIS);
		m_iCurPos = max(m_iCurPos,m_nMinVisual);		
		lpScrollInfo ->nPos = (int)m_iCurPos;
	}
	if(lpScrollInfo ->fMask &SIF_RANGE )
	{
		if(!IsWindowVisible())
		{
			lpScrollInfo->nMax = lpScrollInfo->nMin;			
		}
		else
		{
			lpScrollInfo->nMax  = (int)m_nMaxVisual-1;
			lpScrollInfo->nMin  = (int)m_nMinVisual;			
		}
	}	
	return TRUE;		 
}
int   CTPScrollBar::GetScrollLimit()
{
	return int(m_nMaxVisual - max(m_iPageSize -TP_DIS,0));
}
#if(_WIN32_WINNT >= 0x0501) 
BOOL   CTPScrollBar::GetScrollBarInfo(PSCROLLBARINFO pScrollInfo) const
{
	pScrollInfo ->cbSize = sizeof(SCROLLBARINFO);

	if(m_bVert)
	{
		pScrollInfo ->rcScrollBar   = m_rtScrolBar;
		pScrollInfo ->dxyLineButton = m_rtScrolBar.Height();
		pScrollInfo ->xyThumbTop    = m_rtScrolBar.top;
		pScrollInfo ->xyThumbBottom = m_rtScrolBar.bottom;
	}
	else 
	{
		pScrollInfo ->rcScrollBar    = m_rtScrolBar;
		pScrollInfo ->dxyLineButton  = m_rtScrolBar.Width();
		pScrollInfo ->xyThumbTop     = m_rtScrolBar.left;
		pScrollInfo ->xyThumbBottom  = m_rtScrolBar.right;
	}
	return TRUE;
}
#endif

BOOL CTPScrollBar::OnEraseBkgnd(CDC* pDC) 
{
	if(CTPBaseWnd::EraseBkgnd(pDC,this)) return TRUE;
	else return TRUE;
	return CScrollBar::OnEraseBkgnd(pDC);
}
void CTPScrollBar::OnPaint() 
{
	CPaintDC dc(this); 
	DrawDC(&dc);
}
void CTPScrollBar::GetScrollBarRect( CRect &rtBar)
{
	GetClientRect(&rtBar);
}
void  CTPScrollBar::DrawDC(CDC *pDC)
{
	CRect rectClient;
	GetClientRect(&rectClient);
	if(rectClient.Width()<=0) return;
	CTPMemoDC cdcMemo(pDC,rectClient.Width(),rectClient.Height());			
	cdcMemo.SetStretchBltMode(COLORONCOLOR);	
	cdcMemo.FillSolidRect(rectClient,RGB(255,0,0));
	if(m_bReDraw) 	DrawScrollBar(&cdcMemo);
	pDC->BitBlt(0,0, rectClient.Width(),rectClient.Height(),&cdcMemo,0,0,SRCCOPY);  
}
void  CTPScrollBar::DrawScrollBar(CDC *pDC)
{
	CRect rectBar,rectTemp;
	int iTopArrow =0 , iBottomArrow =0;
	GetScrollBarRect(rectBar);
	if(rectBar.Width() <=0) rectBar;

	if(!m_bVert)
	{
		m_rtLeftTopArrow        =	rectBar;
		m_rtLeftTopArrow.right  =  m_rtLeftTopArrow.left    +  m_rtLeftTopArrow.Height();

		m_rtRightBottomArrow       = 	rectBar;
		m_rtRightBottomArrow.left  =  m_rtRightBottomArrow.right   -  m_rtRightBottomArrow.Height();

		m_iBarPage = max(TP_DoubleToInt((m_rtRightBottomArrow.left - m_rtLeftTopArrow.right) *m_iPageSize *1.0/ (m_nMaxVisual - m_nMinVisual+0.00001)),10);	
	

		int iIndex = SCROLLBAR_BACK;
		TP_StretchDIBRect(pDC,m_aBmpres[iIndex] ->GetByte(),m_aBmpres[iIndex]->Width(),m_aBmpres[iIndex]->Height(),0,0,rectBar,FALSE);			
		if(m_iDragMode != DRAG_SCROLL_LEFT)		iIndex = SCROLLBAR_TL_NOR;		
		else                          		    iIndex = SCROLLBAR_TL_DOWN;		
		TP_StretchDIBits(pDC,m_aBmpres[iIndex] ->GetByte(),m_aBmpres[iIndex]->Width(),m_aBmpres[iIndex]->Height(),m_rtLeftTopArrow);		
		if(m_iDragMode != DRAG_SCROLL_RIGHT) 	iIndex = SCROLLBAR_RB_NOR;		
		else                                    iIndex = SCROLLBAR_RB_DOWN;		
		TP_StretchDIBits(pDC,m_aBmpres[iIndex] ->GetByte(),m_aBmpres[iIndex]->Width(),m_aBmpres[iIndex]->Height(),m_rtRightBottomArrow);		

		m_rtScrolBar = rectBar;

		m_rtScrolBar.left     =  PosTpPixel(m_iCurPos);
		m_rtScrolBar.right    = min(m_rtScrolBar.left       + m_iBarPage,m_rtRightBottomArrow.left);	

		if(m_rtScrolBar.Width()>0 && m_nMaxVisual - m_nMinVisual >=m_iPageSize)
		{		
			if(m_iDragMode == DRAG_SCROLL_MIDDLEOK || m_iDragMode == DRAG_SCROLL_MIDDLE)
				iIndex = SCROLLBAR_THUMB_DOWN;
			else if(m_bHover)                       iIndex = SCROLLBAR_THUMB_OVER;
			else                                    iIndex = SCROLLBAR_THUMB_NOR;
			TP_StretchDIBRect(pDC,m_aBmpres[iIndex] ->GetByte(),m_aBmpres[iIndex]->Width(),m_aBmpres[iIndex]->Height(),15,15,m_rtScrolBar,FALSE);			
		}
		//画拖动
		if(m_iDragMode == DRAG_SCROLL_MIDDLELEFT)
		{
			rectTemp = m_rtScrolBar;
			rectTemp.left     = m_rtLeftTopArrow.right  ;
			rectTemp.right    = m_rtScrolBar.left;
			rectTemp.DeflateRect(1,1);
			if(rectTemp.Width()>0) pDC->InvertRect(rectTemp); 
		}
		else if(m_iDragMode == DRAG_SCROLL_MIDDLERIGHT)
		{
			rectTemp = m_rtScrolBar;
			rectTemp.left    = m_rtScrolBar.right;
			rectTemp.right   = m_rtRightBottomArrow.left ;
			rectTemp.DeflateRect(1,1);
			if(rectTemp.Width()>0) pDC->InvertRect(rectTemp); 
		}	
	}
	else
	{
		m_rtLeftTopArrow        =	rectBar;
		m_rtLeftTopArrow.bottom =   m_rtLeftTopArrow.top +  m_rtLeftTopArrow.Width();

		m_rtRightBottomArrow     = 	rectBar;
		m_rtRightBottomArrow.top =   m_rtRightBottomArrow.bottom  -  m_rtRightBottomArrow.Width();



		if(m_bTLFace)
		{
			m_rtLeftTopArrow.bottom  =   m_rtLeftTopArrow.top         +  8;
			m_rtRightBottomArrow.top =   m_rtRightBottomArrow.bottom  -  8;
			pDC ->FillSolidRect(rectBar,TP_GetSysColor(COLOR_BASE_FRAME));

 			if(m_iDragMode == DRAG_SCROLL_TOP) ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMLEFTDOWNV,pDC->GetSafeHdc(),m_rtLeftTopArrow.left,m_rtLeftTopArrow.top,ILD_TRANSPARENT);		
			else                               ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMLEFTUPV,  pDC->GetSafeHdc(),m_rtLeftTopArrow.left,m_rtLeftTopArrow.top,ILD_TRANSPARENT);

			if(m_iDragMode == DRAG_SCROLL_BOTTOM ) ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMRIGHTDOWNV,pDC->GetSafeHdc(),m_rtRightBottomArrow.left,m_rtRightBottomArrow.top,ILD_TRANSPARENT);	 
			else                                   ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMRIGHTUPV ,  pDC->GetSafeHdc(),m_rtRightBottomArrow.left,m_rtRightBottomArrow.top,ILD_TRANSPARENT);	
		}
		else
		{
			int  iIndex = SCROLLBAR_BACK;
			TP_StretchDIBRect(pDC,m_aBmpres[iIndex] ->GetByte(),m_aBmpres[iIndex]->Width(),m_aBmpres[iIndex]->Height(),0,0,rectBar,TRUE,TRUE);			
			if(m_iDragMode != DRAG_SCROLL_TOP)      iIndex = SCROLLBAR_TL_NOR;		
			else                                    iIndex = SCROLLBAR_TL_DOWN;		
			TP_StretchDIBits(pDC,m_aBmpres[iIndex] ->GetByte(),m_aBmpres[iIndex]->Width(),m_aBmpres[iIndex]->Height(),m_rtLeftTopArrow);		
			if(m_iDragMode != DRAG_SCROLL_BOTTOM) 	iIndex = SCROLLBAR_RB_NOR;		
			else                              	    iIndex = SCROLLBAR_RB_DOWN;		
			TP_StretchDIBits(pDC,m_aBmpres[iIndex] ->GetByte(),m_aBmpres[iIndex]->Width(),m_aBmpres[iIndex]->Height(),m_rtRightBottomArrow);		
		}

		m_iBarPage = max(10,TP_DoubleToInt((m_rtRightBottomArrow.top - m_rtLeftTopArrow.bottom) *m_iPageSize *1.0/ (m_nMaxVisual - m_nMinVisual+0.00001)));	

		m_rtScrolBar = rectBar;
		m_rtScrolBar.top      = PosTpPixel(m_iCurPos);
		m_rtScrolBar.bottom   = min(m_rtScrolBar.top    +  m_iBarPage,m_rtRightBottomArrow.top);	

        
		if(m_rtScrolBar.Height()>0  && m_nMaxVisual - m_nMinVisual >=m_iPageSize)
		{
			int iIndex = SCROLLBAR_BACK;
			if(m_iDragMode == DRAG_SCROLL_MIDDLEOK || m_iDragMode == DRAG_SCROLL_MIDDLE)
				iIndex = SCROLLBAR_THUMB_DOWN;
			else if(m_bHover)                       iIndex = SCROLLBAR_THUMB_OVER;
			else                                    iIndex = SCROLLBAR_THUMB_NOR;

			if(m_bTLFace) TP_StretchDIBRect(pDC,m_pResBarBack ->GetByte(),m_pResBarBack ->Width(),m_pResBarBack ->Height(),0,0,m_rtScrolBar,TRUE);
			else          TP_StretchDIBRect(pDC,m_aBmpres[iIndex] ->GetByte(),m_aBmpres[iIndex]->Width(),m_aBmpres[iIndex]->Height(),15,15,m_rtScrolBar,TRUE,FALSE);			
		}
		if(m_iDragMode == DRAG_SCROLL_MIDDLETOP)
		{
			rectTemp = m_rtScrolBar;
			rectTemp.top    = m_rtLeftTopArrow.bottom ;
			rectTemp.bottom = m_rtScrolBar.top ;
			rectTemp.DeflateRect(1,1);
			if(rectTemp.Height()>0) pDC->InvertRect(rectTemp); 
		}
		else if(m_iDragMode == DRAG_SCROLL_MIDDLEBOTTOM)
		{
			rectTemp = m_rtScrolBar;
			rectTemp.top    = m_rtScrolBar.bottom;
			rectTemp.bottom = m_rtRightBottomArrow.top ;
			rectTemp.DeflateRect(1,1);
			if(rectTemp.Height()>0) pDC->InvertRect(rectTemp); 
		}
	}
}
INT64  CTPScrollBar::PixelToPos(int iPixel)
{
	if(!m_bVert)
	{
		return (INT64(iPixel - m_rtLeftTopArrow.right)) *(m_nMaxVisual - m_nMinVisual-m_iPageSize)/(m_rtRightBottomArrow.left - m_rtLeftTopArrow.right - m_iBarPage) + m_nMinVisual;	
	}
	else 
	{
		return (INT64(iPixel - m_rtLeftTopArrow.bottom)) *(m_nMaxVisual - m_nMinVisual-m_iPageSize)/(m_rtRightBottomArrow.top - m_rtLeftTopArrow.bottom - m_iBarPage) + m_nMinVisual;
	}
	return 0;
}
int  CTPScrollBar::PosTpPixel(INT64 iPos)
{	
	//int iIndex = 0;	
	if(!m_bVert)
	{
		return TP_DoubleToInt((m_rtClient.Width () - m_rtClient.Height()*2 - m_iBarPage) *(iPos - m_nMinVisual) *1.0   / (m_nMaxVisual - m_nMinVisual-m_iPageSize+0.00001)+m_rtClient.Height());	
	}
	else 
	{
		return TP_DoubleToInt((m_rtClient.Height() - m_rtClient.Width()*2 - m_iBarPage) *(iPos - m_nMinVisual) *1.0  / (m_nMaxVisual - m_nMinVisual-m_iPageSize+0.00001)+ m_rtClient.Width());
	}
}

void CTPScrollBar::OnTimer(UINT nIDEvent) 
{	
	if (nIDEvent == 1)
	{	
		if(!m_bVert)
		{
			INT64 iCurPos = m_iCurPos;
			if(m_iSetTimer>DRAGPENDING||!TP_IsLButtonDown())
			{
				CPoint point;
				::GetCursorPos(&point); 
				ScreenToClient(&point); 
				if(m_rtLeftTopArrow.PtInRect(point) && m_iDragMode == DRAG_SCROLL_LEFT)
				{
					if(m_iCurPos>m_nMinVisual)
					{
						iCurPos = m_iCurPos -1;
						iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+ TP_DIS);
						iCurPos = max(iCurPos,m_nMinVisual);	
						if(iCurPos != m_iDragPos)
						{						
							m_iDragPos = iCurPos;
							m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINELEFT,(WORD)m_iDragPos),(LPARAM)m_hWnd);
						}
					}
				}
				else if(m_rtRightBottomArrow.PtInRect(point) && m_iDragMode == DRAG_SCROLL_RIGHT)
				{ 
					if(m_iCurPos < m_nMaxVisual)
					{
						iCurPos = m_iCurPos +1;
						iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
						iCurPos = max(iCurPos,m_nMinVisual);	
						if(iCurPos != m_iDragPos)
						{						
							m_iDragPos = iCurPos;
							m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINERIGHT,(WORD)m_iDragPos),(LPARAM)m_hWnd);
						}
					}
				}

				else if(point.x >=m_rtLeftTopArrow.right && point.x <= m_rtScrolBar.left && m_iDragMode == DRAG_SCROLL_MIDDLELEFT)
				{
					if(m_iCurPos>m_nMinVisual)
					{
						iCurPos = m_iCurPos -10;
						iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
						iCurPos = max(iCurPos,m_nMinVisual);	
						if(iCurPos != m_iDragPos)
						{						
							m_iDragPos = iCurPos;
							m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGELEFT,(WORD)m_iDragPos),(LPARAM)m_hWnd);
						}
					}
				}
				else if(point.x <=m_rtRightBottomArrow.left   && point.x >= m_rtScrolBar.right  && m_iDragMode == DRAG_SCROLL_MIDDLERIGHT)
				{
					if(m_iCurPos < m_nMaxVisual)
					{
						iCurPos = m_iCurPos +10;
						iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
						iCurPos = max(iCurPos,m_nMinVisual);	
						if(iCurPos != m_iDragPos)
						{						
							m_iDragPos = iCurPos;
							m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGERIGHT,(WORD)m_iDragPos),(LPARAM)m_hWnd);
						}
					}
				}
			}
			else m_iSetTimer ++;
		}
		else 
		{
			INT64 iCurPos = m_iCurPos;
			if(m_iSetTimer>DRAGPENDING||!TP_IsLButtonDown())
			{
				CPoint point;
				::GetCursorPos(&point); 
				ScreenToClient(&point); 
				if(m_rtLeftTopArrow.PtInRect(point) && m_iDragMode == DRAG_SCROLL_TOP)
				{
					if(m_iCurPos>m_nMinVisual)
					{
						iCurPos = m_iCurPos -1;
						iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
						iCurPos = max(iCurPos,m_nMinVisual);	
						if(iCurPos != m_iDragPos)
						{						
							m_iDragPos = iCurPos;
							m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEUP,(WORD)m_iDragPos),(LPARAM)m_hWnd);
						}
					}
				}
				else if(m_rtRightBottomArrow.PtInRect(point) && m_iDragMode == DRAG_SCROLL_BOTTOM)
				{ 
					if(m_iCurPos < m_nMaxVisual)
					{
						iCurPos = m_iCurPos +1;
						iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
						iCurPos = max(iCurPos,m_nMinVisual);	
						if(iCurPos != m_iDragPos)
						{						
							m_iDragPos = iCurPos;
							m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,(WORD)m_iDragPos),(LPARAM)m_hWnd);			
						}
					}
				}

				else if(point.y >=m_rtLeftTopArrow.bottom && point.y <= m_rtScrolBar.top && m_iDragMode == DRAG_SCROLL_MIDDLETOP)
				{
					if(m_iCurPos>m_nMinVisual)
					{
						iCurPos = m_iCurPos -10;
						iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
						iCurPos = max(iCurPos,m_nMinVisual);	
						if(iCurPos != m_iDragPos)
						{						
							m_iDragPos = iCurPos;
							m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEUP,(WORD)m_iDragPos),(LPARAM)m_hWnd);		
						}
					}
				}
				else if(point.y <=m_rtRightBottomArrow.top  && point.y >= m_rtScrolBar.bottom && m_iDragMode == DRAG_SCROLL_MIDDLEBOTTOM)
				{
					if(m_iCurPos < m_nMaxVisual)
					{
						iCurPos = m_iCurPos +10;
						iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
						iCurPos = max(iCurPos,m_nMinVisual);	
						if(iCurPos != m_iDragPos)
						{						
							m_iDragPos = iCurPos;
							m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEDOWN,(WORD)m_iDragPos),(LPARAM)m_hWnd);		
						}
					}
				}
			}
			else m_iSetTimer ++;
		}
	}
	else if(nIDEvent == 2)
	{
		CPoint point;
		::GetCursorPos(&point); 
		ScreenToClient(&point); 
		if(!m_rtScrolBar.PtInRect(point))
		{
			m_bHover = FALSE;
			InvalidateRect(m_rtScrolBar);
			KillTimer(m_hTimerHover);
			m_hTimerHover = 0;
		}
	}
	//CScrollBar::OnTimer(nIDEvent);
}

void CTPScrollBar::OnSize(UINT nType, int cx, int cy) 
{
	CScrollBar::OnSize(nType, cx, cy);
	GetScrollBarRect(m_rtClient);

	
}
void CTPScrollBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CScrollBar::OnShowWindow(bShow, nStatus);	
	if(bShow)	CScrollBar::EnableScrollBarCtrl(SB_BOTH,FALSE);	
}
void CTPScrollBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//CScrollBar::OnLButtonDown(nFlags, point);
	if(m_bSetCapture && GetFocus() != this) SetFocus();
	INT64 iCurPos  = m_iCurPos;
	BOOL bUpdate = FALSE;
	if(m_pMsgWnd) m_pMsgWnd ->SendMessage(WM_SCROLLBAR,3,(LPARAM)this);
	if(!m_bVert)
	{
		m_iSetTimer = 0; 
		if(m_rtLeftTopArrow.PtInRect(point) )
		{
			if(m_iCurPos>=m_nMinVisual && !(m_nArrowFlags & ESB_DISABLE_LTUP))
			{
				m_iDragMode = DRAG_SCROLL_LEFT;
				iCurPos --;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				iCurPos = max(iCurPos,m_nMinVisual);		
				if(m_iDragPos != iCurPos)
				{				
					m_iDragPos = iCurPos;
					m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINELEFT,(WORD)iCurPos),(LPARAM)m_hWnd);
				}
				if(m_bSetCapture)					SetCapture();
				m_hTimerAcce = SetTimer(1,PRESSS_CROLL_ECHO_TIME,NULL);				
			}
		}
		else if(m_rtRightBottomArrow.PtInRect(point))
		{ 
			if(m_iCurPos <= m_nMaxVisual-m_iPageSize+TP_DIS && !(m_nArrowFlags & ESB_DISABLE_RTDN))
			{
				m_iDragMode = DRAG_SCROLL_RIGHT;
				iCurPos ++;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				iCurPos = max(iCurPos,m_nMinVisual);
				if(m_iDragPos != iCurPos)
				{				
					m_iDragPos = iCurPos;
					m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINERIGHT,(WORD)iCurPos),(LPARAM)m_hWnd);
				}
				if(m_bSetCapture)	SetCapture();		
				m_hTimerAcce = SetTimer(1,PRESSS_CROLL_ECHO_TIME,NULL);				
			}
		}
		else if(m_rtScrolBar.PtInRect(point))
		{ 
			m_iDragMode = DRAG_SCROLL_MIDDLE;
			if(m_bSetCapture)	SetCapture();		
			m_ptDragSave = point;
			m_iDragLeftSave  = point.x - m_rtScrolBar.left ;
			Invalidate();
		}
		else if(point.x >=m_rtLeftTopArrow.right  && point.x <= m_rtScrolBar.left)
		{
			if(m_iCurPos>m_nMinVisual)
			{
				m_iDragMode = DRAG_SCROLL_MIDDLELEFT;	
				iCurPos -= m_iPageSize;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				iCurPos = max(iCurPos,m_nMinVisual);	
				if(m_iDragPos != iCurPos)
				{				
					m_iDragPos = iCurPos;
					m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGELEFT,(WORD)iCurPos),(LPARAM)m_hWnd);
				}
				if(m_bSetCapture)			SetCapture();
				m_hTimerAcce = SetTimer(1,PRESSS_CROLL_ECHO_TIME,NULL);
				
			}
		}
		else if(point.x <=m_rtRightBottomArrow.right  && point.x >= m_rtScrolBar.left )
		{
			if(m_iCurPos < m_nMaxVisual-m_iPageSize+TP_DIS)
			{
				m_iDragMode = DRAG_SCROLL_MIDDLERIGHT;
				iCurPos += m_iPageSize;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				iCurPos = max(iCurPos,m_nMinVisual);				
				if(m_iDragPos != iCurPos)
				{				
					m_iDragPos = iCurPos;
					m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGERIGHT,(WORD)iCurPos),(LPARAM)m_hWnd);
				}
				if(m_bSetCapture)				SetCapture();	
				m_hTimerAcce = SetTimer(1,PRESSS_CROLL_ECHO_TIME,NULL);
				
			}
		}
	}
	else
	{
		m_iSetTimer = 0; 	
		if(m_rtLeftTopArrow.PtInRect(point) )
		{
			if(m_iCurPos>=m_nMinVisual  && !(m_nArrowFlags & ESB_DISABLE_RTDN))
			{
				m_iDragMode = DRAG_SCROLL_TOP;
				iCurPos --;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				iCurPos = max(iCurPos,m_nMinVisual);
				if(m_iDragPos != iCurPos)
				{				
					m_iDragPos = iCurPos;
					m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEUP,(WORD)iCurPos),(LPARAM)m_hWnd);
				}
				if(m_bSetCapture)		SetCapture();	
				m_hTimerAcce =  SetTimer(1,PRESSS_CROLL_ECHO_TIME,NULL);
				
			}
		}
		else if(m_rtRightBottomArrow.PtInRect(point))
		{ 
			if(m_iCurPos <= m_nMaxVisual-m_iPageSize+TP_DIS&& !(m_nArrowFlags & ESB_DISABLE_RTDN))
			{
				m_iDragMode = DRAG_SCROLL_BOTTOM;		
				iCurPos ++;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				iCurPos = max(iCurPos,m_nMinVisual);		
				if(m_iDragPos!=iCurPos)
				{				
					m_iDragPos = iCurPos;
					m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,(WORD)iCurPos),(LPARAM)m_hWnd);
				}
				if(m_bSetCapture)		SetCapture();
				m_hTimerAcce =  SetTimer(1,PRESSS_CROLL_ECHO_TIME,NULL);
				
			}
		}
		else if(m_rtScrolBar.PtInRect(point))
		{ 
			m_iDragMode = DRAG_SCROLL_MIDDLE;
			if(m_bSetCapture)
			{
				SetCapture();		
			}
			m_ptDragSave = point;
			m_iDragLeftSave  = point.y - m_rtScrolBar.top;
			Invalidate();
		}
		else if(point.y >=m_rtLeftTopArrow.bottom && point.y <= m_rtScrolBar.top)
		{
			if(m_iCurPos>m_nMinVisual)
			{
				m_iDragMode = DRAG_SCROLL_MIDDLETOP;
				iCurPos -= m_iPageSize;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				iCurPos = max(iCurPos,m_nMinVisual);	
				if(m_iDragPos != iCurPos)
				{				
					m_iDragPos = iCurPos;
					m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEUP,(WORD)iCurPos),(LPARAM)m_hWnd);
				}
				if(m_bSetCapture)		SetCapture();		
				m_hTimerAcce = SetTimer(1,PRESSS_CROLL_ECHO_TIME,NULL);		
			}
		}
		else if(point.y <=m_rtRightBottomArrow.top  && point.y >= m_rtScrolBar.bottom)
		{
			if(m_iCurPos < m_nMaxVisual-m_iPageSize+TP_DIS)
			{
				m_iDragMode = DRAG_SCROLL_MIDDLEBOTTOM;
				iCurPos += m_iPageSize;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				iCurPos = max(iCurPos,m_nMinVisual);	
				if(m_iDragPos != iCurPos)
				{				
					m_iDragPos = iCurPos;
					m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEDOWN,(WORD)iCurPos),(LPARAM)m_hWnd);
				}
				if(m_bSetCapture)	SetCapture();		
				m_hTimerAcce = SetTimer(1,PRESSS_CROLL_ECHO_TIME,NULL);
			}
		}
	}
	if(bUpdate||iCurPos != m_iCurPos)
	{
		//	m_iCurPos = iCurPos;
		InvalidateRect(m_rtClient);	
	}
}

void CTPScrollBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//CScrollBar::OnLButtonUp(nFlags, point);

	INT64 iDragPos = m_iDragPos;
	if(!m_bVert)
	{
		if(	m_iDragMode == DRAG_SCROLL_LEFT)
		{
			if(m_bSetCapture)
			{
				ReleaseCapture();
				KillTimer(m_hTimerAcce);
				m_hTimerAcce = 0;
			}
			m_iDragMode = DRAG_SCROLL_NONE;

			Invalidate();
		}
		else if(m_iDragMode == DRAG_SCROLL_RIGHT)
		{ 
			if(m_bSetCapture)
			{
				ReleaseCapture();
				KillTimer(m_hTimerAcce);
				m_hTimerAcce = 0;
			}
			m_iDragMode = DRAG_SCROLL_NONE;			
			Invalidate();
		}
		else if(m_iDragMode == DRAG_SCROLL_MIDDLEOK || m_iDragMode == DRAG_SCROLL_MIDDLE)
		{ 
			if(m_bSetCapture) ReleaseCapture();
			if(m_iDragMode == DRAG_SCROLL_MIDDLEOK)		
			{
				if(point.x  - m_iDragLeftSave  <= m_rtLeftTopArrow.right) 
				{
					if(m_iCurPos>m_nMinVisual) m_iDragPos = m_nMinVisual;
				}												
				else if(point.x  - m_iDragLeftSave + m_iBarPage >m_rtRightBottomArrow.left  ) 
				{
					if(m_iCurPos<m_nMaxVisual-m_iPageSize+TP_DIS) m_iDragPos = m_nMaxVisual-m_iPageSize+TP_DIS;						
				}			
				else
				{				
					m_iDragPos = PixelToPos(point.x  - m_iDragLeftSave);
				}
				m_iDragPos = min(m_iDragPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				m_iDragPos = max(m_iDragPos,m_nMinVisual);
				if(iDragPos != m_iDragPos)
				{				
					m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)m_iDragPos),(LPARAM)m_hWnd);	
					SetScrollPos(m_iDragPos,TRUE);
				}
			}		
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD)0),(LPARAM)m_hWnd);				
			Invalidate();
			m_iDragMode = DRAG_SCROLL_NONE;
		}	
		else if(m_iDragMode == DRAG_SCROLL_MIDDLELEFT)
		{ 
			if(m_bSetCapture)
			{
				ReleaseCapture();
				KillTimer(m_hTimerAcce);
				m_hTimerAcce = 0;
			}
			m_iDragMode = DRAG_SCROLL_NONE;
			Invalidate();
		}	
		if(m_iDragMode == DRAG_SCROLL_MIDDLERIGHT)
		{
			if(m_bSetCapture) 
			{
				ReleaseCapture();
				KillTimer(m_hTimerAcce);
				m_hTimerAcce = 0;
			}
			m_iDragMode = DRAG_SCROLL_NONE;			
			Invalidate();
		}
	}
	else
	{
		if(	m_iDragMode == DRAG_SCROLL_TOP)
		{
			m_iDragMode = DRAG_SCROLL_NONE;

			if(m_bSetCapture) 
			{				
				ReleaseCapture();			
				KillTimer(m_hTimerAcce);
				m_hTimerAcce = 0;
			}
			Invalidate();
		}
		else if(m_iDragMode == DRAG_SCROLL_BOTTOM)
		{ 
			m_iDragMode = DRAG_SCROLL_NONE;	
			if(m_bSetCapture) 
			{			
				ReleaseCapture();
				KillTimer(m_hTimerAcce);
				m_hTimerAcce = 0;
			}
			Invalidate();
		}
		else if(m_iDragMode == DRAG_SCROLL_MIDDLEOK|| m_iDragMode == DRAG_SCROLL_MIDDLE)
		{ 
			if(m_bSetCapture) 		ReleaseCapture();			
			if(m_iDragMode == DRAG_SCROLL_MIDDLEOK)		
			{
				if(point.y  - m_iDragLeftSave  <= m_rtLeftTopArrow.bottom) 
				{
					if(m_iCurPos>m_nMinVisual) m_iDragPos = m_nMinVisual;
				}												
				else if(point.y  - m_iDragLeftSave + m_iBarPage >m_rtRightBottomArrow.top  ) 
				{
					if(m_iCurPos<m_nMaxVisual-m_iPageSize+TP_DIS) m_iDragPos = m_nMaxVisual-m_iPageSize+TP_DIS;						
				}			
				else
				{				
					m_iDragPos = PixelToPos(point.y  - m_iDragLeftSave);
				}
				m_iDragPos = min(m_iDragPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				m_iDragPos = max(m_iDragPos,m_nMinVisual);
				if(iDragPos != m_iDragPos)
				{				
					m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)m_iDragPos),(LPARAM)m_hWnd);	
					SetScrollPos(m_iDragPos,TRUE);
				}
			}
			m_iDragMode = DRAG_SCROLL_NONE;
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD)0),(LPARAM)m_hWnd);
			Invalidate();
		}	
		else if(m_iDragMode == DRAG_SCROLL_MIDDLETOP)
		{ 
			if(m_bSetCapture) 
			{			
				ReleaseCapture();
				KillTimer(1);
				m_hTimerAcce = 0;
			}
			m_iDragMode = DRAG_SCROLL_NONE;			
			Invalidate();
		}	
		if(m_iDragMode == DRAG_SCROLL_MIDDLEBOTTOM)
		{
			if(m_bSetCapture) 
			{			
				ReleaseCapture();
				KillTimer(m_hTimerAcce);
				m_hTimerAcce = 0;
			}
			m_iDragMode = DRAG_SCROLL_NONE;

			Invalidate();
		}
	}
	if(m_pMsgWnd) m_pMsgWnd ->SendMessage(WM_SCROLLBAR,4,(LPARAM)this);
}

void CTPScrollBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!(MK_LBUTTON &  nFlags)) m_iDragMode = 0;
	if(!m_bVert)
	{
		if(m_iDragMode == DRAG_SCROLL_MIDDLEOK || m_iDragMode == DRAG_SCROLL_MIDDLE)
		{ 
			if(GetTickCount() - m_dwTimeTickSave >MOUSE_DELAY)
			{
				m_iDragMode = DRAG_SCROLL_MIDDLEOK;
				if(m_ptDragSave.x != point.x)		
				{
					INT64 iDragPos = m_iDragPos;
					if(point.x  - m_iDragLeftSave  <= m_rtLeftTopArrow.right) 
					{
						if(m_iCurPos>m_nMinVisual) m_iDragPos = m_nMinVisual;
					}												
					else if(point.x  - m_iDragLeftSave + m_iBarPage >m_rtRightBottomArrow.left  ) 
					{
						if(m_iCurPos<m_nMaxVisual-m_iPageSize+TP_DIS) m_iDragPos = m_nMaxVisual-m_iPageSize+TP_DIS;						
					}			
					else
					{				
						m_iDragPos = PixelToPos(point.x  - m_iDragLeftSave);
					}
					m_iDragPos = min(m_iDragPos,m_nMaxVisual-m_iPageSize+TP_DIS);
					m_iDragPos = max(m_iDragPos,m_nMinVisual);
					if(iDragPos != m_iDragPos)
					{
						m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)m_iDragPos),(LPARAM)m_hWnd);	
						if(m_bSetScrollInfo)	SetScrollPos(m_iDragPos,TRUE);
					}
				}
				m_ptDragSave = point;
				m_dwTimeTickSave = GetTickCount();
				Invalidate();
			}
		}
	}
	else
	{
		if(m_iDragMode == DRAG_SCROLL_MIDDLEOK || m_iDragMode == DRAG_SCROLL_MIDDLE)
		{ 
			if(GetTickCount() - m_dwTimeTickSave >MOUSE_DELAY)
			{
				m_iDragMode = DRAG_SCROLL_MIDDLEOK;
				if(m_ptDragSave.y != point.y)		
				{
					INT64 iDragPos = m_iDragPos;
					if(point.y  - m_iDragLeftSave  <= m_rtLeftTopArrow.bottom) 
					{
						if(m_iCurPos>m_nMinVisual) m_iDragPos = m_nMinVisual;
					}												
					else if(point.y  - m_iDragLeftSave + m_iBarPage >m_rtRightBottomArrow.top  ) 
					{
						if(m_iCurPos<m_nMaxVisual-m_iPageSize+TP_DIS) m_iDragPos = m_nMaxVisual-m_iPageSize+TP_DIS;						
					}			
					else
					{				
						m_iDragPos = PixelToPos(point.y  - m_iDragLeftSave);
					}				
					m_iDragPos = min(m_iDragPos,m_nMaxVisual-m_iPageSize+TP_DIS);
					m_iDragPos = max(m_iDragPos,m_nMinVisual);
					if(iDragPos != m_iDragPos)
					{					
						m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)m_iDragPos),(LPARAM)m_hWnd);				
						if(m_bSetScrollInfo)	SetScrollPos(m_iDragPos,TRUE);
					}
				}
				m_ptDragSave = point;
				m_dwTimeTickSave = GetTickCount();
			}		
		}
	}
	if(m_iDragMode == DRAG_SCROLL_NONE && !m_bHover)
	{
		CPoint point;
		::GetCursorPos(&point); 
		ScreenToClient(&point); 
		if(m_rtScrolBar.PtInRect(point))
		{
			m_bHover = TRUE;
			InvalidateRect(m_rtScrolBar);
			
			m_hTimerHover = SetTimer(2,PRESSS_CROLL_ECHO_TIME,NULL);			
		}
	}
	//	CScrollBar::OnMouseMove(nFlags, point);
}


void CTPScrollBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CScrollBar::OnLButtonDblClk(nFlags, point);
}

void CTPScrollBar::OnDestroy()
{
	if(m_hTimerAcce)  KillTimer(m_hTimerAcce);
	if(m_hTimerHover) KillTimer(m_hTimerHover);
	SubWindow(FALSE);
	CScrollBar::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CTPScrollBar::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//CScrollBar::OnKeyDown(nChar, nRepCnt, nFlags);

	BOOL bUpdate = FALSE;
	INT64  iCurPos = m_iCurPos;
	if(!m_bVert)
	{
		if(nChar == VK_LEFT)		
		{
			if(m_iCurPos>m_nMinVisual && !(m_nArrowFlags & ESB_DISABLE_LTUP))
			{
				iCurPos --;
				iCurPos = max(iCurPos,m_nMinVisual);	
				if(m_iCurPos !=iCurPos)	m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINELEFT,(WORD)iCurPos),(LPARAM)m_hWnd);			
			}
		}
		else if(nChar == VK_RIGHT)		
		{ 
			if(m_iCurPos < m_nMaxVisual-m_iPageSize+TP_DIS && !(m_nArrowFlags & ESB_DISABLE_RTDN))
			{				
				iCurPos ++;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
				if(m_iCurPos !=iCurPos)	m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINERIGHT,(WORD)iCurPos),(LPARAM)m_hWnd);			
			}
		}		
		else if(nChar == VK_PRIOR)		
		{
			if(m_iCurPos>m_nMinVisual)
			{		
				iCurPos -= m_iPageSize;
				iCurPos = max(iCurPos,m_nMinVisual);	
				if(m_iCurPos !=iCurPos)	m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGELEFT,(WORD)iCurPos),(LPARAM)m_hWnd);			
			}
		}
		else if(nChar == VK_NEXT)		
		{
			if(m_iCurPos < m_nMaxVisual-m_iPageSize+TP_DIS)
			{				
				iCurPos += m_iPageSize;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);				
				if(m_iCurPos !=iCurPos)	m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGERIGHT,(WORD)iCurPos),(LPARAM)m_hWnd);			
			}
		}
		else
		{
			m_pMsgWnd ->PostMessage(WM_KEYDOWN,nChar,0);
			return;
		}
	}
	else
	{		
		if(nChar == VK_UP)		
		{
			if(m_iCurPos>m_nMinVisual  && !(m_nArrowFlags & ESB_DISABLE_RTDN))
			{				
				iCurPos --;
				iCurPos = max(iCurPos,m_nMinVisual);				
				if(m_iCurPos !=iCurPos)	m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEUP,(WORD)iCurPos),(LPARAM)m_hWnd);			
			}
		}
		else if(nChar == VK_DOWN)		
		{ 
			if(m_iCurPos < m_nMaxVisual-m_iPageSize+TP_DIS&& !(m_nArrowFlags & ESB_DISABLE_RTDN))
			{				
				iCurPos ++;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);			
				if(m_iCurPos !=iCurPos)	m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,(WORD)iCurPos),(LPARAM)m_hWnd);			
			}
		}		
		else  if(nChar == VK_PRIOR)		
		{
			if(m_iCurPos>m_nMinVisual)
			{				
				iCurPos -= m_iPageSize;
				iCurPos = max(iCurPos,m_nMinVisual);				
				if(m_iCurPos !=iCurPos)	m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEUP,(WORD)iCurPos),(LPARAM)m_hWnd);			
			}
		}
		else  if(nChar == VK_NEXT)		
		{
			if(m_iCurPos < m_nMaxVisual-m_iPageSize+TP_DIS)
			{				
				iCurPos += m_iPageSize;
				iCurPos = min(iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);			
				if(m_iCurPos !=iCurPos)	m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEDOWN,(WORD)iCurPos),(LPARAM)m_hWnd);			
			}
		}
		else
		{
			m_pMsgWnd ->PostMessage(WM_KEYDOWN,nChar,0);
			return;
		}
	}
	
	if(bUpdate||iCurPos != m_iCurPos)
	{
		//m_iCurPos = iCurPos;
		InvalidateRect(m_rtClient);	
	}
}

BOOL CTPScrollBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_iDragPos = m_iCurPos;
	INT64 iDragPos = m_iDragPos;

	int   iScrollLine = 0;
	if(m_iScollLine >0) iScrollLine = m_iScollLine;
	else                iScrollLine = (int)m_iPageSize/4;

	iScrollLine = max(iScrollLine,1); //xl added 20100427

	if(zDelta >0)
	{
		if(m_iDragPos>m_nMinVisual)
		{		
			m_iDragPos -= iScrollLine;//m_iSrollSize; //xl change 20090902
			m_iDragPos = max(m_iDragPos,m_nMinVisual);	
			if(iDragPos != m_iDragPos)
			{			
				if(m_bVert)  m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)m_iDragPos),(LPARAM)m_hWnd);			
				else  		 m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)m_iDragPos),(LPARAM)m_hWnd);				
			}
		}
	}
	else if(zDelta <0)
	{
		if(m_iDragPos < m_nMaxVisual)
		{				
			m_iDragPos += iScrollLine;//m_iSrollSize;  //xl change 20090902
			m_iDragPos = min(m_iDragPos,m_nMaxVisual-m_iPageSize+TP_DIS);				
			if(iDragPos != m_iDragPos)
			{
				if(m_bVert)  m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)m_iDragPos),(LPARAM)m_hWnd);			
				else  		 m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)m_iDragPos),(LPARAM)m_hWnd);			
			}
		}
	}
	if(m_iDragPos != m_iCurPos) 	SetScrollPos(m_iDragPos,TRUE);
	return 1;
}
void CTPScrollBar::SetScroll(UINT nSBCode, UINT nPos)
{
	INT64 iPos        = m_iCurPos;
	int   iScrollLine = 0;
	if(m_iScollLine >0) iScrollLine = m_iScollLine;
	else                iScrollLine = (int)m_iPageSize/4;

	iScrollLine = max(iScrollLine,1); //xl added 20100427
	switch(nSBCode)
	{

	case SB_LINELEFT:
		m_iCurPos -= iScrollLine;
		break;

	case SB_LINERIGHT:
		m_iCurPos += iScrollLine;
		break;

	case SB_TOP:
		m_iCurPos = m_nMinVisual;
		break;

	case SB_BOTTOM:
		m_iCurPos = m_nMaxVisual;
		break;

	case SB_PAGELEFT:
		m_iCurPos -= m_iPageSize;
		break;

	case SB_PAGERIGHT:
		m_iCurPos += m_iPageSize;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		if(m_iDragPos >0)	m_iCurPos = m_iDragPos;
		else            	m_iCurPos = m_iDragPos;
		break;	
	default:
		return;
	}
	m_iCurPos = min(m_iCurPos,m_nMaxVisual-m_iPageSize+TP_DIS);
	m_iCurPos = max(m_iCurPos,m_nMinVisual);	
	if(iPos !=m_iCurPos)
	{
		iPos =m_iCurPos;
		m_iCurPos = -1;
		SetScrollPos(iPos,TRUE);
	}		
}

void CTPScrollBar::OnSetFocus(CWnd* pOldWnd)
{
	//	CScrollBar::OnSetFocus(pOldWnd);	
	if(m_pMsgWnd) m_pMsgWnd ->SendMessage(WM_SCROLLBAR,0,(LPARAM)this);
}
INT64 CTPScrollBar::GetDragPos()
{
	return m_iDragPos;
}	
void CTPScrollBar::SetTLFace(BOOL bSet)
{
	if(m_bTLFace == bSet) return;
	TP_ReleaseCommonRes(m_pResBar);     m_pResBar     = NULL;
	TP_ReleaseCommonRes(m_pResBarBack); m_pResBarBack = NULL;


	m_bTLFace = bSet;
	if(bSet)
	{
		CString sSkinPath       = TP_GetSkinPath(),sPath = sSkinPath + _L("\\CommonCtrl\\TimeLine\\TimeLine1");

		if(m_bVert)
			m_pResBarBack = TP_LoadBmpByte (sPath + _L("\\TLBarBackV.bmp") );	
		else
			m_pResBarBack = TP_LoadBmpByte (sPath + _L("\\TLBarBack.bmp") );	
		m_pResBar     = TP_LoadImage(sPath + _L("\\timelineicon3.bmp"),32,RGB(0,128,128),FALSE,FALSE);	
	}
}


void CTPScrollBar::SetReDraw(BOOL bReDraw)
{
	m_bReDraw = bReDraw;
}
void CTPScrollBar::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);
	if(m_pMsgWnd) m_pMsgWnd ->SendMessage(WM_SCROLLBAR,1,(LPARAM)this);
}
BOOL CTPScrollBar::SetScrollLine(UINT lLine)
{
	m_iScollLine = lLine;
	return TRUE;
}
BOOL CTPScrollBar::SetScrollMode(UINT uMode)
{
	m_uScrollMode = uMode;
	return TRUE;
}

void CTPScrollBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bAttachBar) return;

	__super::OnRButtonDown(nFlags, point);
}

void CTPScrollBar::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bAttachBar) return;

	__super::OnRButtonUp(nFlags, point);
}

void _tp_commonctrl::CTPScrollBar::OnCaptureChanged(CWnd *pWnd)
{
	if(pWnd !=this) 
		m_iDragMode = 0;

	__super::OnCaptureChanged(pWnd);
}
