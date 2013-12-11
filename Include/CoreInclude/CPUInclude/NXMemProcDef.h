//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 常用的内存处理接口定义，如拷贝、翻转、填充等.
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
// 编译声明

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
// 库定义

#define NXMEMPROC_DLL_NAME			_T("NXMemProc")
#define NXMEMPROC_API_EXT_NAME		"NMP_GetAPIStuPtr"

// 通道数据类型
enum nx_channel_type
{
	NX_CHANNEL_NONE,
	NX_CHANNEL_8U,					// unsigned char
	NX_CHANNEL_8S,					// signed char
	NX_CHANNEL_10_LEAST,			// 使用 16bit 的低 10bit 存储
	NX_CHANNEL_10_MOST,				// 使用 16bit 的高 10bit 存储
	NX_CHANNEL_10_STREAM_LEAST,		// 10bit 流式存储，使用 32bit 的低 30bit 存储
	NX_CHANNEL_10_STREAM_MOST,		// 10bit 流式存储，使用 32bit 的高 30bit 存储
	NX_CHANNEL_12_LEAST,			// 使用 16bit 的低 12bit 存储
	NX_CHANNEL_12_MOST,				// 使用 16bit 的高 12bit 存储
	NX_CHANNEL_14_LEAST,			// 使用 16bit 的低 14bit 存储
	NX_CHANNEL_14_MOST,				// 使用 16bit 的高 14bit 存储
	NX_CHANNEL_16U,					// unsigned short
	NX_CHANNEL_16S,					// signed short
	NX_CHANNEL_24U,					// 无符号 24bit
	NX_CHANNEL_24S,					// 有符号 24bit
	NX_CHANNEL_32U,					// unsigned int
	NX_CHANNEL_32S,					// signed int
	NX_CHANNEL_64U,					// unsigned __int64
	NX_CHANNEL_64S,					// signed __int64
	NX_CHANNEL_FLOAT,				// 单精度浮点数
	NX_CHANNEL_DOUBLE,				// 双精度浮点数
};

//////////////////////////////////////////////////////////////////////////
// 提取部分内容的接口定义

// 通用拷贝
typedef void* (*DefMemcpy)(OUT void *des, IN const void *src, IN size_t count);
typedef DefMemcpy	DefMemmove, DefMemcpyRev;
// 按字反向拷贝
typedef unsigned short* (*DefWMemcpyRev)(OUT unsigned short *des, IN const unsigned short *src, IN size_t count);
// 按双字反向拷贝
typedef unsigned long* (*DefDMemcpyRev)(OUT unsigned long *des, IN const unsigned long *src, IN size_t count);
// 2d 拷贝
typedef unsigned char* (*DefMemcpy2D)(OUT unsigned char *pucDes,		// 目标
									  IN intptr_t nDesPitch,			// 目标一行占用的字节数
									  IN const unsigned char *pucSrc,	// 源
									  IN intptr_t nSrcPitch,			// 源一行占用的字节数
									  IN size_t nCount,					// 一行拷贝的字节数，一般要小于两个缓存的行字节数
									  IN int nLines,					// 拷贝的行数
									  IN bool bOverturn);				// 翻转标志
// 提取双字中的某个字节
typedef void (*DefDMemSplitByte)(OUT unsigned char *des,		// 字节目标
								 IN const unsigned char *src,	// 双字源
								 IN size_t count,				// 双字个数
								 IN int nByteIndex);			// 提取的字节索引 (0 ~ 3)
typedef void (*DefDMemSplitByte2D)(OUT unsigned char *pucDes,		// 字节目标
								   IN intptr_t nDesPitch,			// 目标一行占用的字节数
								   IN const unsigned char *pucSrc,	// 双字源
								   IN intptr_t nSrcPitch,			// 源一行占用的字节数
								   IN size_t count,					// 双字个数
								   IN int nByteIndex,				// 提取的字节索引 (0 ~ 3)
								   IN int nLines,					// 拷贝的行数
								   IN bool bOverturn);				// 翻转标志
// 交换一个字内的两个字节，即交换 16 位中的高低 8 位
typedef void (*DefWMemcpySwapByte)(OUT unsigned char *des,			// 字目标
								   IN const unsigned char *src,		// 字源
								   IN size_t count);				// 字个数
