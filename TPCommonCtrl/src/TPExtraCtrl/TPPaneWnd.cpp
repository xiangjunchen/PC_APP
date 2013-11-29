// TPPaneBar.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPPaneWnd.h"

typedef struct tag_TPPaneItem
{	
	//接点窗口信息	
	struct _tagWnd
	{
		CWnd         *pWnd; 
		CRect         rtWnd;
		BOOL          bAutoDel;
		BOOL          bAutoVScroll;
		BOOL          bAutoHScroll;
		int           iCXScroll;
		int           iCYScroll;
		CWnd         *pParent; 
		_tagWnd()
		{
			rtWnd        = CRect(0,0,0,0);			
			pWnd         = NULL; 
			bAutoDel     = FALSE;
			bAutoVScroll = FALSE;
			bAutoHScroll = FALSE;
			iCXScroll    = 0;
			iCYScroll    = 0;
			pParent      = NULL;
		}
	};
	CArray<_tagWnd *,_tagWnd *&>  aWnd;	
	_tagWnd                *pShowWnd; 	
	CTPCaseWnd             *pCaseWnd;
	TP_BORDER_STYLE         eBorderStyle;
	int                     iBorderWidth;
	BOOL                    bShow;  
	int                     iCurWnd; 

	//如果不是接点信息
	BOOL                                                bNode; 
	BOOL                                                bVert; 
	int                                                 iBarSize;
	int                                                 iCurBar;
	CRect                                               rtItem;	
	int                                                 iBarPos; 

	
	CArray<tag_TPPaneItem *,tag_TPPaneItem *&>  aTPPaneItem;
	tag_TPPaneItem                                  *pParentItem;
	tag_TPPaneItem()
	{	
		
		pShowWnd     = NULL; 		
		pCaseWnd     = NULL;
		eBorderStyle = TP_GetBorderStyle();
		iBorderWidth = 2;
		bShow        = TRUE; 

		bNode       = FALSE;
		bVert       = TRUE; 
		iBarSize    = 3;
		iCurBar     = -1;
		iBarPos     = -1;
		rtItem      = CRect(0,0,100,100);	
		pParentItem = NULL;
	}
	~tag_TPPaneItem()
	{
		for(INT l=0;l<aWnd.GetSize();l++)
		{
			if(aWnd[l]->bAutoDel)
			{
				aWnd[l]->pWnd ->DestroyWindow();
				delete aWnd[l]->pWnd;
			}
			else aWnd[l]->pWnd ->SetParent(aWnd[l]->pParent);
			delete aWnd[l];
		}
		aWnd.RemoveAll();

		if(pCaseWnd) { pCaseWnd ->DestroyWindow();delete pCaseWnd;	}


		for(INT l=0;l<aTPPaneItem.GetSize();l++)
			delete 	aTPPaneItem[l];
		aTPPaneItem.RemoveAll();
		
	}
}TPPaneItem;
// CTPPaneWnd

IMPLEMENT_DYNAMIC(CTPPaneWnd, CWnd)
CTPPaneWnd::CTPPaneWnd()
{	
	m_dwWindowLevel = TPWNDLEVEL1; 

	m_sizeMin      = CSize(4,4);
	m_pParentWnd   = NULL;
	m_hRoot        = new TPPaneItem();
	m_rtRoot       = CRect(0,0,0,0);
	m_pDCTemp      = NULL; 
	m_iEdgeSize    = 0;

	m_hDragItem    = NULL;
	m_ptSave       = CPoint(0,0);
	m_eDragMode    = TP_DRAG_UNKNOW;
	m_rtInvert     = CRect(0,0,0,0);

	m_bSubWindow   = FALSE;
	m_bCreate      = FALSE;	
	m_eBorderStyle = TP_GetBorderStyle();

	m_pResCursor[0] = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\Cursorwe.cur"));
	m_pResCursor[1] = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\Cursorns.cur"));

}

CTPPaneWnd::~CTPPaneWnd()
{
	if(m_hRoot)
	{	
		delete (TPPaneItem *)m_hRoot;
		m_hRoot = NULL;
	}
	TP_ReleaseCommonRes(m_pResCursor[0]);
	TP_ReleaseCommonRes(m_pResCursor[1]);
}


BEGIN_MESSAGE_MAP(CTPPaneWnd, CWnd)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void     CTPPaneWnd::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_STATIC,&wndClass);		
		wndClass.lpszClassName = WC_PANEWND;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_PANEWND,AfxGetInstanceHandle());
}



