//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 音频特效算法接口
//
// Created: ZhaoZhimeng @ 2010-11-03
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_AUDIO_EFFECT_PROC_H__
#define __NX_AUDIO_EFFECT_PROC_H__

#include "NXAudioEffectProcDef.h"

//////////////////////////////////////////////////////////////////////////
// 自动加载器
#include "NXCPUBase.h"

// 加载器相对路径类
class CNXAudioEffectProcDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXAUDIOEFFECTPROC_DLL_NAME; }
	LPCSTR APIExtName() const { return NXAUDIOEFFECTPROC_API_EXT_NAME; }
};

// 全局单件加载器
// （1）具体细节请参阅 NXLoader.h 文件。
__nxselectany CNXLoader<NXAudioEffectProcAPI, CNXAudioEffectProcDLLPos> g_NXAudioEffectProc;


#endif		// #ifndef __NX_AUDIO_EFFECT_PROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.