// TPListCtrlEx.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPListPropertyCtrl.h"
#include ".\tplistpropertyctrl.h"


// CTPListPropertyCtrl

#define  TP_LISTCTRL_WIDTH 75
#define  TP_LISTCTRL_HIGH  60



IMPLEMENT_DYNAMIC(CTPListPropertyCtrl, CTPWnd)
CTPListPropertyCtrl::CTPListPropertyCtrl()
{
	m_pMsgWnd        = NULL;
	m_uPrivateStyle  = LIST_SIZECOLUMN |  LIST_SELMORE| LIST_DRAGITEM|LIST_SELECTITEM;
	m_bSubWindow     = FALSE; 
	m_bCreate        = FALSE;
	m_dwWindowLevel  = TPWNDLEVEL1;
	m_eBorderStyle   = TP_GetBorderStyle();
	m_uState             = 0;

	m_szItemReport       = CSize(100,20);		
		

	m_iTotalHeight       = 0;
	m_iTotalWidth        = 0;	
	m_iTopX              = 0;
	m_iTopY              = 0;	
	m_iBarWidth          = TPSCROLLBAR;
	m_iBackColor         = COLOR_BASE_WINDOW;
	
	m_bEditItem        = TRUE; 
	m_bReDraw          = TRUE;
	m_bCalcScroll      = FALSE;
	m_bDrag            = FALSE; 

	m_iItemSave        = -1;
	m_iItemSubSave     = -1;
	m_iItemLast        = -1;
	m_iItemSubLast     = -1;
	m_rtItemSubSave    = CRect(0,0,0,0);
	m_pEditWnd         = NULL;

	m_pLinePen[0]      = NULL;
	m_iColumn          = TP_LISTPROPERTY_COLUMN;

	m_iDragMode        = 0;	
	m_ptDrag           = CPoint(0,0);	
	m_ptSave           = CPoint(0,0);
	m_ptMiddle         = CPoint(0,0);
	m_iDragIndex       = -1;
	m_rtInvert         = CRect(0,0,0,0);
	m_pEdit            = NULL;
	m_pTimeCode        = NULL;
	m_pComboBox        = NULL;
	m_enVideoType      = VIDEO_25;
	m_pFont            = NULL; 

	for(INT l=0;l<TP_LISTPROPERTY_COLUMN;l++)
	{
		m_iColumnWidth[l] = m_szItemReport.cx;
		m_iColumnAlign[l] = DT_LEFT;
	}
	m_pScrollBar   = NULL;
	ZeroMemory(&m_sVScrollInfo,sizeof(SCROLLINFO));
	ZeroMemory(&m_sHScrollInfo,sizeof(SCROLLINFO));

	m_pLinePen[0] = TP_LoadPen(COLOR_BASE_TEXT,PS_SOLID,1);

	CString sSkinPath = TP_GetSkinPath();
#ifndef TP_NS_EDIT
	m_pCursor[0]      = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorDragNS.cur"));	
	m_pCursor[1]      = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorDragwe.cur"));	
#else
	m_pCursor[0]      = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorNS_ns.cur"));	
	m_pCursor[1]      = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorwe_ns.cur"));	
#endif

	m_bCalcEdit       = FALSE;
	m_bCanEdit        = TRUE; 
}

CTPListPropertyCtrl::~CTPListPropertyCtrl()
{
	SubWindow(FALSE);
	
	for(INT l=0;l<m_aItem.GetSize();l++)
		delete m_aItem[l];
	m_aItem.RemoveAll();
	for(INT l=0;l<1;l++)	TP_ReleaseCommonRes(m_pLinePen[l]);	
	for(INT l=0;l<2;l++)	TP_ReleaseCommonRes(m_pCursor[l]);	
	if(m_pFont){ m_pFont ->DeleteObject(); delete m_pFont; m_pFont = NULL;}
}


BEGIN_MESSAGE_MAP(CTPListPropertyCtrl, CTPWnd)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_STYLECHANGED()

	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCCALCSIZE()

	ON_MESSAGE(WM_USER+14321,OnMessageSize)
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()

	ON_MESSAGE(WM_TPEDIT,OnMsgEditChange)
	ON_MESSAGE(WM_TIMECODE_CHANGE,OnMsgtimeCodeChange)
	ON_EN_KILLFOCUS(10,OnEnKillFocus)	
	ON_EN_KILLFOCUS(11,OnEnKillFocus)	
	ON_EN_SETFOCUS(10,OnEnSetFocus)	
	ON_EN_SETFOCUS(11,OnEnSetFocus)	
	ON_MESSAGE(WM_SCROLLBAR,OnMsgScrollBar)
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0, 0xffff, OnToolTipNotify)	//2006-03-14
END_MESSAGE_MAP()

void     CTPListPropertyCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_WND,&wndClass);		
		wndClass.lpszClassName = WC_LISTPROPERTY;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_LISTPROPERTY,AfxGetInstanceHandle());
}

void CTPListPropertyCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	
	
		ModifyStyle(0,WS_VSCROLL,0);
		SaveFont(m_hWnd);
		m_pScrollBar =  TP_ReplaceWnd(this);		
		if(m_pScrollBar)
		{
			m_pScrollBar ->iScrollHeight  = TPSCROLLBAR;
			m_pScrollBar ->iScrollWidth   = TPSCROLLBAR;
			m_pScrollBar ->m_pVScrollInfo = &m_sVScrollInfo;
			m_pScrollBar ->m_pHScrollInfo = &m_sHScrollInfo;
		}
		if(m_pMsgWnd ==NULL)	m_pMsgWnd = GetParent();

		SetClassLongPtr(m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)TP_GetCtrlCursor());				
	}
	else 
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;			
		RestoreFont(m_hWnd);
		TP_UnReplaceWnd(this);
		m_pScrollBar = NULL;	
	}
}
void CTPListPropertyCtrl::OnDestroy()
{	
	DeleteAllItems();
	if(m_pEdit)
	{
		m_pEdit ->DestroyWindow();
		delete m_pEdit;
		m_pEdit = NULL;
	}
	if(m_pTimeCode)
	{
		m_pTimeCode ->DestroyWindow();
		delete m_pTimeCode;
		m_pTimeCode = NULL;
	}
	if(m_pComboBox)
	{
		m_pComboBox ->DestroyWindow();
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	SubWindow(FALSE);
	CTPWnd::OnDestroy();
}
LRESULT CTPListPropertyCtrl::OnNcHitTest(CPoint point)
{
	return HTCLIENT;	
}
int CTPListPropertyCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
    if(m_pScrollBar) ((CTPScrollBar *)m_pScrollBar->pVScroll)->SetScrollLine(m_szItemReport.cy);//qushaojie 20091021 控制滚动条移动距离
	return 0;
}

BOOL CTPListPropertyCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CTPWnd::OnEraseBkgnd(pDC);
}

