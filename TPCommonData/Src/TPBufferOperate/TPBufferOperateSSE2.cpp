//////////////////////////////////////////////////////////////////////////
// TPBufferOperateSSE2.cppp
//
// Lai Shoubo @ 2009-09-02
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cmath>
#include <malloc.h>
#include <cassert>
#include "TPBufferOperateSSE2.h"

#ifdef _USE_MULTI_CORE_
bo::thread_manager g_thdmgr;
#endif

// 掩码常量
static __declspec(align(16)) DWORD const g_maskDLB[4] = {0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF};
static __declspec(align(16)) DWORD const g_maskDHB[4] = {0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000};
static __declspec(align(16)) DWORD const g_maskDLBN[4] = {0xFFFFFF00, 0xFFFFFF00, 0xFFFFFF00, 0xFFFFFF00};
static __declspec(align(16)) DWORD const g_maskDHBN[4] = {0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF};
static __declspec(align(16)) DWORD const g_maskLW[4] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
static __declspec(align(16)) DWORD const g_maskHW[4] = {0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00};
static __declspec(align(16)) DWORD const g_maskLD[4] = {0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF};
static __declspec(align(16)) DWORD const g_maskHD[4] = {0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000};
static __declspec(align(16)) DWORD const g_maskQHW[4] = {0x00000000, 0x00FF0000, 0x00000000, 0x00FF0000};
static __declspec(align(16)) DWORD const g_maskSpec1[4] = {0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
static __declspec(align(16)) DWORD const g_maskSpec2[4] = {0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF};
static __declspec(align(16)) DWORD const g_maskSpec3[4] = {0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF};

// Safe Color饱合常量
static __declspec(align(16)) DWORD const g_maskByte16[4] = {0x10101010, 0x10101010, 0x10101010, 0x10101010};
static __declspec(align(16)) DWORD const g_maskByte235[4] = {0xEBEBEBEB, 0xEBEBEBEB, 0xEBEBEBEB, 0xEBEBEBEB};
static __declspec(align(16)) DWORD const g_maskWord16[4] = {0x00100010, 0x00100010, 0x00100010, 0x00100010};
static __declspec(align(16)) DWORD const g_maskWord235[4] = {0x00EB00EB, 0x00EB00EB, 0x00EB00EB, 0x00EB00EB};
static __declspec(align(16)) DWORD const g_maskByte8[4] = {0x08080808, 0x08080808, 0x08080808, 0x08080808};
static __declspec(align(16)) DWORD const g_maskByte127[4] = {0x7F7F7F7F, 0x7F7F7F7F, 0x7F7F7F7F, 0x7F7F7F7F};
static __declspec(align(16)) DWORD const g_maskByte117[4] = {0x75757575, 0x75757575, 0x75757575, 0x75757575};
static __declspec(align(16)) DWORD const g_maskByte128[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
static __declspec(align(16)) DWORD const g_maskWord128[4] = {0x00800080, 0x00800080, 0x00800080, 0x00800080};

// YUV RGB 饱合常量
static __declspec(align(16)) DWORD const g_maskByte0[4] = {0x00000000, 0x00000000, 0x00000000, 0x00000000};
static __declspec(align(16)) DWORD const g_maskWord0[4] = {0x00000000, 0x00000000, 0x00000000, 0x00000000};
static __declspec(align(16)) DWORD const g_maskByte255[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
static __declspec(align(16)) DWORD const g_maskWord255[4] = {0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF};

// 补偿常量
static __declspec(align(16)) DWORD const g_maskComp1654[4] = {0x01080108, 0x01080108, 0x01080108, 0x01080108};
static __declspec(align(16)) DWORD const g_maskComp164[4] = {0x01000100, 0x01000100, 0x01000100, 0x01000100};
static __declspec(align(16)) DWORD const g_maskComp51[4] = {0x00010001, 0x00010001, 0x00010001, 0x00010001};
static __declspec(align(16)) DWORD const g_maskComp54[4] = {0x00080008, 0x00080008, 0x00080008, 0x00080008};
static __declspec(align(16)) DWORD const g_maskComp1284[4] = {0x08000800, 0x08000800, 0x08000800, 0x08000800};
static __declspec(align(16)) DWORD const g_maskComp1286[4] = {0x20002000, 0x20002000, 0x20002000, 0x20002000};
static __declspec(align(16)) DWORD const g_maskComp12851[4] = {0x01010101, 0x01010101, 0x01010101, 0x01010101};
static __declspec(align(16)) DWORD const g_maskComp12854[4] = {0x08080808, 0x08080808, 0x08080808, 0x08080808};
static __declspec(align(16)) DWORD const g_maskComp128514[4] = {0x00202000, 0x00202000, 0x00202000, 0x00202000};

// 系数常量，放大16位 65536 * 219 / 255 = 56284
static __declspec(align(16)) DWORD const g_maskSafeColor[4] = {0xDBDCDBDC, 0xDBDCDBDC, 0xDBDCDBDC, 0xDBDCDBDC};

// 系数常量，放大14位 16384 * 255 / 219 = 19077
static __declspec(align(16)) DWORD const g_maskNotSafeColor[4] = {0x4A854A85, 0x4A854A85, 0x4A854A85, 0x4A854A85};

// 系数常量，放大20位 1048576 / 255 = 4112
static __declspec(align(16)) DWORD const g_mask255rcp20[4] = {0x10101010, 0x10101010, 0x10101010, 0x10101010};

// 系数常量，放大14位，顺序为 RyRu RvGu GvBy BuBv
static __declspec(align(16)) DWORD const g_maskYUV2RGBHD[4] = {0x00004000, 0xF40364C8, 0x4000E209, 0x000076C2};
static __declspec(align(16)) DWORD const g_maskYUV2RGBSD[4] = {0x00004000, 0xE9FA59BA, 0x4000D24C, 0x00007168};

// 系数常量，放大14位，顺序为 YrYg YbUr UgUb VgVb
static __declspec(align(16)) DWORD const g_maskRGB2YUVHD[4] = {0x2DC50D9C, 0xF8AB049F, 0x2000E755, 0xFD11E2EF};
static __declspec(align(16)) DWORD const g_maskRGB2YUVSD[4] = {0x25921322, 0xF534074C, 0x2000EACC, 0xFACCE534};

// 单精度浮点常量
static __declspec(align(16)) float const g_mask16384f[4] = {16384.f, 16384.f, 16384.f, 16384.f};
static __declspec(align(16)) float const g_mask05f[4] = {0.5f, 0.5f, 0.5f, 0.5f};

//
// Y = Y * (235 - 16) / 255 + 16
// UV = (UV - 128) * (235 - 16) / 255 + 128
//
void TPYUYVConvertSafeColor(IN OUT void* pYUYV,
							IN int iPitch,				// 一行的字节数
							IN int iHeight)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	if(!pYUYV || iPitch <= 0 || iHeight <= 0)
	{
		return;
	}
	__asm
	{
		mov			esi, pYUYV;
		mov			eax, iPitch;
		mov			ecx, iHeight;
		mul			ecx;
		shr			eax, 02h;
		mov			ecx, eax;
		shr			ecx, 03h;			// 一次处理8个
		and			eax, 07h;
		movaps		xmm4, g_maskSafeColor;
		movaps		xmm5, g_maskComp1654;
		movaps		xmm6, g_maskComp12854;
		psrld		xmm5, 10h;
		pslld		xmm6, 10h;
		orps		xmm5, xmm6;
		movaps		xmm6, g_maskWord128;
		xorps		xmm7, xmm7;
		pslld		xmm6, 10h;
		prefetchnta	[esi];
		test		ecx, ecx;
		jz			loop_4;
		test		esi, 0fh;
		jnz			loop_8;
loop_a8:
		prefetchnta	[esi + 20h];
		movaps		xmm0, [esi];
		movaps		xmm2, [esi + 10h];
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm7;
		punpcklbw	xmm2, xmm7;
		punpckhbw	xmm1, xmm7;
		punpckhbw	xmm3, xmm7;
		psubsw		xmm0, xmm6;
		psubsw		xmm1, xmm6;
		psubsw		xmm2, xmm6;
		psubsw		xmm3, xmm6;
		psllw		xmm0, 04h;
		psllw		xmm1, 04h;
		psllw		xmm2, 04h;
		psllw		xmm3, 04h;
		pmulhw		xmm0, xmm4;
		pmulhw		xmm1, xmm4;
		pmulhw		xmm2, xmm4;
		pmulhw		xmm3, xmm4;
		paddsw		xmm0, xmm5;
		paddsw		xmm1, xmm5;
		paddsw		xmm2, xmm5;
		paddsw		xmm3, xmm5;
		psrlw		xmm0, 04h;
		psrlw		xmm1, 04h;
		psrlw		xmm2, 04h;
		psrlw		xmm3, 04h;
		packuswb	xmm0, xmm1;
		packuswb	xmm2, xmm3;
		movntps		[esi], xmm0;
		movntps		[esi + 10h], xmm2;
		add			esi, 20h;
		dec			ecx;
		jnz			loop_a8;
		sfence
		jmp			loop_4;
loop_8:
		prefetchnta	[esi + 20h];
		movups		xmm0, [esi];
		movups		xmm2, [esi + 10h];
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm7;
		punpcklbw	xmm2, xmm7;
		punpckhbw	xmm1, xmm7;
		punpckhbw	xmm3, xmm7;
		psubsw		xmm0, xmm6;
		psubsw		xmm1, xmm6;
		psubsw		xmm2, xmm6;
		psubsw		xmm3, xmm6;
		psllw		xmm0, 04h;
		psllw		xmm1, 04h;
		psllw		xmm2, 04h;
		psllw		xmm3, 04h;
		pmulhw		xmm0, xmm4;
		pmulhw		xmm1, xmm4;
		pmulhw		xmm2, xmm4;
		pmulhw		xmm3, xmm4;
		paddsw		xmm0, xmm5;
		paddsw		xmm1, xmm5;
		paddsw		xmm2, xmm5;
		paddsw		xmm3, xmm5;
		psrlw		xmm0, 04h;
		psrlw		xmm1, 04h;
		psrlw		xmm2, 04h;
		psrlw		xmm3, 04h;
		packuswb	xmm0, xmm1;
		packuswb	xmm2, xmm3;
		movups		[esi], xmm0;
		movups		[esi + 10h], xmm2;
		add			esi, 20h;
		dec			ecx;
		jnz			loop_8;
loop_4:
		test		eax, 04h;
		jz			loop_2;
		movups		xmm0, [esi];
		movaps		xmm1, xmm0;
		punpcklbw	xmm0, xmm7;
		punpckhbw	xmm1, xmm7;
		psubsw		xmm0, xmm6;
		psubsw		xmm1, xmm6;
		psllw		xmm0, 04h;
		psllw		xmm1, 04h;
		pmulhw		xmm0, xmm4;
		pmulhw		xmm1, xmm4;
		paddsw		xmm0, xmm5;
		paddsw		xmm1, xmm5;
		psrlw		xmm0, 04h;
		psrlw		xmm1, 04h;
		packuswb	xmm0, xmm1;
		movups		[esi], xmm0;
		add			esi, 10h;
loop_2:
		test		eax, 02h;
		jz			loop_1;
		movlps		xmm0, [esi];
		punpcklbw	xmm0, xmm7;
		psubsw		xmm0, xmm6;
		psllw		xmm0, 04h;
		pmulhw		xmm0, xmm4;
		paddsw		xmm0, xmm5;
		psrlw		xmm0, 04h;
		packuswb	xmm0, xmm0;
		movlps		[esi], xmm0;
		add			esi, 08h;
loop_1:
		test		eax, 01h;
		jz			loop_0;
		movss		xmm0, [esi];
		punpcklbw	xmm0, xmm7;
		psubsw		xmm0, xmm6;
		psllw		xmm0, 04h;
		pmulhw		xmm0, xmm4;
		paddsw		xmm0, xmm5;
		psrlw		xmm0, 04h;
		packuswb	xmm0, xmm0;
		movss		[esi], xmm0;
loop_0:
	}
}

// A不处理
//
// RGB = RGB * (235 - 16) / 255 + 16
//
void TPBGRAConvertSafeColor(IN OUT void* pBGRA,
							IN int iPitch,				// 一行的字节数
							IN int iHeight)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	if(!pBGRA || iPitch <= 0 || iHeight <= 0)
	{
		return;
	}

	__asm
	{
		mov			edi, pBGRA;
		mov			ecx, iPitch;
		mov			edx, iHeight;
		movaps		xmm7, g_maskSafeColor;
		movaps		xmm6, g_maskComp1654;
		pcmpeqb		xmm5, xmm5;
		xorps		xmm4, xmm4;
		psrlq		xmm7, 16;
		psllq		xmm5, 48;
		psrlq		xmm6, 16;
		orps		xmm7, xmm5;
		prefetchnta	[edi];
loop_height_beg:
		cmp			edx, 0;
		je			loop_end;
		dec			edx;
		push		edi;
		push		ecx;
		test		edi, 0x0f;
		jnz			loop_32;
loop_a32:
		cmp			ecx, 0x20;
		jl			loop_16;
		prefetchnta	[edi + 0x20];
		movaps		xmm0, [edi];
		movaps		xmm2, [edi + 0x10];
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm4;
		punpcklbw	xmm2, xmm4;
		punpckhbw	xmm1, xmm4;
		punpckhbw	xmm3, xmm4;
		psllw		xmm0, 4;
		psllw		xmm1, 4;
		psllw		xmm2, 4;
		psllw		xmm3, 4;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		pmulhuw		xmm2, xmm7;
		pmulhuw		xmm3, xmm7;
		paddusw		xmm0, xmm6;
		paddusw		xmm1, xmm6;
		paddusw		xmm2, xmm6;
		paddusw		xmm3, xmm6;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		psrlw		xmm2, 4;
		psrlw		xmm3, 4;
		packuswb	xmm0, xmm1;
		packuswb	xmm2, xmm3;
		movntps		[edi], xmm0;
		movntps		[edi + 0x10], xmm2;
		add			edi, 0x20;
		sub			ecx, 0x20;
		jmp			loop_a32;
loop_32:
		cmp			ecx, 0x20;
		jl			loop_16;
		prefetchnta	[edi + 0x20];
		movups		xmm0, [edi];
		movups		xmm2, [edi + 0x10];
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm4;
		punpcklbw	xmm2, xmm4;
		punpckhbw	xmm1, xmm4;
		punpckhbw	xmm3, xmm4;
		psllw		xmm0, 4;
		psllw		xmm1, 4;
		psllw		xmm2, 4;
		psllw		xmm3, 4;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		pmulhuw		xmm2, xmm7;
		pmulhuw		xmm3, xmm7;
		paddusw		xmm0, xmm6;
		paddusw		xmm1, xmm6;
		paddusw		xmm2, xmm6;
		paddusw		xmm3, xmm6;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		psrlw		xmm2, 4;
		psrlw		xmm3, 4;
		packuswb	xmm0, xmm1;
		packuswb	xmm2, xmm3;
		movups		[edi], xmm0;
		movups		[edi + 0x10], xmm2;
		add			edi, 0x20;
		sub			ecx, 0x20;
		jmp			loop_32;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_12;
		movups		xmm0, [edi];
		movaps		xmm1, xmm0;
		punpcklbw	xmm0, xmm4;
		punpckhbw	xmm1, xmm4;
		psllw		xmm0, 4;
		psllw		xmm1, 4;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		paddusw		xmm0, xmm6;
		paddusw		xmm1, xmm6;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		packuswb	xmm0, xmm1;
		movups		[edi], xmm0;
		add			edi, 0x10;
		sub			ecx, 0x10;
		jz			loop_height_end;
loop_12:
		cmp			ecx, 12;
		jl			loop_8;
		movss		xmm0, [edi];
		movhps		xmm0, [edi + 4];
		movaps		xmm1, xmm0;
		punpcklbw	xmm0, xmm4;
		punpckhbw	xmm1, xmm4;
		psllw		xmm0, 4;
		psllw		xmm1, 4;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		paddusw		xmm0, xmm6;
		paddusw		xmm1, xmm6;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		packuswb	xmm0, xmm1;
		movss		[edi], xmm0;
		movhps		[edi + 4], xmm0;
		jmp			loop_height_end;
loop_8:
		cmp			ecx, 8;
		jl			loop_4;
		movlps		xmm0, [edi];
		punpcklbw	xmm0, xmm4;
		psllw		xmm0, 4;
		pmulhuw		xmm0, xmm7;
		paddusw		xmm0, xmm6;
		psrlw		xmm0, 4;
		packuswb	xmm0, xmm0;
		movlps		[edi], xmm0;
		jmp			loop_height_end;
loop_4:
		cmp			ecx, 4;
		jl			loop_height_end;
		movss		xmm0, [edi];
		punpcklbw	xmm0, xmm4;
		psllw		xmm0, 4;
		pmulhuw		xmm0, xmm7;
		paddusw		xmm0, xmm6;
		psrlw		xmm0, 4;
		packuswb	xmm0, xmm0;
		movss		[edi], xmm0;
loop_height_end:
		pop			ecx;
		pop			edi;
		add			edi, ecx;
		jmp			loop_height_beg;
loop_end:
		sfence
	}
}

// ARGB <-> BGRA
void TPByteSwapARGBOrBGRA(OUT void* des,
						  IN void* src,
						  IN int count)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	__asm
	{
		mov			esi, src;
		mov			edi, des;
		mov			ecx, count;
		prefetchnta [esi];
		test		esi, 0x0f;
		jnz			loop_64;
		test		edi, 0x0f;
		jnz			loop_64;
loop_a64:
		cmp			ecx, 0x40;
		jl			loop_a16;
		prefetchnta [esi + 64];
		movaps		xmm0, [esi];
		movaps		xmm2, [esi + 16];
		movaps		xmm4, [esi + 32];
		movaps		xmm6, [esi + 48];
		pshuflw		xmm0, xmm0, 0xb1;
		pshufhw		xmm0, xmm0, 0xb1;
		movaps		xmm1, xmm0;
		pshuflw		xmm2, xmm2, 0xb1;
		pshufhw		xmm2, xmm2, 0xb1;
		movaps		xmm3, xmm2;
		pshuflw		xmm4, xmm4, 0xb1;
		pshufhw		xmm4, xmm4, 0xb1;
		movaps		xmm5, xmm4;
		pshuflw		xmm6, xmm6, 0xb1;
		pshufhw		xmm6, xmm6, 0xb1;
		movaps		xmm7, xmm6;
		psllw		xmm0, 8;
		psrlw		xmm1, 8;
		orps		xmm0, xmm1;
		psllw		xmm2, 8;
		psrlw		xmm3, 8;
		orps		xmm2, xmm3;
		psllw		xmm4, 8;
		psrlw		xmm5, 8;
		orps		xmm4, xmm5;
		psllw		xmm6, 8;
		psrlw		xmm7, 8;
		orps		xmm6, xmm7;
		movntps		[edi], xmm0;
		movntps		[edi + 16], xmm2;
		movntps		[edi + 32], xmm4;
		movntps		[edi + 48], xmm6;
		add			esi, 0x40;
		add			edi, 0x40;
		sub			ecx, 0x40;
		jmp			loop_a64;
loop_64:
		cmp			ecx, 0x40;
		jl			loop_16;
		prefetchnta [esi + 64];
		movups		xmm0, [esi];
		movups		xmm2, [esi + 16];
		movups		xmm4, [esi + 32];
		movups		xmm6, [esi + 48];
		pshuflw		xmm0, xmm0, 0xb1;
		pshufhw		xmm0, xmm0, 0xb1;
		movaps		xmm1, xmm0;
		pshuflw		xmm2, xmm2, 0xb1;
		pshufhw		xmm2, xmm2, 0xb1;
		movaps		xmm3, xmm2;
		pshuflw		xmm4, xmm4, 0xb1;
		pshufhw		xmm4, xmm4, 0xb1;
		movaps		xmm5, xmm4;
		pshuflw		xmm6, xmm6, 0xb1;
		pshufhw		xmm6, xmm6, 0xb1;
		movaps		xmm7, xmm6;
		psllw		xmm0, 8;
		psrlw		xmm1, 8;
		orps		xmm0, xmm1;
		psllw		xmm2, 8;
		psrlw		xmm3, 8;
		orps		xmm2, xmm3;
		psllw		xmm4, 8;
		psrlw		xmm5, 8;
		orps		xmm4, xmm5;
		psllw		xmm6, 8;
		psrlw		xmm7, 8;
		orps		xmm6, xmm7;
		movups		[edi], xmm0;
		movups		[edi + 16], xmm2;
		movups		[edi + 32], xmm4;
		movups		[edi + 48], xmm6;
		add			esi, 0x40;
		add			edi, 0x40;
		sub			ecx, 0x40;
		jmp			loop_64;
loop_a16:
		cmp			ecx, 0x10;
		jl			loop_12;
		movaps		xmm0, [esi];
		pshuflw		xmm0, xmm0, 0xb1;
		pshufhw		xmm0, xmm0, 0xb1;
		movaps		xmm1, xmm0;
		psllw		xmm0, 8;
		psrlw		xmm1, 8;
		orps		xmm0, xmm1;
		movntps		[edi], xmm0;
		add			esi, 0x10;
		add			edi, 0x10;
		sub			ecx, 0x10;
		jmp			loop_a16;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_12;
		movups		xmm0, [esi];
		pshuflw		xmm0, xmm0, 0xb1;
		pshufhw		xmm0, xmm0, 0xb1;
		movaps		xmm1, xmm0;
		psllw		xmm0, 8;
		psllw		xmm1, 8;
		orps		xmm0, xmm1;
		movups		[edi], xmm0;
		add			esi, 0x10;
		add			edi, 0x10;
		sub			ecx, 0x10;
		jmp			loop_16;
loop_12:
		cmp			ecx, 12;
		jl			loop_8;
		movss		xmm0, [esi];
		movhpd		xmm0, [esi + 4];
		pshuflw		xmm0, xmm0, 0xb1;
		pshufhw		xmm0, xmm0, 0xb1;
		movaps		xmm1, xmm0;
		psllw		xmm0, 8;
		psrlw		xmm1, 8;
		orps		xmm0, xmm1;
		movss		[edi], xmm0;
		movhpd		[edi + 4], xmm0;
		add			esi, 12;
		add			edi, 12;
		sub			ecx, 12;
		jz			loop_end;
loop_8:
		cmp			ecx, 8;
		jl			loop_4;
		movsd		xmm0, [esi];
		pshuflw		xmm0, xmm0, 0xb1;
		movaps		xmm1, xmm0;
		psllw		xmm0, 8;
		psrlw		xmm1, 8;
		orps		xmm0, xmm1;
		movsd		[edi], xmm0;
		add			esi, 8;
		add			edi, 8;
		sub			ecx, 8;
		jz			loop_end;
loop_4:
		cmp			ecx, 4;
		jl			loop_end;
		movss		xmm0, [esi];
		pshuflw		xmm0, xmm0, 0xb1;
		movaps		xmm1, xmm0;
		psllw		xmm0, 8;
		psrlw		xmm1, 8;
		orps		xmm0, xmm1;
		movss		[edi], xmm0;
loop_end:
		sfence
	}
}

