//////////////////////////////////////////////////////////////////////////
// Copyright (C) Sobey Corporation. All rights reserved.
//
// Desc:	WIN ƽ̨��������.
//
// Author:	Zhaozhimeng @ 2010-10-25 Beijing
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <winbase.h>

namespace nxsys {

class CNXSleepWin
{
public:
	CNXSleepWin() {}
	
	void Sleep(unsigned long ulMilliseconds)
	{
		::Sleep(ulMilliseconds);
	}

protected:
	CNXSleepWin(const CNXSleepWin&);
	CNXSleepWin& operator=(const CNXSleepWin&);
};

}

//////////////////////////////////////////////////////////////////////////
// End of file.