//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 颜色空间转换.
//
// Created: ZhaoZhimeng @ 2010-05-17
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_COLOR_SPACE_DEF_H__
#define __NX_COLOR_SPACE_DEF_H__

#include ".\NXBasic.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#include <crtdefs.h>
#else	// VC6
#include <stddef.h>		// for intptr_t
#ifndef _INTPTR_T_DEFINED
typedef ptrdiff_t		intptr_t;
#define _INTPTR_T_DEFINED
#endif
#endif		// #if defined(_MSC_VER) && (_MSC_VER >= 1300)

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

#define NXCOLORSPACE_DLL_NAME			_T("NXColorSpace")
#define NXCOLORSPACE_API_EXT_NAME		"NCS_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// 视频信号的清晰度定义
enum NXVIDEO_DEFINITION
{
	NXVIDEO_DEFINITION_HIGH,				// 高清
	NXVIDEO_DEFINITION_STANDARD,			// 标清
	NXVIDEO_DEFINITION_ULTRA_HIGH = 3,		// 超高清（非恒定亮度，当使用与 SDTV 和 HDTV 环境相同的操作是工作重点时）
	NXVIDEO_DEFINITION_UNKNOWN,
};

// 常用分辨率定义
#define NXVIDEO_WIDTH_8K			7680
#define NXVIDEO_WIDTH_4K			3840
#define NXVIDEO_WIDTH_HD			1920
#define NXVIDEO_WIDTH_HDV			1440
#define NXVIDEO_WIDTH_SD			720
#define NXVIDEO_HEIGHT_8K			4320
#define NXVIDEO_HEIGHT_4K			2160
#define NXVIDEO_HEIGHT_HD			1080
#define NXVIDEO_HEIGHT_SD			540
#define NXVIDEO_HEIGHT_SD_PAL		576
#define NXVIDEO_HEIGHT_SD_NTSC		486
#define NXVIDEO_HEIGHT_SD_NTSC_E	480


//////////////////////////////////////////////////////////////////////////
// 视频信号的像素格式
enum NXVIDEO_PIXEL_FMT
{
	// 32 位像素
	NXVIDEO_PIXEL_FMT_BGRA,		// A R G B（B 在低位）
	NXVIDEO_PIXEL_FMT_RGBA,		// A B G R（R 在低位）
	NXVIDEO_PIXEL_FMT_YUVA,		// A V U Y（Y 在低位）
	// 24 位像素
	NXVIDEO_PIXEL_FMT_BGR,		// R G B（B 在低位）
	NXVIDEO_PIXEL_FMT_RGB,		// B G R（R 在低位）
	NXVIDEO_PIXEL_FMT_YUV,		// V U Y（Y 在低位）
	// 16 位像素
	NXVIDEO_PIXEL_FMT_YUYV,		// V0 Y1 U0 Y0（Y0 在低位）
	NXVIDEO_PIXEL_FMT_UYVY,		// Y1 V0 Y0 U0（U0 在低位）
	// 原始解码数据，像素各通道分片存储，Y 一片、U 一片、V 一片
	NXVIDEO_PIXEL_FMT_YUV422,	// 行内相邻的两个像素共用
	NXVIDEO_PIXEL_FMT_YUV420,	// 场内相邻的水平垂直方向四个像素共用
	// 8 位像素
	NXVIDEO_PIXEL_FMT_KEY,		// 键值
	// V1.0.8.9 注释中均指通道的按流存储顺序
	NXVIDEO_PIXEL_FMT_ARGB = NXVIDEO_PIXEL_FMT_KEY + 2,	// A R G B *** (for AE)
	NXVIDEO_PIXEL_FMT_ABGR,		// A B G R ***
	NXVIDEO_PIXEL_FMT_YUV444,	// 8bit Y、U、V 分片数据 4：4：4
	// 8bit 只含 UV 的各采样率分片数据，即 U 一片、V 一片
	NXVIDEO_PIXEL_FMT_UV44,
	NXVIDEO_PIXEL_FMT_UV22,
	NXVIDEO_PIXEL_FMT_UV20,
	// 8bit YUV 4:1:1 分片数据
	NXVIDEO_PIXEL_FMT_YUV411,
	NXVIDEO_PIXEL_FMT_NV12,			// 存储顺序为 Y 一片，UV 打包（U 在低位）
	NXVIDEO_PIXEL_FMT_YV12,			// 分片存储，Y 一片，V 一片，U 一片
	NXVIDEO_PIXEL_FMT_UV11,			// YUV 4:1:1 分片存储，U 一片，V 一片
	NXVIDEO_PIXEL_FMT_YUV420_P,		// 逐行帧内水平垂直方向的二次采样
	NXVIDEO_PIXEL_FMT_UV20_P,
	//////////////////////////////////////////////////////////////////////////
	// YUYV 10bit 8-2 型基带数据（Y 在低位，320bit 为一个 block，含 16 个像素，
	// 其中前 256bit 存储各通道高 8 位数据，后 64bit 存储各通道低 2 位数据）
	NXVIDEO_PIXEL_FMT_YUYV10_8_2 = NXVIDEO_PIXEL_FMT_ABGR + 1000,
	// 10bit 像素格式定义，使用 16bit 的高 10 位存储
	NXVIDEO_PIXEL_FMT_YUYV10,		// Y0 U0 Y1 V0 ***
	NXVIDEO_PIXEL_FMT_UYVY10,		// U0 Y0 V0 Y1 ***
	NXVIDEO_PIXEL_FMT_YUVA10,		// Y U V A ***
	NXVIDEO_PIXEL_FMT_RGBA10,		// R G B A ***
	NXVIDEO_PIXEL_FMT_BGRA10,		// B G R A ***
	NXVIDEO_PIXEL_FMT_ARGB10,		// A R G B ***
	NXVIDEO_PIXEL_FMT_ABGR10,		// A B G R ***
	NXVIDEO_PIXEL_FMT_RGB10,		// R G B ***
	NXVIDEO_PIXEL_FMT_BGR10,		// B G R ***
	NXVIDEO_PIXEL_FMT_YUV10,		// Y U V ***
	NXVIDEO_PIXEL_FMT_YUV422P10,	// Y、U、V 分片数据 4：2：2
	NXVIDEO_PIXEL_FMT_YUV420P10,	// Y、U、V 分片数据 4：2：0
	NXVIDEO_PIXEL_FMT_10BIT_LEAST,	// 使用 16bit 的低 10bit
	NXVIDEO_PIXEL_FMT_10BIT_MOST,	// 使用 16bit 的高 10bit
	NXVIDEO_PIXEL_FMT_YUV444P10,	// 10bit Y、U、V 分片数据 4：4：4
	// 10bit 只含 UV 的各采样率分片数据，即 U 一片、V 一片
	NXVIDEO_PIXEL_FMT_UV44_10,
	NXVIDEO_PIXEL_FMT_UV22_10,
	NXVIDEO_PIXEL_FMT_UV20_10,
	// U Y V Y *** 10bit 流式存储，使用 32bit 的低 30bit，高 2bit 置 0（即小端）
	NXVIDEO_PIXEL_FMT_V210_LE,
	NXVIDEO_PIXEL_FMT_KEY10,		// 键值
	NXVIDEO_PIXEL_FMT_UV11_10,		// YUV 4:1:1 分片存储，U 一片，V 一片
	NXVIDEO_PIXEL_FMT_YUV420P10_P,	// 逐行帧内水平垂直方向的二次采样
	NXVIDEO_PIXEL_FMT_UV20_10_P,
	//////////////////////////////////////////////////////////////////////////
	// 新版本定义从这里开始
	NXVIDEO_PIXEL_FMT_UNKNOWN = 0x7fffffff
};

// 颜色空间包装定义
// 打包数据：使用 color 存储缓冲
// 分片数据：按 color、cb、cr 的顺序存储缓冲
struct color_wrapper
{
	unsigned char*		color;			// 颜色通道
	intptr_t			color_pitch;	// 颜色通道一行的字节数
	unsigned char*		cb;				// U 分片数据
	intptr_t			cb_pitch;		// U 分片数据一行的字节数
	unsigned char*		cr;				// V 分片数据
	intptr_t			cr_pitch;		// V 分片数据一行的字节数
	unsigned char*		key;			// A 通道，部分像素格式未用（如不含 A 或者无分离的 A 通道）
	intptr_t			key_pitch;		// A 通道一行的字节数
	NXVIDEO_PIXEL_FMT	pixel_fmt;		// 像素格式
};

//////////////////////////////////////////////////////////////////////////
// 导出接口通用定义

// 24/32 位 RGB/BGR 颜色交换 R、B 通道
typedef void (*DefRGB_SwapRB_Frame)(OUT unsigned char *pucRGBOutFrame,	// 输出缓冲（存储方式 X2 G X0）
									IN intptr_t nOutPitch,				// 输出缓冲一行所占用的字节数
									IN unsigned char *pucRGBInFrame,	// 输入缓冲（存储方式 X0 G X2）
									IN intptr_t nInPitch,				// 输入缓冲一行所占用的字节数
									IN int nWidth,						// 源图像素矩阵大小
									IN int nHeight,
									IN bool bOverturn);					// 翻转图像标志
typedef void (*DefRGB_SwapRB_Split)(OUT unsigned char *pucRGBOutTopField,		// 输出顶场（存储方式 X2 G X0）
									IN intptr_t nOutTopPitch,					// 输出顶场一行所占用的字节数
									OUT unsigned char *pucRGBOutBottomField,	// 输出底场（存储方式 X2 G X0）
									IN intptr_t nOutBottomPitch,				// 输出底场一行所占用的字节数
									IN unsigned char *pucRGBInFrame,			// 输入缓冲（存储方式 X0 G X2）
									IN intptr_t nInPitch,						// 输入缓冲一行所占用的字节数
									IN int nWidth,								// 源图像素矩阵大小
									IN int nHeight,
									IN bool bOverturn);							// 翻转图像标志
typedef void (*DefRGB_SwapRB_Compose)(OUT unsigned char *pucRGBOutFrame,		// 输出缓冲（存储方式 X2 G X0）
									  IN intptr_t nOutPitch,					// 输出缓冲一行所占用的字节数
									  IN unsigned char *pucRGBInTopField,		// 输入顶场（存储方式 X0 G X2）
									  IN intptr_t nInTopPitch,					// 输入顶场一行所占用的字节数
									  IN unsigned char *pucRGBInBottomField,	// 输入底场（存储方式 X0 G X2）
									  IN intptr_t nInBottomPitch,				// 输入底场一行所占用的字节数
									  IN int nWidth,							// 源图像素矩阵大小
									  IN int nHeight,
									  IN bool bOverturn);						// 翻转图像标志

// 24/32 位 BGR/RGB 颜色到 24 位 YUV 颜色的变换，如果有 A 则直接丢弃
// 32 位 BGRA/RGBA 颜色到 32 位 YUVA 颜色的变换
typedef void (*DefRGB_2_YUV_Frame)(OUT unsigned char *pucYUVFrame,		// 输出缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
								   IN intptr_t nYUVPitch,				// 输出缓存一行所占用的字节数
								   IN unsigned char *pucRGBFrame,		// 输入缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
								   IN intptr_t nRGBPitch,				// 输入缓存一行所占用的字节数
								   IN int nWidth,						// 源图像素矩阵大小
								   IN int nHeight,
								   IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
								   IN bool bBGR,						// 输入缓冲的像素格式为 R G B（B在低位）
								   IN bool bOverturn);					// 翻转图像标志
typedef void (*DefRGB_2_YUV_Split)(OUT unsigned char *pucYUVTopField,		// 输出顶场（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
								   IN intptr_t nYUVTopPitch,				// 输出顶场一行所占用的字节数
								   OUT unsigned char *pucYUVBottomField,	// 输出底场（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
								   IN intptr_t nYUVBottomPitch,				// 输出底场一行所占用的字节数
								   IN unsigned char *pucRGBFrame,			// 输入帧（存储方式 X0 X1 X2 . X0 X1 X2 ...）
								   IN intptr_t nRGBPitch,					// 输入帧一行所占用的字节数
								   IN int nWidth,							// 源图像素矩阵大小
								   IN int nHeight,
								   IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
								   IN bool bBGR,							// 输入帧的像素格式为 R G B（B在低位）
								   IN bool bOverturn);						// 翻转图像标志
typedef void (*DefRGB_2_YUV_Compose)(OUT unsigned char *pucYUVFrame,		// 输出帧（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
									 IN intptr_t nYUVAPitch,				// 输出帧一行所占用的字节数
									 IN unsigned char *pucRGBTopField,		// 输入顶场（存储方式 X0 X1 X2 . X0 X1 X2 ...）
									 IN intptr_t nRGBTopPitch,				// 输入顶场一行所占用的字节数
									 IN unsigned char *pucRGBBottomField,	// 输入底场（存储方式 X0 X1 X2 . X0 X1 X2 ...）
									 IN intptr_t nRGB24BottomPitch,			// 输入底场一行所占用的字节数
									 IN int nWidth,							// 源图像素矩阵大小
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
									 IN bool bBGR,							// 输入缓冲的像素格式为 R G B（B在低位）
									 IN bool bOverturn);					// 翻转图像标志

// 24/32 位 BGR/RGB 颜色到 16 位 YUYV/UYVY 颜色的变换，如果有 A 直接丢弃，并直接丢弃第二个 UV
typedef void (*DefRGB_2_YUV16_Frame)(OUT unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									 IN intptr_t nYUV16Pitch,			// 16 位缓存一行所占用的字节数
									 IN unsigned char *pucRGBFrame,		// 输入缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
									 IN intptr_t nRGBPitch,				// 输入缓存一行所占用的字节数
									 IN int nWidth,						// 源图像素矩阵大小
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
									 IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									 IN bool bBGR,						// 输入缓冲的像素格式为 R G B（B在低位）
									 IN bool bOverturn);				// 翻转图像标志
