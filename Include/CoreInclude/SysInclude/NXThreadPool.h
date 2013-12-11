//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ͨ���̳߳�
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
// ͨ���̳߳�ģ��

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
	// ����/�ͷ��̳߳�
	int Create(unsigned int _Count)		// �����̸߳���
	{
		if (!_Myworkpool.empty())
			return NXTHREAD_POOL_RET_FAILED;
		// Ҫ��������������������򴴽����̻߳���������
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
	// ����¹����߳�
	void AddNewThread(unsigned int _Count)
	{
		for (unsigned int i=0; i<_Count; i++)
		{
			pointer _Ptr = new _thread_ty;
			// �����̣߳���һ������Ϊ���������ָ�룬�ڶ�������Ϊ _Suspend
			_Ptr->Create(&_GetTaskMan(), false);
			_Myworkpool.push_back(_Ptr);
		}
	}
	// ����/�ָ����������
	// ��1����������ֹ���е��������붯����ͬʱ��������еĹ����̡߳�
	int Suspend()
	{
		return _GetTaskMan().Suspend();
	}
	int Resume()
	{
		return _GetTaskMan().Resume();
	}
	// �����̳߳�
	// ��1�������趨�����̵߳ĸ����������ǰ�̸߳������� _Count���������Զ����٣�����������̡߳�
	// ��2���� _Count == 0 ʱ��ʾ���ı����еĹ����̸߳�����
	// ��3������������������������ʣ������
	int Reset(unsigned int _Count)
	{
		_ClearRecycle();	// �����ǰ���չ��Ĺ����̣߳��ͷ��ڴ�ռ�
		if (_Count > 0)		// Ϊ 0 ʱ���ı乤���̸߳���
		{
			if (_Count > GetThreadCount())		// ������߳�
			{
				AddNewThread(_Count - GetThreadCount());
			}
			else	// ���б�β��ʼ������ٹ����߳�
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
	// ��������һ������£����� _Block ������
	int PushTask(_task_base_ty *_Ptr, bool _Block)
	{
		if (_Myworkpool.empty())
			return NXTHREAD_POOL_RET_EMPTY;
		return _GetTaskMan().PushTask(_Ptr, _Block);
	}
	// ��ȡ��ǰ���ڹ������߳�����
	unsigned int GetThreadCount() const
	{
		return static_cast<unsigned int>(_Myworkpool.size());
	}

protected:
	CNXThreadPool(const _Myt&);
	_Myt& operator=(const _Myt&);
	// ���������
	_task_man& _GetTaskMan()
	{
		return _Mytaskman;
	}
	const _task_man& _GetTaskMan() const
	{
		return _Mytaskman;
	}
	// ���ٱ�ֹͣ���߳�
	void _ClearRecycle()
	{
		std::for_each(_Myrecycle.begin(), _Myrecycle.end(), std::ptr_fun(NX_DestoryOne<_thread_ty>));
		_Myrecycle.clear();
	}

private:
	_task_man	_Mytaskman;		// ���������
	_Mypoolt	_Myworkpool;	// �����е��߳�
	_Mypoolt	_Myrecycle;		// ��ֹͣ���߳�
};

//////////////////////////////////////////////////////////////////////////
// 2 ά�첽�̳߳�ģ��

template <class _thread_ty, class _task_man, class _task_base_ty>
class CNXThreadPoolAsync2D : public CNXThreadPool<_thread_ty, _task_man, _task_base_ty>
{
public:
	typedef CNXThreadPoolAsync2D<_thread_ty, _task_man, _task_base_ty> _Myt;
	typedef CNXThreadPool<_thread_ty, _task_man, _task_base_ty> _Mybaset;

	CNXThreadPoolAsync2D() throw() {}
	virtual ~CNXThreadPoolAsync2D() throw() {}
	// �����̳߳أ��μ� CNXThreadPool::(unsigned int _Count)��
	// ��1�������趨�����̵߳ĸ����������ǰ�̸߳������� _Count���������Զ����٣�����������̡߳�
	// ��2������������������������ʣ������
	// ��3���첽�����е� Barrier ����������Ϊ 0�������߿���ͨ�� GetCurrBarrier() ��ȡ��
	// ����/�ָ�������������첽�������
	// ��1�������첽���ƵĴ��ڣ�������������̣߳�ֻ��ֹ�첽��������붯����
	int SuspendAsync()
	{
		return _GetTaskMan().SuspendAsync();
	}
	int ResumeAsync()
	{
		return _GetTaskMan().ResumeAsync();
	}
	// ͬ������������е��첽�������
	// ��1��_Wait_run Ϊ true ��ʾ�ȴ�ִ��������ʣ����첽����Ϊ false ���ʾ�������ʣ����첽����
	// ��2�������µ�դ���������µ� Barrier �����ֵ��
	// ��3�������µĵ�ǰ Barrier ������
	int Synchronize(unsigned int _Bar_count,		// դ������
					unsigned int _Bar_check,		// դ�����еļ����ֵ
					bool _Wait_run)					// �Ƿ�����е��������н���
	{
		return _GetTaskMan().Synchronize(_Bar_count, _Bar_check, _Wait_run);
	}
	// ���ص�ǰ��դ��ֵ�������첽���ƵĴ��ڣ���ִ����ת�Ĺ����У�ȡֵ�����ͺ�
	int GetCurrBarrier() const
	{
		return _GetTaskMan().GetCurrBarrier();
	}
	// ��������դ��ֵ��ȡ��һ����Ч��դ��ֵ�������첽�����ͬ����ͬ������ֱ��ʹ�� < 0 ��ֵ����
	int GetNextBarrier(int _Barrier)
	{
		return _GetTaskMan().GetNextBarrier(_Barrier);
	}
	// �������񣨲μ� CNXThreadPool::PushTask(INXUnknownTask *_Ptr, bool _Block)��
	// ��1��������� Barrier ֵС������������ĵ�ǰ Barrier ֵʱ������ͬ�����ƣ����� _Block ������
	// ��1��������� Barrier ֵ��������������� Barrier ����ʱ��_Block Ϊ true �������̣߳������������أ���������ʧ�ܡ�
	// ��1��������� Barrier ֵ��������ʱ�������첽���ƣ����� _Block ������

protected:
	CNXThreadPoolAsync2D(const _Myt&);
	_Myt& operator=(const _Myt&);
};

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// End of file.