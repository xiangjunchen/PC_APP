// TPEditListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPEditListCtrl.h"
#include ".\tpeditlistctrl.h"

#define   WM_EDITLISTCTRLBEGIN              WM_USER+125
#define   WM_RENAMEBEGIN					WM_USER+127  //zhaofei

// CTPEditListCtrl

IMPLEMENT_DYNAMIC(CTPEditListCtrl, CTPListCtrl)
CTPEditListCtrl::CTPEditListCtrl()
{
	ZeroMemory(m_dwColEdit,100 * sizeof(DWORD));
	m_pThumbBmp     = NULL;
	m_iItemSave     = -1;
	m_iItemSubSave  = -1;
	m_rtItemSubSave = CRect(0,0,0,0);

	m_iItemLast     = -1;
	m_iItemSubLast  = -1;

	m_pEdit         = NULL;
	m_pTimeCodeEdit = NULL;
	m_pEditWnd      = NULL;  
	m_pMsgWnd       = NULL;
	m_bCanEdit      = TRUE; 
}

CTPEditListCtrl::~CTPEditListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTPEditListCtrl, CTPListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_EN_KILLFOCUS(10,OnEnKillFocus)
	ON_MESSAGE(WM_EDITLISTCTRLBEGIN,OnMessageListCtrl)
	ON_MESSAGE(WM_TIMECODE_CHANGE, OnMessageTimeCode)
	ON_MESSAGE(WM_RENAMEBEGIN,OnMessageReName)

END_MESSAGE_MAP()

void     CTPEditListCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle() ,WC_LISTVIEW,&wndClass);				
		wndClass.lpszClassName = WC_EDITLISTCTRL;	
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_EDITLISTCTRL,AfxGetInstanceHandle());	
}
void CTPEditListCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		CTPListCtrl::SubWindow(bSub);
		ModifyStyle(0,WS_CLIPCHILDREN,FALSE);
		if(m_pHeadCtrl)		m_pHeadCtrl ->ModifyStyle(HDS_DRAGDROP,0,FALSE); 
		m_pThumbBmp =  TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\ProgressCtrl\\Progress_H_thumb.bmp"));
		if(m_pMsgWnd == NULL) m_pMsgWnd = GetParent();
	}
	else
	{	
		CTPListCtrl::SubWindow(bSub);
		TP_ReleaseCommonRes(m_pThumbBmp);
	}
}
// CTPEditListCtrl 消息处理程序
void     CTPEditListCtrl::GetEditItem(int &iItem,int &iSubItem,CWnd *&pWnd)
{
	iItem    = m_iItemSave;
	iSubItem = m_iItemSubSave;
	pWnd     = m_pEditWnd;
}
void     CTPEditListCtrl::SetItemPos(int iItem,int iSubItem,int iPos)
{
	CRect rtText,rtClient;
	CString sText;
	sText.Format(_L("%d"),iPos);
	SetItemText(iItem,iSubItem,sText);
	GetItemRect(iItem,rtText,FALSE);
	GetClientRect(&rtClient);
	rtText.left  = rtClient.left;
	rtText.right = rtClient.right;
	InvalidateRect(rtText);	
}

void     CTPEditListCtrl::SetColumeEditType(int iCol,DWORD dEditType)
{
	m_dwColEdit[iCol] = dEditType;
}

//////////////////////////////////////////////////////////////////////////zhaofei   XN00016577[[
DWORD   CTPEditListCtrl::GetColumnEditType(int iCol)
{
	if(iCol >= 0 && iCol < 100)
		return m_dwColEdit[iCol];
	else
		return TP_COLOUM_NULL;
}
//////////////////////////////////////////////////////////////////////////zhaofei   XN00016577]]

