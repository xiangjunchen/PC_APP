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
};
