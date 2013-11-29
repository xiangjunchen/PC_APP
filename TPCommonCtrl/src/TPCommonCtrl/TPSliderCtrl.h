#pragma once


// CTPSliderCtrl
#define  WM_SLIDERCTRLMSG  WM_USER + 1023
#define  WM_SLIDERCTRL_LBD 0x00000001


class AFX_EXT_CLASS CTPSliderCtrl : public CSliderCtrl ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPSliderCtrl)

public:
	CTPSliderCtrl();
	virtual ~CTPSliderCtrl();	
public:
	UINT                    m_eBackColor;
	UINT                    m_eChanelColor;
	BYTE                    m_cPaintTickType;
	int                     m_iThumbType;	
	BOOL                    m_bDoubleClick;
	BOOL                    m_bCtrlApply;
	BOOL                    m_bPressed;
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle;
	TP_BORDER_STYLE         m_eBorderStyle;
	TPCommonRes             *m_pTicLinePen;
	TPCommonRes             *m_pThumbBmp[4];
	TPCommonRes             *m_pBackRes;
	BOOL                    m_bOver;
	UINT                    m_hTimer;
	BOOL                    m_bEsc;
	int                     m_iEscPos; 
	int                     m_iLineSize;

	CRect                   m_rtChannel;
	CRect                   m_rtThumb;
	int                     m_iTickEdge;

	COLORREF                m_clChannel; 
	COLORREF                m_clBack; 
	BOOL                    m_bPaint;
	int                     m_iPosSave;
	int                     m_iPos;
	int                     m_iGetPos;
	BYTE                    m_cMicro;
	CString                 m_sTooltips;
	CPoint                  m_ptDrag;
	DWORD                   m_dwDragTime;
	DWORD                   m_dwMouseMove;   
	int                     m_iMinOffset;

	int                     m_iMin;
	int                     m_iMax;

public:
	void                    SetChannelColor(COLORREF  clSet); 
	void                    SetBackColor(COLORREF  clSet); 
	void                    SetThumb(CString sPathName);
	void                    SetBack(CString sPathName);

	void                    SetPos(int nPos);
	int                     GetPos() const;
	void                    GetRange(int& nMin, int& nMax) const;
	void                    SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);	
	void                    SetToolTips(CString sToolTips); 

	int                     SetLineSize(int nSize);
	static void             RegisterClass(BOOL bRegister = TRUE);
protected:
	virtual void    SubWindow(BOOL bSub);
	virtual void    LoadThumb();
	virtual void    PaintCtrl(CDC *pDC);
    virtual void    PaintTicks(CDC *pDC);
	virtual void    PaintTicks1(CDC *pDC);
	virtual int     GetMicroSetp(int iPos,CPoint &ptCur);

	virtual BOOL    OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
};