typedef void (*DefRGB_2_YUV16_Split)(OUT unsigned char *pucYUV16TopField,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									 IN intptr_t nYUV16TopPitch,				// 16 位缓存一行所占用的字节数
									 OUT unsigned char *pucYUV16BottomField,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									 IN intptr_t nYUV16BottomPitch,				// 16 位缓存一行所占用的字节数
									 IN unsigned char *pucRGBFrame,				// 输入缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
									 IN intptr_t nRGBPitch,						// 输入缓存一行所占用的字节数
									 IN int nWidth,								// 源图像素矩阵大小
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,			// 清晰度
									 IN bool bUYVY,								// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									 IN bool bBGR,								// 输入缓冲的像素格式为 R G B（B在低位）
									 IN bool bOverturn);						// 翻转图像标志
typedef void (*DefRGB_2_YUV16_Compose)(OUT unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									   IN intptr_t nYUV16Pitch,				// 16 位缓存一行所占用的字节数
									   IN unsigned char *pucRGBTopField,	// 输入顶场（存储方式 X0 X1 X2 . X0 X1 X2 ...）
									   IN intptr_t nRGB32TopPitch,			// 输入顶场一行所占用的字节数
									   IN unsigned char *pucRGBBottomField,	// 输入底场（存储方式 X0 X1 X2 . X0 X1 X2 ...）
									   IN intptr_t nRGBBottomPitch,			// 输入底场一行所占用的字节数
									   IN int nWidth,						// 源图像素矩阵大小
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
									   IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									   IN bool bBGR,						// 输入缓冲的像素格式为 R G B（B在低位）
									   IN bool bOverturn);					// 翻转图像标志

// 24/32 位 BGR/RGB 颜色到分片 YUV422/YUV420 的变换，如果有 A 则直接丢弃，并直接丢弃第二个 UV，YUV420 时 UV 由两行插值出一行
typedef void (*DefRGB_2_YUVSlice_Frame)(OUT unsigned char *pucYFrame,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										IN intptr_t nYPitch,				// 分片 Y 缓冲一行所占用的字节数
										OUT unsigned char *pucUUFrame,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										IN intptr_t nUUPitch,				// 分片 U 缓冲一行所占用的字节数
										OUT unsigned char *pucVVFrame,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										IN intptr_t nVVPitch,				// 分片 V 缓冲一行所占用的字节数
										IN unsigned char *pucRGBFrame,		// 输入缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
										IN intptr_t nRGBPitch,				// 输入缓存一行所占用的字节数
										IN int nWidth,						// 源图像素矩阵大小
										IN int nHeight,
										IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
										IN bool bBGR,						// 输入缓冲的像素格式为 R G B（B在低位）的标志
										IN bool bOverturn);					// 翻转图像标志
typedef void (*DefRGB_2_YUVSlice_Split)(OUT unsigned char *pucYTopField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										IN intptr_t nYTopPitch,					// 分片 Y 缓冲一行所占用的字节数
										OUT unsigned char *pucYBottomField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										IN intptr_t nYBottomPitch,				// 分片 Y 缓冲一行所占用的字节数
										OUT unsigned char *pucUUTopField,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										IN intptr_t nUUTopPitch,				// 分片 U 缓冲一行所占用的字节数
										OUT unsigned char *pucUUBottomField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										IN intptr_t nUUBottomPitch,				// 分片 U 缓冲一行所占用的字节数
										OUT unsigned char *pucVVTopField,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										IN intptr_t nVVTopPitch,					// 分片 V 缓冲一行所占用的字节数
										OUT unsigned char *pucVVBottomField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										IN intptr_t nVVBottomPitch,				// 分片 V 缓冲一行所占用的字节数
										IN unsigned char *pucRGBFrame,			// 输入帧（存储方式 X0 X1 X2 . X0 X1 X2 ...）
										IN intptr_t nRGBPitch,					// 输入帧一行所占用的字节数
										IN int nWidth,							// 源图像素矩阵大小
										IN int nHeight,
										IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
										IN bool bBGR,							// 输入缓冲的像素格式为 R G B（B在低位）的标志
										IN bool bOverturn);						// 翻转图像标志
typedef void (*DefRGB_2_YUVSlice_Compose)(OUT unsigned char *pucYFrame,			// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYPitch,					// 分片 Y 缓冲一行所占用的字节数
										  OUT unsigned char *pucUUFrame,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUPitch,					// 分片 U 缓冲一行所占用的字节数
										  OUT unsigned char *pucVVFrame,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVPitch,					// 分片 V 缓冲一行所占用的字节数
										  IN unsigned char *pucRGBTopField,		// 输入顶场（存储方式 X0 X1 X2 . X0 X1 X2 ...）
										  IN intptr_t nRGBTopPitch,				// 输入顶场一行所占用的字节数
										  IN unsigned char *pucRGBBottomField,	// 输入底场（存储方式 X0 X1 X2 . X0 X1 X2 ...）
										  IN intptr_t nRGBBottomPitch,			// 输入底场一行所占用的字节数
										  IN int nWidth,						// 源图像素矩阵大小
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
										  IN bool bBGRA,						// 输入缓冲的像素格式为 R G B（B在低位）的标志
										  IN bool bOverturn);					// 翻转图像标志

// 32 位 YUVA 颜色到 32 位 BGRA/RGBA 颜色的变换
// 24/32 位 YUV/YUVA 颜色到 24 位 BGR/RGB 颜色的变换，如果有 A 直接丢弃
typedef void (*DefYUV_2_RGB_Frame)(OUT unsigned char *pucRGBFrame,		// 输出缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
								   IN intptr_t nRGBPitch,				// 输出缓存一行所占用的字节数
								   IN unsigned char *pucYUVFrame,		// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
								   IN intptr_t nYUVAPitch,				// 输入缓存一行所占用的字节数
								   IN int nWidth,						// 源图像素矩阵大小
								   IN int nHeight,
								   IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
								   IN bool bBGR,						// 输出缓冲的像素格式为 R G B（B在低位）的标志
								   IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUV_2_RGB_Split)(OUT unsigned char *pucRGBTopField,		// 输出顶场（存储方式 X0 X1 X2 . X0 X1 X2 ...）
								   IN intptr_t nRGBTopPitch,				// 输出顶场一行所占用的字节数
								   OUT unsigned char *pucRGBBottomField,	// 输出底场（存储方式 X0 X1 X2 . X0 X1 X2 ...）
								   IN intptr_t nRGBBottomPitch,				// 输出底场一行所占用的字节数
								   IN unsigned char *pucYUVAFrame,			// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
								   IN intptr_t nYUVAPitch,					// 输入缓存一行所占用的字节数
								   IN int nWidth,							// 源图像素矩阵大小
								   IN int nHeight,
								   IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
								   IN bool bBGR,							// 输出缓冲的像素格式为 R G B（B在低位）
								   IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUV_2_RGB_Compose)(OUT unsigned char *pucRGBFrame,		// 输出缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
									 IN intptr_t nRGBPitch,					// 输出缓存一行所占用的字节数
									 IN unsigned char *pucYUVATopField,		// 输入顶场（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
									 IN intptr_t nYUVATopPitch,				// 输入顶场一行所占用的字节数
									 IN unsigned char *pucYUVABottomField,	// 输入底场（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
									 IN intptr_t nYUVABottomPitch,			// 输入底场一行所占用的字节数
									 IN int nWidth,							// 源图像素矩阵大小
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
									 IN bool bBGR,							// 输出缓冲的像素格式为 R G B（B在低位）
									 IN bool bOverturn);					// 翻转图像标志

// 24/32 位 YUV/YUVA 颜色到 16 位 YUYV/UYVY 颜色的变换，直接丢弃第二个 UV，如果有 A 则直接丢弃
typedef void (*DefYUV_2_YUV16_Frame)(OUT unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									 IN intptr_t nYUV16Pitch,			// 16 位缓存一行所占用的字节数
									 IN unsigned char *pucYUVFrame,		// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
									 IN intptr_t nYUVPitch,				// 缓存一行所占用的字节数
									 IN int nWidth,						// 源图像素矩阵大小
									 IN int nHeight,
									 IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									 IN bool bOverturn);				// 翻转图像标志
typedef void (*DefYUV_2_YUV16_Split)(OUT unsigned char *pucYUV16TopField,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									 IN intptr_t nYUV16TopPitch,				// 16 位缓存一行所占用的字节数
									 OUT unsigned char *pucYUV16BottomField,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									 IN intptr_t nYUV16BottomPitch,				// 16 位缓存一行所占用的字节数
									 IN unsigned char *pucYUVFrame,				// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
									 IN intptr_t nYUVPitch,						// 输入缓存一行所占用的字节数
									 IN int nWidth,								// 源图像素矩阵大小
									 IN int nHeight,
									 IN bool bUYVY,								// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									 IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUV_2_YUV16_Compose)(OUT unsigned char *pucYUV16Frame,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									   IN intptr_t nYUV16Pitch,					// 16 位缓存一行所占用的字节数
									   IN unsigned char *pucYUVTopField,		// 输入顶场（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
									   IN intptr_t nYUVTopPitch,				// 输入顶场一行所占用的字节数
									   IN unsigned char *pucYUVBottomField,		// 输入底场（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
									   IN intptr_t nYUVBottomPitch,				// 输入底场一行所占用的字节数
									   IN int nWidth,							// 源图像素矩阵大小
									   IN int nHeight,
									   IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									   IN bool bOverturn);						// 翻转图像标志

// 分片 YUV422/YUV420(8Bit/10Bit) 到 16 位 YUYV/UYVY 颜色的变换，第二行 UV 值采用上下行 UV 进行插值
typedef void (*DefYUVSlice_2_YUV16_Frame)(OUT unsigned char *pucYUV16Frame,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										  IN intptr_t nYUV16Pitch,				// 16 位缓冲一行所占用的字节数
										  IN unsigned char *pucYFrame,			// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYPitch,					// 分片 Y 缓冲一行所占用的字节数
										  IN unsigned char *pucUUFrame,			// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUPitch,					// 分片 U 缓冲一行所占用的字节数
										  IN unsigned char *pucVVFrame,			// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVPitch,					// 分片 V 缓冲一行所占用的字节数
										  IN int nWidth,						// 源图像素矩阵大小
										  IN int nHeight,
										  IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
										  IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUVSlice_2_YUV16_Split)(OUT unsigned char *pucYUV16TopField,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										  IN intptr_t nYUV16TopPitch,				// 16 位缓冲一行所占用的字节数
										  OUT unsigned char *pucYUV16BottomField,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										  IN intptr_t nYUV16BottomPitch,			// 16 位缓冲一行所占用的字节数
										  IN unsigned char *pucYFrame,				// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYPitch,						// 分片 Y 缓冲一行所占用的字节数
										  IN unsigned char *pucUUFrame,				// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUPitch,						// 分片 U 缓冲一行所占用的字节数
										  IN unsigned char *pucVVFrame,				// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVPitch,						// 分片 V 缓冲一行所占用的字节数
										  IN int nWidth,							// 源图像素矩阵大小
										  IN int nHeight,
										  IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
										  IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUVSlice_2_YUV16_Compose)(OUT unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
											IN intptr_t nYUV16Pitch,			// 16 位缓冲一行所占用的字节数
											IN unsigned char *pucYTopField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
											IN intptr_t nYTopPitch,				// 分片 Y 缓冲一行所占用的字节数
											IN unsigned char *pucYBottomField,	// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
											IN intptr_t nYBottomPitch,			// 分片 Y 缓冲一行所占用的字节数
											IN unsigned char *pucUUTopField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUTopPitch,			// 分片 U 缓冲一行所占用的字节数
											IN unsigned char *pucUUBottomField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUBottomPitch,			// 分片 U 缓冲一行所占用的字节数
											IN unsigned char *pucVVTopField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVTopPitch,			// 分片 V 缓冲一行所占用的字节数
											IN unsigned char *pucVVBottomField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVBottomPitch,			// 分片 V 缓冲一行所占用的字节数
											IN int nWidth,						// 源图像素矩阵大小
											IN int nHeight,
											IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
											IN bool bOverturn);					// 翻转图像标志

// 分片 YUV422/YUV420 到 32 位 YUVA 颜色的变换，A 可填充为统一值，第二个 UV 值采用前后 UV 进行插值，第二行 UV 值采用上下行 UV 进行插值
typedef void (*DefYUVSlice_2_YUVA_Frame)(OUT unsigned char *pucYUVAFrame,	// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
										 IN intptr_t nYUVAPitch,			// 32 位缓冲一行所占用的字节数
										 IN unsigned char *pucYFrame,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										 IN intptr_t nYPitch,				// 分片 Y 缓冲一行所占用的字节数
										 IN unsigned char *pucUUFrame,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										 IN intptr_t nUUPitch,				// 分片 U 缓冲一行所占用的字节数
										 IN unsigned char *pucVVFrame,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										 IN intptr_t nVVPitch,				// 分片 V 缓冲一行所占用的字节数
										 IN int nWidth,						// 源图像素矩阵大小
										 IN int nHeight,
										 IN unsigned char cAlpha,			// 填充的 A 值
										 IN bool bOverturn);				// 翻转图像标志
