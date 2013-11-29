#pragma once


// CTPPaneWnd
#define WC_PANEWND _L("PaneWnd")




#define HPANEITEM void *


class AFX_EXT_CLASS CTPPaneWnd : public CWnd,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPPaneWnd)

public:
	CTPPaneWnd();
	virtual ~CTPPaneWnd();
protected:
	CSize                                   m_sizeMin;
	CWnd                                   *m_pParentWnd; 
	HPANEITEM                               m_hRoot;
	CRect                                   m_rtRoot;
	int                                     m_iEdgeSize;
	CDC                                    *m_pDCTemp;
	TPCommonRes                             *m_pResCursor[2];


	HPANEITEM                               m_hDragItem;
	CPoint                                  m_ptSave;
	UINT                                    m_eDragMode;
	CRect                                   m_rtInvert;
	CRect                                   m_rtClipRect;

private:
	BOOL                                    m_bSubWindow;
	BOOL                                    m_bCreate;	
	TP_BORDER_STYLE                         m_eBorderStyle;

public:
	virtual   BOOL Create(CWnd* pParentWnd,DWORD dwStyle = WS_CHILD | WS_VISIBLE,UINT nID = AFX_IDW_PANE_FIRST );

	void           SetMinSize(SIZE sizeMin);
	void           SetRect(HPANEITEM  hItem,CRect rtItem);
	void           ShowItem(HPANEITEM hItem,int iItem,BOOL bShow);	
	void           SetItemSize(HPANEITEM hItem,int iItem,int iSize);	
	void           SetItemRate(HPANEITEM hItem,int iItem,double dRate);
	void           SetItemBarSize(HPANEITEM hItem,int iItem,int iBarSize);	
	
	int            GetItemCount(HPANEITEM hItem);
	HPANEITEM      GetItem(HPANEITEM hItem,int iPos);
	void           InsertItem(HPANEITEM hItem,int iInsertPos = -1,int iSubItemCount = 0,BOOL bVert = TRUE);
	void           DeleteItem(HPANEITEM hItem,int iPos);

	void           InsertItemWnd(HPANEITEM hItem,CWnd *pWnd,BOOL bAutoVScroll = FALSE,BOOL bAutoHScroll =FALSE ,BOOL bAudoDel = FALSE,int iInsertPos = -1);
	void           DeleteItemWnd(CWnd *pWnd,HPANEITEM hItem = NULL);	
	CWnd          *GetItemWnd(HPANEITEM hItem,int iPos);
	void           ShowItemWnd(CWnd *pWnd,HPANEITEM hItem = NULL);
	HPANEITEM      GetWndItem(CWnd *pWnd,HPANEITEM hItem = NULL);

private:
	virtual  void  SubWindow(BOOL bSub);
	void           PaintItem(HPANEITEM hItem,CDC *pDC);  
	int            GetMinSize(HPANEITEM  hItem,BOOL bVert);
	void           SetItemSize(HPANEITEM  hItem,int iItem,int iSize,BOOL bTopLeft);
	BOOL           HitTest(CPoint point, HPANEITEM &hItem,BOOL &bBar,HPANEITEM hTestItem = NULL); 
	HPANEITEM      GetSiblingItem(HPANEITEM hItem,int iNext);
	int            GetItemIndex(HPANEITEM hParentItem,HPANEITEM hItem);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
public:
	static void     RegisterClass(BOOL bRegister = TRUE);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


