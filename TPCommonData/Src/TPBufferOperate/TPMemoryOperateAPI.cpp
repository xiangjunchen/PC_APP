/********************************************************************
	Copyright (c) 2004, Sobey数码 内容制作部
	All rights reserved.
	创建时间:	2004年8月3日   10:10
	文件名: 	g:\e7\chenyufeng_desktop_cg\desktopsobey\tpcommondata\src\tpbufferoperate\tpmaskoperateapi.cpp
	当前版本：	1.0
	作者:		陈玉峰
	摘要:		内存缓冲区操作
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
// 功能：设置内存为指定的值
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
void* CALLBACK TP_MemSet(IN OUT void* pDest,
						IN int nChar,
						IN int nSize);

//////////////////////////////////////////////////////////////////////////
// 功能：内存拷贝(一次可以拷贝16个字节，加速)
// 参数：
// 返回值：
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

		neg		eax			// EAX取相反数，CF标识其是否为0
		add		eax, 63			
		jbe		skipblastloop		// CF或ZF为1时跳转
blastloop:
		movq	mm0, [edx]			// 一次8×8＝64个字节
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
		jnc		blastloop	// 没有进位（CF＝0）则跳转
skipblastloop:
		sub		eax, 63-7
		jns		noextras	// SF=0
quadloop:
		movq	mm0, [edx]		// 一次8个字节
		movntq	[ebx], mm0
		add		edx, 8
		add		ebx, 8
		add		eax, 8
		jnc		quadloop
noextras:
		sub		eax, 7		// 一次1个字节
		jz		nooddballs
		mov		ecx, eax
		neg		ecx
		mov		esi, edx
		mov		edi, ebx
		rep		movsb		// 重复复制1个字节，ECX次，从ESI到EDI
nooddballs:
		EMMS
	}
	return pDest;
}