// TPTLDropTarget.cpp : implementation file
//

#include "stdafx.h"

#include "TPOleDropTarget.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CTPOleDropTarget

IMPLEMENT_DYNAMIC(CTPOleDropTarget, COleDropTarget)
CTPOleDropTarget::CTPOleDropTarget()
{
	m_pMsgWnd = NULL;

	m_stuOleDropData.dwKeyState   = 0;
	m_stuOleDropData.pDataObject  = NULL;
	m_stuOleDropData.point        = CPoint(0,0);
	m_stuOleDropData.pWnd         = NULL;
	m_stuOleDropData.eRetrun      = DROPEFFECT_NONE;
}
CTPOleDropTarget::~CTPOleDropTarget()
{
}


BEGIN_MESSAGE_MAP(CTPOleDropTarget, COleDropTarget)
END_MESSAGE_MAP()

DROPEFFECT CTPOleDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,DWORD dwKeyState, CPoint point)
{	
	m_stuOleDropData.dwKeyState   = dwKeyState;
	m_stuOleDropData.pDataObject  = pDataObject;
	m_stuOleDropData.point        = point;
	m_stuOleDropData.pWnd         = pWnd;
	if(pWnd == m_pMsgWnd && m_pMsgWnd)
	{
		m_pMsgWnd->SendMessage(WM_OLEDROPTARGET_MSG,0,(LPARAM)&m_stuOleDropData);
		return m_stuOleDropData.eRetrun;
	}
	else return DROPEFFECT_NONE;
}
DROPEFFECT CTPOleDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,DWORD dwKeyState, CPoint point)
{
	m_stuOleDropData.dwKeyState   = dwKeyState;
	m_stuOleDropData.pDataObject  = pDataObject;
	m_stuOleDropData.point        = point;
	m_stuOleDropData.pWnd         = pWnd;
	if(pWnd == m_pMsgWnd && m_pMsgWnd)
	{
		m_pMsgWnd->SendMessage(WM_OLEDROPTARGET_MSG,1,(LPARAM)&m_stuOleDropData);
		return m_stuOleDropData.eRetrun;
	}
	else return DROPEFFECT_NONE;
}
BOOL CTPOleDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,DROPEFFECT dropEffect, CPoint point)
{
	m_stuOleDropData.dwKeyState   = dropEffect;
	m_stuOleDropData.pDataObject  = pDataObject;
	m_stuOleDropData.point        = point;
	m_stuOleDropData.pWnd         = pWnd;
	if(pWnd == m_pMsgWnd && m_pMsgWnd)
	{
		m_pMsgWnd->SendMessage(WM_OLEDROPTARGET_MSG,2,(LPARAM)&m_stuOleDropData);
		return m_stuOleDropData.eRetrun;
	}
	else return DROPEFFECT_NONE;
}
void CTPOleDropTarget::OnDragLeave(CWnd* pWnd)
{	
	m_stuOleDropData.dwKeyState   = 0;
	m_stuOleDropData.pDataObject  = NULL;
	m_stuOleDropData.point        = CPoint(0,0);
	m_stuOleDropData.pWnd         = pWnd;
	if(m_pMsgWnd)
	{
		m_pMsgWnd->SendMessage(WM_OLEDROPTARGET_MSG,3,(LPARAM)&m_stuOleDropData);		
	}
}
BOOL TPOleDropData::CheckType(UINT uType,LPCTSTR sType)
{
	if(pDataObject == NULL) return FALSE;
	if(CF_HDROP == uType)
	{
		return pDataObject->IsDataAvailable(CF_HDROP,NULL);
	}
	else
	{
		if(!pDataObject->IsDataAvailable(uType,NULL)) return FALSE;
		if(lstrlen(sType)>0)
		{		
			HGLOBAL hGlobal   =   pDataObject->GetGlobalData(uType,NULL);;
			LPBYTE lpData     = (LPBYTE)GlobalLock(hGlobal);	
			CString      sFileName = _T("");
			sFileName  = (LPCTSTR)(LPTSTR)lpData;	
			GlobalUnlock(hGlobal);			
			if(sFileName.Find(sType)>=0) return TRUE;
			else                         return FALSE;
		}
		else return TRUE;		
	}
	return FALSE;
}
// CTPOleDropTarget message handlers
