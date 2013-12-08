#pragma once

class CTPArticleDataBase
{
public:
	CTPArticleDataBase(void);
	~CTPArticleDataBase(void);

private:
	CString m_sLocalDataPath;

	CTPMapStringToString     m_aClipFileName;
	CTPMapStringToString     m_aCatalogFileName;
	CTPMapStringToString     m_aHideClipFileName;
	CTPMapStringToString     m_aProgramFileName;
	CTPMapStringToString     m_aTemplateFileName;	

public:
	CString GetLocalDataPath();
	//ChannelNode
	LRESULT GetChannelRes(GUID guidRes, TPResDataArray &hChildRes);
	//Channel
	LRESULT ReadChannel(GUID guidRes,TPChannelData &stuChannelData);
	LRESULT WriteChannel(GUID guidRes,TPChannelData &stuChannelData);

	//Article
	LRESULT ReadArticle(GUID guidRes,TPArticleData &stuArticleData);
	LRESULT WriteArticle(GUID guidRes,TPArticleData &stuArticleData);
	
	CString GetResFilePath(GUID guidRes,TP_RES_TYPE eResType,void *pData);
	CString ResTypeToExt(TP_RES_TYPE eResType);
	CString ResTypeToFolderName(TP_RES_TYPE eResType);

};