void CTPPaneWnd::SetMinSize(SIZE sizeMin)
{
	m_sizeMin = sizeMin;
}
BOOL CTPPaneWnd::Create(CWnd* pParentWnd,DWORD dwStyle,UINT nID)
{
	if(m_pParentWnd == NULL && pParentWnd) m_pParentWnd = pParentWnd;
	return CWnd::Create(WC_PANEWND,NULL,dwStyle,CRect(0,0,0,0),pParentWnd,nID);
}

void CTPPaneWnd::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		if(m_pParentWnd == NULL) m_pParentWnd = GetParent();
		CRect rtClient;
		GetClientRect(&rtClient);
		OnSize(0,rtClient.Width(),rtClient.Height());
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	

	}
}
void CTPPaneWnd::OnDestroy()
{
	delete (TPPaneItem *)m_hRoot;
	m_hRoot = NULL;
	SubWindow(FALSE);
	CWnd::OnDestroy();
}
void  CTPPaneWnd::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}
BOOL CTPPaneWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	m_bCreate = TRUE;
	return CWnd::PreCreateWindow(cs);
}


BOOL CTPPaneWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	BOOL bBar;HPANEITEM hItem;
	if(HitTest(ptCursor,hItem,bBar,NULL) && bBar)
	{
		TPPaneItem *pItem = (TPPaneItem *)hItem;
		if(pItem ->pParentItem ->bVert) SetCursor(m_pResCursor[0] ->GetCursor());
		else                            SetCursor(m_pResCursor[1] ->GetCursor());
		return TRUE;
	}
	else return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
int CTPPaneWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}

LRESULT CTPPaneWnd::OnNcHitTest(CPoint point)
{
	return HTCLIENT;
}

void CTPPaneWnd::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}
void CTPPaneWnd::PaintItem(HPANEITEM hItem,CDC *pDC) 
{
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(hItem == NULL) return;
	if(pItem ->bNode)
	{
		for(INT l=0;l<pItem ->aTPPaneItem.GetSize();l++)
		{
			if(pItem ->aTPPaneItem[l]->bShow)
			{
			//	pDC ->FillSolidRect(pItem->aTPPaneItem[l] ->rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
				PaintItem(pItem ->aTPPaneItem[l],pDC);				
				if(pItem ->aTPPaneItem[l] -> iBarPos>=0)
				{
					CRect rtItem = pItem ->rtItem;
					if(pItem ->bVert)
					{
						rtItem.left  = pItem ->aTPPaneItem[l]->iBarPos;
						rtItem.right = rtItem.left + pItem ->iBarSize;
					}
					else
					{
						rtItem.top    = pItem ->aTPPaneItem[l]->iBarPos;
						rtItem.bottom = rtItem.top + pItem ->iBarSize;
					}
					pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_FRAME));
				}
			}

		}
	}
	else
	{
		CRect rtItem = pItem ->rtItem;	
		g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
		g_pRoundCorner ->PaintRect(pItem ->eBorderStyle,pDC,rtItem,pItem->iBorderWidth);			
		rtItem.DeflateRect(pItem->iBorderWidth,pItem->iBorderWidth);
		pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));	
		if(pItem ->aWnd.GetSize()>0)	m_pDCTemp ->ExcludeClipRect(rtItem);
	}
}
void CTPPaneWnd::OnPaint()
{
	CPaintDC dc(this);
	CRect rtClient;
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());
	m_pDCTemp = &dc;
	PaintItem(m_hRoot,&cdcMemo);
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
}

void CTPPaneWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);	
	SetRect(m_hRoot,CRect(m_iEdgeSize,m_iEdgeSize,cx-m_iEdgeSize,cy-m_iEdgeSize));
}
int CTPPaneWnd::GetMinSize(HPANEITEM  hItem,BOOL bVert)
{
	if(hItem == NULL) return 0;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(pItem ->bNode == FALSE)  return bVert ? m_sizeMin.cx : m_sizeMin.cy;	
	else 
	{
		int iSize =0,iMinSize = 0;
		for(int l=0;l<pItem ->aTPPaneItem.GetSize();l++)
		{
			if(pItem ->aTPPaneItem[l]->bShow)
			{
				iMinSize = GetMinSize(pItem ->aTPPaneItem[l],bVert);
				if(pItem ->bVert == bVert)	iSize += iMinSize;
				else                        iSize  = max(iMinSize,iSize);
				if(pItem ->aTPPaneItem[l] ->bVert == bVert && pItem ->aTPPaneItem[l] ->iBarPos >=0)
					iSize +=  pItem ->iBarSize;
			}
		}
		return iSize;
	}
}
int CTPPaneWnd::GetItemIndex(HPANEITEM hParentItem,HPANEITEM hItem)
{
	TPPaneItem *pItem       = (TPPaneItem *)hParentItem;
	if(pItem == NULL) return -1;	
	for(int l=0;l<pItem ->aTPPaneItem.GetSize();l++)
	{
		if(pItem ->aTPPaneItem[l] == hItem) return l;		
	}
	return -1;
}
HPANEITEM  CTPPaneWnd::GetSiblingItem(HPANEITEM hItem,int iNext)
{
	if(hItem == NULL) return 0;
	TPPaneItem *pItem       = (TPPaneItem *)hItem;
	TPPaneItem *pParentItem = pItem ->pParentItem;
	int l=0;
	for(l=0;l<pParentItem ->aTPPaneItem.GetSize();l++)
	{
		if(pParentItem ->aTPPaneItem[l] == hItem) break;		
	}
	if(l>=pParentItem ->aTPPaneItem.GetSize()) return NULL;
	if(iNext>0)
	{
		for(++l;l<pParentItem ->aTPPaneItem.GetSize();l++)
		{
			if(pParentItem ->aTPPaneItem[l] ->bShow) iNext --;
			if(iNext <=0) break;
		}
	}
	else
	{
		for(--l;l>=0;l--)
		{
			if(pParentItem ->aTPPaneItem[l] ->bShow) iNext ++;
			if(iNext >=0) break;
		}
	}
	if(l>=pParentItem ->aTPPaneItem.GetSize()) return NULL;
	else return pParentItem ->aTPPaneItem[l];

}

