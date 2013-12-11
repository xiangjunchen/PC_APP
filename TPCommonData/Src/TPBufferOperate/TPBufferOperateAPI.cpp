/********************************************************************
Copyright (c) 2004, Sobey数码 内容制作部
All rights reserved.
创建时间:	2004年2月10日   18:14
文件名: 	c:\ndsrc\chenyufeng_desktop_2\devvob\sobeydesktop\src\tpcommon\tpbufferoperate\tpbufferoperateapi.cpp
当前版本：	1.0
作者:		陈玉峰
摘要:		对缓冲区各种操作的加速算法的实现
*********************************************************************/
#include "stdafx.h"
#include <math.h>
#include "TPBufferOperateAPI.h"
#include "TPSSE2.h"
#include "TPBufferOperateSSE.h"
#include <assert.h>

#include "CoreInclude\CPUInclude\NXVideoProc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

#define  TP_MEDIA_CLASS_V_G		 0x00001000
#define  TP_MEDIA_CLASS_V_P		 0x00002000
#define  TP_MEDIA_CLASS_V_V		 0x00004000
#define  TP_MEDIA_CLASS_V_SERIAL 0x00008000
#define RECTWIDTH(rc) (rc.right-rc.left)
#define RECTHEIGHT(rc) (rc.bottom-rc.top)

//////////////////////////////////////////////////////////////////////////
// 全局变量
extern float *g_pfTable;
extern float *g_pfPosTable;
extern LPNXVIDEOPROCAPI g_lpVideoProcAPI; 

//////////////////////////////////////////////////////////////////////////
// 私有函数声明
BOOL TPBuffer_Copy(DWORD* pdwDestBuffer, 
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
				   int nParity);
BOOL TPBuffer_Mix(DWORD* pdwDestBuffer, 
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
				  int nParity);
BOOL TPBuffer_NotMix(DWORD* pdwDestBuffer, 
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
					 int nParity);
BOOL TPBuffer_StretchCopy(DWORD* pdwDestBuffer, 
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
						  int nParity);
BOOL TPBuffer_StretchMix(DWORD* pdwDestBuffer, 
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
						 int nParity);
BOOL TPBuffer_StretchNotMix(DWORD* pdwDestBuffer, 
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
							int nParity);

BOOL TPBuffer_QuickMix(DWORD* pdwDestBuffer, 
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
					   int nParity);
BOOL TPBuffer_QuickNotMix(DWORD* pdwDestBuffer, 
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
						  int nParity);

//////////////////////////////////////////////////////////////////////////
// 导出函数

//////////////////////////////////////////////////////////////////////////
// 拷贝一行 Buffer
LRESULT CALLBACK TP_MemcpyLine(OUT void *des,		// 目标
							   IN void *src,		// 源
							   IN int nlen)			// 字节长度
{
	memcpy_line(des, src, (nlen >> 4), (nlen & 0x0f));

	return TRUE;
}

// 以字节为单位设置一行 Buffer
LRESULT CALLBACK TP_MemsetChar(OUT void *des,		// 目标
							   IN char value,		// 字节值
							   IN int nlen)			// 字节长度
{
	memset_char_line(des, value, (nlen >> 4), (nlen & 0x0f));

	return TRUE;
}

// 一行 Buffer 的双字设值操作
LRESULT CALLBACK TP_MemsetDWORD(OUT DWORD *des,		// 目标
								IN DWORD value,		// 源
								IN int nsize)		// 双字的个数
{
	memset_dword_line(des, value, (nsize >> 2), ((nsize & 0x03) << 2));

	return TRUE;
}

// 翻转
BOOL CALLBACK TP_OverTurnSelf(IN OUT void* pBuf,    // 内存
							  IN int nPitch,        // 一行字节宽度（四字节对齐）
							  IN int nHeight)       // 高度
{
	assert(pBuf != NULL);

	if (pBuf == NULL || nPitch < 0 || nHeight < 0)
		return FALSE;

	int nHalfHeight = (nHeight >> 1);
	BYTE* pTop = (BYTE*)pBuf, *pBottom = pTop + (nHeight - 1) * nPitch;
	int na16 = (nPitch >> 4), nm16 = (nPitch & 0x0f);
	BYTE* pTmpLine = TPSSE_NEW(nPitch, BYTE);

	for (int i=0; i<nHalfHeight; i++)
	{
		memcpy_line(pTmpLine, pTop, na16, nm16);
		memcpy_line(pTop, pBottom, na16, nm16);
		memcpy_line(pBottom, pTmpLine, na16, nm16);
		pTop += nPitch;
		pBottom -= nPitch;
	}
	TPSSE_FREE(pTmpLine);

	return TRUE;
}
BOOL CALLBACK TP_OverTurnCopy(OUT void* pDes,       // 目标
							  IN int nDesPitch,     // 目标一行字节宽度（四字节对齐）
							  IN void* pSrc,        // 源
							  IN int nSrcPitch,     // 源一行字节宽度（四字节对齐）
							  IN int nHeight)       // 高度
{
	assert(pDes != NULL && pSrc != NULL && pSrc != pDes);

	if (pDes == NULL || pSrc == NULL || pSrc == pDes
		|| nDesPitch < 0 || nHeight < 0 || nSrcPitch < 0)
		return FALSE;

	int nMinPitch = min(nDesPitch, nSrcPitch);
	BYTE *pDesLine = (BYTE*)pDes;
	BYTE *pSrcLine = (BYTE*)pSrc + (nHeight - 1) * nSrcPitch;
	int na16 = (nMinPitch >> 4), nm16 = (nMinPitch & 0x0f);

	for (int i=0; i<nHeight; i++)
	{
		memcpy_line(pDesLine, pSrcLine, na16, nm16);
		pDesLine += nDesPitch;
		pSrcLine -= nSrcPitch;
	}
	return TRUE;
}

