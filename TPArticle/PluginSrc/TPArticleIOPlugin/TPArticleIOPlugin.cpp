// TPArticleIOPlugin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TPArticleIOPlugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTPArticleDataBase g_stuArticleDataBase;

LRESULT TP_GetProcessName(LPTSTR &strManageName)
{
	strManageName = _T("TPArticleIOPlugin");
	return S_OK;
}

LRESULT  TP_GetArticleInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPArticleData &stuArtData) 
{
	g_stuArticleDataBase.ReadArticle(guidRes, stuArtData);
	return S_OK;
}
LRESULT  TP_SetArticleInfo(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPArticleData &stuArtData)
{
	g_stuArticleDataBase.WriteArticle(guidRes, stuArtData);
	return S_OK;
}

LRESULT  TP_DelArticleInfo(GUID guidRes)
{
	return S_OK;
}

LRESULT  TP_GetChannelInfo(GUID guidRes,TPChannelData &stuChannelData) 
{
	g_stuArticleDataBase.ReadChannel(guidRes, stuChannelData);
	return S_OK;
}
LRESULT  TP_SetChannelInfo(GUID guidRes,TPChannelData &stuChannelData)
{
	g_stuArticleDataBase.WriteChannel(guidRes, stuChannelData);
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

LRESULT  TP_GetChildRes(GUID guidRes, TPResDataArray &hChildRes)
{
	g_stuArticleDataBase.GetChannelRes(guidRes, hChildRes);
	return S_OK;
}

LRESULT TP_GetPlugInFunction(TPArticleIOPluginInterface *pInterface)
{
	//Article
	pInterface->stuArticleInterface.TP_GetArticleInfo = TP_GetArticleInfo;
	pInterface->stuArticleInterface.TP_SetArticleInfo = TP_SetArticleInfo;
	pInterface->stuArticleInterface.TP_DelArticleInfo = TP_DelArticleInfo;

	//ChannelNode
 	pInterface->stuChannelNodeInterface.TP_GetChildRes = TP_GetChildRes;
// 	pInterface->stuChannelNodeInterface.TP_SetChannelInfo = TP_SetChannelInfo;
// 	pInterface->stuChannelNodeInterface.TP_DelChannelInfo = TP_DelChannelInfo;

	//Channel
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
	CString sFilePersonal = g_stuArticleDataBase.GetLocalDataPath();
	if(!PathFileExists(sFilePersonal)) 
	{ 
		if(!::CreateDirectory(sFilePersonal,NULL)) 
		{
			ASSERT(0);
		}
		if(!PathFileExists(sFilePersonal + _T("\\Channel")))
		{
			::CreateDirectory(sFilePersonal + _T("\\Channel"),NULL);
			CString sDefaultChannel = TP_GetSysPath() + _T("\\Default\\Channel\\");
			CStringArray aFile;
			File_FindFile(sDefaultChannel,g_stuArticleDataBase.ResTypeToExt(TP_RES_CHANNEL),aFile);
			for (int l = 0; l < aFile.GetSize(); l++)
			{
				CString sFileName = PathFindFileName(aFile[l]);
				sFileName = sFilePersonal + _T("\\Channel\\") + sFileName;
				::CopyFile(aFile[l], sFileName,TRUE);
			}
		}
		if(!PathFileExists(sFilePersonal + _T("\\ChannelNode")))
		{
			::CreateDirectory(sFilePersonal + _T("\\ChannelNode"),NULL);
		}
		if(!PathFileExists(sFilePersonal + _T("\\Article")))
		{
			::CreateDirectory(sFilePersonal + _T("\\Article"),NULL);
		}
		if(!PathFileExists(sFilePersonal + _T("\\ArticleSource")))
		{
			::CreateDirectory(sFilePersonal + _T("\\ArticleSource"),NULL);
		}

	}
	g_stuArticleDataBase.InitFileMap();
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