// ARGB -> YUYVA
void TPCvtARGB2YUYVALine(OUT BYTE* pYUYV,
						 OUT BYTE* pAlpha,
						 IN BYTE* pARGB,
						 IN int iPitch,
						 IN BYTE* pRGB2YUVCoef)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	__asm
	{
		mov			esi, pARGB;
		mov			edi, pYUYV;
		mov			edx, pAlpha;
		mov			ecx, iPitch;
		mov			eax, pRGB2YUVCoef;
		movaps		xmm7, [eax];				// VbVg UbUg UrYb YgYr
		prefetchnta [esi];
		test		esi, 0x0f;
		jnz			loop_32;
		test		edi, 0x0f;
		jnz			loop_32;
loop_a32:
		cmp			ecx, 0x20;
		jl			loop_16;
		prefetchnta	[esi + 32];
		movaps		xmm0, [esi];
		movaps		xmm1, [esi + 16];			// BGRA BGRA BGRA BGRA
		movaps		xmm2, xmm0;
		movaps		xmm3, xmm1;
		andps		xmm2, g_maskDLB;
		andps		xmm3, g_maskDLB;			// 000A 000A 000A 000A
		packssdw	xmm2, xmm3;					// 0A0A 0A0A 0A0A 0A0A
		packuswb	xmm2, xmm2;					// AAAA AAAA AAAA AAAA
		movsd		[edx], xmm2;				// AAAA AAAA
		psrld		xmm0, 8;
		psrld		xmm1, 8;					// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm2, xmm0;
		movaps		xmm3, xmm1;
		andps		xmm2, g_maskDLB;
		andps		xmm3, g_maskDLB;			// 000R 000R 000R 000R
		packssdw	xmm2, xmm3;
		psllw		xmm2, 6;					// R7R6 R5R4 R3R2 R1R0
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		andps		xmm3, g_maskHW;
		andps		xmm4, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm3, 8;
		psrld		xmm4, 8;
		packssdw	xmm3, xmm4;					// 0G0G 0G0G 0G0G 0G0G
		psllw		xmm3, 6;					// G7G6 G5G4 G3G2 G1G0
		psrld		xmm0, 16;
		psrld		xmm1, 16;					// 000B 000B 000B 000B
		packssdw	xmm0, xmm1;					// 0B0B 0B0B 0B0B 0B0B
		psllw		xmm0, 6;					// B7B6 B5B4 B3B2 B1B0
		pshuflw		xmm1, xmm7, 0x00;
		shufps		xmm1, xmm1, 0x00;			// YrYr YrYr YrYr YrYr
		pmulhw		xmm1, xmm2;					// YR
		pshuflw		xmm4, xmm7, 0x05;
		shufps		xmm4, xmm4, 0x00;			// YgYg YgYg YgYg YgYg
		pmulhw		xmm4, xmm3;					// YG
		paddsw		xmm1, xmm4;					// YR + YG
		pshuflw		xmm4, xmm7, 0x0a;
		shufps		xmm4, xmm4, 0x00;			// YbYb YbYb YbYb YbYb
		pmulhw		xmm4, xmm0;					// YB
		paddsw		xmm1, xmm4;
		paddsw		xmm1, g_maskComp54;
		psrlw		xmm1, 4;					// Y
		pminsw		xmm1, g_maskWord255;
		pmaxsw		xmm1, g_maskWord0;
		pshuflw		xmm2, xmm2, 0xa0;
		pshufhw		xmm2, xmm2, 0xa0;			// R6R6 R4R4 R2R2 R0R0
		pshufd		xmm4, xmm7, 0x09;			// YgYr YgYr UbUg UrYb
		pshuflw		xmm4, xmm4, 0x0d;
		shufps		xmm4, xmm4, 0x00;			// VrUr VrUr VrUr VrUr
		pmulhw		xmm2, xmm4;					// VR UR
		pshuflw		xmm3, xmm3, 0xa0;
		pshufhw		xmm3, xmm3, 0xa0;			// G6G6 G4G4 G2G2 G0G0
		pshufhw		xmm4, xmm7, 0x88;
		shufps		xmm4, xmm4, 0xff;			// VgUg VgUg VgUg VgUg
		pmulhw		xmm3, xmm4;
		paddsw		xmm2, xmm3;					// VR + VG UR + UG
		pshuflw		xmm0, xmm0, 0xa0;
		pshufhw		xmm0, xmm0, 0xa0;			// B6B6 B4B4 B2B2 B0B0
		pshufhw		xmm4, xmm7, 0xdd;
		shufps		xmm4, xmm4, 0xff;			// VbUb VbUb VbUb VbUb
		pmulhw		xmm0, xmm4;					// VB UB
		paddsw		xmm0, xmm2;
		paddsw		xmm0, g_maskComp12854;
		psrlw		xmm0, 4;					// V U
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm0, g_maskWord0;
		psllw		xmm0, 8;
		orps		xmm0, xmm1;					// V6Y7U6Y6 V4Y5U4Y4 V2Y3U2Y2 V0Y1U0Y0
		movntps		[edi], xmm0;
		add			esi, 32;
		add			edi, 16;
		add			edx, 8;
		sub			ecx, 32;
		jmp			loop_a32;
loop_32:
		cmp			ecx, 0x20;
		jl			loop_16;
		prefetchnta	[esi + 32];
		movups		xmm0, [esi];
		movups		xmm1, [esi + 16];			// BGRA BGRA BGRA BGRA
		movaps		xmm2, xmm0;
		movaps		xmm3, xmm1;
		andps		xmm2, g_maskDLB;
		andps		xmm3, g_maskDLB;			// 000A 000A 000A 000A
		packssdw	xmm2, xmm3;					// 0A0A 0A0A 0A0A 0A0A
		packuswb	xmm2, xmm2;					// AAAA AAAA AAAA AAAA
		movsd		[edx], xmm2;				// AAAA AAAA
		psrld		xmm0, 8;
		psrld		xmm1, 8;					// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm2, xmm0;
		movaps		xmm3, xmm1;
		andps		xmm2, g_maskDLB;
		andps		xmm3, g_maskDLB;			// 000R 000R 000R 000R
		packssdw	xmm2, xmm3;
		psllw		xmm2, 6;					// R7R6 R5R4 R3R2 R1R0
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		andps		xmm3, g_maskHW;
		andps		xmm4, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm3, 8;
		psrld		xmm4, 8;					// 000G 000G 000G 000G
		packssdw	xmm3, xmm4;					// 0G0G 0G0G 0G0G 0G0G
		psrlw		xmm3, 2;					// G7G6 G5G4 G3G2 G1G0
		psrld		xmm0, 16;
		psrld		xmm1, 16;					// 000B 000B 000B 000B
		packssdw	xmm0, xmm1;					// 0B0B 0B0B 0B0B 0B0B
		psllw		xmm0, 6;					// B7B6 B5B4 B3B2 B1B0
		pshuflw		xmm1, xmm7, 0x00;
		shufps		xmm1, xmm1, 0x00;			// YrYr YrYr YrYr YrYr
		pmulhw		xmm1, xmm2;					// YR
		pshuflw		xmm4, xmm7, 0x05;
		shufps		xmm4, xmm4, 0x00;			// YgYg YgYg YgYg YgYg
		pmulhw		xmm4, xmm3;					// YG
		paddsw		xmm1, xmm4;					// YR + YG
		pshuflw		xmm4, xmm7, 0x0a;
		shufps		xmm4, xmm4, 0x00;			// YbYb YbYb YbYb YbYb
		pmulhw		xmm4, xmm0;					// YB
		paddsw		xmm1, xmm4;
		paddsw		xmm1, g_maskComp54;
		psrlw		xmm1, 4;					// Y
		pminsw		xmm1, g_maskWord255;
		pmaxsw		xmm1, g_maskWord0;
		pshuflw		xmm2, xmm2, 0xa0;
		pshufhw		xmm2, xmm2, 0xa0;			// R6R6 R4R4 R2R2 R0R0
		pshufd		xmm4, xmm7, 0x09;			// YgYr YgYr UbUg UrYb
		pshuflw		xmm4, xmm4, 0x0d;
		shufps		xmm4, xmm4, 0x00;			// VrUr VrUr VrUr VrUr
		pmulhw		xmm2, xmm4;					// VR UR
		pshuflw		xmm3, xmm3, 0xa0;
		pshufhw		xmm3, xmm3, 0xa0;			// G6G6 G4G4 G2G2 G0G0
		pshufhw		xmm4, xmm7, 0x88;
		shufps		xmm4, xmm4, 0xff;			// VgUg VgUg VgUg VgUg
		pmulhw		xmm3, xmm4;
		paddsw		xmm2, xmm3;					// VR + VG UR + UG
		pshuflw		xmm0, xmm0, 0xa0;
		pshufhw		xmm0, xmm0, 0xa0;			// B6B6 B4B4 B2B2 B0B0
		pshufhw		xmm4, xmm7, 0xdd;
		shufps		xmm4, xmm4, 0xff;			// VbUb VbUb VbUb VbUb
		pmulhw		xmm0, xmm4;					// VB UB
		paddsw		xmm0, xmm2;
		paddsw		xmm0, g_maskComp12854;
		psrlw		xmm0, 4;					// V U
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm0, g_maskWord0;
		psllw		xmm0, 8;
		orps		xmm0, xmm1;					// V6Y7U6Y6 V4Y5U4Y4 V2Y3U2Y2 V0Y1U0Y0
		movups		[edi], xmm0;
		add			esi, 32;
		add			edi, 16;
		add			edx, 8;
		sub			ecx, 32;
		jmp			loop_32;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_12;
		movups		xmm0, [esi];				// BGRA BGRA BGRA BGRA
		movaps		xmm1, xmm0;
		andps		xmm1, g_maskDLB;			// 000A 000A 000A 000A
		packssdw	xmm1, xmm1;					// 0A0A 0A0A 0A0A 0A0A
		packuswb	xmm1, xmm1;					// AAAA AAAA AAAA AAAA
		movss		[edx], xmm1;				// AAAA
		psrld		xmm0, 8;					// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm1, xmm0;
		andps		xmm1, g_maskLW;				// 0B0R 0B0R 0B0R 0B0R
		movaps		xmm2, xmm0;
		andps		xmm2, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm2, 8;					// 000G 000G 000G 000G
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		pshuflw		xmm4, xmm7, 0x08;
		shufps		xmm4, xmm4, 0x00;			// YbYr YbYr YbYr YbYr
		pshuflw		xmm5, xmm7, 0x05;
		shufps		xmm5, xmm5, 0x00;			// YgYg YgYg YgYg YgYg
		pmaddwd		xmm1, xmm4;					// YR + YB
		pmaddwd		xmm3, xmm5;					// YG
		shufps		xmm0, xmm0, 0xa0;
		shufps		xmm2, xmm2, 0xa0;
		pshufd		xmm4, xmm7, 0xe9;			// VbVg UbUg UbUg UrYb
		pshuflw		xmm4, xmm4, 0x0d;
		pshufhw		xmm4, xmm4, 0xd0;
		shufps		xmm4, xmm4, 0xcc;			// VbVr UbUr VbVr UbUr
		pshufd		xmm5, xmm7, 0xa0;			// VgVg UgUg VgVg UgUg
		pmaddwd		xmm0, xmm4;
		pmaddwd		xmm2, xmm5;
		paddd		xmm1, xmm3;
		paddd		xmm0, xmm2;
		paddd		xmm1, g_maskComp128514;
		paddd		xmm0, g_maskComp128514;
		psrlw		xmm1, 14;
		psrlw		xmm0, 14;
		packssdw	xmm1, xmm1;
		packssdw	xmm0, xmm0;
		pminsw		xmm1, g_maskWord255;
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm1, g_maskWord0;
		pmaxsw		xmm0, g_maskWord0;
		psllw		xmm0, 8;
		orps		xmm0, xmm1;					// V2Y3U2Y2 V0Y1U0Y0 V2Y3U2Y2 V0Y1U0Y0
		movsd		[edi], xmm0;
		add			edx, 4;
		add			esi, 16;
		add			edi, 8;
		sub			ecx, 16;
		jz			loop_end;
loop_12:
		cmp			ecx, 0x0c;
		jl			loop_8;
		movss		xmm0, [esi];
		movhpd		xmm0, [esi + 4];			// BGRA BGRA 0000 BGRA
		movaps		xmm1, xmm0;
		shufps		xmm0, xmm0, 0xf8;			// BGRA BGRA BGRA BGRA
		psrld		xmm0, 8;					// 0BGR 0BGR 0BGR 0BGR
		andps		xmm1, g_maskDLB;			// 000A 000A 0000 000A
		packssdw	xmm1, xmm1;
		packuswb	xmm1, xmm1;					// AA0A AA0A AA0A AA0A
		movd		eax, xmm1;
		mov			[edx], al;
		shr			eax, 16;
		mov			[edx + 1], ax;
		movaps		xmm1, xmm0;
		andps		xmm1, g_maskLW;				// 0B0R 0B0R 0B0R 0B0R
		movaps		xmm2, xmm0;
		andps		xmm2, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm2, 8;					// 000G 000G 000G 000G
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		pshuflw		xmm4, xmm7, 0x08;
		shufps		xmm4, xmm4, 0x00;			// YbYr YbYr YbYr YbYr
		pshuflw		xmm5, xmm7, 0x05;
		shufps		xmm5, xmm5, 0x00;			// YgYg YgYg YgYg YgYg
		pmaddwd		xmm1, xmm4;					// YR + YB
		pmaddwd		xmm3, xmm5;					// YG
		shufps		xmm0, xmm0, 0xa0;
		shufps		xmm2, xmm2, 0xa0;
		pshufd		xmm4, xmm7, 0xe9;			// VbVg UbUg UbUg UrYb
		pshuflw		xmm4, xmm4, 0x0d;
		pshufhw		xmm4, xmm4, 0xd0;
		shufps		xmm4, xmm4, 0xcc;			// VbVr UbUr VbVr UbUr
		pshufd		xmm5, xmm7, 0xa0;			// VgVg UgUg VgVg UgUg
		pmaddwd		xmm0, xmm4;
		pmaddwd		xmm2, xmm5;
		paddd		xmm1, xmm3;
		paddd		xmm0, xmm2;
		paddd		xmm1, g_maskComp128514;
		paddd		xmm0, g_maskComp128514;
		psrlw		xmm1, 14;
		psrlw		xmm0, 14;
		packssdw	xmm1, xmm1;
		packssdw	xmm0, xmm0;
		pminsw		xmm1, g_maskWord255;
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm1, g_maskWord0;
		pmaxsw		xmm0, g_maskWord0;
		psllw		xmm0, 8;
		orps		xmm0, xmm1;					// V2Y3U2Y2 V0Y1U0Y0 V2Y3U2Y2 V0Y1U0Y0
		movsd		[edi], xmm0;
		add			edx, 3;
		add			esi, 12;
		add			edi, 8;
		sub			ecx, 12;
		jz			loop_end;
loop_8:
		cmp			ecx, 8;
		jl			loop_4;
		movsd		xmm0, [esi];				// 0000 0000 BGRA BGRA
		movaps		xmm1, xmm0;
		psrld		xmm0, 8;					// 0000 0000 0BGR 0BGR
		shufps		xmm0, xmm0, 0x11;			// 0BGR 0BGR 0BGR 0BGR
		andps		xmm1, g_maskDLB;
		packssdw	xmm1, xmm1;					// 0000 0A0A 0000 0A0A
		packuswb	xmm1, xmm1;					// 00AA 00AA 00AA 00AA
		movd		eax, xmm1;
		mov			[edx], ax;
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;				// 0B0R 0B0R 0B0R 0B0R
		pshuflw		xmm2, xmm7, 0x38;			// VbVg UbUg YrUr YbYr
		pshufhw		xmm2, xmm2, 0xd4;			// VbUb UbUg YrUr YbYr
		shufps		xmm2, xmm2, 0xc9;			// VbUb YbYr UbUg YrUr
		pshuflw		xmm2, xmm2, 0xc0;
		shufps		xmm2, xmm2, 0xe6;			// VbUb YbYr UbUr YbYr
		pmaddwd		xmm0, xmm2;
		andps		xmm1, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm1, 8;					// 000G 000G 000G 000G
		pshufhw		xmm2, xmm7, 0xa0;			// VbVb UgUg UrYb YgYr
		pshuflw		xmm2, xmm2, 0x55;			// VbVb UgUg YgYg YgYg
		shufps		xmm2, xmm2, 0xc8;			// VbVb YgYg UgUg YgYg
		andps		xmm2, g_maskLD;				// 00Vb 00Yg 00Yg 00Yg
		pmaddwd		xmm1, xmm2;
		paddd		xmm0, xmm1;					// V0 Y1 U0 Y0
		paddd		xmm0, g_maskComp128514;
		psrld		xmm0, 14;
		packssdw	xmm0, xmm0;
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm0, g_maskWord0;
		packuswb	xmm0, xmm0;
		movss		[edi], xmm0;
		add			esi, 8;
		add			edx, 2;
		add			edi, 4;
		sub			ecx, 8;
		jz			loop_end;
loop_4:
		cmp			ecx, 4;
		jl			loop_end;
		mov			al, [esi];
		mov			[edx], al;
		movss		xmm0, [esi];				// 0000 0000 0000 BGRA
		psllq		xmm0, 8;					// 0000 0000 0000 0BGR
		shufps		xmm0, xmm0, 0x00;			// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;				// 0B0R 0B0R 0B0R 0B0R
		pshuflw		xmm2, xmm7, 0x38;			// VbVg UbUg YrUr YbYr
		pshufhw		xmm2, xmm2, 0xd4;			// VbUb UbUg YrUr YbYr
		shufps		xmm2, xmm2, 0xc9;			// VbUb YbYr UbUg YrUr
		pshuflw		xmm2, xmm2, 0xc0;
		shufps		xmm2, xmm2, 0xe6;			// VbUb YbYr UbUr YbYr
		pmaddwd		xmm0, xmm2;
		andps		xmm1, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm1, 8;					// 000G 000G 000G 000G
		pshufhw		xmm2, xmm7, 0xa0;			// VbVb UgUg UrYb YgYr
		pshuflw		xmm2, xmm2, 0x55;			// VbVb UgUg YgYg YgYg
		shufps		xmm2, xmm2, 0xc8;			// VbVb YgYg UgUg YgYg
		andps		xmm2, g_maskLD;				// 00Vb 00Yg 00Yg 00Yg
		pmaddwd		xmm1, xmm2;
		paddd		xmm0, xmm1;					// V0 Y1 U0 Y0
		paddd		xmm0, g_maskComp128514;
		psrld		xmm0, 14;
		packssdw	xmm0, xmm0;
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm0, g_maskWord0;
		packuswb	xmm0, xmm0;
		movss		[edi], xmm0;
loop_end:
		sfence
	}
}

void TPCvtARGB2YUYVAFrame(OUT BYTE* pYUYV,
						  OUT BYTE* pAlpha,
						  IN BYTE* pARGB,
						  IN int iPitch,
						  IN int iWidth,
						  IN int iHeight,
						  IN bool bHD,
						  IN bool bReverse)				// 翻转标志
{
	if(!pYUYV || !pAlpha || !pARGB || iPitch <= 0 || iWidth <= 0 || iHeight <= 0)
	{
		return;
	}

	int iYUYVPitch = iWidth << 1;
	int iAlphaPitch = iWidth;
	int iARGBPitch = iWidth << 2;

	if(bReverse)
	{
		pARGB += (iHeight - 1) * iWidth * 4;
		iARGBPitch = -iARGBPitch;
	}

	if(bHD)
	{
		for(int i = 0; i < iHeight; ++i)
		{
			TPCvtARGB2YUYVALine(pYUYV, pAlpha, pARGB, iPitch, (BYTE *)g_maskRGB2YUVHD);
			pYUYV += iYUYVPitch;
			pARGB += iARGBPitch;
			pAlpha += iAlphaPitch;
		}
	}
	else
	{
		for(int i = 0; i < iHeight; ++i)
		{
			TPCvtARGB2YUYVALine(pYUYV, pAlpha, pARGB, iPitch, (BYTE *)g_maskRGB2YUVSD);
			pYUYV += iYUYVPitch;
			pARGB += iARGBPitch;
			pAlpha += iAlphaPitch;
		}
	}
}

void TPMemCpyLine(OUT void* des,
				  IN void const* src,
				  IN int iPitch)
{
	if(!des || !src || iPitch <= 0)
	{
		return;
	}
	__asm
	{
		mov			esi_ptr, src;
		mov			edi_ptr, des;
		mov			ecx, iPitch;
		prefetchnta	[esi_ptr];
		cld			;
		mov			edx, ecx;
		shr			ecx, 6;
		test		ecx, ecx;
		jz			loop_end;
		mov			ecx_ptr, edi_ptr;
		and			ecx_ptr, 0x0f;
		neg			ecx;
		add			ecx, 16;
		and			ecx, 0x0f;
		sub			edx, ecx;
		rep			movsb;
		mov			ecx, edx;
		shr			ecx, 6;
		test		ecx, ecx;
		jz			loop_end;
		test		esi_ptr, 0x0f;
		jnz			loop_64;
loop_a64:
		prefetchnta	[esi_ptr + 64];
		movdqa		xmm0, [esi_ptr];
		movdqa		xmm1, [esi_ptr + 16];
		movdqa		xmm2, [esi_ptr + 32];
		movdqa		xmm3, [esi_ptr + 48];
		movntdq		[edi_ptr], xmm0;
		movntdq		[edi_ptr + 16], xmm1;
		movntdq		[edi_ptr + 32], xmm2;
		movntdq		[edi_ptr + 48], xmm3;
		add			esi_ptr, 0x40;
		add			edi_ptr, 0x40;
		dec			ecx;
		jnz			loop_a64;
		sfence		;
		jmp			loop_end;
loop_64:
		prefetchnta	[esi_ptr + 64];
		movdqu		xmm0, [esi_ptr];
		movdqu		xmm1, [esi_ptr + 16];
		movdqu		xmm2, [esi_ptr + 32];
		movdqu		xmm3, [esi_ptr + 48];
		movntdq		[edi_ptr], xmm0;
		movntdq		[edi_ptr + 16], xmm1;
		movntdq		[edi_ptr + 32], xmm2;
		movntdq		[edi_ptr + 48], xmm3;
		add			esi_ptr, 0x40;
		add			edi_ptr, 0x40;
		dec			ecx;
		jnz			loop_64;
		sfence		;
loop_end:
		and			edx, 63;
		mov			ecx, edx;
		and			ecx, 3;
		rep			movsb;
		mov			ecx, edx;
		shr			ecx, 2;
		rep			movsd;
	}
}

