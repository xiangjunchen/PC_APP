// TPComboTreeBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPComboTreeBox.h"
#include ".\tpcombotreebox.h"


#define TREECOMBOX_BUTTONDOWN 0x00000001
#define TREECOMBOX_BROPDOWN   0x00000002
// CTPComboTreeBox
IMPLEMENT_DYNAMIC(CTPComboTreeBox, CTPWnd)
CTPComboTreeBox::CTPComboTreeBox()
{
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_pTreeCtrlEx   = NULL;
	m_pDTreeCtrlEx  = NULL;
	m_pBaseTreeCtrl = NULL;
	m_pEdit         = NULL;
	m_pDropDown     = NULL;
	m_eBorderStyle  = g_eBorderStyle;
	m_uState        = 0;
	m_uStyle        = CBS_DROPDOWNLIST;  
	m_iTreeHeight   = 300;
	m_hSelItem      = NULL;
#ifdef TP_MINI_EDIT
	m_pButton[0]    = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ComboBox\\MiniComboBox\\button_nor.bmp")); 	
	m_pButton[1]    = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ComboBox\\MiniComboBox\\button_down.bmp")); 	
#else
	m_pButton[0]    = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ComboBox\\button_nor.bmp")); 	
	m_pButton[1]    = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ComboBox\\button_down.bmp")); 	
#endif

	m_bDirTree      = 0;
	m_bSetCapture   = FALSE;

#ifdef TP_NS_NET
	m_uBaseState	|=TP_BASESTATE_TOPMOST;
#endif
}

CTPComboTreeBox::~CTPComboTreeBox()
{
	SubWindow(FALSE);
	TP_ReleaseCommonRes(m_pButton[0]);
	TP_ReleaseCommonRes(m_pButton[1]);
}
void     CTPComboTreeBox::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_WND,&wndClass);		
		wndClass.lpszClassName = WC_COMBOTREEBOX;
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_COMBOTREEBOX,AfxGetInstanceHandle());
}

BEGIN_MESSAGE_MAP(CTPComboTreeBox, CTPWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOVE()
    ON_MESSAGE(WM_DIRTREECTRL,OnMsgDirTree)
    ON_MESSAGE(CB_SHOWDROPDOWN,OnMsgShowDropDown)
	ON_NOTIFY(TVN_SELCHANGED, 101, OnTvnSelchangedTree)
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff,       OnToolTipNotify )
END_MESSAGE_MAP()
CTPTreeCtrl *CTPComboTreeBox::GetTreeCtrl()
{
	return m_pBaseTreeCtrl != NULL ? m_pBaseTreeCtrl ->m_pTreeCtrl :NULL;
}
BOOL CTPComboTreeBox::TestSelectItem(HTREEITEM hTreeItemNew,HTREEITEM hTreeItemOld)
{
	return TRUE;
}
int  GetTreeHeight(CTPTreeCtrl *pTree,HTREEITEM hTreeItem)
{
	int   iHeight = 0;
	CRect rtFirst = CRect(0,0,0,0);
	HTREEITEM hFirst = pTree ->GetChildItem(hTreeItem);
	while (hFirst)
	{
		iHeight += GetTreeHeight(pTree,hFirst); 
		pTree ->GetItemRect(hFirst,rtFirst,FALSE);
		rtFirst.NormalizeRect();
		iHeight += rtFirst.Height();
		hFirst = pTree->GetNextItem(hFirst,TVGN_NEXT );
	}
	return iHeight;

}
void CTPComboTreeBox::TestTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_uState & TREECOMBOX_BROPDOWN)
	{
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		int iHeight =  GetTreeHeight(m_pBaseTreeCtrl ->m_pTreeCtrl,NULL) + 20;
 
		iHeight  = max(iHeight,200);
		iHeight  = min(iHeight,m_iTreeHeight);

		int cy = ::GetSystemMetrics(SM_CYSCREEN) ;
		if(iHeight + rtWnd.bottom>cy)	rtWnd = CRect(rtWnd.left,rtWnd.top - iHeight,rtWnd.right,rtWnd.top);	
		else 		                    rtWnd = CRect(rtWnd.left,rtWnd.bottom,rtWnd.right,rtWnd.bottom + iHeight);
		m_pBaseTreeCtrl -> m_pTreeCtrl ->MoveWindow(CRect(0,0,rtWnd.Width(),rtWnd.Height()));
 		m_pDropDown ->MoveWindow(rtWnd);	
	}
}
void CTPComboTreeBox::GetComboBoxInfo(COMBOBOXINFO *comboBoxInfo)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	if((m_uStyle & 0x00000003) == CTS_SIMPLE)
	{
		rtClient.DeflateRect(2,2);
		comboBoxInfo->stateButton = STATE_SYSTEM_INVISIBLE;
		comboBoxInfo ->rcButton   = CRect(0,0,0,0);
		comboBoxInfo->rcItem      = rtClient;
	}
	else
	{
		CRect rtTemp =rtClient;
		if(m_uState & TREECOMBOX_BUTTONDOWN) comboBoxInfo->stateButton = STATE_SYSTEM_PRESSED;
		else                                 comboBoxInfo->stateButton = 0;
		rtTemp.DeflateRect(1,2);
		rtTemp.left = rtTemp.right - ::GetSystemMetrics(SM_CXVSCROLL);
		rtClient.right = rtTemp.left;
		rtClient.DeflateRect(2,2);
		comboBoxInfo ->rcButton   = rtTemp;
		comboBoxInfo->rcItem      = rtClient;
	}
	comboBoxInfo ->hwndCombo = m_hWnd;
	comboBoxInfo->hwndItem   = m_pEdit ->m_hWnd;
	comboBoxInfo ->hwndList  = m_pBaseTreeCtrl -> m_pTreeCtrl -> m_hWnd;
	
}

