
#include "stdafx.h"
#include "TPCommonCtrlhook.h"
#include  <intrin.h> 

#include "Imagehlp.h"
#pragma comment (lib,"Imagehlp.lib")


#include "detours.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define TP_SB_WND            0xffffffffL

#define TP_NS_MEMORY

#define TP_OK             1
#define TP_ERROR         -1
#define TP_ALL          0x7fffffff

CTPMenuEx         *g_pPopMenu           = NULL;
void             (*g_pPopMenuOperate)(CTPMenuEx * pMenu) = NULL;  
namespace _tp_commonctrl
{
	TPScrollBarDataArray   g_aAttachWnd;
	TPScrollBarDataArray   g_aAttachScrollBar;
	TPScrollBarCallBack    g_pScrollBarCallBack;

	

	DETOUR_TRAMPOLINE(BOOL WINAPI Detour_EnableScrollBar  (HWND hwnd, int wSBflags, UINT wArrows), EnableScrollBar);
	DETOUR_TRAMPOLINE(BOOL WINAPI Detour_GetScrollInfo	  (HWND hwnd, int fnBar, LPSCROLLINFO lpsi), GetScrollInfo);
	DETOUR_TRAMPOLINE(int  WINAPI Detour_GetScrollPos	  (HWND hwnd, int nBar), GetScrollPos);	
	DETOUR_TRAMPOLINE(int  WINAPI Detour_SetScrollInfo	  (HWND hwnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw), SetScrollInfo);
	DETOUR_TRAMPOLINE(int  WINAPI Detour_SetScrollPos	  (HWND hwnd, int nBar, int nPos, BOOL fRedraw), SetScrollPos);
	DETOUR_TRAMPOLINE(int  WINAPI Detour_SetScrollRange	  (HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL fRedraw), SetScrollRange);
	DETOUR_TRAMPOLINE(BOOL WINAPI Detour_ShowScrollBar	  (HWND hwnd, int wBar, BOOL fShow), ShowScrollBar);
	DETOUR_TRAMPOLINE(int   WINAPI Detour_BringWindowToTop(HWND hwnd), BringWindowToTop);
	DETOUR_TRAMPOLINE(HWND  WINAPI Detour_SetParent       (HWND hWndChild,HWND hWndNewParent), SetParent);
	DETOUR_TRAMPOLINE(BOOL  WINAPI Detour_MoveWindow      (HWND hWnd,int X,int Y,int nWidth,int nHeight,BOOL bRepaint), MoveWindow);
	DETOUR_TRAMPOLINE(BOOL  WINAPI Detour_ShowWindow      (HWND hWnd, int nCmdShow), ShowWindow);
	DETOUR_TRAMPOLINE(BOOL  WINAPI Detour_SetWindowPos    (HWND hWnd,HWND hWndInsertAfter,int X,int Y,int cx,int cy,UINT uFlags), SetWindowPos);
	DETOUR_TRAMPOLINE(DWORD WINAPI Detour_GetSysColor     (int nIndex), GetSysColor);
	DETOUR_TRAMPOLINE(BOOL  WINAPI Detour_PathFileExists  (LPCTSTR pszPath), PathFileExists);
	DETOUR_TRAMPOLINE(int   WINAPI Detour_MessageBox      (HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType), MessageBox);
 
	DETOUR_TRAMPOLINE(BOOL  WINAPI Detour_TrackPopupMenuEx(HMENU hMenu,UINT uFlags,int x,int y,HWND hWnd,LPTPMPARAMS prcRect), TrackPopupMenuEx);

	void inline TP_CalCWindowPos( TPScrollBarData *pScrollBarData,BOOL bHShow,BOOL bVShow)
	{
		ASSERT(pScrollBarData);
		int bSaveHShow = pScrollBarData ->bShowHScrollBar;
		int bSaveVShow = pScrollBarData ->bShowVScrollBar;

		bHShow = bHShow & pScrollBarData ->bForceHScrollBar;
		bVShow = bVShow & pScrollBarData ->bForceVScrollBar;

		pScrollBarData ->bShowHScrollBar = bHShow;
		pScrollBarData ->bShowVScrollBar = bVShow;

		int Xoffset = pScrollBarData ->iScrollWidth ;
		int Yoffset = pScrollBarData ->iScrollHeight;

		CWnd *pHScrollBar = pScrollBarData ->pHScroll;
		CWnd *pVScrollBar = pScrollBarData ->pVScroll;
		CWnd *pWnd        = CWnd::FromHandle(pScrollBarData ->hAttachWnd);
		BOOL  bWindowShow = pWnd ->IsWindowVisible() && pWnd ->GetParent() ->IsWindowVisible();
		BOOL  bLeftScroll = FALSE;
		pScrollBarData -> bShowAttachWnd = bWindowShow; 
		CRect rtClient    = CRect(0,0,0,0),rtWnd,rtWndCalc;
		CWnd* pParentWnd  = pWnd->GetParent();

		pWnd->GetClientRect(&rtClient);
		pWnd->ClientToScreen(&rtClient);
		pWnd ->GetWindowRect(&rtWnd);	
		rtWndCalc = rtWnd;

		bLeftScroll = rtClient.left - rtWnd.left  > rtWnd.right - rtClient.right;
		if(rtWnd.bottom - rtClient.bottom <= rtClient.top - rtWnd.top)  bHShow = FALSE; 

		if(::IsWindow(pParentWnd->GetSafeHwnd()))
			pParentWnd->ScreenToClient(&rtClient);

		if(!bWindowShow)
		{
			Detour_ShowWindow(pHScrollBar->m_hWnd,SW_HIDE);
			Detour_ShowWindow(pVScrollBar->m_hWnd,SW_HIDE);
		}		

		if(bHShow&&bVShow)
		{
			MoveWindow(
				pHScrollBar->m_hWnd,
				rtClient.left,
				rtClient.bottom ,
				rtClient.Width(),
				Yoffset,
				TRUE);

			if(bLeftScroll)
				MoveWindow(
				pVScrollBar->m_hWnd,
				rtClient.left-Xoffset,
				rtClient.top,
				Xoffset,
				rtClient.Height(),
				TRUE);
			else
				MoveWindow(
				pVScrollBar->m_hWnd,
				rtClient.right,
				rtClient.top,
				Xoffset,
				rtClient.Height(),
				TRUE);

			if(!bWindowShow)
				return;
			Detour_ShowWindow(pHScrollBar->m_hWnd,SW_SHOW);
			Detour_ShowWindow(pVScrollBar->m_hWnd,SW_SHOW);
			return;
		}
		else if(bHShow && !bVShow)
		{
			if(pVScrollBar)
				Detour_ShowWindow(pVScrollBar->m_hWnd,SW_HIDE);
			ASSERT(pHScrollBar);

			MoveWindow(
				pHScrollBar->m_hWnd,
				rtClient.left,
				rtClient.bottom,
				rtClient.Width(),
				Yoffset,
				TRUE);

			if(bWindowShow)
				Detour_ShowWindow(pHScrollBar->m_hWnd,SW_SHOW);
			return;
		}
		else if(!bHShow && bVShow)
		{
			if(pHScrollBar)		Detour_ShowWindow(pHScrollBar->m_hWnd,SW_HIDE);
			ASSERT(pVScrollBar);

			if(bLeftScroll)
				MoveWindow(
				pVScrollBar->m_hWnd,
				rtClient.left -  Xoffset ,
				rtClient.top,
				Xoffset,
				rtClient.Height(),
				TRUE);
			else
				MoveWindow(
				pVScrollBar->m_hWnd,
				rtClient.right  ,
				rtClient.top,
				Xoffset,
				rtClient.Height(),
				TRUE);
			if(bWindowShow)	Detour_ShowWindow(pVScrollBar->m_hWnd,SW_SHOW);	
			return;
		}
		else if(!bHShow && !bVShow)
		{
			if(pHScrollBar)		Detour_ShowWindow(pHScrollBar->m_hWnd,SW_HIDE);
			if(pVScrollBar)		Detour_ShowWindow(pVScrollBar->m_hWnd,SW_HIDE);
			return;
		}
	}


