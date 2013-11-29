// TPTimeLine.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include ".\tptimeline.h"
#include <math.h>


#define TP_PAINTTIMECODELINE2
#define SCALE_COUNT  20
// CTPTimeLine
BOOL  CTPTimeLine::m_bXpriTrim    = FALSE;
BOOL *CTPTimeLine::m_pbAudioScrub = NULL;
double (*CTPTimeLine::GetShuttleSpeed)(long &lState) = NULL;


IMPLEMENT_DYNAMIC(CTPTimeLine, CWnd)

#ifdef TP_NS_NET
DWORD g_bSafeLength_Debug = FALSE; //安全区调试模式开关
typedef struct _TLLIMITLENGTH_
{
	DWORD dwTLPointer;
	long  lLimitLength;
	_TLLIMITLENGTH_()
	{
		dwTLPointer = 0;
		lLimitLength = -1;
	}
}TLLIMITLENGTH, *PTLLIMITLENGTH;
CArray<TLLIMITLENGTH, TLLIMITLENGTH &> g_aTLLimitLengths;
CCriticalSection g_csLimitLengths;

void TP_TL_AddLimitLength(DWORD dwTLPointer, long  lLimitLength)
{
	CSingleLock singleLock(&g_csLimitLengths, TRUE);
	int nSize = g_aTLLimitLengths.GetSize();
	int i = 0;
	for ( ; i < nSize; ++i)
	{
		TLLIMITLENGTH &tlLimitLength = g_aTLLimitLengths[i];
		if (tlLimitLength.dwTLPointer == dwTLPointer)
		{
			tlLimitLength.lLimitLength = lLimitLength;
			break;
		}
	}
	if (i >= nSize)
	{
		TLLIMITLENGTH tlLimitLength;
		tlLimitLength.dwTLPointer = dwTLPointer;
		tlLimitLength.lLimitLength = lLimitLength;
		g_aTLLimitLengths.Add(tlLimitLength);
	}
}
long TP_TL_GetLimitLength(DWORD dwTLPointer)
{
	CSingleLock singleLock(&g_csLimitLengths, TRUE);
	int nSize = g_aTLLimitLengths.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		TLLIMITLENGTH &tlLimitLength = g_aTLLimitLengths[i];
		if (tlLimitLength.dwTLPointer == dwTLPointer)
		{
			return tlLimitLength.lLimitLength;
		}
	}
	return -1;
}
void TP_TL_RemoveLimitLength(DWORD dwTLPointer)
{
	CSingleLock singleLock(&g_csLimitLengths, TRUE);
	int nSize = g_aTLLimitLengths.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		TLLIMITLENGTH &tlLimitLength = g_aTLLimitLengths[i];
		if (tlLimitLength.dwTLPointer == dwTLPointer)
		{
			g_aTLLimitLengths.RemoveAt(i);
			break;
		}
	}
}
#endif

CTPTimeLine::CTPTimeLine()
{
	m_pMsgWnd			  = NULL;
	m_dwWindowLevel       = TPWNDLEVEL1; 

	m_dPixelPerFrame	  = 1.0;
	m_dStartFrame		  = 0;
	m_dCurFrame			  = 0;
	m_nTrimIn			  = 1;
	m_nTrimOut			  = 5;
	m_nIndecatorIn        = -1;
	m_nIndecatorOut       = -1;

	m_nMarkIn             = -1;
	m_nMarkOut            = -1;

	m_enVideoType         = VIDEO_25;
	m_bForceDraw          = FALSE;
	m_bUpdateWindow       = TRUE;

	m_bDrawTrim			  = TRUE;
	m_bDragTrim           = TRUE;
	m_bDragScrollBar      = FALSE;
	m_bDrawBlock		  = FALSE;
	m_bAutoRise           = FALSE;
	m_bDragMarkPoint      = TRUE;
	m_bAltDragMarkPoint   = TRUE;
	m_bMarkColor          = FALSE;
	m_bSceneInout         = FALSE;
	m_szSceneInOut        = CSize(140,140);

	m_pShowTips           = NULL;
	m_pShowTipsEx         = NULL;
	m_nMinFrame			  = -1;
	m_nMaxFrame			  = -1;
	m_nMaxTLFrame		  = -1;

#ifdef TP_NS_NET
	m_nMaxFrame			  = 1; //安全区
	g_bSafeLength_Debug = FALSE;
	HKEY hKey;
	if( ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T("SoftWare\\Sony\\Settings"), 0L, KEY_READ, &hKey) )
	{
		DWORD dwType = REG_DWORD;
		DWORD dwSize = sizeof (DWORD);
		RegQueryValueEx ( hKey, _T("SV_SafeLength_Debug"), NULL, &dwType, (LPBYTE)&g_bSafeLength_Debug, &dwSize );
		m_nSafeLength = max(0, m_nSafeLength);

		RegCloseKey(hKey);
	}
	m_nSafeLength		  = 0;				// 理想安全长度
	m_nActualMaxFrame	  = m_nMaxFrame;	// 素材实际长度
#else
	m_nSafeLength         = 0;	   //理想安全长度
	m_nActualMaxFrame     = -1;	   //实际最大长度
#endif

	m_dZoomIn			  = 1.00;
	m_dStartZoomIn		  = 0.00;

	m_dSaveStartZoomIn    = -0.01;
	m_dSaveZoomIn         = -0.01;
	m_dSavePixelPerFrame  = -0.01;
	m_dSaveStatrFrame     = -1;
	m_dSaveCurFrame       = -0.01;
	m_dSecutionFrame       = 0;
	m_bSaveClick          = FALSE;
	m_iDBClickStep        = 10;

	m_iBarHeadState       = 0;
	m_iTimeCodeWidth      = 0;
	m_iTimeCodeLeft       = 0;
	m_iTimeCodeRight      = 0;

	m_iBarHeight          = 0;
	m_iTimeCodeHeight     = 26;
	m_iEdgeDisatnce       = 0;
	m_iTimeCodeType       = 1;
	m_iTimeCodeEdge       = 0;
	m_bDrawViewZoom       = FALSE;
	m_bForceZoom          = TRUE;
	m_iStartFrame         = 0;
	m_iViewFrame          = 100;
	m_bDrawPhantomMark    = FALSE;
	m_nPhantomMarkIn      = -1;
	m_nPhantomMarkOut     = -1;
	m_rtPhantomMarkIn     = CRect(0,0,0,0);
	m_rtPhantomMarkOut    = CRect(0,0,0,0);
	m_iTimeCodeLeft       = 13;
	m_iTimeCodeRight      = 13;
	m_iBackColor          = COLOR_CODE_BACK;
	m_dMinZoomIn	      = 0.001;
	GetSuctionFrame       = NULL;
	PaintTimeLineEx       = NULL;
	m_iCommandType        = 0;

	m_iUnDoReDoIndex      = -1;
	m_iTimeCodeBackColor  = -1;

	m_bShuttleState       = FALSE;
	m_bPassedZero		  = FALSE;
	m_iShuttleState       = 0;
	m_iShuttleIndex       = 0;

	m_rtCursorArea        = CRect(0,0,0,0);
	m_rtLeftHead          = CRect(0,0,14,0);
	m_rtRightHead         = CRect(0,0,14,0);
	m_rtBarTotal          = CRect(0,0,0,0);
	m_rtBarHandle         = CRect(0,0,0,0);
	m_rtTimeCode          = CRect(0,0,0,0);
	m_rtTimeBar           = CRect(0,0,0,0);
	m_rtClient            = CRect(0,0,0,0);
	m_rtTrimIn            = CRect(0,0,0,0);
	m_rtTrimOut           = CRect(0,0,0,0);
	m_rtTimeLine          = CRect(0,0,0,0);
	m_rtTimeLineTest      = CRect(0,0,0,0);
	m_rtClipBox           = CRect(0,0,0,0);
	m_rtClipRect          = CRect(0,0,0,0);

	m_lSceneIn            = -1;
	m_lSceneOut           = -1;

	m_enDragType          = TP_DRAG_UNKNOW;
	m_ptDrag              = CPoint(0,0);
	m_iDrag               = -1;
	m_rtDrag              = CRect(0,0,0,0);

	m_iDrawWidth          = 0;
	m_iDrawHeight         = 0;
	m_pBmpInOut           = NULL;
	m_uBaseState          = TP_PAINT_TRIMINOUT;
	m_hSetCursor          = NULL;

	m_hClientRgn          = NULL;
	m_hInvertRgn          = NULL;
	m_hTimeLineRgn        = NULL;
	m_iTimeLineRgnPos     = 0;
	m_iDrawWidth          = 0;
	m_iDrawHeight         = 0;
	m_hTimer              = -1;
	m_bWndFocus           = FALSE;
	m_bWndEnable          = TRUE;
	m_bUseRgn             = FALSE;
 	m_bPlay               = FALSE;
	m_bResetStart         = TRUE;

	for(INT l=0;l<TL_MAX;l++) m_pResCursor[l] = NULL;
	for(INT l=0;l<TL_PEN_MAX;l++) m_pResPen[l] = NULL;

	m_pResBar             = NULL;
	m_pResBarBack         = NULL;
	m_bSubWindow          = FALSE;
	m_bCreate             = FALSE;
	m_hTimer              = NULL;
	m_hTimerScroll        = 0;
	m_bZooming            = FALSE;
	m_iMenuPoint          = -1;
	m_dChangePage         = 0.95;
	m_dPageTurn           = 0.95;
	m_dXpriCurFrame       =-1;
	m_iTimeLineWidthEx    = 0;
	m_iOffsetPixel        = 0;
	m_bXpriTrimSeek       = TRUE;
	for(INT l=0;l<4;l++) m_dSaveState[l] = -1.0; //XN25559 jw 20091123

}

CTPTimeLine::~CTPTimeLine()
{
	m_arrBlock.RemoveAll();
	m_arrUnDoReDo.RemoveAll();
	m_arrMarkPoint.RemoveAll();

	if(m_bmpDrawSave.m_hObject)
		m_bmpDrawSave.DeleteObject();
	if(m_pBmpInOut && m_pBmpInOut ->m_hObject)
		m_pBmpInOut ->DeleteObject();

	for(INT l=0;l<TL_MAX;l++)
	{
		TP_ReleaseCommonRes(m_pResCursor[l]);
		m_pResCursor[l] = NULL;
	}

	TP_ReleaseCommonRes(m_pResBar);
	m_pResBar = NULL;
	TP_ReleaseCommonRes(m_pResBarBack);
	m_pResBarBack = NULL;

	for(INT l=0;l<TL_PEN_MAX;l++)
	{
		TP_ReleaseCommonRes(m_pResPen[l]);
		m_pResPen[l] = NULL;
	}
	if(m_hTimeLineRgn) DeleteObject(m_hTimeLineRgn);
	m_hTimeLineRgn = NULL;
	if(m_hClientRgn) DeleteObject(m_hClientRgn);
	m_hClientRgn = NULL;
	if(m_hInvertRgn) DeleteObject(m_hInvertRgn);
	m_hInvertRgn = NULL;

	m_arrScenePoint.RemoveAll();
#ifdef TP_NS_EDIT
	m_aBrakeFrame.RemoveAll();
	m_aSuctionFrame.RemoveAll();
	GetSuctionFrame = NULL;
#endif

	SubWindow(FALSE);
}

BEGIN_MESSAGE_MAP(CTPTimeLine, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_WM_ENABLE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0,0xffff, OnToolTipNotify)
	ON_WM_ERASEBKGND()
	ON_WM_CAPTURECHANGED()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONDBLCLK()
	ON_COMMAND_RANGE(ID_MARKCOLOR_DELETE,ID_TIMECODEVIEW_MAX,OnMenuCommond)
END_MESSAGE_MAP()

void CTPTimeLine::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_STATIC,&wndClass);
		wndClass.lpszClassName = WC_TIMELINE;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_TIMELINE,AfxGetInstanceHandle());
}

void CTPTimeLine::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;

		m_pShowTips = new CTPToolTipCtrl();
		m_pShowTips->Create(this,TTS_ALWAYSTIP);
		m_pShowTips->EnableToolTips(TRUE);
		m_pShowTips->Activate(TRUE);
		m_pShowTips->AddTool(this,LPSTR_TEXTCALLBACK,m_rtTimeCode|m_rtBarTotal,20000);
		m_pShowTips ->SetDelayTime(TTDT_INITIAL,500);
		m_pShowTips ->SetDelayTime(TTDT_RESHOW,500);

		if(m_pMsgWnd == NULL) m_pMsgWnd = GetParent();

		CString sSkinPath       = TP_GetSkinPath();
		m_pResCursor[TL_LEFT]   = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorleft.cur"),FALSE,FALSE);
		m_pResCursor[TL_RIGHT]  = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorright.cur"),FALSE,FALSE);
		if(m_pResCursor[TL_EW] == NULL)
			m_pResCursor[TL_EW] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorwe.cur"),FALSE,FALSE);
		m_pResCursor[TL_ARROW]  = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorarrow.cur"),FALSE,FALSE);
		m_pResCursor[TL_STOP]   = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorno.cur"),FALSE,FALSE);
		m_pResCursor[TL_TOP]    = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursortop.cur"),FALSE,FALSE);
		m_pResCursor[TL_BOTTOM] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorbottom.cur"),FALSE,FALSE);
		if(m_pResCursor[TL_NS] == NULL)
			m_pResCursor[TL_NS] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorns.cur"),FALSE,FALSE);
		m_pResCursor[TL_HANDLE] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorhandle.cur"),FALSE,FALSE);
		m_pResCursor[TL_OK]     = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorok.cur"),FALSE,FALSE);

		SetResPath(sSkinPath + _L("\\CommonCtrl\\TimeLine\\TimeLine1"));
		GetClientRect(&m_rtClient);
		OnSize(0,m_rtClient.Width(),m_rtClient.Height());
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;
		if(m_pShowTips)
		{
			m_pShowTips->DestroyToolTipCtrl();
			m_pShowTips = NULL;
		}
	}
}

void CTPTimeLine::SetResPath(CString sPath)
{
	TP_ReleaseCommonRes(m_pResBar);
	m_pResBar = NULL;
	TP_ReleaseCommonRes(m_pResBarBack);
	m_pResBarBack = NULL;

	for(INT l=0;l<TL_PEN_MAX;l++)
	{
		TP_ReleaseCommonRes(m_pResPen[l]);
		m_pResPen[l] = NULL;
	}

	if(m_iTimeCodeType == 3)
		m_pResBar = TP_LoadImage(sPath + _L("\\timelineicon3.bmp"),32,RGB(0,128,128),FALSE,FALSE);
	else
		m_pResBar = TP_LoadImage(sPath + _L("\\timelineicon.bmp"),32,RGB(0,128,128),FALSE,FALSE);

	if(m_bDragScrollBar)
		m_pResBarBack = TP_LoadBmpByte (sPath + _L("\\TLBarBack.bmp"));

	if(m_iTimeCodeType ==1 || m_iTimeCodeType == 3)
	{
		m_pResPen[TL_PEN_UP]   = TP_LoadPen(COLOR_BASE_TEXTFOCUS,PS_SOLID,1);
		m_pResPen[TL_PEN_LINE] = TP_LoadPen(COLOR_BASE_TEXT,PS_SOLID,1);
	}
	else
	{
		m_pResPen[TL_PEN_UP]   = TP_LoadPen(COLOR_CODE_LINE,PS_SOLID,1);
		m_pResPen[TL_PEN_LINE] = TP_LoadPen(COLOR_CODE_LINEDIS,PS_SOLID,1);
	}
	if(m_bSceneInout)
	{
		m_pResPen[TL_PEN_SCENE]    = TP_LoadPen(COLOR_LINE_COMPOSE,PS_SOLID,1);
		m_pResPen[TL_PEN_SCENESEL] = TP_LoadPen(COLOR_RED,PS_SOLID,1);
	}
	m_pResPen[TL_PEN_TLLINE] = TP_LoadPen(COLOR_LINE_LINE,PS_SOLID,1);
}

BOOL CTPTimeLine::PreTranslateMessage(MSG* pMsg)
{
	if(m_pShowTips   && m_pShowTips->m_hWnd)	m_pShowTips->RelayEvent(pMsg);
	if(m_pShowTipsEx && m_pShowTipsEx->m_hWnd)	m_pShowTipsEx->RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CTPTimeLine::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CWnd::PreCreateWindow(cs);
}

BOOL CTPTimeLine::Create(DWORD dwStyle,const RECT& rect,CWnd* pParentWnd, UINT nID)
{
	BOOL bResult = CWnd::Create(NULL,NULL,dwStyle,rect,pParentWnd,nID);
	if(!bResult) return bResult;
	m_pMsgWnd = pParentWnd; 
	return bResult;
}

int CTPTimeLine::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}
void CTPTimeLine::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}
void CTPTimeLine::OnDestroy()
{
	SubWindow(FALSE);
	CWnd::OnDestroy();
}

BOOL CTPTimeLine::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	CPoint ptCursor;
	::GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	if(m_rtTimeCode.PtInRect(ptCursor) || m_rtBarTotal.PtInRect(ptCursor))
	{
		lstrcpy( pTTT->szText, TP_StringFromFrame(m_enVideoType,TP_DoubleToInt64(TranslatePixel(ptCursor.x))));
		m_pShowTips->m_sToolTips = pTTT->szText;
		return  TRUE;
	}
	else return FALSE;
}
int   CTPTimeLine::OnToolHitTest(CPoint point,TOOLINFO* pTI ) const
{
	pTI->hwnd     = m_hWnd;
	pTI->lpszText = LPSTR_TEXTCALLBACK;
	pTI->uId      = (UINT)(UINT_PTR)m_hWnd;
	pTI->uFlags  |= TTF_IDISHWND;
	return  GetDlgCtrlID();
}

void CTPTimeLine::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_pMsgWnd && m_iShuttleState !=0)
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_JOGSHUTTLESTATE,(LPARAM)0); 
	m_iShuttleState=0;
	if(m_enDragType != TP_DRAG_UNKNOW) return;
	if(GetFocus()!=this) SetFocus();
	m_uBaseState &=  ~TP_TLWND_BUTTON; m_uBaseState |=  TP_TLWND_LBUTTON;
	//滑动条左边
	if(nFlags == 0x9870) goto SeekDrag;
	if(m_bDragScrollBar && m_rtLeftHead.PtInRect(point) )
	{
		m_uBaseState |= TP_DRAG_CAPTURE;
		SetCapture();
		m_enDragType         = TP_DRAG_LEFT;
		m_ptDrag             = point;
		m_iDrag              = point.x - m_rtLeftHead.left;
		m_rtCursorArea       = GetTimeBarRect();
		m_rtCursorArea.left  = 0;
		m_rtCursorArea.right = m_rtRightHead.left - (m_rtLeftHead.Width()- (long)m_iDrag);
		SetCursor(m_pResCursor[TL_LEFT] ->GetCursor());
		return;
	}
	//滑动条右边
	else if(m_bDragScrollBar && m_rtRightHead.PtInRect(point))
	{ 
		m_uBaseState |= TP_DRAG_CAPTURE;
		SetCapture();
		m_enDragType         =  TP_DRAG_RIGHT;
		m_ptDrag             =  point;
		m_iDrag              =  point.x - m_rtRightHead.left;
		m_rtCursorArea       =  GetTimeBarRect();
		m_rtCursorArea.left  =  m_rtLeftHead.right + (long)m_iDrag;
		m_rtCursorArea.right =  0x7ffff;
		SetCursor(m_pResCursor[TL_RIGHT] ->GetCursor());
		return;
	}
	//中间滑动条
	else if(m_bDragScrollBar &&  m_rtBarHandle.PtInRect(point))
	{ 
		m_uBaseState |= TP_DRAG_CAPTURE;
		SetCapture();		
		m_enDragType         = TP_DRAG_BAR;
		m_ptDrag             = point;
		m_iDrag              = point.x - m_rtLeftHead.left;
		m_rtCursorArea       = GetTimeBarRect();

		m_rtCursorArea.left  +=  point.x - m_rtLeftHead.left;
		m_rtCursorArea.right -=  m_rtRightHead.right - point.x;
		return;
	}
	else if(point.y <= m_rtTimeCode.bottom && point.y >= m_rtTimeCode.top)
	{
		double dPixelTemp;
		//判断是否在入出点之间，
		dPixelTemp = TranslateFrame(m_dCurFrame);
		if(point.x >= dPixelTemp-3 && point.x <= dPixelTemp + 3)
		{
DragTL:		m_enDragType = TP_DRAG_TIMELINE;
			m_uBaseState |= TP_DRAG_CAPTURE;
			SetCapture();
			m_ptDrag = point;
			m_rtCursorArea = GetTimeCodeRect();
			m_rtCursorArea.InflateRect(10,0); 
			SetCursor(m_pResCursor[TL_EW] ->GetCursor());
			if(m_bXpriTrim && !m_bXpriTrimSeek)
			{
				m_dXpriCurFrame = m_dCurFrame;
				m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMBEGIN,(LPARAM)this); 
			}
			return;
		}
		else if(m_rtTrimIn.PtInRect(point) && m_bDragTrim && TP_IsAltKeyDown())
		{
			m_enDragType = TP_DRAG_TRIMIN;
			m_uBaseState |= TP_DRAG_CAPTURE;
			SetCapture();
			m_ptDrag = point;
			m_rtCursorArea = GetTimeCodeRect();
			m_rtCursorArea.InflateRect(10,0); 
			SetCursor(m_pResCursor[TL_LEFT] ->GetCursor());	
			m_bForceDraw = TRUE;
			InvalidateRect(m_rtTimeCode);
			return;
		}
		else if(m_rtTrimOut.PtInRect(point) && m_bDragTrim && TP_IsAltKeyDown())
		{
			m_enDragType = TP_DRAG_TRIMOUT;
			m_uBaseState |= TP_DRAG_CAPTURE;
			SetCapture();
			m_ptDrag = point;
			m_rtCursorArea = GetTimeCodeRect();
			m_rtCursorArea.InflateRect(10,0);
			SetCursor(m_pResCursor[TL_RIGHT]->GetCursor());
			m_bForceDraw = TRUE;
			InvalidateRect(m_rtTimeCode);
			return;
		}
		//标记点
		CRect rtTemp =  m_rtTimeCode;
		rtTemp.bottom = rtTemp.top + m_szSceneInOut.cx;
		if(TP_IsAltKeyDown()||!m_bAltDragMarkPoint)
		{
			if(point.y <=m_szSceneInOut.cx)
			{
				for(int i=0;i<m_arrMarkPoint.GetSize();i++)
				{
					dPixelTemp = TranslateFrame(m_arrMarkPoint[i].lFrame);
					rtTemp.left  = (long)dPixelTemp -2;
					rtTemp.right = (long)dPixelTemp +2;
					if(rtTemp.PtInRect(point) && m_bDragMarkPoint)
					{
						m_enDragType = TP_DRAG_MARKPOINT;
						m_uBaseState |= TP_DRAG_CAPTURE;
						SetCapture();
						m_ptDrag = point;
						m_rtCursorArea = GetTimeCodeRect();
						m_rtCursorArea.InflateRect(10,0); 
						m_rtCursorArea.top = m_rtCursorArea.bottom - 14;
						SetCursor(m_pResCursor[TL_EW] ->GetCursor());
						m_iDrag        = m_arrMarkPoint[i].lFrame;
						m_rtDrag.left  = (long)m_arrMarkPoint[i].lFrame;
						m_bForceDraw = TRUE;
						InvalidateRect(m_rtTimeCode);
						return;
					}

					if(point.x >= dPixelTemp-3 && point.x <= dPixelTemp + 3 && m_dCurFrame !=m_arrMarkPoint[i].lFrame)
					{
						SetCurFrame(m_arrMarkPoint[i].lFrame,TRUE,TRUE);
						m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEK,(LPARAM)this); 
						goto DragTL;
						return;
					}
				}
			}
			if(m_bSceneInout && point.y>=m_szSceneInOut.cx && point.y<=m_szSceneInOut.cy)
			{
				CPoint ptCursor   = point;
				BOOL   bSetCursor = FALSE;
				CRect  rtTemp     = m_rtTimeCode; 
				rtTemp.top        = m_szSceneInOut.cx;
				rtTemp.bottom     = m_szSceneInOut.cy;
				m_uBaseState     &= ~TP_TLWND_DOSELECT;
				for(int i=0;i<m_arrBlock.GetSize();i++)
				{
					rtTemp.left  = (long)TranslateFrame(m_arrBlock[i].lIn);
					rtTemp.right = (long)TranslateFrame(m_arrBlock[i].lOut);
					if(ptCursor.x<=rtTemp.left+4 && ptCursor.x>=rtTemp.left+2)
					{
						m_iDrag      = i;
						m_enDragType = TP_DRAG_SCENEIN;
						bSetCursor = TRUE;
						SetCursor(m_pResCursor[TL_EW] ->GetCursor());
					}
					else if(ptCursor.x<=rtTemp.right && ptCursor.x>=rtTemp.right-4)
					{
						m_iDrag      = i;
						m_enDragType = TP_DRAG_SCENEOUT;
						bSetCursor = TRUE;
						SetCursor(m_pResCursor[TL_EW] ->GetCursor());
					}
					else if(rtTemp.PtInRect(ptCursor))
					{
						if(TP_IsCtrlKeyDown())
						{
							m_arrBlock[i].bSelect = !m_arrBlock[i].bSelect;
							m_uBaseState |= TP_TLWND_DOSELECT;
						}
						else
						{
							if(!m_arrBlock[i].bSelect)
							{
								for(int k=0;k<m_arrBlock.GetSize();k++)
									m_arrBlock[k].bSelect = (i == k);
								m_uBaseState |= TP_TLWND_DOSELECT;
							}
						}
						if(m_arrBlock[i].bSelect)
						{
							m_iDrag      = i;
							m_enDragType = TP_DRAG_SCENEINOUT;
							bSetCursor = TRUE;
							SetCursor(m_pResCursor[TL_HANDLE] ->GetCursor());
						}
						else
						{
							double dCurFrame = TranslatePixel(point.x);
							dCurFrame = min(dCurFrame,m_nMaxTLFrame);
							dCurFrame = max(dCurFrame,m_nMinFrame);
#ifdef TP_NS_EDIT
							if(TP_IsAltKeyDown() || TP_IsCtrlKeyDown())
							{
								dCurFrame = GetSecution(point,dCurFrame);
								if(!m_bXpriTrim||m_bXpriTrimSeek)
								{
									dCurFrame = min(dCurFrame,m_nMaxTLFrame);
									dCurFrame = max(dCurFrame,m_nMinFrame);
								}
							}
#endif
							m_ptDrag      = point;
							m_rtDrag.left = point.x;
							m_enDragType = TP_DRAG_TIMELINE;
							m_uBaseState |= TP_DRAG_CAPTURE;
							SetCapture();
							m_rtCursorArea = GetTimeCodeRect();
							m_rtCursorArea.InflateRect(10,0);
							return;
						}
					}

					if(bSetCursor)
					{
						m_uBaseState |= TP_DRAG_CAPTURE;
						SetCapture();
						m_ptDrag = point;
						m_rtCursorArea = GetTimeCodeRect();
						m_rtCursorArea.InflateRect(10,0); 
						m_rtCursorArea.top    = m_szSceneInOut.cx;
						m_rtCursorArea.bottom = m_szSceneInOut.cy;
						m_rtDrag.left  = (long)TranslatePixel(point.x)-(long)m_arrBlock[i].lIn;
						m_rtDrag.right = (long)m_arrBlock[i].lOut - (long)TranslatePixel(point.x);
						m_rtDrag.top   = (long)TranslatePixel(point.x);
						m_bForceDraw = TRUE;
						InvalidateRect(m_rtTimeCode);
						return;
					}
				}
			}
		}
	}
