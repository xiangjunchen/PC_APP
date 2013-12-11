//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ��ʽ�任����ӿ�������.
//				��������ʽת�������±任��.
//
// Created: ZhaoZhimeng @ 2008-11-19
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_FMT_CONV_H__
#define __I_NX_VIDEO_FMT_CONV_H__

#include ".\NXBasic.h"
#include ".\NXVideoProcDef.h"
#include ".\INXVideoShareBufferCB.h"

//////////////////////////////////////////////////////////////////////////
// ����ֵ����
enum NXFMTCONV_RET
{
	NXFMTCONV_RET_SUCCEED		= 1,
	NXFMTCONV_RET_FAILED		= 0,

	NXFMTCONV_RET_INVALID_PARAM		= -1,	// ��Ч�������
	NXFMTCONV_RET_NOT_INIT			= -2,	// �任��δ����ʼ�������������任��
	NXFMTCONV_RET_ALREADY_INIT		= -3,	// �任���ѱ���ʼ���������ʼ����
	NXFMTCONV_RET_NONSUPPORT		= -4,	// ��֧�ֵ��������

	NXFMTCONV_RET_IN_FULL		= 2,		// ���뻺��Ϊ�����ȴ��任�е�״̬��
	NXFMTCONV_RET_OUT_EMPTY		= 3,		// �������Ϊ�գ��ȴ��任�е�״̬��
	NXFMTCONV_RET_OUT_END		= 4,		// �����������ֹͣ�任��������һ֡���״̬��
};

//////////////////////////////////////////////////////////////////////////
// ����ʽ����
enum NXFMTCONV_TYPE
{
	NXFMTCONV_TYPE_MOTION_DETECT,		// �˶����ģʽ
	NXFMTCONV_TYPE_STATIC_INTERLACED,	// ��֡ģʽ��������
	NXFMTCONV_TYPE_STATIC_PROGRESSIVE,	// ��֡ģʽ��֡����

