// TPDialog.cpp : 实现文件
//

#include "stdafx.h"


// CTPDialog 对话框
BOOL     (*CTPDialog::m_pDlgSysKeyDown)(UINT nChar,UINT nID) = NULL;

IMPLEMENT_DYNAMIC(CTPDialog, CDialog)
CTPDialog::CTPDialog(CWnd* pParent)
: CDialog(CTPDialog::IDD, pParent),CTPBaseFrame()
{
	m_pWndList    = NULL;
	m_bChild      = FALSE;
	m_iBackColor  = COLOR_BASE_FRAME2;
#ifdef TP_NS_EDIT
	m_iFontIndex  = FONT_BASE_TEXT3;
#endif
	m_uFrameType = FRAME_DIALOG;
	m_hFocusWnd  = NULL;
}
CTPDialog::CTPDialog(UINT nID,CWnd* pParent)
: CDialog(nID, pParent),CTPBaseFrame()
{
	m_pWndList    = NULL;
	m_bChild      = FALSE;
	m_iBackColor  = COLOR_BASE_FRAME2;
#ifdef TP_NS_EDIT
	m_iFontIndex  = FONT_BASE_TEXT3;
#endif
	m_uFrameType = FRAME_DIALOG;
	m_hFocusWnd  = NULL;
}
CTPDialog::~CTPDialog()
{
	if(m_pWndList)
	{	
		CTPWndList *pWndList =(CTPWndList *)m_pWndList;	
		CWnd    *pWnd = NULL;
		for(INT l=0;l< pWndList ->GetCount();l++)
		{
			pWnd = (CWnd *)pWndList ->GetAt(l);	
			delete pWnd;
		}
		pWndList ->RemoveAll();
		delete pWndList;
	}
	m_pWndList = NULL;
}

void CTPDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTPDialog, CDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	//ON_WM_MEASUREITEM()
	//ON_WM_DRAWITEM()
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_CLOSE()
	ON_WM_NCACTIVATE()
END_MESSAGE_MAP()


// CTPDialog 消息处理程序
BOOL CTPDialog::GetData(void *pData)
{
	return TRUE;
}
BOOL CTPDialog::IsWindowVisible() const
{
	return m_bShow;
}

BOOL CTPDialog::OnInitDialog()
{
	__super::OnInitDialog();	
	SetFont(TP_GetSysFont(m_iFontIndex));
	ModifyStyle(0,WS_CLIPCHILDREN,0);
	if( m_pWndList) TP_ReleaseControl(this,m_pWndList); 
	m_pWndList = NULL;
	m_pWndList = TP_AutoLoadControl(this,TPWNDLEVEL1);
	m_iBackColor = COLOR_BASE_FRAME;		 
	if(GetStyle () & WS_CHILD)   	m_bChild = TRUE;

	if( (CTPBaseFrame::m_uBaseFlag &  TP_BASESTATE_TOPMOSTTM) && CTPBaseFrame::m_pTMCmd(1,(LPARAM)m_hWnd) != 2)
		CTPBaseFrame::m_uBaseFlag &=  ~TP_BASESTATE_TOPMOSTTM;


	return TRUE; 
}

void CTPDialog::SetPathNameEdit(UINT nID,long lMaxLen)
{
	extern CWnd    *(*TP_ExtraSubControl)(CString ,DWORD );
	if(TP_ExtraSubControl == NULL) return;
	if(m_pWndList == NULL) return;
	CTPWndList *pWndList = (CTPWndList *)m_pWndList;
	for(int l=0;l<pWndList->GetSize();l++)
	{
		if(((CWnd *)pWndList->GetAt(l)) ->GetDlgCtrlID() == nID)
		{
			CTPEdit *pWnd = (CTPEdit *)pWndList->GetAt(l);
			pWnd ->SendMessage(WM_UNSUBCLASSWINDOW,0,0);
			pWnd ->UnsubclassWindow();
			delete pWnd;
			pWndList->RemoveAt(l);	
			pWnd = NULL;
			CWnd *pEdit = TP_ExtraSubControl(_T("TPEditEx"),TPWNDLEVEL1);	
			((CTPEdit *)pEdit) ->m_uPrivateStyle |=	TPEDIT_PATHNAME;
			pEdit ->SubclassDlgItem(nID,this);
			((CTPEdit *)pEdit) ->SetLimitText(lMaxLen);
			void *pAddWnd = pEdit;
			pWndList->Add(pAddWnd);
			break;
		}
	}
}
LRESULT CTPDialog::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{	
	if(!m_bChild || message == WM_SIZE ||  message == WM_COMMAND)
	{
		CTPBaseFrame::m_hBaseWnd     = m_hWnd;
		CTPBaseFrame::m_pfnBaseSuper = m_pfnSuper; 
		return CTPBaseFrame::DefWindowProc(message, wParam, lParam);		
	}
	else return CDialog::DefWindowProc(message, wParam, lParam);
}
void CTPDialog::OnDestroy()
{
	if(m_pWndList) 	TP_ReleaseControl(this,m_pWndList);
	m_pWndList = NULL;
	CDialog::OnDestroy();
}

