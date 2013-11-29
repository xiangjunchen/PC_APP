// TPEditSearch.cpp : implementation file
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPEditSearch.h"


// CTPEditSearch

IMPLEMENT_DYNAMIC(CTPEditSearch, CTPEditEx)

CTPEditSearch::CTPEditSearch()
{
	for(INT l=0;l<2;l++)
	{
		m_pCommomRes[l] = NULL;
		m_uCommonRes[l] = 0;
	}	
	m_sCommonText = _T("");
}

CTPEditSearch::~CTPEditSearch()
{
	for(INT l=0;l<2;l++)
	{
		TP_ReleaseCommonRes(m_pCommomRes[l]);
		m_uCommonRes[l] = 0;
	}
}


BEGIN_MESSAGE_MAP(CTPEditSearch, CTPEditEx)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CTPEditSearch::OnNcPaint()
{
	CWnd *pWnd    = this;
	CDC  *pDC     = pWnd ->GetWindowDC();	
	CTPDCExchange stuTemp(pDC);
	CRect rtWnd,rtClient,rtCorner;
	pWnd ->GetWindowRect(&rtWnd);
	pWnd ->GetClientRect(&rtClient);
	pWnd ->ClientToScreen(&rtClient);
	rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
	rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);

	//if(GetFocus() == this)
	//	pDC ->FillSolidRect(rtWnd,TP_GetSysColor(COLOR_BASE_EDITFOCUS));		
	//else
	pDC ->FillSolidRect(rtWnd,TP_GetSysColor(COLOR_BASE_EDIT));		

	if(m_pCommomRes[0])
	{
		rtClient = rtWnd;
		rtClient.left = rtWnd.right - m_pCommomRes[0]->Width() - 4;
		rtClient.DeflateRect(1,2);

		TP_SetDIBitsToDevice(pDC,m_pCommomRes[0]->GetByte(),m_pCommomRes[0]->Width(),m_pCommomRes[0]->Height(),rtClient);
	}

	g_pRoundCorner-> PaintRect(m_eBorderStyle,pDC,rtWnd,1);

	pWnd ->ReleaseDC(pDC);
}
void CTPEditSearch::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{	

	CTPEditEx::OnNcCalcSize(bCalcValidRects, lpncsp);
	if(m_pCommomRes[0])
	{
		lpncsp ->rgrc[0].right -= m_pCommomRes[0]->Width() + 4;
	}
	/*
	int iScrollWidth     = ::GetSystemMetrics(SM_CXVSCROLL);
	int iScrollHeight    = ::GetSystemMetrics(SM_CYHSCROLL);
	if(bCalcValidRects && lpncsp ->lppos->hwnd == m_hWnd)
	{
		if(m_sVScrollInfo.nMax>m_sVScrollInfo.nMin && m_sVScrollInfo.nMax- m_sVScrollInfo.nMin + 1 > (int)m_sVScrollInfo.nPage)
			lpncsp ->rgrc[0].right   += iScrollWidth -  TPSCROLLBAR;
		if(m_sHScrollInfo.nMax>m_sHScrollInfo.nMin && m_sHScrollInfo.nMax- m_sHScrollInfo.nMin + 1 > (int)m_sHScrollInfo.nPage)
			lpncsp ->rgrc[0].bottom  += iScrollHeight -  TPSCROLLBAR;	
	}*/
}
void  CTPEditSearch::SetResourceName(CString &sFileName,UINT uState)
{
	TP_ReleaseCommonRes(m_pCommomRes[0]);
//	m_pCommomRes[0] = TP_LoadBmpByte(sFileName);
//	m_uCommonRes[0] = uState;
}
void   CTPEditSearch::OnDestroy()
{
	CTPEditEx::OnDestroy();
}
// CTPEditSearch message handlers


