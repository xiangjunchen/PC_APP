//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ��Ч�㷨�ӿڶ���
//
// Created: ZhaoZhimeng @ 2010-11-03
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_AUDIO_EFFECT_PROC_DEF_H__
#define __NX_AUDIO_EFFECT_PROC_DEF_H__

#include ".\INXAudioEffect.h"
#include ".\NXAudioProcDef.h"

//////////////////////////////////////////////////////////////////////////
// ��������

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
// �ⶨ��

#define NXAUDIOEFFECTPROC_DLL_NAME			_T("NXAudioEffectProc")
#define NXAUDIOEFFECTPROC_API_EXT_NAME		"NAEP_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// �����ӿڶ���

//////////////////////////////////////////////////////////////////////////
// �ؼ�ʵ�������ӿ�
typedef INXAudioEffect* (*pfnae_create_effect)(NXAE_TYPE effect_type);

//////////////////////////////////////////////////////////////////////////
// �ӿڽṹ����

typedef struct tag_nxaeouterapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.18
	pfnae_create_effect		create_effect;				// �ؼ�ʵ�������ӿ�
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڶ�������￪ʼ
}NXAEOuterAPI, *LPNXAEOUTERAPI;

typedef struct tag_nxaeinnerapi *LPNXAEINNERAPI;

typedef struct tag_nxaudioeffectprocapi
{
	LPNXAEOUTERAPI			pStuOuterAPI;
	// Private Ȩ�ޣ������Ż����ڲ��Ľӿ�
	LPNXAEINNERAPI			pStuInnerAPI;
} NXAudioEffectProcAPI, *LPNXAUDIOEFFECTPROCAPI;


#endif		// #ifndef __NX_AUDIO_EFFECT_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
