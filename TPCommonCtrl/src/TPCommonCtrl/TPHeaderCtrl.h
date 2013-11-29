#pragma once


// CTPHeaderCtrl

class AFX_EXT_CLASS CTPHeaderCtrl : public CHeaderCtrl ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPHeaderCtrl)

public:
	CTPHeaderCtrl();
	virtual ~CTPHeaderCtrl();
private:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	TPCommonRes             *m_pHeaderBmp[5]; 	
	int                     m_iOver;
	int                     m_iDown;
	UINT_PTR                m_hTimer;

public:
	void                    SetSortFlag(int iColumn,UINT uFlag);
	int                     HitTest(CPoint point);
	void                    SetColumnFmt(int iColumn,UINT uFlagAdd,UINT uFlagRemove);
	HWND                    UnsubclassWindow();
private:
	virtual void            SubWindow(BOOL bSub);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


