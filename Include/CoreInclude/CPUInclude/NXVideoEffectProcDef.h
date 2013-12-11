//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 视频特效算法接口定义，如淡入淡出、划像、闪黑闪白、缩放混叠等.
//
// Created: ZhaoZhimeng @ 2009-06-09
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_VIDEO_EFFECT_PROC_DEF_H__
#define __NX_VIDEO_EFFECT_PROC_DEF_H__

#include ".\NXBasic.h"
#include ".\NXVideoProcDef.h"

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

#define NXVIDEOEFFECTPROC_DLL_NAME			_T("NXVideoEffectProc")
#define NXVIDEOEFFECTPROC_API_EXT_NAME		"NVEP_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// 划像特技参数定义

// （1）类型定义针对于参数中的左边图像，表示按既定方式将左边图像划出、右边图像划入。
// （2）如果希望物理位置在左边的图像按既定方式划入，可以调用特技接口时，将其做为参数的右边图像传入。
enum NXVE_WIPE_TYPE
{
	NXVE_WIPE_L_2_R_SLIDE,			// 左边到右边的垂直线划出
	NXVE_WIPE_R_2_L_SLIDE,			// 右边到左边的垂直线划出
	NXVE_WIPE_T_2_B_SLIDE,			// 上边到下边的水平线划出
	NXVE_WIPE_B_2_T_SLIDE,			// 下边到上边的水平线划出

	NXVE_WIPE_C_2_LR_SLIDE,			// 中心到左右两边的垂直线划出
	NXVE_WIPE_LR_2_C_SLIDE,			// 左右两边到中心的垂直线划出
	NXVE_WIPE_C_2_TB_SLIDE,			// 中心到上下两边的水平线划出
	NXVE_WIPE_TB_2_C_SLIDE,			// 上下两边到中心的水平线划出

	NXVE_WIPE_C_2_LT_RB_SLIDE,		// 中心到左上角与右下角的对角线划出
	NXVE_WIPE_LT_RB_2_C_SLIDE,		// 左上角与右下角到中心的对角线划出
	NXVE_WIPE_C_2_RT_LB_SLIDE,		// 中心到右上角与左下角的对角线划出
	NXVE_WIPE_RT_LB_2_C_SLIDE,		// 右上角与左下角到中心的对角线划出

	NXVE_WIPE_RT_2_LB_SLIDE,		// 右上到左下的对角线划出
	NXVE_WIPE_LB_2_RT_SLIDE,		// 左下到右上的对角线划出
	NXVE_WIPE_LT_2_RB_SLIDE,		// 左上到右下的对角线划出
	NXVE_WIPE_RB_2_LT_SLIDE,		// 右下到左上的对角线划出

	NXVE_WIPE_RT_2_LB_CORNER,		// 右上到左下的直角划出
	NXVE_WIPE_LB_2_RT_CORNER,		// 左下到右上的直角划出
	NXVE_WIPE_RB_2_LT_CORNER,		// 右下到左上的直角划出
	NXVE_WIPE_LT_2_RB_CORNER,		// 左上到右下的直角划出

	NXVE_WIPE_RT_2_LB_BOX,			// 右上到左下的方格划出
	NXVE_WIPE_LB_2_RT_BOX,			// 左下到右上的方格划出
	NXVE_WIPE_RB_2_LT_BOX,			// 右下到左上的方格划出
	NXVE_WIPE_LT_2_RB_BOX,			// 左上到右下的方格划出

	NXVE_WIPE_LTRB_2_C_CORNER,		// 四边到中心的四直角划出
	NXVE_WIPE_C_2_LTRB_CORNER,		// 中心到四边的四直角划出
	NXVE_WIPE_LTRB_2_C_BOX,			// 四边到中心的四方格划出
	NXVE_WIPE_C_2_LTRB_BOX,			// 中心到四边的方格划出

	NXVE_WIPE_LTRB_2_C_CIRCLE,		// 四个角到中心的圆形划出
	NXVE_WIPE_C_2_LTRB_CIRCLE,		// 中心到四个角的圆形划出

	NXVE_WIPE_TYPE_COUNT
};

typedef struct tag_nxvewipeparam
{
	NXVE_WIPE_TYPE		eType;			// 划像类型
	double				dPercent;		// 划像进度，取值 [0, 1]
	// 不同类型的私有参数
	union{
	};
} NXVEWipeParam, *LPNXVEWIPEPARAM;