void CTPPaneWnd::SetItemSize(HPANEITEM  hItem,int iItem,int iNewSize,BOOL bTopLeft)
{
	if(hItem == NULL) return;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(pItem ->bNode == FALSE) return;
	if(iItem <0 || iItem >= pItem->aTPPaneItem.GetSize()) return;
	if(!pItem->aTPPaneItem[iItem]->bShow) return;		
	int iBar =0,iTempBar = 0;int iSize = 0,iBegin =0,iEnd = 0,iBeginPos,iMinSize = 0;
	if(bTopLeft == FALSE) //往右边扩展
	{
		iBegin = iItem +  1;
		iEnd = (int)pItem->aTPPaneItem.GetSize();
	}
	else
	{
		iBegin = 0;
		iEnd = iItem;
	}
	for(int l=iBegin;l<iEnd;l++)
	{
		if(pItem->aTPPaneItem[l]->bShow)
		{
			if(pItem ->bVert)
			{
				iMinSize += GetMinSize(pItem ->aTPPaneItem[l],TRUE);
				iSize += pItem->aTPPaneItem[l]->rtItem.Width();
				if(iBar ==0) iBeginPos = pItem->aTPPaneItem[l]->rtItem.left;
			}
			else 
			{
				iMinSize += GetMinSize(pItem ->aTPPaneItem[l],FALSE);
				iSize += pItem->aTPPaneItem[l]->rtItem.Height();
				if(iBar ==0) iBeginPos = pItem->aTPPaneItem[l]->rtItem.top;
			}
			iBar ++;
		}
	}	
	if(iBar<=0) { return;}  //如果扩展没有边界	
	if(pItem ->bVert)
	{							
		CRect rtTemp1 = pItem ->rtItem;
		if(iSize - (iNewSize - pItem->aTPPaneItem[iItem] ->rtItem.Width()) < iMinSize) 
			iNewSize = iSize - iMinSize + pItem->aTPPaneItem[iItem] ->rtItem.Width();
		int   iCx = iBeginPos + (bTopLeft ? 0 : iNewSize - pItem->aTPPaneItem[iItem] ->rtItem.Width()),iMinSize;
		double dWidth = (iSize - (iNewSize - pItem->aTPPaneItem[iItem] ->rtItem.Width() )) *1.0 / iSize;
		dWidth = max(0,dWidth);
		iTempBar = 0;				
		for(int l=iBegin;l<iEnd;l++)
		{
			if(pItem->aTPPaneItem[l]->bShow)
			{					
				iMinSize = GetMinSize(pItem->aTPPaneItem[l],TRUE);
				rtTemp1.left  = iCx;						
				rtTemp1.right = rtTemp1.left + max(TP_DoubleToInt(pItem->aTPPaneItem[l]->rtItem.Width() * dWidth),iMinSize);						
				if(iTempBar == iBar-1) rtTemp1.right = max(rtTemp1.left +iMinSize,pItem->rtItem.right);
				SetRect(pItem->aTPPaneItem[l],rtTemp1);
				if(iTempBar>0 || !bTopLeft)  pItem->aTPPaneItem[l] ->iBarPos = rtTemp1.left -pItem->iBarSize;
				else pItem->aTPPaneItem[l] ->iBarPos = -1;
				iCx += rtTemp1.Width() + pItem ->iBarSize;
				iTempBar ++;
			}
		}
		pItem->aTPPaneItem[iItem] ->rtItem.right = pItem->aTPPaneItem[iItem] ->rtItem.left + iNewSize;
		SetRect(pItem->aTPPaneItem[iItem],pItem->aTPPaneItem[iItem] ->rtItem);
	}
	else
	{
		CRect rtTemp1 = pItem ->rtItem;
		if(iSize - (iNewSize - pItem->aTPPaneItem[iItem] ->rtItem.Height()) < iMinSize) 
			iNewSize = iSize - iMinSize + pItem->aTPPaneItem[iItem] ->rtItem.Height();
		int   iCy = iBeginPos + (bTopLeft ? 0 : iNewSize - pItem->aTPPaneItem[iItem] ->rtItem.Height()),iMinSize;
		double dHeight = (iSize - (iNewSize -  pItem->aTPPaneItem[iItem] ->rtItem.Height() )) *1.0 / iSize;
		dHeight = max(0,dHeight);
		iTempBar = 0;				
		for(int l=iBegin;l<iEnd;l++)
		{
			if(pItem->aTPPaneItem[l]->bShow)
			{					
				iMinSize = GetMinSize(pItem->aTPPaneItem[l],FALSE);
				rtTemp1.top    = iCy;						
				rtTemp1.bottom = rtTemp1.top + max(TP_DoubleToInt(pItem->aTPPaneItem[l]->rtItem.Height() * dHeight),iMinSize);						
				if(iTempBar == iBar-1) rtTemp1.bottom = max(rtTemp1.top +iMinSize,pItem ->rtItem.bottom);
				SetRect(pItem->aTPPaneItem[l],rtTemp1);
				if(iTempBar>0 || !bTopLeft)  pItem->aTPPaneItem[l] ->iBarPos = rtTemp1.top -pItem->iBarSize;
				else pItem->aTPPaneItem[l] ->iBarPos = -1;
				iCy += rtTemp1.Height() + pItem ->iBarSize;
				iTempBar ++;
			}
		}
		pItem->aTPPaneItem[iItem] ->rtItem.bottom = pItem->aTPPaneItem[iItem] ->rtItem.top + iNewSize;
		SetRect(pItem->aTPPaneItem[iItem],pItem->aTPPaneItem[iItem] ->rtItem);
	}

}
void CTPPaneWnd::SetRect(HPANEITEM  hItem,CRect rtItem)
{
	if(hItem == NULL) return;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(pItem ->bNode == TRUE)
	{		
		CRect rtTemp = rtItem;	
		int iBar =0,iTempBar = 0;int iSize = 0;
		for(int l=0;l<pItem->aTPPaneItem.GetSize();l++)
		{
			if(pItem->aTPPaneItem[l]->bShow)
			{
				if(pItem ->bVert) iSize += pItem->aTPPaneItem[l]->rtItem.Width();
				else iSize += pItem->aTPPaneItem[l]->rtItem.Height();
				iBar ++;
			}
		}
		if(iBar<=0) { 		pItem->rtItem = rtItem;		return;}
		if(pItem ->bVert)
		{							
			CRect rtTemp1 = rtTemp;
			int   iCx = rtTemp1.left,iMinSize;
			double dWidth = (rtTemp.Width() - (iBar -1) *pItem ->iBarSize) *1.0 / iSize;
			dWidth = max(0,dWidth);
			iTempBar = 0;				
			for(int l=0;l<pItem->aTPPaneItem.GetSize();l++)
			{
				if(pItem->aTPPaneItem[l]->bShow)
				{					
					iMinSize = GetMinSize(pItem->aTPPaneItem[l],TRUE);
					rtTemp1.left  = iCx;						
					rtTemp1.right = rtTemp1.left + max(TP_DoubleToInt(pItem->aTPPaneItem[l]->rtItem.Width() * dWidth),iMinSize);						
					if(iTempBar == iBar-1) rtTemp1.right = max(rtTemp1.left +iMinSize,rtTemp.right);
					SetRect(pItem->aTPPaneItem[l],rtTemp1);
					if(iTempBar>0)  pItem->aTPPaneItem[l] ->iBarPos = rtTemp1.left -pItem->iBarSize;
					else pItem->aTPPaneItem[l] ->iBarPos = -1;
					iCx += rtTemp1.Width() + pItem ->iBarSize;
					iTempBar ++;
				}
			}
			pItem ->rtItem = rtItem;
		}
		else
		{
			CRect rtTemp1 = rtTemp;
			int   iCy = rtTemp1.top,iMinSize;
			double dHeight = (rtTemp.Height() - (iBar -1) *pItem ->iBarSize) *1.0 / iSize;
			dHeight = max(0,dHeight);
			iTempBar = 0;				
			for(int l=0;l<pItem->aTPPaneItem.GetSize();l++)
			{
				if(pItem->aTPPaneItem[l]->bShow)
				{	
					iMinSize = GetMinSize(pItem->aTPPaneItem[l],FALSE);
					rtTemp1.top = iCy;						
					rtTemp1.bottom = rtTemp1.top + max(TP_DoubleToInt(pItem->aTPPaneItem[l]->rtItem.Height() * dHeight),iMinSize);						
					if(iTempBar == iBar-1) rtTemp1.bottom = max(rtTemp1.top + iMinSize,rtTemp.bottom);
					SetRect(pItem->aTPPaneItem[l],rtTemp1);
					if(iTempBar>0)  pItem->aTPPaneItem[l] ->iBarPos = rtTemp1.top -pItem->iBarSize;
					else pItem->aTPPaneItem[l] ->iBarPos = -1;
					iCy += rtTemp1.Height() + pItem ->iBarSize;
					iTempBar ++;
				}
			}
			pItem ->rtItem = rtItem;
		}
	}
	else
	{
		pItem ->rtItem = rtItem;
		if(pItem ->iCurWnd>=0 && pItem ->iCurWnd < pItem->aWnd.GetSize())
			ShowItemWnd(pItem->aWnd[pItem ->iCurWnd]->pWnd,pItem);
	}

}
void CTPPaneWnd::ShowItem(HPANEITEM hItem,int iItem,BOOL bShow)
{
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(iItem<0 || iItem >= pItem ->aTPPaneItem.GetSize()) return;
	pItem ->aTPPaneItem[iItem]->bShow = bShow;
	SetRect(hItem,pItem ->rtItem);
	Invalidate();
}
void CTPPaneWnd::SetItemBarSize(HPANEITEM hItem,int iItem,int iBarSize)
{
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem   = (TPPaneItem *)hItem;
	if(iItem<0 || iItem >= pItem ->aTPPaneItem.GetSize()) return;
	pItem ->aTPPaneItem[iItem]->iBarSize = iBarSize;
	SetRect(hItem,pItem ->rtItem);
	Invalidate();
}
void CTPPaneWnd::SetItemRate(HPANEITEM hItem,int iItem,double dRate)
{
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(pItem ->bVert)	SetItemSize(hItem,iItem,TP_DoubleToInt(pItem ->rtItem.Width() *dRate),FALSE);
	else                SetItemSize(hItem,iItem,TP_DoubleToInt(pItem ->rtItem.Height() *dRate),FALSE);
}
void CTPPaneWnd::SetItemSize(HPANEITEM hItem,int iItem,int iSize)
{
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(iItem<0 || iItem >= pItem ->aTPPaneItem.GetSize()) return;
	SetItemSize(hItem,iItem,iSize,FALSE);
}
int CTPPaneWnd::GetItemCount(HPANEITEM hItem)
{	
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	return  (int)pItem ->aTPPaneItem.GetSize();	
}
HPANEITEM  CTPPaneWnd::GetItem(HPANEITEM hItem,int iPos)
{
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(iPos<0 || iPos >= pItem ->aTPPaneItem.GetSize()) return NULL;
	return pItem ->aTPPaneItem[iPos];
}
void CTPPaneWnd::InsertItem(HPANEITEM hItem,int iInsertPos,int iSubItemCount,BOOL bVert)
{
	if(iSubItemCount <=0) return;
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(pItem ->aTPPaneItem.GetSize() >0 && pItem ->bVert != bVert) return;
	pItem ->bNode = TRUE;
	pItem ->bVert = bVert;
	if(iInsertPos <0) iInsertPos = (int)pItem ->aTPPaneItem.GetSize();
	for(INT l=0;l<iSubItemCount;l++)
	{
		TPPaneItem *pItemAdd = new TPPaneItem();
		pItemAdd ->bShow       = TRUE;
		pItemAdd ->pParentItem = pItem;
		pItem ->aTPPaneItem.InsertAt(iInsertPos,pItemAdd);
	}
	SetRect(hItem,pItem ->rtItem);
	Invalidate(FALSE);
}
void CTPPaneWnd::DeleteItem(HPANEITEM hItem,int iPos)
{
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(iPos<0 || iPos >= pItem ->aTPPaneItem.GetSize()) return ;
	delete pItem ->aTPPaneItem[iPos];
	pItem ->aTPPaneItem.RemoveAt(iPos);
	SetRect(hItem,pItem ->rtItem);
	Invalidate(FALSE);
}
void CTPPaneWnd::InsertItemWnd(HPANEITEM hItem,CWnd *pWnd,BOOL bAutoVScroll,BOOL bAutoHScroll ,BOOL bAudoDel,int iInsertPos)
{
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	pItem ->bNode = FALSE;
	if(iInsertPos <0) iInsertPos = (int)pItem ->aWnd.GetSize();
	TPPaneItem::_tagWnd *pWndItem = new TPPaneItem::_tagWnd;
	pWndItem ->pWnd         = pWnd; pWnd ->GetWindowRect(&pWndItem ->rtWnd);	        
	pWndItem ->bAutoVScroll = bAutoVScroll;
	pWndItem ->bAutoHScroll = bAutoHScroll;
	pWndItem ->bAutoDel     = bAudoDel;
	pWndItem ->pParent      = pWnd->GetParent();
	if(bAutoVScroll || bAutoHScroll)
	{
		if(pItem ->pCaseWnd == NULL)
		{
			pItem ->pCaseWnd = new CTPCaseWnd();
			pItem ->pCaseWnd -> Create(NULL ,NULL,WS_CHILD|WS_VSCROLL|WS_HSCROLL|WS_CLIPCHILDREN,CRect(0,0,0,0),this,0);			
			pItem ->pCaseWnd ->ShowWindow(SW_HIDE);			
		}
	}
	else
	{		
		pWnd ->SetParent(this);
		pWnd ->ShowWindow(SW_HIDE);
	}		
	pItem ->aWnd.InsertAt(iInsertPos,pWndItem);	
	if(pItem ->iCurWnd <0) ShowItemWnd(pWnd,pItem);
}
void CTPPaneWnd::DeleteItemWnd(CWnd *pWnd,HPANEITEM hItem)
{
	if(hItem == NULL) hItem = GetWndItem(pWnd);
	if(hItem == NULL) return;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(pItem ->bNode != FALSE) return;
	for(INT l = (int)pItem->aWnd.GetSize() -1;l>=0;l--)
	{
		if(pItem ->aWnd[l]->pWnd == pWnd)
		{
			if(pItem ->aWnd[l]->bAutoDel)
			{
				pItem ->aWnd[l]->pWnd ->DestroyWindow();
				delete pItem ->aWnd[l]->pWnd;
			}
			delete pItem ->aWnd[l];			
			pItem ->aWnd.RemoveAt(l);
			if(pItem ->iCurWnd == l && pItem ->aWnd.GetSize()>0)
			{
				int iItem = min(pItem ->iCurWnd,(int)pItem ->aWnd.GetSize() -1);
				ShowItemWnd(pItem ->aWnd[pItem ->iCurWnd]->pWnd,pItem);
			}
		}
	}
}
void  CTPPaneWnd::ShowItemWnd(CWnd *pWnd,HPANEITEM hItem)
{
	if(hItem == NULL) hItem = GetWndItem(pWnd);
	if(hItem == NULL) return ;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(pItem ->bNode != FALSE) return;
	INT l =  0;
	for(l = (int)pItem->aWnd.GetSize() -1;l>=0;l--)
	{
		if(pItem ->aWnd[l]->pWnd == pWnd) break;		
	}
	if(pItem->iCurWnd >=0 && pItem->iCurWnd <pItem ->aWnd.GetSize())
	{
		if(pItem ->aWnd[pItem->iCurWnd]->bAutoVScroll || pItem ->aWnd[pItem->iCurWnd]->bAutoHScroll)
		{
			TPPaneItem::_tagWnd *pWndItem = pItem ->aWnd[pItem->iCurWnd];		
			pWndItem ->iCXScroll          = pItem ->pCaseWnd ->GetScrollPos(SB_HORZ);
			pWndItem ->iCYScroll          = pItem ->pCaseWnd ->GetScrollPos(SB_VERT);
		}
		 if( l != pItem->iCurWnd)	pItem ->aWnd[pItem->iCurWnd]->pWnd ->ShowWindow(SW_HIDE);	
	}
	if(l>=0)
	{
		CRect rtTemp	= pItem ->rtItem;
		rtTemp.DeflateRect(pItem ->iBorderWidth,pItem->iBorderWidth);
		TPPaneItem::_tagWnd *pWndItem = pItem ->aWnd[l];		
		if(pWndItem ->bAutoVScroll || pWndItem ->bAutoHScroll)
		{
			pItem ->pCaseWnd  ->MoveWindow(rtTemp);
			pWndItem ->pWnd ->SetParent(pItem ->pCaseWnd);
			pItem ->pCaseWnd ->SetSubWnd(pWndItem ->pWnd,pWndItem ->bAutoVScroll,pWndItem ->bAutoHScroll);
			if(pWndItem ->bAutoHScroll) pItem ->pCaseWnd ->SetScrollPos(SB_HORZ,pWndItem ->iCXScroll);
			if(pWndItem ->bAutoVScroll) pItem ->pCaseWnd ->SetScrollPos(SB_VERT,pWndItem ->iCYScroll);
			pItem ->pCaseWnd ->ShowWindow(SW_SHOW);
		}
		else 
		{
			if(pItem->pCaseWnd) pItem->pCaseWnd ->ShowWindow(SW_HIDE);
			pItem ->aWnd[l]->pWnd ->MoveWindow(rtTemp);
			pItem ->aWnd[l]->pWnd ->ShowWindow(SW_SHOW);
		}
	}
	pItem->iCurWnd = l;	//注意滚动条的位置
}
HPANEITEM   CTPPaneWnd::GetWndItem(CWnd *pWnd,HPANEITEM hItem)
{
	if(hItem == NULL) hItem = m_hRoot;
	HPANEITEM    hFind = NULL;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(pItem ->bNode)
	{
		for(INT l=0;l<pItem ->aTPPaneItem.GetSize();l++)
		{
			hFind = GetWndItem(pWnd,pItem ->aTPPaneItem[l]);
			if(hFind) return hFind;
		}
	}
	else
	{
		for(INT l = (int)pItem->aWnd.GetSize() -1;l>=0;l--)
		{
			if(pItem ->aWnd[l]->pWnd == pWnd)
			{
				return hItem;
			}
		}
	}
	return NULL;
}

