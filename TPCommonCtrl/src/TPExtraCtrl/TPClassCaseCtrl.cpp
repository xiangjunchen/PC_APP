


#include "stdafx.h"
#include "TPClassCaseCtrl.h"
#include ".\tpclasscasectrl.h"



typedef struct _tagTPClassCaseBar
{
	int          iIndex;
	CString      sCaption;	
	int          iWndHeight;
	UINT         uShowState;  //1  全部显示  2 显示窗口 
	BOOL         bEnable;

	CTPCaseWnd  *pCaseWnd;

	CWnd         *pWnd; 
	CRect         rtWnd;
	BOOL          bAutoDel;
	BOOL          bAutoVScroll;
	BOOL          bAutoHScroll;	
	CWnd         *pParent; 

	CRect         rtBar;
	CRect         rtClose;
	CRect         rtDrop;
	CRect         rtShow;
	UINT          uDownType; 
	CRect         rtCase;
	 
	_tagTPClassCaseBar()
	{
		iIndex       = 0;
		sCaption     = _L("");		
		iWndHeight   = 0;
		uShowState   = 0;
		bEnable      = TRUE;
		pCaseWnd     = NULL;

		pWnd         = NULL;  
		rtWnd        = CRect(0,0,0,0);
		bAutoDel     = FALSE;
		bAutoVScroll = FALSE;
		bAutoHScroll = FALSE;	
		pParent      = NULL; 

		rtBar        = CRect(0,0,0,0);
		rtClose      = CRect(0,0,0,0);
		rtDrop       = CRect(0,0,0,0);
		rtShow       = CRect(0,0,0,0);
		uDownType    = 0; 
	}
	~_tagTPClassCaseBar()
	{
		if(pCaseWnd)
		{
			pCaseWnd ->DestroyWindow();
			delete pCaseWnd;
			pCaseWnd = NULL;
		}
		if(bAutoDel && pWnd)
		{
			pWnd ->DestroyWindow();
			delete pWnd;
		}
		else if(pWnd)
		{
			pWnd ->SetParent(pParent);
		}
	}

}TPClassCaseBar, *LPTPClassCaseBar;

typedef  CArray<LPTPClassCaseBar ,LPTPClassCaseBar &>TPClassCaseBarArray;


 
IMPLEMENT_DYNAMIC(CTPClassCaseCtrl, CWnd)

CTPClassCaseCtrl::CTPClassCaseCtrl()
{
	m_dwWindowLevel = TPWNDLEVEL1; 

	m_bSubWindow   = FALSE;
	m_bCreate      = FALSE;	
	m_eBorderStyle = TP_GetBorderStyle();

	m_aItemRoot    = NULL;

	for(int l=0;l<TPCLASSCASE_MAX;l++) 
		m_pResBar[l] = NULL;

	m_pCursor[0]     = NULL;
	m_pCursor[1]     = NULL;
	m_hSelItem       = NULL; 	
	m_hSingleItem    = NULL;
	m_hDropItem      = NULL;
	m_iDropPos       = -1; 
	m_iDropLine      = -1;
	m_pDragImageList = NULL;

	m_uState         = CLASSCASE_STATE_VERT|CLASSCASE_STATE_DROPMENU |CLASSCASE_STATE_CLOSE ;

	

	m_hDragItem   = NULL;
	m_ptDrag      = CPoint(0,0);
	m_uDragState  = TP_DRAG_UNKNOW;
	m_uLButtonCmd = 0; 
	
}

CTPClassCaseCtrl::~CTPClassCaseCtrl()
{
	SubWindow(FALSE);

	for(int l=0;l<TPCLASSCASE_MAX;l++) 
		TP_ReleaseCommonRes(m_pResBar[l]);
	TP_ReleaseCommonRes(m_pCursor[0]);
	TP_ReleaseCommonRes(m_pCursor[1]);
	if(m_pDragImageList)
	{
		if(m_pDragImageList ->m_hImageList) m_pDragImageList ->DeleteImageList();
		delete m_pDragImageList;
		m_pDragImageList = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTPClassCaseCtrl, CWnd)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()

	ON_COMMAND_RANGE(ID_CLASSCASE_SHOWALL,ID_CLASSCASE_SHOWALL + 100,OnMenuCmd)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void     CTPClassCaseCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_STATIC,&wndClass);		
		wndClass.lpszClassName = WC_CLASSCASECTRL;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_CLASSCASECTRL,AfxGetInstanceHandle());
}

void CTPClassCaseCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		ModifyStyle(0,WS_CLIPCHILDREN,0);
		m_aItemRoot = new TPClassCaseBarArray;

		m_pResBar[TPCLASSCASE_BARUNSEL] = TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\ClassCase\\ClassCaseBarUnSel.bmp"));
		m_pResBar[TPCLASSCASE_BARSEL]   = TP_LoadBmpByte(TP_GetSkinPath() + _L("\\CommonCtrl\\ClassCase\\ClassCaseBarSel.bmp"));
		m_pResBar[TPCLASSCASE_CMDBTN]   = TP_LoadImage(TP_GetSkinPath() + _L("\\CommonCtrl\\ClassCase\\ClassCaseIcon.bmp"),11,RGB(0,128,128));
 
		m_pCursor[0] = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\Cursorns.cur"));
		m_pCursor[1] = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\CursorHandle.cur"));

	 
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;	

		if(m_aItemRoot)
		{
			TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
			for(INT l=0;l<pArry->GetSize();l++)
			{
				delete pArry->GetAt(l);
			}
			pArry ->RemoveAll();
			delete pArry;
			m_aItemRoot = NULL;
		}
	}
}
void CTPClassCaseCtrl::OnDestroy()
{
	SubWindow(FALSE);
	CWnd::OnDestroy();
}

int CTPClassCaseCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}

void CTPClassCaseCtrl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

BOOL CTPClassCaseCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CWnd::PreCreateWindow(cs);
}

