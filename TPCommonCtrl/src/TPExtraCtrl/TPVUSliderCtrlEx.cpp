#include "StdAfx.h"
#include ".\tpvusliderctrlex.h"
#include <math.h>

double TP_NS_StepDB[]   = { -100, -60, -51, -40, -30, -20, -10,  -5,   0,   5,  10};
double TP_NS_StepTick[] = { 0.00,0.04,0.09,0.16,0.29,0.39,0.51,0.63,0.76,0.88,1.00};

CTPVUSliderCtrlEx::CTPVUSliderCtrlEx(void)
{
	m_iMin          = 0;
	m_iMax          = 100;
	m_iPage         = 0;
	m_iType         = 1;
	m_iTextColor    = COLOR_YELLOW;
	m_rtBackChannle = CRect(0,0,0,0);
	m_rtDBText      = CRect(0,0,0,0);
	m_iPosSave      = -1;
	m_dDBSave       = -1;
	m_iFontIndex    = FONT_BASE_TEXT1;
	m_pBackRes      = NULL;
	m_pTicLinePen   = NULL;
	m_pThumbBmp[0]  = NULL;
	m_rtBtnPos      = CRect(2,225,49,248); //xy
	m_pEditName     = NULL;

	m_dDBSave_SliderStop = -1; //added by lizhiqiao;2008.05.21;

	m_iRefCount++;
	if(m_iRefCount ==1)
	{
		m_pLargIcon = TP_LoadIcon(TP_GetSkinPath() + _T("\\CommonCtrl\\VUTable\\Larg.ico"));
	}
}

CTPVUSliderCtrlEx::~CTPVUSliderCtrlEx(void)
{
	TP_ReleaseCommonRes(m_pBackRes);     m_pBackRes     = NULL;
	TP_ReleaseCommonRes(m_pTicLinePen);  m_pTicLinePen  = NULL;
	TP_ReleaseCommonRes(m_pThumbBmp[0]); m_pThumbBmp[0] = NULL;
	m_iRefCount--;
	if(m_iRefCount ==0)
	{
		TP_ReleaseCommonRes(m_pLargIcon);
		m_pLargIcon = NULL;
	}
}
void     CTPVUSliderCtrlEx::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_SLIDERBAR,&wndClass);
		wndClass.lpszClassName = WC_VUSLIDERCTRLEX;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_VUSLIDERCTRLEX,AfxGetInstanceHandle());
}

