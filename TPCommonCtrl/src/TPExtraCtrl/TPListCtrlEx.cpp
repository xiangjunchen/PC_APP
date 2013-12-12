// TPListCtrlEx.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPListCtrlEx.h"
#include ".\tplistctrlex.h"
#include <math.h>


// CTPListCtrlEx

#define  TP_LISTCTRL_WIDTH 75
#define  TP_LISTCTRL_HIGH  60
#define  TP_LSITEXTEND_HEIGHT		30
BOOL CTPListCtrlEx::m_bCreateLargeIcon = TRUE;
IMPLEMENT_DYNAMIC(CTPListCtrlEx, CTPWnd)
CTPListCtrlEx::CTPListCtrlEx()
{
	m_pMsgWnd        = NULL;
	m_uPrivateStyle  = LIST_SIZECOLUMN | LIST_DRAGCOLUMN | LIST_CLICKCOLUMN |LIST_SELMORE|LIST_HOTHEAD | LIST_DRAGITEM|LIST_SELECTITEM;
	m_bSubWindow     = FALSE; 
	m_bCreate        = FALSE;
	m_uStyle         = 0; 
	m_uStyleEx       =  LVS_EX_GRIDLINES;
	m_dwWindowLevel  = TPWNDLEVEL1;
	m_eBorderStyle   = TP_GetBorderStyle();
	m_uState         = 0;
	m_iCheckIndex    = -1;    


	m_iHotItem           = -1;
	m_bReDraw            = TRUE;
	m_iViewType          = VIEWTYPE_TEXTPICTURE;
	m_iArrType           = ARRANGE_FREE;
	m_lIconSize          = 4; 

	m_szItemList         = CSize(200,20);
	m_szItemIcon         = CSize(100,90);
	m_szItemReport       = CSize(20,20);	
	m_szItemTextPicture  = CSize(20,60);
	m_szItemInfo         = CSize(20,40);
	m_szItemBuffer       = CSize(TP_LISTCTRL_WIDTH + 6,TP_LISTCTRL_HIGH + 23);

	m_szEdge             = CSize(10,10);
	m_szSpace            = CSize(4,4);

	m_iTotalHeight       = 0;
	m_iTotalWidth        = 0;
	m_iBarHeight         = TPSCROLLBAR;
	m_iBarWidth          = TPSCROLLBAR;
	m_iColumnHeight      = 21;///::GetSystemMetrics(SM_CYHSCROLL);;
	m_iTopX              = 0;
	m_iTopY              = 0;	
	m_iLockColumn        = -1;
	m_iLockDragColumn    = -1;
	m_bLeftDrag          = FALSE;
	//m_iCountPerLine      = 0;
	m_szTextSize         = CSize(0,0);	
	m_iWidthCheck        = 0;


	for(INT l=0;l<5;l++)	m_pHeaderBmp[l] = NULL;
	for(INT l=0;l<2;l++)	m_pLinePen[l] = NULL;	
	for(INT l=0;l<4;l++)	m_hCursorDrag[l] = NULL;
	for(INT l=0;l<2;l++)	m_pImageRes[l] = NULL;	

	m_pCheckBox         = NULL;

	m_iHeadOver        = -1;
	m_iHeadDown        = -1;

	m_pScrollBar       = 0;

	m_iDragMode        = TP_DRAG_UNKNOW;
	m_rtInvert         = CRect(0,0,0,0);
	m_ptDrag           = CPoint(0,0);
	m_hTimer[0]        = 0;
	m_hTimer[1]        = 0;
	m_hTimer[2]        = 0;
	m_hTimer[3]        = 0;
	m_hTimer[4]        = 0;
	m_pDragImageList   = NULL;
	m_bDragInWnd       = FALSE;
	m_hDragCursor      = NULL;
	m_iDragIndex       = -1;
	m_bCalcScroll      = FALSE;
	m_bHeadMenu        = FALSE; 
	m_bEditItem        = TRUE; 


	m_iItemSave        = -1;
	m_iItemSubSave     = -1;
	m_iItemLast        = -1;
	m_iItemSubLast     = -1;
	m_rtItemSubSave    = CRect(0,0,0,0);
	m_pEditWnd         = NULL;

	m_pEdit            = NULL;
	m_pToolTipCtrl     = NULL;
	m_bDrawFocus       = FALSE; 
	m_pFocusWnd        = NULL;  
	m_pDragBitmap      = NULL; 


	ZeroMemory(&m_sVScrollInfo,sizeof(SCROLLINFO));
	ZeroMemory(&m_sHScrollInfo,sizeof(SCROLLINFO));
}

CTPListCtrlEx::~CTPListCtrlEx()
{
	SubWindow(FALSE);

	for(INT l=0;l<m_aColumn.GetSize();l++)
		delete m_aColumn[l];
	m_aColumn.RemoveAll();
	for(INT l=0;l<m_aItem.GetSize();l++)
		delete m_aItem[l];
	m_aItem.RemoveAll();
	m_aSelItem.RemoveAll();

	if(m_imgNormal.m_hImageList) m_imgNormal.DeleteImageList();
	if(m_imgSmall.m_hImageList)  m_imgSmall.DeleteImageList();
	if(m_imgState.m_hImageList)  m_imgState.DeleteImageList();
	if(m_imgHead.m_hImageList)   m_imgHead.DeleteImageList();
	if(m_hDragCursor)           ::DestroyIcon(m_hDragCursor);

	if(m_pDragBitmap)
	{
		m_pDragBitmap->DeleteObject();
		delete m_pDragBitmap;
		m_pDragBitmap = NULL;
	}


	for(INT l=0;l<5;l++)	TP_ReleaseCommonRes(m_pHeaderBmp[l]);
	for(INT l=0;l<2;l++)	TP_ReleaseCommonRes(m_pLinePen[l]);	
	for(INT l=0;l<4;l++)	TP_ReleaseCommonRes(m_hCursorDrag[l]);	
	for(INT l=0;l<2;l++)	TP_ReleaseCommonRes(m_pImageRes[l]);

	TP_ReleaseCommonRes(m_pCheckBox);


}


BEGIN_MESSAGE_MAP(CTPListCtrlEx, CTPWnd)
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
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_STYLECHANGED()

	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCCALCSIZE()

	ON_EN_KILLFOCUS(10, OnEnKillfocus)


	ON_MESSAGE(WM_USER+14321,OnMessageSize)
	ON_COMMAND_RANGE(ID_LISTCTRL_DELETECOLUMN,ID_LISTCTRL_MOVETOLEFTCOLUMN,OnMenuCmd)
	ON_MESSAGE(WM_TIMECODE_CHANGE,OnMessageTimeCode)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void     CTPListCtrlEx::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_WND,&wndClass);		
		wndClass.lpszClassName = WC_LISTCTRLEX;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_LISTCTRLEX,AfxGetInstanceHandle());
}
void CTPListCtrlEx::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;	

		ModifyStyle(0,WS_HSCROLL|WS_VSCROLL,0);

		SaveFont(m_hWnd);
		m_pScrollBar =  TP_ReplaceWnd(this);		
		if(m_pScrollBar)
		{
			m_pScrollBar ->iScrollHeight  = TPSCROLLBAR;
			m_pScrollBar ->iScrollWidth   = TPSCROLLBAR;
			m_pScrollBar ->m_pVScrollInfo = &m_sVScrollInfo;
			m_pScrollBar ->m_pHScrollInfo = &m_sHScrollInfo;
		}

		LoadRes();

		m_uStyle   = CTPWnd::GetStyle();
		if(m_pMsgWnd ==NULL)	m_pMsgWnd = GetParent();

		SetClassLongPtr(m_hWnd ,GCL_HCURSOR,(LONG)(LONG_PTR)TP_GetCtrlCursor());		

		m_pToolTipCtrl = new CTPToolTipCtrl();
		m_pToolTipCtrl->Create(this,TTS_ALWAYSTIP); 
		m_pToolTipCtrl->EnableToolTips(TRUE); 
		m_pToolTipCtrl->Activate(TRUE); 

		m_pToolTipCtrl ->SetDelayTime(TTDT_INITIAL,500); 
		m_pToolTipCtrl ->SetDelayTime(TTDT_RESHOW,500); 

		EnableToolTips(TRUE); 	
		for(INT l=0;l<100;l++) m_pToolTipCtrl->AddTool(this,LPSTR_TEXTCALLBACK,CRect(0,0,10,10),l+1);
	}
	else 
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;			
		RestoreFont(m_hWnd);
		TP_UnReplaceWnd(this);
		m_pScrollBar = NULL;

		for(INT l=0;l<4;l++)
		{
			if(m_hTimer[l]) KillTimer(m_hTimer[l]);
			m_hTimer[l] = 0;
		}		

		if(m_pToolTipCtrl)
		{
			if(m_pToolTipCtrl ->m_hWnd)
				m_pToolTipCtrl ->DestroyToolTipCtrl();
			else 
				delete m_pToolTipCtrl;
			m_pToolTipCtrl = NULL;
		}
	}
}
void CTPListCtrlEx::LoadRes()
{
	CString sSkinPath = TP_GetSkinPath();


	m_pHeaderBmp[0]  = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\ListCtrl\\List21\\HeaderCtrl_Dis.bmp"),FALSE,FALSE); 
	m_pHeaderBmp[1]  = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\ListCtrl\\List21\\HeaderCtrl_Over.bmp"),FALSE,FALSE);
	m_pHeaderBmp[2]  = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\ListCtrl\\List21\\HeaderCtrl_Nor.bmp"),FALSE,FALSE);
	m_pHeaderBmp[3]  = TP_LoadBmpByte(sSkinPath + _L("\\CommonCtrl\\ListCtrl\\List21\\HeaderCtrl_Down.bmp"),FALSE,FALSE);
	m_pHeaderBmp[4]  = TP_LoadImage(sSkinPath + _L("\\CommonCtrl\\ListCtrl\\HeaderCtrl_Sort.bmp"),12,RGB(0,0,0),FALSE,FALSE);

	m_pCheckBox      = TP_LoadImage(sSkinPath + _L("\\CommonCtrl\\TreeCtrl\\TreeCtrlCheck.bmp"),9,RGB(95,95,95),FALSE,FALSE);

	m_hCursorDrag[0] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor1\\CursorArrow.cur"),FALSE,FALSE);
	m_hCursorDrag[1] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor1\\CursorHandle.cur"),FALSE,FALSE);
	m_hCursorDrag[2] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor1\\CursorWe.Cur"),FALSE,FALSE);
	m_hCursorDrag[3] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor1\\CursorMove.Cur"),FALSE,FALSE);

	m_pImageRes[0]   = TP_LoadImage(sSkinPath + _L("\\CommonCtrl\\ListCtrl\\ListIconEdgeSelect.bmp"),5,RGB(14,14,14),FALSE,FALSE);
	m_pImageRes[1]   = TP_LoadImage(sSkinPath + _L("\\CommonCtrl\\ListCtrl\\ListIconEdgeUnSelect.bmp"),5,RGB(14,14,14),FALSE,FALSE);


	if(m_dwWindowLevel == TPWNDLEVEL1) 
	{
		m_iBackColor      = COLOR_BASE_WINDOW;
		m_pLinePen[0]     = TP_LoadPen(COLOR_BASE_3DLEFT,PS_SOLID,1);
		m_pLinePen[1]     = TP_LoadPen(COLOR_BASE_3DRIGHT,PS_SOLID,1);
	}
	else
	{
		m_iBackColor      = COLOR_BASE_WINDOW2;
		m_pLinePen[0]     = TP_LoadPen(COLOR_BASE_3DLEFT2,PS_SOLID,1);
		m_pLinePen[1]     = TP_LoadPen(COLOR_BASE_3DRIGHT2,PS_SOLID,1);
	}
}


void CTPListCtrlEx::OnDestroy()
{	
	DeleteAllItems();
	if(m_pEdit) 
	{
		m_pEdit ->DestroyWindow();
		delete m_pEdit;
		m_pEdit = NULL;
	}
	SubWindow(FALSE);
	CTPWnd::OnDestroy();
}
LRESULT CTPListCtrlEx::OnNcHitTest(CPoint point)
{
	return HTCLIENT;	
}
int CTPListCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}

BOOL CTPListCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CTPWnd::OnEraseBkgnd(pDC);
}

void CTPListCtrlEx::OnNcPaint()
{
#ifdef  TP_NS_EDIT
	if(m_bDrawFocus  && (GetFocus() == this || m_pFocusWnd && *m_pFocusWnd == this))
	{	
		CDC   *pDC   = GetWindowDC();		
		CTPDCExchange stuTemp(pDC);
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
		pDC->Draw3dRect(rtWnd,RGB(255,255,255),RGB(255,255,255));
		stuTemp.ReleaseDC();
		ReleaseDC(pDC);
	}
	else
#endif
	{	
		g_pRoundCorner ->m_iBackColor    = m_iBackColor;
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
	}
}
void CTPListCtrlEx::OnPaint()
{
	CPaintDC dc(this);
	if(m_bReDraw) PaintDC(&dc);
}
LRESULT CTPListCtrlEx::OnMessageTimeCode(WPARAM wp,LPARAM lp)
{
	if(TP_TIMECODE_RETURN == wp)
	{
		if(m_pEditWnd && m_aColumn[m_iItemSubSave] ->dEditType == TP_COLOUM_EDIT)  EndEdit();
	}
	return S_OK;
}
LRESULT CTPListCtrlEx::OnMessageSize(WPARAM wp,LPARAM lp)
{
	m_uState |= STATE_SIZE;
	m_iTopX 	= ((CTPScrollBar *)m_pScrollBar ->pHScroll) ->GetScrollPos();	
	m_iTopY 	= ((CTPScrollBar *)m_pScrollBar ->pVScroll) ->GetScrollPos();	
	ReLayOut();
	m_uState &= ~STATE_SIZE;
	return S_OK;
}
void CTPListCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CTPWnd::OnSize(nType, cx, cy);
	GetClientRect(&m_rtClient);	
	if(!(m_uState & STATE_SIZE) && cx>0 && cy>0) PostMessage(WM_USER+14321,0,0);
}
BOOL CTPListCtrlEx::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CTPWnd::PreCreateWindow(cs);
}

void CTPListCtrlEx::PreSubclassWindow()
{
	CTPWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}





BOOL CTPListCtrlEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(WC_LISTCTRLEX,NULL,dwStyle,rect,pParentWnd,nID);
}
DWORD CTPListCtrlEx::SetExtendedStyle(DWORD dwNewStyle)
{
	m_uStyleEx = dwNewStyle;
	return m_uStyleEx;
}

CImageList* CTPListCtrlEx::GetImageList(int nImageList) const
{
	if(LVSIL_NORMAL ==  nImageList)      return  (CImageList*)&m_imgNormal;
	else if(LVSIL_SMALL  ==  nImageList) return  (CImageList*)&m_imgSmall;
	else if(LVSIL_STATE  ==  nImageList) return  (CImageList*)&m_imgState;
	else return NULL;
}
CImageList* CTPListCtrlEx::SetImageList(CImageList* pImageList, int nImageListType)
{
	CImageList * pSrcImageList = NULL;
	if(LVSIL_NORMAL ==  nImageListType)      pSrcImageList =  &m_imgNormal;
	else if(LVSIL_SMALL  ==  nImageListType) pSrcImageList =  &m_imgSmall;
	else if(LVSIL_STATE  ==  nImageListType) pSrcImageList =  &m_imgState;
	else return NULL;
	if(pSrcImageList ->m_hImageList) pSrcImageList ->DeleteImageList();
	pSrcImageList ->Create(pImageList);
	if(m_bReDraw) PaintDC();
	return pSrcImageList;
}
int CTPListCtrlEx::GetItemCount() const
{
	return (int)m_aItem.GetCount();
}
BOOL CTPListCtrlEx::SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,int nImage, UINT nState, UINT nStateMask, LPARAM lParam,int iIndent)
{
	if(nSubItem<0) nSubItem = 0;
	if(nItem<0 || nItem >= m_aItem.GetSize() || nSubItem>=m_aColumn.GetSize()) return FALSE;	
	TPListCtrlItem     *pItem    = m_aItem[nItem];
	TPListCtrlSubItem  *pSubItem = pItem ->aSubItem[nSubItem];

	if(nMask&LVIF_IMAGE)  pSubItem ->iImage  = nImage;
	if(nMask&LVIF_INDENT) pSubItem ->iIndent = iIndent;	
	if(nMask&LVIF_PARAM)  pSubItem ->dwData  = lParam;	
	if(nMask&LVIF_STATE)  pSubItem ->uState  = nState & nStateMask;	
	if(nMask&LVIF_TEXT)	  pSubItem ->sText   = lpszItem;	

	if(m_bReDraw) PaintDC();

	return TRUE;
}
int CTPListCtrlEx::GetNextItem(int nItem, int nFlags) 
{
	if(nFlags == LVNI_SELECTED)
	{
		for(INT l= nItem + 1;l<m_aItem.GetSize();l++)
		{
			if(m_aItem[l] ->uState &LVIS_SELECTED) return l;
		}
	}
	return -1;
}	
POSITION CTPListCtrlEx::GetLastSelectedItemPosition()
{
	int iIndex = m_aSelItem.GetSize();
	return (POSITION)iIndex;

}
POSITION CTPListCtrlEx::GetFirstSelectedItemPosition(BOOL bSort) 
{
	m_aSelItem.RemoveAll();
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(!m_aItem[l] ->bShow) {m_aItem[l] ->uState  &= ~LVIS_SELECTED; continue;}
		if(m_aItem[l] ->uState &LVIS_SELECTED)  m_aSelItem.Add(m_aItem[l]);
	}
	//排序
	int iSelCount = (int)m_aSelItem.GetSize();
	if(iSelCount <=0) return NULL;
	if(bSort)
	{	
		for(int l=0;l<iSelCount;l++)
		{
			for(INT k=l+1;k<iSelCount;k++)
			{
				if(m_aSelItem[l] ->dwSelTick >  m_aSelItem[k] ->dwSelTick)
				{
					TPListCtrlItem *pSwap =  m_aSelItem[k];
					m_aSelItem[k]        =  m_aSelItem[l];
					m_aSelItem[l]        = pSwap; 
				}
			}
		}
	}
	m_iSelItem = 0x00;
	return (POSITION)(m_iSelItem+1);
}
int CTPListCtrlEx::GetNextSelectedItem(POSITION& pos) 
{	
	int iIndex = -1;
	if(m_iSelItem < m_aSelItem.GetSize())  iIndex = m_aSelItem[m_iSelItem] ->iIndex;
	m_iSelItem ++;
	if(m_iSelItem >= m_aSelItem.GetSize())  pos = NULL;
	else                                    pos = (POSITION)(m_iSelItem + 1);
	return iIndex;
}
BOOL CTPListCtrlEx::GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	if(m_aItem[nItem]->aSubItem.GetSize()<=0) return FALSE;
	//add by qushaojie 20090916 LVIR_BOUNDS返回item所有列的区域和
	if(LVIR_BOUNDS == nCode)      
	{
		CRect rtTemp(0,0,0,0);
		if(VIEWTYPE_ICON == m_iViewType )
		{
            *lpRect = m_aItem[nItem]->aSubItem[0]->rtItem;
		}
		else if (VIEWTYPE_BUFFER == m_iViewType || VIEWTYPE_INFO == m_iViewType)
		{
			ASSERT(FALSE);
		}
		else 
		{
			for (int i=0;i<m_aColumn.GetSize();i++)
			{	
				if(!m_aColumn[i]->bShow) continue;
				rtTemp |=  m_aItem[nItem]->aSubItem[i]->rtItem;
			}	
			*lpRect = rtTemp;
		}
	}
	else if(LVIR_ICON == nCode)   *lpRect =  m_aItem[nItem]->aSubItem[0]->rtIcon;
	else if(LVIR_LABEL == nCode)  *lpRect =  m_aItem[nItem]->aSubItem[0]->rtLable;
	else return FALSE;	
	return TRUE;
}
BOOL CTPListCtrlEx::SetItemPosition(int nItem, POINT pt)
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	if(m_aItem[nItem]->aSubItem.GetSize()<=0) return FALSE;
	CRect rtItem = 	m_aItem[nItem]->aSubItem[0]->rtItem;
	rtItem.OffsetRect(-rtItem.left,-rtItem.top);
	rtItem.OffsetRect(pt);
	m_aItem[nItem]->aSubItem[0]->rtItem = rtItem;
	m_aItem[nItem]->aSubItem[0]->CalcSize();
	if(m_bReDraw)
	{
		FunCalcScrollBar();
		PaintDC();
	}
	return TRUE;
}
BOOL CTPListCtrlEx::GetItemPosition(int nItem, LPPOINT lpPoint) const
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	if(m_aItem[nItem]->aSubItem.GetSize()<=0) return FALSE;
	CRect rtItem = 	m_aItem[nItem]->aSubItem[0]->rtItem;
	lpPoint->x = rtItem.left;
	lpPoint->y = rtItem.top;
	return TRUE;;
}
int CTPListCtrlEx::GetStringWidth(LPCTSTR lpsz) 
{
	CDC *pDC = CTPWnd::GetDC();
	CRect rtText;
	pDC->DrawText(lpsz,rtText,DT_CALCRECT);
	CTPWnd::ReleaseDC(pDC);
	return rtText.Width();
}
int CTPListCtrlEx::GetColumnWidth(int nCol) const
{
	if(nCol <0 || nCol >= m_aColumn.GetSize()) return 0;
	return m_aColumn[nCol] ->iWidth;	
}
void  CTPListCtrlEx::SetColumnFormat(int nCol,UINT uFormat,UINT uMask)
{
	if(nCol <0 || nCol >= m_aColumn.GetSize()) return;
	m_aColumn[nCol] ->uFmt &= ~uMask;	
	m_aColumn[nCol] ->uFmt |= uFormat;	
}
void CTPListCtrlEx::SetColumnText(int nCol,LPCTSTR sText)
{
	if(nCol <0 || nCol >= m_aColumn.GetSize()) return ;
	m_aColumn[nCol] ->sItem = sText;}

BOOL CTPListCtrlEx::SetColumnWidth(int nCol, int cx)
{
	if(nCol <0 || nCol >= m_aColumn.GetSize()) return 0;
	if(m_aColumn[nCol] ->iWidth == cx) return TRUE;
	m_aColumn[nCol] ->iWidth = cx;
	if(m_bReDraw)
	{
		ReLayOut();
		PaintDC();
	}
	return TRUE;
}
int CTPListCtrlEx::GetTopIndex() const
{
	if(m_aColumn.GetSize() <=0 || m_aItem.GetSize()<=0) return -1;
	for(int l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l]->aSubItem[0]->rtItem.bottom>=0) return l;
	}
	return -1;
}
void  CTPListCtrlEx::SetTopIndex(int nItem) 
{
	CRect rtClient;
	GetClientRect(&rtClient);

	for(int  l=0;l<(int)m_aItem.GetSize();l++)
	{
		if(nItem == l)
		{
			CRect  rtItem;
			if(m_aItem.GetAt(l)->aSubItem[0]->rtItem.top<0)
			{
				m_iTopY -= abs(m_aItem.GetAt(l)->aSubItem[0]->rtItem.top);
				m_iTopY  = max(0,m_iTopY);
			}
			else 
			{
				m_iTopY += m_aItem.GetAt(l)->aSubItem[0]->rtItem.bottom;
			}

			if(m_aItem.GetAt(l)->aSubItem[0]->rtItem.right>rtClient.Width())
				m_iTopX += m_aItem.GetAt(l)->aSubItem[0]->rtItem.left;
			break;
		}
	}


	SCROLLINFO scrInfoV,scrInfoH;
	ZeroMemory(&scrInfoV,sizeof(scrInfoV));
	ZeroMemory(&scrInfoH,sizeof(scrInfoH));

	scrInfoV.cbSize = sizeof(SCROLLINFO);
	scrInfoV.fMask  = SIF_POS; 	
	scrInfoH.cbSize = sizeof(SCROLLINFO);
	scrInfoH.fMask  = SIF_POS;


	SetScrollInfo(SBS_HORZ,&scrInfoH,TRUE);
	SetScrollInfo(SBS_VERT,&scrInfoV,TRUE);

	FunCalcSize();

}
int CTPListCtrlEx::GetCountPerPage() const
{
	CRect rtClient;
	GetClientRect(&rtClient);
	if(m_iViewType == VIEWTYPE_ICON)    
	{
		rtClient.DeflateRect(m_szEdge.cx,m_szEdge.cy);
		return rtClient.Width() / (m_szItemIcon.cx + m_szSpace.cx) * (rtClient.Height() / (m_szItemIcon.cy + m_szSpace.cy));
	}
	else if(m_iViewType == VIEWTYPE_REPORT)
	{
		return rtClient.Height()/m_szItemReport.cy;
	}
	else if(m_iViewType == VIEWTYPE_TEXTPICTURE)   
	{		
		return rtClient.Height()/m_szItemTextPicture.cy;
	}
	else if(m_iViewType == VIEWTYPE_LIST)     
	{
		rtClient.DeflateRect(m_szEdge.cx,m_szEdge.cy);
		return rtClient.Width() / (m_szItemSmallIcon.cx + m_szSpace.cx) * (rtClient.Height() / (m_szItemSmallIcon.cy + m_szSpace.cy));
	}
	else if(m_iViewType == VIEWTYPE_INFO)     
	{
		return rtClient.Height()/m_szItemInfo.cy;
	}
	else if(m_iViewType == VIEWTYPE_BUFFER)    
	{
		rtClient.DeflateRect(m_szEdge.cx,m_szEdge.cy);
		return rtClient.Width() / (m_szItemBuffer.cx + m_szSpace.cx) * (rtClient.Height() / (m_szItemBuffer.cy + m_szSpace.cy));
	}
	else return 0;
}
BOOL CTPListCtrlEx::SetColumnState(int nItem, UINT nState, UINT nMask)
{
	if(nItem<0 || nItem >= m_aColumn.GetSize()) return FALSE;
	TPListCtrlColumn     *pItem    = m_aColumn[nItem];
	pItem ->uState  = nState & nMask;	
	return TRUE;
}
UINT CTPListCtrlEx::GetColumnState(int nItem, UINT nMask) const
{
	if(nItem<0 || nItem >= m_aColumn.GetSize()) return 0;
	TPListCtrlColumn     *pItem    = m_aColumn[nItem];
	return pItem ->uState &  nMask;
}
BOOL CTPListCtrlEx::SetItemState(int nItem, UINT nState, UINT nMask)
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	TPListCtrlItem     *pItem    = m_aItem[nItem];
	nState &= ~LVIS_FOCUSED;
	pItem ->uState  = nState & nMask;	
	if(m_bReDraw) PaintDC();
	return TRUE;
}
UINT CTPListCtrlEx::GetItemState(int nItem, UINT nMask) const
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return 0;
	TPListCtrlItem     *pItem    = m_aItem[nItem];
	return pItem ->uState &  nMask;
}
CString CTPListCtrlEx::GetItemText(int nItem, int nSubItem) const
{
	if(nSubItem<0) nSubItem = 0;
	if(nItem<0 || nItem >= m_aItem.GetSize() || nSubItem>=m_aColumn.GetSize()) return FALSE;	
	return m_aItem[nItem] ->aSubItem[nSubItem] ->sText;	
}
int CTPListCtrlEx::GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const
{
	if(nSubItem<0) nSubItem = 0;
	if(nItem<0 || nItem >= m_aItem.GetSize() || nSubItem>=m_aColumn.GetSize()) return 0;	
	CString sText = m_aItem[nItem] ->aSubItem[nSubItem] ->sText;
	lstrcpyn(lpszText,sText,nLen);
	return sText.GetLength();
}
BOOL CTPListCtrlEx::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	if(nSubItem<0) nSubItem = 0;
	if(nItem<0 || nItem >= m_aItem.GetSize() || nSubItem>=m_aColumn.GetSize()) return FALSE;	
	m_aItem[nItem] ->aSubItem[nSubItem] ->sText = lpszText;	
	if(m_bReDraw) Invalidate();
	return TRUE;
}
void CTPListCtrlEx::SetItemCount(int nItems)
{
	int iColumn = (int)m_aColumn.GetSize();
	for(INT l=0;l<nItems;l++)
	{
		TPListCtrlItem *pItem = new TPListCtrlItem;
		pItem ->SetSubItemCount(iColumn);
		m_aItem.Add(pItem);
	}	
	if(m_bReDraw)
	{
		ReLayOut();
		PaintDC();
	}
}
BOOL CTPListCtrlEx::SetItemData(int nItem, DWORD_PTR dwData)
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	m_aItem[nItem] ->dwData = dwData;
	return TRUE;
}
BOOL CTPListCtrlEx:: SetColumnOrderArray(LPINT piArray, int iCount )
{
	iCount = min((int)m_aColumn.GetSize(),iCount);
	for(INT l=0;l<iCount;l++) if(piArray[l] >=iCount) return FALSE;
	TPListCtrlColumnArray aTemp;
	aTemp.Copy(m_aColumn);
	m_aColumn.RemoveAll();
	for(INT l=0;l<iCount;l++)
	{
		if(aTemp[piArray[l]] == NULL) continue;
		m_aColumn.Add(aTemp[piArray[l]]);
		aTemp[piArray[l]] = NULL;
	}
	for(INT l=0;l<aTemp.GetSize();l++)
	{
		if(aTemp[l]!=NULL) m_aColumn.Add(aTemp[l]);
	}
	for(INT l=0;l<m_aColumn.GetSize();l++)
		m_aColumn[l] ->iIndex = l;	
	return TRUE;
}
BOOL CTPListCtrlEx:: GetColumnOrderArray(LPINT piArray, int iCount )
{
	for(INT l=0;l<iCount && l<m_aColumn.GetSize();l++)
	{		
		piArray[l] = m_aColumn[l] ->iIndexInit;
	}
	return TRUE;
}
int CTPListCtrlEx::GetItemOrder(int nItem)
{
	if(nItem <0 || nItem >=m_aColumn.GetSize()) return - 1;
	return m_aColumn[nItem] ->iIndexInit;
}
int CTPListCtrlEx::GetItemByOrder(int nOrder)
{
	for(INT l=0;l<m_aColumn.GetSize();l++)
		if(m_aColumn[l] ->iIndexInit == nOrder)  return l;
	return -1;
}
void CTPListCtrlEx::GetItem(int nItem,HDITEM *hdi)
{
	if(nItem <0 || nItem >=m_aColumn.GetSize()) return;
	hdi ->cxy = m_aColumn[nItem] ->iWidth;
}

DWORD_PTR CTPListCtrlEx::GetItemData(int nItem) const
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return NULL;
	return m_aItem[nItem] ->dwData;	
}
UINT CTPListCtrlEx::GetSelectedCount() const
{
	int iSel = 0;
	for(INT l=0;l<m_aItem.GetSize();l++)
	{		
		if(m_aItem[l] ->uState &LVIS_SELECTED) iSel ++;
	}
	return iSel;
}
BOOL CTPListCtrlEx::GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& ref)
{
	if(nSubItem<0) nSubItem = 0;
	if(nItem<0 || nItem >= m_aItem.GetSize() || nSubItem>=m_aColumn.GetSize()) return FALSE;	
	ref = m_aItem[nItem] ->aSubItem[nSubItem] ->rtItem;
	return TRUE;
}
int CTPListCtrlEx::GetHotItem()
{
	return m_iHotItem;
}
int CTPListCtrlEx::SetHotItem(int iIndex)
{
	if(m_iHotItem == iIndex) return iIndex;
	m_iHotItem = iIndex;
	if(m_bReDraw) PaintDC();
	return iIndex;
}