// ARGB -> YUYV
void TPCvtARGB2YUYVLine(OUT BYTE* pYUYV,
						IN BYTE* pARGB,
						IN int iPitch,
						IN BYTE* pRGB2YUVCoef)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	__asm
	{
		mov			esi, pARGB;
		mov			edi, pYUYV;
		mov			ecx, iPitch;
		mov			eax, pRGB2YUVCoef;
		movaps		xmm7, [eax];				// VbVg UbUg UrYb YgYr
		prefetchnta [esi];
		test		esi, 0x0f;
		jnz			loop_32;
		test		edi, 0x0f;
		jnz			loop_32;
loop_a32:
		cmp			ecx, 0x20;
		jl			loop_16;
		prefetchnta	[esi + 32];
		movaps		xmm0, [esi];
		movaps		xmm1, [esi + 16];			// BGRA BGRA BGRA BGRA
		psrld		xmm0, 8;
		psrld		xmm1, 8;					// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm2, xmm0;
		movaps		xmm3, xmm1;
		andps		xmm2, g_maskDLB;
		andps		xmm3, g_maskDLB;			// 000R 000R 000R 000R
		packssdw	xmm2, xmm3;
		psllw		xmm2, 6;					// R7R6 R5R4 R3R2 R1R0
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		andps		xmm3, g_maskHW;
		andps		xmm4, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm3, 8;
		psrld		xmm4, 8;
		packssdw	xmm3, xmm4;					// 0G0G 0G0G 0G0G 0G0G
		psllw		xmm3, 6;					// G7G6 G5G4 G3G2 G1G0
		psrld		xmm0, 16;
		psrld		xmm1, 16;					// 000B 000B 000B 000B
		packssdw	xmm0, xmm1;					// 0B0B 0B0B 0B0B 0B0B
		psllw		xmm0, 6;					// B7B6 B5B4 B3B2 B1B0
		pshuflw		xmm1, xmm7, 0x00;
		shufps		xmm1, xmm1, 0x00;			// YrYr YrYr YrYr YrYr
		pmulhw		xmm1, xmm2;					// YR
		pshuflw		xmm4, xmm7, 0x05;
		shufps		xmm4, xmm4, 0x00;			// YgYg YgYg YgYg YgYg
		pmulhw		xmm4, xmm3;					// YG
		paddsw		xmm1, xmm4;					// YR + YG
		pshuflw		xmm4, xmm7, 0x0a;
		shufps		xmm4, xmm4, 0x00;			// YbYb YbYb YbYb YbYb
		pmulhw		xmm4, xmm0;					// YB
		paddsw		xmm1, xmm4;
		paddsw		xmm1, g_maskComp54;
		psrlw		xmm1, 4;					// Y
		pminsw		xmm1, g_maskWord255;
		pmaxsw		xmm1, g_maskWord0;
		pshuflw		xmm2, xmm2, 0xa0;
		pshufhw		xmm2, xmm2, 0xa0;			// R6R6 R4R4 R2R2 R0R0
		pshufd		xmm4, xmm7, 0x09;			// YgYr YgYr UbUg UrYb
		pshuflw		xmm4, xmm4, 0x0d;
		shufps		xmm4, xmm4, 0x00;			// VrUr VrUr VrUr VrUr
		pmulhw		xmm2, xmm4;					// VR UR
		pshuflw		xmm3, xmm3, 0xa0;
		pshufhw		xmm3, xmm3, 0xa0;			// G6G6 G4G4 G2G2 G0G0
		pshufhw		xmm4, xmm7, 0x88;
		shufps		xmm4, xmm4, 0xff;			// VgUg VgUg VgUg VgUg
		pmulhw		xmm3, xmm4;
		paddsw		xmm2, xmm3;					// VR + VG UR + UG
		pshuflw		xmm0, xmm0, 0xa0;
		pshufhw		xmm0, xmm0, 0xa0;			// B6B6 B4B4 B2B2 B0B0
		pshufhw		xmm4, xmm7, 0xdd;
		shufps		xmm4, xmm4, 0xff;			// VbUb VbUb VbUb VbUb
		pmulhw		xmm0, xmm4;					// VB UB
		paddsw		xmm0, xmm2;
		paddsw		xmm0, g_maskComp12854;
		psrlw		xmm0, 4;					// V U
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm0, g_maskWord0;
		psllw		xmm0, 8;
		orps		xmm0, xmm1;					// V6Y7U6Y6 V4Y5U4Y4 V2Y3U2Y2 V0Y1U0Y0
		movntps		[edi], xmm0;
		add			esi, 32;
		add			edi, 16;
		sub			ecx, 32;
		jmp			loop_a32;
loop_32:
		cmp			ecx, 0x20;
		jl			loop_16;
		prefetchnta	[esi + 32];
		movups		xmm0, [esi];
		movups		xmm1, [esi + 16];			// BGRA BGRA BGRA BGRA
		psrld		xmm0, 8;
		psrld		xmm1, 8;					// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm2, xmm0;
		movaps		xmm3, xmm1;
		andps		xmm2, g_maskDLB;
		andps		xmm3, g_maskDLB;			// 000R 000R 000R 000R
		packssdw	xmm2, xmm3;
		psllw		xmm2, 6;					// R7R6 R5R4 R3R2 R1R0
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		andps		xmm3, g_maskHW;
		andps		xmm4, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm3, 8;
		psrld		xmm4, 8;					// 000G 000G 000G 000G
		packssdw	xmm3, xmm4;					// 0G0G 0G0G 0G0G 0G0G
		psrlw		xmm3, 2;					// G7G6 G5G4 G3G2 G1G0
		psrld		xmm0, 16;
		psrld		xmm1, 16;					// 000B 000B 000B 000B
		packssdw	xmm0, xmm1;					// 0B0B 0B0B 0B0B 0B0B
		psllw		xmm0, 6;					// B7B6 B5B4 B3B2 B1B0
		pshuflw		xmm1, xmm7, 0x00;
		shufps		xmm1, xmm1, 0x00;			// YrYr YrYr YrYr YrYr
		pmulhw		xmm1, xmm2;					// YR
		pshuflw		xmm4, xmm7, 0x05;
		shufps		xmm4, xmm4, 0x00;			// YgYg YgYg YgYg YgYg
		pmulhw		xmm4, xmm3;					// YG
		paddsw		xmm1, xmm4;					// YR + YG
		pshuflw		xmm4, xmm7, 0x0a;
		shufps		xmm4, xmm4, 0x00;			// YbYb YbYb YbYb YbYb
		pmulhw		xmm4, xmm0;					// YB
		paddsw		xmm1, xmm4;
		paddsw		xmm1, g_maskComp54;
		psrlw		xmm1, 4;					// Y
		pminsw		xmm1, g_maskWord255;
		pmaxsw		xmm1, g_maskWord0;
		pshuflw		xmm2, xmm2, 0xa0;
		pshufhw		xmm2, xmm2, 0xa0;			// R6R6 R4R4 R2R2 R0R0
		pshufd		xmm4, xmm7, 0x09;			// YgYr YgYr UbUg UrYb
		pshuflw		xmm4, xmm4, 0x0d;
		shufps		xmm4, xmm4, 0x00;			// VrUr VrUr VrUr VrUr
		pmulhw		xmm2, xmm4;					// VR UR
		pshuflw		xmm3, xmm3, 0xa0;
		pshufhw		xmm3, xmm3, 0xa0;			// G6G6 G4G4 G2G2 G0G0
		pshufhw		xmm4, xmm7, 0x88;
		shufps		xmm4, xmm4, 0xff;			// VgUg VgUg VgUg VgUg
		pmulhw		xmm3, xmm4;
		paddsw		xmm2, xmm3;					// VR + VG UR + UG
		pshuflw		xmm0, xmm0, 0xa0;
		pshufhw		xmm0, xmm0, 0xa0;			// B6B6 B4B4 B2B2 B0B0
		pshufhw		xmm4, xmm7, 0xdd;
		shufps		xmm4, xmm4, 0xff;			// VbUb VbUb VbUb VbUb
		pmulhw		xmm0, xmm4;					// VB UB
		paddsw		xmm0, xmm2;
		paddsw		xmm0, g_maskComp12854;
		psrlw		xmm0, 4;					// V U
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm0, g_maskWord0;
		psllw		xmm0, 8;
		orps		xmm0, xmm1;					// V6Y7U6Y6 V4Y5U4Y4 V2Y3U2Y2 V0Y1U0Y0
		movups		[edi], xmm0;
		add			esi, 32;
		add			edi, 16;
		sub			ecx, 32;
		jmp			loop_32;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_12;
		movups		xmm0, [esi];				// BGRA BGRA BGRA BGRA
		psrld		xmm0, 8;					// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm1, xmm0;
		andps		xmm1, g_maskLW;				// 0B0R 0B0R 0B0R 0B0R
		movaps		xmm2, xmm0;
		andps		xmm2, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm2, 8;					// 000G 000G 000G 000G
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		pshuflw		xmm4, xmm7, 0x08;
		shufps		xmm4, xmm4, 0x00;			// YbYr YbYr YbYr YbYr
		pshuflw		xmm5, xmm7, 0x05;
		shufps		xmm5, xmm5, 0x00;			// YgYg YgYg YgYg YgYg
		pmaddwd		xmm1, xmm4;					// YR + YB
		pmaddwd		xmm3, xmm5;					// YG
		shufps		xmm0, xmm0, 0xa0;
		shufps		xmm2, xmm2, 0xa0;
		pshufd		xmm4, xmm7, 0xe9;			// VbVg UbUg UbUg UrYb
		pshuflw		xmm4, xmm4, 0x0d;
		pshufhw		xmm4, xmm4, 0xd0;
		shufps		xmm4, xmm4, 0xcc;			// VbVr UbUr VbVr UbUr
		pshufd		xmm5, xmm7, 0xa0;			// VgVg UgUg VgVg UgUg
		pmaddwd		xmm0, xmm4;
		pmaddwd		xmm2, xmm5;
		paddd		xmm1, xmm3;
		paddd		xmm0, xmm2;
		paddd		xmm1, g_maskComp128514;
		paddd		xmm0, g_maskComp128514;
		psrlw		xmm1, 14;
		psrlw		xmm0, 14;
		packssdw	xmm1, xmm1;
		packssdw	xmm0, xmm0;
		pminsw		xmm1, g_maskWord255;
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm1, g_maskWord0;
		pmaxsw		xmm0, g_maskWord0;
		psllw		xmm0, 8;
		orps		xmm0, xmm1;					// V2Y3U2Y2 V0Y1U0Y0 V2Y3U2Y2 V0Y1U0Y0
		movsd		[edi], xmm0;
		add			esi, 16;
		add			edi, 8;
		sub			ecx, 16;
		jz			loop_end;
loop_12:
		cmp			ecx, 0x0c;
		jl			loop_8;
		movss		xmm0, [esi];
		movhpd		xmm0, [esi + 4];			// BGRA BGRA 0000 BGRA
		shufps		xmm0, xmm0, 0xf8;			// BGRA BGRA BGRA BGRA
		psrld		xmm0, 8;					// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm1, xmm0;
		andps		xmm1, g_maskLW;				// 0B0R 0B0R 0B0R 0B0R
		movaps		xmm2, xmm0;
		andps		xmm2, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm2, 8;					// 000G 000G 000G 000G
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		pshuflw		xmm4, xmm7, 0x08;
		shufps		xmm4, xmm4, 0x00;			// YbYr YbYr YbYr YbYr
		pshuflw		xmm5, xmm7, 0x05;
		shufps		xmm5, xmm5, 0x00;			// YgYg YgYg YgYg YgYg
		pmaddwd		xmm1, xmm4;					// YR + YB
		pmaddwd		xmm3, xmm5;					// YG
		shufps		xmm0, xmm0, 0xa0;
		shufps		xmm2, xmm2, 0xa0;
		pshufd		xmm4, xmm7, 0xe9;			// VbVg UbUg UbUg UrYb
		pshuflw		xmm4, xmm4, 0x0d;
		pshufhw		xmm4, xmm4, 0xd0;
		shufps		xmm4, xmm4, 0xcc;			// VbVr UbUr VbVr UbUr
		pshufd		xmm5, xmm7, 0xa0;			// VgVg UgUg VgVg UgUg
		pmaddwd		xmm0, xmm4;
		pmaddwd		xmm2, xmm5;
		paddd		xmm1, xmm3;
		paddd		xmm0, xmm2;
		paddd		xmm1, g_maskComp128514;
		paddd		xmm0, g_maskComp128514;
		psrlw		xmm1, 14;
		psrlw		xmm0, 14;
		packssdw	xmm1, xmm1;
		packssdw	xmm0, xmm0;
		pminsw		xmm1, g_maskWord255;
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm1, g_maskWord0;
		pmaxsw		xmm0, g_maskWord0;
		psllw		xmm0, 8;
		orps		xmm0, xmm1;					// V2Y3U2Y2 V0Y1U0Y0 V2Y3U2Y2 V0Y1U0Y0
		movsd		[edi], xmm0;
		add			esi, 12;
		add			edi, 8;
		sub			ecx, 12;
		jz			loop_end;
loop_8:
		cmp			ecx, 8;
		jl			loop_4;
		movsd		xmm0, [esi];				// 0000 0000 BGRA BGRA
		psrld		xmm0, 8;					// 0000 0000 0BGR 0BGR
		shufps		xmm0, xmm0, 0x11;			// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;				// 0B0R 0B0R 0B0R 0B0R
		pshuflw		xmm2, xmm7, 0x38;			// VbVg UbUg YrUr YbYr
		pshufhw		xmm2, xmm2, 0xd4;			// VbUb UbUg YrUr YbYr
		shufps		xmm2, xmm2, 0xc9;			// VbUb YbYr UbUg YrUr
		pshuflw		xmm2, xmm2, 0xc0;
		shufps		xmm2, xmm2, 0xe6;			// VbUb YbYr UbUr YbYr
		pmaddwd		xmm0, xmm2;
		andps		xmm1, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm1, 8;					// 000G 000G 000G 000G
		pshufhw		xmm2, xmm7, 0xa0;			// VbVb UgUg UrYb YgYr
		pshuflw		xmm2, xmm2, 0x55;			// VbVb UgUg YgYg YgYg
		shufps		xmm2, xmm2, 0xc8;			// VbVb YgYg UgUg YgYg
		andps		xmm2, g_maskLD;				// 00Vb 00Yg 00Yg 00Yg
		pmaddwd		xmm1, xmm2;
		paddd		xmm0, xmm1;					// V0 Y1 U0 Y0
		paddd		xmm0, g_maskComp128514;
		psrld		xmm0, 14;
		packssdw	xmm0, xmm0;
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm0, g_maskWord0;
		packuswb	xmm0, xmm0;
		movss		[edi], xmm0;
		add			esi, 8;
		add			edi, 4;
		sub			ecx, 8;
		jz			loop_end;
loop_4:
		cmp			ecx, 4;
		jl			loop_end;
		movss		xmm0, [esi];				// 0000 0000 0000 BGRA
		psllq		xmm0, 8;					// 0000 0000 0000 0BGR
		shufps		xmm0, xmm0, 0x00;			// 0BGR 0BGR 0BGR 0BGR
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;				// 0B0R 0B0R 0B0R 0B0R
		pshuflw		xmm2, xmm7, 0x38;			// VbVg UbUg YrUr YbYr
		pshufhw		xmm2, xmm2, 0xd4;			// VbUb UbUg YrUr YbYr
		shufps		xmm2, xmm2, 0xc9;			// VbUb YbYr UbUg YrUr
		pshuflw		xmm2, xmm2, 0xc0;
		shufps		xmm2, xmm2, 0xe6;			// VbUb YbYr UbUr YbYr
		pmaddwd		xmm0, xmm2;
		andps		xmm1, g_maskHW;				// 00G0 00G0 00G0 00G0
		psrld		xmm1, 8;					// 000G 000G 000G 000G
		pshufhw		xmm2, xmm7, 0xa0;			// VbVb UgUg UrYb YgYr
		pshuflw		xmm2, xmm2, 0x55;			// VbVb UgUg YgYg YgYg
		shufps		xmm2, xmm2, 0xc8;			// VbVb YgYg UgUg YgYg
		andps		xmm2, g_maskLD;				// 00Vb 00Yg 00Yg 00Yg
		pmaddwd		xmm1, xmm2;
		paddd		xmm0, xmm1;					// V0 Y1 U0 Y0
		paddd		xmm0, g_maskComp128514;
		psrld		xmm0, 14;
		packssdw	xmm0, xmm0;
		pminsw		xmm0, g_maskWord255;
		pmaxsw		xmm0, g_maskWord0;
		packuswb	xmm0, xmm0;
		movss		[edi], xmm0;
loop_end:
		sfence
	}
}

void TPCvtARGB2YUYVFrame(OUT BYTE* pYUYV,
						 IN BYTE* pARGB,
						 IN int iPitch,
						 IN int iWidth,
						 IN int iHeight,
						 IN bool bHD,
						 IN bool bReverse)				// 翻转标志
{
	int iYUYVPitch = iWidth << 1;
	int iARGBPitch = iWidth << 2;

	if(bReverse)
	{
		pARGB += (iHeight - 1) * iWidth * 4;
		iARGBPitch = -iARGBPitch;
	}

	if(bHD)
	{
		for(int i = 0; i < iHeight; ++i)
		{
			TPCvtARGB2YUYVLine(pYUYV, pARGB, iPitch, (BYTE *)g_maskRGB2YUVHD);
			pYUYV += iYUYVPitch;
			pARGB += iARGBPitch;
		}
	}
	else
	{
		for(int i = 0; i < iHeight; ++i)
		{
			TPCvtARGB2YUYVLine(pYUYV, pARGB, iPitch, (BYTE *)g_maskRGB2YUVSD);
			pYUYV += iYUYVPitch;
			pARGB += iARGBPitch;
		}
	}
}

// YUYV -> ARGB
void TPCvtYUYV2ARGBLine(OUT BYTE* pARGB,
						IN BYTE* pYUYV,
						IN int iPitch,
						IN BYTE* pYUV2RGBCoef)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	__asm
	{
		mov			edi, pARGB;
		mov			esi, pYUYV;
		mov			ecx, iPitch;
		mov			eax, pYUV2RGBCoef;			// BvBu ByGv GuRv RuRy
		movaps		xmm7, [eax];
		prefetchnta	[esi];
		test		esi, 0x0f;
		jnz			loop_16_plus;
		test		edi, 0x0f;
		jnz			loop_16_plus;
loop_a16_plus:
		cmp			ecx, 0x10;
		jle			loop_16;
		prefetchnta [esi + 16];
		movaps		xmm0, [esi];				// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y7Y6 Y5Y4 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U6U6 U4U4 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V6V6 V4V4 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		movss		xmm5, [esi + 16];			// 0000 0000 0000 VYUY
		psrldq		xmm3, 2;					// 00U6 U6U4 U4U2 U2U0
		psrldq		xmm4, 2;					// 00V6 V6V4 V4V2 V2V0
		psrlw		xmm5, 8;					// 0000 0000 0000 V8U8
		movaps		xmm6, xmm5;
		pslldq		xmm5, 14;					// U800 0000 0000 0000
		orps		xmm3, xmm5;					// U8U6 U6U4 U4U2 U2U0
		pavgw		xmm1, xmm3;					// U7U6 U5U4 U3U2 U1U0
		psrldq		xmm6, 2;
		pslldq		xmm6, 14;
		orps		xmm4, xmm6;					// V8V6 V6V4 V4V2 V2V0
		pavgw		xmm2, xmm4;					// V7V6 V5V4 V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm1;
		paddsw		xmm5, xmm0;
		paddsw		xmm5, xmm6;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// R7R6 R5R4 R3R2 R1R0
		pminsw		xmm4, xmm6;					// B7B6 B5B4 B3B2 B1B0
		pminsw		xmm5, xmm6;					// G7G6 G5G4 G3G2 G1G0
		psllw		xmm3, 8;
		orps		xmm3, g_maskLW;				// RARA RARA RARA RARA
		movaps		xmm2, xmm3;
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// BGBG BGBG BGBG BGBG
		punpcklwd	xmm2, xmm4;					// BGRA BGRA BGRA BGRA
		punpckhwd	xmm3, xmm4;					// BGRA BGRA BGRA BGRA
		movntps		[edi], xmm2;
		movntps		[edi + 16], xmm3;
		add			esi, 0x10;
		add			edi, 0x20;
		sub			ecx, 0x10;
		jmp			loop_a16_plus;
loop_16_plus:
		cmp			ecx, 0x10;
		jle			loop_16;
		prefetchnta [esi + 16];
		movups		xmm0, [esi];				// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y7Y6 Y5Y4 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U6U6 U4U4 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V6V6 V4V4 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		movss		xmm5, [esi + 16];			// 0000 0000 0000 VYUY
		psrldq		xmm3, 2;					// 00U6 U6U4 U4U2 U2U0
		psrldq		xmm4, 2;					// 00V6 V6V4 V4V2 V2V0
		psrlw		xmm5, 8;					// 0000 0000 0000 V8U8
		movaps		xmm6, xmm5;
		pslldq		xmm5, 14;					// U800 0000 0000 0000
		orps		xmm3, xmm5;					// U8U6 U6U4 U4U2 U2U0
		pavgw		xmm1, xmm3;					// U7U6 U5U4 U3U2 U1U0
		psrldq		xmm6, 2;
		pslldq		xmm6, 14;
		orps		xmm4, xmm6;					// V8V6 V6V4 V4V2 V2V0
		pavgw		xmm2, xmm4;					// V7V6 V5V4 V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm1;
		paddsw		xmm5, xmm0;
		paddsw		xmm5, xmm6;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// R7R6 R5R4 R3R2 R1R0
		pminsw		xmm4, xmm6;					// B7B6 B5B4 B3B2 B1B0
		pminsw		xmm5, xmm6;					// G7G6 G5G4 G3G2 G1G0
		psllw		xmm3, 8;
		orps		xmm3, g_maskLW;				// RARA RARA RARA RARA
		movaps		xmm2, xmm3;
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// BGBG BGBG BGBG BGBG
		punpcklwd	xmm2, xmm4;					// BGRA BGRA BGRA BGRA
		punpckhwd	xmm3, xmm4;					// BGRA BGRA BGRA BGRA
		movups		[edi], xmm2;
		movups		[edi + 16], xmm3;
		add			esi, 0x10;
		add			edi, 0x20;
		sub			ecx, 0x10;
		jmp			loop_16_plus;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_12;
		movups		xmm0, [esi];				// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y7Y6 Y5Y4 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U6U6 U4U4 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V6V6 V4V4 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrldq		xmm3, 2;					// 00U6 U6U4 U4U2 U2U0
		psrldq		xmm4, 2;					// 00V6 V6V4 V4V2 V2V0
		pshufhw		xmm3, xmm3, 0xa4;
		pshufhw		xmm4, xmm4, 0xa4;
		pavgw		xmm1, xmm3;					// U7U6 U5U4 U3U2 U1U0
		pavgw		xmm2, xmm4;					// V7V6 V5V4 V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm1;
		paddsw		xmm5, xmm0;
		paddsw		xmm5, xmm6;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// R7R6 R5R4 R3R2 R1R0
		pminsw		xmm4, xmm6;					// B7B6 B5B4 B3B2 B1B0
		pminsw		xmm5, xmm6;					// G7G6 G5G4 G3G2 G1G0
		psllw		xmm3, 8;
		orps		xmm3, g_maskLW;				// RARA RARA RARA RARA
		movaps		xmm2, xmm3;
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// BGBG BGBG BGBG BGBG
		punpcklwd	xmm2, xmm4;					// BGRA BGRA BGRA BGRA
		punpckhwd	xmm3, xmm4;					// BGRA BGRA BGRA BGRA
		movups		[edi], xmm2;
		movups		[edi + 16], xmm3;
		add			esi, 0x10;
		add			edi, 0x20;
		sub			ecx, 0x10;
		jz			loop_end;
loop_12:
		cmp			ecx, 12;
		jl			loop_8;
		movss		xmm0, [esi];
		movhps		xmm0, [esi + 4];
		shufps		xmm0, xmm0, 0xf8;			// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y5Y4 Y5Y4 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U4U4 U4U4 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V4V4 V4V4 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrldq		xmm3, 2;					// 00U4 U4U4 U4U2 U2U0
		psrldq		xmm4, 2;					// 00V4 V4V4 V4V2 V2V0
		pavgw		xmm1, xmm3;					// **** U5U4 U3U2 U1U0
		pavgw		xmm2, xmm4;					// **** V5V4 V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm2;
		paddsw		xmm5, xmm6;
		paddsw		xmm5, xmm0;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// **** R5R4 R3R2 R1R0
		pminsw		xmm4, xmm6;					// **** B5B4 B3B2 B1B0
		pminsw		xmm5, xmm6;					// **** G5G4 G3G2 G1G0
		psllw		xmm3, 8;
		orps		xmm3, g_maskLW;				// **** RARA RARA RARA
		movaps		xmm2, xmm3;
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// **** BGBG BGBG BGBG
		punpcklwd	xmm2, xmm4;					// BGRA BGRA BGRA BGRA
		punpckhwd	xmm3, xmm4;					// **** **** BGRA BGRA
		movups		[edi], xmm2;
		movsd		[edi + 16], xmm3;
		add			esi, 12;
		add			edi, 24;
		sub			ecx, 12;
		jz			loop_end;
loop_8:
		cmp			ecx, 8;
		jl			loop_4;
		movsd		xmm0, [esi];
		shufps		xmm0, xmm0, 0x54;			// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y3Y2 Y3Y2 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U2U2 U2U2 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V2V2 V2V2 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrldq		xmm3, 2;					// 00U2 U2U2 U2U2 U2U0
		psrldq		xmm4, 2;					// 00V2 V2V2 V2V2 V2V0
		pavgw		xmm1, xmm3;					// **** **** U3U2 U1U0
		pavgw		xmm2, xmm4;					// **** **** V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm2;
		paddsw		xmm5, xmm6;
		paddsw		xmm5, xmm0;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// **** **** R3R2 R1R0
		pminsw		xmm4, xmm6;					// **** **** B3B2 B1B0
		pminsw		xmm5, xmm6;					// **** **** G3G2 G1G0
		psllw		xmm3, 8;
		orps		xmm3, g_maskLW;				// **** **** RARA RARA
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// **** **** BGBG BGBG
		punpcklwd	xmm3, xmm4;					// BGRA BGRA BGRA BGRA
		movups		[edi], xmm2;
		movsd		[edi + 16], xmm3;
		add			esi, 8;
		add			edi, 16;
		sub			ecx, 8;
		jz			loop_end;
loop_4:
		cmp			ecx, 4;
		jl			loop_end;
		movss		xmm0, [esi];
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// 0000 0000 0000 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0000 0000 0000 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm2;
		paddsw		xmm5, xmm6;
		paddsw		xmm5, xmm0;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// **** **** **** R1R0
		pminsw		xmm4, xmm6;					// **** **** **** B1B0
		pminsw		xmm5, xmm6;					// **** **** **** G1G0
		psllw		xmm3, 8;
		orps		xmm3, g_maskLW;				// **** **** **** RARA
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// **** **** **** BGBG
		punpcklwd	xmm3, xmm4;					// **** **** BGRA BGRA
		movsd		[edi], xmm2;
loop_end:
		sfence
	}
}

void TPCvtYUYV2ARGBFrame(OUT BYTE* pARGB,
						 IN BYTE* pYUYV,
						 IN int iPitch,
						 IN int iWidth,
						 IN int iHeight,
						 IN bool bHD,
						 IN bool bReverse)
{
	if(!pARGB || !pYUYV || iPitch <= 0 || iWidth <= 0 || iHeight <= 0)
	{
		return;
	}

	int iARGBPitch = iWidth << 2;
	int iYUYVPitch = iWidth << 1;
	
	if(bReverse)
	{
		pYUYV += (iHeight - 1) * iYUYVPitch;
		iYUYVPitch = -iYUYVPitch;
	}

	if(bHD)
	{
		for(int i = 0; i < iHeight; ++i)
		{
			TPCvtYUYV2ARGBLine(pARGB, pYUYV, iPitch, (BYTE *)g_maskYUV2RGBHD);
			pARGB += iARGBPitch;
			pYUYV += iYUYVPitch;
		}
	}
	else
	{
		for(int i = 0; i < iHeight; ++i)
		{
			TPCvtYUYV2ARGBLine(pARGB, pYUYV, iPitch, (BYTE *)g_maskYUV2RGBSD);
			pARGB += iARGBPitch;
			pYUYV += iYUYVPitch;
		}
	}
}

