#pragma once


// CTPSpinButtonCtrl

class AFX_EXT_CLASS CTPSpinButtonCtrl : public CSpinButtonCtrl ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPSpinButtonCtrl)

public:
	CTPSpinButtonCtrl();
	virtual ~CTPSpinButtonCtrl();
public:
	CWnd                   *m_pMsgDelPosWnd;	
	BOOL                    m_bForceBud;
	
private:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle;
	TP_BORDER_STYLE         m_eBorderStyle;
	UINT_PTR                m_uTimer;
	int                     m_iAccrCount; 

	int                     m_iOver;
	int                     m_iPressed;
	CRect                   m_rtSpinButton[2];
	CString                 m_sSkinPath;

	TPCommonRes            *m_pButtonBmp[8];	

public:
	void            SetSkinPath(CString sSkin);
	int             GetPressed(){return m_iPressed;}
	void            EnableAcclet(BOOL bEnable);
	HWND            UnsubclassWindow();
private:
	virtual void    SubWindow(BOOL bSub);
	void            LoadButtonBmp(); 


protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


