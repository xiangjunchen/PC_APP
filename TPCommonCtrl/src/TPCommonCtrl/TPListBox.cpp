// TPListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPListBox.h"
#include ".\tplistbox.h"


// CTPListBox
#define EFFECT_PROPERTY_BKCOLOUR   RGB(160,160,160)

IMPLEMENT_DYNAMIC(CTPListBox, CListBox)
CTPListBox::CTPListBox()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uStyle        = 0;
	m_eBorderStyle  = g_eBorderStyle;	
	m_pScrollBar   = NULL;

	m_bCheckBox     = FALSE; 
	m_pImageList    = NULL;
	m_pCheckBox     = NULL;

	m_iSparteBar    = 0;
	m_uState        = 0;
	m_bForceNoDraw  = FALSE;
	m_iMaxWidth     = 0; 
	m_bFocus        = FALSE;
}

CTPListBox::~CTPListBox()
{	
	SubWindow(FALSE);

	for(INT l=0;l<m_aItemData.GetSize();l++) delete m_aItemData[l];
	m_aItemData.RemoveAll();
	if(m_pCheckBox) TP_ReleaseCommonRes(m_pCheckBox);
	m_pCheckBox = NULL;
	if(m_pImageList)
	{
		m_pImageList ->DeleteImageList();
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

BEGIN_MESSAGE_MAP(CTPListBox, CListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()

	ON_WM_NCPAINT()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_COMPAREITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()	
	ON_MESSAGE(LB_INSERTSTRING,OnInsertString)
	ON_MESSAGE(LB_ADDSTRING,OnAddString)
	ON_MESSAGE(LB_DELETESTRING,OnDelString)
	ON_MESSAGE(LB_RESETCONTENT,OnResetContent)	
	ON_MESSAGE(LB_SETCURSEL,OnSetCurSel)
	ON_MESSAGE(LB_GETCURSEL,OnGetCurSel)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff,       OnToolTipNotify )
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

LRESULT CTPListBox::OnInsertString(WPARAM wp,LPARAM lp)
{
	int iIndex      = int(wp);
	_tagItem *pItem = new _tagItem;
	if(iIndex  <0) m_aItemData.Add(pItem);
	else           m_aItemData.InsertAt(iIndex,pItem);
	return m_pfnSuper(m_hWnd,LB_INSERTSTRING,wp,lp);
}
LRESULT CTPListBox::OnSetCurSel(WPARAM wp,LPARAM lp)
{
	int iSel = (int)wp;
	if(iSel>=0 && iSel<m_aItemData.GetSize() && !m_aItemData[iSel]->bItemEnable) iSel = -1;
	wp = iSel;
	return m_pfnSuper(m_hWnd,LB_SETCURSEL,wp,lp);
}
LRESULT CTPListBox::OnGetCurSel(WPARAM wp,LPARAM lp)
{
	int iSel = (int) m_pfnSuper(m_hWnd,LB_GETCURSEL,wp,lp);
	if(iSel>=0 && iSel<m_aItemData.GetSize() && !m_aItemData[iSel]->bItemEnable) iSel = -1;
	return iSel;
}
LRESULT CTPListBox::OnResetContent(WPARAM wp,LPARAM lp)
{
	for(INT l=0;l<m_aItemData.GetSize();l++) delete m_aItemData[l];
	m_aItemData.RemoveAll();
	return m_pfnSuper(m_hWnd,LB_RESETCONTENT,wp,lp);
}
LRESULT CTPListBox::OnDelString(WPARAM wp,LPARAM lp)
{
	int iIndex = (int )wp;
	if(iIndex>=0 && iIndex <m_aItemData.GetSize())
	{
		delete m_aItemData[iIndex];
		m_aItemData.RemoveAt(iIndex);
	}
	return m_pfnSuper(m_hWnd,LB_DELETESTRING,wp,lp);
}
LRESULT CTPListBox::OnAddString(WPARAM wp,LPARAM lp)
{
	_tagItem *pItem = new _tagItem;
	m_aItemData.Add(pItem);
	return m_pfnSuper(m_hWnd,LB_ADDSTRING,wp,lp);
}
int CTPListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}
void CTPListBox::OnDestroy()
{
	SubWindow(FALSE);
	CListBox::OnDestroy();	
}
void CTPListBox::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;		
		m_uStyle = GetStyle();	
		SaveFont(m_hWnd);
		if(!(LBS_DISABLENOSCROLL & m_uStyle))
		{
			m_pScrollBar = TP_ReplaceWnd(this); 
		
			if(!(m_uState&LB_SUBCOMBOBOX))
			{			
				m_pScrollBar ->iScrollHeight  = TPSCROLLBAR;
				m_pScrollBar ->iScrollWidth   = TPSCROLLBAR;
				m_pScrollBar ->m_pVScrollInfo = &m_sVScrollInfo;
				m_pScrollBar ->m_pHScrollInfo = &m_sHScrollInfo;
			}
		}
		for(INT l= GetCount()-1;l>=0;l--) //添加初始值
		{
			_tagItem *pItem = new _tagItem;
			m_aItemData.Add(pItem);			
		}
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;
		RestoreFont(m_hWnd);
		if(!(LBS_DISABLENOSCROLL & m_uStyle))
			TP_UnReplaceWnd(this);
		m_pScrollBar = NULL;
	}
}

