// TPTabCtrlExEx.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPTabCtrlEx.h"
#include ".\tptabctrlex.h"


// CTPTabCtrlEx
#define TABCTRL_REPAINT    0x0000001
#define TABCTRL_SINGLELINE 0x0000001

#define TABCTRL_LOCK       0x0000002

#define TAB_EDGE      5
#define TAB_SPACE     0
#define TAB_EXTWIDTH  40

enum TABCTRLEX_INDEX
{
	TABCTRLEX_DIS,
	TABCTRLEX_NOR,
	TABCTRLEX_DOWN,
	TABCTRLEX_LINE,
	TABCTRLEX_MAX,
}; 

IMPLEMENT_DYNAMIC(CTPTabCtrlEx, CTabCtrl)
CTPTabCtrlEx::CTPTabCtrlEx()
{
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uStyle        = 0;
	m_eBorderStyle  = TP_GetBorderStyle();
	m_dwWindowLevel = TPWNDLEVEL1;
	m_uPrivateState = TABCTRL_SINGLELINE;
	m_iBackColor    = COLOR_BASE_FRAME;
	m_eTabDir       = enTabCtrlTop;
	m_iLockSize     = 0;
	m_iTopSize      = 0;
	m_iTabSize      = 20;
	m_iBarSize      = 13;
	m_rtTab         = CRect(0,0,0,0);        
	m_rtWnd         = CRect(0,0,0,0); 
	m_rtClient      = CRect(0,0,0,0); 

	m_szImageList   = CSize(0,0);
	m_szSelItem     = CSize(0,0);

	m_pDownIcon     = NULL;
	m_pSelItem      = NULL;
	m_iDownItem     = -1;


	for(INT l=0;l<TABCTRLEX_MAX;l++) m_pTabBmp[l] = NULL;
	m_pCloseBox = NULL;

	m_pShowTips  = NULL;
 
}

CTPTabCtrlEx::~CTPTabCtrlEx()
{
	SubWindow(FALSE);
	if(m_hImageList.m_hImageList) 
		m_hImageList.DeleteImageList();
	for(INT l=0;l<m_aItem.GetSize();l++)
		delete 	m_aItem[l];
	m_aItem.RemoveAll();

	m_pSliderBar = NULL;

	for(INT l=0;l<TABCTRLEX_MAX;l++)
		TP_ReleaseCommonRes(m_pTabBmp[l]);

	TP_ReleaseCommonRes(m_pCloseBox);	
}


BEGIN_MESSAGE_MAP(CTPTabCtrlEx, CTabCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()

	ON_MESSAGE(TCM_INSERTITEM,     OnMsgInsertItem)
	ON_MESSAGE(TCM_DELETEITEM,     OnMsgDeleteItem)
	ON_MESSAGE(TCM_DELETEALLITEMS, OnMsgDeleteAllItems)

	ON_MESSAGE(TCM_SETIMAGELIST,   OnMsgSetImageList)
	ON_MESSAGE(TCM_GETIMAGELIST,   OnMsgGetImageList)
	ON_MESSAGE(TCM_GETITEMCOUNT,   OnMsgGetItemCount)
	ON_MESSAGE(TCM_SETCURFOCUS,    OnMsgSetCurFocus)
	ON_MESSAGE(TCM_GETCURFOCUS,    OnMsgGetCurFocus)
	ON_MESSAGE(TCM_GETCURSEL,      OnMsgGetCurSel)
	ON_MESSAGE(TCM_SETCURSEL,      OnMsgSetCurSel)
	ON_MESSAGE(TCM_GETROWCOUNT,    OnMsgGetRowCount)

	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()

	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff,       OnToolTipNotify )

END_MESSAGE_MAP()

