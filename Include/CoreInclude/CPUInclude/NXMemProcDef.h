//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ���õ��ڴ洦��ӿڶ��壬�翽������ת������.
//
// Created: ZhaoZhimeng @ 2008-12-7
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_MEM_PROC_DEF_H__
#define __NX_MEM_PROC_DEF_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#include <crtdefs.h>
#else	// VC6
#include <stddef.h>		// for size_t & intptr_t
#ifndef _INTPTR_T_DEFINED
typedef ptrdiff_t		intptr_t;
#define _INTPTR_T_DEFINED
#endif
#endif		// #if defined(_MSC_VER) && (_MSC_VER >= 1300)

#include ".\NXBasic.h"

//////////////////////////////////////////////////////////////////////////
// ��������

#ifndef IN
#define IN
#endif		// #ifndef IN
#ifndef OUT
#define OUT
#endif		// #ifndef OUT

#ifndef __in
#define __in
#endif

#ifndef __out
#define __out
#endif

#ifndef __inout
#define __inout
#endif

//////////////////////////////////////////////////////////////////////////
// �ⶨ��

#define NXMEMPROC_DLL_NAME			_T("NXMemProc")
#define NXMEMPROC_API_EXT_NAME		"NMP_GetAPIStuPtr"

// ͨ����������
enum nx_channel_type
{
	NX_CHANNEL_NONE,
	NX_CHANNEL_8U,					// unsigned char
	NX_CHANNEL_8S,					// signed char
	NX_CHANNEL_10_LEAST,			// ʹ�� 16bit �ĵ� 10bit �洢
	NX_CHANNEL_10_MOST,				// ʹ�� 16bit �ĸ� 10bit �洢
	NX_CHANNEL_10_STREAM_LEAST,		// 10bit ��ʽ�洢��ʹ�� 32bit �ĵ� 30bit �洢
	NX_CHANNEL_10_STREAM_MOST,		// 10bit ��ʽ�洢��ʹ�� 32bit �ĸ� 30bit �洢
	NX_CHANNEL_12_LEAST,			// ʹ�� 16bit �ĵ� 12bit �洢
	NX_CHANNEL_12_MOST,				// ʹ�� 16bit �ĸ� 12bit �洢
	NX_CHANNEL_14_LEAST,			// ʹ�� 16bit �ĵ� 14bit �洢
	NX_CHANNEL_14_MOST,				// ʹ�� 16bit �ĸ� 14bit �洢
	NX_CHANNEL_16U,					// unsigned short
	NX_CHANNEL_16S,					// signed short
	NX_CHANNEL_24U,					// �޷��� 24bit
	NX_CHANNEL_24S,					// �з��� 24bit
	NX_CHANNEL_32U,					// unsigned int
	NX_CHANNEL_32S,					// signed int
	NX_CHANNEL_64U,					// unsigned __int64
	NX_CHANNEL_64S,					// signed __int64
	NX_CHANNEL_FLOAT,				// �����ȸ�����
	NX_CHANNEL_DOUBLE,				// ˫���ȸ�����
};

//////////////////////////////////////////////////////////////////////////
// ��ȡ�������ݵĽӿڶ���

// ͨ�ÿ���
typedef void* (*DefMemcpy)(OUT void *des, IN const void *src, IN size_t count);
typedef DefMemcpy	DefMemmove, DefMemcpyRev;
// ���ַ��򿽱�
typedef unsigned short* (*DefWMemcpyRev)(OUT unsigned short *des, IN const unsigned short *src, IN size_t count);
// ��˫�ַ��򿽱�
typedef unsigned long* (*DefDMemcpyRev)(OUT unsigned long *des, IN const unsigned long *src, IN size_t count);
// 2d ����
typedef unsigned char* (*DefMemcpy2D)(OUT unsigned char *pucDes,		// Ŀ��
									  IN intptr_t nDesPitch,			// Ŀ��һ��ռ�õ��ֽ���
									  IN const unsigned char *pucSrc,	// Դ
									  IN intptr_t nSrcPitch,			// Դһ��ռ�õ��ֽ���
									  IN size_t nCount,					// һ�п������ֽ�����һ��ҪС��������������ֽ���
									  IN int nLines,					// ����������
									  IN bool bOverturn);				// ��ת��־
// ��ȡ˫���е�ĳ���ֽ�
typedef void (*DefDMemSplitByte)(OUT unsigned char *des,		// �ֽ�Ŀ��
								 IN const unsigned char *src,	// ˫��Դ
								 IN size_t count,				// ˫�ָ���
								 IN int nByteIndex);			// ��ȡ���ֽ����� (0 ~ 3)
typedef void (*DefDMemSplitByte2D)(OUT unsigned char *pucDes,		// �ֽ�Ŀ��
								   IN intptr_t nDesPitch,			// Ŀ��һ��ռ�õ��ֽ���
								   IN const unsigned char *pucSrc,	// ˫��Դ
								   IN intptr_t nSrcPitch,			// Դһ��ռ�õ��ֽ���
								   IN size_t count,					// ˫�ָ���
								   IN int nByteIndex,				// ��ȡ���ֽ����� (0 ~ 3)
								   IN int nLines,					// ����������
								   IN bool bOverturn);				// ��ת��־
