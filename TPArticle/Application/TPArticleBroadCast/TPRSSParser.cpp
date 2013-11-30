#include "StdAfx.h"
#include "TPRSSParser.h"
#include "TPDownloadHttp.h"
#import <msxml4.dll>//MSXML2

CTPChannelParser::CTPChannelParser(void)
{
	m_pChannelInfo = NULL;
}

CTPChannelParser::~CTPChannelParser(void)
{
	ReleaseChannelInfo();
}
int	CTPChannelParser::SetChannelAddress(TCHAR *cAddress)
{
	ReleaseChannelInfo();

	int nLen = lstrlen(cAddress);
	if(nLen <= 0)	return 0;

	m_pChannelInfo = new TPChannelInfo;
	TP_StrCpy(m_pChannelInfo->cChannelAddress, cAddress, nLen);
	return 1;
}
void TP_ParseNode(MSXML2::IXMLDOMNodeListPtr pChildNodeList)
{
	long childNodeNum = 0;
	pChildNodeList->get_length(&childNodeNum);
	for(int j=0; j<childNodeNum;j++)
	{
		MSXML2::IXMLDOMNodePtr ChildNode;
		ChildNode = pChildNodeList->Getitem(j);
		BSTR nameString = NULL;
		ChildNode->get_baseName(&nameString);
		CString strNameString(nameString);
		::SysFreeString(nameString);
		strNameString.TrimLeft();
		strNameString.TrimRight();

		if(strNameString == _T("title"))
		{
			//TP_StrCpy(m_pChannelInfo->cChannelTitle, str)
			ChildNode->get_childNodes(&pChildNodeList);
			break;
		}
	}	
}
BOOL CTPChannelParser::ParserChannel(const TCHAR *cFileName)
{
	HRESULT hr = S_OK;
	MSXML2::IXMLDOMDocumentPtr pDoc2;
	hr=pDoc2.CreateInstance(__uuidof(MSXML2::DOMDocument40));

	pDoc2 -> load(cFileName); 

	_bstr_t bNodeText;
	size_t	nLength;
	BSTR nameString = NULL;
	MSXML2::IXMLDOMNodePtr ChildNode;

	MSXML2::IXMLDOMNodeListPtr pChildNodeList;
	pDoc2->get_childNodes(&pChildNodeList);
	LONG childNodeNum = 0;
	pChildNodeList->get_length(&childNodeNum);
	if(childNodeNum <= 0)			return FALSE;

// 	ASSERT(childNodeNum == 1);
	//Objects
	MSXML2::IXMLDOMNodeListPtr pObjectsList;
	for(int j=0; j<childNodeNum;j++)
	{
		MSXML2::IXMLDOMNodePtr ChildNode;
		ChildNode = pChildNodeList->Getitem(j);
		BSTR nameString = NULL;
		ChildNode->get_baseName(&nameString);
		CString strNameString(nameString);
		::SysFreeString(nameString);
		strNameString.TrimLeft();
		strNameString.TrimRight();

		if(strNameString == _T("rss"))
		{
			ChildNode->get_childNodes(&pObjectsList);
			break;
		}
// 		else
// 			ASSERT(0);
	}
	if(NULL == pObjectsList)	
		return FALSE;	

	childNodeNum = 0;
	pObjectsList->get_length(&childNodeNum);
	for(int j=0; j<childNodeNum;j++)
	{
		MSXML2::IXMLDOMNodePtr ChildNode;
		ChildNode = pObjectsList->Getitem(j);
		BSTR nameString = NULL;
		ChildNode->get_baseName(&nameString);
		CString strNameString(nameString);
		::SysFreeString(nameString);
		strNameString.TrimLeft();
		strNameString.TrimRight();

		if(strNameString == _T("channel"))
		{
			ChildNode->get_childNodes(&pObjectsList);
			break;
		}
	}

	if(NULL == pObjectsList)	
		return FALSE;	

	childNodeNum = 0;
	pObjectsList->get_length(&childNodeNum);
	for(int j=0; j<childNodeNum;j++)
	{
		nameString = NULL;
		ChildNode  = NULL;

		ChildNode = pObjectsList->Getitem(j);
		ChildNode->get_baseName(&nameString);
		bNodeText = ChildNode->Gettext();
		nLength	  = bNodeText.length();
		if(TP_StrCmp(nameString, _T("title"))==0)
		{
			TP_StrCpy(m_pChannelInfo->cChannelTitle, bNodeText, nLength);
		}
		else if(TP_StrCmp(nameString,_T("description")) == 0)
		{
			TP_StrCpy(m_pChannelInfo->cChannelDescription, bNodeText, nLength);
		}
		else if(TP_StrCmp(nameString,_T("link")) == 0)
		{
			TP_StrCpy(m_pChannelInfo->cChannelTitle, bNodeText, nLength);
		}
		else if(TP_StrCmp(nameString,_T("generator")) == 0)
		{
			TP_StrCpy(m_pChannelInfo->cChannelGenerator, bNodeText, nLength);
		}
		else if(TP_StrCmp(nameString,_T("image")) == 0)
		{
		//	TP_StrCpy(m_pChannelInfo->cChannelGenerator, bNodeText, nLength);
		}
		

	}	
// 	<title>»¢ÐáÍø</title>
// 		<description>»¢ÐáÍøRSS¶©ÔÄ</description>
// 		<link>http://www.huxiu.com</link>
// 	<generator>huxiu_rss_generator</generator>
// 		<image>...</image>
// 		<item>...</item>
	return TRUE;
}

int CTPChannelParser::GetChannelInfo(TPChannelInfo *pInfo)
{
	if(!m_pChannelInfo || lstrlen(m_pChannelInfo->cChannelAddress) <= 0)	return 0;

	TCHAR cXMLPath[MAX_PATH] ;
	CTPDownloadHttp stuDownload;
	stuDownload.SetHttpUrl(m_pChannelInfo->cChannelAddress, cXMLPath);
	stuDownload.Download();

	ParserChannel(cXMLPath);
	return 1;
}
BOOL CTPChannelParser::ReleaseChannelInfo()
{
	if(m_pChannelInfo)
	{
		delete m_pChannelInfo;
		m_pChannelInfo = NULL;
	}
	return TRUE;
}
