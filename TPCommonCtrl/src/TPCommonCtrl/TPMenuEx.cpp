#include "StdAfx.h"
#include ".\tpmenuex.h"

TP_MENU_STYLE g_eMenuStyle = TP_MENU_STYLE_SLIDE;

typedef struct    tag_TPMenuWndData :public CTPBaseObject
{
	DWORD     dwVertify;
	WNDPROC   fMenuWndProc;
	HMENU     hAttachMenu;
	HWND      hWnd;
	HWND      hShaderWnd;
	CPoint    ptSave;
	HFONT     hFont;
	tag_TPMenuWndData()
	{
		m_dwSafeType = TP_SAFETYPE_MENUWNDDATA;
		fMenuWndProc = NULL;
		hAttachMenu  = NULL;
		hWnd         = NULL;
		hShaderWnd   = NULL;
		dwVertify    = 0x12345678;
		ptSave       = CPoint(0,0);
		hFont        = NULL;  
	}
	~tag_TPMenuWndData()
	{
		fMenuWndProc = NULL;
		hAttachMenu  = NULL;
		dwVertify    = 0;
	}
}TPMenuWndData;


HHOOK               g_hHookMenuFilter = NULL;
HHOOK               g_hHookMenuButton = NULL;
HHOOK               g_hHookMenuKey    = NULL;
BOOL                g_bRButtonDownd   = FALSE;
BOOL                g_bMenuExPopUp    = FALSE; 
HWND                g_hMenuExPopUp    = NULL;
HWND                g_hMenuCreate     = NULL;
TPMenuWndData      *g_pMenuWndData    = NULL;
HMENU               g_hMenuPopUp      = NULL; 
int                 g_iMotioHeoght    = GetSystemMetrics(SM_CYSCREEN);
CTPMenuEx          *g_pMenuCreate     = NULL;
CArray<HWND,HWND &> g_hMenuWnd; 
CTPMenuEx::tag_TPMenuItemData *g_pMenuDrawData   = NULL;

#define          MENU_WIDTH    3 


#ifdef TP_NS_EDIT
// tag_TPMenuTriangle
const UINT cMenuTriangleWidth = 10;
const UINT cMenuTriangleHeigh = 10;

IMPLEMENT_DYNAMIC(tag_TPMenuTriangle, CWnd)
tag_TPMenuTriangle::tag_TPMenuTriangle()
{	
	m_uState = 0;
	m_rtWnd  = CRect(0,0,0,0);
	m_pMenu  = NULL;
}

tag_TPMenuTriangle::~tag_TPMenuTriangle()
{	
}

BEGIN_MESSAGE_MAP(tag_TPMenuTriangle, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void tag_TPMenuTriangle::PaintDC(CDC *pDCSet)
{
	CDC *pDC = NULL;
	if(pDCSet == NULL) pDC = GetDC();
	else pDC = pDCSet;
	CRect rtClient;
	GetClientRect(&rtClient);
	if(m_uState & (ODS_GRAYED | ODS_DISABLED)) 
	{		
		pDC ->FillSolidRect(rtClient,RGB(163,163,163));
	}	
	else	if(m_uState & (ODS_SELECTED | ODS_HOTLIGHT|ODS_FOCUS )) 
	{		
		pDC ->FillSolidRect(rtClient,RGB(204,204,204));
	}
	else
	{	
		pDC ->FillSolidRect(rtClient,RGB(163,163,163));
	}
	pDC->DrawIcon(min(rtClient.CenterPoint().x-cMenuTriangleWidth/2, 0), min(rtClient.CenterPoint().y-cMenuTriangleHeigh/2, 0), m_pMenu->m_pPopMenu->GetIcon());
	if(pDCSet == NULL) ReleaseDC(pDC);
}
void tag_TPMenuTriangle::OnPaint()
{
	CPaintDC dc(this);
	PaintDC(&dc);
}

#endif

DWORD TP_GetMenuState(DWORD dItemState)
{
	DWORD dState = 0;
	if(dItemState & MF_CHECKED) dState |= ODS_CHECKED;
	if(dItemState & MF_GRAYED) dState |= ODS_GRAYED;
	if(dItemState & MF_DISABLED) dState |= ODS_DISABLED;
	if(dItemState & MF_DISABLED) dState |= ODS_DISABLED;
	return dState;
}
void TP_MenuSelect(UINT nID,HMENU &hMenu)
{
	CString    sText;
	CTPMenuEx *pMenu = (CTPMenuEx *)CTPMenuEx::FromHandle(hMenu);
	pMenu ->GetMenuString(nID,sText,MF_BYCOMMAND);
	if(sText.GetLength()>0)
	{
#ifdef TP_NS_LOG							
		//TP_LogEditNotifyCtrl(0,sText,CTPBaseFrame::GetActiveFrameCaption(),EDIT_LOGGER_MODULE,(CString)__FUNCTION__);					
#endif
	}
}
UINT TP_GetMenuID(HWND &hWnd,HMENU &hMenu) 
{
	CPoint ptCursor;
	::GetCursorPos(&ptCursor);
	hWnd = ::WindowFromPoint(ptCursor);
	for(INT l=0;l<g_hMenuWnd.GetSize();l++)
	{
		if(hWnd == g_hMenuWnd[l])
		{
			::ScreenToClient(hWnd,&ptCursor);
			tag_TPMenuWndData *pMenuWndItem = (tag_TPMenuWndData *)(DWORD_PTR)GetWindowLong(hWnd, GWL_USERDATA);
			if(pMenuWndItem == NULL || pMenuWndItem ->dwVertify != 0x12345678 || pMenuWndItem->hAttachMenu == NULL){}
			else 
			{
				CTPMenuEx *pMenu = (CTPMenuEx *)CTPMenuEx::FromHandle(pMenuWndItem ->hAttachMenu);
				if(pMenu)
				{
					for(int i=0;i<pMenu->m_aMenuItem.GetSize();i++)
					{
						if(pMenu->m_aMenuItem[i]->rtItem.top<ptCursor.y && pMenu->m_aMenuItem[i]->rtItem.bottom>ptCursor.y)
						{
							UINT nID = pMenu->GetMenuItemID(i);
							if(nID ==0 || nID == 0xffffffff) return 0;
							UINT nState = pMenu->GetMenuState(i,MF_BYPOSITION);
							if(nState & (MF_DISABLED|MF_GRAYED)) return 0;
							hMenu = pMenuWndItem ->hAttachMenu;
							return nID;
						}
					}
				}					
			}
		}
	}
	return 0;
}
UINT TP_GetMenuWnd(HWND &hWnd,HMENU &hMenu) 
{	
	for(INT l = (INT)g_hMenuWnd.GetSize()-1;l>=0;l--)
	{
		if(g_hMenuWnd[l])
		{			
			tag_TPMenuWndData *pMenuWndItem = (tag_TPMenuWndData *)(DWORD_PTR)GetWindowLong(g_hMenuWnd[l], GWL_USERDATA);
			if(pMenuWndItem == NULL || pMenuWndItem ->dwVertify != 0x12345678 || pMenuWndItem->hAttachMenu == NULL){}
			else 
			{
				CTPMenuEx *pMenu = (CTPMenuEx *)CTPMenuEx::FromHandle(pMenuWndItem ->hAttachMenu);
				if(pMenu)
				{					
					hMenu = pMenuWndItem ->hAttachMenu;
					hWnd  = g_hMenuWnd[l];
					return 1;				
				}					
			}
		}
	}
	return 0;
}
LRESULT CALLBACK TP_MenuKeyProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if(nCode >0)
	{
		HMENU  hMenu = NULL;
		HWND   hWnd   = NULL;
		UINT   nID  = TP_GetMenuWnd(hWnd,hMenu);
		if(nID !=0 && hWnd)
		{
			CTPMenuEx *pMenu = (CTPMenuEx *)CTPMenuEx::FromHandle(hMenu);
			if(pMenu && pMenu ->ApplyKey((UINT)wParam)) return 1;
		}
	}
	return CallNextHookEx(g_hHookMenuKey, nCode,wParam, lParam);
}
BOOL g_iButtonDown = 0;
LRESULT CALLBACK TP_MenuMouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if(nCode >0)
	{	
		MOUSEHOOKSTRUCT* pTemp = (MOUSEHOOKSTRUCT*)lParam;
		HMENU            hMenu = NULL;
		if(wParam == WM_RBUTTONDOWN || wParam == WM_RBUTTONUP)
		{
			if(g_bRButtonDownd) 
			{
				g_bRButtonDownd = FALSE; 
				return 	CallNextHookEx(g_hHookMenuButton, nCode,wParam, lParam);				 
			}
			else
			{			
				HWND hWnd = NULL;
				UINT nID  = TP_GetMenuID(hWnd,hMenu);
				if(nID !=0 && wParam == WM_RBUTTONUP) 
				{
					::PostMessage(hWnd,WM_KEYDOWN,VK_RETURN,0);											
					return 1;
				}
				else if(nID ==0 && wParam == WM_RBUTTONUP)//XN00025609 qushaojie 20091125
				{
                    return 1;
				}
			}
		}
		else if(wParam == WM_LBUTTONUP)
			g_iButtonDown = 0;		
		else if(wParam == WM_LBUTTONDOWN)
		{
			HWND hWnd = NULL;
			UINT nID  = TP_GetMenuID(hWnd,hMenu);			
			if(nID !=0 && hMenu && g_iButtonDown !=nID)
			{
				g_iButtonDown = nID;
				TP_MenuSelect(nID,hMenu);
			}
		}
	}
	return CallNextHookEx(g_hHookMenuButton, nCode,wParam, lParam);


}
void     TP_InitMenuMouseHook(BOOL bInit)
{
	if(bInit)
	{			
		g_bRButtonDownd = GetKeyState(VK_RBUTTON)<0;
		ASSERT(g_hHookMenuButton == NULL);
		ASSERT(g_hHookMenuKey == NULL);
		if(g_hHookMenuButton == NULL)
			g_hHookMenuButton = ::SetWindowsHookEx(WH_MOUSE, TP_MenuMouseProc, NULL,::GetCurrentThreadId()); 
		if(g_hHookMenuKey == NULL)
			g_hHookMenuKey = ::SetWindowsHookEx(WH_KEYBOARD, TP_MenuKeyProc, NULL,::GetCurrentThreadId()); 
		
	}
	else
	{
		g_bRButtonDownd = FALSE;
		if (g_hHookMenuButton != NULL)
		{
			::UnhookWindowsHookEx(g_hHookMenuButton);		
			g_hHookMenuButton = NULL;
		}		
		if (g_hHookMenuKey != NULL)
		{
			::UnhookWindowsHookEx(g_hHookMenuKey);		
			g_hHookMenuKey = NULL;
		}	
	}
}
LRESULT CALLBACK TP_MenuWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	AFX_MANAGE_STATE(AfxGetModuleState());	
	tag_TPMenuWndData *pMenuWndItem = (tag_TPMenuWndData *)(DWORD_PTR)GetWindowLong(hWnd, GWL_USERDATA);
	if(pMenuWndItem == NULL || pMenuWndItem ->dwVertify != 0x12345678)	return FALSE;
	CTPMenuEx *pMenu = (CTPMenuEx *)CTPMenuEx::FromHandle(pMenuWndItem ->hAttachMenu);
	if(pMenu) 	pMenu ->m_hAttachWnd = pMenuWndItem ->hWnd;		
	LRESULT hResult  = S_FALSE;

	WNDPROC   fMenuWndProc = pMenuWndItem ->fMenuWndProc;	
	switch(uMsg)
	{ 	
	case WM_NCPAINT:
		{	
			hResult = CallWindowProc(fMenuWndProc, hWnd, uMsg, wParam, lParam);
			CRect rtClient,rtWnd,rtTemp;			
			CWnd  *pWnd  = CWnd::FromHandle(hWnd);
			CDC   *pDC   = pWnd->GetWindowDC();			
			CTPDCExchange stuTemp(pDC);
			pWnd ->GetWindowRect(&rtWnd);
			pWnd->GetClientRect(&rtClient);
			pWnd->ClientToScreen(&rtClient);
			rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);			
			rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);	

			rtTemp = rtClient;rtTemp.InflateRect(1,1);
			for(INT l=0;l<rtClient.left;l++)
			{
#ifndef TP_NS_EDIT
				pDC->Draw3dRect(rtTemp,TP_GetSysColor(pMenu->m_iBackColor),TP_GetSysColor(pMenu->m_iBackColor));
#else
				pDC->Draw3dRect(rtTemp,RGB(163,163,163),RGB(163,163,163));
#endif
				rtTemp.InflateRect(1,1);
			}
			if(pMenu)
				pDC->Draw3dRect(rtWnd,TP_GetSysColor(pMenu->m_iTopLeft),TP_GetSysColor(pMenu->m_iRightBottom));			
			stuTemp.ReleaseDC();
			pWnd ->ReleaseDC(pDC);
		}
		return hResult;
	case WM_ERASEBKGND:
		{
			CRect rtClient;			
			CDC  *pDC     = CDC::FromHandle((HDC)wParam);
			CWnd *pWnd    = CWnd::FromHandle(hWnd);
			pWnd->GetClientRect(&rtClient);
			pDC->FillSolidRect(rtClient,RGB(163,163,163));
			return TRUE;
		}
	case WM_PRINT:		
		{				
			hResult = CallWindowProc(fMenuWndProc, hWnd, uMsg, wParam,lParam);
			CRect rtClient,rtWnd,rtTemp;			
			CWnd *pWnd    = CWnd::FromHandle(hWnd);
			CDC  *pDC     = CDC::FromHandle((HDC)wParam);	
			pWnd ->GetWindowRect(&rtWnd);
			pWnd->GetClientRect(&rtClient);
			pWnd->ClientToScreen(&rtClient);
			rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);			
			rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);	
			rtTemp = rtClient;rtTemp.InflateRect(1,1);
			for(INT l=0;l<rtClient.left;l++)
			{
#ifndef TP_NS_EDIT
				pDC->Draw3dRect(rtTemp,TP_GetSysColor(pMenu->m_iBackColor),TP_GetSysColor(pMenu->m_iBackColor));
#else
				pDC->Draw3dRect(rtTemp,RGB(163,163,163),RGB(163,163,163));
#endif
				rtTemp.InflateRect(1,1);
			}

			if(pMenu)
			{						
				pDC->Draw3dRect(rtWnd,TP_GetSysColor(pMenu->m_iTopLeft),TP_GetSysColor(pMenu->m_iRightBottom));
#ifdef TP_NS_EDIT
				for(INT l=0;l<pMenu->m_aMenuItem.GetSize();l++) if(pMenu->m_aMenuItem[l]->pTriangle)
					pDC->DrawIcon(pMenu->m_aMenuItem[l]->pTriangle->m_rtWnd.left+3,pMenu->m_aMenuItem[l]->pTriangle->m_rtWnd.top+3,pMenu->m_pPopMenu->GetIcon());
#endif
				pMenu ->PaintMenu(pDC,rtClient);										
			}
			return hResult;
		}	
	case WM_DESTROY:
