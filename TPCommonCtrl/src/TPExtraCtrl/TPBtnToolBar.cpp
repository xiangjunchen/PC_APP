// TPUserToolBar.cpp : 实现文件
//

#include "stdafx.h"

TP_BtnToolBarLink_Data  g_aBtnToolLink[TP_BtnToolBar_Max];
TPNSControlData         *g_pGlobalHotKey = NULL;
long                     g_lGlobalHotKey = 0;
IMPLEMENT_DYNAMIC(CTPBtnToolBar, CWnd)
CTPBtnToolBar::CTPBtnToolBar()
{
	m_dwWindowLevel = TPWNDLEVEL1; 

	m_uBarStyle = 0;
	m_szButton  = CSize(0,0); 
	m_iBtnSpace = 10;

	m_pMsgWnd    = NULL;
	m_pParentWnd = NULL;
	m_uBarStyle  = 0;
	m_szButton   = CSize(0,0);	

	m_uState         = 0;
	m_pControlData   = NULL;
	m_dwControlCount = 0;
	m_uControlCol    = 1000;
	m_uControlLine   = 1;

	m_pHotKeyData       = NULL;
	m_dwHotKeyCount     = 0;
	m_pHotKeyDataEx     = NULL;
	m_dwHotKeyCountEx   = 0;
	m_pHotKeyDataExEx   = NULL;
 
	m_bSubWindow     = FALSE;
	m_bCreate        = FALSE;	

	m_pBarBack       = NULL;
	m_pLogo          = NULL;
	m_iBarBackEx[0]  = 10;
	m_iBarBackEx[1]  = 10;
	m_iButtonSize[0] = 0;
	m_iButtonSize[1] = 0;

	m_iBackColor     = COLOR_BASE_FRAME;
	m_bReLayOut      = FALSE;
	m_iTopX          = 10;
	m_dAmp           = 1.0;
	m_iExLeft        = 0;
	m_iLeftSpace     = 0;
	m_pAttachWnd     = NULL;
	m_pAttachWndEx   = NULL;
	m_iTopOffset     = 0;
	m_iLeftOffset    = 0;

	m_hContinueTimer = 0;
	m_uContinueID    = 0;
	m_uContinueKey   = 0;
	m_uContinueFlag  = 0;

	m_pControlDataSave = NULL;
	m_pHotKeyDataSave  = NULL;
	m_pActWnd          = NULL; 
	m_hPressBtn        = NULL;

	m_uPaletteID        = 0;
	m_uPaletteCol       = 0;
	m_uPaletteLine      = 0;
	m_pPaletteBar       = NULL;
	m_bPaletteShow      = FALSE;
	m_bMouseWheel       = FALSE;

	m_pWndCtrnData      = NULL;
	
	for(INT l=0;l<4;l++)m_pPaletteRes[l] = NULL;

	m_eLinkType            = TP_BtnToolBar_Unknow;
	m_iLinkIndex           = -1;
	m_bHotKey              = FALSE;

	m_pControlLable        = NULL;
	m_iControlLable        = 0;	
	m_pBtnScroll[0]        = NULL;
	m_pBtnScroll[1]        = NULL;
	m_bBtnScroll[0]        = FALSE;
	m_bBtnScroll[1]        = FALSE;
	m_hBtnScrollRgn        = NULL;

	SetCheckLinkCallBack   = NULL; 
	EnableLinkCallBack     = NULL; 
	m_pLinkCallBack        = NULL;

	PreTranslateMessageEx      = NULL;
	m_pPreTranslateMessageData = NULL;
	m_hTopWnd                  = NULL;
}

CTPBtnToolBar::~CTPBtnToolBar()
{
	if(m_pBarBack) TP_ReleaseCommonRes(m_pBarBack);
	if(m_pLogo)    TP_ReleaseCommonRes(m_pLogo);

	SubWindow(FALSE);

	for(INT l=0;l<4;l++) {TP_ReleaseCommonRes(m_pPaletteRes[l]); m_pPaletteRes[l] = NULL;}

	if(m_eLinkType != TP_BtnToolBar_Unknow)
		g_aBtnToolLink[m_eLinkType].Remove(m_iLinkIndex);
	
	if(m_pControlDataSave) delete [] m_pControlDataSave;
	m_pControlDataSave = NULL;
	if(m_pHotKeyDataSave) delete [] m_pHotKeyDataSave;
	m_pHotKeyDataSave = NULL;

	m_aBtnStateInfo.RemoveAll();
	m_aTPButton.RemoveAll();
	m_aTPButtonEx.RemoveAll();
}


BEGIN_MESSAGE_MAP(CTPBtnToolBar, CWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()

	ON_COMMAND_RANGE(0,0x0fffffff,OnButtonClicked)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_BUTTON,OnMsgButton)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_BASEFRAME,OnMsgGetBaseframe)
END_MESSAGE_MAP()

// CTPBtnToolBar 消息处理程序
void      CTPBtnToolBar::ShowPalette()
{
	if(m_pPaletteBar)
	{
		CRect rtWnd,rtPos ;
		CWnd *pButton =  NULL;
		if(CTPButton::m_hPressBtn) pButton = CWnd::FromHandle(CTPButton::m_hPressBtn);
		else  pButton = GetDlgItem(m_uPaletteID);
		if(pButton == NULL) return;
		m_pPaletteBar ->GetWindowRect(&rtWnd);
		pButton->GetWindowRect(&rtPos);
		rtWnd.OffsetRect(-rtWnd.left+rtPos.left,-rtWnd.top+rtPos.bottom);
		if(m_pPaletteBar ->m_bPaletteShow)return;
		m_pPaletteBar ->m_bPaletteShow = TRUE;	
 		if(rtWnd.Height() + rtWnd.top> ::GetSystemMetrics(SM_CYSCREEN))
		{
			rtWnd.top -= rtWnd.Height() + rtPos.Height();
		}
		CTPBaseFrame::m_uBaseFlag |= TP_BASESTATE_NOACTWND;
		m_pPaletteBar ->BringWindowToTop();		
		m_pPaletteBar ->SetWindowPos(NULL,rtWnd.left,rtWnd.top,0,0,SWP_SHOWWINDOW|SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);					
		m_pPaletteBar ->SetFocus();
		CTPBaseFrame::m_uBaseFlag &= ~TP_BASESTATE_NOACTWND;
	}
}
void      CTPBtnToolBar::LoadPalette()
{
	CString sText[] = {_T("PaletteLeft.bmp"),_T("Palettetop.bmp"),_T("Paletteright.bmp"),_T("Palettebottom.bmp"),};
	CString sPath   = TP_GetSkinPath() + _T("\\CommonCtrl\\ToolBar\\");
	for(INT l=0;l<4;l++)
	{
		TP_ReleaseCommonRes(m_pPaletteRes[l]);
		m_pPaletteRes[l]  = TP_LoadBmpByte(sPath + sText[l]);
	}
}

void      CTPBtnToolBar::OnButtonClicked(UINT nID)
{
	if(CTPButton::m_iAccBtnID == nID) return;

	m_hPressBtn = CTPButton::m_hPressBtn;
	if(nID == ID_BTNSCROLL_LEFT)
	{
		if(m_bMouseWheel)  OnMouseWheel(12);
	}
	else if(nID == ID_BTNSCROLL_RIGHT)
	{
		if(m_bMouseWheel) OnMouseWheel(-12);
	}
	else
	{
		m_bHotKey = FALSE;
		if(m_hContinueTimer) KillTimer(m_hContinueTimer);
		m_hContinueTimer = NULL;	
		if(m_uBarStyle == 3)
		{
			m_bPaletteShow = FALSE;
			ShowWindow(SW_HIDE);
			if(m_pActWnd) m_pActWnd ->SetActiveWindow();
		}	
		if(m_uPaletteID == nID) ShowPalette();
		else 			
		{
			UINT uFlag  =  0;
			if(	GetKeyState(VK_CONTROL) <0) uFlag |= TP_CONTROL;
			if(	GetKeyState(VK_SHIFT) <0)   uFlag |= TP_SHIFT;
			if(	GetKeyState(VK_MENU) <0)    uFlag |= TP_MENU;
#ifdef TP_NS_LOG		
			for(DWORD l=0;l<m_dwControlCount;l++)
			{
				if(m_pControlData[l].nID == nID)
				{
				//	TP_LogEditNotifyCtrl(0,m_pControlData[l].sNotifyDown,CTPBaseFrame::GetActiveFrameCaption(),EDIT_LOGGER_MODULE,(CString)__FUNCTION__);
					break;
				}
			}				
#endif
			m_pMsgWnd ->SendMessage(WM_TOOLBARCLICK,nID,uFlag); //XN00003431
		}
	}
}
void     CTPBtnToolBar::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_STATIC,&wndClass);		
		wndClass.lpszClassName = WC_BTNTOOLBAR;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_BTNTOOLBAR,AfxGetInstanceHandle());
}
void CTPBtnToolBar::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	
		ModifyStyle(0,WS_CLIPCHILDREN,0);
		if(m_pMsgWnd == NULL) m_pMsgWnd = GetParent();
		SetWindowText(WC_BTNTOOLBAR);
		SetClassLongPtr(m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)TP_GetCtrlCursor());
		if(m_uBaseState & TP_BASESTATE_CENTER) m_bMouseWheel = FALSE;
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
		for(INT l=0;l<m_aTPButton.GetSize();l++)
		{
			if(m_aTPButton[l])
			{
				if(m_aTPButton[l]->m_lParam) ::DeleteObject((HRGN)m_aTPButton[l]->m_lParam);
				m_aTPButton[l]->m_lParam = NULL;
				m_aTPButton[l]->DestroyWindow();
				delete m_aTPButton[l];
			}
		}
		m_aTPButton.RemoveAll();
	}
}

void CTPBtnToolBar::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

