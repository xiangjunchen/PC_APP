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
	enum { IDD = IDD_DAILOG_ARTICLEMAINDLG };

public:
	GUID		 m_guidCurArticle;
	BOOL         m_bFullScreenArticle;
	LPBYTE       m_pBgIcon;

	CTPArticleListCtrl *m_pArticleList;
	CTPHtmlCtrl *m_pHtmlCtrl;
	void		AddChannel(TPChannelNodeData &stuChannelNode,CString sChannelUrl,CString sChannelKeyDiv, TP_CHANNEL_NODETYPE eNodeType);
	BOOL		AddChannel(GUID guidNode, GUID guidSrcChannel);

protected:
	virtual BOOL    DoSysMenu(CPoint &ptScreen);
	void             OnMenuCmd(UINT uID);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void	SetShowWindow(int nCmdShow);
	void	FullScreen();
	DECLARE_MESSAGE_MAP()
public:
	void	SetArticleBgGround(CDC *pDc);
	void	AdjustHtml(CString sHtml);
	CRect	GetAritcleRect();

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelArticleList();
// 	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButton1();


	afx_msg void OnBnClickedButtonOwnchannel();
	afx_msg void OnBnClickedButtonFullscreen();
	afx_msg void OnBnClickedButtonPublic();
	afx_msg void OnBnClickedButtonFavouritearticle();
	afx_msg void OnBnClickedButtonFavouriteadd();
	afx_msg void OnBnClickedButtonNextArticle();
	afx_msg void OnBnClickedButtonForwardArticle();
	afx_msg void OnPaint();
};
