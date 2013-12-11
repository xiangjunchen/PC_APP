//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��ɫ�ռ�ת��.
//
// Created: ZhaoZhimeng @ 2010-05-17
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_COLOR_SPACE_H__
#define __NX_COLOR_SPACE_H__

#include "NXColorSpaceDef.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ��ʽ���ع��ܿ⺯��

#include "NXCPUBase.h"

// ���ؿ�������ͬʱ������еĽӿڽṹָ��
// ��1�����صĿ���Ҫ����������ʹ�� NXDLL_Unload �� ::FreeLibarary �ӿ��ͷţ�
// ������ӿ�ָ�벻��ҪҲ���ܱ��ͷţ�������ܻ�������������
// ��2������ϸ������� NXLoader.h �ļ��е� NXDLL_Load ������
inline HMODULE NXColorSpace_LoadDLL(IN LPCTSTR szCoreDLLParentPath,		// CoreDLL Ŀ¼�ĸ�·��
									OUT LPNXCOLORSPACEAPI *ppStuAPI)	// ����ӿڽṹָ��
{
	return NXCPU_LoadDLL(szCoreDLLParentPath, NXCOLORSPACE_DLL_NAME, NXCOLORSPACE_API_EXT_NAME, (void**)ppStuAPI);
}

// ���������·����
class CNXColorSpaceDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXCOLORSPACE_DLL_NAME; }
	LPCSTR APIExtName() const { return NXCOLORSPACE_API_EXT_NAME; }
};

// ȫ�ֵ���������
// ��1������ϸ������� NXLoader.h �ļ���
__nxselectany CNXLoader<NXColorSpaceAPI, CNXColorSpaceDLLPos> g_NXColorSpace;


#endif		// #ifndef __NX_COLOR_SPACE_H__
//////////////////////////////////////////////////////////////////////////
// End of file.