// TPTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPTabCtrl.h"
#include ".\tptabctrl.h"


// CTPTabCtrl
#define TABLE_SPACE  _T("    ")
#define TABLE_LENGTH _T("####")


IMPLEMENT_DYNAMIC(CTPTabCtrl, CTabCtrl)
CTPTabCtrl::CTPTabCtrl()
{
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uStyle        = 0;
	m_eBorderStyle  = g_eBorderStyle;
	m_dwWindowLevel = TPWNDLEVEL1; 

	m_pSpinButton  = NULL;
	m_pImageList   = NULL;

	for(int  l=0;l<4;l++)
		m_pTabBmp[l] = NULL;	

	m_iOffsetItem  = 0;
	m_bFoceUnPaint = FALSE;
	m_iBackColor   = COLOR_BASE_FRAME;
	m_bForceALL    = TRUE;  
	m_bDrawFocus   = FALSE;
	
	m_pFont        = NULL;
	m_clTextColor  = 0;
}

CTPTabCtrl::~CTPTabCtrl()
{
	for(int  l=0;l<4;l++)
		TP_ReleaseCommonRes(m_pTabBmp[l]);
	SubWindow(FALSE);
}


BEGIN_MESSAGE_MAP(CTPTabCtrl, CTabCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_PARENTNOTIFY()
	ON_WM_HSCROLL()
//	ON_WM_LBUTTONDBLCLK()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
ON_WM_SIZE()
ON_WM_LBUTTONDOWN()
ON_WM_ENABLE()
END_MESSAGE_MAP()



// CTPTabCtrl 消息处理程序
void   CTPTabCtrl::LoadBitmap()
{
	CString sText = _L("Top");
	if(m_dwWindowLevel == TPWNDLEVEL1)
	{
#ifdef TP_NS_EDIT
		sText = _L("125\\Tab_Top");
#elif  TP_MINI_EDIT
		sText = _L("100\\Tab_Top");
#else
		sText = _L("53\\Tab_Top");
#endif
	}
	else
	{
		sText = _L("53\\Tab_Top");
		m_iBackColor   = COLOR_BASE_FRAME2;
	}

	m_pTabBmp[TABCTRL_DIS]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\TabCtrl\\")  + sText + _L("_Dis.Bmp"));
	m_pTabBmp[TABCTRL_NOR]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\TabCtrl\\")   + sText + _L("_Nor.Bmp"));
	m_pTabBmp[TABCTRL_DOWN] = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\TabCtrl\\")  + sText + _L("_Down.Bmp"));
	m_pTabBmp[TABCTRL_LINE] = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\TabCtrl\\") + sText + _L("_Line.Bmp"));
}
void CTPTabCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	
		m_uStyle = CTabCtrl::GetStyle(); 	
		LoadBitmap();
		SetItemSize(CSize(10,m_pTabBmp[TABCTRL_NOR]->Height()));
	
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
		if(m_pSpinButton)
		{
			m_pSpinButton ->UnsubclassWindow();
			delete m_pSpinButton;
			m_pSpinButton = NULL;
		}	
	}
}
void CTPTabCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->m_hWnd=m_pSpinButton->m_hWnd)    //xl adde 2010-12-29 XN00030443
	{
		InvalidateRect(NULL);
	}
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CTPTabCtrl::PreSubclassWindow()
{
	CTabCtrl::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

BOOL CTPTabCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CTabCtrl::PreCreateWindow(cs);
}
LONG CTPTabCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	CString sText = lpszItem;
	sText += TABLE_LENGTH;
	return CTabCtrl::InsertItem(nItem,sText);
}
int CTPTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}

void CTPTabCtrl::OnDestroy()
{
	SubWindow(FALSE);
	CTabCtrl::OnDestroy();
}