//////////////////////////////////////////////////////////////////////////
// 特技处理区域的形状定义
enum NXVE_SHAPE_TYPE
{
	NXVE_SHAPE_RECT,				// 矩形
	NXVE_SHAPE_ELLIPSE				// （椭）圆
};

//////////////////////////////////////////////////////////////////////////
// 纹理补丁特技参数定义

enum NXVE_TEX_PATCH_ARITH
{
	NXVE_TEX_PATCH_ARITH_GSSORR,	// Gauss-seidel + SOR + Roberts

	NXVE_TEX_PATCH_ARITH_COUNT
};

typedef struct tagnxvetexpatchparam
{
	unsigned char			*pucSrc;		// 源有效起点（为 NULL 时表示纹理消除，不为 NULL 时表示纹理克隆）
	intptr_t				nSrcPitch;		// 源一行所占用的字节数
	unsigned char			*pucMask;		// 像素 8 位掩码有效起点（为 NULL 时表示处理完整区域，不为 NULL 时只处理大于阈值的区域）
	intptr_t				nMaskStrideX;	// 一个像素的掩码所占用的字节数
	intptr_t				nMaskPitch;		// 掩码一行所占用的字节数
	unsigned int			unMaskThr;		// 掩码阈值 [0, 255]（大于此阈值有效）
	float					*pfCache0;		// 计算加速缓冲，个数为 unProcWidth * unProcHeight * nPixelBytes，为 NULL 则由内部自主创建
	float					*pfCache1;
	int						nProcWidth;		// 处理大小
	int						nProcHeight;
	NXVE_TEX_PATCH_ARITH	nAirth;			// 求解方法
} NXVETexPatchParam, *LPNXVETEXPATCHPARAM;

//////////////////////////////////////////////////////////////////////////
// 过渡型特技算法

// 淡入淡出
// （1）淡出左边图像、淡入右边图像。
// （2）当左边图像为空时，填黑场，表现为右边图像淡入。
// （3）当右边图像为空时，填黑场，表现为左边图像淡出。
// （4）ePixelFmt 如果为 NXVIDEO_PIXEL_FMT_YUV422 或 NXVIDEO_PIXEL_FMT_YUV420 格式，
// 要求输入数据严格按照 Y 一片、U 一片、V 一片顺序存储，如同解码器输出的 RAW_DATA，
// 此时 nPitch 表示 Y 一行占用的字节宽度，而 U/V 的行占用字节宽度为 Y 的一半。
typedef bool (*DefVEFade)(OUT unsigned char* pucDes,		// 目的起点
						  IN intptr_t nDesPitch,			// 目的一行所占用的字节数
						  IN unsigned char* pucSrcLeft,		// 左边源
						  IN intptr_t nSrcLeftPitch,		// 左边源一行所占用的字节数
						  IN unsigned char* pucSrcRight,	// 右边源
						  IN intptr_t nSrcRightPitch,		// 右边源一行所占用的字节数
						  IN NXVIDEO_PIXEL_FMT ePixelFmt,	// 像素格式
						  IN double dLeftWeight,			// 左边源的权重
						  IN int nWidth,					// 图像像素幅面
						  IN int nHeight,
						  IN bool bOverTurn);				// 是否翻转
// 划像（无软边）
// （1）划出左边图像、划入右边图像；当 dPercent = 0.0 时，左边图像完全显示，右边图像被完全遮挡；
// 当 dPercent = 1.0 时，左边图像被完全遮挡，右边图像完全显示。
// （2）当左边图像为空时，填黑场，表现为右边图像划入。
// （3）当右边图像为空时，填黑场，表现为左边图像划出。
// （4）ePixelFmt 如果为 NXVIDEO_PIXEL_FMT_YUV422 或 NXVIDEO_PIXEL_FMT_YUV420 格式，
// 要求输入数据严格按照 Y 一片、U 一片、V 一片顺序存储，如同解码器输出的 RAW_DATA，
// 此时 nPitch 表示 Y 一行占用的字节宽度，而 U/V 的行占用字节宽度为 Y 的一半。
typedef bool (*DefVEWipe)(OUT unsigned char* pucDes,		// 目的
						  IN intptr_t nDesPitch,			// 目的一行所占用的字节数
						  IN unsigned char* pucSrcLeft,		// 左边源
						  IN intptr_t nSrcLeftPitch,		// 左边源一行所占用的字节数
						  IN unsigned char* pucSrcRight,	// 右边源
						  IN intptr_t nSrcRightPitch,		// 右边源一行所占用的字节数
						  IN NXVIDEO_PIXEL_FMT ePixelFmt,	// 像素格式
						  IN LPNXVEWIPEPARAM pParam,		// 划像参数
						  IN int nWidth,					// 图像像素幅面
						  IN int nHeight,
						  IN bool bOverTurn);				// 是否翻转

