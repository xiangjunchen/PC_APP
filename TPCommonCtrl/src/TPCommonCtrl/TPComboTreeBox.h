#pragma once


// CTPComboTreeBox
#define WC_COMBOTREEBOX _L("ComboTreeBox")

#define CTS_SIMPLE            0x0001L
#define CTS_DROPDOWN          0x0002L
#define CTS_DROPDOWNLIST      0x0003L

#define  WM_COMBOTREEBOXSELTEST   WM_USER+12200

class AFX_EXT_CLASS CTPComboTreeBox : public CTPWnd ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPComboTreeBox)

public:
	CTPComboTreeBox();
	virtual ~CTPComboTreeBox();
protected:
	BOOL                   m_bSubWindow; 
	BOOL                   m_bCreate; 
	TP_BORDER_STYLE        m_eBorderStyle;
	UINT                   m_uStyle;
	TPCommonRes            *m_pButton[2]; 		


	CWnd                   *m_pDropDown;
	CTPComboTBox           *m_pTreeCtrlEx;
	CTPComboDTBox          *m_pDTreeCtrlEx;
	CTPComboTBase          *m_pBaseTreeCtrl;

	CTPEdit                *m_pEdit;
	UINT                    m_uState; 
	int                     m_iTreeHeight;
	HTREEITEM               m_hSelItem;
	BOOL                    m_bDirTree;
	BOOL                    m_bSetCapture;
	CString                 m_sToolTips;


public:

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	static  void            RegisterClass(BOOL bRegister);
	CTPTreeCtrl            *GetTreeCtrl();  
	BOOL                    SetStyle(UINT uAdd);
	void                    SetDropHeight(int iHeight);


	BOOL                    GetDroppedState();
	void                    EnableToolTips(BOOL bEnable);

    LRESULT                 OnMsgDirTree(WPARAM wp,LPARAM lp);
	void                    GetComboBoxInfo(COMBOBOXINFO *comboBoxInfo);
	virtual BOOL            TestSelectItem(HTREEITEM hTreeItemNew,HTREEITEM hTreeItemOld);	
	virtual void            TestTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void            SubWindow(BOOL bSub);
	virtual void            PaintText(CDC *pDC,CRect rtText);
	virtual void            SelectItem(HTREEITEM hTreeItem);	
	

	void                    PaintButton(CDC *pDC,COMBOBOXINFO &comboBoxInfo);
	void                    OnSize();	

	LRESULT                 OnMsgShowDropDown(WPARAM wp,LPARAM lp);
	BOOL                    OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);	
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
};


