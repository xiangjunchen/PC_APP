//////////////////////////////////////////////////////////////////////////
// 描述：内存操作接口 SSE 优化
// 创建者：赵志猛
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TPBufferOperateSSE.h"
#include "TPBufferOperateDef.h"
#include "TPSSE2.h"


extern DWORD *g_AlphaGainTable;		// Alpha 增益表

//////////////////////////////////////////////////////////////////////////
// 不缩放时的范围剪切
// 范围无效时返回 FALSE 有效时返回 TRUE
//////////////////////////////////////////////////////////////////////////
BOOL TPFix_CutRange(IN OUT int &nDestX,			// 目标起点
					IN OUT int &nDestY,
					IN OUT int &nDestWidth,		// 目标范围
					IN OUT int &nDestHeight,
					IN int nDestBufferWidth,	// 目标 Buffer 大小
					IN int nDestBufferHeight,
					IN OUT int &nSrcX,			// 源起点
					IN OUT int &nSrcY,
					IN OUT int &nSrcWidth,		// 源范围
					IN OUT int &nSrcHeight,
					IN int nSrcBufferWidth,		// 源 Buffer 大小
					IN int nSrcBufferHeight)
{
	// 边界有效性检测
	if (nDestBufferWidth <= 0 || nDestBufferHeight <= 0
		|| nSrcBufferWidth <= 0	|| nSrcBufferHeight <= 0)
		return FALSE;

	if (nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight
		|| nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return FALSE;

	int nTmpWidth(0), nTmpHeight(0);

	// 剪切超出范围
	if (nDestX < 0)		// 目标左边界超出，拷贝范围进行右缩进
	{
		nDestWidth += nDestX; nSrcX -= nDestX; nDestX = 0;
	}
	if (nDestY < 0)		// 目标上边界超出，拷贝范围进行下缩进
	{
		nDestHeight += nDestY; nSrcY -= nDestY;	nDestY = 0;
	}
	if (nSrcX < 0)		// 源左边界超出，拷贝范围进行右缩进
	{
		nDestWidth += nSrcX; nDestX -= nSrcX; nSrcX = 0;
	}
	if (nSrcY < 0)		// 源上边界超出，拷贝范围进行下缩进
	{
		nDestHeight += nSrcY; nDestY -= nSrcY; nSrcY = 0;
	}
	nTmpWidth = nDestWidth + nSrcX;		// 考虑源的右边界
	nTmpHeight = nDestHeight + nSrcY;	// 考虑源的下边界
	if (nTmpWidth > nSrcBufferWidth)	// 源右边界超出，拷贝范围进行左缩进
	{
		nDestWidth -= (nTmpWidth - nSrcBufferWidth);
	}
	if (nTmpHeight > nSrcBufferHeight)	// 源下边界超出，拷贝范围进行上缩进
	{
		nDestHeight -= (nTmpHeight - nSrcBufferHeight);
	}
	nTmpWidth = nDestWidth + nDestX;	// 考虑目标的右边界
	nTmpHeight = nDestHeight + nDestY;	// 考虑目标的下边界
	if (nTmpWidth > nDestBufferWidth)	// 目标右边界超出，拷贝范围进行左缩进
	{
		nDestWidth -= (nTmpWidth - nDestBufferWidth);
	}
	if (nTmpHeight > nDestBufferHeight)	// 目标下边界超出，拷贝范围进行上缩进
	{
		nDestHeight -= (nTmpHeight - nDestBufferHeight);
	}
	if (nDestWidth <= 0 || nDestHeight <= 0)	// 拷贝范围无效
		return FALSE;

	nSrcWidth = nDestWidth;
	nSrcHeight = nDestHeight;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 奇偶场的范围处理
// 范围无效时返回 FALSE 有效时返回 TRUE
//////////////////////////////////////////////////////////////////////////
BOOL TPParity_Range(IN OUT int &nDestY,			// 目标起点
					OUT int &nDestAddWidth,		// 目标 Buffer 增量
					IN int nDestBufferWidth,	// 目标 Buffer 大小
					IN int nDestBufferHeight,
					IN OUT int &nSrcY,			// 源起点
					OUT int &nSrcAddWidth,		// 源 Buffer 增量
					IN int nSrcBufferWidth,		// 源 Buffer 大小
					IN int nSrcBufferHeight,
					OUT int &nLineInc,			// 行增量
					IN int nParity)				// 奇偶场类型
{
	switch (nParity)		// 根据奇偶场类型获得增量
	{
	case TP_PARITY_BOTH:		// 全场
		nDestAddWidth = nDestBufferWidth;
		nSrcAddWidth = nSrcBufferWidth;
		nLineInc = 1;
		break;

	case TP_PARITY_ODD:			// 奇场
		if ((nDestY % 2) == 1)		// 起点在偶场，则从下一行开始
		{
			nDestY++;
			nSrcY++;
			// 判断范围的有效性
			if (nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return FALSE;
		}
		nDestAddWidth = (nDestBufferWidth << 1);
		nSrcAddWidth = (nSrcBufferWidth << 1);
		nLineInc = 2;
		break;

	case TP_PARITY_EVEN:		// 偶场
		if ((nDestY % 2) == 0)		// 起点在奇场，则从下一行开始
		{
			nDestY++;
			nSrcY++;
			// 判断范围的有效性
			if (nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return FALSE;
		}
		nDestAddWidth = (nDestBufferWidth << 1);
		nSrcAddWidth = (nSrcBufferWidth << 1);
		nLineInc = 2;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer Copy 接口
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_Copy_SSE(OUT DWORD* pDest,			// 目标 Buffer
					 IN int nDestX,				// 目标起始点
					 IN int nDestY,
					 IN int nDestWidth,			// 目标 Mix 大小
					 IN int nDestHeight,
					 IN int nDestBufferWidth,	// 目标 Buffer 大小
					 IN int nDestBufferHeight,
					 IN DWORD* pSrc,			// 源 Buffer
					 IN int nSrcX,				// 源起始点
					 IN int nSrcY,
					 IN int nSrcWidth,			// 源 Mix 大小
					 IN int nSrcHeight,
					 IN int nSrcBufferWidth,	// 源 Buffer 大小
					 IN int nSrcBufferHeight,
					 IN int nParity)			// 奇偶场类型
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	// 获得有效的操作范围
	if (!TPFix_CutRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight))
		return FALSE;

	int nDestAddWidth(0), nSrcAddWidth(0), nLineInc(0);

	// 获得有效的奇偶场操作范围
	if (!TPParity_Range(nDestY, nDestAddWidth, nDestBufferWidth, nDestBufferHeight,
		nSrcY, nSrcAddWidth, nSrcBufferWidth, nSrcBufferHeight, nLineInc, nParity))
		return FALSE;

	DWORD* pDesLine = pDest + nDestY * nDestBufferWidth + nDestX;	// 目标 Buffer 起始地址
	DWORD* pSrcLine = pSrc + nSrcY * nSrcBufferWidth + nSrcX;		// 源 Buffer 起始地址
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);	// 拷贝行的对齐参数

	for (int i=0; i<nDestHeight; i+=nLineInc)
	{
		memcpy_line(pDesLine, pSrcLine, na16, nm16);
		pDesLine += nDestAddWidth;
		pSrcLine += nSrcAddWidth;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer Mix 接口
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_Mix_SSE(IN OUT DWORD* pDest,		// 目标 Buffer
					IN int nDestX,				// 目标起始点
					IN int nDestY,
					IN int nDestWidth,			// 目标 Mix 大小
					IN int nDestHeight,
					IN int nDestBufferWidth,	// 目标 Buffer 大小
					IN int nDestBufferHeight,
					IN DWORD* pSrc,				// 源 Buffer
					IN int nSrcX,				// 源起始点
					IN int nSrcY,
					IN int nSrcWidth,			// 源 Mix 大小
					IN int nSrcHeight,
					IN int nSrcBufferWidth,		// 源 Buffer 大小
					IN int nSrcBufferHeight,
					IN int nParity)				// 奇偶场类型
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	// 获得有效的操作范围
	if (!TPFix_CutRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight))
			return FALSE;

	int nDestAddWidth(0), nSrcAddWidth(0), nLineInc(0);

	// 获得有效的奇偶场操作范围
	if (!TPParity_Range(nDestY, nDestAddWidth, nDestBufferWidth, nDestBufferHeight,
			nSrcY, nSrcAddWidth, nSrcBufferWidth, nSrcBufferHeight, nLineInc, nParity))
			return FALSE;

	DWORD* pDesLine = pDest + nDestY * nDestBufferWidth + nDestX;	// 目标 Buffer 起始地址
	DWORD* pSrcLine = pSrc + nSrcY * nSrcBufferWidth + nSrcX;		// 源 Buffer 起始地址
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);	// 拷贝行的对齐参数

	for (int i=0; i<nDestHeight; i+=nLineInc)
	{
		bgra32_line_mix(pDesLine, pSrcLine, na16, nm16, g_AlphaGainTable);
		pDesLine += nDestAddWidth;
		pSrcLine += nSrcAddWidth;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer NotMix 接口
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_NotMix_SSE(IN OUT DWORD* pDest,			// 目标 Buffer
					   IN int nDestX,				// 目标起始点
					   IN int nDestY,
					   IN int nDestWidth,			// 目标 Mix 大小
					   IN int nDestHeight,
					   IN int nDestBufferWidth,		// 目标 Buffer 大小
					   IN int nDestBufferHeight,
					   IN DWORD* pSrc,				// 源 Buffer
					   IN int nSrcX,				// 源起始点
					   IN int nSrcY,
					   IN int nSrcWidth,			// 源 Mix 大小
					   IN int nSrcHeight,
					   IN int nSrcBufferWidth,		// 源 Buffer 大小
					   IN int nSrcBufferHeight,
					   IN int nParity)				// 奇偶场类型
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	// 获得有效的操作范围
	if (!TPFix_CutRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight))
		return FALSE;

	int nDestAddWidth(0), nSrcAddWidth(0), nLineInc(0);

	// 获得有效的奇偶场操作范围
	if (!TPParity_Range(nDestY, nDestAddWidth, nDestBufferWidth, nDestBufferHeight,
		nSrcY, nSrcAddWidth, nSrcBufferWidth, nSrcBufferHeight, nLineInc, nParity))
		return FALSE;

	DWORD* pDesLine = pDest + nDestY * nDestBufferWidth + nDestX;	// 目标 Buffer 起始地址
	DWORD* pSrcLine = pSrc + nSrcY * nSrcBufferWidth + nSrcX;		// 源 Buffer 起始地址
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);	// 拷贝行的对齐参数

	for (int i=0; i<nDestHeight; i+=nLineInc)
	{
		bgra32_line_notmix(pDesLine, pSrcLine, na16, nm16, g_AlphaGainTable);
		pDesLine += nDestAddWidth;
		pSrcLine += nSrcAddWidth;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 双线性插值缩放时的范围剪切
// 范围有效时返回 TRUE 无效时返回 FALSE
//////////////////////////////////////////////////////////////////////////
BOOL TPBLStretch_CurRange(IN OUT int &nDestX,			// 目标起点
						  IN OUT int &nDestY,
						  IN OUT int &nDestWidth,		// 目标范围
						  IN OUT int &nDestHeight,
						  IN int nDestBufferWidth,	// 目标 Buffer 大小
						  IN int nDestBufferHeight,
						  IN OUT int &nSrcX,			// 源起点
						  IN OUT int &nSrcY,
						  IN OUT int &nSrcWidth,		// 源范围
						  IN OUT int &nSrcHeight,
						  IN int nSrcBufferWidth,		// 源 Buffer 大小
						  IN int nSrcBufferHeight,
						  OUT float &fScaleWidth,		// 缩放比例
						  OUT float &fScaleHeight,
						  OUT float &fSrcStartX,		// 源浮点起点坐标
						  OUT float &fSrcStartY,
						  OUT int &nDestAddWidth,		// 目标 Buffer 增量
						  OUT int &nLineIndex,		// 行起始
						  OUT int &nLineInc,			// 行增量
						  IN int nParity)				// 奇偶场类型
{
	// 边界有效性检测
	if (nDestBufferWidth <= 0 || nDestBufferHeight <= 0
		|| nSrcBufferWidth <= 0	|| nSrcBufferHeight <= 0)
		return FALSE;

	if (nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight
		|| nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return FALSE;

	if (nDestWidth <= 0 || nDestHeight <= 0 || nSrcWidth <= 0 || nSrcHeight <= 0)
		return FALSE;

	if ((nDestX + nDestWidth) <= 0 || (nDestY + nDestHeight) <= 0
		|| (nSrcX + nSrcWidth) <= 0 || (nSrcY + nSrcHeight) <= 0)
		return FALSE;

	int nTmpDestX(nDestX), nTmpDestY(nDestY), nTmpSrcX(nSrcX), nTmpSrcY(nSrcY);
	int nTmpOffset(0), nTmpWidth(0), nTmpHeight(0);

	//fScaleWidth = (float)(nSrcWidth - 1) / (nDestWidth - 1);	// 缩放比例
	//fScaleHeight = (float)(nSrcHeight - 1) / (nDestHeight - 1);
	fScaleWidth = (float)nSrcWidth / nDestWidth;	// 缩放比例
	fScaleHeight = (float)nSrcHeight / nDestHeight;
	// 剪切超出范围
	if (nDestX < 0)		// 目标左边界超出，缩放范围进行右缩进
	{
		nTmpOffset = (int)((-nDestX) * fScaleWidth + 0.5f);	// 源的右缩进量
		nDestWidth += nDestX; nDestX = 0;
		nSrcX += nTmpOffset; nSrcWidth -= nTmpOffset;
	}
	if (nDestY < 0)		// 目标上边界超出，缩放范围进行下缩进
	{
		nTmpOffset = (int)((-nDestY) * fScaleHeight + 0.5f);	// 源的下缩进量
		nDestHeight += nDestY; nDestY = 0;
		nSrcY += nTmpOffset; nSrcHeight -= nTmpOffset;
	}
	if (nSrcX < 0)		// 源左边界超出，缩放范围进行右缩进
	{
		nTmpOffset = (int)ceilf((-nSrcX) / fScaleWidth);		// 目标的右缩进量
		nSrcWidth += nSrcX; nSrcX = 0;
		nDestWidth -= nTmpOffset; nDestX += nTmpOffset;
	}
	if (nSrcY < 0)		// 源上边界超出，缩放范围进行下缩进
	{
		nTmpOffset = (int)ceilf((-nSrcY) / fScaleHeight);	// 目标的下缩进量
		nSrcHeight += nSrcY; nSrcY = 0;
		nDestHeight -= nTmpOffset; nDestY += nTmpOffset;
	}
	nTmpWidth = nDestWidth + nDestX;		// 考虑目标的右边界
	nTmpHeight = nDestHeight + nDestY;		// 考虑目标的下边界
	if (nTmpWidth > nDestBufferWidth)	// 目标右边界超出，缩放范围进行左缩进
	{
		nTmpOffset = (nTmpWidth - nDestBufferWidth);	// 目标的左缩进量
		nDestWidth -= nTmpOffset;
		nTmpOffset = (int)(nTmpOffset * fScaleWidth);	// 源的左缩进量
		nSrcWidth -= nTmpOffset;
		if (nSrcWidth <= 0)
			return FALSE;
	}
	if (nTmpHeight > nDestBufferHeight)	// 目标下边界超出，缩放范围进行上缩进
	{
		nTmpOffset = (nTmpHeight - nDestBufferHeight);	// 目标的上缩进量
		nDestHeight -= nTmpOffset;
		nTmpOffset = (int)(nTmpOffset * fScaleHeight);	// 源的上缩进量
		nSrcHeight -= nTmpOffset;
		if (nSrcHeight <= 0)
			return FALSE;
	}
	nTmpWidth = nSrcWidth + nSrcX;		// 考虑源的右边界
	nTmpHeight = nSrcHeight + nSrcY;	// 考虑源的下边界
	if (nTmpWidth > nSrcBufferWidth)	// 源右边界超出，缩放范围进行左缩进
	{
		nTmpOffset = (nTmpWidth - nSrcBufferWidth);		// 源的左缩进量
		nSrcWidth -= nTmpOffset;
		nTmpOffset = (int)ceilf(nTmpOffset / fScaleWidth);	// 目标的左缩进量
		nDestWidth -= nTmpOffset;
		if (nDestWidth <= 0)
			return FALSE;
	}
	if (nTmpHeight > nSrcBufferHeight)	// 源下边界超出，拷贝范围进行上缩进
	{
		nTmpOffset = (nTmpHeight - nSrcBufferHeight);	// 源的上缩进量
		nSrcHeight -= nTmpOffset;
		nTmpOffset = (int)ceilf(nTmpOffset / fScaleHeight);	// 目标的上缩进量
		nDestHeight -= nTmpOffset;
		if (nDestHeight <= 0)
			return FALSE;
	}
	// 源起点坐标
	fSrcStartX = fScaleWidth * (nDestX - nTmpDestX) + nTmpSrcX + fScaleWidth/2;
	fSrcStartY = fScaleHeight * (nDestY - nTmpDestY) + nTmpSrcY + fScaleHeight/2;
	// 根据奇偶场类型获得增量
	switch (nParity)
	{
	case TP_PARITY_BOTH:		// 全场
		nDestAddWidth = nDestBufferWidth;
		nLineIndex = 0;
		nLineInc = 1;
		break;

	case TP_PARITY_ODD:			// 奇场
		nLineIndex = 0;
		if ((nDestY % 2) == 1)		// 起点在偶场，则从下一行开始
		{
			nDestY++;
			nLineIndex = 1;
			// 判断范围的有效性
			if (nDestY >= nDestBufferHeight)
				return FALSE;
		}
		nDestAddWidth = (nDestBufferWidth << 1);
		nLineInc = 2;
		break;

	case TP_PARITY_EVEN:		// 偶场
		nLineIndex = 0;
		if ((nDestY % 2) == 0)		// 起点在奇场，则从下一行开始
		{
			nDestY++;
			nLineIndex = 1;
			// 判断范围的有效性
			if (nDestY >= nDestBufferHeight)
				return FALSE;
		}
		nDestAddWidth = (nDestBufferWidth << 1);
		nLineInc = 2;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 双线性插值权重与映射索引的计算
//////////////////////////////////////////////////////////////////////////
void TPPower_BLStretch(OUT DWORD *pPowerX,		// X 方向上的权重表与索引表
					   OUT int *pMapX,
					   OUT DWORD *pPowerY,		// Y 方向上的权重表与索引表
					   OUT int *pMapY,
					   IN int nDestWidth,		// 目标范围
					   IN int nDestHeight,
					   IN int nSrcX,			// 源起点
					   IN int nSrcY,
					   IN int nSrcWidth,		// 源范围
					   IN int nSrcHeight,
					   IN int nSrcBufferWidth,	// 源 Buffer 大小
					   IN int nSrcBufferHeight,
					   IN float fScaleWidth,	// 缩放比例
					   IN float fScaleHeight,
					   IN float fSrcStartX,		// 源浮点起点坐标
					   IN float fSrcStartY)
{
	int nMapTmp(0), nLastWidth(nSrcBufferWidth - 2), nLastHeight(nSrcBufferHeight - 2);
	DWORD dwPowerTmp1(0), dwPowerTmp2(0);

	nDestWidth--; nDestHeight--;
	// 计算 X 方向的权重表与索引表
	for (int i=0; i<nDestWidth; i++,fSrcStartX+=fScaleWidth)
	{
		nMapTmp = (int)fSrcStartX;
		dwPowerTmp1 = (DWORD)((nMapTmp + 1.0f - fSrcStartX) * 32768.f + 0.5f);
		dwPowerTmp2 = 32768 - dwPowerTmp1;
		pPowerX[i] = (dwPowerTmp1 | (dwPowerTmp2 << 16));
        nMapTmp = (nMapTmp > nLastWidth) ? nLastWidth : nMapTmp;
		pMapX[i] = (nMapTmp << 2);
	}
	if (nSrcX + nSrcWidth == nSrcBufferWidth)
	{
		pMapX[nDestWidth] = ((nSrcBufferWidth - 2) << 2);
		pPowerX[nDestWidth] = 0x80000000;
	}
	else
	{
		nMapTmp = (int)fSrcStartX;
		dwPowerTmp1 = (DWORD)((nMapTmp + 1.0f - fSrcStartX) * 32768.f + 0.5f);
		dwPowerTmp2 = 32768 - dwPowerTmp1;
		pPowerX[nDestWidth] = (dwPowerTmp1 | (dwPowerTmp2 << 16));
		pMapX[nDestWidth] = (nMapTmp << 2);
	}
	// 计算 Y 方向的权重表与索引表
	for (int i=0; i<nDestHeight; i++,fSrcStartY+=fScaleHeight)
	{
        nMapTmp = (int)fSrcStartY;
		dwPowerTmp1 = (DWORD)((nMapTmp + 1.0f - fSrcStartY) * 32768.f + 0.5f);
        pPowerY[i] = (dwPowerTmp1 | (dwPowerTmp1 << 16));
        nMapTmp = (nMapTmp > nLastHeight) ? nLastHeight : nMapTmp;
        pMapY[i] = nMapTmp;
	}
	if (nSrcY + nSrcHeight == nSrcBufferHeight)
	{
		pMapY[nDestHeight] = nSrcBufferHeight - 2;
		pPowerY[nDestHeight] = 0x00000000;
	}
	else
	{
		pMapY[nDestHeight] = nMapTmp = (int)fSrcStartY;
		dwPowerTmp1 = (DWORD)((nMapTmp + 1.0f - fSrcStartY) * 32768.f + 0.5f);
		pPowerY[nDestHeight] = (dwPowerTmp1 | (dwPowerTmp1 << 16));
	}
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 双线性插值缩放拷贝
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchCopy_SSE(OUT DWORD* pDest,			// 目标 Buffer
							  IN int nDestX,			// 目标起点
							  IN int nDestY,
							  IN int nDestWidth,		// 目标缩放区域
							  IN int nDestHeight,
							  IN int nDestBufferWidth,	// 目标 Buffer 大小
							  IN int nDestBufferHeight,
							  IN DWORD* pSrc,			// 源 Buffer
							  IN int nSrcX,				// 源起点
							  IN int nSrcY,
							  IN int nSrcWidth,			// 源缩放区域
							  IN int nSrcHeight,
							  IN int nSrcBufferWidth,	// 源 Buffer 大小
							  IN int nSrcBufferHeight,
							  IN int nParity)			// 奇偶场类型
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// 获得有效的操作范围
	if (!TPBLStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
			fSrcStartX, fSrcStartY,	nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// 申请权重表与索引表的内存空间
	TPSSE_DECLARE(DWORD*) pPowerX = TPSSE_NEW(nDestWidth, DWORD);	// X 方向上目标列映射到源列的权重表
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);			// X 方向上目标列映射到源列的索引表
	TPSSE_DECLARE(DWORD*) pPowerY = TPSSE_NEW(nDestHeight, DWORD);	// Y 方向上目标行映射到源行的权重表
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);		// Y 方向上目标行映射到源行的索引表
	// Buffer 起始地址
	DWORD *pDesLine = pDest + nDestY * nDestBufferWidth + nDestX, *pSrcLine = NULL;
	int nSrcPitch = (nSrcBufferWidth << 2);

	// 计算权重与映射索引
	TPPower_BLStretch(pPowerX, pMapX, pPowerY, pMapY, nDestWidth, nDestHeight, nSrcX, nSrcY,
		nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight, fSrcStartX, fSrcStartY);
	// 逐行处理
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		pSrcLine = pSrc + pMapY[nLineIndex] * nSrcBufferWidth;
		bgra32_bl_copy_line(pDesLine, nDestWidth, pSrcLine, nSrcPitch, pPowerX, pMapX, pPowerY[nLineIndex]);
		pDesLine += nDestAddWidth;
	}
	// 释放权重表与索引表
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pMapY);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 双线性插值缩放时的范围剪切
// 范围有效时返回 TRUE 无效时返回 FALSE
//////////////////////////////////////////////////////////////////////////
BOOL TPMeanStretch_CurRange(IN OUT int &nDestX,			// 目标起点
							IN OUT int &nDestY,
							IN OUT int &nDestWidth,		// 目标范围
							IN OUT int &nDestHeight,
							IN int nDestBufferWidth,	// 目标 Buffer 大小
							IN int nDestBufferHeight,
							IN OUT int &nSrcX,			// 源起点
							IN OUT int &nSrcY,
							IN OUT int &nSrcWidth,		// 源范围
							IN OUT int &nSrcHeight,
							IN int nSrcBufferWidth,		// 源 Buffer 大小
							IN int nSrcBufferHeight,
							OUT float &fScaleWidth,		// 缩放比例
							OUT float &fScaleHeight,
							OUT float &fSrcStartX,		// 源浮点起点坐标
							OUT float &fSrcStartY,
							OUT int &nDestAddWidth,		// 目标 Buffer 增量
							OUT int &nLineIndex,		// 行起始
							OUT int &nLineInc,			// 行增量
							IN int nParity)				// 奇偶场类型
{
	// 边界有效性检测
	if (nDestBufferWidth <= 0 || nDestBufferHeight <= 0
		|| nSrcBufferWidth <= 0	|| nSrcBufferHeight <= 0)
		return FALSE;

	if (nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight
		|| nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return FALSE;

	if (nDestWidth <= 0 || nDestHeight <= 0 || nSrcWidth <= 0 || nSrcHeight <= 0)
		return FALSE;

	if ((nDestX + nDestWidth) <= 0 || (nDestY + nDestHeight) <= 0
		|| (nSrcX + nSrcWidth) <= 0 || (nSrcY + nSrcHeight) <= 0)
		return FALSE;

	int nTmpDestX(nDestX), nTmpDestY(nDestY), nTmpSrcX(nSrcX), nTmpSrcY(nSrcY);
	int nTmpOffset(0), nTmpWidth(0), nTmpHeight(0);

	fScaleWidth = (float)nSrcWidth / nDestWidth;	// 缩放比例
	fScaleHeight = (float)nSrcHeight / nDestHeight;
	// 剪切超出范围
	if (nDestX < 0)		// 目标左边界超出，缩放范围进行右缩进
	{
		nTmpOffset = (int)((-nDestX) * fScaleWidth + 0.5f);	// 源的右缩进量
		nDestWidth += nDestX; nDestX = 0;
		nSrcX += nTmpOffset; nSrcWidth -= nTmpOffset;
		if (nSrcX >= nSrcBufferWidth || nSrcWidth <= 0)
			return FALSE;
	}
	if (nDestY < 0)		// 目标上边界超出，缩放范围进行下缩进
	{
		nTmpOffset = (int)((-nDestY) * fScaleHeight + 0.5f);	// 源的下缩进量
		nDestHeight += nDestY; nDestY = 0;
		nSrcY += nTmpOffset; nSrcHeight -= nTmpOffset;
		if (nSrcY >= nSrcBufferHeight || nSrcHeight <= 0)
			return FALSE;
	}
	if (nSrcX < 0)		// 源左边界超出，缩放范围进行右缩进
	{
		nTmpOffset = (int)ceilf((-nSrcX) / fScaleWidth);		// 目标的右缩进量
		nSrcWidth += nSrcX; nSrcX = 0;
		nDestWidth -= nTmpOffset; nDestX += nTmpOffset;
		if (nDestX >= nDestBufferWidth || nDestWidth <= 0)
			return FALSE;
	}
	if (nSrcY < 0)		// 源上边界超出，缩放范围进行下缩进
	{
		nTmpOffset = (int)ceilf((-nSrcY) / fScaleHeight);	// 目标的下缩进量
		nSrcHeight += nSrcY; nSrcY = 0;
		nDestHeight -= nTmpOffset; nDestY += nTmpOffset;
		if (nDestY >= nDestBufferHeight || nDestHeight <= 0)
			return FALSE;
	}
	nTmpWidth = nDestWidth + nDestX;		// 考虑目标的右边界
	nTmpHeight = nDestHeight + nDestY;		// 考虑目标的下边界
	if (nTmpWidth > nDestBufferWidth)	// 目标右边界超出，缩放范围进行左缩进
	{
		nTmpOffset = (nTmpWidth - nDestBufferWidth);	// 目标的左缩进量
		nDestWidth -= nTmpOffset;
		nTmpOffset = (int)(nTmpOffset * fScaleWidth);	// 源的左缩进量
		nSrcWidth -= nTmpOffset;
		if (nSrcWidth <= 0)
			return FALSE;
	}
	if (nTmpHeight > nDestBufferHeight)	// 目标下边界超出，缩放范围进行上缩进
	{
		nTmpOffset = (nTmpHeight - nDestBufferHeight);	// 目标的上缩进量
		nDestHeight -= nTmpOffset;
		nTmpOffset = (int)(nTmpOffset * fScaleHeight);	// 源的上缩进量
		nSrcHeight -= nTmpOffset;
		if (nSrcHeight <= 0)
			return FALSE;
	}
	nTmpWidth = nSrcWidth + nSrcX;		// 考虑源的右边界
	nTmpHeight = nSrcHeight + nSrcY;	// 考虑源的下边界
	if (nTmpWidth > nSrcBufferWidth)	// 源右边界超出，缩放范围进行左缩进
	{
		nTmpOffset = (nTmpWidth - nSrcBufferWidth);		// 源的左缩进量
		nSrcWidth -= nTmpOffset;
		nTmpOffset = (int)ceilf(nTmpOffset / fScaleWidth);	// 目标的左缩进量
		nDestWidth -= nTmpOffset;
		if (nDestWidth <= 0)
			return FALSE;
	}
	if (nTmpHeight > nSrcBufferHeight)	// 源下边界超出，拷贝范围进行上缩进
	{
		nTmpOffset = (nTmpHeight - nSrcBufferHeight);	// 源的上缩进量
		nSrcHeight -= nTmpOffset;
		nTmpOffset = (int)ceilf(nTmpOffset / fScaleHeight);	// 目标的上缩进量
		nDestHeight -= nTmpOffset;
		if (nDestHeight <= 0)
			return FALSE;
	}
	// 源起点坐标
	fSrcStartX = fScaleWidth * (nDestX - nTmpDestX) + nTmpSrcX;
	fSrcStartY = fScaleHeight * (nDestY - nTmpDestY) + nTmpSrcY;
	// 根据奇偶场类型获得增量
	switch (nParity)
	{
	case TP_PARITY_BOTH:		// 全场
		nDestAddWidth = nDestBufferWidth;
		nLineIndex = 0;
		nLineInc = 1;
		break;

	case TP_PARITY_ODD:			// 奇场
		nLineIndex = 0;
		if ((nDestY % 2) == 1)		// 起点在偶场，则从下一行开始
		{
			nDestY++;
			nLineIndex = 1;
			// 判断范围的有效性
			if (nDestY >= nDestBufferHeight)
				return FALSE;
		}
		nDestAddWidth = (nDestBufferWidth << 1);
		nLineInc = 2;
		break;

	case TP_PARITY_EVEN:		// 偶场
		nLineIndex = 0;
		if ((nDestY % 2) == 0)		// 起点在奇场，则从下一行开始
		{
			nDestY++;
			nLineIndex = 1;
			// 判断范围的有效性
			if (nDestY >= nDestBufferHeight)
				return FALSE;
		}
		nDestAddWidth = (nDestBufferWidth << 1);
		nLineInc = 2;
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 用于加权平均缩放时计算权重的接口
//////////////////////////////////////////////////////////////////////////
// 计算水平或垂直方向上某个像素的权重个数及其首尾权重
int calc_power_pixel(OUT float* pPower,		// 首尾权重表
					 IN OUT float& fStart,	// 起始点坐标值
					 IN float fStep)		// 缩放步长
{
	if (fStep < 1.0f)
	{
		if (1.0f - fStart > fStep)
		{
			pPower[0] = fStep;
			fStart += fStep;
			return 1;
		}
	}
	pPower[0] = 1.0f - fStart;

	// 上一次计算剩余占一个位置
	float fLeave = fStep - (1.0f - fStart);
	int n = (int)floorf(fLeave);

	fStart = pPower[1] = (fLeave - n);

	return n + 2;
}

// 计算水平或垂直方向上各像素的权重个数表与首尾权重表
void strech_calc_power(OUT float* pPower,		// 权重表
					   OUT int* pMap,			// 每个点占用的权重个数表
					   IN int nCount,			// 目标拷贝长度或高度
					   IN float fStep,			// 缩放步长
					   IN float fStart/* = 0.0f*/)	// 起始点坐标值
{
	if (nCount <= 0)
		return;

	nCount--;
	for (int i=0; i<nCount; i++)
	{
		pMap[i] = calc_power_pixel(pPower, fStart, fStep);
		pPower += 2;
	}
	pMap[nCount] = calc_power_pixel(pPower, fStart, fStep) - 1;
	pPower[1] = 1.f;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 加权平均插值缩放拷贝
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchCopy_SSE(OUT DWORD* pDest,			// 目标 Buffer
								IN int nDestX,				// 目标起点
								IN int nDestY,
								IN int nDestWidth,			// 目标缩放区域
								IN int nDestHeight,
								IN int nDestBufferWidth,	// 目标 Buffer 大小
								IN int nDestBufferHeight,
								IN DWORD* pSrc,				// 源 Buffer
								IN int nSrcX,				// 源起点
								IN int nSrcY,
								IN int nSrcWidth,			// 源缩放区域
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		// 源 Buffer 大小
								IN int nSrcBufferHeight,
								IN int nParity)				// 奇偶场类型
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// 获得有效的操作范围
	if (!TPMeanStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
			fSrcStartX, fSrcStartY, nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// 申请权重表、权重数量表及缓存空间
	float fScaleW_I = 1.0f / fScaleWidth, fScaleH_I = 1.0f / fScaleHeight;
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);		// X 方向上的权重数量表
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);	// Y 方向上的权重数量表
	TPSSE_DECLARE(float*) pPowerX = TPSSE_NEW((nDestWidth << 1), float);	// X 方向上的权重表
	TPSSE_DECLARE(float*) pPowerY = TPSSE_NEW((nDestHeight << 1), float);	// Y 方向上的权重表
	DWORD *pDesLine = pDest + nDestX + nDestY * nDestBufferWidth;		// Buffer 起始地址
	DWORD *pSrcLine = pSrc + nSrcX + nSrcY * nSrcBufferWidth;
	float *pTmpPowerY = pPowerY;		// Y 方向权重表地址
	int nSrcPitch = (nSrcBufferWidth << 2);
	TPSSE_DECLARE(float*) pTmpLine = TPSSE_NEW((nSrcWidth << 2), float);	// 源行的临时缓存
	TPSSE_DECLARE(float) fCoefW[4] = { fScaleW_I, fScaleW_I, fScaleW_I, fScaleW_I };
	TPSSE_DECLARE(float) fCoefH[4] = { fScaleH_I, fScaleH_I, fScaleH_I, fScaleH_I };

	// 计算权重
	strech_calc_power(pPowerX, pMapX, nDestWidth, fScaleWidth, fSrcStartX - (int)fSrcStartX);
	strech_calc_power(pPowerY, pMapY, nDestHeight, fScaleHeight, fSrcStartY - (int)fSrcStartY);
	// 逐行处理
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		if ((nSrcY + pMapY[nLineIndex]) > nSrcBufferHeight)	// 防止最后几行越界访问
		{
			pSrcLine = pSrc + nSrcX + (nSrcBufferHeight - pMapY[nLineIndex]) * nSrcBufferWidth;
		}
		bgra32_mean_copy_add_y_quick(pTmpLine, pSrcLine, nSrcWidth, nSrcPitch, pTmpPowerY, fCoefH, pMapY[nLineIndex]);
		bgra32_mean_copy_add_x(pDesLine, nDestWidth, pTmpLine, pPowerX, pMapX, fCoefW);
		if (nLineInc == 1)
		{
			nSrcY += (pMapY[nLineIndex] - 1);
			pSrcLine += ((pMapY[nLineIndex] - 1) * nSrcBufferWidth);
			pTmpPowerY += 2;
		}
		else
		{
			nSrcY += (pMapY[nLineIndex] - 2 + pMapY[nLineIndex + 1]);
			pSrcLine += ((pMapY[nLineIndex] - 2 + pMapY[nLineIndex + 1]) * nSrcBufferWidth);
			pTmpPowerY += 4;
		}
		pDesLine += nDestAddWidth;
	}
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pMapY);
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pTmpLine);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 双线性插值缩放 Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchMix_SSE(OUT DWORD* pDest,			// 目标 Buffer
							 IN int nDestX,				// 目标起点
							 IN int nDestY,
							 IN int nDestWidth,			// 目标缩放区域
							 IN int nDestHeight,
							 IN int nDestBufferWidth,	// 目标 Buffer 大小
							 IN int nDestBufferHeight,
							 IN DWORD* pSrc,			// 源 Buffer
							 IN int nSrcX,				// 源起点
							 IN int nSrcY,
							 IN int nSrcWidth,			// 源缩放区域
							 IN int nSrcHeight,
							 IN int nSrcBufferWidth,	// 源 Buffer 大小
							 IN int nSrcBufferHeight,
							 IN int nParity)			// 奇偶场类型
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// 获得有效的操作范围
	if (!TPBLStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
			fSrcStartX, fSrcStartY,	nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// 申请权重表与索引表的内存空间
	TPSSE_DECLARE(DWORD*) pPowerX = TPSSE_NEW(nDestWidth, DWORD);	// X 方向上目标列映射到源列的权重表
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);			// X 方向上目标列映射到源列的索引表
	TPSSE_DECLARE(DWORD*) pPowerY = TPSSE_NEW(nDestHeight, DWORD);	// Y 方向上目标行映射到源行的权重表
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);			// Y 方向上目标行映射到源行的索引表
	TPSSE_DECLARE(DWORD*) pTmpBuf = TPSSE_NEW(nDestWidth, DWORD);	// 临时缓存
	// Buffer 起始地址
	DWORD *pDesLine = pDest + nDestY * nDestBufferWidth + nDestX, *pSrcLine = NULL;
	int nSrcPitch = (nSrcBufferWidth << 2);
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);

	// 计算权重与映射索引
	TPPower_BLStretch(pPowerX, pMapX, pPowerY, pMapY, nDestWidth, nDestHeight, nSrcX, nSrcY,
		nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight, fSrcStartX, fSrcStartY);
	// 逐行处理
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		pSrcLine = pSrc + pMapY[nLineIndex] * nSrcBufferWidth;
		bgra32_bl_copy_line(pTmpBuf, nDestWidth, pSrcLine, nSrcPitch, pPowerX, pMapX, pPowerY[nLineIndex]);
		bgra32_line_mix(pDesLine, pTmpBuf, na16, nm16, g_AlphaGainTable);
		pDesLine += nDestAddWidth;
	}
	// 释放权重表与索引表
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pMapY);
	TPSSE_FREE(pTmpBuf);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 双线性插值缩放 NotMix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchNotMix_SSE(OUT DWORD* pDest,			// 目标 Buffer
								IN int nDestX,				// 目标起点
								IN int nDestY,
								IN int nDestWidth,			// 目标缩放区域
								IN int nDestHeight,
								IN int nDestBufferWidth,	// 目标 Buffer 大小
								IN int nDestBufferHeight,
								IN DWORD* pSrc,				// 源 Buffer
								IN int nSrcX,				// 源起点
								IN int nSrcY,
								IN int nSrcWidth,			// 源缩放区域
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		// 源 Buffer 大小
								IN int nSrcBufferHeight,
								IN int nParity)				// 奇偶场类型
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// 获得有效的操作范围
	if (!TPBLStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
			fSrcStartX, fSrcStartY,	nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// 申请权重表与索引表的内存空间
	TPSSE_DECLARE(DWORD*) pPowerX = TPSSE_NEW(nDestWidth, DWORD);	// X 方向上目标列映射到源列的权重表
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);			// X 方向上目标列映射到源列的索引表
	TPSSE_DECLARE(DWORD*) pPowerY = TPSSE_NEW(nDestHeight, DWORD);	// Y 方向上目标行映射到源行的权重表
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);		// Y 方向上目标行映射到源行的索引表
	TPSSE_DECLARE(DWORD*) pTmpBuf = TPSSE_NEW(nDestWidth, DWORD);	// 临时缓存
	// Buffer 起始地址
	DWORD *pDesLine = pDest + nDestY * nDestBufferWidth + nDestX, *pSrcLine = NULL;
	int nSrcPitch = (nSrcBufferWidth << 2);
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);

	// 计算权重与映射索引
	TPPower_BLStretch(pPowerX, pMapX, pPowerY, pMapY, nDestWidth, nDestHeight, nSrcX, nSrcY,
		nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight, fSrcStartX, fSrcStartY);
	// 逐行处理
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		pSrcLine = pSrc + pMapY[nLineIndex] * nSrcBufferWidth;
		bgra32_bl_copy_line(pTmpBuf, nDestWidth, pSrcLine, nSrcPitch, pPowerX, pMapX, pPowerY[nLineIndex]);
		bgra32_line_notmix(pDesLine, pTmpBuf, na16, nm16, g_AlphaGainTable);
		pDesLine += nDestAddWidth;
	}
	// 释放权重表与索引表
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pMapY);
	TPSSE_FREE(pTmpBuf);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 加权平均插值缩放 Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchMix_SSE(OUT DWORD* pDest,			// 目标 Buffer
							   IN int nDestX,				// 目标起点
							   IN int nDestY,
							   IN int nDestWidth,			// 目标缩放区域
							   IN int nDestHeight,
							   IN int nDestBufferWidth,		// 目标 Buffer 大小
							   IN int nDestBufferHeight,
							   IN DWORD* pSrc,				// 源 Buffer
							   IN int nSrcX,				// 源起点
							   IN int nSrcY,
							   IN int nSrcWidth,			// 源缩放区域
							   IN int nSrcHeight,
							   IN int nSrcBufferWidth,		// 源 Buffer 大小
							   IN int nSrcBufferHeight,
							   IN int nParity)				// 奇偶场类型
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// 获得有效的操作范围
	if (!TPMeanStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
		fSrcStartX, fSrcStartY, nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// 申请权重表、权重数量表及缓存空间
	float fScaleW_I = 1.0f / fScaleWidth, fScaleH_I = 1.0f / fScaleHeight;
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);		// X 方向上的权重数量表
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);	// Y 方向上的权重数量表
	TPSSE_DECLARE(float*) pPowerX = TPSSE_NEW((nDestWidth << 1), float);	// X 方向上的权重表
	TPSSE_DECLARE(float*) pPowerY = TPSSE_NEW((nDestHeight << 1), float);	// Y 方向上的权重表
	DWORD *pDesLine = pDest + nDestX + nDestY * nDestBufferWidth;		// Buffer 起始地址
	DWORD *pSrcLine = pSrc + nSrcX + nSrcY * nSrcBufferWidth;
	float *pTmpPowerY = pPowerY;		// Y 方向权重表地址
	int nSrcPitch = (nSrcBufferWidth << 2), na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);
	TPSSE_DECLARE(float*) pTmpLine = TPSSE_NEW((nSrcWidth << 2), float);	// 源行的临时缓存
	TPSSE_DECLARE(DWORD*) pTmpBuf = TPSSE_NEW(nDestWidth, DWORD);			// 临时缓存
	TPSSE_DECLARE(float) fCoefW[4] = { fScaleW_I, fScaleW_I, fScaleW_I, fScaleW_I };
	TPSSE_DECLARE(float) fCoefH[4] = { fScaleH_I, fScaleH_I, fScaleH_I, fScaleH_I };

	// 计算权重
	strech_calc_power(pPowerX, pMapX, nDestWidth, fScaleWidth, fSrcStartX - (int)fSrcStartX);
	strech_calc_power(pPowerY, pMapY, nDestHeight, fScaleHeight, fSrcStartY - (int)fSrcStartY);
	// 逐行处理
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		bgra32_mean_copy_add_y_quick(pTmpLine, pSrcLine, nSrcWidth, nSrcPitch, pTmpPowerY, fCoefH, pMapY[nLineIndex]);
		bgra32_mean_copy_add_x(pTmpBuf, nDestWidth, pTmpLine, pPowerX, pMapX, fCoefW);
		bgra32_line_mix(pDesLine, pTmpBuf, na16, nm16, g_AlphaGainTable);
		if (nLineInc == 1)
		{
			pSrcLine += ((pMapY[nLineIndex] - 1) * nSrcBufferWidth);
			pTmpPowerY += 2;
		}
		else
		{
			pSrcLine += ((pMapY[nLineIndex] - 2 + pMapY[nLineIndex + 1]) * nSrcBufferWidth);
			pTmpPowerY += 4;
		}
		pDesLine += nDestAddWidth;
	}
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pMapY);
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pTmpLine);
	TPSSE_FREE(pTmpBuf);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 加权平均插值缩放 Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchNotMix_SSE(OUT DWORD* pDest,				// 目标 Buffer
								  IN int nDestX,				// 目标起点
								  IN int nDestY,
								  IN int nDestWidth,			// 目标缩放区域
								  IN int nDestHeight,
								  IN int nDestBufferWidth,		// 目标 Buffer 大小
								  IN int nDestBufferHeight,
								  IN DWORD* pSrc,				// 源 Buffer
								  IN int nSrcX,					// 源起点
								  IN int nSrcY,
								  IN int nSrcWidth,				// 源缩放区域
								  IN int nSrcHeight,
								  IN int nSrcBufferWidth,		// 源 Buffer 大小
								  IN int nSrcBufferHeight,
								  IN int nParity)				// 奇偶场类型
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// 获得有效的操作范围
	if (!TPMeanStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
		fSrcStartX, fSrcStartY, nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// 申请权重表、权重数量表及缓存空间
	float fScaleW_I = 1.0f / fScaleWidth, fScaleH_I = 1.0f / fScaleHeight;
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);	// X 方向上的权重数量表
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);	// Y 方向上的权重数量表
	TPSSE_DECLARE(float*) pPowerX = TPSSE_NEW((nDestWidth << 1), float);	// X 方向上的权重表
	TPSSE_DECLARE(float*) pPowerY = TPSSE_NEW((nDestHeight << 1), float);	// Y 方向上的权重表
	DWORD *pDesLine = pDest + nDestX + nDestY * nDestBufferWidth;		// Buffer 起始地址
	DWORD *pSrcLine = pSrc + nSrcX + nSrcY * nSrcBufferWidth;
	float *pTmpPowerY = pPowerY;		// Y 方向权重表地址
	int nSrcPitch = (nSrcBufferWidth << 2), na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);
	TPSSE_DECLARE(float*) pTmpLine = TPSSE_NEW((nSrcWidth << 2), float);	// 源行的临时缓存
	TPSSE_DECLARE(DWORD*) pTmpBuf = TPSSE_NEW(nDestWidth, DWORD);		// 临时缓存
	TPSSE_DECLARE(float) fCoefW[4] = { fScaleW_I, fScaleW_I, fScaleW_I, fScaleW_I };
	TPSSE_DECLARE(float) fCoefH[4] = { fScaleH_I, fScaleH_I, fScaleH_I, fScaleH_I };

	// 计算权重
	strech_calc_power(pPowerX, pMapX, nDestWidth, fScaleWidth, fSrcStartX - (int)fSrcStartX);
	strech_calc_power(pPowerY, pMapY, nDestHeight, fScaleHeight, fSrcStartY - (int)fSrcStartY);
	// 逐行处理
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		bgra32_mean_copy_add_y_quick(pTmpLine, pSrcLine, nSrcWidth, nSrcPitch, pTmpPowerY, fCoefH, pMapY[nLineIndex]);
		bgra32_mean_copy_add_x(pTmpBuf, nDestWidth, pTmpLine, pPowerX, pMapX, fCoefW);
		bgra32_line_notmix(pDesLine, pTmpBuf, na16, nm16, g_AlphaGainTable);
		if (nLineInc == 1)
		{
			pSrcLine += ((pMapY[nLineIndex] - 1) * nSrcBufferWidth);
			pTmpPowerY += 2;
		}
		else
		{
			pSrcLine += ((pMapY[nLineIndex] - 2 + pMapY[nLineIndex + 1]) * nSrcBufferWidth);
			pTmpPowerY += 4;
		}
		pDesLine += nDestAddWidth;
	}
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pMapY);
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pTmpLine);
	TPSSE_FREE(pTmpBuf);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 文件结束