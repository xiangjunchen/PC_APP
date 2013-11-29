#include "StdAfx.h"
#include "TPNetInterface.h"
#include "TPUdpSocket.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern void TP_GetSocketError();
void   TPfunMulticastCallBack( LPBYTE lpBuffer, long lLength,LPVOID pParam,LPCTSTR sLocalIP) ;
/////////////////////////////////////////////////////////// CTPListenThread
IMPLEMENT_DYNCREATE(CTPListenThread, CWinThread)

BEGIN_MESSAGE_MAP(CTPListenThread, CWinThread)
END_MESSAGE_MAP()

CTPListenThread::CTPListenThread()
{
	m_pTcpListenSocket1 = NULL;
	m_pTcpListenSocket2 = NULL;
	m_pUdpListenSocket1 = NULL;
	m_pUdpListenSocket2 = NULL;
	m_pTeamCmdProxy     = NULL;
	m_hFinishEvent      = ::CreateEvent(NULL,FALSE,FALSE,NULL);
 }

CTPListenThread::CTPListenThread(UINT nIPort)
{
	m_pTcpListenSocket1  = NULL;
	m_pTcpListenSocket2  = NULL;
	m_pUdpListenSocket1  = NULL;
	m_pUdpListenSocket2  = NULL;
	m_pTeamCmdProxy      = NULL;
	m_hFinishEvent       = ::CreateEvent(NULL,FALSE,FALSE,NULL);
}

CTPListenThread::~CTPListenThread()
{
	::CloseHandle(m_hFinishEvent);
}


BOOL CTPListenThread::InitInstance()
{
	BOOL        bCheck = FALSE;

	ASSERT(m_pTcpListenSocket1 == NULL);

	CString		strLocalIP	= _T("");	
	UINT		nLocalPort	= 0; 

	TPMutex stuLock(_T("ListenSocket"));

	
	//TCP
	if(m_aLocalIP.GetSize()>0)
	{	
		m_pTcpListenSocket1  = new CTPTcpListenSocket(m_pTeamCmdProxy);
		m_pTcpListenSocket1->m_sListenIP = m_aLocalIP[0];
		UINT uPort = m_pTeamCmdProxy ->m_uPort;
		while(uPort <m_pTeamCmdProxy ->m_uPort +10)
		{
			//流式(SOCK_STREAM)Socket是一种面向连接的Socket，针对于面向连接的TCP服务应用
			bCheck	= m_pTcpListenSocket1->Create(uPort,SOCK_STREAM,FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,m_aLocalIP[0]); 
			if(!bCheck)
			{
				TP_GetSocketError();
				uPort ++;
			}
			else 
				break;
		}
		if(uPort != m_pTeamCmdProxy ->m_uPort)
		{
			m_pTeamCmdProxy ->m_uPort = uPort;
			m_pTeamCmdProxy ->LoginInUser(FALSE);//重新通知新的端口
		}	
		bCheck  = m_pTcpListenSocket1->Listen();                                ASSERT(bCheck);//指示内核接受指向此套接口的连接请求,导致套接口有closed状态转换到listen状态
		bCheck	= m_pTcpListenSocket1->GetSockName(strLocalIP, nLocalPort);   	ASSERT(bCheck);
	}
	if(m_aLocalIP.GetSize()>1)
	{
		m_pTcpListenSocket2  = new CTPTcpListenSocket(m_pTeamCmdProxy);
		m_pTcpListenSocket2->m_sListenIP = m_aLocalIP[1];
		bCheck	= m_pTcpListenSocket2->Create(m_pTeamCmdProxy ->m_uPort,SOCK_STREAM,FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,m_aLocalIP[1]); 
		bCheck  = m_pTcpListenSocket2->Listen();                               	ASSERT(bCheck);
		bCheck	= m_pTcpListenSocket2->GetSockName(strLocalIP, nLocalPort);   	ASSERT(bCheck);
	}

	//UDP
	if(m_aLocalIP.GetSize()>0)
	{	
		m_pUdpListenSocket1 = new CTPMultiUdpSocket();
		m_pUdpListenSocket1 ->m_strLocalIP = m_aLocalIP[0];
		m_pUdpListenSocket1->m_aThisIP.Copy(m_aLocalIP);
		m_pUdpListenSocket1 ->CreateListenSocket(TP_MULTICAST_ADRESS,TP_CLIPSERVER_PORT);
		m_pUdpListenSocket1->m_fMulticastCallBack = TPfunMulticastCallBack;
		m_pUdpListenSocket1->m_pMulticastParam    = (LPVOID)m_pTeamCmdProxy;
	}
	if(m_aLocalIP.GetSize()>1)
	{	
		m_pUdpListenSocket1 = new CTPMultiUdpSocket();
		m_pUdpListenSocket1 ->m_strLocalIP = m_aLocalIP[0];
		m_pUdpListenSocket1->m_aThisIP.Copy(m_aLocalIP);
		m_pUdpListenSocket1 ->CreateListenSocket(TP_MULTICAST_ADRESS,TP_CLIPSERVER_PORT);
		m_pUdpListenSocket1->m_fMulticastCallBack = TPfunMulticastCallBack;
		m_pUdpListenSocket1->m_pMulticastParam    = (LPVOID)m_pTeamCmdProxy;
	}
	::SetEvent(m_hFinishEvent);
	return TRUE;
}
 
int CTPListenThread::ExitInstance()
{
	if(m_pTcpListenSocket1)
	{
		m_pTcpListenSocket1->Close();
		delete m_pTcpListenSocket1;
		m_pTcpListenSocket1	= NULL;
	}		
	if(m_pTcpListenSocket2)
	{
		m_pTcpListenSocket2->Close();
		delete m_pTcpListenSocket2;
		m_pTcpListenSocket2	= NULL;		
	}	
	m_pTeamCmdProxy ->ReleaseAcceptSocket(NULL);

	if(m_pUdpListenSocket1)
	{
		m_pUdpListenSocket1 ->CloseListenSocket();
		delete m_pUdpListenSocket1;
		m_pUdpListenSocket1 = NULL;
	}	
	if(m_pUdpListenSocket2)
	{
		m_pUdpListenSocket2 ->CloseListenSocket();
		delete m_pUdpListenSocket2;
		m_pUdpListenSocket2 = NULL;
	}	
	::SetEvent(m_hFinishEvent);
	
	return CWinThread::ExitInstance();
}


/////////////////////////////////////////////////////////// CTPListenThread

CTPNetInterface::CTPNetInterface()
{	 
	m_sUser     = _T("");
	m_sComputer = _T("");
	m_sIP       = _T("");
 	ZeroMemory(m_bIP,sizeof(m_bIP));

	m_pListenThread = NULL;
 
	m_bReciveThread = FALSE;
	m_hReciveThread = NULL;  
 
 


	m_uOpenSate     = 0;

	m_hLoginWnd      = NULL;
	m_hMessageWnd    = NULL;

	m_fClipDisplayCmdCallBack = NULL;
	m_pTeamCmdParam    = NULL;

	m_fAskCmdCallBack  = NULL; 
	m_pAskCmdParam     = NULL;  

	m_fBrdCmdCallBack  = NULL; // 
	m_pBrdCmdParam     = NULL; // 

	m_fFileCmdCallBack = NULL;

	m_fDriverCallBack  = NULL;
	m_lDriverParam     = NULL;


	m_pUdpSendSocket   = NULL;
 
	InitializeCriticalSection(&m_csTeamCmd);

	m_bOpen         = FALSE;
	srand((unsigned)time(NULL));
	m_uPort         = TP_NS_PORT + rand()%100;
	m_cAppType      = 0;

 

}

CTPNetInterface::~CTPNetInterface(void)
{
	DeleteCriticalSection(&m_csTeamCmd);
	m_aThisIP.RemoveAll();
}
void CTPNetInterface::ResetCallBack(BOOL bSet)
{
	m_fClipDisplayCmdCallBack = NULL;

	return ;
}
void   TP_GetLoginUserInfo(CString &sLoginUser,CString &sComputer,CString &sIP)
{
	CTPNetInterface stuInfo;
	stuInfo.GetUserInfo();
	sLoginUser = stuInfo.m_sUser;
	sComputer  = stuInfo.m_sComputer;
	sIP        = stuInfo.m_sIP;
}

