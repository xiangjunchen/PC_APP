// TPComboBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPComboBox.h"
#include ".\tpcombobox.h"

// CTPComboBox

extern CWnd  *(*TP_ExtraSubControl)(CString sClassName,DWORD dwWindowLevel);

IMPLEMENT_DYNAMIC(CTPComboBox, CComboBox)
CTPComboBox::CTPComboBox()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_pComboBoxEx   = NULL;

	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_pListBox      = NULL;
	m_pEdit         = NULL;
	m_eBorderStyle  = g_eBorderStyle;
	m_sWndText      = _T("");
	m_iCurSel       = -1;
	m_iDownSel      = -1;

	m_eBackgroundColor = COLOR_BASE_FRAME;

	m_pButton[0]    = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ComboBox\\button_nor.bmp")); 	
	m_pButton[1]    = TP_LoadBmpByte(g_sSysSkinPath + _L("\\CommonCtrl\\ComboBox\\button_down.bmp")); 	

}

CTPComboBox::~CTPComboBox()
{
	SubWindow(FALSE);
	TP_ReleaseCommonRes(m_pButton[0]);
	TP_ReleaseCommonRes(m_pButton[1]);
}


BEGIN_MESSAGE_MAP(CTPComboBox, CComboBox)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_CLOSEUP,   OnCbnCloseup)
	ON_CONTROL_REFLECT(CBN_DROPDOWN,  OnCbnDropdown)
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff,       OnToolTipNotify )
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
ON_WM_CTLCOLOR_REFLECT()
//ON_WM_MOVE()
ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CTPComboBox::SubWindow(BOOL bSub)
{
	COMBOBOXINFO   comboBoxInfo;	
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		ModifyStyle(0,(CBS_OWNERDRAWFIXED | CBS_HASSTRINGS),TRUE);
				SaveFont(m_hWnd);
		comboBoxInfo.cbSize = sizeof(comboBoxInfo);
		CComboBox::GetComboBoxInfo(&comboBoxInfo);
		m_pListBox = new CTPComboLBox();	
		m_uStyle = GetStyle();

		if((m_uStyle & 0x0000003) == CBS_SIMPLE)
			m_pListBox -> m_bSimple =TRUE;

		m_pListBox ->m_pComboBoxEx = m_pComboBoxEx;		
		m_pListBox ->SubclassWindow(comboBoxInfo.hwndList);		
		SetClassLongPtr(m_pListBox->m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);
		m_pListBox ->SetFont(TP_GetSysFont(m_iFontIndex));
		if(comboBoxInfo.hwndItem && CWnd::FromHandlePermanent(comboBoxInfo.hwndItem) == NULL)
		{
			m_pEdit = new CTPEdit();
			m_pEdit ->SubclassWindow(comboBoxInfo.hwndItem);
			m_pEdit ->m_bClientRect = FALSE;
			SetClassLongPtr(m_pEdit->m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);
			m_pEdit ->SetFont(TP_GetSysFont(m_iFontIndex));
		} 			
	}
	else 
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;		
		EnableToolTips(FALSE);
		if(m_pListBox) 
		{
			if(m_pListBox ->m_hWnd)
			{
				m_pListBox ->EnableToolTips(FALSE);
				m_pListBox ->SendMessage(WM_UNSUBCLASSWINDOW,0,0);
				m_pListBox->UnsubclassWindow();
			}
			delete m_pListBox;
			m_pListBox =NULL;			
		}
		if(m_pEdit)
		{
			if(m_pEdit ->m_hWnd)
			{
				m_pEdit -> SendMessage(WM_UNSUBCLASSWINDOW,0,0);
				m_pEdit ->UnsubclassWindow();
			}
			delete m_pEdit;
			m_pEdit = NULL;
		}
		RestoreFont(m_hWnd);
	}
}
void CTPComboBox::SetEditEx(int iLength)
{
	HWND hWnd = NULL;
	if(m_pEdit)
	{
		if(m_pEdit ->m_hWnd)
		{
			hWnd = m_pEdit ->m_hWnd;
			m_pEdit -> SendMessage(WM_UNSUBCLASSWINDOW,0,0);
			m_pEdit ->UnsubclassWindow();
		}
		delete m_pEdit;
		m_pEdit = NULL;
	}
	if(hWnd)
	{	
		m_pEdit = (CTPEdit *)TP_ExtraSubControl(_T("TPEditEx"),TPWNDLEVEL1);	
		m_pEdit->m_uPrivateStyle |=	TPEDIT_PATHNAME;		
		m_pEdit ->SubclassWindow(hWnd);
		m_pEdit ->m_bClientRect = FALSE;
		SetClassLongPtr(m_pEdit->m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)g_hArrowCursor);
		m_pEdit ->SetFont(TP_GetSysFont(m_iFontIndex));
		if(iLength>0) m_pEdit ->SetLimitText(iLength);
	}	
}
BOOL CTPComboBox::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CComboBox::PreCreateWindow(cs);
}

