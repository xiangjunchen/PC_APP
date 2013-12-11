//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 响度相关接口.
//
// Author: LaiShoubo @ 2012-09-05 Beijing
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_LOUDNESS_H__
#define __I_NX_LOUDNESS_H__

#include ".\NXBasic.h"
#include ".\NXAudioProcDef.h"


//////////////////////////////////////////////////////////////////////////
// 响度相关返回值定义
enum
{
	NXLOUDNESS_FAIL = -1,
	NXLOUDNESS_OK = 0,
};

//////////////////////////////////////////////////////////////////////////
// 响度时间刻度类型
enum NXLOUDNESS_TS_TYPE
{
	NXLOUDNESS_TS_MOMENTARY,
	NXLOUDNESS_TS_SHORT_TERM,
	NXLOUDNESS_TS_INTEGRATED,
};

//////////////////////////////////////////////////////////////////////////
// 响度计算的回调函数
typedef int (*pfnap_loudness_callback)(__in double loudness,					// 响度
									   __in NXLOUDNESS_TS_TYPE ts,				// 响度时标类型
									   __in intptr_t samples);					// 距离起始位置的样本数

//////////////////////////////////////////////////////////////////////////
// 响度范围计算的回调函数
typedef int (*pfnap_loudness_range_callback)(__in double loudness_range,		// 响度范围
											 __in double high_loudness,			// 高响度临界值
											 __in double low_loudness,			// 低响度临界值
											 __in double abs_gated_integrated,	// 绝对门限综合响度
											 __in double* short_term_loudness,	// 所有短时响度值
											 __in int count,					// 短时响度个数
											 __in intptr_t samples);			// 距离起始位置的样本数

//////////////////////////////////////////////////////////////////////////
// 响度相关接口
interface NOVTABLE INXLoudness
{
	// 设置参数
	// （1）时间窗口长度的单位均为 ms
	virtual int set_param(__in nx_channel_type channel_type,				// 声道数据类型
						  __in int channel_count,							// 声道数
						  __in int sample_rate,								// 采样率
						  __in int integrated_interval,						// 计算综合响度的间隔时间
						  __in int range_interval,							// 计算响度范围的间隔时间
						  __in NXLOUDNESS_STANDARD standard,				// 响度标准
						  __in int momentary_loudness_window = 400,			// 瞬时响度的时间窗口长度
						  __in int momentary_loudness_overlap = 75,			// 瞬时响度的时间窗口间覆盖率
						  __in int short_term_loudness_window = 3000,		// 短时响度的时间窗口长度
						  __in int short_term_loudness_overlap = 75,		// 短时响度的时间窗口间覆盖率
						  __in int integrated_momentary_window = 400,		// 综合响度计算时的瞬时窗口长度
						  __in int integrated_momentary_overlap = 75,		// 综合响度计算时的瞬时窗口间覆盖率
						  __in int range_short_term_window = 3000,			// 响度范围计算时的短时响度的时间窗口长度
						  __in int range_short_term_overlap = 66,			// 响度范围计算时的短时响度的时间窗口间覆盖率
						  __in double integrated_absolute_gate = -70,		// 计算综合响度时的绝对门限
						  __in double integrated_relative_gate = -10,		// 计算综合响度时的相对门限
						  __in double range_absolute_gate = -70,			// 计算响度范围时的绝对门限
						  __in double range_relative_gate = -20,			// 计算响度范围时的相对门限
						  __in int range_lower_percentile = 10,				// 计算响度范围时的低响度率 [0, 100]
						  __in int range_upper_percentile = 95,				// 计算响度范围时的高响度率 [0, 100]
						  __in double* prefilter1_coef = NULL,				// 前置滤波器第一级的系数 { b0, b1, b2, a1, a2 }
						  __in double* prefilter2_coef = NULL,				// 前置滤波器第二级的系数 { b0, b1, b2, a1, a2 }
						  __in double* gating_coef = NULL) = 0;				// 声道门限系数 { L, R, C, Ls, Rs }
	// 设置回调函数
	virtual int set_loudness_callback(__in pfnap_loudness_callback loudness_callback) = 0;
	virtual int set_loudness_range_callback(__in pfnap_loudness_range_callback loudness_range_callback) = 0;
	// 初始化
	virtual int initialize(__in int cache_time_length = 10000) = 0;			// 内部缓冲时间长度，默认为 10s
	// 开始
	virtual int start() = 0;
	// 推入音频缓冲
	// （1）raw_data 代表所有轨道数据指针的指针
	// （2）当 raw_data[1] == NULL 时表示音频数据以样本点为单位，如 A1A2A1A2...
	// （3）当 raw_data[0 ... channel_count-1] 均不为 NULL 时，表示以轨道为单位，如 A1A1A1... A2A2A2...
	virtual int push_buffer(__in void** raw_data,			// 音频数据
							__in int bytes_per_sample,		// 每样本字节数
							__in int samples) = 0;			// 样本数
	// 清空音频缓冲数据，用于前后不相邻素材的连续检测，后一个素材回调的样本数从 0 开始
	virtual int flush(__in unsigned long milliseconds = 0xffffffff) = 0;
	// 停止
	virtual int stop(__in bool force = false) = 0;
	// 销毁当前实例
	virtual int release() = 0;
};


#endif	// __I_NX_LOUDNESS_H__
//////////////////////////////////////////////////////////////////////////
// end-of-file
