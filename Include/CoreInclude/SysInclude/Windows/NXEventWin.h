//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: Windows 版本多线程事件
//
// Created: Zhaozhimeng @ 2010-07-26
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <winbase.h>

namespace nxsys {

class CNXEventWin
{
public:
	CNXEventWin() : m_hEvent(NULL) {}
	~CNXEventWin() throw()
	{
		Close();
	}
	// 创建与关闭
	bool Create(bool bManualReset, bool bInitialState)
	{
		if (!IsValid())
		{
			m_hEvent = ::CreateEvent(NULL, bManualReset, bInitialState, NULL);
		}
		return IsValid();
	}
	void Close()
	{
		::CloseHandle(*this);
		m_hEvent = NULL;
	}
	// 有效性
	bool IsValid() const
	{
		return (m_hEvent != NULL && m_hEvent != INVALID_HANDLE_VALUE);
	}
	// 动作
	BOOL Set() const
	{
		return ::SetEvent(*const_cast<CNXEventWin*>(this));
	}
	BOOL Reset() const
	{
		return ::ResetEvent(*const_cast<CNXEventWin*>(this));
	}
	unsigned long Wait(unsigned long ulMilliseconds) const
	{
		return ::WaitForSingleObject(*const_cast<CNXEventWin*>(this), ulMilliseconds);
	}
	// 句柄
	operator HANDLE()
	{
		return m_hEvent;
	}
	operator const HANDLE() const
	{
		return m_hEvent;
	}

protected:
	CNXEventWin(const CNXEventWin&);
	CNXEventWin& operator=(const CNXEventWin&);

private:
	HANDLE		m_hEvent;
};

}

//////////////////////////////////////////////////////////////////////////
// 文件结束