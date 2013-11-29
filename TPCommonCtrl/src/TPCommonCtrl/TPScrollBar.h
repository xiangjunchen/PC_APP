
#pragma once

#define WM_SCROLLBAR WM_USER + 2345

#define	TP_SCROLLBAR_MAX 17
class AFX_EXT_CLASS  CTPScrollBar : public CScrollBar ,public CTPBaseCtrl 
{
// Construction
public:
	CTPScrollBar();
	virtual ~CTPScrollBar();

// Attributes
public:
	CWnd                  *m_pMsgWnd;       	//发送消息窗口	
	BOOL                   m_bSetCapture;       //不能抢夺Capture控制	
	BOOL                   m_bSetScrollInfo;
	BYTE                   m_bAttachBar; 
	INT64                  m_iDragPos;          //拖动位置

protected:
	INT64 	               m_nMinVisual;        //最小值          
	INT64                  m_nMaxVisual;        //最大值 
	INT64                  m_iPageSize;         //滑动条
	INT64                  m_iCurPos;           //当前位置 
	INT64                  m_iOldPos;           //上次位置

	int                    m_iSetTimer;         //定时器记数
	UINT_PTR               m_hTimerHover;     
	UINT_PTR               m_hTimerAcce;
	BOOL                   m_bVert;             //垂直滚动条
	BOOL                   m_bHover;
	UINT                   m_nArrowFlags;

	CRect                  m_rtLeftTopArrow;     //箭头位置
	CRect                  m_rtRightBottomArrow; //箭头位置
	CRect                  m_rtScrolBar;         //滚动条位置
	CRect                  m_rtClient;           //客户区位置                              
    int                    m_iBarPage;           //滑动条高度或宽度
	
	

	int                    m_iDragMode;          //拖拽模式
	CPoint                 m_ptDragSave;         //拖拽保存点
	int                    m_iDragLeftSave;      //拖拽时点与左边距离
	DWORD                  m_dwTimeTickSave  ;   //拖动响应时间间隔

	
	TPCommonRes            *m_aBmpres[TP_SCROLLBAR_MAX];        //
	TPCommonRes            *m_pResBar;
	TPCommonRes            *m_pResBarBack;
	BOOL                    m_bTLFace;
	BOOL                   m_bSubWindow;
	BOOL                   m_bCreate;
	BOOL                   m_bReDraw;
	UINT                   m_uStyle; 
	UINT                   m_uState;
	CPoint                 m_ptDrag; 
	UINT                   m_uScrollMode;
	int                    m_iScollLine;
public:

	// Attributes
	int          GetScrollPos() const;
	INT64        SetScrollPos(INT64 nPos, BOOL bRedraw = TRUE);
	void         GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const;
	void         SetScrollRange(INT64 nMinPos, INT64 nMaxPos, BOOL bRedraw = TRUE);
	void         ShowScrollBar(BOOL bShow = TRUE);

	BOOL         EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH);

	BOOL         SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
	BOOL         GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
	int          GetScrollLimit();
	BOOL         SetScrollMode(UINT uMode);
	BOOL         SetScrollLine(UINT lLine);

	void         SetReDraw(BOOL bReDraw); 
	void         SetTLFace(BOOL bSet);


	INT64        GetDragPos(); 
#if(_WIN32_WINNT >= 0x0501)
	BOOL         GetScrollBarInfo(SCROLLBARINFO *pScrollInfo) const;
#endif 

	void         SetScroll(UINT nSBCode, UINT nPos);
protected:
	virtual void            GetScrollBarRect( CRect &rtBar);           
	virtual void            DrawScrollBar(CDC *pDC);                   
	virtual void            DrawDC(CDC *pDC);                          
	virtual void            SubWindow(BOOL bSub = TRUE);

	void                    LoadThumb();

private:
	INT64                   PixelToPos(int iPixel);	
	int                     PosTpPixel(INT64 iPos);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPScrollBar)
	public:
//	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CTPScrollBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