void CTPTabCtrl::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}
void CTPTabCtrl::PaintTab(CDC *pDC)
{
	CRect     rtClient,rtItem = CRect(0,0,0,0);
	GetClientRect(&rtClient);	
	int iCount = CTabCtrl::GetItemCount();
	m_pImageList = CTabCtrl::GetImageList();
	DRAWITEMSTRUCT drawItem;
	drawItem.hDC = pDC ->m_hDC;
	for(INT l=0;l<iCount;l++)
	{
		CTabCtrl::GetItemRect(l,&rtItem);
		if(rtItem.right + m_iOffsetItem < rtClient.left) continue;
		else if(rtItem.left +m_iOffsetItem > rtClient.right) break;
		drawItem.itemID = l;		
		drawItem.rcItem = rtItem;
		DrawItem(&drawItem);
	}	
	int iSel = GetCurSel();
	if(iSel>=0) GetItemRect(iSel,m_rtSelItem);
	else 
	{
		if(iCount >0)		m_rtSelItem = CRect(0,0,0,0);
		else                m_rtSelItem = rtClient;
		m_rtSelItem.bottom = rtItem.bottom;
	}
	m_rtSelItem.OffsetRect(m_iOffsetItem,0);
}
void CTPTabCtrl::OnPaint()
{
	CPaintDC dc(this);
	if(m_bFoceUnPaint) return;	
	CRect     rtClient,rtSpin;
	GetClientRect(&rtClient);	
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));	
	m_rtSelItem = rtClient;
	CFont *pFnt = NULL;
	if(m_pFont) pFnt = cdcMemo.SelectObject(m_pFont);
	else    	pFnt = cdcMemo.SelectObject(TP_GetSysFont(m_iFontIndex));
	if(m_clTextColor != 0) cdcMemo.SetTextColor(m_clTextColor);
	PaintTab(&cdcMemo);
	if(m_pSpinButton && m_pSpinButton ->IsWindowVisible())
	{
		CRect rtSpin;
		m_pSpinButton ->GetWindowRect(&rtSpin);
		ScreenToClient(&rtSpin);
		if (rtSpin.Height()>16)
		{
			rtSpin.DeflateRect(6,4,0,0);  //xl add 特技窗使用，有问题修改 2010-12-30
			m_pSpinButton->MoveWindow(rtSpin);
		}
		cdcMemo.FillSolidRect(rtSpin.left,rtClient.top,rtClient.right - rtSpin.left,rtSpin.bottom - rtClient.top +5 ,TP_GetSysColor(m_iBackColor));			
		dc.ExcludeClipRect(rtSpin);
	}
	int iIndex = TABCTRL_LINE;
	if(m_rtSelItem.right < rtClient.right)
		TP_StretchDIBRect(&cdcMemo,m_pTabBmp[iIndex] ->GetByte(),m_pTabBmp[iIndex]->Width(),m_pTabBmp[iIndex]->Height(),0,0,CRect(m_rtSelItem.right,m_rtSelItem.bottom-2,rtClient.right,m_rtSelItem.bottom),FALSE,FALSE,2);
	if(m_rtSelItem.left > rtClient.left)
		TP_StretchDIBRect(&cdcMemo,m_pTabBmp[iIndex] ->GetByte(),m_pTabBmp[iIndex]->Width(),m_pTabBmp[iIndex]->Height(),0,0,CRect(rtClient.left,m_rtSelItem.bottom -2,m_rtSelItem.left,m_rtSelItem.bottom),FALSE,FALSE,2);	
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
	cdcMemo.SelectObject(pFnt);	
}
void CTPTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct ->hDC);
	CRect   rtItem = lpDrawItemStruct ->rcItem,rtText;	
	TCITEM  tcItem; TCHAR cText[100];CString sText;
	tcItem.mask        = TCIF_IMAGE | TCIF_STATE | TCIF_TEXT;
	tcItem.dwStateMask = TCIS_BUTTONPRESSED | TCIS_HIGHLIGHTED;
	tcItem.cchTextMax  = 100;
	tcItem.pszText     = cText;
	if(!CTabCtrl::GetItem(lpDrawItemStruct ->itemID,&tcItem)) return;
	lpDrawItemStruct ->itemState = tcItem.dwState;
	sText             = cText;
	sText.Replace(TABLE_LENGTH,TABLE_SPACE);
	
	int iIndex = 0;
	if(tcItem.dwState & TCIS_BUTTONPRESSED)	     {	 pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS)); iIndex = TABCTRL_DOWN; }
	else if(tcItem.dwState & TCIS_HIGHLIGHTED)	 {	 pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_HIGHLIGHT)); iIndex = TABCTRL_NOR ; }
	else if(!IsWindowEnabled())	                 {   pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));   iIndex = TABCTRL_DIS ; }
	else                                    	 {   pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));      iIndex = TABCTRL_NOR ; }

	rtItem.OffsetRect(m_iOffsetItem,0);

	rtItem.InflateRect(1,0);
	TP_StretchDIBRect(pDC,m_pTabBmp[iIndex] ->GetByte(),m_pTabBmp[iIndex]->Width(),m_pTabBmp[iIndex]->Height(),10,10,rtItem,FALSE,FALSE,2);
	rtItem.DeflateRect(1,0);

	pDC ->DrawText(tcItem.pszText,rtText,DT_LEFT | DT_SINGLELINE | DT_VCENTER|DT_CALCRECT);
	IMAGEINFO imageInfo;
	if(m_pImageList && tcItem.iImage>=0 && m_pImageList ->GetImageInfo(tcItem.iImage,&imageInfo))
	{
		CRect rtImage = imageInfo.rcImage;
		if(m_uStyle & TCS_FORCEICONLEFT)
		{
			m_pImageList ->Draw(pDC,tcItem.iImage,CPoint(rtItem.right - (rtImage.Width() + 6) ,rtItem.top + (rtItem.Height() - rtImage.Height())/2),ILD_TRANSPARENT);
			rtItem.right -= rtImage.Width() + 6;
		}
		else
		{
			m_pImageList ->Draw(pDC,tcItem.iImage,CPoint(rtItem.left + 6,rtItem.top + (rtItem.Height() - rtImage.Height())/2),ILD_TRANSPARENT);
			rtItem.left += rtImage.Width() + 6;
		}
	}		
	rtItem.DeflateRect(6,1,6,0);//XN00011829 jw 20080604
	if(m_bDrawFocus && GetFocus() == this && (tcItem.dwState & (TCIS_BUTTONPRESSED|TCIS_HIGHLIGHTED))) pDC ->DrawFocusRect(rtItem);
	pDC ->DrawText(sText,rtItem,DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	
}
BOOL CTPTabCtrl::OnEraseBkgnd(CDC* pDC)
{	
	if(m_uStyle & TCS_MULTILINE )
	{	
		CRect     rtClient;
		GetClientRect(&rtClient);		
		pDC->FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));	
	}
	return TRUE;	
	return CTabCtrl::OnEraseBkgnd(pDC);
}

