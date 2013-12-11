//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 对象分配器，实现托管式构造/析构.
//				对象分配池，实现对象资源的循环利用.
//
// Created: ZhaoZhimeng @ 2010-07-15
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __NX_OBJ_ALLOC_H__
#define __NX_OBJ_ALLOC_H__

#define __USE_NXMP_ALLOCATOR__
#ifdef __USE_NXMP_ALLOCATOR__
#include "..\CPUInclude\NXMemory.h"
#define __MYUSED__	nxmp
#else
#include <vector>
#include <memory>
#define __MYUSED__	std
#endif	// #ifdef __USE_NXMP_ALLOCATOR__

#include ".\NXAutoLock.h"

namespace nxsys {

//////////////////////////////////////////////////////////////////////////
// 对象分配器
template <class _obj_ty, class _Ax = __MYUSED__::allocator<_obj_ty> >
class CNXObjAlloc
{
public:
	typedef typename _Ax::pointer pointer;
	typedef typename _Ax::size_type size_type;

	CNXObjAlloc() throw() {}
	// 分配并构造
	pointer Allocate(size_type _Count) throw()
	{
		pointer _Ptr = _Alval.allocate(_Count);
		stdext::checked_uninitialized_fill_n(_Ptr, _Count, _obj_ty(), _Alval);
		return _Ptr;
	}
	// 析构再释放
	void Deallocate(pointer _Ptr, size_type _Count) throw()
	{
		std::_Destroy_range(_Ptr, (_Ptr + _Count), _Alval);
		_Alval.deallocate(_Ptr, _Count);
	}

protected:
	_Ax		_Alval;
};


//////////////////////////////////////////////////////////////////////////
// 对象分配池

template <class _lock_ty, class _obj_base>
class CNXObjPoolBase
{
public:
	typedef _obj_base* base_pointer;
	typedef CNXAutoLock<_lock_ty> _Myautolockt;
	typedef __MYUSED__::vector<base_pointer> _Mypoolt;

	CNXObjPoolBase() throw() {}
	virtual ~CNXObjPoolBase() throw() {}
	// 分配与释放对象
	base_pointer Allocate() throw()
	{
		base_pointer _Ptr = _PopFromPool();
		if (_Ptr == NULL)
		{
			_Ptr = _New();
		}
		return _Ptr;
	}
	void Deallocate(base_pointer _Ptr)
	{
		_Myautolockt _Lock(_Mylock);
		_Mypool.push_back(_Ptr);
	}
	// 释放分配器
	void Release() throw()
	{
		_Myautolockt _Lock(_Mylock);
		for (_Mypoolt::iterator _iter=_Mypool.begin(); _iter!=_Mypool.end(); _iter++)
		{
			_Destroy(*_iter);
		}
		_Mypool.clear();
	}

protected:
	// 从池子中取出一个对象
	base_pointer _PopFromPool()
	{
		base_pointer _Ptr(NULL);
		_Myautolockt _Lock(_Mylock);
		if (!_Mypool.empty())
		{
			_Ptr = _Mypool.back();
			_Mypool.pop_back();
		}
		return _Ptr;
	}
	// 申请/销毁一个对象
	virtual base_pointer _New() throw() = 0;
	virtual void _Destroy(base_pointer _Ptr) throw() = 0;

private:
	_Mypoolt	_Mypool;
	_lock_ty	_Mylock;
};

template <class _obj_ty, class _lock_ty, class _obj_base = _obj_ty>
class CNXObjPool : public CNXObjPoolBase<_lock_ty, _obj_base>
{
public:
	typedef _obj_ty* pointer;
	typedef CNXObjPoolBase<_lock_ty, _obj_base> _Mybase;
	typedef typename _Mybase::base_pointer base_pointer;

	virtual ~CNXObjPool() throw()
	{
		_Mybase::Release();
	}

protected:
	// 申请/销毁一个对象
	virtual base_pointer _New() throw()
	{
		return static_cast<base_pointer>(new _obj_ty);
	}
	virtual void _Destroy(base_pointer _Ptr) throw()
	{
		delete static_cast<pointer>(_Ptr);
	}
};

template <class _obj_ty, class _lock_ty, class _obj_base = _obj_ty, class _Ax = CNXObjAlloc<_obj_ty> >
class CNXObjPoolEx : public CNXObjPoolBase<_lock_ty, _obj_base>
{
public:
	typedef typename _Ax::pointer pointer;
	typedef CNXObjPoolBase<_lock_ty, _obj_base> _Mybase;
	typedef typename _Mybase::base_pointer base_pointer;

	virtual ~CNXObjPoolEx() throw()
	{
		_Mybase::Release();
	}

protected:
	// 申请/销毁一个对象
	virtual base_pointer _New() throw()
	{
		return static_cast<base_pointer>(_Alval.Allocate(1));
	}
	virtual void _Destroy(base_pointer _Ptr) throw()
	{
		_Alval.Deallocate(static_cast<pointer>(_Ptr), 1);
	}

protected:
	_Ax		_Alval;
};

}

#endif		// #ifndef __NX_OBJ_ALLOC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.