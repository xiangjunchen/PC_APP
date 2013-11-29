#pragma once
#include "afxsock.h"
#include "winsock2.h"
#include <Ws2tcpip.h>

#define  TP_TEAMUSER_MAX   50
#define  WM_TEAMUSER       WM_USER + 2237

#define  TP_CMDFLAG   0x4321234F
#define  TP_CMDVER    7
#define  TP_TEAMNAME  MAX_PATH

#define TP_NS_PORT     4601
#define TP_MAX_SEND    1000
#define TP_HEARTCOUNT  30*1000


#define   TP_USEUDPLOGIN
#define   CTPTeamSendSocke CTPTcpSendSocket
#define   TP_UDPMODE TP_MULTIMODE

enum TPNetCmd
{
	TPNetCmd_None,
	TPNetCmd_Open,				//�û���½
	TPNetCmd_Connect,			//�û����ֳɹ�����
	TPNetCmd_ClipDisplayCmd,			//Teamˢ������
	TPNetCmd_BrdCmd,			//����ˢ������
	TPNetCmd_MsgString,		//�ı���Ϣ
	TPNetCmd_MsgData,			//������Ϣ
	TPNetCmd_Close,			//�û��˳�
	TPNetCmd_TickCount,		//�û��˳�
	TPNetCmd_Ask,				//�û����̻ش�����

	TPNetCmd_SetUserList,		//ȡ��ĳ�����ڵ��û��б�
	TPNetCmd_WndState,			//���촰��״̬
	TPNetCmd_FileChange,   	//���촰��״̬



	TPNetCmd_AskVoiceChat,		//������������
	TPNetCmd_BeginVoiceChat,	//��ʼ��������
	TPNetCmd_CloseVoiceChat,	//�ر���������
	TPNetCmd_DisChatWnd,		//���߳�Ⱥ��

	TPNetCmd_UnConnect,    	//���Ӷ�ʧ
};
enum TPTeamWndMsgType    //���͸���Ϣ���մ�������
{
	TPTeamMsg_None,
	TPTeamMsg_Login,				//�û���½
	TPTeamMsg_Logoff,				//�û��˳�
	TPTeamMsg_MessageString,   		//�ı���Ϣ
	TPTeamMsg_MessageData,   		//������Ϣ
	TPTeamMsg_SetUserList,			//ȡ��ĳ�����ڵ��û��б�
	TPTeamMsg_WndState,				//���촰��״̬
	TPTeamMsg_AskVoiceChat,			//������������
	TPTeamMsg_BeginVoiceChat,		//��ʼ��������
	TPTeamMsg_CloseVoiceChat,		//�ر���������
	TPTeamMsg_DisChatWnd,			//���߳�Ⱥ��
};

struct tag_TPComputer  
{
	CString      sComputer;
	CString      sIP;
	CStringArray aShareFile;

	tag_TPComputer()
	{ 
	}
	tag_TPComputer & tag_TPComputer::operator = (tag_TPComputer &src)
	{ 	 
		sComputer  = src.sComputer;
		sIP        = src.sIP;
		aShareFile.Copy(src.aShareFile);
		return *this;
	}
};
typedef  CArray<tag_TPComputer,tag_TPComputer&> TPComputerArray;

struct TPResGuidEx
{
	GUID      guidRes;
	ULONGLONG eResType;
	GUID      guidDBType; 
	TPResGuidEx()
	{
		guidRes    = GUID_NULL;
		eResType   = 0;
		guidDBType = GUID_NULL; 
	}	
	TPResGuidEx & TPResGuidEx::operator = (TPResGuidEx &other)	
	{
		guidRes    = other.guidRes;
		eResType   = other.eResType;
		guidDBType = other.guidDBType; 
		return *this;
	}
};
typedef struct tag_TPUserIP
{
	CString    sIP;          //��½�����IP
	CString    sLocalIP;     //����IP
	BYTE       bIP[4];       //��½�����IP
	UINT       uPort;        //��½����������˿�
	BYTE       cAppType;     //��½�û��������ͣ�������Ϣ������
	tag_TPUserIP()
	{
		sIP      = _T("0.0.0.0");
		sLocalIP = _T("0.0.0.0");
		uPort    = 0;
		cAppType = 0;
		ZeroMemory(bIP,sizeof(bIP));
	}
	tag_TPUserIP(LPCTSTR sIP,UINT uPort)
	{
		this ->sIP      = sIP;
		this ->sLocalIP = _T("0.0.0.0");
		this ->uPort    = uPort;
		cAppType        = 0;
		ZeroMemory(bIP,sizeof(bIP));
	}
	tag_TPUserIP & tag_TPUserIP::operator = (tag_TPUserIP src)
	{
		sIP      = src.sIP;
		sLocalIP = src.sLocalIP;
		uPort    = src.uPort;
		cAppType = src.cAppType;
		memcpy_s(bIP,4,src.bIP,4);
		return *this;
	}

}TPUserIP;
typedef CArray <TPUserIP ,TPUserIP&> TPUserIPArray;

