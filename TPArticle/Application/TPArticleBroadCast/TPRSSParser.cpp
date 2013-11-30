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
			MSXML2::IXMLDOMNodeListPtr pObjectsImage;
			ChildNode->get_childNodes(&pObjectsImage);
			if(NULL != pObjectsImage)
			{
				LONG childImageNum = 0;
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
		}
		else if(TP_StrCmp(nameString,_T("item")) == 0)
		{
			MSXML2::IXMLDOMNodeListPtr pObjectsItem;
			ChildNode->get_childNodes(&pObjectsItem);
			if(NULL != pObjectsItem)
			{
				LONG childItemNum = 0;
				pObjectsItem->get_length(&childItemNum);
				TPChannelItem *pstuChannelItem = new TPChannelItem;
				for(int j=0; j<childItemNum;j++)
				{
					nameString = NULL;
					ChildNode  = NULL;

					ChildNode = pObjectsItem->Getitem(j);
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
		}
		else
		{
			ASSERT(0);
		}

	}
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
