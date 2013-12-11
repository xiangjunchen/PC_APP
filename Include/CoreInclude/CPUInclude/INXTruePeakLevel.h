//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 测量音频真正峰值电平.
//
// Author: LaiShoubo @ 2012-10-22 Chengdu
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_TRUE_PEAK_LEVEL_H__
#define __I_NX_TRUE_PEAK_LEVEL_H__

#include ".\NXBasic.h"
#include ".\NXAudioProcDef.h"


//////////////////////////////////////////////////////////////////////////
// 返回值定义
enum
{
	NXTRUEPEAKLEVEL_FAIL = -1,
	NXTRUEPEAKLEVEL_OK = 0,
};

//////////////////////////////////////////////////////////////////////////
// 音频真正峰值电平回调函数
typedef int (*pfnap_true_peak_level_callback)(__in double* max_true_peak_level,	// 距离起始位置得到的最大峰值数组
											  __in double* true_peak_level,		// 音频真正峰值电平数组指针: dBTP
											  __in int channel_count,			// 声道数
											  __in intptr_t samples);			// 距离起始位置的样本数

//////////////////////////////////////////////////////////////////////////
interface NOVTABLE INXTruePeakLevel
{
	// 设置参数
	virtual int set_param(__in nx_channel_type channel_type,	// 声道数据类型
						  __in int channel_count,				// 声道数
						  __in int sample_rate,					// 采样率
						  __in NXLOUDNESS_STANDARD standard,	// 采用标准
						  __in int integration_time = 10) = 0;	// 取值时间: ms
	// 设置回调函数
	virtual int set_true_peak_level_callback(__in pfnap_true_peak_level_callback true_peak_level_callback) = 0;
	// 初始化
	virtual int initialize(__in int cache_time_length = 1000) = 0;			// 内部缓冲时间长度，默认为 1s
	// 开始
	virtual int start() = 0;
	// 推入音频缓冲
	// （1）raw_data 代表所有轨道数据指针的指针
	// （2）当 raw_data[1] == NULL 时表示音频数据以样本点为单位，如 A1A2A1A2...
	// （3）当 raw_data[0 ... channel_count-1] 均不为 NULL 时，表示以轨道为单位，如 A1A1A1... A2A2A2...
	virtual int push_buffer(__in void** raw_data,			// 音频数据
							__in int bytes_per_sample,		// 每样本字节数
							__in int samples) = 0;			// 样本数
	// 停止
	virtual int stop(__in bool force = false) = 0;
	// 销毁当前实例
	virtual int release() = 0;
};



#endif	// __I_NX_TRUE_PEAK_LEVEL_H__
//////////////////////////////////////////////////////////////////////////
// end-of-file
