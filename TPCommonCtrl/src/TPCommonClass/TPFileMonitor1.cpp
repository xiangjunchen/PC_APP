#include "StdAfx.h"

CTPFileMonitor1::CTPFileMonitor1(void)
{
	m_stuMonitorCallBack.CreateCritical();	
 	m_dwLastTime = GetTickCount();
}

CTPFileMonitor1::~CTPFileMonitor1(void)
{
} 
UINT TP_MonitorCallBack(LPVOID pVoid,BOOL *pbRun,CTPBaseThread *pThread) 
{
	((CTPFileMonitor1 *)pVoid) ->ThreadCallBack(pThread,pbRun);
	return 1;
}
void   CTPFileMonitor1::ThreadCallBack(CTPBaseThread *pThread,BOOL *pbRun)
{
	while(*pbRun)
	{
		CArray<tag_MonitorData    ,tag_MonitorData &>  aData;

		pThread->Lock();
		aData.Copy(m_aMonitorData);
		m_aMonitorData.RemoveAll();
		pThread ->SetDataSize(0);
		pThread->Unlock();

		if(aData.GetSize()<=0) break;		
		else 
		{
			for(INT l=0;l<aData.GetSize();l++)
			{
				m_aMonitorThread[aData[l].iIndex] ->fCallback(aData[l].pSrc,aData[l].pDest,aData[l].uType,m_aMonitorThread[aData[l].iIndex] ->lCallbackParam);
			}
			aData.RemoveAll();
			m_dwLastTime = GetTickCount();
		}
	}

}
UINT TP_MonitorThread(LPVOID pVoid)
{
	CTPFileMonitor1::tag_MonitorThread *pThread = (CTPFileMonitor1::tag_MonitorThread *)pVoid;
	pThread ->pMonitor->ThreadMonotor(pThread);
	return 1;
}
 
void  CTPFileMonitor1::StartMonitor(CStringArray &aPath,BOOL bChildPath,void (*f)(LPCTSTR,LPCTSTR,UINT uType,LPARAM),LPARAM lParam)
{
	for(INT l=0;l<aPath.GetSize();l++)
	{
		tag_MonitorThread *pMonitorThread = new tag_MonitorThread();
		pMonitorThread->sPath          = aPath[l];
		pMonitorThread->fCallback      = f;
		pMonitorThread->lCallbackParam = lParam;
		pMonitorThread->bThreadRun     = TRUE;
		pMonitorThread->pMonitor       = this;
		pMonitorThread->lThreadIndex   = l;
		pMonitorThread->bChildPath     = bChildPath;
		::ResetEvent(pMonitorThread->hThreadFinish);
		::ResetEvent(pMonitorThread->hThreadExist);
		AfxBeginThread(TP_MonitorThread,(LPVOID)pMonitorThread,THREAD_PRIORITY_HIGHEST);
		m_aMonitorThread.Add(pMonitorThread);
	}
}
void  CTPFileMonitor1::StopMonitor()
{
	for(INT l=0;l<m_aMonitorThread.GetSize();l++)
	{
		m_aMonitorThread[l]->Stop();
		delete m_aMonitorThread[l];
	}
	m_aMonitorThread.RemoveAll();
	m_stuMonitorCallBack.EndThread();
	m_aMonitorData.RemoveAll();
}
void   CTPFileMonitor1::StopMonitor(CString &sPath)
{
	for(INT l=0;l<m_aMonitorThread.GetSize();l++)
	{
		if(m_aMonitorThread[l]->sPath.CollateNoCase(sPath) == 0)
		{
			m_aMonitorThread[l]->Stop();
			delete m_aMonitorThread[l];
			m_aMonitorThread.RemoveAt(l);
			break;
		}
	}
}
void   CTPFileMonitor1::LockFoder(CString &sPath)
{

}

