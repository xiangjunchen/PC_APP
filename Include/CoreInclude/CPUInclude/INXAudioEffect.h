//////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011 Sobey Corporation. All rights reserved.
//
// Description: ��Ƶ�ؼ���������
//
// Author: LaiShoubo @ 2011-09-13 Beijing
//////////////////////////////////////////////////////////////////////////////////
#ifndef __INXAUDIOEFFECT_H__
#define __INXAUDIOEFFECT_H__

#include ".\NXBasic.h"

//////////////////////////////////////////////////////////////////////////////////
// �ؼ�����
enum NXAE_TYPE
{
	NXAE_TEMPOPITCHADJUSTING,		// ���ٱ��
	NXAE_BASSBOOST,					// ��������
	NXAE_DIGITALFILTER,				// �����˲�������ͨ�����衢��ͨ����ͨ����Ƶ�ξ�������
	NXAE_NOISEREMOVE,				// ������������
	NXAE_CLICKREMOVE,				// ���������
	NXAE_ECHO,						// ����
	NXAE_VOICEDISGUISE,				// ����αװ
	NXAE_PHASE,						// ������
	NXAE_WAHWAH,					// ����
};

//////////////////////////////////////////////////////////////////////////////////
// �����˲�������ͨ�����衢��ͨ����ͨ�����Ͷ���
// �� nxae_param_digitalfilter �����ṹ��ָ���˲�������
enum NX_DIGITALFILTER_TYPE
{
	NX_DIGITALFILTER_LOWPASS,		// ��ͨ�˲���
	NX_DIGITALFILTER_HIGHPASS,		// ��ͨ�˲���
	NX_DIGITALFILTER_BANDPASS,		// ��ͨ�˲���
	NX_DIGITALFILTER_BANDSTOP,		// �����˲���
	NX_DIGITALFILTER_EQUALIZER,		// ��Ƶ�ξ�����
};

//////////////////////////////////////////////////////////////////////////////////
// �ؼ�������������

// �������
struct nxae_param_tempo_pitch_adjusting;

// �����˲�������ͨ�����衢��ͨ����ͨ����Ƶ�ξ�������
struct nxae_param_digitalfilter;

// ��������
struct nxae_param_bassboos;

// ����
struct nxae_param_echo;

// ����
struct nxae_param_wah;

// ������
struct nxae_param_phase;

// ����αװ
struct nxae_param_voicedisguise;

// ������������
struct nxae_param_noiseremove;

// ������Ƴ�
struct nxae_param_clickremove;

//////////////////////////////////////////////////////////////////////////////////
// ���ٱ���ؼ�����
struct nxae_param_tempo_pitch_adjusting
{
	// �����ԭʼ�Ľ��ģ����ĵı仯�� 
	// ��Χ��[-50,100] ��λ���ٷֱ� Ĭ��ֵ��0
	// �� pitchChange = 0, rateChange = 0 ʱ�� ���� tempoChange, ʵ�ֱ��ٲ����
	float tempoChange;

	// �����ԭʼ������ԭʼ����Ϊ0���������ı仯ֵ
	// ȡֵ��Χ��[-12,12] ��λ������(Semi-Tones) Ĭ��ֵ��0
	// �� tempoChange = 0, rateChange = 0 ʱ�� ���� pitchChange, ʵ�ֱ��������
	float pitchChange;

	// �����ԭʼ���ʣ� ���ʵı仯�� 
	// ��Χ��[-50,100] ��λ���ٷֱ� Ĭ��ֵ��0
	// �� tempoChange = 0��pitchChange = 0 ʱ�� ���� rateChange��ʵ�ֱ��ٱ��
	float rateChange;

	// ��Ƶ���ݲ�����
	// Ĭ��ֵ��48000
	int	sampleRate;

	// ��������п��������������
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
// �����˲�������ͨ����ͨ����ͨ�����衢��Ƶ�ξ������������ṹ����
struct nxae_param_digitalfilter
{
	// �˲�������, Ĭ��Ϊ��ͨ�˲���
	NX_DIGITALFILTER_TYPE filterType;

	// ע����ͨ����ͨʱ����Ҫ����LowCutOffFreqΪ��ֹƵ��, ����highCutOffFreq
	// �ͽ�ֹƵ��
	// ��Χ��[0,24000] ��λ������ Ĭ��ֵ��0
	float lowCutOffFreq;

