//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: Windows �汾���߳�ͬ����
//
// Created: Zhaozhimeng @ 2010-07-26
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <winbase.h>

namespace nxsys {

class CNXLockWin
{
public:
	CNXLockWin() throw()
	{
		InitializeCriticalSection(&m_CS);
	}
	explicit CNXLockWin(unsigned long ulSpinCount) throw()
	{
		InitializeCriticalSectionAndSpinCount(&m_CS, ulSpinCount);
	}
	~CNXLockWin() throw()
	{
		DeleteCriticalSection(&m_CS);
	}
	// ����
	BOOL TryLock() const
	{
		return TryEnterCriticalSection(*const_cast<CNXLockWin*>(this));
	}
	void Lock() const
	{
		EnterCriticalSection(*const_cast<CNXLockWin*>(this));
	}
	void Unlock() const
	{
		LeaveCriticalSection(*const_cast<CNXLockWin*>(this));
	}
	// ���
	operator LPCRITICAL_SECTION()
	{
		return &m_CS;
	}
	operator const CRITICAL_SECTION*() const
	{
		return &m_CS;
	}

protected:
	CNXLockWin(const CNXLockWin&);
	CNXLockWin& operator=(const CNXLockWin&);

private:
	CRITICAL_SECTION	m_CS;
};

}

//////////////////////////////////////////////////////////////////////////
// �ļ�����