typedef void (*DefYUVSlice_2_YUVA_Split)(OUT unsigned char *pucYUVATopField,		// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
										 IN intptr_t nYUVATopPitch,					// 32 位缓冲一行所占用的字节数
										 OUT unsigned char *pucYUVABottomField,		// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
										 IN intptr_t nYUVABottomPitch,				// 32 位缓冲一行所占用的字节数
										 IN unsigned char *pucYFrame,				// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										 IN intptr_t nYPitch,						// 分片 Y 缓冲一行所占用的字节数
										 IN unsigned char *pucUUFrame,				// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										 IN intptr_t nUUPitch,						// 分片 U 缓冲一行所占用的字节数
										 IN unsigned char *pucVVFrame,				// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										 IN intptr_t nVVPitch,						// 分片 V 缓冲一行所占用的字节数
										 IN int nWidth,								// 源图像素矩阵大小
										 IN int nHeight,
										 IN unsigned char cAlpha,					// 填充的 A 值
										 IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUVSlice_2_YUVA_Compose)(OUT unsigned char *pucYUVAFrame,		// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
										   IN intptr_t nYUVAPitch,				// 32 位缓冲一行所占用的字节数
										   IN unsigned char *pucYTopField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										   IN intptr_t nYTopPitch,				// 分片 Y 缓冲一行所占用的字节数
										   IN unsigned char *pucYBottomField,	// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										   IN intptr_t nYBottomPitch,			// 分片 Y 缓冲一行所占用的字节数
										   IN unsigned char *pucUUTopField,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										   IN intptr_t nUUTopPitch,				// 分片 U 缓冲一行所占用的字节数
										   IN unsigned char *pucUUBottomField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										   IN intptr_t nUUBottomPitch,			// 分片 U 缓冲一行所占用的字节数
										   IN unsigned char *pucVVTopField,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										   IN intptr_t nVVTopPitch,				// 分片 V 缓冲一行所占用的字节数
										   IN unsigned char *pucVVBottomField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										   IN intptr_t nVVBottomPitch,			// 分片 V 缓冲一行所占用的字节数
										   IN int nWidth,						// 源图像素矩阵大小
										   IN int nHeight,
										   IN unsigned char cAlpha,				// 填充的 A 值
										   IN bool bOverturn);					// 翻转图像标志

// 分片 YUV422/YUV420 到 24 位 YUV 颜色的变换，第二个 UV 值采用前后 UV 进行插值，第二行 UV 值采用上下行 UV 进行插值
typedef void (*DefYUVSlice_2_YUV_Frame)(OUT unsigned char *pucYUVFrame,		// 24 位缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
										IN intptr_t nYUVPitch,				// 24 位缓冲一行所占用的字节数
										IN unsigned char *pucYFrame,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										IN intptr_t nYPitch,				// 分片 Y 缓冲一行所占用的字节数
										IN unsigned char *pucUUFrame,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										IN intptr_t nUUPitch,				// 分片 U 缓冲一行所占用的字节数
										IN unsigned char *pucVVFrame,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										IN intptr_t nVVPitch,				// 分片 V 缓冲一行所占用的字节数
										IN int nWidth,						// 源图像素矩阵大小
										IN int nHeight,
										IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUVSlice_2_YUV_Split)(OUT unsigned char *pucYUVTopField,		// 24 位缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
										IN intptr_t nYUVTopPitch,				// 24 位缓冲一行所占用的字节数
										OUT unsigned char *pucYUVBottomField,	// 24 位缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
										IN intptr_t nYUVBottomPitch,			// 24 位缓冲一行所占用的字节数
										IN unsigned char *pucYFrame,			// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										IN intptr_t nYPitch,					// 分片 Y 缓冲一行所占用的字节数
										IN unsigned char *pucUUFrame,			// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										IN intptr_t nUUPitch,					// 分片 U 缓冲一行所占用的字节数
										IN unsigned char *pucVVFrame,			// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										IN intptr_t nVVPitch,					// 分片 V 缓冲一行所占用的字节数
										IN int nWidth,							// 源图像素矩阵大小
										IN int nHeight,
										IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUVSlice_2_YUV_Compose)(OUT unsigned char *pucYUVFrame,		// 24 位缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
										  IN intptr_t nYUVPitch,				// 24 位缓冲一行所占用的字节数
										  IN unsigned char *pucYTopField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYTopPitch,				// 分片 Y 缓冲一行所占用的字节数
										  IN unsigned char *pucYBottomField,	// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYBottomPitch,			// 分片 Y 缓冲一行所占用的字节数
										  IN unsigned char *pucUUTopField,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUTopPitch,				// 分片 U 缓冲一行所占用的字节数
										  IN unsigned char *pucUUBottomField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUBottomPitch,			// 分片 U 缓冲一行所占用的字节数
										  IN unsigned char *pucVVTopField,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVTopPitch,				// 分片 V 缓冲一行所占用的字节数
										  IN unsigned char *pucVVBottomField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVBottomPitch,			// 分片 V 缓冲一行所占用的字节数
										  IN int nWidth,						// 源图像素矩阵大小
										  IN int nHeight,
										  IN bool bOverturn);					// 翻转图像标志

// 分片 YUV422/YUV420 之间的 UV 变换，由两行插值出一行
typedef void (*DefYUVSlice_ConvertUV_Frame)(OUT unsigned char *pucUUOutFrame,	// 分片 U 输出缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUOutPitch,			// 分片 U 输出缓冲一行所占用的字节数
											OUT unsigned char *pucVVOutFrame,	// 分片 V 输出缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVOutPitch,			// 分片 V 输出缓冲一行所占用的字节数
											IN unsigned char *pucUUInFrame,		// 分片 U 输入缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUInPitch,				// 分片 U 输入缓冲一行所占用的字节数
											OUT unsigned char *pucVVInFrame,	// 分片 V 输入缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVInPitch,				// 分片 V 输入缓冲一行所占用的字节数
											IN int nWidth,						// 源图像素矩阵大小
											IN int nHeight,
											IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUVSlice_ConvertUV_Split)(OUT unsigned char *pucUUOutTopField,	// 分片 U 输出缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUOutTopPitch,				// 分片 U 输出缓冲一行所占用的字节数
											OUT unsigned char *pucUUOutBottomField,	// 分片 U 输出缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUOutBottomPitch,			// 分片 U 输出缓冲一行所占用的字节数
											OUT unsigned char *pucVVOutTopField,	// 分片 V 输出缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVOutTopPitch,				// 分片 V 输出缓冲一行所占用的字节数
											OUT unsigned char *pucVVOutBottomField,	// 分片 V 输出缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVOutBottomPitch,			// 分片 V 输出缓冲一行所占用的字节数
											IN unsigned char *pucUUInFrame,			// 分片 U 输入缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUInPitch,					// 分片 U 输入缓冲一行所占用的字节数
											OUT unsigned char *pucVVInFrame,		// 分片 V 输入缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVInPitch,					// 分片 V 输入缓冲一行所占用的字节数
											IN int nWidth,							// 源图像素矩阵大小
											IN int nHeight,
											IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUVSlice_ConvertUV_Compose)(OUT unsigned char *pucUUOutFrame,			// 分片 U 输出缓冲（存储方式 UU0 UU2 UU4 ……）
											  IN intptr_t nUUOutPitch,					// 分片 U 输出缓冲一行所占用的字节数
											  OUT unsigned char *pucVVOutFrame,			// 分片 V 输出缓冲（存储方式 VV0 VV2 VV4 ……）
											  IN intptr_t nVVOutPitch,					// 分片 V 输出缓冲一行所占用的字节数
											  IN unsigned char *pucUUInTopField,		// 分片 U 输入缓冲（存储方式 UU0 UU2 UU4 ……）
											  IN intptr_t nUUInTopPitch,				// 分片 U 输入缓冲一行所占用的字节数
											  IN unsigned char *pucUUInBottomField,		// 分片 U 输入缓冲（存储方式 UU0 UU2 UU4 ……）
											  IN intptr_t nUUInBottomPitch,				// 分片 U 输入缓冲一行所占用的字节数
											  IN unsigned char *pucVVInTopField,		// 分片 V 输入缓冲（存储方式 VV0 VV2 VV4 ……）
											  IN intptr_t nVVInTopPitch,				// 分片 V 输入缓冲一行所占用的字节数
											  IN unsigned char *pucVVInBottomField,		// 分片 V 输入缓冲（存储方式 VV0 VV2 VV4 ……）
											  IN intptr_t nVVInBottomPitch,				// 分片 V 输入缓冲一行所占用的字节数
											  IN int nWidth,							// 源图像素矩阵大小
											  IN int nHeight,
											  IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUVSlice_ConvertUV_Field)(OUT unsigned char *pucUUOutField,	// 分片 U 输出缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUOutPitch,			// 分片 U 输出缓冲一行所占用的字节数
											OUT unsigned char *pucVVOutField,	// 分片 V 输出缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVOutPitch,			// 分片 V 输出缓冲一行所占用的字节数
											IN unsigned char *pucUUInField,		// 分片 U 输入缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUInPitch,				// 分片 U 输入缓冲一行所占用的字节数
											OUT unsigned char *pucVVInField,	// 分片 V 输入缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVInPitch,				// 分片 V 输入缓冲一行所占用的字节数
											IN int nWidth,						// 源图场像素矩阵大小
											IN int nFieldHeight,
											IN bool bTopField,					// 顶场变换标志（顶场与底场的变换不同）
											IN bool bOverturn);					// 翻转图像标志

// 分片 YUV422/YUV420 到 32 位 BGRA/RGBA 颜色的变换，A 可填充为统一值，第二个 UV 值采用前后 UV 进行插值，第二行 UV 值采用上下行 UV 进行插值
typedef void (*DefYUVSlice_2_RGB32_Frame)(OUT unsigned char *pucRGB32Frame,		// 32 位输出缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
										  IN intptr_t nRGB32Pitch,				// 32 位输出缓存一行所占用的字节数
										  IN unsigned char *pucYFrame,			// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYPitch,					// 分片 Y 缓冲一行所占用的字节数
										  IN unsigned char *pucUUFrame,			// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUPitch,					// 分片 U 缓冲一行所占用的字节数
										  IN unsigned char *pucVVFrame,			// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVPitch,					// 分片 V 缓冲一行所占用的字节数
										  IN int nWidth,						// 源图像素矩阵大小
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
										  IN bool bBGRA,						// 32 位输出缓冲的像素格式为 A R G B（B在低位）的标志
										  IN unsigned char cAlpha,				// 填充的 A 值
										  IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUVSlice_2_RGB32_Split)(OUT unsigned char *pucRGB32TopField,		// 32 位输出缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
										  IN intptr_t nRGB32TopPitch,				// 32 位输出缓存一行所占用的字节数
										  OUT unsigned char *pucRGB32BottomField,	// 32 位输出缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
										  IN intptr_t nRGB32BottomPitch,			// 32 位输出缓存一行所占用的字节数
										  IN unsigned char *pucYFrame,				// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYPitch,						// 分片 Y 缓冲一行所占用的字节数
										  IN unsigned char *pucUUFrame,				// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUPitch,						// 分片 U 缓冲一行所占用的字节数
										  IN unsigned char *pucVVFrame,				// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVPitch,						// 分片 V 缓冲一行所占用的字节数
										  IN int nWidth,							// 源图像素矩阵大小
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
										  IN bool bBGRA,							// 32 位输出缓冲的像素格式为 A R G B（B在低位）的标志
										  IN unsigned char cAlpha,					// 填充的 A 值
										  IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUVSlice_2_RGB32_Compose)(OUT unsigned char *pucRGB32Frame,	// 32 位输出缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
											IN intptr_t nRGB32Pitch,			// 32 位输出缓存一行所占用的字节数
											IN unsigned char *pucYTopField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
											IN intptr_t nYTopPitch,				// 分片 Y 缓冲一行所占用的字节数
											IN unsigned char *pucYBottomField,	// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
											IN intptr_t nYBottomPitch,			// 分片 Y 缓冲一行所占用的字节数
											IN unsigned char *pucUUTopField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUTopPitch,			// 分片 U 缓冲一行所占用的字节数
											IN unsigned char *pucUUBottomField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUBottomPitch,			// 分片 U 缓冲一行所占用的字节数
											IN unsigned char *pucVVTopField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVTopPitch,			// 分片 V 缓冲一行所占用的字节数
											IN unsigned char *pucVVBottomField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVBottomPitch,			// 分片 V 缓冲一行所占用的字节数
											IN int nWidth,						// 源图像素矩阵大小
											IN int nHeight,
											IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
											IN bool bBGRA,						// 32 位输出缓冲的像素格式为 A R G B（B在低位）的标志
											IN unsigned char cAlpha,			// 填充的 A 值
											IN bool bOverturn);					// 翻转图像标志

// 分片 YUV422/YUV420 到 24 位 BGR/RGB 颜色的变换，第二个 UV 值采用前后 UV 进行插值，第二行 UV 值采用上下行 UV 进行插值
typedef void (*DefYUVSlice_2_RGB24_Frame)(OUT unsigned char *pucRGB24Frame,		// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
										  IN intptr_t nRGB24Pitch,				// 24 位输出缓存一行所占用的字节数
										  IN unsigned char *pucYFrame,			// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYPitch,					// 分片 Y 缓冲一行所占用的字节数
										  IN unsigned char *pucUUFrame,			// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUPitch,					// 分片 U 缓冲一行所占用的字节数
										  IN unsigned char *pucVVFrame,			// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVPitch,					// 分片 V 缓冲一行所占用的字节数
										  IN int nWidth,						// 源图像素矩阵大小
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
										  IN bool bBGR,							// 24 位输出缓冲的像素格式为 R G B（B在低位）的标志
										  IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUVSlice_2_RGB24_Split)(OUT unsigned char *pucRGB24TopField,		// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
										  IN intptr_t nRGB24TopPitch,				// 24 位输出缓存一行所占用的字节数
										  OUT unsigned char *pucRGB24BottomField,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
										  IN intptr_t nRGB24BottomPitch,			// 24 位输出缓存一行所占用的字节数
										  IN unsigned char *pucYFrame,				// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYPitch,						// 分片 Y 缓冲一行所占用的字节数
										  IN unsigned char *pucUUFrame,				// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUPitch,						// 分片 U 缓冲一行所占用的字节数
										  IN unsigned char *pucVVFrame,				// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVPitch,						// 分片 V 缓冲一行所占用的字节数
										  IN int nWidth,							// 源图像素矩阵大小
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
										  IN bool bBGR,								// 24 位输出缓冲的像素格式为 R G B（B在低位）的标志
										  IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUVSlice_2_RGB24_Compose)(OUT unsigned char *pucRGB24Frame,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
											IN intptr_t nRGB24Pitch,			// 24 位输出缓存一行所占用的字节数
											IN unsigned char *pucYTopField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
											IN intptr_t nYTopPitch,				// 分片 Y 缓冲一行所占用的字节数
											IN unsigned char *pucYBottomField,	// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
											IN intptr_t nYBottomPitch,			// 分片 Y 缓冲一行所占用的字节数
											IN unsigned char *pucUUTopField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUTopPitch,			// 分片 U 缓冲一行所占用的字节数
											IN unsigned char *pucUUBottomField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUBottomPitch,			// 分片 U 缓冲一行所占用的字节数
											IN unsigned char *pucVVTopField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVTopPitch,			// 分片 V 缓冲一行所占用的字节数
											IN unsigned char *pucVVBottomField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVBottomPitch,			// 分片 V 缓冲一行所占用的字节数
											IN int nWidth,						// 源图像素矩阵大小
											IN int nHeight,
											IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
											IN bool bBGR,						// 24 位输出缓冲的像素格式为 R G B（B在低位）的标志
											IN bool bOverturn);					// 翻转图像标志

