// ControlsManager.cpp: implementation of the CTPSizeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TPSizeCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CTPSizeCtrl::CTPSizeCtrl()
{
	m_rtExtra   = CRect(0,0,0,0);
	m_hWnd      = NULL;
}
CTPSizeCtrl::~CTPSizeCtrl()
{
	m_arrControl.RemoveAll(); 
}

CRect   CTPSizeCtrl::CalcCtrlSize(TPSizeCtrlData &addData,CRect rtWnd, double fAmpX,double fAmpY)
{	
	CRect rtTemp = addData.rect,rtCtrl,rtSrcWnd = m_rtWnd;
	rtSrcWnd.right  += m_rtExtra.Width();
	rtSrcWnd.bottom += m_rtExtra.Height();
	rtTemp.left     += m_rtExtra.left;
	rtTemp.top      += m_rtExtra.top;	
	switch(addData.eModeX)
	{
	case TPSizeXNone:
		rtCtrl.left  = rtTemp.left ;
		rtCtrl.right = rtTemp.right;
		break;
	case TPSizeXA : 						   // 左或上对齐
		rtCtrl.left  = rtTemp.left ;
		rtCtrl.right = rtTemp.right;
		break;
	case TPSizeXB	   :				   // 右或下对齐
		rtCtrl.right = rtTemp.right + (rtWnd.Width() - rtSrcWnd.Width()) ;
		rtCtrl.left  = rtCtrl.right - rtTemp.Width(); 
		break;
	case TPSizeXAB   : 		       // 左右或上下对齐
		rtCtrl.right = rtTemp.right + (rtWnd.Width() - rtSrcWnd.Width()) ;
		rtCtrl.left  = rtTemp.left ; 
		break;
	case TPSizeXACB  :                  //居中
		rtCtrl.left  =  rtTemp.left + (rtWnd.Width() - rtSrcWnd.Width())/2; 
		rtCtrl.right  = rtCtrl.left + rtTemp.Width(); 
		break;
	case TPSizeXAF :                   //左或上放大
		rtCtrl.left  =  int(rtTemp.left * fAmpX); 
		rtCtrl.right  = rtCtrl.left + rtTemp.Width(); 
		break;
	case TPSizeXBF :                   //右或下放大
		rtCtrl.right  =  int(rtTemp.right * fAmpX); 
		rtCtrl.left   =  rtCtrl.right - rtTemp.Width(); 
		break;
	case TPSizeXAFBF:                //左或上和右下放大
		rtCtrl.right  =  int(rtTemp.right * fAmpX); 
		rtCtrl.left   =  int(rtTemp.left  * fAmpX); 
		break;
	case TPSizeXABF :                 //左或上对齐 右或下放大
		rtCtrl.left   =  rtTemp.left; 
		rtCtrl.right  =  int(rtTemp.right * fAmpX); 
		break;
	case TPSizeXAFB:             //左或上放大 右或下对齐
		rtCtrl.right  =  rtTemp.right+ (rtWnd.Width() - rtSrcWnd.Width()) ; 
		rtCtrl.left  =  int(rtTemp.left * fAmpX); 
		break;
	}
	switch(addData.eModeY)
	{
	case TPSizeYNone:
		rtCtrl.top  = rtTemp.top ;
		rtCtrl.bottom = rtTemp.bottom;
		break;
	case TPSizeYA : 						   // 左或上对齐
		rtCtrl.top  = rtTemp.top ;
		rtCtrl.bottom = rtTemp.bottom;
		break;
	case TPSizeYB	   :				   // 右或下对齐
		rtCtrl.bottom = rtTemp.bottom + (rtWnd.Height() - rtSrcWnd.Height()) ;
		rtCtrl.top  = rtCtrl.bottom - rtTemp.Height(); 
		break;
	case TPSizeYAB   : 		       // 左右或上下对齐
		rtCtrl.bottom = rtTemp.bottom + (rtWnd.Height() - rtSrcWnd.Height()) ;
		rtCtrl.top  = rtTemp.top ; 
		break;
	case TPSizeYACB  :                  //居中
		rtCtrl.top  =  rtTemp.top + (rtWnd.Height() - rtSrcWnd.Height())/2; 
		rtCtrl.bottom  = rtCtrl.top + rtTemp.Height(); 
		break;
	case TPSizeYAF :                   //左或上放大
		rtCtrl.top  =  int(rtTemp.top * fAmpY); 
		rtCtrl.bottom  = rtCtrl.top + rtTemp.Height(); 
		break;
	case TPSizeYBF :                   //右或下放大
		rtCtrl.bottom  =  int(rtTemp.bottom * fAmpY); 
		rtCtrl.top   =  rtCtrl.bottom - rtTemp.Height(); 
		break;
	case TPSizeYAFBF:                //左或上和右下放大
		rtCtrl.bottom  =  int(rtTemp.bottom * fAmpY); 
		rtCtrl.top   =  int(rtTemp.top  * fAmpY); 
		break;
	case TPSizeYABF :                 //左或上对齐 右或下放大
		rtCtrl.top   =  rtTemp.top; 
		rtCtrl.bottom  =  int(rtTemp.bottom * fAmpY); 
		break;
	case TPSizeYAFB:           //左或上放大 右或下对齐
		rtCtrl.bottom = rtTemp.bottom + (rtWnd.Height() - rtSrcWnd.Height()) ;
		rtCtrl.top  =  int(rtTemp.top * fAmpY); 
		break;
	}
	return rtCtrl;
}
CRect   CTPSizeCtrl::GetChildWindowRect(CWnd *pWnd)
{
	if(pWnd  == NULL)   return CRect(0,0,0,0);
	if(m_hWnd == NULL ) return CRect(0,0,0,0);
	CRect rtWnd;
	CWnd::FromHandle(m_hWnd) ->GetClientRect(&rtWnd); 
	
	double fAmpX =  rtWnd.Width()*1.0 / (m_rtWnd.Width() + m_rtExtra.Width());  
	double fAmpY =  rtWnd.Height()*1.0 / (m_rtWnd.Height() + m_rtExtra.Height());  
	for(int i =0;i<m_arrControl.GetSize();i++)
	{
		if(pWnd ->m_hWnd == m_arrControl[i].hWnd )
		{
			TPSizeCtrlData &addData = m_arrControl[i];
			if(pWnd ==NULL || !::IsWindow(pWnd->m_hWnd)) return CRect(0,0,0,0);
			else return CalcCtrlSize(addData,rtWnd,fAmpX,fAmpY);		
		}
	}
	return CRect(0,0,0,0);
}
void  CTPSizeCtrl::InitWindow(CWnd *pWnd)
{
	m_hWnd = pWnd ->m_hWnd;
	pWnd->GetClientRect(&m_rtWnd); 
	if(m_rtWnd.Width()<=0 ||m_rtWnd.Height()<=0 ) m_hWnd =NULL;
}
void  CTPSizeCtrl::OnSize(CSize *pszDef)
{
	CRect rtWnd;
	CWnd  *pWnd =NULL; 
	if(m_hWnd == NULL ) return;
	CWnd::FromHandle(m_hWnd)->GetClientRect(&rtWnd); 
	if(pszDef){rtWnd.right = rtWnd.left + pszDef->cx;rtWnd.bottom=pszDef->cy+rtWnd.top;}
	double fAmpX =  rtWnd.Width()*1.0 / (m_rtWnd.Width() + m_rtExtra.Width());  
	double fAmpY =  rtWnd.Height()*1.0 / (m_rtWnd.Height() + m_rtExtra.Height());  
	for(int i=0;i<m_arrControl.GetSize();i++)
	{
		if(m_arrControl[i].bEnable)
		{		
			TPSizeCtrlData &addData = m_arrControl[i];
			pWnd   = CWnd::FromHandle(addData.hWnd); 
			if(pWnd ==NULL || !::IsWindow(pWnd->m_hWnd)) continue;		
			pWnd->MoveWindow(CalcCtrlSize(addData,rtWnd,fAmpX,fAmpY)); 
			pWnd->Invalidate(FALSE);
		}
	}

}
void  CTPSizeCtrl::AddChildWindow(CWnd *pWnd,TPSizeMode eModeX,TPSizeMode eModeY)
{
	TPSizeCtrlData addData;
	if(pWnd == NULL ||m_hWnd == NULL ) return;
	addData.hWnd =  pWnd->m_hWnd;
	pWnd->GetWindowRect(&addData.rect); 
	CWnd::FromHandle(m_hWnd) ->ScreenToClient(&addData.rect); 
	addData.eModeX=eModeX;
	addData.eModeY=eModeY; 
	m_arrControl.Add(addData); 
}
void  CTPSizeCtrl::EnableChildWindow(CWnd *pWnd,BOOL bEnableSize)
{
	for(int i = (int)m_arrControl.GetSize()-1;i>=0;i--)
	{
		if(pWnd ->m_hWnd == m_arrControl[i].hWnd )
		{
			m_arrControl[i].bEnable = bEnableSize;
		}
	}
}
void  CTPSizeCtrl::SetExtraSize(int iExLeft,int iExTop,int iExRight,int iExBottom)
{
	m_rtExtra = CRect(iExLeft,iExTop,iExRight,iExBottom);
}
void CTPSizeCtrl::RemoveAllWnd()
{
	m_arrControl.RemoveAll(); 
}
void CTPSizeCtrl::RemoveWnd(CWnd *pWnd)
{
	for(int i = (int)m_arrControl.GetSize()-1;i>=0;i--)
	{
		if(pWnd ->m_hWnd == m_arrControl[i].hWnd )
		{
			m_arrControl.RemoveAt(i);
		}
	}
}
void CTPSizeCtrl::SetWndSizeMode(CWnd *pWnd,TPSizeMode eModeX,TPSizeMode eModeY)
{
	for(int i =0;i<m_arrControl.GetSize();i++)
	{
		if(pWnd ->m_hWnd == m_arrControl[i].hWnd )
		{
			m_arrControl[i].eModeX = eModeX;
			m_arrControl[i].eModeY = eModeY;
		}
	}
}
void CTPSizeCtrl::SetCotrolData(TPControlData *pCtrlData,int iCtrlSize)
{
	if(m_hWnd == NULL ) return;
	CWnd *pWnd = CWnd::FromHandle(m_hWnd);
	for(INT l=0;l<iCtrlSize;l++)
	{
		if(pCtrlData[l].hWnd == NULL) continue;
		TPSizeCtrlData addData;
		addData.hWnd =  pCtrlData[l].hWnd;
		CWnd::FromHandle(addData.hWnd)->GetWindowRect(&addData.rect); 
		pWnd ->ScreenToClient(&addData.rect); 
		addData.eModeX= TPSizeMode(pCtrlData[l].dwAlign & 0x0000ffff);
		addData.eModeY= TPSizeMode(pCtrlData[l].dwAlign & 0xffff0000); 
		m_arrControl.Add(addData); 
	}
}