void CTPListPropertyCtrl::OnNcPaint()
{
		g_pRoundCorner ->m_iBackColor    = m_iBackColor;
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner ->PaintRect(m_eBorderStyle,this);

}
void CTPListPropertyCtrl::OnPaint()
{
	CPaintDC dc(this);
	if(m_bReDraw) PaintDC(&dc);
}
LRESULT CTPListPropertyCtrl::OnMessageSize(WPARAM wp,LPARAM lp)
{
	m_uState |= STATE_SIZE;
	m_iTopX 	= ((CTPScrollBar *)m_pScrollBar ->pHScroll) ->GetScrollPos();	
	m_iTopY 	= ((CTPScrollBar *)m_pScrollBar ->pVScroll) ->GetScrollPos();		
	ReLayOut();
	m_uState &= ~STATE_SIZE;
	return S_OK;
}
void CTPListPropertyCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTPWnd::OnSize(nType, cx, cy);
	GetClientRect(&m_rtClient);
	if(!(m_uState & STATE_SIZE) && cx>0 && cy>0) PostMessage(WM_USER+14321,0,0);
}
BOOL CTPListPropertyCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CTPWnd::PreCreateWindow(cs);
}

void CTPListPropertyCtrl::PreSubclassWindow()
{
	CTPWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}
void CTPListPropertyCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{	
	/*if(m_bCalcScroll) return;
	((CTPScrollBar *)m_pScrollBar ->pHScroll) ->SetScroll(nSBCode,nPos);
	int iTopX 	= ((CTPScrollBar *)m_pScrollBar ->pHScroll) ->GetScrollPos();
	if(iTopX != m_iTopX)
	{	
		m_iTopX = iTopX;
		FunCalcItemSize();
		FunCalcEditItem();
		Invalidate(FALSE);
	}
	m_uState &= ~STATE_TOPVIEW;*/
	m_iTopX = 0;
}

void CTPListPropertyCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(m_bCalcScroll) return;
	((CTPScrollBar *)m_pScrollBar ->pVScroll) ->SetScroll(nSBCode,nPos);
	int iTopY 	= ((CTPScrollBar *)m_pScrollBar ->pVScroll) ->GetScrollPos();
	if(iTopY != m_iTopY)
	{
		m_iTopY = iTopY;
		FunCalcItemSize();
		FunCalcEditItem();
		Invalidate(FALSE);		
	}
	m_uState &= ~STATE_TOPVIEW;
}
void CTPListPropertyCtrl::SelectItem(int iItem,int iSubItem)
{
	BOOL bPaint = FALSE;
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(l == iItem)  continue;
		if(m_aItem[l] ->uState & LVIS_SELECTED) 
		{
			m_aItem[l] ->uState  &=~LVIS_SELECTED;
			bPaint = TRUE;
			EndEdit();
		}
	}	
	if(!(m_aItem[iItem] ->uState & LVIS_SELECTED))
	{	
		m_aItem[iItem] ->uState  |= LVIS_SELECTED;
		FunNotifyParent(LVN_ITEMCHANGED,iItem,-1);
		m_iItemLast    = iItem;
		m_iItemSubLast = 1;
		BeginEdit();
		bPaint = TRUE;
	}
	else if(m_iItemSave != iItem) BeginEdit();
	if(bPaint) Invalidate();
}
int CTPListPropertyCtrl::GetSelectItem()
{
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l] ->uState & LVIS_SELECTED) return l;
	}
	return -1;
}
void CTPListPropertyCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int    iColumn = -1;int iItem = -1;	
	if(GetFocus() != this) SetFocus();
	CTPWnd::OnLButtonDown(nFlags,point);		
	if(HitTestDrag(point,iColumn,iItem)) //拖动
	{
		// XN00011984 LI HUI LI 2008-09-02
		EndEdit();
		//]]]
		if(iColumn>=0)
		{
			m_iDragMode  = TP_DRAG_LEFT;
			m_iDragIndex = iColumn;
			m_ptDrag     = point;
			m_ptMiddle   = point;
			m_rtInvert   = CRect(0,0,0,0);
			SetCapture();
		}
		else if(iItem>=0)
		{
			m_iDragMode  = TP_DRAG_TOP;
			m_iDragIndex = iItem;
			m_ptDrag     = point;
			m_ptMiddle   = point;
			m_rtInvert   = CRect(0,0,0,0);
			SetCapture();
		}
		return;
	}
	int iSubItem,iItemSel = HitTest(point,&iSubItem);
	if(iItemSel <0)
	{	
		BOOL bPaint = FALSE;
		for(INT l=0;l<m_aItem.GetSize();l++)
		{
			if(m_aItem[l] ->uState & LVIS_SELECTED) 
			{
				m_aItem[l] ->uState  &=~LVIS_SELECTED;
				bPaint = TRUE;
				EndEdit();
			}
		}	
		if(bPaint) Invalidate();
	}
	else
	{
		SelectItem(iItemSel,iSubItem);		
		FunNotifyParent(NM_CLICK,iItemSel,iSubItem);		
	}		
}

void CTPListPropertyCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_iDragMode  == TP_DRAG_LEFT || m_iDragMode  == TP_DRAG_LEFTOK)
	{
		ReleaseCapture();
		if(m_iDragMode  == TP_DRAG_LEFTOK)
		{	
			m_iDragMode = TP_DRAG_UNKNOW;
			point.x  = max(point.x,10);
			point.x  = min(point.x,m_rtClient.right-10);
			m_iColumnWidth[m_iDragIndex] += point.x - m_ptMiddle.x;
			m_iColumnWidth[m_iDragIndex]  = max(0,m_iColumnWidth[m_iDragIndex]);
			m_iColumnWidth[m_iDragIndex]  = min(m_iColumnWidth[m_iDragIndex] ,m_rtClient.Width()-10); 
			FunCalcSize();
			Invalidate();			
		}
	}
	else if(m_iDragMode  == TP_DRAG_TOP || m_iDragMode  == TP_DRAG_TOPOK)
	{
		ReleaseCapture();
		if(m_iDragMode  == TP_DRAG_TOPOK)
		{
			m_iDragMode = TP_DRAG_UNKNOW;
			point.y  = max(point.y,10);
			point.y  = min(point.y,m_rtClient.bottom-10);
			m_aItem[m_iDragIndex]->iHeight += point.y - m_ptMiddle.y;
			// modify by liting		2008-09-25   For:高度不应该最小为0，否则该行无法显示，设置成最小高度为10
			//m_aItem[m_iDragIndex]->iHeight  = max(0,m_aItem[m_iDragIndex]->iHeight);
			m_aItem[m_iDragIndex]->iHeight  = max(10,m_aItem[m_iDragIndex]->iHeight);
			// end modify liting
			FunCalcSize();
			Invalidate();
		}
	}	
	m_rtInvert   = CRect(0,0,0,0);
	m_iDragMode  = TP_DRAG_UNKNOW;
	m_iDragIndex = -1;
	 SetCursor(TP_GetCtrlCursor());
	CTPWnd::OnLButtonUp(nFlags,point); 
}


void CTPListPropertyCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_iDragMode  == TP_DRAG_LEFT || m_iDragMode  == TP_DRAG_LEFTOK)
	{
		if(m_ptDrag.x != point.x)
		{
			m_iDragMode  = TP_DRAG_LEFTOK;
			point.x  = max(point.x,10);
			point.x  = min(point.x,m_rtClient.right-10);
			CDC *pDC = GetDC();
			if(m_rtInvert.Width() >0)	pDC->InvertRect(m_rtInvert);      
			m_rtInvert = CRect(point.x,0,point.x +1,min(m_rtClient.bottom,m_iTotalHeight - m_iTopY));			
			pDC->InvertRect(m_rtInvert); 
			ReleaseDC(pDC);
		}
	}
	else if(m_iDragMode  == TP_DRAG_TOP || m_iDragMode  == TP_DRAG_TOPOK)
	{
		if(m_ptDrag.y != point.y)
		{
			m_iDragMode  = TP_DRAG_TOPOK;
			point.y  = max(point.y,10);
			point.y  = min(point.y,m_rtClient.bottom-10);
			CDC *pDC = GetDC();
			if(m_rtInvert.Width() >0)	pDC->InvertRect(m_rtInvert);      
			m_rtInvert = CRect(0,point.y,m_rtClient.right,point.y+1);			
			pDC->InvertRect(m_rtInvert); 
			ReleaseDC(pDC);
		}
	}
}
BOOL CTPListPropertyCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_pScrollBar ->pVScroll ->SendMessage(WM_MOUSEWHEEL,MAKELONG(nFlags,zDelta),MAKELONG(pt.x,pt.y));
	return 0;	
}
BOOL  CTPListPropertyCtrl::HitTestDrag(CPoint &point,int &iColumn,int &iItem)
{
	iColumn  = -1;
	iItem    = -1;
	int  iCx = -m_iTopX;
	if(point.y < m_iTotalHeight - m_iTopY)
	{	
		for(INT l=0;l<m_iColumn-1;l++)
		{
			iCx += m_iColumnWidth[l];
			if(point.x > iCx -2 && point.x < iCx +2 )
			{
				iColumn = l;
				return TRUE;
			}
		}
	}	
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(point.y > m_aItem[l]->aSubItem[0].rtItem.bottom - 2 && point.y < m_aItem[l]->aSubItem[0].rtItem.bottom + 2)
		{
			iItem = l;
			return TRUE;
		}
	}
	return FALSE;
	
}
int  CTPListPropertyCtrl::HitTest(CPoint &point,int *piSubItem)
{	
	for(int i=0;i<m_aItem.GetSize();i++)
	{
		for(int k=0;k<m_iColumn;k++)
		{
			if(m_aItem[i]->aSubItem[k].rtItem .PtInRect(point))
			{				
				if(piSubItem)   *piSubItem  = k;						
				return i;
			}
		}				
	}
	if(piSubItem) *piSubItem = -1;	
	return -1;
}

void CTPListPropertyCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(GetFocus() != this) SetFocus();	
	int iSubItem,iItemSel = HitTest(point,&iSubItem);
	if(iItemSel>=0) FunNotifyParent(NM_RDBLCLK,iItemSel,0);
	CTPWnd::OnRButtonDown(nFlags, point);
}
void CTPListPropertyCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTPWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
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
BOOL CTPListPropertyCtrl::OnSetCursor(CWnd* pWnd, UINT nFunHitTest, UINT message)
{
	if(m_iDragMode !=TP_DRAG_UNKNOW) return TRUE;
	int    iColumn = -1;int iItem = -1;
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if(HitTestDrag(point,iColumn,iItem))
	{
		if(iColumn>=0)     SetCursor(m_pCursor[1]->GetCursor());
		else if(iItem>=0)  SetCursor(m_pCursor[0]->GetCursor());
		return TRUE;
	}
	return CTPWnd::OnSetCursor(pWnd, nFunHitTest, message);
}