BOOL CTPComboTreeBox::SetStyle(UINT uAdd)
{
	if((uAdd & 0x00000003) == (m_uStyle & 0x00000003)) return TRUE;
	else 
	{
		m_uStyle &= ~0x00000003;
		m_uStyle |= (uAdd & 0x00000003);
		OnSize();
		if(m_pBaseTreeCtrl  && m_pBaseTreeCtrl ->m_pScrollData)
		{
			if((m_uStyle & 0x00000003) != CTS_SIMPLE)
			{

				((CTPScrollBar *)m_pBaseTreeCtrl ->m_pScrollData ->pHScroll) ->m_bSetCapture = FALSE;
				((CTPScrollBar *)m_pBaseTreeCtrl ->m_pScrollData ->pVScroll) ->m_bSetCapture = FALSE;
			}
			else
			{
				((CTPScrollBar *)m_pBaseTreeCtrl ->m_pScrollData ->pHScroll) ->m_bSetCapture = TRUE;
				((CTPScrollBar *)m_pBaseTreeCtrl ->m_pScrollData ->pVScroll) ->m_bSetCapture = TRUE;
			}
		}		 
		return TRUE;
	}	
}
void CTPComboTreeBox::SetDropHeight(int iHeight)
{
	m_iTreeHeight = iHeight;
}

