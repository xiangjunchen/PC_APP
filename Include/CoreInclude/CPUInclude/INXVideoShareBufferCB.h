//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: 视频共享缓冲回调虚接口类声明.
//
// Created: ZhaoZhimeng @ 2008-02-16
//////////////////////////////////////////////////////////////////////////
#ifndef __I_NX_VIDEO_SHARE_BUFFER_CB_H__
#define __I_NX_VIDEO_SHARE_BUFFER_CB_H__


//////////////////////////////////////////////////////////////////////////
// 编译声明

#ifndef IN
#define IN
#endif		// #ifndef IN
#ifndef OUT
#define OUT
#endif		// #ifndef OUT


//////////////////////////////////////////////////////////////////////////
// 返回值定义
enum NXVIDEO_SHARE_RET
{
	NXVIDEO_SHARE_RET_SUCCEED	= 1,
	NXVIDEO_SHARE_RET_FAILED	= 0,

	NXVIDEO_SHARE_RET_INPUT_END = 3,		// 在 LockReadUnit 中返回该值表明输入结束
};


//////////////////////////////////////////////////////////////////////////
// 共享缓冲回调接口类声明
// （1）为了使用回调者的实现方便，将读取接口对与写入接口对放在一个回调接口类中进行声明。
// 但一般情况下，一个共享缓冲回调接口对象只会有一对接口被使用到。
// （2）读取接口对与写入接口对是相互独立的两对接口，两者之间没有任何的联系。
interface INXVideoShareBufferCB
{
    // 设置需要保留的可读取缓冲单元个数
	// （1）共享缓冲回调在大多数情况下会被顺序地使用，即缓冲符合先入先出的特性。
	// （2）在读取某帧 nFrameIndex 缓冲时，向前需要保留到第 (nFrameIndex - nReserved) 帧缓冲，
	// 更前的缓冲则可以被丢弃。
	// （3）当 nReserved = -1 时，读取缓冲的帧号将会是无序的。
	// 返回		NXVIDEO_SHARE_RET_SUCCEED		成功
	//			NXVIDEO_SHARE_RET_FAILED		失败
    virtual int SetReadReservedUnits(IN int nReserved) = 0;
	// 获取可读取的某帧缓冲
	// 返回		NXVIDEO_SHARE_RET_SUCCEED		成功
	//			NXVIDEO_SHARE_RET_FAILED		失败
	//			NXVIDEO_SHARE_RET_INPUT_END		输入结束
    virtual int LockReadUnit(OUT unsigned char* &pucBuf,	// 帧缓冲起始地址
                             OUT int &nPitch,				// 帧缓冲一行所占用的字节数
                             IN int nFrameIndex) = 0;		// 帧号
    virtual int UnlockReadUnit(IN unsigned char *pucBuf,	// 帧缓冲起始地址
                               IN int nFrameIndex) = 0;		// 帧号
	// 获取可写入的某帧缓冲
	// （1）LockWriteUnit 接口中当 nFrameIndex < 0 时，表示获取任意一个可写入的缓冲。
	// 当 nFrameIndex >= 0 时，回调实现者可以视自身情况决定是否使用该参数。
	// （2）UnlockWriteUnit 接口中当 nFrameIndex < 0 时，表示写入帧无效。
	// 返回		NXVIDEO_SHARE_RET_SUCCEED		成功
	//			NXVIDEO_SHARE_RET_FAILED		失败
    virtual int LockWriteUnit(OUT unsigned char* &pucBuf,	// 帧缓冲起始地址
							  OUT int &nPitch,				// 帧缓冲一行所占用的字节数
                              IN int nFrameIndex) = 0;		// 帧号
    virtual int UnlockWriteUnit(IN unsigned char *pucBuf,	// 帧缓冲起始地址
                                IN int nFrameIndex) = 0;	// 帧号
};


#endif		// #ifndef __I_NX_VIDEO_SHARE_BUFFER_CB_H__
//////////////////////////////////////////////////////////////////////////
// End of file.