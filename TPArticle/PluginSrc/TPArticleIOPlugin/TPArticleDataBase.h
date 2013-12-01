#pragma once

class CTPArticleDataBase
{
public:
	CTPArticleDataBase(void);
	~CTPArticleDataBase(void);

public:
	LRESULT ReadChannel(GUID guidRes,TPChannelData &stuChannelData);
	LRESULT WriteChannel(GUID guidRes,TPChannelData &stuChannelData);
};
