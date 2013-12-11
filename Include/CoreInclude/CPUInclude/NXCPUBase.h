//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: �Ż����һЩ���ö��塢���ýӿڵ�.
//
// Created: ZhaoZhimeng @ 2008-11-16
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_CPU_BASE_H__
#define __NX_CPU_BASE_H__

#include <tchar.h>
#include "..\SysInclude\NXLoader.h"

// ��������
#define NXCPU_ENV_DEBUG			_T("CPUDebug")
#define NXCPU_ENV_RELEASE		_T("CPURelease")

// ���������·����
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

// ���ؿ�������ͬʱ������еĽӿڽṹָ��
// ��1�����صĿ���Ҫ����������ʹ�� NXDLL_Unload �� ::FreeLibarary �ӿ��ͷţ�
// ������ӿ�ָ�벻��ҪҲ���ܱ��ͷţ�������ܻ�������������
// ��2������ϸ������� NXLoader.h �ļ��е� NXDLL_Load ������
inline HMODULE NXCPU_LoadDLL(LPCTSTR szCoreDLLParentPath,	// CoreDLL Ŀ¼��·��
							 LPCTSTR szDLLName,				// ���ؿ������
							 LPCSTR szAPIExtName,			// ���ؿ⵼���ӿڵ�����
							 void* *ppStuAPI)				// ����ӿڽṹָ��
{
	CNXCPUDLLPos _Pval;

	return NXDLL_Load(szCoreDLLParentPath, _Pval.EnvRefDebug(), _Pval.EnvDebugCount(), \
		_Pval.EnvRefRelease(), _Pval.EnvReleaseCount(), szDLLName, szAPIExtName, ppStuAPI);
}

#endif		// #ifndef __NX_CPU_BASE_H__
//////////////////////////////////////////////////////////////////////////
// End of file.