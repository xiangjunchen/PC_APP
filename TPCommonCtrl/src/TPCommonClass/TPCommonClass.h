// TPCommonClass.h : main header file for the TPCommonClass DLL
//

#pragma once

#include <dsound.h>
#pragma comment(lib, "dsound.lib")
#include <afxmt.h>
#include <ddraw.h>
#pragma comment(lib, "ddraw.lib")
#include <streams.h>  //dx”√


#include "TPDBApi.h"


#include "TPCommonClassStruct.h"

//InternetGetConnectedState π”√
#include <Wininet.h>
#include <afxpriv.h>

#include   <Winnetwk.h> 
#pragma   comment(lib, "Mpr.lib") 
//end

#include "TPBaseNetInterface.h"
#include "TPUdpSocket.h"
#include "TPTcpSocket.h"
#include "TPNetInterface.h"

namespace _tp_commonclass
{
#include "TPCommonClassExp.h"
};

#ifndef	TPCOMMONCLASS_LIB_
#define TPCOMMONCLASS_LIB_

#ifdef _DEBUG
#ifdef _UNICODE
#pragma comment(lib, "TPCommonClassUD.lib")
#else
#pragma comment(lib, "TPCommonClassD.lib")
#endif	//_UNICODE
#else
#ifdef _UNICODE
#pragma comment(lib, "TPCommonClassU.lib")
#else
#pragma comment(lib, "TPCommonClass.lib")
#endif	//_UNICODE
#endif
#endif
