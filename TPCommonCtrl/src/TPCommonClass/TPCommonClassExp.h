#pragma once

#include <dsound.h>
#pragma comment(lib, "dsound.lib")
#include <afxmt.h>
#include <ddraw.h>
#pragma comment(lib, "ddraw.lib")


#include <afxmt.h>

#include "TPCommonClassHead.h"

AFX_EXT_API     void             TP_InitClass();
AFX_EXT_API     void             TP_ReleaseClass();
AFX_EXT_API     CTPNetInterface  *TP_GetNetInterface();

// AFX_EXT_API     CTPGetIcon      *TP_GetIconFun();

AFX_EXT_API    BOOL         TP_GetState(UINT uTye);
AFX_EXT_API    void         TP_SetState(UINT uTye,BOOL bSet);
