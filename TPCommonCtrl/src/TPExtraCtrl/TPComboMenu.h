#pragma once

#define  WC_COMBOMENU   _T("ComboMenu")
#define  WS_SPARTE      _T("-----")



class AFX_EXT_CLASS CTPComboMenu : public CTPWnd ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPComboMenu)

public:
	CTPComboMenu();
	virtual ~CTPComboMenu();
public:

protected:
	BOOL                   m_bSubWindow; 
	BOOL                   m_bCreate; 
	TP_BORDER_STYLE        m_eBorderStyle;
	UINT                   m_uStyle;
	TPCommonRes            *m_pButton[2]; 	
	CTPMenuEx              *m_pMenu;
	
	CString                m_sWndText;
	int                    m_iSelText; 
	BOOL                   m_bPressed;
	UINT_PTR               m_hTimer; 
	BOOL                   m_bSort;
	BOOL                   m_bDropDown;
	CTPMenuEx             *m_pDropMenu;  
	CString                m_sToolTips;
	CRect                  m_rtComBtn;

	struct tag_StrItem
	{
		CString      sText;
		UINT         uID;
		DWORD_PTR    dwData;
		GUID         guidData;  
		void        *pData; 
		tag_StrItem()
		{
			sText    = _T("");
			uID      = 0;
			dwData   = 0;
			pData    = NULL;
			guidData = GUID_NULL;  
		}
	};
	CArray<tag_StrItem *,tag_StrItem *&>   m_aItem;
public:		
	void                    SetLBText(int iItem,CString sText);	
	void                    GetLBText(int iItem,CString &sText);		
	void                    ResetContent();
	int                     AddString(CString sText);
	void                    DeleteString(CString sText);
	void                    DeleteString(int iIndex);
	void					DeleteItem(int iItem); //by zhh
	int                     SetCurSel(int iSel);
	int                     GetCurSel();	
	int                     GetCount();
	int                     InsertString(int index,CString sText);
	void                    SetItemData(int index,DWORD_PTR dwData);
	DWORD_PTR               GetItemData(int index);
	void*                   GetItemDataPtr(int nIndex);
	int                     SetItemDataPtr(int nIndex, void* pData);
	void                    SetItemGUID(int index,GUID dwData);
	GUID                    GetItemGUID(int index);
	void                    SetToolTips(CString sToolTips); 
	void                    EnableToolTips(BOOL bEnable);
	void                    SetGetData(BOOL bGetData);  

	void                    EnableSelect(BOOL bEnable);
	void                    EnableSpecialSel(BOOL bEnable);
	
	static void             RegisterClass(BOOL bRegister);
protected:
	virtual void            SubWindow(BOOL bSub);
	void                    PaintButton(CDC *pDC,COMBOBOXINFO &pInfo);
	void                    DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	LRESULT                 FunNotifyParent(UINT nNotify,int iItem) ;
	void                    SortItem(); 

	void                    OnMenuCmd(UINT uID);
	void                    DropDown(); 	
	LRESULT                 OnCloseMenu(WPARAM wp,LPARAM lp);
	BOOL                    OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	int                     GetSelItem(CString sText);

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void PreSubclassWindow();	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void  OnDrawItem(int nIDCtl,LPDRAWITEMSTRUCT lpDrawItemStruct);
};