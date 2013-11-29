// TPSliderBar.cpp : 实现文件
//

#include "stdafx.h"
 
#include "TPSliderCtrlExEx.h"
 
#include <math.h>


// CTPSliderCtrlExEx

IMPLEMENT_DYNAMIC(CTPSliderCtrlExEx, CTPSliderCtrl)
CTPSliderCtrlExEx::CTPSliderCtrlExEx()
{	
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_pMsgWnd       = NULL;
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uStyle        = 0;
 

  	
 
  
	m_bOver         = FALSE;
	m_bPressed      = FALSE;
	m_bPaint        = TRUE;

	m_rtThumb       = CRect(0,0,0,0);
 	m_rtChannel     = CRect(0,0,0,0);
 
 
	m_iDragMode     = TP_DRAG_UNKNOW;
	m_ptDragSave    = CPoint(0,0);
  
	 

 	m_uState        = 0;
	m_ptDrag        = CPoint(0,0);
	m_dwDragTime    = 0;	
 	m_dlMin         = 1 ;
	m_dlMax         = 100 ;
	m_bReserce      = FALSE;
 

 
}

CTPSliderCtrlExEx::~CTPSliderCtrlExEx()
{ 
}
void     CTPSliderCtrlExEx::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),TRACKBAR_CLASS,&wndClass);		
		wndClass.lpszClassName = WC_SLIDERCTRLEXEX;
		wndClass.style        |= CS_DBLCLKS;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_SLIDERCTRLEXEX,AfxGetInstanceHandle());
}
BEGIN_MESSAGE_MAP(CTPSliderCtrlExEx, CTPSliderCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
  	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
 
	ON_MESSAGE(WM_UNSUBCLASSWINDOW,OnMsgUnSubWnd)

END_MESSAGE_MAP()



// CTPSliderCtrlExEx 消息处理程序



void CTPSliderCtrlExEx::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		if(m_pMsgWnd == NULL) m_pMsgWnd = GetParent();
		m_uStyle = CTPSliderCtrl::GetStyle(); 
        CTPSliderCtrl::SubWindow (bSub);
 		m_bSubWindow = TRUE;
	}
	else
	{
		if(!m_bSubWindow) return;
 		CTPSliderCtrl::SubWindow (bSub);
		m_bSubWindow = FALSE;	
 	}
}
 

LRESULT CTPSliderCtrlExEx::OnMsgUnSubWnd(WPARAM wp,LPARAM lp)
{
	SubWindow(false);
	return S_OK;
}
int CTPSliderCtrlExEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPSliderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);	
	return 0;
}
void CTPSliderCtrlExEx::OnDestroy()
{
	SubWindow(FALSE);
	CTPSliderCtrl::OnDestroy();
}
BOOL CTPSliderCtrlExEx::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;

	return CTPSliderCtrl::PreCreateWindow(cs);
}

