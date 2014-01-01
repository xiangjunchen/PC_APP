#pragma once

#include "TPCommonCtrlHead.h"

#define IDC_SEPARATE   1500 //快捷键消息范围  1500 - 6500 参考TPImportDllDef.h 不要与其他资源ID重复

#define  WM_RESET_SYSCOLOR    WM_USER + 2212
#define  WM_RESET_SYSFONT     WM_USER + 2213
#define  WM_RESET_SYSFACE     WM_USER + 2214
#define  WM_UNSUBCLASSWINDOW  WM_USER + 2215
#define  WM_MAINMESSAGEBOX    WM_USER + 2223

#define  WM_UDNDELTAPOS       WM_USER + 2216    // WPARAM 1 上 2 下 LPARAM 0 DOWN 1 UP 2 加速
#define  WM_DOCKPARENTWND     WM_USER + 2217

#define  WM_LISTCTRL          WM_USER + 2218
#define  WM_DIRTREECTRL       WM_USER + 2219
#define  WM_BASEFRAME         WM_USER + 2220
#define  WM_DELBASEFRAME      WM_USER + 2221
#define  WM_HELPEX            WM_USER + 2222
#define  WM_COMMBOX           WM_USER + 2223

#define  WM_DELETERES               WM_USER + 2224
#define  WM_TOOLTIPS                WM_USER + 2225
#define  WM_UPDATEVIEW		        WM_USER + 2226    // PE Update View 消息 
#define  WM_BASEFRAMESYSKEY         WM_USER + 2227
#define  WM_FORCEMAINLOCK           WM_USER + 2228
#define  WM_EDITLISTIMP             WM_USER + 2229
#define  WM_BASEFRAMESETRGN         WM_USER + 2230
#define  WM_CLOSEMENU               WM_USER + 2231
#define  WM_AI3IMPORT               WM_USER + 2232
#define  WM_DOMODAL                 WM_USER + 2233
#define  WM_LBITEMSETCHECK          WM_USER + 2234
#define  WM_LISTLBUTTONDBLCLK       WM_USER + 2235
#define  WM_XDCAMDELETERES			WM_USER + 2236
#define  WM_TEAMUSER    			WM_USER + 2237
#define  TPWNDLEVEL1         0x00000001
#define  TPWNDLEVEL2         0x00000002
#define  TPWNDLEVEL3         0x00000004 //特殊
#define  TPSCROLLBAR         10
#define  TP_SYSMENU_HELP     6502

#define  WM_MESSAGEEXCHANGE1		WM_USER + 2241
#define  WM_MESSAGEEXCHANGE9		WM_USER + 2249


AFX_EXT_API void	 TP_SetTaskBarCallBack(LRESULT (*pCallBack)(CWnd* pWnd,UINT uMsg,WPARAM wParam,LPARAM lParam));
//AFX_EXT_API void     TP_SetLogEditNotifyCallBack(void  (*pCallBack)(long ,LPCTSTR ,LPCTSTR,LPCTSTR,LPCTSTR ));
//AFX_EXT_API void     TP_LogEditNotifyCtrl(long lLogLevel,LPCTSTR lpLogInfo,LPCTSTR lpModuleName,LPCTSTR lpDllName,LPCTSTR lpFunName);

AFX_EXT_API CWnd    *TP_CreateCtrlWindow(TPControlData *pControlData);
AFX_EXT_API void     TP_DestroyCtrlWindow(TPControlData *pControlData,CWnd *pWnd);

AFX_EXT_API void     TP_RemoveControl(CWnd *pRemoveWnd,LPVOID pLoadData);

AFX_EXT_API LPVOID   TP_AutoLoadControl(CWnd *pParentWnd,DWORD dwWindowLevel);
AFX_EXT_API void     TP_ReleaseControl(CWnd *pParentWnd,LPVOID pLoadData);
//AFX_EXT_API CWnd    *TP_SubControl(CWnd *pWndSub ,DWORD dwWindowLevel);

AFX_EXT_API LPVOID   TP_AutoCreateControl(CWnd *pParentWnd,TPControlData *pCtrlData,int iCtrlSize,DWORD dwWindowLevel);
AFX_EXT_API void     TP_DestoryControl(CWnd *pParentWnd,LPVOID pLoadData);
AFX_EXT_API CWnd    *TP_CreateControl(CWnd *pParentWnd,TPControlData *pCtrlData,DWORD dwWindowLevel);

