// TPArticleMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TPArticleBroadCast.h"
#include "TPArticleMainDlg.h"


HMODULE g_hArticleManage = NULL;  
TPArticleManageProcess   g_stuPress;
TPArticleManageInterface g_stuArticleInterface;
LRESULT TP_InitArticleCenter()
{
	CString sFile   = _T("TPArticleManage");
#ifdef _DEBUG
	g_hArticleManage = ::LoadLibrary((CString)sFile + _T("ud.dll"));
#else
	g_hArticleManage = ::LoadLibrary((CString)sFile + _T("u.dll"));
#endif
	if(g_hArticleManage == NULL){ASSERT(0); return S_FALSE;}

	void (*TP_GetManageProcess)(TPArticleManageProcess * ) = NULL;
	TP_GetManageProcess = (void (* )(TPArticleManageProcess *)) ::GetProcAddress(g_hArticleManage,"TP_GetManageProcess");
	if(TP_GetManageProcess == NULL)
	{
		ASSERT(0);
		::FreeLibrary(g_hArticleManage);
		g_hArticleManage = NULL;
		return S_FALSE;
	}
	TP_GetManageProcess(&g_stuPress);	
	g_stuPress.TP_GetManageFunction(&g_stuArticleInterface);
	g_stuPress.TP_InitData(NULL);
	return S_OK;
}
LRESULT TP_ReleaseArticleCenter()
{
	if(g_stuPress.TP_ReleaseData)
		g_stuPress.TP_ReleaseData(NULL);
	if(g_hArticleManage)
	{
		::FreeLibrary(g_hArticleManage);
		g_hArticleManage = NULL;
		return S_OK;
	}
	return S_FALSE;
}

// CTPArticleMainDlg dialog

IMPLEMENT_DYNAMIC(CTPArticleMainDlg, CTPDialog)

CTPArticleMainDlg::CTPArticleMainDlg(CWnd* pParent /*=NULL*/)
	: CTPDialog(CTPArticleMainDlg::IDD, pParent)
{
	m_pChannelList = NULL;
}

CTPArticleMainDlg::~CTPArticleMainDlg()
{
}

void CTPArticleMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CTPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTPArticleMainDlg, CTPDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ADDCHANNEL, &CTPArticleMainDlg::OnBnClickedButtonAddchannel)
END_MESSAGE_MAP()


// CTPArticleMainDlg message handlers

