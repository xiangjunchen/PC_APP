//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ���õ�ͼ����ѧ����ӿڶ��壬���ݶȡ�ɢ�ȡ���������.
//
// Created: ZhaoZhimeng @ 2010-05-17
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_IMAGE_MATH_DEF_H__
#define __NX_IMAGE_MATH_DEF_H__

#include "NXColorSpaceDef.h"

//////////////////////////////////////////////////////////////////////////
// ��������

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
// �ⶨ��

#define NXIMAGEMATH_DLL_NAME			_T("NXImageMath")
#define NXIMAGEMATH_API_EXT_NAME		"NIM_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// �˶�ģ�Ͷ���
enum NXMOTION_MODEL
{
	NXMOTION_MODEL_RIGID,		// ����任
	NXMOTION_MODEL_AFFINE,		// ����任

	NXMOTION_MODEL_COUNT,
	NXMOTION_MODEL_UNKNOWN = NXMOTION_MODEL_COUNT
};
// ģ�Ͳ���ά������
#define NXRIGID_COEF_DIM		4
#define NXAFFINE_COEF_DIM		6
// ģ�Ͳ�����������
// ����任 { x' = (1 + p1) * x + p2 * y + p3    y' = -p2 * x + (1 + p1) * y + p4 }
//			{ x' = a0 + a1 * x + a2 * y    y' = b0 - a2 * x + a1 * y }
// ����任 { x' = (1 + p1) * x + p3 * y + p5    y' = p2 * x + (1 + p4) * y + p6 }
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
// ͼ���׼�㷨
enum NXIMAGE_ALIGN_ALGORITHM
{
	NXIMAGE_ALIGN_OPTICAL,	// ������
	NXIMAGE_ALIGN_FEATURE,	// ������

	NXIMAGE_ALIGN_COUNT,
	NXIMAGE_ALIGN_UNKNOWN = NXIMAGE_ALIGN_COUNT
};


//////////////////////////////////////////////////////////////////////////
// �ݶ�����
enum NXGRADIENT_OPERATOR
{
	NXGRADIENT_OPERATOR_ROBERTS,	// Roberts ����
	NXGRADIENT_OPERATOR_SOBEL,		// Sobel ����

	NXGRADIENT_OPERATOR_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// ��������ӿڶ���

// ����Ӳο�ͼ����任��Ŀ��ͼ������ͳ������ܺ�����������ܺ�
// ��1��Ϊ��ͳһ���������任����Ҫ������任��ϵ��������չ
// ����任��ϵ��Ϊ [a0, a1, a2, b0, b1, b2]������ӳ�乫ʽ����
//		tx = a0 + a1 * x + a2 * y		ty = b0 + b1 * x + b2 * y
// ����任��ϵ��Ϊ [a0, a1, a2, b0]������ӳ�乫ʽ����
//		tx = a0 + a1 * x + a2 * y		ty = b0 - a2 * x + a1 * y
// �򽫸���任��ϵ����չΪ [a0, a1, a2, b0, -a2, a1] ���ɡ�
// ��2��֧�����������������������ģ��൱�ڽ������š�
// ��3������ӳ�������������Ч�ĵ�ֱ��ȡ�㣬��Ч�ĵ������˫���Բ�ֵ
// ��4��ͳ��ֵ�ĵ�һ����Ϊ����ܺ� Sum(Err)��Err = f(Target) - Anchor��
// �ڶ�����Ϊ��������ܺ� Sum(ErrGain)��ErrGain = Err * Anchor��
// ������ pdLAStat = NULL ʱ����ʾ�����߲���Ҫ��Щͳ�ơ�
typedef bool (*DefAffineErr)(OUT float *pfError,				// ��������� nProcWidth * nProcHeight ��С��
							 OUT double *pdLAStat,				// ͳ��ֵ [ErrSum, ErrGainSum]
							 IN unsigned char *pucAnchor,		// 8 λ�ο�ͼ
							 IN intptr_t nAnchorPitch,			// �ο�ͼһ����ռ�õ��ֽ���
							 IN unsigned char *pucTarget,		// 8 λĿ��ͼ
							 IN intptr_t nTargetPitch,			// Ŀ��ͼһ����ռ�õ��ֽ���
							 IN int nStartX,					// ˮƽ�������
							 IN int nStrideX,					// ˮƽ��������
							 IN int nStartY,					// ��ֱ�������
							 IN int nStrideY,					// ��ֱ��������
							 IN int nProcWidth,					// ˮƽ�����ĸ���
							 IN int nProcHeight,				// ��ֱ�����ĸ���
							 IN int nTargetWidth,				// Ŀ��ͼ�����С
							 IN int nTargetHeight,
							 IN float *pfWarpCoef);				// �任ϵ��
// ͼ���׼�㷨�ӿ�
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
	// �°汾�ӿڴ����ʼ����
} NXImageGenericAPI, *LPNXIMAGEGENERICAPI;