BOOL CTPListBox::PreCreateWindow(CREATESTRUCT& cs)
{	
	m_bCreate = TRUE;
	return CListBox::PreCreateWindow(cs);
}

void CTPListBox::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

void CTPListBox::OnNcPaint()
{
	if(m_bForceNoDraw) return ;
 	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}

HBRUSH CTPListBox::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC ->SetBkColor(TP_GetSysColor(COLOR_BASE_WINDOW));		
	pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));		
	pDC ->SetBkMode(TRANSPARENT);
	return TP_GetSysBrush(BRUSH_WINDOW);	
}
void CTPListBox::OnPaint()
{
	CPaintDC dc(this); 
	if(m_bForceNoDraw) return;
	CRect     rtClient;
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
	m_pfnSuper(m_hWnd,WM_PRINT,(WPARAM)cdcMemo.m_hDC,(LPARAM)(PRF_CLIENT|PRF_CHECKVISIBLE ));	
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	

	
}

BOOL CTPListBox::OnEraseBkgnd(CDC* pDC)
{	
	if(m_bForceNoDraw) return TRUE;
	//if(m_uStyle & LBS_OWNERDRAWFIXED) return TRUE;
	//else
	{	
		CRect     rtClient;
		GetClientRect(&rtClient);
		pDC->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
		return TRUE;
	}
	//return CListBox::OnEraseBkgnd(pDC);
}
void CTPListBox::DrawText(CDC *pDC,CRect &rtItem,CString &sText)
{
	pDC ->DrawText(sText,rtItem,DT_LEFT | DT_SINGLELINE|DT_VCENTER|DT_WORD_ELLIPSIS);
}
void CTPListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(m_bForceNoDraw) return ;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct ->hDC);

	_tagItem *pItem = NULL;
	if(lpDrawItemStruct ->itemID >=0 &&lpDrawItemStruct ->itemID < (UINT)m_aItemData.GetSize())
		pItem = m_aItemData.GetAt(lpDrawItemStruct ->itemID);

	pDC ->SetStretchBltMode(COLORONCOLOR);

	CRect rtItem = lpDrawItemStruct ->rcItem,rtSubItem;


	if(lpDrawItemStruct ->itemAction != 0x7890) pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
	if(m_bCheckBox && lpDrawItemStruct ->itemID < (UINT)m_aItemData.GetSize())
	{
		rtSubItem = rtItem;
		rtSubItem.left += 5;
		if(rtSubItem.Width() > m_pCheckBox ->Width())
		{
			ImageList_Draw(m_pCheckBox ->GetImage(),m_aItemData.GetAt(lpDrawItemStruct ->itemID)->dwCheckData >0 ? 0:1 ,pDC ->m_hDC,rtSubItem.left  ,rtSubItem.top +(rtSubItem.Height() -  m_pCheckBox ->Height())/2 +1,ILD_TRANSPARENT);
		}
		rtItem.left +=  m_pCheckBox ->Width() +7;
	}	
	if(m_pImageList && (int)lpDrawItemStruct ->itemID>=0 && (int)lpDrawItemStruct ->itemID <m_aItemData.GetSize())
	{
		int iImage = m_aItemData.GetAt(lpDrawItemStruct ->itemID)->iImageData;
		IMAGEINFO imageInfo;
		if(m_pImageList ->GetImageInfo(iImage,&imageInfo) && rtItem.Width() > imageInfo.rcImage.right - imageInfo.rcImage.left)
		{
			m_pImageList ->Draw(pDC,iImage,CPoint(rtItem.left  ,rtItem.top + (rtItem.Height() - (imageInfo.rcImage.bottom - imageInfo.rcImage.top)) /2),ILD_TRANSPARENT);
			rtItem.left += imageInfo.rcImage.right - imageInfo.rcImage.left +  2;
		}
		else rtItem.left += rtItem.Height() +2;
	}
	if((int)lpDrawItemStruct ->itemID>=0 && (int)lpDrawItemStruct ->itemID < (UINT)m_aItemData.GetSize() && m_aItemData[lpDrawItemStruct ->itemID]->pIcon)
	{
		CRect rtIcon = rtItem;
		rtIcon.DeflateRect(1,1);
		rtIcon.right = rtIcon.left + rtIcon.Height() *m_aItemData[lpDrawItemStruct ->itemID]->szIcon.cx/m_aItemData[lpDrawItemStruct ->itemID]->szIcon.cy;
		rtItem.left = rtIcon.right +2;
		TP_StretchDIBits(pDC,m_aItemData[lpDrawItemStruct ->itemID]->pIcon,m_aItemData[lpDrawItemStruct ->itemID]->szIcon.cx,m_aItemData[lpDrawItemStruct ->itemID]->szIcon.cy,rtIcon);
	}	
	if(rtItem.right > rtItem.left)
	{
		TCHAR sText[MAX_PATH];
		ZeroMemory(sText,100*sizeof(TCHAR));
		CListBox::GetText(lpDrawItemStruct ->itemID,sText);

		if(lpDrawItemStruct ->itemAction != 0x7890)
		{
			if(pItem && !pItem ->bItemEnable && pItem ->clClolor ==0 || !IsWindowEnabled())	
			{
				pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));			
			}
			else
			{		
				if (!(LBS_NOSEL & m_uStyle) &&(lpDrawItemStruct->itemAction | ODA_SELECT) &&	(lpDrawItemStruct->itemState & ODS_SELECTED))
				{				
#ifdef TP_NS_EDIT					
					pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_SELECTITEMA));
					if(pItem && pItem ->clClolor !=0) pDC ->SetTextColor(pItem ->clClolor);	//jyn added for effect enable/disable 4151
					else							  pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