// 将源 unit 中的某几个连续的 cell 分裂到连续目标中
// 一般 unit 的字节长为 32/16/8/4/2，cell 的位长为 8（也即一个字节）
// 例如将一个 BGRA 像素中的 A 通道取出，又如取 8 通道音频的第 1 通道
// 要求输入参数以及要拷贝的 cell 个数都是逻辑安全的
void CALLBACK TP_MemSplit(OUT void *des,			// 目标（分裂结果）
						  IN void *src,				// 源
						  IN int nUnitCount,		// 分裂 unit 的个数
						  IN int nUnitSize,			// unit 的字节长
						  IN int nCellStart,		// unit 中 cell 的分裂起点
						  IN int nCellCount)		// unit 中 cell 的分裂个数
{
	assert(des != NULL && src != NULL);
	assert(nUnitCount >= 0 && nUnitSize >= 2);
	assert(nCellStart >= 0 && nCellStart < nUnitSize && (nCellStart + nCellCount) <= nUnitSize);

	if (nCellStart == 0 && (nCellStart + nCellCount) == nUnitSize)
	{
		TP_MemcpyLine(des, src, nUnitCount * nUnitSize);
		return;
	}
	switch (nUnitSize)
	{
	case 2:		// 16 位
		if (nCellStart == 0)	// 分裂第一个字节
		{
			memsplit_2_0_1(des, src, nUnitCount);
		}
		else		// 分裂第二个字节
		{
			memsplit_2_1_1(des, src, nUnitCount);
		}
		break;
	case 4:		// 32 位
		switch (nCellStart)
		{
		case 0:
			if (nCellCount == 1)	// 分裂第一个字节
			{
				memsplit_4_0_1(des, src, nUnitCount);
			}
			else if (nCellCount == 2)	// 分裂前两个字节
			{
				memsplit_4_0_2(des, src, nUnitCount);
			}
			else	// 分裂前三个字节
			{
				memsplit_4_0_3(des, src, nUnitCount);
			}
			break;
		case 1:
			if (nCellCount == 1)	// 分裂第二个字节
			{
				memsplit_4_1_1(des, src, nUnitCount);
			}
			else if (nCellCount == 2)	// 分裂中间两个字节
			{
				memsplit_4_1_2(des, src, nUnitCount);
			}
			else		// 分裂后三个字节
			{
				memsplit_4_1_3(des, src, nUnitCount);
			}
			break;
		case 2:
			if (nCellCount == 1)	// 分裂第三个字节
			{
				memsplit_4_2_1(des, src, nUnitCount);
			}
			else	// 分裂最后两个字节
			{
				memsplit_4_2_2(des, src, nUnitCount);
			}
			break;
		case 3:			// 分裂最后一个字节
			memsplit_4_3_1(des, src, nUnitCount);
			break;
		}
		break;
	case 8:
		if (nCellCount != 4 || (nCellStart & 0x03) != 0)	// 不是音频特殊情况，采用任意分裂
		{
			memsplit_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
		}
		else		// 2 通道音频取 1 通道
		{
			if (nCellStart == 0)
			{
				memsplit_8_0_4(des, src, nUnitCount);
			}
			else
			{
				memsplit_8_4_4(des, src, nUnitCount);
			}
		}
		break;
	default:	// 任意分裂
		memsplit_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
		break;
	}
}
// 将源中某几个连续的 cell 合成到的目标 unit 中
// 一般 unit 的字节长为 32/16/8/4/2，cell 的位长为 8（也即一个字节）
// 例如将一个 A 通道合成到一个 BGRA 像素中，又如将 1 通道的音频合成到 8 通道中
// 要求输入参数以及要拷贝的 cell 个数都是逻辑安全的
void CALLBACK TP_MemCompose(OUT void *des,			// 目标（合成结果）
							IN void *src,			// 源
							IN int nUnitCount,		// 合成 unit 的个数
							IN int nUnitSize,		// unit 的字节长
							IN int nCellStart,		// unit 中 cell 的合成起点
							IN int nCellCount)		// unit 中 cell 的合成个数
{
	assert(des != NULL && src != NULL);
	assert(nUnitCount >= 0 && nUnitSize >= 2);
	assert(nCellStart >= 0 && nCellStart < nUnitSize && (nCellStart + nCellCount) <= nUnitSize);

	if (nCellStart == 0 && (nCellStart + nCellCount) == nUnitSize)
	{
		TP_MemcpyLine(des, src, nUnitCount * nUnitSize);
		return;
	}
	switch (nUnitSize)
	{
	case 2:		// 16 位
		if (nCellStart == 0)	// 合成到第一个字节
		{
			memcompose_2_0_1(des, src, nUnitCount);
		}
		else		// 合成到第二个字节
		{
			memcompose_2_1_1(des, src, nUnitCount);
		}
		break;
	case 4:		// 32 位
		switch (nCellStart)
		{
		case 0:
			if (nCellCount == 1)	// 合成到第一个字节
			{
				memcompose_4_0_1(des, src, nUnitCount);
			}
			else if (nCellCount == 2)	// 合成到前两个字节
			{
				memcompose_4_0_2(des, src, nUnitCount);
			}
			else	// 合成到前三个字节，采用任意合成
			{
				memcompose_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
			}
			break;
		case 1:
			if (nCellCount == 1)	// 合成到第二个字节
			{
				memcompose_4_1_1(des, src, nUnitCount);
			}
			else		// 合成到中间两个字节或后三个字节，采用任意合成
			{
				memcompose_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
			}
			break;
		case 2:
			if (nCellCount == 1)	// 合成到第三个字节
			{
				memcompose_4_2_1(des, src, nUnitCount);
			}
			else	// 合成到最后两个字节
			{
				memcompose_4_2_2(des, src, nUnitCount);
			}
			break;
		case 3:			// 合成到最后一个字节
			memcompose_4_3_1(des, src, nUnitCount);
			break;
		}
		break;
	case 8:
		if (nCellCount != 4 || (nCellStart & 0x03) != 0)	// 不是音频特殊情况，采用任意合成
		{
			memcompose_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
		}
		else		// 1 通道音频合成到 2 通道中 
		{
			if (nCellStart == 0)
			{
				memcompose_8_0_4(des, src, nUnitCount);
			}
			else
			{
				memcompose_8_4_4(des, src, nUnitCount);
			}
		}
		break;
	default:	// 任意合成
		memcompose_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// RGB Mean 缩放
LRESULT CALLBACK TP_StretchBlt_Mean(IN OUT DWORD* pdwDestBuffer,
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
									IN int nMix,
									IN int nParity)
{
	switch(nMix)
	{
	case TP_BUFFER_COPY:
		return TPBGRA_MeanStretchCopy_SSE(pdwDestBuffer,
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
			nParity);
	case TP_BUFFER_MIX:
		return TPBGRA_MeanStretchMix_SSE(pdwDestBuffer,
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
			nParity);
	case TP_BUFFER_NOTMIX:
		return TPBGRA_MeanStretchNotMix_SSE(pdwDestBuffer,
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
			nParity);
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// 功能：按指定方式拷贝源缓冲区到目标缓冲区
// 参数：pdwDestBuffer:目标缓冲区指针
//		 nDestX:目标缓冲区横坐标
//		 nDestY:目标缓冲区纵坐标
//		 nDestWidth:目标缓冲区宽度
//		 nDestHeight:目标缓冲区高度
//		 nDestBufferWidth:目标缓冲区已有宽度
//		 nDestBufferHeight:目标缓冲区已有高度
//		 pdwSrcBuffer:源缓冲区指针
//		 nSrcX:源缓冲区横坐标
//		 nSrcY:源缓冲区纵坐标
//		 nSrcWidth:源缓冲区宽度
//		 nSrcHeight:源缓冲区高度
//		 nSrcBufferWidth:源缓冲区已有宽度
//		 nSrcBufferHeight:源缓冲区已有高度
//		 nMix:拷贝类型
//		 nParity:奇偶场(当奇行扫描时画偶行，偶行扫描时画奇行)
// 返回值：是否拷贝成功
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_BitBlt(DWORD* pdwDestBuffer, 
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
						   int nMix,
						   int nParity)
{
	if (pdwSrcBuffer == NULL)
	{
		return E_FAIL;
	}
	// 当两Buffer长宽不等时，拉伸拷贝
	if(nSrcWidth == nDestWidth&&nSrcHeight == nDestHeight)
	{
		switch(nMix)
		{
		case TP_BUFFER_COPY:
			/*return TPBuffer_Copy(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_Copy_SSE(pdwDestBuffer, 
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
				nParity);
		case TP_BUFFER_MIX:
			/*return TPBuffer_Mix(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_Mix_SSE(pdwDestBuffer, 
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
				nParity);
		case TP_BUFFER_NOTMIX:
			/*return TPBuffer_NotMix(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_NotMix_SSE(pdwDestBuffer, 
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
				nParity);
		default:
			return E_FAIL;
		}
	}
	else
	{
		switch(nMix)
		{
		case TP_BUFFER_COPY:
			/*return TPBuffer_StretchCopy(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_BLStretchCopy_SSE(pdwDestBuffer, 
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
				nParity);
		case TP_BUFFER_MIX:
			/*return TPBuffer_StretchMix(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_BLStretchMix_SSE(pdwDestBuffer, 
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
				nParity);
		case TP_BUFFER_NOTMIX:
			/*return TPBuffer_StretchNotMix(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_BLStretchNotMix_SSE(pdwDestBuffer, 
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
				nParity);
		default:
			return E_FAIL;
		}
	}
	return E_FAIL;
}
//////////////////////////////////////////////////////////////////////////
// 功能：简单的BitBlt算法：速度快，用于非编中的快速BitBlt
// 
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_QuickBitBlt(
								IN OUT DWORD* pdwDestBuffer, // 目标缓冲区
								IN int nDestX,				 // 要操作的相对于目标缓冲区的起点坐标
								IN int nDestY,
								IN int nDestWidth,			 // 要操作的目标宽、高
								IN int nDestHeight,
								IN int nDestBufferWidth,	 // 目标缓冲区宽、高
								IN int nDestBufferHeight,
								IN DWORD* pdwSrcBuffer,		 // 源缓冲区
								IN int nSrcX,				 // 要操作的相对于源缓冲区的起点坐标
								IN int nSrcY,
								IN int nSrcWidth,			 // 要操作的源宽、高
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		 // 源缓冲区宽、高
								IN int nSrcBufferHeight,
								IN int nMix,				 // 两缓冲区间的操作模式：TP_BUFFER_MIX, TP_BUFFER_COPY, TP_BUFFER_NOMIX
								IN int nParity)				 // 奇偶场：TP_PARITY_BOTH, TP_PARITY_ODD, TP_PARITY_EVEN
{
	if(nSrcWidth == nDestWidth&&nSrcHeight == nDestHeight)
	{
		switch(nMix)
		{
		case TP_BUFFER_COPY:
			/*return TPBuffer_Copy(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_Copy_SSE(pdwDestBuffer, 
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
				nParity);
		case TP_BUFFER_MIX:
			return TPBuffer_QuickMix(pdwDestBuffer, 
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
				nParity);
		case TP_BUFFER_NOTMIX:
			return TPBuffer_QuickNotMix(pdwDestBuffer, 
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
				nParity);
		default:
			return FALSE;
		}
	}
	else
	{
		switch(nMix)
		{
		case TP_BUFFER_COPY:
			/*return TPBuffer_StretchCopy(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_BLStretchCopy_SSE(pdwDestBuffer, 
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
				nParity);
		case TP_BUFFER_MIX:
			/*return TPBuffer_StretchMix(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_BLStretchMix_SSE(pdwDestBuffer, 
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
				nParity);
		case TP_BUFFER_NOTMIX:
			/*return TPBuffer_StretchNotMix(pdwDestBuffer, 
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
			nParity);*/
			return TPBGRA_BLStretchNotMix_SSE(pdwDestBuffer, 
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
				nParity);
		default:
			return FALSE;
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
// 功能：拉伸拷贝
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_StretchBlt(
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
							   IN int nMix,					// 混叠模式
							   IN int nParity)				// 奇偶场
{
	switch(nMix)
	{
	case TP_BUFFER_COPY:
		/*return TPBuffer_StretchCopy(pdwDestBuffer, 
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
		nParity);*/
		return TPBGRA_BLStretchCopy_SSE(pdwDestBuffer, 
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
			nParity);
	case TP_BUFFER_MIX:
		/*return TPBuffer_StretchMix(pdwDestBuffer, 
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
		nParity);*/
		return TPBGRA_BLStretchMix_SSE(pdwDestBuffer, 
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
			nParity);
	case TP_BUFFER_NOTMIX:
		/*return TPBuffer_StretchNotMix(pdwDestBuffer, 
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
		nParity);*/
		return TPBGRA_BLStretchNotMix_SSE(pdwDestBuffer, 
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
			nParity);
	default:
		return E_FAIL;
	}
	return E_FAIL;
}
//////////////////////////////////////////////////////////////////////////
// 功能：根据加权算子计算Mask值:blur掩模，反走样操作
// 参数：pbyMask：要计算的掩模缓冲区指针
//		 nStartX：掩模起点X坐标
//		 nStartY：掩模起点Y坐标
//		 nMaskWidth：掩模宽度
//		 nMaskHeight：掩模高度
//		 pnOperator：加权算子数组指针
//		 nOperatorWidth：数组宽度
//		 nOperatorHeight：数组高度
//		 nMainOperatorX：主象素点X坐标
//		 nMainOperatorY：主象素点Y坐标
//////////////////////////////////////////////////////////////////////////
void CALLBACK TP_CalMask(IN OUT BYTE *pbyMask,
						 IN int nStartX, 
						 IN int nStartY, 
						 IN int nMaskWidth,
						 IN int nMaskHeight,
						 IN BYTE *pbyOperator,
						 IN int nOperatorWidth, 
						 IN int nOperatorHeight,
						 IN int nMainOperatorX,
						 IN int nMainOperatorY)
{
	return ;
	assert(pbyMask != NULL);
	if(pbyOperator == NULL)
		return;
	//if(nMaskWidth<=64&&nMaskHeight<=64)
	//	return;
	if(nOperatorWidth <= 0 || nOperatorHeight <= 0)
		return;
	if(nStartX >= nMaskWidth || nStartY >= nMaskHeight)
		return;
	if(nStartX < 0)
		nStartX = 0;
	if(nStartY < 0)
		nStartY = 0;
	int nWidth = nMaskWidth - nStartX;			// 要计算的Mask宽度与高度
	int nHeight = nMaskHeight - nStartY;
	if(nWidth <= 0 || nHeight <= 0)
		return;
	if(nMainOperatorX < 0 || nMainOperatorY < 0 || nMainOperatorX >= nOperatorWidth || nMainOperatorY >= nOperatorHeight)
		return;
	//	nWidth = nWidth - (nOperatorWidth - 1);	// 要计算的宽度与高度
	//	nHeight = nHeight - (nOperatorHeight - 1);
	BYTE *pSrcMask = pbyMask + nStartY*nMaskWidth + nStartX;	// 起点指针
	int nCalSum = 0;
	for(int i=0; i<nOperatorHeight; i++)
	{
		for(int j=0; j<nOperatorWidth; j++)
			nCalSum += *(pbyOperator+i*nOperatorWidth+j);
	}
	float fCalSum = 1.0f/nCalSum;
	BYTE *pTempBuf = new BYTE[nHeight*nWidth];
	if(pTempBuf == NULL)
		exit(1);
	memset(pTempBuf, 0, sizeof(BYTE)*(nHeight*nWidth));
	BYTE *pTempBuf1 = pTempBuf;
	BYTE *pDest = pSrcMask;

#if 0
	// 一次算Mask四个点
	__asm
	{
		XOR ECX, ECX
			MOVSS XMM7, fCalSum
			PSHUFD XMM7, XMM7, 0		// 存放4个nCalSum
			MOV EDX, pTempBuf1			// 临时缓冲区指针
LOOPX:
		CMP ECX, nHeight				// x<nHeight
			JGE LOOPXEND

			PUSH ECX
			XOR ECX, ECX
			MOV EAX, pDest					// 临时象素点指针
LOOPY:
		ADD ECX, 4
			CMP ECX, nWidth				// y<nWidth
			JG LOOPY2

			PUSH ECX
			PUSH EAX
			PUSH EDX
		{
			XOR ECX, ECX
				PXOR XMM0, XMM0		// 存放计算的结果
				MOV EDX, pbyOperator
LOOPI:
			CMP ECX, nOperatorHeight
				JGE LOOPIEND
				PUSH ECX
				XOR ECX, ECX
LOOPJ:
			CMP ECX, nOperatorWidth
				JGE LOOPJEND

				PUSH EAX
				XOR EAX, EAX
				MOV  AL,  [ECX+EDX]			// 读出pbyTempOperator[j]值到AX的低8位
				MOVD XMM1,EAX 
					POP EAX	
					PSHUFD XMM1, XMM1, 0		// 打包，XMM1存四个相同的值
					MOVD XMM2, [ECX+EAX]		// 读出pbyTempDest[j]后面的四个BYTE值
					PXOR XMM3, XMM3				// 转换成4个DWORD值
						PUNPCKLBW XMM2, XMM3
						PUNPCKLWD XMM2, XMM3
						PMULLW XMM1, XMM2			// pbyTempOperator[j]*pbyTempDest[j]
						INC ECX
						PADDD XMM0, XMM1			// nSum += pbyTempOperator[j]*pbyTempDest[j]

						JMP LOOPJ
LOOPJEND:
					POP ECX
						INC ECX
						ADD EDX, nOperatorWidth			// pbyTempOperator += nOperatorWidth;
						ADD EAX, nMaskWidth				// pbyTempDest += nMaskWidth;
						JMP LOOPI
LOOPIEND:
					CVTDQ2PS XMM0, XMM0				// 转换成float
						MULPS XMM0,	XMM7				// nSum*1.0f/nCalSum
						POP EDX
						POP EAX
						POP ECX
						ADD EAX, 4							// pDest+4
		}
		CVTPS2DQ XMM0, XMM0					// 转换成DWORD
			PACKSSDW XMM0, XMM0					// 解包成4个Byte放在最后4字节
			PACKUSWB XMM0, XMM0						
			MOVD     [EDX], XMM0				// *pTempBuf1 = nSum/nCalSum;
			ADD EDX, 4							// pTempBuf1+4,后移4

			JMP LOOPY
LOOPY2:
		SUB ECX, 4
LOOPY3:
		CMP ECX, nWidth
			JGE LOOPYEND

			PUSH ECX
			PUSH EAX
			PUSH EDX
		{
			XOR ECX, ECX
				PXOR XMM0, XMM0		// 存放计算的结果
				MOV EDX, pbyOperator
LOOPI2:
			CMP ECX, nOperatorHeight
				JGE LOOPIEND2
				PUSH ECX
				XOR ECX, ECX
LOOPJ2:
			CMP ECX, nOperatorWidth
				JGE LOOPJEND2

				PUSH EAX
				XOR EAX, EAX
				MOV  AL,  [ECX+EDX]			// 读出pbyTempOperator[j]值
				MOVD XMM1,EAX 
					POP EAX	
					PUSH EDX					// 读出pbyTempDest[j]
					XOR EDX, EDX
					MOV DL, [ECX+EAX]
					MOVD XMM2, EDX
						POP EDX 		
						PXOR XMM3, XMM3				// 转换成4个DWORD值
						PUNPCKLBW XMM2, XMM3
						PUNPCKLWD XMM2, XMM3
						PMULLW XMM1, XMM2			// pbyTempOperator[j]*pbyTempDest[j]
						INC ECX
						PADDD XMM0, XMM1			// nSum += pbyTempOperator[j]*pbyTempDest[j]

						JMP LOOPJ2
LOOPJEND2:
					POP ECX
						INC ECX
						ADD EDX, nOperatorWidth			// pbyTempOperator += nOperatorWidth;
						ADD EAX, nMaskWidth				// pbyTempDest += nMaskWidth;
						JMP LOOPI2
LOOPIEND2:
					CVTDQ2PS XMM0, XMM0				// 转换成float
						MULPS XMM0,	XMM7				// nSum*1.0f/nCalSum
						POP EDX
						POP EAX
						POP ECX
						ADD EAX, 1							// pDest+1
		}
		CVTPS2DQ XMM0, XMM0					// 转换成DWORD
			PACKSSDW XMM0, XMM0					// 解包成4个Byte放在最后4字节
			PACKUSWB XMM0, XMM0		
			PUSH EAX
			MOVD  EAX, XMM0
			MOV     [EDX], AL					// *pTempBuf1 = nSum/nCalSum;
			POP EAX
			ADD EDX, 1							// pTempBuf1+1,后移1

			INC ECX
			JMP LOOPY3
LOOPYEND:
		MOV ECX, nMaskWidth
			ADD [pDest], ECX
			POP ECX
			INC ECX
			JMP LOOPX
LOOPXEND:
		EMMS
	}
	pTempBuf1 = pTempBuf;
	pSrcMask = pbyMask +(nStartY+nMainOperatorY)*nMaskWidth + (nStartX+nMainOperatorX);
	pDest = pSrcMask;
	__asm
	{
		XOR ECX, ECX
			MOV EAX, pTempBuf1
			MOV EDX, pDest
LOOPY1:
		CMP ECX, nHeight
			JGE LOOPY1END
			PUSH ECX
			XOR ECX, ECX
LOOPX1:
		Add ECX, 4
			CMP ECX, nWidth
			JG LOOPX2
			SUB ECX, 4
			MOVD MM0, [EAX]
			MOVD [EDX+ECX], MM0
				ADD EAX, 4
				ADD ECX, 4
				JMP LOOPX1
LOOPX2:
			SUB ECX, 4
LOOPX3:
			CMP ECX, nWidth
				JGE LOOPX1END
				PUSH EAX
				PUSH EDX
				MOV DL, [EAX]
				MOV AL, DL
					POP EDX
					MOV [EDX+ECX], AL
					POP EAX
					INC EAX
					INC ECX
					JMP LOOPX3
LOOPX1END:
				POP ECX
					INC ECX
					ADD EDX, nMaskWidth
					JMP LOOPY1
LOOPY1END:
				EMMS
	}

	delete[] pTempBuf;
#endif
#if 0
	for(int y=0; y<nHeight; y++)
	{
		for(int x=0; x<nWidth/4; x++)
		{
			for(int k=0; k<4; k++)
			{
				int nSum = 0;
				BYTE *pbyTempDest = pDest;// - nMainOperatorY*nMaskWidth - nMainOperatorX;
				BYTE *pbyTempOperator = pbyOperator;
				for(int i=0; i<nOperatorHeight; i++)
				{
					for(int j=0; j<nOperatorWidth; j++)
					{
						nSum += pbyTempOperator[j]*pbyTempDest[j];
					}
					pbyTempOperator += nOperatorWidth;
					pbyTempDest += nMaskWidth;
				}
				*pTempBuf1 = nSum/nCalSum;
				pTempBuf1++;
				pDest++;
			}
		}
		for(x=0; x<nWidth%4; x++)
		{
			int nSum = 0;
			BYTE *pbyTempDest = pDest;// - nMainOperatorY*nMaskWidth - nMainOperatorX;
			BYTE *pbyTempOperator = pbyOperator;
			for(int i=0; i<nOperatorHeight; i++)
			{
				for(int j=0; j<nOperatorWidth; j++)
				{
					nSum += pbyTempOperator[j]*pbyTempDest[j];
				}
				pbyTempOperator += nOperatorWidth;
				pbyTempDest += nMaskWidth;
			}
			*pTempBuf1 = nSum/nCalSum;
			pTempBuf1++;
			pDest++;
		}
		pSrcMask += nMaskWidth;
		pDest = pSrcMask;
	}

	pTempBuf1 = pTempBuf;
	pSrcMask = pbyMask +(nStartY+nMainOperatorY)*nMaskWidth + (nStartX+nMainOperatorX);
	pDest = pSrcMask;
	for(int y=0; y<nHeight; y++)
	{
		for(int x=0; x<nWidth; x++)
		{
			*pDest = *pTempBuf1;
			pTempBuf1++;
			pDest++;
		}
		pSrcMask += nMaskWidth;
		pDest = pSrcMask;
	}

	delete[] pTempBuf;
#endif

	// 添加于2004.2.1,添加者：陈玉峰
	// 添加了对Mask边缘的计算
#if 1								// 其汇编实现
	int nCurHeight = 0;					// 当前坐标点
	int nCurWidth = 0;
	int nCurX = 0;
	int nCurY = 0;
	int nCurOperateHeight = 0;
	int nCurOperateWidth = 0;
	// 一次算Mask四个点
	__asm
	{
		MOVSS XMM7, fCalSum
			PSHUFD XMM7, XMM7, 0		// 存放4个fCalSum在XMM7中
			MOV EDX, pTempBuf1			// 临时缓冲区指针
LOOPX:
		MOV ECX, nCurHeight
			CMP ECX, nHeight			// nCurHeight<nHeight
			JGE LOOPXEND
LOOPY:
		MOV ECX, nCurWidth
			ADD ECX, 4
			CMP ECX, nWidth				// nCurWidth<nWidth
			JG LOOPY2

			PUSH EDX
		{
			PXOR XMM0, XMM0		// 存放计算的结果SUM
				MOV EDX, pbyOperator// 算子指针
LOOPI:
			MOV ECX, nCurOperateHeight
				CMP ECX, nOperatorHeight	// nCurOperateHeight<nOperatorHeight
				JGE LOOPIEND

				MOV EAX, nCurHeight			// 计算相对的CurY,存入nCurY
				ADD EAX, ECX
				SUB EAX, nMainOperatorY
				CMP EAX, 0					// nCurY>=0且nCurY<nHeight
				JL  LOOPJEND
				CMP EAX, nHeight
				JGE LOOPJEND
				MOV nCurY, EAX
LOOPJ:
			MOV ECX, nCurOperateWidth
				CMP ECX, nOperatorWidth		// nCurOperateWidth<nOperatorWidth
				JGE LOOPJEND

				XOR EAX, EAX
				MOV  AL,  [ECX+EDX]			// 读出pbyOperator[nCurOperateWidth]值
				MOVD XMM1,EAX 
					PSHUFD XMM1, XMM1, 0		// 打包，XMM1存四个相同的算子值

					PXOR XMM2, XMM2				// 存放pDest的值
					PXOR XMM3, XMM3
					PXOR XMM4, XMM4
					PXOR XMM5, XMM5
					PUSH EDX
					MOV EAX, nCurY				// pDest+nCurY*nMaskWidth
					MUL nMaskWidth
					MOV EDX, pDest
					ADD EDX, EAX

					MOV ECX, nCurWidth			// 计算相对的CurX
					ADD ECX, nCurOperateWidth
					SUB ECX, nMainOperatorX
					CMP ECX, 0					// CurX>=0且nCurX<nWidth
					JL  CurX1
					CMP ECX, nWidth
					JGE CurX1
					XOR EAX, EAX
					MOV AL,  [EDX+ECX]			// 取出pDest[nCurX][nCurY]的值
					MOVD XMM2, EAX
CurX1:
					INC ECX						// CurX+1
						CMP ECX, 0					// CurX>=0且nCurX<nWidth
						JL  CurX2
						CMP ECX, nWidth
						JGE CurX2
						XOR EAX, EAX
						MOV AL,  [EDX+ECX]			// 取出pDest[nCurX][nCurY]的值
						MOVD XMM3, EAX
							PUNPCKLDQ XMM2, XMM3
CurX2:
						INC ECX						// CurX+1
							CMP ECX, 0					// CurX>=0且nCurX<nWidth
							JL  CurX3
							CMP ECX, nWidth
							JGE CurX3
							XOR EAX, EAX
							MOV AL,  [EDX+ECX]			// 取出pDest[nCurX][nCurY]的值
							MOVD XMM4, EAX
CurX3:
							INC ECX						// CurX+1
								CMP ECX, 0					// CurX>=0且nCurX<nWidth
								JL  CurXEnd			
								CMP ECX, nWidth
								JGE CurXEnd
								XOR EAX, EAX
								MOV AL,  [EDX+ECX]			// 取出pDest[nCurX][nCurY]的值
								MOVD XMM5, EAX
									PUNPCKLDQ XMM4, XMM5
CurXEnd:
								PUNPCKLQDQ XMM2, XMM4		// 组合成4个DWORD放入XMM2
									POP EDX

									PMULLW XMM1, XMM2			// pbyOperator[j]*pDest[j]
									PADDD XMM0, XMM1			// nSum += pOperator[j]*pDest[j]

									MOV ECX, nCurOperateWidth
									INC ECX
									MOV nCurOperateWidth, ECX
									JMP LOOPJ
LOOPJEND:
								MOV nCurOperateWidth, 0		// nCurOperateWidth 清0
									MOV ECX, nCurOperateHeight
									INC ECX
									MOV nCurOperateHeight, ECX
									ADD EDX, nOperatorWidth			// pbyTempOperator += nOperatorWidth;
									JMP LOOPI
LOOPIEND:
								MOV nCurOperateHeight, 0		// nCurOperateHeight 清0
									CVTDQ2PS XMM0, XMM0				// 转换成float
									MULPS XMM0,	XMM7				// nSum*1.0f/nCalSum
									POP EDX							// 弹出pTempBuf指针
		}
		CVTPS2DQ XMM0, XMM0					// 转换成DWORD
			PACKSSDW XMM0, XMM0					// 解包成4个Byte放在最后4字节
			PACKUSWB XMM0, XMM0						
			MOVD     [EDX], XMM0				// *pTempBuf1 = nSum/nCalSum;
			ADD EDX, 4							// pTempBuf1+4,后移4

			MOV ECX, nCurWidth
			ADD ECX, 4
			MOV nCurWidth, ECX
			JMP LOOPY
LOOPY2:
		MOV ECX, nCurWidth
LOOPY3:
		CMP ECX, nWidth
			JGE LOOPYEND

			PUSH EDX
		{
			PXOR XMM0, XMM0		// 存放计算的结果SUM
				MOV EDX, pbyOperator// 算子指针
LOOPI1:
			MOV ECX, nCurOperateHeight
				CMP ECX, nOperatorHeight	// nCurOperateHeight<nOperatorHeight
				JGE LOOPIEND1

				MOV EAX, nCurHeight			// 计算相对的CurY,存入nCurY
				ADD EAX, ECX
				SUB EAX, nMainOperatorY
				CMP EAX, 0					// nCurY>=0且nCurY<nHeight
				JL  LOOPJEND1
				CMP EAX, nHeight
				JGE LOOPJEND1
				MOV nCurY, EAX
LOOPJ1:
			MOV ECX, nCurOperateWidth
				CMP ECX, nOperatorWidth		// nCurOperateWidth<nOperatorWidth
				JGE LOOPJEND1

				XOR EAX, EAX
				MOV  AL,  [ECX+EDX]			// 读出pbyOperator[nCurOperateWidth]值
				MOVD XMM1,EAX 

					PXOR XMM2, XMM2				// 存放pDest的值
					PUSH EDX
					MOV EAX, nCurY				// pDest+nCurY*nMaskWidth
					MUL nMaskWidth
					MOV EDX, pDest
					ADD EDX, EAX

					MOV ECX, nCurWidth			// 计算相对的CurX
					ADD ECX, nCurOperateWidth
					SUB ECX, nMainOperatorX
					CMP ECX, 0					// CurX>=0且nCurX<nWidth
					JL  CurXEnd1
					CMP ECX, nWidth
					JGE CurXEnd1
					XOR EAX, EAX
					MOV AL,  [EDX+ECX]			// 取出pDest[nCurX][nCurY]的值
					MOVD XMM2, EAX
CurXEnd1:
					POP EDX

						PMULLW XMM1, XMM2			// pbyOperator[j]*pDest[j]
						PADDD XMM0, XMM1			// nSum += pOperator[j]*pDest[j]

						MOV ECX, nCurOperateWidth
						INC ECX
						MOV nCurOperateWidth, ECX
						JMP LOOPJ1
LOOPJEND1:
					MOV nCurOperateWidth, 0		// nCurOperateWidth 清0
						MOV ECX, nCurOperateHeight
						INC ECX
						MOV nCurOperateHeight, ECX
						ADD EDX, nOperatorWidth			// pbyTempOperator += nOperatorWidth;
						JMP LOOPI1
LOOPIEND1:
					MOV nCurOperateHeight, 0		// nCurOperateHeight 清0
						CVTDQ2PS XMM0, XMM0				// 转换成float
						MULPS XMM0,	XMM7				// nSum*1.0f/nCalSum
						POP EDX							// 弹出pTempBuf指针
		}
		CVTPS2DQ XMM0, XMM0					// 转换成DWORD
			PACKSSDW XMM0, XMM0					// 解包成4个Byte放在最后4字节
			PACKUSWB XMM0, XMM0						
			MOVD     EAX, XMM0				// *pTempBuf1 = nSum/nCalSum;
			MOV      [EDX], AL
			ADD EDX, 1							// pTempBuf1+1,后移1

			MOV ECX, nCurWidth
			INC ECX
			MOV nCurWidth, ECX
			JMP LOOPY3
LOOPYEND:
		MOV nCurWidth, 0				// nCurWidth 清0
			MOV ECX, nCurHeight
			INC ECX
			MOV nCurHeight, ECX
			JMP LOOPX
LOOPXEND:
		EMMS
	}
	pTempBuf1 = pTempBuf;
	__asm
	{
		XOR ECX, ECX
			MOV EAX, pTempBuf1
			MOV EDX, pDest
LOOPY1:
		CMP ECX, nHeight
			JGE LOOPY1END
			PUSH ECX
			XOR ECX, ECX
LOOPX1:
		Add ECX, 4
			CMP ECX, nWidth
			JG LOOPX2
			SUB ECX, 4
			MOVD MM0, [EAX]
			MOVD [EDX+ECX], MM0
				ADD EAX, 4
				ADD ECX, 4
				JMP LOOPX1
LOOPX2:
			SUB ECX, 4
LOOPX3:
			CMP ECX, nWidth
				JGE LOOPX1END
				PUSH EAX
				PUSH EDX
				MOV DL, [EAX]
				MOV AL, DL
					POP EDX
					MOV [EDX+ECX], AL
					POP EAX
					INC EAX
					INC ECX
					JMP LOOPX3
LOOPX1END:
				POP ECX
					INC ECX
					ADD EDX, nMaskWidth
					JMP LOOPY1
LOOPY1END:
				EMMS
	}

	delete[] pTempBuf;
#endif
#if 0
	int nSum = 0;
	for(int y=0; y<nHeight; y++)
	{
		for(int x=0; x<nWidth/4; x++)
		{
			for(int k=0; k<4; k++)
			{
				nSum = 0;
				BYTE *pbyTempOperator = pbyOperator;
				for(int i=0; i<nOperatorHeight; i++)
				{
					for(int j=0; j<nOperatorWidth; j++)
					{
						// 当前点坐标
						int nCurX = x*4 + k - nMainOperatorX + j;
						int nCurY = y - nMainOperatorY + i;
						if(nCurX>=0 && nCurX<nWidth && nCurY>=0 && nCurY<nHeight)
							nSum += pbyTempOperator[j]*(*(pDest+nCurY*nWidth+nCurX));
					}
					pbyTempOperator += nOperatorWidth;
				}
				*pTempBuf1 = nSum/nCalSum;
				pTempBuf1++;
			}
		}
		for(x=x*4; x<nWidth; x++)
		{
			nSum = 0;
			BYTE *pbyTempOperator = pbyOperator;
			for(int i=0; i<nOperatorHeight; i++)
			{
				for(int j=0; j<nOperatorWidth; j++)
				{
					// 当前点坐标
					int nCurX = x - nMainOperatorX + j;
					int nCurY = y - nMainOperatorY + i;
					if(nCurX>=0 && nCurX<nWidth && nCurY>=0 && nCurY<nHeight)
						nSum += pbyTempOperator[j]*(*(pDest+nCurY*nWidth+nCurX));
				}
				pbyTempOperator += nOperatorWidth;
			}
			*pTempBuf1 = nSum/nCalSum;
			pTempBuf1++;
		}
	}

	pTempBuf1 = pTempBuf;
	pSrcMask = pbyMask +nStartY*nMaskWidth + nStartX;
	pDest = pSrcMask;
	for(int y=0; y<nHeight; y++)
	{
		for(int x=0; x<nWidth; x++)
		{
			*pDest = *pTempBuf1;
			pTempBuf1++;
			pDest++;
		}
		pSrcMask += nMaskWidth;
		pDest = pSrcMask;
	}

	delete[] pTempBuf;
#endif
}
//////////////////////////////////////////////////////////////////////////
// 功能：填充掩模
// 
// 2003－10－10，V1.0,编写者：陈玉峰
//
// 2003－11－20，V2.0,修改者：陈玉峰
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK TP_FillMaskColor(
							   IN OUT DWORD* pDesBuffer, 
							   IN int nStartX,						// 要Mask的目标起点
							   IN int nStartY, 
							   IN int nWidth,						// 目标缓冲区高度与宽度
							   IN int nHeight, 
							   IN BYTE* pSrcMask, 
							   IN int nMaskStartX,					// Mask的起点
							   IN int nMaskStartY, 
							   IN int nMaskWidth,					// Mask的高度与宽度
							   IN int nMaskHeight)
{
	// 数据范围检测
	if(nStartX>=nWidth || nStartY>=nHeight)
		return TRUE;
	if(nWidth<=0 || nHeight<=0 || nMaskWidth<=0 || nMaskHeight<=0)
		return TRUE;
	if(nMaskStartX>=nMaskWidth || nMaskStartY>=nMaskHeight)
		return TRUE;

	// 得到起点坐标和高度宽度
	int nDestWidth = nMaskWidth;
	int nDestHeight = nMaskHeight;
	if(nStartX < 0)
	{
		nDestWidth += nStartX;
		nMaskStartX -= nStartX;
		nStartX = 0;
	}
	if(nStartY < 0)
	{
		nDestHeight += nStartY;
		nMaskStartY -= nStartY;
		nStartY = 0;
	}
	if(nMaskStartX < 0)
	{
		nDestWidth += nMaskStartX;
		nStartX -= nMaskStartX;
		nMaskStartX = 0;
	}
	if(nMaskStartY < 0)
	{
		nDestHeight += nMaskStartY;
		nStartY -= nMaskStartY;
		nMaskStartY = 0;
	}
	int nTempWidth = nStartX+nDestWidth;
	int nTempHeight = nStartY+nDestHeight;
	if(nTempWidth > nWidth)
		nDestWidth -= (nTempWidth-nWidth);
	if(nTempHeight > nHeight)
		nDestHeight -= (nTempHeight-nHeight);
	nTempWidth = nMaskStartX+nDestWidth;
	nTempHeight = nMaskStartY+nDestHeight;
	if(nTempWidth > nMaskWidth)
		nDestWidth -= (nTempWidth-nMaskWidth);
	if(nTempHeight > nMaskHeight)
		nDestHeight -= (nTempHeight-nMaskHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	DWORD* pDest = pDesBuffer + nStartY*nWidth + nStartX;
	BYTE* pSrc = pSrcMask + nMaskStartY*nMaskWidth + nMaskStartX;
	if(pSrc == NULL || pDest == NULL)
		return TRUE;
	int nAddDestWidth = nWidth<<2;  // 每行的字节数
	int nAddSrcWidth = nMaskWidth;
	//	DWORD* pDest1 = pDest;

#if 0
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};
	// 一行行赋值,SSE2优化
	_asm
	{
		XOR EAX, EAX	// i清0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // 第一层循环(i<nMaskHeight)
			JGE	EXITLOOP1	 // 退出第一层循环(i>=nMaskHeight)
			XOR ECX, ECX	 // j清0， 第二层循环计数器
			PUSH EAX
LOOP2:
		ADD ECX, 2			// 每次计算2个值，不足2时跳到循环体2中
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2	

			MOV EAX, pSrc	// 一次读取pSrc[j]的两个BYTE值放入MM0

			MOV EDX, DWORD PTR [EAX+ECX]	// EDX包含两个BYTE值

			PUSH ECX
				MOV			ECX,EDX
				SHL			EDX,24
				OR			EDX,0x00ffffff		// EDX变成AFFF
				SHR			ECX,8
				SHL			ECX,24
				OR			ECX,0x00ffffff		// ECX变成AFFF
				MOVD		MM0,EDX
				PXOR		MM2, MM2
				PUNPCKLBW	MM0,MM2				// MM0变成0A0F0F0F

				MOVD		MM1,ECX
				PUNPCKLBW	MM1,MM2				// MM1变成0A0F0F0F
				MOVQ2DQ		XMM2,MM0
				MOVQ2DQ		XMM6,MM1
				PUNPCKLQDQ  XMM2,XMM6			// XMM2变成0A0F0F0F0A0F0F0F
				POP ECX

				MOV EAX, pDest	 // 读取pDest[j]的值
				MOV EDX, ECX
				SHL EDX, 2
				ADD EAX, EDX
				PUSH  EAX		 // 保留指针，后面要用
				MOVD MM0, [EAX]
				ADD EAX, 4		// 读取pDest[j+1]的值
					MOVD MM1, [EAX]
					XOR EAX, EAX	// 清0
						MOVD MM2, EAX
						PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
						PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
						MOVQ2DQ XMM1, MM0
						MOVQ2DQ XMM3, MM1
						PUNPCKLQDQ XMM1, XMM3 // 变为0R0G0B0A0R0G0B0A

						PMULLW XMM1, XMM2	// 对应16位相乘SA
						// 误差补偿
						MOVQ2DQ		XMM7,MM5
						MOVLHPS		XMM7,XMM7
						PADDSW		XMM1,XMM7
						PSRLW  XMM1, 8		// 每16位右移8位（相当于/255)(要进行误差补偿)

						PACKUSWB XMM1, XMM1 // 压缩，每16位压缩为8位(RGBARGBA)

						MOVDQ2Q	MM0,XMM1	// 取低64位(两个DWORD)
						POP EAX					// 赋给pDest
						MOVQ  [EAX], MM0

						ADD ECX, 2			// j增2，返回第二层循环
						JMP LOOP2
LOOP2BODY2:
					SUB ECX, 2			// 不足2时
						CMP ECX, nDestWidth
						JGE LOOP2END	

						MOV EAX, pSrc		// 读取pSrc[j]的值
						MOV EDX, DWORD PTR [EAX+ECX]

						PUSH ECX
							SHL			EDX,24
							OR			EDX,0x00ffffff		// EDX变成AFFF
							XOR ECX, ECX
							MOVD MM3, EDX
							MOVD MM4, ECX
							PUNPCKLBW MM3, MM4			// 变为0A0F0F0F

							POP ECX

							MOV EAX, pDest	 // 读取pDest[j]的值
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // 保留指针，后面要用
							MOVD MM1, [EAX]
							XOR EAX, EAX	// 清0
								MOVD MM2, EAX
								PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A

								PMULLW MM1, MM3		// 对应16位相乘SA
								// 误差补偿
								PADDSW MM1, MM5
								PSRLW  MM1, 8		// 右移8位（相当于/255)(要进行误差补偿)

								PACKUSWB MM1, MM1 // 压缩，每16位压缩为8位(RGBARGBA)

								POP EAX					// 赋给pDest
								MOVD  [EAX], MM1
LOOP2END:
							MOV EAX, pDest
								ADD EAX, nAddDestWidth		// nWidth需乘4
								MOV [pDest], EAX

								MOV EAX, pSrc
								ADD EAX, nAddSrcWidth
								MOV [pSrc], EAX

								POP EAX
								INC	EAX			 // 计数器自增，返回循环1 
								JMP LOOP1
EXITLOOP1:
							EMMS			// 清空
	}
#endif
	// 改进算法，2003－11－20
#if 1
	// 一行行赋值,SSE2优化
	_declspec(align(16)) DWORD dwMask1[]={0x00FFFFFF,0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF};
	_declspec(align(16)) DWORD dwMask2[]={0x000000FF,0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX	// i清0
			MOVDQA XMM4, dwMask1
			MOVDQA XMM5, dwMask2
			MOV ESI, pDest
			MOV EDI, pSrc
LOOP1:
		CMP ECX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1	 // 退出第一层循环(i>=nDestHeight)
			PUSH ECX
			XOR ECX, ECX	 // j清0， 第二层循环计数器
			MOV EAX, EDI	 // 源和目标缓冲区指针 
			MOV EDX, ESI
LOOP2:
		ADD ECX, 4			// 每次计算4个值，不足4时跳到循环体2中
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			// 取出Dest对应的Float值
			MOVDQU XMM0, [EDX]
			MOVAPS XMM6, XMM0
				PAND XMM6, XMM4		// 变成0RGB0RGB0RGB0RGB
				PSRLD XMM0, 24
				PAND XMM0, XMM5		// 变成000A000A000A000A
				CVTDQ2PS XMM7, XMM0
				PUSH EDX
				// 取出四个Mask对应的Float值
				XOR EDX, EDX
				MOV DL, [EAX]
				SHL EDX, 2
					ADD EDX, g_pfPosTable
					MOVSS XMM1, [EDX]
					INC EAX
						XOR EDX, EDX
						MOV DL, [EAX]
						SHL EDX, 2
							ADD EDX, g_pfPosTable
							MOVSS XMM2, [EDX]
							INC EAX
								PUNPCKLDQ XMM1, XMM2
								XOR EDX, EDX
								MOV DL, [EAX]
								SHL EDX, 2
									ADD EDX, g_pfPosTable
									MOVSS XMM2, [EDX]
									INC EAX
										XOR EDX, EDX
										MOV DL, [EAX]
										SHL EDX, 2
											ADD EDX, g_pfPosTable
											MOVSS XMM3, [EDX]
											PUNPCKLDQ XMM2, XMM3
												PUNPCKLQDQ XMM1, XMM2

												MULPS XMM7, XMM1
												INC EAX
												POP EDX
												CVTPS2DQ XMM0, XMM7		// 4个结果A值存入XMM0
												PSLLD XMM0, 24			// 左移24位，变成A000A000A000A000
												POR XMM0, XMM6			// 合并ARGB
												MOVDQU [EDX], XMM0

												ADD EDX, 10H
												JMP LOOP2
LOOP2BODY2:
											SUB ECX, 4			// 不足4时
LOOP2BODY3:
											CMP ECX, nDestWidth
												JGE LOOP2END	

												MOVD XMM0, [EDX]
												MOVAPS XMM6, XMM0
													PAND XMM6, XMM4		// 变成0RGB0RGB0RGB0RGB
													PSRLD XMM0, 24
													PAND XMM0, XMM5		// 变成000A000A000A000A
													CVTDQ2PS XMM7, XMM0
													PUSH EDX
													// 取出四个Mask对应的Float值
													XOR EDX, EDX
													MOV DL, [EAX]
													SHL EDX, 2
														ADD EDX, g_pfPosTable
														MOVSS XMM1, [EDX]

														MULSS XMM7, XMM1
															INC EAX
															POP EDX
															INC ECX
															CVTPS2DQ XMM0, XMM7		// 4个结果A值存入XMM0
															PSLLD XMM0, 24			// 左移24位，变成A000A000A000A000
															POR XMM0, XMM6			// 合并ARGB
															MOVD [EDX], XMM0

															ADD EDX, 4
															JMP LOOP2BODY3
LOOP2END:
														ADD ESI, nAddDestWidth
															ADD EDI, nAddSrcWidth

															POP ECX
															INC	ECX			 // 计数器自增，返回循环1 
															JMP LOOP1
EXITLOOP1:
														EMMS			// 清空
	}
#endif
#if 0
	for(int i=0; i<nMaskHeight; i++)
	{
		if(nStartY+i >= nHeight)
			break;
		if(nMaskStartY+i >= nMaskHeight)
			break;
		for(int j=0; j<nMaskWidth; j++)
		{
			int nTemp = nMaskStartX+j;
			if(nStartX+j >= nWidth)
				break;
			if(nTemp >= nMaskWidth)
				break;
			TPRGBA	rgbDest, rgbCur;
			rgbDest.m_dwColor = pDest[j];
			float fMix = pSrc[j]/(float)255;
			rgbCur.m_bColor.bR = (BYTE)(rgbDest.m_bColor.bR);
			rgbCur.m_bColor.bG = (BYTE)(rgbDest.m_bColor.bG);
			rgbCur.m_bColor.bB = (BYTE)(rgbDest.m_bColor.bB);
			rgbCur.m_bColor.bA = (BYTE)(rgbDest.m_bColor.bA*fMix);

			pDest[j] = rgbCur.m_dwColor;
		}
		pDest += nWidth;
		pSrc += nMaskWidth;
	}
#endif
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 功能：清理缓冲区某一区域
// 参数：pdwBuffer:目标缓冲区,最好是16字节对齐，这样效率能提高一倍
//		 int nPitch	步长,Byte
//		 nX:起点x,单位Pixel
//		 nY:起点y,单位Pixel
//		 nH:宽度,单位Pixel
//		 nW:长度,单位Pixel
//		 dwValue:值
// 返回值：缓冲区指针
//////////////////////////////////////////////////////////////////////////
void* CALLBACK TP_Clear(IN OUT DWORD *pdwBuffer,
						int nPitch,
						int nX,
						int nY,
						int nW,
						int nH,
						DWORD dwValue)
{
	if(pdwBuffer == NULL)
		return NULL;
	int		nBPitch = nPitch>>2;
	int		nBW = nW<<2;
	DWORD *pdwTemp = pdwBuffer+nBPitch*nY+nX;
	DWORD *pdwTempEnd = pdwBuffer+nH*nBPitch+nX+nW;
	if(((DWORD)pdwTemp)%16)
	{
		// 如果不是16字节对齐
		// 一次四个数赋值
		__asm{
			XOR ECX,ECX	// 清0
				MOV EDX, pdwTemp    // EDX存储缓冲区指针
				MOVD   XMM0, dwValue
				PSHUFD XMM0, XMM0, 0 // 得到dwValue值存入XMM0四份
LOOPX:
			ADD ECX, 4		// 先加4，判断dwSize是否被4整除
				CMP ECX, nW
				JG LOOPX1		// 如果>=,则跳到LOOPX1

				MOVDQU [EDX], XMM0	// 给四个点赋值

				// 指针后移16个字节
				ADD EDX, 10H
				JMP LOOPX
LOOPX1:
			// 当不够4个字节时:只能一次赋一个
			SUB	ECX, 4	// 减回到原来的数
LOOPX2:
			CMP ECX, nW
				JGE LOOPX3

				MOV EAX, dwValue
				INC ECX
				MOV [EDX], EAX
				ADD EDX,4             // pdwTemp偏移四个字节
				JMP LOOPX2
LOOPX3:		
			CMP EDX,pdwTempEnd
				JGE	EXITLOOPX

				SUB EDX, nBW			//地址回到行首
				ADD EDX, nPitch			//地址下移一行
				XOR ECX,ECX	//			清0
				JMP LOOPX
EXITLOOPX:
		}
	}
	else
	{
		// 一次四个数赋值
		__asm{
			XOR ECX,ECX	// 清0
				MOV EDX, pdwTemp    // EDX存储缓冲区指针
				MOVD   XMM0, dwValue
				PSHUFD XMM0, XMM0, 0 // 得到dwValue值存入XMM0四份
LOOPY:
			ADD ECX, 4		// 先加4，判断dwSize是否被4整除
				CMP ECX, nW
				JG LOOPY1		// 如果>=,则跳到LOOPX1

				MOVNTDQ [EDX], XMM0	// 给四个点赋值(WC模式，直接写，不经过缓存）

				// 指针后移16个字节
				ADD EDX, 10H
				JMP LOOPY
LOOPY1:
			// 当不够4个字节时:只能一次赋一个
			SUB	ECX, 4	// 减回到原来的数
LOOPY2:
			CMP ECX, nW
				JGE LOOPY3

				MOV EAX, dwValue
				INC ECX
				MOV [EDX], EAX
				ADD EDX,4             // pdwTemp偏移四个字节

				JMP LOOPY2
LOOPY3:
			CMP EDX,pdwTempEnd
				JGE	EXITLOOPY

				SUB EDX, nBW			//地址回到行首
				ADD EDX, nPitch			//地址下移一行
				XOR ECX,ECX	//			清0
				JMP LOOPX
EXITLOOPY:
		}
	}
	return pdwBuffer;
}
//////////////////////////////////////////////////////////////////////////
// 功能：设置象素点Alpha为指定值
// 参数：pdwBuffer:目标缓冲区
//		 dwValue:指定的Alpha值
//		 nWidth:宽
//		 nHeight：高
// 返回值：缓冲区指针
//////////////////////////////////////////////////////////////////////////
void* CALLBACK TP_AlphaMemSet(IN OUT DWORD *pdwBuffer, IN BYTE byValue, IN int nWidth, IN int nHeight)
{
	if(pdwBuffer == NULL)
		return NULL;
	DWORD *pdwTemp = pdwBuffer;
	DWORD dwValue = (DWORD)byValue;
	dwValue = dwValue<<24;

	// 一次四个数赋值
	__asm{
		XOR ECX,ECX	// 清0
			MOV EDX, pdwTemp    // EDX存储缓冲区指针
			MOV EAX, 0x00FFFFFF
			MOVD XMM7, EAX
			PSHUFD XMM7, XMM7, 0 // 存入XMM7四份
			MOV EAX, dwValue
			MOVD XMM0, EAX
			PSHUFD XMM0, XMM0, 0 // 得到byValue值存入XMM0四份
LOOP1:
		CMP ECX, nHeight // 第一层循环(i<nHeight)
			JGE	EXITLOOP1	 // 退出第一层循环(i>=nHeight)
			PUSH ECX
			XOR ECX, ECX	 // j清0， 第二层循环计数器
LOOP2:
		ADD ECX, 4			// 每次计算4个值，不足4时跳到循环体2中
			CMP ECX, nWidth
			JG LOOP2BODY2	

			// 取出Dest对应的Float值
			MOVDQU   XMM1, [EDX]
			PAND  XMM1, XMM7
				POR   XMM1, XMM0
				MOVDQU DWORD PTR [EDX], XMM1

				ADD EDX, 10H
				JMP LOOP2
LOOP2BODY2:
			SUB ECX, 4			// 不足4时
LOOP2BODY3:
			CMP ECX, nWidth
				JGE LOOP2END	

				PUSH ECX
				MOV ECX, DWORD PTR [EDX]
				AND ECX, 0x00FFFFFF
					OR ECX, EAX
					MOV DWORD PTR [EDX], ECX
					POP ECX

					ADD EDX, 4
					INC ECX
					JMP LOOP2BODY3
LOOP2END:
				POP ECX
					INC	ECX			 // 计数器自增，返回循环1 
					JMP LOOP1
EXITLOOP1:
				EMMS			// 清空
	}
	return pdwBuffer;
}
//////////////////////////////////////////////////////////////////////////
// 功能：32位转24位Buffer,除去Alpha键
// 参数：nWidth:宽度（象素数），nHeight：高度（象素数）
// 返回值：
//////////////////////////////////////////////////////////////////////////
void CALLBACK TP_32To24Buffer(void* pDesBuffer, long des, void* pSrcBuffer, long src, int nWidth, int nHeight)
{
	//	long nSum = nWidth*nHeight;	// 总象素个数
	//	__asm
	//	{
	//		XOR ECX, ECX
	//		PXOR XMM7, XMM7					// 0
	//		MOVD XMM7, 0x00FFFFFF			
	//		MOVDQA XMM6, XMM7				// 1
	//		PSLLDQ XMM6, 4
	//		MOVDQA XMM5, XMM7				// 2
	//		PSLLDQ XMM5, 8
	//		MOVDQA XMM4, XMM7				// 3
	//		PSLLDQ XMM4, 12
	//		MOV EDX, pSrcBuffer
	//		MOV EAX, pDesBuffer
	//LOOPX16:	
	//		ADD ECX, 16		// 一次16个点
	//		CMP ECX, nSum
	//		JG LOOPX4
	//
	//		MOVDQU XMM1, [EDX]	// A3R3G3B3A2R2G2B2A1R1G1B1A0R0G0B0
	//		ADD EDX, 16		
	//
	//		MOVDQA XMM0, XMM7
	//		PAND XMM0,   XMM1	// R0G0B0
	//		MOVDQA XMM2, XMM6
	//		PAND XMM2,   XMM1	// R1G1B1
	//		PSRLDQ XMM2,  1		// 右移1字节
	//		POR   XMM0, XMM2	// R1G1B1R0G0B0
	//		MOVDQA XMM2, XMM5
	//		PAND XMM2,   XMM1	// R2G2B2
	//		PSRLDQ XMM2, 2		// 右移2个字节
	//		PAND XMM0, XMM2		// R2G2B2R1G1B1R0G0B0
	//		MOVDQA XMM2, XMM4
	//		PAND XMM2,   XMM1	// R3G3B3
	//		PSRLDQ XMM2, 3		// 右移3个字节
	//		PAND XMM0, XMM2		// R3G3B3R2G2B2R1G1B1R0G0B0
	//
	//		MOVDQU XMM1, [EDX]
	//		ADD EDX, 16
	//		MOVDQA XMM2, XMM7
	//		PAND XMM2, XMM6
	//
	//
	//		MOVDQU XMM1, [EDX]
	//		ADD EDX, 16
	//
	//		MOVDQU XMM1, [EDX]
	//		ADD EDX, 16
	//		JMP LOOPX16
	//LOOPX4:
	//
	//	}

	//__asm
	//{
	//	mov	ecx,[esp+20+16]
	//	lea	eax,[ecx+ecx*2]
	//	lea	ebx,[ecx*4]
	//	sub	[esp+8+16],eax
	//	sub	[esp+16+16],ebx

	//	pcmpeqb	mm7,mm7
	//	psrld	mm7,8
	//	movq	mm6,mm7
	//	psllq	mm7,32			;mm7 = high rgb mask
	//	psrlq	mm6,32			;mm6 = low rgb mask

	//	mov	ebp,[esp+20+16]
	//	mov	edx,[esp+24+16]
	//	mov	eax,[esp+16+16]
	//	mov	ebx,[esp+ 8+16]
	//	DIBconvert3224ISSE@y:
	//	mov	ecx,ebp
	//		shr	ecx,3
	//		jz	DIBconvert3224ISSE@x2
	//		DIBconvert3224ISSE@x:	
	//	movq	mm0,[esi]		;mm0 = a1r1g1b1a0r0g0b0
	//		movq	mm1,mm6

	//		movq	mm2,[esi+8]		;mm2 = a3r3g3b3a2r2g2b2
	//		pand	mm1,mm0			;mm1 = ----------r0g0b0

	//		movq	mm3,mm6
	//		pand	mm0,mm7			;mm0 = --r1g1b1--------

	//		movq	mm4,mm2
	//		pand	mm3,mm2			;mm3 = ----------r2g2b2

	//		psrlq	mm0,8			;mm0 = ----r1g1b1------
	//		pand	mm2,mm7			;mm2 = --r3g3b3--------

	//		movq	mm5,[esi+16]		;mm5 = a5r5g5b5a4r4g4b4
	//		psllq	mm4,48			;mm4 = g2b2------------

	//		por	mm0,mm1			;mm0 = ----r1g1b1r0g0b0
	//		psrlq	mm3,16			;mm3 = --------------r2

	//		por	mm0,mm4			;mm0 = g2b2r1g1b1r0g0b0
	//		movq	mm1,mm6

	//		pand	mm1,mm5			;mm1 = ----------r4g4b4
	//		psrlq	mm2,24			;mm2 = --------r3g3b3--

	//		movntq	[edi],mm0
	//		pand	mm5,mm7			;mm5 = --r5g5b5--------

	//		psllq	mm1,32			;mm1 = --r4g4b4--------
	//		movq	mm4,mm5			;mm4 = --r5g5b5--------

	//		por	mm2,mm3			;mm2 = --------r3g3b3r2
	//		psllq	mm5,24			;mm5 = b5--------------

	//		movq	mm3,[esi+24]		;mm3 = a7r7g7b7a6r6g6b6
	//		por	mm2,mm1			;mm2 = --r4g4b4r3g3b3r2

	//		movq	mm1,mm6
	//		por	mm2,mm5			;mm2 = b5r4g4b4r3g3b3r2

	//		psrlq	mm4,40			;mm4 = ------------r5g5
	//		pand	mm1,mm3			;mm1 = ----------r6g6b6

	//		psllq	mm1,16			;mm1 = ------r6g6b6----	
	//		pand	mm3,mm7			;mm3 = --r7g7b7--------

	//		por	mm4,mm1			;mm4 = ------r6g6b6r5g5
	//		psllq	mm3,8			;mm3 = r7g7b7----------

	//		movntq	[edi+8],mm2
	//		por	mm4,mm3			;mm4 = r7g7b7r6g6b6r5g5

	//		add	esi,32
	//		sub	ecx,1

	//		movntq	[edi+16],mm4		;mm3

	//		lea	edi,[edi+24]
	//		jne	DIBconvert3224ISSE@x

	//			DIBconvert3224ISSE@x2:
	//		mov	ecx,ebp
	//			and	ecx,7
	//			jz	DIBconvert3224ISSE@x3
	//			movd	mm0,eax
	//			DIBconvert3224ISSE@x4:
	//		mov	eax,[esi]
	//		add	esi,4
	//			mov	[edi],ax
	//			shr	eax,16
	//			mov	[edi+2],al
	//			add	edi,3
	//			sub	ecx,1
	//			jnz	DIBconvert3224ISSE@x4
	//			movd	eax,mm0
	//			DIBconvert3224ISSE@x3:
	//		add	esi,eax
	//			add	edi,ebx

	//			sub	edx,1
	//			jne	DIBconvert3224ISSE@y

	//			emms
	//			sfence
	//}
}
//////////////////////////////////////////////////////////////////////////
// 还未实现
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK TP_ColorMixLine(
							  IN		DWORD* pDes,		//目标
							  IN		DWORD* pSrc,		//源
							  IN		int nWidth			//宽度
							  )
{
	return 1;
}

//////////////////////////////////////////////////////////////////////////
// 两掩模混叠操作
// 算法：DA = SA + DA - (SA*DA)/255,其中SA为源掩模，DA为目标掩模
//
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_MixMask(IN OUT BYTE *pbyDestMask,	// 目标掩模缓冲区
							IN int nDestX,				// 目标操作起点坐标
							IN int nDestY, 
							IN int nDestWidth,			// 目标操作宽、高
							IN int nDestHeight,
							IN int nDestMaskWidth,		// 目标掩模缓冲区宽、高
							IN int nDestMaskHeight,
							IN BYTE *pbySrcMask,		// 源掩模缓冲区
							IN int nSrcX,				// 源操作起点坐标
							IN int nSrcY,
							IN int nSrcWidth,			// 源操作宽、高
							IN int nSrcHeight,
							IN int nSrcMaskWidth,		// 源掩模缓冲区宽、高
							IN int nSrcMaskHeight)
{
	// 数据范围检测
	if(nDestX >= nDestMaskWidth || nDestY >= nDestMaskHeight)
		return TRUE;
	if(nDestMaskWidth <= 0||nDestMaskHeight <= 0 || nSrcMaskHeight<=0||nSrcMaskWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcMaskWidth || nSrcY >= nSrcMaskHeight)
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
	int nTempWidth = nSrcX+nDestWidth;
	int nTempHeight = nSrcY+nDestHeight;
	if(nTempWidth > nSrcMaskWidth)
		nDestWidth -= (nTempWidth-nSrcMaskWidth);
	if(nTempHeight > nSrcMaskHeight)
		nDestHeight -= (nTempHeight-nSrcMaskHeight);
	nTempWidth = nDestX+nDestWidth;
	nTempHeight = nDestY+nDestHeight;
	if(nTempWidth > nDestMaskWidth)
		nDestWidth -= (nTempWidth-nDestMaskWidth);
	if(nTempHeight > nDestMaskHeight)
		nDestHeight -= (nTempHeight-nDestMaskHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	nSrcWidth = nDestWidth;
	nSrcHeight = nDestHeight;

	// 得到目标缓冲区起始点和源缓冲区起始点
	BYTE* pDes = pbyDestMask + nDestY*nDestMaskWidth + nDestX;
	BYTE* pSrc = pbySrcMask + nSrcY*nSrcMaskWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;

#if 1
	// 一行行赋值,SSE2优化
	_declspec(align(16)) DWORD dwMask1[]={0x00FF00FF,0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	_declspec(align(16)) DWORD dwMask2[]={0x000000FF,0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX	// i清0
			MOVDQA XMM4, dwMask1
			MOVDQA XMM5, dwMask2
LOOP1:
		CMP ECX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1	 // 退出第一层循环(i>=nDestHeight)
			PUSH ECX
			XOR ECX, ECX	 // j清0， 第二层循环计数器
			MOV EAX, pSrc	 // 源和目标缓冲区指针 
			MOV EDX, pDes
LOOP2:
		ADD ECX, 8			// 每次计算8个值，不足8时跳到循环体2中
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			MOVQ MM0, [EDX]	// 取Dest的8个Byte值，64位
			MOVQ2DQ XMM0, MM0
				PXOR XMM7, XMM7
				PUNPCKLBW XMM0, XMM7	// 每个Byte变成Word值
				MOVDQA XMM3, XMM0
				MOVQ MM1, [EAX]		// 取Src的8个Byte值
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7 // 每个Byte变成Word值
					PMULLW XMM0, XMM1	// SA*DA
					PADDUSW XMM0,XMM4	// SA*DA+0xFF：误差补偿
					PSRLW XMM0, 8		// 右移8位
					PADDUSW XMM3, XMM1	// SA+DA
					PSUBUSW XMM3, XMM0	// SA+DA-(SA*DA+0xFF)>>8
					PACKUSWB XMM3, XMM3	// 解包：转WORD成BYTE值
					MOVDQ2Q MM3, XMM3	// 将结果赋给Dest
					MOVNTQ [EDX], MM3

					ADD EAX, 8			// 指针后移
					ADD EDX, 8
					JMP LOOP2
LOOP2BODY2:
				SUB ECX, 8			// 不足8时
LOOP2BODY3:
				ADD ECX, 4			// 每次计算4个值，不足4时跳到循环体4中
					CMP ECX, nDestWidth
					JG LOOP2BODY4	

					MOVD XMM0, [EDX]	// 取Dest的4个Byte值，32位
					PXOR XMM7, XMM7
						PUNPCKLBW XMM0, XMM7// 每个Byte变成Word
						PUNPCKLWD XMM0, XMM7// 每个Byte变成DWORD
						MOVDQA XMM3, XMM0
						MOVD XMM1, [EAX]	// 取Src的4geByte值，32位
						PUNPCKLBW XMM1, XMM7
							PUNPCKLWD XMM1, XMM7// 每个Byte变成DWORD
							PMULLW XMM0, XMM1	// SA*DA
							PADDD XMM0, XMM5	// SA*DA+0xFF
							PSRLD XMM0, 8		// SA*DA+0xFF右移8位：误差补偿
							PADDD XMM3, XMM1	// SA+DA
							PSUBD XMM3, XMM0	// SA+DA-(SA*DA+0xFF)>>8
							PACKSSDW XMM3, XMM3 // 解包：转DWORD成Word
							PACKUSWB XMM3, XMM3 // 解包：转念Word成Byte
							MOVD  [EDX], XMM3	// 将结果赋给Dest

							ADD EAX, 4
							ADD EDX, 4
							JMP LOOP2BODY3
LOOP2BODY4:
						SUB ECX, 4			// 不足4时
LOOP2BODY5:
						ADD ECX, 2			// 每次计算2个值， 不足2时跳到循环体6中
							CMP ECX, nDestWidth
							JG LOOP2BODY6

							PUSH EAX
							XOR EAX, EAX
							MOV AX, [EDX]
							MOVD XMM0, EAX
								POP EAX

								PUSH EDX
								XOR EDX, EDX
								MOV DX, [EAX]
								MOVD XMM1, EDX
									POP EDX

									PXOR XMM7, XMM7
									PUNPCKLBW XMM0, XMM7// 每个Byte变成Word
									PUNPCKLWD XMM0, XMM7// 每个Byte变成DWORD
									MOVDQA XMM3, XMM0
									PUNPCKLBW XMM1, XMM7// 每个Byte变成Word
									PUNPCKLWD XMM1, XMM7// 每个Byte变成DWORD
									PMULLW XMM0, XMM1	// SA*DA
									PADDD XMM0, XMM5	// SA*DA+0xFF
									PSRLD XMM0, 8		// SA*DA+0xFF右移8位：误差补偿
									PADDD XMM3, XMM1	// SA+DA
									PSUBD XMM3, XMM0	// SA+DA-(SA*DA+0xFF)>>8
									PACKSSDW XMM3, XMM3 // 解包：转DWORD成Word
									PACKUSWB XMM3, XMM3 // 解包：转念Word成Byte

									PUSH EAX
									MOVD EAX, XMM3
									MOV  [EDX], AX		// 将结果赋给Dest
									POP EAX

									ADD EAX, 2
									ADD EDX, 2
									JMP LOOP2BODY5
LOOP2BODY6:
								SUB ECX, 2
									CMP ECX, nDestWidth
									JGE LOOP2END

									PUSH EDX
									XOR ECX, ECX
									MOV CL, [EAX]	// SA放在ECX
									XOR EAX, EAX
										MOV AL, [EDX]	// DA放在EAX
										MOV EDX, ECX	// 保存SA
											ADD ECX, EAX	// SA+DA
											MUL EDX			// SA*DA
											ADD EAX, 0x000000FF	// SA*DA+0xFF
											SHR EAX, 8		// 右移8位
											SUB ECX, EAX	// 两相减
											POP EDX

											MOV BYTE PTR [EDX], CL
LOOP2END:
										MOV EAX, nDestMaskWidth
											ADD [pDes], EAX

											MOV EAX, nSrcMaskWidth
											ADD [pSrc], EAX

											POP ECX
											INC	ECX			 // 计数器自增，返回循环1 
											JMP LOOP1
EXITLOOP1:
										EMMS			// 清空
	}
#endif
#if 0
	for(int i=0; i<nDestHeight; i++)
	{
		for(int j=0; j<nDestWidth; j++)
		{
			//	float fMix = /(float)255;
			pDes[j] = pDes[j]+pSrc[j]-(BYTE)((pDes[j]*pSrc[j]+0xFF)>>8);
		}
		pDes += nDestMaskWidth;
		pSrc += nSrcMaskWidth;
	}
#endif
	return E_FAIL;
}

// 获取计数器值(高精度):时钟周期
// 获取当前的主机时钟周期（纳秒级）/主频HZ数
unsigned _int64 TP_GetCycleCount()
{
	__asm _emit 0x0F		// RDTSC指令
		__asm _emit 0x31
}


//////////////////////////////////////////////////////////////////////////
// 私有函数
//////////////////////////////////////////////////////////////////////////
// 建拉伸拷贝表
BOOL TPBuffer_CreateTable(DWORD*& pnTable, int nWidth, float fAddWidth)
{
	pnTable = (DWORD*)_mm_malloc(sizeof(DWORD)*nWidth, 16);
	DWORD* pDest = pnTable;
	if(pnTable == NULL)
		return FALSE;

	float fVal = -fAddWidth;
	_declspec(align(16)) float fTemp[4];
	float f = fAddWidth;
	for(int i=0; i<4; i++)
	{
		fTemp[i] = f;
		f += fAddWidth;
	}
	//	for(int i=0; i<nWidth; i++)
	//	{
	//		pnTable[i] = (DWORD)fVal;
	//		fVal += fAddWidth;
	//	}
	__asm
	{
		XOR ECX, ECX
			MOV EDX, pDest
			MOVAPS XMM0, [fTemp]
			MOVSS  XMM1, fVal			// 初始值
				SHUFPS XMM1, XMM1, 0
LOOPX:
			ADD ECX, 4
				CMP ECX, nWidth
				JG  LOOPX2

				ADDPS  XMM1, XMM0         // 对应4个数相加
				CVTTPS2DQ XMM2, XMM1
				SHUFPS XMM1, XMM1, 0xFF
				MOVUPS [EDX], XMM2

				ADD EDX, 10H
				JMP LOOPX
LOOPX2:
			SUB ECX, 4
LOOPX3:
			CMP ECX, nWidth
				JGE LOOPXEND

				ADDSS XMM1, XMM0
				INC ECX
				CVTTPS2DQ XMM2, XMM1
				MOVSS [EDX], XMM2

				ADD EDX, 4
				JMP LOOPX3
LOOPXEND:
			EMMS
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 释放表空间
BOOL TPBuffer_FreeTable(void * pnTable)
{
	if(pnTable != NULL)
	{
		_mm_free(pnTable);
		pnTable = NULL;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 功能：进行Mix填充缓冲区，这时nSrcWidth==nDestWidth, nSrcHeight==nDestHeight
//
// 1、2003-10-10,V1.0,编写者：陈玉峰
//	  使用算法：DR = SR*SA/255+(1-SA/255)*DR
// 2、2003-11-19,V2.0,修改者：陈玉峰
//	  改进算法：KA=SA+DA-SA*DA/255; DR = DR+(SR-DR)*SA/KA
// 3、2004－02－11，V2.1，修改者：陈玉峰
//    改进：添加奇偶场信息的处理
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_Mix(DWORD* pdwDestBuffer, 
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
				  int nParity)					
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
	int nTempWidth = nSrcX+nDestWidth;
	int nTempHeight = nSrcY+nDestHeight;
	if(nTempWidth > nSrcBufferWidth)
		nDestWidth -= (nTempWidth-nSrcBufferWidth);
	if(nTempHeight > nSrcBufferHeight)
		nDestHeight -= (nTempHeight-nSrcBufferHeight);
	nTempWidth = nDestX+nDestWidth;
	nTempHeight = nDestY+nDestHeight;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	nSrcWidth = nDestWidth;
	nSrcHeight = nDestHeight;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;

	//////////////////////////////////////////////////////////////////////////
	// 增加了奇偶场信息：04－02－11，chyf
	// 每次加的宽度
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// 计数器的增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;
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
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pSrc += nSrcBufferWidth;
		}
	}
	else
	{
		// 起点为偶场
		if(nParity == TP_PARITY_ODD)
		{
			// 如果奇场拷贝，则从下一行开始
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pSrc += nSrcBufferWidth;
		}
	}
	// 增加了奇偶场信息
	//////////////////////////////////////////////////////////////////////////

#if 0	
	// 误差补偿
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};


	// 一行行赋值,SSE2优化
	_asm
	{
		XOR EAX, EAX	// 清0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1	 // 退出第一层循环(i>=nDestHeight)
			MOV	ECX, nDestY	 // 计算nDestY+i
			ADD ECX, EAX
			CMP ECX, nDestBufferHeight
			JGE	EXITLOOP1	 // 退出第一层循环(nDestY+i>=nDestBufferHeight)
			MOV	ECX, nSrcY	 // 计算nSrcY+i
			ADD ECX, EAX
			CMP ECX, nSrcBufferHeight
			JGE	EXITLOOP1	 // 退出第一层循环(nSrcY+i>=nSrcBufferHeight)
			XOR ECX, ECX	 // 清0， 第二层循环计数器
			PUSH EAX
LOOP2:
		CMP ECX, nDestWidth  // 第二层循环(j<nDestWidth)
			JGE LOOP2END	 // 进入第二层循环体(j<nDestWidth)

			ADD ECX, 2			// 每次计算2个值，不足2时跳到循环体2中
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2

			MOV EDX, nSrcX	// 计算nSrcX+j
			ADD EDX, ECX
			CMP EDX, nSrcBufferWidth
			JGE LOOP2END	// 退出第二层循环(nSrcX+j>=nSrcBufferWidth)

			MOV EDX, nDestX	// 计算nDestX+j
			ADD EDX, ECX
			CMP EDX, nDestBufferWidth
			JGE LOOP2END	// 退出第二层循环(nDestX+j>=nDestBufferWidth)	

			MOV EAX, pSrc	// 读取pSrc[j]的值放入MM0
			MOV EDX, ECX
			SHL EDX, 2
			ADD EAX, EDX 
			MOVD MM0, [EAX]

			PUSH ECX
				MOV ECX, [EAX]					// 读取其Alpha值
				SHR	ECX,  24					// 右移24位
					AND ECX, 0x000000FF
					MOVD MM3, ECX
					PUNPCKLWD MM3, MM3
					PUNPCKLDQ MM3, MM3				// 变为0A0A0A0A

					MOV EDX, 0xFF					// 255-Alpha
					SUB EDX, ECX
					MOVD MM7, EDX
					PUNPCKLWD MM7, MM7
					PUNPCKLDQ MM7, MM7				// 变为0A0A0A0A
					POP ECX

					ADD EAX, 4		// 读取pSrc[j+1]的值放入MM1
					MOVD MM1, [EAX]

					PUSH ECX
						MOV ECX, [EAX]					// 读取其Alpha值
						SHR	ECX,  24					// 右移24位
							AND ECX, 0x000000FF
							MOVD MM4, ECX
							PUNPCKLWD MM4, MM4
							PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

							MOV EAX, 0xFF					// 255-Alpha
							SUB EAX, ECX
							MOVD MM6, EAX
							PUNPCKLWD MM6, MM6
							PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A
							POP ECX

							XOR EAX, EAX	// 清0
							MOVD MM2, EAX
							PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
							PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
							MOVQ2DQ XMM0, MM0
							MOVQ2DQ XMM1, MM1
							PUNPCKLQDQ XMM0, XMM1 // 变为0R0G0B0A0R0G0B0A

							MOV EAX, pDes	 // 读取pDes[j]的值
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // 保留指针，后面要用
							MOVD MM0, [EAX]
							ADD EAX, 4		// 读取pDes[j+1]的值
								MOVD MM1, [EAX]
								XOR EAX, EAX	// 清0
									MOVD MM2, EAX
									PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
									PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
									MOVQ2DQ XMM1, MM0
									MOVQ2DQ XMM2, MM1
									PUNPCKLQDQ XMM1, XMM2 // 变为0R0G0B0A0R0G0B0A
									MOVDQA	XMM3, XMM1	 // 复制一份

									MOVQ2DQ XMM2, MM3		// 取源的alpha值
									MOVQ2DQ XMM4, MM4
									PUNPCKLQDQ XMM2, XMM4	// 变为0A0A0A0A0M0M0M0M

									MOVQ2DQ XMM4, MM7		// 取255-alpha值
									MOVQ2DQ XMM5, MM6
									PUNPCKLQDQ XMM4, XMM5	// 变为0A0A0A0A0M0M0M0M

									PMULLW XMM0, XMM2	// 对应16位相乘SA
									// 误差补偿
									MOVQ2DQ		XMM7,MM5
									MOVLHPS		XMM7,XMM7
									PADDSW		XMM0,XMM7
									PSRLW  XMM0, 8		// 每16位右移8位（相当于/255)(要进行误差补偿)

									PMULLW XMM1, XMM4	// 对应16位相乘D(1-A)
									// 误差补偿
									PADDSW XMM1,XMM7
									PSRLW  XMM1, 8

									PADDSW XMM0, XMM1	// 对应16位相加(SA+DA)

									PACKUSWB XMM0, XMM0 // 压缩，每16位压缩为8位(RGBARGBA)

									MOVDQ2Q	MM0,XMM0	// 取低64位(两个DWORD)
									POP EAX					// 赋给pDest
									MOVQ  [EAX], MM0

									ADD ECX, 2			// j增2，返回第二层循环
									JMP LOOP2
LOOP2BODY2:
								SUB ECX, 2			// 不足2时
									CMP ECX, nDestWidth
									JGE LOOP2END

									MOV EDX, nSrcX	// 计算nSrcX+j
									ADD EDX, ECX
									CMP EDX, nSrcBufferWidth
									JGE LOOP2END	// 退出第二层循环(nSrcX+j>=nSrcBufferWidth)

									MOV EDX, nDestX	// 计算nDestX+j
									ADD EDX, ECX
									CMP EDX, nDestBufferWidth
									JGE LOOP2END	// 退出第二层循环(nDestX+j>=nDestBufferWidth)		

									MOV EAX, pSrc		// 读取pSrc[j]的值
									MOV EDX, ECX
									SHL EDX, 2
									ADD EAX, EDX 
									MOVD MM0, [EAX]

									PUSH ECX
										MOV ECX, [EAX]					// 读取其Alpha值
										SHR	ECX,  24					// 右移24位
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A
											POP ECX

											XOR EAX, EAX	// 清0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A

											MOV EAX, pDes	 // 读取pDes[j]的值
											MOV EDX, ECX
											SHL EDX, 2
											ADD EAX, EDX
											PUSH  EAX		 // 保留指针，后面要用
											MOVD MM1, [EAX]
											XOR EAX, EAX	// 清0
												MOVD MM2, EAX
												PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
												MOVQ	MM3, MM1	 // 复制一份

												PMULLW MM0, MM4	// 对应16位相乘SA
												// 误差补偿
												PADDSW MM0, MM5
												PSRLW  MM0, 8		// 右移8位（相当于/255)(要进行误差补偿)

												PMULLW MM1, MM6	// 对应16位相乘D(1-A)
												// 误差补偿
												PADDSW MM1, MM5
												PSRLW  MM1, 8
												PADDSW MM0, MM1	// 对应16位相加
												PACKUSWB MM0, MM0 // 压缩，每16位压缩为8位(RGBARGBA)

												POP EAX					// 赋给pDest
												MOVD  [EAX], MM0
LOOP2END:
											MOV EAX, pDes
												ADD EAX, nAddDestWidth		// nDestBufferWidth需乘4
												MOV [pDes], EAX

												MOV EAX, pSrc
												ADD EAX, nAddSrcWidth
												MOV [pSrc], EAX

												POP EAX
												INC	EAX			 // 计数器自增，返回循环1 
												JMP LOOP1
EXITLOOP1:
											EMMS			// 清空
	}
#endif
	//////////////////////////////////////////////////////////////////////////
	// 改进算法，2003－11－12，chyf
	//////////////////////////////////////////////////////////////////////////
#if 0
	_declspec(align(16)) DWORD nMask[4] = {0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF};
	_declspec(align(16)) DWORD nMask2[4] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	// 建表
	_declspec(align(16)) DWORD *pTable = (DWORD*)_mm_malloc(256*sizeof(DWORD), 16);
	_declspec(align(16)) float fTemp[4] = {256.0f, 256.0f, 256.0f, 256.0f};
	if(pTable == NULL)
		return FALSE;
	pTable[0] = 0;
	for(int i=1; i<256; i++)
	{
		pTable[i] = (DWORD)(65535/i);
	}

	// 一行行赋值,SSE2优化

	_asm
	{
		XOR ECX, ECX	// i清0
LOOPI:
		CMP ECX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOPI	 // 退出第一层循环(i>=nDestHeight)

			PUSH ECX		// 压入i
			MOV  EAX, pSrc
			MOV  EDX, pDes
			XOR  ECX, ECX	 // j清0， 第二层循环计数器
LOOPJ:
		// 第二层循环体，一次读4个节点
		ADD ECX, 4			// 每次计算4个值，不足4时跳到循环体4中
			CMP ECX, nDestWidth
			JG LOOPJ4		// (j>nDestWidth)

			// 取出Src和Dest4个点存入XMM0和XMM1
			MOVUPS XMM0, [EAX]	// 取出SRC四个点的ARGB
			MOVUPS XMM1, [EDX]	// 取出Dest四个点的ARGB

			MOVAPS XMM6, XMM0	// 存储Src后面
				MOVAPS XMM7, XMM1	// 存储Dest后面用

				// 取出A值，变成000A000A000A000A
				PSRAD  XMM0, 24		// 右移24位，取A值
				PSRAD  XMM1, 24		// 右移24位，取A值
				PAND   XMM0, nMask		// 四个000A
				PAND   XMM1, nMask		// 四个000A

				// 保存SA高和低两个DWORD
				MOVDQ2Q MM4, XMM0       // 保存SA低两个000A000A	
				MOVAPS XMM3, XMM0		// 保存SA
				MOVHLPS XMM3, XMM3
				MOVDQ2Q MM5, XMM3       // 保存SA高两个000A000A

				// Src和Dest变成为000A000A000A000A相加,放入XMM3
				MOVAPS XMM3, XMM0		// 保存SA
				PADDW  XMM3, XMM1		// Src+Dest(16位加)

				// 对应16位相乘，并除256（Src*Dest>>8)
				PMULLW XMM0, XMM1
				PADDW  XMM0, nMask		// 误差补偿
				PSRAD  XMM0, 8			
				//PAND   XMM0, nMask	

				// 对应16位减,得到最终的CA值000A000A000A000A
				PSUBW  XMM3, XMM0	
				PAND   XMM3, nMask

				// 保存指针
				PUSH EAX
				PUSH EDX

				// 每次算两个点（SR*SA*CR>>8＋DR-DR*SA*CR>>8）
				// 查CA值对应的索引表（8个，成0A0A0A0A0A0A0A0A)
				XOR    EDX, EDX
				PEXTRW  EDX, XMM3, 0	// 抽出第一个A值
				MOV    EAX, pTable
				SHL    EDX, 2
				ADD	   EDX, EAX    
				MOVD   MM0, [EDX]
				PUNPCKLWD MM0, MM0  
					PUNPCKLDQ MM0, MM0  // 变成0A0A0A0A
					XOR    EDX, EDX
					PEXTRW  EDX, XMM3, 2	// 抽出第二个A值
					SHL    EDX, 2
					ADD	   EDX, EAX    
					MOVD   MM1, [EDX]
					PUNPCKLWD MM1, MM1 
						PUNPCKLDQ MM1, MM1  // 变成0A0A0A0A
						MOVQ2DQ  XMM0, MM0
						MOVQ2DQ  XMM1, MM1
						PUNPCKLQDQ XMM0, XMM1 // 变成0A0A0A0A0A0A0A0A

						// 取出两个点的RGBA值
						MOVDQ2Q  MM0, XMM6	  // SRC两点
						MOVDQ2Q  MM1, XMM7    // Dest两点
						MOVQ2DQ     XMM2, MM0
						PXOR     XMM1, XMM1
						PUNPCKLBW XMM2, XMM1	// Src变成0A0R0G0B0A0R0G0B,放在XMM2
						MOVQ2DQ     XMM4, MM1
						PUNPCKLBW XMM4, XMM1	// Dest变成0A0R0G0B0A0R0G0B，放在XMM4

						// SRC*SA
						// 取低两个SA，拆包
						MOVQ MM3, MM4
						PUNPCKHWD MM3, MM3
						PUNPCKLDQ MM3, MM3
						PUNPCKLWD MM4, MM4
						PUNPCKLDQ MM4, MM4
						MOVQ2DQ XMM1, MM4
						MOVQ2DQ XMM5, MM3
						PUNPCKLQDQ XMM1, XMM5		// 取出SA值变成0A0A0A0A0A0A0A0A
						PMULLW   XMM2, XMM1
						//PAND     XMM2, nMask2

						// SR*SA*CR
						PMULLW   XMM2, XMM0		// 对应16位乘
						//PAND     XMM2, nMask2

						// SR*SA*CR>>8
						PSRAW  XMM2, 8	
						//	PAND   XMM2, nMask2		

						// SR*SA*CR>>8+DR
						PADDW  XMM2, XMM4
						//PAND     XMM2, nMask2

						// Dest*SA
						PMULLW   XMM4, XMM1
						//PAND     XMM4, nMask2

						// DR*SA*CR
						PMULLW   XMM4, XMM0		// 对应16位乘
						//PAND     XMM4, nMask2

						// DR*SA*CR>>8
						PSRAW  XMM4, 8	
						//	PAND   XMM4, nMask2		

						// DR+SRC*SA*CR>>8-DEST*SA*CR>>8
						PSUBW  XMM2, XMM4      // 对应16位减
						PAND   XMM2, nMask2

						// 插入当前正确的A值
						XOR    EDX, EDX
						PEXTRW  EDX, XMM3, 0	// 抽出第一个A值 
						PINSRW XMM2, EDX, 3
						PEXTRW  EDX, XMM3, 2	// 抽出第二个A值 
						PINSRW XMM2, EDX, 7 // 算出的两个0R0G0B0A值

						// 计算另外两个点（SR*SA*CR>>8＋DR－DR*SA*CR>>8）
						// 查CA值对应的索引表（8个，成0A0A0A0A0A0A0A0A)
						XOR    EDX, EDX
						PEXTRW  EDX, XMM3, 4	// 抽出第三个A值
						SHL    EDX, 2
						MOV    EAX, pTable
						ADD	   EDX, EAX    
						MOVD   MM0, [EDX]
						PUNPCKLWD MM0, MM0  
							PUNPCKLDQ MM0, MM0  // 变成0A0A0A0A
							XOR    EDX, EDX
							PEXTRW  EDX, XMM3, 6	// 抽出第四个A值
							SHL    EDX, 2
							ADD	   EDX, EAX    
							MOVD   MM1, [EDX]
							PUNPCKLWD MM1, MM1 
								PUNPCKLDQ MM1, MM1  // 变成0A0A0A0A
								MOVQ2DQ  XMM0, MM0
								MOVQ2DQ  XMM1, MM1
								PUNPCKLQDQ XMM0, XMM1 // 变成0A0A0A0A0A0A0A0A

								// 取出后两个点的RGBA值
								MOVHLPS  XMM6, XMM6
								MOVDQ2Q  MM0, XMM6	  // SRC高两点
								MOVHLPS  XMM7, XMM7
								MOVDQ2Q  MM1, XMM7    // Dest高两点
								MOVQ2DQ     XMM5, MM0
								PXOR     XMM1, XMM1
								PUNPCKLBW XMM5, XMM1	// Src变成0A0R0G0B0A0R0G0B
								MOVQ2DQ     XMM4, MM1
								PUNPCKLBW XMM4, XMM1	// Dest变成0A0R0G0B0A0R0G0B

								// (SRC)*SA
								MOVQ MM3, MM5
								PUNPCKHWD MM3, MM3
								PUNPCKLDQ MM3, MM3
								PUNPCKLWD MM5, MM5
								PUNPCKLDQ MM5, MM5
								MOVQ2DQ XMM1, MM5
								MOVQ2DQ XMM6, MM3
								PUNPCKLQDQ XMM1, XMM6		// 取出SA值变成0A0A0A0A0A0A0A0A
								PMULLW   XMM5, XMM1
								//PAND     XMM5, nMask2

								// (SR)*SA*CR
								PMULLW   XMM5, XMM0		// 对应16位乘
								//PAND     XMM5, nMask2

								// (SR)*SA*CR>>8
								PSRAW  XMM5, 8	
								//PAND   XMM5, nMask2	

								// (SR)*SA*CR>>8+DR
								PADDW  XMM5, XMM4 
								//PAND     XMM5, nMask2

								// Dest*SA*CR>>8
								// (Dest)*SA
								PMULLW   XMM4, XMM1
								//PAND     XMM4, nMask2

								// (DR)*SA*CR
								PMULLW   XMM4, XMM0		// 对应16位乘
								//PAND     XMM4, nMask2

								// (DR)*SA*CR>>8
								PSRAW  XMM4, 8	
								//PAND   XMM4, nMask2	

								PSUBW  XMM5, XMM4      // 对应16位减
								PAND   XMM5, nMask2

								// 插入当前正确的A值
								XOR     EDX, EDX
								PEXTRW  EDX, XMM3, 4	// 抽出第一个A值 
								PINSRW XMM5, EDX, 3
								PEXTRW  EDX, XMM3, 6	// 抽出第二个A值 
								PINSRW XMM5, EDX, 7 // 算出的两个0R0G0B0A值

								// 最终压缩为四个目的RGBA值
								PACKUSWB XMM2, XMM5

								// 弹出指针
								POP EDX
								POP EAX
								MOVUPS  [EDX], XMM2	// 存入Dest四个点的ARGB
								ADD EDX, 10H
								ADD EAX, 10H
								JMP LOOPJ
LOOPJ4:
							SUB ECX, 4			// 每次计算4个值，不足4时跳到循环体4中
LOOPJ41:
							CMP ECX, nDestWidth
								JGE LOOPJEND

								// 取出Src和Dest1个点
								MOVD MM0, [EAX]	// 取出SRC1个点的ARGB
								MOVD MM1, [EDX]	// 取出Dest1个点的ARGB

								// 保存Src和Dest
								MOVQ MM3, MM0
									MOVQ MM4, MM1

									// 取出SA和DA
									PSRAD MM0, 24	// 得到000A值
									PAND  MM0, nMask
									PSRAD MM1, 24
									PAND MM1, nMask

									// 保存SA
									MOVQ  MM6, MM0
									MOVQ  MM7, MM0	

									// SA+DA
									PADDW MM0, MM1
									//PAND  MM0, nMask

									// (SA*DA+FF)>>8
									PMULLW MM6, MM1
									PADDW  MM6, nMask
									PSRLW  MM6, 8
									//PAND   MM6, nMask

									// SA+DA-(SA*DA+0xFF)>>8
									PSUBW  MM0, MM6
									PAND   MM0, nMask

									// 保存指针
									PUSH EAX
									PUSH EDX

									// 计算CA对应的索引
									MOVD   EDX, MM0			// CA
									SHL    EDX, 2
									MOV    EAX, pTable
									ADD	   EDX, EAX    
									MOVD   EAX, MM0		// 保存CA
									MOVD   MM0, [EDX]
									PUNPCKLWD MM0, MM0  
										PUNPCKLDQ MM0, MM0  // 变成0A0A0A0A

										// 大包SR和DR
										PXOR  MM1, MM1
										PUNPCKLBW MM3, MM1	
										PUNPCKLBW MM4, MM1

										// 打包SA
										PUNPCKLWD MM7, MM7  
										PUNPCKLDQ MM7, MM7  // 变成0A0A0A0A

										// (SRC)*SA
										PMULLW   MM3, MM7		// 对应16位乘
										// (SR)*SA*CR
										PMULLW   MM3, MM0
										PSRAW  MM3, 8	
										//PAND   MM3, nMask2
										// (SR)*SA*CR>>8+DR
										PADDW  MM3,  MM4

										// (Dest)*SA
										PMULLW   MM4, MM7		// 对应16位乘
										// (DR)*SA*CR
										PMULLW   MM4, MM0
										PSRAW  MM4, 8	
										//PAND   MM4, nMask2	
										// SR-DR
										PSUBW  MM3, MM4      // 对应16位减
										PAND   MM3, nMask2

										PINSRW MM3, EAX, 3 // 算出的0R0G0B0A值
										// 压缩成最终的ARGB
										PACKUSWB MM3, MM3

										POP EDX
										POP EAX
										MOVD [EDX], MM3
										ADD EDX, 4
										ADD EAX, 4

										INC ECX
										JMP LOOPJ41
LOOPJEND:
									MOV EAX, pDes
										ADD EAX, nAddDestWidth		// nDestBufferWidth需乘4
										MOV [pDes], EAX

										MOV EAX, pSrc
										ADD EAX, nAddSrcWidth
										MOV [pSrc], EAX 
										POP ECX
										INC ECX
										JMP LOOPI
EXITLOOPI:
									EMMS			// 清空
	}
	_mm_free(pTable);
#endif
	//////////////////////////////////////////////////////////////////////////
	// 改进算法，2003－11－19，chyf
	// D = D+(S-D)*SA/KA
	// A = SA+DA-(SA*DA+0xFF)>>8
	//////////////////////////////////////////////////////////////////////////
#if 1
	_declspec(align(16)) DWORD nMask[4] = {0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX			// i清0
LOOPI:
		CMP ECX, nDestHeight	// 第一层循环(i<nDestHeight)
			JGE	EXITLOOPI			// 退出第一层循环(i>=nDestHeight)

			PUSH ECX				// 压入i
			MOV  EAX, pSrc			// 保存源和目标缓冲区指针
			MOV  EDX, pDes
			XOR  ECX, ECX			// j清0， 第二层循环计数器
LOOPJ:
		// 第二层循环体，一次读4个节点
		ADD ECX, 4			// 每次计算4个值，不足4时跳到循环体4中
			CMP ECX, nDestWidth
			JG LOOPJ4		// (j>nDestWidth)

			// 取出Src和Dest4个点存入XMM0和XMM1
			MOVUPS XMM0, [EAX]	// 取出SRC四个点的ARGB
			MOVUPS XMM1, [EDX]	// 取出Dest四个点的ARGB

			// 取出A值，变成000A000A000A000A
			PSRAD  XMM0, 24		// 右移24位，取A值
				PSRAD  XMM1, 24		// 右移24位，取A值
				PAND   XMM0, nMask		// 四个000A
				PAND   XMM1, nMask		// 四个000A

				// Src和Dest变成为000A000A000A000A相加,放入XMM3
				MOVAPS XMM3, XMM0		// 保存SA
				PADDW  XMM3, XMM1		// Src+Dest(16位加)

				// 对应16位相乘，并除256（Src*Dest>>8)
				PMULLW XMM0, XMM1
				PADDW  XMM0, nMask		// 误差补偿
				PSRAD  XMM0, 8			
				//PAND   XMM0, nMask	

				// 对应16位减,得到最终的KA值000A000A000A000A
				PSUBW  XMM3, XMM0	
				PAND   XMM3, nMask
				//////////////////////////////////////////////////////////////////////////
				// 计算第一个点
				// 每次算一个点（D+(S-D)*SA/KA）
				// 先取出SRC的RGBA,打包成000A000R000G000B放入XMM0
				MOVD    MM0, [EAX]
				MOVQ    MM3, MM0		// 保存Src的RGBA，后面用
					PXOR    MM1, MM1
					PUNPCKLBW MM0, MM1
					MOVQ2DQ    XMM0, MM0
					PXOR    XMM2, XMM2
					PUNPCKLWD XMM0, XMM2
					// 转换DWORD成Float,放在XMM1
					CVTDQ2PS  XMM1, XMM0
					// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
					MOVD    MM0, [EDX]
					PUNPCKLBW MM0, MM1
						MOVQ2DQ    XMM0, MM0
						PUNPCKLWD XMM0, XMM2
						// 转换DWORD成Float,放在XMM2
						CVTDQ2PS  XMM2, XMM0

						// 保存指针
						PUSH EDX
						// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
						SUBPS  XMM1, XMM2		// S-D
						// 取出SA
						PUSH EAX
						MOVD   EAX,  MM3
						SHR    EAX,  16
						AND    EAX,  0x0000FF00  // 变成A000
						// 取出KA
						XOR    EDX, EDX
						PEXTRW  EDX, XMM3, 0	// 抽出第一个KA值
						PUSH ECX
						MOV    ECX, EDX
						ADD    ECX, EAX
						SHL    ECX, 2
						ADD	   ECX, g_pfTable    
						MOVSS  XMM4, [ECX]
						PUNPCKLDQ XMM4, XMM4
							PUNPCKLQDQ XMM4, XMM4
							// (S-D)*R
							MULPS XMM1, XMM4
							POP ECX
							POP EAX
							ADD EAX, 4
							// (S-D)*R+D
							ADDPS XMM1, XMM2
							// 转换成整数(000A000R000G000B)
							CVTPS2DQ XMM2, XMM1
							// 插入正确的A值
							PINSRW XMM2, EDX, 6 
							POP EDX
							// 压缩RGBA
							PACKUSWB XMM2, XMM2
							MOVDQ2Q   MM2, XMM2
							PACKUSWB MM2, MM2
							MOVD  [EDX], MM2	// 存入Dest一个点的ARGB
							ADD EDX, 4
							//////////////////////////////////////////////////////////////////////////
							//////////////////////////////////////////////////////////////////////////
							// 计算第二个点
							// 每次算一个点（D+(S-D)*SA/KA）
							// 先取出SRC的RGBA,打包成000A000R000G000B放入XMM0
							MOVD    MM0, [EAX]
							MOVQ    MM3, MM0		// 保存Src的RGBA，后面用
								PXOR    MM1, MM1
								PUNPCKLBW MM0, MM1
								MOVQ2DQ    XMM0, MM0
								PXOR    XMM2, XMM2
								PUNPCKLWD XMM0, XMM2
								// 转换DWORD成Float,放在XMM1
								CVTDQ2PS  XMM1, XMM0
								// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
								MOVD    MM0, [EDX]
								PUNPCKLBW MM0, MM1
									MOVQ2DQ    XMM0, MM0
									PUNPCKLWD XMM0, XMM2
									// 转换DWORD成Float,放在XMM2
									CVTDQ2PS  XMM2, XMM0

									// 保存指针
									PUSH EDX
									// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
									SUBPS  XMM1, XMM2		// S-D
									// 取出SA
									PUSH EAX
									MOVD   EAX,  MM3
									SHR    EAX,  16
									AND    EAX,  0x0000FF00  // 变成A000
									// 取出KA
									XOR    EDX, EDX
									PEXTRW  EDX, XMM3, 2	// 抽出第二个KA值
									PUSH ECX
									MOV    ECX, EDX
									ADD    ECX, EAX
									SHL    ECX, 2
									ADD	   ECX, g_pfTable    
									MOVSS  XMM4, [ECX]
									PUNPCKLDQ XMM4, XMM4
										PUNPCKLQDQ XMM4, XMM4
										// (S-D)*R
										MULPS XMM1, XMM4
										POP ECX
										POP EAX
										ADD EAX, 4
										// (S-D)*R+D
										ADDPS XMM1, XMM2
										// 转换成整数(000A000R000G000B)
										CVTPS2DQ XMM2, XMM1
										// 插入正确的A值
										PINSRW XMM2, EDX, 6 
										POP EDX
										// 压缩RGBA
										PACKUSWB XMM2, XMM2
										MOVDQ2Q   MM2, XMM2
										PACKUSWB MM2, MM2
										MOVD  [EDX], MM2	// 存入Dest一个点的ARGB
										ADD EDX, 4
										//////////////////////////////////////////////////////////////////////////
										//////////////////////////////////////////////////////////////////////////
										// 计算第三个点
										// 每次算一个点（D+(S-D)*SA/KA）
										// 先取出SRC的RGBA,打包成000A000R000G000B放入XMM0
										MOVD    MM0, [EAX]
										MOVQ    MM3, MM0		// 保存Src的RGBA，后面用
											PXOR    MM1, MM1
											PUNPCKLBW MM0, MM1
											MOVQ2DQ    XMM0, MM0
											PXOR    XMM2, XMM2
											PUNPCKLWD XMM0, XMM2
											// 转换DWORD成Float,放在XMM1
											CVTDQ2PS  XMM1, XMM0
											// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
											MOVD    MM0, [EDX]
											PUNPCKLBW MM0, MM1
												MOVQ2DQ    XMM0, MM0
												PUNPCKLWD XMM0, XMM2
												// 转换DWORD成Float,放在XMM2
												CVTDQ2PS  XMM2, XMM0

												// 保存指针
												PUSH EDX
												// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
												SUBPS  XMM1, XMM2		// S-D
												// 取出SA
												PUSH EAX
												MOVD   EAX,  MM3
												SHR    EAX,  16
												AND    EAX,  0x0000FF00  // 变成A000
												// 取出KA
												XOR    EDX, EDX
												PEXTRW  EDX, XMM3, 4	// 抽出第三个KA值
												PUSH ECX
												MOV    ECX, EDX
												ADD    ECX, EAX
												SHL    ECX, 2
												ADD	   ECX, g_pfTable    
												MOVSS  XMM4, [ECX]
												PUNPCKLDQ XMM4, XMM4
													PUNPCKLQDQ XMM4, XMM4
													// (S-D)*R
													MULPS XMM1, XMM4
													POP ECX
													POP EAX
													ADD EAX, 4
													// (S-D)*R+D
													ADDPS XMM1, XMM2
													// 转换成整数(000A000R000G000B)
													CVTPS2DQ XMM2, XMM1
													// 插入正确的A值
													PINSRW XMM2, EDX, 6 
													POP EDX
													// 压缩RGBA
													PACKUSWB XMM2, XMM2
													MOVDQ2Q   MM2, XMM2
													PACKUSWB MM2, MM2
													MOVD  [EDX], MM2	// 存入Dest一个点的ARGB
													ADD EDX, 4
													//////////////////////////////////////////////////////////////////////////
													//////////////////////////////////////////////////////////////////////////
													// 计算第四个点
													// 每次算一个点（D+(S-D)*SA/KA）
													// 先取出SRC的RGBA,打包成000A000R000G000B放入XMM0
													MOVD    MM0, [EAX]
													MOVQ    MM3, MM0		// 保存Src的RGBA，后面用
														PXOR    MM1, MM1
														PUNPCKLBW MM0, MM1
														MOVQ2DQ    XMM0, MM0
														PXOR    XMM2, XMM2
														PUNPCKLWD XMM0, XMM2
														// 转换DWORD成Float,放在XMM1
														CVTDQ2PS  XMM1, XMM0
														// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
														MOVD    MM0, [EDX]
														PUNPCKLBW MM0, MM1
															MOVQ2DQ    XMM0, MM0
															PUNPCKLWD XMM0, XMM2
															// 转换DWORD成Float,放在XMM2
															CVTDQ2PS  XMM2, XMM0

															// 保存指针
															PUSH EDX
															// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
															SUBPS  XMM1, XMM2		// S-D
															PUSH EAX
															// 取出SA
															MOVD   EAX,  MM3
															SHR    EAX,  16
															AND    EAX,  0x0000FF00  // 变成A000
															// 取出KA
															XOR    EDX, EDX
															PEXTRW  EDX, XMM3, 6	// 抽出第一个KA值
															PUSH ECX
															MOV    ECX, EDX
															ADD    ECX, EAX
															SHL    ECX, 2
															ADD	   ECX, g_pfTable    
															MOVSS  XMM4, [ECX]
															PUNPCKLDQ XMM4, XMM4
																PUNPCKLQDQ XMM4, XMM4
																// (S-D)*R
																MULPS XMM1, XMM4
																POP ECX
																POP EAX
																ADD EAX, 4
																// (S-D)*R+D
																ADDPS XMM1, XMM2
																// 转换成整数(000A000R000G000B)
																CVTPS2DQ XMM2, XMM1
																// 插入正确的A值
																PINSRW XMM2, EDX, 6 
																POP EDX
																// 压缩RGBA
																PACKUSWB XMM2, XMM2
																MOVDQ2Q   MM2, XMM2
																PACKUSWB MM2, MM2
																MOVD  [EDX], MM2	// 存入Dest一个点的ARGB
																ADD EDX, 4
																//////////////////////////////////////////////////////////////////////////
																JMP LOOPJ
LOOPJ4:
															SUB ECX, 4			// 每次计算4个值，不足4时跳到循环体4中
LOOPJ41:
															CMP ECX, nDestWidth
																JGE LOOPJEND

																// 先求目标A的值
																// 取出Src和Dest1个点
																MOVD MM0, [EAX]	// 取出SRC1个点的ARGB
																MOVD MM1, [EDX]	// 取出Dest1个点的ARGB

																// 保存Src和Dest
																MOVQ MM3, MM0
																	MOVQ MM4, MM1

																	// 取出SA和DA
																	PSRAD MM0, 24	// 得到000A值
																	PAND  MM0, nMask
																	PSRAD MM1, 24
																	PAND MM1, nMask

																	// 保存SA
																	MOVQ  MM6, MM0
																	MOVQ  MM7, MM0	

																	// SA+DA
																	PADDW MM0, MM1
																	//PAND  MM0, nMask

																	// (SA*DA+FF)>>8
																	PMULLW MM6, MM1
																	PADDW  MM6, nMask
																	PSRLW  MM6, 8
																	//PAND   MM6, nMask

																	// SA+DA-(SA*DA+0xFF)>>8
																	PSUBW  MM0, MM6
																	PAND   MM0, nMask
																	// 求出目标RGBA
																	// 打包Src和Dest的RGBA值
																	PXOR    MM1, MM1
																	PUNPCKLBW MM3, MM1
																	MOVQ2DQ    XMM0, MM3
																	PXOR    XMM1, XMM1
																	PUNPCKLWD XMM0, XMM1
																	// 转换DWORD成Float,放在XMM1
																	CVTDQ2PS  XMM1, XMM0
																	// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
																	PUNPCKLBW MM4, MM1
																	MOVQ2DQ    XMM0, MM4
																	PXOR    XMM2, XMM2
																	PUNPCKLWD XMM0, XMM2
																	// 转换DWORD成Float,放在XMM2
																	CVTDQ2PS  XMM2, XMM0

																	// 保存指针
																	PUSH EDX
																	PUSH EAX
																	PUSH ECX
																	// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
																	SUBPS  XMM1, XMM2		// S-D
																	// 取出SA
																	MOVD   EAX,  MM7
																	SHL    EAX,  8
																	AND    EAX,  0x0000FF00  // 变成A000
																	// 取出KA
																	MOVD   ECX, MM0
																	MOV    EDX, ECX
																	ADD    ECX, EAX
																	SHL    ECX, 2
																	ADD	   ECX, g_pfTable    
																	MOVSS  XMM4, [ECX]
																	PUNPCKLDQ XMM4, XMM4
																		PUNPCKLQDQ XMM4, XMM4
																		// (S-D)*R
																		MULPS XMM1, XMM4
																		POP ECX
																		POP EAX
																		ADD EAX, 4
																		// (S-D)*R+D
																		ADDPS XMM1, XMM2
																		INC ECX
																		// 转换成整数(000A000R000G000B)
																		CVTPS2DQ XMM2, XMM1
																		// 插入正确的A值
																		PINSRW XMM2, EDX, 6 
																		POP EDX
																		// 压缩RGBA
																		PACKUSWB XMM2, XMM2
																		MOVDQ2Q   MM2, XMM2
																		PACKUSWB MM2, MM2
																		MOVD  [EDX], MM2	// 存入Dest一个点的ARGB

																		ADD EDX, 4
																		JMP LOOPJ41
LOOPJEND:
																	MOV EAX, pDes
																		ADD EAX, nAddDestWidth		// nDestBufferWidth需乘4
																		MOV [pDes], EAX

																		MOV EAX, pSrc
																		ADD EAX, nAddSrcWidth
																		MOV [pSrc], EAX 
																		POP ECX
																		ADD ECX, nInc
																		JMP LOOPI
EXITLOOPI:
																	EMMS			// 清空
	}
#endif
#if 0
	// 原算法
	for(int i=0; i<nDestHeight; i++)
	{
		if(nDestY+i >= nDestBufferHeight)
			break;
		if(nSrcY+i >= nSrcBufferHeight)
			break;
		for(int j=0; j<nDestWidth; j++)
		{
			int nTemp = nSrcX+j;
			if(nDestX+j >= nDestBufferWidth)
				break;
			if(nTemp >= nSrcBufferWidth)
				break;
			TPRGBA	rgbSrc, rgbDest, rgbCur;
			rgbSrc.m_dwColor = pSrc[j];
			rgbDest.m_dwColor = pDes[j];
			float fMix = rgbSrc.m_bColor.bA/(float)255;
			rgbCur.m_bColor.bR = (BYTE)(rgbSrc.m_bColor.bR*fMix + (1-fMix)*rgbDest.m_bColor.bR);
			rgbCur.m_bColor.bG = (BYTE)(rgbSrc.m_bColor.bG*fMix + (1-fMix)*rgbDest.m_bColor.bG);
			rgbCur.m_bColor.bB = (BYTE)(rgbSrc.m_bColor.bB*fMix + (1-fMix)*rgbDest.m_bColor.bB);
			//////////////////////////////////////////////////////////////////////////
			//float fSrcMix = rgbSrc.m_bColor.bA/(float)255;
			//float fDesMix = rgbDest.m_bColor.bA/(float)255;
			//float fA = (fSrcMix+fDesMix-fSrcMix*fDesMix);
			rgbCur.m_bColor.bA = (BYTE)(rgbSrc.m_bColor.bA*fMix + (1-fMix)*rgbDest.m_bColor.bA);

			pDes[j] = rgbCur.m_dwColor;
		}
		pDes += nDestBufferWidth;
		pSrc += nSrcBufferWidth;
	}
#endif
#if 0
	// 改进算法：2003－11－11，chyf
	// 目标A值＝SA+DA-SA*DA
	// 目标颜色值＝(D*DA*(1-SA)+S*SA)/(SA+DA-SA*DA)
	float* pfTable = NULL;
	TPBuffer_BitBltCreateTable(pfTable);
	for(int i=0; i<nDestHeight; i++)
	{
		for(int j=0; j<nDestWidth; j++)
		{
			TPRGBA	rgbSrc, rgbDest, rgbCur;
			rgbSrc.m_dwColor = pSrc[j];
			rgbDest.m_dwColor = pDes[j];
			//			rgbCur.m_bColor.bA = (BYTE)(rgbSrc.m_bColor.bA+rgbDest.m_bColor.bA-((rgbDest.m_bColor.bA*rgbSrc.m_bColor.bA)/255.0f));

			rgbCur.m_bColor.bA = (BYTE)(rgbSrc.m_bColor.bA+rgbDest.m_bColor.bA-(((rgbSrc.m_bColor.bA*rgbDest.m_bColor.bA+0xFF))>>8));//(BYTE)(fA*256);
			float fdA = rgbDest.m_bColor.bA/255.0f;
			float fsA = rgbSrc.m_bColor.bA/255.0f;
			float fkA = fdA+fsA-fdA*fsA;
			BYTE bKA = rgbCur.m_bColor.bA;
			BYTE bSA = rgbSrc.m_bColor.bA;
			BYTE bDA = rgbDest.m_bColor.bA;
			//			rgbCur.m_bColor.bR = (BYTE)((rgbDest.m_bColor.bR*fdA*(1-fsA)+rgbSrc.m_bColor.bR*fsA)/fkA);
			//			rgbCur.m_bColor.bG = (BYTE)((rgbDest.m_bColor.bG*fdA*(1-fsA)+rgbSrc.m_bColor.bG*fsA)/fkA);
			//			rgbCur.m_bColor.bB = (BYTE)((rgbDest.m_bColor.bB*fdA*(1-fsA)+rgbSrc.m_bColor.bB*fsA)/fkA);
			if(bKA==0)
			{
				bKA=1;
			}
			//rgbCur.m_bColor.bR = (BYTE)((rgbDest.m_bColor.bR*(255-bSA*pTable[bKA]/255)+rgbSrc.m_bColor.bR*(bSA*pTable[bKA])/255)/255);
			//rgbCur.m_bColor.bG = (BYTE)((rgbDest.m_bColor.bG*(255-bSA*pTable[bKA]/255)+rgbSrc.m_bColor.bG*(bSA*pTable[bKA])/255)/255);
			//rgbCur.m_bColor.bB = (BYTE)((rgbDest.m_bColor.bB*(255-bSA*pTable[bKA]/255)+rgbSrc.m_bColor.bB*(bSA*pTable[bKA])/255)/255);
			rgbCur.m_bColor.bR =  (BYTE)(((float)rgbDest.m_bColor.bR+(((float)rgbSrc.m_bColor.bR-(float)rgbDest.m_bColor.bR)*pfTable[rgbSrc.m_bColor.bA*256+bKA])));//rgbSrc.m_bColor.bA/(float)bKA)));
			rgbCur.m_bColor.bG =  (BYTE)(((float)rgbDest.m_bColor.bG+(((float)rgbSrc.m_bColor.bG-(float)rgbDest.m_bColor.bG)*pfTable[rgbSrc.m_bColor.bA*256+bKA])));//rgbSrc.m_bColor.bA/(float)bKA)));
			rgbCur.m_bColor.bB =  (BYTE)(((float)rgbDest.m_bColor.bB+(((float)rgbSrc.m_bColor.bB-(float)rgbDest.m_bColor.bB)*pfTable[rgbSrc.m_bColor.bA*256+bKA])));//rgbSrc.m_bColor.bA/(float)bKA)));

			pDes[j] = rgbCur.m_dwColor;
		}
		pDes += nDestBufferWidth;
		pSrc += nSrcBufferWidth;
	}
	TPBuffer_FreeTable(pfTable);
#endif
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 进行拷贝填充缓冲区
BOOL TPBuffer_Copy(DWORD* pdwDestBuffer,		// 目标缓冲区
				   int nDestX,
				   int nDestY,
				   int nDestWidth,
				   int nDestHeight,
				   int nDestBufferWidth,
				   int nDestBufferHeight,
				   DWORD* pdwSrcBuffer,			// 源缓冲区
				   int nSrcX,
				   int nSrcY,
				   int nSrcWidth,
				   int nSrcHeight,
				   int nSrcBufferWidth,
				   int nSrcBufferHeight,
				   int nParity)					// 奇偶场
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
	int nTempWidth = nSrcX+nDestWidth;
	int nTempHeight = nSrcY+nDestHeight;
	if(nTempWidth > nSrcBufferWidth)
		nDestWidth -= (nTempWidth-nSrcBufferWidth);
	if(nTempHeight > nSrcBufferHeight)
		nDestHeight -= (nTempHeight-nSrcBufferHeight);
	nTempWidth = nDestX+nDestWidth;
	nTempHeight = nDestY+nDestHeight;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	nSrcWidth = nDestWidth;
	nSrcHeight = nDestHeight;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;

	// 每次加的宽度
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// 计数器的增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;
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
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pSrc += nSrcBufferWidth;
		}
	}
	else
	{
		// 起点为偶场
		if(nParity == TP_PARITY_ODD)
		{
			// 如果奇场拷贝，则从下一行开始
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pSrc += nSrcBufferWidth;
		}
	}

#if 1
	// 一行行赋值,SSE2优化
	_asm
	{
		XOR ECX, ECX	// 清0
LOOP1:
		CMP ECX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1	 // 退出第一层循环(i>=nDestHeight)
			PUSH ECX
			XOR ECX, ECX	 // 清0， 第二层循环计数器
			MOV EAX, pSrc
			MOV EDX, pDes
LOOP2:
		ADD ECX, 4			// 每次计算4个值，不足4时跳到循环体2中
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			// 读取&pSrc[j]后的四个值放入XMM0
			MOVDQU XMM0, [EAX]			
			ADD EAX, 10H

				// 赋给&pDes[j]后的四个点
				MOVDQU  [EDX], XMM0
				ADD EDX, 10H
				JMP LOOP2
LOOP2BODY2:
			SUB ECX, 4			// 不足4时
LOOP2BODY3:
			CMP ECX, nDestWidth
				JGE LOOP2END	

				// 读取pSrc[j]的值
				MOVD MM0, [EAX]
				ADD EAX, 4
					// 读取pDes[j]的值
					MOVD  [EDX], MM0
					ADD EDX, 4
					INC ECX
					JMP LOOP2BODY3
LOOP2END:
				MOV EAX, nAddDestWidth		// nDestBufferWidth需乘4
					ADD [pDes], EAX

					MOV EAX, nAddSrcWidth
					ADD [pSrc], EAX

					POP ECX
					ADD	ECX, nInc			 // 计数器自增，返回循环1 
					JMP LOOP1
EXITLOOP1:
				EMMS			// 清空
	}
#endif
#if 0
	for(int i=0; i<nDestHeight; i++)
	{
		for(int j=0; j<nDestWidth; j++)
		{
			pDes[j] = pSrc[j];
		}
		pDes += nDestBufferWidth;
		pSrc += nSrcBufferWidth;
	}
#endif
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 功能：进行NotMix填充缓冲区，即源Mix取反
// 
// 2003－10-10,V1.0，修改者：陈玉峰
// 改进算法：KR = (255-SA)/255*SR+SA/255*DR
// 2003－11－20,V2.0，修改者：陈玉峰
// 改进算法：KA = (255-SA)+DA-(255-SA)*DA/255, KR = DR+(SR-DR)*(255-SA)/KA
// 2004-02-11,V2.1,修改者：陈玉峰
// 改进：增加了奇偶场信息处理
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_NotMix(DWORD* pdwDestBuffer, 
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
					 int nParity)
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
	int nTempWidth = nSrcX+nDestWidth;
	int nTempHeight = nSrcY+nDestHeight;
	if(nTempWidth > nSrcBufferWidth)
		nDestWidth -= (nTempWidth-nSrcBufferWidth);
	if(nTempHeight > nSrcBufferHeight)
		nDestHeight -= (nTempHeight-nSrcBufferHeight);
	nTempWidth = nDestX+nDestWidth;
	nTempHeight = nDestY+nDestHeight;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	nSrcWidth = nDestWidth;
	nSrcHeight = nDestHeight;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;

	//////////////////////////////////////////////////////////////////////////
	// 增加了奇偶场信息，2004－02－11，chyf
	// 每次加的宽度
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// 计数器的增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;
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
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pSrc += nSrcBufferWidth;
		}
	}
	else
	{
		// 起点为偶场
		if(nParity == TP_PARITY_ODD)
		{
			// 如果奇场拷贝，则从下一行开始
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pSrc += nSrcBufferWidth;
		}
	}
	// 增加奇偶场信息
	//////////////////////////////////////////////////////////////////////////

#if 0
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};
	// 一行行赋值,SSE2优化
	_asm
	{
		XOR EAX, EAX	// 清0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1		 // 退出第一层循环(i>=nDestHeight)
			MOV	ECX, nDestY		// 计算nDestY+i
			ADD ECX, EAX
			CMP ECX, nDestBufferHeight
			JGE	EXITLOOP1	 // 退出第一层循环(nDestY+i>=nDestBufferHeight)
			MOV	ECX, nSrcY	 // 计算nSrcY+i
			ADD ECX, EAX
			CMP ECX, nSrcBufferHeight
			JGE	EXITLOOP1	 // 退出第一层循环(nSrcY+i>=nSrcBufferHeight)
			XOR ECX, ECX	 // 清0， 第二层循环计数器
			PUSH EAX
LOOP2:
		CMP ECX, nDestWidth  // 第二层循环(j<nDestWidth)
			JGE LOOP2END		 // 进入第二层循环体(j<nDestWidth)

			ADD ECX, 2			 // 每次计算2个值，不足2时跳到循环体2中
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2

			MOV EDX, nSrcX	// 计算nSrcX+j
			ADD EDX, ECX
			CMP EDX, nSrcBufferWidth
			JGE LOOP2BODY2	// 退出第二层循环(nSrcX+j>=nSrcBufferWidth)

			MOV EDX, nDestX	// 计算nDestX+j
			ADD EDX, ECX
			CMP EDX, nDestBufferWidth
			JGE LOOP2BODY2	// 退出第二层循环(nDestX+j>=nDestBufferWidth)	

			MOV EAX, pSrc	// 读取pSrc[j]的值放入MM0
			MOV EDX, ECX
			SHL EDX, 2
			ADD EAX, EDX 
			MOVD MM0, [EAX]

			PUSH ECX
				MOV ECX, [EAX]					// 读取其Alpha值
				SHR	ECX,  24					// 右移24位
					AND ECX, 0x000000FF
					MOVD MM3, ECX
					PUNPCKLWD MM3, MM3
					PUNPCKLDQ MM3, MM3				// 变为0A0A0A0A

					MOV EDX, 0xFF					// 255-Alpha
					SUB EDX, ECX
					MOVD MM7, EDX
					PUNPCKLWD MM7, MM7
					PUNPCKLDQ MM7, MM7				// 变为0A0A0A0A
					POP ECX

					ADD EAX, 4		// 读取pSrc[j+1]的值放入MM1
					MOVD MM1, [EAX]

					PUSH ECX
						MOV ECX, [EAX]					// 读取其Alpha值
						SHR	ECX,  24					// 右移24位
							AND ECX, 0x000000FF
							MOVD MM4, ECX
							PUNPCKLWD MM4, MM4
							PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

							MOV EAX, 0xFF					// 255-Alpha
							SUB EAX, ECX
							MOVD MM6, EAX
							PUNPCKLWD MM6, MM6
							PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A
							POP ECX

							XOR EAX, EAX	// 清0
							MOVD MM2, EAX
							PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
							PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
							MOVQ2DQ XMM0, MM0
							MOVQ2DQ XMM1, MM1
							PUNPCKLQDQ XMM0, XMM1 // 变为0R0G0B0A0R0G0B0A

							MOV EAX, pDes	 // 读取pDes[j]的值
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // 保留指针，后面要用
							MOVD MM0, [EAX]
							ADD EAX, 4		// 读取pDes[j+1]的值
								MOVD MM1, [EAX]
								XOR EAX, EAX	// 清0
									MOVD MM2, EAX
									PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
									PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
									MOVQ2DQ XMM1, MM0
									MOVQ2DQ XMM2, MM1
									PUNPCKLQDQ XMM1, XMM2 // 变为0R0G0B0A0R0G0B0A
									MOVDQA	XMM3, XMM1	 // 复制一份

									MOVQ2DQ XMM2, MM3		// 取源的alpha值
									MOVQ2DQ XMM4, MM4
									PUNPCKLQDQ XMM2, XMM4	// 变为0A0A0A0A0M0M0M0M

									MOVQ2DQ XMM4, MM7		// 取255-alpha值
									MOVQ2DQ XMM5, MM6
									PUNPCKLQDQ XMM4, XMM5	// 变为0A0A0A0A0M0M0M0M

									PMULLW XMM0, XMM4	// 对应16位相乘S(1-A)
									// 误差补偿
									MOVQ2DQ		XMM7,MM5
									MOVLHPS		XMM7,XMM7
									PADDSW		XMM0,XMM7
									PSRLW  XMM0, 8		// 每16位右移8位（相当于/255)(要进行误差补偿)

									PMULLW XMM1, XMM2	// 对应16位相乘DA
									// 误差补偿
									PADDSW		XMM1,XMM7
									PSRLW  XMM1, 8

									PADDSW XMM0, XMM1	// 对应16位相加(SA+DA)

									PACKUSWB XMM0, XMM0 // 压缩，每16位压缩为8位(RGBARGBA)

									MOVDQ2Q	MM0,XMM0	// 取低64位(两个DWORD)
									POP EAX					// 赋给pDest
									MOVQ  [EAX], MM0

									ADD ECX, 2			// j增2，返回第二层循环
									JMP LOOP2
LOOP2BODY2:
								SUB ECX, 2			// 不足2时
									CMP ECX, nDestWidth
									JGE LOOP2END

									MOV EDX, nSrcX	// 计算nSrcX+j
									ADD EDX, ECX
									CMP EDX, nSrcBufferWidth
									JGE LOOP2END	// 退出第二层循环(nSrcX+j>=nSrcBufferWidth)

									MOV EDX, nDestX	// 计算nDestX+j
									ADD EDX, ECX
									CMP EDX, nDestBufferWidth
									JGE LOOP2END	// 退出第二层循环(nDestX+j>=nDestBufferWidth)		

									MOV EAX, pSrc		// 读取pSrc[j]的值
									MOV EDX, ECX
									SHL EDX, 2
									ADD EAX, EDX 
									MOVD MM0, [EAX]

									PUSH ECX
										MOV ECX, [EAX]					// 读取其Alpha值
										SHR	ECX,  24					// 右移24位
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A
											POP ECX

											XOR EAX, EAX	// 清0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A

											MOV EAX, pDes	 // 读取pDes[j]的值
											MOV EDX, ECX
											SHL EDX, 2
											ADD EAX, EDX

											PUSH  EAX		 // 保留指针，后面要用
											MOVD MM1, [EAX]
											XOR EAX, EAX	// 清0
												MOVD MM2, EAX
												PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
												MOVQ	MM3, MM1	 // 复制一份

												PMULLW MM0, MM6	// 对应16位相乘S(1-A)
												// 误差补偿
												PADDSW MM0, MM5
												PSRLW  MM0, 8		// 右移8位（相当于/255)(要进行误差补偿)

												PMULLW MM1, MM4	// 对应16位相乘DA
												// 误差补偿
												PADDSW MM1, MM5
												PSRLW  MM1, 8
												PADDSW MM0, MM1	// 对应16位相加
												PACKUSWB MM0, MM0 // 压缩，每16位压缩为8位(RGBARGBA)

												POP EAX					// 赋给pDest
												MOVD  [EAX], MM0
LOOP2END:
											MOV EAX, pDes
												ADD EAX, nAddDestWidth		// nDestBufferWidth需乘4
												MOV [pDes], EAX

												MOV EAX, pSrc
												ADD EAX, nAddSrcWidth
												MOV [pSrc], EAX

												POP EAX
												INC	EAX			 // 计数器自增，返回循环1 
												JMP LOOP1
EXITLOOP1:
											EMMS			// 清空
	}
#endif
#if 1
	// 改进算法：2003－11－20，修改者：陈玉峰
	// KA = (255-SA)+DA-(255-SA)*DA/255, KR = DR+(SR-DR)*(255-SA)/KA
	_declspec(align(16)) DWORD nMask[4] = {0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX			// i清0
LOOPI:
		CMP ECX, nDestHeight	// 第一层循环(i<nDestHeight)
			JGE	EXITLOOPI			// 退出第一层循环(i>=nDestHeight)

			PUSH ECX				// 压入i
			MOV  EAX, pSrc			// 保存源和目标缓冲区指针
			MOV  EDX, pDes
			XOR  ECX, ECX			// j清0， 第二层循环计数器
LOOPJ:
		// 第二层循环体，一次读4个节点
		ADD ECX, 4			// 每次计算4个值，不足4时跳到循环体4中
			CMP ECX, nDestWidth
			JG LOOPJ4		// (j>nDestWidth)

			// 取出Src和Dest4个点存入XMM0和XMM1
			MOVUPS XMM2, [EAX]	// 取出SRC四个点的ARGB
			MOVUPS XMM1, [EDX]	// 取出Dest四个点的ARGB

			// 取出A值，变成000A000A000A000A
			PSRAD  XMM2, 24		// 右移24位，取A值
				PSRAD  XMM1, 24		// 右移24位，取A值
				PAND   XMM2, nMask		// 四个000A
				PAND   XMM1, nMask		// 四个000A
				MOVUPS XMM0, nMask
				PSUBD  XMM0, XMM2		// 255-SA

				// Src和Dest变成为000A000A000A000A相加,放入XMM3
				MOVAPS XMM3, XMM0		// 保存(255-SA)
				PADDW  XMM3, XMM1		// (255-SA)+DA(16位加)

				// 对应16位相乘，并除256（(255-SA)*DA>>8)
				PMULLW XMM0, XMM1
				PADDW  XMM0, nMask		// 误差补偿
				PSRAD  XMM0, 8			
				//PAND   XMM0, nMask	

				// 对应16位减,得到最终的KA值000A000A000A000A
				PSUBW  XMM3, XMM0	
				PAND   XMM3, nMask
				//////////////////////////////////////////////////////////////////////////
				// 计算第一个点
				// 每次算一个点（D+(S-D)*(255-SA)/KA）
				// 先取出SRC的RGBA,打包成000A000R000G000B放入XMM0
				MOVD    MM0, [EAX]
				MOVQ    MM3, MM0		// 保存Src的RGBA，后面用
					PXOR    MM1, MM1
					PUNPCKLBW MM0, MM1
					MOVQ2DQ    XMM0, MM0
					PXOR    XMM2, XMM2
					PUNPCKLWD XMM0, XMM2
					// 转换DWORD成Float,放在XMM1
					CVTDQ2PS  XMM1, XMM0
					// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
					MOVD    MM0, [EDX]
					PUNPCKLBW MM0, MM1
						MOVQ2DQ    XMM0, MM0
						PUNPCKLWD XMM0, XMM2
						// 转换DWORD成Float,放在XMM2
						CVTDQ2PS  XMM2, XMM0

						// 保存指针
						PUSH EDX
						// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
						SUBPS  XMM1, XMM2		// S-D
						// 取出SA,算出(255-SA)*256
						PUSH EAX
						MOV    EAX,  0xFFFFFFFF
						MOVD   MM0,  EAX
						PSUBB  MM0,  MM3
						MOVD   EAX,  MM0
						SHR    EAX,  16
						AND    EAX,  0x0000FF00  
						// 取出KA
						XOR    EDX, EDX
						PEXTRW  EDX, XMM3, 0	// 抽出第一个KA值
						PUSH ECX
						MOV    ECX, EDX
						ADD    ECX, EAX
						SHL    ECX, 2
						ADD	   ECX, g_pfTable    
						MOVSS  XMM4, [ECX]
						PUNPCKLDQ XMM4, XMM4
							PUNPCKLQDQ XMM4, XMM4
							// (S-D)*R
							MULPS XMM1, XMM4
							POP ECX
							POP EAX
							ADD EAX, 4
							// (S-D)*R+D
							ADDPS XMM1, XMM2
							// 转换成整数(000A000R000G000B)
							CVTPS2DQ XMM2, XMM1
							// 插入正确的A值
							PINSRW XMM2, EDX, 6 
							POP EDX
							// 压缩RGBA
							PACKUSWB XMM2, XMM2
							MOVDQ2Q   MM2, XMM2
							PACKUSWB MM2, MM2
							MOVD  [EDX], MM2	// 存入Dest一个点的ARGB
							ADD EDX, 4
							//////////////////////////////////////////////////////////////////////////
							//////////////////////////////////////////////////////////////////////////
							// 计算第二个点
							// 每次算一个点（D+(S-D)*SA/KA）
							// 先取出SRC的RGBA,打包成000A000R000G000B放入XMM0
							MOVD    MM0, [EAX]
							MOVQ    MM3, MM0		// 保存Src的RGBA，后面用
								PXOR    MM1, MM1
								PUNPCKLBW MM0, MM1
								MOVQ2DQ    XMM0, MM0
								PXOR    XMM2, XMM2
								PUNPCKLWD XMM0, XMM2
								// 转换DWORD成Float,放在XMM1
								CVTDQ2PS  XMM1, XMM0
								// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
								MOVD    MM0, [EDX]
								PUNPCKLBW MM0, MM1
									MOVQ2DQ    XMM0, MM0
									PUNPCKLWD XMM0, XMM2
									// 转换DWORD成Float,放在XMM2
									CVTDQ2PS  XMM2, XMM0

									// 保存指针
									PUSH EDX
									// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
									SUBPS  XMM1, XMM2		// S-D
									// 取出SA,算出(255-SA)*256
									PUSH EAX
									MOV    EAX,  0xFFFFFFFF
									MOVD   MM0,  EAX
									PSUBB  MM0,  MM3
									MOVD   EAX,  MM0
									SHR    EAX,  16
									AND    EAX,  0x0000FF00  // 变成A000
									// 取出KA
									XOR    EDX, EDX
									PEXTRW  EDX, XMM3, 2	// 抽出第二个KA值
									PUSH ECX
									MOV    ECX, EDX
									ADD    ECX, EAX
									SHL    ECX, 2
									ADD	   ECX, g_pfTable    
									MOVSS  XMM4, [ECX]
									PUNPCKLDQ XMM4, XMM4
										PUNPCKLQDQ XMM4, XMM4
										// (S-D)*R
										MULPS XMM1, XMM4
										POP ECX
										POP EAX
										ADD EAX, 4
										// (S-D)*R+D
										ADDPS XMM1, XMM2
										// 转换成整数(000A000R000G000B)
										CVTPS2DQ XMM2, XMM1
										// 插入正确的A值
										PINSRW XMM2, EDX, 6 
										POP EDX
										// 压缩RGBA
										PACKUSWB XMM2, XMM2
										MOVDQ2Q   MM2, XMM2
										PACKUSWB MM2, MM2
										MOVD  [EDX], MM2	// 存入Dest一个点的ARGB
										ADD EDX, 4
										//////////////////////////////////////////////////////////////////////////
										//////////////////////////////////////////////////////////////////////////
										// 计算第三个点
										// 每次算一个点（D+(S-D)*SA/KA）
										// 先取出SRC的RGBA,打包成000A000R000G000B放入XMM0
										MOVD    MM0, [EAX]
										MOVQ    MM3, MM0		// 保存Src的RGBA，后面用
											PXOR    MM1, MM1
											PUNPCKLBW MM0, MM1
											MOVQ2DQ    XMM0, MM0
											PXOR    XMM2, XMM2
											PUNPCKLWD XMM0, XMM2
											// 转换DWORD成Float,放在XMM1
											CVTDQ2PS  XMM1, XMM0
											// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
											MOVD    MM0, [EDX]
											PUNPCKLBW MM0, MM1
												MOVQ2DQ    XMM0, MM0
												PUNPCKLWD XMM0, XMM2
												// 转换DWORD成Float,放在XMM2
												CVTDQ2PS  XMM2, XMM0

												// 保存指针
												PUSH EDX
												// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
												SUBPS  XMM1, XMM2		// S-D
												// 取出SA,算出(255-SA)*256
												PUSH EAX
												MOV    EAX,  0xFFFFFFFF
												MOVD   MM0,  EAX
												PSUBB  MM0,  MM3
												MOVD   EAX,  MM0
												SHR    EAX,  16
												AND    EAX,  0x0000FF00  // 变成A000
												// 取出KA
												XOR    EDX, EDX
												PEXTRW  EDX, XMM3, 4	// 抽出第三个KA值
												PUSH ECX
												MOV    ECX, EDX
												ADD    ECX, EAX
												SHL    ECX, 2
												ADD	   ECX, g_pfTable    
												MOVSS  XMM4, [ECX]
												PUNPCKLDQ XMM4, XMM4
													PUNPCKLQDQ XMM4, XMM4
													// (S-D)*R
													MULPS XMM1, XMM4
													POP ECX
													POP EAX
													ADD EAX, 4
													// (S-D)*R+D
													ADDPS XMM1, XMM2
													// 转换成整数(000A000R000G000B)
													CVTPS2DQ XMM2, XMM1
													// 插入正确的A值
													PINSRW XMM2, EDX, 6 
													POP EDX
													// 压缩RGBA
													PACKUSWB XMM2, XMM2
													MOVDQ2Q   MM2, XMM2
													PACKUSWB MM2, MM2
													MOVD  [EDX], MM2	// 存入Dest一个点的ARGB
													ADD EDX, 4
													//////////////////////////////////////////////////////////////////////////
													//////////////////////////////////////////////////////////////////////////
													// 计算第四个点
													// 每次算一个点（D+(S-D)*SA/KA）
													// 先取出SRC的RGBA,打包成000A000R000G000B放入XMM0
													MOVD    MM0, [EAX]
													MOVQ    MM3, MM0		// 保存Src的RGBA，后面用
														PXOR    MM1, MM1
														PUNPCKLBW MM0, MM1
														MOVQ2DQ    XMM0, MM0
														PXOR    XMM2, XMM2
														PUNPCKLWD XMM0, XMM2
														// 转换DWORD成Float,放在XMM1
														CVTDQ2PS  XMM1, XMM0
														// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
														MOVD    MM0, [EDX]
														PUNPCKLBW MM0, MM1
															MOVQ2DQ    XMM0, MM0
															PUNPCKLWD XMM0, XMM2
															// 转换DWORD成Float,放在XMM2
															CVTDQ2PS  XMM2, XMM0

															// 保存指针
															PUSH EDX
															// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
															SUBPS  XMM1, XMM2		// S-D
															PUSH EAX
															// 取出SA,算出(255-SA)*256
															PUSH EAX
															MOV    EAX,  0xFFFFFFFF
															MOVD   MM0,  EAX
															PSUBB  MM0,  MM3
															MOVD   EAX,  MM0
															SHR    EAX,  16
															AND    EAX,  0x0000FF00  // 变成A000
															// 取出KA
															XOR    EDX, EDX
															PEXTRW  EDX, XMM3, 6	// 抽出第一个KA值
															PUSH ECX
															MOV    ECX, EDX
															ADD    ECX, EAX
															SHL    ECX, 2
															ADD	   ECX, g_pfTable    
															MOVSS  XMM4, [ECX]
															PUNPCKLDQ XMM4, XMM4
																PUNPCKLQDQ XMM4, XMM4
																// (S-D)*R
																MULPS XMM1, XMM4
																POP ECX
																POP EAX
																ADD EAX, 4
																// (S-D)*R+D
																ADDPS XMM1, XMM2
																// 转换成整数(000A000R000G000B)
																CVTPS2DQ XMM2, XMM1
																// 插入正确的A值
																PINSRW XMM2, EDX, 6 
																POP EDX
																// 压缩RGBA
																PACKUSWB XMM2, XMM2
																MOVDQ2Q   MM2, XMM2
																PACKUSWB MM2, MM2
																MOVD  [EDX], MM2	// 存入Dest一个点的ARGB
																ADD EDX, 4
																//////////////////////////////////////////////////////////////////////////
																JMP LOOPJ
LOOPJ4:
															SUB ECX, 4			// 每次计算4个值，不足4时跳到循环体4中
LOOPJ41:
															CMP ECX, nDestWidth
																JGE LOOPJEND

																// 先求目标A的值
																// 取出Src和Dest1个点
																MOVD MM2, [EAX]	// 取出SRC1个点的ARGB
																MOVD MM1, [EDX]	// 取出Dest1个点的ARGB

																// 保存Src和Dest
																MOVQ MM3, MM2
																	MOVQ MM4, MM1

																	// 取出SA和DA
																	PSRAD MM2, 24	// 得到000A值
																	PAND  MM2, nMask
																	MOVQ  MM0, nMask
																	PSUBB MM0, MM2  // 255-SA
																	PSRAD MM1, 24
																	PAND MM1, nMask

																	// 保存255-SA
																	MOVQ  MM6, MM0
																	MOVQ  MM7, MM0	

																	// SA+DA
																	PADDW MM0, MM1
																	//PAND  MM0, nMask

																	// (SA*DA+FF)>>8
																	PMULLW MM6, MM1
																	PADDW  MM6, nMask
																	PSRLW  MM6, 8
																	//PAND   MM6, nMask

																	// SA+DA-(SA*DA+0xFF)>>8
																	PSUBW  MM0, MM6
																	PAND   MM0, nMask
																	// 求出目标RGBA
																	// 打包Src和Dest的RGBA值
																	PXOR    MM1, MM1
																	PUNPCKLBW MM3, MM1
																	MOVQ2DQ    XMM0, MM3
																	PXOR    XMM1, XMM1
																	PUNPCKLWD XMM0, XMM1
																	// 转换DWORD成Float,放在XMM1
																	CVTDQ2PS  XMM1, XMM0
																	// 取出Dest的RGBA，打包成000A000R000G000B放入XMM0
																	PUNPCKLBW MM4, MM1
																	MOVQ2DQ    XMM0, MM4
																	PXOR    XMM2, XMM2
																	PUNPCKLWD XMM0, XMM2
																	// 转换DWORD成Float,放在XMM2
																	CVTDQ2PS  XMM2, XMM0

																	// 保存指针
																	PUSH EDX
																	PUSH EAX
																	PUSH ECX
																	// 查SA,KA值对应的索引表SA/KA放入XMM4（4个，成000R000R000R000R）
																	SUBPS  XMM1, XMM2		// S-D
																	// 取出SA,算出(255-SA)*256
																	MOV    EDX,  0x000000FF
																	MOVD   EAX,  MM7
																	SUB    EDX,  EAX
																	MOV    EAX,  EDX
																	SHL    EAX,  8
																	AND    EAX,  0x0000FF00  // 变成A000
																	// 取出KA
																	MOVD   ECX, MM0
																	MOV    EDX, ECX
																	ADD    ECX, EAX
																	SHL    ECX, 2
																	ADD	   ECX, g_pfTable    
																	MOVSS  XMM4, [ECX]
																	PUNPCKLDQ XMM4, XMM4
																		PUNPCKLQDQ XMM4, XMM4
																		// (S-D)*R
																		MULPS XMM1, XMM4
																		POP ECX
																		POP EAX
																		ADD EAX, 4
																		// (S-D)*R+D
																		ADDPS XMM1, XMM2
																		INC ECX
																		// 转换成整数(000A000R000G000B)
																		CVTPS2DQ XMM2, XMM1
																		// 插入正确的A值
																		PINSRW XMM2, EDX, 6 
																		POP EDX
																		// 压缩RGBA
																		PACKUSWB XMM2, XMM2
																		MOVDQ2Q   MM2, XMM2
																		PACKUSWB MM2, MM2
																		MOVD  [EDX], MM2	// 存入Dest一个点的ARGB

																		ADD EDX, 4
																		JMP LOOPJ41
LOOPJEND:
																	MOV EAX, nAddDestWidth		// nDestBufferWidth需乘4
																		ADD [pDes], EAX

																		MOV EAX, nAddSrcWidth
																		ADD [pSrc], EAX 
																		POP ECX
																		ADD ECX, nInc
																		JMP LOOPI
EXITLOOPI:
																	EMMS			// 清空
	}
#endif
#if 0
	for(int i=0; i<nDestHeight; i++)
	{
		if(nDestY+i >= nDestBufferHeight)
			break;
		if(nSrcY+i >= nSrcBufferHeight)
			break;
		for(int j=0; j<nDestWidth; j++)
		{
			int nTemp = nSrcX+j;
			if(nDestX+j >= nDestBufferWidth)
				break;
			if(nTemp >= nSrcBufferWidth)
				break;
			TPRGBA	rgbSrc, rgbDest, rgbCur;
			rgbSrc.m_dwColor = pSrc[j];
			rgbDest.m_dwColor = pDes[j];
			float fMix = (255-rgbSrc.m_bColor.bA)/(float)255;
			rgbCur.m_bColor.bR = (BYTE)(rgbSrc.m_bColor.bR*fMix + (1-fMix)*rgbDest.m_bColor.bR);
			rgbCur.m_bColor.bG = (BYTE)(rgbSrc.m_bColor.bG*fMix + (1-fMix)*rgbDest.m_bColor.bG);
			rgbCur.m_bColor.bB = (BYTE)(rgbSrc.m_bColor.bB*fMix + (1-fMix)*rgbDest.m_bColor.bB);
			rgbCur.m_bColor.bA = (BYTE)(rgbSrc.m_bColor.bA*fMix + (1-fMix)*rgbDest.m_bColor.bA);

			pDes[j] = rgbCur.m_dwColor;
		}
		pDes += nDestBufferWidth;
		pSrc += nSrcBufferWidth;
	}
#endif
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// 功能：进行拉伸拷贝
// 
// 2003-10-10,V1.0,编写者：陈玉峰
//
// 2003-11-20,V2.0,修改者：陈玉峰
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_StretchCopy(DWORD* pdwDestBuffer, 
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
						  int nParity)
{
	// 数据范围检测
	if(nSrcX>= nSrcBufferWidth || nSrcY>=nSrcBufferHeight)
		return TRUE;
	if(nDestX>=nDestBufferWidth || nDestY>=nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcWidth<=0 || nSrcHeight<=0 ||nDestWidth <=0 ||nDestHeight<=0)
		return TRUE;

	// 源缓冲区与目标缓冲区成比例缩放:缩放比例
	float fWidth = ((float)(nSrcWidth))/nDestWidth;
	float fHeight = ((float)(nSrcHeight))/nDestHeight;

	// 计算目标和源起点，目标和源的宽度与高度
	if(nDestX < 0)
	{
		nSrcX = (int)(nSrcX-nDestX*fWidth);
		nSrcWidth = (int)(nSrcWidth+nDestX*fWidth);
		nDestWidth += nDestX;
		nDestX = 0;
	}
	if(nDestY< 0)
	{
		nSrcY = (int)(nSrcY-nDestY*fHeight);
		nSrcHeight = (int)(nSrcHeight+nDestY*fHeight);
		nDestHeight += nDestY;
		nDestY = 0;
	}
	if(nSrcX < 0)
	{
		nDestX = (int)(nDestX-nSrcX*((float)(nDestWidth)/nSrcWidth));
		nDestWidth = (int)(nDestWidth-nSrcX*((float)(nDestWidth)/nSrcWidth));
		nSrcWidth += nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nDestY = (int)(nDestY-nSrcY*((float)(nDestHeight)/nSrcHeight));
		nDestHeight = (int)(nDestHeight+nSrcY*((float)(nDestHeight)/nSrcHeight));
		nSrcHeight += nSrcY;
		nSrcY = 0;
	}
	int nTempWidth = nDestX+nDestWidth;
	int nTempHeight = nDestY+nDestHeight;
	if(nTempWidth > nDestBufferWidth)
	{
		nDestWidth -= (nTempWidth-nDestBufferWidth);
		nSrcWidth  -= (int)((nTempWidth-nDestBufferWidth)*fWidth);
	}
	if(nTempHeight > nDestBufferHeight)
	{
		nDestHeight -= (nTempHeight-nDestBufferHeight);
		nSrcHeight  -= (int)((nTempHeight-nDestBufferHeight)*fHeight);
	}
	nTempWidth = nSrcX + nSrcWidth;
	nTempHeight = nSrcY + nSrcHeight;
	if(nTempWidth > nSrcBufferWidth)
	{
		nSrcWidth -= (nTempWidth-nSrcBufferWidth);
		nDestWidth -= (int)((nTempWidth-nSrcBufferWidth)*((float)(nDestWidth)/nSrcWidth));
	}
	if(nTempHeight > nSrcBufferHeight)
	{
		nSrcHeight -= (nTempHeight-nSrcBufferHeight);
		nDestHeight -= (int)((nTempHeight-nSrcBufferHeight)*((float)(nDestHeight)/nSrcHeight));
	}
	if(nSrcWidth<=0 || nSrcHeight<=0 ||nDestWidth <=0 ||nDestHeight<=0)
		return TRUE;

	// 指针起点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	DWORD* pTemp = pSrc;
	if(pSrc == NULL || pDes == NULL)
		return TRUE;

	// 将提升的宽度和高度先建表
	DWORD* pnAddHeight = NULL; 
	DWORD* pnAddWidth = NULL;
	TPBuffer_CreateTable(pnAddHeight, nDestHeight, fHeight);
	TPBuffer_CreateTable(pnAddWidth, nDestWidth, fWidth);
	DWORD* pnAddHeight1 = pnAddHeight;

	//////////////////////////////////////////////////////////////////////////
	// 增加了奇偶场信息，2004－02－11，chyf
	// 每次加的宽度
	int nAddDestWidth = nDestBufferWidth<<2;
	int nAddSrcWidth = nSrcBufferWidth<<2;
	int nInc = 0;		// 计数器的增量
	int nTableInc = 0;	// 表指针增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nTableInc = 4;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;
		nTableInc = 8;
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
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pTemp += pnAddHeight[0]*nSrcBufferWidth;
			pnAddHeight1 ++;
		}
	}
	else
	{
		// 起点为偶场
		if(nParity == TP_PARITY_ODD)
		{
			// 如果奇场拷贝，则从下一行开始
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pTemp += pnAddHeight[0]*nSrcBufferWidth;
			pnAddHeight1 ++;
		}
	}
	// 增加奇偶场信息
	//////////////////////////////////////////////////////////////////////////

#if 1
	// 一行行赋值,SSE2优化
	_asm
	{
		XOR ECX, ECX	// i清0
			MOV EAX, pnAddHeight1	// 要加的高度
LOOP1:
		CMP ECX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1		 // 退出第一层循环(i>=nDestHeight)

			MOVD MM0, [EAX]
			MOVD MM1, nAddSrcWidth
				PMULUDQ MM0, MM1
				MOVD EDX, MM0
				ADD EDX, pSrc
				MOV [pTemp], EDX
				ADD EAX, nTableInc

				// i和要加的高度指针压入栈
				PUSH ECX
				PUSH EAX

				XOR ECX, ECX	     // 清0， 第二层循环计数器
				MOV EDX, pnAddWidth  // 要加的宽度
				MOV EAX, pDes		 // 目标指针
LOOP2:
			ADD ECX, 4			 // 每次计算4个值，不足4时跳到循环体2中
				CMP ECX, nDestWidth
				JG LOOP2BODY2		

				// j值压栈
				PUSH ECX

				// 读取pTemp[pnAddWidth[j]]的值放入MM0
				MOV ECX, [EDX]
				SHL ECX, 2		// pnAddWidth[j]*4
					ADD ECX, pTemp 
					MOVD MM0, [ECX]			
					ADD EDX, 4

						// 读取pTemp[pnAddWidth[j+1]]的值
						MOV ECX, [EDX]
						SHL ECX, 2
							ADD ECX, pTemp		
							MOVD MM1, [ECX]
							ADD EDX, 4						

								// 两个值打包
								PUNPCKLDQ MM0, MM1

								// 读取pTemp[pnAddWidth[j+2]]的值放入MM0
								MOV ECX, [EDX]
								SHL ECX, 2
									ADD ECX, pTemp 
									MOVD MM3, [ECX]			
									ADD EDX, 4

										// 读取pTemp[pnAddWidth[j+3]]的值
										MOV ECX, [EDX]
										SHL ECX, 2
											ADD ECX, pTemp 		
											MOVD MM4, [ECX]
											ADD EDX, 4		

												PUNPCKLDQ MM3, MM4
												MOVQ2DQ	XMM0, MM0
												MOVQ2DQ	XMM1, MM3
												PUNPCKLQDQ XMM0, XMM1

												// 存入pDes[j]的值
												POP ECX
												MOVUPS  [EAX], XMM0
												ADD EAX, 10H

												JMP LOOP2
LOOP2BODY2:
											SUB ECX, 4			// 不足4时
LOOP2BODY3:
											CMP ECX, nDestWidth
												JGE LOOP2END

												// 读取pTemp[pnAddWidth[j]]的值
												// j压栈
												PUSH ECX
												MOV ECX, [EDX]
												SHL ECX, 2
													ADD ECX, pTemp 
													MOVD MM0, [ECX]
													ADD EDX, 4

														// 读取pDes[j]的值
														MOVD  [EAX], MM0
														ADD   EAX, 4

														POP ECX
														INC ECX
														JMP LOOP2BODY3
LOOP2END:
													// pDes += nAddDestWidth
													MOV EDX, pDes
														ADD EDX, nAddDestWidth
														MOV [pDes], EDX

														// pTemp = pSrc+pnAddHeight[i]*nAddSrcWidth
														POP EAX

														POP ECX
														ADD	ECX, nInc			 // 计数器自增，返回循环1 
														JMP LOOP1
EXITLOOP1:
													EMMS			// 清空
	}
#endif
#if 0
	for(int i=0; i<nDestHeight; i++)
	{
		pTemp = pSrc+pnAddHeight[i]*nSrcBufferWidth; 
		for(int j=0; j<nDestWidth; j++)
		{
			pDes[j] = pTemp[pnAddWidth[j]];	
		}
		pDes += nDestBufferWidth;

	}
#endif
	TPBuffer_FreeTable(pnAddHeight);
	TPBuffer_FreeTable(pnAddWidth);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 功能：进行拉伸Mix拷贝
// 2003－11：V1.0,陈玉峰
// 算法：
// 2004－02－11：V2.0,陈玉峰
// 算法：
//
//////////////////////////////////////////////////////////////////////////
typedef union _tag_TPRGBA
{
	struct{
		BYTE	bR;
		BYTE	bG;
		BYTE	bB;
		BYTE	bA;
	}m_bColor;
	DWORD	m_dwColor;
} TPRGBA;
BOOL TPBuffer_StretchMix(DWORD* pdwDestBuffer, 
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
						 int nParity)				// 奇偶场
{
	// 源缓冲区与目标缓冲区成比例缩放
	if(nSrcX>= nSrcBufferWidth || nSrcY>=nSrcBufferHeight)
		return TRUE;
	if(nSrcX < 0)
		nSrcX = 0;
	if(nSrcY < 0)
		nSrcY = 0;
	if(nDestX>=nDestBufferWidth || nDestY>=nDestBufferHeight || nDestWidth<=0 || nDestHeight<=0)
		return TRUE;
	// 缩放比例
	float fWidth = ((float)(nSrcWidth))/nDestWidth;
	float fHeight = ((float)(nSrcHeight))/nDestHeight;

	if(nDestX < 0)
	{
		nSrcX = (int)(nSrcX-nDestX*fWidth);
		nDestWidth = nDestWidth+nDestX;
		nDestX = 0;
	}
	if(nDestY< 0)
	{
		nSrcY = (int)(nSrcY-nDestY*fHeight);
		nDestHeight = nDestHeight+nDestY;
		nDestY = 0;
	}
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	if(pDes == NULL)
		return TRUE;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	DWORD* pTemp = pSrc;
	if(pSrc == NULL)
		return TRUE;

	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,
	};

	// 将提升的宽度和高度先建表
	DWORD* pnAddHeight = NULL; 
	DWORD* pnAddWidth = NULL;
	TPBuffer_CreateTable(pnAddHeight, nDestHeight, fHeight);
	TPBuffer_CreateTable(pnAddWidth, nDestWidth, fWidth);

	DWORD* pnAddHeight1 = pnAddHeight;

	//////////////////////////////////////////////////////////////////////////
	// 增加了奇偶场信息，2004－02－11，chyf
	// 每次加的宽度
	int nAddDestWidth = nDestBufferWidth<<2;
	int nAddSrcWidth = nSrcBufferWidth<<2;
	int nInc = 0;		// 计数器的增量
	int nTableInc = 0;	// 表指针增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nTableInc = 4;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;
		nTableInc = 8;
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
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pTemp += pnAddHeight[0]*nSrcBufferWidth;
			pnAddHeight1 ++;
		}
	}
	else
	{
		// 起点为偶场
		if(nParity == TP_PARITY_ODD)
		{
			// 如果奇场拷贝，则从下一行开始
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pTemp += pnAddHeight[0]*nSrcBufferWidth;
			pnAddHeight1 ++;
		}
	}
	// 增加奇偶场信息
	//////////////////////////////////////////////////////////////////////////

#if 1
	// 一行行赋值,SSE2优化
	_asm
	{
		XOR EAX, EAX	// 清0
			PXOR		MM6,MM6
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
			MOV ECX, pnAddHeight
LOOP1:
		CMP EAX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1		 // 退出第一层循环(i>=nDestHeight)

			MOV	EDX, nSrcY	 // 计算nSrcY+pnAddHeight[i]
			ADD EDX, [ECX]
			CMP EDX, nSrcBufferHeight
				JGE	EXITLOOP1	 // 退出第一层循环(nSrcY+nAddHeight>=nSrcBufferHeight)

				PUSH EAX
				PUSH ECX

				MOV EDX, [ECX]
				IMUL EDX,nAddSrcWidth
					MOV ECX, pSrc
					ADD ECX, EDX
					MOV [pTemp], ECX


					MOV	ECX, nDestY	 // 计算nDestY+i
					ADD ECX, EAX
					CMP ECX, nDestBufferHeight
					JGE	EXITLOOP1	 // 退出第一层循环(nDestY+i>=nDestBufferHeight)

					XOR ECX, ECX	 // 清0， 第二层循环计数器
					MOV EDX, pnAddWidth
LOOP2:
				CMP ECX, nDestWidth  // 第二层循环(j<nDestWidth)
					JGE LOOP2END	 // 进入第二层循环体(j<nDestWidth)

					ADD ECX, 2			// 每次计算2个值，不足2时跳到循环体2中
					CMP ECX, nDestWidth
					JG LOOP2BODY2		

					SUB ECX, 2

					// 当pnAddWidth[j]>=nSrcBufferWidth时跳到第一个循环
					MOV EAX, nSrcX		// 计算nSrcX+pnAddWidth[j]
					ADD EAX, [EDX]
					CMP EAX, nSrcBufferWidth
						JGE LOOP2BODY2		// 退出第二层循环(nSrcX+pnAddWidth[j]>=nSrcBufferWidth)

						// 当pnAddWidth[j+1]>=nSrcBufferWidth时跳到第一个循环
						MOV EAX, nSrcX		// 计算nSrcX+pnAddWidth[j+1]
						ADD EAX, [EDX+4]
						CMP EAX, nSrcBufferWidth
							JGE LOOP2BODY2		// 退出第二层循环(nSrcX+pnAddWidth[j+1]>=nSrcBufferWidth)

							MOV EAX, nDestX	// 计算nDestX+j
							ADD EAX, ECX
							CMP EAX, nDestBufferWidth
							JGE LOOP2BODY2	// 退出第二层循环(nDestX+j>=nDestBufferWidth)	

							MOV EAX, pTemp		// 读取pTemp[pnAddWidth[j]]的值放入MM0
							PUSH ECX
							MOV ECX, [EDX]
							SHL ECX, 2
								ADD EAX, ECX 
								MOVD MM0, [EAX]			
								MOV ECX, [EAX]					// 读取其Alpha值
								SHR	ECX,  24					// 右移24位
									AND ECX, 0x000000FF
									MOVD MM3, ECX
									PUNPCKLWD MM3, MM3
									PUNPCKLDQ MM3, MM3				// 变为0A0A0A0A

									MOV EAX, 0xFF					// 255-Alpha
									SUB EAX, ECX
									MOVD MM7, EAX
									PUNPCKLWD MM7, MM7
									PUNPCKLDQ MM7, MM7				// 变为0A0A0A0A

									POP ECX
									ADD EDX, 4

									MOV EAX, pTemp		// 读取pTemp[pnAddWidth[j+1]]的值
									PUSH ECX
									MOV ECX, [EDX]
									SHL ECX, 2
										ADD EAX, ECX 		
										MOVD MM1, [EAX]
										MOV ECX, [EAX]					// 读取其Alpha值
										SHR	ECX,  24					// 右移24位
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A

											POP ECX
											ADD EDX, 4

											XOR EAX, EAX	// 清0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
											PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
											MOVQ2DQ XMM0, MM0
											MOVQ2DQ XMM1, MM1
											PUNPCKLQDQ XMM0, XMM1 // 变为0R0G0B0A0R0G0B0A

											MOV EAX, pDes	 // 读取pDes[j]的值
											PUSH ECX
											SHL ECX, 2
											ADD EAX, ECX
											POP ECX
											PUSH  EAX		 // 保留指针，后面要用

											MOVD MM0, [EAX]
											ADD EAX, 4		// 读取pDes[j+1]的值
												MOVD MM1, [EAX]
												XOR EAX, EAX	// 清0
													MOVD MM2, EAX
													PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
													PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
													MOVQ2DQ XMM1, MM0
													MOVQ2DQ XMM2, MM1
													PUNPCKLQDQ XMM1, XMM2 // 变为0R0G0B0A0R0G0B0A
													MOVDQA	XMM3, XMM1	 // 复制一份

													MOVQ2DQ XMM2, MM3		// 取源的alpha值
													MOVQ2DQ XMM4, MM4
													PUNPCKLQDQ XMM2, XMM4	// 变为0A0A0A0A0M0M0M0M

													MOVQ2DQ XMM4, MM7		// 取255-alpha值
													MOVQ2DQ XMM5, MM6
													PUNPCKLQDQ XMM4, XMM5	// 变为0A0A0A0A0M0M0M0M

													PMULLW XMM0, XMM2	// 对应16位相乘SA
													// 误差补偿
													MOVQ2DQ		XMM7,MM5
													MOVLHPS		XMM7,XMM7
													PADDSW		XMM0,XMM7
													PSRLW  XMM0, 8		// 每16位右移8位（相当于/255)(要进行误差补偿)

													PMULLW XMM1, XMM4	// 对应16位相乘D(1-A)
													// 误差补偿
													PADDSW XMM1, XMM7
													PSRLW  XMM1, 8

													PADDSW XMM0, XMM1	// 对应16位相加(SA+DA)

													PACKUSWB XMM0, XMM0 // 压缩，每16位压缩为8位(RGBARGBA)

													MOVDQ2Q	MM0,XMM0	// 取低64位(两个DWORD)
													POP EAX					// 赋给pDest
													MOVQ  [EAX], MM0

													ADD ECX, 2			// j增2，返回第二层循环
													JMP LOOP2
LOOP2BODY2:
												SUB ECX, 2			// 不足2时
													CMP ECX, nDestWidth
													JGE LOOP2END

													MOV EAX, nSrcX	// 计算nSrcX+pnAddWidth[j]
													ADD EAX, [EDX]
													CMP EAX, nSrcBufferWidth
														JGE LOOP2END	// 退出第二层循环(nSrcX+pnAddWidth[j]>=nSrcBufferWidth)

														MOV EAX, nDestX	// 计算nDestX+j
														ADD EAX, ECX
														CMP EAX, nDestBufferWidth
														JGE LOOP2END	// 退出第二层循环(nDestX+j>=nDestBufferWidth)		

														MOV EAX, pTemp		// 读取pTemp[pnAddWidth[j]]的值
														PUSH ECX
														MOV ECX, [EDX]
														SHL ECX, 2
															ADD EAX, ECX 
															MOVD MM0, [EAX]

															MOV ECX, [EAX]					// 读取其Alpha值
															SHR	ECX,  24					// 右移24位
																AND ECX, 0x000000FF
																MOVD MM4, ECX
																PUNPCKLWD MM4, MM4
																PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

																MOV EAX, 0xFF					// 255-Alpha
																SUB EAX, ECX
																MOVD MM6, EAX
																PUNPCKLWD MM6, MM6
																PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A

																POP ECX
																XOR EAX, EAX	// 清0
																MOVD MM2, EAX
																PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A


																MOV EAX, pDes	 // 读取pDes[j]的值
																PUSH ECX
																SHL ECX, 2
																ADD EAX, ECX
																POP ECX
																PUSH  EAX		 // 保留指针，后面要用
																MOVD MM1, [EAX]
																XOR EAX, EAX	// 清0
																	MOVD MM2, EAX
																	PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
																	MOVQ	MM3, MM1	 // 复制一份

																	PMULLW MM0, MM4	// 对应16位相乘SA
																	// 误差补偿
																	PADDSW MM0, MM5
																	PSRLW  MM0, 8		// 右移8位（相当于/255)(要进行误差补偿)

																	PMULLW MM1, MM6	// 对应16位相乘D(1-A)
																	PADDSW MM1, MM5
																	PSRLW  MM1, 8
																	PADDSW MM0, MM1	// 对应16位相加
																	PACKUSWB MM0, MM0 // 压缩，每16位压缩为8位(RGBARGBA)

																	POP EAX					// 赋给pDest
																	MOVD  [EAX], MM0
LOOP2END:
																MOV EAX, pDes
																	ADD EAX, nAddDestWidth		// nDestBufferWidth需乘4
																	MOV [pDes], EAX

																	// pSrc+pnAddHeight[i]*nAddSrcWidth*4
																	POP ECX
																	ADD ECX, nTableInc

																	POP EAX
																	ADD	EAX, nInc			 // 计数器自增，返回循环1 
																	JMP LOOP1
EXITLOOP1:
																EMMS			// 清空
	}
#endif
#if 0
	for(int i=0; i<nDestHeight; i++)
	{

		// 源缓冲区提升的高度
		if(nDestY+i >= nDestBufferHeight)
			break;
		if(nSrcY+pnAddHeight[i] >= nSrcBufferHeight)
			break;
		pTemp = pSrc+pnAddHeight[i]*nSrcBufferWidth; 
		for(int j=0; j<nDestWidth; j++)
		{
			// 源缓冲区提升的宽度
			if(nDestX+j >= nDestBufferWidth)
				break;
			if(nSrcX+pnAddWidth[j] >= nSrcBufferWidth)
				break;
			TPRGBA	rgbSrc, rgbDest, rgbCur;
			rgbSrc.m_dwColor = pTemp[pnAddWidth[j]];
			rgbDest.m_dwColor = pDes[j];
			float fMix = rgbSrc.m_bColor.bA/(float)255;
			rgbCur.m_bColor.bR = (BYTE)(rgbSrc.m_bColor.bR*fMix + (1-fMix)*rgbDest.m_bColor.bR);
			rgbCur.m_bColor.bG = (BYTE)(rgbSrc.m_bColor.bG*fMix + (1-fMix)*rgbDest.m_bColor.bG);
			rgbCur.m_bColor.bB = (BYTE)(rgbSrc.m_bColor.bB*fMix + (1-fMix)*rgbDest.m_bColor.bB);
			rgbCur.m_bColor.bA = (BYTE)(rgbSrc.m_bColor.bA*fMix + (1-fMix)*rgbDest.m_bColor.bA);
			pDes[j] = rgbCur.m_dwColor;	
		}
		pDes += nDestBufferWidth;

	}
#endif
	TPBuffer_FreeTable(pnAddHeight);
	TPBuffer_FreeTable(pnAddWidth);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// 进行拉伸NotMix拷贝
BOOL TPBuffer_StretchNotMix(DWORD* pdwDestBuffer, 
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
							int nParity)
{
	// 源缓冲区与目标缓冲区成比例缩放
	if(nSrcX>= nSrcBufferWidth || nSrcY>=nSrcBufferHeight)
		return TRUE;
	if(nSrcX < 0)
		nSrcX = 0;
	if(nSrcY < 0)
		nSrcY = 0;
	if(nDestX>=nDestBufferWidth || nDestY>=nDestBufferHeight || nDestWidth<=0 || nDestHeight<=0)
		return TRUE;
	// 缩放比例
	float fWidth = ((float)(nSrcWidth))/nDestWidth;
	float fHeight = ((float)(nSrcHeight))/nDestHeight;

	if(nDestX < 0)
	{
		nSrcX = (int)(nSrcX-nDestX*fWidth);
		nDestWidth = nDestWidth+nDestX;
		nDestX = 0;
	}
	if(nDestY< 0)
	{
		nSrcY = (int)(nSrcY-nDestY*fHeight);
		nDestHeight = nDestHeight+nDestY;
		nDestY = 0;
	}
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	if(pDes == NULL)
		return TRUE;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	DWORD* pTemp = pSrc;
	if(pSrc == NULL)
		return TRUE;

	int nTemp = 0;

	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,
	};


	// 将提升的宽度和高度先建表
	DWORD* pnAddHeight = NULL; 
	DWORD* pnAddWidth = NULL;
	TPBuffer_CreateTable(pnAddHeight, nDestHeight, fHeight);
	TPBuffer_CreateTable(pnAddWidth, nDestWidth, fWidth);

	DWORD* pnAddHeight1 = pnAddHeight;

	//////////////////////////////////////////////////////////////////////////
	// 增加了奇偶场信息，2004－02－11，chyf
	// 每次加的宽度
	int nAddDestWidth = nDestBufferWidth<<2;
	int nAddSrcWidth = nSrcBufferWidth<<2;
	int nInc = 0;		// 计数器的增量
	int nTableInc = 0;	// 表指针增量
	switch(nParity) {
	case TP_PARITY_BOTH:
		nTableInc = 4;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;
		nTableInc = 8;
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
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pTemp += pnAddHeight[0]*nSrcBufferWidth;
			pnAddHeight1 ++;
		}
	}
	else
	{
		// 起点为偶场
		if(nParity == TP_PARITY_ODD)
		{
			// 如果奇场拷贝，则从下一行开始
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pTemp += pnAddHeight[0]*nSrcBufferWidth;
			pnAddHeight1 ++;
		}
	}
	// 增加奇偶场信息
	//////////////////////////////////////////////////////////////////////////

#if 1
	// 一行行赋值,SSE2优化
	_asm
	{
		XOR EAX, EAX	// 清0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
			MOV ECX, pnAddHeight
LOOP1:
		CMP EAX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1		 // 退出第一层循环(i>=nDestHeight)

			MOV	EDX, nSrcY	 // 计算nSrcY+pnAddHeight[i]
			ADD EDX, [ECX]
			CMP EDX, nSrcBufferHeight
				JGE	EXITLOOP1	 // 退出第一层循环(nSrcY+nAddHeight>=nSrcBufferHeight)
				PUSH EAX
				PUSH ECX

				MOV	ECX, nDestY	 // 计算nDestY+i
				ADD ECX, EAX
				CMP ECX, nDestBufferHeight
				JGE	EXITLOOP1	 // 退出第一层循环(nDestY+i>=nDestBufferHeight)

				XOR ECX, ECX	 // 清0， 第二层循环计数器
				MOV EDX, pnAddWidth
LOOP2:
			CMP ECX, nDestWidth  // 第二层循环(j<nDestWidth)
				JGE LOOP2END	 // 进入第二层循环体(j<nDestWidth)	

				ADD ECX, 2			// 每次计算2个值，不足2时跳到循环体2中
				CMP ECX, nDestWidth
				JG LOOP2BODY2		

				SUB ECX, 2

				// 当pnAddWidth[j]>=nSrcBufferWidth时跳到第一个循环
				MOV EAX, nSrcX		// 计算nSrcX+pnAddWidth[j]
				ADD EAX, [EDX]
				CMP EAX, nSrcBufferWidth
					JGE LOOP2BODY2		// 退出第二层循环(nSrcX+pnAddWidth[j]>=nSrcBufferWidth)

					// 当pnAddWidth[j+1]>=nSrcBufferWidth时跳到第一个循环
					MOV EAX, nSrcX		// 计算nSrcX+pnAddWidth[j+1]
					ADD EAX, [EDX+4]
					CMP EAX, nSrcBufferWidth
						JGE LOOP2BODY2		// 退出第二层循环(nSrcX+pnAddWidth[j+1]>=nSrcBufferWidth)

						MOV EAX, nDestX	// 计算nDestX+j
						ADD EAX, ECX
						CMP EAX, nDestBufferWidth
						JGE LOOP2BODY2	// 退出第二层循环(nDestX+j>=nDestBufferWidth)	

						MOV EAX, pTemp		// 读取pTemp[pnAddWidth[j]]的值放入MM0
						PUSH ECX
						MOV ECX, [EDX]
						SHL ECX, 2
							ADD EAX, ECX 
							MOVD MM0, [EAX]			
							MOV ECX, [EAX]					// 读取其Alpha值
							SHR	ECX,  24					// 右移24位
								AND ECX, 0x000000FF
								MOVD MM3, ECX
								PUNPCKLWD MM3, MM3
								PUNPCKLDQ MM3, MM3				// 变为0A0A0A0A

								MOV EAX, 0xFF					// 255-Alpha
								SUB EAX, ECX
								MOVD MM7, EAX
								PUNPCKLWD MM7, MM7
								PUNPCKLDQ MM7, MM7				// 变为0A0A0A0A
								POP ECX

								ADD EDX, 4

								MOV EAX, pTemp		// 读取pTemp[pnAddWidth[j+1]]的值
								PUSH ECX
								MOV ECX, [EDX]
								SHL ECX, 2
									ADD EAX, ECX 		
									MOVD MM1, [EAX]
									MOV ECX, [EAX]					// 读取其Alpha值
									SHR	ECX,  24					// 右移24位
										AND ECX, 0x000000FF
										MOVD MM4, ECX
										PUNPCKLWD MM4, MM4
										PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

										MOV EAX, 0xFF					// 255-Alpha
										SUB EAX, ECX
										MOVD MM6, EAX
										PUNPCKLWD MM6, MM6
										PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A

										POP ECX
										ADD EDX, 4

										XOR EAX, EAX	// 清0
										MOVD MM2, EAX
										PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
										PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
										MOVQ2DQ XMM0, MM0
										MOVQ2DQ XMM1, MM1
										PUNPCKLQDQ XMM0, XMM1 // 变为0R0G0B0A0R0G0B0A

										MOV EAX, pDes	 // 读取pDes[j]的值
										PUSH ECX
										SHL ECX, 2
										ADD EAX, ECX
										POP ECX
										PUSH  EAX		 // 保留指针，后面要用

										MOVD MM0, [EAX]
										ADD EAX, 4		// 读取pDes[j+1]的值
											MOVD MM1, [EAX]
											XOR EAX, EAX	// 清0
												MOVD MM2, EAX
												PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
												PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
												MOVQ2DQ XMM1, MM0
												MOVQ2DQ XMM2, MM1
												PUNPCKLQDQ XMM1, XMM2 // 变为0R0G0B0A0R0G0B0A
												MOVDQA	XMM3, XMM1	 // 复制一份

												MOVQ2DQ XMM2, MM3		// 取源的alpha值
												MOVQ2DQ XMM4, MM4
												PUNPCKLQDQ XMM2, XMM4	// 变为0A0A0A0A0M0M0M0M

												MOVQ2DQ XMM4, MM7		// 取255-alpha值
												MOVQ2DQ XMM5, MM6
												PUNPCKLQDQ XMM4, XMM5	// 变为0A0A0A0A0M0M0M0M

												PMULLW XMM0, XMM4	// 对应16位相乘S(1-A)
												// 误差补偿
												MOVQ2DQ		XMM7,MM5
												MOVLHPS		XMM7,XMM7
												PADDSW		XMM0,XMM7
												PSRLW  XMM0, 8		// 每16位右移8位（相当于/255)(要进行误差补偿)

												PMULLW XMM1, XMM2	// 对应16位相乘DA
												PADDSW XMM1,XMM7
												PSRLW  XMM1, 8

												PADDSW XMM0, XMM1	// 对应16位相加(SA+DA)

												PACKUSWB XMM0, XMM0 // 压缩，每16位压缩为8位(RGBARGBA)

												MOVDQ2Q	MM0,XMM0	// 取低64位(两个DWORD)
												POP EAX					// 赋给pDest
												MOVQ  [EAX], MM0

												ADD ECX, 2			// j增2，返回第二层循环
												JMP LOOP2
LOOP2BODY2:
											SUB ECX, 2			// 不足2时
												CMP ECX, nDestWidth
												JGE LOOP2END

												MOV EAX, nSrcX	// 计算nSrcX+pnAddWidth[j]
												ADD EAX, [EDX]
												CMP EAX, nSrcBufferWidth
													JGE LOOP2END	// 退出第二层循环(nSrcX+pnAddWidth[j]>=nSrcBufferWidth)

													MOV EAX, nDestX	// 计算nDestX+j
													ADD EAX, ECX
													CMP EAX, nDestBufferWidth
													JGE LOOP2END	// 退出第二层循环(nDestX+j>=nDestBufferWidth)	

													MOV EAX, pTemp		// 读取pTemp[pnAddWidth[j]]的值
													PUSH ECX
													MOV ECX, [EDX]
													SHL ECX, 2
														ADD EAX, ECX 
														MOVD MM0, [EAX]

														MOV ECX, [EAX]					// 读取其Alpha值
														SHR	ECX,  24					// 右移24位
															AND ECX, 0x000000FF
															MOVD MM4, ECX
															PUNPCKLWD MM4, MM4
															PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

															MOV EAX, 0xFF					// 255-Alpha
															SUB EAX, ECX
															MOVD MM6, EAX
															PUNPCKLWD MM6, MM6
															PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A

															POP ECX
															XOR EAX, EAX	// 清0
															MOVD MM2, EAX
															PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A

															MOV EAX, pDes	 // 读取pDes[j]的值
															PUSH ECX
															SHL ECX, 2
															ADD EAX, ECX
															POP ECX
															PUSH  EAX		 // 保留指针，后面要用
															MOVD MM1, [EAX]
															XOR EAX, EAX	// 清0
																MOVD MM2, EAX
																PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
																MOVQ	MM3, MM1	 // 复制一份

																PMULLW MM0, MM6	// 对应16位相乘S(1-A)
																PADDSW MM0, MM5
																PSRLW  MM0, 8		// 右移8位（相当于/255)(要进行误差补偿)

																PMULLW MM1, MM4	// 对应16位相乘DA
																PADDSW MM1, MM5 
																PSRLW  MM1, 8
																PADDSW MM0, MM1	// 对应16位相加
																PACKUSWB MM0, MM0 // 压缩，每16位压缩为8位(RGBARGBA)

																POP EAX					// 赋给pDest
																MOVD  [EAX], MM0
LOOP2END:
															MOV EAX, pDes
																ADD EAX, nAddDestWidth		// nDestBufferWidth需乘4
																MOV [pDes], EAX

																// pSrc+pnAddHeight[i]*nAddSrcWidth*4
																POP ECX
																MOV EAX, [ECX]
																MUL nAddSrcWidth
																	MOV EDX, pSrc
																	ADD EDX, EAX
																	MOV [pTemp], EDX
																	ADD ECX, nTableInc

																	POP EAX
																	ADD	EAX, nInc			 // 计数器自增，返回循环1 
																	JMP LOOP1
EXITLOOP1:
																EMMS			// 清空
	}
#endif
#if 0
	for(int i=0; i<nDestHeight; i++)
	{
		// 源缓冲区提升的高度
		if(nDestY+i >= nDestBufferHeight)
			break;
		if(nSrcY+pnAddHeight[i] >= nSrcBufferHeight)
			break;
		for(int j=0; j<nDestWidth; j++)
		{
			// 源缓冲区提升的宽度
			if(nDestX+j >= nDestBufferWidth)
				break;
			if(nSrcX+pnAddWidth[j] >= nSrcBufferWidth)
				break;
			TPRGBA	rgbSrc, rgbDest, rgbCur;
			rgbSrc.m_dwColor = pTemp[pnAddWidth[j]];
			rgbDest.m_dwColor = pDes[j];
			float fMix = (255-rgbSrc.m_bColor.bA)/(float)255;
			rgbCur.m_bColor.bR = (BYTE)(rgbSrc.m_bColor.bR*fMix + (1-fMix)*rgbDest.m_bColor.bR);
			rgbCur.m_bColor.bG = (BYTE)(rgbSrc.m_bColor.bG*fMix + (1-fMix)*rgbDest.m_bColor.bG);
			rgbCur.m_bColor.bB = (BYTE)(rgbSrc.m_bColor.bB*fMix + (1-fMix)*rgbDest.m_bColor.bB);
			rgbCur.m_bColor.bA = (BYTE)(rgbSrc.m_bColor.bA*fMix + (1-fMix)*rgbDest.m_bColor.bA);
			pDes[j] = rgbCur.m_dwColor;	
		}
		pDes += nDestBufferWidth;
		pTemp = pSrc+pnAddHeight[i]*nSrcBufferWidth; 
	}
#endif
	TPBuffer_FreeTable(pnAddHeight);
	TPBuffer_FreeTable(pnAddWidth);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 功能：进行NotMix填充缓冲区，即源Mix取反
// 
// 2003－10-10,V1.0，修改者：陈玉峰
// 改进算法：KR = (255-SA)/255*SR+SA/255*DR
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_QuickNotMix(DWORD* pdwDestBuffer, 
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
						  int nParity)
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
	int nTempWidth = nSrcX+nDestWidth;
	int nTempHeight = nSrcY+nDestHeight;
	if(nTempWidth > nSrcBufferWidth)
		nDestWidth -= (nTempWidth-nSrcBufferWidth);
	if(nTempHeight > nSrcBufferHeight)
		nDestHeight -= (nTempHeight-nSrcBufferHeight);
	nTempWidth = nDestX+nDestWidth;
	nTempHeight = nDestY+nDestHeight;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	nSrcWidth = nDestWidth;
	nSrcHeight = nDestHeight;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;
	// 每次加的宽度
	int nAddDestWidth = nDestBufferWidth<<2;	// 左移2位，相当于乘4
	int nAddSrcWidth = nSrcBufferWidth<<2;

#if 1
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};
	// 一行行赋值,SSE2优化
	_asm
	{
		XOR EAX, EAX	// 清0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1		 // 退出第一层循环(i>=nDestHeight)
			MOV	ECX, nDestY		// 计算nDestY+i
			ADD ECX, EAX
			CMP ECX, nDestBufferHeight
			JGE	EXITLOOP1	 // 退出第一层循环(nDestY+i>=nDestBufferHeight)
			MOV	ECX, nSrcY	 // 计算nSrcY+i
			ADD ECX, EAX
			CMP ECX, nSrcBufferHeight
			JGE	EXITLOOP1	 // 退出第一层循环(nSrcY+i>=nSrcBufferHeight)
			XOR ECX, ECX	 // 清0， 第二层循环计数器
			PUSH EAX
LOOP2:
		CMP ECX, nDestWidth  // 第二层循环(j<nDestWidth)
			JGE LOOP2END		 // 进入第二层循环体(j<nDestWidth)

			ADD ECX, 2			 // 每次计算2个值，不足2时跳到循环体2中
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2

			MOV EDX, nSrcX	// 计算nSrcX+j
			ADD EDX, ECX
			CMP EDX, nSrcBufferWidth
			JGE LOOP2BODY2	// 退出第二层循环(nSrcX+j>=nSrcBufferWidth)

			MOV EDX, nDestX	// 计算nDestX+j
			ADD EDX, ECX
			CMP EDX, nDestBufferWidth
			JGE LOOP2BODY2	// 退出第二层循环(nDestX+j>=nDestBufferWidth)	

			MOV EAX, pSrc	// 读取pSrc[j]的值放入MM0
			MOV EDX, ECX
			SHL EDX, 2
			ADD EAX, EDX 
			MOVD MM0, [EAX]

			PUSH ECX
				MOV ECX, [EAX]					// 读取其Alpha值
				SHR	ECX,  24					// 右移24位
					AND ECX, 0x000000FF
					MOVD MM3, ECX
					PUNPCKLWD MM3, MM3
					PUNPCKLDQ MM3, MM3				// 变为0A0A0A0A

					MOV EDX, 0xFF					// 255-Alpha
					SUB EDX, ECX
					MOVD MM7, EDX
					PUNPCKLWD MM7, MM7
					PUNPCKLDQ MM7, MM7				// 变为0A0A0A0A
					POP ECX

					ADD EAX, 4		// 读取pSrc[j+1]的值放入MM1
					MOVD MM1, [EAX]

					PUSH ECX
						MOV ECX, [EAX]					// 读取其Alpha值
						SHR	ECX,  24					// 右移24位
							AND ECX, 0x000000FF
							MOVD MM4, ECX
							PUNPCKLWD MM4, MM4
							PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

							MOV EAX, 0xFF					// 255-Alpha
							SUB EAX, ECX
							MOVD MM6, EAX
							PUNPCKLWD MM6, MM6
							PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A
							POP ECX

							XOR EAX, EAX	// 清0
							MOVD MM2, EAX
							PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
							PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
							MOVQ2DQ XMM0, MM0
							MOVQ2DQ XMM1, MM1
							PUNPCKLQDQ XMM0, XMM1 // 变为0R0G0B0A0R0G0B0A

							MOV EAX, pDes	 // 读取pDes[j]的值
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // 保留指针，后面要用
							MOVD MM0, [EAX]
							ADD EAX, 4		// 读取pDes[j+1]的值
								MOVD MM1, [EAX]
								XOR EAX, EAX	// 清0
									MOVD MM2, EAX
									PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
									PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
									MOVQ2DQ XMM1, MM0
									MOVQ2DQ XMM2, MM1
									PUNPCKLQDQ XMM1, XMM2 // 变为0R0G0B0A0R0G0B0A
									MOVDQA	XMM3, XMM1	 // 复制一份

									MOVQ2DQ XMM2, MM3		// 取源的alpha值
									MOVQ2DQ XMM4, MM4
									PUNPCKLQDQ XMM2, XMM4	// 变为0A0A0A0A0M0M0M0M

									MOVQ2DQ XMM4, MM7		// 取255-alpha值
									MOVQ2DQ XMM5, MM6
									PUNPCKLQDQ XMM4, XMM5	// 变为0A0A0A0A0M0M0M0M

									PMULLW XMM0, XMM4	// 对应16位相乘S(1-A)
									// 误差补偿
									MOVQ2DQ		XMM7,MM5
									MOVLHPS		XMM7,XMM7
									PADDSW		XMM0,XMM7
									PSRLW  XMM0, 8		// 每16位右移8位（相当于/255)(要进行误差补偿)

									PMULLW XMM1, XMM2	// 对应16位相乘DA
									// 误差补偿
									PADDSW		XMM1,XMM7
									PSRLW  XMM1, 8

									PADDSW XMM0, XMM1	// 对应16位相加(SA+DA)

									PACKUSWB XMM0, XMM0 // 压缩，每16位压缩为8位(RGBARGBA)

									MOVDQ2Q	MM0,XMM0	// 取低64位(两个DWORD)
									POP EAX					// 赋给pDest
									MOVQ  [EAX], MM0

									ADD ECX, 2			// j增2，返回第二层循环
									JMP LOOP2
LOOP2BODY2:
								SUB ECX, 2			// 不足2时
									CMP ECX, nDestWidth
									JGE LOOP2END

									MOV EDX, nSrcX	// 计算nSrcX+j
									ADD EDX, ECX
									CMP EDX, nSrcBufferWidth
									JGE LOOP2END	// 退出第二层循环(nSrcX+j>=nSrcBufferWidth)

									MOV EDX, nDestX	// 计算nDestX+j
									ADD EDX, ECX
									CMP EDX, nDestBufferWidth
									JGE LOOP2END	// 退出第二层循环(nDestX+j>=nDestBufferWidth)		

									MOV EAX, pSrc		// 读取pSrc[j]的值
									MOV EDX, ECX
									SHL EDX, 2
									ADD EAX, EDX 
									MOVD MM0, [EAX]

									PUSH ECX
										MOV ECX, [EAX]					// 读取其Alpha值
										SHR	ECX,  24					// 右移24位
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A
											POP ECX

											XOR EAX, EAX	// 清0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A

											MOV EAX, pDes	 // 读取pDes[j]的值
											MOV EDX, ECX
											SHL EDX, 2
											ADD EAX, EDX

											PUSH  EAX		 // 保留指针，后面要用
											MOVD MM1, [EAX]
											XOR EAX, EAX	// 清0
												MOVD MM2, EAX
												PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
												MOVQ	MM3, MM1	 // 复制一份

												PMULLW MM0, MM6	// 对应16位相乘S(1-A)
												// 误差补偿
												PADDSW MM0, MM5
												PSRLW  MM0, 8		// 右移8位（相当于/255)(要进行误差补偿)

												PMULLW MM1, MM4	// 对应16位相乘DA
												// 误差补偿
												PADDSW MM1, MM5
												PSRLW  MM1, 8
												PADDSW MM0, MM1	// 对应16位相加
												PACKUSWB MM0, MM0 // 压缩，每16位压缩为8位(RGBARGBA)

												POP EAX					// 赋给pDest
												MOVD  [EAX], MM0
LOOP2END:
											MOV EAX, pDes
												ADD EAX, nAddDestWidth		// nDestBufferWidth需乘4
												MOV [pDes], EAX

												MOV EAX, pSrc
												ADD EAX, nAddSrcWidth
												MOV [pSrc], EAX

												POP EAX
												INC	EAX			 // 计数器自增，返回循环1 
												JMP LOOP1
EXITLOOP1:
											EMMS			// 清空
	}
#endif
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 功能：进行Mix填充缓冲区，这时nSrcWidth==nDestWidth, nSrcHeight==nDestHeight
//
// 1、2003-10-10,V1.0,编写者：陈玉峰
//	  使用算法：DR = SR*SA/255+(1-SA/255)*DR
// 2、2004－02－11，V2.0,编写者：陈玉峰
//    算法：
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_QuickMix(DWORD* pdwDestBuffer, 
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
					   int nParity)
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
	int nTempWidth = nSrcX+nDestWidth;
	int nTempHeight = nSrcY+nDestHeight;
	if(nTempWidth > nSrcBufferWidth)
		nDestWidth -= (nTempWidth-nSrcBufferWidth);
	if(nTempHeight > nSrcBufferHeight)
		nDestHeight -= (nTempHeight-nSrcBufferHeight);
	nTempWidth = nDestX+nDestWidth;
	nTempHeight = nDestY+nDestHeight;
	if(nTempWidth > nDestBufferWidth)
		nDestWidth -= (nTempWidth-nDestBufferWidth);
	if(nTempHeight > nDestBufferHeight)
		nDestHeight -= (nTempHeight-nDestBufferHeight);
	if(nDestWidth <= 0||nDestHeight<= 0)
		return TRUE;

	nSrcWidth = nDestWidth;
	nSrcHeight = nDestHeight;

	// 得到目标缓冲区起始点和源缓冲区起始点
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;
	// 每次加的宽度
	int nAddDestWidth = nDestBufferWidth<<2;	// 左移2位，相当于乘4
	int nAddSrcWidth = nSrcBufferWidth<<2;

#if 1	
	// 误差补偿
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};


	// 一行行赋值,SSE2优化
	_asm
	{
		XOR EAX, EAX	// 清0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // 第一层循环(i<nDestHeight)
			JGE	EXITLOOP1	 // 退出第一层循环(i>=nDestHeight)
			MOV	ECX, nDestY	 // 计算nDestY+i
			ADD ECX, EAX
			CMP ECX, nDestBufferHeight
			JGE	EXITLOOP1	 // 退出第一层循环(nDestY+i>=nDestBufferHeight)
			MOV	ECX, nSrcY	 // 计算nSrcY+i
			ADD ECX, EAX
			CMP ECX, nSrcBufferHeight
			JGE	EXITLOOP1	 // 退出第一层循环(nSrcY+i>=nSrcBufferHeight)
			XOR ECX, ECX	 // 清0， 第二层循环计数器
			PUSH EAX
LOOP2:
		CMP ECX, nDestWidth  // 第二层循环(j<nDestWidth)
			JGE LOOP2END	 // 进入第二层循环体(j<nDestWidth)

			ADD ECX, 2			// 每次计算2个值，不足2时跳到循环体2中
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2

			MOV EDX, nSrcX	// 计算nSrcX+j
			ADD EDX, ECX
			CMP EDX, nSrcBufferWidth
			JGE LOOP2END	// 退出第二层循环(nSrcX+j>=nSrcBufferWidth)

			MOV EDX, nDestX	// 计算nDestX+j
			ADD EDX, ECX
			CMP EDX, nDestBufferWidth
			JGE LOOP2END	// 退出第二层循环(nDestX+j>=nDestBufferWidth)	

			MOV EAX, pSrc	// 读取pSrc[j]的值放入MM0
			MOV EDX, ECX
			SHL EDX, 2
			ADD EAX, EDX 
			MOVD MM0, [EAX]

			PUSH ECX
				MOV ECX, [EAX]					// 读取其Alpha值
				SHR	ECX,  24					// 右移24位
					AND ECX, 0x000000FF
					MOVD MM3, ECX
					PUNPCKLWD MM3, MM3
					PUNPCKLDQ MM3, MM3				// 变为0A0A0A0A

					MOV EDX, 0xFF					// 255-Alpha
					SUB EDX, ECX
					MOVD MM7, EDX
					PUNPCKLWD MM7, MM7
					PUNPCKLDQ MM7, MM7				// 变为0A0A0A0A
					POP ECX

					ADD EAX, 4		// 读取pSrc[j+1]的值放入MM1
					MOVD MM1, [EAX]

					PUSH ECX
						MOV ECX, [EAX]					// 读取其Alpha值
						SHR	ECX,  24					// 右移24位
							AND ECX, 0x000000FF
							MOVD MM4, ECX
							PUNPCKLWD MM4, MM4
							PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

							MOV EAX, 0xFF					// 255-Alpha
							SUB EAX, ECX
							MOVD MM6, EAX
							PUNPCKLWD MM6, MM6
							PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A
							POP ECX

							XOR EAX, EAX	// 清0
							MOVD MM2, EAX
							PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
							PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
							MOVQ2DQ XMM0, MM0
							MOVQ2DQ XMM1, MM1
							PUNPCKLQDQ XMM0, XMM1 // 变为0R0G0B0A0R0G0B0A

							MOV EAX, pDes	 // 读取pDes[j]的值
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // 保留指针，后面要用
							MOVD MM0, [EAX]
							ADD EAX, 4		// 读取pDes[j+1]的值
								MOVD MM1, [EAX]
								XOR EAX, EAX	// 清0
									MOVD MM2, EAX
									PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A
									PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
									MOVQ2DQ XMM1, MM0
									MOVQ2DQ XMM2, MM1
									PUNPCKLQDQ XMM1, XMM2 // 变为0R0G0B0A0R0G0B0A
									MOVDQA	XMM3, XMM1	 // 复制一份

									MOVQ2DQ XMM2, MM3		// 取源的alpha值
									MOVQ2DQ XMM4, MM4
									PUNPCKLQDQ XMM2, XMM4	// 变为0A0A0A0A0M0M0M0M

									MOVQ2DQ XMM4, MM7		// 取255-alpha值
									MOVQ2DQ XMM5, MM6
									PUNPCKLQDQ XMM4, XMM5	// 变为0A0A0A0A0M0M0M0M

									PMULLW XMM0, XMM2	// 对应16位相乘SA
									// 误差补偿
									MOVQ2DQ		XMM7,MM5
									MOVLHPS		XMM7,XMM7
									PADDSW		XMM0,XMM7
									PSRLW  XMM0, 8		// 每16位右移8位（相当于/255)(要进行误差补偿)

									PMULLW XMM1, XMM4	// 对应16位相乘D(1-A)
									// 误差补偿
									PADDSW XMM1,XMM7
									PSRLW  XMM1, 8

									PADDSW XMM0, XMM1	// 对应16位相加(SA+DA)

									PACKUSWB XMM0, XMM0 // 压缩，每16位压缩为8位(RGBARGBA)

									MOVDQ2Q	MM0,XMM0	// 取低64位(两个DWORD)
									POP EAX					// 赋给pDest
									MOVQ  [EAX], MM0

									ADD ECX, 2			// j增2，返回第二层循环
									JMP LOOP2
LOOP2BODY2:
								SUB ECX, 2			// 不足2时
									CMP ECX, nDestWidth
									JGE LOOP2END

									MOV EDX, nSrcX	// 计算nSrcX+j
									ADD EDX, ECX
									CMP EDX, nSrcBufferWidth
									JGE LOOP2END	// 退出第二层循环(nSrcX+j>=nSrcBufferWidth)

									MOV EDX, nDestX	// 计算nDestX+j
									ADD EDX, ECX
									CMP EDX, nDestBufferWidth
									JGE LOOP2END	// 退出第二层循环(nDestX+j>=nDestBufferWidth)		

									MOV EAX, pSrc		// 读取pSrc[j]的值
									MOV EDX, ECX
									SHL EDX, 2
									ADD EAX, EDX 
									MOVD MM0, [EAX]

									PUSH ECX
										MOV ECX, [EAX]					// 读取其Alpha值
										SHR	ECX,  24					// 右移24位
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// 变为0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// 变为0A0A0A0A
											POP ECX

											XOR EAX, EAX	// 清0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// 变MM0为0R0G0B0A

											MOV EAX, pDes	 // 读取pDes[j]的值
											MOV EDX, ECX
											SHL EDX, 2
											ADD EAX, EDX
											PUSH  EAX		 // 保留指针，后面要用
											MOVD MM1, [EAX]
											XOR EAX, EAX	// 清0
												MOVD MM2, EAX
												PUNPCKLBW MM1, MM2  // 变MM1为0R0G0B0A
												MOVQ	MM3, MM1	 // 复制一份

												PMULLW MM0, MM4	// 对应16位相乘SA
												// 误差补偿
												PADDSW MM0, MM5
												PSRLW  MM0, 8		// 右移8位（相当于/255)(要进行误差补偿)

												PMULLW MM1, MM6	// 对应16位相乘D(1-A)
												// 误差补偿
												PADDSW MM1, MM5
												PSRLW  MM1, 8
												PADDSW MM0, MM1	// 对应16位相加
												PACKUSWB MM0, MM0 // 压缩，每16位压缩为8位(RGBARGBA)

												POP EAX					// 赋给pDest
												MOVD  [EAX], MM0
LOOP2END:
											MOV EAX, pDes
												ADD EAX, nAddDestWidth		// nDestBufferWidth需乘4
												MOV [pDes], EAX

												MOV EAX, pSrc
												ADD EAX, nAddSrcWidth
												MOV [pSrc], EAX

												POP EAX
												INC	EAX			 // 计数器自增，返回循环1 
												JMP LOOP1
EXITLOOP1:
											EMMS			// 清空
	}
