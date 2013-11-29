// TPToolTipCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPToolTipCtrl.h"
#include ".\tptooltipctrl.h"

#define TP_ICON_WIDTH 25
// CTPToolTipCtrl

IMPLEMENT_DYNAMIC(CTPToolTipCtrl, CToolTipCtrl)
CTPToolTipCtrl::CTPToolTipCtrl()
{
	m_dwWindowLevel   = TPWNDLEVEL1; 
	m_iTextColor      = COLOR_BASE_TEXT;
	m_iBackColor      = COLOR_BASE_WINDOW;
	m_uState          = 0;
	m_pMsgWnd         = NULL;
	m_iPopState       = 0;  
	m_iFontIndex      = FONT_BASE_TEXT4;
}

CTPToolTipCtrl::~CTPToolTipCtrl()
{
	ClearIconView();
}


BEGIN_MESSAGE_MAP(CTPToolTipCtrl, CToolTipCtrl)
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TTN_POP, OnTtnTooltipPop)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CTPToolTipCtrl::OnPaint()
{
	CPaintDC dc(this);
	PaintDC(&dc);
}
void CTPToolTipCtrl::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CToolTipCtrl::OnWindowPosChanging(lpwndpos);		
	if(lpwndpos ->flags & 0x10)
	{
		if(m_aIconView.GetSize()>0 && m_iPopState == 1)
		{		
			CDC memDC; memDC.CreateCompatibleDC(NULL);
			CFont *pFont = (CFont *)memDC.SelectObject(TP_GetSysFont(m_iFontIndex));
			long lWidth = 0,lHeight = 0;
			for(INT l=0;l<m_aIconView.GetSize();l++)	
			{
				CRect rtText = CRect(0,0,0,0);
				memDC.DrawText(m_aIconView[l]->sNotify ,rtText,DT_CALCRECT);
				lWidth   = max(lWidth,rtText.Width() + TP_ICON_WIDTH);
				m_aIconView[l]->lHeight = max(rtText.Height(),m_aIconView[l]->szSize.cy * TP_ICON_WIDTH / m_aIconView[l]->szSize.cx);
				lHeight +=  m_aIconView[l]->lHeight + 3;
			}
			memDC.SelectObject(pFont);
			memDC.DeleteDC();		

			lpwndpos ->cy = lHeight + 3 ; 
			lpwndpos ->cx = lWidth  + 12 ; 			
		}
		else
		{	
			CDC memDC; memDC.CreateCompatibleDC(NULL);
			CFont *pFont = (CFont *)memDC.SelectObject(TP_GetSysFont(m_iFontIndex));
			CRect rtText = CRect(0,0,0,0);
			memDC.DrawText(m_sToolTips,rtText,DT_CALCRECT);
			memDC.SelectObject(pFont);
			memDC.DeleteDC();			
			lpwndpos ->cy = rtText.Height() + 3 ; 
			lpwndpos ->cx = rtText.Width()  + 12 ; 		//modify by xjc for snp4100030348	
		}
	}	
}
void CTPToolTipCtrl::PaintDC(CDC *pDC)
{	
	pDC ->SetBkMode(TRANSPARENT);
	pDC ->SetStretchBltMode(COLORONCOLOR);
	CFont *pFont = (CFont *)pDC ->SelectObject(TP_GetSysFont(m_iFontIndex));
	if(m_aIconView.GetSize()>0 && m_iPopState == 1)
	{
		long  lWidth = 0,lHeight = 0;
		CRect rtText, rtClient;
		GetClientRect(&rtClient);
		pDC->FillSolidRect(rtClient,m_clBackColor);
		for(INT l=0;l<m_aIconView.GetSize();l++)	
		{
			rtText = CRect(2,lHeight + 2,2+TP_ICON_WIDTH,lHeight + 2+m_aIconView[l]->szSize.cy * TP_ICON_WIDTH / m_aIconView[l]->szSize.cx);
			TP_StretchDIBits(pDC,m_aIconView[l]->pByte,m_aIconView[l]->szSize.cx,m_aIconView[l]->szSize.cy,rtText);
			rtText.InflateRect(1,1);
			pDC ->Draw3dRect(rtText,m_aIconView[l]->clRange,m_aIconView[l]->clRange);
			rtText = CRect(2+TP_ICON_WIDTH + 4,lHeight + 2,rtClient.right,lHeight + m_aIconView[l]->lHeight -2);
			pDC ->SetTextColor(m_aIconView[l]->clRange);
			pDC ->DrawText(m_aIconView[l]->sNotify,rtText,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_WORD_ELLIPSIS);			
			lHeight += m_aIconView[l]->lHeight + 3;
		}
	}
	else
	{
		CRect rtText;
		GetClientRect(&rtText);
		pDC->FillSolidRect(rtText,m_clBackColor);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_clTextColor);
		rtText.DeflateRect(1,1);
		pDC ->DrawText(m_sToolTips,rtText,DT_LEFT|DT_NOPREFIX|DT_WORD_ELLIPSIS);
	}
	pDC ->SelectObject(pFont);	
}

int CTPToolTipCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolTipCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SaveFont(m_hWnd);
	m_clTextColor = GetTipTextColor();
	m_clBackColor = GetTipBkColor();
	SetTipTextColor(m_clBackColor);
	SetFont(TP_GetSysFont(m_iFontIndex));
	return 0;
}
void CTPToolTipCtrl::AddIconView(LPBYTE pByte,CSize &szSize,CString sNotify,COLORREF clRange)
{
	//if(pByte == NULL) return; zx
	_tagIconView *pData = new _tagIconView();
	pData ->pByte = new BYTE[szSize.cx*szSize.cy*4];
	if(pByte) memcpy_s(pData->pByte,szSize.cx*szSize.cy*4, pByte,szSize.cx*szSize.cy*4);
	else memset(pData ->pByte,0,szSize.cx*szSize.cy*4);
	pData ->szSize = szSize;
	pData ->sNotify = sNotify;
	pData ->clRange = clRange;
	m_aIconView.Add(pData);
	m_iPopState = 1;
}
void CTPToolTipCtrl::ClearIconView()
{
	for(INT l=0;l<m_aIconView.GetSize();l++)	
		delete m_aIconView[l];
	m_aIconView.RemoveAll();
}
void CTPToolTipCtrl::OnTtnTooltipPop(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	ClearIconView();
	m_sToolTips.Empty();
}

void CTPToolTipCtrl::OnDestroy()
{
	CWnd *pWnd = GetParent();
	if(pWnd && m_pMsgWnd && m_pMsgWnd != pWnd && m_pMsgWnd->GetSafeHwnd())
		m_pMsgWnd->PostMessage(WM_TOOLTIPS,1,NULL);
	RestoreFont(m_hWnd);
	__super::OnDestroy();

	// TODO: Add your message handler code here
}