SeekDrag:
	SetDragState(point);
	return ;
}
void CTPTimeLine::SetDragState(CPoint point)
{
	//CPoint point;GetCursorPos(&point); ScreenToClient(&point); // XN00023197 wangjin 20091110
	double dCurFrame = TranslatePixel(point.x);
	dCurFrame = min(dCurFrame,m_nMaxTLFrame);
	dCurFrame = max(dCurFrame,m_nMinFrame);

	if(TP_IsAltKeyDown() || TP_IsCtrlKeyDown())
	{
		dCurFrame = GetSecution(point,dCurFrame);
		if(!m_bXpriTrim||m_bXpriTrimSeek)
		{
			dCurFrame = min(dCurFrame,m_nMaxTLFrame);
			dCurFrame = max(dCurFrame,m_nMinFrame);
		}
	}

	m_ptDrag      = point;
	m_rtDrag.left = point.x;
	if(TP_DoubleToInt64(dCurFrame)!=TP_DoubleToInt64(m_dCurFrame))
	{
		if(!m_bXpriTrim || m_bXpriTrimSeek)
		{
			SetCurFrame((long double)TP_DoubleToInt64(dCurFrame),TRUE,TRUE);
			m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEK,(LPARAM)this); 
		}
		else
		{
			m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMBEGIN,(LPARAM)this); 
			m_dXpriCurFrame = dCurFrame;
		}
	}
	else if(m_bXpriTrim&&!m_bXpriTrimSeek) 
	{
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMBEGIN,(LPARAM)this); 
		m_dXpriCurFrame = dCurFrame;
	}
	m_enDragType = TP_DRAG_TIMELINE;
	m_uBaseState |= TP_DRAG_CAPTURE;
	SetCapture();
	m_rtCursorArea = GetTimeCodeRect();
	m_rtCursorArea.InflateRect(10,0); 
}
void CTPTimeLine::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//	CWnd::OnLButtonUp(nFlags, point);
	BOOL bTestPos  = FALSE;
	if(!(m_uBaseState & (TP_TLWND_LBUTTON |TP_TLWND_LDBBUTTON))) return;
	m_uBaseState &= ~(TP_TLWND_LBUTTON |TP_TLWND_LDBBUTTON);
	if(m_enDragType != TP_DRAG_UNKNOW)		TP_ClipPoint(m_rtCursorArea,point);
	if(m_hTimerScroll){ m_hTimerScroll = 0; KillTimer(120);}
 	if(	m_enDragType == TP_DRAG_LEFTOK)
	{
		if(m_ptDrag.x != point.x && m_rtRightHead.Width()>0)
		{
			SetZoomIn((m_rtRightHead.right - (point.x  - m_iDrag))*1.0/m_iTimeCodeWidth,TRUE,TRUE);
			m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);
		}
		m_iBarHeadState = 0;
		m_bForceDraw = TRUE;
		InvalidateRect(m_rtTimeCode);
	}
	else if(m_enDragType == TP_DRAG_RIGHTOK)
	{ 
		if(m_ptDrag.x != point.x && m_rtRightHead.Width()>0)
		{
			SetZoomIn((point.x  - m_iDrag + m_rtRightHead.Width() - m_rtLeftHead.left)*1.0/m_iTimeCodeWidth,TRUE,TRUE);
			m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);
		}
		m_iBarHeadState = 0;
		m_bForceDraw = TRUE;
		InvalidateRect(m_rtTimeCode);
	}
	else if(m_enDragType == TP_DRAG_BAROK)
	{
		if(m_ptDrag.x != point.x)
		{
			SetStartZoomIn((point.x  - m_iDrag - m_rtTimeCode.left)*1.0/m_iTimeCodeWidth,TRUE);
			m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_STARTIN,(LPARAM)this);
		}
	}
	else if(m_enDragType == TP_DRAG_TIMELINEOK)
	{
		BOOL bSendPos = FALSE;
		if(m_hTimer)
		{
			if(*m_pbAudioScrub)
				KillTimer(112);
			else
				KillTimer(110); 
			if(m_hTimer == 110 && (TP_IsAltKeyDown() || TP_IsCtrlKeyDown()))
			{
				double dCurFrame = TranslatePixel(point.x);
				dCurFrame = min(dCurFrame,m_nMaxTLFrame);
				dCurFrame = max(dCurFrame,m_nMinFrame);
				dCurFrame = GetSecution(point,dCurFrame);
				if(!m_bXpriTrim||m_bXpriTrimSeek)
				{
					dCurFrame = min(dCurFrame,m_nMaxTLFrame);
					dCurFrame = max(dCurFrame,m_nMinFrame);
					bTestPos = TRUE;
					bSendPos = TRUE;
					UINT uBase    = m_uBaseState;
					m_uBaseState &= ~ TP_TIMER_SEEK;
					SetCurFrame((long double)TP_DoubleToInt64(dCurFrame),TRUE,TRUE);
					m_uBaseState  = uBase;
					m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEK,(LPARAM)this); 
				}
			}
			m_hTimer = NULL; 
		}

		m_uBaseState &= ~TP_TIMER_SEEK;
		KillTimer(111);
		if(m_bXpriTrim &&  !m_bXpriTrimSeek) SetCurFrame((INT64)m_dXpriCurFrame,FALSE);
		if(*m_pbAudioScrub) {double dCurFrame = m_dCurFrame;m_dCurFrame--;SetCurFrame((long double)dCurFrame,TRUE);}
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEKEND,(LPARAM)this); 
		if(m_ptDrag.x != point.x && !bTestPos) 
		{
			if(m_rtTimeCode.left  >= point.x && m_rtTimeCode.right-m_iTimeCodeEdge < point.x)
			{
				double dCurFrame = TranslatePixel(point.x);		
				if(TP_DoubleToInt64(m_dCurFrame) != TP_DoubleToInt64(dCurFrame))
				{
					bSendPos = TRUE;
					SetCurFrame((long double)TP_DoubleToInt64(dCurFrame),TRUE,TRUE);
					m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEK,(LPARAM)this);
				}
			}
		}
		if(!bSendPos)
		{
			GetTimeLineRgn(TLRGN_LINE);
			m_bForceDraw = TRUE;
			Invalidate(NULL);
		}
		ViewCurFrame();
#ifdef TP_NS_EDIT
		m_uSuctionState = 0;
		m_aSuctionFrame.RemoveAll();
#endif
	}
	else if(m_enDragType == TP_DRAG_TIMELINE)
	{
		if(m_bXpriTrim && m_dXpriCurFrame>=0 && !m_bXpriTrimSeek) 
		{
			SetCurFrame((INT64)m_dXpriCurFrame,FALSE,FALSE);
			m_bForceDraw = TRUE;
			m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEK,(LPARAM)this); 
		}
		m_dXpriCurFrame = 0;
	}
	else if(m_enDragType == TP_DRAG_SCENEINOK ||m_enDragType == TP_DRAG_SCENEOUTOK|| m_enDragType == TP_DRAG_SCENEINOUTOK)
	{
		if(m_hTimer)
		{
			KillTimer(110);
			m_hTimer = NULL;
		}
		m_uBaseState &= ~TP_TIMER_SEEK;

		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SCENEINOUTMOVE,(LPARAM)m_iDrag);
	}
	else if(m_enDragType ==  TP_DRAG_SCENEINOUT && (m_uBaseState & TP_TLWND_DOSELECT) != TP_TLWND_DOSELECT)
	{
		if(m_iDrag>=0 && m_iDrag<m_arrBlock.GetSize())
		{
			for(INT l=0;l<m_arrBlock.GetSize();l++)
			{
				m_arrBlock[l].bSelect = (l == m_iDrag);
			}
			m_bForceDraw = TRUE;
			Invalidate();
		}
	}
	else if(m_enDragType == TP_DRAG_MARKPOINTOK)
	{
		if(m_hTimer)
		{
			KillTimer(110); 
			m_hTimer = NULL;
		}	
		m_uBaseState &= ~TP_TIMER_SEEK;
		if(m_rtTimeCode.left  <= point.x && m_rtTimeCode.right > point.x)
		{
			long dCurFrame = (long)TranslatePixel(point.x);
			if(m_rtDrag.left != dCurFrame && m_nMinFrame <= dCurFrame && dCurFrame <= m_nMaxTLFrame)//DQA8977 jw 20100111
			{
				DelMarkPoint(m_rtDrag.left,FALSE);
				AddMarkPoint(dCurFrame,TRUE);
				m_rtDrag.left  = dCurFrame;
				m_rtDrag.right = dCurFrame;
			}
		}
		struct temp
		{
			CWnd    *pthis;
			__int64  iSrc;
			__int64  iDest;
		}_temp;
		_temp.iDest = m_rtDrag.right;_temp.iSrc = m_iDrag;
		_temp.pthis = this;
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_MOVEMARKPOINT,(LPARAM)&_temp);
	}
	else if(m_enDragType == TP_DRAG_TRIMINOK)
	{
		if(m_hTimer) { KillTimer(111); m_hTimer = NULL;}
		if(m_ptDrag.x != point.x) 
		{
			m_nTrimIn = TP_DoubleToInt64(TranslatePixel(point.x));
			m_bForceDraw = TRUE;
			Invalidate();
		}
		m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMIN,(LPARAM)this);
		m_bForceDraw = TRUE;
		InvalidateRect(m_rtTimeCode);
	}
	else if(m_enDragType == TP_DRAG_TRIMOUTOK )
	{
		if(m_hTimer) { KillTimer(111); m_hTimer = NULL; }
		if(m_ptDrag.x != point.x) 
		{
			m_nTrimOut = TP_DoubleToInt64(TranslatePixel(point.x));
			m_bForceDraw = TRUE;
			Invalidate();
		}
		m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMOUT,(LPARAM)this);
		m_bForceDraw = TRUE;
		InvalidateRect(m_rtTimeCode);
	}
	if(m_enDragType != TP_DRAG_UNKNOW)
	{
		m_iDrag      = -1;
		m_enDragType = TP_DRAG_UNKNOW;
		m_uBaseState &= ~TP_DRAG_CAPTURE;
 		SetCursor(m_pResCursor[TL_ARROW] ->GetCursor()); 
		ReleaseCapture();
		m_bForceDraw = TRUE;
		Invalidate();
	}
	m_bZooming = FALSE;
}
void CTPTimeLine::SetDBClick()
{
	if(m_bSaveClick)
	{
		m_bSaveClick = FALSE;

		double dStartZoomIn ,dZoomIn;
		dStartZoomIn  = (m_dSaveStartZoomIn - m_dStartZoomIn)/m_iDBClickStep;
		dZoomIn       = (m_dSaveZoomIn - m_dZoomIn)/m_iDBClickStep;

		for(int i=0;i<m_iDBClickStep;i++)
		{
			m_dStartZoomIn  += dStartZoomIn;
			m_dZoomIn       +=  dZoomIn;
			m_dPixelPerFrame = m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);	
			m_dStartFrame    = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;
			m_bForceDraw     = TRUE;
			GetTimeLineRgn(TLRGN_LINE);	
			Invalidate();
			if(m_bUpdateWindow) UpdateWindow();
			::Sleep(10);
		}

		m_dStartZoomIn       =  m_dSaveStartZoomIn;
		m_dZoomIn            =  m_dSaveZoomIn;
		m_dPixelPerFrame     =  m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);
		m_dStartFrame        =  (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;
		m_dSaveStartZoomIn   = 0.0;
		m_dSaveZoomIn        = 1.0;
		Invalidate();
	}
	else
	{
		m_bSaveClick = TRUE;
		m_dSaveStartZoomIn  = m_dStartZoomIn;
		m_dSaveZoomIn       = m_dZoomIn;

		double dStartZoomIn  = m_dStartZoomIn /m_iDBClickStep;
		double dZoomIn       = (1-m_dZoomIn)  /m_iDBClickStep;

		for(int i=0;i<m_iDBClickStep;i++)
		{
			m_dStartZoomIn  -= dStartZoomIn;
			m_dZoomIn       +=  dZoomIn;
			m_dPixelPerFrame = m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);	
			m_dStartFrame    = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;

			m_bForceDraw = TRUE;
			GetTimeLineRgn(TLRGN_LINE);
			Invalidate();
			if(m_bUpdateWindow) UpdateWindow();
			::Sleep(10);
		}

		m_dStartZoomIn          =  0;
		m_dZoomIn               =  1.0;
		m_dPixelPerFrame        =  m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);	
		m_dStartFrame           =  (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;

		Invalidate();
	}

	GetTimeLineRgn(TLRGN_LINE);	

	m_bForceDraw = TRUE;
	m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,TIME_CHANGE_ZOOMIN,(LPARAM)this);
}
void  CTPTimeLine::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	SetDBClick();
}
void CTPTimeLine::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);
	m_bZooming  = TRUE;

	if(m_rtTimeBar.PtInRect(point) && m_rtLeftHead.right>=m_rtRightHead.left)
	{
		m_bSaveClick = FALSE;
		SetDBClick();
	}
	else if(m_rtBarHandle.PtInRect(point))
	{
		SetDBClick();
	}
	else if(m_rtTimeCode.PtInRect(point) && m_enDragType ==0)
	{
		m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,TIME_DOUBLE_CLICK,(LPARAM)this);
		m_uBaseState |= TP_TLWND_LDBBUTTON;
		SetDragState(point);
	}

	m_bZooming  = FALSE;
}

LRESULT CTPTimeLine::OnNcHitTest(CPoint point)
{
	return HTCLIENT;
}
void CTPTimeLine::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	m_bWndFocus  = FALSE;
	m_bForceDraw = TRUE;
	Invalidate();
}

void CTPTimeLine::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	m_bWndEnable = bEnable;
	m_bForceDraw = TRUE;
	Invalidate();
}

void CTPTimeLine::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	m_bWndFocus  = TRUE;
	m_bForceDraw = TRUE;
	Invalidate();
}

int CTPTimeLine::GetBaseHight()
{
	return m_iBarHeight + m_iTimeCodeHeight;
}
void  CTPTimeLine::SetPixelPerFrame(double dPixelPerFrame,BOOL bCenter,BOOL bPaint)
{
	if(dPixelPerFrame <=0) return;
	if(dPixelPerFrame == m_dPixelPerFrame) return; 
	SetZoomIn(m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*dPixelPerFrame),bCenter,bPaint);
}

void   CTPTimeLine::SetActivate(BOOL bAct)
{
	m_bWndFocus  = bAct;
	m_bForceDraw = TRUE;
	Invalidate();
}
void   CTPTimeLine::SetStartAndView(INT64 iStartFrame,INT64 iViewFrame)
{
	m_iStartFrame = iStartFrame;
	m_iViewFrame  = iViewFrame;
	Invalidate();
}

void CTPTimeLine::SetActMouseWheel(BOOL bEnable)
{
	if(bEnable) SetWindowText(WC_TIMELINE);
	else        SetWindowText(_T(""));
}
void   CTPTimeLine::SetMaxActualFrame(__int64 iMax)
{
	m_nActualMaxFrame = iMax;
}
void   CTPTimeLine::SetMaxTLFrame(__int64 iMax)
{
	m_nMaxTLFrame = iMax;
}
void   CTPTimeLine::GetMiniZoomIn()
{
	m_dMinZoomIn   = m_rtTimeCode.Width() /90.0/(m_nMaxFrame-m_nMinFrame);
	m_dMinZoomIn   = m_dMinZoomIn/m_rtTimeCode.Width()*GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_dMinZoomIn   = max(0.00001,m_dMinZoomIn);
	m_dMinZoomIn   = min(1.0,m_dMinZoomIn);//if((m_nMaxFrame-m_nMinFrame) *35.0<m_rtTimeCode.Width()) m_dMinZoomIn = 1.0;
}

void   CTPTimeLine::SetStartFrame(long  double dFrame,BOOL bPaint)
{
	dFrame = max(dFrame, static_cast<long double>(m_nMinFrame));
	dFrame = min(dFrame, static_cast<long double>(m_nMaxFrame));
	if(TP_IsDoubleEqual(dFrame, m_dStartFrame)) return; 
	SetStartZoomIn((dFrame- m_nMinFrame)*1.0/(m_nMaxFrame -m_nMinFrame),bPaint);
}

double CTPTimeLine::GetStartZoomIn()
{
	return m_dStartZoomIn;
}
void CTPTimeLine::ValidateRect(LPCRECT lpRect)
{
	m_bForceDraw = TRUE;
	CWnd::ValidateRect(lpRect);
}
double CTPTimeLine::GetZoomIn()
{
	return m_dZoomIn;
}
void CTPTimeLine::SetStartZoomIn(double dZoomIn,BOOL bPaint)
{
	m_dStartZoomIn = dZoomIn;
	if(m_dStartZoomIn < 0.0)            m_dStartZoomIn = 0;
	if(m_dStartZoomIn > 1-m_dMinZoomIn) m_dStartZoomIn = 1-m_dMinZoomIn;

	if(m_dZoomIn > 1-m_dStartZoomIn)    m_dStartZoomIn = 1-m_dZoomIn;

	m_dPixelPerFrame = m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);	
	m_dStartFrame    = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;

	if(bPaint && (m_dSaveStartZoomIn != m_dStartZoomIn || m_dSaveZoomIn != m_dZoomIn))
	{
		m_bForceDraw = TRUE;
		Invalidate();
	}
	m_dSaveStartZoomIn = m_dStartZoomIn;
	m_dSaveZoomIn      = m_dZoomIn;
	GetTimeLineRgn(TLRGN_LINE);	
}
void  CTPTimeLine::EnableViewFrame(long double dCurFrame)
{
	INT64 nPixPos =  TP_DoubleToInt(TranslateFrame(dCurFrame));
	if (nPixPos<m_rtTimeCode.left || nPixPos>m_rtTimeCode.right+1) 
		SetStartFrame(dCurFrame - GetViewFrame()/2);
}

void CTPTimeLine::OnMenuCommond(UINT uID)
{
	switch(uID)
	{
	case ID_MARKCOLOR_DELETE:
		if(m_iMenuPoint<0||m_iMenuPoint>=m_arrMarkPoint.GetSize()) return;
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_MARKDELETE,(LPARAM)m_iMenuPoint);
		return;
	case ID_MARKCOLOR_COMMENT:
		if(m_iMenuPoint<0||m_iMenuPoint>=m_arrMarkPoint.GetSize()) return;
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_MARKCOMMENT,(LPARAM)m_iMenuPoint);
		return;
	case ID_MARKCOLOR_DEFAULT:
	case ID_MARKCOLOR_COLOR1:
	case ID_MARKCOLOR_COLOR2:
	case ID_MARKCOLOR_COLOR3:
	case ID_MARKCOLOR_COLOR4:
	case ID_MARKCOLOR_COLOR5:
		{
			BYTE cType = 0;
			if(m_iMenuPoint<0||m_iMenuPoint>=m_arrMarkPoint.GetSize()) return;
			if(ID_MARKCOLOR_DEFAULT == uID)      cType = 0;
			else if(ID_MARKCOLOR_COLOR1 == uID)  cType = 1;
			else if(ID_MARKCOLOR_COLOR2 == uID)  cType = 2;
			else if(ID_MARKCOLOR_COLOR3 == uID)  cType = 3;
			else if(ID_MARKCOLOR_COLOR4 == uID)  cType = 4;
			else if(ID_MARKCOLOR_COLOR5 == uID)  cType = 5;
			m_arrMarkPoint[m_iMenuPoint].cType = cType;
			Invalidate();
			m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_MARKCOLOR,(LPARAM)m_iMenuPoint);
		}
		return;
	case ID_SCENECOLOR_DELETEALL:
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SCENEINOUTDELETEALL,(LPARAM)m_iMenuPoint);
		return;
	case ID_SCENECOLOR_SETINOUT:
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SCENEINOUTSETINOUT,(LPARAM)m_iMenuPoint);
		return;
	case ID_SCENECOLOR_DELETE:
	case ID_SCENECOLOR_DELETEALLSEL:
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SCENEINOUTDELETEALLSEL,(LPARAM)m_iMenuPoint);
		return;
	case ID_SCENECOLOR_COMMENT:
		{
			if(m_iMenuPoint<0||m_iMenuPoint>=m_arrBlock.GetSize()) return;
			m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SCENEINOUTCOMMENT,(LPARAM)m_iMenuPoint);
		}
		return;
	case ID_SCENECOLOR_SELECTALL:
		{
			if(m_iMenuPoint<0||m_iMenuPoint>=m_arrBlock.GetSize()) return;
			for(INT l=0;l<m_arrBlock.GetSize();l++)
				m_arrBlock[l].bSelect = TRUE;
			Invalidate();
		}
		return;
	case ID_SCENECOLOR_DEFAULT:
	case ID_SCENECOLOR_COLOR1:
	case ID_SCENECOLOR_COLOR2:
	case ID_SCENECOLOR_COLOR3:
	case ID_SCENECOLOR_COLOR4:
	case ID_SCENECOLOR_COLOR5:
		{
			BYTE cType = 0;
			if(m_iMenuPoint<0||m_iMenuPoint>=m_arrBlock.GetSize()) return;
			else if(ID_SCENECOLOR_DEFAULT== uID)  cType = 0;
			else if(ID_SCENECOLOR_COLOR1 == uID)  cType = 1;
			else if(ID_SCENECOLOR_COLOR2 == uID)  cType = 2;
			else if(ID_SCENECOLOR_COLOR3 == uID)  cType = 3;
			else if(ID_SCENECOLOR_COLOR4 == uID)  cType = 4;
			else if(ID_SCENECOLOR_COLOR5 == uID)  cType = 5;
			if(m_arrBlock[m_iMenuPoint].bSelect)
			{
				for(INT l=0;l<m_arrBlock.GetSize();l++)
				{
					if(m_arrBlock[l].bSelect) m_arrBlock[l].cType = cType;
				}
			}
			else
				m_arrBlock[m_iMenuPoint].cType = cType;
			Invalidate();
			m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SCENEINOUTCOLOR,(LPARAM)m_iMenuPoint);
		}
		return;
	}
}