typedef struct tag_TPUserState
{
	CString     sTeamUser;       //Ӧ�ó����½��
	TPResGuidEx guidTeamUser;    //�Ǳ��½�û�GUID
	TPResGuidEx guidOpenPrj;     //�򿪵���Ŀ
	TPResGuidEx guidOpenPro;     //�򿪵Ľ�Ŀ
	TPResGuidEx guidOpenEff;     //�򿪵��ؼ�
	TPResGuidEx guidRedord;      //���ڲɼ��ز�
	tag_TPUserState()
	{
		sTeamUser               = _T("");
		guidTeamUser.guidRes    = GUID_NULL;
		guidOpenPrj.guidRes     = GUID_NULL;  
		guidOpenPro.guidRes     = GUID_NULL;   
		guidOpenEff.guidRes     = GUID_NULL;
		guidRedord.guidRes      = GUID_NULL;
	}
	tag_TPUserState & tag_TPUserState::operator = (tag_TPUserState src)
	{ 	 
		sTeamUser              = src.sTeamUser;		 
		guidTeamUser           = src.guidTeamUser;
		guidOpenPrj            = src.guidOpenPrj;
		guidOpenPro            = src.guidOpenPro;
		guidOpenEff            = src.guidOpenEff;
		guidRedord             = src.guidRedord;
		return *this;
	}
}TPUserState;

//�������û����ͨ�ű�ʶ  gettickcount
struct tag_TPUser :public tag_TPUserIP,public tag_TPUserState //��ǰ�����û��б�
{
	CString    sUser;           //�������½��
	CString    sComputer;       //��½�������
	CString    sMessage;  

	DWORD      lParam1;	    
	DWORD      lParam2;	 
	DWORD      lParam3;	 
	DWORD      lParam4;	 
	DWORD      dwTime;

	int        iConnectFail; //����ʧ�ܴ���
	tag_TPUser()
	{
		sUser         = _T("");
		sComputer     = _T(""); 
		sMessage      = _T("");  

		iConnectFail  = 0;
		lParam1       = 0;
		lParam2       = 0;
		lParam3       = 0;
		lParam4       = 0;
		dwTime        = 0;

	}
	tag_TPUser & tag_TPUser::operator = (tag_TPUser src)
	{ 
		*(tag_TPUserIP *)this    = *(tag_TPUserIP *)&src;
		*(tag_TPUserState *)this = *(tag_TPUserState *)&src;

		sUser                    = src.sUser;
		sComputer                = src.sComputer;
		sMessage                 = src.sMessage;	

		lParam1                  = src.lParam1;
		lParam2                  = src.lParam2;
		lParam3                  = src.lParam3;
		lParam4                  = src.lParam4;
		iConnectFail             = src.iConnectFail;
		dwTime                   = src.dwTime;
		return *this;
	}
	TPUserIP &  GetUserIP()
	{
		return *(tag_TPUserIP *)this;
	}
};
typedef CArray <tag_TPUser ,tag_TPUser&> TPTeamUserArray;


class   CTPListenThread : public CWinThread
{
	DECLARE_DYNCREATE(CTPListenThread)

public:
	CTPListenThread();
	CTPListenThread(UINT nIPort);           // protected constructor used by dynamic creation
	virtual ~CTPListenThread();

public:
	CTPTcpListenSocket      *m_pTcpListenSocket1;  //����Socket
	CTPTcpListenSocket      *m_pTcpListenSocket2;  //����Socket
	CTPUdpSocket       *m_pUdpListenSocket1; //����������
	CTPUdpSocket       *m_pUdpListenSocket2;
	CTPNetInterface    *m_pTeamCmdProxy;
	HANDLE              m_hFinishEvent;
	CStringArray        m_aLocalIP;		 
public:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
};