void CTPClassCaseCtrl::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}
LRESULT CTPClassCaseCtrl::OnNcHitTest(CPoint point)
{
	return HTCLIENT;
}
void CTPClassCaseCtrl::OnPaint()
{
	CPaintDC dc(this); 	
	CRect rtClient;	
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());
	cdcMemo.FillSolidRect(rtClient,  TP_GetSysColor(COLOR_BASE_FRAME));
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l=0;l<pArry->GetSize();l++)
	{
		pArry ->GetAt(l) ->iIndex = l;
		if((pArry ->GetAt(l) ->uShowState & TP_SHOWSTATE_SHOW))
			PaintBar(&cdcMemo,pArry ->GetAt(l));
	}
	if(m_iDropLine>=0)
	{
		CPen pPen(PS_SOLID,2,TP_GetSysColor(COLOR_BASE_TEXTHIGHLIGHT)),*pSave;
		pSave = cdcMemo.SelectObject(&pPen);
		cdcMemo.MoveTo(0,m_iDropLine+1); cdcMemo.LineTo(rtClient.right,m_iDropLine+1);
		cdcMemo.SelectObject(pSave);
		pPen.DeleteObject();
	}

	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
}
void CTPClassCaseCtrl::PaintBar(CDC *pDC,HCLASSCASEITEM hItem)
{
	TPClassCaseBar *pBar = (TPClassCaseBar*)hItem; 
	if(pBar == NULL) return;

	int iBarIndex = TPCLASSCASE_BARUNSEL;
	BOOL bSelect  = m_hSelItem == hItem;
	if(m_hSelItem == hItem) 
	{
		pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
		iBarIndex = TPCLASSCASE_BARSEL;
	}
	else pDC->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));	
	if(m_uState & CLASSCASE_STATE_VERT)
	{
		TP_StretchDIBRect(pDC,m_pResBar[iBarIndex] ->GetByte(),m_pResBar[iBarIndex] ->Width(),m_pResBar[iBarIndex] ->Height(),5,5,pBar ->rtBar,FALSE);
		if(pBar ->rtBar.Width() > m_pResBar[TPCLASSCASE_CMDBTN] ->Width() + 5)
		{
			CRect rtText =	CRect(pBar ->rtBar.right -m_pResBar[TPCLASSCASE_CMDBTN] ->Width() - 5 ,pBar ->rtBar.top+ 5,pBar ->rtBar.right-5,pBar ->rtBar.top + m_pResBar[TPCLASSCASE_CMDBTN] ->Height() +5);

			if (m_uState & CLASSCASE_STATE_DROPMENU)
			{
				ImageList_Draw(m_pResBar[TPCLASSCASE_CMDBTN] ->GetImage(),bSelect  ?2:6 ,pDC ->m_hDC,rtText.right-25 ,pBar ->rtBar.CenterPoint().y  - 5,ILD_TRANSPARENT);
			}
			if (m_uState & CLASSCASE_STATE_CLOSE)
			{
				ImageList_Draw(m_pResBar[TPCLASSCASE_CMDBTN] ->GetImage(),bSelect  ?3:7 ,pDC ->m_hDC,rtText.right-10 ,pBar ->rtBar.CenterPoint().y  - 5,ILD_TRANSPARENT);
			}

			rtText = pBar ->rtBar;
			rtText.DeflateRect(17,3,m_pResBar[TPCLASSCASE_CMDBTN] ->Width() *2  + 3,3);
			pDC ->DrawText(pBar -> sCaption ,rtText,DT_SINGLELINE|DT_LEFT|DT_VCENTER);

			pBar ->rtClose =  CRect(pBar ->rtBar.right -20 ,pBar ->rtBar.top +5,pBar ->rtBar.right -5,pBar ->rtBar.top +19);
			pBar ->rtDrop  =  CRect(pBar ->rtBar.right -38 ,pBar ->rtBar.top +5,pBar ->rtBar.right -23,pBar ->rtBar.top +19);
			pBar ->rtShow  =  CRect(pBar ->rtBar.left + 4,pBar ->rtBar.top + 6,pBar ->rtBar.left + 18,pBar ->rtBar.top + 20);

			if(pBar ->uDownType == 1 && (m_uState & CLASSCASE_STATE_CLOSE))
				pDC ->Draw3dRect(pBar ->rtClose,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DLEFT));
			else if(pBar ->uDownType == 2 && (m_uState & CLASSCASE_STATE_DROPMENU))
				pDC ->Draw3dRect(pBar ->rtDrop,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DLEFT));
			else if(pBar ->uDownType == 3)
				pDC ->Draw3dRect(pBar ->rtShow,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DLEFT));
		}
		else
		{
			pBar ->rtClose       = CRect(0,0,0,0);
			pBar ->rtDrop        = CRect(0,0,0,0);
			pBar ->rtShow        = CRect(0,0,0,0);
			pBar ->uDownType     = 0;
		}
		if(pBar ->uShowState & TP_SHOWSTATE_EXPAND)		 
			ImageList_Draw(m_pResBar[TPCLASSCASE_CMDBTN] ->GetImage(),bSelect  ?1:5 ,pDC ->m_hDC,pBar ->rtBar.left + 5 ,pBar ->rtBar.CenterPoint().y  - 5,ILD_TRANSPARENT); 
		else			
			ImageList_Draw(m_pResBar[TPCLASSCASE_CMDBTN] ->GetImage(),bSelect  ?0:4 ,pDC ->m_hDC,pBar ->rtBar.left + 5 ,pBar ->rtBar.CenterPoint().y  - 5,ILD_TRANSPARENT);
			
	}
}
 
void CTPClassCaseCtrl::RemoveWnd(CWnd *pWnd)
{
	if(m_aItemRoot  == NULL) return;
	BOOL                 bFind = FALSE;
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l= pArry->GetSize() -1;l>=0;l--)
	{
		if(pArry->GetAt(l) ->pWnd == pWnd) 
		{
			if(m_hSingleItem ==  pArry->GetAt(l))
				m_hSingleItem = NULL;	 
			delete pArry->GetAt(l);
			pArry->RemoveAt(l);
			bFind = TRUE;
		}
	}		
	if(bFind)
	{
		SetSize();
		Invalidate();
	}
}
int CTPClassCaseCtrl::GetWnd(CWnd *pWnd)
{
	if(m_aItemRoot  == NULL) return -1;
 	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l= pArry->GetSize() -1;l>=0;l--)
	{
		if(pArry->GetAt(l) ->pWnd == pWnd) 
		{
			return l;			 
		}
	}	
	return -1;
}
void CTPClassCaseCtrl::SetWndText(CWnd *pWnd,LPCTSTR sText)
{
	if(m_aItemRoot  == NULL) return;
	BOOL                 bFind = FALSE;
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l= pArry->GetSize() -1;l>=0;l--)
	{
		if(pArry->GetAt(l) ->pWnd == pWnd) 
		{
			pArry->GetAt(l)->sCaption = sText;
			bFind = TRUE;
		}
	}		
	if(bFind)
	{
 		Invalidate();
	}
}
 
