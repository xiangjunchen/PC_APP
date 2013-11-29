#pragma once

// CTPTimeLine
enum TP_TIMELINE_CMD
{
	TIME_CHANGE_STARTIN,
	TIME_CHANGE_ZOOMIN,
	TIME_DOUBLE_CLICK,
	TIME_CHANGE_TRIMIN,
	TIME_CHANGE_TRIMINING,
	TIME_CHANGE_TRIMOUT,
	TIME_CHANGE_TRIMOUTING,
	TIME_CHANGE_SEEK,
	TIME_CHANGE_SEEKING,
	TIME_CHANGE_SEEKBEGIN,
	TIME_CHANGE_SEEKEND,
	TIME_CHANGE_MOVEMARKPOINT,
	TIME_CHANGE_TRIMING,
	TIME_CHANGE_TRIMBEGIN,
	TIME_CHANGE_JOGSHUTTLESTATE,
	TIME_CHANGE_JOGSHUTTLEDIRECTION,
	TIME_CHANGE_JOGSHUTTLEGETSTATE,
	TIME_CHANGE_MARKCOLOR,
	TIME_CHANGE_MARKDELETE,
	TIME_CHANGE_MARKCOMMENT,
	TIME_CHANGE_SCENEINOUTCOLOR,
	TIME_CHANGE_SCENEINOUTDELETE,
	TIME_CHANGE_SCENEINOUTCOMMENT,
	TIME_CHANGE_SCENEINOUTMOVE,
	TIME_CHANGE_SCENEINOUTDELETEALL,
	TIME_CHANGE_SCENEINOUTDELETEALLSEL,
	TIME_CHANGE_SCENEINOUTSETINOUT,
};

#define TIME_CMD_ZOOMIN   1
#define TIME_CMD_STARTIN  2

enum TL_CURSOR
{
	TL_ARROW,
	TL_STOP,
	TL_LEFT,
	TL_RIGHT,
	TL_NS,
	TL_EW,
	TL_TOP,
	TL_BOTTOM,
	TL_HANDLE,
	TL_OK,
	TL_MAX,
};
enum TL_RES_INDEX
{
	TL_RES_PHAINDOWN,
	TL_RES_PHAINUP,
	TL_RES_PHAOUTDOWN,
	TL_RES_PHAOUTUP,

	TL_RES_BRAKE,
	TL_RES_MARKPOINT,
	TL_RES_KEYFRAME,
	TL_RES_TRIMINDOWN,
	TL_RES_TRIMINUP,
	TL_RES_TRIMOUTDOWN,
	TL_RES_TRIMOUTUP,
	TL_RES_MARKPOINT1,
	TL_RES_MARKPOINT2,
	TL_RES_MARKPOINT3,
	TL_RES_MARKPOINT4,
	TL_RES_MARKPOINT5,

	TL_RES_ZOOMLEFTDOWN,
	TL_RES_ZOOMLEFTUP,
	TL_RES_ZOOMRIGHTDOWN,
	TL_RES_ZOOMRIGHTUP,

	TL_RES_ZOOMLEFTDOWNV,
	TL_RES_ZOOMLEFTUPV,
	TL_RES_ZOOMRIGHTDOWNV,
	TL_RES_ZOOMRIGHTUPV,
};

enum TL_PEN_INDEX
{
	TL_PEN_DOWN2,
	TL_PEN_DOWN1,
	TL_PEN_UP,
	TL_PEN_TLLINE,
	TL_PEN_LINE,
	TL_PEN_SCENE,
	TL_PEN_SCENESEL,
	TL_PEN_MAX,
};

