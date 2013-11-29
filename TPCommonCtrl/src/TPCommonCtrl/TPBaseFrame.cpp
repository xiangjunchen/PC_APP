#include "StdAfx.h"
#include ".\tpBaseFrame.h"
#define TIMEREVENT_CHECKACTIVE 1124



enum IMAGE_INDEX
{
	IMAGE_NOR,
	IMAGE_DOWN,
	IMAGE_OVER,
	IMAGE_DIS,
	IMAGE_OVER_N,
};




#define HTNAIL         100 
#define DOCK_DISTANCE  10


struct TPMenuWndData;



#define DOCK_0     0x0000
#define DOCK_1     0x00001
#define DOCK_2     0x00002
#define DOCK_3     0x00004
#define DOCK_4     0x00008



extern HMENU           g_hMenuPopUp; 
extern TPMenuWndData  *g_pMenuWndData;
extern BOOL            g_bMenuExPopUp; 
extern int             g_iDoModal ;

namespace _tp_commonctrl
{
extern CWnd           *g_pMainFrameWnd;
}

// CTPTLDropTa
CRect             CTPBaseFrame::m_rtScreen  = CRect(0,0,0,0);
CRect             CTPBaseFrame::m_rtEdge    = CRect(0,0,0,0);

ULONGLONG         CTPBaseFrame::m_uBaseFlag = 0;
CTPBaseFrameArray CTPBaseFrame::m_aBaseFrame;
void             (*CTPBaseFrame::m_pBaseFrameCmd)(TPFrameState nCmd,CTPBaseFrame *pFrame) = NULL;	
void             (*CTPBaseFrame::m_pBaseFrameAct)(CTPBaseFrame *pFrame,BOOL bAct) = NULL;
LRESULT          (*CTPBaseFrame::m_pBaseFrameGetPos)(LPCTSTR sGuiName,LPCTSTR  sWndType,int iFrameIndex,CRect &rtGet,void *pState) = NULL; //TPBaseFrameState
LRESULT          (*CTPBaseFrame::m_pBaseFrameAppKeyDown)(UINT nChar,UINT nFlag) = NULL;
BOOL             (*CTPBaseFrame::m_pCanActWnd)(HWND  hWnd) = NULL;
BOOL             (*CTPBaseFrame::m_pGetActWnd)(HWND &hWnd,CTPBaseFrame *pThis) = NULL;
BOOL             (*CTPBaseFrame::m_pUserConfigrationCmd)(LPCTSTR sGuiName,LPCTSTR  sWndType,UINT uCmd,LPARAM lParam) = NULL;
void            *(*CTPBaseFrame::m_pMainFrameCmd)(UINT uCmd,LPARAM lParam) = NULL;
LRESULT          (*CTPBaseFrame::m_pTMCmd)(UINT uCmd,LPARAM lParam) = NULL;


int               CTPBaseFrame::m_iRefCount = 0;
CTPBaseFrameTips *CTPBaseFrame::m_pToolTip  = NULL;
BOOL              CTPBaseFrame::m_bToolTips = 0;
CRect             CTPBaseFrame::m_rtMenuBox = CRect(0,0,0,0);
BOOL              CTPBaseFrame::m_bFixWnd   = FALSE;
HWND              CTPBaseFrame::m_hActMsgWnd = NULL;
CWnd             *CTPBaseFrame::m_pActMsgWnd = NULL;

HWND                 g_hBaseWndBack          = NULL;
CArray<HWND,HWND&>   g_aLockWnd;

namespace _tp_commonctrl
{
LPTPNSControlData  g_pGlobalHotKey          = NULL;
long               g_lGlobalHotKey          = 0;
long               g_lGlobalHotKeyHelp      = 0;
}

CTPBaseFrame     *g_pActFrame       = NULL; 
CTPBaseFrame     *g_pDisActFrame    = NULL; 



CTPBaseFrame::CTPBaseFrame(void)
{
	m_dwSafeFlag   = rand();
	m_dwSafeFlagEx = m_dwSafeFlag + 1;
	m_bSubWindow   = FALSE;
	m_bCreate      = FALSE;
	m_uStyle       = 0; 
	m_uStyleEx     = 0; 
	m_uHitTest     = 0;
	m_uDownHitTest = 0;

	m_bAttachTaskBar = FALSE;
	m_uFrameLevel    = 2;
	m_uFrameType     = FRAME_OTHER;//类型
	m_iFrameIndex    = 0;
	m_lAttachType    = FRAME_ATTACH_NULL;
	m_pMonitorInfo   = NULL;

	m_pfnBaseSuper	= NULL;
	m_hBaseWnd		= NULL;

	for(int l=0;l<FRAME_MAX;l++)
		m_pResources[l] = NULL;

	m_uBoxAmount    = 0;
	m_uBoxAmountEx  = 0;
	m_iNailState    = 0;
	m_uState        = 0;
	m_hRgnWnd       = NULL;
	m_pAttachImage  = NULL;
	m_iAttachImage  = 0;
	m_hAttachWnd    = NULL;

	m_rtIconBox    = CRect(0,0,0,0);
	m_rtCloseBox      = CRect(0,0,0,0);
	m_rtMaxBox        = CRect(0,0,0,0);
	m_rtMinBox        = CRect(0,0,0,0);	
	m_rtHelpBox       = CRect(0,0,0,0);
	m_rtNailBox       = CRect(0,0,0,0);
	m_rtText          = CRect(0,0,0,0);
	m_sText           = _L("");
	m_hIcon           = NULL;

	m_iScaleWidth     = 0; 
	m_iScaleHeight    = 0;
	m_rtSrcScale      = CRect(0,0,0,0); 

	m_iWndWidth       = 0;
	m_iWndHeight      = 0;

    m_lPreHotkeySize  = 0;
	m_iBackColor      = COLOR_BASE_FRAME2;
	m_pCtrlSize       = NULL;
	m_hActWnd         = NULL;
	m_hActPreFrame    = NULL;



	m_hDockParentWnd   = NULL; 
	m_dwDockState      = 0;
	m_bChild           = FALSE;
	m_bShow            = TRUE;  
	m_bBaseWnd         = FALSE;
	m_pBaseFrame       = NULL;
	m_pMenuSys         = NULL;
	m_bVistaFillWindow = TRUE;

	m_rtMoving        = CRect(0,0,0,0);	
	m_rtBeforeMov     = CRect(0,0,0,0);
	m_rtSize          = CRect(0,0,0,0);
	m_uFrameStyle     = 0;
	m_pUnActFrame     = NULL;
	m_hUnActFrame     = NULL;
	m_stuHelpInfo.cbSize = sizeof(HELPINFO);

	m_iRefCount ++;
	if(m_iRefCount == 1)
	{
		m_pToolTip = new CTPBaseFrameTips();
		m_pToolTip->Create(AfxGetMainWnd());
	}

	m_bNoMsg          = FALSE;
	m_uRoundStyle	  = 0;
}

CTPBaseFrame::~CTPBaseFrame(void)
{	
	m_dwSafeFlag   = 0;
	for(int l=0;l<FRAME_MAX;l++)
	{
		TP_ReleaseCommonRes(m_pResources[l]);
		m_pResources[l] = NULL;
	}
	if(m_hRgnWnd) 	DeleteObject(m_hRgnWnd);
	if(m_pCtrlSize)	delete m_pCtrlSize;
	m_pCtrlSize = NULL;
	if(m_pMenuSys) delete m_pMenuSys;
	m_pMenuSys = NULL;

	m_aDockWnd.RemoveAll();

	for(INT l= (int)m_aBaseFrame.GetSize()-1;l>=0;l--)
	{
		if(m_aBaseFrame[l] == this)	m_aBaseFrame.RemoveAt(l);		
	}

	m_iRefCount --;
	if(m_iRefCount == 0)
	{
		m_pToolTip->DestroyWindow();
		delete m_pToolTip;
		m_pToolTip = NULL;
	}
	if(g_pActFrame == this)    g_pActFrame =NULL;
	if(g_pDisActFrame == this) g_pDisActFrame =NULL;

	m_aForceOrder.RemoveAll();


	m_lPreHotkeySize  = 0;  
	m_hPreHotkeyWnd   = NULL;

}
void CTPBaseFrame::SubWindow(BOOL bSub)
{
	if(m_bNoMsg) return;
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	
		m_uStyle   = GetWindowLong(m_hBaseWnd,GWL_STYLE );
		m_uStyleEx = GetWindowLong(m_hBaseWnd,GWL_EXSTYLE );

		m_bChild   = !BOOL(m_uStyle & WS_POPUP);


		m_uBoxAmount  |= BOX_CLOSE;
		if((m_uStyle & WS_SIZEBOX) == WS_SIZEBOX)                    m_uBoxAmount |= BOX_SIZE;  //     2006-7-12 第一次修改界脕E
#ifdef TP_NS_EDIT
		SetWindowLong(m_hBaseWnd,GWL_STYLE,m_uStyle|WS_SYSMENU);
		if((m_uStyle & WS_SYSMENU) == WS_SYSMENU)                    m_uBoxAmount |= BOX_SYSMENU;
		if((m_uStyle & WS_MINIMIZEBOX) == WS_MINIMIZEBOX)            m_uBoxAmount |= BOX_MIN;
		if((m_uStyle & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX)            m_uBoxAmount |= BOX_MAX;
#endif
		if((m_uStyle & WS_CAPTION) == WS_CAPTION)                    m_uBoxAmount |= BOX_CAPTION;
		if((m_uStyle & WS_DLGFRAME)||(m_uStyle & WS_BORDER)||(m_uStyle & WS_THICKFRAME)) 
			m_uBoxAmount |= BOX_BORDER;	
	

		SetClassLongPtr(m_hBaseWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);
		CWnd::FromHandle(m_hBaseWnd) ->SetFont(TP_GetSysFont(FONT_BASE_TEXT),FALSE);			

		SetSkin(g_sSysSkinPath + _L("\\CommonCtrl\\Frame"));	

		FunSize();
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;			
	}
}
CTPBaseFrame  * CTPBaseFrame::GetActiveFrame()
{
	CWnd *pWnd     = CWnd::FromHandle(GetActiveWindow());
	if(pWnd == NULL) return NULL;
	else             return CTPBaseFrame::GetBaseFrame(pWnd);	
}
CString  CTPBaseFrame::GetActiveFrameCaption()
{
	CTPBaseFrame *pBaseFrame = GetActiveFrame();
	if(pBaseFrame) return pBaseFrame ->m_sText;
	else return _T("");
}
void  CTPBaseFrame::ShowWnd(CWnd *pFrame)
{
	if(pFrame == NULL || pFrame ->IsWindowVisible()) return;
	pFrame ->ShowWindow(SW_SHOW);	
	return;

	if(TP_GetIsVista())
	{
		CTPBaseFrame::m_uBaseFlag |= (STATE_NOACTWND|TP_BASESTATE_NOACTWND);
		pFrame ->SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);
		pFrame ->ShowWindow(SW_SHOW);
		CTPBaseFrame::m_uBaseFlag &= ~(STATE_NOACTWND|TP_BASESTATE_NOACTWND);
		pFrame -> SendMessage(WM_SYSCOMMAND, SC_RESTORE,0);
	}
	else 
	{					
		pFrame ->ShowWindow(SW_SHOW);	
	}
}
CTPBaseFrame  * CTPBaseFrame::GetBaseFrame(CWnd *pWnd,int iType )
{
	if(pWnd == NULL||!::IsWindow(pWnd->GetSafeHwnd())) return NULL;

	LRESULT hResult = pWnd ->SendMessage(WM_BASEFRAME,0,iType);
	if(-1 <= hResult && hResult < 2)   return NULL; //返回的指諄E赃位可能为1 jw 20080728
	else             return (CTPBaseFrame  *)hResult;		 
}
BOOL TP_CursorInMenu(CTPBaseFrame *pFrame,CRect rtSysMenuBox)
{
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	CRect  rtWnd;
	::GetWindowRect(pFrame->m_hBaseWnd,&rtWnd);
	rtWnd.OffsetRect(rtSysMenuBox.left,rtSysMenuBox.top);
	rtWnd.right  = rtWnd.left + rtSysMenuBox.Width();
	rtWnd.bottom = rtWnd.top  + rtSysMenuBox.Height();
	return rtWnd.PtInRect(ptCursor);
}
void TP_DwmEnableBlurBehindWindow(HWND hWnd,BOOL bSet)
{
	DWM_BLURBEHIND bb = {0};   
	bb.dwFlags = DWM_BB_ENABLE;   
	bb.fEnable = bSet;   
	bb.hRgnBlur = NULL;   
	DwmEnableBlurBehindWindow(hWnd, &bb);  
}
void TP_DwmDisableWnd(HWND hWnd)
{
	DWMNCRENDERINGPOLICY ncrp = DWMNCRP_DISABLED;
	HRESULT lResult = DwmSetWindowAttribute(hWnd,DWMWA_NCRENDERING_POLICY,&ncrp,sizeof(ncrp));

	DWM_BLURBEHIND bb = {0};   
	bb.dwFlags  = DWM_BB_ENABLE;   
	bb.fEnable  = FALSE;   
	bb.hRgnBlur = NULL;   
	lResult = DwmEnableBlurBehindWindow(hWnd, &bb);  

}
BOOL TP_GetIsEdge(UINT uHitTest)
{
	if(uHitTest == HTTOPLEFT || uHitTest == HTBOTTOMRIGHT ||
	   uHitTest == HTTOPRIGHT || uHitTest == HTBOTTOMLEFT ||
	   uHitTest == HTLEFT || uHitTest == HTRIGHT          ||
	   uHitTest == HTTOP || uHitTest == HTBOTTOM) return TRUE;
	else return FALSE;
}
BOOL CTPBaseFrame::FunVistaCmd(int iCmd,LPARAM lParam)
{
	if(!TP_GetIsVista()) return FALSE;
	if(iCmd == 0 && !m_bChild)
	{
		if(!(m_uState & STATE_FRAMESET))
		{	
			m_uState |= STATE_FRAMESET;

			/*DWMNCRENDERINGPOLICY ncrp = DWMNCRP_DISABLED;
			HRESULT lResult = DwmSetWindowAttribute(m_hBaseWnd,DWMWA_NCRENDERING_POLICY,&ncrp,sizeof(ncrp));

			BOOL bSet  = FALSE;
			lResult = DwmSetWindowAttribute(m_hBaseWnd,DWMWA_TRANSITIONS_FORCEDISABLED,&bSet,sizeof(bSet));

			lResult = DwmSetWindowAttribute(m_hBaseWnd,DWMWA_ALLOW_NCPAINT,&bSet,sizeof(bSet));
			 
			DWMFLIP3DWINDOWPOLICY eSet = DWMFLIP3D_EXCLUDEBELOW;
			lResult = DwmSetWindowAttribute(m_hBaseWnd,DWMWA_FLIP3D_POLICY,&eSet,sizeof(eSet));


			DWM_BLURBEHIND bb = {0};   
			bb.dwFlags  = DWM_BB_ENABLE;   
			bb.fEnable  = FALSE;   
			bb.hRgnBlur = NULL;   
			lResult = DwmEnableBlurBehindWindow(m_hBaseWnd, &bb);  */


			m_uStyle  = GetWindowLong(m_hBaseWnd,GWL_STYLE);
			if(m_uStyle & WS_THICKFRAME)
			{		
				m_uState |= STATE_FRAME;
				m_uState &= ~STATE_FRAMESET;
				//CWnd::ModifyStyle(m_hBaseWnd, WS_THICKFRAME, 0, 0);
				//m_uStyle  =  GetWindowLong(m_hBaseWnd,GWL_STYLE);
			}
		}
		return TRUE;
	}
	else if(iCmd == 1)
	{
		if(m_bChild) return FALSE;
		if(!(m_uState & STATE_FRAME)) return FALSE;
		if(::IsZoomed(m_hBaseWnd))    return FALSE;
		if(TP_GetIsEdge(m_uHitTest))
		{		
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			FunNcHitTest(ptCursor);
		}

		if(m_uHitTest == HTTOPLEFT || m_uHitTest == HTBOTTOMRIGHT)
		{
			SetCursor(AfxGetApp()-> LoadStandardCursor(IDC_SIZENWSE));
			return TRUE;
		}
		else if(m_uHitTest == HTTOPRIGHT || m_uHitTest == HTBOTTOMLEFT)
		{
			SetCursor(AfxGetApp()-> LoadStandardCursor(IDC_SIZENESW));
			return TRUE;
		}
		else if(m_uHitTest == HTLEFT || m_uHitTest == HTRIGHT)
		{
			SetCursor(AfxGetApp()-> LoadStandardCursor(IDC_SIZEWE));
			return TRUE;
		}
		else if(m_uHitTest == HTTOP || m_uHitTest == HTBOTTOM)
		{
			SetCursor(AfxGetApp()-> LoadStandardCursor(IDC_SIZENS));
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(iCmd == 2 && !m_bChild)
	{
		if(TP_GetIsEdge(m_uHitTest))
		{		
			if((m_uState & STATE_FRAME) && !(m_uStyle & WS_THICKFRAME))
			{
				LockWindowUpdate(m_hBaseWnd);
				CWnd::ModifyStyle(m_hBaseWnd,0 , WS_THICKFRAME, 0);			
				LockWindowUpdate(NULL);
				m_uStyle   =  GetWindowLong(m_hBaseWnd,GWL_STYLE);
				m_uState  &= ~STATE_FRAMESET;
			}
		}
	}
	else if(iCmd == 3 && !m_bChild)
	{
		if(!(m_uState & STATE_FRAMESET))
		{		
			m_uState |= STATE_FRAMESET;
			if((m_uState & STATE_FRAME) && (m_uStyle & WS_THICKFRAME))
			{
				LockWindowUpdate(m_hBaseWnd);
				CWnd::ModifyStyle(m_hBaseWnd,WS_THICKFRAME,0,0);			
				LockWindowUpdate(NULL);
				m_uStyle   =  GetWindowLong(m_hBaseWnd,GWL_STYLE);			
			}
		}
	}
	else if(iCmd == 4)
	{
		if(TP_GetDWMStatus())
		{	
			CWnd *pWnd = CWnd::FromHandle(m_hBaseWnd); 
			CDC  *pDC  = lParam == NULL?pWnd ->GetDC():(CDC *)lParam;	
			CRect rtWnd;
			pWnd ->GetClientRect(&rtWnd);
			pDC->FillSolidRect(rtWnd, TP_GetSysColor(COLOR_BASE_FRAME));
			if(lParam == NULL) pWnd ->ReleaseDC(pDC);
		}
	}

	return TRUE;
}
//::SetWindowLong()
LRESULT CTPBaseFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{ 
	if(m_bNoMsg)  return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);

	ASSERT(m_dwSafeFlag == m_dwSafeFlagEx -1);	
	switch ( message )
	{
	case WM_STYLECHANGED:
		if(m_uFrameStyle & FRAME_STYLE_NOTOPMOST)  
		{
			if((GWL_EXSTYLE & wParam) == GWL_EXSTYLE)
			{
				LPSTYLESTRUCT pStyle = (LPSTYLESTRUCT)lParam;
				if(pStyle ->styleNew & WS_EX_TOPMOST) 
				{
					AfxMessageBox(_T("Error Wnd Type !"));				 
				}
			}
		}
		return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
		break;
 	case WM_NCMOUSEMOVE:
		{
			if(m_pToolTip && m_uDownHitTest == 0 && m_bToolTips == 0 && (m_uBoxAmount & BOX_SYSMENU) && TP_CursorInMenu(this,m_rtSysMenuBox))
			{
				m_bToolTips = 1;
				SetTimer(m_hBaseWnd,1123,1500,NULL);
				m_pToolTip ->m_sTips = _T("Main Menu");
			}
			else if((m_uBoxAmount & BOX_SYSMENU) && !TP_CursorInMenu(this,m_rtSysMenuBox) && m_pToolTip && m_bToolTips)
			{
				KillTimer(m_hBaseWnd,1123);				
				m_bToolTips = 0;
				m_pToolTip ->ShowWindow(SW_HIDE);
			}	
			else return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);		
		}
		break;
	case WM_TIMER:
		{
			if(wParam == 1123)
			{
				if(TP_CursorInMenu(this,m_rtSysMenuBox)&&m_pToolTip)
				{
					if(m_bToolTips == 1)
					{
						CSize szText = m_pToolTip->GetRect();
						CPoint ptCursor;
						GetCursorPos(&ptCursor);
						::SetWindowPos(m_pToolTip ->GetSafeHwnd(),HWND_TOPMOST,ptCursor.x,ptCursor.y + 20,szText.cx,szText.cy,SWP_SHOWWINDOW|SWP_NOACTIVATE); 						
						m_bToolTips =1;
						KillTimer(m_hBaseWnd,1123);		
						SetTimer(m_hBaseWnd,1123,200,NULL);
					}
					else if(m_bToolTips == 2)
					{
					}
				}
				else
				{
					KillTimer(m_hBaseWnd,1123);				
					m_bToolTips = 0;
					m_pToolTip ->ShowWindow(SW_HIDE);
				}				
				return S_OK;
			}			
			//{2011-01-21:zcd:add
			//Windows有BUG，操作任务栏切换到其他程序后，再切换回来，没有WM_ACTIVATE
#ifdef TP_NS_NET
			else if (wParam == TIMEREVENT_CHECKACTIVE)
			{
				KillTimer(m_hBaseWnd, TIMEREVENT_CHECKACTIVE);
				if (m_uState & STATE_CHECKINGACTIVE)
				{
					m_uState &= ~STATE_CHECKINGACTIVE;
					MSG msg;
					if(::GetActiveWindow() == m_hBaseWnd &&
						!::PeekMessage(&msg, m_hBaseWnd, WM_ACTIVATE, WM_ACTIVATE, PM_NOREMOVE))
					{
						::SendMessage(m_hBaseWnd, WM_ACTIVATE, WA_ACTIVE, 0);
					}
				}
				return S_OK;
			}
#endif//TP_NS_NET
			//}
			return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
		}
		break;
	case WM_CLOSE:
		SetPreActWnd();
		m_uFrameStyle |= FRAME_STYLE_NOACTPRE;
		return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
	case WM_DOMODAL:
		if(wParam)
		{
			m_uBoxAmountEx |=  m_uBoxAmount &(BOX_MIN|BOX_MAX);
			m_uBoxAmount   &= ~(BOX_MIN|BOX_MAX);
			m_uBoxAmount   |=   BOX_MODAL;			 
			PaintFrame();
		}
		else
		{
			m_uBoxAmount     |=  m_uBoxAmountEx &(BOX_MIN|BOX_MAX);
			m_uBoxAmount     &=  ~BOX_MODAL;
			m_uBoxAmountEx   &= ~(BOX_MIN|BOX_MAX);
			FunSize();
			PaintFrame();
		}

		break;
	case WM_BASEFRAME:
		{
			if(wParam == 10000) return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
			else
			{	
				if(m_hBaseWnd == NULL && wParam != NULL) m_hBaseWnd = (HWND)wParam;
				return (LRESULT)this;
			}
		}
		break;
	case WM_NCCREATE:
		SubWindow(TRUE);		
		return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
	case WM_CREATE:
		{
			SaveFont(m_hBaseWnd);
			m_bCreate = TRUE;
			SubWindow(TRUE);		
			CString sTemp; 
			CWnd::FromHandle(m_hBaseWnd) ->GetWindowText(sTemp);			
			CTPBaseFrame *pFrame = this;
			INT l = 0;
			for(l=0;l<m_aBaseFrame.GetSize();l++)
				if(m_aBaseFrame[l] == this) break;
			if(l>=m_aBaseFrame.GetSize() && !(m_uFrameStyle & FRAME_STYLE_NOGLOBLE))
				m_aBaseFrame.Add(pFrame);

			if(m_pBaseFrameCmd) m_pBaseFrameCmd(TPFrameState_Create,this);			
			GetWindowRect(m_hBaseWnd,m_rtSize);

			m_stuHelpInfo.hItemHandle  = m_hBaseWnd;
			m_stuHelpInfo.iCtrlId      = GetDlgCtrlID(m_hBaseWnd);			
			SetClassLongPtr(m_hBaseWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);		

			if(TP_GetIsVista())		FunVistaCmd(0);	
			m_bVistaFillWindow = TRUE;

			return  m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);			

		}	
	case WM_INITDIALOG:
		{
			SaveFont(m_hBaseWnd);
		//	CRect rtWnd;
		//	::GetWindowRect(m_hBaseWnd,&rtWnd);
		//	::SetWindowPos(m_hBaseWnd,NULL,0,0,rtWnd.Width(),rtWnd.Height()-5,SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
			CTPBaseFrame *pFrame = this;

			INT l=0;
			for(l=0;l<m_aBaseFrame.GetSize();l++)
				if(m_aBaseFrame[l] == this) break;
			if(l>=m_aBaseFrame.GetSize() && !(m_uFrameStyle & FRAME_STYLE_NOGLOBLE))
				m_aBaseFrame.Add(pFrame);

			if(m_pBaseFrameCmd) m_pBaseFrameCmd(TPFrameState_Create,this);			
			GetWindowRect(m_hBaseWnd,m_rtSize);
			m_stuHelpInfo.hItemHandle  = m_hBaseWnd;
			m_stuHelpInfo.iCtrlId      = GetDlgCtrlID(m_hBaseWnd);			
			SetClassLongPtr(m_hBaseWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);

			if(TP_GetIsVista())	FunVistaCmd(0);	
			m_bVistaFillWindow = TRUE;
			return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);			
		}
	case WM_ENABLE:
		if((BOOL)wParam == TRUE) 	m_uState &= ~STATE_DISABLE;			
 		else                        m_uState |= STATE_DISABLE;
		break;
	case WM_DESTROY:		
		RestoreFont(m_hBaseWnd);
		for(INT l= (int)m_aBaseFrame.GetSize()-1;l>=0;l--)
		{
			if(m_aBaseFrame[l] == this)	m_aBaseFrame.RemoveAt(l);		
		}
		KillTimer(m_hBaseWnd,1123);
		if(m_uFrameType != FRAME_TRIM)     
		{		
			SetPreActWnd();		
			if(m_pBaseFrameCmd) m_pBaseFrameCmd(TPFrameState_Destroy,this);	
		}
		m_uFrameStyle |= FRAME_STYLE_NOACTPRE;
		return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
	case WM_PAINT:
		{
			PAINTSTRUCT paint;
			BeginPaint(m_hBaseWnd,&paint);
			::SetBkColor(paint.hdc, TP_GetSysColor(COLOR_BASE_FRAME));
			::ExtTextOut(paint.hdc, 0, 0, ETO_OPAQUE,&paint.rcPaint, NULL, 0, NULL);
			EndPaint(m_hBaseWnd,&paint);
			return S_OK;
		}
		break;
	case WM_SIZING:
		{
			GetWindowRect(m_hBaseWnd,m_rtSize);
			LRESULT lResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
			if(m_iScaleHeight >0)	FunSizing((UINT)wParam,(LPRECT)lParam);	
			if(!TP_IsAltKeyDown() &&(m_uState & STATE_LBUTTONDOWN))  OnSizingEx((UINT)wParam,(CRect *)lParam);////7168
			return lResult;
		}		
	case WM_SIZE:
		{			
			m_bVistaFillWindow = TRUE;
			if(m_pCtrlSize)	m_pCtrlSize ->OnSize();			
			PaintFrame();			
			if(TP_GetIsVista()) LockWindowUpdate(m_hBaseWnd);
			LRESULT lResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);			
			if(TP_GetIsVista()) LockWindowUpdate(NULL);
			if(m_hRgnWnd)	SetWindowRgn(m_hBaseWnd,m_hRgnWnd,TRUE);
			GetWindowRect(m_hBaseWnd,m_rtSize);
			m_rtMoving     = CRect(0,0,0,0);			
			m_rtBeforeMov  = CRect(0,0,0,0);