void CTPTimeLine::OnTimer(UINT nIDEvent) 
{
	double dStartFrame = m_dStartFrame;
	if(nIDEvent == 111) //拖动Trim 或时间线到窗口外部，需要自动滚动时间线
	{
		CPoint ptCursor;
		::GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor); //取得当前光标位置

		double dStartZoomIn = m_dStartZoomIn;
		__int64 iMaxFrame = m_nMaxFrame; // DQAdb00008978 wangjin 20100118
		if(m_iTimeCodeType == 2) iMaxFrame = m_nMinFrame + TP_GetFrameFormTime(12,0,0,0,m_enVideoType)-1;

		BOOL bShift = TP_IsShiftKeyDown();
		__int64 nMarkMin = m_nTrimOut-1;
		__int64 nMarkMax = m_nTrimOut;
		if(m_enDragType == TP_DRAG_TRIMIN || m_enDragType == TP_DRAG_TRIMINOK)
		{
			nMarkMin = m_nTrimIn;
			nMarkMax = m_nTrimIn;
		}
		if (bShift)
		{
			nMarkMin = min(m_nTrimIn, m_nTrimOut-1);
			nMarkMax = max(m_nTrimIn, m_nTrimOut);
		}
		__int64 iRangeLeft	= nMarkMin  - m_nMinFrame; // XN00024528 wangjin 20100118
		__int64 iRangeRight	= iMaxFrame - nMarkMax;
		ASSERT(iRangeLeft>=0 && iRangeRight>=0);
		if(ptCursor.x < m_rtTimeCode.left)   //在窗口左边
		{
			if (iRangeLeft>0)
			{
				__int64 iOffset = -5;
				iOffset = max(iOffset, -iRangeLeft);
				iOffset = min(iOffset, iRangeRight);

				if(m_enDragType == TP_DRAG_TRIMINOK) //拖动TrimIn
				{
					if (bShift) m_nTrimOut += iOffset;
					m_nTrimIn += iOffset;
					if (m_nTrimIn-1<m_dStartFrame)	SetStartFrame(static_cast<long double>(max(m_nTrimIn-1, m_nMinFrame)) ,TRUE);
				}
				else if(m_enDragType == TP_DRAG_TRIMOUTOK)
				{
					if (bShift) m_nTrimIn += iOffset;
					m_nTrimOut += iOffset;
					if (m_nTrimOut-1<m_dStartFrame)	SetStartFrame(static_cast<long double>(max(m_nTrimOut-1, m_nMinFrame)) ,TRUE);
				}
			}
		}
		else if(m_rtTimeCode.right < ptCursor.x)
		{
			if (iRangeRight>0)
			{
				__int64 iOffset = 5;
				iOffset = max(iOffset, -iRangeLeft);
				iOffset = min(iOffset, iRangeRight);

				if(m_enDragType == TP_DRAG_TRIMINOK)
				{
					if (bShift) m_nTrimOut += iOffset;
					m_nTrimIn += iOffset;
					SetStartFrame(static_cast<long double>(min(m_nTrimIn+1, iMaxFrame)) - (m_nMaxFrame -m_nMinFrame)*m_dZoomIn ,TRUE);
				}
				else if(m_enDragType == TP_DRAG_TRIMOUTOK)
				{
					if (bShift) m_nTrimIn += iOffset;
					m_nTrimOut += iOffset;
					SetStartFrame(static_cast<long double>(min(m_nTrimOut+1, iMaxFrame)) - (m_nMaxFrame -m_nMinFrame)*m_dZoomIn ,TRUE);		
				}
			}
		}
		else
		{
			if(m_hTimer) { KillTimer(111); m_hTimer = NULL; }
			Invalidate(NULL);
		}
		if(dStartZoomIn != m_dStartZoomIn)
			m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_STARTIN,(LPARAM)this);
		if(dStartFrame != m_dStartFrame)
			GetTimeLineRgn(TLRGN_CLIENT);
	}
	else if(nIDEvent == 110)
	{
		CPoint ptCursor;//DQA6568 jw 20090427
		::GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor); //取得当前光标位置
		double dStartZoomIn = m_dStartZoomIn;
		double dCurFrame    = m_dCurFrame;
		double dView        = (double)GetViewFrame();
		double dSale        = max(1,dView/8);
		if(m_bXpriTrim&&!m_bXpriTrimSeek) 
		{
			dCurFrame = m_dXpriCurFrame;
			if(m_rtTimeCode.left > ptCursor.x)//XN00019192 20090218
			{
				dCurFrame -= dSale;
				dCurFrame = max(dCurFrame,(double)m_nMinFrame);
			}
			else if(m_rtTimeCode.right - m_iTimeCodeEdge< ptCursor.x) //XN00007248 jyn added 2008.3.18
			{
				dCurFrame += dSale;
				dCurFrame = min(dCurFrame,(double)m_nMaxTLFrame);
			}
			else
			{
				if(m_hTimer){KillTimer(110);	m_hTimer = NULL;}
				Invalidate(NULL);
				m_uBaseState &= ~TP_TIMER_SEEK;
			}
		}
		else
		{
			if(m_enDragType == TP_DRAG_TIMELINEOK) // XN00029022 Wangjin 20101104
			{
				if(m_rtTimeCode.left > ptCursor.x)//XN00019192 20090218
				{
					m_dCurFrame -= dSale;
					m_dCurFrame = max(m_dCurFrame, (long double)m_nMinFrame);
					if(m_dStartFrame > m_dCurFrame)
						SetStartFrame(m_dCurFrame,FALSE);
				}
				else if(m_rtTimeCode.right -m_iTimeCodeEdge < ptCursor.x) //XN00007248 jyn added 2008.3.18
				{
					double dEndFrame = m_dStartFrame + (m_nMaxFrame - m_nMinFrame)*m_dZoomIn;
					m_dCurFrame += dSale;
					m_dCurFrame = min(m_dCurFrame, (double)m_nMaxTLFrame);
					if(dEndFrame < m_dCurFrame)
					{
						double dNewSatrtFrame = m_dCurFrame - (m_nMaxFrame - m_nMinFrame)*m_dZoomIn+1;//+dSale/2; // XN00024466 wangjin 20091112
						//dNewSatrtFrame = min(dNewSatrtFrame, m_nMaxFrame - (m_nMaxFrame - m_nMinFrame)*m_dZoomIn);
						dNewSatrtFrame = max(dNewSatrtFrame,m_nMinFrame);
						SetStartFrame(dNewSatrtFrame, TRUE);
					}
				}
			}
			else
			{
				if(m_hTimer) {KillTimer(110);	m_hTimer = NULL;}
				Invalidate(NULL);
				m_uBaseState &= ~TP_TIMER_SEEK;
			}
		}

		if(m_bXpriTrim&&!m_bXpriTrimSeek)//Trim
		{
			m_uBaseState &= ~TP_TIMER_SEEK;
			if(dCurFrame != m_dXpriCurFrame)
			{
				if(m_enDragType == TP_DRAG_TIMELINEOK)
				{
					m_dXpriCurFrame = dCurFrame;
					m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMING,(LPARAM)this); 
				}
			}
		}
		else
		{
			if(dCurFrame != m_dCurFrame)
			{
				m_uBaseState &= ~TP_TIMER_SEEK;
				SetCurFrame(m_dCurFrame,FALSE);
				Invalidate(NULL);
				if(m_enDragType == TP_DRAG_TIMELINEOK)
				{
					m_uBaseState |= TP_TIMER_SEEK;
					m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEKING,(LPARAM)this);
				}
			}
		}

		if(dStartZoomIn != m_dStartZoomIn)
			m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_STARTIN,(LPARAM)this);
		if(dStartFrame != m_dStartFrame)	GetTimeLineRgn(TLRGN_CLIENT);
	}
	else if(nIDEvent == 112) //XN00020464 jw 20090724
	{
		CPoint ptCursor;//DQA6568 jw 20090427
		::GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor); //取得当前光标位置
		double dStartZoomIn = m_dStartZoomIn;
		//double dScrub    = m_dAudioScrub;
		double dCurFrame    = m_dCurFrame;
		double dView        = (double)GetViewFrame();
		double dSale        = max(1,dView/8);
		if(*m_pbAudioScrub)
		{
			if(m_rtTimeCode.left > ptCursor.x)//XN00019192 20090218
			{
				dCurFrame -= dSale;
				dCurFrame = max(dCurFrame, (long double)m_nMinFrame);
				if(m_dStartFrame > dCurFrame)
					SetStartFrame(dCurFrame,TRUE);
			}
			else if(m_rtTimeCode.right  -m_iTimeCodeEdge < ptCursor.x) //XN00007248 jyn added 2008.3.18
			{
				double dEndFrame = m_dStartFrame + (m_nMaxFrame - m_nMinFrame)*m_dZoomIn;
				dCurFrame += dSale;
				dCurFrame = min(dCurFrame, (double)m_nMaxTLFrame);				
				if(dEndFrame < dCurFrame)
					SetStartFrame(dCurFrame - (m_nMaxFrame - m_nMinFrame)*m_dZoomIn+dSale/2,TRUE);
			}
			else
			{
				if(m_hTimer) {KillTimer(112); m_hTimer = NULL;}
				Invalidate(NULL);
			}
			if(dStartZoomIn != m_dStartZoomIn)
				m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_STARTIN,(LPARAM)this);
			if(dStartFrame != m_dStartFrame)
				GetTimeLineRgn(TLRGN_CLIENT);
		}
		else
		{
			if(m_hTimer) {KillTimer(112);m_hTimer = NULL;}
			Invalidate(NULL);
		}
	}
	else if(nIDEvent == 120)
	{
		m_hTimerScroll = 0;
		KillTimer(120);
		m_bZooming = FALSE;
		Invalidate();
		if(m_bUpdateWindow) UpdateWindow();
		m_bZooming = TRUE;
	}
	CWnd::OnTimer(nIDEvent);
}
void   CTPTimeLine::SetMaxFrame(__int64 iMax,BOOL bReset,BOOL bPaint, DWORD dwFlags)
{
	return SetMaxMinFrame(m_nMinFrame,iMax,bReset,bPaint, dwFlags);
}
void   CTPTimeLine::SetMaxMinFrame(__int64 iMin,__int64 iMax,BOOL bReset,BOOL bPaint, DWORD dwFlags)
{
#ifdef TP_NS_NET
	if(m_nMaxFrame == iMax && m_nMinFrame == iMin && m_nSafeLength==0) return;
#else
	if(m_nMaxFrame==iMax && m_nMinFrame==iMin) return;
#endif
	iMax = max(iMax, iMin +1);

#ifdef TP_NS_NET
	if (dwFlags & FLAG_RESET_MAXLENGTH) m_nMaxFrame = 1;
	m_nActualMaxFrame = iMax;
	m_nMinFrame = iMin;
	m_nMaxFrame   = max(m_nMaxFrame, m_nActualMaxFrame - m_nSafeLength );
#else
	m_nMaxFrame = iMax;
	m_nMinFrame = iMin;
#endif

	double dCurPixel = TranslateFrame(m_dCurFrame);
	GetMiniZoomIn();
	m_nMaxTLFrame = m_nMaxFrame;
	if(bReset) SetZoomIn(1,TRUE,bPaint);
#ifdef TP_NS_NET
	else SetZoomIn(m_dZoomIn, TRUE, bPaint, dCurPixel);
#else
	else SetZoomIn(m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dPixelPerFrame), TRUE, bPaint, dCurPixel);
#endif
}
void  CTPTimeLine::SetCurFrame(int dCurFrame,BOOL bReset ,BOOL bPaint, DWORD dwFlags)
{
	SetCurFrame((long double)dCurFrame,bReset,bPaint,dwFlags);
}
void  CTPTimeLine::SetCurFrame(__int64 dCurFrame,BOOL bReset ,BOOL bPaint, DWORD dwFlags)
{
	SetCurFrame((long double)dCurFrame,bReset,bPaint,dwFlags);
}
void  CTPTimeLine::SetCurFrame(long double dCurFrame,BOOL bReset ,BOOL bPaint, DWORD dwFlags)
{
	if(m_uBaseState & TP_TIMER_SEEK) return;
	if(m_bAutoRise && dCurFrame>=m_nMaxFrame-1) 
	{
		INT64 lAddLength = (INT64)((m_nMaxFrame - m_nMinFrame)*0.1);
		SetMaxMinFrame(m_nMinFrame,m_nMaxFrame+lAddLength,FALSE,FALSE);//cx1672 jw 20100705
		//SetMaxMinFrame(m_nMinFrame,__int64(m_nMaxFrame *1.1),FALSE,FALSE);
		if(bPaint) Invalidate();
	}
#ifdef TP_NS_NET
	if (dwFlags&FLAG_CANLENGTH_EXTEND && dCurFrame>=m_nMaxFrame )
	{
		dCurFrame  = min(dCurFrame,m_nActualMaxFrame-1);
		m_nMaxTLFrame = m_nMaxFrame = TP_DoubleToInt64(dCurFrame) + 1;		
		SetZoomIn(m_dZoomIn,TRUE,bPaint); //播放时保持固定的缩放比例不变
		Invalidate();
	}
#else
	if ((dwFlags&FLAG_CANLENGTH_EXTEND) && dCurFrame>=m_nMaxFrame-1 && m_nMaxFrame <m_nActualMaxFrame && m_nActualMaxFrame>0)
	{
		m_nMaxFrame      = min((int)dCurFrame + 2,m_nActualMaxFrame-1);
		m_nMaxTLFrame    = m_nMaxFrame + 1;		
		SetZoomIn(m_dZoomIn,FALSE,FALSE);
		bPaint = TRUE;
	}
#endif
	dCurFrame  = max(dCurFrame,m_nMinFrame);
	if(!m_bAutoRise)	  dCurFrame  = min(dCurFrame,m_nMaxTLFrame);
	else    	          dCurFrame  = min(dCurFrame,m_nMaxFrame);
	if(dCurFrame == m_dCurFrame && m_dSaveCurFrame == dCurFrame && !(m_uBaseState & TP_TLWND_FORCESETCURFRAME)) return;//DQA5217 jw 20090421
	long double dSaveFrame = m_dCurFrame;
	m_dCurFrame = dCurFrame;
	CRect  rtDraw = CRect(0,0,0,0);
	if(bReset && m_bResetStart)
	{
		double dShowFrame = (m_iTimeCodeWidth -m_iTimeCodeEdge)/ m_dPixelPerFrame;
		if(m_bPlay)
		{
			if(m_dCurFrame <m_dStartFrame || m_dCurFrame > m_dStartFrame + dShowFrame * m_dChangePage && m_dZoomIn <1.0)
			{
				if(m_dCurFrame <m_dStartFrame) SetStartFrame(max(m_dCurFrame - dShowFrame * 0.5,0),TRUE);
				else                           SetStartFrame(min(m_dCurFrame - dShowFrame * 0.5,m_nMaxFrame - dShowFrame),TRUE);
				GetClientRect(&rtDraw);
			}
		}
		else if(m_enDragType != TP_DRAG_TIMELINE && m_enDragType != TP_DRAG_TIMELINEOK)
		{	
			if(m_dCurFrame <m_dStartFrame || m_dCurFrame > m_dStartFrame + dShowFrame* m_dPageTurn && m_dZoomIn <1.0)
			{
				if(m_dCurFrame <m_dStartFrame) SetStartFrame(max(m_dCurFrame - dShowFrame * (1-m_dPageTurn),0),TRUE);
				else                           SetStartFrame(min(m_dCurFrame - dShowFrame * m_dPageTurn,m_nMaxFrame - dShowFrame),TRUE);

				m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);
				GetClientRect(&rtDraw);
			}
		}
	}
	if( rtDraw.Width()<=0 && bPaint) 
	{
		rtDraw = GetTimeLineRect();
		rtDraw.left  = TP_DoubleToInt(TranslateFrame(dSaveFrame))-16;
		rtDraw.right = rtDraw.left +32;
		rtDraw |= CRect(TP_DoubleToInt(TranslateFrame(m_dCurFrame))-16,rtDraw.top,TP_DoubleToInt(TranslateFrame(m_dCurFrame))+16,rtDraw.bottom);
		rtDraw.InflateRect(m_iTimeLineWidthEx,0);
	}
	GetTimeLineRgn(TLRGN_LINE);	
	if(bPaint)
	{
		rtDraw.top -=3;
		InvalidateRect(rtDraw);
		if(m_bUpdateWindow) UpdateWindow();
	}
}

void CTPTimeLine::SetZoomIn(double dZoomIn,BOOL bCenter,BOOL bPaint,double dCurPixel)
{
	if(m_bForceZoom)
	{
		m_dZoomIn        = 1.0;
		m_dStartZoomIn   = 0.0;
		m_dPixelPerFrame = m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);
		m_dStartFrame    = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;
		return;
	}

	if(dZoomIn < 0) return;
	if(bCenter && dCurPixel ==0.0) dCurPixel = TranslateFrame(m_dCurFrame);

	m_dZoomIn = dZoomIn;
	m_dZoomIn = max(m_dMinZoomIn, m_dZoomIn);
	m_dZoomIn = min(1.0, m_dZoomIn);

	if(m_dSaveZoomIn != 0.0 || m_dZoomIn !=1.0)	m_bSaveClick = FALSE;

	if(!bCenter)
	{
		if(m_dStartZoomIn >1-m_dZoomIn)  m_dStartZoomIn = 1-m_dZoomIn;
		if(m_dStartZoomIn <0)            m_dStartZoomIn = 0;

		m_dPixelPerFrame =  m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);
		m_dStartFrame = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;
	}
	else
	{
		double dCurFrame = m_dCurFrame;
		if( dCurPixel > m_rtTimeCode.right || dCurPixel < m_rtTimeCode.left)
		{
			dCurPixel = (m_rtTimeCode.left + m_rtTimeCode .right)/2.0;
			dCurFrame = TranslatePixel(dCurPixel);
		}

		m_dPixelPerFrame = m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);

		m_dStartZoomIn =  (dCurFrame - (dCurPixel - m_rtTimeCode.left-m_iOffsetPixel) / m_dPixelPerFrame - m_nMinFrame )/(m_nMaxFrame -m_nMinFrame); 
		if(m_dStartZoomIn >1-m_dZoomIn) m_dStartZoomIn = 1-m_dZoomIn;
		if(m_dStartZoomIn <0)           m_dStartZoomIn = 0;
		m_dStartFrame = (m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame;
	}

#ifdef TP_NS_NET
	if(bPaint && (m_dSaveStartZoomIn != m_dStartZoomIn || m_dSaveZoomIn != m_dZoomIn || m_dZoomIn == 1.0))
	{
		m_bForceDraw = TRUE;
		Invalidate();
	}
#else
	if(bPaint && (m_dSaveStartZoomIn != m_dStartZoomIn || m_dSaveZoomIn != m_dZoomIn))
	{
		m_bForceDraw = TRUE;
		Invalidate();
	}
#endif
	m_dSaveStartZoomIn = m_dStartZoomIn;
	m_dSaveZoomIn      = m_dZoomIn;
	GetTimeLineRgn(TLRGN_LINE);	
}

void CTPTimeLine::SetVirtualFrame(INT64 dCurFrame, BOOL bReset, BOOL bPaint)//DQA5217 jw 20090421
{
	//Trim时的翻页
	dCurFrame  = max(dCurFrame,m_nMinFrame);
	dCurFrame  = min(dCurFrame,m_nMaxTLFrame);
	CRect  rtDraw = CRect(0,0,0,0);

	if(bReset)
	{
		double dShowFrame = (m_nMaxFrame-m_nMinFrame)*m_dZoomIn;
		if(m_bPlay)
		{
			if(dCurFrame <m_dStartFrame || dCurFrame > m_dStartFrame + dShowFrame * m_dChangePage && m_dZoomIn <1.0)
			{
				if(dCurFrame <m_dStartFrame)
					SetStartFrame(max(dCurFrame - dShowFrame * 0.5,0),TRUE);
				else
					SetStartFrame(min(dCurFrame - dShowFrame * 0.5,m_nMaxFrame - dShowFrame),TRUE);
				GetClientRect(&rtDraw);
			}
		}
		else
		{
			if(dCurFrame <m_dStartFrame || dCurFrame > m_dStartFrame + dShowFrame && m_dZoomIn <1.0)
			{
				if(dCurFrame <m_dStartFrame)
				{
					SetStartFrame(max(dCurFrame - dShowFrame * (1-m_dPageTurn),0),TRUE);
				}
				else
					SetStartFrame(min(dCurFrame - dShowFrame * m_dPageTurn,m_nMaxFrame - dShowFrame),TRUE);
				GetClientRect(&rtDraw);
			}
		}
	}
	GetTimeLineRgn(TLRGN_LINE);	
	if(rtDraw.Width()>0 && bPaint)
	{
		rtDraw.top -=3;
		InvalidateRect(rtDraw);
		if(m_bUpdateWindow) UpdateWindow();
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_STARTIN,(LPARAM)this);
	}
}