void CTPComboBox::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();
	if(!m_bCreate)	SubWindow(TRUE);
}
void CTPComboBox::OnDestroy()
{
	SubWindow(FALSE);
	CComboBox::OnDestroy();
}

void CTPComboBox::EnableToolTips(BOOL bEnable)
{
	if(bEnable)
	{	
		m_uBaseState |= 	TP_BASESTATE_ENABLETOOLTIP;
		CComboBox::EnableToolTips(TRUE);	
	}
	else
	{
		m_uBaseState &= 	~TP_BASESTATE_ENABLETOOLTIP;
		CComboBox::EnableToolTips(FALSE);	
	}
	//if(m_pListBox) m_pListBox ->EnableToolTips(bEnable);

}
void CTPComboBox::SetToolTips(CString sText)
{
	m_sToolTips = sText;

	if(m_sToolTips.GetLength() >0)
	{
		EnableToolTips(TRUE);
	}
	else EnableToolTips(FALSE);
}
void CTPComboBox::SetLBText(int iItem,CString sText)
{
	if(iItem <0 || iItem >= m_pListBox ->GetCount()) return;
	int iSel = m_pListBox ->GetCurSel();
	SetRedraw(FALSE);
	m_pListBox ->DeleteString(iItem);
	m_pListBox ->InsertString(iItem,sText);
	if(iSel ==  iItem) m_pListBox ->SetCurSel(iItem);
	SetRedraw(TRUE);
	Invalidate();
}
CTPEdit *CTPComboBox::GetEdit()
{
	return m_pEdit;
}
CTPComboLBox *CTPComboBox::GetListBox()
{
	return m_pListBox;
}

