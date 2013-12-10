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

HMODULE g_hArticleIOPlugin = NULL;  
TPArticleIOPluginProcess g_stuPress;
TPArticleIOPluginInterface g_stuArticleIOPluginInterface;

LRESULT TP_InitData(void *pData)
{
	CString sFile   = _T("TPArticleIOPlugin");
#ifdef _DEBUG
	g_hArticleIOPlugin = ::LoadLibrary((CString)sFile + _T("ud.dll"));
#else
	g_hArticleIOPlugin = ::LoadLibrary((CString)sFile + _T("u.dll"));
#endif
	if(g_hArticleIOPlugin == NULL){ASSERT(0); return S_FALSE;}

	void (*TP_GetPlugInProcess)(TPArticleIOPluginProcess *stuPress) = NULL;
	TP_GetPlugInProcess = (void (* )(TPArticleIOPluginProcess *)) ::GetProcAddress(g_hArticleIOPlugin,"TP_GetPlugInProcess");
	if(TP_GetPlugInProcess == NULL)
	{
		ASSERT(0);
		::FreeLibrary(g_hArticleIOPlugin);
		g_hArticleIOPlugin = NULL;
		return S_FALSE;
	}
	TP_GetPlugInProcess(&g_stuPress);	
	g_stuPress.TP_GetPlugInFunction(&g_stuArticleIOPluginInterface);
	g_stuPress.TP_InitData(NULL);
	return S_OK;

}
LRESULT TP_ReleaseData(void *pData)
{
	g_stuPress.TP_ReleaseData(NULL);
	if(g_hArticleIOPlugin)
	{
		::FreeLibrary(g_hArticleIOPlugin);
		g_hArticleIOPlugin = NULL;
		return S_OK;
	}
	return S_OK;

}
LRESULT  TP_GetChannelInfo(GUID guidRes,TPChannelData &stuChannelData) 
{
	g_stuArticleIOPluginInterface.stuChannelInterface.TP_GetChannelInfo(guidRes,stuChannelData);
	return S_OK;
}
LRESULT  TP_SetChannelInfo(GUID guidRes,TPChannelData &stuChannelData)
{
	g_stuArticleIOPluginInterface.stuChannelInterface.TP_SetChannelInfo(guidRes,stuChannelData);
	return S_OK;
}

LRESULT  TP_DelChannelInfo(GUID guidRes)
{
	g_stuArticleIOPluginInterface.stuChannelInterface.TP_DelChannelInfo(guidRes);
	return S_OK;
}
LRESULT  TP_GetArticleInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade, TPArticleData &stuArticleData) 
{
	g_stuArticleIOPluginInterface.stuArticleInterface.TP_GetArticleInfo(guidRes,eClipGrade,stuArticleData);
	return S_OK;
}
LRESULT  TP_SetArticleInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade, TPArticleData &stuArticleData)
{
	g_stuArticleIOPluginInterface.stuArticleInterface.TP_SetArticleInfo(guidRes,eClipGrade,stuArticleData);
	return S_OK;
}

LRESULT  TP_DelArticleInfo(GUID guidRes)
{
	g_stuArticleIOPluginInterface.stuArticleInterface.TP_DelArticleInfo(guidRes);
	return S_OK;
}
LRESULT  TP_GetChannelNodeInfo(GUID guidRes,TPChannelNodeData &stuChannelNode) 
{
	g_stuArticleIOPluginInterface.stuChannelNodeInterface.TP_GetChannelNodeInfo(guidRes,stuChannelNode);
	return S_OK;
}
LRESULT  TP_SetChannelNodeInfo(GUID guidRes,TPChannelNodeData &stuChannelNode)
{
	g_stuArticleIOPluginInterface.stuChannelNodeInterface.TP_SetChannelNodeInfo(guidRes,stuChannelNode);
	return S_OK;
}

LRESULT  TP_DelChannelNodeInfo(GUID guidRes)
{
	g_stuArticleIOPluginInterface.stuChannelNodeInterface.TP_DelChannelNodeInfo(guidRes);
	return S_OK;
}

LRESULT  TP_GetChannelNodeChild(GUID guidRes, TPResDataArray &hChildRes)
{
	g_stuArticleIOPluginInterface.stuChannelNodeInterface.TP_GetChannelNodeChild(guidRes, hChildRes);
	return S_OK;
}
LRESULT  TP_GetChannelChild(GUID guidRes, TPResDataArray &hChildRes)
{
	g_stuArticleIOPluginInterface.stuChannelInterface.TP_GetChannelChild(guidRes, hChildRes);
	return S_OK;
}
BOOL  TP_IsChannelExist(GUID guidChannelNode, TPChannelBase *pChannelInfo)
{
	return g_stuArticleIOPluginInterface.stuChannelInterface.TP_IsChannelExist(guidChannelNode, pChannelInfo);
}

LRESULT TP_GetManageFunction(TPArticleManageInterface *pInterface)
{
	//ChannelNode
	pInterface->stuChannelNodeInterface.TP_GetChannelNodeChild = TP_GetChannelNodeChild;
	pInterface->stuChannelNodeInterface.TP_GetChannelNodeInfo = TP_GetChannelNodeInfo;
	pInterface->stuChannelNodeInterface.TP_SetChannelNodeInfo = TP_SetChannelNodeInfo;
	pInterface->stuChannelNodeInterface.TP_DelChannelNodeInfo = TP_DelChannelNodeInfo;

	//Channel
	pInterface->stuChannelInterface.TP_GetChannelInfo = TP_GetChannelInfo;
	pInterface->stuChannelInterface.TP_SetChannelInfo = TP_SetChannelInfo;
	pInterface->stuChannelInterface.TP_DelChannelInfo = TP_DelChannelInfo;
	pInterface->stuChannelInterface.TP_GetChannelChild = TP_GetChannelChild;
	pInterface->stuChannelInterface.TP_IsChannelExist = TP_IsChannelExist;

	//Article
	pInterface->stuArticleInterfce.TP_GetArticleInfo = TP_GetArticleInfo;
	pInterface->stuArticleInterfce.TP_SetArticleInfo = TP_SetArticleInfo;
	pInterface->stuArticleInterfce.TP_DelArticleInfo = TP_DelArticleInfo;
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