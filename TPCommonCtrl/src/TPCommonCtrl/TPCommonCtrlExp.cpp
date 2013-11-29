#include "StdAfx.h"
#include "TPCommonCtrlExp.h"


LRESULT           (*g_pTaskBarCallBackFun)(CWnd* pWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) = NULL;
CWnd             *(*TP_ExtraSubControl)(CString sClassName,DWORD dwWindowLevel) = NULL;

namespace _tp_commonctrl
{

	UINT      g_uMainThreadID = 0;
	CWnd     *g_pMainFrameWnd = NULL;
	CWinApp  *g_pMinApp       = NULL;

	CWnd    *(*TP_ExtraCreateControl)(CWnd *pParentWnd,TPControlData *pCtrlData,DWORD dwWindowLevel) = NULL;

	CWnd    *(*TP_ExtendSubControl)(CString sClassName,DWORD dwWindowLevel) = NULL;
	CWnd    *(*TP_ExtendCreateControl)(CWnd *pParentWnd,TPControlData *pCtrlData,DWORD dwWindowLevel) = NULL;

	int      (WINAPI * TP_ExMessageBox)(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)  = NULL;
	LRESULT (*TP_BeepWarningMessagePtr)(long lType) = NULL;


CWnd  *TP_GetMainWnd()
{
	return g_pMainFrameWnd;
}

int  g_iOSVersion  = 0;
BOOL m_bDWMOpened  = -1;

int TP_GetOSVersion()
{
	if(g_iOSVersion ==0)
	{
		TP_GetBinPath(TRUE); 

		OSVERSIONINFO  osInfo;
		osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osInfo);
		if(osInfo.dwMajorVersion<=5)        g_iOSVersion = 1;
		else if(osInfo.dwMajorVersion == 6) g_iOSVersion = 2;
		return g_iOSVersion;
	}
	else return g_iOSVersion;	
}
BOOL  TP_GetIsVista()
{
	if(g_iOSVersion ==0) TP_GetOSVersion();
	return g_iOSVersion>=2;
}


extern CString   g_sSysSysPath;
extern CString   g_sSysBinPath;