void CTPComboTreeBox::SubWindow(BOOL bSub)
{		
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		m_uStyle |= GetStyle();				
		m_pDropDown = new CWnd();
		if(m_uBaseState & TP_BASESTATE_TOPMOST)
			m_pDropDown ->CreateEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST,NULL,NULL,WS_CLIPSIBLINGS|WS_CHILD,CRect(0,0,120,120),GetDesktopWindow(),100);						
		else
			m_pDropDown ->CreateEx(WS_EX_TOOLWINDOW,NULL,NULL,WS_CLIPSIBLINGS|WS_CHILD,CRect(0,0,120,120),GetDesktopWindow(),100);						
		if(m_bDirTree == 0)
		{
			m_pTreeCtrlEx = new CTPComboTBox();			
			m_pTreeCtrlEx ->Create(TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_HASLINES|WS_CHILD|WS_BORDER,CRect(0,0,120,120),m_pDropDown,101);				
			m_pTreeCtrlEx ->SetFont(TP_GetSysFont(m_iFontIndex));		
			m_pTreeCtrlEx ->m_pScrollData = TP_GetReplaceData(m_pTreeCtrlEx);
			m_pTreeCtrlEx ->m_pOwnerWnd = this;
			m_pBaseTreeCtrl = m_pTreeCtrlEx;
		}
		else 	if(m_bDirTree == 1)
		{
			m_pDTreeCtrlEx = new CTPComboDTBox();			
			m_pDTreeCtrlEx ->Create(TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_HASLINES|WS_CHILD|WS_BORDER,CRect(0,0,120,120),m_pDropDown,101);				
			m_pDTreeCtrlEx ->SetFont(TP_GetSysFont(m_iFontIndex));		
			m_pDTreeCtrlEx ->m_pScrollData = TP_GetReplaceData(m_pDTreeCtrlEx);
			m_pDTreeCtrlEx ->m_pOwnerWnd = this;
			m_pBaseTreeCtrl = m_pDTreeCtrlEx;
		}
		UINT uStyle =m_uStyle & 0x00000003;
		m_uStyle  &= ~0x00000003;
		SetStyle(uStyle);		
		if(m_pBaseTreeCtrl)
			SetClassLongPtr(m_pBaseTreeCtrl-> m_pTreeCtrl ->m_hWnd,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);
		m_pEdit = new CTPEdit();
		m_pEdit ->Create(WS_CHILD|ES_AUTOHSCROLL,CRect(0,0,0,0),this,99);
		m_pEdit ->SetFont(TP_GetSysFont(m_iFontIndex));
		SetClassLongPtr(m_pEdit->m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);		
		OnSize();
		SetWindowText(_L(""));
	}
	else 
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;		
		if(m_pTreeCtrlEx) 
		{
			m_pTreeCtrlEx ->m_pOwnerWnd = NULL;
			m_pTreeCtrlEx->DestroyWindow();
			delete m_pTreeCtrlEx;
			m_pTreeCtrlEx =NULL;			
		}
		if(m_pDTreeCtrlEx) 
		{
			m_pDTreeCtrlEx ->m_pOwnerWnd = NULL;
			m_pDTreeCtrlEx->DestroyWindow();
			delete m_pDTreeCtrlEx;
			m_pDTreeCtrlEx =NULL;			
		}
		m_pBaseTreeCtrl = NULL;
		if(m_pDropDown) 
		{
			m_pDropDown->DestroyWindow();
			delete m_pDropDown;
			m_pDropDown =NULL;			
		}		
		if(m_pEdit)
		{
			m_pEdit ->DestroyWindow();
			delete m_pEdit;
			m_pEdit = NULL;
		}
	}
}
BOOL CTPComboTreeBox::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CTPWnd::PreCreateWindow(cs);
}

void CTPComboTreeBox::PreSubclassWindow()
{
	CTPWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

int CTPComboTreeBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}

void CTPComboTreeBox::OnDestroy()
{
	SubWindow(FALSE);
	CTPWnd::OnDestroy();
}
void CTPComboTreeBox::PaintText(CDC *pDC,CRect rtText)
{
	CString sText;
	CTPWnd::GetWindowText(sText);
	pDC ->DrawText(sText,rtText,DT_LEFT | DT_SINGLELINE | DT_VCENTER);
}
void CTPComboTreeBox::OnPaint()
{
	CPaintDC dc(this); 

	CRect     rtClient;
	GetClientRect(&rtClient);
	COMBOBOXINFO   comboBoxInfo;
	comboBoxInfo.cbSize = sizeof(comboBoxInfo);
	CTPComboTreeBox::GetComboBoxInfo(&comboBoxInfo);
	if((m_uStyle & 0x00000003) == CTS_DROPDOWNLIST)
	{	
		CRect rtText =  rtClient;
		rtText.DeflateRect(3,3);
		rtText.right = comboBoxInfo.rcButton.left -1;
		CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());	
		cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner ->PaintRect(m_eBorderStyle,&cdcMemo,rtClient,2);	

		if(GetFocus() == this)	
		{
#ifdef TP_NS_EDIT
			cdcMemo.FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_EDITFOCUS));
#else
			cdcMemo.FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
#endif
			cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
		}
		else if(!IsWindowEnabled())
		{
			cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));
		}
		else
		{
#ifdef TP_NS_EDIT
			cdcMemo.FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_EDITREDONLY));
#endif
			cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));			
		}
#ifdef TP_NS_EDIT		
		cdcMemo.SetTextColor(RGB(0,0,0));
