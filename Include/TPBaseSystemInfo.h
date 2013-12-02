#pragma	  once

//#include "TPBaseVADef.h"
#pragma warning(disable:4244)

enum	TP_EDITMAX_VERSION	// 非编版本
{
	TP_EDITMAX_E7 = 0,		
	TP_EDITMAX_XPRINEWS,   		
	TP_EDITMAX_SOBEYNEWS,
};

enum	TP_EDITMAX_SUB_VERSION	// 非编版本
{
	TP_NULL_STATION  = 0,
	TP_CRAFTEDIT_STATION,		
	TP_PROXYEDIT_STATION,   		
};
//注意以下定义在commonctrl中有重复定义，如果修改，请同步
#define TP_SYSTEMSTATE_LOGING       0x00000001
#define TP_SYSTEMSTATE_EXIT         0x00000002
#define TP_SYSTEMSTATE_CLOSEPRJ     0x00000004
#define TP_SYSTEMSTATE_RECSTOPPING	0x00000008 
#define TP_SYSTEMSTATE_CLEARTOOL	0x00000010 
#define TP_SYSTEMSTATE_XDA      	0x00000020 
#define TP_SYSTEMSTATE_OS64      	0x00000040 
#define TP_SYSTEMSTATE_OS64APP32  	0x00000080
#define TP_SYSTEMSTATE_WIN7      	0x00000100 
#define TP_SYSTEMSTATE_TEAMMODE    	0x00000200
#define TP_SYSTEMSTATE_NOLOCAL    	0x00000400
#define TP_SYSTEMSTATE_COMPILING    0x00000800
#define TP_SYSTEMSTATE_NETMODE	    0x00001000
#define TP_SYSTEMSTATE_X1NOEXIST	0x00002000
#define TP_SYSTEMSTATE_X1INIERR		0x00004000


#define TP_SYSTEMPLUGINLOAD_UNKNOWN					0x00000000
#define TP_SYSTEMPLUGINLOAD_RESEXPLORER				0x00000001
#define TP_SYSTEMPLUGINLOAD_CLIPDISPLAY				0x00000002
#define TP_SYSTEMPLUGINLOAD_CIRCLEEXPLORER			0x00000004
#define TP_SYSTEMPLUGINLOAD_APPLICATIONEXPLORER		0x00000008


#define TP_SYSTEMSTATE_SYSTEM64    (TP_SYSTEMSTATE_OS64 |  TP_SYSTEMSTATE_OS64APP32)
 


#ifdef TP_NS_NET
enum    TP_EDITMAX_PROJECT_VERSION
{
    TP_NULL_PROJECT = 0,
    TP_CX_PROJECT,		
};
#endif

struct TPOSStateInterfce
{
	BOOL (*GetIsVista)();
	BOOL (*GetIsEffectByUAC)();
	BOOL (*GetUACStatus)();
	BOOL (*IsAffectedPath)(const CString &strPath);
	BOOL (*GetDWMStatus)(BOOL bForce);
	void (*SetDWMStatus)(BOOL bOpened);
	void (*GetProgramDataSysPath)(CString &strPath);
	void (*GetProgramDataAppPath)(CString &strPath);
	void (*GetProgramDataPath)(CString &strPath);
	void (*GetProgramDataTempPath)(CString &strPath);	

	TPOSStateInterfce()
	{
		ZeroMemory(this,sizeof(TPOSStateInterfce));
	}
};


/*
日志级别
logLevelFatefulError : 严重错误，会导致应用程序崩溃，停止响应的错误，需用户立即处理
logLevelError        : 错误，导致应用程序不正确响应，需通知用户
logLevelWarring      : 警告，需要引起用户注意
logLevelInfo         : 信息，操作日志，运行信息
logLevelDebug        : 调试信息,开发调试用，在发布版本中应关闭
logLevelMemory       : 状态日志，在StatusTimeOut=10,NMLogMonitor检测到超过10秒没有新的状态日志写入，NMLogMonitor将输出最近的状态日志，该功能只要用于再多线程情况下，检测某条线程的运行状态
*/
//以下参数定义顺序不能变  参见 SCLOGGERTEMPLATE.H

enum NMLogError {logLevelInfo,logLevelWarring,logLevelNormalError,logLevelFatefulError,logLevelForced,logLevelOperation,};
enum NMLogLevel {logLevel_0   = 0, logLevel_1,logLevel_2,logLevel_3};
enum NMLogType  {logLevelTrace  = 1, logLevelOperte};
enum NMBeepType {BeepOK,BeepFail,BeepNoOperate,BeepNoSound};

