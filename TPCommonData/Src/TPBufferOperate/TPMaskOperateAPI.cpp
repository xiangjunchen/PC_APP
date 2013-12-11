/********************************************************************
	Copyright (c) 2004, Sobey���� ����������
	All rights reserved.
	����ʱ��:	2004��8��3��   10:10
	�ļ���: 	g:\e7\chenyufeng_desktop_cg\desktopsobey\tpcommondata\src\tpbufferoperate\tpmaskoperateapi.cpp
	��ǰ�汾��	1.0
	����:		�����
	ժҪ:		��ģ����������
*********************************************************************/
#include "stdAfx.h"
#include "TPMaskOperateAPI.h"
#include "TPBufferOperateAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// DA = DA-SA
LRESULT CALLBACK TP_SubMask(IN OUT BYTE* pbyDestMask,
							IN int nDestX,
							IN int nDestY,
							IN int nDestMaskWidth,
							IN int nDestMaskHeight,
							IN BYTE* pbySrcMask,
							IN int nSrcX,
							IN int nSrcY,
							IN int nSrcMaskWidth,
							IN int nSrcMaskHeight,
							IN int nOperateWidth,
							IN int nOperateHeight,
							IN int nType)			// ��������
{
	// ���ݷ�Χ���
	if(nDestX >= nDestMaskWidth || nDestY >= nDestMaskHeight)
		return E_FAIL;
	if(nDestMaskWidth <= 0||nDestMaskHeight <= 0 || nSrcMaskHeight<=0||nSrcMaskWidth<=0)
		return E_FAIL;
	if(nSrcX >= nSrcMaskWidth || nSrcY >= nSrcMaskHeight)
		return E_FAIL;

	// �õ���������Ҫ�����ĳ���
	if(nDestX < 0)
	{
		nOperateWidth += nDestX;
		nSrcX -= nDestX;
		nDestX = 0;
	}
	if(nDestY < 0)
	{
		nOperateHeight += nDestY;
		nSrcY -= nDestY;
		nDestY = 0;
	}

	if(nSrcX < 0)
	{
		nOperateWidth += nSrcX;
		nDestX -= nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nOperateHeight += nSrcY;
		nDestY -= nSrcY;
		nSrcY = 0;
	}
	int nTempWidth = nSrcX+nOperateWidth;
	int nTempHeight = nSrcY+nOperateHeight;
	if(nTempWidth > nSrcMaskWidth)
		nOperateWidth -= (nTempWidth-nSrcMaskWidth);
	if(nTempHeight > nSrcMaskHeight)
		nOperateHeight -= (nTempHeight-nSrcMaskHeight);
	nTempWidth = nDestX+nOperateWidth;
	nTempHeight = nDestY+nOperateHeight;
	if(nTempWidth > nDestMaskWidth)
		nOperateWidth -= (nTempWidth-nDestMaskWidth);
	if(nTempHeight > nDestMaskHeight)
		nOperateHeight -= (nTempHeight-nDestMaskHeight);
	if(nOperateWidth <= 0||nOperateHeight<= 0)
		return E_FAIL;

	// �õ�Ŀ�껺������ʼ���Դ��������ʼ��
	BYTE* pDes = pbyDestMask + nDestY*nDestMaskWidth + nDestX;
	BYTE* pSrc = pbySrcMask + nSrcY*nSrcMaskWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return E_FAIL;

#if 1
	// һ���и�ֵ,SSE2�Ż�
	_declspec(align(16)) DWORD dwMask1[]={0x00FF00FF,0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	_declspec(align(16)) DWORD dwMask2[]={0x000000FF,0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX	// i��0
LOOP1:
		CMP ECX, nOperateHeight // ��һ��ѭ��(i<nDestHeight)
		JGE	EXITLOOP1	 // �˳���һ��ѭ��(i>=nDestHeight)
		PUSH ECX
		XOR ECX, ECX	 // j��0�� �ڶ���ѭ��������
		MOV EAX, pSrc	 // Դ��Ŀ�껺����ָ�� 
		MOV EDX, pDes
LOOP2:
		ADD ECX, 16			// ÿ�μ���16��ֵ������16ʱ����ѭ����2��
		CMP ECX, nOperateWidth
		JG LOOP2BODY2		

		MOVDQU XMM0, [EDX]	// ȡDest��16��Byteֵ��128λ
		MOVDQU XMM1, [EAX]	// ȡSrc��16��Byteֵ
		PMAXUB XMM0, XMM1	// ȡ�������ֵ
		PSUBB  XMM0, XMM1	// ���ֵ��SRC����֤�������
		MOVDQU [EDX], XMM0

		ADD EAX, 16			// ָ�����
		ADD EDX, 16
		JMP LOOP2
LOOP2BODY2:
		SUB ECX, 16			// ����16ʱ
LOOP2BODY3:
		ADD ECX, 4			// ÿ�μ���4��ֵ������4ʱ����ѭ����4��
		CMP ECX, nOperateWidth
		JG LOOP2BODY4	

		MOVD XMM0, [EDX]	// ȡDest��4��Byteֵ��32λ
		MOVD XMM1, [EAX]	// ȡSrc��4��Byteֵ��32λ
		PMAXUB XMM0, XMM1
		PSUBB  XMM0, XMM1	// ���ֵ��SRC����֤�������
		MOVD  [EDX], XMM0	// ���������Dest

		ADD EAX, 4
		ADD EDX, 4
		JMP LOOP2BODY3
LOOP2BODY4:
		SUB ECX, 4			// ����4ʱ
LOOP2BODY5:
		ADD ECX, 2			// ÿ�μ���2��ֵ�� ����2ʱ����ѭ����6��
		CMP ECX, nOperateWidth
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

		PMAXUB XMM0, XMM1
		PSUBB  XMM0, XMM1	// ���ֵ��SRC����֤�������

		PUSH EAX
		MOVD EAX, XMM0
		MOV  [EDX], AX		// ���������Dest
		POP EAX

		ADD EAX, 2
		ADD EDX, 2
		JMP LOOP2BODY5
LOOP2BODY6:
		SUB ECX, 2
		CMP ECX, nOperateWidth
		JGE LOOP2END

		XOR ECX, ECX
		MOV CL, [EAX]	// SA����ECX
		XOR EAX, EAX
		MOV AL, [EDX]	// DA����EAX				
		SUB AL, CL		// DA-SA
		JNC ZERO
		MOV AL, 0
ZERO:
		MOV BYTE PTR [EDX], AL
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
	for(int i=0; i<nOperateHeight; i++)
	{
		for(int j=0; j<nOperateWidth; j++)
		{
			int nMax = max(pSrc[j], pDes[j]);
			pDes[j] = nMax - pSrc[j];		// ��֤���SA��ʱΪ0���������
		}
		pDes += nDestMaskWidth;
		pSrc += nSrcMaskWidth;
	}
#endif
	return S_OK;
}