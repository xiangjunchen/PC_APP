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
			Op_Nothing		// �����Ȃ�
		,	Op_Intelligent	// "Intelligent/Normal"��Ԃő��쒆
		,	Op_Original		// "Original"��Ԃő��쒆
	};

protected:
	BOOL		m_bRecived;
	int			m_nOperateType;		// "Intelligent"�X���C�_��ԃt���O
	int			m_nPerWidth;		// �X���C�_��
	int			m_nOperateState;	// ������

	double		m_dValMin;				// �l�̍ŏ��l
	double		m_dValMax;				// �l�̍ő�l

	double		m_dValue;			// �l
	double		m_dStartVal;		// "Intelligent"����J�n���̒l
	double		m_dStackValue;		// "Intelligent"���쒆�̗ݐϒl
	SEPOINTREAL	m_ptPrevPoint;		// ���O�̃|�C���g
	double      m_dStep;
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


