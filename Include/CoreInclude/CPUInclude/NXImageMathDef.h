//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 常用的图像数学计算接口定义，如梯度、散度、方程求解等.
//
// Created: ZhaoZhimeng @ 2010-05-17
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_IMAGE_MATH_DEF_H__
#define __NX_IMAGE_MATH_DEF_H__

#include "NXColorSpaceDef.h"

//////////////////////////////////////////////////////////////////////////
// 编译声明

#ifndef interface
#define interface	struct
#endif		// #ifndef interface

#ifndef IN
#define IN
#endif		// #ifndef IN
#ifndef OUT
#define OUT
#endif		// #ifndef OUT


//////////////////////////////////////////////////////////////////////////
// 库定义

#define NXIMAGEMATH_DLL_NAME			_T("NXImageMath")
#define NXIMAGEMATH_API_EXT_NAME		"NIM_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// 运动模型定义
enum NXMOTION_MODEL
{
	NXMOTION_MODEL_RIGID,		// 刚体变换
	NXMOTION_MODEL_AFFINE,		// 仿射变换

	NXMOTION_MODEL_COUNT,
	NXMOTION_MODEL_UNKNOWN = NXMOTION_MODEL_COUNT
};
// 模型参数维数定义
#define NXRIGID_COEF_DIM		4
#define NXAFFINE_COEF_DIM		6
// 模型参数索引定义
// 刚体变换 { x' = (1 + p1) * x + p2 * y + p3    y' = -p2 * x + (1 + p1) * y + p4 }
//			{ x' = a0 + a1 * x + a2 * y    y' = b0 - a2 * x + a1 * y }
// 仿射变换 { x' = (1 + p1) * x + p3 * y + p5    y' = p2 * x + (1 + p4) * y + p6 }
//			{ x' = a0 + a1 * x + a2 * y    y' = b0 + b1 * x + b2 * y }
enum NXMOTION_MODEL_COEF
{
	NXMOTION_MODEL_COEF_P1 = 0,
	NXMOTION_MODEL_COEF_P2,
	NXMOTION_MODEL_COEF_P3,
	NXMOTION_MODEL_COEF_P4,
	NXMOTION_MODEL_COEF_P5,
	NXMOTION_MODEL_COEF_P6,

	NXMOTION_MODEL_COEF_A0 = 0,
	NXMOTION_MODEL_COEF_A1,
	NXMOTION_MODEL_COEF_A2,
	NXMOTION_MODEL_COEF_B0,
	NXMOTION_MODEL_COEF_B1,
	NXMOTION_MODEL_COEF_B2,

	NXMOTION_MODEL_COEF_MAX
};


//////////////////////////////////////////////////////////////////////////
// 图像对准算法
enum NXIMAGE_ALIGN_ALGORITHM
{
	NXIMAGE_ALIGN_OPTICAL,	// 光流法
	NXIMAGE_ALIGN_FEATURE,	// 特征法

	NXIMAGE_ALIGN_COUNT,
	NXIMAGE_ALIGN_UNKNOWN = NXIMAGE_ALIGN_COUNT
};


//////////////////////////////////////////////////////////////////////////
// 梯度算子
enum NXGRADIENT_OPERATOR
{
	NXGRADIENT_OPERATOR_ROBERTS,	// Roberts 算子
	NXGRADIENT_OPERATOR_SOBEL,		// Sobel 算子

