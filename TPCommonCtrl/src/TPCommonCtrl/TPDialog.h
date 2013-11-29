#pragma once

#define   TP_SYSKEY_MAINMENU         4000 + 5
#define   TP_SYSKEY_AUDIOOUTPUTTOOL  4000 + 32

// CTPDialog 对话框 
class AFX_EXT_CLASS CTPDialog : public CDialog ,public CTPBaseFrame
{
	DECLARE_DYNAMIC(CTPDialog)

public:
	CTPDialog(CWnd* pParent = NULL);   // 标准构造函数
	CTPDialog(UINT nID,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTPDialog();

	// 对话框数据
	enum { IDD = 0 };
protected:
	LPVOID            m_pWndList;
	BOOL              m_bChild; 
	HWND              m_hFocusWnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL GetData(void *pData);

	BOOL         IsWindowVisible() const;
	void         SetPathNameEdit(UINT nID,long lMaxLen = MAX_PATH);

	static      CTPDialog *GetDialog();  
	static      int        GetDialogCount();  

	void        SetActivate(BOOL bAct);

	 void FunTest(TPControlData* pCtrl);
	 void FunTest(TPMarkPointInfo* pData);
	static      BOOL     (*m_pDlgSysKeyDown)(UINT nChar,UINT nID);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnClose();
	virtual INT_PTR DoModal();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	afx_msg BOOL OnNcActivate(BOOL bActive);
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
};
