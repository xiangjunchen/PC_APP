#include "StdAfx.h"

namespace _tp_commonclass
{
CString TP_GetWindowsSysPath(TP_WINDOWSPATH_TYPE eType)
{
	TCHAR MyDir[_MAX_PATH];   
	SHGetSpecialFolderPath(NULL,MyDir,eType,0); 
	return MyDir;
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
	HANDLE          hFind   = FindFirstFile(strPath + _L("\\*.") + sType ,&findData);
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
BOOL TP_GetStatus(LPCTSTR lpszFileName, CFileStatus& rStatus)
{
	ASSERT( lpszFileName != NULL );

	if ( lpszFileName == NULL ) 
	{
		return FALSE;
	}

	if ( lstrlen(lpszFileName) >= _MAX_PATH )
	{
		ASSERT(FALSE); // MFC requires paths with length < _MAX_PATH
		return FALSE;
	}

	// attempt to fully qualify path first
/*	if (!AfxFullPath(rStatus.m_szFullName, lpszFileName))
	{
		rStatus.m_szFullName[0] = '\0';
		return FALSE;
	}*/

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((LPTSTR)lpszFileName, &findFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return FALSE;
	VERIFY(FindClose(hFind));

	// strip attribute of NORMAL bit, our API doesn't have a "normal" bit.
	rStatus.m_attribute = (BYTE)
		(findFileData.dwFileAttributes & ~FILE_ATTRIBUTE_NORMAL);

	
	
	rStatus.m_size = (LONG)findFileData.nFileSizeLow;

	// convert times as appropriate
	rStatus.m_ctime = CTime(findFileData.ftCreationTime);
	if(findFileData.ftLastAccessTime.dwHighDateTime && findFileData.ftLastAccessTime.dwLowDateTime)  //XN00031663  虚拟盘没有accesstime，赋值一个初始值1970，QA那边赋值1601
	   rStatus.m_atime = CTime(findFileData.ftLastAccessTime);
	rStatus.m_mtime = CTime(findFileData.ftLastWriteTime);

	if (rStatus.m_ctime.GetTime() == 0)
		rStatus.m_ctime = rStatus.m_mtime;

	if (rStatus.m_atime.GetTime() == 0)
		rStatus.m_atime = rStatus.m_mtime;

	return TRUE;
}
}