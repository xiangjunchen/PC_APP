#pragma once
#include <afxole.h>


#define WC_TPTIMECODEEDITNS _L("TPTimeCodeEditNSClass")


class AFX_EXT_CLASS CTPTimeCodeEditNS : public CTPTimeCodeEditEx
{
	DECLARE_DYNAMIC(CTPTimeCodeEditNS)

public:
	CTPTimeCodeEditNS();
	virtual ~CTPTimeCodeEditNS();

public:
	CPoint          m_ptDrag;

	int             m_iLeftWidth;  
	int             m_uShowStyleTemp;

private:
	TPCommonRes       *m_pLogo; 
	TPCommonRes       *m_pLeftLogo; 	
	UINT               m_uLogoPos;
	CTPOleDropTarget  *m_pOleDrag;

public:
	static void     RegisterClass(BOOL bRegister);
	virtual void    SubWindow(BOOL bSub);
	virtual void    GetTimeCodeRect(CRect &rtTimeCode);
	virtual void    PaintDC(CDC *pDC);

	void            SetLogo(CString sFileName,int iWidth,COLORREF clMask);
	void            SetLeftLogo(CString sLogo,UINT uTPos);

	void            SetSVMode();
	void            SetDigMode(BOOL bCTL = TRUE);
	void            SetMVMode();
	void            SetRecordMode();
	void            SetTCMode();
	void            SelectTypeMenu(CWnd *pWnd);
	void            SelectNextType();
	void            SetTypeEnable(UINT uType,BOOL bMisMatchReset = TRUE);

	void            OnNSMenuCmd(UINT nID);
	LRESULT         OnMsgOleDrag(WPARAM wp,LPARAM lp);
	BOOL            ValidateClipboardData();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};