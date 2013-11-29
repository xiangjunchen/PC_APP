// TPTimeCodeStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPTimeCodeStaticex.h"
#include ".\tptimecodestaticex.h"

BOOL CTPTimeCodeStaticEx::m_bInOutConcept = FALSE;
// CTPTimeCodeStaticEx

IMPLEMENT_DYNAMIC(CTPTimeCodeStaticEx, CTPWnd)
CTPTimeCodeStaticEx::CTPTimeCodeStaticEx()
{
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_eBorderStyle  = TP_GetBorderStyle();
	m_dwWindowLevel = TPWNDLEVEL1;	
	m_iBackColor    = COLOR_BASE_WINDOW;
#ifdef  TP_NS_EDIT
	m_iTextColor    = COLOR_BASE_TIMECODE;
#else
	m_iTextColor    = COLOR_BASE_TEXT;
#endif
	
	m_bSign         = FALSE;    


	m_eVideoType    = VIDEO_25;
	m_sText         = _L("00:00:00:00");
	m_iFrame        = 0;
	m_pFont         = TP_GetSysFont(m_iFontIndex);
	m_clText        = RGB(0,0,0);


	for(INT l=0;l<12;l++)
		m_pResNumber[l] = NULL;
	m_pLeftLogo      = NULL;
	m_uLogoPos       = 0;
	m_uTextAlign     = 0;
	m_pBackRes       = NULL;
	m_uPrivateType   = 0;
	m_iBaseTextWidth = 10000;
	m_iBaseTextSignWidth = 10000;
}

CTPTimeCodeStaticEx::~CTPTimeCodeStaticEx()
{
	if(m_pFont && m_pFont != TP_GetSysFont(m_iFontIndex))
	{
		m_pFont ->DeleteObject();
		delete m_pFont;
		m_pFont = NULL;
	}

	for(INT l=0;l<12;l++)
	{
		if(m_pResNumber[l]) TP_ReleaseCommonRes(m_pResNumber[l]);
		m_pResNumber[l] = NULL;
	}
	if(m_pLeftLogo) TP_ReleaseCommonRes(m_pLeftLogo);
	m_pLeftLogo = NULL;
	if(m_pBackRes) TP_ReleaseCommonRes(m_pBackRes);
	m_pBackRes = NULL;	
}


BEGIN_MESSAGE_MAP(CTPTimeCodeStaticEx, CTPWnd)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()


void     CTPTimeCodeStaticEx::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_WND,&wndClass);		
		wndClass.lpszClassName = WC_TPTIMECODESTATIC;		
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_TPTIMECODESTATIC,AfxGetInstanceHandle());
}

// CTPTimeCodeStaticEx 消息处理程序


void CTPTimeCodeStaticEx::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;

		if(m_dwWindowLevel == TPWNDLEVEL1)	m_iBackColor   = COLOR_BASE_FRAME;
		else	                        	m_iBackColor   = COLOR_BASE_FRAME2;
		SaveFont(m_hWnd);
		CTPWnd::SetFont(TP_GetSysFont(m_iFontIndex));
		GetBaseTextWidth();
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;				
		RestoreFont(m_hWnd);
	}
}
void     CTPTimeCodeStaticEx::SetFrame(__int64 iFrame,BOOL bPaint)
{
	m_iFrame = iFrame;
	m_sText = TP_StringFromFrame(m_eVideoType,m_bSign ? abs((int)iFrame) :iFrame);
	if(m_uPrivateType & TP_TIMECODESTATICEX_SHORTCODE)
	{
		if(m_sText[0] == '0' && m_sText[1] == '0') m_sText = m_sText.Left(3);
		if(m_sText[0] == '0' && m_sText[1] == '0') m_sText = m_sText.Left(3);		
		if(m_sText[0] == '0') m_sText = m_sText.Left(1);		
	}
	if(m_bSign) 
	{
		if(iFrame>0)       m_sText = _T("+") + m_sText;
		else if(iFrame<0)  m_sText = _T("-") + m_sText;
	}
	if(bPaint) 
	{
		InvalidateRect(NULL);	
		/*jyn added 20090602 for DQAdb00007011*/
		UpdateWindow();
		/*jyn added end*/
	}
}
__int64    CTPTimeCodeStaticEx::GetFrame()
{
	return m_iFrame;
}

void     CTPTimeCodeStaticEx::SetString(CString strFrame,BOOL bPaint)
{
	m_iFrame = TP_GetFrameFromString(m_eVideoType,strFrame);
	m_sText  = strFrame;
	if(bPaint) Invalidate();
}

