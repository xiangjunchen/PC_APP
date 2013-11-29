// TPCommonCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



HINSTANCE        g_hThisInstance  = NULL;
CTPRoundCorner   g_stuRoundCorner;
CTPRoundCorner  *g_pRoundCorner    = &g_stuRoundCorner;
TP_BORDER_STYLE  g_eBorderStyle    = TP_BORDER_20040702; 

CString          g_sRegisterClass  = _L("");


static AFX_EXTENSION_MODULE TPCommonCtrlDLL = { NULL, NULL };



BOOL  SetScrollInfo(CWnd *pScrollBarWnd,LPCSCROLLINFO lpScrollInfo, BOOL bRedraw)
{		
	CTPScrollBar *pScrollBar = ( CTPScrollBar *)pScrollBarWnd;
	return  pScrollBar->SetScrollInfo((LPSCROLLINFO)lpScrollInfo,bRedraw);			
}
BOOL  GetScrollInfo(CWnd *pScrollBarWnd,LPSCROLLINFO lpScrollInfo, UINT nMask)
{
	CTPScrollBar *pScrollBar = ( CTPScrollBar *)pScrollBarWnd;
	return  pScrollBar->GetScrollInfo(lpScrollInfo,nMask);		
}
void  GetScrollRange(CWnd *pScrollBarWnd,LPINT lpMinPos, LPINT lpMaxPos)
{
	CTPScrollBar *pScrollBar = ( CTPScrollBar *)pScrollBarWnd;
	return  pScrollBar->GetScrollRange(lpMinPos, lpMaxPos);		

}
void  SetScrollRange(CWnd *pScrollBarWnd,int nMinPos, int nMaxPos, BOOL bRedraw)
{
	CTPScrollBar *pScrollBar = ( CTPScrollBar *)pScrollBarWnd;
	return  pScrollBar->SetScrollRange(nMinPos, nMaxPos, bRedraw);		
}

int   GetScrollPos(CWnd *pScrollBarWnd)
{
	CTPScrollBar *pScrollBar = ( CTPScrollBar *)pScrollBarWnd;
	return  (int)pScrollBar->GetScrollPos();		
}
int   SetScrollPos(CWnd *pScrollBarWnd,int nPos, BOOL bRedraw)
{
	CTPScrollBar *pScrollBar = ( CTPScrollBar *)pScrollBarWnd;
	return (int)pScrollBar ->SetScrollPos(nPos,bRedraw);			
}

void  ShowScrollBar(CWnd *pScrollBarWnd,BOOL bShow)
{
	CTPScrollBar *pScrollBar = ( CTPScrollBar *)pScrollBarWnd;
	return pScrollBar ->ShowScrollBar(bShow);			
}
BOOL  EnableScrollBar(CWnd *pScrollBarWnd,UINT nArrowFlags)
{
	CTPScrollBar *pScrollBar = ( CTPScrollBar *)pScrollBarWnd;
	return pScrollBar ->EnableScrollBar(nArrowFlags);			
}
int   GetScrollLimit(CWnd *pScrollBarWnd)
{
	CTPScrollBar *pScrollBar = ( CTPScrollBar *)pScrollBarWnd;
	return pScrollBar ->GetScrollLimit();			
}


