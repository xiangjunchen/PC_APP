//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: Windows �汾�̶߳���
//
// Created: Zhaozhimeng @ 2010-07-26
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <winbase.h>
#include <process.h>
#include "..\NXThread.h"

_NXSYS_BEGIN

class CNXThreadWin : public CNXThread<INXUnknownJob>
{
public:
	typedef CNXThread<INXUnknownJob> _Mybaset;

	CNXThreadWin() throw() : _Mybaset(), m_hThread(NULL) {}
	virtual ~CNXThreadWin() throw()
	{
		::CloseHandle(*this);	// �����������̣߳�Ҫ��ʹ�����ֶ�����
	}
	// �����̶߳���
	virtual void Exit(unsigned long ulExitCode)
	{
		if (::WaitForSingleObject(*this, 200) == WAIT_TIMEOUT)
		{
			::TerminateThread(*this, ulExitCode);
			return;
		}
		::CloseHandle(*this);
		m_hThread = NULL;
	}
	bool Terminate(unsigned long ulExitCode)
	{
		bool bRet = (::TerminateThread(*this, ulExitCode) != FALSE);
		::CloseHandle(*this);
		m_hThread = NULL;
		return bRet;
	}
	// �����ָ��̶߳���
	virtual unsigned long Suspend()
	{
		return ::SuspendThread(*this);
	}
	virtual unsigned long Resume()
	{
		return ::ResumeThread(*this);
	}
	// ���
	operator HANDLE()
	{
		return m_hThread;
	}
	operator const HANDLE() const
	{
		return m_hThread;
	}

protected:
	virtual bool _Create(bool bSuspend)
	{
		if (m_hThread != NULL)
			return true;
		m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, CNXThreadWin::ThreadProc, this, (bSuspend ? CREATE_SUSPENDED : 0), NULL));
		return (m_hThread != NULL);
	}
	// ִ�е�Ԫ
	static unsigned _stdcall ThreadProc(void *pArg)
	{
		reinterpret_cast<CNXThreadWin*>(pArg)->_Run();
		return 0;
	}

private:
	HANDLE		m_hThread;
};

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// �ļ�����