double  CTPTimeLine::GetSecution(CPoint point,double dCurFrame,UINT uBaseType)
{
	UINT uState = 0;
	if(TP_IsCtrlKeyDown()) uState |= TP_CTRL;
	if(TP_IsAltKeyDown())  uState |= TP_ALT;
	if(m_uSuctionState != uState && GetSuctionFrame) {m_aSuctionFrame.RemoveAll();GetSuctionFrame(this,uState|uBaseType);}	
	m_uSuctionState = uState;

	if(m_bXpriTrim && (uState & TP_CTRL) && !(uState & TP_ALT) && !m_bXpriTrimSeek)
	{
		dCurFrame = m_dCurFrame + point.x - TranslateFrame(m_dCurFrame);
		dCurFrame= max(dCurFrame, m_nMinFrame);
		dCurFrame= min(dCurFrame, m_nMaxFrame);
		m_rtDrag.left = point.x;
	}
	else
	{
		if(m_uSuctionState !=0 && m_aSuctionFrame.GetSize()>0)
		{
			INT l=0;
			for(l=0;l<m_aSuctionFrame.GetSize();l++)
			{
				if(m_aSuctionFrame[l] ==dCurFrame) return dCurFrame;
				if(m_aSuctionFrame[l]>dCurFrame) break;
			}
			if(l<m_aSuctionFrame.GetSize())
			{
				if(point.x > (TranslateFrame(m_aSuctionFrame[l]) + TranslateFrame((l >0 ?m_aSuctionFrame[l-1]:0)))/2) dCurFrame  = (double)m_aSuctionFrame[l];
				else                                                                                                  dCurFrame  = (double)(l >0 ?m_aSuctionFrame[l-1]:0);
			}	
			else return (double)m_aSuctionFrame[m_aSuctionFrame.GetSize()-1];
		}
	}
	return dCurFrame;
}
void CTPTimeLine::OnMouseMove(UINT nFlags, CPoint point) 
{
	//	CWnd::OnMouseMove(nFlags, point);
	CPoint ptSave = point;
 	if(GetKeyState(VK_LBUTTON)>=0) nFlags &=~MK_LBUTTON;
	if((nFlags & MK_RBUTTON) == MK_RBUTTON)
	{
		//if(m_enDragType != TP_DRAG_UNKNOW) TP_ClipPoint(m_rtCursorArea,point);

		if(m_enDragType == TP_DRAG_RECT || m_enDragType == TP_DRAG_RECTOK)
		{
			if(m_ptDrag != point)
			{
				if(m_enDragType == TP_DRAG_RECT)
				{
					m_enDragType = TP_DRAG_RECTOK;
					m_uBaseState |= TP_DRAG_CAPTURE;
					SetCapture();
				}
				if(TP_IsCtrlKeyDown())
				{
					BOOL bZoomX = ZoomX(CPoint(m_rtDrag.left,m_rtDrag.right),point),bPaint = FALSE;

					//横向移动
					m_iCommandType = TIME_CMD_ZOOMIN;
					if(bZoomX)
					{
						double dStartFrame = m_dStartFrame;
						//dStartFrame += -TP_DoubleToInt(TranslatePixel(point.x)) + m_rtDrag.top;
						dStartFrame += TP_DoubleToInt(TranslatePixel(m_ptDrag.x))-TP_DoubleToInt(TranslatePixel(point.x));
						dStartFrame  = max(GetMinFrame() ,dStartFrame);
						dStartFrame  = min(GetMaxFrame()-GetViewFrame(),dStartFrame);
						if(TP_DoubleToInt(dStartFrame) != TP_DoubleToInt(m_dStartFrame))
						{
							bPaint = TRUE;
							SetStartFrame(dStartFrame,FALSE);
							m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,TIME_CHANGE_STARTIN,(LPARAM)this);
						}
					}
					else
					{
						//纵向移动
						double dZoomIn = m_dZoomIn;
						dZoomIn += (point.y - m_ptDrag.y)*1.0/m_rtTimeCode.Height() * m_dZoomIn;
						dZoomIn  = min(dZoomIn,1.0);
						dZoomIn  = max(dZoomIn,m_dMinZoomIn);
						if(dZoomIn != m_dZoomIn)
						{
							bPaint = TRUE;
							CTPTimeLine::SetZoomIn(dZoomIn,TRUE,FALSE);
							m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,TIME_CHANGE_ZOOMIN,(LPARAM)this);
						}
					}
					m_iCommandType = 0;
					if(bPaint) Invalidate();
					m_ptDrag = point;
				}
			}
		}
	}
	else if((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		if(m_enDragType != TP_DRAG_UNKNOW) TP_ClipPoint(m_rtCursorArea,point);
		if(m_enDragType == TP_DRAG_LEFTOK || m_enDragType == TP_DRAG_LEFT)
		{
			if(m_ptDrag.x != point.x && m_rtRightHead.Width()>0)
			{
				m_enDragType = TP_DRAG_LEFTOK;
				SetZoomIn((m_rtRightHead.right - (point.x  - m_iDrag))*1.0/m_iTimeCodeWidth,TRUE,TRUE);
				m_ptDrag = point;
				m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);
				if(m_hTimerScroll ==0) m_hTimerScroll = SetTimer(120,500,NULL);
			}
		}
		else if(m_enDragType == TP_DRAG_RIGHTOK || m_enDragType == TP_DRAG_RIGHT)
		{ 
			if(m_ptDrag.x != point.x && m_rtRightHead.Width()>0)
			{
				m_enDragType = TP_DRAG_RIGHTOK;
				SetZoomIn((point.x  - m_iDrag + m_rtRightHead.Width() - m_rtLeftHead.left)*1.0/m_iTimeCodeWidth,TRUE,TRUE);
				m_ptDrag = point;
				m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);
				if(m_hTimerScroll ==0) m_hTimerScroll = SetTimer(120,500,NULL);
			}
		}
		else if(m_enDragType == TP_DRAG_BAROK || m_enDragType == TP_DRAG_BAR)
		{ 
			if(m_ptDrag.x != point.x)
			{
				if(m_enDragType == TP_DRAG_BAR)
				{
					SetCursor(m_pResCursor[TL_HANDLE] ->GetCursor()); 
					m_enDragType = TP_DRAG_BAROK;
				}
				SetStartZoomIn((point.x  - m_iDrag - m_rtTimeCode.left)*1.0/m_iTimeCodeWidth,TRUE);
				m_ptDrag = point;
				m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_ZOOMIN,(LPARAM)this);
				if(m_hTimerScroll ==0) m_hTimerScroll = SetTimer(120,500,NULL);
			}
		}
		else if(m_enDragType == TP_DRAG_TRIMOUT ||m_enDragType == TP_DRAG_TRIMOUTOK)
		{
			if(m_ptDrag.x != point.x) 
			{
				m_ptDrag = point;
				m_enDragType = TP_DRAG_TRIMOUTOK;
				
				BOOL bShift = TP_IsShiftKeyDown();
				__int64 iMaxFrame = m_nMaxFrame; // DQAdb00008978 wangjin 20100118
				//if(m_iTimeCodeType == 2) iMaxFrame = m_nMinFrame + TP_GetFrameFormTime(12,0,0,0,m_enVideoType)-1;

				__int64 nMarkMin = m_nTrimOut-1;
				__int64 nMarkMax = m_nTrimOut;
				if (bShift && m_nTrimIn>=0)
				{
					nMarkMin	= min(m_nTrimIn, m_nTrimOut-1);
					nMarkMax	= max(m_nTrimIn, m_nTrimOut);
				}

				__int64 iRangeLeft	= nMarkMin  - m_nMinFrame; // XN00024528 wangjin 20100118
				__int64 iRangeRight	= iMaxFrame - nMarkMax;
				ASSERT(iRangeLeft>=0 && iRangeRight>=0);
				if(point.x < m_rtTimeCode.left || m_rtTimeCode.right < point.x)
				{
					if ((point.x<m_rtTimeCode.left && iRangeLeft>0) || (m_rtTimeCode.right<point.x && iRangeRight>0) && !m_hTimer && m_dZoomIn <1.0) 
						m_hTimer=SetTimer(111,20,NULL);
				}
				else
				{
					if(m_hTimer) { KillTimer(111); m_hTimer = NULL; }

					__int64 iOffset = TP_DoubleToInt64(TranslatePixel(point.x))-m_nTrimOut;
					iOffset = max(iOffset, -iRangeLeft);
					iOffset = min(iOffset, iRangeRight);

					if (bShift && m_nTrimIn>=0) m_nTrimIn  += iOffset;
					m_nTrimOut += iOffset;

					if (iOffset != 0)
					{
						Invalidate();
						m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMOUTING,(LPARAM)this); 
					}
				}
				if(m_hTimerScroll ==0) m_hTimerScroll = SetTimer(120,500,NULL);
			}
		}
		else if(m_enDragType == TP_DRAG_TRIMIN || m_enDragType == TP_DRAG_TRIMINOK )
		{
			if(m_ptDrag.x != point.x) 
			{
				m_ptDrag = point;
				m_enDragType = TP_DRAG_TRIMINOK;

				BOOL bShift = TP_IsShiftKeyDown();
				__int64 iMaxFrame = m_nMaxFrame; // DQAdb00008978 wangjin 20100118
				//if(m_iTimeCodeType == 2) iMaxFrame = m_nMinFrame + TP_GetFrameFormTime(12,0,0,0,m_enVideoType)-1;

				__int64 nMarkMin = m_nTrimIn;
				__int64 nMarkMax = m_nTrimIn;
				if (bShift && m_nTrimOut>=0)
				{
					nMarkMin = min(m_nTrimIn, m_nTrimOut-1);
					nMarkMax = max(m_nTrimIn, m_nTrimOut);
				}

				__int64 iRangeLeft	= nMarkMin  - m_nMinFrame; // XN00024528 wangjin 20100118
				__int64 iRangeRight	= iMaxFrame - nMarkMax;
				ASSERT(iRangeLeft>=0 && iRangeRight>=0);
				if(point.x < m_rtTimeCode.left || m_rtTimeCode.right < point.x)
				{
					if ((point.x<m_rtTimeCode.left && iRangeLeft>0) || (m_rtTimeCode.right<point.x && iRangeRight>0) && !m_hTimer && m_dZoomIn <1.0) 
						m_hTimer=SetTimer(111,20,NULL);
				}
				else
				{
					if(m_hTimer) { KillTimer(111); m_hTimer = NULL; }

					__int64 iOffset = TP_DoubleToInt64(TranslatePixel(point.x))-m_nTrimIn;
					iOffset = max(iOffset, -iRangeLeft);
					iOffset = min(iOffset, iRangeRight);

					if(bShift && m_nTrimOut>=0) m_nTrimOut  += iOffset;
					m_nTrimIn += iOffset;

					if (iOffset != 0)
					{
						Invalidate();
						m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMINING,(LPARAM)this); 
					}
				}
				if(m_hTimerScroll ==0) m_hTimerScroll = SetTimer(120,500,NULL);
			}
		}
		else if(m_enDragType == TP_DRAG_TIMELINE || m_enDragType == TP_DRAG_TIMELINEOK)
		{
			UINT uState = 0;
			if(TP_IsCtrlKeyDown()) uState |= TP_CTRL;
			if(TP_IsAltKeyDown())  uState |= TP_ALT;

			if(m_ptDrag.x != point.x || m_uSuctionState != uState) 
			{
				if(m_enDragType == TP_DRAG_TIMELINE)
				{
					m_enDragType = TP_DRAG_TIMELINEOK;
					SetCursor(m_pResCursor[TL_EW] ->GetCursor());
					if(!m_bXpriTrim || m_bXpriTrimSeek) m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEKBEGIN,(LPARAM)this); 	
				}
				if(m_rtTimeCode.left > point.x || m_rtTimeCode.right-m_iTimeCodeEdge < point.x)//XN00007248 jyn added 3.18
				{
					if(*m_pbAudioScrub)
					{
						double dAudioScrub = m_dAudioScrub;
						if(m_rtTimeCode.left > point.x) m_dAudioScrub = 0.0; 
						else                            m_dAudioScrub = (double)m_nMaxTLFrame;						
						if(dAudioScrub != m_dAudioScrub)//XN00020464 jw 20090724
						{
							if(!m_bXpriTrim || m_bXpriTrimSeek)
								m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEKING,(LPARAM)this);
							else
								m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMING,(LPARAM)this);
						}
						if(!m_hTimer)
							m_hTimer = SetTimer(112, 100, NULL);
					}
					else
					{
						if(!m_hTimer)
							m_hTimer=SetTimer(110,100,NULL);
					}
				}
				else
				{
					if(m_hTimer) 
					{
						KillTimer(111); 
						KillTimer(110); 
						KillTimer(112);
						m_hTimer = NULL; 
						Invalidate();
					}
					m_uBaseState &= ~TP_TIMER_SEEK;
					double dCurFrame = TP_DoubleToInt(TranslatePixel(point.x));
					if(m_bXpriTrim&&!m_bXpriTrimSeek && dCurFrame <= 0 && point.x <=m_rtTimeCode.left)	dCurFrame =-10;
					double dCurFrameSave = dCurFrame;
					dCurFrame = GetSecution(point,dCurFrame);
					if(!m_bXpriTrim||m_bXpriTrimSeek)
					{
						dCurFrame = min(dCurFrame,m_nMaxTLFrame);
						dCurFrame = max(dCurFrame,m_nMinFrame);
					}
					else if(m_bXpriTrim)
					{
						dCurFrame = min(dCurFrame,m_nMaxFrame);
						dCurFrame = max(dCurFrame,m_nMinFrame);
					}					
					if((!m_bXpriTrim||m_bXpriTrimSeek) && TP_DoubleToInt64(m_dCurFrame) != TP_DoubleToInt64(dCurFrame)  ||
						m_bXpriTrim && !m_bXpriTrimSeek && TP_DoubleToInt64(m_dXpriCurFrame) != TP_DoubleToInt64(dCurFrame))
					{
						if(!m_bXpriTrim || m_bXpriTrimSeek) 
						{
							if(*m_pbAudioScrub) m_dAudioScrub = dCurFrame;
							else  		 	  SetCurFrame((long double)TP_DoubleToInt64(dCurFrame),TRUE,TRUE);
							m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEKING,(LPARAM)this); 
						}
						else
						{
							m_dXpriCurFrame = dCurFrame;
							m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_TRIMING,(LPARAM)this); 
						}
						ViewCurFrame();
					}
					m_ptDrag = point;

					if(m_uSuctionState !=0 && (!m_bXpriTrim||m_bXpriTrimSeek))
					{
						if(!m_bXpriTrim||m_bXpriTrimSeek)  dCurFrame = m_dCurFrame;
						else              dCurFrame = m_dXpriCurFrame;
						if(dCurFrame == m_dSecutionFrame)    return;
						m_dSecutionFrame = dCurFrame;
						const double nMinPresion = 0.001;
						if((m_uSuctionState & TP_ALT)==TP_ALT && abs(dCurFrameSave-dCurFrame)>nMinPresion)
						{
							CPoint pt = point;
							GetCursorPos(&pt); // snp4100028852 Wangjin 20101015
							ScreenToClient(&pt);
							pt.x = (int)TranslateFrame((long double)dCurFrame);
							pt.y =  ptSave.y;
							m_ptDrag = pt;
							ClientToScreen(&pt);
							if (!m_pbAudioScrub || !*m_pbAudioScrub)	SetCursorPos(pt.x,pt.y);
						}
					}
				}
			}
		}
		else if(m_enDragType >= TP_DRAG_SCENEIN && m_enDragType <= TP_DRAG_SCENEINOUTOK)
		{
			if(m_ptDrag.x != point.x) 
			{
				if(m_enDragType == TP_DRAG_SCENEIN)
				{
					m_enDragType = TP_DRAG_SCENEINOK;
					SetCursor(m_pResCursor[TL_EW] ->GetCursor()); 
				}
				else if(m_enDragType == TP_DRAG_SCENEOUT)
				{
					m_enDragType = TP_DRAG_SCENEOUTOK;
					SetCursor(m_pResCursor[TL_EW] ->GetCursor()); 
				}
				else if(m_enDragType == TP_DRAG_SCENEINOUT)
				{
					m_enDragType = TP_DRAG_SCENEINOUTOK;
					SetCursor(m_pResCursor[TL_HANDLE] ->GetCursor()); 
				}

				m_ptDrag = point;
				if(m_rtTimeCode.left > point.x || m_rtTimeCode.right -m_iTimeCodeEdge< point.x)
				{
					//if(m_hTimer==NULL)
					//	m_hTimer=SetTimer(110,100,NULL);
				}
				else
				{
					if(m_hTimer) { KillTimer(110); m_hTimer = NULL;}

					m_uBaseState &= ~TP_TIMER_SEEK;
					INT64 lCurFrame = (INT64)TranslatePixel(point.x);
					int   iIndex    = (int)m_iDrag;
					if(m_enDragType == TP_DRAG_SCENEINOK)
					{
						lCurFrame = min(lCurFrame,m_arrBlock[iIndex].lOut-1);
						lCurFrame = max(lCurFrame,m_nMinFrame);
						m_arrBlock[iIndex].lIn = (INT64)lCurFrame;
					}
					else if(m_enDragType == TP_DRAG_SCENEOUTOK)
					{
						lCurFrame = max(lCurFrame,m_arrBlock[iIndex].lIn+1);
						lCurFrame = min(lCurFrame,m_nMaxFrame);
						m_arrBlock[iIndex].lOut = (INT64)lCurFrame;
					}
					if(m_enDragType == TP_DRAG_SCENEINOUTOK)
					{
						INT64 iDistance = lCurFrame - m_rtDrag.top;
						if(iDistance!=0)
						{
							for(INT l=0;l<m_arrBlock.GetSize();l++)
							{
								if(!m_arrBlock[l].bSelect) continue;
								if(iDistance<=0) iDistance = max(iDistance,m_nMinFrame - m_arrBlock[l].lIn);
								else             iDistance = min(iDistance,m_nMaxFrame - m_arrBlock[l].lOut);
							}

							for(INT l=0;l<m_arrBlock.GetSize();l++)
							{
								if(!m_arrBlock[l].bSelect) continue;
								m_arrBlock[l].lIn  += iDistance;
								m_arrBlock[l].lOut += iDistance;
							}
							m_rtDrag.top = (long)lCurFrame;
						}
					}
					Invalidate();
				}
			}
		}
		else if(m_enDragType == TP_DRAG_MARKPOINT || m_enDragType == TP_DRAG_MARKPOINTOK)
		{
			if(m_ptDrag.x != point.x) 
			{
				if(m_enDragType == TP_DRAG_MARKPOINT)
				{
					m_enDragType = TP_DRAG_MARKPOINTOK;
					SetCursor(m_pResCursor[TL_EW] ->GetCursor());
				}
				m_ptDrag = point;
				if(m_rtTimeCode.left > point.x || m_rtTimeCode.right -m_iTimeCodeEdge< point.x)
				{
					//if(m_hTimer==NULL)
					//	m_hTimer=SetTimer(110,100,NULL);
				}
				else
				{
					if(m_hTimer){ KillTimer(110); m_hTimer = NULL;}

					m_uBaseState &= ~TP_TIMER_SEEK;
					long dCurFrame = (long)TranslatePixel(point.x);
					if(m_rtDrag.left != dCurFrame && m_nMinFrame <= dCurFrame && dCurFrame <= m_nMaxTLFrame)//DQA8977 jw 20100111
					{
						BYTE cTypeTmp = 0;
						for(INT l=0;l<m_arrMarkPoint.GetSize();l++)
						{
							if(m_rtDrag.left == m_arrMarkPoint[l].lFrame)
							{
								cTypeTmp = m_arrMarkPoint[l].cType;
								break;
							}
						}
						for(INT l=0;l<m_arrMarkPoint.GetSize();l++)
						{
							if(m_arrMarkPoint[l].lFrame == dCurFrame)
								return;
						}
						DelMarkPoint(m_rtDrag.left,FALSE);
						if(AddMarkPoint(dCurFrame,TRUE)>=0)
							m_rtDrag.left  = dCurFrame;
						else
							m_rtDrag.left = -1;
						BYTE cType = 0;
						for(INT l=0;l<m_arrMarkPoint.GetSize();l++)
						{
							if(dCurFrame == m_arrMarkPoint[l].lFrame)
							{
								m_arrMarkPoint[l].cType = cTypeTmp;
								break;
							}
						}
						m_rtDrag.right = dCurFrame;
					}
				}
			}
		}
	}
	else
	{
		if(m_enDragType == TP_DRAG_RECT || m_enDragType == TP_DRAG_RECTOK)
			m_enDragType = TP_DRAG_UNKNOW;
	}
}
void CTPTimeLine::SetShuttleActivate(BOOL bAct)
{
	if(GetShuttleSpeed && bAct)
	{
		long   lState   = 0;
		double dShuttle = GetShuttleSpeed(lState);
		if(lState == 1)
		{
			m_iShuttleIndex = 9;
			m_iShuttleState = 0;
		}
	}
}

BOOL CTPTimeLine::SpecialCallback(int iCmd,LPARAM  lParam)
{
	return TRUE;
}

void CTPTimeLine::OnMButtonDblClk( UINT nFlags, CPoint point)
{
	OnMButtonDown(nFlags, point);
}
void CTPTimeLine::OnMButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bShuttleState) return;
	if(GetFocus()!=this) SetFocus();
	m_iShuttleIndex = 9;
	int iJogShuttleState = 0;

	if(m_pMsgWnd)
	{
		BOOL bShuttle = FALSE;
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_JOGSHUTTLEGETSTATE,(LPARAM)&bShuttle); 
		if(bShuttle) iJogShuttleState=0;//状态取反
		else         iJogShuttleState=1;
		m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_JOGSHUTTLESTATE,(LPARAM)iJogShuttleState); 
	}
	else
	{
		iJogShuttleState = m_iShuttleState;
		iJogShuttleState = (iJogShuttleState+1)%2;
	}
	
	m_iShuttleState = iJogShuttleState;
	__super::OnMButtonDown(nFlags, point);
}

BOOL CTPTimeLine::OnMouseWheel(MSG *pMsg)
{
	if(pMsg->hwnd == m_hWnd) return FALSE;
	CRect  rtWnd;
	CPoint ptCursor = CPoint(LOWORD(pMsg ->lParam),HIWORD(pMsg ->lParam));
	GetWindowRect(&rtWnd);
 	if(rtWnd.PtInRect(ptCursor))
	{
		OnMouseWheel(LOWORD(pMsg ->wParam),HIWORD(pMsg ->wParam),ptCursor);
		return TRUE;
	}
	return FALSE;
}
BOOL CTPTimeLine::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(m_enDragType != TP_DRAG_UNKNOW) return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	if(m_iShuttleState == 0)
	{
NextSeek:
		double dCurFrame = m_dCurFrame;
		if(zDelta<0) dCurFrame ++;
		else         dCurFrame --;
		dCurFrame = min(dCurFrame,m_nMaxTLFrame);
		dCurFrame = max(dCurFrame,m_nMinFrame);
		if(dCurFrame != m_dCurFrame)
		{
			BOOL   bAudioScrub = *m_pbAudioScrub;
			*m_pbAudioScrub = FALSE;
			SetCurFrame((long double)TP_DoubleToInt64(dCurFrame),TRUE,TRUE);
			m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_SEEKING,(LPARAM)this);
			*m_pbAudioScrub = bAudioScrub;
		}
	}
	else
	{
		double dSpeed[] = {-16,-8,-4,-2,-1,-1.0/2,-1.0/4,-1.0/8,-1.0/16 ,0, 1.0/16, 1.0/8, 1.0/4, 1.0/2,1,2,4,8,16};
		int    iSpeed   = sizeof(dSpeed)/sizeof(double);

		if(m_bPassedZero)
		{
			m_iShuttleIndex = 9;
		}
		else if(GetShuttleSpeed)
		{
			long   lState   = 0;
			BOOL   bShuttle = FALSE;
			m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_JOGSHUTTLEGETSTATE,(LPARAM)&bShuttle); 
			double dShuttle = GetShuttleSpeed(lState);
			//TRACE(TEXT("\n ^^^ ^^^ the dShuttle is : %f \n"),dShuttle );
			if(!bShuttle) {m_iShuttleState = 0;goto NextSeek;}
			if(dShuttle>=-32 && dShuttle<=32)
			{
				m_iShuttleIndex = iSpeed-1;
				for(INT l = 0;l<iSpeed-1;l++)
				{
					if(dShuttle ==dSpeed[l]) 
					{
						m_iShuttleIndex = l;
						break;
					}
				}
			}
			else m_iShuttleIndex = 9;
		}

		int iShuttleIndex = m_iShuttleIndex;

		if(zDelta<0)
			m_iShuttleIndex++;
		else
			m_iShuttleIndex--;
		m_iShuttleIndex = max(0,m_iShuttleIndex);
		m_iShuttleIndex = min( iSpeed- 1 ,m_iShuttleIndex);

		if(iShuttleIndex != m_iShuttleIndex)
			m_pMsgWnd->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_JOGSHUTTLEDIRECTION,(LPARAM)&dSpeed[m_iShuttleIndex]); 
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CTPTimeLine::OnPaint()
{
	CPaintDC dc(this);
	PaintDC(&dc);
}
int CTPTimeLine::AddSceneInOut(__int64 iIn,__int64 iOut,BOOL bPaint)
{
	if(bPaint) Invalidate();
	TPTimeLineScence stuAdd(iIn,iOut);
	for(INT l=0;l<m_arrBlock.GetSize();l++)
	{
		if(m_arrBlock[l].lIn>=iIn)
		{
			if(m_arrBlock[l].lOut == iOut) return l;
			else if(m_arrBlock[l].lOut < iOut)
			{
				m_arrBlock.InsertAt(l+1,stuAdd);
				return l+1;
			}
			else
			{
				m_arrBlock.InsertAt(l,stuAdd);
				return l;
			}
		}
	}
	m_arrBlock.InsertAt(0,stuAdd);
	return 0;
}
int CTPTimeLine::DelSceneInOut(__int64 iIn,__int64 iOut,BOOL bPaint)
{
	return 0;
}

int CTPTimeLine::AddMarkPoint(__int64 iFrame,BOOL bPaint)
{
	int iIndex = 0;
	if(m_arrMarkPoint.GetSize()<=0)          m_arrMarkPoint.Add(TPTimeLineMark(iFrame));
	else if(m_arrMarkPoint[0].lFrame>iFrame) m_arrMarkPoint.InsertAt(0,TPTimeLineMark(iFrame));
	else if(m_arrMarkPoint[m_arrMarkPoint.GetSize()-1].lFrame<iFrame) 
	{
		m_arrMarkPoint.Add(TPTimeLineMark(iFrame));
		iIndex = (long)m_arrMarkPoint.GetSize()-1;
	}
	else
	{
		for(INT l=0;l<m_arrMarkPoint.GetSize();l++)
		{
			if(iFrame == m_arrMarkPoint[l].lFrame) 
				return -(l+1);
			else if(iFrame <m_arrMarkPoint[l].lFrame)
			{
				iIndex = l;
				m_arrMarkPoint.InsertAt(l,TPTimeLineMark(iFrame));
				break;
			}
		}
	}
	if(bPaint)
	{
		m_bForceDraw = TRUE;
		InvalidateRect(m_rtTimeCode,TRUE);
	}
	return iIndex;
}
int CTPTimeLine::DelMarkPoint(__int64 iFrame,BOOL bPaint)
{
	int iIndex = -1;
	for(INT l=0;l<m_arrMarkPoint.GetSize();l++)
	{
		if(iFrame == m_arrMarkPoint[l].lFrame)
		{
			iIndex = l;
			m_arrMarkPoint.RemoveAt(l);
			break;
		}
	}
	if(bPaint && iIndex >=0)
	{
		m_bForceDraw = TRUE;
		InvalidateRect(m_rtTimeCode,TRUE);
	}
	return iIndex;
}
TL_RES_INDEX CTPTimeLine::GetMarkIndex(int iIndex)
{
	switch(iIndex)
	{
	case 1: return TL_RES_MARKPOINT1;
	case 2: return TL_RES_MARKPOINT2;
	case 3: return TL_RES_MARKPOINT3;
	case 4: return TL_RES_MARKPOINT4;
	case 5: return TL_RES_MARKPOINT5;
	default:return TL_RES_MARKPOINT;
	}
}
COLORREF CTPTimeLine::GetMarkColor(int iIndex)
{
	switch(iIndex)
	{
	case 1: return RGB(161,192,233);
	case 2: return RGB(237,80,80);
	case 3: return RGB(170,223,87);
	case 4: return RGB(182,140,255);
	case 5: return RGB(242,238,129);
	default:
		return RGB(255,128,64);
	}
}

