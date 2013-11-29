// TPImageListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPImageButton.h"
#include ".\TPImageButton.h"


// CTPImageButton

IMPLEMENT_DYNAMIC(CTPImageButton, CWnd)
CTPImageButton::CTPImageButton()
{
	m_dwWindowLevel = TPWNDLEVEL1; 

	m_rtImage       = CRect(0,0,0,0);
	m_hTimer        = NULL;
	m_eBorderStyle  = TP_GetBorderStyle();
	m_uState        = 0;
	m_uAlign        = DT_LEFT;
	m_hImagelist    = NULL;
	m_szImage       = CSize(0,0);
	m_bAutoCheck    = TRUE;
	m_bAutoDel      = FALSE;

	for(INT l=0;l<IMAGELIST_MAX;l++)
		m_iImageListIndex[l] = -1;	


	SHFILEINFO    sfi;
	SetImageList((HIMAGELIST)SHGetFileInfo(_L(""),0,&sfi,sizeof(SHFILEINFO),SHGFI_SYSICONINDEX | SHGFI_SMALLICON));
}

CTPImageButton::~CTPImageButton()
{
	if(m_bAutoDel && m_hImagelist)
	{
		ImageList_Destroy(m_hImagelist);
		m_hImagelist = NULL;
	}
}

BEGIN_MESSAGE_MAP(CTPImageButton, CWnd)
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()
void     CTPImageButton::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_STATIC,&wndClass);		
		wndClass.lpszClassName = WC_IMAGEBUTTON;		
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_IMAGEBUTTON,AfxGetInstanceHandle());
}
BOOL CTPImageButton::Create(UINT uStyle,RECT &rtWnd,CWnd *pParentWnd,UINT nID)
{
	m_bAutoCheck = uStyle & BS_AUTOCHECKBOX;
	uStyle &= ~BS_AUTOCHECKBOX;
	return CWnd::Create(WC_LINK,NULL,uStyle,rtWnd,pParentWnd,nID);
}
void CTPImageButton::SetAlign(UINT uAlign)
{
	m_uAlign = uAlign;
}
void CTPImageButton::SetImage(int iNor,int iDown,int iOver,int iDis,int iFocus)
{
	for(INT l=0;l<IMAGELIST_MAX;l++)	m_iImageListIndex[l] = -1;	
	m_iImageListIndex[IMAGELIST_NOR_NOR]   = iNor;
	m_iImageListIndex[IMAGELIST_NOR_DOWN]  = iDown;
	m_iImageListIndex[IMAGELIST_NOR_OVER]  = iOver;
	m_iImageListIndex[IMAGELIST_NOR_FOCUS] = iFocus;
	m_iImageListIndex[IMAGELIST_NOR_DIS]   = iDis;
	ResembleImageIndex();
}
void CTPImageButton::SetImage(int iNorNor,  int iNorDown,  int iNorOver  ,int iNorDis,  int iNorFocus,
						int iCheckNor,int iCheckDown,int iCheckOver,int iCheckDis,int iCheckFocus)
{
	for(INT l=0;l<IMAGELIST_MAX;l++)	m_iImageListIndex[l] = -1;	

	m_iImageListIndex[IMAGELIST_NOR_NOR]   = iNorNor;
	m_iImageListIndex[IMAGELIST_NOR_DOWN]  = iNorDown;
	m_iImageListIndex[IMAGELIST_NOR_OVER]  = iNorOver;
	m_iImageListIndex[IMAGELIST_NOR_FOCUS] = iNorFocus;
	m_iImageListIndex[IMAGELIST_NOR_DIS]   = iNorDis;

	m_iImageListIndex[IMAGELIST_CHECK_NOR]   = iCheckNor;
	m_iImageListIndex[IMAGELIST_CHECK_DOWN]  = iCheckDown;
	m_iImageListIndex[IMAGELIST_CHECK_OVER]  = iCheckOver;
	m_iImageListIndex[IMAGELIST_CHECK_FOCUS] = iCheckFocus;
	m_iImageListIndex[IMAGELIST_CHECK_DIS]   = iCheckDis;

	ResembleImageIndex();
}

