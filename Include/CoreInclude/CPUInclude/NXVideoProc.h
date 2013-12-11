//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ͼ������Ƶ����ӿڣ�����ɫ�����źű任���źŻ����.
//
// Created: ZhaoZhimeng @ 2008-11-19
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_VIDEO_PROC_H__
#define __NX_VIDEO_PROC_H__

#include "NXVideoProcDef.h"
#include "INXVideoFmtConv.h"
#include "INXVideoSpeedProc.h"
#include "INXVideoSpeedFunc.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ��ʽ���ع��ܿ⺯��

#include "NXCPUBase.h"

// ���ؿ�������ͬʱ������еĽӿڽṹָ��
// ��1�����صĿ���Ҫ����������ʹ�� NXDLL_Unload �� ::FreeLibarary �ӿ��ͷţ�
// ������ӿ�ָ�벻��ҪҲ���ܱ��ͷţ�������ܻ�������������
// ��2������ϸ������� NXLoader.h �ļ��е� NXDLL_Load ������
inline HMODULE NXVideoProc_LoadDLL(IN LPCTSTR szCoreDLLParentPath,	// CoreDLL Ŀ¼�ĸ�·��
								   OUT LPNXVIDEOPROCAPI *ppStuAPI)	// ����ӿڽṹָ��
{
	return NXCPU_LoadDLL(szCoreDLLParentPath, NXVIDEOPROC_DLL_NAME, NXVIDEOPROC_API_EXT_NAME, (void**)ppStuAPI);
}

// ���������·����
class CNXVideoProcDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXVIDEOPROC_DLL_NAME; }
	LPCSTR APIExtName() const { return NXVIDEOPROC_API_EXT_NAME; }
};

// ȫ�ֵ���������
// ��1������ϸ������� NXLoader.h �ļ���
__nxselectany CNXLoader<NXVideoProcAPI, CNXVideoProcDLLPos> g_NXVideoProc;


#endif		// #ifndef __NX_VIDEO_PROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.