#pragma once
#import <msxml4.dll>//MSXML2

// <title>»¢ÐáÍø</title>
// <description>»¢ÐáÍøRSS¶©ÔÄ</description>
// <link>http://www.huxiu.com</link>
// <generator>huxiu_rss_generator</generator>

class AFX_EXT_CLASS CTPChannelParser
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