void CTPImageButton::SetAutoCheck(BOOL bSet)
{
	m_bAutoCheck = bSet;
}
UINT CTPImageButton::GetState()
{
	return m_uState;
}
void CTPImageButton::SetState(UINT uState)
{
	m_uState = uState;
}
void CTPImageButton::SetImageList(HIMAGELIST hImageList,BOOL bAutoDel)
{
	m_hImagelist = hImageList;
	if(hImageList)
	{
		IMAGEINFO imagInfo;
		CImageList *pImageList = CImageList::FromHandle(hImageList);
		if(!pImageList ->GetImageInfo(0,&imagInfo))
		{
			m_hImagelist = NULL;
			return;
		}
		m_szImage.cx = imagInfo.rcImage.right  - imagInfo.rcImage.left;
		m_szImage.cy = imagInfo.rcImage.bottom - imagInfo.rcImage.top;
	}
	m_bAutoDel = bAutoDel;
}



void CTPImageButton::OnDestroy()
{
	if(m_hTimer)
	{
		KillTimer(m_hTimer);
		m_hTimer = NULL;
	}
	CWnd::OnDestroy();
}

void CTPImageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
//	CWnd::OnLButtonDown(nFlags, point);
	if(m_uState & IMAGELIST_DISABLED) return;	
	if(GetFocus() != this) SetFocus();
	if(m_rtImage.PtInRect(point))
	{
		if(!(m_uState  & IMAGELIST_PUSHED))
		{
			m_uState  |= IMAGELIST_PUSHED;
			Invalidate(FALSE);
		}
	}	
}
void CTPImageButton::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if(m_uState & IMAGELIST_DISABLED) return;	
	if(!m_rtImage.PtInRect(point)) return;
    NMHDR nmLink;
	nmLink.code = NM_CLICK;
	nmLink.hwndFrom = m_hWnd;
	nmLink.idFrom   = GetDlgCtrlID();
	GetParent()->SendMessage(WM_NOTIFY,nmLink.idFrom,(LPARAM)&nmLink);
	if(m_bAutoCheck)
	{
		if(m_uState & IMAGELIST_CHECKED) m_uState &= ~IMAGELIST_CHECKED;
		else                             m_uState |= IMAGELIST_CHECKED;
		Invalidate(FALSE);
	}
	if(m_uState  & IMAGELIST_PUSHED)
	{
		m_uState  &= ~IMAGELIST_PUSHED;
		Invalidate(FALSE);
	}
}

void CTPImageButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_uState & IMAGELIST_DISABLED) return;	
	if(m_rtImage.PtInRect(point))
	{
		if(!(m_uState & IMAGELIST_HOTLIGHT))
		{
			m_uState |= IMAGELIST_HOTLIGHT;
			Invalidate(FALSE);
			if(m_hTimer == NULL) m_hTimer = SetTimer(100,100,NULL);
		}
		if(nFlags & MK_LBUTTON)
		{
			if(!(m_uState  & IMAGELIST_PUSHED))
			{
				m_uState  |= IMAGELIST_PUSHED;
				Invalidate(FALSE);
			}
		}
	}
	else
	{
		if(m_uState & IMAGELIST_HOTLIGHT)
		{
			m_uState &= ~IMAGELIST_HOTLIGHT;
			Invalidate(FALSE);
			if(m_hTimer)
			{
				KillTimer(m_hTimer);
				m_hTimer = NULL;
			}
		}
		if(m_uState  & IMAGELIST_PUSHED)
		{
			m_uState  &= ~IMAGELIST_PUSHED;
			Invalidate(FALSE);
		}
	}
}

void CTPImageButton::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 100)
	{
		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		if(!m_rtImage.PtInRect(ptCursor))
		{		
			if(m_uState & IMAGELIST_HOTLIGHT)
			{
				m_uState &= ~IMAGELIST_HOTLIGHT;
				Invalidate(FALSE);		
			}
			if(m_hTimer)
			{
				KillTimer(m_hTimer);
				m_hTimer = NULL;
			}
			if(m_uState  & IMAGELIST_PUSHED)
			{
				m_uState  &= ~IMAGELIST_PUSHED;
				Invalidate(FALSE);
			}
		}
	}
	else CWnd::OnTimer(nIDEvent);
}

