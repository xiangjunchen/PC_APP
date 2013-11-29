#include "StdAfx.h"

extern void 		TP_GetSocketError();
/////////////////////////////////////////////////////////// CTPTcpListenSocket
CTPTcpListenSocket::CTPTcpListenSocket(CTPNetInterface   *pTemCmdProxy) 
{	
	m_pTemCmdProxy    = pTemCmdProxy;
	m_pRecive         = NULL;
	m_lRecive         = 0;
	m_lBuffer         = 16*1024;
	m_lBaseLen        = sizeof(CTPNetInterface::tag_TPNetCmdHead);
	m_lTickCount      = 0;
	m_lTickReceive    = ::GetTickCount();
	m_lConnectTimeOut = 3;
	m_lSendTimeOut    = 6000;
	m_lReciveTimeOut  = 6000;
	m_lAcceptSockAddrLen = sizeof(sockaddr_in);
	ZeroMemory(&m_stuAcceptSockAddr,m_lAcceptSockAddrLen); 
	ZeroMemory(&m_stuLocalSockAddr,m_lAcceptSockAddrLen); 	
}
CTPTcpListenSocket::~CTPTcpListenSocket()
{
	if(m_pRecive) delete m_pRecive;
	m_pRecive = NULL;
}
void CTPTcpListenSocket::OnConnect(int nErrorCode)
{
	CAsyncSocket::OnConnect(nErrorCode);
}
void CTPTcpListenSocket::OnClose(int nErrorCode)
{
	CAsyncSocket::OnClose(nErrorCode);
	m_pTemCmdProxy ->ReleaseAcceptSocket(this);

}
CString  CTPTcpListenSocket::GetIP(sockaddr_in &stuIP)
{
	CString sIP;
	sIP.Format(_T("%d.%d.%d.%d"),stuIP.sin_addr.S_un.S_un_b.s_b1,stuIP.sin_addr.S_un.S_un_b.s_b2,stuIP.sin_addr.S_un.S_un_b.s_b3,stuIP.sin_addr.S_un.S_un_b.s_b4);
	return sIP;
}
void CTPTcpListenSocket::OnAccept(int nErrorCode)
{
	CTPTcpListenSocket *pAcceptSocket = m_pTemCmdProxy ->GetAcceptSocket();
	if(CAsyncSocket::Accept(*pAcceptSocket,(SOCKADDR *) &pAcceptSocket->m_stuAcceptSockAddr,&pAcceptSocket ->m_lAcceptSockAddrLen))
	{
		::getsockname(pAcceptSocket->m_hSocket,(SOCKADDR *) &pAcceptSocket->m_stuLocalSockAddr,&pAcceptSocket ->m_lAcceptSockAddrLen);//获取客户的目标ip地址
		pAcceptSocket->m_sLocalIP =   GetIP(pAcceptSocket->m_stuLocalSockAddr); 	
	}
	CAsyncSocket::OnAccept(nErrorCode); 
} 
void CTPTcpListenSocket::OnReceive(int nErrorCode)
{	 
	m_lTickReceive    = ::GetTickCount();
	if(m_pRecive == NULL) m_pRecive = new BYTE[m_lBuffer];
	int iRecive       =  CAsyncSocket::Receive(m_pRecive + m_lRecive, m_lBuffer-m_lRecive);
	if(iRecive >0)
	{
		m_lRecive	+= iRecive; 
		if(m_lRecive>=m_lBaseLen)//同一IP可能发送连续命令
		{
			TPNetCmdHead *pTeamCmd = (TPNetCmdHead *)m_pRecive; 
			if(pTeamCmd ->dwVersion != TP_CMDVER)     
			{
				ASSERT(0);
				CString strLog;
				strLog.Format(_T("TeamCmdProxy error :Data received error version \n"));
				//SB_LogManager::WriteLog(strLog,0,_T(""),3);
				return  ;
			}

			if(pTeamCmd->lBufSize>m_lBuffer) //接收长度有问题 ，需要重新new Buffer
			{
				m_lBuffer = pTeamCmd->lBufSize + 1000;
				LPBYTE pBuffer = new BYTE[m_lBuffer];
				memcpy_s(pBuffer,m_lRecive,m_pRecive,m_lRecive);
				delete [] m_pRecive;
				m_pRecive = pBuffer;

				pTeamCmd = (TPNetCmdHead *)m_pRecive;
			}
			if(pTeamCmd->lBufSize == m_lRecive)//长度正好相等
			{
				m_pTemCmdProxy ->SetReceiveData(m_pRecive,m_lRecive,TRUE,this,m_sLocalIP);
				m_pRecive   = NULL;
				m_lRecive   = 0;	
			}
			else if(pTeamCmd->lBufSize < m_lRecive) //数据有多余
			{
				long lSend = 0;
				while(m_lRecive - lSend >= m_lBaseLen && m_lRecive - lSend >= pTeamCmd->lBufSize)
				{
					if(pTeamCmd ->dwVersion != TP_CMDVER)
					{
						ASSERT(0);
						CString strLog;
						strLog.Format(_T("TeamCmdProxy error :Data received error version \n"));
						//SB_LogManager::WriteLog(strLog,0,_T(""),3);
						return;
					}
					m_pTemCmdProxy ->SetReceiveData(m_pRecive + lSend,pTeamCmd->lBufSize,FALSE,this,m_sLocalIP);
					lSend += pTeamCmd->lBufSize;
					pTeamCmd = (TPNetCmdHead *)(m_pRecive + lSend);
				}
				if(m_lRecive - lSend == 0)
					m_lRecive = 0;
				else if(m_lRecive - lSend>0 && lSend >0)
				{
					memcpy_s(m_pRecive,m_lRecive - lSend,m_pRecive + lSend,m_lRecive - lSend);
					m_lRecive -= lSend;
				}				
			}
			else //继续等待
			{
			}
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
bool CTPTcpListenSocket::IsClose()
{
	if(m_hSocket == INVALID_SOCKET)
		return true;

	bool     bReturn = false;
	fd_set   fdsetWrite;   
	FD_ZERO(&fdsetWrite);   
	FD_SET(m_hSocket,&fdsetWrite); 

	struct   timeval   tv;   
	tv.tv_sec=0;     tv.tv_usec=0;   
	if(select(0,NULL,&fdsetWrite,NULL,&tv)==SOCKET_ERROR)   
	{   
		bReturn = true; 
	} 
	else
	{
		if(FD_ISSET(m_hSocket,&fdsetWrite))
			bReturn = false;
		else
			bReturn = true;
	}

	FD_ZERO(&fdsetWrite);

	return bReturn;
}
void CTPTcpListenSocket::GetTickCount(BOOL bStart)
{
	if(bStart) m_lTickCount = ::GetTickCount();
	else m_lTickCount       = ::GetTickCount( ) - m_lTickCount;
}
BOOL CTPTcpListenSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	unsigned long ul = 1;
	if(::ioctlsocket(m_hSocket,FIONBIO, &ul)==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		return FALSE;
	} 

	TIMEVAL timeval;
	fd_set  r;                      

	FD_ZERO(&r);
	FD_SET(m_hSocket, &r);
	timeval.tv_sec  = m_lConnectTimeOut;   //3秒
	timeval.tv_usec = 0;   //0毫秒

	int err = CAsyncSocket::Connect(lpszHostAddress,nHostPort);

	if(err == 1)	
	{
	}
	else if (err == WSAEWOULDBLOCK || err == 0) 
	{
		err = ::select(0, 0, &r, 0, &timeval);  
		if ( err <= 0 )
			return FALSE;
	}
	else if (err <0) 
		return FALSE;

	AsyncSelect(0);

	ul = 0;
	if(::ioctlsocket(m_hSocket,FIONBIO, &ul)==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		return FALSE;
	} 

	int   TimeOut = m_lSendTimeOut;   //设置发送超时6秒   
	if(::setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char   *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{   
		return   0;   
	}   
	TimeOut      = m_lReciveTimeOut;//设置接收超时6秒   
	if(::setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char   *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{   
		DWORD dwError = WSAGetLastError();
		return   FALSE;   
	}   

	return TRUE;
} 
/////////////////////////////////////////////////////////// CTPTcpListenSocket


/////////////////////////////////////////////////////////// CTPTcpSendSocket

CTPTcpSendSocket::CTPTcpSendSocket() 
{		 
	m_lConnectTimeOut = 3;
	m_lSendTimeOut    = 6000;
	m_lReciveTimeOut  = 6000;
	m_hSocket         = 0;
}
CTPTcpSendSocket::~CTPTcpSendSocket() 
{		 
	m_lConnectTimeOut = 0;
	m_lSendTimeOut    = 0;
	m_lReciveTimeOut  = 0;
	m_hSocket         = 0;
}

bool CTPTcpSendSocket::Connect(const unsigned short * lpszHostAddress, unsigned int nHostPort)
{
	USES_CONVERSION_EX;//用于编码转换如W2A,A2W使用的一些局步变量
	int    ret      = 0 ;   
	LPSTR  lpszIP   = T2A_EX((LPTSTR)lpszHostAddress, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
	LPSTR  lpLocaIP = T2A_EX((LPTSTR)(LPCTSTR)m_sLocalIP, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);

	//socket
	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ASSERT(m_hSocket != INVALID_SOCKET);

	//bind
	SOCKADDR_IN   saLocal;        	//本地地址信息
	memset(&saLocal, 0, sizeof(saLocal));
	saLocal.sin_family		= AF_INET;
	saLocal.sin_addr.s_addr = inet_addr(lpLocaIP);// htonl(INADDR_ANY);客户端可以绑定一个
	saLocal.sin_port		= htons(0);//如果参数1为0，则WinSock分配一个端口给它，范围在1024和5000之间
	if(bind(m_hSocket,(sockaddr *)&saLocal,sizeof(sockaddr_in))<0)
	{	
		TP_GetSocketError();
		DWORD dwReturn = WSAGetLastError();		
		BOOL bLink = InternetGetConnectedState(&dwReturn,0); //返回本地系统的网络连接状态
		if(bLink == FALSE &&!(dwReturn & (INTERNET_CONNECTION_LAN |INTERNET_CONNECTION_PROXY)))
		{
		}
		//return FALSE;
	} 

	//设置重用
	BOOL bReuseaddr=TRUE;
	setsockopt(m_hSocket,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(BOOL));

	int   TimeOut=m_lSendTimeOut;   //设置发送超时6秒   
	if(::setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char   *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{   
		return   0;   
	}   
	TimeOut=m_lReciveTimeOut;//设置接收超时6秒   
	if(::setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char   *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{   
		return   0;   
	}   
	//设置非阻塞方式连接   
	unsigned   long   ul   =   1;   
	ret   =   ioctlsocket(m_hSocket,   FIONBIO,   (unsigned   long*)&ul);   
	if(ret==SOCKET_ERROR)
	{
		TP_GetSocketError();
		return   0;   
	}

	//connect
	SOCKADDR_IN   saRemote;        	//远程地址信息
	memset(&saRemote, 0, sizeof(saRemote));
	saRemote.sin_family		 = AF_INET;
	saRemote.sin_addr.s_addr = inet_addr(lpszIP);
	saRemote.sin_port		 = htons(nHostPort);
	ret = connect( m_hSocket, (sockaddr*)&saRemote, sizeof(sockaddr));// 每当函数connect失败，都必须关闭套接口描述字，重新调用socket

	//select   模型，即设置超时   
	struct   timeval   timeout   ;   
	fd_set   r;   

	FD_ZERO(&r);   
	FD_SET(m_hSocket,   &r);   
	timeout.tv_sec   =   m_lConnectTimeOut;   //连接超时15秒   
	timeout.tv_usec   =0;   
	ret   =   select(0,   0,   &r,   0,   &timeout);   
	if   (   ret   <=   0   )   
	{ 
		TP_GetSocketError();
		return   0;   
	}   
	//一般非锁定模式套接比较难控制，可以根据实际情况考虑   再设回阻塞模式   
	unsigned   long   ul1=   0   ;   
	ret   =   ioctlsocket(m_hSocket,   FIONBIO,   (unsigned   long*)&ul1);   
	if(ret==SOCKET_ERROR)
	{   
		TP_GetSocketError();
		return   0;   
	}

	linger   sLinger; 
	sLinger.l_onoff   = 1;   //   (在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)  
	sLinger. l_linger = 3;
	ret = setsockopt(m_hSocket, SOL_SOCKET, SO_LINGER,(const   char*)&sLinger, 	sizeof(linger));
	if(ret <= 0)
	{
		TP_GetSocketError();
	}

	return TRUE;
} 
bool CTPTcpSendSocket::Create()
{ 
	//	m_hSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);   
	//	ASSERT(m_hSocket != INVALID_SOCKET);
	return TRUE;
}
void CTPTcpSendSocket::Close()
{
	if(m_hSocket     != INVALID_SOCKET)
	{
		shutdown(m_hSocket, SD_BOTH);
		closesocket(m_hSocket);
	}
	m_hSocket = INVALID_SOCKET;
}
int CTPTcpSendSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	return send(m_hSocket,(char *)lpBuf,nBufLen,nFlags);

}
int CTPTcpSendSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
{
	return recv(m_hSocket,(char *)lpBuf,nBufLen,nFlags);
}
bool  CTPTcpSendSocket::IsClose()
{
	if(m_hSocket == INVALID_SOCKET)
		return true;

	bool     bReturn = false;
	fd_set   fdsetWrite;   
	FD_ZERO(&fdsetWrite);   
	FD_SET(m_hSocket,&fdsetWrite); 

	struct   timeval   tv;   
	tv.tv_sec=0;     tv.tv_usec=0;   
	if(select(0,NULL,&fdsetWrite,NULL,&tv)==SOCKET_ERROR)   
	{   
		bReturn = true; 
	} 
	else
	{
		if(FD_ISSET(m_hSocket,&fdsetWrite))
			bReturn = false;
		else
			bReturn = true;
	}

	FD_ZERO(&fdsetWrite);
	return bReturn;
}
/////////////////////////////////////////////////////////// CTPTcpSendSocket


//暂未使用
CTPTcpSocketEx::CTPTcpSocketEx() 
{		 
	m_lConnectTimeOut = 3;
	m_lSendTimeOut    = 6000;
	m_lReciveTimeOut  = 6000;
}
BOOL CTPTcpSocketEx::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	unsigned long ul = 1;
	if(::ioctlsocket(m_hSocket,FIONBIO, &ul)==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		return FALSE;
	} 

	TIMEVAL timeval;
	fd_set  r;                      

	FD_ZERO(&r);
	FD_SET(m_hSocket, &r);
	timeval.tv_sec  = m_lConnectTimeOut;   //3秒
	timeval.tv_usec = 0;   //0毫秒

	int err = CAsyncSocket::Connect(lpszHostAddress,nHostPort);

	if(err == 1)	
	{
	}
	else if (err == WSAEWOULDBLOCK || err == 0) 
	{
		err = ::select(0, 0, &r, 0, &timeval);  
		if ( err <= 0 )
			return FALSE;
	}
	else if (err <0) 
		return FALSE;

	AsyncSelect(0);

	ul = 0;
	if(::ioctlsocket(m_hSocket,FIONBIO, &ul)==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		return FALSE;
	} 

	int   TimeOut = m_lSendTimeOut;   //设置发送超时6秒   
	if(::setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char   *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{   
		return   0;   
	}   
	TimeOut      = m_lReciveTimeOut;//设置接收超时6秒   
	if(::setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char   *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{   
		DWORD dwError = WSAGetLastError();
		return   FALSE;   
	}   

	return TRUE;
} 