BOOL CTPListCtrlEx::GetCheck(int nItem) const
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	return m_aItem[nItem] ->bCheck;
}
BOOL CTPListCtrlEx::SetCheck(int nItem, BOOL fCheck )
{
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	m_aItem[nItem] ->bCheck = fCheck;
	if(m_bReDraw) PaintDC();
	return TRUE;
}
int CTPListCtrlEx::InsertItem(int nItem, LPCTSTR lpszItem,int iHeight)
{
	InsertItem(nItem,lpszItem,-1,iHeight);
	return nItem;
}
int CTPListCtrlEx::InsertItem(int nItem, LPCTSTR lpszItem, int nImage,int iHeight)
{
	if(nItem<0 || nItem >= (int)m_aItem.GetSize()) nItem = (int)m_aItem.GetSize();
	int iColumnCount  = (int)m_aColumn.GetSize();
	if(iColumnCount <=0) InsertColumn(0,_L(""));
	iColumnCount  = (int)m_aColumn.GetSize();
	if(iColumnCount <=0)  return -1;
	TPListCtrlItem *pItem = new TPListCtrlItem();
	pItem ->SetSubItemCount(iColumnCount);
	pItem ->SetItemText(0,lpszItem,DT_CENTER);
	pItem ->SetItemImage(0,nImage);
	if(iHeight >0)
		pItem->SetItemHeight(iHeight);
	m_aItem.InsertAt(nItem,pItem);
	m_uState &= ~STATE_DRAGITEM;
	if(m_bReDraw)
	{
		FunCalcSize();
		Invalidate(FALSE);
	}	
	return nItem;
}
BOOL CTPListCtrlEx::DeleteItem(int nItem)
{
	FunNotifyParent(LVN_DELETEITEM,nItem,0);
	if(nItem <0 || nItem >= m_aItem.GetSize()) return FALSE;
	delete m_aItem[nItem];
	m_aItem.RemoveAt(nItem);
	m_uState &= ~STATE_DRAGITEM;
	if(m_bReDraw)
	{
		FunCalcSize();		
		PaintDC();
	}
	return TRUE;
}
BOOL CTPListCtrlEx::DeleteAllItems()
{
	m_uBaseState |= TP_BASESTATE_DELETEITEM;
	for(int n = (int)m_aItem.GetSize()-1;n>=0;n--)
	{
		ASSERT(m_aItem.GetSize() >= 0);
		FunNotifyParent(LVN_DELETEITEM,n,0);
		ASSERT(m_aItem.GetSize() >= 0);
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
		PaintDC();
	}
	m_uBaseState &= ~TP_BASESTATE_DELETEITEM;
	return FALSE;
}

int CTPListCtrlEx::HitTest(CPoint pt, UINT* pFlags)  const
{
	TPListCtrlHitTest hitData;
	if(FunHitTest(pt,&hitData) && !hitData.bHead)
	{
		return hitData.iItem;
	}
	return -1;
}
int CTPListCtrlEx::SubItemHitTest(LPLVHITTESTINFO pInfo)
{
	return 0;
}
BOOL CTPListCtrlEx::EnsureVisible(int nItem, BOOL bPartialOK)
{
	CRect rtClient;
	GetClientRect(&rtClient);

	for(int  l=0;l<(int)m_aItem.GetSize();l++)
	{
		if(nItem == l)
		{
			CRect  rtItem;
			if(m_aItem.GetAt(l)->aSubItem[0]->rtItem.bottom>rtClient.Height())
				m_iTopY += (m_aItem.GetAt(l)->aSubItem[0]->rtItem.bottom-rtClient.Height());
			else if(m_iViewType != VIEWTYPE_REPORT && m_aItem.GetAt(l)->aSubItem[0]->rtItem.top<0 || m_iViewType == VIEWTYPE_REPORT && m_aItem.GetAt(l)->aSubItem[0]->rtItem.top<m_iColumnHeight)
			{
				//m_iTopY -= abs(abs(m_aItem.GetAt(l)->aSubItem[0]->rtItem.top) +m_aItem.GetAt(l)->aSubItem[0]->rtItem.Height());//jyn added 10.27 移动的太多
				m_iTopY -=abs(m_iColumnHeight-m_aItem.GetAt(l)->aSubItem[0]->rtItem.top+1);
				m_iTopY  = max(0,m_iTopY);
			}

			if(m_aItem.GetAt(l)->aSubItem[0]->rtItem.right>rtClient.Width())
			{
				if(m_iViewType == VIEWTYPE_REPORT)
					m_iTopX += m_aItem.GetAt(l)->aSubItem[0]->rtItem.left; //qushaojie 20090916
				else
					m_iTopX += m_aItem.GetAt(l)->aSubItem[0]->rtItem.right - rtClient.Width();
			}
			//20090806 最左边看不见情况
			if (VIEWTYPE_ICON == m_iViewType || m_iViewType == VIEWTYPE_REPORT)
			{
				if(m_aItem.GetAt(l)->aSubItem[0]->rtItem.left<0)
					m_iTopX += m_aItem.GetAt(l)->aSubItem[0]->rtItem.left;
			}
			break;
		}
	}


	SCROLLINFO scrInfoV,scrInfoH;
	ZeroMemory(&scrInfoV,sizeof(scrInfoV));
	ZeroMemory(&scrInfoH,sizeof(scrInfoH));

	scrInfoV.nPos   = m_iTopY;
	scrInfoH.nPos	= m_iTopX;
	scrInfoV.cbSize = sizeof(SCROLLINFO);
	scrInfoV.fMask  = SIF_POS; 	
	scrInfoH.cbSize = sizeof(SCROLLINFO);
	scrInfoH.fMask  = SIF_POS;


	SetScrollInfo(SBS_HORZ,&scrInfoH,TRUE);
	SetScrollInfo(SBS_VERT,&scrInfoV,TRUE);

	FunCalcSize();

	return FALSE;
}
int CTPListCtrlEx::InsertColumn(int nCol, const LVCOLUMN* pColumn)
{
	return InsertColumn(nCol,pColumn->pszText,pColumn->fmt,pColumn->cx,pColumn->iSubItem);
}
int CTPListCtrlEx::InsertColumn(int nCol, LPCTSTR lpszColumnHeading,int nFormat, int nWidth, int nSubItem)
{
	if(nCol < 0 || nCol >= (int)m_aColumn.GetSize()) nCol = (int)m_aColumn.GetSize();

	TPListCtrlColumn *pColumn = new TPListCtrlColumn;

	pColumn ->iIndex      = nCol;
	pColumn ->iIndexInit  = nCol;
	pColumn ->iWidth      = nWidth;    
	pColumn ->iImage      = -1;
	pColumn ->sItem       = lpszColumnHeading;
	pColumn ->uFmt        = HDF_STRING;

	if(nFormat == LVCFMT_LEFT)        pColumn ->uFmt |= HDF_LEFT;
	else if(nFormat == LVCFMT_RIGHT)  pColumn ->uFmt |= HDF_RIGHT;
	else if(nFormat == LVCFMT_CENTER) pColumn ->uFmt |= HDF_CENTER;

	m_aColumn.InsertAt(nCol,pColumn);

	for(int  l=0;l<(int)m_aItem.GetSize();l++)
		m_aItem[l]->InsertSubItem(nCol);

	for(int l=0;l<m_aColumn.GetSize();l++)
	{
		m_aColumn[l] ->iIndex      = l;
		m_aColumn[l] ->iIndexInit  = l;
	}

	m_uState &= ~STATE_DRAGITEM;
	if(m_bReDraw)
	{
		FunCalcSize();		
		PaintDC();
	}



	return -1;
}
int  CTPListCtrlEx::GetColumnCount()
{
	return (int)m_aColumn.GetSize();
}

BOOL CTPListCtrlEx::DeleteColumn(int nCol)
{
	if(nCol < 0 || nCol >= m_aColumn.GetSize()) return FALSE;
	delete m_aColumn[nCol];
	m_aColumn.RemoveAt(nCol);

	for(int  l=0;l<(int)m_aItem.GetSize();l++)
		m_aItem[l]->DeleteSubItem(nCol);

	m_uState &= ~STATE_DRAGITEM;
	if(m_bReDraw)
	{
		FunCalcSize();		
		PaintDC();
	}
	return TRUE;
}
CImageList* CTPListCtrlEx::CreateHeadDragImage(int nItem)
{	
	if(nItem<0 || nItem >= m_aColumn.GetSize()) return NULL;
	CDC *pDC = GetDC(),cdcMemo;
	CRect rtColumn = m_aColumn[nItem] ->rtItem;
	m_aColumn[nItem] ->rtItem.OffsetRect(-m_aColumn[nItem] ->rtItem.left,-m_aColumn[nItem] ->rtItem.top);
	cdcMemo.CreateCompatibleDC(pDC);
	CBitmap bmLoad,bmCreate;BITMAP bmInfo;
	bmLoad.LoadBitmap(_L("IDB_DEFBITMAP"));
	bmLoad.GetBitmap(&bmInfo);
	bmLoad.DeleteObject();
	bmInfo.bmHeight  = rtColumn.Height();
	bmInfo.bmWidth   = rtColumn.Width();
	bmInfo.bmWidthBytes = rtColumn.Width() *4;
	bmCreate.CreateBitmapIndirect(&bmInfo);
	CBitmap *pSaveBmp  = cdcMemo.SelectObject(&bmCreate);
	CFont *pSaveFont   = cdcMemo.SelectObject(TP_GetSysFont(m_iFontIndex)); 
	cdcMemo.SetBkMode(TRANSPARENT);
	cdcMemo.SetStretchBltMode(COLORONCOLOR);	
	DrawColumn(&cdcMemo,nItem);
	cdcMemo.SelectObject(pSaveBmp);
	cdcMemo.SelectObject(pSaveFont);
	cdcMemo.DeleteDC();
	CImageList *pImageList = new CImageList;
	pImageList ->Create(bmInfo.bmWidth,bmInfo.bmHeight,ILC_COLORDDB|ILC_MASK,0,1);
	int iIndex = pImageList ->Add(&bmCreate,RGB(0,0,0));
	bmCreate.DeleteObject();
	ReleaseDC(pDC);
	m_aColumn[nItem] ->rtItem = rtColumn;
	return pImageList;		
}
CImageList *CTPListCtrlEx::CreateDragImage(LPBYTE pSrcBuffer,CSize szBuffer,COLORREF clMask, BYTE bTrans)
{
	CDC    *pDC = AfxGetMainWnd() ->GetDC();

	HBITMAP  hbmColor  =  TP_CreateDIBBitmap(pSrcBuffer,szBuffer.cx,szBuffer.cy,pDC);
	CBitmap *pbmCreate = CBitmap::FromHandle(hbmColor);

	BITMAP bmInfo;	
	pbmCreate->GetBitmap(&bmInfo);	
	CImageList *pImageList = new CImageList;
	pImageList ->Create(bmInfo.bmWidth,bmInfo.bmHeight,ILC_COLOR32|ILC_MASK,0,1);
	int   iIndex =   pImageList ->Add(pbmCreate,clMask);	
	if(hbmColor) ::DeleteObject(hbmColor);

	AfxGetMainWnd() ->ReleaseDC(pDC);
	return pImageList;
}
HCURSOR     CTPListCtrlEx::CreateDragIcon(LPBYTE pSrcBuffer,CSize szBuffer,COLORREF clMask, LPPOINT lpPoint,HCURSOR hCursor,BYTE bTrans)
{
	if(!m_bCreateLargeIcon) return CopyCursor(hCursor);

	HICON hIcon = NULL;

	CPoint ptCursor = *lpPoint;	
	CDC    *pDC = AfxGetMainWnd() ->GetDC();
	CTPMemoDC memDC(pDC,szBuffer .cx + 64,szBuffer .cy + 64);
	memDC.FillSolidRect(0,0,szBuffer .cx + 64,szBuffer .cy + 64,clMask);
	CTPMemoDC memDC1(pDC,szBuffer .cx + 64,szBuffer .cy + 64);
	memDC1.FillSolidRect(0,0,szBuffer .cx + 64,szBuffer .cy + 64,clMask);
	TP_SetDIBitsToDevice(&memDC,pSrcBuffer,szBuffer.cx,szBuffer.cy,CRect(32,32,32+szBuffer.cx,32+szBuffer.cy));

	if(NULL != hCursor) 
	{
		memDC.DrawIcon(ptCursor.x ,ptCursor.y ,hCursor);
		memDC1.DrawIcon(ptCursor.x ,ptCursor.y ,hCursor);
	}
	else if(GetCursor())
	{
		memDC.DrawIcon(ptCursor.x ,ptCursor.y ,GetCursor());
		memDC1.DrawIcon(ptCursor.x ,ptCursor.y ,GetCursor());
	}

	CSize  szTemp,szTemp1;
	LPBYTE pBuffer  =  TP_GetBitmapBits((HBITMAP)memDC.GetBitmap(FALSE) ->GetSafeHandle(),szTemp,pDC);
	LPBYTE pBuffer1 =  TP_GetBitmapBits((HBITMAP)memDC1.GetBitmap(FALSE) ->GetSafeHandle(),szTemp1,pDC);
	if(pBuffer  == NULL ||  pBuffer1 == NULL)
	{
		if(pBuffer) delete pBuffer;
		pBuffer = NULL;
		if(pBuffer1) delete pBuffer1;
		pBuffer1 = NULL;	
		AfxGetMainWnd() ->ReleaseDC(pDC);
		return NULL;
	}

	BYTE bR = GetRValue(clMask);
	BYTE bG = GetGValue(clMask);
	BYTE bB = GetBValue(clMask);

	int iCursorPos = ((szTemp.cy - ptCursor.y-2)  * szTemp.cx  + ptCursor.x+2) *4;
	for(int l= 0;l<szTemp.cx * szTemp.cy;l++)
	{		
		pBuffer [l*4+3] = 0xff;
		if(pBuffer [l*4] == pBuffer[0] &&pBuffer [l*4+1] == pBuffer[1] && pBuffer [l*4+2] == pBuffer[2] ||
			pBuffer [l*4] == bR &&pBuffer [l*4+1] == bG && pBuffer [l*4+2] == bB || 
			pBuffer [l*4] == pBuffer1[iCursorPos] &&pBuffer[l*4+1] == pBuffer1[iCursorPos + 1] && pBuffer [l*4+2] == pBuffer1[iCursorPos + 2])  
		{
			pBuffer [l*4 +3] = 0x0;
		}
		else if(pBuffer1 [l*4] == pBuffer1[0] && pBuffer1 [l*4+1] == pBuffer1[1] && pBuffer1 [l*4+2] == pBuffer1[2] ||
			pBuffer1 [l*4] == pBuffer1[iCursorPos] && pBuffer1 [l*4+1] == pBuffer1[iCursorPos + 1] && pBuffer1 [l*4+2] == pBuffer1[iCursorPos + 2])
			pBuffer [l*4 +3] = bTrans;
	}

	ICONINFO iconinfo;
	iconinfo.fIcon = FALSE;	
	iconinfo.xHotspot = 0;
	iconinfo.yHotspot = 0;
	if(GetCursor()) 
	{
		iconinfo.xHotspot = ptCursor.x +9 ;
		iconinfo.yHotspot = ptCursor.y +7;
	}

	iconinfo.hbmColor =  TP_CreateDIBBitmap(pBuffer,szTemp.cx,szTemp.cy,pDC);
	iconinfo.hbmMask  =  iconinfo.hbmColor;

	hIcon  = (HCURSOR)::CreateIconIndirect(&iconinfo);
	if(iconinfo.hbmColor) ::DeleteObject(iconinfo.hbmColor);


	if(pBuffer) delete pBuffer;
	pBuffer = NULL;
	if(pBuffer1) delete pBuffer1;
	pBuffer1 = NULL;		

	AfxGetMainWnd() ->ReleaseDC(pDC);
	return hIcon;
}
BOOL TP_ComposeColor(LPBYTE pByteSrc,LPBYTE pByteDest)
{
	try
	{	
		if(pByteDest[0] <pByteSrc[0]-2 || pByteDest[0] >pByteSrc[0]+2) return FALSE;
		if(pByteDest[1] <pByteSrc[1]-2 || pByteDest[1] >pByteSrc[1]+2) return FALSE;
		if(pByteDest[2] <pByteSrc[2]-2 || pByteDest[2] >pByteSrc[2]+2) return FALSE;
		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}
}
int WriteBmp1(TCHAR* ptszFileName,int iWidth,int iHeight,int iDepth,BYTE* pData)
{
	BITMAPINFOHEADER stInfoHdr;
	memset(&stInfoHdr,0,sizeof(BITMAPINFOHEADER));
	stInfoHdr.biSize=sizeof(BITMAPINFOHEADER);
	stInfoHdr. biPlanes = 1;
	stInfoHdr.biWidth=iWidth;
	stInfoHdr.biHeight=iHeight;
	stInfoHdr.biBitCount=iDepth;
	stInfoHdr.biSizeImage=stInfoHdr.biWidth*stInfoHdr.biHeight*stInfoHdr.biBitCount/8;

	BITMAPFILEHEADER stFileHdr;
	ZeroMemory(&stFileHdr, sizeof(BITMAPFILEHEADER));
	stFileHdr.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	stFileHdr.bfSize=stInfoHdr.biSizeImage + stFileHdr.bfOffBits;
	stFileHdr.bfType=0x4d42;

	FILE* fp=NULL;
	_tfopen_s(&fp,ptszFileName,_T("wb"));
	fwrite(&stFileHdr,1,sizeof(BITMAPFILEHEADER),fp);
	fwrite(&stInfoHdr,1,sizeof(BITMAPINFOHEADER),fp);
	fwrite(pData,1,stInfoHdr.biSizeImage,fp);
	fclose(fp);
	return 1;
}
void TP_GetCenterBuffer(LPBYTE pBuffer,CSize &szBuffer,CPoint &ptCursor,BYTE bTrans)
{
	if(szBuffer.cx *szBuffer.cy <800*500) return ;
	CRect rtClient = CRect(0,0,szBuffer.cx,szBuffer.cy);
	CRect rtCenter = CRect(ptCursor.x-10,ptCursor.y-10,ptCursor.x+10,ptCursor.y+10);
	while(rtCenter.Width()*rtCenter.Height()<800*500-20)
	{
		rtCenter.InflateRect(10,10);
		rtCenter &= rtClient;
	}
	ptCursor.Offset(-rtCenter.left,-rtCenter.top);
	for(INT l = rtCenter.top;l<rtCenter.bottom;l++)
	{
		memcpy(pBuffer + (l-rtCenter.top) * rtCenter.Width()* 4,pBuffer + l * szBuffer.cx * 4 + rtCenter.left *4,rtCenter.Width()*4);
	}

	int iSetp  = bTrans;
	int iWidth =rtCenter.Width(),iHeight = rtCenter.Height();
	if(rtCenter.top > rtClient.top && iHeight>iSetp)
	{
		for(INT l=0;l<iSetp;l++)
		{
			for(INT k=0;k<iWidth;k++)
			{
				if(*(pBuffer + (iWidth -1- l) *iWidth *4 + k *4 + 3)!=0)
				{					
					*(pBuffer + (iWidth-1- l) *iWidth *4 + k *4 + 3)  = l;
				}
			}
		}		
	}
	if(rtCenter.bottom < rtClient.bottom&& iHeight>iSetp)
	{	
		for(INT l=0;l<iSetp;l++)
		{
			for(INT k=0;k<iWidth;k++)
			{
				if(*(pBuffer + l *iWidth *4 + k *4 + 3)!=0)
				{

					*(pBuffer + l *iWidth *4 + k *4 + 3)  = l;
				}
			}
		}

	}
	if(rtCenter.left > rtClient.left &&iWidth>iSetp)
	{
		for(INT l=0;l<iSetp;l++)
		{
			for(INT k=0;k<iHeight;k++)
			{
				if(*(pBuffer + k *iWidth *4 + (iWidth - 1 -l) *4 + 3)!=0)
				{					
					*(pBuffer + k *iWidth *4 + (iWidth - 1 -l) *4 + 3)  = l;
				}
			}
		}	
	}
	if(rtCenter.right < rtClient.right&&iWidth>iSetp)
	{	
		for(INT l=0;l<iSetp;l++)
		{
			for(INT k=0;k<iHeight;k++)
			{
				if(*(pBuffer + k *iWidth *4 + l *4 + 3)!=0)
				{					
					*(pBuffer + k *iWidth *4 + l *4 + 3)  = l;
				}
			}
		}		
	}

	szBuffer = CSize(rtCenter.Width(),rtCenter.Height());
}
void CTPListCtrlEx::ReleaseDragBitmap()
{
	if(NULL != m_pDragBitmap)
	{
		m_pDragBitmap ->DeleteObject();
		delete m_pDragBitmap;
		m_pDragBitmap = NULL;
	}
}
HCURSOR     CTPListCtrlEx::CreateDragIcon(int nItem,COLORREF clMask, BOOL bEndCreate,LPPOINT lpPoint,HCURSOR hCutsor,BYTE bTrans)
{
	if(!m_bCreateLargeIcon) return CopyCursor(hCutsor);
//
//#ifdef _DEBUG
//	{
//		HCURSOR hCursor = (HCURSOR)LoadImage(NULL, _T("D:\\Test.cur"), IMAGE_CURSOR, 512, 512, LR_DEFAULTCOLOR | LR_LOADFROMFILE);
//		return hCursor;
//	}
//#endif
	HICON hIcon     = NULL;
	BITMAP imageInfo;
	CPoint ptCursor = *lpPoint;
	if(m_pDragBitmap == NULL) m_pDragBitmap = CreateDragBitmap(nItem, lpPoint);	
	if(m_pDragBitmap == NULL) return NULL;
	ptCursor.Offset(-m_szTemp.cx  + 32,-m_szTemp.cy  + 32);
	if(!m_pDragBitmap ->GetBitmap(&imageInfo))
	{
		m_pDragBitmap ->DeleteObject();
		delete m_pDragBitmap;
		m_pDragBitmap = NULL;
		return NULL;
	}


	ptCursor.x = min(ptCursor.x,imageInfo.bmWidth);
	ptCursor.y = min(ptCursor.y,imageInfo.bmHeight +32);

	if(GetCursor() == NULL)
	{		
		m_pDragBitmap->DeleteObject();
		delete m_pDragBitmap ;	
		m_pDragBitmap = NULL;
		return NULL;
	}
	CDC    *pDC = GetDC();
	CTPMemoDC memDC(pDC,imageInfo.bmWidth + 64,imageInfo.bmHeight + 64);
	memDC.FillSolidRect(0,0,imageInfo.bmWidth+64,imageInfo.bmHeight+64,clMask);
	CTPMemoDC memDC1(pDC,imageInfo.bmWidth + 64,imageInfo.bmHeight+64);
	memDC1.FillSolidRect(0,0,imageInfo.bmWidth + 64,imageInfo.bmHeight+64,clMask);
	memDC.DrawBitmap(m_pDragBitmap,CPoint(32,32));
	if(bEndCreate)
	{	
		m_pDragBitmap->DeleteObject();
		delete m_pDragBitmap ;
		m_pDragBitmap = NULL;
	}
	//pImageList ->Draw(&memDC,0,CPoint(32,32),ILD_NORMAL);
	if(NULL != hCutsor)
	{
		memDC.DrawIcon(ptCursor.x ,ptCursor.y ,hCutsor);
		memDC1.DrawIcon(ptCursor.x ,ptCursor.y ,hCutsor);
	}
	else if(GetCursor()) 
	{
		memDC.DrawIcon(ptCursor.x ,ptCursor.y ,GetCursor());
		memDC1.DrawIcon(ptCursor.x ,ptCursor.y ,GetCursor());
	}
	CSize  szTemp,szTemp1;
	LPBYTE pBuffer  =  TP_GetBitmapBits((HBITMAP)memDC.GetBitmap(FALSE) ->GetSafeHandle(),szTemp,pDC);
	LPBYTE pBuffer1 =  TP_GetBitmapBits((HBITMAP)memDC1.GetBitmap(FALSE) ->GetSafeHandle(),szTemp1,pDC);
	memDC.DeleteDC();
	memDC1.DeleteDC();

	if(pBuffer  == NULL ||  pBuffer1 == NULL || szTemp.cx != szTemp1.cx ||  szTemp.cy != szTemp1.cy)
	{	
		if(pBuffer) delete pBuffer;
		pBuffer = NULL;
		if(pBuffer1) delete pBuffer1;
		pBuffer1 = NULL;	

		ReleaseDC(pDC);
		return NULL;
	}
	BYTE byColor[3] ;
	byColor[0] = GetRValue(clMask);
	byColor[1] = GetGValue(clMask);
	byColor[2] = GetBValue(clMask);

	int iCursorPos = ((szTemp.cy - ptCursor.y-2)  * szTemp.cx  + ptCursor.x+2) *4;
	if(iCursorPos>=(szTemp.cx * szTemp.cy-1)*4)
	{		
		if(pBuffer) delete pBuffer;
		pBuffer = NULL;
		if(pBuffer1) delete pBuffer1;
		pBuffer1 = NULL;	

		ReleaseDC(pDC);
		return NULL;
	}

	for(int l= 0;l<szTemp.cx * szTemp.cy;l++)
	{		
		pBuffer [l*4+3] = 0xff;
		if(TP_ComposeColor(pBuffer,pBuffer+l*4)||TP_ComposeColor(byColor,pBuffer+l*4) ||TP_ComposeColor(pBuffer1+iCursorPos ,pBuffer+l*4))			
		{
			pBuffer  [l*4 +3] = 0x0;												
		}
		else if(TP_ComposeColor(pBuffer1,pBuffer1+l*4)||TP_ComposeColor(pBuffer1+iCursorPos,pBuffer1+l*4))
		{
			pBuffer [l*4 +3]  = bTrans;			
		}
	}


	//	TP_GetCenterBuffer(pBuffer,szTemp,ptCursor,bTrans);

	ICONINFO iconinfo;
	iconinfo.fIcon = FALSE;	
	iconinfo.xHotspot = 0;
	iconinfo.yHotspot = 0;
	if(GetCursor()) 
	{
		iconinfo.xHotspot = ptCursor.x;
		iconinfo.yHotspot = ptCursor.y;
	}

	iconinfo.hbmColor =  TP_CreateDIBBitmap(pBuffer,szTemp.cx,szTemp.cy,pDC);
	iconinfo.hbmMask  =  iconinfo.hbmColor;


	if(pBuffer) delete pBuffer;
	pBuffer = NULL;
	if(pBuffer1) delete pBuffer1;
	pBuffer1 = NULL;	


	hIcon  = (HCURSOR)::CreateIconIndirect(&iconinfo);
	if(iconinfo.hbmColor) ::DeleteObject(iconinfo.hbmColor);

	ReleaseDC(pDC);

	return hIcon;
}
#define TP_ICONSIZE 800*500
//#define TP_ICONSIZE 300*200
CRect TP_GetCenterRect(CRect &rtClient,LPPOINT ptCursor)
{
	CPoint ptPoint = *ptCursor;

	if(rtClient.Width() *rtClient.Height() <TP_ICONSIZE) return rtClient ;
	if(!rtClient.PtInRect(ptPoint)) ptPoint = rtClient.CenterPoint();
	
	CRect rtCenter = CRect(ptPoint.x-10,ptPoint.y-10,ptPoint.x+10,ptPoint.y+10);
	INT nMaxCount = 0;
	while(rtCenter.Width()*rtCenter.Height()<TP_ICONSIZE-20)
	{
		rtCenter.InflateRect(10,10);
		rtCenter &= rtClient;
		if(nMaxCount++ > 10000) break;
	}
	return rtCenter;
}
void CTPListCtrlEx::FunGetSelectItemInScreen(int nItem,TPListCtrlItemArray &aListGet,CRect &rtClient,LPPOINT ptCutsor,int iLevel )
{	
	//20090916 qushaojie 
	TPListCtrlItemArray aList;
	CRect rtItem(0x7ffff,0x7ffff,0,0), rtTemp(0,0,0,0);;
	if(nItem <0) //创建全部数据
	{
		CTPIntArray aSel;
		FunGetCurSel(aSel);
		if(aSel.GetSize()<=0) return ;
		for(INT l=0;l<aSel.GetSize();l++)	aList.Add(m_aItem[aSel[l]]);			
		FunGetItemInScreen(aList,rtClient);
		if(aList.GetSize() <= 0) return ;
	}
	else
	{
		aList.Add(m_aItem[nItem]);
	}	

	for(INT l=0;l<aList.GetSize();l++)
	{		
		GetItemRect(aList[l]->iIndex,rtTemp,LVIR_BOUNDS);
		rtItem.top    = min(rtTemp.top,rtItem.top);
		rtItem.bottom = max(rtTemp.bottom,rtItem.bottom);
		rtItem.left   = min(rtTemp.left,rtItem.left);
		rtItem.right  = max(rtTemp.right,rtItem.right);
	}
	// LI HUI LI 2009-04-28 DQAdb00006024
	rtClient = rtItem;
	if( rtClient.Width() * rtClient.Height() >TP_ICONSIZE)
	{
		rtClient = TP_GetCenterRect(rtClient,ptCutsor);
		FunGetItemInScreen(aList,rtClient);
	}
	if(VIEWTYPE_ICON == m_iViewType)
	{
		for (INT l=(INT)aList.GetSize()-1;l>=0;l--)
		{
			TPListCtrlItem * pItem = aList.GetAt(l);
            rtItem = pItem->aSubItem[0]->rtItem;
			if (!rtItem.EqualRect(rtItem & rtClient))
			{
				aList.RemoveAt(l);
			}
		}
	}
	if(aList.GetSize() > 0) aListGet.Copy(aList);
}
void CTPListCtrlEx::FunGetItemInScreen(TPListCtrlItemArray &aList,CRect &rtClient)
{
	CRect rtItem = CRect(0,0,0,0);
	CRect rtScreen = rtClient;	

	for(INT l= (int)aList.GetSize()-1;l>=0;l--)
	{
		GetItemRect(aList[l]->iIndex,&rtItem,LVIR_BOUNDS);
		CRect rtCross = rtItem & rtScreen;
		if(rtCross.IsRectEmpty())
		{
			aList.RemoveAt(l);
		}	
	}
}
CBitmap* CTPListCtrlEx::CreateDragBitmap(int nItem, LPPOINT lpPoint)
{
	CRect  rtClient;
	GetClientRect(&rtClient);

	TPListCtrlItemArray aList;
	CRect rtItem;
	FunGetSelectItemInScreen(nItem,aList,rtClient,lpPoint);
	rtItem = rtClient;
	if(aList.GetSize()<=0) return NULL;


	CDC *pDC = GetDC(),cdcMemo;
	cdcMemo.CreateCompatibleDC(pDC);
	CBitmap *pbmCreate = new CBitmap();				
	pbmCreate ->CreateCompatibleBitmap(pDC,rtItem.Width(),rtItem.Height());
	CBitmap *pSaveBmp  = cdcMemo.SelectObject(pbmCreate);
	CFont *pSaveFont   = cdcMemo.SelectObject(TP_GetSysFont(m_iFontIndex)); 
	cdcMemo.SetBkMode(TRANSPARENT);
	cdcMemo.SetStretchBltMode(COLORONCOLOR);
	cdcMemo.FillSolidRect(0,0,rtItem.Width(),rtItem.Height(),TP_GetSysColor(m_iBackColor));
	cdcMemo.SetViewportOrg(-rtItem.left,-rtItem.top);
	m_szTemp = CSize(rtItem.left,rtItem.top);

	if(m_iViewType == VIEWTYPE_REPORT || m_iViewType == VIEWTYPE_TEXTPICTURE)
	{
		for(INT l=0;l<aList.GetSize();l++)
		{
			if(aList[l]->aSubItem[0]->rtItem.bottom <0 || aList[l]->aSubItem[0]->rtItem.top > rtClient.bottom)
				cdcMemo.Draw3dRect(rtClient.left,aList[l]->aSubItem[0]->rtItem.top,rtClient.right,aList[l]->aSubItem[0]->rtItem.bottom,RGB(0,0,0),RGB(0,0,0));
			else 
			{
				if(m_iViewType == VIEWTYPE_REPORT)					DrawReport(&cdcMemo,aList[l]->iIndex);
				else if(m_iViewType == VIEWTYPE_TEXTPICTURE)		DrawTextPicture(&cdcMemo,aList[l]->iIndex);
			}
		}
	}
	else if(m_iViewType == VIEWTYPE_LIST || m_iViewType == VIEWTYPE_ICON || m_iViewType == VIEWTYPE_BUFFER)
	{
		for(INT l=0;l<aList.GetSize();l++)
		{
			if(aList[l]->aSubItem[0]->rtItem.bottom <0 || aList[l]->aSubItem[0]->rtItem.top > rtClient.bottom)
			{
				cdcMemo.Draw3dRect(aList[l]->aSubItem[0]->rtItem,RGB(0,0,0),RGB(0,0,0));
			}
			else
			{			
				if(m_iViewType == VIEWTYPE_LIST)         DrawList(&cdcMemo,aList[l]->iIndex);
				else if(m_iViewType == VIEWTYPE_ICON)    DrawIcon(&cdcMemo,aList[l]->iIndex);
				else if(m_iViewType == VIEWTYPE_BUFFER)  DrawBuffer(&cdcMemo,aList[l]->iIndex);
			}
		}
	}
	else if(m_iViewType == VIEWTYPE_INFO)
	{
		for(INT l=0;l<aList.GetSize();l++)
			DrawInfo(&cdcMemo,aList[l]->iIndex);
	}

	cdcMemo.SetViewportOrg(0,0);
	cdcMemo.SelectObject(pSaveBmp);
	cdcMemo.SelectObject(pSaveFont);
	cdcMemo.DeleteDC();		
	ReleaseDC(pDC);
	return pbmCreate;	
}
CImageList* CTPListCtrlEx::CreateDragImage(int nItem, LPPOINT lpPoint)
{
	CBitmap  *pbmCreate = CreateDragBitmap (nItem,lpPoint);
	if(pbmCreate == NULL) return NULL;
	BITMAP bmInfo;	
	pbmCreate->GetBitmap(&bmInfo);	
	CImageList *pImageList = new CImageList;
	pImageList ->Create(bmInfo.bmWidth,bmInfo.bmHeight,ILC_COLOR32|ILC_MASK,0,1);
	int iIndex = pImageList ->Add(pbmCreate,TP_GetSysColor(m_iBackColor));
	pbmCreate->DeleteObject();
	delete pbmCreate;
	return pImageList;
}
BOOL CTPListCtrlEx::SortItems(PFNLVCOMPARE pfnCompare, DWORD_PTR dwData)
{
	int             iItemCount = (int)m_aItem.GetSize();
	TPListCtrlItem *pItem      = NULL;
	for(int l=0;l<iItemCount;l++)
	{
		for(INT k=l+1;k<iItemCount;k++)
		{
			TPListCtrlItem *pItemTemp      = NULL;
			pItem = m_aItem[l];
			if(pfnCompare)
			{
				if(pfnCompare(m_aItem[k]->dwData,pItem->dwData,dwData)<0)
				{
					pItemTemp   = pItem;
					pItem       = m_aItem[k];
					m_aItem[k]  = pItemTemp;
				}
				m_aItem[l]=pItem;
			}
			else
			{
				if(lstrcmp(m_aItem[k]->aSubItem[0]->sText, pItem->aSubItem[0]->sText) <0)
				{
					pItemTemp   = pItem;
					pItem       = m_aItem[k];
					m_aItem[k]  = pItemTemp;
				}
				m_aItem[l]=pItem;
			}
		}
	}	
	m_uState &= ~STATE_DRAGITEM;
	FunCalcSize();
	InvalidateRect(FALSE);
	return TRUE;
}

