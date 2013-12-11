//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 音频信号的基本定义
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
// 库定义

#define NXAUDIOPROC_DLL_NAME		_T("NXAudioProc")
#define NXAUDIOPROC_API_EXT_NAME	"NAP_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 响度标准
enum NXLOUDNESS_STANDARD
{
	NXLOUDNESS_STANDARD_UNKNOWN,
	NXLOUDNESS_STANDARD_ITU_BS_1770_2 = 3,	// ITU-R BS.1770-2
	NXLOUDNESS_STANDARD_ARIB = 100,			// 日标
	NXLOUDNESS_STANDARD_EBU = 200,			// 欧标
	NXLOUDNESS_STANDARD_ATSC = 300,			// 美标
};

//////////////////////////////////////////////////////////////////////////
// 导出接口定义

//////////////////////////////////////////////////////////////////////////
// 响度相关接口声明
interface INXLoudness;
typedef INXLoudness* (*pfnap_create_loudness)();

// 音频真正峰值电平接口声明
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
	// 新版本接口定义从这里开始
}NXAudioGenericAPI, *LPNXAUDIOGENERICAPI;

typedef struct tag_nxaudioinnerapi *LPNXAUDIOINNERAPI;

//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxaudioprocapi
{
	LPNXAUDIOGENERICAPI		pStuGenericAPI;
	// Private 权限，用于优化库内部的接口
	LPNXAUDIOINNERAPI		pStuInnerAPI;
} NXAudioProcAPI, *LPNXAUDIOPROCAPI;


#endif		// #ifndef __NX_AUDIO_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of File.
