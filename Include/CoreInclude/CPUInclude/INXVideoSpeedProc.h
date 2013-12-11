//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: �򵥱�����ӿ�������.
//
// Created: ZhaoZhimeng @ 2009-02-13
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_SPEED_PROC_H__
#define __I_NX_VIDEO_SPEED_PROC_H__

#include ".\NXBasic.h"
#include ".\NXVideoProcDef.h"
#include ".\INXVideoShareBufferCB.h"

//////////////////////////////////////////////////////////////////////////
// ����ֵ����
enum NXSPEED_PROC_RET
{
	NXSPEED_PROC_RET_SUCCEED		= 1,
	NXSPEED_PROC_RET_FAILED			= 0,

	NXSPEED_PROC_RET_INVALID_PARAM	= -1,		// ��Ч�������
	NXSPEED_PROC_RET_NOT_INIT		= -2,		// ������δ����ʼ����������б任��
	NXSPEED_PROC_RET_ALREADY_INIT	= -3,		// �������ѱ���ʼ�����������ò�����
	NXSPEED_PROC_RET_NONSUPPORT		= -4,		// ��֧�ֵ��������
	NXSPEED_PROC_RET_NOSHARE_INPUT	= -5,		// ��ʼ��ʱδע�����빲����

	NXSPEED_PROC_RET_OUT_END		= 2,		// ����ģʽ�£����������־
	NXSPEED_PROC_RET_OUT_EMPTY		= 3,		// ����ģʽ�£����Ϊ�ձ�־
};

//////////////////////////////////////////////////////////////////////////
// ��������ģʽ����
enum NXSPEED_PROC_MODE
{
	NXSPEED_PROC_MODE_SEEK,			// ��������ģʽ����Ҫʹ���������任����
	NXSPEED_PROC_MODE_PLAY,			// ��������ģʽ���ڲ��Զ����������任�ź�

	NXSPEED_PROC_MODE_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////
// �����㷨����
// FilterOff			��������ӳ�䣬ֱ�ӿ���������
// FieldCut				��������ӳ�䣬����ӳ�䶥����׳�ӳ��׳�ʱֱ�ӿ��������ݣ�������в�ֵ
// FieldMix				��������ӳ�䣬����ǰ���������л�ϲ�ֵ
// FrameCut				��֡����ӳ�䣬ֱ�ӿ���֡����
// FrameMix				��֡����ӳ�䣬����ǰ����֡���л�ϲ�ֵ
// Still1st				��ֹģʽ���̶�ӳ��ΪԴ�زĵĵ�һ�������λ��ӳ�䲻ƥ��ʱ�����в�ֵ
// Still2nd				��ֹģʽ���̶�ӳ��ΪԴ�زĵĵڶ��������λ��ӳ�䲻ƥ��ʱ�����в�ֵ
// StillFrame			��ֹģʽ���̶�ӳ��ΪԴ�زĵĵ�һ֡
// MotionAdaptiveCut	�� FieldCut ģʽ������˶�����
// MotionAdaptiveMix	�� FieldMix ģʽ������˶�����
enum NXSPEED_PROC_ARITH
{
	NXSPEED_PROC_ARITH_FILTER_OFF,			// FilterOff
	NXSPEED_PROC_ARITH_FIELD_CUT,			// FieldCut
	NXSPEED_PROC_ARITH_FIELD_MIX,			// FieldMix
	NXSPEED_PROC_ARITH_FRAME_CUT,			// FrameCut
	NXSPEED_PROC_ARITH_FRAME_MIX,			// FrameMix
	NXSPEED_PROC_ARITH_STILL_FIRST,			// Still1st
	NXSPEED_PROC_ARITH_STILL_SECOND,		// Still2nd
	NXSPEED_PROC_ARITH_STILL_FRAME,			// StillFrame
	NXSPEED_PROC_ARITH_MOTION_ADAPTIVE_CUT,	// MotionAdaptiveCut
	NXSPEED_PROC_ARITH_MOTION_ADAPTIVE_MIX,	// MotionAdaptiveMix

