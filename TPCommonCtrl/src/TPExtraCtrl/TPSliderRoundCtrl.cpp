// TPSliderRoundCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPSliderRoundCtrl.h"
#include ".\tpsliderroundctrl.h"
#include <math.h>
#include <cmath>


extern double TP_NS_StepDB[];
extern double TP_NS_StepTick[];	

// CTPSliderRoundCtrl

IMPLEMENT_DYNAMIC(CTPSliderRoundCtrl, CTPSliderBar)
CTPSliderRoundCtrl::CTPSliderRoundCtrl()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_iStartAngle = -45;
	m_iEndAngle   = 240; 
	m_iMin        = 0;
	m_iMax        = 100000;

	m_iRadius     = 10;
	m_iThumbLen   = 2;
	m_ptCenter    = CPoint(10,10);

	m_rtCircle    = CRect(0,0,0,0);

	//m_pResCursor = TP_LoadCursor(TP_GetSkinPath() + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorhandle.cur"));
	
	m_iRoundType = 1;
	m_iThumbType = 2;

	m_iEscPos     = 0;
	m_bEsc        = FALSE;
	m_dAngle      = -10000;
}

CTPSliderRoundCtrl::~CTPSliderRoundCtrl()
{
	//TP_ReleaseCommonRes(m_pResCursor);
}


BEGIN_MESSAGE_MAP(CTPSliderRoundCtrl, CTPSliderBar)

  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_WM_SETCURSOR()
  ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0XFFFFFFF,       OnToolTipNotify )

//  ON_WM_TIMER()
  ON_WM_CAPTURECHANGED()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_MOUSEWHEEL()
  ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void    CTPSliderRoundCtrl::SubWindow(BOOL bSub)
{	
	if(bSub) 
	{
		CTPSliderBar::SubWindow(bSub);
		SetStepValue(TP_NS_StepDB,11,TP_NS_StepTick);		
	}
	else CTPSliderBar::SubWindow(bSub);
}
void    CTPSliderRoundCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_SLIDERBAR,&wndClass);		
		wndClass.lpszClassName = WC_SLIDERROUNDCTRL;
		wndClass.style        |= CS_DBLCLKS;
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_SLIDERROUNDCTRL,AfxGetInstanceHandle());
}
void   CTPSliderRoundCtrl::SetResType(int iType)
{
	CString sPath = TP_GetSkinPath(),sBack;
	m_iRoundType     = iType;
	if(m_iRoundType == 1)
	{	
		sPath += _T("\\CommonCtrl\\SliderRoundCtrl\\Round2");	
		for(int l=0;l<2;l++) TP_ReleaseCommonRes(m_pThumbBmp[l]);

		sBack.Format(_T("\\SliderBack%d.bmp"),m_iRoundType);

		m_pThumbBmp[0] = TP_LoadBmpByte(sPath + sBack);
		m_pThumbBmp[1] = TP_LoadImage(sPath +_L("\\SliderTick.bmp"),35,RGB(255,0,255));

		m_rtCircle   = CRect(0,0,m_pThumbBmp[0]->Width(),m_pThumbBmp[0]->Height());
		m_ptCenter = m_rtCircle.CenterPoint();
		m_iRadius  = m_pThumbBmp[1]->Width()/2; 
	}
	else  if(m_iRoundType == 2)
	{
		sPath += _T("\\CommonCtrl\\SliderRoundCtrl\\Round3\\");	
		for(int l=0;l<2;l++) TP_ReleaseCommonRes(m_pThumbBmp[l]);
		m_pThumbBmp[0] = TP_LoadBmpByte(sPath + _T("AudioRound1.bmp"));
		sBack.Format(_T("\\AudioRound%d.bmp"),m_iThumbType == 1? 2:m_iThumbType);		
		m_pThumbBmp[1] = TP_LoadBmpByte(sPath + sBack);
		m_rtCircle   = CRect(0,0,m_pThumbBmp[0]->Width(),m_pThumbBmp[0]->Height());
		m_rtThumb= m_rtCircle;
		m_rtThumb.right  = m_rtThumb.left + m_pThumbBmp[1]->Width();
		m_rtThumb.bottom = m_rtThumb.top + m_pThumbBmp[1]->Height();
		m_rtThumb.OffsetRect(9,9);		 
		m_ptCenter = m_rtThumb.CenterPoint();
		m_iRadius  = m_pThumbBmp[1]->Width()/2; 
	}
	else if(m_iRoundType == 3)//add by lzq:SliderRound的新的显示方式，0为起点，左负，右正。 XN00012791 2009.05.31
	{				
		sPath += _T("\\CommonCtrl\\SliderRoundCtrl\\Round2");	
		for(int l=0;l<2;l++) TP_ReleaseCommonRes(m_pThumbBmp[l]);

		sBack.Format(_T("\\SliderBack%d.bmp"),m_iRoundType);

		m_pThumbBmp[0] = TP_LoadBmpByte(sPath + sBack);
		m_pThumbBmp[1] = TP_LoadImage(sPath +_L("\\SliderTickEx.bmp"),35,RGB(255,0,255));

		m_rtCircle   = CRect(0,0,m_pThumbBmp[0]->Width(),m_pThumbBmp[0]->Height());
		m_ptCenter = m_rtCircle.CenterPoint();
		m_iRadius  = m_pThumbBmp[1]->Width()/2;
	}
}

