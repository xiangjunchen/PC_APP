// TPTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPTreeCtrl.h"
#include ".\tptreectrl.h"

#define  STATE_PAINT      0x00000010
// CTPTreeCtrl

IMPLEMENT_DYNAMIC(CTPTreeCtrl, CTreeCtrl)
CTPTreeCtrl::CTPTreeCtrl()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE; 
	m_eBorderState  = g_eBorderStyle;
	m_pScrollBar    = NULL;
	m_pCheckBox     = TP_LoadImage(g_sSysSkinPath + _L("\\CommonCtrl\\TreeCtrl\\TreeCtrlCheck.bmp"),9,RGB(95,95,95));
	m_pIndent       = TP_LoadImage(g_sSysSkinPath + _L("\\CommonCtrl\\TreeCtrl\\TreeCtrlIdent.bmp"),9,RGB(255,0,255));
	m_pLinePen      = TP_LoadPen(COLOR_BASE_TEXT,PS_SOLID,1);

	m_uState        = 0;  
	m_uStyle        = 0;
	m_pEdit         = NULL;
	m_lEditLimit    = 0;
	m_hEditItem     = NULL; 
	m_bCave         = TRUE;
	m_bReDraw       = TRUE;
	m_hRightSelect  = NULL;
	m_bFous         = FALSE;
	m_bDrawFocus    = FALSE;
	m_pFocusWnd     = NULL;  
	m_hDownItem     = NULL;
	m_iCheckLevel   = -1;
	m_bDownSelect   = FALSE;
}

CTPTreeCtrl::~CTPTreeCtrl()
{	
	SubWindow(FALSE);
	TP_ReleaseCommonRes(m_pCheckBox);
	TP_ReleaseCommonRes(m_pIndent);
	TP_ReleaseCommonRes(m_pLinePen);
}


BEGIN_MESSAGE_MAP(CTPTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
    ON_WM_NCPAINT()
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_LBUTTONDOWN()
    ON_WM_SIZE()
    ON_WM_NCCALCSIZE()
    ON_WM_PARENTNOTIFY()
    ON_WM_SHOWWINDOW()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CTPTreeCtrl 消息处理程序


void CTPTreeCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;			
		SetScrollRange(SB_VERT,0,0);
		SetScrollRange(SB_HORZ,0,0);
		m_pScrollBar = TP_ReplaceWnd(this);
		SaveFont(m_hWnd);

		if(!(TREE_SUBCOMBOBOX & m_uState))
		{		
			m_pScrollBar ->iScrollHeight  = TPSCROLLBAR;
			m_pScrollBar ->iScrollWidth   = TPSCROLLBAR;
			m_pScrollBar ->m_pVScrollInfo = &m_sVScrollInfo;
			m_pScrollBar ->m_pHScrollInfo = &m_sHScrollInfo;
		}		

		GetClientRect(&m_rtClient);
#ifdef TP_NS_EDIT
		m_iBackColor = COLOR_CLIP_TREE;
#else
		if(m_dwWindowLevel == TPWNDLEVEL1)      m_iBackColor = 	COLOR_BASE_WINDOW;
		else if(m_dwWindowLevel == TPWNDLEVEL2) m_iBackColor = 	COLOR_BASE_WINDOW2;
#endif
		ModifyStyle(0,TVS_FULLROWSELECT |  TVS_SHOWSELALWAYS  ,0);

		m_uStyle  = CTPTreeCtrl::GetStyle(); 

		SetFont(TP_GetSysFont(m_iFontIndex));
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
		TP_UnReplaceWnd(this);
		m_pScrollBar = NULL;
		if(m_pEdit) delete m_pEdit;
		m_pEdit = NULL;
		RestoreFont(m_hWnd);
	}
}
int CTPTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;	
	SubWindow(TRUE);
	return 0;
}
BOOL CTPTreeCtrl::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	BOOL bRet = CWnd::ModifyStyle(dwRemove, dwAdd, nFlags);
	if (bRet) m_uStyle = GetStyle();
	return bRet;
}
BOOL CTPTreeCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bCreate = TRUE;

	return CTreeCtrl::PreCreateWindow(cs);
}
void CTPTreeCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

