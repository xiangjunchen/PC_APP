// TPArticleIOPlugin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TPArticleIOPlugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LRESULT TP_GetProcessName(LPTSTR &strManageName)
{
	strManageName = _T("TPArticleIOPlugin");
	return S_OK;
}

LRESULT  TP_GetArticleInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPArticleData &stuArtData) 
{
	return S_OK;
}
LRESULT  TP_SetArticleInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPArticleData &stuArtData)
{
	return S_OK;
}

LRESULT  TP_DelArticleInfo(GUID guidRes)
{
	return S_OK;
}

LRESULT  TP_GetChannelInfo(GUID guidRes,TPChannelData &stuChannelData) 
{
	return S_OK;
}
LRESULT  TP_SetChannelInfo(GUID guidRes,TPChannelData &stuChannelData)
{
	return S_OK;
}

LRESULT  TP_DelChannelInfo(GUID guidRes)
{
	return S_OK;
}

LRESULT  TP_GetCommentInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPCommentData &stuCommentData) 
{
	return S_OK;
}
LRESULT  TP_SetCommentInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPCommentData &stuCommentData)
{
	return S_OK;
}

LRESULT  TP_DelCommentInfo(GUID guidRes)
{
	return S_OK;
}

LRESULT TP_GetPlugInFunction(TPArticleIOPluginInterface *pInterface)
{
	//Article
	pInterface->stuArticleInterface.TP_GetArticleInfo = TP_GetArticleInfo;
	pInterface->stuArticleInterface.TP_SetArticleInfo = TP_SetArticleInfo;
	pInterface->stuArticleInterface.TP_DelArticleInfo = TP_DelArticleInfo;

	//Column
	pInterface->stuChannelInterface.TP_GetChannelInfo = TP_GetChannelInfo;
	pInterface->stuChannelInterface.TP_SetChannelInfo = TP_SetChannelInfo;
	pInterface->stuChannelInterface.TP_DelChannelInfo = TP_DelChannelInfo;

	//Comment
	pInterface->stuCommentInterface.TP_GetCommentInfo = TP_GetCommentInfo;
	pInterface->stuCommentInterface.TP_SetCommentInfo = TP_SetCommentInfo;
	pInterface->stuCommentInterface.TP_DelCommentInfo = TP_DelCommentInfo;

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

void TP_GetPlugInProcess(TPArticleIOPluginProcess *pstuProcess)
{
	pstuProcess->strProcessName       = _T("TP_ClipFilePlunIn");	
	pstuProcess->TP_GetProcessName    = TP_GetProcessName;
	pstuProcess->TP_GetPlugInFunction = TP_GetPlugInFunction;
	pstuProcess->TP_InitData		  = TP_InitData;
	pstuProcess->TP_ReleaseData		  = TP_ReleaseData;
	
}