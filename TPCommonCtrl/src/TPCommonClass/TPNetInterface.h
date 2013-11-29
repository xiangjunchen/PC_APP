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
	TPNetCmd_Open,				//用户登陆
	TPNetCmd_Connect,			//用户握手成功反馈
	TPNetCmd_ClipDisplayCmd,			//Team刷新命令
	TPNetCmd_BrdCmd,			//蓝光刷新命令
	TPNetCmd_MsgString,		//文本消息
	TPNetCmd_MsgData,			//数据消息
	TPNetCmd_Close,			//用户退出
	TPNetCmd_TickCount,		//用户退出
	TPNetCmd_Ask,				//用户立刻回答命令

	TPNetCmd_SetUserList,		//取得某个窗口的用户列表
	TPNetCmd_WndState,			//聊天窗口状态
	TPNetCmd_FileChange,   	//聊天窗口状态



	TPNetCmd_AskVoiceChat,		//请求语音聊天
	TPNetCmd_BeginVoiceChat,	//开始语音聊天
	TPNetCmd_CloseVoiceChat,	//关闭语音聊天
	TPNetCmd_DisChatWnd,		//被踢出群聊

	TPNetCmd_UnConnect,    	//连接丢失
};
enum TPTeamWndMsgType    //发送给消息接收窗口命令
{
	TPTeamMsg_None,
	TPTeamMsg_Login,				//用户登陆
	TPTeamMsg_Logoff,				//用户退出
	TPTeamMsg_MessageString,   		//文本消息
	TPTeamMsg_MessageData,   		//数据消息
	TPTeamMsg_SetUserList,			//取得某个窗口的用户列表
	TPTeamMsg_WndState,				//聊天窗口状态
	TPTeamMsg_AskVoiceChat,			//请求语音聊天
	TPTeamMsg_BeginVoiceChat,		//开始语音聊天
	TPTeamMsg_CloseVoiceChat,		//关闭语音聊天
	TPTeamMsg_DisChatWnd,			//被踢出群聊
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
	CString    sIP;          //登陆计算机IP
	CString    sLocalIP;     //本机IP
	BYTE       bIP[4];       //登陆计算机IP
	UINT       uPort;        //登陆计算机侦听端口
	BYTE       cAppType;     //登陆用户程序类型，避免消息发串了
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
	CString     sTeamUser;       //应用程序登陆名
	TPResGuidEx guidTeamUser;    //非编登陆用户GUID
	TPResGuidEx guidOpenPrj;     //打开的项目
	TPResGuidEx guidOpenPro;     //打开的节目
	TPResGuidEx guidOpenEff;     //打开的特技
	TPResGuidEx guidRedord;      //正在采集素材
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

//拟增加用户最后通信标识  gettickcount
struct tag_TPUser :public tag_TPUserIP,public tag_TPUserState //当前在线用户列表
{
	CString    sUser;           //计算机登陆名
	CString    sComputer;       //登陆计算机名
	CString    sMessage;  

	DWORD      lParam1;	    
	DWORD      lParam2;	 
	DWORD      lParam3;	 
	DWORD      lParam4;	 
	DWORD      dwTime;

	int        iConnectFail; //连接失败次数
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
	CTPTcpListenSocket      *m_pTcpListenSocket1;  //监听Socket
	CTPTcpListenSocket      *m_pTcpListenSocket2;  //监听Socket
	CTPUdpSocket       *m_pUdpListenSocket1; //多网卡监听
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
	struct tag_TPNetCmdHead  //信息头部
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
	struct tag_TPNetCmdUseInfo :public tag_TPNetCmdHead //登陆用户信息,用于握手
	{
		BYTE        bIP[4];     
		TCHAR       sUser[TP_TEAMUSER_MAX];	     //登陆用户名
		TCHAR       sTeamUser[TP_TEAMUSER_MAX];   //登陆用户名
		TCHAR       sComputer[TP_TEAMUSER_MAX];   //机器名
		TCHAR       sIP[16];

