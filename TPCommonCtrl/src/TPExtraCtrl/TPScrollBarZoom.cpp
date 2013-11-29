// TPTimeLine.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPScrollBarZoom.h"
 
#include <math.h>


  
 IMPLEMENT_DYNAMIC(CTPScrollBarZoom, CWnd)

 
CTPScrollBarZoom::CTPScrollBarZoom(BOOL bVert  )
{
	m_bVert               = bVert;
	m_pMsgWnd			  = NULL;            
	m_dwWindowLevel       = TPWNDLEVEL1; 

	m_dPixelPerFrame	  = 1.0;      
	m_dStartFrame		  = 0;        
 
 
	m_nMinFrame			  = 0;             
	m_nMaxFrame			  = 100; 
 
	m_dZoomIn			  = 1.00;               
	m_dStartZoomIn		  = 0.00;          

	m_dSaveStartZoomIn    = -0.01;     
	m_dSaveZoomIn         = -0.01;          
	m_dSavePixelPerFrame  = -0.01;
	m_dSaveStatrFrame     = -1;     
 	m_bSaveClick          = FALSE;
	m_iDBClickStep        = 10; 

	m_iBarHeadState       = 0;         
	 
	 
	m_dMinZoomIn	      = 0.001;       


	m_rtCursorArea        = CRect(0,0,0,0);          
	m_rtLeftHead          = CRect(0,0,14,0);          
	m_rtRightHead         = CRect(0,0,14,0);         
 	m_rtBarHandle         = CRect(0,0,0,0);         
 	m_rtTimeBar           = CRect(0,0,0,0);          
	m_rtClient            = CRect(0,0,0,0);          
 	m_enDragType          = TP_DRAG_UNKNOW;           
	m_ptDrag              = CPoint(0,0);            
	m_iDrag               = -1;  
	m_rtDrag              = CRect(0,0,0,0); 


 	m_uBaseState          = TP_PAINT_TRIMINOUT;
    m_bWndEnable          = TRUE;
 
	for(INT l=0;l<TL_MAX;l++)
		m_pResCursor[l]  = NULL;	
	m_pResBar     = NULL;
	m_pResBarBack = NULL;
	for(INT l=0;l<3;l++)
	{
		m_pResBtn[l]  = NULL;	
		m_rtBtn[l]    = CRect(0,0,0,0);
	}
    m_iBtnState = 0;

	m_iLeftEdge    = 0;
	m_iRigthEdge   = 0;

	m_bSubWindow   = FALSE;
	m_bCreate      = FALSE;	
 	 
 
	m_hSetCursor   = NULL;

	m_iBarWidth    = 14;
 }

CTPScrollBarZoom::~CTPScrollBarZoom()
{
 
	for(INT l=0;l<TL_MAX;l++)
	{
		TP_ReleaseCommonRes(m_pResCursor[l]);
		m_pResCursor[l] = NULL;
	}
	for(INT l=0;l<3;l++)
	{
		TP_ReleaseCommonRes(m_pResBtn[l]);
		m_pResBtn[l]  = NULL;	
	}
	
	TP_ReleaseCommonRes(m_pResBar);
	m_pResBar = NULL;
	TP_ReleaseCommonRes(m_pResBarBack);
	m_pResBarBack = NULL;
 
 

	SubWindow(FALSE);
}
 

BEGIN_MESSAGE_MAP(CTPScrollBarZoom, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
 	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_WM_ENABLE()
 	ON_WM_DESTROY()
 	ON_WM_CREATE()
 	ON_WM_ERASEBKGND()
	ON_WM_CAPTURECHANGED()
   	ON_WM_MBUTTONDBLCLK()
 END_MESSAGE_MAP()



void     CTPScrollBarZoom::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_STATIC,&wndClass);		
		wndClass.lpszClassName = WC_TIMELINE;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_SCROLLBARZOOM,AfxGetInstanceHandle());
}

