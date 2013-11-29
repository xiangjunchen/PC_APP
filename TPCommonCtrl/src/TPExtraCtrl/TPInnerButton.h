/*
 *	�ļ�����InnerButton.h
 *	���ã�λͼʰȡ������������ϵ��Ի水ť
 */
#pragma once


// CInnerButton

class CInnerButton : public CButton
{
	DECLARE_DYNAMIC(CInnerButton)

public:
	CInnerButton();
	virtual ~CInnerButton();
	void SetBitmap(CBitmap *pBitmap);

protected:
	DECLARE_MESSAGE_MAP()
	BOOL m_bTracking;
	BOOL m_bOver;
	CBitmap* m_pBitmap;
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


