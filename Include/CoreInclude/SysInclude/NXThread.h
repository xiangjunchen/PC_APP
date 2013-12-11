//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: �̻߳���ģ��
//
// Created by Zhaozhimeng @ 2010-05-20
//////////////////////////////////////////////////////////////////////////
#pragma once

#include ".\NXMultiThread.h"

_NXSYS_BEGIN

//////////////////////////////////////////////////////////////////////////
// �̻߳���ģ��

template <class _job_ty>
class CNXThread : public INXUnknownThread
{
public:
	typedef CNXThread<_job_ty> _Myt;
	typedef _job_ty* pointer;
	typedef const _job_ty* const_pointer;

	CNXThread() throw() : _Myjob(NULL), _Myautokill(false), _Mylocalobj(false) {}
	virtual ~CNXThread() throw() {}
	// ������_Local_obj ������Ҫ����ȷ���̶߳����Ƿ�Ϊ�ֲ�����
	bool Create(const_pointer _Ptr_job, bool _Suspend, bool _Local_obj = false)
	{
		if (_Ptr_job == NULL)
			return false;
		_Myautokill = false;
		_Mylocalobj = _Local_obj;
		_Myjob = const_cast<pointer>(_Ptr_job);
		return _Create(_Suspend);	// ʵ�ʴ�������
	}
	// ״̬
	void AutoKill()		// �����̶߳��������˳�
	{
		_Myautokill = true;
	}
	virtual bool IsAutoKill() const
	{
		return _Myautokill;
	}
	// �����̶߳���
	virtual void Exit(unsigned long ulExitCode) = 0;
	// �����ָ��̶߳���
	virtual unsigned long Suspend() = 0;
	virtual unsigned long Resume() = 0;

protected:
	CNXThread(const _Myt &_Right);
	_Myt& operator=(const _Myt &_Right);
	// ����
	virtual bool _Create(bool _Suspend) = 0;
	// �̻߳ص�����
	int _Run()
	{
		return _Myjob->Run(_Mylocalobj ? NULL : this);
	}

private:
	pointer	_Myjob;
	bool	_Myautokill;	// �������ٱ�־
	bool	_Mylocalobj;	// �ֲ������־
};

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// End of file.