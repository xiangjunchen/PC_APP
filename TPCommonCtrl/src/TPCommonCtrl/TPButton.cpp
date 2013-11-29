// TPButton.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPButton.h"
#include ".\tpbutton.h"




TPCommonRes            *m_pBtnResSave[BUTTON_TYPE_MAX][BUTTON_MAX]; 	
CPen                    m_hFocusPen; 

HWND         CTPButton::m_hPressBtn    = NULL;   
UINT         CTPButton::m_iAccBtnID    = 0;

IMPLEMENT_DYNAMIC(CTPButton, CButton)
CTPButton::CTPButton()
{
	m_pMsgWnd       = NULL;
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_pControlData  = NULL;

	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uState        = 0;
	m_uStyle        = 0;
	m_hTimerOver    = 0;
	m_hTimerAcc     = 0;
	m_eBackColor    = COLOR_BASE_FRAME;
	m_eTextColor    = COLOR_MAX;

	m_iImageIndex   = -1;
	m_bOver         = FALSE;
	m_uDelay        = 0;
	m_uInterval     = 0; 
	m_iDelayCount   = 0; 
	m_cBackIndex    = -1;
	m_cTextIndex    = -1;
	m_pbForceEnable = NULL;

	for(INT l=0;l<BUTTON_MAX;l++)
		m_pBtnRes[l] = NULL; 	

	m_bDefaultData   = TRUE;
	m_uPrivateStyle  = TPBTNTYPE_RDFOCUS;

	m_sToolTips[0] = _L("");
	m_sToolTips[1] = _L("");

	m_sText        = _L("");
	m_bVisible     = TRUE;
	m_iStateIndex  = -1;

	
	m_pButtonAccletCallback  = NULL;
	m_pCallBackParam         = NULL;
	m_pButtonToolTipCallBack = NULL;
	m_pToolTipParam          = NULL;
	m_pHotKeyData            = NULL;

	m_bLButtonDown   = FALSE;
	m_uControlState  = 0;
	m_ptStretchAlign = CPoint(0,0);

	m_clMask         = RGB(0,0,0);

	m_vHotKey        = 0;
	m_vKeyFlag       = 0;
}

CTPButton::~CTPButton()
{
	m_aButtonState.RemoveAll();
	if(m_bDefaultData ==FALSE)
	{	
		for(INT l=0;l<BUTTON_MAX;l++)
			TP_ReleaseCommonRes(m_pBtnRes[l]); 		
	}
	if(m_rgnWnd.m_hObject) m_rgnWnd.DeleteObject();
}


BEGIN_MESSAGE_MAP(CTPButton, CButton)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINT,OnPrint)
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()

	ON_MESSAGE(BM_SETSTATE,OnSetState)
	ON_MESSAGE(BM_SETCHECK,OnSetCheck)

	ON_WM_SETFOCUS()

	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_STYLECHANGED()

	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff,       OnToolTipNotify )

	ON_WM_ENABLE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_SHOWWINDOW()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// CTPButton 消息处理程序

void  CTPButton::OnStyleChanged(int nStyleType,  LPSTYLESTRUCT pStruct) 
{
	if(nStyleType == GWL_STYLE)
	{
		m_uStyle = pStruct ->styleNew;
		if(m_uPrivateStyle & TPBTNTYPE_GROUPBOX) return ;
		if((m_uPrivateStyle & TPBSKIN_CUSTOM) != TPBSKIN_CUSTOM)
		{		
			UINT uStyle = m_uPrivateStyle;
			m_uPrivateStyle &= ~TPBTNTYPE_CHECK;
			m_uPrivateStyle &= ~TPBTNTYPE_RADIO;
			m_uPrivateStyle &= ~TPBTNTYPE_GROUPBOX;
			m_uPrivateStyle &= ~TPBTNTYPE_DEFAULT;
			m_uPrivateStyle &= ~TPBTNTYPE_BUTTON;
			m_uPrivateStyle &= ~TPBTNTYPE_BUTTON;
			//决定按钮类型
			if((m_uStyle & 0x0000000f)  == BS_CHECKBOX ||  (m_uStyle & 0x0000000f)  == BS_AUTOCHECKBOX)
				m_uPrivateStyle |= TPBTNTYPE_CHECK;
			if((m_uStyle & 0x0000000f)  == BS_RADIOBUTTON ||  (m_uStyle & 0x0000000f)  == BS_AUTORADIOBUTTON)
				m_uPrivateStyle |= TPBTNTYPE_RADIO;
			if((m_uStyle & 0x0000000f)  == BS_GROUPBOX)
				m_uPrivateStyle |= TPBTNTYPE_GROUPBOX;
			if((m_uStyle & 0x0000000f)  == BS_DEFPUSHBUTTON)
				m_uPrivateStyle |= TPBTNTYPE_DEFAULT | TPBTNTYPE_BUTTON;
			if((m_uStyle & 0x0000000f)  == BS_PUSHBUTTON)
				m_uPrivateStyle |=  TPBTNTYPE_BUTTON;		
			if((uStyle & 0xff000000)!= (m_uPrivateStyle & 0xff000000) && m_bDefaultData) //判断按钮状态发生变化			
				LoadRes();							
		}
	}
	return ;
}
void   CTPButton::ResembleRes(TPCommonRes **pResData,int iImage)
{
	int iStateNum[] = {3,3,2,4,4,2,2,2};
	int iState[8][4]  =
	{
		{BUTTON_NOR,CHECK_NOR,STATE_NOR},
		{BUTTON_NOR,BUTTON_OVER,BUTTON_FOCUS},
		{BUTTON_NOR,BUTTON_DOWN},
		{CHECK_NOR,CHECK_DOWN,CHECK_OVER,CHECK_FOCUS},
		{STATE_NOR,STATE_DOWN,STATE_OVER,STATE_FOCUS},
		{BUTTON_NOR,BUTTON_DIS},
		{CHECK_NOR,CHECK_DIS},
		{STATE_NOR,STATE_DIS},		
	};
	if(pResData[BUTTON_NOR] == NULL) return;
	pResData[BUTTON_NOR]->uState |= TP_RESEMBLERES;
	for(INT l=0;l<8;l++)
	{
		for(INT k=1;k<iStateNum[l];k++)
		{
			if(iState[l][k] == iImage)
			{
				for(INT j=1;j<=k;j++)
				{
					if(pResData[iState[l][j]] == NULL || (!(pResData[iState[l][j]]->uState&TP_RESEMBLERES)) && TP_IsDefault(pResData[iState[l][j]]))
					{
						if(iState[l][j] == iState[l][j-1]) ASSERT(0);
						if(m_bDefaultData == FALSE)	TP_ReleaseCommonRes(pResData[iState[l][j]]);
						pResData[iState[l][j]] = pResData[iState[l][j-1]];
						if(pResData[iState[l][j]] != NULL)
						{						
							if(m_bDefaultData == FALSE)	pResData[iState[l][j]] ->dwRefCount++;
							pResData[iState[l][j]]->uState |= TP_RESEMBLERES;
						}
					}
				}	
				return;			
			}
		}
	}
}
void  CTPButton::ResembleRes(TPCommonRes **pResData)
{	
	return;
	//int iStateNum[] = {3,4,4,4,2,2,2};
	//int iState[7][4]  =
	//{
	//	{BUTTON_NOR,CHECK_NOR,STATE_NOR},
	//	{BUTTON_NOR,BUTTON_DOWN,BUTTON_OVER,BUTTON_FOCUS},
	//	{CHECK_NOR,CHECK_DOWN,CHECK_OVER,CHECK_FOCUS},
	//	{STATE_NOR,STATE_DOWN,STATE_OVER,STATE_FOCUS},
	//	{BUTTON_NOR,BUTTON_DIS},
	//	{CHECK_NOR,CHECK_DIS},
	//	{STATE_NOR,STATE_DIS},		
	//};
	//if(pResData[BUTTON_NOR] == NULL) return;
	//for(INT l=0;l<7;l++)
	//{
	//	for(INT k=1;k<iStateNum[l];k++)
	//	{
	//		if(TP_IsDefault(pResData[iState[l][k]]))
	//		{
	//		/*	if(bDefaultData == FALSE)	
	//				TP_ReleaseCommonRes(pResData[iState[l][k]]);*/
	//			pResData[iState[l][k]] = pResData[iState[l][k-1]];
	//			pResData[iState[l][k]] ->dwRefCount++;
	//		}
	//	}
	//}
}
void  CTPButton::LoadDefRes(BOOL bLoad)
{	
	if(bLoad)
	{	
		CString sBtnExt[] = 
		{
			_L("_NOR_NOR"),_L("_NOR_DOWN"),_L("_NOR_OVER"),_L("_NOR_FOCUS"),_L("_NOR_DIS"),_L("_DOWN_NOR"),_L("_DOWN_DOWN"),_L("_DOWN_OVER"),
				_L("_DOWN_FOCUS"),_L("_DOWN_DIS"),_L("_STATE_NOR"),_L("_STATE_DOWN"),_L("_STATE_OVER"),_L("_STATE_FOCUS"),_L("_STATE_DIS"),
		};		
		CString sPrex = _L("");

		sPrex = _L("\\CommonCtrl\\Button\\Check\\CheckBox");
		for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnResSave[BUTTON_TYPE_CHECK][l] = TP_LoadBmpByte(g_sSysSkinPath + sPrex + sBtnExt[l] + _L(".bmp"),FALSE,FALSE); 			
		ResembleRes(m_pBtnResSave[BUTTON_TYPE_CHECK]);
		sPrex = _L("\\CommonCtrl\\Button\\Radio\\Radio");
		for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnResSave[BUTTON_TYPE_RADIO][l] = TP_LoadBmpByte(g_sSysSkinPath + sPrex + sBtnExt[l] + _L(".bmp"),FALSE,FALSE); 			
		ResembleRes(m_pBtnResSave[BUTTON_TYPE_RADIO]);	
		sPrex = _L("\\CommonCtrl\\Button\\Square1\\Square1");
		for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnResSave[BUTTON_TYPE_SQUARE1][l] = TP_LoadBmpByte(g_sSysSkinPath + sPrex + sBtnExt[l] + _L(".bmp"),FALSE,FALSE); 			
		ResembleRes(m_pBtnResSave[BUTTON_TYPE_SQUARE1]);	
		sPrex = _L("\\CommonCtrl\\Button\\Square2\\Square2");
		for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnResSave[BUTTON_TYPE_SQUARE2][l] = TP_LoadBmpByte(g_sSysSkinPath + sPrex + sBtnExt[l] + _L(".bmp"),FALSE,FALSE); 			
		ResembleRes(m_pBtnResSave[BUTTON_TYPE_SQUARE2]);	


		if(m_hFocusPen.m_hObject == NULL)
			m_hFocusPen.CreatePen(PS_DOT,1,RGB(172,172,172));
	}
	else
	{
		for(INT k=0;k<BUTTON_TYPE_MAX;k++)
			for(INT l=0;l<BUTTON_MAX;l++)
				TP_ReleaseCommonRes(m_pBtnResSave[k][l]); 	
		if(m_hFocusPen.m_hObject)
			m_hFocusPen.DeleteObject();
	}
}
void  CTPButton::LoadRes()
{	
	if(m_bDefaultData == FALSE)
	{	
		for(INT l=0;l<BUTTON_MAX;l++)
		{
			TP_ReleaseCommonRes(m_pBtnRes[l]); 	
			m_pBtnRes[l] = NULL;
		}
	}	
	m_bDefaultData = TRUE;
	if((m_uPrivateStyle & TPBTNTYPE_CHECK) && !(BS_PUSHLIKE & m_uStyle))
	{			
		for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnRes[l] = m_pBtnResSave[BUTTON_TYPE_CHECK][l]; 			
	}
	else if((m_uPrivateStyle & TPBTNTYPE_RADIO) && !(BS_PUSHLIKE & m_uStyle))
	{			
		for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnRes[l] = m_pBtnResSave[BUTTON_TYPE_RADIO][l]; 					
	}
	else  if((m_uPrivateStyle & TPBTNTYPE_BUTTON) || (BS_PUSHLIKE & m_uStyle))
	{
		if(m_uPrivateStyle & TPBSKIN_SQUARE1)     { for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnRes[l] = m_pBtnResSave[BUTTON_TYPE_SQUARE1][l];} 			
		else if(m_uPrivateStyle & TPBSKIN_SQUARE2){ for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnRes[l] = m_pBtnResSave[BUTTON_TYPE_SQUARE2][l];} 						
	}
	else if(m_uPrivateStyle & TPBTNTYPE_GROUPBOX)
	{
		return;
	}	
}
void CTPButton::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		SaveFont(m_hWnd);
		 SetFont(TP_GetSysFont(m_iFontIndex));
		 m_bSubWindow = TRUE;	
		 m_uState  = CButton::GetState();
		 m_pMsgWnd = GetParent();
		 m_uStyle  = CButton::GetStyle(); 
		 m_bVisible = m_uStyle & WS_VISIBLE;
