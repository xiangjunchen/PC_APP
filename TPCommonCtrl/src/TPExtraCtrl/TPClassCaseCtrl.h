#pragma once

enum TPCLASSCASE_INDEX
{
	TPCLASSCASE_BARUNSEL,
	TPCLASSCASE_BARSEL,
	TPCLASSCASE_CMDBTN,	
	TPCLASSCASE_MAX,
};

#define   TP_SHOWSTATE_SHOW       0x00000001
#define   TP_SHOWSTATE_EXPAND     0x00000002
#define   TP_SHOWSTATE_SINGLE     0x00000004
#define   TP_SHOWSTATE_MAIN       0x00000008
#define   TP_SHOWSTATE_SAMEH      0x00000010
#define   TP_SHOWSTATE_SAMEV      0x00000020


#define   CLASSCASE_STATE_VERT    0x00000001  //垂直模式
#define   CLASSCASE_STATE_CAPTURE 0x00000002  //当前鼠标锁定
#define   CLASSCASE_STATE_DROPMENU 0x00000004  //expend menu
#define   CLASSCASE_STATE_CLOSE    0x00000008  //close 符号

#define WC_CLASSCASECTRL _L("ClassCaseCtrl")

#define HCLASSCASEITEM   void *

#define CLASSCASECTRL_SELCHANGED      0x00000001
#define CLASSCASECTRL_DBCLICK         0x00000002
#define CLASSCASECTRL_SELCHANGCLICK   0x00000004

// CTPClassCaseCtrl
class AFX_EXT_CLASS CTPClassCaseCtrl : public CWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPClassCaseCtrl)

public:
	CTPClassCaseCtrl();
	virtual ~CTPClassCaseCtrl();
private:
	BOOL                                    m_bSubWindow;
	BOOL                                    m_bCreate;	
	TP_BORDER_STYLE                         m_eBorderStyle;

	HCLASSCASEITEM                          m_aItemRoot;
	HCLASSCASEITEM                          m_hSelItem;	
	HCLASSCASEITEM                          m_hSingleItem;	


	TPCommonRes                             *m_pResBar[TPCLASSCASE_MAX];
	TPCommonRes                             *m_pCursor[2];
	UINT                                    m_uState;
	

	HCLASSCASEITEM                          m_hDragItem;
	HCLASSCASEITEM                          m_hDropItem;	
	int                                     m_iDropPos;
	CPoint                                  m_ptDrag;
	UINT                                    m_uDragState;
	CRect                                   m_rtClipRect;
	UINT                                    m_uLButtonCmd;
	int                                     m_iDropLine;
	
	int                                     m_iDragLeft;
	CImageList                             *m_pDragImageList; 


public:
	HCLASSCASEITEM     InsertWnd(CString sCaption,CWnd *pWnd,int iWndHeight ,int iIndex = -1,BOOL bAutoHorz = FALSE,BOOL bAutoVert = FALSE,BOOL bAutoDel = FALSE);
	void               RemoveWnd(CWnd *pWnd);
	int                GetWnd(CWnd *pWnd);
	void               SetWndHeight(CWnd *pWnd,int iWndHeight);
	void               SetWndText(CWnd *pWnd,LPCTSTR sText);

	void               SetSize(); 
	void               SetCurItem(int iPos );
	void               SetCurItem(CWnd *pWnd );
	int                GetSelItem();	
	CWnd              *GetSelWnd();
	void               ShowItem(CWnd *pWnd,BOOL bShow);
	void               ExpandItem(CWnd *pWnd,BOOL bShow);
	void               SetItemState(CWnd *pWnd,DWORD dwAdd,DWORD dwRemove);
	void               EnableItem(CWnd *pWnd,BOOL bEnable);

	void			   SetStateMode(DWORD dwAdd,DWORD dwRemove);
 private:
	void               ExpandBar(HCLASSCASEITEM pWnd,BOOL bExpand);
public:
	static void       RegisterClass(BOOL bRegister = TRUE);

protected:
	virtual  void  SubWindow(BOOL bSub = TRUE);

	void           PaintBar(CDC *pDC,HCLASSCASEITEM hItem);
	BOOL           HitTest(CPoint ptTest,HCLASSCASEITEM &hItem,UINT &uCmd);
	HCLASSCASEITEM GetPreItem(HCLASSCASEITEM hItem);
	HCLASSCASEITEM GetNextItem(HCLASSCASEITEM hItem);
	HCLASSCASEITEM GetDropItem(int &iPos);
	void           AppendMenu(CTPMenuEx *pSubMenu);

	void           OnMenuCmd(UINT nID);
	void           SetMainSize();
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