class AFX_EXT_CLASS CTPNetInterface : public CTPBaseNetInterface
{
public:
	CTPNetInterface();
	~CTPNetInterface(void);
public:
	struct tag_TPNetCmdHead  //��Ϣͷ��
	{
		TPNetCmd			eTeamCmd;  
		long				lSendSize;
		long				lDataSize;
		long				lBufSize;
		BYTE				cIP[4];    
		BYTE				cAppType;
		unsigned int        dwFlag;
		unsigned int        dwVersion;
		unsigned int        uPort;      
		unsigned int        lParam1;	 
		unsigned int        lParam2;		 

		tag_TPNetCmdHead()
		{
			dwFlag     = TP_CMDFLAG;
			dwVersion  = TP_CMDVER;
			eTeamCmd   = TPNetCmd_None;
			lSendSize  = 0;
			lDataSize  = 0;
			lBufSize   = 0;
			cIP[0]     = 0;
			uPort      = 0;
			cAppType   = 0;
			lParam1    = 0;
			lParam2    = 0;
		 
		}		 
		BOOL CheckData(long lSize)
		{
			if(lSize<lSendSize)          { ASSERT(0);    return FALSE;}
			if(dwFlag != TP_CMDFLAG)     { ASSERT(0);    return FALSE;}
			if(dwVersion != TP_CMDVER)     return FALSE;
			return TRUE;
		}
	};	
	struct tag_TPNetCmdUseInfo :public tag_TPNetCmdHead //��½�û���Ϣ,��������
	{
		BYTE        bIP[4];     
		TCHAR       sUser[TP_TEAMUSER_MAX];	     //��½�û���
		TCHAR       sTeamUser[TP_TEAMUSER_MAX];   //��½�û���
		TCHAR       sComputer[TP_TEAMUSER_MAX];   //������
		TCHAR       sIP[16];

		TPResGuidEx guidTeamUser;    //�Ǳ��½�û�GUID
		TPResGuidEx guidOpenPrj;     //�򿪵���Ŀ
		TPResGuidEx guidOpenPro;     //�򿪵Ľ�Ŀ
		TPResGuidEx guidOpenEff;     //�򿪵��ؼ�
		TPResGuidEx guidRedord;      //���ڲɼ��ز�

		tag_TPNetCmdUseInfo(TPNetCmd nCmd)
		{
			eTeamCmd = nCmd;

		}
	};
	struct tag_TPTeamLoginUseInfo   //��ѯ�û��̣߳����ڰ�IP��ѯ��ǰ�����û�
	{
		tag_TPNetCmdUseInfo *pInfo;        //�û���Ϣ
		HANDLE                hThread;      //��ǰ��ѯ�߳�
		CString               sIP;		 	//��ǰ��ѯIP 
		BOOL                  bConnect;     //�Ƿ���Ҫ��ѯ
		tag_TPTeamLoginUseInfo()
		{		 
			hThread   = NULL;
			bConnect  = TRUE;
		}
	};
	struct tag_TPReciveData //���ܵ��������б�
	{
		LPBYTE  pBuffer;
		long    lSize;
		CString sLocalIP;
		tag_TPReciveData()
		{
			pBuffer = NULL;
		}
		~tag_TPReciveData()
		{
			if(pBuffer) delete pBuffer;
		}
	};
	struct tag_TPTeaMsgThread //���������߳�
	{ 		 
		BOOL                                           bOpenThread;
		HANDLE                                         hOpenThread;  
		CWinThread                                    *pRunThread; 
		CTPNetInterface                               *pProxy;
		CString                                        sTP;
		CString                                        sLocalIP;
		UINT                                           uPort;
		CRITICAL_SECTION			                   csTeamCmd;   
		CTPTcpSendSocket                           *pSocket; 
		CArray<tag_TPReciveData *,tag_TPReciveData *&> aReciveData;  