// ����һ�����ڵ������ֽڣ������� 16 λ�еĸߵ� 8 λ
typedef void (*DefWMemcpySwapByte)(OUT unsigned char *des,			// ��Ŀ��
								   IN const unsigned char *src,		// ��Դ
								   IN size_t count);				// �ָ���
typedef void (*DefWMemcpySwapByte2D)(OUT unsigned char *pucDes,		// ��Ŀ��
									 IN intptr_t nDesPitch,			// Ŀ��һ��ռ�õ��ֽ���
									 IN const unsigned char *pucSrc,// ��Դ
									 IN intptr_t nSrcPitch,			// Դһ��ռ�õ��ֽ���
									 IN size_t count,				// �ָ���
									 IN int nLines,					// ����������
									 IN bool bOverturn);			// ��ת��־
// ���ַ���һ��˫�֣������� 32 λ�еĸߵ� 16 λ��Դ��Ŀ�Ŀ�����ͬ
typedef void (*pfnmp_wreverse)(__out void* des,		// Ŀ��
							   __in void const* src,// Դ
							   __in int count);		// ˫�ָ���
// ��תʱԴ��Ŀ�Ĳ�����ͬ
typedef void (*pfnmp_wreverse2d)(__out void* des,			// Ŀ��
								 __in intptr_t des_pitch,	// Ŀ��һ�е��ֽ���
								 __in void const* src,		// Դ
								 __in intptr_t src_pitch,	// Դһ�е��ֽ���
								 __in int count,			// һ�е�˫�ָ���
								 __in int line,				// ����
								 __in bool overturn);		// ��ת��־
// ���ֽڷ���һ��˫�֣����С��ת����Դ��Ŀ�Ŀ�����ͬ
typedef pfnmp_wreverse pfnmp_breverse;
// ��תʱԴ��Ŀ�Ĳ�����ͬ
typedef pfnmp_wreverse2d pfnmp_breverse2d;
// ��ȡ�����е�ĳ���ֽ�
typedef DefDMemSplitByte	DefWMemSplitByte;
typedef DefDMemSplitByte2D	DefWMemSplitByte2D;
// ��ȡ�����ֽڿ��е����������ֽڵ�Ԫ��������洢����������һ�� Block ���ֽڳ���Ϊ 2 ����
// �ù�������ȡĳ����ɫ�ռ��ĳ��ͨ������ PCM ��Ƶ�����е�ĳ��������������
typedef bool (*pfnmp_memsplit)(OUT unsigned char *pucDes,			// Ŀ�꣨���������
							   IN const unsigned char *pucSrc,		// Դ
							   IN size_t nBlockCount,				// ����Ŀ����
							   IN size_t nBlockBytes,				// ÿ����ռ���ֽ���
							   IN size_t nCellStart,				// �ӿ�����ȡ����ʼ�ֽ����� (0 ~ nBlockBytes-1)
							   IN size_t nCellBytes);				// ��ȡ���ֽ���
typedef bool (*pfnmp_memsplit2d)(OUT unsigned char *pucDes,			// Ŀ�꣨�����������
								 IN intptr_t nDesPitch,				// Ŀ��һ��ռ�õ��ֽ���
								 IN const unsigned char *pucSrc,	// Դ
								 IN intptr_t nSrcPitch,				// Դһ��ռ�õ��ֽ���
								 IN size_t nBlockCount,				// һ�д���Ŀ����
								 IN size_t nBlockBytes,				// ÿ����ռ���ֽ���
								 IN size_t nCellStart,				// �ӿ�����ȡ����ʼ�ֽ����� (0 ~ nBlockBytes-1)
								 IN size_t nCellBytes,				// ��ȡ���ֽ���
								 IN int nLines,						// ���������
								 IN bool bOverturn);				// ��ת��־
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxmempickapi		// ��ȡ��������
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	// �����ӿ�
	DefMemcpy			pfnMemcpy;
	DefMemmove			pfnMemmove;
	DefMemcpyRev		pfnMemcpyRev;
	DefWMemcpyRev		pfnWMemcpyRev;
	DefDMemcpyRev		pfnDMemcpyRev;
	DefMemcpy2D			pfnMemcpy2D;
	// ��ȡ˫���е�ĳ���ֽ�
	DefDMemSplitByte	pfnDMemSplitByte;
	DefDMemSplitByte2D	pfnDMemSplitByte2D;
	// ���� 16 λ�еĸߵ� 8 λ
	DefWMemcpySwapByte		pfnWMemcpySwapByte;
	DefWMemcpySwapByte2D	pfnWMemcpySwapByte2D;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// ��ȡ�����е�ĳ���ֽ�
	DefWMemSplitByte	pfnWMemSplitByte;
	DefWMemSplitByte2D	pfnWMemSplitByte2D;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.9
	// ˫�����ֽڷ�����ַ���
	pfnmp_breverse		breverse;
	pfnmp_wreverse		wreverse;
	pfnmp_breverse2d	breverse2d;
	pfnmp_wreverse2d	wreverse2d;
	//////////////////////////////////////////////////////////////////////////
	// V1.1.0.3
	// ��ȡ�����ֽڿ��е����������ֽڵ�Ԫ
	pfnmp_memsplit		memsplit;
	pfnmp_memsplit2d	memsplit2d;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXMemPickAPI, *LPNXMEMPICKAPI;

//////////////////////////////////////////////////////////////////////////
// �����������ݵĽӿڶ���

// ���ֽڷ�ת
typedef void (*DefMemrev)(IN OUT void *buf, IN size_t count);
// ���ַ�ת
typedef void (*DefWMemrev)(IN OUT unsigned short *buf, IN size_t count);
// ��˫�ַ�ת
typedef void (*DefDMemrev)(IN OUT unsigned long *buf, IN size_t count);
// �������л��������
typedef void (*DefMemswap)(IN OUT void *buf0, IN OUT void *buf1, IN size_t count);
typedef void (*DefMemOverturn2D)(IN OUT unsigned char *pucBuf,	// ���з�ת�� 2D ����
								 IN intptr_t nBufPitch,			// һ��ռ�õ��ֽ���
								 IN size_t count,				// һ����Ҫ�������ֽ�����һ��ҪС�� nBufPitch
								 IN int nLines);				// 2D ���������

typedef struct tag_nxmemareorderapi		// ������������
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	// ��ת
	DefMemrev		pfnMemrev;
	DefWMemrev		pfnWMemrev;
	DefDMemrev		pfnDMemrev;
	DefMemswap			pfnMemswap;
	DefMemOverturn2D	pfnMemOverturn2D;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXMemReorderAPI, *LPNXMEMREORDERAPI;

//////////////////////////////////////////////////////////////////////////
// ���ӿڶ���

// ����ֽ�
typedef void* (*DefMemset)(OUT void *des, IN int c, IN size_t count);
// �����
typedef unsigned short* (*DefWMemset)(OUT unsigned short *des, IN unsigned short c, IN size_t count);
// ���˫��
typedef unsigned long* (*DefDMemset)(OUT unsigned long *des, IN unsigned long c, IN size_t count);
// �������
typedef unsigned __int64* (*pfnmp_qmemset)(__out unsigned __int64* des, __in unsigned __int64 qword_value, __in size_t qword_count);
// ��� 24 λ��Ԫ��ʽ
typedef void* (*DefMemset24B)(OUT void *des, IN int c, IN size_t count);	// 24 λ��Ԫ�ĸ���
// 2d ���
typedef unsigned char* (*DefMemset2D)(OUT unsigned char *pucDes,	// Ŀ��
									  IN intptr_t nDesPitch,		// Ŀ��һ��ռ�õ��ֽ���
									  IN size_t nCount,				// һ�������ֽ�����һ��ҪС�ڻ�������ֽ���
									  IN int nCharValue,			// �����ֽ�ֵ
									  IN int nLines);				// ��������
typedef unsigned short* (*DefWMemset2D)(OUT unsigned short *pusDes,		// Ŀ��
										IN intptr_t nDesPitch,			// Ŀ��һ��ռ�õ��ֽ���
										IN size_t nCount,				// һ����������
										IN unsigned short usWordValue,	// ������ֵ
										IN int nLines);					// ��������
typedef unsigned long* (*DefDMemset2D)(OUT unsigned long *pulDes,		// Ŀ��
									   IN intptr_t nDesPitch,			// Ŀ��һ��ռ�õ��ֽ���
									   IN size_t nCount,				// һ������˫����
									   IN unsigned long ulDwordValue,	// ����˫��ֵ
									   IN int nLines);					// ��������
typedef unsigned __int64* (*pfnmp_qmemset2d)(__out unsigned __int64* des,		// Ŀ��
											 __in intptr_t des_pitch,			// һ�е��ֽ���
											 __in size_t qword_count,			// һ���������ָ���
											 __in unsigned __int64 qword_value,	// ��������ֵ
											 __in int lines);					// �������
typedef DefMemset2D		DefMemset24B2D;
// ������������ֽڿ��е����������ֽڵ�Ԫ��һ�� Block ���ֽڳ���Ϊ 2 ����
// �ù��������ĳ����ɫ�ռ�� Key ͨ������ PCM ��Ƶ�����е������ϳ�����������
typedef bool (*pfnmp_memcompose)(OUT unsigned char *pucDes,			// Ŀ��
								 IN const unsigned char *pucSrc,	// Դ������������
								 IN size_t nBlockCount,				// ����Ŀ����
								 IN size_t nBlockBytes,				// ÿ����ռ���ֽ���
								 IN size_t nCellStart,				// ��䵽�����ʼ�ֽ����� (0 ~ nBlockBytes-1)
								 IN size_t nCellBytes);				// �����ֽ���
typedef bool (*pfnmp_memcompose2d)(OUT unsigned char *pucDes,		// Ŀ��
								   IN intptr_t nDesPitch,			// Ŀ��һ��ռ�õ��ֽ���
								   IN const unsigned char *pucSrc,	// Դ��������������
								   IN intptr_t nSrcPitch,			// Դһ��ռ�õ��ֽ���
								   IN size_t nBlockCount,			// һ�д���Ŀ����
								   IN size_t nBlockBytes,			// ÿ����ռ���ֽ���
								   IN size_t nCellStart,			// ��䵽�����ʼ�ֽ����� (0 ~ nBlockBytes-1)
								   IN size_t nCellBytes,			// �����ֽ���
								   IN int nLines,					// ���������
								   IN bool bOverturn);				// ��ת��־