LPCTSTR          TP_GetBinPath(BOOL bForce )
{
	if(bForce)
	{
		TCHAR cPath[MAX_PATH];
		::GetModuleFileName(g_hThisInstance,cPath,MAX_PATH);
		PathRemoveFileSpec(cPath);	
		g_sSysBinPath  = cPath;
	}
	return g_sSysBinPath;
}
LPCTSTR          TP_GetSysPath()
{
	return g_sSysSysPath;
}
LPCTSTR          TP_GetDllPath()
{
	g_sTemp = g_sSysSysPath;
	g_sTemp = g_sTemp.Left(g_sTemp.GetLength()-4);
	g_sTemp = g_sTemp + _L("\\Dll");
	return g_sTemp;

}
LPCTSTR          TP_GetDictionaryPath()
{
	g_sTemp = g_sSysSysPath + _L("\\Dict");
	return g_sTemp;
}
LPCTSTR          TP_GetTempPath()
{
	g_sTemp = g_sSysSysPath + _L("\\Temp");
	return g_sTemp;	
}
CString TP_GetRegisterClass()
{
	return g_sRegisterClass;
}
LPVOID   TP_AutoCreateControl(CWnd *pParentWnd,TPControlData *pCtrlData,int iCtrlSize,DWORD dwWindowLevel)
{
	CTPWndList *pWndList  = new CTPWndList();
	void       *pVoid     = NULL;
	for(int l=0;l<iCtrlSize;l++)
	{
		pVoid = TP_CreateControl(pParentWnd,&pCtrlData[l],dwWindowLevel);
		if(pVoid) pWndList ->Add(pVoid);
	}
	return pWndList;
}
CWnd    *TP_CreateControl(CWnd *pParentWnd,TPControlData *pCtrlData,DWORD dwWindowLevel)
{
	CString   cClassName = pCtrlData ->sClassType;	
	CWnd      *pWndAdd  = NULL;

	if(lstrcmpi(cClassName,WC_SCROLLBAR) == 0)
	{
		CTPScrollBar *pScrollBar = new CTPScrollBar();													
		pScrollBar ->m_dwWindowLevel = dwWindowLevel;
		pScrollBar ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);		
		pWndAdd = pScrollBar;		
	}		
	else if(lstrcmpi(cClassName,WC_LISTVIEW) == 0)
	{
		CTPListCtrl *pListCtrl = new CTPListCtrl();											
		pListCtrl ->m_dwWindowLevel = dwWindowLevel;
		pListCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pListCtrl;							
	}	
	else if(lstrcmpi(cClassName,WC_LISTBOX) == 0)
	{						
		CTPListBox *pListBox = new CTPListBox();	
		pListBox ->m_dwWindowLevel = dwWindowLevel;
		pListBox ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pListBox;							
	}
	else if(lstrcmpi(cClassName,WC_TREEVIEW) == 0)
	{
		CTPTreeCtrl *pTreeCtrl = new CTPTreeCtrl();	
		pTreeCtrl ->m_dwWindowLevel = dwWindowLevel;
		pTreeCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pTreeCtrl;							
	}			
	else if(lstrcmpi(cClassName,RICHEDIT_CLASS) == 0)
	{
		CTPRichEditCtrl *pEdit = new CTPRichEditCtrl();				
		pEdit ->m_dwWindowLevel = dwWindowLevel;
		pEdit ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pEdit;					
	}	
	else if(lstrcmpi(cClassName,WC_EDIT) == 0)
	{
		CTPEdit *pEdit = new CTPEdit();				
		pEdit ->m_dwWindowLevel = dwWindowLevel;
		pEdit ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pEdit;							
	}		
	else if(lstrcmpi(cClassName,WC_COMBOBOX) == 0)
	{				
		CTPComboBox *pComboBox = new CTPComboBox();	
		pComboBox ->m_dwWindowLevel = dwWindowLevel;
		pComboBox ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pComboBox;							
	}		
	else if(lstrcmpi(cClassName,WC_COMBOBOXEX) == 0)
	{				
		CTPComboBoxEx *pComboBox = new CTPComboBoxEx();											
		pComboBox ->m_dwWindowLevel = dwWindowLevel;
		pComboBox ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pComboBox;							
	}				
