#pragma once


// CTPEdit
#define WM_TPEDIT   WM_USER+121

#define TPEDIT_RETURE       0x00000001
#define TPEDIT_PATHNAME     0x00000002
#define TPEDIT_DECIMAL      0x00000004
#define TPEDIT_CONTENTMENU  0x00000008

#define WC_NAMEEXCLUDE      _T(":\n*/\\<>'~!^#|?:\"&;,")


class CTPMenuEx;

class AFX_EXT_CLASS CTPEdit : public CEdit ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPEdit)

public:
	CTPEdit();
	virtual ~CTPEdit();
public:
	BOOL                    m_bClientRect;   //特殊用途，用户不要使用	
	BOOL                    m_bForceFocus;
	UINT                    m_uPrivateStyle;
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	TP_BORDER_STYLE         m_eBorderStyle;
	TPScrollBarData        *m_pScrollBar;
	CRect                   m_rtClient;
	CString                 m_sTextSave;
	UINT                     m_uState;

public:
	int                     GetInt();
	void                    SetInt(int iSet);

	HWND                    UnsubclassWindow();
	BOOL					ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags=0);

	static  void            PreTranslateFocus(UINT message, WPARAM wParam,LPARAM lParam);

protected:
	virtual void    SubWindow(BOOL bSub);
	LRESULT         OnMsgUnSubWindow(WPARAM wp,LPARAM lp);
	LRESULT         OnMsgScollBar(WPARAM wp,LPARAM lp);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnDestroy();
public:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	
protected:

public:
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};


