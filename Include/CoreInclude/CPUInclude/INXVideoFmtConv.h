//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 视频格式变换器虚接口类声明.
//				可用于制式转换、上下变换等.
//
// Created: ZhaoZhimeng @ 2008-11-19
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_FMT_CONV_H__
#define __I_NX_VIDEO_FMT_CONV_H__

#include ".\NXBasic.h"
#include ".\NXVideoProcDef.h"
#include ".\INXVideoShareBufferCB.h"

//////////////////////////////////////////////////////////////////////////
// 返回值定义
enum NXFMTCONV_RET
{
	NXFMTCONV_RET_SUCCEED		= 1,
	NXFMTCONV_RET_FAILED		= 0,

	NXFMTCONV_RET_INVALID_PARAM		= -1,	// 无效输入参数
	NXFMTCONV_RET_NOT_INIT			= -2,	// 变换器未被初始化（例如启动变换）
	NXFMTCONV_RET_ALREADY_INIT		= -3,	// 变换器已被初始化（例如初始化）
	NXFMTCONV_RET_NONSUPPORT		= -4,	// 不支持的输入参数

	NXFMTCONV_RET_IN_FULL		= 2,		// 输入缓冲为满（等待变换中的状态）
	NXFMTCONV_RET_OUT_EMPTY		= 3,		// 输出缓冲为空（等待变换中的状态）
	NXFMTCONV_RET_OUT_END		= 4,		// 输出结束（逐步停止变换后输出最后一帧后的状态）
};

//////////////////////////////////////////////////////////////////////////
// 处理方式定义
enum NXFMTCONV_TYPE
{
	NXFMTCONV_TYPE_MOTION_DETECT,		// 运动检测模式
	NXFMTCONV_TYPE_STATIC_INTERLACED,	// 静帧模式按场处理
	NXFMTCONV_TYPE_STATIC_PROGRESSIVE,	// 静帧模式按帧处理

