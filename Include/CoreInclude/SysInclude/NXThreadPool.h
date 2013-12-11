//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 通用线程池
//
// Created by Zhaozhimeng @ 2010-05-20
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include ".\NXTemplate.h"
#include ".\NXMultiThread.h"

_NXSYS_BEGIN

//////////////////////////////////////////////////////////////////////////
// 通用线程池模板

template <class _thread_ty, class _task_man, class _task_base_ty>
class CNXThreadPool
{
public:
	typedef CNXThreadPool<_thread_ty, _task_man, _task_base_ty> _Myt;
	typedef _thread_ty* pointer;
	typedef std::vector<pointer> _Mypoolt;

	CNXThreadPool() throw() {}
	virtual ~CNXThreadPool() throw()
	{
		Release();
	}
	// 创建/释放线程池
	int Create(unsigned int _Count)		// 工作线程个数
	{
		if (!_Myworkpool.empty())
			return NXTHREAD_POOL_RET_FAILED;
		// 要先启动任务管理器，否则创建的线程会立即结束
		int nRet = _GetTaskMan().Start();
		if (nRet != NXTHREAD_POOL_RET_SUCCEED)
			return nRet;
		AddNewThread(_Count);
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	void Release()
	{
		_GetTaskMan().Stop();
		std::for_each(_Myworkpool.begin(), _Myworkpool.end(), std::ptr_fun(NX_DestoryOne<_thread_ty>));
		_Myworkpool.clear();
		_ClearRecycle();
	}
	// 添加新工作线程
	void AddNewThread(unsigned int _Count)
	{
		for (unsigned int i=0; i<_Count; i++)
		{
			pointer _Ptr = new _thread_ty;
			// 创建线程，第一个参数为任务管理器指针，第二个参数为 _Suspend
			_Ptr->Create(&_GetTaskMan(), false);
			_Myworkpool.push_back(_Ptr);
		}
	}
	// 挂起/恢复任务管理器
	// （1）不仅会阻止所有的任务推入动作，同时会挂起所有的工作线程。
	int Suspend()
	{
		return _GetTaskMan().Suspend();
	}
	int Resume()
	{
		return _GetTaskMan().Resume();
	}
	// 重置线程池
	// （1）重新设定工作线程的个数，如果当前线程个数大于 _Count，则设置自动销毁，否则添加新线程。
	// （2）当 _Count == 0 时表示不改变已有的工作线程个数。
	// （3）重置任务管理器，清空所有剩余任务。
	int Reset(unsigned int _Count)
	{
		_ClearRecycle();	// 清除以前回收过的工作线程，释放内存空间
		if (_Count > 0)		// 为 0 时不改变工作线程个数
		{
			if (_Count > GetThreadCount())		// 添加新线程
			{
				AddNewThread(_Count - GetThreadCount());
			}
			else	// 从列表尾开始逐个销毁工作线程
			{
				while (_Count-- > GetThreadCount())
				{
					pointer _Ptr = _Myworkpool.back();
					_Ptr->AutoKill();
					_Myworkpool.pop_back();
					_Myrecycle.push_back(_Ptr);
				}
			}
		}
		if (_Myworkpool.empty())
			return NXTHREAD_POOL_RET_EMPTY;
		return _GetTaskMan().Reset();
	}
	// 推入任务（一般情况下，忽略 _Block 参数）
	int PushTask(_task_base_ty *_Ptr, bool _Block)
	{
		if (_Myworkpool.empty())
			return NXTHREAD_POOL_RET_EMPTY;
		return _GetTaskMan().PushTask(_Ptr, _Block);
	}
	// 获取当前正在工作的线程总数
	unsigned int GetThreadCount() const
	{
		return static_cast<unsigned int>(_Myworkpool.size());
	}

protected:
	CNXThreadPool(const _Myt&);
	_Myt& operator=(const _Myt&);
	// 任务管理器
	_task_man& _GetTaskMan()
	{
		return _Mytaskman;
	}
	const _task_man& _GetTaskMan() const
	{
		return _Mytaskman;
	}
	// 销毁被停止的线程
	void _ClearRecycle()
	{
		std::for_each(_Myrecycle.begin(), _Myrecycle.end(), std::ptr_fun(NX_DestoryOne<_thread_ty>));
		_Myrecycle.clear();
	}

private:
	_task_man	_Mytaskman;		// 任务管理器
	_Mypoolt	_Myworkpool;	// 工作中的线程
	_Mypoolt	_Myrecycle;		// 被停止的线程
};

//////////////////////////////////////////////////////////////////////////
// 2 维异步线程池模板

template <class _thread_ty, class _task_man, class _task_base_ty>
class CNXThreadPoolAsync2D : public CNXThreadPool<_thread_ty, _task_man, _task_base_ty>
{
public:
	typedef CNXThreadPoolAsync2D<_thread_ty, _task_man, _task_base_ty> _Myt;
	typedef CNXThreadPool<_thread_ty, _task_man, _task_base_ty> _Mybaset;

	CNXThreadPoolAsync2D() throw() {}
	virtual ~CNXThreadPoolAsync2D() throw() {}
	// 重置线程池（参见 CNXThreadPool::(unsigned int _Count)）
	// （1）重新设定工作线程的个数，如果当前线程个数大于 _Count，则设置自动销毁，否则添加新线程。
	// （2）重置任务管理器，清空所有剩余任务。
	// （3）异步机制中的 Barrier 索引被重置为 0，调用者可以通过 GetCurrBarrier() 获取。
	// 挂起/恢复任务管理器的异步处理机制
	// （1）由于异步机制的存在，将不会挂起工作线程，只阻止异步任务的推入动作。
	int SuspendAsync()
	{
		return _GetTaskMan().SuspendAsync();
	}
	int ResumeAsync()
	{
		return _GetTaskMan().ResumeAsync();
	}
	// 同步任务管理器中的异步处理机制
	// （1）_Wait_run 为 true 表示等待执行完所有剩余的异步任务，为 false 则表示清空所有剩余的异步任务。
	// （2）设置新的栅栏总数及新的 Barrier 检测阈值。
	// （3）返回新的当前 Barrier 索引。
	int Synchronize(unsigned int _Bar_count,		// 栅栏总数
					unsigned int _Bar_check,		// 栅栏放行的检测阈值
					bool _Wait_run)					// 是否等所有的任务运行结束
	{
		return _GetTaskMan().Synchronize(_Bar_count, _Bar_check, _Wait_run);
	}
	// 返回当前的栅栏值，由于异步机制的存在，在执续运转的过程中，取值可能滞后
	int GetCurrBarrier() const
	{
		return _GetTaskMan().GetCurrBarrier();
	}
	// 根据输入栅栏值获取下一个有效的栅栏值，用于异步任务的同步，同步任务直接使用 < 0 的值即可
	int GetNextBarrier(int _Barrier)
	{
		return _GetTaskMan().GetNextBarrier(_Barrier);
	}
	// 推入任务（参见 CNXThreadPool::PushTask(INXUnknownTask *_Ptr, bool _Block)）
	// （1）当任务的 Barrier 值小于任务管理器的当前 Barrier 值时，进入同步机制，忽略 _Block 参数。
	// （1）当任务的 Barrier 值大于任务管理器的 Barrier 上限时，_Block 为 true 则阻塞线程，否则立即返回，推入任务失败。
	// （1）当任务的 Barrier 值在区间内时，进入异步机制，忽略 _Block 参数。

protected:
	CNXThreadPoolAsync2D(const _Myt&);
	_Myt& operator=(const _Myt&);
};

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// End of file.