	NXGRADIENT_OPERATOR_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// 基本运算接口定义

// 计算从参考图仿射变换到目标图的误差，并统计误差总和与误差增益总和
// （1）为了统一刚体与仿射变换，需要将刚体变换的系数进行扩展
// 仿射变换的系数为 [a0, a1, a2, b0, b1, b2]，坐标映射公式如下
//		tx = a0 + a1 * x + a2 * y		ty = b0 + b1 * x + b2 * y
// 刚体变换的系数为 [a0, a1, a2, b0]，坐标映射公式如下
//		tx = a0 + a1 * x + a2 * y		ty = b0 - a2 * x + a1 * y
// 则将刚体变换的系数扩展为 [a0, a1, a2, b0, -a2, a1] 即可。
// （2）支持跳点采样，但输出是连续的，相当于进行缩放。
// （3）经过映射后，像素坐标无效的点直接取零，有效的点则进行双线性插值
// （4）统计值的第一个数为误差总和 Sum(Err)，Err = f(Target) - Anchor，
// 第二个数为误差增益总和 Sum(ErrGain)，ErrGain = Err * Anchor，
// 当参数 pdLAStat = NULL 时，表示调用者不需要这些统计。
typedef bool (*DefAffineErr)(OUT float *pfError,				// 输出（至少 nProcWidth * nProcHeight 大小）
							 OUT double *pdLAStat,				// 统计值 [ErrSum, ErrGainSum]
							 IN unsigned char *pucAnchor,		// 8 位参考图
							 IN intptr_t nAnchorPitch,			// 参考图一行所占用的字节数
							 IN unsigned char *pucTarget,		// 8 位目标图
							 IN intptr_t nTargetPitch,			// 目标图一行所占用的字节数
							 IN int nStartX,					// 水平方向起点
							 IN int nStrideX,					// 水平方向跳步
							 IN int nStartY,					// 垂直方向起点
							 IN int nStrideY,					// 垂直方向跳步
							 IN int nProcWidth,					// 水平计算点的个数
							 IN int nProcHeight,				// 垂直计算点的个数
							 IN int nTargetWidth,				// 目标图幅面大小
							 IN int nTargetHeight,
							 IN float *pfWarpCoef);				// 变换系数
// 图像对准算法接口
interface INXImageAlign;
typedef INXImageAlign* (*DefCreateImageAlign)(IN NXIMAGE_ALIGN_ALGORITHM eAlgorithm);
typedef void (*DefReleaseImageAlign)(INXImageAlign* &pObj);

typedef struct tag_nximagegenericapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	DefAffineErr		pfnAffineErr;
	DefCreateImageAlign		pfnCreateImageAlign;
	DefReleaseImageAlign	pfnReleaseImageAlign;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXImageGenericAPI, *LPNXIMAGEGENERICAPI;

//////////////////////////////////////////////////////////////////////////
// 与边缘梯度计算相关的接口定义

// 带掩码的 Roberts 算子散度求解
// （1）散度结果按像素展开，则 pfDIV 的每个元素大小为 nPixelBytes。
// （2）不支持 NXVIDEO_PIXEL_FMT_YUV422 与 NXVIDEO_PIXEL_FMT_YUV420 两种像素格式。
// （3）当 pucSrc == NULL 时，将 pfDIV 清空后立即返回。
// （4）当 pucMask == NULL 时，输出源中所有像素的散度；当不为 NULL 时，只输出大于阈值的区域。
// （5）最小幅面要求为 4x3，低于此幅面不处理。
typedef bool (*DefRobertsDIV)(OUT float *pfDIV,					// 散度结果（按通道分页存储）
							  IN unsigned char *pucSrc,			// 源有效起点
							  IN intptr_t nSrcPitch,			// 源一行所占用的字节数
							  IN unsigned char *pucMask,		// 像素掩码有效起点
							  IN intptr_t nMaskStrideX,			// 一个元素的掩码所占用的字节数
							  IN intptr_t nMaskPitch,			// 掩码一行所占用的字节数
							  IN unsigned int unMaskThr,		// 掩码阈值 [0, 255]（大于此阈值有效）
							  IN NXVIDEO_PIXEL_FMT ePixelFmt,	// 像素格式
							  IN int nWidth,					// 处理幅面
							  IN int nHeight);
// 8 位灰度图的 Gradient Descent 计算，并统计 Linear Appearance Variation = sum(src^2)
// （1）支持多种梯度算子、支持多种运动模型。
// （2）支持跳点采样，但输出是连续的，相当于进行邻近点的缩放。
// （3）对于图像的边界，即算子不能处理的区域，梯度一律清零。而一般应用中，边界的用途也不大，多不处理。
// （4）参数 pdLAV = NULL 时，表示调用者不需要该统计。
typedef bool (*DefDescent)(OUT float *pfDescent,				// 输出（至少 nProcWidth * nProcHeight 大小）
						   OUT double *pdLAV,					// 统计值
						   IN unsigned char *pucSrc,			// 8 位灰度图源
						   IN intptr_t nSrcPitch,				// 源一行所占用的字节数
						   IN int nStartX,						// 水平方向起点
						   IN int nStrideX,						// 水平方向跳步
						   IN int nStartY,						// 垂直方向起点
						   IN int nStrideY,						// 垂直方向跳步
						   IN int nProcWidth,					// 水平计算点的个数
						   IN int nProcHeight,					// 垂直计算点的个数
						   IN int nSrcWidth,					// 源图幅面大小
						   IN int nSrcHeight,
						   IN NXGRADIENT_OPERATOR eGradient,	// 梯度算子
						   IN NXMOTION_MODEL eModel);			// 运动模型
// Gradient Descent 的 Hessian 矩阵求和，mat = []' * []
// （1）支持多种维数的向量，要求梯度下降向量按像素连续存储。
// （2）Hessian 矩阵正定对称。
typedef bool (*DefDescentHessian)(OUT double *pdMatHessian,	// Hessian 矩阵
								  IN float *pfDescent,		// 梯度下降向量
								  IN int nDim,				// 向量维数
								  IN int nProcCount);		// 向量个数
// 最速下降误差统计，归一化误差，去除 Linear Appearance Variation 干扰
// （1）支持多种运动模型。
// （2）对源图支持跳点采样，但 pfDescent、pfError 两者的内容连续存储。
// （3）参数 pdLAV = NULL 时，表示调用者不需要进行归一化。
// （4）pdLAV 的第一个值为 LABias，第二个值为 LAGain。
typedef bool (*DefStatDescentErr)(OUT double *pdErrSum,			// 误差和
								  IN double *pdLAV,				// LAV 归一化参数 [LABias, LAGain]
								  IN float *pfDescent,			// 梯度下降图
								  IN float *pfError,			// 映射误差图
								  IN unsigned char *pucSrc,		// 8 位灰度图源
								  IN intptr_t nSrcPitch,		// 源一行所占用的字节数
								  IN int nStartX,				// 水平方向起点
								  IN int nStrideX,				// 水平方向跳步
								  IN int nStartY,				// 垂直方向起点
								  IN int nStrideY,				// 垂直方向跳步
								  IN int nProcWidth,			// 水平计算点的个数
								  IN int nProcHeight,			// 垂直计算点的个数
								  IN NXMOTION_MODEL eModel);	// 运动模型


typedef struct tag_nximageedgeapi	// 图像边缘处理
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// Roberts 算子散度
	DefRobertsDIV		pfnRobertsDIV;
	// Gradient Descent
	DefDescent			pfnDescent;
	// Gradient Descent Hessian
	DefDescentHessian	pfnDescentHessian;
	// 最速下降误差统计
	DefStatDescentErr	pfnStatDescentErr;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXImageEdgeAPI, *LPNXIMAGEEDGEAPI;


//////////////////////////////////////////////////////////////////////////
// 图像偏微分方程的相关接口定义

// Gauss-seidel + SOR + Roberts 离散求解 Laplace 方程，近似方程为 A * x = b
// （1）fEpsilon 为归一化迭代中止阈值，过小会造成效率偏低，一般半个像素精度足够，即 0.5 / 255。
// （2）fRelaxFactor 为超松驰因子，理论取值范围为 (0, 2)，取值越小，效率越低，结果越平滑。
// （3）pfRes 与 pfDIV 按像素展开，每个元素的大小为 nPixelBytes。
// （4）不支持 NXVIDEO_PIXEL_FMT_YUV422 与 NXVIDEO_PIXEL_FMT_YUV420 两种像素格式。
// （5）当 pucMask == NULL 时，处理所有的像素；当不为 NULL 时，只处理大于阈值的区域。
// （6）最小幅面要求为 4x3，低于此幅面不处理。
typedef bool (*DefRobertsGSSOR)(IN OUT float *pfRes,			// 迭代对象 x
								IN float *pfDIV,				// 散度 b
								IN unsigned char *pucMask,		// 掩码
								IN intptr_t nMaskStrideX,		// 一个元素的掩码所占用的字节数
								IN intptr_t nMaskPitch,			// 掩码一行所占用的字节数
								IN unsigned int unMaskThr,		// 掩码阈值 [0, 255]（大于此阈值有效）
								IN float fEpsilon,				// 迭代中止阈值
								IN int nMaxIterate,				// 最大迭代次数（防止陷入死循环）
								IN float fRelaxFactor,			// 超松驰因子
								IN NXVIDEO_PIXEL_FMT ePixelFmt,	// 像素格式
								IN int nWidth,					// 处理幅面
								IN int nHeight);

typedef struct tag_nximagepdeapi	// 图像偏微分方程
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// Laplace 方程求解
	DefRobertsGSSOR		pfnRobertsGSSOR;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXImagePDEAPI, *LPNXIMAGEPDEAPI;


//////////////////////////////////////////////////////////////////////////
// 接口结构定义

typedef struct tag_nximageinnerapi *LPNXIMAGEINNERAPI;

typedef struct tag_nximagemathapi
{
	// 基本计算接口
	LPNXIMAGEGENERICAPI		pStuImageGenericAPI;
	// 图像边缘处理接口
	LPNXIMAGEEDGEAPI		pStuImageEdgeAPI;
	// 图像偏微分方程求解接口
	LPNXIMAGEPDEAPI			pStuImagePDEAPI;
	// Private 权限，用于优化库内部的接口
	LPNXIMAGEINNERAPI		pStuInnerAPI;
} NXImageMathAPI, *LPNXIMAGEMATHAPI;


#endif		// #ifndef __NX_IMAGE_MATH_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.