//日志信息
typedef void (*PTP_LogEditNotify)(NMLogError lErrorLevel,NMLogLevel lLogLevel,NMLogType lDebugInfo,LPCTSTR lpLogInfo,LPCTSTR lpModuleName,LPCTSTR lpDllName,LPCTSTR lpFunName);

typedef LPCTSTR (*PTP_LoadString)(LPCTSTR lptszStringID);

typedef BOOL    (*PTP_PopupHelpInfo)(LPCTSTR lptszHelpTitle);
typedef LRESULT (*PTP_DataCenterCallBack)(UINT uCmd,LPARAM lParam);
typedef LRESULT (*PTP_BeepWarningMessage)(NMBeepType lType);
typedef LRESULT (*PTP_ShowTitleMessage)(LPCTSTR lptszTitleMessage,BOOL bStart);  //显示启动信息


//转换回调
typedef struct tag_TPBaseSystemInfo{
	PTP_LoadString          TP_LoadString;
	PTP_LogEditNotify       TP_LogNotify; 
	PTP_PopupHelpInfo       TP_PopupHelpInfo;
	PTP_DataCenterCallBack  TP_DataCenterCallBack;
	PTP_BeepWarningMessage  TP_BeepWarningMessage;
	PTP_ShowTitleMessage    TP_ShowTitleMessage;

	TP_EDITMAX_VERSION      eEditVersion;
	TP_EDITMAX_SUB_VERSION  eSubEditVersion;  //保留
#ifdef TP_NS_NET
    TP_EDITMAX_PROJECT_VERSION eProjectEditVersion;
#endif
	BOOL                    bEasyEdit; 
	long                    lEditRate;
	long                    lCompileRate;
	HINSTANCE				hMainInstance;
	long                    lVideoWidth;
	long                    lVideoHeight;

	LPARAM                  lParam;         //临时使用 2007-06-11 用于各程序中传递临时参数,不能长期占用
	BOOL                    bX1Board;
	TPOSStateInterfce      *pOSStateInterfce;
	ULONGLONG               uSystemState;  
	ULONGLONG	            uPluginLoad;

#ifdef TP_NS_NET
	BOOL					bLocalRenderOREdit;
	BOOL					bGetFloatLicense;//CD2009.10.01
#endif
	tag_TPBaseSystemInfo()
	{
		TP_LoadString         = NULL;
		TP_LogNotify          = NULL;
		TP_PopupHelpInfo      = NULL;
		TP_DataCenterCallBack = NULL;
		TP_BeepWarningMessage = NULL;
		TP_ShowTitleMessage   = NULL;
		eEditVersion          = TP_EDITMAX_E7;
		eSubEditVersion       = TP_CRAFTEDIT_STATION;		
#ifdef TP_NS_NET
        eProjectEditVersion = TP_NULL_PROJECT;
#endif
		bEasyEdit       = FALSE; 
		hMainInstance    = NULL;
		lEditRate        = 1;
		lCompileRate     = 1;
		lVideoWidth      = 720;
		lVideoHeight     = 576;
		lParam           = NULL;
		bX1Board         = FALSE;
		pOSStateInterfce = NULL;
		uSystemState     = 0;
		uPluginLoad      = TP_SYSTEMPLUGINLOAD_UNKNOWN;
#ifdef TP_NS_NET
		bLocalRenderOREdit = FALSE;
		bGetFloatLicense   = FALSE;//CD2009.10.01
#endif
	}

}TPBaseSystemInfo,*LPTPBaseSystemInfo;

#define TP_LogEditNotify(x,a,b,c,z,l) if(x->TP_LogNotify) x->TP_LogNotify(a,b,c,z,l,EDIT_LOGGER_MODULE,(CString)__FUNCTION__)

typedef struct stu_TPRegisterInfo  //文件节点
{
	DWORD dwVersion;
	TCHAR cName[20];
	BOOL  bAVIDEffect;
	DWORD dwRes[10];
	DWORD bRes[10];
	stu_TPRegisterInfo()
	{
		dwVersion   = 1;
		lstrcpyn(cName,_T("RegisterInfo"),20);

		bAVIDEffect = FALSE;
		ZeroMemory(dwRes,sizeof(dwRes));
		ZeroMemory(bRes,sizeof(dwRes));
	}
	void SetComputerInfo(stu_TPRegisterInfo *pSet)
	{

	}
	void SetUserInfo(stu_TPRegisterInfo *pSet)
	{
		bAVIDEffect = pSet->bAVIDEffect;
	}
}TPRegisterInfo;

#define TP_NS_AVI2

#ifdef  TP_NS_NET
#define TP_NS_EDIT
#endif

#ifdef  TP_NS_CX
#undef  TP_NS_AVI2AUDIO
#endif


//////////////////////////////////////////////////////////////////////////
