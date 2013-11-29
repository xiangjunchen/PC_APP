// TPListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPListCtrl.h"
#include ".\tplistctrl.h"


// CTPListCtrl

IMPLEMENT_DYNAMIC(CTPListCtrl, CListCtrl)
CTPListCtrl::CTPListCtrl()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bSubWindow   = FALSE;
	m_bCreate      = FALSE;
	m_eBorderStyle = g_eBorderStyle;
	m_pHeadCtrl    = NULL;
	m_uStyle       = 0;
	m_pEdit        = NULL;
	m_pCheckBox    = NULL;
	m_pLinePen[2]  = NULL;
	m_pLinePen[1]  = NULL;
	m_pLinePen[0]  = NULL;

	m_hImageIcon      = NULL; 
	m_hImageSmallIcon = NULL;
	m_pScrollBar      = NULL;

	m_bLine           = FALSE; 

#ifdef TP_NS_EDIT
	m_iBackColorIndex = COLOR_CLIP_LIST;
#else
	m_iBackColorIndex = COLOR_BASE_WINDOW;
#endif
	m_bItemTextColor  = FALSE;

	m_bAscSort        = TRUE;
	m_iColumSort      = -1;	
	m_clrBackColorOdd = -1;
	m_clrBackColorAcc = -1;

}

CTPListCtrl::~CTPListCtrl()
{
	SubWindow(FALSE);


	TP_ReleaseCommonRes(m_pCheckBox);
	TP_ReleaseCommonRes(m_pLinePen[2]);
	TP_ReleaseCommonRes(m_pLinePen[1]);
	TP_ReleaseCommonRes(m_pLinePen[0]);
	m_ilRowHeight.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CTPListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_STYLECHANGED()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	//ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnLvnBeginlabeledit)
	//ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
//	ON_WM_PARENTNOTIFY()
ON_WM_PARENTNOTIFY()
ON_WM_SIZE()
ON_WM_NCCALCSIZE()
ON_WM_NCLBUTTONDOWN()
ON_WM_NCHITTEST()
END_MESSAGE_MAP()



// CTPListCtrl 消息处理程序
BOOL CTPListCtrl::SetItemHeight(int iRowHeight)
{
	CImageList *pil = __super::GetImageList(LVSIL_SMALL);
	if( pil && (pil->GetSafeHandle() == m_ilRowHeight.GetSafeHandle()) )
		return FALSE;

	m_ilRowHeight.DeleteImageList();
	m_ilRowHeight.Create(1,iRowHeight,TRUE|ILC_COLOR32,1,0);   
	__super::SetImageList(&m_ilRowHeight,LVSIL_SMALL);
	return TRUE;
}
void CTPListCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	
		m_pScrollBar = TP_ReplaceWnd(this);
		m_pScrollBar ->iScrollHeight = TPSCROLLBAR;
		m_pScrollBar ->iScrollWidth  = TPSCROLLBAR;
		m_pScrollBar ->m_pVScrollInfo = &m_sVScrollInfo;
		m_pScrollBar ->m_pHScrollInfo = &m_sHScrollInfo;
		SaveFont(m_hWnd);
		CHeaderCtrl *pHeaderCtrl = CListCtrl::GetHeaderCtrl();
		ModifyStyle(0,WS_CLIPCHILDREN);
		ListView_SetExtendedListViewStyle(m_hWnd,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP); 
		if(pHeaderCtrl && m_pHeadCtrl == NULL) 
		{
			m_pHeadCtrl = new CTPHeaderCtrl();
			m_pHeadCtrl ->SubclassWindow(pHeaderCtrl ->m_hWnd);
			CRect rtHead; m_pHeadCtrl ->GetWindowRect(&rtHead);
			m_iHeadHeight = rtHead.Height();
		}
		m_uStyle    = CListCtrl::GetStyle();
		m_uStyleEx = ListView_GetExtendedListViewStyle(m_hWnd);

		if(m_dwWindowLevel == TPWNDLEVEL1) 
		{
			m_iBackColorIndex = COLOR_BASE_WINDOW;
			m_pLinePen[0]     = TP_LoadPen(COLOR_BASE_3DLEFT,PS_SOLID,1);
			m_pLinePen[1]     = TP_LoadPen(COLOR_BASE_3DRIGHT,PS_SOLID,1);
		}
		else
		{
			m_iBackColorIndex = COLOR_BASE_WINDOW2;
			m_pLinePen[0]     = TP_LoadPen(COLOR_BASE_3DLEFT2,PS_SOLID,1);
			m_pLinePen[1]     = TP_LoadPen(COLOR_BASE_3DRIGHT2,PS_SOLID,1);
		}
		m_pLinePen[2]     = TP_LoadPen(COLOR_BASE_TEXT,PS_SOLID,1);
		m_pCheckBox       = TP_LoadImage(g_sSysSkinPath + _L("\\CommonCtrl\\TreeCtrl\\TreeCtrlCheck.bmp"),9,RGB(95,95,95));
		SetFont(TP_GetSysFont(m_iFontIndex));
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	
		TP_UnReplaceWnd(this);
		m_pScrollBar = NULL;
		if(m_pHeadCtrl)
		{
			m_pHeadCtrl ->UnsubclassWindow();
			delete m_pHeadCtrl;
			m_pHeadCtrl = NULL;
		}
		RestoreFont(m_hWnd);
		
	}
}
DWORD CTPListCtrl::SetExtendedStyle(DWORD dwNewStyle)
{
	DWORD dw = CListCtrl::SetExtendedStyle(dwNewStyle);
	m_uStyle    = CListCtrl::GetStyle();
	m_uStyleEx  = ListView_GetExtendedListViewStyle(m_hWnd);
	return dw;
}
void CTPListCtrl::OnStyleChanged(int nStyleType,	LPSTYLESTRUCT lpStyleStruct)
{
	CListCtrl::OnStyleChanged(nStyleType,lpStyleStruct);
	if(m_pHeadCtrl == NULL)
	{
		UINT uStyle = CListCtrl::GetStyle();
		if(uStyle & LVS_REPORT)
		{
			CHeaderCtrl *pHeaderCtrl = CListCtrl::GetHeaderCtrl();
			if(pHeaderCtrl && m_pHeadCtrl == NULL) 
			{
				m_pHeadCtrl = new CTPHeaderCtrl();
				m_pHeadCtrl ->SubclassWindow(pHeaderCtrl ->m_hWnd);
				CRect rtHead; m_pHeadCtrl ->GetWindowRect(&rtHead);
				m_iHeadHeight = rtHead.Height();
			}
		}
	}
	m_uStyle    = CListCtrl::GetStyle();
	m_uStyleEx  = ListView_GetExtendedListViewStyle(m_hWnd);
}
int CTPListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}
void CTPListCtrl::OnDestroy()
{
	SubWindow(FALSE);
	CListCtrl::OnDestroy();
}