void CTPTreeCtrl::OnDestroy()
{
	SubWindow(FALSE);
	CTreeCtrl::OnDestroy();	
}

void CTPTreeCtrl::OnNcPaint()
{
#ifdef  TP_NS_EDIT
	if(m_bDrawFocus  && (GetFocus() == this || m_pFocusWnd && *m_pFocusWnd == this))
	{	
		CDC    *pDC  = GetWindowDC();		
		CTPDCExchange stuTemp(pDC);
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
		pDC->Draw3dRect(rtWnd,RGB(255,255,255),RGB(255,255,255));		
		stuTemp.ReleaseDC();
		ReleaseDC(pDC);
	}
	else
#endif
	{	
		g_pRoundCorner ->m_iBackColor    = m_iBackColor;
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner ->PaintRect(m_eBorderState,this,m_pScrollBar);
	}
}
BOOL CTPTreeCtrl::GetItemIcon(HTREEITEM hItem,LPBYTE &pBuf,CSize &szIcon,BOOL &bOverTurn)
{
	return FALSE;
}
BOOL CTPTreeCtrl::GetIsCurrent(HTREEITEM hItem)
{
	return FALSE;
}
BOOL CTPTreeCtrl::GetIsSelect(HTREEITEM hItem)
{
	return FALSE;
}
BOOL CTPTreeCtrl::GetItemTextColor(HTREEITEM hItem,COLORREF &clText)
{
	return FALSE;
}
void CTPTreeCtrl::DrawItem(CDC *pDC,HTREEITEM hItem,int iLevel)
{	
	CRect rtText        = CRect(0,0,0,0),rtIdent = CRect(0,0,0,0),rtCheck = CRect(0,0,0,0),rtImage = CRect(0,0,0,0),rtDC;
	CImageList *pImage  = CTreeCtrl::GetImageList(TVSIL_NORMAL);
	CString     sText   = CTreeCtrl::GetItemText(hItem);
	
#ifdef TP_NS_NET
	UINT        uState  = CTreeCtrl::GetItemState(hItem,TVIS_SELECTED|TVIS_EXPANDED|TVIS_DROPHILITED);	
#else
	UINT        uState  = CTreeCtrl::GetItemState(hItem,TVIS_SELECTED|TVIS_EXPANDED);	
	
#endif
	BOOL        bImage  = FALSE;	
	UINT        uStyle  = m_uStyle|TVS_HASLINES;
	BOOL        bCheck  = uStyle &TVS_CHECKBOXES;
	if(m_iCheckLevel>=0 && m_iCheckLevel != iLevel) bCheck = FALSE;

	IMAGEINFO   imgInfo;
	ZeroMemory(&imgInfo,sizeof(IMAGEINFO));
	CTPTreeCtrl::GetItemRect(hItem,&rtText,TRUE);		
	int   iIndent = CTreeCtrl::GetIndent();
	pDC ->SetBkMode(TRANSPARENT);

	rtImage = rtText;
#ifdef TP_NS_EDIT		

	if(m_hDownItem && GetKeyState(VK_LBUTTON) && m_hDownItem != hItem && !(uStyle & TVS_CHECKBOXES)) uState &= ~TVIS_SELECTED;

	CRect rtItem = rtText;rtItem.left = m_rtClient.left;rtItem.right = m_rtClient.right;
	rtItem.DeflateRect(0,1);

	if((uState & (TVIS_SELECTED)) || GetIsSelect(hItem))  
	{
		if(m_bFous ||GetFocus() == this)		pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_SELECTITEMA));
		else      	                           	pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_SELECTITEM));
	}
	else if(m_hRightSelect == hItem)        
	{
		if(GetKeyState(VK_RBUTTON) <0) pDC ->FillSolidRect(rtItem,RGB(130,150,128));	
		else                          
		{
			m_hRightSelect = NULL;
			pDC ->FillSolidRect(rtItem,TP_GetSysColor(m_iBackColor));
		}
	}
	else if((uState & TVIS_DROPHILITED) && !(uStyle & TVS_CHECKBOXES))
	{
		pDC->FillSolidRect(rtItem, TP_GetSysColor(COLOR_BASE_SELECTITEMA));
	}
	else   pDC ->FillSolidRect(rtItem,TP_GetSysColor(m_iBackColor));

	COLORREF clText = TP_GetSysColor(COLOR_BASE_TEXT);
	GetItemTextColor(hItem,clText);
	if(GetIsCurrent(hItem)) clText = TP_GetSysColor(COLOR_VU_ALARM);
	pDC ->SetTextColor(clText);	