BOOL     CTPEditListCtrl::DrawReport(CDC *pDC,int iItem,int iSubItem,CRect &rtSub)
{
	if(m_dwColEdit[iSubItem] == TP_COLOUM_PROGRESS)

	{	
		CRect rtText = rtSub;
		rtText.DeflateRect(2,2);
		UINT uState  = CListCtrl::GetItemState(iItem,LVIS_DROPHILITED  | LVIS_FOCUSED | LVIS_SELECTED);	
		if(uState & ( LVIS_SELECTED))
		{
#ifdef TP_NS_EDIT
			if(GetFocus() == this) pDC ->FillSolidRect(rtText ,TP_GetSysColor(COLOR_BASE_SELECTITEMA));
			else                   pDC ->FillSolidRect(rtText ,TP_GetSysColor(COLOR_BASE_SELECTITEM));			
#else
			pDC ->FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
#endif
		}
		else 
		{
#ifdef TP_NS_EDIT
			pDC ->FillSolidRect(rtText,TP_GetSysColor(m_iBackColorIndex));							
#else
			if( iSubItem%2==1)   pDC ->FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_FRAME));							
			else                      pDC ->FillSolidRect(rtText,TP_GetSysColor(COLOR_BASE_WINDOW));				
#endif
		}
		CString sText = 	GetItemText(iItem,iSubItem);
		int iPos = StrToInt(sText);
		rtText.right = rtText.left + rtText.Width() * iPos /10000;
		TP_StretchDIBRect(pDC,m_pThumbBmp ->GetByte(),m_pThumbBmp->Width(),m_pThumbBmp->Height(),5,5,rtText,TRUE,TRUE);
		if(iPos>=100)
		{	
			iPos = min(iPos,100000);
			sText.Format(_T("%.0f%%"),iPos/100.0);
			pDC->DrawText(sText,rtSub,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		}
		return TRUE;	
	}
	if(m_iItemSave     == iItem && m_iItemSubSave  == iSubItem)
	{
		if(m_pEditWnd)
		{	
			CRect rtClient,rtItemSubSave;
			GetClientRect(&rtClient);
			CListCtrl::GetSubItemRect(m_iItemSave,m_iItemSubSave,LVIR_BOUNDS ,rtItemSubSave);
			if(rtItemSubSave != m_rtItemSubSave)
			{			
				if(rtClient.PtInRect(m_rtItemSubSave.TopLeft()) || rtClient.PtInRect(m_rtItemSubSave.BottomRight()))
				{
					m_pEditWnd ->MoveWindow(m_rtItemSubSave);
					return TRUE;
				}
				else EndEdit();
			}			
		}	
	}
	return FALSE;
}

void CTPEditListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	TP_RemoveMessage(WM_EDITLISTCTRLBEGIN);
	PostMessage(WM_EDITLISTCTRLBEGIN, pNMListView ->iItem,pNMListView ->iSubItem);
	*pResult = 0;
}

