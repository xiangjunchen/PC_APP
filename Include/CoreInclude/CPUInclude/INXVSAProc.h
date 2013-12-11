//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: Video Stabilization 算法处理器接口.
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
	// 设置当前素材的处理范围，从 nStart 帧开始处理 nCount 帧
	virtual bool SetFrameRange(NXIMAGE_ALIGN_ALGORITHM eAlgorithm,	// 算法类型
							   NXMOTION_MODEL eModel,				// 运动模型
							   int nStart,							// 起始帧
							   int nCount) = 0;						// 总长
	// 判断某帧是否被处理过
	virtual bool IsFrameNeedProc(NXIMAGE_ALIGN_ALGORITHM eAlgorithm,	// 算法类型
								 NXMOTION_MODEL eModel,					// 运动模型
								 int nFrame) = 0;
	// 设置灰度图匹配处理参数，如果参数发生变化，之前的匹配数据将被清空
	virtual bool SetProcParam(int nProcSpeed,				// 处理速度
							  float fProcPrecision) = 0;	// 处理精度
	// 根据灰度图进行匹配处理
	virtual bool ProcFrame(NXIMAGE_ALIGN_ALGORITHM eAlgorithm,	// 算法类型
						   NXMOTION_MODEL eModel,				// 运动模型
						   unsigned char *pucAnchor,
						   intptr_t nAnchorPitch,
						   unsigned char *pucTarget,
						   intptr_t nTargetPitch,
						   int nFrameWidth,						// 图像幅面
						   int nFrameHeight,
						   int nTargetFrame) = 0;				// 目标帧号
	// 根据已获得的匹配数据进行滤波，返回滤波的实际帧数，<= 0 表示失败
	// 模型与观测噪声方差均为像素坐标系下的数值，必须都大于等于 0
	virtual int Filter(NXIMAGE_ALIGN_ALGORITHM eAlgorithm,	// 算法类型
					   NXMOTION_MODEL eModel,				// 运动模型
					   int nStart,							// 起始帧号
					   int nCount,							// 总长
					   int nFrameWidth,						// 图像幅面
					   int nFrameHeight,
					   float fModelTransVar,				// 模型方差
					   float fModelScaleVar,
					   float fModelRotVar,
					   float fObsTransVar,					// 量测方差
					   float fObsScaleVar,
					   float fObsRotVar) = 0;
	// 获取某帧的归一化补偿参数，如果该帧未被处理则返回 NULL
	// （1）为了统一刚体变换与仿射变换的数据结构，输出参数存储方式 [a0 a1 a2 b0 b1 b2]
	// 对于刚体变换有：b1 = -a2, b2 = a1
	// （2）该参数为最近一次调用 Filter 所产生的结果
	// （3）pfFullAdapter 输出值表示经过补偿后，完全消除黑边（即全屏填充）所需要的放大倍数，
	// 即保证任意像素点经过补偿后的坐标，通过适配可以缩放到有效区，公式如下：
	// x' = a0 + a1 * x + a2 * y    y' = b0 + b1 * x + b2 * y
	// x" = (x' - 0.5) * s + 0.5    y" = (y' - 0.5) * s + 0.5
	// max_x = max(abs(a0-0.5), abs(a0+a1-0.5), abs(a0+a2-0.5), abs(a0+a1+a2-0.5))
	// max_y = max(abs(b0-0.5), abs(b0+b1-0.5), abs(b0+b2-0.5), abs(b0+b1+b2-0.5))
	// s = 0.5 / max(max_x, max_y)
	virtual const float* GetNormComp(float *pfFullAdapter,	// 全屏填充的缩放系数
									 int nFrame) = 0;
	// 获取统一的全屏填充放大倍数
	// （1）该参数为最近一次调用 Filter 所产生的结果
	// （2）s_u = min(s(i))，即最小的缩放倍数
	virtual bool GetUnifyAdapter(float *pfFullAdapter) = 0;
	//////////////////////////////////////////////////////////////////////////
	// 获取配准后的输出帧，要求调用者先调用 Filter 接口
	virtual NXRESULT GetAlignedFrame(__out unsigned char* des_frame,		// 经过配准处理后的输出帧
									 __in intptr_t des_frame_pitch,			// 输出帧一行的字节数
									 __in unsigned char* src_frame,			// 源帧
									 __in intptr_t src_frame_pitch,			// 源帧一行的字节数
									 __in NXVIDEO_PIXEL_FMT pixel_fmt,		// 像素格式，只支持 32 位
									 __in int width,						// 图像幅面
									 __in int height,
									 __in int frame,						// 帧号
									 __in NXVIDEO_FILTER_TYPE filter) = 0;	// 支持 NXVIDEO_FILTER_POINT、NXVIDEO_FILTER_LINEAR
};

#endif	// __INXVSAPROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.