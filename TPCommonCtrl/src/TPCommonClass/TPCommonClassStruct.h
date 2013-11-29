//MessageRange 
//IDRange     
#pragma once




//基本函数
inline CString TP_GetSysErrMsg( DWORD dwErrCode)
{
#define LEN_ERR_MSG 512

	LANGID dwDefaultLanguageID;		//系统缺省的语言ID
	TCHAR strBuf[ LEN_ERR_MSG + 1 ];
	memset( strBuf, 0, sizeof(TCHAR) * ( LEN_ERR_MSG + 1 ) );

	dwDefaultLanguageID = GetSystemDefaultLangID();

	CString strMsg;

	DWORD dwReturnCode = FormatMessage
		(
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_MAX_WIDTH_MASK,	//Flags
		NULL,							//No message source
		dwErrCode,						//Message ID
		dwDefaultLanguageID,			//Language ID
		strBuf,						//Pointer of message buffer
		LEN_ERR_MSG,					//Message buffer size
		NULL							//No arguments
		);

	if( dwReturnCode == 0 )
		strMsg = _T("取得系统错误信息失败，或没有相应的错误信息!");
	else
		strMsg = strBuf;

	return strMsg;
}



class AFX_EXT_CLASS CTPSyncLock
{
public:
	CTPSyncLock(CMutex *pMutex)
	{
		m_pSection    = NULL;
		m_pMutex      = pMutex;
		m_pbFlag      = NULL;
		m_pSectionEx  = NULL;
		if(m_pMutex)    m_pMutex ->Lock();
	}
	CTPSyncLock(CMutex *pMutex,BOOL *bFlag)
	{
		m_pSection    = NULL;
		m_pMutex      = pMutex;
		m_pSectionEx  = NULL;
		m_pbFlag      = bFlag;
		if(m_pMutex)	m_pMutex ->Lock();
		if(m_pbFlag)   *m_pbFlag = TRUE;
	}
	CTPSyncLock(CCriticalSection *pLock)
	{
		m_pSection   = pLock;
		m_pMutex     = NULL;
		m_pbFlag     = NULL;
		m_pSectionEx = NULL;
		if(m_pSection)	m_pSection ->Lock();
	}
	CTPSyncLock(CRITICAL_SECTION *pSection)
	{
		m_pSection   = NULL;
		m_pMutex     = NULL;
		m_pbFlag     = NULL;
		m_pSectionEx = pSection;
		if(m_pSectionEx)   EnterCriticalSection(m_pSectionEx);
	}
	~CTPSyncLock()
	{
		if(m_pMutex)	    m_pMutex ->Unlock();
		if(m_pSection)	    m_pSection ->Unlock();
		if(m_pSectionEx) 	LeaveCriticalSection(m_pSectionEx);
		if(m_pbFlag)       *m_pbFlag = FALSE;
		m_pMutex     = NULL;
		m_pbFlag     = NULL;
		m_pSection   = NULL;
		m_pSectionEx = NULL;
	}
private:
	CMutex            *m_pMutex;
	BOOL              *m_pbFlag;
	CCriticalSection  *m_pSection;
	CRITICAL_SECTION  *m_pSectionEx;
};
class AFX_EXT_CLASS CTPFileLock:public CFile
{
public:
 