//start{liubo add CD2009.10.01 PE 刷新功能优化 针对每一个来排序,使用此函数必须保证除index之外的所有Item都是有序的 V4.5Feature
#ifdef TP_NS_NET
BOOL CTPListCtrlEx::SortItems(PFNLVCOMPARE pfnCompare, DWORD_PTR dwData, int index)
{
	int             iItemCount = (int)m_aItem.GetSize();
	TPListCtrlItem *pItem      = NULL;
	if (index<0 || index>= iItemCount) return FALSE;

	//先将这个ITEM放到数组的最后
	pItem = m_aItem[index];
	for (int m=index+1;m<iItemCount;m++)
	{
		m_aItem[m-1] = m_aItem[m];
	}
	m_aItem[iItemCount-1] = pItem;

	int k = iItemCount - 1;
	for(int l=0;l<iItemCount;l++)
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
			if(lstrcmp(m_aItem[l]->aSubItem[0]->sText, m_aItem[k]->aSubItem[0]->sText) >=0)
			{
				pItem      = m_aItem[l];
				m_aItem[l] = m_aItem[k];
				m_aItem[k] = pItem;
			}
		}
	}	
	m_uState &= ~STATE_DRAGITEM;
	FunCalcSize();
	InvalidateRect(FALSE);
	return TRUE;
}
#endif
//}end

void  CTPListCtrlEx::SetRedraw(BOOL bRedraw)
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
void   CTPListCtrlEx::ReLayOut()
{
	FunCalcSize();	
	PaintDC();
}
void CTPListCtrlEx::DrawIcon(CDC *pDC,int iItem)
{
	TPListCtrlItem    *pItem     = m_aItem[iItem];
	TPListCtrlSubItem *pSubItem  = pItem ->aSubItem[0];
	CRect rtIcon,rtText;
	rtText = pSubItem ->rtItem;
	rtIcon = pSubItem ->rtItem;
	rtText.top    = rtText.bottom - 16;
	rtIcon.bottom = rtText.bottom - 16;


	//图标
	CImageList *pImageList = &m_imgNormal;
	IMAGEINFO   imageInfo;	

	if(pImageList && pImageList ->m_hImageList && pImageList ->GetImageInfo(pSubItem ->iImage,&imageInfo))
	{
		pSubItem ->rtIcon = CRect(rtIcon.left  ,rtIcon.top +(rtIcon.Height() -  (imageInfo.rcImage.bottom - imageInfo.rcImage.top ))/2,0,0);
		pSubItem ->rtIcon.right  = pSubItem ->rtIcon.left + imageInfo.rcImage.right  - imageInfo.rcImage.left;
		pSubItem ->rtIcon.bottom = pSubItem ->rtIcon.top  + imageInfo.rcImage.bottom - imageInfo.rcImage.top;

		pImageList ->Draw(pDC,pSubItem ->iImage,CPoint(pSubItem ->rtIcon .left,pSubItem ->rtIcon.top),ILD_TRANSPARENT);
	}
	else pSubItem ->rtIcon = CRect(0,0,0,0);

	//文本		
	pSubItem ->rtLable = rtText;
	pSubItem ->rtDrag  = pSubItem ->rtItem; 

	if(pItem ->uState & ( LVIS_FOCUSED |LVIS_SELECTED ))
	{
		pDC ->FillSolidRect(pSubItem ->rtItem,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
		if(pItem ->uState & LVIS_FOCUSED)
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
			TP_DrawTextInRect(pDC,pSubItem ->sText,rtText,DT_CENTER|DT_NOPREFIX);		
		}
		else
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));
			pDC ->DrawText(pSubItem ->sText,rtText,DT_CENTER|DT_END_ELLIPSIS|DT_WORD_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);	
		}
	}
	else if(!IsWindowEnabled())
	{
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));
		pDC ->DrawText(pSubItem ->sText,rtText,DT_CENTER|DT_END_ELLIPSIS|DT_WORD_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);	
	}
	else
	{
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
		pDC ->DrawText(pSubItem ->sText,rtText,DT_CENTER|DT_END_ELLIPSIS|DT_WORD_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);	
	}
}
void    CTPListCtrlEx::FunDrawCorner(CDC *pDC, CRect rtCorner,DWORD dwMode)
{
	CImageList *pImageList =  NULL;
	if(dwMode ==0)      pImageList = CImageList::FromHandle(m_pImageRes[1] ->GetImage());
	else if(dwMode ==1) pImageList = CImageList::FromHandle(m_pImageRes[0] ->GetImage());
	else return ;
	IMAGEINFO imgInfo;	
	pImageList ->GetImageInfo(0,&imgInfo);
	int iWidth = imgInfo.rcImage.right  - imgInfo.rcImage.left;
	int iHight = imgInfo.rcImage.bottom - imgInfo.rcImage.top;

	for(int l=rtCorner.left;l<rtCorner.right -iWidth +1;l+=iWidth)
	{
		pImageList ->Draw(pDC,1,CPoint(l,rtCorner.top),ILD_NORMAL);	
		pImageList ->Draw(pDC,5,CPoint(l,rtCorner.bottom-iHight),ILD_NORMAL);
	}
	for(int l=rtCorner.top;l<rtCorner.bottom -iHight +1;l+=iHight)
	{
		pImageList ->Draw(pDC,7,CPoint(rtCorner.left ,l),ILD_NORMAL);
		pImageList ->Draw(pDC,3,CPoint(rtCorner.right -iWidth ,l),ILD_NORMAL);
	}
	pImageList ->Draw(pDC,0,CPoint(rtCorner.left ,rtCorner.top),ILD_NORMAL);
	pImageList ->Draw(pDC,2,CPoint(rtCorner.right - iWidth ,rtCorner.top),ILD_NORMAL);
	pImageList ->Draw(pDC,4,CPoint(rtCorner.right - iWidth ,rtCorner.bottom - iHight),ILD_NORMAL);
	pImageList ->Draw(pDC,6,CPoint(rtCorner.left           ,rtCorner.bottom - iHight),ILD_NORMAL);	
}
void CTPListCtrlEx::DrawBuffer(CDC *pDC,int iItem)
{
	TPListCtrlItem    *pItem     = m_aItem[iItem];
	TPListCtrlSubItem *pSubItem  = pItem ->aSubItem[0];


	CRect rtIcon = pSubItem ->rtItem,rtText;		
	rtIcon = pSubItem ->rtItem;
	rtIcon.DeflateRect(3,6,3,17);		
	rtText= pSubItem ->rtItem ;
	rtText.DeflateRect(3,67,3,1);

	FunDrawCorner(pDC,pSubItem ->rtItem,(pItem ->uState & LVIS_SELECTED ) ? 1:0);	

	if(pItem ->uState & ( LVIS_FOCUSED |LVIS_SELECTED ))
	{

		if(pItem ->uState & LVIS_FOCUSED)
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));					
		}
		else
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));

		}
	}
	else if(!IsWindowEnabled())
	{
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));		
	}
	else
	{
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));		
	}
	if(pItem ->pIcon)
		TP_StretchDIBits(pDC,pItem ->pIcon,pItem ->szIcon.cx,pItem ->szIcon.cy,rtIcon);	
	else pDC ->FillSolidRect(rtIcon,RGB(0,0,0));
	pDC ->DrawText(pSubItem ->sText,rtText,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_NOPREFIX);

}
void CTPListCtrlEx::DrawList(CDC *pDC,int iItem)
{
	TPListCtrlItem    *pItem     = m_aItem[iItem];
	TPListCtrlSubItem *pSubItem  = pItem ->aSubItem[0];

	CRect  rtText,rtImage;
	//位置		
	CRect rtItem = pSubItem ->rtItem;

	CImageList *pImgeList = &m_imgSmall;
	IMAGEINFO   imageInfo;

	if(pItem ->uState & ( LVIS_FOCUSED |LVIS_SELECTED))
	{
		pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
		if(pItem ->uState & LVIS_FOCUSED)	    pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
		else                				pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));				
	}
	else
	{

		if(pItem ->iBackColor>=0)          pDC ->FillSolidRect(rtItem,TP_GetSysColor(pItem ->iBackColor));
		else if(pSubItem ->iBackColor>=0)  pDC ->FillSolidRect(rtItem,TP_GetSysColor(pItem ->iBackColor));	
		if(!IsWindowEnabled())
		{	

			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));			
		}
		else
		{	

			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));			
		}    
	}
	rtItem.left += 5;
	if(m_uStyleEx & LVS_EX_CHECKBOXES) //注意画CheckBox
	{ 
		//Check				
		pSubItem ->rtCheck = CRect(rtItem.left  ,rtItem.top +(rtItem.Height() -  m_pCheckBox ->Height())/2,0,0);
		pSubItem ->rtCheck.right  = pSubItem ->rtCheck.left + m_pCheckBox ->Width();
		pSubItem ->rtCheck.bottom = pSubItem ->rtCheck.top  + m_pCheckBox ->Height();
		if(rtItem.Width() > m_pCheckBox ->Width())
		{
			ImageList_Draw(m_pCheckBox ->GetImage(),pItem ->bCheck  ? 0:1 ,pDC ->m_hDC,pSubItem ->rtCheck.left  ,pSubItem ->rtCheck.top,ILD_TRANSPARENT);
		}
		rtItem.left +=  m_pCheckBox ->Width() +5;
	}
	//Image
	if(pImgeList && pImgeList ->m_hImageList)
	{
		if(pSubItem ->iImage>=0 && pImgeList ->GetImageInfo(pSubItem ->iImage,&imageInfo))
		{		
			pSubItem ->rtIcon = CRect(rtItem.left  ,rtItem.top +(rtItem.Height() -  (imageInfo.rcImage.bottom - imageInfo.rcImage.top ))/2,0,0);
			pSubItem ->rtIcon.right  = pSubItem ->rtIcon.left + imageInfo.rcImage.right  - imageInfo.rcImage.left;
			pSubItem ->rtIcon.bottom = pSubItem ->rtIcon.top  + imageInfo.rcImage.bottom - imageInfo.rcImage.top;
			if(rtItem.Width() > imageInfo.rcImage.right - imageInfo.rcImage.left)
				ImageList_Draw(pImgeList->m_hImageList,pSubItem ->iImage ,pDC ->m_hDC,pSubItem ->rtIcon.left,pSubItem ->rtIcon.top,ILD_TRANSPARENT);
			rtItem.left +=  imageInfo.rcImage.right - imageInfo.rcImage.left + 5;
		}
		else
		{
			pSubItem ->rtIcon = CRect(0,0,0,0);
			pSubItem ->rtIcon.right  = pSubItem ->rtIcon.left + rtItem.Height();				
			rtItem.left += rtItem.Height() + 5;
		}	
	}
	else pSubItem ->rtIcon = CRect(0,0,0,0);

	//Text 
	pSubItem ->rtLable = rtItem;
	pDC ->DrawText(pSubItem ->sText,rtItem,DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_LEFT|DT_NOPREFIX);
	pSubItem ->rtDrag  = pSubItem ->rtLable|pSubItem ->rtIcon;
	//Line
	if(m_uStyleEx & LVS_EX_GRIDLINES)
	{
		CPen *pPenSave = pDC ->SelectObject(CPen::FromHandle(m_pLinePen[0]->hPen));	
		pDC ->MoveTo(m_rtClient.left,rtItem.top); pDC ->LineTo(m_rtClient.right,rtItem.top);		
		pDC ->SelectObject(CPen::FromHandle(m_pLinePen[1]->hPen));
		pDC ->MoveTo(m_rtClient.left,rtItem.bottom-1); pDC ->LineTo(m_rtClient.right,rtItem.bottom-1);		
		pDC ->SelectObject(pPenSave);	
	}


}
void CTPListCtrlEx::DrawInfo(CDC *pDC,int iItem)
{
	TPListCtrlItem    *pItem     = m_aItem[iItem];
	TPListCtrlSubItem *pSubItem  = pItem ->aSubItem[0];

	CRect rtItem = pSubItem ->rtItem;

	CImageList *pImgeList = &m_imgNormal;
	IMAGEINFO   imageInfo;

	if(pItem ->uState & ( LVIS_FOCUSED |LVIS_SELECTED))
	{
		pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
		if(pItem ->uState & LVIS_FOCUSED)	    pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
		else                				pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));				
	}
	else
	{

		if(pItem ->iBackColor>0)          pDC ->FillSolidRect(rtItem,TP_GetSysColor(pItem ->iBackColor));
		else if(pSubItem ->iBackColor>0)  pDC ->FillSolidRect(rtItem,TP_GetSysColor(pItem ->iBackColor));	
		if(!IsWindowEnabled())
		{	

			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));			
		}
		else
		{	

			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));			
		}    
	}
	rtItem.left += 5;

	//Image
	if(pImgeList && pImgeList ->m_hImageList)
	{
		if(pSubItem ->iImage>=0 && pImgeList ->GetImageInfo(pSubItem ->iImage,&imageInfo))
		{		
			pSubItem ->rtIcon = CRect(rtItem.left  ,rtItem.top +(rtItem.Height() -  (imageInfo.rcImage.bottom - imageInfo.rcImage.top ))/2,0,0);
			pSubItem ->rtIcon.right  = pSubItem ->rtIcon.left + imageInfo.rcImage.right  - imageInfo.rcImage.left;
			pSubItem ->rtIcon.bottom = pSubItem ->rtIcon.top  + imageInfo.rcImage.bottom - imageInfo.rcImage.top;
			if(rtItem.Width() > imageInfo.rcImage.right - imageInfo.rcImage.left)
				ImageList_Draw(pImgeList->m_hImageList,pSubItem ->iImage ,pDC ->m_hDC,pSubItem ->rtIcon.left,pSubItem ->rtIcon.top,ILD_TRANSPARENT);
			rtItem.left +=  imageInfo.rcImage.right - imageInfo.rcImage.left + 5;
		}
		else
		{
			pSubItem ->rtIcon = CRect(0,0,0,0);
			pSubItem ->rtIcon.right  = pSubItem ->rtIcon.left + rtItem.Height();				
			rtItem.left += rtItem.Height() + 5;
		}	
	}
	else pSubItem ->rtIcon = CRect(0,0,0,0);
	//Text 
	pSubItem ->rtLable = rtItem;
	pDC ->DrawText(pSubItem ->sText,rtItem,DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_LEFT|DT_NOPREFIX);
	pSubItem ->rtDrag  = pSubItem ->rtLable|pSubItem ->rtIcon;
	//Line
	if(m_uStyleEx & LVS_EX_GRIDLINES)
	{
		CPen *pPenSave = pDC ->SelectObject(CPen::FromHandle(m_pLinePen[0]->hPen));	
		pDC ->MoveTo(m_rtClient.left,rtItem.top); pDC ->LineTo(m_rtClient.right,rtItem.top);		
		pDC ->SelectObject(CPen::FromHandle(m_pLinePen[1]->hPen));
		pDC ->MoveTo(m_rtClient.left,rtItem.bottom-1); pDC ->LineTo(m_rtClient.right,rtItem.bottom-1);		
		pDC ->SelectObject(pPenSave);	
	}
}
void CTPListCtrlEx::FunMoveColumn(int iSrcIndex,int iDestIndex)
{
	if(iDestIndex >=0 && iSrcIndex >=0 && iSrcIndex != iDestIndex)
	{			
		TPListCtrlColumn *pHeadSource = m_aColumn[iSrcIndex];
		m_aColumn.InsertAt(iDestIndex,pHeadSource);
		if(iSrcIndex > iDestIndex)				m_aColumn.RemoveAt(iSrcIndex+1); 
		else 	                                m_aColumn.RemoveAt(iSrcIndex); 
		for(int i=0;i<m_aItem.GetSize();i++)
		{
			TPListCtrlSubItem *pItemData = m_aItem[i]->aSubItem[iSrcIndex];
			m_aItem[i]->aSubItem.InsertAt(iDestIndex,pItemData); 
			if(iSrcIndex> iDestIndex)				m_aItem[i]->aSubItem.RemoveAt(iSrcIndex+1); 
			else 	                                m_aItem[i]->aSubItem.RemoveAt(iSrcIndex); 
		}
		ReLayOut();					
		ChangeColume(2);
	}
}

