//////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011 Sobey Corporation. All rights reserved.
//
// Description: 音频特技公共基类
//
// Author: LaiShoubo @ 2011-09-13 Beijing
//////////////////////////////////////////////////////////////////////////////////
#ifndef __INXAUDIOEFFECT_H__
#define __INXAUDIOEFFECT_H__

#include ".\NXBasic.h"

//////////////////////////////////////////////////////////////////////////////////
// 特技类型
enum NXAE_TYPE
{
	NXAE_TEMPOPITCHADJUSTING,		// 变速变调
	NXAE_BASSBOOST,					// 低音加重
	NXAE_DIGITALFILTER,				// 数字滤波器（带通、带阻、低通、高通、多频段均衡器）
	NXAE_NOISEREMOVE,				// 背景噪声降噪
	NXAE_CLICKREMOVE,				// 咔嗒声消除
	NXAE_ECHO,						// 回音
	NXAE_VOICEDISGUISE,				// 声音伪装
	NXAE_PHASE,						// 法兹器
	NXAE_WAHWAH,					// 哇音
};

//////////////////////////////////////////////////////////////////////////////////
// 数字滤波器（带通、带阻、低通、高通）类型定义
// 在 nxae_param_digitalfilter 参数结构中指定滤波器类型
enum NX_DIGITALFILTER_TYPE
{
	NX_DIGITALFILTER_LOWPASS,		// 低通滤波器
	NX_DIGITALFILTER_HIGHPASS,		// 高通滤波器
	NX_DIGITALFILTER_BANDPASS,		// 带通滤波器
	NX_DIGITALFILTER_BANDSTOP,		// 带阻滤波器
	NX_DIGITALFILTER_EQUALIZER,		// 多频段均衡器
};

//////////////////////////////////////////////////////////////////////////////////
// 特技参数类型声明

// 变调变速
struct nxae_param_tempo_pitch_adjusting;

// 数字滤波器（带通、带阻、低通、高通、多频段均衡器）
struct nxae_param_digitalfilter;

// 低音加重
struct nxae_param_bassboos;

// 回声
struct nxae_param_echo;

// 哇音
struct nxae_param_wah;

// 法兹器
struct nxae_param_phase;

// 声音伪装
struct nxae_param_voicedisguise;

// 背景噪声降噪
struct nxae_param_noiseremove;

// 咔嗒声移除
struct nxae_param_clickremove;

//////////////////////////////////////////////////////////////////////////////////
// 变速变调特技参数
struct nxae_param_tempo_pitch_adjusting
{
	// 相对于原始的节拍，节拍的变化率 
	// 范围：[-50,100] 单位：百分比 默认值：0
	// 当 pitchChange = 0, rateChange = 0 时， 调节 tempoChange, 实现变速不变调
	float tempoChange;

	// 相对于原始音调（原始音调为0），音调的变化值
	// 取值范围：[-12,12] 单位：半音(Semi-Tones) 默认值：0
	// 当 tempoChange = 0, rateChange = 0 时， 调节 pitchChange, 实现变调不变速
	float pitchChange;

	// 相对于原始速率， 速率的变化率 
	// 范围：[-50,100] 单位：百分比 默认值：0
	// 当 tempoChange = 0，pitchChange = 0 时， 调节 rateChange，实现变速变调
	float rateChange;

	// 音频数据采样率
	// 默认值：48000
	int	sampleRate;

	// 输出缓冲中可以输出的样本数
	int numSamplesAvailable;

	nxae_param_tempo_pitch_adjusting()
	{
		tempoChange = 0.0;
		pitchChange = 0.0;
		rateChange = 0.0;
		sampleRate = 48000;
		numSamplesAvailable = 0;
	}
};

//////////////////////////////////////////////////////////////////////////////////
// 数字滤波器（低通、高通、带通、带阻、多频段均衡器）参数结构定义
struct nxae_param_digitalfilter
{
	// 滤波器类型, 默认为高通滤波器
	NX_DIGITALFILTER_TYPE filterType;

	// 注：低通、高通时仅需要设置LowCutOffFreq为截止频率, 闲置highCutOffFreq
	// 低截止频率
	// 范围：[0,24000] 单位：赫兹 默认值：0
	float lowCutOffFreq;

	// 高截止频率
	// 范围：[0,24000] 单位：赫兹 默认值：0
	float highCutOffFreq;

	// 仅供多频段均衡器使用， 在[0, 16000]赫兹范围内划分的频带个数
	int keyFreqNum;

	// 仅供多频段均衡器使用， 索引标识， 在设置参数时指示改变的频带的索引
	int keyFreqIndex;

	// 仅供多频段均衡器使用， 频带分界点处的频率值， 由keyFreqIndex指示
	// 范围：[0， 16000]
	// 应该保证 keyFreqNum 个频带分界点值递增
	int keyFreq;

