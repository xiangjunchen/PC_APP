/********************************************************************
	Copyright (c) 2004, Sobey数码 内容制作部
	All rights reserved.
	创建时间:	2004年3月4日   15:18
	文件名: 	c:\ndsrc\chenyufeng_desktop_3\devvob\sobeydesktop\src\tpcommon\tpbufferoperate\tpscrollscreen_bufferoperate.cpp
	当前版本：	1.0
	作者:		陈玉峰
	摘要:		滚屏缓冲区操作的实现
*********************************************************************/
#include "stdAfx.h"
#include "TPScrollScreen_BufferOperate.h"
#include "TPBufferOperateAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// 全局变量


//////////////////////////////////////////////////////////////////////////
// 私有函数定义
BOOL TPBuffer_ScrollScreen_Mix(DWORD* pdwDestBuffer, 
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
							   int nParity,				 // 奇偶场模式
							   int nScrollType,			 // 滚动类型
							   float fPower,			 // 象素所占权重
							   int nFadeIn,				 // 淡入宽
							   int nFadeOut);			 // 淡出宽
BOOL TPBuffer_ScrollScreen_NotMix(DWORD* pdwDestBuffer, 
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
								  int nParity,				 // 奇偶场模式
								  int nScrollType,			 // 滚动类型
								  float fPower,			 // 象素所占权重
								  int nFadeIn,				 // 淡入宽
								  int nFadeOut);			 // 淡出宽


