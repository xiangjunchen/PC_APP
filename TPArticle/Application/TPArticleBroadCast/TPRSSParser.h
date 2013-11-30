#pragma once

// <title>������</title>
// <description>������RSS����</description>
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
	TCHAR	*cChannelLink;			//��ַ��ҳ
	TCHAR	*cChannelAddress;       //rss��ַ
	TCHAR	*cChannelGenerator;     //��ʼ��
	int		lUpdateInterval;		//ˢ�¼��ʱ�䣨��λ�Ƿ֣�
	int		lSaveNum;				//������Ŀ��
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
