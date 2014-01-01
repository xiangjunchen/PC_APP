#include "StdAfx.h"

CString TP_GetSysPath()
{
	CString sPatch;
	TCHAR cPath[MAX_PATH];
	::GetModuleFileName(NULL,cPath,MAX_PATH);
	PathRemoveFileSpec(cPath);	
	sPatch  = cPath;
	sPatch  = sPatch.Left(sPatch.ReverseFind('\\'));
	sPatch += _T("\\Sys");
	return sPatch;
}
BOOL TP_DeleteDirectory(CString sPath,BOOL bRoot)
{
	CStringArray aPath;
	TP_FindFile(sPath,_L("\\*.*"), FALSE,FALSE,aPath);
	for(INT_PTR l=0;l<aPath.GetSize();l++)
	{
		if(!DeleteFile(aPath[l])) return FALSE;
	}
	aPath.RemoveAll();
	TP_FindFile(sPath,_L("\\*.*"), TRUE,FALSE,aPath);
	for(INT_PTR l=0;l<aPath.GetSize();l++) 
	{
		BOOL bReturn = TP_DeleteDirectory(aPath[l],TRUE);
		if(bReturn == FALSE) return FALSE;
	}	
	BOOL bReturn = TRUE;
	if(bRoot) bReturn = ::RemoveDirectory(sPath);	
	return bReturn;
}
void    File_FindFile(CString strPath,CString strFile,BOOL bPath,CStringArray &aFile)
{
	WIN32_FIND_DATA findData;
	HANDLE          hFind   = FindFirstFile(strPath + strFile ,&findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if(bPath && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')			
			aFile.Add(strPath + _L("\\") + findData.cFileName);
		else if(!bPath && !((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
			aFile.Add(strPath + _L("\\") + findData.cFileName);

		while(FindNextFile(hFind,&findData))
		{
			if(bPath && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
				aFile.Add(strPath + _L("\\") + findData.cFileName);
			else if(!bPath && !((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
				aFile.Add(strPath + _L("\\") + findData.cFileName);
		}
		FindClose(hFind);
	}
}
void   TP_FindFileOnly(CString strPath,CStringArray &aFile)
{
	WIN32_FIND_DATA findData;
	HANDLE          hFind   = FindFirstFile(strPath + _L("\\*.*") ,&findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if(!((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
			aFile.Add(strPath + _L("\\") + findData.cFileName);

		while(FindNextFile(hFind,&findData))
		{		
			if(!((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
				aFile.Add(strPath + _L("\\") + findData.cFileName);
		}
		FindClose(hFind);
	}
}

void   File_FindFile(CString strPath,CString sType,CStringArray &aFile)
{
	WIN32_FIND_DATA findData;
	HANDLE          hFind   = FindFirstFile(strPath + _L("\\*") + sType ,&findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
		{			
		}
		else if(!((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
			aFile.Add(strPath + _L("\\") + findData.cFileName);

		while(FindNextFile(hFind,&findData))
		{
			if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
			{		
			}
			else if(!((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
				aFile.Add(strPath + _L("\\") + findData.cFileName);
		}
		FindClose(hFind);
	}
}
BOOL TP_FindFile(CString& sFindPath)
{
	WIN32_FIND_DATA findData;
	HANDLE          hFind   = FindFirstFile(sFindPath,&findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return TRUE;
	}

	return FALSE;
}
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
		return S_FALSE;
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