typedef void (*DefWMemcpySwapByte2D)(OUT unsigned char *pucDes,		// 字目标
									 IN intptr_t nDesPitch,			// 目标一行占用的字节数
									 IN const unsigned char *pucSrc,// 字源
									 IN intptr_t nSrcPitch,			// 源一行占用的字节数
									 IN size_t count,				// 字个数
									 IN int nLines,					// 拷贝的行数
									 IN bool bOverturn);			// 翻转标志
// 按字反序一个双字，即交换 32 位中的高低 16 位，源与目的可以相同
typedef void (*pfnmp_wreverse)(__out void* des,		// 目的
							   __in void const* src,// 源
							   __in int count);		// 双字个数
// 翻转时源与目的不能相同
typedef void (*pfnmp_wreverse2d)(__out void* des,			// 目的
								 __in intptr_t des_pitch,	// 目的一行的字节数
								 __in void const* src,		// 源
								 __in intptr_t src_pitch,	// 源一行的字节数
								 __in int count,			// 一行的双字个数
								 __in int line,				// 行数
								 __in bool overturn);		// 翻转标志
// 按字节反序一个双字，如大小端转换，源与目的可以相同
typedef pfnmp_wreverse pfnmp_breverse;
// 翻转时源与目的不能相同
typedef pfnmp_wreverse2d pfnmp_breverse2d;
// 提取单字中的某个字节
typedef DefDMemSplitByte	DefWMemSplitByte;
typedef DefDMemSplitByte2D	DefWMemSplitByte2D;
// 提取任意字节块中的任意连续字节单元，并将其存储到连续对象，一般 Block 的字节长多为 2 的幂
// 该功能在提取某种颜色空间的某个通道，或 PCM 音频数据中的某个声道作用显著
typedef bool (*pfnmp_memsplit)(OUT unsigned char *pucDes,			// 目标（输出连续）
							   IN const unsigned char *pucSrc,		// 源
							   IN size_t nBlockCount,				// 处理的块个数
							   IN size_t nBlockBytes,				// 每个块占的字节数
							   IN size_t nCellStart,				// 从块中提取的起始字节索引 (0 ~ nBlockBytes-1)
							   IN size_t nCellBytes);				// 提取的字节数
typedef bool (*pfnmp_memsplit2d)(OUT unsigned char *pucDes,			// 目标（行输出连续）
								 IN intptr_t nDesPitch,				// 目标一行占用的字节数
								 IN const unsigned char *pucSrc,	// 源
								 IN intptr_t nSrcPitch,				// 源一行占用的字节数
								 IN size_t nBlockCount,				// 一行处理的块个数
								 IN size_t nBlockBytes,				// 每个块占的字节数
								 IN size_t nCellStart,				// 从块中提取的起始字节索引 (0 ~ nBlockBytes-1)
								 IN size_t nCellBytes,				// 提取的字节数
								 IN int nLines,						// 处理的行数
								 IN bool bOverturn);				// 翻转标志
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxmempickapi		// 提取部分内容
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	// 拷贝接口
	DefMemcpy			pfnMemcpy;
	DefMemmove			pfnMemmove;
	DefMemcpyRev		pfnMemcpyRev;
	DefWMemcpyRev		pfnWMemcpyRev;
	DefDMemcpyRev		pfnDMemcpyRev;
	DefMemcpy2D			pfnMemcpy2D;
	// 提取双字中的某个字节
	DefDMemSplitByte	pfnDMemSplitByte;
	DefDMemSplitByte2D	pfnDMemSplitByte2D;
	// 交换 16 位中的高低 8 位
	DefWMemcpySwapByte		pfnWMemcpySwapByte;
	DefWMemcpySwapByte2D	pfnWMemcpySwapByte2D;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// 提取单字中的某个字节
	DefWMemSplitByte	pfnWMemSplitByte;
	DefWMemSplitByte2D	pfnWMemSplitByte2D;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.9
	// 双字中字节反序和字反序
	pfnmp_breverse		breverse;
	pfnmp_wreverse		wreverse;
	pfnmp_breverse2d	breverse2d;
	pfnmp_wreverse2d	wreverse2d;
	//////////////////////////////////////////////////////////////////////////
	// V1.1.0.3
	// 提取任意字节块中的任意连续字节单元
	pfnmp_memsplit		memsplit;
	pfnmp_memsplit2d	memsplit2d;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXMemPickAPI, *LPNXMEMPICKAPI;

