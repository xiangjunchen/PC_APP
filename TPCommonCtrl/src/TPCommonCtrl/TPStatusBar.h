#pragma once


// CTPStatusBar

class AFX_EXT_CLASS CTPStatusBar : public CStatusBar ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPStatusBar)

public:
	CTPStatusBar();
	virtual ~CTPStatusBar();

public:
	int                     m_iBackColor;
private:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	TP_BORDER_STYLE         m_eBorderStyle;
	TPCommonRes             *m_pSizeBox; 
	TPCommonRes             *m_pLogImage; 
	int                      m_iPanleImage[5]; 
public:
	void            SetPaneImage(int iIndex,int iImage); 
	void            SetLogImage(CString sText,int iWidth,COLORREF clMask);
private:
	virtual void    SubWindow(BOOL bSub);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
};