BOOL CTPListPropertyCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(WC_LISTCTRLEX,NULL,dwStyle,rect,pParentWnd,nID);
}
void CTPListPropertyCtrl::SetItemToolTip(int nItem,BOOL bEnable)
{
	if(bEnable)
	{
		if(nItem<0)
		{
			for(INT l=0;l<m_aItem.GetSize();l++)
				m_aItem[l]->uState |= STATE_TOOLTIP;
			if(!(m_uBaseState &TP_BASESTATE_ENABLETOOLTIP))
				EnableToolTips(TRUE);
			m_uBaseState |= TP_BASESTATE_ENABLETOOLTIP|TP_BASESTATE_TEMPLATE;
		}
	}
	else 
	{
		if(nItem<0)
		{
			for(INT l=0;l<m_aItem.GetSize();l++)
				m_aItem[l]->uState |= STATE_TOOLTIP;
			if(m_uBaseState &TP_BASESTATE_ENABLETOOLTIP)
				EnableToolTips(FALSE);
			m_uBaseState &= ~(TP_BASESTATE_ENABLETOOLTIP|TP_BASESTATE_TEMPLATE);
		}
	}
}
int CTPListPropertyCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	InsertItem(nItem,lpszItem,-1);
	return nItem;
}
CString CTPListPropertyCtrl::GetItemText(int nItem, int nSubItem) const
{
	if(nSubItem<0) nSubItem = 0;
	if(nItem<0 || nItem >= m_aItem.GetSize() || nSubItem>=m_iColumn) return FALSE;	
	return m_aItem[nItem] ->aSubItem[nSubItem] .sText;	
}
int CTPListPropertyCtrl::GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const
{
	if(nSubItem<0) nSubItem = 0;
	if(nItem<0 || nItem >= m_aItem.GetSize() || nSubItem>=m_iColumn) return 0;	
	CString sText = m_aItem[nItem] ->aSubItem[nSubItem] .sText;
	lstrcpyn(lpszText,sText,nLen);
	return sText.GetLength();
}
BOOL CTPListPropertyCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	if(nSubItem<0) nSubItem = 0;
	if(nItem<0 || nItem >= m_aItem.GetSize() || nSubItem>=m_iColumn) return FALSE;	
	m_aItem[nItem] ->aSubItem[nSubItem] .sText = lpszText;	
	if(m_bReDraw) Invalidate();
	return TRUE;
}
int CTPListPropertyCtrl::InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
{
	if(nItem<0 || nItem >= (int)m_aItem.GetSize()) nItem = (int)m_aItem.GetSize();	
	TPListPropertyItem *pItem = new TPListPropertyItem();	
	pItem ->SetItemText(0,lpszItem,DT_LEFT);
	pItem ->SetItemImage(0,nImage);
	pItem ->iHeight = m_szItemReport.cy;	
	m_aItem.InsertAt(nItem,pItem);	
	if(m_bReDraw)
	{
		FunCalcSize();
		Invalidate(FALSE);
	}	
	return nItem;
}
void CTPListPropertyCtrl::DrawReport(CDC *pDC,int iItem)
{
	TPListPropertyItem             *pItem     = m_aItem[iItem];
	TPListPropertyItem::tagSubItem *pSubItem  = NULL;
	CWnd                           *pFocus    = GetFocus();
	BOOL                            bFocus    = pFocus == this || pFocus == m_pEditWnd || pFocus == m_pScrollBar->pVScroll;
	int iColumnCount      = (int) m_iColumn;	
	if(iColumnCount<=0) return;

	CRect rtItem = pItem ->aSubItem[0].rtItem;	
	if(pItem ->uState & ( LVIS_FOCUSED |LVIS_SELECTED))
	{		
#ifndef  TP_NS_EDIT
		if(bFocus)   pDC ->FillSolidRect(CRect(m_rtClient.left,rtItem.top,m_rtClient.right,rtItem.bottom) ,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));		
		else         pDC ->FillSolidRect(CRect(m_rtClient.left,rtItem.top,m_rtClient.right,rtItem.bottom) ,TP_GetSysColor(m_iBackColor));		
#else
		if(bFocus)   pDC ->FillSolidRect(CRect(m_rtClient.left,rtItem.top,m_rtClient.right,rtItem.bottom) ,TP_GetSysColor(COLOR_BASE_SELECTITEMA));		
		else         pDC ->FillSolidRect(CRect(m_rtClient.left,rtItem.top,m_rtClient.right,rtItem.bottom) ,TP_GetSysColor(COLOR_BASE_SELECTITEM));		
#endif
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));				
	}
	else if(!IsWindowEnabled())
	{		
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));			
	}
	else
	{	
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));			
	}
	HPEN hPen = (HPEN)::SelectObject(pDC->m_hDC,m_pLinePen[0]->hPen);
	for(int l = iColumnCount - 1 ;l >=0;l --)
	{
		pSubItem = &pItem ->aSubItem[l];
		rtItem   =  pSubItem->rtItem;		
		if(l!=iColumnCount - 1){pDC ->MoveTo(rtItem.right-1,rtItem.top); pDC ->LineTo(rtItem.right-1,rtItem.bottom);}
		rtItem.DeflateRect(3,0);
		if(rtItem.left < rtItem.right)
		{
			UINT uFromat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS|DT_NOPREFIX|pSubItem ->iAlign;
			pDC ->DrawText(pSubItem ->sText,rtItem,uFromat);
			pSubItem ->rtLable = rtItem;
		}		
	}	
	pDC ->MoveTo(m_rtClient.left,rtItem.bottom-1); pDC ->LineTo(m_rtClient.right,rtItem.bottom-1);
	::SelectObject(pDC->m_hDC,hPen);
}
void CTPListPropertyCtrl::PaintDC(CDC *pDC)
{
	if(!m_bReDraw) return;
	CRect rtClient; 
	BOOL  bReleaseDC = FALSE;
	if(pDC == NULL) { pDC = GetDC(); bReleaseDC = TRUE; }
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));
	CFont *pSaveFont = NULL;
	if(m_pFont) pSaveFont = cdcMemo.SelectObject(m_pFont);
	else pSaveFont = cdcMemo.SelectObject(TP_GetSysFont(m_iFontIndex));

	if(m_iColumn>=0)
	{		
		for(INT l=0;l<m_aItem.GetSize();l++)
		{
			if(m_aItem[l]->aSubItem[0].rtItem.bottom <= 0) continue;
			else if(m_aItem[l]->aSubItem[0].rtItem.top >= rtClient.bottom) break;
			DrawReport(&cdcMemo,l);
		}
	}
	if(pSaveFont) cdcMemo.SelectObject(pSaveFont);

	pDC->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);

	cdcMemo.DeleteDC();
	if(bReleaseDC) ReleaseDC(pDC);
}

void   CTPListPropertyCtrl::ReLayOut()
{
	FunCalcSize();	
	InvalidateRect(NULL);
}
int CTPListPropertyCtrl::GetColumnMaxWidth(int nCol)
{
	int iWidth = m_szItemReport.cx;
	if(m_aItem.GetSize()>0)
	{
		CDC dcMemo;
		dcMemo.CreateCompatibleDC(NULL);
		CFont *pFont = dcMemo.SelectObject(m_pFont);
		CSize  szText;
		for(INT l=0;l<m_aItem.GetSize();l++)
		{
			szText = dcMemo.GetTextExtent(m_aItem[l]->aSubItem[nCol].sText);
			iWidth = max(iWidth,szText.cx+8);
		}
		dcMemo.SelectObject(pFont);
	}
	return iWidth;
}
BOOL CTPListPropertyCtrl::SetColumnWidth(int nCol, int cx,BOOL bPaint)
{
	if(nCol <0 || nCol >= m_iColumn) return 0;
	if(m_iColumnWidth[nCol] == cx) return TRUE;
	m_iColumnWidth[nCol] = cx;
	if(m_bReDraw && bPaint)
	{
		ReLayOut();
		PaintDC();
	}
	return TRUE;
}
void     CTPListPropertyCtrl::SetColumeEditType(int iIndex,int iCol,DWORD dEditType,long lMax)
{
	if(iCol<0 || iCol>=m_iColumn)             return;
	if(iIndex<0 || iIndex>=m_aItem.GetSize()) return;
	m_aItem[iIndex]->aSubItem[iCol].dwEditType = dEditType;
	m_aItem[iIndex]->aSubItem[iCol].lEditMax   = lMax;
}

TPListPropertyItem *     CTPListPropertyCtrl::GetItem(int iIndex)
{
	return m_aItem[iIndex];
}
void     CTPListPropertyCtrl::GetEditItem(int &iItem,int &iSubItem,CWnd *&pWnd)
{
	iItem    = m_iItemSave;
	iSubItem = m_iItemSubSave;
	pWnd     = m_pEditWnd;
}