#endif
	if(pImage)
	{		
		rtImage.right = rtText.left;
		if(bImage = pImage ->GetImageInfo(0,&imgInfo))
			rtImage.left = rtImage.right - (imgInfo.rcImage.right - imgInfo.rcImage.left);
		else rtImage.left = rtImage.right;
	}
	//else rtImage.left = rtImage.right;
	rtCheck = rtImage;
	if(uStyle & TVS_CHECKBOXES)
	{	
		rtCheck       = rtText;
		rtCheck.left  = rtIdent.right;
		if(bImage)	rtCheck.right = rtImage.left;
		else 		rtCheck.right = rtText.left;
		rtCheck.left = rtCheck.right - rtCheck.Height();
	}
	rtIdent = rtCheck;
	rtIdent.OffsetRect(-2,0);
	rtIdent.left  = rtIdent.left - iIndent;
	rtIdent.right = rtIdent.left + iIndent;

	//画线条	
	if((uStyle & TVS_LINESATROOT)  || iLevel >0)
	{	
		HGDIOBJ hPen = ::SelectObject(pDC->m_hDC,m_pLinePen ->hPen);			
		if(uStyle & TVS_HASLINES)
		{
			HTREEITEM  hParent        = NULL;
			HTREEITEM  hNextSibling   = NULL;
			HTREEITEM  hPrevSibling   = NULL;
			hParent        = CTreeCtrl::GetParentItem(hItem);
			int        iIndex         = iLevel-1;
			while (hParent && iIndex>=((uStyle & TVS_LINESATROOT) ? 0:1))
			{
				if(CTreeCtrl::GetNextSiblingItem(hParent) != NULL)
				{
					pDC ->MoveTo(rtIdent.left - (iLevel - iIndex) * iIndent + iIndent /2,rtIdent.top);
					pDC ->LineTo(rtIdent.left - (iLevel - iIndex) * iIndent + iIndent /2,rtIdent.bottom);
				}
				iIndex --;
				hParent        = CTreeCtrl::GetParentItem(hParent);
			}
			hParent        = CTreeCtrl::GetParentItem(hItem);
			hNextSibling   = CTreeCtrl::GetNextSiblingItem(hItem);
			hPrevSibling   = CTreeCtrl::GetPrevSiblingItem(hItem);
			pDC ->MoveTo(rtIdent.left + iIndent /2 ,rtIdent.top + rtIdent.Height()/2-1);
			pDC ->LineTo(rtIdent.right,rtIdent.top + rtIdent.Height()/2-1);
			if(hNextSibling != NULL)
			{				
				pDC ->MoveTo(rtIdent.left + iIndent /2 ,rtIdent.top+rtIdent.Height()/2);
				pDC ->LineTo(rtIdent.left + iIndent /2 ,rtIdent.bottom);
			}
			if(hParent != NULL || hPrevSibling != NULL)
			{				
				pDC ->MoveTo(rtIdent.left + iIndent /2 ,rtIdent.top+ rtIdent.Height()/2-1);
				pDC ->LineTo(rtIdent.left + iIndent /2, rtIdent.top-1);			
			}
			//有下一个
			::SelectObject(pDC->m_hDC,hPen);
		}
		if(uStyle & TVS_HASBUTTONS)
		{
			if(CTreeCtrl::ItemHasChildren(hItem))
			{
				int         iImageIndex = 0;
				CRect       rtTemp = rtIdent;
				if(uState & TVIS_EXPANDED) iImageIndex = 1;
				rtTemp.DeflateRect((rtIdent.Width() -m_pIndent ->Width())/2,(rtIdent.Height() - m_pIndent ->Height())/2);
				rtTemp.InflateRect(1,1,1,0);				
				pDC ->FillSolidRect(rtTemp,TP_GetSysColor(m_iBackColor));								
				ImageList_Draw(m_pIndent ->GetImage(),iImageIndex,pDC ->m_hDC,rtTemp.left +1,rtTemp.top+1,ILD_TRANSPARENT);			
			}		 
		}
	}	
	//画CheckBox
	if(bCheck)
	{
		UINT        uCheck = TreeView_GetCheckState(m_hWnd,hItem);
		int         iImageIndex =0;
		if(uCheck &&  (uState & (TVIS_SELECTED | TVIS_DROPHILITED))) iImageIndex  = 3;
		else if(uCheck)  iImageIndex  = 0;
		else   iImageIndex  = 1;
		ImageList_Draw(m_pCheckBox ->GetImage(),iImageIndex,pDC ->m_hDC,rtCheck.left + (rtCheck.Width() -  m_pCheckBox ->Width())/2,rtCheck.top +(rtCheck.Height() -  m_pCheckBox ->Height())/2,ILD_TRANSPARENT);
	}
	//画图片
	if(bImage)
	{
		LPBYTE pBuf,pBufTemp;CSize szIcon;BOOL bOverTurn;
		if(GetItemIcon(hItem,pBuf,szIcon,bOverTurn))
		{
			if(m_bCave)
			{
				pBufTemp = new BYTE[szIcon.cx*szIcon.cy*4];
				memcpy(pBufTemp,pBuf,szIcon.cx*szIcon.cy*4);
				COLORREF clBack = TP_GetSysColor(COLOR_BASE_WINDOW);
				BYTE bSave[4] ={0,0,0,0};for(INT l=0;l<4;l++) bSave[l] = pBufTemp[l];
				BYTE bBack[4] ={0,0,0,0};bBack[0] = GetRValue(clBack);bBack[1] = GetGValue(clBack);bBack[2] = GetBValue(clBack);
				for(INT l=0;l<szIcon.cx*szIcon.cy;l++)
				{
					if(pBufTemp[l*4+1] == bSave[1] && pBufTemp[l*4+2] == bSave[2] && pBufTemp[l*4] == bSave[0])
					{
						pBufTemp[l*4+1] = bBack[1];pBufTemp[l*4+2] = bBack[2];pBufTemp[l*4] = bBack[0];
					}
				}
			}
			else pBufTemp = pBuf;
			TP_StretchDIBits(pDC,pBufTemp,szIcon.cx,szIcon.cy,rtImage,bOverTurn);
			if(m_bCave) delete pBufTemp;
		}
		else
		{
			int  iImage,iSelImage;
			CTreeCtrl::GetItemImage(hItem,iImage,iSelImage);
			if((uState & (TVIS_SELECTED | TVIS_DROPHILITED)) || GetIsSelect(hItem)) iImage = iSelImage;
			pImage ->Draw(pDC,iImage,CPoint(rtImage.left,rtImage.top),ILD_TRANSPARENT);
		}
	}
	//画文本
