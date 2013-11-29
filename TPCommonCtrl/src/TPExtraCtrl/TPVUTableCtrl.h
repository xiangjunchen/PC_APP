#pragma once

#define VUTABLE_SMOOTH   0x00000001    //平滑
#define VUTABLE_COMPART  0x00000002    //间隔

#define VUTABLE_VERT     0x00000010    //间隔
#define VUTABLE_HORZ     0x00000020    //间隔
#define VUTABLE_ADBE     0x00000040    //间隔

#define VUTABLE_FLAT     0x00010000    //空白
#define VUTABLE_RES      0x00020000    //图片
#define VUTABLE_ENABLE   0x10000000    //图片
#define VUTABLE_PEEKHOLD 0x20000000    //图片

// CTPVUTableCtrl
#define WC_VUTABLE _L("VUTableCtrl")

enum VATABLE_INDEX
{
	VATABLE_NORMAL,
	VATABLE_ALARM,
	VATABLE_DANGER,
	VATABLE_DISABLE,
	VATABLE_BACK,	
	VATABLE_MAX,
};


class AFX_EXT_CLASS CTPVUTableCtrl : public CStatic,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPVUTableCtrl)

public:
	CTPVUTableCtrl();
	virtual ~CTPVUTableCtrl();
public:
	UINT                    m_uPeekHold;
	int                     m_iDelayPos;    //停留位置
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	TP_BORDER_STYLE         m_eBorderStyle;

	TPCommonRes             *m_pTableRes[VATABLE_MAX];
	TPCommonRes             *m_pPenRes[3];

	static TPCommonRes      *m_pLargIcon;
	static int               m_iRefCount;




protected:
	int                     m_iMin;         //最小值
	int                     m_iMax;         //最大值
	int                     m_iPos;         //当前值
	int                     m_iDivMin;	
	int                     m_iDivMax;
	int                     m_iLastPos;     //上次位置
	
	int                     m_iDelayMis ;   //停留时间  毫秒
	int                     m_iTickCount; 
	int                     m_iDelayCount; 
	UINT_PTR                m_hDelayTimer;
	DWORD                   m_uVUState;
	int                     m_iOffeset;	
public:
	BOOL            Create(UINT uStyle,CRect rtWnd,CWnd *pParent,UINT nID = 0);

	void            SetRange(int iMin,int iMax);
	void            GetRange(int &iMin,int &iMax);

	void            SetPos(int iPos);
	int             GetPos();

	void            SetDelayMis(int iMillisecond );
	int             GetDelayMis();

	void            SetDivision(int iMin,int iMax);
	void            SetHorz(BOOL bHorz); 
	void            SetSmooth(BOOL bSmooth); 
	void            SetFlat(BOOL bFlat); 

	void            ReSetPeek();
	BOOL            IsOverflow(); 

	void            SetDBEState(BOOL bDbe);

	int             GetMiniWidth();

	void            SetPeekHold(BOOL bSet); 

	static void     PaintTicks(CDC *pDC,CRect rtClient,BOOL bText,BOOL bLine);       

	static  void    RegisterClass(BOOL bRegister = TRUE);

private:
	virtual void    SubWindow(BOOL bSub);
	void            LodaRes();
	void            PaintTableVertSmooth(CDC *pDC);	
	void            PaintTableHorzSmooth(CDC *pDC);	
	void            PaintTableVertDivision(CDC *pDC);
	void            PaintTableHorzDivision(CDC *pDC);
	void            PaintTableVerBack(CDC *pDC);
	void            PaintTableVerNS(CDC *pDC);

	void            PaintDC(CDC *pDC);

	LRESULT         OnMsgUnSubWindow(WPARAM wp,LPARAM lp);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
};