//决定按钮类型
		 if((m_uStyle & 0x0000000f)  == BS_CHECKBOX ||  (m_uStyle & 0x0000000f)  == BS_AUTOCHECKBOX || (m_uStyle & 0x0000000f)  == BS_3STATE ||  (m_uStyle & 0x0000000f)  == BS_AUTO3STATE)
			 m_uPrivateStyle |= TPBTNTYPE_CHECK;
		 if((m_uStyle & 0x0000000f)  == BS_RADIOBUTTON ||  (m_uStyle & 0x0000000f)  == BS_AUTORADIOBUTTON)
			 m_uPrivateStyle |= TPBTNTYPE_RADIO;
		  if((m_uStyle & 0x0000000f)  == BS_GROUPBOX)
				 m_uPrivateStyle |= TPBTNTYPE_GROUPBOX;
		 if((m_uStyle & 0x0000000f)  == BS_DEFPUSHBUTTON)
			 m_uPrivateStyle |= TPBTNTYPE_DEFAULT | TPBTNTYPE_BUTTON;
		 if((m_uStyle & 0x0000000f)  == BS_PUSHBUTTON)
			 m_uPrivateStyle |=  TPBTNTYPE_BUTTON;		 
		 EnableToolTips(TRUE);

		 GetWindowText(m_sText);

		 if(m_uPrivateStyle & TPBTNTYPE_GROUPBOX) 
		 {
			 LoadRes();
			 CRect rtClient;
			 GetClientRect(&rtClient);
			 OnSize(0,rtClient.Width(),rtClient.Height());
		 }
		 else if((m_uPrivateStyle & TPBSKIN_CUSTOM) != TPBSKIN_CUSTOM) //取得资源
			 SetSkinStyle(TPBSKIN_SQUARE1); 

		 if(m_dwWindowLevel == TPWNDLEVEL1)  	   	m_eBackColor    = COLOR_BASE_FRAME;
		 else if(m_dwWindowLevel == TPWNDLEVEL2)	m_eBackColor    = COLOR_BASE_FRAME2;

#ifdef TP_NS_NET		//2010-11-16 LiuGang subclass SetClassLongPtr影响到第三方button。这里只为了减少影响，只针对已知问题做了屏蔽
		 CString appname = ::AfxGetAppName();
		 if ((appname.Find(_T("MPC")) >= 0) || (appname.Find(_T("Test")) >= 0))
		 { return; }
#endif
		 SetClassLongPtr(m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;
		RestoreFont(m_hWnd);
	}
}

int CTPButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);	
	m_sText = lpCreateStruct ->lpszName;
	return 0;
}

BOOL CTPButton::PreCreateWindow(CREATESTRUCT& cs)
{	
	m_bCreate = TRUE;
	return CButton::PreCreateWindow(cs);
}


void CTPButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	if(!m_bCreate)	SubWindow(TRUE);
}
LRESULT CTPButton::OnPrint(WPARAM wp,LPARAM lp)
{
	return S_OK;
}
void CTPButton::GetImageIndex(CDC *pDC)
{
	if(!IsWindowEnabled())
	{
		if(m_uState & BST_CHECKED)            m_iImageIndex = CHECK_DIS;
		else if(m_uState & BST_INDETERMINATE) m_iImageIndex = STATE_DIS;		
		else                                  m_iImageIndex = BUTTON_DIS;
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS))	;
	}	
	else if(m_uState & BST_FOCUS)  
	{
		if(m_uState & BST_PUSHED) 
		{			
			if(m_uState & BST_CHECKED)            m_iImageIndex = CHECK_DOWN;
			else if(m_uState & BST_INDETERMINATE) m_iImageIndex = STATE_DOWN;
			else                                  m_iImageIndex = BUTTON_DOWN;
		}
		else
		{
			if(m_uState & BST_CHECKED)            m_iImageIndex = CHECK_FOCUS;
			else if(m_uState & BST_INDETERMINATE) m_iImageIndex = STATE_FOCUS;
			else                                  m_iImageIndex = BUTTON_FOCUS;
		}		
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));	
	}
	else if(m_bOver)  
	{			
		if(m_uState & BST_CHECKED)            m_iImageIndex = CHECK_OVER;
		else if(m_uState & BST_INDETERMINATE) m_iImageIndex = STATE_OVER;
		else                                  m_iImageIndex = BUTTON_OVER;
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));
	}
	else
	{		
		if(m_uState & BST_INDETERMINATE)      m_iImageIndex = STATE_NOR;
		else if(m_uState & BST_CHECKED)       m_iImageIndex = CHECK_NOR;
		else                                  m_iImageIndex = BUTTON_NOR;
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT))	;
	}
}//TP_GetSysColor(m_eBackColor)+
void CTPButton::OnPaint()
{
	CPaintDC dc(this); 		

	CRect     rtClient;
	GetClientRect(&rtClient);	
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());		
	if(m_cBackIndex !=-1) cdcMemo.FillSolidRect(rtClient,m_cBackIndex);	
	else                  cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_eBackColor));	
	CFont *pFont        = cdcMemo.SelectObject(TP_GetSysFont(m_iFontIndex));
	GetImageIndex(&cdcMemo);
	if(m_eTextColor!= COLOR_MAX )  cdcMemo.SetTextColor(TP_GetSysColor(m_eTextColor));		
	else   if(m_cTextIndex!=-1)    cdcMemo.SetTextColor(m_cTextIndex);
	if((m_uPrivateStyle & TPBTNTYPE_CHECK) && !(BS_PUSHLIKE & m_uStyle))       PaintCheck(&cdcMemo);
	else if((m_uPrivateStyle & TPBTNTYPE_RADIO) && !(BS_PUSHLIKE & m_uStyle))  PaintRadio(&cdcMemo);
	else  if((m_uPrivateStyle & TPBTNTYPE_BUTTON) || (BS_PUSHLIKE & m_uStyle)) PaintButton(&cdcMemo);
	else  if(m_uPrivateStyle & TPBTNTYPE_GROUPBOX)                         	   PaintGroup(&cdcMemo);
	
	if(m_uPrivateStyle & TPBTNTYPE_GROUPBOX) 
	{
		CRect rtBox = rtClient; 
		if(m_sText.GetLength() >0)	rtBox.DeflateRect(6,16,6,6); 	
		else             			rtBox.DeflateRect(6,6,6,6); 
		dc.ExcludeClipRect(rtBox);
	}
	
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
	cdcMemo.SelectObject(pFont);
	cdcMemo.DeleteDC();
}
void CTPButton::OnNcPaint()
{
}
BOOL CTPButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}
LRESULT CTPButton::OnSetState(WPARAM wp,LPARAM lp)
{
	BOOL bCurState = BOOL(SendMessage(BM_GETSTATE,0,0) & BST_PUSHED);

	if ((wp && bCurState) || (!wp && !bCurState))	return S_OK;

	if(m_bVisible)	SetRedraw(FALSE);
	LRESULT lResult = Default();
	if(m_bVisible)	SetRedraw(TRUE);
	m_uState = CButton::GetState();

	Invalidate(FALSE);	
	return lResult;
}
LRESULT CTPButton::OnSetCheck(WPARAM wp,LPARAM lp)
{	
	BOOL bVisible = m_bVisible;
	if(bVisible) CButton::SetRedraw(FALSE);
	LRESULT lResult = CButton::Default();
	if(bVisible) CButton::SetRedraw(TRUE);
	m_uState = CButton::GetState();
	if(bVisible) Invalidate(FALSE);
	else         ShowWindow(SW_HIDE);
	m_bVisible = bVisible;
	return lResult;
}
void CTPButton::OnLButtonDown(UINT nFlags, CPoint point)
{	
	if(m_bVisible)CButton::SetRedraw(FALSE);
	m_bLButtonDown = TRUE;
	m_hPressBtn    = m_hWnd;
	CButton::OnLButtonDown(nFlags, point);
	m_uState = CButton::GetState();
	if(m_bVisible)CButton::SetRedraw(TRUE);
	Invalidate(FALSE);	
	if(m_uInterval >0)
	{
		m_iDelayCount = 0; 
		if(m_hTimerAcc ==0)			        m_hTimerAcc = SetTimer(1001,m_uInterval,NULL);
		if(m_pButtonAccletCallback)			m_pButtonAccletCallback(m_hWnd,m_pCallBackParam);
		else if(m_pMsgWnd)				    m_pMsgWnd ->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),BN_CLICKED),(LPARAM)m_hWnd);									
	}
}

void CTPButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_iDelayCount = 0;
	if(m_hTimerAcc)
	{
		KillTimer(m_hTimerAcc);
		m_hTimerAcc = 0;
	}	
	if(m_bVisible)CButton::SetRedraw(FALSE);
	m_bLButtonDown = FALSE;	
	m_iAccBtnID    = 0;
	HWND hWnd      = m_hWnd;
	UINT uState    = m_uState;
	if(m_uInterval >0 && m_pButtonAccletCallback == NULL) m_iAccBtnID = GetDlgCtrlID();
 	CButton::OnLButtonUp(nFlags, point);	
	if(::IsWindow(hWnd))//有可能在UP删除窗口了
	{	
		if(m_bVisible)CButton::SetRedraw(TRUE);
		m_uState = CButton::GetState();
		if(m_uState!=uState) Invalidate(FALSE);
		m_iAccBtnID    = 0; 
	}
}

void CTPButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_bVisible)CButton::SetRedraw(FALSE);
	CButton::OnLButtonDblClk(nFlags, point);
	if(m_bVisible)CButton::SetRedraw(TRUE);
	m_uState = CButton::GetState();
	m_hPressBtn = m_hWnd;
	Invalidate(FALSE);
	if(m_uInterval >0)
	{	
		m_bLButtonDown = TRUE;
		if(m_pButtonAccletCallback)			m_pButtonAccletCallback(m_hWnd,m_pCallBackParam);
		else if(m_pMsgWnd)				    m_pMsgWnd ->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),BN_CLICKED),(LPARAM)m_hWnd);						
		m_bLButtonDown = FALSE;
	}
}

void CTPButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CButton::OnMouseMove(nFlags, point);
	UINT uState = CButton::GetState();
	if(uState != m_uState)
	{
		m_uState = uState;
		Invalidate(FALSE);
	}	
	if(!m_bOver)
	{
		CRect rtClient;
		GetClientRect(&rtClient);
		if(rtClient.PtInRect(point))
		{
			m_bOver = TRUE;
			m_hTimerOver = SetTimer(1000,100,NULL);
			Invalidate(FALSE);
		}
	}
}

void CTPButton::OnSetFocus(CWnd* pOldWnd)
{	
	if(m_bVisible) CButton::SetRedraw(FALSE);
	CButton::OnSetFocus(pOldWnd);
	if(m_bVisible) CButton::SetRedraw(TRUE);
	m_uState = CButton::GetState();
	Invalidate(FALSE);
}

void CTPButton::OnKillFocus(CWnd* pNewWnd)
{
	if(m_bVisible) CButton::SetRedraw(FALSE);
	CButton::OnKillFocus(pNewWnd);
	if(m_bVisible) CButton::SetRedraw(TRUE);
	m_uState = CButton::GetState();
	Invalidate(FALSE);
}

void CTPButton::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1000)
	{
		CRect rtClient;
		GetClientRect(&rtClient);
		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		HWND hWnd = ::WindowFromPoint(ptCursor);
		ScreenToClient(&ptCursor);
		if(!rtClient.PtInRect(ptCursor) || hWnd != m_hWnd)
		{
			KillTimer(m_hTimerOver);
			m_hTimerOver = 0;
			m_bOver = FALSE;
			Invalidate(FALSE);
		}
	}
	else if(nIDEvent == 1001)
	{
		if(GetKeyState(VK_LBUTTON)>=0)
		{
			KillTimer(m_hTimerAcc);
			m_hTimerAcc = 0;			
			Invalidate(FALSE);
		}
		else
		{
			if(m_iDelayCount>=m_uDelay / m_uInterval)
			{
				CRect rtClient;
				GetClientRect(&rtClient);
				CPoint ptCursor;
				GetCursorPos(&ptCursor); 
				HWND hWnd = ::WindowFromPoint(ptCursor);
				ScreenToClient(&ptCursor);
				if(rtClient.PtInRect(ptCursor) && hWnd == m_hWnd) 
				{	
					if(m_pButtonAccletCallback)			m_pButtonAccletCallback(m_hWnd,m_pCallBackParam);
					else if(m_pMsgWnd)					m_pMsgWnd ->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),BN_CLICKED),(LPARAM)m_hWnd);												
				}
			}
			else
			{
				m_iDelayCount ++;
			}
		}
	}

	CButton::OnTimer(nIDEvent);
}

