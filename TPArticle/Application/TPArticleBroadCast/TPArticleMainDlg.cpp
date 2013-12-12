// TPArticleMainDlg.cpp : implementation file
//

#include "stdafx.h"



// CTPArticleMainDlg dialog

IMPLEMENT_DYNAMIC(CTPArticleMainDlg, CTPDialog)

CTPArticleMainDlg::CTPArticleMainDlg(CWnd* pParent /*=NULL*/)
	: CTPDialog(CTPArticleMainDlg::IDD, pParent)
{
	m_pChannelNodeListPublic = NULL;
	m_pChannelListPublic = NULL;
	m_pChannelList = NULL;
	m_pArticleList = NULL;
	m_pHtmlCtrl    = NULL;
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
	ON_CBN_SELENDOK(IDC_COMBO_CHANNELLIST, &CTPArticleMainDlg::OnCbnSelChannelList)
	
	ON_CBN_SELENDOK(IDC_COMBO_CHANNELNODELISTPUBLIC, &CTPArticleMainDlg::OnCbnSelChannelNodeList)
	ON_CBN_SELENDOK(IDC_LIST_ARTICLE, &CTPArticleMainDlg::OnCbnSelArticleList)
// 	ON_NOTIFY(NM_CLICK,IDC_LIST_ARTICLE,OnNMClick)
// 	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_BN_CLICKED(IDC_BUTTON_ADDCHANNEL2, &CTPArticleMainDlg::OnBnClickedButtonAddchannel2)
	ON_BN_CLICKED(IDC_BUTTON1, &CTPArticleMainDlg::OnBnClickedButton1)


END_MESSAGE_MAP()


// CTPArticleMainDlg message handlers
BOOL CTPArticleMainDlg::OnInitDialog()
{
	CTPDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_pChannelList = (CTPComboBox*)GetDlgItem(IDC_COMBO_CHANNELLIST);
	m_pChannelListPublic = (CTPComboBox*)GetDlgItem(IDC_COMBO_CHANNELLISTPUBLIC);
	m_pChannelNodeListPublic = (CTPComboBox*)GetDlgItem(IDC_COMBO_CHANNELNODELISTPUBLIC);

	//List
//	m_pArticleList = (CTPListCtrl *)GetDlgItem(IDC_LIST_ARTICLE); 
	CRect rtArticleList;
	GetDlgItem(IDC_LIST_ARTICLE)->GetWindowRect(&rtArticleList);
	ScreenToClient(rtArticleList);
	(CTPListCtrl *)GetDlgItem(IDC_LIST_ARTICLE)->ShowWindow(SW_HIDE);
	m_pArticleList    =  new  CTPArticleListCtrl();	
	m_pArticleList    -> Create(WS_BORDER | WS_CLIPCHILDREN | LVS_ICON | WS_CHILD | WS_VISIBLE | LVS_SHOWSELALWAYS | LVS_AUTOARRANGE  ,rtArticleList,this,0); //modify by xjc
//	m_pArticleList    ->SetWindowText(_L("ClipExplorer"));
	m_pArticleList->m_iViewType = VIEWTYPE_TEXTPICTURE;//VIEWTYPE_REPORT;//;

	m_pArticleList->InsertColumn(0, _T("Article Title"), LVCFMT_LEFT,500);	
	m_pArticleList->m_pArticleMainDlg = this;
	//
	TP_InitArticleCenter();

	CString sFileName = g_stuArticleInterface.stuArticleInterfce.TP_GetCurArticleHtmlPath();
	AdjustHtml(sFileName);
	//public channelNode
	GUID guidNode = guidPublicChannelNode;
	TPResDataArray aResData;
	g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeChild(guidNode, aResData);
	for (int l = 0 ; l < aResData.GetSize(); l++)
	{
		TPChannelNodeData stuChannelNode;
		g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeInfo(aResData[l].guidRes,stuChannelNode);
		m_aChannelNodeListPublic.Add(stuChannelNode.guidRes);
		m_pChannelNodeListPublic->InsertString(l, stuChannelNode.cNodeName);	
	}	

	//private channelNode
	ResetChannelContent(guidPrivateChannelNode, m_pChannelList, m_aChannelList);	
	
	//////////////////////////////////////////////////////////////////////////ctrl test
//  	CTPImgPreviewWnd *m_pImgPreWnd = new CTPImgPreviewWnd;
//  	m_pImgPreWnd->Create(0,this,0);
//  
//  	if(m_pImgPreWnd)
//  	{
//  		m_pImgPreWnd->SetLayeredWindowAttributes(0,150,2);
//  		m_pImgPreWnd->ShowWindow(SW_SHOW);
//  		m_pImgPreWnd->PreviewImg();		
//  	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTPArticleMainDlg::OnDestroy()
{
	CTPDialog::OnDestroy();

	if(m_pHtmlCtrl)
	{
		delete m_pHtmlCtrl;
		m_pHtmlCtrl = NULL;
	}

	if(m_pArticleList)
	{
		m_pArticleList->DestroyWindow();
		delete m_pArticleList;
		m_pArticleList = NULL;
	}
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

	if(m_pChannelNodeListPublic)
	{
		int iSel = m_pChannelNodeListPublic->GetCurSel();
		if(iSel<0 || m_aChannelNodeListPublic.GetSize() <= iSel) return;

		TPChannelNodeData stuChannelNode;
		g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeInfo(m_aChannelNodeListPublic[iSel],stuChannelNode);
		AddChannel(stuChannelNode,sChannelUrl,sChannelKeyDiv,stuChannelNode.eNodeType);
	}
}

void CTPArticleMainDlg::OnCbnSelChannelList()
{
	if(m_pChannelList)
	{
		int iSel = m_pChannelList->GetCurSel();
		if(iSel<0 || m_aChannelList.GetSize() <= iSel) return;

		TPChannelData stuChannel;
		g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(m_aChannelList[iSel],stuChannel);
		//AfxMessageBox(stuChannel.stuChannelBase.cChannelTitle);

		TPChannelBase *pChannelInfo = NULL;
		CTPChannelParser stuChannelParser;
		stuChannelParser.SetChannelAddress(stuChannel.stuChannelBase.cChannelAddress);
		stuChannelParser.GetChannelInfo(pChannelInfo);
		if(!pChannelInfo || pChannelInfo->aChannelItem.GetSize() <= 0)	{ASSERT(0); return ;}

		stuChannel.stuChannelBase = *pChannelInfo;
		stuChannel.AppendUpdateItem();

		//update
		m_pArticleList->SetResData(m_aArticleList, stuChannel);
		g_stuArticleInterface.stuChannelInterface.TP_SetChannelInfo(m_aChannelList[iSel],stuChannel);
	}
}
void CTPArticleMainDlg::OnCbnSelChannelNodeList()
{
	if(m_pChannelNodeListPublic)
	{
		int iSel = m_pChannelNodeListPublic->GetCurSel();
		if(iSel<0 || m_aChannelNodeListPublic.GetSize() <= iSel) return;


		TPChannelNodeData stuChannelNode;
		g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeInfo(m_aChannelNodeListPublic[iSel],stuChannelNode);
		//AfxMessageBox(stuChannel.stuChannelBase.cChannelTitle);

		ResetChannelContent(stuChannelNode.guidRes, m_pChannelListPublic, m_aChannelListPublic);
	}
}
void CTPArticleMainDlg::OnCbnSelArticleList()
{
	ASSERT(0);
}
void CTPArticleMainDlg::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	ASSERT(0);
	if(m_pArticleList)
	{
		LPNMITEMACTIVATE pNMLV = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		if(pNMLV)
		{
			if(m_pArticleList)
			{
				int iCurIndex = pNMLV ->iItem;
				if(iCurIndex >= 0 && iCurIndex < m_pArticleList ->GetItemCount())
				{
					//m_pArticleList->GetItemData(iCurIndex);
					TPArticleData stuArticle;
					if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(m_aArticleList[iCurIndex],TP_GRADE_ALL,stuArticle))
					{
						TPChannelData stuChannel;
						g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(stuArticle.guidNode,stuChannel);
						CString sFileName = g_stuArticleInterface.stuArticleInterfce.TP_GetCurArticleHtmlPath();
						CTPArticleParser::SaveHtml(sFileName, stuArticle.cText, stuArticle.stuChannelItem.cItemTitle,stuChannel.stuChannelBase.cChannelTitle,stuArticle.stuChannelItem.cItemPubDate);
						AdjustHtml(sFileName);

					}
				}
			}
		}

	}

	*pResult = 0;	
}

