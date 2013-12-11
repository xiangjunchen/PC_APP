//////////////////////////////////////////////////////////////////////////
// Copyright (C) Sobey Corporation. All rights reserved.
//
// Desc:	WIN 平台下系统时间统计类.
//
// Author:	Zhaozhimeng @ 2010-07-23 Beijing
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <winbase.h>

namespace nxsys {

class CNXClockWin
{
public:
	CNXClock() : m_i64Start(0), m_i64Stop(0) {}

	BOOL Start()
	{
		return QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_i64Start));
	}
	BOOL Stop()
	{
		return QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_i64Stop));
	}
	double GetMilliSecond()
	{
		__int64 i64Freq(1);
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&i64Freq))
			return (static_cast<double>((m_i64Stop - m_i64Start) * 1000) / static_cast<double>(i64Freq));
		return 0.0;
	}

protected:
	CNXClock(const CNXClock&);
	CNXClock& operator=(const CNXClock&);

private:
	__int64		m_i64Start;
	__int64		m_i64Stop;
};

}

//////////////////////////////////////////////////////////////////////////
// End of file.