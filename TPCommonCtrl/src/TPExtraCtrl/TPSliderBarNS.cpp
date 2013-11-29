// TPSliderBarNS.cpp : 实现文件
//

#include "stdafx.h"
#include ".\tpsliderbarns.h"

// CTPSliderBarNS

IMPLEMENT_DYNAMIC(CTPSliderBarNS, CWnd)
CTPSliderBarNS::CTPSliderBarNS()
{	
	m_pMsgWnd       = NULL;
	m_uStyle        = 0;

	m_iPage         = 20;
	m_iPagePixel    = 10;

	m_iTopLeft     = 5;
	m_iRightBottom = 5;
	m_iBarSize     = 0;
	
	m_iMin          = 0;
	m_iMax          = 100;
	m_iPos          = 0;

	m_bOver         = FALSE;
	m_bPressed      = FALSE;

	m_rtThumb       = CRect(0,0,0,0);
	m_rtThumbHandle = CRect(0,0,0,0);
	m_rtChannel     = CRect(0,0,0,0);
	m_rtClient      = CRect(0,0,0,0);

	m_ptDragSave    = CPoint(0,0);
	m_iDragSave     = 0;
	m_hTimerHover   = NULL;
	m_iChanneOffset = 0;
	m_hTimerDBClick = NULL;
	m_iThumbType	=0;
	m_bDoubleClick  = FALSE;
	m_bCtrlApply    = FALSE;
	m_clChannel      = RGB(0,0,0);
	m_clBack         = RGB(0,0,0);
	for(INT  l=0;l<4;l++)
		m_pThumbBmp[l]   = NULL;
	m_pBackRes      = NULL;
	m_eBackColor     = COLOR_BASE_FRAME;
	m_eChanelColor   = COLOR_BASE_WINDOW;
	m_eBorderStyle   = TP_GetBorderStyle();;

}