typedef struct tagnxmemfillapi		// ���
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	DefMemset		pfnMemset;
	DefWMemset		pfnWMemset;
	DefDMemset		pfnDMemset;
	DefMemset2D		pfnMemset2D;
	DefWMemset2D	pfnWMemset2D;
	DefDMemset2D	pfnDMemset2D;
	DefMemset24B	pfnMemset24B;
	DefMemset24B2D	pfnMemset24B2D;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.15 �������
	pfnmp_qmemset	qmemset;
	pfnmp_qmemset2d	qmemset2d;
	//////////////////////////////////////////////////////////////////////////
	// V1.1.0.3
	pfnmp_memcompose	memcompose;
	pfnmp_memcompose2d	memcompose2d;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXMemFillAPI, *LPNXMEMFILLAPI;

//////////////////////////////////////////////////////////////////////////
// ��������ӿڶ���

// ���޷����ֽ��󱥺ͼӷ���des = saturation(srcL + srcR)��
typedef void (*DefMemsumSat)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
// ���޷����ֽ����Ȩ�����des = srcL * dWeightL + srcR * (1 - dWeightL))��
typedef void (*DefMemmix)(OUT void *des,
						  IN const void *srcL,
						  IN const void *srcR,
						  IN double dWeightL,			// Ҫ��ȡֵ��ΧΪ [0, 1]
						  IN size_t count);
typedef void (*DefMemmix2D)(OUT unsigned char *pucDes,			// Ŀ��
							IN intptr_t nDesPitch,				// Ŀ��һ��ռ�õ��ֽ���
							IN const unsigned char *pucSrcL,	// ��Դ
							IN intptr_t nSrcPitchL,				// ��Դһ��ռ�õ��ֽ���
							IN const unsigned char *pucSrcR,	// ��Դ
							IN intptr_t nSrcPitchR,				// ��Դһ��ռ�õ��ֽ���
							IN double dWeightL,					// ��ԴȨ�أ�Ҫ��ȡֵ��ΧΪ [0, 1]
							IN size_t nCount,					// һ�л�����ֽ�����һ��ҪС�ڻ�������ֽ���
							IN int nLines);						// ���������
// ���Ի��
// des = src_left * src_left_weight + src_right * (1 - src_left_weight)
// Ŀ���������Դ������Դ��ͬ
typedef NXRESULT (*pfnmp_memmix)(__out void* des,						// Ŀ��
								 __in void const* src_left,				// ��Դ
								 __in void const* src_right,			// ��Դ
								 __in double src_left_weight,			// ��ԴȨ�أ���ΧΪ [0, 1]
								 __in nx_channel_type channel_type,		// ͨ����������
								 __in size_t channels);					// ͨ����
// ��תʱĿ�겻������Դ����Դ��ͬ
typedef NXRESULT (*pfnmp_memmix2d)(__out void* des,						// Ŀ��
								   __in intptr_t des_pitch,				// Ŀ��һ�е��ֽ���
								   __in void const* src_left,			// ��Դ
								   __in intptr_t src_left_pitch,		// ��Դһ�е��ֽ���
								   __in void const* src_right,			// ��Դ
								   __in intptr_t src_right_pitch,		// ��Դһ�е��ֽ���
								   __in double src_left_weight,			// ��ԴȨ�أ���ΧΪ [0, 1]
								   __in nx_channel_type channel_type,	// ͨ����������
								   __in size_t channels,				// һ�л����ͨ����
								   __in int lines,						// �������
								   __in bool overturn);					// �Ƿ�ת
// ���޷����ֽ��󱥺ͼ�����des = saturation(srcL - srcR)��
typedef void (*DefMemsubSat)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
// ���޷����ֽ�����Բdes = |srcL - srcR|��
typedef void (*DefMemsubAbs)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
// ���޷����ֽ���ƽ����des = (srcL + srcR) / 2��
typedef void (*DefMemaver)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
typedef void (*DefMemaver2D)(OUT unsigned char *pucDes,			// Ŀ��
							 IN intptr_t nDesPitch,				// Ŀ��һ��ռ�õ��ֽ���
							 IN const unsigned char *pucSrcL,	// ��Դ
							 IN intptr_t nSrcPitchL,			// ��Դһ��ռ�õ��ֽ���
							 IN const unsigned char *pucSrcR,	// ��Դ
							 IN intptr_t nSrcPitchR,			// ��Դһ��ռ�õ��ֽ���
							 IN size_t nCount,					// һ����ƽ�����ֽ�����һ��ҪС�ڻ�������ֽ���
							 IN int nLines);					// ��ƽ��������
// ����ƽ��ֵ
// des = (src_left + src_right) / 2
// Ŀ���������Դ������Դ��ͬ
typedef NXRESULT (*pfnmp_memavg)(__out void* des,						// Ŀ��
								 __in void const* src_left,				// ��Դ
								 __in void const* src_right,			// ��Դ
								 __in nx_channel_type channel_type,		// ͨ����������
								 __in size_t channels);					// ͨ����
// ��תʱĿ�겻������Դ����Դ��ͬ
typedef NXRESULT (*pfnmp_memavg2d)(__out void* des,						// Ŀ��
								   __in intptr_t des_pitch,				// Ŀ��һ�е��ֽ���
								   __in void const* src_left,			// ��Դ
								   __in intptr_t src_left_pitch,		// ��Դһ�е��ֽ���
								   __in void const* src_right,			// ��Դ
								   __in intptr_t src_right_pitch,		// ��Դһ�е��ֽ���
								   __in nx_channel_type channel_type,	// ͨ����������
								   __in size_t channels,				// һ�е�ͨ����
								   __in int lines,						// ����
								   __in bool overturn);					// �Ƿ�ת
