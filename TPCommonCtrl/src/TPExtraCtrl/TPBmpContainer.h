/*
 *	�ļ�����BmpContainer.h
 *	���ã�λͼʰȡ�����������
 */
#pragma once

#include "TPInnerButton.h"
// CBmpContainer

class CBmpContainer : public CWnd
{
	DECLARE_DYNAMIC(CBmpContainer)

public:
	CBmpContainer();
	virtual ~CBmpContainer();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int m_nCellHeight;
	int m_nCellWidth;
	int m_nCol;
	int m_nRow;
	BOOL m_bClosed;
	void Close(int index=-3);
	CButton* m_pParentButton;
	CArray<CInnerButton*,CInnerButton*> m_InnerButtonArray;
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
public:
	virtual BOOL Create(POINT pt,CButton* pParentWnd,CArray<CBitmap*,CBitmap*> *pBitmapArray);	
};


