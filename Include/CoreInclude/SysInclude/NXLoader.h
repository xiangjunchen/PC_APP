//////////////////////////////////////////////////////////////////////////
// Copyright (C) Sobey Corporation. All rights reserved.
//
// Desc:	SDK 加载器模板.
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
// 常量定义

#ifndef MAX_PATH
#define MAX_PATH		260
#endif		// #ifndef MAX_PATH

// 环境变量的缺省最大长度，如果系统环境变量长度大于该值时，可选择修改此值。
#define NXENV_PATH_LEN		4096


//////////////////////////////////////////////////////////////////////////
// 显式加载功能库函数

// 库相对路径与库后缀声明
#define NXDLL_ROOT_FOLDER		_T("CoreDLL")
#define NXDLL_X86_FOLDER		_T("x86")
#define NXDLL_X64_FOLDER		_T("x64")
#define NXDLL_POSTFIX_DEBUG		_T("D.dll")
#define NXDLL_POSTFIX_RELEASE	_T(".dll")
#define NXDIR_SEPARATOR			_T('\\')
#define NXENV_SEPARATOR			_T(';')

// 库导出通用定义
typedef void* (*DefGetAPIStuPtr)();

// 加载库子函数
inline HMODULE _NXDLL_LoadLibrary(LPCTSTR szDLLName,	// 动态库名
								  LPCTSTR szPostfix)	// 后缀名
{
	tstring s_name(szDLLName);

	s_name += szPostfix;
	return ::LoadLibrary(const_cast<TCHAR*>(s_name.c_str()));
}
// 设置环境变量子函数
inline void _NXDLL_SetEnv(tstring &strEnv,			// 原有的环境变量
						  const tstring &strRoot,	// 顶层路径
						  LPCTSTR *pszEnvRef,		// 加载库在 CoreDLL 目录下需要设置的环境变量的相对路径
						  intptr_t nEnvCount)		// 环境变量相对路径的个数（包括加载库自身所在的相对路径）
{
	if (pszEnvRef != NULL && nEnvCount > 0)
	{
		tstring s_path;

		for (intptr_t i=0; i<nEnvCount; i++)
		{
			s_path = strRoot + pszEnvRef[i];
			s_path += NXDIR_SEPARATOR;
#if defined( _M_X64 )		// 64 位与 32 位平台
			s_path += NXDLL_X64_FOLDER;
#else
			s_path += NXDLL_X86_FOLDER;
#endif		// #if defined( _M_X64 )		// 64 位与 32 位平台
			s_path += NXENV_SEPARATOR;
			// 如果系统环境变量拥有当前环境变量则不再加入
			if (strEnv.find(s_path) == tstring::npos)
			{
				strEnv += s_path;
			}
		}
		::SetEnvironmentVariable(_T("Path"), const_cast<TCHAR*>(strEnv.c_str()));
	}
}

// 库析构
inline void NXDLL_Unload(HMODULE hDLL)
{
	::FreeLibrary(hDLL);
}

// 获取 API 子函数
inline void _NXDLL_GetAPI(HMODULE &hDLL,		// 已加载的库句柄
						  LPCSTR szAPIExtName,	// 加载库导出接口的名字
						  void* *ppAPI)			// 输出功能组接口指针)
{
	if (hDLL == NULL)
		return;

	DefGetAPIStuPtr pfnGet = reinterpret_cast<DefGetAPIStuPtr>(GetProcAddress(hDLL, szAPIExtName));

	*ppAPI = NULL;
	if (pfnGet != NULL)
	{
		*ppAPI = pfnGet();
	}
	if (*ppAPI == NULL)		// 如果导出接口不成功，则释放 DLL
	{
		NXDLL_Unload(hDLL);
		hDLL = NULL;
	}
}

// 返回库句柄，并同时输出库中的功能组接口指针
// （1）如果 szCoreDLLParentPath = NULL，将通过 NXDLL_ROOT_FOLDER 自动搜索缺省的路径。
// 如果搜索成功则设置环境变量，然后按库名直接加载。
// （2）自动搜索时，首先搜索与 BIN 目录同一路径，如果未找到，则遍历下一级子目录，搜索深度只为两层。
// （3）设置环境变量时，可能需要设置不只一个位置，CoreDLL 内部可能存在跨类别的调用。
// 此处按照约定的物理结构来映射，比如某类别的某库相对路径为：CoreDLL\"Type"\x86\*.dll。
// （4）返回的库句柄要求调用者最后使用 NXDLL_Unload 或 ::FreeLibarary 接口释放，
// 功能组接口指针不需要也不能被释放，否则可能会引起程序崩溃。
inline HMODULE NXDLL_Load(LPCTSTR szCoreDLLParentPath,	// CoreDLL 目录父路径
						  LPCTSTR *pszEnvRefDebug,		// 加载库在 CoreDLL 目录下需要设置的环境变量的相对路径
						  intptr_t nEnvDebugCount,		// 环境变量相对路径的个数（包括加载库自身所在的相对路径）
						  LPCTSTR *pszEnvRefRelease,
						  intptr_t nEnvReleaseCount,
						  LPCTSTR szDLLName,			// 加载库的名字
						  LPCSTR szAPIExtName,			// 加载库导出接口的名字
						  void* *ppAPI)					// 输出功能组接口指针
{
	if (szDLLName == NULL || szAPIExtName == NULL)
		return NULL;

	HMODULE hDLL = NULL;

	// 预先加载一次，避免重复的搜索缺省路径，尤其是单件加载器被多个库调用时
#ifdef _DEBUG		// 如果系统处于 DEBUG 模式下时，尝试导入 DEBUG 版本库
	hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_DEBUG);