CWnd *CreateScrollBar(UINT uStyle,CWnd *pParent)
{		
	CTPScrollBar *pScrollBar = new CTPScrollBar();
	pScrollBar ->Create(uStyle,CRect(0,0,10,10),pParent->GetParent(),0);
	pScrollBar ->m_pMsgWnd    = pParent;
	pScrollBar ->m_bAttachBar = TRUE;
	return pScrollBar;

}
void  DeleteScrollBar(CWnd *pScrollBar)
{	

	CTPScrollBar *pScrollBarDel = ( CTPScrollBar *)pScrollBar;
	pScrollBarDel ->m_pMsgWnd = NULL; //by zhh
	if(::IsWindow(pScrollBarDel ->m_hWnd)) pScrollBarDel ->DestroyWindow();
	delete pScrollBarDel;

}

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// 如果使用 lpReserved，请将此移除
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TPCommonCtrl.DLL Initializing!\n");

		// 扩展 DLL 一次性初始化
		if (!AfxInitExtensionModule(TPCommonCtrlDLL, hInstance))
			return 0;

		// 将此 DLL 插入到资源链中
		// 注意: 如果此扩展 DLL 由
		//  MFC 规则 DLL(如 ActiveX 控件)隐式链接到
		//  而不是由 MFC 应用程序链接到，则需要
		//  将此行从 DllMain 中移除并将其放置在一个
		//  从此扩展 DLL 导出的单独的函数中。使用此扩展 DLL 的
		//  规则 DLL 然后应显式
		//  调用该函数以初始化此扩展 DLL。否则，
		//  CDynLinkLibrary 对象不会附加到
		//  规则 DLL 的资源链，并将导致严重的
		//  问题。

		g_hThisInstance   = hInstance;	

		new CDynLinkLibrary(TPCommonCtrlDLL);
	}

	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TPCommonCtrl.DLL 正在终止！\n");

		// 在调用析构函数之前终止该库

		g_hThisInstance = NULL;
		AfxTermExtensionModule(TPCommonCtrlDLL);
	}
	return 1;   // 确定
}

LPCTSTR TP_SBLoadString(LPCTSTR strFind)
{
	return strFind;
}

void TP_SetSysKey()
{
	BOOL bStickyON = FALSE,bFilterON = FALSE,bToggleON = FALSE;

	STICKYKEYS stkKeys={sizeof(STICKYKEYS), 0UL};
	if(::SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &stkKeys, 0)){
		if(bStickyON){
			stkKeys.dwFlags |= SKF_HOTKEYACTIVE;
		}else{
			stkKeys.dwFlags &= ~SKF_HOTKEYACTIVE;
		}
		::SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &stkKeys, SPIF_SENDCHANGE);
	}

	// Filtery key's short cut
	FILTERKEYS fltKeys={sizeof(FILTERKEYS), 0UL};
	if(::SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &fltKeys, 0)){
		if(bFilterON){
			fltKeys.dwFlags |= FKF_HOTKEYACTIVE;
		}else{
			fltKeys.dwFlags &= ~FKF_HOTKEYACTIVE;
		}
		::SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fltKeys, SPIF_SENDCHANGE);
	}

	// Toggle key's short cut
	TOGGLEKEYS tglKeys={sizeof(TOGGLEKEYS), 0UL};
	if(::SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tglKeys, 0)){
		if(bToggleON){
			tglKeys.dwFlags |= TKF_HOTKEYACTIVE;
		}else{
			tglKeys.dwFlags &= ~TKF_HOTKEYACTIVE;
		}
		::SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tglKeys, SPIF_SENDCHANGE);
	}
}
namespace _tp_commonctrl
{
	BOOL g_bInitControl = FALSE;   

	extern BYTE *g_pbufTemp;
	extern int   g_ibufTemp;

