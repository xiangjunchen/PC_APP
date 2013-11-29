#pragma once


/*
#ifdef  TPCOMMONCTRL_NOLIB_
#define TPCOMMONCTRL_NOLIB_

#define TPCOMMONCTRL_LIB_
#define TPEXTRACTRL_LIB_
#define TPCOMMONWND_LIB_
#define TPEXTENDCTRL_LIB_

#endif
*/


#include "..\Src\TPCommonCtrl\TPCommonCtrl.h"
using namespace _tp_commonctrl;
#include "..\Src\TPCommonClass\TPCommonClass.h"
using namespace _tp_commonclass;
#include "..\Src\TPExtraCtrl\TPExtraCtrl.h"
using namespace _tp_extractrl;
//#include "..\Src\TPCommonWnd\TPCommonWnd.h"
//using namespace _tp_commonwnd;
//#include "..\Src\TPExtendCtrl\TPExtendCtrl.h"
//#include "..\Src\TPNewsCtrl\TPNewsCtrlExp.h"

#ifdef TP_USESIMPLEPLAY
#include "..\Src\E7VASimplePlay\E7VAPlayExp.h"
#endif



