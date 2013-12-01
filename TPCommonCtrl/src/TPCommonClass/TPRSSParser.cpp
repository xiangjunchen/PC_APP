#include "StdAfx.h"

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

	int nLen = TP_StrLen(cAddress);
	if(nLen <= 0)	return 0;

	m_pChannelInfo = new TPChannelBase;
	TP_StrCpy(m_pChannelInfo->cChannelAddress, cAddress, nLen);
	return 1;
}
int CTPChannelParser::GetChannelInfo(TPChannelBase *pInfo)
{
	if(!m_pChannelInfo || TP_StrLen(m_pChannelInfo->cChannelAddress) <= 0)	return 0;

	TCHAR cXMLPath[MAX_PATH] ;
	CTPDownloadHttp stuDownload;
	stuDownload.SetHttpUrl(m_pChannelInfo->cChannelAddress, cXMLPath);
	stuDownload.Download();

	ParserChannel(cXMLPath);
	return 1;
}
BOOL CTPChannelParser::ParserChannel(const TCHAR *cFileName)
{
	if(TP_StrLen(cFileName) <= 0)	{ASSERT(0) ;return FALSE;}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMDocumentPtr pDoc2;
	hr=pDoc2.CreateInstance(__uuidof(MSXML2::DOMDocument40));
	pDoc2 -> load(cFileName); 

	_bstr_t bNodeText   = _T("");
	LONG childNodeNum	= 0;
	BSTR nameString		= NULL;
	MSXML2::IXMLDOMNodePtr ChildNode = NULL;

	//Objects
	MSXML2::IXMLDOMNodeListPtr pObjectNodeList;
	pDoc2->get_childNodes(&pObjectNodeList);
	pObjectNodeList->get_length(&childNodeNum);
	if(childNodeNum <= 0)			return FALSE;

	for(int j=0; j<childNodeNum;j++)
	{
		nameString = NULL;
		ChildNode  = NULL;

		MSXML2::IXMLDOMNodePtr ChildNode;
		ChildNode = pObjectNodeList->Getitem(j);
		ChildNode->get_baseName(&nameString);
		if(TP_StrCmp(nameString, _T("rss"))==0)
		{
			MSXML2::IXMLDOMNodeListPtr pRSSNodeList;
			ChildNode->get_childNodes(&pRSSNodeList);
			if(NULL != pRSSNodeList)	
			{
				childNodeNum = 0;
				pRSSNodeList->get_length(&childNodeNum);
				for(int j=0; j<childNodeNum;j++)
				{
					nameString = NULL;
					MSXML2::IXMLDOMNodePtr ChildNode;
					ChildNode = pRSSNodeList->Getitem(j);
					ChildNode->get_baseName(&nameString);
					if(TP_StrCmp(nameString,_T("channel"))==0)
					{
						ParserChannelNode(ChildNode);
					}
					else
					{
						ASSERT(0);
					}
				}
			}
		}
		else if(TP_StrCmp(nameString, _T("xml"))==0)
		{
			//ASSERT(0);
		}
		else
		{
			ASSERT(0);
		}
	}
	return TRUE;
}