#define TP_DRAG_CAPTURE           0x00000001
#define TP_TIMER_SEEK             0x00000002
#define TP_PAINT_CHANGE           0x00000004
#define TP_PAINT_FORCEVIEW        0x00000008
#define TP_PAINT_TRIMIN           0x00000010
#define TP_PAINT_TRIMOUT          0x00000020
#define TP_PAINT_TRIMRECT         0x00000040
#define TP_PAINT_FORCETRIM        0x00000080
#define TP_PAINT_PHANTOMIN        0x00000100
#define TP_PAINT_PHANTOMOUT       0x00000200
#define TP_TLWND_LBUTTON          0x00000400
#define TP_TLWND_MBUTTON          0x00000800
#define TP_TLWND_RBUTTON          0x00001000
#define TP_PAINT_TRIMINOUT        0x00002000
#define TP_PAINT_TRIMINPHAN       0x00004000
#define TP_TLWND_LDB              0x00008000
#define TP_DRAG_MOUSEWHEEL        0x00010000
#define TP_DRAG_ESC               0x00020000
#define TP_TLWND_SENDATTACH       0x00040000
#define TP_TLWND_SELECTAUDIOKEY   0x00080000
#define TP_TLWND_AUDIOEFF         0x00100000
#define TP_TLWND_AUDIOLEVEL       0x00200000
#define TP_TLWND_SELBLANK         0x00400000
#define TP_TLWND_GETNODE          0x00800000
#define TP_TLWND_CANCLESEEK       0x01000000
#define TP_PAINT_AUDIORENDER      0x02000000
#define TP_TLWND_FORCESETCURFRAME 0x04000000 //DQA5217 ǿ��SetCurFrame jw 20090421
#define TP_TLWND_LDBBUTTON        0x08000000
#define TP_TLWND_NODELETETRIM     0x10000000 //��Ҫ����Trim
#define TP_TLWND_CENTERCURSOR     0x20000000
#define TP_TLWND_DOSELECT         0x40000000
#define TP_TLWND_NOTRIM           0x80000000


#define TP_MAX_SPLIT             3.0
#define TP_TLBAR_HEIGH           14
#define TP_TLCODE_HEIGH          35

#define TP_TLWND_BUTTON (TP_TLWND_LBUTTON | TP_TLWND_MBUTTON | TP_TLWND_RBUTTON)

#define TLRGN_CLIENT 0x00000001
#define TLRGN_LINE   0x00000002

const DWORD FLAG_RESET_MAXLENGTH	= 0x00000001;

const DWORD FLAG_CANLENGTH_EXTEND	= 0x00000001;

#define  WC_TIMELINE _L("TimeLine")

#undef  CInt64Array
#define CInt64Array CArray<__int64,__int64 >


struct TPTLPhantomMark
{
	DWORD   eCmd;      //1 ����
	INT64   iSVIn;
	INT64   iSVOut;
	INT64   iSVSpeedIn;
	INT64   iSVSpeedOut;
	INT64   iSVLen;
	INT64   iMVIn;
	INT64   iMVOut;
	INT64   iMVLen;
	INT64   iTLPos; 
	BOOL    bSVEnable;
	TPTLPhantomMark()
	{
		eCmd    = 0;
		iSVIn   = -1;
		iSVOut  = -1;
		iSVLen  = 0; 
		iMVIn   = -1;
		iMVOut  = -1;
		iMVLen  = 0;
		iTLPos  = -1;
		iSVSpeedIn  = -1;
		iSVSpeedOut = -1;
		bSVEnable   = TRUE;
	}
	INT64 GetSVLen() const
	{
		if(iSVSpeedIn >=0 && iSVSpeedOut >iSVSpeedIn)
		{
			if (iSVIn >=0 && iSVOut >= iSVIn)
				return iSVOut - iSVIn;
			else if(iSVIn>=0)
				return iSVSpeedOut - iSVIn;
			else if(iSVOut>=0)
				return iSVOut - iSVSpeedIn;
			else
				return iSVSpeedOut - iSVSpeedIn;
		}
		else if(iSVIn >=0 && iSVOut >= iSVIn)//DQA912 wj 20090731
			return iSVOut - iSVIn;
		else if(iSVIn>=0)
			return iSVLen - iSVIn;
		else if(iSVOut>=0)
			return iSVOut;
		else
			return iSVLen;
	}
};

