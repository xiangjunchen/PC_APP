#include "StdAfx.h"
#include ".\tpcombomenu.h"


#define TP_BASECMD    10001
#define CM_DISABLE    0x02000000
#define CM_SPECIALSEL 0x01000000  

IMPLEMENT_DYNAMIC(CTPComboMenu, CTPWnd)
CTPComboMenu::CTPComboMenu()
{
	m_dwWindowLevel = TPWNDLEVEL1; 

	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_eBorderStyle  = TP_GetBorderStyle();

	m_sWndText         = _T("");
	m_iSelText         = -1;


	m_pButton[0]    = TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\ComboBox\\button_nor.bmp")); 	
	m_pButton[1]    = TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\ComboBox\\button_down.bmp")); 	

	m_pMenu         = NULL;
	m_bPressed      = FALSE;
	m_hTimer        = 0;
	m_bSort         = FALSE;
	m_bDropDown     = FALSE; 
	m_pDropMenu     = NULL;
	m_sToolTips     = _T("");

}

CTPComboMenu::~CTPComboMenu()
{
	for(INT l=0;l<m_aItem.GetSize();l++)	delete m_aItem[l];
	m_aItem.RemoveAll();
	SubWindow(FALSE);
	TP_ReleaseCommonRes(m_pButton[0]);
	TP_ReleaseCommonRes(m_pButton[1]);
}


BEGIN_MESSAGE_MAP(CTPComboMenu, CTPWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()		
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()	
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_CLOSEMENU,OnCloseMenu)
	ON_WM_DRAWITEM()
	ON_COMMAND_RANGE(TP_BASECMD,TP_BASECMD*2,OnMenuCmd)
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff,       OnToolTipNotify )
END_MESSAGE_MAP()

void CTPComboMenu::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;				
		m_uStyle = GetStyle();	
		m_bSort  = m_uStyle & CBS_SORT;
		m_pMenu = new CTPMenuEx;
		m_pMenu ->m_bComMenu = TRUE;
		m_pMenu->LoadMenu(IDR_EXTRAMENU);
		SaveFont(m_hWnd);
	}
	else 
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;
		if(m_pMenu)
		{
			m_pMenu->DestroyMenu();
			delete m_pMenu;
			m_pMenu = NULL;
		}
		RestoreFont(m_hWnd);
		EnableToolTips(FALSE);
	}
}