void   CTPSliderRoundCtrl::LoadThumb()
{
	SetResType(m_iRoundType);
}

void   CTPSliderRoundCtrl::SetBackRes(CString sResPath) //add by pyc 2007-7-25
{
	if(m_pThumbBmp[0])
	{
		TP_ReleaseCommonRes(m_pThumbBmp[0]);
		m_pThumbBmp[0] = NULL;
	}
	m_pThumbBmp[0] = TP_LoadBmpByte(sResPath);
}
//0x50010000
void   CTPSliderRoundCtrl::PaintCtrl(CDC *pDC)
{
	CTPSliderBar::GetRange(m_iMin,m_iMax);
	m_iPos   = CTPSliderBar::GetPos();
	if(m_iRoundType ==1)
	{	
		TP_SetDIBitsToDevice(pDC,m_pThumbBmp[0]->GetByte(),m_rtCircle.Width(),m_rtCircle.Height(),m_rtCircle);
		int iPos = 0;
		if(m_iPos >= m_iMax) iPos = 30;
		else iPos = (m_iPos - m_iMin)*31/(m_iMax - m_iMin);		
		if(iPos>0) ImageList_Draw(m_pThumbBmp[1] ->GetImage(),iPos-1,pDC->m_hDC,m_rtCircle.left+11,m_rtCircle.top+11,ILD_TRANSPARENT);	

	}
	else if(m_iRoundType ==2)
	{
		TP_SetDIBitsToDevice(pDC,m_pThumbBmp[0]->GetByte(),m_rtCircle.Width(),m_rtCircle.Height(),m_rtCircle);
		if(m_iThumbType>=2)	TP_SetDIBitsToDevice(pDC,m_pThumbBmp[1]->GetByte(),m_pThumbBmp[1]->Width(),m_pThumbBmp[1]->Height(),m_rtThumb);
		CPoint ptPos =  PosToPoint(m_iPos);	
		pDC ->FillSolidRect(ptPos.x,ptPos.y,2,2,RGB(255,255,255));
	}
	else if(m_iRoundType == 3)//add by lzq:SliderRound的新的显示方式，0为起点，左负，右正。 XN00012791 2009.05.31
	{	
		TP_SetDIBitsToDevice(pDC,m_pThumbBmp[0]->GetByte(),m_rtCircle.Width(),m_rtCircle.Height(),m_rtCircle);
		int iPos = 0;
		if(m_iPos >= m_iMax) iPos = 30;
		else iPos = (m_iPos - m_iMin)*31/(m_iMax - m_iMin);			
		if(iPos>=0) ImageList_Draw(m_pThumbBmp[1] ->GetImage(),max(0,iPos-1),pDC->m_hDC,m_rtCircle.left+11,m_rtCircle.top+11,ILD_TRANSPARENT);	
	}
}
//                   |
//        90         |       180  
//---------------------------------------------
//       -45         |       270 
//                   |   
//
double CTPSliderRoundCtrl::PointToAngle(CPoint pt)
{
	CPoint ptCenter = m_ptCenter;
	double  dAngle = 0;
	if(pt.x - ptCenter.x == 0)
	{
		if(pt.y - ptCenter.y <=0) dAngle = 90;
		else                      dAngle = 270;
	}
	else if(pt.y - ptCenter.y == 0)
	{
		if(pt.x - ptCenter.x >0) dAngle = 180;
		else                     dAngle = 0;
	}
	else
	{
		dAngle  = atan(fabs((pt.y - ptCenter.y)*1.0/(pt.x - ptCenter.x))) * 180/3.14;
		if(pt.y - ptCenter.y <0      && pt.x - ptCenter.x >0)       dAngle = 180-dAngle;//1
		else if(pt.y - ptCenter.y <0 && pt.x - ptCenter.x <0)       dAngle = dAngle;    //2
		else if(pt.y - ptCenter.y >0 && pt.x - ptCenter.x <0)       dAngle = -dAngle;   //3
		else if(pt.y - ptCenter.y >0 && pt.x - ptCenter.x >0)       dAngle = 180+dAngle;//4
	}
	return dAngle;
}
void CTPSliderRoundCtrl::AngleToPoint(double dAngle,double &dx,double &dy)
{
	//int     iLengtjh = m_iRadius;
	//CPoint  ptCenter = m_ptCenter;
	//double  dAngleEx = 0;
	//CPoint ptGet = CPoint(0,0);

	//if(dAngle <=0)        dAngleEx = 180-dAngle;//3
	//else if(dAngle <=90)  dAngleEx = 180-dAngle;//2
	//else if(dAngle <=180) dAngleEx = 180-dAngle;//1
	//else if(dAngle <=270) dAngleEx = 180-dAngle;//4

	//ptGet.x      = int(fabs(iLengtjh * cos(dAngleEx *3.14/180)));
	//ptGet.y      = int(fabs(iLengtjh * sin(dAngleEx*3.14/180)));
	//if(dAngle <=0)        {ptGet.x  = ptCenter.x - ptGet.x ;	ptGet.y  = ptCenter.y + ptGet.y;}
	//else if(dAngle <=90)  {ptGet.x  = ptCenter.x - ptGet.x ;	ptGet.y  = ptCenter.y - ptGet.y;}
	//else if(dAngle <=180) {ptGet.x  = ptCenter.x + ptGet.x ;	ptGet.y  = ptCenter.y - ptGet.y;}
	//else if(dAngle <=270) {ptGet.x  = ptCenter.x + ptGet.x ;	ptGet.y  = ptCenter.y + ptGet.y;}	
	//return ptGet;
}
CPoint CTPSliderRoundCtrl::AngleToPoint(double dAngle)
{
	int     iLengtjh = m_iRadius;
	CPoint  ptCenter = m_ptCenter;
	double  dAngleEx = 0;
	CPoint ptGet = CPoint(0,0);

	if(dAngle <=0)        dAngleEx = 180-dAngle;//3
	else if(dAngle <=90)  dAngleEx = 180-dAngle;//2
	else if(dAngle <=180) dAngleEx = 180-dAngle;//1
	else if(dAngle <=270) dAngleEx = 180-dAngle;//4

	ptGet.x      = int(fabs(iLengtjh * cos(dAngleEx *3.14/180)));
	ptGet.y      = int(fabs(iLengtjh * sin(dAngleEx*3.14/180)));
	if(dAngle <=0)        {ptGet.x  = ptCenter.x - ptGet.x ;	ptGet.y  = ptCenter.y + ptGet.y;}
	else if(dAngle <=90)  {ptGet.x  = ptCenter.x - ptGet.x ;	ptGet.y  = ptCenter.y - ptGet.y;}
	else if(dAngle <=180) {ptGet.x  = ptCenter.x + ptGet.x ;	ptGet.y  = ptCenter.y - ptGet.y;}
	else if(dAngle <=270) {ptGet.x  = ptCenter.x + ptGet.x ;	ptGet.y  = ptCenter.y + ptGet.y;}	
	return ptGet;
}
double CTPSliderRoundCtrl::PointToPos(CPoint pt)
{
	double  dAngle =  PointToAngle(pt);	
	int     iPos    = 0; 
	if(dAngle>=m_iStartAngle && dAngle<= m_iEndAngle)
	{	
		iPos =  (int)AngleToPos(dAngle);
		iPos = min(m_iMax,iPos);
		iPos = max(m_iMin,iPos);	
	}
	else if(dAngle<m_iStartAngle)//DQAdb00008018
		iPos = m_iMin;
	else if(dAngle> m_iEndAngle)
		iPos = m_iMax;
	else
	{
		if(m_iPos -m_iMin> m_iMax - m_iPos) iPos = m_iMax;
		else                                iPos = m_iMin;
	}
	return iPos;	
}
double  CTPSliderRoundCtrl::PosToAngle(double iPos)
{
	return (iPos - m_iMin) * (m_iEndAngle - m_iStartAngle)*1.0/(m_iMax - m_iMin) + m_iStartAngle;
}
double     CTPSliderRoundCtrl::AngleToPos(double dAngle)
{
	return (dAngle -m_iStartAngle)* (m_iMax - m_iMin)*1.0/(m_iEndAngle - m_iStartAngle) + m_iMin;
}

