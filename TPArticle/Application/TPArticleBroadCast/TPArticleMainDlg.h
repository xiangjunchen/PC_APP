#pragma once


// CTPArticleMainDlg dialog
class CTPArticleListCtrl;
class CTPArticleMainDlg : public CTPDialog
{
	DECLARE_DYNAMIC(CTPArticleMainDlg)

public:
	CTPArticleMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTPArticleMainDlg();

// Dialog Data
	enum { IDD = IDD_TPARTICLEMAINDLG };

public:
	CTPComboBox *m_pChannelNodeListPublic;
	CGuidArray   m_aChannelNodeListPublic;

	CTPComboBox *m_pChannelListPublic;
	CGuidArray   m_aChannelListPublic;

	CTPComboBox *m_pChannelList;
	CTPArticleListCtrl *m_pArticleList;
	CTPHtmlCtrl *m_pHtmlCtrl;
	CGuidArray   m_aChannelList;
	CGuidArray   m_aArticleList;
	void AddChannel(TPChannelNodeData &stuChannelNode,CString sChannelUrl,CString sChannelKeyDiv,TP_CHANNEL_NODETYPE eNodeType);
	void ResetChannelContent(GUID guidChannelNode, CTPComboBox *pChannel,	CGuidArray   &aChannelListPublic);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void SetShowWindow(int nCmdShow);
	DECLARE_MESSAGE_MAP()
public:
	void AdjustHtml(CString sHtml);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonAddchannel();
	afx_msg void OnCbnSelChannelList();
	afx_msg void OnCbnSelChannelNodeList();
	afx_msg void OnCbnSelArticleList();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButtonAddchannel2();
	afx_msg void OnBnClickedButton1();


	afx_msg void OnBnClickedButtonOwnchannel();
};