// 	else if(lstrcmpi(cClassName,WC_CHECKCOMBOBOX) == 0)
// 	{				
// 		CTPCheckComboBox *pComboBox = new CTPCheckComboBox();											
// 		pComboBox ->m_dwWindowLevel = dwWindowLevel;
// 		pComboBox ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		pWndAdd = pComboBox;							
// 	}					
	else if(lstrcmpi(cClassName,WC_BUTTON) == 0)
	{			
		CTPButton *pButton = new CTPButton();		
		pButton ->m_dwWindowLevel = dwWindowLevel;
		pButton ->Create(pCtrlData->sText,pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		if(lstrlen(pCtrlData->sFaceRes)>0)
		{		
			if(pCtrlData->dwStyle & BS_PUSHLIKE)	pButton ->SetBitmaps(g_sSysSkinPath + _L("\\") + pCtrlData->sFaceRes,FALSE,FALSE);
			else                                  	pButton ->SetBitmaps(g_sSysSkinPath + _L("\\") + pCtrlData->sFaceRes,FALSE,TRUE);
		}
		pButton ->SetToolTipText(pCtrlData ->sNotifyUp,pCtrlData ->sNotifyDown);
		pWndAdd = pButton;									
	}
// 	else if(lstrcmpi(cClassName,WC_STATIC) == 0)
// 	{			
// 		CTPStatic *pStatic = new CTPStatic();		
// 		pStatic ->m_dwWindowLevel = dwWindowLevel;
// 		pStatic ->Create(pCtrlData ->sText,pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		if(lstrlen(pCtrlData ->sFaceRes)>0)
// 		{
// 			CString sText = pCtrlData ->sFaceRes;
// 			sText += _L(".Bmp");
// 			TPCommonRes *pRes = TP_LoadBmpByte(sText);
// 			pStatic ->SetRes(pRes,FALSE,TRUE);
// 		}
// 		pWndAdd = pStatic;									
// 	}
	else if(lstrcmpi(cClassName,TRACKBAR_CLASS) == 0)
	{			
		CTPSliderCtrl *pSliderCtrl = new CTPSliderCtrl();											
		pSliderCtrl ->m_dwWindowLevel = dwWindowLevel;
		pSliderCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pSliderCtrl;									
	}	
	else if(lstrcmpi(cClassName,UPDOWN_CLASS) == 0)
	{			
		CTPSpinButtonCtrl *pSpinCtrl = new CTPSpinButtonCtrl();											
		pSpinCtrl ->m_dwWindowLevel = dwWindowLevel;
		pSpinCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pSpinCtrl;									
	}			
// 	else if(lstrcmpi(cClassName,PROGRESS_CLASS) == 0)
// 	{			
// 		CTPProgressCtrl *pProgressCtrl = new CTPProgressCtrl();											
// 		pProgressCtrl ->m_dwWindowLevel = dwWindowLevel;
// 		pProgressCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		pWndAdd = pProgressCtrl;									
// 	}			
// 	else if(lstrcmpi(cClassName,HOTKEY_CLASS) == 0)
// 	{			
// 		CTPHotKeyCtrl *pHotKeyCtrl = new CTPHotKeyCtrl();											
// 		pHotKeyCtrl ->m_dwWindowLevel = dwWindowLevel;
// 		pHotKeyCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		pWndAdd = pHotKeyCtrl;									
// 	}			
// 	else if(lstrcmpi(cClassName,WC_IPADDRESS) == 0)
// 	{			
// 		CTPIPAddressCtrl *pIPAddressCtrl = new CTPIPAddressCtrl();											
// 		pIPAddressCtrl ->m_dwWindowLevel = dwWindowLevel;
// 		pIPAddressCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		pWndAdd = pIPAddressCtrl;									
// 	}
	else if(lstrcmpi(cClassName,WC_TABCONTROL) == 0)
	{			
		CTPTabCtrl *pTabCtrl = new CTPTabCtrl();	
		pTabCtrl ->m_dwWindowLevel = dwWindowLevel;
		pTabCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pTabCtrl;									
	}		
// 	else if(lstrcmpi(cClassName,DATETIMEPICK_CLASS) == 0)
// 	{			
// 		CTPDateTimeCtrl *pDateTimeCtrl = new CTPDateTimeCtrl();											
// 		pDateTimeCtrl ->m_dwWindowLevel = dwWindowLevel;
// 		pDateTimeCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		pWndAdd = pDateTimeCtrl;									
// 	}
// 	else if(lstrcmpi(cClassName,MONTHCAL_CLASS) == 0)
// 	{			
// 		CTPMonthCalCtrl *pMonthCalCtrl = new CTPMonthCalCtrl();											
// 		pMonthCalCtrl ->m_dwWindowLevel = dwWindowLevel;
// 		pMonthCalCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		pWndAdd = pMonthCalCtrl;									
// 	}			
	else if(lstrcmpi(cClassName,WC_SLIDERBAR) == 0)
	{			
		CTPSliderBar *pSliderBar = new CTPSliderBar();											
		pSliderBar ->m_dwWindowLevel = dwWindowLevel;
		pSliderBar ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pSliderBar;									
	}			
// 	else if(lstrcmpi(cClassName,WC_DRAGLISTBOX) == 0)
// 	{			
// 		CTPDragListBox *pDragListBox = new CTPDragListBox();	
// 		pDragListBox ->m_dwWindowLevel = dwWindowLevel;
// 		pDragListBox ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		pWndAdd = pDragListBox;									
// 	}	
	else if(lstrcmpi(cClassName,WC_COMBOTREEBOX) == 0)
	{			
		CTPComboTreeBox *pComboTreeBox = new CTPComboTreeBox();											
		pComboTreeBox ->m_dwWindowLevel = dwWindowLevel;
		pComboTreeBox ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pComboTreeBox;									
	}
	else if(lstrcmpi(cClassName,WC_DIRTREECTRL) == 0)
	{			
		CTPDirTreeCtrl *pDirTreeCtrl = new CTPDirTreeCtrl();											
		pDirTreeCtrl ->m_dwWindowLevel = dwWindowLevel;
		pDirTreeCtrl ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pDirTreeCtrl;									
	}
	else if(lstrcmpi(cClassName,WC_DIRLISTCTRL) == 0)
	{			
		CTPDirListCtrl *pDirListCtrl = new CTPDirListCtrl();											
		pDirListCtrl ->m_dwWindowLevel = dwWindowLevel;
		pDirListCtrl ->Create(pCtrlData ->dwStyle|LVS_SHAREIMAGELISTS,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
		pWndAdd = pDirListCtrl;									
	}
// 	else if(lstrcmpi(cClassName,WC_LINK) == 0)
// 	{			
// 		CTPSysLink *pSysLink = new CTPSysLink();	
// 		pSysLink ->m_dwWindowLevel = dwWindowLevel;
// 		pSysLink ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		pWndAdd = pSysLink;									
// 	}		
// 	else if(lstrcmpi(cClassName,WC_COMBODIRTREEBOX) == 0)
// 	{			
// 		CTPComboDirTreeBox *pDirTreeBox = new CTPComboDirTreeBox();	
// 		pDirTreeBox ->Create(pCtrlData ->dwStyle,pCtrlData ->rtCtrl,pParentWnd,pCtrlData->nID);
// 		pDirTreeBox ->m_dwWindowLevel = dwWindowLevel;
// 		pWndAdd = pDirTreeBox;									
// 	}		
	else
	{
		if(TP_ExtraCreateControl)
			pWndAdd = TP_ExtraCreateControl(pParentWnd,pCtrlData,dwWindowLevel);
		if(pWndAdd == NULL && TP_ExtendCreateControl)
			pWndAdd = TP_ExtendCreateControl(pParentWnd,pCtrlData,dwWindowLevel);
	}		
	if(pWndAdd)
	{
		if(pCtrlData ->dwStyleEx !=0) pWndAdd ->ModifyStyleEx(0,pCtrlData ->dwStyleEx,0);
		pCtrlData->hWnd = pWndAdd ->m_hWnd;
		if(CTPBaseCtrl::m_stuSBDictionary.SBLoadString)
		{
			CString sText;
			pWndAdd ->GetWindowText(sText);
			if(sText.GetLength()>0) pWndAdd ->SetWindowText(CTPBaseCtrl::m_stuSBDictionary.SBLoadString(sText));		
		}
	}
	return pWndAdd;
}
void     TP_DestoryControl(CWnd *pParentWnd,LPVOID pLoadData)
{
	CTPWndList *pWndList =(CTPWndList *)pLoadData;
	if(pWndList == NULL) return;
	CWnd    *pWnd = NULL;
	for(INT l=0;l< pWndList ->GetCount();l++)
	{
		pWnd = (CWnd *)pWndList ->GetAt(l);
		pWnd ->SendMessage(WM_UNSUBCLASSWINDOW,0,0);
		pWnd ->DestroyWindow();
		delete pWnd;
	}
	pWndList ->RemoveAll();
	delete pWndList;
}

CWnd    *TP_SubControl(CWnd *pWndSub,DWORD dwWindowLevel)
{	
	TCHAR      cClassName[MAX_PATH];	
	CWnd      *pWndAdd  = NULL;
	ZeroMemory(cClassName,sizeof(TCHAR)*MAX_PATH);
	GetClassName(pWndSub ->m_hWnd,cClassName,MAX_PATH);
	if(CWnd::FromHandlePermanent(pWndSub ->m_hWnd)==NULL)
	{
		UINT uStyle = ::GetWindowLong(pWndSub ->m_hWnd,GWL_STYLE);
		if(lstrcmpi(cClassName,WC_SCROLLBAR) == 0)
		{
			CTPScrollBar *pScrollBar = new CTPScrollBar();													
			pScrollBar ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pScrollBar;		
		}		
		//else if(lstrcmpi(cClassName,WC_LISTVIEW) == 0)
		//{
		//	CTPListCtrl *pListCtrl = new CTPListCtrl();											
		//	pListCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pListCtrl;							
		//}	
		//else if(lstrcmpi(cClassName,WC_LISTBOX) == 0)
		//{						
		//	CTPListBox *pListBox = new CTPListBox();	
		//	pListBox ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pListBox;							
		//}
		//else if(lstrcmpi(cClassName,WC_TREEVIEW) == 0)
		//{
		//	CTPTreeCtrl *pTreeCtrl = new CTPTreeCtrl();	
		//	pTreeCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pTreeCtrl;							
		//}					
		//else if(lstrcmpi(cClassName,RICHEDIT_CLASS) == 0)
		//{
		//	CTPRichEditCtrl *pEdit = new CTPRichEditCtrl();				
		//	pEdit ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pEdit;					
		//}		
		//else if(lstrcmpi(cClassName,WC_EDIT) == 0)
		//{		
		//	CTPEdit *pEdit = new CTPEdit();				
		//	pEdit ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pEdit;							
		//}		
		//else if(lstrcmpi(cClassName,WC_COMBOBOX) == 0)
		//{				
		//	CTPComboBox *pComboBox = new CTPComboBox();	
		//	pComboBox ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pComboBox;							
		//}		
		//else if(lstrcmpi(cClassName,WC_COMBOBOXEX) == 0)
		//{				
		//	CTPComboBoxEx *pComboBox = new CTPComboBoxEx();											
		//	pComboBox ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pComboBox;							
		//}		
		//else if(lstrcmpi(cClassName,WC_CHECKCOMBOBOX) == 0)
		//{				
		//	CTPCheckComboBox *pComboBox = new CTPCheckComboBox();											
		//	pComboBox ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pComboBox;							
		//}								
		else if(lstrcmpi(cClassName,WC_BUTTON) == 0)
		{			
			CTPButton *pButton = new CTPButton();		
			pButton ->m_dwWindowLevel = dwWindowLevel;
			pWndAdd = pButton;									
		}
		//else if(lstrcmpi(cClassName,WC_STATIC) == 0)
		//{		
		//	CTPStatic *pStatic = new CTPStatic();		
		//	pStatic ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pStatic;									
		//}
		//else if(lstrcmpi(cClassName,TRACKBAR_CLASS) == 0)
		//{			
		//	CTPSliderCtrl *pSliderCtrl = new CTPSliderCtrl();											
		//	pSliderCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pSliderCtrl;									
		//}	
		//else if(lstrcmpi(cClassName,UPDOWN_CLASS) == 0)
		//{			
		//	CTPSpinButtonCtrl *pSpinCtrl = new CTPSpinButtonCtrl();											
		//	pSpinCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pSpinCtrl;									
		//}			
		//else if(lstrcmpi(cClassName,PROGRESS_CLASS) == 0)
		//{			
		//	CTPProgressCtrl *pProgressCtrl = new CTPProgressCtrl();											
		//	pProgressCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pProgressCtrl;									
		//}			
		//else if(lstrcmpi(cClassName,HOTKEY_CLASS) == 0)
		//{			
		//	CTPHotKeyCtrl *pHotKeyCtrl = new CTPHotKeyCtrl();											
		//	pHotKeyCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pHotKeyCtrl;									
		//}			
		//else if(lstrcmpi(cClassName,WC_IPADDRESS) == 0)
		//{			
		//	CTPIPAddressCtrl *pIPAddressCtrl = new CTPIPAddressCtrl();											
		//	pIPAddressCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pIPAddressCtrl;									
		//}
		//else if(lstrcmpi(cClassName,WC_TABCONTROL) == 0)
		//{			
		//	CTPTabCtrl *pTabCtrl = new CTPTabCtrl();	
		//	pTabCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pTabCtrl;									
		//}		
		//else if(lstrcmpi(cClassName,DATETIMEPICK_CLASS) == 0)
		//{			
		//	CTPDateTimeCtrl *pDateTimeCtrl = new CTPDateTimeCtrl();											
		//	pDateTimeCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pDateTimeCtrl;									
		//}
		//else if(lstrcmpi(cClassName,MONTHCAL_CLASS) == 0)
		//{			
		//	CTPMonthCalCtrl *pMonthCalCtrl = new CTPMonthCalCtrl();											
		//	pMonthCalCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pMonthCalCtrl;									
		//}			
		//else if(lstrcmpi(cClassName,WC_SLIDERBAR) == 0)
		//{			
		//	CTPSliderBar *pSliderBar = new CTPSliderBar();											
		//	pSliderBar ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pSliderBar;									
		//}			
		//else if(lstrcmpi(cClassName,WC_DRAGLISTBOX) == 0)
		//{			
		//	CTPDragListBox *pDragListBox = new CTPDragListBox();	
		//	pDragListBox ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pDragListBox;									
		//}	
		//else if(lstrcmpi(cClassName,WC_COMBOTREEBOX) == 0)
		//{			
		//	CTPComboTreeBox *pComboTreeBox = new CTPComboTreeBox();											
		//	pComboTreeBox ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pComboTreeBox;									
		//}
		//else if(lstrcmpi(cClassName,WC_DIRTREECTRL) == 0)
		//{			
		//	CTPDirTreeCtrl *pDirTreeCtrl = new CTPDirTreeCtrl();											
		//	pDirTreeCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pDirTreeCtrl;									
		//}
		//else if(lstrcmpi(cClassName,WC_DIRLISTCTRL) == 0)
		//{			
		//	CTPDirListCtrl *pDirListCtrl = new CTPDirListCtrl();											
		//	pDirListCtrl ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pDirListCtrl;									
		//}
		//else if(lstrcmpi(cClassName,WC_LINK) == 0)
		//{			
		//	CTPSysLink *pSysLink = new CTPSysLink();	
		//	pSysLink ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pSysLink;									
		//}		
		//else if(lstrcmpi(cClassName,WC_COMBODIRTREEBOX) == 0)
		//{			
		//	CTPComboDirTreeBox *pDirTreeBox = new CTPComboDirTreeBox();	
		//	pDirTreeBox ->m_dwWindowLevel = dwWindowLevel;
		//	pWndAdd = pDirTreeBox;									
		//}		
		//else 
		//{
		//	if(TP_ExtraSubControl)
		//		pWndAdd = TP_ExtraSubControl(cClassName,dwWindowLevel);
		//	//	if(TP_ExtraSubControl && pWndAdd == NULL)
		//	//		pWndAdd = TP_ExtendSubControl(cClassName,dwWindowLevel);
		//}		
		if(pWndAdd)
		{	
			SetClassLongPtr(pWndSub->m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);
			pWndAdd ->SubclassWindow(pWndSub ->m_hWnd);
			pWndAdd ->SetFont(TP_GetSysFont(FONT_BASE_TEXT),FALSE);			
		}
	}
	if(CTPBaseCtrl::m_stuSBDictionary.SBLoadString)
	{
		CString sText;
		pWndSub ->GetWindowText(sText);
		if(sText.GetLength()>0) pWndSub ->SetWindowText(CTPBaseCtrl::m_stuSBDictionary.SBLoadString(sText));		
	}
	return pWndAdd;
}
void     TP_ReleaseControl(CWnd *pParentWnd,LPVOID pLoadData)
{
	CTPWndList *pWndList =(CTPWndList *)pLoadData;
	if(pWndList == NULL) return;
	CWnd    *pWnd = NULL;
	for(INT l=0;l< pWndList ->GetCount();l++)
	{
		pWnd = (CWnd *)pWndList ->GetAt(l);
		pWnd ->SendMessage(WM_UNSUBCLASSWINDOW,0,0);
		pWnd ->SetFont(NULL);
		pWnd ->UnsubclassWindow();
		delete pWnd;
	}
	pWndList ->RemoveAll();
	delete pWndList;
}
BOOL   TP_IsParentWnd(HWND hParent,HWND hChild)
{	
	while(hChild)
	{
		if(hParent == hChild) return TRUE;
		hChild = GetParent(hChild);
	}
	return FALSE;
}
LPVOID   TP_AutoLoadControl(CWnd *pParentWnd,DWORD dwWindowLevel)
{	
	CWnd       *pWnd      = pParentWnd ->GetTopWindow();	
	CTPWndList *pWndList  = new CTPWndList();
	CWnd       *pWndAdd   = NULL;
	void       *pVoid     = NULL;

	if(CTPBaseCtrl::m_stuSBDictionary.SBLoadString)
	{
		CString sText;
		pParentWnd ->GetWindowText(sText);
		if(sText.GetLength()>0) pParentWnd ->SetWindowText(CTPBaseCtrl::m_stuSBDictionary.SBLoadString(sText));		
	}
	while (pWnd)
	{
		pWndAdd = TP_SubControl(pWnd,dwWindowLevel);
		if(pWndAdd)
		{		
			pVoid = pWndAdd;
			pWndList ->Add(pVoid);
		}
		pWnd = CWnd::FromHandle(::GetNextWindow(pWnd ->m_hWnd,GW_HWNDNEXT));		
	}
	return pWndList;
}
BOOL  TP_GetDWMStatus(BOOL bForce)
{
	if(g_iOSVersion>=2)
	{
		if(m_bDWMOpened ==-1 || bForce)
		{
			DwmIsCompositionEnabled(&m_bDWMOpened);			
		}
		return m_bDWMOpened;
	}
	else return FALSE;		
}
CTPRoundCorner  *TP_GetRoundCorner()
{
	return &g_stuRoundCorner;
}

TP_BORDER_STYLE  TP_GetBorderStyle()
{
	return g_eBorderStyle;
}
void TP_SetExtraCreateCtrl(CWnd  *(*fun)(CWnd *,TPControlData *,DWORD))
{
	TP_ExtraCreateControl = fun;
}
void TP_SetExtendSubCtrl(CWnd  *(*fun)(CString,DWORD))
{
	TP_ExtendSubControl = fun;
}
void TP_SetExtendCreateCtrl(CWnd  *(*fun)(CWnd *,TPControlData *,DWORD))
{
	TP_ExtendCreateControl = fun;
}
void TP_SetMessageBox(int  (WINAPI * fun)(HWND,LPCTSTR ,LPCTSTR ,UINT ))
{
	TP_ExMessageBox = fun;
}
void TP_SetMessageBeep(LRESULT (*fun)(long lType))
{
	TP_BeepWarningMessagePtr = fun;
}
BOOL  TP_SetDWMStatus(BOOL bSet)
{
	if(g_iOSVersion>=2)
	{		
		if(TP_GetDWMStatus(TRUE))
		{		
			DwmEnableComposition(bSet?DWM_EC_ENABLECOMPOSITION:DWM_EC_DISABLECOMPOSITION);
			m_bDWMOpened = bSet;
		}
	}
	return TRUE;
}
void TP_SetExtraSubCtrl(CWnd  *(*fun)(CString,DWORD))
{
	TP_ExtraSubControl = fun;
}
void TP_SetUITemplateState(HWND hWnd,UINT uState)
{
	if(hWnd)
	{
		if(uState & TP_GUISHOW_MAXMIZE)   
		{			
			::SendMessage(hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,0);
		}
		else if(uState & TP_GUISHOW_MINMIZE)
		{		
			::SendMessage(hWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
		}
	}
}
}