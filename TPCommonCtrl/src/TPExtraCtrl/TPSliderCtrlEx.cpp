// SeSliderCtrl.cpp : インプリメンテーション ファイル
//


#include "stdafx.h"
#include "TPSliderCtrlEx.h"
#include "Resource.h"


#include "TPMouseController.h"
#include "TPMouseDeviceHooker.h"
#include "TPMouseCalc.h"
#include <cmath>
#include ".\tpsliderctrlex.h"



IMPLEMENT_DYNAMIC(CTPSliderCtrlEX, CTPSliderCtrl)

CTPSliderCtrlEX::CTPSliderCtrlEX()
:	m_bRecived(FALSE)
,	m_dValMin(0.0)
,	m_dValMax(0.0)
,	m_dValue(0.0)
,	m_dStartVal(0.0)
,	m_nOperateState(Op_Nothing)
,	m_dStackValue(0.0)
,	m_nOperateType(CTPMouseCalc::SE_SLT_INTELLIGENT)
,	m_dStep(0)
{
}

CTPSliderCtrlEX::~CTPSliderCtrlEX()
{
}

BEGIN_MESSAGE_MAP(CTPSliderCtrlEX, CTPSliderCtrl)
	//{{AFX_MSG_MAP(CSeSliderCtrlEx)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SE_MOUSEHOOK, OnMouseDeviceHookCursorMove)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


int  CTPSliderCtrlEX::PixelToPos(CPoint point)
{
	int iMin,iMax;
	GetRange(iMin,iMax);
	return int((point.x - m_rtChannel.left - m_rtThumb.Width()/2) * (iMax - iMin)/(m_rtChannel.Width() - m_rtThumb.Width()+0.00001)) + iMin;	
}

void CTPSliderCtrlEX::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),TRACKBAR_CLASS,&wndClass);		
		wndClass.lpszClassName = WC_SLIDERCTRLEX;
		::RegisterClass(&wndClass);
	}
	else 
		UnregisterClass(WC_SLIDERCTRLEX,AfxGetInstanceHandle());

}
void CTPSliderCtrlEX::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(GetFocus() != this) SetFocus();

	if(!m_rtThumb.PtInRect(point) && point.y <m_rtThumb.bottom && point.y>m_rtThumb.top)	
	{
		SetPos(PixelToPos(point));

		::SendMessage(GetParent()->GetSafeHwnd(), WM_HSCROLL, MAKELONG(TB_THUMBPOSITION, GetPos()), LPARAM(GetSafeHwnd()));

	}

	if(GetCapture() == this){
		return;
	}

	CTPMouseDeviceHooker* pHooker = NULL;

	if(CanIntelligentOperation() && (pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker()) != NULL)
	{
		pHooker->SetMouseAxis(SE_CALC_X);
		InitCalcVal();

		CPoint pt = point;
		ClientToScreen(&pt);
		m_ptPrevPoint.x = pt.x;
		m_ptPrevPoint.y = pt.y;

		CTPMouseCalc* pCalc = NULL;
		if(m_nOperateType == CTPMouseCalc::SE_SLT_INTELLIGENT){
			pCalc = CTPMouseDeviceImpl::GetDefMouseCalcIntelligent();
		}
		pCalc->SetCalcAxis(SE_CALC_X);


		if(GetKeyState(VK_CONTROL)<0)
		{
			if(pHooker)
				pHooker->SetCtrl(TRUE);
		}

	
		CRect rcThumb;
		GetThumbRect(&rcThumb);
		if(m_pThumbBmp[0])
			rcThumb.DeflateRect(-(m_pThumbBmp[0]->Width() - rcThumb.Width())/2,-(m_pThumbBmp[0]->Height() - rcThumb.Height())/2);	

		if(rcThumb.PtInRect(point))
		{
			if(pHooker->StartHook(GetSafeHwnd(), pt, pCalc, CTPMouseDeviceHooker::Chk_LButtonUp))
			{
				/*jyn added 20090510 
				*content:Mini Scroll
				*/
				double dRange = (m_dValMax-m_dValMin)/m_dStep;
				pHooker->SetRange(dRange);
				pHooker->SetChannelRect(m_rtChannel);
				/*jyn added end*/
				m_nOperateState = Op_Intelligent;
				SetCapture();
				/*GetParent()->SendMessage(WM_SE_PARAMETER, GetDlgCtrlID());*/
			}
			else{
				m_nOperateState = Op_Nothing;
				ASSERT(FALSE);
			}
		}
	}

	if(m_nOperateState == Op_Nothing)
	{
		m_nOperateState = Op_Original;
		CTPSliderCtrl::OnLButtonDown(nFlags, point);
	}
}