// 24/32 位 YUV/YUVA 颜色到分片 YUV422/YUV420 的变换，直接丢弃第二个 UV，YUV420 时 UV 由两行插值出一行，如果有 A 则直接丢弃
typedef void (*DefYUV_2_YUVSlice_Frame)(OUT unsigned char *pucYFrame,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										IN intptr_t nYPitch,				// 分片 Y 缓冲一行所占用的字节数
										OUT unsigned char *pucUUFrame,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										IN intptr_t nUUPitch,				// 分片 U 缓冲一行所占用的字节数
										OUT unsigned char *pucVVFrame,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										IN intptr_t nVVPitch,				// 分片 V 缓冲一行所占用的字节数
										IN unsigned char *pucYUVFrame,		// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
										IN intptr_t nYUVPitch,				// 输入缓冲一行所占用的字节数
										IN int nWidth,						// 源图像素矩阵大小
										IN int nHeight,
										IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUV_2_YUVSlice_Split)(OUT unsigned char *pucYTopField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										IN intptr_t nYTopPitch,					// 分片 Y 缓冲一行所占用的字节数
										OUT unsigned char *pucYBottomField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										IN intptr_t nYBottomPitch,				// 分片 Y 缓冲一行所占用的字节数
										OUT unsigned char *pucUUTopField,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										IN intptr_t nUUTopPitch,				// 分片 U 缓冲一行所占用的字节数
										OUT unsigned char *pucUUBottomField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										IN intptr_t nUUBottomPitch,				// 分片 U 缓冲一行所占用的字节数
										OUT unsigned char *pucVVTopField,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										IN intptr_t nVVTopPitch,				// 分片 V 缓冲一行所占用的字节数
										OUT unsigned char *pucVVBottomField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										IN intptr_t nVVBottomPitch,				// 分片 V 缓冲一行所占用的字节数
										IN unsigned char *pucYUVFrame,			// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
										IN intptr_t nYUVPitch,					// 输入缓冲一行所占用的字节数
										IN int nWidth,							// 源图像素矩阵大小
										IN int nHeight,
										IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUV_2_YUVSlice_Compose)(OUT unsigned char *pucYFrame,				// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYPitch,						// 分片 Y 缓冲一行所占用的字节数
										  OUT unsigned char *pucUUFrame,			// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUPitch,						// 分片 U 缓冲一行所占用的字节数
										  OUT unsigned char *pucVVFrame,			// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVPitch,						// 分片 V 缓冲一行所占用的字节数
										  IN unsigned char *pucYUVTopField,			// 输入顶场（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
										  IN intptr_t nYUVTopPitch,					// 输入顶场一行所占用的字节数
										  IN unsigned char *pucYUVBottomField,		// 输入底场（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
										  IN intptr_t nYUVBottomPitch,				// 输入底场一行所占用的字节数
										  IN int nWidth,							// 源图像素矩阵大小
										  IN int nHeight,
										  IN bool bOverturn);						// 翻转图像标志


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 接口结构定义

//////////////////////////////////////////////////////////////////////////
// 通用接口

// 统一接口
// （1）实现常用像素格式之间的颜色变换，分为整帧变换、拆场变换、合场变换三种。
// 当出现场缓冲时，要求 nHeight 为偶数，否则会产生行丢失。
// （2）对于 NXVIDEO_PIXEL_FMT_YUV422、NXVIDEO_PIXEL_FMT_YUV420 两种原始解码数据
// 格式，严格要求其存储方式。对于 NXVIDEO_PIXEL_FMT_KEY 格式，一般只能用于目标，
// 表示取源图中的 KEY 通道，如果源图没有 KEY 通道，则填充为 0xff。
// （3）清晰度 eDefinition 参数在涉及 YUV 颜色空间变换时有效，YUV 颜色空间区分高标清。
// （4）返回 false 表示变换失败，在输入参数有效的情况下，说明格式不支持。
// （5）新增的像素格式定义基本符合上述规则。
typedef bool (*DefColorConvFrame)(OUT unsigned char *pucDesFrame,		// 目标帧
								  IN intptr_t nDesPitch,				// 目标帧一行所占用的字节数
								  IN NXVIDEO_PIXEL_FMT eDesPixelFmt,	// 目标像素格式
								  IN unsigned char *pucSrcFrame,		// 源帧
								  IN intptr_t nSrcPitch,				// 源帧一行所占用的字节数
								  IN NXVIDEO_PIXEL_FMT eSrcPixelFmt,	// 源像素格式
								  IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
								  IN int nWidth,						// 像素矩阵大小
								  IN int nHeight,
								  IN bool bOverturn);					// 翻转图像标志
typedef bool (*DefColorConvSplit)(OUT unsigned char *pucDesTopField,	// 目标顶场
								  IN intptr_t nDesTopPitch,				// 目标顶场一行所占用的字节数
								  OUT unsigned char *pucDesBottomField,	// 目标底场
								  IN intptr_t nDesBottomPitch,			// 目标底场一行所占用的字节数
								  IN NXVIDEO_PIXEL_FMT eDesPixelFmt,	// 目标像素格式
								  IN unsigned char *pucSrcFrame,		// 源帧
								  IN intptr_t nSrcPitch,				// 源帧一行所占用的字节数
								  IN NXVIDEO_PIXEL_FMT eSrcPixelFmt,	// 源像素格式
								  IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
								  IN int nWidth,						// 像素矩阵大小
								  IN int nHeight,
								  IN bool bOverturn);					// 翻转图像标志
typedef bool (*DefColorConvCompose)(OUT unsigned char *pucDesFrame,			// 目标帧
									IN intptr_t nDesPitch,					// 目标帧一行所占用的字节数
									IN NXVIDEO_PIXEL_FMT eDesPixelFmt,		// 目标像素格式
									IN unsigned char *pucSrcTopField,		// 源顶场
									IN intptr_t nSrcTopPitch,				// 源顶场一行所占用的字节数
									IN unsigned char *pucSrcBottomField,	// 源底场
									IN intptr_t nSrcNottomPitch,			// 源底场一行所占用的字节数
									IN NXVIDEO_PIXEL_FMT eSrcPixelFmt,		// 源像素格式
									IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
									IN int nWidth,							// 像素矩阵大小
									IN int nHeight,
									IN bool bOverturn);						// 翻转图像标志
// 32 位 BGRA/RGBA/YUVA 颜色中的通道分解
typedef void (*DefCr32_Split_Channel_Frame)(OUT unsigned char *pucOutFrame,		// 通道输出缓冲
											IN intptr_t nOutPitch,				// 输出缓冲一行所占用的字节数
											IN unsigned char *pucCr32Frame,		// 32 位缓冲（存储方式 X0 X1 X2 A0 X0 X1 X2 A1 ……）
											IN intptr_t nCr32Pitch,				// 32 位缓冲一行所占用的字节数
											IN int nWidth,						// 源图像素矩阵大小
											IN int nHeight,
											IN int nChannel,					// 需要分解的通道索引，取值为 (0 ~ 3)
											IN bool bOverturn);					// 翻转图像标志
typedef void (*DefCr32_Split_Channel_Split)(OUT unsigned char *pucOutTopField,		// 通道输出缓冲
											IN intptr_t nOutTopPitch,				// 输出缓冲一行所占用的字节数
											OUT unsigned char *pucOutBottomField,	// 通道输出缓冲
											IN intptr_t nOutBottomPitch,			// 输出缓冲一行所占用的字节数
											IN unsigned char *pucCr32Frame,			// 32 位缓冲（存储方式 X0 X1 X2 A0 X0 X1 X2 A1 ……）
											IN intptr_t nCr32Pitch,					// 32 位缓冲一行所占用的字节数
											IN int nWidth,							// 源图像素矩阵大小
											IN int nHeight,
											IN int nChannel,						// 需要分解的通道索引，取值为 (0 ~ 3)
											IN bool bOverturn);						// 翻转图像标志
typedef void (*DefCr32_Split_Channel_Compose)(OUT unsigned char *pucOutFrame,		// 通道输出缓冲
											  IN intptr_t nOutPitch,				// 输出缓冲一行所占用的字节数
											  IN unsigned char *pucCr32TopField,	// 32 位缓冲（存储方式 X0 X1 X2 A0 X0 X1 X2 A1 ……）
											  IN intptr_t nCr32TopPitch,			// 32 位缓冲一行所占用的字节数
											  IN unsigned char *pucCr32BottomField,	// 32 位缓冲（存储方式 X0 X1 X2 A0 X0 X1 X2 A1 ……）
											  IN intptr_t nCr32BottomPitch,			// 32 位缓冲一行所占用的字节数
											  IN int nWidth,						// 源图像素矩阵大小
											  IN int nHeight,
											  IN int nChannel,						// 需要分解的通道索引，取值为 (0 ~ 3)
											  IN bool bOverturn);					// 翻转图像标志

// 24 位 RGB/BGR/YUV 颜色到 32 位 RGBA/BGRA/YUVA 颜色的变换，A 可填充为统一值，同时支持 R、B 两个通道的互换
typedef void (*DefCr24_2_Cr32_Frame)(OUT unsigned char *pucCr32Frame,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									 IN intptr_t nCr32Pitch,			// 32 位缓存一行所占用的字节数
									 IN unsigned char *pucCr24Frame,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									 IN intptr_t nCr24Pitch,			// 24 位缓存一行所占用的字节数
									 IN int nWidth,						// 源图像素矩阵大小
									 IN int nHeight,
									 IN bool bExchangeRB,				// 交换 0、2 字节（R、B 通道）的标志
									 IN unsigned char ucAlpha,			// 填充的 A 值
									 IN bool bOverturn);				// 翻转图像标志
typedef void (*DefCr24_2_Cr32_Split)(OUT unsigned char *pucCr32TopField,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									 IN intptr_t nCr32TopPitch,				// 32 位缓存一行所占用的字节数
									 OUT unsigned char *pucCr32BottomField,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									 IN intptr_t nCr32BottomPitch,			// 32 位缓存一行所占用的字节数
									 IN unsigned char *pucCr24Frame,		// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									 IN intptr_t nCr24Pitch,				// 24 位缓存一行所占用的字节数
									 IN int nWidth,							// 源图像素矩阵大小
									 IN int nHeight,
									 IN bool bExchangeRB,					// 交换 0、2 字节（R、B 通道）的标志
									 IN unsigned char ucAlpha,				// 填充的 A 值
									 IN bool bOverturn);					// 翻转图像标志
typedef void (*DefCr24_2_Cr32_Compose)(OUT unsigned char *pucCr32Frame,			// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									   IN intptr_t nCr32Pitch,					// 32 位缓存一行所占用的字节数
									   IN unsigned char *pucCr24TopField,		// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									   IN intptr_t nCr24TopPitch,				// 24 位缓存一行所占用的字节数
									   IN unsigned char *pucCr24BottomField,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									   IN intptr_t nCr24BottomPitch,			// 24 位缓存一行所占用的字节数
									   IN int nWidth,							// 源图像素矩阵大小
									   IN int nHeight,
									   IN bool bExchangeRB,						// 交换 0、2 字节（R、B 通道）的标志
									   IN unsigned char ucAlpha,				// 填充的 A 值
									   IN bool bOverturn);						// 翻转图像标志
// 32 位 RGBA/BGRA/YUVA 颜色到 24 位 RGB/BGR/YUV 颜色的变换，直接丢弃 A，同时支持 R、B 两个通道的互换
typedef void (*DefCr32_2_Cr24_Frame)(OUT unsigned char *pucCr24Frame,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									 IN intptr_t nCr24Pitch,			// 24 位缓存一行所占用的字节数
									 IN unsigned char *pucCr32Frame,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									 IN intptr_t nCr32Pitch,			// 32 位缓存一行所占用的字节数
									 IN int nWidth,						// 源图像素矩阵大小
									 IN int nHeight,
									 IN bool bExchangeRB,				// 交换 0、2 字节（R、B 通道）的标志
									 IN bool bOverturn);				// 翻转图像标志
typedef void (*DefCr32_2_Cr24_Split)(OUT unsigned char *pucCr24TopField,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									 IN intptr_t nCr24TopPitch,				// 24 位缓存一行所占用的字节数
									 OUT unsigned char *pucCr24BottomField,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									 IN intptr_t nCr24BottomPitch,			// 24 位缓存一行所占用的字节数
									 IN unsigned char *pucCr32Frame,		// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									 IN intptr_t nCr32Pitch,				// 32 位缓存一行所占用的字节数
									 IN int nWidth,							// 源图像素矩阵大小
									 IN int nHeight,
									 IN bool bExchangeRB,					// 交换 0、2 字节（R、B 通道）的标志
									 IN bool bOverturn);					// 翻转图像标志
