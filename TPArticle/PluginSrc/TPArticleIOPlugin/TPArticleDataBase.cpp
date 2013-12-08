#include "StdAfx.h"
void    File_FindFile(CString strPath,CString strFile,BOOL bPath,CStringArray &aFile)
{
	WIN32_FIND_DATA findData;
	HANDLE          hFind   = FindFirstFile(strPath + strFile ,&findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if(bPath && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')			
			aFile.Add(strPath + _L("\\") + findData.cFileName);
		else if(!bPath && !((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
			aFile.Add(strPath + _L("\\") + findData.cFileName);

		while(FindNextFile(hFind,&findData))
		{
			if(bPath && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
				aFile.Add(strPath + _L("\\") + findData.cFileName);
			else if(!bPath && !((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
				aFile.Add(strPath + _L("\\") + findData.cFileName);
		}
		FindClose(hFind);
	}
}
void   TP_FindFileOnly(CString strPath,CStringArray &aFile)
{
	WIN32_FIND_DATA findData;
	HANDLE          hFind   = FindFirstFile(strPath + _L("\\*.*") ,&findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if(!((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
			aFile.Add(strPath + _L("\\") + findData.cFileName);

		while(FindNextFile(hFind,&findData))
		{		
			if(!((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
				aFile.Add(strPath + _L("\\") + findData.cFileName);
		}
		FindClose(hFind);
	}
}

void   File_FindFile(CString strPath,CString sType,CStringArray &aFile)
{
	WIN32_FIND_DATA findData;
	HANDLE          hFind   = FindFirstFile(strPath + _L("\\*.") + sType ,&findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
		{			
		}
		else if(!((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
			aFile.Add(strPath + _L("\\") + findData.cFileName);

		while(FindNextFile(hFind,&findData))
		{
			if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
			{		
			}
			else if(!((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
				aFile.Add(strPath + _L("\\") + findData.cFileName);
		}
		FindClose(hFind);
	}
}
BOOL TP_FindFile(CString& sFindPath)
{
	WIN32_FIND_DATA findData;
	HANDLE          hFind   = FindFirstFile(sFindPath,&findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return TRUE;
	}

	return FALSE;
}
CTPArticleDataBase::CTPArticleDataBase(void)
{
	m_sLocalDataPath = _T("");
}

CTPArticleDataBase::~CTPArticleDataBase(void)
{
}
LRESULT CTPArticleDataBase::ReadArticle(GUID guidRes,TPArticleData &stuArticleData)
{
	DWORD dwSize = 0;
	CString  sFileName = GetResFilePath(guidRes, TP_RES_ARTICLE, &stuArticleData);
	CTPMemFile hMemFile;
	hMemFile.ReadFile(sFileName);
	DWORD dwTemp = 0;
	//
	stuArticleData.Release();
	hMemFile.Read(&stuArticleData.dwVersion,sizeof(DWORD));
	hMemFile.Read(&stuArticleData.dwState,sizeof(DWORD));
	hMemFile.Read(&stuArticleData.guidRes,sizeof(GUID));				
	hMemFile.Read(&stuArticleData.guidNode,sizeof(GUID));				
	hMemFile.Read(&stuArticleData.guidDBType,sizeof(GUID));				
	hMemFile.Read(&stuArticleData.tmCreate,sizeof(SYSTEMTIME));				
	hMemFile.Read(&stuArticleData.tmModify,sizeof(SYSTEMTIME));				
	hMemFile.Read(&stuArticleData.tmRead,sizeof(SYSTEMTIME));				
	hMemFile.Read(&stuArticleData.eResType,sizeof(ULONGLONG));			

	TP_ReadStrFromFile(stuArticleData.cText, hMemFile);
	hMemFile.Read(&stuArticleData.lCommentSize,sizeof(INT64));				
	stuArticleData.stuChannelItem.ReadFile(hMemFile);
	hMemFile.Read(&dwSize,sizeof(int));			
	for(DWORD l = 0 ; l < dwSize; l++)
	{
		TPPictureItem *pItem = new TPPictureItem;
		pItem->ReadFile(hMemFile);
		stuArticleData.aPictureItem.Add(pItem);
	}
	//
	hMemFile.Close();
	return S_OK;
}
LRESULT CTPArticleDataBase::WriteArticle(GUID guidRes,TPArticleData &stuArticleData)
{
	DWORD dwSize = 0;
	CString  sFileName = GetResFilePath(guidRes, TP_RES_ARTICLE, &stuArticleData);
	CTPMemFile hMemFile;
	//
	stuArticleData.dwVersion = TP_ARTICLE_VERSION;
	hMemFile.Write(&stuArticleData.dwVersion,sizeof(DWORD));
	hMemFile.Write(&stuArticleData.dwState,sizeof(DWORD));
	hMemFile.Write(&stuArticleData.guidRes,sizeof(GUID));				
	hMemFile.Write(&stuArticleData.guidNode,sizeof(GUID));				
	hMemFile.Write(&stuArticleData.guidDBType,sizeof(GUID));				
	hMemFile.Write(&stuArticleData.tmCreate,sizeof(SYSTEMTIME));				
	hMemFile.Write(&stuArticleData.tmModify,sizeof(SYSTEMTIME));				
	hMemFile.Write(&stuArticleData.tmRead,sizeof(SYSTEMTIME));				
	hMemFile.Write(&stuArticleData.eResType,sizeof(ULONGLONG));			

	TP_WriteStrToFile(stuArticleData.cText, hMemFile);
	hMemFile.Write(&stuArticleData.lCommentSize,sizeof(INT64));				
	stuArticleData.stuChannelItem.SaveFile(hMemFile);
	hMemFile.Write(&dwSize,sizeof(int));			
	for(DWORD l = 0 ; l < dwSize; l++)
	{
		TPPictureItem *pItem = new TPPictureItem;
		pItem->SaveFile(hMemFile);
		stuArticleData.aPictureItem.Add(pItem);
	}
	//
	hMemFile.WriteFile(sFileName);
	hMemFile.Close();
	return S_OK;
}

LRESULT CTPArticleDataBase::ReadChannel(GUID guidRes,TPChannelData &stuChannelData)
{
	DWORD dwSize = 0;
	CString  sFileName = GetResFilePath(guidRes, TP_RES_CHANNEL, &stuChannelData);

	CTPMemFile hMemFile;
	hMemFile.ReadFile(sFileName);
	DWORD dwTemp = 0;
	//
	stuChannelData.Release();
	hMemFile.Read(&stuChannelData.dwVersion,sizeof(DWORD));
	hMemFile.Read(&stuChannelData.dwState,sizeof(DWORD));
	hMemFile.Read(&stuChannelData.guidRes,sizeof(GUID));				
	hMemFile.Read(&stuChannelData.guidNode,sizeof(GUID));				
	hMemFile.Read(&stuChannelData.guidDBType,sizeof(GUID));				
	hMemFile.Read(&stuChannelData.tmCreate,sizeof(SYSTEMTIME));				
	hMemFile.Read(&stuChannelData.tmModify,sizeof(SYSTEMTIME));				
	hMemFile.Read(&stuChannelData.tmRead,sizeof(SYSTEMTIME));				
	hMemFile.Read(&stuChannelData.eResType,sizeof(ULONGLONG));			

	hMemFile.Read(&stuChannelData.eNodeType,sizeof(ULONGLONG));			
	hMemFile.Read(&stuChannelData.lUpdateInterval,sizeof(int));			
	hMemFile.Read(&stuChannelData.lSaveNum,sizeof(int));			

	stuChannelData.stuChannelBase.ReadFile(hMemFile);

	hMemFile.Read(&dwSize,sizeof(int));			
	for(DWORD l = 0 ; l < dwSize; l++)
	{
		TPChannelItem *pItem = new TPChannelItem;
		pItem->ReadFile(hMemFile);
		stuChannelData.aChannelItemAll.Add(pItem);
	}

	//
	hMemFile.Close();
	return S_OK;
}
LRESULT CTPArticleDataBase::WriteChannel(GUID guidRes,TPChannelData &stuChannelData)
{
	DWORD dwSize = 0;
	CString  sFileName = GetResFilePath(guidRes, TP_RES_CHANNEL, &stuChannelData);
	CTPMemFile hMemFile;
	//
	stuChannelData.dwVersion = TP_CHANNEL_VERSION;
	hMemFile.Write(&stuChannelData.dwVersion,sizeof(DWORD));
	hMemFile.Write(&stuChannelData.dwState,sizeof(DWORD));
	hMemFile.Write(&stuChannelData.guidRes,sizeof(GUID));				
	hMemFile.Write(&stuChannelData.guidNode,sizeof(GUID));				
	hMemFile.Write(&stuChannelData.guidDBType,sizeof(GUID));				
	hMemFile.Write(&stuChannelData.tmCreate,sizeof(SYSTEMTIME));				
	hMemFile.Write(&stuChannelData.tmModify,sizeof(SYSTEMTIME));				
	hMemFile.Write(&stuChannelData.tmRead,sizeof(SYSTEMTIME));				
	hMemFile.Write(&stuChannelData.eResType,sizeof(ULONGLONG));			

	hMemFile.Write(&stuChannelData.eNodeType,sizeof(ULONGLONG));			
	hMemFile.Write(&stuChannelData.lUpdateInterval,sizeof(int));			
	hMemFile.Write(&stuChannelData.lSaveNum,sizeof(int));			

	stuChannelData.stuChannelBase.SaveFile(hMemFile);

	dwSize = stuChannelData.aChannelItemAll.GetSize();
	hMemFile.Write(&dwSize,sizeof(int));			
	for(DWORD l = 0 ; l < dwSize; l++)
	{
		stuChannelData.aChannelItemAll[l]->SaveFile(hMemFile);
	}

	//
	hMemFile.WriteFile(sFileName);
	hMemFile.Close();
	return S_OK;
}
LRESULT CTPArticleDataBase::GetChannelRes(GUID guidRes, TPResDataArray &hChildRes)
{
	CString sChannelNodePath = GetLocalDataPath() + _T("\\Channel\\");
	if(!PathFileExists(sChannelNodePath)) return S_FALSE;

	CString      sFileName = _T("");
	CStringArray aFile;
	File_FindFile(sChannelNodePath,_T("chl"),aFile);
	for (int l = 0 ; l < aFile.GetSize(); l++)
	{
		sFileName = PathFindFileName(aFile[l]);
		if(sFileName.Right(4).CompareNoCase(ResTypeToExt(TP_RES_CHANNEL))==0)
		{
			TPResData stuResData;
			stuResData.guidRes = TP_GuidFromString(sFileName.Left(36));
			hChildRes.Add(stuResData);
		}		
	}

	return S_OK;
}
CString CTPArticleDataBase::GetLocalDataPath()
{
	if(!m_sLocalDataPath.IsEmpty())	return m_sLocalDataPath;

	m_sLocalDataPath = TP_GetWindowsSysPath(TP_WINDOWSPATH_PERSONAL);
	m_sLocalDataPath += _T("\\Article");
	return m_sLocalDataPath;
}
CString CTPArticleDataBase::ResTypeToExt(TP_RES_TYPE eResType)
{
	if(eResType & TP_RES_CHANNELNODE)		return _T(".che");
	else if(eResType & TP_RES_CHANNEL)		return _T(".chl");
	else if(eResType & TP_RES_ARTICLE)		return _T(".ate");
	else if(eResType & TP_RES_COMMENT)		return _T(".cmt");
	else									ASSERT(0);
	return _T("");
}
CString CTPArticleDataBase::ResTypeToFolderName(TP_RES_TYPE eResType)
{
	if(eResType & TP_RES_CHANNELNODE)		return _T("ChannelNode");
	else if(eResType & TP_RES_CHANNEL)		return _T("Channel");
	else if(eResType & TP_RES_ARTICLE)		return _T("Article");
	else if(eResType & TP_RES_COMMENT)		return _T("ArticleSource");
	else									ASSERT(0);
	return _T("");
}
CString CTPArticleDataBase::GetResFilePath(GUID guidRes,TP_RES_TYPE eResType,void *pData)
{
	CString sFileName = GetLocalDataPath();
	if(!pData)	{ASSERT(0);return sFileName;}

	CString sGuidRes,sGuidNode,sName ;
	if(eResType & TP_RES_CHANNELNODE)
	{
		TPChannelNode *pNode = (TPChannelNode *)pData;
		sGuidRes = TP_UuidToString(&guidRes);
		sGuidNode = TP_UuidToString(&pNode->guidNode) ;
		sName	= pNode->cNodeName;
	}
	else if(eResType & TP_RES_CHANNEL)
	{
		TPChannelData *pChannelData = (TPChannelData *)pData;
		sGuidRes = TP_UuidToString(&guidRes);
		sGuidNode = TP_UuidToString(&pChannelData->guidNode) ;
		sName	= pChannelData->stuChannelBase.cChannelTitle;

	}
	else if(eResType & TP_RES_ARTICLE)
	{
		TPArticleData *pArticleData = (TPArticleData *)pData;
		sGuidRes = TP_UuidToString(&guidRes);
		sGuidNode = TP_UuidToString(&pArticleData->guidNode) ;
		sName	= pArticleData->stuChannelItem.cItemTitle;

	}
	else if(eResType & TP_RES_COMMENT)
	{
		TPCommentData *pCommentData = (TPCommentData *)pData;
	}
	else
	{
		ASSERT(0);
	}	
	TP_GetValidFileName(sName);
	sFileName.Format(_T("%s\\%s\\%s#%s#%s%s"),GetLocalDataPath(),ResTypeToFolderName(eResType),sGuidRes,sGuidNode,sName,ResTypeToExt(eResType));
	if(!PathFileExists(sFileName))
	{
		CStringArray aFile;
		File_FindFile(GetLocalDataPath(),TP_GuidToString(&guidRes),aFile);
		ASSERT(aFile.GetSize() == 1);
		sFileName = aFile[0];
	}
	return sFileName;
}