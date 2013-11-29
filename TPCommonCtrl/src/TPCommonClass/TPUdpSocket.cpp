// MulticastSocket.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void TP_GetSocketError()
{
	INT nErrorCode = 0;
	CString sError = _T("");
	nErrorCode = WSAGetLastError();
	if(nErrorCode != 0)
	{
		sError = TP_GetSysErrMsg(nErrorCode);
		ASSERT(0);
	}
}

//CTPUdpSocket
CTPUdpSocket::CTPUdpSocket()
{
	m_lBaseLen      = sizeof(CTPNetInterface::tag_TPNetCmdHead);
	m_nLocalPort         = 0;
	m_fMulticastCallBack = NULL;
	m_pMulticastParam    = NULL;
	m_uInitState         = 0;
}

CTPUdpSocket::~CTPUdpSocket()
{
}
BOOL  CTPUdpSocket::CreateSendSocket()
{
	return TRUE;
}
BOOL  CTPUdpSocket::CreateListenSocket(UINT uPort)
{
	return TRUE;
}
BOOL  CTPUdpSocket::CreateListenSocket(LPCSTR lpIP, UINT uPort)
{
	return TRUE;
}

BOOL CTPUdpSocket::CloseListenSocket()
{
	Close();	
	m_uInitState &= ~TP_CREATELISTEN ;	
	return TRUE;
}
BOOL CTPUdpSocket::SendSocket(const void* strMessage, int nSize,LPCTSTR sIP)
{
	if(m_uInitState & TP_CREATESEND)	return TRUE;
	return FALSE;
}
void CTPUdpSocket::OnReceive(int nErrorCode)
{
	int iLen = ReceiveFrom ( m_strBuffer, TP_RECEIVE, m_strReciveIP, m_nRecivePort);

	if( iLen == SOCKET_ERROR)
	{
		TP_GetSocketError();
	}
	else if(iLen>m_lBaseLen)
	{
		BOOL bThisIP = FALSE;
		if(m_strReciveIP == m_strLocalIP) 
			bThisIP = TRUE;
		else
		{
			for(INT l=0;l<m_aThisIP.GetSize();l++)
			{
				if(m_aThisIP[l] == m_strReciveIP)
				{
					bThisIP = TRUE;
					break;
				}
			}
		}

		if( !bThisIP && m_fMulticastCallBack != NULL) //call back
		{
			m_fMulticastCallBack((LPBYTE)m_strBuffer, iLen,m_pMulticastParam,m_strLocalIP);
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

BOOL CTPUdpSocket::GetMultiSendAddress(LPCSTR strGroupIP, UINT nGroupPort)
{
	/* Fill m_saHostGroup_in for sending datagrams */
	memset(&m_saHostGroup, 0, sizeof(m_saHostGroup));
	m_saHostGroup.sin_family = AF_INET;
	m_saHostGroup.sin_addr.s_addr = inet_addr(strGroupIP);
	m_saHostGroup.sin_port = htons((USHORT)nGroupPort);

	/* Join the multicast group */
	m_mrMReq.imr_multiaddr.s_addr = inet_addr(strGroupIP);	/* group addr */ 
	m_mrMReq.imr_interface.s_addr = htons(INADDR_ANY);		/* use default */ 

	return TRUE;
}
BOOL CTPUdpSocket::CloseSendSocket()
{
	m_SendSocket.Close();
	m_uInitState &=~TP_CREATESEND;
	return TRUE;
}




//CTPMultiUdpSocket
BOOL CTPMultiUdpSocket::CloseListenSocket()
{
	if(setsockopt (this->m_hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char FAR *)&m_mrMReq, sizeof(m_mrMReq)) < 0)
		return FALSE;

	return CTPUdpSocket::CloseListenSocket();
}
BOOL CTPMultiUdpSocket::CreateListenSocket(LPCSTR lpIP, UINT uPort)
{
	/* Create socket for receiving packets from multicast group */
	if(!Create(uPort, SOCK_DGRAM, FD_READ,m_strLocalIP))
	{
		m_uInitState &= ~TP_CREATELISTEN;
		return FALSE;
	}

	INT nFlag = 1; 
	if(setsockopt(this->m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char FAR *)&nFlag,sizeof(nFlag)) < 0) 
	{
		TP_GetSocketError();
		//return FALSE;
	}

	/* Fill m_saHostGroup_in for sending datagrams */
	memset(&m_saHostGroup, 0, sizeof(m_saHostGroup));
	m_saHostGroup.sin_family		= AF_INET;
	m_saHostGroup.sin_addr.s_addr   = inet_addr(lpIP);
	m_saHostGroup.sin_port			= htons((USHORT)uPort);

	/* Join the multicast group */
	USES_CONVERSION_EX;
	LPSTR lpszAscii = T2A_EX((LPTSTR)(LPCTSTR)m_strLocalIP, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
	m_mrMReq.imr_multiaddr.s_addr = inet_addr(lpIP);	/* group addr */ 
	//m_mrMReq.imr_interface.s_addr = htons(INADDR_ANY);		/* use default */ 
	m_mrMReq.imr_interface.s_addr = inet_addr(lpszAscii);		/* use default */ 

	if(setsockopt(this->m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR *)&m_mrMReq, sizeof(m_mrMReq)) < 0)
	{		 
		TP_GetSocketError();
		//return FALSE;
	} 

	INT  nBufLen = TP_RECEIVE;
	if(setsockopt(this->m_hSocket, SOL_SOCKET, SO_RCVBUF, (char FAR *)&nBufLen,sizeof(nBufLen)) < 0)
	{
		TP_GetSocketError();
		//return FALSE;
	}

	m_uInitState |=  TP_CREATELISTEN;	
	m_nLocalPort = uPort ;
	return TRUE;

}
BOOL CTPMultiUdpSocket::SendSocket(const void* strMessage, int nSize,LPCTSTR sIP)
{
	if(!CTPUdpSocket::SendSocket(strMessage,nSize,sIP))		return FALSE;

	if(sIP != NULL)
	{
		USES_CONVERSION_EX;
		LPSTR lpszAscii =  T2A_EX((LPCTSTR)sIP, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
		unsigned long addr = inet_addr(lpszAscii); 
		setsockopt(m_SendSocket.m_hSocket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr)); 
		if(m_SendSocket.SendTo(strMessage, nSize, (SOCKADDR*)&m_saHostGroup, sizeof(SOCKADDR), 0) == SOCKET_ERROR)
			return FALSE;			

	}
	else if(m_aThisIP.GetSize()<=1)
	{		
		if(m_SendSocket.SendTo(strMessage, nSize, (SOCKADDR*)&m_saHostGroup, sizeof(SOCKADDR), 0) == SOCKET_ERROR)
			return FALSE;			
	}
	else
	{
		USES_CONVERSION_EX;
		LPSTR lpszAscii = NULL;
		for(int l=0;l<m_aThisIP.GetSize();l++)
		{			
			lpszAscii = T2A_EX((LPCTSTR)m_aThisIP[l], _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
			unsigned long addr = inet_addr(lpszAscii); 
			setsockopt(m_SendSocket.m_hSocket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr)); 
			if(m_SendSocket.SendTo(strMessage, nSize, (SOCKADDR*)&m_saHostGroup, sizeof(SOCKADDR), 0) == SOCKET_ERROR)
				return FALSE;			
		}
	}
	return TRUE;
}

BOOL CTPMultiUdpSocket::CreateSendSocket()
{
	GetMultiSendAddress(TP_MULTICAST_ADRESS,TP_CLIPSERVER_PORT);

	if(!m_SendSocket.Create(0, SOCK_DGRAM, 0))		// Create an unconnected UDP socket  ,SOCK_DGRAM数据报式Socket是一种无连接的Socket，对应于无连接的UDP服务应用
	{
		m_uInitState &=~TP_CREATESEND;
		return FALSE;
	}
	UINT nTTL = 32;
	if(m_SendSocket.SetSockOpt(IP_MULTICAST_TTL, &nTTL, sizeof(int), IPPROTO_IP) == 0)
	{
		m_uInitState &=~TP_CREATESEND;
		m_SendSocket.Close();
		return FALSE;		
	}

	int nLoopBack = 0;
	if(m_SendSocket.SetSockOpt(IP_MULTICAST_LOOP, &nLoopBack, sizeof(int), IPPROTO_IP) == 0)
	{	
		m_uInitState &=~TP_CREATESEND;
		m_SendSocket.Close();
		return FALSE;		
	}
	m_uInitState |=TP_CREATESEND;
	return TRUE;
}

//CTPBroastUdpSocket
BOOL CTPBroastUdpSocket::CreateListenSocket(UINT uPort)
{
	if (!Create( uPort,SOCK_DGRAM,FD_READ))
	{
		m_uInitState &=  ~TP_CREATELISTEN;	
		return FALSE;
	}

	BOOL b       = TRUE;
	int  eee     = TP_RECEIVE;

	if(setsockopt(this->m_hSocket, SOL_SOCKET, SO_BROADCAST, (char FAR *)&b,sizeof(BOOL)) < 0)
	{
		TP_GetSocketError();
		//return FALSE;
	}
	if(setsockopt(this->m_hSocket, IPPROTO_IP, SO_DONTROUTE, (char FAR *)&b, sizeof(BOOL)) < 0)
	{		 
		TP_GetSocketError();
		//return FALSE;
	} 
	if(setsockopt(this->m_hSocket, SOL_SOCKET, SO_RCVBUF, (char FAR *)&eee,sizeof(int)) < 0)
	{
		TP_GetSocketError();
		//return FALSE;
	}
	m_nLocalPort = uPort;	
	return TRUE;
}

BOOL CTPBroastUdpSocket::CreateSendSocket()
{
	if(!m_SendSocket.Create(0, SOCK_DGRAM, 0))
	{
		m_uInitState &=~TP_CREATESEND;
		return FALSE;
	}

	BOOL fBroadcast = TRUE;
	if(setsockopt ( m_SendSocket.m_hSocket,SOL_SOCKET,SO_BROADCAST,(CHAR *)&fBroadcast,sizeof ( BOOL )) != 0)
	{
		TP_GetSocketError();
		m_SendSocket.Close();
		m_uInitState &=~TP_CREATESEND;
		//return FALSE;
	}
	m_uInitState |=TP_CREATESEND;
	return TRUE;
}

BOOL CTPBroastUdpSocket::SendSocket(const void* strMessage, int nSize,LPCTSTR sIP)
{
	if(!CTPUdpSocket::SendSocket(strMessage,nSize,sIP))		return FALSE;

	USES_CONVERSION_EX;
	LPSTR lpszAscii = NULL;

	if(sIP != NULL)
		lpszAscii = T2A_EX((LPTSTR)sIP, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);

	SOCKADDR_IN saUdpServ;

	saUdpServ.sin_family = AF_INET;
	saUdpServ.sin_addr.s_addr = inet_addr(lpszAscii);// 或者用saUdpServ.sin_addr.s_addr = htonl(INADDR_BROADCAST);htonl ( INADDR_BROADCAST );//
	saUdpServ.sin_port = htons (TP_BROASTSERVER_PORT);//发送用的端口，可以根据需要更改
	saUdpServ.sin_addr.s_impno = 255;

	if(m_SendSocket.SendTo(strMessage, nSize, (SOCKADDR*)&saUdpServ, sizeof(SOCKADDR), 0) == SOCKET_ERROR)
		return FALSE;
	else
		return TRUE;
}




//
//void  CTPUdpBaseSocket::GetLocalInfo()
//{
//	char localHost[255];
//	gethostname(localHost, 255);
//	struct hostent *host = gethostbyname(localHost);	/* Get local host IP */
//	m_strLocalIP = inet_ntoa (*(struct in_addr*)*host->h_addr_list);
//	in_addr *pAdd = (struct in_addr*)*host->h_addr_list;
//	pAdd->S_un.S_un_b.s_b4 = 255;
//	m_strBroastIP  = inet_ntoa (*(struct in_addr*)*host->h_addr_list);
//	
//}
/////////////////////////////////////////////////////////////////////////////
// CTPUdpBaseSocket member functions
// SOL_SOCKET: 基本套接口  
// IPPROTO_IP: IPv4套接口  
// IPPROTO_IPV6: IPv6套接口  
// IPPROTO_TCP: TCP套接口 

// SO_REUSEADDR
// 这个套接字选项通知内核，如果端口忙，但TCP状态位于 TIME_WAIT ，可以重用端口。如果端口忙，而TCP状态位于其他状态，重用端口时依旧得
//到一个错误信息，指明"地址已经使用中"。如果你的服务程序停止后想立即重启，而新套接字依旧使用同一端口，此时 SO_REUSEADDR 选项非常有
//用。必须意识到，此时任何非期望数据到达，都可能导致服务程序反应混乱，不过这只是一种可能，事实上很不可能。
// 一个套接字由相关五元组构成，协议、本地地址、本地端口、远程地址、远程端口。SO_REUSEADDR 仅仅表示可以重用本地本地地址、本地端口，整
//个相关五元组还是唯一确定的。所以，重启后的服务程序有可能收到非期望数据。必须慎重使用 SO_REUSEADDR 选项。

// 对于局域网来说，我们可以使用的多播地址为224.0.0.0-224.0.0.255(想知道为什么？看看书吧。:-) )
// 多播是通过设置套接口(socket, 来自书上的翻译)选项来实现的，这个套接口必须是一个UDP的套接口
// IP_ADD_MEMBERSHIP: 加入一个多播组 
// IP_DROP_MEMBERSHIP: 离开一个多播组 
// IP_MULTICAST_IF: 指定外出多播数据报的外出接口 
// IP_MULTICAST_TTL: TTL数 
// IP_MULTICAST_LOOP: 是否禁止回馈，我的理解是一台机器是否可以接收到自己发送的多播数据报在设置IP_ADD_MEMBERSHIP和IP_DROP_MEMBERSHIP选项
// 的时候，我们需要用到ip_mreq结构, 要使用此结构，你必须include , 并确保此include位于include 之下

// SO_RCVBUF表示服务器端的用于接收数据的缓冲区的大小，以字节为单位。一般说来，传输大的连续的数据块（基于HTTP或FTP协议的数据传输）可以使用
// 较大的缓冲区，这可以减少传输数据的次数，从而提高传输数据的效率。而对于交互式的通信（Telnet和网络游戏），则应该采用小的缓冲区，确保能及时
// 把小批量的数据发送给对方。