void CTPButton::OnDestroy()
{
	if(m_hWnd == m_hPressBtn) m_hPressBtn = NULL;
	if(m_hTimerOver)
	{
		KillTimer(m_hTimerOver);
		m_hTimerOver = 0;
	}
	if(m_hTimerAcc)
	{
		KillTimer(m_hTimerAcc);
		m_hTimerAcc = 0;
	}

	SubWindow(FALSE);
	m_uPrivateStyle |= TPBSKIN_DESTROY;
	CButton::OnDestroy();	
}
void   CTPButton::SetBackColor(COLORREF eBackIndex)
{
	m_cBackIndex = eBackIndex;
}
void   CTPButton::SetBackColor(TP_SYSTEM_COLOR eBackIndex)
{
	m_eBackColor = eBackIndex;	
}
void   CTPButton::SetAccelerateTime(UINT uDelay,UINT uInterval)
{
	m_uDelay      = uDelay;
	m_uInterval   = uInterval; 
}
int   CTPButton::OnToolHitTest(CPoint point,TOOLINFO* pTI ) const
{	
	if(m_sToolTips[0].GetLength()>0)
	{		
		pTI ->hwnd     = m_hWnd;	
		pTI ->lpszText = LPSTR_TEXTCALLBACK;
		pTI ->uId      = (UINT)(UINT_PTR)m_hWnd;
		pTI ->uFlags  |= TTF_IDISHWND;

		return  GetDlgCtrlID();			
	}
	else return -1;
}
BOOL CTPButton::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{	
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	CString sText;
	if(m_vHotKey>0)        sText = _T("   ") + TP_GetVKName(m_vHotKey,m_vKeyFlag);	
	else if(m_pHotKeyData) sText = _T("   ") + TP_GetVKName(m_pHotKeyData ->vkCharSet,m_pHotKeyData->uFlagSet);
	if(m_pButtonToolTipCallBack)   sText += _T("   ") + m_pButtonToolTipCallBack(m_pToolTipParam);	          
	if(m_uState & BST_CHECKED)  	lstrcpyn( pTTT->szText ,m_sToolTips[1] +sText,79);
	else                        	lstrcpyn( pTTT->szText ,m_sToolTips[0] +sText,79);
	return  TRUE;		

}
void CTPButton::OnEnable(BOOL bEnable)
{
	//CButton::OnEnable(bEnable);
	if(!bEnable)
	{
		if(m_hTimerAcc) KillTimer(m_hTimerAcc);
		m_hTimerAcc = NULL;
		m_bLButtonDown = FALSE;
	}
	Invalidate(FALSE);
}
void CTPButton::SetSkinStyle(CWnd *pParent,UINT uID,UINT uStyle)
{
	CTPButton *pButton = (CTPButton *)pParent ->GetDlgItem(uID);
	if(pButton) pButton ->SetSkinStyle(uStyle);
}
void CTPButton::SetAccelerateCallBack(void (*f)(HWND hWnd,LPVOID pVoid),LPVOID pVoid)
{
	m_pButtonAccletCallback = f;
	m_pCallBackParam        = pVoid;
}