BOOL CTPComboMenu::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CTPWnd::PreCreateWindow(cs);
}
void CTPComboMenu::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_WND,&wndClass);		
		wndClass.lpszClassName = WC_COMBOMENU;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_COMBOMENU,AfxGetInstanceHandle());
}
void CTPComboMenu::PreSubclassWindow()
{
	CTPWnd::PreSubclassWindow();
	if(!m_bCreate)	SubWindow(TRUE);
}
void CTPComboMenu::OnDestroy()
{
	SubWindow(FALSE);
	if(m_hTimer) KillTimer(m_hTimer);
	m_hTimer = NULL;
	CTPWnd::OnDestroy();
}
void CTPComboMenu::SetLBText(int iItem,CString sText)
{
	if(iItem <0 || iItem >= m_aItem.GetSize()) return;
	if(m_iSelText == iItem){m_sWndText = sText;Invalidate();}
	m_aItem[iItem]->sText = sText;	
}
void CTPComboMenu::GetLBText(int iItem,CString &sText)
{
	if(iItem <0 || iItem >= m_aItem.GetSize()) return;
	sText = m_aItem[iItem]->sText;
}
void CTPComboMenu::ResetContent()
{
	m_sWndText.Empty();
	m_iSelText = -1;
	for(INT l=0;l<m_aItem.GetSize();l++)
		delete m_aItem[l];
	m_aItem.RemoveAll();
	Invalidate();
}
void CTPComboMenu::SortItem()
{
	for(INT l=0;l<m_aItem.GetSize();l++)
		for(INT k=l;k<m_aItem.GetSize();k++)
		{
			if(m_aItem[l]->sText.Compare(m_aItem[k] ->sText)>0)
			{
				CTPComboMenu::tag_StrItem *pItem = m_aItem[l];
				m_aItem[l] = m_aItem[k];
				m_aItem[l] = pItem;
			}
		}
}
int CTPComboMenu::AddString(CString sText)
{	
	CTPComboMenu::tag_StrItem *pItem = new CTPComboMenu::tag_StrItem;
	pItem->sText = sText;
	m_aItem.Add(pItem);
	if(m_bSort) SortItem();
	return (int)m_aItem.GetSize()-1;
}
int CTPComboMenu::SetCurSel(int iSel)
{
	int iSelSave = m_iSelText;

	if(iSel <0 || iSel >= m_aItem.GetSize())
	{
		m_sWndText = _T("");
		m_iSelText = -1;
	}
	else //by zhh
	{
		m_iSelText = iSel;
		m_sWndText = m_aItem[iSel] ->sText;
	}

	Invalidate();
	return iSelSave;
}
int CTPComboMenu::GetCount()
{
	return (int) m_aItem.GetSize();
}
void* CTPComboMenu::GetItemDataPtr(int index)
{
	if(index <0 || index > m_aItem.GetSize()) return NULL;
	return m_aItem[index]->pData;
}
int CTPComboMenu::SetItemDataPtr(int index, void* pData)
{
	if(index <0 || index > m_aItem.GetSize()) return 0;
	m_aItem[index]->pData = pData;
	return 1;
}

void CTPComboMenu::SetItemData(int index,DWORD_PTR dwData)
{
	if(index <0 || index > m_aItem.GetSize()) return ;
	m_aItem[index]->dwData = dwData;
}
DWORD_PTR CTPComboMenu::GetItemData(int index)
{
	if(index <0 || index > m_aItem.GetSize()) return 0;
	return m_aItem[index]->dwData;
}
void CTPComboMenu::SetItemGUID(int index,GUID dwData)
{
	if(index <0 || index > m_aItem.GetSize()) return ;
	m_aItem[index]->guidData = dwData;
}
GUID CTPComboMenu::GetItemGUID(int index)
{
	if(index <0 || index > m_aItem.GetSize()) return GUID_NULL;
	return m_aItem[index]->guidData;
}
int CTPComboMenu::InsertString(int index,CString sText)
{
	if(index <0 || index > m_aItem.GetSize()) return -1;
	CTPComboMenu::tag_StrItem *pItem = new CTPComboMenu::tag_StrItem;
	pItem->sText = sText;
	m_aItem.InsertAt(index,pItem);
	if(m_bSort) SortItem();
	return 0;
}
int CTPComboMenu::GetCurSel()
{
	return m_iSelText;
}
void CTPComboMenu::DeleteString(int iIndex)
{
	DeleteItem(iIndex);
}
void CTPComboMenu::DeleteString(CString sText)
{
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l]->sText == sText)
		{
			if(m_iSelText == l){m_sWndText.Empty();m_iSelText = -1;Invalidate();}
			delete m_aItem[l];
			m_aItem.RemoveAt(l);
			if(m_bSort) SortItem();
			return;
		}
	}
}
void CTPComboMenu::DeleteItem( int iItem) //by zhh
{
	if( iItem >= 0 && iItem < m_aItem.GetSize())
	{
		delete m_aItem[iItem];
		m_aItem.RemoveAt( iItem);
		if( m_bSort) SortItem();

		Invalidate();
	}
}
void CTPComboMenu::PaintButton(CDC *pDC,COMBOBOXINFO &comboBoxInfo)
{
	CRect rtButton = comboBoxInfo.rcButton;

	m_rtComBtn = rtButton;
	if(comboBoxInfo.stateButton ==0)
	{			
		TP_StretchDIBits(pDC,m_pButton[0]->GetByte(),m_pButton[0]->Width(),m_pButton[0]->Height(),rtButton);
	}
	else if(comboBoxInfo.stateButton == STATE_SYSTEM_PRESSED)
	{
		TP_StretchDIBits(pDC,m_pButton[1]->GetByte(),m_pButton[1]->Width(),m_pButton[1]->Height(),rtButton);
	}

}
void CTPComboMenu::OnPaint()
{
	CPaintDC dc(this); 
	DRAWITEMSTRUCT stuDraw;
	stuDraw.hDC = dc.m_hDC;
	DrawItem(&stuDraw);	
}


void CTPComboMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{	
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC); 

	CRect     rtClient;
	GetClientRect(&rtClient);
	COMBOBOXINFO   comboBoxInfo;
	comboBoxInfo.cbSize           = sizeof(comboBoxInfo);
	comboBoxInfo.rcButton         = rtClient;
	comboBoxInfo.rcButton.top    += 2;
	comboBoxInfo.rcButton.right  -= 2;
	comboBoxInfo.rcButton.bottom -= 2;
	comboBoxInfo.rcButton.left    = comboBoxInfo.rcButton.right - rtClient.Height() * m_pButton[0]->Width()/m_pButton[0]->Height();	
	comboBoxInfo.stateButton      = m_bPressed ? STATE_SYSTEM_PRESSED : 0;

	CRect rtText =  rtClient;
	rtText.DeflateRect(3,3);
	rtText.right = comboBoxInfo.rcButton.left -1;
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());	
	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_EDITREDONLY));

	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,&cdcMemo,rtClient,2);	

	if(GetFocus() == this)	
	{		
		cdcMemo.FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_EDITFOCUS));
		cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
	}
	else if(!IsWindowEnabled())
	{
		cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));
	}
	else cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));	
	cdcMemo.SetTextColor(RGB(0,0,0));
	cdcMemo.DrawText(m_sWndText,rtText,DT_LEFT | DT_SINGLELINE | DT_VCENTER);		
	PaintButton(&cdcMemo,comboBoxInfo);
	pDC->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	

 }

int CTPComboMenu::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	SetFont(TP_GetSysFont(m_iFontIndex));
	// TODO:  在此添加您专用的创建代码
	return 0;
}
BOOL CTPComboMenu::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//CTPWnd::OnEraseBkgnd(pDC);	
}
HBRUSH CTPComboMenu::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
#ifdef  TP_NS_EDIT
	if((m_uStyle & ES_READONLY) || (m_uStyle & CM_DISABLE))   
	{		
		return TP_GetSysBrush(BRUSH_EDITREDONLY);	  
	}
	else if(GetFocus() == this)	
	{	
		return TP_GetSysBrush(BRUSH_EDITFOCUS);	  
	}
	else
	{	
		return TP_GetSysBrush(BRUSH_EDITREDONLY);	  
	}
#endif
	return NULL;
}
void CTPComboMenu::OnLButtonDown(UINT nFlags, CPoint point)
{	
	if(m_uStyle & CM_DISABLE) return;
	if(!m_bPressed) Invalidate();
	if(GetFocus() != this) SetFocus();
	m_bPressed = TRUE;	
	m_hTimer   = SetTimer(1000,100,NULL);
	DropDown();
	__super::OnLButtonDown(nFlags, point);
}

void CTPComboMenu::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_uStyle & CM_DISABLE) return;
	if(m_bPressed) Invalidate();
	m_bPressed = FALSE;	
	if(m_hTimer) KillTimer(m_hTimer);
	m_hTimer   = NULL;	
	__super::OnLButtonUp(nFlags, point);
}