#ifdef TP_NS_LOG
			if(m_uState & STATE_LOGOTEXT)
			{			
				CString sText;sText.Format(_T("Window Size %d %d %d %d"),m_rtSize.left,m_rtSize.top,m_rtSize.right,m_rtSize.bottom);
				//TP_LogEditNotifyCtrl(0,sText,m_sText,EDIT_LOGGER_MODULE,(CString)__FUNCTION__);					
			}
#endif
			return lResult;
		}		
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
		return S_OK;
	case WM_SYNCPAINT:
		PaintFrame();	
		m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
		return S_OK;
	case WM_NCPAINT:			
		PaintFrame();		
		return S_OK;
	case WM_SETCURSOR:
		if(TP_GetIsVista())
		{
			if(FunVistaCmd(1)) return TRUE;
		}				
		return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
	case WM_NCCALCSIZE:
		{
			if (m_bSubWindow && !m_bChild && (!TP_GetIsVista() || wParam == TRUE))
			{			
				NCCALCSIZE_PARAMS *lpncsp = (NCCALCSIZE_PARAMS *)lParam;
				CRect rtClient = lpncsp->rgrc[0];
				rtClient.left   = rtClient.left   + m_pResources[BORDER_LEFT_N] ->Width();
				rtClient.right  = rtClient.right  - m_pResources[BORDER_RIGHT_N] ->Width();
				rtClient.top    = rtClient.top    + m_pResources[BORDER_TOP_N] ->Height();// + ((m_uBoxAmount & CAPTION_BAR ) ? m_pResources[BORDER_BAR_N] ->Height() : 0);
				rtClient.bottom = rtClient.bottom - m_pResources[BORDER_BOTTOM_N] ->Height();
				lpncsp->rgrc[0] = rtClient;
				return S_OK;			
			}
			else
			{
				return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
			}	
		}
		break;
	case WM_GETMINMAXINFO:
		{	
			m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
			MINMAXINFO *pMinMaxInfo = (MINMAXINFO *)lParam;
			if(pMinMaxInfo ->ptMaxTrackSize.x  * pMinMaxInfo ->ptMaxTrackSize.y<=0 || pMinMaxInfo ->ptReserved.x == 0x12345) return S_OK;
			if(m_bSubWindow && !m_bChild )
			{			
				pMinMaxInfo ->ptMinTrackSize = CPoint(m_pResources[BORDER_LEFT_N] ->Width(),m_pResources[BORDER_BOTTOM_N] ->Height());
				pMinMaxInfo ->ptMaxTrackSize.x *= 2;
			}
			pMinMaxInfo ->ptMinTrackSize.x = max(pMinMaxInfo ->ptMinTrackSize.x,20);
			pMinMaxInfo ->ptMinTrackSize.y = max(pMinMaxInfo ->ptMinTrackSize.y,20);
			if(!(m_uFrameStyle & FRAME_STYLE_NOCALCSIZE))
			{
				if((m_uState & STATE_MAXABLE) && m_iScaleHeight!= 0 && m_iScaleWidth !=0)
				{			
					CRect rtTemp = CRect(0,0,::GetSystemMetrics(SM_CXSCREEN),::GetSystemMetrics(SM_CYSCREEN));
					FunSizing(rtTemp.Width()/rtTemp.Height() > m_iScaleWidth/m_iScaleHeight ?WMSZ_BOTTOM:WMSZ_RIGHT,rtTemp);
					pMinMaxInfo ->ptMaxTrackSize.x = rtTemp.Width();
					pMinMaxInfo ->ptMaxTrackSize.y = rtTemp.Height();
				}	
				pMinMaxInfo ->ptMaxTrackSize.x = min(pMinMaxInfo ->ptMaxTrackSize.x,2*1920);
				pMinMaxInfo ->ptMaxTrackSize.y = min(pMinMaxInfo ->ptMaxTrackSize.y,2*1440);
				pMinMaxInfo ->ptMaxTrackSize.x = min(8*1024*1024/(pMinMaxInfo ->ptMaxTrackSize.y*4),pMinMaxInfo ->ptMaxTrackSize.x);//XN00024140
			}
		}
		return S_OK;
	case WM_SETICON:
		if(wParam == ICON_SMALL)
		{
			m_hIcon = (HICON)lParam;
			if(m_hIcon) m_uBoxAmount |= BOX_ICON;
			else        m_uBoxAmount &= ~BOX_ICON;
			FunSize();			
			PaintFrame();
		}
		return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
	case WM_GETTEXT:
		{			
			if(m_sText.GetLength()<=0) 
			{
				m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);				
				m_sText = ( TCHAR *)lParam;
			}
			else lstrcpyn(( TCHAR *)lParam,m_sText,(int)wParam);
		}
		return m_sText.GetLength();
	case WM_SETTEXT:
		{		
			m_sText  = ( TCHAR *)lParam;	
			PaintFrame();
			if(TP_GetIsVista()) LockWindowUpdate(m_hBaseWnd);			
			LRESULT lResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);		
			if(TP_GetIsVista()) LockWindowUpdate(NULL);
			return TRUE;
		}	
	case WM_ACTIVATE:
		{
			//{2011-01-25:zcd:add
#ifdef TP_NS_NET
			m_uState &= ~STATE_CHECKINGACTIVE;
#endif//TP_NS_NET
			//}
			m_hActMsgWnd = (HWND)lParam;
			if(m_uBaseFlag & TP_BASESTATE_NOACTWND) return S_OK;			
			m_bVistaFillWindow = TRUE;
			if(TP_GetIsVista()) LockWindowUpdate(m_hBaseWnd);

			if(lParam) 
			{
				m_pUnActFrame = GetBaseFrame(CWnd::FromHandle((HWND)lParam));
			}

			if(LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE )
			{				
				if(!m_bChild)
				{				
					if(g_pActFrame) g_pActFrame ->m_uState &= ~STATE_ACTIVATE;		
					if((m_hActPreFrame == NULL ||!::IsWindow(m_hActPreFrame)) &&m_pCanActWnd)
					{
						if(m_pCanActWnd((HWND)lParam))	m_hActPreFrame = (HWND)lParam;	
						else if(g_pDisActFrame)         m_hActPreFrame = g_pDisActFrame ->m_hBaseWnd;
					}
					if(!(m_uState &STATE_DOMODL)) g_pActFrame    = this;				
					for(INT l=0;l<m_aBaseFrame.GetSize();l++)
					{
						if(m_aBaseFrame[l] !=this) 
						{
							if(m_aBaseFrame[l] ->m_uState & STATE_ACTIVATE)
							{							
								m_aBaseFrame[l] ->m_uState &= ~STATE_ACTIVATE;	
								m_aBaseFrame[l] ->PaintFrame();
							}
						}
					}
				}
				if(!(m_uState & STATE_ACTIVATE)/* && m_bShow*/)//chenxiaolong 2010.4.23 for bug XN00028865
				{				
					if(!(m_uBaseFlag & TP_BASESTATE_NOACTNOTIFY) && m_pBaseFrameCmd) m_pBaseFrameCmd(TPFrameState_Activate,this);			
					else if(m_pBaseFrameCmd)                                  		 m_pBaseFrameCmd(TPFrameState_ActivatePanel,this);			
				}
				m_uState |= STATE_ACTIVATE;
				m_uState &= ~STATE_DISABLE;		
			}
			else
			{
				if(!m_bChild) g_pDisActFrame = this;				 
				m_uState &= ~STATE_ACTIVATE;		

				if(LOWORD(wParam) == WA_INACTIVE)
				{
					if(m_hUnActFrame != NULL && m_hUnActFrame != (HWND)lParam && ::IsWindow(m_hUnActFrame))
						SendMessage(m_hUnActFrame,WM_ACTIVATE,WA_INACTIVE,(LPARAM)m_hActMsgWnd);
					m_hUnActFrame = NULL;
				}
			}								

			LRESULT lResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);				
			if((m_uBoxAmount & BOX_NAIL ) && !(m_uState & STATE_ACTIVATE))
			{
				if(m_iNailState == 2)
				{					
					if(!PushPinProc((UINT)wParam, CWnd::FromHandle((HWND)lParam),0))
					{
						PostMessage(m_hBaseWnd,WM_CLOSE,0,0);
					}
				}
			}						
			if(TP_GetIsVista()) LockWindowUpdate(NULL);
			PaintFrame();
			if(m_pBaseFrameAct && !(m_uBaseFlag &STATE_NOACTWND)) 
				m_pBaseFrameAct(this,m_uState &STATE_ACTIVATE);				
			return lResult;
		}
		break;	
		
	case WM_NCACTIVATE:
		{
			if(TP_GetIsVista())		FunVistaCmd(3);			
			m_bVistaFillWindow = TRUE;
			if(TP_GetIsVista()) LockWindowUpdate(m_hBaseWnd);
		    LRESULT lResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);						
			if(TP_GetIsVista()) LockWindowUpdate(NULL);
			PaintFrame();
//{2011-01-21:zcd:add
//Windows有BUG，操作任务栏切换到其他程序后，再切换回来，没有WM_ACTIVATE
#ifdef TP_NS_NET
			m_uState |= STATE_CHECKINGACTIVE;
			SetTimer(m_hBaseWnd, TIMEREVENT_CHECKACTIVE, 100, NULL);