void CTPListCtrlEx::FunDrawLine(CDC *pDC,CRect rtItem)
{
	CPen *pPenSave = pDC ->SelectObject(CPen::FromHandle(m_pLinePen[0]->hPen));	
	pDC ->MoveTo(m_rtClient.left,rtItem.top); pDC ->LineTo(m_rtClient.right,rtItem.top);		
	pDC ->SelectObject(CPen::FromHandle(m_pLinePen[1]->hPen));
	pDC ->MoveTo(m_rtClient.left,rtItem.bottom-1); pDC ->LineTo(m_rtClient.right,rtItem.bottom-1);		
	pDC ->SelectObject(pPenSave);

}
void CTPListCtrlEx::DrawTextPicture(CDC *pDC,int iItem)
{
}
void CTPListCtrlEx::DrawReport(CDC *pDC,int iItem)
{
	TPListCtrlItem    *pItem     = m_aItem[iItem];
	TPListCtrlSubItem *pSubItem  = NULL;
	int iColumnCount      = (int) m_aColumn.GetSize();	
	if(iColumnCount<=0) return;
	if(!pItem ->bShow) return;

	IMAGEINFO   imageInfo;
	CImageList *pImgeList = &m_imgNormal; 


	CRect rtItem = pItem ->aSubItem[0]->rtItem;	
	if(pItem ->uState & ( LVIS_FOCUSED |LVIS_SELECTED))
	{		
		pDC ->FillSolidRect(CRect(m_rtClient.left,rtItem.top,m_rtClient.right,rtItem.bottom) , TP_GetSysColor(COLOR_BASE_SELECTITEMA));
		if(pItem ->uState & LVIS_FOCUSED)	pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
		else        			        	pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT));				
	}
	else if(!IsWindowEnabled())
	{		
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));			
	}
	else
	{	
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));			
	}
	for(int l = iColumnCount - 1 ;l >=0;l --)
	{
		if(!m_aColumn[l]->bShow) continue;
		pSubItem = pItem ->aSubItem[l];
		rtItem   = pSubItem->rtItem;		
		//画背景
		if(pItem ->uState & ( LVIS_FOCUSED |LVIS_SELECTED)) pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_SELECTITEMA));
		else 
		{
			if(pItem ->iBackColor>=0)          pDC ->FillSolidRect(rtItem,TP_GetSysColor(pItem ->iBackColor));
			else if(pSubItem ->iBackColor>=0)  pDC ->FillSolidRect(rtItem,TP_GetSysColor(pItem ->iBackColor));
			else
			{
				if( l%2==1)               
				{
					if(m_dwWindowLevel == TPWNDLEVEL1)		pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_FRAME));
					else                              		pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_FRAME2));
				}
				else
				{
					if(m_dwWindowLevel == TPWNDLEVEL1)		pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
					else                              		pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW2));				
				}
			}
		}
		rtItem.left += 5;
		if((m_uStyleEx & LVS_EX_CHECKBOXES) && m_aColumn[l]->iIndexInit==0) //注意画CheckBox
		{ 
			//Check				
			pSubItem ->rtCheck = CRect(rtItem.left  ,rtItem.top +(rtItem.Height() -  m_pCheckBox ->Height())/2,0,0);
			pSubItem ->rtCheck.right  = pSubItem ->rtCheck.left + m_pCheckBox ->Width();
			pSubItem ->rtCheck.bottom = pSubItem ->rtCheck.top  + m_pCheckBox ->Height();
			if(rtItem.Width() > m_pCheckBox ->Width())
			{
				ImageList_Draw(m_pCheckBox ->GetImage(),pItem ->bCheck  ? 0:1 ,pDC ->m_hDC,pSubItem ->rtCheck.left  ,pSubItem ->rtCheck.top,ILD_TRANSPARENT);
			}
			rtItem.left +=  m_pCheckBox ->Width() +5;
		}
		//Image		
		if(m_aColumn[l]->iIndexInit  != 0 && pSubItem ->hImagList.m_hImageList)  pImgeList = &pSubItem ->hImagList;

		if(pImgeList && pImgeList ->m_hImageList && pSubItem ->iImage>=0 && pImgeList ->GetImageInfo(pSubItem ->iImage,&imageInfo))
		{
			pSubItem ->rtIcon = CRect(rtItem.left  ,rtItem.top +(rtItem.Height() -  (imageInfo.rcImage.bottom - imageInfo.rcImage.top ))/2,0,0);
			pSubItem ->rtIcon.right  = pSubItem ->rtIcon.left + imageInfo.rcImage.right  - imageInfo.rcImage.left;
			pSubItem ->rtIcon.bottom = pSubItem ->rtIcon.top  + imageInfo.rcImage.bottom - imageInfo.rcImage.top;
			if(rtItem.Width() > imageInfo.rcImage.right - imageInfo.rcImage.left)
				ImageList_Draw(pImgeList->m_hImageList,pSubItem ->iImage ,pDC ->m_hDC,pSubItem ->rtIcon.left,pSubItem ->rtIcon.top,ILD_TRANSPARENT);
			rtItem.left +=  imageInfo.rcImage.right - imageInfo.rcImage.left + 5;
		}
		else if(m_aColumn[l]->iIndexInit  == 0 && pSubItem ->iImage>=0)
		{
			pSubItem ->rtIcon = rtItem;
			pSubItem ->rtIcon.right  = pSubItem ->rtIcon.left + rtItem.Height();				
			rtItem.left += rtItem.Height() + 5;
		}	
		else pSubItem ->rtIcon = CRect(0,0,0,0);		
		//Text 
		if(rtItem.left < rtItem.right)
		{
			UINT uFromat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS|DT_NOPREFIX;
			UINT    uFmt = m_aColumn[l] ->uFmt;			
			if(uFmt & HDF_CENTER)       uFromat |= DT_CENTER; 
			else if(uFmt & HDF_RIGHT)   uFromat |= DT_RIGHT;
			else                        uFromat |= DT_LEFT;

			pDC ->DrawText(pSubItem ->sText,rtItem,uFromat);
			pSubItem ->rtLable = rtItem;
		}
		pSubItem ->rtDrag  = pSubItem ->rtLable|pSubItem ->rtIcon;
	}	
	if(m_uStyleEx & LVS_EX_GRIDLINES)
	{	
		CPen *pPenSave = pDC ->SelectObject(CPen::FromHandle(m_pLinePen[0]->hPen));	
		pDC ->MoveTo(m_rtClient.left,rtItem.top); pDC ->LineTo(m_rtClient.right,rtItem.top);		
		pDC ->SelectObject(CPen::FromHandle(m_pLinePen[1]->hPen));
		pDC ->MoveTo(m_rtClient.left,rtItem.bottom-1); pDC ->LineTo(m_rtClient.right,rtItem.bottom-1);		
		pDC ->SelectObject(pPenSave);
	}
}
void CTPListCtrlEx::SetColumnFmt(int iColumn,UINT uFlagAdd,UINT uFlagRemove)
{
	if(iColumn <0 || iColumn >= m_aColumn.GetSize()) return;
	for(INT l=0;l<m_aColumn.GetSize();l++) m_aColumn[l] ->uFmt &= ~uFlagRemove;
	TPListCtrlColumn *pColumn = m_aColumn[iColumn];	
	pColumn ->uFmt |=  uFlagAdd;
}
void CTPListCtrlEx::DrawColumn(CDC *pDC,int iColumn)
{
	int iIndex = 0;
	TPListCtrlColumn *pColumn = m_aColumn[iColumn];

	if(m_iHeadDown ==iColumn)    
	{
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
		iIndex = 3;
	}
	else if(m_iHeadOver == iColumn)
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
	TP_StretchDIBRect(pDC,m_pHeaderBmp[iIndex] ->GetByte(),m_pHeaderBmp[iIndex]->Width(),m_pHeaderBmp[iIndex]->Height(),10,10,pColumn ->rtItem,FALSE,FALSE,2);

	int iLeft = pColumn->rtItem.left + 5 ,iRight = pColumn->rtItem.right - 5;
	if(m_imgHead.m_hImageList && pColumn ->iImage >=0)
	{			
		IMAGEINFO imageInfo;
		if(m_imgHead.GetImageInfo( pColumn ->iImage,&imageInfo))
		{
			//画图
			if(iLeft  +(imageInfo.rcImage.right - imageInfo.rcImage.left) <iRight)
			{					
				int iImageTop  = max(pColumn->rtItem.top,pColumn->rtItem.top + (pColumn->rtItem.Height() - (imageInfo.rcImage.bottom - imageInfo.rcImage.top))/2);
				int iImageLeft = iLeft;
				m_imgHead.Draw(pDC,pColumn ->iImage,CPoint(iImageLeft,iImageTop),ILD_TRANSPARENT);
			}
			iLeft  += imageInfo.rcImage.right - imageInfo.rcImage.left + 2;

		}
	}
	else iLeft += pColumn->rtItem.Height();

	if(pColumn-> uFmt & (HDF_SORTDOWN | HDF_SORTUP))
	{
		CRect rtTemp = pColumn->rtItem;
		rtTemp.right = iRight;
		rtTemp.left = rtTemp.right - m_pHeaderBmp[4] ->Width();
		if(rtTemp.left > iLeft)
		{
			ImageList_Draw(m_pHeaderBmp[4] ->GetImage(),(pColumn-> uFmt & HDF_SORTDOWN)? 0:1,pDC ->m_hDC,rtTemp.left ,rtTemp.top +(rtTemp.Height() -  m_pHeaderBmp[4]  ->Height())/2,ILD_TRANSPARENT	);
		}
		iRight -= m_pHeaderBmp[4] ->Width() + 2;			
	}
	if(pColumn-> uFmt & HDF_STRING)
	{
		CRect rtTemp = pColumn->rtItem;
		rtTemp.left  = iLeft;
		rtTemp.right = iRight;
		if(rtTemp.left <rtTemp.right)
		{ 
			UINT uFormat = DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_NOPREFIX;
			if(pColumn->uFmt & HDF_LEFT)        uFormat |= DT_LEFT;
			else if(pColumn->uFmt & HDF_RIGHT)  uFormat |= DT_RIGHT;
			else if(pColumn->uFmt & HDF_CENTER) uFormat |= DT_CENTER;
			pDC ->DrawText(pColumn ->sItem,rtTemp,uFormat);	
		}
	}
	else if(pColumn-> uFmt& HDF_BITMAP)
	{
	}
}
void CTPListCtrlEx::PaintHead(CDC *pDC)
{	
	int iColumn  = (int)m_aColumn.GetSize();
	int iHeadEnd = m_rtClient.left;
	for(int l = iColumn - 1;l>=0;l--)
	{	
		if(m_aColumn[l]->rtItem.right <=0 && l>m_iLockColumn) continue;
		if(m_aColumn[l]->rtItem.left >m_rtClient.right || !m_aColumn[l]->bShow) continue;
		DrawColumn(pDC,l);
		iHeadEnd = max(m_aColumn[l]->rtItem.right,iHeadEnd);
	}
	if(iHeadEnd <m_rtClient.right && !(LIST_OWNHEAD & m_uPrivateStyle)) 
		TP_StretchDIBRect(pDC,m_pHeaderBmp[2] ->GetByte(),m_pHeaderBmp[2]->Width(),m_pHeaderBmp[2]->Height(),5,5,CRect(iHeadEnd,0,m_rtClient.right,m_iColumnHeight),FALSE);
	if(m_iDragIndex >=0 && m_iDragMode == TP_DRAG_MIDDLEOK)
	{
		CRect rtLine = CRect(0,0,0,0);
		INT nIndex = (int)m_aColumn.GetSize() - 1;
		for(; nIndex > 0; nIndex--)
			if(m_aColumn[nIndex]->bShow) break;

		INT iDragIndex = m_iDragIndex;
		if (iDragIndex == m_aColumn.GetSize()) iDragIndex--;
		BOOL bLeft = FALSE;
		if(m_aColumn[iDragIndex]->bShow) bLeft = TRUE;
		for(; iDragIndex > 0; iDragIndex--)
			if(m_aColumn[iDragIndex]->bShow) break;

		
		if(m_iDragIndex > nIndex) rtLine = CRect(m_aColumn[nIndex] ->rtItem.right,0,0,0);
		else                                    
		{
			if(bLeft) rtLine = CRect(m_aColumn[iDragIndex] ->rtItem.left ,0,0,0);
			else      rtLine = CRect(m_aColumn[iDragIndex] ->rtItem.right ,0,0,0);
		}
		CPen pLine(PS_SOLID,2,RGB(0,0,255));
		CPen *pPen = pDC ->SelectObject(&pLine);		
		pDC ->MoveTo(rtLine.left,0);pDC->LineTo(rtLine.left,m_iColumnHeight);
		pDC ->SelectObject(pPen);
	}
}
void CTPListCtrlEx::PaintInvert(CRectArray &aRectInvert)
{
	if(m_aInvertRect.GetSize() ==aRectInvert.GetSize())
	{
		INT l=0;
		for(l=0;l<m_aInvertRect.GetSize();l++)
		{
			if(m_aInvertRect[l] != aRectInvert[l]) break;
		}
		if(l>=m_aInvertRect.GetSize()) return;
	}
	m_aInvertRect.Copy(aRectInvert);
	m_bReDraw = TRUE;
	InvalidateRect(NULL);
	//UpdateWindow();
	return;
	
	CDC  *pDC   = GetDC();	
	int   iMode = pDC ->SetROP2(R2_XORPEN); 
	CPen  penRead(PS_SOLID,2,RGB(98,139,139));
	CPen *pPen  = (CPen *)pDC ->SelectObject(&penRead);
	for(INT l = (INT)m_aInvertRect.GetSize()-1;l>=0;l--)
	{
		INT k = 0;
		for(k = (INT)aRectInvert.GetSize() -1;k>=0;k--)
		{
			if(m_aInvertRect[l] == aRectInvert[k])
			{
				aRectInvert.RemoveAt(k);				
				break;
			}
		}
		if(k<0)
		{
			pDC ->MoveTo(m_aInvertRect[l].left,m_aInvertRect[l].top);   pDC->LineTo(m_aInvertRect[l].left,m_aInvertRect[l].bottom);
			pDC->LineTo(m_aInvertRect[l].right,m_aInvertRect[l].bottom);pDC->LineTo(m_aInvertRect[l].right,m_aInvertRect[l].top);
			pDC->LineTo(m_aInvertRect[l].left,m_aInvertRect[l].top);
			m_aInvertRect.RemoveAt(l);
		}
	}	
	for(INT l=0;l<aRectInvert.GetSize();l++)
	{	
		pDC ->MoveTo(aRectInvert[l].left,aRectInvert[l].top);   pDC->LineTo(aRectInvert[l].left,aRectInvert[l].bottom);
		pDC->LineTo(aRectInvert[l].right,aRectInvert[l].bottom);pDC->LineTo(aRectInvert[l].right,aRectInvert[l].top);
		pDC->LineTo(aRectInvert[l].left,aRectInvert[l].top);		
	}
	m_aInvertRect.Append(aRectInvert);	
	pDC ->SetROP2(iMode); 
	pDC ->SelectObject(pPen);
	ReleaseDC(pDC);
}
void CTPListCtrlEx::PaintDC(CDC *pDC)
{
	if(!m_bReDraw) return;
	if(pDC == NULL){ InvalidateRect(NULL);UpdateWindow();return; }
	CRect rtClient; 
	BOOL  bReleaseDC = FALSE;
	if(pDC == NULL) { pDC = GetDC(); bReleaseDC = TRUE; }
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());	
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));

	m_rtInvert = CRect(0,0,0,0);

	//m_aInvertRect.RemoveAll();

	if(m_iViewType == VIEWTYPE_REPORT || m_iViewType == VIEWTYPE_TEXTPICTURE)
	{
		if(m_aColumn.GetSize()>=0)
		{		
			for(INT l=0;l<m_aItem.GetSize();l++)
			{
				if(!m_aItem[l]->bShow)										  continue;										
				if(m_aItem[l]->aSubItem[0] ->rtItem.bottom <= m_iColumnHeight) continue;
				else if(m_aItem[l]->aSubItem[0] ->rtItem.top >= rtClient.bottom) break;
				if(m_iViewType == VIEWTYPE_TEXTPICTURE)		    DrawTextPicture(&cdcMemo,l);
				else                                      		DrawReport(&cdcMemo,l);
			}
			if(TP_DRAG_ITEMOK == m_iDragMode && m_iDragIndex >=0) //拖动插入位置
			{
				CRect rtItem = m_rtClient;
				if(m_iDragIndex >=m_aItem.GetSize()) rtItem.top = m_aItem[m_aItem.GetSize()-1] ->aSubItem[0] ->rtItem.bottom;
				else rtItem.top = m_aItem[m_iDragIndex] ->aSubItem[0] ->rtItem.top;
				CPen pen(PS_SOLID,1,RGB(0,0,255)),*pSavePen = cdcMemo.SelectObject(&pen);
				cdcMemo.MoveTo(rtItem.left,rtItem.top);cdcMemo.LineTo(rtItem.right,rtItem.top);
				cdcMemo.SelectObject(pSavePen);				
			}
		}
		PaintHead(&cdcMemo);
	}
	else if(m_iViewType == VIEWTYPE_LIST)
	{
		if(m_aColumn.GetSize()>=0)
		{		
			for(INT l=0;l<m_aItem.GetSize();l++)
			{
				if(m_aItem[l]->aSubItem[0] ->rtItem.bottom <= m_rtClient.top) continue;
				else if(m_aItem[l]->aSubItem[0] ->rtItem.top >= rtClient.bottom) break;
				DrawList(&cdcMemo,l);
			}
			if(TP_DRAG_ITEMOK == m_iDragMode && m_iDragIndex >=0) //拖动插入位置
			{
				CRect rtItem = m_rtClient;
				if(m_iDragIndex >=m_aItem.GetSize()) 
				{
					rtItem     = m_aItem[m_aItem.GetSize()-1] ->aSubItem[0] ->rtItem;
					rtItem.top = m_aItem[m_aItem.GetSize()-1] ->aSubItem[0] ->rtItem.bottom;
				}
				else
				{
					rtItem     = m_aItem[m_iDragIndex] ->aSubItem[0] ->rtItem;
					rtItem.top = m_aItem[m_iDragIndex] ->aSubItem[0] ->rtItem.top;
				}
				CPen pen(PS_SOLID,1,RGB(0,0,255)),*pSavePen = cdcMemo.SelectObject(&pen);
				cdcMemo.MoveTo(rtItem.left,rtItem.top);cdcMemo.LineTo(rtItem.right,rtItem.top);
				cdcMemo.SelectObject(pSavePen);				
			}
		}
	}
	else if(m_iViewType == VIEWTYPE_INFO)
	{
		if(m_aColumn.GetSize()>=0)
		{		
			for(INT l=0;l<m_aItem.GetSize();l++)
			{
				if(m_aItem[l]->aSubItem[0] ->rtItem.bottom <= m_rtClient.top) continue;
				else if(m_aItem[l]->aSubItem[0] ->rtItem.top >= rtClient.bottom) break;
				DrawInfo(&cdcMemo,l);
			}
			if(TP_DRAG_ITEMOK == m_iDragMode && m_iDragIndex >=0) //拖动插入位置
			{
				CRect rtItem = m_rtClient;
				if(m_iDragIndex >=m_aItem.GetSize()) 
				{
					rtItem     = m_aItem[m_aItem.GetSize()-1] ->aSubItem[0] ->rtItem;
					rtItem.top = m_aItem[m_aItem.GetSize()-1] ->aSubItem[0] ->rtItem.bottom;
				}
				else
				{
					rtItem     = m_aItem[m_iDragIndex] ->aSubItem[0] ->rtItem;
					rtItem.top = m_aItem[m_iDragIndex] ->aSubItem[0] ->rtItem.top;
				}
				CPen pen(PS_SOLID,1,RGB(0,0,255)),*pSavePen = cdcMemo.SelectObject(&pen);
				cdcMemo.MoveTo(rtItem.left,rtItem.top);cdcMemo.LineTo(rtItem.right,rtItem.top);
				cdcMemo.SelectObject(pSavePen);				
			}
		}
	}
	else if(m_iViewType == VIEWTYPE_BUFFER || m_iViewType == VIEWTYPE_ICON)
	{
		if(m_aColumn.GetSize()>=0)
		{		
			for(INT l=0;l<m_aItem.GetSize();l++)
			{
				if(!m_aItem[l]->bShow)										  continue;
				else if(m_aItem[l]->bShow !=TP_LIST_COLNUMMAX)
				{
					if(m_aItem[l]->aSubItem[0] ->rtItem.bottom <= m_rtClient.top)    continue;
					else if(m_aItem[l]->aSubItem[0] ->rtItem.top >= rtClient.bottom) continue;
				}
				if(m_iViewType == VIEWTYPE_BUFFER)		    DrawBuffer(&cdcMemo,l);
				else if (m_iViewType == VIEWTYPE_ICON)		DrawIcon(&cdcMemo,l);
			}
		}		
	}

	if(m_aInvertRect.GetSize()>0)
	{	
		CPen  penRead(PS_SOLID,2,RGB(255,0,0));
		CPen *pPen  = (CPen *)cdcMemo.SelectObject(&penRead);	
		for(INT l=0;l<m_aInvertRect.GetSize();l++)
		{	
			cdcMemo.MoveTo(m_aInvertRect[l].left,m_aInvertRect[l].top);   cdcMemo.LineTo(m_aInvertRect[l].left,m_aInvertRect[l].bottom);
			cdcMemo.LineTo(m_aInvertRect[l].right,m_aInvertRect[l].bottom);cdcMemo.LineTo(m_aInvertRect[l].right,m_aInvertRect[l].top);
			cdcMemo.LineTo(m_aInvertRect[l].left,m_aInvertRect[l].top);		
		}
		cdcMemo.SelectObject(pPen);
	}


	if(m_pDragImageList && m_bDragInWnd) m_pDragImageList ->DragLeave(NULL);
	pDC->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
	if(m_pDragImageList && m_bDragInWnd)
	{
		CPoint ptCursor;::GetCursorPos(&ptCursor);
		//ScreenToClient(&ptCursor);
		m_pDragImageList ->DragEnter(NULL,ptCursor);
	}
	if(bReleaseDC) ReleaseDC(pDC);
	//m_pDragImageList ->Draw()
	/*if(m_pDragImageList)
	{
		CPoint ptCursor;::GetCursorPos(&ptCursor);
		m_pDragImageList ->DragMove(ptCursor);
	}*/
}
void CTPListCtrlEx::FunCalcHeadSize()
{
	if(m_iViewType != VIEWTYPE_REPORT &&  m_iViewType != VIEWTYPE_TEXTPICTURE) return;	
	int iSize = (int)m_aColumn.GetSize(),iCX = 0;	
	for(int i=0;i<iSize;i++)
	{
		TPListCtrlColumn *pItem =m_aColumn[i]; 
		pItem->iIndex = i;
		if(pItem->bShow) 
		{
			pItem ->iWidth = max(pItem ->iWidth,pItem ->lMinWidth);
			pItem ->iWidth = min(pItem ->iWidth,pItem ->lMaxWidth);
			if(i>m_iLockColumn) 
				pItem ->rtItem = CRect(iCX - m_iTopX,0,iCX + pItem ->iWidth - m_iTopX ,m_iColumnHeight);
			else
				pItem ->rtItem = CRect(iCX,0,iCX + pItem ->iWidth,m_iColumnHeight);
			pItem ->CalcSize();
			iCX += pItem->iWidth;  
		}
		else pItem ->rtItem = CRect(0,0,0,0);
	}
	m_iTotalWidth = iCX;
}
void CTPListCtrlEx::FunReSortItem()
{	
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		for(int k = l+1;k<m_aItem.GetSize();k++)
		{
			if(m_aItem[k] ->aSubItem[0] ->rtItem.left < m_aItem[l] ->aSubItem[0] ->rtItem.left||
				m_aItem[k] ->aSubItem[0] ->rtItem.top  < m_aItem[l] ->aSubItem[0] ->rtItem.top )
			{
				TPListCtrlItem *pItem = m_aItem[k];
				m_aItem[k]            = m_aItem[l];
				m_aItem[l]            = pItem; 
			}
		}
		m_aItem[l]->iIndex = l;
	}
}
void CTPListCtrlEx::FunCalcSize()
{
	FunCalcHeadSize();
	FunCalcItemSize();
	FunCalcScrollBar();
	FunCalcEditItem();
}

void CTPListCtrlEx::FunCalcItemSize()
{
	if(!m_bSubWindow) return;
	CRect rtClient;
	GetClientRect(&rtClient);
	BOOL  iWidthCheck  = (m_uState & STATE_SCROLL) ?m_iWidthCheck:rtClient.right;
	long  lToolIndex   = 1;
	if(m_iViewType == VIEWTYPE_REPORT || m_iViewType == VIEWTYPE_TEXTPICTURE)
	{
		int   iItemSize    = (int)m_aItem.GetSize(),iCY = m_iColumnHeight;
		int   iColumnSize  = (int)m_aColumn.GetSize();		
		CRect rtItem = CRect(0,0,0,0);
		for(int i=0;i<iItemSize;i++)
		{
			TPListCtrlItem *pItem =m_aItem[i]; 
			pItem ->iIndex = i;
			if(!pItem ->bShow)
			{
				pItem->EmptySubItem();
				continue;			
			}
			int iHeigth = pItem->iItemHeight;
			if(iHeigth <= 0)
			{
				if(m_iViewType == VIEWTYPE_TEXTPICTURE)	iHeigth = m_szItemTextPicture.cy;
				else									iHeigth = m_szItemReport.cy;
			}

			rtItem = CRect(0,iCY - m_iTopY,0,iCY + iHeigth - m_iTopY);	
			CRect rtItemAll = CRect(0, 0, 0, 0);
			for(INT k=0;k<iColumnSize;k++)
			{
				rtItem .left  = m_aColumn[k]->rtItem.left;
				rtItem .right = m_aColumn[k]->rtItem.right;
				rtItemAll |= rtItem;
				pItem ->aSubItem[k] ->rtItem = rtItem;
				pItem ->aSubItem[k] ->CalcSize();
			}
			if(rtClient.PtInRect(rtItemAll.BottomRight()) || rtClient.PtInRect(rtItemAll.TopLeft()))
				m_pToolTipCtrl->SetToolRect(this,lToolIndex++,rtItemAll);
			iCY += iHeigth;
		}
		m_iTotalHeight = iCY + m_szItemReport.cy;
	}
	else if(m_iViewType == VIEWTYPE_INFO)
	{
		GetClientRect(&m_rtClient);
		int   iItemSize    = (int)m_aItem.GetSize(),iCY = 0,iCx = -m_iTopX;
		CRect rtItem = CRect(0,0,0,0);
		m_iTotalHeight = 0;
		m_iTotalWidth  = 0;
		for(int i=0;i<iItemSize;i++)
		{
			TPListCtrlItem *pItem =m_aItem[i]; 
			pItem ->iIndex = i;
			if(!pItem ->bShow)
			{
				pItem->EmptySubItem();
				continue;
			}
			rtItem       = CRect(0-m_iTopX,iCY - m_iTopY,max(m_rtClient.right-15 -m_iTopX,300 -m_iTopX),iCY + m_szItemInfo.cy - m_iTopY);
			iCY += m_szItemInfo.cy;
			pItem ->aSubItem[0] ->rtItem= rtItem;	
			if(rtClient.PtInRect(rtItem.BottomRight()) || rtClient.PtInRect(rtItem.TopLeft()))
				m_pToolTipCtrl->SetToolRect(this,lToolIndex++,rtItem);
		}
		m_iTotalWidth  = m_rtClient.Width();
		m_iTotalHeight = iCY + m_szItemInfo.cy;
	}	
	else if(m_iViewType == VIEWTYPE_LIST)
	{
		//m_iCountPerLine = 0;
		GetClientRect(&m_rtClient);
		int   iItemSize    = (int)m_aItem.GetSize(),iCY = 0,iCx = -m_iTopX;

		CRect rtItem = CRect(0,0,0,0);
		m_iTotalHeight = 0;
		m_iTotalWidth  = 0;
		for(int i=0;i<iItemSize;i++)
		{
			TPListCtrlItem *pItem =m_aItem[i]; 
			pItem ->iIndex = i;
			if(!pItem ->bShow) 
			{
				pItem->EmptySubItem();
				continue;
			}
			rtItem = CRect(iCx,iCY - m_iTopY,iCx + m_szItemList.cx,iCY + m_szItemList.cy - m_iTopY);
			iCx += m_szItemList.cx + 5;
			if(iCx + m_szItemList.cx > iWidthCheck)
			{
				iCx = -m_iTopX;
				iCY += m_szItemList.cy;
				//if(m_iCountPerLine ==0) m_iCountPerLine = i;
			}
			pItem ->aSubItem[0] ->rtItem= rtItem;
			if(rtClient.PtInRect(rtItem.BottomRight()) || rtClient.PtInRect(rtItem.TopLeft()))
				m_pToolTipCtrl->SetToolRect(this,lToolIndex++,rtItem);
			m_iTotalWidth = max(iCx,m_iTotalWidth);
		}
		m_iTotalHeight = iCY+m_szItemList.cy;
	}
	else if(m_iViewType == VIEWTYPE_ICON || m_iViewType == VIEWTYPE_BUFFER)
	{
		//m_iCountPerLine = 0;
		if(!(m_uState & STATE_DRAGITEM) || m_iViewType == VIEWTYPE_BUFFER)
		{	
			//add by qushaojie 20090806
			if(m_iArrType == ARRANGE_FREE)
			{			
				CSize szIcon = m_szItemBuffer;
				if(m_iViewType == VIEWTYPE_ICON) szIcon = m_szItemIcon;
				GetClientRect(&m_rtClient);
				int   iItemSize    = (int)m_aItem.GetSize(),iCY = m_szEdge.cy,iCx = m_szEdge.cx-m_iTopX;
				CRect rtItem = CRect(0,0,0,0);
				m_iTotalHeight = 0;
				m_iTotalWidth  = 0;
				for(int i=0;i<iItemSize;i++)
				{
					TPListCtrlItem *pItem =m_aItem[i]; 
					pItem ->iIndex = i;
					if(!pItem ->bShow)
					{
						pItem->EmptySubItem();
						continue;
					}
					rtItem = CRect(iCx,iCY - m_iTopY,iCx + szIcon.cx,iCY + szIcon.cy - m_iTopY);
					iCx += szIcon.cx + m_szSpace.cx;
					if(iCx + szIcon.cx > iWidthCheck)
					{
						iCx = m_szEdge.cx-m_iTopX;
						iCY += szIcon.cy + m_szSpace.cy;
						//if(m_iCountPerLine ==0) m_iCountPerLine = i;
					}
					pItem ->aSubItem[0] ->rtItem= rtItem;
					if((rtClient.PtInRect(rtItem.BottomRight()) || rtClient.PtInRect(rtItem.TopLeft())) && m_pToolTipCtrl->GetSafeHwnd())
						m_pToolTipCtrl->SetToolRect(this,lToolIndex++,rtItem);
					m_iTotalWidth = max(iCx,m_iTotalWidth);
				}
				m_iTotalHeight = iCY +szIcon.cy;
			}


			else if(m_iArrType == ARRANGE_AUTO)
			{
				CSize szIcon = m_szItemBuffer;
				if(m_iViewType == VIEWTYPE_ICON) szIcon = m_szItemIcon;
				GetClientRect(&m_rtClient);
				int   iItemSize    = (int)m_aItem.GetSize(),iCY = m_szEdge.cy,iCx = m_szEdge.cx-m_iTopX;
				CRect rtItem = CRect(0,0,0,0);
				m_iTotalHeight = 0;
				m_iTotalWidth  = 0;
				for(int i=0;i<iItemSize;i++)
				{
					TPListCtrlItem *pItem =m_aItem[i]; 
					pItem ->iIndex = i;
					if(!pItem ->bShow)
					{
						pItem->EmptySubItem();
						continue;
					}
					rtItem = CRect(iCx,iCY - m_iTopY,iCx + szIcon.cx,iCY + szIcon.cy - m_iTopY);
					iCx += szIcon.cx + m_szSpace.cx;
					if(iCx + szIcon.cx > iWidthCheck)
					{
						iCx = m_szEdge.cx-m_iTopX;
						iCY += szIcon.cy + m_szSpace.cy;
						//if(m_iCountPerLine ==0) m_iCountPerLine = i;
					}
					CRect rtLabel = rtItem;
					pItem ->aSubItem[0] ->rtItem= rtItem;
					rtLabel.DeflateRect(6, 0);
					// li song ming 2009-12-23 DQAdb00008800.
					rtLabel.top = rtLabel.bottom - 25;
					pItem ->aSubItem[0] ->rtLable = rtLabel;
					if((rtClient.PtInRect(rtItem.BottomRight()) || rtClient.PtInRect(rtItem.TopLeft())) && m_pToolTipCtrl->GetSafeHwnd())
						m_pToolTipCtrl->SetToolRect(this,lToolIndex++,rtItem);
					m_iTotalWidth = max(iCx,m_iTotalWidth);
				}
				m_iTotalHeight = iCY +szIcon.cy;
			}
			else if(m_iArrType == ARRANGE_STORYBOARD)
			{
				//add by qushaojie 20090803 
				CRect rtItem = CRect(0,0,0,0);
				m_iTotalWidth=0;
				m_iTotalHeight = 0;
				for (int i=0;i<m_aItem.GetSize();i++)
				{
					TPListCtrlItem *pItem =m_aItem[i]; 
					pItem ->iIndex = i;
					if(!pItem ->bShow)
					{
						pItem->EmptySubItem();
						continue;
					}
					long lIconIndex1 = pItem ->lIconIndex;
					ASSERT(lIconIndex1 >= 0);
					rtItem = FunItemRectByIndex(lIconIndex1);
					pItem->aSubItem[0] ->rtItem= rtItem;
					if (rtItem.right>m_iTotalWidth)    m_iTotalWidth = rtItem.right;
					if (rtItem.bottom>m_iTotalHeight)  m_iTotalHeight = rtItem.bottom;
					CRect rtLabel = rtItem;
					rtLabel.DeflateRect(6, 0);
					// li song ming 2009-12-23 DQAdb00008800.
					rtLabel.top = rtLabel.bottom - 25;
					pItem->aSubItem[0] ->rtLable = rtLabel;
					if((rtClient.PtInRect(rtItem.BottomRight()) || rtClient.PtInRect(rtItem.TopLeft())) && m_pToolTipCtrl->GetSafeHwnd())
						m_pToolTipCtrl->SetToolRect(this,lToolIndex++,rtItem);
				}				
			}
		}
		else
		{			
			for(int i=0;i<m_aItem.GetSize();i++)
			{
				TPListCtrlItem *pItem =m_aItem[i]; 
				pItem ->iIndex = i;
				if(!pItem ->bShow)
				{
					pItem->EmptySubItem();
					continue;
				}
				pItem ->aSubItem[0]->rtItem = pItem ->aSubItem[0]->rtItemSave;
				pItem ->aSubItem[0]->rtItem.OffsetRect(-m_iTopX,-m_iTopY);	
				CRect rtItem = pItem ->aSubItem[0]->rtItem;
				if(rtClient.PtInRect(rtItem.BottomRight()) || rtClient.PtInRect(rtItem.TopLeft()))
					m_pToolTipCtrl->SetToolRect(this,lToolIndex++,rtItem);
			}			
		}
	}
	m_iTotalHeight += 15;
	m_iWidthCheck   = iWidthCheck;
}
//add by qushaojie 20090803 一行有6个图标，iCountPerLine值为5
int CTPListCtrlEx::FunPerLineItemCount(void)
{
	int iCountPerLine = 0;
	CRect rtItem = CRect(0,0,0,0);
	CSize szIcon = m_szItemBuffer;
	if(m_iViewType == VIEWTYPE_ICON) szIcon = m_szItemIcon;
	GetClientRect(&m_rtClient);
	iCountPerLine = (m_rtClient.right-m_rtClient.left)/(szIcon.cx+m_szSpace.cx)-1;
	if(m_iArrType == ARRANGE_STORYBOARD)
	{
		for (int i=0;i<m_aItem.GetSize();i++)
		{
			int iItemCount = m_aItem[i] ->lIconIndex;
			if (iItemCount%TP_LIST_COLNUMMAX>iCountPerLine) iCountPerLine = iItemCount%TP_LIST_COLNUMMAX;
		}
	}
	return iCountPerLine;
}
CRect CTPListCtrlEx::FunItemRectByIndex(long IconIndex)
{
	CSize szIcon = m_szItemBuffer;
	CRect rtItem = CRect(0,0,0,0);
	if(m_iViewType == VIEWTYPE_ICON) szIcon = m_szItemIcon;

	rtItem.left = (m_szEdge.cx-m_iTopX)+IconIndex%TP_LIST_COLNUMMAX*(szIcon.cx+m_szSpace.cx);
	rtItem.top = (m_szEdge.cy-m_iTopY)+IconIndex/TP_LIST_COLNUMMAX*(szIcon.cy+m_szSpace.cy);
	rtItem.right = (m_szEdge.cx-m_iTopX)+IconIndex%TP_LIST_COLNUMMAX*(szIcon.cx+m_szSpace.cx)+szIcon.cx;
	rtItem.bottom = (m_szEdge.cy-m_iTopY)+IconIndex/TP_LIST_COLNUMMAX*(szIcon.cy+m_szSpace.cy)+szIcon.cy;
	return rtItem;
}
void CTPListCtrlEx::FunSortExItem(TPListCtrlItemArray &aItem, DWORD dwMode)
{

}
void CTPListCtrlEx::FunSortbyIconIndex(void)
{
	if (m_iViewType == VIEWTYPE_ICON)
	{
		for (int i=0;i<m_aItem.GetSize();i++)
		{
			int iMinIconIndex =m_aItem[i]->lIconIndex;
			int iMinFlag      = -1;
			for (int j=i+1;j<m_aItem.GetSize();j++)
			{
				if (m_aItem.GetAt(j)->lIconIndex < iMinIconIndex)
				{
					iMinIconIndex = m_aItem.GetAt(j)->lIconIndex;
					iMinFlag  = j;
				}
			}
			if (iMinFlag >= 0)
			{
				TPListCtrlItem * listItemTemp  = NULL;
				listItemTemp = m_aItem.GetAt(iMinFlag);
				m_aItem.SetAt(iMinFlag,m_aItem.GetAt(i));
				m_aItem.SetAt(i,listItemTemp);
			}
		}
	}
	else
	{
		for (int i=0;i<m_aItem.GetSize();i++)
		{
			int iMinReportIndex =m_aItem[i]->lReportIndex;
			int iMinFlag      = -1;
			for (int j=i+1;j<m_aItem.GetSize();j++)
			{
				if (m_aItem.GetAt(j)->lReportIndex < iMinReportIndex)
				{
					iMinReportIndex = m_aItem.GetAt(j)->lReportIndex;
					iMinFlag  = j;
				}
			}
			if (iMinFlag >= 0)
			{
				TPListCtrlItem * listItemTemp  = NULL;
				listItemTemp = m_aItem.GetAt(iMinFlag);
				m_aItem.SetAt(iMinFlag,m_aItem.GetAt(i));
				m_aItem.SetAt(i,listItemTemp);
			}
		}
	}

}
//end qsj
void CTPListCtrlEx::FunCalcEditItem()
{
	if(m_pEditWnd)
	{	
		CRect rtClient,rtItem;
		GetClientRect(&rtClient);
		rtItem = GetItemEditRect(m_iItemSave,m_iItemSubSave);
		if(rtClient.PtInRect(rtItem.TopLeft()) && rtClient.PtInRect(rtItem.BottomRight()))
		{
			if(m_rtItemSubSave != rtItem)		m_pEditWnd ->MoveWindow(rtItem);
			m_rtItemSubSave = rtItem;
		}
		else EndEdit();
	}
}

void CTPListCtrlEx::FunCalcScrollBar()
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

	int iScrollLine = m_szItemReport.cy;
	if(m_iViewType == VIEWTYPE_REPORT || m_iViewType == VIEWTYPE_TEXTPICTURE)
	{	
		if(m_iViewType == VIEWTYPE_REPORT) iScrollLine = m_szItemReport.cy;
		else iScrollLine = m_szItemTextPicture.cy;
		m_iTotalWidth = 0;
		m_iTotalHeight = 0;
		if(m_aColumn.GetSize()>0)
		{
			for(int l=0;l<m_aColumn.GetSize();l++)
				m_iTotalWidth += m_aColumn[l]->iWidth;
			for(int l=0; l<m_aItem.GetSize();l++)
				m_iTotalHeight += m_aItem[l]->aSubItem[0]->rtItem.Height();
		}	
		m_iTotalHeight += m_iColumnHeight;

		if(m_iTotalWidth >rtClient.Width() && !m_pScrollBar ->GetShow(FALSE))  m_iTotalHeight  += m_iBarHeight;
		if(m_iTotalHeight>rtClient.Height()&& !m_pScrollBar ->GetShow(TRUE))   m_iTotalWidth   += m_iBarWidth;
		if(m_iTotalWidth >rtClient.Width() && !m_pScrollBar ->GetShow(FALSE))  m_iTotalHeight  += m_iBarHeight;


		scrInfoH.nMax   = m_iTotalWidth;
		scrInfoH.nMin   = 0;
		scrInfoH.nPos   = m_iTopX;

		scrInfoV.nMax   = m_iTotalHeight;
		if(m_iTotalHeight >= rtClient.Height())
		scrInfoV.nMax  += TP_LSITEXTEND_HEIGHT;
		scrInfoV.nMin   = 0;
		scrInfoV.nPos   = m_iTopY;

	}
	if(m_iViewType == VIEWTYPE_LIST || m_iViewType == VIEWTYPE_ICON || m_iViewType == VIEWTYPE_BUFFER)
	{
		iScrollLine = m_szItemIcon.cy + m_szSpace.cy;
		m_iTotalWidth  = 0;
		m_iTotalHeight = 0;
		int iItemSize  = (int)m_aItem.GetSize();
		CRect rtItem = CRect(0,0,0,0);
		for(int  l=0;l<iItemSize;l++)
		{
			rtItem  = m_aItem[l]->aSubItem[0]->rtItem;
			m_iTotalHeight = max(rtItem.bottom + m_iTopY ,m_iTotalHeight);
			m_iTotalWidth  = max(rtItem.right  + m_iTopX ,m_iTotalWidth);
		}

		if(m_iTotalWidth >rtClient.Width() && !m_pScrollBar ->GetShow(FALSE))  m_iTotalHeight  += m_iBarHeight;
		if(m_iTotalHeight>rtClient.Height()&& !m_pScrollBar ->GetShow(TRUE))   m_iTotalWidth   += m_iBarWidth;
		if(m_iTotalWidth >rtClient.Width() && !m_pScrollBar ->GetShow(FALSE))  m_iTotalHeight  += m_iBarHeight;


		scrInfoH.nMax   = m_iTotalWidth ;
		scrInfoH.nMin   = 0;
		scrInfoH.nPos   = m_iTopX;

		scrInfoV.nMax   = m_iTotalHeight   ;
		if(m_iTotalHeight >= rtClient.Height())
	    scrInfoV.nMax   += TP_LSITEXTEND_HEIGHT;
		scrInfoV.nMin   = 0;
		scrInfoV.nPos   = m_iTopY;
	}
	if(m_iViewType == VIEWTYPE_INFO)
	{
		iScrollLine = m_szItemInfo.cy;
		m_iTotalWidth  = m_szItemInfo.cx;
		m_iTotalHeight = 0;
		for(int l=0; l<m_aItem.GetSize();l++)
		{
			m_iTotalHeight += m_aItem[l]->aSubItem[0]->rtItem.Height();
			m_iTotalWidth   = max(m_iTotalWidth,m_aItem[l]->aSubItem[0]->rtItem.Width());
		}

		if(m_iTotalWidth >rtClient.Width() && !m_pScrollBar ->GetShow(FALSE))  m_iTotalHeight  += m_iBarHeight;
		if(m_iTotalHeight>rtClient.Height()&& !m_pScrollBar ->GetShow(TRUE))   m_iTotalWidth   += m_iBarWidth;
		if(m_iTotalWidth >rtClient.Width() && !m_pScrollBar ->GetShow(FALSE))  m_iTotalHeight  += m_iBarHeight;


		scrInfoH.nMax   = m_iTotalWidth;
		scrInfoH.nMin   = 0;
		scrInfoH.nPos   = m_iTopX;

		scrInfoV.nMax   = m_iTotalHeight;
		if(m_iTotalHeight >= rtClient.Height())
		scrInfoV.nMax  += TP_LSITEXTEND_HEIGHT;
		scrInfoV.nMin   = 0;
		scrInfoV.nPos   = m_iTopY;
	}
    ((CTPScrollBar *)m_pScrollBar ->pVScroll) ->SetScrollLine(iScrollLine);//qushaojie 20091021设置不同模式下每次滚动条移动的距离

	scrInfoV.nPos  = min(scrInfoV.nPos,scrInfoV.nMax-(int)scrInfoV.nPage+1);
	scrInfoH.nPos  = min(scrInfoH.nPos,scrInfoH.nMax-(int)scrInfoH.nPage+1);
	scrInfoV.nPos  = max(scrInfoV.nPos,0);
	scrInfoH.nPos  = max(scrInfoH.nPos,0);

	SetScrollInfo(SBS_HORZ,&scrInfoH,TRUE);
	SetScrollInfo(SBS_VERT,&scrInfoV,TRUE);

	m_iTopX = GetScrollPos(SBS_HORZ);
	m_iTopY = GetScrollPos(SBS_VERT);

	if(iTopX != m_iTopX || iTopY != m_iTopY)
	{
		FunCalcHeadSize();
		FunCalcItemSize();	
	}

	m_bCalcScroll = FALSE;	
}
void CTPListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{	
	if(m_bCalcScroll) return;
	((CTPScrollBar *)m_pScrollBar ->pHScroll) ->SetScroll(nSBCode,nPos);
	int iTopX 	= ((CTPScrollBar *)m_pScrollBar ->pHScroll) ->GetScrollPos();
	if(iTopX != m_iTopX)
	{	
		int     iTotalHeight =  m_iTotalHeight;

		m_uState |= STATE_SCROLL;
		m_iTopX = iTopX;
		FunCalcHeadSize();
		FunCalcItemSize();
		FunCalcEditItem();
		m_uState &= ~STATE_SCROLL;	
		Invalidate(FALSE);
	}
	m_uState &= ~STATE_TOPVIEW;
}

void CTPListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(m_bCalcScroll) return;
	((CTPScrollBar *)m_pScrollBar ->pVScroll) ->SetScroll(nSBCode,nPos);
	int iTopY 	= ((CTPScrollBar *)m_pScrollBar ->pVScroll) ->GetScrollPos();
	if(iTopY != m_iTopY)
	{
		m_iTopY = iTopY;
		m_uState |= STATE_SCROLL;
		FunCalcItemSize();
		FunCalcEditItem();
		m_uState &= ~STATE_SCROLL;
		Invalidate(FALSE);
	}
	m_uState &= ~STATE_TOPVIEW;
}
BOOL CTPListCtrlEx::FunHitTest(CPoint point, TPListCtrlHitTest *pData) const
{
	if(m_iViewType == VIEWTYPE_REPORT  || m_iViewType == VIEWTYPE_TEXTPICTURE)
	{
		if(point.y < m_iColumnHeight)
		{		
			pData->bHead = TRUE;
			for(int i=0;i<m_aColumn.GetSize();i++)
			{
				if(!m_aColumn[i]->bShow)  continue;
				if(m_aColumn[i]->rtItem.PtInRect(point))
				{				
					pData->iItem = i;
					pData->pData = m_aColumn[i];
					if(m_uPrivateStyle & LIST_SIZECOLUMN)
					{
						if(point.x <= m_aColumn[i]->rtItem.left +4)          pData->iHeadFlag = 1;
						else	if(point.x >= m_aColumn[i]->rtItem.right -4) pData->iHeadFlag = 2;
						else  	                                             pData->iHeadFlag = 0;
					}
					else   pData->iHeadFlag = 0;
					return TRUE;
				}
				else if(m_aColumn[i]->rtItem.Width()<=4)
				{
					pData->iItem = i;
					pData->pData = m_aColumn[i];
					if(m_uPrivateStyle & LIST_SIZECOLUMN) pData->iHeadFlag = 2;
					else                                  pData->iHeadFlag = 0;
					return TRUE;
				}

			}
		}
		else
		{		
			pData->bHead = FALSE;
			for(int i=0;i<m_aItem.GetSize();i++)
			{
				if(!m_aItem[i]->bShow) continue;
				for(int k=0;k<m_aItem[i]->aSubItem.GetSize();k++)
				{
					if(m_aItem[i]->aSubItem[k]->rtItem .PtInRect(point))
					{
						pData->iItem    = i;
						pData->iSubItem = k;
						pData->pData    = m_aItem[i]->aSubItem[k];
						if(m_aItem[i]->aSubItem[k]->rtDrag.PtInRect(point))						pData ->bDrag = TRUE;
						else 	                                        						pData ->bDrag = FALSE;
						return TRUE;
					}
				}
				if(point.y>= m_aItem[i]->aSubItem[0]->rtItem.top && point.y<= m_aItem[i]->aSubItem[0]->rtItem.bottom)
				{
					pData->iItem    = i;
					pData->iSubItem = -1;
					pData->pData    = NULL;
					return TRUE;
				}
			}
		}
	}
	if(m_iViewType == VIEWTYPE_LIST || m_iViewType == VIEWTYPE_INFO || m_iViewType == VIEWTYPE_ICON || m_iViewType == VIEWTYPE_BUFFER)
	{
		pData->bHead = FALSE;
		for(int i=0;i<m_aItem.GetSize();i++)
		{
			if(!m_aItem[i]->bShow) continue;
			if(m_aItem[i]->aSubItem[0]->rtItem .PtInRect(point))
			{
				pData->iItem    = i;
				pData->iSubItem = 0;
				pData->pData    = m_aItem[i]->aSubItem[0];
				if(m_aItem[i]->aSubItem[0]->rtDrag.PtInRect(point))		 	pData ->bDrag = TRUE;
				else                                        					pData ->bDrag = FALSE;
				return TRUE;
			}			
		}
	}
	return FALSE;
}

void CTPListCtrlEx::OnLButtonDownLargeIcon(UINT nFlags, CPoint point)
{
	if(!FunHitTest(point,&m_hitTest)) //什么都没有选中 将选中项取消选中
	{
 		if(GetSelectedCount()>0 && !TP_IsShiftKeyDown() && !TP_IsCtrlKeyDown())  
		{
			for(INT l=0;l<m_aItem.GetSize();l++) 
			{
				if(m_aItem[l] ->uState & LVIS_SELECTED)
				{
					m_aItem[l] ->uState  &= ~LVIS_SELECTED;
					FunNotifyParent(LVN_ITEMCHANGED,l,-1);
				}
			}
			Invalidate(FALSE);

		}
		//	else
		{
			m_iDragMode = TP_DRAG_RECT;
			m_ptSave = point;
			m_ptSave.Offset(m_iTopX,m_iTopY); 
			m_ptMiddle = point;
		}

		return;
	}
	if(!m_hitTest.bHead)
	{	
		m_uState &= ~STATE_SELECTED;
		FunSelectItem(m_hitTest.iItem,TRUE);						
		m_ptMiddle.x = m_aItem[m_hitTest.iItem]->aSubItem[0]->rtItem.top;
		m_ptMiddle.y = m_aItem[m_hitTest.iItem]->aSubItem[0]->rtItem.bottom;
		if(GetSelectedCount()>0)
		{
			m_ptDrag = point;
			m_ptSave = point;
			m_ptSave.Offset(m_iTopX,m_iTopY); 
			m_iDragMode = TP_DRAG_ITEM;
		}
		FunNotifyParent(NM_CLICK,m_hitTest.iItem,m_hitTest.iSubItem);
	}			
}
void CTPListCtrlEx::OnLButtonDownBuffer(UINT nFlags, CPoint point)
{
	OnLButtonDownLargeIcon(nFlags,point);
}
void CTPListCtrlEx::OnLButtonDownInfo(UINT nFlags, CPoint point)
{
	OnLButtonDownList(nFlags, point);
}
void CTPListCtrlEx::OnLButtonDownList(UINT nFlags, CPoint point)
{
	if(!FunHitTest(point,&m_hitTest)) //什么都没有选中 将选中项取消选中
	{
 		if(GetSelectedCount()>0  && !TP_IsShiftKeyDown() && !TP_IsCtrlKeyDown())  
		{
			for(INT l=0;l<m_aItem.GetSize();l++) 
			{
				if(m_aItem[l] ->uState  & LVIS_SELECTED)
				{				
					m_aItem[l] ->uState  &= ~LVIS_SELECTED;
					FunNotifyParent(LVN_ITEMCHANGED,l,-1);
				}
			}
			Invalidate(FALSE);			
		}
		//	else
		{
			m_iDragMode = TP_DRAG_RECT;
			m_ptSave = point;
			m_ptSave.Offset(m_iTopX,m_iTopY); 
			m_ptMiddle = point;
		}
		return;
	}
	if(!m_hitTest.bHead)
	{	
		m_uState &= ~STATE_SELECTED;
		FunSelectItem(m_hitTest.iItem,TRUE);						
		m_ptMiddle.x = m_aItem[m_hitTest.iItem]->aSubItem[0]->rtItem.top;
		m_ptMiddle.y = m_aItem[m_hitTest.iItem]->aSubItem[0]->rtItem.bottom;
		if(GetSelectedCount()>0 && m_hitTest.bDrag)	
		{
			m_iDragMode = TP_DRAG_ITEM;		
			m_ptDrag    = point;
		}
		else
		{
			m_iDragMode = TP_DRAG_RECT;
			m_ptSave = point;
			m_ptSave.Offset(m_iTopX,m_iTopY); 
			m_ptMiddle = point;
		}
		FunNotifyParent(NM_CLICK,m_hitTest.iItem,m_hitTest.iSubItem);
	}			
}
void CTPListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(GetFocus()!=this) SetFocus();
	if(m_pEditWnd) return;
	for(INT l=0;l<m_aItem.GetSize();l++) m_aItem[l]->uNotify = 0;
	m_uState |= STATE_NOSENDNOTIFY;
	m_hitTest.iItem = -1;
	m_bLeftDrag     = TRUE;
	if(m_iViewType == VIEWTYPE_REPORT)       	 OnLButtonDownReport(nFlags, point);
	else if(m_iViewType == VIEWTYPE_TEXTPICTURE) OnLButtonDownReport(nFlags, point);
	else if(m_iViewType == VIEWTYPE_ICON)	     OnLButtonDownLargeIcon(nFlags, point);
	else if(m_iViewType == VIEWTYPE_INFO)	     OnLButtonDownInfo(nFlags, point);
	else if(m_iViewType == VIEWTYPE_BUFFER)	     OnLButtonDownBuffer(nFlags, point);
	else if(m_iViewType == VIEWTYPE_LIST) 	     OnLButtonDownList(nFlags, point);
	FunNotifyParentAll();

}
void CTPListCtrlEx::SortItem(int iHeadIndex,BOOL bAsc)
{
}

void CTPListCtrlEx::OnLButtonDownReport(UINT nFlags, CPoint point) 
{
	if(!FunHitTest(point,&m_hitTest)) //什么都没有选中 将选中项取消选中
	{
 		if(GetSelectedCount()>0  && !TP_IsShiftKeyDown() && !TP_IsCtrlKeyDown())  
		{
			for(INT l=0;l<m_aItem.GetSize();l++) 
			{
				if(m_aItem[l] ->uState  & LVIS_SELECTED)
				{
					m_aItem[l] ->uState  &= ~LVIS_SELECTED;
					FunNotifyParent(LVN_ITEMCHANGED,l,-1);
				}
			}
			Invalidate(FALSE);			
		}
		//	else
		{
			m_iDragMode = TP_DRAG_RECT;
			m_ptSave = point;
			m_ptSave.Offset(m_iTopX,m_iTopY); 
			m_ptMiddle = point;
		}
		return;
	}
	if(!m_hitTest.bHead)
	{
		if(m_hitTest.iSubItem >=0)
		{			
			m_uState &= ~STATE_SELECTED;
			m_iCheckIndex = -1;
			if(m_aItem[m_hitTest.iItem]->aSubItem[m_hitTest.iSubItem]->rtCheck.PtInRect(point))
			{
				m_iCheckIndex = m_hitTest.iItem;
				m_aItem[m_hitTest.iItem]->bCheck = !m_aItem[m_hitTest.iItem]->bCheck;
				Invalidate(FALSE);
			}			
			FunSelectItem(m_hitTest.iItem,TRUE);						
			m_ptMiddle.x = m_aItem[m_hitTest.iItem]->aSubItem[0]->rtItem.top;
			m_ptMiddle.y = m_aItem[m_hitTest.iItem]->aSubItem[0]->rtItem.bottom;
			if(m_aItem[m_hitTest.iItem]->aSubItem[m_hitTest.iSubItem]->rtCheck.PtInRect(point))
			{
				if(m_iCheckIndex!=-1)
					FunNotifyParent(LVN_ITEMCHANGED,m_hitTest.iItem,m_hitTest.iSubItem);
				m_iCheckIndex = -1;
			}			
			else if(GetSelectedCount()>0 && m_hitTest.bDrag)
			{
				m_ptDrag = point;
				m_iDragMode = TP_DRAG_ITEM;	
			}
			else 
			{
				m_iDragMode = TP_DRAG_RECT;
				m_ptSave = point;
				m_ptSave.Offset(m_iTopX,m_iTopY); 
				m_ptMiddle = point;
			}
			FunNotifyParent(NM_CLICK,m_hitTest.iItem,m_hitTest.iSubItem);
		}
		else
		{
			m_iDragMode = TP_DRAG_RECT;
			m_ptSave = point;
			m_ptSave.Offset(m_iTopX,m_iTopY); 
			m_ptMiddle = point;
		}	
	}		
	else
	{
		m_rtInvert           = CRect(0,0,0,0);
		if(m_hitTest.iHeadFlag ==0)
		{
			m_iHeadDown = m_hitTest.iItem; 
			CRect rtClient;
			GetClientRect(&rtClient);
			rtClient.bottom = rtClient.top + m_iColumnHeight;
			InvalidateRect(rtClient);
			if((LIST_DRAGCOLUMN & m_uPrivateStyle) && (m_hitTest.iItem > m_iLockDragColumn))
				m_iDragMode = TP_DRAG_MIDDLE;
			TPListCtrlColumn *pHead = (TPListCtrlColumn *)m_hitTest.pData ;
			m_ptDrag   = point;
			m_ptMiddle.x = point.x +  pHead->rtItem .left ;
		}
		else if(m_hitTest.iHeadFlag == 1 && m_hitTest.iItem >0)
		{
			m_iDragMode = TP_DRAG_LEFT;
			SetCapture();
			m_ptDrag = point;
			::SetCursor(m_hCursorDrag[2] ->GetCursor()); 
			m_ptMiddle = point;
		}
		else if(m_hitTest.iHeadFlag == 2)
		{
			m_iDragMode = TP_DRAG_RIGHT;
			SetCapture();
			m_ptDrag = point;
			::SetCursor(m_hCursorDrag[2] ->GetCursor()); 
		}
	}
}
void CTPListCtrlEx::OnLButtonUpLargeIcon(UINT nFlags, CPoint point)
{
	if(m_iDragMode == TP_DRAG_RECTOK)
	{
		if(m_hTimer[3]) {KillTimer(m_hTimer[3]);m_hTimer[3] = NULL;	}
		if(m_hTimer[2]) {KillTimer(m_hTimer[2]);m_hTimer[2] = NULL;	}
		ReleaseCapture();
		::SetCursor(m_hCursorDrag[0] ->GetCursor()); 
		CRect rtRect = CRect(m_ptSave.x,m_ptSave.y,point.x + m_iTopX,point.y + m_iTopY),rtItem;
		rtRect.NormalizeRect();
		for(INT l=0;l<m_aItem.GetSize();l++)
		{
			rtItem = m_aItem[l]->aSubItem[0] ->rtItem;
			rtItem.OffsetRect(m_iTopX,m_iTopY);
			rtItem &= rtRect;

			if(rtItem.Width()>0 && rtItem.Height() >0)
			{
				if(!(m_aItem[l]->uState & LVIS_SELECTED))
				{					
					m_aItem[l]->dwSelTick = GetTickCount();
					m_aItem[l]->uState  |= LVIS_SELECTED;
					FunNotifyParent(LVN_ITEMCHANGED,l,0);
				}
			}
			else
			{
				if(m_aItem[l]->uState & LVIS_SELECTED)
				{
					//song
					if(TP_IsCtrlKeyDown() || TP_IsShiftKeyDown());
					else 
					{
						m_aItem[l]->uState &= ~LVIS_SELECTED;
						FunNotifyParent(LVN_ITEMCHANGED,l,0);
					}
				}
			}
		}		
		Invalidate(FALSE);

		m_iDragIndex = -1;
		m_iDragMode = TP_DRAG_UNKNOW;
	}
	else if(TP_DRAG_ITEMOK == m_iDragMode)
	{
		if(m_iDragMode == TP_DRAG_ITEMOK)
		{
			if(m_hTimer[3]) {KillTimer(m_hTimer[3]);m_hTimer[3] = NULL;	}


			CRect rtClient;
			GetClientRect(&rtClient);
			ReleaseCapture();
			::SetCursor(m_hCursorDrag[0] ->GetCursor());
			m_aInvertRect.RemoveAll();

			if(m_hDragCursor) ::DestroyIcon(m_hDragCursor);
			m_hDragCursor = NULL;

			if(m_iArrType == ARRANGE_AUTO)
			{
				if(m_bLeftDrag && !(nFlags &MK_CONTROL))//qushaojie 20100407 isctrlkeydown滞后、不准确，左键+ctrl拖动，同时松开，判断ctrl没有摁下
				{
					TPListCtrlItemArray  aListItem;
					int          iItem    = FunHitTestIcon(point);
					if(iItem <0) return;
					//0偏移return qushaojie
					if (iItem == m_hitTest.iItem) 
					{
						m_rtInvert  = CRect(0,0,0,0);
						Invalidate();
						return;
					}
					if(iItem>(int)m_aItem.GetSize()) iItem = (int)m_aItem.GetSize();

					int iMinChangeItem = iItem;
					for(INT l= (int)m_aItem.GetSize() -1;l>=0;l--)
					{		
						if(m_aItem[l]->uState & LVIS_SELECTED)
						{		
							aListItem.Add(m_aItem[l]);
							m_aItem[l] = NULL;
							if(l < iMinChangeItem)  iMinChangeItem = l;
						}
					}
					for(INT l = (int)aListItem.GetSize()-1;l>=0;l--)
					{
						m_aItem.InsertAt(iItem++,aListItem[l]);
					}
					for(INT l= (int)m_aItem.GetSize() -1;l>=0;l--)
					{
						if(m_aItem[l] == NULL) m_aItem.RemoveAt(l);
					}
					FunItemIconIndex(TRUE);
					//auto拖动顺序后，清除report模式的排序箭头标志
                    ClearSortFlag(0);
				}
				FunCalcSize();
			}
			else if(m_iArrType == ARRANGE_STORYBOARD)
			{
				//add qushaojie 20090803
				CRect SquareRect(9999,9999,0,0);
				if(m_bLeftDrag&& !(nFlags &MK_CONTROL))	//qushaojie 20100407 isctrlkeydown滞后、不准确，左键+ctrl拖动，同时松开，判断ctrl没有摁下
				{
					FunMoveClip(point,FALSE,SquareRect,TP_CLIPDRAGTYPE_MOVE); 
					FunSortbyIconIndex();
				}
				FunCalcSize();
			}
			else if(m_iArrType == ARRANGE_FREE)
			{
				long lMinX = 0x7fffff,lMinY = 0x7fffff;
				if(m_uPrivateStyle & LIST_DRAGITEM)
				{				
					CPoint pt = point;
					pt.Offset(m_iTopX,m_iTopY);
					for(INT l=0;l<m_aItem.GetSize();l++)
					{		
						if(m_aItem[l] ->uState &LVIS_SELECTED)
						{
							m_aItem[l] ->aSubItem[0]->rtItem.OffsetRect(pt.x - m_ptSave.x,pt.y - m_ptSave.y);
						}
						m_aItem[l] ->aSubItem[0]->rtItemSave = m_aItem[l] ->aSubItem[0]->rtItem;
						m_aItem[l] ->aSubItem[0]->rtItemSave.OffsetRect(m_iTopX,m_iTopY);	
						lMinX = min(m_aItem[l] ->aSubItem[0]->rtItemSave.left,lMinX);
						lMinY = min(m_aItem[l] ->aSubItem[0]->rtItemSave.top,lMinY);
					}
					if(lMinX <0 || lMinY <0)
					{
						for(INT l=0;l<m_aItem.GetSize();l++)
						{		
							if(m_aItem[l] ->uState &LVIS_SELECTED)
							{
								m_aItem[l] ->aSubItem[0]->rtItemSave.OffsetRect(lMinX <0 ? -lMinX:0,lMinY <0 ? -lMinY:0);
							}
						}
					}
					if(m_iViewType !=VIEWTYPE_BUFFER)			m_uState |=STATE_DRAGITEM;				
					FunCalcSize();	
					FunReSortItem();
				}
			}
		}
		Invalidate();
		m_iDragIndex = -1;
		m_iDragMode = TP_DRAG_UNKNOW;
	}
	else if(m_bLeftDrag)
	{
		if(nFlags == 0) BeginEditTest(m_hitTest.iItem,m_hitTest.iSubItem);
		if(FunHitTest(point,&m_hitTest) && !m_hitTest.bHead && m_hitTest.iItem>=0)// && !TP_IsCtrlKeyDown() && !TP_IsShiftKeyDown())
		{			
			FunSelectItem(m_hitTest.iItem,FALSE);			
		}
		m_uState &= ~STATE_SELECTED;
	}
	m_rtInvert  = CRect(0,0,0,0);
	m_aInvertRect.RemoveAll();
	m_iDragMode = TP_DRAG_UNKNOW;
}
void CTPListCtrlEx::OnLButtonUpBuffer(UINT nFlags, CPoint point)
{
	OnLButtonUpLargeIcon(nFlags,point);
}
void CTPListCtrlEx::OnLButtonUpInfo(UINT nFlags, CPoint point)
{
	OnLButtonUpList(nFlags,  point);
}
void CTPListCtrlEx::OnLButtonUpList(UINT nFlags, CPoint point)
{
	if(m_iDragMode == TP_DRAG_RECTOK)
	{
		if(m_hTimer[3]) {KillTimer(m_hTimer[3]);m_hTimer[3] = NULL;	}
		if(m_hTimer[2]) {KillTimer(m_hTimer[2]);m_hTimer[2] = NULL;	}
		ReleaseCapture();
		::SetCursor(m_hCursorDrag[0] ->GetCursor()); 
		CRect rtRect = CRect(m_ptSave.x,m_ptSave.y,point.x + m_iTopX,point.y + m_iTopY),rtItem;
		rtRect.NormalizeRect();		
		for(INT l=0;l<m_aItem.GetSize();l++)
		{
			//rtItem = m_aItem[l]->aSubItem[0] ->rtItem;
			//qushaojie 20090915
			GetItemRect(l,rtItem,LVIR_BOUNDS);
			rtItem.OffsetRect(m_iTopX,m_iTopY);
			rtItem &= rtRect;

			if(rtItem.Width()>0 && rtItem.Height() >0)
			{
				if(!(m_aItem[l]->uState & LVIS_SELECTED))
				{
					m_aItem[l]->dwSelTick = GetTickCount();
					m_aItem[l]->uState  |= LVIS_SELECTED;
					FunNotifyParent(LVN_ITEMCHANGED,l,0);
				}
			}
			else
			{
				if(m_aItem[l]->uState & LVIS_SELECTED)
				{
					//song
					if(TP_IsCtrlKeyDown() || TP_IsShiftKeyDown());
					else 
					{
						m_aItem[l]->uState &= ~LVIS_SELECTED;
						FunNotifyParent(LVN_ITEMCHANGED,l,0);
					}
				}
			}
		}		
		Invalidate(FALSE);

		m_iDragIndex = -1;
		m_iDragMode = TP_DRAG_UNKNOW;
	}
	else if(TP_DRAG_ITEMOK == m_iDragMode)
	{
		if(m_iDragMode == TP_DRAG_ITEMOK)
		{
			if(m_hTimer[3]) {KillTimer(m_hTimer[3]);m_hTimer[3] = NULL;	}		

			CRect rtClient;
			GetClientRect(&rtClient);
			ReleaseCapture();
			::SetCursor(m_hCursorDrag[0] ->GetCursor()); 

			if(m_hDragCursor) ::DestroyIcon(m_hDragCursor);
			m_hDragCursor = NULL;


			if(m_uPrivateStyle & LIST_DRAGITEM)
			{				
				FunMoveSelectItemTo(m_iDragIndex);			
				ReLayOut();
			}
		}
		Invalidate();
		m_iDragIndex = -1;
		m_iDragMode = TP_DRAG_UNKNOW;
	}
	else  if(m_bLeftDrag)
	{
		if(nFlags == 0)  BeginEditTest(m_hitTest.iItem,m_hitTest.iSubItem);
		if(FunHitTest(point,&m_hitTest) && !m_hitTest.bHead && m_hitTest.iItem>=0)// && !TP_IsCtrlKeyDown() && !TP_IsShiftKeyDown())
		{
			FunSelectItem(m_hitTest.iItem,FALSE);			
		}
		m_uState &= ~STATE_SELECTED;
	}
	m_rtInvert  = CRect(0,0,0,0);
	m_iDragMode = TP_DRAG_UNKNOW;
}
void CTPListCtrlEx::SelectItem(int iIndex, BOOL bSelect,BOOL bUnSelAll)
{
	if(bUnSelAll)
	{
		for(INT l=0;l<m_aItem.GetSize();l++)
		{
			if(m_aItem[l]->uState &LVIS_SELECTED)
			{
				m_aItem[l]->uState &= ~LVIS_SELECTED;
				FunNotifyParent(LVN_ITEMCHANGED,l,0);
			}
		}
	}
	if(iIndex>=0 && iIndex <m_aItem.GetSize())
	{
		if(!bSelect)  m_aItem[iIndex]->uState &= ~LVIS_SELECTED;
		else         m_aItem[iIndex]->uState |= LVIS_SELECTED;
		FunNotifyParent(LVN_ITEMCHANGED,iIndex,0);
	}	
}
void CTPListCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_iViewType == VIEWTYPE_REPORT)     	      OnLButtonUpReport(nFlags, point);
	else if(m_iViewType == VIEWTYPE_TEXTPICTURE)  OnLButtonUpReport(nFlags, point); 
	else if(m_iViewType == VIEWTYPE_ICON) 	      OnLButtonUpLargeIcon(nFlags, point);
	else if(m_iViewType == VIEWTYPE_INFO)	      OnLButtonUpInfo(nFlags, point);
	else if(m_iViewType == VIEWTYPE_BUFFER)       OnLButtonUpBuffer(nFlags, point);
	else if(m_iViewType == VIEWTYPE_LIST)         OnLButtonUpList(nFlags, point);
	m_uState &= ~STATE_DBCLICK;
}
void CTPListCtrlEx::OnLButtonUpReport(UINT nFlags, CPoint point) 
{
	if(m_iDragMode == TP_DRAG_RECTOK)
	{
		if(m_hTimer[3]) {KillTimer(m_hTimer[3]);m_hTimer[3] = NULL;	}
		if(m_hTimer[2]) {KillTimer(m_hTimer[2]);m_hTimer[2] = NULL;	}
		ReleaseCapture();
		::SetCursor(m_hCursorDrag[0] ->GetCursor()); 
		CRect rtRect = CRect(m_ptSave.x,m_ptSave.y,point.x + m_iTopX,point.y + m_iTopY),rtItem;
		rtRect.NormalizeRect();		
		for(INT l=0;l<m_aItem.GetSize();l++)
		{
			//rtItem = m_aItem[l]->aSubItem[0] ->rtItem;
			//rtItem.right = m_aItem[l]->GetRight();
			//add by qushaojie 20090915
			GetItemRect(l,rtItem,LVIR_BOUNDS);
			rtItem.OffsetRect(m_iTopX,m_iTopY);
			rtItem &= rtRect;

			if(rtItem.Width()>0 && rtItem.Height() >0)
			{
				if(!(m_aItem[l]->uState & LVIS_SELECTED))
				{
					m_aItem[l]->dwSelTick = GetTickCount();
					m_aItem[l]->uState  |= LVIS_SELECTED;
					FunNotifyParent(LVN_ITEMCHANGED,l,0);
				}
			}
			else
			{
				if(m_aItem[l]->uState & LVIS_SELECTED)
				{					
					if(TP_IsCtrlKeyDown() || TP_IsShiftKeyDown());
					else 
					{
						m_aItem[l]->uState &= ~LVIS_SELECTED;
						FunNotifyParent(LVN_ITEMCHANGED,l,0);
					}
				}
			}
		}

		Invalidate(FALSE);

		m_iDragIndex = -1;
		m_iDragMode = TP_DRAG_UNKNOW;
	}
	else if(TP_DRAG_LEFTOK == m_iDragMode || TP_DRAG_LEFT == m_iDragMode)
	{
		if(TP_DRAG_LEFTOK == m_iDragMode)
		{
			TPListCtrlColumn *pHead = (TPListCtrlColumn *)m_hitTest.pData ;
			if(pHead)
			{
				INT nIndex = m_hitTest.iItem - 1;
				for(; nIndex > 0; nIndex--)
					if(m_aColumn[nIndex]->bShow) break;
				if(nIndex >= 0)
				{
					TPListCtrlColumn *pHeadLeft = m_aColumn[nIndex];
					pHeadLeft->iWidth = max(0,point.x - pHeadLeft->rtItem.left);
					if(m_hitTest.iItem-1<=m_iLockColumn) pHeadLeft->iWidth = min(m_rtClient.Width() - 20,pHeadLeft->iWidth);
					pHeadLeft->iWidth = min(pHeadLeft->iWidth,pHeadLeft->lMaxWidth);
					pHeadLeft->iWidth = max(pHeadLeft->iWidth,pHeadLeft->lMinWidth);
				ReLayOut();
				Invalidate(FALSE);
				ChangeColume(0);
				}
			}
		}
		ReleaseCapture();
		m_iHeadDown = -1;
		m_iDragMode = TP_DRAG_UNKNOW;
		Invalidate(FALSE);
		::SetCursor(m_hCursorDrag[0] ->GetCursor()); 
	}	
	else if(TP_DRAG_RIGHTOK == m_iDragMode|| TP_DRAG_RIGHT == m_iDragMode)
	{
		if(TP_DRAG_RIGHTOK == m_iDragMode)
		{			
			TPListCtrlColumn *pHead = (TPListCtrlColumn *)m_hitTest.pData ;
			if(pHead &&point.x >= pHead->rtItem.left )
			{
				pHead->iWidth =  point.x - pHead->rtItem.left ;  								
				if(pHead ->iIndex<=m_iLockColumn) pHead->iWidth = min(m_rtClient.Width() - 20,pHead->iWidth);
				pHead->iWidth = min(pHead->iWidth,pHead->lMaxWidth);
				pHead->iWidth = max(pHead->iWidth,pHead->lMinWidth);
				ReLayOut();
				Invalidate(FALSE);
				ChangeColume(1);
			}
		}
		ReleaseCapture();
		m_iHeadDown = -1;
		Invalidate(false);
		::SetCursor(m_hCursorDrag[0] ->GetCursor()); 
	}
	else if(TP_DRAG_MIDDLEOK == m_iDragMode)
	{
		if(TP_DRAG_MIDDLEOK == m_iDragMode)
		{
			ReleaseCapture();
			TPListCtrlHitTest  hitData;
			FunHitTest(CPoint(point.x,5),&hitData);			
			if(hitData.bHead && m_hitTest.iItem !=hitData.iItem && (hitData.iItem < 0 || hitData.iItem > m_iLockDragColumn))
			{
				TPListCtrlColumn *pHeadSource = (TPListCtrlColumn *)m_hitTest.pData;
				int iSourceIndex = pHeadSource->iIndex ;
				int iDestIndex   =  FunGetHeadInsertIndex(point); 
				FunMoveColumn(iSourceIndex,iDestIndex);				
			}
			if(m_pDragImageList)
			{
				m_pDragImageList ->DragLeave(this);
				m_pDragImageList ->EndDrag();
				m_pDragImageList ->DeleteImageList();
				delete m_pDragImageList;
				m_pDragImageList = NULL;
			}
			if(m_hTimer[2])
			{
				KillTimer(m_hTimer[2]);
				m_hTimer[2] = NULL;
			}
		}		
		m_iHeadDown  = -1;
		m_iDragIndex = -1;
		Invalidate();
		::SetCursor(m_hCursorDrag[0] ->GetCursor()); 
	}	
	else if(m_iHeadDown >=0)
	{
		FunNotifyParent(HDN_ITEMCLICK,m_iHeadDown,0);
		m_iHeadDown = -1;
		CRect rtClient;
		GetClientRect(&rtClient);
		rtClient.bottom = rtClient.top + m_iColumnHeight;
		InvalidateRect(rtClient);		
	}
	else if(TP_DRAG_ITEMOK == m_iDragMode)
	{
		if(m_iDragMode == TP_DRAG_ITEMOK)
		{
			if(m_hTimer[3]) {KillTimer(m_hTimer[3]);m_hTimer[3] = NULL;	}			

			CRect rtClient;
			GetClientRect(&rtClient);
			ReleaseCapture();
			::SetCursor(m_hCursorDrag[0] ->GetCursor()); 

			if(m_hDragCursor) ::DestroyIcon(m_hDragCursor);
			m_hDragCursor = NULL;

			if(m_uPrivateStyle & LIST_DRAGITEM)
			{				
				FunMoveSelectItemTo(m_iDragIndex);			
				ReLayOut();
			}
		}
		Invalidate();
		m_iDragIndex = -1;
		m_iDragMode = TP_DRAG_UNKNOW;
	}
	else if(m_bLeftDrag)
	{
		if(nFlags == 0)  BeginEditTest(m_hitTest.iItem,m_hitTest.iSubItem);
		if(FunHitTest(point,&m_hitTest) && !m_hitTest.bHead && m_hitTest.iItem>=0)// && !TP_IsCtrlKeyDown() && !TP_IsShiftKeyDown())
		{	
			FunSelectItem(m_hitTest.iItem,FALSE);			
		}
		m_uState &= ~STATE_SELECTED;
	}
	m_rtInvert  = CRect(0,0,0,0);
	m_iDragMode = TP_DRAG_UNKNOW;
}