	int inline TP_GetAttcahWndIndex(HWND hWnd,int fnBar)
	{
		if(fnBar == SB_CTL)	return TP_ERROR;   
		if(g_aAttachWnd.GetSize() == 0) return TP_ERROR;


		for(int i = 0;i<g_aAttachWnd.GetSize();i++)
		{
			if(hWnd == g_aAttachWnd[i]->hAttachWnd)
			{
				if(!::IsWindow(hWnd))
				{
					TP_UnReplaceWnd(CWnd::FromHandle(g_aAttachWnd[i]->hAttachWnd));
					return TP_ERROR;
				}
				return i;
			} 
		}
		return TP_ERROR;
	}


	int inline TP_GetScrollBar(HWND hWnd)
	{	
		if(g_aAttachScrollBar.GetSize() == 0) return TP_ERROR;
		for(int i = 0;i<g_aAttachScrollBar.GetSize();i++)
		{
			if(hWnd == g_aAttachScrollBar[i]->hAttachWnd )//in array
			{
				if(!::IsWindow(hWnd))
				{
					TP_UnReplaceScrollBar(CWnd::FromHandle(g_aAttachScrollBar[i]->hAttachWnd));
					return TP_ERROR;
				}
				return i;
			} 
		}	
		return TP_ERROR;
	}
	extern "C"
		BOOL APIENTRY TP_TrackPopupMenuEx(HMENU hMenu,UINT uFlags,int x,int y,HWND hWnd,LPTPMPARAMS prcRect)
	{
		if(g_pPopMenu)
		{				
			g_pPopMenu ->Attach(hMenu);									
			g_pPopMenu ->InitMenu(hMenu);					
			if(g_pPopMenuOperate) g_pPopMenuOperate(g_pPopMenu);
			BOOL bReturn = Detour_TrackPopupMenuEx(hMenu,uFlags,x,y,hWnd,prcRect);
			g_pPopMenu->Detach();
			return bReturn;
		}
		else 
			return Detour_TrackPopupMenuEx(hMenu,uFlags,x,y,hWnd,prcRect);
	}

	extern "C"
		int APIENTRY TP_BringWindowToTop(HWND hWnd)
	{
		BOOL bReturn = Detour_BringWindowToTop(hWnd);

		int ni = TP_GetAttcahWndIndex(hWnd,TP_ALL);
		if(ni >= 0)
		{
			CWnd* pHScrollBar = g_aAttachWnd[ni] -> pHScroll;
			CWnd* pVScrollBar = g_aAttachWnd[ni] -> pVScroll;

			if(g_aAttachWnd[ni] ->bShowHScrollBar && g_aAttachWnd[ni] ->bShowVScrollBar)
			{
				pHScrollBar->BringWindowToTop();
				pVScrollBar->BringWindowToTop();
			}
			else if(g_aAttachWnd[ni] ->bShowHScrollBar)
			{
				pHScrollBar->BringWindowToTop();
			}
			else if(g_aAttachWnd[ni] ->bShowVScrollBar)
			{
				pVScrollBar->BringWindowToTop();
			}
		}
		return bReturn;
	}
	BOOL CALLBACK TP_EnumChildWndProc(HWND hWndChild,LPARAM lParam)
	{
		int ni = TP_GetAttcahWndIndex(hWndChild, TP_ALL);
		int nCmdShow = (int)lParam;
		if(ni >= 0)
		{
			if(nCmdShow == SW_HIDE || nCmdShow == SW_SHOW || SW_SHOWNORMAL == nCmdShow)
			{
				g_aAttachWnd[ni] -> bShowAttachWnd  = ::IsWindowVisible(hWndChild);	
				if(g_aAttachWnd[ni] -> bShowAttachWnd)
				{
					CWnd* pHScrollBar = g_aAttachWnd[ni] -> pHScroll;
					CWnd* pVScrollBar = g_aAttachWnd[ni] -> pVScroll;

					if(g_aAttachWnd[ni] ->bShowHScrollBar  && g_aAttachWnd[ni] ->bShowVScrollBar)
					{
						Detour_ShowWindow(pHScrollBar->m_hWnd, nCmdShow);
						Detour_ShowWindow(pVScrollBar->m_hWnd, nCmdShow);
					}
					else if(g_aAttachWnd[ni] ->bShowHScrollBar)
					{			
						Detour_ShowWindow(pHScrollBar->m_hWnd, nCmdShow);

						if(pVScrollBar)
							Detour_ShowWindow(pVScrollBar->m_hWnd, SW_HIDE);
					}
					else if(g_aAttachWnd[ni] ->bShowVScrollBar)
					{
						Detour_ShowWindow(pVScrollBar->m_hWnd, nCmdShow);
						if(pHScrollBar)
							Detour_ShowWindow(pHScrollBar->m_hWnd, SW_HIDE);
					}
				}
			}
		}		
		return TRUE;
	}