//////////////////////////////////////////////////////////////////////////
// 重新排列内容的接口定义

// 按字节翻转
typedef void (*DefMemrev)(IN OUT void *buf, IN size_t count);
// 按字翻转
typedef void (*DefWMemrev)(IN OUT unsigned short *buf, IN size_t count);
// 按双字翻转
typedef void (*DefDMemrev)(IN OUT unsigned long *buf, IN size_t count);
// 交换两行缓存的内容
typedef void (*DefMemswap)(IN OUT void *buf0, IN OUT void *buf1, IN size_t count);
typedef void (*DefMemOverturn2D)(IN OUT unsigned char *pucBuf,	// 按行翻转的 2D 缓存
								 IN intptr_t nBufPitch,			// 一行占用的字节数
								 IN size_t count,				// 一行需要操作的字节数，一般要小于 nBufPitch
								 IN int nLines);				// 2D 缓存的行数

typedef struct tag_nxmemareorderapi		// 重新排列内容
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	// 翻转
	DefMemrev		pfnMemrev;
	DefWMemrev		pfnWMemrev;
	DefDMemrev		pfnDMemrev;
	DefMemswap			pfnMemswap;
	DefMemOverturn2D	pfnMemOverturn2D;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXMemReorderAPI, *LPNXMEMREORDERAPI;

//////////////////////////////////////////////////////////////////////////
// 填充接口定义

// 填充字节
typedef void* (*DefMemset)(OUT void *des, IN int c, IN size_t count);
// 填充字
typedef unsigned short* (*DefWMemset)(OUT unsigned short *des, IN unsigned short c, IN size_t count);
// 填充双字
typedef unsigned long* (*DefDMemset)(OUT unsigned long *des, IN unsigned long c, IN size_t count);
// 填充四字
typedef unsigned __int64* (*pfnmp_qmemset)(__out unsigned __int64* des, __in unsigned __int64 qword_value, __in size_t qword_count);
// 填充 24 位单元格式
typedef void* (*DefMemset24B)(OUT void *des, IN int c, IN size_t count);	// 24 位单元的个数
// 2d 填充
typedef unsigned char* (*DefMemset2D)(OUT unsigned char *pucDes,	// 目标
									  IN intptr_t nDesPitch,		// 目标一行占用的字节数
									  IN size_t nCount,				// 一行填充的字节数，一般要小于缓存的行字节数
									  IN int nCharValue,			// 填充的字节值
									  IN int nLines);				// 填充的行数
typedef unsigned short* (*DefWMemset2D)(OUT unsigned short *pusDes,		// 目标
										IN intptr_t nDesPitch,			// 目标一行占用的字节数
										IN size_t nCount,				// 一行填充的字数
										IN unsigned short usWordValue,	// 填充的字值
										IN int nLines);					// 填充的行数
typedef unsigned long* (*DefDMemset2D)(OUT unsigned long *pulDes,		// 目标
									   IN intptr_t nDesPitch,			// 目标一行占用的字节数
									   IN size_t nCount,				// 一行填充的双字数
									   IN unsigned long ulDwordValue,	// 填充的双字值
									   IN int nLines);					// 填充的行数
typedef unsigned __int64* (*pfnmp_qmemset2d)(__out unsigned __int64* des,		// 目标
											 __in intptr_t des_pitch,			// 一行的字节数
											 __in size_t qword_count,			// 一行填充的四字个数
											 __in unsigned __int64 qword_value,	// 填充的四字值
											 __in int lines);					// 填充行数
typedef DefMemset2D		DefMemset24B2D;
// 填充任意连续字节块中的任意连续字节单元，一般 Block 的字节长多为 2 的幂
// 该功能在组合某种颜色空间的 Key 通道，或 PCM 音频数据中的声道合成中作用显著
typedef bool (*pfnmp_memcompose)(OUT unsigned char *pucDes,			// 目标
								 IN const unsigned char *pucSrc,	// 源（输入连续）
								 IN size_t nBlockCount,				// 处理的块个数
								 IN size_t nBlockBytes,				// 每个块占的字节数
								 IN size_t nCellStart,				// 填充到块的起始字节索引 (0 ~ nBlockBytes-1)
								 IN size_t nCellBytes);				// 填充的字节数