#ifdef TP_NS_EDIT
		if(pMenu)
		{
			CTPBaseCtrl::SetBaseFont(hWnd,pMenuWndItem->hFont);	
			for(INT l=0;l<pMenu->m_aMenuItem.GetSize();l++)
			{

				if(pMenu->m_aMenuItem[l]->pTriangle)
				{
					pMenu->m_aMenuItem[l]->pTriangle ->DestroyWindow();
					delete pMenu->m_aMenuItem[l]->pTriangle;
					pMenu->m_aMenuItem[l]->pTriangle = NULL;
				}
			}
		}
		
#endif
		return CallWindowProc(fMenuWndProc, hWnd, uMsg, wParam, lParam);
	case WM_NCDESTROY:	

		for(INT l= (INT)g_hMenuWnd.GetSize()-1;l>=0;l--)
		{
			if(g_hMenuWnd[l] == hWnd) 
				g_hMenuWnd.RemoveAt(l);
		}
		if(g_hMenuWnd.GetSize() == 0) TP_InitMenuMouseHook(FALSE);	

		if(g_hMenuExPopUp && ::IsWindow(g_hMenuExPopUp))	PostMessage(g_hMenuExPopUp,WM_CLOSEMENU,0,0);
		//g_hMenuExPopUp = NULL;
		SetWindowLong(hWnd, GWL_USERDATA,(DWORD)(DWORD_PTR)NULL);
		SetWindowLong(hWnd, GWL_WNDPROC,(DWORD)(DWORD_PTR)pMenuWndItem ->fMenuWndProc);
		delete pMenuWndItem;
		break;
	case WM_MOUSEWHEEL:
		{
			if(pMenu)
			{
				if(HIWORD(wParam)>0)  ::PostMessage(pMenu  ->m_hAttachWnd,WM_KEYDOWN,VK_UP,0);
				else                  ::PostMessage(pMenu  ->m_hAttachWnd,WM_KEYDOWN,VK_DOWN,0);				
			}
		}
		break;
	case WM_WINDOWPOSCHANGED:
	case WM_WINDOWPOSCHANGING:
		{
			LPWINDOWPOS pPos = (LPWINDOWPOS)lParam;
			if(uMsg==WM_WINDOWPOSCHANGING)
			{					
				if(pMenu && (pMenu->m_uBaseState&TP_BASESTATE_COMBOBOXMENU))
				{					
					/*int iScreenHeight    = ::GetSystemMetrics(SM_CYSCREEN);
					pMenu->m_iMenucy = max(pMenu->m_iMenucy,pPos->cy);
					pPos->x = pMenu->m_rtMenuBox.left;
					pPos->y = pMenu->m_rtMenuBox.bottom;
					if(pPos->y + pMenu->m_iMenucy > iScreenHeight)
						pPos->y = pMenu->m_rtMenuBox.top - pMenu->m_iMenucy;	*/
				}
  				else if(pMenu && pMenu->m_rtMenuBox.bottom>pPos->y && (pMenu->m_uBaseState&TP_BASESTATE_MAINMENU)) // XN00027929 Wangjin 20101105
 				{					
 					pPos->x = pMenu->m_rtMenuBox.right;
					pPos->x = max(pPos->x,0);
 				}				
			}
			else
			{				
			}
		}
		break;
	default:
		break;
	}
	return CallWindowProc(fMenuWndProc, hWnd, uMsg, wParam, lParam);	
} 

