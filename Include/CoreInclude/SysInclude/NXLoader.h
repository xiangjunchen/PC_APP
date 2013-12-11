//////////////////////////////////////////////////////////////////////////
// Copyright (C) Sobey Corporation. All rights reserved.
//
// Desc:	SDK ������ģ��.
//
// Author:	Zhaozhimeng @ 2010-07-20 Beijing
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __NX_LOADER_H__
#define __NX_LOADER_H__

#include <io.h>
#include <string>
#include <wtypes.h>
#include <WinBase.h>

typedef std::basic_string<TCHAR> tstring;


//////////////////////////////////////////////////////////////////////////
// ��������

#ifndef MAX_PATH
#define MAX_PATH		260
#endif		// #ifndef MAX_PATH

// ����������ȱʡ��󳤶ȣ����ϵͳ�����������ȴ��ڸ�ֵʱ����ѡ���޸Ĵ�ֵ��
#define NXENV_PATH_LEN		4096


//////////////////////////////////////////////////////////////////////////
// ��ʽ���ع��ܿ⺯��

// �����·������׺����
#define NXDLL_ROOT_FOLDER		_T("CoreDLL")
#define NXDLL_X86_FOLDER		_T("x86")
#define NXDLL_X64_FOLDER		_T("x64")
#define NXDLL_POSTFIX_DEBUG		_T("D.dll")
#define NXDLL_POSTFIX_RELEASE	_T(".dll")
#define NXDIR_SEPARATOR			_T('\\')
#define NXENV_SEPARATOR			_T(';')

// �⵼��ͨ�ö���
typedef void* (*DefGetAPIStuPtr)();

// ���ؿ��Ӻ���
inline HMODULE _NXDLL_LoadLibrary(LPCTSTR szDLLName,	// ��̬����
								  LPCTSTR szPostfix)	// ��׺��
{
	tstring s_name(szDLLName);

	s_name += szPostfix;
	return ::LoadLibrary(const_cast<TCHAR*>(s_name.c_str()));
}
// ���û��������Ӻ���
inline void _NXDLL_SetEnv(tstring &strEnv,			// ԭ�еĻ�������
						  const tstring &strRoot,	// ����·��
						  LPCTSTR *pszEnvRef,		// ���ؿ��� CoreDLL Ŀ¼����Ҫ���õĻ������������·��
						  intptr_t nEnvCount)		// �����������·���ĸ������������ؿ��������ڵ����·����
{
	if (pszEnvRef != NULL && nEnvCount > 0)
	{
		tstring s_path;

		for (intptr_t i=0; i<nEnvCount; i++)
		{
			s_path = strRoot + pszEnvRef[i];
			s_path += NXDIR_SEPARATOR;
#if defined( _M_X64 )		// 64 λ�� 32 λƽ̨
			s_path += NXDLL_X64_FOLDER;
#else
			s_path += NXDLL_X86_FOLDER;
#endif		// #if defined( _M_X64 )		// 64 λ�� 32 λƽ̨
			s_path += NXENV_SEPARATOR;
			// ���ϵͳ��������ӵ�е�ǰ�����������ټ���
			if (strEnv.find(s_path) == tstring::npos)
			{
				strEnv += s_path;
			}
		}
		::SetEnvironmentVariable(_T("Path"), const_cast<TCHAR*>(strEnv.c_str()));
	}
}

// ������
inline void NXDLL_Unload(HMODULE hDLL)
{
	::FreeLibrary(hDLL);
}

// ��ȡ API �Ӻ���
inline void _NXDLL_GetAPI(HMODULE &hDLL,		// �Ѽ��صĿ���
						  LPCSTR szAPIExtName,	// ���ؿ⵼���ӿڵ�����
						  void* *ppAPI)			// ���������ӿ�ָ��)
{
	if (hDLL == NULL)
		return;

	DefGetAPIStuPtr pfnGet = reinterpret_cast<DefGetAPIStuPtr>(GetProcAddress(hDLL, szAPIExtName));

	*ppAPI = NULL;
	if (pfnGet != NULL)
	{
		*ppAPI = pfnGet();
	}
	if (*ppAPI == NULL)		// ��������ӿڲ��ɹ������ͷ� DLL
	{
		NXDLL_Unload(hDLL);
		hDLL = NULL;
	}
}

