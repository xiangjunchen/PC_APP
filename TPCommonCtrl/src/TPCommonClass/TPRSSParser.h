#pragma once
#import <msxml4.dll>//MSXML2

// <title>������</title>
// <description>������RSS����</description>
// <link>http://www.huxiu.com</link>
// <generator>huxiu_rss_generator</generator>

//RSS <channel> �ο��ֲ�
//Ԫ��	����
//<category>	��ѡ�ġ�Ϊ feed ����������һ���������ࡣ
//<cloud>	��ѡ�ġ�ע����̣��Ի�� feed ���µ�����֪ͨ��
//<copyright>	��ѡ����֪��Ȩ���ϡ�
//<description>	����ġ�����Ƶ����
//<docs>	��ѡ�ġ��涨ָ��ǰ RSS �ļ����ø�ʽ˵���� URL��
//<generator>	��ѡ�ġ�ָ���������� feed �ĳ���
//<image>	��ѡ�ġ��ھۺ�������ĳ�� feed ʱ����ʾһ��ͼ��
//<language>	��ѡ�ġ��涨��д feed ���õ����ԡ�
//<lastBuildDate>	��ѡ�ġ����� feed ���ݵ�����޸����ڡ�
//<link>	����ġ�����ָ��Ƶ���ĳ����ӡ�
//<managingEditor>	��ѡ�ġ����� feed ���ݱ༭�ĵ����ʼ���ַ��
//<pubDate>	��ѡ�ġ�Ϊ feed �����ݶ�����󷢲����ڡ�
//<rating>	��ѡ�ġ�feed �� PICS ����
//<skipDays>	��ѡ�ġ��涨���� feed ���µ��졣
//<skipHours>	��ѡ�ġ��涨���� feed ���µ�Сʱ��
//<textInput>	��ѡ�ġ��涨Ӧ���� feed һͬ��ʾ���ı�������
//<title>	����ġ�����Ƶ���ı��⡣
//<ttl>	��ѡ�ġ�ָ���� feed Դ���´� feed ֮ǰ��feed �ɱ�����ķ�������
//<webMaster>	��ѡ�ġ������ feed �� web ����Ա�ĵ����ʼ���ַ��

//RSS <item> �ο��ֲ�
//Ԫ��	����
//<author>	��ѡ�ġ��涨��Ŀ���ߵĵ����ʼ���ַ��
//<category>	��ѡ�ġ�������Ŀ������һ���������
//<comments>	��ѡ�ġ�������Ŀ���ӵ��йش���Ŀ��ע�ͣ��ļ�����
//<description>	����ġ���������Ŀ��
//<enclosure>	��ѡ�ġ�����һ��ý���ļ�����һ�����С�
//<guid>	��ѡ�ġ�Ϊ��Ŀ����һ��Ψһ�ı�ʶ����
//<link>	����ġ�����ָ�����Ŀ�ĳ����ӡ�
//<pubDate>	��ѡ�ġ��������Ŀ����󷢲����ڡ�
//<source>	��ѡ�ġ�Ϊ����Ŀָ��һ����������Դ��
//<title>	����ġ��������Ŀ�ı��⡣
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