//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 视频信号的基本定义，如幅面比例、扫描方式、常量等.
//				视频处理接口的基本定义.
//
// Created: ZhaoZhimeng @ 2008-11-27
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_VIDEO_PROC_DEF_H__
#define __NX_VIDEO_PROC_DEF_H__

#include ".\NXBasic.h"
#include ".\NXColorSpaceDef.h"

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

#define NXVIDEOPROC_DLL_NAME		_T("NXVideoProc")
#define NXVIDEOPROC_API_EXT_NAME	"NVP_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// 视频信号的监视器输出幅面比例
enum NXVIDEO_ASPECT
{
	NXVIDEO_ASPECT_16_9,			// 16:9
	NXVIDEO_ASPECT_4_3,				// 4:3
	NXVIDEO_ASPECT_ARBITRARY = 3,	// 任意幅面
	NXVIDEO_ASPECT_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// 视频信号的适配方式
enum NXVIDEO_ADAPTION
{
	NXVIDEO_ADAPTION_HEIGHT,		// 按高度适配
	NXVIDEO_ADAPTION_WIDTH,			// 按宽度适配
	NXVIDEO_ADAPTION_NONE,			// 不适配
	NXVIDEO_ADAPTION_CENTER = 4,	// 居中
	NXVIDEO_ADAPTION_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// 视频信号的扫描方式
enum NXVIDEO_SCAN
{
	NXVIDEO_SCAN_INTERLACED_TOP,		// 隔行扫描顶场
	NXVIDEO_SCAN_INTERLACED = NXVIDEO_SCAN_INTERLACED_TOP,
	NXVIDEO_SCAN_INTERLACED_BOTTOM,		// 隔行扫描底场

	NXVIDEO_SCAN_PROGRESSIVE,			// 逐行扫描

	NXVIDEO_SCAN_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// 抗混叠定义
enum NXVIDEO_ANTIALIAS
{
	NXVIDEO_ANTIALIAS_4,		// 使用截止频率比为 0.5 的 3x3 抗混叠滤波盒
	NXVIDEO_ANTIALIAS_NONE,		// 不使用抗混叠

	NXVIDEO_ANTIALIAS_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// 视频信号重采样的逼近窗口类型与窗口大小
enum NXVIDEO_SAM_WINDOW
{
	NXVIDEO_SAM_WINDOW_SONY,
	NXVIDEO_SAM_WINDOW_KAISER,		// 推荐使用
	NXVIDEO_SAM_WINDOW_HAMMING,
	NXVIDEO_SAM_WINDOW_LANCZOS,
	NXVIDEO_SAM_WINDOW_BLACKMAN,

	NXVIDEO_SAM_WINDOW_UNKNOWN
};

enum NXVIDEO_SAM_TAPS
{
	NXVIDEO_SAM_TAPS_4,
	NXVIDEO_SAM_TAPS_8,			// 推荐使用
	NXVIDEO_SAM_TAPS_16,
	NXVIDEO_SAM_TAPS_24,

	NXVIDEO_SAM_TAPS_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////
// 滤波器类型定义
enum NXVIDEO_FILTER_TYPE
{
	NXVIDEO_FILTER_NONE,
	NXVIDEO_FILTER_POINT,			// 临近点
	NXVIDEO_FILTER_MEAN,			// 平均值
	NXVIDEO_FILTER_MEDIAN,			// 中值
	NXVIDEO_FILTER_LINEAR,			// 线性插值
	NXVIDEO_FILTER_CUBIC,			// 三次插值
	NXVIDEO_FILTER_GAUSS,			// 高斯
};

//////////////////////////////////////////////////////////////////////////
// 二维图像线性内存数据结构声明（指定区域不一定是内存访问有效区域）
typedef struct tag_nxbuf2dImg
{
	unsigned char*		pucBuf;			// 线性内存起始地址
	int					nBufBytes;		// 线性内存的总长（小于等于实际占用长度，大于等于 nBaseHeight * nPitch）
	int					nPitch;			// 一行分配的字节长度（大于等于 nBaseWidth * (nBitCount >> 3)）
	int					nBitCount;		// 一个像素的颜色位数
	NXVIDEO_PIXEL_FMT	ePixelFmt;		// 像素的格式
	int					nBaseWidth;		// 2D 内存一行所能支持的最大像素个数
	int					nBaseHeight;	// 2D 内存所能支持的最大行数
	NXVIDEO_SCAN		eScanType;		// 信号扫描方式
	double				dX;				// 指定区域起点（相对于 2D 内存起点的坐标，整数部分与 nWidth、nHeight 组成指定数据区域）
	double				dY;				// （小数部分表示子像素起点，可实现子像素重采样，一般小数部分的值为 0）
	int					nWidth;			// 指定区域大小（与 nX、nY 组成指定数据区域）（一般情况下表示图像幅面）
	int					nHeight;
	NXVIDEO_ASPECT		eAspect;		// 幅面比例
	NXVIDEO_DEFINITION	eDefinition;	// 清晰度
} NXBuf2DImg, *LPNXBUF2DIMG;


//////////////////////////////////////////////////////////////////////////
// 视频信号 deinterlacing 时的处理参数类别（不同的参数适用于不同的场合）
enum NXVIDEO_DEINTERLACE
{
	NXVIDEO_DEINTERLACE_REMOVE_OTHER,		// 去除相异场的影响，适用于制式转换/上下变换、简单变速等
	NXVIDEO_DEINTERLACE_RESERVE_OTHER,		// 保留相异场的影响，适用于 50i->25p 的丢场 IPC 等