BOOL CTPListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bCreate = TRUE;

	return CListCtrl::PreCreateWindow(cs);
}
void CTPListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

HBRUSH CTPListCtrl::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{	
	return TP_GetSysBrush(BRUSH_WINDOW);
}

BOOL CTPListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CListCtrl::OnEraseBkgnd(pDC);
}

void CTPListCtrl::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}

void CTPListCtrl::OnPaint()
{
	CPaintDC dc(this); 	
 	int iTop = 0;	
	
	CRect     rtClient;
	GetClientRect(&rtClient);	m_rtClient= rtClient;
#ifdef TP_NS_EDIT
	if((m_uStyle & 0x00000003) == LVS_REPORT)	iTop = m_iHeadHeight;
#else
	if((m_uStyle & 0x00000003) == LVS_REPORT)	iTop = m_iHeadHeight-1;
#endif
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColorIndex));
	m_pfnSuper(m_hWnd,WM_PRINT,(WPARAM)cdcMemo.m_hDC,(LPARAM)(PRF_CLIENT|PRF_CHECKVISIBLE));	 	
	dc.BitBlt(0,0+iTop,rtClient.Width(),rtClient.Height()-iTop,&cdcMemo,0,0+iTop,SRCCOPY);		
}

void CTPListCtrl::DrawIcon(CDC *pDC,int iItem)
{
	TCHAR  cText[300];
	CRect  rtText,rtImage,rtUnion, rtIntersect;
	LVITEM lvItem;
	lvItem.mask = LVIF_IMAGE | LVIF_INDENT | LVIF_STATE | LVIF_TEXT;		
	lvItem.iItem      = iItem; 
	lvItem.iSubItem   = 0; 	
	lvItem.stateMask  = LVIS_DROPHILITED  | LVIS_FOCUSED | LVIS_SELECTED; 
	lvItem.pszText    = cText; 
	lvItem.cchTextMax = 300; 
	CListCtrl::GetItem(&lvItem);
	//位置	
	CListCtrl::GetItemRect(iItem,rtText,FALSE);
	CListCtrl::GetItemRect(iItem,rtImage,TRUE);
	rtText.top = rtImage.bottom;
	rtUnion = rtText | rtImage;
	rtIntersect = rtUnion & m_rtClient;
	if(rtIntersect.IsRectNull())	return;	//snp4100033256 snp4100032671 xjc 20110303

	CImageList *pImageList = NULL;
	if(m_hImageIcon) pImageList = CImageList::FromHandle(m_hImageIcon);
	else             pImageList = CListCtrl::GetImageList(LVSIL_NORMAL);
	IMAGEINFO   imageInfo;
	if(pImageList && pImageList ->GetImageInfo(lvItem.iImage,&imageInfo))
	{
		CPoint point = CPoint(rtImage.left + (rtImage.Width() - (imageInfo.rcImage.right - imageInfo.rcImage.left)) /2,rtImage.top + (rtImage.Height() - (imageInfo.rcImage.bottom - imageInfo.rcImage.top)) /2);
		pImageList ->Draw(pDC,lvItem.iImage,point,ILD_TRANSPARENT);
		//SIZE szTemp ;
		//szTemp.cx = imageInfo.rcImage.right - imageInfo.rcImage.left + 20;
		//szTemp.cy = imageInfo.rcImage.bottom - imageInfo.rcImage.top + 20;
		////HIMAGELIST hImage ;hImage = pImageList->GetSafeHandle();
		//////pImageList ->DrawEx(pDC,lvItem.iImage,point,szTemp,CLR_NONE,CLR_NONE,ILD_TRANSPARENT);
		////ImageList_DrawEx(hImage,iItem,pDC->m_hDC,point.x,point.y, szTemp.cx,
		////	szTemp.cy, CLR_NONE, CLR_NONE, ILD_TRANSPARENT);
		////pDC->DrawIcon(()
		//CRect rtItem = CRect(point.x,point.y,point.x+szTemp.cx,point.y+szTemp.cy);
		//TP_SetDIBitsToDevice(pDC,(LPBYTE)imageInfo.hbmImage,szTemp.cx,szTemp.cy,rtItem);

	}
	//文本		

	if(lvItem.state & ( LVIS_SELECTED ))
	{
		pDC ->FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_SELECTITEMA));
		if(lvItem.state & LVIS_FOCUSED)
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
			TP_DrawTextInRect(pDC,lvItem.pszText,rtText,DT_CENTER|DT_WORDBREAK|DT_NOPREFIX);		
		}
		else
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));
			pDC ->DrawText(lvItem.pszText,rtText,DT_CENTER|DT_END_ELLIPSIS|DT_WORD_ELLIPSIS|DT_SINGLELINE|DT_NOPREFIX);	
		}
	}
	else if(!IsWindowEnabled())
	{
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));
		pDC ->DrawText(lvItem.pszText,rtText,DT_CENTER|DT_END_ELLIPSIS|DT_WORD_ELLIPSIS|DT_SINGLELINE|DT_NOPREFIX);	
	}
	else
	{
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
		if(lvItem.state & LVIS_FOCUSED) // LI HUI LI 2009-05-21 DQAdb00005154 有Focus时，需要在大Rect把字显示出来
			TP_DrawTextInRect(pDC,lvItem.pszText,rtText,DT_CENTER|DT_WORDBREAK|DT_NOPREFIX);		
		else
			pDC ->DrawText(lvItem.pszText,rtText,DT_CENTER|DT_END_ELLIPSIS|DT_WORD_ELLIPSIS|DT_SINGLELINE|DT_NOPREFIX);	
	}
}
void CTPListCtrl::DrawList(CDC *pDC,int iItem)
{
	TCHAR  cText[300];
	CRect  rtText,rtImage;
	LVITEM lvItem;
	lvItem.mask = LVIF_IMAGE | LVIF_INDENT | LVIF_STATE | LVIF_TEXT;		
	lvItem.iItem      = iItem; 
	lvItem.iSubItem   = 0; 	
	lvItem.stateMask  = LVIS_DROPHILITED  | LVIS_FOCUSED | LVIS_SELECTED; 
	lvItem.pszText    = cText; 
	lvItem.cchTextMax = 300; 
	CListCtrl::GetItem(&lvItem);	
	//位置	
	CListCtrl::GetItemRect(iItem,rtText,FALSE);
	CListCtrl::GetItemRect(iItem,rtImage,TRUE);
	rtText.left = rtImage.right;	

	if(rtImage.bottom <m_rtClient.top || rtText.right < m_rtClient.left || rtImage.left -10 > m_rtClient.right || rtText.top > m_rtClient.bottom) return;

	CImageList *pImageList = NULL;		
	IMAGEINFO   imageInfo;
	BOOL        bImage = FALSE; 
	if(m_hImageSmallIcon) pImageList = CImageList::FromHandle(m_hImageSmallIcon);
	else                  pImageList = CListCtrl::GetImageList(LVSIL_SMALL);
	if(pImageList && pImageList ->GetImageInfo(0,&imageInfo)) bImage = TRUE;	

	

	if(lvItem.state & ( LVIS_SELECTED))
	{
#ifdef  TP_NS_EDIT
		pDC ->FillSolidRect(rtImage|rtText,RGB(80,89,160));
#else
		if(m_uStyle & LVS_SMALLICON)	
			pDC ->FillSolidRect(rtImage ,TP_GetSysColor(COLOR_BASE_WINDOW));
		pDC ->FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
#endif
		if(lvItem.state & LVIS_FOCUSED)	    pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
		else                				pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));				
	}
	else if(!IsWindowEnabled())
	{	
		if(m_uStyle & LVS_SMALLICON)	pDC ->FillSolidRect(rtImage | rtText,TP_GetSysColor(COLOR_BASE_WINDOW));
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));			
	}
	else
	{	
		if(m_uStyle & LVS_SMALLICON)	pDC ->FillSolidRect(rtImage | rtText,TP_GetSysColor(COLOR_BASE_WINDOW));
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));			
	}    
	//Image
	if(pImageList &&  lvItem.iImage>=0) 
	{
		if(bImage)
		{
			if(rtImage.Width() >= imageInfo.rcImage.right - imageInfo.rcImage.left)
			{				
				ImageList_Draw(pImageList->m_hImageList,lvItem.iImage ,pDC ->m_hDC,rtImage.right - (imageInfo.rcImage.right - imageInfo.rcImage.left)  ,rtImage.top +(rtImage.Height() -  (imageInfo.rcImage.bottom - imageInfo.rcImage.top ))/2,ILD_TRANSPARENT);
			}
		}
	}
	//CheckBox
	if((m_uStyleEx & LVS_EX_CHECKBOXES) && rtImage.left > m_pCheckBox ->Width())
	{
		ImageList_Draw(m_pCheckBox ->GetImage(),CListCtrl::GetCheck(iItem) ? 0:1 ,pDC ->m_hDC,rtImage.left  -  m_pCheckBox ->Width() -2  ,rtImage.top +(rtImage.Height() -  m_pCheckBox ->Height())/2,ILD_TRANSPARENT);
	}

	//Text 
	//{XpriNS_Chengdu:2007.10.10:liguozheng:XN00002577 
	//pDC ->DrawText(lvItem.pszText,rtText,DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_LEFT |DT_NOPREFIX);
	CString strText(lvItem.pszText);
	strText.Replace(_T("\40"),_T(" ")); 
	strText.Replace(_T("\r"),_T("")); 
	strText.Replace(_T("\n"),_T("")); 
	pDC ->DrawText(strText,rtText,DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_LEFT |DT_NOPREFIX);
	//}
	//Line
	if(m_uStyleEx & LVS_EX_GRIDLINES)
	{	
		CPen *pPenSave = pDC ->SelectObject(CPen::FromHandle(m_pLinePen[0]->hPen));
		pDC ->MoveTo(m_rtClient.left,rtText.top - (iItem == 0 ? 1:0)); pDC ->LineTo(m_rtClient.right,rtText.top- (iItem == 0 ? 1:0));
		if(iItem == CListCtrl::GetItemCount() - 1)
			pDC ->MoveTo(m_rtClient.left,rtText.bottom); pDC ->LineTo(m_rtClient.right,rtText.bottom);
		pDC ->SelectObject(CPen::FromHandle(m_pLinePen[1]->hPen));
		pDC ->MoveTo(m_rtClient.left,rtText.bottom-1); pDC ->LineTo(m_rtClient.right,rtText.bottom-1);		
		pDC ->SelectObject(pPenSave);
	}
}
void CTPListCtrl::DrawSmallIcon(CDC *pDC,int iItem)
{
	DrawList(pDC,iItem);
}
BOOL CTPListCtrl::DrawReport(CDC *pDC,int iItem,int iSubItem,CRect &rtSub)
{
	return FALSE;
}
void CTPListCtrl::DrawReport(CDC *pDC,int iItem)
{
	TCHAR  cText[300];
	CRect  rtText,rtImage,rtItem,rtHead,rtSubItem;

	HDITEM hdItem;
	hdItem.mask       = HDI_FORMAT;

	LVITEM lvItem;
	lvItem.iItem	  = iItem;
	lvItem.iSubItem	  = 0;
	lvItem.mask       = LVIF_IMAGE | LVIF_TEXT;				
	lvItem.pszText    = cText; 
	lvItem.iImage     = 0;
	lvItem.cchTextMax = 300;

	CListCtrl::GetItem( &lvItem);	// Added by zhh on 2005-08-18
	CListCtrl::GetItemRect(iItem,rtItem,FALSE);

	UINT   uState;	
	uState = CListCtrl::GetItemState( iItem, LVIS_DROPHILITED  | LVIS_FOCUSED | LVIS_SELECTED);

	CImageList *pImageList = NULL;
	if(m_hImageSmallIcon) pImageList = CImageList::FromHandle(m_hImageSmallIcon);
	else                  pImageList = CListCtrl::GetImageList(LVSIL_SMALL);

	IMAGEINFO   imageInfo;
	BOOL        bImage = FALSE; 
	if(pImageList && pImageList ->GetImageInfo(0,&imageInfo)) bImage = TRUE;	


	if(uState & ( LVIS_SELECTED))
	{		
#ifdef TP_NS_EDIT
		if(GetFocus() == this) pDC ->FillSolidRect(rtItem.left,rtItem.top,rtItem.right-rtItem.left,rtItem.Height() ,TP_GetSysColor(COLOR_BASE_SELECTITEMA));
		else  pDC ->FillSolidRect(rtItem.left,rtItem.top,rtItem.right-rtItem.left,rtItem.Height() ,TP_GetSysColor(COLOR_BASE_SELECTITEM));			
#else
		pDC ->FillSolidRect(rtItem.left,rtItem.top,rtItem.right-rtItem.left,rtItem.Height() ,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
#endif 
		if(uState & LVIS_FOCUSED)	pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
		else        				pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));				
	}
	else if(!IsWindowEnabled())
	{		
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));			
	}
	else
	{	
		
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));			
	}

	if( m_bItemTextColor &&  lvItem.iImage != 0)	// by zhh
	{
		pDC ->SetTextColor( TP_GetSysColor((TP_SYSTEM_COLOR)lvItem.iImage));
	}

	int   iCount = m_pHeadCtrl ->GetItemCount();	
	HPEN  hPen   = NULL;
	if(m_bLine) hPen = (HPEN)::SelectObject(pDC->GetSafeHdc(),m_pLinePen[2]->hPen);
	GetSubItemRect(iItem,0,LVIR_BOUNDS,rtHead);

	int iOffset = rtHead.left;
	for(int l=0;l<iCount;l++)
	{
//		GetSubItemRect(iItem,l,LVIR_BOUNDS,rtHead);
		m_pHeadCtrl ->GetItemRect(l,&rtHead);
		rtHead.OffsetRect(iOffset,0);
		m_pHeadCtrl ->GetItem(l,&hdItem);
		rtSubItem = rtHead ;
		rtSubItem.top    = rtItem.top;
		rtSubItem.bottom = rtItem.bottom;
		if(DrawReport(pDC,iItem,l,rtSubItem) == TRUE) continue;

		lvItem.iItem      = iItem; 
		lvItem.iSubItem   = l; 				
		CListCtrl::GetItem(&lvItem);
		//画背景
		if(m_bLine) 
		{
			pDC ->MoveTo(rtSubItem.left, rtSubItem.top);pDC ->LineTo(rtSubItem.left,rtSubItem.bottom);
			pDC ->MoveTo(rtSubItem.right,rtSubItem.top);pDC ->LineTo(rtSubItem.right,rtSubItem.bottom);
		}		
		
		rtSubItem.left += 2;
		if(l==0) //注意画CheckBox
		{ 
			//Check	
			if((m_uStyleEx & LVS_EX_CHECKBOXES) && rtSubItem.Width() > m_pCheckBox ->Width())
			{
				ImageList_Draw(m_pCheckBox ->GetImage(),CListCtrl::GetCheck(iItem) ? 0:1 ,pDC ->m_hDC,rtSubItem.left  ,rtSubItem.top +(rtSubItem.Height() -  m_pCheckBox ->Height())/2,ILD_TRANSPARENT);
				rtSubItem.left +=  m_pCheckBox ->Width();
			}
			 rtSubItem.left += 2;
		}
		//Image
		if(pImageList && (l==0 || (m_uStyleEx & LVS_EX_SUBITEMIMAGES && lvItem.iImage>=0))) 
		{
			if(bImage)
			{
				if(rtSubItem.Width() > imageInfo.rcImage.right - imageInfo.rcImage.left)
					ImageList_Draw(pImageList->m_hImageList,lvItem.iImage ,pDC ->m_hDC,rtSubItem.left  ,rtSubItem.top +(rtSubItem.Height() -  (imageInfo.rcImage.bottom - imageInfo.rcImage.top ))/2,ILD_TRANSPARENT);
				rtSubItem.left +=  imageInfo.rcImage.right - imageInfo.rcImage.left + 2;
			}
			else rtSubItem.left += rtSubItem.Height() + 2;
		}
		//Text 
		if(rtSubItem.left < rtSubItem.right)
		{
			UINT uFromat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
			if(hdItem.fmt & HDF_CENTER)       uFromat |= DT_CENTER; 
			else if(hdItem.fmt & HDF_LEFT)    uFromat |= DT_LEFT;
			else if(hdItem.fmt & HDF_RIGHT)   uFromat |= DT_RIGHT;

			//{XpriNS_Chengdu:2007.10.10:liguozheng:XN00002577 
			//pDC ->DrawText(lvItem.pszText,rtSubItem,uFromat);
			CString strText(lvItem.pszText);
			strText.Replace(_T("\40"),_T(" ")); 
			strText.Replace(_T("\r"),_T("")); 
			strText.Replace(_T("\n"),_T("")); 
			pDC ->DrawText(strText,rtSubItem,uFromat);
			//}
		}
	}	
	if(m_bLine) 
	{
		pDC ->MoveTo(m_rtClient.left, rtItem.top);   pDC ->LineTo(m_rtClient.right,rtItem.top);
		pDC ->MoveTo(m_rtClient.left, rtItem.bottom);pDC ->LineTo(m_rtClient.right,rtItem.bottom);		
	}
	if(m_bLine)::SelectObject(pDC->GetSafeHdc(),hPen);