//////////////////////////////////////////////////////////////////////////
// 导出函数
//////////////////////////////////////////////////////////////////////////
// 功能：滚屏播出时的BitBlt操作
// 参数：
// 返回值：是否成功
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_SCROLLSCREEN_BitBlt(
										IN OUT DWORD* pdwDestBuffer, // 目标buffer
										IN int nDestX,				 // 要操作的目标起点坐标
										IN int nDestY,
										IN int nDestWidth,			 // 要操作的目标宽、高
										IN int nDestHeight,
										IN int nDestBufferWidth,	 // 目标buffer宽、高 
										IN int nDestBufferHeight,
										IN DWORD* pdwSrcBuffer,		 // 源Buffer
										IN int nSrcX,				 // 要操作的源起点坐标				
										IN int nSrcY,
										IN int nSrcWidth,			 // 要操作的源宽、高
										IN int nSrcHeight,
										IN int nSrcBufferWidth,		 // 要操作的源Buffer宽、高
										IN int nSrcBufferHeight,
										IN int nMix,				 // 混叠模式
										IN int nParity,				 // 奇偶场模式
										IN int nScrollType,		     // 滚动类型
										IN float fPower,			 // 象素所占权重
										IN int nFadeIn,
										IN int nFadeOut)
{
	if(nSrcWidth == nDestWidth&&nSrcHeight == nDestHeight)
	{
		switch(nMix) 
		{
		case TP_BUFFER_COPY:
			switch(nScrollType)
			{
			case TP_SCROLLSCREEN_UPMOVE:
				return TPBuffer_ScrollScreen_Up(pdwDestBuffer,
					nDestX,
					nDestY,
					nDestWidth,
					nDestHeight,
					nDestBufferWidth,
					nDestBufferHeight,
					pdwSrcBuffer,
					nSrcX,
					nSrcY,
					nSrcWidth,
					nSrcHeight,
					nSrcBufferWidth,
					nSrcBufferHeight,
					nParity,
					fPower,
					nFadeIn,
					nFadeOut);
			case TP_SCROLLSCREEN_DOWNMOVE:
				return TPBuffer_ScrollScreen_Down(pdwDestBuffer,
					nDestX,
					nDestY,
					nDestWidth,
					nDestHeight,
					nDestBufferWidth,
					nDestBufferHeight,
					pdwSrcBuffer,
					nSrcX,
					nSrcY,
					nSrcWidth,
					nSrcHeight,
					nSrcBufferWidth,
					nSrcBufferHeight,
					nParity,
					fPower,
					nFadeIn,
					nFadeOut);
			case TP_SCROLLSCREEN_LEFTMOVE:
				return TPBuffer_ScrollScreen_Left(pdwDestBuffer,
					nDestX,
					nDestY,
					nDestWidth,
					nDestHeight,
					nDestBufferWidth,
					nDestBufferHeight,
					pdwSrcBuffer,
					nSrcX,
					nSrcY,
					nSrcWidth,
					nSrcHeight,
					nSrcBufferWidth,
					nSrcBufferHeight,
					nParity,
					fPower,
					nFadeIn,
					nFadeOut);
			case TP_SCROLLSCREEN_RIGHTMOVE:
				return TPBuffer_ScrollScreen_Right(pdwDestBuffer,
					nDestX,
					nDestY,
					nDestWidth,
					nDestHeight,
					nDestBufferWidth,
					nDestBufferHeight,
					pdwSrcBuffer,
					nSrcX,
					nSrcY,
					nSrcWidth,
					nSrcHeight,
					nSrcBufferWidth,
					nSrcBufferHeight,
					nParity,
					fPower,
					nFadeIn,
					nFadeOut);
			default:
				return E_FAIL;
			}
		case TP_BUFFER_MIX:
			return TPBuffer_ScrollScreen_Mix(pdwDestBuffer,
				nDestX,
				nDestY,
				nDestWidth,
				nDestHeight,
				nDestBufferWidth,
				nDestBufferHeight,
				pdwSrcBuffer,
				nSrcX,
				nSrcY,
				nSrcWidth,
				nSrcHeight,
				nSrcBufferWidth,
				nSrcBufferHeight,
				nParity,
				nScrollType,
				fPower,
				nFadeIn,
				nFadeOut);
		case TP_BUFFER_NOTMIX:
			return TPBuffer_ScrollScreen_NotMix(pdwDestBuffer,
				nDestX,
				nDestY,
				nDestWidth,
				nDestHeight,
				nDestBufferWidth,
				nDestBufferHeight,
				pdwSrcBuffer,
				nSrcX,
				nSrcY,
				nSrcWidth,
				nSrcHeight,
				nSrcBufferWidth,
				nSrcBufferHeight,
				nParity,
				nScrollType,
				fPower,
				nFadeIn,
				nFadeOut);
		default:
			return E_FAIL;
		}
	}
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
// 私有函数实现
// 滚屏操作
//////////////////////////////////////////////////////////////////////////
// 功能：按滚动方式不同拷贝源Buffer到目标Buffer指定位置
// 算法：上滚时CA = CA*fPower+DA*(1-fPower),其中CA为当前象素Alpha值，DA为下一行象素点Alpha值，当最下一行为空，则DA为SRC中最上面一行象素Alpha值
//		 淡入淡出：CA = CA*(j*255/(nFadeOut-1))/255;其中j为当前点y值
// 参数：
// 返回值：是否拷贝成功
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_ScrollScreen_Copy(DWORD* pdwDestBuffer, // 目标Buffer
								int nDestX,						  // 要操作的目标起点
								int nDestY,
								int nDestWidth,					  // 要操作的目标宽、高
								int nDestHeight,
								int nDestBufferWidth,			  // 目标Buffer宽、高
								int nDestBufferHeight,
								DWORD* pdwSrcBuffer,				  // 源Buffer
								int nSrcX,						  // 要操作的源起点
								int nSrcY,
								int nSrcWidth,					  // 要操作的源宽、高
								int nSrcHeight,
								int nSrcBufferWidth,				  // 源Buffer宽、高
								int nSrcBufferHeight,
								int nParity,				 // 奇偶场模式
								int nScrollType,			 // 滚动类型
								float fPower,			 // 象素所占权重
								int nFadeIn,				 // 淡入宽
								int nFadeOut)			 // 淡出宽
{
	switch(nScrollType) 
	{
	case TP_SCROLLSCREEN_UPMOVE:
		return TPBuffer_ScrollScreen_Up(pdwDestBuffer, // 目标Buffer
			nDestX,						  // 要操作的目标起点
			nDestY,
			nDestWidth,					  // 要操作的目标宽、高
			nDestHeight,
			nDestBufferWidth,			  // 目标Buffer宽、高
			nDestBufferHeight,
			pdwSrcBuffer,				  // 源Buffer
			nSrcX,						  // 要操作的源起点
			nSrcY,
			nSrcWidth,					  // 要操作的源宽、高
			nSrcHeight,
			nSrcBufferWidth,				  // 源Buffer宽、高
			nSrcBufferHeight,
			nParity,				 // 奇偶场模式
			fPower,			 // 象素所占权重
			nFadeIn,				 // 淡入宽
			nFadeOut);
	case TP_SCROLLSCREEN_DOWNMOVE:
		break;
	case TP_SCROLLSCREEN_LEFTMOVE:
		return TPBuffer_ScrollScreen_Left(pdwDestBuffer, // 目标Buffer
			nDestX,						  // 要操作的目标起点
			nDestY,
			nDestWidth,					  // 要操作的目标宽、高
			nDestHeight,
			nDestBufferWidth,			  // 目标Buffer宽、高
			nDestBufferHeight,
			pdwSrcBuffer,				  // 源Buffer
			nSrcX,						  // 要操作的源起点
			nSrcY,
			nSrcWidth,					  // 要操作的源宽、高
			nSrcHeight,
			nSrcBufferWidth,				  // 源Buffer宽、高
			nSrcBufferHeight,
			nParity,				 // 奇偶场模式
			fPower,			 // 象素所占权重
			nFadeIn,				 // 淡入宽
			nFadeOut);
	case TP_SCROLLSCREEN_RIGHTMOVE:
		break;
	default:
		return FALSE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
// 功能：按照奇偶场Mix源Buffer与目标Buffer
// 算法：上滚时CA = CA*fPower+DA*(1-fPower),其中CA为当前象素Alpha值，DA为下一行象素点Alpha值，当最下一行为空，则DA为SRC中最上面一行象素Alpha值
//		 Mix算法：KA=SA+DA-SA*DA/255; DR = DR+(SR-DR)*SA/KA
// 创建时间：2004－03－02，未实现源与目标的Mix操作
// 返回值：是否成功
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_ScrollScreen_Mix(DWORD* pdwDestBuffer, 
							   int nDestX,						  // 要操作的目标起点
							   int nDestY,
							   int nDestWidth,					  // 要操作的目标宽、高
							   int nDestHeight,
							   int nDestBufferWidth,			  // 目标Buffer宽、高
							   int nDestBufferHeight,
							   DWORD* pdwSrcBuffer,				  // 源Buffer
							   int nSrcX,						  // 要操作的源起点
							   int nSrcY,
							   int nSrcWidth,					  // 要操作的源宽、高
							   int nSrcHeight,
							   int nSrcBufferWidth,				  // 源Buffer宽、高
							   int nSrcBufferHeight,
							   int nParity,				 // 奇偶场模式
							   int nScrollType,			 // 滚动类型
							   float fPower,			 // 象素所占权重
							   int nFadeIn,				 // 淡入宽
							   int nFadeOut)			 // 淡出宽
{
	if(pdwDestBuffer == NULL||pdwSrcBuffer == NULL)
		return TRUE;

	// 数据范围检测
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// 得到起点坐标和要拷贝的长宽
	if(nDestX < 0)
	{
		nDestWidth += nDestX;
		nSrcX -= nDestX;
		nDestX = 0;
	}
	if(nDestY < 0)
	{
		nDestHeight += nDestY;
		nSrcY -= nDestY;
		nDestY = 0;
	}

	if(nSrcX < 0)
	{
		nDestWidth += nSrcX;
		nDestX -= nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nDestHeight += nSrcY;
		nDestY -= nSrcY;
		nSrcY = 0;
	}
	// 上滚时，宽度不够就被截掉,高度不够时则从头开始取
	int nTempWidth = nSrcX+nDestWidth;
	if(nTempWidth > nSrcBufferWidth)
		nDestWidth -= (nTempWidth-nSrcBufferWidth);
	nTempWidth = nDestX+nDestWidth;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);

	int nTempHeight = nDestY+nDestHeight;
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// 每次加的宽度
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// 计数器的增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// 一次一行
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// 一次两行
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nDestY % 2 == 0)	// 起点为奇场
	{
		if(nParity == TP_PARITY_EVEN)
		{
			// 如果偶场拷贝，则从下一行开始
			if(nSrcY >= nSrcBufferHeight)
			{
				nSrcY = 0;
				pSrc = pdwSrcBuffer + nSrcX;
			}
			else
				pSrc += nSrcBufferWidth;			// 源移到下一行，如果下一行为空，则移到源的第一行
			nDestHeight--;
			if(nDestY >= nDestBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;				// 目标移到下一行
		}
	}
	else
	{
		// 起点为偶场
		if(nParity == TP_PARITY_ODD)
		{
			// 如果奇场拷贝，则从下一行开始
			nDestHeight--;
			if(nSrcY >= nSrcBufferHeight)
			{
				nSrcY = 0;
				pSrc = pdwSrcBuffer + nSrcX;
			}
			else
				pSrc += nSrcBufferWidth;		// 源移到下一行，如果下一行为空，则移到源的第一行
			if(nDestY >= nDestBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;			// 目标移到下一行
		}
	}

	// 源缓冲区最后一行的下一行的指针
	DWORD* pdwLast = pdwSrcBuffer + nSrcBufferWidth*nSrcBufferHeight;
	DWORD* pdwFirst = pdwSrcBuffer + nSrcX;								// 首行指针
	WORD nPower = (WORD)(fPower*255);
	WORD nSubPower = (WORD)(255-nPower);
	int nSrcLineBytes = nSrcBufferWidth<<2;						// 一行的字节数
	_declspec(align(16)) WORD narPower[] = {nPower, nPower, nPower, nPower, nPower, nPower, nPower, nPower};
	_declspec(align(16)) WORD narSubPower[] = {nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower};
	_declspec(align(16)) DWORD dwFade[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	WORD* pdFadeOut = new WORD[nFadeOut];			// 淡出缓冲区
	int i=0;
	if(nFadeOut == 1)
		pdFadeOut[0] = 128;
	else
	{
		for(i=0; i<nFadeOut; i++)
		{
			pdFadeOut[i] = (WORD)((i/(float)(nFadeOut-1))*255);
		}
	}
	WORD* pdFadeIn = new WORD[nFadeIn];		// 淡入缓冲区
	int nFadeInStart = nDestHeight - nFadeIn;	// 淡入区起始点
	if(nFadeIn == 1)
		pdFadeIn[0] = 128;
	else
	{
		for(i=0; i<nFadeIn; i++)
		{
			pdFadeIn[i] = (WORD)((i/(float)(nFadeIn-1))*255);
		}
	}

	__asm
	{
		XOR ECX, ECX
			PXOR XMM7, XMM7
			MOVDQA XMM6, narPower
			MOVDQA XMM5, narSubPower
			MOV EAX, pSrc			// 源指针
			MOV EDX, pDes
LOOPY:
		CMP ECX, nDestHeight	// y<nDestHeight
			JGE LOOPYEND
			CMP EAX, pdwLast		// pSrc<pdwLast
			JL  LOOPYSRC
			MOV EAX, pdwFirst		// 如果pSrc>=pdwLast,则pSrc=pdwFirst
LOOPYSRC:
		MOV ESI, EAX			// 下一行指针
			ADD ESI, nSrcLineBytes
			CMP ESI, pdwLast
			JL LOOPYNEXT
			MOV ESI, pdwFirst
LOOPYNEXT:
		PUSH EDX
			PUSH EAX
			MOVDQA XMM4, dwFade		// 找到淡入淡出值:默认为0F0F0F0F0F0F0F0F
			CMP ECX, nFadeOut
			JGE  FADEOUT
			MOV EDI, ECX					// 算出当前淡出值
			SHL EDI, 1
			ADD EDI, pdFadeOut
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
			JMP FADEIN
FADEOUT:
		CMP ECX, nFadeInStart			// 与淡入区起始点比较
			JLE  FADEIN
			MOV EDI, nDestHeight					// 算出当前淡入值
			SUB EDI, ECX
			SHL EDI, 1
			ADD EDI, pdFadeIn
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
FADEIN:
		PUSH ECX
			XOR ECX, ECX
LOOPX:
		ADD ECX, 2				// 每次计算2个象素点
			CMP ECX, nDestWidth		// x<nDestWidth
			JG  LOOPX1

			MOVQ MM0, [EAX]			// 得到源行数据
			MOVQ2DQ XMM0, MM0
				PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

				MOVQ MM1, [ESI]			// 得到下一行数据
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					ADD EAX, 8				// 指针后移
					PMULLW XMM1, XMM6		// pSrcNext*nPower
					ADD ESI, 8
					PADDUSW XMM0, XMM1		// 相加
					PSRLW XMM0, 8			// 右移8位
					PMULLW XMM0, XMM4		// 乘以nFadeOut
					PSRLW XMM0, 8			// 右移8位
					PACKUSWB XMM0, XMM0		// 压缩
					MOVDQ2Q MM0, XMM0
					MOVNTQ [EDX], MM0
					ADD EDX, 8

					JMP LOOPX
LOOPX1:
				SUB ECX, 2
					CMP ECX, nDestWidth
					JGE LOOPXEND

					MOVD XMM0, [EAX]			// 得到源行数据
					PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// 得到下一行数据
						PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, 4				// 指针后移
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, 4
							PADDUSW XMM0, XMM1		// 相加
							PSRLW XMM0, 8			// 右移8位
							PMULLW XMM0, XMM4		// 乘以nFadeOut
							PSRLW XMM0, 8			// 右移8位
							PACKUSWB XMM0, XMM0		// 压缩
							MOVD [EDX], XMM0			// 赋值
							ADD EDX, 4
LOOPXEND:
						POP ECX
							ADD ECX, nInc
							POP EAX
							ADD EAX, nAddSrcWidth
							POP EDX
							ADD EDX, nAddDestWidth
							JMP LOOPY
LOOPYEND:
						EMMS
	}

	delete[] pdFadeOut;
	delete[] pdFadeIn;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// NotMix操作
//
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_ScrollScreen_NotMix(DWORD* pdwDestBuffer, 
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
								  int nParity,				 // 奇偶场模式
								  int nScrollType,			 // 滚动类型
								  float fPower,			 // 象素所占权重
								  int nFadeIn,				 // 淡入宽
								  int nFadeOut)			 // 淡出宽
{
	if(pdwDestBuffer == NULL||pdwSrcBuffer == NULL)
		return TRUE;

	// 数据范围检测
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// 得到起点坐标和要拷贝的长宽
	if(nDestX < 0)
	{
		nDestWidth += nDestX;
		nSrcX -= nDestX;
		nDestX = 0;
	}
	if(nDestY < 0)
	{
		nDestHeight += nDestY;
		nSrcY -= nDestY;
		nDestY = 0;
	}

	if(nSrcX < 0)
	{
		nDestWidth += nSrcX;
		nDestX -= nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nDestHeight += nSrcY;
		nDestY -= nSrcY;
		nSrcY = 0;
	}
	// 上滚时，宽度不够就被截掉,高度不够时则从头开始取
	int nTempWidth = nSrcX+nDestWidth;
	if(nTempWidth > nSrcBufferWidth)
		nDestWidth -= (nTempWidth-nSrcBufferWidth);
	nTempWidth = nDestX+nDestWidth;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);

	int nTempHeight = nDestY+nDestHeight;
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// 每次加的宽度
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// 计数器的增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// 一次一行
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// 一次两行
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nDestY % 2 == 0)	// 起点为奇场
	{
		if(nParity == TP_PARITY_EVEN)
		{
			// 如果偶场拷贝，则从下一行开始
			if(nSrcY >= nSrcBufferHeight)
			{
				nSrcY = 0;
				pSrc = pdwSrcBuffer + nSrcX;
			}
			else
				pSrc += nSrcBufferWidth;			// 源移到下一行，如果下一行为空，则移到源的第一行
			nDestHeight--;
			if(nDestY >= nDestBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;				// 目标移到下一行
		}
	}
	else
	{
		// 起点为偶场
		if(nParity == TP_PARITY_ODD)
		{
			// 如果奇场拷贝，则从下一行开始
			nDestHeight--;
			if(nSrcY >= nSrcBufferHeight)
			{
				nSrcY = 0;
				pSrc = pdwSrcBuffer + nSrcX;
			}
			else
				pSrc += nSrcBufferWidth;		// 源移到下一行，如果下一行为空，则移到源的第一行
			if(nDestY >= nDestBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;			// 目标移到下一行
		}
	}

	// 源缓冲区最后一行的下一行的指针
	DWORD* pdwLast = pdwSrcBuffer + nSrcBufferWidth*nSrcBufferHeight;
	DWORD* pdwFirst = pdwSrcBuffer + nSrcX;								// 首行指针
	WORD nPower = (WORD)(fPower*255);
	WORD nSubPower = (WORD)(255-nPower);
	int nSrcLineBytes = nSrcBufferWidth<<2;						// 一行的字节数
	_declspec(align(16)) WORD narPower[] = {nPower, nPower, nPower, nPower, nPower, nPower, nPower, nPower};
	_declspec(align(16)) WORD narSubPower[] = {nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower};
	_declspec(align(16)) DWORD dwFade[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	WORD* pdFadeOut = new WORD[nFadeOut];			// 淡出缓冲区
	int i=0;
	if(nFadeOut == 1)
		pdFadeOut[0] = 128;
	else
	{
		for(i=0; i<nFadeOut; i++)
		{
			pdFadeOut[i] = (WORD)((i/(float)(nFadeOut-1))*255);
		}
	}
	WORD* pdFadeIn = new WORD[nFadeIn];		// 淡入缓冲区
	int nFadeInStart = nDestHeight - nFadeIn;	// 淡入区起始点
	if(nFadeIn == 1)
		pdFadeIn[0] = 128;
	else
	{
		for(i=0; i<nFadeIn; i++)
		{
			pdFadeIn[i] = (WORD)((i/(float)(nFadeIn-1))*255);
		}
	}

	__asm
	{
		XOR ECX, ECX
			PXOR XMM7, XMM7
			MOVDQA XMM6, narPower
			MOVDQA XMM5, narSubPower
			MOV EAX, pSrc			// 源指针
			MOV EDX, pDes
LOOPY:
		CMP ECX, nDestHeight	// y<nDestHeight
			JGE LOOPYEND
			CMP EAX, pdwLast		// pSrc<pdwLast
			JL  LOOPYSRC
			MOV EAX, pdwFirst		// 如果pSrc>=pdwLast,则pSrc=pdwFirst
LOOPYSRC:
		MOV ESI, EAX			// 下一行指针
			ADD ESI, nSrcLineBytes
			CMP ESI, pdwLast
			JL LOOPYNEXT
			MOV ESI, pdwFirst
LOOPYNEXT:
		PUSH EDX
			PUSH EAX
			MOVDQA XMM4, dwFade		// 找到淡入淡出值:默认为0F0F0F0F0F0F0F0F
			CMP ECX, nFadeOut
			JGE  FADEOUT
			MOV EDI, ECX					// 算出当前淡出值
			SHL EDI, 1
			ADD EDI, pdFadeOut
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
			JMP FADEIN
FADEOUT:
		CMP ECX, nFadeInStart			// 与淡入区起始点比较
			JLE  FADEIN
			MOV EDI, nDestHeight					// 算出当前淡入值
			SUB EDI, ECX
			SHL EDI, 1
			ADD EDI, pdFadeIn
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
FADEIN:
		PUSH ECX
			XOR ECX, ECX
LOOPX:
		ADD ECX, 2				// 每次计算2个象素点
			CMP ECX, nDestWidth		// x<nDestWidth
			JG  LOOPX1

			MOVQ MM0, [EAX]			// 得到源行数据
			MOVQ2DQ XMM0, MM0
				PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

				MOVQ MM1, [ESI]			// 得到下一行数据
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					ADD EAX, 8				// 指针后移
					PMULLW XMM1, XMM6		// pSrcNext*nPower
					ADD ESI, 8
					PADDUSW XMM0, XMM1		// 相加
					PSRLW XMM0, 8			// 右移8位
					PMULLW XMM0, XMM4		// 乘以nFadeOut
					PSRLW XMM0, 8			// 右移8位
					PACKUSWB XMM0, XMM0		// 压缩
					MOVDQ2Q MM0, XMM0
					MOVQ [EDX], MM0
					ADD EDX, 8

					JMP LOOPX
LOOPX1:
				SUB ECX, 2
					CMP ECX, nDestWidth
					JGE LOOPXEND

					MOVD XMM0, [EAX]			// 得到源行数据
					PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// 得到下一行数据
						PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, 4				// 指针后移
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, 4
							PADDUSW XMM0, XMM1		// 相加
							PSRLW XMM0, 8			// 右移8位
							PMULLW XMM0, XMM4		// 乘以nFadeOut
							PSRLW XMM0, 8			// 右移8位
							PACKUSWB XMM0, XMM0		// 压缩
							MOVD [EDX], XMM0			// 赋值
							ADD EDX, 4
LOOPXEND:
						POP ECX
							ADD ECX, nInc
							POP EAX
							ADD EAX, nAddSrcWidth
							POP EDX
							ADD EDX, nAddDestWidth
							JMP LOOPY
LOOPYEND:
						EMMS
	}

	delete[] pdFadeOut;
	delete[] pdFadeIn;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 功能：滚屏上滚
// 算法：pCur*(1-fSubPower) + pNextCur*fPower 
//		 宽度不够就被截掉,高度不够时则从头开始取
//////////////////////////////////////////////////////////////////////////
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
							  int nFadeOut)
{
	// 数据范围检测
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// 得到起点坐标和要拷贝的长宽
	if(nDestX < 0)
	{
		nDestWidth += nDestX;
		nSrcX -= nDestX;
		nDestX = 0;
	}
	if(nDestY < 0)
	{
		nDestHeight += nDestY;
		nSrcY -= nDestY;
		nDestY = 0;
	}

	if(nSrcX < 0)
	{
		nDestWidth += nSrcX;
		nDestX -= nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nDestHeight += nSrcY;
		nDestY -= nSrcY;
		nSrcY = 0;
	}

	// 上滚时，宽度不够就被截掉,高度不够时则从头开始取
	int nTempWidth = nSrcX+nDestWidth;
	if(nTempWidth > nSrcBufferWidth)
		nDestWidth -= (nTempWidth-nSrcBufferWidth);
	nTempWidth = nDestX+nDestWidth;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);

	int nTempHeight = nDestY+nDestHeight;
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// 每次加的宽度
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// 计数器的增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// 一次一行
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// 一次两行
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nParity == TP_PARITY_EVEN && nDestY%2 == 0)
	{
		// 如果偶场拷贝且起点为奇场，则从下一行开始
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;			// 源移到下一行，如果下一行为空，则移到源的第一行
		nDestHeight--;
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;				// 目标移到下一行
	}
	if(nParity == TP_PARITY_ODD && nDestY%2 != 0)
	{
		// 如果奇场拷贝且起点为偶场，则从下一行开始
		nDestHeight--;
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;		// 源移到下一行，如果下一行为空，则移到源的第一行
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;			// 目标移到下一行
	}

	// 源缓冲区最后一行的下一行的指针
	DWORD* pdwLast = pdwSrcBuffer + nSrcBufferWidth*nSrcBufferHeight;
	DWORD* pdwFirst = pdwSrcBuffer + nSrcX;								// 首行指针
	WORD nPower = (WORD)(fPower*255);
	WORD nSubPower = (WORD)(255-nPower);
	_declspec(align(16)) WORD narPower[] = {nPower, nPower, nPower, nPower, nPower, nPower, nPower, nPower};
	_declspec(align(16)) WORD narSubPower[] = {nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower};
	_declspec(align(16)) DWORD dwFade[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	_declspec(align(16)) DWORD dwRepair[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	WORD* pdFadeOut = NULL;
	WORD* pdFadeIn = NULL;
	int nFadeInStart = nDestHeight - nFadeIn;	// 淡入区起始点
	if(nFadeOut > 0)
	{
		pdFadeOut = new WORD[nFadeOut];			// 淡出缓冲区
		if(nFadeOut == 1)
			pdFadeOut[0] = 128;
		else
		{
			/*float f = 255.0f/(nFadeOut-1);
			pdFadeOut[0] = 0;
			for(int i=1; i<nFadeOut; i++)
			{
				pdFadeOut[i] = (WORD)(pdFadeOut[i-1]+f);
			}*/
			for(int i=0; i<nFadeOut; i++)
			{
				pdFadeOut[i] = (WORD)((i/(float)(nFadeOut-1))*255);
			}
		}
	}
	if(nFadeIn > 0)
	{
		pdFadeIn = new WORD[nFadeIn];		// 淡入缓冲区
		if(nFadeIn == 1)
			pdFadeIn[0] = 128;
		else
		{
			/*float f = 255.0f/(nFadeIn-1);
			pdFadeIn[0] = 0;
			for(int i=1; i<nFadeIn; i++)
			{
				pdFadeIn[i] = (WORD)(pdFadeIn[i-1]+f);
			}*/
			for(int i=0; i<nFadeIn; i++)
			{
				pdFadeIn[i] = (WORD)((i/(float)(nFadeIn-1))*255);
			}
		}
	}

if(pdFadeIn != NULL || pdFadeOut != NULL)
{
	// 汇编算法
	__asm
	{
		XOR ECX, ECX
			PXOR XMM7, XMM7
			MOVDQA XMM6, narPower
			MOVDQA XMM5, narSubPower
			MOVDQA XMM3, dwRepair
			MOV EAX, pSrc			// 源指针
			MOV EDX, pDes
LOOPY:
		CMP ECX, nDestHeight	// y<nDestHeight
			JGE LOOPYEND
			CMP EAX, pdwLast		// pSrc<pdwLast
			JL  LOOPYSRC
			MOV EAX, pdwFirst		// 如果pSrc>=pdwLast,则pSrc=pdwFirst
LOOPYSRC:
		MOV ESI, EAX			// 下一行指针, pNext>=pdwLast,则pNext=pdwFirst
			ADD ESI, nAddSrcWidth		//nAddSrcWidth
			CMP ESI, pdwLast
			JL LOOPYNEXT
			MOV ESI, pdwFirst
LOOPYNEXT:
		PUSH EDX
			PUSH EAX
			MOVDQA XMM4, dwFade		// 找到淡入淡出值:默认为0F0F0F0F0F0F0F0F
			CMP ECX, nFadeOut
			JGE  FADEOUT
			MOV EDI, ECX					// 算出当前淡出值
			SHL EDI, 1
			ADD EDI, pdFadeOut
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
			JMP FADEIN
FADEOUT:
		CMP ECX, nFadeInStart			// 与淡入区起始点比较
			JL  FADEIN
			MOV EDI, nDestHeight				// 算出当前淡入值
			DEC EDI
			SUB EDI, ECX
			SHL EDI, 1
			ADD EDI, pdFadeIn
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
FADEIN:
			XOR EDI, EDI
LOOPX:
		ADD EDI, 2				// 每次计算2个象素点
			CMP EDI, nDestWidth		// x<nDestWidth
			JG  LOOPX1

			MOVQ MM0, [EAX]			// 得到源行数据
			MOVQ2DQ XMM0, MM0
				PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

				MOVQ MM1, [ESI]			// 得到下一行数据
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					ADD EAX, 8				// 指针后移
					PMULLW XMM1, XMM6		// pSrcNext*nPower
					ADD ESI, 8
					PADDUSW XMM0, XMM1		// 相加
					PADDUSW XMM0, XMM3		// 误差补偿
					PSRLW XMM0, 8			// 右移8位
					PMULLW XMM0, XMM4		// 乘以nFadeOut
					PADDUSW XMM0, XMM3		// 误差补偿
					PSRLW XMM0, 8			// 右移8位
					PACKUSWB XMM0, XMM0		// 压缩
					MOVDQ2Q MM0, XMM0
					MOVQ [EDX], MM0
					ADD EDX, 8

					JMP LOOPX
LOOPX1:
				SUB EDI, 2
					CMP EDI, nDestWidth
					JGE LOOPXEND

					MOVD XMM0, [EAX]			// 得到源行数据
					PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// 得到下一行数据
						PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, 4				// 指针后移
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, 4
							PADDUSW XMM0, XMM1		// 相加
							PADDUSW XMM0, XMM3		// 误差补偿
							PSRLW XMM0, 8			// 右移8位
							PMULLW XMM0, XMM4		// 乘以nFadeOut
							PADDUSW XMM0, XMM3		// 误差补偿
							PSRLW XMM0, 8			// 右移8位
							PACKUSWB XMM0, XMM0		// 压缩
							MOVD [EDX], XMM0			// 赋值
							ADD EDX, 4
LOOPXEND:
							ADD ECX, nInc
							POP EAX
							ADD EAX, nAddSrcWidth

							POP EDX
							ADD EDX, nAddDestWidth
							JMP LOOPY
LOOPYEND:
						EMMS
	}

	if(pdFadeOut)
		delete[] pdFadeOut;
	if(pdFadeIn)
		delete[] pdFadeIn;
	return TRUE;
}
__asm
{
	XOR ECX, ECX
	PXOR XMM7, XMM7
	MOVDQA XMM6, narPower
	MOVDQA XMM5, narSubPower
	MOVDQA XMM3, dwRepair
	MOV EAX, pSrc			// 源指针
	MOV EDX, pDes
LOOPY_1:
	CMP ECX, nDestHeight	// y<nDestHeight
	JGE LOOPYEND_1
	CMP EAX, pdwLast		// pSrc<pdwLast
	JL  LOOPYSRC_1
	MOV EAX, pdwFirst		// 如果pSrc>=pdwLast,则pSrc=pdwFirst
LOOPYSRC_1:
	MOV ESI, EAX			// 下一行指针, pNext>=pdwLast,则pNext=pdwFirst
	ADD ESI, nAddSrcWidth		//nAddSrcWidth
	CMP ESI, pdwLast
	JL LOOPYNEXT_1
	MOV ESI, pdwFirst
LOOPYNEXT_1:
	PUSH EDX
	PUSH EAX
	XOR EDI, EDI
LOOPX_1:
	ADD EDI, 2				// 每次计算2个象素点
		CMP EDI, nDestWidth		// x<nDestWidth
		JG  LOOPX1_1

		MOVQ MM0, [EAX]			// 得到源行数据
		MOVQ2DQ XMM0, MM0
			PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

			MOVQ MM1, [ESI]			// 得到下一行数据
			MOVQ2DQ XMM1, MM1
				PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
				PMULLW XMM0, XMM5		// pSrc*nSubPower

				ADD EAX, 8				// 指针后移
				PMULLW XMM1, XMM6		// pSrcNext*nPower
				ADD ESI, 8
				PADDUSW XMM0, XMM1		// 相加
				PADDUSW XMM0, XMM3		// 误差补偿
				PSRLW XMM0, 8			// 右移8位
				PACKUSWB XMM0, XMM0		// 压缩
				MOVDQ2Q MM0, XMM0
				MOVQ [EDX], MM0
				ADD EDX, 8

				JMP LOOPX_1
LOOPX1_1:
			SUB EDI, 2
				CMP EDI, nDestWidth
				JGE LOOPXEND_1

				MOVD XMM0, [EAX]			// 得到源行数据
				PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

					MOVD XMM1, [ESI]			// 得到下一行数据
					PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
						PMULLW XMM0, XMM5		// pSrc*nSubPower

						ADD EAX, 4				// 指针后移
						PMULLW XMM1, XMM6		// pSrcNext*nPower
						ADD ESI, 4
						PADDUSW XMM0, XMM1		// 相加
						PADDUSW XMM0, XMM3		// 误差补偿
						PSRLW XMM0, 8			// 右移8位
						PACKUSWB XMM0, XMM0		// 压缩
						MOVD [EDX], XMM0			// 赋值
						ADD EDX, 4
LOOPXEND_1:
					ADD ECX, nInc
						POP EAX
						ADD EAX, nAddSrcWidth

						POP EDX
						ADD EDX, nAddDestWidth
						JMP LOOPY_1
LOOPYEND_1:
					EMMS
}
return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 滚屏左滚
//
//////////////////////////////////////////////////////////////////////////
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
								int nFadeOut)
{
	// 数据范围检测
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// 得到起点坐标和要拷贝的长宽
	if(nDestX < 0)
	{
		nDestWidth += nDestX;
		nSrcX -= nDestX;
		nDestX = 0;
	}
	if(nDestY < 0)
	{
		nDestHeight += nDestY;
		nSrcY -= nDestY;
		nDestY = 0;
	}

	if(nSrcX < 0)
	{
		nDestWidth += nSrcX;
		nDestX -= nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nDestHeight += nSrcY;
		nDestY -= nSrcY;
		nSrcY = 0;
	}

	// 左滚时，高度不够就被截掉,宽度不够时则从第一列开始取
	int nTempHeight = nSrcY+nDestHeight;
	if(nTempHeight > nSrcBufferHeight)
		nDestHeight -= (nTempHeight-nSrcBufferHeight);

	int nTempWidth = nDestX+nDestWidth;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);
	nTempHeight = nDestY+nDestHeight;
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// 每次加的宽度(字节数）
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// 计数器的增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// 一次一行
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// 一次两行
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nParity == TP_PARITY_EVEN && nDestY % 2 == 0)
	{
		// 如果偶场拷贝且起点为奇场，则从下一行开始
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;			// 源移到下一行，如果下一行为空，则移到源的第一行
		nDestHeight--;
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;				// 目标移到下一行
	}

	// 起点为偶场
	if(nParity == TP_PARITY_ODD && nDestY%2)
	{
		// 如果奇场拷贝且起点为偶场，则从下一行开始
		nDestHeight--;
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;		// 源移到下一行，如果下一行为空，则移到源的第一行
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;			// 目标移到下一行
	}

	WORD nPower = (WORD)(fPower*255);			// 权重
	WORD nSubPower = (WORD)(255-nPower);
	WORD* pdFadeOut = NULL;
	WORD* pdFadeIn = NULL;
	int nFadeInStart = nDestWidth - nFadeIn;	// 淡入区起始点
	if(nFadeOut > 0)
	{
		pdFadeOut = new WORD[nFadeOut];			// 淡出缓冲区
		if(nFadeOut == 1)
			pdFadeOut[0] = 128;
		else
		{
			/*float f = 255.0f/(nFadeOut-1);
			pdFadeOut[0] = 0;
			for(int i=1; i<nFadeOut; i++)
			{
			pdFadeOut[i] = (WORD)(pdFadeOut[i-1]+f);
			}*/
			for(int i=0; i<nFadeOut; i++)
			{
				pdFadeOut[i] = (WORD)((i/(float)(nFadeOut-1))*255);
			}
		}
	}
	if(nFadeIn > 0)
	{
		pdFadeIn = new WORD[nFadeIn];		// 淡入缓冲区
		if(nFadeIn == 1)
			pdFadeIn[0] = 128;
		else
		{
			/*float f = 255.0f/(nFadeIn-1);
			pdFadeIn[0] = 0;
			for(int i=1; i<nFadeIn; i++)
			{
			pdFadeIn[i] = (WORD)(pdFadeIn[i-1]+f);
			}*/
			for(int i=0; i<nFadeIn; i++)
			{
				pdFadeIn[i] = (WORD)((i/(float)(nFadeIn-1))*255);
			}
		}
	}

	_declspec(align(16)) WORD narPower[] = {nPower, nPower, nPower, nPower, nPower, nPower, nPower, nPower};
	_declspec(align(16)) WORD narSubPower[] = {nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower};
	_declspec(align(16)) DWORD dwFade[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	_declspec(align(16)) DWORD dwRepair[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	int nEnd = nSrcBufferWidth - nSrcX;
	int nSubEnd = nEnd - 1;
	DWORD* pdwTempSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth;
	if(pdFadeIn != NULL || pdFadeOut != NULL)
	{
		__asm
		{
			XOR ECX, ECX
				PXOR XMM7, XMM7
				MOVDQA XMM6, narPower
				MOVDQA XMM5, narSubPower
				MOVDQA XMM3, dwRepair
				MOV EAX, pSrc			// 源指针
				MOV EDX, pDes			// 目标指针
LOOPX:
			CMP ECX, nDestWidth		// x<nDestWidth
				JGE LOOPXEND
				CMP ECX, nEnd		// x<nEnd
				JL  LOOPXSRC
				//	MOV EAX, ECX		// 如果x>=nEnd,则pCur=pSrc + (i-nSrcBufferWidth)
				//	SUB EAX, nSrcBufferWidth
				//	SHL EAX, 2
				//	ADD EAX, pSrc
				MOV EAX, ECX		// 如果x>=nEnd,则pCur = pdwSrcBuffer + (i-nEnd)
				SUB EAX, nEnd
				SHL EAX, 2
				ADD EAX, pdwTempSrc
LOOPXSRC:
			MOV ESI, EAX			// 源的下一列指针:pSrc++
				ADD ESI, 4
				CMP ECX, nSubEnd		// x<nEnd
				JL LOOPXNEXT
				//MOV ESI, ECX		// 如果x>=nEnd,则pNext=pSrc + (i+1-nSrcBufferWidth)
				//INC ESI
				//SUB ESI, nSrcBufferWidth
				//SHL ESI, 2
				//ADD ESI, pSrc
				MOV ESI, ECX		// 如果x>=nEnd,则pCur = pdwSrcBuffer + (i+1-nEnd)
				SUB ESI, nSubEnd
				SHL ESI, 2
				ADD ESI, pdwTempSrc
LOOPXNEXT:
			MOVDQA XMM4, dwFade		// 找到淡入淡出值:默认为0F0F0F0F0F0F0F0F
				CMP ECX, nFadeOut
				JGE  FADEOUT			// x>=nFadeOut时不淡出
				MOV EDI, ECX			// 算出当前淡出值
				SHL EDI, 1
				ADD EDI, pdFadeOut
				PINSRW XMM4, [EDI], 3
				PINSRW XMM4, [EDI], 7
				JMP FADEIN				// 有淡出时不用淡入
FADEOUT:
			CMP ECX, nFadeInStart	// 与淡入区起始点比较
				JL  FADEIN				// x<nFadeInStart，不淡入
				MOV EDI, nDestWidth		// 算出当前淡入值
				DEC EDI
				SUB EDI, ECX			
				SHL EDI, 1
				ADD EDI, pdFadeIn		// pdFadeIn[nDestWidth-1-i]
				PINSRW XMM4, [EDI], 3
				PINSRW XMM4, [EDI], 7
FADEIN:
			PUSH EDX
				PUSH EAX
				XOR EDI, EDI
LOOPY:
			ADD EDI, 2				// 每次计算2个象素点
				CMP EDI, nDestHeight	// y<nDestHeight
				JG  LOOPY1

				MOVD XMM0, [EAX]			// 得到源列数据
				ADD EAX, nAddSrcWidth
					MOVD XMM1, [EAX]
					PUNPCKLDQ XMM0, XMM1
						PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// 得到下一列数据
						ADD ESI, nAddSrcWidth
							MOVD XMM2, [ESI]
							PUNPCKLDQ XMM1, XMM2
								PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
								PMULLW XMM0, XMM5		// pSrc*nSubPower

								ADD EAX, nAddSrcWidth	// 指针后移
								PMULLW XMM1, XMM6		// pSrcNext*nPower
								ADD ESI, nAddSrcWidth
								PADDUSW XMM0, XMM1		// 相加
								PADDUSW XMM0, XMM3		// 误差补偿
								PSRLW XMM0, 8			// 右移8位
								PMULLW XMM0, XMM4		// 乘以nFadeOut或nFadeIn
								PADDUSW XMM0, XMM3		// 误差补偿
								PSRLW XMM0, 8			// 右移8位
								PACKUSWB XMM0, XMM0		// 压缩

								MOVD [EDX], XMM0
								ADD  EDX, nAddDestWidth
								PSRLDQ XMM0, 4
								MOVD [EDX], XMM0
								ADD  EDX, nAddDestWidth

								JMP LOOPY
LOOPY1:
							SUB EDI, 2
								CMP EDI, nDestHeight
								JGE LOOPYEND

								MOVD XMM0, [EAX]			// 得到源列数据
								PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

									MOVD XMM1, [ESI]			// 得到下一列数据
									PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
										PMULLW XMM0, XMM5		// pSrc*nSubPower

										ADD EAX, nAddSrcWidth	// 指针后移
										PMULLW XMM1, XMM6		// pSrcNext*nPower
										ADD ESI, nAddSrcWidth
										PADDUSW XMM0, XMM1		// 相加
										PADDUSW XMM0, XMM3		// 误差补偿
										PSRLW XMM0, 8			// 右移8位
										PMULLW XMM0, XMM4		// 乘以nFadeOut
										PADDUSW XMM0, XMM3		// 误差补偿
										PSRLW XMM0, 8			// 右移8位
										PACKUSWB XMM0, XMM0		// 压缩
										MOVD [EDX], XMM0		// 赋值
										ADD EDX, nAddDestWidth
LOOPYEND:
									ADD ECX, nInc
										POP EAX
										ADD EAX, 4
										POP EDX
										ADD EDX, 4
										JMP LOOPX
LOOPXEND:
		}
		if(pdFadeOut)
			delete[] pdFadeOut;
		if(pdFadeIn)
			delete[] pdFadeIn;
		return TRUE;
	}
	__asm
	{
		XOR ECX, ECX
		PXOR XMM7, XMM7
		MOVDQA XMM6, narPower
		MOVDQA XMM5, narSubPower
		MOVDQA XMM3, dwRepair
		MOV EAX, pSrc			// 源指针
		MOV EDX, pDes			// 目标指针
LOOPX_1:
		CMP ECX, nDestWidth		// x<nDestWidth
		JGE LOOPXEND_1
		CMP ECX, nEnd		// x<nEnd
		JL  LOOPXSRC_1
			//	MOV EAX, ECX		// 如果x>=nEnd,则pCur=pSrc + (i-nSrcBufferWidth)
			//	SUB EAX, nSrcBufferWidth
			//	SHL EAX, 2
			//	ADD EAX, pSrc
			MOV EAX, ECX		// 如果x>=nEnd,则pCur = pdwSrcBuffer + (i-nEnd)
			SUB EAX, nEnd
			SHL EAX, 2
			ADD EAX, pdwTempSrc
LOOPXSRC_1:
		MOV ESI, EAX			// 源的下一列指针:pSrc++
			ADD ESI, 4
			CMP ECX, nSubEnd		// x<nEnd
			JL LOOPXNEXT_1
			//MOV ESI, ECX		// 如果x>=nEnd,则pNext=pSrc + (i+1-nSrcBufferWidth)
			//INC ESI
			//SUB ESI, nSrcBufferWidth
			//SHL ESI, 2
			//ADD ESI, pSrc
			MOV ESI, ECX		// 如果x>=nEnd,则pCur = pdwSrcBuffer + (i+1-nEnd)
			SUB ESI, nSubEnd
			SHL ESI, 2
			ADD ESI, pdwTempSrc
LOOPXNEXT_1:
		PUSH EDX
		PUSH EAX
		XOR EDI, EDI
LOOPY_1:
		ADD EDI, 2				// 每次计算2个象素点
			CMP EDI, nDestHeight	// y<nDestHeight
			JG  LOOPY1_1

			MOVD XMM0, [EAX]			// 得到源列数据
			ADD EAX, nAddSrcWidth
				MOVD XMM1, [EAX]
				PUNPCKLDQ XMM0, XMM1
					PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

					MOVD XMM1, [ESI]			// 得到下一列数据
					ADD ESI, nAddSrcWidth
						MOVD XMM2, [ESI]
						PUNPCKLDQ XMM1, XMM2
							PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, nAddSrcWidth	// 指针后移
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, nAddSrcWidth
							PADDUSW XMM0, XMM1		// 相加
							PADDUSW XMM0, XMM3		// 误差补偿
							PSRLW XMM0, 8			// 右移8位
							PACKUSWB XMM0, XMM0		// 压缩

							MOVD [EDX], XMM0
							ADD  EDX, nAddDestWidth
							PSRLDQ XMM0, 4
							MOVD [EDX], XMM0
							ADD  EDX, nAddDestWidth

							JMP LOOPY_1
LOOPY1_1:
						SUB EDI, 2
							CMP EDI, nDestHeight
							JGE LOOPYEND_1

							MOVD XMM0, [EAX]			// 得到源列数据
							PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

								MOVD XMM1, [ESI]			// 得到下一列数据
								PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
									PMULLW XMM0, XMM5		// pSrc*nSubPower

									ADD EAX, nAddSrcWidth	// 指针后移
									PMULLW XMM1, XMM6		// pSrcNext*nPower
									ADD ESI, nAddSrcWidth
									PADDUSW XMM0, XMM1		// 相加
									PADDUSW XMM0, XMM3		// 误差补偿
									PSRLW XMM0, 8			// 右移8位
									PACKUSWB XMM0, XMM0		// 压缩
									MOVD [EDX], XMM0		// 赋值
									ADD EDX, nAddDestWidth
LOOPYEND_1:
								ADD ECX, nInc
									POP EAX
									ADD EAX, 4
									POP EDX
									ADD EDX, 4
									JMP LOOPX_1
LOOPXEND_1:
	}
return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 功能：滚屏下滚:当前行与上一行混叠
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
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
								int nFadeOut)
{
	// 数据范围检测
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// 得到起点坐标和要拷贝的长宽
	if(nDestX < 0)
	{
		nDestWidth += nDestX;
		nSrcX -= nDestX;
		nDestX = 0;
	}
	if(nDestY < 0)
	{
		nDestHeight += nDestY;
		nSrcY -= nDestY;
		nDestY = 0;
	}

	if(nSrcX < 0)
	{
		nDestWidth += nSrcX;
		nDestX -= nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nDestHeight += nSrcY;
		nDestY -= nSrcY;
		nSrcY = 0;
	}

	// 下滚时，宽度不够就被截掉,高度不够时则从头开始取
	int nTempWidth = nSrcX+nDestWidth;
	if(nTempWidth > nSrcBufferWidth)
		nDestWidth -= (nTempWidth-nSrcBufferWidth);

	nTempWidth = nDestX+nDestWidth;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);
	int nTempHeight = nDestY+nDestHeight;
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// 每次加的宽度（字节数）
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// 计数器的增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// 一次一行
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// 一次两行
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nParity == TP_PARITY_EVEN && nDestY % 2 == 0)
	{
		// 如果偶场拷贝且起点为奇场，则从下一行开始
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;			// 源移到下一行，如果下一行为空，则移到源的第一行
		nDestHeight--;
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;				// 目标移到下一行
	}
	if(nParity == TP_PARITY_ODD && nDestY%2)
	{
		// 如果奇场拷贝且起点为偶场，则从下一行开始
		nDestHeight--;
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;		// 源移到下一行，如果下一行为空，则移到源的第一行
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;			// 目标移到下一行
	}
	
	// 全部倒置拷贝：从下往上操作
	DWORD* pdwLast = pdwSrcBuffer + nSrcX;					// 源首行指针，当小于此指针时，转到源末行
	DWORD* pdwFirst = pdwSrcBuffer + (nSrcBufferHeight-1)*nSrcBufferWidth + nSrcX;	// 源的末行行指针
	pDes += (nDestHeight-1)*nDestBufferWidth;				// 要操作的目标末行指针
	WORD nPower = (WORD)(fPower*255);
	WORD nSubPower = (WORD)(255-nPower);
	DWORD* pNext = pSrc - nSrcBufferWidth;		// 上一行
	WORD* pdFadeOut = NULL;
	WORD* pdFadeIn = NULL;
	int nFadeInStart = nDestHeight - nFadeIn;		// 淡入区起始点
	if(nFadeOut > 0)
	{
		pdFadeOut = new WORD[nFadeOut];			// 淡出缓冲区
		if(nFadeOut == 1)
			pdFadeOut[0] = 128;
		else
		{
			/*float f = 255.0f/(nFadeOut-1);
			pdFadeOut[0] = 0;
			for(int i=1; i<nFadeOut; i++)
			{
			pdFadeOut[i] = (WORD)(pdFadeOut[i-1]+f);
			}*/
			for(int i=0; i<nFadeOut; i++)
			{
				pdFadeOut[i] = (WORD)((i/(float)(nFadeOut-1))*255);
			}
		}
	}
	if(nFadeIn > 0)
	{
		pdFadeIn = new WORD[nFadeIn];		// 淡入缓冲区
		if(nFadeIn == 1)
			pdFadeIn[0] = 128;
		else
		{
			/*float f = 255.0f/(nFadeIn-1);
			pdFadeIn[0] = 0;
			for(int i=1; i<nFadeIn; i++)
			{
			pdFadeIn[i] = (WORD)(pdFadeIn[i-1]+f);
			}*/
			for(int i=0; i<nFadeIn; i++)
			{
				pdFadeIn[i] = (WORD)((i/(float)(nFadeIn-1))*255);
			}
		}
	}

	_declspec(align(16)) WORD narPower[] = {nPower, nPower, nPower, nPower, nPower, nPower, nPower, nPower};
	_declspec(align(16)) WORD narSubPower[] = {nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower};
	_declspec(align(16)) DWORD dwFade[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	_declspec(align(16)) DWORD dwRepair[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	// 汇编实现
	if(pdFadeIn != NULL || pdFadeOut != NULL)
	{
	__asm
	{
		XOR ECX, ECX
		PXOR XMM7, XMM7
		MOVDQA XMM6, narSubPower
		MOVDQA XMM5, narPower
		MOVDQA XMM3, dwRepair
		MOV EDX, pDes
		MOV EAX, pSrc
LOOPY:		
		CMP ECX, nDestHeight
		JGE LOOPYEND
		CMP EAX, pdwLast
		JGE LOOPYSRC				// 如果 pSrc<pdwLast, 则pSrc = pdwFirst
		MOV EAX, pdwFirst
LOOPYSRC:
		MOV ESI, EAX				// 上一行指针:当前行象素与上一行象素混叠
		SUB ESI, nAddSrcWidth
		CMP ESI, pdwLast			
		JGE LOOPYNEXT				// 如果 pNext<pdwLast, 则pNext = pdwFirst
		MOV ESI, pdwFirst
LOOPYNEXT:
		MOVDQA XMM4, dwFade			// 找到淡入淡出值:默认为0F0F0F0F0F0F0F0F
		CMP ECX, nFadeOut
		JGE  FADEOUT
		MOV EDI, ECX				// 算出当前淡出值
		SHL EDI, 1
		ADD EDI, pdFadeOut
		PINSRW XMM4, [EDI], 3
		PINSRW XMM4, [EDI], 7
		JMP FADEIN
FADEOUT:
		CMP ECX, nFadeInStart			// 与淡入区起始点比较
		JL  FADEIN
		MOV EDI, nDestHeight				// 算出当前淡入值
		DEC EDI
		SUB EDI, ECX
		SHL EDI, 1
		ADD EDI, pdFadeIn
		PINSRW XMM4, [EDI], 3
		PINSRW XMM4, [EDI], 7
FADEIN:
		PUSH EAX
		PUSH EDX
		XOR EDI, EDI
LOOPX:
		ADD EDI, 2
		CMP EDI, nDestWidth
		JG  LOOPX1
		
		MOVQ MM0, [EAX]			// 得到源行数据
		MOVQ2DQ XMM0, MM0
		PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

		MOVQ MM1, [ESI]			// 得到下一行数据
		MOVQ2DQ XMM1, MM1
		PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
		PMULLW XMM0, XMM5		// pSrc*nSubPower

		ADD EAX, 8				// 指针后移
		PMULLW XMM1, XMM6		// pSrcNext*nPower
		ADD ESI, 8
		PADDUSW XMM0, XMM1		// 相加
		PADDUSW XMM0, XMM3		// 误差补偿(加上00FF)
		PSRLW XMM0, 8			// 右移8位
		PMULLW XMM0, XMM4		// 乘以nFadeOut
		PADDUSW XMM0, XMM3		// 误差补偿
		PSRLW XMM0, 8			// 右移8位
		PACKUSWB XMM0, XMM0		// 压缩
		MOVDQ2Q MM0, XMM0
		MOVQ [EDX], MM0
		ADD EDX, 8

		JMP LOOPX
LOOPX1:
		SUB EDI, 2
		CMP EDI, nDestWidth
		JGE LOOPXEND

		MOVD XMM0, [EAX]			// 得到源行数据
		PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

		MOVD XMM1, [ESI]			// 得到下一行数据
		PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
		PMULLW XMM0, XMM5		// pSrc*nSubPower

		ADD EAX, 4				// 指针后移
		PMULLW XMM1, XMM6		// pSrcNext*nPower
		ADD ESI, 4
		PADDUSW XMM0, XMM1		// 相加
		PADDUSW XMM0, XMM3		// 误差补偿
		PSRLW XMM0, 8			// 右移8位
		PMULLW XMM0, XMM4		// 乘以nFadeOut
		PADDUSW XMM0, XMM3		// 误差补偿
		PSRLW XMM0, 8			// 右移8位
		PACKUSWB XMM0, XMM0		// 压缩
		MOVD [EDX], XMM0			// 赋值
		ADD EDX, 4
LOOPXEND:
		ADD ECX, nInc
		POP EDX
		SUB EDX, nAddDestWidth
		POP EAX
		SUB EAX, nAddSrcWidth
		JMP LOOPY
LOOPYEND:
		EMMS
	}
	if(pdFadeOut)
		delete[] pdFadeOut;
	if(pdFadeIn)
		delete[] pdFadeIn;
	return TRUE;
	}
	__asm
	{
		XOR ECX, ECX
		PXOR XMM7, XMM7
		MOVDQA XMM6, narSubPower
		MOVDQA XMM5, narPower
		MOVDQA XMM3, dwRepair
		MOV EDX, pDes
		MOV EAX, pSrc
LOOPY_1:		
		CMP ECX, nDestHeight
			JGE LOOPYEND_1
			CMP EAX, pdwLast
			JGE LOOPYSRC_1				// 如果 pSrc<pdwLast, 则pSrc = pdwFirst
			MOV EAX, pdwFirst
LOOPYSRC_1:
		MOV ESI, EAX				// 上一行指针:当前行象素与上一行象素混叠
			SUB ESI, nAddSrcWidth
			CMP ESI, pdwLast			
			JGE LOOPYNEXT_1				// 如果 pNext<pdwLast, 则pNext = pdwFirst
			MOV ESI, pdwFirst
LOOPYNEXT_1:
		PUSH EAX
			PUSH EDX
			XOR EDI, EDI
LOOPX_1:
		ADD EDI, 2
			CMP EDI, nDestWidth
			JG  LOOPX1_1

			MOVQ MM0, [EAX]			// 得到源行数据
			MOVQ2DQ XMM0, MM0
				PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

				MOVQ MM1, [ESI]			// 得到下一行数据
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					ADD EAX, 8				// 指针后移
					PMULLW XMM1, XMM6		// pSrcNext*nPower
					ADD ESI, 8
					PADDUSW XMM0, XMM1		// 相加
					PADDUSW XMM0, XMM3		// 误差补偿(加上00FF)
					PSRLW XMM0, 8			// 右移8位
					PACKUSWB XMM0, XMM0		// 压缩
					MOVDQ2Q MM0, XMM0
					MOVQ [EDX], MM0
					ADD EDX, 8

					JMP LOOPX_1
LOOPX1_1:
				SUB EDI, 2
					CMP EDI, nDestWidth
					JGE LOOPXEND_1

					MOVD XMM0, [EAX]			// 得到源行数据
					PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// 得到下一行数据
						PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, 4				// 指针后移
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, 4
							PADDUSW XMM0, XMM1		// 相加
							PADDUSW XMM0, XMM3		// 误差补偿
							PSRLW XMM0, 8			// 右移8位
							PACKUSWB XMM0, XMM0		// 压缩
							MOVD [EDX], XMM0			// 赋值
							ADD EDX, 4
LOOPXEND_1:
						ADD ECX, nInc
							POP EDX
							SUB EDX, nAddDestWidth
							POP EAX
							SUB EAX, nAddSrcWidth
							JMP LOOPY_1
LOOPYEND_1:
						EMMS
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 滚屏右滚
//
//////////////////////////////////////////////////////////////////////////
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
								int nFadeOut)
{
	// 数据范围检测
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// 得到起点坐标和要拷贝的长宽
	if(nDestX < 0)
	{
		nDestWidth += nDestX;
		nSrcX -= nDestX;
		nDestX = 0;
	}
	if(nDestY < 0)
	{
		nDestHeight += nDestY;
		nSrcY -= nDestY;
		nDestY = 0;
	}

	if(nSrcX < 0)
	{
		nDestWidth += nSrcX;
		nDestX -= nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nDestHeight += nSrcY;
		nDestY -= nSrcY;
		nSrcY = 0;
	}

	// 右滚时，高度不够就被截掉,宽度不够时则从第一列开始取
	int nTempHeight = nSrcY+nDestHeight;
	if(nTempHeight > nSrcBufferHeight)
		nDestHeight -= (nTempHeight-nSrcBufferHeight);

	int nTempWidth = nDestX+nDestWidth;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);
	nTempHeight = nDestY+nDestHeight;
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// 每次加的宽度(字节数）
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// 计数器的增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// 一次一行
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// 一次两行
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nParity == TP_PARITY_EVEN && nDestY % 2 == 0)
	{
		// 如果偶场拷贝且起点为奇场，则从下一行开始
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;			// 源移到下一行，如果下一行为空，则移到源的第一行
		nDestHeight--;
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;				// 目标移到下一行
	}
	if(nParity == TP_PARITY_ODD && nDestY%2)
	{
		// 如果奇场拷贝且起点为偶场，则从下一行开始
		nDestHeight--;
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;		// 源移到下一行，如果下一行为空，则移到源的第一行
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;			// 目标移到下一行
	}

	WORD nPower = (WORD)(fPower*255);			// 权重
	WORD nSubPower = (WORD)(255-nPower);
	WORD* pdFadeOut = NULL;
	WORD* pdFadeIn = NULL;
	int nFadeInStart = nDestWidth - nFadeIn;	// 淡入区起始点
	if(nFadeOut > 0)
	{
		pdFadeOut = new WORD[nFadeOut];			// 淡出缓冲区
		if(nFadeOut == 1)
			pdFadeOut[0] = 128;
		else
		{
			/*float f = 255.0f/(nFadeOut-1);
			pdFadeOut[0] = 0;
			for(int i=1; i<nFadeOut; i++)
			{
			pdFadeOut[i] = (WORD)(pdFadeOut[i-1]+f);
			}*/
			for(int i=0; i<nFadeOut; i++)
			{
				pdFadeOut[i] = (WORD)((i/(float)(nFadeOut-1))*255);
			}
		}
	}
	if(nFadeIn > 0)
	{
		pdFadeIn = new WORD[nFadeIn];		// 淡入缓冲区
		if(nFadeIn == 1)
			pdFadeIn[0] = 128;
		else
		{
			/*float f = 255.0f/(nFadeIn-1);
			pdFadeIn[0] = 0;
			for(int i=1; i<nFadeIn; i++)
			{
			pdFadeIn[i] = (WORD)(pdFadeIn[i-1]+f);
			}*/
			for(int i=0; i<nFadeIn; i++)
			{
				pdFadeIn[i] = (WORD)((i/(float)(nFadeIn-1))*255);
			}
		}
	}

	_declspec(align(16)) WORD narPower[] = {nPower, nPower, nPower, nPower, nPower, nPower, nPower, nPower};
	_declspec(align(16)) WORD narSubPower[] = {nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower};
	_declspec(align(16)) DWORD dwFade[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	_declspec(align(16)) DWORD dwRepair[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	int nNextEnd = nSrcX -1;
	int nEnd = nSrcX;
	pDes += nDestWidth - 1;
//	DWORD* pCur = pSrc;
//	DWORD* pNext = pSrc - 1;	// 上一列
	//TPRGBA rgb, rgb1;
	//for(i=0; i<nDestWidth; i++)
	//{
	//	if(nSrcX - i - 1 < 0)
	//		pNext = pSrc + (nSrcBufferWidth - i - 1);
	//	if(nSrcX - i < 0)
	//		pCur = pSrc + (nSrcBufferWidth - i);
	//	for(j=0; j<nDestHeight;)
	//	{
	//		rgb.m_dwColor = *(pCur+j*nSrcBufferWidth);
	//		rgb1.m_dwColor = *(pNext+j*nSrcBufferWidth);
	//		rgb.m_bColor.bA = (BYTE)((rgb.m_bColor.bA*nSubPower+rgb1.m_bColor.bA*nPower)/255);
	//		if(i < nFadeOut)
	//			rgb.m_bColor.bA = (BYTE)(rgb.m_bColor.bA*pdFadeOut[i]/255);
	//		else if(i >= nFadeInStart)
	//			rgb.m_bColor.bA = (BYTE)(rgb.m_bColor.bA*pdFadeIn[nDestWidth-1-i]/255);
	//		rgb.m_bColor.bR = (BYTE)((rgb.m_bColor.bR*nSubPower+rgb1.m_bColor.bR*nPower)/255);
	//		rgb.m_bColor.bG = (BYTE)((rgb.m_bColor.bG*nSubPower+rgb1.m_bColor.bG*nPower)/255);
	//		rgb.m_bColor.bB = (BYTE)((rgb.m_bColor.bB*nSubPower+rgb1.m_bColor.bB*nPower)/255);
	//		*(pDes+j*nDestBufferWidth) = rgb.m_dwColor;
	//		j += nInc;
	//	}
	//	pDes--;		// 目标指针
	//	pCur--;		// 当前列指针
	//	pNext--;	// 上一列指针
	//}
	DWORD* pTempSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth;
	if(pdFadeIn != NULL || pdFadeOut != NULL)
	{
	__asm
	{
		XOR ECX, ECX
		PXOR XMM7, XMM7
		MOVDQA XMM6, narSubPower
		MOVDQA XMM5, narPower
		MOVDQA XMM3, dwRepair
		MOV EAX, pSrc			// 源当前指针
		MOV EDX, pDes			// 目标指针
LOOPX:
		CMP ECX, nDestWidth		// x<nDestWidth
		JGE LOOPXEND
		CMP ECX, nEnd		// x<=nEnd
		JLE  LOOPXSRC
		MOV EAX, nSrcBufferWidth	// 如果x>nEnd,则pCur=pSrc + (nSrcBufferWidth-i)
		SUB EAX, ECX
		ADD EAX, nSrcX
		SHL EAX, 2
		ADD EAX, pTempSrc
LOOPXSRC:
		MOV ESI, EAX			// 上一列指针
		SUB ESI, 4
		CMP ECX, nEnd		// x<nNextEnd
		JL LOOPXNEXT
		MOV ESI, nSrcBufferWidth		// 如果x>nNextEnd,则pNext=pSrc + (nSrcBufferWidth - i -1)
		DEC ESI
		SUB ESI, ECX
		ADD ESI, nSrcX
		SHL ESI, 2
		ADD ESI, pTempSrc
LOOPXNEXT:
		MOVDQA XMM4, dwFade		// 找到淡入淡出值:默认为0F0F0F0F0F0F0F0F
		CMP ECX, nFadeOut
		JGE  FADEOUT			// x>=nFadeOut时不淡出
		MOV EDI, ECX			// 算出当前淡出值
		SHL EDI, 1
		ADD EDI, pdFadeOut
		PINSRW XMM4, [EDI], 3
		PINSRW XMM4, [EDI], 7
		JMP FADEIN				// 有淡出时不用淡入
FADEOUT:
		CMP ECX, nFadeInStart	// 与淡入区起始点比较
		JL  FADEIN				// x<nFadeInStart，不淡入
		MOV EDI, nDestWidth		// 算出当前淡入值
		DEC EDI
		SUB EDI, ECX			
		SHL EDI, 1
		ADD EDI, pdFadeIn		// pdFadeIn[nDestWidth-1-i]
		PINSRW XMM4, [EDI], 3
		PINSRW XMM4, [EDI], 7
FADEIN:
		PUSH EDX
		PUSH EAX
		XOR EDI, EDI
LOOPY:
		ADD EDI, 2				// 每次计算2个象素点
		CMP EDI, nDestHeight	// y<nDestHeight
		JG  LOOPY1

		MOVD XMM0, [EAX]			// 得到源列数据
		ADD EAX, nAddSrcWidth
		MOVD XMM1, [EAX]
		PUNPCKLDQ XMM0, XMM1
		PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A
		PMULLW XMM0, XMM5		// pSrc*nSubPower

		MOVD XMM1, [ESI]			// 得到上一列数据
		ADD ESI, nAddSrcWidth
		MOVD XMM2, [ESI]
		PUNPCKLDQ XMM1, XMM2
		PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A

		ADD EAX, nAddSrcWidth	// 指针后移
		PMULLW XMM1, XMM6		// pSrcNext*nPower
		ADD ESI, nAddSrcWidth
		PADDUSW XMM0, XMM1		// 相加
		PADDUSW XMM0, XMM3		// 误差补偿
		PSRLW XMM0, 8			// 右移8位
		PMULLW XMM0, XMM4		// 乘以nFadeOut或nFadeIn
		PADDUSW XMM0, XMM3		// 误差补偿
		PSRLW XMM0, 8			// 右移8位
		PACKUSWB XMM0, XMM0		// 压缩

		MOVD [EDX], XMM0
		ADD  EDX, nAddDestWidth
		PSRLDQ XMM0, 4
		MOVD [EDX], XMM0
		ADD  EDX, nAddDestWidth

		JMP LOOPY
LOOPY1:
		SUB EDI, 2
		CMP EDI, nDestHeight
		JGE LOOPYEND

		MOVD XMM0, [EAX]			// 得到源列数据
		PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

		MOVD XMM1, [ESI]			// 得到下一列数据
		PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
		PMULLW XMM0, XMM5		// pSrc*nSubPower

		ADD EAX, nAddSrcWidth	// 指针后移
		PMULLW XMM1, XMM6		// pSrcNext*nPower
		ADD ESI, nAddSrcWidth
		PADDUSW XMM0, XMM1		// 相加
		PADDUSW XMM0, XMM3		// 误差补偿
		PSRLW XMM0, 8			// 右移8位
		PMULLW XMM0, XMM4		// 乘以nFadeOut
		PADDUSW XMM0, XMM3		// 误差补偿
		PSRLW XMM0, 8			// 右移8位
		PACKUSWB XMM0, XMM0		// 压缩
		MOVD [EDX], XMM0		// 赋值
		ADD EDX, nAddDestWidth
LOOPYEND:
		ADD ECX, nInc
		POP EAX
		SUB EAX, 4
		POP EDX
		SUB EDX, 4
		JMP LOOPX
LOOPXEND:
	}
	if(pdFadeOut)
		delete[] pdFadeOut;
	if(pdFadeIn)
		delete[] pdFadeIn;
	return TRUE;
	}
	__asm
	{
		XOR ECX, ECX
			PXOR XMM7, XMM7
			MOVDQA XMM6, narSubPower
			MOVDQA XMM5, narPower
			MOVDQA XMM3, dwRepair
			MOV EAX, pSrc			// 源当前指针
			MOV EDX, pDes			// 目标指针
LOOPX_1:
		CMP ECX, nDestWidth		// x<nDestWidth
			JGE LOOPXEND_1
			CMP ECX, nEnd		// x<=nEnd
			JLE  LOOPXSRC_1
			MOV EAX, nSrcBufferWidth	// 如果x>nEnd,则pCur=pSrc + (nSrcBufferWidth-i)
			SUB EAX, ECX
			ADD EAX, nSrcX
			SHL EAX, 2
			ADD EAX, pTempSrc
LOOPXSRC_1:
		MOV ESI, EAX			// 上一列指针
			SUB ESI, 4
			CMP ECX, nEnd		// x<nNextEnd
			JL LOOPXNEXT_1
			MOV ESI, nSrcBufferWidth		// 如果x>nNextEnd,则pNext=pSrc + (nSrcBufferWidth - i -1)
			DEC ESI
			SUB ESI, ECX
			ADD ESI, nSrcX
			SHL ESI, 2
			ADD ESI, pTempSrc
LOOPXNEXT_1:
		PUSH EDX
			PUSH EAX
			XOR EDI, EDI
LOOPY_1:
		ADD EDI, 2				// 每次计算2个象素点
			CMP EDI, nDestHeight	// y<nDestHeight
			JG  LOOPY1_1

			MOVD XMM0, [EAX]			// 得到源列数据
			ADD EAX, nAddSrcWidth
				MOVD XMM1, [EAX]
				PUNPCKLDQ XMM0, XMM1
					PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					MOVD XMM1, [ESI]			// 得到上一列数据
					ADD ESI, nAddSrcWidth
						MOVD XMM2, [ESI]
						PUNPCKLDQ XMM1, XMM2
							PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A

							ADD EAX, nAddSrcWidth	// 指针后移
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, nAddSrcWidth
							PADDUSW XMM0, XMM1		// 相加
							PADDUSW XMM0, XMM3		// 误差补偿
							PSRLW XMM0, 8			// 右移8位
							PACKUSWB XMM0, XMM0		// 压缩

							MOVD [EDX], XMM0
							ADD  EDX, nAddDestWidth
							PSRLDQ XMM0, 4
							MOVD [EDX], XMM0
							ADD  EDX, nAddDestWidth

							JMP LOOPY_1
LOOPY1_1:
						SUB EDI, 2
							CMP EDI, nDestHeight
							JGE LOOPYEND_1

							MOVD XMM0, [EAX]			// 得到源列数据
							PUNPCKLBW XMM0, XMM7	// 变成0R0G0B0A0R0G0B0A

								MOVD XMM1, [ESI]			// 得到下一列数据
								PUNPCKLBW XMM1, XMM7	// 变成0R0G0B0A0R0G0B0A
									PMULLW XMM0, XMM5		// pSrc*nSubPower

									ADD EAX, nAddSrcWidth	// 指针后移
									PMULLW XMM1, XMM6		// pSrcNext*nPower
									ADD ESI, nAddSrcWidth
									PADDUSW XMM0, XMM1		// 相加
									PADDUSW XMM0, XMM3		// 误差补偿
									PSRLW XMM0, 8			// 右移8位
									PACKUSWB XMM0, XMM0		// 压缩
									MOVD [EDX], XMM0		// 赋值
									ADD EDX, nAddDestWidth
LOOPYEND_1:
								ADD ECX, nInc
									POP EAX
									SUB EAX, 4
									POP EDX
									SUB EDX, 4
									JMP LOOPX_1
LOOPXEND_1:
	}
	return TRUE;
}