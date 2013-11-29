// TPBaseFrameToolTips.cpp : implementation file
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPBaseFrameToolTips.h"
#include ".\tpBaseFrametooltips.h"


// CTPBaseFrameTips

IMPLEMENT_DYNAMIC(CTPBaseFrameTips, CWnd)
CTPBaseFrameTips::CTPBaseFrameTips()
{
	m_iTextColor      = COLOR_BASE_TEXT;
	m_iBackColor      = COLOR_BASE_WINDOW;	
	m_iFontIndex      = FONT_BASE_TEXT4;
	m_clTextColor     = RGB(0,0,0);
	m_clBackColor     = RGB(255,0,0);
}

CTPBaseFrameTips::~CTPBaseFrameTips()
{
	if(m_hFont.m_hObject) m_hFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CTPBaseFrameTips, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTPBaseFrameTips message handlers


int CTPBaseFrameTips::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CToolTipCtrl ctl;
	ctl.Create(this,0);
	m_clTextColor = ctl.GetTipTextColor() ;
	m_clBackColor = ctl.GetTipBkColor();
	CFont *pFont = ctl.GetFont();
	LOGFONT logFont;
	pFont->GetLogFont(&logFont);
	m_hFont.CreateFontIndirect(&logFont);
	ctl.DestroyWindow();

	return 0;
}

void CTPBaseFrameTips::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void CTPBaseFrameTips::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CRect rtWnd;
	CFont *pFnt = dc.SelectObject(&m_hFont);
	GetClientRect(&rtWnd);
	dc.FillSolidRect(rtWnd,m_clBackColor);
	dc.SetTextColor(m_clTextColor);
	rtWnd.OffsetRect(3,2);
	dc.DrawText(m_sTips,rtWnd, DT_VCENTER|DT_LEFT);
	rtWnd.OffsetRect(-3,-2);
	dc.Draw3dRect(rtWnd,m_clTextColor,m_clTextColor);
	dc.SelectObject(pFnt);
}
CSize CTPBaseFrameTips::GetRect()
{
	CDC   *pDC  = GetDC();
	CFont *pFnt = pDC ->SelectObject(TP_GetSysFont(FONT_BASE_TEXT));
	pDC ->SelectObject(pFnt);
	CSize szExtent =  pDC ->GetTextExtent(m_sTips);
	ReleaseDC(pDC);
	szExtent.cx += 6;
	szExtent.cy += 4;
	return szExtent;
}
extern CString    g_sRegisterClass;
void CTPBaseFrameTips::Create(CWnd *pParent)
{
	CWnd::CreateEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST, g_sRegisterClass,NULL,WS_POPUP,CRect(0,0,200,10),pParent,0,NULL);
}