void CTPButton::SetSkinStyle(UINT uStyle)
{
	UINT uStyleSave = m_uPrivateStyle;
	m_uPrivateStyle &= 0xffff0000;
	m_uPrivateStyle |= uStyle;
	if(m_uPrivateStyle & TPBSKIN_CUSTOM)     return;
	else if(m_uPrivateStyle & TPBTNTYPE_GROUPBOX) return;
	else
	{	
		if((uStyleSave & 0x000000ff) != (m_uPrivateStyle &0x000000ff))
			LoadRes();
	}
}
void TP_DrawRect(CDC *pDC,CRect &rtClient,CPen *pPen)
{
	CPen *pPenSave = pDC ->SelectObject(pPen);
	pDC ->MoveTo(rtClient.left,rtClient.top);
	pDC ->LineTo(rtClient.left,rtClient.bottom);
	pDC ->LineTo(rtClient.right,rtClient.bottom);
	pDC ->LineTo(rtClient.right,rtClient.top);
	pDC ->LineTo(rtClient.left,rtClient.top);
	pDC ->SelectObject(pPenSave);
}
void CTPButton::PaintRadio(CDC *pDC)
{
	PaintCheck(pDC);
}
void CTPButton::PaintCheck(CDC *pDC)
{

	UINT    uFormat = DT_SINGLELINE;
	CRect rtClient,rtCheck,rtText = CRect(0,0,0,0);
	GetClientRect(&rtClient);	//rtClient.DeflateRect(2,2);
	pDC ->DrawText(m_sText,rtText,DT_CALCRECT);

	int ImageIndex = m_iImageIndex;
	if(m_pbForceEnable && *m_pbForceEnable == FALSE)  ImageIndex = BUTTON_NOR;

	if(!(m_pBtnRes[ImageIndex]->uState & TP_RESEMBLERES)) ResembleRes(m_pBtnRes,ImageIndex);

	if(m_uStyle & BS_LEFTTEXT)
	{
		rtCheck = rtClient;
		rtCheck.left   = rtCheck.right - m_pBtnRes[ImageIndex]->Width();
		rtClient.right = rtCheck.left - 6;
	}
	else 
	{
		rtCheck       = rtClient; 
		rtClient.left = rtClient.left + m_pBtnRes[ImageIndex]->Width() + 6;
	}
	if(m_uStyle & BS_TOP)
	{
		if(rtText.Height()>m_pBtnRes[ImageIndex]->Height())
		{
			rtCheck.top = rtText.top + (rtText.Height() - m_pBtnRes[ImageIndex]->Height())/2;
		}
		else rtCheck.top = 0;
		uFormat |=  DT_TOP;		
	}
	else if(m_uStyle & BS_BOTTOM)
	{
		if(rtText.Height()>m_pBtnRes[ImageIndex]->Height())
		{
			rtCheck.top = rtClient.bottom - (rtText.Height() + m_pBtnRes[ImageIndex]->Height())/2 ;
		}
		else rtCheck.top = rtClient.bottom - m_pBtnRes[ImageIndex]->Height();
		uFormat |=  DT_BOTTOM;		
	}
	else
	{
		rtCheck.top = rtClient.top +(rtClient.Height() - m_pBtnRes[ImageIndex]->Height())/2 ;
		uFormat |=  DT_VCENTER|DT_SINGLELINE;
	}
	if((m_uStyle & BS_CENTER) == BS_CENTER)
	{
		uFormat |= DT_CENTER;
	}
	else if(m_uStyle & BS_RIGHT)
	{
		uFormat |= DT_RIGHT;
	}
	else
	{
		uFormat |= DT_LEFT;
	}
	rtCheck.right  = rtCheck.left + m_pBtnRes[ImageIndex]->Width() ;
	rtCheck.bottom = rtCheck .top + m_pBtnRes[ImageIndex]->Height();
	if ( m_uStyle & BS_MULTILINE ) 
	{
		if(rtText.Width() >=rtClient.right -rtCheck.right)
		{		
			uFormat &= ~DT_SINGLELINE;
			uFormat |= DT_WORDBREAK;
		} 
		else m_uStyle &= ~BS_MULTILINE;
	}
	TP_StretchDIBits(pDC,m_pBtnRes[ImageIndex] ->GetByte(),m_pBtnRes[ImageIndex]->Width(),m_pBtnRes[ImageIndex]->Height(),rtCheck,FALSE);
	pDC ->DrawText(m_sText,rtClient,uFormat);
	if(GetFocus() == this && m_sText.GetLength()>0) 
	{
		if(m_uStyle & BS_TOP)         rtClient.bottom = rtClient.top + rtText.Height();		
		else if(m_uStyle & BS_BOTTOM) rtClient.top = rtClient.bottom - rtText.Height();		
		else
		{
			rtClient.top = rtClient.top + (rtClient.Height() -  rtText.Height())/2;		
			rtClient.bottom = rtClient.top + rtText.Height();					
		}
		if((m_uStyle & BS_CENTER) == BS_CENTER)
		{
			rtClient.left  = rtClient.left + (rtClient.Width() -  rtText.Width())/2;		
			rtClient.right = rtClient.left + rtText.Width();				
		}
		else if(m_uStyle & BS_RIGHT)	rtClient.left  = rtClient.right - rtText.Width(); 	
		else                            rtClient.right = rtClient.left  + rtText.Width(); 	
		rtClient.InflateRect(2,2);
		GetClientRect(&rtText);rtText.DeflateRect(1,1); rtClient &= rtText;
		TP_DrawRect(pDC,rtClient,&m_hFocusPen);//DQAdb00006919
	}
}
void  CTPButton::PaintGroup(CDC *pDC)
{
	CRect rect;
	CRect rcBox;
	GetClientRect(&rect);
	rcBox = rect;

	CSize textSize = pDC->GetTextExtent(m_sText);
	textSize.cx +=6;

	rcBox.top += textSize.cy/2;
	CRect rcText  = rect;
	DWORD dwStyle = GetStyle();
	if((dwStyle & BS_CENTER) == BS_RIGHT)
	{
		rcText.right -= 15;
		rcText.left = rcText.right - textSize.cx;		
		rcText.bottom = rcText.top + textSize.cy;
	}
	else if((!(dwStyle & BS_CENTER)) || ((dwStyle & BS_CENTER) == BS_LEFT))
	{
		rcText.left += 15;
		rcText.right = rcText.left + textSize.cx;		
		rcText.bottom = rcText.top + textSize.cy;
	}
	else if((dwStyle & BS_CENTER) == BS_CENTER)
	{
		rcText.left = (rect.left + rect.right )/2 - textSize.cx/2;
		rcText.right = rcText.left + textSize.cx;
		rcText.bottom = rcText.top + textSize.cy;		
	}	
	rcBox.DeflateRect(3,0,3,3);
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(TP_BORDER_GROUPBOX,pDC,rcBox,2);	
	pDC->FillSolidRect(rcText,TP_GetSysColor(m_eBackColor));	
	pDC->DrawText(m_sText,&rcText,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	
}

void CTPButton::PaintButton(CDC *pDC)
{
	if(m_uPrivateStyle & TPBSKIN_CUSTOM)   //自己数据
	{
		CRect rtClient,rtBmp;		
		GetClientRect(&rtClient);
		if(m_pBtnRes[m_iImageIndex] == NULL || !(m_pBtnRes[m_iImageIndex]->uState & TP_RESEMBLERES)) ResembleRes(m_pBtnRes,m_iImageIndex);
		if(m_pBtnRes[m_iImageIndex] == NULL) return;
		if(m_uPrivateStyle & TPBSKIN_STRETCH)
		{
			rtBmp = rtClient;
			TP_StretchDIBits(pDC,m_pBtnRes[m_iImageIndex] ->GetByte(),m_pBtnRes[m_iImageIndex]->Width(),m_pBtnRes[m_iImageIndex]->Height(),rtBmp,FALSE);
			if(m_sText.GetLength()>0) pDC->DrawText(m_sText,rtBmp,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else if(m_uControlState & TP_CONTROL_DOUBLE)
		{
			TP_StretchDIBRect(pDC,m_pBtnResSave[BUTTON_TYPE_SQUARE2][0] ->GetByte(),m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Width(),m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Height(),10,10,rtClient,FALSE);
			CRect rtDest = rtClient;rtDest.DeflateRect((rtClient.Width() - (m_pBtnRes[m_iImageIndex]->Width() - m_ptStretchAlign.y -m_ptStretchAlign.x))/2,0);	
			CTPMemoDC memDC1(pDC,m_pBtnRes[m_iImageIndex]->Width(),m_pBtnRes[m_iImageIndex]->Height());
			CTPMemoDC memDC2(pDC,m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Width(),m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Height());
			TP_SetDIBitsToDevice(&memDC1,m_pBtnRes[m_iImageIndex] ->GetByte(),0,0,m_pBtnRes[m_iImageIndex]->Width(),m_pBtnRes[m_iImageIndex]->Height(),CRect(0,0,m_pBtnRes[m_iImageIndex]->Width(),m_pBtnRes[m_iImageIndex]->Height()));
			TP_SetDIBitsToDevice(&memDC2,m_pBtnResSave[BUTTON_TYPE_SQUARE2][0] ->GetByte(),0,0,m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Width(),m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Height(),CRect(0,0,m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Width(),m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Height()));
			pDC->BitBlt((rtClient.Width() - m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Width())/2,0,m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Width(),m_pBtnResSave[BUTTON_TYPE_SQUARE2][0]->Height(),&memDC2,0,0,SRCINVERT);
			pDC->BitBlt((rtClient.Width() - m_pBtnRes[m_iImageIndex]->Width())/2,0,m_pBtnRes[m_iImageIndex]->Width(),m_pBtnRes[m_iImageIndex]->Height(),&memDC1,0,0,SRCINVERT);				
			
			/*rtDest.right = rtDest.left+m_pBtnRes[m_iImageIndex]->Width() - m_ptStretchAlign.y -m_ptStretchAlign.x;
			TP_SetDIBitsToDevice(pDC,m_pBtnRes[m_iImageIndex] ->GetByte(),m_ptStretchAlign.x,0,m_pBtnRes[m_iImageIndex]->Width(),m_pBtnRes[m_iImageIndex]->Height(),rtDest);*/
			if(m_sText.GetLength()>0) pDC->DrawText(m_sText,rtBmp,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
		{
			rtBmp.left = rtClient.left + (rtClient.Width()  - m_pBtnRes[m_iImageIndex]->Width())/2;
			rtBmp.top  = rtClient.top  + (rtClient.Height() - m_pBtnRes[m_iImageIndex]->Height())/2;
			rtBmp.right  = rtBmp.left + m_pBtnRes[m_iImageIndex]->Width();
			rtBmp.bottom = rtBmp.top  + m_pBtnRes[m_iImageIndex]->Height();
			if(m_clMask == 0)	TP_SetDIBitsToDevice(pDC,m_pBtnRes[m_iImageIndex] ->GetByte(),m_pBtnRes[m_iImageIndex]->Width(),m_pBtnRes[m_iImageIndex]->Height(),rtBmp,FALSE);
			else				TP_SetDIBitsToDeviceTrans(pDC,m_pBtnRes[m_iImageIndex] ->GetByte(),m_pBtnRes[m_iImageIndex]->Width(),m_pBtnRes[m_iImageIndex]->Height(),rtBmp,m_clMask);
			if(m_sText.GetLength()>0) pDC->DrawText(m_sText,rtBmp,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}

	}
	else  //公共数据
	{
		CRect rtClient,rtBmp;		
		GetClientRect(&rtClient);		
		if(m_uStyle & BS_FLAT)
		{
			if(m_uState & (BST_CHECKED | BST_PUSHED))         
				pDC ->Draw3dRect(rtClient,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DLEFT));
			else if((m_uState & BST_FOCUS) || m_bOver)
				pDC ->Draw3dRect(rtClient,TP_GetSysColor(COLOR_BASE_3DLEFT),TP_GetSysColor(COLOR_BASE_3DRIGHT));						
			rtBmp = rtClient;
		}
		else
		{
			if(m_pBtnRes[m_iImageIndex] == NULL || !(m_pBtnRes[m_iImageIndex]->uState & TP_RESEMBLERES)) ResembleRes(m_pBtnRes,m_iImageIndex);
			if(m_pBtnRes[m_iImageIndex] == NULL) return;
			if(m_uPrivateStyle & TPBSKIN_STRETCH)
			{
				rtBmp = rtClient;
			}
			else
			{
				rtBmp = rtClient;
				if(m_uStyle & BS_TOP)					rtBmp       = rtClient;
				else if(m_uStyle & BS_BOTTOM)			rtBmp.top   = rtClient.bottom - m_pBtnRes[m_iImageIndex]->Height();
				else                					rtBmp.top   = rtClient.top  + (rtClient.Height() - m_pBtnRes[m_iImageIndex]->Height())/2;
				rtBmp.bottom = rtBmp.top  + m_pBtnRes[m_iImageIndex]->Height();
			}
			TP_StretchDIBRect(pDC,m_pBtnRes[m_iImageIndex] ->GetByte(),m_pBtnRes[m_iImageIndex]->Width(),m_pBtnRes[m_iImageIndex]->Height(),15,15,rtBmp,FALSE);
			if((m_uStyle & BS_ICON) == BS_ICON)
			{
				CRect rtIcon = rtBmp;
				HICON hIcon = CButton::GetIcon();
				if(hIcon)
				{				
					if(m_uStyle & BS_TOP)					rtIcon = rtBmp;
					else if(m_uStyle & BS_BOTTOM)			rtIcon.top  = rtBmp.bottom - 32;
					else                					rtIcon.top  = rtBmp.top + (rtBmp.Height() - 32)/2;
					if(m_uStyle & BS_LEFT)					rtIcon.left = rtBmp.left;
					else if(m_uStyle & BS_RIGHT)			rtIcon.left = rtBmp.right - 32;
					else              					    rtIcon.left = rtBmp.left + ( rtBmp.Width()  - 32)/32;	
					pDC ->DrawIcon(CPoint(rtIcon.left,rtIcon.top),hIcon);
				}
			}
			else if((m_uStyle & BS_BITMAP) == BS_BITMAP)
			{
				CRect rtIcon = rtBmp;
				CBitmap *pBmp = CBitmap::FromHandle(CButton::GetBitmap());
				if(pBmp)
				{	
					BITMAP bmpInfo;
					pBmp ->GetBitmap(&bmpInfo);
					if(m_uStyle & BS_TOP)					rtIcon = rtBmp;
					else if(m_uStyle & BS_BOTTOM)			rtIcon.top  = rtBmp.bottom - bmpInfo.bmHeight;
					else                					rtIcon.top  = rtBmp.top + (rtBmp.Height() - bmpInfo.bmHeight)/2;
					if(m_uStyle & BS_LEFT)					rtIcon.left = rtBmp.left;
					else if(m_uStyle & BS_RIGHT)			rtIcon.left = rtBmp.right - bmpInfo.bmWidth;
					else              					    rtIcon.left = rtBmp.left + ( rtBmp.Width()  - bmpInfo.bmWidth)/32;	
					CDC cdcMemo;CBitmap *pSaveBmp = NULL;
					cdcMemo.CreateCompatibleDC(pDC);
					pSaveBmp = cdcMemo.SelectObject(pBmp);
					pDC ->BitBlt(rtIcon.left,rtIcon.top,bmpInfo.bmWidth,bmpInfo.bmHeight,&cdcMemo,0,0,SRCCOPY);
					cdcMemo.SelectObject(pSaveBmp);
					cdcMemo.DeleteDC();
				}
			}
			else if((m_uStyle & BS_TEXT) == BS_TEXT)
			{

				UINT    uFormat = 0;
				if((m_uStyle & BS_VCENTER) == BS_VCENTER) uFormat |= DT_VCENTER|DT_SINGLELINE;
				else if(m_uStyle & BS_TOP)				  uFormat |= DT_TOP;					
				else if(m_uStyle & BS_BOTTOM)			  uFormat |= DT_BOTTOM;	
				else                                      uFormat |= DT_VCENTER|DT_SINGLELINE;
				
				if((m_uStyle & BS_CENTER) == BS_CENTER) uFormat |= DT_CENTER;
				else if(m_uStyle & BS_LEFT)				uFormat |= DT_LEFT;
				else if(m_uStyle & BS_RIGHT)			uFormat |= DT_RIGHT;
				else                                    uFormat |= DT_CENTER;
				if(m_uStyle & BS_MULTILINE)
				{				
				}
				else
				{
					uFormat |= DT_SINGLELINE;
					pDC ->DrawText(m_sText,rtClient,uFormat);
				}
			}
			if(( m_uPrivateStyle & (TPBSKIN_SQUARE1|TPBSKIN_SQUARE1)) && GetFocus() == this) 
			{
				GetClientRect(&rtClient);	
				rtClient.DeflateRect(6,4);
				TP_DrawRect(pDC,rtClient,&m_hFocusPen);
			}
		}
	}
}
void CTPButton::SetBitmaps(LPCTSTR lpText,BOOL bStrech,BOOL bSimple)
{
	if(bStrech)	m_uPrivateStyle |= TPBSKIN_STRETCH;
	else    	m_uPrivateStyle &= ~TPBSKIN_STRETCH;
	m_uPrivateStyle |= TPBSKIN_CUSTOM;

	for(INT l=0;l<BUTTON_MAX;l++)
	{
		if(m_bDefaultData == FALSE)
			TP_ReleaseCommonRes(m_pBtnRes[l]); 	
		m_pBtnRes[l] = NULL;
	}
	CString sDoubleEx = _T("");
	if(m_uControlState & TP_CONTROL_DOUBLEBMP)
		sDoubleEx = _T("_D");

	m_bDefaultData = FALSE;


	if(bSimple == 1000)
	{	
		CString sBtnExt[BUTTON_MAX] = 
		{
			_L("_NOR"), _L("_DOWN"),_L("_OVER"),_L("_FOCUS"),_L("_DIS"), _L("_DOWN"), _L("_NOR"),_L("_DOWN"),
			_L("_DOWN"),_L("_DOWN"),_L("_NOR"), _L("_DOWN"), _L("_OVER"),_L("_FOCUS"),_L("_DIS"),
		};
		for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnRes[l] = TP_LoadBmpByte(lpText + sBtnExt[l] +  sDoubleEx +_L(".bmp"),FALSE,FALSE); 
	}
	else if(!bSimple)
	{	
		CString sBtnExt[] = 
		{
			_L("_NOR_NOR"),_L("_NOR_DOWN"),_L("_NOR_OVER"),_L("_NOR_FOCUS"),_L("_NOR_DIS"),_L("_DOWN_NOR"),_L("_DOWN_DOWN"),_L("_DOWN_OVER"),
				_L("_DOWN_FOCUS"),_L("_DOWN_DIS"),_L("_STATE_NOR"),_L("_STATE_DOWN"),_L("_STATE_OVER"),_L("_STATE_FOCUS"),_L("_STATE_DIS"),
		};
		for(INT l=0;l<BUTTON_MAX;l++)	m_pBtnRes[l] = TP_LoadBmpByte(lpText + sBtnExt[l] + sDoubleEx +_L(".bmp"),FALSE,FALSE); 
	}
	else
	{	
		CString sBtnExt[] = 
		{
			_L("_NOR"),_L("_DOWN"),_L("_OVER"),_L("_FOCUS"),_L("_DIS"),	
		};
		for(INT l=0;l<5;l++)	m_pBtnRes[l] = TP_LoadBmpByte(lpText + sBtnExt[l] +  sDoubleEx + _L(".bmp"),FALSE,FALSE); 
	}	

	ResembleRes(m_pBtnRes);
	Invalidate();	
}
void CTPButton::SetBitmaps(LPCTSTR lpNor,LPCTSTR lpDown,LPCTSTR lpOver,LPCTSTR lpDis,BOOL bStrech)
{
	if(bStrech)	m_uPrivateStyle |= TPBSKIN_STRETCH;
	else    	m_uPrivateStyle &= ~TPBSKIN_STRETCH;

	for(INT l=0;l<BUTTON_MAX;l++)
	{
		if(m_bDefaultData == FALSE)
			TP_ReleaseCommonRes(m_pBtnRes[l]); 	
		m_pBtnRes[l] = NULL;
	}

	m_bDefaultData = FALSE;

	m_pBtnRes[BUTTON_NOR] = TP_LoadBmpByte(lpNor,FALSE,FALSE); 			
	m_pBtnRes[BUTTON_DOWN] = TP_LoadBmpByte(lpDown,FALSE,FALSE); 			
	m_pBtnRes[BUTTON_OVER] = TP_LoadBmpByte(lpOver,FALSE,FALSE); 			
	m_pBtnRes[BUTTON_DIS] = TP_LoadBmpByte(lpDis,FALSE,FALSE); 	

	m_uPrivateStyle |= TPBSKIN_CUSTOM;
	ResembleRes(m_pBtnRes);
	Invalidate();	
}
void CTPButton::SetBitmaps(CBitmap* pNor, CBitmap* pDown,CBitmap* pOver,CBitmap* pDis,BOOL bStrech)
{
	if(bStrech)	m_uPrivateStyle |= TPBSKIN_STRETCH;
	else    	m_uPrivateStyle &= ~TPBSKIN_STRETCH;

	for(INT l=0;l<BUTTON_MAX;l++)
	{
		if(m_bDefaultData == FALSE)
			TP_ReleaseCommonRes(m_pBtnRes[l]); 	
		m_pBtnRes[l] = NULL;
	}

	m_bDefaultData = FALSE;

	m_pBtnRes[BUTTON_NOR]  = TP_LoadBmpByte(pNor); 			
	m_pBtnRes[BUTTON_DOWN] = TP_LoadBmpByte(pDown); 			
	m_pBtnRes[BUTTON_OVER] = TP_LoadBmpByte(pOver); 			
	m_pBtnRes[BUTTON_DIS]  = TP_LoadBmpByte(pDis); 			

	m_uPrivateStyle |= TPBSKIN_CUSTOM;
	ResembleRes(m_pBtnRes);
	Invalidate();
}
void CTPButton::SetBitmaps(LPCTSTR lpNor,LPCTSTR lpNorDown,LPCTSTR lpNorOver,LPCTSTR lpNorDis,LPCTSTR lpNorFocus,
						   LPCTSTR lpCheckNor, LPCTSTR lpCheckDown,LPCTSTR lpCheckOver, LPCTSTR lpCheckDis,LPCTSTR lpCheckFocus,   
						   LPCTSTR lpStateNor,  LPCTSTR lpStateDown,LPCTSTR lpStateOver,LPCTSTR lpStateDis,LPCTSTR lpStateFocus,BOOL bStrech)
{
	if(bStrech)	m_uPrivateStyle |= TPBSKIN_STRETCH;
	else    	m_uPrivateStyle &= ~TPBSKIN_STRETCH;

	for(INT l=0;l<BUTTON_MAX;l++)
	{
		if(m_bDefaultData == FALSE)
			TP_ReleaseCommonRes(m_pBtnRes[l]); 	
		m_pBtnRes[l] = NULL;
	}

	m_bDefaultData = FALSE;


	m_pBtnRes[BUTTON_NOR]   = TP_LoadBmpByte(lpNor,FALSE,FALSE); 			
	m_pBtnRes[BUTTON_DOWN]  = TP_LoadBmpByte(lpNorDown,FALSE,FALSE); 			
	m_pBtnRes[BUTTON_OVER]  = TP_LoadBmpByte(lpNorOver,FALSE,FALSE); 			
	m_pBtnRes[BUTTON_FOCUS] = TP_LoadBmpByte(lpNorFocus,FALSE,FALSE); 	
	m_pBtnRes[BUTTON_DIS]   = TP_LoadBmpByte(lpNorDis,FALSE,FALSE); 			

	m_pBtnRes[CHECK_NOR]    = TP_LoadBmpByte(lpCheckNor,FALSE,FALSE); 			
	m_pBtnRes[CHECK_DOWN]   = TP_LoadBmpByte(lpCheckDown,FALSE,FALSE); 			
	m_pBtnRes[CHECK_OVER]   = TP_LoadBmpByte(lpCheckOver,FALSE,FALSE); 	
	m_pBtnRes[CHECK_FOCUS]  = TP_LoadBmpByte(lpCheckFocus,FALSE,FALSE); 			
	m_pBtnRes[CHECK_DIS]    = TP_LoadBmpByte(lpCheckDis,FALSE,FALSE); 			

	m_pBtnRes[STATE_NOR]   = TP_LoadBmpByte(lpStateNor,FALSE,FALSE); 			
	m_pBtnRes[STATE_DOWN]  = TP_LoadBmpByte(lpStateDown,FALSE,FALSE); 	
	m_pBtnRes[STATE_OVER]  = TP_LoadBmpByte(lpStateOver,FALSE,FALSE); 			
	m_pBtnRes[STATE_FOCUS] = TP_LoadBmpByte(lpStateFocus,FALSE,FALSE); 			
	m_pBtnRes[STATE_DIS]   = TP_LoadBmpByte(lpStateDis,FALSE,FALSE); 			

	m_uPrivateStyle |= TPBSKIN_CUSTOM;
	ResembleRes(m_pBtnRes);
	Invalidate();

}
void CTPButton::SetBitmaps(CBitmap* pNor,CBitmap* pNorDown, CBitmap* pNorOver, CBitmap* pNorDis, CBitmap* pNorFocus,				  
						   CBitmap* pCheckNor, CBitmap* pCheckDown, CBitmap* pCheckOver, CBitmap* pCheckDis, CBitmap* pCheckFocus,   
						   CBitmap* pStateNor, CBitmap* pStateDown, CBitmap* pStateOver,    CBitmap* pStateDis, CBitmap* pStateFocus, BOOL bStrech)
{
	if(bStrech)	m_uPrivateStyle |= TPBSKIN_STRETCH;
	else    	m_uPrivateStyle &= ~TPBSKIN_STRETCH;


	for(INT l=0;l<BUTTON_MAX;l++)
	{
		if(m_bDefaultData == FALSE)
			TP_ReleaseCommonRes(m_pBtnRes[l]); 	
		m_pBtnRes[l] = NULL;
	}

	m_bDefaultData = FALSE;

	m_pBtnRes[BUTTON_NOR]   = TP_LoadBmpByte(pNor); 			
	m_pBtnRes[BUTTON_DOWN]  = TP_LoadBmpByte(pNorDown); 			
	m_pBtnRes[BUTTON_OVER]  = TP_LoadBmpByte(pNorOver); 			
	m_pBtnRes[BUTTON_FOCUS] = TP_LoadBmpByte(pNorFocus); 	
	m_pBtnRes[BUTTON_DIS]   = TP_LoadBmpByte(pNorDis); 			

	m_pBtnRes[CHECK_NOR]    = TP_LoadBmpByte(pCheckNor); 			
	m_pBtnRes[CHECK_DOWN]   = TP_LoadBmpByte(pCheckDown); 			
	m_pBtnRes[CHECK_OVER]   = TP_LoadBmpByte(pCheckOver); 	
	m_pBtnRes[CHECK_FOCUS]  = TP_LoadBmpByte(pCheckFocus); 			
	m_pBtnRes[CHECK_DIS]    = TP_LoadBmpByte(pCheckDis); 			

	m_pBtnRes[STATE_NOR]   = TP_LoadBmpByte(pStateNor); 			
	m_pBtnRes[STATE_DOWN]  = TP_LoadBmpByte(pStateDown); 	
	m_pBtnRes[STATE_OVER]  = TP_LoadBmpByte(pStateOver); 			
	m_pBtnRes[STATE_FOCUS] = TP_LoadBmpByte(pStateFocus); 			
	m_pBtnRes[STATE_DIS]   = TP_LoadBmpByte(pStateDis); 			


	m_uPrivateStyle |= TPBSKIN_CUSTOM;
	ResembleRes(m_pBtnRes);
	Invalidate();
}
void CTPButton::SetCheck(int nCheck)
{
	if((m_uState & BST_CHECKED)  && nCheck) return;
	else if(!(m_uState & BST_CHECKED)  && !nCheck) return;
	return CButton::SetCheck(nCheck);
}
void CTPButton::SetToolTipText(LPCTSTR lpText,LPCTSTR lpTextCheck )
{
	m_sToolTips[0] = lpText;
	m_sToolTips[1] = lpTextCheck;
}
void CTPButton::SetToolTipText2( LPVOID lpText,LPVOID lpTextCheck) //by zhh
{
	SetToolTipText((LPCTSTR)lpText, (LPCTSTR)lpTextCheck);
}
int          CTPButton::GetBtnResWidth()
{
	for(INT l=0;l<BUTTON_MAX;l++)
	{
		if(m_pBtnRes[l] && !TP_IsDefault(m_pBtnRes[l]))
			return m_pBtnRes[l] ->Width();
	}
	return 0;
}
int          CTPButton::GetBtnResHeight()
{
	for(INT l=0;l<BUTTON_MAX;l++)
	{
		if(m_pBtnRes[l] && !TP_IsDefault(m_pBtnRes[l]))
			return m_pBtnRes[l] ->Height();
	}
	return 0;
}
TPCommonRes *CTPButton::GetBtnRes(BUTTON_INDEX eIndex)
{
	if(m_pBtnRes[eIndex]->GetByte() == NULL) TP_LoadCommonRes(m_pBtnRes[eIndex]);
	return m_pBtnRes[eIndex];
}
void CTPButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);
	if(m_uPrivateStyle & TPBTNTYPE_GROUPBOX) 
	{
		if(m_rgnWnd.m_hObject)	m_rgnWnd.DeleteObject();
		CRgn rgn1,rgn2;		
		rgn1.CreateRectRgn(0,0,cx,cy);
		if(m_sText.GetLength() >0)	rgn2.CreateRectRgn(6,16,cx-6,cy-6);		
		else             			rgn2.CreateRectRgn(6,6,cx-6,cy-6);
		m_rgnWnd.CreateRectRgn(0,0,0,0);
		m_rgnWnd.CombineRgn(&rgn1,&rgn2,RGN_DIFF);
		SetWindowRgn((HRGN)m_rgnWnd.GetSafeHandle(),TRUE);
		rgn1.DeleteObject();
		rgn2.DeleteObject();
	}
}

void CTPButton::OnSizing(UINT fwSide, LPRECT pRect)
{
	CButton::OnSizing(fwSide, pRect);

	if((m_uPrivateStyle & TPBSKIN_AUTOSIZE) && (TPBSKIN_CUSTOM &m_uPrivateStyle) && m_pBtnRes[0])
	{
		pRect ->bottom = pRect ->top + m_pBtnRes[0]->Height();
		pRect ->right  = pRect ->top + m_pBtnRes[0]->Width();
	}
}
void CTPButton::SetMask(COLORREF clMask)
{
	m_clMask = clMask;
}
void CTPButton::SetStretch(UINT bSet,int iLeft,int iRight)
{
	if(m_pControlData && m_pControlData ->dwStyleEx & TP_CONTROL_DOUBLEBMP)
	{
		bSet &= ~TP_CONTROL_DOUBLE;
		bSet |= TP_CONTROL_DOUBLEBMP;
	}
	if(bSet & TP_CONTROL_DOUBLE)
	{			
		m_ptStretchAlign = CPoint(iLeft,iRight);
	}
	m_uControlState       = bSet;
}
void CTPButton::SetStretch(BOOL bSet)
{
	if(bSet) m_uPrivateStyle |=  TPBSKIN_STRETCH;
	else     m_uPrivateStyle &= ~TPBSKIN_STRETCH;
	Invalidate(FALSE);
}
void CTPButton::SetResSize()
{
	if(m_pBtnRes[0] == NULL) return;
	CRect rtWnd;
	GetWindowRect(&rtWnd);
	GetParent()->ScreenToClient(&rtWnd);
	MoveWindow(rtWnd.left,rtWnd.top,m_pBtnRes[0]->Width(),m_pBtnRes[0]->Height());
}
void CTPButton::SetAutoSize(BOOL bAuto)
{
	if(bAuto) m_uPrivateStyle |= TPBSKIN_AUTOSIZE;
	else      m_uPrivateStyle &= ~TPBSKIN_AUTOSIZE;

	if((m_uPrivateStyle & TPBSKIN_AUTOSIZE) && (TPBSKIN_CUSTOM &m_uPrivateStyle)  && m_pBtnRes[0])
	{
		SetWindowPos(NULL,0,0,m_pBtnRes[0]->Width(),m_pBtnRes[0]->Height(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	}
	else if((m_uPrivateStyle & TPBSKIN_AUTOSIZE) && m_pBtnRes[0]) //文字
	{
		CString strText;
		int iCx;int iCy;
		CDC *pDC = GetDC();
		CRect rtText = CRect(0,0,0,0);
		pDC ->DrawText(m_sText,rtText,DT_CALCRECT);
		ReleaseDC(pDC);

		if((m_uPrivateStyle & TPBTNTYPE_CHECK) && !(BS_PUSHLIKE & m_uStyle)) 
		{		
			iCx = rtText.Width() + 6 + m_pBtnRes[0]->Width();
			iCy = max(m_pBtnRes[0]->Height(),rtText.Height() + 6);
		}
		else if((m_uPrivateStyle & TPBTNTYPE_RADIO) && !(BS_PUSHLIKE & m_uStyle))  
		{		
			iCx = rtText.Width() + 6 + m_pBtnRes[0]->Width();
			iCy = max(m_pBtnRes[0]->Height(),rtText.Height() + 6);
		}
		else  if((m_uPrivateStyle & TPBTNTYPE_BUTTON) || (BS_PUSHLIKE & m_uStyle))
		{
			iCx = max(rtText.Width() + 6, m_pBtnRes[0]->Width());
			iCy = max(m_pBtnRes[0]->Height(),rtText.Height() + 6);
		}
		SetWindowPos(NULL,0,0,iCx,iCy,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);	

	}
}
LRESULT CTPButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_SETTEXT)
	{
		m_sText = (LPTSTR)lParam;
		return TRUE;
	}
	else if(message == WM_GETTEXT)
	{
		lstrcpyn((TCHAR *)lParam,(LPCTSTR)m_sText,(int)wParam);		
		return TRUE;
	}
	else if(message == WM_KEYDOWN && wParam == VK_SPACE && (m_uPrivateStyle & TPBTNTYPE_TOOLBAR))
	{
		return TRUE;
	}
	else if(message == WM_CONTEXTMENU)
	{
 		return S_OK;
	}
	else if(message == WM_KEYDOWN && wParam == VK_RETURN )
	{
		if((m_uPrivateStyle & TPBTNTYPE_CHECK) && !((m_uPrivateStyle & TPBTNTYPE_TOOLBAR)))
		{
			SetCheck(!GetCheck());
			if(m_pMsgWnd) m_pMsgWnd ->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),BN_CLICKED),(LPARAM)m_hWnd);												
		}
	}
	else if(message == WM_UPDATEUISTATE) return S_OK;
	return CButton::DefWindowProc(message, wParam, lParam);
}
BOOL CTPButton::IsWindowVisible()
{
	return m_bVisible;
}
BOOL CTPButton::ShowWindow(int nCmdShow)
{
	m_bVisible = (nCmdShow == SW_SHOW);
	return CButton::ShowWindow(nCmdShow);
}
void CTPButton::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CButton::OnShowWindow(bShow, nStatus);
	m_bVisible = bShow;
	if(bShow)
	{
		if(m_hTimerAcc) KillTimer(m_hTimerAcc);
		m_hTimerAcc = NULL;
		m_bLButtonDown = FALSE;
	}
	// TODO: 在此处添加消息处理程序代码
}
void CTPButton::SetHotKeyData(TPHotKeyData *pHotKeyData)
{
	m_pHotKeyData = pHotKeyData;
}
void CTPButton::SetHotKeyData(UINT vHotKey,UINT vKeyFlag)
{
	m_vHotKey        = vHotKey;
	m_vKeyFlag       = vKeyFlag;
}