HBRUSH CTPDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{	
		return TP_GetSysBrush(BRUSH_FRAME);
	}
	else return hbr;
}

BOOL CTPDialog::PreTranslateMessage(MSG* pMsg)
{	
#ifdef TP_NS_EDIT
	//	if(!m_bChild && m_pBaseFrameAppKeyDown && m_pBaseFrameAppKeyDown((UINT)pMsg ->wParam,(UINT)pMsg ->wParam) == S_OK) return S_OK;
	if(CTPBaseFrame::PreTranslateMessageEx(pMsg)) 
		return TRUE;	
#endif	
	if((m_uBaseState & TP_BASESTATE_CANCELKEY) && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)) 
		return FALSE;
	if(pMsg->message == WM_KEYDOWN  && m_pDlgSysKeyDown)
		if(m_pDlgSysKeyDown((UINT)pMsg ->wParam,TP_SYSKEY_MAINMENU)) return TRUE;		
	return CDialog::PreTranslateMessage(pMsg);
}
void CTPDialog::PreSubclassWindow()
{
	CDialog::PreSubclassWindow();
	if(!m_bCreate && m_hBaseWnd)	SubWindow(TRUE);
}
void CTPDialog::OnPaint()
{
	CPaintDC dc(this); 
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));
}

void CTPDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct ->CtlType == ODT_MENU && m_pMenuSys ) m_pMenuSys ->DrawItem(lpDrawItemStruct);
	else 	__super::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CTPDialog::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(lpMeasureItemStruct ->CtlType == ODT_MENU && m_pMenuSys ) m_pMenuSys ->MeasureItem(lpMeasureItemStruct);
	else __super::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CTPDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(m_bChild) return;
	__super::OnClose();
}
CTPDialog *g_pDoModal = NULL;
int        g_iDoModal = 0;
INT_PTR CTPDialog::DoModal()
{
	if(CTPBaseFrame::m_uBaseFlag & TP_BASESTATE_TOPMOSTDLG)
		::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	g_pDoModal = this;
	m_uState  |= STATE_DOMODL;
	CTPBaseFrame::SetModeState(this,TPMODE_DLGBEGIN);
	INT_PTR iReturn = __super::DoModal();
	CTPBaseFrame::SetModeState(this,TPMODE_DLGEND);
	m_uState  &= ~STATE_DOMODL;
	g_pDoModal =  NULL;
	return iReturn;
}
int CTPDialog::GetDialogCount()
{
	return g_iDoModal;
}

CTPDialog * CTPDialog::GetDialog()
{
	return g_pDoModal;
}

INT_PTR CTPDialog::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
#ifdef TP_NS_EDIT			
	pTI ->hwnd     = m_hWnd;	
	pTI ->lpszText = LPSTR_TEXTCALLBACK;
	pTI ->uId      = (UINT)(UINT_PTR)m_hWnd;
	pTI ->uFlags  |= TTF_IDISHWND;
	return  GetDlgCtrlID();		
#else
	return __super::OnToolHitTest(point, pTI);
#endif
}


BOOL CTPDialog::OnNcActivate(BOOL bActive)
{
	return __super::OnNcActivate(bActive);
}

BOOL CTPDialog::Create(UINT nIDTemplate, CWnd* pParentWnd)
{	
	if(m_uFrameType == FRAME_DIALOG)	m_uFrameType = FRAME_DIALOGLESS;
	return __super::Create(nIDTemplate, pParentWnd);
}
//
BOOL CTPDialog::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	if(m_uFrameType == FRAME_DIALOG)	m_uFrameType = FRAME_DIALOGLESS;
	return __super::Create(lpszTemplateName, pParentWnd);
}
void CTPDialog::SetActivate(BOOL bAct)
{
	if(!bAct)
		m_hFocusWnd = ::GetFocus();
	else
	{
		if(m_hFocusWnd && ::IsWindow(m_hFocusWnd) && TP_IsParentWnd(m_hWnd,m_hFocusWnd))
			::SetFocus(m_hFocusWnd);
		m_hFocusWnd = NULL;
	}
}
void CTPDialog::FunTest(TPControlData* pCtrl)
{

}
void CTPDialog::FunTest(TPMarkPointInfo* pData)
{

}