void     CTPTabCtrlEx::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_TABCONTROL,&wndClass);		
		wndClass.lpszClassName = WC_TABCTRLEX;
		wndClass.style  	  |= CS_HREDRAW|CS_VREDRAW;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_TABCTRLEX,AfxGetInstanceHandle());
}
void CTPTabCtrlEx::LoadSkin(CString sPath)
{
	CString sText = _L("Top");
	if(m_eTabDir == enTabCtrlTop) sText = _L("Top");
	
	for(INT l=0;l<TABCTRLEX_MAX;l++) TP_ReleaseCommonRes(m_pTabBmp[l]);

	m_pTabBmp[TABCTRLEX_DIS]  = TP_LoadBmpByte(sPath + sText + _L("_Dis.Bmp"));
	m_pTabBmp[TABCTRLEX_NOR]  = TP_LoadBmpByte(sPath + sText + _L("_Nor.Bmp"));
	m_pTabBmp[TABCTRLEX_DOWN] = TP_LoadBmpByte(sPath + sText + _L("_Down.Bmp"));
	m_pTabBmp[TABCTRLEX_LINE] = TP_LoadBmpByte(sPath + sText + _L("_Line.Bmp"));

	m_pCloseBox  = TP_LoadImage(TP_GetSkinPath() + _L("\\CommonCtrl\\TabCtrl\\53\\Tab_closebox.bmp"),11,RGB(0,0,0));

	m_iTabSize  = m_pTabBmp[TABCTRLEX_NOR] ->Height() ;


}
void CTPTabCtrlEx::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		ModifyStyle(0,WS_CLIPCHILDREN|TCS_TOOLTIPS,0);

		if(m_dwWindowLevel == TPWNDLEVEL1)
		{
			LoadSkin( TP_GetSkinPath() +  _L("\\CommonCtrl\\TabCtrl\\53\\Tab_"));
			m_iBackColor   = COLOR_BASE_FRAME;
		}
		else
		{
			LoadSkin( TP_GetSkinPath() +  _L("\\CommonCtrl\\TabCtrl\\77\\Tab_"));
			m_iBackColor   = COLOR_BASE_FRAME2;
		}
		CRect rtClient;
		GetClientRect(&rtClient);
		m_pSliderBar = new CTPSliderBar();
		m_pSliderBar ->Create(WS_CHILD|WS_VISIBLE|TBS_NOTICKS|TBS_BOTH,CRect(0,0,rtClient.Width(),m_iBarSize),this,0);
		m_pSliderBar ->SetBackRes(TP_GetSkinPath() + _L("\\CommonCtrl\\TabCtrl\\53\\Tab_Bar.bmp"));
		m_pSliderBar ->m_iChanneOffset = -1;
		m_pSliderBar ->m_pMsgWnd       = this; 
		m_pSliderBar ->m_bFoceHideBar  = TRUE;

		SaveFont(m_hWnd);

		SetFont(TP_GetSysFont(m_iFontIndex));


		m_pShowTips = new CToolTipCtrl();
		ASSERT(m_pShowTips->Create(this,TTS_ALWAYSTIP)); 
		m_pShowTips->EnableToolTips(TRUE); 
		m_pShowTips->Activate(TRUE); 
		m_pShowTips->AddTool(this,LPSTR_TEXTCALLBACK,rtClient,20000);
		EnableToolTips(TRUE); 	

	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;		
		if(m_pShowTips)
		{
			m_pShowTips->DestroyWindow();
			delete m_pShowTips;
			m_pShowTips = NULL;
		}
		RestoreFont(m_hWnd);
	}
}
// CTPTabCtrlEx 消息处理程序


void CTPTabCtrlEx::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CTabCtrl::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

BOOL CTPTabCtrlEx::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bCreate = TRUE;

	return CTabCtrl::PreCreateWindow(cs);
}
BOOL CTPTabCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;//CTabCtrl::OnEraseBkgnd(pDC);
}
int CTPTabCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CTPTabCtrlEx::OnDestroy()
{
	SubWindow(FALSE);
	if(m_pSliderBar)
	{
		m_pSliderBar ->DestroyWindow();
		delete m_pSliderBar;
		m_pSliderBar = NULL;
	}
	CTabCtrl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CTPTabCtrlEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CTabCtrl::OnPaint()
	PaintDC(&dc);
}

void CTPTabCtrlEx::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}

LRESULT CTPTabCtrlEx::OnNcHitTest(CPoint point)
{

//	if(m_rtTab.PtInRect(point))
		return HTCLIENT;
//	else return HTTRANSPARENT;
}


