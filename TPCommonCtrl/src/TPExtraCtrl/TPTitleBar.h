#pragma once


// CTPTitleBar

class AFX_EXT_CLASS CTPTitleBar : public CWnd ,public CTPBaseFrame
{
	DECLARE_DYNAMIC(CTPTitleBar)

public:
	CTPTitleBar();
	virtual ~CTPTitleBar();
public:
	CWnd  *m_pMsgWnd;
public:
	int    GetBarHeight();
    void   SetWindowText(LPCTSTR lpszString);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

protected: // by zhh on 2006-04-02
	CString			m_sToolTips;
	CTPToolTipCtrl*	m_pShowTips;

public:
	void SetToolTips( LPCTSTR sText);
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult);
};


