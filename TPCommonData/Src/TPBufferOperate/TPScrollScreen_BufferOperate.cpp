/********************************************************************
	Copyright (c) 2004, Sobey���� ����������
	All rights reserved.
	����ʱ��:	2004��3��4��   15:18
	�ļ���: 	c:\ndsrc\chenyufeng_desktop_3\devvob\sobeydesktop\src\tpcommon\tpbufferoperate\tpscrollscreen_bufferoperate.cpp
	��ǰ�汾��	1.0
	����:		�����
	ժҪ:		����������������ʵ��
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
// ȫ�ֱ���


//////////////////////////////////////////////////////////////////////////
// ˽�к�������
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
							   int nParity,				 // ��ż��ģʽ
							   int nScrollType,			 // ��������
							   float fPower,			 // ������ռȨ��
							   int nFadeIn,				 // �����
							   int nFadeOut);			 // ������
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
								  int nParity,				 // ��ż��ģʽ
								  int nScrollType,			 // ��������
								  float fPower,			 // ������ռȨ��
								  int nFadeIn,				 // �����
								  int nFadeOut);			 // ������


//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////
// ���ܣ���������ʱ��BitBlt����
// ������
// ����ֵ���Ƿ�ɹ�
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TP_SCROLLSCREEN_BitBlt(
										IN OUT DWORD* pdwDestBuffer, // Ŀ��buffer
										IN int nDestX,				 // Ҫ������Ŀ���������
										IN int nDestY,
										IN int nDestWidth,			 // Ҫ������Ŀ�����
										IN int nDestHeight,
										IN int nDestBufferWidth,	 // Ŀ��buffer���� 
										IN int nDestBufferHeight,
										IN DWORD* pdwSrcBuffer,		 // ԴBuffer
										IN int nSrcX,				 // Ҫ������Դ�������				
										IN int nSrcY,
										IN int nSrcWidth,			 // Ҫ������Դ����
										IN int nSrcHeight,
										IN int nSrcBufferWidth,		 // Ҫ������ԴBuffer����
										IN int nSrcBufferHeight,
										IN int nMix,				 // ���ģʽ
										IN int nParity,				 // ��ż��ģʽ
										IN int nScrollType,		     // ��������
										IN float fPower,			 // ������ռȨ��
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
// ˽�к���ʵ��
// ��������
//////////////////////////////////////////////////////////////////////////
// ���ܣ���������ʽ��ͬ����ԴBuffer��Ŀ��Bufferָ��λ��
// �㷨���Ϲ�ʱCA = CA*fPower+DA*(1-fPower),����CAΪ��ǰ����Alphaֵ��DAΪ��һ�����ص�Alphaֵ��������һ��Ϊ�գ���DAΪSRC��������һ������Alphaֵ
//		 ���뵭����CA = CA*(j*255/(nFadeOut-1))/255;����jΪ��ǰ��yֵ
// ������
// ����ֵ���Ƿ񿽱��ɹ�
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_ScrollScreen_Copy(DWORD* pdwDestBuffer, // Ŀ��Buffer
								int nDestX,						  // Ҫ������Ŀ�����
								int nDestY,
								int nDestWidth,					  // Ҫ������Ŀ�����
								int nDestHeight,
								int nDestBufferWidth,			  // Ŀ��Buffer����
								int nDestBufferHeight,
								DWORD* pdwSrcBuffer,				  // ԴBuffer
								int nSrcX,						  // Ҫ������Դ���
								int nSrcY,
								int nSrcWidth,					  // Ҫ������Դ����
								int nSrcHeight,
								int nSrcBufferWidth,				  // ԴBuffer����
								int nSrcBufferHeight,
								int nParity,				 // ��ż��ģʽ
								int nScrollType,			 // ��������
								float fPower,			 // ������ռȨ��
								int nFadeIn,				 // �����
								int nFadeOut)			 // ������
{
	switch(nScrollType) 
	{
	case TP_SCROLLSCREEN_UPMOVE:
		return TPBuffer_ScrollScreen_Up(pdwDestBuffer, // Ŀ��Buffer
			nDestX,						  // Ҫ������Ŀ�����
			nDestY,
			nDestWidth,					  // Ҫ������Ŀ�����
			nDestHeight,
			nDestBufferWidth,			  // Ŀ��Buffer����
			nDestBufferHeight,
			pdwSrcBuffer,				  // ԴBuffer
			nSrcX,						  // Ҫ������Դ���
			nSrcY,
			nSrcWidth,					  // Ҫ������Դ����
			nSrcHeight,
			nSrcBufferWidth,				  // ԴBuffer����
			nSrcBufferHeight,
			nParity,				 // ��ż��ģʽ
			fPower,			 // ������ռȨ��
			nFadeIn,				 // �����
			nFadeOut);
	case TP_SCROLLSCREEN_DOWNMOVE:
		break;
	case TP_SCROLLSCREEN_LEFTMOVE:
		return TPBuffer_ScrollScreen_Left(pdwDestBuffer, // Ŀ��Buffer
			nDestX,						  // Ҫ������Ŀ�����
			nDestY,
			nDestWidth,					  // Ҫ������Ŀ�����
			nDestHeight,
			nDestBufferWidth,			  // Ŀ��Buffer����
			nDestBufferHeight,
			pdwSrcBuffer,				  // ԴBuffer
			nSrcX,						  // Ҫ������Դ���
			nSrcY,
			nSrcWidth,					  // Ҫ������Դ����
			nSrcHeight,
			nSrcBufferWidth,				  // ԴBuffer����
			nSrcBufferHeight,
			nParity,				 // ��ż��ģʽ
			fPower,			 // ������ռȨ��
			nFadeIn,				 // �����
			nFadeOut);
	case TP_SCROLLSCREEN_RIGHTMOVE:
		break;
	default:
		return FALSE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
// ���ܣ�������ż��MixԴBuffer��Ŀ��Buffer
// �㷨���Ϲ�ʱCA = CA*fPower+DA*(1-fPower),����CAΪ��ǰ����Alphaֵ��DAΪ��һ�����ص�Alphaֵ��������һ��Ϊ�գ���DAΪSRC��������һ������Alphaֵ
//		 Mix�㷨��KA=SA+DA-SA*DA/255; DR = DR+(SR-DR)*SA/KA
// ����ʱ�䣺2004��03��02��δʵ��Դ��Ŀ���Mix����
// ����ֵ���Ƿ�ɹ�
//////////////////////////////////////////////////////////////////////////
BOOL TPBuffer_ScrollScreen_Mix(DWORD* pdwDestBuffer, 
							   int nDestX,						  // Ҫ������Ŀ�����
							   int nDestY,
							   int nDestWidth,					  // Ҫ������Ŀ�����
							   int nDestHeight,
							   int nDestBufferWidth,			  // Ŀ��Buffer����
							   int nDestBufferHeight,
							   DWORD* pdwSrcBuffer,				  // ԴBuffer
							   int nSrcX,						  // Ҫ������Դ���
							   int nSrcY,
							   int nSrcWidth,					  // Ҫ������Դ����
							   int nSrcHeight,
							   int nSrcBufferWidth,				  // ԴBuffer����
							   int nSrcBufferHeight,
							   int nParity,				 // ��ż��ģʽ
							   int nScrollType,			 // ��������
							   float fPower,			 // ������ռȨ��
							   int nFadeIn,				 // �����
							   int nFadeOut)			 // ������
{
	if(pdwDestBuffer == NULL||pdwSrcBuffer == NULL)
		return TRUE;

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
	// �Ϲ�ʱ����Ȳ����ͱ��ص�,�߶Ȳ���ʱ���ͷ��ʼȡ
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// ÿ�μӵĿ��
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// ������������
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// һ��һ��
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// һ������
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
			if(nSrcY >= nSrcBufferHeight)
			{
				nSrcY = 0;
				pSrc = pdwSrcBuffer + nSrcX;
			}
			else
				pSrc += nSrcBufferWidth;			// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
			nDestHeight--;
			if(nDestY >= nDestBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;				// Ŀ���Ƶ���һ��
		}
	}
	else
	{
		// ���Ϊż��
		if(nParity == TP_PARITY_ODD)
		{
			// ����泡�����������һ�п�ʼ
			nDestHeight--;
			if(nSrcY >= nSrcBufferHeight)
			{
				nSrcY = 0;
				pSrc = pdwSrcBuffer + nSrcX;
			}
			else
				pSrc += nSrcBufferWidth;		// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
			if(nDestY >= nDestBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;			// Ŀ���Ƶ���һ��
		}
	}

	// Դ���������һ�е���һ�е�ָ��
	DWORD* pdwLast = pdwSrcBuffer + nSrcBufferWidth*nSrcBufferHeight;
	DWORD* pdwFirst = pdwSrcBuffer + nSrcX;								// ����ָ��
	WORD nPower = (WORD)(fPower*255);
	WORD nSubPower = (WORD)(255-nPower);
	int nSrcLineBytes = nSrcBufferWidth<<2;						// һ�е��ֽ���
	_declspec(align(16)) WORD narPower[] = {nPower, nPower, nPower, nPower, nPower, nPower, nPower, nPower};
	_declspec(align(16)) WORD narSubPower[] = {nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower};
	_declspec(align(16)) DWORD dwFade[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	WORD* pdFadeOut = new WORD[nFadeOut];			// ����������
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
	WORD* pdFadeIn = new WORD[nFadeIn];		// ���뻺����
	int nFadeInStart = nDestHeight - nFadeIn;	// ��������ʼ��
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
			MOV EAX, pSrc			// Դָ��
			MOV EDX, pDes
LOOPY:
		CMP ECX, nDestHeight	// y<nDestHeight
			JGE LOOPYEND
			CMP EAX, pdwLast		// pSrc<pdwLast
			JL  LOOPYSRC
			MOV EAX, pdwFirst		// ���pSrc>=pdwLast,��pSrc=pdwFirst
LOOPYSRC:
		MOV ESI, EAX			// ��һ��ָ��
			ADD ESI, nSrcLineBytes
			CMP ESI, pdwLast
			JL LOOPYNEXT
			MOV ESI, pdwFirst
LOOPYNEXT:
		PUSH EDX
			PUSH EAX
			MOVDQA XMM4, dwFade		// �ҵ����뵭��ֵ:Ĭ��Ϊ0F0F0F0F0F0F0F0F
			CMP ECX, nFadeOut
			JGE  FADEOUT
			MOV EDI, ECX					// �����ǰ����ֵ
			SHL EDI, 1
			ADD EDI, pdFadeOut
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
			JMP FADEIN
FADEOUT:
		CMP ECX, nFadeInStart			// �뵭������ʼ��Ƚ�
			JLE  FADEIN
			MOV EDI, nDestHeight					// �����ǰ����ֵ
			SUB EDI, ECX
			SHL EDI, 1
			ADD EDI, pdFadeIn
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
FADEIN:
		PUSH ECX
			XOR ECX, ECX
LOOPX:
		ADD ECX, 2				// ÿ�μ���2�����ص�
			CMP ECX, nDestWidth		// x<nDestWidth
			JG  LOOPX1

			MOVQ MM0, [EAX]			// �õ�Դ������
			MOVQ2DQ XMM0, MM0
				PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

				MOVQ MM1, [ESI]			// �õ���һ������
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					ADD EAX, 8				// ָ�����
					PMULLW XMM1, XMM6		// pSrcNext*nPower
					ADD ESI, 8
					PADDUSW XMM0, XMM1		// ���
					PSRLW XMM0, 8			// ����8λ
					PMULLW XMM0, XMM4		// ����nFadeOut
					PSRLW XMM0, 8			// ����8λ
					PACKUSWB XMM0, XMM0		// ѹ��
					MOVDQ2Q MM0, XMM0
					MOVNTQ [EDX], MM0
					ADD EDX, 8

					JMP LOOPX
LOOPX1:
				SUB ECX, 2
					CMP ECX, nDestWidth
					JGE LOOPXEND

					MOVD XMM0, [EAX]			// �õ�Դ������
					PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// �õ���һ������
						PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, 4				// ָ�����
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, 4
							PADDUSW XMM0, XMM1		// ���
							PSRLW XMM0, 8			// ����8λ
							PMULLW XMM0, XMM4		// ����nFadeOut
							PSRLW XMM0, 8			// ����8λ
							PACKUSWB XMM0, XMM0		// ѹ��
							MOVD [EDX], XMM0			// ��ֵ
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
// NotMix����
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
								  int nParity,				 // ��ż��ģʽ
								  int nScrollType,			 // ��������
								  float fPower,			 // ������ռȨ��
								  int nFadeIn,				 // �����
								  int nFadeOut)			 // ������
{
	if(pdwDestBuffer == NULL||pdwSrcBuffer == NULL)
		return TRUE;

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
	// �Ϲ�ʱ����Ȳ����ͱ��ص�,�߶Ȳ���ʱ���ͷ��ʼȡ
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// ÿ�μӵĿ��
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// ������������
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// һ��һ��
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// һ������
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
			if(nSrcY >= nSrcBufferHeight)
			{
				nSrcY = 0;
				pSrc = pdwSrcBuffer + nSrcX;
			}
			else
				pSrc += nSrcBufferWidth;			// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
			nDestHeight--;
			if(nDestY >= nDestBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;				// Ŀ���Ƶ���һ��
		}
	}
	else
	{
		// ���Ϊż��
		if(nParity == TP_PARITY_ODD)
		{
			// ����泡�����������һ�п�ʼ
			nDestHeight--;
			if(nSrcY >= nSrcBufferHeight)
			{
				nSrcY = 0;
				pSrc = pdwSrcBuffer + nSrcX;
			}
			else
				pSrc += nSrcBufferWidth;		// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
			if(nDestY >= nDestBufferHeight)
				return TRUE;
			pDes += nDestBufferWidth;			// Ŀ���Ƶ���һ��
		}
	}

	// Դ���������һ�е���һ�е�ָ��
	DWORD* pdwLast = pdwSrcBuffer + nSrcBufferWidth*nSrcBufferHeight;
	DWORD* pdwFirst = pdwSrcBuffer + nSrcX;								// ����ָ��
	WORD nPower = (WORD)(fPower*255);
	WORD nSubPower = (WORD)(255-nPower);
	int nSrcLineBytes = nSrcBufferWidth<<2;						// һ�е��ֽ���
	_declspec(align(16)) WORD narPower[] = {nPower, nPower, nPower, nPower, nPower, nPower, nPower, nPower};
	_declspec(align(16)) WORD narSubPower[] = {nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower};
	_declspec(align(16)) DWORD dwFade[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	WORD* pdFadeOut = new WORD[nFadeOut];			// ����������
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
	WORD* pdFadeIn = new WORD[nFadeIn];		// ���뻺����
	int nFadeInStart = nDestHeight - nFadeIn;	// ��������ʼ��
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
			MOV EAX, pSrc			// Դָ��
			MOV EDX, pDes
LOOPY:
		CMP ECX, nDestHeight	// y<nDestHeight
			JGE LOOPYEND
			CMP EAX, pdwLast		// pSrc<pdwLast
			JL  LOOPYSRC
			MOV EAX, pdwFirst		// ���pSrc>=pdwLast,��pSrc=pdwFirst
LOOPYSRC:
		MOV ESI, EAX			// ��һ��ָ��
			ADD ESI, nSrcLineBytes
			CMP ESI, pdwLast
			JL LOOPYNEXT
			MOV ESI, pdwFirst
LOOPYNEXT:
		PUSH EDX
			PUSH EAX
			MOVDQA XMM4, dwFade		// �ҵ����뵭��ֵ:Ĭ��Ϊ0F0F0F0F0F0F0F0F
			CMP ECX, nFadeOut
			JGE  FADEOUT
			MOV EDI, ECX					// �����ǰ����ֵ
			SHL EDI, 1
			ADD EDI, pdFadeOut
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
			JMP FADEIN
FADEOUT:
		CMP ECX, nFadeInStart			// �뵭������ʼ��Ƚ�
			JLE  FADEIN
			MOV EDI, nDestHeight					// �����ǰ����ֵ
			SUB EDI, ECX
			SHL EDI, 1
			ADD EDI, pdFadeIn
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
FADEIN:
		PUSH ECX
			XOR ECX, ECX
LOOPX:
		ADD ECX, 2				// ÿ�μ���2�����ص�
			CMP ECX, nDestWidth		// x<nDestWidth
			JG  LOOPX1

			MOVQ MM0, [EAX]			// �õ�Դ������
			MOVQ2DQ XMM0, MM0
				PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

				MOVQ MM1, [ESI]			// �õ���һ������
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					ADD EAX, 8				// ָ�����
					PMULLW XMM1, XMM6		// pSrcNext*nPower
					ADD ESI, 8
					PADDUSW XMM0, XMM1		// ���
					PSRLW XMM0, 8			// ����8λ
					PMULLW XMM0, XMM4		// ����nFadeOut
					PSRLW XMM0, 8			// ����8λ
					PACKUSWB XMM0, XMM0		// ѹ��
					MOVDQ2Q MM0, XMM0
					MOVQ [EDX], MM0
					ADD EDX, 8

					JMP LOOPX
LOOPX1:
				SUB ECX, 2
					CMP ECX, nDestWidth
					JGE LOOPXEND

					MOVD XMM0, [EAX]			// �õ�Դ������
					PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// �õ���һ������
						PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, 4				// ָ�����
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, 4
							PADDUSW XMM0, XMM1		// ���
							PSRLW XMM0, 8			// ����8λ
							PMULLW XMM0, XMM4		// ����nFadeOut
							PSRLW XMM0, 8			// ����8λ
							PACKUSWB XMM0, XMM0		// ѹ��
							MOVD [EDX], XMM0			// ��ֵ
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
// ���ܣ������Ϲ�
// �㷨��pCur*(1-fSubPower) + pNextCur*fPower 
//		 ��Ȳ����ͱ��ص�,�߶Ȳ���ʱ���ͷ��ʼȡ
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
							  int nParity,				// ��ż��ģʽ
							  float fPower,			 // ������ռȨ��
							  int nFadeIn,
							  int nFadeOut)
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

	// �Ϲ�ʱ����Ȳ����ͱ��ص�,�߶Ȳ���ʱ���ͷ��ʼȡ
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// ÿ�μӵĿ��
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// ������������
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// һ��һ��
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// һ������
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nParity == TP_PARITY_EVEN && nDestY%2 == 0)
	{
		// ���ż�����������Ϊ�泡�������һ�п�ʼ
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;			// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
		nDestHeight--;
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;				// Ŀ���Ƶ���һ��
	}
	if(nParity == TP_PARITY_ODD && nDestY%2 != 0)
	{
		// ����泡���������Ϊż���������һ�п�ʼ
		nDestHeight--;
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;		// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;			// Ŀ���Ƶ���һ��
	}

	// Դ���������һ�е���һ�е�ָ��
	DWORD* pdwLast = pdwSrcBuffer + nSrcBufferWidth*nSrcBufferHeight;
	DWORD* pdwFirst = pdwSrcBuffer + nSrcX;								// ����ָ��
	WORD nPower = (WORD)(fPower*255);
	WORD nSubPower = (WORD)(255-nPower);
	_declspec(align(16)) WORD narPower[] = {nPower, nPower, nPower, nPower, nPower, nPower, nPower, nPower};
	_declspec(align(16)) WORD narSubPower[] = {nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower, nSubPower};
	_declspec(align(16)) DWORD dwFade[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	_declspec(align(16)) DWORD dwRepair[] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	WORD* pdFadeOut = NULL;
	WORD* pdFadeIn = NULL;
	int nFadeInStart = nDestHeight - nFadeIn;	// ��������ʼ��
	if(nFadeOut > 0)
	{
		pdFadeOut = new WORD[nFadeOut];			// ����������
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
		pdFadeIn = new WORD[nFadeIn];		// ���뻺����
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
	// ����㷨
	__asm
	{
		XOR ECX, ECX
			PXOR XMM7, XMM7
			MOVDQA XMM6, narPower
			MOVDQA XMM5, narSubPower
			MOVDQA XMM3, dwRepair
			MOV EAX, pSrc			// Դָ��
			MOV EDX, pDes
LOOPY:
		CMP ECX, nDestHeight	// y<nDestHeight
			JGE LOOPYEND
			CMP EAX, pdwLast		// pSrc<pdwLast
			JL  LOOPYSRC
			MOV EAX, pdwFirst		// ���pSrc>=pdwLast,��pSrc=pdwFirst
LOOPYSRC:
		MOV ESI, EAX			// ��һ��ָ��, pNext>=pdwLast,��pNext=pdwFirst
			ADD ESI, nAddSrcWidth		//nAddSrcWidth
			CMP ESI, pdwLast
			JL LOOPYNEXT
			MOV ESI, pdwFirst
LOOPYNEXT:
		PUSH EDX
			PUSH EAX
			MOVDQA XMM4, dwFade		// �ҵ����뵭��ֵ:Ĭ��Ϊ0F0F0F0F0F0F0F0F
			CMP ECX, nFadeOut
			JGE  FADEOUT
			MOV EDI, ECX					// �����ǰ����ֵ
			SHL EDI, 1
			ADD EDI, pdFadeOut
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
			JMP FADEIN
FADEOUT:
		CMP ECX, nFadeInStart			// �뵭������ʼ��Ƚ�
			JL  FADEIN
			MOV EDI, nDestHeight				// �����ǰ����ֵ
			DEC EDI
			SUB EDI, ECX
			SHL EDI, 1
			ADD EDI, pdFadeIn
			PINSRW XMM4, [EDI], 3
			PINSRW XMM4, [EDI], 7
FADEIN:
			XOR EDI, EDI
LOOPX:
		ADD EDI, 2				// ÿ�μ���2�����ص�
			CMP EDI, nDestWidth		// x<nDestWidth
			JG  LOOPX1

			MOVQ MM0, [EAX]			// �õ�Դ������
			MOVQ2DQ XMM0, MM0
				PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

				MOVQ MM1, [ESI]			// �õ���һ������
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					ADD EAX, 8				// ָ�����
					PMULLW XMM1, XMM6		// pSrcNext*nPower
					ADD ESI, 8
					PADDUSW XMM0, XMM1		// ���
					PADDUSW XMM0, XMM3		// ����
					PSRLW XMM0, 8			// ����8λ
					PMULLW XMM0, XMM4		// ����nFadeOut
					PADDUSW XMM0, XMM3		// ����
					PSRLW XMM0, 8			// ����8λ
					PACKUSWB XMM0, XMM0		// ѹ��
					MOVDQ2Q MM0, XMM0
					MOVQ [EDX], MM0
					ADD EDX, 8

					JMP LOOPX
LOOPX1:
				SUB EDI, 2
					CMP EDI, nDestWidth
					JGE LOOPXEND

					MOVD XMM0, [EAX]			// �õ�Դ������
					PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// �õ���һ������
						PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, 4				// ָ�����
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, 4
							PADDUSW XMM0, XMM1		// ���
							PADDUSW XMM0, XMM3		// ����
							PSRLW XMM0, 8			// ����8λ
							PMULLW XMM0, XMM4		// ����nFadeOut
							PADDUSW XMM0, XMM3		// ����
							PSRLW XMM0, 8			// ����8λ
							PACKUSWB XMM0, XMM0		// ѹ��
							MOVD [EDX], XMM0			// ��ֵ
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
	MOV EAX, pSrc			// Դָ��
	MOV EDX, pDes
LOOPY_1:
	CMP ECX, nDestHeight	// y<nDestHeight
	JGE LOOPYEND_1
	CMP EAX, pdwLast		// pSrc<pdwLast
	JL  LOOPYSRC_1
	MOV EAX, pdwFirst		// ���pSrc>=pdwLast,��pSrc=pdwFirst
LOOPYSRC_1:
	MOV ESI, EAX			// ��һ��ָ��, pNext>=pdwLast,��pNext=pdwFirst
	ADD ESI, nAddSrcWidth		//nAddSrcWidth
	CMP ESI, pdwLast
	JL LOOPYNEXT_1
	MOV ESI, pdwFirst
LOOPYNEXT_1:
	PUSH EDX
	PUSH EAX
	XOR EDI, EDI
LOOPX_1:
	ADD EDI, 2				// ÿ�μ���2�����ص�
		CMP EDI, nDestWidth		// x<nDestWidth
		JG  LOOPX1_1

		MOVQ MM0, [EAX]			// �õ�Դ������
		MOVQ2DQ XMM0, MM0
			PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

			MOVQ MM1, [ESI]			// �õ���һ������
			MOVQ2DQ XMM1, MM1
				PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
				PMULLW XMM0, XMM5		// pSrc*nSubPower

				ADD EAX, 8				// ָ�����
				PMULLW XMM1, XMM6		// pSrcNext*nPower
				ADD ESI, 8
				PADDUSW XMM0, XMM1		// ���
				PADDUSW XMM0, XMM3		// ����
				PSRLW XMM0, 8			// ����8λ
				PACKUSWB XMM0, XMM0		// ѹ��
				MOVDQ2Q MM0, XMM0
				MOVQ [EDX], MM0
				ADD EDX, 8

				JMP LOOPX_1
LOOPX1_1:
			SUB EDI, 2
				CMP EDI, nDestWidth
				JGE LOOPXEND_1

				MOVD XMM0, [EAX]			// �õ�Դ������
				PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

					MOVD XMM1, [ESI]			// �õ���һ������
					PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
						PMULLW XMM0, XMM5		// pSrc*nSubPower

						ADD EAX, 4				// ָ�����
						PMULLW XMM1, XMM6		// pSrcNext*nPower
						ADD ESI, 4
						PADDUSW XMM0, XMM1		// ���
						PADDUSW XMM0, XMM3		// ����
						PSRLW XMM0, 8			// ����8λ
						PACKUSWB XMM0, XMM0		// ѹ��
						MOVD [EDX], XMM0			// ��ֵ
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
// �������
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
								int nParity,				// ��ż��ģʽ
								float fPower,			 // ������ռȨ��
								int nFadeIn,
								int nFadeOut)
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

	// ���ʱ���߶Ȳ����ͱ��ص�,��Ȳ���ʱ��ӵ�һ�п�ʼȡ
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// ÿ�μӵĿ��(�ֽ�����
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// ������������
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// һ��һ��
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// һ������
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nParity == TP_PARITY_EVEN && nDestY % 2 == 0)
	{
		// ���ż�����������Ϊ�泡�������һ�п�ʼ
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;			// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
		nDestHeight--;
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;				// Ŀ���Ƶ���һ��
	}

	// ���Ϊż��
	if(nParity == TP_PARITY_ODD && nDestY%2)
	{
		// ����泡���������Ϊż���������һ�п�ʼ
		nDestHeight--;
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;		// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;			// Ŀ���Ƶ���һ��
	}

	WORD nPower = (WORD)(fPower*255);			// Ȩ��
	WORD nSubPower = (WORD)(255-nPower);
	WORD* pdFadeOut = NULL;
	WORD* pdFadeIn = NULL;
	int nFadeInStart = nDestWidth - nFadeIn;	// ��������ʼ��
	if(nFadeOut > 0)
	{
		pdFadeOut = new WORD[nFadeOut];			// ����������
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
		pdFadeIn = new WORD[nFadeIn];		// ���뻺����
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
				MOV EAX, pSrc			// Դָ��
				MOV EDX, pDes			// Ŀ��ָ��
LOOPX:
			CMP ECX, nDestWidth		// x<nDestWidth
				JGE LOOPXEND
				CMP ECX, nEnd		// x<nEnd
				JL  LOOPXSRC
				//	MOV EAX, ECX		// ���x>=nEnd,��pCur=pSrc + (i-nSrcBufferWidth)
				//	SUB EAX, nSrcBufferWidth
				//	SHL EAX, 2
				//	ADD EAX, pSrc
				MOV EAX, ECX		// ���x>=nEnd,��pCur = pdwSrcBuffer + (i-nEnd)
				SUB EAX, nEnd
				SHL EAX, 2
				ADD EAX, pdwTempSrc
LOOPXSRC:
			MOV ESI, EAX			// Դ����һ��ָ��:pSrc++
				ADD ESI, 4
				CMP ECX, nSubEnd		// x<nEnd
				JL LOOPXNEXT
				//MOV ESI, ECX		// ���x>=nEnd,��pNext=pSrc + (i+1-nSrcBufferWidth)
				//INC ESI
				//SUB ESI, nSrcBufferWidth
				//SHL ESI, 2
				//ADD ESI, pSrc
				MOV ESI, ECX		// ���x>=nEnd,��pCur = pdwSrcBuffer + (i+1-nEnd)
				SUB ESI, nSubEnd
				SHL ESI, 2
				ADD ESI, pdwTempSrc
LOOPXNEXT:
			MOVDQA XMM4, dwFade		// �ҵ����뵭��ֵ:Ĭ��Ϊ0F0F0F0F0F0F0F0F
				CMP ECX, nFadeOut
				JGE  FADEOUT			// x>=nFadeOutʱ������
				MOV EDI, ECX			// �����ǰ����ֵ
				SHL EDI, 1
				ADD EDI, pdFadeOut
				PINSRW XMM4, [EDI], 3
				PINSRW XMM4, [EDI], 7
				JMP FADEIN				// �е���ʱ���õ���
FADEOUT:
			CMP ECX, nFadeInStart	// �뵭������ʼ��Ƚ�
				JL  FADEIN				// x<nFadeInStart��������
				MOV EDI, nDestWidth		// �����ǰ����ֵ
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
			ADD EDI, 2				// ÿ�μ���2�����ص�
				CMP EDI, nDestHeight	// y<nDestHeight
				JG  LOOPY1

				MOVD XMM0, [EAX]			// �õ�Դ������
				ADD EAX, nAddSrcWidth
					MOVD XMM1, [EAX]
					PUNPCKLDQ XMM0, XMM1
						PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// �õ���һ������
						ADD ESI, nAddSrcWidth
							MOVD XMM2, [ESI]
							PUNPCKLDQ XMM1, XMM2
								PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
								PMULLW XMM0, XMM5		// pSrc*nSubPower

								ADD EAX, nAddSrcWidth	// ָ�����
								PMULLW XMM1, XMM6		// pSrcNext*nPower
								ADD ESI, nAddSrcWidth
								PADDUSW XMM0, XMM1		// ���
								PADDUSW XMM0, XMM3		// ����
								PSRLW XMM0, 8			// ����8λ
								PMULLW XMM0, XMM4		// ����nFadeOut��nFadeIn
								PADDUSW XMM0, XMM3		// ����
								PSRLW XMM0, 8			// ����8λ
								PACKUSWB XMM0, XMM0		// ѹ��

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

								MOVD XMM0, [EAX]			// �õ�Դ������
								PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

									MOVD XMM1, [ESI]			// �õ���һ������
									PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
										PMULLW XMM0, XMM5		// pSrc*nSubPower

										ADD EAX, nAddSrcWidth	// ָ�����
										PMULLW XMM1, XMM6		// pSrcNext*nPower
										ADD ESI, nAddSrcWidth
										PADDUSW XMM0, XMM1		// ���
										PADDUSW XMM0, XMM3		// ����
										PSRLW XMM0, 8			// ����8λ
										PMULLW XMM0, XMM4		// ����nFadeOut
										PADDUSW XMM0, XMM3		// ����
										PSRLW XMM0, 8			// ����8λ
										PACKUSWB XMM0, XMM0		// ѹ��
										MOVD [EDX], XMM0		// ��ֵ
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
		MOV EAX, pSrc			// Դָ��
		MOV EDX, pDes			// Ŀ��ָ��
LOOPX_1:
		CMP ECX, nDestWidth		// x<nDestWidth
		JGE LOOPXEND_1
		CMP ECX, nEnd		// x<nEnd
		JL  LOOPXSRC_1
			//	MOV EAX, ECX		// ���x>=nEnd,��pCur=pSrc + (i-nSrcBufferWidth)
			//	SUB EAX, nSrcBufferWidth
			//	SHL EAX, 2
			//	ADD EAX, pSrc
			MOV EAX, ECX		// ���x>=nEnd,��pCur = pdwSrcBuffer + (i-nEnd)
			SUB EAX, nEnd
			SHL EAX, 2
			ADD EAX, pdwTempSrc
LOOPXSRC_1:
		MOV ESI, EAX			// Դ����һ��ָ��:pSrc++
			ADD ESI, 4
			CMP ECX, nSubEnd		// x<nEnd
			JL LOOPXNEXT_1
			//MOV ESI, ECX		// ���x>=nEnd,��pNext=pSrc + (i+1-nSrcBufferWidth)
			//INC ESI
			//SUB ESI, nSrcBufferWidth
			//SHL ESI, 2
			//ADD ESI, pSrc
			MOV ESI, ECX		// ���x>=nEnd,��pCur = pdwSrcBuffer + (i+1-nEnd)
			SUB ESI, nSubEnd
			SHL ESI, 2
			ADD ESI, pdwTempSrc
LOOPXNEXT_1:
		PUSH EDX
		PUSH EAX
		XOR EDI, EDI
LOOPY_1:
		ADD EDI, 2				// ÿ�μ���2�����ص�
			CMP EDI, nDestHeight	// y<nDestHeight
			JG  LOOPY1_1

			MOVD XMM0, [EAX]			// �õ�Դ������
			ADD EAX, nAddSrcWidth
				MOVD XMM1, [EAX]
				PUNPCKLDQ XMM0, XMM1
					PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

					MOVD XMM1, [ESI]			// �õ���һ������
					ADD ESI, nAddSrcWidth
						MOVD XMM2, [ESI]
						PUNPCKLDQ XMM1, XMM2
							PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, nAddSrcWidth	// ָ�����
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, nAddSrcWidth
							PADDUSW XMM0, XMM1		// ���
							PADDUSW XMM0, XMM3		// ����
							PSRLW XMM0, 8			// ����8λ
							PACKUSWB XMM0, XMM0		// ѹ��

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

							MOVD XMM0, [EAX]			// �õ�Դ������
							PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

								MOVD XMM1, [ESI]			// �õ���һ������
								PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
									PMULLW XMM0, XMM5		// pSrc*nSubPower

									ADD EAX, nAddSrcWidth	// ָ�����
									PMULLW XMM1, XMM6		// pSrcNext*nPower
									ADD ESI, nAddSrcWidth
									PADDUSW XMM0, XMM1		// ���
									PADDUSW XMM0, XMM3		// ����
									PSRLW XMM0, 8			// ����8λ
									PACKUSWB XMM0, XMM0		// ѹ��
									MOVD [EDX], XMM0		// ��ֵ
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
// ���ܣ������¹�:��ǰ������һ�л��
// ������
// ����ֵ��
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
								int nParity,				// ��ż��ģʽ
								float fPower,			 // ������ռȨ��
								int nFadeIn,
								int nFadeOut)
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

	// �¹�ʱ����Ȳ����ͱ��ص�,�߶Ȳ���ʱ���ͷ��ʼȡ
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// ÿ�μӵĿ�ȣ��ֽ�����
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// ������������
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// һ��һ��
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// һ������
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nParity == TP_PARITY_EVEN && nDestY % 2 == 0)
	{
		// ���ż�����������Ϊ�泡�������һ�п�ʼ
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;			// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
		nDestHeight--;
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;				// Ŀ���Ƶ���һ��
	}
	if(nParity == TP_PARITY_ODD && nDestY%2)
	{
		// ����泡���������Ϊż���������һ�п�ʼ
		nDestHeight--;
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;		// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;			// Ŀ���Ƶ���һ��
	}
	
	// ȫ�����ÿ������������ϲ���
	DWORD* pdwLast = pdwSrcBuffer + nSrcX;					// Դ����ָ�룬��С�ڴ�ָ��ʱ��ת��Դĩ��
	DWORD* pdwFirst = pdwSrcBuffer + (nSrcBufferHeight-1)*nSrcBufferWidth + nSrcX;	// Դ��ĩ����ָ��
	pDes += (nDestHeight-1)*nDestBufferWidth;				// Ҫ������Ŀ��ĩ��ָ��
	WORD nPower = (WORD)(fPower*255);
	WORD nSubPower = (WORD)(255-nPower);
	DWORD* pNext = pSrc - nSrcBufferWidth;		// ��һ��
	WORD* pdFadeOut = NULL;
	WORD* pdFadeIn = NULL;
	int nFadeInStart = nDestHeight - nFadeIn;		// ��������ʼ��
	if(nFadeOut > 0)
	{
		pdFadeOut = new WORD[nFadeOut];			// ����������
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
		pdFadeIn = new WORD[nFadeIn];		// ���뻺����
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
	// ���ʵ��
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
		JGE LOOPYSRC				// ��� pSrc<pdwLast, ��pSrc = pdwFirst
		MOV EAX, pdwFirst
LOOPYSRC:
		MOV ESI, EAX				// ��һ��ָ��:��ǰ����������һ�����ػ��
		SUB ESI, nAddSrcWidth
		CMP ESI, pdwLast			
		JGE LOOPYNEXT				// ��� pNext<pdwLast, ��pNext = pdwFirst
		MOV ESI, pdwFirst
LOOPYNEXT:
		MOVDQA XMM4, dwFade			// �ҵ����뵭��ֵ:Ĭ��Ϊ0F0F0F0F0F0F0F0F
		CMP ECX, nFadeOut
		JGE  FADEOUT
		MOV EDI, ECX				// �����ǰ����ֵ
		SHL EDI, 1
		ADD EDI, pdFadeOut
		PINSRW XMM4, [EDI], 3
		PINSRW XMM4, [EDI], 7
		JMP FADEIN
FADEOUT:
		CMP ECX, nFadeInStart			// �뵭������ʼ��Ƚ�
		JL  FADEIN
		MOV EDI, nDestHeight				// �����ǰ����ֵ
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
		
		MOVQ MM0, [EAX]			// �õ�Դ������
		MOVQ2DQ XMM0, MM0
		PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

		MOVQ MM1, [ESI]			// �õ���һ������
		MOVQ2DQ XMM1, MM1
		PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
		PMULLW XMM0, XMM5		// pSrc*nSubPower

		ADD EAX, 8				// ָ�����
		PMULLW XMM1, XMM6		// pSrcNext*nPower
		ADD ESI, 8
		PADDUSW XMM0, XMM1		// ���
		PADDUSW XMM0, XMM3		// ����(����00FF)
		PSRLW XMM0, 8			// ����8λ
		PMULLW XMM0, XMM4		// ����nFadeOut
		PADDUSW XMM0, XMM3		// ����
		PSRLW XMM0, 8			// ����8λ
		PACKUSWB XMM0, XMM0		// ѹ��
		MOVDQ2Q MM0, XMM0
		MOVQ [EDX], MM0
		ADD EDX, 8

		JMP LOOPX
LOOPX1:
		SUB EDI, 2
		CMP EDI, nDestWidth
		JGE LOOPXEND

		MOVD XMM0, [EAX]			// �õ�Դ������
		PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

		MOVD XMM1, [ESI]			// �õ���һ������
		PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
		PMULLW XMM0, XMM5		// pSrc*nSubPower

		ADD EAX, 4				// ָ�����
		PMULLW XMM1, XMM6		// pSrcNext*nPower
		ADD ESI, 4
		PADDUSW XMM0, XMM1		// ���
		PADDUSW XMM0, XMM3		// ����
		PSRLW XMM0, 8			// ����8λ
		PMULLW XMM0, XMM4		// ����nFadeOut
		PADDUSW XMM0, XMM3		// ����
		PSRLW XMM0, 8			// ����8λ
		PACKUSWB XMM0, XMM0		// ѹ��
		MOVD [EDX], XMM0			// ��ֵ
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
			JGE LOOPYSRC_1				// ��� pSrc<pdwLast, ��pSrc = pdwFirst
			MOV EAX, pdwFirst
LOOPYSRC_1:
		MOV ESI, EAX				// ��һ��ָ��:��ǰ����������һ�����ػ��
			SUB ESI, nAddSrcWidth
			CMP ESI, pdwLast			
			JGE LOOPYNEXT_1				// ��� pNext<pdwLast, ��pNext = pdwFirst
			MOV ESI, pdwFirst
LOOPYNEXT_1:
		PUSH EAX
			PUSH EDX
			XOR EDI, EDI
LOOPX_1:
		ADD EDI, 2
			CMP EDI, nDestWidth
			JG  LOOPX1_1

			MOVQ MM0, [EAX]			// �õ�Դ������
			MOVQ2DQ XMM0, MM0
				PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

				MOVQ MM1, [ESI]			// �õ���һ������
				MOVQ2DQ XMM1, MM1
					PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					ADD EAX, 8				// ָ�����
					PMULLW XMM1, XMM6		// pSrcNext*nPower
					ADD ESI, 8
					PADDUSW XMM0, XMM1		// ���
					PADDUSW XMM0, XMM3		// ����(����00FF)
					PSRLW XMM0, 8			// ����8λ
					PACKUSWB XMM0, XMM0		// ѹ��
					MOVDQ2Q MM0, XMM0
					MOVQ [EDX], MM0
					ADD EDX, 8

					JMP LOOPX_1
LOOPX1_1:
				SUB EDI, 2
					CMP EDI, nDestWidth
					JGE LOOPXEND_1

					MOVD XMM0, [EAX]			// �õ�Դ������
					PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

						MOVD XMM1, [ESI]			// �õ���һ������
						PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
							PMULLW XMM0, XMM5		// pSrc*nSubPower

							ADD EAX, 4				// ָ�����
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, 4
							PADDUSW XMM0, XMM1		// ���
							PADDUSW XMM0, XMM3		// ����
							PSRLW XMM0, 8			// ����8λ
							PACKUSWB XMM0, XMM0		// ѹ��
							MOVD [EDX], XMM0			// ��ֵ
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
// �����ҹ�
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
								int nParity,				// ��ż��ģʽ
								float fPower,			 // ������ռȨ��
								int nFadeIn,
								int nFadeOut)
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

	// �ҹ�ʱ���߶Ȳ����ͱ��ص�,��Ȳ���ʱ��ӵ�һ�п�ʼȡ
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

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	DWORD* pDes = pdwDestBuffer + nDestY*nDestBufferWidth + nDestX;
	DWORD* pSrc = pdwSrcBuffer + nSrcY*nSrcBufferWidth + nSrcX;

	// ÿ�μӵĿ��(�ֽ�����
	int nAddDestWidth = 0;
	int nAddSrcWidth = 0;
	int nInc = 0;		// ������������
	switch(nParity) {
	case TP_PARITY_BOTH:
		nAddDestWidth = nDestBufferWidth<<2;	// һ��һ��
		nAddSrcWidth = nSrcBufferWidth<<2;
		nInc = 1;
		break;
	case TP_PARITY_ODD:
	case TP_PARITY_EVEN:
		nAddDestWidth = nDestBufferWidth<<3;	// һ������
		nAddSrcWidth = nSrcBufferWidth<<3;
		nInc = 2;
		break;
	default:
		return FALSE;
	}

	if(nParity == TP_PARITY_EVEN && nDestY % 2 == 0)
	{
		// ���ż�����������Ϊ�泡�������һ�п�ʼ
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;			// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
		nDestHeight--;
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;				// Ŀ���Ƶ���һ��
	}
	if(nParity == TP_PARITY_ODD && nDestY%2)
	{
		// ����泡���������Ϊż���������һ�п�ʼ
		nDestHeight--;
		if(nSrcY >= nSrcBufferHeight)
		{
			nSrcY = 0;
			pSrc = pdwSrcBuffer + nSrcX;
		}
		else
			pSrc += nSrcBufferWidth;		// Դ�Ƶ���һ�У������һ��Ϊ�գ����Ƶ�Դ�ĵ�һ��
		if(nDestY >= nDestBufferHeight)
			return TRUE;
		pDes += nDestBufferWidth;			// Ŀ���Ƶ���һ��
	}

	WORD nPower = (WORD)(fPower*255);			// Ȩ��
	WORD nSubPower = (WORD)(255-nPower);
	WORD* pdFadeOut = NULL;
	WORD* pdFadeIn = NULL;
	int nFadeInStart = nDestWidth - nFadeIn;	// ��������ʼ��
	if(nFadeOut > 0)
	{
		pdFadeOut = new WORD[nFadeOut];			// ����������
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
		pdFadeIn = new WORD[nFadeIn];		// ���뻺����
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
//	DWORD* pNext = pSrc - 1;	// ��һ��
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
	//	pDes--;		// Ŀ��ָ��
	//	pCur--;		// ��ǰ��ָ��
	//	pNext--;	// ��һ��ָ��
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
		MOV EAX, pSrc			// Դ��ǰָ��
		MOV EDX, pDes			// Ŀ��ָ��
LOOPX:
		CMP ECX, nDestWidth		// x<nDestWidth
		JGE LOOPXEND
		CMP ECX, nEnd		// x<=nEnd
		JLE  LOOPXSRC
		MOV EAX, nSrcBufferWidth	// ���x>nEnd,��pCur=pSrc + (nSrcBufferWidth-i)
		SUB EAX, ECX
		ADD EAX, nSrcX
		SHL EAX, 2
		ADD EAX, pTempSrc
LOOPXSRC:
		MOV ESI, EAX			// ��һ��ָ��
		SUB ESI, 4
		CMP ECX, nEnd		// x<nNextEnd
		JL LOOPXNEXT
		MOV ESI, nSrcBufferWidth		// ���x>nNextEnd,��pNext=pSrc + (nSrcBufferWidth - i -1)
		DEC ESI
		SUB ESI, ECX
		ADD ESI, nSrcX
		SHL ESI, 2
		ADD ESI, pTempSrc
LOOPXNEXT:
		MOVDQA XMM4, dwFade		// �ҵ����뵭��ֵ:Ĭ��Ϊ0F0F0F0F0F0F0F0F
		CMP ECX, nFadeOut
		JGE  FADEOUT			// x>=nFadeOutʱ������
		MOV EDI, ECX			// �����ǰ����ֵ
		SHL EDI, 1
		ADD EDI, pdFadeOut
		PINSRW XMM4, [EDI], 3
		PINSRW XMM4, [EDI], 7
		JMP FADEIN				// �е���ʱ���õ���
FADEOUT:
		CMP ECX, nFadeInStart	// �뵭������ʼ��Ƚ�
		JL  FADEIN				// x<nFadeInStart��������
		MOV EDI, nDestWidth		// �����ǰ����ֵ
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
		ADD EDI, 2				// ÿ�μ���2�����ص�
		CMP EDI, nDestHeight	// y<nDestHeight
		JG  LOOPY1

		MOVD XMM0, [EAX]			// �õ�Դ������
		ADD EAX, nAddSrcWidth
		MOVD XMM1, [EAX]
		PUNPCKLDQ XMM0, XMM1
		PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A
		PMULLW XMM0, XMM5		// pSrc*nSubPower

		MOVD XMM1, [ESI]			// �õ���һ������
		ADD ESI, nAddSrcWidth
		MOVD XMM2, [ESI]
		PUNPCKLDQ XMM1, XMM2
		PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A

		ADD EAX, nAddSrcWidth	// ָ�����
		PMULLW XMM1, XMM6		// pSrcNext*nPower
		ADD ESI, nAddSrcWidth
		PADDUSW XMM0, XMM1		// ���
		PADDUSW XMM0, XMM3		// ����
		PSRLW XMM0, 8			// ����8λ
		PMULLW XMM0, XMM4		// ����nFadeOut��nFadeIn
		PADDUSW XMM0, XMM3		// ����
		PSRLW XMM0, 8			// ����8λ
		PACKUSWB XMM0, XMM0		// ѹ��

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

		MOVD XMM0, [EAX]			// �õ�Դ������
		PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

		MOVD XMM1, [ESI]			// �õ���һ������
		PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
		PMULLW XMM0, XMM5		// pSrc*nSubPower

		ADD EAX, nAddSrcWidth	// ָ�����
		PMULLW XMM1, XMM6		// pSrcNext*nPower
		ADD ESI, nAddSrcWidth
		PADDUSW XMM0, XMM1		// ���
		PADDUSW XMM0, XMM3		// ����
		PSRLW XMM0, 8			// ����8λ
		PMULLW XMM0, XMM4		// ����nFadeOut
		PADDUSW XMM0, XMM3		// ����
		PSRLW XMM0, 8			// ����8λ
		PACKUSWB XMM0, XMM0		// ѹ��
		MOVD [EDX], XMM0		// ��ֵ
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
			MOV EAX, pSrc			// Դ��ǰָ��
			MOV EDX, pDes			// Ŀ��ָ��
LOOPX_1:
		CMP ECX, nDestWidth		// x<nDestWidth
			JGE LOOPXEND_1
			CMP ECX, nEnd		// x<=nEnd
			JLE  LOOPXSRC_1
			MOV EAX, nSrcBufferWidth	// ���x>nEnd,��pCur=pSrc + (nSrcBufferWidth-i)
			SUB EAX, ECX
			ADD EAX, nSrcX
			SHL EAX, 2
			ADD EAX, pTempSrc
LOOPXSRC_1:
		MOV ESI, EAX			// ��һ��ָ��
			SUB ESI, 4
			CMP ECX, nEnd		// x<nNextEnd
			JL LOOPXNEXT_1
			MOV ESI, nSrcBufferWidth		// ���x>nNextEnd,��pNext=pSrc + (nSrcBufferWidth - i -1)
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
		ADD EDI, 2				// ÿ�μ���2�����ص�
			CMP EDI, nDestHeight	// y<nDestHeight
			JG  LOOPY1_1

			MOVD XMM0, [EAX]			// �õ�Դ������
			ADD EAX, nAddSrcWidth
				MOVD XMM1, [EAX]
				PUNPCKLDQ XMM0, XMM1
					PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A
					PMULLW XMM0, XMM5		// pSrc*nSubPower

					MOVD XMM1, [ESI]			// �õ���һ������
					ADD ESI, nAddSrcWidth
						MOVD XMM2, [ESI]
						PUNPCKLDQ XMM1, XMM2
							PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A

							ADD EAX, nAddSrcWidth	// ָ�����
							PMULLW XMM1, XMM6		// pSrcNext*nPower
							ADD ESI, nAddSrcWidth
							PADDUSW XMM0, XMM1		// ���
							PADDUSW XMM0, XMM3		// ����
							PSRLW XMM0, 8			// ����8λ
							PACKUSWB XMM0, XMM0		// ѹ��

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

							MOVD XMM0, [EAX]			// �õ�Դ������
							PUNPCKLBW XMM0, XMM7	// ���0R0G0B0A0R0G0B0A

								MOVD XMM1, [ESI]			// �õ���һ������
								PUNPCKLBW XMM1, XMM7	// ���0R0G0B0A0R0G0B0A
									PMULLW XMM0, XMM5		// pSrc*nSubPower

									ADD EAX, nAddSrcWidth	// ָ�����
									PMULLW XMM1, XMM6		// pSrcNext*nPower
									ADD ESI, nAddSrcWidth
									PADDUSW XMM0, XMM1		// ���
									PADDUSW XMM0, XMM3		// ����
									PSRLW XMM0, 8			// ����8λ
									PACKUSWB XMM0, XMM0		// ѹ��
									MOVD [EDX], XMM0		// ��ֵ
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