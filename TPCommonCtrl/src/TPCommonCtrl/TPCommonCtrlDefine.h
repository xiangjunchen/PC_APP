#pragma  once



extern HINSTANCE       g_hThisInstance;
extern HCURSOR         g_hArrowCursor;
extern CTPRoundCorner *g_pRoundCorner;
extern TP_BORDER_STYLE g_eBorderStyle; 
extern CString         g_sSysSkinPath;
extern CString         g_sTemp;
extern VIDEO_TYPE      g_eSysVideoType;
extern   CTPRoundCorner g_stuRoundCorner;
extern   CString        g_sRegisterClass;

extern LRESULT  (*g_pTaskBarCallBackFun)(CWnd* pWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
extern void     (*TP_LogEditNotify)(long lLogLevel,LPCTSTR lpLogInfo,LPCTSTR lpModuleName,LPCTSTR lpDllName,LPCTSTR lpFunName);
extern void     TP_InitMenuHook(BOOL bInit);