CWnd *CTPPaneWnd::GetItemWnd(HPANEITEM hItem,int iPos)
{
	if(hItem == NULL) hItem = m_hRoot;
	TPPaneItem *pItem = (TPPaneItem *)hItem;
	if(pItem ->bNode != FALSE) return NULL;
	if(iPos<0 || iPos >= pItem ->aWnd.GetSize()) return NULL;
	return pItem ->aWnd[iPos] ->pWnd;
}
BOOL CTPPaneWnd::HitTest(CPoint point, HPANEITEM &hItem,BOOL &bBar,HPANEITEM hTestItem)
{
	hItem = NULL;
	bBar  = FALSE;
	TPPaneItem *pItem = (TPPaneItem *)hTestItem;
	if(pItem == NULL) pItem = (TPPaneItem *)m_hRoot;
	if(pItem->rtItem.PtInRect(point))
	{
		for(INT l=0;l<pItem ->aTPPaneItem.GetSize();l++)
		{
			if(pItem ->aTPPaneItem[l]->bShow)
			{
				if(pItem ->aTPPaneItem[l]->iBarPos>=0)
				{
					if(pItem ->bVert) 
					{
						if(pItem ->aTPPaneItem[l]->iBarPos -1 <= point.x && pItem ->aTPPaneItem[l]->iBarPos + pItem ->iBarSize +1 >= point.x)
						{
							hItem = pItem ->aTPPaneItem[l];
							bBar  = TRUE;
							return TRUE;
						}
					}
					else
					{
						if(pItem ->aTPPaneItem[l]->iBarPos -1 <= point.y && pItem ->aTPPaneItem[l]->iBarPos + pItem ->iBarSize +1 >= point.y)
						{
							hItem = pItem ->aTPPaneItem[l];
							bBar  = TRUE;
							return TRUE;
						}
					}
				}
				if(pItem ->aTPPaneItem[l]->rtItem.PtInRect(point))
				{
					BOOL bReturn = HitTest(point,hItem,bBar,pItem ->aTPPaneItem[l]);
					if(bReturn) return TRUE;
				}
			}
		}
		return FALSE;
	}
	else return FALSE;
}


void CTPPaneWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CWnd::OnLButtonDown(nFlags, point);
	BOOL bBar;HPANEITEM hItem;
	if(HitTest(point,hItem,bBar,NULL) && bBar)
	{
		m_hDragItem = hItem;
		m_ptSave = point;
		SetCapture();
		m_eDragMode = TP_DRAG_MIDDLE;
		GetClientRect(&m_rtClipRect);		
		TPPaneItem *pParentItem = ((TPPaneItem *)hItem) ->pParentItem;
		TPPaneItem *pItem       = (TPPaneItem *)GetSiblingItem((TPPaneItem *)hItem,-1);
		if(pItem)
		{
			if(pParentItem ->bVert) m_rtClipRect.left = pItem ->rtItem.left;
			else                    m_rtClipRect.top  = pItem ->rtItem.top;
		}
	}
}

void CTPPaneWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_eDragMode == TP_DRAG_MIDDLE || m_eDragMode == TP_DRAG_MIDDLEOK)
	{
		ReleaseCapture();
		if(m_eDragMode == TP_DRAG_MIDDLEOK)
		{
			TPPaneItem *pParentItem = ((TPPaneItem *)m_hDragItem) ->pParentItem;
			TPPaneItem *pItem       = (TPPaneItem *)GetSiblingItem((TPPaneItem *)m_hDragItem,-1);
			if(pItem)
			{
				int iSize = 0;
				int iMin  = GetMinSize(pItem,pParentItem->bVert);
				if(pParentItem->bVert)
					iSize = point.x - pItem ->rtItem.left;
				else
					iSize = point.y - pItem ->rtItem.top;
				iSize = max(iSize,iMin);
				int iIndex =GetItemIndex(pParentItem,pItem);
				if(iIndex>=0) SetItemSize(pParentItem,iIndex,iSize,FALSE);
				Invalidate();
			}
		}
		m_eDragMode = TP_DRAG_UNKNOW;
		m_rtInvert  = CRect(0,0,0,0); 
	}
	//CWnd::OnLButtonUp(nFlags, point);
}

void CTPPaneWnd::OnMouseMove(UINT nFlags, CPoint point)
{
//	CWnd::OnMouseMove(nFlags, point);
	if(m_eDragMode == TP_DRAG_MIDDLE || m_eDragMode == TP_DRAG_MIDDLEOK)
	{
		TP_ClipPoint(m_rtClipRect,point);
		if(m_ptSave!=point)
		{
			CRect rtClient;
			GetClientRect(&rtClient);
			m_eDragMode = TP_DRAG_MIDDLEOK;
			CDC *pDC = GetDC();
			if(m_rtInvert.Width()>0) pDC->InvertRect(m_rtInvert);
			TPPaneItem *pParentItem = ((TPPaneItem *)m_hDragItem) ->pParentItem;
			m_rtInvert = pParentItem ->rtItem;
			if(pParentItem ->bVert)       { m_rtInvert.left = point.x - 1;m_rtInvert.right  = point.x + 1;}
			else if(!pParentItem ->bVert) { m_rtInvert.top  = point.y - 1;m_rtInvert.bottom = point.y + 1;}
			pDC->InvertRect(m_rtInvert);
			ReleaseDC(pDC);
		}
	}
}