// YUYVA -> ARGB
void TPCvtYUYVA2ARGBLine(OUT BYTE* pARGB,
						 IN BYTE* pYUYV,
						 IN BYTE* pAlpha,
						 IN int iPitch,
						 IN BYTE* pYUV2RGBCoef)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	__asm
	{
		mov			edi, pARGB;
		mov			esi, pYUYV;
		mov			ecx, iPitch;
		mov			edx, pAlpha;
		mov			eax, pYUV2RGBCoef;			// BvBu ByGv GuRv RuRy
		movaps		xmm7, [eax];
		prefetchnta	[esi];
		test		esi, 0x0f;
		jnz			loop_16_plus;
		test		edi, 0x0f;
		jnz			loop_16_plus;
loop_a16_plus:
		cmp			ecx, 0x10;
		jle			loop_16;
		prefetchnta [esi + 16];
		movaps		xmm0, [esi];				// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y7Y6 Y5Y4 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U6U6 U4U4 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V6V6 V4V4 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		movss		xmm5, [esi + 16];			// 0000 0000 0000 VYUY
		psrldq		xmm3, 2;					// 00U6 U6U4 U4U2 U2U0
		psrldq		xmm4, 2;					// 00V6 V6V4 V4V2 V2V0
		psrlw		xmm5, 8;					// 0000 0000 0000 V8U8
		movaps		xmm6, xmm5;
		pslldq		xmm5, 14;					// U800 0000 0000 0000
		orps		xmm3, xmm5;					// U8U6 U6U4 U4U2 U2U0
		pavgw		xmm1, xmm3;					// U7U6 U5U4 U3U2 U1U0
		psrldq		xmm6, 2;
		pslldq		xmm6, 14;
		orps		xmm4, xmm6;					// V8V6 V6V4 V4V2 V2V0
		pavgw		xmm2, xmm4;					// V7V6 V5V4 V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm1;
		paddsw		xmm5, xmm0;
		paddsw		xmm5, xmm6;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// R7R6 R5R4 R3R2 R1R0
		pminsw		xmm4, xmm6;					// B7B6 B5B4 B3B2 B1B0
		pminsw		xmm5, xmm6;					// G7G6 G5G4 G3G2 G1G0
		psllw		xmm3, 8;
		movsd		xmm0, [edx];				// 0000 0000 AAAA AAAA
		movhlps		xmm1, xmm0;					// **** **** 0000 0000
		punpcklbw	xmm0, xmm1;					// 0A0A 0A0A 0A0A 0A0A
		orps		xmm3, xmm0;					// RARA RARA RARA RARA
		movaps		xmm2, xmm3;
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// BGBG BGBG BGBG BGBG
		punpcklwd	xmm2, xmm4;					// BGRA BGRA BGRA BGRA
		punpckhwd	xmm3, xmm4;					// BGRA BGRA BGRA BGRA
		movntps		[edi], xmm2;
		movntps		[edi + 16], xmm3;
		add			esi, 0x10;
		add			edi, 0x20;
		add			edx, 8;
		sub			ecx, 0x10;
		jmp			loop_a16_plus;
loop_16_plus:
		cmp			ecx, 0x10;
		jle			loop_16;
		prefetchnta [esi + 16];
		movups		xmm0, [esi];				// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y7Y6 Y5Y4 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U6U6 U4U4 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V6V6 V4V4 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		movss		xmm5, [esi + 16];			// 0000 0000 0000 VYUY
		psrldq		xmm3, 2;					// 00U6 U6U4 U4U2 U2U0
		psrldq		xmm4, 2;					// 00V6 V6V4 V4V2 V2V0
		psrlw		xmm5, 8;					// 0000 0000 0000 V8U8
		movaps		xmm6, xmm5;
		pslldq		xmm5, 14;					// U800 0000 0000 0000
		orps		xmm3, xmm5;					// U8U6 U6U4 U4U2 U2U0
		pavgw		xmm1, xmm3;					// U7U6 U5U4 U3U2 U1U0
		psrldq		xmm6, 2;
		pslldq		xmm6, 14;
		orps		xmm4, xmm6;					// V8V6 V6V4 V4V2 V2V0
		pavgw		xmm2, xmm4;					// V7V6 V5V4 V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm1;
		paddsw		xmm5, xmm0;
		paddsw		xmm5, xmm6;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// R7R6 R5R4 R3R2 R1R0
		pminsw		xmm4, xmm6;					// B7B6 B5B4 B3B2 B1B0
		pminsw		xmm5, xmm6;					// G7G6 G5G4 G3G2 G1G0
		psllw		xmm3, 8;
		movsd		xmm0, [edx];				// 0000 0000 AAAA AAAA
		movhlps		xmm1, xmm0;					// **** **** 0000 0000
		punpcklbw	xmm0, xmm1;					// 0A0A 0A0A 0A0A 0A0A
		orps		xmm3, xmm0;					// RARA RARA RARA RARA
		movaps		xmm2, xmm3;
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// BGBG BGBG BGBG BGBG
		punpcklwd	xmm2, xmm4;					// BGRA BGRA BGRA BGRA
		punpckhwd	xmm3, xmm4;					// BGRA BGRA BGRA BGRA
		movups		[edi], xmm2;
		movups		[edi + 16], xmm3;
		add			esi, 0x10;
		add			edi, 0x20;
		add			edx, 8;
		sub			ecx, 0x10;
		jmp			loop_16_plus;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_12;
		movups		xmm0, [esi];				// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y7Y6 Y5Y4 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U6U6 U4U4 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V6V6 V4V4 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrldq		xmm3, 2;					// 00U6 U6U4 U4U2 U2U0
		psrldq		xmm4, 2;					// 00V6 V6V4 V4V2 V2V0
		pshufhw		xmm3, xmm3, 0xa4;
		pshufhw		xmm4, xmm4, 0xa4;
		pavgw		xmm1, xmm3;					// U7U6 U5U4 U3U2 U1U0
		pavgw		xmm2, xmm4;					// V7V6 V5V4 V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm1;
		paddsw		xmm5, xmm0;
		paddsw		xmm5, xmm6;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// R7R6 R5R4 R3R2 R1R0
		pminsw		xmm4, xmm6;					// B7B6 B5B4 B3B2 B1B0
		pminsw		xmm5, xmm6;					// G7G6 G5G4 G3G2 G1G0
		psllw		xmm3, 8;
		movsd		xmm0, [edx];				// 0000 0000 AAAA AAAA
		movhlps		xmm1, xmm0;					// **** **** 0000 0000
		punpcklbw	xmm0, xmm1;					// 0A0A 0A0A 0A0A 0A0A
		orps		xmm3, xmm0;					// RARA RARA RARA RARA
		movaps		xmm2, xmm3;
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// BGBG BGBG BGBG BGBG
		punpcklwd	xmm2, xmm4;					// BGRA BGRA BGRA BGRA
		punpckhwd	xmm3, xmm4;					// BGRA BGRA BGRA BGRA
		movups		[edi], xmm2;
		movups		[edi + 16], xmm3;
		add			esi, 0x10;
		add			edi, 0x20;
		add			edx, 8;
		sub			ecx, 0x10;
		jz			loop_end;
loop_12:
		cmp			ecx, 12;
		jl			loop_8;
		movss		xmm0, [esi];
		movhps		xmm0, [esi + 4];
		shufps		xmm0, xmm0, 0xf8;			// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y5Y4 Y5Y4 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U4U4 U4U4 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V4V4 V4V4 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrldq		xmm3, 2;					// 00U4 U4U4 U4U2 U2U0
		psrldq		xmm4, 2;					// 00V4 V4V4 V4V2 V2V0
		pavgw		xmm1, xmm3;					// **** U5U4 U3U2 U1U0
		pavgw		xmm2, xmm4;					// **** V5V4 V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm2;
		paddsw		xmm5, xmm6;
		paddsw		xmm5, xmm0;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// **** R5R4 R3R2 R1R0
		pminsw		xmm4, xmm6;					// **** B5B4 B3B2 B1B0
		pminsw		xmm5, xmm6;					// **** G5G4 G3G2 G1G0
		psllw		xmm3, 8;
		movss		xmm0, [edx];
		pinsrw		xmm0, [edx + 4], 2;			// 0000 0000 00AA AAAA
		xorps		xmm1, xmm1;
		punpcklbw	xmm0, xmm1;
		orps		xmm3, xmm0;					// **** RARA RARA RARA
		movaps		xmm2, xmm3;
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// **** BGBG BGBG BGBG
		punpcklwd	xmm2, xmm4;					// BGRA BGRA BGRA BGRA
		punpckhwd	xmm3, xmm4;					// **** **** BGRA BGRA
		movups		[edi], xmm2;
		movsd		[edi + 16], xmm3;
		add			esi, 12;
		add			edi, 24;
		add			edx, 6;
		sub			ecx, 12;
		jz			loop_end;
loop_8:
		cmp			ecx, 8;
		jl			loop_4;
		movsd		xmm0, [esi];
		shufps		xmm0, xmm0, 0x54;			// VYUY VYUY VYUY VYUY
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// Y3Y2 Y3Y2 Y3Y2 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0V0U 0V0U 0V0U 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		pshufhw		xmm1, xmm1, 0xa0;			// U2U2 U2U2 U2U2 U0U0
		pshufhw		xmm2, xmm2, 0xf5;			// V2V2 V2V2 V2V2 V0V0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrldq		xmm3, 2;					// 00U2 U2U2 U2U2 U2U0
		psrldq		xmm4, 2;					// 00V2 V2V2 V2V2 V2V0
		pavgw		xmm1, xmm3;					// **** **** U3U2 U1U0
		pavgw		xmm2, xmm4;					// **** **** V3V2 V1V0
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm2;
		paddsw		xmm5, xmm6;
		paddsw		xmm5, xmm0;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// **** **** R3R2 R1R0
		pminsw		xmm4, xmm6;					// **** **** B3B2 B1B0
		pminsw		xmm5, xmm6;					// **** **** G3G2 G1G0
		psllw		xmm3, 8;
		movss		xmm0, [edx];
		xorps		xmm1, xmm1;
		punpcklbw	xmm0, xmm1;
		orps		xmm3, xmm0;				// **** **** RARA RARA
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// **** **** BGBG BGBG
		punpcklwd	xmm3, xmm4;					// BGRA BGRA BGRA BGRA
		movups		[edi], xmm2;
		movsd		[edi + 16], xmm3;
		add			esi, 8;
		add			edi, 16;
		add			edx, 4;
		sub			ecx, 8;
		jz			loop_end;
loop_4:
		cmp			ecx, 4;
		jl			loop_end;
		movss		xmm0, [esi];
		movaps		xmm1, xmm0;
		andps		xmm0, g_maskLW;
		psllw		xmm0, 4;					// 0000 0000 0000 Y1Y0
		paddsw		xmm0, g_maskComp54;
		psrlw		xmm1, 8;					// 0000 0000 0000 0V0U
		pshuflw		xmm2, xmm1, 0xf5;
		pshuflw		xmm1, xmm1, 0xa0;
		movaps		xmm3, g_maskComp1286;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psubsw		xmm1, xmm3;
		psubsw		xmm2, xmm3;
		pshuflw		xmm3, xmm7, 0x0a;
		shufps		xmm3, xmm3, 0x00;			// RvRv RvRv RvRv RvRv
		pshufhw		xmm4, xmm7, 0xa0;
		shufps		xmm4, xmm4, 0xff;			// BuBu BuBu BuBu BuBu
		pshuflw		xmm5, xmm7, 0x0f;
		pshufhw		xmm6, xmm7, 0x00;
		shufps		xmm5, xmm5, 0x00;			// GuGu GuGu GuGu GuGu
		shufps		xmm6, xmm6, 0xff;			// GvGv GvGv GvGv GvGv
		pmulhw		xmm5, xmm1;
		pmulhw		xmm6, xmm2;
		pmulhw		xmm3, xmm2;
		pmulhw		xmm4, xmm2;
		paddsw		xmm5, xmm6;
		paddsw		xmm5, xmm0;
		xorps		xmm6, xmm6;
		paddsw		xmm3, xmm0;
		paddsw		xmm4, xmm0;
		pmaxsw		xmm3, xmm6;
		pmaxsw		xmm4, xmm6;
		pmaxsw		xmm5, xmm6;
		movaps		xmm6, g_maskWord255;
		psrlw		xmm3, 4;
		psrlw		xmm4, 4;
		psrlw		xmm5, 4;
		pminsw		xmm3, xmm6;					// **** **** **** R1R0
		pminsw		xmm4, xmm6;					// **** **** **** B1B0
		pminsw		xmm5, xmm6;					// **** **** **** G1G0
		psllw		xmm3, 8;
		pinsrw		xmm0, [edx], 0;
		xorps		xmm1, xmm1;
		punpcklbw	xmm0, xmm1;
		orps		xmm3, xmm0;					// **** **** **** RARA
		psllw		xmm4, 8;
		orps		xmm4, xmm5;					// **** **** **** BGBG
		punpcklwd	xmm3, xmm4;					// **** **** BGRA BGRA
		movsd		[edi], xmm2;
loop_end:
		sfence
	}

}

void TPCvtYUYVA2ARGBFrame(OUT BYTE* pARGB,
						  IN BYTE* pYUYV,
						  IN BYTE* pAlpha,
						  IN int iPitch,
						  IN int iWidth,
						  IN int iHeight,
						  IN bool bHD,
						  IN bool bReverse)
{
	if(!pARGB || !pYUYV || !pAlpha || iWidth <= 0 || iHeight <= 0)
	{
		return;
	}

	int iARGBPitch = iWidth << 2;
	int iYUYVPitch = iWidth << 1;
	int iAlphaPitch = iWidth;

	if(bReverse)
	{
		pYUYV += (iHeight - 1) * iYUYVPitch;
		iYUYVPitch = -iYUYVPitch;
	}

	if(bHD)
	{
		for(int i = 0; i < iHeight; ++i)
		{
			TPCvtYUYVA2ARGBLine(pARGB, pYUYV, pAlpha, iPitch, (BYTE *)g_maskYUV2RGBHD);
			pARGB += iARGBPitch;
			pYUYV += iYUYVPitch;
			pAlpha += iAlphaPitch;
		}
	}
	else
	{
		for(int i = 0; i < iHeight; ++i)
		{
			TPCvtYUYVA2ARGBLine(pARGB, pYUYV, pAlpha, iPitch, (BYTE *)g_maskYUV2RGBSD);
			pARGB += iARGBPitch;
			pYUYV += iYUYVPitch;
			pAlpha += iAlphaPitch;
		}
	}
}

// 只得到奇场或偶场
void TPSplitFrame2Field(OUT BYTE* pField,
						IN int iFieldPitch,
						IN BYTE* pFrame,
						IN int iFramePitch,
						IN int iHeight,
						IN bool bOddField)				// 奇偶场标志
{
	if(!pField || !pFrame || iFieldPitch <= 0 || iHeight <= 0)
	{
		return;
	}

	if(bOddField)
	{
		for(int i = 0; i < iHeight; i += 2)
		{
			TPMemCpyLine(pField, pFrame, iFieldPitch);
			pField += iFieldPitch;
			pFrame += iFramePitch << 1;
		}
	}
	else
	{
		pFrame += iFramePitch;
		for(int i = 1; i < iHeight; i += 2)
		{
			TPMemCpyLine(pField, pFrame, iFieldPitch);
			pField += iFieldPitch;
			pFrame += iFramePitch << 1;
		}
	}
}

void TPMemSetByteLine(IN OUT void* pucDes,
					  IN int iPitch,
					  IN BYTE byVal)
{
	if(!pucDes || iPitch <= 0)
	{
		return;
	}
	__asm
	{
		mov			edi_ptr, pucDes;
		mov			ecx, iPitch;
		movzx		eax, byVal;
		mov			edx, ecx;
		shr			ecx, 6;
		cld			;
		test		ecx, ecx;
		jz			loop_end;
		movd		xmm0, eax;
		pshuflw		xmm0, xmm0, 0x00;
		packuswb	xmm0, xmm0;
		pshufd		xmm0, xmm0, 0x00;
		test		edi_ptr, 0x0f;
		jz			loop_a64;
		mov			ecx_ptr, edi_ptr;
		and			ecx_ptr, 0x0f;
		neg			ecx;
		add			ecx, 16;
		sub			edx, ecx;
		rep			stosb;
		mov			ecx, edx;
		shr			ecx, 6;
		test		ecx, ecx;
		jz			loop_end;
loop_a64:
		movntdq		[edi_ptr], xmm0;
		movntdq		[edi_ptr + 0x10], xmm0;
		movntdq		[edi_ptr + 0x20], xmm0;
		movntdq		[edi_ptr + 0x30], xmm0;
		add			edi_ptr, 0x40;
		dec			ecx;
		jnz			loop_a64;
		sfence		;
loop_end:
		and			edx, 63;
		mov			ecx, edx;
		and			ecx, 3;
		rep			stosb;
		mov			ecx, edx;
		shr			ecx, 2;
		rep			stosd;
	}
}

void TPMemSetDwordLine(IN OUT void* des,
					   IN int iPitch,
					   IN DWORD val)
{
	if(!des || iPitch <= 0)
	{
		return;
	}
	__asm
	{
		mov			edi_ptr, des;
		mov			ecx, iPitch;
		mov			eax, val;
		mov			edx, ecx;
		cld			;
		shr			ecx, 6;
		test		ecx, ecx;
		jz			loop_end;
		movd		xmm0, eax;
		pshufd		xmm0, xmm0, 0x00;
		test		edi_ptr, 0x0f;
		jz			loop_a64;
		mov			ecx_ptr, edi_ptr;
		and			ecx_ptr, 15;
		neg			ecx;
		add			ecx, 16;
		sub			edx, ecx;
		rep			stosb;
		mov			ecx, edx;
		shr			ecx, 6;
		test		ecx, ecx;
		jz			loop_end;
loop_a64:
		movntdq		[edi_ptr], xmm0;
		movntdq		[edi_ptr + 0x10], xmm0;
		movntdq		[edi_ptr + 0x20], xmm0;
		movntdq		[edi_ptr + 0x30], xmm0;
		add			edi_ptr, 0x40;
		dec			ecx;
		jnz			loop_a64;
		sfence		;
loop_end:
		and			edx, 63;
		mov			ecx, edx;
		and			ecx, 3;
		rep			stosb;
		mov			ecx, edx;
		shr			ecx, 2;
		rep			stosd;
	}
}

void TPMemSetDwordRect(IN OUT void* des,
					   IN int iPitch,
					   IN DWORD val,
					   IN int iWidth,
					   IN int iHeight)
{
	if(!des || iPitch <= 0 || iWidth <= 0 || iHeight <= 0 || iPitch < (iWidth << 2))
	{
		return;
	}
	PBYTE puc = (PBYTE)des;
	iWidth <<= 2;
	for(int i = 0; i < iHeight; ++i)
	{
		TPMemSetDwordLine(puc, iWidth, val);
		puc += iPitch;
	}
}

// 只对图像内区域有效，如果需要处理的区域超出了原图，则超出部分不予处理
void TPMosaic(IN OUT BYTE* pDes,
			  IN int iPitch,							// 一行的字节数
			  IN int iWidth,							// 图像宽高
			  IN int iHeight,
			  IN int iBitCount,							// 16为YUYV，32为BGRA
			  IN TPMosaicParam const& paramMosaic)		// 马赛克结构体参数
{
	if(!pDes || iPitch <= 0 || iWidth <= 0 || iHeight <= 0 || paramMosaic.iWidth <= 0 || paramMosaic.iHeight <= 0)
	{
		return;
	}

	int iX = paramMosaic.iX;
	int iY = paramMosaic.iY;
	int iRectWidth = paramMosaic.iWidth;
	int iRectHeight = paramMosaic.iHeight;

	if(iBitCount == 16)
	{
		iX /= 2;
		iWidth /= 2;
		iRectWidth /= 2;
	}

	RECT rcSrc, rcDst;
	SetRect(&rcSrc, iX, iY, iX + iRectWidth, iY + iRectHeight);
	SetRect(&rcDst, 0, 0, iWidth, iHeight);
	IntersectRect(&rcDst, &rcSrc, &rcDst);

	if(IsRectEmpty(&rcDst))
	{
		return;
	}

	iX = rcDst.left;
	iY = rcDst.top;
	iRectWidth = rcDst.right - rcDst.left;
	iRectHeight = rcDst.bottom - rcDst.top;

	BYTE* pucBegin = pDes + iPitch * iY + iX * 4;
	int iStepH = static_cast<int>(paramMosaic.fBoxX * iRectWidth);
	int iStepV = static_cast<int>(paramMosaic.fBoxY * iRectHeight);
	int iOffsetH = static_cast<int>(paramMosaic.fOffsetX * iStepH);
	int iOffsetV = static_cast<int>(paramMosaic.fOffsetY * iStepV);
	int iCenterX = iRectWidth/2;
	int iCenterY = iRectHeight/2;

	if(!iStepH || !iStepV)
	{
		return;
	}

	switch(paramMosaic.eType)
	{
	case TPMosaicRect:
		for(int i = 0; i < iRectHeight / iStepV; ++i)
		{
			BYTE* pucRect = pucBegin;
			for(int j = 0; j < iRectWidth / iStepH; ++j)
			{
				DWORD dwVal = *(DWORD *)(pucRect + iPitch * iOffsetV + iOffsetH * 4);
				TPMemSetDwordRect(pucRect, iPitch, dwVal, iStepH, iStepV);
				pucRect += iStepH * 4;
			}
			if(iRectWidth % iStepH)
			{
				TPMemSetDwordRect(pucRect, iPitch, *(DWORD *)(pucRect + iPitch * iOffsetV), iRectWidth % iStepH, iStepV);
			}
			pucBegin += iStepV * iPitch;
		}
		if(iRectHeight % iStepV)
		{
			BYTE* pucRect = pucBegin;
			for(int j = 0; j < iRectWidth / iStepH; ++j)
			{
				TPMemSetDwordRect(pucRect, iPitch, *(DWORD *)(pucRect + iOffsetH * 4), iStepH, iRectHeight % iStepV);
				pucRect += iStepH * 4;
			}
			if(iRectWidth % iStepH)
			{
				TPMemSetDwordRect(pucRect, iPitch, *(DWORD *)pucRect, iRectWidth % iStepH, iRectHeight % iStepV);
			}
		}
		break;
	case TPMosaicEllipse:
		for(int i = 0; i < iRectHeight / iStepV; ++i)
		{
			BYTE* pucRect = pucBegin;
			for(int j = 0; j < iStepV; ++j)
			{
				int iUp = i * iStepV + j;
				int iHalf;
				if(iRectWidth == iRectHeight)
					iHalf = (int)sqrt((double)iUp * (iRectHeight - iUp));
				else
				{
					double dbWidth2 = pow((double)iCenterX, 2);
					double dbHeight2 = pow((double)iCenterY, 2);
					iHalf = (int)sqrt((dbWidth2*dbHeight2-dbWidth2*pow((double)iUp-iCenterY,2))/dbHeight2);
				}
				if(iHalf <= 0)
				{
					continue;
				}
				assert(iCenterX >= iHalf);
				BYTE* pucLine = pucRect + j * iPitch;
				BYTE* pucLeft = pucLine + (iCenterX - iHalf) * 4;
				BYTE* pucRight = pucLine + (iCenterX + iHalf) * 4;
				pucRight = pucRight >= pucLine + iPitch ? pucLine + iPitch : pucRight;
				if(pucLeft >= pucLine + iPitch)
				{
					continue;
				}
				for(int m = 0; m < iRectWidth / iStepH; ++m)
				{
					if(pucLine + iStepH * 4 <= pucLeft)
					{
						pucLine += iStepH * 4;
						continue;
					}
					if(pucLine <= pucLeft)
					{
						TPMemSetDwordLine(pucLeft, iStepH*4-(int)(pucLeft-pucLine), *(DWORD*)(pucRect+(m*iStepH+iOffsetH)*4+iOffsetV*iPitch));
					}
					else if(pucLine + iStepH * 4 <= pucRight)
					{
						TPMemSetDwordLine(pucLine, iStepH*4, *(DWORD*)(pucRect+(m*iStepH+iOffsetH)*4+iOffsetV*iPitch));
					}
					else if(pucLine < pucRight)
					{
						TPMemSetDwordLine(pucLine, (int)(pucRight-pucLine), *(DWORD*)(pucRect+(m*iStepH+iOffsetH)*4+iOffsetV*iPitch));
					}
					else
					{
						break;
					}
					pucLine += iStepH * 4;
				}
				if(iRectWidth % iStepH && pucLine < pucRight)
				{
					TPMemSetDwordLine(pucLine, (int)(pucRight - pucLine), *(DWORD*)(pucRect+iRectWidth/iStepH*iStepH*4+iOffsetV*iPitch));
				}
			}
			pucBegin += iStepV * iPitch;
		}
		if(iRectHeight % iStepV)
		{
			BYTE* pucRect = pucBegin;
			for(int j = 0; j < iRectHeight % iStepV; ++j)
			{
				int iUp = iRectHeight / iStepV * iStepV + j + rcDst.top;
				int iHalf;
				if(iRectWidth == iRectHeight)
					iHalf = (int)sqrt((double)iUp * (iRectHeight - iUp));
				else
				{
					double dbWidth2 = pow((double)iCenterX, 2);
					double dbHeight2 = pow((double)iCenterY, 2);
					iHalf = (int)sqrt((dbWidth2*dbHeight2-dbWidth2*pow((double)iUp-iCenterY,2))/dbHeight2);
				}
				if(iHalf <= 0)
				{
					continue;
				}
				assert(iCenterX >= iHalf);
				BYTE* pucLine = pucRect + j * iPitch;
				BYTE* pucLeft = pucLine + (iCenterX - iHalf) * 4;
				BYTE* pucRight = pucLine + (iCenterX + iHalf) * 4;
				pucRight = pucRight >= pucLine + iPitch ? pucLine + iPitch : pucRight;
				if(pucLeft >= pucLine + iPitch)
				{
					continue;
				}
				for(int m = 0; m < iRectWidth / iStepH; ++m)
				{
					if(pucLine + iStepH * 4 <= pucLeft)
					{
						pucLine += iStepH * 4;
						continue;
					}
					if(pucLine <= pucLeft)
					{
						TPMemSetDwordLine(pucLeft, iStepH*4-(int)(pucLeft-pucLine), *(DWORD*)(pucRect+(m*iStepH+iOffsetH)*4));
					}
					else if(pucLine + iStepH * 4 <= pucRight)
					{
						TPMemSetDwordLine(pucLine, iStepH*4, *(DWORD*)(pucRect+(m*iStepH+iOffsetH)*4));
					}
					else if(pucLine < pucRight)
					{
						TPMemSetDwordLine(pucLine, (int)(pucRight-pucLine), *(DWORD*)(pucRect+(m*iStepH+iOffsetH)*4));
					}
					else
					{
						break;
					}
					pucLine += iStepH * 4;
				}
				if(iRectWidth % iStepH && pucLine < pucRight)
				{
					TPMemSetDwordLine(pucLine, (int)(pucRight - pucLine), *(DWORD*)(pucRect+iRectWidth/iStepH*iStepH*4));
				}
			}
		}
		break;
	default:
		break;
	}
}

void TPInitGaussianBlurCoef(IN OUT float* fCoef,
							IN int iCount,
							IN float fTotal)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	__asm
	{
		lea			eax, [fTotal];
		movss		xmm7, [eax];
		mov			edi, fCoef;
		mov			ecx, iCount;
		rcpss		xmm7, xmm7;
		prefetchnta	[edi];
		shufps		xmm7, xmm7, 0x00;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_12;
		prefetchnta	[edi + 0x40];
		movaps		xmm0, [edi];
		movaps		xmm1, [edi + 0x10];
		movaps		xmm2, [edi + 0x20];
		movaps		xmm3, [edi + 0x30];
		mulps		xmm0, xmm7;
		mulps		xmm1, xmm7;
		mulps		xmm2, xmm7;
		mulps		xmm3, xmm7;
		movntps		[edi], xmm0;
		movntps		[edi + 0x10], xmm1;
		movntps		[edi + 0x20], xmm2;
		movntps		[edi + 0x30], xmm3;
		add			edi, 0x40;
		sub			ecx, 0x10;
		jmp			loop_16;
loop_12:
		cmp			ecx, 0x0c;
		jl			loop_8;
		movaps		xmm0, [edi];
		movaps		xmm1, [edi + 0x10];
		movaps		xmm2, [edi + 0x20];
		mulps		xmm0, xmm7;
		mulps		xmm1, xmm7;
		mulps		xmm2, xmm7;
		movntps		[edi], xmm0;
		movntps		[edi + 0x10], xmm1;
		movntps		[edi + 0x20], xmm2;
		add			edi, 0x30;
		sub			ecx, 0x0c;
loop_8:
		cmp			ecx, 8;
		jl			loop_4;
		movaps		xmm0, [edi];
		movaps		xmm1, [edi + 0x10];
		mulps		xmm0, xmm7;
		mulps		xmm1, xmm7;
		movntps		[edi], xmm0;
		movntps		[edi + 0x10], xmm1;
		add			edi, 0x20;
		sub			ecx, 8;
loop_4:
		cmp			ecx, 4;
		jl			loop_2;
		movaps		xmm0, [edi];
		mulps		xmm0, xmm7;
		movntps		[edi], xmm0;
		add			edi, 0x10;
		sub			ecx, 4;
loop_2:
		cmp			ecx, 2;
		jl			loop_1;
		movlps		xmm0, [edi];
		mulps		xmm0, xmm7;
		movlps		[edi], xmm0;
		add			edi, 8;
		sub			ecx, 2;
loop_1:
		cmp			ecx, 1;
		jl			loop_0;
		movss		xmm0, [edi];
		mulss		xmm0, xmm7;
		movss		[edi], xmm0;
loop_0:
		sfence
	}
}

