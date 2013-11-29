// TPVUSliderCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include ".\tpvusliderctrl.h"

#ifdef   TP_SMARTSLIDER_CONTORL
#include "TPMouseController.h"
#include "TPMouseDeviceHooker.h"
#include "TPMouseCalc.h"
#endif

#include <math.h>

// CTPVUSliderCtrl
TPCommonRes      *CTPVUSliderCtrl::m_pLargIcon = NULL;
int               CTPVUSliderCtrl::m_iRefCount = 0;
extern           double            TP_NS_StepDB[];
extern           double            TP_NS_StepTick[];

IMPLEMENT_DYNAMIC(CTPVUSliderCtrl, CTPSliderBar)
CTPVUSliderCtrl::CTPVUSliderCtrl()
{
	m_dwWindowLevel = TPWNDLEVEL1;
	m_pdStepValue  = NULL;
	m_iStepCount   = 0;

	m_iTickEdge    = -2;
	m_bPaintLable  = TRUE;
	m_iMin         = 0;
	m_iMax         = 100;
	m_iPage        = 0;
	m_showTips     = NULL;

	m_uState       |= TP_SLIDERBAR_NOBACK;

	m_uPrivateStyle = 0;
	m_clBack        = RGB(0,0,0);

	m_bEsc            = FALSE;
	m_iEscPos         = 0;
	m_dDBSet          = 0;
	m_hClickTime      = 0;
	m_iClickPos       = 0;

#ifdef TP_SMARTSLIDER_CONTORL
	m_dValMax         = 10;
	m_dValMin         = -100;
	m_bRecived		  = FALSE;
	m_nPerWidth		  = 0;
	m_nOperateState   = Op_Nothing;
	m_dValue		  = 0.0;
	m_dStartVal		  = 0.0;
	m_dStackValue	  = 0.0;
	m_dStep			  = 0.01;
#endif
}

CTPVUSliderCtrl::~CTPVUSliderCtrl()
{
}

BEGIN_MESSAGE_MAP(CTPVUSliderCtrl, CTPSliderBar)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_SIZE()
ON_WM_LBUTTONDBLCLK()
ON_WM_TIMER()
ON_WM_KEYDOWN()//add by lizhiqiao for:按上下键 滑块移动反向的问题。2008.09.25
ON_WM_MOUSEWHEEL()
ON_WM_CREATE()
#ifdef TP_SMARTSLIDER_CONTORL
ON_MESSAGE(WM_SE_MOUSEHOOK, OnMouseDeviceHookCursorMove)
#endif
END_MESSAGE_MAP()

void     CTPVUSliderCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_SLIDERBAR,&wndClass);
		wndClass.lpszClassName = WC_VUSLIDERCTRL;
		wndClass.style        |= CS_DBLCLKS;
		::RegisterClass(&wndClass);
	}
	else
		UnregisterClass(WC_VUSLIDERCTRL,AfxGetInstanceHandle());
}
void CTPVUSliderCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;

		CTPSliderBar::SubWindow(bSub);
		CTPSliderBar::ModifyStyle(0,TBS_ENABLESELRANGE|TBS_AUTOTICKS|TBS_VERT|SLIDER_THUMB|TBS_TOOLTIPS,0);
		m_uStyle = CTPSliderBar::GetStyle() & (~TBS_NOTICKS);

		SetStepValue(TP_NS_StepDB,11,TP_NS_StepTick);

		CRect rtClient; GetClientRect(&rtClient);
#ifdef TP_NS_EDIT
		m_uStyle &= ~TBS_TOOLTIPS;
#else
		m_showTips = new CToolTipCtrl();
		m_showTips ->Create(this,TTS_ALWAYSTIP); 
		m_showTips ->AddTool(this,LPSTR_TEXTCALLBACK,CRect(0,0,rtClient.Width(),rtClient.Height()),1);
		m_showTips ->SetDelayTime(TTDT_INITIAL,10);
		m_showTips ->SetDelayTime(TTDT_RESHOW,10);
		m_showTips ->Activate(TRUE);
		EnableToolTips();