void CTPFileMonitor1::FileChangeNotify(LPCTSTR pSrc, LPCTSTR pDest,UINT uType,long lIndex)
{
	CString  sFileName = pSrc;
	CString  sFileExt;
	CString  sGuid;
	CString  sPathFileName  = m_aMonitorThread[lIndex]->sPath + _T("\\") + sFileName;
	CString  sPathFileName1 = sPathFileName;//避免变成小写

	if(m_aMonitorThread[lIndex]->bChildPath)
		sFileName = PathFindFileName(pSrc);
	if(sFileName.GetLength()<40) return;


	sPathFileName.MakeLower();
	if(sPathFileName.Find(_T("\\resmanage\\resinfo\\"))<0 && sPathFileName.Find(_T("\\resmanage\\hideclip\\"))<0) return;

	sFileName.MakeLower();

	sFileExt  = sFileName.Right(4);
	sGuid     = sFileName.Left(36);

	if(sGuid[8]!= '-' || sGuid[13]!= '-' ||  sGuid[18]!='-') 
		return;


	tag_MonitorData stuData;
	stuData.pSrc   = sGuid;
	stuData.pDest  = sPathFileName1;
	stuData.iIndex = lIndex;
	switch(uType)
	{
	case FILE_ACTION_RENAMED_NEW_NAME:
	case FILE_ACTION_ADDED:
		{			 
			if(sFileExt == _T(".cat"))
			{
				stuData.uType = 0;
			}
			else if(sFileExt == _T(".clp"))
			{
				stuData.uType = 1;
			}
			else if(sFileExt == _T(".pro"))
			{
				stuData.uType = 2;
			}
			else if(sFileExt == _T(".tmp"))
			{
				stuData.uType = 3;
			}
			else if(sFileExt == _T(".hcp"))
			{
				stuData.uType = 4;
			}
			else return;
		}
		break;
	case FILE_ACTION_REMOVED:
		{		

			if(sFileExt == _T(".cat"))
			{
				stuData.uType = 10;
			}
			else if(sFileExt == _T(".clp"))
			{
				stuData.uType = 11;
			}
			else if(sFileExt == _T(".pro"))
			{
				stuData.uType = 12;
			}
			else if(sFileExt == _T(".tmp"))
			{
				stuData.uType = 13;
			}
			else if(sFileExt == _T(".hcp"))
			{
				stuData.uType = 14;
			}
			else return;
		}	 
		break;	
	default:
		return;
	}	 
	m_stuMonitorCallBack.Lock();
	m_aMonitorData.Add(stuData);
	m_stuMonitorCallBack.SetDataSize(1);
	m_stuMonitorCallBack.Unlock();

	if(!m_stuMonitorCallBack.IsRun())
		m_stuMonitorCallBack.BeginThread(TP_MonitorCallBack,this,THREAD_PRIORITY_HIGHEST);
	else
		m_stuMonitorCallBack.SetDataEvent();

}
void  CTPFileMonitor1::ThreadMonotor(tag_MonitorThread *pThread)
{
	HANDLE                   hDirHandle    = NULL;;
	BYTE                    *pNotify       = new BYTE[4*1024+10];
	long                     lNotify       = 4*1024;
	DWORD                    lNotifyReturn = 0;
	int                      iNotifyOffset = 0;
	OVERLAPPED               stuNotifyOV; 
	CString                  sFileName;
	int                      iFailCount    = 0;
 	FILE_NOTIFY_INFORMATION *pNotifyIndex  = (FILE_NOTIFY_INFORMATION *)pNotify; 
	FILE_NOTIFY_INFORMATION *pNotifyNext   = NULL;
	hDirHandle = CreateFile(pThread ->sPath, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL); 
	if (hDirHandle != INVALID_HANDLE_VALUE) 
	{
		 ZeroMemory(&stuNotifyOV,sizeof(OVERLAPPED)); 
		 stuNotifyOV.hEvent   =   pThread ->hThreadExist; 

		while (pThread ->bThreadRun)
		{
			ZeroMemory(pNotify,lNotify);
			if(ReadDirectoryChangesW(hDirHandle,pNotify,lNotify, pThread->bChildPath,  FILE_NOTIFY_CHANGE_FILE_NAME/*| FILE_NOTIFY_CHANGE_LAST_WRITE*/, &lNotifyReturn, &stuNotifyOV, NULL)) 
			{  
				if(WaitForSingleObject(stuNotifyOV.hEvent,   INFINITE)   ==   WAIT_OBJECT_0) 
				{ 
					if (!pThread ->bThreadRun) 
						break;                        
				} 
				pNotifyNext = pNotifyIndex; 
				do
				{
					switch(pNotifyNext->Action) 
					{ 
					case FILE_ACTION_ADDED: 
						pNotifyNext->FileName[pNotifyNext->FileNameLength/2]    = 0;
						sFileName = pNotifyNext->FileName;
						FileChangeNotify(sFileName,NULL,pNotifyNext->Action,pThread->lThreadIndex);
						break; 
					case FILE_ACTION_REMOVED:
						pNotifyNext->FileName[pNotifyNext->FileNameLength/2]    = 0;
						sFileName = pNotifyNext->FileName;
						FileChangeNotify(sFileName,NULL,pNotifyNext->Action,pThread->lThreadIndex);
						break; 
					case FILE_ACTION_MODIFIED:
						pNotifyNext->FileName[pNotifyNext->FileNameLength/2]    = 0;
						sFileName = pNotifyNext->FileName;
						break; 
					case FILE_ACTION_RENAMED_OLD_NAME:
						pNotifyNext->FileName[pNotifyNext->FileNameLength/2]    = 0;
						sFileName = pNotifyNext->FileName;
						break; 
					case FILE_ACTION_RENAMED_NEW_NAME:
						pNotifyNext->FileName[pNotifyNext->FileNameLength/2]    = 0;
						sFileName = pNotifyNext->FileName;
						FileChangeNotify(sFileName,NULL,pNotifyNext->Action,pThread->lThreadIndex);
 						break; 
					default: break; 
					}
					if(pNotifyNext->NextEntryOffset !=0) 
					{ 
						iNotifyOffset = pNotifyNext->NextEntryOffset; 
						pNotifyNext  = (PFILE_NOTIFY_INFORMATION) ((LPBYTE)pNotifyNext+iNotifyOffset);
					}
					else break; 
				}
				while(pThread ->bThreadRun); 
			}
			else 
			{
				DWORD dwError = GetLastError();
				CString sError = TP_GetSysErrMsg(dwError);
				TRACE(sError);
				iFailCount ++;
				if(iFailCount>10)
				{
					if(pThread ->fCallback)
						pThread ->fCallback(NULL,NULL,1000,pThread->lCallbackParam);
					ASSERT(0);
					break;;
				}
			}
		}
	}
	::CloseHandle(hDirHandle);
	delete [] pNotify;

	pThread ->bThreadRun = FALSE;
	::SetEvent(pThread ->hThreadFinish);

	//add by xjc for temp
	//HANDLE                   hDirHandle    = NULL;;
	//HANDLE					 hCompPort     = NULL;	//i/o completion port
	//BYTE                    *pNotifyBuffer       = new BYTE[4*1024+10];
	//long                     lNotify       = 4*1024;
	//DWORD                    lNotifyReturn = 0;
	//int                      iNotifyOffset = 0;
	//OVERLAPPED               stuNotifyOV; 
	//LPOVERLAPPED			 lpOverlapped  = 0;
	//CString                  sFileName;
	//int                      iFailCount    = 0;
	//DWORD					 numBytes      = 0;

	//PFILE_NOTIFY_INFORMATION pCurNotify   = NULL;
	//hDirHandle = CreateFile(pThread ->sPath, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL); 
	//if (hDirHandle != INVALID_HANDLE_VALUE) 
	//{
	//	hCompPort = CreateIoCompletionPort(hDirHandle, hCompPort, (DWORD)pNotifyBuffer,0);
	//	if(NULL != hCompPort)
	//	{
	//		ZeroMemory(&stuNotifyOV,sizeof(OVERLAPPED)); 
	//		stuNotifyOV.hEvent = pThread->hThreadExist;
	//		lpOverlapped = &stuNotifyOV;
	//		//Start
	//		if(!ReadDirectoryChangesW(hDirHandle,pNotifyBuffer,lNotify, pThread->bChildPath,  FILE_NOTIFY_CHANGE_FILE_NAME, &lNotifyReturn, lpOverlapped, NULL))
	//		{  
	//			TRACE(_T("ReadDirectoryChangesW  failed\n"));
	//		}
	//		//PostQueuedCompletionStatus(hCompPort,sizeof(pNotifyBuffer),(DWORD)pNotifyBuffer,lpOverlapped);
	//		while (pThread ->bThreadRun)
	//		{
	//			if(GetQueuedCompletionStatus(hCompPort,&numBytes,(LPDWORD)&pNotifyBuffer,&lpOverlapped,INFINITE))
	//			{
	//				if(WaitForSingleObject(lpOverlapped->hEvent,   INFINITE)   ==   WAIT_OBJECT_0)
	//				{ 
	//					if (!pThread ->bThreadRun) 
	//						break;                        
	//				} 
	//				pCurNotify = (PFILE_NOTIFY_INFORMATION)pNotifyBuffer;					
	//				do
	//				{
	//					switch(pCurNotify->Action) 
	//					{ 
	//					case FILE_ACTION_ADDED: 
	//						pCurNotify->FileName[pCurNotify->FileNameLength/2]    = 0;
	//						sFileName = pCurNotify->FileName;
	//						FileChangeNotify(sFileName,NULL,pCurNotify->Action,pThread->lThreadIndex);
	//						break; 
	//					case FILE_ACTION_REMOVED:
	//						pCurNotify->FileName[pCurNotify->FileNameLength/2]    = 0;
	//						sFileName = pCurNotify->FileName;
	//						FileChangeNotify(sFileName,NULL,pCurNotify->Action,pThread->lThreadIndex);
	//						break; 
	//					case FILE_ACTION_MODIFIED:
	//						pCurNotify->FileName[pCurNotify->FileNameLength/2]    = 0;
	//						sFileName = pCurNotify->FileName;
	//						break; 
	//					case FILE_ACTION_RENAMED_OLD_NAME:
	//						pCurNotify->FileName[pCurNotify->FileNameLength/2]    = 0;
	//						sFileName = pCurNotify->FileName;
	//						break; 
	//					case FILE_ACTION_RENAMED_NEW_NAME:
	//						pCurNotify->FileName[pCurNotify->FileNameLength/2]    = 0;
	//						sFileName = pCurNotify->FileName;
	//						FileChangeNotify(sFileName,NULL,pCurNotify->Action,pThread->lThreadIndex);
	//						break; 
	//					default: break; 
	//					}
	//					iNotifyOffset = pCurNotify->NextEntryOffset; 
	//					pCurNotify  = (PFILE_NOTIFY_INFORMATION) ((LPBYTE)pCurNotify+iNotifyOffset);						
	//				}
	//				while(pCurNotify && pCurNotify->NextEntryOffset !=0UL); 
	//				if(!ReadDirectoryChangesW(hDirHandle,pNotifyBuffer,lNotify, pThread->bChildPath,  FILE_NOTIFY_CHANGE_FILE_NAME, &lNotifyReturn, lpOverlapped, NULL)) 
	//				{  
	//					TRACE(_T("ReadDirectoryChangesW  failed\n"));
	//				}
	//			}
	//		}
	//	}
	//}
	//::CloseHandle(hDirHandle);
	//delete [] pNotifyBuffer;

	//pThread ->bThreadRun = FALSE;
	//::SetEvent(pThread ->hThreadFinish);
}