void CTPClassCaseCtrl::SetWndHeight(CWnd *pWnd,int iWndHeight)
{
	if(m_aItemRoot  == NULL) return;
	BOOL                 bFind = FALSE;
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	TPClassCaseBar      *pBar  = NULL;
	for(INT l= pArry->GetSize() -1;l>=0;l--)
	{
		if(pArry->GetAt(l) ->pWnd == pWnd) 
		{
			pBar = pArry->GetAt(l);
			CRect rtWnd;
			pBar->iWndHeight = iWndHeight+1;
			pBar ->pWnd->GetWindowRect(&rtWnd);
			pBar ->pWnd->SetWindowPos(NULL,0,0,rtWnd.Width(),iWndHeight,SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
 			bFind = TRUE;
			if(pBar ->pCaseWnd)
			{
				pBar ->pCaseWnd->GetWindowRect(&rtWnd);
				pBar ->pCaseWnd ->PostMessage(WM_SIZE,0,MAKELONG(rtWnd.Width(),rtWnd.Height()));
			}			 
		}
	}		
	if(bFind)
	{
		SetSize();
		Invalidate();
	}
}

HCLASSCASEITEM CTPClassCaseCtrl::InsertWnd(CString sCaption,CWnd *pWnd,int iWndHeight ,int iIndex,BOOL bAutoHorz,BOOL bAutoVert,BOOL bAutoDel)
{
	if(m_aItemRoot  == NULL) return NULL;

	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l=0;l<pArry->GetSize();l++)
	{
		if(pArry->GetAt(l) ->pWnd == pWnd) return NULL;
	}		
	TPClassCaseBar *pBar = new TPClassCaseBar();	
	pBar ->sCaption      = sCaption;
	pBar ->uShowState    = 1;
	pBar ->bAutoDel      = bAutoDel;
	pBar ->bAutoHScroll  = bAutoHorz;
	pBar ->bAutoVScroll  = bAutoVert;
	pBar ->iWndHeight    = iWndHeight;
	pBar ->pParent       = pWnd ->GetParent();
	pBar ->pWnd          = pWnd;

	pWnd ->GetWindowRect(&pBar ->rtWnd);
	 
	if(bAutoHorz || bAutoVert)
	{
		pBar ->pCaseWnd = new CTPCaseWnd();
		pBar ->pCaseWnd -> Create(NULL ,NULL,WS_CHILD|WS_CLIPCHILDREN,CRect(0,0,0,0),this,0);			
		pBar ->pCaseWnd ->ShowWindow(SW_HIDE);					
		pBar ->pCaseWnd ->SetSubWnd(pWnd,bAutoVert,bAutoHorz);
	}
	else
	{
		pWnd ->ShowWindow(SW_HIDE);
		pWnd ->SetParent(this);
	}
	if(iIndex <0) iIndex = (int)pArry ->GetSize();
	pArry ->InsertAt(iIndex,pBar);
	m_hSelItem = pBar;
	SetSize();	
	return pBar;
}
void CTPClassCaseCtrl::SetSize() 
{
	if(m_aItemRoot  == NULL) return ;
	if(m_uState & CLASSCASE_STATE_VERT)
	{
		CRect rtCleint,rtWnd;
		GetClientRect(rtCleint);
		if(rtCleint.Height()<24) return;
		TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
		TPClassCaseBar      *pBar  = NULL;

		if(m_hSingleItem && !(((TPClassCaseBar *)m_hSingleItem)->uShowState & TP_SHOWSTATE_SHOW))		 
			m_hSingleItem = NULL;

		SetMainSize();

		if(m_hSingleItem != NULL)
		{
			int iBar = 0,iSize = 0;
			for(INT l=0;l<pArry->GetSize();l++)
			{
				pBar  = pArry ->GetAt(l);
				if((pBar ->uShowState & TP_SHOWSTATE_SHOW) && pBar != m_hSingleItem)
					iBar ++;
			}
			iSize = rtCleint.Height() - iBar * m_pResBar[TPCLASSCASE_BARUNSEL] ->Height();
			for(INT l=0;l<pArry->GetSize();l++)
			{
				pBar  = pArry ->GetAt(l);
				if((pBar ->uShowState & TP_SHOWSTATE_SHOW) && pBar ->bEnable)
				{
					if(pBar != m_hSingleItem)
					{					
						pBar ->rtBar = rtCleint;
						pBar ->rtBar.bottom = pBar ->rtBar.top + m_pResBar[TPCLASSCASE_BARUNSEL] ->Height();
						rtCleint.top = pBar ->rtBar.bottom;
						if(pBar ->pCaseWnd)		pBar ->pCaseWnd ->ShowWindow(SW_HIDE);							
						else    				pBar ->pWnd ->ShowWindow(SW_HIDE) ;						
					}
					else
					{						
						pBar ->rtBar = rtCleint;
						pBar ->rtBar.bottom = pBar ->rtBar.top + m_pResBar[TPCLASSCASE_BARUNSEL] ->Height();
						rtCleint.top = pBar ->rtBar.bottom;
						if(pBar ->uShowState & TP_SHOWSTATE_EXPAND)
						{						
							rtWnd = rtCleint;
							rtWnd.bottom  = rtWnd.top + iSize - m_pResBar[TPCLASSCASE_BARUNSEL] ->Height();
							rtCleint.top = rtWnd.bottom;
							pBar ->rtCase  = rtWnd;
							if(pBar ->pCaseWnd)
							{								
								pBar ->pCaseWnd ->ShowWindow(SW_SHOW) ;
								pBar ->pCaseWnd ->MoveWindow(rtWnd);
							}
							else 
							{
								pBar ->pWnd ->ShowWindow(SW_SHOW) ;
								pBar ->pWnd ->MoveWindow(rtWnd);
							}
						}
						else
						{
							if(pBar ->pCaseWnd)		pBar ->pCaseWnd ->ShowWindow(SW_HIDE);							
							else    				pBar ->pWnd ->ShowWindow(SW_HIDE) ;	
						}
					}							
				}
				else
				{
					if(pBar ->pCaseWnd)		pBar ->pCaseWnd ->ShowWindow(SW_HIDE);							
					else    				pBar ->pWnd ->ShowWindow(SW_HIDE) ;	
				}
			}
		}
		else
		{		
			for(INT l=0;l<pArry->GetSize();l++)
			{
				pBar  = pArry ->GetAt(l);
				if(!(pBar ->uShowState & TP_SHOWSTATE_SHOW)  || !pBar ->bEnable)
				{
					if(pBar ->pCaseWnd)  pBar ->pCaseWnd ->ShowWindow(SW_HIDE);
					else                 pBar ->pWnd ->ShowWindow(SW_HIDE);
				}
				else
				{					
					pBar ->rtBar = rtCleint;
					pBar ->rtBar.bottom = pBar ->rtBar.top + m_pResBar[TPCLASSCASE_BARUNSEL] ->Height();
					rtCleint.top = pBar ->rtBar.bottom;
					if(pBar ->uShowState & TP_SHOWSTATE_EXPAND)
					{
						rtWnd = rtCleint;
						rtWnd.bottom  = rtWnd.top + pBar ->iWndHeight;
						pBar ->rtCase  = rtWnd;
						if(pBar ->pCaseWnd)
						{
							pBar ->pCaseWnd ->ShowWindow(SW_SHOW) ;
							pBar ->pCaseWnd ->MoveWindow(rtWnd);
						}
						else 
						{
							pBar ->pWnd ->ShowWindow(SW_SHOW) ;
							pBar ->pWnd ->MoveWindow(rtWnd);
						}
						rtCleint.top = rtWnd.bottom;
					}
					else
					{
						if(pBar ->pCaseWnd)		pBar ->pCaseWnd ->ShowWindow(SW_HIDE);							
						else    				pBar ->pWnd ->ShowWindow(SW_HIDE) ;	
					}
				}
			}
			for(INT l= (int)pArry->GetSize() - 1;l>=0;l--)
			{
				pBar  = pArry ->GetAt(l);
				if((pBar ->uShowState & TP_SHOWSTATE_SHOW) && pBar ->bEnable)
				{ 
					if(pBar ->uShowState & TP_SHOWSTATE_EXPAND)
					{
						rtWnd = pBar ->rtBar;
						if(rtWnd.bottom + pBar ->iWndHeight  < rtCleint.bottom)
						{
							rtWnd.top     = pBar ->rtBar.bottom;
							rtWnd.bottom  = rtCleint.bottom;
							pBar ->rtCase  = rtWnd;
							if(pBar ->pCaseWnd)	pBar ->pCaseWnd ->MoveWindow(rtWnd);
							else     			pBar ->pWnd ->MoveWindow(rtWnd);						
						}
					}
					break;
				}
			}				
		}
	}
}
BOOL CTPClassCaseCtrl::HitTest(CPoint ptTest,HCLASSCASEITEM &hItem,UINT &uCmd)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	TPClassCaseBar      *pBar  = NULL;
	for(INT l=0;l<pArry->GetSize();l++)
	{	
		pBar = pArry ->GetAt(l);
		if(!(pBar ->uShowState & TP_SHOWSTATE_SHOW) || !pBar ->bEnable) continue;		
		if(pBar ->rtBar.PtInRect(ptTest))
		{
			hItem = pBar;
			if(pBar ->rtClose.PtInRect(ptTest) && m_uState & CLASSCASE_STATE_CLOSE )      uCmd = 1;
			else if(pBar ->rtDrop.PtInRect(ptTest)&& m_uState & CLASSCASE_STATE_DROPMENU )  uCmd = 2;
			else if(pBar ->rtShow.PtInRect(ptTest) )  uCmd = 3;
			else                                      uCmd = 0;
			return TRUE;
		}		
	}
	return FALSE;
}
HCLASSCASEITEM CTPClassCaseCtrl::GetPreItem(HCLASSCASEITEM hItem)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	TPClassCaseBar      *pBar  = NULL;
	INT l=0;
	for(l=0;l<pArry->GetSize();l++)
	{
		if(pArry ->GetAt(l) == hItem) break;
	}
	if(l>=pArry->GetSize() || l<=0) return NULL;	
	for(--l;l>=0;l--)
	{
		pBar = pArry ->GetAt(l);
		if((pBar ->uShowState & TP_SHOWSTATE_SHOW) && pBar ->bEnable) return pBar;		
	}
	return NULL;
}
HCLASSCASEITEM CTPClassCaseCtrl::GetNextItem(HCLASSCASEITEM hItem)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	TPClassCaseBar      *pBar  = NULL;
	INT l=0;
	for(l=0;l<pArry->GetSize();l++)
	{
		if(pArry ->GetAt(l) == hItem) break;
	}
	if(l>=pArry->GetSize() -1) return NULL;	
	for(++l;l<pArry->GetSize();l++)
	{
		pBar = pArry ->GetAt(l);
		if((pBar ->uShowState & TP_SHOWSTATE_SHOW ) && pBar ->bEnable) return pBar;		
	}
	return NULL;
}
HCLASSCASEITEM CTPClassCaseCtrl::GetDropItem(int &iPos)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	TPClassCaseBar      *pBar  = NULL;
	CPoint               ptTest;
	GetCursorPos(&ptTest);
	ScreenToClient(&ptTest);
	for(INT l=0;l<pArry->GetSize();l++)
	{
		pBar = pArry ->GetAt(l);
		if((pBar ->uShowState & TP_SHOWSTATE_SHOW)   && pBar ->bEnable)
		{
			CRect rtItem = pBar ->rtBar;
			if(pBar ->uShowState & TP_SHOWSTATE_EXPAND)
				rtItem.bottom += pBar ->rtCase.Height();			
			if(rtItem.PtInRect(ptTest))
			{
				if(ptTest.y <= rtItem.CenterPoint().y) iPos = 1;
				else iPos = 2;
				return pBar;			
			}
		}
	}
	for(INT l=(int) pArry->GetSize() - 1;l>=0;l--)
	{
		pBar = pArry ->GetAt(l);
		iPos = 2;
		if((pBar ->uShowState & TP_SHOWSTATE_SHOW) && ptTest.y > pBar->rtBar.bottom  && pBar ->bEnable)
			return pBar;
	}
	return NULL;
}
void CTPClassCaseCtrl::SetCurItem(CWnd *pWnd)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l=0;l<pArry->GetSize();l++)
	{
		if(pArry ->GetAt(l) ->pWnd == pWnd)
		{
			SetCurItem(l);
			return;
		}
	}
}
void CTPClassCaseCtrl::SetCurItem(int iPos)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	if(iPos <0 || iPos >= pArry -> GetSize()) return;	 
 	SetSize();
	Invalidate(FALSE);
	if(m_hSelItem != pArry ->GetAt(iPos))
	{
		m_hSelItem = m_hSingleItem;
		GetParent() ->SendMessage(WM_CLASSBARCMD,CLASSCASECTRL_SELCHANGED,(LPARAM)m_hWnd);
	}

}
void CTPClassCaseCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HCLASSCASEITEM hItem;
	UINT           uCmd;
	if(HitTest(point,hItem,uCmd) && uCmd == 0)
	{
		TPClassCaseBar *pBar = (TPClassCaseBar *)hItem;
		if(pBar  ->uShowState & TP_SHOWSTATE_EXPAND) ExpandBar(pBar,FALSE); 
		else                                         ExpandBar(pBar,TRUE);	
		Invalidate(FALSE);
		if(m_hSelItem != hItem)
		{
			m_hSelItem = hItem;	 
			GetParent() ->SendMessage(WM_CLASSBARCMD,CLASSCASECTRL_SELCHANGED,(LPARAM)m_hWnd);		
		}
	}
}

