// TPPublicArticleManage.cpp : implementation file
//

#include "stdafx.h"
#include "TPArticleBroadCast.h"
#include "TPPublicArticleManage.h"


// CTPPublicArticleManage dialog

IMPLEMENT_DYNAMIC(CTPPublicArticleManage, CTPDialog)

CTPPublicArticleManage::CTPPublicArticleManage(CWnd* pParent /*=NULL*/)
	: CTPDialog(CTPPublicArticleManage::IDD, pParent)
{
	m_pChannelListPublic = NULL;
	m_pChannelList = NULL;
	m_pChannelNodeListPublic = NULL;
	m_pArticleMainDlg = NULL;
}

CTPPublicArticleManage::~CTPPublicArticleManage()
{
}

void CTPPublicArticleManage::DoDataExchange(CDataExchange* pDX)
{
	CTPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTPPublicArticleManage, CTPDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADDCHANNEL2, &CTPPublicArticleManage::OnBnClickedButtonAddchannel2)
	ON_CBN_SELENDOK(IDC_COMBO_CHANNELLIST, &CTPPublicArticleManage::OnCbnSelChannelList)
	ON_CBN_SELENDOK(IDC_COMBO_CHANNELNODELISTPUBLIC, &CTPPublicArticleManage::OnCbnSelChannelNodeList)
	ON_BN_CLICKED(IDC_BUTTON_ADDCHANNEL, &CTPPublicArticleManage::OnBnClickedButtonAddchannel)

END_MESSAGE_MAP()


// CTPPublicArticleManage message handlers
void CTPPublicArticleManage::OnBnClickedButtonAddchannel()
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
		m_pArticleMainDlg->AddChannel(stuChannelNode,sChannelUrl,sChannelKeyDiv,stuChannelNode.eNodeType);
	}
}

void CTPPublicArticleManage::OnCbnSelChannelNodeList()
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
void CTPPublicArticleManage::OnCbnSelChannelList()
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
		//m_pArticleList->SetResData(m_aArticleList, stuChannel);
		g_stuArticleInterface.stuChannelInterface.TP_SetChannelInfo(m_aChannelList[iSel],stuChannel);
	}
}

void CTPPublicArticleManage::OnBnClickedButtonAddchannel2()
{	
 	if(m_pChannelListPublic)
 	{
 		int iSel = m_pChannelListPublic->GetCurSel();
 		if(iSel<0 || m_aChannelListPublic.GetSize() <= iSel) return;
 
		m_pArticleMainDlg->AddChannel(guidPrivateChannelNode, m_aChannelListPublic[iSel]);
 
 		ResetChannelContent(guidPrivateChannelNode, m_pChannelList, m_aChannelList);

 	}
}
BOOL CTPPublicArticleManage::OnInitDialog()
{
	CTPDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_pChannelListPublic = (CTPComboBox*)GetDlgItem(IDC_COMBO_CHANNELLISTPUBLIC);
	m_pChannelList = (CTPComboBox*)GetDlgItem(IDC_COMBO_CHANNELLIST);
	m_pChannelNodeListPublic = (CTPComboBox*)GetDlgItem(IDC_COMBO_CHANNELNODELISTPUBLIC);

	//public channelNode
	TPResDataArray aResData;
	g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeChild(guidPublicChannelNode, aResData);
	for (int l = 0 ; l < aResData.GetSize(); l++)
	{
		TPChannelNodeData stuChannelNode;
		g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeInfo(aResData[l].guidRes,stuChannelNode);
		m_aChannelNodeListPublic.Add(stuChannelNode.guidRes);
		m_pChannelNodeListPublic->InsertString(l, stuChannelNode.cNodeName);	
	}	


	ResetChannelContent(guidPrivateChannelNode, m_pChannelList, m_aChannelList);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTPPublicArticleManage::ResetChannelContent(GUID guidChannelNode, CTPComboBox *pChannel,	CGuidArray   &aChannelListPublic)
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
	// 	pChannel->SetCurSel(1);
	// 	OnCbnSelChannelList();
}