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
	LRESULT ReadChannel(GUID guidRes,TPChannelData &stuChannelData);
	LRESULT WriteChannel(GUID guidRes,TPChannelData &stuChannelData);

	CString GetResFilePath(TP_RES_TYPE eResType,void *pData);
	CString ResTypeToExt(TP_RES_TYPE eResType);
	CString ResTypeToFolderName(TP_RES_TYPE eResType);

};
