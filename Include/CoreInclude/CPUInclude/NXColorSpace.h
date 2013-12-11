//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 颜色空间转换.
//
// Created: ZhaoZhimeng @ 2010-05-17
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_COLOR_SPACE_H__
#define __NX_COLOR_SPACE_H__

#include "NXColorSpaceDef.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 显式加载功能库函数

#include "NXCPUBase.h"

// 返回库句柄，并同时输出库中的接口结构指针
// （1）返回的库句柄要求调用者最后使用 NXDLL_Unload 或 ::FreeLibarary 接口释放，
// 功能组接口指针不需要也不能被释放，否则可能会引起程序崩溃。
// （2）具体细节请参阅 NXLoader.h 文件中的 NXDLL_Load 函数。
inline HMODULE NXColorSpace_LoadDLL(IN LPCTSTR szCoreDLLParentPath,		// CoreDLL 目录的父路径
									OUT LPNXCOLORSPACEAPI *ppStuAPI)	// 输出接口结构指针
{
	return NXCPU_LoadDLL(szCoreDLLParentPath, NXCOLORSPACE_DLL_NAME, NXCOLORSPACE_API_EXT_NAME, (void**)ppStuAPI);
}

// 加载器相对路径类
class CNXColorSpaceDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXCOLORSPACE_DLL_NAME; }
	LPCSTR APIExtName() const { return NXCOLORSPACE_API_EXT_NAME; }
};

// 全局单件加载器
// （1）具体细节请参阅 NXLoader.h 文件。
__nxselectany CNXLoader<NXColorSpaceAPI, CNXColorSpaceDLLPos> g_NXColorSpace;


#endif		// #ifndef __NX_COLOR_SPACE_H__
//////////////////////////////////////////////////////////////////////////
// End of file.