void CTPScrollBarZoom::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	

		if(m_pMsgWnd == NULL) m_pMsgWnd = GetParent();	

		CString sSkinPath       = TP_GetSkinPath(),sPath = sSkinPath + _L("\\CommonCtrl\\TimeLine\\TimeLine1");

		if(m_bVert)
		{
			m_pResCursor[TL_LEFT]   = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorbottom.cur"),FALSE,FALSE);
			m_pResCursor[TL_RIGHT]  = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursortop.cur"),FALSE,FALSE);		
			m_pResBarBack = TP_LoadBmpByte (sPath + _L("\\TLBarBackV.bmp") );	
		}
		else
		{
			m_pResCursor[TL_LEFT]   = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorleft.cur"),FALSE,FALSE);
			m_pResCursor[TL_RIGHT]  = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorright.cur"),FALSE,FALSE);		
			m_pResBarBack = TP_LoadBmpByte (sPath + _L("\\TLBarBack.bmp") );	
		}
		m_pResCursor[TL_ARROW]  = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorarrow.cur"),FALSE,FALSE);
		m_pResCursor[TL_HANDLE] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorhandle.cur"),FALSE,FALSE);
		m_pResBar     = TP_LoadImage(sPath + _L("\\timelineicon3.bmp"),32,RGB(0,128,128),FALSE,FALSE);			

 		GetClientRect(&m_rtClient);
		OnSize(0,m_rtClient.Width(),m_rtClient.Height());		

	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
		 
	}
} 
 
BOOL CTPScrollBarZoom::Create(DWORD dwStyle,const RECT& rect,CWnd* pParentWnd, UINT nID)
{
	BOOL bResult = CWnd::Create(NULL,NULL,dwStyle,rect,pParentWnd,nID);
	if(!bResult) return bResult;	
	m_pMsgWnd            = pParentWnd; 
	return bResult;

}
BOOL CTPScrollBarZoom::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CWnd::PreCreateWindow(cs);
}

int CTPScrollBarZoom::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}
void CTPScrollBarZoom::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}
void CTPScrollBarZoom::OnDestroy() 
{	
	SubWindow(FALSE);
	CWnd::OnDestroy();	
} 
void   CTPScrollBarZoom::SetStartFrame(long  double dFrame,BOOL bPaint)
{
	if(dFrame < m_nMinFrame) 	dFrame = (long double)m_nMinFrame;
	if(dFrame > m_nMaxFrame)	dFrame = (long double)m_nMaxFrame;
	if(dFrame == m_dStartFrame) return; 
	SetStartZoomIn((dFrame- m_nMinFrame)*1.0/(m_nMaxFrame -m_nMinFrame),bPaint);
}

double CTPScrollBarZoom::GetStartZoomIn()
{
	return m_dStartZoomIn;
}
 
double CTPScrollBarZoom::GetZoomIn()
{
	return m_dZoomIn;
}
void CTPScrollBarZoom::SetStartZoomIn(double dZoomIn,BOOL bPaint)
{
	m_dStartZoomIn = dZoomIn;
	if(m_dStartZoomIn < 0.0)            m_dStartZoomIn = 0;
	if(m_dStartZoomIn > 1-m_dMinZoomIn) m_dStartZoomIn = 1-m_dMinZoomIn;

	if(m_dZoomIn > 1-m_dStartZoomIn)    m_dStartZoomIn = 1-m_dZoomIn;

	m_dPixelPerFrame = m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);	
	m_dStartFrame    = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;

	if(bPaint && (m_dSaveStartZoomIn != m_dStartZoomIn || m_dSaveZoomIn != m_dZoomIn))
	{
		Invalidate();
	}
	m_dSaveStartZoomIn = m_dStartZoomIn;
	m_dSaveZoomIn      = m_dZoomIn;
}
 
 
void   CTPScrollBarZoom::SetMaxMinFrame(__int64 iMin,__int64 iMax,BOOL bReset,BOOL bPaint)
{
	if(m_nMaxFrame == iMax && m_nMinFrame == iMin) return;

	if(iMax <=iMin) iMax = iMin +1;

	m_nMaxFrame = iMax;
	m_nMinFrame = iMin;

	GetMiniZoomIn();

	if(bReset) SetZoomIn(1,TRUE,bPaint);
	else       SetZoomIn(m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dPixelPerFrame),TRUE,bPaint);
}

 

 
 
