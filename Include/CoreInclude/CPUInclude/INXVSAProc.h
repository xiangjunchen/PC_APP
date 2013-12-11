//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: Video Stabilization �㷨�������ӿ�.
//
// Created: ZhaoZhimeng @ 2010-08-10
//////////////////////////////////////////////////////////////////////////
#ifndef __INXVSAPROC_H__
#define __INXVSAPROC_H__

#include ".\NXBasic.h"
#include ".\NXImageMathDef.h"
#include ".\NXVideoProcDef.h"

interface NOVTABLE INXVSAProc
{
	// ���õ�ǰ�زĵĴ���Χ���� nStart ֡��ʼ���� nCount ֡
	virtual bool SetFrameRange(NXIMAGE_ALIGN_ALGORITHM eAlgorithm,	// �㷨����
							   NXMOTION_MODEL eModel,				// �˶�ģ��
							   int nStart,							// ��ʼ֡
							   int nCount) = 0;						// �ܳ�
	// �ж�ĳ֡�Ƿ񱻴����
	virtual bool IsFrameNeedProc(NXIMAGE_ALIGN_ALGORITHM eAlgorithm,	// �㷨����
								 NXMOTION_MODEL eModel,					// �˶�ģ��
								 int nFrame) = 0;
	// ���ûҶ�ͼƥ�䴦�������������������仯��֮ǰ��ƥ�����ݽ������
	virtual bool SetProcParam(int nProcSpeed,				// �����ٶ�
							  float fProcPrecision) = 0;	// ������
	// ���ݻҶ�ͼ����ƥ�䴦��
	virtual bool ProcFrame(NXIMAGE_ALIGN_ALGORITHM eAlgorithm,	// �㷨����
						   NXMOTION_MODEL eModel,				// �˶�ģ��
						   unsigned char *pucAnchor,
						   intptr_t nAnchorPitch,
						   unsigned char *pucTarget,
						   intptr_t nTargetPitch,
						   int nFrameWidth,						// ͼ�����
						   int nFrameHeight,
						   int nTargetFrame) = 0;				// Ŀ��֡��
	// �����ѻ�õ�ƥ�����ݽ����˲��������˲���ʵ��֡����<= 0 ��ʾʧ��
	// ģ����۲����������Ϊ��������ϵ�µ���ֵ�����붼���ڵ��� 0
	virtual int Filter(NXIMAGE_ALIGN_ALGORITHM eAlgorithm,	// �㷨����
					   NXMOTION_MODEL eModel,				// �˶�ģ��
					   int nStart,							// ��ʼ֡��
					   int nCount,							// �ܳ�
					   int nFrameWidth,						// ͼ�����
					   int nFrameHeight,
					   float fModelTransVar,				// ģ�ͷ���
					   float fModelScaleVar,
					   float fModelRotVar,
					   float fObsTransVar,					// ���ⷽ��
					   float fObsScaleVar,
					   float fObsRotVar) = 0;
	// ��ȡĳ֡�Ĺ�һ�����������������֡δ�������򷵻� NULL
	// ��1��Ϊ��ͳһ����任�����任�����ݽṹ����������洢��ʽ [a0 a1 a2 b0 b1 b2]
	// ���ڸ���任�У�b1 = -a2, b2 = a1
	// ��2���ò���Ϊ���һ�ε��� Filter �������Ľ��
	// ��3��pfFullAdapter ���ֵ��ʾ������������ȫ�����ڱߣ���ȫ����䣩����Ҫ�ķŴ�����
	// ����֤�������ص㾭������������꣬ͨ������������ŵ���Ч������ʽ���£�
	// x' = a0 + a1 * x + a2 * y    y' = b0 + b1 * x + b2 * y
	// x" = (x' - 0.5) * s + 0.5    y" = (y' - 0.5) * s + 0.5
	// max_x = max(abs(a0-0.5), abs(a0+a1-0.5), abs(a0+a2-0.5), abs(a0+a1+a2-0.5))
	// max_y = max(abs(b0-0.5), abs(b0+b1-0.5), abs(b0+b2-0.5), abs(b0+b1+b2-0.5))
	// s = 0.5 / max(max_x, max_y)
	virtual const float* GetNormComp(float *pfFullAdapter,	// ȫ����������ϵ��
									 int nFrame) = 0;
	// ��ȡͳһ��ȫ�����Ŵ���
	// ��1���ò���Ϊ���һ�ε��� Filter �������Ľ��
	// ��2��s_u = min(s(i))������С�����ű���
	virtual bool GetUnifyAdapter(float *pfFullAdapter) = 0;
	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��׼������֡��Ҫ��������ȵ��� Filter �ӿ�
	virtual NXRESULT GetAlignedFrame(__out unsigned char* des_frame,		// ������׼���������֡
									 __in intptr_t des_frame_pitch,			// ���֡һ�е��ֽ���
									 __in unsigned char* src_frame,			// Դ֡
									 __in intptr_t src_frame_pitch,			// Դ֡һ�е��ֽ���
									 __in NXVIDEO_PIXEL_FMT pixel_fmt,		// ���ظ�ʽ��ֻ֧�� 32 λ
									 __in int width,						// ͼ�����
									 __in int height,
									 __in int frame,						// ֡��
									 __in NXVIDEO_FILTER_TYPE filter) = 0;	// ֧�� NXVIDEO_FILTER_POINT��NXVIDEO_FILTER_LINEAR
};

#endif	// __INXVSAPROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.