BOOL CTPBtnToolBar::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CWnd::PreCreateWindow(cs);
}
void TP_BtnAccelerateTime(HWND hWnd,LPVOID lpVoid)
{
	CTPBtnToolBar *pBtn = (CTPBtnToolBar *)lpVoid;
	pBtn->OnButtonClicked(GetDlgCtrlID(hWnd));	
}
void CTPBtnToolBar::SetScrollBtnBmp(CString sPathLeft,CString sPathRight)
{
	CString sSkin = TP_GetSkinPath();
	if(m_pBtnScroll[0] == NULL)
	{
		m_pBtnScroll[0] = new CTPButton();
		m_pBtnScroll[0] ->Create(_T(""),WS_CHILD,CRect(0,0,10,10),GetParent(),ID_BTNSCROLL_LEFT);
		m_pBtnScroll[0] ->SetAccelerateTime(100,100);
		m_pBtnScroll[0]->SetAccelerateCallBack(TP_BtnAccelerateTime,this);		
		m_pBtnScroll[0]->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE);
		m_pBtnScroll[1] = new CTPButton();
		m_pBtnScroll[1] ->Create(_T(""),WS_CHILD,CRect(0,0,10,10),GetParent(),ID_BTNSCROLL_RIGHT);
		m_pBtnScroll[1] ->SetAccelerateTime(100,100);
		m_pBtnScroll[1]->SetAccelerateCallBack(TP_BtnAccelerateTime,this);		
		m_pBtnScroll[1]->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE);		
	}
	m_pBtnScroll[0]  ->SetBitmaps(sPathLeft,FALSE);
	m_pBtnScroll[1]  ->SetBitmaps(sPathRight,FALSE);
}
int CTPBtnToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	if(m_eLinkType!=TP_BtnToolBar_Unknow)
		m_iLinkIndex = g_aBtnToolLink[m_eLinkType].GetIndex();
	return 0;
}
void CTPBtnToolBar::SetBarBack(CString sBack,int iLeftTop,int iRightBottom)
{	
	if(m_pBarBack) TP_ReleaseCommonRes(m_pBarBack);
	m_pBarBack = NULL;
	m_pBarBack       = TP_LoadBmpByte(sBack);
	m_iBarBackEx[0]  = iLeftTop;
	m_iBarBackEx[1]  = iRightBottom;
	m_uBarStyle      = 2;
}
void CTPBtnToolBar::OnDestroy()
{
	if(m_pPaletteBar)
	{
		m_pPaletteBar ->DestroyWindow();
		delete m_pPaletteBar;
		m_pPaletteBar = NULL;
	}
	for(INT l=0;l<2;l++)
	{
		if(m_pBtnScroll[l])  m_pBtnScroll[l] ->DestroyWindow();
		delete m_pBtnScroll[l];
		m_pBtnScroll[l] = NULL;		
	}
	if(m_hBtnScrollRgn) DeleteObject(m_hBtnScrollRgn);		
	m_hBtnScrollRgn = NULL;
	SubWindow(FALSE);

	if(m_eLinkType != TP_BtnToolBar_Unknow)
		g_aBtnToolLink[m_eLinkType].Remove(m_iLinkIndex);

	CWnd::OnDestroy();
}
void CTPBtnToolBar::OnPaint()
{
	CPaintDC dcSet(this); // device context for painting	
	CRect rtClient;	
	GetClientRect(&rtClient);	
	CTPMemoDC dc(&dcSet,rtClient.Width(),rtClient.Height());
	if(m_uBarStyle == 0) dc.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));		
	else if(m_uBarStyle == 1)
	{		
		dc.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));	
		rtClient.InflateRect(0,0,0,1);
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner->PaintLine(&dc,rtClient,0x00000004);
	}
	else if(m_uBarStyle == 2 && m_pBarBack)
	{
		if(m_pBarBack )
		TP_StretchDIBRect(&dc,m_pBarBack ->GetByte(),m_pBarBack ->Width(),m_pBarBack ->Height(),m_iBarBackEx[0],m_iBarBackEx[1],rtClient); 			
	}	
	else if(m_uBarStyle == 3)
	{
		dc.FillSolidRect(rtClient,RGB(87,87,87));	
		CRect rtItem = rtClient;rtItem.bottom = rtItem.top + m_pPaletteRes[1] ->Height();
		TP_StretchDIBRect(&dc,m_pPaletteRes[1] ->GetByte(),m_pPaletteRes[1] ->Width(),m_pPaletteRes[1] ->Height(),13,10,rtItem); 			
		rtItem = rtClient;rtItem.top = rtItem.bottom - m_pPaletteRes[3] ->Height();
		TP_StretchDIBRect(&dc,m_pPaletteRes[3] ->GetByte(),m_pPaletteRes[3] ->Width(),m_pPaletteRes[3] ->Height(),13,10,rtItem); 			
		rtItem = rtClient;rtItem.right = rtItem.left + m_pPaletteRes[0] ->Width();
		TP_StretchDIBRect(&dc,m_pPaletteRes[0] ->GetByte(),m_pPaletteRes[0] ->Width(),m_pPaletteRes[0] ->Height(),10,14,rtItem,TRUE); 			
		rtItem = rtClient;rtItem.left = rtItem.right - m_pPaletteRes[2] ->Width();
		TP_StretchDIBRect(&dc,m_pPaletteRes[2] ->GetByte(),m_pPaletteRes[2] ->Width(),m_pPaletteRes[2] ->Height(),10,10,rtItem,TRUE); 					
	}
	if(m_pLogo)
	{
		CRect rtLogo;
		rtLogo.top     = (rtClient.Height() - m_pLogo ->Height())/2;
		rtLogo.bottom  = rtLogo.top   +    m_pLogo ->Height();
		rtLogo.right   = rtClient.right - 19;
		rtLogo.left    = rtLogo.right   - m_pLogo ->Width();
		TP_StretchDIBits(&dc,m_pLogo ->GetByte(),m_pLogo ->Width(),m_pLogo ->Height(),rtLogo); 			
	}
	dcSet.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&dc,0,0,SRCCOPY);
}
void       CTPBtnToolBar::SetBtnToolBarStateInfo(TPBtnToolBarStateInfoArray &aInfo)
{
	m_aBtnStateInfo.RemoveAll();
	for(INT l=0;l<aInfo.GetSize();l++)	m_aBtnStateInfo.Add(aInfo[l]);
	if(m_pPaletteBar) m_pPaletteBar ->SetBtnToolBarStateInfo(aInfo);
}
void        CTPBtnToolBar::SetBtnToolBarStateFlag(UINT nID,UINT uFlag)
{
	for(INT l=0;l<m_aTPButton.GetSize();l++)
	{
		if(m_aTPButton[l] && m_aTPButton[l] ->m_pControlData &&  m_aTPButton[l] ->m_pControlData->nID == nID)
		{
			m_aTPButton[l] ->SetStateFlag(uFlag);			
		}
	}
}
void        CTPBtnToolBar::SetBtnToolBarStateIndex(UINT nID,int iIndex)
{	
	for(INT l=0;l<m_aTPButton.GetSize();l++)
	{
		if(m_aTPButton[l] && m_aTPButton[l] ->m_pControlData &&  m_aTPButton[l] ->m_pControlData->nID == nID)
		{
			m_aTPButton[l] ->SetStateIndex(iIndex);			
		}
	}
}
void        CTPBtnToolBar::SetCheckEx(UINT nID,UINT *pnIDExtend,int iCountExtend,BOOL bCheck)
{
	CTPButton *pButton = (CTPButton *)GetDlgItem(nID);
	if(pButton) pButton ->SetCheck(bCheck);
	for(INT l=0;l<iCountExtend;l++)
	{
		if(nID == pnIDExtend[l]) continue;
		pButton = (CTPButton *)GetDlgItem(pnIDExtend[l]);
		if(pButton) pButton ->SetCheck(!bCheck);
	}
}
BOOL TP_IsButon(HWND hWnd)
{
	if(!::IsWindow(hWnd)) return FALSE;
	TCHAR cChar[20];ZeroMemory(cChar,sizeof(cChar));
	::GetClassName(hWnd,cChar,20);
	if(lstrcmp(cChar,WC_BUTTON) ==0) return TRUE;
	else return FALSE;
}
void CTPBtnToolBar::SetCheck(UINT nID, BOOL bCheck )
{
	CTPButton        *pButton = NULL;
	for(INT l=0;l<m_aTPButton.GetSize();l++)
	{
		if(m_aTPButton[l] && m_aTPButton[l] ->m_pControlData &&  m_aTPButton[l] ->m_pControlData->nID == nID)
		{
			pButton = m_aTPButton[l];			
			if(pButton) pButton ->SetCheck(bCheck);			
		}
	}
	if(m_uState & TPBTNTOOLSTATE_PALETTE) return;
	if(m_pPaletteBar) m_pPaletteBar ->SetCheck(nID,bCheck);
	return;
	if(m_eLinkType != TP_BtnToolBar_Unknow)
	{		
		for(INT l=0;l<m_iControlLable;l++)
		{
			if(m_pControlLable[l].x == nID)
			{
				SetCheckLink(m_pControlData,m_pControlLable[l].y,bCheck,m_eLinkType);
			}
		}	
	}
}
void CTPBtnToolBar::SetAccelerate(UINT nID,UINT uDelay,UINT uInterval)
{
	CTPButton *pButton = NULL;
	for(DWORD l=0;l<m_dwControlCount;l++)
	{
		if(m_pControlData[l].nID == nID && m_pControlData[l].hWnd)
		{
			pButton = NULL;
			pButton = (CTPButton *)CWnd::FromHandle(m_pControlData[l].hWnd);
			if(pButton) pButton->SetAccelerateTime(uDelay,uInterval);			
		}
	}
}
void        CTPBtnToolBar::SetCheckIndex(int iIndex,BOOL bCheck)
{
	if(iIndex < 0 || iIndex >=m_aTPButton.GetSize() ||  m_aTPButton[iIndex] == NULL) return;
	m_aTPButton[iIndex] ->SetCheck(bCheck);
}
BOOL        CTPBtnToolBar::GetCheck(UINT nID)
{
	CTPButton *pButton = (CTPButton *)GetDlgItem(nID);
	if(pButton) return pButton ->GetCheck();
	else 
	{
		if(m_pPaletteBar)
		{
			pButton = (CTPButton *)m_pPaletteBar->GetDlgItem(nID);
			if(pButton) return  pButton ->GetCheck();	
			else        return FALSE;
		}
		else        return FALSE;
	}
}
BOOL        CTPBtnToolBar::GetCheckIndex(int iIndex)
{
	if(iIndex < 0 || iIndex >m_aTPButton.GetSize()  ||  m_aTPButton[iIndex] == NULL) return FALSE;
	return m_aTPButton[iIndex] ->GetCheck();	
}
LRESULT CTPBtnToolBar::OnMsgGetBaseframe(WPARAM lp,LPARAM wp)
{
	if(wp == 4 && (m_uState & TPBTNTOOLSTATE_PALETTE) && m_pParentWnd)
	{
		HWND hWnd = TP_GetTopFrame(m_pParentWnd ->GetSafeHwnd());
		if(hWnd) return (LRESULT)CTPBaseFrame::GetBaseFrame(CWnd::FromHandle(hWnd));
	}
	return 0;
}
LRESULT CTPBtnToolBar::OnMsgButton(WPARAM wp,LPARAM lp)
{
	m_bHotKey   = FALSE; 
	m_hPressBtn = (HWND)lp;

	if(wp == TP_BUTTON_RDOWN)
	{
		m_pMsgWnd->SendMessage(WM_TOOLBARRCLICK,::GetDlgCtrlID((HWND)lp),lp);
	}
	return S_OK;
}
void CTPBtnToolBar::EnableExEx(UINT *pnID,int iCount,BOOL bEnable)
{
	if(bEnable)
	{
		for(INT l=0;l<m_aTPButtonEx.GetSize();l++)
			m_aTPButtonEx[l] ->EnableWindow(TRUE);
		m_aTPButtonEx.RemoveAll();
	}
	else
	{
		INT k=0, l=0;
		for(  l=0;l<m_aTPButton.GetSize();l++)
		{
			if(m_aTPButton[l]  == NULL || m_aTPButton[l] ->m_pControlData == NULL) continue;
			for(  k=0;k<iCount;k++)				
				if(m_aTPButton[l] ->m_pControlData->nID == pnID[k]) break;
			if(k>=iCount && m_aTPButton[l] ->IsWindowEnabled())
			{
				m_aTPButtonEx.Add( m_aTPButton[l]);
				m_aTPButton[l] ->EnableWindow(FALSE);
			}			 
		}
	}

	
}
void CTPBtnToolBar::EnableEx(UINT *pnID,int iCount,UINT *pnIDExtend,int iCountExtend,BOOL bEnable)
{
	CTPButton *pButton = NULL;
	for(INT l=0;l<iCount;l++)
	{		
		pButton = (CTPButton *)GetDlgItem(pnID[l]);
		if(pButton) pButton ->EnableWindow(bEnable);
	}
	for(INT l=0;l<iCountExtend;l++)
	{
		INT k=0;
		for(k=0;k<iCount;k++)
		{
			if(pnID[k] == pnIDExtend[l]) break;
		}
		if(k<iCount) continue;
		pButton = (CTPButton *)GetDlgItem(pnIDExtend[l]);
		if(pButton) pButton ->EnableWindow(!bEnable);
	}
}

