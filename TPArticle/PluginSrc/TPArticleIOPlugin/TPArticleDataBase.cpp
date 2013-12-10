#include "StdAfx.h"

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

	m_aArticleFileName.SetAt(TP_GuidToString(&guidRes), sFileName);
	return S_OK;
}

LRESULT CTPArticleDataBase::ReadChannel(CString  sFileName,TPChannelData &stuChannelData)
{
	if(sFileName.IsEmpty())	return  S_FALSE;

	DWORD dwSize = 0;

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
	TP_ReadStrFromFile(stuChannelData.cKeyDiv, hMemFile);

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
LRESULT CTPArticleDataBase::ReadChannel(GUID guidRes,TPChannelData &stuChannelData)
{
	CString  sFileName = GetResFilePath(guidRes, TP_RES_CHANNEL, &stuChannelData);

	ReadChannel(sFileName, stuChannelData);
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
	TP_WriteStrToFile(stuChannelData.cKeyDiv, hMemFile);

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
	m_aChannelFileName.SetAt(TP_GuidToString(&guidRes), sFileName);
	return S_OK;
}
LRESULT CTPArticleDataBase::ReadChannelNode(GUID guidRes,TPChannelNodeData &stuChannelNodeData)
{
	DWORD dwSize = 0;
	CString  sFileName = GetResFilePath(guidRes, TP_RES_CHANNELNODE, &stuChannelNodeData);

	CTPMemFile hMemFile;
	hMemFile.ReadFile(sFileName);
	DWORD dwTemp = 0;
	//
	stuChannelNodeData.Release();
	hMemFile.Read(&stuChannelNodeData.dwVersion,sizeof(DWORD));
	hMemFile.Read(&stuChannelNodeData.dwState,sizeof(DWORD));
	hMemFile.Read(&stuChannelNodeData.guidRes,sizeof(GUID));				
	hMemFile.Read(&stuChannelNodeData.guidNode,sizeof(GUID));				
	hMemFile.Read(&stuChannelNodeData.guidDBType,sizeof(GUID));				
	hMemFile.Read(&stuChannelNodeData.tmCreate,sizeof(SYSTEMTIME));				
	hMemFile.Read(&stuChannelNodeData.tmModify,sizeof(SYSTEMTIME));				
	hMemFile.Read(&stuChannelNodeData.tmRead,sizeof(SYSTEMTIME));				
	hMemFile.Read(&stuChannelNodeData.eResType,sizeof(ULONGLONG));			

	hMemFile.Read(&stuChannelNodeData.eNodeType,sizeof(ULONGLONG));			
	TP_ReadStrFromFile(stuChannelNodeData.cNodeName, hMemFile);
	//
	hMemFile.Close();

	return S_OK;
}
LRESULT CTPArticleDataBase::WriteChannelNode(GUID guidRes,TPChannelNodeData &stuChannelNodeData)
{
	DWORD dwSize = 0;
	CString  sFileName = GetResFilePath(guidRes, TP_RES_CHANNELNODE, &stuChannelNodeData);
	CTPMemFile hMemFile;
	//
	stuChannelNodeData.dwVersion = TP_CHANNELNODE_VERSION;
	hMemFile.Write(&stuChannelNodeData.dwVersion,sizeof(DWORD));
	hMemFile.Write(&stuChannelNodeData.dwState,sizeof(DWORD));
	hMemFile.Write(&stuChannelNodeData.guidRes,sizeof(GUID));				
	hMemFile.Write(&stuChannelNodeData.guidNode,sizeof(GUID));				
	hMemFile.Write(&stuChannelNodeData.guidDBType,sizeof(GUID));				
	hMemFile.Write(&stuChannelNodeData.tmCreate,sizeof(SYSTEMTIME));				
	hMemFile.Write(&stuChannelNodeData.tmModify,sizeof(SYSTEMTIME));				
	hMemFile.Write(&stuChannelNodeData.tmRead,sizeof(SYSTEMTIME));				
	hMemFile.Write(&stuChannelNodeData.eResType,sizeof(ULONGLONG));			

	hMemFile.Write(&stuChannelNodeData.eNodeType,sizeof(ULONGLONG));			
	TP_WriteStrToFile(stuChannelNodeData.cNodeName, hMemFile);
	//
	hMemFile.WriteFile(sFileName);
	hMemFile.Close();

	m_aChannelNodeFileName.SetAt(TP_GuidToString(&guidRes), sFileName);
	return S_OK;
}
LRESULT CTPArticleDataBase::GetChannelNodeChild(GUID guidRes, TPResDataArray &hChildRes)
{
	hChildRes.RemoveAll();
	CString sCatalog  = TP_UuidToString(&guidRes);
	CString sFileName,sGuid;
	POSITION posGet = NULL;
	m_aChannelNodeFileName.Lock();
	posGet = m_aChannelNodeFileName.GetStartPosition();
	while (posGet)
	{
		m_aChannelNodeFileName.GetNextAssoc(posGet,sGuid,sFileName);
		if(sFileName.Find(sCatalog)>0) 
		{
			TPResData stuRes;
			stuRes.guidRes = TP_UuidFromString(sGuid);
			hChildRes.Add(stuRes);
		}
	}
	m_aChannelNodeFileName.Unlock();
	return S_OK;
}
LRESULT CTPArticleDataBase::GetChannelChild(GUID guidRes, TPResDataArray &hChildRes)
{
	hChildRes.RemoveAll();
	CString sCatalog  = TP_UuidToString(&guidRes);
	CString sFileName,sGuid;
	POSITION posGet = NULL;
	m_aChannelFileName.Lock();
	posGet = m_aChannelFileName.GetStartPosition();
	while (posGet)
	{
		m_aChannelFileName.GetNextAssoc(posGet,sGuid,sFileName);
		if(sFileName.Find(sCatalog)>0) 
		{
			TPResData stuRes;
			stuRes.guidRes = TP_UuidFromString(sGuid);
			hChildRes.Add(stuRes);
		}
	}
	m_aChannelFileName.Unlock();
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
		TPChannelNodeData *pNode = (TPChannelNodeData *)pData;
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
		CTPMapStringToString *p_aMap = GetMap(eResType);
		if(p_aMap)
		{
			p_aMap->Lookup(sGuidRes, sFileName);
		}
	}
	return sFileName;
}

