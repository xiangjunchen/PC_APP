//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ���̻߳�������
//
// Created by Zhaozhimeng @ 2010-12-11
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "..\CPUInclude\NXMemory.h"

//////////////////////////////////////////////////////////////////////////
// ���붨��

#ifndef interface
#define interface	struct
#endif

// namespace
#ifndef _NXSYS_BEGIN
#define _NXSYS_BEGIN	namespace nxsys {
#define _NXSYS_END		}
#endif	// #ifndef _NXSYS_BEGIN

#define NX_INFINITE		((unsigned long)-1)		// ������ʱ����

_NXSYS_BEGIN

//////////////////////////////////////////////////////////////////////////
// �߳������
interface INXUnknownThread : public INXObject
{
	// ������������ѭ���пɸ��ݸú����ж��߳��Ƿ���Ҫ�˳�
	virtual bool IsAutoKill() const = 0;
};
// ���������
interface INXUnknownJob : public INXObject
{
	// ִ������
	// ����̶߳���Ϊ�ֲ�����ʱ������ _pthread ����Ϊ�գ���Ϊ�ֲ�������������������ڴ�Խ�����
	virtual int Run(const INXUnknownThread *_pthread) = 0;
};

//////////////////////////////////////////////////////////////////////////
// ���������
interface INXUnknownTask : public INXObject
{
	// �������ü�������
	virtual void IncRefCount() = 0;
	virtual void DecRefCount() = 0;
	// ִ������
	virtual int Run() = 0;
	// ������ɺ�ķ���
	virtual void Done(int nRetRun) = 0;
};
// �첽����
interface INXUnknownAsyncTask : public INXUnknownTask
{
	// ����������դ��λ��
	// ���������ͨ��դ��ʵ���첽����������ͬ�����첽�������ȼ������첽�����ؾ���ȹ���
	// ��դ��λ��������������� GetNextBarrier() �ṩ
	// < 0 ��ֵ��ʾͬ�����񣬽���ͬ���������ͨ��
	virtual int GetBarrier() = 0;
};

//////////////////////////////////////////////////////////////////////////
// �̳߳ط���ֵ����
typedef int NXTHREAD_POOL_RET;
enum
{
	NXTHREAD_POOL_RET_SUCCEED	= 1,
	NXTHREAD_POOL_RET_FAILED	= 0,

	NXTHREAD_POOL_RET_STOP		= -1,	// �̳߳�ֹͣ
	NXTHREAD_POOL_RET_SUSPEND	= -2,	// �̳߳ر�����
	NXTHREAD_POOL_RET_BLOCK		= -3,	// �̳߳�������
	NXTHREAD_POOL_RET_EMPTY		= -4,	// �̳߳�Ϊ�գ����޹����߳�

	NXTHREAD_POOL_RET_INVALID_PARAM = -10,	// ��Ч��������
};

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// End of file.