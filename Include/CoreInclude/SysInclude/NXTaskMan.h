//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 通用线程池之任务管理器
//
// Created: Zhaozhimeng @ 2010-05-20
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <algorithm>
#include <functional>
#include ".\NXTemplate.h"
#include ".\NXMultiThread.h"
#include ".\NXAsyncCounter.h"
#include ".\NXAutoLock.h"

_NXSYS_BEGIN

//////////////////////////////////////////////////////////////////////////
// 简单任务队列，先入先出

template<class _task_base_ty, class _lock_ty>
class CNXTaskQueue : public INXObject
{
public:
	typedef CNXTaskQueue<_task_base_ty, _lock_ty> _Myt;
	typedef _task_base_ty* pointer;
	typedef CNXAutoLock<_lock_ty> _Myautolockt;
	typedef nxmp::list<pointer> _Myquet;

	CNXTaskQueue() throw() {}
	~CNXTaskQueue() throw() {}
	bool PushTask(pointer _Ptr)
	{
		if (_Ptr == NULL)
			return false;
		_Myautolockt _Autolock(_GetLock());
		_GetQueue().push_back(_Ptr);
		return true;
	}
	pointer PopTask(bool &bEmpty)
	{
		_Myautolockt _Autolock(_GetLock());
		if (bEmpty = _GetQueue().empty())
			return NULL;
		pointer _Ptr = _GetQueue().front();
		_GetQueue().pop_front();
		return _Ptr;
	}
	bool IsEmpty() const
	{
		_Myautolockt _Autolock(_GetLock());
		return _GetQueue().empty();
	}
	int GetCount() const
	{
		_Myautolockt _Autolock(_GetLock());
		return static_cast<int>(_GetQueue().size());
	}

protected:
	CNXTaskQueue(const _Myt&);
	_Myt& operator=(const _Myt&);

	const _lock_ty& _GetLock() const
	{
		return _Mylock;
	}
	_lock_ty& _GetLock()
	{
		return _Mylock;
	}
	const _Myquet& _GetQueue() const
	{
		return _Myqueue;
	}
	_Myquet& _GetQueue()
	{
		return _Myqueue;
	}

private:
	_lock_ty	_Mylock;
	_Myquet		_Myqueue;
};

//////////////////////////////////////////////////////////////////////////
// 通用任务管理器

template <class _task_base_ty,
		  class _lock_ty,
		  class _event_ty,
		  class _semaphore_ty,
		  class _sleep_ty,
		  class _task_queue_ty = CNXTaskQueue<_task_base_ty, _lock_ty> >
class CNXTaskMan : public INXUnknownJob
{
public:
	typedef CNXTaskMan<_task_base_ty, _lock_ty, _event_ty, _semaphore_ty, _sleep_ty, _task_queue_ty> _Myt;
	typedef _task_base_ty* pointer;
	typedef CNXAutoLock<_lock_ty> _Myautolockt;
	typedef long _count_ty;
	typedef CNXAsyncCounter<_count_ty, _lock_ty, _event_ty> _Mycountert;