void CTPEditListCtrl::EndEdit()
{
	if(m_uBaseState & TP_BASESTATE_ENDEDIT) return;
	m_uBaseState |= TP_BASESTATE_ENDEDIT;
	if(m_pEditWnd)
	{
		FunNotifyParent(LVN_ENDLABELEDIT,m_iItemLast,m_iItemSubLast);
		m_pMsgWnd->SendMessage(WM_EDITLISTCTRL,TP_ENDLABELEDIT ,LPARAM(m_hWnd));
	}
	if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_EDIT)
	{
		if(m_pEdit)
		{
			m_pEdit ->DestroyWindow();
			delete m_pEdit;
			m_pEdit = NULL;
		}		
	}
	else if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_EDITEX)
	{
		if(m_pEdit)
		{
			CTPEditEx *pEdit = (CTPEditEx *)m_pEdit;
			pEdit ->DestroyWindow();
			delete pEdit;
			m_pEdit = NULL;
		}	
	}
	else if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_EDITPATH)
	{
		if(m_pEdit)
		{
			CTPEditEx *pEdit = (CTPEditEx *)m_pEdit;
			pEdit ->DestroyWindow();
			delete pEdit;
			m_pEdit = NULL;
		}	
	}
	else if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_TIMECODEEDIT)
	{
		m_pTimeCodeEdit ->DestroyWindow();
		delete m_pTimeCodeEdit;
		m_pTimeCodeEdit = NULL;
	}
	else if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_CUSTOM)
	{		
		m_pMsgWnd->SendMessage(WM_EDITLISTCTRL,TP_DESTORYCUSTOMWND,LPARAM(m_pCustomWnd));
		m_pCustomWnd = NULL;		
	}		
	InvalidateRect(m_rtItemSubSave);
	m_iItemSave     = -1;
	m_iItemSubSave  = -1;
	m_rtItemSubSave = CRect(0,0,0,0);
	m_pEditWnd      = NULL;
	m_uBaseState &= ~TP_BASESTATE_ENDEDIT;
}
void CTPEditListCtrl::BeginEdit()
{	
	if(!m_bCanEdit) return;
	if(m_iItemLast <0 || m_iItemLast >= GetItemCount() || m_iItemSubLast <0) return;
	FunNotifyParent(LVN_BEGINLABELEDIT,m_iItemLast,m_iItemSubLast);
	{
		m_iItemSave    = m_iItemLast;
		m_iItemSubSave = m_iItemSubLast;
		CListCtrl::GetSubItemRect(m_iItemSave,m_iItemSubSave,LVIR_BOUNDS ,m_rtItemSubSave);
		//qushaojie 20100120 当取第0列的rect时，实际取得为整行的rect，需要单独处理,暂时这么改，具体处理再议
		if (m_iItemSubSave == 0)
		{
			CRect rtTemp(0,0,0,0);
			CListCtrl::GetSubItemRect(m_iItemSave,1,LVIR_BOUNDS ,rtTemp);
			m_rtItemSubSave.right = rtTemp.left; 
		}
		if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_EDIT)
		{
			if(m_pEdit == NULL)
			{
				m_pEdit = new CTPEdit();
				m_pEdit ->Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL,m_rtItemSubSave,this,10);
			}
			m_pEdit ->ShowWindow(SW_SHOW);
			m_pEdit ->MoveWindow(m_rtItemSubSave);
			m_pEdit ->SetFocus();
			m_pEditWnd = m_pEdit;
		}
		else if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_EDITEX)
		{
			if(m_pEdit == NULL)
			{
				CTPEditEx *pEdit = new CTPEditEx();
				pEdit ->Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL,m_rtItemSubSave,this,10);
				m_pEdit = pEdit;
			}
			m_pEdit ->ShowWindow(SW_SHOW);
			m_pEdit ->MoveWindow(m_rtItemSubSave);
			m_pEdit ->SetFocus();
			m_pEditWnd = m_pEdit;
		}
		else if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_EDITPATH)
		{
			if(m_pEdit == NULL)
			{
				CTPEditEx *pEdit = new CTPEditEx();
				pEdit->SetExcludeKey(WC_NAMEEXCLUDE);
				CString sKeyNoteTip = TP_LoadStr(_T("IDS_TIMELINEWND_INVALID_CHAR"));
				sKeyNoteTip+= WC_NAMEEXCLUDE;
				pEdit ->SetToolTips(sKeyNoteTip);
				pEdit ->Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL,m_rtItemSubSave,this,10);
				m_pEdit = pEdit;
			}
			m_pEdit ->ShowWindow(SW_SHOW);
			m_pEdit ->MoveWindow(m_rtItemSubSave);
			m_pEdit ->SetFocus();
			m_pEditWnd = m_pEdit;
		}
		
		else if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_TIMECODEEDIT)
		{
			if(m_pTimeCodeEdit == NULL)
			{
				m_pTimeCodeEdit = new  CTPTimeCodeEditEx();
				m_pTimeCodeEdit ->Create(WS_CHILD|WS_VISIBLE,m_rtItemSubSave,this,10);
			}
			m_pTimeCodeEdit ->ShowWindow(SW_SHOW);
			m_pTimeCodeEdit ->MoveWindow(m_rtItemSubSave);
			m_pTimeCodeEdit ->SetFocus();
			m_pEditWnd = m_pTimeCodeEdit;
		}
		else if(m_dwColEdit[m_iItemSubSave] == TP_COLOUM_CUSTOM)
		{
			CWnd *pWnd = NULL;
			m_pMsgWnd->SendMessage(WM_EDITLISTCTRL,TP_GETCUSTOMWND,LPARAM(&pWnd));
			if(pWnd == NULL) return;
			m_pCustomWnd = pWnd;
			m_pCustomWnd ->ShowWindow(SW_SHOW);
			m_pCustomWnd ->MoveWindow(m_rtItemSubSave);
			m_pCustomWnd ->SetFocus();
			m_pEditWnd = m_pCustomWnd;
		}		
	}	
	if(m_pMsgWnd->SendMessage(WM_EDITLISTCTRL,TP_BEGINLABELEDIT,LPARAM(m_hWnd)) != S_FALSE)
	{
		m_pEditWnd = NULL;
		EndEdit();
	}
	m_iItemLast    = -1;
	m_iItemSubLast = -1;
}
void CTPEditListCtrl::OnDestroy()
{	
	EndEdit();
	if(m_pEdit)
	{
		m_pEdit ->DestroyWindow();
		delete m_pEdit;
		m_pEdit = NULL;
	}
	if(m_pTimeCodeEdit)
	{
		m_pTimeCodeEdit ->DestroyWindow();
		delete m_pTimeCodeEdit;
		m_pTimeCodeEdit = NULL;
	}
	CTPListCtrl::OnDestroy();
}

