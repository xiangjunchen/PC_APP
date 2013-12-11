//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ��Ч�㷨�ӿڣ��絭�뵭���������������ס����Ż����.
//
// Created: ZhaoZhimeng @ 2009-06-09
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_VIDEO_EFFECT_PROC_H__
#define __NX_VIDEO_EFFECT_PROC_H__

#include "NXVideoEffectProcDef.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ��ʽ���ع��ܿ⺯��

#include "NXCPUBase.h"

// ���ؿ�������ͬʱ������еĽӿڽṹָ��
// ��1�����صĿ���Ҫ����������ʹ�� NXDLL_Unload �� ::FreeLibarary �ӿ��ͷţ�
// ������ӿ�ָ�벻��ҪҲ���ܱ��ͷţ�������ܻ�������������
// ��2������ϸ������� NXLoader.h �ļ��е� NXDLL_Load ������
inline HMODULE NXVideoEffectProc_LoadDLL(IN LPCTSTR szCoreDLLParentPath,	// CoreDLL Ŀ¼�ĸ�·��
										 OUT LPNXVIDEOEFFECTPROCAPI *ppStuAPI)		// ����ӿڽṹָ��
{
	return NXCPU_LoadDLL(szCoreDLLParentPath, NXVIDEOEFFECTPROC_DLL_NAME, NXVIDEOEFFECTPROC_API_EXT_NAME, (void**)ppStuAPI);
}

// ���������·����
class CNXVideoEffectProcDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXVIDEOEFFECTPROC_DLL_NAME; }
	LPCSTR APIExtName() const { return NXVIDEOEFFECTPROC_API_EXT_NAME; }
};

// ȫ�ֵ���������
// ��1������ϸ������� NXLoader.h �ļ���
__nxselectany CNXLoader<NXVideoEffectProcAPI, CNXVideoEffectProcDLLPos> g_NXVideoEffectProc;


#endif		// #ifndef __NX_VIDEO_EFFECT_PROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.