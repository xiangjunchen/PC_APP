#pragma once
#include "afxmt.h"

class AFX_EXT_CLASS CTPGuidLock :public CMutex
{
public:
	CTPGuidLock(GUID guidLock,int iIndex = 0,BOOL bLock = TRUE);
	CTPGuidLock(DWORD_PTR dwLock,int iIndex = 0,BOOL bLock = TRUE);
	~CTPGuidLock(void);
public:
	BOOL  Lock(DWORD dwTimeout = INFINITE);	
	BOOL  Unlock();	
	BOOL  IsLock() { return m_bLock; }
private:
	CString    m_sLockName;
	BOOL       m_bLock; 
};