typedef bool (*pfnmp_memcompose2d)(OUT unsigned char *pucDes,		// 目标
								   IN intptr_t nDesPitch,			// 目标一行占用的字节数
								   IN const unsigned char *pucSrc,	// 源（行输入连续）
								   IN intptr_t nSrcPitch,			// 源一行占用的字节数
								   IN size_t nBlockCount,			// 一行处理的块个数
								   IN size_t nBlockBytes,			// 每个块占的字节数
								   IN size_t nCellStart,			// 填充到块的起始字节索引 (0 ~ nBlockBytes-1)
								   IN size_t nCellBytes,			// 填充的字节数
								   IN int nLines,					// 处理的行数
								   IN bool bOverturn);				// 翻转标志

typedef struct tagnxmemfillapi		// 填充
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
	// V1.0.8.15 四字填充
	pfnmp_qmemset	qmemset;
	pfnmp_qmemset2d	qmemset2d;
	//////////////////////////////////////////////////////////////////////////
	// V1.1.0.3
	pfnmp_memcompose	memcompose;
	pfnmp_memcompose2d	memcompose2d;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXMemFillAPI, *LPNXMEMFILLAPI;

//////////////////////////////////////////////////////////////////////////
// 线性运算接口定义

// 按无符号字节求饱和加法（des = saturation(srcL + srcR)）
typedef void (*DefMemsumSat)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
// 按无符号字节求加权混叠（des = srcL * dWeightL + srcR * (1 - dWeightL))）
typedef void (*DefMemmix)(OUT void *des,
						  IN const void *srcL,
						  IN const void *srcR,
						  IN double dWeightL,			// 要求取值范围为 [0, 1]
						  IN size_t count);
typedef void (*DefMemmix2D)(OUT unsigned char *pucDes,			// 目标
							IN intptr_t nDesPitch,				// 目标一行占用的字节数
							IN const unsigned char *pucSrcL,	// 左源
							IN intptr_t nSrcPitchL,				// 左源一行占用的字节数
							IN const unsigned char *pucSrcR,	// 右源
							IN intptr_t nSrcPitchR,				// 右源一行占用的字节数
							IN double dWeightL,					// 左源权重，要求取值范围为 [0, 1]
							IN size_t nCount,					// 一行混叠的字节数，一般要小于缓存的行字节数
							IN int nLines);						// 混叠的行数
// 线性混叠
// des = src_left * src_left_weight + src_right * (1 - src_left_weight)
// 目标可以与左源或者右源相同
typedef NXRESULT (*pfnmp_memmix)(__out void* des,						// 目标
								 __in void const* src_left,				// 左源
								 __in void const* src_right,			// 右源
								 __in double src_left_weight,			// 左源权重，范围为 [0, 1]
								 __in nx_channel_type channel_type,		// 通道数据类型
								 __in size_t channels);					// 通道数
// 反转时目标不能与左源或右源相同
typedef NXRESULT (*pfnmp_memmix2d)(__out void* des,						// 目标
								   __in intptr_t des_pitch,				// 目标一行的字节数
								   __in void const* src_left,			// 左源
								   __in intptr_t src_left_pitch,		// 左源一行的字节数
								   __in void const* src_right,			// 右源
								   __in intptr_t src_right_pitch,		// 右源一行的字节数
								   __in double src_left_weight,			// 左源权重，范围为 [0, 1]
								   __in nx_channel_type channel_type,	// 通道数据类型
								   __in size_t channels,				// 一行混叠的通道数
								   __in int lines,						// 混叠行数
								   __in bool overturn);					// 是否反转
// 按无符号字节求饱和减法（des = saturation(srcL - srcR)）
typedef void (*DefMemsubSat)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
// 按无符号字节求绝对差（des = |srcL - srcR|）
typedef void (*DefMemsubAbs)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
// 按无符号字节求平均（des = (srcL + srcR) / 2）
typedef void (*DefMemaver)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
typedef void (*DefMemaver2D)(OUT unsigned char *pucDes,			// 目标
							 IN intptr_t nDesPitch,				// 目标一行占用的字节数
							 IN const unsigned char *pucSrcL,	// 左源
							 IN intptr_t nSrcPitchL,			// 左源一行占用的字节数
							 IN const unsigned char *pucSrcR,	// 右源
							 IN intptr_t nSrcPitchR,			// 右源一行占用的字节数
							 IN size_t nCount,					// 一行求平均的字节数，一般要小于缓存的行字节数
							 IN int nLines);					// 求平均的行数