		tag_TPTeaMsgThread()
		{			 
			bOpenThread  = FALSE;
			hOpenThread  = ::CreateEvent( NULL, TRUE, FALSE, NULL);;  
			pRunThread   = NULL; 		
			pProxy       = NULL;
			sTP          = _T("0.0.0.0");
			uPort        = 0;
			pSocket      = NULL;
			InitializeCriticalSection(&csTeamCmd);
		}
		void Stop()
		{
			if(bOpenThread)
			{
				bOpenThread = FALSE;
				::WaitForSingleObject(hOpenThread,INFINITE);
			}
			RemoveData();
		}
		void SendData(LPBYTE pBuffer,long lSize,BOOL bDelData,int iPos = -1);		 
		void RemoveData()
		{
			EnterCriticalSection(&csTeamCmd);
			for(INT l=0;l<aReciveData.GetSize();l++)
				delete aReciveData[l];
			aReciveData.RemoveAll();			 
			LeaveCriticalSection(&csTeamCmd);
		}
		~tag_TPTeaMsgThread()
		{
			Stop();			
			::CloseHandle(hOpenThread);
			hOpenThread = NULL;
			pProxy      = NULL;
			pRunThread  = NULL;
			sTP         = _T("");
			DeleteCriticalSection(&csTeamCmd); 
			if(pSocket)
			{
				pSocket->Close();
				delete  pSocket;
				pSocket = NULL;
			}
		}
	};

	UINT                           m_uOpenSate;         //��ǰ��״̬

	HWND                           m_hLoginWnd;        //�û���½��Ϣ����
	HWND                           m_hMessageWnd;      //������Ϣ���մ���
	void                         (*m_fClipDisplayCmdCallBack)(LPBYTE ,long ,LPVOID,CTPTcpListenSocket * ); //ClipManage Team ��Դ�ص�����
	LPVOID                         m_pTeamCmdParam;                           //ClipManage Team ��Դ�ص�����
	BOOL                         (*m_fAskCmdCallBack)(LPBYTE ,long ,LPVOID); //ClipManage Team ��Դ�ص�����
	LPVOID                         m_pAskCmdParam;                           //ClipManage Team ��Դ�ص�����
	void                         (*m_fBrdCmdCallBack)(LPBYTE ,long ,LPVOID,CTPTcpListenSocket * ); // 
	LPVOID                         m_pBrdCmdParam;                                           // 
	void                         (*m_fFileCmdCallBack)(LPBYTE ,long ); // 
	void                          (*m_fDriverCallBack)(LPCTSTR ,UINT  ,LPARAM  );
	LPARAM                          m_lDriverParam;

	CString                        m_sUser;     //�����û���
	CString                        m_sComputer; //�����������
	CString                        m_sIP;       //����IP
	BYTE                           m_bIP[4];    //����IP
	CStringArray                   m_aThisIP;   //����������IP
	UINT                           m_uPort;
	BYTE                           m_cAppType;
	CString                        m_sLastError;

public:
	tag_TPUserState                m_stuUserState;	
private:

	TPTeamUserArray                                    m_aUser;    //��ǰ�����û�Ŀ¼
	CArray<CTPTcpListenSocket*,CTPTcpListenSocket *&>            m_aSocket;  //�������ӵ���ʱSocket
	CArray<tag_TPReciveData *,tag_TPReciveData *&>     m_aReciveData; //���յȴ�����������б�
	tag_TPUser                                         m_stuUserInfo; //��ʱ�����û���Ϣ�����ڸ�����PostMessage

	CArray<tag_TPTeaMsgThread *,tag_TPTeaMsgThread *&> m_aSendThread; //���������߳��б�


	CTPListenThread                 *m_pListenThread;

	BOOL                           m_bReciveThread;  //�������ݴ����߳�
	HANDLE                         m_hReciveThread;  

	CRITICAL_SECTION			   m_csTeamCmd;      //����������

	CTPUdpSocket                  *m_pUdpSendSocket;     //UDP���ͼ���
	BOOL                           m_bOpen;

	CTPBaseThread                  m_hHeartCount;



public:
	BOOL               Open( );
	BOOL               Close();
	BOOL               Release();

	void               LoginInUser(BOOL bClearUser = FALSE);
	void               LoginOutUser();
	void			   CheckUserState();  
	void               SendTickCount();
	BOOL               IsConnect();