#endif		// #ifdef _DEBUG
	if (hDLL == NULL)	// 如果不在 DEBUG 模式下或导入 DEBUG 版本库失败，则尝试导入 RELEASE 版本库
	{
		hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_RELEASE);
	}
	_NXDLL_GetAPI(hDLL, szAPIExtName, ppAPI);
	if (hDLL != NULL)	// 如果加载成功则直接返回，否则继续进行搜索
		return hDLL;

	tstring s_top, s_search, s_env, s_path;
	_tfinddata_t c_file;
	intptr_t h_file;
	bool has_dll_path = true;

	if (szCoreDLLParentPath == NULL)	// 搜索缺省路径
	{
		s_top.assign(MAX_PATH, _T('\0'));
		s_top.resize(::GetModuleFileName(NULL, const_cast<TCHAR*>(s_top.c_str()), MAX_PATH - 1));
		s_top.erase(s_top.rfind(NXDIR_SEPARATOR));
		s_top.erase(s_top.rfind(NXDIR_SEPARATOR) + 1);
		// 首先查找 BIN 目录所在路径，看 CoreDLL 目录是否存在
		s_search = s_top + NXDLL_ROOT_FOLDER;
		if ((h_file = _tfindfirst(const_cast<TCHAR*>(s_search.c_str()), &c_file)) == -1
			|| (c_file.attrib & _A_SUBDIR) == 0)	// 不存在则搜索子目录
		{
			has_dll_path = false;
			_findclose(h_file);	// 关闭前一搜索，开始遍历子目录层
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
							if (has_dll_path)	// 在某个子目录中找到 CoreDLL 目录
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
		_findclose(h_file);		// 关闭搜索
	}
	else	// 看 CoreDLL 目录是否存在
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
	if (has_dll_path)	// CoreDLL 目录存在才设置环境变理
	{
		s_env.assign(NXENV_PATH_LEN, _T('\0'));
		s_env.resize(::GetEnvironmentVariable(_T("Path"), const_cast<TCHAR*>(s_env.c_str()), NXENV_PATH_LEN - 1));
		s_env += NXENV_SEPARATOR;		// 防止最后一个环境变量未带分割符
		// 首先设置 CoreDLL 目录，如果系统环境变量拥有当前环境变量则不再加入
		s_top += NXDLL_ROOT_FOLDER;
		s_path = s_top + NXENV_SEPARATOR;
		s_top += NXDIR_SEPARATOR;
		if (s_env.find(s_path) == tstring::npos)
		{
			s_env += s_path;
		}
#ifdef _DEBUG		// 如果系统处于 DEBUG 模式下时，尝试导入 DEBUG 版本库
		_NXDLL_SetEnv(s_env, s_top, pszEnvRefDebug, nEnvDebugCount);
		hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_DEBUG);
#endif		// #ifdef _DEBUG
		if (hDLL == NULL)	// 如果不在 DEBUG 模式下或导入 DEBUG 版本库失败，则尝试导入 RELEASE 版本库
		{
			_NXDLL_SetEnv(s_env, s_top, pszEnvRefRelease, nEnvReleaseCount);
			hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_RELEASE);
		}
	}
	else	// CoreDLL 目录不存在时，直接试图加载
	{
#ifdef _DEBUG		// 如果系统处于 DEBUG 模式下时，尝试导入 DEBUG 版本库
		hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_DEBUG);
#endif		// #ifdef _DEBUG
		if (hDLL == NULL)	// 如果不在 DEBUG 模式下或导入 DEBUG 版本库失败，则尝试导入 RELEASE 版本库
		{
			hDLL = _NXDLL_LoadLibrary(szDLLName, NXDLL_POSTFIX_RELEASE);
		}
	}
	_NXDLL_GetAPI(hDLL, szAPIExtName, ppAPI);
	return hDLL;
}


//////////////////////////////////////////////////////////////////////////
// 加载器（支持延时手动 Load）
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
	// 指定路径的动态加载过程，如果构造时已加载成功，此处直接返回
	// （1）具体细节请参阅 NXDLL_Load 函数。
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
	// 获取功能接口
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
// 加载代理（VC6 版本下 __declspec(selectany) 不支持动态初始化特性，
// 采用静态变量作为单件的代理实现兼容）
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
	// 获取功能接口
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