#ifndef TP_NS_EDIT	
	{	
		CPen *pPenSave = pDC ->SelectObject(CPen::FromHandle(m_pLinePen[0]->hPen));
		pDC ->MoveTo(m_rtClient.left,rtItem.top - (iItem == 0 ? 1:0)); pDC ->LineTo(m_rtClient.right,rtItem.top- (iItem == 0 ? 1:0));
		if(iItem == CListCtrl::GetItemCount() - 1)
			pDC ->MoveTo(m_rtClient.left,rtItem.bottom); pDC ->LineTo(m_rtClient.right,rtItem.bottom);
		pDC ->SelectObject(CPen::FromHandle(m_pLinePen[1]->hPen));
		pDC ->MoveTo(m_rtClient.left,rtItem.bottom-1); pDC ->LineTo(m_rtClient.right,rtItem.bottom-1);		
		pDC ->SelectObject(pPenSave);
	}
#endif

}
void CTPListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if(pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW  ;
	}
	else if(pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{			
	    if((m_uStyle & LVS_LIST) == LVS_LIST)                DrawList(CDC::FromHandle(pNMCD ->hdc),(int)pNMCD ->dwItemSpec);
		else if((m_uStyle & LVS_REPORT) == LVS_REPORT)       DrawReport(CDC::FromHandle(pNMCD ->hdc),(int)pNMCD ->dwItemSpec);
		else if((m_uStyle & LVS_SMALLICON) ==LVS_SMALLICON)  DrawSmallIcon(CDC::FromHandle(pNMCD ->hdc),(int)pNMCD ->dwItemSpec);
		else                                                 DrawIcon(CDC::FromHandle(pNMCD ->hdc),(int)pNMCD ->dwItemSpec);
		*pResult =  CDRF_SKIPDEFAULT;		
	}	
	else 	*pResult =  CDRF_DODEFAULT;	
}

