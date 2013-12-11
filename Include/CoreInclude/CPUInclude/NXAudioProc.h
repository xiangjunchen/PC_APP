//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 音频处理接口
//
// Created: ZhaoZhimeng @ 2010-11-03
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_AUDIO_PROC_H__
#define __NX_AUDIO_PROC_H__

#include "NXAudioProcDef.h"

//////////////////////////////////////////////////////////////////////////
// 自动加载器
#include "NXCPUBase.h"

// 加载器相对路径类
class CNXAudioProcDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXAUDIOPROC_DLL_NAME; }
	LPCSTR APIExtName() const { return NXAUDIOPROC_API_EXT_NAME; }
};

// 全局单件加载器
// （1）具体细节请参阅 NXLoader.h 文件。
__nxselectany CNXLoader<NXAudioProcAPI, CNXAudioProcDLLPos> g_NXAudioProc;


#endif		// #ifndef __NX_AUDIO_PROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