struct TPTimeLineMark
{
	INT64 lFrame;
	BYTE  cType;
	TPTimeLineMark(INT64 lFrameSet =-1,BYTE  cTypeSet = 0)
	{
		lFrame = lFrameSet;
		cType  = cTypeSet;
	}
};
typedef  CArray<TPTimeLineMark,TPTimeLineMark&> TPTimeLineMarkArray;

struct TPTimeLineScence
{
	INT64 lIn;
	INT64 lOut;
	long  lLevel;
	long  lIndex;
	BYTE  cType;
	BYTE  bSelect;
	TPTimeLineScence()
	{
		lIn     = -1;
		lOut    = -1;
		lLevel  = -1;
		lIndex  = -1;
		cType   = 0;
		bSelect = FALSE;
	}
	TPTimeLineScence(INT64 lInSet,INT64 lOutSet)
	{
		lIn     = lInSet;
		lOut    = lOutSet;
		lLevel  = -1;
		lIndex  = -1;
		cType   = 0;
		bSelect = FALSE;
	}
};
typedef  CArray<TPTimeLineScence,TPTimeLineScence&> TPTimeLineScenceArray;

class AFX_EXT_CLASS CTPTimeLine : public CWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPTimeLine)

public:
	CTPTimeLine();
	virtual ~CTPTimeLine();
public:
	CWnd                  *m_pMsgWnd;              //������Ϣ�Ĵ���

	long double	           m_dPixelPerFrame;       //ÿ֡����
	long double            m_dStartFrame;          //��ʼ֡
	long double            m_dCurFrame;            //��ǰ֡

	__int64                m_nTrimIn;              //���
	__int64                m_nTrimOut;             //����
	__int64                m_nMarkIn;              //���
	__int64                m_nMarkOut;             //����

	double                 m_dChangePage;          //��Ҷλ��
	double                 m_dPageTurn;            //��Ҷ����

	BYTE                   m_bDrawTrim;            //�Ƿ���ʾ�����  0 ���� 1 Trim 2 DrawTrimDummy;
	BYTE                   m_bDragTrim;            //�Ƿ��϶������
	BYTE                   m_bDragScrollBar;       //�Ƿ��϶������
	BYTE                   m_bDrawBlock;           //�Ƿ������� 
	BYTE                   m_bAutoRise;            //��ǰλ�ñ����ֵ�����ʱ���Զ��������ֵ
	BYTE                   m_bDragMarkPoint;       //�Ƿ����϶���ǵ�
	BYTE                   m_bAltDragMarkPoint;    //�Ƿ����϶���ǵ�
	BYTE                   m_bMarkColor;
	BYTE                   m_bSceneInout;          //�Ƿ����޸�


	VIDEO_TYPE             m_enVideoType;          //��ǰ��Ƶ��ʽ
	BOOL                   m_bForceDraw;           //ǿ�����ػ�
	BYTE                   m_bUpdateWindow; 

	CTPToolTipCtrl        *m_pShowTips;            //������ʾ
	CTPToolTipCtrl        *m_pShowTipsEx;          //������ʾ
	CRect                  m_rtClipBox;

	TPTimeLineMarkArray    m_arrMarkPoint;         //��ǵ�
	CInt64Array            m_arrScenePoint;        //ת���ǵ�
	int                    m_iMenuPoint;
	UINT                   m_uBaseState;
	BOOL                   m_bZooming;
	UINT_PTR               m_hTimerScroll;
	static BOOL           *m_pbAudioScrub;
	double                 m_dAudioScrub;

	static BOOL            m_bXpriTrim;
	BOOL                   m_bXpriTrimSeek;

    BOOL                   m_bPlay;
	BOOL                   m_bResetStart;

	CRect                  m_rtClient;             //�ͻ���λ��
	int                    m_iTimeCodeType;
	int                    m_iTimeCodeEdge;
