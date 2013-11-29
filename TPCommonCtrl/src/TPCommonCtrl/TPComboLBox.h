#pragma once


// CTPComboLBox
class CTPToolTipCtrl;
class AFX_EXT_CLASS CTPComboLBox : public CTPListBox
{
	DECLARE_DYNAMIC(CTPComboLBox)

public:
	CTPComboLBox();
	virtual ~CTPComboLBox();
public:
	BOOL               m_bSimple;
	CWnd              *m_pComboBoxEx; 
private:
	CWnd              *m_pMouseScrollBar;	
	CTPToolTipCtrl    *m_pToolTips;
public:
	void               EnableToolTips(BOOL bEnable);
private:
	LRESULT            OnPrint(WPARAM wp,LPARAM lp);
	LRESULT            OnShowScrollBar(WPARAM wp,LPARAM lp);
protected:
	BOOL               OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
protected:
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


