#pragma once


// CTPWnd
#define WC_WND _L("CTPWnd")
class AFX_EXT_CLASS CTPWnd : public CTabCtrl
{
	DECLARE_DYNAMIC(CTPWnd)

public:
	CTPWnd();
	virtual ~CTPWnd();
public:
	static void   RegisterClass(BOOL bRegister);
	BOOL          Create(DWORD dwStyle,const RECT &rtWnd,CWnd *pParent,UINT nID);

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


