//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 常用的图像数学计算接口定义，如梯度、散度、方程求解等.
//
// Created: ZhaoZhimeng @ 2010-05-17
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_IMAGE_MATH_H__
#define __NX_IMAGE_MATH_H__

#include "NXImageMathDef.h"

//////////////////////////////////////////////////////////////////////////
// 自动加载器
#include "NXCPUBase.h"

// 加载器相对路径类
class CNXImageMathDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXIMAGEMATH_DLL_NAME; }
	LPCSTR APIExtName() const { return NXIMAGEMATH_API_EXT_NAME; }
};

// 全局单件加载器
// （1）具体细节请参阅 NXLoader.h 文件。
__nxselectany CNXLoader<NXImageMathAPI, CNXImageMathDLLPos> g_NXImageMath;


#endif		// #ifndef __NX_IMAGE_MATH_H__
//////////////////////////////////////////////////////////////////////////
// End of file.