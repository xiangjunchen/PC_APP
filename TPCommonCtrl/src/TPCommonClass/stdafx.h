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

#undef   _L
#define  _L _T

#include <AfxTempl.h>
#include <Math.h>
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#include "Imagehlp.h"
#pragma comment(lib,"Imagehlp.lib")
#include "psapi.h"
#pragma comment(lib,"psapi.lib")
#include "Winsock2.h"
#include "Winsock.h"
#pragma comment(lib, "Ws2_32.lib")

//#include <streams.h>  //dx”√

#include "TPImportDllDef.h"


#include "..\TPCommonCtrl\TPCommonCtrl.h"
using namespace _tp_commonctrl;


#include "TPExportDllDef.h"



#define   TPCOMMONCLASS_LIB_
#define   AFX_EXEX_CLASS AFX_CLASS_EXPORT
#include "TPCommonClass.h"
using namespace _tp_commonclass;

#include "TPCompressEx.h"
