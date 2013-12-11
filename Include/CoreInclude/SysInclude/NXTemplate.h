//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ����ģ��
//
// Created: Zhaozhimeng @ 2010-08-19
//////////////////////////////////////////////////////////////////////////
#pragma once

// namespace
#ifndef _NXSYS_BEGIN
#define _NXSYS_BEGIN	namespace nxsys {
#define _NXSYS_END		}
#endif	// #ifndef _NXSYS_BEGIN

_NXSYS_BEGIN

// ���ٶ���
template <class _Ty> inline
void NX_DestoryOne(_Ty *_Ptr)
{
	delete _Ptr;
}
template <class _Ty> inline
void NX_DestoryArray(_Ty *_Ptr)
{
	delete[] _Ptr;
}

_NXSYS_END

//////////////////////////////////////////////////////////////////////////
// End of file.