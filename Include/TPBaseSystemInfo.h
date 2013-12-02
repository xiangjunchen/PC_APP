#pragma	  once

//#include "TPBaseVADef.h"
#pragma warning(disable:4244)

enum	TP_EDITMAX_VERSION	// �Ǳ�汾
{
	TP_EDITMAX_E7 = 0,		
	TP_EDITMAX_XPRINEWS,   		
	TP_EDITMAX_SOBEYNEWS,
};

enum	TP_EDITMAX_SUB_VERSION	// �Ǳ�汾
{
	TP_NULL_STATION  = 0,
	TP_CRAFTEDIT_STATION,		
	TP_PROXYEDIT_STATION,   		
};
//ע�����¶�����commonctrl�����ظ����壬����޸ģ���ͬ��
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
��־����
logLevelFatefulError : ���ش��󣬻ᵼ��Ӧ�ó��������ֹͣ��Ӧ�Ĵ������û���������
logLevelError        : ���󣬵���Ӧ�ó�����ȷ��Ӧ����֪ͨ�û�
logLevelWarring      : ���棬��Ҫ�����û�ע��
logLevelInfo         : ��Ϣ��������־��������Ϣ
logLevelDebug        : ������Ϣ,���������ã��ڷ����汾��Ӧ�ر�
logLevelMemory       : ״̬��־����StatusTimeOut=10,NMLogMonitor��⵽����10��û���µ�״̬��־д�룬NMLogMonitor����������״̬��־���ù���ֻҪ�����ٶ��߳�����£����ĳ���̵߳�����״̬
*/
//���²�������˳���ܱ�  �μ� SCLOGGERTEMPLATE.H

enum NMLogError {logLevelInfo,logLevelWarring,logLevelNormalError,logLevelFatefulError,logLevelForced,logLevelOperation,};
enum NMLogLevel {logLevel_0   = 0, logLevel_1,logLevel_2,logLevel_3};
enum NMLogType  {logLevelTrace  = 1, logLevelOperte};
enum NMBeepType {BeepOK,BeepFail,BeepNoOperate,BeepNoSound};

//��־��Ϣ
typedef void (*PTP_LogEditNotify)(NMLogError lErrorLevel,NMLogLevel lLogLevel,NMLogType lDebugInfo,LPCTSTR lpLogInfo,LPCTSTR lpModuleName,LPCTSTR lpDllName,LPCTSTR lpFunName);

typedef LPCTSTR (*PTP_LoadString)(LPCTSTR lptszStringID);

typedef BOOL    (*PTP_PopupHelpInfo)(LPCTSTR lptszHelpTitle);
typedef LRESULT (*PTP_DataCenterCallBack)(UINT uCmd,LPARAM lParam);
typedef LRESULT (*PTP_BeepWarningMessage)(NMBeepType lType);
typedef LRESULT (*PTP_ShowTitleMessage)(LPCTSTR lptszTitleMessage,BOOL bStart);  //��ʾ������Ϣ


//ת���ص�
typedef struct tag_TPBaseSystemInfo{
	PTP_LoadString          TP_LoadString;
	PTP_LogEditNotify       TP_LogNotify; 
	PTP_PopupHelpInfo       TP_PopupHelpInfo;
	PTP_DataCenterCallBack  TP_DataCenterCallBack;
	PTP_BeepWarningMessage  TP_BeepWarningMessage;
	PTP_ShowTitleMessage    TP_ShowTitleMessage;

	TP_EDITMAX_VERSION      eEditVersion;
	TP_EDITMAX_SUB_VERSION  eSubEditVersion;  //����
#ifdef TP_NS_NET
    TP_EDITMAX_PROJECT_VERSION eProjectEditVersion;
#endif
	BOOL                    bEasyEdit; 
	long                    lEditRate;
	long                    lCompileRate;
	HINSTANCE				hMainInstance;
	long                    lVideoWidth;
	long                    lVideoHeight;

	LPARAM                  lParam;         //��ʱʹ�� 2007-06-11 ���ڸ������д�����ʱ����,���ܳ���ռ��
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

typedef struct stu_TPRegisterInfo  //�ļ��ڵ�
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