void CTPComboMenu::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1000)
	{
		CPoint pt;
		GetCursorPos(&pt);		
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		if(!rtWnd.PtInRect(pt) || GetKeyState(VK_LBUTTON) >=0)
		{
			KillTimer(m_hTimer);
			m_hTimer = NULL;
			if(m_bPressed) Invalidate();
			m_bPressed = FALSE;
		}
	}
	__super::OnTimer(nIDEvent);
}
int CTPComboMenu::GetSelItem(CString sText)
{
	for(INT l=1;l<m_aItem.GetSize();l++)
	{
		if(sText == m_aItem[l]->sText) return l;
	}	
	return m_iSelText;
}
void CTPComboMenu::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_DOWN || nChar == VK_UP)
	{
		int iNewSel, iSel;

		iNewSel = iSel = m_iSelText;		
		if(m_uStyle & CM_SPECIALSEL) 
			iNewSel = iSel = GetSelItem(m_sWndText);

NextSel:

		if( nChar == VK_DOWN)
			++ iSel;
		else if( nChar == VK_UP)
			-- iSel;

		if( iSel >= 0 && iSel < m_aItem.GetSize())
		{
			if( m_aItem[iSel] ->sText.Find(_T("-----")) == 0)
				goto NextSel;

			iNewSel = iSel;
		}

		if( iNewSel == m_iSelText)
			return;
		m_iSelText = iNewSel;

		m_sWndText = m_aItem[m_iSelText]->sText;
		Invalidate();
		FunNotifyParent(CBN_SELENDOK,m_iSelText);
	}

	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}
LRESULT CTPComboMenu::FunNotifyParent(UINT nNotify,int iItem) 
{
	GetParent() ->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID() ,nNotify),(LPARAM)m_hWnd);
	return S_OK;
}
void CTPComboMenu::SetGetData(BOOL bGetData)
{
	if(bGetData)
		m_uBaseState |= TP_BASESTATE_ERASE;
	else 
		m_uBaseState &= ~TP_BASESTATE_ERASE;
}
void CTPComboMenu::DropDown()
{
	if(m_bDropDown) return;
	m_bDropDown = TRUE;

	if(m_uBaseState&TP_BASESTATE_ERASE)
	{
		m_uBaseState &= ~TP_BASESTATE_ERASE;
		GetParent()->SendMessage(WM_COMMBOX,2,(LPARAM)m_hWnd);
	}

	CRect rtWnd;
	GetWindowRect(&rtWnd);

	CTPMenuEx *pSubMenu = m_pMenu ->GetSubMenu(1);
	while(pSubMenu->GetMenuItemCount()>0) pSubMenu->DeleteMenu(0,MF_BYPOSITION);
	pSubMenu ->m_lMinWidth   = rtWnd.Width()-20;
	pSubMenu->m_iMenucy      = -1;
	pSubMenu ->m_uBaseState |=TP_BASESTATE_COMBOBOXMENU;
	pSubMenu ->m_rtMenuBox   = m_rtComBtn;
	ClientToScreen(&pSubMenu ->m_rtMenuBox);
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l]->sText.Find(_T("-----"))==0) pSubMenu->AppendMenu(MF_BYCOMMAND|MF_SEPARATOR,0,_T(""));
		else                                       pSubMenu->AppendMenu(MF_BYCOMMAND|MF_STRING,TP_BASECMD+l,(LPCTSTR)m_aItem[l]->sText);
		m_aItem[l]->uID = TP_BASECMD +l;
	}
	for(INT l=0;l<pSubMenu->m_aMenuItem.GetSize();l++)
		pSubMenu->m_aMenuItem[l]->lMinWidth = pSubMenu ->m_lMinWidth;
	TP_LoadMenuStr(pSubMenu);
	
	CRect rtClient;
	GetClientRect(rtClient);
	pSubMenu ->m_lCheckWidth = 0;
	pSubMenu ->m_dwType     |= TP_MENU_UNCHECK ;
 	pSubMenu ->m_bComMenu    = TRUE;
	pSubMenu ->m_rtMenuBox = rtWnd;
	m_pDropMenu = pSubMenu;

	if(m_iSelText>=0)
	{		
		m_pDropMenu ->m_iSelectID = m_pDropMenu ->GetMenuItemID(m_iSelText);
		/*MENUITEMINFO stuMenuItemInfo;ZeroMemory(&stuMenuItemInfo,sizeof(MENUITEMINFO));
		stuMenuItemInfo.cbSize = sizeof(MENUITEMINFO);
		stuMenuItemInfo.fMask = MIIM_STATE;
		m_pDropMenu ->GetMenuItemInfo(m_iSelText,&stuMenuItemInfo,TRUE);
		stuMenuItemInfo.fState |= MFS_HILITE;
		m_pDropMenu ->SetMenuItemInfo(m_iSelText,&stuMenuItemInfo,TRUE);*/
	}

	//pSubMenu ->SetSendCloseMsg(TRUE);

	if(CTPBaseFrame::m_rtScreen.bottom - long(pSubMenu ->GetMenuItemCount()*19) <rtWnd.bottom && rtWnd.bottom >CTPBaseFrame::m_rtScreen.Height()/2)
		pSubMenu ->TrackPopupMenu(TPM_LEFTBUTTON|TPM_BOTTOMALIGN,rtWnd.left-1,rtWnd.top,this);
	else
		pSubMenu ->TrackPopupMenu(TPM_LEFTBUTTON,rtWnd.left-1,rtWnd.bottom,this);

	m_pDropMenu = NULL;
	FunNotifyParent(CBN_DROPDOWN,0);
}
LRESULT CTPComboMenu::OnCloseMenu(WPARAM wp,LPARAM lp)
{
	m_bDropDown = FALSE;
	FunNotifyParent(CBN_CLOSEUP,0);
	TP_RemoveMessage(WM_LBUTTONDOWN);
	TP_RemoveMessage(WM_LBUTTONUP);
	return S_OK;		 
}
void CTPComboMenu::OnMenuCmd(UINT uID)
{
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l]->uID == uID)
		{
			m_iSelText = l;
			m_sWndText = m_aItem[l]->sText;
			Invalidate();
			FunNotifyParent(CBN_SELENDOK,l);
			break;
		}
	}	
}
void CTPComboMenu::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);
	Invalidate();
}

