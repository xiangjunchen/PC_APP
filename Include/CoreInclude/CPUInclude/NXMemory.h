//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 通用内存分配器，支持私有内存池与系统方法间的无缝切换.
//
// Designed: LaiShoubo
// Created: ZhaoZhimeng @ 2010-07-15
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __NX_MEMORY_H__
#define __NX_MEMORY_H__

#include ".\NXMemoryPool.h"


//////////////////////////////////////////////////////////////////////////
// 自动加载器
#include "NXCPUBase.h"

// 加载器相对路径类
class CNXMemoryPoolDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXMEMORYPOOL_DLL_NAME; }
	LPCSTR APIExtName() const { return NXMEMORYPOOL_API_EXT_NAME; }
};

// 全局单件加载器
// （1）具体细节请参阅 NXLoader.h 文件。
__nxselectany CNXLoader<INXMemoryPool, CNXMemoryPoolDLLPos> g_NXMemoryPool;


//////////////////////////////////////////////////////////////////////////
#include <memory>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <string>
#include <tchar.h>
#include <malloc.h>

//////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma pack(push, _CRT_PACKING)
#pragma warning(push, 3)
#pragma push_macro("new")
#endif

#undef new

//////////////////////////////////////////////////////////////////////////
// 所有类的内存管理基类，被其它类公有继承
interface INXObject
{
	void* __cdecl operator new(size_t sz)
	{
		return (g_NXMemoryPool ? g_NXMemoryPool->AllocateArray(sz) : (malloc(sz)));
	}
	void* __cdecl operator new[](size_t sz)
	{
		return (g_NXMemoryPool ? g_NXMemoryPool->AllocateArray(sz) : (malloc(sz)));
	}
	void __cdecl operator delete(void *ptr) throw()
	{
		g_NXMemoryPool ? g_NXMemoryPool->DeallocateArray(ptr) : (free(ptr));
	}
	void __cdecl operator delete[](void *ptr) throw()
	{
		g_NXMemoryPool ? g_NXMemoryPool->DeallocateArray(ptr) : (free(ptr));
	}
	// 用于 DEBUG 版本中定位调用者来源
	void* __cdecl operator new(size_t sz, char const* file, int line)
	{
		return (g_NXMemoryPool ? g_NXMemoryPool->AllocateArray(sz, file, line) : (malloc(sz)));
	}
	void* __cdecl operator new[](size_t sz, char const* file, int line)
	{
		return (g_NXMemoryPool ? g_NXMemoryPool->AllocateArray(sz, file, line) : (malloc(sz)));
	}
	// placement delete if placement new for debug fails
	void __cdecl operator delete(void* ptr, char const*, int) throw()
	{
		g_NXMemoryPool ? g_NXMemoryPool->DeallocateArray(ptr) : (free(ptr));
	}
	// placement array delete if placement array new for debug fails
	void __cdecl operator delete[](void* ptr, char const*, int) throw()
	{
		g_NXMemoryPool ? g_NXMemoryPool->DeallocateArray(ptr) : (free(ptr));
	}
};

//////////////////////////////////////////////////////////////////////////
// 内存池使用的宏定义
#define nx_new(_Ty, _Size)		reinterpret_cast<##_Ty*>(INXObject::operator new[]((_Size) * sizeof(##_Ty)))
#define nx_delete(_Ptr)			INXObject::operator delete[](_Ptr)

//////////////////////////////////////////////////////////////////////////
// STD 标准库扩展
_NXMP_BEGIN

// STL 内存分配器扩展
template<class _Ty>
class allocator : public std::allocator<_Ty>
{
public:
	typedef std::allocator<_Ty> base_type;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::size_type size_type;

	allocator() throw()
	{
	}
	allocator(allocator<_Ty> const&) throw()
	{
	}

	template<class _Other>
	struct rebind
	{
		typedef allocator<_Other> other;
	};

	template<class _Other>
	allocator(allocator<_Other> const&) throw()
	{
	}

	template<class _Other>
	allocator<_Ty>& operator=(allocator<_Other> const&) throw()
	{
		return (*this);
	}

	bool operator==(allocator<_Ty> const&) const throw()
	{
		return true;
	}
	bool operator!=(allocator<_Ty> const&) const throw()
	{
		return false;
	}

	pointer allocate(size_type _Count)
	{
		return reinterpret_cast<pointer>(g_NXMemoryPool ? g_NXMemoryPool->AllocateOne(_Count * sizeof(_Ty)) : base_type::allocate(_Count));
	}
	pointer allocate(size_type _Count, void const*)
	{
		return allocate(_Count);
	}
	void deallocate(pointer _Ptr, size_type _Count) throw()
	{
		g_NXMemoryPool ? g_NXMemoryPool->DeallocateOne(_Ptr, _Count * sizeof(_Ty)) : base_type::deallocate(_Ptr, _Count);
	}
};

// STL 容器扩展
template<class _Ty, class _Ax = allocator<_Ty> >
class vector : public std::vector<_Ty, _Ax>, public INXObject
{
public:
	typedef std::vector<_Ty, _Ax> _Mybase;
	typedef typename _Mybase::_Alloc _Alloc;
	typedef typename _Mybase::size_type size_type;