void TPBGRAGaussianBlurRow(OUT BYTE* pDst,				// 目的
						   IN BYTE* pSrc,				// 源
						   IN int iWidth,				// 源的宽度
						   IN float* fCoef,				// 权重参数
						   IN int iCoef,				// 权重参数个数
						   IN int iBegin,				// 当前行的模糊起点
						   IN int iEnd)					// 当前行的模糊终点
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	int iMin = iCoef >> 1;
	int iMax = iWidth - iMin;
	int iPitch = iMin << 2;
	int iOffset = iBegin << 2;

	__asm
	{
		mov			edi, pDst;
		mov			esi, pSrc;
		mov			edx, fCoef;
		mov			ecx, iBegin;
		add			esi, iOffset;
		prefetchnta	[esi];
loop_left:
		cmp			ecx, iMin;
		jge			loop_mid;
		movss		xmm7, [esi];
		push		esi;
		push		edx;
		push		ecx;
		mov			esi, pSrc;
		mov			eax, iMin;
		sub			eax, ecx;
		shl			eax, 2;
		add			edx, eax;
		add			ecx, iMin;
		inc			ecx;
		xorps		xmm0, xmm0;
		xorps		xmm1, xmm1;
		xorps		xmm2, xmm2;
		psrld		xmm7, 24;
		push		esi;
		push		edx;
		push		ecx;
		mov			eax, ecx;
		sub			eax, iMin;
		shl			eax, 2;
		add			esi, eax;
		mov			eax, ecx;
		shl			eax, 2;
		mov			edx, fCoef;
		add			edx, eax;
		sub			ecx, iCoef;
		neg			ecx;
		mov			eax, 2;
loop_left_4:
		cmp			ecx, 4;
		jl			loop_left_2;
		movups		xmm3, [esi];
		movups		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulps		xmm3, xmm6;
		mulps		xmm4, xmm6;
		mulps		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		add			esi, 0x10;
		add			edx, 0x10;
		sub			ecx, 4;
		jmp			loop_left_4;
loop_left_2:
		movhlps		xmm3, xmm0;
		movhlps		xmm4, xmm1;
		movhlps		xmm5, xmm2;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		cmp			ecx, 2;
		jl			loop_left_1;
		movlps		xmm3, [esi];
		movlps		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		movlhps		xmm6, xmm6;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		movlhps		xmm3, xmm5;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm3, xmm3;
		mulps		xmm4, xmm6;
		mulps		xmm3, xmm6;
		addps		xmm1, xmm4;
		movhlps		xmm5, xmm3;
		addps		xmm0, xmm3;
		addps		xmm2, xmm5;
		add			esi, 8;
		add			edx, 8;
		sub			ecx, 2;
loop_left_1:
		cmp			ecx, 1;
		jl			loop_left_0;
		movss		xmm3, [esi];
		movss		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulss		xmm3, xmm6;
		mulss		xmm4, xmm6;
		mulss		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
loop_left_0:
		dec			eax;
		pop			ecx;
		pop			edx;
		pop			esi;
		movhlps		xmm3, xmm7;
		movlhps		xmm0, xmm3;
		movlhps		xmm1, xmm3;
		movlhps		xmm2, xmm3;
		cmp			eax, 0;
		jg			loop_left_4;
		pslld		xmm7, 24;
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm2;
		psrlq		xmm3, 32;
		psrlq		xmm4, 32;
		psrlq		xmm5, 32;
		addss		xmm0, xmm3;
		addss		xmm1, xmm4;
		addss		xmm2, xmm5;
		cvtps2dq	xmm0, xmm0;
		cvtps2dq	xmm1, xmm1;
		cvtps2dq	xmm2, xmm2;
		pslld		xmm1, 8;
		pslld		xmm2, 16;
		orps		xmm0, xmm1;
		orps		xmm7, xmm2;
		orps		xmm0, xmm7;
		movss		[edi], xmm0;
		add			esi, 4;
		add			edi, 4;
		inc			ecx;
		cmp			ecx, iEnd;
		jge			loop_end;
		jmp			loop_left;
loop_mid:
		cmp			ecx, iMax;
		jge			loop_right;
		movss		xmm7, [esi];
		push		esi;
		push		edx;
		push		ecx;
		sub			esi, iPitch;
		mov			ecx, iCoef;
		xorps		xmm0, xmm0;
		xorps		xmm1, xmm1;
		xorps		xmm2, xmm2;
		psrld		xmm7, 24;
loop_mid_4:
		cmp			ecx, 4;
		jl			loop_mid_2;
		movups		xmm3, [esi];
		movups		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulps		xmm3, xmm6;
		mulps		xmm4, xmm6;
		mulps		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		add			esi, 0x10;
		add			edx, 0x10;
		sub			ecx, 4;
		jmp			loop_mid_4;
loop_mid_2:
		movhlps		xmm3, xmm0;
		movhlps		xmm4, xmm1;
		movhlps		xmm5, xmm2;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		cmp			ecx, 2;
		jl			loop_mid_1;
		movlps		xmm3, [esi];
		movlps		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		movlhps		xmm6, xmm6;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		movlhps		xmm3, xmm5;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm3, xmm3;
		mulps		xmm4, xmm6;
		mulps		xmm3, xmm6;
		addps		xmm1, xmm4;
		movhlps		xmm5, xmm3;
		addps		xmm0, xmm3;
		addps		xmm2, xmm5;
		add			esi, 8;
		add			edx, 8;
		sub			ecx, 2;
loop_mid_1:
		cmp			ecx, 1;
		jl			loop_mid_0;
		movss		xmm3, [esi];
		movss		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulss		xmm3, xmm6;
		mulss		xmm4, xmm6;
		mulss		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
loop_mid_0:
		pslld		xmm7, 24;
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm2;
		psrlq		xmm3, 32;
		psrlq		xmm4, 32;
		psrlq		xmm5, 32;
		addss		xmm0, xmm3;
		addss		xmm1, xmm4;
		addss		xmm2, xmm5;
		cvtps2dq	xmm0, xmm0;
		cvtps2dq	xmm1, xmm1;
		cvtps2dq	xmm2, xmm2;
		pslld		xmm1, 8;
		pslld		xmm2, 16;
		orps		xmm0, xmm1;
		orps		xmm7, xmm2;
		orps		xmm0, xmm7;
		movss		[edi], xmm0;
		pop			ecx;
		pop			edx;
		pop			esi;
		add			esi, 4;
		add			edi, 4;
		inc			ecx;
		cmp			ecx, iEnd;
		jge			loop_end;
		jmp			loop_mid;
loop_right:
		cmp			ecx, iWidth;
		jge			loop_end;
		movss		xmm7, [esi];
		push		esi;
		push		edx;
		push		ecx;
		sub			esi, iPitch;
		sub			ecx, iWidth;
		neg			ecx;
		add			ecx, iMin;
		xorps		xmm0, xmm0;
		xorps		xmm1, xmm1;
		xorps		xmm2, xmm2;
		psrld		xmm7, 24;
		push		esi;
		push		edx;
		push		ecx;
		sub			ecx, iCoef;
		neg			ecx;
		mov			eax, 2;
loop_right_4:
		cmp			ecx, 4;
		jl			loop_right_2;
		movups		xmm3, [esi];
		movups		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulps		xmm3, xmm6;
		mulps		xmm4, xmm6;
		mulps		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		add			esi, 0x10;
		add			edx, 0x10;
		sub			ecx, 4;
		jmp			loop_right_4;
loop_right_2:
		movhlps		xmm3, xmm0;
		movhlps		xmm4, xmm1;
		movhlps		xmm5, xmm2;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		cmp			ecx, 2;
		jl			loop_right_1;
		movlps		xmm3, [esi];
		movlps		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		movlhps		xmm6, xmm6;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		movlhps		xmm3, xmm5;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm3, xmm3;
		mulps		xmm4, xmm6;
		mulps		xmm3, xmm6;
		addps		xmm1, xmm4;
		movhlps		xmm5, xmm3;
		addps		xmm0, xmm3;
		addps		xmm2, xmm5;
		add			esi, 8;
		add			edx, 8;
		sub			ecx, 2;
loop_right_1:
		cmp			ecx, 1;
		jl			loop_right_0;
		movss		xmm3, [esi];
		movss		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulss		xmm3, xmm6;
		mulss		xmm4, xmm6;
		mulss		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
loop_right_0:
		dec			eax;
		pop			ecx;
		pop			edx;
		pop			esi;
		movhlps		xmm3, xmm7;
		movlhps		xmm0, xmm3;
		movlhps		xmm1, xmm3;
		movlhps		xmm2, xmm3;
		cmp			eax, 0;
		jg			loop_right_4;
		pslld		xmm7, 24;
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm2;
		psrlq		xmm3, 32;
		psrlq		xmm4, 32;
		psrlq		xmm5, 32;
		addss		xmm0, xmm3;
		addss		xmm1, xmm4;
		addss		xmm2, xmm5;
		cvtps2dq	xmm0, xmm0;
		cvtps2dq	xmm1, xmm1;
		cvtps2dq	xmm2, xmm2;
		pslld		xmm1, 8;
		pslld		xmm2, 16;
		orps		xmm0, xmm1;
		orps		xmm7, xmm2;
		orps		xmm0, xmm7;
		movss		[edi], xmm0;
		add			esi, 4;
		add			edi, 4;
		inc			ecx;
		cmp			ecx, iEnd;
		jge			loop_end;
		jmp			loop_right;
loop_end:
	}
}

void TPBGRAGaussianBlurCol(OUT BYTE* pDst,				// 目的
						   IN int iDstPitch,			// 目的一行的字节数
						   IN BYTE* pSrc,				// 源
						   IN int iSrcPitch,			// 源一行的字节数
						   IN int iHeight,				// 源的高度
						   IN float* fCoef,				// 权重参数
						   IN int iCoef,				// 权重参数的个数
						   IN int iBegin,				// 当前列的模糊起点
						   IN int iEnd)					// 当前列的模糊终点
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	int iMin = iCoef >> 1;
	int iMax = iHeight - iMin;
	int iOffset = iSrcPitch * iBegin;
	int iPitch2 = iSrcPitch << 1;
	int iPitchMin = iMin * iSrcPitch;

	__asm
	{
		mov			edi, pDst;
		mov			esi, pSrc;
		mov			ecx, iBegin;
		mov			edx, fCoef;
		add			esi, iOffset;
		mov			eax, iSrcPitch;
		prefetchnta	[esi];
loop_left:
		cmp			ecx, iMin;
		jge			loop_mid;
		movss		xmm7, [esi];
		push		esi;
		push		edx;
		push		ecx;
		mov			esi, pSrc;
		sub			ecx, iMin;
		neg			ecx;
		shl			ecx, 2;
		add			edx, ecx;
		shr			ecx, 2;
		sub			ecx, iCoef;
		neg			ecx;
		xorps		xmm0, xmm0;
		xorps		xmm1, xmm1;
		xorps		xmm2, xmm2;
		psrld		xmm7, 24;
		push		esi;
		push		edx;
		push		ecx;
		push		eax;
		mov			eax, ecx;
		sub			eax, iMin;
		mov			ebx, iSrcPitch;
		push		edx;
		mul			ebx;
		pop			edx;
		add			esi, eax;
		mov			eax, ecx;
		shl			eax, 2;
		mov			edx, fCoef;
		add			edx, eax;
		sub			ecx, iCoef;
		neg			ecx;
		pop			eax;
		mov			ebx, 2;
loop_left_4:
		cmp			ecx, 4;
		jl			loop_left_2;
		movss		xmm3, [esi];
		movss		xmm4, [esi + eax];
		add			esi, iPitch2;
		movss		xmm5, [esi];
		movss		xmm6, [esi + eax];
		add			esi, iPitch2;
		shufps		xmm3, xmm5, 0x44;
		shufps		xmm4, xmm6, 0x11;
		movups		xmm6, [edx];
		orps		xmm3, xmm4;
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulps		xmm3, xmm6;
		mulps		xmm4, xmm6;
		mulps		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		add			edx, 0x10;
		sub			ecx, 4;
		jmp			loop_left_4;
loop_left_2:
		cmp			ecx, 2;
		jl			loop_left_1;
		movss		xmm3, [esi];
		movss		xmm4, [esi + eax];
		movlps		xmm6, [edx];
		movlhps		xmm3, xmm4;
		shufps		xmm6, xmm6, 0x50;
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulps		xmm3, xmm6;
		mulps		xmm4, xmm6;
		mulps		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		add			esi, iPitch2;
		add			edx, 8;
		sub			ecx, 2;
loop_left_1:
		cmp			ecx, 1;
		jl			loop_left_0;
		movss		xmm3, [esi];
		movss		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulss		xmm3, xmm6;
		mulss		xmm4, xmm6;
		mulss		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
loop_left_0:
		dec			ebx;
		pop			ecx;
		pop			edx;
		pop			esi;
		cmp			ebx, 0;
		jg			loop_left_4;
		pslld		xmm7, 24;
		movhlps		xmm3, xmm0;
		movhlps		xmm4, xmm1;
		movhlps		xmm5, xmm2;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm2;
		psrlq		xmm3, 32;
		psrlq		xmm4, 32;
		psrlq		xmm5, 32;
		addss		xmm0, xmm3;
		addss		xmm1, xmm4;
		addss		xmm2, xmm5;
		cvtps2dq	xmm0, xmm0;
		cvtps2dq	xmm1, xmm1;
		cvtps2dq	xmm2, xmm2;
		pslld		xmm1, 8;
		pslld		xmm2, 16;
		orps		xmm0, xmm1;
		orps		xmm2, xmm7;
		orps		xmm0, xmm2;
		movss		[edi], xmm0;
		add			edi, iDstPitch;
		add			esi, eax;
		inc			ecx;
		cmp			ecx, iEnd;
		jge			loop_end;
		jmp			loop_left;
loop_mid:
		cmp			ecx, iMax;
		jge			loop_right;
		movss		xmm7, [esi];
		push		esi;
		push		edx;
		push		ecx;
		sub			esi, iPitchMin;
		mov			ecx, iCoef;
		xorps		xmm0, xmm0;
		xorps		xmm1, xmm1;
		xorps		xmm2, xmm2;
		psrld		xmm7, 24;
loop_mid_4:
		cmp			ecx, 4;
		jl			loop_mid_2;
		movss		xmm3, [esi];
		movss		xmm4, [esi + eax];
		add			esi, iPitch2;
		movss		xmm5, [esi];
		movss		xmm6, [esi + eax];
		add			esi, iPitch2;
		shufps		xmm3, xmm5, 0x44;
		shufps		xmm4, xmm6, 0x11;
		movups		xmm6, [edx];
		orps		xmm3, xmm4;
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulps		xmm3, xmm6;
		mulps		xmm4, xmm6;
		mulps		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		add			edx, 0x10;
		sub			ecx, 4;
		jmp			loop_mid_4;
loop_mid_2:
		cmp			ecx, 2;
		jl			loop_mid_1;
		movss		xmm3, [esi];
		movss		xmm4, [esi + eax];
		movlps		xmm6, [edx];
		movlhps		xmm3, xmm4;
		shufps		xmm6, xmm6, 0x50;
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulps		xmm3, xmm6;
		mulps		xmm4, xmm6;
		mulps		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		add			esi, iPitch2;
		add			edx, 8;
		sub			ecx, 2;
loop_mid_1:
		cmp			ecx, 1;
		jl			loop_mid_0;
		movss		xmm3, [esi];
		movss		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulss		xmm3, xmm6;
		mulss		xmm4, xmm6;
		mulss		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
loop_mid_0:
		pslld		xmm7, 24;
		movhlps		xmm3, xmm0;
		movhlps		xmm4, xmm1;
		movhlps		xmm5, xmm2;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm2;
		psrlq		xmm3, 32;
		psrlq		xmm4, 32;
		psrlq		xmm5, 32;
		addss		xmm0, xmm3;
		addss		xmm1, xmm4;
		addss		xmm2, xmm5;
		cvtps2dq	xmm0, xmm0;
		cvtps2dq	xmm1, xmm1;
		cvtps2dq	xmm2, xmm2;
		pslld		xmm1, 8;
		pslld		xmm2, 16;
		orps		xmm0, xmm1;
		orps		xmm2, xmm7;
		orps		xmm0, xmm2;
		movss		[edi], xmm0;
		add			edi, iDstPitch;
		pop			ecx;
		pop			edx;
		pop			esi;
		add			esi, eax;
		inc			ecx;
		cmp			ecx, iEnd;
		jge			loop_end;
		jmp			loop_mid;
loop_right:
		cmp			ecx, iHeight;
		jge			loop_end;
		movss		xmm7, [esi];
		push		esi;
		push		edx;
		push		ecx;
		sub			esi, iPitchMin;
		sub			ecx, iHeight;
		neg			ecx;
		add			ecx, iMin;
		xorps		xmm0, xmm0;
		xorps		xmm1, xmm1;
		xorps		xmm2, xmm2;
		psrld		xmm7, 24;
		push		esi;
		push		edx;
		push		ecx;
		sub			ecx, iCoef;
		neg			ecx;
		mov			ebx, 2;
loop_right_4:
		cmp			ecx, 4;
		jl			loop_right_2;
		movss		xmm3, [esi];
		movss		xmm4, [esi + eax];
		add			esi, iPitch2;
		movss		xmm5, [esi];
		movss		xmm6, [esi + eax];
		add			esi, iPitch2;
		shufps		xmm3, xmm5, 0x44;
		shufps		xmm4, xmm6, 0x11;
		movups		xmm6, [edx];
		orps		xmm3, xmm4;
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulps		xmm3, xmm6;
		mulps		xmm4, xmm6;
		mulps		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		add			edx, 0x10;
		sub			ecx, 4;
		jmp			loop_right_4;
loop_right_2:
		cmp			ecx, 2;
		jl			loop_right_1;
		movss		xmm3, [esi];
		movss		xmm4, [esi + eax];
		movlps		xmm6, [edx];
		movlhps		xmm3, xmm4;
		shufps		xmm6, xmm6, 0x50;
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulps		xmm3, xmm6;
		mulps		xmm4, xmm6;
		mulps		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		add			esi, iPitch2;
		add			edx, 8;
		sub			ecx, 2;
loop_right_1:
		cmp			ecx, 1;
		jl			loop_right_0;
		movss		xmm3, [esi];
		movss		xmm6, [edx];
		movaps		xmm4, xmm3;
		movaps		xmm5, xmm3;
		pslld		xmm3, 24;
		pslld		xmm4, 16;
		pslld		xmm5, 8;
		psrld		xmm3, 24;
		psrld		xmm4, 24;
		psrld		xmm5, 24;
		cvtdq2ps	xmm3, xmm3;
		cvtdq2ps	xmm4, xmm4;
		cvtdq2ps	xmm5, xmm5;
		mulss		xmm3, xmm6;
		mulss		xmm4, xmm6;
		mulss		xmm5, xmm6;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
loop_right_0:
		dec			ebx;
		pop			ecx;
		pop			edx;
		pop			esi;
		cmp			ebx, 0;
		jg			loop_right_4;
		pslld		xmm7, 24;
		movhlps		xmm3, xmm0;
		movhlps		xmm4, xmm1;
		movhlps		xmm5, xmm2;
		addps		xmm0, xmm3;
		addps		xmm1, xmm4;
		addps		xmm2, xmm5;
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm2;
		psrlq		xmm3, 32;
		psrlq		xmm4, 32;
		psrlq		xmm5, 32;
		addss		xmm0, xmm3;
		addss		xmm1, xmm4;
		addss		xmm2, xmm5;
		cvtps2dq	xmm0, xmm0;
		cvtps2dq	xmm1, xmm1;
		cvtps2dq	xmm2, xmm2;
		pslld		xmm1, 8;
		pslld		xmm2, 16;
		orps		xmm0, xmm1;
		orps		xmm2, xmm7;
		orps		xmm0, xmm2;
		movss		[edi], xmm0;
		add			edi, iDstPitch;
		add			esi, iSrcPitch;
		inc			ecx;
		cmp			ecx, iEnd;
		jge			loop_end;
		jmp			loop_right;
loop_end:
	}
}

void TPCvtYUYV2YUVAFrame(OUT void* pDes,
						 IN int iDstPitch,
						 IN void* pSrc,
						 IN int iSrcPitch,
						 IN int iWidth,
						 IN int iHeight)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	if(!pDes || !pSrc || iWidth <= 0 || iHeight <= 0 || iSrcPitch <= 0 || iDstPitch <= 0)
	{
		return;
	}

	__asm
	{
		mov			edi, pDes;
		mov			esi, pSrc;
		mov			ecx, iSrcPitch;
		mov			edx, iHeight;
		xorps		xmm7, xmm7;
		prefetchnta	[esi];
loop_height_1:
		cmp			edx, 0;
		je			loop_end;
		push		esi;
		push		edi;
		push		ecx;
loop_16_plus:
		cmp			ecx, 0x10;
		jle			loop_16;
		prefetchnta	[esi + 0x10];
		movups		xmm0, [esi];
		movss		xmm6, [esi + 0x10];
		movaps		xmm2, xmm0;
		psrlw		xmm6, 8;				// 0000 0000 0000 V8U8
		psllw		xmm0, 8;
		psrlw		xmm2, 8;				// V6U6 V4U4 V2U2 V0U0
		psrlw		xmm0, 8;				// Y7Y6 Y5Y4 Y3Y2 Y1Y0
		movaps		xmm3, xmm2;
		pshuflw		xmm2, xmm2, 0xa0;
		pshuflw		xmm3, xmm3, 0xf5;
		pshufhw		xmm2, xmm2, 0xa0;		// U6U6 U4U4 U2U2 U0U0
		pshufhw		xmm3, xmm3, 0xf5;		// V6V6 V4V4 V2V2 V0V0
		movaps		xmm4, xmm2;
		movaps		xmm5, xmm3;
		movaps		xmm1, xmm6;
		psrldq		xmm2, 2;
		psrldq		xmm3, 2;
		pslldq		xmm1, 14;
		psrld		xmm6, 16;
		orps		xmm2, xmm1;
		pslldq		xmm6, 14;
		pavgw		xmm2, xmm4;				// U7U6 U5U4 U3U2 U1U0
		orps		xmm3, xmm6;
		movaps		xmm1, xmm0;
		pavgw		xmm3, xmm5;				// V7V6 V5V4 V3V2 V1V0
		punpcklwd	xmm0, xmm7;
		punpckhwd	xmm1, xmm7;
		movaps		xmm2, xmm4;
		movaps		xmm3, xmm5;
		punpcklwd	xmm2, xmm7;
		punpcklwd	xmm3, xmm7;
		punpckhwd	xmm4, xmm7;
		punpckhwd	xmm5, xmm7;
		pslld		xmm2, 8;
		pslld		xmm3, 16;
		pslld		xmm4, 8;
		pslld		xmm5, 16;
		orps		xmm0, xmm2;
		orps		xmm1, xmm4;
		orps		xmm0, xmm3;
		orps		xmm1, xmm5;
		movntps		[edi], xmm0;
		movntps		[edi + 0x10], xmm1;
		add			esi, 0x10;
		add			edi, 0x20;
		sub			ecx, 0x10;
		jmp			loop_16_plus;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_12;
		movups		xmm0, [esi];
		movaps		xmm2, xmm0;
		psllw		xmm0, 8;
		psrlw		xmm2, 8;
		psrlw		xmm0, 8;
		movaps		xmm3, xmm2;
		movaps		xmm1, xmm0;
		pshuflw		xmm2, xmm2, 0xa0;
		pshuflw		xmm3, xmm3, 0xf5;
		pshufhw		xmm2, xmm2, 0xa0;
		pshufhw		xmm3, xmm3, 0xf5;
		punpcklwd	xmm0, xmm7;
		punpckhwd	xmm1, xmm7;
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm3;
		psrldq		xmm2, 2;
		psrldq		xmm3, 2;
		pshufhw		xmm2, xmm2, 0xe4;
		pshufhw		xmm3, xmm3, 0xe4;
		pavgw		xmm2, xmm4;
		pavgw		xmm3, xmm5;
		movaps		xmm4, xmm2;
		movaps		xmm5, xmm3;
		punpcklwd	xmm2, xmm7;
		punpcklwd	xmm3, xmm7;
		punpckhwd	xmm4, xmm7;
		punpckhwd	xmm5, xmm7;
		pslld		xmm2, 8;
		pslld		xmm4, 8;
		pslld		xmm3, 16;
		pslld		xmm5, 16;
		orps		xmm0, xmm2;
		orps		xmm1, xmm4;
		orps		xmm0, xmm3;
		orps		xmm1, xmm5;
		movntps		[edi], xmm0;
		movntps		[edi + 0x10], xmm1;
		jmp			loop_height_0;
loop_12:
		cmp			ecx, 0x0c;
		jl			loop_8;
		movlps		xmm0, [esi];
		movss		xmm1, [esi + 8];
		movlhps		xmm0, xmm1;
		movaps		xmm2, xmm0;
		psllw		xmm0, 8;
		psrlw		xmm2, 8;
		psrlw		xmm0, 8;
		movaps		xmm3, xmm2;
		movaps		xmm1, xmm0;
		pshuflw		xmm2, xmm2, 0xa0;
		pshuflw		xmm3, xmm3, 0xf5;
		pshufhw		xmm2, xmm2, 0x00;
		pshufhw		xmm3, xmm3, 0x00;
		movaps		xmm4, xmm2;
		movaps		xmm5, xmm3;
		psrldq		xmm2, 2;
		psrldq		xmm3, 2;
		pavgw		xmm2, xmm4;
		pavgw		xmm3, xmm3;
		punpcklwd	xmm2, xmm7;
		punpcklwd	xmm3, xmm7;
		punpckhwd	xmm4, xmm7;
		punpckhwd	xmm5, xmm7;
		punpcklwd	xmm0, xmm7;
		punpckhwd	xmm1, xmm7;
		orps		xmm0, xmm2;
		orps		xmm1, xmm3;
		orps		xmm0, xmm4;
		orps		xmm1, xmm5;
		movntps		[edi], xmm0;
		movlps		[edi + 0x10], xmm1;
		jmp			loop_height_0;
loop_8:
		cmp			ecx, 8;
		jl			loop_4;
		movlps		xmm0, [esi];
		movlhps		xmm0, xmm0;
		movaps		xmm2, xmm0;
		psllw		xmm0, 8;
		psrlw		xmm2, 8;
		psrlw		xmm0, 8;
		movaps		xmm3, xmm2;
		punpcklwd	xmm0, xmm7;
		pshuflw		xmm2, xmm2, 0xa0;
		pshuflw		xmm3, xmm3, 0xf5;
		pshufhw		xmm2, xmm2, 0xaa;
		pshufhw		xmm3, xmm3, 0xff;
		movaps		xmm4, xmm2;
		movaps		xmm5, xmm3;
		psrldq		xmm2, 2;
		psrldq		xmm3, 3;
		pavgw		xmm2, xmm4;
		pavgw		xmm3, xmm5;
		punpcklwd	xmm2, xmm7;
		punpcklwd	xmm3, xmm7;
		pslld		xmm2, 8;
		pslld		xmm3, 16;
		orps		xmm0, xmm2;
		orps		xmm0, xmm3;
		movntps		[edi], xmm0;
		jmp			loop_height_0;
loop_4:
		cmp			ecx, 4;
		jl			loop_height_0;
		movss		xmm0, [esi];
		punpcklbw	xmm0, xmm7;
		movlhps		xmm0, xmm0;
		pshuflw		xmm0, xmm0, 0xd0;
		pshufhw		xmm0, xmm0, 0xda;
		psrlq		xmm0, 16;
		packuswb	xmm0, xmm0;
		movlps		[edi], xmm0;
loop_height_0:
		pop			ecx;
		pop			edi;
		pop			esi;
		dec			edx;
		add			esi, iSrcPitch;
		add			edi, iDstPitch;
		jmp			loop_height_1;
loop_end:
		sfence
	}
}

