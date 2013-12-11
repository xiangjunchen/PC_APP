#include "StdAfx.h"


HMODULE g_hArticleManage = NULL;  
TPArticleManageProcess   g_stuPress;
TPArticleManageInterface g_stuArticleInterface;

LRESULT TP_InitArticleCenter()
{
	CString sFile   = _T("TPArticleManage");
#ifdef _DEBUG
	g_hArticleManage = ::LoadLibrary((CString)sFile + _T("ud.dll"));
#else
	g_hArticleManage = ::LoadLibrary((CString)sFile + _T("u.dll"));
#endif
	if(g_hArticleManage == NULL){ASSERT(0); return S_FALSE;}

	void (*TP_GetManageProcess)(TPArticleManageProcess * ) = NULL;
	TP_GetManageProcess = (void (* )(TPArticleManageProcess *)) ::GetProcAddress(g_hArticleManage,"TP_GetManageProcess");
	if(TP_GetManageProcess == NULL)
	{
		ASSERT(0);
		::FreeLibrary(g_hArticleManage);
		g_hArticleManage = NULL;
		return S_FALSE;
	}
	TP_GetManageProcess(&g_stuPress);	
	g_stuPress.TP_GetManageFunction(&g_stuArticleInterface);
	g_stuPress.TP_InitData(NULL);
	return S_OK;
}
LRESULT TP_ReleaseArticleCenter()
{
	if(g_stuPress.TP_ReleaseData)
		g_stuPress.TP_ReleaseData(NULL);
	if(g_hArticleManage)
	{
		::FreeLibrary(g_hArticleManage);
		g_hArticleManage = NULL;
		return S_OK;
	}
	return S_FALSE;
}