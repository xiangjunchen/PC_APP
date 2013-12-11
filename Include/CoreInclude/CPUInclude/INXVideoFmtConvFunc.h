//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ��ʽ�任����ӿ�������.
//				��������ʽת�������±任��.
//
// Author: LaiShoubo @ 2013-01-08 Chengdu
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_FMT_CONV_FUNC_H__
#define __I_NX_VIDEO_FMT_CONV_FUNC_H__

#include ".\INXVideoFmtConv.h"
#include ".\INXVideoSpeedFunc.h"


//////////////////////////////////////////////////////////////////////////
// �ӿ�������
interface INXVideoFmtConvFunc
{
	// ���û�ȡ�����źŸ�ʽ���������桢�߱��塢������������ظ�ʽ��֡�ʡ���������ɨ�跽ʽ��
	// ��1��������Ƶ��ʽ���ص㣬��������ϱ�׼�ķ���ֵ������任�����ܲ�����Ӧ��
	// ��2���߱���������ɫ�ռ�任���߱������������ͬһ���ź���ʽ�µķֱ�������
	// ��3��eAspect ��������������㣬����һ��Ϊ 16:9������һ��Ϊ 4:3��
	// ��4����������������֡�����һ������ֻ�������仯�������±任��������������֡��
	// ��ͬ�����ͬʱ����ʱ�յı仯���� PAL �� NTSC ֮��ı任��
	// ��5������Ҫ����ʱ��仯ʱ����Ƶ֡�ĳ��������Ǽ���ʱ��ӳ��Ĺؼ��������Ӱ��任������
	// ��6��֧�ֽ�ȡ��Ƶ����������
	// ע�⣺SD50i��HD59.94i��HD50i �������Ȳ����� SD59.94i �򰴵׳��Ȳ���
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ������ѳ�ʼ��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	//			NXFMTCONV_RET_NONSUPPORT		���������Ч����֧��
	virtual int SetInputVideoFmt(IN int nX,
								 IN int nY,
								 IN int nWidth,
								 IN int nHeight,
								 IN int nVideoWidth,
								 IN int nVideoHeight,
								 IN NXVIDEO_DEFINITION eDefinition,
								 IN NXVIDEO_ASPECT eAspect,
								 IN NXVIDEO_PIXEL_FMT ePixelFmt,
								 IN double dFrameRate,
								 IN bool bTopFieldFirst,
								 IN NXVIDEO_SCAN eScan = NXVIDEO_SCAN_INTERLACED) = 0;
	virtual int GetInputVideoFmt(OUT int& nX,
								 OUT int& nY,
								 OUT int& nWidth,
								 OUT int& nHeight,
								 OUT int& nVideoWidth,
								 OUT int& nVideoHeight,
								 OUT NXVIDEO_DEFINITION& eDefinition,
								 OUT NXVIDEO_ASPECT& eAspect,
								 OUT NXVIDEO_PIXEL_FMT& ePixelFmt,
								 OUT double& dFrameRate,
								 OUT bool& bTopFieldFirst,
								 OUT NXVIDEO_SCAN& eScan) const = 0;
	// ���û�ȡ����źŸ�ʽ���������桢�߱��塢������������ظ�ʽ��֡�ʡ���������ɨ�跽ʽ��
	// ��1��������Ƶ��ʽ���ص㣬��������ϱ�׼�ķ���ֵ������任�����ܲ�����Ӧ��
	// ��2���߱���������ɫ�ռ�任���߱������������ͬһ���ź���ʽ�µķֱ�������
	// ��3��eAspect ��������������㣬����һ��Ϊ 16:9������һ��Ϊ 4:3��
	// ��4����������������֡�����һ������ֻ�������仯�������±任��
	// ������������֡�ʲ�ͬ�����ͬʱ����ʱ�յı仯���� PAL �� NTSC ֮��ı任��
	// ��5������Ҫ����ʱ��仯ʱ����Ƶ֡�ĳ��������Ǽ���ʱ��ӳ��Ĺؼ��������Ӱ��任������
	// ��6��֧�ֽ�ȡ��Ƶ����������
	// ע�⣺SD50i��HD59.94i��HD50i �������Ȳ����� SD59.94i �򰴵׳��Ȳ���
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ������ѳ�ʼ��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	//			NXFMTCONV_RET_NONSUPPORT		���������Ч����֧��
	virtual int SetOutputVideoFmt(IN int nX,
								  IN int nY,
								  IN int nWidth,
								  IN int nHeight,
								  IN int nVideoWidth,
								  IN int nVideoHeight,
								  IN NXVIDEO_DEFINITION eDefinition,
								  IN NXVIDEO_ASPECT eAspect,
								  IN NXVIDEO_PIXEL_FMT ePixelFmt,
								  IN double dFrameRate,
								  IN bool bTopFieldFirst,
								  IN NXVIDEO_SCAN eScan = NXVIDEO_SCAN_INTERLACED) = 0;
	virtual int GetOutputVideoFmt(OUT int& nX,
								  OUT int& nY,
								  OUT int& nWidth,
								  OUT int& nHeight,
								  OUT int& nVideoWidth,
								  OUT int& nVideoHeight,
								  OUT NXVIDEO_DEFINITION& eDefinition,
								  OUT NXVIDEO_ASPECT& eAspect,
								  OUT NXVIDEO_PIXEL_FMT& ePixelFmt,
								  OUT double& dFrameRate,
								  OUT bool& bTopFieldFirst,
								  OUT NXVIDEO_SCAN& eScan) const = 0;
	// ���û�ȡ��������������������͡����ô��ڡ����ڴ�С���˶������������������ʽ�������任��
	// ȱʡֵ�ֱ�Ϊ		NXVIDEO_ADAPTION_HEIGHT�����߶����䣩
	//					NXVIDEO_SAM_WINDOW_KAISER��kaiser ���ƽ���
	//					NXVIDEO_SAM_TAPS_8��8-taps ������
	//					NXVIDEO_DEINTERLACE_TAPS_4��4-taps �˶�������
	//					NXVIDEO_ANTIALIAS_NONE���޿������
	//					NXFMTCONV_TYPE_MOTION_DETECT�������˶���⣩
	// ��1���� eSamWindow = NXVIDEO_SAM_WINDOW_SONY ʱ��eSamTaps ֻ֧�ֵ� NXVIDEO_SAM_TAPS_8��
	// ��2��eMotionArith ֻ�� eConvType = NXFMTCONV_TYPE_MOTION_DETECT ʱ�����á�
	// ��3���� eConvType = NXFMTCONV_TYPE_STATIC_PROGRESSIVE ʱ���������������ĳ���������ͬʱ������ɴ���
	// ��4�����������Ҫ��ڱ�ʱ����Ӧ�����ʽ�ĺ�ɫֵ�ɲ��� bSafeColor �����Ƿ񰴰�ȫֵ����
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ������ѳ�ʼ��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	//			NXFMTCONV_RET_NONSUPPORT		���������Ч����֧��
	virtual int SetConvParam(IN NXVIDEO_ADAPTION eAdaption,
							 IN NXVIDEO_SAM_WINDOW eSamWindow,
							 IN NXVIDEO_SAM_TAPS eSamTaps,
							 IN NXVIDEO_DEINTERLACE_ARITH eMotionArith,
							 IN NXVIDEO_ANTIALIAS eAntiAlias,
							 IN NXFMTCONV_TYPE eConvType,
							 IN bool bUseTelecine = false,
							 IN bool bSafeColor = true) = 0;
	virtual int GetConvParam(OUT NXVIDEO_ADAPTION& eAdaption,
							 OUT NXVIDEO_SAM_WINDOW& eSamWindow,
							 OUT NXVIDEO_SAM_TAPS& eSamTaps,
							 OUT NXVIDEO_DEINTERLACE_ARITH& eMotionArith,
							 OUT NXVIDEO_ANTIALIAS& eAntiAlias,
							 OUT NXFMTCONV_TYPE& eConvType,
							 OUT bool& bUseTelecine,
							 OUT bool& bSafeColor) const = 0;
	// ע�����빲����ص��ӿ�
	// ��1��Ҫ����԰�֡����ȡ���ݣ���Ҫ��ȥ��֯ʱ������Ҫ����ס��һ֡�����ݣ�����Ч�ʻ��½���
	// ��2���任��ֻ��ʹ�����빲����ص��ӿ��еĶ�ȡ�ӿڶԣ��Ի�ȡ����֡���ݡ�
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ������ѳ�ʼ��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	virtual int RegInputShareBufferCB(IN INXVideoShareBufferCB* pShareCB) = 0;
	// ע��ȥ��֯������ص��ӿ�
	// ��1���ù���Ϊ���Ч�����裬ע��ɹ��󣬱任���ڲ��������������з����̡�
	// ��2���任����ʹ�ù�����ص��ӿ��еĶ�д�ӿڶԣ��Ի�ȡ�ɶ�д��֡����
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ������ѳ�ʼ��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	virtual int RegDeinterlaceShareBufferCB(IN INXVideoShareBufferCB* pTopShareCB,
											IN INXVideoShareBufferCB* pBottomShareCB) = 0;
	// ��ʼ���任�������������ϵ������
	// ����		NXFMTCONV_RET_SUCCEED		�ɹ�
	//			NXFMTCONV_RET_FAILED		ʧ�ܣ������ѳ�ʼ��
	//			NXFMTCONV_RET_ALREADY_INIT	�任���ѱ���ʼ��
	virtual int Initialize() = 0;
	// �����������ӳ��
	// ��1���������õ�֡�ʲ����������֡��Ӧ������ӳ��֡
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	virtual int CalcMapping(OUT LPNXSPEEDMAPDATA pMapData,		// ӳ������ṹ
							IN int nOutFrameIndex) = 0;			// ���֡�������
	// ������һ֡����
	// ��1���˽ӿ�ʹ��ע������뻺�干��ص��ӿ�
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������δ����ʼ��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	virtual int GetOutputFrame(OUT unsigned char* pucBuf,			// ���Ŀ��
							   IN int nPitch,						// һ���ź�ռ�õ��ֽڿ��
							   IN LPNXSPEEDMAPDATA pMapData) = 0;	// ӳ��
	// ������һ֡����
	// ����		NXFMTCONV_RET_SUCCEED			�ɹ�
	//			NXFMTCONV_RET_FAILED			ʧ�ܣ��ñ任������δ����ʼ��
	//			NXFMTCONV_RET_INVALID_PARAM		���������Ч
	virtual int GetOutputFrame(OUT unsigned char* pucBuf,			// ���Ŀ��
							   IN int nPitch,						// һ���ź�ռ�õ��ֽڿ��
							   IN unsigned char* pucPrevFrame[4],	// ӳ���Ӧ������֡��ǰһ֡
							   IN unsigned char* pucCurrFrame[4],	// ӳ���Ӧ������֡
							   IN int nInFramePitch,				// ����֡һ�ж�Ӧ���ֽ���
							   IN LPNXSPEEDMAPDATA pMapData) = 0;	// ӳ��
	// �ͷű任��
	// ��1���ͷű任���ڲ����壬������ϵ������
	// ����		NXFMTCONV_RET_SUCCEED		�ɹ�
	//			NXFMTCONV_RET_FAILED		ʧ��
	virtual int Release() = 0;
	// �任����״̬��ȡ
	virtual bool IsInitialized() = 0;
};


#endif		//__I_NX_VIDEO_FMT_CONV_FUNC_H__
//////////////////////////////////////////////////////////////////////////
// end-of-file
