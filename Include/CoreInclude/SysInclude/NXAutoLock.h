//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 多线程同步锁的自动锁动作
//
// Created: Zhaozhimeng @ 2010-09-16
//////////////////////////////////////////////////////////////////////////
#pragma once

namespace nxsys {

template <class _lock_ty>
class CNXAutoLock
{
public:
	typedef _lock_ty* pointer;

	explicit CNXAutoLock(const _lock_ty& _Lock) throw()
		: _Mylock(const_cast<pointer>(&_Lock))
	{
		_Mylock->Lock();
	}
	~CNXAutoLock() throw()
	{
		_Mylock->Unlock();
	}

protected:
	CNXAutoLock();
	CNXAutoLock(const CNXAutoLock&);
	CNXAutoLock& operator=(const CNXAutoLock&);

private:
	pointer		_Mylock;
};

}

//////////////////////////////////////////////////////////////////////////
// 文件结束