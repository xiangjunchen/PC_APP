#include "StdAfx.h"
#include ".\tpbasectrl.h"

SBDictionary  CTPBaseCtrl::m_stuSBDictionary;

CTPBaseCtrl::CTPBaseCtrl(void)
{
	m_dwWindowLevel = TPWNDLEVEL1;
	m_iFontIndex    = FONT_BASE_TEXT;
	m_uBaseState    = 0;
	m_lParam        = NULL; 
	DestoryNotify   = NULL;
	m_hBaseFont     = NULL;
	m_pRes          = NULL;
	m_lRes          = 0;
}
CTPBaseCtrl::~CTPBaseCtrl(void)
{
}
void CTPBaseCtrl::SaveFont(HWND hWnd)
{
	if(m_hBaseFont == NULL)
		m_hBaseFont  = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);	
	if(m_hBaseFont == NULL)
		m_hBaseFont  = (HFONT)GetStockObject(DEFAULT_GUI_FONT);   
}
void CTPBaseCtrl::RestoreFont(HWND hWnd)
{	
	::SendMessage(hWnd,WM_SETFONT,(WPARAM)m_hBaseFont,TRUE);
	m_hBaseFont = NULL;
}
HFONT CTPBaseCtrl::GetBaseFont(HWND hWnd)
{
	HFONT hFont =  (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);	
	if(hFont == NULL)
		hFont  = (HFONT)GetStockObject(DEFAULT_GUI_FONT);   
	return hFont;
}
void CTPBaseCtrl::SetBaseFont(HWND hWnd,HFONT &hFont)
{
	::SendMessage(hWnd,WM_SETFONT,(WPARAM)hFont,TRUE);
	hFont = NULL;
}