#pragma once


// CTPVUSliderCtrl
#define WC_VUSLIDERCTRL  _L("VUSliderCtrl") 
#define TP_SMARTSLIDER_CONTORL

#define TP_VUSLIDERCTRL_AUDIOTICKL 0x00000001
#define TP_VUSLIDERCTRL_AUDIOTICKR 0x00000002
#define TP_VUSLIDERCTRL_NOTHUMB    0x00000004

#ifdef TP_SMARTSLIDER_CONTORL
#include "TPSmartSliderData.h"
#endif

class AFX_EXT_CLASS CTPVUSliderCtrl : public CTPSliderBar
{
	DECLARE_DYNAMIC(CTPVUSliderCtrl)

public:
	CTPVUSliderCtrl();
	virtual ~CTPVUSliderCtrl();
public:
	UINT            m_uPrivateStyle; 
	COLORREF        m_clBack;
protected:
	BOOL            m_bPaintLable;
	int             m_iTickEdge;
	CToolTipCtrl   *m_showTips;	
	BOOL            m_bEsc;
	int             m_iEscPos; 
	double          m_dDBSet;

	static TPCommonRes      *m_pLargIcon;
	static int               m_iRefCount;

	UINT_PTR                 m_hClickTime;
	int                      m_iClickPos;

public:
	double          GetDB();  
	void            SetDB(double dDB,BOOL bComplex = FALSE);
	BOOL            SetValue(double dValue);
	double          GetValue();
	void            SetPaintLable(BOOL bPaintLable = FALSE);	

	void            PaintTicksEx(CDC *pDC,CRect rtTick);
public:
	static void     RegisterClass(BOOL bRegister = TRUE);

protected:
	int             ValueToPos(double dValue,double  *pdValue,int iCount);
	double          PosToValue(int iPos,double  *pdValue,int iCount);
	BOOL            OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	virtual int     GetMicroSetp(int iDragPos,CPoint &ptCur);
	virtual void    SetCursorPosition(CPoint &ptCur);
protected:
	virtual  void   CalcSize(UINT uFlag);
protected:
	virtual void    SubWindow(BOOL bSub);
	virtual void    LoadThumb();
	virtual void    PaintTicks(CDC *pDC);
	virtual void    PaintCtrl(CDC *pDC);
	virtual void    PaintTicksAudio(CDC *pDC);
	virtual int     PixelToPos(int iPixel);   

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);//add by lizhiqiao for:按上下键 滑块移动反向的问题。2008.09.25
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);


#ifdef TP_SMARTSLIDER_CONTORL
protected:
	enum
	{
		Op_Nothing		// 壗傕側偟
		,	Op_Intelligent	// "Intelligent/Normal"忬懺偱憖嶌拞
		,	Op_Original		// "Original"忬懺偱憖嶌拞
	};
	double		m_dValMax;
	double		m_dValMin;
	BOOL		m_bRecived;
	int			m_nPerWidth;		
	int			m_nOperateState;	
	double		m_dValue;			
	double		m_dStartVal;		
	double		m_dStackValue;		
	SEPOINTREAL	m_ptPrevPoint;		
	double      m_dStep;
	BOOL		CanIntelligentOperation();
	void		InitIntelligentResource();
	void		InitCalcVal();
	double		Pos2Value(int nPos);
	int			Value2Pos(double dValue);
	afx_msg	LRESULT	OnMouseDeviceHookCursorMove(WPARAM wParam, LPARAM lParam);
	void		CalcValue(const SEPOINTREAL& point);
public:
	void		SetStep(double dStep);
	void		SetValueRange(double dMin, double dMax);
#endif
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PreSubclassWindow();
};