////////////////////////////////////////////////////////////////////////////
// OnLButtonUp

void CTPSliderCtrlEX::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int nLastOperateState = m_nOperateState;
	m_nOperateState = Op_Nothing;


	if(nLastOperateState == Op_Original){
		CTPSliderCtrl::OnLButtonUp(nFlags, point);
	}
	else if(nLastOperateState == Op_Intelligent)
	{
		CTPMouseDeviceHooker* pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker();
		/*ﾊ�ﾌｧﾆｬﾒｪﾉ靹ﾃCtrlﾎｪﾄｬﾈﾏﾖｵ  */
		if(pHooker)	
			pHooker->SetCtrl(FALSE);

		if(pHooker != NULL)
			pHooker->ExitHook();

		ReleaseCapture();

		if(m_bRecived && pHooker){
			CalcValue(pHooker->GetCurrentPosReal());
		}
		else
		{
			ClientToScreen(&point);
			SEPOINTREAL pt = SEPOINTREAL(point.x, point.y );
			CalcValue(pt);
		}
		::SendMessage(GetParent()->GetSafeHwnd(), WM_HSCROLL, MAKELONG(TB_ENDTRACK, GetPos()), LPARAM(GetSafeHwnd()));
	}
}

////////////////////////////////////////////////////////////////////////////
// OnCaptureChange

void CTPSliderCtrlEX::OnCaptureChanged(CWnd *pWnd) 
{

	int nLastOperateState = m_nOperateState;
	m_nOperateState = Op_Nothing;

	if(nLastOperateState == Op_Intelligent)
	{
		CTPMouseDeviceHooker* pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker();
		
		if(pHooker != NULL)
			pHooker->ExitHook();

	}
	else if(nLastOperateState == Op_Original){
		CTPSliderCtrl::OnCaptureChanged(pWnd);
	}
}



LRESULT CTPSliderCtrlEX::OnMouseDeviceHookCursorMove(WPARAM wParam, LPARAM lParam)
{
	MSG	msg;
	while(::PeekMessage(&msg, GetSafeHwnd(), WM_SE_MOUSEHOOK, WM_SE_MOUSEHOOK, PM_REMOVE));

	if(m_nOperateState == Op_Intelligent)
	{
		m_bRecived = TRUE;
		CTPMouseDeviceHooker* pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker();
		if(pHooker != NULL)
			CalcValue(pHooker->GetCurrentPosReal());
	}

	return 0;
}



int CTPSliderCtrlEX::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTPSliderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitIntelligentResource();
	

	return 0;
}



void CTPSliderCtrlEX::PreSubclassWindow() 
{
	CTPSliderCtrl::PreSubclassWindow();

	InitIntelligentResource();
}



void CTPSliderCtrlEX::CalcValue(const SEPOINTREAL& point)
{
	SEPOINTREAL ptDiff;
	ptDiff.x = m_ptPrevPoint.x - point.x;
	ptDiff.y = m_ptPrevPoint.y - point.y;

	m_ptPrevPoint = point;

	double dDiffVal = 0;

	if(GetExStyle() & TBS_VERT){
		dDiffVal = ptDiff.y;
	}
	else{
		dDiffVal = ptDiff.x;
	}

	if(dDiffVal == 0.0){
		return ;
	}

	m_dStackValue += dDiffVal;

	double dRange(0.0), dPer(0.0), dValue(m_dStartVal);

	dRange = (m_dValMax - m_dValMin);

	if(m_nPerWidth){
		dPer = dRange / m_nPerWidth;
	}

	dValue -= (m_dStackValue * dPer);


	if(SetValue(dValue)){
		::SendMessage(GetParent()->GetSafeHwnd(), WM_HSCROLL, MAKELONG(SB_THUMBTRACK, GetPos()), LPARAM(GetSafeHwnd()));
	}
}



BOOL CTPSliderCtrlEX::SetValue(double dValue,BOOL bForce)
{
	if(dValue > m_dValMax){
		dValue = m_dValMax;
	}
	if(dValue < m_dValMin){
		dValue = m_dValMin;
	}

	if(m_dValue == dValue && !bForce)
		return FALSE;

	m_dValue = dValue;

	SetPos(Value2Pos(m_dValue));
	return TRUE;
}

