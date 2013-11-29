// TPTreeCtrlEx.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPTreeCtrlEx.h"
#include ".\tptreectrlex.h"


#define TVIS_MULTISEL     TVIS_BOLD 

// CTPTreeCtrlEx

IMPLEMENT_DYNAMIC(CTPTreeCtrlEx, CTPTreeCtrl)
CTPTreeCtrlEx::CTPTreeCtrlEx()
{	
	m_bBaseView   = FALSE;    
	m_dwRootData  = NULL;
	m_hShiftItem  = NULL;
	CString sSkinPath   = TP_GetSkinPath();
	m_pPenRes[0]  = TP_LoadPen(COLOR_BASE_3DLEFT ,PS_SOLID,1);
	m_pPenRes[1]  = TP_LoadPen(COLOR_BASE_TEXTDIS,PS_SOLID,1);

	
#ifdef TP_MINI_EDIT
	m_pBackRes[TREE_BACK_CHILDSEL]   = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\TreeCtrl\\MINITreeCtrl\\TreeChildBackSel.bmp"));
	m_pBackRes[TREE_BACK_ROOTSEL]    = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\TreeCtrl\\MINITreeCtrl\\TreeRootBackSel.bmp"));
	m_pBackRes[TREE_BACK_ROOTUNSEL]  = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\TreeCtrl\\MINITreeCtrl\\TreeRootBackUnSel.bmp"));
	m_pBackRes[TREE_BACK_CHILDUNSEL] = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\TreeCtrl\\MINITreeCtrl\\TreeChildBackUnSel.bmp"));
	m_pBackRes[TREE_BACK_TRINBTN]    = TP_LoadImage(sSkinPath   + _L("\\CommonCtrl\\ClassCase\\MINITreeCtrl\\ClassCaseTriBtn.bmp"),10,RGB(0,0,0));
#else
	m_pBackRes[TREE_BACK_CHILDSEL]   = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\TreeCtrl\\TreeChildBackSel.bmp"));
	m_pBackRes[TREE_BACK_ROOTSEL]    = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\TreeCtrl\\TreeRootBackSel.bmp"));
	m_pBackRes[TREE_BACK_ROOTUNSEL]  = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\TreeCtrl\\TreeRootBackUnSel.bmp"));
	m_pBackRes[TREE_BACK_CHILDUNSEL] = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\TreeCtrl\\TreeChildBackUnSel.bmp"));
	m_pBackRes[TREE_BACK_TRINBTN]    = TP_LoadImage(sSkinPath   + _L("\\CommonCtrl\\ClassCase\\ClassCaseTriBtn.bmp"),11,RGB(0,0,0));
#endif
}

CTPTreeCtrlEx::~CTPTreeCtrlEx()
{
	for(int l=0;l<2;l++) TP_ReleaseCommonRes(m_pPenRes[l]);
	for(int l=0;l<TREE_BACK_MAX;l++) TP_ReleaseCommonRes(m_pBackRes[l]);
	m_aMultiSel.RemoveAll();
}


