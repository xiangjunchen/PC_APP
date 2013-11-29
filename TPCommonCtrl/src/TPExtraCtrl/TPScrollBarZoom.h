#pragma once

  
#define  WC_SCROLLBARZOOM _L("ScrollBarZoom")

 

class AFX_EXT_CLASS CTPScrollBarZoom : public CWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPScrollBarZoom)

public:
	CTPScrollBarZoom(BOOL bVert = FALSE);
	virtual ~CTPScrollBarZoom();
public:
	CWnd                  *m_pMsgWnd;               //发送消息的窗口	
	BOOL                   m_bVert;

	long double	           m_dPixelPerFrame;       //每帧象素
	long double            m_dStartFrame;          //起始帧
 
	 
  
	CRect                  m_rtClient;             //客户区位置


    //支持按钮Zoom
	int                    m_iLeftEdge;
	int                    m_iRigthEdge;
 
 protected:

 	__int64                m_nMinFrame;            //最小值          
	__int64                m_nMaxFrame;            //最大值 
 
	double                 m_dZoomIn;              //显示帧百分比 
	double                 m_dStartZoomIn;         //起始帧百分比 

	double                 m_dMinZoomIn;           //最小显示百分比	
	double                 m_dSaveStartZoomIn;     //起始帧百分比 
	double                 m_dSaveZoomIn;          //显示帧百分比 
	double	               m_dSavePixelPerFrame;   //每帧象素
	double 	               m_dSaveStatrFrame;      //起始帧
 	BOOL                   m_bSaveClick;           //上次是否双击  
	int                    m_iDBClickStep;         //双击时的不住
	int                    m_iTimeCodeWidth;

	int                    m_iBarWidth;
	int                    m_iBarHeadState;        //滑动条头部状态
 
	CRect                  m_rtCursorArea;         //鼠标测试范围
	CRect                  m_rtLeftHead;           //滑动条左边头部位置
	CRect                  m_rtRightHead;          //滑动条右边头部位置
 	CRect                  m_rtBarHandle;          //滑动条中部位置
	CRect                  m_rtTimeBar;            //时间线数据位置  
	CRect                  m_rtBarTotal;
 
	TP_DRAG_TYPE           m_enDragType;           //拖拽模式
	CPoint                 m_ptDrag;               //拖拽保存点
	__int64                m_iDrag;                //拖拽时保存数据
	CRect                  m_rtDrag;


	BOOL                   m_bWndEnable;
	HCURSOR                m_hSetCursor;
 
 
	TPCommonRes            *m_pResCursor[TL_MAX];
	TPCommonRes            *m_pResBar;
	TPCommonRes            *m_pResBarBack;
	TPCommonRes            *m_pResBtn[3];
	CRect                   m_rtBtn[3];
	int                     m_iBtnState;
	
 private:
	BOOL                   m_bSubWindow;
	BOOL                   m_bCreate;	


public:
	BOOL               Create(DWORD dwStyle,const RECT& rect,CWnd* pParentWnd, UINT nID = 0);
 

	virtual   CRect    GetTimeBarRect();          //滑动条
	virtual   CRect    GetTimeBtnRect();          //滑动条
 
 
 
 	void               SetStartFrame(long double dFrame,BOOL bPaint = TRUE);                                //起始位置
 
 
	__int64            GetMaxFrame() { return m_nMaxFrame ;}
	__int64            GetMinFrame() { return m_nMinFrame ;}

	 

 
	void               SetMaxMinFrame(__int64 iMin,__int64 iMax,BOOL bReset = FALSE,BOOL bPaint = TRUE);
  

	void               SetStartZoomIn(double dZoomIn,BOOL bPaint = TRUE);
	void               SetZoomIn(double dPPF,BOOL bCenter = TRUE,BOOL bPaint = TRUE,double dCurPixel = 0.0);


	double             GetStartZoomIn();
	double             GetZoomIn();
	double             GetMinZoomIn(){return m_dMinZoomIn; }          //最小显示百分比	  
    double             GetViewFrame();
  
 

 	double             TranslateFrame(long double dFrame) const;     //帧   -> 象素
	long double        TranslatePixel(double dPixel)      const;     //象素 -> 帧

protected:

	void            SetSize( int cx, int cy);
	void            SetDBClick();

	void            GetMiniZoomIn();

	virtual void    PaintTimeBar(CDC *pDC);
	 

 public:
	static   void   RegisterClass(BOOL bRegister = TRUE);

 
protected:
	virtual  void   SubWindow(BOOL bSub);
 
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
 	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
 	afx_msg void OnEnable(BOOL bEnable);
  	afx_msg void OnDestroy();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
 	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
 };


