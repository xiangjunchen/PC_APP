#pragma once


// CTPComboTBox
class AFX_EXT_CLASS CTPComboTBase
{
	public:
		CTPComboTBase();
public:
	BOOL              m_bButtonPress;
	CWnd             *m_pOwnerWnd;
	BOOL              m_bDropDown;  
	TPScrollBarData  *m_pScrollData;
	BOOL              m_bTreeCtrl;
	BOOL              m_bSetCapture;
	CTPTreeCtrl      *m_pTreeCtrl;
protected:
	CWnd             *m_pMouseWnd;	
};

class AFX_EXT_CLASS CTPComboTBox : public CTPTreeCtrl,public CTPComboTBase
{
	DECLARE_DYNAMIC(CTPComboTBox)

public:
	CTPComboTBox();
	virtual ~CTPComboTBox();

 private:
	BOOL              m_bTestSelect;
	HTREEITEM         m_SelItem;

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
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


