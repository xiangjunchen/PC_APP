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
	CString  sFileName = _T("\\test.Chl");
	CTPMemFile hMemFile;
	hMemFile.ReadFile(sFileName);
	DWORD dwTemp = 0;
	//
	hMemFile.Read(&dwTemp,sizeof(DWORD));	

	//
	hMemFile.Close();
	return S_OK;
}
LRESULT CTPArticleDataBase::WriteChannel(GUID guidRes,TPChannelData &stuChannelData)
{
	CString  sFileName = _T("\\test.Chl");
	CTPMemFile hMemFile;
	DWORD dwTemp = 8;
	//
	hMemFile.Write(&dwTemp,sizeof(DWORD));
	//
	hMemFile.WriteFile(sFileName);
	hMemFile.Close();
	return S_OK;
}
CString CTPArticleDataBase::GetLocalDataPath()
{
	if(!m_sLocalDataPath.IsEmpty())	return m_sLocalDataPath;

	m_sLocalDataPath = TP_GetWindowsSysPath(TP_WINDOWSPATH_PERSONAL);
	m_sLocalDataPath += _T("\\Article\\Data");
	return m_sLocalDataPath;
}
CString   CTPArticleDataBase::GetFileName(CString sName,GUID guidRes,GUID guidNode,TP_RES_TYPE eResType,__int64 lLength,SYSTEMTIME sTime,DWORD dwVideoStrand)
{
	CString sFileName,sGuidRes = TP_UuidToString(&guidRes),sGuidNode = TP_UuidToString(&guidNode) ;
	CTime tm(sTime);	
	TPTimeBase sBase = TP_GetSysTimebaseEx((TP_VIDEO_STANDARD)dwVideoStrand);
	sFileName.Format(_T("%s#%s#%s#%d#%d#%d#%s"),sGuidRes,sGuidNode,sName,(int)tm.GetTime(),(int)lLength,sBase.lTimeRate%100,TP_ResTypeToName(eResType));
	return sFileName;
}
CString CTPArticleDataBase::GetResFilePath(TP_RES_TYPE eResType,void *pData)
{
	if(eResType & TP_RES_CHANNELTYPE)
	{

	}
	else if(eResType & TP_RES_CHANNEL)
	{

	}
	else if(eResType & TP_RES_ARTICLE)
	{

	}
	else if(eResType & TP_RES_COMMENT)
	{

	}
	else
	{
		ASSERT(0);
	}

	//if(eResType & TP_RES_CLIP)
	//{
	//	TPClipData  *pClipData = (TPClipData  *)pData;	
	//	CString sName     = pClipData->sName;		
	//	ASSERT(pClipData->eVideoStandard != TP_VIDEOSTANDARD_UNKNOW);
	//	CString sCatalog  =  m_sResClipPath;	
	//	CString sFileName =  _L("");	
	//	sFileName = sCatalog + _L("\\");
	//	sFileName += GetFileName(sName,pClipData->guidRes,pClipData->guidNode,pClipData->eResType,pClipData->lClipLength,pClipData->tmCreate,pClipData->eVideoStandard);		
	//	return sFileName;
	//}
	//else if(eResType & TP_RES_CATALOG)
	//{
}