void CTPTabCtrl::OnParentNotify(UINT message, LPARAM lParam)
{
	CTabCtrl::OnParentNotify(message, lParam);
	if(LOWORD(message) == WM_CREATE && m_pSpinButton == NULL)
	{	
		m_pSpinButton  = new CTPSpinButtonCtrl();
		m_pSpinButton ->SetSkinPath(g_sSysSkinPath + _L("\\CommonCtrl\\SpinButtonCtrl\\1616"));
		m_pSpinButton ->SubclassWindow((HWND)lParam);	
		SetClassLongPtr(m_pSpinButton->m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);
	}	
	if(LOWORD(message) == WM_DESTROY && m_pSpinButton )
	{
		m_pSpinButton ->UnsubclassWindow();
		delete m_pSpinButton;
		m_pSpinButton = NULL;
	}
	if(LOWORD(message) == WM_LBUTTONDOWN && m_pSpinButton )
	{
		InvalidateRect(NULL);
	}
}
void CTPTabCtrl::SetItemText(int iIndex,CString lpszItem)
{
	TCITEM tmSet;
	tmSet.pszText = (LPTSTR)(LPCTSTR)lpszItem;
	tmSet.mask    = TCIF_TEXT;	
	CTabCtrl::SetItem(iIndex,&tmSet);
}
void CTPTabCtrl::SetFullDraw(BOOL bSet)
{
	m_bForceALL = bSet;
}
int CTPTabCtrl::GetMinHeight()
{
	return max(m_pTabBmp[TABCTRL_NOR]->Height(),m_pTabBmp[TABCTRL_DOWN]->Height());
}
void CTPTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
#ifdef TP_NS_EDIT
	if(m_pSpinButton)
	{
		CRect rtWnd;
		m_pSpinButton ->GetWindowRect(&rtWnd);
		rtWnd.DeflateRect(6,4,0,0);
		ScreenToClient(&rtWnd);
		m_pSpinButton->MoveWindow(rtWnd);
	}
#endif
}
void CTPTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
		Invalidate(TRUE);
	__super::OnLButtonDown(nFlags, point);
	//if(GetFocus() != this) SetFocus();
	//DQAdb00005232 刷新背景清除文字残留 qushaojie 20091014
	//Invalidate(TRUE);
}

void CTPTabCtrl::OnEnable(BOOL bEnable)
{
	InvalidateRect(NULL);
	__super::OnEnable(bEnable);

	// TODO: Add your message handler code here
}
