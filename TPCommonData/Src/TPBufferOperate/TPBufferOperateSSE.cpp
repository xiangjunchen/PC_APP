//////////////////////////////////////////////////////////////////////////
// �������ڴ�����ӿ� SSE �Ż�
// �����ߣ���־��
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TPBufferOperateSSE.h"
#include "TPBufferOperateDef.h"
#include "TPSSE2.h"


extern DWORD *g_AlphaGainTable;		// Alpha �����

//////////////////////////////////////////////////////////////////////////
// ������ʱ�ķ�Χ����
// ��Χ��Чʱ���� FALSE ��Чʱ���� TRUE
//////////////////////////////////////////////////////////////////////////
BOOL TPFix_CutRange(IN OUT int &nDestX,			// Ŀ�����
					IN OUT int &nDestY,
					IN OUT int &nDestWidth,		// Ŀ�귶Χ
					IN OUT int &nDestHeight,
					IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
					IN int nDestBufferHeight,
					IN OUT int &nSrcX,			// Դ���
					IN OUT int &nSrcY,
					IN OUT int &nSrcWidth,		// Դ��Χ
					IN OUT int &nSrcHeight,
					IN int nSrcBufferWidth,		// Դ Buffer ��С
					IN int nSrcBufferHeight)
{
	// �߽���Ч�Լ��
	if (nDestBufferWidth <= 0 || nDestBufferHeight <= 0
		|| nSrcBufferWidth <= 0	|| nSrcBufferHeight <= 0)
		return FALSE;

	if (nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight
		|| nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return FALSE;

	int nTmpWidth(0), nTmpHeight(0);

	// ���г�����Χ
	if (nDestX < 0)		// Ŀ����߽糬����������Χ����������
	{
		nDestWidth += nDestX; nSrcX -= nDestX; nDestX = 0;
	}
	if (nDestY < 0)		// Ŀ���ϱ߽糬����������Χ����������
	{
		nDestHeight += nDestY; nSrcY -= nDestY;	nDestY = 0;
	}
	if (nSrcX < 0)		// Դ��߽糬����������Χ����������
	{
		nDestWidth += nSrcX; nDestX -= nSrcX; nSrcX = 0;
	}
	if (nSrcY < 0)		// Դ�ϱ߽糬����������Χ����������
	{
		nDestHeight += nSrcY; nDestY -= nSrcY; nSrcY = 0;
	}
	nTmpWidth = nDestWidth + nSrcX;		// ����Դ���ұ߽�
	nTmpHeight = nDestHeight + nSrcY;	// ����Դ���±߽�
	if (nTmpWidth > nSrcBufferWidth)	// Դ�ұ߽糬����������Χ����������
	{
		nDestWidth -= (nTmpWidth - nSrcBufferWidth);
	}
	if (nTmpHeight > nSrcBufferHeight)	// Դ�±߽糬����������Χ����������
	{
		nDestHeight -= (nTmpHeight - nSrcBufferHeight);
	}
	nTmpWidth = nDestWidth + nDestX;	// ����Ŀ����ұ߽�
	nTmpHeight = nDestHeight + nDestY;	// ����Ŀ����±߽�
	if (nTmpWidth > nDestBufferWidth)	// Ŀ���ұ߽糬����������Χ����������
	{
		nDestWidth -= (nTmpWidth - nDestBufferWidth);
	}
	if (nTmpHeight > nDestBufferHeight)	// Ŀ���±߽糬����������Χ����������
	{
		nDestHeight -= (nTmpHeight - nDestBufferHeight);
	}
	if (nDestWidth <= 0 || nDestHeight <= 0)	// ������Χ��Ч
		return FALSE;

	nSrcWidth = nDestWidth;
	nSrcHeight = nDestHeight;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// ��ż���ķ�Χ����
// ��Χ��Чʱ���� FALSE ��Чʱ���� TRUE
//////////////////////////////////////////////////////////////////////////
BOOL TPParity_Range(IN OUT int &nDestY,			// Ŀ�����
					OUT int &nDestAddWidth,		// Ŀ�� Buffer ����
					IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
					IN int nDestBufferHeight,
					IN OUT int &nSrcY,			// Դ���
					OUT int &nSrcAddWidth,		// Դ Buffer ����
					IN int nSrcBufferWidth,		// Դ Buffer ��С
					IN int nSrcBufferHeight,
					OUT int &nLineInc,			// ������
					IN int nParity)				// ��ż������
{
	switch (nParity)		// ������ż�����ͻ������
	{
	case TP_PARITY_BOTH:		// ȫ��
		nDestAddWidth = nDestBufferWidth;
		nSrcAddWidth = nSrcBufferWidth;
		nLineInc = 1;
		break;

	case TP_PARITY_ODD:			// �泡
		if ((nDestY % 2) == 1)		// �����ż���������һ�п�ʼ
		{
			nDestY++;
			nSrcY++;
			// �жϷ�Χ����Ч��
			if (nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return FALSE;
		}
		nDestAddWidth = (nDestBufferWidth << 1);
		nSrcAddWidth = (nSrcBufferWidth << 1);
		nLineInc = 2;
		break;

	case TP_PARITY_EVEN:		// ż��
		if ((nDestY % 2) == 0)		// ������泡�������һ�п�ʼ
		{
			nDestY++;
			nSrcY++;
			// �жϷ�Χ����Ч��
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
// BGRA 32 λɫ Buffer Copy �ӿ�
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_Copy_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
					 IN int nDestX,				// Ŀ����ʼ��
					 IN int nDestY,
					 IN int nDestWidth,			// Ŀ�� Mix ��С
					 IN int nDestHeight,
					 IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
					 IN int nDestBufferHeight,
					 IN DWORD* pSrc,			// Դ Buffer
					 IN int nSrcX,				// Դ��ʼ��
					 IN int nSrcY,
					 IN int nSrcWidth,			// Դ Mix ��С
					 IN int nSrcHeight,
					 IN int nSrcBufferWidth,	// Դ Buffer ��С
					 IN int nSrcBufferHeight,
					 IN int nParity)			// ��ż������
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	// �����Ч�Ĳ�����Χ
	if (!TPFix_CutRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight))
		return FALSE;

	int nDestAddWidth(0), nSrcAddWidth(0), nLineInc(0);

	// �����Ч����ż��������Χ
	if (!TPParity_Range(nDestY, nDestAddWidth, nDestBufferWidth, nDestBufferHeight,
		nSrcY, nSrcAddWidth, nSrcBufferWidth, nSrcBufferHeight, nLineInc, nParity))
		return FALSE;

	DWORD* pDesLine = pDest + nDestY * nDestBufferWidth + nDestX;	// Ŀ�� Buffer ��ʼ��ַ
	DWORD* pSrcLine = pSrc + nSrcY * nSrcBufferWidth + nSrcX;		// Դ Buffer ��ʼ��ַ
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);	// �����еĶ������

	for (int i=0; i<nDestHeight; i+=nLineInc)
	{
		memcpy_line(pDesLine, pSrcLine, na16, nm16);
		pDesLine += nDestAddWidth;
		pSrcLine += nSrcAddWidth;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer Mix �ӿ�
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_Mix_SSE(IN OUT DWORD* pDest,		// Ŀ�� Buffer
					IN int nDestX,				// Ŀ����ʼ��
					IN int nDestY,
					IN int nDestWidth,			// Ŀ�� Mix ��С
					IN int nDestHeight,
					IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
					IN int nDestBufferHeight,
					IN DWORD* pSrc,				// Դ Buffer
					IN int nSrcX,				// Դ��ʼ��
					IN int nSrcY,
					IN int nSrcWidth,			// Դ Mix ��С
					IN int nSrcHeight,
					IN int nSrcBufferWidth,		// Դ Buffer ��С
					IN int nSrcBufferHeight,
					IN int nParity)				// ��ż������
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	// �����Ч�Ĳ�����Χ
	if (!TPFix_CutRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight))
			return FALSE;

	int nDestAddWidth(0), nSrcAddWidth(0), nLineInc(0);

	// �����Ч����ż��������Χ
	if (!TPParity_Range(nDestY, nDestAddWidth, nDestBufferWidth, nDestBufferHeight,
			nSrcY, nSrcAddWidth, nSrcBufferWidth, nSrcBufferHeight, nLineInc, nParity))
			return FALSE;

	DWORD* pDesLine = pDest + nDestY * nDestBufferWidth + nDestX;	// Ŀ�� Buffer ��ʼ��ַ
	DWORD* pSrcLine = pSrc + nSrcY * nSrcBufferWidth + nSrcX;		// Դ Buffer ��ʼ��ַ
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);	// �����еĶ������

	for (int i=0; i<nDestHeight; i+=nLineInc)
	{
		bgra32_line_mix(pDesLine, pSrcLine, na16, nm16, g_AlphaGainTable);
		pDesLine += nDestAddWidth;
		pSrcLine += nSrcAddWidth;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer NotMix �ӿ�
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_NotMix_SSE(IN OUT DWORD* pDest,			// Ŀ�� Buffer
					   IN int nDestX,				// Ŀ����ʼ��
					   IN int nDestY,
					   IN int nDestWidth,			// Ŀ�� Mix ��С
					   IN int nDestHeight,
					   IN int nDestBufferWidth,		// Ŀ�� Buffer ��С
					   IN int nDestBufferHeight,
					   IN DWORD* pSrc,				// Դ Buffer
					   IN int nSrcX,				// Դ��ʼ��
					   IN int nSrcY,
					   IN int nSrcWidth,			// Դ Mix ��С
					   IN int nSrcHeight,
					   IN int nSrcBufferWidth,		// Դ Buffer ��С
					   IN int nSrcBufferHeight,
					   IN int nParity)				// ��ż������
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	// �����Ч�Ĳ�����Χ
	if (!TPFix_CutRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight))
		return FALSE;

	int nDestAddWidth(0), nSrcAddWidth(0), nLineInc(0);

	// �����Ч����ż��������Χ
	if (!TPParity_Range(nDestY, nDestAddWidth, nDestBufferWidth, nDestBufferHeight,
		nSrcY, nSrcAddWidth, nSrcBufferWidth, nSrcBufferHeight, nLineInc, nParity))
		return FALSE;

	DWORD* pDesLine = pDest + nDestY * nDestBufferWidth + nDestX;	// Ŀ�� Buffer ��ʼ��ַ
	DWORD* pSrcLine = pSrc + nSrcY * nSrcBufferWidth + nSrcX;		// Դ Buffer ��ʼ��ַ
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);	// �����еĶ������

	for (int i=0; i<nDestHeight; i+=nLineInc)
	{
		bgra32_line_notmix(pDesLine, pSrcLine, na16, nm16, g_AlphaGainTable);
		pDesLine += nDestAddWidth;
		pSrcLine += nSrcAddWidth;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// ˫���Բ�ֵ����ʱ�ķ�Χ����
// ��Χ��Чʱ���� TRUE ��Чʱ���� FALSE
//////////////////////////////////////////////////////////////////////////
BOOL TPBLStretch_CurRange(IN OUT int &nDestX,			// Ŀ�����
						  IN OUT int &nDestY,
						  IN OUT int &nDestWidth,		// Ŀ�귶Χ
						  IN OUT int &nDestHeight,
						  IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
						  IN int nDestBufferHeight,
						  IN OUT int &nSrcX,			// Դ���
						  IN OUT int &nSrcY,
						  IN OUT int &nSrcWidth,		// Դ��Χ
						  IN OUT int &nSrcHeight,
						  IN int nSrcBufferWidth,		// Դ Buffer ��С
						  IN int nSrcBufferHeight,
						  OUT float &fScaleWidth,		// ���ű���
						  OUT float &fScaleHeight,
						  OUT float &fSrcStartX,		// Դ�����������
						  OUT float &fSrcStartY,
						  OUT int &nDestAddWidth,		// Ŀ�� Buffer ����
						  OUT int &nLineIndex,		// ����ʼ
						  OUT int &nLineInc,			// ������
						  IN int nParity)				// ��ż������
{
	// �߽���Ч�Լ��
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

	//fScaleWidth = (float)(nSrcWidth - 1) / (nDestWidth - 1);	// ���ű���
	//fScaleHeight = (float)(nSrcHeight - 1) / (nDestHeight - 1);
	fScaleWidth = (float)nSrcWidth / nDestWidth;	// ���ű���
	fScaleHeight = (float)nSrcHeight / nDestHeight;
	// ���г�����Χ
	if (nDestX < 0)		// Ŀ����߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (int)((-nDestX) * fScaleWidth + 0.5f);	// Դ����������
		nDestWidth += nDestX; nDestX = 0;
		nSrcX += nTmpOffset; nSrcWidth -= nTmpOffset;
	}
	if (nDestY < 0)		// Ŀ���ϱ߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (int)((-nDestY) * fScaleHeight + 0.5f);	// Դ����������
		nDestHeight += nDestY; nDestY = 0;
		nSrcY += nTmpOffset; nSrcHeight -= nTmpOffset;
	}
	if (nSrcX < 0)		// Դ��߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (int)ceilf((-nSrcX) / fScaleWidth);		// Ŀ�����������
		nSrcWidth += nSrcX; nSrcX = 0;
		nDestWidth -= nTmpOffset; nDestX += nTmpOffset;
	}
	if (nSrcY < 0)		// Դ�ϱ߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (int)ceilf((-nSrcY) / fScaleHeight);	// Ŀ�����������
		nSrcHeight += nSrcY; nSrcY = 0;
		nDestHeight -= nTmpOffset; nDestY += nTmpOffset;
	}
	nTmpWidth = nDestWidth + nDestX;		// ����Ŀ����ұ߽�
	nTmpHeight = nDestHeight + nDestY;		// ����Ŀ����±߽�
	if (nTmpWidth > nDestBufferWidth)	// Ŀ���ұ߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (nTmpWidth - nDestBufferWidth);	// Ŀ�����������
		nDestWidth -= nTmpOffset;
		nTmpOffset = (int)(nTmpOffset * fScaleWidth);	// Դ����������
		nSrcWidth -= nTmpOffset;
		if (nSrcWidth <= 0)
			return FALSE;
	}
	if (nTmpHeight > nDestBufferHeight)	// Ŀ���±߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (nTmpHeight - nDestBufferHeight);	// Ŀ�����������
		nDestHeight -= nTmpOffset;
		nTmpOffset = (int)(nTmpOffset * fScaleHeight);	// Դ����������
		nSrcHeight -= nTmpOffset;
		if (nSrcHeight <= 0)
			return FALSE;
	}
	nTmpWidth = nSrcWidth + nSrcX;		// ����Դ���ұ߽�
	nTmpHeight = nSrcHeight + nSrcY;	// ����Դ���±߽�
	if (nTmpWidth > nSrcBufferWidth)	// Դ�ұ߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (nTmpWidth - nSrcBufferWidth);		// Դ����������
		nSrcWidth -= nTmpOffset;
		nTmpOffset = (int)ceilf(nTmpOffset / fScaleWidth);	// Ŀ�����������
		nDestWidth -= nTmpOffset;
		if (nDestWidth <= 0)
			return FALSE;
	}
	if (nTmpHeight > nSrcBufferHeight)	// Դ�±߽糬����������Χ����������
	{
		nTmpOffset = (nTmpHeight - nSrcBufferHeight);	// Դ����������
		nSrcHeight -= nTmpOffset;
		nTmpOffset = (int)ceilf(nTmpOffset / fScaleHeight);	// Ŀ�����������
		nDestHeight -= nTmpOffset;
		if (nDestHeight <= 0)
			return FALSE;
	}
	// Դ�������
	fSrcStartX = fScaleWidth * (nDestX - nTmpDestX) + nTmpSrcX + fScaleWidth/2;
	fSrcStartY = fScaleHeight * (nDestY - nTmpDestY) + nTmpSrcY + fScaleHeight/2;
	// ������ż�����ͻ������
	switch (nParity)
	{
	case TP_PARITY_BOTH:		// ȫ��
		nDestAddWidth = nDestBufferWidth;
		nLineIndex = 0;
		nLineInc = 1;
		break;

	case TP_PARITY_ODD:			// �泡
		nLineIndex = 0;
		if ((nDestY % 2) == 1)		// �����ż���������һ�п�ʼ
		{
			nDestY++;
			nLineIndex = 1;
			// �жϷ�Χ����Ч��
			if (nDestY >= nDestBufferHeight)
				return FALSE;
		}
		nDestAddWidth = (nDestBufferWidth << 1);
		nLineInc = 2;
		break;

	case TP_PARITY_EVEN:		// ż��
		nLineIndex = 0;
		if ((nDestY % 2) == 0)		// ������泡�������һ�п�ʼ
		{
			nDestY++;
			nLineIndex = 1;
			// �жϷ�Χ����Ч��
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
// ˫���Բ�ֵȨ����ӳ�������ļ���
//////////////////////////////////////////////////////////////////////////
void TPPower_BLStretch(OUT DWORD *pPowerX,		// X �����ϵ�Ȩ�ر���������
					   OUT int *pMapX,
					   OUT DWORD *pPowerY,		// Y �����ϵ�Ȩ�ر���������
					   OUT int *pMapY,
					   IN int nDestWidth,		// Ŀ�귶Χ
					   IN int nDestHeight,
					   IN int nSrcX,			// Դ���
					   IN int nSrcY,
					   IN int nSrcWidth,		// Դ��Χ
					   IN int nSrcHeight,
					   IN int nSrcBufferWidth,	// Դ Buffer ��С
					   IN int nSrcBufferHeight,
					   IN float fScaleWidth,	// ���ű���
					   IN float fScaleHeight,
					   IN float fSrcStartX,		// Դ�����������
					   IN float fSrcStartY)
{
	int nMapTmp(0), nLastWidth(nSrcBufferWidth - 2), nLastHeight(nSrcBufferHeight - 2);
	DWORD dwPowerTmp1(0), dwPowerTmp2(0);

	nDestWidth--; nDestHeight--;
	// ���� X �����Ȩ�ر���������
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
	// ���� Y �����Ȩ�ر���������
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
// BGRA 32 λɫ Buffer ˫���Բ�ֵ���ſ���
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchCopy_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
							  IN int nDestX,			// Ŀ�����
							  IN int nDestY,
							  IN int nDestWidth,		// Ŀ����������
							  IN int nDestHeight,
							  IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
							  IN int nDestBufferHeight,
							  IN DWORD* pSrc,			// Դ Buffer
							  IN int nSrcX,				// Դ���
							  IN int nSrcY,
							  IN int nSrcWidth,			// Դ��������
							  IN int nSrcHeight,
							  IN int nSrcBufferWidth,	// Դ Buffer ��С
							  IN int nSrcBufferHeight,
							  IN int nParity)			// ��ż������
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// �����Ч�Ĳ�����Χ
	if (!TPBLStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
			fSrcStartX, fSrcStartY,	nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// ����Ȩ�ر�����������ڴ�ռ�
	TPSSE_DECLARE(DWORD*) pPowerX = TPSSE_NEW(nDestWidth, DWORD);	// X ������Ŀ����ӳ�䵽Դ�е�Ȩ�ر�
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);			// X ������Ŀ����ӳ�䵽Դ�е�������
	TPSSE_DECLARE(DWORD*) pPowerY = TPSSE_NEW(nDestHeight, DWORD);	// Y ������Ŀ����ӳ�䵽Դ�е�Ȩ�ر�
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);		// Y ������Ŀ����ӳ�䵽Դ�е�������
	// Buffer ��ʼ��ַ
	DWORD *pDesLine = pDest + nDestY * nDestBufferWidth + nDestX, *pSrcLine = NULL;
	int nSrcPitch = (nSrcBufferWidth << 2);

	// ����Ȩ����ӳ������
	TPPower_BLStretch(pPowerX, pMapX, pPowerY, pMapY, nDestWidth, nDestHeight, nSrcX, nSrcY,
		nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight, fSrcStartX, fSrcStartY);
	// ���д���
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		pSrcLine = pSrc + pMapY[nLineIndex] * nSrcBufferWidth;
		bgra32_bl_copy_line(pDesLine, nDestWidth, pSrcLine, nSrcPitch, pPowerX, pMapX, pPowerY[nLineIndex]);
		pDesLine += nDestAddWidth;
	}
	// �ͷ�Ȩ�ر���������
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pMapY);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// ˫���Բ�ֵ����ʱ�ķ�Χ����
// ��Χ��Чʱ���� TRUE ��Чʱ���� FALSE
//////////////////////////////////////////////////////////////////////////
BOOL TPMeanStretch_CurRange(IN OUT int &nDestX,			// Ŀ�����
							IN OUT int &nDestY,
							IN OUT int &nDestWidth,		// Ŀ�귶Χ
							IN OUT int &nDestHeight,
							IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
							IN int nDestBufferHeight,
							IN OUT int &nSrcX,			// Դ���
							IN OUT int &nSrcY,
							IN OUT int &nSrcWidth,		// Դ��Χ
							IN OUT int &nSrcHeight,
							IN int nSrcBufferWidth,		// Դ Buffer ��С
							IN int nSrcBufferHeight,
							OUT float &fScaleWidth,		// ���ű���
							OUT float &fScaleHeight,
							OUT float &fSrcStartX,		// Դ�����������
							OUT float &fSrcStartY,
							OUT int &nDestAddWidth,		// Ŀ�� Buffer ����
							OUT int &nLineIndex,		// ����ʼ
							OUT int &nLineInc,			// ������
							IN int nParity)				// ��ż������
{
	// �߽���Ч�Լ��
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

	fScaleWidth = (float)nSrcWidth / nDestWidth;	// ���ű���
	fScaleHeight = (float)nSrcHeight / nDestHeight;
	// ���г�����Χ
	if (nDestX < 0)		// Ŀ����߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (int)((-nDestX) * fScaleWidth + 0.5f);	// Դ����������
		nDestWidth += nDestX; nDestX = 0;
		nSrcX += nTmpOffset; nSrcWidth -= nTmpOffset;
		if (nSrcX >= nSrcBufferWidth || nSrcWidth <= 0)
			return FALSE;
	}
	if (nDestY < 0)		// Ŀ���ϱ߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (int)((-nDestY) * fScaleHeight + 0.5f);	// Դ����������
		nDestHeight += nDestY; nDestY = 0;
		nSrcY += nTmpOffset; nSrcHeight -= nTmpOffset;
		if (nSrcY >= nSrcBufferHeight || nSrcHeight <= 0)
			return FALSE;
	}
	if (nSrcX < 0)		// Դ��߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (int)ceilf((-nSrcX) / fScaleWidth);		// Ŀ�����������
		nSrcWidth += nSrcX; nSrcX = 0;
		nDestWidth -= nTmpOffset; nDestX += nTmpOffset;
		if (nDestX >= nDestBufferWidth || nDestWidth <= 0)
			return FALSE;
	}
	if (nSrcY < 0)		// Դ�ϱ߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (int)ceilf((-nSrcY) / fScaleHeight);	// Ŀ�����������
		nSrcHeight += nSrcY; nSrcY = 0;
		nDestHeight -= nTmpOffset; nDestY += nTmpOffset;
		if (nDestY >= nDestBufferHeight || nDestHeight <= 0)
			return FALSE;
	}
	nTmpWidth = nDestWidth + nDestX;		// ����Ŀ����ұ߽�
	nTmpHeight = nDestHeight + nDestY;		// ����Ŀ����±߽�
	if (nTmpWidth > nDestBufferWidth)	// Ŀ���ұ߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (nTmpWidth - nDestBufferWidth);	// Ŀ�����������
		nDestWidth -= nTmpOffset;
		nTmpOffset = (int)(nTmpOffset * fScaleWidth);	// Դ����������
		nSrcWidth -= nTmpOffset;
		if (nSrcWidth <= 0)
			return FALSE;
	}
	if (nTmpHeight > nDestBufferHeight)	// Ŀ���±߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (nTmpHeight - nDestBufferHeight);	// Ŀ�����������
		nDestHeight -= nTmpOffset;
		nTmpOffset = (int)(nTmpOffset * fScaleHeight);	// Դ����������
		nSrcHeight -= nTmpOffset;
		if (nSrcHeight <= 0)
			return FALSE;
	}
	nTmpWidth = nSrcWidth + nSrcX;		// ����Դ���ұ߽�
	nTmpHeight = nSrcHeight + nSrcY;	// ����Դ���±߽�
	if (nTmpWidth > nSrcBufferWidth)	// Դ�ұ߽糬�������ŷ�Χ����������
	{
		nTmpOffset = (nTmpWidth - nSrcBufferWidth);		// Դ����������
		nSrcWidth -= nTmpOffset;
		nTmpOffset = (int)ceilf(nTmpOffset / fScaleWidth);	// Ŀ�����������
		nDestWidth -= nTmpOffset;
		if (nDestWidth <= 0)
			return FALSE;
	}
	if (nTmpHeight > nSrcBufferHeight)	// Դ�±߽糬����������Χ����������
	{
		nTmpOffset = (nTmpHeight - nSrcBufferHeight);	// Դ����������
		nSrcHeight -= nTmpOffset;
		nTmpOffset = (int)ceilf(nTmpOffset / fScaleHeight);	// Ŀ�����������
		nDestHeight -= nTmpOffset;
		if (nDestHeight <= 0)
			return FALSE;
	}
	// Դ�������
	fSrcStartX = fScaleWidth * (nDestX - nTmpDestX) + nTmpSrcX;
	fSrcStartY = fScaleHeight * (nDestY - nTmpDestY) + nTmpSrcY;
	// ������ż�����ͻ������
	switch (nParity)
	{
	case TP_PARITY_BOTH:		// ȫ��
		nDestAddWidth = nDestBufferWidth;
		nLineIndex = 0;
		nLineInc = 1;
		break;

	case TP_PARITY_ODD:			// �泡
		nLineIndex = 0;
		if ((nDestY % 2) == 1)		// �����ż���������һ�п�ʼ
		{
			nDestY++;
			nLineIndex = 1;
			// �жϷ�Χ����Ч��
			if (nDestY >= nDestBufferHeight)
				return FALSE;
		}
		nDestAddWidth = (nDestBufferWidth << 1);
		nLineInc = 2;
		break;

	case TP_PARITY_EVEN:		// ż��
		nLineIndex = 0;
		if ((nDestY % 2) == 0)		// ������泡�������һ�п�ʼ
		{
			nDestY++;
			nLineIndex = 1;
			// �жϷ�Χ����Ч��
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
// ���ڼ�Ȩƽ������ʱ����Ȩ�صĽӿ�
//////////////////////////////////////////////////////////////////////////
// ����ˮƽ��ֱ������ĳ�����ص�Ȩ�ظ���������βȨ��
int calc_power_pixel(OUT float* pPower,		// ��βȨ�ر�
					 IN OUT float& fStart,	// ��ʼ������ֵ
					 IN float fStep)		// ���Ų���
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

	// ��һ�μ���ʣ��ռһ��λ��
	float fLeave = fStep - (1.0f - fStart);
	int n = (int)floorf(fLeave);

	fStart = pPower[1] = (fLeave - n);

	return n + 2;
}

// ����ˮƽ��ֱ�����ϸ����ص�Ȩ�ظ���������βȨ�ر�
void strech_calc_power(OUT float* pPower,		// Ȩ�ر�
					   OUT int* pMap,			// ÿ����ռ�õ�Ȩ�ظ�����
					   IN int nCount,			// Ŀ�꿽�����Ȼ�߶�
					   IN float fStep,			// ���Ų���
					   IN float fStart/* = 0.0f*/)	// ��ʼ������ֵ
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
// BGRA 32 λɫ Buffer ��Ȩƽ����ֵ���ſ���
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchCopy_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
								IN int nDestX,				// Ŀ�����
								IN int nDestY,
								IN int nDestWidth,			// Ŀ����������
								IN int nDestHeight,
								IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
								IN int nDestBufferHeight,
								IN DWORD* pSrc,				// Դ Buffer
								IN int nSrcX,				// Դ���
								IN int nSrcY,
								IN int nSrcWidth,			// Դ��������
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		// Դ Buffer ��С
								IN int nSrcBufferHeight,
								IN int nParity)				// ��ż������
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// �����Ч�Ĳ�����Χ
	if (!TPMeanStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
			fSrcStartX, fSrcStartY, nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// ����Ȩ�ر�Ȩ������������ռ�
	float fScaleW_I = 1.0f / fScaleWidth, fScaleH_I = 1.0f / fScaleHeight;
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);		// X �����ϵ�Ȩ��������
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);	// Y �����ϵ�Ȩ��������
	TPSSE_DECLARE(float*) pPowerX = TPSSE_NEW((nDestWidth << 1), float);	// X �����ϵ�Ȩ�ر�
	TPSSE_DECLARE(float*) pPowerY = TPSSE_NEW((nDestHeight << 1), float);	// Y �����ϵ�Ȩ�ر�
	DWORD *pDesLine = pDest + nDestX + nDestY * nDestBufferWidth;		// Buffer ��ʼ��ַ
	DWORD *pSrcLine = pSrc + nSrcX + nSrcY * nSrcBufferWidth;
	float *pTmpPowerY = pPowerY;		// Y ����Ȩ�ر��ַ
	int nSrcPitch = (nSrcBufferWidth << 2);
	TPSSE_DECLARE(float*) pTmpLine = TPSSE_NEW((nSrcWidth << 2), float);	// Դ�е���ʱ����
	TPSSE_DECLARE(float) fCoefW[4] = { fScaleW_I, fScaleW_I, fScaleW_I, fScaleW_I };
	TPSSE_DECLARE(float) fCoefH[4] = { fScaleH_I, fScaleH_I, fScaleH_I, fScaleH_I };

	// ����Ȩ��
	strech_calc_power(pPowerX, pMapX, nDestWidth, fScaleWidth, fSrcStartX - (int)fSrcStartX);
	strech_calc_power(pPowerY, pMapY, nDestHeight, fScaleHeight, fSrcStartY - (int)fSrcStartY);
	// ���д���
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		if ((nSrcY + pMapY[nLineIndex]) > nSrcBufferHeight)	// ��ֹ�����Խ�����
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
// BGRA 32 λɫ Buffer ˫���Բ�ֵ���� Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchMix_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
							 IN int nDestX,				// Ŀ�����
							 IN int nDestY,
							 IN int nDestWidth,			// Ŀ����������
							 IN int nDestHeight,
							 IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
							 IN int nDestBufferHeight,
							 IN DWORD* pSrc,			// Դ Buffer
							 IN int nSrcX,				// Դ���
							 IN int nSrcY,
							 IN int nSrcWidth,			// Դ��������
							 IN int nSrcHeight,
							 IN int nSrcBufferWidth,	// Դ Buffer ��С
							 IN int nSrcBufferHeight,
							 IN int nParity)			// ��ż������
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// �����Ч�Ĳ�����Χ
	if (!TPBLStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
			fSrcStartX, fSrcStartY,	nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// ����Ȩ�ر�����������ڴ�ռ�
	TPSSE_DECLARE(DWORD*) pPowerX = TPSSE_NEW(nDestWidth, DWORD);	// X ������Ŀ����ӳ�䵽Դ�е�Ȩ�ر�
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);			// X ������Ŀ����ӳ�䵽Դ�е�������
	TPSSE_DECLARE(DWORD*) pPowerY = TPSSE_NEW(nDestHeight, DWORD);	// Y ������Ŀ����ӳ�䵽Դ�е�Ȩ�ر�
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);			// Y ������Ŀ����ӳ�䵽Դ�е�������
	TPSSE_DECLARE(DWORD*) pTmpBuf = TPSSE_NEW(nDestWidth, DWORD);	// ��ʱ����
	// Buffer ��ʼ��ַ
	DWORD *pDesLine = pDest + nDestY * nDestBufferWidth + nDestX, *pSrcLine = NULL;
	int nSrcPitch = (nSrcBufferWidth << 2);
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);

	// ����Ȩ����ӳ������
	TPPower_BLStretch(pPowerX, pMapX, pPowerY, pMapY, nDestWidth, nDestHeight, nSrcX, nSrcY,
		nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight, fSrcStartX, fSrcStartY);
	// ���д���
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		pSrcLine = pSrc + pMapY[nLineIndex] * nSrcBufferWidth;
		bgra32_bl_copy_line(pTmpBuf, nDestWidth, pSrcLine, nSrcPitch, pPowerX, pMapX, pPowerY[nLineIndex]);
		bgra32_line_mix(pDesLine, pTmpBuf, na16, nm16, g_AlphaGainTable);
		pDesLine += nDestAddWidth;
	}
	// �ͷ�Ȩ�ر���������
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pMapY);
	TPSSE_FREE(pTmpBuf);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer ˫���Բ�ֵ���� NotMix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchNotMix_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
								IN int nDestX,				// Ŀ�����
								IN int nDestY,
								IN int nDestWidth,			// Ŀ����������
								IN int nDestHeight,
								IN int nDestBufferWidth,	// Ŀ�� Buffer ��С
								IN int nDestBufferHeight,
								IN DWORD* pSrc,				// Դ Buffer
								IN int nSrcX,				// Դ���
								IN int nSrcY,
								IN int nSrcWidth,			// Դ��������
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		// Դ Buffer ��С
								IN int nSrcBufferHeight,
								IN int nParity)				// ��ż������
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// �����Ч�Ĳ�����Χ
	if (!TPBLStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
			nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
			fSrcStartX, fSrcStartY,	nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// ����Ȩ�ر�����������ڴ�ռ�
	TPSSE_DECLARE(DWORD*) pPowerX = TPSSE_NEW(nDestWidth, DWORD);	// X ������Ŀ����ӳ�䵽Դ�е�Ȩ�ر�
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);			// X ������Ŀ����ӳ�䵽Դ�е�������
	TPSSE_DECLARE(DWORD*) pPowerY = TPSSE_NEW(nDestHeight, DWORD);	// Y ������Ŀ����ӳ�䵽Դ�е�Ȩ�ر�
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);		// Y ������Ŀ����ӳ�䵽Դ�е�������
	TPSSE_DECLARE(DWORD*) pTmpBuf = TPSSE_NEW(nDestWidth, DWORD);	// ��ʱ����
	// Buffer ��ʼ��ַ
	DWORD *pDesLine = pDest + nDestY * nDestBufferWidth + nDestX, *pSrcLine = NULL;
	int nSrcPitch = (nSrcBufferWidth << 2);
	int na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);

	// ����Ȩ����ӳ������
	TPPower_BLStretch(pPowerX, pMapX, pPowerY, pMapY, nDestWidth, nDestHeight, nSrcX, nSrcY,
		nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight, fSrcStartX, fSrcStartY);
	// ���д���
	for (; nLineIndex<nDestHeight; nLineIndex+=nLineInc)
	{
		pSrcLine = pSrc + pMapY[nLineIndex] * nSrcBufferWidth;
		bgra32_bl_copy_line(pTmpBuf, nDestWidth, pSrcLine, nSrcPitch, pPowerX, pMapX, pPowerY[nLineIndex]);
		bgra32_line_notmix(pDesLine, pTmpBuf, na16, nm16, g_AlphaGainTable);
		pDesLine += nDestAddWidth;
	}
	// �ͷ�Ȩ�ر���������
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pMapY);
	TPSSE_FREE(pTmpBuf);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// BGRA 32 λɫ Buffer ��Ȩƽ����ֵ���� Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchMix_SSE(OUT DWORD* pDest,			// Ŀ�� Buffer
							   IN int nDestX,				// Ŀ�����
							   IN int nDestY,
							   IN int nDestWidth,			// Ŀ����������
							   IN int nDestHeight,
							   IN int nDestBufferWidth,		// Ŀ�� Buffer ��С
							   IN int nDestBufferHeight,
							   IN DWORD* pSrc,				// Դ Buffer
							   IN int nSrcX,				// Դ���
							   IN int nSrcY,
							   IN int nSrcWidth,			// Դ��������
							   IN int nSrcHeight,
							   IN int nSrcBufferWidth,		// Դ Buffer ��С
							   IN int nSrcBufferHeight,
							   IN int nParity)				// ��ż������
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// �����Ч�Ĳ�����Χ
	if (!TPMeanStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
		fSrcStartX, fSrcStartY, nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// ����Ȩ�ر�Ȩ������������ռ�
	float fScaleW_I = 1.0f / fScaleWidth, fScaleH_I = 1.0f / fScaleHeight;
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);		// X �����ϵ�Ȩ��������
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);	// Y �����ϵ�Ȩ��������
	TPSSE_DECLARE(float*) pPowerX = TPSSE_NEW((nDestWidth << 1), float);	// X �����ϵ�Ȩ�ر�
	TPSSE_DECLARE(float*) pPowerY = TPSSE_NEW((nDestHeight << 1), float);	// Y �����ϵ�Ȩ�ر�
	DWORD *pDesLine = pDest + nDestX + nDestY * nDestBufferWidth;		// Buffer ��ʼ��ַ
	DWORD *pSrcLine = pSrc + nSrcX + nSrcY * nSrcBufferWidth;
	float *pTmpPowerY = pPowerY;		// Y ����Ȩ�ر��ַ
	int nSrcPitch = (nSrcBufferWidth << 2), na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);
	TPSSE_DECLARE(float*) pTmpLine = TPSSE_NEW((nSrcWidth << 2), float);	// Դ�е���ʱ����
	TPSSE_DECLARE(DWORD*) pTmpBuf = TPSSE_NEW(nDestWidth, DWORD);			// ��ʱ����
	TPSSE_DECLARE(float) fCoefW[4] = { fScaleW_I, fScaleW_I, fScaleW_I, fScaleW_I };
	TPSSE_DECLARE(float) fCoefH[4] = { fScaleH_I, fScaleH_I, fScaleH_I, fScaleH_I };

	// ����Ȩ��
	strech_calc_power(pPowerX, pMapX, nDestWidth, fScaleWidth, fSrcStartX - (int)fSrcStartX);
	strech_calc_power(pPowerY, pMapY, nDestHeight, fScaleHeight, fSrcStartY - (int)fSrcStartY);
	// ���д���
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
// BGRA 32 λɫ Buffer ��Ȩƽ����ֵ���� Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchNotMix_SSE(OUT DWORD* pDest,				// Ŀ�� Buffer
								  IN int nDestX,				// Ŀ�����
								  IN int nDestY,
								  IN int nDestWidth,			// Ŀ����������
								  IN int nDestHeight,
								  IN int nDestBufferWidth,		// Ŀ�� Buffer ��С
								  IN int nDestBufferHeight,
								  IN DWORD* pSrc,				// Դ Buffer
								  IN int nSrcX,					// Դ���
								  IN int nSrcY,
								  IN int nSrcWidth,				// Դ��������
								  IN int nSrcHeight,
								  IN int nSrcBufferWidth,		// Դ Buffer ��С
								  IN int nSrcBufferHeight,
								  IN int nParity)				// ��ż������
{
	if (pSrc == NULL || pDest == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDestAddWidth(0), nLineIndex(0), nLineInc(1);

	// �����Ч�Ĳ�����Χ
	if (!TPMeanStretch_CurRange(nDestX, nDestY, nDestWidth, nDestHeight, nDestBufferWidth, nDestBufferHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcBufferWidth, nSrcBufferHeight, fScaleWidth, fScaleHeight,
		fSrcStartX, fSrcStartY, nDestAddWidth, nLineIndex, nLineInc, nParity))
		return FALSE;

	// ����Ȩ�ر�Ȩ������������ռ�
	float fScaleW_I = 1.0f / fScaleWidth, fScaleH_I = 1.0f / fScaleHeight;
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDestWidth, int);	// X �����ϵ�Ȩ��������
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDestHeight, int);	// Y �����ϵ�Ȩ��������
	TPSSE_DECLARE(float*) pPowerX = TPSSE_NEW((nDestWidth << 1), float);	// X �����ϵ�Ȩ�ر�
	TPSSE_DECLARE(float*) pPowerY = TPSSE_NEW((nDestHeight << 1), float);	// Y �����ϵ�Ȩ�ر�
	DWORD *pDesLine = pDest + nDestX + nDestY * nDestBufferWidth;		// Buffer ��ʼ��ַ
	DWORD *pSrcLine = pSrc + nSrcX + nSrcY * nSrcBufferWidth;
	float *pTmpPowerY = pPowerY;		// Y ����Ȩ�ر��ַ
	int nSrcPitch = (nSrcBufferWidth << 2), na16 = (nDestWidth >> 2), nm16 = ((nDestWidth & 0x03) << 2);
	TPSSE_DECLARE(float*) pTmpLine = TPSSE_NEW((nSrcWidth << 2), float);	// Դ�е���ʱ����
	TPSSE_DECLARE(DWORD*) pTmpBuf = TPSSE_NEW(nDestWidth, DWORD);		// ��ʱ����
	TPSSE_DECLARE(float) fCoefW[4] = { fScaleW_I, fScaleW_I, fScaleW_I, fScaleW_I };
	TPSSE_DECLARE(float) fCoefH[4] = { fScaleH_I, fScaleH_I, fScaleH_I, fScaleH_I };

	// ����Ȩ��
	strech_calc_power(pPowerX, pMapX, nDestWidth, fScaleWidth, fSrcStartX - (int)fSrcStartX);
	strech_calc_power(pPowerY, pMapY, nDestHeight, fScaleHeight, fSrcStartY - (int)fSrcStartY);
	// ���д���
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
// �ļ�����