typedef void (*DefCr32_2_Cr24_Compose)(OUT unsigned char *pucCr24Frame,			// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									   IN intptr_t nCr24Pitch,					// 24 位缓存一行所占用的字节数
									   IN unsigned char *pucCr32TopField,		// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									   IN intptr_t nCr32TopPitch,				// 32 位缓存一行所占用的字节数
									   IN unsigned char *pucCr32BottomField,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									   IN intptr_t nCr32BottomPitch,			// 32 位缓存一行所占用的字节数
									   IN int nWidth,							// 源图像素矩阵大小
									   IN int nHeight,
									   IN bool bExchangeRB,						// 交换 0、2 字节（R、B 通道）的标志
									   IN bool bOverturn);						// 翻转图像标志

// 帧信号的拆场与合场
// （1）如果 nHeight 为奇数时，会丢失一行。
// （2）图像翻转标志要正确使用，确保顶场与底场的数据与视频的制式稳合。
// （3）定义正向图像的标准为图像的左上角点与内存起点对齐。
typedef void (*DefSplitFrame)(OUT unsigned char *pucTopField,		// 顶场输出
							  IN intptr_t nTopPitch,				// 一行所占字节数
							  OUT unsigned char *pucBottomField,	// 底场输出
							  IN intptr_t nBottomPitch,				// 一行所占字节数
							  IN unsigned char *pucFrame,			// 源帧输入
							  IN intptr_t nFramePitch,				// 一行所占字节数
							  IN int nWidth,						// 源图像素矩阵大小
							  IN int nHeight,
							  IN int nPixelBytes,					// 一个像素所占字节数
							  IN bool bOverturn);					// 图像翻转标志
typedef void (*DefComposeFields)(OUT unsigned char *pucFrame,		// 目标帧输出
								 IN intptr_t nFramePitch,			// 一行所占字节数
								 IN unsigned char *pucTopField,		// 顶场输入
								 IN intptr_t nTopPitch,				// 一行所占字节数
								 IN unsigned char *pucBottomField,	// 底场输入
								 IN intptr_t nBottomPitch,			// 一行所占字节数
								 IN int nWidth,						// 源图像素矩阵大小
								 IN int nHeight,
								 IN int nPixelBytes,				// 一个像素所占字节数
								 IN bool bOverturn);				// 图像翻转标志

// 安全色转换
// （1）8bit 颜色 RGBY 按如下值域 [16, 235] ～ [0, 255]，10bit 颜色按未来定义扩展
// （2）8bit 颜色 UV 按如下值域 [16, 240] ~ [0, 255], 10bit 颜色按未来定义扩展
// （3）对于存在 Alpha 通道的像素，保持 Alpha 不变
// （4）源与目的可以相同
typedef void (*DefPackSafeRGB)(OUT unsigned char *pucDes,		// 目标
							   IN intptr_t nDesPitch,			// 目标一行所占用的字节数
							   IN const unsigned char *pucSrc,	// 源
							   IN intptr_t nSrcPitch,			// 源一行所占用的字节数
							   IN NXVIDEO_PIXEL_FMT ePixFmt,	// 像素格式
							   IN int nWidth,					// 像素矩阵大小
							   IN int nHeight,
							   IN bool bOverturn);				// 翻转图像标志
typedef DefPackSafeRGB		DefUnpackSafeRGB;

//////////////////////////////////////////////////////////////////////////
// 颜色转换通用接口，从任一格式到任一格式的颜色转换
// 主要为支持 10bit、12bit、16bit 的像素格式扩展
// 内部实现将根据源和目的的像素格式进行区分
typedef void (*pfncs_any_to_any_frame)(__out color_wrapper* des_frame,			// 目的帧
									   __in	color_wrapper* src_frame,			// 源帧
									   __in NXVIDEO_DEFINITION definition,		// 清晰度
									   __in int width,							// 帧宽
									   __in int height,							// 帧高
									   __in bool overturn);						// 翻转标志
typedef void (*pfncs_any_to_any_split)(__out color_wrapper* des_top_field,		// 目的顶场
									   __out color_wrapper* des_bottom_field,	// 目的底场
									   __in color_wrapper* src_frame,			// 源帧
									   __in NXVIDEO_DEFINITION definition,		// 清晰度
									   __in int width,							// 帧宽
									   __in int height,							// 帧高
									   __in bool overturn);						// 翻转标志
typedef void (*pfncs_any_to_any_compose)(__out color_wrapper* des_frame,		// 目的帧
										 __in color_wrapper* src_top_field,		// 源顶场
										 __in color_wrapper* src_bottom_field,	// 源底场
										 __in NXVIDEO_DEFINITION definition,	// 清晰度
										 __in int width,						// 帧宽
										 __in int height,						// 帧高
										 __in bool overturn);					// 翻转标志
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxcolorgenericapi
{
	// 统一接口
	DefColorConvFrame		pfnColorConvFrame;
	DefColorConvSplit		pfnColorConvSplit;
	DefColorConvCompose		pfnColorConvCompose;
	// RGBA/BGRA/YUVA -> Channel
	DefCr32_Split_Channel_Frame		pfnCr32_Split_Channel_Frame;
	DefCr32_Split_Channel_Split		pfnCr32_Split_Channel_Split;
	DefCr32_Split_Channel_Compose	pfnCr32_Split_Channel_Compose;
	// RGB/BGR/YUV <-> RGBA/BGRA/YUVA
	DefCr24_2_Cr32_Frame		pfnCr24_2_Cr32_Frame;
	DefCr24_2_Cr32_Split		pfnCr24_2_Cr32_Split;
	DefCr24_2_Cr32_Compose		pfnCr24_2_Cr32_Compose;
	DefCr32_2_Cr24_Frame		pfnCr32_2_Cr24_Frame;
	DefCr32_2_Cr24_Split		pfnCr32_2_Cr24_Split;
	DefCr32_2_Cr24_Compose		pfnCr32_2_Cr24_Compose;
	// 拆/合场
	DefSplitFrame			pfnSplitFrame;
	DefComposeFields		pfnComposeFields;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.6
	// 安全色转换
	DefPackSafeRGB			pfnPackSafeRGB;
	DefUnpackSafeRGB		pfnUnpackSafeRGB;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.9
	pfncs_any_to_any_frame		any_to_any_frame;
	pfncs_any_to_any_split		any_to_any_split;
	pfncs_any_to_any_compose	any_to_any_compose;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从此开始定义
} NXColorGenericAPI, *LPNXCOLORGENERICAPI;

//////////////////////////////////////////////////////////////////////////
// RGB32(RGBA/BGRA) => Others

// 32 位 RGBA/BGRA 颜色交换 R、B 通道
typedef DefRGB_SwapRB_Frame			DefRGB32_SwapRB_Frame;
typedef DefRGB_SwapRB_Split			DefRGB32_SwapRB_Split;
typedef DefRGB_SwapRB_Compose		DefRGB32_SwapRB_Compose;
// 32 位 BGRA/RGBA 颜色到 32 位 YUVA 颜色的变换
typedef DefRGB_2_YUV_Frame			DefRGB32_2_YUVA_Frame;
typedef DefRGB_2_YUV_Split			DefRGB32_2_YUVA_Split;
typedef DefRGB_2_YUV_Compose		DefRGB32_2_YUVA_Compose;
// 32 位 BGRA/RGBA 颜色到 16 位 YUYV/UYVY 颜色的变换，直接丢弃 A 与第二个 UV
typedef DefRGB_2_YUV16_Frame		DefRGB32_2_YUV16_Frame;
typedef DefRGB_2_YUV16_Split		DefRGB32_2_YUV16_Split;
typedef DefRGB_2_YUV16_Compose		DefRGB32_2_YUV16_Compose;
// 32 位 BGRA/RGBA 颜色到分片 YUV422/YUV420 的变换，直接丢弃 A 与第二个 UV，YUV420 时 UV 由两行插值出一行
typedef DefRGB_2_YUVSlice_Frame		DefRGB32_2_YUVSlice_Frame;
typedef DefRGB_2_YUVSlice_Split		DefRGB32_2_YUVSlice_Split;
typedef DefRGB_2_YUVSlice_Compose	DefRGB32_2_YUVSlice_Compose;
// 32 位 BGRA/RGBA 颜色到 24 位 YUV 颜色的变换，直接丢弃 A
typedef DefRGB_2_YUV_Frame			DefRGB32_2_YUV_Frame;
typedef DefRGB_2_YUV_Split			DefRGB32_2_YUV_Split;
typedef DefRGB_2_YUV_Compose		DefRGB32_2_YUV_Compose;
// RGBA/BGRA => Gray
typedef void (*pfncs_rgb32_to_gray_frame)(__out unsigned char* gray,			// 灰度缓冲
										  __in intptr_t gray_pitch,				// 灰度缓冲一行的字节数
										  __in unsigned char* rgb32,			// RGBA/BGRA 32 位颜色缓冲
										  __in intptr_t rgb32_pitch,			// RGBA/BGRA 32 位颜色缓冲一行的字节数
										  __in int width,						// 处理幅面
										  __in int height,
										  __in NXVIDEO_DEFINITION definition,	// 清晰度
										  __in bool isbgra,						// 是否为 32 位 BGRA
										  __in bool overturn);					// 是否翻转
typedef void (*pfncs_rgb32_to_gray_split)(__out unsigned char* gray_top,		// 灰度顶场缓冲
										  __in intptr_t gray_top_pitch,			// 灰度顶场缓冲一行的字节数
										  __out unsigned char* gray_bottom,		// 灰度底场缓冲
										  __in intptr_t gray_bottom_pitch,		// 灰度底场缓冲一行的字节数
										  __in unsigned char* rgb32,			// RGBA/BGRA 32 位颜色缓冲
										  __in intptr_t rgb32_pitch,			// RGBA/BGRA 32 位颜色缓冲一行的字节数
										  __in int width,						// 处理幅面
										  __in int height,
										  __in NXVIDEO_DEFINITION definition,	// 清晰度
										  __in bool isbgra,						// 是否为 32 位 BGRA
										  __in bool overturn);					// 是否翻转
typedef void (*pfncs_rgb32_to_gray_compose)(__out unsigned char* gray,			// 灰度缓冲
											__in intptr_t gray_pitch,			// 灰度缓冲一行的字节数
											__in unsigned char* rgb32_top,		// RGBA/BGRA 32 位颜色顶场缓冲
											__in intptr_t rgb32_top_pitch,		// RGBA/BGRA 32 位颜色顶场缓冲一行的字节数
											__in unsigned char* rgb32_bottom,	// RGBA/BGRA 32 位颜色底场缓冲
											__in intptr_t rgb32_bottom_pitch,	// RGBA/BGRA 32 位颜色底场缓冲一行的字节数
											__in int width,						// 处理幅面
											__in int height,
											__in NXVIDEO_DEFINITION definition,	// 清晰度
											__in bool isbgra,					// 是否为 32 位 BGRA
											__in bool overturn);				// 是否翻转
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxrgb32toothers
{
	DefRGB32_SwapRB_Frame		pfnRGB32_SwapRB_Frame;
	DefRGB32_SwapRB_Split		pfnRGB32_SwapRB_Split;
	DefRGB32_SwapRB_Compose		pfnRGB32_SwapRB_Compose;

	DefRGB32_2_YUVA_Frame		pfnRGB32_2_YUVA_Frame;
	DefRGB32_2_YUVA_Split		pfnRGB32_2_YUVA_Split;
	DefRGB32_2_YUVA_Compose		pfnRGB32_2_YUVA_Compose;

	DefRGB32_2_YUV16_Frame		pfnRGB32_2_YUV16_Frame;
	DefRGB32_2_YUV16_Split		pfnRGB32_2_YUV16_Split;
	DefRGB32_2_YUV16_Compose	pfnRGB32_2_YUV16_Compose;

	DefRGB32_2_YUVSlice_Frame	pfnRGB32_2_YUV422_Frame;
	DefRGB32_2_YUVSlice_Split	pfnRGB32_2_YUV422_Split;
	DefRGB32_2_YUVSlice_Compose	pfnRGB32_2_YUV422_Compose;

	DefRGB32_2_YUVSlice_Frame	pfnRGB32_2_YUV420_Frame;
	DefRGB32_2_YUVSlice_Split	pfnRGB32_2_YUV420_Split;
	DefRGB32_2_YUVSlice_Compose	pfnRGB32_2_YUV420_Compose;

	DefRGB32_2_YUV_Frame		pfnRGB32_2_YUV_Frame;
	DefRGB32_2_YUV_Split		pfnRGB32_2_YUV_Split;
	DefRGB32_2_YUV_Compose		pfnRGB32_2_YUV_Compose;
	// V1.1.0.5
	pfncs_rgb32_to_gray_frame	rgb32_to_gray_frame;
	pfncs_rgb32_to_gray_split	rgb32_to_gray_split;
	pfncs_rgb32_to_gray_compose	rgb32_to_gray_compose;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始定义
} NXRGB32toOthers, *LPNXRGB32TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// RGB24(RGB/BGR) => Others

// 24 位 RGB/BGR 颜色交换 R、B 通道
typedef DefRGB_SwapRB_Frame			DefRGB24_SwapRB_Frame;
typedef DefRGB_SwapRB_Split			DefRGB24_SwapRB_Split;
typedef DefRGB_SwapRB_Compose		DefRGB24_SwapRB_Compose;
// 24 位 BGR/RGB 颜色到 32 位 YUVA 颜色的变换，A 可填充为统一值
typedef void (*DefRGB24_2_YUVA_Frame)(OUT unsigned char *pucYUVAFrame,		// 32 位输出缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
									  IN intptr_t nYUVAPitch,				// 32 位输出缓存一行所占用的字节数
									  IN unsigned char *pucRGB24Frame,		// 24 位输入缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									  IN intptr_t nRGB24Pitch,				// 24 位输入缓存一行所占用的字节数
									  IN int nWidth,						// 源图像素矩阵大小
									  IN int nHeight,
									  IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
									  IN bool bBGR,							// 24 位输入缓冲的像素格式为 R G B（B在低位）
									  IN unsigned char cAlpha,				// 填充的 A 值
									  IN bool bOverturn);					// 翻转图像标志
