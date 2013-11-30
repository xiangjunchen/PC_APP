#pragma once

// <title>虎嗅网</title>
// <description>虎嗅网RSS订阅</description>
// <link>http://www.huxiu.com</link>
// <generator>huxiu_rss_generator</generator>
typedef struct _tagTPChannelItem
{
	TCHAR	*cItemTitle;
	TCHAR	*cItemLink;
	TCHAR	*cItemDescription;
	TCHAR	*cItemPubDate;
	_tagTPChannelItem()
	{
		Reset();
	}
	~_tagTPChannelItem()
	{
		if(cItemTitle)  {delete cItemTitle ; cItemTitle = NULL;}
		if(cItemLink)  {delete cItemLink ; cItemLink = NULL;}
		if(cItemDescription)  {delete cItemDescription ; cItemDescription = NULL;}
		if(cItemPubDate)  {delete cItemPubDate ; cItemPubDate = NULL;}
	}
	void Reset()
	{
		cItemTitle = NULL;
		cItemLink = NULL;
		cItemDescription = NULL;
		cItemPubDate = NULL;
	}

}TPChannelItem;
typedef CArray<TPChannelItem *, TPChannelItem *&>	CTPChannelItemArray;

typedef struct _tagTPChannelImage
{
	TCHAR	*cImageUrl;
	TCHAR	*cImageTitle;
	TCHAR	*cImageLink;
	_tagTPChannelImage()
	{
		Reset();
	}
	~_tagTPChannelImage()
	{
		if(cImageUrl)  {delete cImageUrl ; cImageUrl = NULL;}
		if(cImageTitle)  {delete cImageTitle ; cImageTitle = NULL;}
		if(cImageLink)  {delete cImageLink ; cImageLink = NULL;}
	}
	void Reset()
	{
		cImageUrl	= NULL;
		cImageTitle = NULL;
		cImageLink	= NULL;		
	}
}TPChannelImage;
typedef struct _tagTPChannelInfo
{
	TCHAR	*cChannelAddress;       //rss地址
	int		lUpdateInterval;		//刷新间隔时间（单位是分）
	int		lSaveNum;				//保存条目数
	//
	TCHAR	*cChannelTitle;
	TCHAR	*cChannelDescription;
	TCHAR	*cChannelLink;			//网址主页
	TCHAR	*cChannelGenerator;     //创始者
	TPChannelImage		stuChannelImage;
	CTPChannelItemArray	aChannelItem;
	//
	_tagTPChannelInfo()
	{
		cChannelTitle = NULL;
		cChannelDescription = NULL;
		cChannelLink  = NULL;
		cChannelAddress = NULL;
		cChannelGenerator = NULL;
		lUpdateInterval = 60;  
		lSaveNum		= 500;
		stuChannelImage.Reset();
		aChannelItem.RemoveAll();
	}
	~_tagTPChannelInfo()
	{
		if(cChannelTitle)     {delete cChannelTitle ; cChannelTitle = NULL;}
		if(cChannelDescription)     {delete cChannelDescription ; cChannelDescription = NULL;}
		if(cChannelLink)     {delete cChannelLink ; cChannelLink = NULL;}
		if(cChannelAddress)     {delete cChannelAddress ; cChannelAddress = NULL;}
		if(cChannelGenerator)     {delete cChannelGenerator ; cChannelGenerator = NULL;}
		for (int l=0;l<aChannelItem.GetSize();l++)	
		{
			delete aChannelItem[l];
			aChannelItem[l] = NULL;
		}
		aChannelItem.RemoveAll();		
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
	BOOL ParserChannelNode(MSXML2::IXMLDOMNodePtr ChildNode);
	BOOL ParserImageNode(MSXML2::IXMLDOMNodePtr ChildNode);
	BOOL ParserItemNode(MSXML2::IXMLDOMNodePtr ChildNode);

	BOOL ReleaseChannelInfo();

};
