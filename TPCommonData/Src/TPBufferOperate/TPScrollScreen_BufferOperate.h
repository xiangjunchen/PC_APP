/********************************************************************
	Copyright (c) 2004, Sobey数码 内容制作部
	All rights reserved.
	创建时间:	2004年3月4日   15:21
	文件名: 	c:\ndsrc\chenyufeng_desktop_3\devvob\sobeydesktop\src\tpcommon\tpbufferoperate\tpscrollscreen_bufferoperate.h
	当前版本：	1.0
	作者:		陈玉峰
	摘要:		滚屏缓冲区操作定义
*********************************************************************/
#ifndef __TP_SCROLLSCREEN_BUFFER_OPERATE_INC__
#define __TP_SCROLLSCREEN_BUFFER_OPERATE_INC__

// 滚屏滚动模式
#define TP_SCROLLSCREEN_UPMOVE	0	// 上移
#define TP_SCROLLSCREEN_DOWNMOVE 1// 下移
#define TP_SCROLLSCREEN_LEFTMOVE 2	// 左移
#define TP_SCROLLSCREEN_RIGHTMOVE 3 // 右移

// 滚屏上滚
BOOL CALLBACK TPBuffer_ScrollScreen_Up(DWORD* pdwDestBuffer, 
							  int nDestX,
							  int nDestY,
							  int nDestWidth,
							  int nDestHeight,
							  int nDestBufferWidth,
							  int nDestBufferHeight,
							  DWORD* pdwSrcBuffer,
							  int nSrcX,
							  int nSrcY,
							  int nSrcWidth,
							  int nSrcHeight,
							  int nSrcBufferWidth,
							  int nSrcBufferHeight,
							  int nParity,				// 奇偶场模式
							  float fPower,			 // 象素所占权重
							  int nFadeIn,
							  int nFadeOut);
// 滚屏左滚
BOOL CALLBACK TPBuffer_ScrollScreen_Left(DWORD* pdwDestBuffer, 
								int nDestX,
								int nDestY,
								int nDestWidth,
								int nDestHeight,
								int nDestBufferWidth,
								int nDestBufferHeight,
								DWORD* pdwSrcBuffer,
								int nSrcX,
								int nSrcY,
								int nSrcWidth,
								int nSrcHeight,
								int nSrcBufferWidth,
								int nSrcBufferHeight,
								int nParity,				// 奇偶场模式
								float fPower,			 // 象素所占权重
								int nFadeIn,
								int nFadeOut);
// 滚屏下滚
BOOL CALLBACK TPBuffer_ScrollScreen_Down(DWORD* pdwDestBuffer, 
								int nDestX,
								int nDestY,
								int nDestWidth,
								int nDestHeight,
								int nDestBufferWidth,
								int nDestBufferHeight,
								DWORD* pdwSrcBuffer,
								int nSrcX,
								int nSrcY,
								int nSrcWidth,
								int nSrcHeight,
								int nSrcBufferWidth,
								int nSrcBufferHeight,
								int nParity,				// 奇偶场模式
								float fPower,			 // 象素所占权重
								int nFadeIn,
								int nFadeOut);
// 滚屏右滚
BOOL CALLBACK TPBuffer_ScrollScreen_Right(DWORD* pdwDestBuffer, 
								 int nDestX,
								 int nDestY,
								 int nDestWidth,
								 int nDestHeight,
								 int nDestBufferWidth,
								 int nDestBufferHeight,
								 DWORD* pdwSrcBuffer,
								 int nSrcX,
								 int nSrcY,
								 int nSrcWidth,
								 int nSrcHeight,
								 int nSrcBufferWidth,
								 int nSrcBufferHeight,
								 int nParity,				// 奇偶场模式
								 float fPower,			 // 象素所占权重
								 int nFadeIn,
								 int nFadeOut);
// 滚屏的BitBlt操作
LRESULT CALLBACK TP_SCROLLSCREEN_BitBlt(
										IN OUT DWORD* pdwDestBuffer, 
										IN int nDestX,
										IN int nDestY,
										IN int nDestWidth,
										IN int nDestHeight,
										IN int nDestBufferWidth,
										IN int nDestBufferHeight,
										IN DWORD* pdwSrcBuffer,
										IN int nSrcX,
										IN int nSrcY,
										IN int nSrcWidth,
										IN int nSrcHeight,
										IN int nSrcBufferWidth,
										IN int nSrcBufferHeight,
										IN int nMix,			// 混叠模式
										IN int nParity,		// 奇偶场模式
										IN int nScrollType,  // 滚动类型
										IN float fPower,		// 象素所占权重 
										IN int nFadeIn,
										IN int nFadeOut);

#endif // __TP_SCROLLSCREEN_BUFFER_OPERATE_INC__