#pragma once


// CTPSplitterBar

#define WC_SPLITTERBAR _L("SplitterBar")

class AFX_EXT_CLASS CTPSplitterBar : public CWnd,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPSplitterBar)

public:
	CTPSplitterBar();
	virtual ~CTPSplitterBar();
public:	
	CPoint                  m_ptRange;
	int                     m_iLineType;
private:
	BOOL              m_bHorz; 	
	BOOL              m_bDrag;
	CPoint            m_ptDrag;
	CPoint            m_ptDragSave;

	CRect             m_rtDragBar;
	TPCommonRes       *m_pResCursor;
	TP_BORDER_STYLE   m_eBorderStyle;
	
private:
	BOOL          m_bSubWindow;
	BOOL          m_bCreate;	

public:
	BOOL           Create(BOOL bHorz,UINT nStyle,RECT &rtWnd,CWnd *pParent,UINT nID =0);
	void           SetDirection (BOOL bHorz);
	void		   SetMoveRange(INT iMaxValue, INT iMinValue);
private:
	virtual  void  SubWindow(BOOL bSub);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
protected:
	virtual BOOL    PreCreateWindow(CREATESTRUCT& cs);
	virtual void    PreSubclassWindow();

	afx_msg LRESULT OnNcHitTest(CPoint point);
public:
	static void     RegisterClass(BOOL bRegister = TRUE);

	afx_msg void OnNcPaint();
};


