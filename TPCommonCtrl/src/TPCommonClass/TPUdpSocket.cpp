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

	if(!m_SendSocket.Create(0, SOCK_DGRAM, 0))		// Create an unconnected UDP socket  ,SOCK_DGRAM���ݱ�ʽSocket��һ�������ӵ�Socket����Ӧ�������ӵ�UDP����Ӧ��
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
	saUdpServ.sin_addr.s_addr = inet_addr(lpszAscii);// ������saUdpServ.sin_addr.s_addr = htonl(INADDR_BROADCAST);htonl ( INADDR_BROADCAST );//
	saUdpServ.sin_port = htons (TP_BROASTSERVER_PORT);//�����õĶ˿ڣ����Ը�����Ҫ����
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
// SOL_SOCKET: �����׽ӿ�  
// IPPROTO_IP: IPv4�׽ӿ�  
// IPPROTO_IPV6: IPv6�׽ӿ�  
// IPPROTO_TCP: TCP�׽ӿ� 

// SO_REUSEADDR
// ����׽���ѡ��֪ͨ�ںˣ�����˿�æ����TCP״̬λ�� TIME_WAIT ���������ö˿ڡ�����˿�æ����TCP״̬λ������״̬�����ö˿�ʱ���ɵ�
//��һ��������Ϣ��ָ��"��ַ�Ѿ�ʹ����"�������ķ������ֹͣ�������������������׽�������ʹ��ͬһ�˿ڣ���ʱ SO_REUSEADDR ѡ��ǳ���
//�á�������ʶ������ʱ�κη��������ݵ�������ܵ��·������Ӧ���ң�������ֻ��һ�ֿ��ܣ���ʵ�Ϻܲ����ܡ�
// һ���׽����������Ԫ�鹹�ɣ�Э�顢���ص�ַ�����ض˿ڡ�Զ�̵�ַ��Զ�̶˿ڡ�SO_REUSEADDR ������ʾ�������ñ��ر��ص�ַ�����ض˿ڣ���
//�������Ԫ�黹��Ψһȷ���ġ����ԣ�������ķ�������п����յ����������ݡ���������ʹ�� SO_REUSEADDR ѡ�

// ���ھ�������˵�����ǿ���ʹ�õĶಥ��ַΪ224.0.0.0-224.0.0.255(��֪��Ϊʲô��������ɡ�:-) )
// �ಥ��ͨ�������׽ӿ�(socket, �������ϵķ���)ѡ����ʵ�ֵģ�����׽ӿڱ�����һ��UDP���׽ӿ�
// IP_ADD_MEMBERSHIP: ����һ���ಥ�� 
// IP_DROP_MEMBERSHIP: �뿪һ���ಥ�� 
// IP_MULTICAST_IF: ָ������ಥ���ݱ�������ӿ� 
// IP_MULTICAST_TTL: TTL�� 
// IP_MULTICAST_LOOP: �Ƿ��ֹ�������ҵ������һ̨�����Ƿ���Խ��յ��Լ����͵Ķಥ���ݱ�������IP_ADD_MEMBERSHIP��IP_DROP_MEMBERSHIPѡ��
// ��ʱ��������Ҫ�õ�ip_mreq�ṹ, Ҫʹ�ô˽ṹ�������include , ��ȷ����includeλ��include ֮��

// SO_RCVBUF��ʾ�������˵����ڽ������ݵĻ������Ĵ�С�����ֽ�Ϊ��λ��һ��˵�������������������ݿ飨����HTTP��FTPЭ������ݴ��䣩����ʹ��
// �ϴ�Ļ�����������Լ��ٴ������ݵĴ������Ӷ���ߴ������ݵ�Ч�ʡ������ڽ���ʽ��ͨ�ţ�Telnet��������Ϸ������Ӧ�ò���С�Ļ�������ȷ���ܼ�ʱ
// ��С���������ݷ��͸��Է���