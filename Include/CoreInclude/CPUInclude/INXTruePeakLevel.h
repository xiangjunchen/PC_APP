//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ������Ƶ������ֵ��ƽ.
//
// Author: LaiShoubo @ 2012-10-22 Chengdu
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_TRUE_PEAK_LEVEL_H__
#define __I_NX_TRUE_PEAK_LEVEL_H__

#include ".\NXBasic.h"
#include ".\NXAudioProcDef.h"


//////////////////////////////////////////////////////////////////////////
// ����ֵ����
enum
{
	NXTRUEPEAKLEVEL_FAIL = -1,
	NXTRUEPEAKLEVEL_OK = 0,
};

//////////////////////////////////////////////////////////////////////////
// ��Ƶ������ֵ��ƽ�ص�����
typedef int (*pfnap_true_peak_level_callback)(__in double* max_true_peak_level,	// ������ʼλ�õõ�������ֵ����
											  __in double* true_peak_level,		// ��Ƶ������ֵ��ƽ����ָ��: dBTP
											  __in int channel_count,			// ������
											  __in intptr_t samples);			// ������ʼλ�õ�������

//////////////////////////////////////////////////////////////////////////
interface NOVTABLE INXTruePeakLevel
{
	// ���ò���
	virtual int set_param(__in nx_channel_type channel_type,	// ������������
						  __in int channel_count,				// ������
						  __in int sample_rate,					// ������
						  __in NXLOUDNESS_STANDARD standard,	// ���ñ�׼
						  __in int integration_time = 10) = 0;	// ȡֵʱ��: ms
	// ���ûص�����
	virtual int set_true_peak_level_callback(__in pfnap_true_peak_level_callback true_peak_level_callback) = 0;
	// ��ʼ��
	virtual int initialize(__in int cache_time_length = 1000) = 0;			// �ڲ�����ʱ�䳤�ȣ�Ĭ��Ϊ 1s
	// ��ʼ
	virtual int start() = 0;
	// ������Ƶ����
	// ��1��raw_data �������й������ָ���ָ��
	// ��2���� raw_data[1] == NULL ʱ��ʾ��Ƶ������������Ϊ��λ���� A1A2A1A2...
	// ��3���� raw_data[0 ... channel_count-1] ����Ϊ NULL ʱ����ʾ�Թ��Ϊ��λ���� A1A1A1... A2A2A2...
	virtual int push_buffer(__in void** raw_data,			// ��Ƶ����
							__in int bytes_per_sample,		// ÿ�����ֽ���
							__in int samples) = 0;			// ������
	// ֹͣ
	virtual int stop(__in bool force = false) = 0;
	// ���ٵ�ǰʵ��
	virtual int release() = 0;
};



#endif	// __I_NX_TRUE_PEAK_LEVEL_H__
//////////////////////////////////////////////////////////////////////////
// end-of-file