#ifdef TP_NS_EDIT
	BOOL                   m_bDrawPhantomMark;     //�Ƿ���ʾ�����  0 ���� 1 Trim 2 DrawTrimDummy;
	BOOL                   m_bDrawViewZoom;
	BOOL                   m_bForceZoom;
	CDWordArray            m_aBrakeFrame;
	TPTLPhantomMark        m_stuPhantomMark;
	__int64                m_nPhantomMarkIn;
	__int64                m_nPhantomMarkOut;
	__int64                m_iStartFrame;
	__int64                m_iViewFrame;
	CRect                  m_rtPhantomMarkIn;
	CRect                  m_rtPhantomMarkOut;
	CInt64Array            m_aSuctionFrame;
	UINT                   m_uSuctionState;
	int                    m_iCommandType;
#endif
	CBitmap                m_bmpDrawSave;
	CBitmap               *m_pBmpInOut;
	int                    m_iDrawWidth;
	int                    m_iDrawHeight;
	int                    m_iOffsetPixel; 

	int                    m_iShuttleState;
	BYTE                   m_bShuttleState;
	int                    m_iShuttleIndex;
	BYTE				   m_bPassedZero;

	INT64                  m_lSceneIn;
	INT64                  m_lSceneOut;
	TPTimeLineScenceArray  m_arrBlock;             //ʱ���߿�
	CSize                  m_szSceneInOut;

	__int64                m_nIndecatorIn;         //���
	__int64                m_nIndecatorOut;        //����

protected:
	CRect                  m_rtClipRect;
	int                    m_iTimeLineWidthEx;
	__int64                m_nMinFrame;            //��Сֵ
	__int64                m_nMaxFrame;            //���ֵ
	__int64                m_nMaxTLFrame;          //���ֵ

	double                 m_dZoomIn;              //��ʾ֡�ٷֱ�
	double                 m_dStartZoomIn;         //��ʼ֡�ٷֱ�

	double                 m_dMinZoomIn;           //��С��ʾ�ٷֱ�
	double                 m_dSaveStartZoomIn;     //��ʼ֡�ٷֱ�
	double                 m_dSaveZoomIn;          //��ʾ֡�ٷֱ�
	double	               m_dSavePixelPerFrame;   //ÿ֡����
	double 	               m_dSaveStatrFrame;      //��ʼ֡
	double                 m_dSaveCurFrame;        //��ǰ֡
	double                 m_dXpriCurFrame;        //��ǰ֡
	double                 m_dSecutionFrame;        //��ǰ֡
	BOOL                   m_bSaveClick;           //�ϴ��Ƿ�˫��
	int                    m_iDBClickStep;         //˫��ʱ�Ĳ�ס

	int                    m_iBarHeadState;        //������ͷ��״̬
	int                    m_iTimeCodeWidth;       //ʱ���߿��
	int                    m_iTimeCodeLeft;        //ʱ������߿��
	int                    m_iTimeCodeRight;       //ʱ�����ұ߿��
	int                    m_iBarHeight;           //�������߶�
	int                    m_iTimeCodeHeight;      //ʱ���߸߶�
	int                    m_iEdgeDisatnce;        //�߿�
	int                    m_iBackColor;
	int                    m_iTimeCodeBackColor;

	CTPIntArray            m_arrUnDoReDo;          //�Ŵ�������
	INT_PTR                m_iUnDoReDoIndex;       //�Ŵ�������

	CRect                  m_rtCursorArea;         //�����Է�Χ
	CRect                  m_rtLeftHead;           //���������ͷ��λ��
	CRect                  m_rtRightHead;          //�������ұ�ͷ��λ��
	CRect                  m_rtBarTotal;           //������λ��
	CRect                  m_rtBarHandle;          //�������в�λ��
	CRect                  m_rtTimeCode;           //ʱ��������λ��
	CRect                  m_rtTimeBar;            //ʱ��������λ��
	CRect                  m_rtTrimIn;             //���ͼ������λ��
	CRect                  m_rtTrimOut;            //����ͼ������λ��
	CRect                  m_rtTimeLine;           //ʱ����λ��
	CRect                  m_rtTimeLineTest;       //ʱ���߲���λ��

	TP_DRAG_TYPE           m_enDragType;           //��קģʽ
	CPoint                 m_ptDrag;               //��ק�����
	__int64                m_iDrag;                //��קʱ��������
	CRect                  m_rtDrag;

	HRGN                   m_hTimeLineRgn;
	HRGN                   m_hClientRgn;
	HRGN                   m_hInvertRgn;
	__int64                m_iTimeLineRgnPos;

	HCURSOR                m_hSetCursor;
	UINT_PTR               m_hTimer;

	BOOL                   m_bWndFocus;
	BOOL                   m_bWndEnable;
	BOOL                   m_bUseRgn;