void CTPSliderCtrlExEx::PreSubclassWindow()
{
	CTPSliderCtrl::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

  
int CTPSliderCtrlExEx::PixelToPos(int iPixel)
{	
	int iPos = 0;
	if(m_uStyle & TBS_VERT)
	{
		if(m_iMax>m_iMin)	iPos = (m_rtChannel.bottom -  iPixel) * ( m_iMax - m_iMin) / m_rtChannel.Height() + m_iMin;
		else iPos = m_iMin;
	}
	else
	{		
		if(m_iMax>m_iMin)	iPos =  TP_DoubleToInt(double( (INT64)(iPixel - m_rtChannel.left-m_rtThumb.Width()/2) * ( m_iMax - m_iMin) /(m_rtChannel.Width() - m_rtThumb.Width())+ m_iMin));
		else iPos = m_rtChannel.left;
	}
	iPos = max(iPos,m_iMin);
	iPos = min(iPos,m_iMax);
	return iPos;
}
int CTPSliderCtrlExEx::PosToPixel(int iPos)
{
	if(m_uStyle & TBS_VERT)
	{
		if(m_iMax>m_iMin)	return m_rtChannel.bottom - TP_DoubleToInt( m_rtChannel.Height() *(iPos - m_iMin) /( m_iMax - m_iMin));
		else return m_rtChannel.top;
	}
	else
	{		
		if(m_iMax>m_iMin)	return m_rtChannel.left + m_rtThumb.Width()/2+ TP_DoubleToInt(double((INT64)(m_rtChannel.Width() - m_rtThumb.Width()) *(iPos - m_iMin) /(  m_iMax - m_iMin)));
		else return m_rtChannel.left;
	}
}
 
void CTPSliderCtrlExEx::OnSize(UINT nType, int cx, int cy)
{
	CTPSliderCtrl::OnSize(nType, cx, cy);
 }
void CTPSliderCtrlExEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(GetFocus() != this) SetFocus();

	m_ptDrag = point;

	CTPSliderCtrl::GetRange(m_iMin,m_iMax);
	m_iPos  = CTPSliderCtrl::GetPos();
	
	if(!(m_uStyle & TBS_VERT))
	{		
	 
		int iCurPos = PixelToPos(point.x);
		if(iCurPos != m_iPos)
		{					
			SetPos(iCurPos);	
			m_iPos = iCurPos;
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)iCurPos),(LPARAM)m_hWnd);						
		}
		m_bPressed  = TRUE;
		m_iDragMode = TP_DRAG_MIDDLE;
		m_ptDragSave = point;
 		SetCapture();
		Invalidate();
	 	 
	}
	else
	{		
		int iCurPos = PixelToPos(point.y);

		if(iCurPos != m_iPos)
		{					
			SetPos(iCurPos);	
			m_iPos = iCurPos;
			m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)iCurPos),(LPARAM)m_hWnd);						
		}

		m_bPressed   = TRUE;
		m_iDragMode  = TP_DRAG_MIDDLE;			
		m_ptDragSave = point;
 		SetCapture();
		Invalidate();
		 
	}		
}

void CTPSliderCtrlExEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTPSliderCtrl::GetRange(m_iMin,m_iMax);
	m_iPos  = CTPSliderCtrl::GetPos();
	int iDragPos = m_iPos;	
	m_bPressed  = FALSE;
	if(!(m_uStyle & TBS_VERT))
	{	
		if(m_iDragMode == TP_DRAG_MIDDLEOK || m_iDragMode == TP_DRAG_MIDDLE)
		{ 			
			if(m_iDragMode == TP_DRAG_MIDDLEOK)
			{	
				if(m_ptDragSave.x != point.x)		
				{
					if(point.x     <= m_rtChannel.left) 
					{
						if(m_iPos>m_iMin) iDragPos = m_iMin;
					}												
					else if(point.x     >m_rtChannel.right) 
					{
						if(m_iPos<m_iMax) iDragPos = m_iMax;						
					}			
					else
					{				
						iDragPos = PixelToPos(point.x );
						iDragPos = max(iDragPos,m_iMin);
						iDragPos = min(iDragPos,m_iMax);
					}

					if(iDragPos != m_iPos)
					{
						SetPos(iDragPos);		
						m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)iDragPos),(LPARAM)m_hWnd);					
					}
				}		
				m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD)0),(LPARAM)m_hWnd);							
			}
			m_iDragMode = TP_DRAG_UNKNOW;
			ReleaseCapture();
			Invalidate(FALSE);
		}			
	}
	else
	{
		if(m_iDragMode == TP_DRAG_MIDDLEOK || m_iDragMode == TP_DRAG_MIDDLE)
		{ 	
			if(m_iDragMode == TP_DRAG_MIDDLEOK)
			{
				if(m_ptDragSave.y != point.y)		
				{
					if(point.y    <= m_rtChannel.top) 
					{
						if(m_iPos<m_iMax) iDragPos = m_iMax;
					}												
					else if(point.y     >m_rtChannel.bottom) 
					{
						if(m_iPos>m_iMin) iDragPos = m_iMin;						
					}			
					else
					{				
						iDragPos = PixelToPos(point.y   );
						iDragPos = max(iDragPos,m_iMin);
						iDragPos = min(iDragPos,m_iMax);
					}
					if(iDragPos != m_iPos)
					{
						SetPos(iDragPos);		
						m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)iDragPos),(LPARAM)m_hWnd);					
					}
				}
				m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD)0),(LPARAM)m_hWnd);			
			}
			m_iDragMode = TP_DRAG_UNKNOW;
			ReleaseCapture();
			Invalidate(FALSE);
		}		
	}	
} 
void CTPSliderCtrlExEx::OnMouseMove(UINT nFlags, CPoint point) 
{	
	int  iDragPos = m_iPos;
	BOOL bAdd     = FALSE;
	if(!(m_uStyle & TBS_VERT))
	{
		if(m_iDragMode == TP_DRAG_MIDDLEOK || m_iDragMode == TP_DRAG_MIDDLE)
		{ 			
			m_iDragMode = TP_DRAG_MIDDLEOK;
			if(m_ptDragSave.x != point.x)		
			{
				if(point.x    <= m_rtChannel.left) 
				{
					if(m_iPos>m_iMin) iDragPos = m_iMin;
				}												
				else if(point.x     >m_rtChannel.right) 
				{
					if(m_iPos<m_iMax) iDragPos = m_iMax;						
				}			
				else
				{				
					iDragPos = PixelToPos(point.x  );
					iDragPos = max(iDragPos,m_iMin);
					iDragPos = min(iDragPos,m_iMax);
				}
				if(iDragPos != m_iPos)
				{					
					SetPos(iDragPos);	
					m_iPos = iDragPos;
					m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)iDragPos),(LPARAM)m_hWnd);						
				}
			}
			m_ptDragSave = point;				
			Invalidate(FALSE);			
		}
	}
	else
	{
		// by zhh on 2006-02-23
		if( m_iDragMode == TP_DRAG_MIDDLEOK || m_iDragMode == TP_DRAG_MIDDLE)
		{
			if( (nFlags & MK_LBUTTON) == 0 &&  m_iDragMode == TP_DRAG_MIDDLEOK)
			{
				m_bPressed  = FALSE;
				m_iDragMode = TP_DRAG_UNKNOW;
				ReleaseCapture();
			}
		}
		if(m_iDragMode == TP_DRAG_MIDDLEOK || m_iDragMode == TP_DRAG_MIDDLE)
		{ 
			 
			m_iDragMode = TP_DRAG_MIDDLEOK;			
			if(m_ptDragSave.y != point.y)		
			{
				if(  point.y   <= m_rtChannel.top) 
				{
					if(m_iPos < m_iMax) iDragPos = m_iMax;
				}												
				else if(  point.y     >m_rtChannel.bottom) 
				{
					if(m_iPos > m_iMin) iDragPos = m_iMin;						
				}			
				else
				{						
					iDragPos = PixelToPos(point.y  );
					iDragPos = max(iDragPos,m_iMin);
					iDragPos = min(iDragPos,m_iMax);					
					 
				}
				if(iDragPos != m_iPos)
				{
					m_iPos = iDragPos;
					SetPos(iDragPos);	
					m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)iDragPos),(LPARAM)m_hWnd);									
				}
 				m_ptDrag = point;
				 
			}
			m_ptDragSave = point;	
			Invalidate(FALSE);			
		}
	}
	if(m_iDragMode == TP_DRAG_UNKNOW && !m_bOver)
	{		 
		if(m_rtThumb.PtInRect(point))
		{
			m_bOver = TRUE;
			Invalidate(FALSE);
			if(m_hTimer == NULL) m_hTimer = (UINT)SetTimer(100,100,NULL);
		}
	}		
}


void CTPSliderCtrlExEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_SLIDERCTRLMSG,WM_SLIDERCTRL_LBD,(LPARAM)m_hWnd);
	CTPSliderCtrl::OnLButtonDblClk(nFlags, point);
}

int CTPSliderCtrlExEx::GetPos()
{
	return   CTPSliderCtrl::GetPos()  ;
}
void CTPSliderCtrlExEx::SetPos(int nPos)
{
	 CTPSliderCtrl::SetPos( nPos );
}
void CTPSliderCtrlExEx::SetRange(_In_ int nMin, _In_ int nMax, _In_ BOOL bRedraw )
{
	CTPSliderCtrl::SetRange( nMin,nMax,bRedraw );

}