	vector()
		: _Mybase()
	{
	}
	explicit vector(_Alloc const& _Al)
		: _Mybase(_Al)
	{
	}
	explicit vector(size_type _Count)
		: _Mybase(_Count)
	{
	}
	vector(size_type _Count, _Ty const& _Val)
		: _Mybase(_Count, _Val)
	{
	}
	vector(size_type _Count, _Ty const& _Val, _Alloc const& _Al)
		: _Mybase(_Count, _Val, _Al)
	{
	}
	template<class _Iter>
		vector(_Iter _First, _Iter _Last)
		: _Mybase(_First, _Last)
	{
	};
	template<class _Iter>
	vector(_Iter _First, _Iter _Last, const _Alloc& _Al)
		: _Mybase(_First, _Last, _Al)
	{
	}
};

template<class _Ty, class _Ax = allocator<_Ty> >
class deque : public std::deque<_Ty, _Ax>, public INXObject
{
public:
	typedef std::deque<_Ty, _Ax> _Mybase;
	typedef typename _Mybase::_Alloc _Alloc;
	typedef typename _Mybase::size_type size_type;

	deque()
		: _Mybase()
	{
	}
	explicit deque(_Alloc const& _Al)
		: _Mybase(_Al)
	{
	}
	explicit deque(size_type _Count)
		: _Mybase(_Count)
	{
	}
	deque(size_type _Count, const _Ty& _Val)
		: _Mybase(_Count, _Val)
	{
	}
	deque(size_type _Count, const _Ty& _Val, const _Alloc& _Al)
		: _Mybase(_Count, _Val, _Al)
	{
	}
	template<class _It>
		deque(_It _First, _It _Last)
		: _Mybase(_First, _Last)
	{
	}
	template<class _It>
		deque(_It _First, _It _Last, const _Alloc& _Al)
		: _Mybase(_First, _Last, _Al)
	{
	}
};

template<class _Ty, class _Ax = allocator<_Ty> >
class list : public std::list<_Ty, _Ax>, public INXObject
{
public:
	typedef std::list<_Ty, _Ax> _Mybase;
	typedef typename _Mybase::_Alloc _Alloc;
	typedef typename _Mybase::size_type size_type;

	list()
		: _Mybase()
	{
	}
	explicit list(const _Alloc& _Al)
		:_Mybase(_Al)
	{
	}
	explicit list(size_type _Count)
		: _Mybase(_Count)
	{
	}
	list(size_type _Count, const _Ty& _Val)
		: _Mybase(_Count, _Val)
	{
	}
	list(size_type _Count, const _Ty& _Val, const _Alloc& _Al)
		:_Mybase(_Count, _Val, _Al)
	{
	}
	template<class _Iter>
		list(_Iter _First, _Iter _Last)
		: _Mybase(_First, _Last)
	{
	}
	template<class _Iter>
		list(_Iter _First, _Iter _Last, const _Alloc& _Al)
		: _Mybase(_First, _Last, _Al)
	{
	}
};

template<class _Kty, class _Pr = std::less<_Kty>, class _Ax = allocator<_Kty> >
class set : public std::set<_Kty, _Pr, _Ax>, public INXObject
{
public:
	typedef std::set<_Kty, _Pr, _Ax> _Mybase;
	typedef typename _Mybase::key_compare key_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;

	set()
		: _Mybase()
	{
	}
	explicit set(const key_compare& _Pred)
		: _Mybase(_Pred)
	{
	}
	set(const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{
	}
	template<class _Iter>
		set(_Iter _First, _Iter _Last)
		:_Mybase(_First, _Last)
	{
	}
	template<class _Iter>
		set(_Iter _First, _Iter _Last, const key_compare& _Pred)
		: _Mybase(_First, _Last, _Pred)
	{
	}
	template<class _Iter>
		set(_Iter _First, _Iter _Last, const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_First, _Last, _Pred, _Al)
	{
	}
};

template<class _Kty, class _Pr = std::less<_Kty>, class _Ax = allocator<_Kty> >
class multiset : public std::multiset<_Kty, _Pr, _Ax>, public INXObject
{
public:
	typedef std::multiset<_Kty, _Pr, _Ax> _Mybase;
	typedef typename _Mybase::key_compare key_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;

	multiset()
		: _Mybase()
	{
	}
	explicit multiset(const key_compare& _Pred)
		: _Mybase(_Pred)
	{
	}
	multiset(const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{
	}
	template<class _Iter>
	multiset(_Iter _First, _Iter _Last)
		:_Mybase(_First, _Last)
	{
	}
	template<class _Iter>
	multiset(_Iter _First, _Iter _Last, const key_compare& _Pred)
		: _Mybase(_First, _Last, _Pred)
	{
	}
	template<class _Iter>
	multiset(_Iter _First, _Iter _Last, const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_First, _Last, _Pred, _Al)
	{
	}
};

template<class _Kty, class _Ty, class _Pr = std::less<_Kty>, class _Ax = allocator<std::pair<const _Kty, _Ty> > >
class map : public std::map<_Kty, _Ty, _Pr, _Ax>, public INXObject
{
public:
	typedef std::map<_Kty, _Ty, _Pr, _Ax> _Mybase;
	typedef typename _Mybase::key_compare key_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;