void CTPComboBox::PaintButton(CDC *pDC,COMBOBOXINFO &comboBoxInfo)
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
void CTPComboBox::OnPaint()
{
	CPaintDC dc(this); 
	DRAWITEMSTRUCT stuDraw;
	stuDraw.hDC = dc.m_hDC;
	DrawItem(&stuDraw);
/*	if(m_pComboBoxEx)
	{	
		CRect     rtClient,rtTemp;
		GetClientRect(&rtClient);
		COMBOBOXINFO   comboBoxInfo;
		comboBoxInfo.cbSize = sizeof(comboBoxInfo);
		CComboBox::GetComboBoxInfo(&comboBoxInfo);
		CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());	
		cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner ->PaintRect(m_eBorderStyle,&cdcMemo,rtClient,2);	
		rtTemp = rtClient;
		rtTemp.DeflateRect(2,2);
		cdcMemo.FillSolidRect(rtTemp,TP_GetSysColor(COLOR_BASE_WINDOW));		
		PaintButton(&cdcMemo,comboBoxInfo);	
		rtTemp.right = comboBoxInfo.rcButton.left-1;
		DRAWITEMSTRUCT DrawItemStruct;
		DrawItemStruct.itemID = ((CTPComboBoxEx *)m_pComboBoxEx) ->GetCurSel();
		if(GetFocus() == this &&((m_uStyle & 0x00000003) == CBS_DROPDOWNLIST)) 
		{
			DrawItemStruct.itemAction = ODA_SELECT;
			DrawItemStruct.itemState  = ODS_SELECTED;
		}		
		DrawItemStruct.itemData = 2;
		DrawItemStruct.hDC      = cdcMemo.m_hDC;
		DrawItemStruct.rcItem   = rtTemp;
		((CTPComboBoxEx *)m_pComboBoxEx) ->PaintItem(&DrawItemStruct);
		dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
	}
	else
	{
		CRect     rtClient;
		GetClientRect(&rtClient);
		COMBOBOXINFO   comboBoxInfo;
		comboBoxInfo.cbSize = sizeof(comboBoxInfo);
		CComboBox::GetComboBoxInfo(&comboBoxInfo);
		if((m_uStyle & 0x00000003) == CBS_DROPDOWNLIST)
		{	
			CRect rtText =  rtClient;
			rtText.DeflateRect(3,3);
			rtText.right = comboBoxInfo.rcButton.left -1;
			CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());	
#ifdef TP_NS_EDIT		
			cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_EDITREDONLY));
#else
			cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_eBackgroundColor));
#endif
			g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
			g_pRoundCorner ->PaintRect(m_eBorderStyle,&cdcMemo,rtClient,2);	

			if(GetFocus() == this || m_bFocus)	
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
			else cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));	

#ifdef TP_NS_EDIT		
			cdcMemo.SetTextColor(RGB(0,0,0));
#endif

			CString sText;
			CComboBox::GetWindowText(sText);
			if(m_sWndText.GetLength()>0) sText = m_sWndText;
			cdcMemo.DrawText(sText,rtText,DT_LEFT | DT_SINGLELINE | DT_VCENTER|DT_NOPREFIX);		
			PaintButton(&cdcMemo,comboBoxInfo);
			dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	
		}
		else
		{
			if((m_uStyle & 0x0000003) == CBS_SIMPLE)
			{
				dc.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_FRAME));	
				rtClient.bottom = comboBoxInfo.rcItem.bottom + 3;
				dc.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));		
				g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
				g_pRoundCorner ->PaintRect(m_eBorderStyle,&dc,rtClient,2);	
			}
			else
			{				
				dc.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_FRAME));		
				g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
				g_pRoundCorner ->PaintRect(m_eBorderStyle,&dc,rtClient,2);	
				PaintButton(&dc,comboBoxInfo);	
			}			
	
		}
	}*/
}
void CTPComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if((m_uStyle & 0x0000003) == CBS_DROPDOWNLIST)
	{
		if(nChar == VK_DOWN || nChar == VK_UP)
		{
			SetRedraw(FALSE);			
			CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
			m_pListBox->InvalidateScrollBar();			
			SetRedraw(TRUE);
			Invalidate(FALSE);			
		}
	}
	else CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CTPComboBox::OnCbnCloseup()
{
	m_uBaseState &= ~TP_BASESTATE_NOACTWND;
	if((m_pListBox ->m_uState & LB_MOUSESELECT) == 0 && m_iDownSel!= m_pListBox->GetCurSel())
	{
		GetParent()->SendMessage(WM_COMMAND ,MAKELONG(GetDlgCtrlID(),CBN_SELENDOK),(LPARAM)m_hWnd);		
	}		
	m_pListBox ->m_uState &= ~LB_MOUSESELECT;
	GetParent()->SendMessage(WM_COMMBOX ,MAKELONG(GetDlgCtrlID(),CBN_CLOSEUP),(LPARAM)m_hWnd);		
}
void CTPComboBox::OnCbnDropdown()
{
	m_uBaseState |= TP_BASESTATE_NOACTWND;
	m_iDownSel    = CComboBox::GetCurSel();
	m_pListBox ->m_uState &= ~LB_MOUSESELECT; 
	GetParent()->SendMessage(WM_COMMBOX ,MAKELONG(GetDlgCtrlID(),CBN_DROPDOWN),(LPARAM)m_hWnd);	
	//m_pListBox ->PostMessage(WM_USER+11,0,0);
}
void CTPComboBox::OnCbnSelchange()
{
	if(m_pComboBoxEx)
		((CTPComboBoxEx *)m_pComboBoxEx) ->OnCbnSelchange();
	{	
		if(TP_BASESTATE_NOACTNOTIFY & m_uBaseState)
			m_pListBox ->SelectCheck(this);
		Invalidate();

		if(m_uBaseState & TP_BASESTATE_NOACTWND)
		{		
			m_pListBox ->Invalidate(FALSE);
			m_pListBox ->UpdateWindow();
		}
	}	
	if(m_pListBox) m_pListBox ->m_uBaseState |= TP_BASESTATE_SELCHANGE;

	GetParent()->SendMessage(WM_COMMBOX ,MAKELONG(GetDlgCtrlID(),CBN_SELCHANGE),(LPARAM)m_hWnd);		

}

void CTPComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(m_pComboBoxEx)
		((CTPComboBoxEx *)m_pComboBoxEx) ->PaintItem(lpDrawItemStruct);
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC); 
	if(m_pComboBoxEx)
	{
	
		CRect     rtClient,rtTemp;
		GetClientRect(&rtClient);
		COMBOBOXINFO   comboBoxInfo;
		comboBoxInfo.cbSize = sizeof(comboBoxInfo);
		CComboBox::GetComboBoxInfo(&comboBoxInfo);
		CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());	
		cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner ->PaintRect(m_eBorderStyle,&cdcMemo,rtClient,2);	
		rtTemp = rtClient;
		rtTemp.DeflateRect(2,2);
		cdcMemo.FillSolidRect(rtTemp,TP_GetSysColor(COLOR_BASE_WINDOW));		
		PaintButton(&cdcMemo,comboBoxInfo);	
		rtTemp.right = comboBoxInfo.rcButton.left-1;
		DRAWITEMSTRUCT DrawItemStruct;
		DrawItemStruct.itemID = ((CTPComboBoxEx *)m_pComboBoxEx) ->GetCurSel();
		if(GetFocus() == this &&((m_uStyle & 0x00000003) == CBS_DROPDOWNLIST)) 
		{
			DrawItemStruct.itemAction = ODA_SELECT;
			DrawItemStruct.itemState  = ODS_SELECTED;
		}		
		DrawItemStruct.itemData = 2;
		DrawItemStruct.hDC      = cdcMemo.m_hDC;
		DrawItemStruct.rcItem   = rtTemp;
		((CTPComboBoxEx *)m_pComboBoxEx) ->PaintItem(&DrawItemStruct);
		pDC->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
	}
	else
	{
		CRect     rtClient;
		GetClientRect(&rtClient);
		COMBOBOXINFO   comboBoxInfo;
		comboBoxInfo.cbSize = sizeof(comboBoxInfo);
		CComboBox::GetComboBoxInfo(&comboBoxInfo);
		if((m_uStyle & 0x00000003) == CBS_DROPDOWNLIST)
		{	
			CRect rtText =  rtClient;
			rtText.DeflateRect(3,3);
			rtText.right = comboBoxInfo.rcButton.left -1;
			CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());	
#ifdef TP_NS_EDIT		
			cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_EDITREDONLY));
#else
			cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
#endif
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
			else cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));	
#ifdef TP_NS_EDIT	
			if (IsWindowEnabled())
				cdcMemo.SetTextColor(RGB(0,0,0));
			else 
				cdcMemo.SetTextColor(RGB(128,128,128));
#endif

			CString sText;
			CComboBox::GetWindowText(sText);
			if(m_sWndText.GetLength()>0) sText = m_sWndText;
			if(m_pListBox && (m_pListBox->m_uState & LB_OWNERIMAGE) && (m_pListBox->m_uState & LB_SZIMAGE))
			{
				CImageList* t_ImgList = m_pListBox->GetImageList();
				if(t_ImgList)
				{
					int cx = 0,cy = 0;
					::ImageList_GetIconSize(*t_ImgList, &cx, &cy);
					int t_width = cx;
					if(cy <= rtText.Height())
					{
						CPoint t_ptLeftTop;
						t_ptLeftTop.x = rtText.left;
						t_ptLeftTop.y = rtText.top + (rtText.Height() - cy)/2; // 图片垂直方向居中
						t_ImgList->Draw(&cdcMemo,m_pListBox->GetImage(m_pListBox->GetCurSel()),	t_ptLeftTop,ILD_TRANSPARENT);
					}
					else // 如果图片大于绘制区域则进行定比缩放
					{
						t_width = int(rtText.Height() * cx / double(cy) );
						CTPMemoDC t_memDC(pDC,cx,cy);
						t_ImgList->Draw(&t_memDC,m_pListBox->GetImage(m_pListBox->GetCurSel()),	CPoint(0,0),ILD_TRANSPARENT);
						cdcMemo.StretchBlt(rtText.left,rtText.top,t_width,rtText.Height(),	&t_memDC, 0,0, cx,cy,SRCCOPY );
					}
					// 使得文字绘制右移
					rtText.left += t_width + 2; // 2 是下拉列表中的图片文字距离

					cdcMemo.DrawText(sText,rtText,DT_LEFT | DT_SINGLELINE | DT_VCENTER);		
					PaintButton(&cdcMemo,comboBoxInfo);
					pDC->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	
				}
			}
			else if(m_pListBox && (m_pListBox->m_uState & LB_OWNERIMAGE))
			{
				DRAWITEMSTRUCT DrawItemStruct;
				DrawItemStruct.hDC = cdcMemo.GetSafeHdc();
				DrawItemStruct.itemAction = 0x7890;
				DrawItemStruct.rcItem     = rtText;
				DrawItemStruct.itemID     = m_pListBox->GetCurSel();
				m_pListBox->DrawItem(&DrawItemStruct);
				PaintButton(&cdcMemo,comboBoxInfo);
				pDC->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	
			}
			else
			{

				cdcMemo.DrawText(sText,rtText,DT_LEFT | DT_SINGLELINE | DT_VCENTER);		
				PaintButton(&cdcMemo,comboBoxInfo);
				pDC->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	
			}
		}
		else
		{
			if((m_uStyle & 0x0000003) == CBS_SIMPLE)
			{
				pDC->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_FRAME));	
				rtClient.bottom = comboBoxInfo.rcItem.bottom + 3;
				pDC->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));		
				g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
				g_pRoundCorner ->PaintRect(m_eBorderStyle,pDC,rtClient,2);	
			}
			else
			{				
#ifdef TP_NS_EDIT
				if(GetFocus() == m_pEdit) pDC->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_EDITFOCUS));
				else  pDC->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_EDIT));
