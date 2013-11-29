#pragma once


// CTPToolTipCtrl
#define WM_TPTOOLTIPCTRL  WM_USER + 11111 
class AFX_EXT_CLASS CTPToolTipCtrl : public CToolTipCtrl ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPToolTipCtrl)

public:
	CTPToolTipCtrl();
	virtual ~CTPToolTipCtrl();
public:	
	CString                 m_sToolTips; 
	UINT                    m_uState; 
	CWnd                   *m_pMsgWnd; 
	int                     m_iPopState;
		
private:
	int                     m_iTextColor;
	int                     m_iBackColor;

	COLORREF                m_clTextColor;
	COLORREF                m_clBackColor;

	struct _tagIconView
	{
		COLORREF clRange;
		LPBYTE   pByte;
		CSize    szSize;
		CString  sNotify;
		long     lHeight;
		_tagIconView(){pByte = NULL;}
		~_tagIconView(){if(pByte) delete [] pByte;pByte = NULL;}
	};
	CArray<_tagIconView *,_tagIconView *&> m_aIconView;
public:
	void                    AddIconView(LPBYTE pByte,CSize &szSize,CString sNotify,COLORREF clRange);
	void                    ClearIconView();
private:
	void                    PaintDC(CDC *pDC);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTtnTooltipPop(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};


