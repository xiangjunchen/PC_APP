// TPCommonCtrl.h : main header file for the TPCommonCtrl DLL
//

#pragma once

#include <AfxTempl.h>
#include <mmsystem.h>    //for WAVEFORMATEX
#include <afxole.h>
#include <afxmt.h>

#include "TPCommonInclude.h"

#include <afxhtml.h>

namespace _tp_commonctrl
{
	#include "TPCommonCtrlExp.h"	
};

#ifndef	TPCOMMONCTRL_LIB_
#define TPCOMMONCTRL_LIB_

#ifdef _DEBUG
#ifdef _UNICODE
#pragma comment(lib, "TPCommonCtrlUD.lib")
#else
#pragma comment(lib, "TPCommonCtrlD.lib")
#endif	//_UNICODE
#else
#ifdef _UNICODE
#pragma comment(lib, "TPCommonCtrlU.lib")
#else
#pragma comment(lib, "TPCommonCtrl.lib")
#endif	//_UNICODE
#endif
#endif
