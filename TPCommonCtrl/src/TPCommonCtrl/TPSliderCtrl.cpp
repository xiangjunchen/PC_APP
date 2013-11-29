// TPSliderCtrl.cpp : 实现文件
//

#include "stdafx.h"
//#include "TPCommonCtrl.h"
//#include "TPSliderCtrl.h"
//#include ".\tpsliderctrl.h"


// CTPSliderCtrl

IMPLEMENT_DYNAMIC(CTPSliderCtrl, CSliderCtrl)
CTPSliderCtrl::CTPSliderCtrl()
{
	m_dwWindowLevel  = TPWNDLEVEL1; 
	m_eBackColor     = COLOR_BASE_FRAME;
	m_eChanelColor   = COLOR_BASE_WINDOW;
	m_cPaintTickType = 0;

	m_bSubWindow     = FALSE;
	m_bCreate        = FALSE;
	m_eBorderStyle   = g_eBorderStyle;
	m_pTicLinePen    = TP_LoadPen(COLOR_BASE_TEXT,PS_SOLID,1);

	m_bOver          = FALSE;
	m_bPressed       = FALSE;
	m_hTimer         = NULL;;

	m_iTickEdge      = 0;  
	m_clChannel      = RGB(0,0,0);
	m_clBack         = RGB(0,0,0);

	for(INT  l=0;l<4;l++)
		m_pThumbBmp[l]   = NULL;
	m_pBackRes      = NULL;
	m_iThumbType    = 0;

	
	m_bPaint        = TRUE;
	m_bDoubleClick  = FALSE;
	m_bCtrlApply    = FALSE;

	m_bEsc          = FALSE;
	m_iEscPos       = 0; 
	m_iPos          = 0;
	m_iGetPos       = 0;
	m_cMicro        = FALSE;
	m_sTooltips     = _T("");
	m_iLineSize     = 0;
	m_ptDrag        = CPoint(0,0); 	
	m_dwDragTime    = 0;
	m_dwMouseMove   = 80;
	m_rtChannel     = CRect(0,0,100,100);
	m_rtThumb       = CRect(0,0,10,10);
	m_iMinOffset    = 0;
}

CTPSliderCtrl::~CTPSliderCtrl()
{
	TP_ReleaseCommonRes(m_pTicLinePen);

	TP_ReleaseCommonRes(m_pBackRes);
	m_pBackRes = NULL;

	for(INT  l=0;l<4;l++)
		TP_ReleaseCommonRes(m_pThumbBmp[l]);
}


BEGIN_MESSAGE_MAP(CTPSliderCtrl, CSliderCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff,       OnToolTipNotify )
END_MESSAGE_MAP()
void CTPSliderCtrl::LoadThumb()
{
	if(m_pThumbBmp[0]!=NULL) return;

	if(!(TBS_NOTHUMB & m_uStyle)) 
	{
		CString sThumbName = _L("");
		if(m_uStyle & TBS_BOTH)		m_uStyle &= ~TBS_BOTH ;	
		UINT uStyle = m_uStyle;
		if(m_uStyle & TBS_VERT)
		{
			m_uStyle |= TBS_RIGHT;	
			if(m_uStyle & TBS_BOTH)        sThumbName = _L("Vert");
			else if(m_uStyle & TBS_LEFT)   sThumbName = _L("Left");
			else                           sThumbName = _L("Right");
		}
		else
		{
			m_uStyle |= TBS_TOP;	
			if(m_uStyle & TBS_BOTH)        sThumbName = _L("Horz");
			else if(m_uStyle & TBS_TOP)    sThumbName = _L("Top");
			else                           sThumbName = _L("Bottom");
		}
		m_uStyle = uStyle;

		

		if(m_iThumbType == 0)
		{		
			m_pThumbBmp[0] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider_") + sThumbName +_L("_over.bmp"));
			m_pThumbBmp[1] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider_") + sThumbName +_L("_Dis.bmp"));
			m_pThumbBmp[2] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider_") + sThumbName +_L("_Nor.bmp"));
			m_pThumbBmp[3] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider_") + sThumbName +_L("_Down.bmp"));				
		}
		else if(m_iThumbType == 1)
		{
			m_pThumbBmp[0] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider1_") + sThumbName +_L("_over.bmp"));
			m_pThumbBmp[1] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider1_") + sThumbName +_L("_Dis.bmp"));
			m_pThumbBmp[2] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider1_") + sThumbName +_L("_Nor.bmp"));
			m_pThumbBmp[3] =  TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider1_") + sThumbName +_L("_Down.bmp"));				
		}
	}
}
void     CTPSliderCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),TRACKBAR_CLASS,&wndClass);				
		wndClass.style        |= CS_DBLCLKS;
		::RegisterClass(&wndClass);
	}	
}
void CTPSliderCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		m_uStyle     = GetStyle();
		LoadThumb();
		if(m_dwWindowLevel == TPWNDLEVEL1)      m_eBackColor = COLOR_BASE_FRAME;
		else if(m_dwWindowLevel == TPWNDLEVEL2) m_eBackColor = COLOR_BASE_FRAME2;
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
		if(m_hTimer) KillTimer(m_hTimer);
		m_hTimer = NULL;		
	}
}
int CTPSliderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSliderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SubWindow(TRUE);
	return 0;
}