void CTPBtnToolBar::Enable(UINT nID,BOOL bEnable)
{
	CTPButton        *pButton = NULL;
	for(INT l=0;l<m_aTPButton.GetSize();l++)
	{
		if(m_aTPButton[l] && m_aTPButton[l] ->m_pControlData &&  m_aTPButton[l] ->m_pControlData->nID == nID)
		{
			pButton = m_aTPButton[l];
			::EnableWindow(pButton->GetSafeHwnd() ,bEnable);
		}
	}
	if(m_uState & TPBTNTOOLSTATE_PALETTE) return;
	if(m_pPaletteBar) m_pPaletteBar->Enable(nID,bEnable);
	return;
	if(m_eLinkType != TP_BtnToolBar_Unknow)
	{		
		for(INT l=0;l<m_iControlLable;l++)
		{
			if(m_pControlLable[l].x == nID)
			{
				EnableLink(m_pControlData,m_pControlLable[l].y,bEnable,m_eLinkType);
				break;
			}
		}				
	}
}

void CTPBtnToolBar::EnableIndex(int iIndex,BOOL bEnable)
{
	if(iIndex < 0 || iIndex >=m_aTPButton.GetSize() ||  m_aTPButton[iIndex] == NULL) return ;
	m_aTPButton[iIndex] ->EnableWindow(bEnable);	
}

LRESULT CTPBtnToolBar::SetPaletteID(UINT nID,UINT uPaletteCol,UINT uPaletteLine)
{
	m_uPaletteID   = nID;
	m_uPaletteCol  = uPaletteCol;
	m_uPaletteLine = uPaletteLine;
	return S_OK;
}
LRESULT CTPBtnToolBar::SetPaletteCtlData(TPControlData   *pControlData,DWORD dwControlCount,CString sCtrlPath,BOOL bSaveData)
{	
	if(m_pPaletteBar)
	{
		m_pPaletteBar ->DestroyWindow();
		delete m_pPaletteBar;
		m_pPaletteBar = NULL;
	}
	if(pControlData == NULL) return S_FALSE;
	DWORD  dwStyle   = (WS_POPUPWINDOW|WS_CLIPCHILDREN ) & ~WS_BORDER;
	LPCTSTR lpszClassName = AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW|CS_GLOBALCLASS|CS_DBLCLKS, AfxGetApp()->LoadStandardCursor(IDC_ARROW) );
	HWND  hTopFrame  = m_hTopWnd?m_hTopWnd:TP_GetTopFrame(m_hWnd);
	CWnd *pParentWnd = 	CWnd::FromHandle(hTopFrame);	
	if(pParentWnd == NULL)  pParentWnd = AfxGetMainWnd();	 
	m_pPaletteBar = new CTPBtnToolBar();
	m_pPaletteBar->m_pParentWnd = this;
	m_pPaletteBar ->m_uState   |= TPBTNTOOLSTATE_PALETTE;
	m_pPaletteBar ->m_eLinkType = m_eLinkType;
	m_pPaletteBar ->m_szButton  = m_szButton;
	m_pPaletteBar ->m_uBarStyle = 3;
	m_pPaletteBar ->LoadPalette();
	m_pPaletteBar ->m_uControlLine = m_uPaletteLine;
	m_pPaletteBar ->m_uControlCol  = m_uPaletteCol;
	m_pPaletteBar ->m_iTopOffset   = 10;
	m_pPaletteBar ->m_iLeftOffset  = 12;
	m_pPaletteBar ->m_iBtnSpace    = 1;
	m_pPaletteBar ->CreateEx(0x00000080,lpszClassName,NULL,dwStyle,CRect(0,0,m_uPaletteCol * m_szButton.cx + (m_uPaletteCol -1 ) *m_pPaletteBar ->m_iBtnSpace + 36,m_uPaletteLine * m_szButton.cy + (m_uPaletteLine -1 ) *m_pPaletteBar ->m_iBtnSpace + 28),pParentWnd,0);	
	m_pPaletteBar ->SetBtnToolBarStateInfo(m_aBtnStateInfo);
	m_pPaletteBar ->SetControlData(pControlData,dwControlCount,sCtrlPath,bSaveData);	
	m_pPaletteBar ->m_pMsgWnd = m_pMsgWnd;
	m_hTopWnd =hTopFrame;
	return S_OK;
}
LRESULT CTPBtnToolBar::SetPaletteHotData(TPNSControlData  *pHotKeyData, DWORD dwHotKeyCount,POINT *pIndentify,int lIndentify)
{
	if(m_pPaletteBar) m_pPaletteBar->SetHotKeyData(pHotKeyData,dwHotKeyCount,pIndentify,lIndentify);
	return S_OK;
}
LRESULT CTPBtnToolBar::SetPaletteHotData(TPHotKeyData    *pHotKeyData, DWORD dwHotKeyCount,BOOL bSaveData)
{
	if(m_pPaletteBar) m_pPaletteBar->SetHotKeyData(pHotKeyData,dwHotKeyCount,bSaveData);
	return S_OK;
}
LRESULT CTPBtnToolBar::SetPaletteWndCtrlData(void *pWndCtrnData)
{
	if(m_pPaletteBar) m_pPaletteBar->SetWndCtrlData(pWndCtrnData);
	return S_OK;
}
void CTPBtnToolBar::SetWndCtrlData(void *pWndCtrnData)
{
	m_pWndCtrnData = pWndCtrnData;
}

LRESULT CTPBtnToolBar::SetControlLable(POINT *pLable,int iLable)
{
	m_pControlLable = pLable;
	m_iControlLable = iLable;
	if(m_pPaletteBar) m_pPaletteBar ->SetControlLable(pLable,iLable);
	return S_OK;
}
LRESULT CTPBtnToolBar::SetInitState()
{
	if(m_eLinkType != TP_BtnToolBar_Unknow && SetCheckLinkCallBack && m_pControlLable)
	{
		BOOL                    bCheck  = FALSE,bEnable = FALSE;
		CTPButton              *pButton = NULL;
		TP_BtnToolBarLink_Data *pLinkData = &g_aBtnToolLink[m_eLinkType];
		for(DWORD l=0;l<m_dwControlCount;l++)
		{
			for(INT k=0;k<m_iControlLable;k++)
				if(m_pControlLable->x == m_pControlData[l].nID)
				{
					pButton = NULL;
					if(pLinkData->aCtrlData[l][k].hWnd && TP_IsButon(pLinkData->aCtrlData[l][k].hWnd))
						pButton = (CTPButton *)CWnd::FromHandle(pLinkData->aCtrlData[l][k].hWnd);
					if(pButton == NULL) continue;
					g_aBtnToolLink[m_eLinkType].GetState(m_pControlLable->y,bCheck,bEnable);
					if(SetCheckLinkCallBack(m_pControlLable->y,bCheck,m_pLinkCallBack))
					{						
						pButton ->SetCheck(bCheck);
					}
					if(EnableLinkCallBack(m_pControlLable->y,bEnable,m_pLinkCallBack))
					{						
						pButton ->EnableWindow(bEnable);
					}
				}
		}		
	}
	return S_OK;
}
LRESULT CTPBtnToolBar::SetLinkData()
{
	if(m_eLinkType != TP_BtnToolBar_Unknow)
	{
		ASSERT(m_iLinkIndex>=0);
		g_aBtnToolLink[m_eLinkType].SetData(m_iLinkIndex,m_pControlData,m_dwControlCount,m_pControlLable,m_iControlLable,this);
		if(m_pPaletteBar) m_pPaletteBar->SetLinkData();
	}
	return S_OK;
}
 
