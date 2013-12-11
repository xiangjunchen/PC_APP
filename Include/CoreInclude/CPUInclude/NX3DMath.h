//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 3D 数学运算，如向量、矩阵、四元数等.
//
// Created: ZhaoZhimeng @ 2010-07-27
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_3D_MATH_H__
#define __NX_3D_MATH_H__

#include "NX3DMathDef.h"

//////////////////////////////////////////////////////////////////////////
// 自动加载器
#include "NXCPUBase.h"

// 加载器相对路径类
class CNX3DMathDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NX3DMATH_DLL_NAME; }
	LPCSTR APIExtName() const { return NX3DMATH_API_EXT_NAME; }
};

// 全局单件加载器
// （1）具体细节请参阅 NXLoader.h 文件。
__nxselectany CNXLoader<NX3DMathAPI, CNX3DMathDLLPos> g_NX3DMath;


#endif		// #ifndef __NX_3D_MATH_H__
//////////////////////////////////////////////////////////////////////////
// End of file.