	map()
		: _Mybase()
	{
	}
	explicit map(const key_compare& _Pred)
		: _Mybase(_Pred)
	{
	}
	map(const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{
	}
	template<class _Iter>
	map(_Iter _First, _Iter _Last)
		:_Mybase(_First, _Last)
	{
	}
	template<class _Iter>
	map(_Iter _First, _Iter _Last, const key_compare& _Pred)
		: _Mybase(_First, _Last, _Pred)
	{
	}
	template<class _Iter>
	map(_Iter _First, _Iter _Last, const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_First, _Last, _Pred, _Al)
	{
	}
};

template<class _Kty, class _Ty, class _Pr = std::less<_Kty>, class _Ax = allocator<std::pair<const _Kty, _Ty> > >
class multimap : public std::multimap<_Kty, _Ty, _Pr, _Ax>, public INXObject
{
public:
	typedef std::multimap<_Kty, _Ty, _Pr, _Ax> _Mybase;
	typedef typename _Mybase::key_compare key_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;

	multimap()
		: _Mybase()
	{
	}
	explicit multimap(const key_compare& _Pred)
		: _Mybase(_Pred)
	{
	}
	multimap(const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{
	}
	template<class _Iter>
	multimap(_Iter _First, _Iter _Last)
		:_Mybase(_First, _Last)
	{
	}
	template<class _Iter>
	multimap(_Iter _First, _Iter _Last, const key_compare& _Pred)
		: _Mybase(_First, _Last, _Pred)
	{
	}
	template<class _Iter>
	multimap(_Iter _First, _Iter _Last, const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_First, _Last, _Pred, _Al)
	{
	}
};

template<class _Elem, class _Traits = std::char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string : public std::basic_string<_Elem, _Traits, _Ax>, public INXObject
{
public:
	typedef basic_string<_Elem, _Traits, _Ax> _Myt;
	typedef std::basic_string<_Elem, _Traits, _Ax> _Mybase;
	typedef typename _Mybase::_Alloc _Alloc;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::const_pointer const_pointer;
	typedef typename _Mybase::const_iterator const_iterator;

	__CLR_OR_THIS_CALL basic_string()
		: _Mybase()
	{
	}
	explicit __CLR_OR_THIS_CALL basic_string(const _Alloc& _Al)
		: _Mybase(_Al)
	{
	}
	__CLR_OR_THIS_CALL basic_string(const _Myt& _Right, size_type _Roff, size_type _Count = npos)
		: _Mybase(_Right, _Roff, _Count)
	{
	}
	__CLR_OR_THIS_CALL basic_string(const _Myt& _Right, size_type _Roff, size_type _Count,
		const _Alloc& _Al)
		: _Mybase(_Right, _Roff, _Count, _Al)
	{
	}
	__CLR_OR_THIS_CALL basic_string(const _Elem *_Ptr, size_type _Count)
		: _Mybase(_Ptr, _Count)
	{
	}
	__CLR_OR_THIS_CALL basic_string(const _Elem *_Ptr, size_type _Count, const _Alloc& _Al)
		: _Mybase(_Ptr, _Count, _Al)
	{
	}
	__CLR_OR_THIS_CALL basic_string(const _Elem *_Ptr)
		: _Mybase(_Ptr)
	{
	}
	__CLR_OR_THIS_CALL basic_string(const _Elem *_Ptr, const _Alloc& _Al)
		: _Mybase(_Ptr, _Al)
	{
	}
	__CLR_OR_THIS_CALL basic_string(size_type _Count, _Elem _Ch)
		: _Mybase(_Count, _Ch)
	{
	}
	__CLR_OR_THIS_CALL basic_string(size_type _Count, _Elem _Ch, const _Alloc& _Al)
		: _Mybase(_Count, _Ch, _Al)
	{
	}
	template<class _It>
	__CLR_OR_THIS_CALL basic_string(_It _First, _It _Last)
		: _Mybase(_First, _Last)
	{
	}
	template<class _It>
	__CLR_OR_THIS_CALL basic_string(_It _First, _It _Last, const _Alloc& _Al)
		: _Mybase(_First, _Last, _Al)
	{
	}
	__CLR_OR_THIS_CALL basic_string(const_pointer _First, const_pointer _Last)
		: _Mybase(_First, _Last)
	{
	}
	__CLR_OR_THIS_CALL basic_string(const_iterator _First, const_iterator _Last)
		: _Mybase(_First, _Last)
	{
	}
};

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
typedef basic_string<TCHAR> tstring;

_NXMP_END

//////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma pack(pop)
#pragma warning(pop)
#pragma pop_macro("new")
#endif


#endif		// #ifndef __NX_MEMORY_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
