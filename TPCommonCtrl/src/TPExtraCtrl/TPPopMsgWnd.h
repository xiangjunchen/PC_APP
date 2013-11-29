#pragma once


// CTPPopMsgWnd
enum TP_TASKBARPOPWND_INDEX
{
	TP_TASKBARPOPWND_HIDDEN,
	TP_TASKBARPOPWND_APPEARING,
	TP_TASKBARPOPWND_SHOW,
	TP_TASKBARPOPWND_DISAPPEARING,
};
enum TP_POPDIRECT_INDEX
{
	TP_POPDIRECT_UP,
	TP_POPDIRECT_DOWN,
	TP_POPDIRECT_LEFT,
	TP_POPDIRECT_RIGHT,
};

#define TP_TASKBARPLACEMENT_TOP     1
#define TP_TASKBARPLACEMENT_BOTTOM  2

class  AFX_EXT_CLASS CTPPopMsgWnd : public CWnd
{
	DECLARE_DYNAMIC(CTPPopMsgWnd)

public:
	CTPPopMsgWnd();
	virtual ~CTPPopMsgWnd();

public:
	struct tag_Poster
	{
		CString   sPic;
		CString   sTitle;
		CString   sComment;
		CString   sActor;
		UINT      uState;
		LPARAM    lParam1;
		CString   sMessage;
		DWORD	  dwDirect;
		tag_Poster()
		{
			sPic        = _T("");
			sTitle      = _T("");
			sComment    = _T("");
			sActor      = _T("");
			uState      = 0;
			lParam1     = NULL;
			dwDirect    = TP_POPDIRECT_UP;
		}
		tag_Poster &operator = (tag_Poster &other)
		{
			sPic        = other.sPic;
			sTitle      = other.sTitle;
			sComment    = other.sComment;
			sActor      = other.sActor;
			uState      = other.uState;
			lParam1     = other.lParam1;
			sMessage    = other.sMessage;
			dwDirect    = other.dwDirect;
			return *this;
		}
	}               m_stuUser;

	static  CWnd   *m_pMsgSneWnd;
	CRect           m_rtWnd;
protected:
	DWORD		   m_dwTimeToLive;
	INT			   m_nIncrement;
	INT			   m_nWndHeight;
	INT			   m_nWndWidth;
	INT			   m_nStartPosX;
	INT			   m_nStartPosY;
	INT			   m_nCurPosX;
	INT			   m_nCurPosY;
	INT			   m_nWndState;//´°¿Ú×´Ì¬    1.µ¯³ö×´Ì¬  2.ÏÔÊ¾×´Ì¬   3.ÏûÊ§×´Ì¬
	INT			   m_nAlpha;
	CString		   m_sMessage;
	CRect          m_rtCloseBox;
	CString        m_sTitle;
	BOOL           m_bEnable;

public:
	BOOL     Create(DWORD uStyle, CWnd *pWnd, UINT nID);
	void     Show(LPCTSTR sMessage, CPoint ptLeftTop, DWORD dwTimeToLive);
	void     Show(LPCTSTR sMessage);
private:
	void     PaintDC(CDC *pDC);
	void     LoadRes();

	BYTE*	 Get24BitPixels(HBITMAP pBitmap, WORD *pwWidth, WORD *pwHeight);
	HRGN     GenerateRegion(HBITMAP hBitmap, BYTE red, BYTE green, BYTE blue);
	void	 CalcPopPos();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnPaint();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

AFX_EXT_API  void TP_SetPosMessageState(CPoint ptLeftTop,long lTime,DWORD dwDirect  , CWnd* pParent,BOOL bSingle = TRUE);
AFX_EXT_API  void TP_ShowPosMessage(LPCTSTR sMessage,CTPPopMsgWnd::tag_Poster *pUser = NULL, CPoint ptLeftTop = CPoint(0,0),long lTime   = -1);
AFX_EXT_API  void TP_ClosePosMessage(CTPPopMsgWnd::tag_Poster *pUser = NULL);
AFX_EXT_API  void TP_CloseAllPosMessage();