	// �߽�ֹƵ��
	// ��Χ��[0,24000] ��λ������ Ĭ��ֵ��0
	float highCutOffFreq;

	// ������Ƶ�ξ�����ʹ�ã� ��[0, 16000]���ȷ�Χ�ڻ��ֵ�Ƶ������
	int keyFreqNum;

	// ������Ƶ�ξ�����ʹ�ã� ������ʶ�� �����ò���ʱָʾ�ı��Ƶ��������
	int keyFreqIndex;

	// ������Ƶ�ξ�����ʹ�ã� Ƶ���ֽ�㴦��Ƶ��ֵ�� ��keyFreqIndexָʾ
	// ��Χ��[0�� 16000]
	// Ӧ�ñ�֤ keyFreqNum ��Ƶ���ֽ��ֵ����
	int keyFreq;

	// ������Ƶ�ξ�����ʹ�ã� ÿ��Ƶ�������棬 ��keyFreqIndexָʾ
	// ȡֵ��Χ��[-12, 12] ��λ���ֱ� Ĭ��ֵ��0
	int keyFreqDecay;

	// FFT���ڳ���
	// ��Χ��[256, 8192], ����Ϊ2���ݴη� Ĭ��ֵ��4096
	int	fftLen;

	// ��Ƶ���ݲ�����
	// Ĭ��ֵ��48000
	int sampleRate;

	// ��������п��������������
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
// ���������ؼ������ṹ����
struct nxae_param_bassboost
{
	// ��������Ƶ�� 
	// ��Χ��[0,1000] ��λ������ Ĭ��ֵ��0
	float freqGate;

	// ��������
	// ��Χ��[0,36] ��λ��db Ĭ��ֵ��0
	float boostDB;

	// ��Ƶ���ݲ�����
	// Ĭ��ֵ��48000
	int sampleRate;

	// ��������п��������������
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
// �����ؼ������ṹ����
struct nxae_param_echo
{
	// ��������
	// ��Χ��[0,5] Ĭ��ֵ��0
	int feedBackNum;

	// ������ʱ
	// ��Χ��[0,2000] ��λ������ Ĭ��ֵ��0
	float delayTime;

	// ��������
	// ��Χ��[0,100] ��λ���ֱ� Ĭ��ֵ��0
	float feedBackGain;

	// ��Ƶ���ݲ�����
	// Ĭ��ֵ��48000
	int sampleRate;

	// ��������п��������������
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
// �����ؼ������ṹ����
struct nxae_param_wah
{
	// ��Ƶ��Ƶ�� 
	// ��Χ��[0.0,4.0] Ĭ��ֵ��0.0
	float lfoFreq; 

	// ��Ƶ����ʼ��λ
	// ��Χ��[0,360] Ĭ��ֵ��0.0
	float startPhase;

	// ��Ƶ�����
	// ��Χ��[0,99] Ĭ��ֵ��99
	float depth; 

	// ����ϵ��
	// ��Χ��[0.0,9.8] Ĭ��ֵ��0.0
	float resonance;

	// Ƶ��ƫ��
	// ��Χ��[0,100] Ĭ��ֵ��60
	float freqOffset; 

	// ��Ƶ���ݲ�����
	// Ĭ��ֵ��48000
	int sampleRate;

	// ��������п��������������
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
// �������ؼ������ṹ����
struct nxae_param_phase
{
	// ��Ƶ��Ƶ��
	// ��Χ��[0.0,4.0] Ĭ��ֵ��0.1
	float lfoFreq; 

	// ��Ƶ����ʼ��λ
	// ��Χ��[0, 360] Ĭ��ֵ��0.0
	float startPhase; 

	 // ��Ƶ�����
	 // ��Χ��[0,255] Ĭ��ֵ��100
	float depth;

	// ����ϵ��
	// ��Χ��[-50,50] Ĭ��ֵ��0
	float fbGain; 

	// ����
	// ��Χ��[2,24]-ż�� Ĭ��ֵ��2
	int stageNum;

	// Dry/wet mix, (0 - dry, 128 - dry=wet, 255 - wet)
	// ��Χ��[0,255] Ĭ��ֵ��128
	float dryWet;

	// ��Ƶ���ݲ�����
	// Ĭ��ֵ��48000
	int sampleRate;

	// ��������п��������������
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
// ����αװ�ؼ������ṹ����
struct nxae_param_voicedisguise
{
	// ƽ��
	// ��Χ��[0,100]-�ٷֱ� Ĭ��ֵ��0
	float fEffBalance;