	extern "C"
		int APIENTRY TP_ShowWindow(HWND hWnd, int nCmdShow)
	{
		BOOL bReturn  = Detour_ShowWindow(hWnd, nCmdShow);

		int ni = TP_GetAttcahWndIndex(hWnd, TP_ALL);

		if(ni >= 0)
		{
			if(nCmdShow == SW_HIDE || nCmdShow == SW_SHOW)
			{
				g_aAttachWnd[ni] -> bShowAttachWnd  =(nCmdShow == SW_SHOW);
				CWnd* pHScrollBar = g_aAttachWnd[ni] -> pHScroll;
				CWnd* pVScrollBar = g_aAttachWnd[ni] -> pVScroll;

				if(nCmdShow == SW_HIDE)
				{
					if(pHScrollBar) Detour_ShowWindow(pHScrollBar->m_hWnd, nCmdShow);
					if(pHScrollBar) Detour_ShowWindow(pVScrollBar->m_hWnd, nCmdShow);
				}			
				else
				{
					if(g_aAttachWnd[ni] ->bShowHScrollBar && g_aAttachWnd[ni] ->bForceHScrollBar  && g_aAttachWnd[ni] ->bShowVScrollBar && g_aAttachWnd[ni] ->bForceVScrollBar)
					{
						Detour_ShowWindow(pHScrollBar->m_hWnd, nCmdShow);
						Detour_ShowWindow(pVScrollBar->m_hWnd, nCmdShow);
					}
					else if(g_aAttachWnd[ni] ->bShowHScrollBar  && g_aAttachWnd[ni] ->bForceHScrollBar )
					{			
						Detour_ShowWindow(pHScrollBar->m_hWnd, nCmdShow);

						if(pVScrollBar)
							Detour_ShowWindow(pVScrollBar->m_hWnd, SW_HIDE);
					}
					else if(g_aAttachWnd[ni] ->bShowVScrollBar  && g_aAttachWnd[ni] ->bForceVScrollBar )
					{
						Detour_ShowWindow(pVScrollBar->m_hWnd, nCmdShow);
						if(pHScrollBar)
							Detour_ShowWindow(pHScrollBar->m_hWnd, SW_HIDE);
					}
				}
			}
		}
		EnumChildWindows(hWnd,(WNDENUMPROC)TP_EnumChildWndProc,nCmdShow);	
		return bReturn;	
	}

	extern "C"
		int APIENTRY TP_EnableScrollBar(HWND hWnd,UINT wSBflags,UINT nArrows)
	{
		if(wSBflags == SB_CTL)
		{
			int iIndex = TP_GetScrollBar(hWnd);
			if(iIndex >=0)			
				return g_pScrollBarCallBack.EnableScrollBar(CWnd::FromHandle(g_aAttachScrollBar[iIndex]->hAttachWnd),nArrows);
			else return Detour_EnableScrollBar(hWnd,wSBflags,nArrows);
		}
		else  return Detour_EnableScrollBar(hWnd,wSBflags,nArrows);	
	}