#else
					pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
					pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
#endif
				}
				else
				{
					if(pItem && pItem ->clClolor !=0) pDC ->SetTextColor(pItem ->clClolor);				
					else                              pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));				
				}
			}		
		}
		rtItem.left += 2;		
		DrawText(pDC,rtItem,(CString)sText);
	}
	
}

int CTPListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	TCHAR sText1[MAX_PATH],sText2[MAX_PATH];
	GetText(lpCompareItemStruct ->itemID1,sText1);
	GetText(lpCompareItemStruct ->itemID2,sText2);
	return lstrcmp(sText1,sText2);
}
void  CTPListBox::EnableCheck(BOOL bEnable)
{
	m_uStyle  = GetStyle();
	if((m_uStyle & (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS)) != (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS))
	{
#ifdef _DEBUG
		AfxMessageBox(_L("请设置该控件的 LBS_OWNERDRAWFIXED LBS_HASSTRINGS 属性"));
#else
		return;
#endif		
	}
	m_bCheckBox = bEnable;
	if(m_pCheckBox == NULL) 
		m_pCheckBox = TP_LoadImage(g_sSysSkinPath + _L("\\CommonCtrl\\TreeCtrl\\TreeCtrlCheck.bmp"),9,RGB(95,95,95));
	Invalidate(FALSE);
}
BOOL  CTPListBox::GetCheck(int iItem)
{
	if(iItem <0 || iItem >= m_aItemData.GetSize()) return FALSE;
	return m_aItemData.GetAt(iItem)->dwCheckData >0;	
}
void  CTPListBox::SetItemColor(int iItem,COLORREF clItem)
{
	if(iItem <0 || iItem >= m_aItemData.GetSize()) return;
	m_aItemData[iItem] ->clClolor = clItem;	
	Invalidate(FALSE);
}
void  CTPListBox::SetCheck(int iItem,BOOL bCheck)
{
	if(iItem <0 || iItem >= m_aItemData.GetSize()) return;
	m_aItemData[iItem] ->dwCheckData =(DWORD)bCheck;	
	Invalidate(FALSE);
}
void  CTPListBox::SetImageList(CImageList *pImageList)
{
	m_uStyle  = GetStyle();
	if((m_uStyle & (LBS_OWNERDRAWVARIABLE  | LBS_HASSTRINGS)) != (LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS))
	{
#ifdef _DEBUG
		AfxMessageBox(_L("请设置该控件的 LBS_OWNERDRAWVARIABLE LBS_HASSTRINGS 属性"));
#else
		return;
#endif		
	}
	m_uState |= LB_OWNERIMAGE;
	if(m_pImageList)
	{
		m_pImageList ->DeleteImageList();		
		delete m_pImageList;
		m_pImageList = NULL;
	}
	m_pImageList = new CImageList();
	if(!m_pImageList ->Create(pImageList)) 
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}	
}