void CTPScrollBarZoom::SetZoomIn(double dZoomIn,BOOL bCenter,BOOL bPaint,double dCurPixel)
{ 
 
	if(dZoomIn < 0) return;
	if(bCenter && dCurPixel ==0.0)
	{
		if(m_bVert)
			dCurPixel = m_rtTimeBar.CenterPoint().y;
		else 
			dCurPixel = m_rtTimeBar.CenterPoint().x;
	}
	double dCurFrame = TranslatePixel(dCurPixel);;

	m_dZoomIn = dZoomIn;
	m_dZoomIn = max(m_dMinZoomIn, m_dZoomIn);
	m_dZoomIn = min(1.0, m_dZoomIn);

	if(m_dSaveZoomIn != 0.0 || m_dZoomIn !=1.0)	m_bSaveClick = FALSE;

	if(!bCenter)
	{	
		if(m_dStartZoomIn >1-m_dZoomIn)  m_dStartZoomIn = 1-m_dZoomIn;
		if(m_dStartZoomIn <0)            m_dStartZoomIn = 0;

		m_dPixelPerFrame =  m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);
		m_dStartFrame = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;		
	}
	else
	{ 

		m_dPixelPerFrame = m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);

		if(m_bVert)
			m_dStartZoomIn =  (dCurFrame - (dCurPixel - m_rtTimeBar.left) / m_dPixelPerFrame - m_nMinFrame )/(m_nMaxFrame -m_nMinFrame); 
		else
			m_dStartZoomIn =  (dCurFrame - (dCurPixel - m_rtTimeBar.top) / m_dPixelPerFrame - m_nMinFrame )/(m_nMaxFrame -m_nMinFrame); 

		if(m_dStartZoomIn >1-m_dZoomIn) m_dStartZoomIn = 1-m_dZoomIn;
		if(m_dStartZoomIn <0)           m_dStartZoomIn = 0;
		m_dStartFrame = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;
	}

	if(bPaint && (m_dSaveStartZoomIn != m_dStartZoomIn || m_dSaveZoomIn != m_dZoomIn))
	{
		Invalidate();
	}
	m_dSaveStartZoomIn = m_dStartZoomIn;
	m_dSaveZoomIn      = m_dZoomIn;
 }
 CRect  CTPScrollBarZoom::GetTimeBtnRect()
 {
	 CRect rtTemp = GetTimeBarRect(),rtBtn;
	 rtBtn = rtTemp;

	 if(m_bVert)
	 {
		 if(m_iRigthEdge>rtTemp.Width() * 3)
		 {
			 rtBtn.top = rtTemp.bottom;
			 rtBtn.bottom = m_rtClient.bottom;
		 }
		 else  if(m_iLeftEdge>rtTemp.Width() * 3)
		 {
			 rtBtn.top    = rtTemp.top-rtTemp.Width() * 3;
			 rtBtn.bottom = rtTemp.top;
		 }
		 else 
		 {
 			 rtBtn = CRect(0,0,0,0);
		 }
		 for(INT l=0;l<3;l++)
			 m_rtBtn[l] = CRect(rtBtn.left,rtBtn.top +  l*rtTemp.Width() ,rtBtn.right, rtBtn.top +  (l + 1)*rtTemp.Width()) ;
	 }
	 else
	 {
		 if(m_iRigthEdge>rtTemp.Height() * 3)
		 {
			 rtBtn.left  = rtTemp.right;
			 rtBtn.right = m_rtClient.right;
		 }
		 else  if(m_iLeftEdge>rtTemp.Height() * 3)
		 {
			 rtBtn.left    = rtTemp.left-rtTemp.Height() * 3;
			 rtBtn.right   = rtTemp.left;
		 }
		 else 
		 {
 			 rtBtn = CRect(0,0,0,0);
		 }
		 for(INT l=0;l<3;l++)
			 m_rtBtn[l] = CRect(rtBtn.left +  l*rtTemp.Height(),rtBtn.top ,rtBtn.left +  (l + 1)*rtTemp.Height(), rtBtn.bottom) ;
	 }
	 return rtBtn;
 }
CRect CTPScrollBarZoom::GetTimeBarRect()
{
	CRect rtTemp = CRect(0,0,0,0);
	if(m_bVert)
	{
		rtTemp         = m_rtClient;
		rtTemp.right   = rtTemp.left + m_iBarWidth;	 
		rtTemp.DeflateRect(0,m_iLeftEdge,0,m_iRigthEdge);
		m_iTimeCodeWidth = rtTemp.Height();
	}
	else
	{
		rtTemp           = m_rtClient;
		rtTemp.bottom    = rtTemp.top + m_iBarWidth;	 
		rtTemp.DeflateRect(m_iLeftEdge,0,m_iRigthEdge,0);
		m_iTimeCodeWidth = rtTemp.Width();
	}
	return rtTemp;
}
 

void CTPScrollBarZoom::OnPaint() 
{
	CPaintDC dc(this); 	
	CTPMemoDC stuDC(&dc,m_rtClient.Width(),m_rtClient.Height());
	PaintTimeBar(&stuDC);
	dc.BitBlt(m_rtTimeBar.left,m_rtTimeBar.top,m_rtTimeBar.Width(),m_rtTimeBar.Height(),&stuDC,m_rtTimeBar.left,m_rtTimeBar.top,SRCCOPY);
}
 