HHOOK         g_MessageHook = NULL;
LRESULT CALLBACK TP_MessageHookProc( int nCode, WPARAM wParam, LPARAM lParam)
{ 
	if( nCode == HC_ACTION)
	{
		MSG  *pMsg = (MSG *)lParam;
		if(pMsg ->message == 0XC801)
		{
			ASSERT(0);
		}
	}
	return CallNextHookEx( g_MessageHook, nCode, wParam, lParam);
}
LRESULT TP_InstallHook()
{
	if(g_MessageHook == NULL)
	{
		g_MessageHook = SetWindowsHookEx(WH_GETMESSAGE,
			(HOOKPROC)TP_MessageHookProc,
			(HINSTANCE)0,::GetCurrentThreadId());

	}
	return S_OK;
}
void TP_UnInstallHook()
{
	if(g_MessageHook) UnhookWindowsHookEx( g_MessageHook);
	g_MessageHook = NULL;	
}
BOOL EnumerateNetComputer( LPNETRESOURCE lpnr,TPComputerArray &aComputer,tag_TPComputer *pstuComputer,BOOL bPath) 
{ 
	DWORD         dwRet     = 0, dwRetEnum = 0; 
	HANDLE        hEnum     = NULL; 
	DWORD         cbBuffer  = 16384; 
	DWORD         cEntries  = 0xFFFFFFFF; 
	LPNETRESOURCE lpnrLocal = NULL; 
	DWORD         i         = 0; 

	CString str; 
	int     nLevel = -1; 
	CString strDisplayName; 
	CString strTrueName; 
	BOOL    bEnumChild = FALSE;
	HOSTENT*pHostEnt   = NULL;  

	dwRet = WNetOpenEnum( RESOURCE_GLOBALNET, RESOURCETYPE_ANY, 0, lpnr, &hEnum ); 
	if( dwRet == WN_FUNCTION_BUSY || dwRet == ERROR_BAD_NET_NAME ) 
		return TRUE; 
	else if( dwRet != NO_ERROR )  
		return FALSE; 
	lpnrLocal = (LPNETRESOURCE)GlobalAlloc( GPTR, cbBuffer ); 
	do 
	{ 
		dwRetEnum = WNetEnumResource( hEnum, &cEntries, lpnrLocal, &cbBuffer); 
		if( dwRetEnum == NO_ERROR)
		{ 
			for( i = 0; i < cEntries; i++ ) 
			{ 
				// 只搜索网络提供者为"Microsoft Network"的资源 
				str = (LPCTSTR)lpnrLocal[i].lpProvider;
				if(str.Find(_T("Microsoft"))<0 || str.Find(_T("Network"))<0) 
					continue;

				if( RESOURCEUSAGE_CONTAINER == ( lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER ) )
				{ 
					nLevel = -1; 
					switch( lpnrLocal[i].dwDisplayType )
					{ 
					case RESOURCEDISPLAYTYPE_NETWORK: // 整个网络 
						nLevel      = 0;
						bEnumChild  = TRUE;
						strTrueName = lpnrLocal[i].lpRemoteName; 
						if( !EnumerateNetComputer( &lpnrLocal[i],aComputer,NULL,FALSE))
							goto ErrorEnum;
						break; 
					case RESOURCEDISPLAYTYPE_DOMAIN: // 工作组 
						nLevel      = 1; 
						bEnumChild  = TRUE;						 
						strTrueName = lpnrLocal[i].lpRemoteName; 
						if( !EnumerateNetComputer( &lpnrLocal[i],aComputer,NULL,FALSE))
							goto ErrorEnum;
						break; 
					case RESOURCEDISPLAYTYPE_SERVER: // 计算机 
						nLevel = 2; 						 
						strTrueName.Format(_T("%s"),lpnrLocal[i].lpRemoteName + 2); 
						USES_CONVERSION;
						pHostEnt	= gethostbyname(W2A(strTrueName));
					 
						if(pHostEnt != NULL)
						{
							char		**pIPAddress = NULL;
							for(pIPAddress = pHostEnt->h_addr_list; *pIPAddress != NULL; pIPAddress++)
							{			 							 
								in_addr *pIPAddr = (in_addr *)*pIPAddress;
								if(pIPAddr->S_un.S_un_b.s_b1 == 0) continue;
								strDisplayName		= inet_ntoa(*pIPAddr); 		
								break;
							}
						}
						else 
							break;
						tag_TPComputer stuComputer;
						stuComputer.sComputer = strTrueName;
						stuComputer.sIP       = strDisplayName;
						if(EnumerateNetComputer( &lpnrLocal[i],aComputer,&stuComputer,FALSE))
							aComputer.Add(stuComputer);	
						break; 
					} 						
				} 
				else if( RESOURCEUSAGE_CONNECTABLE == ( lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONNECTABLE ) )
				{ 
					nLevel = -1; 
					switch( lpnrLocal[i].dwDisplayType )
					{ 
					case RESOURCEDISPLAYTYPE_SHARE: // 共享目录 
						if(pstuComputer)
						{
							strTrueName = lpnrLocal[i].lpRemoteName; 
							pstuComputer ->aShareFile.Add(strTrueName);
						}						 
						break; 					 
					} 						
				} 
			} 
		} 
		else if( dwRetEnum != ERROR_NO_MORE_ITEMS )  
			goto ErrorEnum;
	} 
	while( dwRetEnum != ERROR_NO_MORE_ITEMS ); 

	GlobalFree( (HGLOBAL)lpnrLocal ); 
	dwRet = WNetCloseEnum( hEnum ); 
	if( dwRet != NO_ERROR ) 
		return FALSE; 
	return TRUE; 
ErrorEnum:
	GlobalFree( (HGLOBAL)lpnrLocal ); 
	dwRet = WNetCloseEnum( hEnum ); 
	return FALSE;
} 
BOOL CTPNetInterface::Open()
{
	if(m_bOpen) return TRUE;
	m_bOpen = TRUE;

	GetUserInfo();
	if(m_sIP.GetLength()<4)
	{
		m_uOpenSate = 0;
		return FALSE;
	}
	OpenListen();
	OpenUser();
 
	return FALSE;
}
BOOL CTPNetInterface::Close()
{
	m_hHeartCount.EndThread(); // modify by xjc 确保heartcount线程退出

	if(!m_bOpen) return TRUE;
	m_bOpen = FALSE;

	if(m_uOpenSate == 0) return TRUE;
	m_uOpenSate |= 0x10000000;
 
	CloseListen();
	CloseUser();
	return FALSE;
}
BOOL CTPNetInterface::Release()
{ 
	if(m_uOpenSate == 0) return S_OK;
	CloseSendCmd();
	m_uOpenSate = 0;
 	return TRUE;
}
void CTPNetInterface::GetLocalIP(tag_TPNetCmdUseInfo *pInfo,LPCTSTR sLocalIP)
{
	USES_CONVERSION_EX;
	LPSTR lpszAscii = T2A_EX((LPTSTR)sLocalIP, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
	BYTE  bIP[4];
	SOCKADDR_IN saLocal; 
	saLocal.sin_addr.s_addr = inet_addr(lpszAscii); 

	bIP[0] = saLocal.sin_addr.S_un.S_un_b.s_b1;
	bIP[1] = saLocal.sin_addr.S_un.S_un_b.s_b2;
	bIP[2] = saLocal.sin_addr.S_un.S_un_b.s_b3;
	bIP[3] = saLocal.sin_addr.S_un.S_un_b.s_b4;

	memcpy_s(pInfo->  cIP,4,bIP,4);
	memcpy_s(pInfo->  bIP,4,bIP,4);
	lstrcpyn(pInfo->  sIP,sLocalIP,15);

}
void CTPNetInterface::GetTeamCmdUseInfo(tag_TPNetCmdUseInfo *pInfo)
{
	memcpy_s(pInfo->  cIP,4,m_bIP,4);
	memcpy_s(pInfo->  bIP,4,m_bIP,4);
	lstrcpyn(pInfo->  sUser,m_sUser,50);
	lstrcpyn(pInfo->  sTeamUser,m_stuUserState.sTeamUser,TP_TEAMUSER_MAX-1);
	lstrcpyn(pInfo->  sComputer,m_sComputer,TP_TEAMUSER_MAX-1);
	lstrcpyn(pInfo->  sIP,m_sIP,15);
	pInfo->cAppType   = m_cAppType;
	pInfo->uPort      = m_uPort;
	pInfo-> lSendSize = sizeof(tag_TPNetCmdUseInfo);
	pInfo->lBufSize   = sizeof(tag_TPNetCmdUseInfo);

	pInfo->lDataSize   = 0;
}
void CTPNetInterface::ReciveDataThread()
{
 	BOOL     bCheck      = FALSE;
	in_addr *pIPAddr     = NULL;
	CString  sIP         = _T("");
	HWND     hSocketWnd  = NULL;
 
	WSADATA		wsaData;
	WORD wVersion	= MAKEWORD(2, 2);
	int  iCheck     = WSAStartup(wVersion, &wsaData);

	while(m_bReciveThread)
	{
		CArray<tag_TPReciveData *,tag_TPReciveData * &>aDataEx;
		EnterCriticalSection(&m_csTeamCmd);
		aDataEx.Copy(m_aReciveData);
		m_aReciveData.RemoveAll();
 		LeaveCriticalSection(&m_csTeamCmd);

		if(aDataEx.GetSize()<=0) break;

		for(INT j=0;j<aDataEx.GetSize();j++)
		{
			if(!m_bReciveThread)		break;
 			tag_TPNetCmdHead *pTeamCmd = (tag_TPNetCmdHead *)aDataEx[j]->pBuffer;
			if(pTeamCmd ->dwFlag != TP_CMDFLAG)      continue;
			if(pTeamCmd->lSendSize >aDataEx[j]->lSize)       continue;
			
			if(pTeamCmd ->eTeamCmd == TPNetCmd_Open )
			{
				tag_TPNetCmdUseInfo *pData = (tag_TPNetCmdUseInfo *)aDataEx[j]->pBuffer;
				pIPAddr = (in_addr*)pData->bIP;
				if(pIPAddr->S_un.S_un_b.s_b1 == 0) continue;
				sIP = inet_ntoa(*pIPAddr);

				tag_TPUser stuUser,*pUser = NULL;
				stuUser.sIP        = sIP;
				stuUser.sUser      = pData ->sUser;
				stuUser.sTeamUser  = pData ->sTeamUser;
				stuUser.sComputer  = pData ->sComputer;
				stuUser.cAppType   = pData ->cAppType;
				stuUser.uPort      = pData ->uPort;
				stuUser.dwTime     = GetTickCount();
				stuUser.sLocalIP   = aDataEx[j]->sLocalIP;  
				memcpy_s(stuUser.bIP,4,pData ->bIP,4);

				if(!SendConnect(stuUser))
				{
					SetConnectState(sIP,FALSE,TRUE);					
					continue;
				}

				EnterCriticalSection(&m_csTeamCmd);
 				for(INT m = m_aUser.GetSize()-1;m>=0;m--)
				{
					if(m_aUser[m].sIP == sIP)		
					{
						m_aUser[m].iConnectFail = 0;
						m_aUser[m].cAppType     = pData ->cAppType;
						m_aUser[m].uPort        = pData ->uPort;
						m_aUser[m].sTeamUser    = pData ->sTeamUser;
						m_aUser[m].sLocalIP     = aDataEx[j]->sLocalIP;
						m_aUser[m].dwTime       = GetTickCount();
 						pUser = &m_aUser[m]; 
						break;
					}
				}	
				if(pUser == NULL)
				{ 	
					m_aUser.Add(stuUser);
					pUser = &m_aUser[m_aUser.GetSize()-1]; 
				}
				else pUser = NULL;
				LeaveCriticalSection(&m_csTeamCmd);

				if(pUser)   	SendUserLogin(pUser,TRUE);

			}
			else if(pTeamCmd ->eTeamCmd  == TPNetCmd_Connect )
			{
				tag_TPNetCmdUseInfo *pData = (tag_TPNetCmdUseInfo *)aDataEx[j]->pBuffer;
				in_addr *pIPAddr = (in_addr*)pData->bIP;
				if(pIPAddr->S_un.S_un_b.s_b1 == 0) continue;
				sIP = inet_ntoa(*pIPAddr);

				tag_TPUser stuUser, *pUser = NULL;
				stuUser.sIP           = sIP;
				stuUser.sUser         = pData ->sUser;
				stuUser.sTeamUser     = pData ->sTeamUser;
				stuUser.sComputer     = pData ->sComputer;
				stuUser.cAppType      = pData ->cAppType;
				stuUser.uPort         = pData ->uPort;
				stuUser.guidTeamUser  = pData ->guidTeamUser;
				stuUser.guidOpenPrj   = pData ->guidOpenPrj;    
				stuUser.guidOpenPro   = pData ->guidOpenPro;  
				stuUser.guidOpenEff   = pData ->guidOpenEff;  
				stuUser.guidRedord    = pData ->guidRedord;  
				stuUser.dwTime        = GetTickCount();
				stuUser.sLocalIP      = aDataEx[j]->sLocalIP;
				memcpy_s(stuUser.bIP,4,pData ->bIP,4);

				EnterCriticalSection(&m_csTeamCmd);
 				for(INT m = m_aUser.GetSize()-1;m>=0;m--)
				{					 			 
					if(m_aUser[m].sIP == sIP)		
					{
						m_aUser[m].iConnectFail  = 0;
						m_aUser[m].cAppType      = pData ->cAppType;
						m_aUser[m].uPort         = pData ->uPort;
						m_aUser[m].sTeamUser     = pData ->sTeamUser;
						m_aUser[m].guidTeamUser  = pData ->guidTeamUser;
						m_aUser[m].guidOpenPrj   = pData ->guidOpenPrj;
						m_aUser[m].guidOpenPro   = pData ->guidOpenPro;
						m_aUser[m].guidOpenEff   = pData ->guidOpenEff;  
						m_aUser[m].guidRedord    = pData ->guidRedord;  
						m_aUser[m].sLocalIP      = aDataEx[j]->sLocalIP;						
						m_aUser[m].dwTime        = GetTickCount();
						pUser = &m_aUser[m];
						break;
					}					  
				}	
				if(pUser == NULL)
				{
					m_aUser.Add(stuUser);
					pUser = &m_aUser[m_aUser.GetSize()-1];
				}
				else	pUser = NULL;					
				LeaveCriticalSection(&m_csTeamCmd);

				if(pUser )	
				{
					SendUserLogin(pUser,TRUE);
					StartHeartCount();
				}
				m_uOpenSate |= 0x00000004;
			} 		
			else if(pTeamCmd ->eTeamCmd  == TPNetCmd_Close )
			{
				BOOL                 bFind  = FALSE;
				tag_TPNetCmdUseInfo *pData = (tag_TPNetCmdUseInfo *)aDataEx[j]->pBuffer;
 				in_addr *pIPAddr = (in_addr*)pData->bIP;
				if(pIPAddr->S_un.S_un_b.s_b1 == 0) continue;
				sIP = inet_ntoa(*pIPAddr);

				EnterCriticalSection(&m_csTeamCmd);
				for(INT l = m_aUser.GetSize()-1;l>=0;l--)
				{
					if(m_aUser[l].sIP == sIP)
					{
						if(!bFind)	m_stuUserInfo = m_aUser[l];
						bFind = TRUE;
						m_aUser.RemoveAt(l);
					}
				}				 
				LeaveCriticalSection(&m_csTeamCmd);

				if(bFind) SendUserLogin(&m_stuUserInfo,FALSE);

				CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(m_stuUserInfo.GetUserIP());
				if(pThread) 
				{
					pThread ->Stop();
					pThread ->sTP = _T("");
					ASSERT(NULL == pThread->pSocket);
				}
			} 
			else if(pTeamCmd ->eTeamCmd  == TPNetCmd_MsgString)
			{
				LPBYTE pBuffer = (LPBYTE)aDataEx[j]->pBuffer + sizeof(tag_TPNetCmdHead);
				long   lSize   = pTeamCmd->lDataSize;
				pBuffer[lSize] = 0;
				pBuffer[lSize+1] = 0;
				if(m_hMessageWnd)
				{
					tag_TPUser stuUser;
					EnterCriticalSection(&m_csTeamCmd);
					for(INT m = m_aUser.GetSize()-1;m>=0;m--)
					{
						if(m_aUser[m].bIP[2] == pTeamCmd->cIP[2] && m_aUser[m].bIP[3] == pTeamCmd->cIP[3])
						{
							stuUser = m_aUser[m];
							break;
						}
					}
					LeaveCriticalSection(&m_csTeamCmd);
					CString sMessage = (LPCTSTR)pBuffer;
					int     iIP      = sMessage.ReverseFind('\a');
					stuUser.sMessage = sMessage.Mid(iIP+1);
					stuUser.sIP      = sMessage.Left(iIP);
					stuUser.lParam1  = pTeamCmd->lParam1;
					::SendMessage(m_hMessageWnd,WM_TEAMUSER,TPTeamMsg_MessageString,(LPARAM)&stuUser);
				}
			}
			else if(pTeamCmd ->eTeamCmd  == TPNetCmd_MsgData)
			{
				LPBYTE  pBuffer  = (LPBYTE)aDataEx[j]->pBuffer + sizeof(tag_TPNetCmdHead);
				LPBYTE  pData    = pBuffer;
				LPBYTE  pMsgData = NULL;
				DWORD   dwMsgLen = 0;
 				CString sIP;
				CString sTitle;

				if(m_hMessageWnd)
				{
					struct tag_MsgData
					{
						int  iDataOff;
						int  iDataLen;
						int  iDataSrc;
						int  iIPOff;
						int  iIPLen;
						int  iTitleOff;
						int  iTitleLen;
						UINT  uDataType;
					}*stuData = (tag_MsgData *)pBuffer;
					pMsgData  = new BYTE[stuData ->iDataSrc +200];//解出来多了130BYTE

				 
					dwMsgLen = RLE_Decode( pBuffer + stuData ->iDataOff,stuData ->iDataLen, pMsgData);

					ASSERT(stuData->iDataSrc+200 >= (int)dwMsgLen);

					pData     = pBuffer + stuData ->iIPOff;
					pData[stuData ->iIPLen-1] = 0;
					pData[stuData ->iIPLen-2] = 0;

					sIP      = (LPCTSTR)pData;

					pData     = pBuffer + stuData ->iTitleOff;
					pData[stuData ->iTitleLen-1] = 0;
					pData[stuData ->iTitleLen-2] = 0;

					sTitle      = (LPCTSTR)pData;


					tag_TPUser stuUser;
					EnterCriticalSection(&m_csTeamCmd);
					for(INT m = m_aUser.GetSize()-1;m>=0;m--)
					{
						if(m_aUser[m].bIP[2] == pTeamCmd->cIP[2] && m_aUser[m].bIP[3] == pTeamCmd->cIP[3])
						{
							stuUser = m_aUser[m];
							break;
						}
					}
					LeaveCriticalSection(&m_csTeamCmd);


					stuUser.sMessage = sTitle;
					stuUser.sIP      = sIP;
					stuUser.lParam1  = pTeamCmd->lParam1;
					stuUser.lParam2  = (LPARAM)pMsgData;
					stuUser.lParam3  = dwMsgLen;
					stuUser.lParam4  = stuData ->uDataType;


					::SendMessage(m_hMessageWnd,WM_TEAMUSER,TPTeamMsg_MessageData,(LPARAM)&stuUser);

					delete [] pMsgData;

				}
			}
			else if(pTeamCmd ->eTeamCmd  ==TPNetCmd_ClipDisplayCmd)
			{
				LPBYTE pBuffer = (LPBYTE)aDataEx[j]->pBuffer + sizeof(tag_TPNetCmdHead);
				long   lSize   = pTeamCmd->lDataSize;
				if(m_fClipDisplayCmdCallBack)
					m_fClipDisplayCmdCallBack(pBuffer,lSize,m_pTeamCmdParam,NULL);
			}	
			else if(pTeamCmd ->eTeamCmd  ==TPNetCmd_BrdCmd)
			{
				LPBYTE pBuffer = (LPBYTE)aDataEx[j]->pBuffer + sizeof(tag_TPNetCmdHead);
				long   lSize   = pTeamCmd->lDataSize;
				if(m_fBrdCmdCallBack)
					m_fBrdCmdCallBack(pBuffer,lSize,m_pBrdCmdParam,NULL);
			}	
			else if(pTeamCmd ->eTeamCmd  == TPNetCmd_SetUserList)
			{
				LPBYTE pBuffer = (LPBYTE)aDataEx[j]->pBuffer + sizeof(tag_TPNetCmdHead);
				long   lSize   = pTeamCmd->lDataSize;
				pBuffer[lSize] = 0;
				pBuffer[lSize+1] = 0;
				CString sMessage = (LPCTSTR)pBuffer;
				struct tag_AskUserlist
				{
					DWORD dwWndType;
					CStringArray aIP;
				}stuData;
				stuData.dwWndType = pTeamCmd ->lParam1;
				while(sMessage.GetLength()>0)
				{
					int iFind = sMessage.Find('\t');
					if(iFind<0) break;
					CString sIP = sMessage.Left(iFind);
					sMessage = sMessage.Mid(iFind+1);
					if(sIP.GetLength()>3)
						stuData.aIP.Add(sIP);
				}
				if(m_hMessageWnd && stuData.aIP.GetSize()>0)		 
					::SendMessage(m_hMessageWnd,WM_TEAMUSER,TPTeamMsg_SetUserList,(LPARAM)&stuData);
			}
			else if(pTeamCmd ->eTeamCmd  == TPNetCmd_WndState)
			{
				if(m_hMessageWnd)		 
					::SendMessage(m_hMessageWnd,WM_TEAMUSER,TPTeamMsg_WndState,(LPARAM)&pTeamCmd);
			}
			else if (pTeamCmd->eTeamCmd	 == TPNetCmd_AskVoiceChat)
			{
				LPBYTE pBuffer = (LPBYTE)aDataEx[j]->pBuffer + sizeof(tag_TPNetCmdHead);
				long   lSize   = pTeamCmd->lDataSize;
				pBuffer[lSize] = 0;
				pBuffer[lSize+1] = 0;
				if(m_hMessageWnd)
				{
					tag_TPUser stuUser;
					EnterCriticalSection(&m_csTeamCmd);
					for(INT m = m_aUser.GetSize()-1;m>=0;m--)
					{
						if(m_aUser[m].bIP[2] == pTeamCmd->cIP[2] && m_aUser[m].bIP[3] == pTeamCmd->cIP[3])
						{
							stuUser = m_aUser[m];
							break;
						}
					}
					LeaveCriticalSection(&m_csTeamCmd);
					CString sMessage = (LPCTSTR)pBuffer;
					int     iIP      = sMessage.ReverseFind('\a');
					stuUser.sMessage = sMessage.Mid(iIP+1);
					stuUser.sIP      = sMessage.Left(iIP);
					stuUser.lParam1  = pTeamCmd->lParam1;
					stuUser.lParam4  = 1;//voice
					::SendMessage(m_hMessageWnd,WM_TEAMUSER,TPTeamMsg_AskVoiceChat,(LPARAM)&stuUser);
				}
			}
			else if (pTeamCmd->eTeamCmd  == TPNetCmd_BeginVoiceChat)
			{
				LPBYTE pBuffer = (LPBYTE)aDataEx[j]->pBuffer + sizeof(tag_TPNetCmdHead);
				long   lSize   = pTeamCmd->lDataSize;
				pBuffer[lSize] = 0;
				pBuffer[lSize+1] = 0;
				if(m_hMessageWnd)
				{
					tag_TPUser stuUser;
					EnterCriticalSection(&m_csTeamCmd);
					for(INT m = m_aUser.GetSize()-1;m>=0;m--)
					{
						if(m_aUser[m].bIP[2] == pTeamCmd->cIP[2] && m_aUser[m].bIP[3] == pTeamCmd->cIP[3])
						{
							stuUser = m_aUser[m];
							break;
						}
					}
					LeaveCriticalSection(&m_csTeamCmd);
					CString sMessage = (LPCTSTR)pBuffer;
					int     iIP      = sMessage.ReverseFind('\a');
					stuUser.sMessage = sMessage.Mid(iIP+1);
					stuUser.sIP      = sMessage.Left(iIP);
					stuUser.lParam1  = pTeamCmd->lParam1;
					stuUser.lParam4  = 1;//voice
					::SendMessage(m_hMessageWnd,WM_TEAMUSER,TPTeamMsg_BeginVoiceChat,(LPARAM)&stuUser);
				}
			}
			else if (pTeamCmd->eTeamCmd  == TPNetCmd_CloseVoiceChat)
			{
				LPBYTE pBuffer = (LPBYTE)aDataEx[j]->pBuffer + sizeof(tag_TPNetCmdHead);
				long   lSize   = pTeamCmd->lDataSize;
				pBuffer[lSize] = 0;
				pBuffer[lSize+1] = 0;
				if(m_hMessageWnd)
				{
					tag_TPUser stuUser;
					EnterCriticalSection(&m_csTeamCmd);
					for(INT m = m_aUser.GetSize()-1;m>=0;m--)
					{
						if(m_aUser[m].bIP[2] == pTeamCmd->cIP[2] && m_aUser[m].bIP[3] == pTeamCmd->cIP[3])
						{
							stuUser = m_aUser[m];
							break;
						}
					}
					LeaveCriticalSection(&m_csTeamCmd);
					CString sMessage = (LPCTSTR)pBuffer;
					int     iIP      = sMessage.ReverseFind('\a');
					stuUser.sMessage = sMessage.Mid(iIP+1);
					stuUser.sIP      = sMessage.Left(iIP);
					stuUser.lParam1  = pTeamCmd->lParam1;
					stuUser.lParam4  = 1;//voice
					::SendMessage(m_hMessageWnd,WM_TEAMUSER,TPTeamMsg_CloseVoiceChat,(LPARAM)&stuUser);
				}
			}
			else if (pTeamCmd->eTeamCmd  == TPNetCmd_DisChatWnd)
			{
				LPBYTE pBuffer = (LPBYTE)aDataEx[j]->pBuffer + sizeof(tag_TPNetCmdHead);
				long   lSize   = pTeamCmd->lDataSize;
				pBuffer[lSize] = 0;
				pBuffer[lSize+1] = 0;
				if(m_hMessageWnd)
				{
					tag_TPUser stuUser;
					EnterCriticalSection(&m_csTeamCmd);
					for(INT m = m_aUser.GetSize()-1;m>=0;m--)
					{
						if(m_aUser[m].bIP[2] == pTeamCmd->cIP[2] && m_aUser[m].bIP[3] == pTeamCmd->cIP[3])
						{
							stuUser = m_aUser[m];
							break;
						}
					}
					LeaveCriticalSection(&m_csTeamCmd);
					CString sMessage = (LPCTSTR)pBuffer;
					int     iIP      = sMessage.ReverseFind('\a');
					stuUser.sMessage = sMessage.Mid(iIP+1);
					stuUser.sIP      = sMessage.Left(iIP);
					stuUser.lParam1  = pTeamCmd->lParam1;
					stuUser.lParam4  = 1;//voice
					::SendMessage(m_hMessageWnd,WM_TEAMUSER,TPTeamMsg_DisChatWnd,(LPARAM)&stuUser);
				}
			}
			delete aDataEx[j];
			aDataEx[j] = NULL;
		}
		for(INT k=0;k<aDataEx.GetSize();k++){if(aDataEx[k]) delete aDataEx[k];}

	}		
		WSACleanup();

 	m_bReciveThread = FALSE;
	::SetEvent(m_hReciveThread);
}
 
void   TPfunMulticastCallBack( LPBYTE lpBuffer, long lLength,LPVOID pParam,LPCTSTR sLocalIP) 
{
	((CTPNetInterface *)pParam) ->SetReceiveData(lpBuffer,lLength,FALSE,NULL,sLocalIP);
}
void CTPNetInterface::SetTeamUser(GUID guidDBType, GUID guidTeamUser,LPCTSTR sUser)
{
	m_stuUserState.guidTeamUser.guidRes    = guidTeamUser; //非编登陆用户
	m_stuUserState.guidTeamUser.guidDBType = guidDBType;
	m_stuUserState.sTeamUser               = sUser;

	SendTickCount(); 
}
void CTPNetInterface::SendTickCount()
{	
	if(m_pUdpSendSocket)
	{
		tag_TPNetCmdUseInfo cmdData(TPNetCmd_TickCount);
		GetTeamCmdUseInfo(&cmdData);
		cmdData.guidTeamUser = m_stuUserState.guidTeamUser;
		cmdData.guidOpenPrj  = m_stuUserState.guidOpenPrj; 
		cmdData.guidOpenPro  = m_stuUserState.guidOpenPro; 
		cmdData.guidOpenEff  = m_stuUserState.guidOpenEff; 
		cmdData.guidRedord   = m_stuUserState.guidRedord; 


		for(INT l=0;l<m_aThisIP.GetSize();l++)
		{
			GetLocalIP(&cmdData,m_aThisIP[l]);
			m_pUdpSendSocket->SendSocket((LPBYTE)&cmdData,sizeof(tag_TPNetCmdUseInfo),m_aThisIP[l]);	
		}		 
	}	
}
	
void CTPNetInterface::LoginInUser(BOOL bClearUser)
{
	EnterCriticalSection(&m_csTeamCmd);
	if(bClearUser) m_aUser.RemoveAll();

	if(m_pUdpSendSocket)
	{
		tag_TPNetCmdUseInfo cmdData(TPNetCmd_Open);
		GetTeamCmdUseInfo(&cmdData);

		cmdData.guidTeamUser = m_stuUserState.guidTeamUser;
		cmdData.guidOpenPrj = m_stuUserState.guidOpenPrj; 
		cmdData.guidOpenPro = m_stuUserState.guidOpenPro; 
		cmdData.guidOpenEff = m_stuUserState.guidOpenEff; 
		cmdData.guidRedord = m_stuUserState.guidRedord; 


		for(INT l=0;l<m_aThisIP.GetSize();l++)
		{
			GetLocalIP(&cmdData,m_aThisIP[l]);
			m_pUdpSendSocket->SendSocket((LPBYTE)&cmdData,sizeof(tag_TPNetCmdUseInfo),m_aThisIP[l]);	
		}
	}	

	LeaveCriticalSection(&m_csTeamCmd);
}

void CTPNetInterface::LoginOutUser()
{	
	if(m_pUdpSendSocket)
	{
		tag_TPNetCmdUseInfo cmdData(TPNetCmd_Close);
		GetTeamCmdUseInfo(&cmdData);
		m_pUdpSendSocket->SendSocket((LPBYTE)&cmdData,sizeof(tag_TPNetCmdUseInfo),m_pUdpSendSocket ->m_strBroastIP);

	}	
}



void CTPNetInterface::OpenUser()
{	
	if(m_pUdpSendSocket == NULL)
	{
		m_pUdpSendSocket = new CTPMultiUdpSocket();
		m_pUdpSendSocket->m_aThisIP.Copy(m_aThisIP);
		m_pUdpSendSocket->CreateSendSocket();
		m_pUdpSendSocket->m_fMulticastCallBack = TPfunMulticastCallBack;
		m_pUdpSendSocket->m_pMulticastParam    = (LPVOID)this;

		tag_TPNetCmdUseInfo cmdData(TPNetCmd_Open);
		GetTeamCmdUseInfo(&cmdData);

		cmdData.guidTeamUser = m_stuUserState.guidTeamUser;
		cmdData.guidOpenPrj = m_stuUserState.guidOpenPrj; 
		cmdData.guidOpenPro = m_stuUserState.guidOpenPro; 
		cmdData.guidOpenEff = m_stuUserState.guidOpenEff; 
		cmdData.guidRedord = m_stuUserState.guidRedord; 

		cmdData.lParam1    = 0x1234; 

		for(INT l=0;l<m_aThisIP.GetSize();l++)
		{
			GetLocalIP(&cmdData,m_aThisIP[l]);
			m_pUdpSendSocket->SendSocket((LPBYTE)&cmdData,sizeof(tag_TPNetCmdUseInfo),m_aThisIP[l]);	
		}
	}	
} 
void CTPNetInterface::CloseUser()
{
	if(m_pUdpSendSocket)
	{
		tag_TPNetCmdUseInfo cmdData(TPNetCmd_Close);
		GetTeamCmdUseInfo(&cmdData);
		m_pUdpSendSocket->SendSocket((LPBYTE)&cmdData,sizeof(tag_TPNetCmdUseInfo),m_pUdpSendSocket->m_strLocalIP);
		m_pUdpSendSocket ->CloseSendSocket();
		delete m_pUdpSendSocket;
		m_pUdpSendSocket = NULL;
	}	
}


void CTPNetInterface::CloseSendCmd()
{
	m_uOpenSate |= 0x20000000;

	CArray<tag_TPTeaMsgThread *,tag_TPTeaMsgThread *&> aSendThread;
	EnterCriticalSection(&m_csTeamCmd);
	aSendThread.Copy(m_aSendThread);
	LeaveCriticalSection(&m_csTeamCmd);

 
	for(INT l=0;l<aSendThread.GetSize();l++)
		aSendThread[l]->Stop();

	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<m_aSendThread.GetSize();l++)
		delete m_aSendThread[l];
	m_aSendThread.RemoveAll();
	LeaveCriticalSection(&m_csTeamCmd);
}
UINT TP_ReciveThread(LPVOID *pVoid)
{
	((CTPNetInterface *)pVoid) ->ReciveDataThread();
	return 0;

}
 
