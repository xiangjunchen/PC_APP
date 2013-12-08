#include "StdAfx.h"

CTPMemFile::CTPMemFile(void)
{
}

CTPMemFile::~CTPMemFile(void)
{
}

LRESULT CTPMemFile::ReadFile(LPCTSTR sFileName)
{
	CFile    cRead;
	CFileException  eFail;
	if(!cRead.Open(sFileName,CFile::modeRead,&eFail))
	{
		return S_FALSE;
	}
	try
	{
		long lReadSize = cRead.GetLength();
		BYTE *pBuf = new BYTE[lReadSize];
		DWORD nSize = cRead.Read(pBuf,lReadSize);
 		Attach(pBuf,lReadSize,0);
	}
	catch (...)
	{
		ASSERT(0);
	}	
	cRead.Close();

	return S_OK;
}
LRESULT CTPMemFile::WriteFile(LPCTSTR sFileName)
{
	CFile cFile;
	UINT    nLong       = m_nPosition;
	LPBYTE pBufferRet   = m_lpBuffer;
	if(!cFile.Open(sFileName, CFile::modeCreate | CFile::modeWrite))
	{
		return S_FALSE;
	}
	try
	{
		cFile.SeekToBegin();
		cFile.Write(pBufferRet, nLong);
		cFile.Flush();
	}
	catch (...)
	{
		ASSERT(0);
	}	
	cFile.Close();
	return S_OK;
}
void CTPMemFile::Close()
{
	LPBYTE lpByte = Detach();
	if(lpByte)
	{
		delete lpByte;
		lpByte = NULL;
	}
	CMemFile::Close();
}

CTPMapStringToString::CTPMapStringToString()
{ 
	InitializeCriticalSection(&m_pCriticalSection);
}
CTPMapStringToString::~CTPMapStringToString()
{
	DeleteCriticalSection(&m_pCriticalSection);
}
void CTPMapStringToString::Lock()
{
	EnterCriticalSection(&m_pCriticalSection);
}
void CTPMapStringToString::Unlock()
{
	LeaveCriticalSection(&m_pCriticalSection);
}
BOOL CTPMapStringToString::Lookup(LPCTSTR key, CString& rValue) 
{
	EnterCriticalSection(&m_pCriticalSection);
	BOOL bReturn = CMapStringToString::Lookup(key,rValue);
	LeaveCriticalSection(&m_pCriticalSection);
	return bReturn;
}
void CTPMapStringToString::SetAt(LPCTSTR key, LPCTSTR newValue)
{
	EnterCriticalSection(&m_pCriticalSection);
	CMapStringToString::SetAt(key,newValue);
	LeaveCriticalSection(&m_pCriticalSection);	 
}
void CTPMapStringToString::SetAtEx(LPCTSTR key, LPCTSTR newValue)
{
	CMapStringToString::SetAt(key,newValue);
}
BOOL CTPMapStringToString::RemoveKey(LPCTSTR key)
{
	EnterCriticalSection(&m_pCriticalSection);
	BOOL bReturn = CMapStringToString::RemoveKey(key);
	LeaveCriticalSection(&m_pCriticalSection);
	return bReturn;
}
void CTPMapStringToString::RemoveAll()
{
	EnterCriticalSection(&m_pCriticalSection);
	CMapStringToString::RemoveAll();
	LeaveCriticalSection(&m_pCriticalSection);
}
INT_PTR  CTPMapStringToString::GetSize()
{
	EnterCriticalSection(&m_pCriticalSection);
	INT_PTR iReturn = CMapStringToString::GetSize();
	LeaveCriticalSection(&m_pCriticalSection);
	return iReturn;
}