void CTPTimeCodeStaticEx::UpdateResDC(CDC *pDC)
{
	ASSERT( m_sText.GetLength() == 11);

	CRect	rtClient;
	GetClientRect(&rtClient);			

	CRect	tmpRect;
	int		iTemp;
	int		iTop  = rtClient.top + ( rtClient.Height() - m_pResNumber[0] ->Height()) / 2;

	for( int i = 0; i < 11; ++ i)
	{
		if( m_sText[i] != m_sTextSave[i])
		{
			iTemp = GetNumberIndex( m_sText[i]);
			CTPMemoDC cdcMemo(pDC, m_pResNumber[iTemp] ->Width(), m_pResNumber[iTemp] ->Height());
			tmpRect.SetRect( 0, 0, m_pResNumber[iTemp] ->Width(), m_pResNumber[iTemp] ->Height());
			TP_StretchDIBits( &cdcMemo, m_pResNumber[iTemp] ->GetByte(), m_pResNumber[iTemp] ->Width(), m_pResNumber[iTemp] ->Height(), tmpRect, FALSE);
			pDC->BitBlt( m_iBmpLeft[i], iTop, rtClient.Width(), rtClient.Height(), &cdcMemo, 0, 0, SRCCOPY);
		}
	}

	m_sTextSave = m_sText;
}
void CTPTimeCodeStaticEx::SetLeftLogo(CString sLogo,UINT uTPos)
{
	m_uLogoPos = uTPos;
	if(m_pLeftLogo) TP_ReleaseCommonRes(m_pLeftLogo);
	m_pLeftLogo = TP_LoadBmpByte(sLogo);
}
void CTPTimeCodeStaticEx::DrawResDC(CDC *pDC)
{	
	ASSERT( m_sText.GetLength() == 11);
	m_sTextSave = m_sText;

	CRect	tmpRect;
	CRect	rtClient;

	GetClientRect(&rtClient);	
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());		
	
	// 绘制背景
	TP_StretchDIBRect( &cdcMemo,m_pResNumber[11] ->GetByte(), m_pResNumber[11] ->Width(), m_pResNumber[11] ->Height(),20,20, rtClient, FALSE);

	// 获取字符绘制位置
	int iTemp;
	int iWidth = 0;

	for( int i = 0; i < 11; ++ i)
	{
		iTemp = GetNumberIndex( m_sText[i]);
		ASSERT( iTemp != -1);
		iWidth += m_pResNumber[i] ->Width();
	}

	iWidth += 20;	// 在数字之间留一定间隔

	int iLeft = rtClient.left + ( rtClient.Width() - iWidth) / 2;
	int iTop  = rtClient.top + ( rtClient.Height() - m_pResNumber[0] ->Height()) / 2;

	// 绘制字串
	for( int i = 0; i < 11; ++ i)
	{
		iTemp = GetNumberIndex( m_sText[i]);
		tmpRect.SetRect( iLeft, iTop, iLeft + m_pResNumber[iTemp] ->Width(), iTop + m_pResNumber[iTemp] ->Height());
		TP_StretchDIBits( &cdcMemo, m_pResNumber[iTemp] ->GetByte(), m_pResNumber[iTemp] ->Width(), m_pResNumber[iTemp] ->Height(), tmpRect, FALSE);

		m_iBmpLeft[i] = iLeft;
		iLeft += m_pResNumber[iTemp] ->Width() + 2;
	}

	// 位图拷贝
	pDC->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	
}
void CTPTimeCodeStaticEx::DrawDC(CDC *pDC)
{
	CRect     rtClient,rtTemp;
	GetClientRect(&rtClient);
	rtTemp   = rtClient;
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());		
	CFont* pOldFont = cdcMemo.SelectObject(m_pFont);
	cdcMemo.SetBkMode(TRANSPARENT); 
	if(m_clText == RGB(0,0,0))	cdcMemo.SetTextColor(TP_GetSysColor(m_iTextColor)); 
	else                   	    cdcMemo.SetTextColor(m_clText); 	
	if(m_pBackRes)
	{
		TP_StretchDIBRect(&cdcMemo,m_pBackRes->GetByte(),m_pBackRes->Width(),m_pBackRes->Height(),10,10,rtClient);
	}
	else
	{
		cdcMemo.FillSolidRect(&rtClient,TP_GetSysColor(m_iBackColor)); 
		if(m_pLeftLogo)
		{
			CRect rtLogo = rtClient;
			rtLogo.left +=3;
			rtLogo.right = rtLogo.left + m_pLeftLogo->Width();
			if(m_uLogoPos == 0)     { rtLogo.top    = rtLogo.bottom - m_pLeftLogo->Height(); rtLogo.OffsetRect(0,-3);}
			else if(m_uLogoPos == 1){ rtLogo.bottom = rtLogo.top + m_pLeftLogo->Height();rtLogo.OffsetRect(0,3);}
			else {rtLogo.top = rtClient.top + (rtClient.Height() - m_pLeftLogo->Height())/2, rtLogo.bottom = rtLogo.top + m_pLeftLogo->Height();}
			TP_SetDIBitsToDevice(&cdcMemo,m_pLeftLogo->GetByte(),m_pLeftLogo->Width(),m_pLeftLogo->Height(),rtLogo);
			rtClient.left = rtLogo.right +3;
		}
	}
	if(m_bSign && m_iFrame!=0)
		rtClient.DeflateRect((rtClient.Width() - m_iBaseTextSignWidth)/2-1,0);
	else
		rtClient.DeflateRect((rtClient.Width() - m_iBaseTextWidth)/2-1,0);
	if(m_uTextAlign == 0)      cdcMemo.DrawText(m_sText,rtClient,DT_LEFT|DT_VCENTER|DT_SINGLELINE); 
	else if(m_uTextAlign == 1) {rtClient.top +=1;     cdcMemo.DrawText(m_sText,rtClient,DT_LEFT|DT_TOP|DT_SINGLELINE); }
	else if(m_uTextAlign == 2) {rtClient.bottom -= 1; cdcMemo.DrawText(m_sText,rtClient,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);} 
	cdcMemo.SelectObject(pOldFont);		
	pDC->BitBlt(0,0,rtTemp.Width(),rtTemp.Height(),&cdcMemo,0,0,SRCCOPY);	

}
void CTPTimeCodeStaticEx::GetBaseTextWidth()
{
	CDC   *pDC       = GetDC();
	CFont *pFont     = pDC ->SelectObject(m_pFont);
	CSize  szText    = pDC->GetTextExtent(_T("99:99:99:99"));
	m_iBaseTextWidth = szText.cx;
	szText           = pDC->GetTextExtent(_T("+99:99:99:99"));
	m_iBaseTextSignWidth = szText.cx;
	pDC ->SelectObject(pFont);
	ReleaseDC(pDC);
}
void CTPTimeCodeStaticEx::SetFont(CFont &ftSet)
{
	if(m_pFont && m_pFont != TP_GetSysFont(m_iFontIndex))
	{
		m_pFont ->DeleteObject();
		delete m_pFont;
		m_pFont = NULL;
	}
	m_pFont = new CFont();
	LOGFONT lgFont;
	ftSet.GetLogFont(&lgFont);
	m_pFont ->CreateFontIndirect(&lgFont);
	GetBaseTextWidth();
	
}
int CTPTimeCodeStaticEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}
void CTPTimeCodeStaticEx::OnPaint()
{
	CPaintDC dc(this); 
	if(m_pResNumber[0] == NULL)	DrawDC(&dc);
	else                   	    DrawResDC(&dc);
}