LRESULT CTPArticleDataBase::InitFileMap()
{
	CString sChannelPath	 = GetLocalDataPath() + _T("\\Channel\\");
	CString sChannelNodePath = GetLocalDataPath() + _T("\\ChannelNode\\");
	CString sArticlePath	 = GetLocalDataPath() + _T("\\Article\\");
	if(!PathFileExists(sChannelPath) || !PathFileExists(sChannelNodePath) || !PathFileExists(sArticlePath)) return S_FALSE;

	CString      sFileName = _T("");
	CString      sExt      = _T("");
	CStringArray aFile;
	//ChannelNode
	sExt = ResTypeToExt(TP_RES_CHANNELNODE);
	aFile.RemoveAll();
	File_FindFile(sChannelNodePath,sExt,aFile);
	m_aChannelNodeFileName.RemoveAll();
	for (int l = 0 ; l < aFile.GetSize(); l++)
	{
		sFileName = PathFindFileName(aFile[l]);
		if(sFileName.Right(4).CompareNoCase(sExt)==0)
		{
			m_aChannelNodeFileName.SetAtEx(sFileName.Left(36),aFile[l]);	
		}		
	}
	//Channel
	sExt = ResTypeToExt(TP_RES_CHANNEL);
	aFile.RemoveAll();
	File_FindFile(sChannelPath,sExt,aFile);
	m_aChannelFileName.RemoveAll();
	for (int l = 0 ; l < aFile.GetSize(); l++)
	{
		sFileName = PathFindFileName(aFile[l]);
		if(sFileName.Right(4).CompareNoCase(sExt)==0)
		{
			m_aChannelFileName.SetAtEx(sFileName.Left(36),aFile[l]);	
		}		
	}
	//article
	sExt = ResTypeToExt(TP_RES_ARTICLE);
	aFile.RemoveAll();
	File_FindFile(sArticlePath,sExt,aFile);
	m_aArticleFileName.RemoveAll();
	for (int l = 0 ; l < aFile.GetSize(); l++)
	{
		sFileName = PathFindFileName(aFile[l]);
		if(sFileName.Right(4).CompareNoCase(sExt)==0)
		{
			m_aArticleFileName.SetAtEx(sFileName.Left(36),aFile[l]);	
		}		
	}
	return S_OK;
}
CTPMapStringToString *CTPArticleDataBase::GetMap(TP_RES_TYPE eResType)
{
	CTPMapStringToString *p_aMap = NULL;
	if(eResType & TP_RES_CHANNEL)			p_aMap = &m_aChannelFileName;
	else if(eResType & TP_RES_ARTICLE)		p_aMap = &m_aArticleFileName;
	else if(eResType & TP_RES_CHANNELNODE)	p_aMap = &m_aChannelNodeFileName;

	return p_aMap;
}
BOOL CTPArticleDataBase::CreateDefChannelNode(GUID guidRes, GUID guidNode, TCHAR *cName, TP_CHANNEL_NODETYPE eNodeType)
{
	TPChannelNodeData stuChannelNode;
	stuChannelNode.guidRes = guidRes;
	stuChannelNode.guidNode = guidNode;
	stuChannelNode.eNodeType = eNodeType;
	TP_StrCpy(stuChannelNode.cNodeName, cName,MAX_PATH);
	WriteChannelNode(stuChannelNode.guidRes, stuChannelNode);
	return TRUE;
}

BOOL CTPArticleDataBase::IsChannelExist(GUID guidChannelNode, TPChannelBase *pChannelInfo)
{
	CString sCatalog  = TP_UuidToString(&guidChannelNode);
	CString sFileName,sGuid;
	POSITION posGet = NULL;
	m_aChannelFileName.Lock();
	posGet = m_aChannelFileName.GetStartPosition();
	while (posGet)
	{
		m_aChannelFileName.GetNextAssoc(posGet,sGuid,sFileName);
		if(sFileName.Find(sCatalog)>0) 
		{
			TPChannelData stuChannel;
			ReadChannel(sFileName, stuChannel);
			if((TP_StrCmp(pChannelInfo->cChannelTitle, stuChannel.stuChannelBase.cChannelTitle) == 0)
				|| (TP_StrCmp(pChannelInfo->cChannelLink, stuChannel.stuChannelBase.cChannelLink) == 0))
				return TRUE;
		}
	}
	m_aChannelFileName.Unlock();
	return FALSE;
}