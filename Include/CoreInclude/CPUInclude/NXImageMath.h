//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ���õ�ͼ����ѧ����ӿڶ��壬���ݶȡ�ɢ�ȡ���������.
//
// Created: ZhaoZhimeng @ 2010-05-17
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_IMAGE_MATH_H__
#define __NX_IMAGE_MATH_H__

#include "NXImageMathDef.h"

//////////////////////////////////////////////////////////////////////////
// �Զ�������
#include "NXCPUBase.h"

// ���������·����
class CNXImageMathDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXIMAGEMATH_DLL_NAME; }
	LPCSTR APIExtName() const { return NXIMAGEMATH_API_EXT_NAME; }
};

// ȫ�ֵ���������
// ��1������ϸ������� NXLoader.h �ļ���
__nxselectany CNXLoader<NXImageMathAPI, CNXImageMathDLLPos> g_NXImageMath;


#endif		// #ifndef __NX_IMAGE_MATH_H__
//////////////////////////////////////////////////////////////////////////
// End of file.