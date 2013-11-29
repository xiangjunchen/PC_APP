#pragma once


// CTPComboDTBox

class AFX_EXT_CLASS CTPComboDTBox : public CTPDirTreeCtrl,public CTPComboTBase
{
	DECLARE_DYNAMIC(CTPComboDTBox)

public:
	CTPComboDTBox();
	virtual ~CTPComboDTBox();
public:
	void              SetTreeCtrl(BOOL bSet);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