void CTPScrollBarZoom::SetSize( int cx, int cy)
{
	m_rtClient        = CRect(0,0,cx,cy);
	m_rtTimeBar       = GetTimeBarRect();          
 
 	if(m_nMaxFrame <=m_nMinFrame) return;

	m_dPixelPerFrame =  m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn); 
	GetMiniZoomIn();
 }
void CTPScrollBarZoom::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	m_rtClient = CRect(0,0,cx,cy);
	SetSize(cx,cy);	
}

void   CTPScrollBarZoom::GetMiniZoomIn()
{
	if(m_bVert)
		m_dMinZoomIn   = m_rtTimeBar.Height() /90.0/(m_nMaxFrame-m_nMinFrame);
	else 
		m_dMinZoomIn   = m_rtTimeBar.Width() /90.0/(m_nMaxFrame-m_nMinFrame);

 	m_dMinZoomIn   = max(0.00001,m_dMinZoomIn);
	m_dMinZoomIn   = min(1.0,m_dMinZoomIn); 
}
void CTPScrollBarZoom::PaintTimeBar(CDC *pDC)
{	
	CRect rectBar,rectTemp;
	rectBar      = GetTimeBarRect(); //总位置
	pDC->FillSolidRect(rectBar,TP_GetSysColor(COLOR_TL_TIMECODEBACK)); 	

	m_rtTimeBar         = rectBar;
	m_rtBarTotal        = rectBar;   //滑动条位置
	if(m_bVert)
	{
		m_rtBarTotal.top    += TP_DoubleToInt (m_dStartZoomIn  *  m_iTimeCodeWidth);
		m_rtBarTotal.bottom  = rectBar.top +  TP_DoubleToInt ((m_dStartZoomIn + m_dZoomIn) *  m_iTimeCodeWidth);

		m_rtLeftHead         = m_rtBarTotal;  //左边头部位置
		m_rtLeftHead.bottom  = m_rtLeftHead.top + 8;//m_pResBar[TL_RES_BARLEFTUP] ->Width();	

		m_rtRightHead        = m_rtBarTotal;  //右边头部位置
		m_rtRightHead.top    = m_rtRightHead.bottom - 7;//m_pResBar[TL_RES_BARRIGHTUP] ->Width();	

		m_rtBarHandle         = m_rtBarTotal;  //滑动条位置
		m_rtBarHandle.top     = m_rtLeftHead.bottom;
		m_rtBarHandle.bottom  = m_rtRightHead.top;

		if(m_rtLeftHead.bottom>=m_rtRightHead.top)
		{
			m_rtRightHead = m_rtLeftHead;
			m_rtRightHead.OffsetRect(0, 8 *2);
			m_rtBarTotal  = CRect(m_rtLeftHead.left,m_rtLeftHead.top,m_rtRightHead.right,m_rtRightHead.bottom);
			m_rtBarHandle = CRect(0,0,0,0);
		}

		if(m_rtBarHandle.Width()>2)
			TP_StretchDIBRect(pDC,m_pResBarBack ->GetByte(),m_pResBarBack ->Width(),m_pResBarBack ->Height(),0,0,m_rtBarHandle,TRUE);

		 
		{
			if(m_iBarHeadState ==1 ) ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMLEFTDOWNV,pDC->GetSafeHdc(),m_rtLeftHead.left,m_rtLeftHead.top,ILD_TRANSPARENT);		
			else                     ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMLEFTUPV,  pDC->GetSafeHdc(),m_rtLeftHead.left,m_rtLeftHead.top,ILD_TRANSPARENT);


			if(m_iBarHeadState ==2 ) ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMRIGHTDOWNV,pDC->GetSafeHdc(),m_rtRightHead.left,m_rtRightHead.top,ILD_TRANSPARENT);	 
			else                     ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMRIGHTUPV,  pDC->GetSafeHdc(),m_rtRightHead.left,m_rtRightHead.top,ILD_TRANSPARENT);	
		}
	}
	else
	{
		m_rtBarTotal.left  += TP_DoubleToInt (m_dStartZoomIn  *  m_iTimeCodeWidth);
		m_rtBarTotal.right  = rectBar.left +  TP_DoubleToInt ((m_dStartZoomIn + m_dZoomIn) *  m_iTimeCodeWidth);

		m_rtLeftHead        = m_rtBarTotal;  //左边头部位置
		m_rtLeftHead.right  = m_rtLeftHead.left + 8;//m_pResBar[TL_RES_BARLEFTUP] ->Width();	

		m_rtRightHead        = m_rtBarTotal;  //右边头部位置
		m_rtRightHead.left   = m_rtRightHead.right - 7;//m_pResBar[TL_RES_BARRIGHTUP] ->Width();	

		m_rtBarHandle        = m_rtBarTotal;  //滑动条位置
		m_rtBarHandle.left   = m_rtLeftHead.right;
		m_rtBarHandle.right  = m_rtRightHead.left;

		if(m_rtLeftHead.right>=m_rtRightHead.left)
		{
			m_rtRightHead = m_rtLeftHead;
			m_rtRightHead.OffsetRect(/*m_pResBar[TL_RES_BARLEFTUP] ->Width()*/ 8 *2,0);
			m_rtBarTotal  = CRect(m_rtLeftHead.left,m_rtLeftHead.top,m_rtRightHead.right,m_rtRightHead.bottom);
			m_rtBarHandle = CRect(0,0,0,0);
		}

		if(m_rtBarHandle.Width()>2)
			TP_StretchDIBRect(pDC,m_pResBarBack ->GetByte(),m_pResBarBack ->Width(),m_pResBarBack ->Height(),0,0,m_rtBarHandle);

		 
		{
			if(m_iBarHeadState ==1 ) ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMLEFTDOWN,pDC->GetSafeHdc(),m_rtLeftHead.left,m_rtLeftHead.top,ILD_TRANSPARENT);		
			else                     ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMLEFTUP,  pDC->GetSafeHdc(),m_rtLeftHead.left,m_rtLeftHead.top,ILD_TRANSPARENT);


			if(m_iBarHeadState ==2 ) ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMRIGHTDOWN,pDC->GetSafeHdc(),m_rtRightHead.left,m_rtRightHead.top,ILD_TRANSPARENT);	 
			else                     ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMRIGHTUP,  pDC->GetSafeHdc(),m_rtRightHead.left,m_rtRightHead.top,ILD_TRANSPARENT);	
		}
	}
}


 
 
 
 
 
double CTPScrollBarZoom::TranslateFrame(long double dFrame) const
{
	if(m_bVert)
	return   ((dFrame - ((m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame)) * m_dPixelPerFrame)+m_rtTimeBar.top ;
	else
	return  ((dFrame - ((m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame)) * m_dPixelPerFrame)+m_rtTimeBar.left;

 
}
long double CTPScrollBarZoom::TranslatePixel(double dPixel ) const
{
	long double dValue = 0;		
	if(m_bVert)
		dValue = ((dPixel-  m_rtTimeBar.top) / m_dPixelPerFrame +  ((m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame));
	else 
		dValue = ((dPixel-  m_rtTimeBar.left) / m_dPixelPerFrame +  ((m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame));
	dValue = max(dValue,m_nMinFrame);
	return dValue;

}
 

BOOL CTPScrollBarZoom::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if( (m_uBaseState &TP_TLWND_RBUTTON))
	{
		SetCursor(m_pResCursor[TL_ARROW] ->GetCursor());
		return TRUE;
	}

	else if(m_enDragType == TP_DRAG_UNKNOW)
	{
 		CPoint  ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);		
		m_hSetCursor = NULL;
		 
		if(m_rtLeftHead.PtInRect(ptCursor) )                  m_hSetCursor = m_pResCursor[TL_LEFT]->GetCursor();
		else if(m_rtRightHead.PtInRect(ptCursor) )            m_hSetCursor = m_pResCursor[TL_RIGHT]->GetCursor();
		//else if(m_rtBarHandle.PtInRect(ptCursor) )            m_hSetCursor = m_pResCursor[TL_HANDLE]->GetCursor();
		 
 		if(m_hSetCursor ==NULL)		m_hSetCursor = m_pResCursor[TL_ARROW] ->GetCursor();
		SetCursor(m_hSetCursor);		 
		return TRUE;		
	}
	else 	
	{
		SetCursor(m_hSetCursor);
		return TRUE;		
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);

}

 
BOOL CTPScrollBarZoom::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
 
void CTPScrollBarZoom::OnCaptureChanged(CWnd *pWnd)
{
	CWnd::OnCaptureChanged(pWnd);
	if(m_uBaseState &TP_DRAG_CAPTURE)
	{
		if(m_enDragType != TP_DRAG_UNKNOW)
		{
			m_enDragType = TP_DRAG_UNKNOW;
			SetCursor(m_pResCursor[TL_ARROW] ->GetCursor()); 
			Invalidate();
		}
		m_uBaseState &= ~TP_DRAG_CAPTURE;		
	}
}
 
void CTPScrollBarZoom::OnLButtonDown(UINT nFlags, CPoint point) 
{	 
	if(m_enDragType != TP_DRAG_UNKNOW) return;
	if(GetFocus()!=this) SetFocus();
	m_uBaseState &=  ~TP_TLWND_BUTTON; 
	m_uBaseState |=  TP_TLWND_LBUTTON;
 
	if(  m_rtLeftHead.PtInRect(point) )
	{
		m_uBaseState |= TP_DRAG_CAPTURE;
		SetCapture();
		m_enDragType         = TP_DRAG_LEFT;
		m_ptDrag             = point;
		m_rtCursorArea       = GetTimeBarRect();
		if(m_bVert)
		{
			m_iDrag              = point.y - m_rtLeftHead.top;
			m_rtCursorArea.top    = 0;
			m_rtCursorArea.bottom = m_rtRightHead.top - (m_rtLeftHead.Height()- (long)m_iDrag);
		}
		else
		{
			m_iDrag              = point.x - m_rtLeftHead.left;
			m_rtCursorArea.left  = 0;
			m_rtCursorArea.right = m_rtRightHead.left - (m_rtLeftHead.Width()- (long)m_iDrag);
		}
		SetCursor(m_pResCursor[TL_LEFT] ->GetCursor());
		return;
	}
	//滑动条右边
	else if(  m_rtRightHead.PtInRect(point))
	{ 
		m_uBaseState |= TP_DRAG_CAPTURE;
		SetCapture();
		m_enDragType         =  TP_DRAG_RIGHT;
		m_ptDrag             =  point;
		m_rtCursorArea       =  GetTimeBarRect();
		if(m_bVert)
		{
			m_iDrag              =  point.y - m_rtRightHead.top;
			m_rtCursorArea.top   =  m_rtLeftHead.bottom + (long)m_iDrag;
			m_rtCursorArea.right =  0x7ffff;
		}
		else
		{
			m_iDrag              =  point.x - m_rtRightHead.left;
			m_rtCursorArea.left  =  m_rtLeftHead.right + (long)m_iDrag;
			m_rtCursorArea.right =  0x7ffff;
		}
		SetCursor(m_pResCursor[TL_RIGHT] ->GetCursor());
		return;
	}
	//中间滑动条
	else if( m_rtBarHandle.PtInRect(point))
	{ 
		m_uBaseState |= TP_DRAG_CAPTURE;
		SetCapture();		
		m_enDragType         = TP_DRAG_BAR;
		m_ptDrag             = point;
		m_rtCursorArea       = GetTimeBarRect();
		if(m_bVert)
		{
			m_iDrag              = point.y - m_rtLeftHead.top;
			m_rtCursorArea.top  +=  point.y - m_rtLeftHead.top;
			m_rtCursorArea.bottom -=  m_rtRightHead.bottom - point.y;
		}
		else
		{
			m_iDrag              = point.x - m_rtLeftHead.left;
			m_rtCursorArea.left  +=  point.x - m_rtLeftHead.left;
			m_rtCursorArea.right -=  m_rtRightHead.right - point.x;
		}

		m_hSetCursor = m_pResCursor[TL_HANDLE]->GetCursor();
		SetCursor(m_hSetCursor);
		return;
	} 

	return ;
}
 
void CTPScrollBarZoom::OnLButtonUp(UINT nFlags, CPoint point) 
{
 
	if(!(m_uBaseState & (TP_TLWND_LBUTTON |TP_TLWND_LDBBUTTON))) return;
	m_uBaseState &= ~(TP_TLWND_LBUTTON |TP_TLWND_LDBBUTTON);
	if(m_enDragType != TP_DRAG_UNKNOW)		TP_ClipPoint(m_rtCursorArea,point);
  	if(	m_enDragType == TP_DRAG_LEFTOK)
	{
		if(m_ptDrag.x != point.x && m_rtRightHead.Width()>0 && !m_bVert || m_ptDrag.y != point.y && m_rtRightHead.Height()>0 && m_bVert)	
		{
			if(m_bVert)
				SetZoomIn((m_rtRightHead.bottom - (point.y  - m_iDrag))*1.0/m_iTimeCodeWidth,TRUE,TRUE);
			else
				SetZoomIn((m_rtRightHead.right - (point.x  - m_iDrag))*1.0/m_iTimeCodeWidth,TRUE,TRUE);
			m_pMsgWnd ->SendMessage(WM_SCROLLBARZOOM,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);
		}
		m_iBarHeadState = 0;
	}
	else if(m_enDragType == TP_DRAG_RIGHTOK)
	{ 
		if(m_ptDrag.x != point.x && m_rtRightHead.Width()>0 && !m_bVert  || m_ptDrag.y != point.y && m_rtRightHead.Height()>0 && m_bVert)	
		{
			if(m_bVert)
				SetZoomIn((point.y  - m_iDrag + m_rtRightHead.Height() - m_rtLeftHead.top)*1.0/m_iTimeCodeWidth,TRUE,TRUE);
			else
				SetZoomIn((point.x  - m_iDrag + m_rtRightHead.Width() - m_rtLeftHead.left)*1.0/m_iTimeCodeWidth,TRUE,TRUE);
			m_pMsgWnd ->SendMessage(WM_SCROLLBARZOOM,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);			
		}		
		m_iBarHeadState = 0;	 
	}
	else if(m_enDragType == TP_DRAG_BAROK)
	{ 
		if(m_ptDrag.x != point.x && !m_bVert || m_ptDrag.y != point.y && m_bVert)	
		{
			if(m_bVert)
				SetStartZoomIn((point.y  - m_iDrag - m_rtTimeBar.top)*1.0/m_iTimeCodeWidth,TRUE);
			else
				SetStartZoomIn((point.x  - m_iDrag - m_rtTimeBar.left)*1.0/m_iTimeCodeWidth,TRUE);
			m_pMsgWnd ->SendMessage(WM_SCROLLBARZOOM,(WPARAM)TIME_CHANGE_STARTIN,(LPARAM)this);
		}
	}	
	 
	if(m_enDragType != TP_DRAG_UNKNOW)
	{
		m_iDrag      = -1;
		m_enDragType = TP_DRAG_UNKNOW;
		m_uBaseState &= ~TP_DRAG_CAPTURE;
 		SetCursor(m_pResCursor[TL_ARROW] ->GetCursor()); 
		ReleaseCapture();	
 		Invalidate();
	}
 }
void CTPScrollBarZoom::SetDBClick()
{
	if(m_bSaveClick)
	{
		m_bSaveClick = FALSE;

		double dStartZoomIn ,dZoomIn;
		dStartZoomIn  = (m_dSaveStartZoomIn - m_dStartZoomIn) /m_iDBClickStep;
		dZoomIn       = (m_dSaveZoomIn      - m_dZoomIn)      /m_iDBClickStep;

		for(int i=0;i<m_iDBClickStep;i++)
		{
			m_dStartZoomIn += dStartZoomIn;
			m_dZoomIn      +=  dZoomIn;
			m_dPixelPerFrame        = m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);	
			m_dStartFrame           = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;
			 
			Invalidate();
			UpdateWindow();
			::Sleep(10);
		}

		m_dStartZoomIn          =  m_dSaveStartZoomIn;
		m_dZoomIn               =  m_dSaveZoomIn;
		m_dPixelPerFrame        =  m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);	
		m_dStartFrame           =  (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;
		m_dSaveStartZoomIn      = 0.0;
		m_dSaveZoomIn           = 1.0;

		Invalidate();
	}
	else
	{
		m_bSaveClick = TRUE;

		m_dSaveStartZoomIn  = m_dStartZoomIn;
		m_dSaveZoomIn       = m_dZoomIn;


		double dStartZoomIn ,dZoomIn;
		dStartZoomIn  = m_dStartZoomIn /m_iDBClickStep;
		dZoomIn       = (1-m_dZoomIn)  /m_iDBClickStep;

		for(int i=0;i<m_iDBClickStep;i++)
		{
			m_dStartZoomIn -= dStartZoomIn;
			m_dZoomIn      +=  dZoomIn;
			m_dPixelPerFrame        =  m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);	
			m_dStartFrame           =  (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;

			 
			Invalidate();
			 UpdateWindow();
			::Sleep(10);
		}

		m_dStartZoomIn          =  0;
		m_dZoomIn               =  1.0;
		m_dPixelPerFrame        =  m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);	
		m_dStartFrame           =  (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;

		Invalidate();
	}

	
	m_pMsgWnd ->SendMessage(WM_SCROLLBARZOOM,TIME_CHANGE_ZOOMIN,(LPARAM)this);
}
void  CTPScrollBarZoom::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	SetDBClick();
}
void CTPScrollBarZoom::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);
 
	if(m_rtTimeBar.PtInRect(point) && m_rtLeftHead.right>=m_rtRightHead.left)
	{
		m_bSaveClick = FALSE;
		SetDBClick();
	}
	else if(m_rtBarHandle.PtInRect(point))
	{		
		SetDBClick();
	}	 
}

 
void CTPScrollBarZoom::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	m_bWndEnable = bEnable;
 	Invalidate();	
}
 

