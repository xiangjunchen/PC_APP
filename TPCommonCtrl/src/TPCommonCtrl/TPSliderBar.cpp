// TPSliderBar.cpp : 实现文件
//

#include "stdafx.h"
//#include "TPCommonCtrl.h"
//#include "TPSliderBar.h"
//#include ".\tpsliderbar.h"
#include <math.h>


// CTPSliderBar

IMPLEMENT_DYNAMIC(CTPSliderBar, CSliderCtrl)
CTPSliderBar::CTPSliderBar()
{	
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_pMsgWnd       = NULL;
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uStyle        = 0;
	m_eBorderStyle  = g_eBorderStyle;
	m_iBackColor    = COLOR_BASE_FRAME;

	m_iPage         = 100;
	m_iPagePixel    = 10;

	m_iTopLeft     = 3;
	m_iRightBottom = 3;
	m_iBarSize     = 0;
	
	m_iMin          = 0;
	m_iMax          = 100;
	m_iPos          = 0;

	m_bOver         = FALSE;
	m_bPressed      = FALSE;
	m_bPaint        = TRUE;

	m_rtThumb       = CRect(0,0,0,0);
	m_rtThumbHandle = CRect(0,0,0,0);
	m_rtChannel     = CRect(0,0,0,0);
	m_rtClient      = CRect(0,0,0,0);

 
	m_iDragMode     = TP_DRAG_UNKNOW;
	m_ptDragSave    = CPoint(0,0);
	m_iDragSave     = 0;

	m_hTimerHover   = NULL;

	m_pTicLinePen   = NULL;

	for(INT l=0;l<SLIDER_MAX;l++)
		m_pThumbBmp[l] = NULL;
	m_pBackRes = NULL;

	m_iChanneOffset = 0;
	m_bFoceHideBar  = FALSE;
	m_uState        = 0;
	m_bReDraw       = TRUE;
	m_ptDrag        = CPoint(0,0);
	m_dwDragTime    = 0;	

	m_pdStepTicks = NULL;
	m_pdStepValue = NULL;
	m_iStepCount  = 0;
	m_iMicType    = 1;
	m_iDimLen     = -1;
}