BOOL CTPClassCaseCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCLASSCASEITEM hItem;
	UINT           uCmd;
	CPoint         point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if(HitTest(point,hItem,uCmd))
	{		
		if(point.y < ((TPClassCaseBar *)hItem) ->rtBar.top + 4 && GetPreItem(hItem) != NULL && m_hSingleItem == NULL && (((TPClassCaseBar *)GetPreItem(hItem)) ->uShowState & TP_SHOWSTATE_EXPAND) )
		{
			SetCursor(m_pCursor[0] ->GetCursor());
			return TRUE;
		}
	}	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CTPClassCaseCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	HCLASSCASEITEM hItem;
	UINT           uCmd;
	if(HitTest(point,hItem,uCmd))
	{		
		if(point.y < ((TPClassCaseBar *)hItem) ->rtBar.top + 4 && GetPreItem(hItem) != NULL && m_hSingleItem == NULL &&(((TPClassCaseBar *)GetPreItem(hItem)) ->uShowState & TP_SHOWSTATE_EXPAND))
		{
			m_hDragItem   = GetPreItem(hItem);
			m_ptDrag      = point;
			m_uDragState  = TP_DRAG_BOTTOM;
			m_uState  |= CLASSCASE_STATE_CAPTURE;		
			SetCapture();	

			SetCursor(m_pCursor[0] ->GetCursor());

			GetClientRect(&m_rtClipRect);
			m_rtClipRect.top = ((TPClassCaseBar *)m_hDragItem) ->rtBar.bottom;			

			//创建拖动的ImageList
			TPClassCaseBar *pBar = (TPClassCaseBar *)m_hDragItem;
			CDC *pDC = GetDC();					
			CTPMemoDC cdcMemo(pDC,pBar ->rtBar.Width(),3);			
			cdcMemo.FillSolidRect(pBar ->rtBar,RGB(0,255,0));

			m_pDragImageList = new CImageList;			
			m_pDragImageList ->Create(pBar ->rtBar.Width(),3,ILC_COLOR32|ILC_MASK,1,1);
			m_pDragImageList ->Add(cdcMemo.GetBitmap(),cdcMemo.GetBitmap());

			ReleaseDC(pDC);
			m_pDragImageList->BeginDrag(0, CPoint(0, 0));
			m_pDragImageList->DragEnter(this,point);
		}
		else if(uCmd == 2)
		{
			CTPMenuEx menuLoad,*pSubMenu = NULL;
			menuLoad.LoadMenu(IDR_EXTRAMENU);
			pSubMenu = menuLoad.GetSubMenu(EXTERACTRL_RMENU_CLASSCASEBAR);
			if(m_hSelItem !=hItem) Invalidate(FALSE);	
			m_hSelItem = hItem;					
			if(pSubMenu)
			{
				CPoint point;
				GetCursorPos(&point);
				if(GetPreItem(hItem) == NULL)
				{							
					pSubMenu ->EnableMenuItem(ID_CLASSCASE_MOVETOP,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);					
					pSubMenu ->EnableMenuItem(ID_CLASSCASE_MOVEUP,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);					
				}
				if(GetNextItem(hItem) == NULL)
				{
					pSubMenu ->EnableMenuItem(ID_CLASSCASE_MOVEBOTTOM,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);						
					pSubMenu ->EnableMenuItem(ID_CLASSCASE_MOVEDOWN,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);					
				}
				AppendMenu(pSubMenu);
				TP_ClearMenu(pSubMenu);	
				TP_LoadMenuStr(pSubMenu);
				pSubMenu ->TrackPopupMenu(TPM_LEFTBUTTON,point.x,point.y,this);
			}
			menuLoad.DestroyMenu();			
		}
		else 
		{
			if(m_hSelItem !=hItem || uCmd ) Invalidate(FALSE);	
			if(m_hSelItem !=hItem && hItem)
			{
				m_hSelItem = hItem;		
				GetParent() ->SendMessage(WM_CLASSBARCMD,CLASSCASECTRL_SELCHANGED,(LPARAM)m_hWnd);
			}
			m_uLButtonCmd = uCmd;
			m_hSelItem = hItem;		
			m_ptDrag   = point;
			((TPClassCaseBar *)hItem) ->uDownType = uCmd;
			m_uState  |= CLASSCASE_STATE_CAPTURE;		
			SetCapture();		
		}
	}
}

