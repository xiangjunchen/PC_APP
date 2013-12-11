//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 3D ��ѧ���㣬��������������Ԫ����.
//
// Created: ZhaoZhimeng @ 2010-07-27
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_3D_MATH_H__
#define __NX_3D_MATH_H__

#include "NX3DMathDef.h"

//////////////////////////////////////////////////////////////////////////
// �Զ�������
#include "NXCPUBase.h"

// ���������·����
class CNX3DMathDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NX3DMATH_DLL_NAME; }
	LPCSTR APIExtName() const { return NX3DMATH_API_EXT_NAME; }
};

// ȫ�ֵ���������
// ��1������ϸ������� NXLoader.h �ļ���
__nxselectany CNXLoader<NX3DMathAPI, CNX3DMathDLLPos> g_NX3DMath;


#endif		// #ifndef __NX_3D_MATH_H__
//////////////////////////////////////////////////////////////////////////
// End of file.