#else 
				pDC->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));		
#endif
				g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
				g_pRoundCorner ->PaintRect(m_eBorderStyle,pDC,rtClient,2);	
				PaintButton(pDC,comboBoxInfo);	
			}			
	
		}
	}
}

int CTPComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	SetFont(TP_GetSysFont(m_iFontIndex));

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CTPComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(m_pListBox) m_pListBox ->MeasureItem(lpMeasureItemStruct);
}

LRESULT CTPComboBox::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(CB_SETCURSEL == message || CB_RESETCONTENT == message)
	{
		SetRedraw(FALSE);
		LRESULT lResult = CComboBox::DefWindowProc(message, wParam, lParam);
		SetRedraw(TRUE);
		Invalidate(FALSE);
		m_iCurSel = (int)wParam;
		return lResult;
	}
	else if(CB_GETCURSEL == message)
	{		
		LRESULT lResult = CComboBox::DefWindowProc(message, wParam, lParam);
		m_iCurSel = (int)lResult;
		if(m_pListBox && (TP_BASESTATE_NOACTNOTIFY & m_uBaseState))
			m_iCurSel = m_pListBox ->SelectCheck(this,m_iCurSel);
		return m_iCurSel;
	}    
 

	else if(WM_MOUSEWHEEL == message && m_pListBox)
	{				
		short lMove = HIWORD(wParam);
		 m_pListBox ->m_uState &= ~(LB_MOUSEDOWN|LB_MOUSEUP);
		if(lMove>=0 && lMove <0xFF00) m_pListBox ->m_uState |= LB_MOUSEUP;
		else                          m_pListBox ->m_uState |= LB_MOUSEDOWN;
		return CComboBox::DefWindowProc(message, wParam, lParam);						
	}
	else if(WM_SETREDRAW == message)
	{
		return CComboBox::DefWindowProc(message, wParam, lParam);
	}
	else if(CB_SELECTSTRING == message)
	{
		SetRedraw(FALSE);
		LRESULT lResult   = CComboBox::DefWindowProc(CB_FINDSTRINGEXACT,wParam, lParam);
		unsigned int iSel = (unsigned int)lResult;
		lResult           = CComboBox::DefWindowProc(CB_SETCURSEL, iSel, NULL);
		SetRedraw(TRUE);
		Invalidate(FALSE);
		m_iCurSel = (int)iSel;
		return lResult;
	}
	LRESULT lResult =  CComboBox::DefWindowProc(message, wParam, lParam);
	if(m_pListBox) m_pListBox ->m_uState &= ~(LB_MOUSEDOWN|LB_MOUSEUP);
	return lResult;
}

BOOL CTPComboBox::OnEraseBkgnd(CDC* pDC)
{
	return CComboBox::OnEraseBkgnd(pDC);	
}