void CTPVUSliderCtrlEx::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		CTPVUSliderCtrl::SubWindow(bSub);
		CTPSliderBar::ModifyStyle(0,TBS_ENABLESELRANGE | TBS_AUTOTICKS | TBS_VERT|SLIDER_THUMB |TBS_TOOLTIPS,0);
		m_uStyle = CTPSliderBar::GetStyle() & (~TBS_NOTICKS);
		m_uStyle |= TBS_LEFT|TBS_VERT;
		LoadThumb();

		if(m_pMsgWnd == NULL) m_pMsgWnd = GetParent();
		SetDB(-23);
	}
	else
	{
		if(m_pEditName)
		{
			m_pEditName ->DestroyWindow();
			delete m_pEditName;
			m_pEditName = NULL;
		}
		CTPVUSliderCtrl::SubWindow(bSub);
	}
}
void CTPVUSliderCtrlEx::LoadThumb()
{
	TP_ReleaseCommonRes(m_pBackRes);
	TP_ReleaseCommonRes(m_pTicLinePen);
	TP_ReleaseCommonRes(m_pThumbBmp[0]);
	if(m_iType == 1)
	{
		m_pBackRes      = TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\VUSliderBack1.bmp"));
		m_pThumbBmp[0]  = TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\VUSliderTick.bmp"));
		m_rtBackChannle = CRect(32,26,52,200);
		m_rtChannel     = m_rtBackChannle;
		m_rtDBText      = CRect(11,7,47,20);/*CRect(12,7,44,20);*///mod by lizhiqiao,数值显示不全。2009.03.06
		m_bPaintLable   = TRUE;
		m_iTickEdge     = 1;
		m_iTickWidth    = 5;
	}
	else if(m_iType == 2)
	{
		m_pBackRes      = TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\VUSliderBack2.bmp"));
		m_pThumbBmp[0] =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\VUSlider\\VUSliderTick.bmp"));
		m_rtBackChannle = CRect(28,29,42,204);
		m_rtChannel     = m_rtBackChannle;
		m_rtDBText      = CRect(3,8,45,22);/*CRect(5,8,43,22);*///mod by lizhiqiao 2009.03.06
		m_bPaintLable   = FALSE;
		m_iTickEdge     = 4;
		m_iTickWidth    = 10;
	}
	m_pTicLinePen    = TP_LoadPen(COLOR_BASE_TEXT,PS_SOLID,1);
	
}
void CTPVUSliderCtrlEx::GetChannelRect(CRect &rtChannel)
{
	rtChannel = m_rtBackChannle;
}
void CTPVUSliderCtrlEx::PaintTicks(CDC *pDC)
{
	int     iTick = m_iStepCount;
	CPoint  ptText;
	CRect   rtThumb;
	CString sText;
	int     iPosLine=0,iPosTick=0;

	CFont   *pFnt  = pDC ->SelectObject(TP_GetSysFont(FONT_BASE_TEXT4));
	CSize   szText = pDC->GetTextExtent(_L("9"));
	rtThumb = m_rtThumb;
	pDC->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
	if(m_uStyle & TBS_VERT)
	{
		if(m_uStyle & TBS_LEFT)    iPosLine = rtThumb.left   - m_iTickEdge;
		else                       iPosLine = rtThumb.right  + m_iTickEdge;
	}
	else
	{
		if(m_uStyle & TBS_TOP)    iPosLine = rtThumb.top    - m_iTickEdge;
		else                      iPosLine = rtThumb.bottom + m_iTickEdge;
	}
	CPen *pSavePen = pDC->SelectObject(CPen::FromHandle(m_pTicLinePen ->hPen));
	for(INT l=0;l<iTick;l++)
	{
		if(m_uStyle & TBS_VERT)
		{
			iPosTick = TP_DoubleToInt(m_rtChannel.top +rtThumb.Height()/2 + (m_rtChannel.Height() - rtThumb.Height())* (m_pdStepTicks[l] - m_pdStepTicks[m_iStepCount-1]) /(m_pdStepTicks[0] - m_pdStepTicks[m_iStepCount-1]))+1;

			if(m_uStyle & TBS_LEFT)
			{
				pDC->MoveTo(iPosLine,iPosTick);pDC->LineTo((l == 0 || l == iTick - 1) ?  iPosLine-m_iTickWidth-1 : iPosLine-m_iTickWidth,iPosTick);
			}
			else
			{
				pDC->MoveTo(iPosLine,iPosTick);pDC->LineTo((l == 0 || l == iTick - 1) ?  iPosLine+m_iTickWidth+1 : iPosLine+m_iTickWidth,iPosTick);
			}

			if(m_uStyle & TBS_LEFT)
			{
				ptText = CPoint(iPosLine - m_iTickWidth - 4 * szText.cx,iPosTick - szText.cy/2);
			}
			else
			{
				ptText = CPoint(iPosLine-2,iPosTick -szText.cy);
			}

			/*disabled by lizhiqiao ,move down.bug:XN00019264 2009.02.18，当音量调到最小时，应该显示"--"，同Audio Mixer一致。
			if(m_pdStepValue[ l ]<=-120) sText = _T("-∞");
			else                         sText.Format(_L("%.0f"),m_pdStepValue[l]);
			*/

			//add by lizhiqiao bug:XN00019264 2009.02.18，当音量调到最小时，应该显示"--"，同Audio Mixer一致。
			if(m_pdStepValue[ l ]<=-100) sText = _T("--");
			else                         sText.Format(_L("%.0f"),m_pdStepValue[l]);
		}

		if( m_bPaintLable)
		{
			CRect rtText = CRect(ptText.x ,ptText.y,ptText.x +  szText.cx *4,ptText.y + szText.cy);
			if(m_pdStepValue[ l ]<=-100) pDC->DrawIcon(rtText.right-15,rtText.top+4,m_pLargIcon->GetIcon());
			else                         pDC ->DrawText(sText,rtText,DT_RIGHT|DT_TOP);
		}
	}
	pDC ->SelectObject(pFnt);
	pDC ->SelectObject(pSavePen);

}
double  CTPVUSliderCtrlEx::GetDB()
{
	return PosToDB(CTPSliderBar::GetPos());
}
void  CTPVUSliderCtrlEx::SetDB(double dDB,BOOL bComplex)
{
	CTPVUSliderCtrl::SetDB(dDB,bComplex);//jyn changed 20090513
}