void CTPNetInterface::OpenListen()
{
	m_uOpenSate |= 0x00000010;

	m_bReciveThread   = FALSE;
	m_hReciveThread   = ::CreateEvent(NULL,TRUE,FALSE,NULL);

	m_pListenThread   = new CTPListenThread(m_uPort); //必须创建界面线程，避免主线程堵塞造成消息无法接收
	m_pListenThread ->m_pTeamCmdProxy = this;
	m_pListenThread ->m_aLocalIP.Copy(m_aThisIP);
	m_pListenThread ->CreateThread();
	m_pListenThread ->SetThreadPriority(THREAD_PRIORITY_HIGHEST);//防止消息丢失

	::WaitForSingleObject(m_pListenThread ->m_hFinishEvent,1000000);
	::ResetEvent(m_pListenThread ->m_hFinishEvent);
 }
void CTPNetInterface::CloseListen()
{
	m_uOpenSate |= 0x00000020;
	 
	if(m_pListenThread)
	{	 
		::PostThreadMessage(m_pListenThread->m_nThreadID,WM_QUIT,0,0);
		::WaitForSingleObject(m_pListenThread ->m_hFinishEvent,1000000);
		m_pListenThread = NULL;
	}
	
	
	if(m_bReciveThread)
	{
		m_bReciveThread = FALSE;
 		::WaitForSingleObject(m_hReciveThread,10000000);
	}
	::CloseHandle(m_hReciveThread);

	
 
	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<m_aSocket.GetSize();l++)
	{
		m_aSocket[l] ->Close();
		delete m_aSocket[l];
	}	 
	m_aSocket.RemoveAll();

	for(INT l=0;l<m_aReciveData.GetSize();l++)
	{
		delete  m_aReciveData[l];
	}	 
	m_aReciveData.RemoveAll(); 

	LeaveCriticalSection(&m_csTeamCmd); 

}
UINT TP_ProxyHeartCount(LPVOID pVoid,BOOL *pbFlag,CTPBaseThread *pThread)
{
	CTPNetInterface *pProxy = (CTPNetInterface *)pVoid;
	while(*pbFlag)
	{
		pThread->WaitDataEvent(TP_HEARTCOUNT);
		if(*pbFlag == FALSE) break;
		pProxy ->SendTickCount();
	}
	*pbFlag = FALSE;
	return 1;
}
void CTPNetInterface::StartHeartCount()
{
	if(!m_hHeartCount.IsRun())
		m_hHeartCount.BeginThread(TP_ProxyHeartCount,this);
}