	// ����ʱ
	// ��Χ��[0,300] ��λ������ Ĭ��ֵ��0
	float fDryDelay;

	// ʪ��ʱ
	// ��Χ��[0,300] ��λ������ Ĭ��ֵ��0
	float fEffDelay;

	// ���
	// ��Χ��[-1200, 1200] Ĭ��ֵ��0
	float fPitchShift;

	// ����ˮƽ
	// ��Χ��[-60, 60] Ĭ��ֵ��0
	float fFeedbackLevel;

	// ������ʱ
	// ��Χ��[0,300] Ĭ��ֵ��0
	float fFeedbackDelay;

	// ��Ƶ���ݲ�����
	// Ĭ��ֵ��48000
	int sampleRate;

	// ��������п��������������
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
// �������������ؼ������ṹ����
// ע��Ӧ���ڱ����������룬����ǰ����mDoProfile = true,ѡȡһ�α�������Ϊ�������ݣ�
// ��ȡ�������������ļ�����ñ������������ļ�֮����mDoProfile = false,��ʱִ�н��빦��
struct nxae_param_noiseremove
{
	// ��������
	// ��Χ��[-48,0] ��λ���ֱ� Ĭ��ֵ��-20
	float noiseGain;

	// ƽ��Ƶ�� 
	// ��Χ��[0,1000] ��λ������ Ĭ��ֵ��150
	float smoothFreq;

	// ѹ����ʱ
	// ��Χ��[0.00,1.00] ��λ���� Ĭ��ֵ��0.15
	float attackDecayTime;

	// ��Ƶ���ݲ�����
	// Ĭ��ֵ��48000
	int sampleRate;

	// ��������п��������������
	int numSamplesAvailable;

	//�Ƿ�ȡ�������������ļ�
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
// �������Ƴ��ؼ������ṹ����
// ע��Ӧ����ȥ���ֲ���������ͻ������ı�����������
struct nxae_param_clickremove
{
	// ��ֵ��ԽСԽ���� 
	// ��Χ��[0,900] Ĭ��ֵ��200
	float threshholdLevel;

	// click�Ŀ�ȣ�Խ��Խ����
	// ��Χ��[0,40] Ĭ��ֵ��20
	float clickWidth;

	// ��Ƶ���ݲ�����
	// Ĭ��ֵ��48000
	int sampleRate;

	// ��������п��������������
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
	// ��ʼ���ؼ�ָ�뼰�ؼ���Դ
	virtual NXRESULT Init() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// �ͷ��ؼ���Դ���ؼ�ָ��
	virtual NXRESULT Release() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// �����ؼ�����
	// param���ؼ������ṹ��ָ��
	virtual NXRESULT SetParameter(void *param) = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// ��ȡ�ؼ�����
	// param���ؼ������ṹ��ָ��
	virtual NXRESULT GetParameter(void *param) = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// ��Init֮�󣬿�ʼ��ǰ�������Ĵ���
	virtual NXRESULT StartSequence() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// ����release֮ǰ��������ǰ�������Ĵ���
	virtual NXRESULT EndSequence() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// ������Ƶ�������ݣ�����ֵΪ��һ���ĸ�������ȡֵ��Χ[-1.0,1.0]
	// input : ��ά���飬���ڶ���������е�������磺������������������ʱ�����input[0]
	// numSamples : �������������
	virtual NXRESULT PushSamples(float **input, int numSamples) = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// �����Ƶ�������ݣ�����ֵΪ��һ���ĸ�������ȡֵ��Χ[-1.0,1.0]
	// output : ��ά���飬���ڶ����������е�����������������ʱ����output[0]��ȡ����
	// numSamples : ȡ������������
	virtual NXRESULT ReceiveSamples(float **output, int numSamples) = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// ����Flush���������ؼ�������δ��������ݴ����꣬�ڴ�������в�Ҫʹ��Flush
	// �����뾲�����ݣ�Ӧ�÷��ڴ������֮�����Flush���������������Flush��������ĩβ
	// ��������ݲ������
	virtual NXRESULT Flush() = 0;
	//////////////////////////////////////////////////////////////////////////////////
	// ���ڽ��ؼ����롢��������е�������գ���StartSequence֮ǰʹ��
	virtual NXRESULT Clear() = 0;
};

//////////////////////////////////////////////////////////////////////////////////
#endif	// __INXAUDIOEFFECT_H__