#endif
	}
	else
	{
		if(!m_bSubWindow) return;
		if(m_showTips)
		{
			m_showTips ->DestroyToolTipCtrl();
			m_showTips = NULL;
		}
		CTPSliderBar::SubWindow(bSub);
	}
}
void     CTPVUSliderCtrl::LoadThumb()
{
	if(!(TBS_NOTHUMB & m_uStyle)) 
	{
		CString sThumbName = _L("");
		if(m_uStyle & TBS_VERT)     sThumbName = _L("V");
		else	                    sThumbName = _L("H");

#ifdef TP_NS_EDIT
		m_pThumbBmp[0] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\slider1_") + sThumbName +_L("_over.bmp"));
		m_pThumbBmp[1] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\slider1_") + sThumbName +_L("_Dis.bmp"));
		m_pThumbBmp[2] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\slider1_") + sThumbName +_L("_Nor.bmp"));
		m_pThumbBmp[3] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\slider1_") + sThumbName +_L("_Down.bmp"));	
#elif TP_MINI_EDIT
		m_pThumbBmp[0] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\mini\\slider_") + sThumbName +_L("_over.bmp"));
		m_pThumbBmp[1] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\mini\\slider_") + sThumbName +_L("_Dis.bmp"));
		m_pThumbBmp[2] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\mini\\slider_") + sThumbName +_L("_Nor.bmp"));
		m_pThumbBmp[3] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\mini\\slider_") + sThumbName +_L("_Down.bmp"));	
#else 
		m_pThumbBmp[0] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\slider_") + sThumbName +_L("_over.bmp"));
		m_pThumbBmp[1] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\slider_") + sThumbName +_L("_Dis.bmp"));
		m_pThumbBmp[2] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\slider_") + sThumbName +_L("_Nor.bmp"));
		m_pThumbBmp[3] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\slider_") + sThumbName +_L("_Down.bmp"));	
#endif
		SetBackRes(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\slider_v_back.bmp"));
		m_pTicLinePen  = TP_LoadPen(COLOR_BASE_TEXT,PS_SOLID,1);
	}
}
void CTPVUSliderCtrl::PaintCtrl(CDC *pDC)
{
	if(!m_bReDraw) return;

	CRect     rtClient;
	GetClientRect(&rtClient);
	CSliderCtrl::GetRange(m_iMin,m_iMax);
	m_iPos = CSliderCtrl::GetPos();
	CTPVUSliderCtrl::CalcSize(SLIDER_THUMB);

	m_iTickEdge = 6;

	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	CRect rtChannel = m_rtChannel;
	if(m_pBackRes)
	{
		if(m_clBack != RGB(0,0,0)) pDC->FillSolidRect(rtClient,m_clBack);
		rtChannel.InflateRect(1,0);
		TP_StretchDIBRect(pDC,m_pBackRes->GetByte(),m_pBackRes ->Width(),m_pBackRes ->Height(),10,10,rtChannel,TRUE,FALSE,2);
	}
	else
	{
		pDC->FillSolidRect(rtChannel,TP_GetSysColor(COLOR_BASE_WINDOW)); // Channel
		g_pRoundCorner ->PaintRect(m_eBorderStyle,pDC,rtChannel,2);
	}
	PaintTicks(pDC);
	if(!(m_uPrivateStyle & TP_VUSLIDERCTRL_NOTHUMB))
	{
		if(!(TBS_NOTHUMB & m_uStyle)) 
		{
			int iImage = 0;

			if(!IsWindowEnabled()) iImage = SLIDER_DIS;
			else if(m_bPressed)    iImage = SLIDER_DOWN;
			else if(m_bOver)       iImage = SLIDER_OVER;
			else                   iImage = SLIDER_NOR;

			TP_SetDIBitsToDevice(pDC,m_pThumbBmp[iImage] ->GetByte(),m_pThumbBmp[iImage]->Width(),m_pThumbBmp[iImage]->Height(),m_rtThumb);
		}
	}
}
void CTPVUSliderCtrl::PaintTicksEx(CDC *pDC,CRect rtTick)
{
	CPen pen(PS_SOLID,1,TP_GetSysColor(COLOR_BASE_TEXT));
	CPen *pSavePen = pDC->SelectObject(&pen);
	CalcSize(SLIDER_THUMB);
	for(INT l=0;l<m_iStepCount;l++)
	{
		int iPos = PosToPixel(int(DBToPos(m_pdStepValue[l]))) + m_rtThumb.Height()/2 + rtTick.top;

		pDC->MoveTo(rtTick.left,iPos);
		pDC->LineTo(rtTick.right,iPos);
		if(m_pdStepValue[l] == 0)
		{
			pDC->MoveTo(rtTick.left,iPos+1);
			pDC->LineTo(rtTick.right,iPos+1);
		}
	}
	pDC ->SelectObject(pSavePen);
}
int CTPVUSliderCtrl::PixelToPos(int iPixel)
{
	return CTPSliderBar::PixelToPos(iPixel-2);
}

void CTPVUSliderCtrl::PaintTicksAudio(CDC *pDC)
{
	CRect rtClient,rtThumb = m_rtThumb;
	GetClientRect(&rtClient);
	CPen *pSavePen = pDC->SelectObject(CPen::FromHandle(m_pTicLinePen ->hPen));
	for(INT l=0;l<m_iStepCount;l++)
	{
		int iPos = PosToPixel(int(DBToPos(m_pdStepValue[l]))) + rtThumb.Height()/2;
		if(m_uPrivateStyle &TP_VUSLIDERCTRL_AUDIOTICKL)
		{
			pDC->MoveTo(rtClient.left,iPos);
			pDC->LineTo(rtClient.left+3,iPos);
			if(m_pdStepValue[l] == 0)
			{
				pDC->MoveTo(rtClient.left,iPos+1);
				pDC->LineTo(rtClient.left+3,iPos+1);
			}
		}
		else if(m_uPrivateStyle &TP_VUSLIDERCTRL_AUDIOTICKR)
		{
			pDC->MoveTo(rtClient.right,iPos);
			pDC->LineTo(rtClient.right-3,iPos);
			if(m_pdStepValue[l] == 0)
			{
				pDC->MoveTo(rtClient.right,iPos+1);
				pDC->LineTo(rtClient.right-3,iPos+1);
			}
		}
	}
	pDC ->SelectObject(pSavePen);
}
void CTPVUSliderCtrl::PaintTicks(CDC *pDC)
{
//	CTPSliderBar::PaintTicks(pDC);

	if(m_uPrivateStyle &(TP_VUSLIDERCTRL_AUDIOTICKL|TP_VUSLIDERCTRL_AUDIOTICKR))
	{
		PaintTicksAudio(pDC);
	}
	else
	{
		if(!(TBS_NOTICKS & m_uStyle) && m_pdStepValue)  //Tic
		{
			int     iTick = CTPSliderBar::GetNumTics();
			CPoint  ptText;
			CRect   rtThumb;
			CString sText;
			int     iPosLine=0, iPosTick=0;
			CSize   szText = pDC->GetTextExtent(_L("9"));
			CTPSliderBar::GetThumbRect(&rtThumb);
			pDC->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
			if(m_uStyle & TBS_VERT)
			{
				if(m_uStyle & TBS_LEFT) iPosLine = rtThumb.left   - m_iTickEdge;
				else                    iPosLine = rtThumb.right  + m_iTickEdge;
			}
			else
			{
				if(m_uStyle & TBS_TOP) iPosLine = rtThumb.top    - m_iTickEdge;
				else                   iPosLine = rtThumb.bottom + m_iTickEdge;
			}
			CPen *pSavePen = pDC->SelectObject(CPen::FromHandle(m_pTicLinePen ->hPen));
			for(INT l=0;l<iTick;l++)
			{
				if(m_uStyle & TBS_VERT)
				{
					iPosTick = TP_DoubleToInt(m_rtChannel.top +rtThumb.Height()/2 + (m_rtChannel.Height() - rtThumb.Height())* (m_pdStepTicks[l] - m_pdStepTicks[m_iStepCount-1]) /(m_pdStepTicks[0] - m_pdStepTicks[m_iStepCount-1]))+1;
					if(m_uStyle & TBS_LEFT) 
					{
						pDC->MoveTo(iPosLine,iPosTick);pDC->LineTo((l == 0 || l == iTick - 1) ?  iPosLine-3 : iPosLine-2,iPosTick);
					}
					else
					{
						pDC->MoveTo(iPosLine,iPosTick);pDC->LineTo((l == 0 || l == iTick - 1) ?  iPosLine+3 : iPosLine+2,iPosTick);
					}

					if(m_uStyle & TBS_LEFT)
					{
						ptText = CPoint(iPosLine - 4 * szText.cx,iPosTick - szText.cy);
					}
					else
					{
						ptText = CPoint(iPosLine-2,iPosTick -szText.cy);
					}
					sText.Format(_L("%+4.0f"),m_pdStepValue[m_iStepCount - l -1]);
				}

				if( m_bPaintLable) pDC->TextOut(ptText.x,ptText.y,sText);
			}
			pDC ->SelectObject(pSavePen);
		}
	}
}
int CTPVUSliderCtrl::ValueToPos(double dValue,double  *pdValue,int iCount)
{
	if(pdValue == NULL || iCount <1) return TP_DoubleToInt(dValue);
	else
	{
		if(dValue <= pdValue[0]) return m_iMin;
		else if(dValue >= pdValue[iCount-1]) return m_iMax;

		double dStep = (m_iMax- m_iMin)*1.0/(iCount-1);

		int i=0;
		for(i=0;i<iCount;i++)
		{
			if(dValue <= pdValue[i]) break;
		}
		return TP_DoubleToInt((i-1)*dStep +(dValue-pdValue[i-1])*dStep/(pdValue[i] -pdValue[i-1])) + m_iMin;
	}
}
double CTPVUSliderCtrl::PosToValue(int iPos,double  *pdValue,int iCount)
{
	if(pdValue == NULL || iCount <1) return iPos;
	else
	{
		iPos -= m_iMin;
		double db  = 0;
		int    nIndex = int(iPos * (iCount - 1)/(m_iMax- m_iMin));
		double dValue = iPos * (iCount - 1) *1.0/(m_iMax- m_iMin);
		if(nIndex < 0)
		{
			db = pdValue[0];
		}
		else if(nIndex >= (iCount - 1))
		{
			db = pdValue[iCount-1];
		}
		else
		{
			db = pdValue[nIndex] + (pdValue[nIndex+1] - pdValue[nIndex] )*(dValue - nIndex);
		}
		return db;
	}
}

double CTPVUSliderCtrl::GetDB()
{
	if(m_dDBSet !=0)                         return m_dDBSet;
	if(m_uBaseState & TP_BASESTATE_NOSCROLL) return PosToDB(m_iEscPos);
	else                                     return DBAlign(PosToDB(CTPSliderBar::GetPos()),m_iDimLen);
}
double CTPVUSliderCtrl::GetValue()
{
	return  CTPSliderBar::GetPos() / ((m_iStepCount - 1) * 10.0);
}

BOOL   CTPVUSliderCtrl::SetValue(double dValue)
{
#ifndef TP_SMARTSLIDER_CONTORL
	dValue = dValue * (m_iStepCount - 1) * 10;
	CTPSliderBar::SetPos(TP_DoubleToInt(dValue));
	return TRUE;
#else
	if(dValue > m_dValMax)
	{
		dValue = m_dValMax;
	}
	if(dValue < m_dValMin)
	{
		dValue = m_dValMin;
	}

	if(m_dValue == dValue)
		return FALSE;
	m_dValue = dValue;
	SetPos(Value2Pos(m_dValue));
	return TRUE;
#endif
}
void   CTPVUSliderCtrl::SetDB(double dDB,BOOL bComplex)
{
	m_dDBSet = dDB;
	if(bComplex) CTPSliderBar::SetPos(TP_DoubleToInt(DBToPosComplex(dDB)));
	else         CTPSliderBar::SetPos(TP_DoubleToInt(DBToPos(dDB)));
#ifdef TP_SMARTSLIDER_CONTORL
	m_dValue = Pos2Value(GetPos());
#endif
}
void   CTPVUSliderCtrl::SetPaintLable(BOOL bPaintLable)
{
	m_bPaintLable = bPaintLable;
}

void CTPVUSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bEsc = FALSE;
	if(GetFocus() != this) SetFocus();
	if(m_uPrivateStyle & TP_VUSLIDERCTRL_NOTHUMB) return;

	CTPSliderBar::GetRange(m_iMin,m_iMax);
	m_iEscPos = m_iPos  = CSliderCtrl::GetPos();
	int iCurPos = m_iPos;
	if(m_hClickTime) KillTimer(m_hClickTime);
	m_iClickPos       = 0;
	m_hClickTime      = 0;

#ifndef TP_SMARTSLIDER_CONTORL
	if(!(m_uStyle & TBS_VERT))
	{
		int iPos = PixelToPos(point.x - m_iPagePixel/2);
		if(m_iPos != iPos)
		{
			m_iClickPos = iPos;
			m_hClickTime = SetTimer(1000,500,NULL);
			m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,0,(LPARAM)this);
		}
		else
			m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,0,(LPARAM)this);
		Invalidate(FALSE);
		m_bPressed  = TRUE;
		m_iDragMode = TP_DRAG_MIDDLE;
		SetCapture();

		m_ptDragSave = point;
		m_iDragSave  = point.x - m_rtThumb.left ;
	}
	else
	{
		m_dwDragTime = TP_GetMMTimeMinSecond();
		int iPos = PixelToPos(point.y - m_iPagePixel/2);
		if(m_iPos != iPos) 
		{
			m_iClickPos = iPos;
			m_hClickTime = SetTimer(1000,500,NULL);
			m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,0,(LPARAM)this);
		}
		else
			m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,0,(LPARAM)this);
		Invalidate(FALSE);
		m_bPressed   = TRUE;
		m_iDragMode  = TP_DRAG_MIDDLE;
		m_ptDragSave = point;
		m_iDragSave  = point.y - m_rtThumb.top;
		SetCapture();
	}
