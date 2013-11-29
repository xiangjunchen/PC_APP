#pragma once

#include <Winsock2.h>
#include <afxsock.h>
#include <WS2tcpip.h>


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TPMulticastSocket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#define TP_RECEIVE   900
#define TP_BROASTMODE 0x00000001
#define TP_MULTIMODE  0x00000002

#define TP_BROASTSERVER_PORT  5024
#define TP_CLIPSERVER_PORT    2044
#define TP_MULTICAST_ADRESS	"239.5.6.13"  //224.0,0,0 - 239.255.255.255֮�������ֵ

#define TP_CREATELISTEN		0x00000001
#define TP_CREATESEND		0x00000002


 class CTPUdpSocket :public CAsyncSocket
{
public:
	CTPUdpSocket();
	virtual ~CTPUdpSocket();

	//send 
	virtual BOOL  CreateSendSocket();
	virtual BOOL  CloseSendSocket();
	virtual	BOOL  SendSocket(const void* strMessage, int nSize,LPCTSTR sIP);
	//listen
	virtual BOOL  CreateListenSocket(UINT uPort);
	virtual BOOL  CreateListenSocket(LPCSTR lpIP, UINT uPort);
	virtual BOOL  CloseListenSocket();
	virtual void  OnReceive(int nErrorCode);

	//base
	BOOL GetMultiSendAddress(LPCSTR lpIP, UINT uPort);

protected:
	char          m_strBuffer[TP_RECEIVE];	//�������ݵĴ�С
	SOCKADDR_IN   m_saHostGroup;        	//UDP �����Ϣ
	ip_mreq       m_mrMReq;			        //����Ϣ

	UINT          m_nRecivePort;		    //�������ݶ˿�
	CString       m_strReciveIP;		    //��������IP	
	CAsyncSocket  m_SendSocket;	            //�㲥	

	long          m_lBaseLen;       
	UINT          m_uInitState;
	CString       m_sError;
public:
	CStringArray  m_aOtherNet;             //��IP��ַ

	CStringArray  m_aThisIP;
	UINT          m_nLocalPort;		   	  //���ض˿�
	CString       m_strLocalIP;		      //����IP
	CString       m_strBroastIP;          //�㲥Ŀ���ַ	

	
	void        (*m_fMulticastCallBack)( LPBYTE, long,LPVOID,LPCTSTR) ;
	LPVOID        m_pMulticastParam;
};
class CTPMultiUdpSocket :public CTPUdpSocket
{
public:
	virtual BOOL CreateListenSocket(LPCSTR lpIP, UINT uPort);
	virtual BOOL CreateSendSocket();
	virtual BOOL CloseListenSocket();
	virtual	BOOL  SendSocket(const void* strMessage, int nSize,LPCTSTR sIP);

protected:
private:
};
class CTPBroastUdpSocket :public CTPUdpSocket
{
public:
	virtual BOOL CreateListenSocket(UINT uPort);
	virtual BOOL CreateSendSocket();
	virtual	BOOL  SendSocket(const void* strMessage, int nSize,LPCTSTR sIP);

protected:
private:

};