	CNXTaskMan() throw() : _Mystatus(0)
	{
		_Mytaskcountsema.Create();
		// 第一个参数为 bManualReset，第二个参数为 bInitialState
		_Myrunningevent.Create(true, true);
	}
	virtual ~CNXTaskMan() throw()
	{
		Stop();
		_Myrunningevent.Close();
		_Mytaskcountsema.Close();
	}
	// 启动/停止任务管理器
	int Start()
	{
		if (_IsAlive())
			return NXTHREAD_POOL_RET_FAILED;
		// 初始化参数
		_InitParam();
		// 启动
		_SetStatusBit(STATUS_BIT_ALIVE);
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	int Stop()
	{
		if (!_IsAlive())
			return NXTHREAD_POOL_RET_SUCCEED;
		_SetStatusBit(STATUS_BIT_ALIVE, true);
		_Resume();			// 恢复运行状态，防止之前的挂起动作导致可能的死锁
		_ActiveAll();		// 激活所有的动作（包括工作线程、推入任务等）
		_WaitNoAction();	// 等待所有的动作结束
		_ClearAllTasks();	// 清除未处理的所有任务
		// 等待所有的工作线程退出
		_Myworkers.WaitEmpty(NX_INFINITE);
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	// 挂起/恢复任务管理器
	// （1）不仅会阻止所有的任务推入动作，同时会挂起所有的工作线程。
	int Suspend()
	{
		if (!_IsAlive())
			return NXTHREAD_POOL_RET_STOP;
		_Suspend();
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	int Resume()
	{
		if (!_IsAlive())
			return NXTHREAD_POOL_RET_STOP;
		_Resume();
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	// 重置任务管理器（清空所有剩余任务）
	int Reset()
	{
		if (!_IsAlive())
			return NXTHREAD_POOL_RET_STOP;
		_Suspend();			// 先挂起所有工作线程，并阻止任务推入
		_ActiveAll();		// 激活所有的动作（包括工作线程、推入任务等）
		_WaitNoAction();	// 等待所有的动作结束
		_ClearAllTasks();	// 清除未处理的所有任务
		_ResetParam();		// 重置部分运转参数
		_Resume();			// 恢复运转状态
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	// 推入任务（忽略 _Block 参数）
	int PushTask(pointer _Ptr, bool _Block)
	{
		if (_Ptr == NULL)
			return NXTHREAD_POOL_RET_INVALID_PARAM;
		if (!_IsAlive())
			return NXTHREAD_POOL_RET_STOP;
		if (_IsSuspend())
			return NXTHREAD_POOL_RET_SUSPEND;
		_IncAction();
		int _Ret = _PushTask(_Ptr, _Block);
		_DecAction();
		// 增加总任务信标量
		if (NXTHREAD_POOL_RET_SUCCEED == _Ret)
		{
			_IncTaskSema();
		}
		return _Ret;
	}
	// --------------------------------------------------------------
	// 执行主体：扫描任务
	virtual int Run(const INXUnknownThread *_pthread)
	{
		int _Ret(1);
		++_Myworkers;	// 统计工作者
		while (_IsAlive() && (_pthread == NULL || !_pthread->IsAutoKill()))
		{
			if (_Myrunningevent.Wait(NX_INFINITE) != 0)		// 挂起状态
			{
				_Ret = 0;
				break;
			}
			// 自我销毁或线程对象终止
			if (!_IsAlive() || (_pthread != NULL && _pthread->IsAutoKill()))
				break;
			if (_Mytaskcountsema.Wait(NX_INFINITE) != 0)	// 等待任务失败
			{
				_Ret = 0;
				break;
			}
			if (!_IsAlive())
				break;
			// 线程对象终止时，需要还原任务计数
			if (_pthread != NULL && _pthread->IsAutoKill())
			{
				_IncTaskSema();
				break;
			}
			// 取出当前优先级最高的任务
			bool bEmpty(true);
			pointer _Ptr = _PopTask(bEmpty);
			if (_Ptr == NULL)
			{
				// 当任务队列不为空时，还原任务计数，并休眠交出 CPU 权限
				// 一般不会出现这种情况，但一些特殊的任务队列可能需要
				if (!bEmpty)
				{
					_sleep_ty _MySleep;
					_MySleep.Sleep(0);
					_IncTaskSema();
					_MySleep.Sleep(0);
				}
				continue;
			}
			// 执行任务
			_Ptr->Done(_Ptr->Run());
			_Ptr->DecRefCount();
		}
		--_Myworkers;	// 递减工作者
		return _Ret;
	}

protected:
	CNXTaskMan(const _Myt&);
	_Myt& operator=(const _Myt&);
	// 常量定义
	enum
	{
		STATUS_BIT_ALIVE	= 0x80000000,	// 存活位测试
		STATUS_BIT_ALL		= 0xffffffff,	// 所有位测试
	};
	// 初始化参数
	void _InitParam()
	{
		_SetStatusBit(STATUS_BIT_ALL, true);
		_Myworkers.Reset();
		_Mytaskcountsema.Reset();
		_Myactions.Reset();
		_Resume();
	}
	// 挂起/恢复
	void _Suspend()
	{
		_Myrunningevent.Reset();
	}
	void _Resume()
	{
		_Myrunningevent.Set();
	}
	// 激活所有
	void _ActiveAll()
	{
		// 激活所有的工作线程，使之正常退出
		_IncTaskSema(_Myworkers);
		_sleep_ty _MySleep;		// 切换时间片
		_MySleep.Sleep(0);
	}
	// 重置部分运转参数
	void _ResetParam()
	{
		_Mytaskcountsema.Reset();
		_Myactions.Reset();
	}
	// 动作
	void _IncAction()
	{
		++_Myactions;
	}
	void _DecAction()
	{
		--_Myactions;
	}
	void _WaitNoAction()
	{
		_Myactions.WaitEmpty(NX_INFINITE);
	}
	// 任务信标量
	void _IncTaskSema(long _Count = 1L)
	{
		_Mytaskcountsema.Release(_Count, NULL);
	}
	// 推入任务
	int _PushTask(pointer _Ptr, bool _Block)
	{
		_Ptr->IncRefCount();
		_Mysyncqueue.PushTask(_Ptr);
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	// 弹出任务
	pointer _PopTask(bool &bEmpty)
	{
		if (!_IsAlive())	// 停止后则直接返回
		{
			bEmpty = false;
			return NULL;
		}
		return _Mysyncqueue.PopTask(bEmpty);
	}
	// 清空所有的任务
	void _ClearAllTasks()
	{
		_ClearTask(&_Mysyncqueue);
	}
	// 清空某个任务队列的任务，返回清空的数量
	_count_ty _ClearTask(_task_queue_ty *_Ptr_que)
	{
		_count_ty _Count(static_cast<_count_ty>(0));
		bool bEmpty(false);
		pointer _Ptr = _Ptr_que->PopTask(bEmpty);
		while (_Ptr != NULL)	// 清除一个任务需要相应的减少一个信标量
		{
			_Count++;
			_Ptr->DecRefCount();
			_Ptr = _Ptr_que->PopTask(bEmpty);
			// 极端情况下，该动作可能会引起死锁，不过此时也说明，整个流程基本面临崩溃
			_Mytaskcountsema.Wait(NX_INFINITE);
		}
		return _Count;
	}
	// 状态操作
	void _SetStatusBit(unsigned int _Bit, bool _Clear = false)
	{
		if (_Clear)
		{
			_Mystatus &= ~_Bit;
		}
		else
		{
			_Mystatus |= _Bit;
		}
	}
	bool _TestStatusBit(unsigned int _Bit) const
	{
		return ((_Mystatus & _Bit) == _Bit);
	}
	bool _IsAlive() const
	{
		return _TestStatusBit(STATUS_BIT_ALIVE);
	}
	bool _IsSuspend() const
	{
		return (_Myrunningevent.Wait(0) != 0);
	}

private:
	_Mycountert		_Myactions;			// 动作计数器
	_task_queue_ty	_Mysyncqueue;		// 同步任务队列
	_semaphore_ty	_Mytaskcountsema;	// 任务总数信标量
	_event_ty		_Myrunningevent;	// 管理器挂起事件，手动设置
	unsigned int	_Mystatus;			// 管理器运行状态，无需加锁
	_Mycountert		_Myworkers;			// 工作者个数
};

//////////////////////////////////////////////////////////////////////////
// 栅栏元件
// 用于同步，值小于 0 表示无效的元件，要求正确 Notify() 之后不再会调用 Wait()。

template<class _lock_ty, class _event_ty>
class CNXBarrierCell : public INXObject
{
public:
	typedef CNXBarrierCell<_lock_ty, _event_ty> _Myt;
	typedef CNXAutoLock<_lock_ty> _Myautolockt;

	CNXBarrierCell() throw() : _Mykey(-1), _Myref(0)
	{
		// 第一个参数为 bManualReset，第二个参数为 bInitialState
		_Mywait.Create(true, false);
		_Myidle.Create(true, true);
	}
	~CNXBarrierCell() throw()
	{
		_Myidle.Wait(NX_INFINITE);
		_Mywait.Close();
		_Myidle.Close();
	}
	// < 0 的键值表示无效
	void SetKey(int _Key)
	{
		_Mykey = _Key;
	}
	int GetKey() const
	{
		return _Mykey;
	}
	bool IsHit(int _Barrier) const
	{
		return (_Barrier == GetKey());
	}
	// 判断元件是否闲置
	bool IsIdle() const
	{
		_Myautolockt _Autolock(_Mylock);
		return _IsIdle();
	}
	// 等待前增加引用计数
	void PreWait()
	{
		_IncRef();
	}
	void Wait(unsigned long _Milliseconds)
	{
		_Mywait.Wait(_Milliseconds);
		_DecRef();
	}
	// 激活，_Key < 0 表示完全激活
	void Notify(int _Key)
	{
		if (_Key < 0 || _Key >= GetKey())
		{
			_Mywait.Set();
		}
	}
	// 重置
	void Reset()
	{
		_Mywait.Reset();
		SetKey(-1);
		_SetRef(0);
	}

protected:
	CNXBarrierCell(const _Myt&);
	_Myt& operator=(const _Myt&);
	void _SetRef(int _Count)
	{
		_Myref = _Count;
	}
	int _GetRef() const
	{
		return _Myref;
	}
	bool _IsIdle() const
	{
		return (_GetRef() <= 0);
	}
	void _IncRef()
	{
		_Myautolockt _Autolock(_Mylock);
		_SetRef(_GetRef() + 1);
		if (!_IsIdle())
		{
			_Myidle.Reset();
		}
	}
	void _DecRef()
	{
		_Myautolockt _Autolock(_Mylock);
		_SetRef(_GetRef() - 1);
		if (_IsIdle())
		{
			_Myidle.Set();
		}
	}

private:
	_lock_ty		_Mylock;
	int				_Myref;
	int				_Mykey;
	_event_ty		_Mywait;	// 等待事件
	_event_ty		_Myidle;	// 空闲事件
};

//////////////////////////////////////////////////////////////////////////
// 栅栏管理

template <class _lock_ty, class _event_ty>
class CNXBarrierMan
{
public:
	typedef CNXBarrierMan<_lock_ty, _event_ty> _Myt;
	typedef CNXBarrierCell<_lock_ty, _event_ty> _Mybarriert;
	typedef _Mybarriert* pointer;
	typedef nxmp::list<pointer> _Mybarrierlistt;
	typedef typename _Mybarrierlistt::iterator iterator;

	CNXBarrierMan() throw() : _Mykey(0) {}
	~CNXBarrierMan() throw()
	{
		Release();
	}
	// 释放
	void Release()
	{
		NotifyAll();
		_LockList();
		std::for_each(_Myvalid.begin(), _Myvalid.end(), std::ptr_fun(NX_DestoryOne<_Mybarriert>));
		_Myvalid.clear();
		std::for_each(_Mypool.begin(), _Mypool.end(), std::ptr_fun(NX_DestoryOne<_Mybarriert>));
		_Mypool.clear();
		_UnlockList();
	}
	// 阻塞某个栅栏
	void Block(int _Barrier)
	{
		pointer _Ptr(NULL);
		_LockList();
		if (_Barrier > _GetKey() && _GetKey() >= 0)		// 需要阻塞
		{
			// 先搜索元件是否已存在
			for (iterator _iter=_Myvalid.begin(); _iter!=_Myvalid.end(); )
			{
				if ((*_iter)->IsHit(_Barrier))
				{
					_Ptr = *_iter;
					break;
				}
				else if ((*_iter)->IsIdle())	// 否则移除无效的元件
				{
					_Recycle(*_iter);
					_iter = _Myvalid.erase(_iter);
					continue;
				}
				_iter++;
			}
			if (_Ptr == NULL)		// 未找到对应的元件，则申请新的
			{
				_Ptr = _Allocate();
				_Ptr->SetKey(_Barrier);
				_Myvalid.push_back(_Ptr);
			}
			_Ptr->PreWait();	// 预先增加引用
		}
		_UnlockList();		// 此处必须解锁
		if (_Ptr)	// 阻塞
		{
			_Ptr->Wait(NX_INFINITE);
		}
	}
	// 通知元件
	void NotifyAll()
	{
		_LockList();
		std::for_each(_Myvalid.begin(), _Myvalid.end(), std::bind2nd(std::mem_fun(&(_Mybarriert::Notify)), (int)-1));
		_UnlockList();
	}
	void Notify(int _Key)
	{
		if (_Key < _GetKey())
			return;
		// 通知元件
		_LockList();
		_SetKey(_Key);
		std::for_each(_Myvalid.begin(), _Myvalid.end(), std::bind2nd(std::mem_fun(&(_Mybarriert::Notify)), _Key));
		_UnlockList();
	}
	// 重置
	void Reset()
	{
		NotifyAll();
		_LockList();
		_SetKey(0);
		_UnlockList();
	}

protected:
	CNXBarrierMan(const _Myt&);
	_Myt& operator=(const _Myt&);
	void _LockList() const
	{
		_Mylock.Lock();
	}
	void _UnlockList() const
	{
		_Mylock.Unlock();
	}
	void _Recycle(pointer _Ptr)
	{
		_Ptr->Reset();
		_Mypool.push_back(_Ptr);
	}
	pointer _Allocate()
	{
		if (_Mypool.empty())	// 回收
		{
			for (iterator _iter=_Myvalid.begin(); _iter!=_Myvalid.end(); )
			{
				if ((*_iter)->IsIdle())	// 移除无效的元件
				{
					_Recycle(*_iter);
					_iter = _Myvalid.erase(_iter);
					continue;
				}
				_iter++;
			}
		}
		if (_Mypool.empty())	// 仍为空则申请
			return (new _Mybarriert);
		pointer _Ptr = _Mypool.back();
		_Mypool.pop_back();
		return _Ptr;
	}
	// key 操作
	void _SetKey(int _Key)
	{
		_Mykey = _Key;
	}
	int _GetKey() const
	{
		return _Mykey;
	}

private:
	_Mybarrierlistt		_Myvalid;
	_Mybarrierlistt		_Mypool;
	_lock_ty			_Mylock;
	int					_Mykey;
};

//////////////////////////////////////////////////////////////////////////
// 2 维异步任务管理器

template <class _task_base_ty,
		  class _lock_ty,
		  class _event_ty,
		  class _semaphore_ty,
		  class _sleep_ty,
		  class _task_queue_ty = CNXTaskQueue<_task_base_ty, _lock_ty> >
class CNXTaskManAsync2D : public CNXTaskMan<_task_base_ty, _lock_ty, _event_ty, _semaphore_ty, _sleep_ty, _task_queue_ty>
{
public:
	typedef CNXTaskManAsync2D<_task_base_ty, _lock_ty, _event_ty, _semaphore_ty, _sleep_ty, _task_queue_ty> _Myt;
	typedef CNXTaskMan<_task_base_ty, _lock_ty, _event_ty, _semaphore_ty, _sleep_ty, _task_queue_ty> _Mybaset;
	typedef CNXBarrierMan<_lock_ty, _event_ty> _Mybarriermant;
	typedef typename _Mybaset::_Mycountert _Mycountert;
	typedef typename _Mybaset::pointer pointer;
	typedef typename _Mybaset::_Myautolockt _Myautolockt;
	typedef nxmp::vector<_task_queue_ty*> _Mytaskmatt;

	CNXTaskManAsync2D() throw()
		: _Mybaset(), _Mybarcount(0), _Mycurrbar(0), _Mybarcheck(0)
	{
	}
	virtual ~CNXTaskManAsync2D() throw()
	{
		Stop();
		_SetBarrierCount(0);	// 释放异步任务队列资源
		_Mybarman.Release();
	}
	// 挂起/恢复异步任务处理机制
	// （1）由于异步机制的存在，将不会挂起工作线程，只阻止异步任务的推入动作。
	int SuspendAsync()
	{
		if (!_IsAlive())
			return NXTHREAD_POOL_RET_STOP;
		_SetStatusBit(STATUS_BIT_ASYNC_SUSPEND);
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	int ResumeAsync()
	{
		if (!_IsAlive())
			return NXTHREAD_POOL_RET_STOP;
		_SetStatusBit(STATUS_BIT_ASYNC_SUSPEND, true);
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	// 同步异步处理机制
	// （1）_Wait_run 为 true 表示等待执行完所有剩余的异步任务，为 false 则表示清空所有剩余的异步任务。
	// （2）设置新的栅栏总数及新的 Barrier 检测阈值。
	// （3）返回新的当前 Barrier 索引。
	int Synchronize(unsigned int _Bar_count,	// 栅栏总数
					unsigned int _Bar_check,	// 栅栏放行的检测阈值
					bool _Wait_run)				// 是否等所有的任务运行结束
	{
		// 挂起异步机制
		int _Ret = SuspendAsync();
		if (_Ret != NXTHREAD_POOL_RET_SUCCEED)
			return _Ret;
		if (!_Wait_run)	// 不等待则清除未执行的任务
		{
			_ClearAsyncTasks();
		}
		// 等待所有的异步任务执行结束
		_WaitNoAction();
		_Myasynctasks.WaitEmpty(NX_INFINITE);
		// 设置 Barrier 相关参数
		_SetBarrierCount(_Bar_count);
		_SetBarrierCheck(static_cast<int>(_Bar_check));
		_ResetAsyncParam();
		// 恢复异步机制
		ResumeAsync();
		return _GetCurrBarrier();
	}
	// 返回当前的栅栏值，由于异步机制的存在，在执续运转的过程中，取值可能滞后
	int GetCurrBarrier() const
	{
		return _GetCurrBarrier();
	}
	// 根据输入栅栏值获取下一个有效的栅栏值，用于异步任务的同步，同步任务直接使用 < 0 的值即可
	int GetNextBarrier(int _Barrier)
	{
		if (_Barrier < 0)
			return _Barrier;
		return (_Barrier + 1);
	}
	// 推入任务（参见 CNXTaskMan::PushTask(pointer _Ptr, bool _Block)）
	// 当任务的 Barrier 值小于任务管理器的当前 Barrier 值时，进入同步机制，忽略 _Block 参数。
	// 当任务的 Barrier 值大于任务管理器的 Barrier 上限时，_Block 为 true 则阻塞线程，否则立即返回，推入任务失败。
	// 当任务的 Barrier 值在区间内时，进入异步机制，忽略 _Block 参数。

protected:
	CNXTaskManAsync2D(const _Myt&);
	_Myt& operator=(const _Myt&);
	// 常量定义
	enum
	{
		STATUS_BIT_ASYNC_SUSPEND	= 0x08000000,	// 异步挂起位测试
	};
	// 初始化参数
	void _InitParam()
	{
		_ResetAsyncParam();
		_SetBarrierCount(0);
		_Mybaset::_InitParam();
	}
	// 激活所有
	void _ActiveAll()
	{
		// 激活正在阻塞的异步任务推入动作
		_Mybarman.NotifyAll();
		_Mybaset::_ActiveAll();
	}
	// 重置部分运转参数
	void _ResetParam()
	{
		_ResetAsyncParam();
		_Mybaset::_ResetParam();
	}
	void _ResetAsyncParam()
	{
		_SetCurrBarrier(0);
		_Mybarman.Reset();
		_Myasynctasks.Reset();
	}
	// 异步矩阵
	const _task_queue_ty* _GetAsyncQueue(int i) const
	{
		return _Myasyncmat[i];
	}
	_task_queue_ty* _GetAsyncQueue(int i)
	{
		return _Myasyncmat[i];
	}
	int _GetBarrierCount() const
	{
		return _Mybarcount;
	}
	void _SetBarrierCount(unsigned int _Bar_count)
	{
		_Mybarcount = static_cast<int>(_Bar_count);
		// 补充任务队列
		for (int i=static_cast<int>(_Myasyncmat.size()); i<_GetBarrierCount(); i++)
		{
			_Myasyncmat.push_back(new _task_queue_ty);
		}
		// 清除多余的任务队列
		for (int i=static_cast<int>(_Myasyncmat.size()); i>_GetBarrierCount(); i--)
		{
			delete _Myasyncmat.back();
			_Myasyncmat.pop_back();
		}
		_Myruncount.assign(_Bar_count, 0);
	}
	void _LockBarrier() const
	{
		_Mybarrierlock.Lock();
	}
	void _UnlockBarrier() const
	{
		_Mybarrierlock.Unlock();
	}
	// Barrier 计算
	void _SetCurrBarrier(int _Barrier)
	{
		_Mycurrbar = _Barrier;
	}
	int _GetCurrBarrier() const
	{
		return _Mycurrbar;
	}
	int _GetValidBarrierMax() const
	{
		return (_GetCurrBarrier() + _GetBarrierCount() - 1);
	}
	int _CalcBarrierCol(int _Barrier)
	{
		return (_Barrier % _GetBarrierCount());
	}
	// Barrier Check
	void _SetBarrierCheck(int _Check)
	{
		_Mybarcheck = _Check;
	}
	int _GetBarrierCheck() const
	{
		return _Mybarcheck;
	}
	// 任务执行数
	void _IncRunCount(int _Bar_col)
	{
		_Myruncount[_Bar_col]++;
	}
	int _GetRunCount(int _Bar_col) const
	{
		return _Myruncount[_Bar_col];
	}
	void _SetRunCount(int _Bar_col, int _Count)
	{
		_Myruncount[_Bar_col] = _Count;
	}
	// 清除所有任务
	void _ClearAllTasks()
	{
		_ClearAsyncTasks();
		_Mybaset::_ClearAllTasks();
	}
	void _ClearAsyncTasks()
	{
		_LockBarrier();
		for (int i=0; i<_GetBarrierCount(); i++)
		{
			_Myasynctasks -= _ClearTask(_GetAsyncQueue(_CalcBarrierCol(i + _GetCurrBarrier())));
		}
		_UnlockBarrier();
	}
	// 推入任务
	int _PushTask(pointer _Ptr, bool _Block)
	{
		if (_IsTaskAsync(_Ptr->GetBarrier()))
		{
			// 异步任务处理机制被挂起
			if (_IsAsyncSuspend())
				return NXTHREAD_POOL_RET_HANG;
			// 异步任务不在当前处理范围
			if (_IsTaskBlock(_Ptr->GetBarrier()))
			{
				if (!_Block)	// 不等待则直接返回
					return NXTHREAD_POOL_RET_BLOCK;
				// 等待栅栏有效
				_Mybarman.Block(_Ptr->GetBarrier());
				if (!_IsAlive())
					return NXTHREAD_POOL_RET_STOP;
			}
			// 加入异步任务矩阵
			_Ptr->IncRefCount();
			_LockBarrier();
			++_Myasynctasks;
			_GetAsyncQueue(_CalcBarrierCol(_Ptr->GetBarrier()))->PushTask(_Ptr);
			_UnlockBarrier();
			return NXTHREAD_POOL_RET_SUCCEED;
		}
		return _Mybaset::_PushTask(_Ptr, _Block);
	}
	// 弹出任务
	pointer _PopTask(bool &bEmpty)
	{
		if (!_IsAlive())	// 停止后则直接返回
		{
			bEmpty = false;
			return NULL;
		}
		pointer _Ptr = _Mybaset::_PopTask(bEmpty);
		if (_Ptr != NULL)		// 优先执行同步任务
			return _Ptr;
		_IncAction();	// 增加动作
		int _Col(-1), i(-1);
		// 异步任务则按栅栏的优先级进行排序
		bEmpty = true;
		_LockBarrier();
		for (i=0; i<_GetBarrierCount(); i++)
		{
			_Col = _CalcBarrierCol(_GetCurrBarrier() + i);
			bool bOneEmpty(true);
			_Ptr = _GetAsyncQueue(_Col)->PopTask(bOneEmpty);
			bEmpty &= bOneEmpty;
			if (_Ptr != NULL)
			{
				_IncRunCount(_Col);
				break;
			}
		}
		// 依次激活正在等待的栅栏
		while (i == 0
			&& _GetRunCount(_Col) >= _GetBarrierCheck()
			&& _GetAsyncQueue(_Col)->IsEmpty())
		{
			_SetRunCount(_Col, 0);		// 清空任务执行数
			_SetCurrBarrier(_GetCurrBarrier() + 1);		// 推动栅栏
			_Mybarman.Notify(_GetValidBarrierMax());	// 激活阻塞的栅栏
			_Col = _CalcBarrierCol(_GetCurrBarrier());	// 探测下一个栅栏
		}
		_UnlockBarrier();
		if (_Ptr != NULL)		// 减少异步任务计数
		{
			--_Myasynctasks;
		}
		_DecAction();	// 减少动作
		return _Ptr;
	}
	// 状态操作
	bool _IsAsyncSuspend() const
	{
		return _TestStatusBit(STATUS_BIT_ASYNC_SUSPEND);
	}
	// 任务特性操作
	bool _IsTaskAsync(int _Barrier) const
	{
		_Myautolockt _Autolock(_Mybarrierlock);
		return (_GetBarrierCount() > 0 && _Barrier >= _GetCurrBarrier());
	}
	bool _IsTaskBlock(int _Barrier) const
	{
		_Myautolockt _Autolock(_Mybarrierlock);
		return (_Barrier > _GetValidBarrierMax());
	}

private:
	// 异步任务矩阵相关参数
	// 矩阵的行表示栅栏前进一步所需要执行的同列任务数，
	// 即栅栏所在列的每行都有任务执行后，且该列的任务数为空，栅栏前进一步。
	_Mytaskmatt		_Myasyncmat;		// 异步任务矩阵，每个任务队列组成一列
	_lock_ty		_Mybarrierlock;		// 栅栏锁
	_Mybarriermant	_Mybarman;			// 栅栏控制器
	int				_Mybarcount;		// 栅栏总数
	int				_Mycurrbar;			// 当前栅栏位置，从 0 开始，逐步递增，对栅栏总数求余后即得栅栏所在的任务队列索引
	int				_Mybarcheck;		// 栅栏前移的检测阈值，同时是矩阵的行高，即当前栅栏所指向的任务队列需要执行的任务个数
	nxmp::vector<int>	_Myruncount;	// 异步任务矩阵中每个任务队列在一个循环周期中所执行过的任务数量
	_Mycountert		_Myasynctasks;		// 异步任务计数器
	// 同步任务队列，当任务的 Barrier < 0 时推入基类任务队列
	// 弹出任务时，优先级分别为：同步任务队列、异步任务矩阵
};

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// End of file.