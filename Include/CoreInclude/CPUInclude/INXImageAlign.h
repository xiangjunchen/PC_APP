//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: Image Alignment Process.
//
// Created: ZhaoZhimeng @ 2010-08-12
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "NXImageMathDef.h"

interface INXImageAlign
{
	// ��ȡ�㷨����
	virtual NXIMAGE_ALIGN_ALGORITHM GetAlgorithm() = 0;
	// ���õ�ǰ�زĵĴ���Χ���� nStart ֡��ʼ���� nCount ֡
	virtual bool SetFrameRange(NXMOTION_MODEL eModel,	// �˶�ģ��
							   int nStart,				// ��ʼ֡
							   int nCount) = 0;			// �ܳ�
	// �ж�ĳ֡�Ƿ�����Ч��Χ��
	virtual bool IsFrameValid(NXMOTION_MODEL eModel,	// �˶�ģ��
							  int nFrame) = 0;
	// �ж�ĳ֡�Ƿ񱻴����
	virtual bool IsFrameNeedProc(NXMOTION_MODEL eModel,		// �˶�ģ��
								 int nFrame) = 0;
	// ���ݻҶ�ͼ����ƥ�䴦��
	virtual bool ProcFrame(NXMOTION_MODEL eModel,		// �˶�ģ��
						   unsigned char *pucAnchor,
						   intptr_t nAnchorPitch,
						   unsigned char *pucTarget,
						   intptr_t nTargetPitch,
						   int nFrameWidth,				// ͼ�����
						   int nFrameHeight,
						   int nTargetFrame) = 0;		// Ŀ��֡��
	// �ж�ĳ֡�ǲ���ת������仯̫��
	virtual bool IsMatchDegrade(NXMOTION_MODEL eModel,	// �˶�ģ��
								int nFrame) = 0;
	// ��ȡĳ֡�ı任����
	// ��1�������洢��ʽΪ [a0, a1, a2, b0, b1, b2]
	// ��2���� Rigid & Affine ͳһ������ Rigid ���ԣ��� b1 = -a2, b2 = a1
	virtual const float* GetWarpData(NXMOTION_MODEL eModel,	// �˶�ģ��
									 int nFrame) = 0;
};


//////////////////////////////////////////////////////////////////////////
// End of file.