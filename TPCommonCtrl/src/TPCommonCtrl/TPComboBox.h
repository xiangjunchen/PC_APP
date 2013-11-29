#pragma once


// CTPComboBox
class CTPToolTipCtrl;
class AFX_EXT_CLASS CTPComboBox : public CComboBox ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPComboBox)

public:
	CTPComboBox();
	virtual ~CTPComboBox();
public:
	CWnd                  *m_pComboBoxEx; 	
	CTPComboLBox          *m_pListBox;
	CRect                  m_rtListBox;
	TP_BORDER_STYLE        m_eBorderStyle;

	struct TPComboBoxItem
	{
		CString     sText;
		DWORD_PTR   dwItemData;
		BOOL        bSelect;
	};
protected:
	BOOL                   m_bSubWindow; 
	BOOL                   m_bCreate; 
	UINT                   m_uStyle;
	TPCommonRes            *m_pButton[2]; 	


	 CTPEdit               *m_pEdit;
	 CString                m_sToolTips;	 
	 CString                m_sWndText;
	 TP_SYSTEM_COLOR		m_eBackgroundColor;	 
	 int                    m_iCurSel;
	 int                    m_iDownSel;

public:
	CTPEdit           *GetEdit();
	CTPComboLBox      *GetListBox();
	void               SetLBText(int iItem,CString sText);
	void               SetToolTips(CString sText);
	TP_SYSTEM_COLOR	   SetBackgroundColor( TP_SYSTEM_COLOR eColor); 	
	void               EnableToolTips(BOOL bEnable);
	void               EnableItem(int iItem,BOOL bEnable);  
	void               SetItemColor(int iItem,COLORREF clColor);  
	int                GetEnableItem(int iItem,BOOL bUp);//added by chenxiaolong 2010.3.16 
	void               SetEditEx(int iLength = -1);
	void               SortItem(BOOL (*f)(CTPComboBox::TPComboBoxItem *p1,CTPComboBox::TPComboBoxItem *p2));

protected:
	virtual void            SubWindow(BOOL bSub);

	void                    PaintButton(CDC *pDC,COMBOBOXINFO &pInfo);
protected:
	DECLARE_MESSAGE_MAP()
public:
protected:
	virtual void PreSubclassWindow();
	BOOL         OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCbnSelchange();
	afx_msg void OnCbnCloseup();
	afx_msg void OnCbnDropdown();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
};


