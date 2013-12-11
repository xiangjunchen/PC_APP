//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 常用类型及返回值定义
//
// Author: LaiShoubo @ 2011-08-19
//////////////////////////////////////////////////////////////////////////
#ifndef __NXBASIC_H__
#define __NXBASIC_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#include <crtdefs.h>
#else	// VC6
#include <stddef.h>		// for intptr_t
#ifndef _INTPTR_T_DEFINED
typedef ptrdiff_t		intptr_t;
#define _INTPTR_T_DEFINED
#endif
#endif		// #if defined(_MSC_VER) && (_MSC_VER >= 1300)

//////////////////////////////////////////////////////////////////////////
// 形式参数修饰符
#ifndef __in
#define __in
#endif

#ifndef __out
#define __out
#endif

#ifndef __inout
#define __inout
#endif

//////////////////////////////////////////////////////////////////////////
#ifndef NOVTABLE
#define NOVTABLE __declspec(novtable)
#endif

#ifndef interface
#define interface struct
#endif

//////////////////////////////////////////////////////////////////////////
// 返回值信息定义
enum NXRESULT
{
	NXRESULT_UNKNOWN = 0x80000000,			// 未知错误
	NXRESULT_INVALID_PARAM,					// 无效参数
	NXRESULT_OUT_OF_MEMORY,					// 内存不足
	NXRESULT_NOT_SUPPORTED,					// 不支持
	NXRESULT_CACHE_FULL,					// 缓冲满
	NXRESULT_CACHE_EMPTY,					// 缓冲空
	NXRESULT_INVALID_POINTER,				// 无效的指针
	NXRESULT_INITIALIZED,					// 已初始化
	NXRESULT_FAILED,						// 失败
	NXRESULT_OUT_OF_RANGE,					// 越界
	NXRESULT_OK = 0,						// 成功
};

//////////////////////////////////////////////////////////////////////////
#endif // __NXBASIC_H__