public:
	__int64				   m_nSafeLength;		   //���밲ȫ����
	__int64				   m_nActualMaxFrame;	   //ʵ����󳤶�

	TPCommonRes            *m_pResCursor[TL_MAX];
	TPCommonRes            *m_pResBar;
	TPCommonRes            *m_pResBarBack;
	TPCommonRes            *m_pResPen[TL_PEN_MAX];

	double                 m_dSaveState[6];

private:
	BOOL                   m_bSubWindow;
	BOOL                   m_bCreate;

public:
	BOOL          Create(DWORD dwStyle,const RECT& rect,CWnd* pParentWnd, UINT nID = 0);
	void          SetResPath(CString sPath);

	double        TranslateFrame(__int64 dFrame) const;
	double        TranslateFrame(long double dFrame) const; //֡ -> ����
	long double   TranslatePixel(double dPixel) const;      //���� -> ֡
	long double   TranslateString(CString sFrame);     //���� -> ֡
	
	virtual CRect GetTimeCodeRect();         //ʱ��
	virtual CRect GetTimeBarRect();          //������
	virtual CRect GetTimeLineRect();         //ʱ����

	//��ʾ����
	void          SetPixelPerFrame(double dPixelPerFrame,BOOL bCenter = TRUE,BOOL bPaint = TRUE);              //��ʾ����
	void          SetStartFrame(long double dFrame,BOOL bPaint = TRUE);                                        //��ʼλ��
	void          SetCurFrame(long double dCurFrame,BOOL bReset ,BOOL bPaint = TRUE, DWORD dwFlags=0);           //��ǰλ��
	void          SetCurFrame(__int64 dCurFrame,BOOL bReset ,BOOL bPaint = TRUE, DWORD dwFlags=0);
	void          SetCurFrame(int dCurFrame,BOOL bReset ,BOOL bPaint = TRUE, DWORD dwFlags=0);

	void          SetMaxMinFrame(__int64 iMin,__int64 iMax,BOOL bReset = FALSE,BOOL bPaint = TRUE, DWORD dwFlags=0);
	void          SetMaxFrame(__int64 iMax,BOOL bReset = FALSE,BOOL bPaint = TRUE, DWORD dwFlags=0);
	void          SetMaxTLFrame(__int64 iMax);           //�������ʱ���ܵ��ĵط�
	void          SetMaxActualFrame(__int64 iMax);           //�������ʱ���ܵ��ĵط�
	void          EnableViewFrame(long double dCurFrame);
	void          SetVirtualFrame(INT64 dCurFrame, BOOL bReset, BOOL bPaint = TRUE);
	__int64       GetViewFrame();                        //ȡ�õ�ǰ��ʾ��֡�� 
	__int64       GetMaxFrame() { return m_nMaxFrame ;}
	__int64       GetMinFrame() { return m_nMinFrame ;}

	int           AddMarkPoint(__int64 iFrame,BOOL bPaint = TRUE);
	int           DelMarkPoint(__int64 iFrame,BOOL bPaint = TRUE);
	int           AddSceneInOut(__int64 iIn,__int64 iOut,BOOL bPaint = TRUE);
	int           DelSceneInOut(__int64 iIn,__int64 iOut,BOOL bPaint = TRUE);

	static void   Draw3DRect(CDC *pDC,CRect &rtClip,TPCommonRes *p3DL,TPCommonRes *p3DR);
	static void   Draw3DRect(CDC *pDC,CRect &rtClip,CPen *p3DL,CPen *p3DR);
	static void   Draw3DLine(CDC *pDC,CRect &rtClip,CPen *p3DL,CPen *p3DR,BOOL bVert);
	void          Invalidate(BOOL bErase = TRUE);
	void          SetDBClick();

	void          SetStartZoomIn(double dZoomIn,BOOL bPaint = TRUE);
	double        GetStartZoomIn();
	void          SetZoomIn(double dPPF,BOOL bCenter = TRUE,BOOL bPaint = TRUE,double dCurPixel = 0.0);
	double        GetZoomIn();
	double        GetMinZoomIn(){return m_dMinZoomIn; }   //��С��ʾ�ٷֱ�

	void          ValidateRect(LPCRECT lpRect = NULL);

	void          SetLineType(BOOL bSeekEff);
	void          SetActMouseWheel(BOOL bEnable);
	void          SetCaptureChanged();

	static COLORREF       GetMarkColor(int iIndex);
	static TL_RES_INDEX   GetMarkIndex(int iIndex);

