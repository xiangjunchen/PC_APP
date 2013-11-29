#pragma once


// CTPBaseFrameTips

class CTPBaseFrameTips : public CWnd,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPBaseFrameTips)

public:
	CTPBaseFrameTips();
	virtual ~CTPBaseFrameTips();
public:
	CString  m_sTips;

	int                     m_iTextColor;
	int                     m_iBackColor;
	UINT                    m_uStyle;

	COLORREF                m_clTextColor;
	COLORREF                m_clBackColor;
	CFont                   m_hFont;
public:
	CSize   GetRect();
	void    Create(CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
};