#endif
		CTPTreeCtrl *pTreeCtrl = m_pBaseTreeCtrl ->m_pTreeCtrl;
		CImageList *pImage = pTreeCtrl ->GetImageList(TVSIL_NORMAL);
		HTREEITEM   hItem  = pTreeCtrl ->GetSelectedItem();
		if(pImage && hItem)
		{
			int        iImage,iSelImage;
			IMAGEINFO  imgInfo;
			CString    sText;
			pTreeCtrl ->GetItemImage(hItem,iImage,iSelImage);
			sText = pTreeCtrl ->GetItemText(hItem);
			pImage ->GetImageInfo(iImage,&imgInfo);
			pImage ->Draw(&cdcMemo,iImage,CPoint(rtText.left+2,rtText.top + (rtText.Height() - (imgInfo.rcImage.bottom - imgInfo.rcImage.top))/2),ILD_TRANSPARENT);
			rtText.left += imgInfo.rcImage.right - imgInfo.rcImage.left  +4;
			cdcMemo.DrawText(sText,rtText,DT_LEFT | DT_SINGLELINE | DT_VCENTER);
		}
		else PaintText(&cdcMemo,rtText);
	

		PaintButton(&cdcMemo,comboBoxInfo);
		dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	
	}
	else
	{
		dc.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));		
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner ->PaintRect(m_eBorderStyle,&dc,rtClient,2);	
		PaintButton(&dc,comboBoxInfo);
	}
}
void CTPComboTreeBox::PaintButton(CDC *pDC,COMBOBOXINFO &comboBoxInfo)
{
	CRect rtButton = comboBoxInfo.rcButton;
	if(comboBoxInfo.stateButton!=STATE_SYSTEM_INVISIBLE)
	{
		if(comboBoxInfo.stateButton ==0)
		{			
			TP_StretchDIBits(pDC,m_pButton[0]->GetByte(),m_pButton[0]->Width(),m_pButton[0]->Height(),rtButton);
		}
		else if(comboBoxInfo.stateButton == STATE_SYSTEM_PRESSED)
		{
			TP_StretchDIBits(pDC,m_pButton[1]->GetByte(),m_pButton[1]->Width(),m_pButton[1]->Height(),rtButton);
		}			
	}
}

void CTPComboTreeBox::OnSetFocus(CWnd* pOldWnd)
{
	if((m_uStyle & 0x0000003) == CTS_DROPDOWNLIST)
	{
		SetRedraw(FALSE);
		CTPWnd::OnSetFocus(pOldWnd);
		SetRedraw(TRUE);
		Invalidate(FALSE);	
	}
	else CTPWnd::OnSetFocus(pOldWnd);
}

void CTPComboTreeBox::OnKillFocus(CWnd* pNewWnd)
{
	if((m_uStyle & 0x0000003) == CTS_DROPDOWNLIST)
	{	
		SetRedraw(FALSE);
		CTPWnd::OnKillFocus(pNewWnd);
		SetRedraw(TRUE);
		Invalidate(FALSE);
	}
	else CTPWnd::OnKillFocus(pNewWnd);
}

void CTPComboTreeBox::OnSize()
{
	if(m_pEdit == NULL) return;
	if((m_uStyle & 0x0000003) == CTS_DROPDOWNLIST) m_pEdit ->ShowWindow(SW_HIDE);
	else
	{
		COMBOBOXINFO comboBoxInfo;
		GetComboBoxInfo(&comboBoxInfo);
		m_pEdit ->ShowWindow(SW_SHOW);
		m_pEdit ->MoveWindow(&comboBoxInfo.rcItem);
	}	
}
void CTPComboTreeBox::OnSize(UINT nType, int cx, int cy)
{
	CTPWnd::OnSize(nType, cx, cy);
	OnSize();

	// TODO: 在此处添加消息处理程序代码
}
void CTPComboTreeBox::SelectItem(HTREEITEM hTreeItem)
{
}