void CTPArticleMainDlg::OnBnClickedButtonAddchannel2()
{
	if(m_pChannelListPublic)
	{
		int iSel = m_pChannelListPublic->GetCurSel();
		if(iSel<0 || m_aChannelListPublic.GetSize() <= iSel) return;

		TPChannelNodeData stuChannelNode;
		g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeInfo(guidPrivateChannelNode,stuChannelNode);

		TPChannelData stuChannel;
		g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(m_aChannelListPublic[iSel],stuChannel);
		AddChannel(stuChannelNode, stuChannel.stuChannelBase.cChannelAddress,stuChannel.cKeyDiv,stuChannel.eNodeType);

		ResetChannelContent(guidPrivateChannelNode, m_pChannelList, m_aChannelList);

	}
}
void CTPArticleMainDlg::AddChannel(TPChannelNodeData &stuChannelNode,CString sChannelUrl,CString sChannelKeyDiv, TP_CHANNEL_NODETYPE eNodeType)
{
	if(sChannelUrl.IsEmpty()/* || sChannelKeyDiv.IsEmpty()*/)
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
	if(g_stuArticleInterface.stuChannelInterface.TP_IsChannelExist(stuChannelNode.guidRes, pChannelInfo))
	{
		if(cAddress)	{delete cAddress; cAddress = NULL;}
		if(cKeyDiv)		{delete cKeyDiv; cKeyDiv = NULL;}
		AfxMessageBox(_T("Already exist!"));
		return ;

	}

	TPChannelData stuChannel;
	CoCreateGuid(&stuChannel.guidRes);
	stuChannel.guidNode = stuChannelNode.guidRes;
	TP_StrCpy(stuChannel.cKeyDiv, sChannelKeyDiv.GetBuffer(), sChannelKeyDiv.GetLength());
	stuChannel.eNodeType = (stuChannelNode.eNodeType | (eNodeType&TP_CHANNEL_TYPEALL));
	stuChannel.stuChannelBase = *pChannelInfo;
	//	stuChannel.AppendUpdateItem();
	g_stuArticleInterface.stuChannelInterface.TP_SetChannelInfo(stuChannel.guidRes,stuChannel);
	g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(stuChannel.guidRes,stuChannel);

	if(cAddress)	{delete cAddress; cAddress = NULL;}
	if(cKeyDiv)		{delete cKeyDiv; cKeyDiv = NULL;}
}
void CTPArticleMainDlg::ResetChannelContent(GUID guidChannelNode, CTPComboBox *pChannel,	CGuidArray   &aChannelListPublic)
{
	pChannel->ResetContent();
	aChannelListPublic.RemoveAll();

	TPResDataArray aResData;
	g_stuArticleInterface.stuChannelInterface.TP_GetChannelChild(guidChannelNode, aResData);
	for (int l = 0 ; l < aResData.GetSize(); l++)
	{
		TPChannelData stuChannel;
		g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(aResData[l].guidRes,stuChannel);
		aChannelListPublic.Add(stuChannel.guidRes);
		pChannel->InsertString(l, stuChannel.stuChannelBase.cChannelTitle);	
	}
	pChannel->SetCurSel(1);
	OnCbnSelChannelList();
}
void CTPArticleMainDlg::OnBnClickedButton1()
{
	ASSERT(0);
	CRect rtMain;
	GetWindowRect(&rtMain);
	CRect rc;
	GetDlgItem(IDC_STATIC_ARTICLETEXT)->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left = rtMain.left + 10;
	rc.right = rtMain.right - 10;
	GetDlgItem(IDC_STATIC_ARTICLETEXT)->MoveWindow(rc);
	CString sFileName = g_stuArticleInterface.stuArticleInterfce.TP_GetCurArticleHtmlPath();

	AdjustHtml(sFileName);

}
void CTPArticleMainDlg::AdjustHtml(CString sHtml)
{
	if(sHtml.IsEmpty() || !PathFileExists(sHtml))	return ;
	CRect rc;
	GetDlgItem(IDC_STATIC_ARTICLETEXT)->GetWindowRect(&rc);
	this->ScreenToClient(&rc);

	if(NULL == m_pHtmlCtrl)
	{
		m_pHtmlCtrl = new CTPHtmlCtrl;
		m_pHtmlCtrl->Create(NULL,						 // class name
			NULL,										 // title
			(WS_CHILD | WS_VISIBLE ),					 // style
			rc,											 // rectangle
			this,									     // parent
			IDC_STATIC_ARTICLETEXT,										 // control ID
			NULL);									 // frame/doc context not used
	}
	else
	{
		m_pHtmlCtrl->MoveWindow(rc,FALSE);
		m_pHtmlCtrl->Navigate2(sHtml);
	}

}