LRESULT CALLBACK TP_MenuHook(int code, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetModuleState());
	CWPSTRUCT* pTemp = (CWPSTRUCT*)lParam;
	if(code == HC_ACTION )
	{
		HWND hWnd = pTemp->hwnd;		
		if(pTemp->message==0x01E2 || pTemp->message== WM_CREATE)
		{				
			TCHAR Name[20];
			int Count = GetClassName (hWnd,Name,sizeof(Name)/sizeof(TCHAR));			
			if(Count==6 && _tcscmp(Name,_T("#32768"))==0 && g_bMenuExPopUp )
			{
				tag_TPMenuWndData *pMenuWndItem = (tag_TPMenuWndData *)(DWORD_PTR)GetWindowLong(hWnd, GWL_USERDATA);
				if(pMenuWndItem == NULL || pMenuWndItem ->dwVertify != 0x12345678)
				{					
					g_hMenuCreate = hWnd;
					pMenuWndItem  = new tag_TPMenuWndData();
					pMenuWndItem ->fMenuWndProc = (WNDPROC)(LONG_PTR)SetWindowLong(hWnd, GWL_WNDPROC,(DWORD)(DWORD_PTR)TP_MenuWndProc);
					pMenuWndItem ->hWnd         = hWnd;
					pMenuWndItem ->hAttachMenu  = g_hMenuPopUp;
					SetWindowLong(hWnd, GWL_USERDATA,(DWORD)(DWORD_PTR)pMenuWndItem);					
					UINT uStyle = GetWindowLong (hWnd, GWL_STYLE);																
					uStyle &= ~WS_BORDER;
					SetWindowLong (hWnd, GWL_STYLE,uStyle);								
					CFont *pFont = TP_GetSysFont(FONT_BASE_TEXT1);
					pMenuWndItem->hFont = CTPBaseCtrl::GetBaseFont(hWnd);															
					::SendMessage(hWnd, WM_SETFONT, (WPARAM)pFont->GetSafeHandle(), TRUE);							
				}
				g_pMenuWndData = pMenuWndItem;
				g_hMenuWnd.Add(hWnd);
				if(g_hMenuWnd.GetSize() ==1) TP_InitMenuMouseHook(TRUE);
				return 0;
			}
			else if(Count==9 && _tcscmp(Name,_T("SysShadow"))==0 && g_bMenuExPopUp )
			{			
				if(TP_GetSafeAdress(g_pMenuWndData,TP_SAFETYPE_MENUWNDDATA))	g_pMenuWndData ->hShaderWnd  = hWnd;
				g_pMenuWndData = NULL;
				g_hMenuPopUp   = NULL;
			}
			else if(g_pMenuCreate)
			{
			}
		}	
		else if(pTemp->message==WM_INITMENUPOPUP) 
		{						
			if(TP_GetSafeAdress(g_pMenuWndData,TP_SAFETYPE_MENUWNDDATA)) 
				g_pMenuWndData ->hAttachMenu = (HMENU)pTemp->wParam;
			g_hMenuPopUp = (HMENU)pTemp->wParam;	
		}
		else if(pTemp->message==WM_UNINITMENUPOPUP)
		{
			g_hMenuPopUp = NULL;
		}			
	}
	return CallNextHookEx(g_hHookMenuFilter, code,wParam, lParam);
}
void     TP_InitMenuHook(BOOL bInit)
{
	if(bInit)
	{
		if(g_hHookMenuFilter == NULL)
			g_hHookMenuFilter = ::SetWindowsHookEx(WH_CALLWNDPROC, TP_MenuHook, NULL, ::GetCurrentThreadId());
	}
	else
	{
		if (g_hHookMenuFilter != NULL)
		{
			::UnhookWindowsHookEx(g_hHookMenuFilter);		
			g_hHookMenuFilter = NULL;
		}		
		if (g_hHookMenuButton != NULL)
		{
			::UnhookWindowsHookEx(g_hHookMenuButton);		
			g_hHookMenuButton = NULL;
		}			
		if (g_hHookMenuKey != NULL)
		{
			::UnhookWindowsHookEx(g_hHookMenuKey);		
			g_hHookMenuKey = NULL;
		}
	}
}



CTPMenuEx::CTPMenuEx(void)
{
	m_dwVertify    = 0x12345678;
	m_iBackColor   = COLOR_BASE_BACK;
	m_iTopLeft     = COLOR_BASE_3DLEFT;
	m_iRightBottom = COLOR_BASE_3DRIGHT;
	m_iText        = COLOR_BASE_TEXT;
	m_iTextDisable = COLOR_BASE_TEXTDIS;
	m_iTextHight   = COLOR_BASE_TEXTFOCUS;

	m_dwType       = 0;
	m_lBarWidth    = 0;
	m_lMinWidth    = 10;
	m_lMaxExtWidth = 0;
	m_hAttachWnd   = NULL;
	m_bComMenu     = FALSE;
	m_iSelectID    = -5;
	m_stuSelectItem.itemID  = -5;
	
	m_pLeftBar     = NULL;
	m_pResCheck    = TP_LoadImage(TP_GetSkinPath() + _L("\\CommonCtrl\\Menu\\MenuCheck.bmp"),12,RGB(53,53,53),FALSE,FALSE);
	m_pPopMenu     = TP_LoadIcon(TP_GetSkinPath() + _L("\\CommonCtrl\\Menu\\PopMenu.ico"));	
	m_lCheckWidth  = m_pResCheck ->Width();

	m_iFontIndex   = FONT_BASE_TEXT1;
	m_iExtAddWidth = 0;
	m_rtMenuBox    = CTPBaseFrame::m_rtMenuBox;
	m_iMenucy      = -1;
	m_bLoadString  = FALSE;
	ZeroMemory(&m_stuSelectItem, sizeof(m_stuSelectItem));
	m_bIsFocus     = FALSE;
}

CTPMenuEx::~CTPMenuEx(void)
{
	for(INT l=0;l<m_aMenuItem.GetSize();l++)
	{		
		delete m_aMenuItem[l];
	}
	m_aMenuItem.RemoveAll();

	TP_ReleaseCommonRes(m_pResCheck);
	m_pResCheck = NULL;
	TP_ReleaseCommonRes(m_pLeftBar);
	m_pLeftBar = NULL;
	TP_ReleaseCommonRes(m_pPopMenu);
	m_pPopMenu = NULL;	
}

void CTPMenuEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	ASSERT(lpMeasureItemStruct);

	tag_TPMenuItemData *pMenuItemData = (tag_TPMenuItemData *)lpMeasureItemStruct ->itemData;
	if(pMenuItemData == NULL || pMenuItemData ->dwType  != 0x7654321)
		return;
	long lMinWidth = max(pMenuItemData ->pParentMenuEx ->m_lMinWidth,pMenuItemData ->lMinWidth);	
	if(pMenuItemData ->uID  == 0)
	{
		if(m_uBaseState & TP_BASESTATE_NOSPARTEBAR)
			lpMeasureItemStruct ->itemHeight =  0;
		else
			lpMeasureItemStruct ->itemHeight =  4;
		if(m_bComMenu)	lpMeasureItemStruct ->itemWidth  =  max(m_lBarWidth + 2 + m_lCheckWidth,lMinWidth);		
		else 		    lpMeasureItemStruct ->itemWidth  =  m_lBarWidth + 2 + m_lCheckWidth +  lMinWidth;		
	}
	else
	{	
		if(pMenuItemData ->pIcon && pMenuItemData ->bButton)
		{
			lpMeasureItemStruct ->itemWidth  = (UINT)pMenuItemData->szIcon.cx+4;
			lpMeasureItemStruct ->itemHeight = (UINT)pMenuItemData->szIcon.cy+6;
		}
		else
		{
			CDC  *pDC  = AfxGetMainWnd () ->GetDC();
			CFont *pFont = (CFont *)pDC ->SelectObject(TP_GetSysFont(FONT_BASE_TEXT));
			CRect   rtText = CRect(0,0,0,0);				
			pDC ->DrawText( pMenuItemData ->sText,rtText,DT_CALCRECT);		
			pDC ->SelectObject(pFont);
			AfxGetMainWnd () ->ReleaseDC(pDC);		
			lpMeasureItemStruct ->itemHeight = rtText.Height()+ 4;		
			if(m_bComMenu)	lpMeasureItemStruct ->itemWidth  = max(rtText.Width() + m_lMaxExtWidth + m_lBarWidth + 4 + m_lCheckWidth,lMinWidth);				
			else            lpMeasureItemStruct ->itemWidth  = max(rtText.Width() + m_lMaxExtWidth,lMinWidth) + m_lBarWidth + 4 + m_lCheckWidth;				
			if(pMenuItemData->pImage)
			{
				lpMeasureItemStruct ->itemWidth += pMenuItemData->pImage->Width() +4;
				lpMeasureItemStruct ->itemHeight = max(lpMeasureItemStruct ->itemHeight,(UINT)pMenuItemData->pImage->Height() +4);
			}
			else if(pMenuItemData->cImage != 0)
			{
				lpMeasureItemStruct ->itemWidth += 40 + 4;
			}
		}	
	}
	lpMeasureItemStruct ->itemWidth += m_iExtAddWidth;
}
 
COLORREF TP_DarkenColor( long lScale, COLORREF lColor)
{ 
	long red   = MulDiv(GetRValue(lColor),(255-lScale),255);
	long green = MulDiv(GetGValue(lColor),(255-lScale),255);
	long blue  = MulDiv(GetBValue(lColor),(255-lScale),255);

	return RGB(red, green, blue); 
}
COLORREF TP_LightenColor( long lScale, COLORREF lColor)
{ 
	long R = MulDiv(255-GetRValue(lColor),lScale,255)+GetRValue(lColor);
	long G = MulDiv(255-GetGValue(lColor),lScale,255)+GetGValue(lColor);
	long B = MulDiv(255-GetBValue(lColor),lScale,255)+GetBValue(lColor);

	return RGB(R, G, B); 
}

