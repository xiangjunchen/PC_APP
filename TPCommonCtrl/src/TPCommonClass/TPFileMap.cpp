
#include "stdafx.h"
#include "TPFileMap.h"


CMapGuidToGuid::CMapGuidToGuid()
{
	m_bLoadData   = FALSE;
	m_sFileName   = _T("");
	m_bDataChange = FALSE;
}
CMapGuidToGuid::~CMapGuidToGuid()
{
	m_csLock.Lock();
	CMapStringToString::RemoveAll();
	m_csLock.Unlock();
	m_bLoadData   = FALSE;
	m_bDataChange = FALSE;
} 
void   CMapGuidToGuid::AddItem(GUID guidKey,GUID guidValue)
{
	m_csLock.Lock();
 	CString sGuid = GuidToString(&guidKey),sValue = GuidToString(&guidValue);
	CMapStringToString::SetAt(sGuid,sValue);
	m_csLock.Unlock(); 
	m_bDataChange = TRUE;
}
BOOL   CMapGuidToGuid::GetItem(GUID guidKey,GUID &guidValue)
{
	m_csLock.Lock();
	guidValue  = GUID_NULL;
	CString sGuid = GuidToString(&guidKey),sValue;
	if(!CMapStringToString::Lookup(sGuid,sValue)) 
	{
		m_csLock.Unlock();
		return FALSE;
	}
	guidValue = GuidFromString(sValue);
	m_csLock.Unlock();
	return TRUE;
}
void   CMapGuidToGuid::DelItem(GUID guidKey)
{
	m_csLock.Lock();
	CMapStringToString::RemoveKey(GuidToString(&guidKey));
	m_csLock.Unlock();
	m_bDataChange = TRUE;
}
void   CMapGuidToGuid::Clear()
{
	ASSERT(0);
	m_csLock.Lock();
	CMapStringToString::RemoveAll();
	m_csLock.Unlock();
}
void   CMapGuidToGuid::SaveFile(CString sFile,BOOL bClear)
{
	if(!m_bDataChange) return; 

	if(sFile.IsEmpty())  sFile = m_sFileName;
	CFile stuFile;
	if(!stuFile.Open(sFile,CFile::modeCreate|CFile::modeWrite)) return;
	CString   sKey,sValue;
	DWORD dwVersion = 1,dwLen1 = CMapStringToString::GetCount() ,dwLen2 = dwLen1+1;
	stuFile.SeekToBegin();

	stuFile.Write(&dwVersion,sizeof(DWORD));
	stuFile.Write(&dwLen1,sizeof(DWORD));
	stuFile.Write(&dwLen2,sizeof(DWORD));

	m_csLock.Lock();
	POSITION posGet = CMapStringToString::GetStartPosition();
	TCHAR chRead[40];
	ZeroMemory(chRead,sizeof(chRead));
	while(posGet)
	{
		CMapStringToString ::GetNextAssoc(posGet,sKey,sValue);	
		lstrcpyn(chRead,sKey,40);
		stuFile.Write(chRead,sizeof(chRead));
		lstrcpyn(chRead,sValue,40);
		stuFile.Write(chRead,sizeof(chRead));
	}
	m_csLock.Unlock();
	stuFile.Close();

	m_bDataChange = FALSE;
	if(bClear)
	{
		m_csLock.Lock();
		CMapStringToString::RemoveAll();
		m_csLock.Unlock();
		m_bLoadData   = FALSE;
	}
  }