void CTPSliderCtrl::OnDestroy()
{
	CSliderCtrl::OnDestroy();
	SubWindow(FALSE);	
}
BOOL CTPSliderCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bCreate = TRUE;

	return CSliderCtrl::PreCreateWindow(cs);
}
void CTPSliderCtrl::PreSubclassWindow()
{
	CSliderCtrl::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}
void CTPSliderCtrl::PaintTicks1(CDC *pDC)
{
	CPen *pSavePen = pDC->SelectObject(CPen::FromHandle(m_pTicLinePen ->hPen));
	int        iYPos     = 0,iPos = 0;
	CRect      rtThumb;
	CSliderCtrl::GetThumbRect(&rtThumb);
	iYPos = rtThumb.bottom    + m_iTickEdge;
	int        iRange[6] = {0,5,10,20,50,100};
	for(INT l=0;l<6;l++)
	{
		iPos = m_rtChannel.left + m_rtChannel.Width() * iRange[l]/100;		
		pDC->MoveTo(iPos,iYPos);pDC->LineTo(iPos,iYPos + 3);		
	}	
	pDC->SelectObject(pSavePen);
}

void CTPSliderCtrl::PaintTicks(CDC *pDC)
{

	if(!(TBS_NOTICKS & m_uStyle))  //Tic
	{
		int   iTick     = CSliderCtrl::GetNumTics();
		int   iPos      = 0;
		CPen *pSavePen = pDC->SelectObject(CPen::FromHandle(m_pTicLinePen ->hPen));
		int   iYPos     = 0;
		CRect rtThumb;
		CSliderCtrl::GetThumbRect(&rtThumb);
		if(m_uStyle & TBS_VERT)
		{
			if(m_uStyle & TBS_LEFT)    iYPos = rtThumb.left   - m_iTickEdge; 
			else                       iYPos = rtThumb.right  + m_iTickEdge;
		}
		else
		{		
			if(m_uStyle & TBS_TOP)    iYPos = rtThumb.top    - m_iTickEdge; 
			else                      iYPos = rtThumb.bottom + m_iTickEdge;
		}
		for(INT l=0;l<iTick;l++)
		{
			if(m_uStyle & TBS_VERT)
			{
				iPos = m_rtChannel.top +rtThumb.Height()/2 + (m_rtChannel.Height() - rtThumb.Height())* l /(iTick-1);		

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
				iPos = m_rtChannel.left +rtThumb.Width()/2 + (m_rtChannel.Width() - rtThumb.Width())* l /(iTick-1);		
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
		pDC->SelectObject(pSavePen);
	}
}
void CTPSliderCtrl::PaintCtrl(CDC *pDC)
{
	CRect     rtClient;
	GetClientRect(&rtClient);

	m_iPos = CSliderCtrl::GetPos()-m_iMinOffset;
	CSliderCtrl::GetRange(m_iMin,m_iMax);

	CSliderCtrl::GetChannelRect(&m_rtChannel);
	CSliderCtrl::GetThumbRect(&m_rtThumb);
	if(m_pThumbBmp[0])
		m_rtThumb.DeflateRect(-(m_pThumbBmp[0]->Width() - m_rtThumb.Width())/2,-(m_pThumbBmp[0]->Height() - m_rtThumb.Height())/2);	
	if(m_uStyle & TBS_VERT) 
	{
		CRect rtTemp     = m_rtChannel;
		m_rtChannel.left   = rtTemp.top;
		m_rtChannel.right  = rtTemp.bottom;
		m_rtChannel.top    = rtTemp.left;
		m_rtChannel.bottom = rtTemp.right;
	}
	if(m_clChannel != RGB(0,0,0))
	{
		CRect rtTemp = m_rtChannel;
		rtTemp.DeflateRect(2,2);
		pDC -> FillSolidRect(rtTemp,m_clChannel); // Channe
	}
	if(m_iThumbType == 1)
	{
		CRect rtChannle = m_rtChannel;rtChannle.InflateRect(2,2,2,2);
		pDC->FillSolidRect(rtChannle,m_clBack); // Channel
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner ->PaintRect(m_eBorderStyle,pDC,m_rtChannel,1);
	}
	else
	{
		pDC->FillSolidRect(m_rtChannel,TP_GetSysColor(m_eChanelColor)); // Channel
		if(m_pBackRes) 
		{
			CRect rtChannel = m_rtChannel;rtChannel.InflateRect(0,(m_pBackRes->Height() - m_rtChannel.Height())/2);
			rtChannel.bottom = rtChannel.top + m_pBackRes->Height();
			TP_StretchDIBRect(pDC,m_pBackRes->GetByte(),m_pBackRes->Width(),m_pBackRes->Height(),10,10,rtChannel,FALSE);
		}
			
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		if(m_cPaintTickType ==1 )     
		{
			if(m_pBackRes == NULL) g_pRoundCorner ->PaintRect(m_eBorderStyle,pDC,m_rtChannel,1);
			PaintTicks1(pDC);
		}
		else if(m_cPaintTickType ==0 )
		{
			if(m_pBackRes == NULL) g_pRoundCorner ->PaintRect(m_eBorderStyle,pDC,m_rtChannel,2);
			PaintTicks(pDC);
		}
	}
	if(!(TBS_NOTHUMB & m_uStyle)) 
	{
		int iImage = 0;
		if(!IsWindowEnabled())             iImage = SLIDER_DIS;
		else if(m_bPressed)                iImage = SLIDER_DOWN;
		else if(m_bOver)                   iImage = SLIDER_OVER;		
		else                               iImage = SLIDER_NOR;
		TP_StretchDIBits(pDC,m_pThumbBmp[iImage] ->GetByte(),m_pThumbBmp[iImage]->Width(),m_pThumbBmp[iImage]->Height(),m_rtThumb);
	}	
}
void CTPSliderCtrl::OnPaint()
{
	CPaintDC dc(this); 
	if(!m_bPaint) return;	
	CRect     rtClient;
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());
	if(m_clBack != RGB(0,0,0)) cdcMemo.FillSolidRect(rtClient,m_clBack);
	else cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_eBackColor));
	PaintCtrl(&cdcMemo);	
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);			
}