CTPSliderBarNS::~CTPSliderBarNS()
{
	for(INT  l=0;l<4;l++)
		TP_ReleaseCommonRes(m_pThumbBmp[l]);
		
}
void     CTPSliderBarNS::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),TRACKBAR_CLASS,&wndClass);		
		wndClass.lpszClassName = WC_SLIDERBARNS;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_SLIDERBARNS,AfxGetInstanceHandle());
}
BEGIN_MESSAGE_MAP(CTPSliderBarNS, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CTPSliderBarNS::LoadThumb()
{
	if(m_pThumbBmp[0] != NULL) return;
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
		CString sSysSkinPath = TP_GetSkinPath();

		for(INT  l=0;l<4;l++)
			TP_ReleaseCommonRes(m_pThumbBmp[l]);

		if(m_iThumbType == 0)
		{		
			m_pThumbBmp[0] =  TP_LoadBmpByte(sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider_") + sThumbName +_L("_over.bmp"));
			m_pThumbBmp[1] =  TP_LoadBmpByte(sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider_") + sThumbName +_L("_Dis.bmp"));
			m_pThumbBmp[2] =  TP_LoadBmpByte(sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider_") + sThumbName +_L("_Nor.bmp"));
			m_pThumbBmp[3] =  TP_LoadBmpByte(sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider_") + sThumbName +_L("_Down.bmp"));				
		}
		else if(m_iThumbType == 1)
		{
			m_pThumbBmp[0] =  TP_LoadBmpByte(sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider1_") + sThumbName +_L("_over.bmp"));
			m_pThumbBmp[1] =  TP_LoadBmpByte(sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider1_") + sThumbName +_L("_Dis.bmp"));
			m_pThumbBmp[2] =  TP_LoadBmpByte(sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider1_") + sThumbName +_L("_Nor.bmp"));
			m_pThumbBmp[3] =  TP_LoadBmpByte(sSysSkinPath + _L("\\CommonCtrl\\SliderCtrl\\slider1_") + sThumbName +_L("_Down.bmp"));				
		}
	}
}


int CTPSliderBarNS::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_uStyle = CWnd::GetStyle(); 
	m_uStyle |= TBS_HORZ;
	LoadThumb();		
	CalcSize(SLIDER_CHANNEL);		
	EnableToolTips(TRUE);
	return 0;
}

BOOL CTPSliderBarNS::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CTPSliderBarNS::OnNcPaint()
{
}

void CTPSliderBarNS::PaintCtrl(CDC *pDC)
{
	CRect     rtClient;
	GetClientRect(&rtClient);
	CalcSize(SLIDER_CHANNEL);
	CalcSize(SLIDER_THUMB);	
	m_rtThumb.DeflateRect(0,-(m_pThumbBmp[0]->Height() - m_rtThumb.Height())/2);	
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
void CTPSliderBarNS::OnPaint()
{
	CPaintDC dc(this); 
	CDC *pDC = &dc;
	CRect     rtClient;
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());
	if(m_clBack != RGB(0,0,0)) cdcMemo.FillSolidRect(rtClient,m_clBack);
	else cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_eBackColor));
	PaintCtrl(&cdcMemo);	
	pDC ->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);			
}
int CTPSliderBarNS::GetPageSize() const
{
	return m_iPage;
}
void CTPSliderBarNS::SetPageSize(int iPageSize,BOOL bPaint)
{
	m_iPage = iPageSize;
	if(bPaint)	Invalidate(FALSE);	
}

int CTPSliderBarNS::PixelToPos(int iPixel)
{	
	int iPos = 0;
	if(m_uStyle & TBS_VERT)
	{
		if(m_iMax>m_iMin)	iPos = (m_rtChannel.bottom - m_iPagePixel - iPixel) * (m_iPage + m_iMax - m_iMin) / m_rtChannel.Height() + m_iMin;
		else iPos = m_iMin;
	}
	else
	{		
		if(m_iMax>m_iMin)	iPos =  int((double(iPixel - m_rtChannel.left)) * (m_iPage + m_iMax - m_iMin) /m_rtChannel.Width() + m_iMin);
		else iPos = m_rtChannel.left;
	}
	iPos = max(iPos,m_iMin);
	iPos = min(iPos,m_iMax);
	return iPos;

}
int CTPSliderBarNS::PosToPixel(int iPos)
{
	if(m_uStyle & TBS_VERT)
	{
		if(m_iMax>m_iMin)	return int(m_rtChannel.bottom - m_iPagePixel - (double)m_rtChannel.Height() *(iPos - m_iMin) /(m_iPage + m_iMax - m_iMin));
		else return m_rtChannel.top;
	}
	else
	{		
		if(m_iMax>m_iMin)	
		{
			float fPixel = (float)m_rtChannel.Width() *(iPos - m_iMin) /(m_iPage + m_iMax - m_iMin);
			int nPixel = (int)fPixel;
			nPixel = fPixel - nPixel >= 0.5? nPixel+1:nPixel;
			return int(m_rtChannel.left +  nPixel);
		}
		else return m_rtChannel.left;
	}

}

void   CTPSliderBarNS::GetChannelRect(CRect &rtChannel)
{
	GetClientRect(&rtChannel);
	int nChannelHeight = rtChannel.Height();
	rtChannel.top +=nChannelHeight*2/5;
	rtChannel.bottom -= nChannelHeight*2/5;
	rtChannel.left +=m_iTopLeft;
	rtChannel.right -= m_iRightBottom;

}
void   CTPSliderBarNS::CalcSize(UINT uFlag)
{
	if(uFlag & SLIDER_CHANNEL)
	{
		GetClientRect(&m_rtClient);
		GetChannelRect(m_rtChannel);	
		m_rtThumb = m_rtChannel;
		m_rtThumb.left +=2;
		m_rtThumb.right--;
	}
	else if(uFlag & SLIDER_THUMB)
	{
		if (!m_iPage)
		{
			m_iPage++;
		}

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

		}
		else
		{
			if(m_iMax > m_iMin)
			{
				float fPagePixel = (float)m_rtChannel.Width() * m_iPage / (m_iPage + m_iMax - m_iMin);
				
				m_iPagePixel    = int(fPagePixel);
				m_iPagePixel	= fPagePixel - m_iPagePixel >= 0.5? m_iPagePixel+1:m_iPagePixel;
				m_rtThumb.left  = PosToPixel(m_iPos);
				m_rtThumb.right = m_rtThumb.left + m_iPagePixel;
			}
			else
			{
				m_rtThumb.left  = m_rtChannel.left;
				m_rtThumb.right = m_rtChannel.right;
				m_iPagePixel    = m_rtChannel.Width();
			}

		}
	}
}

void CTPSliderBarNS::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(GetFocus() != this) 
		SetFocus();		
	if(m_rtThumb.PtInRect(point))
	{ 
		m_bPressed  = TRUE;
		SetCapture();	
		m_iDragInit = m_iPos;
		m_ptDragSave = point;
		m_iDragSave  = point.x - m_rtThumb.left ;
		Invalidate();
	}
			
}

void CTPSliderBarNS::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int iDragPos = m_iPos;	
	ReleaseCapture();
	if (m_bPressed)
	{
		m_bPressed  = FALSE;
	}
	else
	{
		return;
	}
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
	Invalidate(FALSE);

}