#else
	CRect rectTemp = m_rtThumb;
	if(!(m_uStyle & TBS_VERT))
	{
		int iPos = PixelToPos(point.x - m_iPagePixel/2);
		if(m_iPos != iPos)
		{
			if(!m_rtThumb.PtInRect(point))
			{
				SetPos(iPos);
				m_dValue = Pos2Value(GetPos());
			}
			m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,0,(LPARAM)this);
		}
		else
			m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,0,(LPARAM)this);
	}
	else
	{
		int iPos = PixelToPos(point.y - m_iPagePixel/2);
		if(m_iPos != iPos)
		{
			if(!m_rtThumb.PtInRect(point))
			{
				SetPos(iPos);
				m_dValue = Pos2Value(GetPos());
			}
			m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,0,(LPARAM)this);
		}
		else
			m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,0,(LPARAM)this);
	}
	CalcSize(SLIDER_THUMB);
	rectTemp = m_rtThumb;
	CTPMouseDeviceHooker* pHooker = NULL;

	if(CanIntelligentOperation() && (pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker()) != NULL)
	{
		if(!(m_uStyle & TBS_VERT))
			pHooker->SetMouseAxis(SE_CALC_X);
		else
			pHooker->SetMouseAxis(SE_CALC_Y);
		InitCalcVal();

		CPoint pt = point;
		ClientToScreen(&pt);
		m_ptPrevPoint.x = pt.x;
		m_ptPrevPoint.y = pt.y;

		CTPMouseCalc* pCalc = NULL;
		pCalc = CTPMouseDeviceImpl::GetDefMouseCalcIntelligent();

		if(!(m_uStyle & TBS_VERT))
			pCalc->SetCalcAxis(SE_CALC_X);
		else
			pCalc->SetCalcAxis(SE_CALC_Y);

		if(GetKeyState(VK_CONTROL)<0)
		{
			if(pHooker)
				pHooker->SetCtrl(TRUE);
		}

		pHooker->SetChannelRect(m_rtChannel);
	
		/*CRect rcThumb;
		CSliderCtrl::GetThumbRect(&rcThumb);*/
		//rcThumb.InflateRect(SE_SLIDER_EXPAND_HITTEST);

		//snp4100026905 Liangxuan 2010-10-25
		point.x += m_rtThumb.left - rectTemp.left;
		point.y += m_rtThumb.top  - rectTemp.top;
		if(m_rtThumb.PtInRect(point))
		{
			if(pHooker->StartHook(GetSafeHwnd(), pt, pCalc, CTPMouseDeviceHooker::Chk_LButtonUp))
			{
				//jyn added 20090510
				double dRange = (m_dValMax-m_dValMin)/m_dStep;
				pHooker->SetRange(dRange);
				//jyn added end
				m_nOperateState = Op_Intelligent;
				m_bPressed	    = TRUE;
				SetCapture();
				//GetParent()->SendMessage(WM_SE_PARAMETER, GetDlgCtrlID());
			}
			else
			{
				m_nOperateState = Op_Nothing;
				ASSERT(FALSE);
			}
		}
	}

	if(m_nOperateState == Op_Nothing)
	{
		m_nOperateState = Op_Original;
		//CTPSliderBar::OnLButtonDown(nFlags, point);
	}