LRESULT CTPBtnToolBar::SetPaletteCtlData(TPNSControlData  *pControlData, DWORD dwControlCount,CString sCtrlPath,POINT *pIndentify,int lIndentify)
{
	if(m_pPaletteBar)
	{
		m_pPaletteBar ->DestroyWindow();
		delete m_pPaletteBar;
		m_pPaletteBar = NULL;
	}
	DWORD  dwStyle = (WS_POPUPWINDOW|WS_CLIPCHILDREN ) & ~WS_BORDER;
	LPCTSTR lpszClassName = AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW|CS_GLOBALCLASS|CS_DBLCLKS, AfxGetApp()->LoadStandardCursor(IDC_ARROW) );
	HWND  hTopFrame  = m_hTopWnd?m_hTopWnd:TP_GetTopFrame(m_hWnd);
	CWnd *pParentWnd = 	CWnd::FromHandle(hTopFrame);	
	if(pParentWnd == NULL)  pParentWnd = AfxGetMainWnd();
	m_pPaletteBar = new CTPBtnToolBar();
	m_pPaletteBar->m_pParentWnd = this;
	m_pPaletteBar ->m_uState   |= TPBTNTOOLSTATE_PALETTE;
	m_pPaletteBar ->m_eLinkType = m_eLinkType;
	m_pPaletteBar ->m_szButton  = m_szButton;
	m_pPaletteBar ->m_uBarStyle = 3;
	m_pPaletteBar ->LoadPalette();
	m_pPaletteBar ->m_uControlLine = m_uPaletteLine;
	m_pPaletteBar ->m_uControlCol  = m_uPaletteCol;
	m_pPaletteBar ->m_iTopOffset   = 10;
	m_pPaletteBar ->m_iLeftOffset  = 12;
	m_pPaletteBar ->m_iBtnSpace    = 1;
	m_pPaletteBar ->CreateEx(0x00000080,lpszClassName,NULL,dwStyle,CRect(0,0,m_uPaletteCol * m_szButton.cx + (m_uPaletteCol -1 ) *m_pPaletteBar ->m_iBtnSpace + 36,m_uPaletteLine * m_szButton.cy + (m_uPaletteLine -1 ) *m_pPaletteBar ->m_iBtnSpace + 28),pParentWnd,0);	
	m_pPaletteBar ->SetControlData(pControlData,dwControlCount,sCtrlPath,pIndentify,lIndentify);	
	m_pPaletteBar ->m_pMsgWnd = m_pMsgWnd;
	m_hTopWnd = hTopFrame;

	return S_OK;
}
LRESULT CTPBtnToolBar::SetControlData(TPNSControlData  *pControlData, DWORD dwControlCount,CString sCtrlPath,POINT *pIndentify,int lIndentify)
{
	if(m_pControlDataSave) delete [] m_pControlDataSave; m_pControlDataSave = new TPControlData[dwControlCount];
	m_pControlDataSave = NULL;
	TP_ExchangeControlData(pControlData,m_pControlDataSave, dwControlCount,m_dwControlCount,pIndentify,lIndentify);		 
	SetControlData(m_pControlDataSave,m_dwControlCount,sCtrlPath,FALSE);
	return S_OK;
}
 LRESULT CTPBtnToolBar::SetControlData(TPControlData   *pControlData,DWORD dwControlCount,CString sCtrlPath,BOOL bSaveData)
{	
	CRect      rtClient;	
	CTPButton *pButton; 
	int        iCx = 10 +m_iExLeft;
	GetClientRect(&rtClient);
	
	if(m_eLinkType != TP_BtnToolBar_Unknow)
		g_aBtnToolLink[m_eLinkType].Remove(m_iLinkIndex);

	if(bSaveData)
	{
		if(m_pControlDataSave) delete [] m_pControlDataSave; m_pControlDataSave = new TPControlData[dwControlCount];
		for(DWORD l=0;l<dwControlCount;l++) m_pControlDataSave[l] = pControlData[l];
		m_pControlData   = m_pControlDataSave;
	}
	else	m_pControlData   = pControlData;

	m_dwControlCount = dwControlCount;

	if(m_hContinueTimer) KillTimer(m_hContinueTimer);
	m_hContinueTimer = NULL;

	m_hPressBtn      = NULL;
	for(INT l=0;l<m_aTPButton.GetSize();l++)
	{
		if(m_aTPButton[l])
		{
			if(m_aTPButton[l]->m_lParam) ::DeleteObject((HRGN)m_aTPButton[l]->m_lParam);
			m_aTPButton[l]->m_lParam = NULL;
			m_aTPButton[l]->DestroyWindow();
			delete m_aTPButton[l];
		}
	}
	m_aTPButton.RemoveAll();

	for(DWORD l=0;l<dwControlCount;l++)
	{	
		m_pControlData[l].pHotKeyData = NULL;
		m_pControlData[l].hWnd        = NULL;

		if(m_pControlData[l].nID == IDC_SEPARATE || m_pControlData[l].nID <= 0)
		{
			pButton = NULL;
			m_aTPButton.Add(pButton);
		}
		else
		{		
			pButton  = new CTPButton();
			pButton ->m_pControlData = &m_pControlData[l];
			pButton ->Create(_L(""),m_pControlData[l].dwStyle ,CRect(iCx,rtClient.top+4,iCx +20,rtClient.bottom -4),this,m_pControlData[l].nID);
			pButton ->SetToolTipText(m_pControlData[l].sNotifyUp,m_pControlData[l].sNotifyDown);
			pButton ->SetPrivateStyle(TPBTNTYPE_TOOLBAR,0);

			if(pButton ->m_pControlData->dwState & TP_CONTROL_DOUBLE) pButton->SetStretch(pButton ->m_pControlData->dwState ,5,6);
			if(m_pControlData[l].dwStyle & BS_PUSHLIKE)
			{
				if(m_pControlData[l].dwState & TP_CONTROL_PUSHLIKE)	  pButton ->SetBitmaps(sCtrlPath + _L("\\") + m_pControlData[l].sFaceRes,FALSE,1000);
				else                                                  pButton ->SetBitmaps(sCtrlPath + _L("\\") + m_pControlData[l].sFaceRes,FALSE,FALSE);
			}
			else                                                      pButton ->SetBitmaps(sCtrlPath + _L("\\") + m_pControlData[l].sFaceRes,FALSE,TRUE);
			iCx +=25;
			m_pControlData[l].hWnd = pButton ->GetSafeHwnd();
			m_aTPButton.Add(pButton);
			for(INT j=0;j<m_aBtnStateInfo.GetSize();j++) 		
				if(m_aBtnStateInfo[j].nID == m_pControlData[l].nID) {pButton->SetStateInfo(m_aBtnStateInfo[j].aState);break;}			
		}
	}	
	SetInitState();

	if((m_uBaseState & TP_BASESTATE_CENTER)  && !(m_uState & TPBTNTOOLSTATE_PALETTE))
	{
		FunCalcSize();
		m_iTopX = max(0,(rtClient.Width() - m_iTotalWidth) /2);
	}

	SetSize(TRUE);	
	return S_OK;	
}
LRESULT CTPBtnToolBar::SetHotKeyDataEx(TPHotKeyData    *pHotKeyData, DWORD dwHotKeyCount )
{
	m_dwHotKeyCountEx  = dwHotKeyCount;
	m_pHotKeyDataEx    = pHotKeyData;
	if(m_hContinueTimer) KillTimer(m_hContinueTimer);
	m_hContinueTimer = NULL;
	for(DWORD l=0;l<dwHotKeyCount;l++)
	{
		pHotKeyData[l].pControlData = NULL;
		if (pHotKeyData[l].vkCharSet == 0 && pHotKeyData[l].uFlagSet == 0)
		{
			pHotKeyData[l].vkCharSet = pHotKeyData[l].vkChar;
			pHotKeyData[l].uFlagSet  = pHotKeyData[l].uFlag;
		}
	}
	return S_OK;
} 
 

UINT CTPBtnToolBar::GetIndentify(UINT nID)
{
	for(INT l=0;l<m_iControlLable;l++)
	{
		if(m_pControlLable[l].x == nID) return m_pControlLable[l].y;
	}
	return -1;
}
UINT CTPBtnToolBar::GetID(UINT uIndentify)
{
	for(INT l=0;l<m_iControlLable;l++)
	{
		if(m_pControlLable[l].y == uIndentify) return m_pControlLable[l].x;
	}
	return -1;
}
LRESULT CTPBtnToolBar::SetHotKeyData(TPNSControlData *pHotKeyData, DWORD dwHotKeyCount,CTPButton *pButton,POINT *pIndentify,long  lIndentify)
{		
	if(pButton == NULL) return S_FALSE;
	UINT nID = pButton ->GetDlgCtrlID();
	pButton ->SetHotKeyData(0,0);
	for(DWORD m=0;m<dwHotKeyCount;m++)
	{
		INT j=0;
		for(j=0;j<lIndentify;j++)
		{
			if(pIndentify[j].x == pHotKeyData[m].dwIdentifer) break;
		}
		if(j>=lIndentify) continue;
		if(pIndentify[j].y == nID  && pHotKeyData[m].vkChar<0xFF)
		{
			pButton ->SetHotKeyData(pHotKeyData[m].vkChar,pHotKeyData[m].uFlag);
			break;
		}
	}
	return S_OK;
}
LRESULT CTPBtnToolBar::SetHotKeyData(TPNSControlData  *pHotKeyData, DWORD dwHotKeyCount,POINT *pIndentify,int lIndentify)
{
	if(m_pHotKeyDataSave) delete [] m_pHotKeyDataSave;
	m_pHotKeyDataSave = NULL;
	TP_ExchangeHotKeyData(pHotKeyData,m_pHotKeyDataSave,dwHotKeyCount,m_dwHotKeyCount,pIndentify,lIndentify);
	SetHotKeyData(m_pHotKeyDataSave,m_dwHotKeyCount,FALSE);	
	return S_OK;
}
LRESULT CTPBtnToolBar::SetHotKeyData(TPHotKeyData    *pHotKeyData, DWORD dwHotKeyCount,BOOL bSaveData)
{	
	if(bSaveData)
	{
		if(m_pHotKeyDataSave) delete [] m_pHotKeyDataSave; m_pHotKeyDataSave = new TPHotKeyData[dwHotKeyCount];
		for(DWORD l=0;l<dwHotKeyCount;l++) m_pHotKeyDataSave[l] = pHotKeyData[l];
		m_pHotKeyData   = m_pHotKeyDataSave;
	}
	else	m_pHotKeyData   = pHotKeyData;

	m_dwHotKeyCount  = dwHotKeyCount;

	if(m_hContinueTimer) KillTimer(m_hContinueTimer);
	m_hContinueTimer = NULL;

	for(DWORD l=0;l<dwHotKeyCount;l++)
	{
		m_pHotKeyData[l].pControlData = NULL;
		/*if(m_pHotKeyData[l].vkChar>=0xFF && g_pGlobalHotKey)
		{
			for(int m = 0 ; m <g_lGlobalHotKey;m++)
			{
				if(g_pGlobalHotKey[m].nID == m_pHotKeyData[l].vkChar)
				{
					pHotKeyData[l].vkCharSet = g_pGlobalHotKey[m].vkChar;
					pHotKeyData[l].uFlagSet  = g_pGlobalHotKey[m].uFlag;
					break;
				}
			}			
		}*/
		if (pHotKeyData[l].vkCharSet == 0 && pHotKeyData[l].uFlagSet == 0)
		{
			pHotKeyData[l].vkCharSet = pHotKeyData[l].vkChar;
			pHotKeyData[l].uFlagSet  = pHotKeyData[l].uFlag;
		}
	}
	CTPButton *pButton = NULL;
	for(DWORD l=0;l<m_dwControlCount;l++)
	{
		if(!m_pControlData[l].hWnd) continue;
		pButton = (CTPButton *)CWnd::FromHandle(m_pControlData[l].hWnd);
		if(pButton == NULL)         continue;
		DWORD m=0;
		for(m=0;m<m_dwHotKeyCount;m++)
		{
			if(m_pHotKeyData[m].nID == m_pControlData[l].nID && m_pHotKeyData[m].vkChar<0xFF)
			{
				m_pControlData[l].pHotKeyData = &m_pHotKeyData[m];
				m_pHotKeyData[m].pControlData = &m_pControlData[l];
				pButton ->SetHotKeyData(&m_pHotKeyData[m]);
				break;
			}
		}
		if(m>=m_dwHotKeyCount) pButton ->SetHotKeyData(NULL);
	}
	return S_OK;
}

void CTPBtnToolBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if((m_uBaseState & TP_BASESTATE_CENTER) && !(m_uState & TPBTNTOOLSTATE_PALETTE))
		m_iTopX = max(0,(cx - m_iTotalWidth) /2);
	else if(m_iTotalWidth < cx)
		m_iTopX = min(10,(cx - m_iTotalWidth) /2);
	SetSize(FALSE);	
}
CSize CTPBtnToolBar::GetWndSize()            
{
	return CSize(8+ m_uControlCol * m_szButton.cx + (m_uControlCol -1) *m_iBtnSpace,8+ m_uControlLine * m_szButton.cy + (m_uControlLine -1) * m_iBtnSpace);
}
void CTPBtnToolBar::FunCalcSize()
{
	CSize      szButton = m_szButton;
	int        iCx = m_iTopX + m_iExLeft+m_iLeftSpace;
	int        iCy = 4;
	UINT    iLineCount = 0,iColCount = 0;
	int        iBtnCount  = m_aTPButton.GetSize();

	m_iTotalWidth    = 0;
	for(INT l=0;l<iBtnCount;l++)
	{		
		if(m_aTPButton[l] && m_aTPButton[l] ->m_pControlData)
		{
			if(iLineCount >=m_uControlLine)  break;			 
			else if(!(m_aTPButton[l] ->m_pControlData ->dwStateSet & (TP_CONTROL_HIDEFORCE| TP_CONTROL_HIDE)) ||  m_aTPButton[l] ->m_pControlData ->dwStateSet & TP_CONTROL_UNSET)
			{
				szButton = m_szButton;
 				if(m_aTPButton[l] ->m_pControlData->dwState & TP_CONTROL_DOUBLE)   szButton.cx =  szButton.cx *2 + m_iBtnSpace;
				iColCount ++;				
				if(iColCount >=m_uControlCol)
				{
					m_iTotalWidth = max(m_iTotalWidth,iCx - m_iTopX);
					iCx =m_iTopX + m_iExLeft;
					iLineCount ++;
					iColCount = 0;
					iCy += m_szButton.cy + m_iBtnSpace;
				}
				else 	if(l<iBtnCount-1)
				{
					iCx +=szButton.cx + m_iBtnSpace;		
					if(m_aTPButton[l] ->m_pControlData->dwState & TP_CONTROL_DOUBLE) iCx -= (szButton.cx - m_iBtnSpace)/2 +m_iBtnSpace;
				}
			}			
		}
		else 
		{
			iColCount ++;
			if(iColCount >=m_uControlCol)
			{
				m_iTotalWidth = max(m_iTotalWidth,iCx - m_iTopX);
				iCx =m_iTopX + m_iExLeft;
				iLineCount ++;
				iColCount = 0;
				iCy += m_szButton.cy + m_iBtnSpace;
			}
			else if(l<iBtnCount-1)
				iCx += m_szButton.cx + m_iBtnSpace;			
		}
	}	
	m_iTotalWidth = max(m_iTotalWidth,iCx - m_iTopX) + szButton.cx;
	
}
void CTPBtnToolBar::SetSize(BOOL bHideWnd)
{
	CRect      rtClient,rtTemp;
	CSize      szButton = m_szButton;
	GetClientRect(rtClient);	
	if(m_uBarStyle == 1) rtClient.bottom -=4;
	int        iCx = m_iTopX + m_iExLeft+m_iLeftSpace;
	int        iCy = 4,l=0;
	UINT    iLineCount = 0,iColCount = 0;
	int        iBtnCount  = m_aTPButton.GetSize();
	if(m_uControlLine == 1)  iCy =  rtClient.top + (rtClient.Height() - m_szButton.cy) / 2+1;
	m_iButtonSize[0] = 0;
	m_iButtonSize[1] = 0;
	m_iTotalWidth    = 0;
	m_iButtonSize[0] = szButton.cx + m_iBtnSpace;
	m_bBtnScroll[0]  = m_bBtnScroll[1] = FALSE;
	for(  l=0;l<iBtnCount;l++)
	{		
		if(m_aTPButton[l] && m_aTPButton[l] ->m_pControlData)
		{
			if(iLineCount >=m_uControlLine) 
			{
				if(bHideWnd)
				{
					m_aTPButton[l] ->ShowWindow(SW_HIDE);
					m_aTPButton[l] ->m_bVisible = FALSE;
				}
			}
			else if(!(m_aTPButton[l] ->m_pControlData ->dwStateSet & (TP_CONTROL_HIDEFORCE| TP_CONTROL_HIDE)) ||  m_aTPButton[l] ->m_pControlData ->dwStateSet & TP_CONTROL_UNSET)
			{
				if(bHideWnd){ m_aTPButton[l] ->ShowWindow(SW_SHOW);m_aTPButton[l] ->m_bVisible = TRUE;}
				if(m_szButton.cx >0) szButton = m_szButton;
				else { TPCommonRes *pRes = m_aTPButton[l] ->GetBtnRes(BUTTON_NOR); szButton = CSize(int(pRes ->Width() * m_dAmp),int(pRes ->Height()*m_dAmp)); }
				if(m_aTPButton[l] ->m_pControlData->dwState & TP_CONTROL_DOUBLE)   szButton.cx =  szButton.cx *2 + m_iBtnSpace;
				if(m_iExLeft>0) 	
				{
					if(iCx <m_iExLeft) {m_bBtnScroll[0] = TRUE; m_aTPButton[l] ->m_bVisible = FALSE; m_aTPButton[l] ->ShowWindow(SW_HIDE);m_iButtonSize[0] = szButton.cx + m_iBtnSpace;}
					else if(iCx + szButton.cx + m_iBtnSpace >rtClient.right){m_aTPButton[l] ->m_bVisible = FALSE; m_aTPButton[l] ->ShowWindow(SW_HIDE);m_bBtnScroll[1] = TRUE;}
					else
					{
						if(!m_aTPButton[l]->m_bVisible) m_aTPButton[l]->ShowWindow(SW_SHOW);
						m_aTPButton[l] ->m_bVisible = TRUE;
						if(m_iButtonSize[1] == 0) m_iButtonSize[1] = szButton.cx + m_iBtnSpace;
						m_aTPButton[l] ->MoveWindow(CRect(m_iLeftOffset + iCx,m_iTopOffset +iCy ,m_iLeftOffset + iCx +szButton.cx,m_iTopOffset + iCy + szButton.cy));
					}
				}				
				else
				{
					m_aTPButton[l] ->MoveWindow(CRect(m_iLeftOffset + iCx,m_iTopOffset +iCy,m_iLeftOffset + iCx +szButton.cx,m_iTopOffset +iCy + szButton.cy));
					if(iCx <m_iExLeft)               { m_iButtonSize[0] = szButton.cx + m_iBtnSpace;m_bBtnScroll[0] = TRUE;}
					else if(iCx + szButton.cx + m_iBtnSpace >rtClient.right){m_bBtnScroll[1] = TRUE;}
					else{	if(m_iButtonSize[1] == 0) m_iButtonSize[1] = szButton.cx + m_iBtnSpace;}
				}

				iColCount ++;				

				if(iColCount >=m_uControlCol)
				{
					m_iTotalWidth = max(m_iTotalWidth,iCx - m_iTopX);
					iCx =m_iTopX + m_iExLeft;
					iLineCount ++;
					iColCount = 0;
					iCy += m_szButton.cy + m_iBtnSpace;
				}
				else 	if(l<iBtnCount-1)
				{
					iCx +=szButton.cx + m_iBtnSpace;		
					if(m_aTPButton[l] ->m_pControlData->dwState & TP_CONTROL_DOUBLE) iCx -= (szButton.cx - m_iBtnSpace)/2 +m_iBtnSpace;
				}
				m_aTPButton[l] ->Invalidate();
				
			}
			else
			{
				if(bHideWnd)
				{
					m_aTPButton[l] ->ShowWindow(SW_HIDE);
					m_aTPButton[l] ->m_bVisible = FALSE;
				}
			}
		}
		else 
		{
			iColCount ++;
			if(iColCount >=m_uControlCol)
			{
				m_iTotalWidth = max(m_iTotalWidth,iCx - m_iTopX);
				iCx =m_iTopX + m_iExLeft;
				iLineCount ++;
				iColCount = 0;
				iCy += m_szButton.cy + m_iBtnSpace;
			}
			else	if(l<iBtnCount-1)
				iCx += m_szButton.cx + m_iBtnSpace;			
		}
	}	
	m_iTotalWidth = max(m_iTotalWidth,iCx - m_iTopX) + szButton.cx;

	if(l>=iBtnCount-1 && iLineCount <= 1)
	{	
		iCx           += m_szButton.cy + m_iBtnSpace;
		m_iTotalWidth += m_szButton.cy + m_iBtnSpace;
	}
	if(m_pAttachWnd)
	{
		CRect rtWnd;
		m_pAttachWnd->GetWindowRect(&rtWnd);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
		rtWnd.OffsetRect(iCx + 4,0);
		rtWnd.OffsetRect(0,(rtClient.Height() - rtWnd.Height())/2);
		//XN00023743_WFP_20091030
		//if(rtWnd.right  + m_iTopX> rtClient.Width() -4)
		if(rtWnd.right> rtClient.Width() -4)
		{
			m_pAttachWnd ->ShowWindow(SW_HIDE);
			m_bBtnScroll[1] = TRUE;
		}
		else
		{
			m_pAttachWnd ->ShowWindow(SW_SHOW);
			m_pAttachWnd ->Invalidate();
			m_pAttachWnd ->MoveWindow(rtWnd);
			rtTemp = rtWnd;		//ScreenToClient(&rtTemp); //zhaofei XN00020530
			if(rtTemp.right>rtClient.right) m_bBtnScroll[1] = TRUE;
		}
		iCx           += rtWnd.Width()+4;
		m_iTotalWidth += rtWnd.Width()+4;
	}
	if(m_pAttachWndEx)
	{
		CRect rtWnd;
		m_pAttachWndEx->GetWindowRect(&rtWnd);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
		rtWnd.OffsetRect(iCx+8,0);
		rtWnd.OffsetRect(0,(rtClient.Height() - rtWnd.Height())/2);
		//XN00023743_WFP_20091030
		//if(rtWnd.right + m_iTopX> rtClient.Width() -4) 
		if(rtWnd.right> rtClient.Width() -4) 
		{
			m_pAttachWndEx ->ShowWindow(SW_HIDE);
			m_bBtnScroll[1] = TRUE;
		}
		else
		{			
			m_pAttachWndEx ->ShowWindow(SW_SHOW);
			m_pAttachWndEx ->Invalidate();
			m_pAttachWndEx ->MoveWindow(rtWnd);
			rtTemp = rtWnd;		
			//ScreenToClient(&rtTemp);   //zhaofei XN00020530
			if(rtTemp.right>rtClient.right) m_bBtnScroll[1] = TRUE;
		}
		m_iTotalWidth += rtWnd.Width() + 12;
	}	
	SetScrollBtn();	
}
void CTPBtnToolBar::SetScrollBtn()
{
	if(!m_bMouseWheel || m_pBtnScroll[0] == NULL) return;

	CRect rtClient,rtWnd;
	HRGN  rgnTemp;
	int   iBtnWidth  = m_pBtnScroll[0]->GetBtnResWidth();
	int   iBtnHeight = m_pBtnScroll[0]->GetBtnResHeight();
	GetClientRect(&rtClient);
	if(m_hBtnScrollRgn) DeleteObject(m_hBtnScrollRgn);	
	int   iTopx      =    rtClient.top + (rtClient.Height() - iBtnHeight)/2;
	m_hBtnScrollRgn = ::CreateRectRgn(0,0,rtClient.Width(),rtClient.Height());
	if(m_bBtnScroll[0])
	{		
		if(!m_pBtnScroll[0]->m_bVisible) m_pBtnScroll[0]->ShowWindow(SW_SHOW);
		rtWnd = CRect(0,iTopx,iBtnWidth,rtClient.Height()-iTopx);
		rgnTemp = CreateRectRgn(rtWnd.left,rtWnd.top,rtWnd.right,rtWnd.bottom);			
		CombineRgn(m_hBtnScrollRgn,m_hBtnScrollRgn,rgnTemp,RGN_DIFF);	
		DeleteObject(rgnTemp);
		ClientToScreen(&rtWnd);
		GetParent()->ScreenToClient(&rtWnd);
		m_pBtnScroll[0]->MoveWindow(rtWnd);				
	} 
	else
	{		
		if(m_pBtnScroll[0]->m_bVisible) m_pBtnScroll[0]->ShowWindow(SW_HIDE);		
	}
	if(m_bBtnScroll[1])
	{
		if(!m_pBtnScroll[1]->m_bVisible) m_pBtnScroll[1]->ShowWindow(SW_SHOW);
		rtWnd = CRect(rtClient.right-iBtnWidth,iTopx,rtClient.right,rtClient.Height()-iTopx);
		rgnTemp = CreateRectRgn(rtWnd.left,rtWnd.top,rtWnd.right,rtWnd.bottom);			
		CombineRgn(m_hBtnScrollRgn,m_hBtnScrollRgn ,rgnTemp,RGN_DIFF);	
		DeleteObject(rgnTemp);				
		ClientToScreen(&rtWnd);
		GetParent()->ScreenToClient(&rtWnd);
		m_pBtnScroll[1]->MoveWindow(rtWnd);		
	}
	else
	{	
		if(m_pBtnScroll[1]->m_bVisible) m_pBtnScroll[1]->ShowWindow(SW_HIDE);				
	}			
	::SetWindowRgn(m_hWnd,m_hBtnScrollRgn,TRUE);
}