#ifndef TP_NS_EDIT
	if((uState & TVIS_SELECTED) || GetIsSelect(hItem))
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
	else if(!IsWindowEnabled())
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));
	else if(GetIsCurrent(hItem))
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
	else pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));	
#endif
	if(m_hEditItem != hItem)
	{
		CFont *pFont = pDC ->SelectObject(TP_GetSysFont(m_iFontIndex));
		if (uStyle&TVS_CHECKBOXES && !bCheck) rtText.left-=rtText.Height();
		pDC ->DrawText(sText,rtText,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);	
		pDC ->SelectObject(pFont);
	}

}
void CTPTreeCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);	

	NMTVCUSTOMDRAW*	pCD = (NMTVCUSTOMDRAW *)pNMCD;
	if(pCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}	
	else if(pCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{		
		if(!(m_rtClient.bottom <pCD ->nmcd.rc.top ||  m_rtClient.top >pCD ->nmcd.rc.bottom)/* && m_hEditItem != (HTREEITEM)(pCD ->nmcd.dwItemSpec)*/)
		{
			DrawItem(CDC::FromHandle(pCD ->nmcd.hdc),(HTREEITEM)(pCD ->nmcd.dwItemSpec),pCD ->iLevel);		
		}
		*pResult =  CDRF_SKIPDEFAULT;		
	}	
	else 	*pResult =  CDRF_DODEFAULT;		
}
HBRUSH CTPTreeCtrl::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC ->SetBkMode(TRANSPARENT);
#ifdef TP_NS_EDIT
	return TP_GetSysBrush(BRUSH_CLIPTREE);
