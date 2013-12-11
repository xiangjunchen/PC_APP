//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 音频特效算法接口定义
//
// Created: ZhaoZhimeng @ 2010-11-03
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_AUDIO_EFFECT_PROC_DEF_H__
#define __NX_AUDIO_EFFECT_PROC_DEF_H__

#include ".\INXAudioEffect.h"
#include ".\NXAudioProcDef.h"

//////////////////////////////////////////////////////////////////////////
// 编译声明

#ifndef interface
#define interface	struct
#endif		// #ifndef interface

#ifndef IN
#define IN
#endif		// #ifndef IN
#ifndef OUT
#define OUT
#endif		// #ifndef OUT


//////////////////////////////////////////////////////////////////////////
// 库定义

#define NXAUDIOEFFECTPROC_DLL_NAME			_T("NXAudioEffectProc")
#define NXAUDIOEFFECTPROC_API_EXT_NAME		"NAEP_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// 导出接口定义

//////////////////////////////////////////////////////////////////////////
// 特技实例创建接口
typedef INXAudioEffect* (*pfnae_create_effect)(NXAE_TYPE effect_type);

//////////////////////////////////////////////////////////////////////////
// 接口结构定义

typedef struct tag_nxaeouterapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.18
	pfnae_create_effect		create_effect;				// 特技实例创建接口
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口定义从这里开始
}NXAEOuterAPI, *LPNXAEOUTERAPI;

typedef struct tag_nxaeinnerapi *LPNXAEINNERAPI;

typedef struct tag_nxaudioeffectprocapi
{
	LPNXAEOUTERAPI			pStuOuterAPI;
	// Private 权限，用于优化库内部的接口
	LPNXAEINNERAPI			pStuInnerAPI;
} NXAudioEffectProcAPI, *LPNXAUDIOEFFECTPROCAPI;


#endif		// #ifndef __NX_AUDIO_EFFECT_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