BOOL CTPBtnToolBar::Create(DWORD uStyle,RECT &rtWnd,CWnd *pWnd,UINT nID)
{
	m_uBarStyle = uStyle;
	return CWnd::Create(NULL,NULL,WS_VISIBLE | WS_CHILD |WS_CLIPCHILDREN,rtWnd,pWnd,nID);
}
BOOL TP_CmpKeyState(UINT uFlageSet,UINT uSate,UINT uFlage)
{
	uFlage &= ~(TP_C2|TP_C5);
	if((uSate & TP_CONTROL_NOALTCTRL) && ((uFlage &(TP_ALT | TP_CONTROL)) == (TP_ALT | TP_CONTROL))) return TRUE;
	if((uSate & TP_CONTROL_NOALT))    {uFlageSet|=TP_ALT;     uFlage|=TP_ALT;    } //&& (uFlage &TP_ALT))     return TRUE;
	if((uSate & TP_CONTROL_NOCTRL))   {uFlageSet|=TP_CONTROL; uFlage|=TP_CONTROL;}  //&& (uFlage &TP_CONTROL)) return TRUE;
	if((uSate & TP_CONTROL_NOSHIFT))  {uFlageSet|=TP_SHIFT;   uFlage|=TP_SHIFT;  }  //&& (uFlage &TP_SHIFT))  return TRUE;
	if(uSate & TP_CONTROL_NOSTATE)     		                   return TRUE;
	
	if(uFlageSet == uFlage) return TRUE;
	return FALSE;
}
BOOL TP_CheckKeyState(UINT uFlageSet,UINT &uFlag)
{
	if(uFlageSet&TP_ALT)     uFlag &=~TP_ALT;
	if(uFlageSet&TP_CONTROL) uFlag &=~TP_CONTROL;
	if(uFlageSet&TP_SHIFT)   uFlag &=~TP_SHIFT;
	return TRUE;
}
int CTPBtnToolBar::OnKeyDownEx(TPNSControlData *pHotkeyData,long lHotkeySize ,UINT vKey)
{
	UINT uFlag     =  TP_GetKeyFlag(vKey);
	if(pHotkeyData && lHotkeySize>0)
	{
		for(long i=0;i<lHotkeySize;i++)
		{		
			if(pHotkeyData[i].dwStyleEx & (TP_CONTROL_NOUSE|TP_CONTROL_STATEKEY)) continue;
			if(pHotkeyData[i].dwStyleEx & TP_CONTROL_NOSTATE)     	               continue;
			if(TP_CmpKeyState(pHotkeyData[i].uFlag,pHotkeyData[i].dwStyleEx,uFlag) && pHotkeyData[i].vkChar == vKey)
			{			
				return pHotkeyData[i].nID ;	
			}				
		}
	}
	return -1;

}
 
