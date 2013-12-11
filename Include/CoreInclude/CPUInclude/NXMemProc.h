//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ���õ��ڴ洦��ӿڶ��壬�翽������ת������.
//
// Created: ZhaoZhimeng @ 2008-11-16
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_MEM_PROC_H__
#define __NX_MEM_PROC_H__

#include "NXMemProcDef.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ��ʽ���ع��ܿ⺯��

#include "NXCPUBase.h"

// ���ؿ�������ͬʱ������еĽӿڽṹָ��
// ��1�����صĿ���Ҫ����������ʹ�� NXDLL_Unload �� ::FreeLibarary �ӿ��ͷţ�
// ������ӿ�ָ�벻��ҪҲ���ܱ��ͷţ�������ܻ�������������
// ��2������ϸ������� NXLoader.h �ļ��е� NXDLL_Load ������
inline HMODULE NXMemProc_LoadDLL(IN LPCTSTR szCoreDLLParentPath,	// CoreDLL Ŀ¼�ĸ�·��
								 OUT LPNXMEMPROCAPI *ppStuAPI)		// ����ӿڽṹָ��
{
	return NXCPU_LoadDLL(szCoreDLLParentPath, NXMEMPROC_DLL_NAME, NXMEMPROC_API_EXT_NAME, (void**)ppStuAPI);
}

// ���������·����
class CNXMemProcDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXMEMPROC_DLL_NAME; }
	LPCSTR APIExtName() const { return NXMEMPROC_API_EXT_NAME; }
};

// ȫ�ֵ���������
// ��1������ϸ������� NXLoader.h �ļ���
__nxselectany CNXLoader<NXMemProcAPI, CNXMemProcDLLPos> g_NXMemProc;

//////////////////////////////////////////////////////////////////////////
// ���ú����궨��

#define nx_memset		g_NXMemProc->pStuMemFillAPI->pfnMemset
#define nx_memcpy		g_NXMemProc->pStuMemPickAPI->pfnMemcpy


#endif		// #ifndef __NX_MEM_PROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.