void CTPMenuEx::SetMenuWndData(HWND hWnd,ULONG_PTR uItemData)
{
	tag_TPMenuItemData *pMenuItemData = (tag_TPMenuItemData *)uItemData;
	if(pMenuItemData == NULL || pMenuItemData ->dwType  != 0x7654321 || pMenuItemData ->pWndData != NULL) return;
	CTPMenuEx *pMenu = (CTPMenuEx *)CMenu::FromHandle(pMenuItemData->hParentMenu);
	if(pMenu == NULL) return;
	tag_TPMenuWndData *pMenuWndItem = (tag_TPMenuWndData *)(DWORD_PTR)GetWindowLong(hWnd, GWL_USERDATA);
	if(pMenuWndItem == NULL || pMenuWndItem ->dwVertify != 0x12345678) return;
	pMenuItemData ->pWndData   = pMenuWndItem;
	pMenuWndItem ->hAttachMenu = pMenuItemData ->hMenu;
	pMenuWndItem ->hWnd        = hWnd;
	pMenu ->m_hAttachWnd       = hWnd;	
}
void CTPMenuEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct != NULL);

	tag_TPMenuItemData *pMenuItemData = (tag_TPMenuItemData *)lpDrawItemStruct ->itemData;
	if(pMenuItemData == NULL || pMenuItemData ->dwType  != 0x7654321) return;

	CRect       rtText       = lpDrawItemStruct->rcItem;
	CDC        *pDCGet       = CDC::FromHandle(lpDrawItemStruct->hDC);	
	if(pDCGet == NULL) 	return;	
	CTPMemoDC   memDC(pDCGet,rtText.Width(),rtText.Height()),*pDC = &memDC;	
	CTPDCObject stuFont(&memDC,TP_GetSysFont(FONT_BASE_TEXT1));
	CString     sText     = pMenuItemData ->sText;	
	int         iBackMode = pDC ->SetBkMode(TRANSPARENT);
	pDC->SetStretchBltMode(COLORONCOLOR);
	pMenuItemData ->rtItem = rtText;
	rtText.OffsetRect(-rtText.left,-rtText.top);


	COLORREF clText; 
	rtText.left   += m_lBarWidth;

	COLORREF colorBack      = TP_GetSysColor(COLOR_BASE_FRAME);
	COLORREF color3DShadow  = TP_DarkenColor(60,colorBack);
	COLORREF color3DHilight = TP_LightenColor(60,colorBack);
	COLORREF colorGrayText  = TP_DarkenColor(100,colorBack);

	COLORREF colorHilight   = RGB(110,102,162);
	COLORREF colorGrayed    = pDC->GetNearestColor(TP_DarkenColor(10,colorBack)); 

	if(m_bComMenu && (lpDrawItemStruct ->itemAction & ODA_SELECT) &&m_iSelectID != -5) 	
	{
		m_iSelectID = -5;		
		m_stuSelectItem.hDC = lpDrawItemStruct->hDC;
		DrawItem(&m_stuSelectItem);
	}
	if(pMenuItemData ->uID  == 0)
	{			    
		rtText = pMenuItemData ->rtItem;
		if(rtText.Height()<=0) return;
		rtText.right ++;
		if(m_bComMenu)
			pDCGet ->FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_WINDOW));
		else
			pDCGet ->FillSolidRect(rtText,RGB(163,163,163));
		if(!(m_uBaseState & TP_BASESTATE_NOSPARTEBAR))
		{
			rtText.DeflateRect(0,1);
			pDCGet->Draw3dRect(rtText,color3DHilight ,color3DShadow);
		}
		return;		
	}
	long lCheckIndex = -1;
	if(pMenuItemData ->bButton && pMenuItemData->pIcon)
	{	
		CTPMemoDC cdcMemo(pDC,rtText.Width(),rtText.Height());
		CRect rtMemo = CRect(0,0,rtText.Width(),rtText.Height());				
		CRect rtIcon  = rtText;
		rtIcon.left   = rtIcon.left + (rtIcon.Width()- pMenuItemData->szIcon.cx)/2;		
		rtIcon.right  = rtIcon.left + pMenuItemData->szIcon.cx;
		rtIcon.top    = rtText.top  + (rtText.Height() - pMenuItemData->szIcon.cy)/2;
		rtIcon.bottom = rtIcon.top  + pMenuItemData->szIcon.cy;
		pDC ->FillSolidRect(rtText,TP_GetSysColor(m_iBackColor));
		TP_StretchDIBits(pDC,pMenuItemData->pIcon,pMenuItemData->szIcon.cx,pMenuItemData->szIcon.cy,rtIcon);		
		BLENDFUNCTION blf;
		blf.AlphaFormat = 0;
		blf.BlendFlags = 0;
		blf.BlendOp    = 0;
		blf.SourceConstantAlpha = 40;

		AlphaBlend(pDC ->m_hDC, rtText.left, rtText.top, rtText.Width(), rtText.Height(),
			cdcMemo.m_hDC, rtMemo.left, rtMemo.top, rtMemo.Width(), rtMemo.Height(), blf);		
	}
	else
	{	
		if(m_bComMenu)
		{
			if(m_iSelectID == lpDrawItemStruct ->itemID || lpDrawItemStruct->itemState & (ODS_SELECTED | ODS_HOTLIGHT|ODS_FOCUS )) 
			{						
				m_stuSelectItem = *lpDrawItemStruct;
				clText      = pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));						
				pDC ->FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_SELECTITEMA));
			}
			else
			{			
				clText      = pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));		
				pDC ->FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_WINDOW));
			}
		}
		else
		{		
			if(lpDrawItemStruct->itemState & (ODS_GRAYED | ODS_DISABLED)) 
			{
				lCheckIndex = 0;
				clText      = pDC->SetTextColor(RGB(128,128,128));				
				pDC ->FillSolidRect(rtText,RGB(163,163,163));
			}	
			else	if(lpDrawItemStruct->itemState & (ODS_SELECTED | ODS_HOTLIGHT|ODS_FOCUS )) 
			{
				lCheckIndex = 2;
				clText      = pDC->SetTextColor(RGB(0,0,0));

				pDC ->FillSolidRect(rtText,RGB(204,204,204));
				pDC->Draw3dRect(rtText ,RGB(255,255,255),RGB(77,77,77));
			}
			else
			{
				lCheckIndex = 1;
				clText      = pDC->SetTextColor(RGB(0,0,0));	
				pDC ->FillSolidRect(rtText,RGB(163,163,163));
			}
		}
		if(!(m_dwType & TP_MENU_UNCHECK ) && ( lpDrawItemStruct->itemState & ODS_CHECKED)) 
		{
			ImageList_Draw(m_pResCheck ->GetImage(),lCheckIndex ,pDC ->m_hDC,rtText.left + 2,rtText.top  +  (rtText.Height() - m_pResCheck ->Height())/2,ILD_TRANSPARENT);
		}
		if(m_lCheckWidth>0 && pMenuItemData ->pIcon)
		{
			CRect rtIcon = 	rtText;
			rtIcon.right = rtIcon.left + m_lCheckWidth;		
			TP_StretchDIBits(pDC,pMenuItemData->pIcon,pMenuItemData->szIcon.cx,pMenuItemData->szIcon.cy,rtIcon);
		}
		rtText.left += m_lCheckWidth + 4;
		if(pMenuItemData->pImage)
		{
			if(pMenuItemData->bCheck)
			{
				ImageList_Draw(pMenuItemData->pImage->GetImage(),pMenuItemData->iImageIndex,pDC->m_hDC,rtText.left - m_lCheckWidth,rtText.top + (rtText.Height() - pMenuItemData->pImage->Height())/2,0);
			}
			else
			{			
				ImageList_Draw(pMenuItemData->pImage->GetImage(),pMenuItemData->iImageIndex,pDC->m_hDC,rtText.left + 2,rtText.top + (rtText.Height() - pMenuItemData->pImage->Height())/2,0);
				rtText.left +=pMenuItemData->pImage->Width() + 4;
			}
		}		
		if(pMenuItemData->cImage !=0)
		{
			if(pMenuItemData->bTextMode ==1)
			{				
				CRect rColorRect = rtText;
				rColorRect.right = rtText.left + 40;
				rColorRect.DeflateRect(4,4);
				pDC ->FillSolidRect(rColorRect,pMenuItemData->cImage);
				CRect rTextRect = rtText;
				rTextRect.left = rColorRect.right + 4;
				rTextRect.right = rtText.right - 4;
				pDC ->DrawText(sText,rTextRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			else
			{			
				rtText.DeflateRect(4,4);
				pDC ->FillSolidRect(rtText,pMenuItemData->cImage);	
				rtText.InflateRect(4,4);
			}
		}
		else
		{		
			pDC ->DrawText(sText,rtText,DT_LEFT|DT_VCENTER|DT_SINGLELINE);	
			if(pMenuItemData->sExtText)
			{
				CRect rtExt = rtText; rtExt.right -= 6;rtExt.left = rtExt.right - m_lMaxExtWidth;
				pDC ->DrawText(pMenuItemData->sExtText,rtExt,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);	
			}
		}
		if(pMenuItemData ->bPopUp && m_hAttachWnd)
		{
			CRect rtTrangle = CRect(pMenuItemData->rtItem.right-cMenuTriangleWidth-1,	pMenuItemData->rtItem.CenterPoint().y-cMenuTriangleHeigh/2, // XN00029871 Wangjin 20101117
									pMenuItemData->rtItem.right-1,						pMenuItemData->rtItem.CenterPoint().y+cMenuTriangleHeigh/2);
			if(pMenuItemData ->pTriangle == NULL )
			{
				pMenuItemData ->pTriangle          = new tag_TPMenuTriangle();
				pMenuItemData ->pTriangle->Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rtTrangle,CWnd::FromHandle(m_hAttachWnd),1000);			
				pMenuItemData ->pTriangle->m_pMenu = this;				
			}
			if(pMenuItemData ->pTriangle->m_rtWnd != rtTrangle)
			{
				pMenuItemData ->pTriangle->m_rtWnd = rtTrangle;
				pMenuItemData ->pTriangle->MoveWindow(rtTrangle);				
			}
			pMenuItemData ->pTriangle->m_uState = lpDrawItemStruct->itemState ;
			pMenuItemData ->pTriangle->Invalidate();
		}		
		pDC ->SetTextColor(clText);
		pDC ->SetBkMode(iBackMode);
	}
	pDCGet->BitBlt(pMenuItemData ->rtItem.left,pMenuItemData ->rtItem.top,pMenuItemData ->rtItem .Width(),pMenuItemData ->rtItem .Height(),&memDC,0,0,SRCCOPY);
}

BOOL  CTPMenuEx::LoadMenu(LPCTSTR lpszResourceName)
{
	BOOL bReturn = CMenu::LoadMenu(lpszResourceName);
	if(bReturn)	InitMenu(m_hMenu);
	return bReturn;
}
BOOL  CTPMenuEx::LoadMenu(UINT nIDResource)
{
	BOOL bReturn = CMenu::LoadMenu(nIDResource);
	if(bReturn)	InitMenu(m_hMenu);
	return bReturn;
}
BOOL   CTPMenuEx::DestroyMenu()
{	

	for(INT l=0;l<m_aMenuItem.GetSize();l++)
	{
#ifdef TP_NS_EDIT
		if(m_aMenuItem[l]->pTriangle)
		{
			m_aMenuItem[l]->pTriangle ->DestroyWindow();
			delete m_aMenuItem[l]->pTriangle;
			m_aMenuItem[l]->pTriangle = NULL;
		}
#endif
		delete m_aMenuItem[l];
	}
	m_aMenuItem.RemoveAll();
	return CMenu::DestroyMenu();	
}
BOOL   CTPMenuEx::DeleteMenu(UINT nPosition, UINT nFlags)
{
	tag_TPMenuItemData *pMenuItemData = NULL;	

	if(nFlags & MF_BYPOSITION) pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,TRUE);
	else                       pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,FALSE);

	if(pMenuItemData)
	{	
		for(INT l=0;l<m_aMenuItem.GetSize();l++)
		{
			if(m_aMenuItem[l] == pMenuItemData)
			{
				delete m_aMenuItem[l];
				m_aMenuItem.RemoveAt(l);
				break;
			}			
		}
	}	
	return CMenu::DeleteMenu(nPosition,nFlags);
}

