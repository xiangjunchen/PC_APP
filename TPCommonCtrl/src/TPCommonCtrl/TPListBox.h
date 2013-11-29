#pragma once


// CTPListBox

#define LB_SUBCOMBOBOX         0x0100
#define LB_OWNERIMAGE          0x0001
#define LB_SZIMAGE             0x0002
#define LB_MOUSEUP             0x0004
#define LB_MOUSEDOWN           0x0008
#define LB_MOUSESELECT         0x0010

class CTPComboBox;

class AFX_EXT_CLASS CTPListBox : public CListBox ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPListBox)

public:
	CTPListBox();
	virtual ~CTPListBox();
public:
	int                     m_iSparteBar;
	BOOL                    m_bForceNoDraw;
	int                     m_iMaxWidth;
	UINT                    m_uState;	

protected:
	BOOL                    m_bSubWindow; 
	BOOL                    m_bCreate;
	UINT                    m_uStyle;
	TP_BORDER_STYLE         m_eBorderStyle;
	TPScrollBarData        *m_pScrollBar;
	SCROLLINFO              m_sVScrollInfo;
	SCROLLINFO              m_sHScrollInfo;


	BOOL                    m_bCheckBox; 
	CImageList             *m_pImageList;
	struct _tagItem
	{
		DWORD               dwCheckData;
		int                 iImageData;
		LPBYTE              pIcon;
		CSize               szIcon;
		BOOL                bSaveIcon;
		COLORREF            clClolor;
		BOOL                bItemEnable;
		_tagItem(){dwCheckData=0;iImageData=-1;pIcon=NULL;szIcon = CSize(0,0);bSaveIcon = FALSE;clClolor = 0;bItemEnable = TRUE;}
		~_tagItem(){if(pIcon && bSaveIcon) delete pIcon;pIcon=NULL;szIcon=CSize(0,0);bSaveIcon = FALSE;}
	};	
	CArray<_tagItem *,_tagItem *&>  m_aItemData;

	TPCommonRes             *m_pCheckBox;
	BOOL                     m_bFocus; 
	UINT                     m_uKeyType;
private:
	virtual void            SubWindow(BOOL bSub);
public:
//可以设置Check 状态
	void                    EnableCheck(BOOL bEnable);
	BOOL                    GetCheck(int iItem);
	void                    SetCheck(int iItem,BOOL bCheck);
	void                    SetItemColor(int iItem,COLORREF clItem);
//可以设置显示图标
	void                    SetImageList(CImageList *pImageList);
	CImageList*				GetImageList();
	int                     GetImage(int iIndex);
	void                    SetImage(int iIndex,int iImage);
	void                    SetIcon(int iIndex,LPBYTE pBuffer,CSize szBuffer,BOOL bSave);
	void                    EnableToolTips(BOOL bEnable);
	void                    EnableItem(int iItem,BOOL bEnable);
	int                     SelectCheck(CTPComboBox *pComboBox = NULL,int iSelCheck = -1);

	void					InvalidateScrollBar(UINT uType = 0); 
	int                     GetEnableItem(int iItem,BOOL bUp); //modify by chenxiaolong for Getting next enabled item.

	void                    SetLBText(int iItem,CString sText);
protected:
	BOOL                    OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
private:
	LRESULT                 OnAddString(WPARAM wp,LPARAM lp);
	LRESULT                 OnDelString(WPARAM wp,LPARAM lp);
	LRESULT                 OnInsertString(WPARAM wp,LPARAM lp);
	LRESULT                 OnResetContent(WPARAM wp,LPARAM lp);
	LRESULT                 OnSetCurSel(WPARAM wp,LPARAM lp);	
	LRESULT                 OnGetCurSel(WPARAM wp,LPARAM lp);
	virtual void            DrawText(CDC *pDC,CRect &rtItem,CString &sText);   
	BOOL                    GetIsEnable(CPoint &pt);
	//int                     GetEnableItem(int iItem,BOOL bUp); 
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
protected:
	virtual void PreSubclassWindow();
public:

	afx_msg void OnNcPaint();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg int  CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