LRESULT CTPTabCtrlEx::OnMsgDeleteItem(WPARAM wp,LPARAM lp)
{
	int nItem = (int)wp;
	if(nItem<0 || nItem >= m_aItem.GetSize()) return FALSE;
	if(m_pSelItem == m_aItem[nItem])  m_pSelItem = NULL;
	m_iTopSize -= m_aItem[nItem] ->rtItem.Width();
	m_iTopSize  = max(0,m_iTopSize);
	delete m_aItem[nItem];
	m_aItem.RemoveAt(nItem);

	if(m_uPrivateState & TABCTRL_REPAINT)
	{
		FunCalcSize();
		InvalidateRect(NULL);
	}
	
	return TRUE;
}
LRESULT CTPTabCtrlEx::OnMsgDeleteAllItems(WPARAM wp,LPARAM lp)
{
	m_pSelItem = NULL;
	for(INT_PTR l=0;l<m_aItem.GetSize();l++)
	{
		delete m_aItem[l];
	}
	m_aItem.RemoveAll();	

	if(m_uPrivateState & TABCTRL_REPAINT)
	{
		FunCalcSize();
		CDC *pDC = GetDC();
		PaintDC(pDC);
		ReleaseDC(pDC);
	}

	return TRUE;
}
void CTPTabCtrlEx::SetItemText(int nIndex,LPCTSTR sText)
{
	if(nIndex <0 || nIndex>= (int)m_aItem.GetSize()) return;
	m_aItem[nIndex] ->sText =sText;

	CDC *pDC = GetDC();
	CFont *pFont = pDC ->SelectObject(TP_GetSysFont(m_iFontIndex));
	CRect rtText = CRect(0,0,0,0);
	pDC ->DrawText(m_aItem[nIndex] ->sText,rtText,DT_CALCRECT|DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	pDC ->SelectObject(pFont);
	m_aItem[nIndex] ->szText  = CSize(rtText.Width()+TAB_EXTWIDTH,rtText.Height());
	ReleaseDC(pDC);


	if(m_uPrivateState & TABCTRL_REPAINT)
	{
		FunCalcSize();
		CDC *pDC = GetDC();
		PaintDC(pDC);
		ReleaseDC(pDC);
	}


}
LRESULT CTPTabCtrlEx::OnMsgInsertItem(WPARAM wp,LPARAM lp)
{
	int     nItem        = (int)wp;
	TCITEM *pTabCtrlItem = (TCITEM *)lp;
	if(nItem <0) nItem = (int)m_aItem.GetSize();
	TPTabCtrlItem *pItem = new TPTabCtrlItem();

	if(pTabCtrlItem ->mask & TCIF_TEXT)  pItem ->sText  = pTabCtrlItem ->pszText;
	if(pTabCtrlItem ->mask & TCIF_PARAM) pItem ->dwData = pTabCtrlItem ->lParam;
	if(pTabCtrlItem ->mask & TCIF_IMAGE) pItem ->iImage = pTabCtrlItem ->iImage;
	if(pTabCtrlItem ->mask & TCIF_STATE) pItem ->uState = pTabCtrlItem ->dwState & pTabCtrlItem ->dwStateMask;

	CDC *pDC = GetDC();
	CFont *pFont = pDC ->SelectObject(TP_GetSysFont(m_iFontIndex));
	CRect rtText = CRect(0,0,0,0);
	pDC ->DrawText(pItem ->sText,rtText,DT_CALCRECT|DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	pDC ->SelectObject(pFont);
	ReleaseDC(pDC);
	pItem ->szText  = CSize(rtText.Width()+TAB_EXTWIDTH,rtText.Height());

	m_aItem.InsertAt(nItem,pItem);

	TPTabCtrlIcon stuIcon ;
	stuIcon.pImage = m_pCloseBox;
	pItem ->aIcon.Add(stuIcon);

	for(INT l=0;l<m_aItem.GetSize();l++)
		m_aItem[l]->uState &= ~TCIS_BUTTONPRESSED;
	
	pItem ->uState |= TCIS_BUTTONPRESSED;


	if(m_uPrivateState & TABCTRL_REPAINT)
	{
		FunCalcSize();
		CDC *pDC = GetDC();
		PaintDC(pDC);
		ReleaseDC(pDC);
	}
	return nItem;
}
LRESULT CTPTabCtrlEx::OnMsgSetImageList(WPARAM wp,LPARAM lp)
{
	HIMAGELIST hImageList = (HIMAGELIST)lp;
	if(lp == NULL) return NULL;
	if(m_hImageList.m_hImageList) m_hImageList.DeleteImageList();
	m_hImageList.Create(CImageList::FromHandle(hImageList));

	IMAGEINFO info;
	if(m_hImageList.GetImageInfo(0,&info))
	{
		m_szImageList = CSize(info.rcImage.right - info.rcImage.left,info.rcImage.bottom - info.rcImage.top);
	}
	else 	m_szImageList = CSize(0,0);
	return (LRESULT)m_hImageList.m_hImageList;
}
LRESULT CTPTabCtrlEx::OnMsgGetImageList(WPARAM wp,LPARAM lp)
{
	return (LRESULT)m_hImageList.m_hImageList;
}
LRESULT CTPTabCtrlEx::OnMsgGetItemCount(WPARAM wp,LPARAM lp)
{
	return (LRESULT)m_aItem.GetSize();
}
LRESULT CTPTabCtrlEx::OnMsgSetCurFocus(WPARAM wp,LPARAM lp)
{
	return S_OK;
}
LRESULT CTPTabCtrlEx::OnMsgGetCurFocus(WPARAM wp,LPARAM lp)
{
	return S_OK;
}
LRESULT CTPTabCtrlEx::OnMsgGetCurSel(WPARAM wp,LPARAM lp)
{
	for(int l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l]->uState &	TCIS_BUTTONPRESSED) return l;
	}
	return -1;
}
LRESULT CTPTabCtrlEx::OnMsgSetCurSel(WPARAM wp,LPARAM lp)
{
	int iItem = (int)wp;
	if(iItem <0 || iItem >=m_aItem.GetSize()) return S_FALSE;

	for(int l=0;l<m_aItem.GetSize();l++)
	{
		if(iItem == l)	m_aItem[l]->uState |= TCIS_BUTTONPRESSED;
		else        	m_aItem[l]->uState &= ~TCIS_BUTTONPRESSED;
	}
	TPTabCtrlItem *pItem  = m_aItem[iItem];
	if(pItem ->rtItem.left <m_rtTab.left)
	{
		m_iTopSize += pItem ->rtItem.left -m_rtTab.left;
		m_pSliderBar ->SetPos(m_iTopSize);
		FunCalcSize(FALSE);
	}
	else if(pItem ->rtItem.right >m_rtTab.right)
	{
		m_iTopSize += pItem ->rtItem.right -m_rtTab.right;
		m_pSliderBar ->SetPos(m_iTopSize);
		FunCalcSize(FALSE);
	}
	if(m_uPrivateState & TABCTRL_REPAINT) Invalidate(FALSE);

	m_pSelItem = m_aItem[iItem];
	return S_OK;
}
LRESULT CTPTabCtrlEx::OnMsgGetRowCount(WPARAM wp,LPARAM lp)
{
	return S_OK;
}
void CTPTabCtrlEx::FunCalcSize(BOOL bSlider)
{
	if(m_uPrivateState & TABCTRL_SINGLELINE) FunCalcSizeSingLine(bSlider);
	else FunCalcSizeMutilLine(bSlider);
}
void CTPTabCtrlEx::FunCalcSizeMutilLine(BOOL bSlider)
{
}
int CTPTabCtrlEx::GetMinHeight()
{
	 return 2 + m_iBarSize + m_iTabSize;
}
void CTPTabCtrlEx::FunCalcSizeSingLine(BOOL bSlider)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	if(m_eTabDir == enTabCtrlTop || m_eTabDir == enTabCtrlBottom)
	{
		if(m_iLockSize != 0)
		{
			if(m_eTabDir == enTabCtrlTop)
			{
				m_rtBar      = CRect(TAB_EDGE,0,rtClient.right - TAB_EDGE,m_iBarSize);
				m_rtClient   = CRect(TAB_EDGE,m_iBarSize,rtClient.right - TAB_EDGE,m_iBarSize + m_iTabSize);
				if(m_iTabSize>0) {	m_rtWnd      = m_rtClient;  m_rtWnd.left = m_rtWnd.right   - m_iLockSize; }
				else             {	m_rtWnd      = m_rtClient;  m_rtWnd.left = m_rtClient.left - m_iLockSize; }
				m_rtWnd.bottom -= 2;
				m_rtTab = m_rtClient;
				m_rtTab.right = m_rtWnd.left;
			}
			else
			{
			}
		}
		else
		{
			if(m_eTabDir == enTabCtrlTop)
			{
				m_rtBar      = CRect(TAB_EDGE,0,rtClient.right - TAB_EDGE,m_iBarSize);
				m_rtClient   = CRect(TAB_EDGE,m_iBarSize+2,rtClient.right - TAB_EDGE,m_iBarSize + m_iTabSize+2);
				m_rtWnd      = CRect(rtClient.right - TAB_EDGE,m_iBarSize,rtClient.right - TAB_EDGE,m_iBarSize + m_iTabSize);
				m_rtWnd.bottom -= 2;
				m_rtTab = m_rtClient;
			}
			else
			{
			}
		}
		int iCount            = (int)m_aItem.GetSize();
		int iCx               = m_rtTab.left - m_iTopSize;
		TPTabCtrlIcon *pIcon  = NULL;
		TPTabCtrlItem *pItem  = NULL;
		for(int i=0;i<iCount;i++)
		{
			pItem    = m_aItem[i];
			pItem ->nIndex = i;

			pItem ->rtItem = m_rtTab;
			pItem ->rtItem.left = iCx;
			if(m_hImageList.m_hImageList && pItem ->iImage >=0)
			{
				pItem ->rtImage = CRect(iCx + 8,m_rtTab.top + (m_rtTab.Height() - m_szImageList.cy)/2  ,iCx + 8 + m_szImageList.cx ,m_rtTab.top + (m_rtTab.Height() + m_szImageList.cy)/2);
				iCx += m_szImageList.cx + 10;
			}
			else
			{
				iCx += 8;
				pItem ->rtImage = CRect(0,0,0,0);
			}
			pItem -> rtText = m_rtTab;
			pItem -> rtText.left  = iCx;
			pItem -> rtText.right = pItem ->szText.cx + iCx;
			pItem -> rtText.top  += 2; 
			iCx = pItem -> rtText.right +4 ;


			for(int k=0;k<pItem->aIcon.GetSize();k++)
			{
				pIcon  = &pItem->aIcon[k];
				if(pIcon ->pImage && pIcon ->pImage ->GetImage())
				{
					pIcon ->rtIcon = CRect(iCx ,m_rtTab.top + (m_rtTab.Height() - pIcon ->pImage ->Height())/2+1  ,iCx + pIcon ->pImage ->Width() ,m_rtTab.top + (m_rtTab.Height() + pIcon ->pImage ->Height() )/2);
					iCx += pIcon ->pImage ->Width() +2;
				}							
			}
			pItem ->rtItem.right = max(iCx+8,pItem ->rtItem.left + 10);	
			iCx = pItem ->rtItem.right-3;
			
			if(bSlider)
			{			
				if(iCx - (m_rtTab.left - m_iTopSize) > rtClient.Width())
				{
					m_pSliderBar ->m_bFoceHideBar  = FALSE;				
					m_pSliderBar ->SetRange(0,iCx - (m_rtTab.left - m_iTopSize)  + 20 -rtClient.Width());
					m_pSliderBar ->SetPageSize(rtClient.Width());
					m_pSliderBar ->SetPos(m_iTopSize);
				}
				else
				{
					m_pSliderBar ->m_bFoceHideBar  = TRUE;
					m_pSliderBar ->Invalidate();
				}
			}
		}
	}
	else if(m_eTabDir == enTabCtrlLeft || m_eTabDir == enTabCtrlRight)
	{
	}
}