void CTPListCtrl::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(m_pEdit == NULL)
	{		
		CEdit *pEdit = CListCtrl::GetEditControl();
		if(pEdit)
		{		
			m_pEdit = new CTPEdit();
			m_pEdit ->SubclassWindow(pEdit ->m_hWnd);
		}
	}
	*pResult = 0;
}

void CTPListCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(m_pEdit)			
	{
		m_pEdit ->UnsubclassWindow();
		delete m_pEdit;
		m_pEdit = NULL;
	}
	*pResult = 0;
}

void CTPListCtrl::OnParentNotify(UINT message, LPARAM lParam)
{
	CListCtrl::OnParentNotify(message, lParam);
	if(message == WM_CREATE && m_pHeadCtrl == NULL && lParam)
	{
		TCHAR      cClassName[MAX_PATH];			
		ZeroMemory(cClassName,sizeof(TCHAR)*MAX_PATH);
		GetClassName((HWND)lParam,cClassName,MAX_PATH);
		if(lstrcmpi(cClassName,_L("SysHeader32")) == 0)
		{
			m_pHeadCtrl = new CTPHeaderCtrl();
			m_pHeadCtrl ->SubclassWindow((HWND)lParam);
			CRect rtHead; m_pHeadCtrl ->GetWindowRect(&rtHead);
			m_iHeadHeight = rtHead.Height();
		}
	}
}

void CTPListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
	if(m_pHeadCtrl)
	{
		CRect rtHead; m_pHeadCtrl ->GetWindowRect(&rtHead);
		m_iHeadHeight = rtHead.Height();
		m_pHeadCtrl->Invalidate();    
	}	// TODO: 在此处添加消息处理程序代码

}
void CTPListCtrl::SetItemTextColor(int iItem,TP_SYSTEM_COLOR clText)
{
	m_bItemTextColor = TRUE;

	LVITEM stuItem;
	stuItem.mask      = LVIF_IMAGE; 
	stuItem.iItem     = iItem; 
	stuItem.iSubItem  = 0; 	
	stuItem.iImage    = (int)clText; 
	SetItem(&stuItem);

	Invalidate();
}
int CTPListCtrl::SetColumnFmtEx(int nCol, UINT uFlagAdd,UINT uFlagRemove)
{
	if(m_pHeadCtrl == NULL) return 0;
	HDITEM  iItemData;	
	iItemData.mask =  HDI_FORMAT;	

	m_pHeadCtrl ->GetItem(nCol,&iItemData);
	iItemData.fmt &= ~uFlagRemove;
	iItemData.fmt |= uFlagAdd;
	return m_pHeadCtrl ->SetItem(nCol,&iItemData);
}
void CTPListCtrl::SetColumnText(int iColumn,LPCTSTR sText)
{
	if(m_pHeadCtrl == NULL) return;
	if(iColumn <0 || iColumn >= m_pHeadCtrl ->GetItemCount()) return;

	HDITEM  iItemData;	
	iItemData.mask =  HDI_TEXT;	
	m_pHeadCtrl ->GetItem(iColumn,&iItemData);
	iItemData.pszText    = (LPTSTR)sText;	
	iItemData.cchTextMax = lstrlen(sText);	
	m_pHeadCtrl ->SetItem(iColumn,&iItemData);	 
}