int CTPSliderCtrl::GetMicroSetp(int iPos,CPoint &ptCur)
{
	if(m_iLineSize ==0 ) return iPos;
	int iDbPos = m_iPos;
	
	if(!(m_uStyle & TBS_VERT))
	{
		int iAmp   = abs(ptCur.x - m_ptDrag.x);		
		if(m_ptDrag.x >ptCur.x) iDbPos -= m_iLineSize*iAmp;
		else                    iDbPos += m_iLineSize*iAmp;
	}
	else
	{
		int iAmp   = abs(ptCur.y - m_ptDrag.y);		
		if(m_ptDrag.y >ptCur.y) iDbPos -= m_iLineSize*iAmp;
		else                    iDbPos += m_iLineSize*iAmp;
	}				
	return iDbPos;
}
void CTPSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{		
	if(m_bEsc) return;
	if(m_rtThumb.PtInRect(point))
	{
		if(!m_bOver)
		{		
			m_bOver = TRUE;
			InvalidateRect(NULL);			
			if(m_hTimer == NULL) m_hTimer = (UINT)SetTimer(100,100,NULL);
		}
	}
	else if(m_bOver)
	{
		m_bOver = FALSE;
		m_bPaint = TRUE;
		InvalidateRect(NULL);
		if(m_hTimer) KillTimer(m_hTimer);
		m_hTimer = NULL;			 
	}
	if(m_bCtrlApply && (nFlags & MK_LBUTTON))
	{	
	}	
	CSliderCtrl::OnMouseMove(nFlags, point);
	if(m_bPressed) InvalidateRect(NULL);
}

