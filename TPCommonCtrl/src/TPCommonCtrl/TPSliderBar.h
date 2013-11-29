#pragma once

#define SLIDER_CHANNEL 0x0000001
#define SLIDER_THUMB   0x0000002



enum SLIDER_INDEX
{
	SLIDER_OVER,
	SLIDER_DIS,
	SLIDER_NOR,
	SLIDER_DOWN,

	SLIDER_THUMB_OVER,
	SLIDER_THUMB_DIS,
	SLIDER_THUMB_NOR,
	SLIDER_THUMB_DOWN,

	SLIDER_BACK,

	SLIDER_MAX,
};
#define  TP_SLIDERBAR_NOBACK 0x00000001

// CTPSliderBar
#define WC_SLIDERBAR _L("SliderBar")

class AFX_EXT_CLASS   CTPSliderBar : public CSliderCtrl ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPSliderBar)

public:
	CTPSliderBar();
	virtual ~CTPSliderBar();
public:
	CWnd                   *m_pMsgWnd ;
	int                     m_iChanneOffset;
	BOOL                    m_bFoceHideBar;

	BOOL                    m_bPressed;
	BOOL                    m_iMicType;
	BOOL                    m_iDimLen;
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	TP_BORDER_STYLE         m_eBorderStyle;
	int                     m_iBackColor;
	UINT                    m_uState;
	BOOL                    m_bPaint; 


	TPCommonRes             *m_pThumbBmp[9];
	TPCommonRes              *m_pBackRes;

	TPCommonRes             *m_pTicLinePen;


	int                     m_iPage;
	int                     m_iPagePixel;
	int                     m_iMin;
	int                     m_iMax;
	int                     m_iPos;
	double                  m_dAngle;
	int                     m_iTopLeft;
	int                     m_iRightBottom;
	int                     m_iBarSize;

	BOOL                    m_bOver;

	CRect                   m_rtThumb;
	CRect                   m_rtThumbHandle;
	CRect                   m_rtChannel;
	CRect                   m_rtClient;

	int                    m_iDragMode;          //拖拽模式
	CPoint                 m_ptDragSave;         //拖拽保存点
	int                    m_iDragSave    ;      //拖拽时点与左边距离
	UINT_PTR               m_hTimerHover;
	BOOL                   m_bReDraw;
	CPoint                 m_ptDrag;
	DWORD                  m_dwDragTime;

	double                *m_pdStepTicks;
	double                *m_pdStepValue;
	int                    m_iStepCount;
public:

	int             GetPageSize() const;
	void            SetPageSize(int iPageSize,BOOL bPaint = TRUE);

	void            SetEdge(int iTopLeft,int iRightBottom); 

	void            SetBackRes(CString sResPath);

	void            SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);

	TPCommonRes    *GetRes(SLIDER_INDEX eIndex);


	static void     RegisterClass(BOOL bRegister = TRUE);

	BOOL            Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,UINT nID);

	void            SetStepValue(double *pStepValue,long lStepCount,double *pdStepTicks = NULL);
protected:
	virtual void    SubWindow(BOOL bSub);
	virtual void    LoadThumb();
	virtual void    PaintCtrl(CDC *pDC);
	virtual void    PaintTicks(CDC *pDC);
	virtual void    GetChannelRect(CRect &rtChannel);

	virtual int     PixelToPos(int iPixel);   
	int             PosToPixel(int iPos);   
	virtual void    CalcSize(UINT uFlag);
	virtual int     GetMicroSetp(int iPos,CPoint &ptCur);	
    virtual void    SetCursorPosition(CPoint &ptCur);
	
	double          PosToDB(double iPos);
	double          DBToPos(double dDB);
	double          DBToPosComplex(double dDB);
	static  double  DBAlign(double dDB,int iDimLen);
	LRESULT         OnMsgUnSubWnd(WPARAM wp,LPARAM lp);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
};