void CTPTimeLine::GetTimeLineRgn(UINT uRgn)
{

	if(!m_bUseRgn) return;
	if(uRgn & TLRGN_CLIENT)
	{
		if(m_hTimeLineRgn) DeleteObject(m_hTimeLineRgn);
		m_hTimeLineRgn = NULL;
		m_iTimeLineRgnPos = 0;

#ifdef  TP_NS_EDIT
		m_hTimeLineRgn = CreateRectRgn(m_rtTimeLine.left+1,m_rtTimeLine.top,1,m_rtTimeLine.Height());
#else
		CDC *pDC = GetDC();
		CTPMemoDC cdcMemo(pDC,32,m_rtTimeLine.Height());
		cdcMemo.FillSolidRect(0,0,32,m_rtTimeLine.Height(),RGB(0,0,0));
		//画头部
		cdcMemo.DrawIcon(CPoint(1,2),m_pResBar[TL_RES_TIMELINE] ->GetIcon());
		CPen *pSavePen = (CPen *) cdcMemo.SelectObject(CPen::FromHandle(m_pResPen[TL_PEN_TLLINE] ->hPen));
		cdcMemo.MoveTo(16,12);
		cdcMemo.LineTo(16,m_rtTimeLine.bottom);	
		cdcMemo.MoveTo(17,12);
		cdcMemo.LineTo(17,m_rtTimeLine.bottom);	
		cdcMemo.SelectObject(pSavePen);
		TP_BitmapToRgn(m_hTimeLineRgn,(HBITMAP)cdcMemo.GetBitmap() ->GetSafeHandle(),RGB(0,0,0),&cdcMemo);
		ReleaseDC(pDC);
#endif
		if(m_hClientRgn) DeleteObject(m_hClientRgn);
		m_hClientRgn = CreateRectRgn(0,0,m_rtClient.Width(),m_rtClient.Height());
	}
	if(uRgn & TLRGN_LINE)
	{
		__int64  iPos = TP_DoubleToInt64(TranslateFrame(m_dCurFrame) -17);
		if(iPos != m_iTimeLineRgnPos)
		{
			OffsetRgn(m_hTimeLineRgn,int(iPos -m_iTimeLineRgnPos),0);
			m_iTimeLineRgnPos = iPos;
		}
	}
	if(m_hInvertRgn) DeleteObject(m_hInvertRgn);
	m_hInvertRgn  = CreateRectRgn(0,0,m_rtClient.Width(),m_rtClient.Height());
	if(m_dCurFrame >=m_dStartFrame)	CombineRgn(m_hInvertRgn,m_hClientRgn,m_hTimeLineRgn,RGN_DIFF);
}
void CTPTimeLine::ViewCurFrame()
{
	double dStart        = TP_DoubleToInt(m_dStartFrame);
	double dEnd          = TP_DoubleToInt(m_dStartFrame + GetViewFrame());
	double dStartZoomIn  = m_dStartZoomIn;
	if(m_dCurFrame<dStart)       SetStartFrame(m_dCurFrame,TRUE);
	else if(m_dCurFrame>=dEnd)   SetStartFrame(max(m_nMinFrame, m_dCurFrame-GetViewFrame()+1),TRUE);
	else return;
	if(dStartZoomIn != m_dStartZoomIn)
		m_pMsgWnd ->SendMessage(WM_TP_TIMELINE,(WPARAM)TIME_CHANGE_STARTIN,(LPARAM)this);
}
void CTPTimeLine::SetSize( int cx, int cy)
{
	m_rtClient   = CRect(0,0,cx,cy);
	m_rtBarTotal = GetTimeBarRect();
	m_rtTimeCode = GetTimeCodeRect();
	m_rtTimeLine = GetTimeLineRect();
	m_rtTimeBar  = m_rtBarTotal;

	if(m_nMaxFrame <=m_nMinFrame) return;

	m_dPixelPerFrame = m_iTimeCodeWidth/((m_nMaxFrame -m_nMinFrame)*m_dZoomIn);

	GetMiniZoomIn();

	if(m_pShowTips)	m_pShowTips->SetToolRect(this,20000,m_rtTimeCode|m_rtBarTotal);
	GetTimeLineRgn(TLRGN_CLIENT|TLRGN_LINE);
}
void CTPTimeLine::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	SetSize(cx,cy);

	GetTimeCodeRect();
	if (m_iTimeCodeWidth>0 && 0<=m_nMinFrame && m_nMinFrame<m_nMaxFrame && m_nMinFrame<=m_dStartFrame && m_dStartFrame<m_nMaxFrame)
	{
		GetMiniZoomIn();
		SetZoomIn(m_dZoomIn, FALSE, TRUE, TranslateFrame(m_dCurFrame));
		SetStartZoomIn((m_dStartFrame-m_nMinFrame)*1.0/(m_nMaxFrame -m_nMinFrame), TRUE);
	}
}

BOOL CTPTimeLine::SetZoomInOut(BOOL bIn,long double lCenterPos)
{
	double dScale[SCALE_COUNT+2] = {1.0};
	INT l=0;
	for(l=1;l<SCALE_COUNT+1;l++) dScale[l] = dScale[l-1]/1.7;
	double dMax = dScale[0];
	for(l=0;l<SCALE_COUNT+1;l++)
	{
		dScale[l] = (1.0-m_dMinZoomIn) *(dScale[l]-dScale[SCALE_COUNT])/dMax + m_dMinZoomIn;
	}
	dScale[0]  = 1.0;
	dScale[SCALE_COUNT] = m_dMinZoomIn;
	double iScale = 1.0;

	if(bIn)
	{
		for(l=0;l<SCALE_COUNT;l++)
		{
			if(dScale[l]>=m_dZoomIn && dScale[l+1]<m_dZoomIn) break;
		}
		if(l>=SCALE_COUNT) return FALSE;
		iScale = dScale[l+1];//m_dZoomIn -0.05;
		iScale =max(m_dMinZoomIn,iScale);
	}
	else
	{
		for(l=0;l<SCALE_COUNT+1;l++)
		{
			if(dScale[l]>=m_dZoomIn && dScale[l+1]<m_dZoomIn) break;
		}
		if(l==0) iScale = dScale[0];
		else     iScale = dScale[l-1];
		iScale = min(1.0,iScale);
	}

	if(iScale == m_dZoomIn) return FALSE;
	long double dCurFrame = m_dCurFrame;
	if(lCenterPos>=0) m_dCurFrame = lCenterPos;
	CTPTimeLine::SetZoomIn(iScale,TRUE,TRUE);
	m_dCurFrame = dCurFrame;
	if(lCenterPos>=0) GetTimeLineRgn(TLRGN_LINE);
	return TRUE;
}

int TP_GetSubStep(VIDEO_TYPE eVideoType,int iParentStep)
{
	int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0,dwDay = 0;
	TP_GetTimeFormFrame((DWORD)iParentStep,dwHour,dwMinute,dwSecond,dwFrame,eVideoType);
	if(dwHour==1)        {dwHour = 0;  dwMinute = 60;}
	else if(dwMinute==1) {dwMinute = 0;dwSecond = 60;}
	else if(dwSecond==1) {dwSecond = 0;dwFrame = eVideoType == VIDEO_25?25:30;}
	if(dwHour>0)
	{
		for(INT l=9;l>=1;l--)
		{
			if(dwHour %l==0)  {dwHour = dwHour/l;break;}
		}
		return  TP_GetFrameFormTime(dwHour,0,0,0,eVideoType);
	}
	else if(dwMinute>0)
	{
		int iSec[4] = {1,5,10,15};
		for(INT l=2;l>=0;l--)
		{
			if(dwMinute == iSec[l]) continue;
			if(dwMinute %iSec[l]==0)  {dwMinute = iSec[l];break;}
		}
		if(dwMinute == 1 &&  dwSecond == 0 && dwFrame == 0) return iParentStep/5;
		return  TP_GetFrameFormTime(0,dwMinute,0,0,eVideoType);
	}
	else if(dwSecond>0)
	{
		int iSec[4] = {1,5,10,15};
		for(INT l=2;l>=0;l--)
		{
			if(dwSecond == iSec[l]) continue;
			else if(dwSecond %iSec[l]==0) {dwSecond = iSec[l];break;}
		}
		if(dwSecond == 1 &&  dwFrame == 0) return iParentStep/5;
		return  TP_GetFrameFormTime(0,0,dwSecond,0,eVideoType);
	}
	else  if(dwFrame>0)
	{
		int iSec[3] = {1,5,10};
		for(INT l=(eVideoType ==VIDEO_25?1:2);l>=0;l--)
		{
			if(dwFrame == iSec[l]) continue;
			else if(dwFrame %iSec[l]==0) {dwFrame = iSec[l];break;}
		}
		return  TP_GetFrameFormTime(0,0,0,dwFrame,eVideoType);
	}
	else
		return 1;
}
int    TP_GetParentStep(VIDEO_TYPE eVideoType,int iParentStep,double &dSample)
{
	int i1  = TP_GetFrameFormTime(0,0,1,0,eVideoType);
	dSample = iParentStep;
	if(iParentStep<=i1) return iParentStep;
	int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0,dwDay = 0;
	TP_GetTimeFormFrame((DWORD)iParentStep,dwHour,dwMinute,dwSecond,dwFrame,eVideoType);
	if(dwFrame>0)    {dwSecond ++; dwFrame  = 0;}
	if(dwSecond>30) {dwMinute ++; dwSecond = 0;}
	if(dwMinute>30) {dwHour ++;   dwMinute = 0;}
	if(dwHour >0)
	{
		dwMinute = dwSecond = dwFrame = 0;
	}
	else if(dwMinute>0)
	{
		dwSecond = dwFrame = 0;
		if(dwMinute<=1)        dwMinute = 1;
		else if(dwMinute<=5)   dwMinute = 5;
		else if(dwMinute<=10)  dwMinute = 10;
		else if(dwMinute<=15)  dwMinute = 15;
		else if(dwMinute<=30)  dwMinute = 30;
		else                   dwMinute = 60;
	}
	else if(dwSecond>0)
	{
		dwFrame = 0;
		if(dwSecond<=1)        dwSecond = 1;
		else if(dwSecond<=5)   dwSecond = 5;
		else if(dwSecond<=10)  dwSecond = 10;
		else if(dwSecond<=15)  dwSecond = 15;
		else if(dwSecond<=30)  dwSecond = 30;
		else                   dwSecond = 60;
	}
	dSample = TP_GetFrameFormTime(dwHour,dwMinute,dwSecond,dwFrame,eVideoType == VIDEO_25?VIDEO_25:VIDEO_30);
	return TP_GetFrameFormTime(dwHour,dwMinute,dwSecond,dwFrame,eVideoType);
}

double TP_GetInterFrameEx(VIDEO_TYPE eVideoType,double dFrame,int iParentStep)
{
	//int i1    = TP_GetFrameFormTime(0,0,1,0,eVideoType);
	//int iDay  = 0;
	//if(eVideoType == VIDEO_25)	        iDay  = (int)dFrame/(24*FR_HOUR_PAL);
	//else if(eVideoType == VIDEO_30)  	iDay  = (int)dFrame/(24*FR_HOUR_ND);
	//else if(eVideoType == VIDEO_30M)	iDay  = (int)dFrame/(24*FR_HOUR_DROP);

	//int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0,dwDay = 0;
	//TP_GetTimeFormFrame((DWORD)dFrame,dwHour,dwMinute,dwSecond,dwFrame,eVideoType);
	//if(iParentStep>=i1)
	//{
	//	if(dwFrame>15)
	//		dwSecond ++;
	//	dwFrame = 0;
	//}
	//else if(iParentStep>=5)
	//	dwFrame = dwFrame/5*5;
	//else
	//	dwFrame = dwFrame/iParentStep*iParentStep;
	//dwHour += iDay * 24;
	//return (double)TP_GetFrameFormTime(dwHour,dwMinute,dwSecond,dwFrame,eVideoType);
	return 0;
}
double TP_GetInterFrame(VIDEO_TYPE eVideoType,double dFrame,int iParentStep)
{
	/*int i1    = TP_GetFrameFormTime(0,0,1,0,eVideoType);
	int iDay  = 0;
	if(eVideoType == VIDEO_25)	        iDay  = (int)dFrame/(24*FR_HOUR_PAL);
	else if(eVideoType == VIDEO_30)  	iDay  = (int)dFrame/(24*FR_HOUR_ND);
	else if(eVideoType == VIDEO_30M)	iDay  = (int)dFrame/(24*FR_HOUR_DROP);	
		
	int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0,dwDay = 0;
	TP_GetTimeFormFrame((DWORD)dFrame,dwHour,dwMinute,dwSecond,dwFrame,eVideoType);
	if(iParentStep>=i1)
		dwFrame = 0;
	else if(iParentStep>=5)
		dwFrame = dwFrame/5*5;
	else
		dwFrame = dwFrame/iParentStep*iParentStep;
	dwHour += iDay * 24;
	return (double)TP_GetFrameFormTime(dwHour,dwMinute,dwSecond,dwFrame,eVideoType);*/
	return 0;
}

void    TP_GetTimeFormFrameEx(VIDEO_TYPE eVideoType,double dFrame,int *iTime)
{
	//int iDay = (int)dFrame/(24*FR_HOUR_DROP);
	//TP_GetTimeFormFrame((DWORD)dFrame,iTime[0],iTime[1],iTime[2],iTime[3],eVideoType);
	//iTime[0] += iDay*24;
	//if(iTime[0]>0||iTime[1]>0||iTime[2]>0) iTime[3]=0;
	return ;
}

double    TP_AddTime(VIDEO_TYPE eVideoType,int *iTimeFrame, int *iTimeStep)
{
	for(INT l=0;l<4;l++) iTimeFrame[l] += iTimeStep[l];
	if(iTimeFrame[3]>30){ iTimeFrame[2] += iTimeFrame[3]/30;iTimeFrame[3] = iTimeFrame[3] %30;}
	if(iTimeFrame[2]>=60){ iTimeFrame[1] += iTimeFrame[2]/60;iTimeFrame[2] = iTimeFrame[2] %60;}
	if(iTimeFrame[1]>=60){ iTimeFrame[0] += iTimeFrame[1]/60;iTimeFrame[1] = iTimeFrame[1] %60;}
	return (double)TP_GetFrameFormTime(iTimeFrame[0],iTimeFrame[1],iTimeFrame[2],iTimeFrame[3],eVideoType);
}

double TP_GetStartFrameExEx(VIDEO_TYPE eVideoType,double dFrame,int iParentStep,double dSample)
{
	dFrame = TP_DoubleToInt(dFrame);
	if(iParentStep ==1) return dFrame;
	if(eVideoType != VIDEO_30M)
	{
		dFrame = (((int)dFrame/(int)dSample) -2) *(int)dSample;
		dFrame = max(0,dFrame);
		return dFrame;
	}
	else
	{
		dFrame -= iParentStep*2;
		if(dFrame<=0) return 0;

		int iTimeFrame[4],iTimeStep[4];
		TP_GetTimeFormFrameEx(eVideoType,dFrame,iTimeFrame);
		TP_GetTimeFormFrameEx(eVideoType,iParentStep,iTimeStep);
		if(iTimeStep[0]>0)     { iTimeFrame[0] = iTimeFrame[0]/iTimeStep[0] *iTimeStep[0];iTimeFrame[1]=iTimeFrame[2]=iTimeFrame[3]=0;}
		else if(iTimeStep[1]>0){ iTimeFrame[1] = iTimeFrame[1]/iTimeStep[1] *iTimeStep[1];iTimeFrame[2]=iTimeFrame[3]=0;}
		else if(iTimeStep[2]>0){ iTimeFrame[2] = iTimeFrame[2]/iTimeStep[2] *iTimeStep[2];iTimeFrame[3]=0;}
		else if(iTimeStep[3]>0){ iTimeFrame[3] = iTimeFrame[3]/iTimeStep[3] *iTimeStep[3];}
		return (double)TP_GetFrameFormTime(iTimeFrame[0],iTimeFrame[1],iTimeFrame[2],iTimeFrame[3],eVideoType);
	}
}
double TP_GetStartFrameEx(VIDEO_TYPE eVideoType,double dFrame,int iParentStep,double dSample)
{
	if(iParentStep ==1) return dFrame;

	int iDay = 0;
	if(eVideoType == VIDEO_25)	        iDay  = TP_GetFrameFormTime(23,59,59,24,eVideoType)+1;
	else if(eVideoType == VIDEO_30)  	iDay  = TP_GetFrameFormTime(23,59,59,29,eVideoType)+1;
	else if(eVideoType == VIDEO_30M)	iDay  = TP_GetFrameFormTime(23,59,59,29,eVideoType)+1;

	if(dFrame>iDay/2 && dFrame<iDay)
	{
		double dStartFrame = iDay;
		while(dStartFrame>=dFrame-dSample*2)
		{
			dStartFrame -= iParentStep;
		}
		if(eVideoType == VIDEO_30M)
			dStartFrame = TP_GetInterFrameEx(eVideoType,dStartFrame,iParentStep);
		return dStartFrame;
	}
	else
	{
		double dStartFrame = 0;
		if(dFrame>=iDay)  dStartFrame = iDay;
		while(dStartFrame<dFrame-dSample*2)
		{
			dStartFrame += iParentStep;
		}
		if(eVideoType == VIDEO_30M)
			dStartFrame = TP_GetInterFrameEx(eVideoType,dStartFrame,iParentStep);
		return dStartFrame;
	}
}

