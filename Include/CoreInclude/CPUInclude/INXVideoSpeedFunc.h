//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 变速处理中的功能函数虚接口类.
//
// Created: ZhaoZhimeng @ 2010-05-24
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_SPEED_FUNC_H__
#define __I_NX_VIDEO_SPEED_FUNC_H__

#include ".\NXBasic.h"
#include ".\INXVideoSpeedProc.h"

//////////////////////////////////////////////////////////////////////////
// 场映射定义（out = left * d + right * (1 - d)）
typedef struct tag_nxspeedfieldmap
{
	double					dInFramePos;		// 映射的真实位置
	int						nLeftFrameIndex;	// 左场映射的源帧号
	NXVIDEO_FIELD_PLAY		eLeftFieldIndex;	// 左场映射在源帧中的场序
	int						nRightFrameIndex;	// 右场映射的源帧号
	NXVIDEO_FIELD_PLAY		eRightFieldIndex;	// 右场映射在源帧中的场序
	double					dLeftFieldWeight;	// 左场映射的权重
} NXSpeedFieldMap, *LPNXSPEEDFIELDMAP;
// 帧内按场映射定义
typedef struct tag_nxspeedmapdata
{
	int					nOutFrameIndex;			// 输出帧号
	NXSpeedFieldMap		stuFirstFieldMap;		// 第一场映射
	NXSpeedFieldMap		stuSecondFieldMap;		// 第二场映射
} NXSpeedMapData, *LPNXSPEEDMAPDATA;

//////////////////////////////////////////////////////////////////////////
// 接口类声明
interface NOVTABLE INXVideoSpeedFunc
{
	// 设置不变化的处理参数（包括幅面、像素格式、变速模式、场播放时序）
	// （1）ePixelFmt 如果为 NXVIDEO_PIXEL_FMT_YUV422 或 NXVIDEO_PIXEL_FMT_YUV420 格式，
	// 要求输入数据严格按照 Y 一片、U 一片、V 一片顺序存储，如同解码器输出的 RAW_DATA，
	// 此时 nPitch 表示 Y 一行占用的字节宽度，而 U/V 的行占用字节宽度为 Y 的一半。
	// （2）不同视频制式的场播放序有差异，SD50i、HD59.94i、HD50i 按顶场先播，而 SD59.94i 则按底场先播。
	// （3）nFrameWidth、nFrameHeight 均代表全部区域，实际处理区域由 SetProcArea 接口设置
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	virtual int SetProcParam(int nFrameWidth,
							 int nFrameHeight,
							 NXVIDEO_PIXEL_FMT ePixelFmt,
							 NXSPEED_PROC_ARITH eProcArith,
							 bool bTopFieldFirst) = 0;
	// 注册运动检测运算所需要的临时缓冲（只有在算法为 MotionAdaptiveCut 和 MotionAdaptiveMix 时需要）
	// 由于会被频繁使用，请事先分配好，假设有 N 个运动检测线程在并行，则有
	// pFrameCB:
	// （1）可被同时读写的帧临时缓冲个数为 4 * N；
	// （2）每帧缓冲的大小为 nFrameWidth * nFrameHeight * nPixelBytes；
	// 特殊情况为 YUV420 格式被转换为 YUV422 格式进行处理，而 YUV422 又将 Y、U、V 分开处理，
	// 因此这两种格式的临时缓冲大小可为 nFrameWidth * nFrameHeight。
	// pCoefCB:
	// （1）可被同时读写的系数临时缓冲个数为 N；
	// （2）每个缓冲的大小为 nFrameWidth * nPixelBytes * sizeof(short)，即系数在运算时以 short 类型存在；
	// 特殊情况为 YUV422、YUV420 两种分片格式，将 Y、U、V 分开处理，则大小可为 nFrameWidth * sizeof(short)。
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	virtual int RegMotionCalcShareCB(INXVideoShareBufferCB *pFrameCB,		// 帧临时缓冲
									 INXVideoShareBufferCB *pCoefCB) = 0;	// 系数临时缓冲
	// 注册 YUV420 格式所需变换的中间临时缓冲（只有像素为 YUV420 时才需要）
	// 由于会被频繁使用，请事先分配好，假设有 N 个运动检测线程、M 个输出线程在并行，则有
	// （1）可被同时读写的缓冲个数为 N * 2 + M；
	// （2）每介缓冲的大小为 nFrameWidth * nFrameHeight。
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	virtual int RegUV22MidShareCB(INXVideoShareBufferCB *pUV22CB) = 0;

