//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 异步计数器
//
// Created: Zhaozhimeng @ 2010-05-20
//////////////////////////////////////////////////////////////////////////
#pragma once

#include ".\NXAutoLock.h"

namespace nxsys {

template <typename _count_ty, class _lock_ty, class _event_ty>
class CNXAsyncCounter
{
public:
	typedef CNXAsyncCounter<_count_ty, _lock_ty, _event_ty> _Myt;
	typedef CNXAutoLock<_lock_ty> _Myautolockt;

	CNXAsyncCounter() throw() : _Mycount(static_cast<_count_ty>(0))
	{
		// 第一个参数为 bManualReset，第二个参数为 bInitialState
		_Myempty.Create(true, true);
	}
	~CNXAsyncCounter() throw()
	{
		Reset();
		_Myempty.Close();
	}
	void Reset()
	{
		_Myautolockt _Autolock(_Mylock);
		_SetCount(0);
	}
	_Myt& operator++()
	{
		_Myautolockt _Autolock(_Mylock);
		_SetCount(_GetCount() + static_cast<_count_ty>(1));
		return (*this);
	}
	_count_ty operator++(int)
	{
		_Myautolockt _Autolock(_Mylock);
		_count_ty _Count = _GetCount();
		_SetCount(_GetCount() + static_cast<_count_ty>(1));
		return _Count;
	}
	_Myt& operator+=(_count_ty _Val)
	{
		_Myautolockt _Autolock(_Mylock);
		_SetCount(_GetCount() + _Val);
		return (*this);
	}
	_Myt& operator--()
	{
		_Myautolockt _Autolock(_Mylock);
		_SetCount(_GetCount() - static_cast<_count_ty>(1));
		return (*this);
	}
	_count_ty operator--(int)
	{
		_Myautolockt _Autolock(_Mylock);
		_count_ty _Count = _GetCount();
		_SetCount(_GetCount() - static_cast<_count_ty>(1));
		return _Count;
	}
	_Myt& operator-=(_count_ty _Val)
	{
		_Myautolockt _Autolock(_Mylock);
		_SetCount(_GetCount() - _Val);
		return (*this);
	}
	operator _count_ty() const
	{
		_Myautolockt _Autolock(_Mylock);
		return _GetCount();
	}
	void WaitEmpty(unsigned long _Milliseconds)
	{
		_Myempty.Wait(_Milliseconds);
	}

protected:
	CNXAsyncCounter(const _Myt&);
	_Myt& operator=(const _Myt&);
	void _SetCount(_count_ty _Count)
	{
		_Mycount = (_Count <= static_cast<_count_ty>(0) ? static_cast<_count_ty>(0) : _Count);
		if (_GetCount() <= static_cast<_count_ty>(0))
		{
			_Myempty.Set();
		}
		else
		{
			_Myempty.Reset();
		}
	}
	_count_ty _GetCount() const
	{
		return _Mycount;
	}

private:
	_lock_ty		_Mylock;
	_event_ty		_Myempty;
	_count_ty		_Mycount;
};

}

//////////////////////////////////////////////////////////////////////////
// End of file.