//////////////////////////////////////////////////////////////////////////

typedef struct tag_nxvetransitionapi		// 过渡型特技
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	DefVEFade			pfnVEFade;			// 淡入淡出
	DefVEWipe			pfnVEWipe;			// 划像
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXVETransitionAPI, *LPNXVETRANSITIONAPI;

//////////////////////////////////////////////////////////////////////////
// 颜色处理特技算法

// 闪入/闪出
// （1）闪现的颜色要求以 BGRA(0xARGB) 的格式给出，内部需要根据 ePixelFmt + eDefinition 判断处理方式。
// （2）当 ulCrBGRA = 0xff000000 时即是闪黑，当 ulCrBGRA = 0xffffffff 时即是闪白。
// （3）ePixelFmt 如果为 NXVIDEO_PIXEL_FMT_YUV422 或 NXVIDEO_PIXEL_FMT_YUV420 格式，
// 要求输入数据严格按照 Y 一片、U 一片、V 一片顺序存储，如同解码器输出的 RAW_DATA，
// 此时 nPitch 表示 Y 一行占用的字节宽度，而 U/V 的行占用字节宽度为 Y 的一半。
typedef bool (*DefVEFlash)(OUT unsigned char* pucDes,			// 目的
						   IN intptr_t nDesPitch,				// 目的一行所占用的字节数
						   IN unsigned char* pucSrc,			// 源
						   IN intptr_t nSrcPitch,				// 源一行所占用的字节数
						   IN NXVIDEO_PIXEL_FMT ePixelFmt,		// 像素格式
						   IN NXVIDEO_DEFINITION eDefinition,	// 信号清晰度（用于颜色变换）
						   IN bool bFlashIn,					// 闪入/闪出标志
						   IN unsigned long ulCrBGRA,			// 闪现的颜色，BGRA 格式
						   IN double dPercent,					// 进度，取值 [0, 1]
						   IN int nWidth,						// 图像像素幅面
						   IN int nHeight,
						   IN bool bOverTurn);					// 是否翻转

//////////////////////////////////////////////////////////////////////////
// 马赛克特技
// 考虑效率，源和目的相同，只重写需要处理的区域
// 处理区域包括矩形或（椭）圆，推荐矩形，效率更高
// 特技粒度参数指定特技的细粒度，参数越小，效果越接近原画面
// 如果是非 32 位的图像，将宽变换为 32 位图像的宽，如 YUYV 则将原宽除以 2
//////////////////////////////////////////////////////////////////////////
typedef bool (*DefVEMosaic)(IN OUT unsigned char* pucDes,		// 源和目的相同
							IN intptr_t nDesPitch,				// 目的一行的字节数
							IN int nWidth,						// 整区域宽高
							IN int nHeight,
							IN int nX,							// 处理区域偏移
							IN int nY,
							IN int nValidWidth,					// 处理区域宽高
							IN int nValidHeight,
							IN NXVE_SHAPE_TYPE eShape,			// 处理区域形状类型
							IN float fGranularity);				// 马赛克的粒度 [0, 1]