void CTPListCtrl::SetColumnFmt(int iColumn,UINT uFlagAdd,UINT uFlagRemove)
{
	if(m_pHeadCtrl == NULL) return;
	if(iColumn <0 || iColumn >= m_pHeadCtrl ->GetItemCount()) return;
	for(INT l=0;l<m_pHeadCtrl ->GetItemCount();l++) m_pHeadCtrl ->SetColumnFmt(l,0,uFlagRemove);
	m_pHeadCtrl ->SetColumnFmt(iColumn,uFlagAdd,0);	
}
BOOL CTPListCtrl::GetSelectItem(int iItem)
{
	return CListCtrl::GetItemState(iItem,LVIS_SELECTED) == LVIS_SELECTED;
}

void CTPListCtrl::SelectItem(int iItem,BOOL bSelect)
{
	if(bSelect) CListCtrl::SetItemState(iItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	else        CListCtrl::SetItemState(iItem,0,LVIS_SELECTED|LVIS_FOCUSED);
}
void CTPListCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);	
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
void CTPListCtrl::SetRedraw(BOOL bRedraw)
{

	CListCtrl::SetRedraw(bRedraw);	

	if(m_pScrollBar)
	{
		if(m_pScrollBar ->pHScroll && m_pScrollBar ->bShowHScrollBar) m_pScrollBar ->pHScroll ->SetRedraw(bRedraw);
		if(m_pScrollBar ->pVScroll && m_pScrollBar ->bShowVScrollBar) m_pScrollBar ->pVScroll ->SetRedraw(bRedraw);
	}
	if(bRedraw)
	{
		InvalidateRect(NULL);		
		if(m_pScrollBar)
		{
			if(m_pScrollBar ->pHScroll && m_pScrollBar ->bShowHScrollBar) m_pScrollBar ->pHScroll ->InvalidateRect(NULL);
			if(m_pScrollBar ->pVScroll && m_pScrollBar ->bShowVScrollBar) m_pScrollBar ->pVScroll ->InvalidateRect(NULL);
		}
	}
}