long  CTPMenuEx::GetMenuWidth(CString sText)
{
	CDC   *pDC   = AfxGetMainWnd() ->GetDC();
	CFont *pFont = pDC ->SelectObject(TP_GetSysFont(FONT_BASE_TEXT));
	CRect szWidth  = CRect(0,0,0,0);
	pDC->DrawText(sText,szWidth,DT_CALCRECT);
	pDC   ->SelectObject(pFont);
	AfxGetMainWnd() ->ReleaseDC(pDC);
	return szWidth.Width();
}
BOOL   CTPMenuEx::SetMenuExtText(UINT nFlags, UINT_PTR nIDNewItem ,LPCTSTR sExtText)
{
	tag_TPMenuItemData *pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nIDNewItem,nFlags & MF_BYPOSITION);
	if(pMenuItemData ) 
	{
		pMenuItemData ->sExtText = sExtText;

		if(lstrlen(sExtText)>0)
		{				
			CDC  *pDC  = AfxGetMainWnd() ->GetDC();
			CFont *pFont = (CFont *)pDC ->SelectObject(TP_GetSysFont(FONT_BASE_TEXT));		
			CRect   rtTextEx = CRect(0,0,0,0);		
			pDC ->DrawText(pMenuItemData ->sExtText,rtTextEx,DT_CALCRECT);
			pDC ->SelectObject(pFont);
			AfxGetMainWnd() ->ReleaseDC(pDC);
			m_lMaxExtWidth = max(m_lMaxExtWidth,rtTextEx.Width());
		}
	}
	return TRUE;
}