void CTPNetInterface::SetConnectState(CString sIP,BOOL bConnect,BOOL bRemove)
{
	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<m_aUser.GetSize();l++)
	{
		if(m_aUser[l].sIP == sIP)
		{
			if(bConnect) m_aUser[l].iConnectFail = 0;
			else      
			{
				m_aUser[l].iConnectFail ++;
				if(bRemove) m_aUser.RemoveAt(l);
			}
			break;
		}
	}
	LeaveCriticalSection(&m_csTeamCmd);
}
void CTPNetInterface::GetLoginUser(TPTeamUserArray &aUser,int iFailConnect)
{
	DWORD iOpenCount = GetTickCount();
	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<m_aUser.GetSize();l++)
	{	 
		if(m_aUser[l].iConnectFail>iFailConnect) continue;	
		if(iOpenCount - m_aUser[l].dwTime>=3 * TP_HEARTCOUNT)  continue;	
		aUser.Add(m_aUser[l]);
		  
	} 
	LeaveCriticalSection(&m_csTeamCmd); 
}

 CTPNetInterface::tag_TPTeaMsgThread * CTPNetInterface::GetSendThread(TPUserIP & sUserIP)
{
	if(m_uOpenSate & 0x20000000) return NULL;

	CTPNetInterface::tag_TPTeaMsgThread * pThread = NULL;
	EnterCriticalSection(&m_csTeamCmd);
	for(int l=0;l<m_aUser.GetSize();l++)
	{
		if(sUserIP.sIP == m_aUser[l].sIP)
		{
			sUserIP = m_aUser[l];
			break;
		}
	}
	 for(INT l=m_aSendThread.GetSize()-1;l>=0;l--)
	 {
		 if(m_aSendThread[l]->sTP == sUserIP.sIP || m_aSendThread[l]->sTP == _T(""))
		 {
 			 pThread = m_aSendThread[l];
			 pThread ->sTP      = sUserIP.sIP;
			 pThread ->sLocalIP = sUserIP.sLocalIP;
			 pThread ->uPort    = sUserIP.uPort;
			 break;
		 }
	 }
	 if(pThread == NULL)
	 {
		 pThread = new CTPNetInterface::tag_TPTeaMsgThread();
		 pThread ->sTP      = sUserIP.sIP;
		 pThread ->sLocalIP = sUserIP.sLocalIP;
		 pThread ->uPort    = sUserIP.uPort;
		 m_aSendThread.Add(pThread);
	 }
	LeaveCriticalSection(&m_csTeamCmd);
	return pThread;
}
void CTPNetInterface::ReleaseSendThread(tag_TPTeaMsgThread * pThread)
{
	EnterCriticalSection(&pThread->csTeamCmd);
	pThread ->bOpenThread = FALSE;
	pThread->pRunThread   = NULL;
	pThread ->RemoveData();		 
 	LeaveCriticalSection(&pThread->csTeamCmd);
 }
