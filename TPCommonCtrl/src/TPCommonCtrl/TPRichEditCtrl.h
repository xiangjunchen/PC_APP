#pragma once


// CTPRichEditCtrl

class AFX_EXT_CLASS CTPRichEditCtrl : public CRichEditCtrl ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPRichEditCtrl)

public:
	CTPRichEditCtrl();
	virtual ~CTPRichEditCtrl();
private:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	TP_BORDER_STYLE         m_eBorderStyle;
	
private:
	virtual void    SubWindow(BOOL bSub);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
protected:
	virtual void PreSubclassWindow();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