BOOL CTPComboBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg ->message == WM_KEYDOWN && pMsg ->wParam == VK_RETURN && GetDroppedState())
	{
		int iSel = m_pListBox?m_pListBox ->GetCurSel():-1;
		ShowDropDown(FALSE);
		if(m_pListBox && m_iCurSel != iSel)
		{			
			m_iCurSel = iSel;
			if(m_iCurSel>=0) 	SetCurSel(m_iCurSel);				
			GetParent()->SendMessage(WM_COMMAND ,MAKELONG(GetDlgCtrlID(),CBN_SELENDOK),(LPARAM)m_hWnd);				
		}
		return TRUE;
	}	
	else if(m_pListBox &&  (pMsg ->message == WM_KEYDOWN || pMsg ->message == WM_KEYUP) && (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN))
	{
		if(m_uBaseState & TP_BASESTATE_NOACTNOTIFY)
		{
			if(pMsg ->message == WM_KEYUP) return TRUE;		
			int iSel = GetCurSel(),iSetSel = iSel;
			if(pMsg->wParam == VK_UP && iSetSel>0)                                  iSetSel --;
			else if(pMsg->wParam == VK_DOWN && iSetSel <m_pListBox -> GetCount())   iSetSel ++;
			iSetSel =  m_pListBox -> SelectCheck(NULL,iSetSel);
			SetCurSel(iSetSel);			
			m_pListBox ->InvalidateRect(NULL);
			m_pListBox ->InvalidateScrollBar();		
			m_iCurSel = -1;
			return TRUE;
		}
		else
		{
			if(m_uBaseState & TP_BASESTATE_NOACTWND)
				m_pListBox ->InvalidateScrollBar();		
		}
	} 
	if(pMsg ->message == WM_COMMBOX)
	{
		int l =0;
		l++;

	}
	return CComboBox::PreTranslateMessage(pMsg);
}

TP_SYSTEM_COLOR CTPComboBox::SetBackgroundColor( TP_SYSTEM_COLOR eColor) //by zhh on 2006-06-09
{
	if( eColor == m_eBackgroundColor)
		return m_eBackgroundColor;

	TP_SYSTEM_COLOR eOldColor = m_eBackgroundColor;

	m_eBackgroundColor = eColor;
	Invalidate( FALSE);

	return eOldColor;
}


HBRUSH CTPComboBox::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
#ifdef  TP_NS_EDIT
	if(m_uStyle & ES_READONLY)   
	{		
		return TP_GetSysBrush(BRUSH_EDITREDONLY);	  
	}
	else if(GetFocus() == this)	
	{	
		return TP_GetSysBrush(BRUSH_EDITFOCUS);	  
	}
	else
	{	
		if((m_uStyle & 0x00000003) == CBS_DROPDOWNLIST)
			return TP_GetSysBrush(BRUSH_EDITREDONLY);	  
		else 
			return TP_GetSysBrush(BRUSH_EDIT);	  
	}
#endif
	return NULL;
}


void CTPComboBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if(!bShow) ShowDropDown(FALSE);
	__super::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}

INT_PTR CTPComboBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
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
BOOL CTPComboBox::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
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
void CTPComboBox::SetItemColor(int iItem,COLORREF clColor)
{
	if(m_pListBox)
		m_pListBox ->SetItemColor(iItem,clColor);
}
void CTPComboBox::EnableItem(int iItem,BOOL bEnable)
{
	m_uBaseState |= TP_BASESTATE_NOACTNOTIFY;
	if(m_pListBox)
		m_pListBox ->EnableItem(iItem,bEnable);
}

//added by chenxiaolong 2010.3.16 for Getting next enabled item.
int  CTPComboBox::GetEnableItem(int iItem, BOOL bUp)
{
	int iNextItem=CB_ERR;
	if(m_pListBox)
		iNextItem=m_pListBox->GetEnableItem(iItem,bUp);
	return iNextItem;
}
void  CTPComboBox::SortItem(BOOL (*f)(CTPComboBox::TPComboBoxItem *p1,CTPComboBox::TPComboBoxItem *p2))
{
	int iCurSel = GetCurSel();
	CArray<TPComboBoxItem *,TPComboBoxItem *&> aItem;
	for(INT l=0;l<GetCount();l++)
	{
		CTPComboBox::TPComboBoxItem *pItem = new CTPComboBox::TPComboBoxItem();
		GetLBText(l,pItem->sText);
		pItem->dwItemData = GetItemData(l);
		pItem->bSelect    = l == iCurSel;
		aItem.Add(pItem);
	}
	for(INT l=0;l<aItem.GetSize();l++)
		for(INT k=l+1;k<aItem.GetSize();k++)
		{
			if(f(aItem[l],aItem[k]))
			{
				TPComboBoxItem *pItem = aItem[l];
				aItem[l] = aItem[k];
				aItem[k] = pItem;
			}
		}

	ResetContent();
	for(INT l=0;l<aItem.GetSize();l++)
	{
		AddString(aItem[l]->sText);
		SetItemData(l,aItem[l]->dwItemData);
		if(aItem[l]->bSelect) SetCurSel(l);
		delete aItem[l];			
	}
	aItem.RemoveAll();

}

