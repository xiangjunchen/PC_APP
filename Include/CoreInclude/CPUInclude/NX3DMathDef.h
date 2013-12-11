//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 3D ��ѧ���㣬��������������Ԫ����.
//
// Created: ZhaoZhimeng @ 2010-07-27
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_3D_MATH_DEF_H__
#define __NX_3D_MATH_DEF_H__

#include ".\NXBasic.h"

//////////////////////////////////////////////////////////////////////////
// ��������

#ifndef IN
#define IN
#endif		// #ifndef IN
#ifndef OUT
#define OUT
#endif		// #ifndef OUT


//////////////////////////////////////////////////////////////////////////
// �ⶨ��

#define NX3DMATH_DLL_NAME			_T("NX3DMath")
#define NX3DMATH_API_EXT_NAME		"N3D_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// ��������ӿڶ���

// ���
typedef float (*DefVecDotF)(IN const float *pfVec1, IN const float *pfVec2, IN int nDim);
typedef double (*DefVecDotD)(IN const double *pdVec1, IN const double *pdVec2, IN int nDim);
// ��ģ
typedef float (*DefVecLengthF)(IN const float *pfVec, IN int nDim);
typedef double (*DefVecLengthD)(IN const double *pdVec, IN int nDim);
// ��ģ��ƽ��
typedef DefVecLengthF pfn3d_length2_f;
typedef DefVecLengthD pfn3d_length2_d;
// �����
typedef DefVecDotF	DefVecDistanceF;
typedef DefVecDotD	DefVecDistanceD;
// ����
typedef void (*DefVecInverseF)(OUT float *pfVecRes, IN const float *pfVecSrc, IN int nDim);
typedef void (*DefVecInverseD)(OUT double *pdVecRes, IN const double *pdVecSrc, IN int nDim);
// ��һ��
typedef DefVecInverseF	DefVecNormalizeF;
typedef DefVecInverseD	DefVecNormalizeD;
// ����
typedef void (*DefVecMulF)(OUT float *pfVecRes, IN const float *pfVec1, IN const float *pfVec2, IN int nDim);
typedef void (*DefVecMulD)(OUT double *pdVecRes, IN const double *pdVec1, IN const double *pdVec2, IN int nDim);
// ���
typedef float* (*pfn3d_crossproduct_f)(__out float* des, __in float const* vec1, __in float const* vec2, __in int dim);
typedef double* (*pfn3d_crossproduct_d)(__out double* des, __in double const* vec1, __in double const* vec2, __in int dim);
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nx3dvectorapi
{
	DefVecDotF			pfnVecDotF;
	DefVecDotD			pfnVecDotD;
	DefVecLengthF		pfnVecModuleF;
	DefVecLengthD		pfnVecModuleD;
	DefVecInverseF		pfnVecInverseF;
	DefVecMulF			pfnVecMulF;
	DefVecMulD			pfnVecMulD;
	// V1.0.9.2
	DefVecDistanceF		pfnVecDistanceF;
	DefVecDistanceD		pfnVecDistanceD;
	DefVecInverseD		pfnVecInverseD;
	DefVecNormalizeF	pfnVecNormalizeF;
	DefVecNormalizeD	pfnVecNormalizeD;
	pfn3d_crossproduct_f	crossproduct_f;
	pfn3d_crossproduct_d	crossproduct_d;
	pfn3d_length2_f			length2_f;
	pfn3d_length2_d			length2_d;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����￪ʼ����
} NX3DVectorAPI, *LPNX3DVECTORAPI;


//////////////////////////////////////////////////////////////////////////
// ��������ӿ�

// ��˹��Ԫ��ⷽ�����
typedef bool (*DefMatInvGaussD)(OUT double *pdInv, IN const double *pdMat, IN int nDim);
// ���������
typedef bool (*DefMatMulVecF)(OUT float *pfRes, IN int nRow, IN const float *pfMat, IN const float *pfVec, IN int nCol);
typedef bool (*DefMatMulVecD)(OUT double *pdRes, IN int nRow, IN const double *pdMat, IN const double *pdVec, IN int nCol);
// �õ�λ����
typedef float* (*pfn3d_m16_identity)(__inout float* mat);
// ��λ�����ж�
typedef bool (*pfn3d_m16_isidentity)(__in float const* mat);
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nx3dmatrixapi
{
	DefMatInvGaussD		pfnMatInvGaussD;
	DefMatMulVecF		pfnMatMulVecF;
	DefMatMulVecD		pfnMatMulVecD;
	// V1.0.9.2
	pfn3d_m16_identity		m16_identity;
	pfn3d_m16_isidentity	m16_isidentity;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����￪ʼ����
} NX3DMatrixAPI, *LPNX3DMATRIXAPI;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// �ӿڽṹ����

typedef struct tag_nx3dinnerapi *LPNX3DINNERAPI;

typedef struct tag_nx3dmathapi
{
	// ��������ӿ�
	LPNX3DVECTORAPI		pStu3DVectorAPI;
	// ��������ӿ�
	LPNX3DMATRIXAPI		pStu3DMatrixAPI;
	// Private Ȩ�ޣ������Ż����ڲ��Ľӿ�
	LPNX3DINNERAPI		pStuInnerAPI;
} NX3DMathAPI, *LPNX3DMATHAPI;


#endif		// #ifndef __NX_3D_MATH_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.