#endif
	Invalidate(FALSE);
}

void CTPVUSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_uPrivateStyle & TP_VUSLIDERCTRL_NOTHUMB) return;
#ifndef TP_SMARTSLIDER_CONTORL
	if(m_bPressed )
	{
		if( (m_uStyle & TBS_TOOLTIPS) && m_showTips)
		{
			m_showTips ->UpdateTipText(LPSTR_TEXTCALLBACK,this,1);
		}
		ReleaseCapture();
		GetParent() ->SendMessage(WM_VSCROLL,MAKELONG(SB_ENDSCROLL,0),(LPARAM)m_hWnd);
		m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,1,(LPARAM)this);	
		m_bPressed = FALSE;
	}
	if(m_hClickTime) KillTimer(m_hClickTime);
	m_hClickTime = NULL;
	m_iClickPos  = 0;
	if(!m_bEsc) CTPSliderBar::OnLButtonUp(nFlags, point);
#else

	if(m_bPressed)
	{
		if( (m_uStyle & TBS_TOOLTIPS) && m_showTips)
		{
			m_showTips ->UpdateTipText(LPSTR_TEXTCALLBACK,this,1);
		}
		m_bPressed = FALSE;
	}
	if(m_hClickTime) KillTimer(m_hClickTime);
	m_hClickTime = NULL;
	m_iClickPos  = 0;

	int nLastOperateState = m_nOperateState;
	m_nOperateState = Op_Nothing;

	if(nLastOperateState == Op_Original)
	{
		if(!(m_uStyle & TBS_VERT))
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD) GetPos()),(LPARAM)m_hWnd);
		else
			m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD) GetPos()),(LPARAM)m_hWnd);
	//	CTPSliderBar::OnLButtonUp(nFlags, point);
	}
	else if(nLastOperateState == Op_Intelligent)
	{
		CTPMouseDeviceHooker* pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker();
		if(pHooker != NULL)
			pHooker->ExitHook();

		ReleaseCapture();

		if(m_bRecived && pHooker)
		{
			CalcValue(pHooker->GetCurrentPosReal());
		}
		else
		{
			ClientToScreen(&point);
			SEPOINTREAL pt = SEPOINTREAL(point.x, point.y );
			CalcValue(pt);
		}

		if(!(m_uStyle & TBS_VERT))
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD) GetPos()),(LPARAM)m_hWnd);
		else
			m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD) GetPos()),(LPARAM)m_hWnd);

		m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,1,(LPARAM)this);
	}