		TPResGuidEx guidTeamUser;    //非编登陆用户GUID
		TPResGuidEx guidOpenPrj;     //打开的项目
		TPResGuidEx guidOpenPro;     //打开的节目
		TPResGuidEx guidOpenEff;     //打开的特技
		TPResGuidEx guidRedord;      //正在采集素材

		tag_TPNetCmdUseInfo(TPNetCmd nCmd)
		{
			eTeamCmd = nCmd;

		}
	};
	struct tag_TPTeamLoginUseInfo   //查询用户线程，用于按IP查询当前在线用户
	{
		tag_TPNetCmdUseInfo *pInfo;        //用户信息
		HANDLE                hThread;      //当前查询线程
		CString               sIP;		 	//当前查询IP 
		BOOL                  bConnect;     //是否需要查询
		tag_TPTeamLoginUseInfo()
		{		 
			hThread   = NULL;
			bConnect  = TRUE;
		}
	};
	struct tag_TPReciveData //接受到的数据列表
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
	struct tag_TPTeaMsgThread //发送数据线程
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

	UINT                           m_uOpenSate;         //当前打开状态

	HWND                           m_hLoginWnd;        //用户登陆消息窗口
	HWND                           m_hMessageWnd;      //发送信息接收窗口
	void                         (*m_fClipDisplayCmdCallBack)(LPBYTE ,long ,LPVOID,CTPTcpListenSocket * ); //ClipManage Team 资源回调函数
	LPVOID                         m_pTeamCmdParam;                           //ClipManage Team 资源回调参数
	BOOL                         (*m_fAskCmdCallBack)(LPBYTE ,long ,LPVOID); //ClipManage Team 资源回调函数
	LPVOID                         m_pAskCmdParam;                           //ClipManage Team 资源回调参数
	void                         (*m_fBrdCmdCallBack)(LPBYTE ,long ,LPVOID,CTPTcpListenSocket * ); // 
	LPVOID                         m_pBrdCmdParam;                                           // 
	void                         (*m_fFileCmdCallBack)(LPBYTE ,long ); // 
	void                          (*m_fDriverCallBack)(LPCTSTR ,UINT  ,LPARAM  );
	LPARAM                          m_lDriverParam;

	CString                        m_sUser;     //本机用户名
	CString                        m_sComputer; //本机计算机名
	CString                        m_sIP;       //本机IP
	BYTE                           m_bIP[4];    //本机IP
	CStringArray                   m_aThisIP;   //本机多网卡IP
	UINT                           m_uPort;
	BYTE                           m_cAppType;
	CString                        m_sLastError;

public:
	tag_TPUserState                m_stuUserState;	
private:

	TPTeamUserArray                                    m_aUser;    //当前在线用户目录
	CArray<CTPTcpListenSocket*,CTPTcpListenSocket *&>            m_aSocket;  //负责连接的临时Socket
	CArray<tag_TPReciveData *,tag_TPReciveData *&>     m_aReciveData; //接收等待处理的数据列表
	tag_TPUser                                         m_stuUserInfo; //临时保存用户信息，用于给窗口PostMessage

	CArray<tag_TPTeaMsgThread *,tag_TPTeaMsgThread *&> m_aSendThread; //发送数据线程列表


	CTPListenThread                 *m_pListenThread;

	BOOL                           m_bReciveThread;  //接收数据处理线程
	HANDLE                         m_hReciveThread;  

	CRITICAL_SECTION			   m_csTeamCmd;      //公共数据锁

	CTPUdpSocket                  *m_pUdpSendSocket;     //UDP发送监听
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
public:   //Team专用
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
	void				SendAskVoiceChat(TPUserIPArray &aIP,UINT uWndType);//请求语音聊天
	void				SendBeginVoiceChat(TPUserIPArray &aIP,UINT uWndType);//开始语音聊天
	void				SendDisWnd(TPUserIPArray &aIP, UINT uWndType);//禁止输入，被踢出群聊
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
