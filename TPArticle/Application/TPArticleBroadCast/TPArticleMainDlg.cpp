// TPArticleMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TPArticleMainDlg.h"


// CTPArticleMainDlg dialog

IMPLEMENT_DYNAMIC(CTPArticleMainDlg, CTPDialog)

CTPArticleMainDlg::CTPArticleMainDlg(CWnd* pParent /*=NULL*/)
	: CTPDialog(CTPArticleMainDlg::IDD, pParent)
{
	m_bFullScreenArticle = FALSE;
	m_pArticleList = NULL;
	m_pHtmlCtrl    = NULL;
	m_guidCurArticle = GUID_NULL;
	m_pBgIcon	   = NULL;
}

CTPArticleMainDlg::~CTPArticleMainDlg()
{
	if(m_pBgIcon)
	{
		delete m_pBgIcon;
		m_pBgIcon = NULL;
	}
}

void CTPArticleMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CTPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTPArticleMainDlg, CTPDialog)
	ON_WM_DESTROY()
	
// 	ON_NOTIFY(NM_CLICK,IDC_LIST_ARTICLE,OnNMClick)
// 	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
// 	ON_BN_CLICKED(IDC_BUTTON1, &CTPArticleMainDlg::OnBnClickedButton1)


	ON_BN_CLICKED(IDC_BUTTON_OWNCHANNEL, &CTPArticleMainDlg::OnBnClickedButtonOwnchannel)
	ON_BN_CLICKED(IDC_BUTTON_FULLSCREEN, &CTPArticleMainDlg::OnBnClickedButtonFullscreen)
	ON_BN_CLICKED(IDC_BUTTON_PUBLIC, &CTPArticleMainDlg::OnBnClickedButtonPublic)
	ON_BN_CLICKED(IDC_BUTTON_FAVOURITEARTICLE, &CTPArticleMainDlg::OnBnClickedButtonFavouritearticle)
	ON_BN_CLICKED(IDC_BUTTON_FAVOURITEADD, &CTPArticleMainDlg::OnBnClickedButtonFavouriteadd)
	ON_BN_CLICKED(IDC_BUTTON_FORWARDARTICLE, &CTPArticleMainDlg::OnBnClickedButtonForwardArticle)
	ON_BN_CLICKED(IDC_BUTTON_NEXTARTICLE, &CTPArticleMainDlg::OnBnClickedButtonNextArticle)
	ON_COMMAND_RANGE(ID_MAIN_BASE ,ID_MAIN_MAX ,OnMenuCmd)	

	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTPArticleMainDlg message handlers