	void TP_InitCommonCtrl()
	{
		if(g_bInitControl) return ;
		g_bInitControl = TRUE;

		TP_InitSysGdi();  //一定要在前面


		CTPWnd::RegisterClass(TRUE);
		CTPSliderCtrl::RegisterClass(TRUE);
		CTPSliderBar::RegisterClass();
		CTPComboTreeBox::RegisterClass(TRUE);
		CTPDirTreeCtrl::RegisterClass(TRUE) ;
		CTPDirTreeCtrl::LoadSysImageList(TRUE);
		//CTPSysLink::RegisterClass(TRUE);
		CTPDirListCtrl::RegisterClass(TRUE);
		//CTPComboDirTreeBox::RegisterClass(TRUE);
		//CTPCheckComboBox::RegisterClass(TRUE);

		TPScrollBarCallBack stuCallBack;	

		stuCallBack.SetScrollInfo    = SetScrollInfo;
		stuCallBack.GetScrollInfo    = GetScrollInfo;
		stuCallBack.GetScrollPos     = GetScrollPos;
		stuCallBack.SetScrollPos     = SetScrollPos;
		stuCallBack.GetScrollRange   = GetScrollRange;
		stuCallBack.SetScrollRange   = SetScrollRange;

		stuCallBack.ShowScrollBar    = ShowScrollBar;
		stuCallBack.EnableScrollBar  = EnableScrollBar;
		stuCallBack.GetScrollLimit   = GetScrollLimit;

		stuCallBack.CreateScrollBar  = CreateScrollBar;
		stuCallBack.DeleteScrollBar  = DeleteScrollBar;


		TP_InitScrollHook(&stuCallBack);
		//register ctrl
		TP_InitCommonRes();
		CTPButton::LoadDefRes(TRUE);
		g_pRoundCorner->LoadRes();

		::SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, SPIF_SENDCHANGE);
		ANIMATIONINFO stuAni;
		stuAni.iMinAnimate = FALSE;
		stuAni.cbSize      = sizeof(ANIMATIONINFO);
		::SystemParametersInfo(SPI_SETANIMATION,0,&stuAni,0);///XN00027194
		g_sRegisterClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,g_hArrowCursor,NULL,NULL);

		///////http://www.wangchao.net.cn/bbsdetail_54733.html
		TP_InitMenuHook(TRUE);


		TP_SetSysKey();
		long lLen1(0),lLen2(0),lLen3(0),lLen4(0);
		lLen1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
		lLen2 = GetSystemMetrics(SM_YVIRTUALSCREEN);
		lLen3 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		lLen4 = GetSystemMetrics(SM_CYVIRTUALSCREEN);

		CTPBaseFrame::m_rtScreen = CRect(lLen1,lLen2,lLen1+lLen3,lLen2+lLen4);
		//CTPBaseFrame::m_rtScreen = CRect(0,0,GetSystemMetrics(SM_CMONITORS)*GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
		//TCHAR cDic[4];
		//::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,cDic,4 );	
		//g_cDim = cDic[0];	

		if(CTPBaseCtrl::m_stuSBDictionary.SBLoadString == NULL)
		{
			//CTPBaseCtrl::m_stuSBDictionary.SBLoadString = SBLoadString;
			CTPBaseCtrl::m_stuSBDictionary.SBLoadString = TP_SBLoadString;
		}

	}
	void TP_ReleaseCommonCtrl()
	{
		if(!g_bInitControl) return ;
		g_bInitControl = FALSE;

		g_pRoundCorner->UnloadRes();
		CTPButton::LoadDefRes(FALSE);
		TP_ReleaseSysGdi();
		TP_ReleaseCommonRes();

		TP_ReleaseScrollHook();
		TP_InitMenuHook(FALSE);	

		CTPBaseFrame::m_aBaseFrame.RemoveAll();

		if(g_pbufTemp) delete [] g_pbufTemp;
		g_pbufTemp = NULL;

	}

