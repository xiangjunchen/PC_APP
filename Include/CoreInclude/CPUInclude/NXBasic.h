//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: �������ͼ�����ֵ����
//
// Author: LaiShoubo @ 2011-08-19
//////////////////////////////////////////////////////////////////////////
#ifndef __NXBASIC_H__
#define __NXBASIC_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#include <crtdefs.h>
#else	// VC6
#include <stddef.h>		// for intptr_t
#ifndef _INTPTR_T_DEFINED
typedef ptrdiff_t		intptr_t;
#define _INTPTR_T_DEFINED
#endif
#endif		// #if defined(_MSC_VER) && (_MSC_VER >= 1300)

//////////////////////////////////////////////////////////////////////////
// ��ʽ�������η�
#ifndef __in
#define __in
#endif

#ifndef __out
#define __out
#endif

#ifndef __inout
#define __inout
#endif

//////////////////////////////////////////////////////////////////////////
#ifndef NOVTABLE
#define NOVTABLE __declspec(novtable)
#endif

#ifndef interface
#define interface struct
#endif

//////////////////////////////////////////////////////////////////////////
// ����ֵ��Ϣ����
enum NXRESULT
{
	NXRESULT_UNKNOWN = 0x80000000,			// δ֪����
	NXRESULT_INVALID_PARAM,					// ��Ч����
	NXRESULT_OUT_OF_MEMORY,					// �ڴ治��
	NXRESULT_NOT_SUPPORTED,					// ��֧��
	NXRESULT_CACHE_FULL,					// ������
	NXRESULT_CACHE_EMPTY,					// �����
	NXRESULT_INVALID_POINTER,				// ��Ч��ָ��
	NXRESULT_INITIALIZED,					// �ѳ�ʼ��
	NXRESULT_FAILED,						// ʧ��
	NXRESULT_OUT_OF_RANGE,					// Խ��
	NXRESULT_OK = 0,						// �ɹ�
};

//////////////////////////////////////////////////////////////////////////
#endif // __NXBASIC_H__
