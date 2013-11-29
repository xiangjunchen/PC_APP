// TPVUStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include ".\tpvustatic.h"
#include <math.h>


// CTPVUStatic

#ifdef TP_NS_EDIT
const int DISPLAY_PRECISION = 2;
#else
const int DISPLAY_PRECISION = 1;
#endif

IMPLEMENT_DYNAMIC(CTPVUStatic, CStatic)
CTPVUStatic::CTPVUStatic()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uStyle        = 0;
	m_eBorderStyle  = TP_GetBorderStyle();

	m_dDB  = 0.0;

	m_iTextColor    = COLOR_BASE_TEXT;
	m_iBackColor    = COLOR_BASE_FRAME;
	m_bBorder       = TRUE;

	m_dPan          = 0;
	m_bPan          = FALSE;
	m_bEnable       = TRUE;
	m_dMinText      = 0.099999999;
}

CTPVUStatic::~CTPVUStatic()
{
}

BEGIN_MESSAGE_MAP(CTPVUStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CTPVUStatic::OnPaint()
{
	CPaintDC dc(this); 
	PaintDC(&dc);
}

BOOL CTPVUStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CStatic::OnEraseBkgnd(pDC);
}
BOOL CTPVUStatic::Create(LPCTSTR sText,UINT uStyle,RECT &rtWnd,CWnd *pParentWnd,UINT nID)
{
	return CStatic::Create(sText,uStyle,rtWnd,pParentWnd,nID);
}
void CTPVUStatic::SetDB(double dDB,BOOL bPaint)
{
	double nPrecision = 1.0;
	for (int i=0; i<DISPLAY_PRECISION; ++i)	nPrecision*=10.0;
	dDB = TP_DoubleToInt(dDB*nPrecision)/nPrecision;

	if(TP_IsDoubleEqual(m_dDB, dDB)) return;
	dDB   = min(12,dDB);
	dDB   = max(-120,dDB);
	m_dDB = dDB;
	if(bPaint) InvalidateRect(NULL);
}

void CTPVUStatic::GetDB(double& dDB)
{
	dDB = m_dDB;
}

void CTPVUStatic::GetPan(double& dPan)
{
	dPan = m_dPan;
}
void CTPVUStatic::SetPan(double dPan,BOOL bPaint)
{
	if(m_dPan == dPan) return;
	m_dPan = dPan;
	if(bPaint) InvalidateRect(NULL);
}
void CTPVUStatic::SetMinText(double dMinText)
{
	m_dMinText = dMinText;
}

void CTPVUStatic::PaintDC(CDC *pDC)
{
	CRect rtClient;CString sText;
	GetClientRect(&rtClient);
	CTPMemoDC memoDC(pDC,rtClient.Width(),rtClient.Height());
	if(m_bEnable)
		memoDC.SetTextColor(TP_GetSysColor(m_iTextColor));
	else
		memoDC.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));
	memoDC.SetBkMode(TRANSPARENT);
	CFont *pSaveFont = memoDC.SelectObject(TP_GetSysFont(m_iFontIndex));

	if(m_dwWindowLevel == TPWNDLEVEL3)
	{
		memoDC.FillSolidRect(rtClient,RGB(0,0,0));
		if(m_bBorder) memoDC.Draw3dRect(rtClient,RGB(63,63,63),RGB(63,63,63));
	}
	else
	{
		memoDC.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));
		if(m_bBorder)	memoDC.Draw3dRect(rtClient,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DLEFT));
	}
	if(!m_bPan)
	{
		CString strDisplayFormat;
		strDisplayFormat.Format(_T("%df"), DISPLAY_PRECISION);
		strDisplayFormat = _T("%.") + strDisplayFormat;
		if(m_dDB <=-100)	sText = _T("--");
		else				sText.Format(strDisplayFormat, m_dDB);

		m_sText = sText;
		CSize  szText = memoDC.GetTextExtent(sText);
		if(szText.cx > rtClient.Width()) sText = sText.Left(sText.GetLength()-1);
		memoDC.DrawText(sText,rtClient,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}
	else
	{
		if(m_dPan <0)      sText.Format(_T("L%d%%"),int(-m_dPan));
		else if(m_dPan >0) sText.Format(_T("R%d%%"),int(m_dPan));
		else sText = _T("Center");
		m_sText = sText;
		memoDC.DrawText(sText,rtClient,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}
	memoDC.SelectObject(pSaveFont);
	pDC ->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&memoDC,0,0,SRCCOPY);

}
void CTPVUStatic::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);
	m_bEnable = bEnable;
}
void     CTPVUStatic::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_STATIC,&wndClass);
		wndClass.lpszClassName = WC_VUSTATIC;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_VUSTATIC,AfxGetInstanceHandle());
}
void CTPVUStatic::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		m_uStyle = CStatic::GetStyle();
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;
	}
}
int CTPVUStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}

void CTPVUStatic::OnDestroy()
{
	SubWindow(FALSE);
	CStatic::OnDestroy();
}

BOOL CTPVUStatic::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CStatic::PreCreateWindow(cs);
}

void CTPVUStatic::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}