void CTPButton::SetPrivateStyle(UINT uAdd,UINT uRemove)
{
	m_uPrivateStyle |= uAdd;
	m_uPrivateStyle &= ~uRemove;
}
void CTPButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_uPrivateStyle & TPBTNTYPE_RDFOCUS)
	{
		if(GetFocus() !=this )		SetFocus();
	}
	__super::OnRButtonDown(nFlags, point);
	GetParent()->SendMessage(WM_BUTTON,TP_BUTTON_RDOWN,(LPARAM)m_hWnd);
}
void CTPButton::SetTextColor(COLORREF clSet)
{
	m_cTextIndex = clSet;
}
void CTPButton::SetStateInfo(TPButtonStateInfoArray &aState)
{
	m_aButtonState.Copy(aState);
}
void CTPButton::SetStateFlag(UINT uFlag)
{
	for(INT l=0;l<m_aButtonState.GetSize();l++)
	{
		if(m_aButtonState[l].uFlag == uFlag)
		{
			SetStateIndex(l);
			return;
		}
	}
}
void CTPButton::SetStateIndex(int iIndex)
{
	if(m_iStateIndex == iIndex || iIndex<0 || iIndex>=m_aButtonState.GetSize()) return;
	SetToolTipText(m_aButtonState[iIndex].sToolTips,m_aButtonState[iIndex].sToolTips);
	SetBitmaps(m_aButtonState[iIndex].sFace,FALSE);
	m_iStateIndex = iIndex;
}
int CTPButton::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	 
	return __super::OnMouseActivate(pDesktopWnd, nHitTest, message);
}