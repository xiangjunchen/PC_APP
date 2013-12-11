//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 优化库的一些公用定义、公用接口等.
//
// Created: ZhaoZhimeng @ 2008-11-16
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_CPU_BASE_H__
#define __NX_CPU_BASE_H__

#include <tchar.h>
#include "..\SysInclude\NXLoader.h"

// 常量定义
#define NXCPU_ENV_DEBUG			_T("CPUDebug")
#define NXCPU_ENV_RELEASE		_T("CPURelease")

// 加载器相对路径类
class CNXCPUDLLPos
{
public:
	CNXCPUDLLPos()
	{
#ifdef _DEBUG
		static LPCTSTR s_pszEnvRefDebug[] = { NXCPU_ENV_DEBUG };
		_EnvRefDebug_ptr = s_pszEnvRefDebug;
		_EnvDebugCount = sizeof(s_pszEnvRefDebug) / sizeof(LPCTSTR);;
#else
		_EnvRefDebug_ptr = NULL;
		_EnvDebugCount = 0;
#endif
		static LPCTSTR s_pszEnvRefRelease[] = { NXCPU_ENV_RELEASE };
		_EnvRefRelease_ptr = s_pszEnvRefRelease;
		_EnvReleaseCount = sizeof(s_pszEnvRefRelease) / sizeof(LPCTSTR);
	}

	LPCTSTR* EnvRefDebug() const { return _EnvRefDebug_ptr; }
	int EnvDebugCount() const { return _EnvDebugCount; }
	LPCTSTR* EnvRefRelease() const { return _EnvRefRelease_ptr; }
	int EnvReleaseCount() const { return _EnvReleaseCount; }

private:
	LPCTSTR		*_EnvRefDebug_ptr;
	int			_EnvDebugCount;
	LPCTSTR		*_EnvRefRelease_ptr;
	int			_EnvReleaseCount;
};

// 返回库句柄，并同时输出库中的接口结构指针
// （1）返回的库句柄要求调用者最后使用 NXDLL_Unload 或 ::FreeLibarary 接口释放，
// 功能组接口指针不需要也不能被释放，否则可能会引起程序崩溃。
// （2）具体细节请参阅 NXLoader.h 文件中的 NXDLL_Load 函数。
inline HMODULE NXCPU_LoadDLL(LPCTSTR szCoreDLLParentPath,	// CoreDLL 目录父路径
							 LPCTSTR szDLLName,				// 加载库的名字
							 LPCSTR szAPIExtName,			// 加载库导出接口的名字
							 void* *ppStuAPI)				// 输出接口结构指针
{
	CNXCPUDLLPos _Pval;

	return NXDLL_Load(szCoreDLLParentPath, _Pval.EnvRefDebug(), _Pval.EnvDebugCount(), \
		_Pval.EnvRefRelease(), _Pval.EnvReleaseCount(), szDLLName, szAPIExtName, ppStuAPI);
}

#endif		// #ifndef __NX_CPU_BASE_H__
//////////////////////////////////////////////////////////////////////////
// End of file.