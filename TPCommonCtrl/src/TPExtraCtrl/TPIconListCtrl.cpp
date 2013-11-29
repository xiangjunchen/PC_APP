// TPIconListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPIconListCtrl.h"
#include ".\tpiconlistctrl.h"


#define  TP_ICONLIST_WIDTH 75
#define  TP_ICONLIST_HIGH  60

// CTPIconListCtrl

IMPLEMENT_DYNAMIC(CTPIconListCtrl, CTPListCtrlEx)
CTPIconListCtrl::CTPIconListCtrl()
{
	m_uPrivateStyle      = 0;
	m_iViewType          = VIEWTYPE_ICON;

	m_bHorz              = TRUE;
	m_szIcon             = CSize(TP_ICONLIST_WIDTH,TP_ICONLIST_HIGH);
	m_szDistance         = CSize(3,3);
	m_bText              = FALSE;
}

CTPIconListCtrl::~CTPIconListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTPIconListCtrl, CTPListCtrlEx)
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void     CTPIconListCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_LISTCTRLEX,&wndClass);		
		wndClass.lpszClassName = WC_ICONLISTCTRLEX;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_ICONLISTCTRLEX,AfxGetInstanceHandle());
}

void CTPIconListCtrl::OnDestroy()
{
	CTPListCtrlEx::OnDestroy();	
}
void CTPIconListCtrl::SubWindow(BOOL bSub)
{
	CTPListCtrlEx::SubWindow(bSub);
	if(bSub)
	{
		InsertColumn(0,_L(""),LVCFMT_CENTER,100);
		CRect rtClient;
		GetClientRect(&rtClient);
		m_szIcon.cy = rtClient.Height() - m_szDistance.cy *2;
		m_szIcon.cx = m_szIcon.cy * TP_ICONLIST_WIDTH/TP_ICONLIST_HIGH;
	}
	else
	{
		DeleteAllItems();
		DeleteColumn(0);
	}
}
void CTPIconListCtrl::DrawIcon(CDC *pDC,int iItem)
{
	TPListCtrlItem    *pItem     = m_aItem[iItem];
	TPListCtrlSubItem *pSubItem  = pItem ->aSubItem[0];
	CRect rtIcon,rtText;
	rtText = pSubItem ->rtItem;
	rtIcon = pSubItem ->rtItem;

	TPIconListItem   *pIconItem = (TPIconListItem   *)pItem ->dwData;
	if(pIconItem && pIconItem ->pIcon) TP_StretchDIBits(pDC,pIconItem ->pIcon,pIconItem ->szIcon.cx,pIconItem ->szIcon.cy,rtIcon);
	else 	pDC ->FillSolidRect(rtIcon,RGB(0,0,0));

}
void CTPIconListCtrl::FunCalcScrollBar()
{
}
void CTPIconListCtrl::FunCalcItemSize()
{
	CSize szIcon = m_szIcon;	
	GetClientRect(&m_rtClient);
	int   iItemSize    = (int)m_aItem.GetSize(),iCy = m_szDistance.cy,iCx = m_szDistance.cx;
	CRect rtItem = CRect(0,0,0,0);
	m_iTotalHeight = 0;
	m_iTotalWidth  = 0;
	for(int i=0;i<iItemSize;i++)
	{
		TPListCtrlItem *pItem =m_aItem[i]; 
		if(m_bHorz)
		{
			pItem ->iIndex = i;
			rtItem = CRect(iCx- m_iTopX,iCy - m_iTopY,iCx + szIcon.cx- m_iTopX,iCy + szIcon.cy - m_iTopY);
			iCy += szIcon.cy + m_szDistance.cy;
			if(iCy + szIcon.cy > m_rtClient.bottom)
			{
				iCy = m_szDistance.cy;
				iCx += szIcon.cx + m_szDistance.cx;				
			}
			m_iTotalWidth = iCx;
			pItem ->aSubItem[0] ->rtItem= rtItem;
		}
		else
		{		
			pItem ->iIndex = i;
			rtItem = CRect(iCx - m_iTopX,iCy - m_iTopY,iCx + szIcon.cx- m_iTopX,iCy + szIcon.cy - m_iTopY);
			iCx += szIcon.cx + m_szSpace.cx;
			if(iCx + szIcon.cx > m_rtClient.right)
			{
				iCx = m_szEdge.cx;
				iCy += szIcon.cy + m_szSpace.cy;				
			}
			m_iTotalHeight = iCy;
			pItem ->aSubItem[0] ->rtItem= rtItem;
		}
	}
}
int   CTPIconListCtrl::InsertItem(int nItem, TPIconListItem &stuItem)
{
	TPIconListItem *pItem = new TPIconListItem();
	*pItem = stuItem;
	int iIndex = CTPListCtrlEx::InsertItem(nItem,_L(""));
	CTPListCtrlEx::SetItemData(nItem,(DWORD_PTR)pItem);
	return iIndex;
}
BOOL  CTPIconListCtrl::DeleteItem(int nItem)
{
	TPIconListItem *pItem = (TPIconListItem *)CTPListCtrlEx::GetItemData(nItem);
	if(pItem) delete pItem;
	return CTPListCtrlEx::DeleteItem(nItem);
}
BOOL  CTPIconListCtrl::DeleteAllItems()
{
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		TPIconListItem *pItem = (TPIconListItem *)CTPListCtrlEx::GetItemData(l);
		if(pItem) delete pItem;
	}
	return CTPListCtrlEx::DeleteAllItems();
}

BOOL CTPIconListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL bReturn =  CTPListCtrlEx::OnMouseWheel(nFlags, zDelta, pt);
	long lX = m_iTopX,lY = m_iTopY;
	if(zDelta >0)
	{		
		if(m_bHorz)
		{
			m_iTopX -= 5; 
			m_iTopX = max(0,m_iTopX);
			if(lX != m_iTopX)
			{
				FunCalcItemSize();
				Invalidate();
			}
		}
		else 
		{
			m_iTopY -= 5;
			m_iTopY = max(0,m_iTopY);
			if(lY != m_iTopY)
			{
				FunCalcItemSize();
				Invalidate();
			}
		} 
	}
	else
	{
		if(m_bHorz)
		{
			m_iTopX += 5; 
			m_iTopX = min(m_iTotalWidth - m_rtClient.Width(),m_iTopX);
			m_iTopX = max(0,m_iTopX);
			if(lX != m_iTopX)
			{
				FunCalcItemSize();
				Invalidate();
			}
		}
		else 
		{
			m_iTopY += 5;
			m_iTopY = min(m_iTotalHeight - m_rtClient.Height(),m_iTopY);
			m_iTopY = max(0,m_iTopY);
			if(lY != m_iTopY)
			{
				FunCalcItemSize();
				Invalidate();
			}
		} 
	}
	return bReturn;
}
