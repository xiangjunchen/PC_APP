#pragma once

enum TABCTRL_INDEX
{
	TABCTRL_DIS,
	TABCTRL_NOR,
	TABCTRL_DOWN,
	TABCTRL_LINE,
}; 
// CTPTabCtrl

class AFX_EXT_CLASS CTPTabCtrl : public CTabCtrl,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPTabCtrl) 

public:
	CTPTabCtrl();
	virtual ~CTPTabCtrl();
public:
	BOOL                     m_bDrawFocus;
	CFont                   *m_pFont;
	COLORREF                 m_clTextColor;
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	TP_BORDER_STYLE         m_eBorderStyle;

	CTPSpinButtonCtrl      *m_pSpinButton;
	TPCommonRes             *m_pTabBmp[4];
	CRect                   m_rtSelItem;
	CImageList             *m_pImageList; 
	int                     m_iOffsetItem;
	BOOL                    m_bFoceUnPaint;
	int                     m_iBackColor;
	BOOL                    m_bForceALL;

public:
	LONG           InsertItem(int nItem, LPCTSTR lpszItem);
	void           SetItemText(int iIndex,CString lpszItem);
	void           SetFullDraw(BOOL bSet);
	int            GetMinHeight();

protected:
	virtual void    SubWindow(BOOL bSub);
	void            LoadBitmap();
	void            PaintTab(CDC *pDC);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void	OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
};