double TP_GetStartFrame(VIDEO_TYPE eVideoType,double dFrame,int iParentStep)
{
	//if(iParentStep ==1) return dFrame;

	//int iDay  = 0;
	//if(eVideoType == VIDEO_25)       iDay  = FR_HOUR_PAL*24;
	//else if(eVideoType == VIDEO_30)  iDay  = FR_HOUR_ND*24;
	//else if(eVideoType == VIDEO_30M) iDay  = FR_HOUR_DROP*24;

	//double dStartFrameEx = 0;
	//int    iIncrese      = eVideoType == VIDEO_30M ? 10 : 1;
	//if(dFrame>iDay/2)
	//{
	//	int dwHour=0,dwMinute=0;
	//	for(dwHour = 23;dwHour>=0;dwHour--)
	//	{
	//		dStartFrameEx = TP_GetFrameFormTime(dwHour,dwMinute,0,0,eVideoType);
	//		if(dStartFrameEx<=dFrame) return dStartFrameEx;
	//	}
	//}
	//else
	//{
	//	int dwHour=0,dwMinute=0;
	//	DWORD dwHourSave=0, dwMinuteSave=0;
	//	for(dwHour = 0;dwHour<=23;dwHour++)
	//	{
	//		dStartFrameEx = TP_GetFrameFormTime(dwHour,dwMinute,0,0,eVideoType);
	//		if(dStartFrameEx==dFrame)      return dStartFrameEx;
	//		else if(dStartFrameEx>dFrame)  return TP_GetFrameFormTime(dwHourSave,dwMinuteSave,0,0,eVideoType);
	//		dwHourSave	 = dwHour;
	//		dwMinuteSave = dwMinute;
	//	}
	//}
	//return dFrame;
	return 0;
}
BOOL   TP_GetTimeFormNTSCDrop( int Frame,DWORD eVtrFormat )
{
	//DWORD	dwReste, hour, minute;
	//if(eVtrFormat == VIDEO_30M)
	//{
	//	hour = (Frame/FR_HOUR_DROP);
	//	dwReste = (Frame%FR_HOUR_DROP);
	//	minute = 10*(dwReste/FR_TENMIN_DROP);
	//	dwReste = (dwReste%FR_TENMIN_DROP);
	//	if( dwReste >= FR_MINUTE_ND )
	//	{
	//		dwReste  -= FR_MINUTE_ND;
	//		minute += 1 + (dwReste / FR_MINUTE_DROP);
	//		dwReste %= FR_MINUTE_DROP;
	//		dwReste  += 2;
	//		return dwReste == 2 ?TRUE:FALSE;
	//	}
	//}
	//else
	//	ASSERT(0);
	return FALSE;
}
void CTPTimeLine::PaintTimeCodeLine2(CDC *pDC,double dStartFrame,double dEndFrame ,CRect &rectCode)
{
	int    iParentStep     = 1,iSubStep = 1;   //每大格的帧数
	int    iPixelTemp      = 0,iSubPixelTemp = 0,iSubPixelRight=-1;
	int    iPixStartFrame  = (int)TranslateFrame(m_nMinFrame);
	int    iDay            = 0;
	double dStartFramePre  = -1;
	int    iTimeFrame[4],iTimeStep[4];
	CSize  szText          = pDC ->GetTextExtent(_T("23:59:59:23"));

	//if(m_enVideoType == VIDEO_25)      iDay = FR_HOUR_PAL  *24;
	//else if(m_enVideoType == VIDEO_30) iDay = FR_HOUR_ND   *24;
	//if(m_enVideoType == VIDEO_30M)     iDay = FR_HOUR_DROP *24;

	szText.cx = szText.cx/2+2;;
	if(m_dSaveState[0] != m_dPixelPerFrame)
	{
		INT64 nHourFrame = TP_GetFrameFormTime(1,0,0,0, m_enVideoType);
		if (dEndFrame-dStartFrame >= nHourFrame*24)
		{
			INT64 nHour		= static_cast<INT64>(dEndFrame-dStartFrame)/nHourFrame;
			m_dSaveState[5] = static_cast<double>((max(nHour/25,1))*nHourFrame);
			m_dSaveState[3] = m_dSaveState[4] = m_dSaveState[5]*5;
			iParentStep = (int)m_dSaveState[3];
			iSubStep    = (int)m_dSaveState[5];
		}
		else
		{
			iParentStep = max(1,TP_DoubleToInt(150/m_dPixelPerFrame));  //每大格宽度不能小于 150 个象素	
			if(iParentStep <=1)       iParentStep  = 1;
			else if(iParentStep <5)   iParentStep  = 5;
			else if(iParentStep >= 5) iParentStep  = iParentStep /5*5;  //每大格最好为5的整数倍		
			if(m_enVideoType == VIDEO_25 &&  iParentStep >= 25)  iParentStep = ((iParentStep-1)/25 + 1)*25; //每大格最好为每秒的整数倍
			else if(iParentStep >= 30)                           iParentStep = ((iParentStep-1)/30 + 1)*30;
			iParentStep = max(iParentStep,1);
			iParentStep = TP_GetParentStep(m_enVideoType,iParentStep,m_dSaveState[4]);
			if(m_enVideoType == VIDEO_25)
			{
				if(iParentStep<25 && iParentStep>5)
				{
					iParentStep = 25;
					m_dSaveState[4] = iParentStep;
				}
			}
			else
			{
				if(iParentStep<30 && iParentStep>15)
				{
					iParentStep = 30;m_dSaveState[4] = iParentStep;
				}
			}
			m_dSaveState[3] = iParentStep;
			iSubStep = TP_GetSubStep(m_enVideoType,iParentStep);
			m_dSaveState[5] = iSubStep;
		}
	}
	else
	{
		iParentStep = (int)m_dSaveState[3];
		iSubStep    = (int)m_dSaveState[5];
	}

	if(m_dSaveState[0] != m_dPixelPerFrame || m_dSaveState[1] != m_dStartFrame)
	{
		dStartFrame = TP_GetStartFrameExEx(m_enVideoType,dStartFrame,iParentStep,m_dSaveState[4]);
		m_dSaveState[0] = m_dPixelPerFrame;
		m_dSaveState[1] = m_dStartFrame;
		m_dSaveState[2] = -1;
	}
	else
		dStartFrame = m_dSaveState[2];

	if(iPixStartFrame>rectCode.left) rectCode.left += iPixStartFrame-rectCode.left;
	CRect rtTemp = rectCode,rtText;
	rtTemp.DeflateRect(0,0,0,2);

	if(m_enVideoType == VIDEO_30M && iParentStep>1)
	{
		TP_GetTimeFormFrameEx(m_enVideoType,dStartFrame,iTimeFrame);
		TP_GetTimeFormFrameEx(m_enVideoType,iParentStep,iTimeStep);
	}

	/*if(m_dSaveState[2]==-1 && m_enVideoType == VIDEO_30M)
	{
		int iDay =  TP_GetFrameFormTime(23,59,59,29,m_enVideoType)+1;
		if(dStartFrame<iDay && dEndFrame>iDay)
		{
			double dStartFrameSave = dStartFrame; 
			while(dStartFrameSave <dEndFrame+iParentStep)
			{
				dStartFrameSave   += iParentStep;
				if(m_enVideoType == VIDEO_30M && iParentStep>1)
					dStartFrameSave = TP_GetInterFrame(m_enVideoType,dStartFrameSave,iParentStep);	
				if(dStartFrameSave>iDay) break;
			}
			dStartFrame -= dStartFrameSave - iDay;
		}
	}*/

	while(dStartFrame <dEndFrame+iParentStep)
	{
		if(dStartFrame>=m_nMinFrame-iParentStep)
		{
			iPixelTemp = TP_DoubleToInt(TranslateFrame((long  double)dStartFrame)) +1;
			rtTemp.left =   iPixelTemp  -szText.cx;
			rtTemp.right =  iPixelTemp  +szText.cx;
			if(iPixelTemp  >rectCode.right+500)	
				break;
			else if(iPixelTemp >=rectCode.left-szText.cx)
			{
				if(m_dSaveState[2]==-1) m_dSaveState[2] = dStartFramePre == -1?dStartFrame:dStartFramePre;//可能是重新校验过的。
				if(m_iTimeCodeType ==1)
				{
					pDC->MoveTo(iPixelTemp,rtTemp.top + 8);
					pDC->LineTo(iPixelTemp,rtTemp.top +1);
					rtText = rtTemp;  rtText.top = rtTemp.top + 8;
				}
				else if(m_iTimeCodeType == 3)
				{
					pDC->MoveTo(iPixelTemp,rtTemp.top    + 5);
					pDC->LineTo(iPixelTemp,rtTemp.bottom - 1);
					rtText = rtTemp;  rtText.top = rtTemp.top + 5;
				}
				else
				{
					pDC->MoveTo(iPixelTemp,rtTemp.bottom - 11); 
					pDC->LineTo(iPixelTemp,rtTemp.bottom - 1 );
					rtText = rtTemp;  rtText.top = rtTemp.top + 1; rtText.bottom = rtTemp.bottom - 12;
				}
				if(m_iTimeCodeType == 2)
				{
					int iFrame = (int)dStartFrame;
					if(iFrame<0) {if(m_enVideoType == VIDEO_25) iFrame+= 24*3600*25;else  iFrame+= 24*3600*30;}	
					if(rtText.left + szText.cx/2 <rectCode.right && iSubPixelRight < rtText.left)
					{
						pDC->DrawText(TP_StringFromFrame(m_enVideoType,iFrame,FALSE),rtText,DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
						iSubPixelRight = rtText.right;
					}
				}

				if(dStartFramePre>=0 && iSubStep >=1)
				{
					if(m_enVideoType == VIDEO_30M && TP_GetTimeFormNTSCDrop((int)dStartFramePre,m_enVideoType)) dStartFramePre-=2;
					for(int k = int(dStartFramePre + iSubStep);k<int(dStartFrame-(iSubStep>1?1:0));k+=iSubStep)
					{
						iSubPixelTemp = TP_DoubleToInt(TranslateFrame((long double)k))+1;
						if(iSubPixelTemp <rectCode.left) continue;
						if(iSubPixelTemp  >rectCode.right)
							break;
						if(m_iTimeCodeType ==1)
						{
							pDC->MoveTo(TP_DoubleToInt(iSubPixelTemp),rtTemp.top + 4);
							pDC->LineTo(TP_DoubleToInt(iSubPixelTemp),rtTemp.top +1);
						}
						else if(m_iTimeCodeType == 3)
						{
							pDC->MoveTo(TP_DoubleToInt(iSubPixelTemp),rtTemp.top + 9);
							pDC->LineTo(TP_DoubleToInt(iSubPixelTemp),rtTemp.bottom-1);
						}
						else
						{
							pDC->MoveTo(TP_DoubleToInt(iSubPixelTemp),rtTemp.bottom - 7);
							pDC->LineTo(TP_DoubleToInt(iSubPixelTemp),rtTemp.bottom-1);
						}
					}
				}
			}
		}
		dStartFramePre = dStartFrame;
		if(m_enVideoType == VIDEO_30M && iParentStep>1)
			dStartFrame = TP_AddTime(m_enVideoType,iTimeFrame,iTimeStep);
		else
			dStartFrame += iParentStep;
	}
}
void CTPTimeLine::PaintTimeSubIndecator(CDC *pDC,double dStartFrame,double dEndFrame)
{
	if(m_nIndecatorIn>=0 && m_nIndecatorIn<m_nIndecatorOut && m_nIndecatorOut + m_nMinFrame>dStartFrame && m_nIndecatorIn+ m_nMinFrame<dEndFrame)
	{
		CRect rtTemp   = m_rtTimeCode;
		rtTemp.left    = TP_DoubleToInt(TranslateFrame(m_nIndecatorIn+ m_nMinFrame));
		rtTemp.right   = TP_DoubleToInt(TranslateFrame(m_nIndecatorOut+ m_nMinFrame));
		rtTemp        &= m_rtTimeCode;
		pDC->FillSolidRect(rtTemp,RGB(0x1B,0x37,0x4B));
	}
}

void CTPTimeLine::PaintTimeSceneInOut(CDC *pDC,double dStartFrame,double dEndFrame,CRect &rectCode)
{
	if(!m_bSceneInout) return;

	CRect rtTemp    = rectCode;
	int   iPixTemp1 = 0,iPixTemp2 = 0;
	rtTemp.top      = rtTemp.top + 6;
	rtTemp.bottom   = rtTemp.top + 10;
	rtTemp.right	+=  25;
	m_szSceneInOut = CSize(rtTemp.bottom,rtTemp.bottom);
	
	if(m_arrBlock.GetSize()>0)
	{
		CTPMemoDCEx *pMemDC = NULL;
		BOOL         bSel   = FALSE;

		m_szSceneInOut = CSize(rtTemp.top,rtTemp.bottom);

		BLENDFUNCTION blf;
		blf.AlphaFormat = 0;
		blf.BlendFlags = 0;
		blf.BlendOp = 0;
		blf.SourceConstantAlpha = 150;

		CRect rtInout = CRect(0,0,rtTemp.Width(),rtTemp.Height());
		for(INT l=0;l<m_arrBlock.GetSize();l++)
		{
			if(m_arrBlock[l].lOut<=dStartFrame || m_arrBlock[l].lIn >=dEndFrame) continue;
			if(pMemDC == NULL)
				pMemDC = new CTPMemoDCEx(pDC,NULL,rtTemp.Width()+2,rtTemp.Height());

			iPixTemp1 = TP_DoubleToInt(TranslateFrame(m_arrBlock[l].lIn));
			iPixTemp2 = TP_DoubleToInt(TranslateFrame(m_arrBlock[l].lOut)) - iPixTemp1;
			if(iPixTemp2<=0) continue;

			rtInout.left  = 0;
			rtInout.right = iPixTemp2;

			if(rtInout.right > rtTemp.Width())
			{
				if(iPixTemp1 >= 0)
					rtInout.right = min(rtInout.right,rtTemp.Width());
				else
				{
					if(iPixTemp1 + iPixTemp2 < rtTemp.Width())
					{
						iPixTemp1 += (rtInout.right - rtTemp.Width());
						rtInout.right = rtTemp.Width();
					}
					else
					{
						iPixTemp1 = 0;
						rtInout.right = rtTemp.Width();
					}
				}
			}

			if(m_arrBlock[l].bSelect)
			{
				pMemDC->FillSolidRect(0,0,rtTemp.Width(),rtTemp.Height(),RGB(255,255,255));
				CRect rtTemp = rtInout;
				rtTemp.left = rtInout.left + 1;
				rtTemp.right = rtInout.right - 1;
				rtTemp.top = rtInout.top + 1;
				rtTemp.bottom = rtInout.bottom - 1;
				pMemDC->FillSolidRect(rtTemp,GetMarkColor(m_arrBlock[l].cType));
				pMemDC->DrawFocusRect(rtInout);
			}
			else
			{
				pMemDC->FillSolidRect(0,0,rtTemp.Width(),rtTemp.Height(),RGB(0,0,0));
				pMemDC->FillSolidRect(rtInout,GetMarkColor(m_arrBlock[l].cType));
			}

			AlphaBlend(pDC ->m_hDC, iPixTemp1, rtTemp.top,rtInout.Width()+1, rtInout.Height(),
				pMemDC->m_hDC, 0,0,rtInout.Width()+1, rtInout.Height(), blf);
		}
		if(pMemDC)
		{
			pMemDC->DeleteDC();
			delete pMemDC;
		}
	}
	if(m_lSceneIn>=0 && m_lSceneIn>=dStartFrame && m_lSceneIn<=dEndFrame)
	{
		iPixTemp1  = TP_DoubleToInt(TranslateFrame(m_lSceneIn));
		if(iPixTemp1>rectCode.left && iPixTemp1 <rectCode.right)
		{
			HPEN hPen = (HPEN)pDC ->SelectObject(m_pResPen[TL_PEN_SCENE] ->hPen);
			pDC ->MoveTo(iPixTemp1,rtTemp.top);
			pDC ->LineTo(iPixTemp1,rtTemp.bottom-1);
			pDC ->LineTo(iPixTemp1-2,rtTemp.bottom-1);
			pDC ->SelectObject(hPen);
		}
	}
	else if(m_lSceneOut>=0 && m_lSceneOut>=dStartFrame && m_lSceneOut<=dEndFrame)
	{
		iPixTemp1  = TP_DoubleToInt(TranslateFrame(m_lSceneOut));
		if(iPixTemp1>rectCode.left && iPixTemp1 <rectCode.right)
		{
			HPEN hPen = (HPEN)pDC ->SelectObject(m_pResPen[TL_PEN_SCENE] ->hPen);
			pDC ->MoveTo(iPixTemp1,rtTemp.top);
			pDC ->LineTo(iPixTemp1,rtTemp.bottom-1);
			pDC ->LineTo(iPixTemp1+2,rtTemp.bottom-1);
			pDC ->SelectObject(hPen);
		}
	}
}
void CTPTimeLine::PaintTimeCode(CDC *pDC)
{
	CRect rectCode,rtTemp,rtText;
	m_rtTimeCode       = GetTimeCodeRect();
	rectCode           = m_rtTimeCode;
	double dStartFrame = m_dStartFrame;
	double dEndFrame   = min(m_dStartFrame + m_iTimeCodeWidth / m_dPixelPerFrame,m_nMaxFrame);

	//画底色
	if(m_iTimeCodeBackColor>=0)  pDC->FillSolidRect(rectCode,TP_GetSysColor(m_iTimeCodeBackColor)); 
	else if(m_bWndFocus)       	 pDC->FillSolidRect(rectCode,TP_GetSysColor(COLOR_CODE_BACK)); 
	else if(m_bWndEnable)        pDC->FillSolidRect(rectCode,TP_GetSysColor(COLOR_CODE_BACK)); 
	else           	     	     pDC->FillSolidRect(rectCode,TP_GetSysColor(COLOR_CODE_BACKDIS)); 

	if(m_bWndFocus)       	     pDC->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
	else if(m_bWndEnable)        pDC->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
	else                  	     pDC->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));

	
	//画入出点之间的底色
	BOOL   bTrimIn = FALSE,bTrimOut = FALSE,bPhantomMarkIn = FALSE,bPhantomMarkOut  = FALSE;
	m_rtTrimOut = CRect(0,0,0,0);
	m_rtTrimIn  = CRect(0,0,0,0);

	m_uBaseState &= ~(TP_PAINT_TRIMOUT |TP_PAINT_TRIMIN|TP_PAINT_PHANTOMOUT|TP_PAINT_PHANTOMIN|TP_PAINT_TRIMRECT); 

	m_rtPhantomMarkIn = CRect(0,0,0,0);
	m_rtPhantomMarkIn = CRect(0,0,0,0);
	BOOL bHighlightPhantomMarkArea = FALSE;

	if(m_bDrawPhantomMark == TRUE)   //计算Trim位置
	{
		INT64 lStartFrame = TP_DoubleToInt(m_dStartFrame); //XN00022740 用INT64 jw 20090724
		INT64 lEndFrame   = TP_DoubleToInt(m_dStartFrame + m_iTimeCodeWidth / m_dPixelPerFrame);
		if(m_nPhantomMarkIn >= 0 && m_nPhantomMarkOut >= 0)
		{
			rtTemp         = m_rtTimeCode;
			rtTemp.left    = TP_DoubleToInt(TranslateFrame(m_nPhantomMarkIn))+1;
			rtTemp.right   = TP_DoubleToInt(TranslateFrame(m_nPhantomMarkOut))+1;
			m_rtPhantomMarkIn         = rtTemp;
			m_rtPhantomMarkIn.left    = rtTemp.left - 10;
			m_rtPhantomMarkIn.right   = rtTemp.left;
			m_rtPhantomMarkOut        = rtTemp;
			m_rtPhantomMarkOut.left   = rtTemp.right; 
			m_rtPhantomMarkOut.right  = rtTemp.right + 10;
			if(m_nPhantomMarkIn >= lStartFrame && m_nPhantomMarkIn <= lEndFrame)
				bPhantomMarkIn = TRUE;
			if(m_nPhantomMarkOut >= lStartFrame && m_nPhantomMarkOut <= lEndFrame)
				bPhantomMarkOut = TRUE;
			if(m_nPhantomMarkIn || m_nPhantomMarkOut || bPhantomMarkIn < lStartFrame && bPhantomMarkOut >= lEndFrame) // XN00023179 wangjin 20091225
				bHighlightPhantomMarkArea = TRUE;//高亮
		}
		else if(m_nPhantomMarkIn >= 0)
		{
			rtTemp                   = m_rtTimeCode;
			rtTemp.left              = TP_DoubleToInt(TranslateFrame(m_nPhantomMarkIn))+1;
			m_rtPhantomMarkIn        = rtTemp;
			m_rtPhantomMarkIn.left   = rtTemp .left - 6;
			m_rtPhantomMarkIn.right  = rtTemp .left;
			if(m_nPhantomMarkIn >= lStartFrame && m_nPhantomMarkIn <= lEndFrame)
				bPhantomMarkIn = TRUE;
		}
		else if(m_nPhantomMarkOut >=0)
		{
			rtTemp                    = m_rtTimeCode;
			rtTemp.right              = TP_DoubleToInt(TranslateFrame(m_nPhantomMarkOut))+1;
			m_rtPhantomMarkOut        = rtTemp;
			m_rtPhantomMarkOut.left   = rtTemp.right;
			m_rtPhantomMarkOut.right  = rtTemp.right + 6;
			if(m_nPhantomMarkOut >= lStartFrame && m_nPhantomMarkOut <= lEndFrame)
				bPhantomMarkOut = TRUE;
		}
		if(m_nPhantomMarkIn  == m_nTrimIn)  bPhantomMarkIn = FALSE;
		if(m_nPhantomMarkOut == m_nTrimOut) bPhantomMarkOut = FALSE;
	}

	if(m_bDrawTrim == TRUE)   //计算Trim位置
	{
		INT64 lStartFrame = TP_DoubleToInt(m_dStartFrame);//XN00022740 用INT64 jw 20090724
		INT64 lEndFrame   = TP_DoubleToInt(m_dStartFrame + m_iTimeCodeWidth / m_dPixelPerFrame);

		BOOL   bDrawRect  = FALSE;

		if(m_nTrimIn >= 0 && m_nTrimOut >= 0)
		{
			rtTemp         = m_rtTimeCode;
			rtTemp.left  = TP_DoubleToInt(TranslateFrame(m_nTrimIn))+1;
#ifdef TP_NS_NET
			rtTemp.right   = TP_DoubleToInt(TranslateFrame(min(m_nTrimOut, m_nMaxFrame)))+1;
#else
			rtTemp.right = TP_DoubleToInt(TranslateFrame(m_nTrimOut))+1;
#endif
			m_rtTrimIn        = rtTemp;
			m_rtTrimIn.left   = rtTemp.left - 10; 
			m_rtTrimIn.right  = rtTemp.left; 
			m_rtTrimOut        = rtTemp;
			m_rtTrimOut.left   = rtTemp.right; 
			m_rtTrimOut.right  = rtTemp.right + 10;

			if(m_nTrimIn >= lStartFrame && m_nTrimIn <= lEndFrame)
				bTrimIn = TRUE;
#ifdef TP_NS_NET
			if(m_nTrimOut >= lStartFrame)
#else
			if(m_nTrimOut >= lStartFrame && m_nTrimOut <= lEndFrame)
#endif
				bTrimOut = TRUE;
			if(bTrimIn || bTrimOut || m_nTrimIn < lStartFrame && m_nTrimOut >= lEndFrame)
				bDrawRect = TRUE;
		}
		else if(m_nTrimIn >= 0)
		{
			rtTemp            = m_rtTimeCode;
			rtTemp.left       = TP_DoubleToInt(TranslateFrame(m_nTrimIn))+1;
			m_rtTrimIn        = rtTemp;
			m_rtTrimIn.left   = rtTemp.left - 6;
			m_rtTrimIn.right  = rtTemp.left;
			if(m_nTrimIn >= lStartFrame && m_nTrimIn <= lEndFrame)
				bTrimIn = TRUE;
		}
		else if(m_nTrimOut >= 0)
		{
			rtTemp = m_rtTimeCode;
#ifdef TP_NS_NET
			rtTemp.right = TP_DoubleToInt(TranslateFrame(min(m_nTrimOut, m_nMaxFrame)))+1;
#else
			rtTemp.right = TP_DoubleToInt(TranslateFrame(m_nTrimOut))+1;
#endif
			m_rtTrimOut        = rtTemp;
			m_rtTrimOut.left   = rtTemp.right;
			m_rtTrimOut.right  = rtTemp.right + 6;
			if(m_nTrimOut >= lStartFrame && m_nTrimOut <= lEndFrame)
				bTrimOut = TRUE;
		}
		if(bDrawRect) m_uBaseState |= TP_PAINT_TRIMRECT;//高亮
	}
	if(bHighlightPhantomMarkArea && (m_uBaseState & TP_PAINT_TRIMINPHAN))
	{
		rtTemp = m_rtTimeCode;
		rtTemp.left  = m_rtPhantomMarkIn.right;
		rtTemp.right = m_rtPhantomMarkOut.left;
		pDC->FillSolidRect(rtTemp,TP_GetSysColor(COLOR_TRIM_BACK));
	}
	else if((m_uBaseState & TP_PAINT_TRIMRECT) &&( m_bWndFocus || m_iTimeCodeType == 2) && (m_uBaseState & TP_PAINT_TRIMINOUT))
	{
		rtTemp = m_rtTimeCode;
		rtTemp.left  = m_rtTrimIn.right;
		rtTemp.right = m_rtTrimOut.left;
		if(!(m_uBaseState & TP_PAINT_FORCETRIM)) 
		{
			pDC->FillSolidRect(rtTemp,TP_GetSysColor(COLOR_TRIM_BACK));
		}
	}
	if(!m_bForceZoom && !m_bDragScrollBar && (m_iTimeCodeType == 1 || m_iTimeCodeType == 3))
	{
		rtTemp        = m_rtTimeCode;
		rtTemp.left  += TP_DoubleToInt (m_dStartZoomIn * m_iTimeCodeWidth);
		rtTemp.right  = m_rtTimeCode.left +  TP_DoubleToInt((m_dStartZoomIn + m_dZoomIn) * m_iTimeCodeWidth);
		if(m_iTimeCodeType == 1)	    rtTemp.top      = m_rtTimeCode.CenterPoint().y;
		else if(m_iTimeCodeType == 3)	rtTemp.bottom   = m_rtTimeCode.CenterPoint().y;
		pDC->FillSolidRect(rtTemp,TP_GetSysColor(COLOR_TRIM_BACKDIS));
	}
	

	PaintTimeSubIndecator(pDC,dStartFrame,dEndFrame);

	//画上下三根线条
	CPen *pSavePen = (CPen *)pDC ->SelectObject(CPen::FromHandle(m_pResPen[TL_PEN_LINE] ->hPen));
	if(m_bWndFocus)          pDC ->SelectObject(CPen::FromHandle(m_pResPen[TL_PEN_UP] ->hPen));

	if(m_nMaxFrame == m_nMinFrame +1)
	{
		if(bTrimOut) m_uBaseState |= TP_PAINT_TRIMOUT;
		if(bTrimIn)  m_uBaseState |= TP_PAINT_TRIMIN;

		if(bPhantomMarkOut) m_uBaseState |= TP_PAINT_PHANTOMOUT;
		if(bPhantomMarkIn)  m_uBaseState |= TP_PAINT_PHANTOMIN;

		rtTemp = rectCode;
		rtTemp.DeflateRect(0,0,0,2);
		if(m_iTimeCodeType ==1)
		{
			pDC->MoveTo(rtTemp.left,rtTemp.top + 8);
			pDC->LineTo(rtTemp.left,rtTemp.top +1);
			pDC->MoveTo(rtTemp.right,rtTemp.top + 8);
			pDC->LineTo(rtTemp.right,rtTemp.top +1);
		}
		else if(m_iTimeCodeType == 3)
		{
			pDC->MoveTo(rtTemp.left,rtTemp.top    + 5);
			pDC->LineTo(rtTemp.left,rtTemp.bottom - 1);
			pDC->MoveTo(rtTemp.right,rtTemp.top    + 5);
			pDC->LineTo(rtTemp.right,rtTemp.bottom - 1);
		}
		else
		{
			pDC->MoveTo(rtTemp.left,rtTemp.bottom - 11);
			pDC->LineTo(rtTemp.left,rtTemp.bottom - 1 );
			pDC->MoveTo(rtTemp.right,rtTemp.bottom - 11);
			pDC->LineTo(rtTemp.right,rtTemp.bottom - 1 );
		}
	}
	else
	{
#ifdef TP_PAINTTIMECODELINE2
		PaintTimeCodeLine2(pDC,dStartFrame,dEndFrame ,rectCode);
#else
		PaintTimeCodeLine1(pDC,dStartFrame,dEndFrame ,rectCode);
#endif
		if(bTrimOut) m_uBaseState |= TP_PAINT_TRIMOUT;
		if(bTrimIn)  m_uBaseState |= TP_PAINT_TRIMIN;

		if(bPhantomMarkOut) m_uBaseState |= TP_PAINT_PHANTOMOUT;
		if(bPhantomMarkIn)  m_uBaseState |= TP_PAINT_PHANTOMIN;

		//SceneInOut
		PaintTimeSceneInOut(pDC,dStartFrame,dEndFrame,rectCode);
		//画标记点
		rtTemp = rectCode;

#ifdef TP_NS_NET

		if ( m_nSafeLength > 0 && g_bSafeLength_Debug )
		{
			CRect rcSafeLength = m_rtTimeCode;
			rcSafeLength.left = rcSafeLength.right - 50;
			CString strSafeLength;
			strSafeLength.Format( _T("SL: %.3d"), m_nActualMaxFrame - m_nMaxFrame );
			COLORREF clrOld = pDC->SetTextColor( RGB(255,153,0) );
			pDC->DrawText( strSafeLength, &rcSafeLength, DT_RIGHT | DT_TOP | DT_SINGLELINE );
			pDC->SetTextColor( clrOld );
		}
		int iPixelCurDraw = 0, iPixelPreDraw = 0;//下面画跳变点用的
#endif
		for(INT l=0;l<m_aBrakeFrame.GetSize();l++)
		{
			if(m_aBrakeFrame[l]< dStartFrame) continue;
			else if(m_aBrakeFrame[l]> dEndFrame) break;	
			int iPixelTemp = TP_DoubleToInt(TranslateFrame(__int64(m_aBrakeFrame[l])));
#ifdef TP_NS_NET
			if (iPixelPreDraw == iPixelTemp) continue;
			iPixelPreDraw = iPixelTemp;
#endif
			ImageList_Draw(m_pResBar->GetImage(),TL_RES_BRAKE,pDC->GetSafeHdc(),iPixelTemp-3,rtTemp.top,ILD_TRANSPARENT);
		}

		int iMark1,iMark2 = -1;BYTE cType1,cType2 = 0;
		for(INT_PTR i=0;i<m_arrMarkPoint.GetSize();i++)
		{
			if(m_arrMarkPoint[i].lFrame >= dStartFrame && m_arrMarkPoint[i].lFrame <= dEndFrame)
			{
				iMark1 = TP_DoubleToInt(TranslateFrame(m_arrMarkPoint[i].lFrame))-5;
				cType1 = m_arrMarkPoint[i].cType;
				//位置重复但颜色不一样，也要重绘
				if(iMark1!=iMark2 || ((iMark1==iMark2)&&(cType1 != cType2)))
					ImageList_Draw(m_pResBar->GetImage(),GetMarkIndex(m_arrMarkPoint[i].cType),pDC->GetSafeHdc(),iMark1,rtTemp.top,ILD_TRANSPARENT);
				iMark2 = iMark1;
				cType2 = cType1;
			}
		}
		if(m_bSceneInout)  rtTemp.top = m_szSceneInOut.cy;
		else               rtTemp.top = rtTemp.top + 6;
		for(INT l=0;l<m_arrScenePoint.GetSize();l++)
		{
			if(m_arrScenePoint[l]< dStartFrame) continue;
			else if(m_arrScenePoint[l]> dEndFrame) break;
			int iPixelTemp = TP_DoubleToInt(TranslateFrame(__int64(m_arrScenePoint[l])));
			ImageList_Draw(m_pResBar->GetImage(),TL_RES_KEYFRAME,pDC->GetSafeHdc(),iPixelTemp-6,rtTemp.top,ILD_TRANSPARENT);
		}
	}
	pDC ->SelectObject(pSavePen);
}

