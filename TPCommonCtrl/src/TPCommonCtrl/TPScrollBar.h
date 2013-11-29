
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
	CWnd                  *m_pMsgWnd;       	//������Ϣ����	
	BOOL                   m_bSetCapture;       //��������Capture����	
	BOOL                   m_bSetScrollInfo;
	BYTE                   m_bAttachBar; 
	INT64                  m_iDragPos;          //�϶�λ��

protected:
	INT64 	               m_nMinVisual;        //��Сֵ          
	INT64                  m_nMaxVisual;        //���ֵ 
	INT64                  m_iPageSize;         //������
	INT64                  m_iCurPos;           //��ǰλ�� 
	INT64                  m_iOldPos;           //�ϴ�λ��

	int                    m_iSetTimer;         //��ʱ������
	UINT_PTR               m_hTimerHover;     
	UINT_PTR               m_hTimerAcce;
	BOOL                   m_bVert;             //��ֱ������
	BOOL                   m_bHover;
	UINT                   m_nArrowFlags;

	CRect                  m_rtLeftTopArrow;     //��ͷλ��
	CRect                  m_rtRightBottomArrow; //��ͷλ��
	CRect                  m_rtScrolBar;         //������λ��
	CRect                  m_rtClient;           //�ͻ���λ��                              
    int                    m_iBarPage;           //�������߶Ȼ���
	
	

	int                    m_iDragMode;          //��קģʽ
	CPoint                 m_ptDragSave;         //��ק�����
	int                    m_iDragLeftSave;      //��קʱ������߾���
	DWORD                  m_dwTimeTickSave  ;   //�϶���Ӧʱ����

	
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

