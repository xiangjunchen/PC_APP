#pragma once

class CTPArticleDataBase
{
public:
	CTPArticleDataBase(void);
	~CTPArticleDataBase(void);

private:
	CString m_sLocalDataPath;

	CTPMapStringToString     m_aChannelFileName;
	CTPMapStringToString     m_aChannelNodeFileName;
	CTPMapStringToString     m_aArticleFileName;

public:
	//ChannelNode
	LRESULT ReadChannelNode(GUID guidRes,TPChannelNodeData &stuChannelNodeData);
	LRESULT WriteChannelNode(GUID guidRes,TPChannelNodeData &stuChannelNodeData);
	LRESULT GetChannelNodeChild(GUID guidRes, TPResDataArray &hChildRes);

	//Channel
	LRESULT ReadChannel(CString  sFileName,TPChannelData &stuChannelData);
	LRESULT ReadChannel(GUID guidRes,TPChannelData &stuChannelData);
	LRESULT WriteChannel(GUID guidRes,TPChannelData &stuChannelData);
	LRESULT DeleteChannel(GUID guidRes);
	LRESULT GetChannelChild(GUID guidRes, TPResDataArray &hChildRes);
	BOOL    IsChannelExist(GUID guidChannelNode, TPChannelBase *pChannelInfo, GUID &guidExist);

	//Article
	LRESULT ReadArticle(CString sFileName,TP_GRADE_TYPE eGradeType, TPArticleData &stuArticleData);
	LRESULT ReadArticle(GUID guidRes,TP_GRADE_TYPE eGradeType, TPArticleData &stuArticleData);
	LRESULT WriteArticle(GUID guidRes,TPArticleData &stuArticleData);
	LRESULT DeleteArticle(GUID guidRes);
	CString GetCurArticleHtmlPath();	
	CString GetArticleResourcePath(GUID guidRes, TCHAR *cPath);
	BOOL    IsArticleExist(GUID guidChannel, TPChannelItem *pChannelItem);
	LRESULT GetArticleChild(GUID guidNode, TPResDataArray &hChildRes);
	//base
	CTPMapStringToString *GetMap(TP_RES_TYPE eResType);
	LRESULT InitFileMap();
	CString GetLocalDataPath();
	CString GetResFilePath(GUID guidRes,TP_RES_TYPE eResType,void *pData);
	CString ResTypeToExt(TP_RES_TYPE eResType);
	CString ResTypeToFolderName(TP_RES_TYPE eResType);
	BOOL    CreateDefChannelNode(GUID guidRes, GUID guidNode, TCHAR *cName, TP_CHANNEL_NODETYPE eNodeType);

};