#endif
}
void CTPVUSliderCtrl::SetCursorPosition(CPoint &point)
{
	Invalidate();
	UpdateWindow();
	point.y = m_rtThumb.CenterPoint().y;
	ClientToScreen(&point);
	SetCursorPos(point.x,point.y);
	GetCursorPos(&point);	
	ScreenToClient(&point);
}
int CTPVUSliderCtrl::GetMicroSetp(int iDragPos,CPoint &ptCur)
{
	if(m_ptDrag.y == ptCur.y) return m_iPos;

	double  dDBDrag = GetDB();
	double  sStep   = 0.01;

	if(abs(dDBDrag)>=10) sStep    = 0.1;
	else                 sStep    = 0.01;

	if(!(m_uStyle & TBS_VERT))
	{
		if(m_ptDrag.x > ptCur.x)  dDBDrag -=  sStep;
		else                      dDBDrag +=  sStep;
	}
	else
	{
		if(m_ptDrag.y < ptCur.y)  dDBDrag -=  sStep;
		else                      dDBDrag +=  sStep;
	}
	
	return (int)DBToPosComplex(dDBDrag);	
}
void CTPVUSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bEsc) return;	
	m_dDBSet = 0;
	if(m_uPrivateStyle & TP_VUSLIDERCTRL_NOTHUMB) return;
	if(m_hClickTime)
	{
		KillTimer(m_hClickTime);
		m_hClickTime = NULL;
		SetPos(m_iClickPos);
		Invalidate();
		UpdateWindow();
		if(!(m_uStyle & TBS_VERT))
		{
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)m_iClickPos),(LPARAM)m_hWnd);
			m_iDragSave  = point.x - m_rtThumb.left;
		}
		else
		{
			m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)m_iClickPos),(LPARAM)m_hWnd);
			m_iDragSave  = point.y - m_rtThumb.top;
		}
		m_hClickTime = NULL;
		m_iClickPos  = 0;
	}
	CTPSliderBar::OnMouseMove(nFlags, point);
	if(m_bPressed && m_showTips)
	{
		CString sText;
		if(m_uStyle & TBS_VERT) 	sText.Format(_L("%.1f"), GetDB());
		else                    	sText.Format(_L("%.1f"), GetDB());
		m_showTips ->UpdateTipText(sText,this,1);
	}
}

