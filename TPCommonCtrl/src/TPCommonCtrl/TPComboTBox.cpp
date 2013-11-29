// TPComboTBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPComboTBox.h"
#include ".\tpcombotbox.h"


CTPComboTBase::CTPComboTBase()
{
	m_bButtonPress = FALSE;
	m_pOwnerWnd    = NULL;
	m_bDropDown    = FALSE;
	m_pMouseWnd    = NULL;
	m_pScrollData  = NULL;
	m_bSetCapture  = FALSE;
	m_pTreeCtrl    = NULL;
}

// CTPComboTBox

IMPLEMENT_DYNAMIC(CTPComboTBox, CTPTreeCtrl)
CTPComboTBox::CTPComboTBox()
{	
	m_uState      |= TREE_SUBCOMBOBOX; 
	m_bFous        = TRUE;
	m_bTestSelect  = TRUE;
	m_pTreeCtrl    = this;
}

CTPComboTBox::~CTPComboTBox()
{
}


BEGIN_MESSAGE_MAP(CTPComboTBox, CTPTreeCtrl)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnTvnItemexpanded)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
void CTPComboTBox::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	CPoint ptTest = point;
	if(m_pScrollData)
	{
		CRect rtWnd;
		if(m_pScrollData ->bShowHScrollBar)
		{
			m_pScrollData ->pHScroll ->GetWindowRect(&rtWnd);
			if(rtWnd.PtInRect(ptTest))
			{
				m_pScrollData ->pHScroll ->ScreenToClient(&ptTest);
				m_pScrollData ->pHScroll ->SendMessage(WM_LBUTTONDOWN,nHitTest,MAKELONG(ptTest.x,ptTest.y));
				m_pMouseWnd = m_pScrollData ->pHScroll;		
				return;
			}
		}
		if(m_pScrollData ->bShowVScrollBar)
		{
			m_pScrollData ->pVScroll ->GetWindowRect(&rtWnd);
			if(rtWnd.PtInRect(ptTest))
			{
				m_pScrollData ->pVScroll ->ScreenToClient(&ptTest);
				m_pScrollData ->pVScroll ->SendMessage(WM_LBUTTONDOWN,nHitTest,MAKELONG(ptTest.x,ptTest.y));
				m_pMouseWnd = m_pScrollData ->pVScroll;		
				return;
			}
		}
	}
	else CTPTreeCtrl::OnNcLButtonDown(nHitTest, point);
}

void CTPComboTBox::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	CPoint ptTest = point;
	ClientToScreen(&ptTest);
	if(m_pMouseWnd)
	{	
		m_pMouseWnd ->ScreenToClient(&ptTest);
		m_pMouseWnd ->SendMessage(WM_NCLBUTTONUP,nHitTest,MAKELONG(ptTest.x,ptTest.y));				
	}
	else CTPTreeCtrl::OnNcLButtonUp(nHitTest, point);
}

void CTPComboTBox::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(m_bButtonPress)
	{
		CPoint ptTest = point;
		ClientToScreen(&ptTest);
		if(m_pOwnerWnd)
		{	
			m_pOwnerWnd ->ScreenToClient(&ptTest);
			m_pOwnerWnd ->SendMessage(WM_MOUSEMOVE,nFlags,MAKELONG(ptTest.x,ptTest.y));				
		}
	}
	else
	{
		CPoint ptTest = point;
		ClientToScreen(&ptTest);
		if(m_pMouseWnd)
		{	
			m_pMouseWnd ->ScreenToClient(&ptTest);
			m_pMouseWnd ->SendMessage(WM_MOUSEMOVE,nFlags,MAKELONG(ptTest.x,ptTest.y));				
			return;
		}
		else if(m_pScrollData)
		{
			CRect rtWnd;
			if(m_pScrollData ->bShowHScrollBar)
			{
				m_pScrollData ->pHScroll ->GetWindowRect(&rtWnd);
				if(rtWnd.PtInRect(ptTest))
				{
					m_pScrollData ->pHScroll ->ScreenToClient(&ptTest);
					m_pScrollData ->pHScroll ->SendMessage(WM_MOUSEMOVE,nFlags,MAKELONG(ptTest.x,ptTest.y));
					return;
				}
			}
			if(m_pScrollData ->bShowVScrollBar)
			{
				m_pScrollData ->pVScroll ->GetWindowRect(&rtWnd);
				if(rtWnd.PtInRect(ptTest))
				{
					m_pScrollData ->pVScroll ->ScreenToClient(&ptTest);
					m_pScrollData ->pVScroll ->SendMessage(WM_MOUSEMOVE,nFlags,MAKELONG(ptTest.x,ptTest.y));
					return;
				}
			}
		}
		CTPTreeCtrl::OnMouseMove(nFlags, point);
	}
}

void CTPComboTBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	m_bSetCapture = FALSE;
	if(rtClient.PtInRect(point))
	{
		m_SelItem = GetSelectedItem();
		CTPTreeCtrl::OnLButtonDown(nFlags, point);
	}
	else
	{
		CPoint ptTest = point;
		ClientToScreen(&ptTest);
		if(m_pScrollData)
		{
			CRect rtWnd;
			if(m_pScrollData ->bShowHScrollBar)
			{
				m_pScrollData ->pHScroll ->GetWindowRect(&rtWnd);
				if(rtWnd.PtInRect(ptTest))
				{
					m_pScrollData ->pHScroll ->ScreenToClient(&ptTest);
					m_pScrollData ->pHScroll ->SendMessage(WM_LBUTTONDOWN,nFlags,MAKELONG(ptTest.x,ptTest.y));
					m_pMouseWnd = m_pScrollData ->pHScroll;		
					return;
				}
			}
			if(m_pScrollData ->bShowVScrollBar)
			{
				m_pScrollData ->pVScroll ->GetWindowRect(&rtWnd);
				if(rtWnd.PtInRect(ptTest))
				{
					m_pScrollData ->pVScroll ->ScreenToClient(&ptTest);
					m_pScrollData ->pVScroll ->SendMessage(WM_LBUTTONDOWN,nFlags,MAKELONG(ptTest.x,ptTest.y));
					m_pMouseWnd = m_pScrollData ->pVScroll;		
					return;
				}
			}
		}
		if(m_pOwnerWnd)
		{	
			m_pOwnerWnd ->ScreenToClient(&ptTest);
			m_pOwnerWnd ->SendMessage(WM_LBUTTONDOWN,nFlags,MAKELONG(ptTest.x,ptTest.y));				
		}
	}
}

void CTPComboTBox::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if(m_bButtonPress)
	{
		CPoint ptTest = point;
		ClientToScreen(&ptTest);
		if(m_pOwnerWnd)
		{	
			m_pOwnerWnd ->ScreenToClient(&ptTest);
			m_pOwnerWnd ->SendMessage(WM_LBUTTONUP,nFlags,MAKELONG(ptTest.x,ptTest.y));				
		}
	}
	else
	{
		CPoint ptTest = point;
		ClientToScreen(&ptTest);
		if(m_pMouseWnd)
		{
			m_pMouseWnd ->ScreenToClient(&ptTest);
			m_pMouseWnd ->SendMessage(WM_LBUTTONUP,nFlags,MAKELONG(ptTest.x,ptTest.y));				
			m_pMouseWnd = FALSE;	
		}
		else
		{
			CTPTreeCtrl::OnLButtonUp(nFlags, point);
		}
	}
}

static BOOL bSendMessage = FALSE;
void CTPComboTBox::OnCaptureChanged(CWnd *pWnd)
{
	CTPTreeCtrl::OnCaptureChanged(pWnd);
	/*if(pWnd == this) 
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		UINT pFlags;
		HTREEITEM CurItem = HitTest(point,&pFlags);
		if(CurItem == m_SelItem)
			bSendMessage = TRUE;
		return;
	}
	else */if(pWnd == NULL)
	{
		if(m_bDropDown && m_bSetCapture) 
		{
			m_pOwnerWnd ->SendMessage(WM_LBUTTONDOWN,0,MAKELONG(0,0));	
			bSendMessage = FALSE;
		}
	}
	
}

void CTPComboTBox::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;
	if(m_pOwnerWnd && m_bTestSelect)
	{			
		if(((CTPComboTreeBox *)m_pOwnerWnd) ->TestSelectItem(pNMTreeView ->itemNew.hItem,pNMTreeView ->itemOld.hItem))
		{
			*pResult = 0;
			ReleaseCapture(); //zhaofei XN00019212
			m_pOwnerWnd ->SendMessage(WM_LBUTTONDOWN,0,MAKELONG(0,0));				
		}
		else 
		{
			m_bTestSelect = FALSE;
			SelectItem(pNMTreeView ->itemOld.hItem);
			SetCapture(); //zhaofei 
			m_bTestSelect = TRUE;
			*pResult = 0;		
		}
	}
}

// li song ming 2009-11-23 XN00025539
void CTPComboTBox::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码	
	*pResult = 0;
	CPoint ptTest;
	GetCursorPos(&ptTest);
	ScreenToClient(&ptTest);
	HTREEITEM hTreeItem  = CTPTreeCtrl::HitTest(ptTest,NULL);
	if(hTreeItem && m_pOwnerWnd)
	{
		CRect rtText,rtImage;
		IMAGEINFO   imgInfo;
		ZeroMemory(&imgInfo,sizeof(IMAGEINFO));
		CTPTreeCtrl::GetItemRect(hTreeItem,rtText,TRUE);
		rtImage = rtText;
		CImageList *pImage  = CTreeCtrl::GetImageList(TVSIL_NORMAL);
		if(pImage)
		{		
			rtImage.right = rtText.left;
			if(FALSE != pImage ->GetImageInfo(0,&imgInfo))
				rtImage.left = rtImage.right - (imgInfo.rcImage.right - imgInfo.rcImage.left);
			else rtImage.left = rtImage.right;
		}
		CRect rtShow = CRect(rtImage.left,rtImage.top,rtText.right,rtImage.bottom);
		if(rtShow.PtInRect(ptTest))
			m_pOwnerWnd ->PostMessage(WM_LBUTTONDOWN,0,MAKELONG(0,0));				
	}
}

void CTPComboTBox::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if(m_pOwnerWnd && m_bTestSelect)
	{			
		((CTPComboTreeBox *)m_pOwnerWnd) ->TestTvnItemexpanded(pNMHDR,pResult);
	}		
	*pResult = 0;
}

void CTPComboTBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTPTreeCtrl::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}

BOOL CTPComboTBox::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg ->message == WM_CAPTURECHANGED)
	{
	}
	return CTPTreeCtrl::PreTranslateMessage(pMsg);
}