	// 仅供多频段均衡器使用， 每个频带的增益， 由keyFreqIndex指示
	// 取值范围：[-12, 12] 单位：分贝 默认值：0
	int keyFreqDecay;

	// FFT窗口长度
	// 范围：[256, 8192], 必须为2的幂次方 默认值：4096
	int	fftLen;

	// 音频数据采样率
	// 默认值：48000
	int sampleRate;

	// 输出缓冲中可以输出的样本数
	int numSamplesAvailable;

	nxae_param_digitalfilter()
	{
		filterType = NX_DIGITALFILTER_HIGHPASS;
		lowCutOffFreq = 0;
		highCutOffFreq = 0;
		keyFreqNum = 0;
		keyFreqIndex = 0;
		keyFreq = 0;
		keyFreqDecay = 0;
		fftLen = 4096;
		sampleRate = 48000;
		numSamplesAvailable = 0;
	}
};

//////////////////////////////////////////////////////////////////////////////////
// 低音加重特技参数结构定义
struct nxae_param_bassboost
{
	// 低音门限频率 
	// 范围：[0,1000] 单位：赫兹 默认值：0
	float freqGate;

	// 低音增益
	// 范围：[0,36] 单位：db 默认值：0
	float boostDB;

	// 音频数据采样率
	// 默认值：48000
	int sampleRate;

	// 输出缓冲中可以输出的样本数
	int numSamplesAvailable;

	nxae_param_bassboost()
	{
		freqGate = 0;
		boostDB = 0;
		sampleRate = 48000;
		numSamplesAvailable = 0;
	}
};

//////////////////////////////////////////////////////////////////////////////////
// 回音特技参数结构定义
struct nxae_param_echo
{
	// 回音个数
	// 范围：[0,5] 默认值：0
	int feedBackNum;

	// 回音延时
	// 范围：[0,2000] 单位：毫秒 默认值：0
	float delayTime;

	// 回音增益
	// 范围：[0,100] 单位：分贝 默认值：0
	float feedBackGain;

	// 音频数据采样率
	// 默认值：48000
	int sampleRate;

	// 输出缓冲中可以输出的样本数
	int numSamplesAvailable;

	nxae_param_echo()
	{
		feedBackNum = 0;
		delayTime = 0;
		feedBackGain = 0;
		sampleRate = 48000;
		numSamplesAvailable = 0;
	}
};

//////////////////////////////////////////////////////////////////////////////////
// 哇音特技参数结构定义
struct nxae_param_wah
{
	// 低频振荡频率 
	// 范围：[0.0,4.0] 默认值：0.0
	float lfoFreq; 

	// 低频振荡起始相位
	// 范围：[0,360] 默认值：0.0
	float startPhase;

	// 低频振荡深度
	// 范围：[0,99] 默认值：99
	float depth; 

	// 共鸣系数
	// 范围：[0.0,9.8] 默认值：0.0
	float resonance;

	// 频率偏移
	// 范围：[0,100] 默认值：60
	float freqOffset; 

	// 音频数据采样率
	// 默认值：48000
	int sampleRate;

	// 输出缓冲中可以输出的样本数
	int numSamplesAvailable;

	nxae_param_wah()
	{
		lfoFreq = 0.0;
		resonance = 0.0;
		startPhase = 0.0;
		freqOffset = 60;
		depth = 99;
		sampleRate = 48000;
		numSamplesAvailable = 0;
	}
};

//////////////////////////////////////////////////////////////////////////////////
// 法兹器特技参数结构定义
struct nxae_param_phase
{
	// 低频振荡频率
	// 范围：[0.0,4.0] 默认值：0.1
	float lfoFreq; 

	// 低频振荡起始相位
	// 范围：[0, 360] 默认值：0.0
	float startPhase; 

	 // 低频振荡深度
	 // 范围：[0,255] 默认值：100
	float depth;

	// 反馈系数
	// 范围：[-50,50] 默认值：0
	float fbGain; 

	// 阶数
	// 范围：[2,24]-偶数 默认值：2
	int stageNum;

	// Dry/wet mix, (0 - dry, 128 - dry=wet, 255 - wet)
	// 范围：[0,255] 默认值：128
	float dryWet;

	// 音频数据采样率
	// 默认值：48000
	int sampleRate;

	// 输出缓冲中可以输出的样本数
	int numSamplesAvailable;

	nxae_param_phase()
	{
		lfoFreq = 0.1f;
		depth = 100.0f;
		fbGain = 0.0f;
		dryWet = 128.0f;
		startPhase = 0.0f;
		stageNum = 2;
		sampleRate = 48000;
		numSamplesAvailable = 0;
	}
};

//////////////////////////////////////////////////////////////////////////////////
// 声音伪装特技参数结构定义
struct nxae_param_voicedisguise
{
	// 平衡
	// 范围：[0,100]-百分比 默认值：0
	float fEffBalance;

