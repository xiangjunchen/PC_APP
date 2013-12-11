/********************************************************************
	Copyright (c) 2004, Sobey���� ����������
	All rights reserved.
	����ʱ��:	2004��8��3��   10:10
	�ļ���: 	g:\e7\chenyufeng_desktop_cg\desktopsobey\tpcommondata\src\tpbufferoperate\tpmaskoperateapi.cpp
	��ǰ�汾��	1.0
	����:		�����
	ժҪ:		�ڴ滺��������
*********************************************************************/
#include "stdAfx.h"
#include "TPMemoryOperateAPI.h"
#include "TPBufferOperateAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// ���ܣ������ڴ�Ϊָ����ֵ
// ������
// ����ֵ��
//////////////////////////////////////////////////////////////////////////
void* CALLBACK TP_MemSet(IN OUT void* pDest,
						IN int nChar,
						IN int nSize);

//////////////////////////////////////////////////////////////////////////
// ���ܣ��ڴ濽��(һ�ο��Կ���16���ֽڣ�����)
// ������
// ����ֵ��
//////////////////////////////////////////////////////////////////////////
void* CALLBACK TP_Memcpy(IN OUT void* pDest,
			IN const void* pSrc,
			IN int nSize)
{
	if(nSize==0||pSrc==NULL || pDest==NULL)
		return NULL;
	void *pTemp = pDest;

	__asm
	{
		MOV	EDX, pSrc
		MOV	EBX, pTemp
		Mov     EAX, nSize

		neg		eax			// EAXȡ�෴����CF��ʶ���Ƿ�Ϊ0
		add		eax, 63			
		jbe		skipblastloop		// CF��ZFΪ1ʱ��ת
blastloop:
		movq	mm0, [edx]			// һ��8��8��64���ֽ�
		movq	mm1, [edx+8]
		movq	mm2, [edx+16]
		movq	mm3, [edx+24]
		movq	mm4, [edx+32]
		movq	mm5, [edx+40]
		movq	mm6, [edx+48]
		movq	mm7, [edx+56]
		movntq	[ebx], mm0
		movntq	[ebx+8], mm1
		movntq	[ebx+16], mm2
		movntq	[ebx+24], mm3
		movntq	[ebx+32], mm4
		movntq	[ebx+40], mm5
		movntq	[ebx+48], mm6
		movntq	[ebx+56], mm7
		add		ebx, 64
		add		edx, 64
		add		eax, 64
		jnc		blastloop	// û�н�λ��CF��0������ת
skipblastloop:
		sub		eax, 63-7
		jns		noextras	// SF=0
quadloop:
		movq	mm0, [edx]		// һ��8���ֽ�
		movntq	[ebx], mm0
		add		edx, 8
		add		ebx, 8
		add		eax, 8
		jnc		quadloop
noextras:
		sub		eax, 7		// һ��1���ֽ�
		jz		nooddballs
		mov		ecx, eax
		neg		ecx
		mov		esi, edx
		mov		edi, ebx
		rep		movsb		// �ظ�����1���ֽڣ�ECX�Σ���ESI��EDI
nooddballs:
		EMMS
	}
	return pDest;
}