//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: Windows 版本多线程信标量
//
// Created: Zhaozhimeng @ 2010-07-26
//////////////////////////////////////////////////////////////////////////
#pragma once

namespace nxsys {

class CNXSemaphoreWin
{
public:
	CNXSemaphoreWin() : m_hSemaphore(NULL) {}
	~CNXSemaphoreWin() throw()
	{
		Close();
	}
	// 创建与关闭
	bool Create()
	{
		if (!IsValid())
		{
			m_hSemaphore = ::CreateSemaphore(NULL, 0, LONG_MAX, NULL);
		}
		return IsValid();
	}
	void Close()
	{
		::CloseHandle(*this);
		m_hSemaphore = INVALID_HANDLE_VALUE;
	}
	// 有效性
	bool IsValid() const
	{
		return (m_hSemaphore != NULL && m_hSemaphore != INVALID_HANDLE_VALUE);
	}
	// 动作
	BOOL Release(long lReleaseCount, long *plPreviousCount)
	{
		return ::ReleaseSemaphore(*this, lReleaseCount, plPreviousCount);
	}
	unsigned long Wait(unsigned long ulMilliseconds)
	{
		return ::WaitForSingleObject(*this, ulMilliseconds);
	}
	void Reset()
	{
		while (::WaitForSingleObject(*this, 0) == WAIT_OBJECT_0) ;
	}
	// 句柄
	operator HANDLE()
	{
		return m_hSemaphore;
	}
	operator const HANDLE() const
	{
		return m_hSemaphore;
	}

protected:
	CNXSemaphoreWin(const CNXSemaphoreWin&);
	CNXSemaphoreWin& operator=(const CNXSemaphoreWin&);

private:
	HANDLE		m_hSemaphore;
};

}

//////////////////////////////////////////////////////////////////////////
// 文件结束