// ���޷����ֽ��������Сֵ��des = max/min(srcL, srcR)��
typedef void (*DefMemmax)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
typedef void (*DefMemmin)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
// ���з������������Сֵ��des = max/min(srcL, srcR)��
typedef void (*DefSMemmax)(OUT short *des, IN const short *srcL, IN const short *srcR, IN size_t count);
typedef void (*DefSMemmin)(OUT short *des, IN const short *srcL, IN const short *srcR, IN size_t count);
// ���޷����ֽ��� [0, 1] ������
typedef void (*DefMemgain)(OUT void *des,
						   IN const void *src,
						   IN double dGain,			// Ҫ��ȡֵ��ΧΪ [0, 1]
						   IN size_t count);
typedef void (*DefMemgain2D)(OUT unsigned char *pucDes,			// Ŀ��
							 IN intptr_t nDesPitch,				// Ŀ��һ��ռ�õ��ֽ���
							 IN const unsigned char *pucSrc,	// Դ
							 IN intptr_t nSrcPitch,				// Դһ��ռ�õ��ֽ���
							 IN double dGain,					// Ҫ��ȡֵ��ΧΪ [0, 1]
							 IN size_t nCount,					// һ�д�����ֽ�����һ��ҪС�ڻ�������ֽ���
							 IN int nLines,						// ����
							 IN bool bOvertrun);				// ��ת��־
// ���з����ֽ��� [0, 1] �����棬�ֽڵ�ʵ��ֵ���ڴ洢ֵƫ�� offset
typedef void (*DefMemgainS)(OUT void *des,
							IN const void *src,
							IN double dGain,			// Ҫ��ȡֵ��ΧΪ [0, 1]
							IN unsigned char ucOffset,	// ����ƫ��ֵ
							IN size_t count);
typedef void (*DefMemgainS2D)(OUT unsigned char *pucDes,		// Ŀ��
							  IN intptr_t nDesPitch,			// Ŀ��һ��ռ�õ��ֽ���
							  IN const unsigned char *pucSrc,	// Դ
							  IN intptr_t nSrcPitch,			// Դһ��ռ�õ��ֽ���
							  IN double dGain,					// Ҫ��ȡֵ��ΧΪ [0, 1]
							  IN unsigned char ucOffset,		// ����ƫ��ֵ
							  IN size_t nCount,					// һ�д�����ֽ�����һ��ҪС�ڻ�������ֽ���
							  IN int nLines,					// ����
							  IN bool bOvertrun);				// ��ת��־
// ���з����ֽ���˫���и��ֽ� [0, 1] �����棬�ֽڵ�ʵ��ֵ���ڴ洢ֵƫ�� offset
typedef void (*DefDMemgainSB)(OUT unsigned long *des,
							  IN const unsigned long *src,
							  IN double dGain,				// Ҫ��ȡֵ��ΧΪ [0, 1]
							  IN unsigned long ulOffset,	// ����ƫ��ֵ���ĸ��ֽڶ�����
							  IN size_t count);				// ˫�ֵĸ���
typedef void (*DefDMemgainSB2D)(OUT unsigned long *pulDes,			// Ŀ��
								IN intptr_t nDesPitch,				// Ŀ��һ��ռ�õ��ֽ���
								IN const unsigned long *pulSrc,		// Դ
								IN intptr_t nSrcPitch,				// Դһ��ռ�õ��ֽ���
								IN double dGain,					// Ҫ��ȡֵ��ΧΪ [0, 1]
								IN unsigned long ulOffset,			// ����ƫ��ֵ���ĸ��ֽڶ�����
								IN size_t nCount,					// һ�д����˫����
								IN int nLines,						// ����
								IN bool bOvertrun);					// ��ת��־
// ���з����ֽ��� 24 λ�и��ֽ� [0, 1] �����棬�ֽڵ�ʵ��ֵ���ڴ洢ֵƫ�� offset
typedef void (*Def24BMemgainSB)(OUT unsigned char *des,
								IN const unsigned char *src,
								IN double dGain,				// Ҫ��ȡֵ��ΧΪ [0, 1]
								IN unsigned long ulOffset,		// ����ƫ��ֵ���ĸ��ֽڶ�����
								IN size_t count);				// 24 λ��Ԫ�ĸ���
typedef void (*Def24BMemgainSB2D)(OUT unsigned char *pucDes,		// Ŀ��
								  IN intptr_t nDesPitch,			// Ŀ��һ��ռ�õ��ֽ���
								  IN const unsigned char *pucSrc,	// Դ
								  IN intptr_t nSrcPitch,			// Դһ��ռ�õ��ֽ���
								  IN double dGain,					// Ҫ��ȡֵ��ΧΪ [0, 1]
								  IN unsigned long ulOffset,		// ����ƫ��ֵ���ĸ��ֽڶ�����
								  IN size_t nCount,					// һ�д���� 24 λ��Ԫ��
								  IN int nLines,					// ����
								  IN bool bOvertrun);				// ��ת��־
