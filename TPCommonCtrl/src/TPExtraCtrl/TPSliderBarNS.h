#pragma once

#define SLIDER_CHANNEL 0x0000001
#define SLIDER_THUMB   0x0000002




#define  TP_SLIDERBAR_NOBACK 0x00000001
// CTPSliderBarNS
#define WC_SLIDERBARNS _T("SliderBarNS")

class  AFX_EXT_CLASS CTPSliderBarNS : public CWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPSliderBarNS)

public:
	CTPSliderBarNS();
	virtual ~CTPSliderBarNS();
public:
	CWnd                   *m_pMsgWnd;
	int                     m_iChanneOffset;
	int                     m_iThumbType;
	BOOL                    m_bDoubleClick;
	BOOL                    m_bCtrlApply;
	BOOL                    m_bPressed;
protected:
	UINT                    m_uStyle; 
	TPCommonRes				*m_pThumbBmp[4];


	int                     m_iPage;
	int                     m_iPagePixel;
	int                     m_iMin;
	int                     m_iMax;
	int                     m_iPos;
	int                     m_iTopLeft;
	int                     m_iRightBottom;
	int                     m_iBarSize;

	BOOL                    m_bOver;

	CRect                   m_rtThumb;
	CRect                   m_rtThumbHandle;
	CRect                   m_rtChannel;
	CRect                   m_rtClient;

	CPoint                 m_ptDragSave;         //拖拽保存点
	int                    m_iDragSave;		     //拖拽时点与左边距离
	int		               m_iDragInit;         //拖拽起始点

	UINT_PTR               m_hTimerHover;

	UINT_PTR                m_hTimerDBClick;
	CPoint                  m_pLButton;
	COLORREF                m_clChannel; 
	COLORREF                m_clBack; 
	UINT                    m_eBackColor;
	UINT                    m_eChanelColor;
	TPCommonRes             *m_pBackRes;
	TP_BORDER_STYLE         m_eBorderStyle;


public:
	void SetChannelColor(COLORREF  clSet); 
	void SetBackColor(COLORREF  clSet); 
	int  GetPageSize() const;
	void SetPageSize(int iPageSize,BOOL bPaint = TRUE);

	void GetRange(int &nMin, int &nMax)
	{
		nMin = m_iMin;
		nMax = m_iMax;
	}
	void SetRange(int nMin, int nMax)
	{
		m_iMin = nMin;
		m_iMax = nMax;
	}

	int GetPos()
	{
		return m_iPos;
	}
	void SetPos(int nPos)
	{
		m_iPos = nPos;
		Invalidate();
	}
	static void     RegisterClass(BOOL bRegister = TRUE);

protected:
	virtual void    LoadThumb();
	virtual void    PaintCtrl(CDC *pDC);
	virtual void    GetChannelRect(CRect &rtChannel);

	int             PixelToPos(int iPixel);   
	int             PosToPixel(int iPos);   
	virtual void    CalcSize(UINT uFlag);
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


