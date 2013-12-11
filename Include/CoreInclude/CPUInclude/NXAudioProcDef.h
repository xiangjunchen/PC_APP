//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ�źŵĻ�������
//
// Created: ZhaoZhimeng @ 2010-11-03
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_AUDIO_PROC_DEF_H__
#define __NX_AUDIO_PROC_DEF_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#include <crtdefs.h>
#else	// VC6
#include <stddef.h>		// for intptr_t
#ifndef _INTPTR_T_DEFINED
typedef ptrdiff_t		intptr_t
#define _INTPTR_T_DEFINED
#endif
#endif		// #if defined(_MSC_VER) && (_MSC_VER >= 1300)

#include ".\NXBasic.h"
#include ".\NXMemProcDef.h"

//////////////////////////////////////////////////////////////////////////
// �ⶨ��

#define NXAUDIOPROC_DLL_NAME		_T("NXAudioProc")
#define NXAUDIOPROC_API_EXT_NAME	"NAP_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ��ȱ�׼
enum NXLOUDNESS_STANDARD
{
	NXLOUDNESS_STANDARD_UNKNOWN,
	NXLOUDNESS_STANDARD_ITU_BS_1770_2 = 3,	// ITU-R BS.1770-2
	NXLOUDNESS_STANDARD_ARIB = 100,			// �ձ�
	NXLOUDNESS_STANDARD_EBU = 200,			// ŷ��
	NXLOUDNESS_STANDARD_ATSC = 300,			// ����
};

//////////////////////////////////////////////////////////////////////////
// �����ӿڶ���

//////////////////////////////////////////////////////////////////////////
// �����ؽӿ�����
interface INXLoudness;
typedef INXLoudness* (*pfnap_create_loudness)();

// ��Ƶ������ֵ��ƽ�ӿ�����
interface INXTruePeakLevel;
typedef INXTruePeakLevel* (*pfnap_create_true_peak_level)();

//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxaudiogenericapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.9.9
	pfnap_create_loudness			create_loudness;
	pfnap_create_true_peak_level	create_true_peak_level;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڶ�������￪ʼ
}NXAudioGenericAPI, *LPNXAUDIOGENERICAPI;

typedef struct tag_nxaudioinnerapi *LPNXAUDIOINNERAPI;

//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxaudioprocapi
{
	LPNXAUDIOGENERICAPI		pStuGenericAPI;
	// Private Ȩ�ޣ������Ż����ڲ��Ľӿ�
	LPNXAUDIOINNERAPI		pStuInnerAPI;
} NXAudioProcAPI, *LPNXAUDIOPROCAPI;


#endif		// #ifndef __NX_AUDIO_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of File.