	// 获取参数
	virtual int GetFrameWidth() = 0;
	virtual int GetFrameHeight() = 0;
	virtual int GetFieldHeight() = 0;
	virtual NXVIDEO_PIXEL_FMT GetPixelFmt() = 0;
	virtual NXSPEED_PROC_ARITH GetProcArith() = 0;
	// 检测状态
	virtual bool IsTopFieldFirst() = 0;		// 判断是否为顶场先播
	virtual bool IsStillArith() = 0;		// 判断是否为静帧算法
	virtual bool IsNeedMotion() = 0;		// 判断是否需要运动检测算法
	virtual bool IsFieldArith() = 0;		// 判断是否为按场处理算法
	virtual bool IsReverseSpeed(double dSpeed) = 0;		// 倒放
	virtual bool IsSpeedValid(double dSpeed) = 0;	// 判断速度是否有效

	//////////////////////////////////////////////////////////////////////////
	// 映射计算
	// 返回		NXSPEED_PROC_RET_SUCCEED			成功
	//			NXSPEED_PROC_RET_INVALID_PARAM		输入参数无效
	//////////////////////////////////////////////////////////////////////////
	// 计算输出输入映射
	// （1）输出映射结构的同时，返回下一帧输出到输入的映射起始点 dInFramePosNext。
	// （2）当 bIntegralMode 参数为 true 时表示进行积分型映射计算，使用 dInFramePosStart
	// 参数及当前速度累加出当前帧的映射；为 false 时则直接按简单变速方式计算。
	virtual int CalcMapping(LPNXSPEEDMAPDATA pMapData,	// 映射输出结构
							double &dInFramePosStart,	// 当前帧输出映射的输入起始点
							int nOutFrameIndex,			// 输出帧相对索引
							int nInClipLength,			// 输入总帧长
							bool bMapField,				// 按场映射标志
							bool bIntegralMode,			// 积分型计算模式
							double dSpeed) = 0;			// 速度
	// 根据速度的变化、下一帧映射的输入起始位置、已输出的帧长计算最新的输出总帧长度
	// （1）当 dNewSpeed < -1e^-6 时，令 nOutLength = 0 && dInFramePosStart = nInClipLength 则表示计算输出总帧长。
	// （2）当 dNewSpeed > 1e^-6 时，令 nOutLength = 0 && dInFramePosStart = 0 则表示计算输出总帧长。
	virtual int UpdateOutClipLength(int &nOutLength,			// 已输出的帧长
									double dInFramePosStart,	// 下一帧映射的输入起始位置
									double dNewSpeed,			// 新速度
									int nInClipLength) = 0;		// 输入总帧长

	//////////////////////////////////////////////////////////////////////////
	// 运动检测
	// （1）输入两帧，输出也是两帧。
	// （2）算法接口不做参数安全检测。
	// （3）内部需要的参数由最初设置，如幅面、算法类型、像素格式、场序。
	// 返回		NXSPEED_PROC_RET_SUCCEED		成功
	//			NXSPEED_PROC_RET_FAILED			失败
	//////////////////////////////////////////////////////////////////////////
	virtual int MotionFrame(unsigned char *pucTopFrame,			// 输出顶场帧
							intptr_t nTopFramePitch,			// 输出一行所占用的字节数
							unsigned char *pucBottomFrame,		// 输出底场帧
							intptr_t nBottomFramePitch,			// 输出一行所占用的字节数
							unsigned char *pucCurrFrame,		// 输入当前帧
							intptr_t nCurrFramePitch,			// 输入一行所占用的字节数
							unsigned char *pucPrevFrame,		// 输入前一帧
							intptr_t nPrevFramePitch,			// 输入一行所占用的字节数
							int nLineProcBytes,					// 一行处理的字节数
							NXVIDEO_PIXEL_FMT ePixelFmtIn) = 0;	// 输入帧的像素格式

