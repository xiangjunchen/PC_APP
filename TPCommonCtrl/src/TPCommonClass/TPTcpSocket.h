#pragma once

class	CTPNetInterface;

class	CTPTcpListenSocket : public CAsyncSocket  
{
public:
	CTPTcpListenSocket(CTPNetInterface   *pTemCmdProxy = NULL);
	virtual ~CTPTcpListenSocket();

public:
	void         GetTickCount(BOOL bStart);
	BOOL         Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	bool         IsClose(); 
	CString      GetIP(sockaddr_in &stuIP);

public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);

public:
	DWORD              	m_lConnectTimeOut;
	DWORD              	m_lSendTimeOut;
	DWORD              	m_lReciveTimeOut;
	sockaddr_in       	m_stuAcceptSockAddr ;
	int               	m_lAcceptSockAddrLen;
	sockaddr_in       	m_stuLocalSockAddr ;
	CString           	m_sLocalIP;
	CString           	m_sListenIP;

private:
	CTPNetInterface 	*m_pTemCmdProxy;
	LPBYTE          	 m_pRecive;
	long            	 m_lRecive;
	long            	 m_lBuffer;
	long            	 m_lBaseLen;
	DWORD           	 m_lTickCount;
	DWORD           	 m_lTickReceive;
};

class   CTPTcpSendSocket   
{
public:
	CTPTcpSendSocket();
	virtual ~CTPTcpSendSocket();

public:
	bool        Create();
	bool        Connect(const unsigned short * lpszHostAddress, unsigned int nHostPort);
	void        Close();
	int         Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	int         Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	bool        IsClose();
public:
	unsigned long       m_lConnectTimeOut;
	unsigned long       m_lSendTimeOut;
	unsigned long       m_lReciveTimeOut; 
	unsigned int        m_hSocket;
	CString             m_sLocalIP;
};

//‘›Œ¥ π”√
class CTPTcpSocketEx : public CSocket  
{
public:
	CTPTcpSocketEx();

public:
	BOOL        Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
public:
	DWORD       m_lConnectTimeOut;
	DWORD       m_lSendTimeOut;
	DWORD       m_lReciveTimeOut; 

};