BOOL CTPArticleMainDlg::OnInitDialog()
{
	CTPDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_pChannelList = (CTPComboBox*)GetDlgItem(IDC_COMBO_CHANNELLIST);

	TP_InitArticleCenter();

	GUID guidNode = GUID_NULL;
	TPResDataArray aResData;
	g_stuArticleInterface.stuChannelNodeInterface.TP_GetChildRes(guidNode, aResData);
	for (int l = 0 ; l < aResData.GetSize(); l++)
	{
		TPChannelData stuChannel;
		g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(aResData[l].guidRes,stuChannel);

		m_pChannelList->AddString(stuChannel.stuChannelBase.cChannelTitle);	
	}
	
	////////////////////////////////////////////////////////////////////////////rss test

	//DWORD dwTick = GetTickCount();
	//TCHAR cAddress[] = _T("http://www.huxiu.com/rss/0.xml");//;//_T("http://www.36kr.com/feed")
	////TCHAR cAddress[] = _T("http://www.36kr.com/feed");//;//
	//TCHAR cKeyDiv [] = _T("<div class=\"neirong-box\" id=\"neirong_box\">");
	////TCHAR cKeyDiv [] = _T("<div class=\"mainContent sep-10\">");
	//TPChannelBase *pChannelInfo = NULL;
	//CTPChannelParser stuChannelParser;
	//stuChannelParser.SetChannelAddress(cAddress);
	//stuChannelParser.GetChannelInfo(pChannelInfo);

	//TPChannelData stuChannel;
	//CoCreateGuid(&stuChannel.guidRes);
	//stuChannel.eNodeType = TP_CHANNEL_TECH|TP_CHANNEL_SYSTEM;
	//stuChannel.stuChannelBase = *pChannelInfo;
	//stuChannel.AppendUpdateItem();
	//g_stuArticleInterface.stuChannelInterface.TP_SetChannelInfo(stuChannel.guidRes,stuChannel);
	//g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(stuChannel.guidRes,stuChannel);

	//dwTick = GetTickCount() - dwTick;
	//CString sTick = _T("");
	//sTick.Format(_T("%d ms"), dwTick);
	//AfxMessageBox(sTick);
	//dwTick = GetTickCount();

	//for (int l = 0 ; l < stuChannel.aChannelItemAll.GetSize(); l++)
	//{
	//	TCHAR *cItemText = NULL;
	//	TPChannelItem *pItemInfo = NULL;
	//	CTPArticleParser stuArticleParser;
	//	stuArticleParser.SetChannelItem(stuChannel.aChannelItemAll[l], cKeyDiv);
	//	stuArticleParser.GetItemInfo(cItemText);

	//	TPArticleData stuArticle;
	//	CoCreateGuid(&stuArticle.guidRes);
	//	stuArticle.stuChannelItem = *stuChannel.aChannelItemAll[l];
	//	TP_StrCpy(stuArticle.cText, cItemText, TP_StrLen(cItemText));
	//	g_stuArticleInterface.stuArticleInterfce.TP_SetArticleInfo(stuArticle.guidRes,TP_GRADE_ALL,stuArticle);
	//	g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(stuArticle.guidRes,TP_GRADE_ALL,stuArticle);
	//}
	//dwTick = GetTickCount() - dwTick;
	//sTick = _T("");
	//sTick.Format(_T("%d ms"), dwTick);
	//AfxMessageBox(sTick);


	//////////////////////////////////////////////////////////////////////////ctrl test
	// 	CTPImgPreviewWnd *m_pImgPreWnd = new CTPImgPreviewWnd;
	// 	m_pImgPreWnd->Create(0,pFrame,0);
	// 
	// 	if(m_pImgPreWnd)
	// 	{
	// 		m_pImgPreWnd->SetLayeredWindowAttributes(0,150,2);
	// 		m_pImgPreWnd->ShowWindow(SW_SHOW);
	// 		m_pImgPreWnd->PreviewImg();		
	// 	}


	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTPArticleMainDlg::OnDestroy()
{
	CTPDialog::OnDestroy();

	TP_ReleaseArticleCenter();

	// TODO: Add your message handler code here
}

void CTPArticleMainDlg::OnBnClickedButtonAddchannel()
{
	CString sChannelUrl = _T(""), sChannelKeyDiv = _T("");
	GetDlgItem(IDC_EDIT_CHANNELURL)->GetWindowText(sChannelUrl);
	GetDlgItem(IDC_EDIT_CHANNELKEYDIV)->GetWindowText(sChannelKeyDiv);
	sChannelUrl = sChannelUrl.Trim();
	sChannelKeyDiv = sChannelKeyDiv.Trim();
	if(sChannelUrl.IsEmpty() || sChannelKeyDiv.IsEmpty())
	{
		AfxMessageBox(_T("Add Failed!"));
		return ;
	}

	TCHAR *cAddress = NULL;
	TP_StrCpy(cAddress, sChannelUrl.GetBuffer(), sChannelUrl.GetLength());//_T("http://www.huxiu.com/rss/0.xml");//;//_T("http://www.36kr.com/feed")
	//TCHAR cAddress[] = _T("http://www.36kr.com/feed");//;//
	TCHAR *cKeyDiv = NULL;
	TP_StrCpy(cKeyDiv, sChannelKeyDiv.GetBuffer(), sChannelKeyDiv.GetLength());
	//TCHAR cKeyDiv [] = sChannelKeyDiv.GetBuffer();//_T("<div class=\"neirong-box\" id=\"neirong_box\">");
	//TCHAR cKeyDiv [] = _T("<div class=\"mainContent sep-10\">");
	TPChannelBase *pChannelInfo = NULL;
	CTPChannelParser stuChannelParser;
	stuChannelParser.SetChannelAddress(cAddress);
	stuChannelParser.GetChannelInfo(pChannelInfo);

	TPChannelData stuChannel;
	CoCreateGuid(&stuChannel.guidRes);
	stuChannel.eNodeType = TP_CHANNEL_TECH|TP_CHANNEL_SYSTEM;
	stuChannel.stuChannelBase = *pChannelInfo;
	stuChannel.AppendUpdateItem();
	g_stuArticleInterface.stuChannelInterface.TP_SetChannelInfo(stuChannel.guidRes,stuChannel);
	g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(stuChannel.guidRes,stuChannel);

	if(cAddress)	{delete cAddress; cAddress = NULL;}
	if(cKeyDiv)		{delete cKeyDiv; cKeyDiv = NULL;}
}