LRESULT CTPComboTreeBox::OnMsgDirTree(WPARAM wp,LPARAM lp)
{
	if(3 == wp )
	{
		Invalidate(FALSE);	
        CTPTreeCtrl *pTreeCtrl = m_pBaseTreeCtrl ->m_pTreeCtrl;
		 
		HTREEITEM hItem = pTreeCtrl->GetSelectedItem();
		if(hItem)
		{
			m_hSelItem = hItem;
			CString sText = pTreeCtrl->GetItemText(hItem);
			if(m_pEdit && m_pEdit->IsWindowVisible())
			{
				m_pEdit->SetWindowText(sText);
			}
			else 
			{
				SetWindowText(sText);
			}
			CWnd *pParent = GetParent();
			if(pParent)
				pParent->PostMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_SELCHANGE),(LPARAM)m_hWnd);
		}
	}
	return S_FALSE;
}
void CTPComboTreeBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if((m_uStyle & 0x00000003) == CTS_SIMPLE)
	{
		CTPWnd::OnLButtonDown(nFlags, point);
		return;
	}
	else 
	{
		if(GetFocus()!=this && point != CPoint(0,0)) SetFocus();//25910
		if(m_uState & TREECOMBOX_BROPDOWN)
		{
			m_uState &= ~TREECOMBOX_BUTTONDOWN;
			m_uState &= ~TREECOMBOX_BROPDOWN;
			m_pDropDown ->ShowWindow(SW_HIDE);
		 		
			m_pBaseTreeCtrl ->m_pTreeCtrl -> ShowWindow(SW_HIDE);
			m_pBaseTreeCtrl ->m_bDropDown    = FALSE;		
			m_pBaseTreeCtrl ->m_bSetCapture  = FALSE;		 
			 
			if(m_pEdit && m_pEdit->IsWindowVisible()) m_pEdit ->SetFocus();
			else                                      SetFocus();
			Invalidate(FALSE);	
			HTREEITEM hItem = m_pBaseTreeCtrl ->m_pTreeCtrl ->GetSelectedItem();
			if(hItem && m_hSelItem != hItem)
			{
				m_hSelItem = hItem;
				CString sText = m_pBaseTreeCtrl ->m_pTreeCtrl ->GetItemText(hItem);
				if(m_pEdit && m_pEdit->IsWindowVisible()) m_pEdit->SetWindowText(sText);
				else SetWindowText(sText);
				GetParent() ->PostMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_SELCHANGE),(LPARAM)m_hWnd);
			}
			else GetParent() ->PostMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_CLOSEUP),(LPARAM)m_hWnd);
			SelectItem(hItem);
			ReleaseCapture();
			m_bSetCapture = FALSE;	
		}
		else
		{
			COMBOBOXINFO cmInfo;
			GetComboBoxInfo(&cmInfo);
			CRect rtTemp1= cmInfo.rcButton,rtTemp2 =cmInfo.rcItem;
			if(rtTemp1.PtInRect(point) || rtTemp2.PtInRect(point))
			{			
				m_uState |= TREECOMBOX_BUTTONDOWN;
				m_uState |= TREECOMBOX_BROPDOWN;				
				Invalidate(FALSE);
				CRect rtWnd;
				GetWindowRect(&rtWnd);	
				int iHeight =   GetTreeHeight( m_pBaseTreeCtrl ->m_pTreeCtrl,NULL) +20;
 				iHeight  = max(iHeight,200);
				iHeight  = min(iHeight,m_iTreeHeight);

				int cy = ::GetSystemMetrics(SM_CYSCREEN) ;
				if(iHeight + rtWnd.bottom>cy)
				{
					rtWnd = CRect(rtWnd.left,rtWnd.top - iHeight,rtWnd.right,rtWnd.top);
				}
				else
				{
					rtWnd = CRect(rtWnd.left,rtWnd.bottom,rtWnd.right,rtWnd.bottom + iHeight);
				}

				m_pBaseTreeCtrl ->m_pTreeCtrl ->ShowWindow(SW_SHOW);
				m_pBaseTreeCtrl ->m_pTreeCtrl ->MoveWindow(CRect(0,0,rtWnd.Width(),rtWnd.Height()));
				m_pBaseTreeCtrl ->m_pOwnerWnd    = this;
				m_pBaseTreeCtrl ->m_bButtonPress = TRUE;

				m_pDropDown ->MoveWindow(rtWnd);
				m_pDropDown ->ShowWindow(SW_SHOW);		
				m_pDropDown ->BringWindowToTop();		

				m_pBaseTreeCtrl ->m_pTreeCtrl ->SetCapture();
				m_pBaseTreeCtrl ->m_pTreeCtrl ->SetFocus();
				m_pBaseTreeCtrl ->m_pTreeCtrl ->Invalidate(FALSE);
				m_pBaseTreeCtrl ->m_bDropDown    = TRUE;
				m_pBaseTreeCtrl ->m_bSetCapture  = TRUE;
			 
				 		
				GetParent() ->PostMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_DROPDOWN),(LPARAM)m_hWnd);
				m_hSelItem = m_pBaseTreeCtrl ->m_pTreeCtrl -> GetSelectedItem();	///6857			
				m_pBaseTreeCtrl ->m_pTreeCtrl ->m_uBaseState  |= TP_BASESTATE_SELCHANGE;
 			}
		}
	}
}

void CTPComboTreeBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if((m_uStyle & 0x00000003) == CTS_SIMPLE)
	{
		CTPWnd::OnLButtonUp(nFlags, point);
	}
	else
	{
		if(m_uState &TREECOMBOX_BROPDOWN)
		{			
			if (m_uState & TREECOMBOX_BUTTONDOWN)
			{
				m_uState &= ~TREECOMBOX_BUTTONDOWN;
				m_pBaseTreeCtrl ->m_bButtonPress = FALSE;									
				Invalidate(FALSE);
				return;
			}									
		}
		else if(m_bSetCapture) ReleaseCapture();
		else CTPWnd::OnLButtonUp(nFlags, point);
	}
}

void CTPComboTreeBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if((m_uStyle & 0x00000003) == CTS_SIMPLE)
	{
		CTPWnd::OnMouseMove(nFlags, point);
		return;
	}
	else
	{
		if(m_uState &TREECOMBOX_BROPDOWN)
		{
			if (m_uState & TREECOMBOX_BUTTONDOWN)
			{
				COMBOBOXINFO cmInfo;
				GetComboBoxInfo(&cmInfo);
				CRect rtTemp= cmInfo.rcButton;
				if(!rtTemp.PtInRect(point))
				{
					m_uState &= ~TREECOMBOX_BUTTONDOWN;
					m_pBaseTreeCtrl ->m_bButtonPress  = FALSE; 
					Invalidate(FALSE);
					return;
				}
			}			
		}
		else CTPWnd::OnMouseMove(nFlags, point);
	}
}
 LRESULT CTPComboTreeBox::OnNcHitTest(CPoint point)
{
	return HTCLIENT;
}
BOOL  CTPComboTreeBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(WC_COMBOTREEBOX,NULL,dwStyle,rect,pParentWnd,nID);
}
BOOL CTPComboTreeBox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CTPWnd::OnEraseBkgnd(pDC);
}


BOOL CTPComboTreeBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	///zhaofei 在没有垂直滚动条的时候，水平滚动条不能滚动，所以改为下面的形式
	if(m_uState & TREECOMBOX_BROPDOWN)	
	{
		if(m_pBaseTreeCtrl -> m_pScrollData)
		{
			m_pBaseTreeCtrl ->m_pTreeCtrl ->PostMessage(WM_MOUSEWHEEL,MAKELONG(nFlags,zDelta),MAKELONG(pt.x,pt.y));
			return TRUE;
		}		 
	}
	else 
	{
		HTREEITEM hSel1,hSel2;
		 
			hSel1 = m_pBaseTreeCtrl ->m_pTreeCtrl->GetSelectedItem();
			if(hSel1)
			{
				if(zDelta<0)	hSel2 = m_pBaseTreeCtrl ->m_pTreeCtrl ->GetNextVisibleItem(hSel1);
				else        	hSel2 = m_pBaseTreeCtrl ->m_pTreeCtrl ->GetPrevVisibleItem(hSel1);
			}
			if(hSel2) m_pBaseTreeCtrl ->m_pTreeCtrl->SelectItem(hSel2);
			hSel2 = m_pBaseTreeCtrl ->m_pTreeCtrl->GetSelectedItem();
		 
		if(hSel1 != hSel2)
		{
			GetParent()->SendMessage(WM_COMMAND ,MAKELONG(GetDlgCtrlID(),CBN_SELCHANGE),(LPARAM)m_hWnd);
			m_pBaseTreeCtrl ->m_pTreeCtrl ->m_uBaseState  &= ~TP_BASESTATE_SELCHANGE;
 			Invalidate();
		}
		return TRUE;
	} 

	return __super::OnMouseWheel(nFlags, zDelta, pt);
}
LRESULT CTPComboTreeBox::OnMsgShowDropDown(WPARAM wp,LPARAM lp)
{
	if(wp)
	{
		if(GetDroppedState())  return S_OK;
		else OnLButtonDown(0,CPoint(10,10));
	}
	else
	{
		if(!GetDroppedState())  return S_OK;
		else OnLButtonDown(0,CPoint(10,10));
	}
	return S_OK;
}
BOOL CTPComboTreeBox::GetDroppedState()
{
	if(m_uState & TREECOMBOX_BROPDOWN) return TRUE;
	else return FALSE;
}