void CTPImageButton::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}
void CTPImageButton::ResembleImageIndex()
{
	int iStateNum[] = {2,4,4,4,2,2};
	int iState[5][4]  =
	{
		{BUTTON_NOR,CHECK_NOR},
		{BUTTON_NOR,BUTTON_DOWN,BUTTON_OVER,BUTTON_FOCUS},
		{CHECK_NOR,CHECK_DOWN,CHECK_OVER,CHECK_FOCUS},		
		{BUTTON_NOR,BUTTON_DIS},
		{CHECK_NOR,CHECK_DIS},		
	};	
	for(INT l=0;l<5;l++)
	{
		for(INT k=1;k<iStateNum[l];k++)
		{
			if(m_iImageListIndex[iState[l][k]] <0)
				m_iImageListIndex[iState[l][k]] = m_iImageListIndex[iState[l][k-1]];			
		}
	}
}
int CTPImageButton::GetImageIndex()
{
	int iImageIndex = 0;
	if(m_uState & IMAGELIST_DISABLED)
	{
		if(m_uState & BST_CHECKED)            iImageIndex = m_iImageListIndex[IMAGELIST_CHECK_DIS];			
		else                                  iImageIndex = m_iImageListIndex[IMAGELIST_NOR_DIS];		
	}	
	else if(m_uState & IMAGELIST_FOCUS)  
	{
		if(m_uState & IMAGELIST_PUSHED) 
		{			
			if(m_uState & IMAGELIST_CHECKED)    iImageIndex = m_iImageListIndex[IMAGELIST_CHECK_DOWN];			
			else                                iImageIndex = m_iImageListIndex[IMAGELIST_NOR_DOWN];
		}
		else
		{
			if(m_uState & IMAGELIST_CHECKED)    iImageIndex = m_iImageListIndex[IMAGELIST_CHECK_FOCUS];		
			else                                iImageIndex = m_iImageListIndex[IMAGELIST_NOR_FOCUS];
		}		
		
	}
	else if(m_uState & IMAGELIST_HOTLIGHT)  
	{			
		if(m_uState & IMAGELIST_CHECKED)         iImageIndex = m_iImageListIndex[IMAGELIST_CHECK_OVER];		
		else                                     iImageIndex = m_iImageListIndex[IMAGELIST_NOR_OVER];		
	}
	else
	{		
		
		if(m_uState & IMAGELIST_CHECKED)      iImageIndex = m_iImageListIndex[IMAGELIST_CHECK_NOR];
		else                                  iImageIndex = m_iImageListIndex[IMAGELIST_NOR_NOR];		
	}
	return iImageIndex;
}
void CTPImageButton::OnPaint()
{
	CPaintDC dc(this);
	CRect rtClient;
	GetClientRect(&rtClient);
	if(m_hImagelist == NULL)
	{
		dc.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_FRAME));
		return;
	}
	else
	{	
		CTPMemoDC cdcMemo(&dc,rtClient.Width(),rtClient.Height());
		cdcMemo.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_FRAME));	
		int iImageIndex = GetImageIndex();
		m_rtImage = CRect(0,0,m_szImage.cx,m_szImage.cy);
		if(m_uAlign & DT_LEFT)        {}
		else if(m_uAlign & DT_RIGHT)  m_rtImage.OffsetRect(rtClient.Width() - m_szImage.cx,0);
		else if(m_uAlign & DT_CENTER) m_rtImage.OffsetRect((rtClient.Width() - m_szImage.cx)/2,0);
		if(m_uAlign & DT_TOP)         {}
		else if(m_uAlign & DT_BOTTOM)  m_rtImage.OffsetRect(0,rtClient.Height() - m_szImage.cy);
		else if(m_uAlign & DT_VCENTER) m_rtImage.OffsetRect(0,(rtClient.Height()- m_szImage.cy)/2);

		if(iImageIndex >=0)
			ImageList_Draw(m_hImagelist,iImageIndex,cdcMemo.m_hDC,m_rtImage.left ,m_rtImage.top,ILD_TRANSPARENT);	
		dc.BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);
	}
}
LRESULT CTPImageButton::OnNcHitTest(CPoint point)
{
	return HTCLIENT;	
}

void CTPImageButton::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	if(m_uState & IMAGELIST_FOCUS)
	{
		m_uState &= ~IMAGELIST_FOCUS;
		Invalidate(FALSE);
	}	
}

void CTPImageButton::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	if(!(m_uState & IMAGELIST_FOCUS))
	{
		m_uState |= IMAGELIST_FOCUS;
		Invalidate(FALSE);
	}
}

BOOL CTPImageButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CWnd::OnEraseBkgnd(pDC);
}

