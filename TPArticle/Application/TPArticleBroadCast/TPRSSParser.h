#pragma once

// <title>虎嗅网</title>
// <description>虎嗅网RSS订阅</description>
// <link>http://www.huxiu.com</link>
// <generator>huxiu_rss_generator</generator>
typedef struct _tagTPChannelItem
{
	_tagTPChannelItem()
	{

	}
	~_tagTPChannelItem()
	{

	}

}TPChannelItem;
typedef CArray<TPChannelItem *, TPChannelItem *&>	CTPChannelItemArray;

typedef struct _tagTPChannelInfo
{
	TCHAR	*cChannelTitle;
	TCHAR	*cChannelDescription;
	TCHAR	*cChannelLink;			//网址主页
	TCHAR	*cChannelAddress;       //rss地址
	TCHAR	*cChannelGenerator;     //创始者
	int		lUpdateInterval;		//刷新间隔时间（单位是分）
	int		lSaveNum;				//保存条目数
	CTPChannelItemArray	aChannelItem;
	_tagTPChannelInfo()
	{
		cChannelTitle = NULL;
		cChannelDescription = NULL;
		cChannelLink  = NULL;
		cChannelAddress = NULL;
		cChannelGenerator = NULL;
		lUpdateInterval = 60;  
		lSaveNum		= 500;
		aChannelItem.RemoveAll();
	}
	~_tagTPChannelInfo()
	{
		if(cChannelTitle)     {delete cChannelTitle ; cChannelTitle = NULL;}
		if(cChannelDescription)     {delete cChannelDescription ; cChannelDescription = NULL;}
		if(cChannelLink)     {delete cChannelLink ; cChannelLink = NULL;}
		if(cChannelAddress)     {delete cChannelAddress ; cChannelAddress = NULL;}
		if(cChannelGenerator)     {delete cChannelGenerator ; cChannelGenerator = NULL;}
	}
}TPChannelInfo, *LPTPChannelInfo;
typedef CArray<TPChannelInfo *, TPChannelInfo*&> CTPChannelInfo;

class CTPChannelParser
{
public:
	CTPChannelParser(void);
	~CTPChannelParser(void);

	LPTPChannelInfo	m_pChannelInfo;
public:
	int	SetChannelAddress(TCHAR *cAddress);
	int GetChannelInfo(TPChannelInfo *pInfo);

private:
	BOOL ParserChannel(const TCHAR *cFileName);
	BOOL ReleaseChannelInfo();

};