void CTPListCtrlEx::OnMouseMoveLargeIcon(UINT nFlags, CPoint point)
{
	if(m_iDragMode == TP_DRAG_RECT ||  TP_DRAG_RECTOK == m_iDragMode)
	{

		if(!(nFlags & (MK_LBUTTON | MK_RBUTTON)))   m_iDragMode = TP_DRAG_UNKNOW;
		else if(m_ptMiddle != point)
		{
			if(m_iDragMode == TP_DRAG_RECT)
			{
				SetCapture();
				m_iDragMode = TP_DRAG_RECTOK;
				::SetCursor(m_hCursorDrag[0] ->GetCursor()); 
			}
			else
			{
				//自动移动位置
				if(point.y < m_rtClient.top  || point.y >m_rtClient.bottom)
				{
					if(	m_hTimer[3] == NULL)
						m_hTimer[3] = SetTimer(13,100,NULL);
					//return; qushaojie DQAdb00008999 20100115 直接返回画的框就不对了
				}				
				if(point.x < m_rtClient.left  || point.x >m_rtClient.right)
				{
					if(	m_hTimer[2] == NULL)
						m_hTimer[2] = SetTimer(12,100,NULL);
					//return;
				}				
				//选中			
				BOOL bPaint = FALSE;
				CRect rtRect = CRect(m_ptSave.x,m_ptSave.y,point.x + m_iTopX,point.y + m_iTopY),rtItem;
				rtRect.NormalizeRect();

				for(INT l=0;l<m_aItem.GetSize();l++)
				{
					rtItem = m_aItem[l]->aSubItem[0] ->rtItem;
					rtItem.OffsetRect(m_iTopX,m_iTopY);
					rtItem &= rtRect;
					if(rtItem.Width()>0 && rtItem.Height() >0)
					{
						m_aItem[l]->dwSelTick = GetTickCount() + l;
						if(!(m_aItem[l]->uState & LVIS_SELECTED))
						{		
							m_aItem[l]->uState  |= LVIS_SELECTED;
							if(!bPaint)
							{
								bPaint = TRUE;
								FunNotifyParent(LVN_ITEMCHANGED,l,0);
							}
						}
					}
					else
					{
						if(m_aItem[l]->uState & LVIS_SELECTED)
						{
							//song
							if(TP_IsCtrlKeyDown() || TP_IsShiftKeyDown());
							else 	
							{
								m_aItem[l]->uState &= ~LVIS_SELECTED;							
								if(!bPaint)
								{
									bPaint = TRUE;
									FunNotifyParent(LVN_ITEMCHANGED,l,0);
								}
							}
						}
					}
				}	
				if(bPaint){ Invalidate(FALSE);m_rtInvert= CRect(0,0,0,0); }

				CDC *pDC = GetDC();
				if(m_rtInvert.Width() >0) pDC->DrawFocusRect(m_rtInvert);
				m_rtInvert = CRect(m_ptSave.x,m_ptSave.y,point.x + m_iTopX,point.y + m_iTopY);
				m_rtInvert.OffsetRect( -m_iTopX, -m_iTopY);
				m_rtInvert.NormalizeRect();
				pDC->DrawFocusRect(m_rtInvert);
				ReleaseDC(pDC);
			}
			m_ptMiddle = point;
		}
	}
	else if(m_iDragMode == TP_DRAG_ITEM ||  TP_DRAG_ITEMOK == m_iDragMode)
	{

		if(!(nFlags & (MK_LBUTTON | MK_RBUTTON)))  
			m_iDragMode = TP_DRAG_UNKNOW;
		else if(!IsViewItem(m_hitTest.iItem)) m_iDragMode = TP_DRAG_UNKNOW;
		else if(m_ptDrag != point)
		{
			if(m_iDragMode == TP_DRAG_ITEM && (abs(m_ptDrag.x - point.x) > 6 || abs(m_ptDrag.y - point.y) > 6))
			{
				if(m_hitTest.iItem>=0&&m_hitTest.iItem<m_aItem.GetSize() && !(m_aItem[m_hitTest.iItem] ->uState & LVIS_SELECTED))
					FunSelectItem(m_hitTest.iItem,FALSE);
				SetCapture();
				m_iDragMode = TP_DRAG_ITEMOK;
				if(m_uPrivateStyle & LIST_DRAGITEM)
				{
					if(m_hDragCursor) ::DestroyIcon(m_hDragCursor);
					m_hDragCursor = CreateDragIcon(-1,TP_GetSysColor(m_iBackColor),TRUE,&point,m_hCursorDrag[3] ->GetCursor());
					::SetCursor(m_hDragCursor); 	
				}
				else
				{
					EndEdit();
					FunNotifyParent(LVN_BEGINDRAG,0,0);
				}
			}		
			if((m_uPrivateStyle & LIST_DRAGITEM) && TP_DRAG_ITEMOK == m_iDragMode)
			{
				//自动移动位置
				if(point.y < m_rtClient.top  || point.y >m_rtClient.bottom)
				{
					if(	m_hTimer[3] == NULL)
						m_hTimer[3] = SetTimer(13,100,NULL);
				}		
			}

			if(!m_rtClient.PtInRect(point))
			{
				CRectArray   aInvert;PaintInvert(aInvert); 
			}
			else if(m_iArrType == ARRANGE_AUTO)
			{
				int   iItem = FunHitTestIcon(point);
				if(iItem <0 || iItem == m_hitTest.iItem) return;
				if(iItem>(int)m_aItem.GetSize()) iItem = (int)m_aItem.GetSize();
				CRectArray   aInvert;
				for(INT l= (int)m_aItem.GetSize() -1;l>=0;l--)
				{		
					if(m_aItem[l]->uState & LVIS_SELECTED)
					{
						CRect rtItem = GetIconRect(iItem++);
						aInvert.Add(rtItem);
					}
				}
				PaintInvert(aInvert);
			}
			else if(m_iArrType == ARRANGE_STORYBOARD)
			{				
				CRect rtPaintRect(9999,9999,0,0);
				CRectArray aPaintRect;
				if (FunMoveClip(point,TRUE,rtPaintRect,TP_CLIPDRAGTYPE_MOVE))
					aPaintRect.Add(rtPaintRect);
				PaintInvert(aPaintRect);
			}	
		}
	}	
	else if(FunHitTest(point,&m_hitTest))
	{
		if(!m_hitTest.bHead && m_hitTest.iItem != m_iHotItem && m_hitTest.iItem >=0 && (m_uPrivateStyle & LIST_HOTITEM))
		{
			if(m_hTimer[1] == NULL)
				m_hTimer[1] = SetTimer(11,100,NULL);	
			m_iHotItem = m_hitTest.iItem;
			Invalidate(FALSE);
		}
	}
	else if(m_iHotItem>=0 && (m_uPrivateStyle & LIST_HOTITEM))// 表头 或 Item Over
	{	
		m_iHotItem  = -1;
		Invalidate(FALSE);		
		if(m_hTimer[1]) KillTimer(m_hTimer[1]);
		m_hTimer[1] = 0;
	}
}

void CTPListCtrlEx::OnMouseMoveList(UINT nFlags, CPoint point)
{
	if(m_iDragMode == TP_DRAG_RECT ||  TP_DRAG_RECTOK == m_iDragMode)
	{
#ifdef  TP_NS_EDIT
		if(!(nFlags & (MK_LBUTTON | MK_RBUTTON)))   m_iDragMode = TP_DRAG_UNKNOW;
#else
		if(!(nFlags & MK_LBUTTON))   m_iDragMode = TP_DRAG_UNKNOW;
#endif		
		else if(m_ptMiddle != point)
		{
			if(m_iDragMode == TP_DRAG_RECT)
			{
				SetCapture();
				m_iDragMode = TP_DRAG_RECTOK;
				::SetCursor(m_hCursorDrag[0] ->GetCursor()); 
			}
			else
			{
				//自动移动位置
				if(point.y < m_rtClient.top  || point.y >m_rtClient.bottom)
				{
					if(	m_hTimer[3] == NULL)
						m_hTimer[3] = SetTimer(13,100,NULL);
					//return; qushaojie DQAdb00008999 20100115 直接返回画的框就不对了
				}				
				if(point.x < m_rtClient.left  || point.x >m_rtClient.right)
				{
					if(	m_hTimer[2] == NULL)
						m_hTimer[2] = SetTimer(12,100,NULL);
					//return; qushaojie DQAdb00008999 20100115 直接返回画的框就不对了
				}			

				BOOL bPaint = FALSE;
				CRect rtRect = CRect(m_ptSave.x,m_ptSave.y,point.x + m_iTopX,point.y + m_iTopY),rtItem;
				rtRect.NormalizeRect();	
				for(INT l=0;l<m_aItem.GetSize();l++)
				{
					//rtItem = m_aItem[l]->aSubItem[0] ->rtItem;
					//add by qushaojie 20090915
					GetItemRect(l,rtItem,LVIR_BOUNDS);
					rtItem.OffsetRect(m_iTopX,m_iTopY);
					rtItem &= rtRect;

					if(rtItem.Width()>0 && rtItem.Height() >0)
					{
						m_aItem[l]->dwSelTick = GetTickCount() + l;
						if(!(m_aItem[l]->uState & LVIS_SELECTED))
						{		
							m_aItem[l]->uState  |= LVIS_SELECTED;
							if(!bPaint)
							{
								bPaint = TRUE;
								FunNotifyParent(LVN_ITEMCHANGED,l,0);
							}							
						}
					}
					else
					{
						if(m_aItem[l]->uState & LVIS_SELECTED)
						{
							//song
							if(TP_IsCtrlKeyDown() || TP_IsShiftKeyDown());
							else 
							{
								m_aItem[l]->uState &= ~LVIS_SELECTED;
								if(!bPaint)
								{
									bPaint = TRUE;
									FunNotifyParent(LVN_ITEMCHANGED,l,0);
								}
							}
						}
					}
				}

				if(bPaint) {Invalidate(FALSE);m_rtInvert = CRect(0,0,0,0);}

				CDC *pDC = GetDC();
				if(m_rtInvert.Width() >0) pDC->DrawFocusRect(m_rtInvert);
				m_rtInvert = CRect(m_ptSave.x,m_ptSave.y,point.x + m_iTopX,point.y + m_iTopY);
				m_rtInvert.OffsetRect( -m_iTopX, -m_iTopY);
				m_rtInvert.NormalizeRect();
				pDC->DrawFocusRect(m_rtInvert);
				ReleaseDC(pDC);
			}
			m_ptMiddle = point;
		}
	}
	else 	if(m_iDragMode == TP_DRAG_ITEM ||  TP_DRAG_ITEMOK == m_iDragMode)
	{
#ifdef  TP_NS_EDIT
		if(!(nFlags & (MK_LBUTTON | MK_RBUTTON)))   m_iDragMode = TP_DRAG_UNKNOW;
#else
		if(!(nFlags & MK_LBUTTON))   m_iDragMode = TP_DRAG_UNKNOW;
#endif		
		else if(m_ptDrag != point)
		{
			if(m_iDragMode == TP_DRAG_ITEM   && (abs(m_ptDrag.x - point.x) > 6 || abs(m_ptDrag.y - point.y) > 6))
			{
				if(m_hitTest.iItem>=0&&m_hitTest.iItem<m_aItem.GetSize() && !(m_aItem[m_hitTest.iItem] ->uState & LVIS_SELECTED))
					FunSelectItem(m_hitTest.iItem,FALSE);
				SetCapture();
				m_iDragMode = TP_DRAG_ITEMOK;
				if(m_uPrivateStyle & LIST_DRAGITEM)
				{							
					if(m_hDragCursor) ::DestroyIcon(m_hDragCursor);
					m_hDragCursor = CreateDragIcon(-1,TP_GetSysColor(m_iBackColor),TRUE,&point,m_hCursorDrag[3] ->GetCursor());
					::SetCursor(m_hDragCursor); 
				}
				else
				{
					EndEdit();
					FunNotifyParent(LVN_BEGINDRAG,0,0);
				}
			}		
			if((m_uPrivateStyle & LIST_DRAGITEM) && TP_DRAG_ITEMOK == m_iDragMode)
			{
				//自动移动位置
				if(point.y < m_rtClient.top  || point.y >m_rtClient.bottom)
				{
					if(	m_hTimer[3] == NULL)
						m_hTimer[3] = SetTimer(13,100,NULL);
				}				
				//计算插入位置
				CTPIntArray selArray1;
				FunGetCurSel(selArray1);
				if(FunHitTest(point,&m_hitTest))
				{
					m_ptDrag = point;
					if(!m_hitTest.bHead && m_iDragIndex != m_hitTest.iItem)
					{
						if(selArray1.GetSize()>0 && m_hitTest.iItem>=selArray1[0] && m_hitTest.iItem <= selArray1[selArray1.GetSize()-1]+1)
						{
							if(m_iDragIndex != -1)
							{
								m_iDragIndex = -1;
								Invalidate();
							}
							else	m_iDragIndex = -1;
						}

						else
						{
							m_iDragIndex = m_hitTest.iItem;
							Invalidate();
						}
					}
				}
				else if(point.y + m_iTopY >= m_iTotalHeight  && m_iDragIndex != m_aItem.GetSize() && point.y <m_rtClient.bottom && selArray1.GetSize()>0 &&  selArray1[selArray1.GetSize()-1] != m_aItem.GetSize()-1) 
				{
					m_iDragIndex = (int)m_aItem.GetSize() ;
					Invalidate();
				}	
			}
		}
	}	
	else if(FunHitTest(point,&m_hitTest))
	{
		if(!m_hitTest.bHead && m_hitTest.iItem != m_iHotItem && m_hitTest.iItem >=0 && (m_uPrivateStyle & LIST_HOTITEM))
		{
			if(m_hTimer[1] == NULL)
				m_hTimer[1] = SetTimer(11,100,NULL);	
			m_iHotItem = m_hitTest.iItem;
			Invalidate(FALSE);
		}
	}
	else if(m_iHotItem>=0 && (m_uPrivateStyle & LIST_HOTITEM))// 表头 或 Item Over
	{	
		m_iHotItem  = -1;
		Invalidate(FALSE);		
		if(m_hTimer[1]) KillTimer(m_hTimer[1]);
		m_hTimer[1] = 0;
	}
}
void CTPListCtrlEx::OnMouseMoveBuffer(UINT nFlags, CPoint point)
{
	OnMouseMoveLargeIcon(nFlags,point);
}

void CTPListCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_iViewType == VIEWTYPE_REPORT)      	  OnMouseMoveReport(nFlags, point);
	else if(m_iViewType == VIEWTYPE_TEXTPICTURE)  OnMouseMoveReport(nFlags, point);      		     	
	else if(m_iViewType == VIEWTYPE_ICON)   	  OnMouseMoveLargeIcon(nFlags, point);
	else if(m_iViewType == VIEWTYPE_INFO)	      OnMouseMoveInfo(nFlags, point);
	else if(m_iViewType == VIEWTYPE_BUFFER) 	  OnMouseMoveBuffer(nFlags, point);
	else if(m_iViewType == VIEWTYPE_LIST)   	  OnMouseMoveList(nFlags, point);
}
void CTPListCtrlEx::OnMouseMoveInfo(UINT nFlags, CPoint point)
{
	OnMouseMoveList(nFlags, point);
}
void CTPListCtrlEx::OnMouseMoveReport(UINT nFlags, CPoint point) 
{	
	if(m_iDragMode == TP_DRAG_RECT ||  TP_DRAG_RECTOK == m_iDragMode)
	{
#ifdef  TP_NS_EDIT
		if(!(nFlags & (MK_LBUTTON | MK_RBUTTON)))   m_iDragMode = TP_DRAG_UNKNOW;
#else
		if(!(nFlags & MK_LBUTTON))   m_iDragMode = TP_DRAG_UNKNOW;
#endif		
		else if(m_ptMiddle != point)
		{
			if(m_iDragMode == TP_DRAG_RECT)
			{
				SetCapture();
				m_iDragMode = TP_DRAG_RECTOK;
				::SetCursor(m_hCursorDrag[0] ->GetCursor()); 
			}
			else
			{
				//自动移动位置
				if(point.y < m_rtClient.top  || point.y >m_rtClient.bottom)
				{
					if(	m_hTimer[3] == NULL)
						m_hTimer[3] = SetTimer(13,100,NULL);
					//return; qushaojie DQAdb00008999 20100115 直接返回画的选择虚框就不对了
				}				
				if(point.x < m_rtClient.left  || point.x >m_rtClient.right)
				{
					if(	m_hTimer[2] == NULL)
						m_hTimer[2] = SetTimer(12,100,NULL);
					//return;
				}				

				BOOL bPaint = FALSE;
				CRect rtRect = CRect(m_ptSave.x,m_ptSave.y,point.x + m_iTopX,point.y + m_iTopY),rtItem;
				rtRect.NormalizeRect();	
				for(INT l=0;l<m_aItem.GetSize();l++)
				{
					//add by qushaojie 20090915
					//rtItem = m_aItem[l]->aSubItem[0] ->rtItem;
					//rtItem.right = m_aItem[l]->aSubItem[m_aItem[l]->aSubItem.GetSize()-1] ->rtItem.right;
					GetItemRect(l,rtItem,LVIR_BOUNDS);
					rtItem.OffsetRect(m_iTopX,m_iTopY);
					rtItem &= rtRect;

					if(rtItem.Width()>0 && rtItem.Height() >0)
					{
						m_aItem[l]->dwSelTick = GetTickCount() + l;
						if(!(m_aItem[l]->uState & LVIS_SELECTED))
						{	
							m_aItem[l]->uState  |= LVIS_SELECTED;
							if(!bPaint)
							{
								bPaint = TRUE;
								FunNotifyParent(LVN_ITEMCHANGED,l,0);
							}
						}
					}
					else
					{
						if(m_aItem[l]->uState & LVIS_SELECTED)
						{					
							if(TP_IsCtrlKeyDown() || TP_IsShiftKeyDown());
							else 
							{
								m_aItem[l]->uState &= ~LVIS_SELECTED;
								if(!bPaint)
								{
									bPaint = TRUE;
									FunNotifyParent(LVN_ITEMCHANGED,l,0);
								}
							}
						}
					}
				}
				if(bPaint){Invalidate(FALSE);m_rtInvert = CRect(0,0,0,0);}

				CDC *pDC = GetDC();
				if(m_rtInvert.Width() >0) pDC->DrawFocusRect(m_rtInvert);
				m_rtInvert = CRect(m_ptSave.x,m_ptSave.y,point.x + m_iTopX,point.y + m_iTopY);
				m_rtInvert.OffsetRect( -m_iTopX, -m_iTopY);
				m_rtInvert.NormalizeRect();
				pDC->DrawFocusRect(m_rtInvert);
				ReleaseDC(pDC);
			}
			m_ptMiddle = point;
		}
	}
	else 	if(m_iDragMode == TP_DRAG_LEFT || TP_DRAG_LEFTOK == m_iDragMode )
	{
		m_iDragMode = TP_DRAG_LEFTOK ; 
		if(m_ptDrag.x != point.x)
		{
			TPListCtrlColumn *pHead = (TPListCtrlColumn *)m_hitTest.pData ;
			if(pHead && pHead->iIndex>0 && point.x >= m_aColumn[pHead->iIndex-1]->rtItem.left)
			{		
				CDC *pDC = GetDC();
				if(m_rtInvert.Width() >0)	pDC->InvertRect(m_rtInvert);      
				m_rtInvert = CRect(point.x,0,point.x +1,m_iColumnHeight);
				m_rtInvert.left = max(m_aColumn[pHead->iIndex-1]->rtItem.left + m_aColumn[pHead->iIndex-1]->lMinWidth,m_rtInvert.left);
				m_rtInvert.left = min(m_aColumn[pHead->iIndex-1]->rtItem.left + m_aColumn[pHead->iIndex-1]->lMaxWidth,m_rtInvert.left);
				m_rtInvert.right = m_rtInvert.left + 1;
				if(m_aItem.GetSize()>0)
					m_rtInvert.bottom = min(m_rtClient.bottom,m_aItem[m_aItem.GetSize() -1] ->aSubItem[0]->rtItem.bottom);
				pDC->InvertRect(m_rtInvert); 
				ReleaseDC(pDC);
			}
		}
		m_ptDrag = point;
	}
	else if(m_iDragMode == TP_DRAG_RIGHT || TP_DRAG_RIGHTOK == m_iDragMode )
	{
		m_iDragMode = TP_DRAG_RIGHTOK ; 
		if(m_ptDrag.x != point.x)
		{
			TPListCtrlColumn *pHead = (TPListCtrlColumn *)m_hitTest.pData ;
			if(pHead &&point.x >= pHead->rtItem.left )
			{				
				CDC *pDC = GetDC();
				if(m_rtInvert.Width() >0) pDC->InvertRect(m_rtInvert); 
				m_rtInvert = CRect(point.x,0,point.x +1,m_iColumnHeight);
				m_rtInvert.left = max(pHead->rtItem.left + pHead->lMinWidth,m_rtInvert.left);
				m_rtInvert.left = min(pHead->rtItem.left + pHead->lMaxWidth,m_rtInvert.left);
				m_rtInvert.right = m_rtInvert.left + 1;
				if(m_aItem.GetSize()>0)
					m_rtInvert.bottom = min(m_rtClient.bottom,m_aItem[m_aItem.GetSize() -1] ->aSubItem[0]->rtItem.bottom);
				pDC->InvertRect(m_rtInvert); 
				ReleaseDC(pDC);
			}
		}
		m_ptDrag = point;
	}
	else if(m_iDragMode == TP_DRAG_MIDDLE || TP_DRAG_MIDDLEOK == m_iDragMode )
	{
		if(m_iDragMode == TP_DRAG_MIDDLE && m_ptDrag != point)
		{
			::SetCursor(m_hCursorDrag[1] ->GetCursor()); 
			SetCapture();		
			m_iDragMode = TP_DRAG_MIDDLEOK ;
			m_ptMiddle = point - m_aColumn[m_hitTest.iItem] ->rtItem.TopLeft();
			if(m_pDragImageList) 
			{
				m_pDragImageList ->DeleteImageList();
				delete m_pDragImageList;
			}
			m_pDragImageList = CreateHeadDragImage(/*m_hitTest.iHeadFlag*/m_hitTest.iItem); // LI HUI I 2007-11-28 Modify to iItem
			if(m_pDragImageList)
			{
				m_pDragImageList ->BeginDrag(0,CPoint(0,0));
				m_pDragImageList ->DragEnter(this,CPoint(0,0));
			}
		}
		if(m_ptDrag.x != point.x && m_iDragMode == TP_DRAG_MIDDLEOK)
		{
			//移动
			if(point.x < m_rtClient.left  || point.x >m_rtClient.right)
			{
				if(	m_hTimer[2] == NULL)
					m_hTimer[2] = SetTimer(12,100,NULL);
			}
			TPListCtrlColumn *pHead = (TPListCtrlColumn *)m_hitTest.pData ;
			if(pHead)
			{
				CPoint ptImage;
				int iDragIndex = FunGetHeadInsertIndex(point);		
				if(iDragIndex >m_iLockDragColumn && iDragIndex != m_hitTest.iItem && iDragIndex != m_hitTest.iItem + 1 && iDragIndex >=0)
				{
					if(m_iDragIndex != iDragIndex)
					{					
						m_pDragImageList ->DragLeave(this);
						m_iDragIndex = iDragIndex;
						ptImage      = CPoint(point.x - m_ptMiddle.x,2);
						InvalidateRect(CRect(0,0,m_rtClient.right,m_iColumnHeight));
						UpdateWindow();//TFS 2621   DQAdb00008693 导致image draw的坐标以屏幕左上计算，而非期望的listctrl左上					
						m_pDragImageList ->DragEnter(this,ptImage);
					}
				}							
				else if(m_iDragIndex >=0)
				{					
					m_pDragImageList ->DragLeave(this);
					m_iDragIndex = -1;
					ptImage      = CPoint(point.x - m_ptMiddle.x,2);
					InvalidateRect(CRect(0,0,m_rtClient.right,m_iColumnHeight));				
					UpdateWindow();
					m_pDragImageList ->DragEnter(this,ptImage);
				}
			}
			m_pDragImageList ->DragMove(CPoint(point.x - m_ptMiddle.x,2));			
		}
		m_ptDrag = point;
	}		
	else if(m_iDragMode == TP_DRAG_ITEM ||  TP_DRAG_ITEMOK == m_iDragMode)
	{
#ifdef  TP_NS_EDIT
		if(!(nFlags & (MK_LBUTTON | MK_RBUTTON)))   m_iDragMode = TP_DRAG_UNKNOW;
#else
		if(!(nFlags & MK_LBUTTON))   m_iDragMode = TP_DRAG_UNKNOW;
#endif		
		else if(m_ptDrag != point && m_aItem.GetSize()>0)
		{			
			CRect rtItem(0,0,0,0);
			//rtItem.right = m_aItem[0]->aSubItem[m_aItem[0]->aSubItem.GetSize()-1] ->rtItem.right;
            GetItemRect(m_hitTest.iItem,rtItem,LVIR_BOUNDS);
			if(m_iDragMode == TP_DRAG_ITEM &&  (point.x < rtItem.right)  && (abs(m_ptDrag.x - point.x) > 6 || abs(m_ptDrag.y - point.y) > 6))
			{
				if(m_hitTest.iItem>=0&&m_hitTest.iItem<m_aItem.GetSize() && !(m_aItem[m_hitTest.iItem] ->uState & LVIS_SELECTED))
					FunSelectItem(m_hitTest.iItem,FALSE);
				SetCapture();
				m_iDragMode = TP_DRAG_ITEMOK;
				if(m_uPrivateStyle & LIST_DRAGITEM)
				{
					if(m_hDragCursor) ::DestroyIcon(m_hDragCursor);
					m_hDragCursor = CreateDragIcon(-1,TP_GetSysColor(m_iBackColor),TRUE,&point,m_hCursorDrag[3] ->GetCursor());
					::SetCursor(m_hDragCursor); 
				}
				else
				{
					EndEdit();
					FunNotifyParent(LVN_BEGINDRAG,0,0);
				}
			}		
			if((m_uPrivateStyle & LIST_DRAGITEM) && TP_DRAG_ITEMOK == m_iDragMode)
			{
				//自动移动位置
				if(point.y < m_rtClient.top  || point.y >m_rtClient.bottom)
				{
					if(	m_hTimer[3] == NULL)
						m_hTimer[3] = SetTimer(13,100,NULL);
				}				
				//计算插入位置
				CTPIntArray selArray1;
				FunGetCurSel(selArray1);
				if(FunHitTest(point,&m_hitTest))
				{
					m_ptDrag = point;
					if(!m_hitTest.bHead && m_iDragIndex != m_hitTest.iItem)
					{
						if(selArray1.GetSize()>0 && m_hitTest.iItem>=selArray1[0] && m_hitTest.iItem <= selArray1[selArray1.GetSize()-1]+1)
						{
							if(m_iDragIndex != -1)
							{
								m_iDragIndex = -1;
								Invalidate();
							}
							else	m_iDragIndex = -1;
						}

						else
						{
							m_iDragIndex = m_hitTest.iItem;
							Invalidate();
						}
					}
				}
				else if(point.y + m_iTopY >= m_iTotalHeight  && m_iDragIndex != m_aItem.GetSize() && point.y <m_rtClient.bottom && selArray1.GetSize()>0 &&  selArray1[selArray1.GetSize()-1] != m_aItem.GetSize()-1) 
				{
					m_iDragIndex = (int)m_aItem.GetSize() ;
					Invalidate();
				}	
			}
		}
	}	
	else if(FunHitTest(point,&m_hitTest))
	{
		if(m_hitTest.bHead && m_hitTest.iItem != m_iHeadOver && m_hitTest.iItem >=0 && (m_uPrivateStyle & LIST_HOTHEAD))
		{
			if(m_hTimer[0] == NULL)
				m_hTimer[0] = SetTimer(10,100,NULL);
			m_iHeadOver = m_hitTest.iItem;
			InvalidateRect(CRect(0,0,m_rtClient.right,m_iColumnHeight),FALSE);
		}
		else if(!m_hitTest.bHead && m_hitTest.iItem != m_iHotItem && m_hitTest.iItem >=0 && (m_uPrivateStyle & LIST_HOTITEM))
		{
			if(m_hTimer[1] == NULL)
				m_hTimer[1] = SetTimer(11,100,NULL);	
			m_iHotItem = m_hitTest.iItem;
			Invalidate(FALSE);
		}
	}
	else if(m_iHeadOver>=0  && (m_uPrivateStyle & LIST_HOTHEAD) || m_iHotItem>=0 && (m_uPrivateStyle & LIST_HOTITEM))// 表头 或 Item Over
	{
		m_iHeadOver = -1;
		m_iHotItem  = -1;
		Invalidate(FALSE);

		if(m_hTimer[1]) KillTimer(m_hTimer[1]);
		m_hTimer[1] = 0;
		if(m_hTimer[0]) KillTimer(m_hTimer[0]);
		m_hTimer[0] = 0;
	}
}
void CTPListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_uState |= STATE_DBCLICK;
	if(m_iViewType == VIEWTYPE_REPORT)      	  OnLButtonDblClkReport(nFlags, point);
	else if(m_iViewType == VIEWTYPE_TEXTPICTURE)  OnLButtonDblClkReport(nFlags, point);  
	else if(m_iViewType == VIEWTYPE_ICON)   	  OnLButtonDblClkLargeIcon(nFlags, point);
	else if(m_iViewType == VIEWTYPE_INFO)	      OnLButtonDblClkInfo(nFlags, point);
	else if(m_iViewType == VIEWTYPE_BUFFER) 	  OnLButtonDblClkBuffer(nFlags, point);
	else if(m_iViewType == VIEWTYPE_LIST) 	      OnLButtonDblClkList(nFlags, point);
	m_iItemLast = -1;

} 
void CTPListCtrlEx::OnLButtonDblClkLargeIcon(UINT nFlags, CPoint point) 
{
	if(!FunHitTest(point,&m_hitTest)) return;
	if(m_hitTest.iItem >=0) FunNotifyParent(NM_DBLCLK,m_hitTest.iItem,0);	
}
void CTPListCtrlEx::OnLButtonDblClkList(UINT nFlags, CPoint point) 
{
	if(!FunHitTest(point,&m_hitTest)) return;
	if(m_hitTest.iItem >=0) FunNotifyParent(NM_DBLCLK,m_hitTest.iItem,0);	
}
void CTPListCtrlEx::OnLButtonDblClkBuffer(UINT nFlags, CPoint point) 
{
	if(!FunHitTest(point,&m_hitTest)) return;
	if(m_hitTest.iItem >=0) FunNotifyParent(NM_DBLCLK,m_hitTest.iItem,0);	
}
void CTPListCtrlEx::OnLButtonDblClkInfo(UINT nFlags, CPoint point) 
{
	if(!FunHitTest(point,&m_hitTest)) return;
	if(m_hitTest.iItem >=0) FunNotifyParent(NM_DBLCLK,m_hitTest.iItem,0);	
}
void CTPListCtrlEx::OnLButtonDblClkReport(UINT nFlags, CPoint point) 
{
	if(!FunHitTest(point,&m_hitTest)) return;
	if(m_hitTest.bHead)
	{
		if(m_uStyle & LVS_NOSORTHEADER) return;		
		if(m_hitTest.iItem >=0 && m_hitTest.iHeadFlag ==1)	    FunAdjColumnWidth(m_hitTest.iItem-1);
		else if(m_hitTest.iItem >=0 && m_hitTest.iHeadFlag ==2)	FunAdjColumnWidth(m_hitTest.iItem);			
/*		// XN0024351 双击时没有必要通知单击
		if(m_hitTest.iItem >=0 && m_hitTest.iHeadFlag != 1 && m_hitTest.iHeadFlag != 2)
			FunNotifyParent(HDN_ITEMCLICK,m_hitTest.iItem,0);*/	
	}
	else	if(m_hitTest.iItem >=0) FunNotifyParent(NM_DBLCLK,m_hitTest.iItem,0);	

}
LRESULT CTPListCtrlEx::FunNotifyParentAll()
{
	m_uState &= ~STATE_NOSENDNOTIFY;
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l]->uNotify)
		{
			FunNotifyParent(m_aItem[l]->uNotify,l,-1);
			m_aItem[l]->uNotify = 0;
		}
	}
	return S_OK;
}
LRESULT CTPListCtrlEx::FunNotifyParent(UINT nNotify,int iItem,int iSubItem) 
{
	if((STATE_NOSENDNOTIFY & m_uState) && (LVN_ITEMCHANGED == nNotify))
	{
		if(iItem>=0 && iItem <	m_aItem.GetSize())
			m_aItem[iItem] ->uNotify |= LVN_ITEMCHANGED;
		return S_OK;
	}
	else if((int)nNotify <=(int)HDN_FIRST && (int)nNotify >=(int)HDN_TRACK || IsViewItem(iItem) )
	{
		NMLISTVIEW nmList;
		nmList.hdr.code =  nNotify;
		nmList.hdr.hwndFrom = m_hWnd;
		nmList.hdr.idFrom   = GetDlgCtrlID();
		nmList.iItem        = iItem;
		nmList. iSubItem    = iSubItem;
		nmList. uNewState   = 0;
		nmList. lParam      = 0;
		nmList. uChanged    = 0;

		CWnd *pWnd = m_pMsgWnd;
		if(nNotify >=  HDN_FIRST)   
		{
			if(iItem>=0 && iItem <	m_aItem.GetSize())
			{
				if(m_aItem[iItem]->bCheck)
				{
					m_aItem[iItem]->uState &= ~0x00001000;
					m_aItem[iItem]->uState |=  0x00002000;
				}
				else
				{
					m_aItem[iItem]->uState &= ~0x00002000;
					m_aItem[iItem]->uState |=  0x00001000;
				}

				nmList. uNewState   = m_aItem[iItem]->uState;
				nmList. lParam      = m_aItem[iItem]->dwData;
				nmList. uOldState   = m_aItem[iItem]->uStateOld;
				m_aItem[iItem]->uStateOld = m_aItem[iItem]->uState;
			}
		}
		else
		{
			if(iItem>=0 && iItem <	m_aColumn.GetSize())
				nmList. uNewState   = iItem>=0? m_aColumn[iItem]->uFmt : 0;
			pWnd = this;
		}

		GetCursorPos(&nmList. ptAction);
		ScreenToClient(&nmList. ptAction);

		if((nNotify == LVN_ITEMCHANGED) && m_iCheckIndex == iItem) 
			m_iCheckIndex = -1;	

		return pWnd ->SendMessage(WM_NOTIFY,nmList.hdr.idFrom,(LPARAM)&nmList);		
	}	
	else return S_OK;
}