void CTPTimeLine::PaintFrame(CDC *pDC)
{
#ifndef TP_NS_EDIT
	CRect rtTemp = m_rtTimeCode | m_rtTimeBar;
	rtTemp.InflateRect(1,1);
	pDC->Draw3dRect(rtTemp,TP_GetSysColor(COLOR_TL_RECT),TP_GetSysColor(COLOR_TL_RECT));
#endif
}

void CTPTimeLine::PaintTimeBar(CDC *pDC)
{
	CRect rectBar,rectTemp;
	rectBar      = GetTimeBarRect(); //总位置
	pDC->FillSolidRect(rectBar,TP_GetSysColor(COLOR_TL_TIMECODEBACK));

	m_rtTimeBar         = rectBar;
	m_rtBarTotal        = rectBar;   //滑动条位置
	m_rtBarTotal.left  += TP_DoubleToInt(m_dStartZoomIn * m_iTimeCodeWidth);
	m_rtBarTotal.right  = rectBar.left + TP_DoubleToInt((m_dStartZoomIn + m_dZoomIn) * m_iTimeCodeWidth);

	m_rtLeftHead        = m_rtBarTotal;  //左边头部位置
	m_rtLeftHead.right  = m_rtLeftHead.left + 8;

	m_rtRightHead        = m_rtBarTotal;  //右边头部位置
	m_rtRightHead.left   = m_rtRightHead.right - 7;

	m_rtBarHandle        = m_rtBarTotal;  //滑动条位置
	m_rtBarHandle.left   = m_rtLeftHead.right;
	m_rtBarHandle.right  = m_rtRightHead.left;

	if(m_rtLeftHead.right>=m_rtRightHead.left)
	{
		m_rtRightHead = m_rtLeftHead;m_rtRightHead.OffsetRect(/*m_pResBar[TL_RES_BARLEFTUP] ->Width()*/ 8 *2,0);
		m_rtBarTotal  = CRect(m_rtLeftHead.left,m_rtLeftHead.top,m_rtRightHead.right,m_rtRightHead.bottom);
		m_rtLeftHead  = CRect(m_rtLeftHead.right,m_rtLeftHead.top,m_rtRightHead.left,m_rtRightHead.bottom);	
	}

	if(m_rtBarHandle.Width()>2)
		TP_StretchDIBRect(pDC,m_pResBarBack ->GetByte(),m_pResBarBack ->Width(),m_pResBarBack ->Height(),0,0,m_rtBarHandle);

	if(m_iBarHeadState ==1 ) ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMLEFTDOWN,pDC->GetSafeHdc(),m_rtLeftHead.left,m_rtLeftHead.top,ILD_TRANSPARENT);
	else                     ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMLEFTUP,  pDC->GetSafeHdc(),m_rtLeftHead.left,m_rtLeftHead.top,ILD_TRANSPARENT);

	if(m_iBarHeadState ==2 ) ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMRIGHTDOWN,pDC->GetSafeHdc(),m_rtRightHead.left,m_rtRightHead.top,ILD_TRANSPARENT);
	else                     ImageList_Draw(m_pResBar->GetImage(),TL_RES_ZOOMRIGHTUP,  pDC->GetSafeHdc(),m_rtRightHead.left,m_rtRightHead.top,ILD_TRANSPARENT);
}

void CTPTimeLine::PaintTrimIcon(CDC *pDC)
{
	if(m_bDrawPhantomMark)
	{
		if(m_uBaseState & TP_PAINT_PHANTOMOUT)
		{
			if(m_enDragType == TP_DRAG_TRIMOUTOK || m_enDragType == TP_DRAG_TRIMOUT)
				ImageList_Draw(m_pResBar->GetImage(),TL_RES_PHAOUTDOWN,pDC->GetSafeHdc(),m_rtPhantomMarkOut.left - 12,m_rtPhantomMarkOut.top,ILD_TRANSPARENT);
			else
				ImageList_Draw(m_pResBar->GetImage(),TL_RES_PHAOUTUP,pDC->GetSafeHdc(),m_rtPhantomMarkOut.left - 12,  m_rtPhantomMarkOut.top,ILD_TRANSPARENT);//XN21611 jw 20090603
		}
		if(m_uBaseState & TP_PAINT_PHANTOMIN) 
		{
			if(m_enDragType == TP_DRAG_TRIMINOK ||  m_enDragType == TP_DRAG_TRIMIN)
				ImageList_Draw(m_pResBar->GetImage(),TL_RES_PHAINDOWN,pDC->GetSafeHdc(),m_rtPhantomMarkIn.right -20,m_rtPhantomMarkIn.top,ILD_TRANSPARENT);
			else
				ImageList_Draw(m_pResBar->GetImage(),TL_RES_PHAINUP,pDC->GetSafeHdc(),m_rtPhantomMarkIn.right -20,m_rtPhantomMarkIn.top,ILD_TRANSPARENT);
		}
	}
	if(m_bDrawTrim)
	{
		if(m_iTimeCodeType == 3)
		{
			if(m_uBaseState & TP_PAINT_TRIMOUT)
			{
				if(m_enDragType == TP_DRAG_TRIMOUTOK || m_enDragType == TP_DRAG_TRIMOUT)
					ImageList_Draw(m_pResBar->GetImage(),TL_RES_TRIMOUTDOWN,pDC->GetSafeHdc(),m_rtTrimOut.left - 23, m_rtTrimOut.top,ILD_TRANSPARENT);
				else
					ImageList_Draw(m_pResBar->GetImage(),TL_RES_TRIMOUTUP,pDC->GetSafeHdc(),m_rtTrimOut.left - 23, m_rtTrimOut.top,ILD_TRANSPARENT);
			}
			if(m_uBaseState & TP_PAINT_TRIMIN) 
			{
				if(m_enDragType == TP_DRAG_TRIMINOK || m_enDragType == TP_DRAG_TRIMIN)
					ImageList_Draw(m_pResBar->GetImage(),TL_RES_TRIMINDOWN,pDC->GetSafeHdc(),m_rtTrimIn.right -  10 ,m_rtTrimIn.top,ILD_TRANSPARENT);
				else
					ImageList_Draw(m_pResBar->GetImage(),TL_RES_TRIMINUP,pDC->GetSafeHdc(),m_rtTrimIn.right -  10, m_rtTrimIn.top,ILD_TRANSPARENT);
			}
		}
		else
		{
			if(m_uBaseState & TP_PAINT_TRIMOUT)
			{
				if(m_enDragType == TP_DRAG_TRIMOUTOK || m_enDragType == TP_DRAG_TRIMOUT)
					ImageList_Draw(m_pResBar->GetImage(),TL_RES_TRIMOUTDOWN,pDC->GetSafeHdc(),m_rtTrimOut.left - 12, m_rtTrimOut.top,ILD_TRANSPARENT);
				else
					ImageList_Draw(m_pResBar->GetImage(),TL_RES_TRIMOUTUP,pDC->GetSafeHdc(),m_rtTrimOut.left - 12, m_rtTrimOut.top,ILD_TRANSPARENT);
			}
			if(m_uBaseState & TP_PAINT_TRIMIN) 
			{
				if(m_enDragType == TP_DRAG_TRIMINOK ||  m_enDragType == TP_DRAG_TRIMIN)
					ImageList_Draw(m_pResBar->GetImage(),TL_RES_TRIMINDOWN,pDC->GetSafeHdc(),m_rtTrimIn.right -  20 ,m_rtTrimIn.top,ILD_TRANSPARENT);
				else
					ImageList_Draw(m_pResBar->GetImage(),TL_RES_TRIMINUP,pDC->GetSafeHdc(),m_rtTrimIn.right -  20, m_rtTrimIn.top,ILD_TRANSPARENT);
			}
		}
	}
}

void CTPTimeLine::PaintTimeCodeLine1(CDC *pDC,double dStartFrame,double dEndFrame ,CRect &rectCode)
{
	__int64       iParentStep     = 1;                        //每大格的帧数
	__int64       iChileStep      = 5;                        //每大格多少小格
	double        dPixelPerStep   = 0;                        //每小格占的象素 
	int           iPixelTemp,iStartFrame;
	double        iPixelTempStep;

	//计算每大格的帧数
	iParentStep = max(1,TP_DoubleToInt(150/m_dPixelPerFrame));                    //每大格宽度不能小于 150 个象素
	if(iParentStep > 5) iParentStep = iParentStep /5*5;                           //每大格最好为5的整数倍
	if(m_enVideoType == 3 && iParentStep >= 25)  iParentStep = iParentStep/25*25; //每大格最好为每秒的整数倍
	else if(iParentStep >= 30)                   iParentStep = iParentStep/30*30;
	iParentStep = max(iParentStep,1);

	if(iParentStep>1)
	{
		iChileStep    = 5;
		while(iParentStep * m_dPixelPerFrame/iChileStep > 10)	  iChileStep += 5;
		if(iParentStep % iChileStep != 0 && iParentStep % 5 ==0 ) iChileStep -=5;
		if(iChileStep > iParentStep) iChileStep = iParentStep;
	}

	if(dStartFrame>=dEndFrame) dStartFrame = max(0,dEndFrame-1);
	iStartFrame = TP_DoubleToInt(dStartFrame);
	if(iStartFrame<0) iStartFrame = (iStartFrame /5-1) *5; 
	else              iStartFrame = iStartFrame /5*5; 
	dPixelPerStep = iParentStep * m_dPixelPerFrame/iChileStep;

	CRect rtTemp = rectCode,rtText;
	rtTemp.DeflateRect(0,0,0,2);
	__int64 iFrame = 0,iBegin = iStartFrame<0?((iStartFrame /iParentStep-1) * iParentStep):(iStartFrame /iParentStep * iParentStep);
	for(__int64 i = iBegin;i<dEndFrame +iParentStep;i+=iParentStep) //大格
	{
		iPixelTemp   = TP_DoubleToInt(TranslateFrame(i));
		rtTemp.left  = iPixelTemp-45;
		rtTemp.right = iPixelTemp+45;
		if(rtTemp.left > rectCode.right) break;

		if(m_iTimeCodeType ==1)
		{
			pDC->MoveTo(iPixelTemp,rtTemp.top + 8); 
			pDC->LineTo(iPixelTemp,rtTemp.top +1); 
			rtText = rtTemp;  rtText.top = rtTemp.top + 8;
		}
		else if(m_iTimeCodeType == 3)
		{
			pDC->MoveTo(iPixelTemp,rtTemp.top    + 5); 
			pDC->LineTo(iPixelTemp,rtTemp.bottom - 1); 
			rtText = rtTemp;  rtText.top = rtTemp.top + 5;
		}
		else
		{
			pDC->MoveTo(iPixelTemp,rtTemp.bottom - 11);
			pDC->LineTo(iPixelTemp,rtTemp.bottom - 1 );
			rtText = rtTemp;  rtText.top = rtTemp.top + 1; rtText.bottom = rtTemp.bottom - 12;
		}
		if(m_iTimeCodeType == 2)
		{
			iFrame = i;
			if(iFrame<0) {if(m_enVideoType == VIDEO_25) iFrame+= 24*3600*25;else  iFrame+= 24*3600*30;}
			pDC->DrawText(TP_StringFromFrame(m_enVideoType,iFrame,FALSE),rtText,DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
		}
		iPixelTempStep = iPixelTemp;
		for(int l = 1;l<iChileStep;l++) //小格
		{
			iPixelTempStep +=  dPixelPerStep;

			if(iPixelTempStep <rectCode.left)   continue;
			if(iPixelTempStep  >rectCode.right) break;
			if(m_iTimeCodeType ==1)
			{
				if(iChileStep %5 == 0)
				{
					if(l %5==0)	pDC->MoveTo(TP_DoubleToInt(iPixelTempStep),rtTemp.top + 6);
					else	 	pDC->MoveTo(TP_DoubleToInt(iPixelTempStep),rtTemp.top + 4);
				}
				else 	 	    pDC->MoveTo(TP_DoubleToInt(iPixelTempStep),rtTemp.top + 4);
				pDC->LineTo(TP_DoubleToInt(iPixelTempStep),rtTemp.top +1);
			}
			else if(m_iTimeCodeType == 3)
			{
				if(iChileStep %5 == 0)
				{
					if(l %5==0)	pDC->MoveTo(TP_DoubleToInt(iPixelTempStep),rtTemp.top + 7);
					else	 	pDC->MoveTo(TP_DoubleToInt(iPixelTempStep),rtTemp.top + 9);
				}
				else 	 	    pDC->MoveTo(TP_DoubleToInt(iPixelTempStep),rtTemp.top + 9);
				pDC->LineTo(TP_DoubleToInt(iPixelTempStep),rtTemp.bottom-1);
			}
			else
			{
				if(iChileStep %5 == 0)
				{
					if(l %5==0)	pDC->MoveTo(TP_DoubleToInt(iPixelTempStep),rtTemp.bottom - 9);
					else	 	pDC->MoveTo(TP_DoubleToInt(iPixelTempStep),rtTemp.bottom - 7);
				}
				else 	 	    pDC->MoveTo(TP_DoubleToInt(iPixelTempStep),rtTemp.bottom - 7);
				pDC->LineTo(TP_DoubleToInt(iPixelTempStep),rtTemp.bottom-1);
			}
		}
	}
}
void CTPTimeLine::PaintTimeTrim(CDC *pDC)
{
}
void CTPTimeLine::PaintTimeTrack(CDC *pDC)
{
}
void CTPTimeLine::PaintTimeLine(CDC *pDC)
{
	if(PaintTimeLineEx)
	{
		PaintTimeLineEx(pDC);
	}
	else
	{
		int    iIconWidth  = 7;
		double dStartFrame = m_dStartFrame;
		double dEndFrame   = m_dStartFrame + m_iTimeCodeWidth / m_dPixelPerFrame;

		m_rtTimeLine = CRect(0,0,0,0);
		CRect rectTime = GetTimeLineRect();	
		m_rtTimeLine = rectTime;

		if(m_dCurFrame <dStartFrame || m_dCurFrame >TP_DoubleToInt(dEndFrame)) return;

		//画头部
		CPen *pSavePen = (CPen *) pDC ->SelectObject(CPen::FromHandle(m_pResPen[TL_PEN_TLLINE] ->hPen));

		pDC->MoveTo(rectTime.left +1,rectTime.top );
		pDC->LineTo(rectTime.left +1,rectTime.bottom);

		pDC ->SelectObject(pSavePen);

		m_rtTimeLineTest = m_rtTimeLine;
		m_rtTimeLineTest.left  = m_rtTimeLineTest.CenterPoint().x -1;
		m_rtTimeLineTest.right = m_rtTimeLineTest.left + 3;
	}
}

void CTPTimeLine::Invalidate(BOOL bErase)
{
	m_bForceDraw = TRUE;
	CWnd::Invalidate(bErase);
}
void CTPTimeLine::Draw3DLine(CDC *pDC,CRect &rtClip,CPen *p3DL,CPen *p3DR,BOOL bVert)
{
	if(pDC == NULL || p3DL == NULL || p3DR == NULL) return;
	HPEN hPen = (HPEN)::SelectObject(pDC ->m_hDC,(HPEN)p3DL ->m_hObject);
	if(bVert)
	{
		pDC ->MoveTo(rtClip.left,rtClip.bottom-1);
		pDC ->LineTo(rtClip.left,rtClip.top);
	}
	else
	{
		pDC ->MoveTo(rtClip.left,rtClip.top);
		pDC ->LineTo(rtClip.right,rtClip.top);
	}	
	::SelectObject(pDC ->m_hDC,(HPEN)p3DR ->m_hObject);	
	if(bVert)
	{
		pDC ->MoveTo(rtClip.right,rtClip.bottom-1);
		pDC ->LineTo(rtClip.right,rtClip.top);
	}
	else
	{
		pDC ->MoveTo(rtClip.left,rtClip.bottom);
		pDC ->LineTo(rtClip.right,rtClip.bottom);
	}
	pDC->SelectObject(hPen);
}
void CTPTimeLine::Draw3DRect(CDC *pDC,CRect &rtClip,CPen *p3DL,CPen *p3DR)
{
	if(pDC == NULL || p3DL == NULL || p3DR == NULL) return;
	HPEN hPen = (HPEN)::SelectObject(pDC ->m_hDC,(HPEN)p3DL ->m_hObject);
	pDC ->MoveTo(rtClip.left,rtClip.bottom-1);
	pDC ->LineTo(rtClip.left,rtClip.top);
	pDC ->LineTo(rtClip.right-1,rtClip.top);
	::SelectObject(pDC ->m_hDC,(HPEN)p3DR ->m_hObject);	
	pDC ->LineTo(rtClip.right-1,rtClip.bottom-1);
	pDC ->LineTo(rtClip.left,rtClip.bottom-1);
	pDC->SelectObject(hPen);
}
void CTPTimeLine::Draw3DRect(CDC *pDC,CRect &rtClip,TPCommonRes *p3DL,TPCommonRes *p3DR)
{
	if(pDC == NULL || p3DL == NULL || p3DR == NULL) return;
	HPEN hPen = (HPEN)::SelectObject(pDC ->m_hDC,p3DL ->hPen);
	pDC ->MoveTo(rtClip.left,rtClip.bottom-1);
	pDC ->LineTo(rtClip.left,rtClip.top);
	pDC ->LineTo(rtClip.right-1,rtClip.top);
	::SelectObject(pDC ->m_hDC,p3DR ->hPen);	
	pDC ->LineTo(rtClip.right-1,rtClip.bottom-1);
	pDC ->LineTo(rtClip.left,rtClip.bottom-1);
	pDC->SelectObject(hPen);

}
void CTPTimeLine::PaintTimeLinePre(CDC *pDC)
{
}
void CTPTimeLine::PaintDC(CDC *pDC,BOOL bClipBox)
{
	CDC      cdcMemo;
	CBitmap *bmpSave = NULL;
	cdcMemo.CreateCompatibleDC(pDC);
	pDC->GetClipBox(m_rtClipBox);
	BOOL bForceDrawTemp = m_bForceDraw;
	if (m_bForceDraw == 100) // XN00030021 Wangjin 20101118
	{
		m_bForceDraw = FALSE;
		bmpSave = cdcMemo.SelectObject(&m_bmpDrawSave);
	}
	else if(m_bForceDraw || m_dPixelPerFrame !=m_dSavePixelPerFrame || m_dStartFrame !=m_dSaveStatrFrame || m_iDrawWidth != m_rtClient.Width()||m_iDrawHeight != m_rtClient.Height())
	{
		m_bForceDraw = FALSE;//可能多线程访问,先修改 DQA8534 jw 20091230
		cdcMemo.SetBkMode(TRANSPARENT);
		cdcMemo.SetStretchBltMode(COLORONCOLOR);

		if(m_iDrawWidth != m_rtClient.Width()||m_iDrawHeight != m_rtClient.Height())
		{
			m_iDrawWidth  = m_rtClient.Width();
			m_iDrawHeight = m_rtClient.Height();
			if(m_bmpDrawSave.m_hObject) m_bmpDrawSave.DeleteObject();
			m_bmpDrawSave.CreateCompatibleBitmap(pDC,m_iDrawWidth,m_iDrawHeight); 
			if(m_pBmpInOut && m_pBmpInOut ->m_hObject) m_pBmpInOut->DeleteObject();
			if(m_pBmpInOut)	m_pBmpInOut->CreateCompatibleBitmap(pDC,m_iDrawWidth,m_iDrawHeight);

			m_dSaveCurFrame = -1;
		}

		bmpSave = cdcMemo.SelectObject(&m_bmpDrawSave); 
		CFont *ftSave = cdcMemo.SelectObject(TP_GetSysFont(m_iFontIndex)); 

		if(bForceDrawTemp == 5 && !(m_uBaseState & (TP_PAINT_TRIMINOUT|TP_PAINT_TRIMINPHAN)))
		{
			cdcMemo.FillSolidRect(CRect(m_rtTimeCode.left,m_rtTimeCode.top,m_rtTimeCode.right,m_rtTimeCode.bottom),TP_GetSysColor(m_iBackColor));
			PaintTimeCode(&cdcMemo);
			PaintTimeTrim(&cdcMemo);
			PaintTrimIcon(&cdcMemo);
		}
		else
		{
			cdcMemo.FillSolidRect(CRect(m_rtClient.left,m_rtClient.top,m_rtClient.right,m_rtClient.bottom),TP_GetSysColor(m_iBackColor));
            if(m_bDragScrollBar) 	PaintTimeBar(&cdcMemo);
			PaintTimeCode(&cdcMemo);
			PaintTimeTrack(&cdcMemo);
			PaintTimeTrim(&cdcMemo);
			PaintFrame(&cdcMemo);   // 画logo
			PaintTrimIcon(&cdcMemo);
		}

		cdcMemo.SelectObject(ftSave);

		m_dSavePixelPerFrame = m_dPixelPerFrame;
		m_dSaveStatrFrame    = m_dStartFrame;
	}
	else
		bmpSave = cdcMemo.SelectObject(&m_bmpDrawSave); 

	PaintTimeLinePre(pDC);

	if(m_bUseRgn) SelectClipRgn(pDC->m_hDC,m_hInvertRgn);

	if(bClipBox && m_dPixelPerFrame >TP_MAX_SPLIT)
		pDC->BitBlt(m_rtClipBox.left,m_rtClipBox.top, m_rtClipBox.Width(),m_rtClipBox.Height(),&cdcMemo,m_rtClipBox.left,m_rtClipBox.top,SRCCOPY);  
	else
		pDC->BitBlt(0,0,m_iDrawWidth,m_iDrawHeight,&cdcMemo,0,0,SRCCOPY);  
	
	if(m_bUseRgn) SelectClipRgn(pDC->m_hDC,m_hClientRgn);

	if(m_nMaxFrame >= m_nMinFrame +1)
		PaintTimeLine(pDC);

	cdcMemo.SelectObject(bmpSave);
	cdcMemo.DeleteDC(); 

	if(m_bUseRgn) SelectClipRgn(pDC->m_hDC,(HRGN)NULL);
	m_dSaveCurFrame = m_dCurFrame;
}

CRect CTPTimeLine::GetTimeCodeRect()
{
	CRect rtTemp = m_rtClient;
	rtTemp.top    += m_iEdgeDisatnce;
	rtTemp.bottom  = rtTemp.top + min(m_iTimeCodeHeight,m_rtClient.Height());
	rtTemp.left   += m_iTimeCodeLeft+m_iEdgeDisatnce;
	rtTemp.right  -= m_iTimeCodeRight+m_iEdgeDisatnce;
	m_iTimeCodeWidth = rtTemp.Width();
	return rtTemp;
}
CRect CTPTimeLine::GetTimeBarRect()
{
	CRect rtTemp = m_rtClient;
	rtTemp.top    += m_iEdgeDisatnce+m_iTimeCodeHeight ;
	rtTemp.bottom  = rtTemp.top + m_iBarHeight;
	rtTemp.left   += m_iTimeCodeLeft+m_iEdgeDisatnce;
	rtTemp.right  -= m_iTimeCodeRight+m_iEdgeDisatnce;
	m_iTimeCodeWidth = rtTemp.Width();
	return rtTemp;
}
CRect CTPTimeLine::GetTimeLineRect()
{
	CRect rtTemp = m_rtClient;

	rtTemp.top    += m_iEdgeDisatnce;
	rtTemp.bottom -= m_iEdgeDisatnce;
	rtTemp.left    = TP_DoubleToInt(TranslateFrame(m_dCurFrame));
	rtTemp.right   = rtTemp.left + 3;
	return rtTemp;

	return rtTemp;
}
double CTPTimeLine::TranslateFrame(__int64 dFrame) const
{
	return m_iTimeCodeLeft+m_iEdgeDisatnce + ((dFrame - ((m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame)) * m_dPixelPerFrame)+m_iOffsetPixel;
}
double CTPTimeLine::TranslateFrame(long double dFrame) const
{
	return m_iTimeCodeLeft+m_iEdgeDisatnce + ((dFrame - ((m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame)) * m_dPixelPerFrame)+m_iOffsetPixel;
}
long double CTPTimeLine::TranslatePixel(double dPixel ) const
{
	long double dValue = ((dPixel-m_iOffsetPixel - (m_iTimeCodeLeft+m_iEdgeDisatnce)) / m_dPixelPerFrame +  ((m_nMaxFrame -m_nMinFrame)*m_dStartZoomIn + m_nMinFrame));
	dValue = max(dValue,m_nMinFrame);
	return dValue;
}
long double CTPTimeLine::TranslateString(CString sFrame)
{
	return TP_GetFrameFromString(m_enVideoType,sFrame);
}

BOOL CTPTimeLine::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_uBaseState &TP_TLWND_RBUTTON)
	{
		SetCursor(m_pResCursor[TL_ARROW] ->GetCursor());
		return TRUE;
	}

	else if(m_enDragType == TP_DRAG_UNKNOW)
	{
		int     iBarHeadState = m_iBarHeadState;
		CPoint  ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		m_hSetCursor = NULL;
		if(m_rtTrimIn.PtInRect(ptCursor) && m_bDragTrim && TP_IsAltKeyDown())         m_hSetCursor = m_pResCursor[TL_LEFT]->GetCursor();
		else if(m_rtTrimOut.PtInRect(ptCursor) && m_bDragTrim && TP_IsAltKeyDown())   m_hSetCursor = m_pResCursor[TL_RIGHT]->GetCursor();
		else if(m_rtLeftHead.PtInRect(ptCursor) && m_bDragScrollBar)                  m_hSetCursor = m_pResCursor[TL_LEFT]->GetCursor();
		else if(m_rtRightHead.PtInRect(ptCursor) && m_bDragScrollBar)                 m_hSetCursor = m_pResCursor[TL_RIGHT]->GetCursor();
		else if(TP_IsAltKeyDown()||!m_bAltDragMarkPoint)
		{
			BOOL bSetCursor = FALSE;
			if(m_bDragMarkPoint &&   ptCursor.y<=m_szSceneInOut.cx)
			{
				CRect rtTemp =  m_rtTimeCode; rtTemp.bottom = rtTemp.top + 6;
				for(INT l=0;l<m_arrMarkPoint.GetSize();l++)
				{
					rtTemp.left  = (long)TranslateFrame(m_arrMarkPoint[l].lFrame) -2;
					rtTemp.right = rtTemp.left + 4;
					if(rtTemp.PtInRect(ptCursor))
					{
						m_hSetCursor = m_pResCursor[TL_EW] ->GetCursor();
						bSetCursor = TRUE;
						break;
					}
				}
			}
			if(m_bSceneInout && bSetCursor == FALSE && ptCursor.y>=m_szSceneInOut.cx && ptCursor.y<=m_szSceneInOut.cy)
			{
				CRect rtTemp =  m_rtTimeCode;
				rtTemp.top    = m_szSceneInOut.cx;
				rtTemp.bottom = m_szSceneInOut.cy;
				for(int i=0;i<m_arrBlock.GetSize();i++)
				{
					rtTemp.left  = (long)TranslateFrame(m_arrBlock[i].lIn);
					rtTemp.right = (long)TranslateFrame(m_arrBlock[i].lOut);
					if(ptCursor.x<=rtTemp.left+4 && ptCursor.x>=rtTemp.left+2)
					{
						m_hSetCursor = m_pResCursor[TL_EW] ->GetCursor();
						bSetCursor = TRUE;
						break;
					}
					else if(ptCursor.x<=rtTemp.right && ptCursor.x>=rtTemp.right-4)
					{
						m_hSetCursor = m_pResCursor[TL_EW] ->GetCursor();
						bSetCursor = TRUE;
						break;
					}
					if(rtTemp.PtInRect(ptCursor))
					{
						m_hSetCursor = m_pResCursor[TL_HANDLE] ->GetCursor();
						bSetCursor = TRUE;
						break;
					}
				}
			}
		}
		if(m_hSetCursor ==NULL)	m_hSetCursor = m_pResCursor[TL_ARROW] ->GetCursor();
		SetCursor(m_hSetCursor);
		if(m_iBarHeadState != iBarHeadState)
		{
			m_bForceDraw = TRUE;
			InvalidateRect(GetTimeBarRect());
		}
		return TRUE;
	}
	else
	{
		SetCursor(m_hSetCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

__int64 CTPTimeLine::GetViewFrame()
{
	return TP_DoubleToInt64(m_iTimeCodeWidth / m_dPixelPerFrame);
}

BOOL CTPTimeLine::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CTPTimeLine::SetCaptureChanged( )
{
	if(!(m_uBaseState & (TP_TLWND_LBUTTON |TP_TLWND_LDBBUTTON))) return;
	if(m_enDragType == TP_DRAG_MARKPOINTOK || m_enDragType == TP_DRAG_SCENEINOK||m_enDragType == TP_DRAG_SCENEOUTOK)
	{
		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		OnLButtonUp(0,ptCursor);
	}
}
void CTPTimeLine::OnCaptureChanged(CWnd *pWnd)
{
	CWnd::OnCaptureChanged(pWnd);
	if(m_uBaseState &TP_DRAG_CAPTURE)
	{
		if(m_enDragType != TP_DRAG_UNKNOW)
		{
			SetCaptureChanged();
			m_enDragType = TP_DRAG_UNKNOW;
			SetCursor(m_pResCursor[TL_ARROW] ->GetCursor()); 
			m_bForceDraw = TRUE;
			Invalidate();
		}
		m_uBaseState &= ~TP_DRAG_CAPTURE;
	}
}

void CTPTimeLine::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_enDragType != TP_DRAG_UNKNOW) return;
	m_uBaseState &=  ~TP_TLWND_BUTTON; m_uBaseState |=  TP_TLWND_RBUTTON;
	if(GetFocus()!=this) SetFocus();
	CWnd::OnRButtonDown(nFlags, point);
	SetCursor(m_pResCursor[TL_ARROW] ->GetCursor());
	if(m_rtTimeCode.PtInRect(point) && TP_IsCtrlKeyDown())
	{
		m_enDragType   = TP_DRAG_RECT;
		m_ptDrag       = point;		
		m_rtDrag.top   = TP_DoubleToInt(TranslatePixel(point.x));
		m_rtDrag.left  = point.x;//记录拖动的起始点 jw 20081015
		m_rtDrag.right = point.y;
		m_rtCursorArea = m_rtTimeCode;
		return;
	}
}

void CTPTimeLine::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);
	if(!(m_uBaseState & TP_TLWND_RBUTTON)) return;
	if(m_enDragType == TP_DRAG_RECTOK)
	{
		m_uBaseState &= ~TP_DRAG_CAPTURE;
		ReleaseCapture();
	}
	else if(m_enDragType != TP_DRAG_UNKNOW)
	{
		m_enDragType = TP_DRAG_UNKNOW;
		SetCursor(m_pResCursor[TL_ARROW]->GetCursor()); 
		m_bForceDraw = TRUE;
		Invalidate();
	}
	else
	{
		//标记点
		m_iMenuPoint = -1;
		if(m_bMarkColor && point.y<=m_szSceneInOut.cx)
		{
			CRect rtTemp =  m_rtTimeCode;
			rtTemp.bottom = rtTemp.top   + m_szSceneInOut.cx;
			for(int i=0;i<m_arrMarkPoint.GetSize();i++)
			{
				double dPixelTemp = TranslateFrame(m_arrMarkPoint[i].lFrame);
				rtTemp.left  = (long)dPixelTemp -2;
				rtTemp.right = (long)dPixelTemp +4;
				if(rtTemp.PtInRect(point))
				{
					CTPMenuEx mainLoad,*pSubMenu = NULL;
					mainLoad.LoadMenu(IDR_EXTRAMENU);
					pSubMenu = mainLoad.GetSubMenu(8);
					pSubMenu ->SetMenuItemImage(ID_MARKCOLOR_DEFAULT,MF_BYCOMMAND,GetMarkColor(0),GetMarkIndex(m_arrMarkPoint[i].cType) == TL_RES_MARKPOINT,1);
					pSubMenu ->SetMenuItemImage(ID_MARKCOLOR_COLOR1, MF_BYCOMMAND,GetMarkColor(1),GetMarkIndex(m_arrMarkPoint[i].cType) == TL_RES_MARKPOINT1,1);
					pSubMenu ->SetMenuItemImage(ID_MARKCOLOR_COLOR2, MF_BYCOMMAND,GetMarkColor(2),GetMarkIndex(m_arrMarkPoint[i].cType) == TL_RES_MARKPOINT2,1);
					pSubMenu ->SetMenuItemImage(ID_MARKCOLOR_COLOR3, MF_BYCOMMAND,GetMarkColor(3),GetMarkIndex(m_arrMarkPoint[i].cType) == TL_RES_MARKPOINT3,1);
					pSubMenu ->SetMenuItemImage(ID_MARKCOLOR_COLOR4, MF_BYCOMMAND,GetMarkColor(4),GetMarkIndex(m_arrMarkPoint[i].cType) == TL_RES_MARKPOINT4,1);
					pSubMenu ->SetMenuItemImage(ID_MARKCOLOR_COLOR5, MF_BYCOMMAND,GetMarkColor(5),GetMarkIndex(m_arrMarkPoint[i].cType) == TL_RES_MARKPOINT5,1);
					ClientToScreen(&point);
					m_iMenuPoint = i;
					pSubMenu ->TrackPopupMenu(TPM_LEFTBUTTON,point.x,point.y,this);
					mainLoad.DestroyMenu();
				}
			}
		}
		if(m_bSceneInout && m_iMenuPoint ==-1 && point.y>=m_szSceneInOut.cx && point.y<=m_szSceneInOut.cy)
		{
			CRect rtTemp  = m_rtTimeCode;
			rtTemp.top    = m_szSceneInOut.cx;
			rtTemp.bottom = m_szSceneInOut.cy;
			for(int i=0;i<m_arrBlock.GetSize();i++)
			{
				rtTemp.left  = (long)TranslateFrame(m_arrBlock[i].lIn);
				rtTemp.right = (long)TranslateFrame(m_arrBlock[i].lOut);
				if(rtTemp.PtInRect(point))
				{
					CTPMenuEx mainLoad,*pSubMenu = NULL;
					mainLoad.LoadMenu(IDR_EXTRAMENU);
					pSubMenu = mainLoad.GetSubMenu(9);
					pSubMenu ->SetMenuItemImage(ID_SCENECOLOR_DEFAULT, MF_BYCOMMAND,GetMarkColor(0),GetMarkIndex(m_arrBlock[i].cType) == TL_RES_MARKPOINT,1);
					pSubMenu ->SetMenuItemImage(ID_SCENECOLOR_COLOR1, MF_BYCOMMAND,GetMarkColor(1),GetMarkIndex(m_arrBlock[i].cType) == TL_RES_MARKPOINT1,1);
					pSubMenu ->SetMenuItemImage(ID_SCENECOLOR_COLOR2, MF_BYCOMMAND,GetMarkColor(2),GetMarkIndex(m_arrBlock[i].cType) == TL_RES_MARKPOINT2,1);
					pSubMenu ->SetMenuItemImage(ID_SCENECOLOR_COLOR3, MF_BYCOMMAND,GetMarkColor(3),GetMarkIndex(m_arrBlock[i].cType) == TL_RES_MARKPOINT3,1);
					pSubMenu ->SetMenuItemImage(ID_SCENECOLOR_COLOR4, MF_BYCOMMAND,GetMarkColor(4),GetMarkIndex(m_arrBlock[i].cType) == TL_RES_MARKPOINT4,1);
					pSubMenu ->SetMenuItemImage(ID_SCENECOLOR_COLOR5, MF_BYCOMMAND,GetMarkColor(5),GetMarkIndex(m_arrBlock[i].cType) == TL_RES_MARKPOINT5,1);
					ClientToScreen(&point);
					m_iMenuPoint = i;
					if(!m_arrBlock[m_iMenuPoint].bSelect)
					{
						for(INT l=0;l<m_arrBlock.GetSize();l++)
							m_arrBlock[l].bSelect = (l == m_iMenuPoint);
						Invalidate();
					}
					pSubMenu ->TrackPopupMenu(TPM_LEFTBUTTON,point.x,point.y,this);
					mainLoad.DestroyMenu();
				}
			}
		}
	}
	m_uBaseState &= ~TP_TLWND_RBUTTON;
}
void CTPTimeLine::SetToolTipExText(CString sText)
{
	if(m_pShowTipsEx)
	{
		m_pShowTipsEx->UpdateTipText(sText,this,100);
		m_pShowTipsEx->m_sToolTips = sText;
		m_pShowTipsEx ->Invalidate();
	}
}