// 计算平均值
// des = (src_left + src_right) / 2
// 目标可以与左源或者右源相同
typedef NXRESULT (*pfnmp_memavg)(__out void* des,						// 目标
								 __in void const* src_left,				// 左源
								 __in void const* src_right,			// 右源
								 __in nx_channel_type channel_type,		// 通道数据类型
								 __in size_t channels);					// 通道数
// 反转时目标不能与左源或右源相同
typedef NXRESULT (*pfnmp_memavg2d)(__out void* des,						// 目标
								   __in intptr_t des_pitch,				// 目标一行的字节数
								   __in void const* src_left,			// 左源
								   __in intptr_t src_left_pitch,		// 左源一行的字节数
								   __in void const* src_right,			// 右源
								   __in intptr_t src_right_pitch,		// 右源一行的字节数
								   __in nx_channel_type channel_type,	// 通道数据类型
								   __in size_t channels,				// 一行的通道数
								   __in int lines,						// 行数
								   __in bool overturn);					// 是否反转
// 按无符号字节求最大最小值（des = max/min(srcL, srcR)）
typedef void (*DefMemmax)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
typedef void (*DefMemmin)(OUT void *des, IN const void *srcL, IN const void *srcR, IN size_t count);
// 按有符号字求最大最小值（des = max/min(srcL, srcR)）
typedef void (*DefSMemmax)(OUT short *des, IN const short *srcL, IN const short *srcR, IN size_t count);
typedef void (*DefSMemmin)(OUT short *des, IN const short *srcL, IN const short *srcR, IN size_t count);
// 按无符号字节求 [0, 1] 的增益
typedef void (*DefMemgain)(OUT void *des,
						   IN const void *src,
						   IN double dGain,			// 要求取值范围为 [0, 1]
						   IN size_t count);
typedef void (*DefMemgain2D)(OUT unsigned char *pucDes,			// 目标
							 IN intptr_t nDesPitch,				// 目标一行占用的字节数
							 IN const unsigned char *pucSrc,	// 源
							 IN intptr_t nSrcPitch,				// 源一行占用的字节数
							 IN double dGain,					// 要求取值范围为 [0, 1]
							 IN size_t nCount,					// 一行处理的字节数，一般要小于缓存的行字节数
							 IN int nLines,						// 行数
							 IN bool bOvertrun);				// 翻转标志
// 按有符号字节求 [0, 1] 的增益，字节的实际值等于存储值偏移 offset
typedef void (*DefMemgainS)(OUT void *des,
							IN const void *src,
							IN double dGain,			// 要求取值范围为 [0, 1]
							IN unsigned char ucOffset,	// 中心偏移值
							IN size_t count);
typedef void (*DefMemgainS2D)(OUT unsigned char *pucDes,		// 目标
							  IN intptr_t nDesPitch,			// 目标一行占用的字节数
							  IN const unsigned char *pucSrc,	// 源
							  IN intptr_t nSrcPitch,			// 源一行占用的字节数
							  IN double dGain,					// 要求取值范围为 [0, 1]
							  IN unsigned char ucOffset,		// 中心偏移值
							  IN size_t nCount,					// 一行处理的字节数，一般要小于缓存的行字节数
							  IN int nLines,					// 行数
							  IN bool bOvertrun);				// 翻转标志
// 按有符号字节求双字中各字节 [0, 1] 的增益，字节的实际值等于存储值偏移 offset
typedef void (*DefDMemgainSB)(OUT unsigned long *des,
							  IN const unsigned long *src,
							  IN double dGain,				// 要求取值范围为 [0, 1]
							  IN unsigned long ulOffset,	// 中心偏移值（四个字节独立）
							  IN size_t count);				// 双字的个数
typedef void (*DefDMemgainSB2D)(OUT unsigned long *pulDes,			// 目标
								IN intptr_t nDesPitch,				// 目标一行占用的字节数
								IN const unsigned long *pulSrc,		// 源
								IN intptr_t nSrcPitch,				// 源一行占用的字节数
								IN double dGain,					// 要求取值范围为 [0, 1]
								IN unsigned long ulOffset,			// 中心偏移值（四个字节独立）
								IN size_t nCount,					// 一行处理的双字数
								IN int nLines,						// 行数
								IN bool bOvertrun);					// 翻转标志