/*
  typedef BOOL (WINAPI *READ_DIRECTORY_CHANGES)(HANDLE,LPVOID,DWORD,BOOL,DWORD,LPDWORD,LPOVERLAPPED,
                                                    LPOVERLAPPED_COMPLETION_ROUTINE);
          READ_DIRECTORY_CHANGES ReadDirectoryChanges;//先定义
          ReadDirectoryChanges=(READ_DIRECTORY_CHANGES)GetProcAddress(LoadLibrary("KERNEL32.dll"),
                                                                              "ReadDirectoryChangesW");
          if(!ReadDirectoryChanges)
               { Error_MSG(GetLastError());exit(0);};
//最新的SDK支持这个函数(ReadDirectoryChangesW) 不用做以上操作           
    HANDLE hFile  =  CreateFile( 
        Path, 
        GENERIC_READ|GENERIC_WRITE, 
        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
        NULL, 
        OPEN_EXISTING, 
        FILE_FLAG_BACKUP_SEMANTICS, 
        NULL 
        ); //打开设备句柄
        if(!hFile)
           { Error_MSG(GetLastError());exit(0);}    
        
        char buff[2*(sizeof(FILE_NOTIFY_INFORMATION)+MAX_PATH)]={0};//定义一个数组
        FILE_NOTIFY_INFORMATION * notify=(FILE_NOTIFY_INFORMATION*)buff;
        DWORD BytesReturn;
        while(TRUE) 
                 {
                 if(ReadDirectoryChanges(hFile,notify,sizeof(buff),true,            
                                                       FILE_NOTIFY_CHANGE_FILE_NAME| 
                                                    FILE_NOTIFY_CHANGE_DIR_NAME| 
                                                    FILE_NOTIFY_CHANGE_ATTRIBUTES| 
                                                    FILE_NOTIFY_CHANGE_SIZE| 
                                                    FILE_NOTIFY_CHANGE_LAST_WRITE| 
                                                    FILE_NOTIFY_CHANGE_LAST_ACCESS| 
                                                    FILE_NOTIFY_CHANGE_CREATION| 
                                                    FILE_NOTIFY_CHANGE_SECURITY,
                                                    &BytesReturn,NULL,NULL));
         char temp[MAX_PATH]={0},temp1[MAX_PATH]={0};                                            
         switch(notify->Action)
            {                
              case FILE_ACTION_ADDED:                            
                 
                 WideCharToMultiByte(CP_ACP,0,notify->FileName,notify->FileNameLength/2,temp,MAX_PATH,NULL,NULL);
                 strcpy_s(temp1,sizeof(temp1),temp);
                 printf("增加操作：%s%s\n",Path,temp1);        
               break;
               
               case FILE_ACTION_REMOVED:
                 WideCharToMultiByte(CP_ACP,0,notify->FileName,notify->FileNameLength/2,temp,MAX_PATH,NULL,NULL);
                 strcpy_s(temp1,sizeof(temp1),temp);
                 printf("删除操作：%s%s\n",Path,temp1);
               break;
               
               case FILE_ACTION_RENAMED_NEW_NAME:
               case FILE_ACTION_RENAMED_OLD_NAME:
                 WideCharToMultiByte(CP_ACP,0,notify->FileName,notify->FileNameLength/2,temp,MAX_PATH,NULL,NULL);
                 strcpy_s(temp1,sizeof(temp1),temp);
                 printf("命名操作：%s%s\n",Path,temp1);
               break;
              // default:
               //  WideCharToMultiByte(CP_ACP,0,notify->FileName,notify->FileNameLength/2,temp,MAX_PATH,NULL,NULL);
                // strcpy_s(temp1,sizeof(temp1),temp);
                // printf("IO操作：%s%s\n",Path,temp1);
              // break;                                            
            }
         }
        
		*/

 
 /*

 这是我做的一个简单例子,   监视文件名的修改,   可以随时停止线程: 

HANDLE   hDir   =   NULL; 
HANDLE   hThread   =   NULL; 
bool   bStop   =   false; 
bool   bExit   =   false; 
char   Buff[1024]; 
OVERLAPPED   ov; 

DWORD   WINAPI   ThreadProc(LPVOID   lpParameter) 
{ 
        DWORD   Read   =   0; 
        memset(&ov,   0,   sizeof(ov)); 
        ov.hEvent   =   CreateEvent(NULL,   false,   NULL,   NULL); 

        while   (!bStop) 
        { 
                if   (!ReadDirectoryChangesW(hDir, 
                        Buff, 
                        1024, 
                        false, 
                        FILE_NOTIFY_CHANGE_FILE_NAME, 
                        &Read, 
                        &ov, 
                        NULL)) 
                { 
                        break; 
                } 
                if   (WaitForSingleObject(ov.hEvent,   INFINITE)   ==   WAIT_OBJECT_0     ) 
                { 
                        if   (!bStop) 
                        { 
                                ShowMessage( "Change "); 
                        } 
                } 
        } 
        CloseHandle(ov.hEvent); 
        bExit   =   true; 
} 
void   __fastcall   TForm1::Button1Click(TObject   *Sender) 
{ 
        hDir   =   CreateFile(   "c:\\xxx ",   //文件目录 
                        FILE_LIST_DIRECTORY, 
                        FILE_SHARE_READ   |FILE_SHARE_WRITE   |FILE_SHARE_DELETE, 
                        NULL, 
                        OPEN_EXISTING, 
                        FILE_FLAG_BACKUP_SEMANTICS   |FILE_FLAG_OVERLAPPED, 
                        NULL); 
        if   (hDir   ==   INVALID_HANDLE_VALUE   ) 
        { 
                return; 
        } 

        hThread   =   CreateThread(NULL,   0,   ThreadProc,   NULL,   0   ,   NULL); 
} 
//--------------------------------------------------------------------------- 

void   __fastcall   TForm1::Button2Click(TObject   *Sender) 
{ 
        bStop   =   true; 
        SetEvent(ov.hEvent);     //使可以退出 
        while   (!bExit) 
        { 
                Application-> ProcessMessages(); 
                Sleep(100); 
        } 
        CloseHandle(hThread); 
        CloseHandle(hDir); 
} */