void CTPClassCaseCtrl::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(m_uDragState  == TP_DRAG_BOTTOM || m_uDragState  == TP_DRAG_BOTTOMOK)
	{
		TP_ClipPoint(m_rtClipRect,point);
		if(point != m_ptDrag)
		{
			m_ptDrag = point;
			m_uDragState  = TP_DRAG_BOTTOMOK;
			m_pDragImageList->DragMove(CPoint(1,point.y - 1));
		}
	}
	else if(m_uDragState  == TP_DRAG_BAR || m_uDragState  == TP_DRAG_BAROK)
	{		
		if(point != m_ptDrag)
		{
			//移动位置
			HCLASSCASEITEM hDropItem = GetDropItem(m_iDropPos);
			int iLine = m_iDropLine;
			if(m_iDropPos == 2 && GetNextItem(hDropItem))
			{
				hDropItem = GetNextItem(hDropItem);
				m_iDropLine = ((TPClassCaseBar *)hDropItem) ->rtBar.top;
			}
			else if(m_iDropPos == 2 && hDropItem)
			{
				m_iDropLine = ((TPClassCaseBar *)hDropItem) ->rtCase.bottom;
			}
			else if(hDropItem)
			{
				m_iDropLine = ((TPClassCaseBar *)hDropItem) ->rtBar.top;
			}
			if(m_iDropLine != iLine)
			{			
				//m_iDropLine = iLine;
				Invalidate(FALSE);	
			}
			m_pDragImageList->DragMove(CPoint(1,point.y - m_iDragLeft));
		}
	}
	else if(m_uLButtonCmd == 0 && (m_uState & CLASSCASE_STATE_CAPTURE))
	{
		if(m_ptDrag != point)
		{
			m_hDragItem   = m_hSelItem;
			m_uDragState  = TP_DRAG_BAR;
			SetCursor(m_pCursor[1] ->GetCursor());
			if(m_pDragImageList) 
			{
				if(m_pDragImageList ->m_hImageList) m_pDragImageList ->DeleteImageList();
				delete m_pDragImageList;
			}
			TPClassCaseBar *pBar = (TPClassCaseBar *)m_hSelItem;
			m_iDragLeft = point.y - pBar ->rtBar.top;

//创建拖动的ImageList
			CDC *pDC = GetDC();					
			CTPMemoDC cdcMemo(pDC,pBar ->rtBar.Width(),pBar ->rtBar.Height());			
			CRect rtBar = pBar ->rtBar;
			pBar ->rtBar = CRect(0,0,rtBar.Width(),rtBar.Height());
			PaintBar(&cdcMemo,pBar);
			cdcMemo.FillSolidRect(pBar ->rtBar,RGB(0,255,0));
			pBar ->rtBar = rtBar;

			m_pDragImageList = new CImageList;			
			m_pDragImageList ->Create(pBar ->rtBar.Width(),pBar ->rtBar.Height(),ILC_COLOR32|ILC_MASK,1,1);
			m_pDragImageList ->Add(cdcMemo.GetBitmap(),cdcMemo.GetBitmap());

			ReleaseDC(pDC);
			m_pDragImageList->BeginDrag(0, CPoint(0, 0));
			m_pDragImageList->DragEnter(this,point);

		}
	}
	//CWnd::OnMouseMove(nFlags, point);
}
void CTPClassCaseCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{		
	if(m_uState & CLASSCASE_STATE_CAPTURE)
	{
		m_uState  &= ~CLASSCASE_STATE_CAPTURE;	
		ReleaseCapture();	
	}
	if(m_uDragState  == TP_DRAG_BOTTOM || m_uDragState  == TP_DRAG_BOTTOMOK)
	{
		m_pDragImageList ->DragLeave(this);
		m_pDragImageList ->EndDrag();
		if(m_pDragImageList) 
		{
			if(m_pDragImageList ->m_hImageList) m_pDragImageList ->DeleteImageList();
			delete m_pDragImageList;
			m_pDragImageList = NULL;
		}	
		if(m_hDragItem && m_uDragState  == TP_DRAG_BOTTOMOK)
		{
			TP_ClipPoint(m_rtClipRect,point);
			((TPClassCaseBar *)m_hDragItem) ->iWndHeight = max(point.y - ((TPClassCaseBar *)m_hDragItem) ->rtBar.bottom,0);
			SetSize();
		}	
		Invalidate(FALSE);
		m_uDragState  = TP_DRAG_UNKNOW;
		SetCursor(NULL);
	}
	if(m_uDragState  == TP_DRAG_BAR || m_uDragState  == TP_DRAG_BAROK)
	{
		HCLASSCASEITEM hDropItem = GetDropItem(m_iDropPos);				
		if(m_hDropItem != m_hSelItem)			
		{
			m_pDragImageList ->DragLeave(this);
			m_pDragImageList ->EndDrag();
			if(m_pDragImageList) 
			{
				if(m_pDragImageList ->m_hImageList) m_pDragImageList ->DeleteImageList();
				delete m_pDragImageList;
				m_pDragImageList = NULL;
			}	
			
			int                  iSrc =0,iDes =0;
			TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
			TPClassCaseBar      *pBar  = NULL;
			for(INT l=0;l<pArry->GetSize();l++)
			{
				if(pArry ->GetAt(l) == m_hSelItem)  iSrc = l;
				if(pArry ->GetAt(l) == hDropItem)   iDes = l;
			}
			if(!(iSrc == iDes || iSrc+1 == iDes && m_iDropPos == 1 || iSrc-1 == iDes && m_iDropPos == 2))
			{
				if(m_iDropPos == 2) iDes ++;
				pBar  = pArry ->GetAt(iSrc);
				pArry ->InsertAt(iDes,pBar);
				if(iSrc>iDes) pArry ->RemoveAt(iSrc + 1);
				else          pArry ->RemoveAt(iSrc);
				SetSize();
			}			
		}	
		
		m_iDropLine   = -1;
		m_iDropPos    = -1;
		m_hDropItem   = NULL;
		m_uDragState  = TP_DRAG_UNKNOW;
		Invalidate(FALSE);
		SetCursor(NULL);
	}
	else
	{
		if(m_hSelItem && ((TPClassCaseBar *)m_hSelItem) ->uDownType !=0)
		{
			((TPClassCaseBar *)m_hSelItem) ->uDownType =0;
			Invalidate(FALSE);
		}	
		HCLASSCASEITEM hItem;
		UINT           uCmd;
		if(HitTest(point,hItem,uCmd) && uCmd == m_uLButtonCmd )
		{
			TPClassCaseBar  *pBar = (TPClassCaseBar  *)hItem;
			if(uCmd == 1)//关闭
			{				
				if(m_hSingleItem == pBar) m_hSingleItem = NULL;
				pBar  ->uShowState &= ~TP_SHOWSTATE_SHOW;
				SetSize();
				Invalidate(FALSE);
			}			
			else if(uCmd == 3)
			{
				if(pBar  ->uShowState & TP_SHOWSTATE_EXPAND) ExpandBar(pBar,FALSE); 
				else                                         ExpandBar(pBar,TRUE);				 
			}
		}
	}			
}
void CTPClassCaseCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CTPMenuEx menuLoad,*pSubMenu = NULL;
	menuLoad.LoadMenu(IDR_EXTRAMENU);
	pSubMenu = menuLoad.GetSubMenu(EXTERACTRL_RMENU_CLASSCASEBAR);
	if(pSubMenu)
	{
		CPoint point;
		GetCursorPos(&point);
		pSubMenu ->EnableMenuItem(ID_CLASSCASE_MOVEBOTTOM,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);					
		pSubMenu ->EnableMenuItem(ID_CLASSCASE_MOVETOP,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);					
		pSubMenu ->EnableMenuItem(ID_CLASSCASE_MOVEUP,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);					
		pSubMenu ->EnableMenuItem(ID_CLASSCASE_MOVEDOWN,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);					
		AppendMenu(pSubMenu);
		TP_ClearMenu(pSubMenu);			
		TP_LoadMenuStr(pSubMenu);
		pSubMenu ->TrackPopupMenu(TPM_LEFTBUTTON,point.x,point.y,this);
	}
	menuLoad.DestroyMenu();			
}
void CTPClassCaseCtrl::AppendMenu(CTPMenuEx *pSubMenu)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	TPClassCaseBar      *pBar  = NULL;
	for(INT l=0;l<pArry->GetSize();l++)
	{
		pBar = pArry ->GetAt(l);
		if(pArry ->GetAt(l) ->bEnable)
		{		
			pSubMenu ->AppendMenu(MF_STRING|MF_BYCOMMAND,ID_CLASSCASE_MOVEBOTTOM +(int)l + 1,pBar ->sCaption);
			if(pBar ->uShowState & TP_SHOWSTATE_SHOW)
			{
				pSubMenu ->CheckMenuItem(ID_CLASSCASE_MOVEBOTTOM +(int)l + 1,MF_BYCOMMAND | MF_CHECKED);					
			}
		}
	}	

}
void CTPClassCaseCtrl::OnMenuCmd(UINT nID)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	TPClassCaseBar      *pBar  = NULL;
	if(nID ==  ID_CLASSCASE_SHOWALL)
	{		
		for(INT l=0;l<pArry->GetSize();l++)
		{
			pBar = pArry ->GetAt(l);
			pBar ->uShowState |= TP_SHOWSTATE_SHOW;
		}
		SetSize();
		Invalidate(FALSE);
	}
	else if(nID ==  ID_CLASSCASE_HIDEALL)
	{
		for(INT l=0;l<pArry->GetSize();l++)
		{
			pBar = pArry ->GetAt(l);
			pBar ->uShowState &= ~TP_SHOWSTATE_SHOW;
		}
		SetSize();
		Invalidate(FALSE);
	}
	else if(nID ==  ID_CLASSCASE_MOVEUP)
	{
		for(INT l=0;l<pArry->GetSize();l++)
		{
			if(m_hSelItem ==  pArry ->GetAt(l))
			{
				if(l>0)
				{
					TPClassCaseBar *p = pArry ->GetAt(l);
					pArry ->SetAt(l,pArry ->GetAt(l-1));
					pArry ->SetAt(l-1,p);
					SetSize();
					Invalidate(FALSE);
					return;
				}
			}			
		}
	}
	else  if(nID ==  ID_CLASSCASE_MOVEDOWN)
	{
		for(INT l=0;l<pArry->GetSize();l++)
		{
			if(m_hSelItem ==  pArry ->GetAt(l))
			{
				if(l<pArry ->GetSize())
				{					
					TPClassCaseBar *p = pArry ->GetAt(l);
					pArry ->SetAt(l,pArry ->GetAt(l+1));
					pArry ->SetAt(l+1,p);
					SetSize();
					Invalidate(FALSE);
					return;
				}
			}			
		}
	}
	else  if(nID ==  ID_CLASSCASE_MOVETOP)
	{
		for(INT l=0;l<pArry->GetSize();l++)
		{
			if(m_hSelItem ==  pArry ->GetAt(l))
			{
				TPClassCaseBar *p = pArry ->GetAt(l);
				pArry ->RemoveAt(l);
				pArry ->InsertAt(0,p);
				SetSize();
				Invalidate(FALSE);
				return;
			}
		}
	}
	else  if(nID ==  ID_CLASSCASE_MOVEBOTTOM)
	{
		for(INT l=0;l<pArry->GetSize();l++)
		{
			if(m_hSelItem ==  pArry ->GetAt(l))
			{
				TPClassCaseBar *p = pArry ->GetAt(l);
				pArry ->RemoveAt(l);
				pArry ->Add(p);
				SetSize();
				Invalidate(FALSE);
				return;
			}
		}
	}
	else
	{
		int nIndex = nID - ID_CLASSCASE_MOVEBOTTOM -1;
		if(nIndex <0 || nIndex >= (int)pArry ->GetSize()) return;
		pBar = pArry ->GetAt(nIndex);

		if(pBar  ->uShowState & TP_SHOWSTATE_SHOW) pBar  ->uShowState &= ~TP_SHOWSTATE_SHOW;
		else                                       pBar  ->uShowState |= TP_SHOWSTATE_SHOW;
		SetSize();
		Invalidate(FALSE);	

	}
}

void CTPClassCaseCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	SetSize();
}
CWnd *CTPClassCaseCtrl::GetSelWnd()
{
	if(m_hSelItem == NULL) return NULL;
	else return ((TPClassCaseBar *)m_hSelItem) ->pWnd;
}
int CTPClassCaseCtrl::GetSelItem()
{
	if(m_hSelItem == NULL) return -1;
	else return ((TPClassCaseBar *)m_hSelItem) ->iIndex;
}
void CTPClassCaseCtrl::ShowItem(CWnd *pWnd,BOOL bShow)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l=0;l<pArry->GetSize();l++)
	{	
		if(pArry ->GetAt(l) ->pWnd == pWnd)
		{
			if(bShow == FALSE)		pArry ->GetAt(l) ->uShowState &= ~TP_SHOWSTATE_SHOW;
			else 		            pArry ->GetAt(l) ->uShowState |=  TP_SHOWSTATE_SHOW;
			SetSize();
		}			
	}
}
void CTPClassCaseCtrl::SetMainSize()
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	TPClassCaseBar      *pBar  = NULL,*pMainBar = NULL;
	CRect                rtCleint;
	GetClientRect(&rtCleint);

	for(INT l=0;l<pArry->GetSize();l++)
	{
		pBar  = pArry ->GetAt(l);
		if(pBar ->uShowState & TP_SHOWSTATE_MAIN)
		{
			pMainBar = pBar;
			break;
		}		 
	}
	if(pMainBar == NULL) return;

	if(m_hSingleItem != NULL)
	{
		if(m_hSingleItem != pBar) return;
		int iBar = 0,iSize = pArry->GetSize();
		for(INT l=0;l<pArry->GetSize();l++)
		{
			pBar  = pArry ->GetAt(l);
			if((pBar ->uShowState & TP_SHOWSTATE_SHOW))
				iBar ++;
		}
		pMainBar ->iWndHeight = rtCleint.Height() - iBar * m_pResBar[TPCLASSCASE_BARUNSEL] ->Height();		 
	}
	else
	{		
		int iHeight = m_pResBar[TPCLASSCASE_BARUNSEL] ->Height();
		for(INT l=0;l<pArry->GetSize();l++)
		{
			pBar  = pArry ->GetAt(l);
			if(!(pBar ->uShowState & TP_SHOWSTATE_SHOW)  || !pBar ->bEnable)
			{			 
			}
			else if(pBar != pMainBar)
			{						 
				iHeight +=   m_pResBar[TPCLASSCASE_BARUNSEL] ->Height();
				if(pBar ->uShowState & TP_SHOWSTATE_EXPAND)
					iHeight +=  pBar ->iWndHeight;				 
			}
		}	
		pMainBar ->iWndHeight = rtCleint.Height() -iHeight;		 
	}
	 
}
void CTPClassCaseCtrl::SetItemState(CWnd *pWnd,DWORD dwAdd,DWORD dwRemove)
{ 
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l=0;l<pArry->GetSize();l++)
	{	
		if(pArry ->GetAt(l) ->pWnd == pWnd)
		{
			pArry ->GetAt(l) ->uShowState &= ~dwRemove;
			pArry ->GetAt(l) ->uShowState |=  dwAdd;

			if(pArry ->GetAt(l) ->pCaseWnd)
			{
				if(pArry ->GetAt(l) ->uShowState & TP_SHOWSTATE_SAMEH)
					pArry ->GetAt(l) ->pCaseWnd ->m_bSameH = TRUE;
				else 
					pArry ->GetAt(l) ->pCaseWnd ->m_bSameH = FALSE;

				if(pArry ->GetAt(l) ->uShowState & TP_SHOWSTATE_SAMEV)
					pArry ->GetAt(l) ->pCaseWnd ->m_bSameV = TRUE;
				else 
					pArry ->GetAt(l) ->pCaseWnd ->m_bSameV = FALSE;
			}			
		}			
	}
}
void CTPClassCaseCtrl::SetStateMode(DWORD dwAdd,DWORD dwRemove)
{
	m_uState &= ~dwRemove;
	m_uState |=  dwAdd;
}
void CTPClassCaseCtrl::ExpandBar(HCLASSCASEITEM pWnd,BOOL bExpand)
{
	TPClassCaseBar  *pBar = (TPClassCaseBar  *)pWnd;
	if(bExpand)
	{
		pBar ->uShowState |=  TP_SHOWSTATE_EXPAND;
		if(pBar ->uShowState  &  TP_SHOWSTATE_SINGLE)
			m_hSingleItem = pWnd;
		else
			m_hSingleItem = NULL;
	}
	else
	{
		pBar ->uShowState &= ~TP_SHOWSTATE_EXPAND;
		if(m_hSingleItem == pWnd)
			m_hSingleItem = NULL;
	}
	SetSize();
}
void CTPClassCaseCtrl::ExpandItem(CWnd *pWnd,BOOL bExpand)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l=0;l<pArry->GetSize();l++)
	{	
		if(pArry ->GetAt(l) ->pWnd == pWnd)
		{
			ExpandBar(pArry ->GetAt(l),bExpand);
		}			
	}
}
void CTPClassCaseCtrl::EnableItem(CWnd *pWnd,BOOL bEnable)
{
	TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
	for(INT l=0;l<pArry->GetSize();l++)
	{	
		if(pArry ->GetAt(l) ->pWnd == pWnd)
		{
 			pArry ->GetAt(l) ->bEnable = bEnable;
			SetSize();
			Invalidate(FALSE);
		}			
	}
}
BOOL CTPClassCaseCtrl::PreTranslateMessage(MSG* pMsg)
{	
	if(pMsg ->message == WM_LBUTTONDOWN && pMsg ->hwnd != m_hWnd)
	{
		CPoint ptTest;	
		GetCursorPos(&ptTest);		
		ScreenToClient(&ptTest);
		TPClassCaseBar      *pBar  = NULL;			 
		TPClassCaseBarArray *pArry = (TPClassCaseBarArray *)m_aItemRoot;
		for(INT l=0;l<pArry->GetSize();l++)
		{	
			pBar = pArry ->GetAt(l);
			if(!(pBar ->uShowState & TP_SHOWSTATE_SHOW)) continue;		
			if(pBar ->rtCase.PtInRect(ptTest) && m_hSelItem != pBar && (pBar ->uShowState & TP_SHOWSTATE_EXPAND)/* && pBar ->pWnd ->m_hWnd == pMsg ->hwnd*/ )
			{
				m_hSelItem = pBar;
				GetParent() ->SendMessage(WM_CLASSBARCMD,CLASSCASECTRL_SELCHANGCLICK,(LPARAM)m_hWnd);
				Invalidate();
				break;
			}
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CTPClassCaseCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理	程序代码和/或调用默认值
	return TRUE;

	return CWnd::OnEraseBkgnd(pDC);
}