LRESULT CTPBtnToolBar::OnKeyDown(UINT vKey)
{
	UINT uFlag     =  TP_GetKeyFlag(vKey);
	if(m_hContinueTimer) KillTimer(m_hContinueTimer);
	m_hContinueTimer       = NULL;
	CTPButton::m_hPressBtn = NULL;

	if (vKey>20000)//TouchPanel发送的KeyDown消息
	{
		m_pMsgWnd ->SendMessage(WM_TOOLBARCLICK,vKey-20000,0);
		return S_OK;
	}

	if(m_pWndCtrnData)
	{
		TPNSWndControlData *pWndData = (TPNSWndControlData *)m_pWndCtrnData;
		for(DWORD l=0;l<pWndData ->dwCount;l++)
		{
			if(pWndData ->pDwIdentifer->y == vKey)
			{
				for(DWORD m=0;m<4;m++)
				{
					for(DWORD k=0;k<pWndData ->dwCtrlIdentiferNum[m];k++)
					{
						if(pWndData ->pDwCtrlGroupIdentifer[m][k] == pWndData ->pDwIdentifer->x)
						{
							m_bHotKey = TRUE;				
							if(uFlag & (TP_C2|TP_C5))
								m_pMsgWnd ->SendMessage(WM_TOOLBARCLICK,vKey,uFlag);
							else
								m_pMsgWnd ->PostMessage(WM_TOOLBARCLICK,vKey,uFlag);
							for(DWORD n=0;n<m_dwControlCount;n++)
							{
								if(m_pControlData[n].nID == vKey && m_pControlData[n].dwStyle & BS_PUSHLIKE)
								{
									CTPButton *pButton = (CTPButton *)GetDlgItem(vKey);
									if(pButton) pButton ->SetCheck(pButton->GetCheck());
								}
							}
							return S_OK;
						}
					}
				}
			}
		}
	}	
 	//if(!m_bPaletteShow || (m_uState & TPBTNTOOLSTATE_PALETTE))
	if(!(m_pPaletteBar && m_pPaletteBar->m_bPaletteShow) || (m_uState & TPBTNTOOLSTATE_PALETTE))//XN00019723 jw 20101103  TPBTNTOOLSTATE_PALETTE(标识palette类型) m_bPaletteShow(标识palette自己的状态)
	{	
		for(DWORD i=0;i<m_dwHotKeyCount;i++)
		{		
			if(m_pHotKeyData[i].uState & (TP_CONTROL_NOUSE|TP_CONTROL_STATEKEY) ) continue;	
			if(m_pHotKeyData[i].uState & TP_CONTROL_NOSTATE)                 	  continue;
			if(TP_CmpKeyState(m_pHotKeyData[i].uFlagSet,m_pHotKeyData[i].uState,uFlag) && m_pHotKeyData[i].vkCharSet == vKey)
			{			
				if(m_pMsgWnd && ::IsWindow(m_pMsgWnd ->GetSafeHwnd()))
				{
					if(m_pPaletteBar && m_pHotKeyData[i].nID == m_uPaletteID)
					{
						ShowPalette();
					}
					else
					{					
						m_bHotKey = TRUE;
						TP_CheckKeyState(m_pHotKeyData[i].uFlagSet,uFlag);
						if(uFlag & (TP_C2|TP_C5))
							m_pMsgWnd ->SendMessage(WM_TOOLBARCLICK,m_pHotKeyData[i].nID,uFlag);	
						else 
							m_pMsgWnd ->PostMessage(WM_TOOLBARCLICK,m_pHotKeyData[i].nID,uFlag);	
#ifdef TP_NS_LOG	
						//TP_LogEditNotifyCtrl(0,m_pHotKeyData[i].sDescrip,CTPBaseFrame::GetActiveFrameCaption(),EDIT_LOGGER_MODULE,(CString)__FUNCTION__);									
#endif
						if(m_pHotKeyData[i].pControlData)
						{
							if(((TPControlData *)m_pHotKeyData[i].pControlData)->dwStyle & BS_PUSHLIKE)
							{
								CTPButton *pButton = (CTPButton *)GetDlgItem(m_pHotKeyData[i].nID);
								if(pButton) pButton ->SetCheck(pButton->GetCheck());
							}
						}						
					}
				}
				return S_OK;
			}				
		}		
 		if(m_pHotKeyDataEx && m_dwHotKeyCountEx>0)
		{
			for(DWORD i=0;i<m_dwHotKeyCountEx;i++)
			{		
				if(m_pHotKeyDataEx[i].uState & (TP_CONTROL_NOUSE|TP_CONTROL_STATEKEY)) continue;
				if(m_pHotKeyDataEx[i].uState & TP_CONTROL_NOSTATE)     	               continue;
				if(TP_CmpKeyState(m_pHotKeyDataEx[i].uFlagSet,m_pHotKeyDataEx[i].uState,uFlag) && m_pHotKeyDataEx[i].vkCharSet == vKey)
				{			
					if((m_pHotKeyDataEx[i].uState & TP_CONTROL_NOAPPLY) && !(m_uBaseState & TP_BASESTATE_APPLY)) continue;
					if(m_pMsgWnd && ::IsWindow(m_pMsgWnd ->GetSafeHwnd()))
					{
						if(m_pPaletteBar && m_pHotKeyDataEx[i].nID == m_uPaletteID)
						{
							ShowPalette();
						}
						else
						{	
							m_bHotKey = TRUE;
							TP_CheckKeyState(m_pHotKeyDataEx[i].uFlagSet,uFlag);
							if(uFlag & (TP_C2|TP_C5))
								m_pMsgWnd ->SendMessage(WM_TOOLBARCLICK,m_pHotKeyDataEx[i].nID,uFlag);	
							else
								m_pMsgWnd ->PostMessage(WM_TOOLBARCLICK,m_pHotKeyDataEx[i].nID,uFlag);	
#ifdef TP_NS_LOG	
					//		TP_LogEditNotifyCtrl(0,m_pHotKeyDataEx[i].sDescrip,CTPBaseFrame::GetActiveFrameCaption(),EDIT_LOGGER_MODULE,(CString)__FUNCTION__);									
#endif
							if(m_pHotKeyDataEx[i].pControlData)
							{
								if(((TPControlData *)m_pHotKeyDataEx[i].pControlData)->dwStyle & BS_PUSHLIKE)
								{
									CTPButton *pButton = (CTPButton *)GetDlgItem(m_pHotKeyDataEx[i].nID);
									if(pButton) pButton ->SetCheck(pButton->GetCheck());
								}
							}					
						}
					}
					return S_OK;
				}				
			}
		}
 		return S_FALSE;
	}
	else if(m_pPaletteBar) return  m_pPaletteBar->OnKeyDown(vKey);
	else              return S_FALSE;	
}
void CTPBtnToolBar::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	OnKeyDown(nChar);
//	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CTPBtnToolBar::SetToolTipCallBack(UINT nID,CString (*p)(LPVOID),LPVOID pParam)
{
	for(INT l=0;l<m_aTPButton.GetSize();l++)
	{
		if(m_aTPButton[l] && m_aTPButton[l]->GetDlgCtrlID() == nID)
		{
			m_aTPButton[l]  ->m_pButtonToolTipCallBack = p;
			m_aTPButton[l]  ->m_pToolTipParam          = pParam;
		}		
	}	
}
void CTPBtnToolBar::SetToolTip(UINT nID,CString sUp,CString sDown)
{
	for(INT l=0;l<m_aTPButton.GetSize();l++)
	{
		if(m_aTPButton[l] && m_aTPButton[l]->GetDlgCtrlID() == nID)
			m_aTPButton[l] ->SetToolTipText(sUp,sDown);	
	}	
}
void CTPBtnToolBar::ShowHotKeyData(TPHotKeyData *pHotKeyData, DWORD dwHotKeyCount,UINT uID,BOOL bUse)
{
	for(DWORD l=0;l<dwHotKeyCount;l++)
	{
		if(pHotKeyData[l].nID == uID)
		{
			if(!bUse)  pHotKeyData[l].uState |=  TP_CONTROL_NOUSE;
			else       pHotKeyData[l].uState &= ~TP_CONTROL_NOUSE;
		}
	}
}
void CTPBtnToolBar::ShowControlData(TPControlData *pControlData,DWORD dwControlCount,UINT uID,BOOL bShow)
{
	for(DWORD l=0;l<dwControlCount;l++)
	{
		if(pControlData[l].nID == uID)
		{
			if(bShow)  pControlData[l].dwStateSet |=  TP_CONTROL_HIDE;
			else       pControlData[l].dwStateSet &= ~TP_CONTROL_HIDE;
		}
	}
}
void CTPBtnToolBar::Show(UINT nID,BOOL bShow)
{
	CTPButton *pButton = (CTPButton *)GetDlgItem(nID);
	if(pButton &&pButton ->m_pControlData)
	{
		if(bShow && (pButton ->m_pControlData ->dwStateSet & TP_CONTROL_HIDE) || !bShow && !(pButton ->m_pControlData ->dwStateSet & TP_CONTROL_HIDE))
		{
			m_bReLayOut = TRUE;
			pButton ->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
			if(bShow) 
			{
				pButton ->m_pControlData ->dwStateSet &= ~TP_CONTROL_HIDE;
				pButton ->Invalidate();
			}
			else pButton ->m_pControlData ->dwStateSet |= TP_CONTROL_HIDE;
		}		
	}
}
void CTPBtnToolBar::ShowIndex(int iIndex,BOOL bShow)
{
	if(iIndex < 0 || iIndex >m_aTPButton.GetSize() ||  m_aTPButton[iIndex] == NULL) return ;
	if(m_aTPButton[iIndex] ->m_pControlData) Show(m_aTPButton[iIndex]->m_pControlData ->nID,bShow);
}
void CTPBtnToolBar::ReLayOut(BOOL bForce)
{
	if(!bForce &&  m_bReLayOut == FALSE) return ;
	m_bReLayOut = FALSE;
	SetSize(TRUE);
	Invalidate(TRUE);
}
void CTPBtnToolBar::OnMouseWheel(int iRot)
{
	int iTopX = m_iTopX;
	if(iRot<0)
	{
		CRect rtClient;
		GetClientRect(rtClient);
		m_iTopX -= max(m_iButtonSize[1],m_iButtonSize[0]);
		m_iTopX = max(m_iTopX, rtClient.Width()- m_iExLeft - m_iTotalWidth);
		m_iTopX = min(10,m_iTopX);
	}
	else
	{
		m_iTopX +=m_iButtonSize[0];
		m_iTopX = min(10,m_iTopX);
	}
	if(iTopX != m_iTopX) SetSize(m_iExLeft>0);
}
void CTPBtnToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(GetFocus() != this) SetFocus();
	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CTPBtnToolBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_bMouseWheel) OnMouseWheel(zDelta);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
void CTPBtnToolBar::SetLogo(CString sFileName)
{
	if(m_pLogo)    TP_ReleaseCommonRes(m_pLogo);
	m_pLogo       = TP_LoadBmpByte(sFileName);
}

void CTPBtnToolBar::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1000)
	{
		UINT uFlag  =  0;
		if(	GetKeyState(VK_CONTROL) <0) uFlag |= TP_CONTROL;
		if(	GetKeyState(VK_SHIFT) <0)   uFlag |= TP_SHIFT;
		if(	GetKeyState(VK_MENU) <0)    uFlag |= TP_MENU;		
		if(GetKeyState(m_uContinueKey)<0 && TP_CmpKeyState(m_uContinueFlag,m_uContinueState,uFlag))
		{
			if(m_pMsgWnd && ::IsWindow(m_pMsgWnd ->GetSafeHwnd()))
			{
				m_pMsgWnd ->PostMessage(WM_TOOLBARCLICK,m_uContinueID,uFlag);	
			}
		}
		else
		{
			if(m_hContinueTimer) KillTimer(m_hContinueTimer);
			m_hContinueTimer = NULL;
		}
	}
	else __super::OnTimer(nIDEvent);
}

void CTPBtnToolBar::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);
	if(m_uBarStyle != 3) return;
	if(pNewWnd  && pNewWnd ->GetParent() == this) return;	
	ShowWindow(SW_HIDE);	
	if(m_bPaletteShow && m_pActWnd && ::IsWindow(m_pActWnd ->GetSafeHwnd()) && GetActiveWindow() == this)
		m_pActWnd ->SetActiveWindow();
	m_bPaletteShow = FALSE;
}