BOOL CTPVUSliderCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(m_showTips && m_showTips ->m_hWnd)	m_showTips ->RelayEvent(pMsg);
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		if(m_bPressed)
		{
			m_uBaseState |= TP_BASESTATE_NOSCROLL;
			OnLButtonUp(0, CPoint(0,0));
			m_uBaseState &= ~TP_BASESTATE_NOSCROLL;
			SetPos(m_iEscPos);
			m_bEsc = TRUE;
			Invalidate();
			m_dDBSet = 0;
			return TRUE;
		}
	}
	if(pMsg->message == WM_KEYDOWN)
	{
		if(!(TP_BASESTATE_TEMPLATE & m_uBaseState))
		{
			switch(pMsg->wParam)
			{
			case VK_HOME:   m_uBaseState |= TP_BASESTATE_TEMPLATE; SendMessage(WM_KEYDOWN,VK_END,0);   m_uBaseState &= ~TP_BASESTATE_TEMPLATE; m_dDBSet = 0;return TRUE;
			case VK_END:    m_uBaseState |= TP_BASESTATE_TEMPLATE; SendMessage(WM_KEYDOWN,VK_HOME,0);  m_uBaseState &= ~TP_BASESTATE_TEMPLATE; m_dDBSet = 0;return TRUE;
			case VK_UP:     m_uBaseState |= TP_BASESTATE_TEMPLATE; SendMessage(WM_KEYDOWN,VK_UP,0);  m_uBaseState &= ~TP_BASESTATE_TEMPLATE; m_dDBSet = 0;return TRUE;
			case VK_DOWN:   m_uBaseState |= TP_BASESTATE_TEMPLATE; SendMessage(WM_KEYDOWN,VK_DOWN,0);    m_uBaseState &= ~TP_BASESTATE_TEMPLATE; m_dDBSet = 0;return TRUE;
			case VK_PRIOR:  m_uBaseState |= TP_BASESTATE_TEMPLATE; SendMessage(WM_KEYDOWN,VK_PRIOR,0);  m_uBaseState &= ~TP_BASESTATE_TEMPLATE; m_dDBSet = 0;return TRUE;
			case VK_NEXT:   m_uBaseState |= TP_BASESTATE_TEMPLATE; SendMessage(WM_KEYDOWN,VK_NEXT,0); m_uBaseState &= ~TP_BASESTATE_TEMPLATE; m_dDBSet = 0;return TRUE;
			default:
				break;
			}
		}

		if(pMsg->wParam == VK_CONTROL)
		{
			CTPMouseDeviceHooker* pHooker = NULL;
			pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker();
			if(pHooker)
				pHooker->SetCtrl(TRUE);
		}
	}
	if(pMsg->message == WM_KEYUP||pMsg->message == WM_RBUTTONDOWN ||pMsg->message == WM_LBUTTONDOWN)
	{
		//if(pMsg->wParam == VK_CONTROL)
		{
			CTPMouseDeviceHooker* pHooker = NULL;
			pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker();
			if(pHooker)
				pHooker->SetCtrl(FALSE);
		}
	}
	return CTPSliderBar::PreTranslateMessage(pMsg);
}

void CTPVUSliderCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTPSliderBar::OnSize(nType, cx, cy);
	if(m_bSubWindow) CTPVUSliderCtrl::CalcSize(0x01);
	if(m_showTips && ::IsWindow(m_showTips ->m_hWnd))
	{
		m_showTips ->SetToolRect(this,1,CRect(0,0,cx,cy));
	}

#ifdef  TP_SMARTSLIDER_CONTORL
	InitIntelligentResource();
