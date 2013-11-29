// TPArticleManage.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TPArticleManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LRESULT TP_GetProcessName(LPTSTR &strManageName)
{
	strManageName = _T("TPArticleManage");
	return S_OK;
}

LRESULT TP_InitData(void *pData)
{
	return S_OK;

}
LRESULT TP_ReleaseData(void *pData)
{
	return S_OK;

}

LRESULT TP_GetManageFunction(TPArticleManageInterface *pInterface)
{
	return S_OK;
}
void TP_GetManageProcess(TPArticleManageProcess *pstuProcess)
{
	pstuProcess->strProcessName       = _T("TPArticleManage");	
	pstuProcess->TP_GetProcessName    = TP_GetProcessName;
	pstuProcess->TP_GetManageFunction = TP_GetManageFunction;
	pstuProcess->TP_InitData		  = TP_InitData;
	pstuProcess->TP_ReleaseData		  = TP_ReleaseData;

}