void CTPComboMenu::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);
	Invalidate();
}
BOOL CTPComboMenu::PreTranslateMessage(MSG* pMsg)
{	
	if(m_pDropMenu && m_pDropMenu  ->m_hAttachWnd != NULL && pMsg->message == WM_MOUSEWHEEL)
	{
		if(HIWORD(pMsg->wParam)>0)  ::PostMessage(m_pDropMenu  ->m_hAttachWnd,WM_KEYDOWN,VK_UP,0);
		else                        ::PostMessage(m_pDropMenu  ->m_hAttachWnd,WM_KEYDOWN,VK_DOWN,0);
	}

	return __super::PreTranslateMessage(pMsg);
}
void CTPComboMenu::EnableSpecialSel(BOOL bEnable)
{
	if(bEnable)	m_uStyle |=  CM_SPECIALSEL;
	else 	    m_uStyle &= ~CM_SPECIALSEL;
}
void CTPComboMenu::EnableSelect(BOOL bEnable)
{
	if(bEnable)		m_uStyle &= ~CM_DISABLE;
	else 		    m_uStyle |= CM_DISABLE;
	Invalidate();
}
void CTPComboMenu::EnableToolTips(BOOL bEnable)
{
	if(bEnable)
	{	
		if(!(m_uBaseState & TP_BASESTATE_ENABLETOOLTIP))
		{		
			m_uBaseState |= 	TP_BASESTATE_ENABLETOOLTIP;
			CWnd::EnableToolTips(TRUE);	
		}
	}
	else
	{
		if(m_uBaseState & TP_BASESTATE_ENABLETOOLTIP)
		{		
			m_uBaseState &= 	~TP_BASESTATE_ENABLETOOLTIP;
			CWnd::EnableToolTips(FALSE);	
		}
	}
}
void CTPComboMenu::SetToolTips(CString sToolTips)
{	
	m_sToolTips = sToolTips;
	EnableToolTips(m_sToolTips.GetLength()>0);
}
BOOL CTPComboMenu::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	if(m_uBaseState &TP_BASESTATE_ENABLETOOLTIP)
	{	
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
		CString  sToolTips = m_sToolTips;
		if(sToolTips.GetLength()<=0) sToolTips = m_sWndText;
		lstrcpyn( pTTT->szText,sToolTips,80);	
		return  TRUE;		
	}
	else return FALSE;
}
INT_PTR CTPComboMenu::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if(m_uBaseState &TP_BASESTATE_ENABLETOOLTIP)
	{		
		pTI ->hwnd     = m_hWnd;	
		pTI ->lpszText = LPSTR_TEXTCALLBACK;
		pTI ->uId      = (UINT)(UINT_PTR)m_hWnd;
		pTI ->uFlags  |= TTF_IDISHWND;
		return  GetDlgCtrlID();			
	}
	else return __super::OnToolHitTest(point, pTI);
}
BOOL CTPComboMenu::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if( zDelta < 0)
		OnKeyDown( VK_DOWN, 0, 0);
	else if( zDelta > 0)
		OnKeyDown( VK_UP, 0, 0);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

