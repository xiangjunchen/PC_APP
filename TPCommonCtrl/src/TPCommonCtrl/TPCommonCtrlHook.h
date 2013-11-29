#pragma once

#include "tlhelp32.h"
/*#include "Imagehlp.h"
#pragma comment (lib,"Imagehlp.lib")*/

typedef struct _tag_APIHOOK32_ENTRY
{
	CString  sAPIName;
	CString  sCallModuleName;
	PROC     pfnOriginApiAddress;
	PROC     pfnDummyFunAddress;
	HMODULE  hModule;
	_tag_APIHOOK32_ENTRY()
	{
		sAPIName         = _L("");
		sCallModuleName  = _L("");
		pfnOriginApiAddress = NULL;
		pfnDummyFunAddress  = NULL;
		hModule             = NULL;
	}
	void CopyTo(_tag_APIHOOK32_ENTRY &other)
	{
		other.sAPIName             = sAPIName;
		other.sCallModuleName      = sCallModuleName;
		other.pfnOriginApiAddress  = pfnOriginApiAddress;
		other.pfnDummyFunAddress   = pfnDummyFunAddress;
		other.hModule              = hModule;
	}
}APIHOOK32_ENTERY;
typedef CArray<APIHOOK32_ENTERY,APIHOOK32_ENTERY&> APIHOOK32_ENTERYArray;

AFX_EXT_API BOOL  TP_SetWindowsAPIHook(APIHOOK32_ENTERY *phk);
AFX_EXT_API BOOL  TP_UnSetWindowsAPIHook(APIHOOK32_ENTERY *phk);


typedef struct _tagScrollBarData
{
	HWND            hAttachWnd;
	CWnd           *pAttachWnd;
	CWnd           *pVScroll;
	CWnd           *pHScroll;
	BOOL            bShowAttachWnd;
	BOOL            bShowVScrollBar;
	BOOL            bShowHScrollBar;	
	BOOL            bForceVScrollBar;
	BOOL            bForceHScrollBar;
	BOOL            bEdit;
	int             iScrollWidth;
	int             iScrollHeight;
	SCROLLINFO     *m_pVScrollInfo; 
	SCROLLINFO     *m_pHScrollInfo;
	_tagScrollBarData()
	{
		pAttachWnd       = NULL;
		hAttachWnd       = NULL;
		pVScroll         = NULL;
		pHScroll         = NULL;
		bShowAttachWnd   = TRUE;
		bShowVScrollBar  = FALSE;
		bShowHScrollBar  = FALSE;
		bForceVScrollBar = TRUE;
		bForceHScrollBar = TRUE;
		bEdit            = FALSE;
		iScrollWidth     = ::GetSystemMetrics(SM_CXVSCROLL);
		iScrollHeight    = ::GetSystemMetrics(SM_CYHSCROLL);
		m_pVScrollInfo   = NULL; 
		m_pHScrollInfo   = NULL;
	}
	~_tagScrollBarData()
	{
		m_pVScrollInfo   = NULL; 
		m_pHScrollInfo   = NULL;
		pVScroll         = NULL;
		pHScroll         = NULL;
	}
	BOOL GetShow(BOOL bVert)
	{
		if(!bVert) return bShowHScrollBar & bForceHScrollBar;
		else       return bShowVScrollBar & bForceVScrollBar;
	}
}TPScrollBarData;

typedef CArray<TPScrollBarData *,TPScrollBarData *&> TPScrollBarDataArray; 
typedef struct _tagScrollBarCallBack
{
	BOOL  (*SetScrollInfo)(CWnd *pScrollBar,LPCSCROLLINFO lpScrollInfo, BOOL bRedraw);
	BOOL  (*GetScrollInfo)(CWnd *pScrollBar,LPSCROLLINFO  lpScrollInfo, UINT nMask);
	int   (*GetScrollPos)(CWnd *pScrollBar);
	int   (*SetScrollPos)(CWnd *pScrollBar,int nPos, BOOL bRedraw);
	void  (*GetScrollRange)(CWnd *pScrollBar,LPINT lpMinPos, LPINT lpMaxPos);
	void  (*SetScrollRange)(CWnd *pScrollBar,int nMinPos, int nMaxPos, BOOL bRedraw);

	void  (*ShowScrollBar)(CWnd *pScrollBar,BOOL bShow);
	BOOL  (*EnableScrollBar)(CWnd *pScrollBar,UINT nArrowFlags);
	int   (*GetScrollLimit)(CWnd *pScrollBar);

	CWnd *(*CreateScrollBar)(UINT uStyle,CWnd *pParent);
	void  (*DeleteScrollBar)(CWnd *pScrollBar);

}TPScrollBarCallBack;

AFX_EXT_API BOOL  TP_InitScrollHook(TPScrollBarCallBack *pCallBack);
AFX_EXT_API BOOL  TP_ReleaseScrollHook(void);


AFX_EXT_API void  TP_UnReplaceScrollBar(CWnd *pWnd);
AFX_EXT_API void  TP_ReplaceScrollBar(CWnd *pWnd);
AFX_EXT_API void  TP_ResetScrollBar(TPScrollBarData *pData);

AFX_EXT_API void  TP_UnReplaceWnd(CWnd *pWnd);
AFX_EXT_API TPScrollBarData *  TP_ReplaceWnd(CWnd *pWnd);
AFX_EXT_API TPScrollBarData *  TP_GetReplaceData(CWnd *pWnd);

AFX_EXT_API void               TP_PumpMemory();