// 按有符号字节求 24 位中各字节 [0, 1] 的增益，字节的实际值等于存储值偏移 offset
typedef void (*Def24BMemgainSB)(OUT unsigned char *des,
								IN const unsigned char *src,
								IN double dGain,				// 要求取值范围为 [0, 1]
								IN unsigned long ulOffset,		// 中心偏移值（四个字节独立）
								IN size_t count);				// 24 位单元的个数
typedef void (*Def24BMemgainSB2D)(OUT unsigned char *pucDes,		// 目标
								  IN intptr_t nDesPitch,			// 目标一行占用的字节数
								  IN const unsigned char *pucSrc,	// 源
								  IN intptr_t nSrcPitch,			// 源一行占用的字节数
								  IN double dGain,					// 要求取值范围为 [0, 1]
								  IN unsigned long ulOffset,		// 中心偏移值（四个字节独立）
								  IN size_t nCount,					// 一行处理的 24 位单元数
								  IN int nLines,					// 行数
								  IN bool bOvertrun);				// 翻转标志
// 按无符号字节求加权混叠（des = src * dWeight + const * (1 - dWeight))）
typedef void (*DefMemmixC)(OUT void *des,
						   IN const void *src,
						   IN unsigned char ucConst,	// 常量
						   IN double dWeight,			// 要求取值范围为 [0, 1]
						   IN size_t count);
typedef void (*DefMemmixC2D)(OUT unsigned char *pucDes,			// 目标
							 IN intptr_t nDesPitch,				// 目标一行占用的字节数
							 IN const unsigned char *pucSrc,	// 源
							 IN intptr_t nSrcPitch,				// 源一行占用的字节数
							 IN unsigned char ucConst,			// 常量
							 IN double dWeight,					// 源权重，要求取值范围为 [0, 1]
							 IN size_t nCount,					// 一行混叠的字节数，一般要小于缓存的行字节数
							 IN int nLines,						// 行数
							 IN bool bOvertrun);				// 翻转标志
// 按无符号字节求双字中各字节的加权混叠（des = src * dWeight + const * (1 - dWeight))）
typedef void (*DefDMemmixCB)(OUT unsigned long *des,
							 IN const unsigned long *src,
							 IN unsigned long ulConst,		// 常量
							 IN double dWeight,				// 要求取值范围为 [0, 1]
							 IN size_t count);				// 双字的个数
typedef void (*DefDMemmixCB2D)(OUT unsigned long *pulDes,		// 目标
							   IN intptr_t nDesPitch,			// 目标一行占用的字节数
							   IN const unsigned long *pulSrc,	// 源
							   IN intptr_t nSrcPitch,			// 源一行占用的字节数
							   IN unsigned long ulConst,		// 常量
							   IN double dWeight,				// 源权重，要求取值范围为 [0, 1]
							   IN size_t nCount,				// 一行处理的双字数
							   IN int nLines,					// 行数
							   IN bool bOvertrun);				// 翻转标志
// 按无符号字节求 24 位单元中各字节的加权混叠（des = src * dWeight + const * (1 - dWeight))）
typedef void (*Def24BMemmixCB)(OUT unsigned char *des,
							   IN const unsigned char *src,
							   IN unsigned long ulConst,		// 常量
							   IN double dWeight,				// 要求取值范围为 [0, 1]
							   IN size_t count);				// 24 单元的个数
typedef void (*Def24BMemmixCB2D)(OUT unsigned char *pucDes,			// 目标
								 IN intptr_t nDesPitch,				// 目标一行占用的字节数
								 IN const unsigned char *pucSrc,	// 源
								 IN intptr_t nSrcPitch,				// 源一行占用的字节数
								 IN unsigned long ulConst,			// 常量
								 IN double dWeight,					// 源权重，要求取值范围为 [0, 1]
								 IN size_t nCount,					// 一行处理的 24 位单元数
								 IN int nLines,						// 行数
								 IN bool bOvertrun);				// 翻转标志
