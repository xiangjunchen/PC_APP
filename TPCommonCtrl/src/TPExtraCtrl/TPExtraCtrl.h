//MessageRange 
//IDRange     
#pragma once

#include "..\TPCommonCtrl\TPCommonCtrl.h"

#include "TPCommonInclude.h"

namespace _tp_extractrl
{
	#include "TPExtraCtrlExp.h"
};
	    
#ifndef	TPEXTRACTRL_LIB_
#define TPEXTRACTRL_LIB_

#ifdef _DEBUG
#ifdef _UNICODE
#pragma comment(lib, "TPExtraCtrlUD.lib")
#else
#pragma comment(lib, "TPExtraCtrlD.lib")
#endif	//_UNICODE
#else
#ifdef _UNICODE
#pragma comment(lib, "TPExtraCtrlU.lib")
#else
#pragma comment(lib, "TPExtraCtrl.lib")
#endif	//_UNICODE
#endif
#endif