void CTPTimeLine::ShowToolTipEx(BOOL bShow)
{
	if(bShow)
	{
		CRect  rtClient;
		GetClientRect(&rtClient);
		m_pShowTipsEx = new CTPToolTipCtrl();
		m_pShowTipsEx ->m_iFontIndex = FONT_BASE_TEXT1;
		m_pShowTipsEx->Create(this,TTS_ALWAYSTIP); 

		m_pShowTipsEx ->SetTipTextColor(TP_GetSysColor(COLOR_BASE_TEXT));
		m_pShowTipsEx->EnableToolTips(TRUE); 
		m_pShowTipsEx->Activate(TRUE); 
		m_pShowTipsEx->AddTool(this,_T(""),rtClient,100);
		m_pShowTipsEx ->SetDelayTime(TTDT_INITIAL,10); 
		m_pShowTipsEx ->SetDelayTime(TTDT_RESHOW,10); 
	}
	else if(m_pShowTipsEx)
	{
		m_pShowTipsEx->DestroyToolTipCtrl();
		m_pShowTipsEx = NULL;
	}
}
void  CTPTimeLine::SetLineType(BOOL bSeekEff)
{
	TP_ReleaseCommonRes(m_pResPen[TL_PEN_TLLINE]);
	if(bSeekEff) m_pResPen[TL_PEN_TLLINE] = TP_LoadPen(COLOR_LINE_LINE,PS_SOLID,1);
	else  m_pResPen[TL_PEN_TLLINE] = TP_LoadPen(COLOR_LINE_LINESEEKEFF,PS_SOLID,1); 
}
BOOL CTPTimeLine::ZoomX(CPoint &ptStart,CPoint &ptEnd)
{
	if(ptStart.x == ptEnd.x) return FALSE;
	if(ptStart.y == ptEnd.y) return TRUE;
	if(abs(ptStart.x - ptEnd.x) >abs(ptStart.y - ptEnd.y)) return TRUE;

	return FALSE;
}

void CTPTimeLine::SetPhantomMark(BOOL bSV,TPTLPhantomMark &stuData)
{
	m_stuPhantomMark = stuData;
	Invalidate();
	//stuData.iSVLen --;
	stuData.iMVLen --;
	if(!stuData.bSVEnable)
	{
		m_nPhantomMarkIn  =  -1;
		m_nPhantomMarkOut =  -1;
	}
	else if(bSV)
	{
		INT64 iTLoffsetIn = m_nMinFrame; // XN00025650 wangjin 20091125
		if (stuData.iSVSpeedIn >= 0)	iTLoffsetIn = stuData.iSVSpeedIn;
		INT64   iSVLen = stuData.iSVLen + iTLoffsetIn;


		m_nTrimIn  = stuData.iSVIn>=0  ? stuData.iSVIn  : -1;
		m_nTrimOut = stuData.iSVOut>=0 ? stuData.iSVOut : -1;
		
		if(stuData.iSVIn <0 && stuData.iSVOut<0)
		{
			if (stuData.iMVIn <0 && stuData.iMVOut <0)
			{
				if(iSVLen>0)
				{
					m_nPhantomMarkIn  = iTLoffsetIn;
					m_nPhantomMarkOut = iSVLen;
				}
				else
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = -1;
				}
			}
			else if (stuData.iMVIn <0 && stuData.iMVOut >=0)
			{
				if (stuData.iTLPos < stuData.iMVOut)
				{
					if ((stuData.iMVOut-stuData.iTLPos) <= iSVLen)
					{
						m_nPhantomMarkIn  = iTLoffsetIn + 0;
						m_nPhantomMarkOut = iTLoffsetIn + stuData.iMVOut-stuData.iTLPos;
					}
					else
					{
						m_nPhantomMarkIn  = -1;
						m_nPhantomMarkOut = -1;
					}
				}
				else
				{
					if (iSVLen <= stuData.iMVOut-m_nMinFrame)
					{
						m_nPhantomMarkIn  = iTLoffsetIn +0; // DQAdb00005844 wangjin 29100107
						m_nPhantomMarkOut = iSVLen;
					}
					else
					{
						m_nPhantomMarkIn  = -1;  //DQAdb00007737
						m_nPhantomMarkOut = -1;
					}
				}
			}
			else if (stuData.iMVIn >=0 && stuData.iMVOut <0)
			{
				m_nPhantomMarkIn  = iTLoffsetIn +0;
				m_nPhantomMarkOut = iSVLen;
			}
			else if (stuData.iMVIn >=0 && stuData.iMVOut >0)
			{
				if (stuData.iMVIn < stuData.iMVOut)
				{
					if ((stuData.iMVOut-stuData.iMVIn) <= iSVLen)
					{
						m_nPhantomMarkIn  = iTLoffsetIn +0;
						m_nPhantomMarkOut = iTLoffsetIn +stuData.iMVOut-stuData.iMVIn;
					}
					else
					{
						m_nPhantomMarkIn  = -1;
						m_nPhantomMarkOut = -1;
					}
				}
				else
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = -1;
				}
			}
		}
		else if (stuData.iSVIn <0 && stuData.iSVOut>=0)
		{
			if (stuData.iMVIn <0 && stuData.iMVOut <0)
			{
				m_nPhantomMarkIn  = iTLoffsetIn +0;
				m_nPhantomMarkOut = -1;
			}
			else if (stuData.iMVIn <0 && stuData.iMVOut >=0)
			{
				if (stuData.iTLPos < stuData.iMVOut)
				{
					m_nPhantomMarkIn  = stuData.iSVOut-(stuData.iMVOut-stuData.iTLPos);
					m_nPhantomMarkOut = -1;
				}
				else
				{
					if (stuData.iSVOut-iTLoffsetIn <= stuData.iMVOut-m_nMinFrame) // DQAdb00008969 wangjin 20100108
					{
						m_nPhantomMarkIn  = iTLoffsetIn +0;
						m_nPhantomMarkOut = -1;
					}
					else
					{
						m_nPhantomMarkIn  = -1;
						m_nPhantomMarkOut = -1;
					}
				}
			}
			else if (stuData.iMVIn >=0 && stuData.iMVOut <0)
			{
				m_nPhantomMarkIn  = iTLoffsetIn +0;
				m_nPhantomMarkOut = -1;
			}
			else if (stuData.iMVIn >=0 && stuData.iMVOut >0)
			{
				if (stuData.iMVIn <= stuData.iMVOut)
				{
					m_nPhantomMarkIn  = stuData.iSVOut-(stuData.iMVOut-stuData.iMVIn);
					m_nPhantomMarkOut = -1;
				}
				else
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = -1;
				}
			}
		}
		else if (stuData.iSVIn >=0 && stuData.iSVOut <0)
		{
			if (stuData.iMVIn <0 && stuData.iMVOut <0)
			{
				m_nPhantomMarkIn  = -1;
				m_nPhantomMarkOut = iSVLen;
			}
			else if (stuData.iMVIn <0 && stuData.iMVOut >=0)
			{
				if (stuData.iTLPos < stuData.iMVOut)
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = stuData.iSVIn+stuData.iMVOut-stuData.iTLPos;
				}
				else if (iSVLen-stuData.iSVIn > stuData.iMVOut)
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = -1;
				}
				else
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = iSVLen;
				}
			}
			else if (stuData.iMVIn >=0 && stuData.iMVOut <0)
			{
				m_nPhantomMarkIn  = -1;
				m_nPhantomMarkOut = iSVLen;
			}
			else if (stuData.iMVIn >=0 && stuData.iMVOut >0)
			{
				if (stuData.iMVIn < stuData.iMVOut)
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = stuData.iSVIn+stuData.iMVOut-stuData.iMVIn;
				}
				else
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = -1;
				}
			}
		}
		else if (stuData.iSVIn >=0 && stuData.iSVOut >=0)
		{
			if (stuData.iSVIn >= stuData.iSVOut)
			{
				m_nPhantomMarkIn  = -1;
				m_nPhantomMarkOut = -1;
			}
			else
			{
				if (stuData.iMVIn <0 && stuData.iMVOut <0)
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = -1;
				}
				else if (stuData.iMVIn <0 && stuData.iMVOut >=0)
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = -1;
				}
				else if (stuData.iMVIn >=0 && stuData.iMVOut <0)
				{
					m_nPhantomMarkIn  = -1;
					m_nPhantomMarkOut = -1;
				}
				else if (stuData.iMVIn >=0 && stuData.iMVOut >=0)
				{
					if (stuData.iMVIn < stuData.iMVOut)
					{
						m_nPhantomMarkIn  = -1;
						m_nPhantomMarkOut = stuData.iSVIn+stuData.iMVOut-stuData.iMVIn;
					}
					else
					{
						m_nPhantomMarkIn  = -1;
						m_nPhantomMarkOut = -1;
					}
				}
			}
		}
	}
	else
	{
		m_nTrimIn  = stuData.iMVIn>=0  ? stuData.iMVIn  : -1;
		m_nTrimOut = stuData.iMVOut>=0 ? stuData.iMVOut : -1;

		if (stuData.iMVIn >=0 && stuData.iMVOut >=0)
		{
			m_nPhantomMarkIn = -1;
			m_nPhantomMarkOut = -1;
		}
		else
		{
			if(stuData.iMVIn<0 && stuData.iMVOut<0 && stuData.iSVIn<stuData.iSVOut)
			{
				m_nPhantomMarkIn  = stuData.iTLPos;
				m_nPhantomMarkOut = stuData.iTLPos + stuData.GetSVLen();
			}
			else if(stuData.iMVIn < 0)
			{
				BOOL bPLPreMVOut = stuData.iMVOut<0 || (stuData.iMVOut>0 && stuData.iTLPos<stuData.iMVOut);
				BOOL bSVInOutBothExit = (stuData.iSVIn>=0 && stuData.iSVOut>=0);
				if(bPLPreMVOut && !bSVInOutBothExit)
				{
					if (stuData.iTLPos < stuData.iMVOut-stuData.GetSVLen())	m_nPhantomMarkIn  = -1;
					else													m_nPhantomMarkIn  = stuData.iTLPos;
					if (stuData.iMVOut > 0)									m_nPhantomMarkOut = -1;
					else													m_nPhantomMarkOut = stuData.iTLPos+stuData.GetSVLen();
				}
				else
				{
					if ((!bSVInOutBothExit || stuData.iSVIn<stuData.iSVOut) && stuData.iMVOut-stuData.GetSVLen()>=0)
						m_nPhantomMarkIn = stuData.iMVOut-stuData.GetSVLen();
					else
						m_nPhantomMarkIn = -1;
					m_nPhantomMarkOut = -1;
				}
			}
			else
			{
				m_nPhantomMarkIn  = -1;
				if (stuData.iSVIn>=0 && stuData.iSVOut>=0 && stuData.iSVIn>=stuData.iSVOut)
					m_nPhantomMarkOut  = -1;
				else
					m_nPhantomMarkOut = stuData.iMVIn+stuData.GetSVLen();
			}
		}
	}

	stuData = m_stuPhantomMark;
}