void CTPListCtrl::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	CRect rtWnd;
	GetWindowRect(&rtWnd);
	if(point.x >= rtWnd.right  -2) 
		return;
	if(point.y >= rtWnd.bottom -2) 
		return;
	CListCtrl::OnNcLButtonDown(nHitTest, point);
}
void CTPListCtrl::SetBackColorIndex(int iIndex)
{
	m_iBackColorIndex = iIndex;
}
void CTPListCtrl::SetLine(BOOL bLine)
{
	m_bLine = bLine;
}

HWND CTPListCtrl::UnsubclassWindow()
{
	SubWindow(FALSE);
	return CListCtrl::UnsubclassWindow();
}

LRESULT CTPListCtrl::OnNcHitTest(CPoint point)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	ClientToScreen(&rtClient);
	if(rtClient.PtInRect(point)) 
		return HTCLIENT;
	else return __super::OnNcHitTest(point);
}
void CTPListCtrl::SetScrollPosKeep(BOOL bSave)
{
	if(m_pScrollBar == NULL) return;

	if(bSave)
	{
		if(m_pScrollBar->pHScroll)
			m_ptScrollPos.x = ((CTPScrollBar *) m_pScrollBar->pHScroll)->GetScrollPos();
		m_ptScrollPos.y = GetTopIndex();
	}
	else
	{
		EnsureVisible(GetItemCount()-1,FALSE);
		EnsureVisible(m_ptScrollPos.y,FALSE);
		CPoint CurScrollPos;
		if(m_pScrollBar->pHScroll)
		{
			CurScrollPos.x = ((CTPScrollBar *) m_pScrollBar->pHScroll)->GetScrollPos();
			Scroll(CSize(m_ptScrollPos.x-CurScrollPos.x,0));
		}
	}
}
static int CALLBACK TP_SortItemDefault(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the list view control.
	CTPListCtrl* pListCtrl = (CTPListCtrl*) (DWORD_PTR)lParamSort;
	CString    strItem1 = pListCtrl->GetItemText((int)lParam1,  pListCtrl ->m_iColumSort);
	CString    strItem2 = pListCtrl->GetItemText((int)lParam2,  pListCtrl ->m_iColumSort);

	BOOL bReturn = lstrcmp(strItem2, strItem1);
	return pListCtrl ->m_bAscSort?bReturn:-bReturn;	
}

void CTPListCtrl::SortItemDefault(int iColumn)
{
	if(m_pHeadCtrl == NULL) return;
	if(iColumn <0)
	{
		m_iColumSort = -1;
		m_bAscSort   = TRUE;	
		SetColumnFmt(0,0 ,(HDF_SORTDOWN | HDF_SORTUP));
	}
	else
	{	
		m_iColumSort = iColumn;
		for(INT l=0;l<GetItemCount();l++) SetItemData(l,l);
		SetColumnFmt(iColumn,m_bAscSort ? HDF_SORTUP : HDF_SORTDOWN ,(HDF_SORTDOWN | HDF_SORTUP));
		SortItems(TP_SortItemDefault,(DWORD_PTR)this);
		m_bAscSort   = !m_bAscSort;	
	}
}
LRESULT CTPListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_CONTEXTMENU)
	{
		CPoint ptCursor = CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		CRect  rtClient;
		GetClientRect(&rtClient);
		ClientToScreen(&rtClient);
		if(!rtClient.PtInRect(ptCursor))
			return S_OK;
	}
	return __super::DefWindowProc(message, wParam, lParam);
}
