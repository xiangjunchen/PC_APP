#include "StdAfx.h"

CTPArticleDataBase::CTPArticleDataBase(void)
{
	m_sLocalDataPath = _T("");
}

CTPArticleDataBase::~CTPArticleDataBase(void)
{
}
LRESULT CTPArticleDataBase::ReadChannel(GUID guidRes,TPChannelData &stuChannelData)
{
	DWORD dwSize = 0;
	CString  sFileName = GetResFilePath(TP_RES_CHANNEL, &stuChannelData);
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

	hMemFile.Write(&dwSize,sizeof(int));			
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
	CString  sFileName = GetResFilePath(TP_RES_CHANNEL, &stuChannelData);
	CTPMemFile hMemFile;
	//
	dwSize = TP_StrLen(stuChannelData.stuChannelBase.cChannelAddress) * sizeof(TCHAR) + 2;
	hMemFile.Write(&dwSize,sizeof(DWORD));
	hMemFile.Write(stuChannelData.stuChannelBase.cChannelAddress,dwSize);

	hMemFile.SeekToBegin();
	stuChannelData.Release();
	hMemFile.Read(&dwSize,sizeof(DWORD));
	TCHAR cChar[10];// = new TCHAR;
	stuChannelData.stuChannelBase.cChannelAddress = new TCHAR[dwSize/sizeof(TCHAR)];
	ZeroMemory(stuChannelData.stuChannelBase.cChannelAddress,dwSize);
	hMemFile.Read(stuChannelData.stuChannelBase.cChannelAddress,dwSize);

	hMemFile.WriteFile(sFileName);
	hMemFile.Close();
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
CString CTPArticleDataBase::GetResFilePath(TP_RES_TYPE eResType,void *pData)
{
	CString sFileName = GetLocalDataPath();
	if(!pData)	{ASSERT(0);return sFileName;}

	CString sGuidRes,sGuidNode,sName ;
	if(eResType & TP_RES_CHANNELNODE)
	{
		TPChannelNode *pNode = (TPChannelNode *)pData;
		sGuidRes = TP_UuidToString(&pNode->guidRes);
		sGuidNode = TP_UuidToString(&pNode->guidNode) ;
		sName	= pNode->cNodeName;
		sFileName.Format(_T("%s\\%s\\%s#%s#%s%s"),GetLocalDataPath(),ResTypeToFolderName(eResType),sGuidRes,sGuidNode,sName,ResTypeToExt(eResType));
	}
	else if(eResType & TP_RES_CHANNEL)
	{
		TPChannelData *pChannelData = (TPChannelData *)pData;
		sGuidRes = TP_UuidToString(&pChannelData->guidRes);
		sGuidNode = TP_UuidToString(&pChannelData->guidNode) ;
		sName	= pChannelData->stuChannelBase.cChannelTitle;
		sFileName.Format(_T("%s\\%s\\%s#%s#%s%s"),GetLocalDataPath(),ResTypeToFolderName(eResType),sGuidRes,sGuidNode,sName,ResTypeToExt(eResType));

	}
	else if(eResType & TP_RES_ARTICLE)
	{
		TPArticleData *pArticleData = (TPArticleData *)pData;
		sGuidRes = TP_UuidToString(&pArticleData->guidRes);
		sGuidNode = TP_UuidToString(&pArticleData->guidNode) ;
		sName	= pArticleData->sName;
		sFileName.Format(_T("%s\\%s\\%s#%s#%s%s"),GetLocalDataPath(),ResTypeToFolderName(eResType),sGuidRes,sGuidNode,sName,ResTypeToExt(eResType));

	}
	else if(eResType & TP_RES_COMMENT)
	{
		TPCommentData *pCommentData = (TPCommentData *)pData;
	}
	else
	{
		ASSERT(0);
	}
	return sFileName;
}