void CTPTabCtrlEx::PaintDC(CDC *pDC)
{
	CRect     rtClient,rtTab;
	GetClientRect(&rtClient);	
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());		
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));	

	int iCount = (int)m_aItem.GetSize();
	DRAWITEMSTRUCT drawItem;
	drawItem.hDC   = cdcMemo.m_hDC;
	TPTabCtrlItem  *pItem  = NULL;
	m_szSelItem = CSize(0,0);

	if(m_eTabDir == enTabCtrlTop)
	{
		for(INT l=0;l<iCount;l++)
		{
			pItem = m_aItem[l];
			
			if(pItem ->rtItem.right  < rtClient.left) continue;
			else if(pItem ->rtItem.left  > rtClient.right) break;		
			drawItem.itemID    = l;
			drawItem.rcItem    = pItem ->rtItem;
			drawItem.itemState = pItem ->uState;
			DrawItem(&drawItem);
		}	
		int iIndex = TABCTRL_LINE;
		if(m_szSelItem.cy < rtClient.right)
			TP_StretchDIBRect(&cdcMemo,m_pTabBmp[iIndex] ->GetByte(),m_pTabBmp[iIndex]->Width(),m_pTabBmp[iIndex]->Height(),0,0,CRect(m_szSelItem.cy,m_rtTab.bottom-2,rtClient.right,m_rtTab.bottom),FALSE);
		if(m_szSelItem.cx > rtClient.left)
			TP_StretchDIBRect(&cdcMemo,m_pTabBmp[iIndex] ->GetByte(),m_pTabBmp[iIndex]->Width(),m_pTabBmp[iIndex]->Height(),0,0,CRect(rtClient.left,m_rtTab.bottom-2,m_szSelItem.cx,m_rtTab.bottom),FALSE);	
		cdcMemo.FillSolidRect(m_rtWnd.left,m_rtWnd.top,rtClient.right -m_rtWnd.left,m_rtWnd.Height() ,TP_GetSysColor(m_iBackColor));		
		cdcMemo.FillSolidRect(rtClient.left,m_rtWnd.top,rtClient.left + TAB_EDGE,m_rtWnd.Height() ,TP_GetSysColor(m_iBackColor));		

	}	
	pDC ->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
}
void  CTPTabCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct ->itemID <0 || lpDrawItemStruct ->itemID >= (UINT)m_aItem.GetSize()) return;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct ->hDC);	
	TPTabCtrlIcon *pIcon  = NULL;
	TPTabCtrlItem *pItem  = m_aItem[lpDrawItemStruct ->itemID];

	int iIndex = 0;
	if(lpDrawItemStruct ->itemState & TCIS_BUTTONPRESSED)	     {	 pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS)); iIndex = TABCTRL_DOWN; }
	else if(lpDrawItemStruct ->itemState & TCIS_HIGHLIGHTED)	 {	 pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_HIGHLIGHT)); iIndex = TABCTRL_NOR ; }
	else                                                    	 {   pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));      iIndex = TABCTRL_NOR ; }

	if(m_eTabDir == enTabCtrlTop || m_eTabDir == enTabCtrlBottom)
	{
		if(lpDrawItemStruct ->itemState & TCIS_BUTTONPRESSED) 
		{
			m_szSelItem.cx = pItem ->rtItem.left;
			m_szSelItem.cy = pItem ->rtItem.right;
		}

		TP_StretchDIBRect(pDC,m_pTabBmp[iIndex] ->GetByte(),m_pTabBmp[iIndex]->Width(),m_pTabBmp[iIndex]->Height(),15,15,pItem ->rtItem,FALSE,FALSE,2);

		if(m_hImageList.m_hImageList  && pItem->iImage >=0)
			m_hImageList.Draw(pDC,pItem->iImage,CPoint(pItem->rtImage.left,pItem->rtImage.top),ILD_TRANSPARENT);	

		pDC ->DrawText(pItem ->sText,pItem ->rtText,DT_LEFT | DT_SINGLELINE | DT_VCENTER |DT_NOPREFIX);
		iIndex = 0;
		for(int k=0;k<pItem->aIcon.GetSize();k++)
		{
			pIcon  = &pItem->aIcon[k];
			if(pIcon ->pImage && pIcon ->pImage ->GetImage())
			{
				if(lpDrawItemStruct ->itemState & TCIS_BUTTONPRESSED)
				{
					if(m_pDownIcon == pIcon) iIndex = 1;
					else                     iIndex = 3;
				}
				else
				{
					if(m_pDownIcon == pIcon) iIndex = 0;
					else                     iIndex = 2;
				}

				ImageList_Draw(pIcon ->pImage ->GetImage(),iIndex,lpDrawItemStruct ->hDC,pIcon ->rtIcon.left,pIcon ->rtIcon.top,0);			
			}
		}
	}
}




void CTPTabCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);
	if(m_pSliderBar)
		m_pSliderBar ->MoveWindow(CRect(0,0,cx,m_iBarSize));
	FunCalcSize(TRUE);
	if(m_pShowTips)
		m_pShowTips->SetToolRect(this,20000,CRect(0,0,cx,cy));

}

void CTPTabCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CTabCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
	m_iTopSize = m_pSliderBar ->GetPos();
	FunCalcSize(FALSE);
	Invalidate(FALSE);
}

void CTPTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_iDownItem = -1;
	int iItem ;int iIcon;
	if(HitTest(point,iItem,iIcon) == S_OK)
	{
		m_iDownItem = iItem;
		if(iIcon >=0) 
		{
			m_pDownIcon = &m_aItem[iItem] ->aIcon[iIcon];		
			Invalidate(FALSE);
		}
		SetCapture();
	}
//	CTabCtrl::OnLButtonDown(nFlags, point);
}

void CTPTabCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_iDownItem >=0)
	{	
		ReleaseCapture();		
		if(m_pDownIcon)
		{
			Invalidate(FALSE);
			if(m_pDownIcon ->rtIcon.PtInRect(point))
			{
				GetParent() ->PostMessage(WM_TABCTRLEX,m_iDownItem,(LPARAM)m_hWnd);
			}
		}
		else  if(m_pSelItem && m_pSelItem != m_aItem[m_iDownItem] && !(m_pSelItem ->uFlag & TABCTRL_LOCK))
		{			
			NMHDR hdr;
			hdr.code = TCN_SELCHANGE;
			hdr.hwndFrom = m_hWnd;
			hdr.idFrom   = GetDlgCtrlID();
			OnMsgSetCurSel(m_iDownItem,0);
			GetParent() ->SendMessage(WM_NOTIFY,hdr.idFrom,(LPARAM)&hdr);
		}
	}
	m_iDownItem = -1;
	m_pDownIcon = NULL;

	//	CTabCtrl::OnLButtonUp(nFlags, point);
}
LRESULT CTPTabCtrlEx::HitTest(CPoint pt,int &iItem,int &iIcon)
{
	iItem = -1; iIcon = -1;
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l]->rtItem.PtInRect(pt))
		{
			iItem = l;
			for(int k=0;k<m_aItem[l]->aIcon.GetSize();k++)
			{
				if(m_aItem[l] ->aIcon[k].rtIcon.PtInRect(pt))
				{
					iIcon = k;
				}
			}
			return S_OK;
		}
	}
	return S_FALSE;
}
BOOL CTPTabCtrlEx::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{	
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	CPoint ptCur;
	GetCursorPos(&ptCur);
	ScreenToClient(&ptCur);
	for(INT l=0;l<m_aItem.GetSize();l++)
	{
		if(m_aItem[l]->aIcon.GetSize()>0)
		{
			for(INT k=0;k<m_aItem[l]->aIcon.GetSize();k++)
			{
				if(m_aItem[l]->aIcon[k].rtIcon.PtInRect(ptCur))
				{
					lstrcpy(pTTT->szText ,TP_LoadStr(_L("关闭")));
					return TRUE;
				}
			}
		}
		if(m_aItem[l]->rtItem.PtInRect(ptCur))
		{
			lstrcpy(pTTT->szText ,m_aItem[l]->sText);
			return TRUE;
		}
	}
	return  FALSE;	
}
BOOL CTPTabCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	if(m_pShowTips && m_pShowTips->m_hWnd)	m_pShowTips->RelayEvent(pMsg); 

	return CTabCtrl::PreTranslateMessage(pMsg);
}
void CTPTabCtrlEx::SetLockItem(int lIndex,BOOL bLock)
{
	for(int l=0;l<m_aItem.GetSize();l++)
	{
		if(lIndex == l)
		{
			if(!bLock) m_aItem[l]->uFlag &= ~TABCTRL_LOCK;
			else       m_aItem[l]->uFlag |= TABCTRL_LOCK;
		}
		else 
			m_aItem[l]->uFlag &= ~TABCTRL_LOCK;
	}
}