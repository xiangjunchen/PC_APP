/********************************************************************
Copyright (c) 2004, Sobey���� ����������
All rights reserved.
����ʱ��:	2004��2��10��   18:14
�ļ���: 	c:\ndsrc\chenyufeng_desktop_2\devvob\sobeydesktop\src\tpcommon\tpbufferoperate\tpbufferoperateapi.cpp
��ǰ�汾��	1.0
����:		�����
ժҪ:		�Ի��������ֲ����ļ����㷨��ʵ��
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
// ȫ�ֱ���
extern float *g_pfTable;
extern float *g_pfPosTable;
extern LPNXVIDEOPROCAPI g_lpVideoProcAPI; 

//////////////////////////////////////////////////////////////////////////
// ˽�к�������
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
// ��������

//////////////////////////////////////////////////////////////////////////
// ����һ�� Buffer
LRESULT CALLBACK TP_MemcpyLine(OUT void *des,		// Ŀ��
							   IN void *src,		// Դ
							   IN int nlen)			// �ֽڳ���
{
	memcpy_line(des, src, (nlen >> 4), (nlen & 0x0f));

	return TRUE;
}

// ���ֽ�Ϊ��λ����һ�� Buffer
LRESULT CALLBACK TP_MemsetChar(OUT void *des,		// Ŀ��
							   IN char value,		// �ֽ�ֵ
							   IN int nlen)			// �ֽڳ���
{
	memset_char_line(des, value, (nlen >> 4), (nlen & 0x0f));

	return TRUE;
}

// һ�� Buffer ��˫����ֵ����
LRESULT CALLBACK TP_MemsetDWORD(OUT DWORD *des,		// Ŀ��
								IN DWORD value,		// Դ
								IN int nsize)		// ˫�ֵĸ���
{
	memset_dword_line(des, value, (nsize >> 2), ((nsize & 0x03) << 2));

	return TRUE;
}

// ��ת
BOOL CALLBACK TP_OverTurnSelf(IN OUT void* pBuf,    // �ڴ�
							  IN int nPitch,        // һ���ֽڿ�ȣ����ֽڶ��룩
							  IN int nHeight)       // �߶�
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
BOOL CALLBACK TP_OverTurnCopy(OUT void* pDes,       // Ŀ��
							  IN int nDesPitch,     // Ŀ��һ���ֽڿ�ȣ����ֽڶ��룩
							  IN void* pSrc,        // Դ
							  IN int nSrcPitch,     // Դһ���ֽڿ�ȣ����ֽڶ��룩
							  IN int nHeight)       // �߶�
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

// ��Դ unit �е�ĳ���������� cell ���ѵ�����Ŀ����
// һ�� unit ���ֽڳ�Ϊ 32/16/8/4/2��cell ��λ��Ϊ 8��Ҳ��һ���ֽڣ�
// ���罫һ�� BGRA �����е� A ͨ��ȡ��������ȡ 8 ͨ����Ƶ�ĵ� 1 ͨ��
// Ҫ����������Լ�Ҫ������ cell ���������߼���ȫ��
void CALLBACK TP_MemSplit(OUT void *des,			// Ŀ�꣨���ѽ����
						  IN void *src,				// Դ
						  IN int nUnitCount,		// ���� unit �ĸ���
						  IN int nUnitSize,			// unit ���ֽڳ�
						  IN int nCellStart,		// unit �� cell �ķ������
						  IN int nCellCount)		// unit �� cell �ķ��Ѹ���
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
	case 2:		// 16 λ
		if (nCellStart == 0)	// ���ѵ�һ���ֽ�
		{
			memsplit_2_0_1(des, src, nUnitCount);
		}
		else		// ���ѵڶ����ֽ�
		{
			memsplit_2_1_1(des, src, nUnitCount);
		}
		break;
	case 4:		// 32 λ
		switch (nCellStart)
		{
		case 0:
			if (nCellCount == 1)	// ���ѵ�һ���ֽ�
			{
				memsplit_4_0_1(des, src, nUnitCount);
			}
			else if (nCellCount == 2)	// ����ǰ�����ֽ�
			{
				memsplit_4_0_2(des, src, nUnitCount);
			}
			else	// ����ǰ�����ֽ�
			{
				memsplit_4_0_3(des, src, nUnitCount);
			}
			break;
		case 1:
			if (nCellCount == 1)	// ���ѵڶ����ֽ�
			{
				memsplit_4_1_1(des, src, nUnitCount);
			}
			else if (nCellCount == 2)	// �����м������ֽ�
			{
				memsplit_4_1_2(des, src, nUnitCount);
			}
			else		// ���Ѻ������ֽ�
			{
				memsplit_4_1_3(des, src, nUnitCount);
			}
			break;
		case 2:
			if (nCellCount == 1)	// ���ѵ������ֽ�
			{
				memsplit_4_2_1(des, src, nUnitCount);
			}
			else	// ������������ֽ�
			{
				memsplit_4_2_2(des, src, nUnitCount);
			}
			break;
		case 3:			// �������һ���ֽ�
			memsplit_4_3_1(des, src, nUnitCount);
			break;
		}
		break;
	case 8:
		if (nCellCount != 4 || (nCellStart & 0x03) != 0)	// ������Ƶ��������������������
		{
			memsplit_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
		}
		else		// 2 ͨ����Ƶȡ 1 ͨ��
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
	default:	// �������
		memsplit_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
		break;
	}
}
// ��Դ��ĳ���������� cell �ϳɵ���Ŀ�� unit ��
// һ�� unit ���ֽڳ�Ϊ 32/16/8/4/2��cell ��λ��Ϊ 8��Ҳ��һ���ֽڣ�
// ���罫һ�� A ͨ���ϳɵ�һ�� BGRA �����У����罫 1 ͨ������Ƶ�ϳɵ� 8 ͨ����
// Ҫ����������Լ�Ҫ������ cell ���������߼���ȫ��
void CALLBACK TP_MemCompose(OUT void *des,			// Ŀ�꣨�ϳɽ����
							IN void *src,			// Դ
							IN int nUnitCount,		// �ϳ� unit �ĸ���
							IN int nUnitSize,		// unit ���ֽڳ�
							IN int nCellStart,		// unit �� cell �ĺϳ����
							IN int nCellCount)		// unit �� cell �ĺϳɸ���
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
	case 2:		// 16 λ
		if (nCellStart == 0)	// �ϳɵ���һ���ֽ�
		{
			memcompose_2_0_1(des, src, nUnitCount);
		}
		else		// �ϳɵ��ڶ����ֽ�
		{
			memcompose_2_1_1(des, src, nUnitCount);
		}
		break;
	case 4:		// 32 λ
		switch (nCellStart)
		{
		case 0:
			if (nCellCount == 1)	// �ϳɵ���һ���ֽ�
			{
				memcompose_4_0_1(des, src, nUnitCount);
			}
			else if (nCellCount == 2)	// �ϳɵ�ǰ�����ֽ�
			{
				memcompose_4_0_2(des, src, nUnitCount);
			}
			else	// �ϳɵ�ǰ�����ֽڣ���������ϳ�
			{
				memcompose_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
			}
			break;
		case 1:
			if (nCellCount == 1)	// �ϳɵ��ڶ����ֽ�
			{
				memcompose_4_1_1(des, src, nUnitCount);
			}
			else		// �ϳɵ��м������ֽڻ�������ֽڣ���������ϳ�
			{
				memcompose_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
			}
			break;
		case 2:
			if (nCellCount == 1)	// �ϳɵ��������ֽ�
			{
				memcompose_4_2_1(des, src, nUnitCount);
			}
			else	// �ϳɵ���������ֽ�
			{
				memcompose_4_2_2(des, src, nUnitCount);
			}
			break;
		case 3:			// �ϳɵ����һ���ֽ�
			memcompose_4_3_1(des, src, nUnitCount);
			break;
		}
		break;
	case 8:
		if (nCellCount != 4 || (nCellStart & 0x03) != 0)	// ������Ƶ�����������������ϳ�
		{
			memcompose_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
		}
		else		// 1 ͨ����Ƶ�ϳɵ� 2 ͨ���� 
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
	default:	// ����ϳ�
		memcompose_n_n_n(des, src, nUnitCount, nUnitSize, nCellStart, nCellCount);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// RGB Mean ����
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
// ���ܣ���ָ����ʽ����Դ��������Ŀ�껺����
// ������pdwDestBuffer:Ŀ�껺����ָ��
//		 nDestX:Ŀ�껺����������
//		 nDestY:Ŀ�껺����������
//		 nDestWidth:Ŀ�껺�������
//		 nDestHeight:Ŀ�껺�����߶�
//		 nDestBufferWidth:Ŀ�껺�������п��
//		 nDestBufferHeight:Ŀ�껺�������и߶�
//		 pdwSrcBuffer:Դ������ָ��
//		 nSrcX:Դ������������
//		 nSrcY:Դ������������
//		 nSrcWidth:Դ���������
//		 nSrcHeight:Դ�������߶�
//		 nSrcBufferWidth:Դ���������п��
//		 nSrcBufferHeight:Դ���������и߶�
//		 nMix:��������
//		 nParity:��ż��(������ɨ��ʱ��ż�У�ż��ɨ��ʱ������)
// ����ֵ���Ƿ񿽱��ɹ�
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
	// ����Buffer������ʱ�����쿽��
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
// ���ܣ��򵥵�BitBlt�㷨���ٶȿ죬���ڷǱ��еĿ���BitBlt
// 
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_QuickBitBlt(
								IN OUT DWORD* pdwDestBuffer, // Ŀ�껺����
								IN int nDestX,				 // Ҫ�����������Ŀ�껺�������������
								IN int nDestY,
								IN int nDestWidth,			 // Ҫ������Ŀ�����
								IN int nDestHeight,
								IN int nDestBufferWidth,	 // Ŀ�껺��������
								IN int nDestBufferHeight,
								IN DWORD* pdwSrcBuffer,		 // Դ������
								IN int nSrcX,				 // Ҫ�����������Դ���������������
								IN int nSrcY,
								IN int nSrcWidth,			 // Ҫ������Դ����
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		 // Դ����������
								IN int nSrcBufferHeight,
								IN int nMix,				 // ����������Ĳ���ģʽ��TP_BUFFER_MIX, TP_BUFFER_COPY, TP_BUFFER_NOMIX
								IN int nParity)				 // ��ż����TP_PARITY_BOTH, TP_PARITY_ODD, TP_PARITY_EVEN
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
// ���ܣ����쿽��
// ������
// ����ֵ��
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
							   IN int nMix,					// ���ģʽ
							   IN int nParity)				// ��ż��
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
// ���ܣ����ݼ�Ȩ���Ӽ���Maskֵ:blur��ģ������������
// ������pbyMask��Ҫ�������ģ������ָ��
//		 nStartX����ģ���X����
//		 nStartY����ģ���Y����
//		 nMaskWidth����ģ���
//		 nMaskHeight����ģ�߶�
//		 pnOperator����Ȩ��������ָ��
//		 nOperatorWidth��������
//		 nOperatorHeight������߶�
//		 nMainOperatorX�������ص�X����
//		 nMainOperatorY�������ص�Y����
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
	int nWidth = nMaskWidth - nStartX;			// Ҫ�����Mask�����߶�
	int nHeight = nMaskHeight - nStartY;
	if(nWidth <= 0 || nHeight <= 0)
		return;
	if(nMainOperatorX < 0 || nMainOperatorY < 0 || nMainOperatorX >= nOperatorWidth || nMainOperatorY >= nOperatorHeight)
		return;
	//	nWidth = nWidth - (nOperatorWidth - 1);	// Ҫ����Ŀ����߶�
	//	nHeight = nHeight - (nOperatorHeight - 1);
	BYTE *pSrcMask = pbyMask + nStartY*nMaskWidth + nStartX;	// ���ָ��
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
	// һ����Mask�ĸ���
	__asm
	{
		XOR ECX, ECX
			MOVSS XMM7, fCalSum
			PSHUFD XMM7, XMM7, 0		// ���4��nCalSum
			MOV EDX, pTempBuf1			// ��ʱ������ָ��
LOOPX:
		CMP ECX, nHeight				// x<nHeight
			JGE LOOPXEND

			PUSH ECX
			XOR ECX, ECX
			MOV EAX, pDest					// ��ʱ���ص�ָ��
LOOPY:
		ADD ECX, 4
			CMP ECX, nWidth				// y<nWidth
			JG LOOPY2

			PUSH ECX
			PUSH EAX
			PUSH EDX
		{
			XOR ECX, ECX
				PXOR XMM0, XMM0		// ��ż���Ľ��
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
				MOV  AL,  [ECX+EDX]			// ����pbyTempOperator[j]ֵ��AX�ĵ�8λ
				MOVD XMM1,EAX 
					POP EAX	
					PSHUFD XMM1, XMM1, 0		// �����XMM1���ĸ���ͬ��ֵ
					MOVD XMM2, [ECX+EAX]		// ����pbyTempDest[j]������ĸ�BYTEֵ
					PXOR XMM3, XMM3				// ת����4��DWORDֵ
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
					CVTDQ2PS XMM0, XMM0				// ת����float
						MULPS XMM0,	XMM7				// nSum*1.0f/nCalSum
						POP EDX
						POP EAX
						POP ECX
						ADD EAX, 4							// pDest+4
		}
		CVTPS2DQ XMM0, XMM0					// ת����DWORD
			PACKSSDW XMM0, XMM0					// �����4��Byte�������4�ֽ�
			PACKUSWB XMM0, XMM0						
			MOVD     [EDX], XMM0				// *pTempBuf1 = nSum/nCalSum;
			ADD EDX, 4							// pTempBuf1+4,����4

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
				PXOR XMM0, XMM0		// ��ż���Ľ��
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
				MOV  AL,  [ECX+EDX]			// ����pbyTempOperator[j]ֵ
				MOVD XMM1,EAX 
					POP EAX	
					PUSH EDX					// ����pbyTempDest[j]
					XOR EDX, EDX
					MOV DL, [ECX+EAX]
					MOVD XMM2, EDX
						POP EDX 		
						PXOR XMM3, XMM3				// ת����4��DWORDֵ
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
					CVTDQ2PS XMM0, XMM0				// ת����float
						MULPS XMM0,	XMM7				// nSum*1.0f/nCalSum
						POP EDX
						POP EAX
						POP ECX
						ADD EAX, 1							// pDest+1
		}
		CVTPS2DQ XMM0, XMM0					// ת����DWORD
			PACKSSDW XMM0, XMM0					// �����4��Byte�������4�ֽ�
			PACKUSWB XMM0, XMM0		
			PUSH EAX
			MOVD  EAX, XMM0
			MOV     [EDX], AL					// *pTempBuf1 = nSum/nCalSum;
			POP EAX
			ADD EDX, 1							// pTempBuf1+1,����1

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

	// �����2004.2.1,����ߣ������
	// ����˶�Mask��Ե�ļ���
#if 1								// ����ʵ��
	int nCurHeight = 0;					// ��ǰ�����
	int nCurWidth = 0;
	int nCurX = 0;
	int nCurY = 0;
	int nCurOperateHeight = 0;
	int nCurOperateWidth = 0;
	// һ����Mask�ĸ���
	__asm
	{
		MOVSS XMM7, fCalSum
			PSHUFD XMM7, XMM7, 0		// ���4��fCalSum��XMM7��
			MOV EDX, pTempBuf1			// ��ʱ������ָ��
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
			PXOR XMM0, XMM0		// ��ż���Ľ��SUM
				MOV EDX, pbyOperator// ����ָ��
LOOPI:
			MOV ECX, nCurOperateHeight
				CMP ECX, nOperatorHeight	// nCurOperateHeight<nOperatorHeight
				JGE LOOPIEND

				MOV EAX, nCurHeight			// ������Ե�CurY,����nCurY
				ADD EAX, ECX
				SUB EAX, nMainOperatorY
				CMP EAX, 0					// nCurY>=0��nCurY<nHeight
				JL  LOOPJEND
				CMP EAX, nHeight
				JGE LOOPJEND
				MOV nCurY, EAX
LOOPJ:
			MOV ECX, nCurOperateWidth
				CMP ECX, nOperatorWidth		// nCurOperateWidth<nOperatorWidth
				JGE LOOPJEND

				XOR EAX, EAX
				MOV  AL,  [ECX+EDX]			// ����pbyOperator[nCurOperateWidth]ֵ
				MOVD XMM1,EAX 
					PSHUFD XMM1, XMM1, 0		// �����XMM1���ĸ���ͬ������ֵ

					PXOR XMM2, XMM2				// ���pDest��ֵ
					PXOR XMM3, XMM3
					PXOR XMM4, XMM4
					PXOR XMM5, XMM5
					PUSH EDX
					MOV EAX, nCurY				// pDest+nCurY*nMaskWidth
					MUL nMaskWidth
					MOV EDX, pDest
					ADD EDX, EAX

					MOV ECX, nCurWidth			// ������Ե�CurX
					ADD ECX, nCurOperateWidth
					SUB ECX, nMainOperatorX
					CMP ECX, 0					// CurX>=0��nCurX<nWidth
					JL  CurX1
					CMP ECX, nWidth
					JGE CurX1
					XOR EAX, EAX
					MOV AL,  [EDX+ECX]			// ȡ��pDest[nCurX][nCurY]��ֵ
					MOVD XMM2, EAX
CurX1:
					INC ECX						// CurX+1
						CMP ECX, 0					// CurX>=0��nCurX<nWidth
						JL  CurX2
						CMP ECX, nWidth
						JGE CurX2
						XOR EAX, EAX
						MOV AL,  [EDX+ECX]			// ȡ��pDest[nCurX][nCurY]��ֵ
						MOVD XMM3, EAX
							PUNPCKLDQ XMM2, XMM3
CurX2:
						INC ECX						// CurX+1
							CMP ECX, 0					// CurX>=0��nCurX<nWidth
							JL  CurX3
							CMP ECX, nWidth
							JGE CurX3
							XOR EAX, EAX
							MOV AL,  [EDX+ECX]			// ȡ��pDest[nCurX][nCurY]��ֵ
							MOVD XMM4, EAX
CurX3:
							INC ECX						// CurX+1
								CMP ECX, 0					// CurX>=0��nCurX<nWidth
								JL  CurXEnd			
								CMP ECX, nWidth
								JGE CurXEnd
								XOR EAX, EAX
								MOV AL,  [EDX+ECX]			// ȡ��pDest[nCurX][nCurY]��ֵ
								MOVD XMM5, EAX
									PUNPCKLDQ XMM4, XMM5
CurXEnd:
								PUNPCKLQDQ XMM2, XMM4		// ��ϳ�4��DWORD����XMM2
									POP EDX

									PMULLW XMM1, XMM2			// pbyOperator[j]*pDest[j]
									PADDD XMM0, XMM1			// nSum += pOperator[j]*pDest[j]

									MOV ECX, nCurOperateWidth
									INC ECX
									MOV nCurOperateWidth, ECX
									JMP LOOPJ
LOOPJEND:
								MOV nCurOperateWidth, 0		// nCurOperateWidth ��0
									MOV ECX, nCurOperateHeight
									INC ECX
									MOV nCurOperateHeight, ECX
									ADD EDX, nOperatorWidth			// pbyTempOperator += nOperatorWidth;
									JMP LOOPI
LOOPIEND:
								MOV nCurOperateHeight, 0		// nCurOperateHeight ��0
									CVTDQ2PS XMM0, XMM0				// ת����float
									MULPS XMM0,	XMM7				// nSum*1.0f/nCalSum
									POP EDX							// ����pTempBufָ��
		}
		CVTPS2DQ XMM0, XMM0					// ת����DWORD
			PACKSSDW XMM0, XMM0					// �����4��Byte�������4�ֽ�
			PACKUSWB XMM0, XMM0						
			MOVD     [EDX], XMM0				// *pTempBuf1 = nSum/nCalSum;
			ADD EDX, 4							// pTempBuf1+4,����4

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
			PXOR XMM0, XMM0		// ��ż���Ľ��SUM
				MOV EDX, pbyOperator// ����ָ��
LOOPI1:
			MOV ECX, nCurOperateHeight
				CMP ECX, nOperatorHeight	// nCurOperateHeight<nOperatorHeight
				JGE LOOPIEND1

				MOV EAX, nCurHeight			// ������Ե�CurY,����nCurY
				ADD EAX, ECX
				SUB EAX, nMainOperatorY
				CMP EAX, 0					// nCurY>=0��nCurY<nHeight
				JL  LOOPJEND1
				CMP EAX, nHeight
				JGE LOOPJEND1
				MOV nCurY, EAX
LOOPJ1:
			MOV ECX, nCurOperateWidth
				CMP ECX, nOperatorWidth		// nCurOperateWidth<nOperatorWidth
				JGE LOOPJEND1

				XOR EAX, EAX
				MOV  AL,  [ECX+EDX]			// ����pbyOperator[nCurOperateWidth]ֵ
				MOVD XMM1,EAX 

					PXOR XMM2, XMM2				// ���pDest��ֵ
					PUSH EDX
					MOV EAX, nCurY				// pDest+nCurY*nMaskWidth
					MUL nMaskWidth
					MOV EDX, pDest
					ADD EDX, EAX

					MOV ECX, nCurWidth			// ������Ե�CurX
					ADD ECX, nCurOperateWidth
					SUB ECX, nMainOperatorX
					CMP ECX, 0					// CurX>=0��nCurX<nWidth
					JL  CurXEnd1
					CMP ECX, nWidth
					JGE CurXEnd1
					XOR EAX, EAX
					MOV AL,  [EDX+ECX]			// ȡ��pDest[nCurX][nCurY]��ֵ
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
					MOV nCurOperateWidth, 0		// nCurOperateWidth ��0
						MOV ECX, nCurOperateHeight
						INC ECX
						MOV nCurOperateHeight, ECX
						ADD EDX, nOperatorWidth			// pbyTempOperator += nOperatorWidth;
						JMP LOOPI1
LOOPIEND1:
					MOV nCurOperateHeight, 0		// nCurOperateHeight ��0
						CVTDQ2PS XMM0, XMM0				// ת����float
						MULPS XMM0,	XMM7				// nSum*1.0f/nCalSum
						POP EDX							// ����pTempBufָ��
		}
		CVTPS2DQ XMM0, XMM0					// ת����DWORD
			PACKSSDW XMM0, XMM0					// �����4��Byte�������4�ֽ�
			PACKUSWB XMM0, XMM0						
			MOVD     EAX, XMM0				// *pTempBuf1 = nSum/nCalSum;
			MOV      [EDX], AL
			ADD EDX, 1							// pTempBuf1+1,����1

			MOV ECX, nCurWidth
			INC ECX
			MOV nCurWidth, ECX
			JMP LOOPY3
LOOPYEND:
		MOV nCurWidth, 0				// nCurWidth ��0
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
						// ��ǰ������
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
					// ��ǰ������
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
// ���ܣ������ģ
// 
// 2003��10��10��V1.0,��д�ߣ������
//
// 2003��11��20��V2.0,�޸��ߣ������
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK TP_FillMaskColor(
							   IN OUT DWORD* pDesBuffer, 
							   IN int nStartX,						// ҪMask��Ŀ�����
							   IN int nStartY, 
							   IN int nWidth,						// Ŀ�껺�����߶�����
							   IN int nHeight, 
							   IN BYTE* pSrcMask, 
							   IN int nMaskStartX,					// Mask�����
							   IN int nMaskStartY, 
							   IN int nMaskWidth,					// Mask�ĸ߶�����
							   IN int nMaskHeight)
{
	// ���ݷ�Χ���
	if(nStartX>=nWidth || nStartY>=nHeight)
		return TRUE;
	if(nWidth<=0 || nHeight<=0 || nMaskWidth<=0 || nMaskHeight<=0)
		return TRUE;
	if(nMaskStartX>=nMaskWidth || nMaskStartY>=nMaskHeight)
		return TRUE;

	// �õ��������͸߶ȿ��
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
	int nAddDestWidth = nWidth<<2;  // ÿ�е��ֽ���
	int nAddSrcWidth = nMaskWidth;
	//	DWORD* pDest1 = pDest;

#if 0
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};
	// һ���и�ֵ,SSE2�Ż�
	_asm
	{
		XOR EAX, EAX	// i��0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // ��һ��ѭ��(i<nMaskHeight)
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(i>=nMaskHeight)
			XOR ECX, ECX	 // j��0�� �ڶ���ѭ��������
			PUSH EAX
LOOP2:
		ADD ECX, 2			// ÿ�μ���2��ֵ������2ʱ����ѭ����2��
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2	

			MOV EAX, pSrc	// һ�ζ�ȡpSrc[j]������BYTEֵ����MM0

			MOV EDX, DWORD PTR [EAX+ECX]	// EDX��������BYTEֵ

			PUSH ECX
				MOV			ECX,EDX
				SHL			EDX,24
				OR			EDX,0x00ffffff		// EDX���AFFF
				SHR			ECX,8
				SHL			ECX,24
				OR			ECX,0x00ffffff		// ECX���AFFF
				MOVD		MM0,EDX
				PXOR		MM2, MM2
				PUNPCKLBW	MM0,MM2				// MM0���0A0F0F0F

				MOVD		MM1,ECX
				PUNPCKLBW	MM1,MM2				// MM1���0A0F0F0F
				MOVQ2DQ		XMM2,MM0
				MOVQ2DQ		XMM6,MM1
				PUNPCKLQDQ  XMM2,XMM6			// XMM2���0A0F0F0F0A0F0F0F
				POP ECX

				MOV EAX, pDest	 // ��ȡpDest[j]��ֵ
				MOV EDX, ECX
				SHL EDX, 2
				ADD EAX, EDX
				PUSH  EAX		 // ����ָ�룬����Ҫ��
				MOVD MM0, [EAX]
				ADD EAX, 4		// ��ȡpDest[j+1]��ֵ
					MOVD MM1, [EAX]
					XOR EAX, EAX	// ��0
						MOVD MM2, EAX
						PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
						PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
						MOVQ2DQ XMM1, MM0
						MOVQ2DQ XMM3, MM1
						PUNPCKLQDQ XMM1, XMM3 // ��Ϊ0R0G0B0A0R0G0B0A

						PMULLW XMM1, XMM2	// ��Ӧ16λ���SA
						// ����
						MOVQ2DQ		XMM7,MM5
						MOVLHPS		XMM7,XMM7
						PADDSW		XMM1,XMM7
						PSRLW  XMM1, 8		// ÿ16λ����8λ���൱��/255)(Ҫ��������)

						PACKUSWB XMM1, XMM1 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

						MOVDQ2Q	MM0,XMM1	// ȡ��64λ(����DWORD)
						POP EAX					// ����pDest
						MOVQ  [EAX], MM0

						ADD ECX, 2			// j��2�����صڶ���ѭ��
						JMP LOOP2
LOOP2BODY2:
					SUB ECX, 2			// ����2ʱ
						CMP ECX, nDestWidth
						JGE LOOP2END	

						MOV EAX, pSrc		// ��ȡpSrc[j]��ֵ
						MOV EDX, DWORD PTR [EAX+ECX]

						PUSH ECX
							SHL			EDX,24
							OR			EDX,0x00ffffff		// EDX���AFFF
							XOR ECX, ECX
							MOVD MM3, EDX
							MOVD MM4, ECX
							PUNPCKLBW MM3, MM4			// ��Ϊ0A0F0F0F

							POP ECX

							MOV EAX, pDest	 // ��ȡpDest[j]��ֵ
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // ����ָ�룬����Ҫ��
							MOVD MM1, [EAX]
							XOR EAX, EAX	// ��0
								MOVD MM2, EAX
								PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A

								PMULLW MM1, MM3		// ��Ӧ16λ���SA
								// ����
								PADDSW MM1, MM5
								PSRLW  MM1, 8		// ����8λ���൱��/255)(Ҫ��������)

								PACKUSWB MM1, MM1 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

								POP EAX					// ����pDest
								MOVD  [EAX], MM1
LOOP2END:
							MOV EAX, pDest
								ADD EAX, nAddDestWidth		// nWidth���4
								MOV [pDest], EAX

								MOV EAX, pSrc
								ADD EAX, nAddSrcWidth
								MOV [pSrc], EAX

								POP EAX
								INC	EAX			 // ����������������ѭ��1 
								JMP LOOP1
EXITLOOP1:
							EMMS			// ���
	}
#endif
	// �Ľ��㷨��2003��11��20
#if 1
	// һ���и�ֵ,SSE2�Ż�
	_declspec(align(16)) DWORD dwMask1[]={0x00FFFFFF,0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF};
	_declspec(align(16)) DWORD dwMask2[]={0x000000FF,0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX	// i��0
			MOVDQA XMM4, dwMask1
			MOVDQA XMM5, dwMask2
			MOV ESI, pDest
			MOV EDI, pSrc
LOOP1:
		CMP ECX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(i>=nDestHeight)
			PUSH ECX
			XOR ECX, ECX	 // j��0�� �ڶ���ѭ��������
			MOV EAX, EDI	 // Դ��Ŀ�껺����ָ�� 
			MOV EDX, ESI
LOOP2:
		ADD ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����2��
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			// ȡ��Dest��Ӧ��Floatֵ
			MOVDQU XMM0, [EDX]
			MOVAPS XMM6, XMM0
				PAND XMM6, XMM4		// ���0RGB0RGB0RGB0RGB
				PSRLD XMM0, 24
				PAND XMM0, XMM5		// ���000A000A000A000A
				CVTDQ2PS XMM7, XMM0
				PUSH EDX
				// ȡ���ĸ�Mask��Ӧ��Floatֵ
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
												CVTPS2DQ XMM0, XMM7		// 4�����Aֵ����XMM0
												PSLLD XMM0, 24			// ����24λ�����A000A000A000A000
												POR XMM0, XMM6			// �ϲ�ARGB
												MOVDQU [EDX], XMM0

												ADD EDX, 10H
												JMP LOOP2
LOOP2BODY2:
											SUB ECX, 4			// ����4ʱ
LOOP2BODY3:
											CMP ECX, nDestWidth
												JGE LOOP2END	

												MOVD XMM0, [EDX]
												MOVAPS XMM6, XMM0
													PAND XMM6, XMM4		// ���0RGB0RGB0RGB0RGB
													PSRLD XMM0, 24
													PAND XMM0, XMM5		// ���000A000A000A000A
													CVTDQ2PS XMM7, XMM0
													PUSH EDX
													// ȡ���ĸ�Mask��Ӧ��Floatֵ
													XOR EDX, EDX
													MOV DL, [EAX]
													SHL EDX, 2
														ADD EDX, g_pfPosTable
														MOVSS XMM1, [EDX]

														MULSS XMM7, XMM1
															INC EAX
															POP EDX
															INC ECX
															CVTPS2DQ XMM0, XMM7		// 4�����Aֵ����XMM0
															PSLLD XMM0, 24			// ����24λ�����A000A000A000A000
															POR XMM0, XMM6			// �ϲ�ARGB
															MOVD [EDX], XMM0

															ADD EDX, 4
															JMP LOOP2BODY3
LOOP2END:
														ADD ESI, nAddDestWidth
															ADD EDI, nAddSrcWidth

															POP ECX
															INC	ECX			 // ����������������ѭ��1 
															JMP LOOP1
EXITLOOP1:
														EMMS			// ���
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
// ���ܣ���������ĳһ����
// ������pdwBuffer:Ŀ�껺����,�����16�ֽڶ��룬����Ч�������һ��
//		 int nPitch	����,Byte
//		 nX:���x,��λPixel
//		 nY:���y,��λPixel
//		 nH:���,��λPixel
//		 nW:����,��λPixel
//		 dwValue:ֵ
// ����ֵ��������ָ��
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
		// �������16�ֽڶ���
		// һ���ĸ�����ֵ
		__asm{
			XOR ECX,ECX	// ��0
				MOV EDX, pdwTemp    // EDX�洢������ָ��
				MOVD   XMM0, dwValue
				PSHUFD XMM0, XMM0, 0 // �õ�dwValueֵ����XMM0�ķ�
LOOPX:
			ADD ECX, 4		// �ȼ�4���ж�dwSize�Ƿ�4����
				CMP ECX, nW
				JG LOOPX1		// ���>=,������LOOPX1

				MOVDQU [EDX], XMM0	// ���ĸ��㸳ֵ

				// ָ�����16���ֽ�
				ADD EDX, 10H
				JMP LOOPX
LOOPX1:
			// ������4���ֽ�ʱ:ֻ��һ�θ�һ��
			SUB	ECX, 4	// ���ص�ԭ������
LOOPX2:
			CMP ECX, nW
				JGE LOOPX3

				MOV EAX, dwValue
				INC ECX
				MOV [EDX], EAX
				ADD EDX,4             // pdwTempƫ���ĸ��ֽ�
				JMP LOOPX2
LOOPX3:		
			CMP EDX,pdwTempEnd
				JGE	EXITLOOPX

				SUB EDX, nBW			//��ַ�ص�����
				ADD EDX, nPitch			//��ַ����һ��
				XOR ECX,ECX	//			��0
				JMP LOOPX
EXITLOOPX:
		}
	}
	else
	{
		// һ���ĸ�����ֵ
		__asm{
			XOR ECX,ECX	// ��0
				MOV EDX, pdwTemp    // EDX�洢������ָ��
				MOVD   XMM0, dwValue
				PSHUFD XMM0, XMM0, 0 // �õ�dwValueֵ����XMM0�ķ�
LOOPY:
			ADD ECX, 4		// �ȼ�4���ж�dwSize�Ƿ�4����
				CMP ECX, nW
				JG LOOPY1		// ���>=,������LOOPX1

				MOVNTDQ [EDX], XMM0	// ���ĸ��㸳ֵ(WCģʽ��ֱ��д�����������棩

				// ָ�����16���ֽ�
				ADD EDX, 10H
				JMP LOOPY
LOOPY1:
			// ������4���ֽ�ʱ:ֻ��һ�θ�һ��
			SUB	ECX, 4	// ���ص�ԭ������
LOOPY2:
			CMP ECX, nW
				JGE LOOPY3

				MOV EAX, dwValue
				INC ECX
				MOV [EDX], EAX
				ADD EDX,4             // pdwTempƫ���ĸ��ֽ�

				JMP LOOPY2
LOOPY3:
			CMP EDX,pdwTempEnd
				JGE	EXITLOOPY

				SUB EDX, nBW			//��ַ�ص�����
				ADD EDX, nPitch			//��ַ����һ��
				XOR ECX,ECX	//			��0
				JMP LOOPX
EXITLOOPY:
		}
	}
	return pdwBuffer;
}
//////////////////////////////////////////////////////////////////////////
// ���ܣ��������ص�AlphaΪָ��ֵ
// ������pdwBuffer:Ŀ�껺����
//		 dwValue:ָ����Alphaֵ
//		 nWidth:��
//		 nHeight����
// ����ֵ��������ָ��
//////////////////////////////////////////////////////////////////////////
void* CALLBACK TP_AlphaMemSet(IN OUT DWORD *pdwBuffer, IN BYTE byValue, IN int nWidth, IN int nHeight)
{
	if(pdwBuffer == NULL)
		return NULL;
	DWORD *pdwTemp = pdwBuffer;
	DWORD dwValue = (DWORD)byValue;
	dwValue = dwValue<<24;

	// һ���ĸ�����ֵ
	__asm{
		XOR ECX,ECX	// ��0
			MOV EDX, pdwTemp    // EDX�洢������ָ��
			MOV EAX, 0x00FFFFFF
			MOVD XMM7, EAX
			PSHUFD XMM7, XMM7, 0 // ����XMM7�ķ�
			MOV EAX, dwValue
			MOVD XMM0, EAX
			PSHUFD XMM0, XMM0, 0 // �õ�byValueֵ����XMM0�ķ�
LOOP1:
		CMP ECX, nHeight // ��һ��ѭ��(i<nHeight)
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(i>=nHeight)
			PUSH ECX
			XOR ECX, ECX	 // j��0�� �ڶ���ѭ��������
LOOP2:
		ADD ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����2��
			CMP ECX, nWidth
			JG LOOP2BODY2	

			// ȡ��Dest��Ӧ��Floatֵ
			MOVDQU   XMM1, [EDX]
			PAND  XMM1, XMM7
				POR   XMM1, XMM0
				MOVDQU DWORD PTR [EDX], XMM1

				ADD EDX, 10H
				JMP LOOP2
LOOP2BODY2:
			SUB ECX, 4			// ����4ʱ
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
					INC	ECX			 // ����������������ѭ��1 
					JMP LOOP1
EXITLOOP1:
				EMMS			// ���
	}
	return pdwBuffer;
}
//////////////////////////////////////////////////////////////////////////
// ���ܣ�32λת24λBuffer,��ȥAlpha��
// ������nWidth:��ȣ�����������nHeight���߶ȣ���������
// ����ֵ��
//////////////////////////////////////////////////////////////////////////
void CALLBACK TP_32To24Buffer(void* pDesBuffer, long des, void* pSrcBuffer, long src, int nWidth, int nHeight)
{
	//	long nSum = nWidth*nHeight;	// �����ظ���
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
	//		ADD ECX, 16		// һ��16����
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
	//		PSRLDQ XMM2,  1		// ����1�ֽ�
	//		POR   XMM0, XMM2	// R1G1B1R0G0B0
	//		MOVDQA XMM2, XMM5
	//		PAND XMM2,   XMM1	// R2G2B2
	//		PSRLDQ XMM2, 2		// ����2���ֽ�
	//		PAND XMM0, XMM2		// R2G2B2R1G1B1R0G0B0
	//		MOVDQA XMM2, XMM4
	//		PAND XMM2,   XMM1	// R3G3B3
	//		PSRLDQ XMM2, 3		// ����3���ֽ�
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
// ��δʵ��
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK TP_ColorMixLine(
							  IN		DWORD* pDes,		//Ŀ��
							  IN		DWORD* pSrc,		//Դ
							  IN		int nWidth			//���
							  )
{
	return 1;
}

//////////////////////////////////////////////////////////////////////////
// ����ģ�������
// �㷨��DA = SA + DA - (SA*DA)/255,����SAΪԴ��ģ��DAΪĿ����ģ
//
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_MixMask(IN OUT BYTE *pbyDestMask,	// Ŀ����ģ������
							IN int nDestX,				// Ŀ������������
							IN int nDestY, 
							IN int nDestWidth,			// Ŀ���������
							IN int nDestHeight,
							IN int nDestMaskWidth,		// Ŀ����ģ����������
							IN int nDestMaskHeight,
							IN BYTE *pbySrcMask,		// Դ��ģ������
							IN int nSrcX,				// Դ�����������
							IN int nSrcY,
							IN int nSrcWidth,			// Դ��������
							IN int nSrcHeight,
							IN int nSrcMaskWidth,		// Դ��ģ����������
							IN int nSrcMaskHeight)
{
	// ���ݷ�Χ���
	if(nDestX >= nDestMaskWidth || nDestY >= nDestMaskHeight)
		return TRUE;
	if(nDestMaskWidth <= 0||nDestMaskHeight <= 0 || nSrcMaskHeight<=0||nSrcMaskWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcMaskWidth || nSrcY >= nSrcMaskHeight)
		return TRUE;

	// �õ���������Ҫ�����ĳ���
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	BYTE* pDes = pbyDestMask + nDestY*nDestMaskWidth + nDestX;
	BYTE* pSrc = pbySrcMask + nSrcY*nSrcMaskWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;

#if 1
	// һ���и�ֵ,SSE2�Ż�
	_declspec(align(16)) DWORD dwMask1[]={0x00FF00FF,0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	_declspec(align(16)) DWORD dwMask2[]={0x000000FF,0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX	// i��0
			MOVDQA XMM4, dwMask1
			MOVDQA XMM5, dwMask2
LOOP1:
		CMP ECX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(i>=nDestHeight)
			PUSH ECX
			XOR ECX, ECX	 // j��0�� �ڶ���ѭ��������
			MOV EAX, pSrc	 // Դ��Ŀ�껺����ָ�� 
			MOV EDX, pDes
LOOP2:
		ADD ECX, 8			// ÿ�μ���8��ֵ������8ʱ����ѭ����2��
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			MOVQ MM0, [EDX]	// ȡDest��8��Byteֵ��64λ
			MOVQ2DQ XMM0, MM0
				PXOR XMM7, XMM7
				PUNPCKLBW XMM0, XMM7	// ÿ��Byte���Wordֵ
				MOVDQA XMM3, XMM0
				MOVQ MM1, [EAX]		// ȡSrc��8��Byteֵ
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7 // ÿ��Byte���Wordֵ
					PMULLW XMM0, XMM1	// SA*DA
					PADDUSW XMM0,XMM4	// SA*DA+0xFF������
					PSRLW XMM0, 8		// ����8λ
					PADDUSW XMM3, XMM1	// SA+DA
					PSUBUSW XMM3, XMM0	// SA+DA-(SA*DA+0xFF)>>8
					PACKUSWB XMM3, XMM3	// �����תWORD��BYTEֵ
					MOVDQ2Q MM3, XMM3	// ���������Dest
					MOVNTQ [EDX], MM3

					ADD EAX, 8			// ָ�����
					ADD EDX, 8
					JMP LOOP2
LOOP2BODY2:
				SUB ECX, 8			// ����8ʱ
LOOP2BODY3:
				ADD ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����4��
					CMP ECX, nDestWidth
					JG LOOP2BODY4	

					MOVD XMM0, [EDX]	// ȡDest��4��Byteֵ��32λ
					PXOR XMM7, XMM7
						PUNPCKLBW XMM0, XMM7// ÿ��Byte���Word
						PUNPCKLWD XMM0, XMM7// ÿ��Byte���DWORD
						MOVDQA XMM3, XMM0
						MOVD XMM1, [EAX]	// ȡSrc��4geByteֵ��32λ
						PUNPCKLBW XMM1, XMM7
							PUNPCKLWD XMM1, XMM7// ÿ��Byte���DWORD
							PMULLW XMM0, XMM1	// SA*DA
							PADDD XMM0, XMM5	// SA*DA+0xFF
							PSRLD XMM0, 8		// SA*DA+0xFF����8λ������
							PADDD XMM3, XMM1	// SA+DA
							PSUBD XMM3, XMM0	// SA+DA-(SA*DA+0xFF)>>8
							PACKSSDW XMM3, XMM3 // �����תDWORD��Word
							PACKUSWB XMM3, XMM3 // �����ת��Word��Byte
							MOVD  [EDX], XMM3	// ���������Dest

							ADD EAX, 4
							ADD EDX, 4
							JMP LOOP2BODY3
LOOP2BODY4:
						SUB ECX, 4			// ����4ʱ
LOOP2BODY5:
						ADD ECX, 2			// ÿ�μ���2��ֵ�� ����2ʱ����ѭ����6��
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
									PUNPCKLBW XMM0, XMM7// ÿ��Byte���Word
									PUNPCKLWD XMM0, XMM7// ÿ��Byte���DWORD
									MOVDQA XMM3, XMM0
									PUNPCKLBW XMM1, XMM7// ÿ��Byte���Word
									PUNPCKLWD XMM1, XMM7// ÿ��Byte���DWORD
									PMULLW XMM0, XMM1	// SA*DA
									PADDD XMM0, XMM5	// SA*DA+0xFF
									PSRLD XMM0, 8		// SA*DA+0xFF����8λ������
									PADDD XMM3, XMM1	// SA+DA
									PSUBD XMM3, XMM0	// SA+DA-(SA*DA+0xFF)>>8
									PACKSSDW XMM3, XMM3 // �����תDWORD��Word
									PACKUSWB XMM3, XMM3 // �����ת��Word��Byte

									PUSH EAX
									MOVD EAX, XMM3
									MOV  [EDX], AX		// ���������Dest
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
									MOV CL, [EAX]	// SA����ECX
									XOR EAX, EAX
										MOV AL, [EDX]	// DA����EAX
										MOV EDX, ECX	// ����SA
											ADD ECX, EAX	// SA+DA
											MUL EDX			// SA*DA
											ADD EAX, 0x000000FF	// SA*DA+0xFF
											SHR EAX, 8		// ����8λ
											SUB ECX, EAX	// �����
											POP EDX

											MOV BYTE PTR [EDX], CL
LOOP2END:
										MOV EAX, nDestMaskWidth
											ADD [pDes], EAX

											MOV EAX, nSrcMaskWidth
											ADD [pSrc], EAX

											POP ECX
											INC	ECX			 // ����������������ѭ��1 
											JMP LOOP1
EXITLOOP1:
										EMMS			// ���
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

// ��ȡ������ֵ(�߾���):ʱ������
// ��ȡ��ǰ������ʱ�����ڣ����뼶��/��ƵHZ��
unsigned _int64 TP_GetCycleCount()
{
	__asm _emit 0x0F		// RDTSCָ��
		__asm _emit 0x31
}


//////////////////////////////////////////////////////////////////////////
// ˽�к���
//////////////////////////////////////////////////////////////////////////
// �����쿽����
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
			MOVSS  XMM1, fVal			// ��ʼֵ
				SHUFPS XMM1, XMM1, 0
LOOPX:
			ADD ECX, 4
				CMP ECX, nWidth
				JG  LOOPX2

				ADDPS  XMM1, XMM0         // ��Ӧ4�������
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
// �ͷű�ռ�
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
// ���ܣ�����Mix��仺��������ʱnSrcWidth==nDestWidth, nSrcHeight==nDestHeight
//
// 1��2003-10-10,V1.0,��д�ߣ������
//	  ʹ���㷨��DR = SR*SA/255+(1-SA/255)*DR
// 2��2003-11-19,V2.0,�޸��ߣ������
//	  �Ľ��㷨��KA=SA+DA-SA*DA/255; DR = DR+(SR-DR)*SA/KA
// 3��2004��02��11��V2.1���޸��ߣ������
//    �Ľ��������ż����Ϣ�Ĵ���
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
	// ���ݷ�Χ���
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// �õ���������Ҫ�����ĳ���
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;

	//////////////////////////////////////////////////////////////////////////
	// ��������ż����Ϣ��04��02��11��chyf
	// ÿ�μӵĿ��
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// ������������
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

	if(nDestY % 2 == 0)	// ���Ϊ�泡
	{
		if(nParity == TP_PARITY_EVEN)
		{
			// ���ż�������������һ�п�ʼ
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
		// ���Ϊż��
		if(nParity == TP_PARITY_ODD)
		{
			// ����泡�����������һ�п�ʼ
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pSrc += nSrcBufferWidth;
		}
	}
	// ��������ż����Ϣ
	//////////////////////////////////////////////////////////////////////////

#if 0	
	// ����
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};


	// һ���и�ֵ,SSE2�Ż�
	_asm
	{
		XOR EAX, EAX	// ��0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(i>=nDestHeight)
			MOV	ECX, nDestY	 // ����nDestY+i
			ADD ECX, EAX
			CMP ECX, nDestBufferHeight
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(nDestY+i>=nDestBufferHeight)
			MOV	ECX, nSrcY	 // ����nSrcY+i
			ADD ECX, EAX
			CMP ECX, nSrcBufferHeight
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(nSrcY+i>=nSrcBufferHeight)
			XOR ECX, ECX	 // ��0�� �ڶ���ѭ��������
			PUSH EAX
LOOP2:
		CMP ECX, nDestWidth  // �ڶ���ѭ��(j<nDestWidth)
			JGE LOOP2END	 // ����ڶ���ѭ����(j<nDestWidth)

			ADD ECX, 2			// ÿ�μ���2��ֵ������2ʱ����ѭ����2��
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2

			MOV EDX, nSrcX	// ����nSrcX+j
			ADD EDX, ECX
			CMP EDX, nSrcBufferWidth
			JGE LOOP2END	// �˳��ڶ���ѭ��(nSrcX+j>=nSrcBufferWidth)

			MOV EDX, nDestX	// ����nDestX+j
			ADD EDX, ECX
			CMP EDX, nDestBufferWidth
			JGE LOOP2END	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)	

			MOV EAX, pSrc	// ��ȡpSrc[j]��ֵ����MM0
			MOV EDX, ECX
			SHL EDX, 2
			ADD EAX, EDX 
			MOVD MM0, [EAX]

			PUSH ECX
				MOV ECX, [EAX]					// ��ȡ��Alphaֵ
				SHR	ECX,  24					// ����24λ
					AND ECX, 0x000000FF
					MOVD MM3, ECX
					PUNPCKLWD MM3, MM3
					PUNPCKLDQ MM3, MM3				// ��Ϊ0A0A0A0A

					MOV EDX, 0xFF					// 255-Alpha
					SUB EDX, ECX
					MOVD MM7, EDX
					PUNPCKLWD MM7, MM7
					PUNPCKLDQ MM7, MM7				// ��Ϊ0A0A0A0A
					POP ECX

					ADD EAX, 4		// ��ȡpSrc[j+1]��ֵ����MM1
					MOVD MM1, [EAX]

					PUSH ECX
						MOV ECX, [EAX]					// ��ȡ��Alphaֵ
						SHR	ECX,  24					// ����24λ
							AND ECX, 0x000000FF
							MOVD MM4, ECX
							PUNPCKLWD MM4, MM4
							PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

							MOV EAX, 0xFF					// 255-Alpha
							SUB EAX, ECX
							MOVD MM6, EAX
							PUNPCKLWD MM6, MM6
							PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A
							POP ECX

							XOR EAX, EAX	// ��0
							MOVD MM2, EAX
							PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
							PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
							MOVQ2DQ XMM0, MM0
							MOVQ2DQ XMM1, MM1
							PUNPCKLQDQ XMM0, XMM1 // ��Ϊ0R0G0B0A0R0G0B0A

							MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // ����ָ�룬����Ҫ��
							MOVD MM0, [EAX]
							ADD EAX, 4		// ��ȡpDes[j+1]��ֵ
								MOVD MM1, [EAX]
								XOR EAX, EAX	// ��0
									MOVD MM2, EAX
									PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
									PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
									MOVQ2DQ XMM1, MM0
									MOVQ2DQ XMM2, MM1
									PUNPCKLQDQ XMM1, XMM2 // ��Ϊ0R0G0B0A0R0G0B0A
									MOVDQA	XMM3, XMM1	 // ����һ��

									MOVQ2DQ XMM2, MM3		// ȡԴ��alphaֵ
									MOVQ2DQ XMM4, MM4
									PUNPCKLQDQ XMM2, XMM4	// ��Ϊ0A0A0A0A0M0M0M0M

									MOVQ2DQ XMM4, MM7		// ȡ255-alphaֵ
									MOVQ2DQ XMM5, MM6
									PUNPCKLQDQ XMM4, XMM5	// ��Ϊ0A0A0A0A0M0M0M0M

									PMULLW XMM0, XMM2	// ��Ӧ16λ���SA
									// ����
									MOVQ2DQ		XMM7,MM5
									MOVLHPS		XMM7,XMM7
									PADDSW		XMM0,XMM7
									PSRLW  XMM0, 8		// ÿ16λ����8λ���൱��/255)(Ҫ��������)

									PMULLW XMM1, XMM4	// ��Ӧ16λ���D(1-A)
									// ����
									PADDSW XMM1,XMM7
									PSRLW  XMM1, 8

									PADDSW XMM0, XMM1	// ��Ӧ16λ���(SA+DA)

									PACKUSWB XMM0, XMM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

									MOVDQ2Q	MM0,XMM0	// ȡ��64λ(����DWORD)
									POP EAX					// ����pDest
									MOVQ  [EAX], MM0

									ADD ECX, 2			// j��2�����صڶ���ѭ��
									JMP LOOP2
LOOP2BODY2:
								SUB ECX, 2			// ����2ʱ
									CMP ECX, nDestWidth
									JGE LOOP2END

									MOV EDX, nSrcX	// ����nSrcX+j
									ADD EDX, ECX
									CMP EDX, nSrcBufferWidth
									JGE LOOP2END	// �˳��ڶ���ѭ��(nSrcX+j>=nSrcBufferWidth)

									MOV EDX, nDestX	// ����nDestX+j
									ADD EDX, ECX
									CMP EDX, nDestBufferWidth
									JGE LOOP2END	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)		

									MOV EAX, pSrc		// ��ȡpSrc[j]��ֵ
									MOV EDX, ECX
									SHL EDX, 2
									ADD EAX, EDX 
									MOVD MM0, [EAX]

									PUSH ECX
										MOV ECX, [EAX]					// ��ȡ��Alphaֵ
										SHR	ECX,  24					// ����24λ
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A
											POP ECX

											XOR EAX, EAX	// ��0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A

											MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
											MOV EDX, ECX
											SHL EDX, 2
											ADD EAX, EDX
											PUSH  EAX		 // ����ָ�룬����Ҫ��
											MOVD MM1, [EAX]
											XOR EAX, EAX	// ��0
												MOVD MM2, EAX
												PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
												MOVQ	MM3, MM1	 // ����һ��

												PMULLW MM0, MM4	// ��Ӧ16λ���SA
												// ����
												PADDSW MM0, MM5
												PSRLW  MM0, 8		// ����8λ���൱��/255)(Ҫ��������)

												PMULLW MM1, MM6	// ��Ӧ16λ���D(1-A)
												// ����
												PADDSW MM1, MM5
												PSRLW  MM1, 8
												PADDSW MM0, MM1	// ��Ӧ16λ���
												PACKUSWB MM0, MM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

												POP EAX					// ����pDest
												MOVD  [EAX], MM0
LOOP2END:
											MOV EAX, pDes
												ADD EAX, nAddDestWidth		// nDestBufferWidth���4
												MOV [pDes], EAX

												MOV EAX, pSrc
												ADD EAX, nAddSrcWidth
												MOV [pSrc], EAX

												POP EAX
												INC	EAX			 // ����������������ѭ��1 
												JMP LOOP1
EXITLOOP1:
											EMMS			// ���
	}
#endif
	//////////////////////////////////////////////////////////////////////////
	// �Ľ��㷨��2003��11��12��chyf
	//////////////////////////////////////////////////////////////////////////
#if 0
	_declspec(align(16)) DWORD nMask[4] = {0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF};
	_declspec(align(16)) DWORD nMask2[4] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	// ����
	_declspec(align(16)) DWORD *pTable = (DWORD*)_mm_malloc(256*sizeof(DWORD), 16);
	_declspec(align(16)) float fTemp[4] = {256.0f, 256.0f, 256.0f, 256.0f};
	if(pTable == NULL)
		return FALSE;
	pTable[0] = 0;
	for(int i=1; i<256; i++)
	{
		pTable[i] = (DWORD)(65535/i);
	}

	// һ���и�ֵ,SSE2�Ż�

	_asm
	{
		XOR ECX, ECX	// i��0
LOOPI:
		CMP ECX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOPI	 // �˳���һ��ѭ��(i>=nDestHeight)

			PUSH ECX		// ѹ��i
			MOV  EAX, pSrc
			MOV  EDX, pDes
			XOR  ECX, ECX	 // j��0�� �ڶ���ѭ��������
LOOPJ:
		// �ڶ���ѭ���壬һ�ζ�4���ڵ�
		ADD ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����4��
			CMP ECX, nDestWidth
			JG LOOPJ4		// (j>nDestWidth)

			// ȡ��Src��Dest4�������XMM0��XMM1
			MOVUPS XMM0, [EAX]	// ȡ��SRC�ĸ����ARGB
			MOVUPS XMM1, [EDX]	// ȡ��Dest�ĸ����ARGB

			MOVAPS XMM6, XMM0	// �洢Src����
				MOVAPS XMM7, XMM1	// �洢Dest������

				// ȡ��Aֵ�����000A000A000A000A
				PSRAD  XMM0, 24		// ����24λ��ȡAֵ
				PSRAD  XMM1, 24		// ����24λ��ȡAֵ
				PAND   XMM0, nMask		// �ĸ�000A
				PAND   XMM1, nMask		// �ĸ�000A

				// ����SA�ߺ͵�����DWORD
				MOVDQ2Q MM4, XMM0       // ����SA������000A000A	
				MOVAPS XMM3, XMM0		// ����SA
				MOVHLPS XMM3, XMM3
				MOVDQ2Q MM5, XMM3       // ����SA������000A000A

				// Src��Dest���Ϊ000A000A000A000A���,����XMM3
				MOVAPS XMM3, XMM0		// ����SA
				PADDW  XMM3, XMM1		// Src+Dest(16λ��)

				// ��Ӧ16λ��ˣ�����256��Src*Dest>>8)
				PMULLW XMM0, XMM1
				PADDW  XMM0, nMask		// ����
				PSRAD  XMM0, 8			
				//PAND   XMM0, nMask	

				// ��Ӧ16λ��,�õ����յ�CAֵ000A000A000A000A
				PSUBW  XMM3, XMM0	
				PAND   XMM3, nMask

				// ����ָ��
				PUSH EAX
				PUSH EDX

				// ÿ���������㣨SR*SA*CR>>8��DR-DR*SA*CR>>8��
				// ��CAֵ��Ӧ��������8������0A0A0A0A0A0A0A0A)
				XOR    EDX, EDX
				PEXTRW  EDX, XMM3, 0	// �����һ��Aֵ
				MOV    EAX, pTable
				SHL    EDX, 2
				ADD	   EDX, EAX    
				MOVD   MM0, [EDX]
				PUNPCKLWD MM0, MM0  
					PUNPCKLDQ MM0, MM0  // ���0A0A0A0A
					XOR    EDX, EDX
					PEXTRW  EDX, XMM3, 2	// ����ڶ���Aֵ
					SHL    EDX, 2
					ADD	   EDX, EAX    
					MOVD   MM1, [EDX]
					PUNPCKLWD MM1, MM1 
						PUNPCKLDQ MM1, MM1  // ���0A0A0A0A
						MOVQ2DQ  XMM0, MM0
						MOVQ2DQ  XMM1, MM1
						PUNPCKLQDQ XMM0, XMM1 // ���0A0A0A0A0A0A0A0A

						// ȡ���������RGBAֵ
						MOVDQ2Q  MM0, XMM6	  // SRC����
						MOVDQ2Q  MM1, XMM7    // Dest����
						MOVQ2DQ     XMM2, MM0
						PXOR     XMM1, XMM1
						PUNPCKLBW XMM2, XMM1	// Src���0A0R0G0B0A0R0G0B,����XMM2
						MOVQ2DQ     XMM4, MM1
						PUNPCKLBW XMM4, XMM1	// Dest���0A0R0G0B0A0R0G0B������XMM4

						// SRC*SA
						// ȡ������SA�����
						MOVQ MM3, MM4
						PUNPCKHWD MM3, MM3
						PUNPCKLDQ MM3, MM3
						PUNPCKLWD MM4, MM4
						PUNPCKLDQ MM4, MM4
						MOVQ2DQ XMM1, MM4
						MOVQ2DQ XMM5, MM3
						PUNPCKLQDQ XMM1, XMM5		// ȡ��SAֵ���0A0A0A0A0A0A0A0A
						PMULLW   XMM2, XMM1
						//PAND     XMM2, nMask2

						// SR*SA*CR
						PMULLW   XMM2, XMM0		// ��Ӧ16λ��
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
						PMULLW   XMM4, XMM0		// ��Ӧ16λ��
						//PAND     XMM4, nMask2

						// DR*SA*CR>>8
						PSRAW  XMM4, 8	
						//	PAND   XMM4, nMask2		

						// DR+SRC*SA*CR>>8-DEST*SA*CR>>8
						PSUBW  XMM2, XMM4      // ��Ӧ16λ��
						PAND   XMM2, nMask2

						// ���뵱ǰ��ȷ��Aֵ
						XOR    EDX, EDX
						PEXTRW  EDX, XMM3, 0	// �����һ��Aֵ 
						PINSRW XMM2, EDX, 3
						PEXTRW  EDX, XMM3, 2	// ����ڶ���Aֵ 
						PINSRW XMM2, EDX, 7 // ���������0R0G0B0Aֵ

						// �������������㣨SR*SA*CR>>8��DR��DR*SA*CR>>8��
						// ��CAֵ��Ӧ��������8������0A0A0A0A0A0A0A0A)
						XOR    EDX, EDX
						PEXTRW  EDX, XMM3, 4	// ���������Aֵ
						SHL    EDX, 2
						MOV    EAX, pTable
						ADD	   EDX, EAX    
						MOVD   MM0, [EDX]
						PUNPCKLWD MM0, MM0  
							PUNPCKLDQ MM0, MM0  // ���0A0A0A0A
							XOR    EDX, EDX
							PEXTRW  EDX, XMM3, 6	// ������ĸ�Aֵ
							SHL    EDX, 2
							ADD	   EDX, EAX    
							MOVD   MM1, [EDX]
							PUNPCKLWD MM1, MM1 
								PUNPCKLDQ MM1, MM1  // ���0A0A0A0A
								MOVQ2DQ  XMM0, MM0
								MOVQ2DQ  XMM1, MM1
								PUNPCKLQDQ XMM0, XMM1 // ���0A0A0A0A0A0A0A0A

								// ȡ�����������RGBAֵ
								MOVHLPS  XMM6, XMM6
								MOVDQ2Q  MM0, XMM6	  // SRC������
								MOVHLPS  XMM7, XMM7
								MOVDQ2Q  MM1, XMM7    // Dest������
								MOVQ2DQ     XMM5, MM0
								PXOR     XMM1, XMM1
								PUNPCKLBW XMM5, XMM1	// Src���0A0R0G0B0A0R0G0B
								MOVQ2DQ     XMM4, MM1
								PUNPCKLBW XMM4, XMM1	// Dest���0A0R0G0B0A0R0G0B

								// (SRC)*SA
								MOVQ MM3, MM5
								PUNPCKHWD MM3, MM3
								PUNPCKLDQ MM3, MM3
								PUNPCKLWD MM5, MM5
								PUNPCKLDQ MM5, MM5
								MOVQ2DQ XMM1, MM5
								MOVQ2DQ XMM6, MM3
								PUNPCKLQDQ XMM1, XMM6		// ȡ��SAֵ���0A0A0A0A0A0A0A0A
								PMULLW   XMM5, XMM1
								//PAND     XMM5, nMask2

								// (SR)*SA*CR
								PMULLW   XMM5, XMM0		// ��Ӧ16λ��
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
								PMULLW   XMM4, XMM0		// ��Ӧ16λ��
								//PAND     XMM4, nMask2

								// (DR)*SA*CR>>8
								PSRAW  XMM4, 8	
								//PAND   XMM4, nMask2	

								PSUBW  XMM5, XMM4      // ��Ӧ16λ��
								PAND   XMM5, nMask2

								// ���뵱ǰ��ȷ��Aֵ
								XOR     EDX, EDX
								PEXTRW  EDX, XMM3, 4	// �����һ��Aֵ 
								PINSRW XMM5, EDX, 3
								PEXTRW  EDX, XMM3, 6	// ����ڶ���Aֵ 
								PINSRW XMM5, EDX, 7 // ���������0R0G0B0Aֵ

								// ����ѹ��Ϊ�ĸ�Ŀ��RGBAֵ
								PACKUSWB XMM2, XMM5

								// ����ָ��
								POP EDX
								POP EAX
								MOVUPS  [EDX], XMM2	// ����Dest�ĸ����ARGB
								ADD EDX, 10H
								ADD EAX, 10H
								JMP LOOPJ
LOOPJ4:
							SUB ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����4��
LOOPJ41:
							CMP ECX, nDestWidth
								JGE LOOPJEND

								// ȡ��Src��Dest1����
								MOVD MM0, [EAX]	// ȡ��SRC1�����ARGB
								MOVD MM1, [EDX]	// ȡ��Dest1�����ARGB

								// ����Src��Dest
								MOVQ MM3, MM0
									MOVQ MM4, MM1

									// ȡ��SA��DA
									PSRAD MM0, 24	// �õ�000Aֵ
									PAND  MM0, nMask
									PSRAD MM1, 24
									PAND MM1, nMask

									// ����SA
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

									// ����ָ��
									PUSH EAX
									PUSH EDX

									// ����CA��Ӧ������
									MOVD   EDX, MM0			// CA
									SHL    EDX, 2
									MOV    EAX, pTable
									ADD	   EDX, EAX    
									MOVD   EAX, MM0		// ����CA
									MOVD   MM0, [EDX]
									PUNPCKLWD MM0, MM0  
										PUNPCKLDQ MM0, MM0  // ���0A0A0A0A

										// ���SR��DR
										PXOR  MM1, MM1
										PUNPCKLBW MM3, MM1	
										PUNPCKLBW MM4, MM1

										// ���SA
										PUNPCKLWD MM7, MM7  
										PUNPCKLDQ MM7, MM7  // ���0A0A0A0A

										// (SRC)*SA
										PMULLW   MM3, MM7		// ��Ӧ16λ��
										// (SR)*SA*CR
										PMULLW   MM3, MM0
										PSRAW  MM3, 8	
										//PAND   MM3, nMask2
										// (SR)*SA*CR>>8+DR
										PADDW  MM3,  MM4

										// (Dest)*SA
										PMULLW   MM4, MM7		// ��Ӧ16λ��
										// (DR)*SA*CR
										PMULLW   MM4, MM0
										PSRAW  MM4, 8	
										//PAND   MM4, nMask2	
										// SR-DR
										PSUBW  MM3, MM4      // ��Ӧ16λ��
										PAND   MM3, nMask2

										PINSRW MM3, EAX, 3 // �����0R0G0B0Aֵ
										// ѹ�������յ�ARGB
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
										ADD EAX, nAddDestWidth		// nDestBufferWidth���4
										MOV [pDes], EAX

										MOV EAX, pSrc
										ADD EAX, nAddSrcWidth
										MOV [pSrc], EAX 
										POP ECX
										INC ECX
										JMP LOOPI
EXITLOOPI:
									EMMS			// ���
	}
	_mm_free(pTable);
#endif
	//////////////////////////////////////////////////////////////////////////
	// �Ľ��㷨��2003��11��19��chyf
	// D = D+(S-D)*SA/KA
	// A = SA+DA-(SA*DA+0xFF)>>8
	//////////////////////////////////////////////////////////////////////////
#if 1
	_declspec(align(16)) DWORD nMask[4] = {0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX			// i��0
LOOPI:
		CMP ECX, nDestHeight	// ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOPI			// �˳���һ��ѭ��(i>=nDestHeight)

			PUSH ECX				// ѹ��i
			MOV  EAX, pSrc			// ����Դ��Ŀ�껺����ָ��
			MOV  EDX, pDes
			XOR  ECX, ECX			// j��0�� �ڶ���ѭ��������
LOOPJ:
		// �ڶ���ѭ���壬һ�ζ�4���ڵ�
		ADD ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����4��
			CMP ECX, nDestWidth
			JG LOOPJ4		// (j>nDestWidth)

			// ȡ��Src��Dest4�������XMM0��XMM1
			MOVUPS XMM0, [EAX]	// ȡ��SRC�ĸ����ARGB
			MOVUPS XMM1, [EDX]	// ȡ��Dest�ĸ����ARGB

			// ȡ��Aֵ�����000A000A000A000A
			PSRAD  XMM0, 24		// ����24λ��ȡAֵ
				PSRAD  XMM1, 24		// ����24λ��ȡAֵ
				PAND   XMM0, nMask		// �ĸ�000A
				PAND   XMM1, nMask		// �ĸ�000A

				// Src��Dest���Ϊ000A000A000A000A���,����XMM3
				MOVAPS XMM3, XMM0		// ����SA
				PADDW  XMM3, XMM1		// Src+Dest(16λ��)

				// ��Ӧ16λ��ˣ�����256��Src*Dest>>8)
				PMULLW XMM0, XMM1
				PADDW  XMM0, nMask		// ����
				PSRAD  XMM0, 8			
				//PAND   XMM0, nMask	

				// ��Ӧ16λ��,�õ����յ�KAֵ000A000A000A000A
				PSUBW  XMM3, XMM0	
				PAND   XMM3, nMask
				//////////////////////////////////////////////////////////////////////////
				// �����һ����
				// ÿ����һ���㣨D+(S-D)*SA/KA��
				// ��ȡ��SRC��RGBA,�����000A000R000G000B����XMM0
				MOVD    MM0, [EAX]
				MOVQ    MM3, MM0		// ����Src��RGBA��������
					PXOR    MM1, MM1
					PUNPCKLBW MM0, MM1
					MOVQ2DQ    XMM0, MM0
					PXOR    XMM2, XMM2
					PUNPCKLWD XMM0, XMM2
					// ת��DWORD��Float,����XMM1
					CVTDQ2PS  XMM1, XMM0
					// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
					MOVD    MM0, [EDX]
					PUNPCKLBW MM0, MM1
						MOVQ2DQ    XMM0, MM0
						PUNPCKLWD XMM0, XMM2
						// ת��DWORD��Float,����XMM2
						CVTDQ2PS  XMM2, XMM0

						// ����ָ��
						PUSH EDX
						// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
						SUBPS  XMM1, XMM2		// S-D
						// ȡ��SA
						PUSH EAX
						MOVD   EAX,  MM3
						SHR    EAX,  16
						AND    EAX,  0x0000FF00  // ���A000
						// ȡ��KA
						XOR    EDX, EDX
						PEXTRW  EDX, XMM3, 0	// �����һ��KAֵ
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
							// ת��������(000A000R000G000B)
							CVTPS2DQ XMM2, XMM1
							// ������ȷ��Aֵ
							PINSRW XMM2, EDX, 6 
							POP EDX
							// ѹ��RGBA
							PACKUSWB XMM2, XMM2
							MOVDQ2Q   MM2, XMM2
							PACKUSWB MM2, MM2
							MOVD  [EDX], MM2	// ����Destһ�����ARGB
							ADD EDX, 4
							//////////////////////////////////////////////////////////////////////////
							//////////////////////////////////////////////////////////////////////////
							// ����ڶ�����
							// ÿ����һ���㣨D+(S-D)*SA/KA��
							// ��ȡ��SRC��RGBA,�����000A000R000G000B����XMM0
							MOVD    MM0, [EAX]
							MOVQ    MM3, MM0		// ����Src��RGBA��������
								PXOR    MM1, MM1
								PUNPCKLBW MM0, MM1
								MOVQ2DQ    XMM0, MM0
								PXOR    XMM2, XMM2
								PUNPCKLWD XMM0, XMM2
								// ת��DWORD��Float,����XMM1
								CVTDQ2PS  XMM1, XMM0
								// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
								MOVD    MM0, [EDX]
								PUNPCKLBW MM0, MM1
									MOVQ2DQ    XMM0, MM0
									PUNPCKLWD XMM0, XMM2
									// ת��DWORD��Float,����XMM2
									CVTDQ2PS  XMM2, XMM0

									// ����ָ��
									PUSH EDX
									// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
									SUBPS  XMM1, XMM2		// S-D
									// ȡ��SA
									PUSH EAX
									MOVD   EAX,  MM3
									SHR    EAX,  16
									AND    EAX,  0x0000FF00  // ���A000
									// ȡ��KA
									XOR    EDX, EDX
									PEXTRW  EDX, XMM3, 2	// ����ڶ���KAֵ
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
										// ת��������(000A000R000G000B)
										CVTPS2DQ XMM2, XMM1
										// ������ȷ��Aֵ
										PINSRW XMM2, EDX, 6 
										POP EDX
										// ѹ��RGBA
										PACKUSWB XMM2, XMM2
										MOVDQ2Q   MM2, XMM2
										PACKUSWB MM2, MM2
										MOVD  [EDX], MM2	// ����Destһ�����ARGB
										ADD EDX, 4
										//////////////////////////////////////////////////////////////////////////
										//////////////////////////////////////////////////////////////////////////
										// �����������
										// ÿ����һ���㣨D+(S-D)*SA/KA��
										// ��ȡ��SRC��RGBA,�����000A000R000G000B����XMM0
										MOVD    MM0, [EAX]
										MOVQ    MM3, MM0		// ����Src��RGBA��������
											PXOR    MM1, MM1
											PUNPCKLBW MM0, MM1
											MOVQ2DQ    XMM0, MM0
											PXOR    XMM2, XMM2
											PUNPCKLWD XMM0, XMM2
											// ת��DWORD��Float,����XMM1
											CVTDQ2PS  XMM1, XMM0
											// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
											MOVD    MM0, [EDX]
											PUNPCKLBW MM0, MM1
												MOVQ2DQ    XMM0, MM0
												PUNPCKLWD XMM0, XMM2
												// ת��DWORD��Float,����XMM2
												CVTDQ2PS  XMM2, XMM0

												// ����ָ��
												PUSH EDX
												// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
												SUBPS  XMM1, XMM2		// S-D
												// ȡ��SA
												PUSH EAX
												MOVD   EAX,  MM3
												SHR    EAX,  16
												AND    EAX,  0x0000FF00  // ���A000
												// ȡ��KA
												XOR    EDX, EDX
												PEXTRW  EDX, XMM3, 4	// ���������KAֵ
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
													// ת��������(000A000R000G000B)
													CVTPS2DQ XMM2, XMM1
													// ������ȷ��Aֵ
													PINSRW XMM2, EDX, 6 
													POP EDX
													// ѹ��RGBA
													PACKUSWB XMM2, XMM2
													MOVDQ2Q   MM2, XMM2
													PACKUSWB MM2, MM2
													MOVD  [EDX], MM2	// ����Destһ�����ARGB
													ADD EDX, 4
													//////////////////////////////////////////////////////////////////////////
													//////////////////////////////////////////////////////////////////////////
													// ������ĸ���
													// ÿ����һ���㣨D+(S-D)*SA/KA��
													// ��ȡ��SRC��RGBA,�����000A000R000G000B����XMM0
													MOVD    MM0, [EAX]
													MOVQ    MM3, MM0		// ����Src��RGBA��������
														PXOR    MM1, MM1
														PUNPCKLBW MM0, MM1
														MOVQ2DQ    XMM0, MM0
														PXOR    XMM2, XMM2
														PUNPCKLWD XMM0, XMM2
														// ת��DWORD��Float,����XMM1
														CVTDQ2PS  XMM1, XMM0
														// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
														MOVD    MM0, [EDX]
														PUNPCKLBW MM0, MM1
															MOVQ2DQ    XMM0, MM0
															PUNPCKLWD XMM0, XMM2
															// ת��DWORD��Float,����XMM2
															CVTDQ2PS  XMM2, XMM0

															// ����ָ��
															PUSH EDX
															// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
															SUBPS  XMM1, XMM2		// S-D
															PUSH EAX
															// ȡ��SA
															MOVD   EAX,  MM3
															SHR    EAX,  16
															AND    EAX,  0x0000FF00  // ���A000
															// ȡ��KA
															XOR    EDX, EDX
															PEXTRW  EDX, XMM3, 6	// �����һ��KAֵ
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
																// ת��������(000A000R000G000B)
																CVTPS2DQ XMM2, XMM1
																// ������ȷ��Aֵ
																PINSRW XMM2, EDX, 6 
																POP EDX
																// ѹ��RGBA
																PACKUSWB XMM2, XMM2
																MOVDQ2Q   MM2, XMM2
																PACKUSWB MM2, MM2
																MOVD  [EDX], MM2	// ����Destһ�����ARGB
																ADD EDX, 4
																//////////////////////////////////////////////////////////////////////////
																JMP LOOPJ
LOOPJ4:
															SUB ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����4��
LOOPJ41:
															CMP ECX, nDestWidth
																JGE LOOPJEND

																// ����Ŀ��A��ֵ
																// ȡ��Src��Dest1����
																MOVD MM0, [EAX]	// ȡ��SRC1�����ARGB
																MOVD MM1, [EDX]	// ȡ��Dest1�����ARGB

																// ����Src��Dest
																MOVQ MM3, MM0
																	MOVQ MM4, MM1

																	// ȡ��SA��DA
																	PSRAD MM0, 24	// �õ�000Aֵ
																	PAND  MM0, nMask
																	PSRAD MM1, 24
																	PAND MM1, nMask

																	// ����SA
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
																	// ���Ŀ��RGBA
																	// ���Src��Dest��RGBAֵ
																	PXOR    MM1, MM1
																	PUNPCKLBW MM3, MM1
																	MOVQ2DQ    XMM0, MM3
																	PXOR    XMM1, XMM1
																	PUNPCKLWD XMM0, XMM1
																	// ת��DWORD��Float,����XMM1
																	CVTDQ2PS  XMM1, XMM0
																	// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
																	PUNPCKLBW MM4, MM1
																	MOVQ2DQ    XMM0, MM4
																	PXOR    XMM2, XMM2
																	PUNPCKLWD XMM0, XMM2
																	// ת��DWORD��Float,����XMM2
																	CVTDQ2PS  XMM2, XMM0

																	// ����ָ��
																	PUSH EDX
																	PUSH EAX
																	PUSH ECX
																	// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
																	SUBPS  XMM1, XMM2		// S-D
																	// ȡ��SA
																	MOVD   EAX,  MM7
																	SHL    EAX,  8
																	AND    EAX,  0x0000FF00  // ���A000
																	// ȡ��KA
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
																		// ת��������(000A000R000G000B)
																		CVTPS2DQ XMM2, XMM1
																		// ������ȷ��Aֵ
																		PINSRW XMM2, EDX, 6 
																		POP EDX
																		// ѹ��RGBA
																		PACKUSWB XMM2, XMM2
																		MOVDQ2Q   MM2, XMM2
																		PACKUSWB MM2, MM2
																		MOVD  [EDX], MM2	// ����Destһ�����ARGB

																		ADD EDX, 4
																		JMP LOOPJ41
LOOPJEND:
																	MOV EAX, pDes
																		ADD EAX, nAddDestWidth		// nDestBufferWidth���4
																		MOV [pDes], EAX

																		MOV EAX, pSrc
																		ADD EAX, nAddSrcWidth
																		MOV [pSrc], EAX 
																		POP ECX
																		ADD ECX, nInc
																		JMP LOOPI
EXITLOOPI:
																	EMMS			// ���
	}
#endif
#if 0
	// ԭ�㷨
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
	// �Ľ��㷨��2003��11��11��chyf
	// Ŀ��Aֵ��SA+DA-SA*DA
	// Ŀ����ɫֵ��(D*DA*(1-SA)+S*SA)/(SA+DA-SA*DA)
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
// ���п�����仺����
BOOL TPBuffer_Copy(DWORD* pdwDestBuffer,		// Ŀ�껺����
				   int nDestX,
				   int nDestY,
				   int nDestWidth,
				   int nDestHeight,
				   int nDestBufferWidth,
				   int nDestBufferHeight,
				   DWORD* pdwSrcBuffer,			// Դ������
				   int nSrcX,
				   int nSrcY,
				   int nSrcWidth,
				   int nSrcHeight,
				   int nSrcBufferWidth,
				   int nSrcBufferHeight,
				   int nParity)					// ��ż��
{
	// ���ݷ�Χ���
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// �õ���������Ҫ�����ĳ���
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;

	// ÿ�μӵĿ��
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// ������������
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

	if(nDestY % 2 == 0)	// ���Ϊ�泡
	{
		if(nParity == TP_PARITY_EVEN)
		{
			// ���ż�������������һ�п�ʼ
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
		// ���Ϊż��
		if(nParity == TP_PARITY_ODD)
		{
			// ����泡�����������һ�п�ʼ
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
	// һ���и�ֵ,SSE2�Ż�
	_asm
	{
		XOR ECX, ECX	// ��0
LOOP1:
		CMP ECX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(i>=nDestHeight)
			PUSH ECX
			XOR ECX, ECX	 // ��0�� �ڶ���ѭ��������
			MOV EAX, pSrc
			MOV EDX, pDes
LOOP2:
		ADD ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����2��
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			// ��ȡ&pSrc[j]����ĸ�ֵ����XMM0
			MOVDQU XMM0, [EAX]			
			ADD EAX, 10H

				// ����&pDes[j]����ĸ���
				MOVDQU  [EDX], XMM0
				ADD EDX, 10H
				JMP LOOP2
LOOP2BODY2:
			SUB ECX, 4			// ����4ʱ
LOOP2BODY3:
			CMP ECX, nDestWidth
				JGE LOOP2END	

				// ��ȡpSrc[j]��ֵ
				MOVD MM0, [EAX]
				ADD EAX, 4
					// ��ȡpDes[j]��ֵ
					MOVD  [EDX], MM0
					ADD EDX, 4
					INC ECX
					JMP LOOP2BODY3
LOOP2END:
				MOV EAX, nAddDestWidth		// nDestBufferWidth���4
					ADD [pDes], EAX

					MOV EAX, nAddSrcWidth
					ADD [pSrc], EAX

					POP ECX
					ADD	ECX, nInc			 // ����������������ѭ��1 
					JMP LOOP1
EXITLOOP1:
				EMMS			// ���
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
// ���ܣ�����NotMix��仺��������ԴMixȡ��
// 
// 2003��10-10,V1.0���޸��ߣ������
// �Ľ��㷨��KR = (255-SA)/255*SR+SA/255*DR
// 2003��11��20,V2.0���޸��ߣ������
// �Ľ��㷨��KA = (255-SA)+DA-(255-SA)*DA/255, KR = DR+(SR-DR)*(255-SA)/KA
// 2004-02-11,V2.1,�޸��ߣ������
// �Ľ�����������ż����Ϣ����
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
	// ���ݷ�Χ���
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// �õ���������Ҫ�����ĳ���
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;

	//////////////////////////////////////////////////////////////////////////
	// ��������ż����Ϣ��2004��02��11��chyf
	// ÿ�μӵĿ��
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// ������������
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

	if(nDestY % 2 == 0)	// ���Ϊ�泡
	{
		if(nParity == TP_PARITY_EVEN)
		{
			// ���ż�������������һ�п�ʼ
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
		// ���Ϊż��
		if(nParity == TP_PARITY_ODD)
		{
			// ����泡�����������һ�п�ʼ
			nDestY++;
			nSrcY++;
			nDestHeight--;
			if(nDestY >= nDestBufferHeight || nSrcY >= nSrcBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;
			pSrc += nSrcBufferWidth;
		}
	}
	// ������ż����Ϣ
	//////////////////////////////////////////////////////////////////////////

#if 0
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};
	// һ���и�ֵ,SSE2�Ż�
	_asm
	{
		XOR EAX, EAX	// ��0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1		 // �˳���һ��ѭ��(i>=nDestHeight)
			MOV	ECX, nDestY		// ����nDestY+i
			ADD ECX, EAX
			CMP ECX, nDestBufferHeight
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(nDestY+i>=nDestBufferHeight)
			MOV	ECX, nSrcY	 // ����nSrcY+i
			ADD ECX, EAX
			CMP ECX, nSrcBufferHeight
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(nSrcY+i>=nSrcBufferHeight)
			XOR ECX, ECX	 // ��0�� �ڶ���ѭ��������
			PUSH EAX
LOOP2:
		CMP ECX, nDestWidth  // �ڶ���ѭ��(j<nDestWidth)
			JGE LOOP2END		 // ����ڶ���ѭ����(j<nDestWidth)

			ADD ECX, 2			 // ÿ�μ���2��ֵ������2ʱ����ѭ����2��
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2

			MOV EDX, nSrcX	// ����nSrcX+j
			ADD EDX, ECX
			CMP EDX, nSrcBufferWidth
			JGE LOOP2BODY2	// �˳��ڶ���ѭ��(nSrcX+j>=nSrcBufferWidth)

			MOV EDX, nDestX	// ����nDestX+j
			ADD EDX, ECX
			CMP EDX, nDestBufferWidth
			JGE LOOP2BODY2	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)	

			MOV EAX, pSrc	// ��ȡpSrc[j]��ֵ����MM0
			MOV EDX, ECX
			SHL EDX, 2
			ADD EAX, EDX 
			MOVD MM0, [EAX]

			PUSH ECX
				MOV ECX, [EAX]					// ��ȡ��Alphaֵ
				SHR	ECX,  24					// ����24λ
					AND ECX, 0x000000FF
					MOVD MM3, ECX
					PUNPCKLWD MM3, MM3
					PUNPCKLDQ MM3, MM3				// ��Ϊ0A0A0A0A

					MOV EDX, 0xFF					// 255-Alpha
					SUB EDX, ECX
					MOVD MM7, EDX
					PUNPCKLWD MM7, MM7
					PUNPCKLDQ MM7, MM7				// ��Ϊ0A0A0A0A
					POP ECX

					ADD EAX, 4		// ��ȡpSrc[j+1]��ֵ����MM1
					MOVD MM1, [EAX]

					PUSH ECX
						MOV ECX, [EAX]					// ��ȡ��Alphaֵ
						SHR	ECX,  24					// ����24λ
							AND ECX, 0x000000FF
							MOVD MM4, ECX
							PUNPCKLWD MM4, MM4
							PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

							MOV EAX, 0xFF					// 255-Alpha
							SUB EAX, ECX
							MOVD MM6, EAX
							PUNPCKLWD MM6, MM6
							PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A
							POP ECX

							XOR EAX, EAX	// ��0
							MOVD MM2, EAX
							PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
							PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
							MOVQ2DQ XMM0, MM0
							MOVQ2DQ XMM1, MM1
							PUNPCKLQDQ XMM0, XMM1 // ��Ϊ0R0G0B0A0R0G0B0A

							MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // ����ָ�룬����Ҫ��
							MOVD MM0, [EAX]
							ADD EAX, 4		// ��ȡpDes[j+1]��ֵ
								MOVD MM1, [EAX]
								XOR EAX, EAX	// ��0
									MOVD MM2, EAX
									PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
									PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
									MOVQ2DQ XMM1, MM0
									MOVQ2DQ XMM2, MM1
									PUNPCKLQDQ XMM1, XMM2 // ��Ϊ0R0G0B0A0R0G0B0A
									MOVDQA	XMM3, XMM1	 // ����һ��

									MOVQ2DQ XMM2, MM3		// ȡԴ��alphaֵ
									MOVQ2DQ XMM4, MM4
									PUNPCKLQDQ XMM2, XMM4	// ��Ϊ0A0A0A0A0M0M0M0M

									MOVQ2DQ XMM4, MM7		// ȡ255-alphaֵ
									MOVQ2DQ XMM5, MM6
									PUNPCKLQDQ XMM4, XMM5	// ��Ϊ0A0A0A0A0M0M0M0M

									PMULLW XMM0, XMM4	// ��Ӧ16λ���S(1-A)
									// ����
									MOVQ2DQ		XMM7,MM5
									MOVLHPS		XMM7,XMM7
									PADDSW		XMM0,XMM7
									PSRLW  XMM0, 8		// ÿ16λ����8λ���൱��/255)(Ҫ��������)

									PMULLW XMM1, XMM2	// ��Ӧ16λ���DA
									// ����
									PADDSW		XMM1,XMM7
									PSRLW  XMM1, 8

									PADDSW XMM0, XMM1	// ��Ӧ16λ���(SA+DA)

									PACKUSWB XMM0, XMM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

									MOVDQ2Q	MM0,XMM0	// ȡ��64λ(����DWORD)
									POP EAX					// ����pDest
									MOVQ  [EAX], MM0

									ADD ECX, 2			// j��2�����صڶ���ѭ��
									JMP LOOP2
LOOP2BODY2:
								SUB ECX, 2			// ����2ʱ
									CMP ECX, nDestWidth
									JGE LOOP2END

									MOV EDX, nSrcX	// ����nSrcX+j
									ADD EDX, ECX
									CMP EDX, nSrcBufferWidth
									JGE LOOP2END	// �˳��ڶ���ѭ��(nSrcX+j>=nSrcBufferWidth)

									MOV EDX, nDestX	// ����nDestX+j
									ADD EDX, ECX
									CMP EDX, nDestBufferWidth
									JGE LOOP2END	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)		

									MOV EAX, pSrc		// ��ȡpSrc[j]��ֵ
									MOV EDX, ECX
									SHL EDX, 2
									ADD EAX, EDX 
									MOVD MM0, [EAX]

									PUSH ECX
										MOV ECX, [EAX]					// ��ȡ��Alphaֵ
										SHR	ECX,  24					// ����24λ
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A
											POP ECX

											XOR EAX, EAX	// ��0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A

											MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
											MOV EDX, ECX
											SHL EDX, 2
											ADD EAX, EDX

											PUSH  EAX		 // ����ָ�룬����Ҫ��
											MOVD MM1, [EAX]
											XOR EAX, EAX	// ��0
												MOVD MM2, EAX
												PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
												MOVQ	MM3, MM1	 // ����һ��

												PMULLW MM0, MM6	// ��Ӧ16λ���S(1-A)
												// ����
												PADDSW MM0, MM5
												PSRLW  MM0, 8		// ����8λ���൱��/255)(Ҫ��������)

												PMULLW MM1, MM4	// ��Ӧ16λ���DA
												// ����
												PADDSW MM1, MM5
												PSRLW  MM1, 8
												PADDSW MM0, MM1	// ��Ӧ16λ���
												PACKUSWB MM0, MM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

												POP EAX					// ����pDest
												MOVD  [EAX], MM0
LOOP2END:
											MOV EAX, pDes
												ADD EAX, nAddDestWidth		// nDestBufferWidth���4
												MOV [pDes], EAX

												MOV EAX, pSrc
												ADD EAX, nAddSrcWidth
												MOV [pSrc], EAX

												POP EAX
												INC	EAX			 // ����������������ѭ��1 
												JMP LOOP1
EXITLOOP1:
											EMMS			// ���
	}
#endif
#if 1
	// �Ľ��㷨��2003��11��20���޸��ߣ������
	// KA = (255-SA)+DA-(255-SA)*DA/255, KR = DR+(SR-DR)*(255-SA)/KA
	_declspec(align(16)) DWORD nMask[4] = {0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX			// i��0
LOOPI:
		CMP ECX, nDestHeight	// ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOPI			// �˳���һ��ѭ��(i>=nDestHeight)

			PUSH ECX				// ѹ��i
			MOV  EAX, pSrc			// ����Դ��Ŀ�껺����ָ��
			MOV  EDX, pDes
			XOR  ECX, ECX			// j��0�� �ڶ���ѭ��������
LOOPJ:
		// �ڶ���ѭ���壬һ�ζ�4���ڵ�
		ADD ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����4��
			CMP ECX, nDestWidth
			JG LOOPJ4		// (j>nDestWidth)

			// ȡ��Src��Dest4�������XMM0��XMM1
			MOVUPS XMM2, [EAX]	// ȡ��SRC�ĸ����ARGB
			MOVUPS XMM1, [EDX]	// ȡ��Dest�ĸ����ARGB

			// ȡ��Aֵ�����000A000A000A000A
			PSRAD  XMM2, 24		// ����24λ��ȡAֵ
				PSRAD  XMM1, 24		// ����24λ��ȡAֵ
				PAND   XMM2, nMask		// �ĸ�000A
				PAND   XMM1, nMask		// �ĸ�000A
				MOVUPS XMM0, nMask
				PSUBD  XMM0, XMM2		// 255-SA

				// Src��Dest���Ϊ000A000A000A000A���,����XMM3
				MOVAPS XMM3, XMM0		// ����(255-SA)
				PADDW  XMM3, XMM1		// (255-SA)+DA(16λ��)

				// ��Ӧ16λ��ˣ�����256��(255-SA)*DA>>8)
				PMULLW XMM0, XMM1
				PADDW  XMM0, nMask		// ����
				PSRAD  XMM0, 8			
				//PAND   XMM0, nMask	

				// ��Ӧ16λ��,�õ����յ�KAֵ000A000A000A000A
				PSUBW  XMM3, XMM0	
				PAND   XMM3, nMask
				//////////////////////////////////////////////////////////////////////////
				// �����һ����
				// ÿ����һ���㣨D+(S-D)*(255-SA)/KA��
				// ��ȡ��SRC��RGBA,�����000A000R000G000B����XMM0
				MOVD    MM0, [EAX]
				MOVQ    MM3, MM0		// ����Src��RGBA��������
					PXOR    MM1, MM1
					PUNPCKLBW MM0, MM1
					MOVQ2DQ    XMM0, MM0
					PXOR    XMM2, XMM2
					PUNPCKLWD XMM0, XMM2
					// ת��DWORD��Float,����XMM1
					CVTDQ2PS  XMM1, XMM0
					// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
					MOVD    MM0, [EDX]
					PUNPCKLBW MM0, MM1
						MOVQ2DQ    XMM0, MM0
						PUNPCKLWD XMM0, XMM2
						// ת��DWORD��Float,����XMM2
						CVTDQ2PS  XMM2, XMM0

						// ����ָ��
						PUSH EDX
						// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
						SUBPS  XMM1, XMM2		// S-D
						// ȡ��SA,���(255-SA)*256
						PUSH EAX
						MOV    EAX,  0xFFFFFFFF
						MOVD   MM0,  EAX
						PSUBB  MM0,  MM3
						MOVD   EAX,  MM0
						SHR    EAX,  16
						AND    EAX,  0x0000FF00  
						// ȡ��KA
						XOR    EDX, EDX
						PEXTRW  EDX, XMM3, 0	// �����һ��KAֵ
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
							// ת��������(000A000R000G000B)
							CVTPS2DQ XMM2, XMM1
							// ������ȷ��Aֵ
							PINSRW XMM2, EDX, 6 
							POP EDX
							// ѹ��RGBA
							PACKUSWB XMM2, XMM2
							MOVDQ2Q   MM2, XMM2
							PACKUSWB MM2, MM2
							MOVD  [EDX], MM2	// ����Destһ�����ARGB
							ADD EDX, 4
							//////////////////////////////////////////////////////////////////////////
							//////////////////////////////////////////////////////////////////////////
							// ����ڶ�����
							// ÿ����һ���㣨D+(S-D)*SA/KA��
							// ��ȡ��SRC��RGBA,�����000A000R000G000B����XMM0
							MOVD    MM0, [EAX]
							MOVQ    MM3, MM0		// ����Src��RGBA��������
								PXOR    MM1, MM1
								PUNPCKLBW MM0, MM1
								MOVQ2DQ    XMM0, MM0
								PXOR    XMM2, XMM2
								PUNPCKLWD XMM0, XMM2
								// ת��DWORD��Float,����XMM1
								CVTDQ2PS  XMM1, XMM0
								// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
								MOVD    MM0, [EDX]
								PUNPCKLBW MM0, MM1
									MOVQ2DQ    XMM0, MM0
									PUNPCKLWD XMM0, XMM2
									// ת��DWORD��Float,����XMM2
									CVTDQ2PS  XMM2, XMM0

									// ����ָ��
									PUSH EDX
									// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
									SUBPS  XMM1, XMM2		// S-D
									// ȡ��SA,���(255-SA)*256
									PUSH EAX
									MOV    EAX,  0xFFFFFFFF
									MOVD   MM0,  EAX
									PSUBB  MM0,  MM3
									MOVD   EAX,  MM0
									SHR    EAX,  16
									AND    EAX,  0x0000FF00  // ���A000
									// ȡ��KA
									XOR    EDX, EDX
									PEXTRW  EDX, XMM3, 2	// ����ڶ���KAֵ
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
										// ת��������(000A000R000G000B)
										CVTPS2DQ XMM2, XMM1
										// ������ȷ��Aֵ
										PINSRW XMM2, EDX, 6 
										POP EDX
										// ѹ��RGBA
										PACKUSWB XMM2, XMM2
										MOVDQ2Q   MM2, XMM2
										PACKUSWB MM2, MM2
										MOVD  [EDX], MM2	// ����Destһ�����ARGB
										ADD EDX, 4
										//////////////////////////////////////////////////////////////////////////
										//////////////////////////////////////////////////////////////////////////
										// �����������
										// ÿ����һ���㣨D+(S-D)*SA/KA��
										// ��ȡ��SRC��RGBA,�����000A000R000G000B����XMM0
										MOVD    MM0, [EAX]
										MOVQ    MM3, MM0		// ����Src��RGBA��������
											PXOR    MM1, MM1
											PUNPCKLBW MM0, MM1
											MOVQ2DQ    XMM0, MM0
											PXOR    XMM2, XMM2
											PUNPCKLWD XMM0, XMM2
											// ת��DWORD��Float,����XMM1
											CVTDQ2PS  XMM1, XMM0
											// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
											MOVD    MM0, [EDX]
											PUNPCKLBW MM0, MM1
												MOVQ2DQ    XMM0, MM0
												PUNPCKLWD XMM0, XMM2
												// ת��DWORD��Float,����XMM2
												CVTDQ2PS  XMM2, XMM0

												// ����ָ��
												PUSH EDX
												// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
												SUBPS  XMM1, XMM2		// S-D
												// ȡ��SA,���(255-SA)*256
												PUSH EAX
												MOV    EAX,  0xFFFFFFFF
												MOVD   MM0,  EAX
												PSUBB  MM0,  MM3
												MOVD   EAX,  MM0
												SHR    EAX,  16
												AND    EAX,  0x0000FF00  // ���A000
												// ȡ��KA
												XOR    EDX, EDX
												PEXTRW  EDX, XMM3, 4	// ���������KAֵ
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
													// ת��������(000A000R000G000B)
													CVTPS2DQ XMM2, XMM1
													// ������ȷ��Aֵ
													PINSRW XMM2, EDX, 6 
													POP EDX
													// ѹ��RGBA
													PACKUSWB XMM2, XMM2
													MOVDQ2Q   MM2, XMM2
													PACKUSWB MM2, MM2
													MOVD  [EDX], MM2	// ����Destһ�����ARGB
													ADD EDX, 4
													//////////////////////////////////////////////////////////////////////////
													//////////////////////////////////////////////////////////////////////////
													// ������ĸ���
													// ÿ����һ���㣨D+(S-D)*SA/KA��
													// ��ȡ��SRC��RGBA,�����000A000R000G000B����XMM0
													MOVD    MM0, [EAX]
													MOVQ    MM3, MM0		// ����Src��RGBA��������
														PXOR    MM1, MM1
														PUNPCKLBW MM0, MM1
														MOVQ2DQ    XMM0, MM0
														PXOR    XMM2, XMM2
														PUNPCKLWD XMM0, XMM2
														// ת��DWORD��Float,����XMM1
														CVTDQ2PS  XMM1, XMM0
														// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
														MOVD    MM0, [EDX]
														PUNPCKLBW MM0, MM1
															MOVQ2DQ    XMM0, MM0
															PUNPCKLWD XMM0, XMM2
															// ת��DWORD��Float,����XMM2
															CVTDQ2PS  XMM2, XMM0

															// ����ָ��
															PUSH EDX
															// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
															SUBPS  XMM1, XMM2		// S-D
															PUSH EAX
															// ȡ��SA,���(255-SA)*256
															PUSH EAX
															MOV    EAX,  0xFFFFFFFF
															MOVD   MM0,  EAX
															PSUBB  MM0,  MM3
															MOVD   EAX,  MM0
															SHR    EAX,  16
															AND    EAX,  0x0000FF00  // ���A000
															// ȡ��KA
															XOR    EDX, EDX
															PEXTRW  EDX, XMM3, 6	// �����һ��KAֵ
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
																// ת��������(000A000R000G000B)
																CVTPS2DQ XMM2, XMM1
																// ������ȷ��Aֵ
																PINSRW XMM2, EDX, 6 
																POP EDX
																// ѹ��RGBA
																PACKUSWB XMM2, XMM2
																MOVDQ2Q   MM2, XMM2
																PACKUSWB MM2, MM2
																MOVD  [EDX], MM2	// ����Destһ�����ARGB
																ADD EDX, 4
																//////////////////////////////////////////////////////////////////////////
																JMP LOOPJ
LOOPJ4:
															SUB ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����4��
LOOPJ41:
															CMP ECX, nDestWidth
																JGE LOOPJEND

																// ����Ŀ��A��ֵ
																// ȡ��Src��Dest1����
																MOVD MM2, [EAX]	// ȡ��SRC1�����ARGB
																MOVD MM1, [EDX]	// ȡ��Dest1�����ARGB

																// ����Src��Dest
																MOVQ MM3, MM2
																	MOVQ MM4, MM1

																	// ȡ��SA��DA
																	PSRAD MM2, 24	// �õ�000Aֵ
																	PAND  MM2, nMask
																	MOVQ  MM0, nMask
																	PSUBB MM0, MM2  // 255-SA
																	PSRAD MM1, 24
																	PAND MM1, nMask

																	// ����255-SA
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
																	// ���Ŀ��RGBA
																	// ���Src��Dest��RGBAֵ
																	PXOR    MM1, MM1
																	PUNPCKLBW MM3, MM1
																	MOVQ2DQ    XMM0, MM3
																	PXOR    XMM1, XMM1
																	PUNPCKLWD XMM0, XMM1
																	// ת��DWORD��Float,����XMM1
																	CVTDQ2PS  XMM1, XMM0
																	// ȡ��Dest��RGBA�������000A000R000G000B����XMM0
																	PUNPCKLBW MM4, MM1
																	MOVQ2DQ    XMM0, MM4
																	PXOR    XMM2, XMM2
																	PUNPCKLWD XMM0, XMM2
																	// ת��DWORD��Float,����XMM2
																	CVTDQ2PS  XMM2, XMM0

																	// ����ָ��
																	PUSH EDX
																	PUSH EAX
																	PUSH ECX
																	// ��SA,KAֵ��Ӧ��������SA/KA����XMM4��4������000R000R000R000R��
																	SUBPS  XMM1, XMM2		// S-D
																	// ȡ��SA,���(255-SA)*256
																	MOV    EDX,  0x000000FF
																	MOVD   EAX,  MM7
																	SUB    EDX,  EAX
																	MOV    EAX,  EDX
																	SHL    EAX,  8
																	AND    EAX,  0x0000FF00  // ���A000
																	// ȡ��KA
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
																		// ת��������(000A000R000G000B)
																		CVTPS2DQ XMM2, XMM1
																		// ������ȷ��Aֵ
																		PINSRW XMM2, EDX, 6 
																		POP EDX
																		// ѹ��RGBA
																		PACKUSWB XMM2, XMM2
																		MOVDQ2Q   MM2, XMM2
																		PACKUSWB MM2, MM2
																		MOVD  [EDX], MM2	// ����Destһ�����ARGB

																		ADD EDX, 4
																		JMP LOOPJ41
LOOPJEND:
																	MOV EAX, nAddDestWidth		// nDestBufferWidth���4
																		ADD [pDes], EAX

																		MOV EAX, nAddSrcWidth
																		ADD [pSrc], EAX 
																		POP ECX
																		ADD ECX, nInc
																		JMP LOOPI
EXITLOOPI:
																	EMMS			// ���
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
// ���ܣ��������쿽��
// 
// 2003-10-10,V1.0,��д�ߣ������
//
// 2003-11-20,V2.0,�޸��ߣ������
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
	// ���ݷ�Χ���
	if(nSrcX>= nSrcBufferWidth || nSrcY>=nSrcBufferHeight)
		return TRUE;
	if(nDestX>=nDestBufferWidth || nDestY>=nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcWidth<=0 || nSrcHeight<=0 ||nDestWidth <=0 ||nDestHeight<=0)
		return TRUE;

	// Դ��������Ŀ�껺�����ɱ�������:���ű���
	float fWidth = ((float)(nSrcWidth))/nDestWidth;
	float fHeight = ((float)(nSrcHeight))/nDestHeight;

	// ����Ŀ���Դ��㣬Ŀ���Դ�Ŀ����߶�
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

	// ָ�����
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	DWORD* pTemp = pSrc;
	if(pSrc == NULL || pDes == NULL)
		return TRUE;

	// �������Ŀ�Ⱥ͸߶��Ƚ���
	DWORD* pnAddHeight = NULL; 
	DWORD* pnAddWidth = NULL;
	TPBuffer_CreateTable(pnAddHeight, nDestHeight, fHeight);
	TPBuffer_CreateTable(pnAddWidth, nDestWidth, fWidth);
	DWORD* pnAddHeight1 = pnAddHeight;

	//////////////////////////////////////////////////////////////////////////
	// ��������ż����Ϣ��2004��02��11��chyf
	// ÿ�μӵĿ��
	int nAddDestWidth = nDestBufferWidth<<2;
	int nAddSrcWidth = nSrcBufferWidth<<2;
	int nInc = 0;		// ������������
	int nTableInc = 0;	// ��ָ������
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

	if(nDestY % 2 == 0)	// ���Ϊ�泡
	{
		if(nParity == TP_PARITY_EVEN)
		{
			// ���ż�������������һ�п�ʼ
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
		// ���Ϊż��
		if(nParity == TP_PARITY_ODD)
		{
			// ����泡�����������һ�п�ʼ
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
	// ������ż����Ϣ
	//////////////////////////////////////////////////////////////////////////

#if 1
	// һ���и�ֵ,SSE2�Ż�
	_asm
	{
		XOR ECX, ECX	// i��0
			MOV EAX, pnAddHeight1	// Ҫ�ӵĸ߶�
LOOP1:
		CMP ECX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1		 // �˳���һ��ѭ��(i>=nDestHeight)

			MOVD MM0, [EAX]
			MOVD MM1, nAddSrcWidth
				PMULUDQ MM0, MM1
				MOVD EDX, MM0
				ADD EDX, pSrc
				MOV [pTemp], EDX
				ADD EAX, nTableInc

				// i��Ҫ�ӵĸ߶�ָ��ѹ��ջ
				PUSH ECX
				PUSH EAX

				XOR ECX, ECX	     // ��0�� �ڶ���ѭ��������
				MOV EDX, pnAddWidth  // Ҫ�ӵĿ��
				MOV EAX, pDes		 // Ŀ��ָ��
LOOP2:
			ADD ECX, 4			 // ÿ�μ���4��ֵ������4ʱ����ѭ����2��
				CMP ECX, nDestWidth
				JG LOOP2BODY2		

				// jֵѹջ
				PUSH ECX

				// ��ȡpTemp[pnAddWidth[j]]��ֵ����MM0
				MOV ECX, [EDX]
				SHL ECX, 2		// pnAddWidth[j]*4
					ADD ECX, pTemp 
					MOVD MM0, [ECX]			
					ADD EDX, 4

						// ��ȡpTemp[pnAddWidth[j+1]]��ֵ
						MOV ECX, [EDX]
						SHL ECX, 2
							ADD ECX, pTemp		
							MOVD MM1, [ECX]
							ADD EDX, 4						

								// ����ֵ���
								PUNPCKLDQ MM0, MM1

								// ��ȡpTemp[pnAddWidth[j+2]]��ֵ����MM0
								MOV ECX, [EDX]
								SHL ECX, 2
									ADD ECX, pTemp 
									MOVD MM3, [ECX]			
									ADD EDX, 4

										// ��ȡpTemp[pnAddWidth[j+3]]��ֵ
										MOV ECX, [EDX]
										SHL ECX, 2
											ADD ECX, pTemp 		
											MOVD MM4, [ECX]
											ADD EDX, 4		

												PUNPCKLDQ MM3, MM4
												MOVQ2DQ	XMM0, MM0
												MOVQ2DQ	XMM1, MM3
												PUNPCKLQDQ XMM0, XMM1

												// ����pDes[j]��ֵ
												POP ECX
												MOVUPS  [EAX], XMM0
												ADD EAX, 10H

												JMP LOOP2
LOOP2BODY2:
											SUB ECX, 4			// ����4ʱ
LOOP2BODY3:
											CMP ECX, nDestWidth
												JGE LOOP2END

												// ��ȡpTemp[pnAddWidth[j]]��ֵ
												// jѹջ
												PUSH ECX
												MOV ECX, [EDX]
												SHL ECX, 2
													ADD ECX, pTemp 
													MOVD MM0, [ECX]
													ADD EDX, 4

														// ��ȡpDes[j]��ֵ
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
														ADD	ECX, nInc			 // ����������������ѭ��1 
														JMP LOOP1
EXITLOOP1:
													EMMS			// ���
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
// ���ܣ���������Mix����
// 2003��11��V1.0,�����
// �㷨��
// 2004��02��11��V2.0,�����
// �㷨��
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
						 int nParity)				// ��ż��
{
	// Դ��������Ŀ�껺�����ɱ�������
	if(nSrcX>= nSrcBufferWidth || nSrcY>=nSrcBufferHeight)
		return TRUE;
	if(nSrcX < 0)
		nSrcX = 0;
	if(nSrcY < 0)
		nSrcY = 0;
	if(nDestX>=nDestBufferWidth || nDestY>=nDestBufferHeight || nDestWidth<=0 || nDestHeight<=0)
		return TRUE;
	// ���ű���
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

	// �������Ŀ�Ⱥ͸߶��Ƚ���
	DWORD* pnAddHeight = NULL; 
	DWORD* pnAddWidth = NULL;
	TPBuffer_CreateTable(pnAddHeight, nDestHeight, fHeight);
	TPBuffer_CreateTable(pnAddWidth, nDestWidth, fWidth);

	DWORD* pnAddHeight1 = pnAddHeight;

	//////////////////////////////////////////////////////////////////////////
	// ��������ż����Ϣ��2004��02��11��chyf
	// ÿ�μӵĿ��
	int nAddDestWidth = nDestBufferWidth<<2;
	int nAddSrcWidth = nSrcBufferWidth<<2;
	int nInc = 0;		// ������������
	int nTableInc = 0;	// ��ָ������
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

	if(nDestY % 2 == 0)	// ���Ϊ�泡
	{
		if(nParity == TP_PARITY_EVEN)
		{
			// ���ż�������������һ�п�ʼ
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
		// ���Ϊż��
		if(nParity == TP_PARITY_ODD)
		{
			// ����泡�����������һ�п�ʼ
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
	// ������ż����Ϣ
	//////////////////////////////////////////////////////////////////////////

#if 1
	// һ���и�ֵ,SSE2�Ż�
	_asm
	{
		XOR EAX, EAX	// ��0
			PXOR		MM6,MM6
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
			MOV ECX, pnAddHeight
LOOP1:
		CMP EAX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1		 // �˳���һ��ѭ��(i>=nDestHeight)

			MOV	EDX, nSrcY	 // ����nSrcY+pnAddHeight[i]
			ADD EDX, [ECX]
			CMP EDX, nSrcBufferHeight
				JGE	EXITLOOP1	 // �˳���һ��ѭ��(nSrcY+nAddHeight>=nSrcBufferHeight)

				PUSH EAX
				PUSH ECX

				MOV EDX, [ECX]
				IMUL EDX,nAddSrcWidth
					MOV ECX, pSrc
					ADD ECX, EDX
					MOV [pTemp], ECX


					MOV	ECX, nDestY	 // ����nDestY+i
					ADD ECX, EAX
					CMP ECX, nDestBufferHeight
					JGE	EXITLOOP1	 // �˳���һ��ѭ��(nDestY+i>=nDestBufferHeight)

					XOR ECX, ECX	 // ��0�� �ڶ���ѭ��������
					MOV EDX, pnAddWidth
LOOP2:
				CMP ECX, nDestWidth  // �ڶ���ѭ��(j<nDestWidth)
					JGE LOOP2END	 // ����ڶ���ѭ����(j<nDestWidth)

					ADD ECX, 2			// ÿ�μ���2��ֵ������2ʱ����ѭ����2��
					CMP ECX, nDestWidth
					JG LOOP2BODY2		

					SUB ECX, 2

					// ��pnAddWidth[j]>=nSrcBufferWidthʱ������һ��ѭ��
					MOV EAX, nSrcX		// ����nSrcX+pnAddWidth[j]
					ADD EAX, [EDX]
					CMP EAX, nSrcBufferWidth
						JGE LOOP2BODY2		// �˳��ڶ���ѭ��(nSrcX+pnAddWidth[j]>=nSrcBufferWidth)

						// ��pnAddWidth[j+1]>=nSrcBufferWidthʱ������һ��ѭ��
						MOV EAX, nSrcX		// ����nSrcX+pnAddWidth[j+1]
						ADD EAX, [EDX+4]
						CMP EAX, nSrcBufferWidth
							JGE LOOP2BODY2		// �˳��ڶ���ѭ��(nSrcX+pnAddWidth[j+1]>=nSrcBufferWidth)

							MOV EAX, nDestX	// ����nDestX+j
							ADD EAX, ECX
							CMP EAX, nDestBufferWidth
							JGE LOOP2BODY2	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)	

							MOV EAX, pTemp		// ��ȡpTemp[pnAddWidth[j]]��ֵ����MM0
							PUSH ECX
							MOV ECX, [EDX]
							SHL ECX, 2
								ADD EAX, ECX 
								MOVD MM0, [EAX]			
								MOV ECX, [EAX]					// ��ȡ��Alphaֵ
								SHR	ECX,  24					// ����24λ
									AND ECX, 0x000000FF
									MOVD MM3, ECX
									PUNPCKLWD MM3, MM3
									PUNPCKLDQ MM3, MM3				// ��Ϊ0A0A0A0A

									MOV EAX, 0xFF					// 255-Alpha
									SUB EAX, ECX
									MOVD MM7, EAX
									PUNPCKLWD MM7, MM7
									PUNPCKLDQ MM7, MM7				// ��Ϊ0A0A0A0A

									POP ECX
									ADD EDX, 4

									MOV EAX, pTemp		// ��ȡpTemp[pnAddWidth[j+1]]��ֵ
									PUSH ECX
									MOV ECX, [EDX]
									SHL ECX, 2
										ADD EAX, ECX 		
										MOVD MM1, [EAX]
										MOV ECX, [EAX]					// ��ȡ��Alphaֵ
										SHR	ECX,  24					// ����24λ
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A

											POP ECX
											ADD EDX, 4

											XOR EAX, EAX	// ��0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
											PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
											MOVQ2DQ XMM0, MM0
											MOVQ2DQ XMM1, MM1
											PUNPCKLQDQ XMM0, XMM1 // ��Ϊ0R0G0B0A0R0G0B0A

											MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
											PUSH ECX
											SHL ECX, 2
											ADD EAX, ECX
											POP ECX
											PUSH  EAX		 // ����ָ�룬����Ҫ��

											MOVD MM0, [EAX]
											ADD EAX, 4		// ��ȡpDes[j+1]��ֵ
												MOVD MM1, [EAX]
												XOR EAX, EAX	// ��0
													MOVD MM2, EAX
													PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
													PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
													MOVQ2DQ XMM1, MM0
													MOVQ2DQ XMM2, MM1
													PUNPCKLQDQ XMM1, XMM2 // ��Ϊ0R0G0B0A0R0G0B0A
													MOVDQA	XMM3, XMM1	 // ����һ��

													MOVQ2DQ XMM2, MM3		// ȡԴ��alphaֵ
													MOVQ2DQ XMM4, MM4
													PUNPCKLQDQ XMM2, XMM4	// ��Ϊ0A0A0A0A0M0M0M0M

													MOVQ2DQ XMM4, MM7		// ȡ255-alphaֵ
													MOVQ2DQ XMM5, MM6
													PUNPCKLQDQ XMM4, XMM5	// ��Ϊ0A0A0A0A0M0M0M0M

													PMULLW XMM0, XMM2	// ��Ӧ16λ���SA
													// ����
													MOVQ2DQ		XMM7,MM5
													MOVLHPS		XMM7,XMM7
													PADDSW		XMM0,XMM7
													PSRLW  XMM0, 8		// ÿ16λ����8λ���൱��/255)(Ҫ��������)

													PMULLW XMM1, XMM4	// ��Ӧ16λ���D(1-A)
													// ����
													PADDSW XMM1, XMM7
													PSRLW  XMM1, 8

													PADDSW XMM0, XMM1	// ��Ӧ16λ���(SA+DA)

													PACKUSWB XMM0, XMM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

													MOVDQ2Q	MM0,XMM0	// ȡ��64λ(����DWORD)
													POP EAX					// ����pDest
													MOVQ  [EAX], MM0

													ADD ECX, 2			// j��2�����صڶ���ѭ��
													JMP LOOP2
LOOP2BODY2:
												SUB ECX, 2			// ����2ʱ
													CMP ECX, nDestWidth
													JGE LOOP2END

													MOV EAX, nSrcX	// ����nSrcX+pnAddWidth[j]
													ADD EAX, [EDX]
													CMP EAX, nSrcBufferWidth
														JGE LOOP2END	// �˳��ڶ���ѭ��(nSrcX+pnAddWidth[j]>=nSrcBufferWidth)

														MOV EAX, nDestX	// ����nDestX+j
														ADD EAX, ECX
														CMP EAX, nDestBufferWidth
														JGE LOOP2END	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)		

														MOV EAX, pTemp		// ��ȡpTemp[pnAddWidth[j]]��ֵ
														PUSH ECX
														MOV ECX, [EDX]
														SHL ECX, 2
															ADD EAX, ECX 
															MOVD MM0, [EAX]

															MOV ECX, [EAX]					// ��ȡ��Alphaֵ
															SHR	ECX,  24					// ����24λ
																AND ECX, 0x000000FF
																MOVD MM4, ECX
																PUNPCKLWD MM4, MM4
																PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

																MOV EAX, 0xFF					// 255-Alpha
																SUB EAX, ECX
																MOVD MM6, EAX
																PUNPCKLWD MM6, MM6
																PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A

																POP ECX
																XOR EAX, EAX	// ��0
																MOVD MM2, EAX
																PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A


																MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
																PUSH ECX
																SHL ECX, 2
																ADD EAX, ECX
																POP ECX
																PUSH  EAX		 // ����ָ�룬����Ҫ��
																MOVD MM1, [EAX]
																XOR EAX, EAX	// ��0
																	MOVD MM2, EAX
																	PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
																	MOVQ	MM3, MM1	 // ����һ��

																	PMULLW MM0, MM4	// ��Ӧ16λ���SA
																	// ����
																	PADDSW MM0, MM5
																	PSRLW  MM0, 8		// ����8λ���൱��/255)(Ҫ��������)

																	PMULLW MM1, MM6	// ��Ӧ16λ���D(1-A)
																	PADDSW MM1, MM5
																	PSRLW  MM1, 8
																	PADDSW MM0, MM1	// ��Ӧ16λ���
																	PACKUSWB MM0, MM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

																	POP EAX					// ����pDest
																	MOVD  [EAX], MM0
LOOP2END:
																MOV EAX, pDes
																	ADD EAX, nAddDestWidth		// nDestBufferWidth���4
																	MOV [pDes], EAX

																	// pSrc+pnAddHeight[i]*nAddSrcWidth*4
																	POP ECX
																	ADD ECX, nTableInc

																	POP EAX
																	ADD	EAX, nInc			 // ����������������ѭ��1 
																	JMP LOOP1
EXITLOOP1:
																EMMS			// ���
	}
#endif
#if 0
	for(int i=0; i<nDestHeight; i++)
	{

		// Դ�����������ĸ߶�
		if(nDestY+i >= nDestBufferHeight)
			break;
		if(nSrcY+pnAddHeight[i] >= nSrcBufferHeight)
			break;
		pTemp = pSrc+pnAddHeight[i]*nSrcBufferWidth; 
		for(int j=0; j<nDestWidth; j++)
		{
			// Դ�����������Ŀ��
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
// ��������NotMix����
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
	// Դ��������Ŀ�껺�����ɱ�������
	if(nSrcX>= nSrcBufferWidth || nSrcY>=nSrcBufferHeight)
		return TRUE;
	if(nSrcX < 0)
		nSrcX = 0;
	if(nSrcY < 0)
		nSrcY = 0;
	if(nDestX>=nDestBufferWidth || nDestY>=nDestBufferHeight || nDestWidth<=0 || nDestHeight<=0)
		return TRUE;
	// ���ű���
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


	// �������Ŀ�Ⱥ͸߶��Ƚ���
	DWORD* pnAddHeight = NULL; 
	DWORD* pnAddWidth = NULL;
	TPBuffer_CreateTable(pnAddHeight, nDestHeight, fHeight);
	TPBuffer_CreateTable(pnAddWidth, nDestWidth, fWidth);

	DWORD* pnAddHeight1 = pnAddHeight;

	//////////////////////////////////////////////////////////////////////////
	// ��������ż����Ϣ��2004��02��11��chyf
	// ÿ�μӵĿ��
	int nAddDestWidth = nDestBufferWidth<<2;
	int nAddSrcWidth = nSrcBufferWidth<<2;
	int nInc = 0;		// ������������
	int nTableInc = 0;	// ��ָ������
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

	if(nDestY % 2 == 0)	// ���Ϊ�泡
	{
		if(nParity == TP_PARITY_EVEN)
		{
			// ���ż�������������һ�п�ʼ
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
		// ���Ϊż��
		if(nParity == TP_PARITY_ODD)
		{
			// ����泡�����������һ�п�ʼ
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
	// ������ż����Ϣ
	//////////////////////////////////////////////////////////////////////////

#if 1
	// һ���и�ֵ,SSE2�Ż�
	_asm
	{
		XOR EAX, EAX	// ��0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
			MOV ECX, pnAddHeight
LOOP1:
		CMP EAX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1		 // �˳���һ��ѭ��(i>=nDestHeight)

			MOV	EDX, nSrcY	 // ����nSrcY+pnAddHeight[i]
			ADD EDX, [ECX]
			CMP EDX, nSrcBufferHeight
				JGE	EXITLOOP1	 // �˳���һ��ѭ��(nSrcY+nAddHeight>=nSrcBufferHeight)
				PUSH EAX
				PUSH ECX

				MOV	ECX, nDestY	 // ����nDestY+i
				ADD ECX, EAX
				CMP ECX, nDestBufferHeight
				JGE	EXITLOOP1	 // �˳���һ��ѭ��(nDestY+i>=nDestBufferHeight)

				XOR ECX, ECX	 // ��0�� �ڶ���ѭ��������
				MOV EDX, pnAddWidth
LOOP2:
			CMP ECX, nDestWidth  // �ڶ���ѭ��(j<nDestWidth)
				JGE LOOP2END	 // ����ڶ���ѭ����(j<nDestWidth)	

				ADD ECX, 2			// ÿ�μ���2��ֵ������2ʱ����ѭ����2��
				CMP ECX, nDestWidth
				JG LOOP2BODY2		

				SUB ECX, 2

				// ��pnAddWidth[j]>=nSrcBufferWidthʱ������һ��ѭ��
				MOV EAX, nSrcX		// ����nSrcX+pnAddWidth[j]
				ADD EAX, [EDX]
				CMP EAX, nSrcBufferWidth
					JGE LOOP2BODY2		// �˳��ڶ���ѭ��(nSrcX+pnAddWidth[j]>=nSrcBufferWidth)

					// ��pnAddWidth[j+1]>=nSrcBufferWidthʱ������һ��ѭ��
					MOV EAX, nSrcX		// ����nSrcX+pnAddWidth[j+1]
					ADD EAX, [EDX+4]
					CMP EAX, nSrcBufferWidth
						JGE LOOP2BODY2		// �˳��ڶ���ѭ��(nSrcX+pnAddWidth[j+1]>=nSrcBufferWidth)

						MOV EAX, nDestX	// ����nDestX+j
						ADD EAX, ECX
						CMP EAX, nDestBufferWidth
						JGE LOOP2BODY2	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)	

						MOV EAX, pTemp		// ��ȡpTemp[pnAddWidth[j]]��ֵ����MM0
						PUSH ECX
						MOV ECX, [EDX]
						SHL ECX, 2
							ADD EAX, ECX 
							MOVD MM0, [EAX]			
							MOV ECX, [EAX]					// ��ȡ��Alphaֵ
							SHR	ECX,  24					// ����24λ
								AND ECX, 0x000000FF
								MOVD MM3, ECX
								PUNPCKLWD MM3, MM3
								PUNPCKLDQ MM3, MM3				// ��Ϊ0A0A0A0A

								MOV EAX, 0xFF					// 255-Alpha
								SUB EAX, ECX
								MOVD MM7, EAX
								PUNPCKLWD MM7, MM7
								PUNPCKLDQ MM7, MM7				// ��Ϊ0A0A0A0A
								POP ECX

								ADD EDX, 4

								MOV EAX, pTemp		// ��ȡpTemp[pnAddWidth[j+1]]��ֵ
								PUSH ECX
								MOV ECX, [EDX]
								SHL ECX, 2
									ADD EAX, ECX 		
									MOVD MM1, [EAX]
									MOV ECX, [EAX]					// ��ȡ��Alphaֵ
									SHR	ECX,  24					// ����24λ
										AND ECX, 0x000000FF
										MOVD MM4, ECX
										PUNPCKLWD MM4, MM4
										PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

										MOV EAX, 0xFF					// 255-Alpha
										SUB EAX, ECX
										MOVD MM6, EAX
										PUNPCKLWD MM6, MM6
										PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A

										POP ECX
										ADD EDX, 4

										XOR EAX, EAX	// ��0
										MOVD MM2, EAX
										PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
										PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
										MOVQ2DQ XMM0, MM0
										MOVQ2DQ XMM1, MM1
										PUNPCKLQDQ XMM0, XMM1 // ��Ϊ0R0G0B0A0R0G0B0A

										MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
										PUSH ECX
										SHL ECX, 2
										ADD EAX, ECX
										POP ECX
										PUSH  EAX		 // ����ָ�룬����Ҫ��

										MOVD MM0, [EAX]
										ADD EAX, 4		// ��ȡpDes[j+1]��ֵ
											MOVD MM1, [EAX]
											XOR EAX, EAX	// ��0
												MOVD MM2, EAX
												PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
												PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
												MOVQ2DQ XMM1, MM0
												MOVQ2DQ XMM2, MM1
												PUNPCKLQDQ XMM1, XMM2 // ��Ϊ0R0G0B0A0R0G0B0A
												MOVDQA	XMM3, XMM1	 // ����һ��

												MOVQ2DQ XMM2, MM3		// ȡԴ��alphaֵ
												MOVQ2DQ XMM4, MM4
												PUNPCKLQDQ XMM2, XMM4	// ��Ϊ0A0A0A0A0M0M0M0M

												MOVQ2DQ XMM4, MM7		// ȡ255-alphaֵ
												MOVQ2DQ XMM5, MM6
												PUNPCKLQDQ XMM4, XMM5	// ��Ϊ0A0A0A0A0M0M0M0M

												PMULLW XMM0, XMM4	// ��Ӧ16λ���S(1-A)
												// ����
												MOVQ2DQ		XMM7,MM5
												MOVLHPS		XMM7,XMM7
												PADDSW		XMM0,XMM7
												PSRLW  XMM0, 8		// ÿ16λ����8λ���൱��/255)(Ҫ��������)

												PMULLW XMM1, XMM2	// ��Ӧ16λ���DA
												PADDSW XMM1,XMM7
												PSRLW  XMM1, 8

												PADDSW XMM0, XMM1	// ��Ӧ16λ���(SA+DA)

												PACKUSWB XMM0, XMM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

												MOVDQ2Q	MM0,XMM0	// ȡ��64λ(����DWORD)
												POP EAX					// ����pDest
												MOVQ  [EAX], MM0

												ADD ECX, 2			// j��2�����صڶ���ѭ��
												JMP LOOP2
LOOP2BODY2:
											SUB ECX, 2			// ����2ʱ
												CMP ECX, nDestWidth
												JGE LOOP2END

												MOV EAX, nSrcX	// ����nSrcX+pnAddWidth[j]
												ADD EAX, [EDX]
												CMP EAX, nSrcBufferWidth
													JGE LOOP2END	// �˳��ڶ���ѭ��(nSrcX+pnAddWidth[j]>=nSrcBufferWidth)

													MOV EAX, nDestX	// ����nDestX+j
													ADD EAX, ECX
													CMP EAX, nDestBufferWidth
													JGE LOOP2END	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)	

													MOV EAX, pTemp		// ��ȡpTemp[pnAddWidth[j]]��ֵ
													PUSH ECX
													MOV ECX, [EDX]
													SHL ECX, 2
														ADD EAX, ECX 
														MOVD MM0, [EAX]

														MOV ECX, [EAX]					// ��ȡ��Alphaֵ
														SHR	ECX,  24					// ����24λ
															AND ECX, 0x000000FF
															MOVD MM4, ECX
															PUNPCKLWD MM4, MM4
															PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

															MOV EAX, 0xFF					// 255-Alpha
															SUB EAX, ECX
															MOVD MM6, EAX
															PUNPCKLWD MM6, MM6
															PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A

															POP ECX
															XOR EAX, EAX	// ��0
															MOVD MM2, EAX
															PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A

															MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
															PUSH ECX
															SHL ECX, 2
															ADD EAX, ECX
															POP ECX
															PUSH  EAX		 // ����ָ�룬����Ҫ��
															MOVD MM1, [EAX]
															XOR EAX, EAX	// ��0
																MOVD MM2, EAX
																PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
																MOVQ	MM3, MM1	 // ����һ��

																PMULLW MM0, MM6	// ��Ӧ16λ���S(1-A)
																PADDSW MM0, MM5
																PSRLW  MM0, 8		// ����8λ���൱��/255)(Ҫ��������)

																PMULLW MM1, MM4	// ��Ӧ16λ���DA
																PADDSW MM1, MM5 
																PSRLW  MM1, 8
																PADDSW MM0, MM1	// ��Ӧ16λ���
																PACKUSWB MM0, MM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

																POP EAX					// ����pDest
																MOVD  [EAX], MM0
LOOP2END:
															MOV EAX, pDes
																ADD EAX, nAddDestWidth		// nDestBufferWidth���4
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
																	ADD	EAX, nInc			 // ����������������ѭ��1 
																	JMP LOOP1
EXITLOOP1:
																EMMS			// ���
	}
#endif
#if 0
	for(int i=0; i<nDestHeight; i++)
	{
		// Դ�����������ĸ߶�
		if(nDestY+i >= nDestBufferHeight)
			break;
		if(nSrcY+pnAddHeight[i] >= nSrcBufferHeight)
			break;
		for(int j=0; j<nDestWidth; j++)
		{
			// Դ�����������Ŀ��
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
// ���ܣ�����NotMix��仺��������ԴMixȡ��
// 
// 2003��10-10,V1.0���޸��ߣ������
// �Ľ��㷨��KR = (255-SA)/255*SR+SA/255*DR
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
	// ���ݷ�Χ���
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// �õ���������Ҫ�����ĳ���
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;
	// ÿ�μӵĿ��
	int nAddDestWidth = nDestBufferWidth<<2;	// ����2λ���൱�ڳ�4
	int nAddSrcWidth = nSrcBufferWidth<<2;

#if 1
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};
	// һ���и�ֵ,SSE2�Ż�
	_asm
	{
		XOR EAX, EAX	// ��0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1		 // �˳���һ��ѭ��(i>=nDestHeight)
			MOV	ECX, nDestY		// ����nDestY+i
			ADD ECX, EAX
			CMP ECX, nDestBufferHeight
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(nDestY+i>=nDestBufferHeight)
			MOV	ECX, nSrcY	 // ����nSrcY+i
			ADD ECX, EAX
			CMP ECX, nSrcBufferHeight
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(nSrcY+i>=nSrcBufferHeight)
			XOR ECX, ECX	 // ��0�� �ڶ���ѭ��������
			PUSH EAX
LOOP2:
		CMP ECX, nDestWidth  // �ڶ���ѭ��(j<nDestWidth)
			JGE LOOP2END		 // ����ڶ���ѭ����(j<nDestWidth)

			ADD ECX, 2			 // ÿ�μ���2��ֵ������2ʱ����ѭ����2��
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2

			MOV EDX, nSrcX	// ����nSrcX+j
			ADD EDX, ECX
			CMP EDX, nSrcBufferWidth
			JGE LOOP2BODY2	// �˳��ڶ���ѭ��(nSrcX+j>=nSrcBufferWidth)

			MOV EDX, nDestX	// ����nDestX+j
			ADD EDX, ECX
			CMP EDX, nDestBufferWidth
			JGE LOOP2BODY2	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)	

			MOV EAX, pSrc	// ��ȡpSrc[j]��ֵ����MM0
			MOV EDX, ECX
			SHL EDX, 2
			ADD EAX, EDX 
			MOVD MM0, [EAX]

			PUSH ECX
				MOV ECX, [EAX]					// ��ȡ��Alphaֵ
				SHR	ECX,  24					// ����24λ
					AND ECX, 0x000000FF
					MOVD MM3, ECX
					PUNPCKLWD MM3, MM3
					PUNPCKLDQ MM3, MM3				// ��Ϊ0A0A0A0A

					MOV EDX, 0xFF					// 255-Alpha
					SUB EDX, ECX
					MOVD MM7, EDX
					PUNPCKLWD MM7, MM7
					PUNPCKLDQ MM7, MM7				// ��Ϊ0A0A0A0A
					POP ECX

					ADD EAX, 4		// ��ȡpSrc[j+1]��ֵ����MM1
					MOVD MM1, [EAX]

					PUSH ECX
						MOV ECX, [EAX]					// ��ȡ��Alphaֵ
						SHR	ECX,  24					// ����24λ
							AND ECX, 0x000000FF
							MOVD MM4, ECX
							PUNPCKLWD MM4, MM4
							PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

							MOV EAX, 0xFF					// 255-Alpha
							SUB EAX, ECX
							MOVD MM6, EAX
							PUNPCKLWD MM6, MM6
							PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A
							POP ECX

							XOR EAX, EAX	// ��0
							MOVD MM2, EAX
							PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
							PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
							MOVQ2DQ XMM0, MM0
							MOVQ2DQ XMM1, MM1
							PUNPCKLQDQ XMM0, XMM1 // ��Ϊ0R0G0B0A0R0G0B0A

							MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // ����ָ�룬����Ҫ��
							MOVD MM0, [EAX]
							ADD EAX, 4		// ��ȡpDes[j+1]��ֵ
								MOVD MM1, [EAX]
								XOR EAX, EAX	// ��0
									MOVD MM2, EAX
									PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
									PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
									MOVQ2DQ XMM1, MM0
									MOVQ2DQ XMM2, MM1
									PUNPCKLQDQ XMM1, XMM2 // ��Ϊ0R0G0B0A0R0G0B0A
									MOVDQA	XMM3, XMM1	 // ����һ��

									MOVQ2DQ XMM2, MM3		// ȡԴ��alphaֵ
									MOVQ2DQ XMM4, MM4
									PUNPCKLQDQ XMM2, XMM4	// ��Ϊ0A0A0A0A0M0M0M0M

									MOVQ2DQ XMM4, MM7		// ȡ255-alphaֵ
									MOVQ2DQ XMM5, MM6
									PUNPCKLQDQ XMM4, XMM5	// ��Ϊ0A0A0A0A0M0M0M0M

									PMULLW XMM0, XMM4	// ��Ӧ16λ���S(1-A)
									// ����
									MOVQ2DQ		XMM7,MM5
									MOVLHPS		XMM7,XMM7
									PADDSW		XMM0,XMM7
									PSRLW  XMM0, 8		// ÿ16λ����8λ���൱��/255)(Ҫ��������)

									PMULLW XMM1, XMM2	// ��Ӧ16λ���DA
									// ����
									PADDSW		XMM1,XMM7
									PSRLW  XMM1, 8

									PADDSW XMM0, XMM1	// ��Ӧ16λ���(SA+DA)

									PACKUSWB XMM0, XMM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

									MOVDQ2Q	MM0,XMM0	// ȡ��64λ(����DWORD)
									POP EAX					// ����pDest
									MOVQ  [EAX], MM0

									ADD ECX, 2			// j��2�����صڶ���ѭ��
									JMP LOOP2
LOOP2BODY2:
								SUB ECX, 2			// ����2ʱ
									CMP ECX, nDestWidth
									JGE LOOP2END

									MOV EDX, nSrcX	// ����nSrcX+j
									ADD EDX, ECX
									CMP EDX, nSrcBufferWidth
									JGE LOOP2END	// �˳��ڶ���ѭ��(nSrcX+j>=nSrcBufferWidth)

									MOV EDX, nDestX	// ����nDestX+j
									ADD EDX, ECX
									CMP EDX, nDestBufferWidth
									JGE LOOP2END	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)		

									MOV EAX, pSrc		// ��ȡpSrc[j]��ֵ
									MOV EDX, ECX
									SHL EDX, 2
									ADD EAX, EDX 
									MOVD MM0, [EAX]

									PUSH ECX
										MOV ECX, [EAX]					// ��ȡ��Alphaֵ
										SHR	ECX,  24					// ����24λ
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A
											POP ECX

											XOR EAX, EAX	// ��0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A

											MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
											MOV EDX, ECX
											SHL EDX, 2
											ADD EAX, EDX

											PUSH  EAX		 // ����ָ�룬����Ҫ��
											MOVD MM1, [EAX]
											XOR EAX, EAX	// ��0
												MOVD MM2, EAX
												PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
												MOVQ	MM3, MM1	 // ����һ��

												PMULLW MM0, MM6	// ��Ӧ16λ���S(1-A)
												// ����
												PADDSW MM0, MM5
												PSRLW  MM0, 8		// ����8λ���൱��/255)(Ҫ��������)

												PMULLW MM1, MM4	// ��Ӧ16λ���DA
												// ����
												PADDSW MM1, MM5
												PSRLW  MM1, 8
												PADDSW MM0, MM1	// ��Ӧ16λ���
												PACKUSWB MM0, MM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

												POP EAX					// ����pDest
												MOVD  [EAX], MM0
LOOP2END:
											MOV EAX, pDes
												ADD EAX, nAddDestWidth		// nDestBufferWidth���4
												MOV [pDes], EAX

												MOV EAX, pSrc
												ADD EAX, nAddSrcWidth
												MOV [pSrc], EAX

												POP EAX
												INC	EAX			 // ����������������ѭ��1 
												JMP LOOP1
EXITLOOP1:
											EMMS			// ���
	}
#endif
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// ���ܣ�����Mix��仺��������ʱnSrcWidth==nDestWidth, nSrcHeight==nDestHeight
//
// 1��2003-10-10,V1.0,��д�ߣ������
//	  ʹ���㷨��DR = SR*SA/255+(1-SA/255)*DR
// 2��2004��02��11��V2.0,��д�ߣ������
//    �㷨��
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
	// ���ݷ�Χ���
	if(nDestX >= nDestBufferWidth || nDestY >= nDestBufferHeight)
		return TRUE;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return TRUE;
	if(nSrcX >= nSrcBufferWidth || nSrcY >= nSrcBufferHeight)
		return TRUE;

	// �õ���������Ҫ�����ĳ���
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return TRUE;
	// ÿ�μӵĿ��
	int nAddDestWidth = nDestBufferWidth<<2;	// ����2λ���൱�ڳ�4
	int nAddSrcWidth = nSrcBufferWidth<<2;

#if 1	
	// ����
	_declspec(align(16)) DWORD dwColorRepair[]={0x7f7f7f7f,};


	// һ���и�ֵ,SSE2�Ż�
	_asm
	{
		XOR EAX, EAX	// ��0
			MOVD		MM6,EAX
			MOVD		MM5,dwColorRepair
			PUNPCKLBW	MM5,MM6
LOOP1:
		CMP EAX, nDestHeight // ��һ��ѭ��(i<nDestHeight)
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(i>=nDestHeight)
			MOV	ECX, nDestY	 // ����nDestY+i
			ADD ECX, EAX
			CMP ECX, nDestBufferHeight
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(nDestY+i>=nDestBufferHeight)
			MOV	ECX, nSrcY	 // ����nSrcY+i
			ADD ECX, EAX
			CMP ECX, nSrcBufferHeight
			JGE	EXITLOOP1	 // �˳���һ��ѭ��(nSrcY+i>=nSrcBufferHeight)
			XOR ECX, ECX	 // ��0�� �ڶ���ѭ��������
			PUSH EAX
LOOP2:
		CMP ECX, nDestWidth  // �ڶ���ѭ��(j<nDestWidth)
			JGE LOOP2END	 // ����ڶ���ѭ����(j<nDestWidth)

			ADD ECX, 2			// ÿ�μ���2��ֵ������2ʱ����ѭ����2��
			CMP ECX, nDestWidth
			JG LOOP2BODY2		

			SUB ECX, 2

			MOV EDX, nSrcX	// ����nSrcX+j
			ADD EDX, ECX
			CMP EDX, nSrcBufferWidth
			JGE LOOP2END	// �˳��ڶ���ѭ��(nSrcX+j>=nSrcBufferWidth)

			MOV EDX, nDestX	// ����nDestX+j
			ADD EDX, ECX
			CMP EDX, nDestBufferWidth
			JGE LOOP2END	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)	

			MOV EAX, pSrc	// ��ȡpSrc[j]��ֵ����MM0
			MOV EDX, ECX
			SHL EDX, 2
			ADD EAX, EDX 
			MOVD MM0, [EAX]

			PUSH ECX
				MOV ECX, [EAX]					// ��ȡ��Alphaֵ
				SHR	ECX,  24					// ����24λ
					AND ECX, 0x000000FF
					MOVD MM3, ECX
					PUNPCKLWD MM3, MM3
					PUNPCKLDQ MM3, MM3				// ��Ϊ0A0A0A0A

					MOV EDX, 0xFF					// 255-Alpha
					SUB EDX, ECX
					MOVD MM7, EDX
					PUNPCKLWD MM7, MM7
					PUNPCKLDQ MM7, MM7				// ��Ϊ0A0A0A0A
					POP ECX

					ADD EAX, 4		// ��ȡpSrc[j+1]��ֵ����MM1
					MOVD MM1, [EAX]

					PUSH ECX
						MOV ECX, [EAX]					// ��ȡ��Alphaֵ
						SHR	ECX,  24					// ����24λ
							AND ECX, 0x000000FF
							MOVD MM4, ECX
							PUNPCKLWD MM4, MM4
							PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

							MOV EAX, 0xFF					// 255-Alpha
							SUB EAX, ECX
							MOVD MM6, EAX
							PUNPCKLWD MM6, MM6
							PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A
							POP ECX

							XOR EAX, EAX	// ��0
							MOVD MM2, EAX
							PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
							PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
							MOVQ2DQ XMM0, MM0
							MOVQ2DQ XMM1, MM1
							PUNPCKLQDQ XMM0, XMM1 // ��Ϊ0R0G0B0A0R0G0B0A

							MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
							MOV EDX, ECX
							SHL EDX, 2
							ADD EAX, EDX
							PUSH  EAX		 // ����ָ�룬����Ҫ��
							MOVD MM0, [EAX]
							ADD EAX, 4		// ��ȡpDes[j+1]��ֵ
								MOVD MM1, [EAX]
								XOR EAX, EAX	// ��0
									MOVD MM2, EAX
									PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A
									PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
									MOVQ2DQ XMM1, MM0
									MOVQ2DQ XMM2, MM1
									PUNPCKLQDQ XMM1, XMM2 // ��Ϊ0R0G0B0A0R0G0B0A
									MOVDQA	XMM3, XMM1	 // ����һ��

									MOVQ2DQ XMM2, MM3		// ȡԴ��alphaֵ
									MOVQ2DQ XMM4, MM4
									PUNPCKLQDQ XMM2, XMM4	// ��Ϊ0A0A0A0A0M0M0M0M

									MOVQ2DQ XMM4, MM7		// ȡ255-alphaֵ
									MOVQ2DQ XMM5, MM6
									PUNPCKLQDQ XMM4, XMM5	// ��Ϊ0A0A0A0A0M0M0M0M

									PMULLW XMM0, XMM2	// ��Ӧ16λ���SA
									// ����
									MOVQ2DQ		XMM7,MM5
									MOVLHPS		XMM7,XMM7
									PADDSW		XMM0,XMM7
									PSRLW  XMM0, 8		// ÿ16λ����8λ���൱��/255)(Ҫ��������)

									PMULLW XMM1, XMM4	// ��Ӧ16λ���D(1-A)
									// ����
									PADDSW XMM1,XMM7
									PSRLW  XMM1, 8

									PADDSW XMM0, XMM1	// ��Ӧ16λ���(SA+DA)

									PACKUSWB XMM0, XMM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

									MOVDQ2Q	MM0,XMM0	// ȡ��64λ(����DWORD)
									POP EAX					// ����pDest
									MOVQ  [EAX], MM0

									ADD ECX, 2			// j��2�����صڶ���ѭ��
									JMP LOOP2
LOOP2BODY2:
								SUB ECX, 2			// ����2ʱ
									CMP ECX, nDestWidth
									JGE LOOP2END

									MOV EDX, nSrcX	// ����nSrcX+j
									ADD EDX, ECX
									CMP EDX, nSrcBufferWidth
									JGE LOOP2END	// �˳��ڶ���ѭ��(nSrcX+j>=nSrcBufferWidth)

									MOV EDX, nDestX	// ����nDestX+j
									ADD EDX, ECX
									CMP EDX, nDestBufferWidth
									JGE LOOP2END	// �˳��ڶ���ѭ��(nDestX+j>=nDestBufferWidth)		

									MOV EAX, pSrc		// ��ȡpSrc[j]��ֵ
									MOV EDX, ECX
									SHL EDX, 2
									ADD EAX, EDX 
									MOVD MM0, [EAX]

									PUSH ECX
										MOV ECX, [EAX]					// ��ȡ��Alphaֵ
										SHR	ECX,  24					// ����24λ
											AND ECX, 0x000000FF
											MOVD MM4, ECX
											PUNPCKLWD MM4, MM4
											PUNPCKLDQ MM4, MM4				// ��Ϊ0A0A0A0A

											MOV EAX, 0xFF					// 255-Alpha
											SUB EAX, ECX
											MOVD MM6, EAX
											PUNPCKLWD MM6, MM6
											PUNPCKLDQ MM6, MM6				// ��Ϊ0A0A0A0A
											POP ECX

											XOR EAX, EAX	// ��0
											MOVD MM2, EAX
											PUNPCKLBW MM0, MM2	// ��MM0Ϊ0R0G0B0A

											MOV EAX, pDes	 // ��ȡpDes[j]��ֵ
											MOV EDX, ECX
											SHL EDX, 2
											ADD EAX, EDX
											PUSH  EAX		 // ����ָ�룬����Ҫ��
											MOVD MM1, [EAX]
											XOR EAX, EAX	// ��0
												MOVD MM2, EAX
												PUNPCKLBW MM1, MM2  // ��MM1Ϊ0R0G0B0A
												MOVQ	MM3, MM1	 // ����һ��

												PMULLW MM0, MM4	// ��Ӧ16λ���SA
												// ����
												PADDSW MM0, MM5
												PSRLW  MM0, 8		// ����8λ���൱��/255)(Ҫ��������)

												PMULLW MM1, MM6	// ��Ӧ16λ���D(1-A)
												// ����
												PADDSW MM1, MM5
												PSRLW  MM1, 8
												PADDSW MM0, MM1	// ��Ӧ16λ���
												PACKUSWB MM0, MM0 // ѹ����ÿ16λѹ��Ϊ8λ(RGBARGBA)

												POP EAX					// ����pDest
												MOVD  [EAX], MM0
LOOP2END:
											MOV EAX, pDes
												ADD EAX, nAddDestWidth		// nDestBufferWidth���4
												MOV [pDes], EAX

												MOV EAX, pSrc
												ADD EAX, nAddSrcWidth
												MOV [pSrc], EAX

												POP EAX
												INC	EAX			 // ����������������ѭ��1 
												JMP LOOP1
EXITLOOP1:
											EMMS			// ���
	}
#endif
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// �Ը�����Buffer�������Ų���
// ������
// ����ֵ��
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
			CVTPS2PI MM0, XMM0				// ȡfScaleHeight��fScaleWidth������nScaleHeight, nScaleWidth
			CVTPI2PS XMM1, MM0				// ����תΪFloat
			SUBPS XMM0, XMM1				// �����ȡ����С�����ִ���XMM0

			MOVAPS XMM2, [fTemp]
			MOVAPS XMM7, XMM0
				MOVAPS XMM1, XMM0
				SHUFPS XMM0, XMM0, 0			// �ĸ�fScaleWidth����XMM0
				SHUFPS XMM1, XMM1, 0x55			// �ĸ�fScaleHeight����XMM1

				SUBPS XMM2, XMM7				// �����1.0f - С��
				MOVAPS XMM3, XMM2
				SHUFPS XMM2, XMM2, 0			// �ĸ�1.0f-fScaleWidth����XMM2
				SHUFPS XMM3, XMM3, 0x55			// �ĸ�1.0f-fScaleHeight����XMM3

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
// �Ը�����Buffer�������Ų���
// ������
// ����ֵ��
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
	// ���ݷ�Χ���
	if(nSrcX>= nSrcBufferWidth || nSrcY>=nSrcBufferHeight)
		return E_FAIL;
	if(nDestX>=nDestBufferWidth || nDestY>=nDestBufferHeight)
		return E_FAIL;
	if(nDestBufferWidth <= 0||nDestBufferHeight <= 0 || nSrcBufferHeight<=0||nSrcBufferWidth<=0)
		return E_FAIL;
	if(nSrcWidth<=0 || nSrcHeight<=0 ||nDestWidth <=0 ||nDestHeight<=0)
		return E_FAIL;

	// Դ��������Ŀ�껺�����ɱ�������:���ű���
	float fScaleWidth = nSrcWidth/(float)nDestWidth;
	float fScaleHeight	 = nSrcHeight/(float)nDestHeight;

	// ����Ŀ���Դ��㣬Ŀ���Դ�Ŀ����߶�
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

	// ָ�����
	DWORD* pDes = pdwDestBuf + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuf + nSrcY*nSrcBufferWidth + nSrcX;
	DWORD* pTemp = pSrc;
	if(pSrc == NULL || pDes == NULL)
		return E_FAIL;
#if 0
	__asm
	{
		MOV ECX, nDestHeight
			NEG ECX					// ������ȡ��
			ADD ECX, 3				// һ��4��
			JNS DWORDZOOM_LOOPHEIGHT_MOD4			// ������Ǹ�������ת
DWORDZOOM_LOOPHEIGHT:

		ADD ECX, 4
			JS DWORDZOOM_LOOPHEIGHT		// ����Ǹ�������ת
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
					CVTDQ2PS XMM1, XMM1		// ת��Float��A, R, G, B
					MOVAPS XMM6, XMM0
					PUNPCKHDQ XMM6, XMM6
					PUNPCKHQDQ XMM6, XMM6
					MULPS XMM1, XMM6		// fSubModY*fSubModX*(A, R, G, B)

					MOVD XMM2, pixel1
					PUNPCKLBW XMM2, XMM7
					PUNPCKLWD XMM2, XMM7
					CVTDQ2PS XMM2, XMM2		// ת��Float��A, R, G, B
					MOVAPS XMM6, XMM0
					PUNPCKHQDQ XMM6, XMM6
					PUNPCKLDQ XMM6,  XMM6                 
					PUNPCKLQDQ XMM6, XMM6
					MULPS XMM2, XMM6		// fSubModY*fModX*(A, R, G, B)

					MOVD XMM3, pixel2
					PUNPCKLBW XMM3, XMM7
					PUNPCKLWD XMM3, XMM7
					CVTDQ2PS XMM3, XMM3		// ת��Float��A, R, G, B
					MOVAPS XMM6, XMM0
					PUNPCKLDQ XMM6, XMM6                  
					PUNPCKHQDQ XMM6, XMM6
					MULPS XMM3, XMM6		// fModY*fSubModX*(A, R, G, B)

					ADDPS XMM1, XMM2

					MOVD XMM4, pixel3
					PUNPCKLBW XMM4, XMM7
					PUNPCKLWD XMM4, XMM7
					CVTDQ2PS XMM4, XMM4		// ת��Float��A, R, G, B
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

// Դ����������
_declspec(align(16)) DWORD *g_pnSrcIndexTable = NULL;
// Ȩ�ر�
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
					mov edi, [edx]	// src��ɫ����edi��
					mov esi, ecx
						add esi, g_pnSrcWeightTable
						xor edx, edx
						mov dl, [esi]	// ȡ��Ȩ��ֵ����edx��
						cmp dl, 0
							jnz LOOPWIDTH_WEIGHT0  
							// Ȩ�� == 0 ʱ
							mov [eax], edi	// ֱ�Ӹ�ֵ
							inc ecx
							add eax, 4
							jmp LOOPWIDTH
LOOPWIDTH_WEIGHT0:
						// Ȩ�ط�0ʱ
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
							movd [eax], mm0	// ֱ�Ӹ�ֵ
							inc ecx
							add eax, 4
							jmp LOOPWIDTH

LOOPWIDTH_NORMAL:
						// g_pnSrcIndexTable[j] < nTemp
						shl edx, 2		// g_pnSrcIndexTable[j] * 4
							add edx, pSrcTemp
							mov edi, [edx]	// src��ɫ����edi��
							movd mm6, [edx+4] // src��һ����
							mov esi, ecx
								add esi, g_pnSrcWeightTable
								xor edx, edx
								mov dl, [esi]	// ȡ��Ȩ��ֵ����edx��
								cmp dl, 0 
									jnz LOOPWIDTH_NORMAL_WEIGHT0  
									// Ȩ�� == 0 ʱ
									mov [eax], edi	// ֱ�Ӹ�ֵ 
									inc ecx
									add eax, 4
									jmp LOOPWIDTH
LOOPWIDTH_NORMAL_WEIGHT0:
								// Ȩ�ط�0ʱ
								movq mm0, nMax
									movd mm1, edx
									punpcklwd mm1, mm1
									punpckldq mm1, mm1

									psubw mm0, mm1		// mm0 = 256 - g_pnSrcWeightTable[j]
									movd mm3, edi
									pxor mm2, mm2
									punpcklbw mm3, mm2	// 0A0R0G0B
									pmullw mm0, mm3
									punpcklbw mm6, mm2	// mm6 = ����һ����
									pmullw mm6, mm1
									paddusw mm0, mm6 
									psraw mm0, 8		// /256
									packsswb mm0, mm0
									movd [eax], mm0	// ֱ�Ӹ�ֵ

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
// YUYV���쿽��(���ڶ����YUV������YU0V)
LRESULT CALLBACK TP_StretchBlt_YUYV(
									IN OUT BYTE* pbyDestBuffer, 
									IN int nDestPitch,
									IN int nDestBufferWidth,
									IN int nDestBufferHeight,
									IN BYTE* pbySrcBuffer,
									IN int nSrcPitch,
									IN int nSrcBufferWidth,		// ���ص���
									IN int nSrcBufferHeight)
{

	if(nDestPitch == nDestBufferWidth*4)
	{
		NXBuf2DImg            pNXBufIn;       //����ʹ���µ�CPU SDK����
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
		NXBuf2DImg            pNXBufIn;       //����ʹ���µ�CPU SDK����
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
		NXBuf2DImg            pNXBufIn;       //����ʹ���µ�CPU SDK����
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
	// ��ʱ�洢Y��Y+1��YUVXYUVX��
	// ÿ���ֽ���
	int nSrcBytes = nSrcBufferWidth<<2; // *3
	BYTE* pbyTempSrc1 = (BYTE*)_mm_malloc(sizeof(BYTE)*nSrcBytes, 16);
	BYTE* pbyTempSrc2 =  (BYTE*)_mm_malloc(sizeof(BYTE)*nSrcBytes, 16);
	// ��ʱ�洢Ŀ��YUVYUV��
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
					add ecx, 7					// ÿ��8
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
						punpcklwd xmm0, xmm1	// ���ս��
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
							punpcklwd xmm7, xmm1	// ���ս��
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
								punpcklwd xmm0, xmm1	// ���ս��
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
									punpcklwd mm0, mm1	// ���ս��
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
					add ecx, 7					// ÿ��8
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
						punpcklwd xmm0, xmm1	// ���ս��
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
							punpcklwd xmm7, xmm1	// ���ս��
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
								punpcklwd xmm0, xmm1	// ���ս��
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
									punpcklwd mm0, mm1	// ���ս��
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
				PUNPCKLQDQ XMM1, XMM1			// �ĸ���ֵͬ:XMM0��4��fModY, XMM1��4��fSubModY
				pxor XMM7, XMM7

				mov edx, pbyTempDest
				xor eax, eax
blastloop:
			cmp eax, nDestBufferWidth
				jnl		skipblastloop			// ��ת

				movaps xmm2, xmm7				// fScaleWidth
				CVTPS2DQ XMM5, XMM2				// ȡfScaleWidth������nScaleWidth
				CVTDQ2PS XMM3, XMM5				// ����תΪFloat,����xmm3
				SUBPS XMM2, XMM3				// �����ȡ����С�����ִ���XMM2,fModx
				MOVAPS XMM4, [arfTemp2]
				SUBPS XMM4, XMM2				// �����1.0f - С��,����Xmm4, fSubModx

					movd ecx, xmm5
					cmp ecx, nSrcBufferWidth		// ���nX >= nSrcBufferWidth
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
							cvtdq2ps xmm5, xmm5			// ת��float
							cvtdq2ps xmm6, xmm6
							movaps xmm3, xmm4
							mulps xmm3, xmm1			// fSubModY*fSubModX
							mulps xmm3, xmm5			// fSubModY*fSubModX*p0
							mulps xmm4,	xmm0			// fModY*fSubModX
							mulps xmm4, xmm6			// fModY*fSubModX*p2
							addps xmm3, xmm4

							inc ecx
							cmp ecx, nSrcBufferWidth	// ���nX+1 >= nSrcBufferWidth
							jl blastloop2

							mov ecx, nSrcBufferWidth;
							dec ecx;
							//cvtps2dq xmm3, xmm3			// ת������
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
									cvtdq2ps xmm5, xmm5			// ת��float
									cvtdq2ps xmm6, xmm6
									movaps xmm4, xmm2
									mulps xmm2, xmm1			// fSubModY*fModX
									mulps xmm2, xmm5			// fSubModY*fModX*p1
									mulps xmm4,	xmm0			// fModY*fModX
									mulps xmm4, xmm6			// fModY*fModX*p3
									addps xmm2, xmm4
									addps xmm2, xmm3			//
									cvtps2dq xmm2,xmm2			// ת������
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

		// ���YUYV
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
	float	top,bottom,left,right;		// ��Ա���������mǰ׺

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

		rc.right = 576*4/3, rc.bottom = 576;	// Ҫ����4��3
		break;
	case TP_VIDEOSTANDARD_PAL_16_9:
	case TP_VIDEOSTANDARD_PAL_25P_16_9:
	case TP_VIDEOSTANDARD_PAL_50P_16_9:

		{
			rc.bottom = 576;
			rc.right = 576 * 16 / 9;			// Ҫ����16:9
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
		rc.right = 1920, rc.bottom = 1080;			// Ҫ����16:9
		break;
	case TP_VIDEOSTANDARD_1440_1080_50i:
	case TP_VIDEOSTANDARD_1440_1080_5994i:
	case TP_VIDEOSTANDARD_1440_1080_60i:
	case TP_VIDEOSTANDARD_1440_1080_50p:
	case TP_VIDEOSTANDARD_1440_1080_5994p:
	case TP_VIDEOSTANDARD_1440_1080_25p:
	case TP_VIDEOSTANDARD_1440_1080_2997p:

		rc.right = 1920, rc.bottom = 1080;			// Ҫ����16:9
		break;
	case TP_VIDEOSTANDARD_NTSC_2997_480:
	case TP_VIDEOSTANDARD_NTSC_30_480:
		rc.right = 720, rc.bottom = 480;
		break;
	case TP_VIDEOSTANDARD_UNKNOW:
	default:
		{
			rc = rcOri;	// Ҫ����4��3;
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

	// ����������
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

	// ��Դ�ŵ�����
	imageDst = imageSrc;
	imageDst.Offset( -fullSrc.Width()/2.f, -fullSrc.Height()/2.f );

	// ����
	imageDst.Scale(scaleX,scaleY);

	// ������任��Ŀ������ϵ��
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

		// ȡ��ʱ���߳ߴ�
		RECT rcTL;
		TP_ClipRectFromVideoStandard( adaptParam.eTLVideoStand, rcDestBuffer, rcTL );
		RECT rcTLChanged;
		TP_ClipRectFromVideoStandard_mov( adaptParam.eTLVideoStand, rcDestBuffer, rcTLChanged );

		// ȡ��buffer�ߴ�
		RECT rcSrcImage = adaptParam.rcSrc_buffer;
		if( adaptParam.lAdaptFlag & TP_ADAPTFLAG_1609 )			// 16:9Դ�����⴦��
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

		// ��bufferʼ�վ���ʱ�����м�
		OffsetRect(&rcSrcImage,-oriLeft,-oriTop);
		OffsetRect(&rcSrcImage,(int)((rcTLChanged.right-oriWidth)/2.f+0.5f),(int)((rcTLChanged.bottom-oriHeight)/2.f+0.5f));

		// 16��9ʱ���ߵı�������
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

		// Դȡȫ������Ŀ���м�
		rcSrc = adaptParam.rcSrc_buffer;
		rcDst = rcSrcImage;
		if ( pRcDstFull )
		{
			*pRcDstFull = rcTL;
		}

		return;
	}

	// ��NoChange����100%���Ŵ���
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


	// ԴҪ������ߴ�����������
	RECT rcClipChanged;
	TP_ClipRectFromVideoStandard_Adj( adaptParam.eClipVideoStand, rcClip, rcClipChanged );   

	// ȡ���زĳߴ��ʱ���߳ߴ�
	TP_ClipRectFromVideoStandard( adaptParam.eTLVideoStand, rcDestBuffer, rcTL );

	// ȡ��ʱ���߼���ߴ�
	RECT rcTLChanged;
	if ( adaptParam.eClipVideoStand == TP_VIDEOSTANDARD_UNKNOW )
		TP_ClipRectFromVideoStandard_mov( adaptParam.eTLVideoStand, rcDestBuffer, rcTLChanged );
	else
		TP_ClipRectFromVideoStandard_Adj( adaptParam.eTLVideoStand, rcDestBuffer, rcTLChanged );

	// ���䣬�õ���Ҫ���ز�λ�ú�Ŀ��RECT�����Ǽ���ߴ磩
	RECT rcClipImage, rcTLImage;
	rcClipImage = rcClipChanged; 
	rcTLImage = rcTL;
	float fOffsetX = ((float)adaptParam.dCenterX - 0.5f) * RECTWIDTH(rcTLChanged);
	float fOffsetY = ((float)adaptParam.dCenterY - 0.5f) * RECTHEIGHT(rcTLChanged);
	TP_RectAdapt_Impl( rcClipChanged, rcTLChanged, rcClipImage, rcTLImage, eMode, lSpecify, fOffsetX, fOffsetY );

	// IMX50�زģ�512 -> crop 480 ( 512-32 ) -> add black 486 ( 480 + 6 ), ��Ҫȷ�ϣ�Դbuffer���������һ���׶ε����ݡ�
	// todo
	// XDCAM�ز�, proxy��
	// ����352*240(16:9),��ʵ��360*243(��Ӧ��ͨ����µ�480*270)��һ����;Ҳ����˵������16:9�����һ���֡�
	// ����352*240(4:3)����ʵ��360*243��һ����(��Ӧ��ͨ����µ�360*243);

	// ���ݼ���ߴ緵�صĽ������ö�Ӧ��ԭʼ�ز�RECT��ʱ����RECT
	RECT rcClipImageOri, rcTLImageOri;

	// XDCAM�ز�, proxy��
	if ( rcClip.right == 352 && rcClip.bottom == 240 )
	{
		RECT rcClipDummyOri = { 0, 0, 360, 243 }; 
		RECT rcClipDummy = rcClipDummyOri;
		CropRectAsModel( rcClipDummy, rcClipImage, rcClipChanged );		// �����ز�Ӧ�ý�ȡ�Ĵ�С
		rcTLImageOri = rcTL;
		CropRectAsModel( rcTLImageOri, rcTLImage, rcTLChanged );		// ����ʱ����Ӧ�ý�ȡ�Ĵ�С

		RECT rcClipCroped = { 4, 2, 356, 242 };
		IntersectRect( &rcClipCroped, &rcClipDummy, &rcClipCroped );	// �����ز�Ӧ�ý�ȡ�Ĵ�С

		CropRectAsModel( rcTLImageOri ,rcClipCroped, rcClipDummy );		// ����ʱ���߽�ȡ��С

		rcClipImageOri = rcClipCroped;									// Դ�زĶ�Ӧ������
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

// ���ɽ�����ɫ���͸��ɫ����
void TP_MakeTranslucentBack(OUT DWORD* pBuf,			// ���� Buffer
							IN int nBufWidth,			// Buffer ��С
							IN int nBufHeight,
							IN int nBlockSize,			// �����ɫ���С
							IN DWORD crOddBlock,		// ��������ɫ����ɫ
							IN DWORD crEvenBlock)		// ż������ɫ����ɫ
{
	if (pBuf == NULL)
		return;

	int nBlockCountH = nBufWidth / nBlockSize + 2;	// ȷ��һ�������Ҫ����ɫ�����
	int nBlockCountV = nBufHeight / nBlockSize;		// ȷ��һ��ӵ�е�������ɫ�����
	int nLineSize = nBlockCountH * nBlockSize;
	int na16 = (nBlockSize >> 2), nm16 = ((nBlockSize & 0x03) << 2);
	DWORD* pBlockLine = TPSSE_NEW(nLineSize, DWORD);
	DWORD* pTmpLine = pBlockLine;

	// �������ͼƬ
	for (int i=0; i<nBlockCountH; i++)
	{
		memset_dword_line(pTmpLine, (i & 0x01) ? crEvenBlock : crOddBlock, na16, nm16);
		pTmpLine += nBlockSize;
	}
	// ������䱳��
	na16 = (nBufWidth >> 2), nm16 = ((nBufWidth & 0x03) << 2);
	for (int i=0; i<nBlockCountV; i++)	// �������������ɫ��
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
	for (int k=0; k<nBufHeight; k++)	// ������һ������������ɫ��
	{
		memcpy_line(pBuf, pTmpLine, na16, nm16);
		pBuf += nBufWidth;
	}
	TPSSE_FREE(pBlockLine);
}

_declspec(align(16)) DWORD g_dwComp_5[] = { 0x00100010, 0x00100010, 0x00100010, 0x00100010 };
_declspec(align(16)) DWORD g_dwMaskWLB[] = { 0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff };

void TPSSE_Blur3Line(OUT DWORD* pDest,          // Ŀ��
                     IN DWORD* pSrcPrev,        // ��һ��
                     IN DWORD* pSrcCurr,        // ��ǰ��
                     IN DWORD* pSrcNext,        // ��һ��
                     IN DWORD dwWeightPrev,     // ����Ȩ�أ����Ŵ��� 2^14 �����ܺ�Ϊ 1��
                     IN DWORD dwWeightCurr,     // Ҫ��� 16 λ��� 16 λ��ͬ��Ҳ���洢��������ͬ��Ȩ��
                     IN DWORD dwWeightNext,
                     IN int na8,                // һ�е��ֽ��������� 8 ���̺�����
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
        psllw       xmm1, 0x07;         // �Ŵ� 7 λ
        psllw       xmm2, 0x07;
        psllw       xmm3, 0x07;
        pmulhw      xmm1, xmm5;         // ���Ŵ� 5 λ
        pmulhw      xmm2, xmm6;
        pmulhw      xmm3, xmm7;
        paddw       xmm1, xmm2;
        paddw       xmm3, xmm4;
        paddw       xmm1, xmm3;
        movdqa      xmm4, g_dwMaskWLB;
        psrlw       xmm1, 0x05;         // �ָ�
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
        psllw       xmm1, 0x07;         // �Ŵ� 7 λ
        psllw       xmm2, 0x07;
        psllw       xmm3, 0x07;
        pmulhw      xmm1, xmm5;         // ���Ŵ� 5 λ
        pmulhw      xmm2, xmm6;
        pmulhw      xmm3, xmm7;
        paddw       xmm1, xmm2;
        paddw       xmm3, xmm4;
        paddw       xmm1, xmm3;
        movdqa      xmm4, g_dwMaskWLB;
        psrlw       xmm1, 0x05;         // �ָ�
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

    // ������������
    DWORD* pSrcPreLine  = pSrcBuffer + nWidth;
    DWORD* pSrcCurLine  = pSrcBuffer;
    DWORD* pSrcNextLine = pSrcBuffer + nWidth;
    DWORD* pDesCurLine  = pNewBuffer;
    TPSSE_Blur3Line(pDesCurLine, pSrcPreLine, pSrcCurLine, pSrcNextLine, 
        dwWeight[0], dwWeight[1], dwWeight[2], na8, nm8);

    // �����м���
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

    // ����ĩ������
    pSrcNextLine = pSrcPreLine;
    TPSSE_Blur3Line(pDesCurLine, pSrcPreLine, pSrcCurLine, pSrcNextLine, 
        dwWeight[0], dwWeight[1], dwWeight[2], na8, nm8);

    memcpy_s(pSrcBuffer,sizeof(DWORD)*nWidth*nHeight, pNewBuffer, sizeof(DWORD)*nWidth*nHeight);

    _mm_free(pNewBuffer);
    pNewBuffer = NULL;

    return pSrcBuffer;
}