CTPTcpListenSocket * CTPNetInterface::GetAcceptSocket()
{
	EnterCriticalSection(&m_csTeamCmd);	
	CTPTcpListenSocket *pSocket = new CTPTcpListenSocket(this);
	m_aSocket.Add(pSocket);
	LeaveCriticalSection(&m_csTeamCmd);
	return pSocket;
}
void CTPNetInterface::ReleaseAcceptSocket(CTPTcpListenSocket * pSocket)
{
	EnterCriticalSection(&m_csTeamCmd);
	if(pSocket == NULL)
	{
		for(INT l=0;l<m_aSocket.GetSize();l++)
		{
			m_aSocket[l] ->ShutDown(SD_BOTH);
			m_aSocket[l] ->Close();
			delete m_aSocket[l];
		}
		m_aSocket.RemoveAll();
	}
	else
	{	
		for(INT l=0;l<m_aSocket.GetSize();l++)
		{
			if(m_aSocket[l] == pSocket)
			{
				pSocket ->ShutDown(SD_BOTH);
				pSocket ->Close();
				delete pSocket;
				m_aSocket.RemoveAt(l);
				break;;
			}
		}	 
	}
	LeaveCriticalSection(&m_csTeamCmd); 
}
void CTPNetInterface::GetLoginUserIp(TPUserIPArray &aIP,int lLevel,BOOL bCheckType)
{
	DWORD  iOpenCount = GetTickCount();

	aIP.RemoveAll();
	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<m_aUser.GetSize();l++)
	{
		if(bCheckType && m_aUser[l].cAppType != m_cAppType) continue;
		if(m_aUser[l].iConnectFail!=0) continue;
		if(iOpenCount - m_aUser[l].dwTime>=2 * TP_HEARTCOUNT)  continue;	

		aIP.Add(m_aUser[l].GetUserIP());
	}
	for(INT l=0;l<m_aUser.GetSize();l++)
	{
		if(bCheckType && m_aUser[l].cAppType != m_cAppType) continue;
		if(m_aUser[l].iConnectFail>lLevel || m_aUser[l].iConnectFail == 0) continue;
		if(iOpenCount - m_aUser[l].dwTime>=2 * TP_HEARTCOUNT)  continue;	

		aIP.Add(m_aUser[l].GetUserIP());
	}
	LeaveCriticalSection(&m_csTeamCmd);
};


UINT TP_SendMessageThread(LPVOID pVoid)
{
	CTPNetInterface::tag_TPTeaMsgThread *pThread = (CTPNetInterface::tag_TPTeaMsgThread *)pVoid;
	BOOL   bCheck  = FALSE;

	WSADATA		wsaData;
	WORD wVersion	= MAKEWORD(2, 2);
	int  iCheck     = WSAStartup(wVersion, &wsaData);


	ASSERT(NULL == pThread->pSocket);
	while(pThread->bOpenThread)
	{
		   CArray<CTPNetInterface::tag_TPReciveData *,CTPNetInterface::tag_TPReciveData *&> aReciveData;  
			EnterCriticalSection(&pThread ->csTeamCmd);
			aReciveData.Copy(pThread->aReciveData);
			pThread->aReciveData.RemoveAll();
			LeaveCriticalSection(&pThread ->csTeamCmd);

			if(aReciveData.GetSize() <=0) break;

			long lDataSize = 0;
			for(INT l=0;l<aReciveData.GetSize();l++)
				lDataSize = max(lDataSize,aReciveData[l]->lSize);
			long lSendTimeOut = 16000 * (lDataSize/(10*1024)+ 1);
		 
			TPMutex stuLock(pThread->sTP);
			
			if(pThread ->pSocket == NULL)
			{
				pThread ->pSocket = new CTPTeamSendSocke( );
				pThread ->pSocket->m_sLocalIP = pThread ->sLocalIP;
				bCheck	= pThread ->pSocket->Create();
				if (!bCheck)
				{
					CString strLog;
					DWORD dwSocketErr = WSAGetLastError(); 
					strLog.Format(_T("TeamCmdProxy error :Socket Create error, error code %u  \n"),dwSocketErr);
					//SB_LogManager::WriteLog(strLog,0,_T(""),3);
				}
				pThread ->pSocket ->m_lSendTimeOut = lSendTimeOut;
				bCheck	= pThread ->pSocket->Connect((LPCTSTR)pThread->sTP, pThread->uPort); 
				if (!bCheck)
				{
					CString strLog;
					DWORD dwSocketErr = WSAGetLastError(); 
					strLog.Format(_T("TeamCmdProxy error :Socket connect error, error code %u，connect IP %s,connect port %d  \n"),dwSocketErr,pThread->sTP, pThread->uPort);
					//SB_LogManager::WriteLog(strLog,0,_T(""),3);
				}
			}
			else
				bCheck = TRUE;
			
			if(bCheck) 
			{
				pThread ->pProxy ->SetConnectState(pThread->sTP,TRUE);
NestSend:
				for(INT l=0;l<aReciveData.GetSize();l++)
				{
					if(!pThread->bOpenThread) break;
					bCheck = pThread ->pSocket->Send(aReciveData[l] ->pBuffer,aReciveData[l] ->lSize);
					if(bCheck != aReciveData[l] ->lSize)
					{ 
						DWORD dwSocketErr = WSAGetLastError(); 
						CString strLog;
						strLog.Format(_T("TeamCmdProxy error :Data send error, error code %u \n"),dwSocketErr);
						//SB_LogManager::WriteLog(strLog,0,_T(""),3);
						ASSERT(bCheck == aReciveData[l] ->lSize);
					}
				}

				for(INT l=0;l<aReciveData.GetSize();l++) delete aReciveData[l];
				aReciveData.RemoveAll();

				EnterCriticalSection(&pThread ->csTeamCmd);
				aReciveData.Copy(pThread->aReciveData);
				pThread->aReciveData.RemoveAll();
				LeaveCriticalSection(&pThread ->csTeamCmd);
				if(aReciveData.GetSize() >0) goto NestSend ;
			}
			else
			{
				pThread ->pProxy ->SetConnectState(pThread->sTP,FALSE); 
			
			}
 
			ASSERT(NULL != pThread->pSocket);
			pThread ->pSocket ->Close();
			delete  pThread ->pSocket;
			pThread ->pSocket = NULL;


			for(INT l=0;l<aReciveData.GetSize();l++)
				delete aReciveData[l];
			aReciveData.RemoveAll();
	}

	if(NULL != pThread->pSocket)
	{
		pThread ->pSocket ->Close();
		delete  pThread ->pSocket;
		pThread ->pSocket = NULL;
		ASSERT(0);
	}


	WSACleanup();


	pThread ->pProxy->ReleaseSendThread(pThread);
	::SetEvent(pThread->hOpenThread);
	return 1;
}
void  CTPNetInterface::tag_TPTeaMsgThread::SendData(LPBYTE pBuffer,long lSize,BOOL bUseData,int iPos)
{
	EnterCriticalSection(&csTeamCmd);
	CTPNetInterface::tag_TPReciveData *pData = new CTPNetInterface::tag_TPReciveData;
	((tag_TPNetCmdHead *)pBuffer) ->lBufSize = lSize;
	if(bUseData)
	{
		pData ->pBuffer = pBuffer;
		pData ->lSize   = lSize;
	}
	else
	{
		pData ->pBuffer = new BYTE[lSize];
		memcpy_s(pData->pBuffer,lSize,pBuffer,lSize);
		pData ->lSize   = lSize;
	}
	if(iPos>=0) aReciveData.InsertAt(0,pData);
	else        aReciveData.Add(pData);


	if(bOpenThread == FALSE)
	{
		bOpenThread =  TRUE;
		::ResetEvent(hOpenThread);
		pRunThread = AfxBeginThread((AFX_THREADPROC )TP_SendMessageThread,this);	
	} 
	LeaveCriticalSection(&csTeamCmd);
}

void CTPNetInterface::SendMessageData(TPUserIPArray &aIP,CString &sTitle,UINT uDataType,BYTE *bmpByteset,int iSizeset,UINT uWndType)
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

	//if(aIP.GetSize()<=0 ) GetLoginUserIp(aIP);
	if(aIP.GetSize()<=0 ) 	
	{
		ASSERT(0);
		CString strLog;
		strLog.Format(_T("TeamCmdProxy error :Send MessageData error,Empty User array, user message %s  \n"),sTitle);
		//SB_LogManager::WriteLog(strLog,0,_T(""),3);
		return;
	}


	LPBYTE bmpByte = new BYTE[int(iSizeset *1.5)];
	DWORD  iSize   = 0;

	RLE_Encode(bmpByteset, iSizeset, bmpByte,  iSize);

	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_MsgData;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);
	cmdHead.lParam1  = uWndType;

	CString sIP = aIP[0].sLocalIP + _T("\a");
	for(INT l=0;l<aIP.GetSize();l++)
	{
		sIP += aIP[l].sIP;
		sIP += _T("\a");
	}
	struct tag_MsgData
	{
		int  iDataOff;
		int  iDataLen;
		int  iDataSrc;
		int  iIPOff;
		int  iIPLen;
		int  iTitleOff;
		int  iTitleLen;
		UINT uDataType;
	}stuData;

	stuData.uDataType = uDataType;
	stuData.iDataOff  = sizeof(tag_MsgData);
	stuData.iDataLen  = iSize;
	stuData.iDataSrc  = iSizeset;
	stuData.iIPOff    = stuData.iDataOff + stuData.iDataLen; 
	stuData.iIPLen    = (sIP.GetLength() + 1) * sizeof(TCHAR);
	stuData.iTitleOff = stuData.iIPOff + stuData.iIPLen ;
	stuData.iTitleLen = (sTitle.GetLength() + 1) * sizeof(TCHAR);

 
	cmdHead.lDataSize    = stuData.iTitleOff + stuData.iTitleLen ;
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);

	long    lBufferSize = cmdHead.lSendSize  + 10 ;
	LPBYTE  pBuffer     = new BYTE[lBufferSize];
 
	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) ,sizeof(tag_MsgData),&stuData,sizeof(tag_MsgData));
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) +   stuData.iDataOff ,stuData.iDataLen,bmpByte,stuData.iDataLen);
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) +   stuData.iIPOff ,stuData.iIPLen,(LPCTSTR)sIP,stuData.iIPLen);
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) +   stuData.iTitleOff ,stuData.iTitleLen,(LPCTSTR)sTitle,stuData.iTitleLen);

	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		ASSERT(pThread->pSocket == NULL);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,lBufferSize,l == iCount-1);
	}
	aIP.RemoveAll();

	delete [] bmpByte;
	bmpByte = NULL;
} 