int CTPListCtrlEx::FunGetHeadInsertIndex(CPoint point)
{
	int iDestIndex = -1;
	if(point.x  < m_aColumn[0]->rtItem .left )                              iDestIndex = 0;
	else
	{
		INT nLastIndex = (INT)m_aColumn.GetSize()-1;
		for(; nLastIndex >= 0; nLastIndex --)
		{
			if(m_aColumn[nLastIndex]->bShow) break;
		}
		if(point.x > m_aColumn[nLastIndex]->rtItem .right )
		{
			iDestIndex = nLastIndex + 1;
		}
	else
	{
		for(int i=0;i<m_aColumn.GetSize();i++)
		{
			if(point.x  >= m_aColumn[i]->rtItem.left  && point.x  <=m_aColumn[i]->rtItem.left+m_aColumn[i]->rtItem.Width()/2)
			{
				iDestIndex = i;
				break;
			}
			else if(point.x  >= m_aColumn[i]->rtItem.left +m_aColumn[i]->rtItem.Width()/2 && point.x  <=m_aColumn[i]->rtItem.right )
			{
				iDestIndex = i+1;
				break;
			}
		}
		}
	}
	return iDestIndex;
}

void CTPListCtrlEx::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 100)
	{
		KillTimer(100);
		CWnd *pFocus = GetFocus();// LI HUI LI XN00026435 当前焦点是自己窗口时才处理，因为可能在点击之后，立即进行其它操作
		if(m_iItemLast>=0 && this == pFocus)
		{
			if(GetItemState(m_iItemLast, LVIS_SELECTED))// LI HUI LI snp4100033310 因为在LButtonu时，才会重新edittest
			{
				BeginEdit();
			}
		}
	}	
	else if(nIDEvent == 10)
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		TPListCtrlHitTest HitTest;

		if(!FunHitTest(point,&HitTest) || !HitTest.bHead || HitTest.iItem<0)
		{			
			m_iHeadOver = -1;			
			if(GetKeyState(VK_LBUTTON) <0) InvalidateRect(m_rtClient,FALSE);
			else                           InvalidateRect(CRect(0,0,m_rtClient.right,m_iColumnHeight),FALSE);
			KillTimer(m_hTimer[0]);
			m_hTimer[0] = 0;
		}
	}
	else if(nIDEvent == 11)
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		TPListCtrlHitTest HitTest;

		if(!FunHitTest(point,&HitTest) || HitTest.bHead || HitTest.iItem<0)
		{			
			m_iHotItem = -1;				 
			Invalidate(NULL);
			KillTimer(m_hTimer[1]);
			m_hTimer[1] = 0;
		}
	}
	else if(nIDEvent == 12)
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		int iMax, iMin,iPos = m_iTopX;
		((CTPScrollBar *)m_pScrollBar ->pHScroll)->GetScrollRange(&iMin,&iMax);
		iMax -= m_rtClient.Width();
		if(!m_pScrollBar ->bShowHScrollBar) iMin = iMax = 0;
		if(point.x < m_rtClient.left)		iPos -= 10;
		else if(point.x > m_rtClient.right)	iPos += 10;
		else  {KillTimer(m_hTimer[2]); m_hTimer[2] = NULL; return;}
		iPos = max(iMin,iPos);
		iPos = min(iMax,iPos);		
		if(iPos != m_iTopX)
		{		
			m_iTopX = iPos;
			((CTPScrollBar *)m_pScrollBar ->pHScroll)->SetScrollPos(m_iTopX); 
			if(m_pScrollBar ->bShowHScrollBar)	 
			{
				FunCalcHeadSize();	
				FunCalcItemSize();
				Invalidate();
				UpdateWindow();
			}
		}
	}
	else if(nIDEvent == 13)
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		int iMax, iMin,iPos = m_iTopY;
		((CTPScrollBar *)m_pScrollBar ->pVScroll)->GetScrollRange(&iMin,&iMax);
		iMax -= m_rtClient.Height();
		if(!m_pScrollBar ->bShowVScrollBar) iMin = iMax = 0;
		if(point.y < m_rtClient.top)	    	iPos -= 10;
		else if(point.y > m_rtClient.bottom)	iPos += 10;
		else  {KillTimer(m_hTimer[3]); m_hTimer[3] = NULL; return;}
		iPos = max(iMin,iPos);
		iPos = min(iMax,iPos);
		if(iPos != m_iTopY)
		{		
			m_iTopY = iPos;
			((CTPScrollBar *)m_pScrollBar ->pVScroll)->SetScrollPos(m_iTopY); 
			if(m_pScrollBar ->bShowVScrollBar)	 
			{
				FunCalcHeadSize();	
				FunCalcItemSize();
				Invalidate();
				UpdateWindow();
			}
		}
	}
	else CTPWnd::OnTimer(nIDEvent);
}



BOOL CTPListCtrlEx::OnSetCursor(CWnd* pWnd, UINT nFunHitTest, UINT message)
{
	if(m_iViewType == VIEWTYPE_REPORT || m_iViewType == VIEWTYPE_TEXTPICTURE)
	{
		if(m_iDragMode == TP_DRAG_UNKNOW)
		{
			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);
			TPListCtrlHitTest HitTest;
			if(FunHitTest(point,&HitTest) && HitTest.bHead && HitTest.iItem>=0)
			{
				if(HitTest.iHeadFlag == 1 && HitTest.iItem>0) { SetCursor(m_hCursorDrag[2] ->GetCursor()); return TRUE;}
				else if(HitTest.iHeadFlag == 2)               { SetCursor(m_hCursorDrag[2] ->GetCursor()); return TRUE;}
			}
		}
	}
	return CTPWnd::OnSetCursor(pWnd, nFunHitTest, message);
}
int CTPListCtrlEx::FunGetCurSel(CTPIntArray &aSel)
{
	aSel.RemoveAll();
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l] ->uState &LVIS_SELECTED)  aSel.Add(l);
	}
	return (int)aSel.GetSize();
}
void CTPListCtrlEx::FunMoveSelectItemTo(int iItem)
{
	CTPIntArray selArray;
	if(iItem<0 || iItem>m_aItem.GetSize()) return;
	FunGetCurSel(selArray);
	if(selArray.GetSize()<=0) return;	

	if(iItem ==m_aItem.GetSize())
	{
		int i=0;
		for(i=0;i<selArray.GetSize();i++)
		{
			TPListCtrlItem *pItem = m_aItem[selArray[i]];
			m_aItem.Add(pItem);
		}
		for(i=(int)selArray.GetSize()-1;i>=0;i--)
			m_aItem.RemoveAt(selArray[i]);
	}
	else
	{
		TPListCtrlItem *pItem ;
		TPListCtrlItemArray   arrItem;
		for(int i=(int)selArray.GetSize()-1;i>=0;i--)
		{
			pItem = m_aItem[selArray[i]];
			arrItem.Add(pItem); 
		}

		if(iItem<selArray[0])
		{
			int i= 0;
			for(i=(int)selArray.GetSize()-1;i>=0;i--)
			{
				m_aItem.RemoveAt(selArray[i]);
			}
			for(i=0;i<arrItem.GetSize();i++)
			{
				pItem = arrItem[i];
				m_aItem.InsertAt(iItem,pItem);
			}
		}
		else 	if(iItem > selArray[selArray.GetSize()-1])
		{
			for(int i=(int)selArray.GetSize()-1;i>=0;i--)
			{
				m_aItem.RemoveAt(selArray[i]);
			}
			for(int i=0;i<arrItem.GetSize();i++)
			{
				pItem = arrItem[i];
				m_aItem.InsertAt(iItem - selArray.GetSize(),pItem);
			}

		}		
		arrItem.RemoveAll(); 
	}
	selArray.RemoveAll();
}

void CTPListCtrlEx::FunSelectItem(int iFunHitTest,BOOL bDown)
{
	CTPIntArray selArray1;
	FunGetCurSel(selArray1);
	BOOL bCtrlDown  = TP_IsCtrlKeyDown();
	BOOL bShiftDown = TP_IsShiftKeyDown();	


	m_iDragIndex = -1;
	if(!bDown) //鼠标抬起
	{
		//add by qushaojie 20100416 ctrl选中之后，用shift多选，按ctrl选中的最后一个为起点
		BOOL bCtrlShift = FALSE;
		if (bCtrlDown)
		{
			m_uState |= STATE_CTRLSHIFT;
		}
		else if (bShiftDown && (m_uState & STATE_CTRLSHIFT))
		{
			m_uState &= ~STATE_CTRLSHIFT;
			bCtrlShift = TRUE;
		}
		else
		{
			m_uState &= ~STATE_CTRLSHIFT;
		}

		if(!(m_uState & STATE_SELECTED))
		{
			if((LIST_SELMORE & m_uPrivateStyle) && (bCtrlDown | bShiftDown))			
			{			
				if(bCtrlDown)
				{				
					for(int i=0;i<m_aItem.GetSize();i++)
					{
						if(!m_aItem[i]->bShow) continue;
						if(i == iFunHitTest )	
						{	
							if(m_aItem[i] ->uState & LVIS_SELECTED)
								m_aItem[i] ->uState &= ~LVIS_SELECTED;					
							else 
								m_aItem[i] ->uState |=LVIS_SELECTED;					
							FunNotifyParent(LVN_ITEMCHANGED,i,0);
							m_aItem[i]->dwSelTick = GetTickCount();						
						}
					}				
					m_uState |= STATE_SELECTED ;								
					Invalidate(FALSE);
				}
				else if(bShiftDown)
				{

					POSITION pos = GetFirstSelectedItemPosition();
					if(pos == NULL)
						return;
					int iStartSel = -1;//起始选中
					if (!bCtrlShift)
					{
						iStartSel = GetNextSelectedItem(pos);
					}
					else
					{
						while(pos)
							iStartSel = GetNextSelectedItem(pos);
					}
					
					if(iStartSel<0 || iStartSel>m_aItem.GetSize()) return;
					int  iMin = min(iStartSel,iFunHitTest),iMax = max(iStartSel,iFunHitTest);
					int  iReverse = (iStartSel > iFunHitTest ? -1 : 1);//-1表示从下向上shift选，1从上向下
					for(INT l= 0;l<m_aItem.GetSize();l++)
					{
						if(l>=iMin && l<= iMax)
						{
							if(!(m_aItem[l] ->uState  & LVIS_SELECTED))
							{
								if(!m_aItem[l]->bShow) continue;
								m_aItem[l] ->uState |=LVIS_SELECTED;					
								FunNotifyParent(LVN_ITEMCHANGED,l,0);
								m_aItem[l]->dwSelTick = GetTickCount() + (l - iStartSel)*iReverse;			
							}
						}
						else if(m_aItem[l] ->uState  & LVIS_SELECTED)
						{
							m_aItem[l] ->uState &=~LVIS_SELECTED;					
							FunNotifyParent(LVN_ITEMCHANGED,l,0);						
						}					
					}								
					m_uState |= STATE_SELECTED ;								
					Invalidate(FALSE);
				}
			}
			else
			{
				for(int i=0;i<m_aItem.GetSize();i++)
				{
					if(i == iFunHitTest )	
					{	
						if(!m_aItem[i]->bShow) continue;
						if(!(m_aItem[i] ->uState&LVIS_SELECTED))
						{
							m_aItem[i] ->uState |=LVIS_SELECTED;					
							FunNotifyParent(LVN_ITEMCHANGED,i,0);
						}
						m_aItem[i]->dwSelTick = GetTickCount();
					}
					else
					{
						if(m_aItem[i] ->uState&LVIS_SELECTED)
						{
							m_aItem[i] ->uState &= ~LVIS_SELECTED;	
							FunNotifyParent(LVN_ITEMCHANGED,i,0);
						}
					}
				}	
			}
			Invalidate(FALSE);			
		}
		m_iDragIndex = -1;
		m_uState &= ~STATE_SELECTED;
	}
	else
	{	
		BOOL bSelectClick  = FALSE;
		for(int i=0;i<selArray1.GetSize();i++) //判断是否点机选中
		{
			if(selArray1[i] == iFunHitTest )	
			{
				bSelectClick  = TRUE;
				break;
			}
		}
		if(bSelectClick && selArray1.GetSize() == 1) //当前选中 而且唯一
		{
			if(!bCtrlDown)	m_uState |= STATE_SELECTED ; //已经处理了选择信息
		}
		else if(selArray1.GetSize()<=0)
		{
			for(int i=0;i<m_aItem.GetSize();i++)
			{
				if(i == iFunHitTest )	
				{	
					if(!m_aItem[i]->bShow) continue;
					if(!(m_aItem[i] ->uState&LVIS_SELECTED))
					{
						m_aItem[i] ->uState |=LVIS_SELECTED;					
						FunNotifyParent(LVN_ITEMCHANGED,i,0);
					}
					m_aItem[i]->dwSelTick = GetTickCount();
					break;
				}
				else if(m_aItem[i] ->uState&LVIS_SELECTED)
				{
					m_aItem[i] ->uState &= ~LVIS_SELECTED;	
					FunNotifyParent(LVN_ITEMCHANGED,i,0);
				}
			}
			m_uState |= STATE_SELECTED;			
			Invalidate(FALSE);
		}
		else if((LIST_SELMORE & m_uPrivateStyle) && (bCtrlDown | bShiftDown))	
		{
		}
		else if(!bSelectClick)
		{
			for(int i=0;i<m_aItem.GetSize();i++)
			{
				if(!m_aItem[i]->bShow) continue;
				if(i == iFunHitTest )	
				{	
					m_aItem[i] ->uState |=LVIS_SELECTED;					
					FunNotifyParent(LVN_ITEMCHANGED,i,0);
					m_aItem[i]->dwSelTick = GetTickCount();	
				}
				else 
				{
					m_aItem[i] ->uState &=~LVIS_SELECTED;					
				}
			}				
			m_uState |= STATE_SELECTED ;							
			Invalidate(FALSE);
		}
	}
}
void  CTPListCtrlEx::OnStyleChanged(int nStyleType,	LPSTYLESTRUCT lpStyleStruct)
{
	if(nStyleType == GWL_STYLE)
	{
		m_uStyle = CTPWnd::GetStyle();
		if(m_uStyle & LVS_SINGLESEL) m_uPrivateStyle &= ~LIST_SELMORE;
		else                         m_uPrivateStyle |=  LIST_SELMORE;

		int iViewType = m_iViewType;
		if(m_uStyle & LVS_REPORT)          m_iViewType    = VIEWTYPE_REPORT;
		else if(m_uStyle & LVS_SMALLICON)  m_iViewType    = VIEWTYPE_INFO;
		else if(m_uStyle & LVS_LIST)       m_iViewType    = VIEWTYPE_LIST;
		else                               m_iViewType    = VIEWTYPE_ICON;
		//else                               m_iViewType    = VIEWTYPE_REPORT;

		if(iViewType != m_iViewType && m_pScrollBar) 
		{
			FunCalcSize();
			SetRedraw(m_bReDraw);
		}
		if(m_pEditWnd) EndEdit();
	}
}

BOOL CTPListCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	EndEdit();
	m_pScrollBar ->pVScroll ->SendMessage(WM_MOUSEWHEEL,MAKELONG(nFlags,zDelta),MAKELONG(pt.x,pt.y));
	return 0;
	//	return CTPWnd::OnMouseWheel(nFlags, zDelta, pt);
}
void CTPListCtrlEx::OnMenuCmd(UINT nID)
{
	switch(nID)
	{
	case ID_LISTCTRL_DELETECOLUMN:		
		if(m_iHeadMenu>=0 && m_iHeadMenu<m_aColumn.GetSize())
		{
			m_aColumn[m_iHeadMenu] ->bShow = FALSE;
			ChangeColume(0);
			FunCalcSize();
			Invalidate();
		}		
		m_iHeadMenu = -1;
		return;
	case ID_LISTCTRL_SORTASCENDING:
		SortItem(m_iHeadMenu,TRUE);
		m_iHeadMenu = -1;
		return; 
	case ID_LISTCTRL_SORTDESCENDING:
		SortItem(m_iHeadMenu,FALSE);
		m_iHeadMenu = -1;
		return;
	case ID_LISTCTRL_MOVETORIGHTCOLUMN:
		if(m_iHeadMenu>=0 && m_iHeadMenu<m_aColumn.GetSize()-1)
		{		
			INT nNextCol = m_iHeadMenu+1;
			for(INT i = nNextCol; i < m_aColumn.GetSize();i++)//snp4100025998 移动到第一个可见之后
				if(m_aColumn[i]->bShow) {nNextCol = i+1; break;}
			FunMoveColumn(m_iHeadMenu,nNextCol);
		}
		m_iHeadMenu = -1;
		return;
	case ID_LISTCTRL_MOVETOLEFTCOLUMN:
		if(m_iHeadMenu>=1 && m_iHeadMenu<m_aColumn.GetSize())
		{		
			INT nNextCol = m_iHeadMenu-1;
			for(INT i = nNextCol; i >= 0; i--)//snp4100025998 移动到第一个可见之前
				if(m_aColumn[i]->bShow) {nNextCol = i; break;}

			FunMoveColumn(m_iHeadMenu,nNextCol);
		}
		m_iHeadMenu = -1;
		return;
	default:
		if(nID>=ID_LISTCTRL_INSERTCOLUMN && nID <=ID_LISTCTRL_MOVETOLEFTCOLUMN)
		{
			int iIndex = nID - ID_LISTCTRL_INSERTCOLUMN;
			if(m_iHeadMenu>=0 && m_iHeadMenu<m_aColumn.GetSize() && iIndex>=0 && iIndex < m_aColumn.GetSize())
			{
				m_aColumn[iIndex] ->bShow = TRUE;
				if(m_ptDrag.x > m_aColumn[m_iHeadMenu]->rtItem.CenterPoint().x + m_iTopX) m_iHeadMenu ++;
				FunMoveColumn(iIndex,m_iHeadMenu);
				ReLayOut();
				FunCloumnHide();
			}	
		}
		m_iHeadMenu = -1;
		return;
	}
}
void CTPListCtrlEx::FunCloumnHide()
{
}
void CTPListCtrlEx::SetScrollPosKeep(BOOL bSave)
{
	if(m_pScrollBar == NULL) return;
	if(bSave)
	{
		if(m_pScrollBar->pHScroll)
			m_ptScrollPos.x = ((CTPScrollBar *) m_pScrollBar->pHScroll)->GetScrollPos();
		if(m_pScrollBar->pVScroll)
			m_ptScrollPos.y = ((CTPScrollBar *) m_pScrollBar->pVScroll)->GetScrollPos();
	}
	else
	{
		if(m_pScrollBar->pHScroll)
		{
			((CTPScrollBar *) m_pScrollBar->pHScroll)->SetScrollPos(m_ptScrollPos.x);
			m_iTopX = ((CTPScrollBar *) m_pScrollBar->pHScroll)->GetScrollPos();
		}
		if(m_pScrollBar->pVScroll)
		{
			((CTPScrollBar *) m_pScrollBar->pVScroll)->SetScrollPos(m_ptScrollPos.y);
			m_iTopY = ((CTPScrollBar *) m_pScrollBar->pVScroll)->GetScrollPos();
		}
		FunCalcSize();
	}
}
//add by qushaojie 20100201 headctrl弹出右键菜单在鼠标抬起时才响应XN00022317
void CTPListCtrlEx::OnRButtonUp(UINT nFlags, CPoint point)
{
	if ((m_iViewType == VIEWTYPE_REPORT ||  m_iViewType == VIEWTYPE_TEXTPICTURE))
	{
		BOOL          iShow = 0;
		CArray<int,int>     aIndex;
		CStringArray        aText;
		for(INT l=0;l<m_aColumn.GetSize();l++)
		{
			if(!m_aColumn[l]->bShow)
			{
				aIndex.Add(l);
				aText.Add(m_aColumn[l]->sItem);
			}
			else
			{
				// LI HUI LI 2008-02-19 XN00009177 
				//if(point.x>=m_aColumn[l]->rtItem.left + m_iTopX && point.x<m_aColumn[l]->rtItem.right + m_iTopX)		m_iHeadMenu = l;
				if(point.x>=m_aColumn[l]->rtItem.left && point.x<m_aColumn[l]->rtItem.right)		m_iHeadMenu = l;
				iShow ++;
			}
		}
		if(m_iHeadMenu == -1) return;
		CTPMenuEx mainLoad,*pSubMenu = NULL;
		mainLoad.LoadMenu(IDR_EXTRAMENU);
		pSubMenu = mainLoad.GetSubMenu(3);

		if(iShow <=1)            pSubMenu ->EnableMenuItem(ID_LISTCTRL_DELETECOLUMN,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);	
		if(aIndex.GetSize()<=0)  pSubMenu ->EnableMenuItem(1,MF_BYPOSITION | MF_GRAYED | MF_DISABLED);	
		else
		{
			CTPMenuEx *pAdd = pSubMenu ->GetSubMenu(1);
			if(pAdd)
			{		
				while(pAdd->GetMenuItemCount()>0) pAdd->DeleteMenu(0,MF_BYPOSITION);
				for(INT l=0;l<aIndex.GetSize();l++)
				{
					pAdd->AppendMenu(MF_BYCOMMAND|MF_STRING,ID_LISTCTRL_INSERTCOLUMN + aIndex[l],(LPTSTR)(LPCTSTR)aText[l]);
				}
			}
		}
		BOOL bMoveLeft  = FALSE;
		BOOL bMoveRigth = FALSE;
		for(INT i = 0; i < m_aColumn.GetSize(); i++)
		{
			if(!m_aColumn[i]->bShow) continue;
			if(i < m_iHeadMenu)      bMoveLeft  = TRUE;
			else if(i > m_iHeadMenu) bMoveRigth = TRUE;
		}
		if(!bMoveLeft)	  pSubMenu ->EnableMenuItem(ID_LISTCTRL_MOVETOLEFTCOLUMN,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);	
		if(!bMoveRigth)   pSubMenu ->EnableMenuItem(ID_LISTCTRL_MOVETORIGHTCOLUMN,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);	
		ClientToScreen(&point);
		pSubMenu ->TrackPopupMenu(TPM_LEFTBUTTON,point.x,point.y,this);
		mainLoad.DestroyMenu();
	}

}
void CTPListCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnEnKillfocus();
	if(GetFocus() != this) SetFocus();
	CTPWnd::OnRButtonDown(nFlags, point);
	if(m_bHeadMenu && point.y <m_iColumnHeight && (m_iViewType == VIEWTYPE_REPORT ||  m_iViewType == VIEWTYPE_TEXTPICTURE))
	{
		m_ptDrag    = point;
		m_iHeadMenu = -1;
		// li song ming 2008-9-24 XN00013871
		m_bLeftDrag     = FALSE;
	}	
	else
	{	
		int iItem = HitTest(point);	
		if(iItem <0 || GetItemState(iItem,LVIS_SELECTED) != LVIS_SELECTED) 
		{
			SetRedraw(FALSE);
			for(INT i=0;i<GetItemCount();i++) SetItemState(i,0,LVIS_SELECTED);
			if(iItem >=0) SetItemState(iItem,LVIS_SELECTED,LVIS_SELECTED);
			SetRedraw(TRUE);			
		}
		if(iItem>=0) FunNotifyParent(NM_RDBLCLK,iItem,0);
	}//]]]]]]	
}

