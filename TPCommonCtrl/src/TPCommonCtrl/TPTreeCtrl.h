#pragma once


// CTPTreeCtrl
#define  TREE_SUBCOMBOBOX 0x00000001
#define  STATE_NOCAVE     0x00000002
#define  STATE_SETDATA    0x00000004
#define  STATE_MULTISEL   0x00000008
#define  STATE_DELITEM    0x00000010
#define  STATE_EDITITEM   0x00000020

#define HTREEEMPTY       ((HTREEITEM)-1)

class AFX_EXT_CLASS CTPTreeCtrl : public CTreeCtrl ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPTreeCtrl)

public:
	CTPTreeCtrl();
	virtual ~CTPTreeCtrl();
public:
	BOOL                    m_bDrawFocus;  
	CWnd                  **m_pFocusWnd;  

	TPScrollBarData        *m_pScrollBar;
	UINT                    m_uState; 
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	TP_BORDER_STYLE         m_eBorderState;
	TPCommonRes             *m_pCheckBox;
	TPCommonRes             *m_pIndent;
	TPCommonRes             *m_pLinePen;		
	HTREEITEM               m_hDownItem; 
	CRect                   m_rtClient;
	int                     m_iBackColor;  
	UINT                    m_uStyle;
	SCROLLINFO              m_sVScrollInfo;
	SCROLLINFO              m_sHScrollInfo;
	CTPEdit                *m_pEdit;
	long                    m_lEditLimit;
	HTREEITEM               m_hEditItem;
	BOOL                    m_bCave;
	BOOL                    m_bReDraw;
	HTREEITEM               m_hRightSelect; 
	BOOL                    m_bFous;
	BOOL                    m_bLeftDowm;
	int                     m_iCheckLevel;
	BOOL                    m_bDownSelect;
public:

	void                    SetReDraw(BOOL bReDraw); 
	void                    SetCheckLevel(int iLevel,BOOL bDownSelect = FALSE);
	BOOL					ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags=0);

protected:
	virtual void            SubWindow(BOOL bSub);
	virtual void            DrawItem(CDC *pDC,HTREEITEM hItem,int iLevel);
	virtual BOOL            GetItemIcon(HTREEITEM hItem,LPBYTE &pBuf,CSize &szIcon,BOOL &bOverTurn);
	virtual BOOL            GetItemTextColor(HTREEITEM hItem,COLORREF &clText);
	virtual BOOL            GetIsSelect(HTREEITEM hItem);
	virtual BOOL            GetIsCurrent(HTREEITEM hItem);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnDestroy();
protected:
public:	
	afx_msg void OnNcPaint();
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg HBRUSH CtlColor(CDC *pDC, UINT nCtlColor);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