void CTPVUSliderCtrlEx::PaintCtrl(CDC *pDC)
{
	CRect rtClient,rtBar;
	GetClientRect(rtClient);
	m_rtClient  = rtClient;
	m_rtChannel = CRect(m_rtClient.left + m_rtBackChannle.left,m_rtClient.top + m_rtBackChannle.top,m_rtClient.left + m_rtBackChannle.right,m_rtClient.bottom - (m_pBackRes->Height() - m_rtBackChannle.bottom));

#ifdef TP_SMARTSLIDER_CONTORL
	m_nPerWidth = m_rtChannel.Height()-m_rtThumb.Height();
#endif

	CTPSliderBar::GetRange(m_iMin,m_iMax);
	m_iPos = CTPSliderBar::GetPos();
	if(m_iPosSave != m_iPos)
		CTPVUSliderCtrl::CalcSize(SLIDER_THUMB);
	rtBar = m_rtClient;
	rtBar.right = rtBar.left + m_pBackRes->Width();

	pDC->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_FRAME));
	TP_StretchDIBRect(pDC,m_pBackRes->GetByte(),CRect(0,0,m_pBackRes->Width(),m_pBackRes->Height()),m_pBackRes->Width(),m_pBackRes->Height(),m_rtBackChannle.top+20,m_pBackRes->Height() - m_rtBackChannle.bottom + 20,rtBar,TRUE);
	TP_SetDIBitsToDevice(pDC,m_pThumbBmp[0] ->GetByte(),m_pThumbBmp[0]->Width(),m_pThumbBmp[0]->Height(),m_rtThumb);

	CFont *pFont = pDC->SelectObject(TP_GetSysFont(FONT_BASE_TEXT4));
	PaintTicks(pDC);
	pDC->SelectObject(TP_GetSysFont(m_iFontIndex));
	pDC->SetTextColor(TP_GetSysColor(m_iTextColor));
	if(m_iPosSave != m_iPos)
	{
		m_dDBSave  = PosToDB(m_iPos);
		m_iPosSave = m_iPos;
	}
	CString sDB;

	//add by lizhiqiao,  bug:XN00019264 2009.02.18，当音量调到最小时，应该显示"--"，同Audio Mixer一致。
	if(m_dDBSave<=-100)                         sDB = _T("--");
	else if(m_dDBSave >-10 && m_dDBSave < 10)	sDB.Format((_T("%.2f")),m_dDBSave);
	else if(m_dDBSave >-100)                    sDB.Format((_T("%.2f")),m_dDBSave);
	else                                        sDB.Format((_T("%.0f")),m_dDBSave);

	pDC->DrawText(sDB,m_rtDBText,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	pDC->SelectObject(pFont);
}
BEGIN_MESSAGE_MAP(CTPVUSliderCtrlEx, CTPVUSliderCtrl)
ON_WM_KEYDOWN()
ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDOWN()
ON_WM_DESTROY()
ON_EN_KILLFOCUS(1000,OnEnEdit)
ON_MESSAGE(WM_TIMECODE_CHANGE,OnEnEditEnter)
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//
void CTPVUSliderCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTPVUSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();//add lzq:bug:XN00017453,按Up down箭头键时，滑块不移动。2008.12.23
}
LRESULT CTPVUSliderCtrlEx::OnEnEditEnter(WPARAM wp,LPARAM lp)//LZQ:编辑框按回车时，响应此函数。2008.07.29
{
	if(wp == TP_TIMECODE_RETURN)
	{
		SetFocus();
		//added by lizhiqiao;对输入编辑框中的音量做限制范围；2008.07.29；
		//end of added by lizhiqiao;
		if( GetParent()) 
			GetParent()->PostMessage(WM_TPEDIT, TPEDIT_RETURE, (LPARAM) m_hWnd);
	}
	return S_OK;
}
void   CTPVUSliderCtrlEx::OnEnEdit()//LZQ:焦点从Slider切换走时，响应此函数。2008.07.29
{
	CString  sText;
	m_pEditName->GetWindowText(sText);
	m_pEditName ->ShowWindow(SW_HIDE);
	double dGain = _wtof((LPTSTR)(LPCTSTR)sText);
	dGain = max(-100,dGain);
	dGain = min(10,dGain);
	SetDB(dGain);
	m_dDBSave = dGain;
	m_dDBSave_SliderStop = dGain;//added by lizhiqiao,on2008.05.21;

	m_pEditName ->ShowWindow(SW_HIDE);
	Invalidate();

	if( GetParent()) 
		GetParent()->PostMessage(WM_TPEDIT, TPEDIT_RETURE, (LPARAM) m_hWnd);
}
void CTPVUSliderCtrlEx::OnLButtonDB(CPoint point)
{
	if(m_rtDBText.PtInRect(point))
	{
		if(m_pEditName == NULL)
		{
			m_pEditName = new CTPEditEx();
			m_pEditName ->m_uPrivateStyle |= ES_WANTRETURN;
			m_pEditName ->m_uEditStyle    |= ES_RETURNNOSUND;
			m_pEditName ->Create(WS_CHILD|ES_WANTRETURN|ES_AUTOHSCROLL,CRect(0,0,0,0),this,1000);
			m_pEditName ->m_iFontIndex = FONT_BASE_TEXT4;
			m_pEditName ->SetFont(TP_GetSysFont(FONT_BASE_TEXT4)); 
		}
		CString sDB;
		sDB.Format((_T("%.2f")),m_dDBSave);	
		m_pEditName ->SetIncludeKey(_T("1234567890.-"));
		m_pEditName ->MoveWindow(m_rtDBText);
		m_pEditName ->SetWindowText(sDB);
		m_pEditName ->SetSel(0,sDB.GetLength());
		m_pEditName ->ShowWindow(SW_SHOW);
		m_pEditName ->SetFocus();
		m_pEditName ->SetRange(-100,10);
		m_pEditName->Invalidate();
	}
	else
	{
		m_dDBSave = 0;
		m_dDBSave_SliderStop = 0;//lzq;
		SetDB(0);
		Invalidate();
	}
}
void CTPVUSliderCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_rtDBText.PtInRect(point))
	{
		OnLButtonDB(point);
	}
	else
	{
		double dGain = 0.0;
		SetDB(dGain);
		m_dDBSave = dGain;
		m_dDBSave_SliderStop = dGain;//added by lizhiqiao;

		Invalidate();
		GetParent() ->PostMessage(WM_SLIDERCTRLMSG,WM_SLIDERCTRL_LBD,LPARAM(m_hWnd));
	}
}

void CTPVUSliderCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(GetFocus()!=this) SetFocus();
	if(point.y< m_rtDBText.bottom) return;
	if(m_rtBtnPos.PtInRect(point))
		GetParent()->SendMessage(WM_TPVUSLIDERCTRLEX,TP_TPVUSLIDERCTRLEX_LDOWN,(LPARAM)m_hWnd);
	else
		CTPVUSliderCtrl::OnLButtonDown(nFlags, point);

	//added by lizhiqiao;for bug 3773 on2008.05.22
	if(m_pEditName == NULL)
	{
		m_pEditName = new CTPEditEx();
		m_pEditName ->m_uPrivateStyle |= ES_WANTRETURN;
		m_pEditName ->Create(WS_CHILD|ES_WANTRETURN|ES_AUTOHSCROLL,CRect(0,0,0,0),this,1000);
		m_pEditName ->m_iFontIndex = FONT_BASE_TEXT4;
		m_pEditName ->SetFont(TP_GetSysFont(FONT_BASE_TEXT4)); 
	}
	//added by lizhiqiao;for bug 3773 on2008.05.22
}

void CTPVUSliderCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_rtBtnPos.PtInRect(point))
		GetParent()->SendMessage(WM_TPVUSLIDERCTRLEX,TP_TPVUSLIDERCTRLEX_RDOWN,(LPARAM)m_hWnd);
	else
		CTPVUSliderCtrl::OnRButtonDown(nFlags, point);
}

