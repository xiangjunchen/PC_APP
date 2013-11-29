// TPStatusBar.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPStatusBar.h"
#include ".\tpstatusbar.h"


// CTPStatusBar  

IMPLEMENT_DYNAMIC(CTPStatusBar, CStatusBar)
CTPStatusBar::CTPStatusBar()
{	
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bSubWindow   = FALSE;
	m_bCreate      = FALSE;
	m_uStyle       = 0;
	m_eBorderStyle = g_eBorderStyle;
	m_pSizeBox     = NULL;
	m_pLogImage    = NULL;
	m_iBackColor   = COLOR_BASE_FRAME;
	for(INT l=0;l<5;l++)  m_iPanleImage[l] = -1; 
}

CTPStatusBar::~CTPStatusBar()
{
	TP_ReleaseCommonRes(m_pSizeBox);
	TP_ReleaseCommonRes(m_pLogImage);
}


BEGIN_MESSAGE_MAP(CTPStatusBar, CStatusBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
ON_WM_DESTROY()
ON_WM_CREATE()
ON_WM_NCPAINT()
END_MESSAGE_MAP()



void CTPStatusBar::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	
		m_uStyle = CWnd::GetStyle(); 
		if(m_uStyle & 0x00000100)
			m_pSizeBox = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\Frame\\SizeBox.bmp")); 	
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
	}
}
void CTPStatusBar::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CStatusBar::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

BOOL CTPStatusBar::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bCreate = TRUE;

	return CStatusBar::PreCreateWindow(cs);
}

void CTPStatusBar::OnDestroy()
{
	SubWindow(FALSE);
	CStatusBar::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

int CTPStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);	

	return 0;
}

void CTPStatusBar::OnPaint()
{
	CPaintDC dc(this);
	CDC *pDC =  &dc;//GetDC();
	CRect     rtClient;
	GetClientRect(&rtClient);	
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));
	CFont *pFnt = cdcMemo.SelectObject(TP_GetSysFont(m_iFontIndex));
	UINT uStyle = 0;CRect rtItem;DRAWITEMSTRUCT DrawItemStruct;
	DrawItemStruct.hDC = cdcMemo.m_hDC;	
	for(int l=0;l<m_nCount;l++)
	{
		uStyle = CStatusBar::GetPaneStyle(l);
		if(uStyle & SBT_OWNERDRAW)
		{
		}
		else			
		{
			CStatusBar::GetItemRect(l,&rtItem);
			if(rtItem.left > rtClient.right) break;
			else if(rtItem.right <rtClient.left) continue;
			DrawItemStruct.itemID = l;
			DrawItemStruct.rcItem = rtItem;
			DrawItem(&DrawItemStruct);
		}
	}	
	if(m_uStyle & 0x00000100) //拉动条
	{
		rtItem = rtClient;
		rtItem.top  = rtItem.bottom - m_pSizeBox ->Height();
		rtItem.left = rtItem.right  - m_pSizeBox ->Width();
		TP_SetDIBitsToDevice(&cdcMemo,m_pSizeBox ->GetByte(),m_pSizeBox ->Width(),m_pSizeBox->Height(),rtItem);
	}
	pDC ->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
	cdcMemo.SelectObject(pFnt);
//	ReleaseDC(pDC);
}

void CTPStatusBar::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC    *pDC    = CDC::FromHandle(lpDrawItemStruct ->hDC);
	int     iItem  = lpDrawItemStruct ->itemID;
	CRect   rtItem = lpDrawItemStruct ->rcItem;
	UINT    uStyle = CStatusBar::GetPaneStyle(iItem);

	if(rtItem.Width() < 3 ) return;

	rtItem.DeflateRect(1,1,1,3);
	if(iItem>=0 && iItem < 5 && m_pLogImage)
	{
		ImageList_Draw(m_pLogImage->GetImage(),m_iPanleImage[iItem],pDC ->m_hDC,rtItem.left+4,rtItem.top + (rtItem.Height() - m_pLogImage->Height())/2 ,ILD_TRANSPARENT);
		rtItem.left += 8+ m_pLogImage->Width();
	}
	if(!(uStyle & SBPS_NOBORDERS))  
	{
		pDC ->Draw3dRect(rtItem,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DLEFT));
	}
	if(uStyle & SBPS_POPOUT) 
	{
		rtItem.DeflateRect(1,1);
		pDC ->Draw3dRect(rtItem,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DLEFT));
	}
	if(!(uStyle & SBPS_DISABLED))
	{
		rtItem.DeflateRect(2,2);
		CString sText  = CStatusBar::GetPaneText(iItem);
		if(sText.GetLength()>0)
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
			pDC ->DrawText(sText,rtItem,DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS);
		}
	}
}

BOOL CTPStatusBar::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CStatusBar::OnEraseBkgnd(pDC);
}
void CTPStatusBar::OnNcPaint()
{
	g_pRoundCorner ->PaintRect(TP_BORDER_20040702,this);
	// TODO: Add your message handler code here
	// Do not call CStatusBar::OnNcPaint() for painting messages
}
void CTPStatusBar::SetLogImage(CString sText,int iWidth,COLORREF clMask)
{
	if(m_pLogImage) TP_ReleaseCommonRes(m_pLogImage);
	m_pLogImage = TP_LoadImage(sText,iWidth,clMask);
}
void CTPStatusBar::SetPaneImage(int iIndex,int iImage)
{
	if(iIndex>=0 && iIndex<5) m_iPanleImage[iIndex] = iImage;
}