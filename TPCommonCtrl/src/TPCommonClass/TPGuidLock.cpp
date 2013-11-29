#include "StdAfx.h"
#include ".\tpguidlock.h"
#include "TPCommonBaseData.h"

CString TP_GuidToStr(GUID guidLock,int iIndex)
{
	CString sText;
	sText.Format(_T("%x-%x-%x-%x%x%x%x%x%x%x%x%d"),guidLock.Data1,guidLock.Data2,guidLock.Data3,guidLock.Data4[0],guidLock.Data4[1],guidLock.Data4[2],guidLock.Data4[3],guidLock.Data4[4],guidLock.Data4[5],guidLock.Data4[6],guidLock.Data4[7],iIndex);
	return sText;
}
CString TP_DWORDPTRToStr(DWORD_PTR dwLock,int iIndex)
{
	CString sText;
	sText.Format(_T("%d-%d"),(int)dwLock,iIndex);
	return sText;
}
CTPGuidLock::CTPGuidLock(GUID guidLock,int iIndex,BOOL bLock):
CMutex(FALSE,TP_GuidToStr(guidLock,iIndex),NULL)
{	
	if(bLock)
	{	
		//while(!CMutex::Lock(200)) TP_TransMessage(WM_DELETERES);
		CMutex::Lock();
		m_bLock = TRUE;
	}
	else
	{
		m_bLock = FALSE;
	}
#ifdef _DEBUG
	m_sLockName = m_strName;
#endif
}
CTPGuidLock::CTPGuidLock(DWORD_PTR dwLock,int iIndex ,BOOL bLock)
:CMutex(FALSE,TP_DWORDPTRToStr(dwLock,iIndex),NULL)
{
	if(bLock)
	{	
		//while(!CMutex::Lock(200)) TP_TransMessage(WM_DELETERES);
		CMutex::Lock();
		m_bLock = TRUE;
	}
	else
	{
		m_bLock = FALSE;
	}
#ifdef _DEBUG
	m_sLockName = m_strName;
#endif
}
BOOL CTPGuidLock::Lock(DWORD dwTimeout)
{
	BOOL bReturn = WAIT_OBJECT_0;
	if(!m_bLock)	bReturn = CMutex::Lock(dwTimeout);
	m_bLock = TRUE;
	return bReturn ;
}
BOOL CTPGuidLock::Unlock()
{
	if(m_bLock)	CMutex::Unlock();
	m_bLock = FALSE;
	return TRUE;
}
CTPGuidLock::~CTPGuidLock(void)
{
	if(m_bLock)	CMutex::Unlock();
	m_bLock = FALSE;
}
