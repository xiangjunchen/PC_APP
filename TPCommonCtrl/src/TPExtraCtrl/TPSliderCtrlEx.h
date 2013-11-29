#pragma once
#include "TPSmartSliderData.h"

#define WC_SLIDERCTRLEX  _L("SMARTSliderCtrlEx") 

enum TP_SMARTSLIDER_WIN_TYPE
{
	TP_SMARTSLIDER_WIN_UNKNOWN =0,
	TP_SMARTSLIDER_WIN_EFFECTWINDOW = 0x01,
	TP_SMARTSLIDER_WIN_CGEDIT		= 0x02,
	TP_SMARTSLIDER_WIN_IMPORTTOOL	= 0x04,
	TP_SMARTSLIDER_WIN_TIMELINE		= 0x08,
	TP_SMARTSLIDER_WIN_AUDIOINPUT	= 0x10,
	TP_SMARTSLIDER_WIN_CDEDIT	    = 0x20,
	TP_SMARTSLIDER_WIN_MINILIVEIN   = 0x40,	 
	TP_SMARTSLIDER_WIN_MCEDIT       = 0x80,
};

class  AFX_EXT_CLASS CTPSliderCtrlEX : public CTPSliderCtrl  
{
	DECLARE_DYNAMIC(CTPSliderCtrlEX)
public:
	CTPSliderCtrlEX();
	virtual ~CTPSliderCtrlEX();

	static void RegisterClass(BOOL bRegister = TRUE);
	static void InitSmartSlider(DWORD dwWinType);
	static void UninitSmartSlider(DWORD dwWinType);
public:

	//{{AFX_VIRTUAL(CSeSliderCtrlEx)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSeSliderCtrlEx)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
	afx_msg LRESULT	OnMouseDeviceHookCursorMove(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void	SetValueRange(double dMin, double dMax);
	BOOL	SetValue(double dValue,BOOL bForce = FALSE);
	double	GetDoubleValue();
	int		GetIntValue();
	void	SetSliderOperationType(int nType, double dCurValue);
	void    SetPos(int iPos);
	void    SetStep(double dStep);

protected:
	void	CalcValue(const SEPOINTREAL& point);
	BOOL	CanIntelligentOperation();
	void	InitIntelligentResource();
	void	InitCalcVal();
	int		Value2Pos(double dValue);
	double	Pos2Value(int nPos);

	int  PixelToPos(CPoint point);
protected:

	enum
	{
			Op_Nothing		// 何もなし
		,	Op_Intelligent	// "Intelligent/Normal"状態で操作中
		,	Op_Original		// "Original"状態で操作中
	};

protected:
	BOOL		m_bRecived;
	int			m_nOperateType;		// "Intelligent"スライダ状態フラグ
	int			m_nPerWidth;		// スライダ幅
	int			m_nOperateState;	// 操作状態

	double		m_dValMin;				// 値の最小値
	double		m_dValMax;				// 値の最大値

	double		m_dValue;			// 値
	double		m_dStartVal;		// "Intelligent"操作開始時の値
	double		m_dStackValue;		// "Intelligent"操作中の累積値
	SEPOINTREAL	m_ptPrevPoint;		// 直前のポイント
	double      m_dStep;
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


