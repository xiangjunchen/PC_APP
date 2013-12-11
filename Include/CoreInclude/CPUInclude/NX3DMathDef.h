//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 3D 数学运算，如向量、矩阵、四元数等.
//
// Created: ZhaoZhimeng @ 2010-07-27
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_3D_MATH_DEF_H__
#define __NX_3D_MATH_DEF_H__

#include ".\NXBasic.h"

//////////////////////////////////////////////////////////////////////////
// 编译声明

#ifndef IN
#define IN
#endif		// #ifndef IN
#ifndef OUT
#define OUT
#endif		// #ifndef OUT


//////////////////////////////////////////////////////////////////////////
// 库定义

#define NX3DMATH_DLL_NAME			_T("NX3DMath")
#define NX3DMATH_API_EXT_NAME		"N3D_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// 向量运算接口定义

// 点乘
typedef float (*DefVecDotF)(IN const float *pfVec1, IN const float *pfVec2, IN int nDim);
typedef double (*DefVecDotD)(IN const double *pdVec1, IN const double *pdVec2, IN int nDim);
// 求模
typedef float (*DefVecLengthF)(IN const float *pfVec, IN int nDim);
typedef double (*DefVecLengthD)(IN const double *pdVec, IN int nDim);
// 求模的平方
typedef DefVecLengthF pfn3d_length2_f;
typedef DefVecLengthD pfn3d_length2_d;
// 求距离
typedef DefVecDotF	DefVecDistanceF;
typedef DefVecDotD	DefVecDistanceD;
// 求逆
typedef void (*DefVecInverseF)(OUT float *pfVecRes, IN const float *pfVecSrc, IN int nDim);
typedef void (*DefVecInverseD)(OUT double *pdVecRes, IN const double *pdVecSrc, IN int nDim);
// 归一化
typedef DefVecInverseF	DefVecNormalizeF;
typedef DefVecInverseD	DefVecNormalizeD;
// 数乘
typedef void (*DefVecMulF)(OUT float *pfVecRes, IN const float *pfVec1, IN const float *pfVec2, IN int nDim);
typedef void (*DefVecMulD)(OUT double *pdVecRes, IN const double *pdVec1, IN const double *pdVec2, IN int nDim);
// 叉乘
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
	// 新版本接口从这里开始声明
} NX3DVectorAPI, *LPNX3DVECTORAPI;


//////////////////////////////////////////////////////////////////////////
// 矩阵运算接口

// 高斯消元求解方阵的逆
typedef bool (*DefMatInvGaussD)(OUT double *pdInv, IN const double *pdMat, IN int nDim);
// 矩阵乘向量
typedef bool (*DefMatMulVecF)(OUT float *pfRes, IN int nRow, IN const float *pfMat, IN const float *pfVec, IN int nCol);
typedef bool (*DefMatMulVecD)(OUT double *pdRes, IN int nRow, IN const double *pdMat, IN const double *pdVec, IN int nCol);
// 置单位矩阵
typedef float* (*pfn3d_m16_identity)(__inout float* mat);
// 单位矩阵判断
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
	// 新版本接口从这里开始声明
} NX3DMatrixAPI, *LPNX3DMATRIXAPI;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 接口结构定义

typedef struct tag_nx3dinnerapi *LPNX3DINNERAPI;

typedef struct tag_nx3dmathapi
{
	// 向量运算接口
	LPNX3DVECTORAPI		pStu3DVectorAPI;
	// 矩阵运算接口
	LPNX3DMATRIXAPI		pStu3DMatrixAPI;
	// Private 权限，用于优化库内部的接口
	LPNX3DINNERAPI		pStuInnerAPI;
} NX3DMathAPI, *LPNX3DMATHAPI;


#endif		// #ifndef __NX_3D_MATH_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.