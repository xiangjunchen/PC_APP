//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: �����ؽӿ�.
//
// Author: LaiShoubo @ 2012-09-05 Beijing
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_LOUDNESS_H__
#define __I_NX_LOUDNESS_H__

#include ".\NXBasic.h"
#include ".\NXAudioProcDef.h"


//////////////////////////////////////////////////////////////////////////
// �����ط���ֵ����
enum
{
	NXLOUDNESS_FAIL = -1,
	NXLOUDNESS_OK = 0,
};

//////////////////////////////////////////////////////////////////////////
// ���ʱ��̶�����
enum NXLOUDNESS_TS_TYPE
{
	NXLOUDNESS_TS_MOMENTARY,
	NXLOUDNESS_TS_SHORT_TERM,
	NXLOUDNESS_TS_INTEGRATED,
};

//////////////////////////////////////////////////////////////////////////
// ��ȼ���Ļص�����
typedef int (*pfnap_loudness_callback)(__in double loudness,					// ���
									   __in NXLOUDNESS_TS_TYPE ts,				// ���ʱ������
									   __in intptr_t samples);					// ������ʼλ�õ�������

//////////////////////////////////////////////////////////////////////////
// ��ȷ�Χ����Ļص�����
typedef int (*pfnap_loudness_range_callback)(__in double loudness_range,		// ��ȷ�Χ
											 __in double high_loudness,			// ������ٽ�ֵ
											 __in double low_loudness,			// ������ٽ�ֵ
											 __in double abs_gated_integrated,	// ���������ۺ����
											 __in double* short_term_loudness,	// ���ж�ʱ���ֵ
											 __in int count,					// ��ʱ��ȸ���
											 __in intptr_t samples);			// ������ʼλ�õ�������

//////////////////////////////////////////////////////////////////////////
// �����ؽӿ�
interface NOVTABLE INXLoudness
{
	// ���ò���
	// ��1��ʱ�䴰�ڳ��ȵĵ�λ��Ϊ ms
	virtual int set_param(__in nx_channel_type channel_type,				// ������������
						  __in int channel_count,							// ������
						  __in int sample_rate,								// ������
						  __in int integrated_interval,						// �����ۺ���ȵļ��ʱ��
						  __in int range_interval,							// ������ȷ�Χ�ļ��ʱ��
						  __in NXLOUDNESS_STANDARD standard,				// ��ȱ�׼
						  __in int momentary_loudness_window = 400,			// ˲ʱ��ȵ�ʱ�䴰�ڳ���
						  __in int momentary_loudness_overlap = 75,			// ˲ʱ��ȵ�ʱ�䴰�ڼ串����
						  __in int short_term_loudness_window = 3000,		// ��ʱ��ȵ�ʱ�䴰�ڳ���
						  __in int short_term_loudness_overlap = 75,		// ��ʱ��ȵ�ʱ�䴰�ڼ串����
						  __in int integrated_momentary_window = 400,		// �ۺ���ȼ���ʱ��˲ʱ���ڳ���
						  __in int integrated_momentary_overlap = 75,		// �ۺ���ȼ���ʱ��˲ʱ���ڼ串����
						  __in int range_short_term_window = 3000,			// ��ȷ�Χ����ʱ�Ķ�ʱ��ȵ�ʱ�䴰�ڳ���
						  __in int range_short_term_overlap = 66,			// ��ȷ�Χ����ʱ�Ķ�ʱ��ȵ�ʱ�䴰�ڼ串����
						  __in double integrated_absolute_gate = -70,		// �����ۺ����ʱ�ľ�������
						  __in double integrated_relative_gate = -10,		// �����ۺ����ʱ���������
						  __in double range_absolute_gate = -70,			// ������ȷ�Χʱ�ľ�������
						  __in double range_relative_gate = -20,			// ������ȷ�Χʱ���������
						  __in int range_lower_percentile = 10,				// ������ȷ�Χʱ�ĵ������ [0, 100]
						  __in int range_upper_percentile = 95,				// ������ȷ�Χʱ�ĸ������ [0, 100]
						  __in double* prefilter1_coef = NULL,				// ǰ���˲�����һ����ϵ�� { b0, b1, b2, a1, a2 }
						  __in double* prefilter2_coef = NULL,				// ǰ���˲����ڶ�����ϵ�� { b0, b1, b2, a1, a2 }
						  __in double* gating_coef = NULL) = 0;				// ��������ϵ�� { L, R, C, Ls, Rs }
	// ���ûص�����
	virtual int set_loudness_callback(__in pfnap_loudness_callback loudness_callback) = 0;
	virtual int set_loudness_range_callback(__in pfnap_loudness_range_callback loudness_range_callback) = 0;
	// ��ʼ��
	virtual int initialize(__in int cache_time_length = 10000) = 0;			// �ڲ�����ʱ�䳤�ȣ�Ĭ��Ϊ 10s
	// ��ʼ
	virtual int start() = 0;
	// ������Ƶ����
	// ��1��raw_data �������й������ָ���ָ��
	// ��2���� raw_data[1] == NULL ʱ��ʾ��Ƶ������������Ϊ��λ���� A1A2A1A2...
	// ��3���� raw_data[0 ... channel_count-1] ����Ϊ NULL ʱ����ʾ�Թ��Ϊ��λ���� A1A1A1... A2A2A2...
	virtual int push_buffer(__in void** raw_data,			// ��Ƶ����
							__in int bytes_per_sample,		// ÿ�����ֽ���
							__in int samples) = 0;			// ������
	// �����Ƶ�������ݣ�����ǰ�������زĵ�������⣬��һ���زĻص����������� 0 ��ʼ
	virtual int flush(__in unsigned long milliseconds = 0xffffffff) = 0;
	// ֹͣ
	virtual int stop(__in bool force = false) = 0;
	// ���ٵ�ǰʵ��
	virtual int release() = 0;
};


#endif	// __I_NX_LOUDNESS_H__
//////////////////////////////////////////////////////////////////////////
// end-of-file