BOOL   CMapGuidToGuid::LoadFile(CString sFileName)
{
	m_bLoadData   = TRUE;
	m_bDataChange = FALSE;
	m_sFileName   = sFileName;

	CFile stuFile;
	if(!stuFile.Open(sFileName,CFile::modeRead)) return FALSE;
	DWORD dwVersion,dwLen1,dwLen2;
	stuFile.Read(&dwVersion,sizeof(DWORD));
	stuFile.Read(&dwLen1,sizeof(DWORD));
	stuFile.Read(&dwLen2,sizeof(DWORD));
	if(dwLen1 != dwLen2 -1)
	{
		stuFile.Close();
		return FALSE;
	}
	m_csLock.Lock();
	CMapStringToString ::RemoveAll();
	TCHAR chRead[40];
	ZeroMemory(chRead,sizeof(chRead));
	CString   sKey,sValue;
	for(DWORD l=0;l<dwLen1;l++)
	{
		stuFile.Read(chRead,sizeof(chRead));
		sKey = chRead;
		stuFile.Read(chRead,sizeof(chRead));
		sValue = chRead;
		if(dwLen1 - l>100) continue;
		CMapStringToString::SetAt(sKey,sValue);
	}
	stuFile.Close();
	m_csLock.Unlock();
 	return TRUE;
}
CString	   CMapGuidToGuid::GuidToString( UUID *pUuid)
{
	CString   sTemp;
	unsigned short * pUuidString = NULL;
	RPC_STATUS ret = UuidToString( pUuid, &pUuidString);
	if( pUuidString ) 
	{
		sTemp= (LPTSTR) pUuidString ;
		RpcStringFree((unsigned short**) &pUuidString );
	} 
	else
		sTemp = _L("");
	return sTemp;
}
 GUID   CMapGuidToGuid::GuidFromString( LPCTSTR lpszUuid)
{
	GUID    guidGet = GUID_NULL;
	CString	strGuid = CString(lpszUuid);
	UuidFromString( (unsigned short*)((LPCTSTR)(strGuid.Left(36))), &guidGet );
	return guidGet;
}



CString   CTPFileMap::m_sPath  = _L("");
long      g_lTimeRate;

CTPFileMap::CTPFileMap()
{
	m_hFileHandle     = NULL;
	m_hHandle         = NULL;
	m_pMem            = NULL;
	m_sFileName       = _L("");
	m_bIsRead         = FALSE;
	m_stuTypedata.dVersion  = 3;
	m_stuTypedata.lTimeRate = g_lTimeRate;
}