void CTPSliderBarNS::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bPressed)
	{
		int iDragPos = m_iPos;
		CPoint ptTest = point;
		if(m_ptDragSave.x != point.x)		
		{
			//if(point.x  - m_iDragSave  < m_rtChannel.left) 
			//{ 
			//	if(m_iPos>m_iMin) iDragPos = m_iMin;
			//}												
			//else if(point.x  - m_iDragSave + m_iPagePixel >m_rtChannel.right) 
			//{
			//	if(m_iPos<m_iMax) iDragPos = m_iMax;						
			//}			
			//else
			{	
				if ((nFlags & MK_CONTROL)&&(m_bCtrlApply))
				{
					/*iDragPos = PixelToPos(point.x  - m_iDragSave);
					iDragPos = max(iDragPos,m_iMin);
					iDragPos = min(iDragPos,m_iMax);
					int nTemp = (iDragPos - m_iPos)/10;

					

					if (nTemp == 0)
					{
						if (point.x > m_ptDragSave.x)
						{
							nTemp = 1;
						}
						else
						{
							nTemp = -1;
							
						}
					}
					iDragPos = nTemp + m_iPos;
					ptTest.x = PosToPixel(iDragPos) + m_iDragSave;
					ClientToScreen(&ptTest);
					SetCursorPos(ptTest.x,ptTest.y);
					ScreenToClient(&ptTest);*/
					if (point.x > m_ptDragSave.x)
					{
						iDragPos = 1 + m_iPos;
					}
					else
					{
						iDragPos = m_iPos - 1;

					}

					ptTest.x = PosToPixel(iDragPos) + m_iDragSave;
					ClientToScreen(&ptTest);
					SetCursorPos(ptTest.x,ptTest.y);
					ScreenToClient(&ptTest);
				}	
				else
				{
					iDragPos = PixelToPos(point.x  - m_iDragSave);
				}
				iDragPos = max(iDragPos,m_iMin);
				iDragPos = min(iDragPos,m_iMax);
			}
			SetPos(iDragPos);	
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)iDragPos),(LPARAM)m_hWnd);						
		}
		m_ptDragSave = ptTest;				
		Invalidate(TRUE);
		UpdateWindow();
	}	
		
}




BOOL CTPSliderBarNS::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_pMsgWnd = pParentWnd;
	m_uStyle |= dwStyle;
	return CWnd::Create(NULL,NULL,dwStyle, rect, pParentWnd, nID);
}


