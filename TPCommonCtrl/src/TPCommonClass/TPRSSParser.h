#pragma once
#import <msxml4.dll>//MSXML2

// <title>虎嗅网</title>
// <description>虎嗅网RSS订阅</description>
// <link>http://www.huxiu.com</link>
// <generator>huxiu_rss_generator</generator>

//RSS <channel> 参考手册
//元素	描述
//<category>	可选的。为 feed 定义所属的一个或多个种类。
//<cloud>	可选的。注册进程，以获得 feed 更新的立即通知。
//<copyright>	可选。告知版权资料。
//<description>	必需的。描述频道。
//<docs>	可选的。规定指向当前 RSS 文件所用格式说明的 URL。
//<generator>	可选的。指定用于生成 feed 的程序。
//<image>	可选的。在聚合器呈现某个 feed 时，显示一个图像。
//<language>	可选的。规定编写 feed 所用的语言。
//<lastBuildDate>	可选的。定义 feed 内容的最后修改日期。
//<link>	必需的。定义指向频道的超链接。
//<managingEditor>	可选的。定义 feed 内容编辑的电子邮件地址。
//<pubDate>	可选的。为 feed 的内容定义最后发布日期。
//<rating>	可选的。feed 的 PICS 级别。
//<skipDays>	可选的。规定忽略 feed 更新的天。
//<skipHours>	可选的。规定忽略 feed 更新的小时。
//<textInput>	可选的。规定应当与 feed 一同显示的文本输入域。
//<title>	必需的。定义频道的标题。
//<ttl>	可选的。指定从 feed 源更新此 feed 之前，feed 可被缓存的分钟数。
//<webMaster>	可选的。定义此 feed 的 web 管理员的电子邮件地址。

//RSS <item> 参考手册
//元素	描述
//<author>	可选的。规定项目作者的电子邮件地址。
//<category>	可选的。定义项目所属的一个或多个类别。
//<comments>	可选的。允许项目连接到有关此项目的注释（文件）。
//<description>	必需的。描述此项目。
//<enclosure>	可选的。允许将一个媒体文件导入一个项中。
//<guid>	可选的。为项目定义一个唯一的标识符。
//<link>	必需的。定义指向此项目的超链接。
//<pubDate>	可选的。定义此项目的最后发布日期。
//<source>	可选的。为此项目指定一个第三方来源。
//<title>	必需的。定义此项目的标题。
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
	TCHAR			*m_cKeyDiv;
	TCHAR			*m_cItemText;
public:
	int	SetChannelItem(TPChannelItem *pChannelItem, TCHAR *cKeyDiv);
	int GetItemInfo(TCHAR *&cItemText);

private:
	//BOOL ParserChannel(const TCHAR *cFileName);
	//BOOL ParserChannelNode(MSXML2::IXMLDOMNodePtr ChildNode);
	//BOOL ParserImageNode(MSXML2::IXMLDOMNodePtr ChildNode);
	//BOOL ParserItemNode(MSXML2::IXMLDOMNodePtr ChildNode);

	BOOL	ParserHtml(CString &sHtmlStr);
	CString GetHtmlString(const TCHAR *cFileName);
	BOOL	ReleaseItemInfo();

};