void TPCvtYUVA2YUYVFrame(OUT void* pDst,
						 IN int iDstPitch,
						 IN void* pSrc,
						 IN int iSrcPitch,
						 IN int iWidth,
						 IN int iHeight)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	if(!pDst || !pSrc || iWidth <= 0 || iHeight <= 0 || iSrcPitch <= 0 || iDstPitch <= 0)
	{
		return;
	}

	__asm
	{
		mov			esi, pSrc;
		mov			edi, pDst;
		mov			ecx, iSrcPitch;
		mov			edx, iHeight;
		xorps		xmm7, xmm7;
		prefetchnta	[esi];
loop_height_1:
		cmp			edx, 0;
		je			loop_end;
		push		esi;
		push		edi;
		push		ecx;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_8;
		prefetchnta	[esi + 0x10];
		movaps		xmm0, [esi];
		movaps		xmm1, xmm0;
		punpcklbw	xmm0, xmm7;
		punpckhbw	xmm1, xmm7;
		shufps		xmm0, xmm0, 0x64;
		shufps		xmm1, xmm1, 0x64;
		pshufhw		xmm0, xmm0, 0xf8;
		pshufhw		xmm1, xmm1, 0xf8;
		shufps		xmm0, xmm0, 0x08;
		shufps		xmm1, xmm1, 0x08;
		movlhps		xmm0, xmm1;
		packuswb	xmm0, xmm0;
		movlps		[edi], xmm0;
		add			esi, 0x10;
		add			edi, 8;
		sub			ecx, 0x10;
		jmp			loop_16;
loop_8:
		cmp			ecx, 8;
		jl			loop_height_0;
		movlps		xmm0, [esi];
		punpcklbw	xmm0, xmm7;
		shufps		xmm0, xmm0, 0x64;
		pshufhw		xmm0, xmm0, 0xf8;
		shufps		xmm0, xmm0, 0x08;
		packuswb	xmm0, xmm0;
		movss		[edi], xmm0;
loop_height_0:
		pop			ecx;
		pop			edi;
		pop			esi;
		dec			edx;
		add			esi, iSrcPitch;
		add			edi, iDstPitch;
		jmp			loop_height_1;
loop_end:
	}
}

// 只对图像内区域有效，如果需要处理的区域超出了原图，则超出部分不予处理
void TPGaussianBlur(IN OUT BYTE* pucDes,
					IN int iPitch,								// 一行的字节数
					IN int iWidth,								// 图像宽高
					IN int iHeight,
					IN int iBitCount,							// 16为YUYV，32为BGRA
					IN TPGaussianBlurParam const& paramBlur)	// 高斯模糊参数
{
	if(!pucDes || iPitch <= 0 || iWidth <= 0 || iHeight <= 0)
	{
		return;
	}

	if(iBitCount == 16)
	{
		PBYTE pucImage = (PBYTE)_aligned_malloc(iWidth * iHeight * 4, 16);
		TPCvtYUYV2YUVAFrame(pucImage, iPitch << 1, pucDes, iPitch, iWidth, iHeight);
		TPGaussianBlur(pucImage, iPitch << 1, iWidth, iHeight, 32, paramBlur);
		TPCvtYUVA2YUYVFrame(pucDes, iPitch, pucImage, iPitch << 1, iWidth, iHeight);
		_aligned_free(pucImage);
		return;
	}

	int iX = paramBlur.iX;
	int iY = paramBlur.iY;
	int iRectWidth = paramBlur.iWidth;
	int iRectHeight = paramBlur.iHeight;
	int iBluriness = (int)ceilf(paramBlur.fBluriness);

	RECT rcSrc, rcDst;
	SetRect(&rcSrc, iX, iY, iX + iRectWidth, iY + iRectHeight);
	SetRect(&rcDst, 0, 0, iWidth, iHeight);
	IntersectRect(&rcDst, &rcSrc, &rcDst);

	iX = rcDst.left;
	iY = rcDst.top;
	iRectWidth = rcDst.right - rcDst.left;
	iRectHeight = rcDst.bottom - rcDst.top;
	iBluriness = min(iBluriness, iWidth);
	iBluriness = min(iBluriness, iHeight);
	iBluriness = iBluriness % 2 ? iBluriness : iBluriness + 1;

	if(iBluriness < 3 || IsRectEmpty(&rcDst))
	{
		return;
	}

	float fTotal = 1.f;
	float* fCoef = (float *)_aligned_malloc(sizeof(float) * iBluriness, 16);
	int iHalf = iBluriness >> 1;
	fCoef[iHalf] = 1.f;

	for(int i = 1; i <= iHalf; ++i)
	{
		fCoef[iHalf - i] = fCoef[iHalf + i] = expf(-powf(3.f*i/iHalf, 2)/2);
		fTotal += fCoef[iHalf - i] * 2;
	}

	TPInitGaussianBlurCoef(fCoef, iBluriness, fTotal);

	BYTE* pucRect = pucDes + iPitch * iY + iX * 4;
	BYTE* pucTmp = NULL;
	switch(paramBlur.eDimType)
	{
	case TPDimHorizontal:
		pucTmp = (PBYTE)_aligned_malloc(iRectWidth << 2, 16);
		break;
	case TPDimVertical:
		pucTmp = (PBYTE)_aligned_malloc(iRectWidth * iRectHeight * 4, 16);
		break;
	case TPDimBoth:
		pucTmp = (PBYTE)_aligned_malloc(iRectWidth * iRectHeight * 4, 16);
		break;
	}
	if(pucTmp == NULL)
	{
		_aligned_free(fCoef);
		return;
	}
	switch(paramBlur.eShapeType)
	{
	case TPShapeRect:
		switch(paramBlur.eDimType)
		{
		case TPDimHorizontal:
			pucDes += iY * iPitch;
			for(int i = 0; i < iRectHeight; ++i)
			{
				TPBGRAGaussianBlurRow(pucTmp, pucDes, iWidth, fCoef, iBluriness, iX, iX + iRectWidth);
				TPMemCpyLine(pucRect, pucTmp, iRectWidth << 2);
				pucRect += iPitch;
				pucDes += iPitch;
			}
			break;
		case TPDimVertical:
			pucDes += iX * 4;
			for(int i = 0; i < iRectWidth; ++i)
			{
				TPBGRAGaussianBlurCol(pucTmp+i*4, iRectWidth<<2, pucDes, iPitch, iHeight, fCoef, iBluriness, iY, iY+iRectHeight);
				pucDes += 4;
			}
			for(int i = 0; i < iRectHeight; ++i)
			{
				TPMemCpyLine(pucRect, pucTmp + i * iRectWidth * 4, iRectWidth << 2);
				pucRect += iPitch;
			}
			break;
		case TPDimBoth:
			for(int i = 0; i < iRectHeight; ++i)
			{
				TPBGRAGaussianBlurRow(pucTmp, pucDes + (iY + i) * iPitch, iWidth, fCoef, iBluriness, iX, iX + iRectWidth);
				TPMemCpyLine(pucRect + i * iPitch, pucTmp, iRectWidth << 2);
			}
			pucDes += iX * 4;
			for(int i = 0; i < iRectWidth; ++i)
			{
				TPBGRAGaussianBlurCol(pucTmp+i*4, iRectWidth<<2, pucDes, iPitch, iHeight, fCoef, iBluriness, iY, iY + iRectHeight);
				pucDes += 4;
			}
			for(int i = 0; i < iRectHeight; ++i)
			{
				TPMemCpyLine(pucRect, pucTmp + i * iRectWidth * 4, iRectWidth << 2);
				pucRect += iPitch;
			}
			break;
		}
		break;
	case TPShapeEllipse:
		switch(paramBlur.eDimType)
		{
		case TPDimHorizontal:
			pucDes += iY * iPitch;
			for(int i = 0; i < iRectHeight; ++i)
			{
				int iHalf;
				if(iRectWidth == iRectHeight)
					iHalf = (int)sqrtf((float)i * (iRectHeight - i));
				else
				{
					float fWidth2 = powf(iRectWidth/2.f, 2);
					float fHeight2 = powf(iRectHeight/2.f, 2);
					iHalf = (int)sqrtf((fWidth2*fHeight2 - fWidth2*powf(i - iRectHeight/2.f, 2))/fHeight2);
				}
				if(iHalf <= 0)
					continue;
				TPBGRAGaussianBlurRow(pucTmp, pucDes, iWidth, fCoef, iBluriness, iX + iRectWidth/2 - iHalf, iX + iRectWidth/2 + iHalf);
				TPMemCpyLine(pucRect + 4 * (iRectWidth/2 - iHalf), pucTmp, iHalf << 3);
				pucRect += iPitch;
				pucDes += iPitch;
			}
			break;
		case TPDimVertical:
			pucDes += iX * 4;
			for(int i = 0; i < iRectHeight; ++i)
			{
				TPMemCpyLine(pucTmp + i * iRectWidth * 4, pucRect + i * iPitch, iRectWidth << 2);
			}
			for(int i = 0; i < iRectWidth; ++i)
			{
				int iHalf;
				if(iRectWidth == iRectHeight)
					iHalf = (int)sqrtf((float)i * (iRectWidth - i));
				else
				{
					float fWidth2 = powf(iRectWidth/2.f, 2);
					float fHeight2 = powf(iRectHeight/2.f, 2);
					iHalf = (int)sqrtf((fWidth2*fHeight2 - fHeight2*powf(i - iRectWidth/2.f, 2))/fWidth2);
				}
				if(iHalf <= 0)
					continue;
				PBYTE pucTmpCol = pucTmp + i * 4 + (iRectHeight/2 - iHalf) * iRectWidth * 4;
				TPBGRAGaussianBlurCol(pucTmpCol, iRectWidth<<2, pucDes, iPitch, iHeight, fCoef, iBluriness, iY+iRectHeight/2-iHalf, iY+iRectHeight/2+iHalf);
				pucDes += 4;
			}
			for(int i = 0; i < iRectHeight; ++i)
			{
				TPMemCpyLine(pucRect, pucTmp + i * iRectWidth * 4, iRectWidth << 2);
				pucRect += iPitch;
			}
			break;
		case TPDimBoth:
			for(int i = 0; i < iRectHeight; ++i)
			{
				int iHalf;
				if(iRectWidth == iRectHeight)
					iHalf = (int)sqrtf((float)i * (iRectHeight - i));
				else
				{
					float fWidth2 = powf(iRectWidth/2.f, 2);
					float fHeight2 = powf(iRectHeight/2.f, 2);
					iHalf = (int)sqrtf((fWidth2*fHeight2 - fWidth2*powf(i - iRectHeight/2.f, 2))/fHeight2);
				}
				if(iHalf <= 0)
					continue;
				TPBGRAGaussianBlurRow(pucTmp, pucDes+(i+iY)*iPitch, iWidth, fCoef, iBluriness, iX+iRectWidth/2-iHalf, iX+iRectWidth/2+iHalf);
				TPMemCpyLine(pucRect + 4 * (iRectWidth/2 - iHalf) + i * iPitch, pucTmp, iHalf << 3);
			}
			for(int i = 0; i < iRectHeight; ++i)
			{
				TPMemCpyLine(pucTmp + i * iRectWidth * 4, pucRect + i * iPitch, iRectWidth << 2);
			}
			pucDes += iX * 4;
			for(int i = 0; i < iRectWidth; ++i)
			{
				int iHalf;
				if(iRectWidth == iRectHeight)
					iHalf = (int)sqrtf((float)i * (iRectWidth - i));
				else
				{
					float fWidth2 = powf(iRectWidth/2.f, 2);
					float fHeight2 = powf(iRectHeight/2.f, 2);
					iHalf = (int)sqrtf((fWidth2*fHeight2 - fHeight2*powf(i - iRectWidth/2.f, 2))/fWidth2);
				}
				if(iHalf <= 0)
					continue;
				PBYTE pucTmpCol = pucTmp + i * 4 + (iRectHeight/2 - iHalf) * iRectWidth * 4;
				TPBGRAGaussianBlurCol(pucTmpCol, iRectWidth<<2, pucDes, iPitch, iHeight, fCoef, iBluriness, iY+iRectHeight/2-iHalf, iY+iRectHeight/2+iHalf);
				pucDes += 4;
			}
			for(int i = 0; i < iRectHeight; ++i)
			{
				TPMemCpyLine(pucRect, pucTmp + i * iRectWidth * 4, iRectWidth << 2);
				pucRect += iPitch;
			}
			break;
		}
		break;
	}

	_aligned_free(fCoef);
	_aligned_free(pucTmp);
}

// 转为非安全色
//
// RGB = max(0, min(255, (RGB - 16) * 255 / (235 - 16)))
//
void TPBGRACvtNotSafeColor(IN OUT void* pBGRA,
						   IN size_t iByteCount)				// 字节数
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	if(!pBGRA || !iByteCount)
	{
		return;
	}

	__asm
	{
		mov			edi, pBGRA;
		mov			ecx, iByteCount;;
		movaps		xmm7, g_maskNotSafeColor;
		movaps		xmm6, g_maskByte16;
		pcmpeqb		xmm5, xmm5;
		psrlq		xmm7, 16;
		psllq		xmm5, 48;
		psrld		xmm6, 8;
		psrlw		xmm5, 2;
		xorps		xmm4, xmm4;
		orps		xmm7, xmm5;
		movaps		xmm5, g_maskComp54;
		prefetchnta	[edi];
		test		edi, 0x0f;
		jnz			loop_32;
loop_a32:
		cmp			ecx, 0x20;
		jl			loop_16;
		prefetchnta	[edi + 0x20];
		movaps		xmm0, [edi];
		movaps		xmm2, [edi + 0x10];
		psubusb		xmm0, xmm6;
		psubusb		xmm2, xmm6;
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm4;
		punpcklbw	xmm2, xmm4;
		punpckhbw	xmm1, xmm4;
		punpckhbw	xmm3, xmm4;
		psllw		xmm0, 6;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psllw		xmm3, 6;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		pmulhuw		xmm2, xmm7;
		pmulhuw		xmm3, xmm7;
		paddusw		xmm0, xmm5;
		paddusw		xmm1, xmm5;
		paddusw		xmm2, xmm5;
		paddusw		xmm3, xmm5;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		psrlw		xmm2, 4;
		psrlw		xmm3, 4;
		packuswb	xmm0, xmm1;
		packuswb	xmm2, xmm3;
		movntps		[edi], xmm0;
		movntps		[edi + 0x10], xmm2;
		add			edi, 0x20;
		sub			ecx, 0x20;
		jmp			loop_a32;
loop_32:
		cmp			ecx, 0x20;
		jl			loop_16;
		prefetchnta	[edi + 0x20];
		movups		xmm0, [edi];
		movups		xmm2, [edi + 0x10];
		psubusb		xmm0, xmm6;
		psubusb		xmm2, xmm6;
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm4;
		punpcklbw	xmm2, xmm4;
		punpckhbw	xmm1, xmm4;
		punpckhbw	xmm3, xmm4;
		psllw		xmm0, 6;
		psllw		xmm1, 6;
		psllw		xmm2, 6;
		psllw		xmm3, 6;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		pmulhuw		xmm2, xmm7;
		pmulhuw		xmm3, xmm7;
		paddusw		xmm0, xmm5;
		paddusw		xmm1, xmm5;
		paddusw		xmm2, xmm5;
		paddusw		xmm3, xmm5;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		psrlw		xmm2, 4;
		psrlw		xmm3, 4;
		packuswb	xmm0, xmm1;
		packuswb	xmm2, xmm3;
		movups		[edi], xmm0;
		movups		[edi + 0x10], xmm2;
		add			edi, 0x20;
		sub			ecx, 0x20;
		jmp			loop_32;
loop_16:
		cmp			ecx, 0x10;
		jl			loop_12;
		movups		xmm0, [edi];
		psubusb		xmm0, xmm6;
		movaps		xmm1, xmm0;
		punpcklbw	xmm0, xmm4;
		punpckhbw	xmm1, xmm4;
		psllw		xmm0, 6;
		psllw		xmm1, 6;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		paddusw		xmm0, xmm5;
		paddusw		xmm1, xmm5;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		packuswb	xmm0, xmm1;
		movups		[edi], xmm0;
		add			edi, 0x10;
		sub			ecx, 0x10;
		jz			loop_end;
loop_12:
		cmp			ecx, 12;
		jl			loop_8;
		movss		xmm0, [edi];
		movhps		xmm0, [edi + 4];
		psubusb		xmm0, xmm6;
		movaps		xmm1, xmm0;
		punpcklbw	xmm0, xmm4;
		punpckhbw	xmm1, xmm4;
		psllw		xmm0, 6;
		psllw		xmm1, 6;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		paddusw		xmm0, xmm5;
		paddusw		xmm1, xmm5;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		packuswb	xmm0, xmm1;
		movss		[edi], xmm0;
		movhps		[edi + 4], xmm0;
		jmp			loop_end;
loop_8:
		cmp			ecx, 8;
		jl			loop_4;
		movlps		xmm0, [edi];
		psubusb		xmm0, xmm6;
		punpcklbw	xmm0, xmm4;
		psllw		xmm0, 6;
		pmulhuw		xmm0, xmm7;
		paddusw		xmm0, xmm5;
		psrlw		xmm0, 4;
		packuswb	xmm0, xmm0;
		movlps		[edi], xmm0;
		jmp			loop_end;
loop_4:
		cmp			ecx, 4;
		jl			loop_end;
		movss		xmm0, [edi];
		psubusb		xmm0, xmm6;
		punpcklbw	xmm0, xmm4;
		psllw		xmm0, 6;
		pmulhuw		xmm0, xmm7;
		paddusw		xmm0, xmm5;
		psrlw		xmm0, 4;
		packuswb	xmm0, xmm0;
		movss		[edi], xmm0;
loop_end:
		sfence
	}
}

//
// Y = max(0, min(255, (Y - 16) * 255 / (235 - 16)))
// UV = max(0, min(255, (UV - 128) * 255 / (235 - 16) + 128))
//
#include "CoreInclude\CPUInclude\NXColorSpace.h"


void TPYUYVCvtNotSafeColorCPU(IN OUT BYTE* pYUYV,int nWidth,int nHeidht,BYTE* pSrc)
{
	g_NXColorSpace->pStuColorGenericAPI->pfnPackSafeRGB(pYUYV,nWidth*2,pSrc,nWidth*2,NXVIDEO_PIXEL_FMT_YUYV,nWidth,nHeidht,false);

}
void TPYUYVCvtNotSafeColor(IN OUT void* pYUYV,
						   IN size_t iByteCount)				// 字节数
{

	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	if(!pYUYV || !iByteCount)
	{
		return;
	}
	__asm
	{
		mov			esi, pYUYV;
		mov			ecx, iByteCount;
		shr			ecx, 02h;
		mov			edx, ecx;
		shr			edx, 03h;			// 一次处理8个
		and			ecx, 07h;
		movaps		xmm4, g_maskWord16;
		movaps		xmm5, g_maskWord128;
		movaps		xmm6, g_maskComp54;
		movaps		xmm7, g_maskComp12854;
		psrld		xmm4, 10h;
		pslld		xmm5, 10h;
		psrld		xmm6, 10h;
		pslld		xmm7, 10h;
		orps		xmm4, xmm5;
		orps		xmm6, xmm7;
		xorps		xmm5, xmm5;
		movaps		xmm7, g_maskNotSafeColor;
		prefetchnta	[esi];
		test		edx, edx;
		jz			loop_4;
		test		esi, 0fh;
		jnz			loop_8;
loop_a8:
		prefetchnta	[esi + 20h];
		movaps		xmm0, [esi];
		movaps		xmm2, [esi + 10h];
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm5;
		punpcklbw	xmm2, xmm5;
		punpckhbw	xmm1, xmm5;
		punpckhbw	xmm3, xmm5;
		psubsw		xmm0, xmm4;
		psubsw		xmm1, xmm4;
		psubsw		xmm2, xmm4;
		psubsw		xmm3, xmm4;
		psllw		xmm0, 06h;
		psllw		xmm1, 06h;
		psllw		xmm2, 06h;
		psllw		xmm3, 06h;
		pmulhw		xmm0, xmm7;
		pmulhw		xmm1, xmm7;
		pmulhw		xmm2, xmm7;
		pmulhw		xmm3, xmm7;
		paddsw		xmm0, xmm6;
		paddsw		xmm1, xmm6;
		paddsw		xmm2, xmm6;
		paddsw		xmm3, xmm6;
		psraw		xmm0, 04h;
		psraw		xmm1, 04h;
		psraw		xmm2, 04h;
		psraw		xmm3, 04h;
		pmaxsw		xmm0, xmm5;
		pmaxsw		xmm1, xmm5;
		pmaxsw		xmm2, xmm5;
		pmaxsw		xmm3, xmm5;
		packuswb	xmm0, xmm1;
		packuswb	xmm2, xmm3;
		movntps		[esi], xmm0;
		movntps		[esi + 10h], xmm2;
		add			esi, 20h;
		dec			edx;
		jnz			loop_a8;
		sfence
		jmp			loop_4;
loop_8:
		prefetchnta	[esi + 20h];
		movups		xmm0, [esi];
		movups		xmm2, [esi + 10h];
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm5;
		punpcklbw	xmm2, xmm5;
		punpckhbw	xmm1, xmm5;
		punpckhbw	xmm3, xmm5;
		psubsw		xmm0, xmm4;
		psubsw		xmm1, xmm4;
		psubsw		xmm2, xmm4;
		psubsw		xmm3, xmm4;
		psllw		xmm0, 06h;
		psllw		xmm1, 06h;
		psllw		xmm2, 06h;
		psllw		xmm3, 06h;
		pmulhw		xmm0, xmm7;
		pmulhw		xmm1, xmm7;
		pmulhw		xmm2, xmm7;
		pmulhw		xmm3, xmm7;
		paddsw		xmm0, xmm6;
		paddsw		xmm1, xmm6;
		paddsw		xmm2, xmm6;
		paddsw		xmm3, xmm6;
		psraw		xmm0, 04h;
		psraw		xmm1, 04h;
		psraw		xmm2, 04h;
		psraw		xmm3, 04h;
		pmaxsw		xmm0, xmm5;
		pmaxsw		xmm1, xmm5;
		pmaxsw		xmm2, xmm5;
		pmaxsw		xmm3, xmm5;
		packuswb	xmm0, xmm1;
		packuswb	xmm2, xmm3;
		movups		[esi], xmm0;
		movups		[esi + 10h], xmm2;
		add			esi, 20h;
		dec			edx;
		jnz			loop_8;
loop_4:
		test		ecx, 04h;
		jz			loop_2;
		movups		xmm0, [esi];
		movaps		xmm1, xmm0;
		punpcklbw	xmm0, xmm5;
		punpckhbw	xmm1, xmm5;
		psubsw		xmm0, xmm4;
		psubsw		xmm1, xmm4;
		psllw		xmm0, 06h;
		psllw		xmm1, 06h;
		pmulhw		xmm0, xmm7;
		pmulhw		xmm1, xmm7;
		paddsw		xmm0, xmm6;
		paddsw		xmm1, xmm6;
		psraw		xmm0, 04h;
		psraw		xmm1, 04h;
		pmaxsw		xmm0, xmm5;
		pmaxsw		xmm1, xmm5;
		packuswb	xmm0, xmm1;
		movups		[esi], xmm0;
		add			esi, 10h;
		sub			ecx, 04h;
loop_2:
		test		ecx, 02h;
		jz			loop_1;
		movlps		xmm0, [esi];
		punpcklbw	xmm0, xmm5;
		psubsw		xmm0, xmm4;
		psllw		xmm0, 06h;
		pmulhw		xmm0, xmm7;
		paddsw		xmm0, xmm6;
		psraw		xmm0, 04h;
		pmaxsw		xmm0, xmm5;
		packuswb	xmm0, xmm0;
		movlps		[esi], xmm0;
		add			esi, 08h;
		sub			ecx, 02h;
loop_1:
		test		ecx, 01h;
		jz			loop_0;
		movss		xmm0, [esi];
		punpcklbw	xmm0, xmm5;
		psubsw		xmm0, xmm4;
		psllw		xmm0, 06h;
		pmulhw		xmm0, xmm7;
		paddsw		xmm0, xmm6;
		psraw		xmm0, 04h;
		pmaxsw		xmm0, xmm5;
		packuswb	xmm0, xmm0;
		movss		[esi], xmm0;
loop_0:
	}
}