BOOL   CTPMenuEx::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem,LPCTSTR lpszNewItem ,LPCTSTR sExtText)
{
	BOOL bReturn =  CMenu::AppendMenu(nFlags| MF_OWNERDRAW,nIDNewItem,lpszNewItem);
	if(bReturn)
	{	
		tag_TPMenuItemData *pMenuItemData = new tag_TPMenuItemData(this);


		SetMenuItemData(GetMenuItemCount()-1,(ULONG_PTR)pMenuItemData,TRUE);

		pMenuItemData ->sText        = lpszNewItem;
		pMenuItemData ->sExtText     = sExtText;
		pMenuItemData ->uID          = nIDNewItem;
		pMenuItemData ->hParentMenu  = m_hMenu;

		if(lstrlen(sExtText)>0)
		{				
			CDC  *pDC  = AfxGetMainWnd() ->GetDC();
			CFont *pFont = (CFont *)pDC ->SelectObject(TP_GetSysFont(FONT_BASE_TEXT));		
			CRect   rtTextEx = CRect(0,0,0,0);		
			pDC ->DrawText(pMenuItemData ->sExtText,rtTextEx,DT_CALCRECT);
			pDC ->SelectObject(pFont);
			AfxGetMainWnd() ->ReleaseDC(pDC);
			m_lMaxExtWidth = max(m_lMaxExtWidth,rtTextEx.Width());
		}


		m_aMenuItem.Add(pMenuItemData);
		if((nFlags & MF_POPUP) && nIDNewItem)
		{
			pMenuItemData ->bPopUp = TRUE;
			pMenuItemData ->hMenu  = (HMENU)nIDNewItem;	
			m_lMinWidth            = max(m_lMinWidth,GetMenuWidth(pMenuItemData ->sText) + 10);
		}
	}
	return bReturn;
}
BOOL  CTPMenuEx::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem,LPCTSTR lpszNewItem)
{
	return AppendMenu(nFlags, nIDNewItem,lpszNewItem,NULL);
}
BOOL  CTPMenuEx::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const CBitmap* pBmp)
{
	return FALSE;
}
CTPMenuEx*  CTPMenuEx::GetSubMenu(int nPos) 
{
	HMENU hMenu = ::GetSubMenu(m_hMenu, nPos);
	if(hMenu)
	{
		tag_TPMenuItemData *pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPos,TRUE);
		if(pMenuItemData && pMenuItemData ->pMenuEx) return pMenuItemData ->pMenuEx;
	}		
	return (CTPMenuEx *)CMenu::GetSubMenu(nPos);
}
BOOL  CTPMenuEx::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem ,LPCTSTR lpszNewItem )
{	
	BOOL bReturn =  CMenu::InsertMenu(nPosition,nFlags|MF_OWNERDRAW, nIDNewItem ,lpszNewItem);
	if(bReturn)
	{	
		tag_TPMenuItemData *pMenuItemData = new tag_TPMenuItemData(this);

		SetMenuItemData(nPosition,(ULONG_PTR)pMenuItemData,TRUE);

		pMenuItemData ->sText        = lpszNewItem;
		pMenuItemData ->uID          = nIDNewItem;
		pMenuItemData ->hParentMenu  = m_hMenu;


		m_aMenuItem.Add(pMenuItemData);

		if((nFlags & MF_POPUP) && nIDNewItem)
		{
			pMenuItemData ->bPopUp = TRUE;
			pMenuItemData ->hMenu  = (HMENU)nIDNewItem;		
			m_lMinWidth            = max(m_lMinWidth,GetMenuWidth(pMenuItemData ->sText) + 10);
		}
	}
	return  bReturn; 
}
UINT   CTPMenuEx::GetMenuItemPos(UINT nID)
{
	for(INT l=0;l<(INT)GetMenuItemCount();l++)
	{
		if(GetMenuItemID(l) == nID) return l;
	}
	return -1;
}
BOOL  CTPMenuEx::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem,const CBitmap* pBmp)
{
	return FALSE;
}
int    CTPMenuEx::GetMenuString(UINT nIDItem, CString& rString, UINT nFlags)
{
	tag_TPMenuItemData *pMenuItemData = NULL;	
	if(nFlags & MF_BYPOSITION)
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nIDItem,TRUE);	
	else	
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nIDItem,FALSE);
	if(pMenuItemData)
	{
		rString = pMenuItemData ->sText;
		return rString.GetLength();
	}
	else return CMenu::GetMenuString(nIDItem,rString,nFlags);		
}
BOOL  CTPMenuEx::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem ,LPCTSTR lpszNewItem)
{	
	tag_TPMenuItemData *pMenuItemData = NULL;	
	UINT                uState        = 0;
	BOOL                bReturn       = FALSE;
	if(nFlags & MF_BYPOSITION)
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,TRUE);
		uState        = GetMenuState(nPosition,MF_BYPOSITION);
	}
	else
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,FALSE);
		uState        = GetMenuState(nPosition,MF_BYCOMMAND);
	}

	CString  sText               = lpszNewItem;
	if(pMenuItemData)
	{
		pMenuItemData ->sText        = lpszNewItem;
		pMenuItemData ->uID          = nIDNewItem;		
		
		int iTab = pMenuItemData->sText.Find('\t');
		if(iTab >=0) 
		{
			pMenuItemData->sExtText = pMenuItemData->sText.Mid(iTab+1);
			pMenuItemData->sText    = pMenuItemData->sText.Left(iTab);
			sText = pMenuItemData->sText  + _T("\\t") + pMenuItemData->sExtText;
		}
		CDC  *pDC  = AfxGetMainWnd() ->GetDC();
		CFont *pFont = (CFont *)pDC ->SelectObject(TP_GetSysFont(FONT_BASE_TEXT));		
		CRect   rtTextEx = CRect(0,0,0,0);	
		CSize   szText   = CSize(0,0);
		if(lstrlen(pMenuItemData->sExtText)>0)
		{
			pDC ->DrawText(pMenuItemData ->sExtText,rtTextEx,DT_CALCRECT);
			m_lMaxExtWidth = max(m_lMaxExtWidth,rtTextEx.Width());
		}
		szText = pDC->GetTextExtent(pMenuItemData->sText);
		m_lMinWidth = max(m_lMinWidth,szText.cx);

		pDC ->SelectObject(pFont);
		AfxGetMainWnd() ->ReleaseDC(pDC);	
	}	

	if(uState & MF_CHECKED)	     nFlags |= MF_CHECKED;
	if(uState & MF_DISABLED)	 nFlags |= MF_DISABLED;
	if(uState & MF_GRAYED)	     nFlags |= MF_GRAYED;

	if(MF_LOADSTR & nFlags) bReturn = TRUE;///27950
	else                    bReturn = CMenu::ModifyMenu(nPosition,nFlags|MF_OWNERDRAW, nIDNewItem ,(LPCTSTR)sText);

	if(nFlags & MF_BYPOSITION) SetMenuItemData(nPosition,(ULONG_PTR)pMenuItemData,TRUE);
	else                       SetMenuItemData(nIDNewItem,(ULONG_PTR)pMenuItemData,FALSE);

	return bReturn;
}
BOOL  CTPMenuEx::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem,const CBitmap* pBmp)
{
	return FALSE;
}
BOOL    CTPMenuEx::IsMenuPopup()
{
	return g_bMenuExPopUp;
}
BOOL   CTPMenuEx::TrackPopupMenu(UINT nFlags, int x, int y,CWnd* pWnd, LPCRECT lpRect)
{
//	if(TP_IsCtrlKeyDown() || TP_IsAltKeyDown()) return FALSE;  //modify by pyc 2007-8-13
	if(!m_bLoadString) TP_LoadMenuStr(this);	

	BOOL   bReturn = FALSE;
	g_bMenuExPopUp = TRUE;
	g_hMenuExPopUp = pWnd ->m_hWnd;
	m_uBaseState  |= TP_BASESTATE_MAINMENU;
	bReturn        = CMenu::TrackPopupMenu(nFlags,x,y,pWnd,lpRect);
	m_uBaseState  &= ~TP_BASESTATE_MAINMENU;
	g_bMenuExPopUp = FALSE;
	return bReturn ;
}
void   CTPMenuEx::SetMenuItemTextColor(UINT nPosition, UINT nFlags,COLORREF clText)
{
	tag_TPMenuItemData *pMenuItemData = NULL;	
	UINT                uState        = 0;
	BOOL                bReturn       = FALSE;
	if(nFlags & MF_BYPOSITION)
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,TRUE);
		uState        = GetMenuState(nPosition,MF_BYPOSITION);
	}
	else
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,FALSE);
		uState        = GetMenuState(nPosition,MF_BYCOMMAND);
	}

	if(pMenuItemData)	pMenuItemData ->clText        = clText;	
}
#ifdef TP_NS_EDIT
void   CTPMenuEx::EnableMenuItemsEx(UINT *puID,int iCount,UINT uFlag)
{
	for(INT k=0;k<iCount;k++)
	{
		if(uFlag & MF_BYPOSITION)	
		{
			if(puID[k] < (UINT)m_aMenuItem.GetSize() && m_aMenuItem[puID[k]]->bPopUp)
			{
				for(INT l=0;l<(int)m_aMenuItem[puID[k]]->pMenuEx->GetMenuItemCount();l++)
					m_aMenuItem[puID[k]]->pMenuEx->EnableMenuItem(l ,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
			}			
			else EnableMenuItem(puID[k] ,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);		
		}
		else EnableMenuItem(puID[k] ,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);		
	}			
}
#else
void   CTPMenuEx::EnableMenuItemsEx(UINT *puID,int iCount,UINT uFlag)
{
	for(INT k=0;k<iCount;k++)
	{
		if(uFlag & MF_BYPOSITION)	EnableMenuItem(puID[k] ,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);		
		else 	                    EnableMenuItem(puID[k] ,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);		
	}			
}
#endif
void   CTPMenuEx::EnableMenuItems(UINT *puID,int iCount,UINT uFlag)
{
	if(puID == NULL) return;
	int lCount = GetMenuItemCount();
	UINT uID   = 0;
	INT  l     = 0;
	for(l=0;l<lCount;l++)
	{
		uID = CMenu::GetMenuItemID(l);
		INT k=0;
		for(k=0;k<iCount;k++)
		{
			if(uID == puID[k]) break;
		}
		if(k>=iCount)
		{
			if(uFlag & MF_BYPOSITION)	EnableMenuItem(uID ,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
			else                      	EnableMenuItem(uID ,MF_BYCOMMAND |MF_DISABLED|MF_GRAYED);
		}
	}

}
void   CTPMenuEx::SetMenuItemIcon(UINT nPosition, UINT nFlags,LPBYTE pIcon,CSize szIcon,BOOL bButton)
{
	tag_TPMenuItemData *pMenuItemData = NULL;	
	if(nFlags & MF_BYPOSITION)
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,TRUE);		
	}
	else
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,FALSE);
	}
	if(pMenuItemData)	
	{
		if(pMenuItemData ->pIcon) delete pMenuItemData ->pIcon;
		pMenuItemData ->pIcon = new BYTE[szIcon.cx*szIcon.cy*4];
		memcpy(pMenuItemData ->pIcon,pIcon,szIcon.cx*szIcon.cy*4);
		pMenuItemData ->szIcon  = szIcon;
		pMenuItemData ->bButton = bButton;
	}

}
void   CTPMenuEx::SetMenuItemWidth(UINT nPosition, UINT nFlags,long lWidth)
{
	tag_TPMenuItemData *pMenuItemData = NULL;	
	if(nFlags & MF_BYPOSITION)
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,TRUE);		
	}
	else
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,FALSE);
	}
	if(pMenuItemData)	pMenuItemData ->lMinWidth      = lWidth;	
}
void   CTPMenuEx::SetMenuItemAttachMenu(UINT nPosition, UINT nFlags,CTPMenuEx *pMenu,BOOL bAutoDestory)
{
	tag_TPMenuItemData *pMenuItemData = NULL;	
	if(nFlags & MF_BYPOSITION)
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,TRUE);		
	}
	else
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,FALSE);
	}
	if(pMenuItemData)
	{
		pMenuItemData ->pMenuEx        = pMenu;	
		pMenuItemData ->bPopUp         = TRUE;
		pMenuItemData ->bAutoDestory   = bAutoDestory;
	}
}
void   CTPMenuEx::SetHotKeyData(TPHotKeyData *pHotKeyData,int iCount,CPoint *pIndex,int iIndexCount)
{
	CDC  *pDC  = AfxGetMainWnd() ->GetDC();
	CFont *pFont = (CFont *)pDC ->SelectObject(TP_GetSysFont(FONT_BASE_TEXT));			
	for(int i= GetMenuItemCount() -1;i>=0;i--)
	{
		UINT nID = GetMenuItemID(i);
		if(m_aMenuItem[i]->bPopUp && m_aMenuItem[i]->pMenuEx)
		{
			 m_aMenuItem[i]->pMenuEx->SetHotKeyData(pHotKeyData,iCount,pIndex,iIndexCount);
		}
		else if(nID !=0)
		{
			if(pIndex && iIndexCount>0)
			{
				for(INT k=0;k<iIndexCount;k++)
				{
					if(nID == pIndex[k].x) 
					{
						nID = pIndex[k].y;
						break;
					}
				}
			}

			for(int l=0;l<iCount;l++)
			{
				if(pHotKeyData[l].nID == nID)
				{
					tag_TPMenuItemData *pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(i,TRUE);		
					if(pMenuItemData && pMenuItemData->sExtText.GetLength()<=0)
					{
						pMenuItemData->sExtText = TP_GetVKName(pHotKeyData[l].vkCharSet,pHotKeyData[l].uFlagSet);
						CRect   rtTextEx = CRect(0,0,0,0);		
						pDC ->DrawText(pMenuItemData ->sExtText,rtTextEx,DT_CALCRECT);
						m_lMaxExtWidth = max(m_lMaxExtWidth,rtTextEx.Width());
					}
				}
			}
		}
	}
	pDC ->SelectObject(pFont);
	AfxGetMainWnd() ->ReleaseDC(pDC);
}
BOOL  CTPMenuEx::SetMenuItemImage(UINT nPosition, UINT nFlags,COLORREF eColor,BOOL bCheck,BYTE bTextMode)
{
	tag_TPMenuItemData *pMenuItemData = NULL;
	if(nFlags & MF_BYPOSITION)
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,TRUE);		
	}
	else
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,FALSE);
	}
	if(pMenuItemData)	
	{
		pMenuItemData ->cImage       = eColor;			
		pMenuItemData ->bTextMode    = bTextMode;
		pMenuItemData ->bCheck       = bCheck;
		if(bCheck) CheckMenuItem(nPosition,nFlags|MF_CHECKED);
	}
	return TRUE;		 
}
BOOL  CTPMenuEx::SetMenuItemImage(UINT nPosition, UINT nFlags,TPCommonRes *pImage,int iImageIndex,BOOL bCheck)
{
	tag_TPMenuItemData *pMenuItemData = NULL;
	if(nFlags & MF_BYPOSITION)
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,TRUE);		
	}
	else
	{
		pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(nPosition,FALSE);
	}
	if(pMenuItemData)	
	{
		pMenuItemData ->pImage       = pImage;	
		pMenuItemData ->iImageIndex  = iImageIndex;
		pMenuItemData ->bCheck       = bCheck;
	}
	return TRUE;
}
BOOL  CTPMenuEx::SetMenuItemBitmaps(UINT nPosition, UINT nFlags,const CBitmap* pBmpUnchecked, const CBitmap* pBmpChecked)
{
	return TRUE;
}
ULONG_PTR CTPMenuEx::GetMenuItemData(UINT_PTR uItem,BOOL byPositon)
{
	MENUITEMINFO menuItemInfo;
	ZeroMemory(&menuItemInfo,sizeof(MENUITEMINFO));
	menuItemInfo.cbSize = sizeof(MENUITEMINFO);
	menuItemInfo.fMask  = MIIM_DATA;		
	if(CMenu::GetMenuItemInfo((UINT)uItem,&menuItemInfo,byPositon))
		return menuItemInfo.dwItemData;
	else 
		return 0;
}
void CTPMenuEx::SetMenuItemData(UINT_PTR uItem,ULONG_PTR dwData,BOOL byPositon)
{
	MENUITEMINFO menuItemInfo;
	ZeroMemory(&menuItemInfo,sizeof(MENUITEMINFO));
	menuItemInfo.cbSize = sizeof(MENUITEMINFO);
	menuItemInfo.fMask  = MIIM_DATA;		
	menuItemInfo.dwItemData = (ULONG_PTR)dwData;
	CMenu::SetMenuItemInfo((UINT)uItem,&menuItemInfo,byPositon);	
}
BOOL  CTPMenuEx::CopyMenu(HMENU hMenu)
{
	m_bLoadString  = TRUE;
	if(hMenu == NULL) return FALSE;		
	int     iCount = ::GetMenuItemCount(hMenu);
	if(iCount<=0)    return FALSE;
	UINT         nID    = 0; 	
	UINT         uState = 0;
	TCHAR        cText[200];	
	int          iItem  = 0; 
	for(int i= 0;i<iCount;i++)
	{		
		nID = ::GetMenuItemID(hMenu,i);
		if(nID !=0)
		{
			::GetMenuString(hMenu,i,cText,200,MF_BYPOSITION);	
			uState = ::GetMenuState(hMenu,i,MF_BYPOSITION);		
			if(nID == 0xffffffff)
			{			
				CTPMenuEx *pPopMenu = new CTPMenuEx();								
				pPopMenu ->CreatePopupMenu();
				HMENU hSubMenu = ::GetSubMenu(hMenu,i);
				pPopMenu ->CopyMenu(hSubMenu);
				AppendMenu(MF_BYCOMMAND|MF_POPUP,(UINT_PTR)pPopMenu->GetSafeHmenu(),cText);					
				SetMenuItemAttachMenu(iItem,MF_BYPOSITION,pPopMenu,TRUE);
			}
			else
			{
				AppendMenu(MF_BYCOMMAND|MF_STRING,nID,cText);					
			}
			if(uState &	MF_CHECKED)
				CheckMenuItem(iItem,MF_BYPOSITION|MF_CHECKED); 
			if(uState &	MF_DISABLED)
				EnableMenuItem(iItem,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
			iItem ++;
		}
		else
		{
			AppendMenu(MF_SEPARATOR);
			iItem ++;
		}
	}
	return TRUE;
}
void CTPMenuEx::InitMenu(HMENU hMenu,CDC *pDCSetEx)
{
	MENUITEMINFO        stuItemInfo;
	tag_TPMenuItemData *pMenuItemData = NULL;
	CTPMenuEx          *pMenu         = NULL;
	CDC                *pDCSet        = NULL;
	TCHAR               cText[200];
	if(hMenu    == NULL) return;
	int  uCount     = ::GetMenuItemCount(hMenu);	

	if(pDCSetEx == NULL)
	{	
		pDCSet          = new CDC();
		BOOL bRet = pDCSet ->CreateCompatibleDC(NULL);
		ASSERT(bRet);
	}
	else pDCSet = pDCSetEx;

	m_bLoadString = TRUE;
	
	CSize szText = CSize(0,0);
	int   iTab   = -1;
	for(int l=0;l<uCount;l++)
	{
		ZeroMemory(&stuItemInfo,sizeof(MENUITEMINFO));
		stuItemInfo.cbSize     = sizeof(MENUITEMINFO);
		stuItemInfo.fMask      = MIIM_FTYPE|MIIM_SUBMENU | MIIM_ID|MIIM_STRING;
		stuItemInfo.cch        = 200;
		stuItemInfo.dwTypeData = cText;
		ZeroMemory(cText,200*sizeof(TCHAR));
		::GetMenuItemInfo(hMenu,l,TRUE,&stuItemInfo);
		stuItemInfo.fType |=  MF_OWNERDRAW;
//		stuItemInfo.fMask &= ~(MIIM_SUBMENU | MIIM_ID/*|MIIM_STRING*/);
		stuItemInfo.fMask  =  MIIM_DATA | MIIM_FTYPE|MIIM_STRING;
		pMenuItemData = new tag_TPMenuItemData(this);		
		pMenuItemData->uID         = stuItemInfo.wID;		
		pMenuItemData->sText       = CTPBaseCtrl::m_stuSBDictionary.SBLoadString(stuItemInfo.dwTypeData);
		pMenuItemData->hParentMenu = m_hMenu;
		stuItemInfo.dwTypeData     = (LPTSTR)(LPCTSTR)pMenuItemData->sText;
		stuItemInfo.cch            = pMenuItemData->sText.GetLength();
		stuItemInfo.dwItemData = (ULONG_PTR)pMenuItemData;
		::SetMenuItemInfo(hMenu,l,TRUE,&stuItemInfo);
		iTab = pMenuItemData->sText.Find('\t');
		if(iTab >=0) 
		{
			pMenuItemData->sExtText = pMenuItemData->sText.Mid(iTab+1);
			pMenuItemData->sText    = pMenuItemData->sText.Left(iTab);
			szText = pDCSet->GetTextExtent(pMenuItemData->sExtText);
			m_lMaxExtWidth = max(m_lMaxExtWidth,szText.cx);
			szText         = CSize(0,0) ;
		}
		szText = pDCSet->GetTextExtent(pMenuItemData->sText);
		m_lMinWidth = max(m_lMinWidth,szText.cx);
		if(stuItemInfo.hSubMenu) 
		{
			pMenu = new CTPMenuEx();
			pMenuItemData ->pMenuEx = pMenu;
			pMenuItemData ->bPopUp  = TRUE;
			pMenuItemData ->hMenu   = stuItemInfo.hSubMenu;
			pMenu ->Attach(stuItemInfo.hSubMenu);
			pMenu ->InitMenu(stuItemInfo.hSubMenu,pDCSet);					
		}
		m_aMenuItem.Add(pMenuItemData);

	}	
	if(pDCSetEx == NULL)
	{	
		pDCSet ->DeleteDC();
		delete pDCSet;	
	}
}
void  CTPMenuEx::SetType()
{
	if(GetMenuItemCount() < 4) return; 
	SetType(TP_MENU_LEFTBAR);//|TP_MENU_UNCHECK
	SetBarRes(_L(""));
}
void  CTPMenuEx::SetType(DWORD dwType)
{
	m_dwType = dwType;

	if((m_dwType & TP_MENU_LEFTBAR) && m_pLeftBar)
		m_lBarWidth = m_pLeftBar ->Width()-2;

	if(m_dwType & TP_MENU_UNCHECK)
		m_lCheckWidth = 0;
}
void   CTPMenuEx::PaintMenu(CDC *pDC,CRect rtClient)
{
	if(m_dwType & TP_MENU_LEFTBAR)
	{		
		rtClient.InflateRect(1,1);
		rtClient.right = rtClient.left + m_lBarWidth;
		if(m_pLeftBar)
		{
			TP_SetDIBRect(pDC,m_pLeftBar ->GetByte(),m_pLeftBar ->Width(),m_pLeftBar ->Height(),80,rtClient,TRUE,FALSE,TRUE);
		}
		else pDC  -> FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));				
	}	
}
void    CTPMenuEx::SetBarRes(CString sFileName)
{
	TP_ReleaseCommonRes(m_pLeftBar);
	m_pLeftBar = NULL;

	if(sFileName.GetLength()<=0) 
#ifdef TP_NS_EDIT
		sFileName = TP_GetSkinPath() + _L("\\CommonCtrl\\Menu\\Menu_XpriNS.bmp");
#elif  TP_MINI_EDIT 
		sFileName = TP_GetSkinPath() + _L("\\CommonCtrl\\Menu\\MenulogoMini.bmp");
#else
		sFileName = TP_GetSkinPath() + _L("\\CommonCtrl\\Menu\\Menulogo.bmp");				
#endif

	m_pLeftBar = TP_LoadBmpByte(sFileName);
	if((m_dwType & TP_MENU_LEFTBAR) && m_pLeftBar)
		m_lBarWidth = m_pLeftBar ->Width()-2;
}
void     CTPMenuEx::AppendMenu(int iIndex,CTPMenuEx *pSubMenu)
{
	if(pSubMenu == NULL) return;
	if(iIndex <0) iIndex = CMenu::GetMenuItemCount();

	CString sText;
	UINT    nID = 0;
	for(UINT l=0;l<pSubMenu ->GetMenuItemCount();l++)
	{		
		pSubMenu ->GetMenuString(l,sText,MF_STRING|MF_BYPOSITION);
		nID = pSubMenu ->GetMenuItemID(l);
		if(nID == 0)                InsertMenu(l+iIndex,MF_BYPOSITION|MF_SEPARATOR,0,_L(""));
		else if(nID == 0xffffffff)	
		{
			InsertMenu(l+iIndex,MF_BYPOSITION|MF_POPUP,(UINT_PTR)pSubMenu ->GetSubMenu(l)->GetSafeHmenu(),sText);
	//		CTPMenuEx *pSrc = pSubMenu ->GetSubMenu(l);
	//		SetMenuItemAttachMenu(l+iIndex,MF_BYPOSITION,pSrc);
		}
		else                        InsertMenu(l+iIndex,MF_BYPOSITION|MF_STRING,nID,sText);
	}	
}
void      CTPMenuEx::CopyTo(CTPMenuEx *pSubMenu)
{
	pSubMenu ->m_dwVertify    = m_dwVertify;
	pSubMenu ->m_iBackColor   = m_iBackColor;
	pSubMenu ->m_iTopLeft     = m_iTopLeft;
	pSubMenu ->m_iRightBottom = m_iRightBottom;

	pSubMenu ->m_iText        = m_iText;
	pSubMenu ->m_iTextDisable = m_iTextDisable;
	pSubMenu ->m_iTextHight   = m_iTextHight;

	pSubMenu ->m_dwType       = m_dwType;
	pSubMenu ->m_lBarWidth    = m_lBarWidth;
	pSubMenu ->m_lCheckWidth  = m_lCheckWidth;   
	pSubMenu ->m_hAttachWnd   = m_hAttachWnd;
	pSubMenu ->m_lMinWidth    = m_lMinWidth;
}