BOOL CTPArticleMainDlg::OnInitDialog()
{
	CTPDialog::OnInitDialog();
	SetShowWindow(SW_HIDE);

	//
	TP_InitArticleCenter();

	// TODO:  Add extra initialization here

	//List
	CRect rtArticleList;
	GetDlgItem(IDC_STATIC_LIST)->GetWindowRect(&rtArticleList);
	ScreenToClient(rtArticleList);
	(CTPListCtrl *)GetDlgItem(IDC_STATIC_LIST)->ShowWindow(SW_HIDE);
	m_pArticleList    =  new  CTPArticleListCtrl();	
	m_pArticleList->m_pArticleMainDlg = this;
	m_pArticleList    -> Create(WS_BORDER | WS_CLIPCHILDREN | LVS_ICON | WS_CHILD | WS_VISIBLE | LVS_SHOWSELALWAYS | LVS_AUTOARRANGE  ,rtArticleList,this,0); //modify by xjc
//	m_pArticleList    ->SetWindowText(_L("ClipExplorer"));
	m_pArticleList->m_iViewType = VIEWTYPE_TEXTPICTURE;//VIEWTYPE_REPORT;//;//VIEWTYPE_REPORT;//;

	m_pArticleList->InsertColumn(0, _T("Article Title"), LVCFMT_LEFT,rtArticleList.Width()-20);	

	
	CString sFileName = g_stuArticleInterface.stuArticleInterfce.TP_GetCurArticleHtmlPath();
	AdjustHtml(sFileName);


	//private channelNode
	OnBnClickedButtonOwnchannel();
	
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



void CTPArticleMainDlg::OnCbnSelArticleList()
{
	ASSERT(0);
}
// void CTPArticleMainDlg::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	ASSERT(0);
// 	if(m_pArticleList)
// 	{
// 		LPNMITEMACTIVATE pNMLV = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
// 		if(pNMLV)
// 		{
// 			if(m_pArticleList)
// 			{
// 				int iCurIndex = pNMLV ->iItem;
// 				if(iCurIndex >= 0 && iCurIndex < m_pArticleList ->GetItemCount())
// 				{
// 					//m_pArticleList->GetItemData(iCurIndex);
// 					TPArticleData stuArticle;
// 					if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(m_aArticleList[iCurIndex],TP_GRADE_ALL,stuArticle))
// 					{
// 						TPChannelData stuChannel;
// 						g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(stuArticle.guidNode,stuChannel);
// 						CString sFileName = g_stuArticleInterface.stuArticleInterfce.TP_GetCurArticleHtmlPath();
// 						CTPArticleParser::SaveHtml(sFileName, stuArticle.cText, stuArticle.stuChannelItem.cItemTitle,stuChannel.stuChannelBase.cChannelTitle,stuArticle.stuChannelItem.cItemPubDate);
// 						AdjustHtml(sFileName);
// 
// 					}
// 				}
// 			}
// 		}
// 
// 	}
// 
// 	*pResult = 0;	
// }


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
CRect CTPArticleMainDlg::GetAritcleRect()
{
	CRect rtArticleText;
	if(m_bFullScreenArticle)
	{
		GetDlgItem(IDC_STATIC_ARTICLETEXT)->GetWindowRect(&rtArticleText);
		ScreenToClient(&rtArticleText);

		CRect rtArticleList;
		m_pArticleList->GetWindowRect(&rtArticleList);
		ScreenToClient(&rtArticleList);

		rtArticleText.left = rtArticleList.left;
	}
	else
	{
		GetDlgItem(IDC_STATIC_ARTICLETEXT)->GetWindowRect(&rtArticleText);
		ScreenToClient(&rtArticleText);
	}
	return rtArticleText;
}
void CTPArticleMainDlg::AdjustHtml(CString sHtml)
{
	if(sHtml.IsEmpty() || !PathFileExists(sHtml))	return ;
	CRect rc = GetAritcleRect();
// 	GetDlgItem(IDC_STATIC_ARTICLETEXT)->GetWindowRect(&rc);
// 	this->ScreenToClient(&rc);

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
	if(m_guidCurArticle != GUID_NULL)	
 		m_pHtmlCtrl->ShowWindow(SW_SHOW);
	else
		m_pHtmlCtrl->ShowWindow(SW_HIDE);
}

void CTPArticleMainDlg::SetArticleBgGround(CDC *pDc)
{
	if(m_guidCurArticle != GUID_NULL)	
	{
		return ;
	}
	CRect rtAritcleBack = GetAritcleRect();
	int iWidth = 0 ; int iHeight = 0;
	int iIconWidth = rtAritcleBack.Width(),  iIconHeight = rtAritcleBack.Height();

	if(m_pBgIcon == NULL)
	{
		CString sImgSourcePath = TP_GetSysPath() ;
		sImgSourcePath += _T("\\Default\\Background\\002.jpg");
		CTPImgDecode stuDec;
		if(FIO_Succeed == stuDec.Open(sImgSourcePath.GetBuffer(),iWidth,iHeight))
		{
			LPBYTE pByte = new BYTE[iWidth * iHeight * sizeof(DWORD)];
			ZeroMemory(pByte, iWidth * iHeight * sizeof(DWORD));

			if(FIO_Succeed == stuDec.GetImage(pByte))
			{
				if(m_pBgIcon)	{delete m_pBgIcon; m_pBgIcon = NULL;}
				m_pBgIcon = new BYTE[iIconWidth * iIconHeight * sizeof(DWORD)];
				ZeroMemory(m_pBgIcon,iIconWidth * iIconHeight * sizeof(DWORD));	
				TP_StretchBlt_Mean((LPDWORD)m_pBgIcon, 0, 0, iIconWidth, iIconHeight, iIconWidth, iIconHeight, (LPDWORD)pByte, 0, 0, iWidth, iHeight, iWidth, iHeight, TP_BUFFER_COPY, TP_PARITY_BOTH);

				//TP_WriteBimap(_T("C:\\1.bmp"),iIconWidth,iIconHeight,32,pIcon);
			}
			if(pByte) {delete pByte; pByte = NULL;}
		}
	}

	if(m_pBgIcon && pDc)
		TP_StretchDIBitsEx(pDc,m_pBgIcon,iIconWidth,iIconHeight,rtAritcleBack,rtAritcleBack.Width(),rtAritcleBack.Height(),TRUE);	

}
void CTPArticleMainDlg::OnBnClickedButtonOwnchannel()
{
	if(m_bFullScreenArticle)
		OnBnClickedButtonFullscreen();
	SetShowWindow(SW_HIDE);

	CRect rtArticleText;
	GetDlgItem(IDC_STATIC_ARTICLETEXT)->GetWindowRect(&rtArticleText);
	ScreenToClient(&rtArticleText);

	CRect rtArticleList;
	m_pArticleList->GetWindowRect(&rtArticleList);
	ScreenToClient(&rtArticleList);

	rtArticleList.top = rtArticleText.top;
	m_pArticleList->MoveWindow(rtArticleList);

	TPResDataArray aResData;
	g_stuArticleInterface.stuChannelInterface.TP_GetChannelChild(guidPrivateChannelNode, aResData);

	m_pArticleList->SetViewType(TP_RES_CHANNEL);
	m_pArticleList->SetResData(aResData);
}
void CTPArticleMainDlg::SetShowWindow(int nCmdShow)
{
// 	int id[] = {
// 	IDC_EDIT_CHANNELURL,
// 	IDC_BUTTON_ADDCHANNEL,
// 	IDC_EDIT_CHANNELKEYDIV,
// 	IDC_COMBO_CHANNELNODELISTPUBLIC,
// 	IDC_COMBO_CHANNELLISTPUBLIC,
// 	IDC_BUTTON_ADDCHANNEL2,
// 
// 	IDC_COMBO_CHANNELLIST,
// //	IDC_STATIC_LIST
// 	};
// 	for (int l = 0 ; l < sizeof(id)/sizeof(int); l++)
// 	{
// 		GetDlgItem(id[l])->ShowWindow(nCmdShow);
// 	}
}
void CTPArticleMainDlg::FullScreen()
{
	m_pHtmlCtrl->SetRedraw(FALSE);
	if(!m_bFullScreenArticle)
	{
		SetShowWindow(SW_HIDE);
		m_pArticleList->ShowWindow(SW_HIDE);

		CRect rtArticleText;
		GetDlgItem(IDC_STATIC_ARTICLETEXT)->GetWindowRect(&rtArticleText);
		ScreenToClient(&rtArticleText);

		CRect rtArticleList;
		m_pArticleList->GetWindowRect(&rtArticleList);
		ScreenToClient(&rtArticleList);

		rtArticleText.left = rtArticleList.left;

		m_pHtmlCtrl->MoveWindow(rtArticleText);
		m_bFullScreenArticle = TRUE;
	}
	else
	{
		SetShowWindow(SW_HIDE);
		m_pArticleList->ShowWindow(SW_SHOW);

		CRect rtArticleText;
		GetDlgItem(IDC_STATIC_ARTICLETEXT)->GetWindowRect(&rtArticleText);
		ScreenToClient(&rtArticleText);
		m_pHtmlCtrl->MoveWindow(rtArticleText);
		m_bFullScreenArticle = FALSE;
	}
	m_pArticleList->UpdateCurArticle(m_guidCurArticle);
// 	CString sFileName = g_stuArticleInterface.stuArticleInterfce.TP_GetCurArticleHtmlPath();
// 	m_pHtmlCtrl->Navigate2(sFileName);

	m_pHtmlCtrl->SetRedraw(TRUE);
	Invalidate(TRUE);
	UpdateWindow();
}
void CTPArticleMainDlg::OnBnClickedButtonFullscreen()
{
	FullScreen();
}	

void CTPArticleMainDlg::OnBnClickedButtonPublic()
{

	//public channelNode
	GUID guidNode = guidPublicChannelNode;
	TPResDataArray aResData;
	g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeChild(guidNode, aResData);
	m_pArticleList->SetViewType(TP_RES_CHANNELNODE);
	m_pArticleList->SetResData(aResData);
}

void CTPArticleMainDlg::OnBnClickedButtonFavouritearticle()
{
	// TODO: Add your control notification handler code here
	TPResDataArray aResData;
	g_stuArticleInterface.stuArticleInterfce.TP_GetArticleChild(guidPrivateChannel,aResData);
	m_pArticleList->SetViewType(TP_RES_ARTICLE);
	m_pArticleList->SetResData(aResData);

}

void CTPArticleMainDlg::OnBnClickedButtonFavouriteadd()
{
	// TODO: Add your control notification handler code here
	if(m_guidCurArticle != GUID_NULL)
	{
		TPArticleData stuArticle;
		if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(m_guidCurArticle,TP_GRADE_ALL,stuArticle))
		{
			if(!g_stuArticleInterface.stuArticleInterfce.TP_IsArticleExist(guidPrivateChannel, &stuArticle.stuChannelItem))
			{
				CoCreateGuid(&stuArticle.guidRes);
				stuArticle.guidNode = guidPrivateChannel;
				g_stuArticleInterface.stuArticleInterfce.TP_SetArticleInfo(stuArticle.guidRes,TP_GRADE_ALL,stuArticle);
				AfxMessageBox(_T("收藏成功!"));
			}
			else
			{
				g_stuArticleInterface.stuArticleInterfce.TP_DelArticleInfo(stuArticle.stuChannelItem.guidItem);//guidItem与m_guidCurArticle可能不一致
				AfxMessageBox(_T("成功从收藏中去掉!"));
			}
		}
	}
}
BOOL CTPArticleMainDlg::DoSysMenu(CPoint &ptScreen)
{
	CTPMenuEx menuLoad,*pSubMenu = NULL;

	menuLoad.LoadMenu(IDR_MENU_SYSMENU);
	pSubMenu = menuLoad.GetSubMenu(0);

	if(pSubMenu)
	{		 
		if(0)//后门控制
		{
			pSubMenu ->DeleteMenu( ID_SYSMENU_PUBLICCHANNELMANAGE, MF_BYCOMMAND);
		}


		TP_LoadMenuStr(pSubMenu);
		pSubMenu ->TrackPopupMenu(TPM_LEFTALIGN,ptScreen.x,ptScreen.y,this);
	}

	menuLoad.DestroyMenu();
	return TRUE;
}
void CTPArticleMainDlg::OnMenuCmd(UINT uID)
{
	switch (uID)
	{
	case ID_SYSMENU_PUBLICCHANNELMANAGE:
		CTPPublicArticleManage dlgArticleManage;
		dlgArticleManage.m_pArticleMainDlg = this;
		dlgArticleManage.DoModal();
		break;

// 	case ID_SYSMENU_CLOSE:
// 		{
// 			PostMessage(WM_CLOSE,0,0);
// 		}
// 		break;
// 	case ID_SYSMENU_HELP:
// 		{
// 			TPBaseSystemInfo *pInfo = TP_GetBaseSystemInfo();
// 			if(pInfo && pInfo  ->TP_PopupHelpInfo)
// 				pInfo  ->TP_PopupHelpInfo(_T("Team User Manage Tool"));
// 		}
// 		break;
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
	GUID guidExist = GUID_NULL;

	TPChannelBase *pChannelInfo = NULL;
	CTPChannelParser stuChannelParser;
	stuChannelParser.SetChannelAddress(cAddress);
	stuChannelParser.GetChannelInfo(pChannelInfo);
	if(g_stuArticleInterface.stuChannelInterface.TP_IsChannelExist(stuChannelNode.guidRes, pChannelInfo, guidExist))
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
BOOL CTPArticleMainDlg::AddChannel(GUID guidNode, GUID guidSrcChannel)
{

	BOOL bAdd = TRUE;
	TPChannelData stuChannel;
	if(S_OK != g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(guidSrcChannel,stuChannel))
	{
		ASSERT(0);
		return bAdd;
	}
	GUID guidExist = GUID_NULL;
	if(g_stuArticleInterface.stuChannelInterface.TP_IsChannelExist(guidPrivateChannelNode, &stuChannel.stuChannelBase,guidExist))
	{
		bAdd = FALSE;
		g_stuArticleInterface.stuChannelInterface.TP_DelChannelInfo(guidExist);
	}
	else
	{
		TPChannelNodeData stuChannelNode;
		if(S_OK != g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeInfo(guidNode,stuChannelNode))
		{
			ASSERT(0);
			return bAdd;
		}
		AddChannel(stuChannelNode, stuChannel.stuChannelBase.cChannelAddress,stuChannel.cKeyDiv,stuChannel.eNodeType);
	}
	return bAdd;
}
void CTPArticleMainDlg::OnBnClickedButtonNextArticle()
{
	m_pArticleList->GotoArticle(FALSE);
}
void CTPArticleMainDlg::OnBnClickedButtonForwardArticle()
{
	m_pArticleList->GotoArticle(TRUE);
}
void CTPArticleMainDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	SetArticleBgGround(&dc);
}