#endif
}
BOOL CTPVUSliderCtrl::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	CString sText;

	double dTemp[15];
	for( int i = 0; i < m_iStepCount; ++ i)
		dTemp[i] = pow( 10.0, m_pdStepValue[i] / 20);
	double db = PosToValue( PixelToPos( m_uStyle & TBS_VERT ? ptCursor.y : ptCursor.x), dTemp, m_iStepCount);
	db = 20 * log10( db);

	sText.Format(_L("%.2f"),db);
	if(db>=0) sText = _L("+") +sText;

	lstrcpy( pTTT->szText,sText);
	return  TRUE;
} 
void CTPVUSliderCtrl::CalcSize(UINT uFlag)
{
	if(uFlag & SLIDER_CHANNEL)
	{
		GetClientRect(&m_rtClient);
		GetChannelRect(m_rtChannel);
		m_rtThumb = m_rtChannel;
	}
	else if(uFlag & SLIDER_THUMB)
	{
		CSliderCtrl::GetRange(m_iMin,m_iMax);
		m_iPos  = CSliderCtrl::GetPos();
		if(m_uStyle & TBS_VERT)
		{
			if(m_rtChannel.Height() < m_pThumbBmp[SLIDER_OVER] ->Height()) return;
			if(m_iMax > m_iMin)
			{
				m_iPagePixel = m_pThumbBmp[SLIDER_OVER] ->Height();
				m_iPage      = m_iPagePixel * (m_iMax - m_iMin) /(m_rtChannel.Height() - m_iPagePixel);

				m_rtThumb.top    = PosToPixel(m_iPos);
				m_rtThumb.bottom = m_rtThumb.top + m_iPagePixel;
			}
			else
			{
				m_rtThumb.left  = m_rtChannel.left;
				m_rtThumb.right = m_rtChannel.right;
				m_iPagePixel    = m_rtChannel.Width();
			}
			if(m_pThumbBmp[SLIDER_OVER])
			{
				m_rtThumb.top    = m_rtThumb.top + (m_rtThumb.Height() - m_pThumbBmp[SLIDER_OVER] ->Height())/2;
				m_rtThumb.bottom = m_rtThumb.top + m_pThumbBmp[SLIDER_OVER] ->Height();
				m_rtThumb.left   = m_rtChannel.CenterPoint().x - m_pThumbBmp[SLIDER_OVER] ->Width()/2;
				m_rtThumb.right  = m_rtChannel.CenterPoint().x + m_pThumbBmp[SLIDER_OVER] ->Width()/2;

				m_rtChannel.left  = m_rtThumb.left  +1;
				m_rtChannel.right = m_rtThumb.right -1;
			}
		}
	}
}
void CTPVUSliderCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_hClickTime) KillTimer(m_hClickTime);
	m_hClickTime = NULL;
	m_iClickPos  = 0;
	if(m_uPrivateStyle & TP_VUSLIDERCTRL_NOTHUMB) return;
	GetParent() ->PostMessage(WM_SLIDERCTRLMSG,WM_SLIDERCTRL_LBD,LPARAM(m_hWnd));
	//CTPSliderBar::OnLButtonDblClk(nFlags, point);
}

void CTPVUSliderCtrl::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1000)
	{
		KillTimer(m_hClickTime);
		m_hClickTime = NULL;
		SetPos(m_iClickPos);
		Invalidate();
		UpdateWindow();
		if(!(m_uStyle & TBS_VERT))
		{
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)m_iClickPos),(LPARAM)m_hWnd);
			m_iDragSave  = m_ptDragSave.x - m_rtThumb.left ;
		}
		else
		{
			m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)m_iClickPos),(LPARAM)m_hWnd);
			m_iDragSave  = m_ptDragSave.y - m_rtThumb.top;
		}
		m_hClickTime = NULL;
		m_iClickPos  = 0;
	}
	else
		CTPSliderBar::OnTimer(nIDEvent);
}
void CTPVUSliderCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)//add by lizhiqiao for:按上下键 滑块移动反向的问题。2008.09.25
{
	if(VK_UP == nChar || VK_DOWN == nChar || VK_NEXT == nChar || VK_PRIOR == nChar||VK_HOME == nChar|| VK_END == nChar)
	{
		 double dDBValue = GetDB();
		if(nChar == VK_UP)
			dDBValue+=m_dStep;
		else if(nChar == VK_DOWN)
			dDBValue-=m_dStep;

		dDBValue = min(dDBValue, m_dValMax);
		dDBValue = max(dDBValue, m_dValMin);
		SetDB(dDBValue);

		/*jyn delete 20090817
		*@brief: 不要pgUp,pgDn, Home,End
		*/
		/*else if(nChar == VK_NEXT)   
			SetPos(GetPos()-1000);
		else if(nChar == VK_PRIOR)  
			SetPos(GetPos()+1000);*/
		/*jyn delete end*/

		Invalidate();

		//jyn changed 20090420
		//m_pMsgWnd->SendMessage(WM_VUSLIDERCTRL,0x003,(LPARAM)this);	//add lzq:按 上下按钮 界面不响应。2008.12.24
		m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)0xFF),(LPARAM)m_hWnd);	
		//jyn changed end
	}
	else 
		CTPSliderBar::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CTPVUSliderCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	double  dDBDrag = GetDB();
	double  sStep   = 0.01;

	if(abs(dDBDrag)>10)  sStep    = 0.1;
	else                 sStep    = 0.01;
	if(zDelta<0)         dDBDrag -=  sStep;
	else                 dDBDrag +=  sStep;
	SetPos((int)DBToPosComplex(dDBDrag));
#ifdef TP_SMARTSLIDER_CONTORL
	m_dValue = Pos2Value(GetPos());