void CTPNetInterface::SendMessageString(TPUserIPArray &aIP,CString sMessage,UINT uWndType)
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

 	if(aIP.GetSize()<=0 ) GetLoginUserIp(aIP);
	if(aIP.GetSize()<=0 ) return;

	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_MsgString;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);
	cmdHead.lParam1  = uWndType;

	CString sIP = aIP[0].sLocalIP + _T("\a");
	for(INT l=0;l<aIP.GetSize();l++)
	{
		sIP += aIP[l].sIP;
		sIP += _T("\a");
	}
	sMessage = sIP + sMessage;

	cmdHead.lDataSize    = sMessage.GetLength() * sizeof(TCHAR);
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);

	long    lBufferSize = cmdHead.lSendSize  + 10 ;
	LPBYTE  pBuffer     = new BYTE[lBufferSize];
	ZeroMemory(pBuffer + cmdHead.lSendSize,lBufferSize  - cmdHead.lSendSize  - 2);
	 
	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) ,cmdHead.lDataSize,sMessage,cmdHead.lDataSize);

	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,lBufferSize,l == iCount-1);
	}
	aIP.RemoveAll();

}
void CTPNetInterface::SendUserListCmd(TPUserIPArray &aIP,DWORD uWndType,CStringArray &aIPSend)
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;


	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_SetUserList;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);
	cmdHead.lParam1  = uWndType;

	CString sMessage;

	for(int l=0;l<aIPSend.GetSize();l++)
	{	 
		sMessage += aIPSend[l] + _T("\t") ;
	}
	cmdHead.lDataSize    = sMessage.GetLength() * sizeof(TCHAR);
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);

	long    lBufferSize = cmdHead.lSendSize  + 10 ;
	LPBYTE  pBuffer     = new BYTE[lBufferSize];
	ZeroMemory(pBuffer + cmdHead.lSendSize,lBufferSize  - cmdHead.lSendSize  - 2);

	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) ,cmdHead.lDataSize,sMessage,cmdHead.lDataSize);

	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,lBufferSize,l == iCount-1);
	}
	aIP.RemoveAll();
}
void CTPNetInterface::SetEffOpen(GUID guidUser,GUID guidDBType,GUID guidRes,BOOL bOpen)
{	
	if(guidUser == GUID_NULL)
	{
		if(bOpen)
		{
			m_stuUserState.guidOpenEff.guidRes    = guidRes;
			m_stuUserState.guidOpenEff.guidDBType = guidDBType;
		}
		else
		{
			m_stuUserState.guidOpenEff.guidRes    = GUID_NULL;
			m_stuUserState.guidOpenEff.guidDBType = GUID_NULL;	 
		}
	}
	else
	{
		EnterCriticalSection(&m_csTeamCmd);
		for(INT l=0;l<m_aUser.GetSize();l++)
		{	 
			if(m_aUser[l].guidTeamUser.guidRes == guidUser )
			{
				if(bOpen)
				{
					m_aUser[l].guidOpenEff.guidRes    = guidRes;
					m_aUser[l].guidOpenEff.guidDBType = guidDBType;	
				}
				else
				{
					m_aUser[l].guidOpenEff.guidRes    = GUID_NULL;
					m_aUser[l].guidOpenEff.guidDBType = GUID_NULL;
				}
				break; 			 
			}
		} 
		LeaveCriticalSection(&m_csTeamCmd);
	}
}
void CTPNetInterface::SetRecoreOpen(GUID guidUser,GUID guidDBType,GUID guidRes,BOOL bOpen)
{
	if(guidUser == GUID_NULL)
	{
		if(bOpen)
		{
			m_stuUserState.guidRedord.guidRes    = guidRes;
			m_stuUserState.guidRedord.guidDBType = guidDBType;
		}
		else
		{
			m_stuUserState.guidRedord.guidRes    = GUID_NULL;
			m_stuUserState.guidRedord.guidDBType = GUID_NULL;

		}
	}
	else
	{
		EnterCriticalSection(&m_csTeamCmd);
		for(INT l=0;l<m_aUser.GetSize();l++)
		{	 
			if(m_aUser[l].guidTeamUser.guidRes == guidUser)
			{
				if(bOpen)
				{
					m_aUser[l].guidRedord.guidRes    = guidRes;
					m_aUser[l].guidRedord.guidDBType = guidDBType;
				}
				else
				{
					m_aUser[l].guidRedord.guidRes    = GUID_NULL;
					m_aUser[l].guidRedord.guidDBType = GUID_NULL;
				}
				break; 			 
			}
		} 
		LeaveCriticalSection(&m_csTeamCmd);
	}
}
void CTPNetInterface::SetPrjOpen(GUID guidUser,GUID guidDBType,GUID guidRes,BOOL bOpen)
{ 
	if(guidUser == GUID_NULL)
	{
		if(bOpen)
		{
			m_stuUserState.guidOpenPrj.guidRes    = guidRes;
			m_stuUserState.guidOpenPrj.guidDBType = guidDBType;
		}
		else
		{
			m_stuUserState.guidOpenPrj.guidRes    = GUID_NULL;
			m_stuUserState.guidOpenPrj.guidDBType = GUID_NULL;	
		}
	}
	else
	{
		EnterCriticalSection(&m_csTeamCmd);
		for(INT l=0;l<m_aUser.GetSize();l++)
		{	 
			if(m_aUser[l].guidTeamUser.guidRes == guidUser)
			{
				if(bOpen)
				{
					m_aUser[l].guidOpenPrj.guidRes    = guidRes;
					m_aUser[l].guidOpenPrj.guidDBType = guidDBType;
				}
				else
				{
					m_aUser[l].guidOpenPrj.guidRes    = GUID_NULL;
					m_aUser[l].guidOpenPrj.guidDBType = GUID_NULL;
				}
				break; 			 
			}
		} 
		LeaveCriticalSection(&m_csTeamCmd);
	}
}
void CTPNetInterface::SetProOpen(GUID guidUser,GUID guidDBType,GUID guidRes,BOOL bOpen)
{
	if(guidUser == GUID_NULL)
	{
		if(bOpen)
		{
			m_stuUserState.guidOpenPro.guidRes    = guidRes;
			m_stuUserState.guidOpenPro.guidDBType = guidDBType;
		}
		else
		{
			m_stuUserState.guidOpenPro.guidRes    = GUID_NULL;
			m_stuUserState.guidOpenPro.guidDBType = GUID_NULL;	  
		}
	}
	else
	{
		EnterCriticalSection(&m_csTeamCmd);
		for(INT l=0;l<m_aUser.GetSize();l++)
		{	 
			if(m_aUser[l].guidTeamUser.guidRes == guidUser)
			{		 
				if(bOpen)
				{
					m_aUser[l].guidOpenPro.guidRes    = guidRes;
					m_aUser[l].guidOpenPro.guidDBType = guidDBType;
				}
				else
				{
					m_aUser[l].guidOpenPro.guidRes    = GUID_NULL;
					m_aUser[l].guidOpenPro.guidDBType = GUID_NULL; 
				}
				break; 			 
			}
		} 
		LeaveCriticalSection(&m_csTeamCmd);
	}
}
int  CTPNetInterface::GetPrjOpen(GUID guidUser,GUID guidDBType,GUID guidRes)
{	
	int   iOpenCount = 0;
	DWORD iTickCount = GetTickCount();

	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<m_aUser.GetSize();l++)
	{	 
		if(m_aUser[l].iConnectFail>=2) continue;	
		if(iTickCount - m_aUser[l].dwTime>=2 * TP_HEARTCOUNT)  continue;	
		if(m_aUser[l].guidOpenPrj.guidRes == guidRes && m_aUser[l].guidOpenPrj.guidDBType == guidDBType)
			iOpenCount ++;	 
	} 
	LeaveCriticalSection(&m_csTeamCmd);

	return iOpenCount;
}
int  CTPNetInterface::GetProOpen(GUID guidUser,GUID guidDBType,GUID guidRes)
{
	int   iOpenCount = 0;
	DWORD iTickCount = GetTickCount();

	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<m_aUser.GetSize();l++)
	{	 
		if(m_aUser[l].iConnectFail>=2) continue;	
		if(iTickCount - m_aUser[l].dwTime>=2 * TP_HEARTCOUNT)  continue;	
		if(m_aUser[l].guidOpenPro.guidRes == guidRes && m_aUser[l].guidOpenPro.guidDBType == guidDBType)
			iOpenCount ++;	 
	} 
	LeaveCriticalSection(&m_csTeamCmd);
	return iOpenCount;
}
int  CTPNetInterface::GetEffOpen(GUID guidUser,GUID guidDBType,GUID guidRes)
{	
	int   iOpenCount = 0;
	DWORD iTickCount = GetTickCount();

	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<m_aUser.GetSize();l++)
	{	 
		if(m_aUser[l].iConnectFail>=2) continue;	
		if(iTickCount - m_aUser[l].dwTime>=2 * TP_HEARTCOUNT)  continue;	
		if(m_aUser[l].guidOpenEff.guidRes == guidRes && m_aUser[l].guidOpenEff.guidDBType == guidDBType)
			iOpenCount ++;	 
	} 
	LeaveCriticalSection(&m_csTeamCmd);

	return iOpenCount;
}

BOOL CTPNetInterface::SendAskTeamUser(GUID guidLogin)
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return TRUE;


	//已经纪录登陆GUID
	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<m_aUser.GetSize();l++)
	{	 
		if(m_aUser[l].iConnectFail>=2) continue;	
		if(m_aUser[l].guidTeamUser.guidRes == guidLogin)
		{
			m_sLastError = m_aUser[l].sUser + _T("@") + m_aUser[l].sIP;
			LeaveCriticalSection(&m_csTeamCmd);
			return FALSE;
		}
	} 
	LeaveCriticalSection(&m_csTeamCmd);

	return TRUE;
 /*	TPUserIPArray aIP;
	GetLoginUserIp(aIP,0,TRUE);
	if(aIP.GetSize()<=0 ) return TRUE;

	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_AskLoginUser;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);	

	cmdHead.lDataSize    = sizeof(GUID);
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);
	ASSERT(cmdHead.lSendSize<TP_MAX_SEND);

 
	long   lBufSize    = sizeof(tag_TPNetCmdUseInfo);
	long   lBufferSize = cmdHead.lSendSize  + 10 ;
	LPBYTE pBuffer     = new BYTE[lBufferSize];
	LPBYTE pReceive    = new BYTE[lBufSize] ;
	int    iReceive    = 0;
	BOOL   bContinue   = TRUE;

	cmdHead.lBufSize = lBufferSize;
	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer+sizeof(cmdHead),sizeof(GUID),&guidLogin,sizeof(GUID));


	for(INT l=0;bContinue && l<aIP.GetSize();l++)
	{
		BOOL              bCheck   = FALSE;
		CTPTeamSendSocke *pSocket  = new CTPTeamSendSocke( );
		pSocket ->m_lConnectTimeOut = 1; 
		pSocket ->m_lReciveTimeOut  = 3000; 
		bCheck	= pSocket->Create();	 
		TPMutex stuLock(aIP[l].sIP); 
		bCheck	= pSocket->Connect((LPCTSTR)aIP[l].sIP,aIP[l].uPort); 
		if(bCheck) 
		{
			bCheck   = pSocket->Send(pBuffer,lBufferSize);
			iReceive = pSocket->Receive(pReceive,lBufSize);
			if(iReceive>=lBufSize)
			{
				tag_TPNetCmdUseInfo *guidLogin = (tag_TPNetCmdUseInfo *)pReceive;
			    if(guidLogin ->lParam1 == FALSE)
				{
					m_sLastError = (CString)guidLogin->sUser + _T("@") + guidLogin ->sIP;
					bContinue = FALSE;
				}
			}			  
		} 
		else 
			SetConnectState(aIP[l].sIP, FALSE);

		pSocket->Close(); 
		delete pSocket;			 
	}
	if(pReceive)  delete [] pReceive;
	if(pBuffer)   delete [] pBuffer;
	aIP.RemoveAll();

	return bContinue;*/
}