void CTPVUSliderCtrlEx::OnDestroy()
{
	if(m_pEditName)
	{
		m_pEditName ->DestroyWindow();
		delete m_pEditName;
		m_pEditName = NULL;
	}
	CTPVUSliderCtrl::OnDestroy();
}

BOOL CTPVUSliderCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE||((pMsg->wParam == VK_Z)&&GetKeyState(VK_CONTROL)< 0))&& m_pEditName)
	{
		// by lizhiqiao on2008.05.21,for bug 3773
		if (m_bPressed)//鼠标左键压下
		{
			if(pMsg->wParam == VK_ESCAPE)
			{
				CString sDB;
				sDB.Format((_T("%.2f")),m_dDBSave_SliderStop);
				
				m_pEditName ->SetFocus();
				m_pEditName ->SetWindowText(sDB);
				m_pEditName ->ShowWindow(SW_SHOW);
				m_pEditName->Invalidate();
				SetFocus();
			}
		}// by lizhiqiao on2008.05.21 for bug 3773	
		else
		{
			if (GetFocus()== (CWnd*)m_pEditName)//LZQ:编辑框处于编辑状态，恢复原来值并退出编辑状态。
			{
				//===Modified by yuys on 2007-10-19
				CString sText;
				sText.Format(_L("%.2f"), GetDB());

				m_pEditName ->SetFocus();
				m_pEditName->SetWindowText(sText);//sText
				m_pEditName->SetSel(0,sText.GetLength());

				if(pMsg->wParam == VK_ESCAPE)
				{		
					m_pEditName ->ShowWindow(SW_SHOW);			
					SetFocus();
				}
			}
			else//LZQ:added by lizhiqiao for bug:XN00013305;关闭父对话框2008.08.19
			{
				// LI SONG MING 2009-3-18 XN00020214 
				if(pMsg->wParam == VK_ESCAPE)
				{
					if ( NULL != GetOwner()) 		
					{		
						GetOwner()->PostMessage(WM_KEYDOWN,(WPARAM)VK_ESCAPE,0);
						//GetOwner()->GetParent()->PostMessage(WM_CLOSE);//LZQ：此方法不行，Import File窗口里会退出系统。2008.08.19
					}
				}
			}			
		}
		
		return TRUE;
	}

	return CTPVUSliderCtrl::PreTranslateMessage(pMsg);
}

void CTPVUSliderCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
//	if(point.y<m_rtDBText.bottom) return; // LI HUI LI 2009-06-03 XN00021343 
	m_dDBSave_SliderStop = GetDB(); //by lizhiqiao for bug 3773
	CTPVUSliderCtrl::OnLButtonUp(nFlags, point);
}

void CTPVUSliderCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	//if(point.y< m_rtDBText.bottom) return; //del by lizhiqiao for bug:13000 2008.07.04
	CTPVUSliderCtrl::OnMouseMove(nFlags, point);
}