typedef void (*DefRGB24_2_YUVA_Split)(OUT unsigned char *pucYUVATopField,		// 32 位输出缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
									  IN intptr_t nYUVATopPitch,				// 32 位输出缓存一行所占用的字节数
									  OUT unsigned char *pucYUVABottomField,	// 32 位输出缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
									  IN intptr_t nYUVABottomPitch,				// 32 位输出缓存一行所占用的字节数
									  IN unsigned char *pucRGB24Frame,			// 24 位输入缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									  IN intptr_t nRGB24Pitch,					// 24 位输入缓存一行所占用的字节数
									  IN int nWidth,							// 源图像素矩阵大小
									  IN int nHeight,
									  IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
									  IN bool bBGR,								// 24 位输入缓冲的像素格式为 R G B（B在低位）
									  IN unsigned char cAlpha,					// 填充的 A 值
									  IN bool bOverturn);						// 翻转图像标志
typedef void (*DefRGB24_2_YUVA_Compose)(OUT unsigned char *pucYUVAFrame,		// 32 位输出缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
										IN intptr_t nYUVAPitch,					// 32 位输出缓存一行所占用的字节数
										IN unsigned char *pucRGB24TopField,		// 24 位输入缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
										IN intptr_t nRGB24TopPitch,				// 24 位输入缓存一行所占用的字节数
										IN unsigned char *pucRGB24BottomField,	// 24 位输入缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
										IN intptr_t nRGB24BottomPitch,			// 24 位输入缓存一行所占用的字节数
										IN int nWidth,							// 源图像素矩阵大小
										IN int nHeight,
										IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
										IN bool bBGR,							// 24 位输入出缓冲的像素格式为 R G B（B在低位）
										IN unsigned char cAlpha,				// 填充的 A 值
										IN bool bOverturn);						// 翻转图像标志
// 24 位 BGR/RGB 颜色到 16 位 YUYV/UYVY 颜色的变换，直接丢弃第二个 UV
typedef DefRGB_2_YUV16_Frame		DefRGB24_2_YUV16_Frame;
typedef DefRGB_2_YUV16_Split		DefRGB24_2_YUV16_Split;
typedef DefRGB_2_YUV16_Compose		DefRGB24_2_YUV16_Compose;
// 24 位 BGR/RGB 颜色到分片 YUV422/YUV420 的变换，直接丢弃第二个 UV，YUV420 时 UV 由两行插值出一行
typedef DefRGB_2_YUVSlice_Frame		DefRGB24_2_YUVSlice_Frame;
typedef DefRGB_2_YUVSlice_Split		DefRGB24_2_YUVSlice_Split;
typedef DefRGB_2_YUVSlice_Compose	DefRGB24_2_YUVSlice_Compose;
// 24 位 BGR/RGB 颜色到 24 位 YUV 颜色的变换
typedef DefRGB_2_YUV_Frame			DefRGB24_2_YUV_Frame;
typedef DefRGB_2_YUV_Split			DefRGB24_2_YUV_Split;
typedef DefRGB_2_YUV_Compose		DefRGB24_2_YUV_Compose;
// 24位 BGR/RGB => HSV
typedef void (*pfncs_rgb24_to_hsv_frame)(__out short* hue,					// 色调
										 __in intptr_t hue_pitch,			// 色调一行的字节数
										 __out unsigned char* saturation,	// 饱和度
										 __in intptr_t saturation_pitch,	// 饱和度一行的字节数
										 __out unsigned char* value,		// 亮度
										 __in intptr_t value_pitch,			// 亮度一行的字节数
										 __in unsigned char* rgb24,			// 24 位颜色缓冲
										 __in intptr_t rgb24_pitch,			// 24 位颜色缓冲一行的字节数
										 __in int width,					// 幅面宽
										 __in int height,					// 幅面高
										 __in bool isbgr,					// 是否为 BGR
										 __in bool overturn);				// 是否翻转
typedef void (*pfncs_rgb24_to_hsv_split)(__out short* hue_top,					// 顶场色调
										 __in intptr_t hue_top_pitch,			// 顶场色调一行的字节数
										 __out short* hue_bottom,				// 底场色调
										 __in intptr_t hue_bottom_pitch,		// 底场色调一行的字节数
										 __out unsigned char* saturation_top,	// 顶场饱和度
										 __in intptr_t saturation_top_pitch,	// 顶场饱和度一行的字节数
										 __out unsigned char* saturation_bottom,// 底场饱和度
										 __in intptr_t saturation_bottom_pitch,	// 底场饱和度一行的字节数
										 __out unsigned char* value_top,		// 顶场亮度
										 __in intptr_t value_top_pitch,			// 顶场亮度一行的字节数
										 __out unsigned char* value_bottom,		// 底场亮度
										 __in intptr_t value_bottom_pitch,		// 底场亮度一行的字节数
										 __in unsigned char* rgb24,				// 24 位颜色缓冲
										 __in intptr_t rgb24_pitch,				// 24 位颜色缓冲一行的字节数
										 __in int width,						// 幅面宽
										 __in int height,						// 幅面高
										 __in bool isbgr,						// 是否为 BGR
										 __in bool overturn);					// 是否翻转
typedef void (*pfncs_rgb24_to_hsv_compose)(__out short* hue,				// 色调
										   __in intptr_t hue_pitch,			// 色调一行的字节数
										   __out unsigned char* saturation,	// 饱和度
										   __in intptr_t saturation_pitch,	// 饱和度一行的字节数
										   __out unsigned char* value,		// 亮度
										   __in intptr_t value_pitch,		// 亮度一行的字节数
										   __in unsigned char* rgb24_top,	// 顶场 24 位颜色缓冲
										   __in intptr_t rgb24_top_pitch,	// 顶场 24 位颜色缓冲一行的字节数
										   __in unsigned char* rgb24_bottom,// 底场 24 位颜色缓冲
										   __in intptr_t rgb24_bottom_pitch,// 底场 24 位颜色缓冲一行的字节数
										   __in int width,					// 幅面宽
										   __in int height,					// 幅面高
										   __in bool isbgr,					// 是否为 BGR
										   __in bool overturn);				// 是否翻转
// RGB => Gray
typedef void (*pfncs_rgb24_to_gray_frame)(__out unsigned char* gray,			// 灰度缓冲
										  __in intptr_t gray_pitch,				// 灰度缓冲一行的字节数
										  __in unsigned char* rgb24,			// RGB/BGR 24 位颜色缓冲
										  __in intptr_t rgb24_pitch,			// RGB/BGR 24 位颜色缓冲一行的字节数
										  __in int width,						// 处理幅面
										  __in int height,
										  __in NXVIDEO_DEFINITION definition,	// 清晰度
										  __in bool isbgr,						// 是否为 24 位 BGR
										  __in bool overturn);					// 是否翻转
typedef void (*pfncs_rgb24_to_gray_split)(__out unsigned char* gray_top,		// 灰度顶场缓冲
										  __in intptr_t gray_top_pitch,			// 灰度顶场缓冲一行的字节数
										  __out unsigned char* gray_bottom,		// 灰度底场缓冲
										  __in intptr_t gray_bottom_pitch,		// 灰度底场缓冲一行的字节数
										  __in unsigned char* rgb24,			// RGB/BGR 24 位颜色缓冲
										  __in intptr_t rgb24_pitch,			// RGB/BGR 24 位颜色缓冲一行的字节数
										  __in int width,						// 处理幅面
										  __in int height,
										  __in NXVIDEO_DEFINITION definition,	// 清晰度
										  __in bool isbgr,						// 是否为 24 位 BGR
										  __in bool overturn);					// 是否翻转
typedef void (*pfncs_rgb24_to_gray_compose)(__out unsigned char* gray,			// 灰度缓冲
											__in intptr_t gray_pitch,			// 灰度缓冲一行的字节数
											__in unsigned char* rgb24_top,		// RGB/BGR 24 位颜色顶场缓冲
											__in intptr_t rgb24_top_pitch,		// RGB/BGR 24 位颜色顶场缓冲一行的字节数
											__in unsigned char* rgb24_bottom,	// RGB/BGR 24 位颜色底场缓冲
											__in intptr_t rgb24_bottom_pitch,	// RGB/BGR 24 位颜色底场缓冲一行的字节数
											__in int width,						// 处理幅面
											__in int height,
											__in NXVIDEO_DEFINITION definition,	// 清晰度
											__in bool isbgr,					// 是否为 24 位 BGR
											__in bool overturn);				// 是否翻转
// RGB/BGR 24 位颜色直方图统计，桶数为 256
typedef void (*pfncs_rgb24_histogram)(__out int* rhist,				// 红色通道分布
									  __out int* ghist,				// 绿色通道分布
									  __out int* bhist,				// 蓝色通道分布
									  __in unsigned char* rgb24,	// RGB/BGR 24 位颜色缓冲
									  __in intptr_t rgb24_pitch,	// RGB/BGR 24 位颜色缓冲一行的字节数
									  __in int width,				// 处理幅面
									  __in int height,
									  __in bool isbgr);				// 是否为 24 位 BGR

//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxrgb24toothers
{
	DefRGB24_SwapRB_Frame		pfnRGB24_SwapRB_Frame;
	DefRGB24_SwapRB_Split		pfnRGB24_SwapRB_Split;
	DefRGB24_SwapRB_Compose		pfnRGB24_SwapRB_Compose;

	DefRGB24_2_YUVA_Frame		pfnRGB24_2_YUVA_Frame;
	DefRGB24_2_YUVA_Split		pfnRGB24_2_YUVA_Split;
	DefRGB24_2_YUVA_Compose		pfnRGB24_2_YUVA_Compose;

	DefRGB24_2_YUV16_Frame		pfnRGB24_2_YUV16_Frame;
	DefRGB24_2_YUV16_Split		pfnRGB24_2_YUV16_Split;
	DefRGB24_2_YUV16_Compose	pfnRGB24_2_YUV16_Compose;

	DefRGB24_2_YUVSlice_Frame	pfnRGB24_2_YUV422_Frame;
	DefRGB24_2_YUVSlice_Split	pfnRGB24_2_YUV422_Split;
	DefRGB24_2_YUVSlice_Compose	pfnRGB24_2_YUV422_Compose;

	DefRGB24_2_YUVSlice_Frame	pfnRGB24_2_YUV420_Frame;
	DefRGB24_2_YUVSlice_Split	pfnRGB24_2_YUV420_Split;
	DefRGB24_2_YUVSlice_Compose	pfnRGB24_2_YUV420_Compose;

	DefRGB24_2_YUV_Frame		pfnRGB24_2_YUV_Frame;
	DefRGB24_2_YUV_Split		pfnRGB24_2_YUV_Split;
	DefRGB24_2_YUV_Compose		pfnRGB24_2_YUV_Compose;

	// V1.1.0.0
	pfncs_rgb24_to_hsv_frame	rgb24_to_hsv_frame;
	pfncs_rgb24_to_hsv_split	rgb24_to_hsv_split;
	pfncs_rgb24_to_hsv_compose	rgb24_to_hsv_compose;
	// V1.1.0.1
	pfncs_rgb24_to_gray_frame	rgb24_to_gray_frame;
	pfncs_rgb24_to_gray_split	rgb24_to_gray_split;
	pfncs_rgb24_to_gray_compose	rgb24_to_gray_compose;
	pfncs_rgb24_histogram		rgb24_histogram;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始定义
} NXRGB24toOthers, *LPNXRGB24TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUVA => Others

// 32 位 YUVA 颜色到 32 位 BGRA/RGBA 颜色的变换
typedef DefYUV_2_RGB_Frame			DefYUVA_2_RGB32_Frame;
typedef DefYUV_2_RGB_Split			DefYUVA_2_RGB32_Split;
typedef DefYUV_2_RGB_Compose		DefYUVA_2_RGB32_Compose;
// 32 位 YUVA 颜色到 24 位 BGR/RGB 颜色的变换，直接丢弃 A
typedef DefYUV_2_RGB_Frame			DefYUVA_2_RGB24_Frame;
typedef DefYUV_2_RGB_Split			DefYUVA_2_RGB24_Split;
typedef DefYUV_2_RGB_Compose		DefYUVA_2_RGB24_Compose;
// 32 位 YUVA 颜色到 16 位 YUYV/UYVY 颜色的变换，直接丢弃 A 与 第二个 UV
typedef DefYUV_2_YUV16_Frame		DefYUVA_2_YUV16_Frame;
typedef DefYUV_2_YUV16_Split		DefYUVA_2_YUV16_Split;
typedef DefYUV_2_YUV16_Compose		DefYUVA_2_YUV16_Compose;
// 32 位 YUVA 颜色到分片 YUV422/YUV420 的变换，直接丢弃 A 与第二个 UV，YUV420 时 UV 由两行插值出一行
typedef DefYUV_2_YUVSlice_Frame		DefYUVA_2_YUVSlice_Frame;
typedef DefYUV_2_YUVSlice_Split		DefYUVA_2_YUVSlice_Split;
typedef DefYUV_2_YUVSlice_Compose	DefYUVA_2_YUVSlice_Compose;