void CTPNetInterface::SendAskCmd(LPBYTE pBufferSend,long lBufSize,BOOL (*f)(LPBYTE ,long ,LPVOID ),LPVOID pParam)
{
	ASSERT(0);
	//if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

	//if(m_fClipDisplayCmdCallBack == NULL) return ;
	//TPUserIPArray aIP;
	//GetLoginUserIp(aIP,0,TRUE);
	//if(aIP.GetSize()<=0 ) return;

	//tag_TPNetCmdHead cmdHead;
	//cmdHead.eTeamCmd = TPNetCmd_Ask;
	//memcpy_s(cmdHead.cIP,4,m_bIP,4);	

	//cmdHead.lDataSize    = lBufSize;
	//cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);
	//ASSERT(cmdHead.lSendSize<TP_MAX_SEND);

	//long   lBufferSize = cmdHead.lSendSize  + 10 ;
	//LPBYTE pBuffer     = new BYTE[lBufferSize];
	//LPBYTE pReceive    = new BYTE[lBufSize] ;
	//int    iReceive    = 0;
	//BOOL   bContinue   = TRUE;

	//cmdHead.lBufSize = lBufferSize;
	//memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	//memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) ,cmdHead.lDataSize,pBufferSend,cmdHead.lDataSize);


	//for(INT l=0;bContinue && l<aIP.GetSize();l++)
	//{
	//	BOOL              bCheck   = FALSE;
	//	CTPTeamSendSocke *pSocket  = new CTPTeamSendSocke( );
	//	pSocket ->m_lConnectTimeOut = 1; 
	//	pSocket ->m_lReciveTimeOut  = 3000; 
	//	bCheck	= pSocket->Create();	 
	//	TPMutex stuLock(aIP[l].sIP); 
	//	bCheck	= pSocket->Connect((LPCTSTR)aIP[l].sIP,aIP[l].uPort); 
	//	if(bCheck) 
	//	{
	//	//	ZeroMemory(pReceive,lBufSize);
	//		bCheck   = pSocket->Send(pBuffer,lBufferSize);
	//		iReceive = pSocket->Receive(pReceive,lBufSize);
	//		if(iReceive >10 && !f(pReceive,iReceive,pParam)) bContinue = FALSE;	
	//		else if(iReceive <=0) SetConnectState(aIP[l].sIP, FALSE);
 //		} 
	//	else
 //			SetConnectState(aIP[l].sIP, FALSE);

 //		pSocket->Close(); 
	//	delete pSocket;			 
	//}
	//if(pReceive)  delete [] pReceive;
	//if(pBuffer)   delete [] pBuffer;
 //	aIP.RemoveAll();
}
void CTPNetInterface::SendFileCmd(LPBYTE pBufferSend,long lBufSize)
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

	if(m_fFileCmdCallBack == NULL) return ;
	TPUserIPArray aIP;
	GetLoginUserIp(aIP,1,TRUE);
	if(aIP.GetSize()<=0 ) return;


	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_FileChange;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);

	cmdHead.lDataSize    = lBufSize;
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);
	cmdHead.lBufSize     = cmdHead.lSendSize; 
	
	LPBYTE pBuffer     = new BYTE[cmdHead.lBufSize];

	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer+sizeof(tag_TPNetCmdHead),lBufSize,pBufferSend,lBufSize);


	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,cmdHead.lBufSize  ,l == iCount-1,-1);
	}
	aIP.RemoveAll();
}
void CTPNetInterface::SendMsgWndState(TPUserIPArray &aIP,BOOL bOpen,UINT uWndType)
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

	if(aIP.GetSize()<=0) return;
 

	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_WndState;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);
	cmdHead.lParam1  = uWndType;
	cmdHead.lParam2  = bOpen;

	cmdHead.lDataSize    = 0;
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);
	cmdHead.lBufSize     = cmdHead.lSendSize; 

	long   lBufferSize = cmdHead.lSendSize;
	LPBYTE pBuffer     = new BYTE[lBufferSize];

	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));

	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,lBufferSize,l == iCount-1);
	}
	aIP.RemoveAll();
}
void CTPNetInterface::SendBaseCmd(TPNetCmd eCmd,LPBYTE pBufferSend,long lBufSize)
{
	if(/*(m_uOpenSate & 0x10000000) ||*/ m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

	TPUserIPArray aIP;
	GetLoginUserIp(aIP,10,TRUE);
	if(aIP.GetSize()<=0 ) return;


	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = eCmd;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);	

	cmdHead.lDataSize    = lBufSize;
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(tag_TPNetCmdHead);

	ASSERT(cmdHead.lSendSize<TP_MAX_SEND);

	long   lBufferSize = cmdHead.lSendSize  + 10 ;
	LPBYTE pBuffer     = new BYTE[lBufferSize];
 
	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) ,cmdHead.lDataSize,pBufferSend,cmdHead.lDataSize);

	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,lBufferSize,l == iCount-1);
	}
	aIP.RemoveAll();

}
BOOL CTPNetInterface::SendConnect(tag_TPUser &sUser)
{
	tag_TPNetCmdUseInfo cmdData(TPNetCmd_Connect);
	BOOL                 bCheck = FALSE;
	GetTeamCmdUseInfo(&cmdData);

	cmdData.guidTeamUser = m_stuUserState.guidTeamUser;
	cmdData.guidOpenPrj = m_stuUserState.guidOpenPrj; 
	cmdData.guidOpenPro = m_stuUserState.guidOpenPro; 
	cmdData.guidOpenEff = m_stuUserState.guidOpenEff; 
	cmdData.guidRedord = m_stuUserState.guidRedord; 

	GetLocalIP(&cmdData,sUser.sLocalIP);

	CTPTeamSendSocke *pSocket = new CTPTeamSendSocke();
	pSocket ->m_sLocalIP = sUser.sLocalIP;
	bCheck	= pSocket->Create();	 
 	TPMutex stuLock(sUser.sIP);
	pSocket ->m_lConnectTimeOut = 4;
	bCheck	= pSocket->Connect((LPCTSTR)sUser.sIP, sUser.uPort); 
	if(bCheck) 
		bCheck = pSocket->Send(&cmdData,sizeof(tag_TPNetCmdUseInfo));
	else 
	{

	}
	pSocket->Close();
	delete pSocket;	

	return bCheck;
}


void CTPNetInterface::SetReceiveData(LPBYTE pBuffer,long lSize,BOOL bDelete,CTPTcpListenSocket *pSocket,LPCTSTR sLocalIP)
{
	if(m_uOpenSate& 0x00000020)       { if(bDelete) delete [] pBuffer ;return;}
	tag_TPNetCmdHead *pCmdHead = (tag_TPNetCmdHead *)pBuffer;
	if(!pCmdHead  ->CheckData(lSize)) { if(bDelete) delete [] pBuffer ;return;}

	if(pCmdHead  ->eTeamCmd ==  TPNetCmd_Ask && m_fClipDisplayCmdCallBack) //必须立刻回调
	{
		m_fClipDisplayCmdCallBack(pBuffer + sizeof(tag_TPNetCmdHead),pCmdHead  ->lDataSize,m_pTeamCmdParam,pSocket);
		if(bDelete) delete [] pBuffer ;
		return;
	}	
	else  if(pCmdHead ->eTeamCmd == TPNetCmd_TickCount)
	{
		tag_TPNetCmdUseInfo *pData = (tag_TPNetCmdUseInfo *)pBuffer;

		in_addr *pIPAddr = (in_addr*)pData->bIP;
		CString  sIP     = _T("");
		BOOL     bFind   = FALSE;
		if(pIPAddr->S_un.S_un_b.s_b1 != 0)
		{		
			sIP = inet_ntoa(*pIPAddr);		

			EnterCriticalSection(&m_csTeamCmd);
			for(INT l = m_aUser.GetSize()-1;l>=0;l--)
			{					 			 
				if(m_aUser[l].sIP == sIP)		
				{
					m_aUser[l].iConnectFail  = 0;				
					m_aUser[l].uPort         = pData ->uPort;
					m_aUser[l].sTeamUser     = pData ->sTeamUser;
					m_aUser[l].guidTeamUser  = pData ->guidTeamUser;
					m_aUser[l].guidOpenPrj   = pData ->guidOpenPrj;
					m_aUser[l].guidOpenPro   = pData ->guidOpenPro;
					m_aUser[l].guidOpenEff   = pData ->guidOpenEff;
					m_aUser[l].guidRedord    = pData ->guidRedord;
					m_aUser[l].dwTime        = GetTickCount();		
					m_aUser[l].sLocalIP      = sLocalIP;
					bFind                    = TRUE;
					break;
				}					  
			}			
			LeaveCriticalSection(&m_csTeamCmd);
		}
		if(bFind)
		{
			if(bDelete) delete [] pBuffer ;
		}
		else
		{
			pCmdHead ->eTeamCmd = TPNetCmd_Connect;
			SetReceiveData(pBuffer,lSize,bDelete,pSocket,sLocalIP);
		}
	}
	else  if(pCmdHead ->eTeamCmd == TPNetCmd_FileChange)
	{
		if(m_fFileCmdCallBack)
			m_fFileCmdCallBack(pBuffer + sizeof(tag_TPNetCmdHead),pCmdHead->lDataSize);
		if(bDelete) delete [] pBuffer ;
	}
	else
	{
		if(pCmdHead ->eTeamCmd == TPNetCmd_Open && pCmdHead ->lParam1 == 0x1234) //登陆用户要立即返回，否则TPNetCmd_Open 可能被堵住造成相同用户登陆
			SendTickCount();
		EnterCriticalSection(&m_csTeamCmd);
		tag_TPReciveData *pData = new tag_TPReciveData();
		if(bDelete)
			pData ->pBuffer = pBuffer;
		else
		{
			pData ->pBuffer = new BYTE[lSize];
			memcpy_s(pData ->pBuffer,lSize,pBuffer,lSize);
		}
		pData ->sLocalIP = sLocalIP;
		pData ->lSize    = lSize;
		m_aReciveData.Add(pData);
		LeaveCriticalSection(&m_csTeamCmd);

		if(!m_bReciveThread)
		{
			m_bReciveThread = TRUE;
			::ResetEvent(m_hReciveThread);
			AfxBeginThread((AFX_THREADPROC )TP_ReciveThread,this); 
		}
	}
}
void CTPNetInterface::ParserUser(CString &sIP,TPTeamUserArray &aUser,BOOL bThis)
{
	CStringArray aIP;
	ParserIP(sIP,aIP,FALSE);
	EnterCriticalSection(&m_csTeamCmd);
	for(INT l=0;l<aIP.GetSize();l++)
	{
		for(INT k=0;k<m_aUser.GetSize();k++)
		{
			if(m_aUser[k].sIP == aIP[l])
			{
				aUser.Add(m_aUser[k]);
				break;
			}
		}
	}
	LeaveCriticalSection(&m_csTeamCmd);
	if(bThis)
	{
		tag_TPUser stuUser;
		stuUser.sIP  = m_sIP;
		stuUser.uPort = m_uPort;
		m_aUser.Add(stuUser);
	}
}

