#pragma once
#import <msxml4.dll>//MSXML2

// <title>������</title>
// <description>������RSS����</description>
// <link>http://www.huxiu.com</link>
// <generator>huxiu_rss_generator</generator>

class AFX_EXT_CLASS CTPChannelParser
{
public:
	CTPChannelParser(void);
	~CTPChannelParser(void);

	LPTPChannelBase	m_pChannelInfo;
public:
	int	SetChannelAddress(TCHAR *cAddress);
	int GetChannelInfo(TPChannelBase *&pInfo);

private:
	BOOL ParserChannel(const TCHAR *cFileName);
	BOOL ParserChannelNode(MSXML2::IXMLDOMNodePtr ChildNode);
	BOOL ParserImageNode(MSXML2::IXMLDOMNodePtr ChildNode);
	BOOL ParserItemNode(MSXML2::IXMLDOMNodePtr ChildNode);

	BOOL ReleaseChannelInfo();

};
class AFX_EXT_CLASS CTPArticleParser
{
public:
	CTPArticleParser(void);
	~CTPArticleParser(void);

	TPChannelItem 	*m_pChannelItem;
public:
	int	SetItemAddress(TCHAR *cAddress);
	int GetItemInfo(TPChannelItem *&pInfo);

private:
	//BOOL ParserChannel(const TCHAR *cFileName);
	//BOOL ParserChannelNode(MSXML2::IXMLDOMNodePtr ChildNode);
	//BOOL ParserImageNode(MSXML2::IXMLDOMNodePtr ChildNode);
	//BOOL ParserItemNode(MSXML2::IXMLDOMNodePtr ChildNode);

	BOOL ReleaseItemInfo();

};