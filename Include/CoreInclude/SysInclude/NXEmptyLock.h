//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ����
//
// Created: Zhaozhimeng @ 2010-09-16
//////////////////////////////////////////////////////////////////////////
#pragma once

namespace nxsys {

class CNXEmptyLock
{
public:
	CNXEmptyLock() throw() {}
	~CNXEmptyLock() throw() {}

	void Lock() const {}
	void Unlock() const {}
};

}

//////////////////////////////////////////////////////////////////////////
// End of file.