//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ��Ч�㷨�ӿ�
//
// Created: ZhaoZhimeng @ 2010-11-03
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_AUDIO_EFFECT_PROC_H__
#define __NX_AUDIO_EFFECT_PROC_H__

#include "NXAudioEffectProcDef.h"

//////////////////////////////////////////////////////////////////////////
// �Զ�������
#include "NXCPUBase.h"

// ���������·����
class CNXAudioEffectProcDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXAUDIOEFFECTPROC_DLL_NAME; }
	LPCSTR APIExtName() const { return NXAUDIOEFFECTPROC_API_EXT_NAME; }
};

// ȫ�ֵ���������
// ��1������ϸ������� NXLoader.h �ļ���
__nxselectany CNXLoader<NXAudioEffectProcAPI, CNXAudioEffectProcDLLPos> g_NXAudioEffectProc;


#endif		// #ifndef __NX_AUDIO_EFFECT_PROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.