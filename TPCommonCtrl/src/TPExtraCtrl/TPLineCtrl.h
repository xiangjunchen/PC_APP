#pragma once


// CTPLineCtrl
#define WC_LINECTRL _L("LineCtrlClass")

class AFX_EXT_CLASS CTPLineCtrl : public CTPWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPLineCtrl)

public:
	CTPLineCtrl();
	virtual ~CTPLineCtrl();

protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	TP_BORDER_STYLE         m_eBorderStyle;
	int                     m_iBackColor;
	int                     m_iCount;
	int                     m_iSpace;
	BOOL                    m_bVert;

public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void         SetCount(int iCount,int iSpace);



public:
	static  void            RegisterClass(BOOL bRegister);
	virtual void            SubWindow(BOOL bSub);

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