#endif//TP_NS_NET
//}
			return  lResult;			
		}
	case WM_NCHITTEST:
		{
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			return FunNcHitTest(ptCursor);
		}
	case WM_COMMAND:
		{
			UINT nID = HIWORD(wParam);	
			if(nID == EN_SETFOCUS || nID == EN_KILLFOCUS || CBN_SETFOCUS == nID || CBN_KILLFOCUS == nID)	
				InvalidateRect((HWND)lParam,NULL,FALSE);

			 nID   = LOWORD(wParam);
			if(nID == ID_SYSMENU_CLOSE)     	PostMessage(m_hBaseWnd,WM_CLOSE,0,0);
			else if(nID == ID_SYSMENU_HELP)		
			{
				::GetCursorPos(&m_stuHelpInfo.MousePos);
				PostMessage(m_hBaseWnd,WM_HELPEX,0,(LPARAM)&m_stuHelpInfo);
				return TRUE;
			}
			if(wParam >= SC_SIZE && wParam <= SC_CONTEXTHELP)  return m_pfnBaseSuper(m_hBaseWnd,WM_SYSCOMMAND,wParam,lParam);
			else                                               return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
		}
	case WM_SYSCOMMAND:
		{

			UINT     nID     = (UINT)wParam;
			if((nID & 0XFFF0) ==  SC_MOUSEMENU ||(nID & 0XFFF0) ==  SC_KEYMENU)  return S_OK;
			if((m_uState & STATE_NOMIN)   && (nID & 0XFFF0) ==  SC_MINIMIZE) return S_OK;
			if((m_uBoxAmount & BOX_MODAL) && ((nID & 0XFFF0) ==  SC_MINIMIZE || (nID & 0XFFF0) ==  SC_MAXIMIZE)) return S_OK; 
			if(!(m_uBoxAmount & BOX_MAX)  && ((nID & 0XFFF0) ==  SC_MAXIMIZE ))                                  return S_OK; 
			if((nID & 0XFFF0) ==  SC_MINIMIZE || (nID & 0XFFF0) ==  SC_RESTORE || (nID & 0XFFF0) ==  SC_MAXIMIZE) 
			{			
				UINT     uState  = m_uState;
				LRESULT  hResult =   S_FALSE;			

				if((nID & 0XFFF0) ==  SC_MINIMIZE|| (nID & 0XFFF0) ==  SC_MAXIMIZE) 
					::GetWindowRect(m_hBaseWnd,m_rtRestore);
				m_uHitTest    = 0;	


				if((uState &  STATE_MINABLE) &&((nID & 0XFFF0) == SC_RESTORE || (nID & 0XFFF0) ==  SC_MAXIMIZE))
				{					
					m_bShow = TRUE;				
					SendMessage(m_hBaseWnd,WM_SHOWWINDOW,TRUE,0);					
				}

				if((nID & 0XFFF0) ==  SC_MINIMIZE && m_bAttachTaskBar)		
				{
 					if(m_pBaseFrameCmd) 	m_pBaseFrameCmd(TPFrameState_Minimize,this);
  					m_uBaseFlag |= TP_BASESTATE_NOACTNOTIFY;
  					m_bShow = FALSE;
  					hResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);										
  					m_uState |=  STATE_NOCALLBACK;
  					SendMessage(m_hBaseWnd,WM_SHOWWINDOW,FALSE,0);
  					//ShowWindow(m_hBaseWnd,SW_HIDE);     //xjc temp modify 防止最小化后还原窗口隐藏20110815
  					m_uState &=  ~STATE_INVISIBLE;
  					m_uState &=  ~STATE_NOCALLBACK;
   					if(m_hActWnd) PostMessage(m_hActWnd,WM_ACTIVATE,WA_ACTIVE,0);//XN00027608
  					m_uBaseFlag &= ~TP_BASESTATE_NOACTNOTIFY;					
				}																	
				else if((nID & 0XFFF0) == SC_MOUSEMENU)	
				{
					if(m_bShow)
					{					
						CPoint ptCutsor; GetCursorPos(&ptCutsor);DoSysMenu(ptCutsor);
					}
					return S_OK;
				}		
				else
				{		
					m_bShow = TRUE;
					if(TP_GetIsVista() && ((nID & 0XFFF0) !=  SC_RESTORE))  LockWindowUpdate(m_hBaseWnd);
					hResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);	
					if(TP_GetIsVista() && ((nID & 0XFFF0) !=  SC_RESTORE))  
					{
						LockWindowUpdate(NULL);
						InvalidateRect(m_hBaseWnd,NULL,FALSE);
					}

				}
				m_bVistaFillWindow = TRUE;	

				m_uState &= ~(STATE_MINABLE | STATE_MAXABLE);
				if(IsZoomed(m_hBaseWnd))      m_uState |=  STATE_MAXABLE;
				else if(IsIconic(m_hBaseWnd)) m_uState |=  STATE_MINABLE;


			
				if(!(m_uState&STATE_MINABLE))
					PaintFrame();///7247

				return hResult;		
			}
			else return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);		

		}
		break;		
		break;
	case WM_DELBASEFRAME:
		{
			CWnd *pWnd = (CWnd *)wParam;
			if(pWnd != NULL)
			{
				if(::IsWindow(pWnd->GetSafeHwnd())) pWnd->DestroyWindow();
				delete pWnd;
			}
			return S_OK;
		}
	case PSM_SETCURSEL:
		return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
	case WM_MOUSEMOVE:
		if(!(wParam & MK_LBUTTON)) m_uState &= ~STATE_LBUTTONDOWN;
		if(m_uDownHitTest == HTCLOSE || m_uDownHitTest == HTMAXBUTTON || m_uDownHitTest == HTMINBUTTON ||	m_uDownHitTest == HTHELP ||  m_uDownHitTest == HTNAIL )
		{
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			FunNcHitTest(ptCursor);
		}
		else if((m_uState & STATE_HELPABLE) && !(m_uState &STATE_HELPSET))
		{
			m_uState |= STATE_HELPSET;
		}

		return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);
	case WM_LBUTTONUP:
		if(m_uDownHitTest == HTCLOSE || m_uDownHitTest == HTMAXBUTTON || m_uDownHitTest == HTMINBUTTON ||	m_uDownHitTest == HTHELP ||  m_uDownHitTest == HTNAIL )
		{			
			ReleaseCapture();			
			if(m_uHitTest == m_uDownHitTest)
			{			
				if(m_uDownHitTest == HTCLOSE) 		    	PostMessage(m_hBaseWnd,WM_SYSCOMMAND ,SC_CLOSE,0 );
				else if(m_uDownHitTest == HTHELP) 			SendMessage(m_hBaseWnd,WM_SYSCOMMAND ,SC_CONTEXTHELP,0 );
				else if(m_uDownHitTest == HTNAIL) 			m_iNailState = (m_iNailState == 1 ? 2:1);
				else if(m_uDownHitTest == HTMAXBUTTON) 	
				{
					if(m_uState & STATE_MAXABLE)  SendMessage(m_hBaseWnd,WM_SYSCOMMAND ,SC_RESTORE,0 );			 
					else  						  SendMessage(m_hBaseWnd,WM_SYSCOMMAND ,SC_MAXIMIZE,0 );
				}
				else 	if(m_uDownHitTest == HTMINBUTTON )
				{
					if(m_uState & STATE_MINABLE)  SendMessage(m_hBaseWnd,WM_SYSCOMMAND ,SC_RESTORE,0 );
					else 					      SendMessage(m_hBaseWnd,WM_SYSCOMMAND ,SC_MINIMIZE,0 );
				}							
			}
			m_uDownHitTest = 0;			
			PaintFrame();
			return 0;
		}
		else
		{
			if(m_uHitTest == HTSYSMENU){m_uDownHitTest = 0;PaintFrame();}
			LRESULT lResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);	
			m_uState &= ~STATE_LBUTTONDOWN;
			return lResult;
		}
	case 0x00AE:
		return 0;
	case WM_ERASEBKGND:
		if(m_bVistaFillWindow && TP_GetIsVista() && TP_GetDWMStatus())
		{						
			FunVistaCmd(4,(LPARAM)CDC::FromHandle((HDC)wParam));		
			m_bVistaFillWindow = FALSE;		
		}
		return TRUE;
	case WM_NCLBUTTONDOWN:
		{			
			m_uState |= STATE_LBUTTONDOWN;
			if(m_uHitTest == HTCLOSE || m_uHitTest == HTMAXBUTTON || m_uHitTest == HTMINBUTTON ||	m_uHitTest == HTHELP ||  m_uHitTest == HTNAIL )
			{
				SetCapture(m_hBaseWnd);
				m_uDownHitTest = m_uHitTest;
				PaintFrame();
				return  S_OK;				
			}
			else 
			{
				if(m_uHitTest == HTSYSMENU)
				{								
					if(m_bToolTips >0)
					{	
						KillTimer(m_hBaseWnd,1123);
						m_pToolTip ->ShowWindow(SW_HIDE);
					}
					m_uDownHitTest = m_uHitTest;
					GetWindowRect(m_hBaseWnd, &m_rtMenuBox);
					m_rtMenuBox.OffsetRect(m_rtSysMenuBox.left,m_rtSysMenuBox.top);
					m_rtMenuBox.right = m_rtMenuBox.left + m_rtSysMenuBox.Width();
					m_rtMenuBox.bottom = m_rtMenuBox.top + m_rtSysMenuBox.Height();
					PaintFrame();					
					g_hBaseWndBack = m_hBaseWnd;
					ShowSysMenu();
					if(!::IsWindow(g_hBaseWndBack)) return S_FALSE;
					CPoint ptCursor;
					GetCursorPos(&ptCursor);
					FunNcHitTest(ptCursor);
					m_rtMenuBox    = CRect(0,0,0,0);
					m_uDownHitTest =0;
				}
				else
				{
					if(TP_GetIsVista()) FunVistaCmd(2);
					return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);			
				}
			}
		}		
	case WM_NCLBUTTONDBLCLK:
		{
			if(wParam == HTSYSMENU)		    return S_OK;
			if(m_uBaseState & STATE_NOLDB)	return S_OK;
			return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);			
		}
	case WM_MOVE:
		{		
			LRESULT lResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);				
			if(m_uState & STATE_LBUTTONDOWN) OnMove();
			m_rtMoving     = CRect(0,0,0,0);			
			m_rtBeforeMov  = CRect(0,0,0,0);						
#ifdef TP_NS_LOG
			if(m_uState & STATE_LOGOTEXT)
			{			
				CString sText;sText.Format(_T("Window Move %d %d"),LOWORD(lParam),HIWORD(lParam));
				//TP_LogEditNotifyCtrl(0,sText,m_sText,EDIT_LOGGER_MODULE,(CString)__FUNCTION__);					
			}
#endif
			return lResult;
		}
		break;
	case WM_MOVING:
		{		
			if(m_bFixWnd &&(m_uFrameStyle & FRAME_STYLE_LOCKWND))
			{
				::GetWindowRect(m_hBaseWnd,m_rtSize);
				*(CRect *)lParam = m_rtSize;
				return S_OK;
			}
			else
			{			
				LRESULT lResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);				
#ifdef TP_NS_EDIT
				if(!TP_IsAltKeyDown() && (m_uState & STATE_LBUTTONDOWN)) OnMoving((CRect *)lParam); //7168				
#else
				if(m_hDockParentWnd) OnMoving((CRect *)lParam);				
#endif
				return lResult;
			}
		}
		break;
	case WM_SHOWWINDOW:
		m_bShow = (BOOL)wParam;		
		if(m_bShow) 
		{
			m_uState &= ~STATE_INVISIBLE ;
			m_uState |= STATE_LOGOTEXT ;		
			m_bVistaFillWindow = TRUE;
		}
		else
		{
			m_uState |= STATE_INVISIBLE ;
			m_uState &= ~STATE_LOGOTEXT ;			
		}

		if(m_pBaseFrameCmd && !(m_uState & STATE_NOCALLBACK)) m_pBaseFrameCmd(TPFrameState_ShowWindow,this);	
		if((m_uFrameStyle & FRAME_STYLE_SHOWACT) && !m_bShow && !(m_uState & STATE_NOCALLBACK)) SetPreActWnd();
		if(m_bShow) 
		{		
			if(m_uFrameType == FRAME_SV|| m_uFrameType == FRAME_MV)		m_hActPreFrame = NULL; 
			m_bVistaFillWindow = TRUE;
		}
		else  m_hActPreFrame = NULL;
		return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);

	case WM_DOCKPARENTWND:
		{			
			if(m_hDockParentWnd != NULL || lParam == NULL) return S_OK;
			CWnd *pWnd = (CWnd *)lParam;
			if(wParam == 0)
			{
				for(int l= (int)m_aDockWnd.GetSize()-1;l>=0;l--)
				{
					if(m_aDockWnd[l].hChildWnd == pWnd ->m_hWnd)
						m_aDockWnd.RemoveAt(l);
				}
			}
			else
			{
				for(int l= (int)m_aDockWnd.GetSize()-1;l>=0;l--)
				{
					if(m_aDockWnd[l].hChildWnd == pWnd ->m_hWnd)
					{
						m_aDockWnd[l].wDockX = LOWORD(wParam);						
						m_aDockWnd[l].wDockY = HIWORD(wParam);						
						return S_OK;
					}
				}
				TPDockData  stuDockData;
				stuDockData.wDockX = LOWORD(wParam);						
				stuDockData.wDockY = HIWORD(wParam);						
				stuDockData.hChildWnd   = ((CWnd *)lParam)->m_hWnd;
				m_aDockWnd.Add(stuDockData);
			}
		}
		break;	
	case WM_NOTIFY:
		{		
			NMHDR *pNMHdr = (NMHDR *)lParam;
			if(pNMHdr ->code == TTN_NEEDTEXT && pNMHdr ->hwndFrom == m_hBaseWnd) 
			{
				TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHdr;
				CPoint ptCursor;CRect rtWnd;				
				::GetCursorPos(&ptCursor); 
				GetWindowRect(m_hBaseWnd,rtWnd);
				ptCursor.Offset(-rtWnd.left,-rtWnd.top);
			}
			return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);	
		}
		break;
	case WM_BASEFRAMESETRGN:
		if(m_hBaseWnd && m_hRgnWnd)
			::SetWindowRgn(m_hBaseWnd,m_hRgnWnd,FALSE);
		break;
	case WM_DWMCOMPOSITIONCHANGED:
		{			
			//TP_GetDWMStatus(TRUE);
			//return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);			
		}
		break;
	case WM_WINDOWPOSCHANGING:
		{
			LRESULT lResult = m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);	
			WINDOWPOS *pPos = (WINDOWPOS *)lParam;
			if(pPos->hwndInsertAfter == TP_GetMainWnd()->GetSafeHwnd())
				pPos ->flags |= SWP_NOOWNERZORDER;
			return lResult;
		}
		break;
	case WM_DWMCOLORIZATIONCOLORCHANGED:
	case WM_DWMWINDOWMAXIMIZEDCHANGE:
		break;
	default:
		{
			return m_pfnBaseSuper(m_hBaseWnd,message,wParam,lParam);			
		}
	}
	return S_OK;
}
void CTPBaseFrame::PaintFrame()
{
	if(m_bChild  || m_bNoMsg) return;
	if(!m_bShow) return;
	CWnd   *pWnd      = CWnd::FromHandle(m_hBaseWnd);		
	CDC    *pDC       = pWnd ->GetWindowDC();		
	CTPDCExchange stuTemp(pDC);	
	PaintFrameNS(pDC);
	stuTemp.ReleaseDC();
	pWnd ->ReleaseDC(pDC);
}
CRect  CTPBaseFrame::GetBoxRect(DWORD dAmoutBox)
{
	CRect rtWnd;
	GetWindowRect(m_hBaseWnd,&rtWnd);
	if(dAmoutBox == BOX_SYSMENU)
	{
		rtWnd.OffsetRect( m_rtSysMenuBox.left,m_rtSysMenuBox.top);
		rtWnd.bottom = m_rtSysMenuBox.Height() + rtWnd.top;
		rtWnd.right  = m_rtSysMenuBox.Width()  + rtWnd.left;
	}
	return rtWnd;
}
void  CTPBaseFrame::DoSysMenu()
{
	if(!m_bShow) return;
	if(m_bToolTips >0)
	{	
		KillTimer(m_hBaseWnd,1123);
		m_pToolTip ->ShowWindow(SW_HIDE);
	}
	GetWindowRect(m_hBaseWnd, &m_rtMenuBox);
	m_rtMenuBox.OffsetRect(m_rtSysMenuBox.left,m_rtSysMenuBox.top);
	m_rtMenuBox.right = m_rtMenuBox.left + m_rtSysMenuBox.Width();
	m_rtMenuBox.bottom = m_rtMenuBox.top + m_rtSysMenuBox.Height();
	g_hBaseWndBack = m_hBaseWnd;
	ShowSysMenu();
	if(!::IsWindow(g_hBaseWndBack)) return;
	m_rtMenuBox  = CRect(0,0,0,0);
}

