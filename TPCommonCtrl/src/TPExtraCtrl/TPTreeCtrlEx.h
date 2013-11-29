#pragma once

enum  TREE_BACK_INDEX
{
	TREE_BACK_CHILDSEL,
	TREE_BACK_CHILDUNSEL,
	TREE_BACK_ROOTSEL,
	TREE_BACK_ROOTUNSEL,
	TREE_BACK_TRINBTN,
	TREE_BACK_MAX,
};
// CTPTreeCtrlEx

#define WC_TREECTRLEX _L("TreeCtrlEx")

typedef CArray<HTREEITEM,HTREEITEM&> HTREEITEMList;

class AFX_EXT_CLASS CTPTreeCtrlEx : public CTPTreeCtrl
{
	DECLARE_DYNAMIC(CTPTreeCtrlEx)

public:
	CTPTreeCtrlEx();
	virtual ~CTPTreeCtrlEx();
protected:
	TPCommonRes                  *m_pBackRes[TREE_BACK_MAX];
	TPCommonRes                  *m_pPenRes[2];
	int                           m_iBottomLine;
	BOOL                          m_bBaseView;                 //使用基类的画图
	DWORD_PTR                     m_dwRootData;
	CArray<HTREEITEM,HTREEITEM&>  m_aMultiSel;
	HTREEITEM                     m_hShiftItem; 
public:
	void                    SetViewType(BOOL bBase);
	CImageList             *CreateDragImage(HTREEITEM hItem);

public:
	BOOL                    SetItemData(HTREEITEM hItem,DWORD_PTR dwData ); 
	DWORD_PTR               GetItemData(HTREEITEM hItem ) const; 
	virtual   COLORREF      GetItemTextColor(HTREEITEM hItem);

	BOOL                    GetSelectedItemMulti(HTREEITEMList &listItem,BOOL bDown = TRUE) ;	
	void                    EnableMultiSel(BOOL bEnable); 
	BOOL                    SelectItem(HTREEITEM hItem,BOOL bKeep = FALSE);
	BOOL                    ClearMultiSel(HTREEITEM hItem); 
	BOOL                    ClearMultiChild(HTREEITEMList &listItem); 
protected:
	virtual void            SubWindow(BOOL bSub);
	virtual void            DrawItem(CDC *pDC,HTREEITEM hItem,int iLevel);
	virtual BOOL            GetIsSelect(HTREEITEM hItem);	
	void                    SelectGroup(HTREEITEM hItem);
	void                    SelectShift(BOOL bUp);

	void                    OnRButtonDownEx(UINT nFlags, CPoint point);
	void                    OnRButtonUpEx(UINT nFlags, CPoint point);
	void                    OnTvnSelchangedEx(NMHDR *pNMHDR, LRESULT *pResult);
	void                    SetSelState();
public:
	static  void            RegisterClass(BOOL bRegister);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


