//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation. All rights reserved.
//
// Desc:	the framework of memory pool, included by other files.
//
// Author:	LaiShoubo @ 2010-07-19 Beijing
//////////////////////////////////////////////////////////////////////////
#ifndef _INC_NXMEMORYPOOL_
#define _INC_NXMEMORYPOOL_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#include <crtdefs.h>
#else	// VC6
#include <stddef.h>		// for size_t
#endif		// #if defined(_MSC_VER) && (_MSC_VER >= 1300)

//////////////////////////////////////////////////////////////////////////
// 编译声明

#ifndef interface
#define interface	struct
#endif		// #ifndef interface

// namespace
#define _NXMP_BEGIN		namespace nxmp {
#define _NXMP_END		}

#ifndef NOVTABLE
#define NOVTABLE __declspec(novtable)
#endif


//////////////////////////////////////////////////////////////////////////
// 库定义

#define NXMEMORYPOOL_DLL_NAME			_T("NXMemoryPool")
#define NXMEMORYPOOL_API_EXT_NAME		"NX_GetMemoryPool"


//////////////////////////////////////////////////////////////////////////
// 导出的接口抽象类
interface NOVTABLE INXMemoryPool
{
	virtual void* AllocateOne(size_t _Size) = 0;
	virtual void* AllocateArray(size_t _Size) = 0;
	virtual void DeallocateOne(void* _Ptr, size_t _Size) throw() = 0;
	virtual void DeallocateArray(void* _Ptr) throw() = 0;
	// 用于 DEBUG 版本中定位调用者来源
	virtual void* AllocateOne(size_t _Size, char const* _File, int _Line) = 0;
	virtual void* AllocateArray(size_t _Size, char const* _File, int _Line) = 0;
};


#endif // _INC_NXMEMORYPOOL_
//////////////////////////////////////////////////////////////////////////
// End of file.
