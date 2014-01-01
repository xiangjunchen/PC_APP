#pragma once


// CTPPublicArticleManage dialog

class CTPPublicArticleManage : public CTPDialog
{
	DECLARE_DYNAMIC(CTPPublicArticleManage)

public:
	CTPPublicArticleManage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTPPublicArticleManage();

// Dialog Data
	enum { IDD = IDD_DIALOG_PUBLICCHANNELCENTER };

	CTPComboBox *m_pChannelList;
	CTPComboBox *m_pChannelNodeListPublic;
	CGuidArray   m_aChannelNodeListPublic;
	CGuidArray   m_aChannelList;
	CGuidArray   m_aArticleList;
;	CTPArticleMainDlg *m_pArticleMainDlg;

	CTPComboBox *m_pChannelListPublic;
	CGuidArray   m_aChannelListPublic;
	void ResetChannelContent(GUID guidChannelNode, CTPComboBox *pChannel,	CGuidArray   &aChannelListPublic);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedButtonAddchannel();
	afx_msg void OnBnClickedButtonAddchannel2();
	afx_msg void OnCbnSelChannelList();
	afx_msg void OnCbnSelChannelNodeList();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