	NXVIDEO_DEINTERLACE_UNKNOWN,
};

typedef int NXVIDEO_DEINTERLACE_ARITH;
enum
{
	// 8bit
	NXVIDEO_DEINTERLACE_TAPS_4,			// 兼容 V1.0.8.0 以前版本 NXVIDEO_SAM_TAPS_4
	NXVIDEO_DEINTERLACE_TAPS_8,			// 兼容 V1.0.8.0 以前版本 NXVIDEO_SAM_TAPS_8
	NXVIDEO_DEINTERLACE_MELA_YUYV,
	NXVIDEO_DEINTERLACE_MELA_UYVY,
	NXVIDEO_DEINTERLACE_MELA_32BIT,
	NXVIDEO_DEINTERLACE_MELA_KEY,
	// 10bit
	NXVIDEO_DEINTERLACE_TAPS_4_10B = NXVIDEO_DEINTERLACE_MELA_KEY + 1000,
	NXVIDEO_DEINTERLACE_TAPS_8_10B,
	NXVIDEO_DEINTERLACE_MELA_YUYV_10B,
	NXVIDEO_DEINTERLACE_MELA_UYVY_10B,
	NXVIDEO_DEINTERLACE_MELA_32BIT_10B,
	NXVIDEO_DEINTERLACE_MELA_KEY_10B,

	NXVIDEO_DEINTERLACE_ARITH_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// 视频信号一帧内场播放序的定义
enum NXVIDEO_FIELD_PLAY
{
	NXVIDEO_FIELD_PLAY_FIRST,		// 第一场
	NXVIDEO_FIELD_PLAY_SECOND,		// 第二场

	NXVIDEO_FIELD_PLAY_COUNT		// 一帧的场数
};


//////////////////////////////////////////////////////////////////////////
// 隔行视频信号中顶场与底场的互相转换算法定义
enum NXVIDEO_FIELDCONV_ARITH
{
	NXVIDEO_FIELDCONV_AVER_2,	// 效率最优
	NXVIDEO_FIELDCONV_SINC_4,	// 垂直方向质量更优
	NXVIDEO_FIELDCONV_MELA_3,	// 水平方向质量更优