CPoint CTPSliderRoundCtrl::PosToPoint(double iPos)
{
	return AngleToPoint(PosToAngle(iPos));	
}

void CTPSliderRoundCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
		if(GetFocus()!=this) SetFocus();
		m_iEscPos        = m_iPos;
		m_bEsc           = FALSE;
		int     iLengtjh = m_iRadius;
		CPoint  ptCenter = m_ptCenter;
		double  dLength  = sqrt(pow(double(point.x - ptCenter.x),2) + pow(double(point.y - ptCenter.y),2));
		
		if(m_iRoundType == 1 && dLength <m_iRadius/2)		return;
		else if(m_iRoundType == 2 && dLength <m_iRadius)	return;
		else if(m_iRoundType == 3 && dLength <m_iRadius/2)	return;//add by lzq:SliderRound的新的显示方式，0为起点，左负，右正。 XN00012791 2009.05.31

		double  dAngle   = PointToAngle(point);
		int iPos = (int)PointToPos(point);
		if(iPos != m_iPos)
		{
			double dStep = (iPos - m_iPos)/10.0;
			for(INT l=0;l<10;l++)
			{
				SetPos(int(iPos - dStep *(9-l)));
				Invalidate(FALSE);
				UpdateWindow();
				::Sleep(20);
			}
			SetPos(iPos);
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)iPos),(LPARAM)m_hWnd);						
		}		
		m_dAngle = PosToAngle(m_iPos);
		SetCapture();
		m_bPressed  = TRUE;
		m_iDragMode = TP_DRAG_MIDDLE;	
		m_ptDragSave = point;	
		Invalidate(FALSE);
		//SetCursor(m_pResCursor ->GetCursor());	
}

void CTPSliderRoundCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{	
	m_bPressed = FALSE;
	m_dAngle   = -10000;
	if(m_iDragMode == TP_DRAG_MIDDLEOK || m_iDragMode == TP_DRAG_MIDDLE)
	{
		if(m_iDragMode == TP_DRAG_MIDDLEOK && !m_bEsc)
		{	
			if(m_ptDragSave.x != point.x)		
			{
				int	iDragPos = (int)PointToPos(point);

				if(iDragPos != m_iPos)
				{
					SetPos(iDragPos);		
					m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,(WORD)iDragPos),(LPARAM)m_hWnd);					
					Invalidate(FALSE);
				}
			}		
			m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD)0),(LPARAM)m_hWnd);							
		}
		m_iDragMode = TP_DRAG_UNKNOW;
		ReleaseCapture();
		SetCursor(TP_GetCtrlCursor());
		Invalidate();
	}	
}
void CTPSliderRoundCtrl::SetCursorPosition(CPoint &point)
{		
	point = AngleToPoint(m_dAngle);		
	ClientToScreen(&point);
	SetCursorPos(point.x,point.y);
	ScreenToClient(&point);
}
int CTPSliderRoundCtrl::GetMicroSetp(int iDragPos,CPoint &ptCur)
{	
	BOOL bAdd  = FALSE;
	CPoint ptCenter = m_ptCenter;
	m_ptCenter      = m_ptDrag;
	double  dAngle  = PointToAngle(ptCur);
	m_ptCenter      = ptCenter;
	if(m_dAngle<=90)
	{
		if(dAngle>=0 && dAngle<=180) bAdd = TRUE;
		else                         bAdd = FALSE;
	}
	else 
	{
		if(dAngle>=90) bAdd = TRUE;
		else           bAdd = FALSE;
	}
	
	if(!bAdd)    dAngle = m_dAngle - 1;	
	else         dAngle = m_dAngle + 1;		
	iDragPos = (int)AngleToPos(dAngle);
	iDragPos = min(m_iMax,iDragPos);
	iDragPos = max(m_iMin,iDragPos);		
	m_dAngle = dAngle;
	return iDragPos;
}
void CTPSliderRoundCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_iDragMode == TP_DRAG_MIDDLE || m_iDragMode == TP_DRAG_MIDDLEOK)
	{
		TP_RemoveMessage(m_hWnd,WM_MOUSEMOVE);
		BOOL bCtrl  = FALSE;//TP_IsCtrlKeyDown();
		m_iDragMode = TP_DRAG_MIDDLEOK;
		if(m_ptDragSave != point && !m_bEsc)		
		{
			m_iPos       = GetPos();
			int iDragPos   = (int)PointToPos(point);			
			int iDragPosEx = iDragPos;
			if(bCtrl) 	
			{
				TP_RemoveMessage(m_hWnd,WM_MOUSEMOVE);
				iDragPos = CTPSliderRoundCtrl::GetMicroSetp(iDragPos,point);
				if(iDragPos != iDragPosEx)	CTPSliderRoundCtrl::SetCursorPosition(point);
			}
			if(iDragPos != m_iPos)
			{						
				SetPos(iDragPos);				
				m_pMsgWnd->PostMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)iDragPos),(LPARAM)m_hWnd);						
			}
			m_ptDrag     = point;
			m_ptDragSave = point;
			Invalidate();
		}
	}	
}




