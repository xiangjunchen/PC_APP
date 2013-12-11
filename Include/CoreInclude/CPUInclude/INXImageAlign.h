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
	// 获取算法类型
	virtual NXIMAGE_ALIGN_ALGORITHM GetAlgorithm() = 0;
	// 设置当前素材的处理范围，从 nStart 帧开始处理 nCount 帧
	virtual bool SetFrameRange(NXMOTION_MODEL eModel,	// 运动模型
							   int nStart,				// 起始帧
							   int nCount) = 0;			// 总长
	// 判断某帧是否在有效范围内
	virtual bool IsFrameValid(NXMOTION_MODEL eModel,	// 运动模型
							  int nFrame) = 0;
	// 判断某帧是否被处理过
	virtual bool IsFrameNeedProc(NXMOTION_MODEL eModel,		// 运动模型
								 int nFrame) = 0;
	// 根据灰度图进行匹配处理
	virtual bool ProcFrame(NXMOTION_MODEL eModel,		// 运动模型
						   unsigned char *pucAnchor,
						   intptr_t nAnchorPitch,
						   unsigned char *pucTarget,
						   intptr_t nTargetPitch,
						   int nFrameWidth,				// 图像幅面
						   int nFrameHeight,
						   int nTargetFrame) = 0;		// 目标帧号
	// 判断某帧是不是转场或画面变化太大
	virtual bool IsMatchDegrade(NXMOTION_MODEL eModel,	// 运动模型
								int nFrame) = 0;
	// 获取某帧的变换参数
	// （1）参数存储格式为 [a0, a1, a2, b0, b1, b2]
	// （2）将 Rigid & Affine 统一，对于 Rigid 而言，令 b1 = -a2, b2 = a1
	virtual const float* GetWarpData(NXMOTION_MODEL eModel,	// 运动模型
									 int nFrame) = 0;
};


//////////////////////////////////////////////////////////////////////////
// End of file.