typedef struct tag_nxyuvatoothers
{
	DefYUVA_2_RGB32_Frame		pfnYUVA_2_RGB32_Frame;
	DefYUVA_2_RGB32_Split		pfnYUVA_2_RGB32_Split;
	DefYUVA_2_RGB32_Compose		pfnYUVA_2_RGB32_Compose;

	DefYUVA_2_RGB24_Frame		pfnYUVA_2_RGB24_Frame;
	DefYUVA_2_RGB24_Split		pfnYUVA_2_RGB24_Split;
	DefYUVA_2_RGB24_Compose		pfnYUVA_2_RGB24_Compose;

	DefYUVA_2_YUV16_Frame		pfnYUVA_2_YUV16_Frame;
	DefYUVA_2_YUV16_Split		pfnYUVA_2_YUV16_Split;
	DefYUVA_2_YUV16_Compose		pfnYUVA_2_YUV16_Compose;

	DefYUVA_2_YUVSlice_Frame	pfnYUVA_2_YUV422_Frame;
	DefYUVA_2_YUVSlice_Split	pfnYUVA_2_YUV422_Split;
	DefYUVA_2_YUVSlice_Compose	pfnYUVA_2_YUV422_Compose;

	DefYUVA_2_YUVSlice_Frame	pfnYUVA_2_YUV420_Frame;
	DefYUVA_2_YUVSlice_Split	pfnYUVA_2_YUV420_Split;
	DefYUVA_2_YUVSlice_Compose	pfnYUVA_2_YUV420_Compose;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始定义
} NXYUVAtoOthers, *LPNXYUVATOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUV16(YUYV/UYVY) => Others

// 16 位 UYVY 与 YUYV 颜色的变换，交换一个字内的两个字节即可实现互换
typedef void (*DefYUV16_Swap_Frame)(OUT unsigned char *pucYUV16OutFrame,	// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									IN intptr_t nOutPitch,					// 16 位输出缓冲一行所占用的字节数
									IN unsigned char *pucYUV16InFrame,		// 16 位输入缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									IN intptr_t nInPitch,					// 16 位输入缓冲一行所占用的字节数
									IN int nWidth,							// 源图像素矩阵大小
									IN int nHeight,
									IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUV16_Swap_Split)(OUT unsigned char *pucYUV16OutTopField,		// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									IN intptr_t nOutTopPitch,					// 16 位输出缓冲一行所占用的字节数
									OUT unsigned char *pucYUV16OutBottomField,	// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									IN intptr_t nOutBottomPitch,				// 16 位输出缓冲一行所占用的字节数
									IN unsigned char *pucYUV16InFrame,			// 16 位输入缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									IN intptr_t nInPitch,						// 16 位输入缓冲一行所占用的字节数
									IN int nWidth,								// 源图像素矩阵大小
									IN int nHeight,
									IN bool bOverturn);							// 翻转图像标志
typedef void (*DefYUV16_Swap_Compose)(OUT unsigned char *pucYUV16OutFrame,		// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									  IN intptr_t nOutPitch,					// 16 位输出缓冲一行所占用的字节数
									  IN unsigned char *pucYUV16InTopField,		// 16 位输入缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									  IN intptr_t nInTopPitch,					// 16 位输入缓冲一行所占用的字节数
									  IN unsigned char *pucYUV16InBottomField,	// 16 位输入缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									  IN intptr_t nInBottomPitch,				// 16 位输入缓冲一行所占用的字节数
									  IN int nWidth,							// 源图像素矩阵大小
									  IN int nHeight,
									  IN bool bOverturn);						// 翻转图像标志
// 16 位 YUYV/UYVY 颜色到 32 位 BGRA/RGBA 颜色的变换，A 可填充为统一值
typedef void (*DefYUV16_2_RGB32_Frame)(OUT unsigned char *pucRGB32Frame,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									   IN intptr_t nRGB32Pitch,				// 32 位缓存一行所占用的字节数
									   IN unsigned char *pucYUV16Frame,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									   IN intptr_t nYUV16Pitch,				// 16 位缓存一行所占用的字节数
									   IN int nWidth,						// 源图像素矩阵大小
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
									   IN bool bBGRA,						// 32 位缓冲的像素格式为 A R G B（B在低位）
									   IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									   IN unsigned char cAlpha,				// 填充的 A 值
									   IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUV16_2_RGB32_Split)(OUT unsigned char *pucRGB32TopField,		// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									   IN intptr_t nRGB32TopPitch,				// 32 位缓存一行所占用的字节数
									   OUT unsigned char *pucRGB32BottomField,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									   IN intptr_t nRGB32BottomPitch,			// 32 位缓存一行所占用的字节数
									   IN unsigned char *pucYUV16Frame,			// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									   IN intptr_t nYUV16Pitch,					// 16 位缓存一行所占用的字节数
									   IN int nWidth,							// 源图像素矩阵大小
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
									   IN bool bBGRA,							// 32 位缓冲的像素格式为 A R G B（B在低位）
									   IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									   IN unsigned char cAlpha,					// 填充的 A 值
									   IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUV16_2_RGB32_Compose)(OUT unsigned char *pucRGB32Frame,		// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
										 IN intptr_t nRGB32Pitch,				// 32 位缓存一行所占用的字节数
										 IN unsigned char *pucYUV16TopField,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										 IN intptr_t nYUV16TopPitch,			// 16 位缓存一行所占用的字节数
										 IN unsigned char *pucYUV16BottomField,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										 IN intptr_t nYUV16BottomPitch,			// 16 位缓存一行所占用的字节数
										 IN int nWidth,							// 源图像素矩阵大小
										 IN int nHeight,
										 IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
										 IN bool bBGRA,							// 32 位缓冲的像素格式为 A R G B（B在低位）
										 IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
										 IN unsigned char cAlpha,				// 填充的 A 值
										 IN bool bOverturn);					// 翻转图像标志
// 16 位 YUYV/UYVY 颜色到 24 位 BGR/RGB 颜色的变换
typedef void (*DefYUV16_2_RGB24_Frame)(OUT unsigned char *pucRGB24Frame,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									   IN intptr_t nRGB24Pitch,				// 24 位缓存一行所占用的字节数
									   IN unsigned char *pucYUV16Frame,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									   IN intptr_t nYUV16Pitch,				// 16 位缓存一行所占用的字节数
									   IN int nWidth,						// 源图像素矩阵大小
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
									   IN bool bBGR,						// 24 位缓冲的像素格式为 R G B（B在低位）
									   IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									   IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUV16_2_RGB24_Split)(OUT unsigned char *pucRGB24TopField,		// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									   IN intptr_t nRGB24TopPitch,				// 24 位缓存一行所占用的字节数
									   OUT unsigned char *pucRGB24BottomField,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
									   IN intptr_t nRGB24BottomPitch,			// 24 位缓存一行所占用的字节数
									   IN unsigned char *pucYUV16Frame,			// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									   IN intptr_t nYUV16Pitch,					// 16 位缓存一行所占用的字节数
									   IN int nWidth,							// 源图像素矩阵大小
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
									   IN bool bBGR,							// 24 位缓冲的像素格式为 R G B（B在低位）
									   IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									   IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUV16_2_RGB24_Compose)(OUT unsigned char *pucRGB24Frame,		// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
										 IN intptr_t nRGB24Pitch,				// 24 位缓存一行所占用的字节数
										 IN unsigned char *pucYUV16TopField,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										 IN intptr_t nYUV16TopPitch,			// 16 位缓存一行所占用的字节数
										 IN unsigned char *pucYUV16BottomField,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										 IN intptr_t nYUV16BottomPitch,			// 16 位缓存一行所占用的字节数
										 IN int nWidth,							// 源图像素矩阵大小
										 IN int nHeight,
										 IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
										 IN bool bBGR,							// 24 位缓冲的像素格式为 R G B（B在低位）
										 IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
										 IN bool bOverturn);					// 翻转图像标志
// 16 位 YUYV/UYVY 颜色到 32 位 YUVA 颜色的变换，A 可填充为统一值，第二个 UV 值采用前后 UV 进行插值
typedef void (*DefYUV16_2_YUVA_Frame)(OUT unsigned char *pucYUVAFrame,	// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
									  IN intptr_t nYUVAPitch,			// 32 位缓存一行所占用的字节数
									  IN unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									  IN intptr_t nYUV16Pitch,			// 16 位缓存一行所占用的字节数
									  IN int nWidth,					// 源图像素矩阵大小
									  IN int nHeight,
									  IN bool bUYVY,					// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									  IN unsigned char ucAlpha,			// 填充的 A 值
									  IN bool bOverturn);				// 翻转图像标志
typedef void (*DefYUV16_2_YUVA_Split)(OUT unsigned char *pucYUVATopField,		// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
									  IN intptr_t nYUVATopPitch,				// 32 位缓存一行所占用的字节数
									  OUT unsigned char *pucYUVABottomField,	// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
									  IN intptr_t nYUVABottomPitch,				// 32 位缓存一行所占用的字节数
									  IN unsigned char *pucYUV16Frame,			// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									  IN intptr_t nYUV16Pitch,					// 16 位缓存一行所占用的字节数
									  IN int nWidth,							// 源图像素矩阵大小
									  IN int nHeight,
									  IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									  IN unsigned char ucAlpha,					// 填充的 A 值
									  IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUV16_2_YUVA_Compose)(OUT unsigned char *pucYUVAFrame,		// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
										IN intptr_t nYUVAPitch,					// 32 位缓存一行所占用的字节数
										IN unsigned char *pucYUV16TopField,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										IN intptr_t nYUV16TopPitch,				// 16 位缓存一行所占用的字节数
										IN unsigned char *pucYUV16BottomField,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										IN intptr_t nYUV16BottomPitch,			// 16 位缓存一行所占用的字节数
										IN int nWidth,							// 源图像素矩阵大小
										IN int nHeight,
										IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
										IN unsigned char ucAlpha,				// 填充的 A 值
										IN bool bOverturn);						// 翻转图像标志
// 16 位 YUYV/UYVY 颜色到分片 YUV422/YUV420 的变换，YUV420 时 UV 由两行插值出一行
typedef void (*DefYUV16_2_YUVSlice_Frame)(OUT unsigned char *pucYFrame,			// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYPitch,					// 分片 Y 缓冲一行所占用的字节数
										  OUT unsigned char *pucUUFrame,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUPitch,					// 分片 U 缓冲一行所占用的字节数
										  OUT unsigned char *pucVVFrame,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVPitch,					// 分片 V 缓冲一行所占用的字节数
										  IN unsigned char *pucYUV16Frame,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										  IN intptr_t nYUV16Pitch,				// 16 位缓冲一行所占用的字节数
										  IN int nWidth,						// 源图像素矩阵大小
										  IN int nHeight,
										  IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
										  IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUV16_2_YUVSlice_Split)(OUT unsigned char *pucYTopField,		// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYTopPitch,				// 分片 Y 缓冲一行所占用的字节数
										  OUT unsigned char *pucYBottomField,	// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
										  IN intptr_t nYBottomPitch,			// 分片 Y 缓冲一行所占用的字节数
										  OUT unsigned char *pucUUTopField,		// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUTopPitch,				// 分片 U 缓冲一行所占用的字节数
										  OUT unsigned char *pucUUBottomField,	// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
										  IN intptr_t nUUBottomPitch,			// 分片 U 缓冲一行所占用的字节数
										  OUT unsigned char *pucVVTopField,		// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVTopPitch,				// 分片 V 缓冲一行所占用的字节数
										  OUT unsigned char *pucVVBottomField,	// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
										  IN intptr_t nVVBottomPitch,			// 分片 V 缓冲一行所占用的字节数
										  IN unsigned char *pucYUV16Frame,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
										  IN intptr_t nYUV16Pitch,				// 16 位缓冲一行所占用的字节数
										  IN int nWidth,						// 源图像素矩阵大小
										  IN int nHeight,
										  IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
										  IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUV16_2_YUVSlice_Compose)(OUT unsigned char *pucYFrame,				// 分片 Y 缓冲（存储方式 Y0 Y1 Y2 Y3 ……）
											IN intptr_t nYPitch,						// 分片 Y 缓冲一行所占用的字节数
											OUT unsigned char *pucUUFrame,				// 分片 U 缓冲（存储方式 UU0 UU2 UU4 ……）
											IN intptr_t nUUPitch,						// 分片 U 缓冲一行所占用的字节数
											OUT unsigned char *pucVVFrame,				// 分片 V 缓冲（存储方式 VV0 VV2 VV4 ……）
											IN intptr_t nVVPitch,						// 分片 V 缓冲一行所占用的字节数
											IN unsigned char *pucYUV16TopField,			// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
											IN intptr_t nYUV16TopPitch,					// 16 位缓冲一行所占用的字节数
											IN unsigned char *pucYUV16BottomField,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
											IN intptr_t nYUV16BottomPitch,				// 16 位缓冲一行所占用的字节数
											IN int nWidth,								// 源图像素矩阵大小
											IN int nHeight,
											IN bool bUYVY,								// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
											IN bool bOverturn);							// 翻转图像标志
// 16 位 YUYV/UYVY 颜色到 24 位 YUV 颜色的变换，第二个 UV 值采用前后 UV 进行插值
typedef void (*DefYUV16_2_YUV_Frame)(OUT unsigned char *pucYUVFrame,	// 24 位缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
									 IN intptr_t nYUVPitch,				// 24 位缓存一行所占用的字节数
									 IN unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									 IN intptr_t nYUV16Pitch,			// 16 位缓存一行所占用的字节数
									 IN int nWidth,						// 源图像素矩阵大小
									 IN int nHeight,
									 IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									 IN bool bOverturn);				// 翻转图像标志