double  CTPSliderRoundCtrl::GetDB()
{
	return 	DBAlign(PosToDB(CTPSliderBar::GetPos()),m_iDimLen);
}
void  CTPSliderRoundCtrl::SetDB(double dDB,BOOL bComplex)
{
	if(bComplex)	CTPSliderBar::SetPos(int(DBToPosComplex(dDB)));
	else 	        CTPSliderBar::SetPos(int(DBToPos(dDB)));
    InvalidateRect(NULL);//added by swx
}
BOOL CTPSliderRoundCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	/*	CPoint ptCursor; 
	CRect  rtHandle = GetHandleRect();
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	if(rtHandle.PtInRect(ptCursor)) { SetCursor(m_pResCursor ->GetCursor()); return TRUE ;}
	else*/ return CTPSliderBar::OnSetCursor(pWnd, nHitTest, message);
}
BOOL CTPSliderRoundCtrl::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{	
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	CString sText;
	sText.Format(_L("%d"),PointToPos(ptCursor));		
	lstrcpy( pTTT->szText,sText);
	return  TRUE;		
} 

void CTPSliderRoundCtrl::OnCaptureChanged(CWnd *pWnd)
{	
	if(m_iDragMode != TP_DRAG_UNKNOW)
	{	
		m_iDragMode = TP_DRAG_UNKNOW;	
		SetCursor(TP_GetCtrlCursor());
	}
	CTPSliderBar::OnCaptureChanged(pWnd);
}

BOOL CTPSliderRoundCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE && m_bPressed && !m_bEsc)
	{
		SetPos(m_iEscPos);
		m_bEsc = TRUE;
		m_pMsgWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_ENDSCROLL,(WORD)0),(LPARAM)m_hWnd);	
		return TRUE;
	}
	return CTPSliderBar::PreTranslateMessage(pMsg);
}

void CTPSliderRoundCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	GetParent() ->PostMessage(WM_SLIDERCTRLMSG,WM_SLIDERCTRL_LBD,LPARAM(m_hWnd));	
	CTPSliderBar::OnLButtonDblClk(nFlags, point);
}

BOOL CTPSliderRoundCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_iMicType == 1)
	{	
		double  dDBDrag = GetDB();
		double  sStep   = 0.01;

		if(abs(dDBDrag)>10)  sStep    = 0.1;
		else                 sStep    = 0.01;
		if(zDelta<0)         dDBDrag -=  sStep;
		else                 dDBDrag +=  sStep;
		SetPos((int)DBToPosComplex(dDBDrag));
	}
	else if(m_iMicType == 2)
	{
		double  sStep   = 0.0001;	
		double  dDBDrag = GetDB();
		sStep = 0.1;		
		if(zDelta<0) dDBDrag -=  sStep;
		else         dDBDrag +=  sStep;
		SetPos((int)DBToPosComplex(dDBDrag));
	}
	else
	{
		int iPos = GetPos();
		if(zDelta<0) iPos -=  (m_iMax - m_iMin)/1000;
		else         iPos +=  (m_iMax - m_iMin)/1000;
		SetPos(iPos);
	}
	InvalidateRect(NULL);
	m_pMsgWnd->PostMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,(WORD)GetPos()),(LPARAM)m_hWnd);	
	//return CTPSliderBar::OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}


//add by lzq:bug:XN00012138,2009.05.09
void CTPSliderRoundCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	GetParent()->SendMessage(WM_BUTTON,TP_BUTTON_RDOWN,(LPARAM)m_hWnd);

	CTPSliderBar::OnRButtonDown(nFlags, point);
}