#ifdef TP_NS_EDIT
	void           SetActivate(BOOL bAct); 
	void           SetStartAndView(INT64 iStartFrame,INT64 iViewFrame);
	void           SetPhantomMark(BOOL bSV,TPTLPhantomMark &stuData);
	void          (*GetSuctionFrame)(CTPTimeLine *pThis,UINT uState);	
	void          (*PaintTimeLineEx)(CDC *pDC);	
	static double (*GetShuttleSpeed)(long &lState);	
	void           SetShuttleActivate(BOOL bAct); 
	double         GetSecution(CPoint point,double dCurFrame,UINT uBaseType = 0);
	void           GetMiniZoomIn();
#endif
	int            GetBaseHight();

	void           SetSize(int cx, int cy);

	void           ShowToolTipEx(BOOL bShow);
	void           SetToolTipExText(CString sText);

	BOOL           SetZoomInOut(BOOL bIn,long double lCenterPos = -1);

	virtual BOOL   OnMouseWheel(MSG *pMsg);
	//��ͼ����
	void           PaintDC(CDC *pDC,BOOL bClipBox = TRUE);

protected:
	virtual void    PaintFrame(CDC *pDC);
	void            PaintTimeBar(CDC *pDC);
	virtual void    PaintTimeCode(CDC *pDC);
	virtual void    PaintTimeLine(CDC *pDC);
	virtual void    PaintTimeLinePre(CDC *pDC);
	virtual void    PaintTimeTrim(CDC *pDC);
	virtual void    PaintTimeTrack(CDC *pDC);
	virtual void    PaintTrimIcon(CDC *pDC);

	virtual BOOL    SpecialCallback(int iCmd,LPARAM  lParam);

	void            PaintTimeSceneInOut(CDC *pDC,double dStartFrame,double dEndFrame,CRect &rectCode);
	void            PaintTimeSubIndecator(CDC *pDC,double dStartFrame,double dEndFrame);
	void            PaintTimeCodeLine1(CDC *pDC,double dStartFrame,double dEndFrame ,CRect &rectCode);
	virtual   void  PaintTimeCodeLine2(CDC *pDC,double dStartFrame,double dEndFrame ,CRect &rectCode);
	//��ʾ��������
	virtual void    GetTimeLineRgn(UINT uRgn);

	void            ViewCurFrame();

	BOOL            OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	int             OnToolHitTest(CPoint point,TOOLINFO* pTI ) const;
	void            OnMenuCommond(UINT uID);
public:
	static   void   RegisterClass(BOOL bRegister = TRUE);
	static   BOOL   ZoomX(CPoint &ptStart,CPoint &ptEnd);

protected:
	virtual  void   SubWindow(BOOL bSub);
	void            SetDragState(CPoint point); // XN00023197 wangjin 20091110

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk( UINT, CPoint );
};