void CTPListCtrlEx::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
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
void  CTPListCtrlEx::EditLable(int iItem,int iSubItem)
{	
	m_iItemLast    = iItem;
	int iInitSubItem = GetItemByOrder(iSubItem);
	if(iInitSubItem < 0) return;
	if(m_aColumn[iInitSubItem]->bShow ||m_iViewType == VIEWTYPE_ICON) m_iItemSubLast = iInitSubItem;
	else return;
	EnsureVisible(iItem,TRUE);
	InvalidateRect(NULL);
	UpdateWindow();
	BeginEdit();
}
void CTPListCtrlEx::CancleEdit()
{	
	m_uBaseState |=  TP_BASESTATE_CANCLEEDIT;
	EndEdit();	
	m_uBaseState &= ~TP_BASESTATE_CANCLEEDIT;
}
void CTPListCtrlEx::EndEdit()
{
	if(m_uBaseState & TP_BASESTATE_ENDEDIT) return;
	m_uBaseState |= TP_BASESTATE_ENDEDIT;
	LRESULT lResult = S_FALSE;
	if(m_pEditWnd == NULL)
	{
		m_iItemLast     = -1;
		m_iItemSubLast  = -1;
		m_iItemSave     = -1;
		m_iItemSubSave  = -1;
		m_rtItemSubSave = CRect(0,0,0,0);		
		if(m_hTimer[4]) KillTimer(m_hTimer[4]);
		m_hTimer[4] = 0;
		m_uBaseState &= ~TP_BASESTATE_ENDEDIT;
		return;		
	}
	else
	{
		lResult = FunNotifyParent(LVN_ENDLABELEDIT,m_iItemSave,m_iItemSubSave);
		if(lResult == S_OK) 
			m_pMsgWnd->SendMessage(WM_EDITLISTCTRL,TP_ENDLABELEDIT ,LPARAM(m_hWnd));
	}
	if(m_aColumn[m_iItemSubSave] ->dEditType == TP_COLOUM_EDIT)
	{		
		if(lResult == S_OK) 
		{
			CString sText;
			m_pEdit ->GetWindowText(sText);
			SetItemText(m_iItemSave,m_iItemSubSave,sText);
			if(m_pEdit)
			{	
				m_pEdit ->SendMessage(WM_UNSUBCLASSWINDOW,0,0);
				m_pEdit ->DestroyWindow();
				delete m_pEdit;
				m_pEdit = NULL;
				m_sEditLabel.Empty();
			}		
		}
		else
		{
			if(m_pEdit)
			{
				SelectItem(m_iItemSave,TRUE,TRUE);
				m_pEdit ->SetWindowText(m_sEditLabel);
				m_pEdit ->SetFocus();
				m_uBaseState &= ~TP_BASESTATE_ENDEDIT;
				return;		
			}
		}
	}
	else if(m_aColumn[m_iItemSubSave]->dEditType == TP_COLOUM_TIMECODEEDIT)
	{
		/*	m_pTimeCodeEdit ->DestroyWindow();
		delete m_pTimeCodeEdit;
		m_pTimeCodeEdit = NULL;*/
	}
	InvalidateRect(m_rtItemSubSave);
	m_iItemSave     = -1;
	m_iItemSubSave  = -1;
	m_rtItemSubSave = CRect(0,0,0,0);
	m_pEditWnd      = NULL;
	m_uBaseState &= ~TP_BASESTATE_ENDEDIT;

}
void CTPListCtrlEx::BeginEditTest(int iItem,int iSubItem)
{
	if(iItem<0||iSubItem<0) 
	{
		//XN00031026 lihuili 不符合条件，停止timer
		m_iItemLast = -1;
		if(m_hTimer[4]) KillTimer(m_hTimer[4]);
		m_hTimer[4] = 0;
		return;
	}
	if(!m_bEditItem) return;


	if(m_uState & STATE_DBCLICK)
	{
		m_iItemLast = -1;
		m_uState &= ~STATE_DBCLICK;	
		if(m_hTimer[4]) KillTimer(m_hTimer[4]);
		m_hTimer[4] = 0;
		return ;
	}
	if( m_iItemLast == iItem && m_iItemSubLast == iSubItem && m_aColumn[iSubItem] != 0) m_hTimer[4] = SetTimer(100,500,NULL);
	else 
	{
		if(m_pEditWnd) EndEdit();
		m_iItemLast    = iItem;
		m_iItemSubLast = iSubItem;
	}

}
void CTPListCtrlEx::BeginEdit()
{	
	if(m_iItemLast <0||m_iItemSubLast<0||m_iItemLast>=m_aItem.GetSize()) return;
	if(!m_aItem[m_iItemLast]->bShow) return;
	if(m_aColumn[m_iItemSubLast]->dEditType == TP_COLOUM_NULL) return;
	if(FunNotifyParent(LVN_BEGINLABELEDIT,m_iItemLast,m_iItemSubLast) == S_OK)
	{
		m_iItemSave    = m_iItemLast;
		m_iItemSubSave = m_iItemSubLast;
		m_rtItemSubSave = GetItemEditRect(m_iItemSave,m_iItemSubSave);
		if(m_aColumn[m_iItemSubSave]->dEditType == TP_COLOUM_EDIT)
		{
			CRect rtEdit;
			GetClientRect(&rtEdit);
			rtEdit &= m_rtItemSubSave;
			rtEdit.right = max(rtEdit.left+100,rtEdit.right);
			if(m_pEdit == NULL)
			{
				m_pEdit = new CTPEditEx();
				m_pEdit ->m_bClientRect    = TRUE;
				m_pEdit ->m_uPrivateStyle |= ES_WANTRETURN;
				m_pEdit ->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL|ES_WANTRETURN,rtEdit,this,10);
				if(m_aColumn[m_iItemSubSave]->lMax>0)	m_pEdit ->SetLimitText(m_aColumn[m_iItemSubSave]->lMax);

			}	
			CString sText = GetItemText(m_iItemSave,m_iItemSubSave);
			m_sEditLabel = sText;
			m_pEdit ->SetWindowText(sText);
			m_pEdit ->SetSel(0,sText.GetLength());
			m_pEdit ->ShowWindow(SW_SHOW);

			m_pEdit ->MoveWindow(rtEdit);
			m_pEdit ->SetFocus();
			m_pEditWnd = m_pEdit;
		}
	}		
	m_pMsgWnd->SendMessage(WM_EDITLISTCTRL,TP_BEGINLABELEDIT,LPARAM(m_hWnd));

	m_iItemLast    = -1;
	m_iItemSubLast = -1;
}
void     CTPListCtrlEx::SetColumeEditType(int iCol,DWORD dEditType,long lMax)
{
	if(iCol<0 || iCol>=m_aColumn.GetSize()) return;
	m_aColumn[iCol] ->dEditType = dEditType;
	m_aColumn[iCol] ->lMax      = lMax;  
}
CRect     CTPListCtrlEx::GetIconRect(int iItem)
{
	CRect rtItem = CRect(0,0,0,0);
	if(m_iViewType != VIEWTYPE_ICON && m_iViewType != VIEWTYPE_BUFFER) return rtItem;
	CSize szIcon = m_szItemBuffer;
	if(m_iViewType == VIEWTYPE_ICON) szIcon = m_szItemIcon;
	GetClientRect(&m_rtClient);
	int  iCY = m_szEdge.cy,iCx = m_szEdge.cx-m_iTopX;		
	for(int i=0;i<=iItem;i++)
	{			
		rtItem = CRect(iCx,iCY - m_iTopY,iCx + szIcon.cx,iCY + szIcon.cy - m_iTopY);
		iCx += szIcon.cx + m_szSpace.cx;
		if(iCx + szIcon.cx > m_rtClient.right)
		{
			iCx = m_szEdge.cx-m_iTopX;
			iCY += szIcon.cy + m_szSpace.cy;			
		}		
	}
	return rtItem;
}
CRect     CTPListCtrlEx::GetItemEditRect(int iItem,int iSubItem)
{
	if(iItem <0    || iItem >= m_aItem.GetSize()) return CRect(0,0,0,0);
	if(iSubItem <0 || iSubItem >= m_aColumn.GetSize()) return CRect(0,0,0,0);
	if(VIEWTYPE_ICON ==m_iViewType ) iSubItem = 0;// qsj icon模式时，取0的rtlabel
	CRect rt = m_aItem[iItem] ->aSubItem[iSubItem]->rtLable;

	rt.top     = rt.top + (rt.Height() - 18)/2;
	rt.bottom = rt.top     +  18;
	return rt;
}
TPListCtrlItem *     CTPListCtrlEx::GetItem(int iIndex)
{
	return m_aItem[iIndex];
}
void     CTPListCtrlEx::GetEditItem(int &iItem,int &iSubItem,CWnd *&pWnd)
{
	iItem    = m_iItemSave;
	iSubItem = m_iItemSubSave;
	pWnd     = m_pEditWnd;
}
void CTPListCtrlEx::OnEnKillfocus()
{	
	if(m_pEditWnd) EndEdit();	
}

BOOL CTPListCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	if(m_pToolTipCtrl && m_pToolTipCtrl ->m_hWnd)
		m_pToolTipCtrl ->RelayEvent(pMsg);
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
					m_pEdit->SetWindowText(m_sEditLabel);
					EndEdit();
					return TRUE;
				}
			}
		}
	}	
	return CTPWnd::PreTranslateMessage(pMsg);
}
void CTPListCtrlEx::ChangeColume(int iType)
{
}

void CTPListCtrlEx::ClearSortFlag(INT iItem)
{

}
void CTPListCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	//modify by qsj 20090806
	POSITION pos = GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		if(m_aItem.GetSize() > 0 && (nChar == VK_UP||nChar == VK_LEFT||nChar == VK_RIGHT||nChar == VK_DOWN))
		{
			for(INT i = 0; i < m_aItem.GetSize(); i++) // 找到第一个可见
			{
				if(m_aItem[i]->bShow)
				{
					SelectItem(i,TRUE,TRUE);
					Invalidate();
					EnsureVisible(i, TRUE);
				}
			}
    	}
		return;
	}
	int iStartSel = -1;//起始选中
	iStartSel = GetNextSelectedItem(pos);
	if(iStartSel<0 || iStartSel>m_aItem.GetSize()) return;
	int iSel = iStartSel;
	while(pos)  iSel = GetNextSelectedItem(pos);
	if(iSel<0 || iSel>m_aItem.GetSize()) return;
	BOOL bShiftDown = TP_IsShiftKeyDown();

	if (nChar == VK_UP||nChar == VK_LEFT||nChar == VK_RIGHT||nChar == VK_DOWN||nChar == VK_HOME||nChar == VK_END)
	{
		int  iEndSel   = FunEndSel(nChar,iSel);
		if (bShiftDown)//有shift
		{
			if(iEndSel == -1) return;
			DWORD dwSelTick = 0; 
			if(m_aItem.GetSize()>0) dwSelTick = m_aItem[0]->dwSelTick; 
			for (int i=0;i<m_aItem.GetSize();i++)
			{
				if (i<=max(iEndSel,iStartSel) && i>=min(iEndSel,iStartSel)) 
				{
					if(!IsViewItem(i)) continue;
					SelectItem(i,TRUE,FALSE);
					if(iEndSel > iStartSel) m_aItem[i]->dwSelTick = ++dwSelTick;
					else m_aItem[i]->dwSelTick = --dwSelTick;
				}
				else  SelectItem(i,FALSE,FALSE);
			}
		}
		else//无shift单点
		{
			//出现已经选中多个，切到方向键控制边界，设iendsel为最后一个选中的元素
			if(iEndSel == -1) iEndSel = iSel;
			if(IsViewItem(iEndSel)) SelectItem(iEndSel,TRUE,TRUE);
		}
		Invalidate();
		if(iEndSel >= 0) EnsureVisible(iEndSel,TRUE);
	}
}
void CTPListCtrlEx::GetItemResData(int iItem)
{
}
void CTPListCtrlEx::FunAdjColumnWidth(int iColumnIndex)
{
	if(iColumnIndex<0 || m_aItem.IsEmpty()) return;
	CDC   *pDC   = GetDC();
	CFont *pFont = pDC->SelectObject(TP_GetSysFont(m_iFontIndex));

	int     nMaxWidth = 0;
	int     nLeft     = 0;
	int     nWidth    = 0;	
	CRect   rcSub     = CRect(0,0,0,0);
	CRect   rtTemp    = CRect(0,0,0,0);
	CString sText;
	for (int iItem=0; iItem<m_aItem.GetCount(); ++iItem)
	{		
		GetItemResData(iItem);
		GetSubItemRect(iItem,iColumnIndex,LVIR_BOUNDS,rcSub);
		nLeft = rcSub.left;
		if (iColumnIndex == 0)
		{
			rtTemp = rcSub;
			rtTemp.DeflateRect(1,1);	
			rtTemp.right = rtTemp.left + rtTemp.Height() * TP_RESICON_WIDTH / TP_RESICON_HIGH; 	
			rtTemp.OffsetRect(2,0);
			rcSub.left = rtTemp.right + 5;
		}
		rcSub.DeflateRect(2,0);
		sText = GetItemText(iItem,iColumnIndex);
		pDC ->DrawText(sText,rcSub,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_CALCRECT);
		nWidth = rcSub.right - nLeft;
		// XN00011461 LI HUI LI 2008-05-09
		if(VIEWTYPE_TEXTPICTURE == m_iViewType) nWidth += rtTemp.right - rtTemp.left; 
		if (nWidth > nMaxWidth)	nMaxWidth = nWidth + 10;		
	}
	if (nMaxWidth < 10) 	nMaxWidth = 10;	
	if (GetColumnWidth(iColumnIndex) != nMaxWidth)	SetColumnWidth(iColumnIndex, nMaxWidth);
    ChangeColume(0);// XN00026615 qushaojie 20091211
	pDC->SelectObject(pFont);
	ReleaseDC(pDC);
}
int CTPListCtrlEx::FunMoveClip(POINT point, BOOL bHitTest, CRect& SquareRect,UINT uDragType)
{
	//1、计算offset
	int nItem=-1;
	BOOL bNotExistItemInRect= TRUE;
	int iItem = FunHitTestIcon(point);
	if (iItem<0) return 0;
	if (m_hitTest.iItem<0 || m_hitTest.iItem>=m_aItem.GetSize())return 0;
	//int iSrcItem = FunHitTestIcon(m_aItem[m_hitTest.iItem]->aSubItem[0]->rtItem.CenterPoint());
	int iSrcItem = m_aItem[m_hitTest.iItem]->lIconIndex;
	if (iSrcItem<0||iItem==iSrcItem)	return 0;
	int iOffset = iItem -  iSrcItem;

	int iCountPerLine = FunPerLineItemCount();
	//3、判断movetype和是否到达边界，并记录移动后红框的区域，iCountPerLine+1为每行元素个数
	UINT uMoveType = 0;
	if (iSrcItem/TP_LIST_COLNUMMAX>iItem/TP_LIST_COLNUMMAX) uMoveType = uMoveType |TP_CLIPMOVETYPE_UP;
	else if (iSrcItem/TP_LIST_COLNUMMAX<iItem/TP_LIST_COLNUMMAX) uMoveType = uMoveType |TP_CLIPMOVETYPE_DOWN;
	else uMoveType = uMoveType |TP_CLIPMOVETYPE_UDNOMOVE;

	if (iSrcItem%TP_LIST_COLNUMMAX>iItem%TP_LIST_COLNUMMAX) uMoveType = uMoveType |TP_CLIPMOVETYPE_LEFT;
	else if (iSrcItem%TP_LIST_COLNUMMAX<iItem%TP_LIST_COLNUMMAX) uMoveType = uMoveType |TP_CLIPMOVETYPE_RIGHT;
	else uMoveType = uMoveType |TP_CLIPMOVETYPE_LRNOMOVE;

	//获取原始rect范围,并清除超过边界的情况
	int iSrcColMin = TP_LIST_COLNUMMAX, iSrcColMax = 0;
	int iSrcRowMin = TP_LIST_ROWNUMMAX, iSrcRowMax = 0;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		int iIndexGot = GetNextSelectedItem(pos);
		nItem = m_aItem[iIndexGot]->lIconIndex;
		//右移，如果移动后元素在左边，说明越界，return
		if (uMoveType&TP_CLIPMOVETYPE_RIGHT)
		{
			if ((iOffset+nItem)%TP_LIST_COLNUMMAX<=nItem%TP_LIST_COLNUMMAX) return 0;
		}
		//左移，如果移动后元素在右边，说明越界，或者index 小于0 return
		if (uMoveType&TP_CLIPMOVETYPE_LEFT)
		{
			if ((iOffset+nItem)%TP_LIST_COLNUMMAX>=nItem%TP_LIST_COLNUMMAX || (iOffset+nItem) < 0) return 0;
		}
		//上移，如果有元素iconindex值小于0，说明越界 return
		if (uMoveType&TP_CLIPMOVETYPE_UP)
		{
			if (nItem+iOffset<0) return 0;
		}
		nItem += iOffset;
		CRect crect1 = CRect(0,0,0,0);
		crect1 = FunItemRectByIndex(nItem);

		if (crect1.top<SquareRect.top) SquareRect.top = crect1.top;
		if (crect1.left<SquareRect.left) SquareRect.left = crect1.left;
		if (crect1.right>SquareRect.right) SquareRect.right = crect1.right;
		if (crect1.bottom>SquareRect.bottom) SquareRect.bottom = crect1.bottom;
	}

	//2、判断移动后是否会产生冲突，记录移动后的区域信息，和冲突元素的集合aConflictItem
	int SquareRectColBegin=TP_LIST_COLNUMMAX,SquareRectColEnd = 0;//移动后的列起始结束值（1-n）
	int SquareRectRowBegin=TP_LIST_ROWNUMMAX,SquareRectRowEnd=0;//移动后的行起始结束值（1-n）
	CArray <int,int>aConflictItem;
	for(INT l= (int)m_aItem.GetSize() -1;l>=0;l--)
	{		
		if(m_aItem[l]->uState & LVIS_SELECTED)
		{
			int iItemCount = m_aItem[l] ->lIconIndex + iOffset;
			//记录起始行列
			if (iItemCount%TP_LIST_COLNUMMAX+1>SquareRectColEnd)    SquareRectColEnd     = iItemCount%TP_LIST_COLNUMMAX+1;
			if (iItemCount%TP_LIST_COLNUMMAX+1<SquareRectColBegin)  SquareRectColBegin   = iItemCount%TP_LIST_COLNUMMAX+1;
			if (iItemCount/TP_LIST_COLNUMMAX+1>SquareRectRowEnd)    SquareRectRowEnd     = iItemCount/TP_LIST_COLNUMMAX+1;
			if (iItemCount/TP_LIST_COLNUMMAX+1<SquareRectRowBegin)  SquareRectRowBegin   = iItemCount/TP_LIST_COLNUMMAX+1;
			//记录结束
			for(INT k= (int)m_aItem.GetSize() -1;k>=0;k--)
			{	
				//copy模式下被选中的item仍可能冲突
				//move模式下被选中的item不会冲突
				if (uDragType&TP_CLIPDRAGTYPE_MOVE)
				{
					if(m_aItem[k]->uState & LVIS_SELECTED) continue;
				}

				if(iItemCount ==  m_aItem[k] ->lIconIndex)
				{
					bNotExistItemInRect = FALSE;
					break;
				}
			}
		}
	}

	//获取冲突情况下，除了副本或源之外要移动的元素
	if (!bNotExistItemInRect )
	{			
		for (int i=0;i<m_aItem.GetSize();i++)
		{
			if (uDragType&TP_CLIPDRAGTYPE_MOVE)
			{
				if (m_aItem[i]->uState & LVIS_SELECTED) continue;
			}

			int iItemindex =m_aItem[i]->lIconIndex;
			if (iItemindex/TP_LIST_COLNUMMAX+1>=SquareRectRowBegin&&iItemindex/TP_LIST_COLNUMMAX+1<=SquareRectRowEnd)//行满足移动条件
			{
				if(iItemindex%TP_LIST_COLNUMMAX+1>=SquareRectColBegin&&iItemindex%TP_LIST_COLNUMMAX+1<=TP_LIST_COLNUMMAX-(SquareRectColEnd-SquareRectColBegin)-1)//列满足移动条件
				{
					aConflictItem.Add(i);
				}
			}	
		} 
	}

	//4、在有冲突情况吧，判断因冲突移动的元素是否有越界情况，如有越界，仍禁止移动
	//   即判断一个靠右的等大矩形区域中是否有item存在	
	if (!bNotExistItemInRect)
	{
		for(INT k= (int)m_aItem.GetSize() -1;k>=0;k--)
		{	
			//copy模式下被选中的item仍可能冲突
			//move模式下被选中的item不会冲突
			if (uDragType&TP_CLIPDRAGTYPE_MOVE)
			{
				if(m_aItem[k]->uState & LVIS_SELECTED) continue;
			}
			int iItemCount = m_aItem[k] ->lIconIndex;
			//将移动后的目标区域平移到右边界，如果平移后的区域中没有元素，允许拖动，反之禁止拖动
			if (iItemCount%TP_LIST_COLNUMMAX+1>TP_LIST_COLNUMMAX-(SquareRectColEnd-SquareRectColBegin)-1&&iItemCount/TP_LIST_COLNUMMAX+1<=SquareRectRowEnd&&iItemCount/TP_LIST_COLNUMMAX+1>=SquareRectRowBegin) return 0;
		}
	}

	//5、bHitTest为True仅测试能否移动，False则进行移动 
	if (!bHitTest)
	{
		if (uDragType&TP_CLIPDRAGTYPE_MOVE)
		{
			//不管是否有冲突,都要移动源items
			for(INT l= (int)m_aItem.GetSize() -1;l>=0;l--)
			{		
				if(m_aItem[l]->uState & LVIS_SELECTED)
				{					
					m_aItem[l]->lIconIndex += iOffset;				 
				}
			}	
		}
		//如果有冲突，移动和复制统一移动冲突区域items
		if (!bNotExistItemInRect )
		{
			//移动目的区域的item
			for (int i=0;i<aConflictItem.GetSize();i++)
			{				
				m_aItem[aConflictItem.GetAt(i)] ->lIconIndex += SquareRectColEnd-SquareRectColBegin+1;				 
			}
		}
	}

	return iOffset;
}

//end qsj
int CTPListCtrlEx::FunHitTestIcon(CPoint ptTest)
{
	int iItem     = -1;
	int iCount    = (int)m_aItem.GetSize();
	int iDistance = 0x7ffffff,iDisItem = 0;

	CRect rtItem = CRect(0,0,0,0);
	if(m_iViewType != VIEWTYPE_ICON && m_iViewType != VIEWTYPE_BUFFER) return iItem;
	CSize szIcon = m_szItemBuffer;
	if(m_iViewType == VIEWTYPE_ICON) szIcon = m_szItemIcon;
	GetClientRect(&m_rtClient);
	if(!m_rtClient.PtInRect(ptTest)) return -1; // 如果不在客户区不处理
	int  iCY = m_szEdge.cy,iCx = m_szEdge.cx-m_iTopX;		

	//changed by qushaojie 20090803
	if(m_iViewType == VIEWTYPE_ICON || m_iViewType == VIEWTYPE_BUFFER)
	{
		if(m_iArrType == ARRANGE_STORYBOARD)
		{
			int iRowHit = 0,iColHit = 0;
			iRowHit = (ptTest.y + m_iTopY)/(szIcon.cy + m_szSpace.cy);
			iColHit = (ptTest.x + m_iTopX)/(szIcon.cx + m_szSpace.cx);
			iItem = iRowHit * TP_LIST_COLNUMMAX +iColHit;
		}
		else if (m_iArrType == ARRANGE_AUTO)
		{
			for(int i=0;i<iCount + 500;i++)
			{			
				rtItem = CRect(iCx,iCY - m_iTopY,iCx + szIcon.cx,iCY + szIcon.cy - m_iTopY);
				iCx += szIcon.cx + m_szSpace.cx;
				if(iCx + szIcon.cx > m_rtClient.right)
				{
					iCx = m_szEdge.cx-m_iTopX;
					iCY += szIcon.cy + m_szSpace.cy;			
				}	
				iDisItem  = (int)sqrt(pow((double)(abs(ptTest.x - rtItem.CenterPoint().x)),2) + pow((double)(abs(ptTest.y - rtItem.CenterPoint().y)),2));
				if(iDisItem < iDistance){ iItem = i;iDistance = iDisItem;}
			}
		}
	}
	//end qsj 
	return iItem;
}
//add by qsj 20090806
//取到选中的最后一个item的值
int CTPListCtrlEx::FunEndSel(UINT nChar, int iSel)
{
	int iEndSel = -1;
	//TEXTPICTURE和REPORT模式下只考虑上下
	if (m_iViewType ==VIEWTYPE_TEXTPICTURE||m_iViewType == VIEWTYPE_REPORT)
	{
		switch (nChar)
		{
		case VK_DOWN:
			{
				INT nIndex = GetPreViewItem(-1);				
				if(nIndex >= 0 && iSel < nIndex) iEndSel = iSel+1;
			}
			break;
		case VK_UP:
			if (iSel > 0) iEndSel = GetPreViewItem(iSel);
			break; 
		case VK_LEFT:
			iEndSel = -1;
			break;
		case VK_RIGHT:
			iEndSel = -1;
			break;
		case VK_HOME:
			if(m_aItem.GetSize() > 0) GetNextViewItem(-1);
			break;
		case VK_END:
			iEndSel = (INT)GetPreViewItem(-1);
			break;
		}
	}
	else if (m_iViewType == VIEWTYPE_ICON)
	{
		switch (nChar)
		{
		case VK_DOWN:
			{
				INT iDistance = 1;
				for(INT l=0;l<m_aItem.GetSize();l++)
				{
					if(l==iSel) continue;
					if(!IsViewItem(l))continue;
					if(m_aItem[l]->aSubItem[0] ->rtItem.top > m_aItem[iSel]->aSubItem[0]->rtItem.top)
					{
						if(abs(m_aItem[l]->aSubItem[0]->rtItem.left -m_aItem[iSel]->aSubItem[0]->rtItem.left) < iDistance)
						{
							iDistance = abs(m_aItem[l]->aSubItem[0]->rtItem.left -m_aItem[iSel]->aSubItem[0]->rtItem.left);
							iEndSel = l;
						}
					}
				}
			}
			break;
		case VK_UP:
			{
				INT iDistance = 1;
				for(INT l = (INT)m_aItem.GetSize() - 1;l>= 0;l--)
				{
					if(l==iSel) continue;
					if(!IsViewItem(l)) continue;
					if(m_aItem[l]->aSubItem[0] ->rtItem.top < m_aItem[iSel]->aSubItem[0]->rtItem.top)
					{
						if(abs(m_aItem[l]->aSubItem[0]->rtItem.left -m_aItem[iSel]->aSubItem[0]->rtItem.left) < iDistance)
						{
							iDistance = abs(m_aItem[l]->aSubItem[0]->rtItem.left -m_aItem[iSel]->aSubItem[0]->rtItem.left);
							iEndSel = l;
						}
					}
				}
			}
			break;
		case VK_LEFT:
			if (iSel > 0) iEndSel =  GetPreViewItem(iSel);
			break;
		case VK_RIGHT:
			if (iSel < m_aItem.GetSize()-1) iEndSel = GetNextViewItem(iSel);
			break;
		case VK_HOME:
			if(m_aItem.GetSize() > 0) iEndSel = GetNextViewItem(-1);
			break;
		case VK_END:
			iEndSel = (INT)GetPreViewItem(-1);
			break;
		}
	}
	return iEndSel;
}

INT CTPListCtrlEx::GetPreViewItem(INT nIndex)
{
	if(nIndex < 0) nIndex = (INT)m_aItem.GetSize() -1; //从最后一个找
	else		   nIndex--;
	for(INT i = nIndex; i >= 0; i--)
	{
		if(IsViewItem(i)) return i;
	}
	return -1;
}
INT CTPListCtrlEx::GetNextViewItem(INT nIndex)
{
	if(nIndex < 0) nIndex = 0;
	else		   nIndex++;
	for(INT i = nIndex; i < m_aItem.GetSize(); i++)
	{
		if(IsViewItem(i)) return i;
	}
	return -1;
}
BOOL CTPListCtrlEx::IsViewItem(INT nIndex)
{
	if(nIndex < 0 || nIndex >= m_aItem.GetSize()) return FALSE;

	return m_aItem[nIndex]->bShow;
}

//StoryBorad 和其余模式独立 20090921 qushaojie
//lIconIndex for StoryBoard  and lReportIndex for the other modes 
void CTPListCtrlEx::FunItemIconIndex(BOOL bReOrder /*= FALSE*/, BOOL bCheckSort /*= FALSE*/)
{
	long lMaxIndex = -1;int iViewType = m_iViewType;
	int iCountPerLine = FunPerLineItemCount();
	if(bReOrder)
	{
		for(int l=0;l<m_aItem.GetSize();l++)
		{

			if(1 == bReOrder)
			{
				m_aItem[l]->lReportIndex = l; 
			}
			else if(2 == bReOrder && VIEWTYPE_ICON == iViewType && m_iArrType == ARRANGE_STORYBOARD) // Arrange to top left in stb
			{
				CRect rtItem = CRect(0,0,0,0);
				CSize szIcon = m_szItemIcon;
				GetClientRect(&m_rtClient);
				iCountPerLine = (m_rtClient.right-m_rtClient.left)/(szIcon.cx+m_szSpace.cx)-1;
				m_aItem[l]->lIconIndex = l/(iCountPerLine+1)*TP_LIST_COLNUMMAX +l%(iCountPerLine+1);
			}
		}	
	}
	else
	{
		TPListCtrlItemArray aEmptyItem;
		CMap <INT,INT,INT,INT> aMapIndex;
		for(INT l = 0; l < m_aItem.GetSize(); l++)
		{
			INT lIconIndex;
			if(VIEWTYPE_ICON == iViewType && m_iArrType == ARRANGE_STORYBOARD)  lIconIndex = m_aItem[l]->lIconIndex;
			else																lIconIndex = m_aItem[l]->lReportIndex;

			if(lIconIndex < 0)
			{
				aEmptyItem.Add(m_aItem[l]);
			}
			else
			{
				INT nIndexTemp = -1;
				if(lIconIndex > lMaxIndex) lMaxIndex = lIconIndex;
				// 检查重复Index , 如果重复下面重新分配
				if(aMapIndex.Lookup(lIconIndex, nIndexTemp))		aEmptyItem.Add(m_aItem[l]);
				else												aMapIndex.SetAt(lIconIndex, lIconIndex);
			}
		}
		aMapIndex.RemoveAll();

		FunSortExItem(aEmptyItem, 0);
		// 分配重复和无Index的Item
		lMaxIndex+=1;
		int lMaxReportIndex = lMaxIndex;
		CRect rtItem = CRect(0,0,0,0);
		CSize szIcon = m_szItemBuffer;
		
		szIcon = m_szItemIcon;
		GetClientRect(&m_rtClient);
		int iColNumNow = (m_rtClient.right-m_rtClient.left)/(szIcon.cx+m_szSpace.cx)-1;
		for(INT l=0;l<aEmptyItem.GetSize();l++)
		{
			if(VIEWTYPE_ICON == iViewType && m_iArrType == ARRANGE_STORYBOARD)
			{
				if (lMaxIndex%TP_LIST_COLNUMMAX>iColNumNow)
				{
					lMaxIndex = (lMaxIndex/TP_LIST_COLNUMMAX+1)*TP_LIST_COLNUMMAX;
				}
				aEmptyItem[l]->lIconIndex = lMaxIndex++;
			}
			else 
			{
				aEmptyItem[l]->lReportIndex = lMaxReportIndex++;
			}	
		}

		if(bCheckSort && aEmptyItem.GetSize() > 0) ClearSortFlag(0);
		FunSortItemByIndex(m_iViewType);
	}
}

void CTPListCtrlEx::FunSortItemByIndex(DWORD dwViewType)
{
	if (VIEWTYPE_ICON == dwViewType && m_iArrType == ARRANGE_STORYBOARD)
	{
		for (int i=0;i<m_aItem.GetSize();i++)
		{
			int iMinIconIndex =m_aItem[i]->lIconIndex;
			int iMinFlag      = -1;
			for (int j=i+1;j<m_aItem.GetSize();j++)
			{
				if (m_aItem.GetAt(j)->lIconIndex < iMinIconIndex)
				{
					iMinIconIndex = m_aItem.GetAt(j)->lIconIndex;
					iMinFlag  = j;
				}
			}
			if (iMinFlag >= 0)
			{
				TPListCtrlItem * listItemTemp  = NULL;
				listItemTemp = m_aItem.GetAt(iMinFlag);
				m_aItem.SetAt(iMinFlag,m_aItem.GetAt(i));
				m_aItem.SetAt(i,listItemTemp);
			}
		}
	}
	else
	{
		for (int i=0;i<m_aItem.GetSize();i++)
		{
			int iMinReportIndex =m_aItem[i]->lReportIndex;
			int iMinFlag      = -1;
			for (int j=i+1;j<m_aItem.GetSize();j++)
			{
				if (m_aItem.GetAt(j)->lReportIndex < iMinReportIndex)
				{
					iMinReportIndex = m_aItem.GetAt(j)->lReportIndex;
					iMinFlag  = j;
				}
			}
			if (iMinFlag >= 0)
			{
				TPListCtrlItem * listItemTemp  = NULL;
				listItemTemp = m_aItem.GetAt(iMinFlag);
				m_aItem.SetAt(iMinFlag,m_aItem.GetAt(i));
				m_aItem.SetAt(i,listItemTemp);
			}
		}
	}
}
void CTPListCtrlEx::SetSelectedColumn(int iCol)
{
}
void CTPListCtrlEx::SetItemTextColor(int iItem,TP_SYSTEM_COLOR clText)
{
}
void CTPListCtrlEx::SetItemView(int iItem,BOOL bShow)
{
	if(iItem<0 || iItem >= (int)m_aItem.GetSize()) return;
	m_aItem[iItem]->bShow = bShow;
}