void CTPListPropertyCtrl::FunCalcItemSize()
{
	if(!m_bSubWindow) return;
	CRect rtClient = m_rtClient;
	int   iItemSize    = (int)m_aItem.GetSize(),iCY = 0,iCx = -m_iTopX;
	int   iColumnSize  = m_iColumn;		
	CRect rtItem = CRect(0,0,0,0);
	for(int i=0;i<iItemSize;i++)
	{
		TPListPropertyItem *pItem =m_aItem[i]; 
		pItem ->iIndex = i;			
		rtItem = CRect(0,iCY - m_iTopY,0,iCY + pItem ->iHeight - m_iTopY);
		iCx    = -m_iTopX;
		for(INT k=0;k<iColumnSize;k++)
		{
			rtItem.left  = iCx;
			rtItem .right = iCx + m_iColumnWidth[k];
			if(k == iColumnSize -1) rtItem .right = max(rtItem .right,m_rtClient.right - m_iTopX);
			pItem ->aSubItem[k].rtItem = rtItem;
			pItem ->aSubItem[k].CalcSize();
			iCx += m_iColumnWidth[k];
		}		
		iCY += pItem ->iHeight;		
	}
	m_iTotalHeight = iCY + m_szItemReport.cy;	

}
CRect     CTPListPropertyCtrl::GetItemEditRect(int iItem,int iSubItem)
{
	if(iItem <0    || iItem >= m_aItem.GetSize()) return CRect(0,0,0,0);
	if(iSubItem <0 || iSubItem >= m_iColumn) return CRect(0,0,0,0);
	CRect rt = m_aItem[iItem] ->aSubItem[iSubItem].rtItem;	
	rt.bottom--;
	return rt;
}
void CTPListPropertyCtrl::FunCalcEditItem()
{
	if(m_pEditWnd)
	{	
		CRect rtItem;		
		rtItem = GetItemEditRect(m_iItemSave,m_iItemSubSave);rtItem.DeflateRect(1,1);
	//	if(m_rtClient.PtInRect(rtItem.TopLeft()) && m_rtClient.PtInRect(rtItem.BottomRight()))
		{
			// XN00019053 LI HUI LI 2009-03-03  时码控件不改变大小，Edit控件需要改变大小
			if(m_aItem[m_iItemSave] ->aSubItem[m_iItemSubSave].dwEditType == TP_COLOUM_TIMECODEEDIT)  rtItem.right = m_rtItemSubSave.right;
			else if(m_aItem[m_iItemSave] ->aSubItem[m_iItemSubSave].dwEditType == TP_COLOUM_EDIT)      rtItem.right = max(rtItem.left, m_rtClient.right); // XN00018995 LI HUI LI 如果是Edit最大长度不应该超出窗口范围
			if(m_rtItemSubSave != rtItem)		m_pEditWnd ->MoveWindow(rtItem);
			m_rtItemSubSave = rtItem;
		}
	//	else EndEdit();
	}
}
void CTPListPropertyCtrl::FunCalcScrollBar()
{
	if(m_bCalcScroll) return;
	m_bCalcScroll = TRUE;

	int iTopX = m_iTopX;
	int iTopY = m_iTopY;

	CRect rtClient;
	GetClientRect(&rtClient);
	SCROLLINFO scrInfoV,scrInfoH;
	ZeroMemory(&scrInfoV,sizeof(scrInfoV));
	ZeroMemory(&scrInfoH,sizeof(scrInfoH));

	scrInfoV.cbSize = sizeof(SCROLLINFO);
	scrInfoV.fMask  = SIF_RANGE | SIF_POS | SIF_PAGE;
	scrInfoV.nPage  = rtClient.Height();
	scrInfoH.cbSize = sizeof(SCROLLINFO);
	scrInfoH.fMask  = SIF_RANGE | SIF_POS | SIF_PAGE;
	scrInfoH.nPage  = rtClient.Width();

	m_iTotalWidth = 0;
	m_iTotalHeight = 0;
	if(m_iColumn>0)
	{
		for(int l=0;l<m_iColumn;l++)			m_iTotalWidth += m_iColumnWidth[l];
		for(int l=0; l<m_aItem.GetSize();l++)	m_iTotalHeight += m_aItem[l]->iHeight;
	}				

	//		if(m_iTotalWidth >rtClient.Width() && !m_pScrollBar ->GetShow(FALSE))  m_iTotalHeight  += m_iBarHeight;
	if(m_iTotalHeight>rtClient.Height()&& !m_pScrollBar ->GetShow(TRUE))   m_iTotalWidth   += m_iBarWidth;
	//		if(m_iTotalWidth >rtClient.Width() && !m_pScrollBar ->GetShow(FALSE))  m_iTotalHeight  += m_iBarHeight;


	scrInfoH.nMax   = m_iTotalWidth;
	scrInfoH.nMin   = 0;
	scrInfoH.nPos   = m_iTopX;

	scrInfoV.nMax   = m_iTotalHeight+ m_szItemReport.cy  *2;
	scrInfoV.nMin   = 0;
	scrInfoV.nPos   = m_iTopY;

	scrInfoV.nPos  = min(scrInfoV.nPos,scrInfoV.nMax-(int)scrInfoV.nPage+1);
	scrInfoH.nPos  = min(scrInfoH.nPos,scrInfoH.nMax-(int)scrInfoH.nPage+1);
	scrInfoV.nPos  = max(scrInfoV.nPos,0);
	scrInfoH.nPos  = max(scrInfoH.nPos,0);

	//	SetScrollInfo(SBS_HORZ,&scrInfoH,TRUE);
	SetScrollInfo(SBS_VERT,&scrInfoV,TRUE);

	//	m_iTopX = GetScrollPos(SBS_HORZ);
	m_iTopY = GetScrollPos(SBS_VERT);

	m_iTopX = 0;

	if(iTopX != m_iTopX || iTopY != m_iTopY) FunCalcItemSize();	
	m_bCalcScroll = FALSE;	
}
void CTPListPropertyCtrl::FunCalcSize()
{
	FunCalcItemSize();
	FunCalcScrollBar();
	FunCalcEditItem();
}
void CTPListPropertyCtrl::EndEdit()
{
	if(!m_bCanEdit) return;
	if(m_uBaseState & TP_BASESTATE_ENDEDIT) return;
	m_uBaseState |= TP_BASESTATE_ENDEDIT;
	LRESULT lResult = S_FALSE;
	if(m_pEditWnd == NULL)
	{	
		//m_iItemLast     = -1;
		//m_iItemSubLast  = -1;
		m_iItemSave     = -1;
		m_iItemSubSave  = -1;
		m_rtItemSubSave = CRect(0,0,0,0);		
		m_uBaseState &= ~TP_BASESTATE_ENDEDIT;
		return;		
	}
	else
	{
		lResult = FunNotifyParent(LVN_ENDLABELEDIT,m_iItemLast,m_iItemSubLast);
		m_pMsgWnd->SendMessage(WM_EDITLISTCTRL,TP_ENDLABELEDIT ,LPARAM(this));
	}
	if(m_aItem[m_iItemSave] ->aSubItem[m_iItemSubSave].dwEditType == TP_COLOUM_EDIT)
	{		
		if(lResult == S_OK && !m_bCalcEdit) 
		{
			CString sText;
			m_pEdit ->GetWindowText(sText);
			SetItemText(m_iItemSave,m_iItemSubSave,sText);
		}
		if(m_pEdit)
		{		
			m_pEdit ->DestroyWindow();
			delete m_pEdit;
			m_pEdit      = NULL;
			m_sEditLabel = _T("");
		}		
	}
	else if(m_aItem[m_iItemSave] ->aSubItem[m_iItemSubSave].dwEditType == TP_COLOUM_TIMECODEEDIT)
	{
		if(m_pTimeCode)
		{		
			m_pTimeCode ->DestroyWindow();
			delete m_pTimeCode;
			m_pTimeCode = NULL;
		}		
	}
	else if(m_aItem[m_iItemSave] ->aSubItem[m_iItemSubSave].dwEditType == TP_COLOUM_COMBOBOX)
	{
		if(m_pComboBox)
		{		
			m_pComboBox ->DestroyWindow();
			delete m_pComboBox;
			m_pComboBox = NULL;
		}		
	}
	InvalidateRect(m_rtItemSubSave);
	m_iItemSave     = -1;
	m_iItemSubSave  = -1;
	m_rtItemSubSave = CRect(0,0,0,0);
	m_pEditWnd      = NULL;
	m_uBaseState &= ~TP_BASESTATE_ENDEDIT;
}
void CTPListPropertyCtrl::BeginEdit()
{	
	if(!m_bCanEdit) return;
	m_bCalcEdit = FALSE;
	if(m_iItemLast <0||m_iItemSubLast<0) return;
	if(m_aItem[m_iItemLast] ->aSubItem[m_iItemSubLast].dwEditType == TP_COLOUM_NULL) return;
	if(FunNotifyParent(LVN_BEGINLABELEDIT,m_iItemLast,m_iItemSubLast) == S_OK)
	{
		m_iItemSave    = m_iItemLast;
		m_iItemSubSave = m_iItemSubLast;
		m_rtItemSubSave = GetItemEditRect(m_iItemSave,m_iItemSubSave);		
		if(m_aItem[m_iItemSave] ->aSubItem[m_iItemSubSave].dwEditType == TP_COLOUM_EDIT)
		{
			if(m_pEdit == NULL)
			{
				m_pEdit = new CTPEditEx();
				m_pEdit ->m_bClientRect = FALSE;
				m_pEdit ->Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_WANTRETURN,m_rtItemSubSave,this,10);
				if(m_aItem[m_iItemSave] ->aSubItem[m_iItemSubSave].lEditMax>0)	m_pEdit ->SetLimitText(m_aItem[m_iItemSave] ->aSubItem[m_iItemSubSave].lEditMax);
			}	
			CString sText = GetItemText(m_iItemSave,m_iItemSubSave);
			m_pEdit ->SetWindowText(sText);
			m_sEditLabel = sText;
			m_pEdit ->SetSel(0,sText.GetLength());
			m_pEdit ->ShowWindow(SW_SHOW);
			m_sTextSave = sText;
			if(m_pFont != NULL)	m_pEdit ->SetFont(m_pFont);
			m_rtItemSubSave.right = max(m_rtClient.right, m_rtItemSubSave.left); // XN00018995 LI HUI LI 如果是Edit最大长度不应该超出窗口范围
			m_pEdit ->MoveWindow(m_rtItemSubSave);
			m_pEditWnd = m_pEdit;
			m_pEdit ->SetFocus();
		}		
		else if(m_aItem[m_iItemSave] ->aSubItem[m_iItemSubSave].dwEditType == TP_COLOUM_TIMECODEEDIT)
		{
			if(m_pTimeCode == NULL)
			{
#ifdef TP_NS_EDIT
				m_pTimeCode = new CTPTimeCodeEditNS();
#else
				m_pTimeCode = new CTPTimeCodeEditEx();	
#endif
				m_pTimeCode ->Create(WS_CHILD|WS_VISIBLE,m_rtItemSubSave,this,11);				
			}	
			CString sText = GetItemText(m_iItemSave,m_iItemSubSave);
			m_iFrameSave   = TP_GetFrameFromString(m_enVideoType,sText);
			m_pTimeCode ->SetFrame(m_iFrameSave);			
			m_pTimeCode ->ShowWindow(SW_SHOW);
			m_rtItemSubSave.right = m_rtItemSubSave.left + 120;
			m_pTimeCode ->MoveWindow(m_rtItemSubSave);
			m_pEditWnd = m_pTimeCode;
			m_pTimeCode ->SetFocus();
		}		
	}		
	m_pMsgWnd->SendMessage(WM_EDITLISTCTRL,TP_BEGINLABELEDIT,LPARAM(this));