	NXSPEED_PROC_ARITH_UNKNOWN,
};


//////////////////////////////////////////////////////////////////////////
// �ӿ�������
interface NOVTABLE INXVideoSpeedProc
{
	// ���û�ȡ���ٴ������������Դ�زĳ��ȡ����桢���ظ�ʽ������ģʽ�����ʡ�������ʱ��
	// ��1��Դ�زĳ��ȱ������ 0��һ��Ǳ�������һ���ز��Ϊ 12 ��Сʱ��
	// ��2��ePixelFmt ���Ϊ NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 ��ʽ��
	// Ҫ�����������ϸ��� Y һƬ��U һƬ��V һƬ˳��洢����ͬ����������� RAW_DATA��
	// ��ʱ nPitch ��ʾ Y һ��ռ�õ��ֽڿ�ȣ��� U/V ����ռ���ֽڿ��Ϊ Y ��һ�롣
	// ��3��Ϊ��ȷ��������ȷ���ٶȾ���ֵ��С֧�ֵ� 1e-6�����ٶ�С�� 0 ʱ������Դ�زĵĵ����ڶ�����ʼ����ӳ�䡣
	// ��4����ͬ��Ƶ��ʽ�ĳ��������в��죬SD50i��HD59.94i��HD50i �������Ȳ����� SD59.94i �򰴵׳��Ȳ���
	// ��5��ֻ��������ʱ nInClipLength ������������С�� 0 ��ֵ����ʱ���������ܳ�δ֪����ɼ��У���
	// ��6��nFrameWidth��nFrameHeight ������ȫ������ʵ�ʴ��������� SetProcArea �ӿ�����
	// �����������󽫴�������״̬��ֱ��ʹ���ߵ��� SetInClipLength() �ӿڸı������ܳ���
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ��ñ���������������ת��
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	//			NXSPEED_PROC_RET_NONSUPPORT			���������Ч����֧��
	virtual int SetProcParam(IN int nInClipLength,
							 IN int nFrameWidth,
							 IN int nFrameHeight,
							 IN NXVIDEO_PIXEL_FMT ePixelFmt,
							 IN NXSPEED_PROC_MODE eProcMode,
							 IN NXSPEED_PROC_ARITH eProcArith,
							 IN double dSpeed,
							 IN bool bTopFieldFirst) = 0;
	virtual int GetProcParam(OUT int &nInClipLength,
							 OUT int &nFrameWidth,
							 OUT int &nFrameHeight,
							 OUT NXVIDEO_PIXEL_FMT &ePixelFmt,
							 OUT NXSPEED_PROC_MODE &eProcMode,
							 OUT NXSPEED_PROC_ARITH &eProcArith,
							 OUT double &dSpeed,
							 OUT bool &bTopFieldFirst) = 0;
	// ע�����빲����ص��ӿ�
	// ��1�����ڱ����㷨�Ķ����ԣ��������������ȫ���㷨�����ƿأ����Ϊ�˼��㷨���̣�
	// �Լ�����㷨Ч�ʣ��������빲����ʵ�ֻ�ȡ���롣
	// ��2��Ҫ����԰�֡����ȡ���ݣ���Ҫ������Ҫ����ס��һ֡�����ݣ�����Ч�ʻ��½���
	// ��3��������ֻ��ʹ�����빲����ص��ӿ��еĶ�ȡ�ӿڶԣ��Ի�ȡ����֡���ݡ�
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ��ñ���������������ת��
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	virtual int RegInputShareBufferCB(IN INXVideoShareBufferCB *pShareCB) = 0;
	// ע�����������ص��ӿ�
	// ��1���ù���Ϊ�������ģʽ��Ч�����裬ע��ɹ��󣬾������������� GetOutputFrame �ӿڡ�
	// ��2���ڱ���ģʽ�£��ù�����Ч��
	// ��3��������ֻ��ʹ�����������ص��ӿ��е�д��ӿڶԣ��Ի�ȡ��д��Ķ�֡���󣬲�������ݡ�
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ��ñ���������������ת��
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	virtual int RegOutputShareBufferCB(IN INXVideoShareBufferCB *pShareCB) = 0;
	// ���û�ȡ�ڲ������С��ȱʡ��Ϊ 6 ֡������Ϊ 30 ֡������Ϊ 2 ֡��
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ��ñ���������������ת��
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	virtual int SetBufferRingSize(IN int nRingSize) = 0;
	virtual int GetBufferRingSize(OUT int &nRingSize) = 0;
	// ��ʼ��������
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ��ñ���������������ת��
	//			NXSPEED_PROC_RET_NOSHARE_INPUT		δע�����빲����
	virtual int Initialize() = 0;
	// ��ȡ���ٺ���زĳ���
	// ��1��Ŀ���زĳ��ȵ��� (int)(nInClipLength / fabs(dSpeed) + 0.4999999998)��������������ķ�ʽ��
	// ��2��������ʱ nInClipLength ��������С�� 0���� nOutClipLength ����ҲС�� 0��
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_NOT_INIT			������δ����ʼ��
	virtual int GetOutClipLength(OUT int &nOutClipLength) = 0;
	// ����������
	// ����		NXSPEED_PROC_RET_SUCCEED		�ɹ�
	//			NXSPEED_PROC_RET_FAILED			ʧ�ܣ��ñ���������������ת��
	//			NXSPEED_PROC_RET_NOT_INIT		������δ����ʼ��
	virtual int StartConv() = 0;
	// ������һ֡����
	// ��1���������ڲ����ܴ��ڱ任�̣߳���˿�����Ҫ�ȴ���
    // ��2���ڱ���ģʽ�£�nFrameIndex Ϊ�����������ָ����Ҫ����Ľ��֡������
    // ��3�������� bWaitConv = true ʱ���κ�ģʽ�¶���ȴ����������ŷ��ء�
	// ��4��������ģʽ�£����ע�������������ص��ӿڣ��ýӿ�ʧЧ��
	// ��5��������ģʽ�£�nFrameIndex Ϊ���������Ϊ��ǰ��������֡������
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ�������δ����ʼ��������ԭ��
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	//			NXSPEED_PROC_RET_OUT_EMPTY			����ģʽ�£��������Ϊ�գ������� bWaitConv = false ʱ�����ܷ��ظ�ֵ��
	//			NXSPEED_PROC_RET_OUT_END			����ģʽ�£����������־
	virtual int GetOutputFrame(OUT unsigned char *pucBuf,		// ���Ŀ��
							   IN int nPitch,					// һ���ź�ռ�õ��ֽڿ��
							   IN OUT int &nFrameIndex,			// ���֡��
							   IN bool bWaitConv = true) = 0;	// �Ƿ���б任�ȴ���false ���������أ�
	// ֹͣ������
	// ����		NXFMTCONV_RET_SUCCEED		�ɹ�
	//			NXFMTCONV_RET_FAILED		ʧ�ܣ��ñ���������δ������
	virtual int StopConv() = 0;
	// �ͷű�����
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ��ñ���������δ����ʼ��
	virtual int Release() = 0;
	// ��������״̬��ȡ
	virtual bool IsInitialized() = 0;
	virtual bool IsProcessing() = 0;	// ����ģʽ�²������������ʱ���������жϱ������Ƿ��Ѿ�ֹͣ�������ٽ�������
	// ���������й������޸��ٶ�
	// ��1����չ�ͽӿڣ�Ϊ�򵥱��������ӽ׶��ͱ���֧�֡�
	// ��2�����ڱ������Գ�ʵʱ״̬��ת������Ⱦ��֡�޷������������������ʱЧӦ�����ٶȱ��ˣ�����Ӧ�ͺ�
	// ��3����֡���㷨��֧���ٶ��޸ġ�
	// ��4������ٶȵķ��ŷ����仯�����ܿ��ܵ��±���������������������ʹ�á�
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ��ñ���������δ����ת
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	//			NXSPEED_PROC_RET_NONSUPPORT			���������Ч����֧��
	virtual int ChangeSpeed(IN double dNewSpeed,		// ���ٶ�
							IN int nOutFrameStart) = 0;	// ���ٶȷ������õ���ʼ֡
	// ���������й����������µ������ܳ�
	// ��1����չ�ͽӿڣ�Ϊ�򵥱��������Ӳɼ�ģʽ֧�֣��������ܳ�δ֪��
	// ��2��������������µ������ܳ���ʱ��������ܳ���
	// ��3������ٶ�Ϊ������ nInClipLength ����Ϊ������������Զ������֡����
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ��ñ���������δ����ת
	//			NXSPEED_PROC_RET_INVALID_PARAM		���������Ч
	virtual int SetInClipLength(IN int nInClipLength) = 0;	// �������ܳ�
	// ����ʵ�ʴ���������Ϣ
	// ȫ��������Ϣ�� SetProcParam �ӿ�����
	// ����		NXSPEED_PROC_RET_SUCCEED			�ɹ�
	//			NXSPEED_PROC_RET_FAILED				ʧ�ܣ�����������Ч
	virtual int SetProcArea(IN int nProcX,				// ������㣬�����ͼ�����Ͻ�
							IN int nProcY,
							IN int nProcWidth,			// ������
							IN int nProcHeight) = 0;
	virtual int GetProcArea(IN int& nProcX,				// ������㣬�����ͼ�����Ͻ�
							IN int& nProcY,
							IN int& nProcWidth,			// ������
							IN int& nProcHeight) = 0;
	// ȥ�����㷨���ͣ�Ĭ��Ϊ NXVIDEO_DEINTERLACE_TAPS_4
	virtual void SetDeinterlaceArith(IN NXVIDEO_DEINTERLACE_ARITH eArith) = 0;
	virtual NXVIDEO_DEINTERLACE_ARITH GetDeinterlaceArith() const = 0;
};


#endif		// #ifndef __I_NX_VIDEO_SPEED_PROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