// ���ؿ�������ͬʱ������еĹ�����ӿ�ָ��
// ��1����� szCoreDLLParentPath = NULL����ͨ�� NXDLL_ROOT_FOLDER �Զ�����ȱʡ��·����
// ��������ɹ������û���������Ȼ�󰴿���ֱ�Ӽ��ء�
// ��2���Զ�����ʱ������������ BIN Ŀ¼ͬһ·�������δ�ҵ����������һ����Ŀ¼���������ֻΪ���㡣
// ��3�����û�������ʱ��������Ҫ���ò�ֻһ��λ�ã�CoreDLL �ڲ����ܴ��ڿ����ĵ��á�
// �˴�����Լ��������ṹ��ӳ�䣬����ĳ����ĳ�����·��Ϊ��CoreDLL\"Type"\x86\*.dll��
// ��4�����صĿ���Ҫ����������ʹ�� NXDLL_Unload �� ::FreeLibarary �ӿ��ͷţ�
// ������ӿ�ָ�벻��ҪҲ���ܱ��ͷţ�������ܻ�������������
inline HMODULE NXDLL_Load(LPCTSTR szCoreDLLParentPath,	// CoreDLL Ŀ¼��·��
						  LPCTSTR *pszEnvRefDebug,		// ���ؿ��� CoreDLL Ŀ¼����Ҫ���õĻ������������·��
						  intptr_t nEnvDebugCount,		// �����������·���ĸ������������ؿ��������ڵ����·����
						  LPCTSTR *pszEnvRefRelease,
						  intptr_t nEnvReleaseCount,
						  LPCTSTR szDLLName,			// ���ؿ������
						  LPCSTR szAPIExtName,			// ���ؿ⵼���ӿڵ�����
						  void* *ppAPI)					// ���������ӿ�ָ��
{
	if (szDLLName == NULL || szAPIExtName == NULL)
		return NULL;

	HMODULE hDLL = NULL;

	// Ԥ�ȼ���һ�Σ������ظ�������ȱʡ·���������ǵ�������������������ʱ
#ifdef _DEBUG		// ���ϵͳ���� DEBUG ģʽ��ʱ�����Ե��� DEBUG �汾��
	hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_DEBUG);
#endif		// #ifdef _DEBUG
	if (hDLL == NULL)	// ������� DEBUG ģʽ�»��� DEBUG �汾��ʧ�ܣ����Ե��� RELEASE �汾��
	{
		hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_RELEASE);
	}
	_NXDLL_GetAPI(hDLL, szAPIExtName, ppAPI);
	if (hDLL != NULL)	// ������سɹ���ֱ�ӷ��أ����������������
		return hDLL;

	tstring s_top, s_search, s_env, s_path;
	_tfinddata_t c_file;
	intptr_t h_file;
	bool has_dll_path = true;

	if (szCoreDLLParentPath == NULL)	// ����ȱʡ·��
	{
		s_top.assign(MAX_PATH, _T('\0'));
		s_top.resize(::GetModuleFileName(NULL, const_cast<TCHAR*>(s_top.c_str()), MAX_PATH - 1));
		s_top.erase(s_top.rfind(NXDIR_SEPARATOR));
		s_top.erase(s_top.rfind(NXDIR_SEPARATOR) + 1);
		// ���Ȳ��� BIN Ŀ¼����·������ CoreDLL Ŀ¼�Ƿ����
		s_search = s_top + NXDLL_ROOT_FOLDER;
		if ((h_file = _tfindfirst(const_cast<TCHAR*>(s_search.c_str()), &c_file)) == -1
			|| (c_file.attrib & _A_SUBDIR) == 0)	// ��������������Ŀ¼
		{
			has_dll_path = false;
			_findclose(h_file);	// �ر�ǰһ��������ʼ������Ŀ¼��
			s_search = s_top + _T("*.*");
			if ((h_file = _tfindfirst(const_cast<TCHAR*>(s_search.c_str()), &c_file)) != -1)
			{
				_tfinddata_t c_file_sub;
				intptr_t h_file_sub;
				int n_done(0);

				while (n_done == 0)
				{
					if (c_file.attrib & _A_SUBDIR)
					{
						s_search = s_top + c_file.name;
						s_search += NXDIR_SEPARATOR;
						s_search += NXDLL_ROOT_FOLDER;
						if ((h_file_sub = _tfindfirst(const_cast<TCHAR*>(s_search.c_str()), &c_file_sub)) != -1)
						{
							has_dll_path = ((c_file_sub.attrib & _A_SUBDIR) != 0);
							_findclose(h_file_sub);
							if (has_dll_path)	// ��ĳ����Ŀ¼���ҵ� CoreDLL Ŀ¼
							{
								s_top += c_file.name;
								s_top += NXDIR_SEPARATOR;
								break;
							}
						}
					}
					n_done = _tfindnext(h_file, &c_file);
				}
			}
		}
		_findclose(h_file);		// �ر�����
	}
	else	// �� CoreDLL Ŀ¼�Ƿ����
	{
		s_top = szCoreDLLParentPath;
		s_top += NXDIR_SEPARATOR;
		s_search = s_top + NXDLL_ROOT_FOLDER;
		if ((h_file = _tfindfirst(const_cast<TCHAR*>(s_search.c_str()), &c_file)) == -1
			|| (c_file.attrib & _A_SUBDIR) == 0)
		{
			has_dll_path = false;
		}
		_findclose(h_file);
	}
	if (has_dll_path)	// CoreDLL Ŀ¼���ڲ����û�������
	{
		s_env.assign(NXENV_PATH_LEN, _T('\0'));
		s_env.resize(::GetEnvironmentVariable(_T("Path"), const_cast<TCHAR*>(s_env.c_str()), NXENV_PATH_LEN - 1));
		s_env += NXENV_SEPARATOR;		// ��ֹ���һ����������δ���ָ��
		// �������� CoreDLL Ŀ¼�����ϵͳ��������ӵ�е�ǰ�����������ټ���
		s_top += NXDLL_ROOT_FOLDER;
		s_path = s_top + NXENV_SEPARATOR;
		s_top += NXDIR_SEPARATOR;
		if (s_env.find(s_path) == tstring::npos)
		{
			s_env += s_path;
		}
#ifdef _DEBUG		// ���ϵͳ���� DEBUG ģʽ��ʱ�����Ե��� DEBUG �汾��
		_NXDLL_SetEnv(s_env, s_top, pszEnvRefDebug, nEnvDebugCount);
		hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_DEBUG);