	NXVIDEO_FIELDCONV_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////
// 图像合成类型
enum NX_BLEND_TYPE
{
	NX_BLEND_ALPHA,					// Alpha 合成
	NX_BLEND_NORMAL,				// 正常
	NX_BLEND_ADD,					// 相加
	NX_BLEND_SUB,					// 相减
	NX_BLEND_MUL,					// 正片叠底
	NX_BLEND_DIV,					// 排除
	NX_BLEND_DARKEN,				// 变暗
	NX_BLEND_LIGHTEN,				// 变亮
	NX_BLEND_SCREEN,				// 滤色
	NX_BLEND_HARD_LIGHT,			// 强光
	NX_BLEND_SOFT_LIGHT,			// 柔光
	NX_BLEND_COLOR_BURN,			// 颜色加深
	NX_BLEND_COLOR_DODGE,			// 颜色减淡
	NX_BLEND_OVERLAY,				// 叠加
};

//////////////////////////////////////////////////////////////////////////
// 特技处理方向类型
enum NXVE_ORIENTATION_TYPE
{
	NXVE_ORIENTATION_HOR,				// 水平
	NXVE_ORIENTATION_VER,				// 垂直
	NXVE_ORIENTATION_HOR_VER,			// 水平和垂直
};

//////////////////////////////////////////////////////////////////////////
// 导出接口定义

// 视频图像变换

// 视频格式变换器的创建与释放
interface INXVideoFmtConv;
typedef INXVideoFmtConv* (*DefCreateFmtConv)();
typedef void (*DefReleaseFmtConv)(INXVideoFmtConv* &pFmtConv);
// 视频格式变换器功能接口类的创建与释放
interface INXVideoFmtConvFunc;
typedef INXVideoFmtConvFunc* (*pfnvp_create_fmtconvfunc)();
typedef void (*pfnvp_release_fmtconvfunc)(INXVideoFmtConvFunc*& pFmtConvFunc);
// 简单变速器接口类的创建与释放
interface INXVideoSpeedProc;
typedef INXVideoSpeedProc* (*DefCreateSpeedProc)();
typedef void (*DefReleaseSpeedProc)(INXVideoSpeedProc* &pSpeedProc);
// 变速功能接口类的创建与释放
interface INXVideoSpeedFunc;
typedef INXVideoSpeedFunc* (*DefCreateSpeedFunc)();
typedef void (*DefReleaseSpeedFunc)(INXVideoSpeedFunc* &pSpeedFunc);
// 幅面适配与裁剪（返回 false 说明适配裁剪后操作区域无效）
// （1）由于计算机无法精确地用二进制表达某些小数，会对某些映射造成误差，从而造成结果图像相位偏移。
// 而单浮点精度只能表示 e^-6，对于宽 1920、采样系数量化 64 的情况，其计算累计误差可能会造成结果偏差。
typedef bool (*DefAspectAdaptCut)(OUT double *pdHorScale,			// 初始水平缩放比 srcW / desW
								  OUT double *pdVerScale,			// 初始垂直缩放比 srcH / desH
								  OUT double *pdSrcX,				// 适配裁剪后的源访问起点（与目标访问起点对应）
								  OUT double *pdSrcY,
								  OUT int *pnSrcWidth,				// 适配裁剪后的源访问有效区域大小（与目标访问有效区域对应）
								  OUT int *pnSrcHeight,
								  OUT int *pnDesX,					// 适配裁剪后的目标访问起点（必须为整型）
								  OUT int *pnDesY,
								  OUT int *pnDesWidth,				// 适配裁剪后的目标访问有效区域大小
								  OUT int *pnDesHeight,
								  IN const NXBuf2DImg *pSrcInfo,	// 源缓存的位置信息
								  IN const NXBuf2DImg *pDesInfo,	// 目标缓存的位置信息
								  IN NXVIDEO_ADAPTION eAdaption);	// 适配方式
// Interlacing 到 progressive 的信号变换，采用前一帧进行运动检测与补偿。
// （1）各个输入输出的图像幅面相同，由于不存列之间的插值，不用考虑像素格式。
//		备注：	第（1）条只适用于 V1.0.8.0 版本以前的两种算法，自 MELA 算法开始，就需要考虑像素格式。
//				为了兼容以前的版本，对算法的定义进行像素格式的分类。
//				参数 nLineProcBytes 的含义仍然不变，算法内部将会通过反算机制得出源图像的宽度。
// （2）输入前一帧与当前帧的图像，输出当前帧的两个插值帧，以及帧间差、场间差、场内差、虚拟帧。
// （3）eSamTaps 参数目前只支持到 NXVIDEO_SAM_TAPS_8。
//		备注：	第（3）条的 eSamTaps 参数类型自 V1.0.8.0 由 NXVIDEO_SAM_TAPS 改为 NXVIDEO_DEINTERLACE_ARITH，
//				新版本完全向下兼容以前的算法类型，新定义便于新算法的扩展。
typedef void (*DefDeInterlacing)(OUT unsigned char *pucTopFrame,		// 顶场插值帧
								 IN intptr_t nTopFramePitch,			// 一行所占字节数
								 OUT unsigned char *pucBottomFrame,		// 底场插值帧
								 IN intptr_t nBottomFramePitch,			// 一行所占字节数
								 OUT unsigned char *pucFrameSub,		// 帧间差
								 IN intptr_t nFrameSubPitch,			// 一行所占字节数
								 OUT unsigned char *pucFieldSub,		// 场间差
								 IN intptr_t nFieldSubPitch,			// 一行所占字节数
								 OUT unsigned char *pucVMove,			// 场内差
								 IN intptr_t nVMovePitch,				// 一行所占字节数
								 OUT unsigned char *pucImaginary,		// 虚拟帧
								 IN intptr_t nImaginaryPitch,			// 一行所占字节数
								 IN unsigned char *pucCurrFrame,		// 当前帧
								 IN intptr_t nCurrFramePitch,			// 一行所占字节数
								 IN unsigned char *pucPrevFrame,		// 前一帧
								 IN intptr_t nPrevFramePitch,			// 一行所占字节数
								 IN int nLineProcBytes,					// 一行所处理的字节数（小于上面所有的pitch）
								 IN int nLineCount,						// 总共处理的行数
								 IN NXVIDEO_DEINTERLACE_ARITH eArith,	// 采样算法
								 IN NXVIDEO_DEINTERLACE eDeinterlace);	// 参数类型
// 设置运动检测参数： KFrameSub、KFieldSub、KVMove，放大 16 位。
// （1）该接口用于设置运动检测参数，要求库使用者不调用，否则会影响视频变换质量。
typedef void (*DefSetMotionDetectCoef)(IN unsigned short usKFrameSub,	// 帧间差系数
									   IN unsigned short usKFieldSub,	// 场间差系数
									   IN unsigned short usKVMove);		// 场内差系数
// 图像信号的重采样
// （1）当 eSamWindow = NXVIDEO_SAM_WINDOW_SONY 时，eSamTaps 只支持到 NXVIDEO_SAM_TAPS_8，而且参数只适用于上采样。
typedef bool (*DefResampling)(IN OUT LPNXBUF2DIMG pDesImg,			// 目标图像
							  IN const LPNXBUF2DIMG pSrcImg,		// 源图像
							  IN NXVIDEO_ADAPTION eAdaption,		// 适配方式
							  IN NXVIDEO_ANTIALIAS eAntiAlias,		// 抗混叠滤类型
							  IN unsigned char *pucAACache,			// 抗混叠缓冲（用于加速，为 NULL 则在内部自动申请）
							  IN intptr_t nAACachePitch,			// 抗混叠缓冲一行所占字节数
							  IN NXVIDEO_SAM_WINDOW eSamWindow,		// 重采样窗口类型
							  IN NXVIDEO_SAM_TAPS eSamTaps);		// 重采样窗口宽度
// 帧率变换中计算实际可变换得到的目标帧长
// （1）由于广播信号的帧场差异，需要分开考虑，通过输入 NXVIDEO_SCAN_INTERLACED 或 NXVIDEO_SCAN_PROGRESSIVE 来控制。
// （2）FRC 表示 Frame Rate Conversion，制式转换器要求以此函数为帧长换算标准。
typedef int (*DefGetOutDurationFRC)(IN int nInDuration,			// 输入帧长
									IN double dInFrameRate,		// 输入帧率
									IN NXVIDEO_SCAN eInScan,	// 输入信号扫描方式
									IN double dOutFrameRate,	// 输出帧率
									IN NXVIDEO_SCAN eOutScan);	// 输出信号扫描方式
// 顶场、底场间的互相转换
// （1）当源与目标的场位置相同时直接拷贝，不同则进行转换。
// （2）NXVIDEO_PIXEL_FMT_YUV422 与 NXVIDEO_PIXEL_FMT_YUV420 两种分片格式要求，nDesPitch 与 nSrcPitch 表示
// Y 片的一行占用字节数，而 U、V 片的值为 Y 片值的一半。
typedef int (*DefFieldConv)(OUT unsigned char *pucDesField,		// 目标场
							IN intptr_t nDesPitch,				// 目标一行所占用的字节数
							IN bool bDesTopField,				// 目标场为顶场标志
							IN unsigned char *pucSrcField,		// 源场
							IN intptr_t nSrcPitch,				// 源一行所占用的字节数
							IN bool bSrcTopField,				// 源场为顶场标志
							IN NXVIDEO_PIXEL_FMT ePixelFmt,		// 像素格式
							IN int nWidth,						// 像素宽度
							IN int nFieldHeight,				// 场高度
							IN NXVIDEO_FIELDCONV_ARITH eArith);	// 转换算法
// 任一幅面至任一幅面的图像拷贝或缩放
// 当源和目的的幅面相同时，直接拷贝；否则进行缩放
typedef void (*pfnvp_zoom)(__out unsigned char* des,			// 目的
						   __in int des_pitch,					// 目的一行的字节数
						   __in int des_x,						// 目的起始操作点
						   __in int des_y,
						   __in int des_width,					// 目的操作幅面
						   __in int des_height,
						   __in unsigned char const* src,		// 源
						   __in int src_pitch,					// 源一行的字节数
						   __in int src_x,						// 源起始操作点
						   __in int src_y,
						   __in int src_width,					// 源操作幅面
						   __in int src_height,
						   __in NXVIDEO_FILTER_TYPE filter,		// 目前只支持 NXVIDEO_FILTER_LINEAR
						   __in NXVIDEO_PIXEL_FMT pixel_fmt);	// 像素格式（源与目的必须相同）
//////////////////////////////////////////////////////////////////////////
// 图像合成
typedef NXRESULT (*pfnvp_blend)(__inout unsigned char* des,			// 底层图像颜色
								__in intptr_t des_pitch,			// 底层图像颜色一行的字节数
								__inout unsigned char* des_alpha,	// 底层图像 A 通道，可以为 NULL
								__in intptr_t des_alpha_pitch,		// 底层图像 A 通道一行的字节数
								__in int des_x,						// 底层图像处理起点坐标
								__in int des_y,
								__in int des_total_width,			// 底层图像总幅面
								__in int des_total_height,
								__in unsigned char* src,			// 顶层图像颜色
								__in intptr_t src_pitch,			// 顶层图像颜色一行的字节数
								__in unsigned char* src_alpha,		// 顶层图像 A 通道，可以为 NULL
								__in intptr_t src_alpha_pitch,		// 顶层图像 A 通道一行的字节数
								__in int src_x,						// 顶层图像处理起点坐标
								__in int src_y,
								__in int src_total_width,			// 顶层图像总幅面
								__in int src_total_height,
								__in NXVIDEO_PIXEL_FMT pixel_fmt,	// 图像像素格式，源与目的相同
								__in NX_BLEND_TYPE blend_type,		// 图像合成类型
								__in int proc_width,				// 处理幅面，源与目的相同
								__in int proc_height);
//////////////////////////////////////////////////////////////////////////
// 从多屏缓冲中生成一屏新缓冲
// 水平方向：src_x 的范围为 [0, count * src_width)
// 垂直方向：src_y 的范围为 [0, count * src_height)
typedef NXRESULT (*pfnvp_pages_roll)(__out unsigned char* des,		// 新屏缓冲
									 __in intptr_t des_pitch,		// 新屏缓冲一行的字节数
									 __in int des_x,				// 新屏缓冲起点
									 __in int des_y,
									 __in int des_width,			// 新屏缓冲幅面
									 __in int des_height,
									 __in unsigned char** src,		// 多屏缓冲数组的首地址
									 __in intptr_t* src_pitch,		// 多屏缓冲一行的字节数的数组首地址
									 __in int count,				// 多屏缓冲个数
									 __in int src_width,			// 多屏缓冲每屏的幅面
									 __in int src_height,
									 __in double proc_x,			// 处理区域
									 __in double proc_y,
									 __in int proc_width,
									 __in int proc_height,
									 __in NXVIDEO_PIXEL_FMT pixel_fmt,
									 __in NXVIDEO_SAM_WINDOW window,
									 __in NXVIDEO_SAM_TAPS taps,
									 __in NXVE_ORIENTATION_TYPE orientation);

//////////////////////////////////////////////////////////////////////////
typedef struct  tag_nxvideoconvapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	// 视频格式转换器的创建与释放
	DefCreateFmtConv		pfnCreateFmtConv;
	DefReleaseFmtConv		pfnReleaseFmtConv;
	// 简单变速器的创建与释放
	DefCreateSpeedProc		pfnCreateSpeedProc;
	DefReleaseSpeedProc		pfnReleaseSpeedProc;
	// 幅面适配与裁剪
	DefAspectAdaptCut		pfnAspectAdaptCut;
	// 变换
	DefDeInterlacing		pfnDeInterlacing;
	DefSetMotionDetectCoef	pfnSetMotionDetectCoef;		// 要求调用者不使用该接口
	DefResampling			pfnResampling;
	// 拆场与合场
	DefSplitFrame			pfnSplitFrame;
	DefComposeFields		pfnComposeFields;
	// 帧率换算
	DefGetOutDurationFRC	pfnGetOutDurationFRC;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.7.0
	// 变速功能接口类的创建与释放
	DefCreateSpeedFunc		pfnCreateSpeedFunc;
	DefReleaseSpeedFunc		pfnReleaseSpeedFunc;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// 顶场与底场间的互相转换函数
	DefFieldConv			pfnFieldConv;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.8
	pfnvp_zoom				zoom;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.21
	pfnvp_blend				blend;		// 图像合成
	// V1.1.0.3
	pfnvp_create_fmtconvfunc	create_fmtconvfunc;
	pfnvp_release_fmtconvfunc	release_fmtconvfunc;
	pfnvp_pages_roll		pages_roll;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从这里开始
} NXVideoConvAPI, *LPNXVIDEOCONVAPI;


// 颜色变换

typedef struct  tag_nxvideocolorapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	// 统一接口
	DefColorConvFrame		pfnColorConvFrame;
	DefColorConvSplit		pfnColorConvSplit;
	DefColorConvCompose		pfnColorConvCompose;
	// YUVA <-> YUYV/UYVY
	DefYUV16_2_YUVA_Frame	pfnYUV16_2_YUVA_Frame;
	DefYUV16_2_YUVA_Split	pfnYUV16_2_YUVA_Split;
	DefYUV16_2_YUVA_Compose	pfnYUV16_2_YUVA_Compose;
	DefYUVA_2_YUV16_Frame	pfnYUVA_2_YUV16_Frame;
	DefYUVA_2_YUV16_Split	pfnYUVA_2_YUV16_Split;
	DefYUVA_2_YUV16_Compose	pfnYUVA_2_YUV16_Compose;
	// YUV <-> YUYV/UYVY
	DefYUV16_2_YUV_Frame	pfnYUV16_2_YUV_Frame;
	DefYUV16_2_YUV_Split	pfnYUV16_2_YUV_Split;
	DefYUV16_2_YUV_Compose	pfnYUV16_2_YUV_Compose;
	DefYUV_2_YUV16_Frame	pfnYUV_2_YUV16_Frame;
	DefYUV_2_YUV16_Split	pfnYUV_2_YUV16_Split;
	DefYUV_2_YUV16_Compose	pfnYUV_2_YUV16_Compose;
	// RGB/BGR/YUV <-> RGBA/BGRA/YUVA
	DefCr24_2_Cr32_Frame	pfnCr24_2_Cr32_Frame;
	DefCr24_2_Cr32_Split	pfnCr24_2_Cr32_Split;
	DefCr24_2_Cr32_Compose	pfnCr24_2_Cr32_Compose;
	DefCr32_2_Cr24_Frame	pfnCr32_2_Cr24_Frame;
	DefCr32_2_Cr24_Split	pfnCr32_2_Cr24_Split;
	DefCr32_2_Cr24_Compose	pfnCr32_2_Cr24_Compose;
	// YUYV/UYVY <-> RGBA/BGRA
	DefYUV16_2_RGB32_Frame		pfnYUV16_2_RGB32_Frame;
	DefYUV16_2_RGB32_Split		pfnYUV16_2_RGB32_Split;
	DefYUV16_2_RGB32_Compose	pfnYUV16_2_RGB32_Compose;
	DefRGB32_2_YUV16_Frame		pfnRGB32_2_YUV16_Frame;
	DefRGB32_2_YUV16_Split		pfnRGB32_2_YUV16_Split;
	DefRGB32_2_YUV16_Compose	pfnRGB32_2_YUV16_Compose;
	// YUYV/UYVY <-> RGB/BGR
	DefYUV16_2_RGB24_Frame		pfnYUV16_2_RGB24_Frame;
	DefYUV16_2_RGB24_Split		pfnYUV16_2_RGB24_Split;
	DefYUV16_2_RGB24_Compose	pfnYUV16_2_RGB24_Compose;
	DefRGB24_2_YUV16_Frame		pfnRGB24_2_YUV16_Frame;
	DefRGB24_2_YUV16_Split		pfnRGB24_2_YUV16_Split;
	DefRGB24_2_YUV16_Compose	pfnRGB24_2_YUV16_Compose;
	// RGBA/BGRA <-> YUVA
	DefYUVA_2_RGB32_Frame		pfnYUVA_2_RGB32_Frame;
	DefYUVA_2_RGB32_Split		pfnYUVA_2_RGB32_Split;
	DefYUVA_2_RGB32_Compose		pfnYUVA_2_RGB32_Compose;
	DefRGB32_2_YUVA_Frame		pfnRGB32_2_YUVA_Frame;
	DefRGB32_2_YUVA_Split		pfnRGB32_2_YUVA_Split;
	DefRGB32_2_YUVA_Compose		pfnRGB32_2_YUVA_Compose;
	// RGB/BGR <-> YUVA
	DefYUVA_2_RGB24_Frame		pfnYUVA_2_RGB24_Frame;
	DefYUVA_2_RGB24_Split		pfnYUVA_2_RGB24_Split;
	DefYUVA_2_RGB24_Compose		pfnYUVA_2_RGB24_Compose;
	DefRGB24_2_YUVA_Frame		pfnRGB24_2_YUVA_Frame;
	DefRGB24_2_YUVA_Split		pfnRGB24_2_YUVA_Split;
	DefRGB24_2_YUVA_Compose		pfnRGB24_2_YUVA_Compose;
	// YUV <-> RGBA/BGRA
	DefYUV_2_RGB32_Frame		pfnYUV_2_RGB32_Frame;
	DefYUV_2_RGB32_Split		pfnYUV_2_RGB32_Split;
	DefYUV_2_RGB32_Compose		pfnYUV_2_RGB32_Compose;
	DefRGB32_2_YUV_Frame		pfnRGB32_2_YUV_Frame;
	DefRGB32_2_YUV_Split		pfnRGB32_2_YUV_Split;
	DefRGB32_2_YUV_Compose		pfnRGB32_2_YUV_Compose;
	// YUV <-> RGB/BGR
	DefYUV_2_RGB24_Frame		pfnYUV_2_RGB24_Frame;
	DefYUV_2_RGB24_Split		pfnYUV_2_RGB24_Split;
	DefYUV_2_RGB24_Compose		pfnYUV_2_RGB24_Compose;
	DefRGB24_2_YUV_Frame		pfnRGB24_2_YUV_Frame;
	DefRGB24_2_YUV_Split		pfnRGB24_2_YUV_Split;
	DefRGB24_2_YUV_Compose		pfnRGB24_2_YUV_Compose;
	// RGBA <-> BGRA
	DefRGB32_SwapRB_Frame		pfnRGB32_SwapRB_Frame;
	DefRGB32_SwapRB_Split		pfnRGB32_SwapRB_Split;
	DefRGB32_SwapRB_Compose		pfnRGB32_SwapRB_Compose;
	// RGB <-> BGR
	DefRGB24_SwapRB_Frame		pfnRGB24_SwapRB_Frame;
	DefRGB24_SwapRB_Split		pfnRGB24_SwapRB_Split;
	DefRGB24_SwapRB_Compose		pfnRGB24_SwapRB_Compose;
	// UYVY <-> YUYV
	DefYUV16_Swap_Frame			pfnYUV16_Swap_Frame;
	DefYUV16_Swap_Split			pfnYUV16_Swap_Split;
	DefYUV16_Swap_Compose		pfnYUV16_Swap_Compose;
	// YUV422 <-> YUYV/UYVY
	DefYUVSlice_2_YUV16_Frame	pfnYUV422_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV422_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV422_2_YUV16_Compose;
	DefYUV16_2_YUVSlice_Frame	pfnYUV16_2_YUV422_Frame;
	DefYUV16_2_YUVSlice_Split	pfnYUV16_2_YUV422_Split;
	DefYUV16_2_YUVSlice_Compose	pfnYUV16_2_YUV422_Compose;
	// YUV420 <-> YUYV/UYVY
	DefYUVSlice_2_YUV16_Frame	pfnYUV420_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV420_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV420_2_YUV16_Compose;
	DefYUV16_2_YUVSlice_Frame	pfnYUV16_2_YUV420_Frame;
	DefYUV16_2_YUVSlice_Split	pfnYUV16_2_YUV420_Split;
	DefYUV16_2_YUVSlice_Compose	pfnYUV16_2_YUV420_Compose;
	// YUV422 <-> YUVA
	DefYUVSlice_2_YUVA_Frame	pfnYUV422_2_YUVA_Frame;
	DefYUVSlice_2_YUVA_Split	pfnYUV422_2_YUVA_Split;
	DefYUVSlice_2_YUVA_Compose	pfnYUV422_2_YUVA_Compose;
	DefYUVA_2_YUVSlice_Frame	pfnYUVA_2_YUV422_Frame;
	DefYUVA_2_YUVSlice_Split	pfnYUVA_2_YUV422_Split;
	DefYUVA_2_YUVSlice_Compose	pfnYUVA_2_YUV422_Compose;
	// YUV420 <-> YUVA
	DefYUVSlice_2_YUVA_Frame	pfnYUV420_2_YUVA_Frame;
	DefYUVSlice_2_YUVA_Split	pfnYUV420_2_YUVA_Split;
	DefYUVSlice_2_YUVA_Compose	pfnYUV420_2_YUVA_Compose;
	DefYUVA_2_YUVSlice_Frame	pfnYUVA_2_YUV420_Frame;
	DefYUVA_2_YUVSlice_Split	pfnYUVA_2_YUV420_Split;
	DefYUVA_2_YUVSlice_Compose	pfnYUVA_2_YUV420_Compose;
	// YUV422 <-> YUV
	DefYUVSlice_2_YUV_Frame		pfnYUV422_2_YUV_Frame;
	DefYUVSlice_2_YUV_Split		pfnYUV422_2_YUV_Split;
	DefYUVSlice_2_YUV_Compose	pfnYUV422_2_YUV_Compose;
	DefYUV_2_YUVSlice_Frame		pfnYUV_2_YUV422_Frame;
	DefYUV_2_YUVSlice_Split		pfnYUV_2_YUV422_Split;
	DefYUV_2_YUVSlice_Compose	pfnYUV_2_YUV422_Compose;
	// YUV420 <-> YUV
	DefYUVSlice_2_YUV_Frame		pfnYUV420_2_YUV_Frame;
	DefYUVSlice_2_YUV_Split		pfnYUV420_2_YUV_Split;
	DefYUVSlice_2_YUV_Compose	pfnYUV420_2_YUV_Compose;
	DefYUV_2_YUVSlice_Frame		pfnYUV_2_YUV420_Frame;
	DefYUV_2_YUVSlice_Split		pfnYUV_2_YUV420_Split;
	DefYUV_2_YUVSlice_Compose	pfnYUV_2_YUV420_Compose;
	// YUV422 <-> RGBA/BGRA
	DefYUVSlice_2_RGB32_Frame	pfnYUV422_2_RGB32_Frame;
	DefYUVSlice_2_RGB32_Split	pfnYUV422_2_RGB32_Split;
	DefYUVSlice_2_RGB32_Compose	pfnYUV422_2_RGB32_Compose;
	DefRGB32_2_YUVSlice_Frame	pfnRGB32_2_YUV422_Frame;
	DefRGB32_2_YUVSlice_Split	pfnRGB32_2_YUV422_Split;
	DefRGB32_2_YUVSlice_Compose	pfnRGB32_2_YUV422_Compose;
	// YUV420 <-> RGBA/BGRA
	DefYUVSlice_2_RGB32_Frame	pfnYUV420_2_RGB32_Frame;
	DefYUVSlice_2_RGB32_Split	pfnYUV420_2_RGB32_Split;
	DefYUVSlice_2_RGB32_Compose	pfnYUV420_2_RGB32_Compose;
	DefRGB32_2_YUVSlice_Frame	pfnRGB32_2_YUV420_Frame;
	DefRGB32_2_YUVSlice_Split	pfnRGB32_2_YUV420_Split;
	DefRGB32_2_YUVSlice_Compose	pfnRGB32_2_YUV420_Compose;
	// YUV422 <-> RGB/BGR
	DefYUVSlice_2_RGB24_Frame	pfnYUV422_2_RGB24_Frame;
	DefYUVSlice_2_RGB24_Split	pfnYUV422_2_RGB24_Split;
	DefYUVSlice_2_RGB24_Compose	pfnYUV422_2_RGB24_Compose;
	DefRGB24_2_YUVSlice_Frame	pfnRGB24_2_YUV422_Frame;
	DefRGB24_2_YUVSlice_Split	pfnRGB24_2_YUV422_Split;
	DefRGB24_2_YUVSlice_Compose	pfnRGB24_2_YUV422_Compose;
	// YUV420 <-> RGB/BGR
	DefYUVSlice_2_RGB24_Frame	pfnYUV420_2_RGB24_Frame;
	DefYUVSlice_2_RGB24_Split	pfnYUV420_2_RGB24_Split;
	DefYUVSlice_2_RGB24_Compose	pfnYUV420_2_RGB24_Compose;
	DefRGB24_2_YUVSlice_Frame	pfnRGB24_2_YUV420_Frame;
	DefRGB24_2_YUVSlice_Split	pfnRGB24_2_YUV420_Split;
	DefRGB24_2_YUVSlice_Compose	pfnRGB24_2_YUV420_Compose;
	// YUV420 <-> YUV422 之 UV 变换
	DefYUVSlice_ConvertUV_Frame		pfnYUV422_2_YUV420_UV_Frame;
	DefYUVSlice_ConvertUV_Split		pfnYUV422_2_YUV420_UV_Split;
	DefYUVSlice_ConvertUV_Compose	pfnYUV422_2_YUV420_UV_Compose;
	DefYUVSlice_ConvertUV_Frame		pfnYUV420_2_YUV422_UV_Frame;
	DefYUVSlice_ConvertUV_Split		pfnYUV420_2_YUV422_UV_Split;
	DefYUVSlice_ConvertUV_Compose	pfnYUV420_2_YUV422_UV_Compose;
	// RGBA/BGRA/YUVA -> Channel
	DefCr32_Split_Channel_Frame		pfnCr32_Split_Channel_Frame;
	DefCr32_Split_Channel_Split		pfnCr32_Split_Channel_Split;
	DefCr32_Split_Channel_Compose	pfnCr32_Split_Channel_Compose;
	//////////////////////////////////////////////////////////////////////////
	// 新版本接口从最后开始声明
} NXVideoColorAPI, *LPNXVIDEOCOLORAPI;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 接口结构定义

typedef struct tag_nxvideoinnerapi *LPNXVIDEOINNERAPI;

typedef struct tag_nxvideoprocapi
{
	// 视频图像变换接口
	LPNXVIDEOCONVAPI		pStuVideoConvAPI;
	// 颜色变换接口
	LPNXVIDEOCOLORAPI		pStuVideoColorAPI;
	// Private 权限，用于优化库内部的接口
	LPNXVIDEOINNERAPI		pStuInnerAPI;
} NXVideoProcAPI, *LPNXVIDEOPROCAPI;


#endif		// #ifndef __NX_VIDEO_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of File.
