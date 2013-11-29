// TPComboDTBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPComboDTBox.h"
#include ".\tpcombodtbox.h"

IMPLEMENT_DYNAMIC(CTPComboDTBox, CTPDirTreeCtrl)
CTPComboDTBox::CTPComboDTBox()
{ 
	m_bTreeCtrl    = FALSE;
	m_bFous        = TRUE;
 	m_pTreeCtrl    = this;
}

CTPComboDTBox::~CTPComboDTBox()
{
}


BEGIN_MESSAGE_MAP(CTPComboDTBox, CTPDirTreeCtrl)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
void CTPComboDTBox::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if(m_bTreeCtrl) 
	{
		CTPDirTreeCtrl::OnNcLButtonDown(nHitTest,point);
		return;
	}
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
	else CTPDirTreeCtrl::OnNcLButtonDown(nHitTest, point);
}

void CTPComboDTBox::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	if(m_bTreeCtrl) 
	{
		CTPDirTreeCtrl::OnNcLButtonDown(nHitTest, point);
		return ;
	}
	CPoint ptTest = point;
	ClientToScreen(&ptTest);
	if(m_pMouseWnd)
	{	
		m_pMouseWnd ->ScreenToClient(&ptTest);
		m_pMouseWnd ->SendMessage(WM_NCLBUTTONUP,nHitTest,MAKELONG(ptTest.x,ptTest.y));				
	}
	else CTPDirTreeCtrl::OnNcLButtonUp(nHitTest, point);
}

void CTPComboDTBox::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(m_bTreeCtrl) 
	{
		CTPDirTreeCtrl::OnMouseMove(nFlags, point);
		return;
	}
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
		CTPDirTreeCtrl::OnMouseMove(nFlags, point);
	}
}

void CTPComboDTBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bSetCapture = FALSE;
	if(m_bTreeCtrl) 
	{
		CTPDirTreeCtrl::OnLButtonDown(nFlags, point);
		return;
	}
	CRect rtClient;
	GetClientRect(&rtClient);
	if(rtClient.PtInRect(point))
	{
		CTPDirTreeCtrl::OnLButtonDown(nFlags, point);
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

void CTPComboDTBox::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if(m_bTreeCtrl) 
	{
		CTPDirTreeCtrl::OnLButtonUp(nFlags, point);		
		return;
	}
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
			CTPDirTreeCtrl::OnLButtonUp(nFlags, point);		
		}
	}
}

void CTPComboDTBox::OnCaptureChanged(CWnd *pWnd)
{
	CTPDirTreeCtrl::OnCaptureChanged(pWnd);
	if(pWnd == NULL)
	{
		if(m_bDropDown && m_bSetCapture) 
		{
			m_pOwnerWnd ->SendMessage(WM_LBUTTONDOWN,0,MAKELONG(0,0));				
		}
	}
}

void CTPComboDTBox::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
 
	if(!(m_uState & STATE_SETDATA))
	{
		if(m_pOwnerWnd && !m_bTreeCtrl)
		{	
			if (pNMTreeView->action == TVC_UNKNOWN && (m_uState & STATE_DELITEM))
			{
				m_pOwnerWnd ->SendMessage(WM_DIRTREECTRL,3,MAKELONG(0,0));
			}
			else
			{
				UINT   uFlag = TVHT_ONITEMBUTTON;
				CPoint ptCursor;
				GetCursorPos(&ptCursor);
				ScreenToClient(&ptCursor);
				if(HitTest(ptCursor,&uFlag) == NULL)
				{
					m_pOwnerWnd ->SendMessage(WM_LBUTTONDOWN,0,MAKELONG(0,0));		
				}
			}	
		}
		if(m_bTreeCtrl) GetParent() ->SendMessage(WM_DIRTREECTRL,1,(LPARAM)m_hWnd);				
		m_uBaseState |= TP_BASESTATE_SELCHANGE;
	}
	*pResult = 0;
}

void CTPComboDTBox::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码	
	*pResult = 0;
	if(m_bTreeCtrl) return;
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

void CTPComboDTBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CTPDirTreeCtrl::OnLButtonDblClk(nFlags, point);	
	UINT uFlag = TVHT_ONITEM;
	if(m_bTreeCtrl && HitTest(point,&uFlag)!=NULL)//25582
		 GetParent() ->SendMessage(WM_DIRTREECTRL,2,(LPARAM)m_hWnd);				
}
void CTPComboDTBox::SetTreeCtrl(BOOL bSet)
{
	m_bTreeCtrl = bSet;
	if(m_pScrollData)
	{
		((CTPScrollBar *)m_pScrollData ->pHScroll)->m_bSetCapture = bSet;
		((CTPScrollBar *)m_pScrollData ->pVScroll)->m_bSetCapture = bSet;
	}
}

void CTPComboDTBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if(bShow) m_uBaseState &= ~TP_BASESTATE_SELCHANGE;
	CTPDirTreeCtrl::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}

BOOL CTPComboDTBox::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg ->message == WM_NCRBUTTONUP)
	{		
		return TRUE;
	}
	return CTPDirTreeCtrl::PreTranslateMessage(pMsg);
}
