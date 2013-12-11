//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 简单变速虚接口类声明.
//
// Created: ZhaoZhimeng @ 2009-02-13
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_SPEED_PROC_H__
#define __I_NX_VIDEO_SPEED_PROC_H__

#include ".\NXBasic.h"
#include ".\NXVideoProcDef.h"
#include ".\INXVideoShareBufferCB.h"

//////////////////////////////////////////////////////////////////////////
// 返回值定义
enum NXSPEED_PROC_RET
{
	NXSPEED_PROC_RET_SUCCEED		= 1,
	NXSPEED_PROC_RET_FAILED			= 0,

	NXSPEED_PROC_RET_INVALID_PARAM	= -1,		// 无效输入参数
	NXSPEED_PROC_RET_NOT_INIT		= -2,		// 变速器未被初始化（例如进行变换）
	NXSPEED_PROC_RET_ALREADY_INIT	= -3,		// 变速器已被初始化（例如设置参数）
	NXSPEED_PROC_RET_NONSUPPORT		= -4,		// 不支持的输入参数
	NXSPEED_PROC_RET_NOSHARE_INPUT	= -5,		// 初始化时未注册输入共享缓冲

	NXSPEED_PROC_RET_OUT_END		= 2,		// 主动模式下，输出结束标志
	NXSPEED_PROC_RET_OUT_EMPTY		= 3,		// 主动模式下，输出为空标志
};

//////////////////////////////////////////////////////////////////////////
// 变速运行模式定义
enum NXSPEED_PROC_MODE
{
	NXSPEED_PROC_MODE_SEEK,			// 被动运行模式，需要使用者驱动变换流程
	NXSPEED_PROC_MODE_PLAY,			// 主动运行模式，内部自动产生驱动变换信号

	NXSPEED_PROC_MODE_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////
// 处理算法定义
// FilterOff			按场计算映射，直接拷贝场数据
// FieldCut				按场计算映射，顶场映射顶场或底场映射底场时直接拷贝场数据，否则进行插值
// FieldMix				按场计算映射，采用前后两场进行混合插值
// FrameCut				按帧计算映射，直接拷贝帧数据
// FrameMix				按帧计算映射，采用前后两帧进行混合插值
// Still1st				静止模式，固定映射为源素材的第一场，如果位置映射不匹配时，进行插值
// Still2nd				静止模式，固定映射为源素材的第二场，如果位置映射不匹配时，进行插值
// StillFrame			静止模式，固定映射为源素材的第一帧
// MotionAdaptiveCut	在 FieldCut 模式中添加运动补偿
// MotionAdaptiveMix	在 FieldMix 模式中添加运动补偿
enum NXSPEED_PROC_ARITH
{
	NXSPEED_PROC_ARITH_FILTER_OFF,			// FilterOff
	NXSPEED_PROC_ARITH_FIELD_CUT,			// FieldCut
	NXSPEED_PROC_ARITH_FIELD_MIX,			// FieldMix
	NXSPEED_PROC_ARITH_FRAME_CUT,			// FrameCut
	NXSPEED_PROC_ARITH_FRAME_MIX,			// FrameMix
	NXSPEED_PROC_ARITH_STILL_FIRST,			// Still1st
	NXSPEED_PROC_ARITH_STILL_SECOND,		// Still2nd
	NXSPEED_PROC_ARITH_STILL_FRAME,			// StillFrame
	NXSPEED_PROC_ARITH_MOTION_ADAPTIVE_CUT,	// MotionAdaptiveCut
	NXSPEED_PROC_ARITH_MOTION_ADAPTIVE_MIX,	// MotionAdaptiveMix