// ���޷����ֽ����Ȩ�����des = src * dWeight + const * (1 - dWeight))��
typedef void (*DefMemmixC)(OUT void *des,
						   IN const void *src,
						   IN unsigned char ucConst,	// ����
						   IN double dWeight,			// Ҫ��ȡֵ��ΧΪ [0, 1]
						   IN size_t count);
typedef void (*DefMemmixC2D)(OUT unsigned char *pucDes,			// Ŀ��
							 IN intptr_t nDesPitch,				// Ŀ��һ��ռ�õ��ֽ���
							 IN const unsigned char *pucSrc,	// Դ
							 IN intptr_t nSrcPitch,				// Դһ��ռ�õ��ֽ���
							 IN unsigned char ucConst,			// ����
							 IN double dWeight,					// ԴȨ�أ�Ҫ��ȡֵ��ΧΪ [0, 1]
							 IN size_t nCount,					// һ�л�����ֽ�����һ��ҪС�ڻ�������ֽ���
							 IN int nLines,						// ����
							 IN bool bOvertrun);				// ��ת��־
// ���޷����ֽ���˫���и��ֽڵļ�Ȩ�����des = src * dWeight + const * (1 - dWeight))��
typedef void (*DefDMemmixCB)(OUT unsigned long *des,
							 IN const unsigned long *src,
							 IN unsigned long ulConst,		// ����
							 IN double dWeight,				// Ҫ��ȡֵ��ΧΪ [0, 1]
							 IN size_t count);				// ˫�ֵĸ���
typedef void (*DefDMemmixCB2D)(OUT unsigned long *pulDes,		// Ŀ��
							   IN intptr_t nDesPitch,			// Ŀ��һ��ռ�õ��ֽ���
							   IN const unsigned long *pulSrc,	// Դ
							   IN intptr_t nSrcPitch,			// Դһ��ռ�õ��ֽ���
							   IN unsigned long ulConst,		// ����
							   IN double dWeight,				// ԴȨ�أ�Ҫ��ȡֵ��ΧΪ [0, 1]
							   IN size_t nCount,				// һ�д����˫����
							   IN int nLines,					// ����
							   IN bool bOvertrun);				// ��ת��־
// ���޷����ֽ��� 24 λ��Ԫ�и��ֽڵļ�Ȩ�����des = src * dWeight + const * (1 - dWeight))��
typedef void (*Def24BMemmixCB)(OUT unsigned char *des,
							   IN const unsigned char *src,
							   IN unsigned long ulConst,		// ����
							   IN double dWeight,				// Ҫ��ȡֵ��ΧΪ [0, 1]
							   IN size_t count);				// 24 ��Ԫ�ĸ���
typedef void (*Def24BMemmixCB2D)(OUT unsigned char *pucDes,			// Ŀ��
								 IN intptr_t nDesPitch,				// Ŀ��һ��ռ�õ��ֽ���
								 IN const unsigned char *pucSrc,	// Դ
								 IN intptr_t nSrcPitch,				// Դһ��ռ�õ��ֽ���
								 IN unsigned long ulConst,			// ����
								 IN double dWeight,					// ԴȨ�أ�Ҫ��ȡֵ��ΧΪ [0, 1]
								 IN size_t nCount,					// һ�д���� 24 λ��Ԫ��
								 IN int nLines,						// ����
								 IN bool bOvertrun);				// ��ת��־
// ��������棨des = src * gain��
typedef void (*DefFMemgain)(OUT float *des, IN float *src, IN float fGain, IN size_t count);
typedef void (*DefDbMemgain)(OUT double *des, IN double *src, IN double dGain, IN size_t count);
// ������des = srcL - srcR��
typedef void (*DefFMemsub)(OUT float *des, IN float *srcL, IN float *srcR, IN size_t count);
typedef void (*DefDbMemsub)(OUT double *des, IN double *srcL, IN double *srcR, IN size_t count);
// ������ͣ�des = srcL + srcR��
typedef void (*DefFMemadd)(OUT float *des, IN float *srcL, IN float *srcR, IN size_t count);
typedef void (*DefDbMemadd)(OUT double *des, IN double *srcL, IN double *srcR, IN size_t count);
// �޷����ֽ���ƽ���ͣ�֧�����������
typedef double (*DefMemsqsum)(IN unsigned char *src, IN int stride, IN int count);
// �������ȡ����des = -src��
typedef void (*pfnmp_fnegtive)(__out float *des, __in float *src, __in size_t count);
typedef void (*pfnmp_dnegtive)(__out double *des, __in double *src, __in size_t count);
// ����ƽ����
typedef float (*pfnmp_sumsquare_f)(__in float const* src, __in int count);
typedef double (*pfnmp_sumsquare_d)(__in double const* src, __in int count);
// �����������ֵ���ֵ
typedef float (*pfnmp_absmax_f)(__in float const* src, __in int count);
typedef double (*pfnmp_absmax_d)(__in double const* src, __in int count);
// ��ֵ�����
typedef double (*pfnmp_psnr)(__in unsigned char* src0, __in unsigned char* src1, __in int count);
typedef double (*pfnmp_psnr2d)(__in unsigned char* src0,
							   __in intptr_t src0_pitch,
							   __in unsigned char* src1,
							   __in intptr_t src1_pitch,
							   __in int count,
							   __in int lines);