#else  
	return TP_GetSysBrush(BRUSH_WINDOW);	
#endif
}

void CTPTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_hDownItem = CTreeCtrl::HitTest(point,&nFlags);
	if(m_bDownSelect && m_hDownItem && (TVHT_ONITEM & nFlags)) 
		SelectItem(m_hDownItem);
	CTreeCtrl::OnLButtonDown(nFlags, point);
	m_hDownItem = NULL;
	m_bLeftDowm = TRUE;
}
void CTPTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	BOOL bOnCheckBox = FALSE;
	if (m_uStyle&TVS_CHECKBOXES)
	{
		UINT nHitTestFlags;
		HTREEITEM hCurrentItem = CTreeCtrl::HitTest(point, &nHitTestFlags);
		if (nHitTestFlags & TVHT_ONITEMSTATEICON)
		{
			int nCurrentLevel = 0;
			HTREEITEM hItem = hCurrentItem;
			while (hItem = GetParentItem(hItem)) ++nCurrentLevel;
			if (nCurrentLevel == m_iCheckLevel) 
			{
				SetCheck(hCurrentItem, !GetCheck(hCurrentItem));
				bOnCheckBox=TRUE; 
			}
		}
	}

	if (!bOnCheckBox) CTreeCtrl::OnLButtonDblClk(nFlags, point);
}
void CTPTreeCtrl::OnSize(UINT nType, int cx, int cy)
{

	CTreeCtrl::OnSize(nType, cx, cy);
	if(m_pScrollBar == NULL) return;
	GetClientRect(&m_rtClient);

	CRect rtWnd,rtClient;
	GetWindowRect(&rtWnd);
	GetClientRect(&rtClient);
	ClientToScreen(&rtClient);
	rtClient . OffsetRect(-rtWnd.left,-rtWnd.top);
	rtWnd    . OffsetRect(-rtWnd.left,-rtWnd.top);

	if(m_pScrollBar != NULL)  //zhaofei m_pScrollBar可能为NULL，引起系统异常
	{
		if(abs((rtClient.left - rtWnd.left) - (rtWnd.right-rtClient.right))>10)   m_pScrollBar ->bShowVScrollBar = TRUE;
		else                                                                      m_pScrollBar ->bShowVScrollBar = FALSE;
		if(abs((rtClient.top - rtWnd.top) - (rtWnd.bottom - rtClient.bottom))>10) m_pScrollBar ->bShowHScrollBar = TRUE;
		else                                                                      m_pScrollBar ->bShowHScrollBar = FALSE;
	}
	if(m_hEditItem && m_pEdit)
	{
		CRect rtEdit;
		GetItemRect(m_hEditItem,rtEdit,TRUE);
		m_pEdit ->MoveWindow(rtEdit);
	}
}

void CTPTreeCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CTreeCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
	int iScrollWidth     = ::GetSystemMetrics(SM_CXVSCROLL);
	int iScrollHeight    = ::GetSystemMetrics(SM_CYHSCROLL);
	if(bCalcValidRects && lpncsp ->lppos->hwnd == m_hWnd)
	{
		if(m_sVScrollInfo.nMax>m_sVScrollInfo.nMin && m_sVScrollInfo.nMax- m_sVScrollInfo.nMin + 1 > (int)m_sVScrollInfo.nPage)
			lpncsp ->rgrc[0].right   += iScrollWidth -  TPSCROLLBAR;
		if(m_sHScrollInfo.nMax>m_sHScrollInfo.nMin && m_sHScrollInfo.nMax- m_sHScrollInfo.nMin + 1 > (int)m_sHScrollInfo.nPage)
			lpncsp ->rgrc[0].bottom  += iScrollHeight -  TPSCROLLBAR;	
	}
}

