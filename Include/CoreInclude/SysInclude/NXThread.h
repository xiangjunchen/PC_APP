//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 线程基类模板
//
// Created by Zhaozhimeng @ 2010-05-20
//////////////////////////////////////////////////////////////////////////
#pragma once

#include ".\NXMultiThread.h"

_NXSYS_BEGIN

//////////////////////////////////////////////////////////////////////////
// 线程基类模板

template <class _job_ty>
class CNXThread : public INXUnknownThread
{
public:
	typedef CNXThread<_job_ty> _Myt;
	typedef _job_ty* pointer;
	typedef const _job_ty* const_pointer;

	CNXThread() throw() : _Myjob(NULL), _Myautokill(false), _Mylocalobj(false) {}
	virtual ~CNXThread() throw() {}
	// 创建：_Local_obj 参数主要用于确认线程对象是否为局部变量
	bool Create(const_pointer _Ptr_job, bool _Suspend, bool _Local_obj = false)
	{
		if (_Ptr_job == NULL)
			return false;
		_Myautokill = false;
		_Mylocalobj = _Local_obj;
		_Myjob = const_cast<pointer>(_Ptr_job);
		return _Create(_Suspend);	// 实际创建工作
	}
	// 状态
	void AutoKill()		// 允许线程对象自主退出
	{
		_Myautokill = true;
	}
	virtual bool IsAutoKill() const
	{
		return _Myautokill;
	}
	// 结束线程对象
	virtual void Exit(unsigned long ulExitCode) = 0;
	// 挂起或恢复线程对象
	virtual unsigned long Suspend() = 0;
	virtual unsigned long Resume() = 0;

protected:
	CNXThread(const _Myt &_Right);
	_Myt& operator=(const _Myt &_Right);
	// 创建
	virtual bool _Create(bool _Suspend) = 0;
	// 线程回调主体
	int _Run()
	{
		return _Myjob->Run(_Mylocalobj ? NULL : this);
	}

private:
	pointer	_Myjob;
	bool	_Myautokill;	// 自我销毁标志
	bool	_Mylocalobj;	// 局部对象标志
};

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// End of file.