CTPSliderBar::~CTPSliderBar()
{
	for(INT l=0;l<SLIDER_MAX;l++)
		TP_ReleaseCommonRes(m_pThumbBmp[l]);
	TP_ReleaseCommonRes(m_pTicLinePen);	
	TP_ReleaseCommonRes(m_pBackRes);		

	if(m_pdStepTicks) delete [] m_pdStepTicks; 
	m_pdStepTicks = NULL;
	if(m_pdStepValue) delete [] m_pdStepValue;
	m_pdStepValue = NULL;
}
void     CTPSliderBar::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),TRACKBAR_CLASS,&wndClass);		
		wndClass.lpszClassName = WC_SLIDERBAR;
		wndClass.style        |= CS_DBLCLKS;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_SLIDERBAR,AfxGetInstanceHandle());
}
BEGIN_MESSAGE_MAP(CTPSliderBar, CSliderCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()

	ON_MESSAGE(WM_UNSUBCLASSWINDOW,OnMsgUnSubWnd)

END_MESSAGE_MAP()



// CTPSliderBar 消息处理程序


void CTPSliderBar::LoadThumb()
{
	if(!(TBS_NOTHUMB & m_uStyle)) 
	{
		CString sThumbName = _L("");
		if(m_uStyle & TBS_VERT)	        sThumbName = _L("V");
		else          			        sThumbName = _L("H");

		m_pThumbBmp[SLIDER_OVER]      =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderBar\\slider_") + sThumbName +_L("_over.bmp"));
		m_pThumbBmp[SLIDER_DIS]       =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderBar\\slider_") + sThumbName +_L("_Dis.bmp"));
		m_pThumbBmp[SLIDER_NOR]       =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderBar\\slider_") + sThumbName +_L("_Nor.bmp"));
		m_pThumbBmp[SLIDER_DOWN]      =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderBar\\slider_") + sThumbName +_L("_Down.bmp"));

		m_pThumbBmp[SLIDER_THUMB_OVER] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderBar\\slider_") + sThumbName +_L("_Thumb_over.bmp"));
		m_pThumbBmp[SLIDER_THUMB_DIS]  =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderBar\\slider_") + sThumbName +_L("_Thumb_Dis.bmp"));
		m_pThumbBmp[SLIDER_THUMB_NOR]  =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderBar\\slider_") + sThumbName +_L("_Thumb_Nor.bmp"));
		m_pThumbBmp[SLIDER_THUMB_DOWN] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderBar\\slider_") + sThumbName +_L("_Thumb_Down.bmp"));

		m_pThumbBmp[SLIDER_BACK] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderBar\\slider_") + sThumbName +_L("_Back.bmp"));

		m_pTicLinePen    = TP_LoadPen(COLOR_BASE_TEXT,PS_SOLID,1);
	}	
}
void CTPSliderBar::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		if(m_pMsgWnd == NULL) m_pMsgWnd = GetParent();
		m_uStyle = CSliderCtrl::GetStyle(); 
		LoadThumb();		
		CalcSize(SLIDER_CHANNEL);		
		EnableToolTips(TRUE);


		if(m_dwWindowLevel == TPWNDLEVEL1)      m_iBackColor = COLOR_BASE_FRAME;
		else if(m_dwWindowLevel == TPWNDLEVEL2) m_iBackColor = COLOR_BASE_FRAME2;
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
		if(m_hTimerHover)	KillTimer(m_hTimerHover);
		m_hTimerHover = 0;
	}
}
BOOL CTPSliderBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,UINT nID)
{
	// initialize common controls
	//VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

	CWnd* pWnd = this;
	return pWnd->Create(WC_SLIDERBAR, NULL, dwStyle, rect, pParentWnd, nID);
}

LRESULT CTPSliderBar::OnMsgUnSubWnd(WPARAM wp,LPARAM lp)
{
	SubWindow(false);
	return S_OK;
}
int CTPSliderBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSliderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);	
	return 0;
}
void CTPSliderBar::OnDestroy()
{
	SubWindow(FALSE);
	CSliderCtrl::OnDestroy();
}
BOOL CTPSliderBar::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;

	return CSliderCtrl::PreCreateWindow(cs);
}

void CTPSliderBar::PreSubclassWindow()
{
	CSliderCtrl::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

BOOL CTPSliderBar::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CSliderCtrl::OnEraseBkgnd(pDC);
}
void CTPSliderBar::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);	
}
void CTPSliderBar::PaintTicks(CDC *pDC)
{

	if(!(TBS_NOTICKS & m_uStyle))  //Tic
	{
		int  iTick     = CSliderCtrl::GetNumTics();
		int  iPos      = 0;
		CPen *pSavePen = pDC->SelectObject(CPen::FromHandle(m_pTicLinePen ->hPen));
		int  iYPos     = 0;
		if(m_uStyle & TBS_VERT)
		{
			if(m_uStyle & TBS_LEFT)    iYPos = m_rtThumb.left   - 2; 
			else                       iYPos = m_rtThumb.right  + 2;
		}
		else
		{		
			if(m_uStyle & TBS_TOP)    iYPos = m_rtThumb.top    - 2; 
			else                      iYPos = m_rtThumb.bottom + 2;
		}
		iTick = min(iTick,20);
		for(INT l=0;l<iTick;l++)
		{
			if(m_uStyle & TBS_VERT)
			{
				iPos = m_rtChannel.top +m_rtThumb.Height()/2 + (m_rtChannel.Height() - m_rtThumb.Height())* l /(iTick-1);		

				if(m_uStyle & TBS_LEFT) 
				{
					pDC->MoveTo(iYPos,iPos);pDC->LineTo((l == 0 || l == iTick - 1) ?  iYPos-3 : iYPos-2,iPos);		
				}
				else
				{
					pDC->MoveTo(iYPos,iPos);pDC->LineTo((l == 0 || l == iTick - 1) ?  iYPos+3 : iYPos+2,iPos);		
				}
			}
			else
			{
				iPos = m_rtChannel.left +m_rtThumb.Width()/2 + (m_rtChannel.Width() - m_rtThumb.Width())* l /(iTick-1);		
				if(m_uStyle & TBS_TOP) 
				{
					pDC->MoveTo(iPos,iYPos);pDC->LineTo(iPos,(l == 0 || l == iTick - 1) ?  iYPos-3 : iYPos-2);		
				}
				else
				{
					pDC->MoveTo(iPos,iYPos);pDC->LineTo(iPos,(l == 0 || l == iTick - 1) ?  iYPos+3 : iYPos+2);		
				}
			}
		}
		pSavePen = pDC->SelectObject(pSavePen);
	}
}