	CTPFileLock(BOOL bTeamMode)
	{
		m_lLength     = 400*1024;      //缩定长度
		m_bLockMode   = bTeamMode;     //是否为缩定模式
		m_bLockAuto   = bTeamMode;     //是否Open的时候缩定
		m_bLockState  = FALSE;        //是否缩定成功
		ZeroMemory(&m_stuOverLapped,sizeof(OVERLAPPED));
	}
	CTPFileLock(BOOL bLockMode,BOOL bAutoLock,long lLength = -1)
	{
		m_lLength     = lLength >=0? lLength: 400*1024;  //缩定长度
		m_bLockMode   = bLockMode;                       //是否为缩定模式
		m_bLockAuto   = bAutoLock;                       //是否Open的时候缩定
		m_bLockState  = FALSE;                          //是否缩定成功
		ZeroMemory(&m_stuOverLapped,sizeof(OVERLAPPED));	 
	}
	~CTPFileLock()
	{
		if(m_bLockMode && m_bLockState) 
			Unlock();	
	}
	BOOL Lock()
	{
		if(m_hFile != INVALID_HANDLE_VALUE && !m_bLockState)
		{
			m_bLockState   = LockFileEx(m_hFile, LOCKFILE_EXCLUSIVE_LOCK, 0,0, m_lLength, &m_stuOverLapped);  
		}
		return m_bLockState;

	}
	BOOL Unlock()
	{
		BOOL bReturn = FALSE;
		if(m_hFile != INVALID_HANDLE_VALUE && m_bLockState)
		{
			bReturn      = UnlockFileEx(m_hFile, 0, 0, m_lLength, &m_stuOverLapped);  
			m_bLockState = FALSE;
		}
		return bReturn;
	}
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags,CFileException* pError = NULL)
 	{
		try
		{	 
			if(m_bLockMode) 
			{
				nOpenFlags   &= ~ (CFile::shareExclusive | CFile::shareDenyWrite | CFile::shareDenyRead);
				nOpenFlags   |= CFile::shareDenyNone;
			}
			BOOL bReturn = CFile::Open(lpszFileName,nOpenFlags,pError);
			if(m_bLockAuto) Lock();				 
			return bReturn;
		}
		catch (...)
		{
			DWORD  dwRet = GetLastError();
			CString sError = TP_GetSysErrMsg( dwRet);
			ASSERT(0);
			return FALSE;
		}		
	}
	virtual void Close() 
	{
		if(m_bLockMode && m_bLockState) Unlock();		 
		if(m_hFile != INVALID_HANDLE_VALUE)
			CFile::Close();	 
	}
public:
private:
	long       m_lLength;       //缩定长度
	BOOL       m_bLockMode;     //是否为缩定模式
	BOOL       m_bLockAuto;     //是否Open的时候缩定
	BOOL       m_bLockState;    //是否缩定成功
	OVERLAPPED m_stuOverLapped;
};
class AFX_EXT_CLASS CTPFileLockEx:public CTPFileLock
{
public:
	CTPFileLockEx(LPCTSTR sFileName):CTPFileLock(TRUE)
	{
		Open(sFileName,CFile::modeCreate | CFile::modeWrite );
	}
	~CTPFileLockEx()
	{
		Close();
	}
};

#define INFINITEWAIT (60 *1000)

