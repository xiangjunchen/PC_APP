// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
#define WINVER 0x0501		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将为显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include "TPBaseDef.h"
#include "TPBufferOperateDef.h"

#if defined ( __w64 )		// 64 位与 32 位平台操作指针的寄存器不同

#define EAX_PTR			RAX
#define EBX_PTR			RBX
#define ECX_PTR			RCX
#define EDX_PTR			RDX
#define EDI_PTR			RDI
#define ESI_PTR			RSI
#define PTR
#define ptrword			qword		// 访问指针的指针的内存类型声明
#define PTR_BYTES_1		0x08		// 指针数据占用的内存大小
#define __USE_64_INS__		// 在特殊情况下必须使用 64 位指令

#else		// #if defined ( __w64 )

#define EAX_PTR			EAX
#define EBX_PTR			EBX
#define ECX_PTR			ECX
#define EDX_PTR			EDX
#define EDI_PTR			EDI
#define ESI_PTR			ESI
#define ptrword			dword		// 访问指针的指针的内存类型声明
#define PTR_BYTES_1		0x04		// 指针数据占用的内存大小

#endif		// #if defined ( __w64 )

#define eax_ptr			EAX_PTR
#define ebx_ptr			EBX_PTR
#define ecx_ptr			ECX_PTR
#define edx_ptr			EDX_PTR
#define edi_ptr			EDI_PTR
#define esi_ptr			ESI_PTR

#define _buffer_operate_export
#define __ADAPT_CPU_SDK__