	//////////////////////////////////////////////////////////////////////////
	// 渲染输出
	// （1）算法接口不做参数安全检测。
	// （2）内部需要的参数由最初设置，如幅面、算法类型、像素格式、场序。
	// 返回		NXSPEED_PROC_RET_SUCCEED		成功
	//			NXSPEED_PROC_RET_FAILED			失败
	//////////////////////////////////////////////////////////////////////////
	// 整帧静态渲染，支持 NXSPEED_PROC_ARITH_STILL_FIRST、NXSPEED_PROC_ARITH_STILL_SECOND、
	// NXSPEED_PROC_ARITH_STILL_FRAME 三种算法，其它情况返回失败。
	virtual int StillArith(unsigned char *pucOutFrame,			// 输出帧起始
						   intptr_t nOutPitch,					// 输出一行所占用的字节数
						   unsigned char *pucInFrame,			// 输入帧起始
						   intptr_t nInPitch,					// 输入一行所占用的字节数
						   int nLineProcBytes,					// 一行处理的字节数
						   NXVIDEO_PIXEL_FMT ePixelFmtIn) = 0;	// 输入帧的像素格式
	// 单场 FilterOff 与 FieldCut 渲染
	// 需要注意，输入输出地址都为帧数据的起始点，而非场数据的起始点
	virtual int FilterOffFieldCut(unsigned char *pucOutFrame,	// 输出帧起始
								  intptr_t nOutPitch,			// 输出一行所占用的字节数
								  bool bOutTopField,			// 处理顶场标志
								  unsigned char *pucInFrame,	// 输入帧起始
								  intptr_t nInPitch,			// 输入一行所占用的字节数
								  bool bInMapTopField,			// 映射为输入顶场的标志
								  bool bInputShare,				// 输入帧未做转换标志
								  bool bFilterOff,				// FilterOff 算法标志
								  int nLineProcBytes) = 0;		// 一行处理的字节数
	// 单场 FieldMix 渲染
	// 需要注意，输入输出地址都为帧数据的起始点，而非场数据的起始点
	virtual int FieldMix(unsigned char *pucOutFrame,		// 输出帧起始
						 intptr_t nOutPitch,				// 输出一行所占用的字节数
						 bool bOutTopField,					// 处理顶场标志
						 unsigned char *pucInLeftFrame,		// 输入左帧起始
						 intptr_t nInLeftPitch,				// 输入一行所占用的字节数
						 bool bInLeftTopField,				// 映射为输入顶场的标志
						 unsigned char *pucInRightFrame,	// 输入右帧起始
						 intptr_t nInRightPitch,			// 输入一行所占用的字节数
						 bool bInRightTopField,				// 映射为输入顶场的标志
						 bool bInputShare,					// 输入帧未做转换标志
						 double dLeftFieldWeight,			// 左场权重
						 int nLineProcBytes) = 0;			// 一行处理的字节数
	// 整帧 FrameCut 渲染
	// 需要注意，输入输出地址都为帧数据的起始点，而非场数据的起始点
	// pucInPrevFrame 参数在 bReverseSpeed 参数值为 false 时（即正放）无效
	virtual int FrameCut(unsigned char *pucOutFrame,	// 输出帧起始
						 intptr_t nOutPitch,			// 输出一行所占用的字节数
						 unsigned char *pucInFrame,		// 输入帧起始
						 intptr_t nInPitch,				// 输入一行所占用的字节数
						 unsigned char *pucInPrevFrame,	// 输入帧起始
						 intptr_t nInPrevPitch,			// 输入一行所占用的字节数
						 bool bReverseSpeed,			// 倒放标志
						 int nLineProcBytes) = 0;		// 一行处理的字节数
	// 正放时整帧 && 倒放时单场 FrameMix 渲染
	// 需要注意，输入输出地址都为帧数据的起始点，而非场数据的起始点
	// bOutTopField 参数在 bReverseSpeed 参数值为 false 时（即正放）无效
	virtual int FrameMix(unsigned char *pucOutFrame,		// 输出帧起始
						 intptr_t nOutPitch,				// 输出一行所占用的字节数
						 bool bOutTopField,					// 处理顶场标志
						 unsigned char *pucInLeftFrame,		// 输入左帧起始
						 intptr_t nInLeftPitch,				// 输入一行所占用的字节数
						 unsigned char *pucInRightFrame,	// 输入右帧起始
						 intptr_t nInRightPitch,			// 输入一行所占用的字节数
						 bool bReverseSpeed,				// 倒放标志
						 double dLeftFieldWeight,			// 左帧权重
						 int nLineProcBytes) = 0;			// 一行处理的字节数
	// 单场 MotionAdaptiveCut 渲染
	// 需要注意，输入输出地址都为帧数据的起始点，而非场数据的起始点
	virtual int MotionCut(unsigned char *pucOutFrame,	// 输出帧起始
						  intptr_t nOutPitch,			// 输出一行所占用的字节数
						  bool bOutTopField,			// 处理顶场标志
						  unsigned char *pucInFrame,	// 输入帧起始
						  intptr_t nInPitch,			// 输入一行所占用的字节数
						  int nLineProcBytes) = 0;		// 一行处理的字节数
	// 单场 MotionAdaptiveMix 渲染
	// 需要注意，输入输出地址都为帧数据的起始点，而非场数据的起始点
	virtual int MotionMix(unsigned char *pucOutFrame,		// 输出帧起始
						  intptr_t nOutPitch,				// 输出一行所占用的字节数
						  bool bOutTopField,				// 处理顶场标志
						  unsigned char *pucInLeftFrame,	// 输入左帧起始
						  intptr_t nInLeftPitch,			// 输入一行所占用的字节数
						  unsigned char *pucInRightFrame,	// 输入右帧起始
						  intptr_t nInRightPitch,			// 输入一行所占用的字节数
						  double dLeftFieldWeight,			// 左场权重
						  int nLineProcBytes) = 0;			// 一行处理的字节数
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
	virtual void SetDeinterlaceArith(NXVIDEO_DEINTERLACE_ARITH eArith) = 0;
	virtual NXVIDEO_DEINTERLACE_ARITH GetDeinterlaceArith() const = 0;
};


#endif		// #ifndef __I_NX_VIDEO_SPEED_FUNC_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
