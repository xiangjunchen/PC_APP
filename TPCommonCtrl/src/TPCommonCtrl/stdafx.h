// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////后续加的lib
#include "afxpriv.h"
#include "Wingdi.h"
#pragma comment(lib, "Msimg32.lib")
#include <AfxTempl.h>

#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

#include "dwmapi.h"
#include "winuser.h"
#pragma comment(lib, "dwmapi.lib")

#include "shellapi.h"

#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Rpcrt4.lib")
#include <afxdlgs.h>
//////////////////////////////////////////////////////////////////////////
#define  _L _T
#define EDIT_LOGGER_MODULE _T("CommonCtrl")

#include "TPCommonInclude.h"

#include "TPExportDllDef.h"
#include "Resource.h"

#define  TPCOMMONCTRL_LIB_
#include "TPCommonCtrl.h"
using namespace _tp_commonctrl;

#include "TPCommonCtrlDefine.h"
#include <afxhtml.h>



