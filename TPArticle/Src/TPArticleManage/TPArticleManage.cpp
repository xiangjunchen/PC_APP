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
	return g_stuArticleIOPluginInterface.stuChannelInterface.TP_GetChannelInfo(guidRes,stuChannelData);
}
LRESULT  TP_SetChannelInfo(GUID guidRes,TPChannelData &stuChannelData)
{
	return 	g_stuArticleIOPluginInterface.stuChannelInterface.TP_SetChannelInfo(guidRes,stuChannelData);
}

LRESULT  TP_DelChannelInfo(GUID guidRes)
{
	return g_stuArticleIOPluginInterface.stuChannelInterface.TP_DelChannelInfo(guidRes);
}
LRESULT  TP_GetArticleInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade, TPArticleData &stuArticleData) 
{
	return 	g_stuArticleIOPluginInterface.stuArticleInterface.TP_GetArticleInfo(guidRes,eClipGrade,stuArticleData);
}
LRESULT  TP_SetArticleInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade, TPArticleData &stuArticleData)
{
	return g_stuArticleIOPluginInterface.stuArticleInterface.TP_SetArticleInfo(guidRes,eClipGrade,stuArticleData);
}

LRESULT  TP_DelArticleInfo(GUID guidRes)
{
	return g_stuArticleIOPluginInterface.stuArticleInterface.TP_DelArticleInfo(guidRes);
}
CString  TP_GetCurArticleHtmlPath()
{
	return g_stuArticleIOPluginInterface.stuArticleInterface.TP_GetCurArticleHtmlPath();
}
CString   TP_GetArticleResourcePath(GUID guidRes, TCHAR *cPath)
{
	return g_stuArticleIOPluginInterface.stuArticleInterface.TP_GetArticleResourcePath(guidRes, cPath);
}
BOOL     TP_IsArticleExist(GUID guidChannel, TPChannelItem *pChannelItem)
{
	return g_stuArticleIOPluginInterface.stuArticleInterface.TP_IsArticleExist(guidChannel, pChannelItem);

}
LRESULT  TP_GetArticleChild(GUID guidNode, TPResDataArray &hChildRes)
{
	return g_stuArticleIOPluginInterface.stuArticleInterface.TP_GetArticleChild(guidNode, hChildRes);
}
LRESULT  TP_GetChannelNodeInfo(GUID guidRes,TPChannelNodeData &stuChannelNode) 
{
	return 	g_stuArticleIOPluginInterface.stuChannelNodeInterface.TP_GetChannelNodeInfo(guidRes,stuChannelNode);
}
LRESULT  TP_SetChannelNodeInfo(GUID guidRes,TPChannelNodeData &stuChannelNode)
{
	return g_stuArticleIOPluginInterface.stuChannelNodeInterface.TP_SetChannelNodeInfo(guidRes,stuChannelNode);
}

LRESULT  TP_DelChannelNodeInfo(GUID guidRes)
{
	return g_stuArticleIOPluginInterface.stuChannelNodeInterface.TP_DelChannelNodeInfo(guidRes);
}

LRESULT  TP_GetChannelNodeChild(GUID guidRes, TPResDataArray &hChildRes)
{
	return g_stuArticleIOPluginInterface.stuChannelNodeInterface.TP_GetChannelNodeChild(guidRes, hChildRes);
}
LRESULT  TP_GetChannelChild(GUID guidRes, TPResDataArray &hChildRes)
{
	return g_stuArticleIOPluginInterface.stuChannelInterface.TP_GetChannelChild(guidRes, hChildRes);
}
BOOL  TP_IsChannelExist(GUID guidChannelNode, TPChannelBase *pChannelInfo, GUID &guidExist)
{
	return g_stuArticleIOPluginInterface.stuChannelInterface.TP_IsChannelExist(guidChannelNode, pChannelInfo, guidExist);
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
	pInterface->stuArticleInterfce.TP_GetCurArticleHtmlPath = TP_GetCurArticleHtmlPath;
	pInterface->stuArticleInterfce.TP_GetArticleResourcePath = TP_GetArticleResourcePath;
	pInterface->stuArticleInterfce.TP_IsArticleExist = TP_IsArticleExist;
	pInterface->stuArticleInterfce.TP_GetArticleChild = TP_GetArticleChild;

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