void CTPScrollBarZoom::OnMouseMove(UINT nFlags, CPoint point) 
{
		//	CWnd::OnMouseMove(nFlags, point);
	CPoint ptSave = point;
	if(GetKeyState(VK_LBUTTON)>=0) nFlags &=~MK_LBUTTON;
	if((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		if(m_enDragType != TP_DRAG_UNKNOW) TP_ClipPoint(m_rtCursorArea,point);
		if(m_enDragType == TP_DRAG_LEFTOK || m_enDragType == TP_DRAG_LEFT)
		{
			if(m_ptDrag.x != point.x && m_rtRightHead.Width()>0 && !m_bVert || m_ptDrag.y != point.y && m_rtRightHead.Height()>0 && m_bVert)	
			{
				m_enDragType = TP_DRAG_LEFTOK;
				if(m_bVert)
					SetZoomIn((m_rtRightHead.bottom - (point.y  - m_iDrag))*1.0/m_iTimeCodeWidth,TRUE,TRUE);
				else
					SetZoomIn((m_rtRightHead.right - (point.x  - m_iDrag))*1.0/m_iTimeCodeWidth,TRUE,TRUE);
				m_ptDrag = point;
				m_pMsgWnd ->SendMessage(WM_SCROLLBARZOOM,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);

			}
		}
		else if(m_enDragType == TP_DRAG_RIGHTOK || m_enDragType == TP_DRAG_RIGHT)
		{ 
			if(m_ptDrag.x != point.x && m_rtRightHead.Width()>0 && !m_bVert  || m_ptDrag.y != point.y && m_rtRightHead.Height()>0 && m_bVert)	
			{
				m_enDragType = TP_DRAG_RIGHTOK;
				if(m_bVert)
					SetZoomIn((point.y  - m_iDrag + m_rtRightHead.Height() - m_rtLeftHead.top)*1.0/m_iTimeCodeWidth,TRUE,TRUE);
				else
					SetZoomIn((point.x  - m_iDrag + m_rtRightHead.Width() - m_rtLeftHead.left)*1.0/m_iTimeCodeWidth,TRUE,TRUE);
				m_ptDrag = point;
				m_pMsgWnd ->SendMessage(WM_SCROLLBARZOOM,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);					 
			}		
		}
		else if(m_enDragType == TP_DRAG_BAROK || m_enDragType == TP_DRAG_BAR)
		{ 
			if(m_ptDrag.x != point.x && !m_bVert || m_ptDrag.y != point.y && m_bVert)	
			{
				if(m_enDragType == TP_DRAG_BAR)
				{
					SetCursor(m_pResCursor[TL_HANDLE] ->GetCursor()); 
					m_enDragType = TP_DRAG_BAROK;
				}
				if(m_bVert)
					SetStartZoomIn((point.y  - m_iDrag - m_rtTimeBar.left)*1.0/m_iTimeCodeWidth,TRUE);
				else 
					SetStartZoomIn((point.x  - m_iDrag - m_rtTimeBar.top)*1.0/m_iTimeCodeWidth,TRUE);
				m_ptDrag = point;
				m_pMsgWnd ->SendMessage(WM_SCROLLBARZOOM,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);
			}			
		}				 
	}	 
}

double CTPScrollBarZoom::GetViewFrame()
{
	return  m_iTimeCodeWidth / m_dPixelPerFrame;
}
BOOL CTPScrollBarZoom::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(m_enDragType != TP_DRAG_UNKNOW) return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	ScreenToClient(&pt);
	if( m_rtBarTotal.PtInRect(pt))
	{
		double dViewFrame  = GetViewFrame();
		double dStartFrame = m_dStartFrame;

		if(zDelta>0) dStartFrame -= dViewFrame/4;
		else         dStartFrame += dViewFrame/4;
		dStartFrame = max(m_nMinFrame,dStartFrame);
		dStartFrame = min(m_nMaxFrame-dViewFrame,dStartFrame);
		if((int)dStartFrame != (int)m_dStartFrame)
		{
			SetStartZoomIn(dStartFrame/(m_nMaxFrame - m_nMinFrame),TRUE);		 
			m_pMsgWnd ->SendMessage(WM_SCROLLBARZOOM,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);
		}
	}
	 
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