	void               SendMessageString(TPUserIPArray &aIP,CString sMessage,UINT uWndType = 0);
	void			   SendMessageData(TPUserIPArray &aIP,CString &sTitle,UINT uDataType,BYTE *bmpByte,int iSize,UINT uWndType = 0);
	void               SendBaseCmd(TPNetCmd eCmd,LPBYTE pBuffer,long lBufSize); 
	void               SendFileCmd(LPBYTE pBuffer,long lBufSize);
	void               SendAskCmd(LPBYTE pBuffer,long lBufSize,BOOL (*f)(LPBYTE ,long ,LPVOID ),LPVOID pParam); 
	void               SendUserListCmd(TPUserIPArray &aIP,DWORD uWndType,CStringArray &aIPSend);
	void               SendMsgWndState(TPUserIPArray &aIP,BOOL bOpen,UINT uWndType = 0);

	void                ReciveDataThread();
	void                SetReceiveData(LPBYTE pBuffer,long lSize,BOOL bDelete,CTPTcpListenSocket *pSocket,LPCTSTR sLocalIP);

	CTPTcpListenSocket      *GetAcceptSocket();
	void                ReleaseAcceptSocket(CTPTcpListenSocket * pSocket);

	tag_TPTeaMsgThread *GetSendThread(TPUserIP & sUserIP);
	void                ReleaseSendThread(tag_TPTeaMsgThread * pThread);
	void                ParserIP(CString &sIP,CStringArray &aIp,BOOL bThis);
	void                ParserUser(CString &sIP,TPTeamUserArray &aUser,BOOL bThis);

	void			    ResetCallBack(BOOL bSet);
public:   //Teamר��
	void               SetTeamUser(GUID guidDBType,GUID guidTeamUser,LPCTSTR sUser);
	BOOL               SendAskTeamUser(GUID guidLogin); 
	void               SetPrjOpen(GUID guidUser,GUID guidDBType,GUID guidRes,BOOL bOpen);
	void               SetProOpen(GUID guidUser,GUID guidDBType,GUID guidRes,BOOL bOpen);
	void               SetEffOpen(GUID guidUser,GUID guidDBType,GUID guidRes,BOOL bOpen);
	int                GetPrjOpen(GUID guidUser,GUID guidDBType,GUID guidRes);
	int                GetProOpen(GUID guidUser,GUID guidDBType,GUID guidRes);
	int                GetEffOpen(GUID guidUser,GUID guidDBType,GUID guidRes);
	void               SetRecoreOpen(GUID guidUser,GUID guidDBType,GUID guidRes,BOOL bOpen);

public:
	void                GetUserInfo();
	void                GetLoginUser(TPTeamUserArray &aUser,int iFailConnect);
	void                SetConnectState(CString sIP,BOOL bConnect,BOOL bRemove = FALSE);
	void				SendAskVoiceChat(TPUserIPArray &aIP,UINT uWndType);//������������
	void				SendBeginVoiceChat(TPUserIPArray &aIP,UINT uWndType);//��ʼ��������
	void				SendDisWnd(TPUserIPArray &aIP, UINT uWndType);//��ֹ���룬���߳�Ⱥ��
	void				SendCloseVoiceChat(TPUserIPArray &aIP, UINT uWndType);

private:
	void                OpenUser();
	void                CloseUser();

	void                OpenListen();
	void                CloseListen();

	void                StartHeartCount();


	void                CloseSendCmd();

	void                SendUserLogin(tag_TPUser *pData,BOOL bLogin);
	void                GetLoginUserIp(TPUserIPArray &aIP,int lLevel = 2,BOOL bCheckType = FALSE);
	void                GetTeamCmdUseInfo(tag_TPNetCmdUseInfo *pInfo);
	BOOL                SendConnect(tag_TPUser & sIP );

	void                GetLocalIP(tag_TPNetCmdUseInfo *pInfo,LPCTSTR sLocalIP);
};

AFX_EXT_API   void     TP_GetLoginUserInfo(CString &sLoginUser,CString &sComputer,CString &sIP);

#define   TPNetCmdHead      CTPNetInterface::tag_TPNetCmdHead