BEGIN_MESSAGE_MAP(CTPTreeCtrlEx, CTPTreeCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCPAINT()
	ON_WM_SIZE()	
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()	
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



void     CTPTreeCtrlEx::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_TREEVIEW,&wndClass);		
		wndClass.lpszClassName = WC_TREECTRLEX;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_TREECTRLEX,AfxGetInstanceHandle());
}
void CTPTreeCtrlEx::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		ModifyStyle(0,TVS_HASBUTTONS |TVS_EDITLABELS|TVS_FULLROWSELECT | TVS_LINESATROOT | TVS_SHOWSELALWAYS  ,0);		
		CTPTreeCtrl::SubWindow(bSub);		
	}
	else
	{
		CTPTreeCtrl::SubWindow(bSub);
	}
}
COLORREF CTPTreeCtrlEx::GetItemTextColor(HTREEITEM hItem)
{
	return 	TP_GetSysColor(COLOR_BASE_TEXT);	 
}
void CTPTreeCtrlEx::DrawItem(CDC *pDC,HTREEITEM hItem,int iLevel)
{
	if(m_bBaseView)
		CTPTreeCtrl::DrawItem(pDC,hItem,iLevel);
	else
	{
		CRect       rtText,rtItem;
		CString     sText  = CTreeCtrl::GetItemText(hItem);
		UINT        uState = CTreeCtrl::GetItemState(hItem,TVIS_SELECTED|TVIS_EXPANDED);				
		CTPTreeCtrl::GetItemRect(hItem,&rtText,TRUE);	
		CTPTreeCtrl::GetItemRect(hItem,&rtItem,FALSE);	

		pDC ->FillSolidRect(rtItem,TP_GetSysColor(m_iBackColor));

		int     iBackIndex = 0,iTrimIndex = 0;
		CPoint  ptTrim     = CPoint(0,0);
		if((uState &  (TVIS_SELECTED)) && m_hDownItem == NULL || m_hDownItem == hItem)
		{

			iBackIndex = TREE_BACK_CHILDSEL;
			ptTrim     = CPoint(10,5);
			pDC->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
			iTrimIndex = 6;
		}	
		else		
		{
			iBackIndex = TREE_BACK_CHILDUNSEL; 
			pDC->SetTextColor(GetItemTextColor(hItem));	
			ptTrim     = CPoint(10,5);
			iTrimIndex = 4;
		}	
		if(GetItemState(hItem,TVIS_EXPANDED | TVIS_EXPANDEDONCE | TVIS_EXPANDPARTIAL)  & TVIS_EXPANDED )
		{
			iTrimIndex ++;
			ptTrim.Offset(1,0);
		}	
		pDC ->SetBkMode(TRANSPARENT);
		if(uState & (TVIS_SELECTED))		
			pDC ->FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
		
		
		pDC ->DrawText(sText,rtText,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		if(ItemHasChildren(hItem))	
			ImageList_Draw(m_pBackRes[TREE_BACK_TRINBTN] ->GetImage(),iTrimIndex,pDC->m_hDC,rtText.left - ptTrim.x-3 ,rtText.top + ptTrim.y - 3,ILD_NORMAL);

	}
}
void CTPTreeCtrlEx::SetSelState()
{
	HTREEITEM hCurSel  = CTPTreeCtrl::GetSelectedItem();
	if(hCurSel)	SetItemState(hCurSel,TVIS_MULTISEL,TVIS_MULTISEL);
}
void CTPTreeCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_uState &STATE_MULTISEL)
	{		
		if(TP_IsShiftKeyDown() || TP_IsCtrlKeyDown()) return;
		if(ClearMultiSel(NULL)) InvalidateRect(NULL,FALSE);
	}
	if(m_bBaseView)  CTPTreeCtrl::OnLButtonDblClk(nFlags, point);
	else
	{
		HTREEITEM hTreeItem = CTreeCtrl::HitTest(point,&nFlags);
		if(hTreeItem) 	CTPTreeCtrl::Expand(hTreeItem,TVE_TOGGLE);	
		else CTPTreeCtrl::OnLButtonDblClk(nFlags, point);
	}
	SetSelState();
}
void CTPTreeCtrlEx::SetViewType(BOOL bBase)
{
	m_bBaseView = bBase;
}