void CTPComboTreeBox::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_pBaseTreeCtrl ->m_pTreeCtrl ->m_uBaseState  |= TP_BASESTATE_SELCHANGE;
 	*pResult = 0;
}

BOOL CTPComboTreeBox::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg ->message == WM_KEYDOWN && pMsg ->wParam == VK_RETURN && GetDroppedState())
	{
		if(m_pBaseTreeCtrl ->m_pTreeCtrl ->m_uBaseState & TP_BASESTATE_SELCHANGE)
		{
			GetParent()->SendMessage(WM_COMMAND ,MAKELONG(GetDlgCtrlID(),CBN_SELENDOK),(LPARAM)m_hWnd);
			m_pBaseTreeCtrl ->m_pTreeCtrl ->m_uBaseState  &= ~TP_BASESTATE_SELCHANGE;
		}
		OnMsgShowDropDown(0,0);
		return TRUE;
	}
	if(pMsg ->message == WM_KEYDOWN && (pMsg ->wParam == VK_UP || pMsg ->wParam == VK_DOWN) )
	{
		HTREEITEM hSel1 = NULL,hSel2 = NULL;

		m_pBaseTreeCtrl ->m_pTreeCtrl ->m_uState |= STATE_SETDATA;
		hSel1 = m_pBaseTreeCtrl ->m_pTreeCtrl->GetSelectedItem();
		if(hSel1)
		{
			if(pMsg ->wParam == VK_DOWN)	hSel2 = m_pBaseTreeCtrl ->m_pTreeCtrl ->GetNextVisibleItem(hSel1);
			else                         	hSel2 = m_pBaseTreeCtrl ->m_pTreeCtrl ->GetPrevVisibleItem(hSel1);
		}
		if(hSel2) m_pBaseTreeCtrl ->m_pTreeCtrl->SelectItem(hSel2);
		hSel2 = m_pBaseTreeCtrl ->m_pTreeCtrl->GetSelectedItem();
		m_pBaseTreeCtrl ->m_pTreeCtrl ->m_uState &= ~STATE_SETDATA;
		if(hSel1 != hSel2)
		{ 
			if(GetDroppedState())
			{
				m_pBaseTreeCtrl ->m_pTreeCtrl ->m_uBaseState  |= TP_BASESTATE_SELCHANGE;
			}
			else
			{
				GetParent()->SendMessage(WM_COMMAND ,MAKELONG(GetDlgCtrlID(),CBN_SELCHANGE),(LPARAM)m_hWnd);
				m_pBaseTreeCtrl ->m_pTreeCtrl ->m_uBaseState  &= ~TP_BASESTATE_SELCHANGE;
			}			 
			Invalidate();
		}

		return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}
INT_PTR CTPComboTreeBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
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
BOOL CTPComboTreeBox::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	if(m_uBaseState & TP_BASESTATE_ENABLETOOLTIP)
	{	
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
		CPoint ptCursor;
		::GetCursorPos(&ptCursor); 
		ScreenToClient(&ptCursor);
		CString  sToolTips = m_sToolTips;
		if(sToolTips.GetLength()<=0) GetWindowText(sToolTips);
		lstrcpyn( pTTT->szText,sToolTips,80);	
		return  TRUE;			
	}
	else return FALSE;
}
void CTPComboTreeBox::EnableToolTips(BOOL bEnable)
{
	if(bEnable)
	{	
		m_uBaseState |= 	TP_BASESTATE_ENABLETOOLTIP;
		CWnd::EnableToolTips(TRUE);	
	}
	else
	{
		m_uBaseState &= 	~TP_BASESTATE_ENABLETOOLTIP;
		CWnd::EnableToolTips(FALSE);	
	}
}