//add by chenxiaolong,DQAdb00007814,2009.09.07
void  CTPComboMenu::OnDrawItem(int nIDCtl,LPDRAWITEMSTRUCT lpDrawItemStruct)
{
/*	if(lpDrawItemStruct->CtlType&ODT_MENU)
	{
		if(lpDrawItemStruct->itemAction&ODA_SELECT)
		{
			if(lpDrawItemStruct->itemState&ODS_SELECTED)
			{
				CTPMenuEx * pMenu=m_pMenu ->GetSubMenu(1);
				if(!pMenu)
					return;
				UINT  uTmp = pMenu->GetMenuItemID(m_iSelText);
				if(uTmp!=lpDrawItemStruct->itemID)
				{
					MENUITEMINFO stuMenuItemInfo;
					
					stuMenuItemInfo.cbSize = sizeof(MENUITEMINFO);
					stuMenuItemInfo.fMask = MIIM_DATA;
					pMenu ->GetMenuItemInfo(m_iSelText,&stuMenuItemInfo,TRUE);
					//pMenu->SetMenuItemInfo(m_iSelText,&stuMenuItemInfo,TRUE);

				   LPDRAWITEMSTRUCT lpDrawItemStr=new DRAWITEMSTRUCT();
		
					lpDrawItemStr->CtlType=lpDrawItemStruct->CtlType;
					lpDrawItemStr->CtlID=lpDrawItemStruct->CtlID;
					lpDrawItemStr->itemID=TP_BASECMD + m_iSelText;
					lpDrawItemStr->itemAction=lpDrawItemStruct->itemAction;
					lpDrawItemStr->itemState&=MFS_UNHILITE;
					lpDrawItemStr->hwndItem=lpDrawItemStruct->hwndItem;
					lpDrawItemStr->hDC=lpDrawItemStruct->hDC;			
					lpDrawItemStr->rcItem = pMenu->m_aMenuItem[m_iSelText]->rtItem;
					lpDrawItemStr->itemData=(ULONG_PTR)stuMenuItemInfo.dwItemData;
					pMenu->DrawItem(lpDrawItemStr);
					delete lpDrawItemStr;
					lpDrawItemStr=NULL;
					return CWnd::OnDrawItem(nIDCtl,lpDrawItemStruct);
				}
			}
		}
	}*/
	return CWnd::OnDrawItem(nIDCtl,lpDrawItemStruct);
}