void CTPSliderBar::PaintCtrl(CDC *pDC)
{
	CalcSize(SLIDER_THUMB);		

	int iIndexChannel,iThumbIndex,iIndex =0;	
	if(m_bPressed)				{iIndexChannel  = SLIDER_DOWN ;  iThumbIndex  = SLIDER_THUMB_DOWN; }
	else if(m_bOver)			{iIndexChannel  = SLIDER_OVER ;  iThumbIndex  = SLIDER_THUMB_OVER; }	
	else if(!IsWindowEnabled())	{iIndexChannel  = SLIDER_DIS ;   iThumbIndex  = SLIDER_THUMB_DIS;  }
	else						{iIndexChannel  = SLIDER_NOR ;   iThumbIndex  = SLIDER_THUMB_NOR;  }

	if(m_uStyle & TBS_VERT)
	{
		iIndex = SLIDER_BACK;
		TP_StretchDIBRect(pDC,m_pThumbBmp[iIndex] ->GetByte(),m_pThumbBmp[iIndex]->Width(),m_pThumbBmp[iIndex]->Height(),5,5,m_rtChannel,TRUE,TRUE);
		iIndex = iIndexChannel;
		TP_StretchDIBRect(pDC,m_pThumbBmp[iIndex] ->GetByte(),m_pThumbBmp[iIndex]->Width(),m_pThumbBmp[iIndex]->Height(),5,5,m_rtThumb,TRUE,TRUE);
		if(m_rtThumbHandle.bottom > m_rtThumbHandle.top + 4 && m_pThumbBmp[iIndex])
		{
			iIndex = iThumbIndex;
			TP_StretchDIBRect(pDC,m_pThumbBmp[iIndex] ->GetByte(),m_pThumbBmp[iIndex]->Width(),m_pThumbBmp[iIndex]->Height(),0,0,m_rtThumbHandle,TRUE,TRUE);
		}
	}
	else
	{
		iIndex = SLIDER_BACK;
		TP_StretchDIBRect(pDC,m_pThumbBmp[iIndex] ->GetByte(),m_pThumbBmp[iIndex]->Width(),m_pThumbBmp[iIndex]->Height(),5,5,m_rtChannel,FALSE);
		iIndex = iIndexChannel;
		TP_StretchDIBRect(pDC,m_pThumbBmp[iIndex] ->GetByte(),m_pThumbBmp[iIndex]->Width(),m_pThumbBmp[iIndex]->Height(),5,5,m_rtThumb,FALSE);
		if(m_rtThumbHandle.right > m_rtThumbHandle.left + 4 && m_pThumbBmp[iIndex])
		{
			iIndex = iThumbIndex;
			TP_StretchDIBRect(pDC,m_pThumbBmp[iIndex] ->GetByte(),m_pThumbBmp[iIndex]->Width(),m_pThumbBmp[iIndex]->Height(),0,0,m_rtThumbHandle,FALSE);
		}
	}
	PaintTicks(pDC);
}
void CTPSliderBar::OnPaint()
{
	CPaintDC dc(this); 	
	if(!m_bPaint) return;	
	CTPMemoDC cdcMemo(&dc,m_rtClient.Width(),m_rtClient.Height());	
	cdcMemo.FillSolidRect(m_rtClient,TP_GetSysColor(m_iBackColor));	
	if(m_pBackRes && !(TP_SLIDERBAR_NOBACK & m_uState))
		TP_StretchDIBRect(&cdcMemo,m_pBackRes->GetByte(),m_pBackRes ->Width(),m_pBackRes ->Height(),5,5,m_rtClient,FALSE);
	if(!m_bFoceHideBar)	PaintCtrl(&cdcMemo);	
	dc.BitBlt(0,0,m_rtClient.Width(),m_rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	
}
int CTPSliderBar::GetPageSize() const
{
	return m_iPage;
}
void CTPSliderBar::SetPageSize(int iPageSize,BOOL bPaint)
{
	m_iPage = iPageSize;
	CSliderCtrl::SetPageSize(iPageSize);
	if(bPaint)	Invalidate(FALSE);	
}
void CTPSliderBar::SetEdge(int iTopLeft,int iRightBottom)
{
	m_iTopLeft     =  iTopLeft;
	m_iRightBottom =  iRightBottom;	
	CalcSize(0x0f);
}
void CTPSliderBar::SetBackRes(CString sResPath)
{
	if(m_pBackRes)
	{
		TP_ReleaseCommonRes(m_pBackRes);
		m_pBackRes = NULL;
	}
	m_pBackRes = TP_LoadBmpByte(sResPath);
}
int CTPSliderBar::PixelToPos(int iPixel)
{	
	int iPos = 0;
	if(m_uStyle & TBS_VERT)
	{
		if(m_iMax>m_iMin)	iPos = (m_rtChannel.bottom - m_iPagePixel - iPixel) * (m_iPage + m_iMax - m_iMin) / m_rtChannel.Height() + m_iMin;
		else iPos = m_iMin;
	}
	else
	{		
		if(m_iMax>m_iMin)	iPos =  TP_DoubleToInt( (iPixel - m_rtChannel.left) * (m_iPage + m_iMax - m_iMin) /m_rtChannel.Width() + m_iMin);
		else iPos = m_rtChannel.left;
	}
	iPos = max(iPos,m_iMin);
	iPos = min(iPos,m_iMax);
	return iPos;
}
int CTPSliderBar::PosToPixel(int iPos)
{
	if(m_uStyle & TBS_VERT)
	{
		if(m_iMax>m_iMin)	return m_rtChannel.bottom - m_iPagePixel - TP_DoubleToInt( m_rtChannel.Height() *(iPos - m_iMin) /(m_iPage + m_iMax - m_iMin));
		else return m_rtChannel.top;
	}
	else
	{		
		if(m_iMax>m_iMin)	return m_rtChannel.left +  TP_DoubleToInt( m_rtChannel.Width() *(iPos - m_iMin) /(m_iPage + m_iMax - m_iMin));
		else return m_rtChannel.left;
	}
}

void   CTPSliderBar::GetChannelRect(CRect &rtChannel)
{
	GetClientRect(&rtChannel);
	if(m_pThumbBmp[SLIDER_BACK] == NULL) return;
	if(m_uStyle & TBS_VERT)
	{		
		int iWidth  = m_iBarSize >0 ? m_iBarSize : m_pThumbBmp[SLIDER_BACK]  ->Width();
		rtChannel.top    += m_iTopLeft;
		rtChannel.bottom -= m_iRightBottom;
		if(m_uStyle & TBS_BOTH)
		{		
			rtChannel.left    = m_rtClient.left + (m_rtClient.Width() - iWidth )/2;
			rtChannel.right   = rtChannel.left +  iWidth;
		}
		else if(m_uStyle & TBS_RIGHT)
		{
			rtChannel.left    = m_rtClient.right - iWidth;
			rtChannel.right   = m_rtClient.right;
		}
		else 
		{
			rtChannel.left    = m_rtClient.left;
			rtChannel.right = m_rtClient.left +  iWidth;		
		}
		rtChannel.OffsetRect(m_iChanneOffset,0);
	}
	else
	{			
		int iHeight  = m_iBarSize >0 ? m_iBarSize : m_pThumbBmp[SLIDER_BACK]  ->Height();
		rtChannel.left   += m_iTopLeft;
		rtChannel.right  -= m_iRightBottom;
		if(m_uStyle & TBS_BOTH)
		{
			rtChannel.top    = m_rtClient.top + (m_rtClient.Height() - iHeight)/2;
			rtChannel.bottom = rtChannel.top +  iHeight;
		}
		else if(m_uStyle & TBS_BOTTOM)
		{
			rtChannel.top    = m_rtClient.bottom - iHeight;
			rtChannel.bottom = m_rtClient.bottom;			
		}
		else
		{
			rtChannel.top    = m_rtClient.top;
			rtChannel.bottom = m_rtClient.top +  iHeight;			
		}	
		rtChannel.OffsetRect(0,m_iChanneOffset);
	}
}

void   CTPSliderBar::CalcSize(UINT uFlag)
{
	if(uFlag & SLIDER_CHANNEL)
	{
		GetClientRect(&m_rtClient);
		GetChannelRect(m_rtChannel);	
		m_rtThumb = m_rtChannel;		
		if(m_uStyle & TBS_VERT)
		{
//			m_rtThumb.left      = m_rtChannel.left + (m_pThumbBmp[SLIDER_BACK]  ->Width() - m_pThumbBmp[SLIDER_NOR]  ->Width())/2;
//			m_rtThumb.right     = m_rtThumb.left +  m_pThumbBmp[SLIDER_NOR]  ->Width();
		}
		else 
		{
//			m_rtThumb.top      = m_rtChannel.top + (m_pThumbBmp[SLIDER_BACK]  ->Height() - m_pThumbBmp[SLIDER_NOR]  ->Height())/2;
//			m_rtThumb.bottom   = m_rtThumb.top +  m_pThumbBmp[SLIDER_NOR]  ->Height();
		}
	}
	else if(uFlag & SLIDER_THUMB)
	{
		CSliderCtrl::GetRange(m_iMin,m_iMax);
		m_iPos  = CSliderCtrl::GetPos();
		m_iPage = CSliderCtrl::GetPageSize();

		if(m_uStyle & TBS_VERT)
		{			
			if(m_iMax > m_iMin)
			{
				m_iPagePixel     = m_rtChannel.Height() * m_iPage / (m_iPage + m_iMax - m_iMin);
				m_rtThumb.top    = PosToPixel(m_iPos);
				m_rtThumb.bottom = m_rtThumb.top + m_iPagePixel;
			}
			else
			{
				m_rtThumb.left  = m_rtChannel.left;
				m_rtThumb.right = m_rtChannel.right;
				m_iPagePixel    = m_rtChannel.Width();
			}
			if(m_pThumbBmp[SLIDER_THUMB_NOR])
			{			
				m_rtThumbHandle        = m_rtThumb;
				m_rtThumbHandle.top    = m_rtThumb.top + (m_rtThumb.Height() - m_pThumbBmp[SLIDER_THUMB_NOR] ->Height())/2;
				m_rtThumbHandle.bottom = m_rtThumbHandle.top + m_pThumbBmp[SLIDER_THUMB_NOR] ->Height();
				m_rtThumbHandle.top    = max(m_rtThumb.top    + 6,m_rtThumbHandle.top);
				m_rtThumbHandle.bottom = min(m_rtThumb.bottom - 6,m_rtThumbHandle.bottom);
			}
		}
		else
		{
			if(m_iMax > m_iMin)
			{
				m_iPagePixel    = m_rtChannel.Width() * m_iPage / (m_iPage + m_iMax - m_iMin);
				m_rtThumb.left  = PosToPixel(m_iPos);
				m_rtThumb.right = m_rtThumb.left + m_iPagePixel;
			}
			else
			{
				m_rtThumb.left  = m_rtChannel.left;
				m_rtThumb.right = m_rtChannel.right;
				m_iPagePixel    = m_rtChannel.Width();
			}
			if(m_pThumbBmp[SLIDER_THUMB_NOR])
			{			
				m_rtThumbHandle = m_rtThumb;
				m_rtThumbHandle.left  = m_rtThumb.left + (m_rtThumb.Width() - m_pThumbBmp[SLIDER_THUMB_NOR] ->Width())/2;
				m_rtThumbHandle.right = m_rtThumbHandle.left + m_pThumbBmp[SLIDER_THUMB_NOR] ->Width();
				m_rtThumbHandle.left   = max(m_rtThumb.left + 6,m_rtThumbHandle.left);
				m_rtThumbHandle.right  = min(m_rtThumb.right- 6,m_rtThumbHandle.right);
			}
		}
	}
}
void CTPSliderBar::OnSize(UINT nType, int cx, int cy)
{
	CSliderCtrl::OnSize(nType, cx, cy);
	if(m_bSubWindow) CalcSize(0x01);
}
void CTPSliderBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bFoceHideBar) return;
	if(GetFocus() != this) SetFocus();

	m_ptDrag = point;

	CSliderCtrl::GetRange(m_iMin,m_iMax);
	m_iPos  = CSliderCtrl::GetPos();
	int iCurPos = m_iPos;
	if(!(m_uStyle & TBS_VERT))
	{		
		if(m_rtThumb.PtInRect(point))
		{ 
			m_bPressed  = TRUE;
			m_iDragMode = TP_DRAG_MIDDLE;
			SetCapture();		
			m_ptDragSave = point;
			m_iDragSave  = point.x - m_rtThumb.left ;
			SetCapture();
			Invalidate();
		}
		else if(point.x < m_rtThumb.left)
		{
			if(m_iPos>m_iMin)
			{			
				iCurPos -= m_iPage;
				iCurPos = max(iCurPos,m_iMin);	
				if(iCurPos != m_iPos)
				{
					SetPos(iCurPos);		
					m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGELEFT,(WORD)iCurPos),(LPARAM)m_hWnd);				
				}
			}
		}
		else if(point.x >m_rtThumb.right)
		{
			if(m_iPos < m_iMax)
			{				
				iCurPos += m_iPage;
				iCurPos = min(iCurPos,m_iMax);				
				if(iCurPos != m_iPos)
				{
					SetPos(iCurPos);		
					m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGERIGHT,(WORD)iCurPos),(LPARAM)m_hWnd);	
				}
			}
		}
	}
	else
	{		
		if(m_rtThumb.PtInRect(point))
		{ 
			m_bPressed   = TRUE;
			m_iDragMode  = TP_DRAG_MIDDLE;			
			m_ptDragSave = point;
			m_iDragSave  = point.y - m_rtThumb.top;
			SetCapture();
			Invalidate();
		}
		else if(point.y  < m_rtThumb.top)
		{
			if(iCurPos<m_iMax)
			{
				iCurPos += m_iPage;
				iCurPos = min(iCurPos,m_iMax);				
				if(iCurPos != m_iPos)
				{
					SetPos(iCurPos);		
					m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEUP,(WORD)iCurPos),(LPARAM)m_hWnd);				
				}
			}
		}
		else if(point.y  > m_rtThumb.bottom)
		{
			if(iCurPos > m_iMin)
			{				
				iCurPos -= m_iPage;
				iCurPos = max(iCurPos,m_iMin);				
				if(iCurPos != m_iPos)
				{
					SetPos(iCurPos);		
					m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEDOWN,(WORD)iCurPos),(LPARAM)m_hWnd);				
				}
			}
		}
	}		
}

void CTPSliderBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CSliderCtrl::GetRange(m_iMin,m_iMax);
	m_iPos  = CSliderCtrl::GetPos();
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
					if(point.x  - m_iDragSave  <= m_rtChannel.left) 
					{
						if(m_iPos>m_iMin) iDragPos = m_iMin;
					}												
					else if(point.x  - m_iDragSave + m_iPagePixel >m_rtChannel.right) 
					{
						if(m_iPos<m_iMax) iDragPos = m_iMax;						
					}			
					else
					{				
						iDragPos = PixelToPos(point.x  - m_iDragSave);
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
					if(point.y  - m_iDragSave  <= m_rtChannel.top) 
					{
						if(m_iPos<m_iMax) iDragPos = m_iMax;
					}												
					else if(point.y  - m_iDragSave + m_iPagePixel >m_rtChannel.bottom) 
					{
						if(m_iPos>m_iMin) iDragPos = m_iMin;						
					}			
					else
					{				
						iDragPos = PixelToPos(point.y  - m_iDragSave);
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
void CTPSliderBar::SetCursorPosition(CPoint &ptCur)
{
}
int CTPSliderBar::GetMicroSetp(int iDragPos,CPoint &ptCur)
{
	return iDragPos;
}
void CTPSliderBar::OnMouseMove(UINT nFlags, CPoint point) 
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
				if(point.x  - m_iDragSave  <= m_rtChannel.left) 
				{
					if(m_iPos>m_iMin) iDragPos = m_iMin;
				}												
				else if(point.x  - m_iDragSave + m_iPagePixel >m_rtChannel.right) 
				{
					if(m_iPos<m_iMax) iDragPos = m_iMax;						
				}			
				else
				{				
					iDragPos = PixelToPos(point.x  - m_iDragSave);
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
			BOOL bCtrl  = TP_IsCtrlKeyDown();
			BOOL bPaint = m_bPaint;
			m_iDragMode = TP_DRAG_MIDDLEOK;			
			if(m_ptDragSave.y != point.y)		
			{
				if(!bCtrl && point.y  - m_iDragSave  <= m_rtChannel.top) 
				{
					if(m_iPos < m_iMax) iDragPos = m_iMax;
				}												
				else if(!bCtrl && point.y  - m_iDragSave + m_iPagePixel >m_rtChannel.bottom) 
				{
					if(m_iPos > m_iMin) iDragPos = m_iMin;						
				}			
				else
				{						
					iDragPos = PixelToPos(point.y  - m_iDragSave);
					iDragPos = max(iDragPos,m_iMin);
					iDragPos = min(iDragPos,m_iMax);					
					if(bCtrl) 
					{
						TP_RemoveMessage(m_hWnd,WM_MOUSEMOVE);
						m_bPaint = FALSE;
						iDragPos = GetMicroSetp(iDragPos,point);	
					}
				}
				if(iDragPos != m_iPos)
				{
					m_iPos = iDragPos;
					SetPos(iDragPos);	
					m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)iDragPos),(LPARAM)m_hWnd);									
				}
				m_bPaint = bPaint;
				m_ptDrag = point;
				if(bCtrl) SetCursorPosition(m_ptDrag);
			}
			m_ptDragSave = point;	
			Invalidate(FALSE);			
		}
	}
	if(m_iDragMode == TP_DRAG_UNKNOW && !m_bOver)
	{
		CPoint point;
		::GetCursorPos(&point); 
		ScreenToClient(&point); 
		if(m_rtChannel.PtInRect(point))
		{
			m_bOver = TRUE;
			Invalidate(FALSE);
			m_hTimerHover = SetTimer(200,100,NULL);			
		}
	}		
}


void CTPSliderBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_SLIDERCTRLMSG,WM_SLIDERCTRL_LBD,(LPARAM)m_hWnd);
	CSliderCtrl::OnLButtonDblClk(nFlags, point);
}

void CTPSliderBar::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 200)
	{
		CPoint point;
		::GetCursorPos(&point); 
		ScreenToClient(&point); 
		if(!m_rtChannel.PtInRect(point))
		{
			m_bOver = FALSE;
			Invalidate(FALSE);
			KillTimer(m_hTimerHover);
			m_hTimerHover = 0;
		}
	}

	CSliderCtrl::OnTimer(nIDEvent);
}
void CTPSliderBar::SetRange(int nMin, int nMax, BOOL bRedraw)
{
	CSliderCtrl::SetRange(nMin,nMax,bRedraw);
	CSliderCtrl::GetRange(m_iMin,m_iMax);	
}
void CTPSliderBar::SetStepValue(double *pStepValue,long lStepCount,double *pdStepTicks)
{
	if(m_pdStepTicks) delete [] m_pdStepTicks; m_pdStepTicks = NULL;
	if(m_pdStepValue) delete [] m_pdStepValue; m_pdStepValue = NULL;
	m_iStepCount = lStepCount;
	if(pStepValue)
	{
		m_pdStepValue = new double[m_iStepCount + 1];
		memcpy(m_pdStepValue,pStepValue,m_iStepCount * sizeof(double));
	}
	if(pdStepTicks)
	{
		m_pdStepTicks = new double[m_iStepCount + 1];
		memcpy(m_pdStepTicks,pdStepTicks,m_iStepCount * sizeof(double));
	}
	CTPSliderBar::SetRange(int(m_pdStepValue[0] *1000),int(m_pdStepValue[m_iStepCount-1]*1000));
	CTPSliderBar::SetTicFreq(1000);
}
TPCommonRes    * CTPSliderBar::GetRes(SLIDER_INDEX eIndex)
{
	return m_pThumbBmp[eIndex];
}
double  CTPSliderBar::DBAlign(double dDB,int iDimLen)
{
	if(iDimLen<0) return dDB;
	int iAmp = int(pow((double)10,(double)iDimLen));
	dDB *= iAmp;
	dDB  = TP_DoubleToInt(dDB);
	return dDB/iAmp;
}
double  CTPSliderBar::PosToDB(double iPos)
{
	double dPos = (iPos - m_iMin) *1.0/(m_iMax-m_iMin);
	if(dPos<=  m_pdStepTicks[0])                    return m_pdStepValue[0];
	else if(dPos>= m_pdStepTicks[m_iStepCount -1])  return m_pdStepValue[m_iStepCount -1];
	for(INT l=0;l<m_iStepCount-1;l++)
	{
		if(dPos == m_pdStepTicks[l])  return m_pdStepValue[l];
		else if(dPos < m_pdStepTicks[l+1])
		{
			return m_pdStepValue[l] + (m_pdStepValue[l+1] - m_pdStepValue[l]) * (dPos - m_pdStepTicks[l])/(m_pdStepTicks[l+1]-m_pdStepTicks[l]);
		}
	}
	return 0;
}
double CTPSliderBar::DBToPos(double dDB)
{
	if(dDB<=m_pdStepValue[0])                   return m_iMin;
	else if(dDB>=m_pdStepValue[m_iStepCount-1]) return m_iMax;
	for(int i =0;i<m_iStepCount-1;i++)
	{
		if(dDB == m_pdStepValue[i]) return m_pdStepTicks[i] *(m_iMax -m_iMin) + m_iMin;
		else if(dDB < m_pdStepValue[i+1])
		{
			return ((dDB - m_pdStepValue[i])/(m_pdStepValue[i+1]-m_pdStepValue[i]) *(m_pdStepTicks[i+1]-m_pdStepTicks[i]) + m_pdStepTicks[i]) *(m_iMax -m_iMin) + m_iMin;
		}		
	}	
	return 0;
}
double fabs(double d)
{
	if(d>=0) return d;
	else     return -d;
}
double CTPSliderBar::DBToPosComplex(double dDB)
{
	double dFind = 0,dDistanc = 10000;
	double dValue = 0;
	GetRange(m_iMin,m_iMax);
	int iMin = (int)DBToPos(dDB -50);
	int iMax = (int)DBToPos(dDB +50);
	for(INT l=iMin;l<=iMax;l++)
	{
		dValue = PosToDB(l);
		if(fabs(dValue - dDB)<dDistanc)
		{
			dFind    = l;
			dDistanc = fabs(dValue - dDB);
		}		
	}
	return dFind;
}