void CTPNetInterface::ParserIP(CString &sIP,CStringArray &aIP,BOOL bThis)
{
	CString sGet  = sIP;
	CString sFind = _T("");
	while(sGet.GetLength()>0)
	{
		int iFind = sGet.Find('\a');
		if(iFind<0) 
		{
			if(sGet.GetLength()>7) aIP.Add(sGet);
			break;
		}
		sFind = sGet.Left(iFind);
		sGet  = sGet.Mid(iFind+1);
		if(sFind.GetLength()>7) aIP.Add(sFind);
	}
	INT l=0;
	for(l=0;l<aIP.GetSize();l++)
	{
		if(aIP[l] == m_sIP) break;
	}
	if(bThis && l>=aIP.GetSize()) aIP.Add(m_sIP);
	else if(!bThis && l<aIP.GetSize()) aIP.RemoveAt(l);
	ASSERT(aIP.GetSize() > 0);
}

void CTPNetInterface::SendUserLogin(tag_TPUser *pData,BOOL bLogin)
{
	if((m_uOpenSate & 0x10000000) || m_hLoginWnd == NULL) return;
	::SendMessage(m_hLoginWnd,WM_TEAMUSER,bLogin?TPTeamMsg_Login:TPTeamMsg_Logoff,(LPARAM)pData);
}
void CTPNetInterface::GetUserInfo()
{
	//UserName

	TCHAR cName[MAX_PATH];
	ULONG uLength = MAX_PATH;	
	ZeroMemory(cName,sizeof(TCHAR) * MAX_PATH);
	::GetUserName(cName,&uLength);	
	m_sUser      = cName;

	if (m_stuUserState.sTeamUser.GetLength() <= 0)//if null set teamname
	{
		m_stuUserState.sTeamUser  = cName;
	}	

	m_uOpenSate |= 0x00000001;
	//IP
	int			nCheck			= 0;
	WORD		wVersion		= 0;
	CString		strName			= _T("");
	CString		strIP			= _T("");
	in_addr		*pIPAddr		= NULL;
	HOSTENT		*pstHostInfo	= NULL;
	char		chrName[MAX_PATH];
//	WSADATA		wsaData;
	char		**pIPAddress;

//	wVersion	= MAKEWORD(2, 2);
	//nCheck		= WSAStartup(wVersion, &wsaData);
	nCheck		|= gethostname(chrName, MAX_PATH);
	if(nCheck != 0)
	{
		//WSACleanup();
		return  ;
	}

	strName		= (CString)chrName;

	m_sComputer = strName;

#ifdef	_UNICODE
	USES_CONVERSION;
	pstHostInfo	= gethostbyname(W2A(strName));
#else
	pstHostInfo	= gethostbyname(strName.GetBuffer(strName.GetLength()));
#endif

	if(pstHostInfo != NULL)
	{
		for(pIPAddress = pstHostInfo->h_addr_list; *pIPAddress != NULL; pIPAddress++)
		{			 
			memcpy_s(m_bIP,4, *pIPAddress, 4);
			pIPAddr		= (in_addr*)m_bIP;
			if(pIPAddr->S_un.S_un_b.s_b1 == 0) continue;
			strIP		= inet_ntoa(*pIPAddr); 			 
			m_sIP       = strIP;
			m_aThisIP.Add(strIP);
		}
	}
	else
	{
		//WSACleanup();
		return  ;
	}
	//WSACleanup(); 

	m_uOpenSate |= 0x00000002; 

	return  ;
}

//利用定时器3分钟调用一次该函数，检查在线状态，连接失败iconnectfail加一，大于ilevel后认为该用户不正常断线
void	CTPNetInterface::CheckUserState()
{	 
	 TPTeamUserArray aUser, aUserOffLine, aUserOnline;
	 EnterCriticalSection(&m_csTeamCmd); 
	 aUser.Copy(m_aUser);
	 LeaveCriticalSection(&m_csTeamCmd);


	 //get the online and offline users
	 int l = 0, i = 0;
	 for (l=0;l<aUser.GetSize();l++)
	 {
		 CTPTeamSendSocke *pSocket = new CTPTeamSendSocke();
		 pSocket ->m_sLocalIP = aUser[l].sLocalIP;
		 BOOL              bCheck  = pSocket->Create();	 
		 TPMutex stuLock(aUser[l].sIP);
		 pSocket ->m_lConnectTimeOut = 4;
		 bCheck	= pSocket->Connect((LPCTSTR)aUser[l].sIP, aUser[l].uPort); 		  
		 pSocket->Close();
		 delete pSocket;	
		 if(!bCheck) 
		 {
			 aUserOffLine.Add(aUser.GetAt(l));
		 }
		 else
		 {
			 aUserOnline.Add(aUser.GetAt(l));
		 }
	 }
	 

	 //update the state in m_aUser
	 EnterCriticalSection(&m_csTeamCmd);
	 for (i=0;i<aUserOffLine.GetSize();i++)
	 {
		 for (l=0;l<m_aUser.GetSize();l++)
		 {
			 if (aUserOffLine.GetAt(i).sIP.CompareNoCase(m_aUser.GetAt(l).sIP) == 0)
			 {
				 m_aUser.GetAt(l).iConnectFail ++ ;
				 if (m_aUser.GetAt(l).iConnectFail >10)
				 {
					 m_aUser.RemoveAt(l);
				 }
				 break;
			 }
		 }
	 }

	 for (i=0;i<aUserOnline.GetSize();i++)
	 {
		 for (l=0;l<m_aUser.GetSize();l++)
		 {
			 if (aUserOnline.GetAt(i).sIP.CompareNoCase(m_aUser.GetAt(l).sIP) == 0)
			 {
				 m_aUser.GetAt(l).iConnectFail = 0;
				 break;
			 }
		 }
	 }
	 LeaveCriticalSection(&m_csTeamCmd);
}
void	CTPNetInterface::SendAskVoiceChat(TPUserIPArray &aIP, UINT uWndType)//请求语音聊天
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

	//if(aIP.GetSize()<=0 ) GetLoginUserIp(aIP);
	if(aIP.GetSize()<=0 ) return;
	if(aIP.GetSize()>1)	  return;//暂时不放开多人语聊
	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_AskVoiceChat;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);
	cmdHead.lParam1  = uWndType;

	CString sIP = m_sIP + _T("\a");
	for(INT l=0;l<aIP.GetSize();l++)
	{
		sIP += aIP[l].sIP;
		sIP += _T("\a");
	}
	CString sMessage =  sIP ;

	cmdHead.lDataSize    = sMessage.GetLength() * sizeof(TCHAR);
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);

	long    lBufferSize = cmdHead.lSendSize  + 10 ;
	LPBYTE  pBuffer     = new BYTE[lBufferSize];
	ZeroMemory(pBuffer + cmdHead.lSendSize,lBufferSize  - cmdHead.lSendSize  - 2);

	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) ,cmdHead.lDataSize,sMessage,cmdHead.lDataSize);

	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,lBufferSize,l == iCount-1);
	}
	aIP.RemoveAll();
}

void	CTPNetInterface::SendBeginVoiceChat(TPUserIPArray &aIP, UINT uWndType)//开始语音聊天
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

	//if(aIP.GetSize()<=0 ) GetLoginUserIp(aIP);
	if(aIP.GetSize()<=0 ) return;
	if(aIP.GetSize()>1)	  return;//暂时不放开多人语聊
	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_BeginVoiceChat;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);
	cmdHead.lParam1  = uWndType;

	CString sIP = m_sIP + _T("\a");
	for(INT l=0;l<aIP.GetSize();l++)
	{
		sIP += aIP[l].sIP;
		sIP += _T("\a");
	}

	CString sMessage =  sIP ;

	cmdHead.lDataSize    = sMessage.GetLength() * sizeof(TCHAR);
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);

	long    lBufferSize = cmdHead.lSendSize  + 10 ;
	LPBYTE  pBuffer     = new BYTE[lBufferSize];
	ZeroMemory(pBuffer + cmdHead.lSendSize,lBufferSize  - cmdHead.lSendSize  - 2);

	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) ,cmdHead.lDataSize,sMessage,cmdHead.lDataSize);

	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,lBufferSize,l == iCount-1);
	}
	aIP.RemoveAll();
}
BOOL    CTPNetInterface::IsConnect()
{
	DWORD dwReturn = 0;
	BOOL  bLink    = InternetGetConnectedState(&dwReturn,0); 
	if(bLink == FALSE &&!(dwReturn & (INTERNET_CONNECTION_LAN |INTERNET_CONNECTION_PROXY)))
	{
		return FALSE;
	}
	else
		return TRUE;
}
void    CTPNetInterface::SendCloseVoiceChat(TPUserIPArray &aIP, UINT uWndType)//开始语音聊天
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

	//if(aIP.GetSize()<=0 ) GetLoginUserIp(aIP);
	if(aIP.GetSize()<=0 ) return;
	if(aIP.GetSize()>1)	  return;//暂时不放开多人语聊
	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_CloseVoiceChat;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);
	cmdHead.lParam1  = uWndType;

	CString sIP = m_sIP + _T("\a");
	for(INT l=0;l<aIP.GetSize();l++)
	{
		sIP += aIP[l].sIP;
		sIP += _T("\a");
	}

	CString sMessage =  sIP ;

	cmdHead.lDataSize    = sMessage.GetLength() * sizeof(TCHAR);
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);

	long    lBufferSize = cmdHead.lSendSize  + 10 ;
	LPBYTE  pBuffer     = new BYTE[lBufferSize];
	ZeroMemory(pBuffer + cmdHead.lSendSize,lBufferSize  - cmdHead.lSendSize  - 2);

	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) ,cmdHead.lDataSize,sMessage,cmdHead.lDataSize);

	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,lBufferSize,l == iCount-1);
	}
	aIP.RemoveAll();
}

void    CTPNetInterface::SendDisWnd(TPUserIPArray &aIP, UINT uWndType)//禁止输入，被踢出群聊
{
	if((m_uOpenSate & 0x10000000) || m_uOpenSate == 0 || (m_uOpenSate & 0x20000000)) return;

	//if(aIP.GetSize()<=0 ) GetLoginUserIp(aIP);
	if(aIP.GetSize()<=0 ) return;
	tag_TPNetCmdHead cmdHead;
	cmdHead.eTeamCmd = TPNetCmd_DisChatWnd;
	memcpy_s(cmdHead.cIP,4,m_bIP,4);
	cmdHead.lParam1  = uWndType;

	CString sIP = m_sIP + _T("\a");
	for(INT l=0;l<aIP.GetSize();l++)
	{
		sIP += aIP[l].sIP;
		sIP += _T("\a");
	}

	CString sMessage =  sIP ;

	cmdHead.lDataSize    = sMessage.GetLength() * sizeof(TCHAR);
	cmdHead.lSendSize    = cmdHead.lDataSize + sizeof(cmdHead);

	long    lBufferSize = cmdHead.lSendSize  + 10 ;
	LPBYTE  pBuffer     = new BYTE[lBufferSize];
	ZeroMemory(pBuffer + cmdHead.lSendSize,lBufferSize  - cmdHead.lSendSize  - 2);

	memcpy_s(pBuffer,sizeof(tag_TPNetCmdHead),&cmdHead,sizeof(tag_TPNetCmdHead));
	memcpy_s(pBuffer +sizeof(tag_TPNetCmdHead) ,cmdHead.lDataSize,sMessage,cmdHead.lDataSize);

	int iCount = aIP.GetSize();
	for(INT l=0;l<aIP.GetSize();l++)
	{
		CTPNetInterface::tag_TPTeaMsgThread *pThread = GetSendThread(aIP[l]);
		ASSERT(pThread);
		pThread->pProxy      = this;
		pThread ->SendData(pBuffer,lBufferSize,l == iCount-1);
	}
	aIP.RemoveAll();
}