void CTPSliderCtrl::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 100)
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);		
		if(!m_rtThumb.PtInRect(point))
		{
			if(m_bOver)
			{
				m_bOver = FALSE;
				InvalidateRect(NULL);
			}	
			KillTimer(m_hTimer);
			m_hTimer = NULL;	
		}
	}
	CSliderCtrl::OnTimer(nIDEvent);
}



void CTPSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{	
		if(!m_bPressed) 
		{
			m_bPressed = TRUE;
			InvalidateRect(NULL);
		}		
		m_iEscPos = GetPos();
		m_bEsc    = FALSE;
		if(m_bCtrlApply) m_iPosSave = point.x;	
		CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CTPSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bPressed)
	{
		m_bPressed = FALSE;
		InvalidateRect(NULL);
	}
	if(!m_bEsc) CSliderCtrl::OnLButtonUp(nFlags, point);
}

BOOL CTPSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CSliderCtrl::OnEraseBkgnd(pDC);
}
void CTPSliderCtrl::SetThumb(CString sPathName)
{
	for(INT l=0;l<4;l++) {TP_ReleaseCommonRes(m_pThumbBmp[l]);m_pThumbBmp[l] = NULL;}
	m_pThumbBmp[0] =  TP_LoadBmpByte(sPathName +_L("_over.bmp"));
	m_pThumbBmp[1] =  TP_LoadBmpByte(sPathName +_L("_Dis.bmp"));
	m_pThumbBmp[2] =  TP_LoadBmpByte(sPathName +_L("_Nor.bmp"));
	m_pThumbBmp[3] =  TP_LoadBmpByte(sPathName +_L("_Down.bmp"));			
}
void CTPSliderCtrl::SetBack(CString sPathName)
{
	if(m_pBackRes) TP_ReleaseCommonRes(m_pBackRes);
	m_pBackRes = TP_LoadBmpByte(sPathName);
}

void CTPSliderCtrl::SetBackColor(COLORREF  clSet)
{
	m_clBack = clSet;
}
void CTPSliderCtrl::SetChannelColor(COLORREF  clSet)
{
	m_clChannel = clSet;
	InvalidateRect(NULL);
}
void CTPSliderCtrl::GetRange(int& nMin, int& nMax) const
{
	CSliderCtrl::GetRange(nMin,nMax);
	nMin -= m_iMinOffset;
	nMax -= m_iMinOffset;
}
void CTPSliderCtrl::SetRange(int nMin, int nMax, BOOL bRedraw)
{
	m_iMinOffset = nMin <0 ? -nMin:0;	
	nMin += m_iMinOffset;
	nMax += m_iMinOffset;
	CSliderCtrl::SetRange(nMin,nMax,bRedraw);
}
int CTPSliderCtrl::GetPos() const
{
	if(m_cMicro) return m_iGetPos-m_iMinOffset;
	else if(m_uBaseState &TP_BASESTATE_NOSCROLL) return m_iEscPos-m_iMinOffset;
	else return CSliderCtrl::GetPos()-m_iMinOffset;
}
void CTPSliderCtrl::SetPos(int nPos)
{
	CSliderCtrl::SetPos(nPos+m_iMinOffset);
	InvalidateRect(NULL);
}
BOOL CTPSliderCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE && m_bPressed)
	{		
		m_uBaseState |= TP_BASESTATE_NOSCROLL;
		OnLButtonUp(0, CPoint(0,0));						
		m_uBaseState &= ~TP_BASESTATE_NOSCROLL;
		SetPos(m_iEscPos);
		m_bEsc = TRUE;
		Invalidate();	
	}
	return __super::PreTranslateMessage(pMsg);
}
void CTPSliderCtrl::SetToolTips(CString sToolTips)
{
	m_sTooltips = sToolTips;
	EnableToolTips(m_sTooltips.GetLength()>0);
}
BOOL CTPSliderCtrl::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	lstrcpyn( pTTT->szText ,m_sTooltips, 79);	
	return  TRUE;		
}
INT_PTR CTPSliderCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if(m_sTooltips.GetLength()>0)
	{		
		pTI ->hwnd     = m_hWnd;	
		pTI ->lpszText = LPSTR_TEXTCALLBACK;
		pTI ->uId      = (UINT)(UINT_PTR)m_hWnd;
		pTI ->uFlags  |= TTF_IDISHWND;
		return  GetDlgCtrlID();			
	}
	else return __super::OnToolHitTest(point, pTI);
}
int CTPSliderCtrl::SetLineSize(int nSize)
{
	m_iLineSize = nSize;
	return CSliderCtrl::SetLineSize(nSize);
}