//	m_iItemLast    = -1;
//	m_iItemSubLast = -1;
}
LRESULT CTPListPropertyCtrl::FunNotifyParent(UINT nNotify,int iItem,int iSubItem) 
{
	NMLISTVIEW nmList;
	nmList.hdr.code =  nNotify;
	nmList.hdr.hwndFrom = m_hWnd;
	nmList.hdr.idFrom   = GetDlgCtrlID();
	nmList.iItem        = iItem;
	nmList. iSubItem    = iSubItem;
	CWnd *pWnd = m_pMsgWnd;
	if(nNotify >=  HDN_FIRST)   
	{
		if(iItem>=0 && iItem <	m_aItem.GetSize())
		{
			nmList. uNewState   = m_aItem[iItem]->uState;
			nmList. lParam      = m_aItem[iItem]->dwData;
		}
		else
		{
			nmList. uNewState   = 0;
			nmList. lParam      = 0;
		}		
	}	
	nmList. uOldState   = 0;
	nmList. uChanged    = 0;
	GetCursorPos(&nmList. ptAction);
	ScreenToClient(&nmList. ptAction);
	return pWnd ->SendMessage(WM_NOTIFY,nmList.hdr.idFrom,(LPARAM)&nmList);		
}
int CTPListPropertyCtrl::GetItemCount() const
{
	return (int)m_aItem.GetCount();
}