typedef void (*DefYUV16_2_YUV_Split)(OUT unsigned char *pucYUVTopField,		// 24 位缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
									 IN intptr_t nYUVTopPitch,				// 24 位缓存一行所占用的字节数
									 OUT unsigned char *pucYUVBottomField,	// 24 位缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
									 IN intptr_t nYUVBottomPitch,			// 24 位缓存一行所占用的字节数
									 IN unsigned char *pucYUV16Frame,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									 IN intptr_t nYUV16Pitch,				// 16 位缓存一行所占用的字节数
									 IN int nWidth,							// 源图像素矩阵大小
									 IN int nHeight,
									 IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									 IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUV16_2_YUV_Compose)(OUT unsigned char *pucYUVFrame,			// 24 位缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
									   IN intptr_t nYUVPitch,					// 24 位缓存一行所占用的字节数
									   IN unsigned char *pucYUV16TopField,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									   IN intptr_t nYUV16TopPitch,				// 16 位缓存一行所占用的字节数
									   IN unsigned char *pucYUV16BottomField,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									   IN intptr_t nYUV16BottomPitch,			// 16 位缓存一行所占用的字节数
									   IN int nWidth,							// 源图像素矩阵大小
									   IN int nHeight,
									   IN bool bUYVY,							// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
									   IN bool bOverturn);						// 翻转图像标志
// 10Bit YUV422/YUV420 分片与 8Bit YUV422/YUV420 分片的互转
typedef void (*DefYUVSlice_2_YUVSlice_Frame)(OUT unsigned char* pucY8Bit,	// 8Bit Y 输出
											 IN intptr_t nYPitch8B,			// 8Bit Y 一行的字节数
											 OUT unsigned char* pucU8Bit,	// 8Bit U 输出
											 IN intptr_t nUPitch8B,			// 8Bit U 一行的字节数
											 OUT unsigned char* pucVBit,	// 8Bit V 输出
											 IN intptr_t nVPitch8B,			// 8Bit V 一行的字节数
											 IN unsigned char* pucY10Bit,	// 10Bit Y 输入
											 IN intptr_t nYPitch10B,		// 10Bit Y 一行的字节数
											 IN unsigned char* pucU10Bit,	// 10Bit U 输入
											 IN intptr_t nUPitch10B,		// 10Bit U 一行的字节数
											 IN unsigned char* pucV10Bit,	// 10Bit V 输入
											 IN intptr_t nVPitch10B,		// 10Bit V 一行的字节数
											 IN int nWidth,					// 图像宽高
											 IN int nHeight,
											 IN bool bOverTurn);			// 翻转标志

typedef struct tag_nxyuv16toothers
{
	DefYUV16_Swap_Frame			pfnYUV16_Swap_Frame;
	DefYUV16_Swap_Split			pfnYUV16_Swap_Split;
	DefYUV16_Swap_Compose		pfnYUV16_Swap_Compose;

	DefYUV16_2_RGB32_Frame		pfnYUV16_2_RGB32_Frame;
	DefYUV16_2_RGB32_Split		pfnYUV16_2_RGB32_Split;
	DefYUV16_2_RGB32_Compose	pfnYUV16_2_RGB32_Compose;

	DefYUV16_2_RGB24_Frame		pfnYUV16_2_RGB24_Frame;
	DefYUV16_2_RGB24_Split		pfnYUV16_2_RGB24_Split;
	DefYUV16_2_RGB24_Compose	pfnYUV16_2_RGB24_Compose;

	DefYUV16_2_YUVA_Frame		pfnYUV16_2_YUVA_Frame;
	DefYUV16_2_YUVA_Split		pfnYUV16_2_YUVA_Split;
	DefYUV16_2_YUVA_Compose		pfnYUV16_2_YUVA_Compose;

	DefYUV16_2_YUVSlice_Frame	pfnYUV16_2_YUV422_Frame;
	DefYUV16_2_YUVSlice_Split	pfnYUV16_2_YUV422_Split;
	DefYUV16_2_YUVSlice_Compose	pfnYUV16_2_YUV422_Compose;

	DefYUV16_2_YUVSlice_Frame	pfnYUV16_2_YUV420_Frame;
	DefYUV16_2_YUVSlice_Split	pfnYUV16_2_YUV420_Split;
	DefYUV16_2_YUVSlice_Compose	pfnYUV16_2_YUV420_Compose;

	DefYUV16_2_YUV_Frame		pfnYUV16_2_YUV_Frame;
	DefYUV16_2_YUV_Split		pfnYUV16_2_YUV_Split;
	DefYUV16_2_YUV_Compose		pfnYUV16_2_YUV_Compose;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始定义
} NXYUV16toOthers, *LPNXYUV16TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUV422 => Others

typedef struct tag_nxyuv422toothers
{
	DefYUVSlice_2_RGB32_Frame	pfnYUV422_2_RGB32_Frame;
	DefYUVSlice_2_RGB32_Split	pfnYUV422_2_RGB32_Split;
	DefYUVSlice_2_RGB32_Compose	pfnYUV422_2_RGB32_Compose;

	DefYUVSlice_2_RGB24_Frame	pfnYUV422_2_RGB24_Frame;
	DefYUVSlice_2_RGB24_Split	pfnYUV422_2_RGB24_Split;
	DefYUVSlice_2_RGB24_Compose	pfnYUV422_2_RGB24_Compose;

	DefYUVSlice_2_YUVA_Frame	pfnYUV422_2_YUVA_Frame;
	DefYUVSlice_2_YUVA_Split	pfnYUV422_2_YUVA_Split;
	DefYUVSlice_2_YUVA_Compose	pfnYUV422_2_YUVA_Compose;

	DefYUVSlice_2_YUV16_Frame	pfnYUV422_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV422_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV422_2_YUV16_Compose;

	DefYUVSlice_2_YUV16_Frame	pfnYUV422_10Bit_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV422_10Bit_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV422_10Bit_2_YUV16_Compose;

	DefYUVSlice_2_YUV_Frame		pfnYUV422_2_YUV_Frame;
	DefYUVSlice_2_YUV_Split		pfnYUV422_2_YUV_Split;
	DefYUVSlice_2_YUV_Compose	pfnYUV422_2_YUV_Compose;

	DefYUVSlice_ConvertUV_Frame		pfnYUV422_2_YUV420_UV_Frame;
	DefYUVSlice_ConvertUV_Split		pfnYUV422_2_YUV420_UV_Split;
	DefYUVSlice_ConvertUV_Compose	pfnYUV422_2_YUV420_UV_Compose;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.7.0
	DefYUVSlice_2_YUVSlice_Frame	pfnYUV422_10Bit_2_YUV422_8Bit_Frame;
	DefYUVSlice_ConvertUV_Field		pfnYUV422_2_YUV420_UV_Field;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始定义
} NXYUV422toOthers, *LPNXYUV422TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUV420 => Others

typedef struct tag_nxyuv420toothers
{
	DefYUVSlice_2_RGB32_Frame	pfnYUV420_2_RGB32_Frame;
	DefYUVSlice_2_RGB32_Split	pfnYUV420_2_RGB32_Split;
	DefYUVSlice_2_RGB32_Compose	pfnYUV420_2_RGB32_Compose;

	DefYUVSlice_2_RGB24_Frame	pfnYUV420_2_RGB24_Frame;
	DefYUVSlice_2_RGB24_Split	pfnYUV420_2_RGB24_Split;
	DefYUVSlice_2_RGB24_Compose	pfnYUV420_2_RGB24_Compose;

	DefYUVSlice_2_YUVA_Frame	pfnYUV420_2_YUVA_Frame;
	DefYUVSlice_2_YUVA_Split	pfnYUV420_2_YUVA_Split;
	DefYUVSlice_2_YUVA_Compose	pfnYUV420_2_YUVA_Compose;

	DefYUVSlice_2_YUV16_Frame	pfnYUV420_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV420_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV420_2_YUV16_Compose;

	DefYUVSlice_2_YUV16_Frame	pfnYUV420_10Bit_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV420_10Bit_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV420_10Bit_2_YUV16_Compose;

	DefYUVSlice_2_YUV_Frame		pfnYUV420_2_YUV_Frame;
	DefYUVSlice_2_YUV_Split		pfnYUV420_2_YUV_Split;
	DefYUVSlice_2_YUV_Compose	pfnYUV420_2_YUV_Compose;

	DefYUVSlice_ConvertUV_Frame	pfnYUV420_2_YUV422_UV_Frame;
	DefYUVSlice_ConvertUV_Split	pfnYUV420_2_YUV422_UV_Split;
	DefYUVSlice_ConvertUV_Compose pfnYUV420_2_YUV422_UV_Compose;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.7.0
	DefYUVSlice_2_YUVSlice_Frame	pfnYUV420_10Bit_2_YUV420_8Bit_Frame;
	DefYUVSlice_ConvertUV_Field		pfnYUV420_2_YUV422_UV_Field;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始定义
} NXYUV420toOthers, *LPNXYUV420TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUV => Others

// 24 位 YUV 颜色到 32 位 BGRA/RGBA 颜色的变换，A 可填充为统一值
typedef void (*DefYUV_2_RGB32_Frame)(OUT unsigned char *pucRGB32Frame,		// 32 位输出缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									 IN intptr_t nRGB32Pitch,				// 32 位输出缓存一行所占用的字节数
									 IN unsigned char *pucYUVFrame,			// 24 位输入缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
									 IN intptr_t nYUVPitch,					// 24 位输入缓存一行所占用的字节数
									 IN int nWidth,							// 源图像素矩阵大小
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
									 IN bool bBGRA,							// 32 位输出缓冲的像素格式为 A R G B（B在低位）的标志
									 IN unsigned char cAlpha,				// 填充的 A 值
									 IN bool bOverturn);					// 翻转图像标志
typedef void (*DefYUV_2_RGB32_Split)(OUT unsigned char *pucRGB32TopField,		// 32 位输出缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									 IN intptr_t nRGB32TopPitch,				// 32 位输出缓存一行所占用的字节数
									 OUT unsigned char *pucRGB32BottomField,	// 32 位输出缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									 IN intptr_t nRGB32BottomPitch,				// 32 位输出缓存一行所占用的字节数
									 IN unsigned char *pucYUVFrame,				// 24 位输入缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
									 IN intptr_t nYUVPitch,						// 24 位输入缓存一行所占用的字节数
									 IN int nWidth,								// 源图像素矩阵大小
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,			// 清晰度
									 IN bool bBGRA,								// 32 位输出缓冲的像素格式为 A R G B（B在低位）
									 IN unsigned char cAlpha,					// 填充的 A 值
									 IN bool bOverturn);						// 翻转图像标志
typedef void (*DefYUV_2_RGB32_Compose)(OUT unsigned char *pucRGB32Frame,		// 32 位输出缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
									   IN intptr_t nRGB32Pitch,					// 32 位输出缓存一行所占用的字节数
									   IN unsigned char *pucYUVTopField,		// 24 位输入缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
									   IN intptr_t nYUVTopPitch,				// 24 位输入缓存一行所占用的字节数
									   IN unsigned char *pucYUVBottomField,		// 24 位输入缓存（存储方式 Y0 U0 V0 Y1 U1 V1 ...）
									   IN intptr_t nYUVBottomPitch,				// 24 位输入缓存一行所占用的字节数
									   IN int nWidth,							// 源图像素矩阵大小
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,		// 清晰度
									   IN bool bBGRA,							// 32 位输出缓冲的像素格式为 A R G B（B在低位）
									   IN unsigned char cAlpha,					// 填充的 A 值
									   IN bool bOverturn);						// 翻转图像标志
// 24 位 YUV 颜色到 24 位 BGR/RGB 颜色的变换
typedef DefYUV_2_RGB_Frame			DefYUV_2_RGB24_Frame;
typedef DefYUV_2_RGB_Split			DefYUV_2_RGB24_Split;
typedef DefYUV_2_RGB_Compose		DefYUV_2_RGB24_Compose;

typedef struct tag_nxyuvtoothers
{
	DefYUV_2_RGB32_Frame		pfnYUV_2_RGB32_Frame;
	DefYUV_2_RGB32_Split		pfnYUV_2_RGB32_Split;
	DefYUV_2_RGB32_Compose		pfnYUV_2_RGB32_Compose;

	DefYUV_2_RGB24_Frame		pfnYUV_2_RGB24_Frame;
	DefYUV_2_RGB24_Split		pfnYUV_2_RGB24_Split;
	DefYUV_2_RGB24_Compose		pfnYUV_2_RGB24_Compose;

	DefYUV_2_YUV16_Frame		pfnYUV_2_YUV16_Frame;
	DefYUV_2_YUV16_Split		pfnYUV_2_YUV16_Split;
	DefYUV_2_YUV16_Compose		pfnYUV_2_YUV16_Compose;

	DefYUV_2_YUVSlice_Frame		pfnYUV_2_YUV422_Frame;
	DefYUV_2_YUVSlice_Split		pfnYUV_2_YUV422_Split;
	DefYUV_2_YUVSlice_Compose	pfnYUV_2_YUV422_Compose;

	DefYUV_2_YUVSlice_Frame		pfnYUV_2_YUV420_Frame;
	DefYUV_2_YUVSlice_Split		pfnYUV_2_YUV420_Split;
	DefYUV_2_YUVSlice_Compose	pfnYUV_2_YUV420_Compose;

	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始定义
} NXYUVtoOthers, *LPNXYUVTOOTHERS;

typedef struct tag_nxcolorinnerapi *LPNXCOLORINNERAPI;

typedef struct tag_nxcolorspaceapi
{
	LPNXCOLORGENERICAPI		pStuColorGenericAPI;
	LPNXRGB32TOOTHERS		pStuRGB32toOthers;
	LPNXRGB24TOOTHERS		pStuRGB24toOthers;
	LPNXYUVATOOTHERS		pStuYUVAtoOthers;
	LPNXYUV16TOOTHERS		pStuYUV16toOthers;
	LPNXYUV422TOOTHERS		pStuYUV422toOthers;
	LPNXYUV420TOOTHERS		pStuYUV420toOthers;
	LPNXYUVTOOTHERS			pStuYUVtoOthers;
	// Private 权限，用于优化库内部的接口
	LPNXCOLORINNERAPI		pStuInnerAPI;
} NXColorSpaceAPI, *LPNXCOLORSPACEAPI;


#endif		// #ifndef __NX_COLOR_SPACE_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
