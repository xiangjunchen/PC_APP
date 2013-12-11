//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ����ӿ�
//
// Created: ZhaoZhimeng @ 2010-11-03
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_AUDIO_PROC_H__
#define __NX_AUDIO_PROC_H__

#include "NXAudioProcDef.h"

//////////////////////////////////////////////////////////////////////////
// �Զ�������
#include "NXCPUBase.h"

// ���������·����
class CNXAudioProcDLLPos : public CNXCPUDLLPos
{
public:
	LPCTSTR DLLName() const { return NXAUDIOPROC_DLL_NAME; }
	LPCSTR APIExtName() const { return NXAUDIOPROC_API_EXT_NAME; }
};

// ȫ�ֵ���������
// ��1������ϸ������� NXLoader.h �ļ���
__nxselectany CNXLoader<NXAudioProcAPI, CNXAudioProcDLLPos> g_NXAudioProc;


#endif		// #ifndef __NX_AUDIO_PROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