void CTPEditListCtrl::EditItem(int iItem,int iSubItem)
{
	if(iItem<0 || iItem>=GetItemCount()) return;
	m_iItemLast    = iItem;
	m_iItemSubLast = iSubItem;
	BeginEdit();
}
LRESULT CTPEditListCtrl::OnMessageListCtrl(WPARAM wp,LPARAM lp)
{
	if( m_iItemLast == (int)wp && m_iItemSubLast == (int)lp && m_dwColEdit[m_iItemSubLast] != 0)
		BeginEdit();
	else 
	{
		/*//zhaofei 2008-08-27
		if( m_pEditWnd)
			EndEdit();
		m_iItemLast    = (int)wp;
		m_iItemSubLast = (int)lp;
		if(m_dwColEdit[m_iItemSubLast] != 0)
			BeginEdit();*/

		if( m_pEditWnd)
			EndEdit();
		else if( m_pMsgWnd) //2007-05-29
			m_pMsgWnd ->PostMessage( WM_NM_CLICK, (int)wp, (int)lp);

		m_iItemLast    = (int)wp;
		m_iItemSubLast = (int)lp;
	}
	return S_OK;
}

LRESULT CTPEditListCtrl::OnMessageReName(WPARAM wp,LPARAM lp)
{
	if( m_iItemLast == (int)wp && m_iItemSubLast == (int)lp && m_dwColEdit[m_iItemSubLast] != 0)
		BeginEdit();
	else 
	{
		//zhaofei 2008-08-27
		if( m_pEditWnd)
		EndEdit();
		m_iItemLast    = (int)wp;
		m_iItemSubLast = (int)lp;
		if(m_dwColEdit[m_iItemSubLast] != 0)
		BeginEdit();
	}
	return S_OK;
}

void CTPEditListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CTPListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	if(m_pEditWnd)
	{	
		CRect rtClient;
		GetClientRect(&rtClient);
		CListCtrl::GetSubItemRect(m_iItemSave,m_iItemSubSave,LVIR_BOUNDS ,m_rtItemSubSave);
		if(rtClient.PtInRect(m_rtItemSubSave.TopLeft()) && rtClient.PtInRect(m_rtItemSubSave.BottomRight()))
			m_pEditWnd ->MoveWindow(m_rtItemSubSave);
		else EndEdit();
	}
}

void CTPEditListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CTPListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

	if(m_pEditWnd )
	{	
		CRect rtClient;
		GetClientRect(&rtClient);
		CListCtrl::GetSubItemRect(m_iItemSave,m_iItemSubSave,LVIR_BOUNDS ,m_rtItemSubSave);
		if(rtClient.PtInRect(m_rtItemSubSave.TopLeft())&&rtClient.PtInRect(m_rtItemSubSave.BottomRight()))
			m_pEditWnd ->MoveWindow(m_rtItemSubSave);
		else EndEdit();
	}
}
void  CTPEditListCtrl::OnEnKillFocus()
{
	PostMessage(WM_TIMECODE_CHANGE,1001,0);
	//EndEdit();
}
LRESULT	 CTPEditListCtrl::OnMessageTimeCode( WPARAM wp, LPARAM lp)
{
	if(wp == TP_TIMECODE_RETURN)             EndEdit();
	else if(wp == 1001)          EndEdit();

	return S_OK;
}
LRESULT CTPEditListCtrl::FunNotifyParent(UINT nNotify,int iItem,int iSubItem) 
{
	NMLISTVIEW nmList;
	nmList.hdr.code     =  nNotify;
	nmList.hdr.hwndFrom = m_hWnd;
	nmList.hdr.idFrom   = GetDlgCtrlID();
	nmList.iItem        = iItem;
	nmList.iSubItem    = iSubItem;
	nmList.lParam      = (LPARAM)m_pEditWnd; 
	
	nmList. uOldState   = 0;
	nmList. uChanged    = 0;
	GetCursorPos(&nmList. ptAction);
	ScreenToClient(&nmList. ptAction);

	return m_pMsgWnd ->SendMessage(WM_NOTIFY,nmList.hdr.idFrom,(LPARAM)&nmList);
}

//void CTPEditListCtrl::OnParentNotify(UINT message, LPARAM lParam)
//{
//	if(message == WM_LBUTTONDOWN && m_pEdit) return;
//	CTPListCtrl::OnParentNotify(message, lParam);
//}

/*
void CTPEditListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	int iColumnIndex = -1;
	int iRowIndex = -1;
	// Get the current column and row
	if (!HitTestEx(point, &iRowIndex, &iColumnIndex))
	{
		return;
	}

	PostMessage(WM_EDITLISTCTRLBEGIN, iRowIndex,iColumnIndex);

	CTPListCtrl::OnLButtonDblClk(nFlags, point);
}

bool CTPEditListCtrl::HitTestEx(CPoint&  obPoint, int* pRowIndex , int* pColumnIndex)
{
	if (!pRowIndex || !pColumnIndex)
	{
		return false;
	}
	// Get the row index
	*pRowIndex = HitTest(obPoint, NULL);

	if (pColumnIndex)
	{
		*pColumnIndex = 0;
	}
	//	ScreenToClient(&obPoint);

	// Make sure that the ListView is in LVS_REPORT
	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
	{
		return false;
	}

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	int iColumnCount = pHeader->GetItemCount();

	// Get bounding rect of item and check whether obPoint falls in it.
	CRect obCellRect;
	GetItemRect(*pRowIndex, &obCellRect, LVIR_BOUNDS);

	if (obCellRect.PtInRect(obPoint))
	{
		// Now find the column
		for (*pColumnIndex = 0; *pColumnIndex < iColumnCount; (*pColumnIndex)++)
		{
			int iColWidth = GetColumnWidth(*pColumnIndex);

			if (obPoint.x >= obCellRect.left && obPoint.x <= (obCellRect.left + iColWidth))
			{
				return true;
			}
			obCellRect.left += iColWidth;
		}
	}
	return false;
}
*/
BOOL CTPEditListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message && m_pEditWnd)
	{
		if(m_pEditWnd && VK_RETURN == pMsg->wParam)
		{
			EndEdit();
			return TRUE;
		}
	}	
	return CTPListCtrl::PreTranslateMessage(pMsg);
}

BOOL CTPEditListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL bReturn = __super::OnMouseWheel(nFlags,zDelta, pt);
	if(m_pEditWnd)
	{	
		CRect rtClient;
		GetClientRect(&rtClient);
		CListCtrl::GetSubItemRect(m_iItemSave,m_iItemSubSave,LVIR_BOUNDS ,m_rtItemSubSave);
		if(rtClient.PtInRect(m_rtItemSubSave.TopLeft()) && rtClient.PtInRect(m_rtItemSubSave.BottomRight()))
		{
			m_pEditWnd ->MoveWindow(m_rtItemSubSave);
			Invalidate();
		}
		else EndEdit();
	}
	return bReturn;
}