#endif		// #ifdef _DEBUG
		if (hDLL == NULL)	// ������� DEBUG ģʽ�»��� DEBUG �汾��ʧ�ܣ����Ե��� RELEASE �汾��
		{
			_NXDLL_SetEnv(s_env, s_top, pszEnvRefRelease, nEnvReleaseCount);
			hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_RELEASE);
		}
	}
	else	// CoreDLL Ŀ¼������ʱ��ֱ����ͼ����
	{
#ifdef _DEBUG		// ���ϵͳ���� DEBUG ģʽ��ʱ�����Ե��� DEBUG �汾��
		hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_DEBUG);
#endif		// #ifdef _DEBUG
		if (hDLL == NULL)	// ������� DEBUG ģʽ�»��� DEBUG �汾��ʧ�ܣ����Ե��� RELEASE �汾��
		{
			hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_RELEASE);
		}
	}
	_NXDLL_GetAPI(hDLL, szAPIExtName, ppAPI);
	return hDLL;
}


//////////////////////////////////////////////////////////////////////////
// ��������֧����ʱ�ֶ� Load��
template <class _Func, class _Pos>
class CNXAutoLoader
{
public:
	typedef CNXAutoLoader<_Func, _Pos> _Myt;
	typedef _Func* _Func_ptr;

	CNXAutoLoader() throw() : _Module(NULL), _Fpval(NULL)
	{
		Load(NULL);
	}
	~CNXAutoLoader() throw()
	{
		NXDLL_Unload(_Module);
	}
	// ָ��·���Ķ�̬���ع��̣��������ʱ�Ѽ��سɹ����˴�ֱ�ӷ���
	// ��1������ϸ������� NXDLL_Load ������
	bool Load(LPCTSTR szCoreDLLParentPath)
	{
		if (_IsLoaded())
			return true;
		_Pos _Pval;
		_Module = NXDLL_Load(szCoreDLLParentPath,
			_Pval.EnvRefDebug(), _Pval.EnvDebugCount(),
			_Pval.EnvRefRelease(), _Pval.EnvReleaseCount(),
			_Pval.DLLName(), _Pval.APIExtName(), (void**)&_Fpval);
		return _IsLoaded();
	}
	// ��ȡ���ܽӿ�
	operator bool() const
	{
		return (_Fpval != NULL);
	}
	operator _Func_ptr() const
	{
		return _Fpval;
	}
	_Func_ptr operator->() const
	{
		return (_Fpval);
	}

private:
	CNXAutoLoader(_Myt const&) throw();
	_Myt& operator=(_Myt const&) throw();
	bool _IsLoaded() { return (_Module != NULL); }

private:
	HMODULE		_Module;
	_Func_ptr	_Fpval;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1300)

#define __nxselectany		__declspec(selectany)
#define CNXLoader			CNXAutoLoader

#else		// VC6
// ���ش���VC6 �汾�� __declspec(selectany) ��֧�ֶ�̬��ʼ�����ԣ�
// ���þ�̬������Ϊ�����Ĵ���ʵ�ּ��ݣ�
template <class _Func, class _Pos>
class CNXLoaderProxy
{
public:
	typedef CNXLoaderProxy<_Func, _Pos> _Myt;
	typedef CNXAutoLoader<_Func, _Pos> _Myvart;
	typedef _Func* _Func_ptr;

	CNXLoaderProxy() throw()
	{
		_Singleton();
	}
	~CNXLoaderProxy() throw() {}

	bool Load(LPCTSTR szCoreDLLParentPath)
	{
		return _Singleton().Load(szCoreDLLParentPath);
	}
	// ��ȡ���ܽӿ�
	operator _Func_ptr() const
	{
		return (_Singleton());
	}
	_Func_ptr operator->() const
	{
		return (_Singleton());
	}

private:
	CNXLoaderProxy(_Myt const&) throw();
	_Myt& operator=(_Myt const&) throw();

	_Myvart& _Singleton() const
	{
		static _Myvart _Myloader;
		return _Myloader;
	}
};

#define __nxselectany		static
#define CNXLoader			CNXLoaderProxy

#endif		// #if defined(_MSC_VER) && (_MSC_VER >= 1300)


#endif		// #ifndef __NX_LOADER_H__
//////////////////////////////////////////////////////////////////////////
// End of file.