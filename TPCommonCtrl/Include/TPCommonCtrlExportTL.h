#pragma once

#undef  WINVER
#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
#define WINVER 0x0500		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#endif

#undef  _WIN32_WINNT
#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#endif						

#undef  _WIN32_WINDOWS
#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0510 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#endif

#undef  _WIN32_IE
#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。
#define _WIN32_IE 0x0500	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
#endif


#include "..\Src\TPCommonCtrl\TPCommonCtrl.h"
using namespace _tp_commonctrl;
#include "..\Src\TPCommonClass\TPCommonClass.h"
using namespace _tp_commonclass;
#include "..\Src\TPExtraCtrl\TPExtraCtrl.h"
using namespace _tp_extractrl;
#include "..\Src\TPCommonWnd\TPCommonWnd.h"
using namespace _tp_commonwnd;
