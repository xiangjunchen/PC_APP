//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 多线程基础定义
//
// Created by Zhaozhimeng @ 2010-12-11
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "..\CPUInclude\NXMemory.h"

//////////////////////////////////////////////////////////////////////////
// 编译定义

#ifndef interface
#define interface	struct
#endif

// namespace
#ifndef _NXSYS_BEGIN
#define _NXSYS_BEGIN	namespace nxsys {
#define _NXSYS_END		}
#endif	// #ifndef _NXSYS_BEGIN

#define NX_INFINITE		((unsigned long)-1)		// 无限延时定义

_NXSYS_BEGIN

//////////////////////////////////////////////////////////////////////////
// 线程虚基类
interface INXUnknownThread : public INXObject
{
	// 工作者在无限循环中可根据该函数判定线程是否需要退出
	virtual bool IsAutoKill() const = 0;
};
// 工作虚基类
interface INXUnknownJob : public INXObject
{
	// 执行主体
	// 如果线程对象为局部变量时，建议 _pthread 参数为空，因为局部对象被析构后容易造成内存越界访问
	virtual int Run(const INXUnknownThread *_pthread) = 0;
};

//////////////////////////////////////////////////////////////////////////
// 任务虚基类
interface INXUnknownTask : public INXObject
{
	// 任务引用计数操作
	virtual void IncRefCount() = 0;
	virtual void DecRefCount() = 0;
	// 执行主体
	virtual int Run() = 0;
	// 任务被完成后的反馈
	virtual void Done(int nRetRun) = 0;
};
// 异步任务
interface INXUnknownAsyncTask : public INXUnknownTask
{
	// 任务所处的栅栏位置
	// 任务管理器通过栅栏实现异步任务矩阵的列同步、异步任务优先级排序、异步任务负载均衡等工作
	// 新栅栏位置由任务管理器的 GetNextBarrier() 提供
	// < 0 的值表示同步任务，进入同步任务队列通道
	virtual int GetBarrier() = 0;
};

//////////////////////////////////////////////////////////////////////////
// 线程池返回值定义
typedef int NXTHREAD_POOL_RET;
enum
{
	NXTHREAD_POOL_RET_SUCCEED	= 1,
	NXTHREAD_POOL_RET_FAILED	= 0,

	NXTHREAD_POOL_RET_STOP		= -1,	// 线程池停止
	NXTHREAD_POOL_RET_SUSPEND	= -2,	// 线程池被挂起
	NXTHREAD_POOL_RET_BLOCK		= -3,	// 线程池阻塞中
	NXTHREAD_POOL_RET_EMPTY		= -4,	// 线程池为空，即无工作线程

	NXTHREAD_POOL_RET_INVALID_PARAM = -10,	// 无效参数输入
};

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// End of file.