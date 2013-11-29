#pragma once

class AFX_EXT_CLASS CTPFileMonitor1
{
public:
	CTPFileMonitor1(void);
	~CTPFileMonitor1(void);

public:
	struct tag_MonitorThread
	{
		BOOL            bThreadRun;
		HANDLE          hThreadFinish;
		HANDLE          hThreadExist;
		CString         sPath;
		void          (*fCallback)(LPCTSTR,LPCTSTR,UINT uType,LPARAM);
		LPARAM          lCallbackParam;
		CTPFileMonitor1 *pMonitor;
		long             lThreadIndex;
		BOOL             bChildPath;
		tag_MonitorThread()
		{
			bThreadRun     = FALSE;
			hThreadFinish  = ::CreateEvent(0,FALSE,FALSE,0);
			hThreadExist   = ::CreateEvent(0,FALSE,FALSE,0);
			sPath          = _T("");
			fCallback      = NULL;
			lCallbackParam = NULL;
			pMonitor       = NULL;
			lThreadIndex   = 0;
			bChildPath     = FALSE;
		} 
		void Stop()
		{
			if(bThreadRun)
			{
				bThreadRun = FALSE;
				::SetEvent(hThreadExist);
				::WaitForSingleObject(hThreadFinish,INFINITE);
			}
		}
		~tag_MonitorThread()
		{
			bThreadRun     = FALSE;
			::CloseHandle(hThreadFinish)  ;
			hThreadFinish  = NULL;
			CloseHandle(hThreadExist);
			hThreadExist   = NULL;
			sPath          = _T("");
			fCallback      = NULL;
			lCallbackParam = NULL;
			pMonitor       = NULL;
			lThreadIndex   = 0;
			bChildPath     = FALSE;
		}
	};
	struct tag_MonitorData
	{
		CString pSrc;
		CString pDest;
		UINT    uType;
		int     iIndex;
		tag_MonitorData &operator = (tag_MonitorData other)
		{
			pSrc  = other.pSrc;
			pDest = other.pDest;
			uType = other.uType;
			iIndex=other.iIndex;
			return *this;
		}
	};
public:
	DWORD                                             m_dwLastTime;
protected:
	CArray<tag_MonitorThread *,tag_MonitorThread *&>  m_aMonitorThread;
	CArray<tag_MonitorData    ,tag_MonitorData &>     m_aMonitorData;
	CTPBaseThread                                     m_stuMonitorCallBack;
public:
	void StartMonitor(CStringArray &aPath,BOOL bChildPath,void (*f)(LPCTSTR,LPCTSTR,UINT uType,LPARAM),LPARAM lParam);
	void StopMonitor();
	void StopMonitor(CString &sPath);
	void LockFoder(CString &sPath);
public:
	void ThreadMonotor(tag_MonitorThread *pThread);
	void ThreadCallBack(CTPBaseThread *pThread,BOOL *pbRun);
	void FileChangeNotify(LPCTSTR pSrc, LPCTSTR pDest,UINT uType,long lIndex);
};