	NXSPEED_PROC_ARITH_UNKNOWN,
};


//////////////////////////////////////////////////////////////////////////
// 接口类声明
interface NOVTABLE INXVideoSpeedProc
{
	// 设置获取变速处理参数（包括源素材长度、幅面、像素格式、变速模式、速率、场播放时序）
	// （1）源素材长度必须大于 0，一般非编中限制一个素材最长为 12 个小时。
	// （2）ePixelFmt 如果为 NXVIDEO_PIXEL_FMT_YUV422 或 NXVIDEO_PIXEL_FMT_YUV420 格式，
	// 要求输入数据严格按照 Y 一片、U 一片、V 一片顺序存储，如同解码器输出的 RAW_DATA，
	// 此时 nPitch 表示 Y 一行占用的字节宽度，而 U/V 的行占用字节宽度为 Y 的一半。
	// （3）为了确保处理正确，速度绝对值最小支持到 1e-6，当速度小于 0 时，将从源素材的倒数第二场开始计算映射。
	// （4）不同视频制式的场播放序有差异，SD50i、HD59.94i、HD50i 按顶场先播，而 SD59.94i 则按底场先播。
	// （5）只有在正放时 nInClipLength 参数才能输入小于 0 的值，此时表明输入总长未知（如采集中），
	// （6）nFrameWidth、nFrameHeight 均代表全部区域，实际处理区域由 SetProcArea 接口设置
	// 变速器启动后将处于永动状态，直到使用者调用 SetInClipLength() 接口改变输入总长。
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，该变速器可能正在运转中
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	//			NXSPEED_PROC_RET_NONSUPPORT			输入参数有效但不支持
	virtual int SetProcParam(IN int nInClipLength,
							 IN int nFrameWidth,
							 IN int nFrameHeight,
							 IN NXVIDEO_PIXEL_FMT ePixelFmt,
							 IN NXSPEED_PROC_MODE eProcMode,
							 IN NXSPEED_PROC_ARITH eProcArith,
							 IN double dSpeed,
							 IN bool bTopFieldFirst) = 0;
	virtual int GetProcParam(OUT int &nInClipLength,
							 OUT int &nFrameWidth,
							 OUT int &nFrameHeight,
							 OUT NXVIDEO_PIXEL_FMT &ePixelFmt,
							 OUT NXSPEED_PROC_MODE &eProcMode,
							 OUT NXSPEED_PROC_ARITH &eProcArith,
							 OUT double &dSpeed,
							 OUT bool &bTopFieldFirst) = 0;
	// 注册输入共享缓冲回调接口
	// （1）由于变速算法的独特性，对输入的需求完全由算法自身掌控，因此为了简化算法流程，
	// 以及提高算法效率，采用输入共享缓冲实现获取输入。
	// （2）要求可以按帧号索取数据，并要求共享缓冲要缓冲住上一帧的数据，否则效率会下降。
	// （3）变速器只会使用输入共享缓冲回调接口中的读取接口对，以获取输入帧数据。
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，该变速器可能正在运转中
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	virtual int RegInputShareBufferCB(IN INXVideoShareBufferCB *pShareCB) = 0;
	// 注册输出共享缓冲回调接口
	// （1）该功能为提高主动模式的效率所设，注册成功后，就无需主动调用 GetOutputFrame 接口。
	// （2）在被动模式下，该功能无效。
	// （3）变速器只会使用输出共享缓冲回调接口中的写入接口对，以获取可写入的对帧对象，并输出数据。
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，该变速器可能正在运转中
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	virtual int RegOutputShareBufferCB(IN INXVideoShareBufferCB *pShareCB) = 0;
	// 设置获取内部缓冲大小（缺省都为 6 帧，上限为 30 帧，下限为 2 帧）
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，该变速器可能正在运转中
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	virtual int SetBufferRingSize(IN int nRingSize) = 0;
	virtual int GetBufferRingSize(OUT int &nRingSize) = 0;
	// 初始化变速器
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，该变速器可能正在运转中
	//			NXSPEED_PROC_RET_NOSHARE_INPUT		未注册输入共享缓冲
	virtual int Initialize() = 0;
	// 获取变速后的素材长度
	// （1）目标素材长度等于 (int)(nInClipLength / fabs(dSpeed) + 0.4999999998)，采用四舍五入的方式。
	// （2）当正放时 nInClipLength 参数输入小于 0，则 nOutClipLength 返回也小于 0。
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_NOT_INIT			变速器未被初始化
	virtual int GetOutClipLength(OUT int &nOutClipLength) = 0;
	// 启动变速器
	// 返回		NXSPEED_PROC_RET_SUCCEED		成功
	//			NXSPEED_PROC_RET_FAILED			失败，该变速器可能正在运转中
	//			NXSPEED_PROC_RET_NOT_INIT		变速器未被初始化
	virtual int StartConv() = 0;
	// 输出结果一帧数据
	// （1）变速器内部可能存在变换线程，因此可能需要等待。
    // （2）在被动模式下，nFrameIndex 为输入参数，即指定需要输出的结果帧索引。
    // （3）当参数 bWaitConv = true 时，任何模式下都会等待处理结束后才返回。
	// （4）在主动模式下，如果注册了输出共享缓冲回调接口，该接口失效。
	// （5）在主动模式下，nFrameIndex 为输出参数，为当前输出结果的帧索引。
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，变速器未被初始化或其它原因
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	//			NXSPEED_PROC_RET_OUT_EMPTY			主动模式下，输出缓冲为空（当设置 bWaitConv = false 时，可能返回该值）
	//			NXSPEED_PROC_RET_OUT_END			主动模式下，输出结束标志
	virtual int GetOutputFrame(OUT unsigned char *pucBuf,		// 输出目标
							   IN int nPitch,					// 一行信号占用的字节宽度
							   IN OUT int &nFrameIndex,			// 输出帧号
							   IN bool bWaitConv = true) = 0;	// 是否进行变换等待（false 则立即返回）
	// 停止变速器
	// 返回		NXFMTCONV_RET_SUCCEED		成功
	//			NXFMTCONV_RET_FAILED		失败，该变速器可能未被启动
	virtual int StopConv() = 0;
	// 释放变速器
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，该变速器可能未被初始化
	virtual int Release() = 0;
	// 变速器的状态获取
	virtual bool IsInitialized() = 0;
	virtual bool IsProcessing() = 0;	// 主动模式下采用输出共享缓冲时，可用于判断变速器是否已经停止（即变速结束）。
	// 变速器运行过程中修改速度
	// （1）扩展型接口，为简单变速器增加阶段型变速支持。
	// （2）由于变速器以超实时状态运转，已渲染的帧无法进行清除，则会造成延时效应，即速度变了，但响应滞后。
	// （3）静帧型算法不支持速度修改。
	// （4）如果速度的符号发生变化，将很可能导致变速器流程死锁，请慎重使用。
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，该变速器可能未在运转
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	//			NXSPEED_PROC_RET_NONSUPPORT			输入参数有效但不支持
	virtual int ChangeSpeed(IN double dNewSpeed,		// 新速度
							IN int nOutFrameStart) = 0;	// 新速度发生作用的起始帧
	// 变速器运行过程中设置新的输入总长
	// （1）扩展型接口，为简单变速器增加采集模式支持，即输入总长未知。
	// （2）变速器会根据新的输入总长即时更新输出总长。
	// （3）如果速度为负，则 nInClipLength 不能为负，即倒放永远是有限帧长。
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，该变速器可能未在运转
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	virtual int SetInClipLength(IN int nInClipLength) = 0;	// 新输入总长
	// 设置实际处理区域信息
	// 全部区域信息由 SetProcParam 接口设置
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_FAILED				失败，处理区域无效
	virtual int SetProcArea(IN int nProcX,				// 区域起点，相对于图像左上角
							IN int nProcY,
							IN int nProcWidth,			// 区域宽高
							IN int nProcHeight) = 0;
	virtual int GetProcArea(IN int& nProcX,				// 区域起点，相对于图像左上角
							IN int& nProcY,
							IN int& nProcWidth,			// 区域宽高
							IN int& nProcHeight) = 0;
	// 去隔行算法类型，默认为 NXVIDEO_DEINTERLACE_TAPS_4
	virtual void SetDeinterlaceArith(IN NXVIDEO_DEINTERLACE_ARITH eArith) = 0;
	virtual NXVIDEO_DEINTERLACE_ARITH GetDeinterlaceArith() const = 0;
};


#endif		// #ifndef __I_NX_VIDEO_SPEED_PROC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