void CTPBtnToolBar::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);
	if(WA_ACTIVE == nState)   m_pActWnd = pWndOther;
	else if(WA_INACTIVE == nState && m_bPaletteShow)//XN00028617
	{
		if(m_uState  & TPBTNTOOLSTATE_PALETTE)
		{
			ShowWindow(SW_HIDE);
			m_bPaletteShow = FALSE;
		}
	}

	// TODO: Add your message handler code here
}
void CTPBtnToolBar::SetColAndRow(int  iCol,int iRow)
{
	m_uControlCol  = iCol;
	m_uControlLine = iRow;
}
void  CTPBtnToolBar::EnableLink(TPControlData *pCtrlData,UINT uIndentify,BOOL bEnable,TP_BtnToolBar_Link eType)
{
	return;
	TP_BtnToolBarLink_Data *pLinkData = &g_aBtnToolLink[eType];
	BOOL                    bLink     = FALSE;
	UINT                    nID       = 0;
	pLinkData ->Enable(uIndentify,bEnable);
	for(INT l= (INT)pLinkData->aCtrlData.GetSize() -1;l>=0;l--)
	{
		if(pLinkData->aCtrlData[l] == pCtrlData) continue;
		if(pLinkData->aToolBar[l]->EnableLinkCallBack == NULL) continue;
		nID = pLinkData->aToolBar[l]->GetID(uIndentify);
		for(INT k= pLinkData->aCtrlSize[l] -1;k>=0;k--)
		{
			if(pLinkData->aCtrlData[l][k].nID != nID)    continue;			
			if(pLinkData->aToolBar[l]->EnableLinkCallBack(nID,bEnable,pLinkData->aToolBar[l]->m_pLinkCallBack)) bLink = TRUE;			
			if(bLink && pLinkData->aCtrlData[l][k].hWnd && TP_IsButon(pLinkData->aCtrlData[l][k].hWnd))
			{
				CTPButton * pButton = (CTPButton *)CWnd::FromHandle(pLinkData->aCtrlData[l][k].hWnd);
				if(pButton)	pButton ->EnableWindow(bEnable);
			}
		}
	}
}
void  CTPBtnToolBar::SetCheckLink(TPControlData *pCtrlData,UINT uIndentify,BOOL bCheck,TP_BtnToolBar_Link eType)
{
	return;
	TP_BtnToolBarLink_Data *pLinkData = &g_aBtnToolLink[eType];
	BOOL                    bLink     = FALSE;
	UINT                    nID       = 0;
	pLinkData ->SetCheck(uIndentify,bCheck);
	for(INT l= (INT)pLinkData->aCtrlData.GetSize() -1;l>=0;l--)
	{
		if(pLinkData->aCtrlData[l] == pCtrlData) continue;
		if(pLinkData->aToolBar[l]->SetCheckLinkCallBack == NULL) continue;
		nID = pLinkData->aToolBar[l]->GetID(uIndentify);
		for(INT k= pLinkData->aCtrlSize[l] -1;k>=0;k--)
		{
			if(pLinkData->aCtrlData[l][k].nID != nID)    continue;
			if(pLinkData->aToolBar[l]->SetCheckLinkCallBack(nID,bCheck,pLinkData->aToolBar[l]->m_pLinkCallBack)) bLink = TRUE;		
			if(bLink && pLinkData->aCtrlData[l][k].hWnd && TP_IsButon(pLinkData->aCtrlData[l][k].hWnd))
			{
				CTPButton * pButton = (CTPButton *)CWnd::FromHandle(pLinkData->aCtrlData[l][k].hWnd);
				if(pButton)	pButton ->SetCheck(bCheck);
			}
		}
	}
}
void  CTPBtnToolBar::SetMsgWnd(CWnd *pMsgWnd)
{
	m_pMsgWnd = pMsgWnd;
	if(m_pPaletteBar) m_pPaletteBar ->m_pMsgWnd = pMsgWnd;
}
BOOL CTPBtnToolBar::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg ->message == WM_SYSKEYDOWN  && pMsg->wParam == VK_F4) return TRUE;//时间线的Palette ToolBar不应被destroy XN00011957  jw 20080523
	else if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_F1 || TP_IsCtrlHotKeyHelp((UINT)pMsg->wParam))  && m_pParentWnd)
	{		
		HWND     hBaseWnd = TP_GetTopFrame(m_pParentWnd->GetSafeHwnd());		
		::PostMessage(hBaseWnd,WM_HELPEX,0,0);
		return TRUE;
	}
	else if(PreTranslateMessageEx && m_pPreTranslateMessageData)
	{
		if(PreTranslateMessageEx(pMsg,m_pPreTranslateMessageData)) return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

BOOL CTPBtnToolBar::OnEraseBkgnd(CDC* pDC)
{
	if(CTPBaseWnd::EraseBkgnd(pDC,this)) return TRUE;
	else
	{	
		CRect rtClient;
		GetClientRect(&rtClient);
		if(m_uBarStyle == 0)pDC ->FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));		
		 
		else if(m_uBarStyle == 2 && m_pBarBack)
		{
			if(m_pBarBack )
				TP_StretchDIBRect(pDC,m_pBarBack ->GetByte(),m_pBarBack ->Width(),m_pBarBack ->Height(),m_iBarBackEx[0],m_iBarBackEx[1],rtClient); 			
		}	
		else return TRUE;
//		pDC ->FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));		
		return TRUE;
		return __super::OnEraseBkgnd(pDC);
	}
}
 void  CTPBtnToolBar::SetGlobalWndControlData(TPNSWndControlDataArr* pDataArrSrc)
 {
	 g_lGlobalHotKey = NULL;
	 for(int l=0;l<pDataArrSrc->GetSize();l++)
	 {
		 if(lstrcmpi(pDataArrSrc->GetAt(l)->sGuiPluginName,_T("Global")) == 0)
		{
			g_pGlobalHotKey   = pDataArrSrc->GetAt(l)->pHotkeyData;
			g_lGlobalHotKey   = pDataArrSrc->GetAt(l)->lHotkeySize;
			break;
		}
	 }	 
 }
 BOOL CTPBtnToolBar::OnCommand(WPARAM wParam, LPARAM lParam)
 {
	 UINT nID = HIWORD(wParam);	
	 if(nID == EN_SETFOCUS || nID == EN_KILLFOCUS || CBN_SETFOCUS == nID || CBN_KILLFOCUS == nID)	
		 ::InvalidateRect((HWND)lParam,NULL,FALSE);
	 return __super::OnCommand(wParam, lParam);
 }
 CTPButton *CTPBtnToolBar::GetPressButton()
 {
	 for(INT l=0;l<m_aTPButton.GetSize();l++)
	 {
		 if(m_aTPButton[l] && m_aTPButton[l]->GetSafeHwnd() == m_hPressBtn)
			 return m_aTPButton[l];
	 }
	 return NULL;
  }

 BOOL CTPBtnToolBar::IsHotKey(UINT vKey,UINT uFlag,BOOL bSatte)
 {
	 TPHotKeyData  *pHotKey[4]  = {NULL,NULL,NULL,NULL};
	 DWORD          dwHotKey[4] = {0,0,0,0};
	 CTPBtnToolBar *pToolBarPal = GetPaletteBar();

	 if(uFlag == -1) uFlag = TP_GetKeyFlag(vKey);

	 pHotKey[0]   = m_pHotKeyData;
	 dwHotKey[0]  = m_dwHotKeyCount;
	 pHotKey[1]   = m_pHotKeyDataEx;
	 dwHotKey[1]  = m_dwHotKeyCountEx;
	 if(pToolBarPal)
	 {
		 pHotKey[2]  = pToolBarPal ->m_pHotKeyData;
		 dwHotKey[2] = pToolBarPal ->m_dwHotKeyCount;
		 pHotKey[3]  = pToolBarPal ->m_pHotKeyDataEx;
		 dwHotKey[3] = pToolBarPal ->m_dwHotKeyCountEx;
	 }
	 for(INT l=0;l<4;l++)
	 {
		 if(dwHotKey[l]<=0) continue;			 
		 for(DWORD dCtrl = 0;dCtrl<dwHotKey[l];dCtrl ++)
		 {				 				
			 if( bSatte &((pHotKey[l][dCtrl].uState & TP_CONTROL_NOSTATE)==TP_CONTROL_NOSTATE))
				 continue;
			 if(TP_CmpKeyState(pHotKey[l][dCtrl].uFlagSet,pHotKey[l][dCtrl].uState,uFlag) && pHotKey[l][dCtrl].vkCharSet == vKey)
				 return pHotKey[l][dCtrl].nID;
		 }			 
	 }
	 return FALSE;
 }
 BOOL CTPBtnToolBar::IsDefaultKey(UINT nID,UINT vKey,UINT uFlag)
 {
	 UINT uNoUse = TP_CONTROL_NOUSE;
	 if(!(uFlag & TP_STATE)) uNoUse |= TP_CONTROL_STATEKEY;
	 uFlag &= ~TP_STATE;

	if(!(m_pPaletteBar && m_pPaletteBar->m_bPaletteShow) || (m_uState & TPBTNTOOLSTATE_PALETTE)) //XN00019723 jw 20101103
	{	
		for(DWORD i=0;i<m_dwHotKeyCount;i++)
		{		
			if(m_pHotKeyData[i].uState & uNoUse) continue;
			if(m_pHotKeyData[i].nID == nID)
			{
				if(TP_CmpKeyState(m_pHotKeyData[i].uFlagSet,m_pHotKeyData[i].uState,uFlag) && m_pHotKeyData[i].vkCharSet == vKey)
					return TRUE;
			}
		}		
		if(m_pHotKeyDataEx && m_dwHotKeyCountEx>0)
		{
			for(DWORD i=0;i<m_dwHotKeyCountEx;i++)
			{		
				if(m_pHotKeyDataEx[i].uState & uNoUse) continue;
				if((m_pHotKeyDataEx[i].uState & TP_CONTROL_NOAPPLY) && !(m_uBaseState & TP_BASESTATE_APPLY)) continue;
				if(m_pHotKeyDataEx[i].nID == nID)
				{
					if(TP_CmpKeyState(m_pHotKeyDataEx[i].uFlagSet,m_pHotKeyDataEx[i].uState,uFlag) && m_pHotKeyDataEx[i].vkCharSet == vKey)
						return TRUE;
				}
			}
		}
		return FALSE;
	}
	else if(m_pPaletteBar)
		return m_pPaletteBar->IsDefaultKey(nID, vKey, uFlag);
	else
		return FALSE;
 }

 TPBtnToolBarSpecialCheck::TPBtnToolBarSpecialCheck(CTPBtnToolBar *pToolBar,DWORD dwCheck)
 {
	 TPHotKeyData  *pHotKey[4]  = {NULL,NULL,NULL,NULL};
	 DWORD          dwHotKey[4] = {0,0,0,0};
	 CTPBtnToolBar *pToolBarPal = pToolBar ->GetPaletteBar();
	 DWORD          dCheck[3]   = {TP_CTRL,TP_ALT,TP_SHIFT}; 
	 BOOL           bFind       = FALSE; 

	 m_dwCheckKey = 0;
	 pHotKey[0]   = pToolBar ->m_pHotKeyData;
	 dwHotKey[0]  = pToolBar ->m_dwHotKeyCount;
	 pHotKey[1]   = pToolBar ->m_pHotKeyDataEx;
	 dwHotKey[1]  = pToolBar ->m_dwHotKeyCountEx;
	 if(pToolBarPal)
	 {
		 pHotKey[2]  = pToolBarPal ->m_pHotKeyData;
		 dwHotKey[2] = pToolBarPal ->m_dwHotKeyCount;
		 pHotKey[3]  = pToolBarPal ->m_pHotKeyDataEx;
		 dwHotKey[3] = pToolBarPal ->m_dwHotKeyCountEx;
	 }
	 for(INT l=0;l<4;l++)
	 {
		 if(dwHotKey[l]<=0) continue;
		 for(INT m=0;m<3;m++)
		 {
			 if(!(dwCheck & dCheck[m])) continue;
			 bFind = FALSE;
			 for(DWORD dCtrl = 0;dCtrl<dwHotKey[l];dCtrl ++)
			 {				 				
				 if(pHotKey[l][dCtrl].uFlagSet & dCheck[m])
				 {
					 if(!(pHotKey[l][dCtrl].uState & TP_CONTROL_DRAG)) continue;
					 if(GetKeyState(pHotKey[l][dCtrl].vkCharSet)<0) 
					 {
						 bFind = TRUE;
						 break;
					 }
				 }
			 }
			 if(bFind) m_dwCheckKey |= dCheck[m];
		 }
	 }
	 TP_SetSpecialKey(m_dwCheckKey,0x7FFFFFFF);
 }
 TPBtnToolBarSpecialCheck::~TPBtnToolBarSpecialCheck()
 {
	 TP_SetSpecialKey(0,m_dwCheckKey);
 }
 LRESULT  CTPBtnToolBar::FunTest(TPControlData* pCtrl)
 {
	 return S_OK;
 }