void CTPSliderBarNS::PreSubclassWindow()
{
	if(m_pMsgWnd == NULL) 
		m_pMsgWnd = GetParent();
	m_uStyle = CWnd::GetStyle(); 
	m_uStyle |= TBS_HORZ;

	LoadThumb();		
	CalcSize(SLIDER_CHANNEL);		
	EnableToolTips(TRUE);
	CWnd::PreSubclassWindow();
}

void CTPSliderBarNS::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_bDoubleClick)
	{
		m_pMsgWnd->PostMessage(WM_SLIDERCTRLMSG,WM_SLIDERCTRL_LBD,LPARAM(m_hWnd));					
	}
}

void CTPSliderBarNS::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_ESCAPE:
		{
			if (m_bPressed)
			{
				m_bPressed = FALSE;
				int iDragPos;
				iDragPos = m_iDragInit;
				iDragPos = max(iDragPos,m_iMin);
				iDragPos = min(iDragPos,m_iMax);
				SetPos(iDragPos);	
				Invalidate(FALSE);			
				m_pMsgWnd->PostMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)iDragPos),(LPARAM)m_hWnd);

			}

		}
		break;
	case VK_LEFT:
		{
			if (m_iPos > m_iMin)
			{
				m_iPos--;
				Invalidate(FALSE);			
				m_pMsgWnd->PostMessage(WM_HSCROLL,MAKEWPARAM(SB_LINELEFT,(WORD)m_iPos),(LPARAM)m_hWnd);
			}
		}
		break;
	case VK_RIGHT:
		{
			if (m_iPos < m_iMax)
			{
				m_iPos++;
				Invalidate(FALSE);			
				m_pMsgWnd->PostMessage(WM_HSCROLL,MAKEWPARAM(SB_LINERIGHT,(WORD)m_iPos),(LPARAM)m_hWnd);
			}
		}
		break;
	case VK_HOME:
		{
			m_iPos = m_iMin;
			Invalidate(FALSE);			
			m_pMsgWnd->PostMessage(WM_HSCROLL,MAKEWPARAM(SB_LEFT,(WORD)m_iPos),(LPARAM)m_hWnd);
		}
		break;
	case VK_END:
		{
			m_iPos = m_iMax;
			Invalidate(FALSE);			
			m_pMsgWnd->PostMessage(WM_HSCROLL,MAKEWPARAM(SB_RIGHT,(WORD)m_iPos),(LPARAM)m_hWnd);

		}
		break;
	case VK_NEXT:
		{
			int iDragPos;
			iDragPos = m_iPos + m_iPage;
			iDragPos = min(iDragPos,m_iMax);
			m_iPos = iDragPos;
			Invalidate(FALSE);			
			m_pMsgWnd->PostMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGERIGHT,(WORD)m_iPos),(LPARAM)m_hWnd);

		}
		break;
	case VK_PRIOR:
		{
			int iDragPos;
			iDragPos = m_iPos - m_iPage;
			iDragPos = max(iDragPos,m_iMin);
			m_iPos = iDragPos;
			Invalidate(FALSE);			
			m_pMsgWnd->PostMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGELEFT,(WORD)m_iPos),(LPARAM)m_hWnd);

		}
		break;
	default:
		;
	}

	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CTPSliderBarNS::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			if (GetFocus()== this&&m_bPressed) 
			{
				SendMessage(WM_KEYDOWN,pMsg->wParam,pMsg->lParam);
				return TRUE;

			}
			break;
		case VK_LEFT:
		case VK_RIGHT:
			if (GetFocus()== this) 
			{
				SendMessage(WM_KEYDOWN,pMsg->wParam,pMsg->lParam);
				return TRUE;
			}
			break;
		}
	}
	return __super::PreTranslateMessage(pMsg);
}

void CTPSliderBarNS::SetBackColor(COLORREF  clSet)
{
	m_clBack = clSet;
}
void CTPSliderBarNS::SetChannelColor(COLORREF  clSet)
{
	m_clChannel = clSet;
	Invalidate(FALSE);
}