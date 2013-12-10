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
	LRESULT GetChannelChild(GUID guidRes, TPResDataArray &hChildRes);
	BOOL    IsChannelExist(GUID guidChannelNode, TPChannelBase *pChannelInfo);

	//Article
	LRESULT ReadArticle(GUID guidRes,TPArticleData &stuArticleData);
	LRESULT WriteArticle(GUID guidRes,TPArticleData &stuArticleData);
	
	//base
	CTPMapStringToString *GetMap(TP_RES_TYPE eResType);
	LRESULT InitFileMap();
	CString GetLocalDataPath();
	CString GetResFilePath(GUID guidRes,TP_RES_TYPE eResType,void *pData);
	CString ResTypeToExt(TP_RES_TYPE eResType);
	CString ResTypeToFolderName(TP_RES_TYPE eResType);
	BOOL    CreateDefChannelNode(TCHAR *cName, TP_CHANNEL_NODETYPE eNodeType, GUID guidNode);

};