// BGRA -> RGB
void cr32_blend_a_2_cr24_swaprb(OUT void* des,				// 目的
								IN void* src,				// 源
								IN size_t count)			// 像素个数
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	__asm
	{
		mov			edi, des;
		mov			esi, src;
		mov			ecx, count;
		prefetchnta	[esi];
		mov			eax, ecx;
		shr			ecx, 03h;							// 一次处理 8 个像素
		and			eax, 07h;
		movaps		xmm7, g_mask255rcp20;
		movaps		xmm6, g_maskComp54;
		test		ecx, ecx;
		jz			loop_4;
loop_8:
		prefetchnta	[esi + 20h];
		xorps		xmm5, xmm5;
		movups		xmm0, [esi];						// ARGB
		movups		xmm2, [esi + 10h];
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm5;
		punpckhbw	xmm1, xmm5;
		punpcklbw	xmm2, xmm5;
		punpckhbw	xmm3, xmm5;
		pshuflw		xmm4, xmm0, 0ffh;
		pshuflw		xmm5, xmm1, 0ffh;
		pshufhw		xmm4, xmm4, 0ffh;
		pshufhw		xmm5, xmm5, 0ffh;
		pmullw		xmm0, xmm4;
		pmullw		xmm1, xmm5;
		pshuflw		xmm4, xmm2, 0ffh;
		pshuflw		xmm5, xmm3, 0ffh;
		pshufhw		xmm4, xmm4, 0ffh;
		pshufhw		xmm5, xmm5, 0ffh;
		pmullw		xmm2, xmm4;
		pmullw		xmm3, xmm5;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		pmulhuw		xmm2, xmm7;
		pmulhuw		xmm3, xmm7;
		paddw		xmm0, xmm6;
		paddw		xmm1, xmm6;
		paddw		xmm2, xmm6;
		paddw		xmm3, xmm6;
		psrlw		xmm0, 04h;
		psrlw		xmm1, 04h;
		psrlw		xmm2, 04h;
		psrlw		xmm3, 04h;
		pshuflw		xmm0, xmm0, 1bh;
		pshuflw		xmm1, xmm1, 1bh;
		pshuflw		xmm2, xmm2, 1bh;
		pshuflw		xmm3, xmm3, 1bh;					// BGRA
		pshufhw		xmm0, xmm0, 1eh;					// BGAR
		pshufhw		xmm1, xmm1, 0c6h;					// ABGR
		pshufhw		xmm2, xmm2, 1eh;
		pshufhw		xmm3, xmm3, 0c6h;
		pshufd		xmm4, xmm2, 0ffh;
		pshufd		xmm5, xmm0, 0ffh;
		pslldq		xmm3, 02h;
		pslldq		xmm2, 06h;
		pslldq		xmm1, 02h;
		psrldq		xmm0, 02h;
		movss		xmm1, xmm5;							// GRBG
		movss		xmm3, xmm4;							// BGRB GRBG
		movhlps		xmm2, xmm1;							// RBGR BGRB
		movlhps		xmm0, xmm1;							// GRBG RBGR
		packuswb	xmm3, xmm3;
		packuswb	xmm0, xmm2;
		movlps		[edi + 10h], xmm3;
		movups		[edi], xmm0;
		add			esi, 20h;
		add			edi, 18h;
		dec			ecx;
		jnz			loop_8;
		jz			loop_0;
loop_4:
		test		eax, 04h;
		jz			loop_2;
		xorps		xmm2, xmm2;
		movlps		xmm0, [esi];
		movlps		xmm1, [esi + 08h];
		punpcklbw	xmm0, xmm2;
		punpcklbw	xmm1, xmm2;
		pshuflw		xmm2, xmm0, 0ffh;
		pshuflw		xmm3, xmm1, 0ffh;
		pshufhw		xmm2, xmm2, 0ffh;
		pshufhw		xmm3, xmm3, 0ffh;
		pmullw		xmm0, xmm2;
		pmullw		xmm1, xmm3;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		paddw		xmm0, xmm6;
		paddw		xmm1, xmm6;
		psrlw		xmm0, 04h;
		psrlw		xmm1, 04h;
		pshuflw		xmm0, xmm0, 1bh;
		pshuflw		xmm1, xmm1, 1bh;
		pshufhw		xmm0, xmm0, 1eh;
		pshufhw		xmm1, xmm1, 0c6h;
		pshufd		xmm2, xmm0, 0ffh;
		pslldq		xmm1, 02h;
		psrldq		xmm0, 02h;
		movss		xmm1, xmm2;
		packuswb	xmm0, xmm1;
		movss		[edi], xmm0;
		movhps		[edi + 08h], xmm0;
		add			esi, 10h;
		add			edi, 0ch;
loop_2:
		test		eax, 02h;
		jz			loop_1;
		xorps		xmm1, xmm1;
		movlps		xmm0, [esi];
		punpcklbw	xmm0, xmm1;
		pshuflw		xmm1, xmm0, 0ffh;
		pshufhw		xmm1, xmm1, 0ffh;
		pmullw		xmm0, xmm1;
		pmulhuw		xmm0, xmm7;
		paddw		xmm0, xmm6;
		psrlw		xmm0, 04h;
		pshuflw		xmm0, xmm0, 1bh;
		pshufhw		xmm0, xmm0, 0c6h;
		psrldq		xmm0, 02h;
		packuswb	xmm0, xmm0;
		pextrw		edx, xmm0, 02h;
		movss		[edi], xmm0;
		mov			[edi + 04h], edx;
		add			esi, 08h;
		add			edi, 06h;
loop_1:
		test		eax, 01h;
		jz			loop_0;
		xorps		xmm1, xmm1;
		movss		xmm0, [esi];
		punpcklbw	xmm0, xmm1;
		pshuflw		xmm1, xmm0, 0ffh;
		pmullw		xmm0, xmm1;
		pmulhuw		xmm0, xmm7;
		paddw		xmm0, xmm6;
		psrlw		xmm0, 04h;
		pshuflw		xmm0, xmm0, 0c6h;
		packuswb	xmm0, xmm0;
		movd		edx, xmm0;
		mov			[edi], dx;
		shr			edx, 10h;
		mov			[edi + 02h], dl;
loop_0:
	}
}

// BGRA -> BGR
void cr32_blend_a_2_cr24(OUT void* des,						// 目的
						 IN void* src,						// 源
						 IN size_t count)					// 像素个数
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	__asm
	{
		mov			edi, des;
		mov			esi, src;
		mov			ecx, count;
		prefetchnta	[esi];
		mov			eax, ecx;
		shr			ecx, 03h;							// 一次处理 8 个像素
		and			eax, 07h;
		movaps		xmm7, g_mask255rcp20;
		movaps		xmm6, g_maskComp54;
		test		ecx, ecx;
		jz			loop_4;
loop_8:
		prefetchnta	[esi + 20h];
		xorps		xmm5, xmm5;
		movups		xmm0, [esi];						// ARGB
		movups		xmm2, [esi + 10h];
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpcklbw	xmm0, xmm5;
		punpckhbw	xmm1, xmm5;
		punpcklbw	xmm2, xmm5;
		punpckhbw	xmm3, xmm5;
		pshuflw		xmm4, xmm0, 0ffh;
		pshuflw		xmm5, xmm1, 0ffh;
		pshufhw		xmm4, xmm4, 0ffh;
		pshufhw		xmm5, xmm5, 0ffh;
		pmullw		xmm0, xmm4;
		pmullw		xmm1, xmm5;
		pshuflw		xmm4, xmm2, 0ffh;
		pshuflw		xmm5, xmm3, 0ffh;
		pshufhw		xmm4, xmm4, 0ffh;
		pshufhw		xmm5, xmm5, 0ffh;
		pmullw		xmm2, xmm4;
		pmullw		xmm3, xmm5;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		pmulhuw		xmm2, xmm7;
		pmulhuw		xmm3, xmm7;
		paddw		xmm0, xmm6;
		paddw		xmm1, xmm6;
		paddw		xmm2, xmm6;
		paddw		xmm3, xmm6;
		psrlw		xmm0, 04h;
		psrlw		xmm1, 04h;
		psrlw		xmm2, 04h;
		psrlw		xmm3, 04h;
		pshuflw		xmm0, xmm0, 93h;
		pshuflw		xmm1, xmm1, 93h;
		pshuflw		xmm2, xmm2, 93h;
		pshuflw		xmm3, xmm3, 93h;					// RGBA
		pshufhw		xmm0, xmm0, 9ch;					// RGAB
		pshufhw		xmm2, xmm2, 9ch;
		pshufd		xmm4, xmm2, 0ffh;
		pshufd		xmm5, xmm0, 0ffh;
		pslldq		xmm3, 02h;
		pslldq		xmm2, 06h;
		pslldq		xmm1, 02h;
		psrldq		xmm0, 02h;
		movss		xmm1, xmm5;
		movss		xmm3, xmm4;
		movhlps		xmm2, xmm1;
		movlhps		xmm0, xmm1;
		packuswb	xmm3, xmm3;
		packuswb	xmm0, xmm2;
		movlps		[edi + 10h], xmm3;
		movups		[edi], xmm0;
		add			esi, 20h;
		add			edi, 18h;
		dec			ecx;
		jnz			loop_8;
		jz			loop_0;
loop_4:
		test		eax, 04h;
		jz			loop_2;
		xorps		xmm2, xmm2;
		movlps		xmm0, [esi];
		movlps		xmm1, [esi + 08h];
		punpcklbw	xmm0, xmm2;
		punpcklbw	xmm1, xmm2;
		pshuflw		xmm2, xmm0, 0ffh;
		pshuflw		xmm3, xmm1, 0ffh;
		pshufhw		xmm2, xmm2, 0ffh;
		pshufhw		xmm3, xmm3, 0ffh;
		pmullw		xmm0, xmm2;
		pmullw		xmm1, xmm3;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		paddw		xmm0, xmm6;
		paddw		xmm1, xmm6;
		psrlw		xmm0, 04h;
		psrlw		xmm1, 04h;
		pshuflw		xmm0, xmm0, 93h;
		pshuflw		xmm1, xmm1, 93h;
		pshufhw		xmm0, xmm0, 9ch;
		pshufd		xmm2, xmm0, 0ffh;
		pslldq		xmm1, 02h;
		psrldq		xmm0, 02h;
		movss		xmm1, xmm2;
		packuswb	xmm0, xmm1;
		movss		[edi], xmm0;
		movhps		[edi + 08h], xmm0;
		add			esi, 10h;
		add			edi, 0ch;
loop_2:
		test		eax, 02h;
		jz			loop_1;
		xorps		xmm1, xmm1;
		movlps		xmm0, [esi];
		punpcklbw	xmm0, xmm1;
		pshuflw		xmm1, xmm0, 0ffh;
		pshufhw		xmm1, xmm1, 0ffh;
		pmullw		xmm0, xmm1;
		pmulhuw		xmm0, xmm7;
		paddw		xmm0, xmm6;
		psrlw		xmm0, 04h;
		pshuflw		xmm0, xmm0, 93h;
		psrldq		xmm0, 02h;
		packuswb	xmm0, xmm0;
		pextrw		edx, xmm0, 02h;
		movss		[edi], xmm0;
		mov			[edi + 04h], edx;
		add			esi, 08h;
		add			edi, 06h;
loop_1:
		test		eax, 01h;
		jz			loop_0;
		xorps		xmm1, xmm1;
		movss		xmm0, [esi];
		punpcklbw	xmm0, xmm1;
		pshuflw		xmm1, xmm0, 0ffh;
		pmullw		xmm0, xmm1;
		pmulhuw		xmm0, xmm7;
		paddw		xmm0, xmm6;
		psrlw		xmm0, 04h;
		packuswb	xmm0, xmm0;
		movd		edx, xmm0;
		mov			[edi], dx;
		shr			edx, 10h;
		mov			[edi + 02h], dl;
loop_0:
	}
}

// 源与目的可以相同
void TPCvt32To24BlendAlpha(OUT void* des,					// 目的
						   IN void* src,					// 源
						   IN size_t count,					// 像素个数
						   IN bool swap_rb)					// 交换RB通道的标志
{
	if(!des || !src || !count)
	{
		return;
	}
	if(swap_rb)
	{
		cr32_blend_a_2_cr24_swaprb(des, src, count);
	}
	else
	{
		cr32_blend_a_2_cr24(des, src, count);
	}
}

void cr32_blend_alpha_rb(OUT void* des,
						 IN void* src,
						 IN size_t count)
{
	//[[lgq 2009.11.24
	//定义了64位
#ifdef	_M_X64
	//64位暂时返回
	return;
#endif	//_M_X64
	//]]

	__asm
	{
		mov			esi_ptr, src;
		mov			edi_ptr, des;
		movdqa		xmm7, g_mask255rcp20;
		movdqa		xmm6, g_maskQHW;
		pxor			xmm5, xmm5;
		movdqa		xmm4, g_maskComp54;
		mov			ecx, count;
		mov			edx, ecx;
		shr			ecx, 2;
		jz			loop_2;
loop_4:
		movdqu		xmm0, [esi_ptr];
		movdqa		xmm1, xmm0;
		punpcklbw	xmm0, xmm5;
		punpckhbw	xmm1, xmm5;
		pshuflw		xmm0, xmm0, 0xc6;
		pshuflw		xmm1, xmm1, 0xc6;
		pshufhw		xmm0, xmm0, 0xc6;
		pshufhw		xmm1, xmm1, 0xc6;
		pshuflw		xmm2, xmm0, 0xff;
		pshuflw		xmm3, xmm1, 0xff;
		pshufhw		xmm2, xmm2, 0xff;
		pshufhw		xmm3, xmm3, 0xff;
		por			xmm2, xmm6;
		por			xmm3, xmm6;
		pmullw		xmm0, xmm2;
		pmullw		xmm1, xmm3;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		paddusw		xmm0, xmm4;
		paddusw		xmm1, xmm4;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		packuswb	xmm0, xmm1;
		movdqu		[edi_ptr], xmm0;
		add			esi_ptr, 0x10;
		add			edi_ptr, 0x10;
		dec			ecx;
		jnz			loop_4;
loop_2:
		test		edx, 2;
		jz			loop_1;
		movsd		xmm0, [esi_ptr];
		punpcklbw	xmm0, xmm5;
		pshuflw		xmm0, xmm0, 0xc6;
		pshufhw		xmm0, xmm0, 0xc6;
		pshuflw		xmm1, xmm0, 0xff;
		pshufhw		xmm1, xmm1, 0xff;
		por			xmm1, xmm6;
		pmullw		xmm0, xmm1;
		pmulhuw		xmm0, xmm7;
		paddusw		xmm0, xmm4;
		psrlw		xmm0, 4;
		packuswb	xmm0, xmm0;
		movsd		[edi_ptr], xmm0;
		add			esi_ptr, 8;
		add			edi_ptr, 8;
loop_1:
		test		edx, 1;
		jz			loop_0;
		movd		xmm0, [esi_ptr];
		punpcklbw	xmm0, xmm5;
		pshuflw		xmm0, xmm0, 0xc6;
		pshuflw		xmm1, xmm0, 0xff;
		por			xmm1, xmm6;
		pmullw		xmm0, xmm1;
		pmulhuw		xmm0, xmm7;
		paddusw		xmm0, xmm4;
		psrlw		xmm0, 4;
		packuswb	xmm0, xmm0;
		movd		[edi_ptr], xmm0;
loop_0:
	}
}

void cr32_blend_alpha(OUT void* des,
					  IN void* src,
					  IN size_t count)
{
	__asm
	{
		mov			esi_ptr, src;
		mov			edi_ptr, des;
		movdqa		xmm7, g_mask255rcp20;
		movdqa		xmm6, g_maskQHW;
		pxor		xmm5, xmm5;
		movdqa		xmm4, g_maskComp54;
		mov			ecx_ptr, count;
		mov			edx_ptr, ecx_ptr;
		shr			ecx_ptr, 2;
		jz			loop_2;
loop_4:
		movdqu		xmm0, [esi_ptr];
		movdqa		xmm1, xmm0;
		punpcklbw	xmm0, xmm5;
		punpckhbw	xmm1, xmm5;
		pshuflw		xmm2, xmm0, 0xff;
		pshuflw		xmm3, xmm1, 0xff;
		pshufhw		xmm2, xmm2, 0xff;
		pshufhw		xmm3, xmm3, 0xff;
		por			xmm2, xmm6;
		por			xmm3, xmm6;
		pmullw		xmm0, xmm2;
		pmullw		xmm1, xmm3;
		pmulhuw		xmm0, xmm7;
		pmulhuw		xmm1, xmm7;
		paddusw		xmm0, xmm4;
		paddusw		xmm1, xmm4;
		psrlw		xmm0, 4;
		psrlw		xmm1, 4;
		packuswb	xmm0, xmm1;
		movdqu		[edi_ptr], xmm0;
		add			esi_ptr, 0x10;
		add			edi_ptr, 0x10;
		dec			ecx_ptr;
		jnz			loop_4;
loop_2:
		test		edx_ptr, 2;
		jz			loop_1;
		movsd		xmm0, [esi_ptr];
		punpcklbw	xmm0, xmm5;
		pshuflw		xmm1, xmm0, 0xff;
		pshufhw		xmm1, xmm1, 0xff;
		por			xmm1, xmm6;
		pmullw		xmm0, xmm1;
		pmulhuw		xmm0, xmm7;
		paddusw		xmm0, xmm4;
		psrlw		xmm0, 4;
		packuswb	xmm0, xmm0;
		movsd		[edi_ptr], xmm0;
		add			esi_ptr, 8;
		add			edi_ptr, 8;
loop_1:
		test		edx_ptr, 1;
		jz			loop_0;
		movd		xmm0, [esi_ptr];
		punpcklbw	xmm0, xmm5;
		pshuflw		xmm1, xmm0, 0xff;
		por			xmm1, xmm6;
		pmullw		xmm0, xmm1;
		pmulhuw		xmm0, xmm7;
		paddusw		xmm0, xmm4;
		psrlw		xmm0, 4;
		packuswb	xmm0, xmm0;
		movd		[edi_ptr], xmm0;
loop_0:
	}
}

// 32位颜色混合Alpha，A不改变，RGB改变
// 源与目的可以相同
void TPColor32BlendAlpha(OUT void* des,						// 目的
						 IN void* src,						// 源
						 IN size_t count,					// 像素个数
						 IN bool swap_rb)					// 交换RB通道的标志
{
	if(!des || !src || !count)
	{
		return;
	}
	if(swap_rb)
	{
		cr32_blend_alpha_rb(des, src, count);
	}
	else
	{
		cr32_blend_alpha(des, src, count);
	}
}

//////////////////////////////////////////////////////////////////////////
// 双线性插值索引和权重计算
//////////////////////////////////////////////////////////////////////////
void bl_calc_power_map(int* power, unsigned short* map, int max_index, int count, float* begin, float step)
{
	__asm
	{
		mov			edi_ptr, power;
		mov			esi_ptr, map;
		mov			edx_ptr, begin;
		mov			ecx, count;
		movd		xmm7, max_index;
		movss		xmm6, step;
		movaps		xmm5, [edx_ptr];
		movaps		xmm4, g_mask16384f;
		pshuflw		xmm7, xmm7, 0x00;
		shufps		xmm6, xmm6, 0x00;
		shufpd		xmm7, xmm7, 0x00;
		mov			edx, ecx;
		shr			ecx, 3;
		jz			loop_4;
loop_8:
		cvttps2dq	xmm1, xmm5;
		movaps		xmm0, xmm5;
		addps		xmm5, xmm6;
		cvtdq2ps	xmm3, xmm1;
		movaps		xmm2, xmm5;
		addps		xmm5, xmm6;
		subps		xmm0, xmm3;
		cvttps2dq	xmm3, xmm2;
		mulps		xmm0, xmm4;
		packssdw	xmm1, xmm3;
		cvtdq2ps	xmm3, xmm3;
		pminsw		xmm1, xmm7;
		cvttps2dq	xmm0, xmm0;
		subps		xmm2, xmm3;
		movdqa		[esi_ptr], xmm1;
		cvttps2dq	xmm1, xmm4;
		mulps		xmm2, xmm4;
		packssdw	xmm1, xmm1;
		pslld		xmm0, 16;
		movdqa		xmm3, xmm1;
		cvttps2dq	xmm2, xmm2;
		psubsw		xmm1, xmm0;
		pslld		xmm2, 16;
		psrld		xmm1, 16;
		psubsw		xmm3, xmm2;
		por			xmm0, xmm1;
		psrld		xmm3, 16;
		movdqa		[edi_ptr], xmm0;
		por			xmm2, xmm3;
		movdqa		[edi_ptr + 0x10], xmm2;
		add			esi_ptr, 0x10;
		add			edi_ptr, 0x20;
		dec			ecx;
		jnz			loop_8;
loop_4:
		test		edx, 4;
		jz			loop_2;
		cvttps2dq	xmm1, xmm5;
		movaps		xmm0, xmm5;
		addps		xmm5, xmm6;
		cvtdq2ps	xmm2, xmm1;
		packssdw	xmm1, xmm1;
		subps		xmm0, xmm2;
		pminsw		xmm1, xmm7;
		mulps		xmm0, xmm4;
		movsd		[esi_ptr], xmm1;
		cvttps2dq	xmm1, xmm4;
		cvttps2dq	xmm0, xmm0;
		packssdw	xmm1, xmm1;
		pslld		xmm0, 16;
		psubsw		xmm1, xmm0;
		psrld		xmm1, 16;
		por			xmm0, xmm1;
		movdqa		[edi_ptr], xmm0;
		add			esi_ptr, 8;
		add			edi_ptr, 0x10;
loop_2:
		test		edx, 2;
		jz			loop_1;
		cvttps2dq	xmm1, xmm5;
		movaps		xmm0, xmm5;
		psrldq		xmm5, 8;
		cvtdq2ps	xmm2, xmm1;
		pminsw		xmm1, xmm7;
		subps		xmm0, xmm2;
		packssdw	xmm1, xmm1;
		mulps		xmm0, xmm4;
		movd		[esi_ptr], xmm1;
		cvttps2dq	xmm1, xmm4;
		cvttps2dq	xmm0, xmm0;
		packssdw	xmm1, xmm1;
		pslld		xmm0, 16;
		psubsw		xmm1, xmm0;
		psrld		xmm1, 16;
		por			xmm0, xmm1;
		movsd		[edi_ptr], xmm0;
		add			esi_ptr, 4;
		add			edi_ptr, 8;
loop_1:
		test		edx, 1;
		jz			loop_0;
		cvttps2dq	xmm1, xmm5;
		movaps		xmm0, xmm5;
		cvtdq2ps	xmm2, xmm1;
		pminsw		xmm1, xmm7;
		subps		xmm0, xmm2;
		packssdw	xmm1, xmm1;
		mulps		xmm0, xmm4;
		movd		eax, xmm1;
		mov			[esi_ptr], ax;
		cvttps2dq	xmm1, xmm4;
		cvttps2dq	xmm0, xmm0;
		packssdw	xmm1, xmm1;
		pslld		xmm0, 16;
		psubsw		xmm1, xmm0;
		psrld		xmm1, 16;
		por			xmm0, xmm1;
		movd		[edi_ptr], xmm0;
loop_0:
	}
}
//////////////////////////////////////////////////////////////////////////
// 双线性插值目的像素值计算
//////////////////////////////////////////////////////////////////////////
void bl_hor_ver_out(void* des, void* src, size_t src_pitch, int* power, unsigned short* map, int count, int power_y)
{
	__asm
	{
		mov			edi_ptr, des;
		mov			ecx_ptr, power;
		mov			edx_ptr, map;
		mov			ebx, count;
		mov			esi_ptr, src;
		movd		xmm7, power_y;
		pshuflw		xmm7, xmm7, 0x50;
		pshufd		xmm6, xmm7, 0x00;
		pshufd		xmm7, xmm7, 0x55;
		pxor		xmm5, xmm5;
		test		ebx, ebx;
		jz			loop_0;
loop_1:
		movzx		eax_ptr, word ptr [edx_ptr];
		movsd		xmm0, [eax_ptr * 4 + esi_ptr];
		add			esi_ptr, src_pitch;
		movsd		xmm1, [eax_ptr * 4 + esi_ptr];
		sub			esi_ptr, src_pitch;
		punpcklbw	xmm0, xmm5;
		punpcklbw	xmm1, xmm5;
		psllw		xmm0, 2;
		psllw		xmm1, 2;
		pmulhw		xmm0, xmm6;
		pmulhw		xmm1, xmm7;
		movd		xmm2, [ecx_ptr];
		paddsw		xmm0, xmm1;
		pshufd		xmm2, xmm2, 0x00;
		punpcklqdq	xmm1, xmm0;
		punpckhwd	xmm0, xmm1;
		pmaddwd		xmm0, xmm2;
		psrld		xmm0, 14;
		packssdw	xmm0, xmm0;
		packuswb	xmm0, xmm0;
		movd		[edi_ptr], xmm0;
		add			ecx_ptr, 4;
		add			edx_ptr, 2;
		add			edi_ptr, 4;
		dec			ebx;
		jnz			loop_1;
loop_0:
	}
}
void bl_hor_out(void* des, void* src, int* power, unsigned short* map, int count)
{
	__asm
	{
		mov			edi_ptr, des;
		mov			ecx_ptr, power;
		mov			edx_ptr, map;
		mov			ebx, count;
		mov			esi_ptr, src;
		pxor		xmm7, xmm7;
		test		ebx, ebx;
		jz			loop_0;
loop_1:
		movzx		eax_ptr, word ptr [edx_ptr];
		movsd		xmm0, [eax_ptr * 4 + esi_ptr];
		movd		xmm1, [ecx_ptr];
		punpcklbw	xmm0, xmm7;
		pshufd		xmm1, xmm1, 0x00;
		punpcklqdq	xmm2, xmm0;
		punpckhwd	xmm2, xmm0;
		pmaddwd		xmm2, xmm1;
		psrld		xmm2, 14;
		packssdw	xmm2, xmm2;
		packuswb	xmm2, xmm2;
		movd		[edi_ptr], xmm2;
		add			edi_ptr, 4;
		add			ecx_ptr, 4;
		add			edx_ptr, 2;
		dec			ebx;
		jnz			loop_1;
loop_0:
	}
}
void bl_ver_out(void* des, void* src_0, void* src_1, int power, int count)
{
	__asm
	{
		mov			edi_ptr, des;
		mov			ecx_ptr, src_0;
		mov			edx_ptr, src_1;
		mov			ebx, count;
		movd		xmm7, power;
		mov			eax, ebx;
		pshuflw		xmm7, xmm7, 0x50;
		pshufd		xmm6, xmm7, 0x00;
		pshufd		xmm7, xmm7, 0x55;
		pxor		xmm5, xmm5;
		shr			ebx, 2;
		jz			loop_2;
loop_4:
		movdqu		xmm0, [ecx_ptr];
		movdqu		xmm1, [edx_ptr];
		movdqa		xmm2, xmm0;
		movdqa		xmm3, xmm1;
		punpcklbw	xmm0, xmm5;
		punpcklbw	xmm1, xmm5;
		punpckhbw	xmm2, xmm5;
		punpckhbw	xmm3, xmm5;
		psllw		xmm0, 2;
		psllw		xmm1, 2;
		psllw		xmm2, 2;
		psllw		xmm3, 2;
		pmulhw		xmm0, xmm6;
		pmulhw		xmm1, xmm7;
		pmulhw		xmm2, xmm6;
		pmulhw		xmm3, xmm7;
		paddsw		xmm0, xmm1;
		paddsw		xmm2, xmm3;
		packuswb	xmm0, xmm2;
		movdqu		[edi_ptr], xmm0;
		add			edi_ptr, 0x10;
		add			ecx_ptr, 0x10;
		add			edx_ptr, 0x10;
		dec			ebx;
		jnz			loop_4;
loop_2:
		test		eax, 2;
		jz			loop_1;
		movsd		xmm0, [ecx_ptr];
		movsd		xmm1, [edx_ptr];
		punpcklbw	xmm0, xmm5;
		punpcklbw	xmm1, xmm5;
		psllw		xmm0, 2;
		psllw		xmm1, 2;
		pmulhw		xmm0, xmm6;
		pmulhw		xmm1, xmm7;
		paddsw		xmm0, xmm1;
		packuswb	xmm0, xmm0;
		movsd		[edi_ptr], xmm0;
		add			ecx_ptr, 8;
		add			edx_ptr, 8;
		add			edi_ptr, 8;
loop_1:
		test		eax, 1;
		jz			loop_0;
		movd		xmm0, [ecx_ptr];
		movd		xmm1, [edx_ptr];
		punpcklbw	xmm0, xmm5;
		punpcklbw	xmm1, xmm5;
		psllw		xmm0, 2;
		psllw		xmm1, 2;
		pmulhw		xmm0, xmm6;
		pmulhw		xmm1, xmm7;
		paddsw		xmm0, xmm1;
		packuswb	xmm0, xmm0;
		movd		[edi_ptr], xmm0;
loop_0:
	}
}
//////////////////////////////////////////////////////////////////////////
// 32 位双线性插值缩放，包括直接拷贝，只水平缩放，只垂直缩放，水平且垂直缩放
// 对于 16 位如 YUYV 可以模拟成 32 位处理
// 内部默认参数正确
//////////////////////////////////////////////////////////////////////////
void tp_memcpy(void* des,		// 32 位输出
			   int des_pitch,	// 目的一行的字节数
			   int des_x,		// 目的写入内存起点坐标
			   int des_y,
			   int des_width,	// 目的写入内存宽高
			   int des_height,
			   void const* src,	// 32 位输入
			   int src_pitch,	// 源一行的字节数
			   int src_x,		// 源读取内存的起点坐标
			   int src_y,
			   int src_width,	// 源读取内存的宽高
			   int src_height)
{
	if(!des || !src || !des_width || !des_height || !src_width || !src_height) return;
	unsigned char* des_pointer = (unsigned char *)des + des_y * des_pitch + (des_x << 2);
	unsigned char* src_pointer = (unsigned char *)src + src_y * src_pitch + (src_x << 2);
	_declspec(align(16)) float begin[4];
	if(des_width == src_width)
	{
		if(des_height == src_height)
		{
			if(src_pitch == des_pitch && des_pitch == (des_width << 2))
			{
				memcpy_line(des, src, des_pitch * des_height);
			}
			else
			{
				int count = (des_width << 2);
				for(int i = 0; i < des_height; ++i)
				{
					memcpy_line(des_pointer, src_pointer, count);
					des_pointer += des_pitch;
					src_pointer += src_pitch;
				}
			}
		}
		else
		{
			if(src_height == 1)
			{
				for(int i = 0; i < des_height; ++i)
				{
					memcpy_line(des_pointer, src_pointer, des_width << 2);
					des_pointer += des_pitch;
				}
			}
			else
			{
				int* power_y = (int *)_aligned_malloc(des_height << 2, 16);
				unsigned short* map_y = (unsigned short *)_aligned_malloc(des_height << 1, 16);
				float scale_h = (float)src_height/des_height;
				begin[0] = scale_h/2.1f;
				for(int i = 1; i < 4; ++i)
					begin[i] = begin[i - 1] + scale_h;
				bl_calc_power_map(power_y, map_y, src_height - 2, des_height, begin, scale_h * 4);
#ifndef _USE_MULTI_CORE_
				unsigned char* src_tmp = src_pointer + map_y[0] * src_pitch;
				for(int i = 0; i < des_height; ++i)
				{
					bl_ver_out(des_pointer, src_tmp, src_tmp + src_pitch, power_y[i], des_width);
					des_pointer += des_pitch;
					src_tmp = src_pointer + map_y[i + 1] * src_pitch;
				}
#else
				bo::bl_ver_out ver_out(des_pointer, des_pitch, src_pointer, src_pitch, power_y, map_y, des_width);
				g_thdmgr.lock();
				g_thdmgr.set_info(dynamic_cast<bo::thread_task *>(&ver_out), des_height);
				g_thdmgr.unlock();
#endif
				_aligned_free(map_y);
				_aligned_free(power_y);
			}
		}
	}
	else
	{
		if(des_height == src_height)
		{
			if(src_width == 1)
			{
				for(int i = 0; i < des_height; ++i)
				{
					memcpy_line(des_pointer, src_pointer, des_width << 2);
					des_pointer += des_pitch;
					src_pointer += src_pitch;
				}
			}
			else
			{
				int* power_x = (int *)_aligned_malloc(des_width << 2, 16);
				unsigned short* map_x = (unsigned short *)_aligned_malloc(des_width << 1, 16);
				float scale_w = (float)src_width/des_width;
				begin[0] = scale_w/2.1f;
				for(int i = 1; i < 4; ++i)
					begin[i] = begin[i - 1] + scale_w;
				bl_calc_power_map(power_x, map_x, src_width - 2, des_width, begin, scale_w * 4);
#ifndef _USE_MULTI_CORE_
				for(int i = 0; i < des_height; ++i)
				{
					bl_hor_out(des_pointer, src_pointer, power_x, map_x, des_width);
					des_pointer += des_pitch;
					src_pointer += src_pitch;
				}
#else
				bo::bl_hor_out hor_out(des_pointer, des_pitch, src_pointer, src_pitch, power_x, map_x, des_width);
				g_thdmgr.lock();
				g_thdmgr.set_info(dynamic_cast<bo::thread_task *>(&hor_out), des_height);
				g_thdmgr.unlock();
#endif
				_aligned_free(map_x);
				_aligned_free(power_x);
			}
		}
		else
		{
			if(src_width == 1 || src_height == 1)
			{
				TPMemSetDwordRect(des_pointer, des_pitch, *(PDWORD)src_pointer, des_width, des_height);
			}
			else
			{
				int* power_x = (int *)_aligned_malloc(des_width << 2, 16);
				int* power_y = (int *)_aligned_malloc(des_height << 2, 16);
				unsigned short* map_x = (unsigned short *)_aligned_malloc(des_width << 1, 16);
				unsigned short* map_y = (unsigned short *)_aligned_malloc(des_height << 1, 16);
				float scale_w = (float)src_width/des_width, scale_h = (float)src_height/des_height;
				begin[0] = scale_w/2.1f;
				for(int i = 1; i < 4; ++i)
					begin[i] = begin[i - 1] + scale_w;
				bl_calc_power_map(power_x, map_x, src_width - 2, des_width, begin, scale_w * 4);
				begin[0] = scale_h/2.1f;
				for(int i = 1; i < 4; ++i)
					begin[i] = begin[i - 1] + scale_h;
				bl_calc_power_map(power_y, map_y, src_height - 2, des_height, begin, scale_h * 4);
#ifndef _USE_MULTI_CORE_
				for(int i = 0; i < des_height; ++i)
				{
					bl_hor_ver_out(des_pointer, src_pointer + map_y[i] * src_pitch, src_pitch, power_x, map_x, des_width, power_y[i]);
					des_pointer += des_pitch;
				}
#else
				bo::bl_hor_ver_out hor_ver_out(des_pointer, des_pitch, src_pointer, src_pitch, power_x, power_y, map_x, map_y, des_width);
				g_thdmgr.lock();
				g_thdmgr.set_info(dynamic_cast<bo::thread_task *>(&hor_ver_out), des_height);
				g_thdmgr.unlock();
#endif
				_aligned_free(map_y);
				_aligned_free(map_x);
				_aligned_free(power_y);
				_aligned_free(power_x);
			}
		}
	}
}