void  CTPBaseFrame::ShowSysMenu()
{
	if((m_uBoxAmount&BOX_SYSMENU) != BOX_SYSMENU) return;//DQA4528 jw 20090119
	CPoint ptCutsor; CRect rtWnd;
	GetWindowRect(m_hBaseWnd,rtWnd);
	ptCutsor = CPoint(rtWnd.left,rtWnd.top);
	ptCutsor.Offset(m_rtSysMenuBox.left,m_rtSysMenuBox.bottom);	
	ptCutsor.x = max(ptCutsor.x,0);
	ptCutsor.y = max(ptCutsor.y,0);
	if(!DoSysMenu(ptCutsor))
	{						
		CTPMenuEx mainLoad,*psubMenu = NULL;
		mainLoad.LoadMenu(IDR_CTRLMENU);
		psubMenu = mainLoad.GetSubMenu(0);
		if(psubMenu)
		{		
			TPHotKeyData stuData[] = 
			{
				{0,ID_SYSMENU_HELP,VK_F1,},
				{0,ID_SYSMENU_CLOSE,VK_W,TP_CONTROL},
			};		
			TP_AssembledHotKeyControlData(0,NULL,2,stuData);
			psubMenu->SetHotKeyData(stuData,2);
			TP_LoadMenuStr(psubMenu);
			psubMenu ->TrackPopupMenu(TPM_LEFTALIGN,ptCutsor.x,ptCutsor.y,CWnd::FromHandle(m_hBaseWnd));						
		}
		mainLoad.DestroyMenu();
	}		
}
void  CTPBaseFrame::BitmapToRgn(HRGN &rgnSet,HBITMAP hBitmap,COLORREF backcolor,CDC *pDC,CRect rtExclude)
{	
	BYTE     rValue = GetRValue(backcolor);
	BYTE     gValue = GetGValue(backcolor);
	BYTE     bValue = GetBValue(backcolor);	
	CSize    szBmp  = CSize(0,0);
	int      iIndex = 0;
	LPBYTE   pBuf   = TP_GetBitmapBits(hBitmap,szBmp,pDC);

	rgnSet = NULL;
	if(pBuf)
	{		
		BOOL  bMakeRgn   = FALSE; 
		DWORD maxRects   = 100;
		HANDLE hData = malloc(sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
		if(hData == NULL){delete [] pBuf;return;}
		RGNDATA *pData = (RGNDATA *)hData;
		pData->rdh.dwSize = sizeof(RGNDATAHEADER);
		pData->rdh.iType = RDH_RECTANGLES;
		pData->rdh.nCount = pData->rdh.nRgnSize = 0;
		SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
		for(int height=0;height<szBmp.cy;height++)
		{			
			for(int width=0;width<szBmp.cx;width++)
			{				
				if(rtExclude.PtInRect(CPoint(width,height))) continue;			
				iIndex = (width + (szBmp.cy - height -1) * szBmp.cx)*4;
				bMakeRgn = (pBuf[iIndex] == rValue) &&  (pBuf[iIndex+1] == gValue) && (pBuf[iIndex+2] == bValue);
				if(bMakeRgn)
				{  						
					if (pData->rdh.nCount >= maxRects-1)
					{					
						maxRects += 100;
						hData = realloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
						pData = (RGNDATA *)hData;					
					}					
					RECT *pr = (RECT *)&pData->Buffer;
					SetRect(&pr[pData->rdh.nCount],width,height,width+1,height+1);	
					if (width < pData->rdh.rcBound.left)	  pData->rdh.rcBound.left = width;
					if (height < pData->rdh.rcBound.top)  	  pData->rdh.rcBound.top = height;
					if (width+1 > pData->rdh.rcBound.right)   pData->rdh.rcBound.right = width+1;
					if (height+1 > pData->rdh.rcBound.bottom) pData->rdh.rcBound.bottom = height+1;
					pData->rdh.nCount++;					
					if(pData->rdh.nCount >=200000) break;
				} 				
			}		
		}
		HRGN rgnTemp1 = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);	
		HRGN rgnTemp2 = NULL;
		if(0 == m_uRoundStyle)
		{
			rgnTemp2 = CreateRectRgn(0,0,szBmp.cx,szBmp.cy);
			rgnSet   = CreateRectRgn(0,0,szBmp.cx,szBmp.cy);
		}
		else if(1 == m_uRoundStyle) // 非矩形边框
		{
			rgnTemp2 =  CreateRoundRectRgn(0,0,szBmp.cx,szBmp.cy,15,15);
			rgnSet   =  CreateRoundRectRgn(0,0,szBmp.cx,szBmp.cy,15,15);
		}
		CombineRgn(rgnSet,rgnTemp2,rgnTemp1,RGN_DIFF);	
		DeleteObject(rgnTemp1);
		DeleteObject(rgnTemp2);
		free(hData);
		delete [] pBuf;
	}
}
void CTPBaseFrame::PaintFrameNS(CDC *pDC)
{	
	enum IMAGE_INDEX
	{
		IMAGE_EX_NOR_N,
		IMAGE_EX_NOR_A,
		IMAGE_EX_OVER_A,
		IMAGE_EX_DOWN,
		IMAGE_EX_OVER_N, 
		IMAGE_EX_DIS = IMAGE_EX_NOR_N,

	};
	CRect rtWnd,rtClient,rtTemp;
	CWnd *pWnd = CWnd::FromHandle(m_hBaseWnd);
	if(m_bBaseWnd)
	{
		pWnd->GetClientRect(&rtWnd);
		m_rtClient = rtWnd;
	}
	else
	{
		pWnd->GetWindowRect(&rtWnd);
		pWnd->GetClientRect(rtClient);
		m_rtClient = rtClient;
		pWnd->ClientToScreen(&rtClient);
		rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
	}
	if(m_iWndWidth != rtWnd.Width() ||  m_iWndHeight!= rtWnd.Height()) 	FunSize();
	CTPMemoDC memDC(pDC,rtWnd.Width(),rtWnd.Height());
	memDC.FillSolidRect(rtWnd,RGB(255,0,0));
	CFont *pSaveFont =  memDC.SelectObject(TP_GetSysFont(FONT_BASE_TEXT));
	int  iIndex  = 0,iImageIndex =0;
	BOOL bEnable = !(m_uState & STATE_DISABLE);
	//画边縼E
	if(BOX_BORDER & m_uBoxAmount)
	{	
		if(m_uState&STATE_ACTIVATE) iIndex = BORDER_LEFT_A;
		else                        iIndex = BORDER_LEFT_N;
		rtTemp = CRect(0,0,m_pResources[iIndex]->Width(),rtWnd.bottom);
		TP_TileDIBLine(&memDC,m_pResources[iIndex]->GetByte(),m_pResources[iIndex]->Width(),m_pResources[iIndex]->Height(),rtTemp,TRUE);
		if(m_uState&STATE_ACTIVATE) iIndex = BORDER_RIGHT_A;
		else                        iIndex  = BORDER_RIGHT_N;
		rtTemp = CRect(rtWnd.right - m_pResources[iIndex]->Width(),0,rtWnd.right,rtWnd.bottom);
		TP_TileDIBLine(&memDC,m_pResources[iIndex]->GetByte(),m_pResources[iIndex]->Width(),m_pResources[iIndex]->Height(),rtTemp,TRUE);
	}
	if(m_uState&STATE_ACTIVATE) iIndex = BORDER_TOP_A;
	else                        iIndex = BORDER_TOP_N;
	rtTemp = CRect(0,0,rtWnd.right,m_pResources[iIndex]->Height());
	TP_StretchDIBRect(&memDC,m_pResources[iIndex]->GetByte(),m_pResources[iIndex]->Width(),m_pResources[iIndex]->Height(),10,10,rtTemp,FALSE);
	if(BOX_BORDER & m_uBoxAmount)
	{	
		if(m_uState&STATE_ACTIVATE) iIndex = BORDER_BOTTOM_A;
		else                        iIndex = BORDER_BOTTOM_N;
		m_rtBotton = rtTemp = CRect(0,rtWnd.bottom - m_pResources[iIndex]->Height(),rtWnd.right,rtWnd.bottom);
		TP_StretchDIBRect(&memDC,m_pResources[iIndex]->GetByte(),m_pResources[iIndex]->Width(),m_pResources[iIndex]->Height(),10,10,rtTemp,FALSE); 
		rtTemp.DeflateRect(20,0,0,0);
		memDC.SetTextColor(TP_GetSysColor(COLOR_INACTIVECAPTION));
		if(m_sTips.GetLength()>0) memDC.DrawText(m_sTips,rtTemp,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}			
	if(m_uBoxAmount & BOX_CLOSE) 
	{
		iIndex = BOX_RES_CLOSE;
		rtTemp = m_rtCloseBox;      

		if(!bEnable)                                                    iImageIndex = IMAGE_EX_DIS;
		else if(m_uHitTest == HTCLOSE && m_uDownHitTest == 0)          {if(m_uState&STATE_ACTIVATE) iImageIndex = IMAGE_EX_OVER_A;	else iImageIndex = IMAGE_EX_OVER_N;		}
		else if(m_uDownHitTest == HTCLOSE &&  m_uHitTest == HTCLOSE)	iImageIndex = IMAGE_EX_DOWN; 
		else if(m_uState&STATE_ACTIVATE)                                iImageIndex = IMAGE_EX_NOR_A;
		else                                                            iImageIndex = IMAGE_EX_NOR_N;

		ImageList_Draw(m_pResources[iIndex] ->GetImage(),iImageIndex,memDC.m_hDC,rtTemp.left ,rtTemp.top,ILD_TRANSPARENT);		
	}
	if(m_uBoxAmount & BOX_MAX) 
	{
		if(m_uState & STATE_MAXABLE)	iIndex = BOX_RES_RESTORE;
		else                       	    iIndex = BOX_RES_MAX;
		rtTemp = m_rtMaxBox;

		if(!bEnable)                                                            iImageIndex = IMAGE_EX_DIS;
		else if(m_uHitTest == HTMAXBUTTON && m_uDownHitTest == 0)              {if(m_uState&STATE_ACTIVATE) iImageIndex = IMAGE_EX_OVER_A;	else iImageIndex = IMAGE_EX_OVER_N;		}
		else if(m_uDownHitTest == HTMAXBUTTON &&  m_uHitTest == HTMAXBUTTON)	iImageIndex = IMAGE_EX_DOWN; 
		else if(m_uState&STATE_ACTIVATE)                                        iImageIndex = IMAGE_EX_NOR_A;
		else                                                                    iImageIndex = IMAGE_EX_NOR_N;

		ImageList_Draw(m_pResources[iIndex] ->GetImage(),iImageIndex,memDC.m_hDC,rtTemp.left ,rtTemp.top,ILD_TRANSPARENT);
	}
	if(m_uBoxAmount & BOX_MIN) 
	{		
		if(m_uState & STATE_MINABLE)	iIndex = BOX_RES_RESTORE;
		else                       	    iIndex = BOX_RES_MIN;
		rtTemp = m_rtMinBox;

		if(!bEnable)                                                            iImageIndex = IMAGE_EX_DIS;
		else if(m_uHitTest == HTMINBUTTON && m_uDownHitTest == 0)              {if(m_uState&STATE_ACTIVATE) iImageIndex = IMAGE_EX_OVER_A;	else iImageIndex = IMAGE_EX_OVER_N;		}
		else if(m_uDownHitTest == HTMINBUTTON &&  m_uHitTest == HTMINBUTTON)	iImageIndex = IMAGE_EX_DOWN; 
		else if(m_uState&STATE_ACTIVATE)                                        iImageIndex = IMAGE_EX_NOR_A;
		else                                                                    iImageIndex = IMAGE_EX_NOR_N;

		ImageList_Draw(m_pResources[iIndex] ->GetImage(),iImageIndex,memDC.m_hDC,rtTemp.left ,rtTemp.top,ILD_TRANSPARENT);
	}
	int iAttachImage = 0;
	if(m_pAttachImage && m_iAttachImage>=0)
	{
		iImageIndex = m_iAttachImage;
		if(m_uState&STATE_ACTIVATE)  iImageIndex++; 
		rtTemp.OffsetRect(-m_pAttachImage->Width()-12,0); 
		rtTemp.top += (rtTemp.Height() - m_pAttachImage->Height())/2;
		ImageList_Draw(m_pAttachImage ->GetImage(),iImageIndex,memDC.m_hDC,rtTemp.left ,rtTemp.top,ILD_TRANSPARENT);
		iAttachImage = rtTemp.left ;
	}
	if(m_uBoxAmount & BOX_SYSMENU)
	{		
		iIndex = BOX_RES_SYSMENU;				
		rtTemp = m_rtSysMenuBox;

		if(!bEnable)                                                        iImageIndex = IMAGE_EX_DIS;
		else if(m_uHitTest == HTSYSMENU  && m_uDownHitTest == 0)           {if(m_uState&STATE_ACTIVATE) iImageIndex = IMAGE_EX_OVER_A;	else iImageIndex = IMAGE_EX_OVER_N;		}
		else if(m_uDownHitTest == HTSYSMENU &&  m_uHitTest == HTSYSMENU)	iImageIndex = IMAGE_EX_DOWN; 
		else if(m_uState& STATE_ACTIVATE)                                   iImageIndex = IMAGE_EX_NOR_A;
		else                                                                iImageIndex = IMAGE_EX_NOR_N;

		ImageList_Draw(m_pResources[iIndex] ->GetImage(),iImageIndex,memDC.m_hDC,rtTemp.left ,rtTemp.top,ILD_TRANSPARENT);
	}
	if(m_rtText.left<m_rtText.right - 4)
	{
		rtTemp = m_rtText;
		if(iAttachImage>0) rtTemp.right = iAttachImage - 4;
		if(m_uState&STATE_ACTIVATE) memDC.SetTextColor(TP_GetSysColor(COLOR_ACTIVECAPTION));
		else           memDC.SetTextColor(TP_GetSysColor(COLOR_INACTIVECAPTION));
		memDC.DrawText(m_sText,rtTemp,DT_SINGLELINE | DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_NOPREFIX);
	}
	PaintFrame(&memDC,rtWnd);
	if(m_iWndWidth != rtWnd.Width() ||  m_iWndHeight!= rtWnd.Height()) 
	{
		m_iWndWidth  = rtWnd.Width();
		m_iWndHeight = rtWnd.Height();		
		if(m_hRgnWnd) DeleteObject(m_hRgnWnd);				
		BitmapToRgn(m_hRgnWnd,(HBITMAP)memDC.GetBitmap()->GetSafeHandle(),RGB(255,0,255),&memDC,rtClient);	
		PostMessage(m_hBaseWnd,WM_BASEFRAMESETRGN,0,0);	
	}	
	pDC ->ExcludeClipRect(rtClient);
	pDC ->BitBlt(0,0,rtWnd.Width(),rtWnd.Height(),&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pSaveFont);
}

void CTPBaseFrame::SetSkinEx(CString sSkinPath)
{
	for(int l=0;l<FRAME_MAX;l++)
	{
		TP_ReleaseCommonRes(m_pResources[l]);
		m_pResources[l] = NULL;
	}

	m_pResources[BOX_RES_CLOSE]    = TP_LoadImage(sSkinPath + _L("\\Frame_CloseBox.bmp"),12,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_MAX]      = TP_LoadImage(sSkinPath + _L("\\Frame_MaxBox.bmp"),12,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_MIN]      = TP_LoadImage(sSkinPath + _L("\\Frame_MinBox.bmp"),12,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_RESTORE]  = TP_LoadImage(sSkinPath + _L("\\Frame_RestoreBox.bmp"),12,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_HELP]     = TP_LoadImage(sSkinPath + _L("\\Frame_HelpBox.bmp"),12,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_NAIL_N]   = TP_LoadImage(sSkinPath + _L("\\Frame_NailBox_Nor.bmp"),12,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_NAIL_D]   = TP_LoadImage(sSkinPath + _L("\\Frame_NailBox_Down.bmp"),12,RGB(255,255,255),FALSE,FALSE);

	m_pResources[BORDER_TOP_A]     = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Top_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_TOP_N]     = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Top_Nor.bmp"),FALSE,FALSE);
	m_pResources[BORDER_LEFT_A]    = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Left_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_LEFT_N]    = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Left_Nor.bmp"),FALSE,FALSE);
	m_pResources[BORDER_RIGHT_A]   = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Right_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_RIGHT_N]   = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Right_Nor.bmp"),FALSE,FALSE);
	m_pResources[BORDER_BOTTOM_A]  = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Bottom_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_BOTTOM_N]  = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Bottom_Nor.bmp"),FALSE,FALSE);
	m_pResources[BORDER_BAR_A]     = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Bar_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_BAR_N]     = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Bar_Nor.bmp"),FALSE,FALSE);

	m_pResources[BORDER_SIZEBOX_A] = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Sizebox_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_SIZEBOX_N] = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Sizebox_Nor.bmp"),FALSE,FALSE);

}
void CTPBaseFrame::SetSkin(CString sSkinPath)
{
	for(int l=0;l<FRAME_MAX;l++)
	{
		TP_ReleaseCommonRes(m_pResources[l]);
		m_pResources[l] = NULL;
	}


	m_pResources[BOX_RES_CLOSE]    = TP_LoadImage(sSkinPath + _L("\\Frame_CloseBox.bmp"),17,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_MAX]      = TP_LoadImage(sSkinPath + _L("\\Frame_MaxBox.bmp"),17,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_MIN]      = TP_LoadImage(sSkinPath + _L("\\Frame_MinBox.bmp"),17,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_RESTORE]  = TP_LoadImage(sSkinPath + _L("\\Frame_RestoreBox.bmp"),17,RGB(255,255,255),FALSE,FALSE);
	m_pResources[BOX_RES_SYSMENU]  = TP_LoadImage(sSkinPath + _L("\\Frame_MenuBox.bmp"),17,RGB(255,255,255),FALSE,FALSE);



	m_pResources[BORDER_TOP_A]     = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Top_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_TOP_N]     = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Top_Nor.bmp"),FALSE,FALSE);
	m_pResources[BORDER_LEFT_A]    = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Left_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_LEFT_N]    = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Left_Nor.bmp"),FALSE,FALSE);
	m_pResources[BORDER_RIGHT_A]   = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Right_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_RIGHT_N]   = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Right_Nor.bmp"),FALSE,FALSE);
	m_pResources[BORDER_BOTTOM_A]  = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Bottom_Activate.bmp"),FALSE,FALSE);
	m_pResources[BORDER_BOTTOM_N]  = TP_LoadBmpByte(sSkinPath + _L("\\Frame_Bottom_Nor.bmp"),FALSE,FALSE);


}
void CTPBaseFrame::FunSizing(UINT fwSide, LPRECT pRect) 
{
	if(m_iScaleHeight == 0 || m_iScaleWidth == 0) return;

	CRect rtWnd,rtClient,rtTemp;
	CWnd *pWnd = CWnd::FromHandle(m_hBaseWnd);
	pWnd->GetWindowRect(&rtWnd);
	pWnd->GetClientRect(&rtClient);
	pWnd->ClientToScreen(&rtClient);
	rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
	rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);

	int iWidth    = pRect->right-pRect->left ;
	int iHeight   = pRect->bottom-pRect->top ;
	int iExWidth  = m_rtSrcScale.left + m_rtSrcScale.right + rtWnd.Width() - rtClient.Width() ;
	int iExHeight = m_rtSrcScale.top + m_rtSrcScale.bottom + rtWnd.Height() - rtClient.Height();

	if(fwSide==WMSZ_BOTTOM||fwSide==WMSZ_TOP)//拉上下两边时
	{
		iHeight -= iExHeight;
		iWidth   = iHeight*m_iScaleWidth/m_iScaleHeight;
		iWidth  += iExWidth;
		iHeight += iExHeight;			
	}
	else  if(fwSide==WMSZ_RIGHT||fwSide==WMSZ_LEFT)//拉左右两边时
	{
		iWidth   -= iExWidth ;
		iHeight   = iWidth*m_iScaleHeight/m_iScaleWidth;
		iWidth   += iExWidth ;
		iHeight  += iExHeight;			
	}
	else
	{
		iWidth   -= iExWidth ;
		iHeight  -= iExHeight;

		if(iHeight*m_iScaleHeight < iWidth *m_iScaleWidth)
		{
			iHeight = iWidth*m_iScaleHeight/m_iScaleWidth;
		}
		else
		{
			iWidth = iHeight*m_iScaleWidth/m_iScaleHeight;
		}
		iWidth   += iExWidth;
		iHeight  += iExHeight;
	}

	switch(fwSide)
	{
	case  WMSZ_BOTTOMLEFT:
		pRect->left=pRect->right-iWidth;
		pRect->bottom=pRect->top+iHeight;
		break;
	case WMSZ_BOTTOMRIGHT:
	case WMSZ_BOTTOM:
	case WMSZ_RIGHT:
		pRect->right=pRect->left+iWidth;
		pRect->bottom=pRect->top+iHeight;
		break;
	case WMSZ_TOPLEFT:
	case WMSZ_TOP:
	case WMSZ_LEFT:
		pRect->left=pRect->right-iWidth;
		pRect->top=pRect->bottom-iHeight;
		break;
	case WMSZ_TOPRIGHT:
		pRect->right=pRect->left+iWidth;
		pRect->top=pRect->bottom-iHeight;
		break;
	}
	if(pRect->top<0) ::OffsetRect(pRect,0,abs(pRect->top));

}
void CTPBaseFrame::FunSize()
{
	CRect rtWnd,rtClient;
	CWnd *pWnd = CWnd::FromHandle(m_hBaseWnd);
	pWnd->GetWindowRect(&rtWnd);
	pWnd->GetClientRect(&rtClient);
	pWnd->ClientToScreen(&rtClient);
	rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
	rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
	//计算各种位置
	int iBorder = 2;
	int iCx = rtWnd.right - iBorder*3-6,iCy = (m_pResources[BORDER_TOP_N] ->Height() -  m_pResources[BORDER_TOP_N]->Height()- iBorder )/2+iBorder+3;
	m_rtText =  CRect(iBorder *2+6,iBorder,rtWnd.right - iBorder,m_pResources[BORDER_TOP_N] ->Height());
#ifdef  TP_NS_EDIT
	iCx += 6;
#endif
	if(m_uBoxAmount & BOX_CLOSE) 
	{
		m_rtCloseBox = CRect(iCx -  m_pResources[BOX_RES_CLOSE] ->Width(),iCy,iCx,iCy + m_pResources[BOX_RES_CLOSE] ->Height()) ; 
		iCx -=  m_pResources[BOX_RES_CLOSE] ->Width() + iBorder;
	}
	if(m_uBoxAmount & BOX_MAX) 
	{
		m_rtMaxBox = CRect(iCx -  m_pResources[BOX_RES_MAX] ->Width(),iCy,iCx,iCy + m_pResources[BOX_RES_MAX] ->Height()) ; 
		iCx -=  m_pResources[BOX_RES_MAX] ->Width() + iBorder;
	}
	if(m_uBoxAmount & BOX_MIN) 
	{
		m_rtMinBox = CRect(iCx -  m_pResources[BOX_RES_MIN] ->Width(),iCy,iCx,iCy + m_pResources[BOX_RES_MIN] ->Height()) ; 
		iCx -=  m_pResources[BOX_RES_MIN] ->Width() + iBorder;
	}
	if(m_uBoxAmount & BOX_HELP) 
	{
		m_rtHelpBox = CRect(iCx -  m_pResources[BOX_RES_HELP] ->Width(),iCy,iCx,iCy + m_pResources[BOX_RES_HELP] ->Height()) ; 
		iCx -=  m_pResources[BOX_RES_HELP] ->Width() + iBorder;
	}
	if(m_uBoxAmount & BOX_NAIL)
	{
		m_rtNailBox = CRect(iCx -  m_pResources[BOX_RES_NAIL_N] ->Width(),iCy,iCx,iCy + m_pResources[BOX_RES_NAIL_N] ->Height()) ; 
		iCx -=  m_pResources[BOX_RES_NAIL_N] ->Width() + iBorder;
	}
	if(m_uBoxAmount & BOX_SYSMENU)
	{
		m_rtSysMenuBox =  CRect(iBorder*4+4,iCy,iBorder*4+4 + m_pResources[BOX_RES_SYSMENU] ->Width(),iCy + m_pResources[BOX_RES_SYSMENU] ->Height()) ; 
		m_rtText.left += m_pResources[BOX_RES_SYSMENU] ->Width() + iBorder*4;
	}
	if(m_uBoxAmount & BOX_ICON)
	{
		int cx = GetSystemMetrics(SM_CXSMICON);
		int cy = GetSystemMetrics(SM_CYSMICON);
		m_rtIconBox = CRect(iBorder*2,(m_pResources[BORDER_TOP_N] ->Height() -  cy - iBorder )/2+iBorder,iBorder*2 + cx,(m_pResources[BORDER_TOP_N] ->Height() +  cy - iBorder )/2+iBorder) ; 		
		m_rtText.left += cx + iBorder*2;
	}	
	m_rtText.right = iCx - iBorder*2;
}
UINT CTPBaseFrame::FunNcHitTest(CPoint point)
{	
	CRect rtWnd,rtClient;
	CWnd *pWnd    = CWnd::FromHandle(m_hBaseWnd);
	UINT uHitTest = m_uHitTest;
	pWnd->GetWindowRect(&rtWnd);
	pWnd->GetClientRect(&rtClient);
	pWnd->ClientToScreen(&rtClient);
	rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
	point.Offset(-rtWnd.left,-rtWnd.top);
	rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);	

	int iBorder = max(m_pResources[BORDER_LEFT_N] ->Width(),m_pResources[BORDER_RIGHT_N] ->Width());

	if(m_bChild)
	{
		m_uHitTest = HTCLIENT;
	}
	else
	{
		if((m_uBoxAmount & BOX_CLOSE)        && m_rtCloseBox.PtInRect(point))  	m_uHitTest = HTCLOSE;
		else if((m_uBoxAmount & BOX_MAX)     && m_rtMaxBox.PtInRect(point))     m_uHitTest = HTMAXBUTTON;
		else if((m_uBoxAmount & BOX_MIN)     && m_rtMinBox.PtInRect(point))     m_uHitTest = HTMINBUTTON;
		else if((m_uBoxAmount & BOX_HELP)    && m_rtHelpBox.PtInRect(point))    m_uHitTest = HTHELP;
		else if((m_uBoxAmount & BOX_NAIL)    && m_rtNailBox.PtInRect(point))    m_uHitTest = HTNAIL;
		else if((m_uBoxAmount & BOX_SYSMENU) && m_rtSysMenuBox.PtInRect(point)) m_uHitTest = HTSYSMENU;

		else if((m_uBoxAmount & BOX_SIZE) && point.x <rtWnd.left  + iBorder*3 && point.y < rtWnd.top + iBorder*3)     m_uHitTest = HTTOPLEFT;
		else if((m_uBoxAmount & BOX_SIZE) && point.x >rtWnd.right - iBorder*3 && point.y < rtWnd.top + iBorder*3)     m_uHitTest = HTTOPRIGHT;
		else if((m_uBoxAmount & BOX_SIZE) && point.x <rtWnd.left  + iBorder*3 && point.y > rtWnd.bottom - iBorder*3)  m_uHitTest = HTBOTTOMLEFT;
		else if((m_uBoxAmount & BOX_SIZE) && point.x >rtWnd.right - iBorder*3 && point.y > rtWnd.bottom - iBorder*3)  m_uHitTest = HTBOTTOMRIGHT;

		else if((m_uBoxAmount & BOX_SIZE) && point.x < rtWnd.left  + m_pResources[BORDER_LEFT_N]   ->Width() )     m_uHitTest = HTLEFT;
		else if((m_uBoxAmount & BOX_SIZE) && point.x > rtWnd.right - m_pResources[BORDER_RIGHT_N]  ->Width() )     m_uHitTest = HTRIGHT;
		else if((m_uBoxAmount & BOX_SIZE) && point.y < rtWnd.top   + iBorder )                                     m_uHitTest = HTTOP;
		else if((m_uBoxAmount & BOX_SIZE) && point.y < rtWnd.top   + m_pResources[BORDER_TOP_N]    ->Height() )    m_uHitTest = HTCAPTION;
		else if((m_uBoxAmount & BOX_SIZE) && point.y > rtWnd.bottom- m_pResources[BORDER_BOTTOM_N] ->Height() )    m_uHitTest = HTBOTTOM;

		else if(rtClient.PtInRect(point))      m_uHitTest = HTCLIENT;
		else if(point.y<rtWnd.bottom - 20)     m_uHitTest = HTCAPTION;
	}
	if(m_bFixWnd &&(m_uFrameStyle & FRAME_STYLE_LOCKWND)) 
	{
		if( m_uHitTest >= HTLEFT && m_uHitTest <= HTBOTTOMRIGHT) m_uHitTest = HTBORDER;
	}
	if((m_uFrameStyle & FRAME_STYLE_LOCKLEFT)   && (m_uHitTest == HTLEFT  || m_uHitTest == HTTOPLEFT  || m_uHitTest == HTBOTTOMLEFT))     m_uHitTest = HTBORDER;
	if((m_uFrameStyle & FRAME_STYLE_LOCKRIGHT)  && (m_uHitTest == HTRIGHT || m_uHitTest == HTTOPRIGHT || m_uHitTest == HTBOTTOMRIGHT))    m_uHitTest = HTBORDER;
	if((m_uFrameStyle & FRAME_STYLE_LOCKTOP)    && (m_uHitTest == HTTOP   || m_uHitTest == HTTOPRIGHT || m_uHitTest == HTTOPLEFT))        m_uHitTest = HTBORDER;
	if((m_uFrameStyle & FRAME_STYLE_LOCKBOTTOM) && (m_uHitTest == HTBOTTOM || m_uHitTest == HTBOTTOMLEFT || m_uHitTest == HTBOTTOMRIGHT)) m_uHitTest = HTBORDER;

	if(m_uHitTest != uHitTest && 
		(m_uHitTest == HTCLOSE || m_uHitTest == HTMAXBUTTON || m_uHitTest == HTMINBUTTON ||	m_uHitTest == HTHELP ||  m_uHitTest == HTNAIL || m_uHitTest ==HTSYSMENU ||
		uHitTest == HTCLOSE || uHitTest == HTMAXBUTTON || uHitTest == HTMINBUTTON ||	uHitTest == HTHELP ||  uHitTest == HTNAIL || uHitTest ==HTSYSMENU ))
		PaintFrame();	

	return m_uHitTest;
}
void CTPBaseFrame::SetNail(BOOL bNail)
{
	if(bNail)	m_uBoxAmount |=  BOX_NAIL;
	else        m_uBoxAmount &= ~BOX_NAIL;
}
void CTPBaseFrame::SetCaptionBar(BOOL bBar)
{
	/*	if(bBar)	m_uBoxAmount |= CAPTION_BAR;
	else        m_uBoxAmount &= ~CAPTION_BAR;*/
}
void CTPBaseFrame::SetFlex(int iScaleWidth,int iScaleHeight,int iSrcLeft,int iSrcTop,int iSrcRight,int iSrcBottom)
{
	m_iScaleWidth  = iScaleWidth; 
	m_iScaleHeight = iScaleHeight;
	m_rtSrcScale   = CRect(iSrcLeft,iSrcTop,iSrcRight,iSrcBottom); 
}
void CTPBaseFrame::SetFlexWidth(int iWidth)
{	
	if(m_iScaleHeight == 0 || m_iScaleWidth == 0) return;
	CRect rtWnd,rtClient,rtTemp;
	CWnd *pWnd = CWnd::FromHandle(m_hBaseWnd);
	pWnd->GetWindowRect(&rtWnd);
	pWnd->GetClientRect(&rtClient);
	pWnd->ClientToScreen(&rtClient);
	rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
	rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
	rtTemp = rtWnd;
	rtTemp.right  = rtTemp.left + rtWnd.Width() - rtClient.Width()   +  m_rtSrcScale.left + m_rtSrcScale.right  + iWidth;
	rtTemp.bottom = rtTemp.top  + rtWnd.Height() - rtClient.Height() +  m_rtSrcScale.top  + m_rtSrcScale.bottom + iWidth * m_iScaleHeight / m_iScaleWidth;
	pWnd->SetWindowPos(NULL,0,0,rtTemp.Width(),rtTemp.Height(),SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);	
}
void CTPBaseFrame::GetFlexRect(CRect &rtFlex)
{
	CWnd *pWnd = CWnd::FromHandle(m_hBaseWnd);
	pWnd ->GetClientRect(&rtFlex);
	rtFlex.left   += m_rtSrcScale.left;
	rtFlex.right  -= m_rtSrcScale.right;
	rtFlex.top    += m_rtSrcScale.top;
	rtFlex.bottom -= m_rtSrcScale.bottom;
}
void CTPBaseFrame::SetChildWindow(CWnd *pWnd,TPSizeMode eModeX,TPSizeMode eModeY)
{
	if(m_pCtrlSize)	m_pCtrlSize ->AddChildWindow(pWnd,eModeX,eModeY);
}
void CTPBaseFrame::EnableChildWindow(CWnd *pWnd,BOOL bEnableSize)
{
	if(m_pCtrlSize)	m_pCtrlSize ->EnableChildWindow(pWnd,bEnableSize);
}
void CTPBaseFrame::SetExtraSize(int iExLeft,int iExTop,int iExRight,int iExBottom)
{
	if(m_pCtrlSize)	m_pCtrlSize ->SetExtraSize(iExLeft,iExTop,iExRight,iExBottom);
}
void CTPBaseFrame::SetAutoSize(BOOL bAutoSize,TPControlData *pCtrlData,int iCtrlCount)
{
	if(bAutoSize)
	{
		if(m_pCtrlSize == NULL) m_pCtrlSize = new CTPSizeCtrl();
		m_pCtrlSize ->InitWindow(CWnd::FromHandle(m_hBaseWnd));
		if(iCtrlCount >0)
		{
			m_pCtrlSize ->SetCotrolData(pCtrlData,iCtrlCount);
			m_pCtrlSize ->OnSize();
		}
	}
	else
	{
		if(m_pCtrlSize)
		{
			m_pCtrlSize ->RemoveAllWnd();
			delete m_pCtrlSize;
			m_pCtrlSize = NULL;
		}
	}
}
#ifdef TP_NS_EDIT
BOOL TP_Include(int iSr1,int iSr2,int iDes1,int iDes2)
{
	if(iDes1>iSr2)      return FALSE;
	else if(iDes2<iSr1) return FALSE;
	else                return TRUE;
}
void CTPBaseFrame::OnMoving(CRect *rtWndSet)
{
#ifdef TP_NS_NET
	return;
#endif
	if(m_uFrameType == FRAME_SV || m_uFrameType == FRAME_MV) return;//hzh
	int iLeft1 = 0x7ffffff,iTop1 = 0x7ffffff,iRigth1 = 0x7ffffff,iBottom1 = 0x7ffffff;
	int iLeft2 = 0x7ffffff,iTop2 = 0x7ffffff,iRigth2 = 0x7ffffff,iBottom2 = 0x7ffffff;	
	CArray<CRect,CRect&> aWndArray;
	CRect      rtWndGet;
	//	aWndArray.Add(m_rtScreen);	
	CPoint     ptCursor;
	GetCursorPos(&ptCursor);

	for(INT l=0;l<m_aBaseFrame.GetSize();l++)
	{
		if(m_aBaseFrame[l] == this)                                       continue;
		if(m_aBaseFrame[l] ->m_bChild)                                    continue;
		if(m_aBaseFrame[l] ->m_uState & STATE_INVISIBLE)                  continue;
		if(!::IsWindow(m_aBaseFrame[l]->m_hBaseWnd))                      continue;
		if(!::IsWindowVisible(m_aBaseFrame[l]->m_hBaseWnd))                     continue;
		if(!(GetWindowLong(m_aBaseFrame[l]->m_hBaseWnd, GWL_STYLE)&WS_CAPTION)) continue;		
		if(m_aBaseFrame[l] ->m_uFrameType == FRAME_SV || m_aBaseFrame[l] ->m_uFrameType == FRAME_MV) continue;
		m_aBaseFrame[l]->m_uState &= ~STATE_LBUTTONDOWN;
		CWnd::FromHandle(m_aBaseFrame[l]->m_hBaseWnd) ->GetWindowRect(&rtWndGet);
		if(rtWndGet == *rtWndSet)                                               continue;
		aWndArray.Add(rtWndGet);
	}
	rtWndGet = *rtWndSet;
	for(INT l=0;l<aWndArray.GetSize();l++)
	{
		if(abs(rtWndGet.left  - aWndArray[l].left)  <iLeft1  && TP_Include(rtWndGet.top,rtWndGet.bottom,aWndArray[l].top,aWndArray[l].bottom))   {iLeft1   = abs(rtWndGet.left  - aWndArray[l].left);   iLeft2   = aWndArray[l].left;}
		if(abs(rtWndGet.left  - aWndArray[l].right)  <iLeft1 && TP_Include(rtWndGet.top,rtWndGet.bottom,aWndArray[l].top,aWndArray[l].bottom))   {iLeft1   = abs(rtWndGet.left  - aWndArray[l].right);  iLeft2   = aWndArray[l].right;}

		if(abs(rtWndGet.right - aWndArray[l].right) <iRigth1 && TP_Include(rtWndGet.top,rtWndGet.bottom,aWndArray[l].top,aWndArray[l].bottom))   {iRigth1  = abs(rtWndGet.right - aWndArray[l].right);  iRigth2  = aWndArray[l].right;}
		if(abs(rtWndGet.right - aWndArray[l].left) <iRigth1  && TP_Include(rtWndGet.top,rtWndGet.bottom,aWndArray[l].top,aWndArray[l].bottom))   {iRigth1  = abs(rtWndGet.right - aWndArray[l].left);   iRigth2  = aWndArray[l].left;}

		if(abs(rtWndGet.top   - aWndArray[l].top)   <iTop1    && TP_Include(rtWndGet.left,rtWndGet.right,aWndArray[l].left,aWndArray[l].right))  {iTop1    = abs(rtWndGet.top   - aWndArray[l].top);    iTop2    = aWndArray[l].top;}
		if(abs(rtWndGet.top   - aWndArray[l].bottom)   <iTop1 && TP_Include(rtWndGet.left,rtWndGet.right,aWndArray[l].left,aWndArray[l].right))  {iTop1    = abs(rtWndGet.top   - aWndArray[l].bottom);    iTop2    = aWndArray[l].bottom;}

		if(abs(rtWndGet.bottom- aWndArray[l].bottom)<iBottom1 && TP_Include(rtWndGet.left,rtWndGet.right,aWndArray[l].left,aWndArray[l].right))  {iBottom1 = abs(rtWndGet.bottom- aWndArray[l].bottom); iBottom2 = aWndArray[l].bottom;}
		if(abs(rtWndGet.bottom- aWndArray[l].top)<iBottom1    && TP_Include(rtWndGet.left,rtWndGet.right,aWndArray[l].left,aWndArray[l].right))  {iBottom1 = abs(rtWndGet.bottom- aWndArray[l].top); iBottom2 = aWndArray[l].top;}
	}

	if(m_rtBeforeMov.left == TRUE && abs(m_rtMoving.left - ptCursor.x)>10)
	{
		iLeft1  =1000;
		iRigth1 =1000;
		m_rtBeforeMov.left = FALSE;		  
		rtWndSet->OffsetRect((ptCursor.x - m_rtMoving.right) - rtWndSet->left ,0);
	}
	else if(m_rtBeforeMov.right == TRUE && abs(m_rtMoving.left - ptCursor.x)>10)
	{
		iLeft1  =1000;
		iRigth1 =1000;
		m_rtBeforeMov.right = FALSE;
		rtWndSet->OffsetRect((ptCursor.x - m_rtMoving.right) - rtWndSet->left ,0);
	}
	if(m_rtBeforeMov.top == TRUE && abs(m_rtMoving.top - ptCursor.y)>10)
	{
		iTop1    = 1000;
		iBottom1 = 1000;
		m_rtBeforeMov.top = FALSE;
		rtWndSet->OffsetRect(0,(ptCursor.y - m_rtMoving.bottom) - rtWndSet->top);		
	}
	else if(m_rtBeforeMov.bottom == TRUE && abs(m_rtMoving.top - ptCursor.y)>10)
	{
		iBottom1 = 1000;
		iTop1    = 1000;
		m_rtBeforeMov.bottom = FALSE;
		rtWndSet->OffsetRect(0,(ptCursor.y - m_rtMoving.bottom) - rtWndSet->top);		
	}
	if(iLeft1<10 && iLeft1>0)         { if(m_rtBeforeMov.left == FALSE)  { m_rtMoving.left = ptCursor.x; m_rtMoving.right =ptCursor.x-rtWndSet->left; }   rtWndSet->OffsetRect(iLeft2  - rtWndSet->left,0);      m_rtBeforeMov.left   = TRUE;}
	else if(iRigth1<10 && iRigth1>0)  { if(m_rtBeforeMov.right == FALSE) { m_rtMoving.left = ptCursor.x; m_rtMoving.right =ptCursor.x-rtWndSet->left; }   rtWndSet->OffsetRect(iRigth2 - rtWndSet->right,0);     m_rtBeforeMov.right  = TRUE;}
	if(iTop1<10&&iTop1>0)             { if(m_rtBeforeMov.top == FALSE)   { m_rtMoving.top = ptCursor.y;  m_rtMoving.bottom =ptCursor.y-rtWndSet->top; }   rtWndSet->OffsetRect(0,iTop2 - rtWndSet->top);         m_rtBeforeMov.top    = TRUE;}
	else if(iBottom1<10&&iBottom1>0)  { if(m_rtBeforeMov.bottom == FALSE){ m_rtMoving.top = ptCursor.y;  m_rtMoving.bottom =ptCursor.y-rtWndSet->top; }   rtWndSet->OffsetRect(0,iBottom2 - rtWndSet->bottom);   m_rtBeforeMov.bottom = TRUE;}

	/*	if(rtWndSet->left>m_rtScreen.right-10) rtWndSet->left=m_rtScreen.right-10;
	if(rtWndSet->top>m_rtScreen.bottom-10) rtWndSet->top=m_rtScreen.bottom-10;
	if(rtWndSet->right<m_rtScreen.left+10) rtWndSet->right=m_rtScreen.left+10;
	if(rtWndSet->bottom<m_rtScreen.top+10) rtWndSet->bottom=m_rtScreen.top+10;*/
}
void CTPBaseFrame::OnMove()
{	
}
void CTPBaseFrame::OnSizeEx()
{
#ifdef TP_NS_NET
	return;
#endif
	if(m_uFrameType == FRAME_SV || m_uFrameType == FRAME_MV) return;//hzh
	CArray<CRect,CRect&> aWndArray;
	CArray<CTPBaseFrame *,CTPBaseFrame*&>   hWndArray;	
	CRect      rtWndGet;	

	for(INT l=0;l<m_aBaseFrame.GetSize();l++)
	{
		if(m_aBaseFrame[l] == this)                                             continue;
		if(m_aBaseFrame[l] ->m_uState & STATE_INVISIBLE)                        continue;
		if(!::IsWindow(m_aBaseFrame[l]->m_hBaseWnd))                            continue;
		if(!::IsWindowVisible(m_aBaseFrame[l]->m_hBaseWnd))                     continue;
		if(!(GetWindowLong(m_aBaseFrame[l]->m_hBaseWnd, GWL_STYLE)&WS_CAPTION)) continue;		
		if(m_aBaseFrame[l] ->m_uFrameType == FRAME_SV || m_aBaseFrame[l] ->m_uFrameType == FRAME_MV) continue;
		CWnd::FromHandle(m_aBaseFrame[l]->m_hBaseWnd) ->GetWindowRect(&rtWndGet);	
		m_aBaseFrame[l]->m_uState &= ~STATE_LBUTTONDOWN;
		aWndArray.Add(rtWndGet);
		hWndArray.Add(m_aBaseFrame[l]);
	}	
	GetWindowRect(m_hBaseWnd,rtWndGet);
	for(INT l=0;l<aWndArray.GetSize();l++)
	{
		if(abs(m_rtSize.left  - aWndArray[l].left) <=1  && TP_Include(m_rtSize.top,m_rtSize.bottom,aWndArray[l].top,aWndArray[l].bottom) && rtWndGet.left != m_rtSize.left)  
		{
			CRect      rtWnd;
			::GetWindowRect(hWndArray[l]->m_hBaseWnd,rtWnd);
			rtWnd.left += rtWndGet.left - m_rtSize.left;
			hWndArray[l]->m_uState |= STATE_NOSIZEEX;
			::MoveWindow(hWndArray[l]->m_hBaseWnd,rtWnd.left,rtWnd.top,rtWnd.Width(),rtWnd.Height(),TRUE);
			hWndArray[l]->m_uState &= ~STATE_NOSIZEEX;
		}
		else if(abs(m_rtSize.left  - aWndArray[l].right) <= 1 && TP_Include(m_rtSize.top,m_rtSize.bottom,aWndArray[l].top,aWndArray[l].bottom) && rtWndGet.left != m_rtSize.left) 
		{
			CRect      rtWnd;
			::GetWindowRect(hWndArray[l]->m_hBaseWnd,rtWnd);
			rtWnd.right += rtWndGet.left - m_rtSize.left;
			hWndArray[l]->m_uState |= STATE_NOSIZEEX;
			::MoveWindow(hWndArray[l]->m_hBaseWnd,rtWnd.left,rtWnd.top,rtWnd.Width(),rtWnd.Height(),TRUE);
			hWndArray[l]->m_uState &= ~STATE_NOSIZEEX;
		}
		else if(abs(m_rtSize.right - aWndArray[l].right) <=1 && TP_Include(m_rtSize.top,m_rtSize.bottom,aWndArray[l].top,aWndArray[l].bottom) && rtWndGet.right != m_rtSize.right)  
		{
			CRect      rtWnd;
			::GetWindowRect(hWndArray[l]->m_hBaseWnd,rtWnd);
			rtWnd.right += rtWndGet.right - m_rtSize.right;
			hWndArray[l]->m_uState |= STATE_NOSIZEEX;
			::MoveWindow(hWndArray[l]->m_hBaseWnd,rtWnd.left,rtWnd.top,rtWnd.Width(),rtWnd.Height(),TRUE);
			hWndArray[l]->m_uState &= ~STATE_NOSIZEEX;
		}
		else if(abs(m_rtSize.right - aWndArray[l].left) <=1  && TP_Include(m_rtSize.top,m_rtSize.bottom,aWndArray[l].top,aWndArray[l].bottom)&& rtWndGet.right != m_rtSize.right) 
		{
			CRect      rtWnd;
			::GetWindowRect(hWndArray[l]->m_hBaseWnd,rtWnd);
			rtWnd.left += rtWndGet.right - m_rtSize.right;
			hWndArray[l]->m_uState |= STATE_NOSIZEEX;
			::MoveWindow(hWndArray[l]->m_hBaseWnd,rtWnd.left,rtWnd.top,rtWnd.Width(),rtWnd.Height(),TRUE);
			hWndArray[l]->m_uState &= ~STATE_NOSIZEEX;
		}

		if(abs(m_rtSize.top   - aWndArray[l].top)   <=1    && TP_Include(m_rtSize.left,m_rtSize.right,aWndArray[l].left,aWndArray[l].right)&& rtWndGet.top != m_rtSize.top)
		{
			CRect      rtWnd;
			::GetWindowRect(hWndArray[l]->m_hBaseWnd,rtWnd);
			rtWnd.top += rtWndGet.top - m_rtSize.top;
			hWndArray[l]->m_uState |= STATE_NOSIZEEX;
			::MoveWindow(hWndArray[l]->m_hBaseWnd,rtWnd.left,rtWnd.top,rtWnd.Width(),rtWnd.Height(),TRUE);
			hWndArray[l]->m_uState &= ~STATE_NOSIZEEX;
		}
		else if(abs(m_rtSize.top   - aWndArray[l].bottom)  <=1 && TP_Include(m_rtSize.left,m_rtSize.right,aWndArray[l].left,aWndArray[l].right)&& rtWndGet.top != m_rtSize.top)
		{
			CRect      rtWnd;
			::GetWindowRect(hWndArray[l]->m_hBaseWnd,rtWnd);
			rtWnd.bottom += rtWndGet.top - m_rtSize.top;
			hWndArray[l]->m_uState |= STATE_NOSIZEEX;
			::MoveWindow(hWndArray[l]->m_hBaseWnd,rtWnd.left,rtWnd.top,rtWnd.Width(),rtWnd.Height(),TRUE);
			hWndArray[l]->m_uState &= ~STATE_NOSIZEEX;
		}
		else if(abs(m_rtSize.bottom- aWndArray[l].bottom) <=1 && TP_Include(m_rtSize.left,m_rtSize.right,aWndArray[l].left,aWndArray[l].right)&& rtWndGet.bottom != m_rtSize.bottom)
		{
			CRect      rtWnd;
			::GetWindowRect(hWndArray[l]->m_hBaseWnd,rtWnd);
			rtWnd.bottom += rtWndGet.bottom - m_rtSize.bottom;
			hWndArray[l]->m_uState |= STATE_NOSIZEEX;
			::MoveWindow(hWndArray[l]->m_hBaseWnd,rtWnd.left,rtWnd.top,rtWnd.Width(),rtWnd.Height(),TRUE);
			hWndArray[l]->m_uState &= ~STATE_NOSIZEEX;
		}
		else if(abs(m_rtSize.bottom- aWndArray[l].top)  <=1    && TP_Include(m_rtSize.left,m_rtSize.right,aWndArray[l].left,aWndArray[l].right)&& rtWndGet.bottom != m_rtSize.bottom)
		{
			CRect      rtWnd;
			::GetWindowRect(hWndArray[l]->m_hBaseWnd,rtWnd);
			rtWnd.top += rtWndGet.bottom - m_rtSize.bottom;
			hWndArray[l]->m_uState |= STATE_NOSIZEEX;
			::MoveWindow(hWndArray[l]->m_hBaseWnd,rtWnd.left,rtWnd.top,rtWnd.Width(),rtWnd.Height(),TRUE);
			hWndArray[l]->m_uState &= ~STATE_NOSIZEEX;
		}
	}

}
void CTPBaseFrame::OnSizingEx(UINT uType,CRect *rtWndSet)
{
#ifdef TP_NS_NET
	return;
#endif
	if(m_uFrameType == FRAME_SV || m_uFrameType == FRAME_MV) return;//hzh
	int iLeft1 = 0x7ffffff,iTop1 = 0x7ffffff,iRigth1 = 0x7ffffff,iBottom1 = 0x7ffffff;
	int iLeft2 = 0x7ffffff,iTop2 = 0x7ffffff,iRigth2 = 0x7ffffff,iBottom2 = 0x7ffffff;	
	CArray<CRect,CRect&> aWndArray;
	CRect      rtWndGet;
	//aWndArray.Add(m_rtScreen);	
	CPoint     ptCursor;
	GetCursorPos(&ptCursor);

	for(INT l=0;l<m_aBaseFrame.GetSize();l++)
	{
		if(m_aBaseFrame[l] == this)                                       continue;
		if(m_aBaseFrame[l] ->m_uState & STATE_INVISIBLE)                  continue;
		if(!::IsWindow(m_aBaseFrame[l]->m_hBaseWnd))                      continue;
		if(!::IsWindowVisible(m_aBaseFrame[l]->m_hBaseWnd))                     continue;
		if(!(GetWindowLong(m_aBaseFrame[l]->m_hBaseWnd, GWL_STYLE)&WS_CAPTION)) continue;		
		CWnd::FromHandle(m_aBaseFrame[l]->m_hBaseWnd) ->GetWindowRect(&rtWndGet);
		m_aBaseFrame[l]->m_uState &= ~STATE_LBUTTONDOWN;
		if(rtWndGet == *rtWndSet)                                               continue;
		aWndArray.Add(rtWndGet);
	}
	rtWndGet = *rtWndSet;
	for(INT l=0;l<aWndArray.GetSize();l++)
	{
		if(abs(rtWndGet.left  - aWndArray[l].left)  <iLeft1  && TP_Include(rtWndGet.top,rtWndGet.bottom,aWndArray[l].top,aWndArray[l].bottom))   {iLeft1   = abs(rtWndGet.left  - aWndArray[l].left);   iLeft2   = aWndArray[l].left;}
		if(abs(rtWndGet.left  - aWndArray[l].right)  <iLeft1 && TP_Include(rtWndGet.top,rtWndGet.bottom,aWndArray[l].top,aWndArray[l].bottom))   {iLeft1   = abs(rtWndGet.left  - aWndArray[l].right);  iLeft2   = aWndArray[l].right;}

		if(abs(rtWndGet.right - aWndArray[l].right) <iRigth1 && TP_Include(rtWndGet.top,rtWndGet.bottom,aWndArray[l].top,aWndArray[l].bottom))   {iRigth1  = abs(rtWndGet.right - aWndArray[l].right);  iRigth2  = aWndArray[l].right;}
		if(abs(rtWndGet.right - aWndArray[l].left) <iRigth1  && TP_Include(rtWndGet.top,rtWndGet.bottom,aWndArray[l].top,aWndArray[l].bottom))   {iRigth1  = abs(rtWndGet.right - aWndArray[l].left);   iRigth2  = aWndArray[l].left;}

		if(abs(rtWndGet.top   - aWndArray[l].top)   <iTop1    && TP_Include(rtWndGet.left,rtWndGet.right,aWndArray[l].left,aWndArray[l].right))  {iTop1    = abs(rtWndGet.top   - aWndArray[l].top);    iTop2    = aWndArray[l].top;}
		if(abs(rtWndGet.top   - aWndArray[l].bottom)   <iTop1 && TP_Include(rtWndGet.left,rtWndGet.right,aWndArray[l].left,aWndArray[l].right))  {iTop1    = abs(rtWndGet.top   - aWndArray[l].bottom);    iTop2    = aWndArray[l].bottom;}

		if(abs(rtWndGet.bottom- aWndArray[l].bottom)<iBottom1 && TP_Include(rtWndGet.left,rtWndGet.right,aWndArray[l].left,aWndArray[l].right))  {iBottom1 = abs(rtWndGet.bottom- aWndArray[l].bottom); iBottom2 = aWndArray[l].bottom;}
		if(abs(rtWndGet.bottom- aWndArray[l].top)<iBottom1    && TP_Include(rtWndGet.left,rtWndGet.right,aWndArray[l].left,aWndArray[l].right))  {iBottom1 = abs(rtWndGet.bottom- aWndArray[l].top); iBottom2 = aWndArray[l].top;}
	}

	if(uType != WMSZ_LEFT && uType != WMSZ_TOPLEFT   && uType != WMSZ_BOTTOMLEFT)      iLeft1  =1000;
	if(uType != WMSZ_RIGHT && uType != WMSZ_TOPRIGHT && uType != WMSZ_BOTTOMRIGHT)     iRigth1 =1000;
	if(uType != WMSZ_TOP && uType != WMSZ_TOPLEFT   && uType != WMSZ_TOPRIGHT)         iTop1   =1000;
	if(uType != WMSZ_BOTTOM && uType != WMSZ_BOTTOMLEFT && uType != WMSZ_BOTTOMRIGHT)  iBottom1=1000;

	if(m_rtBeforeMov.left == TRUE && abs(m_rtMoving.left - ptCursor.x)>10)
	{
		iLeft1 =1000;
		m_rtBeforeMov.left = FALSE;
		rtWndSet->left +=ptCursor.x  - m_rtMoving.left;
	}
	else if(m_rtBeforeMov.right == TRUE && abs(m_rtMoving.left - ptCursor.x)>10)
	{
		iRigth1 =1000;
		m_rtBeforeMov.right = FALSE;
		rtWndSet->right += ptCursor.x  - m_rtMoving.left;
	}
	if(m_rtBeforeMov.top == TRUE && abs(m_rtMoving.top - ptCursor.y)>10)
	{
		iTop1 = 1000;
		m_rtBeforeMov.top = FALSE;
		rtWndSet->top += ptCursor.y  - m_rtMoving.top;
	}
	else if(m_rtBeforeMov.bottom == TRUE && abs(m_rtMoving.top - ptCursor.y)>10)
	{
		iBottom1 = 1000;
		m_rtBeforeMov.bottom = FALSE;
		rtWndSet->bottom += ptCursor.y  - m_rtMoving.top;
	}
	if(iLeft1<10)        { if(m_rtBeforeMov.left == FALSE)   m_rtMoving.left = ptCursor.x+iLeft2  - rtWndSet->left;     rtWndSet->left   +=iLeft2  - rtWndSet->left-1;     m_rtBeforeMov.left   = TRUE;}
	else if(iRigth1<10)  { if(m_rtBeforeMov.right == FALSE)  m_rtMoving.left = ptCursor.x+iRigth2 - rtWndSet->right;    rtWndSet->right  +=iRigth2 - rtWndSet->right+1;    m_rtBeforeMov.right  = TRUE;}
	if(iTop1<10)         { if(m_rtBeforeMov.top == FALSE)    m_rtMoving.top = ptCursor.y+iTop2 - rtWndSet->top;         rtWndSet->top    +=iTop2 - rtWndSet->top-1;        m_rtBeforeMov.top    = TRUE;}
	else if(iBottom1<10) { if(m_rtBeforeMov.bottom == FALSE) m_rtMoving.top = ptCursor.y+iBottom2 - rtWndSet->bottom;   rtWndSet->bottom +=iBottom2 - rtWndSet->bottom+1;  m_rtBeforeMov.bottom = TRUE;}

}
#else
void CTPBaseFrame::OnMoving(CRect *rtWnd)
{
	if(m_hDockParentWnd == NULL) return;

	CRect rectParent,rectChild = *rtWnd;

	CWnd  *pParentWnd = CWnd::FromHandle(m_hDockParentWnd);
	CWnd  *pThisWnd   = CWnd::FromHandle(m_hBaseWnd);
	WORD   wDockX     = 0;
	WORD   wDockY     = 0;

	pParentWnd->GetWindowRect(&rectParent); 
	if(abs(rectChild.left-rectParent.right) < DOCK_DISTANCE  )
	{
		rectChild.OffsetRect(rectParent.right-rectChild.left,0); 			
		wDockX = DOCK_1;
	}
	else if(abs(rectChild.right-rectParent.left) < DOCK_DISTANCE  )
	{
		rectChild.OffsetRect(rectParent.left-rectChild.right,0); 
		wDockX = DOCK_2;
	}
	else if(abs(rectChild.right-rectParent.right ) < DOCK_DISTANCE  )
	{
		rectChild.OffsetRect(rectParent.right - rectChild.right,0); 
		wDockX =DOCK_3;		
	}
	else if(abs(rectChild.left-rectParent.left) < DOCK_DISTANCE  )
	{
		rectChild.OffsetRect(rectParent.left- rectChild.left,0); 
		wDockX =DOCK_4;

	}
	else
	{
		wDockX =DOCK_0;			
	}
	if(rectChild.bottom < rectParent.top - DOCK_DISTANCE || rectChild.top > rectParent.bottom + DOCK_DISTANCE)
		wDockX =DOCK_0;			

	if(abs(rectChild.top-rectParent.bottom) < DOCK_DISTANCE  )
	{
		rectChild.OffsetRect(0,rectParent.bottom - rectChild.top); 
		wDockY =DOCK_1;			
	}
	else if(abs(rectChild.bottom -rectParent.top) < DOCK_DISTANCE  )
	{
		rectChild.OffsetRect(0,rectParent.top - rectChild.bottom); 
		wDockY =DOCK_2;

	}
	else if(abs(rectChild.top -rectParent.top ) < DOCK_DISTANCE  )
	{
		rectChild.OffsetRect(0,rectParent.top - rectChild.top ); 
		wDockY =DOCK_3;

	}
	else if(abs(rectChild.bottom-rectParent.bottom) < DOCK_DISTANCE  )
	{
		rectChild.OffsetRect(0,rectParent.bottom - rectChild.bottom); 
		wDockY =DOCK_4;

	}
	else
	{
		wDockY =DOCK_0;

	}
	if(rectChild.right < rectParent.left - DOCK_DISTANCE || rectChild.left > rectParent.right + DOCK_DISTANCE)
		wDockY =DOCK_0;			

	m_dwDockState = MAKELONG(wDockX,wDockY);
	*rtWnd        = rectChild;
}
void CTPBaseFrame::OnMove()
{
#ifdef TP_NS_NET
	return;
#endif
	if(m_hDockParentWnd == NULL) //父窗口
	{
		CRect rectParent,rectChild;
		CWnd  *pParentWnd = CWnd::FromHandle(m_hBaseWnd);
		CWnd  *pChildWnd  = NULL;
		pParentWnd->GetWindowRect(&rectParent);
		for(INT l=0;l<m_aDockWnd.GetSize();l++)
		{
			pChildWnd = CWnd::FromHandle(m_aDockWnd[l] .hChildWnd);
			pChildWnd->GetWindowRect(&rectChild); 

			switch(m_aDockWnd[l].wDockX)
			{
			case DOCK_1:
				rectChild.OffsetRect(rectParent.right - rectChild.left,0); 
				break;
			case DOCK_2:
				rectChild.OffsetRect(rectParent.left-rectChild.right,0); 
				break;
			case DOCK_3:
				rectChild.OffsetRect(rectParent.right - rectChild.right,0); 
				break;
			case DOCK_4:
				rectChild.OffsetRect(rectParent.left- rectChild.left,0); 
				break;
			}

			switch(m_aDockWnd[l].wDockY)
			{
			case DOCK_1:
				rectChild.OffsetRect(0,rectParent.bottom - rectChild.top); 
				break;
			case DOCK_2:
				rectChild.OffsetRect(0,rectParent.top - rectChild.bottom); 
				break;
			case DOCK_3:
				rectChild.OffsetRect(0,rectParent.top - rectChild.top ); 
				break;
			case DOCK_4:
				rectChild.OffsetRect(0,rectParent.bottom - rectChild.bottom); 
				break;
			}

			if(m_aDockWnd[l].wDockX !=DOCK_0 || m_aDockWnd[l].wDockY !=DOCK_0)
				pChildWnd->MoveWindow(rectChild); 
		}

	}
	else
	{
		SendMessage(m_hDockParentWnd,WM_DOCKPARENTWND,(WPARAM)m_dwDockState,(LPARAM)CWnd::FromHandle(m_hBaseWnd));
	}
}
void CTPBaseFrame::OnSizeEx()
{	
}
void CTPBaseFrame::OnSizingEx(UINT uType,CRect *rtWnd)
{
}
#endif
void CTPBaseFrame::SetDockParentWnd(CWnd *pParentWnd)
{
	m_hDockParentWnd = pParentWnd ->m_hWnd;
}
BOOL CTPBaseFrame::PaintFrame(CDC *pDC,CRect rtWnd)
{
	return FALSE;
}
void  CTPBaseFrame::SetTips(CString sTips)
{
	m_sTips = sTips;
	if(m_hBaseWnd == NULL) return;
	CWnd  *pWnd  = CWnd::FromHandle(m_hBaseWnd);
	CDC   *pDC   = pWnd->GetWindowDC();	
	CTPDCExchange stuTemp(pDC);
	CTPMemoDC memDC(pDC,m_rtBotton.Width(),m_rtBotton.Height());
	if(BOX_BORDER & m_uBoxAmount)
	{	
		int iIndex  = 0;
		if(m_uState&STATE_ACTIVATE) iIndex = BORDER_BOTTOM_A;
		else                        iIndex = BORDER_BOTTOM_N;
		CRect rtTemp = m_rtBotton;rtTemp.OffsetRect(0,-rtTemp.top);
		TP_StretchDIBRect(&memDC,m_pResources[iIndex]->GetByte(),m_pResources[iIndex]->Width(),m_pResources[iIndex]->Height(),10,10,rtTemp,FALSE); 
		rtTemp.DeflateRect(20,0,0,0);
		memDC.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
		if(m_sTips.GetLength()>0) memDC.DrawText(m_sTips,rtTemp,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
	pDC ->BitBlt(0,m_rtBotton.top,m_rtBotton.Width(),m_rtBotton.Height(),&memDC,0,0,SRCCOPY);	
	stuTemp.ReleaseDC();
	pWnd->ReleaseDC(pDC);

}
BOOL CTPBaseFrame::PushPinProc(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	return FALSE;
}
CRect  CTPBaseFrame::GetFrameEdge(HWND hWnd)
{
	if(m_rtEdge.top+m_rtEdge.bottom>10) 
		return m_rtEdge;
	else
	{
		CSize szEdge;
		CRect rtWnd,rtClient,rtTemp;
		if(hWnd == NULL) hWnd = m_hBaseWnd;
		CWnd *pWnd = CWnd::FromHandle(hWnd);
		pWnd->GetWindowRect(&rtWnd);
		pWnd->GetClientRect(&rtClient);
		pWnd->ClientToScreen(&rtClient);
		rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
		rtWnd.left = rtClient.left - rtWnd.left;
		rtWnd.top  = rtClient.top - rtWnd.top;
		rtWnd.right = rtWnd.right - rtClient.right;
		rtWnd.bottom = rtWnd.bottom - rtClient.bottom;

		m_rtEdge = rtWnd;
		return rtWnd;
	}
}
BOOL  CTPBaseFrame::DoSysMenu(CPoint &ptScreen)
{
	return FALSE;
}
void  CTPBaseFrame::ModifyBoxAmount(UINT uAdd,UINT uRemove)
{
	m_uBoxAmount &= ~uRemove;
	m_uBoxAmount |=  uAdd;
	if(m_hBaseWnd)
	{
		FunSize();
		PaintFrame();
	}
}

BOOL CTPBaseFrame::PreTranslateMessageEx(MSG* pMsg)
{
	// TODO: 在此铁赜专用代聛E?或调用基纴E
	if(m_bNoMsg) return FALSE;
#ifdef TP_NS_EDIT
	if(pMsg->message == WM_SYSKEYDOWN && ((pMsg->wParam == VK_F4 && TP_IsAltKeyDown()) || pMsg->wParam == VK_SPACE))
		return TRUE;
	else if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_F1 || TP_IsCtrlHotKeyHelp((UINT)pMsg->wParam)))
	{
		::GetCursorPos(&m_stuHelpInfo.MousePos);
		if(m_bChild)
		{
			CWnd *pWnd = CWnd::FromHandle(m_hBaseWnd);
			PostMessage(pWnd->GetParentOwner()->GetSafeHwnd(),  WM_HELPEX,0,(LPARAM)&m_stuHelpInfo);
		}
		else          PostMessage(m_hBaseWnd, WM_HELPEX,0,(LPARAM)&m_stuHelpInfo);
		//AfxMessageBox(_T("Help Wait !!!"));
		return TRUE;
	}
	else if(pMsg ->message == WM_KEYDOWN && (pMsg->wParam == VK_W && TP_IsCtrlKeyDown() || m_uFrameLevel == 2 && pMsg->wParam == VK_ESCAPE))
	{
		if((FRAME_STYLE_NOESC & m_uFrameStyle) && pMsg->wParam == VK_ESCAPE) return FALSE;
		else if(m_bChild)
		{
			CWnd *pWnd = CWnd::FromHandle(m_hBaseWnd);
			PostMessage(pWnd->GetParentOwner()->GetSafeHwnd(), WM_CLOSE,0,0);
		}
		else 		      PostMessage(m_hBaseWnd, WM_CLOSE,0,0);
		return TRUE;
	}		
	else if(pMsg ->message == WM_SYSKEYDOWN &&  (pMsg->wParam == VK_LWIN ||  pMsg->wParam == VK_RWIN)  && TP_IsAltKeyDown())
	{
		if(!m_bChild)
		{				
			if(m_bToolTips >0)
			{	
				KillTimer(m_hBaseWnd,1123);
				m_pToolTip ->ShowWindow(SW_HIDE);
			}
			GetWindowRect(m_hBaseWnd, &m_rtMenuBox);
			m_rtMenuBox.OffsetRect(m_rtSysMenuBox.left,m_rtSysMenuBox.top);
			m_rtMenuBox.right = m_rtMenuBox.left + m_rtSysMenuBox.Width();
			m_rtMenuBox.bottom = m_rtMenuBox.top + m_rtSysMenuBox.Height();
			g_hBaseWndBack = m_hBaseWnd;
			ShowSysMenu();
			if(!::IsWindow(g_hBaseWndBack)) return TRUE;			
			m_rtMenuBox  = CRect(0,0,0,0);
			return TRUE;
		}
		else 
		{
			CTPBaseFrame *pBaseFrame = GetBaseFrame(CWnd::FromHandle(GetParent(m_hBaseWnd)));
			if(pBaseFrame) return pBaseFrame->PreTranslateMessageEx(pMsg);
		}
	}	
	else if(pMsg ->message == WM_KEYDOWN && pMsg ->wParam == VK_TAB && m_aForceOrder.GetSize()>0)
	{
		CWnd *pWnd = CWnd::FromHandle(::GetFocus()),*pFocusWnd = NULL;
		pFocusWnd  = pWnd;
NextFocus:
		if(pWnd)
		{
			//if(pWnd ->GetStyle()& BS_AUTORADIOBUTTON) return FALSE;
			if(TP_IsShiftKeyDown())
			{
				UINT uID =pWnd ->GetDlgCtrlID();
				for(INT l=0;l<m_aForceOrder.GetSize();l++)
				{
					if(m_aForceOrder[l].uNextCtrl  == uID)
					{
						pWnd = CWnd::FromHandle(m_aForceOrder[l].pPreWnd) ->GetDlgItem(m_aForceOrder[l].uPreCtrl);
						if(((pWnd ->GetStyle() & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON) && !CWnd::FromHandle(m_aForceOrder[l].pPreWnd) ->IsDlgButtonChecked(m_aForceOrder[l].uPreCtrl))  {uID = m_aForceOrder[l].uPreCtrl;  l=-1;continue;}
						if(pWnd == NULL || !pWnd ->IsWindowEnabled() || !pWnd ->IsWindowVisible()) {uID = m_aForceOrder[l].uPreCtrl;  l=-1;continue;}
						pWnd ->SetFocus();
						m_uState |= STATE_TABORDER;
						return TRUE;
					}
				}
				if(pWnd) pWnd = CWnd::FromHandle(::GetNextDlgTabItem(m_hBaseWnd,pWnd->GetSafeHwnd(),FALSE));
				if(pFocusWnd != pWnd && pWnd &&(!pWnd ->IsWindowEnabled() || !pWnd ->IsWindowVisible()))
					goto NextFocus;
			}
			else if(TP_IsCtrlKeyDown()) return FALSE;
			else
			{			
				UINT uID =pWnd ->GetDlgCtrlID();
				for(INT l=0;l<m_aForceOrder.GetSize();l++)
				{
					if(m_aForceOrder[l].uPreCtrl  == uID)
					{
						pWnd =  CWnd::FromHandle(m_aForceOrder[l].pNextWnd) ->GetDlgItem(m_aForceOrder[l].uNextCtrl);
						if(((pWnd ->GetStyle() & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON) && !CWnd::FromHandle(m_aForceOrder[l].pNextWnd) ->IsDlgButtonChecked(m_aForceOrder[l].uNextCtrl)) {uID = m_aForceOrder[l].uNextCtrl;  continue;}
						if(pWnd == NULL || !pWnd ->IsWindowEnabled() || !pWnd ->IsWindowVisible()) {uID = m_aForceOrder[l].uNextCtrl;  continue;}
						pWnd ->SetFocus();
						m_uState |= STATE_TABORDER;
						return TRUE;
					}
				}
				if(pWnd) pWnd = CWnd::FromHandle(::GetNextDlgTabItem(m_hBaseWnd,pWnd->GetSafeHwnd(),FALSE));
				if(pFocusWnd != pWnd && pWnd &&(!pWnd ->IsWindowEnabled() || !pWnd ->IsWindowVisible()))
					goto NextFocus;
			}
		}
	}
	else if(pMsg ->message == WM_KEYDOWN && pMsg ->wParam == VK_RETURN && (m_uState & STATE_TABORDER))
	{
		CWnd *pWnd = CWnd::FromHandle(::GetFocus()) ;
		if(pWnd)
		{
			TCHAR cClassName[512];ZeroMemory(cClassName,sizeof(cClassName));
			::GetClassName(pWnd ->GetSafeHwnd(),cClassName,512);
			if(lstrcmpi(cClassName,WC_BUTTON) == 0)
			{
				PostMessage(m_hBaseWnd, WM_COMMAND,MAKELONG(pWnd->GetDlgCtrlID(),BN_CLICKED),(LPARAM)pWnd->m_hWnd);
				return TRUE;
			}
		}
	}

#endif
	return FALSE;
}
void  CTPBaseFrame::Destroy(CWnd *pWnd)
{		
	PostMessage(m_hBaseWnd, WM_DELBASEFRAME,(WPARAM)pWnd,0);	
}
void CTPBaseFrame::SetPreActWnd()
{
	if(m_uFrameStyle & FRAME_STYLE_NOACTPRE)     return;
	if(m_bChild)                                 return;
	if(TP_BASESTATE_NOACTPREWND & m_uBaseFlag)   return;
	if(m_uFrameType == FRAME_DIALOG)             return;
	if(m_uFrameType == FRAME_MSGBOX)             return;
	if(m_uFrameType == FRAME_TRIM)               return;

	HWND  hActWnd = ::GetActiveWindow();

	if(m_hActWnd == NULL)                               goto DefaultAct;
	if(m_pCanActWnd && !m_pCanActWnd(m_hActWnd))        goto DefaultAct;
	if(!::IsWindow(m_hActWnd) || ::IsIconic(m_hActWnd)) goto DefaultAct;
	if(hActWnd!=m_hActWnd) ::PostMessage(m_hActWnd, WM_ACTIVATE,WA_ACTIVE,0);	
	return;
DefaultAct:	
	if(m_pCanActWnd)
	{
		if(m_hActPreFrame && m_hActPreFrame != m_hBaseWnd  && m_pCanActWnd(m_hActPreFrame) && ::IsWindow(m_hActPreFrame) && !::IsIconic(m_hActPreFrame)&& ::IsWindowVisible(m_hActPreFrame))
		{
			if(hActWnd!=m_hActPreFrame ) 
				::PostMessage(m_hActPreFrame, WM_ACTIVATE,WA_ACTIVE,0);	
		}
		else if(g_pDisActFrame && g_pDisActFrame!=this && ::IsWindow(g_pDisActFrame ->m_hBaseWnd) &&  !::IsIconic(g_pDisActFrame ->m_hBaseWnd) && m_pCanActWnd(g_pDisActFrame ->m_hBaseWnd) && ::IsWindowVisible(g_pDisActFrame ->m_hBaseWnd)) //XN00027870 jw 20100202
		{
			CWnd* pWnd = CWnd::FromHandle(g_pDisActFrame ->m_hBaseWnd);
			if(hActWnd!=g_pDisActFrame ->m_hBaseWnd ) 
				::PostMessage(g_pDisActFrame ->m_hBaseWnd, WM_ACTIVATE,WA_ACTIVE,0);
		}
		else if(m_hActPreFrame && m_hActPreFrame != m_hBaseWnd && m_pGetActWnd && m_pGetActWnd(m_hActPreFrame,this) && ::IsWindow(m_hActPreFrame) &&  !::IsIconic(m_hActPreFrame) && IsWindowVisible(m_hActPreFrame))
		{
			if(hActWnd!=m_hActPreFrame ) 
				::PostMessage(m_hActPreFrame, WM_ACTIVATE,WA_ACTIVE,0);
		}
	}
}
CTPBaseFrame *CTPBaseFrame::GetDefAct()
{
	return g_pActFrame;
}
void CTPBaseFrame::SetDefAct(BOOL bAct)
{
	if(CTPDialog::GetDialogCount()>0) return;
	HWND  hActWnd = ::GetActiveWindow();
	if(bAct == 0x7ff) g_pActFrame = NULL;
	if(bAct && g_pActFrame && g_pActFrame ->m_hBaseWnd&& m_pCanActWnd && m_pCanActWnd(g_pActFrame ->m_hBaseWnd) && ::IsWindow(g_pActFrame ->m_hBaseWnd) &&  !::IsIconic(g_pActFrame ->m_hBaseWnd))
	{
		if(hActWnd!=g_pActFrame ->m_hBaseWnd) //27152 必须 SendMessage
			::SendMessage(g_pActFrame ->m_hBaseWnd,WM_ACTIVATE,bAct ? WA_ACTIVE : WA_INACTIVE,NULL);
		return;
	}	
}
void CTPBaseFrame::RestoreWnd(int iType)
{
}
void CTPBaseFrame::Restore()
{
	CTPBaseFrame *pBaseFrame = NULL;
	for(INT l=0;l<m_aBaseFrame.GetSize();l++)
	{
		pBaseFrame = m_aBaseFrame[l];
		if(pBaseFrame->m_hBaseWnd)
		{			
			pBaseFrame->m_uState &= ~(STATE_MINABLE | STATE_MAXABLE);
			if(IsZoomed(pBaseFrame->m_hBaseWnd))      pBaseFrame->m_uState |=  STATE_MAXABLE;
			else if(IsIconic(pBaseFrame->m_hBaseWnd)) pBaseFrame->m_uState |=  STATE_MINABLE;
			pBaseFrame ->PaintFrame();			
		}
	}
}
void CTPBaseFrame::GetUITemplateInfo(CRect &rtWnd,TPBaseFrameState &rtState)
{
	UINT uState = rtState.uState;
	uState = ~(TP_GUISHOW_MAXMIZE|TP_GUISHOW_MINMIZE|TP_GUISHOW_ENABLE|TP_GUISHOW_SHOW);
	if(m_uState &  STATE_MAXABLE)      uState |= TP_GUISHOW_MAXMIZE;
	else if(m_uState &  STATE_MINABLE) uState |= TP_GUISHOW_MINMIZE;
	if(!(m_uState &  STATE_DISABLE))   uState |= TP_GUISHOW_ENABLE;
	if(m_uState &  STATE_INVISIBLE)    uState |= TP_GUISHOW_SHOW;

	if(m_uState &  (STATE_MAXABLE|STATE_MINABLE)) rtWnd = m_rtRestore;
	else                                          ::GetWindowRect(m_hBaseWnd,rtWnd);

	//m_stuFrameState.dwMinWnd = MAKELONG(m_wndSizeInfo.ptMinTrackSize.x,m_wndSizeInfo.ptMinTrackSize.y);
	rtState = m_stuFrameState;
	rtState.uState = uState;
}
BOOL TPBaseFrame_IsNext(HWND hWndPre,HWND hWndNext)
{
	HWND hWnd = GetNextWindow(hWndPre,GW_HWNDNEXT);///below
	while(hWnd)
	{
		if(hWnd == hWndNext) return TRUE;
		hWnd = GetNextWindow(hWnd,GW_HWNDNEXT);
	}
	return FALSE;
}
void CTPBaseFrame::SortZorder(CTPBaseFrameArray &aBaseFrame)
{
	for(INT l=0;l<aBaseFrame.GetSize();l++) // 0 炎瞾E
	{
		if(aBaseFrame[l]->m_hBaseWnd == NULL || !::IsWindow(aBaseFrame[l]->m_hBaseWnd)) continue;
		for(INT k=l+1;k<aBaseFrame.GetSize();k++)
		if(TPBaseFrame_IsNext(aBaseFrame[l]->m_hBaseWnd,aBaseFrame[k]->m_hBaseWnd))
		{
			CTPBaseFrame *pTemp = aBaseFrame[l];
			aBaseFrame[l]       = aBaseFrame[k];
			aBaseFrame[k]       = pTemp;
		}
	}
}
void CTPBaseFrame::SortZorder(CStringArray &aWndTypeName)
{
	CTPBaseFrameArray aBaseFrame;
 	for(INT l = 0;l<aWndTypeName.GetSize();l++)
	{
		for(INT k=0;k<m_aBaseFrame.GetSize();k++)
		{
			if(m_aBaseFrame[k]->m_sWndType == aWndTypeName[l])
			{
 				if(GetWindowLong(m_aBaseFrame[k]->m_hBaseWnd, GWL_EXSTYLE) & WS_EX_TOPMOST) continue;
				aBaseFrame.Add(m_aBaseFrame[k]);
				break;
			}
		}
	}
	if(aBaseFrame.GetSize()<2) return;
	HWND hAct = GetActiveWindow();
	for(int i = (int)aBaseFrame.GetSize()-2;i>=0;i--)
	{
		SetWindowPos(aBaseFrame[i]->m_hBaseWnd,aBaseFrame[i+1]->m_hBaseWnd,0,0,0,0,SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);
	}
	HWND hTop = GetForegroundWindow ();
	if(hTop) hAct = hTop;
	SetActiveWindow(hAct);
	BringWindowToTop(hAct); // li huil i 因为SetWindowPos 会改变窗口顺序，所以需要将激活窗口，显示 到前面
	if(CTPBaseFrame::m_pMainFrameCmd) 
		CTPBaseFrame::m_pMainFrameCmd(1,NULL);
}

CString CTPBaseFrame::GetAMendMenuString(CString strMenu,int uWidth)  //按给定的菜单的宽度计算显示的内容
{
	CDC *pDC = AfxGetMainWnd() ->GetDC();
	if(pDC == NULL)    return strMenu;

	CFont *pFont         = pDC ->SelectObject(TP_GetSysFont(FONT_BASE_TEXT));
	CRect szWidth        = CRect(0,0,0,0);
	BOOL  bAddSuspension = FALSE;
	pDC->DrawText(strMenu,szWidth,DT_CALCRECT);
	if(szWidth.Width()>uWidth)
	{
		bAddSuspension = TRUE;
		int iLow=0, iHigh=strMenu.GetLength()-1, iMid;
		while(iLow<iHigh)
		{
 			iMid = (iLow+iHigh)/2;
			pDC->DrawText(strMenu.Left(iMid),szWidth,DT_CALCRECT);
			int iWidth = szWidth.Width();
			if(iWidth >uWidth)         iHigh = iMid;
			else if(iWidth == uWidth) {iHigh = iMid;break;}
			else if(iWidth <uWidth)    iLow  = iMid +1;
		}

		strMenu = strMenu.Left(iHigh);
	}
	pDC->SelectObject(pFont);
	AfxGetMainWnd() ->ReleaseDC(pDC);
	if(bAddSuspension) strMenu += _T("...");
	return strMenu;
}
void CTPBaseFrame::AddUseConfigrationMenu(CString sDlgName, CTPMenuEx *pMenu,int iPos,UINT uBaseID)
{	
	if(m_pUserConfigrationCmd)
	{
		struct data
		{
			CStringArray aComment;
			int          iCurrent;
		}stuData;
		if(m_pUserConfigrationCmd(m_sGuiName,sDlgName,1,(LPARAM)&stuData) && stuData.aComment.GetSize()>0)
		{
			CTPMenuEx *pSubMenu = new CTPMenuEx();
			pSubMenu ->CreateMenu();
			for(int i=0;i<stuData.aComment.GetSize();i++)
			{
				CString strComment   = GetAMendMenuString(stuData.aComment[i],400);
				pSubMenu ->AppendMenu(MF_STRING|MF_BYCOMMAND,uBaseID + i,strComment);
				if(i == stuData.iCurrent) pSubMenu ->CheckMenuItem(i,MF_CHECKED|MF_BYPOSITION);
			}
			pMenu ->InsertMenu(iPos,MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT_PTR)pSubMenu->GetSafeHmenu(),_L("User Configuration"));	
			pMenu ->SetMenuItemAttachMenu(iPos,MF_BYPOSITION,pSubMenu,TRUE);
		}
	}
}
BOOL CTPBaseFrame::SetUseConfigrationID(CString sDlgName,int iIndex)
{	
	if(m_pUserConfigrationCmd && iIndex >= 0)
	{
		m_pUserConfigrationCmd(m_sGuiName,sDlgName,2,iIndex);
		return TRUE;
	}
	else return FALSE;
}
void CTPBaseFrame::SetForceOrder(int iPre,int iNext)
{
	for(INT l=0;l<m_aForceOrder.GetSize();l++)
	{
		if(m_aForceOrder[l].uPreCtrl == iPre)
		{
			_tagDlgTabItem stuAdd = m_aForceOrder[l];
			m_aForceOrder[l].uNextCtrl = iNext;
			m_aForceOrder[l].pNextWnd  = m_hBaseWnd;

			stuAdd.pPreWnd  = m_hBaseWnd;
			stuAdd.uPreCtrl = iNext;

			m_aForceOrder.InsertAt(l+1,stuAdd);

			break;
		}
	}
}
void CTPBaseFrame::SetForceOrder(CTPBaseFrame **pWnd,int iCount)
{	
	CWnd *pWndPre = NULL,*pWndFrame = NULL,*pChild = NULL;
	TPDlgTabItemArray ForceOrderAll;
	TPDlgTabItemArray ForceOrderWnd;	
	UINT              uStyle        = 0;
	for(INT l=0;l<iCount;l++)	pWnd[l] ->SetForceOrder(ForceOrderAll);

	for(INT l=0;l<iCount;l++)
	{
		pWndFrame = CWnd::FromHandle(pWnd[l]->m_hBaseWnd);
		pWndPre   = NULL;
		pChild    = pWndFrame ->GetWindow(GW_CHILD);	

		ForceOrderWnd.RemoveAll();
		while(pChild && pChild ->GetParent() == pWndFrame)
		{		
			uStyle = pChild ->GetStyle();
			if( (uStyle & WS_TABSTOP) == WS_TABSTOP || (uStyle & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON)
			{		
				if(pWndPre)
				{
					ForceOrderWnd.Add(_tagDlgTabItem(pWndPre ->GetDlgCtrlID(),pChild->GetDlgCtrlID(),pWndFrame,pWndFrame));
				}
				pWndPre = pChild;
			}
			pChild    = pChild ->GetWindow(GW_HWNDNEXT);
		}
		if(ForceOrderAll.GetSize()>0)
		{
			INT_PTR iAll = ForceOrderAll.GetSize()-1;
			ForceOrderAll.Add(_tagDlgTabItem(ForceOrderAll[iAll].uNextCtrl,ForceOrderWnd[0].uPreCtrl,CWnd::FromHandle(ForceOrderWnd[0].pPreWnd),CWnd::FromHandle(ForceOrderAll[iAll].pNextWnd)));
		}
		ForceOrderAll.Append(ForceOrderWnd);
	}
	INT_PTR iAll = ForceOrderAll.GetSize()-1;
	if(iAll <=1) return;
	ForceOrderAll.Add(_tagDlgTabItem(ForceOrderAll[iAll].uNextCtrl,ForceOrderAll[0].uPreCtrl ,CWnd::FromHandle(ForceOrderAll[0].pPreWnd),CWnd::FromHandle(ForceOrderAll[iAll].pNextWnd)));

	for(INT l=0;l<iCount;l++)	pWnd[l] ->SetForceOrder(ForceOrderAll);
	
	ForceOrderAll.RemoveAll();
	ForceOrderWnd.RemoveAll();
}
void CTPBaseFrame::SetForceOrder(TPDlgTabItemArray &ForceOrder)
{
	m_aForceOrder.Copy(ForceOrder);
	for(INT l=0;l<m_aForceOrder.GetSize();l++)
	{
		if(m_aForceOrder[l].pNextWnd == NULL) m_aForceOrder[l].pNextWnd = m_hBaseWnd;
		if(m_aForceOrder[l].pPreWnd == NULL)  m_aForceOrder[l].pPreWnd  = m_hBaseWnd;		
	}
}
UINT CTPBaseFrame::GetVNumbKey(UINT uKey,UINT uFlag)
{
	uFlag    &= 0x0FFFFFFF;
	if(uFlag == 0x004F0001 && uKey == 0x0023)      return VK_VNUM1;
	else if(uFlag == 0x00500001 && uKey == 0x0028) return VK_VNUM2; 
	else if(uFlag == 0x00510001 && uKey == 0x0022) return VK_VNUM3; 
	else if(uFlag == 0x004B0001 && uKey == 0x0025) return VK_VNUM4; 
	else if(uFlag == 0x004C0001 && uKey == 0x000C) return VK_VNUM5; 
	else if(uFlag == 0x004D0001 && uKey == 0x0027) return VK_VNUM6; 
	else if(uFlag == 0x00470001 && uKey == 0x0024) return VK_VNUM7; 
	else if(uFlag == 0x00480001 && uKey == 0x0026) return VK_VNUM8; 
	else if(uFlag == 0x00490001 && uKey == 0x0021) return VK_VNUM9; 	 
	else return uKey;
}
BOOL CTPBaseFrame::ModifyStyleExEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	BOOL bReturn = CWnd::FromHandle(m_hBaseWnd)->ModifyStyle(dwRemove,dwAdd,nFlags);
	if(dwRemove & WS_THICKFRAME)
	{
		 m_uStyle  = GetWindowLong(m_hBaseWnd,GWL_STYLE);
		 m_uState &= ~STATE_FRAME;
		 m_uState |= STATE_FRAMESET;
	}	 
	return bReturn;
}
void CTPBaseFrame::SetTaskBarState(BOOL bShow,BOOL bTop)
{
	long lLen1(0),lLen2(0),lLen3(0),lLen4(0);
	lLen1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
	lLen2 = GetSystemMetrics(SM_YVIRTUALSCREEN);
	lLen3 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	lLen4 = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	if(bShow)
	{
		if(bTop)  lLen2 += 30;
		lLen4 -=30;
	}
	m_rtScreen = CRect(lLen1,lLen2,lLen1+lLen3,lLen2+lLen4);
}
void   CTPBaseFrame::GetMaxSizeInfo(POINT *pPoint)
{
	pPoint->x = m_rtScreen.Width()- m_rtEdge.left-m_rtEdge.right;
	pPoint->y = m_rtScreen.Height() - m_rtEdge.top-m_rtEdge.bottom;
}

BOOL CALLBACK EnumWndForDlgWnd(HWND hwnd, LPARAM lParam  )
{
	if(hwnd && ::IsWindow(hwnd) && ::IsWindowEnabled(hwnd))
	{
		g_aLockWnd.Add(hwnd);
		::EnableWindow(hwnd,FALSE);
	}
	return TRUE;
}
void  CTPBaseFrame::SetModeState(CWnd *pWnd,TPMODE_STATE eMode)
{
	if(eMode == TPMODE_DLGBEGIN)
	{
		g_iDoModal ++;
		if(m_uBaseFlag & TP_BASESTATE_MODESTATE)
		{
			if(g_iDoModal == 1)
			{
				g_aLockWnd.RemoveAll();
				::EnumThreadWindows(AfxGetApp()->m_nThreadID,(WNDENUMPROC)EnumWndForDlgWnd,(LPARAM)0); 
			}
		}
	}
	else if(eMode == TPMODE_DLGEND)
	{
		g_iDoModal --;
		if( g_iDoModal <=0)
		{
			if(m_uBaseFlag & TP_BASESTATE_MODESTATE)
			{
				for(INT l=0;l<g_aLockWnd.GetSize();l++)
					if(::IsWindow(g_aLockWnd[l])) ::EnableWindow(g_aLockWnd[l],TRUE);
				g_aLockWnd.RemoveAll();
			}
			else if(m_uBaseFlag & TP_BASESTATE_ENABLEMAIN)
			{
				AfxGetMainWnd() ->EnableWindow(TRUE);
				m_uBaseFlag &= ~TP_BASESTATE_ENABLEMAIN;
			}
		}
	}
	else
	{
		CTPBaseFrame *pBaseFrame = CTPBaseFrame::GetBaseFrame(pWnd);
		if(pBaseFrame)
		{
			pWnd ->SendMessage(WM_DOMODAL,eMode == TPMODE_START,0);
			if(eMode == TPMODE_START) 
			{
				if(!(m_uBaseFlag & TP_BASESTATE_ENABLEMAIN))
				{				
					AfxGetMainWnd()  ->EnableWindow(FALSE);
					pWnd  ->EnableWindow(TRUE);
					if(::IsIconic(pWnd ->m_hWnd)) 
						pWnd ->PostMessage(WM_SYSCOMMAND ,SC_RESTORE,0);
					pBaseFrame ->m_uBaseFlag &= ~TP_BASESTATE_ENABLEMAIN;
				}
				pBaseFrame ->m_uBaseFlag |= TP_BASESTATE_MODESTATE;
			}
			else   if(eMode == TPMODE_END) 
			{
				if(CTPDialog::GetDialogCount()>0)
				{
					pBaseFrame ->m_uBaseFlag |= TP_BASESTATE_ENABLEMAIN;
				}
				else
				{
					AfxGetMainWnd() ->EnableWindow(TRUE);
				}
				pBaseFrame ->m_uBaseFlag &= ~TP_BASESTATE_MODESTATE;
			}
		}
		else ASSERT(0);
	}
}
// CTPBaseWnd
IMPLEMENT_DYNAMIC(CTPBaseWnd, CWnd)
CTPBaseWnd::CTPBaseWnd()
{
	m_iBackColor = COLOR_BASE_FRAME;
	m_iBackBrush = BRUSH_FRAME;
}

CTPBaseWnd::~CTPBaseWnd()
{
}


BEGIN_MESSAGE_MAP(CTPBaseWnd, CWnd)
	ON_WM_ERASEBKGND()
	//ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

BOOL CTPBaseWnd::OnEraseBkgnd(CDC* pDC)
{
	if(TP_GetIsVista() && TP_GetDWMStatus() && m_iBackColor != -1)
	{
		CRect rcClient;
		GetClientRect( &rcClient);
		//pDC ->FillSolidRect( 0, 0,rcClient.Width(),rcClient.Height(), TP_GetSysColor( m_iBackColor));
		return TRUE;			 
	}
	else if(m_iBackColor != -1) return TRUE;
	else   return CWnd::OnEraseBkgnd(pDC);
}
BOOL  CTPBaseWnd::EraseBkgnd(CDC* pDC,CWnd *pWnd)
{
	if(TP_GetIsVista() && TP_GetDWMStatus())
	{
		CRect rcClient;
		pWnd ->GetClientRect( &rcClient);
		pDC  ->FillSolidRect( 0, 0,rcClient.Width(),rcClient.Height(), TP_GetSysColor( COLOR_BASE_FRAME));
		return TRUE;			 
	}
	return FALSE;
}
HBRUSH CTPBaseWnd::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{	
	if(TP_GetIsVista() && TP_GetDWMStatus() && m_iBackBrush != -1)
	{	
		return TP_GetSysBrush((TP_BRUSH_INDEX)m_iBackBrush);	  
	}
	else return NULL;
}
