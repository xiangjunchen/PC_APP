
// TPBaseTestDlg.h : header file
//

#pragma once
class CTPHtmlCtrl;

// CTPBaseTestDlg dialog
class CTPBaseTestDlg : public CDialog
{
// Construction
public:
	CTPBaseTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TPBASETEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CTPHtmlCtrl *m_pHtmlCtrl;
	void	AdjustHtml();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