//////////////////////////////////////////////////////////////////////////
// 模糊特技
// 考虑效率，源和目的相同，只重写需要处理的区域
// 处理区域包括矩形或（椭）圆，推荐矩形，效率更高
// 模糊半径越大，效果越模糊
// 如果处理区域较大且外部会频繁调用，建议外部开辟临时缓冲
// 临时缓冲的最小值分以下三种情况：
// （1）仅水平方向模糊：proc_width * 8 * 通道数
// （1）仅垂直方向模糊：proc_width * proc_height * 8 * 通道数
// （1）水平和垂直方向模糊：proc_width * proc_height * 8 * 通道数
//////////////////////////////////////////////////////////////////////////
typedef NXRESULT (*pfnve_blur)(__inout unsigned char* des,				// 源和目的相同
							   __in intptr_t des_pitch,					// 一行的字节数
							   __in int proc_x,							// 模糊起点坐标
							   __in int proc_y,
							   __in int proc_width,						// 模糊范围
							   __in int proc_height,
							   __in NXVIDEO_PIXEL_FMT pixel_fmt,		// 像素格式
							   __in NXVE_SHAPE_TYPE shape,				// 处理区域形状类型
							   __in NXVIDEO_FILTER_TYPE filter,			// 目前只支持 NXVIDEO_FILTER_GAUSS
							   __in NXVE_ORIENTATION_TYPE orientation,	// 处理方向
							   __in int radius,							// 模糊半径 [0, 500]
							   __in unsigned char* cache_buf,			// 用于内部临时缓冲，为 NULL 时内部分配
							   __in intptr_t cache_pitch);				// 临时缓冲一行的字节数
//////////////////////////////////////////////////////////////////////////
// 纹理补丁特技
// （1）具有两种功能：纹理消除、纹理克隆，请参见 NXVETexPatchParam 中的 pucSrc 定义。
// （2）处理区域的左上角都由输入的地址起点开始，处理大小最小为 4x3，并要求内存访问完全合法。
// （3）不支持 NXVIDEO_PIXEL_FMT_YUV422 与 NXVIDEO_PIXEL_FMT_YUV420 两种分片格式，
// 如果需要处理这两种格式，请调用者分别将 Y、U、V 按 NXVIDEO_PIXEL_FMT_KEY 的格式处理，
// 此时需要注意像素掩码的关系，UV 的像素掩码需要折半。
typedef bool (*DefVETexPatch)(IN OUT unsigned char* pucDes,		// 目的有效起点
							  IN intptr_t nDesPitch,			// 目的一行所占用的字节数
							  IN NXVIDEO_PIXEL_FMT ePixelFmt,	// 像素格式
							  IN LPNXVETEXPATCHPARAM pParam);	// 参数包
//////////////////////////////////////////////////////////////////////////
// 图像去噪
typedef NXRESULT (*pfnve_denoising)(__out unsigned char* des,
									__in intptr_t des_pitch,
									__in unsigned char* src,
									__in intptr_t src_pitch,
									__in NXVIDEO_PIXEL_FMT pixel_fmt,
									__in int width,
									__in int height,
									__in double space_sigma,			// [0, 1]
									__in double range_sigma);			// [0, 1]
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxvecolorapi		// 颜色处理特技
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	DefVEFlash			pfnVEFlash;			// 闪入/闪出
	DefVEMosaic			pfnVEMosaic;		// 马赛克
	DefVETexPatch		pfnVETexPatch;		// 纹理补丁
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.21
	pfnve_blur			blur;				// 模糊特技
	// V1.1.0.5
	pfnve_denoising		denoising;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从这里开始
} NXVEColorAPI, *LPNXVECOLORAPI;

//////////////////////////////////////////////////////////////////////////
// 大型特技处理器

// Video Stabilization 算法处理器
interface INXVSAProc;
typedef INXVSAProc* (*DefCreateVSAProc)();
typedef void (*DefReleaseVSAProc)(INXVSAProc* &pVSAProc);

typedef struct tag_nxveplantapi		// 处理器生产
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// Video Stabilization 算法处理器
	DefCreateVSAProc		pfnCreateVSAProc;
	DefReleaseVSAProc		pfnReleaseVSAProc;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXVEPlantAPI, *LPNXVEPLANTAPI;

//////////////////////////////////////////////////////////////////////////
// 键值生成特技算法


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 接口结构定义

typedef struct tag_nxveinnerapi *LPNXVEINNERAPI;

typedef struct tag_nxvideoeffectprocapi
{
	// 过渡型特技算法接口
	LPNXVETRANSITIONAPI		pStuTransitionAPI;
	// 颜色处理特技算法接口
	LPNXVECOLORAPI			pStuColorAPI;
	// 处理器生产接口
	LPNXVEPLANTAPI			pStuPlantAPI;
	// Private 权限，用于优化库内部的接口
	LPNXVEINNERAPI			pStuInnerAPI;
} NXVideoEffectProcAPI, *LPNXVIDEOEFFECTPROCAPI;


#endif		// #ifndef __NX_VIDEO_EFFECT_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.