// 浮点的增益（des = src * gain）
typedef void (*DefFMemgain)(OUT float *des, IN float *src, IN float fGain, IN size_t count);
typedef void (*DefDbMemgain)(OUT double *des, IN double *src, IN double dGain, IN size_t count);
// 浮点求差（des = srcL - srcR）
typedef void (*DefFMemsub)(OUT float *des, IN float *srcL, IN float *srcR, IN size_t count);
typedef void (*DefDbMemsub)(OUT double *des, IN double *srcL, IN double *srcR, IN size_t count);
// 浮点求和（des = srcL + srcR）
typedef void (*DefFMemadd)(OUT float *des, IN float *srcL, IN float *srcR, IN size_t count);
typedef void (*DefDbMemadd)(OUT double *des, IN double *srcL, IN double *srcR, IN size_t count);
// 无符号字节求平方和（支持跳点采样）
typedef double (*DefMemsqsum)(IN unsigned char *src, IN int stride, IN int count);
// 浮点符号取反（des = -src）
typedef void (*pfnmp_fnegtive)(__out float *des, __in float *src, __in size_t count);
typedef void (*pfnmp_dnegtive)(__out double *des, __in double *src, __in size_t count);
// 浮点平方和
typedef float (*pfnmp_sumsquare_f)(__in float const* src, __in int count);
typedef double (*pfnmp_sumsquare_d)(__in double const* src, __in int count);
// 浮点数求绝对值最大值
typedef float (*pfnmp_absmax_f)(__in float const* src, __in int count);
typedef double (*pfnmp_absmax_d)(__in double const* src, __in int count);
// 峰值信噪比
typedef double (*pfnmp_psnr)(__in unsigned char* src0, __in unsigned char* src1, __in int count);
typedef double (*pfnmp_psnr2d)(__in unsigned char* src0,
							   __in intptr_t src0_pitch,
							   __in unsigned char* src1,
							   __in intptr_t src1_pitch,
							   __in int count,
							   __in int lines);
// 浮点求积（des = src1 * src2)
typedef void (*pfnmp_memmul_f)(__out float* des, __in float* src1, __in float* src2, __in int count);
// des = src * src
typedef void (*pfnmp_mempow2_f)(__out float* des, __in float* src, __in int count);
// Structural Similarity
typedef float (*pfnmp_ssim)(__out float* ssim_map,			// 可为 NULL
							__in intptr_t ssim_map_pitch,
							__in unsigned char* img1,		// 图像 1
							__in intptr_t img1_pitch,
							__in unsigned char* img2,		// 图像 2
							__in intptr_t img2_pitch,
							__in int img_width,				// 图像幅面
							__in int img_height,
							__in double K1,					// 推荐 0.01
							__in double K2,					// 推荐 0.03
							__in int L,						// 推荐 255
							__in int downsamplewidth,		// 推荐 256，如果直接处理则传 1
							__in int gaussian_window,		// 推荐 11
							__in double gaussian_sigma);	// 推荐 1.5

//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxmemmathapi		// 线性运算
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
	DefMemgain		pfnMemgain;			// 无符号字节求解
	DefMemgain2D	pfnMemgain2D;
	DefMemgainS			pfnMemgainS;		// 有符号字节求解
	DefMemgainS2D		pfnMemgainS2D;
	DefDMemgainSB		pfnDMemgainSB;
	DefDMemgainSB2D		pfnDMemgainSB2D;
	Def24BMemgainSB		pfn24BMemgainSB;
	Def24BMemgainSB2D	pfn24BMemgainSB2D;
	DefMemmixC			pfnMemmixC;		// 无符号字节与常量混叠
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
	pfnmp_memmix		memmix;			// 线性混叠
	pfnmp_memmix2d		memmix2d;
	pfnmp_memavg		memavg;			// 计算平均值
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
	// 新版本接口从最后开始声明
} NXMemMathAPI, *LPNXMEMMATHAPI;


//////////////////////////////////////////////////////////////////////////
// 数据类型转换接口定义