	NXFMTCONV_TYPE_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////
// 接口类声明
interface NOVTABLE INXVideoFmtConv
{
	// 设置获取输入信号格式（包括幅面、高标清、幅面比例、像素格式、帧率、场播放序）
	// （1）基于视频格式的特点，请输入符合标准的幅面值，否则变换器可能不予响应。
	// （2）高标清用于颜色空间变换，高标清的区别在于同一种信号制式下的分辨率区别。
	// （3）eAspect 变量用于适配计算，高清一般为 16:9，标清一般为 4:3。
	// （4）当 ePixelFmt 设置为 NXVIDEO_PIXEL_FMT_YUV422 或 NXVIDEO_PIXEL_FMT_YUV420 两种格式时，
	// 在调用 SetInputFrame 或使用共享输入缓冲时有严格要求。
	// （5）如果将输入输出的帧率设成一样，则只处理幅面变化，即上下变换。如果输入输出的帧率
	// 不同，则会同时处理时空的变化，如 PAL 与 NTSC 之间的变换。
	// （6）当需要处理时间变化时，视频帧的场播放序是计算时序映射的关键，极大的影响变换质量。
	// 注意：SD50i、HD59.94i、HD50i 按顶场先播，而 SD59.94i 则按底场先播。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能正在运转中
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	//			NXFMTCONV_RET_NONSUPPORT		输入参数有效但不支持
	virtual int SetInputVideoFmt(IN int nWidth,
								 IN int nHeight,
								 IN NXVIDEO_DEFINITION eDefinition,
								 IN NXVIDEO_ASPECT eAspect,
								 IN NXVIDEO_PIXEL_FMT ePixelFmt,
								 IN double dFrameRate,
								 IN bool bTopFieldFirst) = 0;
	virtual int GetInputVideoFmt(OUT int &nWidth,
								 OUT int &nHeight,
								 OUT NXVIDEO_DEFINITION &eDefinition,
								 OUT NXVIDEO_ASPECT &eAspect,
								 OUT NXVIDEO_PIXEL_FMT &ePixelFmt,
								 OUT double &dFrameRate,
								 OUT bool &bTopFieldFirst) = 0;
	// 设置获取输出信号格式（包括幅面、高标清、幅面比例、像素格式、帧率、场播放序）
	// （1）基于视频格式的特点，请输入符合标准的幅面值，否则变换器可能不予响应。
	// （2）高标清用于颜色空间变换，高标清的区别在于同一种信号制式下的分辨率区别。
    // （3）eAspect 变量用于适配计算，高清一般为 16:9，标清一般为 4:3。
    // （4）当 ePixelFmt 设置为 NXVIDEO_PIXEL_FMT_YUV422 或 NXVIDEO_PIXEL_FMT_YUV420 两种格式时，
    // 在调用 GetOutputFrame 或使用共享输出缓冲时有严格要求。
	// （5）如果将输入输出的帧率设成一样，则只处理幅面变化，即上下变换。
	// 如果输入输出的帧率不同，则会同时处理时空的变化，如 PAL 与 NTSC 之间的变换。
	// （6）当需要处理时间变化时，视频帧的场播放序是计算时序映射的关键，极大的影响变换质量。
	// 注意：SD50i、HD59.94i、HD50i 按顶场先播，而 SD59.94i 则按底场先播。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能正在运转中
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	//			NXFMTCONV_RET_NONSUPPORT		输入参数有效但不支持
	virtual int SetOutputVideoFmt(IN int nWidth,
								  IN int nHeight,
								  IN NXVIDEO_DEFINITION eDefinition,
								  IN NXVIDEO_ASPECT eAspect,
								  IN NXVIDEO_PIXEL_FMT ePixelFmt,
								  IN double dFrameRate,
								  IN bool bTopFieldFirst) = 0;
	virtual int GetOutputVideoFmt(OUT int &nWidth,
								  OUT int &nHeight,
								  OUT NXVIDEO_DEFINITION &eDefinition,
								  OUT NXVIDEO_ASPECT &eAspect,
								  OUT NXVIDEO_PIXEL_FMT &ePixelFmt,
								  OUT double &dFrameRate,
								  OUT bool &bTopFieldFirst) = 0;
	// 设置获取处理参数（包括适配类型、采用窗口、窗口大小、运动补偿、抗混叠、处理方式）
	// 缺省值分别为		NXVIDEO_ADAPTION_HEIGHT（按高度适配）
	//					NXVIDEO_SAM_WINDOW_KAISER（kaiser 窗逼近）
	//					NXVIDEO_SAM_TAPS_8（8-taps 采样）
	//					NXVIDEO_DEINTERLACE_TAPS_4（4-taps 运动补偿）
	//					NXVIDEO_ANTIALIAS_NONE（无抗混叠）
	//					NXFMTCONV_TYPE_MOTION_DETECT（采用运动检测）
	// （1）当 eSamWindow = NXVIDEO_SAM_WINDOW_SONY 时，eSamTaps 只支持到 NXVIDEO_SAM_TAPS_8。
	// （2）eMotionArith 只在 eConvType = NXFMTCONV_TYPE_MOTION_DETECT 时起作用。
	// （3）当 eConvType = NXFMTCONV_TYPE_STATIC_PROGRESSIVE 时，如果输入与输出的场播放序不相同时，将造成错场。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能正在运转中
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	//			NXFMTCONV_RET_NONSUPPORT		输入参数有效但不支持
	virtual int SetConvParam(IN NXVIDEO_ADAPTION eAdaption,
							 IN NXVIDEO_SAM_WINDOW eSamWindow,
							 IN NXVIDEO_SAM_TAPS eSamTaps,
							 IN NXVIDEO_DEINTERLACE_ARITH eMotionArith,
							 IN NXVIDEO_ANTIALIAS eAntiAlias,
							 IN NXFMTCONV_TYPE eConvType) = 0;
	virtual int GetConvParam(OUT NXVIDEO_ADAPTION &eAdaption,
							 OUT NXVIDEO_SAM_WINDOW &eSamWindow,
							 OUT NXVIDEO_SAM_TAPS &eSamTaps,
							 OUT NXVIDEO_DEINTERLACE_ARITH &eMotionArith,
							 OUT NXVIDEO_ANTIALIAS &eAntiAlias,
							 OUT NXFMTCONV_TYPE &eConvType) = 0;
	// 注册输入共享缓冲回调接口
	// （1）该功能为提高效率所设，注册成功后，变换器内部会根据情况自行切分流程。
	// （2）一旦注册回调成功，就无需主动调用 SetInputFrame 接口。
	// （3）输入共享缓冲数据的定义与 SetInputFrame 接口相同。
	// （4）要求可以按帧号索取数据，并要求共享缓冲要缓冲住上一帧的数据，否则效率会下降。
	// （5）变换器只会使用输入共享缓冲回调接口中的读取接口对，以获取输入帧数据。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能正在运转中
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	virtual int RegInputShareBufferCB(IN INXVideoShareBufferCB *pShareCB) = 0;
	// 注册输出共享缓冲回调接口
	// （1）该功能为提高效率所设，注册成功后，变换器内部会根据情况自行切分流程。
	// （2）一旦注册回调成功，就无需主动调用 GetOutputFrame 接口。
	// （3）输出共享缓冲数据的定义与 GetOutputFrame 接口相同。
	// （4）变换器只会使用输出共享缓冲回调接口中的写入接口对，以获取可写入的对帧对象，并输出数据。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能正在运转中
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	virtual int RegOutputShareBufferCB(IN INXVideoShareBufferCB *pShareCB) = 0;
	// 设置获取输入输出缓冲大小（缺省都为 6 帧，上限为 30 帧，输入下限为 2 帧）
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能正在运转中
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	virtual int SetBufferRingSize(IN int nInput, IN int nOutput) = 0;
	virtual int GetBufferRingSize(OUT int &nInput, OUT int &nOutput) = 0;
	// 初始化变换器（设置处理流程、计算处理参数等）
	// 返回		NXFMTCONV_RET_SUCCEED		成功
	//			NXFMTCONV_RET_FAILED		失败，该变换器可能正在运转中
	//			NXFMTCONV_RET_ALREADY_INIT	变换器已被初始化
	virtual int Initialize() = 0;
	// 启动变换器
	// 返回		NXFMTCONV_RET_SUCCEED		成功
	//			NXFMTCONV_RET_FAILED		失败，该变换器可能正在运转中
	//			NXFMTCONV_RET_NOT_INIT		变换器未被初始化
	virtual int StartConv() = 0;
	// 输入源一帧数据
	// （1）启动变换器后，要求从第 0 帧开始、从小到大的输入。
	// （2）当输入信号格式 ePixelFmt = NXVIDEO_PIXEL_FMT_YUV422 || NXVIDEO_PIXEL_FMT_YUV420 时，
	// 要求输入数据严格按照 Y 一片、U 一片、V 一片顺序存储，如同解码器输出的 RAW_DATA，
	// 此时 nPitch 表示 Y 一行占用的字节宽度，而 U/V 的行占用字节宽度为 Y 的一半。
	// （3）要求输入图像的屏幕左上角点与内存的起点重合，否则会造成效果错误。
	// （4）当注册输入共享缓冲成功后，建议不主动调用该接口，可能会失效。
	// （5）当 pucBuf = NULL 时，nFrameIndex 会返回变换器当前的需求帧号，
	// 如果返回 nFrameIndex = -1 则表示当前变换器的需求为空，需要调用者等待。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能未被启动
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	//			NXFMTCONV_RET_IN_FULL			输入缓冲满（当设置 bWaitConv = false 时，可能返回该值）
	virtual int SetInputFrame(IN unsigned char *pucBuf,			// 输入数据
							  IN int nPitch,					// 一行信号占用的字节宽度
							  IN OUT int &nFrameIndex,			// 输入帧号（当 pucBuf = NULL 时会被填充）
							  IN bool bWaitConv = true) = 0;	// 输入缓冲满时是否进行等待（false 则立即返回）
	// 输出结果一帧数据
	// （1）当 pucBuf = NULL 时，变换器认为调用者要丢弃一帧的结果，如果它存在的话。
    // （2）当注册输出共享缓冲成功后，建议不主动调用该接口，可能会失效。
    // （3）当输出信号格式 ePixelFmt = NXVIDEO_PIXEL_FMT_YUV422 || NXVIDEO_PIXEL_FMT_YUV420 时，
    // 要求输入数据严格按照 Y 一片、U 一片、V 一片顺序存储，如同解码器输出的 RAW_DATA，
    // 此时 nPitch 表示 Y 一行占用的字节宽度，而 U/V 的行占用字节宽度为 Y 的一半。
	// 返回		NXFMTCONV_RET_SUCCEED			成功
	//			NXFMTCONV_RET_FAILED			失败，该变换器可能未被启动
	//			NXFMTCONV_RET_INVALID_PARAM		输入参数无效
	//			NXFMTCONV_RET_OUT_EMPTY			输出缓冲为空（当设置 bWaitConv = false 时，可能返回该值）
	//			NXFMTCONV_RET_OUT_END			变换结束（只会在停止变换后才返回该值）
	virtual int GetOutputFrame(OUT unsigned char *pucBuf,		// 输出目标
							   IN int nPitch,					// 一行信号占用的字节宽度
							   OUT int &nFrameIndex,			// 输出帧号
							   IN bool bWaitConv = true) = 0;	// 输出缓冲空时是否进行等待（false 则立即返回）
	// 停止变换器
	// （1）当 bForced = false 时，该接口只设置停止标志，而变换线程实际可能仍在进行，直到输入缓冲为空。
	// （2）当 bForced = false 时，要求继续 GetOutputFrame，直到返回变换结束，否则可能导致线程阻塞而无法停止。
	// （3）当 bForced = true 时，直接设置输出结束标志，并在最近的一次变换结束后，立刻停止变换器。
	// 返回		NXFMTCONV_RET_SUCCEED		成功
	//			NXFMTCONV_RET_FAILED		失败，该变换器可能未被启动
	virtual int StopConv(IN bool bForced = false) = 0;
	// 释放变换器
	// （1）如果变换器已被设置停止标志，但变换仍在进行时，会进行强制停止。
	// 返回		NXFMTCONV_RET_SUCCEED		成功
	//			NXFMTCONV_RET_FAILED		失败，该变换器可能未被停止
	virtual int Release() = 0;
	// 变换器的状态获取
	virtual bool IsInitialized() = 0;
	virtual bool IsProcessing() = 0;	// 停止变换器后，该状态可能仍会存在一段时间
	// 设置电视电影处理标志
	// （1）默认情况下，内部将按线性帧率映射进行处理
	// （2）电影胶片信号（24FPS）转换为电视信号（PAL、NTSC）时，推荐使用电视电影处理以得到更高质量
	virtual void SetTelecine(IN bool bUseTelecine) = 0;
	virtual bool GetTelecine() const = 0;
	// 设置视频信号扫描方式
	// 默认均为 NXVIDEO_SCAN_INTERLACED
	virtual void SetScanType(IN NXVIDEO_SCAN bInputScan, IN NXVIDEO_SCAN bOutputScan) = 0;
	virtual void GetScanType(OUT NXVIDEO_SCAN& bInputScan, OUT NXVIDEO_SCAN& bOutputScan) const = 0;
};


#endif		// #ifndef __I_NX_VIDEO_FMT_CONV_H__
//////////////////////////////////////////////////////////////////////////
// End of file.