class CTPBaseThread; 
typedef UINT (*AFX_THREADPROCEX)(LPVOID,BOOL *,CTPBaseThread *);
class  CTPBaseThread
{
public:
	CTPBaseThread()
	{	
		m_pThreadFun       = NULL;
		m_pThreadParam     = NULL;
		m_iThreadMode      = 0;
		m_iWaitTime        = INFINITEWAIT; 

		for(INT l=0;l<4;l++)
			m_hDefaultEvent[l] = NULL;

		m_hThreadEvent     = ::CreateEvent(0,FALSE,FALSE,0);
		m_hThreadData      = ::CreateEvent(0,FALSE,FALSE,0);
		m_bThreadRun       = FALSE;
		m_pThreadHandle    = NULL;
		m_pCriticalSection = NULL;
		m_bThreadFlag      = FALSE;
		m_iDataSize        = 0;
		m_bWaitThread      = FALSE;
		m_hWaitMsgWnd      = NULL;
		m_hWaitMsgMin      = 0;
		m_hWaitMsgMax      = 0;
 	}
	~CTPBaseThread()
	{
		if(m_hThreadEvent)  ::CloseHandle(m_hThreadEvent);
		m_hThreadEvent  = NULL;
		if(m_hThreadData)   ::CloseHandle(m_hThreadData);
		m_hThreadData  = NULL;

		for(INT l=0;l<4;l++)
		{
			if(m_hDefaultEvent[l]) 
				::CloseHandle(m_hDefaultEvent[l]);
			m_hDefaultEvent[l] = NULL;
		}

		m_bThreadRun    = FALSE;
		m_pThreadFun    = NULL;
		m_pThreadParam  = NULL;
		m_pThreadHandle = NULL;

		if(m_pCriticalSection)
		{
			DeleteCriticalSection(m_pCriticalSection);
			delete m_pCriticalSection;
			m_pCriticalSection = NULL;
		}

 	}
	BOOL BeginThread(AFX_THREADPROCEX pfnThreadProc, LPVOID pParam,int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL)
	{
		if(pfnThreadProc)
		{
			m_pThreadFun   = pfnThreadProc;
			m_pThreadParam = pParam;
		}
		ASSERT(m_bThreadRun == FALSE);

		::ResetEvent(m_hThreadEvent);
		::ResetEvent(m_hThreadData);
		m_bThreadRun   = TRUE;

		m_pThreadHandle = AfxBeginThread(ThreadProc,this,nPriority,nStackSize,dwCreateFlags,lpSecurityAttrs);
		if(m_pThreadHandle) ::WaitForSingleObject(m_hThreadEvent,INFINITE);
		::ResetEvent(m_hThreadEvent);

		return m_pThreadHandle != NULL;
	}
	UINT ThreadProcStatic()
	{
		::SetEvent(m_hThreadEvent);

		UINT uReturn =  0;

		if(m_pThreadFun)
			uReturn = m_pThreadFun(m_pThreadParam ,&m_bThreadRun,this);
		
		m_bThreadRun = FALSE;
		::SetEvent(m_hThreadEvent);

		return uReturn;
	}	
	UINT ThreadProcDynamic()
	{
		::SetEvent(m_hThreadEvent);

		UINT uReturn =  0;
		while(m_bThreadRun)
		{		
			if(m_pThreadFun)
				uReturn = m_pThreadFun(m_pThreadParam ,&m_bThreadRun,this);

			if(GetDataSize()>0 && 0 != uReturn) continue;			
			else if(m_bWaitThread) break;
			if(WaitDataEvent(m_iWaitTime) == WAIT_OBJECT_0) continue;
			if(GetDataSize()<=0) break;
		}

		m_bThreadRun = FALSE;
		::SetEvent(m_hThreadEvent);

		return uReturn;
	}	
	static UINT ThreadProc(LPVOID pVoid)
	{
		CTPBaseThread *pThread = (CTPBaseThread *)pVoid;
		if(pThread ->m_iThreadMode == 0)      return pThread ->ThreadProcStatic();	
		else if(pThread ->m_iThreadMode == 1) return pThread ->ThreadProcDynamic();	
		else return 0;		
	}
	BOOL IsRun()
	{
		return m_bThreadRun;
	}
	UINT WaitEvent(int iIndex,int iTime)
	{
		if(m_hDefaultEvent[iIndex] == NULL)
			return ::WaitForSingleObject(m_hDefaultEvent[iIndex],iTime);
		else 
			return WAIT_ABANDONED;
	}
	void SetEvent(int iIndex)
	{
		if(m_hDefaultEvent[iIndex] == NULL)
			::SetEvent(m_hDefaultEvent[iIndex]);
	}
	void ResetEvent(int iIndex)
	{
		if(m_hDefaultEvent[iIndex] == NULL)
			::ResetEvent(m_hDefaultEvent[iIndex]);
	}
	void CreateEvent(int iIndex)
	{
		if(m_hDefaultEvent[iIndex] == NULL)
			m_hDefaultEvent[iIndex] = ::CreateEvent(0,FALSE,FALSE,0);
	}
	void CreateCritical()
	{
		if(m_pCriticalSection == NULL)
		{
			m_pCriticalSection = new CRITICAL_SECTION;
			InitializeCriticalSection(m_pCriticalSection);
		}
	}
	void Lock()
	{
		if(m_pCriticalSection)
			EnterCriticalSection(m_pCriticalSection);
	}
	void Unlock()
	{
		if(m_pCriticalSection)
			LeaveCriticalSection(m_pCriticalSection);
	}
	void WaitMsgWnd()
	{
		MSG stuGetMsg;
		while(::WaitForSingleObject(m_hThreadEvent,1000)!= WAIT_OBJECT_0)
		{
			while(PeekMessage(&stuGetMsg,m_hWaitMsgWnd,0,0,PM_REMOVE))
			{
				TranslateMessage(&stuGetMsg);
				DispatchMessage(&stuGetMsg);		
			}
		}		
	}
	void WaitMsgID()
	{
		MSG stuGetMsg;
		while(::WaitForSingleObject(m_hThreadEvent,1000)!= WAIT_OBJECT_0)
		{
			while(PeekMessage(&stuGetMsg,NULL,m_hWaitMsgMin,m_hWaitMsgMax,PM_REMOVE))
			{
				TranslateMessage(&stuGetMsg);
				DispatchMessage(&stuGetMsg);		
			}
		}		
	}
	void SetDataEvent()
	{
		::SetEvent(m_hThreadData);
	}
	void ResetDataEvent()
	{
		::ResetEvent(m_hThreadData);
	}
	void SetDataSize(int iSize)
	{
		if(m_pCriticalSection)	EnterCriticalSection(m_pCriticalSection);
		m_iDataSize = iSize;
		if(m_pCriticalSection)	LeaveCriticalSection(m_pCriticalSection);
	}
	int  GetDataSize()
	{
		int iSize = 0;
		if(m_pCriticalSection)  EnterCriticalSection(m_pCriticalSection);
		iSize = m_iDataSize;
		if(m_pCriticalSection)	LeaveCriticalSection(m_pCriticalSection);
		return iSize;
	}
	UINT WaitDataEvent(int iTime = INFINITE)
	{
		UINT uReturn =  ::WaitForSingleObject(m_hThreadData,iTime);
		return uReturn;
	}
	void WaitThread()
	{
		if(m_bThreadRun)
		{
			m_bWaitThread = TRUE;
			::SetEvent(m_hThreadData);			
			if(m_hWaitMsgWnd!=NULL)                WaitMsgWnd();
			else if(m_hWaitMsgMax > m_hWaitMsgMin) WaitMsgID();
			else  ::WaitForSingleObject(m_hThreadEvent,1000000);

			m_bThreadRun  = FALSE;
			m_bWaitThread = FALSE;
		}
		m_pThreadHandle = NULL;
	}
	void EndThread(HANDLE hStopEvent = NULL)
	{
		if(m_bThreadRun)
		{
			m_bThreadRun     = FALSE;

			::SetEvent(m_hThreadData);

			if(m_hWaitMsgWnd!= NULL)               WaitMsgWnd();
			else if(m_hWaitMsgMax > m_hWaitMsgMin) WaitMsgID();
			else  ::WaitForSingleObject(m_hThreadEvent,1000000);
		}
 		m_pThreadHandle = NULL;
	}
public:
	AFX_THREADPROCEX   m_pThreadFun;
	LPVOID             m_pThreadParam;
	BYTE               m_iThreadMode; 
	int                m_iWaitTime;

private:
	HANDLE             m_hDefaultEvent[4];
	HANDLE             m_hThreadEvent;
	HANDLE             m_hThreadData;

	BOOL               m_bThreadRun; 
	
	CWinThread        *m_pThreadHandle;
	CRITICAL_SECTION  *m_pCriticalSection;

	int                m_iDataSize;
	BYTE               m_bWaitThread;

public:
	BOOL               m_bThreadFlag;
	HWND               m_hWaitMsgWnd;
	UINT               m_hWaitMsgMin;
	UINT               m_hWaitMsgMax;
};