int CTPListPropertyCtrl::GetTopIndex() const
{
	if(m_iColumn <=0 || m_aItem.GetSize()<=0) return -1;
	for(int l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l]->aSubItem[0].rtItem.top>=0) return l;
	}
	return -1;
}
void  CTPListPropertyCtrl::SetTopIndex(int nItem) 
{
	CRect rtClient;
	GetClientRect(&rtClient);

	for(int  l=0;l<(int)m_aItem.GetSize();l++)
	{
		if(nItem == l)
		{
			CRect  rtItem;
			if(m_aItem.GetAt(l)->aSubItem[0].rtItem.bottom>rtClient.Height())
				m_iTopY += m_aItem.GetAt(l)->aSubItem[0].rtItem.bottom;
			else if(m_aItem.GetAt(l)->aSubItem[0].rtItem.top<0)
			{
				m_iTopY -= abs(m_aItem.GetAt(l)->aSubItem[0].rtItem.top);
				m_iTopY  = max(0,m_iTopY);
			}

			if(m_aItem.GetAt(l)->aSubItem[0].rtItem.right>rtClient.Width())
				m_iTopX += m_aItem.GetAt(l)->aSubItem[0].rtItem.left;
			break;
		}
	}

	m_iTopX  = 0;


	SCROLLINFO scrInfoV,scrInfoH;
	ZeroMemory(&scrInfoV,sizeof(scrInfoV));
	ZeroMemory(&scrInfoH,sizeof(scrInfoH));

	scrInfoV.cbSize = sizeof(SCROLLINFO);
	scrInfoV.fMask  = SIF_POS; 	
	scrInfoH.cbSize = sizeof(SCROLLINFO);
	scrInfoH.fMask  = SIF_POS;


//	SetScrollInfo(SBS_HORZ,&scrInfoH,TRUE);
	SetScrollInfo(SBS_VERT,&scrInfoV,TRUE);

	FunCalcSize();

}
BOOL CTPListPropertyCtrl::DeleteItem(int nItem)
{
	EndEdit();
	FunNotifyParent(LVN_DELETEITEM,nItem,0);
	if(nItem <0 || nItem >= m_aItem.GetSize()) return FALSE;
	delete m_aItem[nItem];
	m_aItem.RemoveAt(nItem);
	m_uState &= ~STATE_DRAGITEM;
	if(m_bReDraw)
	{
		FunCalcSize();		
		Invalidate();
	}
	return TRUE;
}
BOOL CTPListPropertyCtrl::DeleteAllItems()
{
	EndEdit();
	for(int n = (int)m_aItem.GetSize()-1;n>=0;n--)
	{
		FunNotifyParent(LVN_DELETEITEM,n,0);
		delete m_aItem[n];
		m_aItem.RemoveAt(n);
	}
	m_aItem.RemoveAll();
	m_uState &= ~STATE_DRAGITEM;
	m_iTopX = 0;
	m_iTopY = 0;
	if(m_bReDraw)
	{
		FunCalcSize();		
		InvalidateRect(NULL);
	}
	return FALSE;
}
BOOL CTPListPropertyCtrl::EnsureVisible(int nItem, BOOL bPartialOK)
{
	CRect rtClient;
	GetClientRect(&rtClient);

	int  iTopY = m_iTopY;
	for(int  l=0;l<(int)m_aItem.GetSize();l++)
	{
		if(nItem == l)
		{
			CRect  rtItem;
			if(m_aItem.GetAt(l)->aSubItem[0].rtItem.bottom>rtClient.Height())
				m_iTopY += (m_aItem.GetAt(l)->aSubItem[0].rtItem.bottom-rtClient.Height());
			else if(m_aItem.GetAt(l)->aSubItem[0].rtItem.top<0)
			{
				m_iTopY -= abs(m_aItem.GetAt(l)->aSubItem[0].rtItem.top);
				m_iTopY  = max(0,m_iTopY);			
			}
			if(m_aItem.GetAt(l)->aSubItem[0].rtItem.right>rtClient.Width())
				m_iTopX += m_aItem.GetAt(l)->aSubItem[0].rtItem.left;
			break;
		}
	}
	m_iTopX = 0;
	if(iTopY != m_iTopY) FunCalcSize();
	return TRUE;
}
DWORD_PTR CTPListPropertyCtrl::GetItemData(int nItem) const
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return NULL;
	return m_aItem[nItem] ->dwData;	
}
BOOL CTPListPropertyCtrl::SetItemData(int nItem, DWORD_PTR dwData)
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	m_aItem[nItem] ->dwData = dwData;
	return TRUE;
}
long CTPListPropertyCtrl::GetUserState(int nItem)
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return NULL;
	return m_aItem[nItem] ->lUserState;	
}
CWnd * CTPListPropertyCtrl::CreateEditWnd(DWORD dwEditType)
{
	if(TP_COLOUM_EDIT == dwEditType)
	{
		if(m_pEdit) return m_pEdit;
		m_pEdit = new CTPEditEx();
		m_pEdit ->Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,0);
		return m_pEdit;
	}
	else if(TP_COLOUM_TIMECODEEDIT == dwEditType)
	{
		if(m_pTimeCode) return m_pTimeCode;

#ifdef TP_NS_EDIT
		m_pTimeCode = new CTPTimeCodeEditNS();
#else
		m_pTimeCode = new CTPTimeCodeEditEx();
#endif
		m_pTimeCode ->Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,0);
		return m_pTimeCode;
	}
	else if(TP_COLOUM_COMBOBOX == dwEditType)
	{
		if(m_pComboBox) return m_pComboBox;
		m_pComboBox = new CTPComboBox();
		m_pComboBox ->Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,500),this,0);
		return m_pComboBox;
	}
	return NULL;
}
void   CTPListPropertyCtrl::DestoryEditWnd(CWnd *pWnd)
{
	if(m_pEdit == pWnd)
	{
		m_pEdit ->DestroyWindow();
		delete m_pEdit;
		m_pEdit = NULL;
	}
	if(m_pTimeCode == pWnd)
	{
		m_pTimeCode ->DestroyWindow();
		delete m_pTimeCode;
		m_pTimeCode = NULL;
	}
	if(m_pComboBox == pWnd)
	{
		m_pComboBox ->DestroyWindow();
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
}
void CTPListPropertyCtrl::SetUserState(int nItem, long dwData)
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return;
	m_aItem[nItem] ->lUserState = dwData;	
}
void  CTPListPropertyCtrl::SetRedraw(BOOL bRedraw)
{
	m_bReDraw = bRedraw;
	if(bRedraw) InvalidateRect(NULL);
	if(m_pScrollBar)
	{
		if(m_pScrollBar ->bShowVScrollBar) ((CTPScrollBar *) m_pScrollBar ->pVScroll) ->SetRedraw(bRedraw);
		if(m_pScrollBar ->bShowHScrollBar) ((CTPScrollBar *) m_pScrollBar ->pHScroll) ->SetRedraw(bRedraw);
		if(bRedraw)
		{		
			m_pScrollBar ->pVScroll ->Invalidate();
			m_pScrollBar ->pHScroll ->Invalidate();
		}
	}
}
BOOL CTPListPropertyCtrl::SortItems(PFNLVCOMPARE pfnCompare, DWORD_PTR dwData)
{
	int                  iItemCount = (int)m_aItem.GetSize();
	TPListPropertyItem  *pItem      = NULL;
	for(int l=0;l<iItemCount;l++)
	{
		for(INT k = l+1;k<iItemCount;k++)
		{
			if(pfnCompare)
			{
				if(pfnCompare(m_aItem[l]->dwData,m_aItem[k]->dwData,dwData)>0)
				{
					pItem      = m_aItem[l];
					m_aItem[l] = m_aItem[k];
					m_aItem[k] = pItem;
				}
			}
			else
			{
				if(lstrcmp(m_aItem[l]->aSubItem[0].sText, m_aItem[k]->aSubItem[0].sText) >=0)
				{
					pItem      = m_aItem[l];
					m_aItem[l] = m_aItem[k];
					m_aItem[k] = pItem;
				}
			}
		}
	}	
	m_uState &= ~STATE_DRAGITEM;
	FunCalcSize();
	InvalidateRect(FALSE);
	return TRUE;
}
void  CTPListPropertyCtrl::EditLable(int iItem,int iSubItem)
{
	m_iItemLast    = iItem;
	m_iItemSubLast = iSubItem;
	EnsureVisible(iItem,TRUE);
	InvalidateRect(NULL);
	UpdateWindow();
	BeginEdit();
}
BOOL CTPListPropertyCtrl::SetItemState(int nItem, UINT nState, UINT nMask)
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	TPListPropertyItem     *pItem    = m_aItem[nItem];
	pItem ->uState  = nState & nMask;	
	if(m_bReDraw) PaintDC();
	return TRUE;
}
UINT CTPListPropertyCtrl::GetItemState(int nItem, UINT nMask) const
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return 0;
	TPListPropertyItem     *pItem    = m_aItem[nItem];
	return pItem ->uState &  nMask;
}
void CTPListPropertyCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int iSel   = GetSelectItem();
	int iCount = (int)m_aItem.GetSize();	
	if(nChar == VK_UP)
	{
		if(iSel<0) iSel = iCount-1;
		iSel --;
		iSel = max(iSel,0);
		EnsureVisible(iSel,TRUE);
		SelectItem(iSel,1);
		if(GetFocus() != this && m_pEditWnd != GetFocus()) SetFocus();
	}
	else if(nChar == VK_DOWN)
	{
		if(iSel<0) iSel = 0;
		iSel ++;
		iSel = min(iSel,iCount-1);
		EnsureVisible(iSel,TRUE);
		SelectItem(iSel,1);
		if(GetFocus() != this && m_pEditWnd != GetFocus()) SetFocus();
	}
	else if(nChar == VK_RETURN)
	{
		if(m_pEditWnd==NULL)
		{
			m_iItemLast    = GetSelectItem();
			m_iItemSubLast = 1;
			if(m_iItemLast >=0) BeginEdit();
			return ;
		}
	}
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTPListPropertyCtrl::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);
	Invalidate();	// TODO: Add your message handler code here
}
void CTPListPropertyCtrl::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);
	Invalidate();
	CWnd *pFocus    = GetFocus();
	BOOL  bFocus    = pFocus == this || pFocus == m_pEditWnd || pFocus == (m_pScrollBar ? m_pScrollBar->pVScroll : NULL);
	if(!bFocus) EndEdit();
}
LRESULT CTPListPropertyCtrl::OnMsgEditChange(WPARAM wp,LPARAM lp)
{
	if(wp == TPEDIT_RETURE)
	{
		//if(!(m_uState & STATE_NORETURN)) EndEdit();
		//m_uState &= ~STATE_NORETURN;
	}
	return S_OK;
}
LRESULT CTPListPropertyCtrl::OnMsgtimeCodeChange(WPARAM wp,LPARAM lp)
{
	if(wp == TP_TIMECODE_RETURN) EndEdit();
	return S_OK;
}
void  CTPListPropertyCtrl::OnEnKillFocus()
{
	Invalidate();
	CWnd *pFocus    = GetFocus();
	BOOL  bFocus    = pFocus == this || pFocus == m_pEditWnd || pFocus == m_pScrollBar->pVScroll;
	if(!bFocus) EndEdit();
}
void   CTPListPropertyCtrl::OnEnSetFocus()
{
	Invalidate();
}
LRESULT   CTPListPropertyCtrl::OnMsgScrollBar(WPARAM wp,LPARAM lp)
{
	Invalidate();
	if(wp ==1)
	{	
		CWnd *pFocus    = GetFocus();
		BOOL  bFocus    = pFocus == this || pFocus == m_pEditWnd || pFocus == m_pScrollBar->pVScroll;
		if(!bFocus) EndEdit();
	}
	return S_OK;
}
void   CTPListPropertyCtrl::SetFont(CFont &pFont)
{
	if(m_pFont){m_pFont->DeleteObject();delete m_pFont;m_pFont =NULL;}
	m_pFont = new CFont;
	LOGFONT logFont;
	pFont.GetLogFont(&logFont);
	m_pFont->CreateFontIndirect(&logFont);	
	CDC   memDC;memDC.CreateCompatibleDC(NULL);
	CFont *pOldFont = memDC.SelectObject(m_pFont); // LI HUI LI 2007-11-05
	CSize szText = memDC.GetTextExtent(_T("Qq孙"));
	m_szItemReport.cy = szText.cy+6;
	if(pOldFont) memDC.SelectObject(pOldFont); // LI HUI LI 2007-11-05
	memDC.DeleteDC();
	FunCalcSize();

}
void CTPListPropertyCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int iColumn = -1,iItem = -1;
	if(HitTestDrag(point,iColumn,iItem))
	{
		// XN00011984 LI HUI LI 2008-09-02
		EndEdit();
		//]]]
		if(iColumn>=0)    
		{			
			m_iColumnWidth[iColumn] = GetColumnMaxWidth(iColumn);// m_szItemReport.cx;
			FunCalcSize();
			Invalidate();
		}
		else if(iItem>=0) 
		{
			m_aItem[iItem] ->iHeight = 20;
			FunCalcSize();
			Invalidate();
		}			
	}
}
BOOL CTPListPropertyCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYDOWN == pMsg->message && (NULL != m_pEdit))
	{
		CWnd *pFocus = GetFocus();
		TCHAR cWndName[MAX_PATH];
		if(pFocus)
		{
			GetClassName(pFocus->GetSafeHwnd(),cWndName,MAX_PATH);
			if(lstrcmp(cWndName,_T("Edit")) ==0)
			{
				if(VK_RETURN == pMsg->wParam)
				{
					EndEdit();
					return TRUE;
				}
				else if(VK_ESCAPE == pMsg->wParam)
				{
					m_bCalcEdit = TRUE;
					m_pEdit->SetWindowText(m_sEditLabel);
					EndEdit();
					return TRUE;
				}
			}
		}
	}	
	return __super::PreTranslateMessage(pMsg);
}
BOOL CTPListPropertyCtrl::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	if(m_uBaseState &TP_BASESTATE_ENABLETOOLTIP)
	{	
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		int iSubItem,iItemSel = HitTest(point,&iSubItem);
		if(iSubItem ==1 && iItemSel>=0 && iItemSel <m_aItem.GetSize() && ((m_aItem[iItemSel]->uState & STATE_TOOLTIP) || (m_uBaseState &TP_BASESTATE_TEMPLATE)))
		{            
			CString  sToolTips = m_aItem[iItemSel]->aSubItem[1].sText;
			lstrcpyn( pTTT->szText,sToolTips,80);	
		}
		return  TRUE;		
	}
	else return FALSE;	
}

INT_PTR CTPListPropertyCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
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
