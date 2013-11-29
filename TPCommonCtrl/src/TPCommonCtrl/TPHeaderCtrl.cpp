// TPHeadCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPHeaderCtrl.h"
#include ".\tpheaderctrl.h"


// CTPHeaderCtrl

IMPLEMENT_DYNAMIC(CTPHeaderCtrl, CHeaderCtrl)
CTPHeaderCtrl::CTPHeaderCtrl()
{	
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bSubWindow = FALSE;
	m_bCreate    = FALSE;
#ifdef TP_MINI_EDIT

	m_pHeaderBmp[0]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\MiniListCtrl\\HeaderCtrl_Dis.bmp")); 
	m_pHeaderBmp[1]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\MiniListCtrl\\HeaderCtrl_Over.bmp"));
	m_pHeaderBmp[2]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\MiniListCtrl\\HeaderCtrl_Nor.bmp"));
	m_pHeaderBmp[3]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\MiniListCtrl\\HeaderCtrl_Down.bmp"));
	m_pHeaderBmp[4]  = TP_LoadImage(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\MiniListCtrl\\HeaderCtrl_Sort.bmp"),12,RGB(0,0,0));
#else
	m_pHeaderBmp[0]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\List17\\HeaderCtrl_Dis.bmp")); 
	m_pHeaderBmp[1]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\List17\\HeaderCtrl_Over.bmp"));
	m_pHeaderBmp[2]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\List17\\HeaderCtrl_Nor.bmp"));
	m_pHeaderBmp[3]  = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\List17\\HeaderCtrl_Down.bmp"));
	m_pHeaderBmp[4]  = TP_LoadImage(g_sSysSkinPath + _L("\\CommonCtrl\\ListCtrl\\HeaderCtrl_Sort.bmp"),12,RGB(0,0,0));
#endif	



	m_iOver   = -1;
	m_iDown   = -1;
	m_hTimer  = NULL;

}

CTPHeaderCtrl::~CTPHeaderCtrl()
{
	for(INT l=0;l<5;l++)
	TP_ReleaseCommonRes(m_pHeaderBmp[l]); 	

}

BEGIN_MESSAGE_MAP(CTPHeaderCtrl, CHeaderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CTPHeaderCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		ModifyStyle(HDS_HOTTRACK,0,0);
		SaveFont(m_hWnd);
		SetFont(TP_GetSysFont(m_iFontIndex));

	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;
		RestoreFont(m_hWnd);
	}
}


void CTPHeaderCtrl::PreSubclassWindow()
{
	CHeaderCtrl::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

BOOL CTPHeaderCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CHeaderCtrl::PreCreateWindow(cs);
}

void CTPHeaderCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW  ;
	}
	else if(pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{		
		int     iIndex = 0;
		CDC *pDC = CDC::FromHandle(pNMCD ->hdc);		
		pDC ->SetBkMode(TRANSPARENT);
		pDC ->SetStretchBltMode(COLORONCOLOR);
		if(m_iDown == pNMCD ->dwItemSpec)    
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
			iIndex = 3;
		}
		else if(m_iOver == pNMCD ->dwItemSpec)
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));
			iIndex = 1;
		}
		else if(!IsWindowEnabled())        
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));
			iIndex = 0;
		}
		else
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
			iIndex = 2;
		}
		
		//画底图
		TP_StretchDIBRect(pDC,m_pHeaderBmp[iIndex] ->GetByte(),m_pHeaderBmp[iIndex]->Width(),m_pHeaderBmp[iIndex]->Height(),5,5,pNMCD ->rc,FALSE);

		HDITEM  iItemData;
		TCHAR   cText[100];
		iItemData.mask =  HDI_BITMAP  | HDI_FORMAT | HDI_IMAGE | HDI_TEXT;
		iItemData.cchTextMax = 100;
		iItemData.pszText    = cText;
		CHeaderCtrl::GetItem((int)pNMCD ->dwItemSpec,&iItemData);

		int iLeft = pNMCD ->rc.left + 5 ,iRight = pNMCD ->rc.right - 5;
		if(iItemData.fmt & HDF_IMAGE)
		{			
			CImageList *pImageList = CHeaderCtrl::GetImageList();
			if(pImageList)
			{
				IMAGEINFO imageInfo;
				if(pImageList ->GetImageInfo(iItemData.iImage,&imageInfo))
				{
					//画图
					if(iLeft  +(imageInfo.rcImage.right - imageInfo.rcImage.left) <iRight)
					{					
						int iImageTop  = max(pNMCD ->rc.top,pNMCD ->rc.top + ((pNMCD ->rc.bottom - pNMCD ->rc.top) - (imageInfo.rcImage.bottom - imageInfo.rcImage.top))/2);
						int iImageLeft = iLeft;
						pImageList ->Draw(pDC,iItemData.iImage,CPoint(iImageLeft,iImageTop),ILD_TRANSPARENT);
					}
					iLeft  += imageInfo.rcImage.right - imageInfo.rcImage.left + 2;

				}
				else iLeft += pNMCD ->rc.bottom - pNMCD ->rc.top;
			}
			else iLeft += pNMCD ->rc.bottom - pNMCD ->rc.top;
		}
		if(iItemData.fmt & (HDF_SORTDOWN | HDF_SORTUP))
		{
			CRect rtTemp = pNMCD ->rc;
			rtTemp.right = iRight;
			rtTemp.left = rtTemp.right - m_pHeaderBmp[4] ->Width();
			if(rtTemp.left > iLeft)
			{
				ImageList_Draw(m_pHeaderBmp[4] ->GetImage(),(iItemData.fmt & HDF_SORTDOWN)? 0:1,pDC ->m_hDC,rtTemp.left ,rtTemp.top +(rtTemp.Height() -  m_pHeaderBmp[4]  ->Height())/2,ILD_TRANSPARENT	);
			}
			iRight -= m_pHeaderBmp[4] ->Width() + 2;			
		}
		if(iItemData.fmt & HDF_STRING)
		{
			CRect rtTemp = pNMCD ->rc;
			rtTemp.left  = iLeft;
			rtTemp.right = iRight;
			if(rtTemp.left <rtTemp.right)
			{ 
				CFont *pFont = (CFont *)pDC->SelectObject(TP_GetSysFont(m_iFontIndex));
				UINT uFormat = DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS;			
				if(iItemData.fmt & HDF_RIGHT)       uFormat |= DT_RIGHT;
				else if(iItemData.fmt & HDF_CENTER) uFormat |= DT_CENTER;
				else                                uFormat |= DT_LEFT;
				pDC ->DrawText(iItemData.pszText,rtTemp,uFormat);	
				pDC ->SelectObject(pFont);
			}
		}
		else if(iItemData.fmt & HDF_BITMAP)
		{
		}
		*pResult =  CDRF_SKIPDEFAULT;		
	}	
	else 	*pResult =  CDRF_DODEFAULT;	

}
void CTPHeaderCtrl::SetSortFlag(int iColumn,UINT uFlag)
{
	int     iCount = GetItemCount();
	if(iColumn<0 || iColumn >= iCount) return;
	HDITEM  iItemData;	
	iItemData.mask =  HDI_FORMAT;	
	for(int l=0;l<iCount;l++)
	{		
		GetItem(l,&iItemData);
		iItemData.fmt &= ~HDF_SORTDOWN;
		iItemData.fmt &= ~HDF_SORTUP;
		if(iColumn == l) iItemData.fmt |= uFlag;
		SetItem(l,&iItemData);
	}	
}
void CTPHeaderCtrl::SetColumnFmt(int iColumn,UINT uFlagAdd,UINT uFlagRemove)
{
	int     iCount = GetItemCount();
	if(iColumn<0 || iColumn >= iCount) return;
	HDITEM  iItemData;	
	iItemData.mask =  HDI_FORMAT;	
	for(int l=0;l<iCount;l++)
	{		
		GetItem(l,&iItemData);
		iItemData.fmt &= ~uFlagRemove;		
		if(iColumn == l) iItemData.fmt |= uFlagAdd;
		SetItem(l,&iItemData);
	}	
}
int CTPHeaderCtrl::HitTest(CPoint point)
{
	int   iCount = CHeaderCtrl::GetItemCount();
	CRect rtItem;
	for(int l=0;l<iCount;l++)
	{
		CHeaderCtrl::GetItemRect(l,&rtItem);
		if(rtItem.PtInRect(point)) return l;
	}
	return -1;
}
void CTPHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{	
	m_iDown = HitTest(point);
	Invalidate();
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CTPHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_iDown = -1;
	Invalidate();
	CHeaderCtrl::OnLButtonUp(nFlags, point);
}

void CTPHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!(nFlags & MK_LBUTTON))
	{	
		int iOver = HitTest(point);
		if(m_iOver != iOver)
		{
			m_iOver = iOver;
			Invalidate();
			if(m_hTimer == NULL)
				m_hTimer = SetTimer(100,100,NULL);
		}	
	}
	CHeaderCtrl::OnMouseMove(nFlags, point);
}

void CTPHeaderCtrl::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 100)
	{
		CPoint ptCur;
		GetCursorPos(&ptCur);
		ScreenToClient(&ptCur);
		int iOver = HitTest(ptCur);		
		if(iOver <0)
		{
			KillTimer(m_hTimer);
			m_hTimer = NULL;
			m_iOver = -1;
			if(GetKeyState(VK_LBUTTON) >=0)	Invalidate();
		}
		else
		{
			if(m_iOver != iOver && GetKeyState(VK_LBUTTON) >=0)
			{
				m_iOver = iOver;
				Invalidate();
			}
		}

	}

	CHeaderCtrl::OnTimer(nIDEvent);
}

void CTPHeaderCtrl::OnDestroy()
{
	if(m_hTimer) KillTimer(m_hTimer);
	CHeaderCtrl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

BOOL CTPHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
//	return CHeaderCtrl::OnEraseBkgnd(pDC);
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;

}

void CTPHeaderCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect     rtClient;
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
	m_pfnSuper(m_hWnd,WM_PRINT,(WPARAM)cdcMemo.m_hDC,(LPARAM)(PRF_CLIENT|PRF_CHECKVISIBLE|PRF_ERASEBKGND));	 
	//画多于的地方
	int   iCount = CHeaderCtrl::GetItemCount();
	CRect rtRemain = rtClient;
	if(iCount >0) 
	{
		CRect rtItem;
		CHeaderCtrl::GetItemRect(OrderToIndex(iCount -1),&rtItem);
		rtRemain.left = rtItem.right;
	}
	if(rtRemain.Width()>0)
	{
		int iIndex = 2;		
		TP_StretchDIBRect(&cdcMemo,m_pHeaderBmp[iIndex] ->GetByte(),m_pHeaderBmp[iIndex]->Width(),m_pHeaderBmp[iIndex]->Height(),5,5,rtRemain,FALSE);
	}
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
	

}

HBRUSH CTPHeaderCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CHeaderCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  在此更改 DC 的任何属性

	//// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return hbr;

	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{	
		return TP_GetSysBrush(BRUSH_FRAME);
	}
	else return hbr;

	return hbr;
}
HWND CTPHeaderCtrl::UnsubclassWindow()
{
	SubWindow(FALSE);
	return CHeaderCtrl::UnsubclassWindow();
}