CImageList* CTPListBox::GetImageList()
{
	return m_pImageList;
}

int  CTPListBox::GetImage(int iItem)
{
	if(iItem <0 || iItem >= m_aItemData.GetSize()) return -1;
	return m_aItemData.GetAt(iItem)->iImageData;
}
void  CTPListBox::SetIcon(int iIndex,LPBYTE pBuffer,CSize szBuffer,BOOL bSave)
{
	if(iIndex <0 || iIndex >= m_aItemData.GetSize()) return ;
	if(m_aItemData[iIndex] ->pIcon && m_aItemData[iIndex] ->bSaveIcon) delete []m_aItemData[iIndex] ->pIcon;
	m_aItemData[iIndex] ->pIcon = NULL;
	m_aItemData[iIndex] ->szIcon =CSize(0,0);
	m_aItemData[iIndex] ->bSaveIcon = FALSE;
	m_uState |= LB_OWNERIMAGE;

	if(bSave)
	{
		m_aItemData[iIndex] ->pIcon = new BYTE[szBuffer.cx * szBuffer.cy *4];
		m_aItemData[iIndex] ->szIcon = szBuffer;
		m_aItemData[iIndex] ->bSaveIcon  = TRUE;
		memcpy_s(m_aItemData[iIndex]->pIcon,szBuffer.cx * szBuffer.cy *4, pBuffer,szBuffer.cx * szBuffer.cy *4);
	}
	else
	{
		m_aItemData[iIndex] ->pIcon = pBuffer;
		m_aItemData[iIndex] ->szIcon = szBuffer;
		m_aItemData[iIndex] ->bSaveIcon  = FALSE;
	}

}
void  CTPListBox::SetImage(int iItem,int iImage)
{
	if(iItem <0 || iItem >= m_aItemData.GetSize()) return;
	m_aItemData[iImage] ->iImageData = iImage;
	Invalidate(FALSE);
}
void CTPListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	
	CDC *pDC = GetDC();
	int iText =0,iCheck = 0,iImge = 0; IMAGEINFO imageInfo;
	CSize szText = pDC ->GetTextExtent(_L("Q")); 
	CRect rtText = CRect(0,0,0,0);
	iText = szText.cy +8;
	if(m_bCheckBox) iCheck = m_pCheckBox ->Height();
	if(m_pImageList && m_pImageList->GetImageInfo(0,&imageInfo))
	{
		iImge = imageInfo.rcImage.bottom - imageInfo.rcImage.top + 4;
	}
	lpMeasureItemStruct ->itemHeight = max(iImge,iCheck); 
	lpMeasureItemStruct ->itemHeight = max((int)lpMeasureItemStruct ->itemHeight,iText);
	ReleaseDC( pDC); //added by zhh on 2007-10-29
	
}
int CTPListBox::GetEnableItem(int iItem,BOOL bUp)
{
	if(bUp)
	{
		for(INT l= iItem-1;l>=0;l--)
		{
			if(m_aItemData[l]->bItemEnable) return l;
		}
		return GetEnableItem(0,FALSE);
	}
	else
	{
		for(INT l= iItem+1;l<m_aItemData.GetSize();l++)
		{
			if(m_aItemData[l]->bItemEnable) return l;
		}
		return GetEnableItem((int)m_aItemData.GetSize()-1,TRUE);
	}
	return iItem;
}
BOOL CTPListBox::GetIsEnable(CPoint &pt)
{
	BOOL bOutSide = TRUE;
	int iIndex = CListBox::ItemFromPoint(pt,bOutSide);
	if(iIndex>=0 && iIndex<m_aItemData.GetSize())
		return  m_aItemData[iIndex]->bItemEnable;
	return TRUE;
}
void CTPListBox::OnLButtonDown(UINT nFlags, CPoint point)
{	
	if(!GetIsEnable(point))
	{
		//{{lzq:bug:XN00028181,2010.03.11
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		ScreenToClient(&rtWnd);		
		if(rtWnd.PtInRect(point)) return;			
		CListBox::OnLButtonDown(nFlags, point);
		//}}
		return;
	}
	if(m_bCheckBox)
	{
		BOOL bOutSide = FALSE; CRect rtItem;
		int iIndex = CListBox::ItemFromPoint(point,bOutSide);
		if(iIndex >=0)
		{
			CListBox::GetItemRect(iIndex,&rtItem);
			rtItem.right = rtItem.left + 15;
			if(rtItem.PtInRect(point))
			{
				DWORD dwData = m_aItemData.GetAt(iIndex)->dwCheckData;				
				m_aItemData[iIndex] ->dwCheckData = !dwData;
				Invalidate(FALSE);	//zx 修改把ID变成Index
				CWnd * pParWnd = GetParent() ;
				GetParent() ->SendMessage(WM_LBITEMSETCHECK,iIndex,(LPARAM)m_hWnd);//GetDlgCtrlID()
			}				
			else CListBox::OnLButtonDown(nFlags, point);
		}	
		else CListBox::OnLButtonDown(nFlags, point);
	}
	else CListBox::OnLButtonDown(nFlags, point);
}
void CTPListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!GetIsEnable(point)) return;

	m_uState |= LB_MOUSESELECT;
	__super::OnLButtonUp(nFlags, point);
}

void CTPListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!GetIsEnable(point)) 	return;
	__super::OnMouseMove(nFlags, point);
}


void CTPListBox::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CListBox::OnNcCalcSize(bCalcValidRects, lpncsp);
	if(m_uState&LB_SUBCOMBOBOX) return;
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


void CTPListBox::InvalidateScrollBar(UINT uType) 
{
	if ((NULL != m_pScrollBar) && (NULL != m_pScrollBar->pVScroll) &&m_pScrollBar ->bShowVScrollBar) 
	{
		((CTPScrollBar *)m_pScrollBar->pVScroll)->SetReDraw(TRUE);
		m_pScrollBar->pVScroll->Invalidate();
  	}
	return;
}




BOOL CTPListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_uStyle & CBS_OWNERDRAWVARIABLE)
	{		
		SetRedraw(FALSE);	
		int iTop = GetTopIndex();
		if(zDelta>0)
		{
			SetTopIndex(iTop -1);
		}
		else 
			SetTopIndex(iTop +1);
		SetRedraw(TRUE);
		Invalidate();
		m_pScrollBar->pVScroll->Invalidate();
		return S_FALSE;
	}
	else return __super::OnMouseWheel(nFlags, zDelta, pt);
}



INT_PTR CTPListBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if(m_uBaseState & TP_BASESTATE_ENABLETOOLTIP)
	{	
		pTI ->hwnd     = m_hWnd;	
		pTI ->lpszText = LPSTR_TEXTCALLBACK;
		pTI ->uId      = (UINT)(UINT_PTR)m_hWnd;
		pTI ->uFlags  |= TTF_IDISHWND;

		return  GetDlgCtrlID();		
	}

	else return __super::OnToolHitTest(point, pTI);
}
BOOL CTPListBox::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	if(m_uBaseState & TP_BASESTATE_ENABLETOOLTIP)
	{	
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;	
		CString  sToolTips;
		int iSel = GetCurSel();
		if(iSel>=0) GetText(iSel,sToolTips);		
		lstrcpyn( pTTT->szText,sToolTips,80);	
		return  TRUE;			
	}
	else return FALSE;
}
void CTPListBox::EnableToolTips(BOOL bEnable)
{
	if(bEnable)
	{	
		m_uBaseState |= 	TP_BASESTATE_ENABLETOOLTIP;
		CListBox::EnableToolTips(TRUE);	
	}
	else
	{
		m_uBaseState &= 	~TP_BASESTATE_ENABLETOOLTIP;
		CListBox::EnableToolTips(FALSE);	
	}
}
void CTPListBox::EnableItem(int iItem,BOOL bEnable)
{
	if(iItem<0||iItem>=m_aItemData.GetSize()) return;
	m_aItemData[iItem]->bItemEnable = bEnable;
}
int CTPListBox::SelectCheck(CTPComboBox *pComboBox,int iSelCheck)
{
	int iSel = iSelCheck;
	if(iSel ==-1)  iSel = (int)m_pfnSuper(m_hWnd,LB_GETCURSEL,0,0);
	if(iSel>=0 && iSel<m_aItemData.GetSize() && !m_aItemData[iSel]->bItemEnable)
	{
		if(GetKeyState(VK_DOWN)<0 || (m_uState & LB_MOUSEDOWN))   iSel = GetEnableItem(iSel,FALSE);
		else if(GetKeyState(VK_UP)<0 || (m_uState & LB_MOUSEUP))  iSel = GetEnableItem(iSel,TRUE);
		else                           iSel = GetEnableItem(iSel,TRUE); 
		if(pComboBox) pComboBox ->SetCurSel(iSel);
		else          m_pfnSuper(m_hWnd,LB_SETCURSEL,iSel,0);
	}
	return iSel;
}

BOOL CTPListBox::PreTranslateMessage(MSG* pMsg)
{	 
	return __super::PreTranslateMessage(pMsg);
}
void CTPListBox::SetLBText(int iItem,CString sText)
{
	if(iItem <0 || iItem >=  GetCount()) return;
	int iSel =  GetCurSel();
	SetRedraw(FALSE);
	DeleteString(iItem);
	InsertString(iItem,sText);
	if(iSel ==  iItem)  SetCurSel(iItem);
	SetRedraw(TRUE);
	Invalidate();
}