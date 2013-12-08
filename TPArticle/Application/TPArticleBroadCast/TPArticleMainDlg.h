#pragma once


// CTPArticleMainDlg dialog

class CTPArticleMainDlg : public CTPDialog
{
	DECLARE_DYNAMIC(CTPArticleMainDlg)

public:
	CTPArticleMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTPArticleMainDlg();

// Dialog Data
	enum { IDD = IDD_TPARTICLEMAINDLG };

public:
	CTPComboBox *m_pChannelList;
	CTPListCtrl *m_pArticleList;
	CTPHtmlCtrl *m_pHtmlCtrl;
	CGuidArray   m_aChannelList;
	CGuidArray   m_aArticleList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void AdjustHtml();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonAddchannel();
	afx_msg void OnCbnSelChannelList();
	afx_msg void OnCbnSelArticleList();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

};