	extern "C"
		int APIENTRY TP_SetScrollInfo(HWND hWnd,int fnBar,LPCSCROLLINFO lpsi,BOOL fRedraw)
	{

		if(fnBar == SB_CTL)
		{
			int iIndex = TP_GetScrollBar(hWnd);
			if(iIndex >=0)			
				return g_pScrollBarCallBack.SetScrollInfo(CWnd::FromHandle(g_aAttachScrollBar[iIndex]->hAttachWnd),lpsi,fRedraw);
			else return Detour_SetScrollInfo(hWnd,fnBar,lpsi,fRedraw);
		}
		else
		{
			int   iReturn      = 0;
			CWnd *pScrollBar   = NULL;
			BOOL  bHideScroll  = FALSE;

			int ni = TP_GetAttcahWndIndex(hWnd,fnBar);

			if(ni < 0)		return Detour_SetScrollInfo(hWnd,fnBar,lpsi,FALSE);
			if(!(lpsi->fMask & (SIF_RANGE|SIF_PAGE)) && (lpsi->fMask & SIF_POS))
			{
				CWnd* pScrollBar = NULL;
				if(fnBar == SB_VERT)  pScrollBar = g_aAttachWnd[ni] ->pVScroll;
				else		          pScrollBar = g_aAttachWnd[ni] ->pHScroll;	

				if(lpsi->nPos != g_pScrollBarCallBack.GetScrollPos(pScrollBar))			
					g_pScrollBarCallBack .SetScrollPos(pScrollBar, lpsi->nPos,fRedraw);		
				return Detour_SetScrollInfo(hWnd,fnBar,lpsi,FALSE);
			}

			SCROLLINFO si;
			ZeroMemory(&si,sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			if(!Detour_GetScrollInfo(hWnd,fnBar,&si))
			{
				if(GetLastError() != ERROR_NO_SCROLLBARS)
					return Detour_SetScrollInfo(hWnd,fnBar,lpsi,fRedraw);
			}		
			TPScrollBarData *pScrollData = g_aAttachWnd[ni];
			if(pScrollData)
			{		
				if(fnBar == SB_VERT && pScrollData ->m_pVScrollInfo)      memcpy(pScrollData ->m_pVScrollInfo,lpsi,sizeof(SCROLLINFO));
				else if(fnBar == SB_HORZ && pScrollData ->m_pHScrollInfo) memcpy(pScrollData ->m_pHScrollInfo,lpsi,sizeof(SCROLLINFO));
			}

			iReturn = Detour_SetScrollInfo(hWnd,fnBar,lpsi,FALSE);	

			if(g_aAttachWnd[ni] ->bEdit)
			{
				CWnd *pWnd = CWnd::FromHandle(hWnd);
				CRect rtWnd,rtClient;
				pWnd ->GetWindowRect(&rtWnd);
				pWnd ->GetClientRect(&rtClient);
				pWnd ->ClientToScreen(&rtClient);
				rtClient . OffsetRect(-rtWnd.left,-rtWnd.top);
				rtWnd    . OffsetRect(-rtWnd.left,-rtWnd.top);

				if(fnBar == SB_VERT) 
				{
					if(abs((rtClient.left - rtWnd.left) - (rtWnd.right-rtClient.right))<5) bHideScroll = TRUE;
					pScrollBar = g_aAttachWnd[ni] ->pVScroll;
				}
				else
				{
					if(abs((rtClient.top - rtWnd.top) - (rtWnd.bottom - rtClient.bottom))<5) bHideScroll = TRUE;
					pScrollBar = g_aAttachWnd[ni] ->pHScroll;		
				}
			}
			else
			{	
				if(lpsi->fMask & SIF_RANGE)
				{
					if(lpsi->nMin >= lpsi->nMax)	bHideScroll = TRUE;
				}
				if(lpsi->fMask & SIF_PAGE)
				{
					if(!bHideScroll)			
					{
						if(lpsi->nPage < 0)			bHideScroll = TRUE;				
						if(lpsi->nPage >= (UINT)(lpsi->nMax -lpsi->nMin +1))
							bHideScroll = TRUE;
					}
				}
				if(fnBar == SB_VERT) 	pScrollBar = g_aAttachWnd[ni] ->pVScroll;
				else        			pScrollBar = g_aAttachWnd[ni] ->pHScroll;				
			}

			g_pScrollBarCallBack.SetScrollInfo(pScrollBar,lpsi,fRedraw);		


			if(bHideScroll)
			{
				if(g_aAttachWnd[ni] ->bShowVScrollBar && fnBar == SB_VERT)
					TP_CalCWindowPos(g_aAttachWnd[ni],g_aAttachWnd[ni] ->bShowHScrollBar ,FALSE);
				else if(g_aAttachWnd[ni] ->bShowHScrollBar &&fnBar == SB_HORZ)
					TP_CalCWindowPos(g_aAttachWnd[ni],FALSE,g_aAttachWnd[ni] ->bShowVScrollBar);
			}
			else
			{
				if(fnBar == SB_VERT)
					TP_CalCWindowPos(g_aAttachWnd[ni],g_aAttachWnd[ni] ->bShowHScrollBar,TRUE);
				if(fnBar == SB_HORZ)
					TP_CalCWindowPos(g_aAttachWnd[ni],TRUE,g_aAttachWnd[ni] ->bShowVScrollBar);
			}

			return iReturn;

		}

	}

	extern "C"
		BOOL APIENTRY TP_GetScrollInfo(HWND hWnd,int fnBar,LPSCROLLINFO lpsi)
	{

		if(fnBar == SB_CTL)
		{
			int iIndex = TP_GetScrollBar(hWnd);
			if(iIndex >=0)			
				return g_pScrollBarCallBack.GetScrollInfo(CWnd::FromHandle(g_aAttachScrollBar[iIndex]->hAttachWnd),lpsi,lpsi ->fMask);
			else return Detour_GetScrollInfo(hWnd,fnBar,lpsi);
		}

		CWnd* pScrollBar = NULL;	
		int ni = TP_GetAttcahWndIndex(hWnd,fnBar);
		if(ni<0)	return Detour_GetScrollInfo(hWnd,fnBar,lpsi);
		else
		{
			if(fnBar == SB_VERT) pScrollBar = g_aAttachWnd[ni] ->pVScroll;
			else		         pScrollBar = g_aAttachWnd[ni] ->pHScroll;	

			Detour_GetScrollInfo(hWnd,fnBar,lpsi);	
			return g_pScrollBarCallBack.GetScrollInfo(pScrollBar,lpsi,lpsi ->fMask);
		}
	}

	extern "C"
		BOOL APIENTRY TP_ShowScrollBar(HWND hWnd,int wBar,BOOL bShow)
	{	
		if(wBar == SB_CTL)
		{
			int iIndex = TP_GetScrollBar(hWnd);
			if(iIndex >=0)			
				g_pScrollBarCallBack.ShowScrollBar(CWnd::FromHandle(g_aAttachScrollBar[iIndex]->hAttachWnd),bShow);
			else return Detour_ShowScrollBar(hWnd,wBar,bShow);
		}	
		int ni = TP_GetAttcahWndIndex(hWnd,wBar);
		if(ni < 0 || ni>=g_aAttachWnd.GetSize())	
			return Detour_ShowScrollBar(hWnd,wBar,bShow);
		else
		{
			CWnd* pHScrollBar = NULL;
			CWnd* pVScrollBar = NULL;
			pHScrollBar = g_aAttachWnd[ni] -> pHScroll;
			pVScrollBar = g_aAttachWnd[ni] -> pVScroll;
			BOOL bReturn  = Detour_ShowScrollBar(hWnd,wBar,bShow);
			if(wBar == SB_VERT)
			{
				g_aAttachWnd[ni] ->bForceVScrollBar = bShow;
				TP_CalCWindowPos(g_aAttachWnd[ni],g_aAttachWnd[ni] ->bShowHScrollBar,bShow);
			}
			else if(wBar == SB_HORZ)
			{
				g_aAttachWnd[ni] ->bForceHScrollBar = bShow;
				TP_CalCWindowPos(g_aAttachWnd[ni],bShow,g_aAttachWnd[ni] ->bShowVScrollBar);
			}
			else if(wBar == SB_BOTH)
			{
				g_aAttachWnd[ni] ->bForceVScrollBar = bShow;
				g_aAttachWnd[ni] ->bForceHScrollBar = bShow;
				TP_CalCWindowPos(g_aAttachWnd[ni],bShow,bShow);
			}
			if(pVScrollBar && g_aAttachWnd[ni] ->bShowVScrollBar)
				pVScrollBar->Invalidate();
			if(pHScrollBar && g_aAttachWnd[ni] ->bShowHScrollBar)
				pHScrollBar->Invalidate();

			return bReturn;	
		}
	}


	extern "C"
		BOOL APIENTRY TP_SetScrollRange(HWND hWnd,int fnBar,int nMinPos,int nMaxPos,BOOL bRedraw)
	{
		if(fnBar == SB_CTL)
		{
			int iIndex = TP_GetScrollBar(hWnd);
			if(iIndex >=0)			
				g_pScrollBarCallBack .SetScrollRange(CWnd::FromHandle(g_aAttachScrollBar[iIndex]->hAttachWnd),nMinPos,nMaxPos,bRedraw);
			else return Detour_SetScrollRange(hWnd,fnBar,nMinPos,nMaxPos,bRedraw);		
		}	
		int ni = TP_GetAttcahWndIndex(hWnd,fnBar);
		if(ni <0)	 return Detour_SetScrollRange(hWnd,fnBar,nMinPos,nMaxPos,bRedraw);		
		else
		{
			CWnd* pScrollBar = NULL;
			if(fnBar == SB_VERT) pScrollBar = g_aAttachWnd[ni] ->pVScroll;
			else		         pScrollBar = g_aAttachWnd[ni] ->pHScroll;		

			// li song ming 2009-09-24 修改为先设置Range，然后再计算Scroll的位置。
			INT nRet = Detour_SetScrollRange(hWnd,fnBar,nMinPos,nMaxPos,bRedraw);			
			if(nMinPos >= nMaxPos)
			{
				if(g_aAttachWnd[ni] ->bShowVScrollBar && fnBar == SB_VERT)
					TP_CalCWindowPos(g_aAttachWnd[ni],g_aAttachWnd[ni] ->bShowHScrollBar ,FALSE);
				else if(g_aAttachWnd[ni] ->bShowHScrollBar &&fnBar == SB_HORZ)
					TP_CalCWindowPos(g_aAttachWnd[ni],FALSE,g_aAttachWnd[ni] ->bShowVScrollBar);
			}
			else
			{
				if(fnBar == SB_VERT && g_aAttachWnd[ni] ->bShowVScrollBar)
					TP_CalCWindowPos(g_aAttachWnd[ni],g_aAttachWnd[ni] ->bShowHScrollBar,TRUE);
				if(fnBar == SB_HORZ && g_aAttachWnd[ni] ->bShowHScrollBar)
					TP_CalCWindowPos(g_aAttachWnd[ni],TRUE,g_aAttachWnd[ni] ->bShowVScrollBar);
			}

			g_pScrollBarCallBack.SetScrollRange(pScrollBar,nMinPos,nMaxPos,bRedraw);
			return nRet;		
		}	
	}

	extern "C"
		int APIENTRY TP_SetScrollPos(HWND hWnd,int nBar,int nPos,BOOL bRedraw)
	{

		if(nBar == SB_CTL)
		{
			int iIndex = TP_GetScrollBar(hWnd);
			if(iIndex >=0)			
				return g_pScrollBarCallBack.SetScrollPos(CWnd::FromHandle(g_aAttachScrollBar[iIndex]->hAttachWnd), nPos, bRedraw);
			else return Detour_SetScrollPos(hWnd,nBar,nPos, bRedraw);	
		}
		int ni = TP_GetAttcahWndIndex(hWnd,nBar);
		CWnd* pScrollBar = NULL;
		if(ni <0)	return Detour_SetScrollPos(hWnd,nBar,nPos, bRedraw);	
		else
		{
			CWnd* pScrollBar = NULL;
			if(nBar == SB_VERT)  pScrollBar = g_aAttachWnd[ni] ->pVScroll;
			else		         pScrollBar = g_aAttachWnd[ni] ->pHScroll;	

			if(nPos != g_pScrollBarCallBack.GetScrollPos(pScrollBar))
			{
				g_pScrollBarCallBack .SetScrollPos(pScrollBar,nPos,bRedraw);
			}
			return Detour_SetScrollPos(hWnd,nBar,nPos, FALSE);	
		}	
	}


	extern "C"
		int APIENTRY TP_GetScrollPos(HWND hWnd,int nBar)
	{

		if(nBar == SB_CTL)
		{
			int iIndex = TP_GetScrollBar(hWnd);
			if(iIndex >=0)			
				return g_pScrollBarCallBack.GetScrollPos(CWnd::FromHandle(g_aAttachScrollBar[iIndex]->hAttachWnd));
			else return Detour_GetScrollPos( hWnd, nBar);	
		}
		int ni = TP_GetAttcahWndIndex(hWnd,nBar);	
		if(ni <0) return Detour_GetScrollPos( hWnd, nBar);		
		else
		{
			CWnd* pScrollBar = NULL;
			if(nBar == SB_VERT)  pScrollBar = g_aAttachWnd[ni] ->pVScroll;
			else		         pScrollBar = g_aAttachWnd[ni] ->pHScroll;	

			Detour_GetScrollPos( hWnd, nBar);
			return g_pScrollBarCallBack.GetScrollPos(pScrollBar);
		}		
	}

	extern "C"
		BOOL APIENTRY TP_MoveWindow(HWND hWnd,int X,int Y,int nWidth,int nHeight,BOOL bRepaint)
	{	
		BOOL bReturn = Detour_MoveWindow(hWnd,X,Y,nWidth,nHeight,bRepaint);
		int ni = TP_GetAttcahWndIndex(hWnd,0x7fffffff);
		if(ni < 0)	return bReturn;	
		{
			/*		CWnd *pWnd = CWnd::FromHandle(g_aAttachWnd[ni] ->hAttachWnd);
			CRect rtWnd,rtClient;
			pWnd ->GetWindowRect(&rtWnd);
			pWnd ->GetClientRect(&rtClient);
			pWnd ->ClientToScreen(&rtClient);
			rtClient . OffsetRect(-rtWnd.left,-rtWnd.top);
			rtWnd    . OffsetRect(-rtWnd.left,-rtWnd.top);

			if(abs((rtClient.left - rtWnd.left) - (rtWnd.right-rtClient.right))>10)   g_aAttachWnd[ni] ->bShowVScrollBar = TRUE;
			else    g_aAttachWnd[ni] ->bShowVScrollBar = FALSE;
			if(abs((rtClient.top - rtWnd.top) - (rtWnd.bottom - rtClient.bottom))>10) g_aAttachWnd[ni] ->bShowHScrollBar = TRUE;
			else    g_aAttachWnd[ni] ->bShowHScrollBar = FALSE;*/

			TP_CalCWindowPos(g_aAttachWnd[ni],g_aAttachWnd[ni] ->bShowHScrollBar,g_aAttachWnd[ni] ->bShowVScrollBar);
			return bReturn;
		}	
	}
	extern "C"
		BOOL APIENTRY TP_SetWindowPos    (HWND hWnd,HWND hWndInsertAfter,int X,int Y,int cx,int cy,UINT uFlags)
	{
		int ni = TP_GetAttcahWndIndex(hWnd,0x7fffffff);
		if(ni < 0)	return  Detour_SetWindowPos( hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);	
		{
			BOOL bReturn = Detour_SetWindowPos( hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags | SWP_NOREDRAW);
			if(uFlags & SWP_SHOWWINDOW) 	EnumChildWindows(hWnd,(WNDENUMPROC)TP_EnumChildWndProc,SW_SHOW);
			g_aAttachWnd[ni] -> bShowAttachWnd = ::IsWindowVisible(hWnd);
			TP_CalCWindowPos(g_aAttachWnd[ni],g_aAttachWnd[ni] ->bShowHScrollBar,g_aAttachWnd[ni] ->bShowVScrollBar);				
			return bReturn;
		}		
	}
	extern "C"
		HWND  APIENTRY TP_SetParent(HWND hWndChild,HWND hWndNewParent)
	{
		int ni = TP_GetAttcahWndIndex(hWndChild,TP_ALL);	
		if(ni <0)      	    return Detour_SetParent( hWndChild, hWndNewParent);	
		else
		{	
			HWND hWnd = Detour_SetParent( hWndChild, hWndNewParent);	
			g_aAttachWnd[ni] ->pVScroll ->SetParent(CWnd::FromHandle(hWndNewParent));
			g_aAttachWnd[ni] ->pHScroll ->SetParent(CWnd::FromHandle(hWndNewParent));	
			TP_CalCWindowPos(g_aAttachWnd[ni],g_aAttachWnd[ni] ->bShowHScrollBar,g_aAttachWnd[ni] ->bShowVScrollBar);
			return hWnd;
		}			
	}

	extern "C"
		DWORD WINAPI TP_GetSysColorEx(IN int nIndex)
	{
		return TP_GetSysColor(nIndex);	
	}

	extern int     (WINAPI * TP_ExMessageBox)(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);

	extern UINT  g_uMainThreadID;
	extern CWnd *g_pMainFrameWnd;

	struct TPPathFileExist
	{
		HANDLE        hPathFileExist;
		BOOL          bPathFileExist;
		CString       sPathFileExist;
		TPPathFileExist()
		{
			hPathFileExist     =  CreateEvent(NULL,FALSE,FALSE,NULL);
			bPathFileExist     = FALSE;
		}
		~TPPathFileExist()
		{
			::CloseHandle(hPathFileExist);
			hPathFileExist = NULL;
		}
	};

	UINT TP_PathFileExistProc(LPVOID pVoid)
	{
		TPPathFileExist *pstuData = (TPPathFileExist*)pVoid;
		pstuData->bPathFileExist = Detour_PathFileExists(pstuData->sPathFileExist);
		::SetEvent(pstuData->hPathFileExist);
		return 1;
	}
	BOOL  TP_PathFileExistsEx  (LPCTSTR pszPath)
	{
		TPPathFileExist stuData;
		stuData.bPathFileExist = FALSE;
		stuData.sPathFileExist = pszPath;
		::ResetEvent(stuData.hPathFileExist);
		CWinThread *pPathFileExist = AfxBeginThread((AFX_THREADPROC)TP_PathFileExistProc,(LPVOID)&stuData, 0, 0, CREATE_SUSPENDED);
		if(pPathFileExist == NULL)
		{
			return Detour_PathFileExists( pszPath);
		}
		else
		{
			HANDLE hThread = pPathFileExist->m_hThread;
			pPathFileExist->ResumeThread();
			if(::WaitForSingleObject(stuData.hPathFileExist,5000) == WAIT_TIMEOUT)
				TerminateThread(hThread,1);
			else 
				::WaitForSingleObject(hThread,1000);
			return stuData.bPathFileExist;
		}
		return FALSE;		
	}
	extern "C" BOOL  WINAPI TP_PathFileExists  (LPCTSTR pszPath)
	{
		if( Detour_PathFileExists( pszPath))
			return TRUE;
		if(lstrlen(pszPath) <1) return FALSE;
		if(pszPath[0] == '\\')
		{
			HANDLE  hFile;  
			hFile = CreateFile(pszPath
				,GENERIC_READ
				,FILE_SHARE_READ|FILE_SHARE_WRITE
				,NULL
				,OPEN_EXISTING
				,FILE_ATTRIBUTE_NORMAL
				,NULL);

			if(INVALID_HANDLE_VALUE == hFile)
				return FALSE;
			else
			{
				CloseHandle(hFile);
				return TRUE;
			}
		}
		else
		{
			WIN32_FIND_DATA lpFindFileData;
			HANDLE hFind = FindFirstFile( pszPath,&lpFindFileData);
			if( INVALID_HANDLE_VALUE == hFind) return FALSE;
			else {FindClose(hFind);return TRUE;}
		}
	}
	TPMessageBoxParam stuParam;
	extern "C"
		int WINAPI TP_MessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
	{		 
	/*	HMODULE hCallerModule = NULL;
		TCHAR szModuleName[MAX_PATH] = _T("");
		void *callerAddress  = _ReturnAddress();
		void *callerAddress1 = (void *) *((ULONG *)_AddressOfReturnAddress()-1);
		if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)hWnd, &hCallerModule))
		{
			GetModuleFileName(hCallerModule, szModuleName, ARRAYSIZE(szModuleName));
		}
		if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)callerAddress1, &hCallerModule))
		{
			GetModuleFileName(hCallerModule, szModuleName, ARRAYSIZE(szModuleName));
		}	*/
#ifdef TP_NS_NET
		if(TP_ExMessageBox	&& (uType & 0x80000000) == 0)
#else
		if(TP_ExMessageBox)
#endif		
		{	 
			if(g_pMainFrameWnd)
			{		
				CWinThread *pThread =  AfxGetThread();
				if(pThread && pThread ->m_nThreadID != g_uMainThreadID) 
				{
					stuParam.hWnd      = hWnd;
					stuParam.lpText    = lpText;
					stuParam.lpCaption = lpCaption;
					stuParam.uType     = uType;
					stuParam.uType    &= ~MB_POSTMESSAGE;
					if(uType & MB_POSTMESSAGE) return (int)g_pMainFrameWnd ->PostMessage(WM_MAINMESSAGEBOX,(WPARAM)&stuParam,NULL); 
					else                       return (int)g_pMainFrameWnd ->SendMessage(WM_MAINMESSAGEBOX,(WPARAM)&stuParam,NULL);
				}
			}
			uType &= ~MB_POSTMESSAGE;
			return TP_ExMessageBox(hWnd,lpText,lpCaption,uType);
		}
		else 
			return Detour_MessageBox(hWnd,lpText,lpCaption,uType);
	}
	//
	
	

	void  TP_UnReplaceScrollBar(CWnd *pWnd)
	{
		for(int i  = (int)g_aAttachScrollBar.GetSize() - 1; i>=0 ;i--)
		{
			if(g_aAttachScrollBar[i] ->hAttachWnd == pWnd ->m_hWnd) 
			{			
				delete g_aAttachScrollBar[i];
				g_aAttachScrollBar.RemoveAt(i);
			}
		}	
	}
	void  TP_ResetScrollBar(TPScrollBarData *pData)
	{
		if(pData->pAttachWnd == NULL) return;

		CRect rtWnd,rtClient;
		pData->pAttachWnd ->GetWindowRect(&rtWnd);
		pData->pAttachWnd ->GetClientRect(&rtClient);
		pData->pAttachWnd ->ClientToScreen(&rtClient);
		rtClient . OffsetRect(-rtWnd.left,-rtWnd.top);
		rtWnd    . OffsetRect(-rtWnd.left,-rtWnd.top);

		if(abs((rtClient.left - rtWnd.left) - (rtWnd.right-rtClient.right))>10)   pData->bShowVScrollBar = TRUE;
		else    pData ->bShowVScrollBar = FALSE;
		if(abs((rtClient.top - rtWnd.top) - (rtWnd.bottom - rtClient.bottom))>10) pData ->bShowHScrollBar = TRUE;
		else    pData ->bShowHScrollBar = FALSE;
		if(pData->pHScroll) pData->pHScroll ->ShowWindow(pData ->bShowHScrollBar?SW_SHOW:SW_HIDE);
		if(pData->pVScroll) pData->pVScroll ->ShowWindow(pData ->bShowVScrollBar?SW_SHOW:SW_HIDE);

	}
	void  TP_ReplaceScrollBar(CWnd *pWnd)
	{
		for(int i = 0;i<g_aAttachScrollBar.GetSize();i++)
		{
			if(g_aAttachScrollBar[i] ->hAttachWnd == pWnd ->m_hWnd) return;
		}
		TPScrollBarData *pAddData = new TPScrollBarData;
		pAddData ->hAttachWnd = pWnd ->m_hWnd;
		g_aAttachScrollBar.Add(pAddData);	

	}
	TPScrollBarData *  TP_GetReplaceData(CWnd *pWnd)
	{
		if(pWnd == NULL || !::IsWindow(pWnd ->m_hWnd))	return NULL;
		for(int l=0;l<g_aAttachWnd.GetSize();l++)
		{
			if(g_aAttachWnd[l] ->hAttachWnd == pWnd ->m_hWnd) return g_aAttachWnd[l]; 
		}
		return NULL;
	}
	TPScrollBarData *  TP_ReplaceWnd(CWnd *pWnd)
	{
		if(pWnd == NULL || !::IsWindow(pWnd ->m_hWnd))	return NULL;
		for(int l=0;l<g_aAttachWnd.GetSize();l++)
		{
			if(g_aAttachWnd[l] ->hAttachWnd == pWnd ->m_hWnd) return g_aAttachWnd[l]; //已经绑定
		}

		BOOL bReturn =CWnd::ModifyStyle(pWnd ->m_hWnd,0,WS_CLIPSIBLINGS,0);
		CWnd* pParent = pWnd->GetParent();
		if(pParent == NULL)	return NULL;

		CWnd* pVScrollBar = NULL;
		CWnd* pHScrollBar = NULL;

		TPScrollBarData *pAddData = new TPScrollBarData();

		TCHAR cName[40];ZeroMemory(cName,sizeof(TCHAR)*40);
		::GetClassName(pWnd->m_hWnd,cName,40);
		if(lstrcmpi(cName,WC_EDIT) ==0 || lstrcmpi(cName,RICHEDIT_CLASS) == 0)
			pAddData ->bEdit = TRUE;
		if(!pAddData ->bEdit)	Detour_ShowScrollBar(pWnd ->m_hWnd,SB_BOTH,FALSE);	
		

		pAddData ->hAttachWnd     = pWnd ->m_hWnd;	
		pAddData ->pAttachWnd     = pWnd; 

		SCROLLINFO si;
		ZeroMemory(&si,sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		Detour_GetScrollInfo(pWnd ->m_hWnd,SB_VERT,&si);
	 

		pVScrollBar = g_pScrollBarCallBack.CreateScrollBar(WS_CHILD|SBS_VERT,pWnd);	
		pVScrollBar->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE);	
		pVScrollBar->ShowWindow(SW_HIDE);
		g_pScrollBarCallBack.SetScrollInfo(pVScrollBar,&si,TRUE);
		pAddData ->pVScroll = pVScrollBar;

		ZeroMemory(&si,sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		Detour_GetScrollInfo(pWnd ->m_hWnd,SB_HORZ,&si);

		pHScrollBar = g_pScrollBarCallBack.CreateScrollBar(WS_CHILD|SBS_HORZ,pWnd);	
		pHScrollBar->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE);	
		pHScrollBar->ShowWindow(SW_HIDE);
		g_pScrollBarCallBack.SetScrollInfo(pHScrollBar,&si,TRUE);
		pAddData ->pHScroll  = pHScrollBar;

		g_aAttachWnd .Add(pAddData);
		return pAddData;
	}
	void  TP_UnReplaceWnd(CWnd *pWnd)
	{
		CWnd            *pHScrollBar = NULL;
		CWnd            *pVScrollBar = NULL;
		TPScrollBarData *pAddData    = NULL;	

		for(int l = (int)g_aAttachWnd.GetSize() -1;l>=0;l--)
		{
			if(g_aAttachWnd[l] && g_aAttachWnd[l] ->pAttachWnd == pWnd)
			{
				pAddData = g_aAttachWnd[l];
				if(pAddData ->pHScroll) g_pScrollBarCallBack.DeleteScrollBar(pAddData ->pHScroll);			pAddData ->pHScroll = NULL;
				if(pAddData ->pVScroll) g_pScrollBarCallBack.DeleteScrollBar(pAddData ->pVScroll);			pAddData ->pVScroll = NULL;
				pAddData ->hAttachWnd = NULL;
				delete pAddData;
				g_aAttachWnd.RemoveAt(l);
				break;
			}		
		}
		return ;
	}
	HHOOK g_hMouseHook = NULL;
#define WC_BTNTOOLBAR _L("BtnToolBar")
	LRESULT CALLBACK TP_MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
	{	
		if( HC_ACTION == nCode  && wParam == WM_MOUSEWHEEL)
		{
			MOUSEHOOKSTRUCTEX *pData =(MOUSEHOOKSTRUCTEX *)lParam;
			CPoint pt = pData ->pt;
			HWND hPointWnd = ::WindowFromPoint(pt);
			if(hPointWnd)
			{		
				TCHAR cClassName[MAX_PATH];				
				GetClassName(hPointWnd,cClassName,MAX_PATH);
				if(lstrcmpi(cClassName,WC_COMBOTREEBOX) == 0 || lstrcmpi(cClassName,WC_COMBOBOX) == 0)
				{
					return CallNextHookEx(g_hMouseHook,nCode, wParam, lParam);
				}
				if(lstrcmpi(cClassName,WC_SLIDERBAR) == 0)
				{
					SendMessage(hPointWnd,WM_MOUSEWHEEL,pData ->mouseData,MAKELONG(pt.x,pt.y));
					return 1;
				}							
				GetWindowText(hPointWnd,cClassName,MAX_PATH);
				if(lstrcmpi(cClassName,WC_BTNTOOLBAR) == 0)
				{	
					SendMessage(hPointWnd,WM_MOUSEWHEEL,pData ->mouseData,MAKELONG(pt.x,pt.y));
					return 1;
				} 
				else if(lstrcmpi(cClassName, _L("TimeLine")) == 0)
				{
					SendMessage(hPointWnd,WM_MOUSEWHEEL,pData ->mouseData,MAKELONG(pt.x,pt.y));
					return 1;
				}		
				hPointWnd = GetParent(hPointWnd);
				if(hPointWnd)
				{			
					GetWindowText(hPointWnd,cClassName,MAX_PATH);
					if(lstrcmpi(cClassName,WC_BTNTOOLBAR) == 0)
					{
						SendMessage(hPointWnd,WM_MOUSEWHEEL,pData ->mouseData,MAKELONG(pt.x,pt.y));
						return 1;
					}
				}	
				hPointWnd = GetParent(hPointWnd);
				if(hPointWnd)
				{			
					GetWindowText(hPointWnd,cClassName,MAX_PATH);
					if(lstrcmpi(cClassName,WC_BTNTOOLBAR) == 0)
					{
						SendMessage(hPointWnd,WM_MOUSEWHEEL,pData ->mouseData,MAKELONG(pt.x,pt.y));
						return 1;
					}
				}			
			}
		}
		return CallNextHookEx(g_hMouseHook,nCode, wParam, lParam);
	}	
	BOOL  TP_InitScrollHook(TPScrollBarCallBack *pCallBack)
	{
		DWORD dwVersion = GetVersion();

		g_pScrollBarCallBack.SetScrollInfo   = pCallBack ->SetScrollInfo;
		g_pScrollBarCallBack.GetScrollInfo   = pCallBack ->GetScrollInfo;
		g_pScrollBarCallBack.GetScrollPos    = pCallBack ->GetScrollPos;
		g_pScrollBarCallBack.SetScrollPos    = pCallBack ->SetScrollPos;
		g_pScrollBarCallBack.GetScrollRange  = pCallBack ->GetScrollRange;
		g_pScrollBarCallBack.SetScrollRange  = pCallBack ->SetScrollRange;


		g_pScrollBarCallBack.ShowScrollBar    = pCallBack ->ShowScrollBar;
		g_pScrollBarCallBack.EnableScrollBar  = pCallBack ->EnableScrollBar;
		g_pScrollBarCallBack.GetScrollLimit   = pCallBack ->GetScrollLimit;

		g_pScrollBarCallBack.CreateScrollBar = pCallBack ->CreateScrollBar;
		g_pScrollBarCallBack.DeleteScrollBar = pCallBack ->DeleteScrollBar;


		g_hMouseHook = SetWindowsHookEx(WH_MOUSE,TP_MouseProc,NULL,AfxGetApp()->m_nThreadID);

		if(dwVersion < 0x80000000)
		{

			DetourFunctionWithTrampoline((PBYTE)Detour_EnableScrollBar, (PBYTE)TP_EnableScrollBar);
			DetourFunctionWithTrampoline((PBYTE)Detour_GetScrollInfo,   (PBYTE)TP_GetScrollInfo);
			DetourFunctionWithTrampoline((PBYTE)Detour_GetScrollPos,    (PBYTE)TP_GetScrollPos);
			DetourFunctionWithTrampoline((PBYTE)Detour_SetScrollInfo,   (PBYTE)TP_SetScrollInfo);
			DetourFunctionWithTrampoline((PBYTE)Detour_SetScrollPos,    (PBYTE)TP_SetScrollPos);
			DetourFunctionWithTrampoline((PBYTE)Detour_SetScrollRange,  (PBYTE)TP_SetScrollRange);
			DetourFunctionWithTrampoline((PBYTE)Detour_ShowScrollBar,   (PBYTE)TP_ShowScrollBar);


			DetourFunctionWithTrampoline((PBYTE)Detour_BringWindowToTop,   (PBYTE)TP_BringWindowToTop);
			DetourFunctionWithTrampoline((PBYTE)Detour_SetParent,          (PBYTE)TP_SetParent);
			DetourFunctionWithTrampoline((PBYTE)Detour_MoveWindow,         (PBYTE)TP_MoveWindow);
			DetourFunctionWithTrampoline((PBYTE)Detour_ShowWindow,         (PBYTE)TP_ShowWindow);
			DetourFunctionWithTrampoline((PBYTE)Detour_SetWindowPos,       (PBYTE)TP_SetWindowPos);	

			DetourFunctionWithTrampoline((PBYTE)Detour_MessageBox,         (PBYTE)TP_MessageBox);	
			DetourFunctionWithTrampoline((PBYTE)Detour_PathFileExists,     (PBYTE)TP_PathFileExists);
			DetourFunctionWithTrampoline((PBYTE)Detour_TrackPopupMenuEx,   (PBYTE)TP_TrackPopupMenuEx);			


			//TP_InitScrollHookEx();
			return TRUE;
		}
		else
		{
			return FALSE;
		}	
	}

	BOOL  TP_ReleaseScrollHook(void)
	{
		DWORD dwVersion = GetVersion();


		CWnd  wndTemp;
		for(int l = (int)g_aAttachWnd.GetSize()-1;l>=0;l--)
		{
			wndTemp.m_hWnd = g_aAttachWnd[l] ->hAttachWnd;
			TP_UnReplaceWnd(&wndTemp);		
		}	
		for(int l = (int)g_aAttachScrollBar.GetSize()-1;l>=0;l--)
		{
			wndTemp.m_hWnd = g_aAttachScrollBar[l] ->hAttachWnd;
			TP_UnReplaceScrollBar(&wndTemp);		
		}	

		if(g_hMouseHook) UnhookWindowsHookEx(g_hMouseHook);
		g_hMouseHook = NULL;


		if(dwVersion < 0x80000000)
		{
			DetourRemove((PBYTE)Detour_EnableScrollBar, (PBYTE)TP_EnableScrollBar);
			DetourRemove((PBYTE)Detour_GetScrollInfo,   (PBYTE)TP_GetScrollInfo);
			DetourRemove((PBYTE)Detour_GetScrollPos,    (PBYTE)TP_GetScrollPos);
			DetourRemove((PBYTE)Detour_SetScrollInfo,   (PBYTE)TP_SetScrollInfo);
			DetourRemove((PBYTE)Detour_SetScrollPos,    (PBYTE)TP_SetScrollPos);
			DetourRemove((PBYTE)Detour_SetScrollRange,  (PBYTE)TP_SetScrollRange);
			DetourRemove((PBYTE)Detour_ShowScrollBar,   (PBYTE)TP_ShowScrollBar);


			DetourRemove((PBYTE)Detour_BringWindowToTop,   (PBYTE)TP_BringWindowToTop);
			DetourRemove((PBYTE)Detour_SetParent,          (PBYTE)TP_SetParent);
			DetourRemove((PBYTE)Detour_MoveWindow,         (PBYTE)TP_MoveWindow);
			DetourRemove((PBYTE)Detour_ShowWindow,         (PBYTE)TP_ShowWindow);
			DetourRemove((PBYTE)Detour_SetWindowPos,       (PBYTE)TP_SetWindowPos);	

			DetourRemove((PBYTE)Detour_MessageBox,         (PBYTE)TP_MessageBox);		
			DetourRemove((PBYTE)Detour_PathFileExists,     (PBYTE)TP_PathFileExists);			
			DetourRemove((PBYTE)Detour_TrackPopupMenuEx,   (PBYTE)TP_TrackPopupMenuEx);	
 
 
			//TP_ReleaseScrollHook();
			return TRUE;

		}
		else
		{
			return FALSE;
		}
	}	
	
};
void TP_PumpMemory()
{
}
