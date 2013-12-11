//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 视频格式变换器虚接口类声明.
//				可用于制式转换、上下变换等.
//
// Author: LaiShoubo @ 2013-01-08 Chengdu
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_FMT_CONV_FUNC_H__
#define __I_NX_VIDEO_FMT_CONV_FUNC_H__

#include ".\INXVideoFmtConv.h"
#include ".\INXVideoSpeedFunc.h"


//////////////////////////////////////////////////////////////////////////
// 接口类声明
interface INXVideoFmtConvFunc
{
	// 设置获取输入信号格式（包括幅面、高标清、幅面比例、像素格式、帧率、场播放序、扫描方式）
	// （1）基于视频格式的特点，请输入符合标准的幅面值，否则变换器可能不予响应。
	// （2）高标清用于颜色空间变换，高标清的区别在于同一种信号制式下的分辨率区别。
	// （3）eAspect 变量用于适配计算，高清一般为 16:9，标清一般为 4:3。
	// （4）如果将输入输出的帧率设成一样，则只处理幅面变化，即上下变换。如果输入输出的帧率
	// 不同，则会同时处理时空的变化，如 PAL 与 NTSC 之间的变换。
	// （5）当需要处理时间变化时，视频帧的场播放序是计算时序映射的关键，极大的影响变换质量。
	// （6）支持截取视频部分区域处理
	// 注意：SD50i、HD59.94i、HD50i 按顶场先播，而 SD59.94i 则按底场先播。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，可能已初始化
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	//			NXFMTCONV_RET_NONSUPPORT		输入参数有效但不支持
	virtual int SetInputVideoFmt(IN int nX,
								 IN int nY,
								 IN int nWidth,
								 IN int nHeight,
								 IN int nVideoWidth,
								 IN int nVideoHeight,
								 IN NXVIDEO_DEFINITION eDefinition,
								 IN NXVIDEO_ASPECT eAspect,
								 IN NXVIDEO_PIXEL_FMT ePixelFmt,
								 IN double dFrameRate,
								 IN bool bTopFieldFirst,
								 IN NXVIDEO_SCAN eScan = NXVIDEO_SCAN_INTERLACED) = 0;
	virtual int GetInputVideoFmt(OUT int& nX,
								 OUT int& nY,
								 OUT int& nWidth,
								 OUT int& nHeight,
								 OUT int& nVideoWidth,
								 OUT int& nVideoHeight,
								 OUT NXVIDEO_DEFINITION& eDefinition,
								 OUT NXVIDEO_ASPECT& eAspect,
								 OUT NXVIDEO_PIXEL_FMT& ePixelFmt,
								 OUT double& dFrameRate,
								 OUT bool& bTopFieldFirst,
								 OUT NXVIDEO_SCAN& eScan) const = 0;
	// 设置获取输出信号格式（包括幅面、高标清、幅面比例、像素格式、帧率、场播放序、扫描方式）
	// （1）基于视频格式的特点，请输入符合标准的幅面值，否则变换器可能不予响应。
	// （2）高标清用于颜色空间变换，高标清的区别在于同一种信号制式下的分辨率区别。
	// （3）eAspect 变量用于适配计算，高清一般为 16:9，标清一般为 4:3。
	// （4）如果将输入输出的帧率设成一样，则只处理幅面变化，即上下变换。
	// 如果输入输出的帧率不同，则会同时处理时空的变化，如 PAL 与 NTSC 之间的变换。
	// （5）当需要处理时间变化时，视频帧的场播放序是计算时序映射的关键，极大的影响变换质量。
	// （6）支持截取视频部分区域处理
	// 注意：SD50i、HD59.94i、HD50i 按顶场先播，而 SD59.94i 则按底场先播。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，可能已初始化
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	//			NXFMTCONV_RET_NONSUPPORT		输入参数有效但不支持
	virtual int SetOutputVideoFmt(IN int nX,
								  IN int nY,
								  IN int nWidth,
								  IN int nHeight,
								  IN int nVideoWidth,
								  IN int nVideoHeight,
								  IN NXVIDEO_DEFINITION eDefinition,
								  IN NXVIDEO_ASPECT eAspect,
								  IN NXVIDEO_PIXEL_FMT ePixelFmt,
								  IN double dFrameRate,
								  IN bool bTopFieldFirst,
								  IN NXVIDEO_SCAN eScan = NXVIDEO_SCAN_INTERLACED) = 0;
	virtual int GetOutputVideoFmt(OUT int& nX,
								  OUT int& nY,
								  OUT int& nWidth,
								  OUT int& nHeight,
								  OUT int& nVideoWidth,
								  OUT int& nVideoHeight,
								  OUT NXVIDEO_DEFINITION& eDefinition,
								  OUT NXVIDEO_ASPECT& eAspect,
								  OUT NXVIDEO_PIXEL_FMT& ePixelFmt,
								  OUT double& dFrameRate,
								  OUT bool& bTopFieldFirst,
								  OUT NXVIDEO_SCAN& eScan) const = 0;
	// 设置获取处理参数（包括适配类型、采用窗口、窗口大小、运动补偿、抗混叠、处理方式、下拉变换）
	// 缺省值分别为		NXVIDEO_ADAPTION_HEIGHT（按高度适配）
	//					NXVIDEO_SAM_WINDOW_KAISER（kaiser 窗逼近）
	//					NXVIDEO_SAM_TAPS_8（8-taps 采样）
	//					NXVIDEO_DEINTERLACE_TAPS_4（4-taps 运动补偿）
	//					NXVIDEO_ANTIALIAS_NONE（无抗混叠）
	//					NXFMTCONV_TYPE_MOTION_DETECT（采用运动检测）
	// （1）当 eSamWindow = NXVIDEO_SAM_WINDOW_SONY 时，eSamTaps 只支持到 NXVIDEO_SAM_TAPS_8。
	// （2）eMotionArith 只在 eConvType = NXFMTCONV_TYPE_MOTION_DETECT 时起作用。
	// （3）当 eConvType = NXFMTCONV_TYPE_STATIC_PROGRESSIVE 时，如果输入与输出的场播放序不相同时，将造成错场。
	// （4）如果适配需要填黑边时，对应输出格式的黑色值由参数 bSafeColor 决定是否按安全值处理
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，可能已初始化
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	//			NXFMTCONV_RET_NONSUPPORT		输入参数有效但不支持
	virtual int SetConvParam(IN NXVIDEO_ADAPTION eAdaption,
							 IN NXVIDEO_SAM_WINDOW eSamWindow,
							 IN NXVIDEO_SAM_TAPS eSamTaps,
							 IN NXVIDEO_DEINTERLACE_ARITH eMotionArith,
							 IN NXVIDEO_ANTIALIAS eAntiAlias,
							 IN NXFMTCONV_TYPE eConvType,
							 IN bool bUseTelecine = false,
							 IN bool bSafeColor = true) = 0;
	virtual int GetConvParam(OUT NXVIDEO_ADAPTION& eAdaption,
							 OUT NXVIDEO_SAM_WINDOW& eSamWindow,
							 OUT NXVIDEO_SAM_TAPS& eSamTaps,
							 OUT NXVIDEO_DEINTERLACE_ARITH& eMotionArith,
							 OUT NXVIDEO_ANTIALIAS& eAntiAlias,
							 OUT NXFMTCONV_TYPE& eConvType,
							 OUT bool& bUseTelecine,
							 OUT bool& bSafeColor) const = 0;
	// 注册输入共享缓冲回调接口
	// （1）要求可以按帧号索取数据，并要求去交织时共享缓冲要缓冲住上一帧的数据，否则效率会下降。
	// （2）变换器只会使用输入共享缓冲回调接口中的读取接口对，以获取输入帧数据。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，可能已初始化
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	virtual int RegInputShareBufferCB(IN INXVideoShareBufferCB* pShareCB) = 0;
	// 注册去交织共享缓冲回调接口
	// （1）该功能为提高效率所设，注册成功后，变换器内部会根据情况自行切分流程。
	// （2）变换器会使用共享缓冲回调接口中的读写接口对，以获取可读写的帧对象
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，可能已初始化
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	virtual int RegDeinterlaceShareBufferCB(IN INXVideoShareBufferCB* pTopShareCB,
											IN INXVideoShareBufferCB* pBottomShareCB) = 0;
	// 初始化变换器，如计算缩放系数缓冲
	// 返回		NXFMTCONV_RET_SUCCEED		成功
	//			NXFMTCONV_RET_FAILED		失败，可能已初始化
	//			NXFMTCONV_RET_ALREADY_INIT	变换器已被初始化
	virtual int Initialize() = 0;
	// 计算输入输出映射
	// （1）根据设置的帧率参数计算输出帧对应的输入映射帧
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	virtual int CalcMapping(OUT LPNXSPEEDMAPDATA pMapData,		// 映射输出结构
							IN int nOutFrameIndex) = 0;			// 输出帧相对索引
	// 输出结果一帧数据
	// （1）此接口使用注册的输入缓冲共享回调接口
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能未被初始化
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	virtual int GetOutputFrame(OUT unsigned char* pucBuf,			// 输出目标
							   IN int nPitch,						// 一行信号占用的字节宽度
							   IN LPNXSPEEDMAPDATA pMapData) = 0;	// 映射
	// 输出结果一帧数据
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能未被初始化
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	virtual int GetOutputFrame(OUT unsigned char* pucBuf,			// 输出目标
							   IN int nPitch,						// 一行信号占用的字节宽度
							   IN unsigned char* pucPrevFrame[4],	// 映射对应的输入帧的前一帧
							   IN unsigned char* pucCurrFrame[4],	// 映射对应的输入帧
							   IN int nInFramePitch,				// 输入帧一行对应的字节数
							   IN LPNXSPEEDMAPDATA pMapData) = 0;	// 映射
	// 释放变换器
	// （1）释放变换器内部缓冲，如缩放系数缓冲
	// 返回		NXFMTCONV_RET_SUCCEED		成功
	//			NXFMTCONV_RET_FAILED		失败
	virtual int Release() = 0;
	// 变换器的状态获取
	virtual bool IsInitialized() = 0;
};


#endif		//__I_NX_VIDEO_FMT_CONV_FUNC_H__
//////////////////////////////////////////////////////////////////////////
// end-of-file