CTPFileMap::~CTPFileMap()
{
	Release();
}
void CTPFileMap::SetPath(CString sPath)
{
	m_sPath = sPath + _L("\\Icon") ;
	if(!PathFileExists(m_sPath))::CreateDirectory((LPCTSTR)m_sPath,NULL);
}
BOOL CTPFileMap::GetStatus(LPCTSTR lpszFileName, CFileStatus& rStatus)
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
void CTPFileMap::SetVideoStandTimeRate(long lTimeRate)
{
	g_lTimeRate = lTimeRate;
}
CString TP_GetMapFileName(CString sPath,CString sFileName, __int64 iBeginFrame, __int64 iEndFrame,int iReadMode,__int64 iCreateTime)//20081231
{
	GUID    guidFile = TP_GetGuidByFileName(sFileName);
	CString sNewName = _T("");
	sNewName.Format(_T("%s%d%d%d%d01"),TP_GuidToString(&guidFile),(long)iBeginFrame,(long)iEndFrame,(long)iReadMode,(long)iCreateTime);
	while(sPath.GetLength()+sNewName.GetLength() >= MAX_PATH-1 && sNewName.GetLength()>0)
		sNewName = sNewName.Mid(1);	
	CString sReturnName = _T("");
	sReturnName.Format(_L("%s\\%s.tmp"),sPath,sNewName); 

	return sReturnName;
}
BOOL CTPFileMap::SetVideoFileName(CString sFileName,__int64 iFrame, CString sNameEx)
{
	CFileStatus getStateSrc;		
	BOOL        bFileExist = PathFileExists(sFileName);
	GetStatus(sFileName,getStateSrc);
	m_tmCreate = getStateSrc.m_mtime;	

	m_stuTypedata.bAudio = FALSE; 
	m_stuTypedata.iFrame   = iFrame;
	if(sFileName.GetLength()<=3 ) return FALSE;	
	m_sFileName = TP_GetMapFileName(m_sPath,sFileName + sNameEx,iFrame,0,0,m_tmCreate.GetTime());

	CFileStatus getState;
	if(GetStatus(m_sFileName,getState)) //文件存在
	{
		m_iBufferSize = (int)getState.m_size;
		if(m_iBufferSize<=sizeof(m_stuTypedata) || (m_tmCreate != getState.m_ctime && bFileExist))
		{			
			m_bIsRead = FALSE;
			::DeleteFile(m_sFileName); 
			return FALSE;
		}
		else 	m_bIsRead =TRUE;		
	}
	else	m_bIsRead = FALSE;

	return m_bIsRead;
}
BOOL CTPFileMap::SetAudioFileName(CString sFileName, int iFileIndex,__int64 iBeginFrame, __int64 iEndFrame,int iReadMode)
{
	CFileStatus getStateSrc;	
	BOOL        bFileExist = PathFileExists(sFileName);
	GetStatus(sFileName,getStateSrc);
	m_tmCreate = getStateSrc.m_mtime;	

    m_stuTypedata.bAudio      = TRUE; 
	m_stuTypedata.iBeginFrame = iBeginFrame;   
	m_stuTypedata.iEndFrame   = iEndFrame;
	m_stuTypedata.iReadMode   = iReadMode;
	if(sFileName.GetLength()<=3 ) return FALSE;
	m_sFileName = TP_GetMapFileName(m_sPath,sFileName,iBeginFrame,iEndFrame,iReadMode,m_tmCreate.GetTime());	

	CFileStatus getState;
	if(GetStatus(m_sFileName,getState)) //文件存在
	{
		m_iBufferSize = (int)getState.m_size;			
		if(m_iBufferSize<=sizeof(m_stuTypedata) || (m_tmCreate != getState.m_ctime && bFileExist))
		{			
			m_bIsRead           = FALSE;
			::DeleteFile(m_sFileName); 
			return FALSE;
		}
		else	m_bIsRead =TRUE;		
	}
	else	m_bIsRead = FALSE;	
	return m_bIsRead;
}
BYTE * CTPFileMap::GetBuffer()
{
	if(m_pMem) return (BYTE *)m_pMem + sizeof(FileHead);
	else return NULL;
}
LPVOID CTPFileMap::GetBuffer(__int64 iSize)
{
	//取得头部信息
	if(m_bIsRead) //文件存在
	{
		m_hFileHandle= CreateFile(m_sFileName,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		
		if (m_hFileHandle == INVALID_HANDLE_VALUE)    
			return NULL;
		
		
		m_hHandle= CreateFileMapping(m_hFileHandle,
			NULL,
			PAGE_READONLY,
			0,
			(DWORD)m_iBufferSize,
			_L(""));	
		
		if (m_hHandle == NULL) 
			return NULL;
		
		m_pMem =  MapViewOfFile(m_hHandle,
			FILE_MAP_READ,
			0, 0, (SIZE_T)m_iBufferSize);

		if(m_pMem == NULL)
			return NULL;

		FileHead stuTemp;
		memcpy(&stuTemp,m_pMem,sizeof(FileHead));
		BOOL bDel = FALSE;

		if(stuTemp.dVersion !=3 || g_lTimeRate != stuTemp.lTimeRate) bDel = TRUE;
		else if(stuTemp.bAudio)
		{
			if(m_stuTypedata.iBeginFrame != stuTemp.iBeginFrame || m_stuTypedata.iEndFrame != stuTemp.iEndFrame || m_stuTypedata.iReadMode != stuTemp.iReadMode  || stuTemp.iBufferLength >  m_iBufferSize - int(sizeof(FileHead)))
				bDel = TRUE;
		}
		else
		{
			if(m_stuTypedata.iFrame != stuTemp.iFrame  || stuTemp.iBufferLength  >  m_iBufferSize - int(sizeof(FileHead)))
				bDel = TRUE;
		}
		if(bDel) //类型或者长度不对
		{
			if(m_pMem)
			{
				FlushViewOfFile(m_pMem,(DWORD)m_iBufferSize);
				UnmapViewOfFile (m_pMem);
				m_pMem = NULL;
			}
			if(m_hHandle)
			{
				CloseHandle (m_hHandle);
				m_hHandle = NULL;
			}	
			if (m_hFileHandle && m_hFileHandle != INVALID_HANDLE_VALUE)
			{
				CloseHandle (m_hFileHandle);
				m_hFileHandle = NULL;
			}	
			::DeleteFile(m_sFileName); 
			return NULL;
		}
		else 
		{			
			memcpy(&m_stuTypedata,m_pMem,sizeof(FileHead));
			return  (BYTE *)m_pMem + sizeof(FileHead);
		}
	}
	else
	{
		CFileStatus getState;
		CFile       fileCreate;
		if(!fileCreate.Open(m_sFileName,CFile::modeCreate|CFile::modeReadWrite)) return NULL;
		fileCreate.GetStatus(m_sFileName,getState);
		getState.m_ctime = m_tmCreate;
		fileCreate.Close();
		fileCreate.SetStatus(m_sFileName,getState);

		m_iBufferSize = iSize + sizeof(FileHead);
		m_hFileHandle= CreateFile(m_sFileName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);		
        
        
		if (m_hFileHandle == INVALID_HANDLE_VALUE) return NULL;		
		
		m_hHandle= CreateFileMapping(m_hFileHandle,
			NULL,
			PAGE_READWRITE,
			0,
			(DWORD)m_iBufferSize,
			_L(""));	
		
		if (m_hHandle == NULL)
			return NULL;
		
		m_pMem =  MapViewOfFile(m_hHandle,
			FILE_MAP_WRITE | FILE_MAP_READ,
			0, 0, (SIZE_T)m_iBufferSize);

		if (m_pMem == NULL)
			return NULL;
		
		m_stuTypedata.iBufferLength  =  m_iBufferSize - __int64(sizeof(FileHead));
		return (BYTE *)m_pMem + sizeof(FileHead);
	}
}

LPVOID CTPFileMap::GetBuffer(CString sFileName,int iSize)
{
	m_sFileName   = sFileName;
	m_bIsRead     = FALSE;
	m_iBufferSize = iSize;

	m_hFileHandle= CreateFile(m_sFileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_hFileHandle == INVALID_HANDLE_VALUE) return NULL;
	
	m_hHandle= CreateFileMapping(m_hFileHandle,
		NULL,
		PAGE_READWRITE,
		0,
		(DWORD)m_iBufferSize,
		_L(""));	

	if (m_hHandle == NULL) return NULL;

	m_pMem =  MapViewOfFile(m_hHandle,
		FILE_MAP_WRITE | FILE_MAP_READ,
		0, 0, (SIZE_T)m_iBufferSize);

	return (BYTE *)m_pMem;	
}
BOOL CTPFileMap::LockMemory(BOOL bSet,BOOL bFlush)
{
	if(m_hHandle == NULL) return FALSE;
	if(bSet)
	{			
		if(m_pMem == NULL)
		{		
			m_pMem =  MapViewOfFile(m_hHandle,
				m_bIsRead ?FILE_MAP_READ : FILE_MAP_WRITE | FILE_MAP_READ,
				0, 0, (SIZE_T)m_iBufferSize);
		}				
		return m_pMem == NULL;
	}
	else if(m_pMem)
	{
		if(!m_bIsRead && bFlush)
		{
			memcpy(m_pMem,&m_stuTypedata,sizeof(FileHead));
			FlushViewOfFile(m_pMem,(SIZE_T)m_iBufferSize);			
		}		
		if(UnmapViewOfFile(m_pMem))
		{
			m_pMem = NULL;
			return TRUE;
		}		
		else return FALSE;
	}
	else return TRUE;
}
BOOL   CTPFileMap::Release()
{

	if(m_pMem)
	{
		if(!m_bIsRead)
		{
			memcpy(m_pMem,&m_stuTypedata,sizeof(FileHead));
			FlushViewOfFile(m_pMem,(DWORD)m_iBufferSize);
		}
		UnmapViewOfFile (m_pMem);
		m_pMem = NULL;
	}
	if(m_hHandle)
	{
		CloseHandle (m_hHandle);
		m_hHandle = NULL;
	}	
	if (m_hFileHandle && m_hFileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle (m_hFileHandle);
		m_hFileHandle = NULL;
	}	

	return TRUE;
}