BOOL  CTPTreeCtrlEx::SetItemData(HTREEITEM hItem,DWORD_PTR dwData )
{
	if(hItem == NULL)
	{
		m_dwRootData = dwData;
		return TRUE;
	}
	else return CTreeCtrl::SetItemData(hItem,dwData);
}
DWORD_PTR CTPTreeCtrlEx::GetItemData(HTREEITEM hItem ) const
{
	DWORD_PTR dwData = 0;
	if(hItem == NULL) return m_dwRootData;	
	dwData = CTreeCtrl::GetItemData(hItem);
	ASSERT(CB_ERR != dwData);
	if(CB_ERR == dwData) return NULL;
	return dwData;
}
CImageList *CTPTreeCtrlEx::CreateDragImage(HTREEITEM hItem )
{
	if(hItem == NULL) return NULL;
	CString sText = CTreeCtrl::GetItemText(hItem);
	if(sText.GetLength() <=0) return NULL;
	CRect rtItem = CRect(0,0,0,0);
	CDC *pDC = GetDC(),cdcMemo;
	pDC ->DrawText(sText,rtItem,DT_CALCRECT);
	rtItem.InflateRect(4,3,4,3);
	cdcMemo.CreateCompatibleDC(pDC);
	CBitmap bmLoad,bmCreate;BITMAP bmInfo;
	bmLoad.LoadBitmap(_L("IDB_DEFBITMAP"));
	bmLoad.GetBitmap(&bmInfo);
	bmLoad.DeleteObject();
	bmInfo.bmHeight  = rtItem.Height();
	bmInfo.bmWidth   = rtItem.Width();
	bmInfo.bmWidthBytes = rtItem.Width() *4;
	bmCreate.CreateBitmapIndirect(&bmInfo);
	CBitmap *pSaveBmp  = cdcMemo.SelectObject(&bmCreate);
	CFont *pSaveFont   = cdcMemo.SelectObject(TP_GetSysFont(m_iFontIndex)); 
	cdcMemo.SetBkMode(TRANSPARENT);
	cdcMemo.SetStretchBltMode(COLORONCOLOR);
	cdcMemo.FillSolidRect(0,0,bmInfo.bmWidth,bmInfo.bmHeight,TP_GetSysColor(m_iBackColor));
	cdcMemo.DrawText(sText,rtItem,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	cdcMemo.Draw3dRect(rtItem,TP_GetSysColor(COLOR_BASE_3DLEFT),TP_GetSysColor(COLOR_BASE_3DRIGHT));	
	cdcMemo.SelectObject(pSaveBmp);
	cdcMemo.SelectObject(pSaveFont);
	cdcMemo.DeleteDC();
	CImageList *pImageList = new CImageList;
	pImageList ->Create(bmInfo.bmWidth,bmInfo.bmHeight,ILC_COLORDDB|ILC_MASK,0,1);
	int iIndex = pImageList ->Add(&bmCreate,TP_GetSysColor(m_iBackColor));
	bmCreate.DeleteObject();
	ReleaseDC(pDC);
	return pImageList;
}
void CTPTreeCtrlEx::OnNcPaint()
{
	CRect rtWnd,rtClient;
	GetWindowRect(&rtWnd);
	GetClientRect(&rtClient);
	ClientToScreen(&rtClient);
	rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
	rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
	if(rtWnd.bottom - rtClient.bottom >2)
	{
		CDC   *pDC   = GetWindowDC();		
		CTPDCExchange stuTemp(pDC);
		pDC ->FillSolidRect(0,rtClient.bottom,rtWnd.Width(),rtWnd.bottom -rtClient.bottom,TP_GetSysColor(m_iBackColor));		
		stuTemp.ReleaseDC();
		ReleaseDC(pDC);
	}

#ifdef  TP_NS_EDIT
	CTPTreeCtrl::OnNcPaint();
#else
	g_pRoundCorner ->m_iBackColor    = m_iBackColor;
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderState,this);
#endif
}

void CTPTreeCtrlEx::OnSize(UINT nType, int cx, int cy)
{	
	CTPTreeCtrl::OnSize(nType, cx, cy);	
}



void CTPTreeCtrlEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CTPTreeCtrl::OnPaint() for painting messages

	if(!m_bReDraw)  return;	
	m_iBottomLine = 0; 
	CRect     rtClient;
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());			
	CFont   *pSaveFont  = cdcMemo.SelectObject(TP_GetSysFont(m_iFontIndex)); 	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));
	m_pfnSuper(m_hWnd,WM_PRINT,(WPARAM)cdcMemo.m_hDC,(LPARAM)(PRF_CLIENT));	
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	
}
BOOL CTPTreeCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CTPTreeCtrl::OnEraseBkgnd(pDC);
}
BOOL CTPTreeCtrlEx::ClearMultiChild(HTREEITEMList &listItem)
{
	if(m_uState &STATE_MULTISEL)
	{ 
		if(listItem.GetSize()<=1) return TRUE;
NextFind:
		for(INT l= 1;l<listItem.GetSize();l++)
		{
			if(listItem[l] == listItem[0]){listItem.RemoveAt(l);goto NextFind;}
			HTREEITEM hItem = GetParentItem(listItem[l]);
			while(hItem)
			{
				if(hItem == listItem[0]){listItem.RemoveAt(l);goto NextFind;}
				hItem = GetParentItem(hItem);
			}
		}
	}
	return TRUE;
}
BOOL CTPTreeCtrlEx::ClearMultiSel(HTREEITEM hItem)
{
	ASSERT(m_uState &STATE_MULTISEL);
	BOOL bClear = FALSE;

	m_hShiftItem =  NULL;

	if(hItem && GetIsSelect(hItem))
	{	
		bClear = TRUE;
		SetItemState(hItem,0,TVIS_MULTISEL);
	}
	hItem = GetChildItem(hItem);
	while (hItem != NULL)
	{
		if(GetIsSelect(hItem))
		{	
			bClear = TRUE;
			SetItemState(hItem,0,TVIS_MULTISEL);
		}
		bClear |= ClearMultiSel(hItem);
		hItem = GetNextItem(hItem, TVGN_NEXT);		
	}
	return bClear;
}
BOOL CTPTreeCtrlEx::GetIsSelect(HTREEITEM hItem)
{	
	if(m_uState &STATE_MULTISEL)
		return (GetItemState(hItem,TVIS_MULTISEL) &  TVIS_MULTISEL) == TVIS_MULTISEL;
	else 
		return FALSE;
}
BOOL CTPTreeCtrlEx::SelectItem(HTREEITEM hItem,BOOL bKeep)
{
	if(m_uState &STATE_MULTISEL)
	{
		if(bKeep == FALSE && ClearMultiSel(NULL)) InvalidateRect(NULL,FALSE);
		SetItemState(hItem,TVIS_MULTISEL,TVIS_MULTISEL);
	}
	return CTPTreeCtrl::SelectItem(hItem);
}
void CTPTreeCtrlEx::EnableMultiSel(BOOL bEnable)
{
	if(bEnable) m_uState |=  STATE_MULTISEL;
	else        m_uState &=~ STATE_MULTISEL;
}
BOOL CTPTreeCtrlEx::GetSelectedItemMulti(HTREEITEMList &listItem,BOOL bDown) 
{
	if(m_uState &STATE_MULTISEL)
	{	
		HTREEITEM hItem    = GetChildItem(NULL);
		HTREEITEM hSelItem = GetSelectedItem();
		while(hItem)
		{
			if(GetIsSelect(hItem))
			{
				if(hSelItem == hItem) hSelItem = NULL;
				if(bDown)	listItem.Add(hItem);
				else        listItem.InsertAt(0,hItem);
			}
			hItem = GetNextVisibleItem(hItem);
		}	
		if(hSelItem) listItem.Add(hSelItem);
	}
	else 
	{
		HTREEITEM hSelItem = GetSelectedItem();
		if(hSelItem) listItem.Add(hSelItem);
	}
	return listItem.GetSize()>0;
}