// unsigned char -> float 格式转换，无数值变化
typedef void (*DefUChar2Float)(OUT float *des, IN unsigned char *src, IN size_t count);
// int -> float 格式转换，无数值变化
typedef void (*DefInt2Float)(OUT float *des, IN int *src, IN size_t count);
// float <-> double 格式转换，无数值变化
typedef void (*DefDouble2Float)(OUT float *des, IN const double *src, IN size_t count);
typedef void (*DefFloat2Double)(OUT double *des, IN const float *src, IN size_t count);
// char -> short/s24/int 量化精度转换
typedef void (*DefCharMap2Short)(OUT short *des, IN const char *src, IN size_t count);
typedef void (*DefCharMap2S24)(OUT unsigned char *des, IN const char *src, IN size_t count);
typedef void (*DefCharMap2Int)(OUT int *des, IN const char *src, IN size_t count);
// short -> s24/int 量化精度转换
typedef void (*DefShortMap2S24)(OUT unsigned char *des, IN const short *src, IN size_t count);
typedef void (*DefShortMap2Int)(OUT int *des, IN const short *src, IN size_t count);
// s24 -> int 量化精度转换
typedef void (*DefS24Map2Int)(OUT int *des, IN const unsigned char *src, IN size_t count);
// char/short/s24/int -> float/double 归一化
typedef void (*DefCharNorm2Float)(OUT float *des, IN const char *src, IN size_t count);
typedef void (*DefShortNorm2Float)(OUT float *des, IN const short *src, IN size_t count);
typedef void (*DefS24Norm2Float)(OUT float *des, IN const unsigned char *src, IN size_t count);
typedef void (*DefIntNorm2Float)(OUT float *des, IN const int *src, IN size_t count);
typedef void (*DefCharNorm2Double)(OUT double *des, IN const char *src, IN size_t count);
typedef void (*DefShortNorm2Double)(OUT double *des, IN const short *src, IN size_t count);
typedef void (*DefS24Norm2Double)(OUT double *des, IN const unsigned char *src, IN size_t count);
typedef void (*DefIntNorm2Double)(OUT double *des, IN const int *src, IN size_t count);
// 归一化 float/double -> char/short/s24/int 的量化还原
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
	DefCharMap2Short	pfnCharMap2Short;	// 量化深度改变
	DefCharMap2S24		pfnCharMap2S24;
	DefCharMap2Int		pfnCharMap2Int;
	DefShortMap2S24		pfnShortMap2S24;
	DefShortMap2Int		pfnShortMap2Int;
	DefS24Map2Int		pfnS24Map2Int;
	DefCharNorm2Float		pfnCharNorm2Float;	// 归一化
	DefShortNorm2Float		pfnShortNorm2Float;
	DefS24Norm2Float		pfnS24Norm2Float;
	DefIntNorm2Float		pfnIntNorm2Float;
	DefCharNorm2Double		pfnCharNorm2Double;
	DefShortNorm2Double		pfnShortNorm2Double;
	DefS24Norm2Double		pfnS24Norm2Double;
	DefIntNorm2Double		pfnIntNorm2Double;
	DefNormFloat2Char		pfnNormFloat2Char;	// 量化还原
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
	// 新版本接口从这里开始
} NXMemConvAPI, *LPNXMEMCONVAPI;


//////////////////////////////////////////////////////////////////////////
// 其它接口定义

// 无符号内存比较
typedef int (*DefMemcmp)(IN const void *buf1, IN const void *buf2, IN size_t count);

typedef struct tag_nxmemmiscapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	DefMemcmp			pfnMemcmp;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXMemMiscAPI, *LPNXMEMMISCAPI;


//////////////////////////////////////////////////////////////////////////
// 接口结构定义

typedef struct tag_nxmeminnerapi *LPNXMEMINNERAPI;

typedef struct tag_nxmemprocapi
{
	// 提取部分内容接口
	LPNXMEMPICKAPI		pStuMemPickAPI;
	// 重新排列内容接口
	LPNXMEMREORDERAPI	pStuMemReorderAPI;
	// 填充接口
	LPNXMEMFILLAPI		pStuMemFillAPI;
	// 线性运算接口
	LPNXMEMMATHAPI		pStuMemMathAPI;
	// 类型转换接口
	LPNXMEMCONVAPI		pStuMemConvAPI;
	// 其它接口
	LPNXMEMMISCAPI		pStuMemMiscAPI;
	// Private 权限，用于优化库内部的接口
	LPNXMEMINNERAPI		pStuInnerAPI;
} NXMemProcAPI, *LPNXMEMPROCAPI;


#endif		// #ifndef __NX_MEM_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