void CTPTreeCtrl::OnParentNotify(UINT message, LPARAM lParam)
{
	CTreeCtrl::OnParentNotify(message, lParam);
	if(LOWORD(message) == WM_CREATE && m_pEdit == NULL)
	{	
		m_pEdit  = new CTPEdit();
		m_pEdit ->SubclassWindow((HWND)lParam);			
		if(m_lEditLimit>0) m_pEdit ->SetLimitText(m_lEditLimit);
		SetClassLongPtr(m_pEdit->m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);
		m_hEditItem = CTPTreeCtrl::GetSelectedItem();
	}	
	if(LOWORD(message) == WM_DESTROY && m_pEdit )
	{
		m_pEdit ->UnsubclassWindow();
		delete m_pEdit;
		m_pEdit = NULL;
		m_hEditItem = NULL;
	}	
}

void CTPTreeCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTreeCtrl::OnShowWindow(bShow, nStatus);
	if(bShow && m_pScrollBar)
	{
		CRect rtWnd,rtClient,rtTemp;
		GetWindowRect(&rtWnd);
		GetClientRect(&rtClient);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
		long lEdge = rtClient.left - rtWnd.left;
		if(rtWnd.bottom - rtClient.bottom >4) 
		{
			rtTemp      = rtWnd;
			rtTemp.top  = rtClient.bottom;
			rtTemp.DeflateRect(lEdge,0,lEdge,lEdge);
			ClientToScreen(&rtTemp);
			GetParent()->ScreenToClient(&rtTemp);
			m_pScrollBar ->bShowHScrollBar = TRUE;
			m_pScrollBar ->pHScroll ->ShowWindow(SW_SHOW);
			m_pScrollBar ->pHScroll ->MoveWindow(rtTemp);

		}
		if(rtWnd .right - rtClient.right >4) 
		{
			rtTemp       = rtWnd;
			rtTemp.left  = rtClient.right;
			rtTemp.DeflateRect(0,lEdge,lEdge,lEdge);
			ClientToScreen(&rtTemp);
			GetParent()->ScreenToClient(&rtTemp);
			m_pScrollBar ->bShowVScrollBar = TRUE;
			m_pScrollBar ->pVScroll ->ShowWindow(SW_SHOW);
			m_pScrollBar ->pVScroll ->MoveWindow(rtTemp);
		}
	}	
}
void CTPTreeCtrl::SetReDraw(BOOL bReDraw)
{
	m_bReDraw = bReDraw;
	if(m_pScrollBar->pHScroll && m_pScrollBar->bShowHScrollBar) m_pScrollBar->pHScroll ->SetRedraw(bReDraw);
	if(m_pScrollBar->pVScroll && m_pScrollBar->bShowVScrollBar) m_pScrollBar->pVScroll ->SetRedraw(bReDraw);
	__super::SetRedraw(bReDraw);
}
void CTPTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
#ifdef TP_NS_EDIT
	m_hRightSelect = HitTest(point);
	if(m_hRightSelect != NULL) Invalidate(NULL);
#endif
	__super::OnRButtonDown(nFlags, point);
	m_bLeftDowm = FALSE;
}

void CTPTreeCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
#ifdef TP_NS_EDIT	
	if(m_hRightSelect != NULL){m_hRightSelect = NULL; Invalidate(NULL);}	
#endif
	__super::OnRButtonUp(nFlags, point);
}
void CTPTreeCtrl::SetCheckLevel(int iLevel,BOOL bDownSelect)
{
	m_iCheckLevel = iLevel;
	m_bDownSelect = bDownSelect;
}
BOOL CTPTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(m_uBaseState & TP_BASESTATE_ERASE) return TRUE;
	CRect rtClient;
	GetClientRect(&rtClient);
	pDC ->FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));
	return TRUE;
	// TODO: Add your message handler code here and/or call default

	return __super::OnEraseBkgnd(pDC);
}