BOOL CTPChannelParser::ParserChannelNode(MSXML2::IXMLDOMNodePtr ChildNode)
{
	if(NULL == ChildNode)	{ASSERT(0) ;return FALSE;}

	_bstr_t bNodeText  = _T("");
	size_t	nLength    = 0;
	BSTR	nameString = NULL;
	LONG childNodeNum = 0;
	MSXML2::IXMLDOMNodeListPtr pChannelNodeList;
	ChildNode->get_childNodes(&pChannelNodeList);
	if(NULL != pChannelNodeList)	
	{
		childNodeNum = 0;
		pChannelNodeList->get_length(&childNodeNum);
		for(int j=0; j<childNodeNum;j++)
		{
			nameString = NULL;
			ChildNode  = NULL;

			ChildNode = pChannelNodeList->Getitem(j);
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
				TP_StrCpy(m_pChannelInfo->cChannelLink, bNodeText, nLength);
			}
			else if(TP_StrCmp(nameString,_T("generator")) == 0)
			{
				TP_StrCpy(m_pChannelInfo->cChannelGenerator, bNodeText, nLength);
			}
			else if(TP_StrCmp(nameString,_T("image")) == 0)
			{
				ParserImageNode(ChildNode);
			}
			else if(TP_StrCmp(nameString,_T("item")) == 0)
			{
				ParserItemNode(ChildNode);
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	return TRUE;
}
BOOL CTPChannelParser::ParserImageNode(MSXML2::IXMLDOMNodePtr ChildNode)
{
	if(NULL == ChildNode)	{ASSERT(0) ;return FALSE;}

	_bstr_t bNodeText  = _T("");
	size_t	nLength    = 0;
	BSTR	nameString = NULL;
	LONG childImageNum = 0;
	MSXML2::IXMLDOMNodeListPtr pObjectsImage;
	ChildNode->get_childNodes(&pObjectsImage);
	if(NULL != pObjectsImage)
	{
		pObjectsImage->get_length(&childImageNum);
		for(int j=0; j<childImageNum;j++)
		{
			nameString = NULL;
			ChildNode  = NULL;

			ChildNode = pObjectsImage->Getitem(j);
			ChildNode->get_baseName(&nameString);
			bNodeText = ChildNode->Gettext();
			nLength	  = bNodeText.length();
			if(TP_StrCmp(nameString, _T("url"))==0)
			{
				TP_StrCpy(m_pChannelInfo->stuChannelImage.cImageUrl, bNodeText, nLength);
			}
			else if(TP_StrCmp(nameString, _T("title"))==0)
			{
				TP_StrCpy(m_pChannelInfo->stuChannelImage.cImageTitle, bNodeText, nLength);
			}
			else if(TP_StrCmp(nameString, _T("link"))==0)
			{
				TP_StrCpy(m_pChannelInfo->stuChannelImage.cImageLink, bNodeText, nLength);
			}
			else
			{
				ASSERT(0);
			}
		}
	}	
	return TRUE;
}
BOOL CTPChannelParser::ParserItemNode(MSXML2::IXMLDOMNodePtr ChildNode)
{
	if(NULL == ChildNode)	{ASSERT(0) ;return FALSE;}

	_bstr_t bNodeText  = _T("");
	size_t	nLength    = 0;
	BSTR	nameString = NULL;
	LONG childItemNum  = 0;

	MSXML2::IXMLDOMNodeListPtr pItemNodeList;
	ChildNode->get_childNodes(&pItemNodeList);
	if(NULL != pItemNodeList)
	{
		pItemNodeList->get_length(&childItemNum);
		TPChannelItem *pstuChannelItem = new TPChannelItem;
		for(int j=0; j<childItemNum;j++)
		{
			nameString = NULL;
			ChildNode  = NULL;

			ChildNode = pItemNodeList->Getitem(j);
			ChildNode->get_baseName(&nameString);
			bNodeText = ChildNode->Gettext();
			nLength	  = bNodeText.length();
			if(TP_StrCmp(nameString, _T("title"))==0)
			{
				TP_StrCpy(pstuChannelItem->cItemTitle, bNodeText, nLength);
			}
			else if(TP_StrCmp(nameString, _T("link"))==0)
			{
				TP_StrCpy(pstuChannelItem->cItemLink, bNodeText, nLength);
			}
			else if(TP_StrCmp(nameString, _T("description"))==0)
			{
				TP_StrCpy(pstuChannelItem->cItemDescription, bNodeText, nLength);
			}
			else if(TP_StrCmp(nameString, _T("pubDate"))==0)
			{
				TP_StrCpy(pstuChannelItem->cItemPubDate, bNodeText, nLength);
			}
			else
			{
				ASSERT(0);
			}
		}
		m_pChannelInfo->aChannelItem.Add(pstuChannelItem);
	}	
	return TRUE;
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
