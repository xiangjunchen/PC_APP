#pragma once

class CTPArticleDataBase
{
public:
	CTPArticleDataBase(void);
	~CTPArticleDataBase(void);

private:
	CString m_sLocalDataPath;
public:
	CString GetLocalDataPath();
	//Channel
	LRESULT ReadChannel(GUID guidRes,TPChannelData &stuChannelData);
	LRESULT WriteChannel(GUID guidRes,TPChannelData &stuChannelData);

	//Article
	LRESULT ReadArticle(GUID guidRes,TPArticleData &stuArticleData);
	LRESULT WriteArticle(GUID guidRes,TPArticleData &stuArticleData);
	
	CString GetResFilePath(TP_RES_TYPE eResType,void *pData);
	CString ResTypeToExt(TP_RES_TYPE eResType);
	CString ResTypeToFolderName(TP_RES_TYPE eResType);

};