int CTPTimeCodeStaticEx::GetResHeight()
{
	return m_pResNumber[11]->Height();
}

void CTPTimeCodeStaticEx::SetResPath(CString sPath)
{
	if( sPath.IsEmpty())
	{
		sPath =  TP_GetSysPath();
		sPath += _L("\\TPSkin\\CommonCtrl\\TimeCode\\T1");
	}

	for(INT l=0;l<12;l++)
	{
		if(m_pResNumber[l]) TP_ReleaseCommonRes(m_pResNumber[l]);
		m_pResNumber[l] = NULL;
	}

	CString sName[12] =
	{ _L("0.bmp"),_L("1.bmp"),_L("2.bmp"),_L("3.bmp"),_L("4.bmp"),_L("5.bmp"),_L("6.bmp"),_L("7.bmp"),_L("8.bmp"),_L("9.bmp"),_L("Split.bmp"), _L("Back.bmp")};
	for(INT l=0;l<12;l++)
		m_pResNumber[l] =  TP_LoadBmpByte(sPath + _L("\\") + sName[l]);	
}
void CTPTimeCodeStaticEx::SetBackColor(TP_SYSTEM_COLOR eColorIndex)
{
	m_iBackColor = eColorIndex;
}
void CTPTimeCodeStaticEx::SetTextColor(COLORREF clText)
{
	m_clText = clText;
}
void CTPTimeCodeStaticEx::SetTextColor(TP_SYSTEM_COLOR eColorIndex)
{
	m_iTextColor = (int)eColorIndex;
}

void CTPTimeCodeStaticEx::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}
BOOL CTPTimeCodeStaticEx::Create(UINT uStyle,CRect rectWnd,CWnd *pParent,UINT nID)
{
	return CTPWnd::Create(uStyle,rectWnd,pParent,nID);
}

int CTPTimeCodeStaticEx::GetNumberIndex( TCHAR ch)
{
	if ( ch >= _T('0') && ch <= _T('9'))
		return ch - _T('0');
	else if( ch == _T(':') || ch==_T('.'))
		return 10;

	return -1;
}
void CTPTimeCodeStaticEx::SetBackRes(CString sPath)
{
	if(m_pBackRes) TP_ReleaseCommonRes(m_pBackRes);
	m_pBackRes = TP_LoadBmpByte(sPath);

}
