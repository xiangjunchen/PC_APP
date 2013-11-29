#pragma once


// CTPListCtrl

class AFX_EXT_CLASS CTPListCtrl : public CListCtrl ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPListCtrl)

public:
	CTPListCtrl();
	virtual ~CTPListCtrl();
public:
	BOOL                    m_bAscSort;
	int                     m_iColumSort;
protected:
	BOOL                    m_bSubWindow; 
	BOOL                    m_bCreate;
	TP_BORDER_STYLE         m_eBorderStyle;
	CTPHeaderCtrl          *m_pHeadCtrl;
	UINT                    m_uStyle;
	UINT                    m_uStyleEx;
	TPCommonRes             *m_pCheckBox;
	TPCommonRes             *m_pLinePen[3];
	CRect                   m_rtClient;
	CTPEdit                *m_pEdit;
	int                     m_iHeadHeight;
	int                     m_iBackColorIndex;
	BOOL                    m_bLine; 
	CImageList				m_ilRowHeight;
	COLORREF                m_clrBackColorOdd;
	COLORREF            	m_clrBackColorAcc;

protected:
	BOOL                    m_bItemTextColor;
	HIMAGELIST              m_hImageIcon; 
	HIMAGELIST              m_hImageSmallIcon;

	TPScrollBarData          *m_pScrollBar;
	SCROLLINFO                m_sVScrollInfo;
	SCROLLINFO                m_sHScrollInfo;
	CPoint                    m_ptScrollPos;
public:
	void            SelectItem(int iItem,BOOL bSelect); 
	BOOL            GetSelectItem(int iItem);
	void            SetItemTextColor(int iItem,TP_SYSTEM_COLOR clText);
	BOOL			SetItemHeight(int iRowHeight);
	void            SetRedraw(BOOL bRedraw);
	DWORD           SetExtendedStyle(DWORD dwNewStyle);	
	void            SetBackColorIndex(int iIndex);
	void            SetLine(BOOL bLine);

	void           SetColumnText(int iItem,LPCTSTR sText);
	void           SetColumnFmt(int iColumn,UINT uFlagAdd,UINT uFlagRemove);
	int            SetColumnFmtEx(int nCol, UINT uFlagAdd,UINT uFlagRemove);
	HWND           UnsubclassWindow();

	void           SetScrollPosKeep(BOOL bSave);  
	void           SortItemDefault(int iColumn);
protected:
	virtual void    DrawIcon(CDC *pDC,int iItem);	
	virtual void    DrawList(CDC *pDC,int iItem);	
	virtual void    DrawSmallIcon(CDC *pDC,int iItem);	
	virtual void    DrawReport(CDC *pDC,int iItem);

	virtual BOOL    DrawReport(CDC *pDC,int iItem,int iSubItem,CRect &rtSub);




	
protected:
	virtual void    SubWindow(BOOL bSub);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();	
	afx_msg void OnStyleChanged(int nStyleType,	LPSTYLESTRUCT lpStyleStruct);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


