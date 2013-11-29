#pragma once


// CTPCaseWnd

class AFX_EXT_CLASS CTPCaseWnd : public CWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPCaseWnd)

public:
	CTPCaseWnd();
	virtual ~CTPCaseWnd();
public:	
	CWnd                   *m_pSubWnd;
	int                     m_iBackColor;
	BOOL                    m_bAutoH;
	BOOL                    m_bAutoV;

	BOOL                    m_bSameH;
	BOOL                    m_bSameV;

private:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	TP_BORDER_STYLE         m_eBorderStyle;

private:
	BOOL                    m_bAutoDel;
	CWnd                   *m_pParentWnd;  
public:
	void            SetSubWnd(CWnd *pWnd,BOOL bAutoV,BOOL bAutoH,BOOL bAtuoDel = FALSE,long lXOffset = 0,long lYOffset = 0); 	
private:
	virtual void    SubWindow(BOOL bSub);
	void            MoveSubWnd();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
typedef CArray <CTPCaseWnd *,CTPCaseWnd *&> CTPCaseWndArray;