int CTPSliderCtrlEX::GetIntValue()
{
	return (int)(GetDoubleValue() + 0.5);
}



void CTPSliderCtrlEX::InitCalcVal()
{
	m_dStartVal = m_dValue;
	m_dStackValue = 0.0;
	m_bRecived = FALSE;
}



double CTPSliderCtrlEX::GetDoubleValue()
{
	
	if(m_nOperateState != Op_Intelligent)
	{
		m_dValue = Pos2Value(GetPos());
	}

	return m_dValue;
}



void CTPSliderCtrlEX::InitIntelligentResource()
{
	CRect rcChannel;
	GetChannelRect(&rcChannel);

	m_nPerWidth = rcChannel.Width();
}



BOOL CTPSliderCtrlEX::CanIntelligentOperation()
{

	if((m_dValMax - m_dValMin) == 0.0){
		return FALSE;
	}


	if(!m_nPerWidth){
		return FALSE;
	}

	return (m_nOperateType != CTPMouseCalc::SE_SLT_ORIGINAL);
}



void CTPSliderCtrlEX::SetSliderOperationType(int nType, double dCurValue)
{
	if(m_nOperateType != nType)
	{
		m_nOperateType = nType;
	}

	if(m_nOperateType){
		SetValue(dCurValue);
	}
}


void CTPSliderCtrlEX::SetValueRange(double dMin, double dMax)
{
	m_dValMin = dMin;
	m_dValMax = dMax;
}



int CTPSliderCtrlEX::Value2Pos(double dValue)
{
	double dRange = m_dValMax - m_dValMin;
	int nMax(0), nMin(0);
	GetRange(nMin, nMax);
	double dRange2 = nMax - nMin;

	if((dRange == 0.0 ) || (dRange2 == 0.0)){
		return 0;
	}

	double dPosReal = m_dValue / dRange;

	int nPos =0;
	if(dPosReal>0)
		nPos = (int)(dRange2 * dPosReal + 0.5);
	else
		nPos = (int)(dRange2 * dPosReal - 0.5);

	if(nPos < nMin){
		nPos = nMin;
	}
	if(nPos > nMax){
		nPos = nMax;
	}

	return nPos;
}


double CTPSliderCtrlEX::Pos2Value(int nPos)
{
	int nMax(0), nMin(0);
	GetRange(nMin, nMax);

	int nRange = nMax - nMin;
	double dRange = m_dValMax - m_dValMin;
	if(!nRange || (dRange == 0.0)){
		return 0.0;
	}

	double dPerVal = (double)nPos / nRange;
	double dValue = dPerVal * dRange;

	if(dValue > m_dValMax){
		dValue = m_dValMax;
	}
	if(dValue < m_dValMin){
		dValue = m_dValMin;
	}

	return dValue;
}

void    CTPSliderCtrlEX::SetStep(double dStep)
{
	m_dStep = dStep;
}

void    CTPSliderCtrlEX::SetPos(int iPos)
{
	CTPSliderCtrl::SetPos(iPos);
	m_dValue = Pos2Value(GetPos());
	
	InvalidateRect(NULL);//xl changed
}

void CTPSliderCtrlEX::InitSmartSlider(DWORD dwWinType)
{
	CTPMouseDeviceImpl::InstallMouseDeviceHooker(AfxGetMainWnd(),dwWinType);
}
void CTPSliderCtrlEX::UninitSmartSlider(DWORD dwWinType)
{
	CTPMouseDeviceImpl::UninstallMouseDeviceHooker(dwWinType);
}
void CTPSliderCtrlEX::OnKillFocus(CWnd* pNewWnd)
{
	CTPSliderCtrl::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}

BOOL CTPSliderCtrlEX::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	cs.lpszClass = WC_SLIDERCTRLEX;
	return CTPSliderCtrl::PreCreateWindow(cs);
}

BOOL CTPSliderCtrlEX::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_CONTROL)
		{
			CTPMouseDeviceHooker* pHooker = NULL;
			pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker();
			if(pHooker)	
				pHooker->SetCtrl(TRUE);
		}
	}
	if(pMsg->message == WM_KEYUP||pMsg->message == WM_RBUTTONDOWN)
	{
		//if(pMsg->wParam == VK_CONTROL)
		//{
			CTPMouseDeviceHooker* pHooker = NULL;
			pHooker = CTPMouseDeviceImpl::GetMouseDeviceHooker();
			if(pHooker)
				pHooker->SetCtrl(FALSE);
		//}
	}

	return FALSE;
}
