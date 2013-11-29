#pragma once

#define WC_TPRECTTRACKER _L("TPRectTracker")

#define WM_TPRECTTRACKER  WM_USER + 1001

class AFX_EXT_CLASS CTPRectTracker : public CTPWnd , public CRectTracker,public CTPBaseCtrl 
{
// Construction
public:
	CTPRectTracker();

	// Attributes
public:	
	TP_BORDER_STYLE   m_eBorderStyle;
	BOOL              m_bChangeSize;

	
private:
	CRect             m_rtRange;
	CRect             m_rtInit;
	// Operations
public:
	BOOL            Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0);
	void			SetTrackRect(POINT point,RECT rt);
	CRect			GetTrackRect();
	void            SetRectRange(CRect rtRange);

	static void     RegisterClass(BOOL bRegister);
private:
	void            OnChangedRect(const CRect& rectOld);
	void            AdjustRect(int nHandle, LPRECT pRect);


private:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPRectTracker)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTPRectTracker();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTPRectTracker)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