//	void TP_InitCommonCtrl()
//	{
//		if(g_bInitControl) return ;
//		g_bInitControl = TRUE;
//#ifdef TP_NS_EDIT
//		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//#endif
//
//		DEVMODE stuMode; ZeroMemory(&stuMode,sizeof(stuMode));
//		stuMode.dmSize = sizeof(stuMode);
//		stuMode.dmBitsPerPel = 32;	
//		stuMode.dmFields	 = DM_BITSPERPEL;
//		LONG lChange =ChangeDisplaySettings (&stuMode,CDS_UPDATEREGISTRY);
//
//		TP_InitSysGdi();  //一定要在前面
//
//
//		CTPWnd::RegisterClass(TRUE);
//		CTPSliderCtrl::RegisterClass(TRUE);
//		CTPSliderBar::RegisterClass();
//		//CTPComboTreeBox::RegisterClass(TRUE);
//		//CTPDirTreeCtrl::RegisterClass(TRUE) ;
//		//CTPDirTreeCtrl::LoadSysImageList(TRUE);
//		//CTPSysLink::RegisterClass(TRUE);
//		//CTPDirListCtrl::RegisterClass(TRUE);
//		//CTPComboDirTreeBox::RegisterClass(TRUE);
//		//CTPCheckComboBox::RegisterClass(TRUE);
//
//// 		TPScrollBarCallBack stuCallBack;	
//// 
//// 		stuCallBack.SetScrollInfo    = SetScrollInfo;
//// 		stuCallBack.GetScrollInfo    = GetScrollInfo;
//// 		stuCallBack.GetScrollPos     = GetScrollPos;
//// 		stuCallBack.SetScrollPos     = SetScrollPos;
//// 		stuCallBack.GetScrollRange   = GetScrollRange;
//// 		stuCallBack.SetScrollRange   = SetScrollRange;
//// 
//// 		stuCallBack.ShowScrollBar    = ShowScrollBar;
//// 		stuCallBack.EnableScrollBar  = EnableScrollBar;
//		//stuCallBack.GetScrollLimit   = GetScrollLimit;
//
//		//stuCallBack.CreateScrollBar  = CreateScrollBar;
//		//stuCallBack.DeleteScrollBar  = DeleteScrollBar;
//
//
//	//	TP_InitScrollHook(&stuCallBack);
//		TP_InitCommonRes();
//
//		CTPButton::LoadDefRes(TRUE);
//		g_pRoundCorner->LoadRes();
//
//
//		::SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, SPIF_SENDCHANGE);
//		ANIMATIONINFO stuAni;
//		stuAni.iMinAnimate = FALSE;
//		stuAni.cbSize      = sizeof(ANIMATIONINFO);
//		::SystemParametersInfo(SPI_SETANIMATION,0,&stuAni,0);///XN00027194
//		g_sRegisterClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,g_hArrowCursor,NULL,NULL);
//
//		///////http://www.wangchao.net.cn/bbsdetail_54733.html
//
//	//	TP_InitMenuHook(TRUE);
//
//
//
//		TP_SetSysKey();
//
//		long lLen1(0),lLen2(0),lLen3(0),lLen4(0);
//		lLen1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
//		lLen2 = GetSystemMetrics(SM_YVIRTUALSCREEN);
//		lLen3 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
//		lLen4 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
//
//		CTPBaseFrame::m_rtScreen = CRect(lLen1,lLen2,lLen1+lLen3,lLen2+lLen4);
//		//CTPBaseFrame::m_rtScreen = CRect(0,0,GetSystemMetrics(SM_CMONITORS)*GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
//
//		ASSERT(TP_SYSMENU_HELP == ID_SYSMENU_HELP);
//
//// 		if(CTPBaseCtrl::m_stuSBDictionary.SBLoadString == NULL)
//// 			CTPBaseCtrl::m_stuSBDictionary.SBLoadString = TP_SBLoadString;
//	}
//
//
//
//
//
//	void TP_ReleaseCommonCtrl()
//	{	
//		if(!g_bInitControl) return;
//		g_bInitControl = FALSE;
//		g_sRegisterClass.Empty();
//
//
//		g_pRoundCorner ->UnloadRes();
//		CTPButton::LoadDefRes(FALSE);
//		g_pRoundCorner = NULL;
//
//		TP_ReleaseSysGdi();
//		TP_ReleaseCommonRes();
//
//		CTPSliderBar::RegisterClass(FALSE);
//// 		CTPComboTreeBox::RegisterClass(FALSE);
//// 		CTPDirTreeCtrl::RegisterClass(FALSE) ;
//// 		CTPDirTreeCtrl::LoadSysImageList(FALSE);
//// 		CTPSysLink::RegisterClass(FALSE);
//// 		CTPDirListCtrl::RegisterClass(FALSE);
//		CTPWnd::RegisterClass(FALSE);
//// 		CTPComboDirTreeBox::RegisterClass(FALSE);
//// 		CTPCheckComboBox::RegisterClass(FALSE);
//
//// 		TP_ReleaseScrollHook();
//// 		TP_InitMenuHook(FALSE);	
//
//
//		CTPBaseFrame::m_aBaseFrame.RemoveAll();
//
//		if(g_pbufTemp) delete [] g_pbufTemp;
//		g_pbufTemp = NULL;
//		g_ibufTemp = 0;
//	}
}