//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ͨ���̳߳�֮���������
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
// ��������У������ȳ�

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
// ͨ�����������

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
		// ��һ������Ϊ bManualReset���ڶ�������Ϊ bInitialState
		_Myrunningevent.Create(true, true);
	}
	virtual ~CNXTaskMan() throw()
	{
		Stop();
		_Myrunningevent.Close();
		_Mytaskcountsema.Close();
	}
	// ����/ֹͣ���������
	int Start()
	{
		if (_IsAlive())
			return NXTHREAD_POOL_RET_FAILED;
		// ��ʼ������
		_InitParam();
		// ����
		_SetStatusBit(STATUS_BIT_ALIVE);
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	int Stop()
	{
		if (!_IsAlive())
			return NXTHREAD_POOL_RET_SUCCEED;
		_SetStatusBit(STATUS_BIT_ALIVE, true);
		_Resume();			// �ָ�����״̬����ֹ֮ǰ�Ĺ��������¿��ܵ�����
		_ActiveAll();		// �������еĶ��������������̡߳���������ȣ�
		_WaitNoAction();	// �ȴ����еĶ�������
		_ClearAllTasks();	// ���δ�������������
		// �ȴ����еĹ����߳��˳�
		_Myworkers.WaitEmpty(NX_INFINITE);
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	// ����/�ָ����������
	// ��1����������ֹ���е��������붯����ͬʱ��������еĹ����̡߳�
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
	// ����������������������ʣ������
	int Reset()
	{
		if (!_IsAlive())
			return NXTHREAD_POOL_RET_STOP;
		_Suspend();			// �ȹ������й����̣߳�����ֹ��������
		_ActiveAll();		// �������еĶ��������������̡߳���������ȣ�
		_WaitNoAction();	// �ȴ����еĶ�������
		_ClearAllTasks();	// ���δ�������������
		_ResetParam();		// ���ò�����ת����
		_Resume();			// �ָ���ת״̬
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	// �������񣨺��� _Block ������
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
		// �����������ű���
		if (NXTHREAD_POOL_RET_SUCCEED == _Ret)
		{
			_IncTaskSema();
		}
		return _Ret;
	}
	// --------------------------------------------------------------
	// ִ�����壺ɨ������
	virtual int Run(const INXUnknownThread *_pthread)
	{
		int _Ret(1);
		++_Myworkers;	// ͳ�ƹ�����
		while (_IsAlive() && (_pthread == NULL || !_pthread->IsAutoKill()))
		{
			if (_Myrunningevent.Wait(NX_INFINITE) != 0)		// ����״̬
			{
				_Ret = 0;
				break;
			}
			// �������ٻ��̶߳�����ֹ
			if (!_IsAlive() || (_pthread != NULL && _pthread->IsAutoKill()))
				break;
			if (_Mytaskcountsema.Wait(NX_INFINITE) != 0)	// �ȴ�����ʧ��
			{
				_Ret = 0;
				break;
			}
			if (!_IsAlive())
				break;
			// �̶߳�����ֹʱ����Ҫ��ԭ�������
			if (_pthread != NULL && _pthread->IsAutoKill())
			{
				_IncTaskSema();
				break;
			}
			// ȡ����ǰ���ȼ���ߵ�����
			bool bEmpty(true);
			pointer _Ptr = _PopTask(bEmpty);
			if (_Ptr == NULL)
			{
				// ��������в�Ϊ��ʱ����ԭ��������������߽��� CPU Ȩ��
				// һ�㲻����������������һЩ�����������п�����Ҫ
				if (!bEmpty)
				{
					_sleep_ty _MySleep;
					_MySleep.Sleep(0);
					_IncTaskSema();
					_MySleep.Sleep(0);
				}
				continue;
			}
			// ִ������
			_Ptr->Done(_Ptr->Run());
			_Ptr->DecRefCount();
		}
		--_Myworkers;	// �ݼ�������
		return _Ret;
	}

protected:
	CNXTaskMan(const _Myt&);
	_Myt& operator=(const _Myt&);
	// ��������
	enum
	{
		STATUS_BIT_ALIVE	= 0x80000000,	// ���λ����
		STATUS_BIT_ALL		= 0xffffffff,	// ����λ����
	};
	// ��ʼ������
	void _InitParam()
	{
		_SetStatusBit(STATUS_BIT_ALL, true);
		_Myworkers.Reset();
		_Mytaskcountsema.Reset();
		_Myactions.Reset();
		_Resume();
	}
	// ����/�ָ�
	void _Suspend()
	{
		_Myrunningevent.Reset();
	}
	void _Resume()
	{
		_Myrunningevent.Set();
	}
	// ��������
	void _ActiveAll()
	{
		// �������еĹ����̣߳�ʹ֮�����˳�
		_IncTaskSema(_Myworkers);
		_sleep_ty _MySleep;		// �л�ʱ��Ƭ
		_MySleep.Sleep(0);
	}
	// ���ò�����ת����
	void _ResetParam()
	{
		_Mytaskcountsema.Reset();
		_Myactions.Reset();
	}
	// ����
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
	// �����ű���
	void _IncTaskSema(long _Count = 1L)
	{
		_Mytaskcountsema.Release(_Count, NULL);
	}
	// ��������
	int _PushTask(pointer _Ptr, bool _Block)
	{
		_Ptr->IncRefCount();
		_Mysyncqueue.PushTask(_Ptr);
		return NXTHREAD_POOL_RET_SUCCEED;
	}
	// ��������
	pointer _PopTask(bool &bEmpty)
	{
		if (!_IsAlive())	// ֹͣ����ֱ�ӷ���
		{
			bEmpty = false;
			return NULL;
		}
		return _Mysyncqueue.PopTask(bEmpty);
	}
	// ������е�����
	void _ClearAllTasks()
	{
		_ClearTask(&_Mysyncqueue);
	}
	// ���ĳ��������е����񣬷�����յ�����
	_count_ty _ClearTask(_task_queue_ty *_Ptr_que)
	{
		_count_ty _Count(static_cast<_count_ty>(0));
		bool bEmpty(false);
		pointer _Ptr = _Ptr_que->PopTask(bEmpty);
		while (_Ptr != NULL)	// ���һ��������Ҫ��Ӧ�ļ���һ���ű���
		{
			_Count++;
			_Ptr->DecRefCount();
			_Ptr = _Ptr_que->PopTask(bEmpty);
			// ��������£��ö������ܻ�����������������ʱҲ˵�����������̻������ٱ���
			_Mytaskcountsema.Wait(NX_INFINITE);
		}
		return _Count;
	}
	// ״̬����
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
	_Mycountert		_Myactions;			// ����������
	_task_queue_ty	_Mysyncqueue;		// ͬ���������
	_semaphore_ty	_Mytaskcountsema;	// ���������ű���
	_event_ty		_Myrunningevent;	// �����������¼����ֶ�����
	unsigned int	_Mystatus;			// ����������״̬���������
	_Mycountert		_Myworkers;			// �����߸���
};

//////////////////////////////////////////////////////////////////////////
// դ��Ԫ��
// ����ͬ����ֵС�� 0 ��ʾ��Ч��Ԫ����Ҫ����ȷ Notify() ֮���ٻ���� Wait()��

template<class _lock_ty, class _event_ty>
class CNXBarrierCell : public INXObject
{
public:
	typedef CNXBarrierCell<_lock_ty, _event_ty> _Myt;
	typedef CNXAutoLock<_lock_ty> _Myautolockt;

	CNXBarrierCell() throw() : _Mykey(-1), _Myref(0)
	{
		// ��һ������Ϊ bManualReset���ڶ�������Ϊ bInitialState
		_Mywait.Create(true, false);
		_Myidle.Create(true, true);
	}
	~CNXBarrierCell() throw()
	{
		_Myidle.Wait(NX_INFINITE);
		_Mywait.Close();
		_Myidle.Close();
	}
	// < 0 �ļ�ֵ��ʾ��Ч
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
	// �ж�Ԫ���Ƿ�����
	bool IsIdle() const
	{
		_Myautolockt _Autolock(_Mylock);
		return _IsIdle();
	}
	// �ȴ�ǰ�������ü���
	void PreWait()
	{
		_IncRef();
	}
	void Wait(unsigned long _Milliseconds)
	{
		_Mywait.Wait(_Milliseconds);
		_DecRef();
	}
	// ���_Key < 0 ��ʾ��ȫ����
	void Notify(int _Key)
	{
		if (_Key < 0 || _Key >= GetKey())
		{
			_Mywait.Set();
		}
	}
	// ����
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
	_event_ty		_Mywait;	// �ȴ��¼�
	_event_ty		_Myidle;	// �����¼�
};

//////////////////////////////////////////////////////////////////////////
// դ������

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
	// �ͷ�
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
	// ����ĳ��դ��
	void Block(int _Barrier)
	{
		pointer _Ptr(NULL);
		_LockList();
		if (_Barrier > _GetKey() && _GetKey() >= 0)		// ��Ҫ����
		{
			// ������Ԫ���Ƿ��Ѵ���
			for (iterator _iter=_Myvalid.begin(); _iter!=_Myvalid.end(); )
			{
				if ((*_iter)->IsHit(_Barrier))
				{
					_Ptr = *_iter;
					break;
				}
				else if ((*_iter)->IsIdle())	// �����Ƴ���Ч��Ԫ��
				{
					_Recycle(*_iter);
					_iter = _Myvalid.erase(_iter);
					continue;
				}
				_iter++;
			}
			if (_Ptr == NULL)		// δ�ҵ���Ӧ��Ԫ�����������µ�
			{
				_Ptr = _Allocate();
				_Ptr->SetKey(_Barrier);
				_Myvalid.push_back(_Ptr);
			}
			_Ptr->PreWait();	// Ԥ����������
		}
		_UnlockList();		// �˴��������
		if (_Ptr)	// ����
		{
			_Ptr->Wait(NX_INFINITE);
		}
	}
	// ֪ͨԪ��
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
		// ֪ͨԪ��
		_LockList();
		_SetKey(_Key);
		std::for_each(_Myvalid.begin(), _Myvalid.end(), std::bind2nd(std::mem_fun(&(_Mybarriert::Notify)), _Key));
		_UnlockList();
	}
	// ����
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
		if (_Mypool.empty())	// ����
		{
			for (iterator _iter=_Myvalid.begin(); _iter!=_Myvalid.end(); )
			{
				if ((*_iter)->IsIdle())	// �Ƴ���Ч��Ԫ��
				{
					_Recycle(*_iter);
					_iter = _Myvalid.erase(_iter);
					continue;
				}
				_iter++;
			}
		}
		if (_Mypool.empty())	// ��Ϊ��������
			return (new _Mybarriert);
		pointer _Ptr = _Mypool.back();
		_Mypool.pop_back();
		return _Ptr;
	}
	// key ����
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
// 2 ά�첽���������

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
		_SetBarrierCount(0);	// �ͷ��첽���������Դ
		_Mybarman.Release();
	}
	// ����/�ָ��첽���������
	// ��1�������첽���ƵĴ��ڣ�������������̣߳�ֻ��ֹ�첽��������붯����
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
	// ͬ���첽�������
	// ��1��_Wait_run Ϊ true ��ʾ�ȴ�ִ��������ʣ����첽����Ϊ false ���ʾ�������ʣ����첽����
	// ��2�������µ�դ���������µ� Barrier �����ֵ��
	// ��3�������µĵ�ǰ Barrier ������
	int Synchronize(unsigned int _Bar_count,	// դ������
					unsigned int _Bar_check,	// դ�����еļ����ֵ
					bool _Wait_run)				// �Ƿ�����е��������н���
	{
		// �����첽����
		int _Ret = SuspendAsync();
		if (_Ret != NXTHREAD_POOL_RET_SUCCEED)
			return _Ret;
		if (!_Wait_run)	// ���ȴ������δִ�е�����
		{
			_ClearAsyncTasks();
		}
		// �ȴ����е��첽����ִ�н���
		_WaitNoAction();
		_Myasynctasks.WaitEmpty(NX_INFINITE);
		// ���� Barrier ��ز���
		_SetBarrierCount(_Bar_count);
		_SetBarrierCheck(static_cast<int>(_Bar_check));
		_ResetAsyncParam();
		// �ָ��첽����
		ResumeAsync();
		return _GetCurrBarrier();
	}
	// ���ص�ǰ��դ��ֵ�������첽���ƵĴ��ڣ���ִ����ת�Ĺ����У�ȡֵ�����ͺ�
	int GetCurrBarrier() const
	{
		return _GetCurrBarrier();
	}
	// ��������դ��ֵ��ȡ��һ����Ч��դ��ֵ�������첽�����ͬ����ͬ������ֱ��ʹ�� < 0 ��ֵ����
	int GetNextBarrier(int _Barrier)
	{
		if (_Barrier < 0)
			return _Barrier;
		return (_Barrier + 1);
	}
	// �������񣨲μ� CNXTaskMan::PushTask(pointer _Ptr, bool _Block)��
	// ������� Barrier ֵС������������ĵ�ǰ Barrier ֵʱ������ͬ�����ƣ����� _Block ������
	// ������� Barrier ֵ��������������� Barrier ����ʱ��_Block Ϊ true �������̣߳������������أ���������ʧ�ܡ�
	// ������� Barrier ֵ��������ʱ�������첽���ƣ����� _Block ������

protected:
	CNXTaskManAsync2D(const _Myt&);
	_Myt& operator=(const _Myt&);
	// ��������
	enum
	{
		STATUS_BIT_ASYNC_SUSPEND	= 0x08000000,	// �첽����λ����
	};
	// ��ʼ������
	void _InitParam()
	{
		_ResetAsyncParam();
		_SetBarrierCount(0);
		_Mybaset::_InitParam();
	}
	// ��������
	void _ActiveAll()
	{
		// ���������������첽�������붯��
		_Mybarman.NotifyAll();
		_Mybaset::_ActiveAll();
	}
	// ���ò�����ת����
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
	// �첽����
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
		// �����������
		for (int i=static_cast<int>(_Myasyncmat.size()); i<_GetBarrierCount(); i++)
		{
			_Myasyncmat.push_back(new _task_queue_ty);
		}
		// ���������������
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
	// Barrier ����
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
	// ����ִ����
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
	// �����������
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
	// ��������
	int _PushTask(pointer _Ptr, bool _Block)
	{
		if (_IsTaskAsync(_Ptr->GetBarrier()))
		{
			// �첽��������Ʊ�����
			if (_IsAsyncSuspend())
				return NXTHREAD_POOL_RET_HANG;
			// �첽�����ڵ�ǰ����Χ
			if (_IsTaskBlock(_Ptr->GetBarrier()))
			{
				if (!_Block)	// ���ȴ���ֱ�ӷ���
					return NXTHREAD_POOL_RET_BLOCK;
				// �ȴ�դ����Ч
				_Mybarman.Block(_Ptr->GetBarrier());
				if (!_IsAlive())
					return NXTHREAD_POOL_RET_STOP;
			}
			// �����첽�������
			_Ptr->IncRefCount();
			_LockBarrier();
			++_Myasynctasks;
			_GetAsyncQueue(_CalcBarrierCol(_Ptr->GetBarrier()))->PushTask(_Ptr);
			_UnlockBarrier();
			return NXTHREAD_POOL_RET_SUCCEED;
		}
		return _Mybaset::_PushTask(_Ptr, _Block);
	}
	// ��������
	pointer _PopTask(bool &bEmpty)
	{
		if (!_IsAlive())	// ֹͣ����ֱ�ӷ���
		{
			bEmpty = false;
			return NULL;
		}
		pointer _Ptr = _Mybaset::_PopTask(bEmpty);
		if (_Ptr != NULL)		// ����ִ��ͬ������
			return _Ptr;
		_IncAction();	// ���Ӷ���
		int _Col(-1), i(-1);
		// �첽������դ�������ȼ���������
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
		// ���μ������ڵȴ���դ��
		while (i == 0
			&& _GetRunCount(_Col) >= _GetBarrierCheck()
			&& _GetAsyncQueue(_Col)->IsEmpty())
		{
			_SetRunCount(_Col, 0);		// �������ִ����
			_SetCurrBarrier(_GetCurrBarrier() + 1);		// �ƶ�դ��
			_Mybarman.Notify(_GetValidBarrierMax());	// ����������դ��
			_Col = _CalcBarrierCol(_GetCurrBarrier());	// ̽����һ��դ��
		}
		_UnlockBarrier();
		if (_Ptr != NULL)		// �����첽�������
		{
			--_Myasynctasks;
		}
		_DecAction();	// ���ٶ���
		return _Ptr;
	}
	// ״̬����
	bool _IsAsyncSuspend() const
	{
		return _TestStatusBit(STATUS_BIT_ASYNC_SUSPEND);
	}
	// �������Բ���
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
	// �첽���������ز���
	// ������б�ʾդ��ǰ��һ������Ҫִ�е�ͬ����������
	// ��դ�������е�ÿ�ж�������ִ�к��Ҹ��е�������Ϊ�գ�դ��ǰ��һ����
	_Mytaskmatt		_Myasyncmat;		// �첽�������ÿ������������һ��
	_lock_ty		_Mybarrierlock;		// դ����
	_Mybarriermant	_Mybarman;			// դ��������
	int				_Mybarcount;		// դ������
	int				_Mycurrbar;			// ��ǰդ��λ�ã��� 0 ��ʼ���𲽵�������դ����������󼴵�դ�����ڵ������������
	int				_Mybarcheck;		// դ��ǰ�Ƶļ����ֵ��ͬʱ�Ǿ�����иߣ�����ǰդ����ָ������������Ҫִ�е��������
	nxmp::vector<int>	_Myruncount;	// �첽���������ÿ�����������һ��ѭ����������ִ�й�����������
	_Mycountert		_Myasynctasks;		// �첽���������
	// ͬ��������У�������� Barrier < 0 ʱ��������������
	// ��������ʱ�����ȼ��ֱ�Ϊ��ͬ��������С��첽�������
};

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// End of file.