	NXFMTCONV_TYPE_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////
// �ӿ�������
interface NOVTABLE INXVideoFmtConv
{
	// ���û�ȡ�����źŸ�ʽ���������桢�߱��塢������������ظ�ʽ��֡�ʡ���������
	// ��1��������Ƶ��ʽ���ص㣬��������ϱ�׼�ķ���ֵ������任�����ܲ�����Ӧ��
	// ��2���߱���������ɫ�ռ�任���߱������������ͬһ���ź���ʽ�µķֱ�������
	// ��3��eAspect ��������������㣬����һ��Ϊ 16:9������һ��Ϊ 4:3��
	// ��4���� ePixelFmt ����Ϊ NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 ���ָ�ʽʱ��
	// �ڵ��� SetInputFrame ��ʹ�ù������뻺��ʱ���ϸ�Ҫ��
	// ��5����������������֡�����һ������ֻ�������仯�������±任��������������֡��
	// ��ͬ�����ͬʱ����ʱ�յı仯���� PAL �� NTSC ֮��ı任��
	// ��6������Ҫ����ʱ��仯ʱ����Ƶ֡�ĳ��������Ǽ���ʱ��ӳ��Ĺؼ��������Ӱ��任������
	// ע�⣺SD50i��HD59.94i��HD50i �������Ȳ����� SD59.94i �򰴵׳��Ȳ���
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������������ת��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	//			NXFMTCONV_RET_NONSUPPORT		���������Ч����֧��
	virtual int SetInputVideoFmt(IN int nWidth,
								 IN int nHeight,
								 IN NXVIDEO_DEFINITION eDefinition,
								 IN NXVIDEO_ASPECT eAspect,
								 IN NXVIDEO_PIXEL_FMT ePixelFmt,
								 IN double dFrameRate,
								 IN bool bTopFieldFirst) = 0;
	virtual int GetInputVideoFmt(OUT int &nWidth,
								 OUT int &nHeight,
								 OUT NXVIDEO_DEFINITION &eDefinition,
								 OUT NXVIDEO_ASPECT &eAspect,
								 OUT NXVIDEO_PIXEL_FMT &ePixelFmt,
								 OUT double &dFrameRate,
								 OUT bool &bTopFieldFirst) = 0;
	// ���û�ȡ����źŸ�ʽ���������桢�߱��塢������������ظ�ʽ��֡�ʡ���������
	// ��1��������Ƶ��ʽ���ص㣬��������ϱ�׼�ķ���ֵ������任�����ܲ�����Ӧ��
	// ��2���߱���������ɫ�ռ�任���߱������������ͬһ���ź���ʽ�µķֱ�������
    // ��3��eAspect ��������������㣬����һ��Ϊ 16:9������һ��Ϊ 4:3��
    // ��4���� ePixelFmt ����Ϊ NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 ���ָ�ʽʱ��
    // �ڵ��� GetOutputFrame ��ʹ�ù����������ʱ���ϸ�Ҫ��
	// ��5����������������֡�����һ������ֻ�������仯�������±任��
	// ������������֡�ʲ�ͬ�����ͬʱ����ʱ�յı仯���� PAL �� NTSC ֮��ı任��
	// ��6������Ҫ����ʱ��仯ʱ����Ƶ֡�ĳ��������Ǽ���ʱ��ӳ��Ĺؼ��������Ӱ��任������
	// ע�⣺SD50i��HD59.94i��HD50i �������Ȳ����� SD59.94i �򰴵׳��Ȳ���
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������������ת��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	//			NXFMTCONV_RET_NONSUPPORT		���������Ч����֧��
	virtual int SetOutputVideoFmt(IN int nWidth,
								  IN int nHeight,
								  IN NXVIDEO_DEFINITION eDefinition,
								  IN NXVIDEO_ASPECT eAspect,
								  IN NXVIDEO_PIXEL_FMT ePixelFmt,
								  IN double dFrameRate,
								  IN bool bTopFieldFirst) = 0;
	virtual int GetOutputVideoFmt(OUT int &nWidth,
								  OUT int &nHeight,
								  OUT NXVIDEO_DEFINITION &eDefinition,
								  OUT NXVIDEO_ASPECT &eAspect,
								  OUT NXVIDEO_PIXEL_FMT &ePixelFmt,
								  OUT double &dFrameRate,
								  OUT bool &bTopFieldFirst) = 0;
	// ���û�ȡ��������������������͡����ô��ڡ����ڴ�С���˶������������������ʽ��
	// ȱʡֵ�ֱ�Ϊ		NXVIDEO_ADAPTION_HEIGHT�����߶����䣩
	//					NXVIDEO_SAM_WINDOW_KAISER��kaiser ���ƽ���
	//					NXVIDEO_SAM_TAPS_8��8-taps ������
	//					NXVIDEO_DEINTERLACE_TAPS_4��4-taps �˶�������
	//					NXVIDEO_ANTIALIAS_NONE���޿������
	//					NXFMTCONV_TYPE_MOTION_DETECT�������˶���⣩
	// ��1���� eSamWindow = NXVIDEO_SAM_WINDOW_SONY ʱ��eSamTaps ֻ֧�ֵ� NXVIDEO_SAM_TAPS_8��
	// ��2��eMotionArith ֻ�� eConvType = NXFMTCONV_TYPE_MOTION_DETECT ʱ�����á�
	// ��3���� eConvType = NXFMTCONV_TYPE_STATIC_PROGRESSIVE ʱ���������������ĳ���������ͬʱ������ɴ���
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������������ת��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	//			NXFMTCONV_RET_NONSUPPORT		���������Ч����֧��
	virtual int SetConvParam(IN NXVIDEO_ADAPTION eAdaption,
							 IN NXVIDEO_SAM_WINDOW eSamWindow,
							 IN NXVIDEO_SAM_TAPS eSamTaps,
							 IN NXVIDEO_DEINTERLACE_ARITH eMotionArith,
							 IN NXVIDEO_ANTIALIAS eAntiAlias,
							 IN NXFMTCONV_TYPE eConvType) = 0;
	virtual int GetConvParam(OUT NXVIDEO_ADAPTION &eAdaption,
							 OUT NXVIDEO_SAM_WINDOW &eSamWindow,
							 OUT NXVIDEO_SAM_TAPS &eSamTaps,
							 OUT NXVIDEO_DEINTERLACE_ARITH &eMotionArith,
							 OUT NXVIDEO_ANTIALIAS &eAntiAlias,
							 OUT NXFMTCONV_TYPE &eConvType) = 0;
	// ע�����빲����ص��ӿ�
	// ��1���ù���Ϊ���Ч�����裬ע��ɹ��󣬱任���ڲ��������������з����̡�
	// ��2��һ��ע��ص��ɹ����������������� SetInputFrame �ӿڡ�
	// ��3�����빲�������ݵĶ����� SetInputFrame �ӿ���ͬ��
	// ��4��Ҫ����԰�֡����ȡ���ݣ���Ҫ������Ҫ����ס��һ֡�����ݣ�����Ч�ʻ��½���
	// ��5���任��ֻ��ʹ�����빲����ص��ӿ��еĶ�ȡ�ӿڶԣ��Ի�ȡ����֡���ݡ�
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������������ת��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	virtual int RegInputShareBufferCB(IN INXVideoShareBufferCB *pShareCB) = 0;
	// ע�����������ص��ӿ�
	// ��1���ù���Ϊ���Ч�����裬ע��ɹ��󣬱任���ڲ��������������з����̡�
	// ��2��һ��ע��ص��ɹ����������������� GetOutputFrame �ӿڡ�
	// ��3��������������ݵĶ����� GetOutputFrame �ӿ���ͬ��
	// ��4���任��ֻ��ʹ�����������ص��ӿ��е�д��ӿڶԣ��Ի�ȡ��д��Ķ�֡���󣬲�������ݡ�
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������������ת��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	virtual int RegOutputShareBufferCB(IN INXVideoShareBufferCB *pShareCB) = 0;
	// ���û�ȡ������������С��ȱʡ��Ϊ 6 ֡������Ϊ 30 ֡����������Ϊ 2 ֡��
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������������ת��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	virtual int SetBufferRingSize(IN int nInput, IN int nOutput) = 0;
	virtual int GetBufferRingSize(OUT int &nInput, OUT int &nOutput) = 0;
	// ��ʼ���任�������ô������̡����㴦������ȣ�
	// ����		NXFMTCONV_RET_SUCCEED		�ɹ�
	//			NXFMTCONV_RET_FAILED		ʧ�ܣ��ñ任������������ת��
	//			NXFMTCONV_RET_ALREADY_INIT	�任���ѱ���ʼ��
	virtual int Initialize() = 0;
	// �����任��
	// ����		NXFMTCONV_RET_SUCCEED		�ɹ�
	//			NXFMTCONV_RET_FAILED		ʧ�ܣ��ñ任������������ת��
	//			NXFMTCONV_RET_NOT_INIT		�任��δ����ʼ��
	virtual int StartConv() = 0;
	// ����Դһ֡����
	// ��1�������任����Ҫ��ӵ� 0 ֡��ʼ����С��������롣
	// ��2���������źŸ�ʽ ePixelFmt = NXVIDEO_PIXEL_FMT_YUV422 || NXVIDEO_PIXEL_FMT_YUV420 ʱ��
	// Ҫ�����������ϸ��� Y һƬ��U һƬ��V һƬ˳��洢����ͬ����������� RAW_DATA��
	// ��ʱ nPitch ��ʾ Y һ��ռ�õ��ֽڿ�ȣ��� U/V ����ռ���ֽڿ��Ϊ Y ��һ�롣
	// ��3��Ҫ������ͼ�����Ļ���Ͻǵ����ڴ������غϣ���������Ч������
	// ��4����ע�����빲����ɹ��󣬽��鲻�������øýӿڣ����ܻ�ʧЧ��
	// ��5���� pucBuf = NULL ʱ��nFrameIndex �᷵�ر任����ǰ������֡�ţ�
	// ������� nFrameIndex = -1 ���ʾ��ǰ�任��������Ϊ�գ���Ҫ�����ߵȴ���
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������δ������
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	//			NXFMTCONV_RET_IN_FULL			���뻺������������ bWaitConv = false ʱ�����ܷ��ظ�ֵ��
	virtual int SetInputFrame(IN unsigned char *pucBuf,			// ��������
							  IN int nPitch,					// һ���ź�ռ�õ��ֽڿ��
							  IN OUT int &nFrameIndex,			// ����֡�ţ��� pucBuf = NULL ʱ�ᱻ��䣩
							  IN bool bWaitConv = true) = 0;	// ���뻺����ʱ�Ƿ���еȴ���false ���������أ�
	// ������һ֡����
	// ��1���� pucBuf = NULL ʱ���任����Ϊ������Ҫ����һ֡�Ľ������������ڵĻ���
    // ��2����ע�����������ɹ��󣬽��鲻�������øýӿڣ����ܻ�ʧЧ��
    // ��3��������źŸ�ʽ ePixelFmt = NXVIDEO_PIXEL_FMT_YUV422 || NXVIDEO_PIXEL_FMT_YUV420 ʱ��
    // Ҫ�����������ϸ��� Y һƬ��U һƬ��V һƬ˳��洢����ͬ����������� RAW_DATA��
    // ��ʱ nPitch ��ʾ Y һ��ռ�õ��ֽڿ�ȣ��� U/V ����ռ���ֽڿ��Ϊ Y ��һ�롣
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������δ������
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	//			NXFMTCONV_RET_OUT_EMPTY			�������Ϊ�գ������� bWaitConv = false ʱ�����ܷ��ظ�ֵ��
	//			NXFMTCONV_RET_OUT_END			�任������ֻ����ֹͣ�任��ŷ��ظ�ֵ��
	virtual int GetOutputFrame(OUT unsigned char *pucBuf,		// ���Ŀ��
							   IN int nPitch,					// һ���ź�ռ�õ��ֽڿ��
							   OUT int &nFrameIndex,			// ���֡��
							   IN bool bWaitConv = true) = 0;	// ��������ʱ�Ƿ���еȴ���false ���������أ�
	// ֹͣ�任��
	// ��1���� bForced = false ʱ���ýӿ�ֻ����ֹͣ��־�����任�߳�ʵ�ʿ������ڽ��У�ֱ�����뻺��Ϊ�ա�
	// ��2���� bForced = false ʱ��Ҫ����� GetOutputFrame��ֱ�����ر任������������ܵ����߳��������޷�ֹͣ��
	// ��3���� bForced = true ʱ��ֱ���������������־�����������һ�α任����������ֹͣ�任����
	// ����		NXFMTCONV_RET_SUCCEED		�ɹ�
	//			NXFMTCONV_RET_FAILED		ʧ�ܣ��ñ任������δ������
	virtual int StopConv(IN bool bForced = false) = 0;
	// �ͷű任��
	// ��1������任���ѱ�����ֹͣ��־�����任���ڽ���ʱ�������ǿ��ֹͣ��
	// ����		NXFMTCONV_RET_SUCCEED		�ɹ�
	//			NXFMTCONV_RET_FAILED		ʧ�ܣ��ñ任������δ��ֹͣ
	virtual int Release() = 0;
	// �任����״̬��ȡ
	virtual bool IsInitialized() = 0;
	virtual bool IsProcessing() = 0;	// ֹͣ�任���󣬸�״̬�����Ի����һ��ʱ��
	// ���õ��ӵ�Ӱ�����־
	// ��1��Ĭ������£��ڲ���������֡��ӳ����д���
	// ��2����Ӱ��Ƭ�źţ�24FPS��ת��Ϊ�����źţ�PAL��NTSC��ʱ���Ƽ�ʹ�õ��ӵ�Ӱ�����Եõ���������
	virtual void SetTelecine(IN bool bUseTelecine) = 0;
	virtual bool GetTelecine() const = 0;
	// ������Ƶ�ź�ɨ�跽ʽ
	// Ĭ�Ͼ�Ϊ NXVIDEO_SCAN_INTERLACED
	virtual void SetScanType(IN NXVIDEO_SCAN bInputScan, IN NXVIDEO_SCAN bOutputScan) = 0;
	virtual void GetScanType(OUT NXVIDEO_SCAN& bInputScan, OUT NXVIDEO_SCAN& bOutputScan) const = 0;
};


#endif		// #ifndef __I_NX_VIDEO_FMT_CONV_H__
//////////////////////////////////////////////////////////////////////////
// End of file.