// ���������des = src1 * src2)
typedef void (*pfnmp_memmul_f)(__out float* des, __in float* src1, __in float* src2, __in int count);
// des = src * src
typedef void (*pfnmp_mempow2_f)(__out float* des, __in float* src, __in int count);
// Structural Similarity
typedef float (*pfnmp_ssim)(__out float* ssim_map,			// ��Ϊ NULL
							__in intptr_t ssim_map_pitch,
							__in unsigned char* img1,		// ͼ�� 1
							__in intptr_t img1_pitch,
							__in unsigned char* img2,		// ͼ�� 2
							__in intptr_t img2_pitch,
							__in int img_width,				// ͼ�����
							__in int img_height,
							__in double K1,					// �Ƽ� 0.01
							__in double K2,					// �Ƽ� 0.03
							__in int L,						// �Ƽ� 255
							__in int downsamplewidth,		// �Ƽ� 256�����ֱ�Ӵ����� 1
							__in int gaussian_window,		// �Ƽ� 11
							__in double gaussian_sigma);	// �Ƽ� 1.5

//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxmemmathapi		// ��������
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	DefMemsumSat	pfnMemsumSat;
	DefMemmix		pfnMemmix;
	DefMemsubSat	pfnMemsubSat;
	DefMemsubAbs	pfnMemsubAbs;
	DefMemaver		pfnMemaver;
	DefMemmax		pfnMemmax;
	DefMemmin		pfnMemmin;
	DefSMemmax		pfnSMemmax;
	DefSMemmin		pfnSMemmin;
	DefMemmix2D		pfnMemmix2D;
	DefMemaver2D	pfnMemaver2D;
	DefMemgain		pfnMemgain;			// �޷����ֽ����
	DefMemgain2D	pfnMemgain2D;
	DefMemgainS			pfnMemgainS;		// �з����ֽ����
	DefMemgainS2D		pfnMemgainS2D;
	DefDMemgainSB		pfnDMemgainSB;
	DefDMemgainSB2D		pfnDMemgainSB2D;
	Def24BMemgainSB		pfn24BMemgainSB;
	Def24BMemgainSB2D	pfn24BMemgainSB2D;
	DefMemmixC			pfnMemmixC;		// �޷����ֽ��볣�����
	DefMemmixC2D		pfnMemmixC2D;
	DefDMemmixCB		pfnDMemmixCB;
	DefDMemmixCB2D		pfnDMemmixCB2D;
	Def24BMemmixCB		pfn24BMemmixCB;
	Def24BMemmixCB2D	pfn24BMemmixCB2D;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	DefFMemgain			pfnFMemgain;
	DefDbMemgain		pfnDbMemgain;
	DefFMemsub			pfnFMemsub;
	DefDbMemsub			pfnDbMemsub;
	DefFMemadd			pfnFMemadd;
	DefDbMemadd			pfnDbMemadd;
	DefMemsqsum			pfnMemsqsum;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.9.2
	pfnmp_fnegtive		fnegtive;
	pfnmp_dnegtive		dnegtive;
	pfnmp_memmix		memmix;			// ���Ի��
	pfnmp_memmix2d		memmix2d;
	pfnmp_memavg		memavg;			// ����ƽ��ֵ
	pfnmp_memavg2d		memavg2d;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.9.9
	pfnmp_sumsquare_f	sumsquare_f;
	pfnmp_sumsquare_d	sumsquare_d;
	pfnmp_absmax_f		absmax_f;
	pfnmp_absmax_d		absmax_d;
	// V1.1.0.3
	pfnmp_psnr			psnr;
	pfnmp_psnr2d		psnr2d;
	// V1.1.0.4
	pfnmp_memmul_f		memmul_f;
	pfnmp_mempow2_f		mempow2_f;
	pfnmp_ssim			ssim;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXMemMathAPI, *LPNXMEMMATHAPI;


//////////////////////////////////////////////////////////////////////////
// ��������ת���ӿڶ���

// unsigned char -> float ��ʽת��������ֵ�仯
typedef void (*DefUChar2Float)(OUT float *des, IN unsigned char *src, IN size_t count);
// int -> float ��ʽת��������ֵ�仯
typedef void (*DefInt2Float)(OUT float *des, IN int *src, IN size_t count);
// float <-> double ��ʽת��������ֵ�仯
typedef void (*DefDouble2Float)(OUT float *des, IN const double *src, IN size_t count);
typedef void (*DefFloat2Double)(OUT double *des, IN const float *src, IN size_t count);
// char -> short/s24/int ��������ת��
typedef void (*DefCharMap2Short)(OUT short *des, IN const char *src, IN size_t count);
typedef void (*DefCharMap2S24)(OUT unsigned char *des, IN const char *src, IN size_t count);
typedef void (*DefCharMap2Int)(OUT int *des, IN const char *src, IN size_t count);
// short -> s24/int ��������ת��
typedef void (*DefShortMap2S24)(OUT unsigned char *des, IN const short *src, IN size_t count);
typedef void (*DefShortMap2Int)(OUT int *des, IN const short *src, IN size_t count);
// s24 -> int ��������ת��
typedef void (*DefS24Map2Int)(OUT int *des, IN const unsigned char *src, IN size_t count);
// char/short/s24/int -> float/double ��һ��
typedef void (*DefCharNorm2Float)(OUT float *des, IN const char *src, IN size_t count);
typedef void (*DefShortNorm2Float)(OUT float *des, IN const short *src, IN size_t count);
typedef void (*DefS24Norm2Float)(OUT float *des, IN const unsigned char *src, IN size_t count);
typedef void (*DefIntNorm2Float)(OUT float *des, IN const int *src, IN size_t count);
typedef void (*DefCharNorm2Double)(OUT double *des, IN const char *src, IN size_t count);
typedef void (*DefShortNorm2Double)(OUT double *des, IN const short *src, IN size_t count);
typedef void (*DefS24Norm2Double)(OUT double *des, IN const unsigned char *src, IN size_t count);
typedef void (*DefIntNorm2Double)(OUT double *des, IN const int *src, IN size_t count);
// ��һ�� float/double -> char/short/s24/int ��������ԭ
typedef void (*DefNormFloat2Char)(OUT char *des, IN const float *src, IN size_t count);
typedef void (*DefNormFloat2Short)(OUT short *des, IN const float *src, IN size_t count);
typedef void (*DefNormFloat2S24)(OUT unsigned char *des, IN const float *src, IN size_t count);
typedef void (*DefNormFloat2Int)(OUT int *des, IN const float *src, IN size_t count);
typedef void (*DefNormDouble2Char)(OUT char *des, IN const double *src, IN size_t count);
typedef void (*DefNormDouble2Short)(OUT short *des, IN const double *src, IN size_t count);
typedef void (*DefNormDouble2S24)(OUT unsigned char *des, IN const double *src, IN size_t count);
typedef void (*DefNormDouble2Int)(OUT int *des, IN const double *src, IN size_t count);
typedef void (*pfnmp_normfloat2uchar)(__out unsigned char* des, __in float const* src, __in size_t count);
typedef void (*pfnmp_memconversion)(__out unsigned char* des,
									__in nx_channel_type des_type,
									__in unsigned char* src,
									__in nx_channel_type src_type,
									__in size_t count);

//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxmemconvapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	DefUChar2Float		pfnUChar2Float;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.17
	DefInt2Float		pfnInt2Float;
	DefDouble2Float		pfnDouble2Float;
	DefFloat2Double		pfnFloat2Double;
	DefCharMap2Short	pfnCharMap2Short;	// ������ȸı�
	DefCharMap2S24		pfnCharMap2S24;
	DefCharMap2Int		pfnCharMap2Int;
	DefShortMap2S24		pfnShortMap2S24;
	DefShortMap2Int		pfnShortMap2Int;
	DefS24Map2Int		pfnS24Map2Int;
	DefCharNorm2Float		pfnCharNorm2Float;	// ��һ��
	DefShortNorm2Float		pfnShortNorm2Float;
	DefS24Norm2Float		pfnS24Norm2Float;
	DefIntNorm2Float		pfnIntNorm2Float;
	DefCharNorm2Double		pfnCharNorm2Double;
	DefShortNorm2Double		pfnShortNorm2Double;
	DefS24Norm2Double		pfnS24Norm2Double;
	DefIntNorm2Double		pfnIntNorm2Double;
	DefNormFloat2Char		pfnNormFloat2Char;	// ������ԭ
	DefNormFloat2Short		pfnNormFloat2Short;
	DefNormFloat2S24		pfnNormFloat2S24;
	DefNormFloat2Int		pfnNormFloat2Int;
	DefNormDouble2Char		pfnNormDouble2Char;
	DefNormDouble2Short		pfnNormDouble2Short;
	DefNormDouble2S24		pfnNormDouble2S24;
	DefNormDouble2Int		pfnNormDouble2Int;
	// V1.1.0.3
	pfnmp_normfloat2uchar	normfloat2uchar;
	// V1.1.0.5
	pfnmp_memconversion		memconversion;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����￪ʼ
} NXMemConvAPI, *LPNXMEMCONVAPI;


//////////////////////////////////////////////////////////////////////////
// �����ӿڶ���

// �޷����ڴ�Ƚ�
typedef int (*DefMemcmp)(IN const void *buf1, IN const void *buf2, IN size_t count);

typedef struct tag_nxmemmiscapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	DefMemcmp			pfnMemcmp;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXMemMiscAPI, *LPNXMEMMISCAPI;


//////////////////////////////////////////////////////////////////////////
// �ӿڽṹ����

typedef struct tag_nxmeminnerapi *LPNXMEMINNERAPI;

typedef struct tag_nxmemprocapi
{
	// ��ȡ�������ݽӿ�
	LPNXMEMPICKAPI		pStuMemPickAPI;
	// �����������ݽӿ�
	LPNXMEMREORDERAPI	pStuMemReorderAPI;
	// ���ӿ�
	LPNXMEMFILLAPI		pStuMemFillAPI;
	// ��������ӿ�
	LPNXMEMMATHAPI		pStuMemMathAPI;
	// ����ת���ӿ�
	LPNXMEMCONVAPI		pStuMemConvAPI;
	// �����ӿ�
	LPNXMEMMISCAPI		pStuMemMiscAPI;
	// Private Ȩ�ޣ������Ż����ڲ��Ľӿ�
	LPNXMEMINNERAPI		pStuInnerAPI;
} NXMemProcAPI, *LPNXMEMPROCAPI;


#endif		// #ifndef __NX_MEM_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