#endif
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 对给定的Buffer进行缩放操作
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_BufferZoom(IN OUT BYTE* pbyDestBuf,
							   IN int nDestWidth,
							   IN int nDestHeight,
							   IN BYTE* pbySrcBuf,
							   IN int nSrcWidth,
							   IN int nSrcHeight)
{
	if(pbyDestBuf == NULL || pbySrcBuf == NULL)
		return E_FAIL;
	if(nDestWidth==0 || nDestHeight==0 || nSrcWidth==0 || nSrcHeight==0)
		return E_FAIL;
	float fScaleWidth = nSrcWidth/(float)nDestWidth;
	float fScaleHeight	 = nSrcHeight/(float)nDestHeight;

#if 0
	_declspec(align(16)) float fTemp[]={1.0f, 1.0f, 1.0f, 1.0f};
	__asm
	{
		MOVSS	XMM0, fScaleWidth
			MOVSS	XMM1, fScaleHeight
			PUNPCKLDQ XMM0, XMM1			// fScaleHeight, fScaleWidth
			CVTPS2PI MM0, XMM0				// 取fScaleHeight、fScaleWidth的整数nScaleHeight, nScaleWidth
			CVTPI2PS XMM1, MM0				// 整数转为Float
			SUBPS XMM0, XMM1				// 相减，取出其小数部分存入XMM0

			MOVAPS XMM2, [fTemp]
			MOVAPS XMM7, XMM0
				MOVAPS XMM1, XMM0
				SHUFPS XMM0, XMM0, 0			// 四个fScaleWidth存入XMM0
				SHUFPS XMM1, XMM1, 0x55			// 四个fScaleHeight存入XMM1

				SUBPS XMM2, XMM7				// 相减：1.0f - 小数
				MOVAPS XMM3, XMM2
				SHUFPS XMM2, XMM2, 0			// 四个1.0f-fScaleWidth存入XMM2
				SHUFPS XMM3, XMM3, 0x55			// 四个1.0f-fScaleHeight存入XMM3

				PUNPCKLDQ XMM0, XMM2			// 1.0f-fScaleWidth,fScaleWidth,1.0f-fScaleWidth, fScaleWidth
				PUNPCKLQDQ XMM1, XMM3			// 1.0f-fScaleHeight, 1.0f-fScaleHeight, fScaleHeight, fScaleHeight
				MULPS XMM0, XMM1


	}
#endif

#if 1
	float fX = 0.0f, fY = 0.0f, fModX = 0.0f, fModY = 0.0f, fSubModX=0.0f, fSubModY=0.0f;
	int nX = 0, nY = 0;
	BYTE* pTemp1 = NULL;
	BYTE* pTemp2 = NULL;
	BYTE pixel[4];
	BYTE* pTempDes = pbyDestBuf;
	for(int i=0; i<nDestHeight; i++)
	{
		nY = (int)fY;
		fModY = fmodf(fY, 1.0f);
		fSubModY = 1.0f - fModY;
		if(nY >= nSrcHeight)
			pTemp1 = NULL;
		else
			pTemp1 = pbySrcBuf + nY*nSrcWidth;
		if(nY+1 >= nSrcHeight)
			pTemp2 = NULL;
		else
			pTemp2 = pbySrcBuf + (nY+1)*nSrcWidth;
		fX = 0.0f;
		for(int j=0; j<nDestWidth; j++)
		{
			nX = (int)fX;
			fModX = fmodf(fX, 1.0f);
			fSubModX = 1.0f - fModX;
			if(nX < nSrcWidth && pTemp1 != NULL)
				pixel[0] = *(pTemp1 + nX);
			else
				pixel[0] = 0;
			if(nX+1 < nSrcWidth && pTemp1 != NULL)
				pixel[1] = *(pTemp1 + nX +1);
			else
				pixel[1] = 0;
			if(nX < nSrcWidth && pTemp2 != NULL)
				pixel[2] = *(pTemp2 + nX);
			else
				pixel[2] = 0;
			if(nX+1 < nSrcWidth && pTemp2 != NULL)
				pixel[3] = *(pTemp2 + nX +1);
			else
				pixel[3] = 0;
			pTempDes[j] = (BYTE)(fSubModY*fSubModX*pixel[0] + fSubModY*fModX*pixel[1] + fModY*fSubModX*pixel[2] + fModY*fModX*pixel[3]);
			fX += fScaleWidth;
		}
		fY += fScaleHeight;
		pTempDes += nDestWidth;
	}
#endif
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
// 对给定的Buffer进行缩放操作
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_DwordBufferZoom(IN OUT DWORD* pdwDestBuf,
									IN int nDestX,
									IN int nDestY,
									IN int nDestWidth,
									IN int nDestHeight,
									IN int nDestBufferWidth,
									IN int nDestBufferHeight,
									IN DWORD* pdwSrcBuf,
									IN int nSrcX,
									IN int nSrcY,
									IN int nSrcWidth,
									IN int nSrcHeight,
									IN int nSrcBufferWidth,
									IN int nSrcBufferHeight)
{
	// 数据范围检测
	if(nSrcX>= nSrcBufferWidth || nSrcY>=nSrcBufferHeight)
		return E_FAIL;
	if(nDestX>=nDestBufferWidth || nDestY>=nDestBufferHeight)
		return E_FAIL;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return E_FAIL;
	if(nSrcWidth<=0 || nSrcHeight<=0 ||nDestWidth <=0 ||nDestHeight<=0)
		return E_FAIL;

	// 源缓冲区与目标缓冲区成比例缩放:缩放比例
	float fScaleWidth = nSrcWidth/(float)nDestWidth;
	float fScaleHeight	 = nSrcHeight/(float)nDestHeight;

	// 计算目标和源起点，目标和源的宽度与高度
	if(nDestX < 0)
	{
		nSrcX = (int)(nSrcX-nDestX*fScaleWidth);
		nSrcWidth = (int)(nSrcWidth+nDestX*fScaleWidth);
		nDestWidth += nDestX;
		nDestX = 0;
	}
	if(nDestY< 0)
	{
		nSrcY = (int)(nSrcY-nDestY*fScaleHeight);
		nSrcHeight = (int)(nSrcHeight+nDestY*fScaleHeight);
		nDestHeight += nDestY;
		nDestY = 0;
	}
	if(nSrcX < 0)
	{
		nDestX = (int)(nDestX-nSrcX*((float)(nDestWidth)/nSrcWidth));
		nDestWidth = (int)(nDestWidth-nSrcX*((float)(nDestWidth)/nSrcWidth));
		nSrcWidth += nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nDestY = (int)(nDestY-nSrcY*((float)(nDestHeight)/nSrcHeight));
		nDestHeight = (int)(nDestHeight+nSrcY*((float)(nDestHeight)/nSrcHeight));
		nSrcHeight += nSrcY;
		nSrcY = 0;
	}
	int nTempWidth = nDestX+nDestWidth;
	int nTempHeight = nDestY+nDestHeight;
	if(nTempWidth > nDestBufferWidth)
	{
		nDestWidth -= (nTempWidth-nDestBufferWidth);
		nSrcWidth  -= (int)((nTempWidth-nDestBufferWidth)*fScaleWidth);
	}
	if(nTempHeight > nDestBufferHeight)
	{
		nDestHeight -= (nTempHeight-nDestBufferHeight);
		nSrcHeight  -= (int)((nTempHeight-nDestBufferHeight)*fScaleHeight);
	}
	nTempWidth = nSrcX + nSrcWidth;
	nTempHeight = nSrcY + nSrcHeight;
	if(nTempWidth > nSrcBufferWidth)
	{
		nSrcWidth -= (nTempWidth-nSrcBufferWidth);
		nDestWidth -= (int)((nTempWidth-nSrcBufferWidth)/fScaleWidth);
	}
	if(nTempHeight > nSrcBufferHeight)
	{
		nSrcHeight -= (nTempHeight-nSrcBufferHeight);
		nDestHeight -= (int)((nTempHeight-nSrcBufferHeight)/fScaleHeight);
	}
	if(nSrcWidth<=0 || nSrcHeight<=0 ||nDestWidth <=0 ||nDestHeight<=0)
		return E_FAIL;

	// 指针起点
	DWORD* pDes = pdwDestBuf + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuf + nSrcY*nSrcBufferWidth + nSrcX;
	DWORD* pTemp = pSrc;
	if(pSrc == NULL || pDes == NULL)
		return E_FAIL;
#if 0
	__asm
	{
		MOV ECX, nDestHeight
			NEG ECX					// 计数器取反
			ADD ECX, 3				// 一次4个
			JNS DWORDZOOM_LOOPHEIGHT_MOD4			// 如果不是负数则跳转
DWORDZOOM_LOOPHEIGHT:

		ADD ECX, 4
			JS DWORDZOOM_LOOPHEIGHT		// 如果是负数则跳转
DWORDZOOM_LOOPHEIGHT_MOD4:
		SUB ECX, 4
			JZ DWORDZOOM_SKIP_LOOPHEIGHT
			NEG ECX

DWORDZOOM_SKIP_LOOPHEIGHT:


	}
#endif
	float fX = 0.0f, fY = 0.0f, fModX = 0.0f, fModY = 0.0f, fSubModX=0.0f, fSubModY=0.0f;
	int nX = 0, nY = 0;
	DWORD* pTemp1 = NULL;
	DWORD* pTemp2 = NULL;
	DWORD pixel0, pixel1, pixel2, pixel3;
	DWORD* pTempDes = pDes;
	DWORD* pTempDes1 = pTempDes;
	for(int i=0; i<nDestHeight; i++)
	{
		nY = (int)fY;
		fModY = fmodf(fY, 1.0f);
		fSubModY = 1.0f - fModY;
		if(nY >= nSrcHeight)
			pTemp1 = NULL;
		else
			pTemp1 = pSrc + nY*nSrcBufferWidth;
		if(nY+1 >= nSrcHeight)
			pTemp2 = NULL;
		else
			pTemp2 = pSrc + (nY+1)*nSrcBufferWidth;
		fX = 0.0f;
		for(int j=0; j<nDestWidth; j++)
		{
			nX = (int)fX;
			fModX = fmodf(fX, 1.0f);
			fSubModX = 1.0f - fModX;
			if(nX < nSrcWidth && pTemp1 != NULL)
				pixel0 = *(pTemp1+nX);
			else
				pixel0 = 0;
			if(nX+1 < nSrcWidth && pTemp1 != NULL)
				pixel1 = *(pTemp1+nX+1);
			else
				pixel1 = 0;
			if(nX < nSrcWidth && pTemp2 != NULL)
				pixel2 = *(pTemp2+nX);
			else
				pixel2 = 0;
			if(nX+1 < nSrcWidth && pTemp2 != NULL)
				pixel3 = *(pTemp2+nX+1);
			else
				pixel3 = 0;
			__asm
			{
				MOV	EDX, pTempDes1
					PXOR XMM7, XMM7
					MOVSS XMM0, fModX
					MOVSS XMM1, fSubModX
					PUNPCKLDQ XMM0, XMM1
					PUNPCKLQDQ XMM0, XMM0
					MOVSS XMM2, fModY
					MOVSS XMM3, fSubModY
					PUNPCKLDQ XMM2, XMM2
					PUNPCKLDQ XMM3, XMM3
					PUNPCKLQDQ XMM2, XMM3
					MULPS XMM0, XMM2		// fSubModY*fSubModX, fSubModY*fModX, fModY*fSubModX, fMoY*fModX

					MOVD XMM1, pixel0
					PUNPCKLBW XMM1, XMM7
					PUNPCKLWD XMM1, XMM7
					CVTDQ2PS XMM1, XMM1		// 转成Float，A, R, G, B
					MOVAPS XMM6, XMM0
					PUNPCKHDQ XMM6, XMM6
					PUNPCKHQDQ XMM6, XMM6
					MULPS XMM1, XMM6		// fSubModY*fSubModX*(A, R, G, B)

					MOVD XMM2, pixel1
					PUNPCKLBW XMM2, XMM7
					PUNPCKLWD XMM2, XMM7
					CVTDQ2PS XMM2, XMM2		// 转成Float，A, R, G, B
					MOVAPS XMM6, XMM0
					PUNPCKHQDQ XMM6, XMM6
					PUNPCKLDQ XMM6,  XMM6                 
					PUNPCKLQDQ XMM6, XMM6
					MULPS XMM2, XMM6		// fSubModY*fModX*(A, R, G, B)

					MOVD XMM3, pixel2
					PUNPCKLBW XMM3, XMM7
					PUNPCKLWD XMM3, XMM7
					CVTDQ2PS XMM3, XMM3		// 转成Float，A, R, G, B
					MOVAPS XMM6, XMM0
					PUNPCKLDQ XMM6, XMM6                  
					PUNPCKHQDQ XMM6, XMM6
					MULPS XMM3, XMM6		// fModY*fSubModX*(A, R, G, B)

					ADDPS XMM1, XMM2

					MOVD XMM4, pixel3
					PUNPCKLBW XMM4, XMM7
					PUNPCKLWD XMM4, XMM7
					CVTDQ2PS XMM4, XMM4		// 转成Float，A, R, G, B
					MOVAPS XMM6, XMM0
					PUNPCKLDQ XMM6,  XMM6                 
					PUNPCKLQDQ XMM6, XMM6
					MULPS XMM4, XMM6		// fModY*fModX*(A, R, G, B)

					ADDPS XMM3, XMM4
					ADDPS XMM1, XMM3
					CVTPS2DQ XMM1, XMM1
					PACKSSDW XMM1, XMM1
					PACKUSWB XMM1, XMM1
					MOVD DWORD PTR [EDX], XMM1
			}
			pTempDes1 ++;
			fX += fScaleWidth;
		}
		fY += fScaleHeight;
		pTempDes += nDestBufferWidth;
		pTempDes1 = pTempDes;
	}
	return S_OK;
}

// 源像素索引表
_declspec(align(16)) DWORD *g_pnSrcIndexTable = NULL;
// 权重表
_declspec(align(8)) BYTE *g_pnSrcWeightTable = NULL;

void CALLBACK TP_CreateTableFor_H1920To1440()
{
	if(!g_pnSrcIndexTable)
		g_pnSrcIndexTable = (DWORD*)_mm_malloc(sizeof(DWORD)*4096, 16);
	if(!g_pnSrcWeightTable)
		g_pnSrcWeightTable = (BYTE*)_mm_malloc(sizeof(BYTE)*4096, 8);
	float fTemp = 0.0f;
	float fAdd = 1920/1440.0f;
	for(int i=0; i<4096; i++)
	{
		fTemp = i*fAdd;
		g_pnSrcIndexTable[i] = (int)fTemp;
		g_pnSrcWeightTable[i] = (BYTE)(fmodf(fTemp, 1.0f) * 256);
	}
}

void CALLBACK TP_DeleteTableFor_H1920To1440()
{
	if(g_pnSrcIndexTable)
	{
		_mm_free(g_pnSrcIndexTable);
		g_pnSrcIndexTable = NULL;
	}
	if(g_pnSrcWeightTable)
	{
		_mm_free(g_pnSrcWeightTable);
		g_pnSrcWeightTable = NULL;
	}
}

//
//int TP_WriteBimap(CString ptszFileName,int iWidth,int iHeight,int iDepth,BYTE* pData)
//{
//	BITMAPFILEHEADER stFileHdr;
//	stFileHdr.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
//	stFileHdr.bfSize=sizeof(BITMAPFILEHEADER);
//	stFileHdr.bfType=0x4d42; 
//	BITMAPINFOHEADER stInfoHdr;
//	memset(&stInfoHdr,0,sizeof(BITMAPINFOHEADER));
//	stInfoHdr.biSize=sizeof(BITMAPINFOHEADER);
//	stInfoHdr.biPlanes=1;
//	stInfoHdr.biWidth=iWidth;
//	stInfoHdr.biHeight=iHeight;
//	stInfoHdr.biBitCount=iDepth;
//	stInfoHdr.biSizeImage=stInfoHdr.biWidth*stInfoHdr.biHeight*stInfoHdr.biBitCount/8;
//
//	FILE* fp= NULL;
//	_wfopen_s(&fp,(LPCTSTR)ptszFileName,_T("wb"));
//	fwrite(&stFileHdr,1,sizeof(BITMAPFILEHEADER),fp);
//	fwrite(&stInfoHdr,1,sizeof(BITMAPINFOHEADER),fp);
//	fwrite(pData,1,stInfoHdr.biSizeImage,fp);
//	fclose(fp);
//	return 1;
//}

void CALLBACK TP_LinearScale_H1920To1440(OUT BYTE* pDest,
										 IN int nDestWidth,
										 IN int nDestPitch,
										 IN BYTE* pSrc,
										 IN int nSrcWidth,
										 IN int nSrcPitch,
										 IN int nHeight
										 )
{

//	NXBuf2DImg stuDes, stuSrc;
//	stuSrc.pucBuf		= pSrc;
//	stuSrc.nBufBytes	= nHeight*nSrcPitch;
//	stuSrc.nPitch		= nSrcPitch;	
//	stuSrc.nBitCount	= 32;
//	stuSrc.ePixelFmt	= NXVIDEO_PIXEL_FMT_RGBA;
//	stuSrc.nBaseWidth	= nSrcWidth;
//	stuSrc.nBaseHeight	= nHeight;
//	stuSrc.eScanType	= NXVIDEO_SCAN_PROGRESSIVE;
//	stuSrc.dX			= 0;
//	stuSrc.dY			= 0;
//	stuSrc.nWidth		= nSrcWidth;
//	stuSrc.nHeight		= nHeight;
//	stuSrc.eAspect		= NXVIDEO_ASPECT_ARBITRARY;
//	stuSrc.eDefinition	= NXVIDEO_DEFINITION_HIGH;
//
//
//	stuDes.pucBuf		= pDest;
//	stuDes.nBufBytes	= nHeight*nDestPitch;
//	stuDes.nPitch		= nDestPitch;
//	stuDes.nBitCount	= 32;
//	stuDes.ePixelFmt	= NXVIDEO_PIXEL_FMT_RGBA;
//	stuDes.nBaseWidth	= nDestPitch/4;
//	stuDes.nBaseHeight	= nHeight;
//	stuDes.eScanType	= NXVIDEO_SCAN_PROGRESSIVE;
//	stuDes.dX			= 0;
//	stuDes.dY			= 0;
//	stuDes.nWidth		= nDestWidth;
//	stuDes.nHeight		= nHeight;
//	stuDes.eAspect		= NXVIDEO_ASPECT_ARBITRARY;
//	stuDes.eDefinition	= NXVIDEO_DEFINITION_HIGH;
//
//	g_lpVideoProcAPI->pStuVideoConvAPI->pfnResampling(&stuDes, &stuSrc, NXVIDEO_ADAPTION_NONE, NXVIDEO_ANTIALIAS_NONE, NULL, 0, NXVIDEO_SAM_WINDOW_KAISER, NXVIDEO_SAM_TAPS_8);

//	return;
	BYTE* pSrcTemp = pSrc;
	BYTE* pDestTemp = pDest;
	int nTemp = nSrcWidth - 1;
	__declspec(align(8)) static const __int64 nMax		=		0x0100010001000100;
	for(int i=0; i<nHeight; i++)
	{
		/*for(int j=0; j<nDestWidth; j++)
		{
			BYTE* pSrc1 = pSrcTemp + g_pnSrcIndexTable[j] * 4;
			if(g_pnSrcWeightTable[j] == 0)
			{
				pDestTemp[j*4] = (*pSrc1);
				pDestTemp[j*4+1] = (*(pSrc1+1));
				pDestTemp[j*4+2] = (*(pSrc1+2));
				pDestTemp[j*4+3] = (*(pSrc1+3));
			}
			else if(g_pnSrcIndexTable[j] >= nTemp)
			{
				pDestTemp[j*4] = (*pSrc1) * (256-g_pnSrcWeightTable[j]) /256;
				pDestTemp[j*4+1] = (*(pSrc1+1)) * (256-g_pnSrcWeightTable[j]) /256;
				pDestTemp[j*4+2] = (*(pSrc1+2)) * (256-g_pnSrcWeightTable[j]) /256;
				pDestTemp[j*4+3] = (*(pSrc1+3)) * (256-g_pnSrcWeightTable[j]) /256;
			}
			else
			{
				pDestTemp[j*4] = ((*pSrc1) * (256-g_pnSrcWeightTable[j]) + (*(pSrc1+4)) * g_pnSrcWeightTable[j])/256;
				pDestTemp[j*4+1] = ((*(pSrc1+1)) * (256-g_pnSrcWeightTable[j]) + (*(pSrc1+5)) * g_pnSrcWeightTable[j])/256;
				pDestTemp[j*4+2] = ((*(pSrc1+2)) * (256-g_pnSrcWeightTable[j]) + (*(pSrc1+6)) * g_pnSrcWeightTable[j])/256;
				pDestTemp[j*4+3] = ((*(pSrc1+3)) * (256-g_pnSrcWeightTable[j]) + (*(pSrc1+7)) * g_pnSrcWeightTable[j])/256;
			}
		}*/
		__asm
		{
			mov eax, pDestTemp
				xor ecx, ecx
LOOPWIDTH:
			cmp ecx, nDestWidth
				jge LOOPWIDTH_END

				mov esi, ecx
				shl esi, 2		// *4
				add esi, g_pnSrcIndexTable
				mov edx, [esi]	// g_pnSrcIndexTable[j] 
				cmp edx, nTemp
					jl  LOOPWIDTH_NORMAL

					// g_pnSrcIndexTable[j] >= nTemp
					shl edx, 2		// g_pnSrcIndexTable[j] * 4
					add edx, pSrcTemp
					mov edi, [edx]	// src颜色放在edi中
					mov esi, ecx
						add esi, g_pnSrcWeightTable
						xor edx, edx
						mov dl, [esi]	// 取出权重值放在edx中
						cmp dl, 0
							jnz LOOPWIDTH_WEIGHT0  
							// 权重 == 0 时
							mov [eax], edi	// 直接附值
							inc ecx
							add eax, 4
							jmp LOOPWIDTH
LOOPWIDTH_WEIGHT0:
						// 权重非0时
						movq mm0, nMax
							movd mm1, edx
							punpcklwd mm1, mm1
							punpckldq mm1, mm1
							psubw mm0, mm1		// 256 - g_pnSrcWeightTable[j]
							movd mm1, edi
							pxor mm2, mm2
							punpcklbw mm1, mm2	// 0A0R0G0B
							pmullw mm0, mm1
							psraw mm0, 8		// /256
							packsswb mm0, mm0
							movd [eax], mm0	// 直接附值
							inc ecx
							add eax, 4
							jmp LOOPWIDTH

LOOPWIDTH_NORMAL:
						// g_pnSrcIndexTable[j] < nTemp
						shl edx, 2		// g_pnSrcIndexTable[j] * 4
							add edx, pSrcTemp
							mov edi, [edx]	// src颜色放在edi中
							movd mm6, [edx+4] // src后一像素
							mov esi, ecx
								add esi, g_pnSrcWeightTable
								xor edx, edx
								mov dl, [esi]	// 取出权重值放在edx中
								cmp dl, 0 
									jnz LOOPWIDTH_NORMAL_WEIGHT0  
									// 权重 == 0 时
									mov [eax], edi	// 直接附值 
									inc ecx
									add eax, 4
									jmp LOOPWIDTH
LOOPWIDTH_NORMAL_WEIGHT0:
								// 权重非0时
								movq mm0, nMax
									movd mm1, edx
									punpcklwd mm1, mm1
									punpckldq mm1, mm1

									psubw mm0, mm1		// mm0 = 256 - g_pnSrcWeightTable[j]
									movd mm3, edi
									pxor mm2, mm2
									punpcklbw mm3, mm2	// 0A0R0G0B
									pmullw mm0, mm3
									punpcklbw mm6, mm2	// mm6 = 后面一像素
									pmullw mm6, mm1
									paddusw mm0, mm6 
									psraw mm0, 8		// /256
									packsswb mm0, mm0
									movd [eax], mm0	// 直接附值

									inc ecx
									add eax, 4
									jmp LOOPWIDTH
LOOPWIDTH_END:

		}
		pSrcTemp += nSrcPitch;
		pDestTemp += nDestPitch;
	}
	__asm emms
}
// YUYV拉伸拷贝(对于多余的YUV，则变成YU0V)
LRESULT CALLBACK TP_StretchBlt_YUYV(
									IN OUT BYTE* pbyDestBuffer, 
									IN int nDestPitch,
									IN int nDestBufferWidth,
									IN int nDestBufferHeight,
									IN BYTE* pbySrcBuffer,
									IN int nSrcPitch,
									IN int nSrcBufferWidth,		// 像素点数
									IN int nSrcBufferHeight)
{

	if(nDestPitch == nDestBufferWidth*4)
	{
		NXBuf2DImg            pNXBufIn;       //适配使用新的CPU SDK函数
		NXBuf2DImg            pNXBufOut;
		pNXBufIn.dX = 0;
		pNXBufIn.dY = 0;
		pNXBufIn.eAspect = nSrcBufferWidth <= 720 ? NXVIDEO_ASPECT_4_3:NXVIDEO_ASPECT_16_9;
		pNXBufIn.eDefinition = nSrcBufferWidth <= 720 ? NXVIDEO_DEFINITION_STANDARD:NXVIDEO_DEFINITION_HIGH;
		pNXBufIn.ePixelFmt = NXVIDEO_PIXEL_FMT_RGBA;
		pNXBufIn.eScanType = NXVIDEO_SCAN_PROGRESSIVE;
		pNXBufIn.nBaseWidth = nSrcBufferWidth;
		pNXBufIn.nBaseHeight = nSrcBufferHeight;
		pNXBufIn.nBitCount = 32;
		pNXBufIn.nBufBytes = nSrcBufferWidth*nSrcBufferHeight*pNXBufIn.nBitCount/8;
		pNXBufIn.nWidth = nSrcBufferWidth;
		pNXBufIn.nHeight = nSrcBufferHeight;
		pNXBufIn.nPitch = nSrcBufferWidth*pNXBufIn.nBitCount/8;
		pNXBufIn.pucBuf = pbySrcBuffer;
		pNXBufOut.dX = 0;
		pNXBufOut.dY = 0;
		pNXBufOut.ePixelFmt = NXVIDEO_PIXEL_FMT_RGBA;
		pNXBufOut.eScanType = NXVIDEO_SCAN_PROGRESSIVE;
		pNXBufOut.nBaseWidth = nDestBufferWidth;
		pNXBufOut.nBaseHeight = nDestBufferHeight;
		pNXBufOut.nBitCount = 32;
		pNXBufOut.nWidth = nDestBufferWidth;
		pNXBufOut.nHeight = nDestBufferHeight;
		pNXBufOut.nBufBytes = nDestBufferWidth*nDestBufferHeight*pNXBufOut.nBitCount/8;
		pNXBufOut.eAspect = nSrcBufferWidth <= 720 ? NXVIDEO_ASPECT_4_3:NXVIDEO_ASPECT_16_9;
		pNXBufOut.eDefinition = nSrcBufferWidth <= 720 ? NXVIDEO_DEFINITION_STANDARD:NXVIDEO_DEFINITION_HIGH;
		pNXBufOut.nPitch = nDestBufferWidth*pNXBufOut.nBitCount/8;
		pNXBufOut.pucBuf = pbyDestBuffer;

		return g_lpVideoProcAPI->pStuVideoConvAPI->pfnResampling(&pNXBufOut,&pNXBufIn,
			NXVIDEO_ADAPTION_NONE,NXVIDEO_ANTIALIAS_NONE,
			NULL,0,
			NXVIDEO_SAM_WINDOW_KAISER,NXVIDEO_SAM_TAPS_8);
	}
	else if(nDestPitch == nDestBufferWidth*2)
	{
		NXBuf2DImg            pNXBufIn;       //适配使用新的CPU SDK函数
		NXBuf2DImg            pNXBufOut;
		pNXBufIn.dX = 0;
		pNXBufIn.dY = 0;
		pNXBufIn.eAspect = nSrcBufferWidth <= 720 ? NXVIDEO_ASPECT_4_3:NXVIDEO_ASPECT_16_9;
		pNXBufIn.eDefinition = nSrcBufferWidth <= 720 ? NXVIDEO_DEFINITION_STANDARD:NXVIDEO_DEFINITION_HIGH;
		pNXBufIn.ePixelFmt = NXVIDEO_PIXEL_FMT_YUYV;
		pNXBufIn.eScanType = NXVIDEO_SCAN_PROGRESSIVE;
		pNXBufIn.nBaseWidth = nSrcBufferWidth;
		pNXBufIn.nBaseHeight = nSrcBufferHeight;
		pNXBufIn.nBitCount = 16;
		pNXBufIn.nBufBytes = nSrcBufferWidth*nSrcBufferHeight*pNXBufIn.nBitCount/8;
		pNXBufIn.nWidth = nSrcBufferWidth;
		pNXBufIn.nHeight = nSrcBufferHeight;
		pNXBufIn.nPitch = nSrcBufferWidth*pNXBufIn.nBitCount/8;
		pNXBufIn.pucBuf = pbySrcBuffer;
		pNXBufOut.dX = 0;
		pNXBufOut.dY = 0;
		pNXBufOut.ePixelFmt = NXVIDEO_PIXEL_FMT_YUYV;
		pNXBufOut.eScanType = NXVIDEO_SCAN_PROGRESSIVE;
		pNXBufOut.nBaseWidth = nDestBufferWidth;
		pNXBufOut.nBaseHeight = nDestBufferHeight;
		pNXBufOut.nBitCount = 16;
		pNXBufOut.nWidth = nDestBufferWidth;
		pNXBufOut.nHeight = nDestBufferHeight;
		pNXBufOut.nBufBytes = nDestBufferWidth*nDestBufferHeight*pNXBufOut.nBitCount/8;
		pNXBufOut.eAspect = nSrcBufferWidth <= 720 ? NXVIDEO_ASPECT_4_3:NXVIDEO_ASPECT_16_9;
		pNXBufOut.eDefinition = nSrcBufferWidth <= 720 ? NXVIDEO_DEFINITION_STANDARD:NXVIDEO_DEFINITION_HIGH;
		pNXBufOut.nPitch = nDestBufferWidth*pNXBufOut.nBitCount/8;
		pNXBufOut.pucBuf = pbyDestBuffer;

		return g_lpVideoProcAPI->pStuVideoConvAPI->pfnResampling(&pNXBufOut,&pNXBufIn,
			NXVIDEO_ADAPTION_NONE,NXVIDEO_ANTIALIAS_NONE,
			NULL,0,
			NXVIDEO_SAM_WINDOW_KAISER,NXVIDEO_SAM_TAPS_8);
	}
	else
	{
		NXBuf2DImg            pNXBufIn;       //适配使用新的CPU SDK函数
		NXBuf2DImg            pNXBufOut;
		pNXBufIn.dX = 0;
		pNXBufIn.dY = 0;
		pNXBufIn.eAspect = nSrcBufferWidth <= 720 ? NXVIDEO_ASPECT_4_3:NXVIDEO_ASPECT_16_9;
		pNXBufIn.eDefinition = nSrcBufferWidth <= 720 ? NXVIDEO_DEFINITION_STANDARD:NXVIDEO_DEFINITION_HIGH;
		pNXBufIn.ePixelFmt = NXVIDEO_PIXEL_FMT_KEY;
		pNXBufIn.eScanType = NXVIDEO_SCAN_PROGRESSIVE;
		pNXBufIn.nBaseWidth = nSrcBufferWidth;
		pNXBufIn.nBaseHeight = nSrcBufferHeight;
		pNXBufIn.nBitCount = 8;
		pNXBufIn.nBufBytes = nSrcBufferWidth*nSrcBufferHeight*pNXBufIn.nBitCount/8;
		pNXBufIn.nWidth = nSrcBufferWidth;
		pNXBufIn.nHeight = nSrcBufferHeight;
		pNXBufIn.nPitch = nSrcBufferWidth*pNXBufIn.nBitCount/8;
		pNXBufIn.pucBuf = pbySrcBuffer;
		pNXBufOut.dX = 0;
		pNXBufOut.dY = 0;
		pNXBufOut.ePixelFmt = NXVIDEO_PIXEL_FMT_KEY;
		pNXBufOut.eScanType = NXVIDEO_SCAN_PROGRESSIVE;
		pNXBufOut.nBaseWidth = nDestBufferWidth;
		pNXBufOut.nBaseHeight = nDestBufferHeight;
		pNXBufOut.nBitCount = 8;
		pNXBufOut.nWidth = nDestBufferWidth;
		pNXBufOut.nHeight = nDestBufferHeight;
		pNXBufOut.nBufBytes = nDestBufferWidth*nDestBufferHeight*pNXBufOut.nBitCount/8;
		pNXBufOut.eAspect = nSrcBufferWidth <= 720 ? NXVIDEO_ASPECT_4_3:NXVIDEO_ASPECT_16_9;
		pNXBufOut.eDefinition = nSrcBufferWidth <= 720 ? NXVIDEO_DEFINITION_STANDARD:NXVIDEO_DEFINITION_HIGH;
		pNXBufOut.nPitch = nDestBufferWidth*pNXBufOut.nBitCount/8;
		pNXBufOut.pucBuf = pbyDestBuffer;

		return g_lpVideoProcAPI->pStuVideoConvAPI->pfnResampling(&pNXBufOut,&pNXBufIn,
			NXVIDEO_ADAPTION_NONE,NXVIDEO_ANTIALIAS_NONE,
			NULL,0,
			NXVIDEO_SAM_WINDOW_KAISER,NXVIDEO_SAM_TAPS_8);
	}
		
	float fScaleWidth	= (nSrcBufferWidth-1)/(float)(nDestBufferWidth-1);
	float fScaleHeight	= (nSrcBufferHeight-1)/(float)(nDestBufferHeight-1);

#if 1
	// 临时存储Y和Y+1的YUVXYUVX串
	// 每行字节数
	int nSrcBytes = nSrcBufferWidth<<2; // *3
	BYTE* pbyTempSrc1 = (BYTE*)_mm_malloc(sizeof(BYTE)*nSrcBytes, 16);
	BYTE* pbyTempSrc2 =  (BYTE*)_mm_malloc(sizeof(BYTE)*nSrcBytes, 16);
	// 临时存储目标YUVYUV串
	int nDstBytes = nDestBufferWidth<<2; // *3
	BYTE* pbyTempDest =  (BYTE*)_mm_malloc(sizeof(BYTE)*nDstBytes, 16);
	float fY = 0.0f, fModY = 0.0f, fSubModY=0.0f;
	int nY = 0;
	BYTE* pDest = pbyDestBuffer;
	//	BYTE pixel[3][4];
	//	int nHalfWidth = nSrcBufferWidth>>1;
	int nHalfDstWidth = nDestBufferWidth>>1;
	_declspec(align(16)) float arfTemp1[4] = {fScaleWidth, fScaleWidth, fScaleWidth, fScaleWidth};
	_declspec(align(16)) float arfTemp2[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	__declspec(align(16)) static DWORD mask[]		=	{0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	BYTE *pTemp;
	for(int i=0; i<nDestBufferHeight; i++)
	{
		nY = (int)fY;
		fModY = fmodf(fY, 1.0f);
		fSubModY = 1.0f - fModY;
		if(nY >= nSrcBufferHeight)
		{
			memset(pbyTempDest, 0, nDstBytes);
			fY += fScaleHeight;
			pDest += nDestPitch;
			continue;
		}
		else
		{
			pTemp = pbySrcBuffer + nY*nSrcPitch;
			_asm
			{
				mov eax, pTemp
					mov edx, pbyTempSrc1
					mov ecx, nSrcBufferWidth
					neg ecx
					add ecx, 7					// 每次8
					jbe loopY1_4
loopY1_8:

				movdqu xmm0, [eax]			//movedqa 07.03.08
				movdqa xmm7, xmm0
					punpckhqdq xmm7, xmm0		// y1Uy2Vy3Uy4V
					movdqa xmm1, xmm0
					psrlw xmm1, 8			// U0V0U0V0
					punpcklbw xmm1, xmm1	// UU100VV00 UU200VV00
					movdqa xmm2, xmm1
					punpckhqdq xmm2, xmm1	// UU200VV00 UU100VV00

					punpcklwd xmm1, xmm2
					movdqa xmm2, xmm1
					punpckhqdq xmm2, xmm2
					punpckldq xmm1, xmm2

					//	packssdw xmm1, xmm1		// UU1VVUUVV 
					//	packssdw xmm2, xmm2		// UU2VVUUVV
					//	punpcklwd xmm1, xmm2	// UU1UU2VV1VV2

					pand xmm0, [mask]
					packuswb xmm0, xmm0
						punpcklbw xmm0, xmm1	// y1U1y2U1y3U2y4U2
						punpcklbw xmm1, xmm0	// 
						punpckhqdq xmm1, xmm1	// V1AV1AV2AV2A
						punpcklwd xmm0, xmm1	// 最终结果
						movdqa [edx], xmm0
						add edx, 16

						movdqa xmm1, xmm7
						psrlw xmm1, 8			// U0V0U0V0
						punpcklbw xmm1, xmm1	// UU100VV00 UU200VV00
						movdqa xmm2, xmm1
						punpckhqdq xmm2, xmm1	// UU200VV00 UU100VV00

						punpcklwd xmm1, xmm2
						movdqa xmm2, xmm1
						punpckhqdq xmm2, xmm2
						punpckldq xmm1, xmm2

						//	packssdw xmm1, xmm1		// UU1VVUUVV
						//	packssdw xmm2, xmm2		// UU2VVUUVV
						//	punpcklwd xmm1, xmm2	// UU1UU2VV1VV2

						pand xmm7, [mask]
						packuswb xmm7, xmm7
							punpcklbw xmm7, xmm1	// y1U1y2U1y3U2y4U2
							punpcklbw xmm1, xmm7	// 
							punpckhqdq xmm1, xmm1	// V1AV1AV2AV2A
							punpcklwd xmm7, xmm1	// 最终结果
							movdqu [edx], xmm7      //movdqa

							add eax, 16
							add edx, 16
							add ecx, 8
							jnc loopY1_8
loopY1_4:
						sub ecx, 4
							jns loopY1_2

							movq mm0, [eax]
							movq2dq xmm0, mm0		// y1Uy2Vy3Uy4V
								movdqa xmm1, xmm0
								psrlw xmm1, 8			// U0V0U0V0
								punpcklbw xmm1, xmm1	// UU100VV00 UU200VV00
								movdqa xmm2, xmm1
								punpckhqdq xmm2, xmm1	// UU200VV00 UU100VV00

								punpcklwd xmm1, xmm2
								movdqa xmm2, xmm1
								punpckhqdq xmm2, xmm2
								punpckldq xmm1, xmm2

								//	packssdw xmm1, xmm1		// UU1VVUUVV
								//	packssdw xmm2, xmm2		// UU2VVUUVV
								//	punpcklwd xmm1, xmm2	// UU1UU2VV1VV2

								punpcklbw xmm0, xmm1	// y1U1y2U1y3U2y4U2
								punpcklbw xmm1, xmm0	// 
								punpckhqdq xmm1, xmm1	// V1AV1AV2AV2A
								punpcklwd xmm0, xmm1	// 最终结果
								movdqa [edx], xmm0

								add eax, 8
								add edx, 16
								add ecx, 4
loopY1_2:
							sub ecx, 2
								jns skipLoopY1_2

								movd mm0, [eax]			// y1Uy2V
								movq mm1, mm0
									psrlw mm1, 8			// U0V0
									punpcklbw mm1, mm1		// UU100VV00
									movq mm2, mm2
									punpckhbw mm2, mm2
									punpcklwd mm1, mm2
									//	packssdw mm1, mm1		// UU1VV

									punpcklbw mm0, mm1		// y1U1y2U1
									psrlw mm1, mm1			// VUVU
									punpcklwd mm0, mm1	// 最终结果
									movq [edx], mm0

skipLoopY1_2:
								emms	
			}

			/*for(int k=0, p=0, q=0; k<nHalfWidth; k++)
			{
			*(pbyTempSrc1+p) = *(pTemp + q);
			*(pbyTempSrc1+p+1) = *(pTemp + q+1);
			*(pbyTempSrc1+p+2) = *(pTemp + q+3);

			*(pbyTempSrc1+p+4) = *(pTemp + q+2);
			*(pbyTempSrc1+p+5) = *(pTemp + q+1);
			*(pbyTempSrc1+p+6) = *(pTemp + q+3);
			p += 8;
			q += 4;
			}*/
		}
		if(nY+1 >= nSrcBufferHeight)
		{
			memcpy_line(pbyTempSrc2, pbyTempSrc1, (nSrcBytes>>4), (nSrcBytes&0xf));
			//memset_dword_line(pbyTempSrc2, 0x00808000, (nSrcBytes>>4), (nSrcBytes&0xf));
			//memset(pbyTempSrc2, 0, nSrcBytes);	// wrong color value for YUV
		}
		else
		{
			pTemp += nSrcPitch;
			_asm
			{
				mov eax, pTemp
					mov edx, pbyTempSrc2
					mov ecx, nSrcBufferWidth
					neg ecx
					add ecx, 7					// 每次8
					jbe loopY2_4
loopY2_8:

				movdqu xmm0, [eax]
				movdqa xmm7, xmm0
					punpckhqdq xmm7, xmm0		// y1Uy2Vy3Uy4V
					movdqa xmm1, xmm0
					psrlw xmm1, 8			// U0V0U0V0
					punpcklbw xmm1, xmm1	// UU100VV00 UU200VV00
					movdqa xmm2, xmm1
					punpckhqdq xmm2, xmm1	// UU200VV00 UU100VV00

					punpcklwd xmm1, xmm2
					movdqa xmm2, xmm1
					punpckhqdq xmm2, xmm2
					punpckldq xmm1, xmm2

					//	packssdw xmm1, xmm1		// UU1VVUUVV 
					//	packssdw xmm2, xmm2		// UU2VVUUVV
					//	punpcklwd xmm1, xmm2	// UU1UU2VV1VV2

					pand xmm0, [mask]
					packuswb xmm0, xmm0
						punpcklbw xmm0, xmm1	// y1U1y2U1y3U2y4U2
						punpcklbw xmm1, xmm0	// 
						punpckhqdq xmm1, xmm1	// V1AV1AV2AV2A
						punpcklwd xmm0, xmm1	// 最终结果
						movdqa [edx], xmm0
						add edx, 16

						movdqa xmm1, xmm7
						psrlw xmm1, 8			// U0V0U0V0
						punpcklbw xmm1, xmm1	// UU100VV00 UU200VV00
						movdqa xmm2, xmm1
						punpckhqdq xmm2, xmm1	// UU200VV00 UU100VV00

						punpcklwd xmm1, xmm2
						movdqa xmm2, xmm1
						punpckhqdq xmm2, xmm2
						punpckldq xmm1, xmm2

						//	packssdw xmm1, xmm1		// UU1VVUUVV
						//	packssdw xmm2, xmm2		// UU2VVUUVV
						//	punpcklwd xmm1, xmm2	// UU1UU2VV1VV2

						pand xmm7, [mask]
						packuswb xmm7, xmm7
							punpcklbw xmm7, xmm1	// y1U1y2U1y3U2y4U2
							punpcklbw xmm1, xmm7	// 
							punpckhqdq xmm1, xmm1	// V1AV1AV2AV2A
							punpcklwd xmm7, xmm1	// 最终结果
							movdqa [edx], xmm7

							add eax, 16
							add edx, 16
							add ecx, 8
							jnc loopY2_8
loopY2_4:
						sub ecx, 4
							jns loopY2_2

							movq mm0, [eax]
							movq2dq xmm0, mm0		// y1Uy2Vy3Uy4V
								movdqa xmm1, xmm0
								psrlw xmm1, 8			// U0V0U0V0
								punpcklbw xmm1, xmm1	// UU100VV00 UU200VV00
								movdqa xmm2, xmm1
								punpckhqdq xmm2, xmm1	// UU200VV00 UU100VV00

								punpcklwd xmm1, xmm2
								movdqa xmm2, xmm1
								punpckhqdq xmm2, xmm2
								punpckldq xmm1, xmm2

								//	packssdw xmm1, xmm1		// UU1VVUUVV
								//	packssdw xmm2, xmm2		// UU2VVUUVV
								//	punpcklwd xmm1, xmm2	// UU1UU2VV1VV2

								punpcklbw xmm0, xmm1	// y1U1y2U1y3U2y4U2
								punpcklbw xmm1, xmm0	// 
								punpckhqdq xmm1, xmm1	// V1AV1AV2AV2A
								punpcklwd xmm0, xmm1	// 最终结果
								movdqa [edx], xmm0

								add eax, 8
								add edx, 16
								add ecx, 4
loopY2_2:
							sub ecx, 2
								jns skipLoopY2_2

								movd mm0, [eax]			// y1Uy2V
								movq mm1, mm0
									psrlw mm1, 8			// U0V0
									punpcklbw mm1, mm1		// UU100VV00
									movq mm2, mm2
									punpckhbw mm2, mm2
									punpcklwd mm1, mm2
									//	packssdw mm1, mm1		// UU1VV

									punpcklbw mm0, mm1		// y1U1y2U1
									psrlw mm1, mm1			// VUVU
									punpcklwd mm0, mm1	// 最终结果
									movq [edx], mm0

skipLoopY2_2:
								emms	
			}
			/*pTemp = pbySrcBuffer + (nY+1)*nSrcPitch;
			for(int k=0, p=0, q=0; k<nHalfWidth; k++)
			{
			*(pbyTempSrc2+p) = *(pTemp + q);
			*(pbyTempSrc2+p+1) = *(pTemp + q+1);
			*(pbyTempSrc2+p+2) = *(pTemp + q+3);

			*(pbyTempSrc2+p+4) = *(pTemp + q+2);
			*(pbyTempSrc2+p+5) = *(pTemp + q+1);
			*(pbyTempSrc2+p+6) = *(pTemp + q+3);
			p += 8;
			q += 4;
			}*/
		}
		//fX = 0.0f;
		_asm
		{
			MOVSS	XMM0, fModY
				MOVSS	XMM1, fSubModY
				PUNPCKLDQ XMM0, XMM0
				PUNPCKLQDQ XMM0, XMM0
				PUNPCKLDQ XMM1, XMM1
				PUNPCKLQDQ XMM1, XMM1			// 四个相同值:XMM0存4个fModY, XMM1存4个fSubModY
				pxor XMM7, XMM7

				mov edx, pbyTempDest
				xor eax, eax
blastloop:
			cmp eax, nDestBufferWidth
				jnl		skipblastloop			// 跳转

				movaps xmm2, xmm7				// fScaleWidth
				CVTPS2DQ XMM5, XMM2				// 取fScaleWidth的整数nScaleWidth
				CVTDQ2PS XMM3, XMM5				// 整数转为Float,存在xmm3
				SUBPS XMM2, XMM3				// 相减，取出其小数部分存入XMM2,fModx
				MOVAPS XMM4, [arfTemp2]
				SUBPS XMM4, XMM2				// 相减：1.0f - 小数,存入Xmm4, fSubModx

					movd ecx, xmm5
					cmp ecx, nSrcBufferWidth		// 如果nX >= nSrcBufferWidth
					jl blastloop1
					mov [edx], 0x00808000;
					add edx, 4
					inc eax
					addps xmm7, [arfTemp1]
					jmp blastloop
blastloop1:

					mov esi, ecx
						shl esi, 2
						mov edi, esi
						add esi, pbyTempSrc1
						add edi, pbyTempSrc2
						movd xmm5, [esi]			// pixel0
						movd xmm6, [edi]			// pixel2
						pxor xmm3, xmm3
							punpcklbw xmm5, xmm3
							punpcklbw xmm6, xmm3
							punpcklwd xmm5, xmm3
							punpcklwd xmm6, xmm3
							cvtdq2ps xmm5, xmm5			// 转成float
							cvtdq2ps xmm6, xmm6
							movaps xmm3, xmm4
							mulps xmm3, xmm1			// fSubModY*fSubModX
							mulps xmm3, xmm5			// fSubModY*fSubModX*p0
							mulps xmm4,	xmm0			// fModY*fSubModX
							mulps xmm4, xmm6			// fModY*fSubModX*p2
							addps xmm3, xmm4

							inc ecx
							cmp ecx, nSrcBufferWidth	// 如果nX+1 >= nSrcBufferWidth
							jl blastloop2

							mov ecx, nSrcBufferWidth;
							dec ecx;
							//cvtps2dq xmm3, xmm3			// 转成整数
							//packssdw xmm3, xmm3
							//PACKUSWB xmm3, xmm3
							//movd [edx], xmm3

							//add edx, 4
							//inc eax
							//addps xmm7, [arfTemp1]
							//jmp blastloop
blastloop2:
							mov esi, ecx
								shl esi, 2
								mov edi, esi
								add esi, pbyTempSrc1
								add edi, pbyTempSrc2
								movd xmm5, [esi]			// pixel1
								movd xmm6, [edi]			// pixel3
								pxor xmm4, xmm4
									punpcklbw xmm5, xmm4
									punpcklbw xmm6, xmm4
									punpcklwd xmm5, xmm4
									punpcklwd xmm6, xmm4
									cvtdq2ps xmm5, xmm5			// 转成float
									cvtdq2ps xmm6, xmm6
									movaps xmm4, xmm2
									mulps xmm2, xmm1			// fSubModY*fModX
									mulps xmm2, xmm5			// fSubModY*fModX*p1
									mulps xmm4,	xmm0			// fModY*fModX
									mulps xmm4, xmm6			// fModY*fModX*p3
									addps xmm2, xmm4
									addps xmm2, xmm3			//
									cvtps2dq xmm2,xmm2			// 转成整数
									packssdw xmm2, xmm2
									PACKUSWB xmm2, xmm2
									movd [edx], xmm2

									add edx, 4
									inc eax
									addps xmm7, [arfTemp1]
									jmp blastloop
skipblastloop:
									emms
		}
		/*for(int j=0; j<nDestBufferWidth; j++)
		{
		nX = (int)fX;
		fModX = fmodf(fX, 1.0f);
		fSubModX = 1.0f - fModX;
		if(nX < nSrcBufferWidth)
		{

		pixel[0][0] = *(pbyTempSrc1 + nX*4 );
		pixel[1][0] = *(pbyTempSrc1 + nX*4 +1);
		pixel[2][0] = *(pbyTempSrc1 + nX*4 +2);

		pixel[0][2] = *(pbyTempSrc2 + nX*4);
		pixel[1][2] = *(pbyTempSrc2 + nX*4 +1);
		pixel[2][2] = *(pbyTempSrc2 + nX*4 +2);
		}
		else
		{
		pixel[0][0] = 0;
		pixel[1][0] = 0;
		pixel[2][0] = 0;

		pixel[0][2] = 0;
		pixel[1][2] = 0;
		pixel[2][2] = 0;
		}
		if(nX+1 < nSrcBufferWidth)
		{
		pixel[0][1] = *(pbyTempSrc1 + (nX+1)*4);
		pixel[1][1] = *(pbyTempSrc1 + (nX+1)*4 +1);
		pixel[2][1] = *(pbyTempSrc1 + (nX+1)*4 +2);

		pixel[0][3] = *(pbyTempSrc2 + (nX+1)*4);
		pixel[1][3] = *(pbyTempSrc2 + (nX+1)*4 +1);
		pixel[2][3] = *(pbyTempSrc2 + (nX+1)*4 +2);
		}
		else
		{
		pixel[0][1] = 0;
		pixel[1][1] = 0;
		pixel[2][1] = 0;

		pixel[0][3] = 0;
		pixel[1][3] = 0;
		pixel[2][3] = 0;
		}

		pTempDes[j*4] = (BYTE)(fSubModY*fSubModX*pixel[0][0] + fSubModY*fModX*pixel[0][1] + fModY*fSubModX*pixel[0][2] + fModY*fModX*pixel[0][3]);
		pTempDes[j*4+1] = (BYTE)(fSubModY*fSubModX*pixel[1][0] + fSubModY*fModX*pixel[1][1] + fModY*fSubModX*pixel[1][2] + fModY*fModX*pixel[1][3]);
		pTempDes[j*4+2] = (BYTE)(fSubModY*fSubModX*pixel[2][0] + fSubModY*fModX*pixel[2][1] + fModY*fSubModX*pixel[2][2] + fModY*fModX*pixel[2][3]);
		fX += fScaleWidth;
		}*/

		// 拆成YUYV
		for(int k=0, p=0, q=0; k<nHalfDstWidth; k++)
		{
			*(pDest+p) = *(pbyTempDest + q);
			*(pDest+p+1) = (*(pbyTempDest + q+1) + *(pbyTempDest + q+5)) >> 1;
			*(pDest+p+2) = *(pbyTempDest + q+4);
			*(pDest+p+3) = (*(pbyTempDest + q+2) + *(pbyTempDest + q+6)) >> 1;
			p += 4;
			q += 8;
		}
		fY += fScaleHeight;
		pDest += nDestPitch;
	}
	_mm_free(pbyTempSrc1);
	_mm_free(pbyTempSrc2);
	_mm_free(pbyTempDest);
#endif
	return 1;
}
_declspec(align(16)) DWORD ymask[] = {0x00ff00ff,0x00ff00ff,0x00ff00ff,0x00ff00ff,};
BOOL yuv_yyuuvv2yuv(BYTE* pDesBuffer,BYTE* pSrcField,int nA16,int nMod16)
{
	_asm{
		PXOR	XMM0,XMM0
			MOVDQA	XMM1,ymask
			MOV		EDI,pDesBuffer
			MOV		ESI,pSrcField
			MOV		ECX,nA16
loop_16:
		TEST	 ECX,ECX
			JZ		 loop_8
			MOVDQU	 XMM2,[ESI]
			PAND	 XMM2,XMM1
				ADD		 ESI,16
				SUB		 ECX,1
				JNZ		 loop_16_1
				PACKUSWB XMM2,XMM2
				MOVSD	 [EDI],XMM2
				ADD		 EDI,8
				JMP		 loop_8
loop_16_1:
			MOVDQU   XMM3,[ESI]
			PAND	 XMM3,XMM1
				PACKUSWB XMM2,XMM3
				MOVDQU	 [EDI],XMM2
				ADD		 EDI,16
				ADD		 ESI,16
				SUB		 ECX,1
				JMP		 loop_16
loop_8:
			MOV		 ECX,nMod16
				TEST	 ECX,0x00000008
				JZ		 loop_4
				MOVSD	 XMM2,[ESI]
				PAND	 XMM2,XMM1
					PACKUSWB XMM2,XMM2
					MOVD	 [EDI],XMM2
					ADD		 EDI,4
					ADD		 ESI,8
loop_4: 
				TEST	 ECX,0x00000004
					JZ		 loop_2
					MOVD	 XMM2,[ESI]
					PAND	 XMM2,XMM1
						PACKUSWB XMM2,XMM2
						MOVD	 EAX,XMM2
						MOV		 [EDI],AX
						ADD		 EDI,2
						ADD		 ESI,4
loop_2:
					TEST	 ECX,0x00000002
						JZ		 loop_end
						MOV		 AX,[ESI]
						MOVD	 XMM2,EAX
							PAND	 XMM2,XMM1
							PACKUSWB XMM2,XMM2
							MOVD	 EAX,XMM2
							MOV		 [EDI],AL
							ADD		 EDI,1
							ADD		 ESI,2
loop_end:
	}
}

BOOL yuv_yuv2yyuuvv(BYTE* pDesBuffer,BYTE* pSrcField,int nA8,int nMod8)
{
	_asm{
		PXOR	XMM0,XMM0
			MOV		EDI,pDesBuffer
			MOV		ESI,pSrcField
			MOV		ECX,nA8
loop_8:
		TEST	 ECX,ECX
			JZ		 loop_4
			MOVSD	 XMM1,[ESI]
			PUNPCKLBW XMM1,XMM0
				MOVDQA	 XMM2,XMM1
				PSLLW	 XMM2,8
				POR		 XMM1,XMM2
				MOVDQU	 [EDI],XMM1
				SUB		 ECX,1
				ADD		 ESI,8
				ADD	     EDI,16
				JMP		 loop_8
loop_4: 
			MOV		 ECX,nMod8
				TEST	 ECX,0x00000004
				JZ		 loop_2
				MOVD	 XMM1,[ESI]
				PUNPCKLBW XMM1,XMM0
					MOVDQA	 XMM2,XMM1
					PSLLW	 XMM2,8
					POR		 XMM1,XMM2
					MOVSD	 [EDI],XMM1
					ADD		 ESI,4
					ADD	     EDI,8
loop_2:
				TEST	 ECX,0x00000002
					JZ		 loop_1
					MOV		 AX,[ESI]
					MOVD	 XMM1,EAX
						PUNPCKLBW XMM1,XMM0
						MOVDQA	 XMM2,XMM1
						PSLLW	 XMM2,8
						POR		 XMM1,XMM2
						MOVD	 [EDI],XMM1
						ADD		 EDI,4
						ADD		 ESI,2
loop_1:
					TEST	 ECX,0x00000001
						JZ		 loop_end
						MOV		 AL,[ESI]
						MOVD	 XMM1,EAX
							PUNPCKLBW XMM1,XMM0
							MOVDQA	 XMM2,XMM1
							PSLLW	 XMM2,8
							POR		 XMM1,XMM2
							MOVD	 EAX,XMM1
							MOV		 [EDI],AX
							ADD		 EDI,2
							ADD		 ESI,1
loop_end:
	}
	return 1;
}

void yuv_uyvy2yuv8(BYTE* puyvy,BYTE* py,BYTE* pu,BYTE* pv,int nA16,int nM16)
{
	__asm{
		MOV			ECX,nA16
			MOV			ESI,puyvy
			MOV			EAX,py
			MOV			EDI,pu
			MOV			EDX,pv
loop_16:		
		TEST		ECX,ECX
			JZ			loop_8
			MOVDQU		XMM1,[ESI]
			MOVDQA		XMM2,XMM1
				PSRLW		XMM1,8
				PACKUSWB	XMM1,XMM1
				PSLLW		XMM2,8
				PSRLW		XMM2,8
				PACKUSWB	XMM2,XMM2

				MOVDQA		XMM3,XMM2
				PSLLW		XMM2,8
				PSRLW		XMM2,8
				PACKUSWB	XMM2,XMM2
				PSRLW		XMM3,8
				PACKUSWB	XMM3,XMM3




				MOVSD		[EAX],XMM1
				MOVD		[EDI],XMM2
				MOVD		[EDX],XMM3

				ADD			ESI,16
				ADD			EAX,8
				ADD			EDX,4
				ADD			EDI,4
				SUB			ECX,1
				JMP			loop_16
loop_8:
			MOV			ECX,nM16
				TEST		ECX,0x00000008
				JZ			loop_4
				MOVSD		XMM1,[ESI]
				MOVSD		XMM2,XMM1
					PSRLW		XMM1,8
					PSLLW		XMM2,8
					PACKUSWB	XMM1,XMM1
					PSRLW		XMM2,8
					PACKUSWB	XMM2,XMM2

					MOVDQA		XMM3,XMM2
					PSLLW		XMM2,8
					PSRLW		XMM2,8
					PACKUSWB	XMM2,XMM2
					PSRLW		XMM3,8
					PACKUSWB	XMM3,XMM3

					MOVD		[EAX],XMM1
					PUSH		EAX
					MOVD		EAX,XMM2
					MOV			[EDI],AX
					MOVD		EAX,XMM3
					MOV			[EDX],AX
					POP			EAX
					ADD			ESI,8
					ADD			EAX,4
					ADD			EDX,2
					ADD			EDI,2
loop_4:
				TEST		ECX,0x00000004
					JZ			loop_end
					MOVD		XMM1,[ESI]
					MOVDQA		XMM2,XMM1
						PSRLW		XMM1,8
						PSLLW		XMM2,8
						PACKUSWB	XMM1,XMM1
						PSRLW		XMM2,8
						PACKUSWB	XMM2,XMM2

						MOVDQA		XMM3,XMM2
						PSLLW		XMM2,8
						PSRLW		XMM2,8
						PACKUSWB	XMM2,XMM2
						PSRLW		XMM3,8
						PACKUSWB	XMM3,XMM3

						PUSH		EDX
						MOVD		EDX,XMM1
						MOV			[EAX],DX
						POP			EDX

						PUSH		EAX
						MOVD		EAX,XMM2
						MOV			[EDI],AL
						MOVD		EAX,XMM3
						MOV			[EDX],AL
						POP			EAX

						ADD			ESI,8
						ADD			EAX,4
						ADD			EDX,2
						ADD			EDI,2
loop_end:

	};
}

void yuv_yuyv2yuv8(BYTE* puyvy,BYTE* py,BYTE* pu,BYTE* pv,int nA16,int nM16)
{
	__asm{
		MOV			ECX,nA16
			MOV			ESI,puyvy
			MOV			EAX,py
			MOV			EDI,pu
			MOV			EDX,pv
loop_16:		
		TEST		ECX,ECX
			JZ			loop_8
			MOVDQU		XMM1,[ESI]
			MOVDQA		XMM2,XMM1
				PSLLW		XMM1,8
				PSRLW		XMM1,8
				PACKUSWB	XMM1,XMM1
				PSRLW		XMM2,8
				PACKUSWB	XMM2,XMM2

				MOVDQA		XMM3,XMM2
				PSLLW		XMM2,8
				PSRLW		XMM2,8
				PACKUSWB	XMM2,XMM2
				PSRLW		XMM3,8
				PACKUSWB	XMM3,XMM3




				MOVSD		[EAX],XMM1
				MOVD		[EDI],XMM2
				MOVD		[EDX],XMM3

				ADD			ESI,16
				ADD			EAX,8
				ADD			EDX,4
				ADD			EDI,4
				SUB			ECX,1
				JMP			loop_16
loop_8:
			MOV			ECX,nM16
				TEST		ECX,0x00000008
				JZ			loop_4
				MOVSD		XMM1,[ESI]
				MOVSD		XMM2,XMM1
					PSLLW		XMM1,8
					PSRLW		XMM1,8
					PACKUSWB	XMM1,XMM1
					PSRLW		XMM2,8
					PACKUSWB	XMM2,XMM2

					MOVDQA		XMM3,XMM2
					PSLLW		XMM2,8
					PSRLW		XMM2,8
					PACKUSWB	XMM2,XMM2
					PSRLW		XMM3,8
					PACKUSWB	XMM3,XMM3

					MOVD		[EAX],XMM1
					PUSH		EAX
					MOVD		EAX,XMM2
					MOV			[EDI],AX
					MOVD		EAX,XMM3
					MOV			[EDX],AX
					POP			EAX
					ADD			ESI,8
					ADD			EAX,4
					ADD			EDX,2
					ADD			EDI,2
loop_4:
				TEST		ECX,0x00000004
					JZ			loop_end
					MOVD		XMM1,[ESI]
					MOVDQA		XMM2,XMM1
						PSLLW		XMM1,8
						PSRLW		XMM1,8
						PACKUSWB	XMM1,XMM1
						PSRLW		XMM2,8
						PACKUSWB	XMM2,XMM2

						MOVDQA		XMM3,XMM2
						PSLLW		XMM2,8
						PSRLW		XMM2,8
						PACKUSWB	XMM2,XMM2
						PSRLW		XMM3,8
						PACKUSWB	XMM3,XMM3

						PUSH		EDX
						MOVD		EDX,XMM1
						MOV			[EAX],DX
						POP			EDX

						PUSH		EAX
						MOVD		EAX,XMM2
						MOV			[EDI],AL
						MOVD		EAX,XMM3
						MOV			[EDX],AL
						POP			EAX

						ADD			ESI,8
						ADD			EAX,4
						ADD			EDX,2
						ADD			EDI,2
loop_end:

	};
}

//////////////////////////////////////////////////////////////////////////
class RECT_F
{
public:
	float	top,bottom,left,right;		// 成员变量，请加m前缀

	RECT_F(int _top, int _bottom, int _left, int _right)
	{
		top = (float)_top;
		bottom = (float)_bottom;
		left = (float)_left;
		right = (float)_right;
	}

	RECT_F(const RECT& rcI)
	{
		top = (float)rcI.top;
		bottom = (float)rcI.bottom;
		left = (float)rcI.left;
		right = (float)rcI.right;
	}

	float Width() const
	{
		return right - left;
	}

	float Height() const
	{
		return bottom - top;
	}

	void Scale( float x, float y )
	{
		left *= x;
		right *= x;
		top *= y;
		bottom *= y;
	}

	void Offset( float x, float y )
	{
		left += x;
		right += x;
		top += y;
		bottom += y;
	}

	void SetRect( float _left, float _top, float _right, float _bottom )
	{
		top = _top;
		bottom = _bottom;
		left = _left;
		right = _right;
	}

	operator RECT()
	{
		RECT rtRet;
		rtRet.top = (long)(top+0.5f);
		rtRet.bottom = (long)(bottom+0.5f);
		rtRet.left = (long)(left+0.5f);
		rtRet.right = (long)(right+0.5f);
		return rtRet;		
	}

	RECT_F& IntersectRect(const RECT_F& rc)
	{
		top = max( top, rc.top );
		bottom = min( bottom, rc.bottom );
		left = max( left, rc.left );
		right = min( right, rc.right );

		return *this;
	}

	RECT_F& OffsetRect( float fOffsetX, float fOffsetY )
	{
		top += fOffsetY;
		bottom += fOffsetY;
		left += fOffsetX;
		right += fOffsetX;

		return *this;
	}
};

inline static long WidthRect(const RECT& rc)
{
	return rc.right - rc.left;
}

inline static long HeightRect(const RECT& rc)
{
	return rc.bottom - rc.top;
}

inline static void ScaleRect(RECT& rc, float scaleX, float scaleY)
{
	rc.left = (long)(rc.left * scaleX);
	rc.right = (long)(rc.right * scaleX);
	rc.top = (long)(rc.top * scaleY);
	rc.bottom = (long)(rc.bottom * scaleY);
}

//////////////////////////////////////////////////////////////////////////

void CALLBACK TP_ClipRectFromVideoStandard( TP_VIDEO_STANDARD fmt, const RECT& rcSrcBuffer, RECT& rc )
{
	rc.left = rc.top = 0;
	switch (fmt)
	{
	case TP_VIDEOSTANDARD_PAL:
	case TP_VIDEOSTANDARD_PAL_16_9:
	case TP_VIDEOSTANDARD_PAL_25P:
	case TP_VIDEOSTANDARD_PAL_25P_16_9:
	case TP_VIDEOSTANDARD_PAL_50P:
	case TP_VIDEOSTANDARD_PAL_50P_16_9:

		rc.right = 720, rc.bottom = 576;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997:
	case TP_VIDEOSTANDARD_NTSC_30:
	case TP_VIDEOSTANDARD_NTSC_2997_16_9:
	case TP_VIDEOSTANDARD_NTSC_30_16_9:
	case TP_VIDEOSTANDARD_NTSC_2997P:
	case TP_VIDEOSTANDARD_NTSC_2997P_16_9:
	case TP_VIDEOSTANDARD_NTSC_5994P:
	case TP_VIDEOSTANDARD_NTSC_5994P_16_9:
		rc.right = 720, rc.bottom = 486;
		break;
	case TP_VIDEOSTANDARD_1920_1080_50i:
	case TP_VIDEOSTANDARD_1920_1080_5994i:
	case TP_VIDEOSTANDARD_1920_1080_60i:
	case TP_VIDEOSTANDARD_1920_1080_2398p:
	case TP_VIDEOSTANDARD_1920_1080_24p:
	case TP_VIDEOSTANDARD_1920_1080_25p:
	case TP_VIDEOSTANDARD_1920_1080_2997p:
	case TP_VIDEOSTANDARD_1920_1080_50p:
	case TP_VIDEOSTANDARD_1920_1080_5994p:
	case TP_VIDEOSTANDARD_1920_1080_30p:
		rc.right = 1920, rc.bottom = 1080;
		break;
	case TP_VIDEOSTANDARD_1280_720_2398p:
	case TP_VIDEOSTANDARD_1280_720_24p:
	case TP_VIDEOSTANDARD_1280_720_25p:
	case TP_VIDEOSTANDARD_1280_720_50p:
	case TP_VIDEOSTANDARD_1280_720_2997p:
	case TP_VIDEOSTANDARD_1280_720_5994p:
		rc.right = 1280, rc.bottom = 720;
		break;
	case TP_VIDEOSTANDARD_1280_1080_50i:
	case TP_VIDEOSTANDARD_1280_1080_5994i:
	case TP_VIDEOSTANDARD_1280_1080_60i:
		rc.right = 1280, rc.bottom = 1080;
		break;
	case TP_VIDEOSTANDARD_1440_1080_50i:
	case TP_VIDEOSTANDARD_1440_1080_5994i:
	case TP_VIDEOSTANDARD_1440_1080_60i:
	case TP_VIDEOSTANDARD_1440_1080_50p:
	case TP_VIDEOSTANDARD_1440_1080_5994p:
	case TP_VIDEOSTANDARD_1440_1080_25p:
	case TP_VIDEOSTANDARD_1440_1080_2997p:
	rc.right = 1440, rc.bottom = 1080;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997_480:
	case TP_VIDEOSTANDARD_NTSC_30_480:
		rc.right = 720, rc.bottom = 480;
		break;
	case TP_VIDEOSTANDARD_UNKNOW:
	default:
		rc = rcSrcBuffer;
		break;
	}
}

void CALLBACK TP_ClipRectFromVideoStandard_Proxy( TP_VIDEO_STANDARD fmt, RECT& rc )
{
	rc.left = rc.top = 0;
	switch (fmt)
	{
	case TP_VIDEOSTANDARD_PAL:
	case TP_VIDEOSTANDARD_PAL_16_9:
	case TP_VIDEOSTANDARD_PAL_25P:
	case TP_VIDEOSTANDARD_PAL_25P_16_9:
	case TP_VIDEOSTANDARD_PAL_50P:
	case TP_VIDEOSTANDARD_PAL_50P_16_9:

		rc.right = 360, rc.bottom = 288;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997:
	case TP_VIDEOSTANDARD_NTSC_30:
	case TP_VIDEOSTANDARD_NTSC_2997_16_9:
	case TP_VIDEOSTANDARD_NTSC_30_16_9:
	case TP_VIDEOSTANDARD_NTSC_2997P:
	case TP_VIDEOSTANDARD_NTSC_2997P_16_9:
	case TP_VIDEOSTANDARD_NTSC_5994P:
	case TP_VIDEOSTANDARD_NTSC_5994P_16_9:

		rc.right = 360, rc.bottom = 243;
		break;
	case TP_VIDEOSTANDARD_1920_1080_50i:
	case TP_VIDEOSTANDARD_1920_1080_5994i:
	case TP_VIDEOSTANDARD_1920_1080_60i:
	case TP_VIDEOSTANDARD_1920_1080_2398p:
	case TP_VIDEOSTANDARD_1920_1080_24p:
	case TP_VIDEOSTANDARD_1920_1080_25p:
	case TP_VIDEOSTANDARD_1920_1080_5994p:
	case TP_VIDEOSTANDARD_1920_1080_50p:
	case TP_VIDEOSTANDARD_1920_1080_2997p:
	case TP_VIDEOSTANDARD_1920_1080_30p:
		rc.right = 480, rc.bottom = 270;
		break;
	case TP_VIDEOSTANDARD_1280_720_2398p:
	case TP_VIDEOSTANDARD_1280_720_24p:
	case TP_VIDEOSTANDARD_1280_720_25p:
	case TP_VIDEOSTANDARD_1280_720_50p:
	case TP_VIDEOSTANDARD_1280_720_2997p:
	case TP_VIDEOSTANDARD_1280_720_5994p:
		rc.right = 1280, rc.bottom = 720;					// ?
		break;
	case TP_VIDEOSTANDARD_1280_1080_50i:
	case TP_VIDEOSTANDARD_1280_1080_5994i:
	case TP_VIDEOSTANDARD_1280_1080_60i:
		rc.right = 480, rc.bottom = 270;
		break;
	case TP_VIDEOSTANDARD_1440_1080_50i:
	case TP_VIDEOSTANDARD_1440_1080_5994i:
	case TP_VIDEOSTANDARD_1440_1080_60i:
	case TP_VIDEOSTANDARD_1440_1080_50p:
	case TP_VIDEOSTANDARD_1440_1080_5994p:
	case TP_VIDEOSTANDARD_1440_1080_25p:
	case TP_VIDEOSTANDARD_1440_1080_2997p:
		rc.right = 480, rc.bottom = 270;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997_480:
	case TP_VIDEOSTANDARD_NTSC_30_480:
	default:
		rc.right = 720, rc.bottom = 480;					// ?
		break;
	}
}

void CALLBACK TP_ClipRectFromVideoStandard_Proxy_XDCAM( TP_VIDEO_STANDARD fmt, RECT& rc )
{
	rc.left = rc.top = 0;
	switch (fmt)
	{
	case TP_VIDEOSTANDARD_PAL:
	case TP_VIDEOSTANDARD_PAL_16_9:
	case TP_VIDEOSTANDARD_PAL_25P:
	case TP_VIDEOSTANDARD_PAL_50P:
	case TP_VIDEOSTANDARD_PAL_25P_16_9:
	case TP_VIDEOSTANDARD_PAL_50P_16_9:
		rc.right = 352/*360*/, rc.bottom = 288/*288*/;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997:
	case TP_VIDEOSTANDARD_NTSC_30:
	case TP_VIDEOSTANDARD_NTSC_2997_16_9:
	case TP_VIDEOSTANDARD_NTSC_30_16_9:
	case TP_VIDEOSTANDARD_NTSC_2997P:
	case TP_VIDEOSTANDARD_NTSC_5994P:
	case TP_VIDEOSTANDARD_NTSC_2997P_16_9:
	case TP_VIDEOSTANDARD_NTSC_5994P_16_9:

		rc.right = 352/*360*/, rc.bottom = 240/*243*/;
		break;
	case TP_VIDEOSTANDARD_1920_1080_50i:
	case TP_VIDEOSTANDARD_1920_1080_5994i:
	case TP_VIDEOSTANDARD_1920_1080_60i:
	case TP_VIDEOSTANDARD_1920_1080_2398p:
	case TP_VIDEOSTANDARD_1920_1080_24p:
	case TP_VIDEOSTANDARD_1920_1080_25p:
	case TP_VIDEOSTANDARD_1920_1080_2997p:
	case TP_VIDEOSTANDARD_1920_1080_5994p:
	case TP_VIDEOSTANDARD_1920_1080_50p:
	case TP_VIDEOSTANDARD_1920_1080_30p:
		rc.right = 352/*480*/, rc.bottom = 240/*270*/;
		break;
	case TP_VIDEOSTANDARD_1280_720_2398p:
	case TP_VIDEOSTANDARD_1280_720_24p:
	case TP_VIDEOSTANDARD_1280_720_25p:
	case TP_VIDEOSTANDARD_1280_720_50p:
	case TP_VIDEOSTANDARD_1280_720_2997p:
	case TP_VIDEOSTANDARD_1280_720_5994p:
		rc.right = 1280, rc.bottom = 720;					// ?
		break;
	case TP_VIDEOSTANDARD_1280_1080_50i:
	case TP_VIDEOSTANDARD_1280_1080_5994i:
	case TP_VIDEOSTANDARD_1280_1080_60i:
		rc.right = 352/*480*/, rc.bottom = 240/*270*/;
		break;
	case TP_VIDEOSTANDARD_1440_1080_50i:
	case TP_VIDEOSTANDARD_1440_1080_5994i:
	case TP_VIDEOSTANDARD_1440_1080_60i:
	case TP_VIDEOSTANDARD_1440_1080_50p:
	case TP_VIDEOSTANDARD_1440_1080_5994p:
	case TP_VIDEOSTANDARD_1440_1080_25p:
	case TP_VIDEOSTANDARD_1440_1080_2997p:

		rc.right = 352/*480*/, rc.bottom = 240/*270*/;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997_480:
	case TP_VIDEOSTANDARD_NTSC_30_480:
	default:
		rc.right = 720, rc.bottom = 480;					// ?
		break;
	}
}

void CALLBACK TP_ClipRectFromVideoStandard_mov( TP_VIDEO_STANDARD fmt, const RECT& rcOri, RECT& rc )
{
	rc.left = rc.top = 0;
	switch (fmt)
	{
	case TP_VIDEOSTANDARD_PAL:
	case TP_VIDEOSTANDARD_PAL_25P:
	case TP_VIDEOSTANDARD_PAL_50P:

		rc.right = 720, rc.bottom = 576;
		break;
	case TP_VIDEOSTANDARD_PAL_16_9:
	case TP_VIDEOSTANDARD_PAL_25P_16_9:
	case TP_VIDEOSTANDARD_PAL_50P_16_9:

		rc.right = 576 * 16 / 9, rc.bottom = 576;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997:
	case TP_VIDEOSTANDARD_NTSC_30:
	case TP_VIDEOSTANDARD_NTSC_2997P:
	case TP_VIDEOSTANDARD_NTSC_5994P:

		rc.right = 720, rc.bottom = 486;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997_16_9:
	case TP_VIDEOSTANDARD_NTSC_30_16_9:
	case TP_VIDEOSTANDARD_NTSC_2997P_16_9:
	case TP_VIDEOSTANDARD_NTSC_5994P_16_9:

		rc.right = 486 * 16 / 9, rc.bottom = 486;
		break;
	case TP_VIDEOSTANDARD_1920_1080_50i:
	case TP_VIDEOSTANDARD_1920_1080_5994i:
	case TP_VIDEOSTANDARD_1920_1080_60i:
	case TP_VIDEOSTANDARD_1920_1080_2398p:
	case TP_VIDEOSTANDARD_1920_1080_24p:
	case TP_VIDEOSTANDARD_1920_1080_25p:
	case TP_VIDEOSTANDARD_1920_1080_2997p:
	case TP_VIDEOSTANDARD_1920_1080_30p:
	case TP_VIDEOSTANDARD_1920_1080_50p:
	case TP_VIDEOSTANDARD_1920_1080_5994p:

		rc.right = 1920, rc.bottom = 1080;
		break;
	case TP_VIDEOSTANDARD_1280_720_2398p:
	case TP_VIDEOSTANDARD_1280_720_24p:
	case TP_VIDEOSTANDARD_1280_720_25p:
	case TP_VIDEOSTANDARD_1280_720_50p:
	case TP_VIDEOSTANDARD_1280_720_2997p:
	case TP_VIDEOSTANDARD_1280_720_5994p:
		rc.right = 1280, rc.bottom = 720;
		break;
	case TP_VIDEOSTANDARD_1280_1080_50i:
	case TP_VIDEOSTANDARD_1280_1080_5994i:
	case TP_VIDEOSTANDARD_1280_1080_60i:
	case TP_VIDEOSTANDARD_1440_1080_50p:
	case TP_VIDEOSTANDARD_1440_1080_5994p:
	case TP_VIDEOSTANDARD_1440_1080_25p:
	case TP_VIDEOSTANDARD_1440_1080_2997p:
		rc.right = 1920, rc.bottom = 1080;
		break;

	case TP_VIDEOSTANDARD_1440_1080_50i:
	case TP_VIDEOSTANDARD_1440_1080_5994i:
	case TP_VIDEOSTANDARD_1440_1080_60i:
		//rc.right = 1440, rc.bottom = 1080;
		rc.right = 1920, rc.bottom = 1080;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997_480:
	case TP_VIDEOSTANDARD_NTSC_30_480:
		rc.right = 720, rc.bottom = 480;
		break;
	case TP_VIDEOSTANDARD_UNKNOW:
	default:
		rc = rcOri;
		break;
	}
}

void CALLBACK TP_ClipRectFromVideoStandard_Adj( TP_VIDEO_STANDARD fmt, const RECT& rcOri, RECT& rc )
{
	rc.left = rc.top = 0;
	switch (fmt)
	{
	case TP_VIDEOSTANDARD_PAL:
	case TP_VIDEOSTANDARD_PAL_25P:
	case TP_VIDEOSTANDARD_PAL_50P:

		rc.right = 576*4/3, rc.bottom = 576;	// 要看作4：3
		break;
	case TP_VIDEOSTANDARD_PAL_16_9:
	case TP_VIDEOSTANDARD_PAL_25P_16_9:
	case TP_VIDEOSTANDARD_PAL_50P_16_9:

		{
			rc.bottom = 576;
			rc.right = 576 * 16 / 9;			// 要看作16:9
		}
		break;
	case TP_VIDEOSTANDARD_NTSC_2997:
	case TP_VIDEOSTANDARD_NTSC_30:
	case TP_VIDEOSTANDARD_NTSC_2997P:
	case TP_VIDEOSTANDARD_NTSC_5994P:

		rc.right = /*720*/486*4/3, rc.bottom = 486;
		break;
	case TP_VIDEOSTANDARD_NTSC_2997_16_9:
	case TP_VIDEOSTANDARD_NTSC_30_16_9:
	case TP_VIDEOSTANDARD_NTSC_2997P_16_9:
	case TP_VIDEOSTANDARD_NTSC_5994P_16_9:

		rc.right = /*720*/486*16/9, rc.bottom = 486;
		break;
	case TP_VIDEOSTANDARD_1920_1080_50i:
	case TP_VIDEOSTANDARD_1920_1080_5994i:
	case TP_VIDEOSTANDARD_1920_1080_60i:
	case TP_VIDEOSTANDARD_1920_1080_2398p:
	case TP_VIDEOSTANDARD_1920_1080_24p:
	case TP_VIDEOSTANDARD_1920_1080_25p:
	case TP_VIDEOSTANDARD_1920_1080_2997p:
	case TP_VIDEOSTANDARD_1920_1080_30p:
	case TP_VIDEOSTANDARD_1920_1080_50p:
	case TP_VIDEOSTANDARD_1920_1080_5994p:

		rc.right = 1920, rc.bottom = 1080;
		break;
	case TP_VIDEOSTANDARD_1280_720_2398p:
	case TP_VIDEOSTANDARD_1280_720_24p:
	case TP_VIDEOSTANDARD_1280_720_25p:
	case TP_VIDEOSTANDARD_1280_720_50p:
	case TP_VIDEOSTANDARD_1280_720_5994p:
	case TP_VIDEOSTANDARD_1280_720_2997p:
		rc.right = 1280, rc.bottom = 720;
		break;
	case TP_VIDEOSTANDARD_1280_1080_50i:
	case TP_VIDEOSTANDARD_1280_1080_5994i:
	case TP_VIDEOSTANDARD_1280_1080_60i:
		rc.right = 1920, rc.bottom = 1080;			// 要看作16:9
		break;
	case TP_VIDEOSTANDARD_1440_1080_50i:
	case TP_VIDEOSTANDARD_1440_1080_5994i:
	case TP_VIDEOSTANDARD_1440_1080_60i:
	case TP_VIDEOSTANDARD_1440_1080_50p:
	case TP_VIDEOSTANDARD_1440_1080_5994p:
	case TP_VIDEOSTANDARD_1440_1080_25p:
	case TP_VIDEOSTANDARD_1440_1080_2997p:

		rc.right = 1920, rc.bottom = 1080;			// 要看作16:9
		break;
	case TP_VIDEOSTANDARD_NTSC_2997_480:
	case TP_VIDEOSTANDARD_NTSC_30_480:
		rc.right = 720, rc.bottom = 480;
		break;
	case TP_VIDEOSTANDARD_UNKNOW:
	default:
		{
			rc = rcOri;	// 要看作4：3;
		}
		break;
	}
} 

inline static void CropRectAsModel(RECT& rcToAdj, const RECT& rcDstImage, const RECT& rcDstImageOld)
{
	if( IsRectEmpty(&rcDstImage) || IsRectEmpty(&rcDstImageOld) )
	{
		rcToAdj.right = rcToAdj.left;
		rcToAdj.bottom = rcToAdj.top;
		return;
	}

	RECT rcDstImageOri = rcDstImage;
	OffsetRect( &rcDstImageOri, -rcDstImageOld.left, -rcDstImageOld.top );

	int srcBufferWidth = rcToAdj.right - rcToAdj.left;
	int srcBufferHeight = rcToAdj.bottom - rcToAdj.top;
	int dstOldWidth = rcDstImageOld.right - rcDstImageOld.left;
	int dstOldHeight = rcDstImageOld.bottom - rcDstImageOld.top;

	int left = rcDstImageOri.left * srcBufferWidth / dstOldWidth;
	int right = rcDstImageOri.right * srcBufferWidth / dstOldWidth;
	int top = rcDstImageOri.top * srcBufferHeight / dstOldHeight;
	int bottom = rcDstImageOri.bottom * srcBufferHeight / dstOldHeight;

	int oriLeft = rcToAdj.left;
	int oriTop = rcToAdj.top;

	SetRect( &rcToAdj, left + oriLeft, top + oriTop, right + oriLeft, bottom + oriTop );
}

inline static void CropRectAsModel(RECT_F& rcSrc_buffer_croped, const RECT_F& rcDstImage, const RECT_F& rcDstImageOld)
{
	RECT_F rcDstImageOri = rcDstImage;
	rcDstImageOri.Offset( -rcDstImageOld.left, -rcDstImageOld.top );
	float left = rcDstImageOri.left * rcSrc_buffer_croped.Width() / rcDstImageOld.Width();
	float right = rcDstImageOri.right * rcSrc_buffer_croped.Width() / rcDstImageOld.Width();
	float top = rcDstImageOri.top * rcSrc_buffer_croped.Height() / rcDstImageOld.Height();
	float bottom = rcDstImageOri.bottom * rcSrc_buffer_croped.Height() / rcDstImageOld.Height();

	float oriLeft = rcSrc_buffer_croped.left;
	float oriTop = rcSrc_buffer_croped.top;

	rcSrc_buffer_croped.SetRect( left + oriLeft, top + oriTop, right + oriLeft, bottom + oriTop );
}

inline static void ImageAdapt( const RECT_F& fullSrc, const RECT_F& fullDst, 
							  TPTLTrimViewAdaptPara type, const RECT_F& imageSrc, RECT_F& imageDst )
{
	assert( fullSrc.left ==0 && fullSrc.top == 0 );
	assert( fullDst.left ==0 && fullDst.top == 0 );
	assert( fullSrc.right > 0 );
	assert( fullSrc.bottom > 0 );

	// 计算缩放量
	float scaleX = 1.f, scaleY = 1.f;
	float srcWidth = max( fullSrc.Width(), 1.f );
	float srcHeight = max( fullSrc.Height(), 1.f );
	float dstWidth = fullDst.Width();
	float dstHeight = fullDst.Height();
	switch ( type.eMode )
	{
	case _TP_HDSD_NoChange:
		break;
	case _TP_HDSD_Stretch:
		scaleX = dstWidth / srcWidth;
		scaleY = dstHeight / srcHeight;
		break;
	case _TP_HDSD_CutHorz:
		scaleX = scaleY = dstWidth / srcWidth;
		break;
	case _TP_HDSD_EstopVert:
		scaleX = scaleY = dstHeight / srcHeight;
		break;
	case _TP_HDSD_Custom:
		scaleX = scaleY = type.lSpecify / 100.f;
		break;
	default:
		scaleX = dstWidth / srcWidth;
		scaleY = dstHeight / srcHeight;
		break;
	}

	// 将源放到中心
	imageDst = imageSrc;
	imageDst.Offset( -fullSrc.Width()/2.f, -fullSrc.Height()/2.f );

	// 缩放
	imageDst.Scale(scaleX,scaleY);

	// 将坐标变换到目标坐标系下
	imageDst.Offset( fullDst.Width()/2.f, fullDst.Height()/2.f );
}


void CALLBACK TP_RectAdapt_Impl( const RECT& rcSrcFull, const RECT& rcDstFull,
								RECT& rcSrcImage, RECT& rcDstImage,
								_TP_HDSD_MODE eMode, long lSpecify,
								float fOffsetX, float fOffsetY )
{
	RECT_F rcSrcImage_F = rcSrcImage;
	RECT_F rcDstImage_F = rcDstImage;
	RECT_F rcSrcFullAdj_F = rcSrcFull;
	RECT_F rcDstFullAdj_F = rcDstFull;
	rcSrcFullAdj_F.Offset(-rcSrcFullAdj_F.left,-rcSrcFullAdj_F.top);
	rcDstFullAdj_F.Offset(-rcDstFullAdj_F.left,-rcDstFullAdj_F.top);

	TPTLTrimViewAdaptPara type;
	type.eMode = eMode;
	type.lSpecify = lSpecify;

	// compute dst image rect
	ImageAdapt(rcSrcFullAdj_F,rcDstFullAdj_F,type,rcSrcImage_F,rcDstImage_F);

	rcDstImage_F.OffsetRect(fOffsetX, fOffsetY);	// offset dst rect ( center x&y )
	// crop dst rect
	RECT_F rcDstImageOld_F = rcDstImage_F;
	rcDstImage_F.IntersectRect( rcDstFullAdj_F );

	// crop src rect
	CropRectAsModel(rcSrcImage_F, rcDstImage_F, rcDstImageOld_F);

	rcSrcImage = rcSrcImage_F;
	rcDstImage = rcDstImage_F;

	OffsetRect(&rcSrcImage,rcSrcFull.left,rcSrcFull.top);
	OffsetRect(&rcDstImage,rcDstFull.left,rcDstFull.top);
}

static DWORD dwColorBlackYUVA = 0x00000000;			// ARGB
static DWORD dwColorBlackYUYV = 0x00000000;			// VYUY

class CTP_VideoAdaptException
{
public:
	CTP_VideoAdaptException()
	{
		m_bChanged = false;
	}
	void	BeginAdaptExpception( _TPTLClipHDSDChange& adaptData )
	{
		m_dataApaptBackup = adaptData;
		if( /*adaptData.eAdaptException == ADAPTE_480_486 ||*/ adaptData.lAdaptFlag & TP_ADAPTFLAG_480_486 )
		{
			//assert( 720 == RECTWIDTH(adaptData.rcSrc_buffer) );
			assert( 480 == RECTHEIGHT(adaptData.rcSrc_buffer) );
			adaptData.szSrc.cy = 486;
			adaptData.rcSrc_buffer.top = 0;
			adaptData.rcSrc_buffer.bottom = 486;
			adaptData.pSrcBuffer = new BYTE[adaptData.szSrc.cx*adaptData.szSrc.cy*4];
			m_bChanged = true;

			DWORD dwColorBlack = 0x00000000;

			TP_Clear( (DWORD*)adaptData.pSrcBuffer, adaptData.szSrc.cx*4, 
				0, 0, adaptData.szSrc.cx, 4, dwColorBlack );
			TP_Clear( (DWORD*)adaptData.pSrcBuffer, adaptData.szSrc.cx*4, 
				0, adaptData.szSrc.cy-2, adaptData.szSrc.cx, 2, dwColorBlack );

			TP_BitBlt( (DWORD*)adaptData.pSrcBuffer, 0, 4, adaptData.szSrc.cx, adaptData.szSrc.cy-6, 
				adaptData.szSrc.cx, adaptData.szSrc.cy,
				(DWORD*)m_dataApaptBackup.pSrcBuffer, 
				m_dataApaptBackup.rcSrc_buffer.left,
				m_dataApaptBackup.rcSrc_buffer.top,
				RECTWIDTH(m_dataApaptBackup.rcSrc_buffer),
				RECTHEIGHT(m_dataApaptBackup.rcSrc_buffer),
				m_dataApaptBackup.szSrc.cx, m_dataApaptBackup.szSrc.cy,
				TP_BUFFER_COPY, TP_PARITY_BOTH );
		}
	}

	void	EndAdaptException( _TPTLClipHDSDChange& adaptData )
	{
		if( m_bChanged )
		{
			delete[] adaptData.pSrcBuffer;
			adaptData = m_dataApaptBackup;		
			m_bChanged = false;
		}
	}

private:
	_TPTLClipHDSDChange	m_dataApaptBackup;
	bool	m_bChanged;
};

LRESULT CALLBACK TP_BufferAdapte(
								 IN OUT BYTE* pbyDestBuffer, 
								 IN int nDestBufferWidth,
								 IN int nDestBufferHeight,
								 IN BYTE* pbySrcBuffer,
								 IN int nSrcBufferWidth,		
								 IN int nSrcBufferHeight,
								 IN RECT rtAdapte,
								 IN _TP_HDSD_MODE eMode,
								 IN long lSpecify,
								 IN OUT RECT& rtDesAdapte)
{
	RECT rcSrcFull = {0,0,nSrcBufferWidth,nSrcBufferHeight};
	RECT rcDstFull = {0,0,nDestBufferWidth,nDestBufferHeight};
	RECT rcSrcImage= rtAdapte;
	RECT rcDstImage= rtAdapte; 
	TP_RectAdapt_Impl(rcSrcFull,rcDstFull,rcSrcImage,rcDstImage,eMode,lSpecify,0,0);
	rtDesAdapte =rcDstImage;

	return TP_BitBlt(
		(DWORD*)pbyDestBuffer,
		rcDstImage.left,
		rcDstImage.top,
		RECTWIDTH(rcDstImage),
		RECTHEIGHT(rcDstImage),
		nDestBufferWidth,
		nDestBufferHeight,
		(DWORD*)pbySrcBuffer,
		rcSrcImage.left,
		rcSrcImage.top,
		RECTWIDTH(rcSrcImage),
		RECTHEIGHT(rcSrcImage),
		nSrcBufferWidth,
		nSrcBufferHeight,
		TP_BUFFER_COPY,
		TP_PARITY_BOTH
		);
}
LRESULT CALLBACK TP_VideoAdapte( _TPTLClipHDSDChange &adaptParam )
{
	if ( adaptParam.bYUV16 == TRUE )
	{
		adaptParam.szSrc.cx /= 2;
		adaptParam.rcSrc_buffer.left /= 2;
		adaptParam.rcSrc_buffer.right /= 2;
		adaptParam.szDest.cx /= 2;
	}

	// change the src buffer and dize
	CTP_VideoAdaptException adpatVideoException;
	adpatVideoException.BeginAdaptExpception(adaptParam);

	RECT rcSrc,rcDst,rcDstFull;
	TP_RectAdapt(adaptParam,rcSrc,rcDst,&rcDstFull);

	if ( adaptParam.bYUV16 == TRUE )
	{
		rcDst.left /= 2;
		rcDst.right /= 2;
	}

	RECT rcDstBuffer;
	rcDstBuffer.left = rcDstBuffer.top = 0;
	rcDstBuffer.right = adaptParam.szDest.cx;
	rcDstBuffer.bottom = adaptParam.szDest.cy;
	CropRectAsModel(rcDstBuffer,rcDst,rcDstFull);
	rcDst = rcDstBuffer;

	// data fill
	LRESULT hRlt =  TP_StretchBlt_Mean( (DWORD*)(adaptParam.pDestBuffer), rcDst.left, rcDst.top, 
		RECTWIDTH(rcDst), RECTHEIGHT(rcDst),
		adaptParam.szDest.cx, adaptParam.szDest.cy,
		(DWORD*)(adaptParam.pSrcBuffer), rcSrc.left, rcSrc.top, 
		RECTWIDTH(rcSrc), RECTHEIGHT(rcSrc),
		adaptParam.szSrc.cx, adaptParam.szSrc.cy,
		TP_BUFFER_COPY, TP_PARITY_BOTH
		);

	adpatVideoException.EndAdaptException(adaptParam);

	return hRlt == S_OK;

}

void CALLBACK TP_RectAdapt( _TPTLClipHDSDChange &adaptParam, RECT& rcSrc, RECT& rcDst, RECT* pRcDstFull )
{
	/*
	TOMODIFY
	*/
	if( adaptParam.eType & (TP_MEDIA_CLASS_V_P|TP_MEDIA_CLASS_V_G|TP_MEDIA_CLASS_V_SERIAL) )	// CG
	{
		RECT rcDestBuffer;
		rcDestBuffer.left = rcDestBuffer.top = 0;
		rcDestBuffer.right = adaptParam.szDest.cx;
		rcDestBuffer.bottom = adaptParam.szDest.cy;

		// 取得时间线尺寸
		RECT rcTL;
		TP_ClipRectFromVideoStandard( adaptParam.eTLVideoStand, rcDestBuffer, rcTL );
		RECT rcTLChanged;
		TP_ClipRectFromVideoStandard_mov( adaptParam.eTLVideoStand, rcDestBuffer, rcTLChanged );

		// 取得buffer尺寸
		RECT rcSrcImage = adaptParam.rcSrc_buffer;
		if( adaptParam.lAdaptFlag & TP_ADAPTFLAG_1609 )			// 16:9源的特殊处理
		{
			int iHeight = rcSrcImage.bottom - rcSrcImage.top;
			int iWidth = (int)(iHeight * 16 / 9.f + 0.5f);
			rcSrcImage.right = rcSrcImage.left + iWidth;
		}		

		int oriLeft = rcSrcImage.left;
		int oriTop = rcSrcImage.top;
		int oriWidth = rcSrcImage.right - rcSrcImage.left;
		int oriHeight = rcSrcImage.bottom - rcSrcImage.top;


		if ( adaptParam.bYUV16 == TRUE )
		{
			rcTL.right /= 2;
			rcTL.left /= 2;
			rcTLChanged.left /= 2;
			rcTLChanged.right /= 2;
		}

		// 令buffer始终居于时间线中间
		OffsetRect(&rcSrcImage,-oriLeft,-oriTop);
		OffsetRect(&rcSrcImage,(int)((rcTLChanged.right-oriWidth)/2.f+0.5f),(int)((rcTLChanged.bottom-oriHeight)/2.f+0.5f));

		// 16：9时间线的比例调整
		RECT rcSrcImageAdjusted = rcSrcImage;
		rcSrcImage = rcTL;		
		CropRectAsModel(rcSrcImage, rcSrcImageAdjusted, rcTLChanged);

		if ( adaptParam.bYUV16 == TRUE )
		{
			rcTL.right *= 2;
			rcTL.left *= 2;
			rcTLChanged.left *= 2;
			rcTLChanged.right *= 2;
			rcSrcImage.left *= 2;
			rcSrcImage.right *= 2;
		}

		// 源取全部，到目标中间
		rcSrc = adaptParam.rcSrc_buffer;
		rcDst = rcSrcImage;
		if ( pRcDstFull )
		{
			*pRcDstFull = rcTL;
		}

		return;
	}

	// 把NoChange看作100%缩放处理
	_TP_HDSD_MODE eMode = (_TP_HDSD_MODE)(adaptParam.eHDSDMode);
	long lSpecify = adaptParam.lSpecify;
	if( eMode == _TP_HDSD_NoChange )
	{
		eMode = _TP_HDSD_Custom;
		lSpecify = 100;
	}

	RECT rcClip,rcTL;
	rcClip.left = rcClip.top = 0;
	rcClip.right = adaptParam.szSrc.cx;
	rcClip.bottom = adaptParam.szSrc.cy;

	RECT rcDestBuffer;
	rcDestBuffer.left = rcDestBuffer.top = 0;
	rcDestBuffer.right = adaptParam.szDest.cx;
	rcDestBuffer.bottom = adaptParam.szDest.cy;


	// 源要以这个尺寸来计算适配
	RECT rcClipChanged;
	TP_ClipRectFromVideoStandard_Adj( adaptParam.eClipVideoStand, rcClip, rcClipChanged );   

	// 取得素材尺寸和时间线尺寸
	TP_ClipRectFromVideoStandard( adaptParam.eTLVideoStand, rcDestBuffer, rcTL );

	// 取得时间线计算尺寸
	RECT rcTLChanged;
	if ( adaptParam.eClipVideoStand == TP_VIDEOSTANDARD_UNKNOW )
		TP_ClipRectFromVideoStandard_mov( adaptParam.eTLVideoStand, rcDestBuffer, rcTLChanged );
	else
		TP_ClipRectFromVideoStandard_Adj( adaptParam.eTLVideoStand, rcDestBuffer, rcTLChanged );

	// 适配，得到需要的素材位置和目标RECT（都是计算尺寸）
	RECT rcClipImage, rcTLImage;
	rcClipImage = rcClipChanged; 
	rcTLImage = rcTL;
	float fOffsetX = ((float)adaptParam.dCenterX - 0.5f) * RECTWIDTH(rcTLChanged);
	float fOffsetY = ((float)adaptParam.dCenterY - 0.5f) * RECTHEIGHT(rcTLChanged);
	TP_RectAdapt_Impl( rcClipChanged, rcTLChanged, rcClipImage, rcTLImage, eMode, lSpecify, fOffsetX, fOffsetY );

	// IMX50素材，512 -> crop 480 ( 512-32 ) -> add black 486 ( 480 + 6 ), 需要确认，源buffer代表的是哪一个阶段的数据。
	// todo
	// XDCAM素材, proxy下
	// 幅面352*240(16:9),其实是360*243(对应普通情况下的480*270)的一部分;也就是说，它是16:9幅面的一部分。
	// 幅面352*240(4:3)，其实是360*243的一部分(对应普通情况下的360*243);

	// 根据计算尺寸返回的结果，求得对应的原始素材RECT和时间线RECT
	RECT rcClipImageOri, rcTLImageOri;

	// XDCAM素材, proxy下
	if ( rcClip.right == 352 && rcClip.bottom == 240 )
	{
		RECT rcClipDummyOri = { 0, 0, 360, 243 }; 
		RECT rcClipDummy = rcClipDummyOri;
		CropRectAsModel( rcClipDummy, rcClipImage, rcClipChanged );		// 完整素材应该截取的大小
		rcTLImageOri = rcTL;
		CropRectAsModel( rcTLImageOri, rcTLImage, rcTLChanged );		// 完整时间线应该截取的大小

		RECT rcClipCroped = { 4, 2, 356, 242 };
		IntersectRect( &rcClipCroped, &rcClipDummy, &rcClipCroped );	// 部分素材应该截取的大小

		CropRectAsModel( rcTLImageOri ,rcClipCroped, rcClipDummy );		// 调整时间线截取大小

		rcClipImageOri = rcClipCroped;									// 源素材对应的区域
		rcClipImageOri.left -= 4;
		rcClipImageOri.right -= 4;
		rcClipImageOri.top -= 2;
		rcClipImageOri.bottom -= 2;
	}
	else
	{
		rcClipImageOri = rcClip;
		CropRectAsModel( rcClipImageOri, rcClipImage, rcClipChanged );
		rcTLImageOri = rcTL;
		CropRectAsModel( rcTLImageOri, rcTLImage, rcTLChanged );
	}

	rcSrc = rcClipImageOri;
	rcDst = rcTLImageOri;

	if ( pRcDstFull )
	{
		*pRcDstFull = rcTL;
	}
}

// 生成交叉颜色块的透明色背景
void TP_MakeTranslucentBack(OUT DWORD* pBuf,			// 背景 Buffer
							IN int nBufWidth,			// Buffer 大小
							IN int nBufHeight,
							IN int nBlockSize,			// 相间颜色块大小
							IN DWORD crOddBlock,		// 奇数列颜色块颜色
							IN DWORD crEvenBlock)		// 偶数列颜色块颜色
{
	if (pBuf == NULL)
		return;

	int nBlockCountH = nBufWidth / nBlockSize + 2;	// 确定一行最多需要的颜色块个数
	int nBlockCountV = nBufHeight / nBlockSize;		// 确定一列拥有的完整颜色块个数
	int nLineSize = nBlockCountH * nBlockSize;
	int na16 = (nBlockSize >> 2), nm16 = ((nBlockSize & 0x03) << 2);
	DWORD* pBlockLine = TPSSE_NEW(nLineSize, DWORD);
	DWORD* pTmpLine = pBlockLine;

	// 填充出相间图片
	for (int i=0; i<nBlockCountH; i++)
	{
		memset_dword_line(pTmpLine, (i & 0x01) ? crEvenBlock : crOddBlock, na16, nm16);
		pTmpLine += nBlockSize;
	}
	// 逐行填充背景
	na16 = (nBufWidth >> 2), nm16 = ((nBufWidth & 0x03) << 2);
	for (int i=0; i<nBlockCountV; i++)	// 先填充完整的颜色块
	{
		pTmpLine = (i & 0x01) ? (pBlockLine + nBlockSize) : pBlockLine;
		for (int k=0; k<nBlockSize; k++)
		{
			memcpy_line(pBuf, pTmpLine, na16, nm16);
			pBuf += nBufWidth;
		}
		nBufHeight -= nBlockSize;
	}
	pTmpLine = (nBlockCountV & 0x01) ? (pBlockLine + nBlockSize) : pBlockLine;
	for (int k=0; k<nBufHeight; k++)	// 填充最后一个不完整的颜色块
	{
		memcpy_line(pBuf, pTmpLine, na16, nm16);
		pBuf += nBufWidth;
	}
	TPSSE_FREE(pBlockLine);
}

_declspec(align(16)) DWORD g_dwComp_5[] = { 0x00100010, 0x00100010, 0x00100010, 0x00100010 };
_declspec(align(16)) DWORD g_dwMaskWLB[] = { 0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff };

void TPSSE_Blur3Line(OUT DWORD* pDest,          // 目标
                     IN DWORD* pSrcPrev,        // 上一行
                     IN DWORD* pSrcCurr,        // 当前行
                     IN DWORD* pSrcNext,        // 下一行
                     IN DWORD dwWeightPrev,     // 三个权重（都放大了 2^14 倍，总和为 1）
                     IN DWORD dwWeightCurr,     // 要求高 16 位与低 16 位相同，也即存储了两个相同地权重
                     IN DWORD dwWeightNext,
                     IN int na8,                // 一行的字节总数除以 8 的商和余数
                     IN int nm8)
{
    _asm
    {
        mov         ecx, na8;
        mov         edi, pDest;
        mov         eax, pSrcPrev;
        mov         esi, pSrcCurr;
        mov         edx, pSrcNext;
        pxor        xmm0, xmm0;
        movd        xmm5, dwWeightPrev;
        movd        xmm6, dwWeightCurr;
        movd        xmm7, dwWeightNext;
        shufps      xmm5, xmm5, 0x00;       // W W W W W W W W
        shufps      xmm6, xmm6, 0x00;
        shufps      xmm7, xmm7, 0x00;
        test        ecx, ecx;
        jnz         loop_a8;
        jmp         loop_m4;
loop_a8:
        movsd       xmm1, [eax];        // 0000 0000 ARGB ARGB
        movsd       xmm2, [esi];
        movsd       xmm3, [edx];
        movdqa      xmm4, g_dwComp_5;
        punpcklbw   xmm1, xmm0;         // 0A 0R 0G 0B 0A 0R 0G 0B
        punpcklbw   xmm2, xmm0;
        punpcklbw   xmm3, xmm0;
        psllw       xmm1, 0x07;         // 放大 7 位
        psllw       xmm2, 0x07;
        psllw       xmm3, 0x07;
        pmulhw      xmm1, xmm5;         // 还放大 5 位
        pmulhw      xmm2, xmm6;
        pmulhw      xmm3, xmm7;
        paddw       xmm1, xmm2;
        paddw       xmm3, xmm4;
        paddw       xmm1, xmm3;
        movdqa      xmm4, g_dwMaskWLB;
        psrlw       xmm1, 0x05;         // 恢复
        pand        xmm1, xmm4;
        packuswb    xmm1, xmm1;
        movlps      [edi], xmm1;
        add         eax, 0x08;
        add         esi, 0x08;
        add         edx, 0x08;
        add         edi, 0x08;
        sub         ecx, 0x01;
        jnz         loop_a8;
loop_m4:
        mov         ecx, nm8;
        test        ecx, 0x04;
        jz          loop_end;

        movss       xmm1, [eax];        // 0000 0000 0000 ARGB
        movss       xmm2, [esi];
        movss       xmm3, [edx];
        movdqa      xmm4, g_dwComp_5;
        punpcklbw   xmm1, xmm0;         // 00 00 00 00 0A 0R 0G 0B
        punpcklbw   xmm2, xmm0;
        punpcklbw   xmm3, xmm0;
        psllw       xmm1, 0x07;         // 放大 7 位
        psllw       xmm2, 0x07;
        psllw       xmm3, 0x07;
        pmulhw      xmm1, xmm5;         // 还放大 5 位
        pmulhw      xmm2, xmm6;
        pmulhw      xmm3, xmm7;
        paddw       xmm1, xmm2;
        paddw       xmm3, xmm4;
        paddw       xmm1, xmm3;
        movdqa      xmm4, g_dwMaskWLB;
        psrlw       xmm1, 0x05;         // 恢复
        pand        xmm1, xmm4;
        packuswb    xmm1, xmm1;
        movss       [edi], xmm1;
loop_end:
    }
}

DWORD* TP_VerticalBlur(DWORD* pSrcBuffer, int nWidth, int nHeight, int* pFactor)
{
    ASSERT(pFactor != NULL);

    DWORD* pNewBuffer = (DWORD*)_mm_malloc(nWidth * nHeight * sizeof(DWORD), 16);
    TP_DMemSet(pNewBuffer, 0x00000000, nWidth * nHeight);

    int na8 = (nWidth >> 1), nm8 = ((nWidth & 0x01) << 2);
    DWORD dwWeight[3] = { 0 };
    float fSumInv = 1.f / (pFactor[0] + pFactor[1] + pFactor[2]);
    DWORD dwTmp(0);
    for (int i=0; i<3; i++)
    {
        dwTmp = (DWORD)(pFactor[i] * fSumInv * (1 << 14) + 0.5f);
        dwWeight[i] = ((dwTmp << 16) | dwTmp);
    }

    // 处理首行象素
    DWORD* pSrcPreLine  = pSrcBuffer + nWidth;
    DWORD* pSrcCurLine  = pSrcBuffer;
    DWORD* pSrcNextLine = pSrcBuffer + nWidth;
    DWORD* pDesCurLine  = pNewBuffer;
    TPSSE_Blur3Line(pDesCurLine, pSrcPreLine, pSrcCurLine, pSrcNextLine, 
        dwWeight[0], dwWeight[1], dwWeight[2], na8, nm8);

    // 处理中间行
    pSrcPreLine      = pSrcBuffer;
    pSrcCurLine     += nWidth;
    pSrcNextLine    += nWidth;
    pDesCurLine     += nWidth;
    for (int i=1; i<nHeight-1; i++)
    {
        TPSSE_Blur3Line(pDesCurLine, pSrcPreLine, pSrcCurLine, pSrcNextLine,
            dwWeight[0], dwWeight[1], dwWeight[2], na8, nm8);

        pSrcPreLine  += nWidth;
        pSrcCurLine  += nWidth;
        pSrcNextLine += nWidth;
        pDesCurLine  += nWidth;
    }

    // 处理末行象素
    pSrcNextLine = pSrcPreLine;
    TPSSE_Blur3Line(pDesCurLine, pSrcPreLine, pSrcCurLine, pSrcNextLine, 
        dwWeight[0], dwWeight[1], dwWeight[2], na8, nm8);

    memcpy_s(pSrcBuffer,sizeof(DWORD)*nWidth*nHeight, pNewBuffer, sizeof(DWORD)*nWidth*nHeight);

    _mm_free(pNewBuffer);
    pNewBuffer = NULL;

    return pSrcBuffer;
}

