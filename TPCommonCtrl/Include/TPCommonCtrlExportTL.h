#pragma once

#undef  WINVER
#ifndef WINVER				// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define WINVER 0x0500		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif

#undef  _WIN32_WINNT
#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif						

#undef  _WIN32_WINDOWS
#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0510 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#endif

#undef  _WIN32_IE
#ifndef _WIN32_IE			// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0500	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#endif


#include "..\Src\TPCommonCtrl\TPCommonCtrl.h"
using namespace _tp_commonctrl;
#include "..\Src\TPCommonClass\TPCommonClass.h"
using namespace _tp_commonclass;
#include "..\Src\TPExtraCtrl\TPExtraCtrl.h"
using namespace _tp_extractrl;
#include "..\Src\TPCommonWnd\TPCommonWnd.h"
using namespace _tp_commonwnd;