BOOL CTPMenuEx::SetItemData(UINT uiId, DWORD_PTR dwItemData, BOOL fByPos)
{  
	tag_TPMenuItemData  *pMenuItemData = NULL;
	if(fByPos) pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(uiId,TRUE);
	else       pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(uiId,FALSE);
	if(pMenuItemData)
	{
		pMenuItemData ->dwData = dwItemData;
		return TRUE;
	}
	else return FALSE;

}
DWORD_PTR CTPMenuEx::GetItemData(UINT uiId, BOOL fByPos) 
{
	tag_TPMenuItemData  *pMenuItemData = NULL;
	if(fByPos) pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(uiId,TRUE);
	else       pMenuItemData = (tag_TPMenuItemData *)GetMenuItemData(uiId,FALSE);
	if(pMenuItemData)	return pMenuItemData ->dwData;		
	else return NULL;
}
int  CTPMenuEx::GetMenuMinWidth()
{		
	int        iCount = GetMenuItemCount();
	UINT       nID    = 0; 	
	CString    sText  = _L("");	
	for(int i=iCount-1;i>=0;i--)
	{
		nID = GetMenuItemID(i);
		if(nID !=0)
		{
			GetMenuString(i,sText,MF_BYPOSITION);
			if(sText.GetLength()<=0) continue;			
			m_lMinWidth = max(m_lMinWidth,GetMenuWidth(sText)+  10); 						
		}
	}
	return m_lMinWidth;
}
BOOL CTPMenuEx::DeleteMenuEx(UINT nPosition, UINT nFlags)
{
	UINT uCheck = 0;
	if(nFlags & MF_BYPOSITION)	
	{
		if(nPosition < (UINT)m_aMenuItem.GetSize() && m_aMenuItem[nPosition]->bPopUp)
		{
			for(INT l=0;l<(int)m_aMenuItem[nPosition]->pMenuEx->GetMenuItemCount();l++)
				m_aMenuItem[nPosition]->pMenuEx->DeleteMenu(nPosition ,nFlags);
			uCheck = nPosition;
		}			
		else uCheck = DeleteMenu(nPosition ,nFlags);		
	}
	else uCheck = DeleteMenu(nPosition ,nFlags);	

	if(uCheck == 0xFFFFFFFF || uCheck == 0)
	{
		for(INT l=0;l<m_aMenuItem.GetSize();l++)
		{
			if(m_aMenuItem[l]->pMenuEx == NULL || !m_aMenuItem[l]->bPopUp) continue;
			uCheck = m_aMenuItem[l]->pMenuEx->DeleteMenuEx(nPosition ,nFlags);
			if(uCheck != 0xFFFFFFFF &&  uCheck != 0) return uCheck;
		}		
	}
	return uCheck;
}
#ifdef TP_NS_EDIT
UINT CTPMenuEx::EnableMenuItemEx(UINT nIDEnableItem, UINT nEnable)
{
	UINT uCheck = 0;
	if(nEnable & MF_BYPOSITION)	
	{
		if(nIDEnableItem < (UINT)m_aMenuItem.GetSize() && m_aMenuItem[nIDEnableItem]->bPopUp)
		{
			for(INT l=0;l<(int)m_aMenuItem[nIDEnableItem]->pMenuEx->GetMenuItemCount();l++)
				m_aMenuItem[nIDEnableItem]->pMenuEx->EnableMenuItem(l ,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
			uCheck = nIDEnableItem;
		}			
		else uCheck = EnableMenuItem(nIDEnableItem ,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);		
	}
	else uCheck = EnableMenuItem(nIDEnableItem ,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);	

	if(uCheck == 0xFFFFFFFF || uCheck == 0)
	{
		for(INT l=0;l<m_aMenuItem.GetSize();l++)
		{
			if(m_aMenuItem[l]->pMenuEx == NULL || !m_aMenuItem[l]->bPopUp) continue;
			uCheck = m_aMenuItem[l]->pMenuEx->EnableMenuItemEx(nIDEnableItem,nEnable);
			if(uCheck != 0xFFFFFFFF &&  uCheck != 0) return uCheck;
		}		
	}
	return uCheck;
}
#else
UINT CTPMenuEx::EnableMenuItemEx(UINT nIDEnableItem, UINT nEnable)
{
	UINT uCheck = CMenu::EnableMenuItem(nIDEnableItem,nEnable);
	if(uCheck == 0xFFFFFFFF || uCheck == 0)
	{
		for(INT l=0;l<m_aMenuItem.GetSize();l++)
		{
			if(m_aMenuItem[l]->pMenuEx == NULL || !m_aMenuItem[l]->bPopUp) continue;
			uCheck = m_aMenuItem[l]->pMenuEx->EnableMenuItem(nIDEnableItem,nEnable);
			if(uCheck != 0xFFFFFFFF &&  uCheck != 0) return uCheck;
		}		
	}
	return uCheck;
}
#endif
void CTPMenuEx::SetCheckImg(CString sCheck)
{
	if(m_pResCheck) TP_ReleaseCommonRes(m_pResCheck);
	m_pResCheck    = TP_LoadImage(sCheck,12,RGB(53,53,53));
}
UINT CTPMenuEx::CheckMenuItemEx(UINT nIDCheckItem, UINT nCheck)
{
	UINT uCheck = CMenu::CheckMenuItem(nIDCheckItem,nCheck);
	if(uCheck == 0xFFFFFFFF || uCheck == 0)
	{
		for(INT l=0;l<m_aMenuItem.GetSize();l++)
		{
			if(m_aMenuItem[l]->pMenuEx == NULL || !m_aMenuItem[l]->bPopUp) continue;
			uCheck = m_aMenuItem[l]->pMenuEx->CheckMenuItemEx(nIDCheckItem,nCheck);
			if(uCheck != 0xFFFFFFFF &&  uCheck != 0) return uCheck;
		}		
	}
	return uCheck;
}
void CTPMenuEx::ClearMenu()
{	
	CTPMenuEx *pMenu = this;
	int  iCount = pMenu->GetMenuItemCount();
	UINT nID    = 0; 
	int  i      = 0;
	for(i=iCount-1;i>=0;i--)
	{
		nID = pMenu ->GetMenuItemID(i);
		if(nID !=0)
		{
			UINT  uState = pMenu ->GetMenuState(i,MF_BYPOSITION);
			if((uState & MF_DISABLED) ==  MF_DISABLED)
				pMenu ->DeleteMenu(i,MF_BYPOSITION);
		}
	}
	iCount = pMenu ->GetMenuItemCount();
	for(i=iCount-1;i>=0;i--)
	{
		nID = pMenu ->GetMenuItemID(i);
		if(nID ==0)
		{
			if(i == 0 ||i== (int)pMenu ->GetMenuItemCount()-1 )
				pMenu ->DeleteMenu(i,MF_BYPOSITION);
			else
			{	
				nID = pMenu ->GetMenuItemID(i-1);
				if(nID ==0) pMenu ->DeleteMenu(i,MF_BYPOSITION);
			}
		}
	}
}
void CTPMenuEx::AddColorMenu(CDWordArray &aColor,DWORD iSel,int iPosition,UINT uBaseID)
{
	CTPMenuEx *pMenu = new CTPMenuEx();
	pMenu ->CreatePopupMenu();
	for(INT l=0;l<aColor.GetSize();l++)
	{
		pMenu->AppendMenu(MF_BYCOMMAND|MF_STRING,uBaseID + l,_T("RGB"));				
		pMenu->SetMenuItemImage(l,MF_BYPOSITION,aColor[l],iSel == aColor[l]);
	}
	InsertMenu(iPosition,MF_BYPOSITION|MF_POPUP,(UINT_PTR)pMenu ->GetSafeHmenu(),TP_LoadStr(_T("IDS_MENU_COLORSEL")));
	SetMenuItemAttachMenu(iPosition,MF_BYPOSITION,pMenu,TRUE);		
}
BOOL CTPMenuEx::ApplyKey(UINT vKey)
{
	if(g_hMenuExPopUp == NULL) return FALSE;
	CString sKey   = TP_GetVKName(vKey,0);
	UINT    uKey   = 0;
	UINT    nState = 0;
	if(TP_IsCtrlKeyDown())  uKey|= TP_CTRL;
	if(TP_IsAltKeyDown())   uKey|= TP_MENU;
	if(TP_IsShiftKeyDown()) uKey|= TP_SHIFT;
	sKey.MakeUpper();
	for(INT l=0;l<m_aMenuItem.GetSize();l++)
	{
		nState = GetMenuState(m_aMenuItem[l]->uID,MF_BYCOMMAND);
		if(nState & (MF_DISABLED|MF_GRAYED)) continue;
		if(m_aMenuItem[l]->sExtText.GetLength()>0)
		{
			UINT    uState = 0;
			CString sText  = m_aMenuItem[l]->sExtText;			
			int     iPlus  = FALSE; 
			sText.MakeUpper();
			sText.Replace(_T("DEL"),_T("DELETE"));
			if(sText.Find(_T("CTRL"))>=0)  uState|= TP_CTRL;
			if(sText.Find(_T("ALT"))>=0)   uState|= TP_MENU;
			if(sText.Find(_T("SHIFT"))>=0) uState|= TP_SHIFT;
			iPlus = sText.ReverseFind('+');
			if(iPlus>=0) sText = sText.Mid(iPlus+1);
			sText.Trim();
			if(sText == sKey && uState == uKey)
			{			
				PostMessage(g_hMenuExPopUp, WM_COMMAND,m_aMenuItem[l]->uID,0);
				TP_CloseMenu();
				return TRUE;
			}			
		}
	}
	return FALSE;
}
void CTPMenuEx::SetEnableID(CUintArray &aID)
{
	
	int     iCount = GetMenuItemCount();
	UINT    nID    = 0; 	
	CString sText  = _L("");
	for(int i=iCount-1;i>=0;i--)
	{
		nID = GetMenuItemID(i);
		if(nID !=0)
		{			
			if(nID == 0xffffffff)
			{				
				CTPMenuEx  *pMenuTemp = NULL;
				pMenuTemp = GetSubMenu(i);
				if(pMenuTemp) 
				{
					pMenuTemp->SetEnableID(aID);
				}
			}
			else
			{
				INT l=0;
				for(l=0;l<aID.GetSize();l++)
					if(aID[l] == nID) break;
				if(l>=aID.GetSize())
					EnableMenuItem(nID,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
			}

		}
	}
}
void CTPMenuEx::CheckPopUp()
{
	int     iCount = GetMenuItemCount();
	UINT    nID    = 0; 	 
 	m_uBaseState &= ~TP_BASESTATE_NOSPARTEBAR;
	if(iCount<=0) return;
	for(int i=iCount-1;i>=0;i--)
	{
		nID = GetMenuItemID(i);
		if(nID !=0xffffffff) return;
	}
	AppendMenu(MF_SEPARATOR);
	m_uBaseState |= TP_BASESTATE_NOSPARTEBAR;
}

namespace _tp_commonctrl
{
	BOOL CALLBACK TP_EnumChildProc(HWND hwnd,LPARAM lParam)
	{
		SendMessage(hwnd,WM_CANCELMODE,0,0);
		return TRUE;
	}
	BOOL CALLBACK TP_EnumWindowsProc(HWND hwnd,LPARAM lParam)
	{
		EnumChildWindows(hwnd,TP_EnumChildProc,lParam);
		return TRUE;
	}
	void TP_CloseMenu(HWND hWnd)
	{
		//if(AfxGetMainWnd())
		//EnumChildWindows(AfxGetMainWnd()->GetSafeHwnd(),TP_EnumChildProc,NULL);
		//EnumWindows(TP_EnumWindowsProc,NULL);
		if(g_hMenuExPopUp && ::IsWindow(g_hMenuExPopUp))
			SendMessage(g_hMenuExPopUp,WM_CANCELMODE,0,0);
		/*CArray<HWND,HWND &> aMenuWnd; 
		aMenuWnd.Copy(g_hMenuWnd);
		for(INT l=0;l<aMenuWnd.GetSize();l++)
		{
		if(::IsWindow(aMenuWnd[l]))
		::SendMessage(aMenuWnd[l],WM_CLOSE,0,0);
		}*/
	}
}