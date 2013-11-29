// TPToolCase.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPToolCase.h"
#include ".\tptoolcase.h"


// CTPToolCase

IMPLEMENT_DYNAMIC(CTPToolCase, CTPWnd)
CTPToolCase::CTPToolCase()
{
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_eBorderStyle  = TP_GetBorderStyle();
	m_dwWindowLevel = TPWNDLEVEL1;
	m_bVert         = FALSE;
	m_bPaint        = TRUE;
    
	m_iBackColor = COLOR_BASE_WINDOW;

	m_hImageList    = NULL;
	m_szImage       = CSize(0,0);

	m_iTopSize      = 0;
}

CTPToolCase::~CTPToolCase()
{
	SubWindow(FALSE);
	for(INT l=0;l<m_aRoot.GetSize();l++)
		delete m_aRoot[l];
	m_aRoot.RemoveAll();
}


BEGIN_MESSAGE_MAP(CTPToolCase, CTPWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void     CTPToolCase::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_WND,&wndClass);		
		wndClass.lpszClassName = WC_TOOLCASE;		
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_TOOLCASE,AfxGetInstanceHandle());
}
void CTPToolCase::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		if(m_dwWindowLevel == TPWNDLEVEL1)	m_iBackColor   = COLOR_BASE_WINDOW;
		else                          		m_iBackColor   = COLOR_BASE_WINDOW2;
		SetFont(TP_GetSysFont(m_iFontIndex));
		//if(GetStyle() & SBS_VERT)		
		m_bVert = TRUE;
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;		
	}
}

void CTPToolCase::PreSubclassWindow()
{	
	CTPWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}

BOOL CTPToolCase::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CTPWnd::PreCreateWindow(cs);
}

int CTPToolCase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}
BOOL CTPToolCase::OnEraseBkgnd(CDC* pDC)
{
	return  TRUE;//::OnEraseBkgnd(pDC);
}
void CTPToolCase::OnDestroy()
{
	SubWindow(FALSE);
	CTPWnd::OnDestroy();
}

void CTPToolCase::OnNcPaint()
{
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}

void CTPToolCase::OnPaint()
{
	CPaintDC dc(this);

	CRect rtClient;
	GetClientRect(&rtClient);
	CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());
	cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));
	
	
	dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);

}
void  CTPToolCase::SetImageList(HIMAGELIST hImageList)
{
	m_hImageList = hImageList;
	if(hImageList)
	{
		IMAGEINFO imagInfo;
		CImageList *pImageList = CImageList::FromHandle(hImageList);
		if(!pImageList ->GetImageInfo(0,&imagInfo))
		{
			m_hImageList = NULL;
			return;
		}
		m_szImage.cx = imagInfo.rcImage.right  - imagInfo.rcImage.left;
		m_szImage.cy = imagInfo.rcImage.bottom - imagInfo.rcImage.top;
	}
}
void CTPToolCase::FunCalcSize()
{
	CRect rtClient;
	GetClientRect(&rtClient);
	int   iCount = (int) m_aRoot.GetSize();
	if(m_bVert)
	{
		for(int l=0;l<iCount;l++)
		{
		}
	}
}
void CTPToolCase::PaintItem(CDC *pDC,int iItem)
{
}
int CTPToolCase::InsertItem(int iItem,CString sLabel,int iImage  )
{
	if(iItem <0 || iItem >(int)m_aRoot.GetSize()) iItem = (int)m_aRoot.GetSize();
	TPToolCaseItem *pItem = new TPToolCaseItem();
	pItem ->iImage = iImage;
	pItem ->sLable = sLabel;
	m_aRoot.InsertAt(iItem,pItem);
	if(m_bPaint)
	{
		FunCalcSize();
		Invalidate(FALSE);
	}
	return iItem;
}
void CTPToolCase::DeleteItem(int iItem)
{
	if(iItem <0 || iItem >=m_aRoot.GetSize()) return;
	delete m_aRoot[iItem];
	m_aRoot.RemoveAt(iItem);
	if(m_bPaint)
	{
		FunCalcSize();
		Invalidate(FALSE);
	}
}
int CTPToolCase::GetItemCount()
{
	return (int)m_aRoot.GetSize();
}
void CTPToolCase::SetItemData(int iItem,LPARAM lParam)
{
	if(iItem <0 || iItem >=m_aRoot.GetSize()) return;
	m_aRoot[iItem] ->pParam = lParam;	
}
LPARAM CTPToolCase::GetItemData(int iItem)
{
	if(iItem <0 || iItem >=m_aRoot.GetSize()) return NULL;
	else return m_aRoot[iItem] ->pParam;
}
void CTPToolCase::SetReDraw(BOOL bPaint)
{
	m_bPaint = bPaint;
}