void CTPTreeCtrlEx::SelectShift(BOOL bUp)
{
	HTREEITEM      hItem    = GetChildItem(NULL);	
	HTREEITEM      hItemSel = GetSelectedItem();
	HTREEITEMList  hItemList;
	int            iShift   = -1; 
	int            iSelMin = 0x7fff,iSelMax = 0;
	if(m_hShiftItem == NULL) m_hShiftItem = GetSelectedItem(); 
	if(m_hShiftItem == NULL) return;
	while(hItem)
	{		
		hItemList.Add(hItem);
		if(GetIsSelect(hItem) || hItemSel == hItem)
		{
			iSelMin   = min(iSelMin,(int)hItemList.GetSize());
			iSelMax   = max(iSelMax,(int)hItemList.GetSize());
		}
		if(m_hShiftItem == hItem) iShift = (int)hItemList.GetSize()-1;
		hItem = GetNextVisibleItem(hItem);
	}		
	if(iShift == -1)  iShift = iSelMin;
	iSelMin = iSelMax = iShift;
	for(int i = iShift;i>=0;i--)
	{
		if(GetIsSelect(hItemList[i]) || hItemSel == hItemList[i]) iSelMin = i;
		else break;
	}
	for(int i = iShift;i<hItemList.GetSize();i++)
	{
		if(GetIsSelect(hItemList[i]) || hItemSel == hItemList[i]) iSelMax = i;
		else break;
	}
	if(iSelMin<iShift && iSelMax <iShift || iSelMin>iShift && iSelMax >iShift)
		iSelMin = iSelMax = iShift;
	hItemSel = m_hShiftItem;
	ClearMultiSel(NULL);
	m_hShiftItem = hItemSel;
	if(bUp)
	{		
		if(iSelMin<iShift)
		{
			iSelMin --;
			iSelMin = max(0,iSelMin);
			for(INT l=iSelMin;l<=iShift;l++)
				 SetItemState(hItemList[l],TVIS_MULTISEL,TVIS_MULTISEL);
			CTPTreeCtrl::SelectItem(hItemList[iSelMin]);
		}
		else if(iSelMax >iShift)
		{
			iSelMax --;
			iSelMax = max(0,iSelMax);
			for(INT l=iShift;l<=iSelMax;l++)
				SetItemState(hItemList[l],TVIS_MULTISEL,TVIS_MULTISEL);
			CTPTreeCtrl::SelectItem(hItemList[iSelMax]);
		}
		else
		{
			iSelMin --;
			iSelMin = max(0,iSelMin);
			for(INT l=iSelMin;l<=iShift;l++)
				SetItemState(hItemList[l],TVIS_MULTISEL,TVIS_MULTISEL);
			CTPTreeCtrl::SelectItem(hItemList[iSelMin]);
		}
	}
	else 
	{
		if(iSelMax >iShift)
		{
			iSelMax ++;
			iSelMax = min((int)hItemList.GetSize()-1,iSelMax);
			for(INT l=iShift;l<=iSelMax;l++)
				SetItemState(hItemList[l],TVIS_MULTISEL,TVIS_MULTISEL);
			CTPTreeCtrl::SelectItem(hItemList[iSelMax]);
		}
		else if(iSelMin<iShift)
		{
			iSelMin ++;
			iSelMin = min((int)hItemList.GetSize()-1,iSelMin);
			for(INT l=iSelMin;l<=iShift;l++)
				SetItemState(hItemList[l],TVIS_MULTISEL,TVIS_MULTISEL);
			CTPTreeCtrl::SelectItem(hItemList[iSelMin]);
		}		
		else
		{
			iSelMax ++;
			iSelMax = min((int)hItemList.GetSize()-1,iSelMax);
			for(INT l=iShift;l<=iSelMax;l++)
				SetItemState(hItemList[l],TVIS_MULTISEL,TVIS_MULTISEL);
			CTPTreeCtrl::SelectItem(hItemList[iSelMax]);
		}
	}
}
void CTPTreeCtrlEx::SelectGroup(HTREEITEM hItemCur)
{
	HTREEITEMList listItem;
	HTREEITEM     hItem    = GetChildItem(NULL);	
	HTREEITEM     hItemSel = GetSelectedItem();
	int           iCur     = -1,iSelMin = 0x7fff,iSelMax = 0,iShift = -1,iMin = 0,iMax=-1;
	while(hItem)
	{
		if(GetIsSelect(hItem) || hItemSel == hItem)
		{
			iSelMin   = min(iSelMin,(int)listItem.GetSize());
			iSelMax   = max(iSelMax,(int)listItem.GetSize());
		}
		if(hItemCur     == hItem)                   iCur   = (int)listItem.GetSize();
		if(m_hShiftItem == hItem)                   iShift = (int)listItem.GetSize();
		listItem.Add(hItem);
		hItem = GetNextVisibleItem(hItem);
	}		
	if(iShift == -1)  iShift = iSelMin;
	if(iCur>=iShift){iMin =iShift; iMax =iCur;}
	else            {iMin =iCur; iMax =iShift;}
	hItemSel = m_hShiftItem;
	ClearMultiSel(NULL);
	m_hShiftItem = hItemSel;
	for(INT i=iMin;i<=iMax;i++)
		SetItemState(listItem[i],TVIS_MULTISEL,TVIS_MULTISEL);
	listItem.RemoveAll();
}
void CTPTreeCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{		
	if(m_uState &STATE_MULTISEL)
	{	
		UINT  uFlags = 0;		
		HTREEITEM hSel     = HitTest(point,&uFlags);	
		HTREEITEM hCurSel  = CTPTreeCtrl::GetSelectedItem();
		if(!(TVHT_ONITEMBUTTON & uFlags))
		{
			if(GetKeyState(VK_CONTROL) <0 )
			{
				SetFocus();
				m_hShiftItem    = hSel;
				if(hSel)
				{
					if(hCurSel == hSel || GetIsSelect(hSel))
					{
						HTREEITEMList listItem;
						GetSelectedItemMulti(listItem);
						SetItemState(hSel,0,TVIS_MULTISEL);
						if(listItem.GetSize()>1)
						{
							INT l=0;
							for(l=0;l<listItem.GetSize();l++)
							{
								if(listItem[l] != hSel)
								{						
									CTPTreeCtrl::SelectItem(listItem[l]);								
									break;
								}
							}
							if(l>=listItem.GetSize())
								CTPTreeCtrl::SelectItem(NULL);								
							InvalidateRect(NULL,FALSE);
							return;
						}
						else 
						{
							SetItemState(hSel,TVIS_MULTISEL,TVIS_MULTISEL);
							CTPTreeCtrl::SelectItem(hSel);
							return;
						}
					}
					else SetItemState(hSel,TVIS_MULTISEL,TVIS_MULTISEL);
				}
			}
			else if(GetKeyState(VK_SHIFT) <0)
			{			
				SetFocus();
				if(hSel/* && (TVHT_ONITEM & uFlags)*/) SelectGroup(hSel);
				CTPTreeCtrl::SelectItem(hSel);
				return;
			}
			else
			{
				if(ClearMultiSel(NULL)) InvalidateRect(NULL,FALSE);
				m_hShiftItem = hSel;
			}
		}
		else if((TVHT_ONITEMBUTTON & uFlags))
		{
			 if(!TP_IsCtrlKeyDown() && !TP_IsShiftKeyDown())
			 {
				 if(GetIsSelect(hSel)){ CTPTreeCtrl::SelectItem(hSel);hCurSel = hSel;}
				 if(ClearMultiSel(NULL)) InvalidateRect(NULL,FALSE);
				 if(hCurSel)  SetItemState(hCurSel,TVIS_MULTISEL,TVIS_MULTISEL);
				 
			 }
		}
	}
	CTPTreeCtrl::OnLButtonDown(nFlags, point);	
	SetSelState();
}
void CTPTreeCtrlEx::OnRButtonDownEx(UINT nFlags, CPoint point)
{
#ifdef TP_NS_EDIT
	m_hRightSelect = HitTest(point);
	if(m_hRightSelect != NULL) Invalidate(NULL);
#endif	
	m_bLeftDowm = FALSE;
}
void CTPTreeCtrlEx::OnTvnSelchangedEx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if(m_uState &STATE_MULTISEL)
	{
		SetItemState(pNMTreeView ->itemNew.hItem,TVIS_MULTISEL,TVIS_MULTISEL);
	}
}
void CTPTreeCtrlEx::OnRButtonUpEx(UINT nFlags, CPoint point)
{
#ifdef TP_NS_EDIT	
	if(m_hRightSelect != NULL){m_hRightSelect = NULL; Invalidate(NULL);}	
#endif
}
void CTPTreeCtrlEx::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(m_uState &STATE_MULTISEL)
	{			
		HTREEITEM hSel  = HitTest(point);
		if(hSel && !GetIsSelect(hSel)){if(ClearMultiSel(NULL)) InvalidateRect(NULL,FALSE);}
	}		
	CTPTreeCtrl::OnRButtonUp(nFlags, point);
}
LRESULT CTPTreeCtrlEx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_uState &STATE_MULTISEL)
	{	
		if(message == WM_SETFOCUS || message == WM_KILLFOCUS)
			InvalidateRect(NULL,FALSE);			
	}
	return CTPTreeCtrl::DefWindowProc(message, wParam, lParam);
}

BOOL CTPTreeCtrlEx::PreTranslateMessage(MSG* pMsg)
{	
	if(m_uState &STATE_MULTISEL)
	{	
		if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT))
		{
			if((pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP) && TP_IsShiftKeyDown()) 
			{
				SelectShift(pMsg->wParam == VK_UP);
				SetSelState();
				return TRUE;
			}
			else if(ClearMultiSel(NULL)) InvalidateRect(NULL,FALSE);
		}
		else if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_HOME || pMsg->wParam == VK_END))
		{
			 if(ClearMultiSel(NULL)) InvalidateRect(NULL,FALSE);
		}
	}
	return CTPTreeCtrl::PreTranslateMessage(pMsg);
}