void yuva_2_yuyv_and_a_line(void* yuyv, void* alpha, void const* yuva, size_t width)
{
	__asm
	{
		mov			esi_ptr, yuyv;
		mov			edi_ptr, alpha;
		mov			edx_ptr, yuva;
		mov			ecx_ptr, width;
		mov			eax_ptr, ecx_ptr;
		pxor		xmm7, xmm7;
		prefetchnta	[edx_ptr];
		shr			ecx_ptr, 2;
		jz			loop_2;
loop_4:
		prefetchnta	[edx_ptr + 0x10];
		movdqu		xmm0, [edx_ptr];
		movdqa		xmm1, xmm0;
		movdqa		xmm2, xmm0;
		punpcklbw	xmm0, xmm7;
		punpckhbw	xmm1, xmm7;
		pshufd		xmm0, xmm0, 0x64;
		pshufd		xmm1, xmm1, 0x64;
		pshufhw		xmm0, xmm0, 0xd8;
		pshufhw		xmm1, xmm1, 0xd8;
		pshufd		xmm0, xmm0, 0x08;
		pshufd		xmm1, xmm1, 0x08;
		psrld		xmm2, 24;
		punpcklqdq	xmm0, xmm1;
		packssdw	xmm2, xmm2;
		packuswb	xmm0, xmm0;
		packuswb	xmm2, xmm2;
		movsd		[esi_ptr], xmm0;
		movd		[edi_ptr], xmm2;
		add			esi_ptr, 8;
		add			edi_ptr, 4;
		add			edx_ptr, 16;
		sub			ecx_ptr, 1;
		jnz			loop_4;
loop_2:
		test		eax_ptr, 2;
		jz			loop_0;
		mov			ebx, [edx_ptr];
		mov			ecx, [edx_ptr + 4];
		mov			[esi_ptr], bx;	// uy
		mov			[esi_ptr + 2], cl;	// y
		shr			ebx, 16;
		shr			ecx, 16;
		mov			[edi_ptr], bh;	// a
		mov			[esi_ptr + 3], bl;	// v
		mov			[edi_ptr + 1], ch;	// a
loop_0:
	}
}

void yuva_2_yuyv_line(void* yuyv, void const* yuva, size_t width)
{
	__asm
	{
		mov			esi_ptr, yuyv;
		mov			edx_ptr, yuva;
		mov			ecx_ptr, width;
		mov			eax_ptr, ecx_ptr;
		pxor		xmm7, xmm7;
		prefetchnta	[edx_ptr];
		shr			ecx_ptr, 2;
		jz			loop_2;
loop_4:
		prefetchnta	[edx_ptr + 0x10];
		movdqu		xmm0, [edx_ptr];
		movdqa		xmm1, xmm0;
		punpcklbw	xmm0, xmm7;
		punpckhbw	xmm1, xmm7;
		pshufhw		xmm0, xmm0, 0xd8;
		pshufhw		xmm1, xmm1, 0xd8;
		pshufd		xmm0, xmm0, 0x08;
		pshufd		xmm1, xmm1, 0x08;
		punpcklqdq	xmm0, xmm1;
		packuswb	xmm0, xmm0;
		movsd		[esi_ptr], xmm0;
		add			esi_ptr, 8;
		add			edx_ptr, 16;
		sub			ecx_ptr, 1;
		jnz			loop_4;
loop_2:
		test		eax_ptr, 2;
		jz			loop_0;
		mov			ebx, [edx_ptr];
		mov			ecx, [edx_ptr + 4];
		mov			[esi_ptr], bx;	// uy
		mov			[esi_ptr + 2], cl;	// y
		shr			ebx, 16;
		shr			ecx, 16;
		mov			[esi_ptr + 3], bl;	// v
loop_0:
	}
}

void memcpy_line(void* des, void const* src, size_t count)
{
	__asm
	{
		mov			esi_ptr, src;
		mov			edi_ptr, des;
		mov			edx_ptr, count;
		cld			;
		mov			ecx_ptr, edx_ptr;
		shr			edx_ptr, 7;
		jz			loop_end;
		test		edi_ptr, 0x0f;
		jz			loop_pre;
		mov			edx_ptr, ecx_ptr;
		mov			ecx_ptr, edi_ptr;
		and			ecx_ptr, 0x0f;
		neg			ecx_ptr;
		add			ecx_ptr, 16;
		sub			edx_ptr, ecx_ptr;
		rep			movsb;
		mov			ecx_ptr, edx_ptr;
		shr			edx_ptr, 7;
		jz			loop_end;
loop_pre:
		prefetchnta	[esi_ptr];
		prefetchnta	[esi_ptr + 0x40];
		test		esi_ptr, 0x0f;
		jnz			loop_128;
loop_a128:
		prefetchnta	[esi_ptr + 0x80];
		prefetchnta	[esi_ptr + 0xc0];
		movdqa		xmm0, [esi_ptr];
		movdqa		xmm1, [esi_ptr + 0x10];
		movdqa		xmm2, [esi_ptr + 0x20];
		movdqa		xmm3, [esi_ptr + 0x30];
		movdqa		xmm4, [esi_ptr + 0x40];
		movdqa		xmm5, [esi_ptr + 0x50];
		movdqa		xmm6, [esi_ptr + 0x60];
		movdqa		xmm7, [esi_ptr + 0x70];
		movntdq		[edi_ptr], xmm0;
		movntdq		[edi_ptr + 0x10], xmm1;
		movntdq		[edi_ptr + 0x20], xmm2;
		movntdq		[edi_ptr + 0x30], xmm3;
		movntdq		[edi_ptr + 0x40], xmm4;
		movntdq		[edi_ptr + 0x50], xmm5;
		movntdq		[edi_ptr + 0x60], xmm6;
		movntdq		[edi_ptr + 0x70], xmm7;
		add			esi_ptr, 0x80;
		add			edi_ptr, 0x80;
		dec			edx_ptr;
		jnz			loop_a128;
		sfence		;
		jmp			loop_end;
loop_128:
		prefetchnta	[esi_ptr + 0x80];
		prefetchnta	[esi_ptr + 0xc0];
		movdqu		xmm0, [esi_ptr];
		movdqu		xmm1, [esi_ptr + 0x10];
		movdqu		xmm2, [esi_ptr + 0x20];
		movdqu		xmm3, [esi_ptr + 0x30];
		movdqu		xmm4, [esi_ptr + 0x40];
		movdqu		xmm5, [esi_ptr + 0x50];
		movdqu		xmm6, [esi_ptr + 0x60];
		movdqu		xmm7, [esi_ptr + 0x70];
		movntdq		[edi_ptr], xmm0;
		movntdq		[edi_ptr + 0x10], xmm1;
		movntdq		[edi_ptr + 0x20], xmm2;
		movntdq		[edi_ptr + 0x30], xmm3;
		movntdq		[edi_ptr + 0x40], xmm4;
		movntdq		[edi_ptr + 0x50], xmm5;
		movntdq		[edi_ptr + 0x60], xmm6;
		movntdq		[edi_ptr + 0x70], xmm7;
		add			esi_ptr, 0x80;
		add			edi_ptr, 0x80;
		dec			edx_ptr;
		jnz			loop_128;
		sfence		;
loop_end:
		and			ecx_ptr, 0x7f;
		mov			edx_ptr, ecx_ptr;
		and			ecx_ptr, 3;
		rep			movsb;
		mov			ecx_ptr, edx_ptr;
		shr			ecx_ptr, 2;
		rep			movsd;
	}
}

void memset_line(void* des, int c, size_t count)
{
	__asm
	{
		mov			edi_ptr, des;
		mov			ecx_ptr, count;
		movzx		eax, c;
		mov			edx_ptr, ecx_ptr;
		cld			;
		shr			ecx_ptr, 6;
		jz			loop_end;
		movd		xmm0, eax;
		pshuflw		xmm0, xmm0, 0x00;
		packuswb	xmm0, xmm0;
		pshufd		xmm0, xmm0, 0x00;
		movd		eax, xmm0;
		test		edi_ptr, 0x0f;
		jz			loop_a64;
		mov			ecx_ptr, edi_ptr;
		and			ecx_ptr, 0x0f;
		neg			ecx_ptr;
		add			ecx_ptr, 16;
		sub			edx_ptr, ecx_ptr;
		rep			stosb;
		mov			ecx_ptr, edx_ptr;
		shr			ecx_ptr, 6;
		jz			loop_end;
loop_a64:
		movntdq		[edi_ptr], xmm0;
		movntdq		[edi_ptr + 0x10], xmm0;
		movntdq		[edi_ptr + 0x20], xmm0;
		movntdq		[edi_ptr + 0x30], xmm0;
		add			edi_ptr, 0x40;
		dec			ecx_ptr;
		jnz			loop_a64;
		sfence		;
loop_end:
		and			edx_ptr, 63;
		mov			ecx_ptr, edx_ptr;
		and			ecx_ptr, 3;
		rep			stosb;
		mov			ecx_ptr, edx_ptr;
		shr			ecx_ptr, 2;
		rep			stosd;
	}
}

void dmemset_line(void* des, int c, size_t count)
{
	__asm
	{
		mov			edi_ptr, des;
		mov			ecx_ptr, count;
		mov			eax, c;
		shl			ecx_ptr, 2;
		mov			edx_ptr, ecx_ptr;
		cld			;
		shr			ecx_ptr, 6;
		jz			loop_end;
		movd		xmm0, eax;
		pshufd		xmm0, xmm0, 0x00;
		test		edi_ptr, 0x0f;
		jz			loop_a64;
		test		edi_ptr, 3;
		jnz			loop_64;
		mov			ecx_ptr, edi_ptr;
		and			ecx_ptr, 15;
		neg			ecx_ptr;
		add			ecx_ptr, 16;
		sub			ecx_ptr, ecx_ptr;
		rep			stosb;
		mov			ecx_ptr, edx_ptr;
		shr			ecx_ptr, 6;
		jz			loop_end;
loop_a64:
		movntdq		[edi_ptr], xmm0;
		movntdq		[edi_ptr + 0x10], xmm0;
		movntdq		[edi_ptr + 0x20], xmm0;
		movntdq		[edi_ptr + 0x30], xmm0;
		add			edi_ptr, 0x40;
		dec			ecx_ptr;
		jnz			loop_a64;
		sfence		;
		jmp			loop_end;
loop_64:
		movdqu		[edi_ptr], xmm0;
		movdqu		[edi_ptr + 0x10], xmm0;
		movdqu		[edi_ptr + 0x20], xmm0;
		movdqu		[edi_ptr + 0x30], xmm0;
		add			edi_ptr, 0x40;
		dec			ecx_ptr;
		jnz			loop_64;
loop_end:
		and			edx_ptr, 63;
		mov			ecx_ptr, edx_ptr;
		shr			ecx_ptr, 2;
		rep			stosd;
	}
}

bo::thread_manager::thread_manager()
{
	semevt[0] = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	semevt[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	event = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(semevt[0] && semevt[1] && event);
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	thread = new HANDLE[si.dwNumberOfProcessors];
	assert(thread);
	for(DWORD i = 0; i < si.dwNumberOfProcessors; ++i)
	{
		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, worker_thread, this, CREATE_SUSPENDED, NULL);
		assert(handle);
		thread[i] = handle;
		SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
		SetThreadIdealProcessor(handle, i);
		ResumeThread(handle);
	}
	InitializeCriticalSection(&cs);
	mutex = CreateMutex(NULL, FALSE, NULL);
	assert(mutex);
	semaphore = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	assert(semaphore);
}

bo::thread_manager::~thread_manager()
{
	SetEvent(semevt[1]);
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	WaitForMultipleObjects(si.dwNumberOfProcessors, thread, TRUE, INFINITE);
	std::for_each(thread, thread + si.dwNumberOfProcessors, CloseHandle);
	std::for_each(semevt, semevt + 2, CloseHandle);
	CloseHandle(event);
	delete[] thread;
	DeleteCriticalSection(&cs);
	CloseHandle(mutex);
	CloseHandle(semaphore);
}

void bo::thread_manager::set_info(thread_task* task_spec, size_t count)
{
	task = task_spec;
	index = count;
	ReleaseSemaphore(semaphore, count, NULL);
	ReleaseSemaphore(semevt[0], count, NULL);
}

void bo::thread_manager::lock()
{
	WaitForSingleObject(mutex, INFINITE);
}

void bo::thread_manager::unlock()
{
	WaitForSingleObject(event, INFINITE);
	ReleaseMutex(mutex);
}

unsigned bo::thread_manager::worker_proc()
{
	while(true)
	{
		DWORD ret = WaitForMultipleObjects(2, semevt, FALSE, INFINITE);
		if(ret == WAIT_OBJECT_0)
		{
			EnterCriticalSection(&cs);
			size_t idx = --index;
			LeaveCriticalSection(&cs);
			task->do_task(idx);
			WaitForSingleObject(semaphore, INFINITE);
			while(!idx && WaitForSingleObject(semaphore, 0) == WAIT_OBJECT_0)
				ReleaseSemaphore(semaphore, 1, NULL);
			idx ? 0 : SetEvent(event);
		}
		else
		{
			break;
		}
	}
	return 0;
}

unsigned bo::thread_manager::worker_thread(void* data)
{
	return (static_cast<thread_manager *>(data)->worker_proc());
}

inline bo::bl_hor_out::bl_hor_out(unsigned char* des, size_t dpitch, unsigned char* src, size_t spitch, int* power, unsigned short* map, size_t count)
: des_in(des), des_pitch(dpitch), src_in(src), src_pitch(spitch), power_in(power), map_in(map), count_in(count)
{
}

void bo::bl_hor_out::do_task(size_t index)
{
	::bl_hor_out(des_in + index * des_pitch, src_in + index * src_pitch, power_in, map_in, count_in);
}

inline bo::bl_hor_ver_out::bl_hor_ver_out(unsigned char* des, size_t dp, unsigned char* src, size_t sp, int* px, int* py, unsigned short* mx, unsigned short* my, size_t count)
: des_in(des), des_pitch(dp), src_in(src), src_pitch(sp), power_x(px), power_y(py), map_x(mx), map_y(my), count_in(count)
{
}

void bo::bl_hor_ver_out::do_task(size_t index)
{
	::bl_hor_ver_out(des_in + index * des_pitch, src_in + map_y[index] * src_pitch, src_pitch, power_x, map_x, count_in, power_y[index]);
}

inline bo::bl_ver_out::bl_ver_out(unsigned char* des, size_t dp, unsigned char* src, size_t sp, int* py, unsigned short* my, size_t count)
: des_in(des), des_pitch(dp), src_in(src), src_pitch(sp), power_y(py), map_y(my), width(count)
{
}

void bo::bl_ver_out::do_task(size_t index)
{
	unsigned char* src_tmp = src_in + map_y[index] * src_pitch;
	::bl_ver_out(des_in + index * des_pitch, src_tmp, src_tmp + src_pitch, power_y[index], width);
}

void yuv422_to_yuyv_line(void* yuyv, void* y, void* u, void* v, size_t width)
{
	__asm
	{
		mov			edi_ptr, yuyv;
		mov			eax_ptr, y;
		mov			ebx_ptr, u;
		mov			ecx_ptr, v;
		prefetchnta	[eax_ptr];
		mov			esi_ptr, width;
		mov			edx_ptr, esi_ptr;
		and			esi_ptr, 0xff;
		shr			edx_ptr, 4;
		jz			loop_8;
loop_16:
		prefetchnta	[eax_ptr + 0x10];
		movdqu		xmm0, [eax_ptr];
		movsd		xmm1, [ebx_ptr];
		movsd		xmm2, [ecx_ptr];
		movdqa		xmm3, xmm0;
		punpcklbw	xmm1, xmm2;
		punpcklbw	xmm0, xmm1;
		punpckhbw	xmm3, xmm1;
		movdqu		[edi_ptr], xmm0;
		movdqu		[edi_ptr + 0x10], xmm3;
		add			eax_ptr, 0x10;
		add			ebx_ptr, 8;
		add			ecx_ptr, 8;
		add			edi_ptr, 0x20;
		dec			edx_ptr;
		jnz			loop_16;
loop_8:
		test		esi_ptr, 8;
		jz			loop_2;
		movsd		xmm0, [eax_ptr];
		movd		xmm1, [ebx_ptr];
		movd		xmm2, [ecx_ptr];
		punpcklbw	xmm1, xmm2;
		punpcklbw	xmm0, xmm1;
		movdqu		[edi_ptr], xmm0;
		add			eax_ptr, 8;
		add			ebx_ptr, 4;
		add			ecx_ptr, 4;
		add			edi_ptr, 0x10;
		sub			esi_ptr, 8;
loop_2:
		test		esi_ptr, 2;
		jz			loop_0;
		mov			dx, [eax_ptr];
		mov			[edi_ptr], dl;
		mov			[edi_ptr + 2], dh;
		mov			dl, [ebx_ptr];
		mov			[edi_ptr + 1], dl;
		mov			dl, [ecx_ptr];
		mov			[edi_ptr + 3], dl;
		add			eax_ptr, 2;
		inc			ebx_ptr;
		inc			ecx_ptr;
		add			edi_ptr, 4;
		sub			esi_ptr, 2;
		jmp			loop_2;
loop_0:
	}
}

void yuyv_to_yuv422_line(void* y, void* u, void* v, void* yuyv, size_t width)
{
	__asm
	{
		mov			eax_ptr, y;
		mov			ebx_ptr, u;
		mov			ecx_ptr, v;
		mov			esi_ptr, yuyv;
		prefetchnta	[esi_ptr];
		mov			edi_ptr, width;
		movdqa		xmm7, g_maskLW;
		mov			edx_ptr, edi_ptr;
		and			edi_ptr, 0xff;
		shr			edx_ptr, 4;
		jz			loop_8;
loop_16:
		prefetchnta	[esi_ptr + 0x20];
		movdqu		xmm0, [esi_ptr];
		movdqu		xmm1, [esi_ptr + 0x10];
		movdqa		xmm2, xmm0;
		movdqa		xmm3, xmm1;
		pand		xmm0, xmm7;
		pand		xmm1, xmm7;
		psrlw		xmm2, 8;
		psrlw		xmm3, 8;
		packuswb	xmm0, xmm1;
		packuswb	xmm2, xmm2;
		packuswb	xmm3, xmm3;
		movdqu		[eax_ptr], xmm0;
		punpcklbw	xmm2, xmm3;
		pshuflw		xmm2, xmm2, 0xd8;
		pshufhw		xmm2, xmm2, 0xd8;
		pshufd		xmm2, xmm2, 0xd8;
		movsd		[ebx_ptr], xmm2;
		punpckhqdq	xmm2, xmm2;
		movsd		[ecx_ptr], xmm2;
		add			eax_ptr, 0x10;
		add			ebx_ptr, 8;
		add			ecx_ptr, 8;
		add			esi_ptr, 0x20;
		dec			edx_ptr;
		jnz			loop_16;
loop_8:
		test		edi_ptr, 8;
		jz			loop_2;
		movdqu		xmm0, [esi_ptr];
		movdqa		xmm2, xmm0;
		pand		xmm0, xmm7;
		psrlw		xmm2, 8;
		packuswb	xmm0, xmm0;
		movsd		[eax_ptr], xmm0;
		pshuflw		xmm2, xmm2, 0xd8;
		pshufhw		xmm2, xmm2, 0xd8;
		pshufd		xmm2, xmm2, 0xd8;
		packuswb	xmm2, xmm2;
		movd		[ebx_ptr], xmm2;
		pshufd		xmm2, xmm2, 0x01;
		movd		[ecx_ptr], xmm2;
		add			eax_ptr, 0x10;
		add			ebx_ptr, 8;
		add			ecx_ptr, 8;
		add			esi_ptr, 0x20;
		sub			edi_ptr, 8;
loop_2:
		test		edi_ptr, 2;
		jz			loop_0;
		mov			edx, [esi_ptr];
		mov			[eax_ptr], dl;
		mov			[ebx_ptr], dh;
		shr			edx, 16;
		mov			[eax_ptr + 2], dl;
		mov			[ecx_ptr], dh;
		add			eax_ptr, 2;
		inc			ebx_ptr;
		inc			ecx_ptr;
		add			esi_ptr, 4;
		sub			edi_ptr, 2;
		jmp			loop_2;
loop_0:
	}
}
