#pragma once

  
#define  WC_SCROLLBARZOOM _L("ScrollBarZoom")

 

class AFX_EXT_CLASS CTPScrollBarZoom : public CWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPScrollBarZoom)

public:
	CTPScrollBarZoom(BOOL bVert = FALSE);
	virtual ~CTPScrollBarZoom();
public:
	CWnd                  *m_pMsgWnd;               //������Ϣ�Ĵ���	
	BOOL                   m_bVert;

	long double	           m_dPixelPerFrame;       //ÿ֡����
	long double            m_dStartFrame;          //��ʼ֡
 
	 
  
	CRect                  m_rtClient;             //�ͻ���λ��


    //֧�ְ�ťZoom
	int                    m_iLeftEdge;
	int                    m_iRigthEdge;
 
 protected:

 	__int64                m_nMinFrame;            //��Сֵ          
	__int64                m_nMaxFrame;            //���ֵ 
 
	double                 m_dZoomIn;              //��ʾ֡�ٷֱ� 
	double                 m_dStartZoomIn;         //��ʼ֡�ٷֱ� 

	double                 m_dMinZoomIn;           //��С��ʾ�ٷֱ�	
	double                 m_dSaveStartZoomIn;     //��ʼ֡�ٷֱ� 
	double                 m_dSaveZoomIn;          //��ʾ֡�ٷֱ� 
	double	               m_dSavePixelPerFrame;   //ÿ֡����
	double 	               m_dSaveStatrFrame;      //��ʼ֡
 	BOOL                   m_bSaveClick;           //�ϴ��Ƿ�˫��  
	int                    m_iDBClickStep;         //˫��ʱ�Ĳ�ס
	int                    m_iTimeCodeWidth;

	int                    m_iBarWidth;
	int                    m_iBarHeadState;        //������ͷ��״̬
 
	CRect                  m_rtCursorArea;         //�����Է�Χ
	CRect                  m_rtLeftHead;           //���������ͷ��λ��
	CRect                  m_rtRightHead;          //�������ұ�ͷ��λ��
 	CRect                  m_rtBarHandle;          //�������в�λ��
	CRect                  m_rtTimeBar;            //ʱ��������λ��  
	CRect                  m_rtBarTotal;
 
	TP_DRAG_TYPE           m_enDragType;           //��קģʽ
	CPoint                 m_ptDrag;               //��ק�����
	__int64                m_iDrag;                //��קʱ��������
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
 

	virtual   CRect    GetTimeBarRect();          //������
	virtual   CRect    GetTimeBtnRect();          //������
 
 
 
 	void               SetStartFrame(long double dFrame,BOOL bPaint = TRUE);                                //��ʼλ��
 
 
	__int64            GetMaxFrame() { return m_nMaxFrame ;}
	__int64            GetMinFrame() { return m_nMinFrame ;}

	 

 
	void               SetMaxMinFrame(__int64 iMin,__int64 iMax,BOOL bReset = FALSE,BOOL bPaint = TRUE);
  

	void               SetStartZoomIn(double dZoomIn,BOOL bPaint = TRUE);
	void               SetZoomIn(double dPPF,BOOL bCenter = TRUE,BOOL bPaint = TRUE,double dCurPixel = 0.0);


	double             GetStartZoomIn();
	double             GetZoomIn();
	double             GetMinZoomIn(){return m_dMinZoomIn; }          //��С��ʾ�ٷֱ�	  
    double             GetViewFrame();
  
 

 	double             TranslateFrame(long double dFrame) const;     //֡   -> ����
	long double        TranslatePixel(double dPixel)      const;     //���� -> ֡

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


