#pragma once


// CTPSliderRoundCtrl
#define WC_SLIDERROUNDCTRL  _L("SliderRoundCtrl") 

#define WM_BUTTON WM_USER +55   //add by lzq:bug:XN00012138,2009.05.09

class AFX_EXT_CLASS CTPSliderRoundCtrl : public CTPSliderBar
{
	DECLARE_DYNAMIC(CTPSliderRoundCtrl)

public:
	CTPSliderRoundCtrl();
	virtual ~CTPSliderRoundCtrl();

public:
	int            m_iThumbType; 
	int            m_iRoundType;//lzq,值: 1:EQ窗口方式; 2:AudioMixer窗口方式; 3:EQ窗口中Gain数据显示方式,( - 0 + )。 2009.05.31
private:
	int            m_iStartAngle;
	int            m_iEndAngle; 	
	CRect          m_rtCircle;	
	int            m_iRadius;
	int            m_iThumbLen;
	CPoint         m_ptCenter;
	CRect          m_rtThumb;


	int             m_iEscPos;
	BOOL            m_bEsc;

public:
	void            SetResType(int iType);

public:
	double          GetDB();  
	void            SetDB(double dDB,BOOL bComplex = FALSE);
protected:
	virtual void    LoadThumb();
	virtual void    PaintCtrl(CDC *pDC);
	virtual void    SetCursorPosition(CPoint &point);
	virtual void    SubWindow(BOOL bSub);
private:


	BOOL            OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );

	double          PosToAngle(double iPos);	
	double          AngleToPos(double dAngle);
	double          PointToPos(CPoint pt); 
	CPoint          PosToPoint(double iPos);
	CPoint          AngleToPoint(double dAngle);
	void            AngleToPoint(double dAngle,double &dx,double &dy);
	double          PointToAngle(CPoint pt);

	int             GetMicroSetp(int iDragPos,CPoint &ptCur);
public:
	static void     RegisterClass(BOOL bRegister);
	virtual void    SetBackRes(CString sResPath); //add by pyc 2007-7-25

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