#endif
	Invalidate();
	m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)0xFF),(LPARAM)m_hWnd);
	return TRUE;
}

int CTPVUSliderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPSliderBar::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifdef  TP_SMARTSLIDER_CONTORL
	InitIntelligentResource();
#endif
	return 0;
}

void CTPVUSliderCtrl::PreSubclassWindow()
{
	CTPSliderBar::PreSubclassWindow();

#ifdef  TP_SMARTSLIDER_CONTORL
	InitIntelligentResource();
#endif
}

#ifdef  TP_SMARTSLIDER_CONTORL
BOOL CTPVUSliderCtrl::CanIntelligentOperation()
{
	if((m_dValMax - m_dValMin) == 0.0) return FALSE;
	if(!m_nPerWidth) return FALSE;

	return TRUE;
}

void CTPVUSliderCtrl::InitIntelligentResource()
{
	CRect rcChannel;
	CSliderCtrl::GetChannelRect(&rcChannel);

	m_nPerWidth = rcChannel.Width();
}

void CTPVUSliderCtrl::InitCalcVal()
{
	m_dStartVal = m_dValue;
	m_dStackValue = 0.0;
	m_bRecived = FALSE;
}

void    CTPVUSliderCtrl::SetStep(double dStep)
{
	m_dStep = dStep;
}

LRESULT CTPVUSliderCtrl::OnMouseDeviceHookCursorMove(WPARAM wParam, LPARAM lParam)
{
	MSG	msg;
	while(::PeekMessage(&msg, GetSafeHwnd(), WM_SE_MOUSEHOOK, WM_SE_MOUSEHOOK, PM_REMOVE));

	if(m_nOperateState == Op_Intelligent)
	{
		if(m_bEsc) return S_FALSE;
		m_dDBSet = 0;
		if(m_uPrivateStyle & TP_VUSLIDERCTRL_NOTHUMB) return S_FALSE;

		m_bRecived = TRUE;
		CTPMouseDeviceHooker* pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker();
		if(pHooker != NULL)
			CalcValue(pHooker->GetCurrentPosReal());

		if(m_bPressed && m_showTips)
		{
			CString sText;
			if(m_uStyle & TBS_VERT)   sText.Format(_L("%.1f"), GetDB());
			else                      sText.Format(_L("%.1f"), GetDB());
			m_showTips->UpdateTipText(sText,this,1);
		}
		Invalidate(FALSE);
	}

	return 0;
}

int CTPVUSliderCtrl::Value2Pos(double dValue)
{
	double dRange = m_dValMax - m_dValMin;
	int nMax(0), nMin(0);
	GetRange(nMin, nMax);
	double dRange2 = nMax - nMin;

	if((dRange == 0.0 ) || (dRange2 == 0.0)){
		return 0;
	}

	double dPosReal = m_dValue / dRange;

	int nPos =0;
	if(dPosReal>0)
		nPos = (int)(dRange2 * dPosReal + 0.5);
	else
		nPos = (int)(dRange2 * dPosReal - 0.5);

	if(nPos < nMin)
		nPos = nMin;
	if(nPos > nMax)
		nPos = nMax;

	return nPos;
}

double CTPVUSliderCtrl::Pos2Value(int nPos)
{
	int nMax(0), nMin(0);
	GetRange(nMin, nMax);

	int nRange = nMax - nMin;
	double dRange = m_dValMax - m_dValMin;
	if(!nRange || (dRange == 0.0))
		return 0.0;

	double dPerVal = (double)nPos / nRange;
	double dValue = dPerVal * dRange;

	if(dValue > m_dValMax)
		dValue = m_dValMax;

	if(dValue < m_dValMin)
		dValue = m_dValMin;

	return dValue;
}

void CTPVUSliderCtrl::CalcValue(const SEPOINTREAL& point)
{
	SEPOINTREAL ptDiff;
	ptDiff.x = m_ptPrevPoint.x - point.x;
	ptDiff.y = m_ptPrevPoint.y - point.y;

	m_ptPrevPoint = point;

	double dDiffVal = 0;

	if(m_uStyle & TBS_VERT)
		dDiffVal = ptDiff.y;
	else
		dDiffVal = ptDiff.x;

	if(dDiffVal == 0.0) return;

	m_dStackValue += dDiffVal;

	double dRange(0.0), dPer(0.0), dValue(m_dStartVal);

	dRange = (m_dValMax - m_dValMin);

	if(m_nPerWidth)
		dPer = dRange / m_nPerWidth;

	if(m_uStyle & TBS_VERT)
		dValue += (m_dStackValue * dPer);
	else
		dValue -= (m_dStackValue * dPer);

	if(SetValue(dValue))
	{
		if(!(m_uStyle & TBS_VERT))
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD) GetPos()),(LPARAM)m_hWnd);
		else
			m_pMsgWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD) GetPos()),(LPARAM)m_hWnd);
	}
}

void CTPVUSliderCtrl::SetValueRange(double dMin, double dMax)
{
	m_dValMin = dMin;
	m_dValMax = dMax;
}
#endif
