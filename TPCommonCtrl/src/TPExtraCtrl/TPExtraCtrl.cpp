// TPExtraCtrl.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HINSTANCE        g_hThisInstance    = NULL;
CTPRoundCorner  *g_pRoundCorner     = NULL;

static AFX_EXTENSION_MODULE TPExtraCtrlDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TPExtraCtrl.DLL Initializing!\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(TPExtraCtrlDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL(�� ActiveX �ؼ�)��ʽ���ӵ�
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣
		g_hThisInstance = hInstance;

		new CDynLinkLibrary(TPExtraCtrlDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TPExtraCtrl.DLL ������ֹ��\n");


		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(TPExtraCtrlDLL);
	}
	return 1;   // ȷ��
}

TCHAR  g_cDim     = '.';

namespace _tp_extractrl
{
extern CWnd  *TP_ExtraSubControl(CString cClassName,DWORD dwWndLevel);
extern CWnd  *TP_ExtraCreateControl(CWnd *pParentWnd,TPControlData *pCtrlData,DWORD dwWindowLevel);

BOOL g_bInitAll = FALSE;

void TP_InitCtrl()
{
	if(g_bInitAll) return;
	g_bInitAll = TRUE;

	TP_InitCommonCtrl();
	TP_InitClass();

	CTPImageButton::RegisterClass(TRUE);
	CTPVUTableCtrl::RegisterClass(TRUE);
	CTPTimeCodeStaticEx::RegisterClass(TRUE);
	CTPTimeCodeEditEx::RegisterClass(TRUE);
	CTPTimeCodeEditNS::RegisterClass(TRUE);
	CTPPaneWnd::RegisterClass(TRUE);
	CTPSplitterBar::RegisterClass(TRUE);
	CTPSliderRoundCtrl::RegisterClass(TRUE);
	CTPVUSliderCtrl::RegisterClass(TRUE);
	CTPVUSliderCtrlEx::RegisterClass(TRUE);
	CTPClassCaseCtrl::RegisterClass(TRUE);
	CTPBtnToolBar::RegisterClass(TRUE);
	CTPTreeCtrlEx::RegisterClass(TRUE);
	CTPTimeLine::RegisterClass(TRUE);
	CTPListCtrlEx::RegisterClass(TRUE);
	CTPIconListCtrl::RegisterClass(TRUE);
	CTPLineCtrl::RegisterClass(TRUE);
	CTPToolCase::RegisterClass(TRUE);
	CTPEditListCtrl::RegisterClass(TRUE);
	CTPTabCtrlEx::RegisterClass(TRUE);
	CTPRectTracker::RegisterClass(TRUE);
	CTPEditEx::RegisterClass(TRUE);
	CTPListPropertyCtrl::RegisterClass(TRUE);
	CTPComboMenu::RegisterClass(TRUE);
	CTPSliderCtrlEX::RegisterClass(TRUE);
	CTPSliderCtrlExEx::RegisterClass(TRUE);
	CTPScrollBarZoom::RegisterClass(TRUE);
	CTPNumberEditExEx::RegisterClass(TRUE,g_hThisInstance);
	CTPRichEditCtrlEx::RegisterClass(TRUE);

	g_pRoundCorner = TP_GetRoundCorner();
	TP_SetExtraSubCtrl(TP_ExtraSubControl);
	TP_SetExtraCreateCtrl(TP_ExtraCreateControl);

	TCHAR cDic[4];
	::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,cDic,4 );	
	g_cDim = cDic[0];	
}
void TP_ReleaseCtrl()
{	
	if(!g_bInitAll) return;
	g_bInitAll = FALSE;

	CTPImageButton::RegisterClass(FALSE);
	CTPVUTableCtrl::RegisterClass(FALSE);
	CTPTimeCodeStaticEx::RegisterClass(FALSE);
	CTPTimeCodeEditEx::RegisterClass(FALSE);
	CTPTimeCodeEditNS::RegisterClass(FALSE);
	CTPPaneWnd::RegisterClass(FALSE);
	CTPSplitterBar::RegisterClass(FALSE);
	CTPSliderRoundCtrl::RegisterClass(FALSE);
	CTPVUSliderCtrl::RegisterClass(FALSE);
	CTPVUSliderCtrlEx::RegisterClass(FALSE);
	CTPClassCaseCtrl::RegisterClass(FALSE);
	CTPBtnToolBar::RegisterClass(FALSE);
	CTPTreeCtrlEx::RegisterClass(FALSE);
	CTPTimeLine::RegisterClass(FALSE);
	CTPListCtrlEx::RegisterClass(FALSE);
	CTPIconListCtrl::RegisterClass(FALSE);
	CTPLineCtrl::RegisterClass(FALSE);
	CTPToolCase::RegisterClass(FALSE);
	CTPEditListCtrl::RegisterClass(FALSE);
	CTPTabCtrlEx::RegisterClass(FALSE);
	CTPRectTracker::RegisterClass(FALSE);
	CTPEditEx::RegisterClass(FALSE);	
	CTPListPropertyCtrl::RegisterClass(FALSE);
	CTPComboMenu::RegisterClass(FALSE);
	CTPSliderCtrlEX::RegisterClass(FALSE);
	CTPSliderCtrlExEx::RegisterClass(FALSE);
	CTPNumberEditExEx::RegisterClass(FALSE,g_hThisInstance);
	CTPRichEditCtrlEx::RegisterClass(FALSE);

	TP_ReleaseClass();
	TP_ReleaseCommonCtrl();

}

}
//
//
//CString  g_sTemp;
//void TP_LoadMenuStr(CTPMenuEx *pMenu)
//{
//	CString sMenu[][2] = 
//	{
//		{_L("ȫ����ʾ"),                    _L("IDS_COMMON_SHOWALL"),},
//		{_L("ȫ������"),                    _L("IDS_COMMON_HIDEALL"),},
//		{_L("����"),                        _L("IDS_COMMON_MOVEUP"),},
//		{_L("����"),                        _L("IDS_COMMON_MOVEDOWN"),},
//		{_L("������"),                      _L("IDS_COMMON_MOVETOP"),},
//		{_L("���ײ�"),                      _L("IDS_COMMON_MOVEBOTTOM"),},
//	};
//	if(pMenu == NULL) return;
//
//	long    lCount = sizeof(sMenu)/(2*sizeof(CString));
//	int     iCount = pMenu->GetMenuItemCount();
//	UINT    nID    = 0; 
//	CString sText  = _L("");
//	for(int i=iCount-1;i>=0;i--)
//	{
//		nID = pMenu ->GetMenuItemID(i);
//		if(nID !=0)
//		{
//			pMenu ->GetMenuString(i,sText,MF_BYPOSITION);
//			if(sText.GetLength()<=0) continue;			
//			INT l=0;
//			for(l=0;l<lCount;l++)
//			{
//				if(sMenu[l][0].CompareNoCase(sText) == 0) break;
//			}
//			if(l>=lCount) continue;
//			sText = TP_LoadSysString(sMenu[l][1]);
//			pMenu ->ModifyMenu(i,MF_BYPOSITION|MF_STRING,nID,sText);
//		}
//	}
//}
//
//LPCTSTR TP_LoadStr(LPCTSTR sText)
//{
//	CString sDlgWndText[][2] = 
//	{		
//		{_L("�ر�"),		_L("IDS_COMMON_CLOSE"),},
//	};
//
//	g_sTemp = sText;
//
//	if(lstrlen(sText) <=0) return g_sTemp;
//	long lNotifyCount = sizeof(sDlgWndText)/(2*sizeof(CString));
//	INT l=0;
//	for(l=0;l<lNotifyCount;l++)
//	{
//		if(sDlgWndText[l][0].CompareNoCase(sText) == 0) break;
//	}
//	if(l>=lNotifyCount) return (LPCTSTR)g_sTemp;
//	g_sTemp = TP_LoadSysString(sDlgWndText[l][1]);
//	return (LPCTSTR)g_sTemp;
//}