//////////////////////////////////////////////////////////////////////////
// ���Ե�ݶȼ�����صĽӿڶ���

// ������� Roberts ����ɢ�����
// ��1��ɢ�Ƚ��������չ������ pfDIV ��ÿ��Ԫ�ش�СΪ nPixelBytes��
// ��2����֧�� NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 �������ظ�ʽ��
// ��3���� pucSrc == NULL ʱ���� pfDIV ��պ��������ء�
// ��4���� pucMask == NULL ʱ�����Դ���������ص�ɢ�ȣ�����Ϊ NULL ʱ��ֻ���������ֵ������
// ��5����С����Ҫ��Ϊ 4x3�����ڴ˷��治����
typedef bool (*DefRobertsDIV)(OUT float *pfDIV,					// ɢ�Ƚ������ͨ����ҳ�洢��
							  IN unsigned char *pucSrc,			// Դ��Ч���
							  IN intptr_t nSrcPitch,			// Դһ����ռ�õ��ֽ���
							  IN unsigned char *pucMask,		// ����������Ч���
							  IN intptr_t nMaskStrideX,			// һ��Ԫ�ص�������ռ�õ��ֽ���
							  IN intptr_t nMaskPitch,			// ����һ����ռ�õ��ֽ���
							  IN unsigned int unMaskThr,		// ������ֵ [0, 255]�����ڴ���ֵ��Ч��
							  IN NXVIDEO_PIXEL_FMT ePixelFmt,	// ���ظ�ʽ
							  IN int nWidth,					// �������
							  IN int nHeight);
// 8 λ�Ҷ�ͼ�� Gradient Descent ���㣬��ͳ�� Linear Appearance Variation = sum(src^2)
// ��1��֧�ֶ����ݶ����ӡ�֧�ֶ����˶�ģ�͡�
// ��2��֧�����������������������ģ��൱�ڽ����ڽ�������š�
// ��3������ͼ��ı߽磬�����Ӳ��ܴ���������ݶ�һ�����㡣��һ��Ӧ���У��߽����;Ҳ���󣬶಻����
// ��4������ pdLAV = NULL ʱ����ʾ�����߲���Ҫ��ͳ�ơ�
typedef bool (*DefDescent)(OUT float *pfDescent,				// ��������� nProcWidth * nProcHeight ��С��
						   OUT double *pdLAV,					// ͳ��ֵ
						   IN unsigned char *pucSrc,			// 8 λ�Ҷ�ͼԴ
						   IN intptr_t nSrcPitch,				// Դһ����ռ�õ��ֽ���
						   IN int nStartX,						// ˮƽ�������
						   IN int nStrideX,						// ˮƽ��������
						   IN int nStartY,						// ��ֱ�������
						   IN int nStrideY,						// ��ֱ��������
						   IN int nProcWidth,					// ˮƽ�����ĸ���
						   IN int nProcHeight,					// ��ֱ�����ĸ���
						   IN int nSrcWidth,					// Դͼ�����С
						   IN int nSrcHeight,
						   IN NXGRADIENT_OPERATOR eGradient,	// �ݶ�����
						   IN NXMOTION_MODEL eModel);			// �˶�ģ��
// Gradient Descent �� Hessian ������ͣ�mat = []' * []
// ��1��֧�ֶ���ά����������Ҫ���ݶ��½����������������洢��
// ��2��Hessian ���������Գơ�
typedef bool (*DefDescentHessian)(OUT double *pdMatHessian,	// Hessian ����
								  IN float *pfDescent,		// �ݶ��½�����
								  IN int nDim,				// ����ά��
								  IN int nProcCount);		// ��������
// �����½����ͳ�ƣ���һ����ȥ�� Linear Appearance Variation ����
// ��1��֧�ֶ����˶�ģ�͡�
// ��2����Դͼ֧������������� pfDescent��pfError ���ߵ����������洢��
// ��3������ pdLAV = NULL ʱ����ʾ�����߲���Ҫ���й�һ����
// ��4��pdLAV �ĵ�һ��ֵΪ LABias���ڶ���ֵΪ LAGain��
typedef bool (*DefStatDescentErr)(OUT double *pdErrSum,			// ����
								  IN double *pdLAV,				// LAV ��һ������ [LABias, LAGain]
								  IN float *pfDescent,			// �ݶ��½�ͼ
								  IN float *pfError,			// ӳ�����ͼ
								  IN unsigned char *pucSrc,		// 8 λ�Ҷ�ͼԴ
								  IN intptr_t nSrcPitch,		// Դһ����ռ�õ��ֽ���
								  IN int nStartX,				// ˮƽ�������
								  IN int nStrideX,				// ˮƽ��������
								  IN int nStartY,				// ��ֱ�������
								  IN int nStrideY,				// ��ֱ��������
								  IN int nProcWidth,			// ˮƽ�����ĸ���
								  IN int nProcHeight,			// ��ֱ�����ĸ���
								  IN NXMOTION_MODEL eModel);	// �˶�ģ��


typedef struct tag_nximageedgeapi	// ͼ���Ե����
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// Roberts ����ɢ��
	DefRobertsDIV		pfnRobertsDIV;
	// Gradient Descent
	DefDescent			pfnDescent;
	// Gradient Descent Hessian
	DefDescentHessian	pfnDescentHessian;
	// �����½����ͳ��
	DefStatDescentErr	pfnStatDescentErr;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXImageEdgeAPI, *LPNXIMAGEEDGEAPI;


//////////////////////////////////////////////////////////////////////////
// ͼ��ƫ΢�ַ��̵���ؽӿڶ���

// Gauss-seidel + SOR + Roberts ��ɢ��� Laplace ���̣����Ʒ���Ϊ A * x = b
// ��1��fEpsilon Ϊ��һ��������ֹ��ֵ����С�����Ч��ƫ�ͣ�һ�������ؾ����㹻���� 0.5 / 255��
// ��2��fRelaxFactor Ϊ���ɳ����ӣ�����ȡֵ��ΧΪ (0, 2)��ȡֵԽС��Ч��Խ�ͣ����Խƽ����
// ��3��pfRes �� pfDIV ������չ����ÿ��Ԫ�صĴ�СΪ nPixelBytes��
// ��4����֧�� NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 �������ظ�ʽ��
// ��5���� pucMask == NULL ʱ���������е����أ�����Ϊ NULL ʱ��ֻ���������ֵ������
// ��6����С����Ҫ��Ϊ 4x3�����ڴ˷��治����
typedef bool (*DefRobertsGSSOR)(IN OUT float *pfRes,			// �������� x
								IN float *pfDIV,				// ɢ�� b
								IN unsigned char *pucMask,		// ����
								IN intptr_t nMaskStrideX,		// һ��Ԫ�ص�������ռ�õ��ֽ���
								IN intptr_t nMaskPitch,			// ����һ����ռ�õ��ֽ���
								IN unsigned int unMaskThr,		// ������ֵ [0, 255]�����ڴ���ֵ��Ч��
								IN float fEpsilon,				// ������ֹ��ֵ
								IN int nMaxIterate,				// ��������������ֹ������ѭ����
								IN float fRelaxFactor,			// ���ɳ�����
								IN NXVIDEO_PIXEL_FMT ePixelFmt,	// ���ظ�ʽ
								IN int nWidth,					// �������
								IN int nHeight);

typedef struct tag_nximagepdeapi	// ͼ��ƫ΢�ַ���
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// Laplace �������
	DefRobertsGSSOR		pfnRobertsGSSOR;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXImagePDEAPI, *LPNXIMAGEPDEAPI;


//////////////////////////////////////////////////////////////////////////
// �ӿڽṹ����

typedef struct tag_nximageinnerapi *LPNXIMAGEINNERAPI;

typedef struct tag_nximagemathapi
{
	// ��������ӿ�
	LPNXIMAGEGENERICAPI		pStuImageGenericAPI;
	// ͼ���Ե����ӿ�
	LPNXIMAGEEDGEAPI		pStuImageEdgeAPI;
	// ͼ��ƫ΢�ַ������ӿ�
	LPNXIMAGEPDEAPI			pStuImagePDEAPI;
	// Private Ȩ�ޣ������Ż����ڲ��Ľӿ�
	LPNXIMAGEINNERAPI		pStuInnerAPI;
} NXImageMathAPI, *LPNXIMAGEMATHAPI;


#endif		// #ifndef __NX_IMAGE_MATH_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.