AFX_EXT_API void     TP_AutoAttachHotKey(CWnd *pParentWnd,TPHotKeyData *pHotKeyData,int iCount);




AFX_EXT_API CTPRoundCorner  *TP_GetRoundCorner();
AFX_EXT_API TP_BORDER_STYLE  TP_GetBorderStyle();

AFX_EXT_API void             TP_SetExtraSubCtrl(CWnd  *(*fun)(CString,DWORD));
AFX_EXT_API void             TP_SetExtraCreateCtrl(CWnd  *(*fun)(CWnd *,TPControlData *,DWORD));

AFX_EXT_API void             TP_SetExtendSubCtrl(CWnd  *(*fun)(CString,DWORD));
AFX_EXT_API void             TP_SetExtendCreateCtrl(CWnd  *(*fun)(CWnd *,TPControlData *,DWORD));

AFX_EXT_API void             TP_SetMessageBox(int  (WINAPI *fun)(HWND,LPCTSTR ,LPCTSTR ,UINT ));
AFX_EXT_API void             TP_SetMessageBeep(LRESULT (*fun)(long lType));
//AFX_EXT_API LRESULT          TP_WarningMessageBeep(long lType);
//AFX_EXT_API LRESULT          TP_WarningMessageBeep(long lType);

AFX_EXT_API BOOL             TP_GetIsVista();
AFX_EXT_API int              TP_GetOSVersion();
AFX_EXT_API DWORD            TP_GetRunVersion();
AFX_EXT_API BOOL             TP_GetDWMStatus(BOOL bForce = FALSE);
AFX_EXT_API BOOL             TP_SetDWMStatus(BOOL bSet);
AFX_EXT_API void             TP_DwmDisableWnd(HWND hWnd);



AFX_EXT_API	void             TP_InitCommonCtrl();
AFX_EXT_API	void             TP_ReleaseCommonCtrl();

AFX_EXT_API	LPCTSTR          TP_GetBinPath(BOOL bForce = FALSE);
AFX_EXT_API	LPCTSTR          TP_GetSysPath();
AFX_EXT_API	LPCTSTR          TP_GetDllPath();
AFX_EXT_API	LPCTSTR          TP_GetDictionaryPath();
AFX_EXT_API	LPCTSTR          TP_GetTempPath();
AFX_EXT_API	void             TP_SetTempPath(LPCTSTR sPath);
AFX_EXT_API	void             TP_SetSysVideoType(VIDEO_TYPE eVideoType);
AFX_EXT_API	DWORD            TP_GetVideoStandard1609(DWORD dVideoStand,BOOL bTo1609);

AFX_EXT_API	void             TP_InitMainWnd();
AFX_EXT_API	void             TP_EnableMainWnd(BOOL bEnable);
AFX_EXT_API	CWnd            *TP_GetMainWnd();
AFX_EXT_API	CString          TP_GetRegisterClass();
AFX_EXT_API	int              AfxMessageBoxEx (LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);
AFX_EXT_API	int              TP_SetGlobalHotKey(LPTPNSControlData pGlobalHotKey,long lGlobalHotKey,long   lGlobalHotKeyHelp);
AFX_EXT_API	BOOL             TP_IsParentWnd(HWND hParent,HWND hChild);
AFX_EXT_API void             TP_SetUITemplateState(HWND hWnd,UINT uState);

AFX_EXT_API LRESULT             TP_SetCtrlPanelDataBase(DWORD dwType,void *pData,DWORD dwDataType);
AFX_EXT_API void                TP_SetCtrlPanelDataBasePtr(LRESULT (*fun)(DWORD ,void *,DWORD ));
AFX_EXT_API CRect               TP_GetSystemMetricsRect();//取得系统窗口区域
AFX_EXT_API void                TP_CenterWnd(CWnd *pParent,CWnd *pThis, CRect* prtLimit, UINT uType);

AFX_EXT_API TPOSCtrlInterfce   *TP_GetOSStateInterfce();
AFX_EXT_API void                TP_SetOSStateInterfce(TPOSCtrlInterfce *pInterface);