	// 干延时
	// 范围：[0,300] 单位：毫秒 默认值：0
	float fDryDelay;

	// 湿延时
	// 范围：[0,300] 单位：毫秒 默认值：0
	float fEffDelay;

	// 变调
	// 范围：[-1200, 1200] 默认值：0
	float fPitchShift;

	// 反馈水平
	// 范围：[-60, 60] 默认值：0
	float fFeedbackLevel;

	// 反馈延时
	// 范围：[0,300] 默认值：0
	float fFeedbackDelay;

	// 音频数据采样率
	// 默认值：48000
	int sampleRate;

	// 输出缓冲中可以输出的样本数
	int numSamplesAvailable;

	nxae_param_voicedisguise()
	{
		fEffBalance = 0.0;
		fDryDelay = 0.0;
		fEffDelay = 0.0;
		fPitchShift = 0;
		fFeedbackLevel = 0 ;
		fFeedbackDelay = 0;
		sampleRate = 48000;
		numSamplesAvailable = 0;
	}
};

//////////////////////////////////////////////////////////////////////////////////
// 背景噪声降噪特技参数结构定义
// 注：应用于背景噪声降噪，降噪前，令mDoProfile = true,选取一段背景噪音为输入数据，
// 获取背景噪声配置文件；获得背景噪声配置文件之后，令mDoProfile = false,此时执行降噪功能
struct nxae_param_noiseremove
{
	// 噪音增益
	// 范围：[-48,0] 单位：分贝 默认值：-20
	float noiseGain;

	// 平滑频率 
	// 范围：[0,1000] 单位：赫兹 默认值：150
	float smoothFreq;

	// 压缩延时
	// 范围：[0.00,1.00] 单位：秒 默认值：0.15
	float attackDecayTime;

	// 音频数据采样率
	// 默认值：48000
	int sampleRate;

	// 输出缓冲中可以输出的样本数
	int numSamplesAvailable;

	//是否取背景噪声配置文件
	bool doProfile;

	nxae_param_noiseremove()
	{
		noiseGain = -20.0f;
		smoothFreq = 150.0f;
		attackDecayTime = 0.15f;
		sampleRate = 48000;
		numSamplesAvailable = 0;
		doProfile = false;
	}
};

//////////////////////////////////////////////////////////////////////////////////
// 咔哒声移除特技参数结构定义
// 注：应用于去除局部声音样本突变引起的爆音、咔哒声
struct nxae_param_clickremove
{
	// 阈值，越小越灵敏 
	// 范围：[0,900] 默认值：200
	float threshholdLevel;

	// click的宽度，越大越灵敏
	// 范围：[0,40] 默认值：20
	float clickWidth;

	// 音频数据采样率
	// 默认值：48000
	int sampleRate;

	// 输出缓冲中可以输出的样本数
	int numSamplesAvailable;

	nxae_param_clickremove()
	{
		threshholdLevel = 200;
		clickWidth = 20;
		sampleRate = 48000;
		numSamplesAvailable = 0;
	}
};


interface NOVTABLE INXAudioEffect
{
	//////////////////////////////////////////////////////////////////////////////////
	// 初始化特技指针及特技资源
	virtual NXRESULT Init() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// 释放特技资源及特技指针
	virtual NXRESULT Release() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// 设置特技参数
	// param：特技参数结构体指针
	virtual NXRESULT SetParameter(void *param) = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// 获取特技参数
	// param：特技参数结构体指针
	virtual NXRESULT GetParameter(void *param) = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// 在Init之后，开始当前数据流的处理
	virtual NXRESULT StartSequence() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// 调用release之前，结束当前数据流的处理
	virtual NXRESULT EndSequence() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// 输入音频样本数据，样本值为归一化的浮点数，取值范围[-1.0,1.0]
	// input : 二维数组，用于多个输入序列的情况，如：混音；单个输入序列时，填充input[0]
	// numSamples : 输入的样本个数
	virtual NXRESULT PushSamples(float **input, int numSamples) = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// 输出音频样本数据，样本值为归一化的浮点数，取值范围[-1.0,1.0]
	// output : 二维数组，用于多个输输出序列的情况，单个输出序列时，从output[0]中取数据
	// numSamples : 取出的样本个数
	virtual NXRESULT ReceiveSamples(float **output, int numSamples) = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// 进行Flush操作，将特技缓存中未处理的数据处理完，在处理过程中不要使用Flush
	// 会引入静音数据，应该放在处理结束之后进行Flush操作；如果不进行Flush操作，则末尾
	// 处理的数据不能输出
	virtual NXRESULT Flush() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// 用于将特技输入、输出缓冲中的数据清空，在StartSequence之前使用
	virtual NXRESULT Clear() = 0;
};

//////////////////////////////////////////////////////////////////////////////////
#endif	// __INXAUDIOEFFECT_H__
