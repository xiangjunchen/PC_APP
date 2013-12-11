// 采用 SSE2 优化算法

#include "stdafx.h"
#include "TPSSE2.h"
#include "TPSSE2Static.h"
#include "TPColorSpaceConv.h"
#include <assert.h>
#include "CoreInclude\CPUInclude\NXColorSpace.h"
//////////////////////////////////////////////////////////////////////////
// 常量定义

#define PAL_SD_HEIGHT			576			// PAL 制式 SD 幅面高
#define SPECIAL_RGBA_VALUE		0xff101010

//////////////////////////////////////////////////////////////////////////
// 初始化参数
void InitSSEStaticCoef()
{
	DWORD dwTmpR(0), dwTmpG(0), dwTmpB(0), dwTmpU(0), dwTmpV(0);

	// HD RGB or BGR to YUV
	dwTmpR = (DWORD)(TPCS_GetMatRGBA2YUVA_Cell(0, 0, HIGH_DEFINITION) * COEF_SCALE_16384 + 0.5f);
	dwTmpG = (DWORD)(TPCS_GetMatRGBA2YUVA_Cell(0, 1, HIGH_DEFINITION) * COEF_SCALE_16384 + 0.5f);
	dwTmpB = (DWORD)(TPCS_GetMatRGBA2YUVA_Cell(0, 2, HIGH_DEFINITION) * COEF_SCALE_16384 + 0.5f);
	g_nY32_BR_HD[0] = g_nY32_BR_HD[1] = g_nY32_BR_HD[2] = g_nY32_BR_HD[3] = g_nY24_RGB_HD[2] = (dwTmpR | (dwTmpB << 16));
	g_nY32_G_HD[0] = g_nY32_G_HD[1] = g_nY32_G_HD[2] = g_nY32_G_HD[3] = dwTmpG;
	g_nY32_RB_HD[0] = g_nY32_RB_HD[1] = g_nY32_RB_HD[2] = g_nY32_RB_HD[3] = g_nY24_BGR_HD[2] = (dwTmpB | (dwTmpR << 16));
	g_nY32_BGR_HD[0] = g_nY32_BGR_HD[2] = g_nY24_BGR_HD[1] = (dwTmpR | (dwTmpG << 16));
	g_nY32_BGR_HD[1] = g_nY32_BGR_HD[3] = dwTmpB;
	g_nY32_RGB_HD[1] = g_nY32_RGB_HD[3] = dwTmpR;
	g_nY24_RGB_HD[0] = g_nY24_RGB_HD[3] = ((dwTmpR << 16) | dwTmpG);
	g_nY24_BGR_HD[0] = g_nY24_BGR_HD[3] = ((dwTmpB << 16) | dwTmpG);
	g_nY32_RGB_HD[0] = g_nY32_RGB_HD[2] = g_nY24_RGB_HD[1] = ((dwTmpG << 16) | dwTmpB);
	dwTmpR = ((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(1, 0, HIGH_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff);
	dwTmpG = (((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(1, 1, HIGH_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff) << 16);
	dwTmpB = ((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(1, 2, HIGH_DEFINITION) * COEF_SCALE_16384 + 0.5f) & 0x0000ffff);
	g_nU_BGR_HD[0] = g_nU_BGR_HD[2] = g_nUV_BGR_HD[0] = (dwTmpR | dwTmpG);
	g_nU_BGR_HD[1] = g_nU_BGR_HD[3] = g_nUV_BGR_HD[1] = dwTmpB;
	g_nU_RGB_HD[0] = g_nU_RGB_HD[2] = g_nUV_RGB_HD[0] = (dwTmpB | dwTmpG);
	g_nU_RGB_HD[1] = g_nU_RGB_HD[3] = g_nUV_RGB_HD[1] = dwTmpR;
	dwTmpR = ((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(2, 0, HIGH_DEFINITION) * COEF_SCALE_16384 + 0.5f) & 0x0000ffff);
	dwTmpG = (((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(2, 1, HIGH_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff) << 16);
	dwTmpB = ((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(2, 2, HIGH_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff);
	g_nV_BGR_HD[0] = g_nV_BGR_HD[2] = g_nUV_BGR_HD[2] = (dwTmpR | dwTmpG);
	g_nV_BGR_HD[1] = g_nV_BGR_HD[3] = g_nUV_BGR_HD[3] = dwTmpB;
	g_nV_RGB_HD[0] = g_nV_RGB_HD[2] = g_nUV_RGB_HD[2] = (dwTmpB | dwTmpG);
	g_nV_RGB_HD[1] = g_nV_RGB_HD[3] = g_nUV_RGB_HD[3] = dwTmpR;
	dwTmpV = ((DWORD)(TPCS_GetMatYUVA2RGBA_Cell(0, 2, HIGH_DEFINITION) * COEF_SCALE_16384 + 0.5f) & 0x0000ffff);
	g_nR_V_HD[0] = g_nR_V_HD[1] = g_nR_V_HD[2] = g_nR_V_HD[3] = (dwTmpV | (dwTmpV << 16));
	dwTmpU = ((DWORD)(TPCS_GetMatYUVA2RGBA_Cell(1, 1, HIGH_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff);
	dwTmpV = ((DWORD)(TPCS_GetMatYUVA2RGBA_Cell(1, 2, HIGH_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff);
	g_nG_U_HD[0] = g_nG_U_HD[1] = g_nG_U_HD[2] = g_nG_U_HD[3] = (dwTmpU | (dwTmpU << 16));
	g_nG_V_HD[0] = g_nG_V_HD[1] = g_nG_V_HD[2] = g_nG_V_HD[3] = (dwTmpV | (dwTmpV << 16));
	dwTmpU = ((DWORD)(TPCS_GetMatYUVA2RGBA_Cell(2, 1, HIGH_DEFINITION) * COEF_SCALE_16384 + 0.5f) & 0x0000ffff);
	g_nB_U_HD[0] = g_nB_U_HD[1] = g_nB_U_HD[2] = g_nB_U_HD[3] = (dwTmpU | (dwTmpU << 16));
	g_fRB_YUV_HD[0] = TPCS_GetMatYUVA2RGBA_Cell(0, 0, HIGH_DEFINITION);
	g_fRB_YUV_HD[1] = TPCS_GetMatYUVA2RGBA_Cell(2, 1, HIGH_DEFINITION);
	g_fRB_YUV_HD[2] = TPCS_GetMatYUVA2RGBA_Cell(0, 2, HIGH_DEFINITION);
	g_fRB_YUV_HD[3] = TPCS_GetMatYUVA2RGBA_Cell(2, 0, HIGH_DEFINITION);
	g_fG_YUV_HD[0] = TPCS_GetMatYUVA2RGBA_Cell(1, 0, HIGH_DEFINITION);
	g_fG_YUV_HD[1] = TPCS_GetMatYUVA2RGBA_Cell(1, 1, HIGH_DEFINITION);
	g_fG_YUV_HD[2] = TPCS_GetMatYUVA2RGBA_Cell(1, 2, HIGH_DEFINITION);
	g_fG_YUV_HD[3] = 0.f;
	// SD RGB or BGR to YUV
	dwTmpR = (DWORD)(TPCS_GetMatRGBA2YUVA_Cell(0, 0, STANDARD_DEFINITION) * COEF_SCALE_16384 + 0.5f);
	dwTmpG = (DWORD)(TPCS_GetMatRGBA2YUVA_Cell(0, 1, STANDARD_DEFINITION) * COEF_SCALE_16384 + 0.5f);
	dwTmpB = (DWORD)(TPCS_GetMatRGBA2YUVA_Cell(0, 2, STANDARD_DEFINITION) * COEF_SCALE_16384 + 0.5f);
	g_nY32_BR_SD[0] = g_nY32_BR_SD[1] = g_nY32_BR_SD[2] = g_nY32_BR_SD[3] = g_nY24_RGB_SD[2] = (dwTmpR | (dwTmpB << 16));
	g_nY32_G_SD[0] = g_nY32_G_SD[1] = g_nY32_G_SD[2] = g_nY32_G_SD[3] = dwTmpG;
	g_nY32_RB_SD[0] = g_nY32_RB_SD[1] = g_nY32_RB_SD[2] = g_nY32_RB_SD[3] = g_nY24_BGR_SD[2] = (dwTmpB | (dwTmpR << 16));
	g_nY32_BGR_SD[0] = g_nY32_BGR_SD[2] = g_nY24_BGR_SD[1] = (dwTmpR | (dwTmpG << 16));
	g_nY32_BGR_SD[1] = g_nY32_BGR_SD[3] = dwTmpB;
	g_nY32_RGB_SD[1] = g_nY32_RGB_SD[3] = dwTmpR;
	g_nY24_RGB_SD[0] = g_nY24_RGB_SD[3] = ((dwTmpR << 16) | dwTmpG);
	g_nY24_BGR_SD[0] = g_nY24_BGR_SD[3] = ((dwTmpB << 16) | dwTmpG);
	g_nY32_RGB_SD[0] = g_nY32_RGB_SD[2] = g_nY24_RGB_SD[1] = ((dwTmpG << 16) | dwTmpB);
	dwTmpR = ((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(1, 0, STANDARD_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff);
	dwTmpG = (((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(1, 1, STANDARD_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff) << 16);
	dwTmpB = ((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(1, 2, STANDARD_DEFINITION) * COEF_SCALE_16384 + 0.5f) & 0x0000ffff);
	g_nU_BGR_SD[0] = g_nU_BGR_SD[2] = g_nUV_BGR_SD[0] = (dwTmpR | dwTmpG);
	g_nU_BGR_SD[1] = g_nU_BGR_SD[3] = g_nUV_BGR_SD[1] = dwTmpB;
	g_nU_RGB_SD[0] = g_nU_RGB_SD[2] = g_nUV_RGB_SD[2] = (dwTmpB | dwTmpG);
	g_nU_RGB_SD[1] = g_nU_RGB_SD[3] = g_nUV_RGB_SD[3] = dwTmpR;
	dwTmpR = ((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(2, 0, STANDARD_DEFINITION) * COEF_SCALE_16384 + 0.5f) & 0x0000ffff);
	dwTmpG = (((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(2, 1, STANDARD_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff) << 16);
	dwTmpB = ((DWORD)(TPCS_GetMatRGBA2YUVA_Cell(2, 2, STANDARD_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff);
	g_nV_BGR_SD[0] = g_nV_BGR_SD[2] = g_nUV_BGR_SD[2] = (dwTmpR | dwTmpG);
	g_nV_BGR_SD[1] = g_nV_BGR_SD[3] = g_nUV_BGR_SD[3] = dwTmpB;
	g_nV_RGB_SD[0] = g_nV_RGB_SD[2] = g_nUV_RGB_SD[2] = (dwTmpB | dwTmpG);
	g_nV_RGB_SD[1] = g_nV_RGB_SD[3] = g_nUV_RGB_SD[3] = dwTmpR;
	dwTmpV = ((DWORD)(TPCS_GetMatYUVA2RGBA_Cell(0, 2, STANDARD_DEFINITION) * COEF_SCALE_16384 + 0.5f) & 0x0000ffff);
	g_nR_V_SD[0] = g_nR_V_SD[1] = g_nR_V_SD[2] = g_nR_V_SD[3] = (dwTmpV | (dwTmpV << 16));
	dwTmpU = ((DWORD)(TPCS_GetMatYUVA2RGBA_Cell(1, 1, STANDARD_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff);
	dwTmpV = ((DWORD)(TPCS_GetMatYUVA2RGBA_Cell(1, 2, STANDARD_DEFINITION) * COEF_SCALE_16384 - 0.5f) & 0x0000ffff);
	g_nG_U_SD[0] = g_nG_U_SD[1] = g_nG_U_SD[2] = g_nG_U_SD[3] = (dwTmpU | (dwTmpU << 16));
	g_nG_V_SD[0] = g_nG_V_SD[1] = g_nG_V_SD[2] = g_nG_V_SD[3] = (dwTmpV | (dwTmpV << 16));
	dwTmpU = ((DWORD)(TPCS_GetMatYUVA2RGBA_Cell(2, 1, STANDARD_DEFINITION) * COEF_SCALE_16384 + 0.5f) & 0x0000ffff);
	g_nB_U_SD[0] = g_nB_U_SD[1] = g_nB_U_SD[2] = g_nB_U_SD[3] = (dwTmpU | (dwTmpU << 16));
	g_fRB_YUV_SD[0] = TPCS_GetMatYUVA2RGBA_Cell(0, 0, STANDARD_DEFINITION);
	g_fRB_YUV_SD[1] = TPCS_GetMatYUVA2RGBA_Cell(2, 1, STANDARD_DEFINITION);
	g_fRB_YUV_SD[2] = TPCS_GetMatYUVA2RGBA_Cell(0, 2, STANDARD_DEFINITION);
	g_fRB_YUV_SD[3] = TPCS_GetMatYUVA2RGBA_Cell(2, 0, STANDARD_DEFINITION);
	g_fG_YUV_SD[0] = TPCS_GetMatYUVA2RGBA_Cell(1, 0, STANDARD_DEFINITION);
	g_fG_YUV_SD[1] = TPCS_GetMatYUVA2RGBA_Cell(1, 1, STANDARD_DEFINITION);
	g_fG_YUV_SD[2] = TPCS_GetMatYUVA2RGBA_Cell(1, 2, STANDARD_DEFINITION);
	g_fG_YUV_SD[3] = 0.f;
}

//////////////////////////////////////////////////////////////////////////
// 拷贝一行 Buffer
//////////////////////////////////////////////////////////////////////////
void memcpy_line(OUT void *des,		// 目标
				 IN void *src,		// 源
				 IN int na16,		// 宽度除以 16 的商
				 IN int nm16)		// 宽度除以 16 的余数
{
	_asm{
		mov			edi, des;
		mov			esi, src;
		mov			ecx, na16;
		mov			edx, nm16;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_a8;
loop_a16:
		movups		xmm0, [esi];
		movups		[edi], xmm0;
		add			esi, 0x10;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_a8:
		test		edx, 0x00000008;
		jz			loop_a4;
		movlps		xmm0, [esi];
		movlps		[edi], xmm0;
		add			esi, 0x08;
		add			edi, 0x08;
loop_a4:
		test		edx, 0x00000004;
		jz			loop_a2;
		mov			eax, [esi];
		add			esi, 0x04;
		mov			[edi], eax;
		add			edi, 0x04;
loop_a2:
		test		edx, 0x00000002;
		jz			loop_a1;
		mov			ax, [esi];
		add			esi, 0x02;
		mov			[edi], ax;
		add			edi, 0x02;
loop_a1:
		test		edx, 0x00000001;
		jz			loop_end;
		mov			al, [esi];
		mov			[edi], al;
loop_end:
	}
}

//////////////////////////////////////////////////////////////////////////
// 以字节为单位设置一行 Buffer
//////////////////////////////////////////////////////////////////////////
void memset_char_line(OUT void *des,	// 目标
					  IN char value,	// 设置值
					  IN int na16,		// 宽度除以 16 的商
					  IN int nm16)		// 宽度除以 16 的余数
{
	_asm{
		mov			edi, des;
		mov			ecx, na16;
		movzx		eax, value;		// 000c
		mov			edx, nm16;
		movd		xmm0, eax;		// 0000 0000 0000 000c
		pshuflw		xmm0, xmm0, 0x00;	// 0000 0000 0c0c 0c0c
		packuswb	xmm0, xmm0;		// 0000 cccc 0000 cccc
		pshufd		xmm0, xmm0, 0x00;	// cccc cccc cccc cccc
		movd		eax, xmm0;		// cccc
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_a8;
loop_a16:
		movups		[edi], xmm0;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_a8:
		test		edx, 0x00000008;
		jz			loop_a4;
		movlps		[edi], xmm0;
		add			edi, 0x08;
loop_a4:
		test		edx, 0x00000004;
		jz			loop_a2;
		mov			[edi], eax;
		add			edi, 0x04;
loop_a2:
		test		edx, 0x00000002;
		jz			loop_a1;
		mov			[edi], ax;
		add			edi, 0x02;
loop_a1:
		test		edx,0x00000001;
		jz			loop_end;
		mov			[edi], al; 
loop_end:
	}
}

//////////////////////////////////////////////////////////////////////////
// 以双字为单位设置一行 Buffer
//////////////////////////////////////////////////////////////////////////
void memset_dword_line(OUT void *des,	// 目标
					   IN DWORD value,	// 设置值
					   IN int na16,		// 宽度除以 16 的商
					   IN int nm16)		// 宽度除以 16 的余数
{
	_asm{
		mov			edi, des;
		mov			ecx, na16;
		mov			eax, value;		// v
		mov			edx, nm16;
		movd		xmm0, eax;		// 0 0 0 v
		shufps		xmm0, xmm0, 0x00;	// v v v v
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_a8;
loop_a16:
		movups		[edi], xmm0;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_a8:
		test		edx, 0x00000008;
		jz			loop_a4;
		movlps		[edi], xmm0;
		add			edi, 0x08;
loop_a4:
		test		edx, 0x00000004;
		jz			loop_end;
		mov			[edi], eax;
loop_end:
	}
}

//////////////////////////////////////////////////////////////////////////
// 分裂字节
// 分裂函数命名规则：第一个 n 表示分裂对象的字节长，第二个 n 表示分裂起始点，
//		第三个 n 表示分裂字节数，都是 n 时表示任意分裂
//////////////////////////////////////////////////////////////////////////
void memsplit_n_n_n(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount,		// 分裂对象的个数
					IN int nUnitBytes,		// 分裂对象的字节长
					IN int nCellStart,		// 分裂起始点
					IN int nCellCount)		// 分裂字节数
{
	__asm {
		mov			edx, nUnitCount;
		test		edx, edx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		mov			eax, nUnitBytes;
		mov			ebx, nCellStart;
		mov			ecx, nCellCount;
		nop;
		add			esi, ebx;		// 分裂起点
		sub			eax, ecx;		// 上一下 unit 分裂完后到下一个 unit 分裂起点的位置差
loop_unit:
		rep movsb;
		mov			ecx, nCellCount;
		add			esi, eax;
		sub			edx, 0x01;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_2_0_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		movaps		xmm7, gc_dwMaskWLB;
		shr			ecx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x40];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		andps		xmm0, xmm7;		// 16 位的低 8 位
		andps		xmm1, xmm7;
		packuswb	xmm0, xmm1;
		movups		[edi], xmm0;
		add			esi, 0x20;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		movsb;
		inc			esi;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_2_1_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		shr			ecx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x40];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		psrlw		xmm0, 0x08;		// 16 位的高 8 位
		psrlw		xmm1, 0x08;
		movups		[edi], xmm0;
		add			esi, 0x20;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		inc			esi;
		movsb;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_4_0_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		movaps		xmm7, gc_dwMaskDLB;
		shr			ecx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		andps		xmm0, xmm7;		// 32 位的低 8 位
		andps		xmm1, xmm7;
		andps		xmm2, xmm7;
		andps		xmm3, xmm7;
		packssdw	xmm0, xmm1;
		packssdw	xmm2, xmm3;
		packuswb	xmm0, xmm2;
		movups		[edi], xmm0;
		add			esi, 0x40;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		movsb;
		add			esi, 0x03;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_4_0_2(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		movaps		xmm7, gc_dwMaskDLW;
		shr			ecx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		andps		xmm0, xmm7;		// 32 位的低 16 位
		andps		xmm1, xmm7;
		andps		xmm2, xmm7;
		andps		xmm3, xmm7;
		packssdw	xmm0, xmm1;
		packssdw	xmm2, xmm3;
		movups		[edi], xmm0;
		movups		[edi + 0x10], xmm2;
		add			esi, 0x40;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		movsw;
		add			esi, 0x02;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_4_0_3(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		shr			ecx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		movaps		xmm7, gc_dwMaskRGB;
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		andps		xmm0, xmm7;		// 32 位的低 24 位
		andps		xmm1, xmm7;
		movaps		xmm2, xmm0;		// 0 b3g3r3  0 b2g2r2  0 b1g1r1  0 b0g0r0
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm0;
		movaps		xmm6, xmm0;
		psrld		xmm2, 0x08;		// @  @  00 b1g1  @
		psrld		xmm3, 0x10;		// @  000 b2  @  @
		pslld		xmm4, 0x18;		// @  @  r1 000  @
		pslld		xmm5, 0x10;		// @  g2r2 00  @  @  @
		pslld		xmm6, 0x08;		// b3g3r3 0  @  @  @
		shufps		xmm0, xmm2, 0x50;	// 00 b1g1  0 b0g0r0
		shufps		xmm3, xmm1, 0x0a;	// 0 b4g4r4  000 b2
		shufps		xmm4, xmm5, 0xa5;	// g2r2 00  r1 000
		shufps		xmm0, xmm3, 0x88;	// 0 b4g4r4  000 b2  00 b1g1  0 b0g0r0
		movaps		xmm2, xmm1;		// 0 b7g7r7  0 b6g6r6  0 b5g5r5  0 b4g4r4
		movaps		xmm3, xmm1;
		movaps		xmm5, xmm1;
		movaps		xmm7, xmm1;
		pslld		xmm1, 0x18;		// @  @  r5 000  @
		psrld		xmm2, 0x08;		// @  @  00 b5g5  @
		psrld		xmm3, 0x10;		// @  000 b6  @  @
		pslld		xmm5, 0x10;		// @  g6r6 00  @  @  @
		pslld		xmm7, 0x08;		// b7g7r7 0  @  @  @
		shufps		xmm6, xmm1, 0x5f;	// r5 000  b3g3r3 0
		shufps		xmm2, xmm3, 0xa5;	// 000 b6  00 b5g5
		shufps		xmm5, xmm7, 0xfa;	// b7g7r7 0  g6r6 00
		shufps		xmm4, xmm6, 0x88;	// r5 000  b3g3r3 0  g2r2 00  r1 000
		orps		xmm5, xmm2;		// b7g7r7b6  g6r6b5g5
		orps		xmm4, xmm0;		// r5b4g4r4  b3g3r3b2  g2r2b1g1  r1b0g0r0
		shufps		xmm5, xmm5, 0x88;	// b7g7r7b6  g6r6b5g5  b7g7r7b6  g6r6b5g5
		movups		[edi], xmm4;
		movlps		[edi + 0x10], xmm5;
		add			esi, 0x20;
		add			edi, 0x18;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			edx, nUnitCount;
		and			edx, 0x07;
		test		edx, edx;
		jz			loop_end;
loop_unit:
		mov			ecx, 0x03;
		rep movsb;
		inc			esi;
		sub			edx, 0x01;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_4_1_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		movaps		xmm7, gc_dwMaskWLB;
		shr			edx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		psrlw		xmm0, 0x08;		// 32 位的低 16 位的高 8 位
		psrlw		xmm1, 0x08;
		psrlw		xmm2, 0x08;
		psrlw		xmm3, 0x08;
		packssdw	xmm0, xmm1;		// 16 位的高 8 位无效
		packssdw	xmm2, xmm3;
		andps		xmm0, xmm7;		// 32 位的低 16 位的高 8 位
		andps		xmm2, xmm7;
		packuswb	xmm0, xmm2;
		movups		[edi], xmm0;
		add			esi, 0x40;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
		inc			esi;
loop_unit:
		movsb;
		add			esi, 0x03;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_4_1_2(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		shr			edx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		pslld		xmm0, 0x08;		// 32 位的中 16 位
		pslld		xmm1, 0x08;
		pslld		xmm2, 0x08;
		pslld		xmm3, 0x08;
		psrld		xmm0, 0x10;		// 32 位的中 16 位
		psrld		xmm1, 0x10;
		psrld		xmm2, 0x10;
		psrld		xmm3, 0x10;
		packssdw	xmm0, xmm1;
		packssdw	xmm2, xmm3;
		movups		[edi], xmm0;
		movups		[edi + 0x10], xmm2;
		add			esi, 0x40;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
		inc			esi;
loop_unit:
		movsw;
		add			esi, 0x02;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_4_1_3(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		shr			ecx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		psrld		xmm0, 0x08;		// 32 位的高 24 位
		psrld		xmm1, 0x08;
		movaps		xmm2, xmm0;		// 0 b3g3r3  0 b2g2r2  0 b1g1r1  0 b0g0r0
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm0;
		movaps		xmm6, xmm0;
		psrld		xmm2, 0x08;		// @  @  00 b1g1  @
		psrld		xmm3, 0x10;		// @  000 b2  @  @
		pslld		xmm4, 0x18;		// @  @  r1 000  @
		pslld		xmm5, 0x10;		// @  g2r2 00  @  @  @
		pslld		xmm6, 0x08;		// b3g3r3 0  @  @  @
		shufps		xmm0, xmm2, 0x50;	// 00 b1g1  0 b0g0r0
		shufps		xmm3, xmm1, 0x0a;	// 0 b4g4r4  000 b2
		shufps		xmm4, xmm5, 0xa5;	// g2r2 00  r1 000
		shufps		xmm0, xmm3, 0x88;	// 0 b4g4r4  000 b2  00 b1g1  0 b0g0r0
		movaps		xmm2, xmm1;		// 0 b7g7r7  0 b6g6r6  0 b5g5r5  0 b4g4r4
		movaps		xmm3, xmm1;
		movaps		xmm5, xmm1;
		movaps		xmm7, xmm1;
		pslld		xmm1, 0x18;		// @  @  r5 000  @
		psrld		xmm2, 0x08;		// @  @  00 b5g5  @
		psrld		xmm3, 0x10;		// @  000 b6  @  @
		pslld		xmm5, 0x10;		// @  g6r6 00  @  @  @
		pslld		xmm7, 0x08;		// b7g7r7 0  @  @  @
		shufps		xmm6, xmm1, 0x5f;	// r5 000  b3g3r3 0
		shufps		xmm2, xmm3, 0xa5;	// 000 b6  00 b5g5
		shufps		xmm5, xmm7, 0xfa;	// b7g7r7 0  g6r6 00
		shufps		xmm4, xmm6, 0x88;	// r5 000  b3g3r3 0  g2r2 00  r1 000
		orps		xmm5, xmm2;		// b7g7r7b6  g6r6b5g5
		orps		xmm4, xmm0;		// r5b4g4r4  b3g3r3b2  g2r2b1g1  r1b0g0r0
		shufps		xmm5, xmm5, 0x88;	// b7g7r7b6  g6r6b5g5  b7g7r7b6  g6r6b5g5
		movups		[edi], xmm4;
		movlps		[edi + 0x10], xmm5;
		add			esi, 0x20;
		add			edi, 0x18;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			edx, nUnitCount;
		and			edx, 0x07;
		test		edx, edx;
		jz			loop_end;
loop_unit:
		mov			ecx, 0x03;
		inc			esi;
		rep movsb;
		sub			edx, 0x01;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_4_2_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		movaps		xmm7, gc_dwMaskWLB;
		shr			edx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		andps		xmm0, xmm7;		// 32 位的高 16 位的低 8 位
		andps		xmm1, xmm7;
		andps		xmm2, xmm7;
		andps		xmm3, xmm7;
		packssdw	xmm0, xmm1;		// 16 位的低 8 位无效
		packssdw	xmm2, xmm3;
		psrlw		xmm0, 0x08;		// 32 位的高 16 位的低 8 位
		psrlw		xmm2, 0x08;
		packuswb	xmm0, xmm2;
		movups		[edi], xmm0;
		add			esi, 0x40;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
		add			esi, 0x02;
loop_unit:
		movsb;
		add			esi, 0x03;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_4_2_2(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		shr			edx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		psrld		xmm0, 0x10;		// 32 位的高 16 位
		psrld		xmm1, 0x10;
		psrld		xmm2, 0x10;
		psrld		xmm3, 0x10;
		packssdw	xmm0, xmm1;
		packssdw	xmm2, xmm3;
		movups		[edi], xmm0;
		movups		[edi + 0x10], xmm2;
		add			esi, 0x40;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		add			esi, 0x02;
		movsw;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_4_3_1(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		shr			edx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		psrld		xmm0, 0x18;		// 32 位的高 8 位
		psrld		xmm1, 0x18;
		psrld		xmm2, 0x18;
		psrld		xmm3, 0x18;
		packssdw	xmm0, xmm1;
		packssdw	xmm2, xmm3;
		packuswb	xmm0, xmm2;
		movups		[edi], xmm0;
		add			esi, 0x40;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		add			esi, 0x03;
		movsb;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
// 8
void memsplit_8_0_4(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		shr			edx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		shufps		xmm0, xmm1, 0x88;		// 64 位的低 32 位
		shufps		xmm2, xmm3, 0x88;
		movups		[edi], xmm0;
		movups		[edi + 0x10], xmm2;
		add			esi, 0x40;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		movsd;
		add			esi, 0x04;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_8_4_4(OUT void *des,			// 目标
					IN void *src,			// 源
					IN int nUnitCount)		// 分裂对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		shr			edx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		shufps		xmm0, xmm1, 0xdd;		// 64 位的高 32 位
		shufps		xmm2, xmm3, 0xdd;
		movups		[edi], xmm0;
		movups		[edi + 0x10], xmm2;
		add			esi, 0x40;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		add			esi, 0x04;
		movsd;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
//////////////////////////////////////////////////////////////////////////
// 合成字节
// 合成函数命名规则：第一个 n 表示合成对象的字节长，第二个 n 表示合成起始点，
//		第三个 n 表示合成字节数，都是 n 时表示任意合成
//////////////////////////////////////////////////////////////////////////
void memcompose_n_n_n(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount,		// 合成对象的个数
					  IN int nUnitBytes,		// 合成对象的字节长
					  IN int nCellStart,		// 合成起始点
					  IN int nCellCount)		// 合成字节数
{
	__asm {
		mov			edx, nUnitCount;
		test		edx, edx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		mov			eax, nUnitBytes;
		mov			ebx, nCellStart;
		mov			ecx, nCellCount;
		nop;
		add			edi, ebx;		// 合成起点
		sub			eax, ecx;		// 上一下 unit 合成完后到下一个 unit 合成起点的位置差
loop_unit:
		rep movsb;
		mov			ecx, nCellCount;
		add			edi, eax;
		sub			edx, 0x01;
		jnz			loop_unit;
loop_end:
	}
}
// 2
void memcompose_2_0_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		xorps		xmm0, xmm0;
		movaps		xmm7, gc_dwMaskWHB;		// des mask
		shr			ecx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi + 0x40];
		movups		xmm1, dword ptr [esi];
		movups		xmm2, dword ptr [edi];
		movups		xmm3, dword ptr [edi + 0x10];
		movaps		xmm4, xmm1;
		andps		xmm2, xmm7;		// 保留 16 位的高 8 位
		andps		xmm3, xmm7;
		punpcklbw	xmm1, xmm0;
		punpckhbw	xmm4, xmm0;
		orps		xmm1, xmm2;		// 写入 16 位的低 8 位
		orps		xmm4, xmm3;
		movups		[edi], xmm1;
		movups		[edi + 0x10], xmm4;
		add			esi, 0x10;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		movsb;
		inc			edi;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memcompose_2_1_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		movaps		xmm7, gc_dwMaskWLB;		// des mask
		shr			ecx, 0x04;		// 一次做 16 个 unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi + 0x40];
		movups		xmm1, dword ptr [esi];
		movups		xmm2, dword ptr [edi];
		movups		xmm3, dword ptr [edi + 0x10];
		xorps		xmm4, xmm4;
		xorps		xmm5, xmm5;
		andps		xmm2, xmm7;		// 保留 16 位的低 8 位
		andps		xmm3, xmm7;
		punpcklbw	xmm4, xmm1;
		punpckhbw	xmm5, xmm1;
		orps		xmm4, xmm2;		// 写入 16 位的高 8 位
		orps		xmm5, xmm3;
		movups		[edi], xmm4;
		movups		[edi + 0x10], xmm5;
		add			esi, 0x10;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_16;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x0f;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		inc			edi;
		movsb;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
// 4
void memcompose_4_0_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		xorps		xmm0, xmm0;
		movaps		xmm7, gc_dwMaskDLB;		// des mask
		pandn		xmm7, gc_dwMaskAll;
		shr			ecx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi + 0x40];
		movlps		xmm1, dword ptr [esi];
		movups		xmm2, dword ptr [edi];
		movups		xmm3, dword ptr [edi + 0x10];
		punpcklbw	xmm1, xmm0;
		andps		xmm2, xmm7;		// 保留 32 位的高 24 位
		andps		xmm3, xmm7;
		movaps		xmm4, xmm1;
		punpcklwd	xmm1, xmm0;
		punpckhwd	xmm4, xmm0;
		orps		xmm1, xmm2;		// 写入 32 位的低 8 位
		orps		xmm4, xmm3;
		movups		[edi], xmm1;
		movups		[edi + 0x10], xmm4;
		add			esi, 0x08;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		movsb;
		add			edi, 0x03;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memcompose_4_0_2(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		xorps		xmm0, xmm0;
		movaps		xmm7, gc_dwMaskWHB;		// des mask
		shr			ecx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi + 0x40];
		movups		xmm1, dword ptr [esi];
		movups		xmm2, dword ptr [edi];
		movups		xmm3, dword ptr [edi + 0x10];
		movaps		xmm4, xmm1;
		andps		xmm2, xmm7;		// 保留 32 位的高 16 位
		andps		xmm3, xmm7;
		punpcklwd	xmm1, xmm0;
		punpckhwd	xmm4, xmm0;
		orps		xmm1, xmm2;		// 写入 32 位的低 16 位
		orps		xmm4, xmm3;
		movups		[edi], xmm1;
		movups		[edi + 0x10], xmm4;
		add			esi, 0x10;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		movsw;
		add			edi, 0x02;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memcompose_4_1_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		xorps		xmm0, xmm0;
		movaps		xmm7, gc_dwMaskDLB;		// des mask
		pslld		xmm7, 0x08;
		pandn		xmm7, gc_dwMaskAll;
		shr			ecx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi + 0x40];
		movlps		xmm1, dword ptr [esi];
		movups		xmm2, dword ptr [edi];
		movups		xmm3, dword ptr [edi + 0x10];
		xorps		xmm4, xmm4;
		punpcklbw	xmm4, xmm1;
		andps		xmm2, xmm7;		// 保留 32 位的高 16 位与低 16 位的低 8 位
		andps		xmm3, xmm7;
		movaps		xmm1, xmm4;
		punpcklwd	xmm4, xmm0;
		punpckhwd	xmm1, xmm0;
		orps		xmm4, xmm2;		// 写入 32 位的低 16 位的高 8 位
		orps		xmm1, xmm3;
		movups		[edi], xmm4;
		movups		[edi + 0x10], xmm1;
		add			esi, 0x08;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
		inc			edi;
loop_unit:
		movsb;
		add			edi, 0x03;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memcompose_4_2_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		xorps		xmm0, xmm0;
		movaps		xmm7, gc_dwMaskDLB;		// des mask
		pslld		xmm7, 0x10;
		pandn		xmm7, gc_dwMaskAll;
		shr			ecx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi + 0x40];
		movlps		xmm1, dword ptr [esi];
		movups		xmm2, dword ptr [edi];
		movups		xmm3, dword ptr [edi + 0x10];
		punpcklbw	xmm1, xmm0;
		andps		xmm2, xmm7;		// 保留 32 位的高 16 位的高 8 位与低 16 位
		andps		xmm3, xmm7;
		xorps		xmm4, xmm4;
		xorps		xmm5, xmm5;
		punpcklwd	xmm4, xmm1;
		punpckhwd	xmm5, xmm1;
		orps		xmm4, xmm2;		// 写入 32 位的高 16 位的低 8 位
		orps		xmm5, xmm3;
		movups		[edi], xmm4;
		movups		[edi + 0x10], xmm5;
		add			esi, 0x08;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
		add			edi, 0x02;
loop_unit:
		movsb;
		add			edi, 0x03;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memcompose_4_2_2(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		movaps		xmm7, gc_dwMaskWLB;		// des mask
		shr			ecx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi + 0x40];
		movups		xmm1, dword ptr [esi];
		movups		xmm2, dword ptr [edi];
		movups		xmm3, dword ptr [edi + 0x10];
		andps		xmm2, xmm7;		// 保留 32 位的低 16 位
		andps		xmm3, xmm7;
		xorps		xmm4, xmm4;
		xorps		xmm5, xmm5;
		punpcklwd	xmm4, xmm1;
		punpckhwd	xmm5, xmm1;
		orps		xmm4, xmm2;		// 写入 32 位的高 16 位
		orps		xmm5, xmm3;
		movups		[edi], xmm4;
		movups		[edi + 0x10], xmm5;
		add			esi, 0x10;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		add			edi, 0x02;
		movsw;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memcompose_4_3_1(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		movaps		xmm7, gc_dwMaskRGB;	// des mask
		shr			ecx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi + 0x40];
		movlps		xmm1, dword ptr [esi];
		movups		xmm2, dword ptr [edi];
		movups		xmm3, dword ptr [edi + 0x10];
		xorps		xmm0, xmm0;
		punpcklbw	xmm0, xmm1;
		andps		xmm2, xmm7;		// 保留 32 位的低 24 位
		andps		xmm3, xmm7;
		xorps		xmm4, xmm4;
		xorps		xmm5, xmm5;
		punpcklwd	xmm4, xmm0;
		punpckhwd	xmm5, xmm0;
		orps		xmm4, xmm2;		// 写入 32 位的高 8 位
		orps		xmm5, xmm3;
		movups		[edi], xmm4;
		movups		[edi + 0x10], xmm5;
		add			esi, 0x08;
		add			edi, 0x20;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		add			edi, 0x03;
		movsb;
		dec			ecx;			// lsb @ 2009-11-16
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
// 8
void memcompose_8_0_4(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		shr			edx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x80];
		prefetchnta	byte ptr [edi + 0x40];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm4, dword ptr [edi];
		movups		xmm5, dword ptr [edi + 0x10];
		movups		xmm6, dword ptr [edi + 0x20];
		movups		xmm7, dword ptr [edi + 0x30];
		shufps		xmm4, xmm0, 0x4d;		// 保留 64 位的高 32 位，写入低 32 位
		shufps		xmm5, xmm0, 0xed;
		shufps		xmm6, xmm1, 0x4d;
		shufps		xmm7, xmm0, 0xed;
		shufps		xmm4, xmm4, 0x72;
		shufps		xmm5, xmm5, 0x72;
		shufps		xmm6, xmm6, 0x72;
		shufps		xmm7, xmm7, 0x72;
		movups		[edi], xmm4;
		movups		[edi + 0x10], xmm5;
		movups		[edi + 0x20], xmm6;
		movups		[edi + 0x30], xmm7;
		add			esi, 0x20;
		add			edi, 0x40;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		movsd;
		add			edi, 0x04;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}
void memcompose_8_4_4(OUT void *des,			// 目标
					  IN void *src,				// 源
					  IN int nUnitCount)		// 合成对象的个数
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [esi + 0x80];
		prefetchnta	byte ptr [edi];
		prefetchnta	byte ptr [edi + 0x40];
		shr			edx, 0x03;		// 一次做 8 个 unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x80];
		prefetchnta	byte ptr [edi + 0x40];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm4, dword ptr [edi];
		movups		xmm5, dword ptr [edi + 0x10];
		movups		xmm6, dword ptr [edi + 0x20];
		movups		xmm7, dword ptr [edi + 0x30];
		shufps		xmm4, xmm0, 0x48;		// 保留 64 位的低 32 位，写入高 32 位
		shufps		xmm5, xmm0, 0xe8;
		shufps		xmm6, xmm1, 0x48;
		shufps		xmm7, xmm0, 0xe8;
		shufps		xmm4, xmm4, 0xd8;
		shufps		xmm5, xmm5, 0xd8;
		shufps		xmm6, xmm6, 0xd8;
		shufps		xmm7, xmm7, 0xd8;
		movups		[edi], xmm4;
		movups		[edi + 0x10], xmm5;
		movups		[edi + 0x20], xmm6;
		movups		[edi + 0x30], xmm7;
		add			esi, 0x20;
		add			edi, 0x40;
		sub			ecx, 0x01;
		jnz			loop_unit_8;
loop_unit_m:
		mov			ecx, nUnitCount;
		and			ecx, 0x07;
		test		ecx, ecx;
		jz			loop_end;
loop_unit:
		add			edi, 0x04;
		movsd;
		test		ecx, ecx;
		jnz			loop_unit;
loop_end:
	}
}

//////////////////////////////////////////////////////////////////////////
// 对一行 BGRA32 Buffer 进行 Alpha 混色
// 优化计算透明度 MA = SA + DA - SA * DA / 255 = SA + DA - (DA * SA + 255) >> 8
// 查表得到增益 KA = SA / MA = SA * 255 / (SA * 255 + DA * 255 - SA * DA)
// 优化计算颜色值 MC = (SC * SA + DC * (1 - SA / 255) * DA) / MA = (SC - DC) * KA + DC
// 增益表的偏移地址 Offset = (SA * 256 + DA) * 4
//////////////////////////////////////////////////////////////////////////
void bgra32_line_mix(IN OUT void *des,		// 目标
					 IN void *src,			// 源
					 IN int na16,			// 宽度除以 16 的商
					 IN int nm16,			// 宽度除以 16 的余数
					 IN void *ka_table)		// 256 * 256 * sizeof(float) 的 KA 表
{
	_asm{
		mov			ecx, na16;
		mov			edx, ka_table;
		mov			edi, des;
		mov			esi, src;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_a8;
loop_a16:
		//////////////////////////////////////////////////////////////////////////
		// 根据 (sa * 256 + da) * 4 查增益表，由于表为 DWORD 型，转成字节时需要乘以长度
		movdqu		xmm6, [esi];	// src(A3R3G3B3 A2R2G2B2 A1R1G1B1 A0R0G0B0)
		movdqu		xmm7, [edi];	// des(A3R3G3B3 A2R2G2B2 A1R1G1B1 A0R0G0B0)
		movdqa		xmm4, xmm6;
		movdqa		xmm5, xmm7;
		psrld		xmm4, 0x18;		// sa(src alpha A3 A2 A1 A0)
		psrld		xmm5, 0x18;		// da(des alpha A3 A2 A1 A0)

		movd		xmm1, edx;		// 表头 0 0 0 table
		pshufd		xmm1, xmm1, 0x00;	// table table table table

		movdqa		xmm2, xmm4;		// sa
		movdqa		xmm3, xmm4;		// sa
		pmullw		xmm4, xmm5;		// sa * da
		paddd		xmm3, xmm5;		// sa + da

		pslld		xmm2, 0x0a;		// sa * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// (sa * 256 + da) * 4 存放 Offset3 Offset2 Offset1 Offset0
		paddd		xmm2, xmm1;		// 增益地址 ptr3 ptr2 ptr1 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		psubd		xmm3, xmm4;		// 计算 MA = SA + DA - (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// 存放结果 A000 A000 A000 A000
		//////////////////////////////////////////////////////////////////////////
		// 挨个像素取出增益值
		movd		eax, xmm2;		// 第 0 个像素的增益 ptr0
		pshufd		xmm2, xmm2, 0x39;	// ptr0 ptr3 ptr2 ptr1
		movd		xmm4, [eax];	// 存放 0 0 0 K0
		movd		eax, xmm2;		// 第 1 个像素的增益 ptr1
		pshufd		xmm2, xmm2, 0x39;	// ptr1 ptr0 ptr3 ptr2
		movd		xmm5, [eax];	// 存放 0 0 0 K1
		movd		eax, xmm2;		// 第 2 个像素的增益 ptr2
		pshufd		xmm2, xmm2, 0x39;	// ptr2 ptr1 ptr0 ptr3
		movd		xmm1, [eax];	// 存放 0 0 0 K2
		movd		eax, xmm2;		// 第 3 个像素的增益 ptr3
		shufps		xmm4, xmm5, 0x00;	// K1 K1 K0 K0
		movd		xmm2, [eax];	// 存放 0 0 0 K3
		shufps		xmm1, xmm2, 0x00;	// K3 K3 K2 K2
		packssdw	xmm4, xmm1;		// K3K3 K2K2 K1K1 K0K0
		//////////////////////////////////////////////////////////////////////////
		// 先计算 R B 再单独计算 G
		movdqa		xmm2, xmm6;
		movdqa		xmm5, xmm7;
		psrld		xmm6, 0x08;
		psrld		xmm7, 0x08;
		pand		xmm2, gc_dwMaskWLB;	// src RB(0R0B 0R0B 0R0B 0R0B)
		pand		xmm5, gc_dwMaskWLB;	// des RB(0R0B 0R0B 0R0B 0R0B)
		pand		xmm6, gc_dwMaskDLB;	// src G(000G 000G 000G 000G)
		pand		xmm7, gc_dwMaskDLB;	// des G(000G 000G 000G 000G)

		movdqa		xmm0, xmm2;		// srb
		movdqa		xmm1, xmm6;		// sg
		pcmpgtw		xmm2, xmm5;		// (srb > drb) ? -1 : 0
		pcmpgtw		xmm6, xmm7;		// (sg > dg) ? -1 : 0
		psubw		xmm0, xmm5;		// srb - drb
		psubw		xmm1, xmm7;		// sg - dg
		psubw		xmm0, xmm2;		// srb - drb + compRB
		psubw		xmm1, xmm6;		// sg - dg + compG
		paddw		xmm0, xmm0;		// (srb - drb + compRB) * 2 浮点乘法到整型乘法的转变
		paddw		xmm1, xmm1;		// (sg - dg + compG) * 2 浮点乘法到整型乘法的转变
		pmulhw		xmm0, xmm4;		// (srb - drb) * KA
		pmulhw		xmm1, xmm4;		// (sg - dg) * KA

		paddw		xmm0, xmm5;		// (srb - drb) * KA + drb
		paddw		xmm1, xmm7;		// (sg - dg) * KA + dg
		pslld		xmm1, 0x08;
		por			xmm3, xmm0;		// mix |= (mrb) 存放 AR0B AR0B AR0B AR0B
		por			xmm3, xmm1;		// mix |= (mg << 8) 存放 ARGB ARGB ARGB ARGB
		//////////////////////////////////////////////////////////////////////////
		// 写入 Mix 结果并开始下一个连续的四个像素计算
		movdqu		[edi], xmm3;
		add			esi, 0x10;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_a8:
		mov			ecx, nm16;
		pxor		xmm0, xmm0;		// 0 0 0 0
		test		ecx, 0x00000008;
		jz			loop_a4;
		//////////////////////////////////////////////////////////////////////////
		movsd		xmm6, [esi];	// src(0000 0000 A1R1G1B1 A0R0G0B0)
		movsd		xmm7, [edi];	// des(0000 0000 A1R1G1B1 A0R0G0B0)
		movdqa		xmm4, xmm6;
		movdqa		xmm5, xmm7;
		psrld		xmm4, 0x18;		// sa(src alpha 0 0 A1 A0)
		psrld		xmm5, 0x18;		// da(des alpha 0 0 A1 A0)

		movd		xmm1, edx;		// 表头 0 0 0 table
		pshufd		xmm1, xmm1, 0x00;	// table table table table

		movdqa		xmm2, xmm4;		// sa
		movdqa		xmm3, xmm4;		// sa
		pmullw		xmm4, xmm5;		// sa * da
		paddd		xmm3, xmm5;		// sa + da

		pslld		xmm2, 0x0a;		// sa * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// (sa * 256 + da) * 4 存放 0 0 Offset1 Offset0
		paddd		xmm2, xmm1;		// 增益地址 0 0 ptr1 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		psubd		xmm3, xmm4;		// 计算 MA = SA + DA - (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// 存放结果 0000 0000 A000 A000
		//////////////////////////////////////////////////////////////////////////
		// 挨个像素取出增益值
		movd		eax, xmm2;		// 第 0 个像素的增益 ptr0
		pshufd		xmm2, xmm2, 0x39;	// ptr0 0 0 ptr1
		movd		xmm4, [eax];	// 存放 0 0 0 K0
		movd		eax, xmm2;		// 第 1 个像素的增益 ptr1
		movd		xmm5, [eax];	// 存放 0 0 0 K1
		shufps		xmm4, xmm4, 0x00;	// K0 K0 K0 K0
		shufps		xmm5, xmm5, 0x00;	// K1 K1 K1 K1
		packssdw	xmm4, xmm5;		// K1K1 K1K1 K0K0 K0K0
		//////////////////////////////////////////////////////////////////////////
		// 同时计算 R G B
		pand		xmm6, gc_dwMaskRGB;// src RGB(0000 0000 0RGB 0RGB)
		punpcklbw	xmm6, xmm0;		// 000R 0G0B 000R 0G0B
		pand		xmm7, gc_dwMaskRGB;// des RGB(0000 0000 0RGB 0RGB)
		punpcklbw	xmm7, xmm0;		// 000R 0G0B 000R 0G0B
		movdqa		xmm2, xmm6;		// src RGB
		pcmpgtw		xmm2, xmm7;		// (src > des) ? -1 : 0
		psubw		xmm6, xmm7;		// src - des
		psubw		xmm6, xmm2;		// src - des + compRGB
		paddw		xmm6, xmm6;		// (src - des + compRGB) * 2 浮点乘法到整型乘法的转变
		pmulhw		xmm6, xmm4;		// (src - des) * KA
		paddw		xmm6, xmm7;		// (src - des) * KA + des
		packuswb	xmm6, xmm0;		// 0000 0000 0RGB 0RGB
		por			xmm3, xmm6;		// mix |= (RGB) 存放 0000 0000 ARGB ARGB
		//////////////////////////////////////////////////////////////////////////
		// 写入 Mix 结果
		movsd		[edi], xmm3;
		add			esi, 0x08;
		add			edi, 0x08;
loop_a4:
		test		ecx, 0x00000004;
		jz			loop_end;
		//////////////////////////////////////////////////////////////////////////
		movd		xmm1, edx;		// 表头 0 0 0 table
		movd		xmm6, [esi];	// src(0000 0000 0000 ARGB)
		movd		xmm7, [edi];	// des(0000 0000 0000 ARGB)
		movdqa		xmm4, xmm6;
		movdqa		xmm5, xmm7;
		psrld		xmm4, 0x18;		// sa(src alpha 0 0 0 A0)
		psrld		xmm5, 0x18;		// da(des alpha 0 0 0 A0)

		movdqa		xmm2, xmm4;		// sa
		movdqa		xmm3, xmm4;		// sa
		pmullw		xmm4, xmm5;		// sa * da
		paddd		xmm3, xmm5;		// sa + da

		pslld		xmm2, 0x0a;		// sa * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// (sa * 256 + da) * 4 存放 0 0 0 Offset0
		paddd		xmm2, xmm1;		// 增益地址 0 0 0 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		psubd		xmm3, xmm4;		// 计算 MA = SA + DA - (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// 存放结果 0000 0000 0000 A000
		//////////////////////////////////////////////////////////////////////////
		// 挨个像素取出增益值
		movd		eax, xmm2;		// 第 0 个像素的增益 ptr0
		movd		xmm4, [eax];	// 存放 0 0 0 K0
		shufps		xmm4, xmm4, 0x00;	// K0 K0 K0 K0
		packssdw	xmm4, xmm0;		// 0 0 K0K0 K0K0

		pand		xmm6, gc_dwMaskRGB;// src RGB(0000 0000 0000 0RGB)
		punpcklbw	xmm6, xmm0;		// 0000 0000 000R 0G0B
		pand		xmm7, gc_dwMaskRGB;// des RGB(0000 0000 0000 0RGB)
		punpcklbw	xmm7, xmm0;		// 0000 0000 000R 0G0B
		movdqa		xmm2, xmm6;		// src RGB
		pcmpgtw		xmm2, xmm7;		// (src > des) ? -1 : 0
		psubw		xmm6, xmm7;		// src - des
		psubw		xmm6, xmm2;		// src - des + compRGB
		paddw		xmm6, xmm6;		// (src - des + compRGB) * 2 浮点乘法到整型乘法的转变
		pmulhw		xmm6, xmm4;		// (src - des) * KA
		paddw		xmm6, xmm7;		// (src - des) * KA + des
		packuswb	xmm6, xmm0;		// 0000 0000 0000 0RGB
		por			xmm3, xmm6;		// mix |= (RGB) 存放 0000 0000 0000 ARGB
		//////////////////////////////////////////////////////////////////////////
		// 写入 Mix 结果
		movd		[edi], xmm3;
loop_end:
	}
}

//////////////////////////////////////////////////////////////////////////
// 对一行 BGRA32 Buffer 进行 Alpha 反混色
// 优化计算透明度 MA = (255 - SA) + DA - (255 - SA) * DA / 255 = 255 - SA + (DA * SA + 255) >> 8
// 查表得到增益 KA = (255 - SA) / MA = (65535 - SA * 255) / (65535 - SA * 255 + SA * DA)
// 优化计算颜色值 MC = (SC * (255 - SA) + DC * (1 - (255 - SA) / 255) * DA) / MA = (SC - DC) * KA + DC
// 增益表的偏移地址 Offset = ((255 - SA) * 256 + DA) * 4
//////////////////////////////////////////////////////////////////////////
void bgra32_line_notmix(IN OUT void *des,		// 目标
						IN void *src,			// 源
						IN int na16,			// 宽度除以 16 的商
						IN int nm16,			// 宽度除以 16 的余数
						IN void *ka_table)		// 256 * 256 * sizeof(float) 的 KA 表
{
	_asm{
		mov			ecx, na16;
		mov			edx, ka_table;
		mov			edi, des;
		mov			esi, src;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_a8;
loop_a16:
		//////////////////////////////////////////////////////////////////////////
		// 根据 ((255 - sa) * 256 + da) * 4 查增益表，由于表为 DWORD 型，转成字节时需要乘以长度
		movdqu		xmm6, [esi];	// src(A3R3G3B3 A2R2G2B2 A1R1G1B1 A0R0G0B0)
		movdqu		xmm7, [edi];	// des(A3R3G3B3 A2R2G2B2 A1R1G1B1 A0R0G0B0)
		movdqa		xmm4, xmm6;
		movdqa		xmm5, xmm7;
		movdqa		xmm2, gc_dwMaskDLB;	// 255
		psrld		xmm4, 0x18;		// sa(src alpha A3 A2 A1 A0)
		psrld		xmm5, 0x18;		// da(des alpha A3 A2 A1 A0)

		movd		xmm1, edx;		// 表头 0 0 0 table
		pshufd		xmm1, xmm1, 0x00;	// table table table table

		psubd		xmm2, xmm4;		// 255 - sa
		pmullw		xmm4, xmm5;		// sa * da
		movdqa		xmm3, xmm2;		// 255 - sa

		pslld		xmm2, 0x0a;		// (255 - sa) * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// ((255 - sa) * 256 + da) * 4 存放 Offset3 Offset2 Offset1 Offset0
		paddd		xmm2, xmm1;		// 增益地址 ptr3 ptr2 ptr1 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		paddd		xmm3, xmm4;		// 计算 MA = 255 - SA + (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// 存放结果 A000 A000 A000 A000
		//////////////////////////////////////////////////////////////////////////
		// 挨个像素取出增益值
		movd		eax, xmm2;		// 第 0 个像素的增益 ptr0
		pshufd		xmm2, xmm2, 0x39;	// ptr0 ptr3 ptr2 ptr1
		movd		xmm4, [eax];	// 存放 0 0 0 K0
		movd		eax, xmm2;		// 第 1 个像素的增益 ptr1
		pshufd		xmm2, xmm2, 0x39;	// ptr1 ptr0 ptr3 ptr2
		movd		xmm5, [eax];	// 存放 0 0 0 K1
		movd		eax, xmm2;		// 第 2 个像素的增益 ptr2
		pshufd		xmm2, xmm2, 0x39;	// ptr2 ptr1 ptr0 ptr3
		movd		xmm1, [eax];	// 存放 0 0 0 K2
		movd		eax, xmm2;		// 第 3 个像素的增益 ptr3
		shufps		xmm4, xmm5, 0x00;	// K1 K1 K0 K0
		movd		xmm2, [eax];	// 存放 0 0 0 K3
		shufps		xmm1, xmm2, 0x00;	// K3 K3 K2 K2
		packssdw	xmm4, xmm1;		// K3K3 K2K2 K1K1 K0K0
		//////////////////////////////////////////////////////////////////////////
		// 先计算 R B 再单独计算 G
		movdqa		xmm2, xmm6;
		movdqa		xmm5, xmm7;
		psrld		xmm6, 0x08;
		psrld		xmm7, 0x08;
		pand		xmm2, gc_dwMaskWLB;	// src RB(0R0B 0R0B 0R0B 0R0B)
		pand		xmm5, gc_dwMaskWLB;	// des RB(0R0B 0R0B 0R0B 0R0B)
		pand		xmm6, gc_dwMaskDLB;	// src G(000G 000G 000G 000G)
		pand		xmm7, gc_dwMaskDLB;	// des G(000G 000G 000G 000G)

		movdqa		xmm0, xmm2;		// srb
		movdqa		xmm1, xmm6;		// sg
		pcmpgtw		xmm2, xmm5;		// (srb > drb) ? -1 : 0
		pcmpgtw		xmm6, xmm7;		// (sg > dg) ? -1 : 0
		psubw		xmm0, xmm5;		// srb - drb
		psubw		xmm1, xmm7;		// sg - dg
		psubw		xmm0, xmm2;		// srb - drb + compRB
		psubw		xmm1, xmm6;		// sg - dg + compG
		paddw		xmm0, xmm0;		// (srb - drb + compRB) * 2 浮点乘法到整型乘法的转变
		paddw		xmm1, xmm1;		// (sg - dg + compG) * 2 浮点乘法到整型乘法的转变
		pmulhw		xmm0, xmm4;		// (srb - drb) * KA
		pmulhw		xmm1, xmm4;		// (sg - dg) * KA

		paddw		xmm0, xmm5;		// (srb - drb) * KA + drb
		paddw		xmm1, xmm7;		// (sg - dg) * KA + dg
		pslld		xmm1, 0x08;
		por			xmm3, xmm0;		// mix |= (mrb) 存放 AR0B AR0B AR0B AR0B
		por			xmm3, xmm1;		// mix |= (mg << 8) 存放 ARGB ARGB ARGB ARGB
		//////////////////////////////////////////////////////////////////////////
		// 写入 Mix 结果并开始下一个连续的四个像素计算
		movdqu		[edi], xmm3;
		add			esi, 0x10;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_a8:
		mov			ecx, nm16;
		pxor		xmm0, xmm0;		// 0 0 0 0
		test		ecx, 0x00000008;
		jz			loop_a4;
		//////////////////////////////////////////////////////////////////////////
		// 根据 ((255 - sa) * 256 + da) * 4 查增益表，由于表为 DWORD 型，转成字节时需要乘以长度
		movsd		xmm6, [esi];	// src(0000 0000 A1R1G1B1 A0R0G0B0)
		movsd		xmm7, [edi];	// des(0000 0000 A1R1G1B1 A0R0G0B0)
		movdqa		xmm4, xmm6;
		movdqa		xmm5, xmm7;
		movdqa		xmm2, gc_dwMaskDLB;	// 255
		psrld		xmm4, 0x18;		// sa(src alpha 0 0 A1 A0)
		psrld		xmm5, 0x18;		// da(des alpha 0 0 A1 A0)

		movd		xmm1, edx;		// 表头 0 0 0 table
		pshufd		xmm1, xmm1, 0x00;	// table table table table

		psubd		xmm2, xmm4;		// 255 - sa
		pmullw		xmm4, xmm5;		// sa * da
		movdqa		xmm3, xmm2;		// 255 - sa

		pslld		xmm2, 0x0a;		// (255 - sa) * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// ((255 - sa) * 256 + da) * 4 存放 0 0 Offset1 Offset0
		paddd		xmm2, xmm1;		// 增益地址 0 0 ptr1 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		paddd		xmm3, xmm4;		// 计算 MA = 255 - SA + (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// 存放结果 0000 0000 A000 A000
		//////////////////////////////////////////////////////////////////////////
		// 挨个像素取出增益值
		movd		eax, xmm2;		// 第 0 个像素的增益 ptr0
		pshufd		xmm2, xmm2, 0x39;	// ptr0 0 0 ptr1
		movd		xmm4, [eax];	// 存放 0 0 0 K0
		movd		eax, xmm2;		// 第 1 个像素的增益 ptr1
		movd		xmm5, [eax];	// 存放 0 0 0 K1
		shufps		xmm4, xmm4, 0x00;	// K0 K0 K0 K0
		shufps		xmm5, xmm5, 0x00;	// K1 K1 K1 K1
		packssdw	xmm4, xmm5;		// K1K1 K1K1 K0K0 K0K0
		//////////////////////////////////////////////////////////////////////////
		// 同时计算 R G B
		pand		xmm6, gc_dwMaskRGB;// src RGB(0000 0000 0RGB 0RGB)
		punpcklbw	xmm6, xmm0;		// 000R 0G0B 000R 0G0B
		pand		xmm7, gc_dwMaskRGB;// des RGB(0000 0000 0RGB 0RGB)
		punpcklbw	xmm7, xmm0;		// 000R 0G0B 000R 0G0B
		movdqa		xmm2, xmm6;		// src RGB
		pcmpgtw		xmm2, xmm7;		// (src > des) ? -1 : 0
		psubw		xmm6, xmm7;		// src - des
		psubw		xmm6, xmm2;		// src - des + compRGB
		paddw		xmm6, xmm6;		// (src - des + compRGB) * 2 浮点乘法到整型乘法的转变
		pmulhw		xmm6, xmm4;		// (src - des) * KA
		paddw		xmm6, xmm7;		// (src - des) * KA + des
		packuswb	xmm6, xmm0;		// 0000 0000 0RGB 0RGB
		por			xmm3, xmm6;		// mix |= (RGB) 存放 0000 0000 ARGB ARGB
		//////////////////////////////////////////////////////////////////////////
		// 写入 Mix 结果
		movsd		[edi], xmm3;
		add			esi, 0x08;
		add			edi, 0x08;
loop_a4:
		test		ecx, 0x00000004;
		jz			loop_end;
		//////////////////////////////////////////////////////////////////////////
		// 根据 ((255 - sa) * 256 + da) * 4 查增益表，由于表为 DWORD 型，转成字节时需要乘以长度
		movd		xmm1, edx;		// 表头 0 0 0 table
		movd		xmm6, [esi];	// src(0000 0000 0000 ARGB)
		movd		xmm7, [edi];	// des(0000 0000 0000 ARGB)
		movdqa		xmm4, xmm6;
		movdqa		xmm5, xmm7;
		movdqa		xmm2, gc_dwMaskDLB;	// 255
		psrld		xmm4, 0x18;		// sa(src alpha 0 0 0 A0)
		psrld		xmm5, 0x18;		// da(des alpha 0 0 0 A0)

		psubd		xmm2, xmm4;		// 255 - sa
		pmullw		xmm4, xmm5;		// sa * da
		movdqa		xmm3, xmm2;		// 255 - sa

		pslld		xmm2, 0x0a;		// (255 - sa) * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// ((255 - sa) * 256 + da) * 4 存放 0 0 0 Offset0
		paddd		xmm2, xmm1;		// 增益地址 0 0 0 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		paddd		xmm3, xmm4;		// 计算 MA = 255 - SA + (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// 存放结果 0000 0000 0000 A000
		//////////////////////////////////////////////////////////////////////////
		// 挨个像素取出增益值
		movd		eax, xmm2;		// 第 0 个像素的增益 ptr0
		movd		xmm4, [eax];	// 存放 0 0 0 K0
		shufps		xmm4, xmm4, 0x00;	// K0 K0 K0 K0
		packssdw	xmm4, xmm0;		// 0 0 K0K0 K0K0

		pand		xmm6, gc_dwMaskRGB;// src RGB(0000 0000 0000 0RGB)
		punpcklbw	xmm6, xmm0;		// 0000 0000 000R 0G0B
		pand		xmm7, gc_dwMaskRGB;// des RGB(0000 0000 0000 0RGB)
		punpcklbw	xmm7, xmm0;		// 0000 0000 000R 0G0B
		movdqa		xmm2, xmm6;		// src RGB
		pcmpgtw		xmm2, xmm7;		// (src > des) ? -1 : 0
		psubw		xmm6, xmm7;		// src - des
		psubw		xmm6, xmm2;		// src - des + compRGB
		paddw		xmm6, xmm6;		// (src - des + compRGB) * 2 浮点乘法到整型乘法的转变
		pmulhw		xmm6, xmm4;		// (src - des) * KA
		paddw		xmm6, xmm7;		// (src - des) * KA + des
		packuswb	xmm6, xmm0;		// 0000 0000 0000 0RGB
		por			xmm3, xmm6;		// mix |= (RGB) 存放 0000 0000 0000 ARGB
		//////////////////////////////////////////////////////////////////////////
		// 写入 Mix 结果
		movd		[edi], xmm3;
loop_end:
	}
}

//////////////////////////////////////////////////////////////////////////
// 采用双线性插值法对 BGRA32 Buffer 进行缩放，使用权重与索引映射表进行逐行逐个像素处
// 通过索引映射表得到目标行像素映射到源行的起始位置，利用以它为首的四个相邻像素进行加权插值
// 计算公式		S00 * x * y				S10 * (1 - x) * y
//				S01 * x * (1 - y)		S11 * (1 - x) * (1 - y)
// D = S11 + (S00 - S10 - S01 + S11) * x * y + (S01 - S11) * x + (S10 - S11) * y
//
// lsb @ 2009-11-26 算法修正
// S00 * (1 - x) * (1 - y)			S10 * x * (1 - y)
// S01 * x * (1 - y)				S11 * x * y
//////////////////////////////////////////////////////////////////////////
void bgra32_bl_copy_line(OUT void* des,		// 目标拷贝行
						 IN int dcw,		// 目标拷贝行的宽度
						 IN void* src,		// 源拷贝行
						 IN int src_pitch,	// 源图一行的字节数
						 IN void* pPowerX,	// 目标映射到源的 X 方向上的列权重
						 IN void* pMapX,	// 目标映射到源的 X 方向上的列索引
						 IN DWORD powerY)	// 目标行映射到源行的行权重
{
	__asm
	{
		mov			edi_ptr, des;
		pxor		xmm7, xmm7;
		movdqa		xmm6, gc_dwMaskWBIT;// 1
		movd		xmm5, powerY;		// 1-y
		movdqa		xmm4, xmm6;			// 1
		psllw		xmm6, 15;			// 32768
		pshufd		xmm5, xmm5, 0;
		psllw		xmm4, 1;
		psubusw		xmm6, xmm5;			// y
		mov			ecx, dcw;
		mov			eax_ptr, pPowerX;
		mov			edx_ptr, pMapX;
		test		ecx, ecx;
		jz			loop_0;
loop_1:
		mov			esi_ptr, [edx_ptr];
		add			esi_ptr, src;		// 00的地址
		movsd		xmm0, [esi_ptr];	// 10 00
		add			esi_ptr, src_pitch;
		movsd		xmm1, [esi_ptr];	// 11 01
		punpcklbw	xmm0, xmm7;
		punpcklbw	xmm1, xmm7;
		psllw		xmm0, 2;
		psllw		xmm1, 2;			// 放大2位
		pmulhuw		xmm0, xmm5;			// S@0 * (1 - y)
		pmulhuw		xmm1, xmm6;			// S@1 * y
		paddusw		xmm0, xmm4;
		movd		xmm2, [eax_ptr];	// x(1-x)
		paddusw		xmm0, xmm1;			// S0@ + S1@ 放大1位
		pshuflw		xmm2, xmm2, 0x50;
		add			eax_ptr, PTR_BYTES_1;
		pshufd		xmm2, xmm2, 0x50;	// xx xx (1-x)(1-x) (1-x)(1-x)
		add			edx_ptr, PTR_BYTES_1;
		pmulhuw		xmm0, xmm2;
		movdqa		xmm1, xmm0;
		punpckhqdq	xmm1, xmm1;
		paddusw		xmm0, xmm1;
		packuswb	xmm0, xmm0;
		movd		[edi_ptr], xmm0;
		add			edi_ptr, PTR_BYTES_1;
		dec			ecx;
		jnz			loop_1;
loop_0:
	}
//	_asm{
//		pxor		xmm0, xmm0;			// 0 0 0 0
//		movd		xmm7, powerY;		// 行权重 0000 0000 0000 0y0y
//		movdqa		xmm6, gc_dwMaskAll;	// ffffffff ffffffff ffffffff ffffffff
//		shufps		xmm7, xmm7, 0x00;	// 0y0y 0y0y 0y0y 0y0y
//		psubusw		xmm6, xmm7;			// 1 - y
//
//		mov 		edi, des;			// 目标行
//		mov 		esi, pPowerX;		// X 方向上的权重表
//		mov 		ecx, dcw;			// 行宽
//		mov 		edx, pMapX;			// X 方向上的索引表
//		test		ecx, ecx;
//		jnz			loop_pixel;
//		jmp			loop_end;
//loop_pixel:
//		mov			eax, [edx];			// 目标行上的映射索引 map
//		add			eax, src;			// 第一像素的地址 src + map
//
//		movsd		xmm1, [eax];		// 0 0 S10 S00
//		punpcklbw	xmm1, xmm0;			// 0a0r 0g0b 0a0r 0g0b
//		paddw		xmm1, gc_dwMaskWBIT;	// 误差补偿
//		pmulhuw		xmm1, xmm7;			// ((S@0 + comp) * y) >> 16
//
//		add			eax, src_pitch;		// 下一行的地址
//		add			edx, 0x04;
//
//		movsd		xmm2, [eax];		// 0 0 S11 S01
//		punpcklbw	xmm2, xmm0;			// 0a0r 0g0b 0a0r 0g0b
//		paddw		xmm2, gc_dwMaskWBIT;	// 误差补偿
//		pmulhuw		xmm2, xmm6;			// ((S@1 + comp) * (65535 - y)) >> 16
//
//		movd		xmm4, [esi];		// 目标行上的映射权重 0 0 0 (1-x)x
//		shufps		xmm4, xmm4, 0x00;	// (1-x)x (1-x)x (1-x)x (1-x)x
//		pshuflw		xmm4, xmm4, 0x00;	// (1-x)x (1-x)x xx xx
//		pshufhw		xmm4, xmm4, 0x55;	// (1-x)(1-x) (1-x)(1-x) xx xx
//
//		paddusw		xmm1, gc_dwMaskWBIT;
//		paddusw		xmm1, xmm2;			// S@0 * y + S@1 * (1 - y)
//		pmulhuw		xmm1, xmm4;			// S1@ * (1 - x)	S0@ * x
//		pshufd		xmm2, xmm1, 0x0e;	// S0@ * x			S1@ * (1 - x)
//		paddusw		xmm1, xmm2;			// 0a0r 0g0b 0a0r 0g0b
//		packuswb	xmm1, xmm1;			// D0 D0 D0 D0
//		movd		[edi], xmm1;
//
//		add			esi, 0x04;
//		add			edi, 0x04;
//		sub			ecx, 0x01;
//		jnz			loop_pixel;
//loop_end:
//	}
}

//////////////////////////////////////////////////////////////////////////
// 采用加权均值滤波法对 BGRA32 Buffer 进行缩放，使用权重与索引映射表进行逐行逐个像素处理
// 先根据垂直方向上的映射对源进行行加权平均运算，将结果存放于临时缓存中
// 而后根据水平方向上的映射对临时缓存进行列加权平均运算，得到每个像素的值
//////////////////////////////////////////////////////////////////////////
void bgra32_mean_copy_add_y(OUT void* des,			// 临时缓存行（是源行拷贝宽度的四倍）
							IN void* src,			// 源拷贝起始行
							IN int scw,				// 源拷贝宽度
							IN int src_pitch,		// 源行字节数
							IN void* pPowerY,		// 目标某行映射的垂直方向上的权重表
							IN void* pCoefH,		// 垂直方向上的缩放系数
							IN int nMapY)			// 目标一行映射到源的行数
{
	_asm{
		mov			edi, des;
		mov			esi, src;
		mov			ecx, scw;
		mov			edx, pPowerY;
		movss		xmm2, [edx];		// 垂直方向上的首权重 0 0 0 powStart
		movss		xmm3, [edx + 0x04];	// 垂直方向上的尾权重 0 0 0 powEnd
		shufps		xmm2, xmm2, 0x00;	// power power power power
		shufps		xmm3, xmm3, 0x00;	// power power power power
		mov			eax, pCoefH;
		pxor		xmm0, xmm0;			// 0 0 0 0
		movaps		xmm7, [eax];		// 缩放系数 coef coef coef coef
loop_pixel:
		mov			eax, nMapY;			// 目标行映射到源的行数
		mov			edx, esi;
		pxor		xmm6, xmm6;			// 存放源的一个像素加权结果
		pxor		xmm5, xmm5;			// 存放中间列的整型和
seg_first_row:
		// 取得第一行的像素
		movss		xmm1, [edx];		// 源像素 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		cvtdq2ps	xmm1, xmm1;
		mulps		xmm1, xmm2;
		add			edx, src_pitch;		// 下一行的同一列像素
		addps		xmm6, xmm1;			// 加权求和
		// 判断占用的像素情况
		sub			eax, 0x02;
		jb			map_end;			// 只占一个像素
		jnz			loop_mid_row;		// 占多于两个的像素
		jmp			seg_last_row;		// 只占两个像素
loop_mid_row:
		// 取得中间行的像素
		movd		xmm1, [edx];		// 源像素 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		add			edx, src_pitch;		// 下一行的同一列像素
		paddd		xmm5, xmm1;
		sub			eax, 0x01;
		jnz			loop_mid_row;
seg_last_row:
		// 取得最后一行的像素
		movss		xmm1, [edx];		// 源像素 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		cvtdq2ps	xmm1, xmm1;
		mulps		xmm1, xmm3;
		cvtdq2ps	xmm5, xmm5;
		addps		xmm6, xmm5;			// 加权求和
		addps		xmm6, xmm1;			// 加权求和
map_end:
		// 加权结束
		mulps		xmm6, xmm7;			// 平均
		movaps		[edi], xmm6;
		add			esi, 0x04;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_pixel;
	}
}

// （缩放倍数小于255的高效算法）
void bgra32_mean_copy_add_y_quick(OUT void* des,		// 临时缓存行（是源行拷贝宽度的四倍）
								  IN void* src,			// 源拷贝起始行
								  IN int scw,			// 源拷贝宽度
								  IN int src_pitch,		// 源行字节数
								  IN void* pPowerY,		// 目标某行映射的垂直方向上的权重表
								  IN void* pCoefH,		// 垂直方向上的缩放系数
								  IN int nMapY)			// 目标一行映射到源的行数
{
	_asm{
		mov			edi, des;
		mov			esi, src;
		mov			ecx, scw;
		mov			edx, pPowerY;
		movss		xmm2, [edx];		// 垂直方向上的首权重 0 0 0 powStart
		movss		xmm3, [edx + 0x04];	// 垂直方向上的尾权重 0 0 0 powEnd
		shufps		xmm2, xmm2, 0x00;	// power power power power
		shufps		xmm3, xmm3, 0x00;	// power power power power
		mov			eax, pCoefH;
		pxor		xmm0, xmm0;			// 0 0 0 0
		movaps		xmm7, [eax];		// 缩放系数 coef coef coef coef
loop_pixel:
		mov			eax, nMapY;			// 目标行映射到源的行数
		mov			edx, esi;
		pxor		xmm6, xmm6;			// 存放源的一个像素加权结果
		pxor		xmm5, xmm5;			// 存放中间列的整型和
seg_first_row:
		// 取得第一行的像素
		movss		xmm1, [edx];		// 源像素 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		cvtdq2ps	xmm1, xmm1;
		mulps		xmm1, xmm2;
		add			edx, src_pitch;		// 下一行的同一列像素
		addps		xmm6, xmm1;			// 加权求和
		// 判断占用的像素情况
		sub			eax, 0x02;
		jb			map_end;			// 只占一个像素
		jnz			loop_mid_row;		// 占多于两个的像素
		jmp			seg_last_row;		// 只占两个像素
loop_mid_row:
		// 取得中间行的像素
		movd		xmm1, [edx];		// 源像素 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		add			edx, src_pitch;		// 下一行的同一列像素
		paddusw		xmm5, xmm1;
		sub			eax, 0x01;
		jnz			loop_mid_row;
seg_last_row:
		// 取得最后一行的像素
		movss		xmm1, [edx];		// 源像素 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		cvtdq2ps	xmm1, xmm1;
		punpcklwd	xmm5, xmm0;			// 000a 000r 000g 000b
		mulps		xmm1, xmm3;
		cvtdq2ps	xmm5, xmm5;
		addps		xmm6, xmm5;			// 加权求和
		addps		xmm6, xmm1;			// 加权求和
map_end:
		// 加权结束
		mulps		xmm6, xmm7;			// 平均
		movaps		[edi], xmm6;
		add			esi, 0x04;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_pixel;
	}
}

void bgra32_mean_copy_add_x(OUT void* des,
							IN int dcw,
							IN void* src,
							IN void* pPowerX,
							IN void* pMapX,
							IN void* pCoefW)
{
	_asm{
		mov			eax, pCoefW;
		mov			ecx, dcw;
		movaps		xmm7, [eax];		// 缩放系数 coef coef coef coef
		mov			edi, des;
		mov			esi, src;
		mov			edx, pMapX;
		mov			eax, pPowerX;
loop_pixel:
		push		ecx;				// 保存一行剩余宽度
		pxor		xmm6, xmm6;			// 存放结果
		mov			ecx, [edx];			// 权重个数
		add			edx, 0x04;
		movss		xmm2, [eax];		// 水平方向上的列首权重 0 0 0 powStart
		movss		xmm3, [eax + 0x04];	// 水平方向上的列尾权重 0 0 0 powEnd
		add			eax, 0x08;
		shufps		xmm2, xmm2, 0x00;	// power power power power
		shufps		xmm3, xmm3, 0x00;	// power power power power
seg_first_col:
		// 取得第一列的像素
		movaps		xmm1, [esi];		// 源的一个像素
		mulps		xmm1, xmm2;
		add			esi, 0x10;
		addps		xmm6, xmm1;			// 加权求和
		// 判断占用的像素情况
		sub			ecx, 0x02;
		jb			map_end;			// 只占一个像素
		jnz			loop_mid_col;		// 占多于两个的像素
		jmp			seg_last_col;		// 只占两个像素
loop_mid_col:
		// 取得中间列的像素
		movaps		xmm1, [esi];		// 源的一个像素
		add			esi, 0x10;
		addps		xmm6, xmm1;			// 加权求和
		sub			ecx, 0x01;
		jnz			loop_mid_col;
seg_last_col:
		// 取得最后一列的像素
		movaps		xmm1, [esi];		// 源的一个像素
		mulps		xmm1, xmm3;
		add			esi, 0x10;
		addps		xmm6, xmm1;			// 加权求和
map_end:
		// 加权结束
		mulps		xmm6, xmm7;			// 平均 000a 000r 000g 000b
		pop			ecx;				// 取回一行剩余宽度
		cvtps2dq	xmm6, xmm6;
		packssdw	xmm6, xmm6;			// 压缩结果 0a0r 0g0b 0a0r 0g0b
		packuswb	xmm6, xmm6;			// argb argb argb argb
		movd		[edi], xmm6;
		sub			esi, 0x10;
		add			edi, 0x04;
		sub			ecx, 0x01;
		jnz			loop_pixel;
	}
}

//////////////////////////////////////////////////////////////////////////

// 颜色变换并拆场（不做边界判断）

void color32_to_yuyva_frame(OUT BYTE* pDesYUYV,         // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							IN int nYUYVPitch,
							OUT BYTE* pDesAlpha,        // Alpha 输出（存储方式 A0 A1 A2 A3 A4 ……）
							IN int nAlphaPitch,
							IN BYTE* pSrc32,            // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
							IN int nSrcPitch,           // 源一行的字节宽度
							IN int nWidth,		        // 源图大小
							IN int nHeight,
							IN BOOL bHD,                // 高标清标志
							IN BOOL bSrcRevert,         // 图像是否行翻转标志
							IN BOOL bBGRA)              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	int na16 = (nWidth >> 2), nm16 = ((nWidth & 0x03) << 2);
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc32 + (nHeight - 1) * nSrcPitch) : pSrc32;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;

	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyva_inner(pDesYUYV, pDesAlpha, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYV += nYUYVPitch;
				pDesAlpha += nAlphaPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyva_inner(pDesYUYV, pDesAlpha, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYV += nYUYVPitch;
				pDesAlpha += nAlphaPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
	}
	else    // 标清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyva_inner(pDesYUYV, pDesAlpha, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYV += nYUYVPitch;
				pDesAlpha += nAlphaPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyva_inner(pDesYUYV, pDesAlpha, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYV += nYUYVPitch;
				pDesAlpha += nAlphaPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
	}
}
void color32_to_yuyva_split(OUT BYTE* pDesYUYVF,        // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							IN int nYUYVPitchF,
							OUT BYTE* pDesAlphaF,       // Alpha 第一场输出（存储方式 A0 A1 A2 A3 A4 ……）
							IN int nAlphaPitchF,
							OUT BYTE* pDesYUYVS,        // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							IN int nYUYVPitchS,
							OUT BYTE* pDesAlphaS,       // Alpha 第二场输出（存储方式 A0 A1 A2 A3 A4 ……）
							IN int nAlphaPitchS,
							IN BYTE* pSrc32,            // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
							IN int nSrcPitch,           // 源一行的字节宽度
							IN int nWidth,              // 源图大小
							IN int nHeight,
							IN BOOL bHD,                // 高标清标志
							IN BOOL bSrcRevert,         // 图像是否行翻转标志
							IN BOOL bBGRA)              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	int na16 = (nWidth >> 2), nm16 = ((nWidth & 0x03) << 2);
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc32 + (nHeight - 1) * nSrcPitch) : pSrc32;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;
	int nHalfHeight = (nHeight >> 1);

	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYVF += nYUYVPitchF;
				pDesAlphaF += nAlphaPitchF;
				pSrcLine += nRealSrcPitch;
				// 第二场
				color32_to_yuyva_inner(pDesYUYVS, pDesAlphaS, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYVS += nYUYVPitchS;
				pDesAlphaS += nAlphaPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				// 第二场
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // 拷贝用
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
				nAlphaPitchS = min(nAlphaPitchF, nAlphaPitchS);      // 拷贝用
				na16 = (nAlphaPitchS >> 4), nm16 = (nAlphaPitchS & 0x0f);
				memcpy_line(pDesAlphaS, pDesAlphaF, na16, nm16);
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYVF += nYUYVPitchF;
				pDesAlphaF += nAlphaPitchF;
				pSrcLine += nRealSrcPitch;
				// 第二场
				color32_to_yuyva_inner(pDesYUYVS, pDesAlphaS, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYVS += nYUYVPitchS;
				pDesAlphaS += nAlphaPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				// 第二场
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // 拷贝用
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
				nAlphaPitchS = min(nAlphaPitchF, nAlphaPitchS);      // 拷贝用
				na16 = (nAlphaPitchS >> 4), nm16 = (nAlphaPitchS & 0x0f);
				memcpy_line(pDesAlphaS, pDesAlphaF, na16, nm16);
			}
		}
	}
	else    // 标清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYVF += nYUYVPitchF;
				pDesAlphaF += nAlphaPitchF;
				pSrcLine += nRealSrcPitch;
				// 第二场
				color32_to_yuyva_inner(pDesYUYVS, pDesAlphaS, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYVS += nYUYVPitchS;
				pDesAlphaS += nAlphaPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				// 第二场
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // 拷贝用
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
				nAlphaPitchS = min(nAlphaPitchF, nAlphaPitchS);      // 拷贝用
				na16 = (nAlphaPitchS >> 4), nm16 = (nAlphaPitchS & 0x0f);
				memcpy_line(pDesAlphaS, pDesAlphaF, na16, nm16);
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYVF += nYUYVPitchF;
				pDesAlphaF += nAlphaPitchF;
				pSrcLine += nRealSrcPitch;
				// 第二场
				color32_to_yuyva_inner(pDesYUYVS, pDesAlphaS, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYVS += nYUYVPitchS;
				pDesAlphaS += nAlphaPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				// 第二场
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // 拷贝用
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
				nAlphaPitchS = min(nAlphaPitchF, nAlphaPitchS);      // 拷贝用
				na16 = (nAlphaPitchS >> 4), nm16 = (nAlphaPitchS & 0x0f);
				memcpy_line(pDesAlphaS, pDesAlphaF, na16, nm16);
			}
		}
	}
}

void color32_to_yuyv_frame(OUT BYTE* pDesYUYV,         // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nYUYVPitch,
						   IN BYTE* pSrc32,            // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN int nSrcPitch,           // 源一行的字节宽度
						   IN int nWidth,		        // 源图大小
						   IN int nHeight,
						   IN BOOL bHD,                // 高标清标志
						   IN BOOL bSrcRevert,         // 图像是否行翻转标志
						   IN BOOL bBGRA)              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	int na16 = (nWidth >> 2), nm16 = ((nWidth & 0x03) << 2);
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc32 + (nHeight - 1) * nSrcPitch) : pSrc32;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;

	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyv_inner(pDesYUYV, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYV += nYUYVPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyv_inner(pDesYUYV, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYV += nYUYVPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
	}
	else    // 标清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyv_inner(pDesYUYV, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYV += nYUYVPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyv_inner(pDesYUYV, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYV += nYUYVPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
	}
}
void color32_to_yuyv_split(OUT BYTE* pDesYUYVF,        // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nYUYVPitchF,
						   OUT BYTE* pDesYUYVS,        // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nYUYVPitchS,
						   IN BYTE* pSrc32,            // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN int nSrcPitch,           // 源一行的字节宽度
						   IN int nWidth,              // 源图大小
						   IN int nHeight,
						   IN BOOL bHD,                // 高标清标志
						   IN BOOL bSrcRevert,         // 图像是否行翻转标志
						   IN BOOL bBGRA)              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	int na16 = (nWidth >> 2), nm16 = ((nWidth & 0x03) << 2);
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc32 + (nHeight - 1) * nSrcPitch) : pSrc32;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;
	int nHalfHeight = (nHeight >> 1);

	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYVF += nYUYVPitchF;
				pSrcLine += nRealSrcPitch;
				// 第二场
				color32_to_yuyv_inner(pDesYUYVS, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYVS += nYUYVPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				// 第二场
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // 拷贝用
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYVF += nYUYVPitchF;
				pSrcLine += nRealSrcPitch;
				// 第二场
				color32_to_yuyv_inner(pDesYUYVS, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYVS += nYUYVPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				// 第二场
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // 拷贝用
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
			}
		}
	}
	else    // 标清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYVF += nYUYVPitchF;
				pSrcLine += nRealSrcPitch;
				// 第二场
				color32_to_yuyv_inner(pDesYUYVS, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYVS += nYUYVPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				// 第二场
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // 拷贝用
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYVF += nYUYVPitchF;
				pSrcLine += nRealSrcPitch;
				// 第二场
				color32_to_yuyv_inner(pDesYUYVS, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYVS += nYUYVPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				// 第二场
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // 拷贝用
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
			}
		}
	}
}

void yuyv_to_color32_compose(OUT BYTE* pDes32,          // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
							 IN int nDesPitch,
							 IN BYTE* pSrcYUYVF,        // YUYV 第一场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							 IN int nFirstPitch,
							 IN BYTE* pSrcYUYVS,        // YUYV 第二场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							 IN int nSecondPitch,
							 IN int nWidth,             // 目标大小
							 IN int nHeight,
							 IN BOOL bHD,               // 高标清标志
							 IN BOOL bBGRA,             // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
							 IN BOOL bDstRevert)		// 图像是否行翻转标志
{
	int nRealYUYVPitch = ((nWidth + (nWidth & 0x01)) << 1);
	int na16 = ((nRealYUYVPitch - 4) >> 4), nm16 = (nRealYUYVPitch - (na16 << 4));
	int nHalfHeight = (nHeight >> 1);

	BYTE* pDstLine = (bDstRevert) ? (pDes32 + (nHeight - 1) * nDesPitch) : pDes32;
	int nRealDstPitch = (bDstRevert) ? -nDesPitch : nDesPitch;

	if (bHD)        // 高清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYVF += nFirstPitch;
				// 第二场
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVS, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYVS += nSecondPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，只采用第一场
			{
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYVF += nFirstPitch;
				// 第二场
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVS, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYVS += nSecondPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，只采用第一场
			{
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
		}
	}
	else            // 标清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYVF += nFirstPitch;
				// 第二场
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVS, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYVS += nSecondPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，只采用第一场
			{
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYVF += nFirstPitch;
				// 第二场
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVS, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYVS += nSecondPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，只采用第一场
			{
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			}
		}
	}
}

void yuyv_to_color32_frame(OUT BYTE* pDes32,          // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
						   IN int nDesPitch,
						   IN BYTE* pSrcYUYV,        // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nSrcPitch,
						   IN int nWidth,             // 目标大小
						   IN int nHeight,
						   IN BOOL bHD,               // 高标清标志
						   IN BOOL bBGRA,			// 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
						   IN BOOL bDstRevert)		// 图像是否行翻转标志
{
	int nRealYUYVPitch = ((nWidth + (nWidth & 0x01)) << 1);
	int na16 = ((nRealYUYVPitch - 4) >> 4), nm16 = (nRealYUYVPitch - (na16 << 4));
	BYTE* pDstLine = (bDstRevert) ? (pDes32 + (nHeight - 1) * nDesPitch) : pDes32;
	int nRealDstPitch = (bDstRevert) ? -nDesPitch : nDesPitch;

	if (bHD)        // 高清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				yuyv_to_bgra_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				// 第一场
				yuyv_to_rgba_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
	}
	else            // 标清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				// 第一场
				yuyv_to_bgra_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				// 第一场
				yuyv_to_rgba_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
	}
}

void color24_to_yuyv_frame(OUT BYTE* pDesYUYV,         // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nYUYVPitch,
						   IN BYTE* pSrc24,            // *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN int nSrcPitch,           // 源一行的字节宽度
						   IN int nWidth,		       // 源图大小
						   IN int nHeight,
						   IN BOOL bHD,                // 高标清标志
						   IN BOOL bSrcRevert,         // 图像是否行翻转标志
						   IN BOOL bBGR)               // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	int na12 = ((nSrcPitch - 4) / 12), nmp = (nWidth - (na12 << 2));
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc24 + (nHeight - 1) * nSrcPitch) : pSrc24;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;

	if (bHD)       // 高清
	{
		if (bBGR)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color24_to_yuyv_inner(pDesYUYV, pSrcLine, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
				pSrcLine += nRealSrcPitch;
				pDesYUYV += nYUYVPitch;
			}
		}
		else        // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color24_to_yuyv_inner(pDesYUYV, pSrcLine, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
				pSrcLine += nRealSrcPitch;
				pDesYUYV += nYUYVPitch;
			}
		}
	}
	else        // 标清
	{
		if (bBGR)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color24_to_yuyv_inner(pDesYUYV, pSrcLine, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
				pSrcLine += nRealSrcPitch;
				pDesYUYV += nYUYVPitch;
			}
		}
		else        // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				color24_to_yuyv_inner(pDesYUYV, pSrcLine, na12, nmp, g_nY24_RGB_SD, g_nY32_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD, g_nUV_BGR_SD);
				pSrcLine += nRealSrcPitch;
				pDesYUYV += nYUYVPitch;
			}
		}
	}
}
void color24_to_yuyv_split(OUT BYTE* pDesFirst,          // 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nFirstPitch,
						   OUT BYTE* pDesSecond,         // 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nSecondPitch,
						   IN BYTE* pSrc24,              // *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN int nSrcPitch,             // 源一行的字节宽度（必须为 4 的倍数）
						   IN int nWidth,			     // 源图大小
						   IN int nHeight,
						   IN BOOL bHD,                  // 高标清标志
						   IN BOOL bSrcRevert,           // 图像是否行翻转标志
						   IN BOOL bBGR)                 // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	int na12 = ((nSrcPitch - 4) / 12), nmp = (nWidth - (na12 << 2));
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc24 + (nHeight - 1) * nSrcPitch) : pSrc24;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;
	int nHalfHeight = (nHeight >> 1);

	if (bHD)       // 高清
	{
		if (bBGR)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
				pSrcLine += nRealSrcPitch;
				pDesFirst += nFirstPitch;
				// 第二场
				color24_to_yuyv_inner(pDesSecond, pSrcLine, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
				pSrcLine += nRealSrcPitch;
				pDesSecond += nSecondPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
				// 第二场
				nSecondPitch = min(nFirstPitch, nSecondPitch);      // 拷贝用
				na12 = (nSecondPitch >> 4), nmp = (nSecondPitch & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesSecond, pDesFirst, na12, nmp);
			}
		}
		else        // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
				pSrcLine += nRealSrcPitch;
				pDesFirst += nFirstPitch;
				// 第二场
				color24_to_yuyv_inner(pDesSecond, pSrcLine, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
				pSrcLine += nRealSrcPitch;
				pDesSecond += nSecondPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
				// 第二场
				nSecondPitch = min(nFirstPitch, nSecondPitch);      // 拷贝用
				na12 = (nSecondPitch >> 4), nmp = (nSecondPitch & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesSecond, pDesFirst, na12, nmp);
			}
		}
	}
	else        // 标清
	{
		if (bBGR)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
				pSrcLine += nRealSrcPitch;
				pDesFirst += nFirstPitch;
				// 第二场
				color24_to_yuyv_inner(pDesSecond, pSrcLine, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
				pSrcLine += nRealSrcPitch;
				pDesSecond += nSecondPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
				// 第二场
				nSecondPitch = min(nFirstPitch, nSecondPitch);      // 拷贝用
				na12 = (nSecondPitch >> 4), nmp = (nSecondPitch & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesSecond, pDesFirst, na12, nmp);
			}
		}
		else        // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_RGB_SD, g_nY32_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD, g_nUV_BGR_SD);
				pSrcLine += nRealSrcPitch;
				pDesFirst += nFirstPitch;
				// 第二场
				color24_to_yuyv_inner(pDesSecond, pSrcLine, na12, nmp, g_nY24_RGB_SD, g_nY32_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD, g_nUV_BGR_SD);
				pSrcLine += nRealSrcPitch;
				pDesSecond += nSecondPitch;
			}
			if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
			{
				// 第一场
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_RGB_SD, g_nY32_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD, g_nUV_BGR_SD);
				// 第二场
				nSecondPitch = min(nFirstPitch, nSecondPitch);      // 拷贝用
				na12 = (nSecondPitch >> 4), nmp = (nSecondPitch & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesSecond, pDesFirst, na12, nmp);
			}
		}
	}
}

void yuyv_to_color24_frame(OUT BYTE* pDes24,          // *G@A 缓存，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN int nDesPitch,
						   IN BYTE* pSrcYUYV,        // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nSrcPitch,
						   IN int nWidth,             // 目标大小
						   IN int nHeight,
						   IN BOOL bHD,               // 高标清标志
						   IN BOOL bBGR,				// 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
						   IN BOOL bDstRevert)			// 图像是否行翻转标志
{
	int nRealYUYVPitch = ((nWidth + (nWidth & 0x01)) << 1);
	int na16 = ((nRealYUYVPitch - 4) >> 4), nm16 = (nRealYUYVPitch - (na16 << 4));
	BYTE* pDstLine = (bDstRevert) ? (pDes24 + (nHeight - 1) * nDesPitch) : pDes24;
	int nRealDstPitch = (bDstRevert) ? -nDesPitch : nDesPitch;

	if (bHD)        // 高清
	{
		if (bBGR)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				yuyv_to_color24_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				// 第一场
				yuyv_to_color24_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
	}
	else            // 标清
	{
		if (bBGR)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				// 第一场
				yuyv_to_color24_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				// 第一场
				yuyv_to_color24_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
	}
}

void yuv420_to_color32_frame(OUT BYTE* pDes32,           // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
							 IN int nDesPitch,
							 IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
							 IN int nSrcYPitch,
							 IN BYTE* pSrcU,             // U 输入（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
							 IN int nSrcUPitch,
							 IN BYTE* pSrcV,             // V 输入（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
							 IN int nSrcVPitch,
							 IN int nWidth,              // 目标大小
							 IN int nHeight,
							 IN BOOL bHD,                // 高标清标志
							 IN BOOL bDesRevert,         // 图像是否行翻转标志
							 IN BOOL bBGRA)              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	g_NXColorSpace->pStuYUV420toOthers->pfnYUV420_2_RGB32_Frame(pDes32, nDesPitch, \
		pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD ? NXVIDEO_DEFINITION_HIGH : NXVIDEO_DEFINITION_STANDARD, \
		bBGRA != FALSE, 0xff, bDesRevert != FALSE);
	return;

	int nHalfHeight = (nHeight >> 1), nHalfHalf = ((nHalfHeight + 0) >> 1);
	int nUVPitch = min(nSrcUPitch, nSrcVPitch);
	int na16 = (nWidth >> 5), nm16 = ((nWidth >> 1) & 0x0f);	// 一行像素宽度的一半（因为一行内的 UV 与 Y 为 422 的关系）
	int na8 = ((nWidth - 1) >> 3), nm8 = (nWidth - (na8 << 3));
	int nYP2 = (nSrcYPitch << 1), nUP2 = (nSrcUPitch << 1), nVP2 = (nSrcVPitch << 1);
	// 采用缓存，先插值计算出 422 的 UV 信号（两行），再使用 422 到 color32 的接口
	BYTE* pTmpU = TPSSE_NEW((nUVPitch << 1), BYTE);
	BYTE* pTmpV = TPSSE_NEW((nUVPitch << 1), BYTE);
	BYTE *pU1 = pTmpU, *pU2 = pTmpU + nUVPitch, *pV1 = pTmpV, *pV2 = pTmpV + nUVPitch;
	// 翻转
	BYTE* pDesF = (bDesRevert) ? (pDes32 + (nHeight - 1) * nDesPitch) : pDes32;
	int nRealDesPitch = (bDesRevert) ? -nDesPitch : nDesPitch;
	BYTE* pDesS = pDesF + nRealDesPitch;
	int nRealDP2 = (bDesRevert) ? -(nDesPitch << 1) : (nDesPitch << 1);
	BYTE *pYF = pSrcY, *pYS = pSrcY + nSrcYPitch;

	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			// 前两行
			// 第一场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesF += nRealDP2; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesF += nRealDP2; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 第二场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesS += nRealDP2; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesS += nRealDP2; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 中间行
			for (int i=2; i<nHalfHalf; i++)
			{
				// 第一场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealDP2; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealDP2; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// 最后两行或一行（场为奇数行时）
			if (nHalfHalf & 0x01)       // 一行
			{
				// 第一场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			}
			else    // 两行
			{
				// 第一场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			}
		}
		else        // R 在低位
		{
			// 前两行
			// 第一场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesF += nRealDP2; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesF += nRealDP2; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 第二场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesS += nRealDP2; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesS += nRealDP2; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 中间行
			for (int i=2; i<nHalfHalf; i++)
			{
				// 第一场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealDP2; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealDP2; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// 最后两行或一行（场为奇数行时）
			if (nHalfHalf & 0x01)       // 一行
			{
				// 第一场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
			else    // 两行
			{
				// 第一场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
		}
	}
	else        // 标清
	{
		if (bBGRA)      // B 在低位
		{
			// 前两行
			// 第一场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesF += nRealDP2; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesF += nRealDP2; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 第二场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesS += nRealDP2; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesS += nRealDP2; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 中间行
			for (int i=2; i<nHalfHalf; i++)
			{
				// 第一场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealDP2; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealDP2; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// 最后两行或一行（场为奇数行时）
			if (nHalfHalf & 0x01)       // 一行
			{
				// 第一场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
					yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				}
			}
			else    // 两行
			{
				// 第一场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealDP2; pYS += nYP2;
				if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				}
			}
		}
		else        // R 在低位
		{
			// 前两行
			// 第一场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesF += nRealDP2; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesF += nRealDP2; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 第二场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesS += nRealDP2; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesS += nRealDP2; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 中间行
			for (int i=2; i<nHalfHalf; i++)
			{
				// 第一场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealDP2; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealDP2; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// 最后两行或一行（场为奇数行时）
			if (nHalfHalf & 0x01)       // 一行
			{
				// 第一场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
					yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				}
			}
			else    // 两行
			{
				// 第一场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealDP2; pYS += nYP2;
				if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				}
			}
		}
	}
	// 释放缓存
	TPSSE_FREE(pTmpU);
	TPSSE_FREE(pTmpV);
}
void yuv420_to_color32_split(OUT BYTE* pDesFirst,        // 第一场 *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
							 IN int nFirstPitch,
							 OUT BYTE* pDesSecond,       // 第二场 *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
							 IN int nSecondPitch,
							 IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
							 IN int nSrcYPitch,
							 IN BYTE* pSrcU,             // U 输入（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
							 IN int nSrcUPitch,
							 IN BYTE* pSrcV,             // V 输入（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
							 IN int nSrcVPitch,
							 IN int nWidth,              // 目标大小
							 IN int nHeight,
							 IN BOOL bHD,                // 高标清标志
							 IN BOOL bDesRevert,         // 图像是否行翻转标志
							 IN BOOL bBGRA)              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	int nHalfHeight = (nHeight >> 1), nHalfHalf = ((nHalfHeight + 1) >> 1);
	int nUVPitch = min(nSrcUPitch, nSrcVPitch);
	int na16 = (nWidth >> 4), nm16 = (nWidth & 0x0f);
	int na8 = ((nWidth - 1) >> 3), nm8 = (nWidth - (na8 << 3));
	int nYP2 = (nSrcYPitch << 1), nUP2 = (nSrcUPitch << 1), nVP2 = (nSrcVPitch << 1);
	// 采用缓存，先插值计算出 422 的 UV 信号（两行），再使用 422 到 color32 的接口
	BYTE* pTmpU = TPSSE_NEW((nUVPitch << 1), BYTE);
	BYTE* pTmpV = TPSSE_NEW((nUVPitch << 1), BYTE);
	BYTE *pU1 = pTmpU, *pU2 = pTmpU + nUVPitch, *pV1 = pTmpV, *pV2 = pTmpV + nUVPitch;
	// 翻转
	BYTE* pDesF = (bDesRevert) ? (pDesFirst + (nHalfHeight - 1) * nFirstPitch) : pDesFirst;
	int nRealFirstPitch = (bDesRevert) ? -nFirstPitch : nFirstPitch;
	BYTE* pDesS = (bDesRevert) ? (pDesSecond + (nHalfHeight - 1) * nSecondPitch) : pDesSecond;
	int nRealSecondPitch = (bDesRevert) ? -nSecondPitch : nSecondPitch;
	BYTE *pYF = pSrcY, *pYS = pSrcY + nSrcYPitch;

	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			// 前两行
			// 第一场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 第二场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 中间行
			for (int i=2; i<nHalfHalf; i++)
			{
				// 第一场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// 最后两行或一行（场为奇数行时）
			if (nHalfHalf & 0x01)       // 一行
			{
				// 第一场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			}
			else    // 两行
			{
				// 第一场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			}
		}
		else        // R 在低位
		{
			// 前两行
			// 第一场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 第二场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 中间行
			for (int i=2; i<nHalfHalf; i++)
			{
				// 第一场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// 最后两行或一行（场为奇数行时）
			if (nHalfHalf & 0x01)       // 一行
			{
				// 第一场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
			else    // 两行
			{
				// 第一场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
		}
	}
	else        // 标清
	{
		if (bBGRA)      // B 在低位
		{
			// 前两行
			// 第一场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 第二场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 中间行
			for (int i=2; i<nHalfHalf; i++)
			{
				// 第一场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// 最后两行或一行（场为奇数行时）
			if (nHalfHalf & 0x01)       // 一行
			{
				// 第一场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
					yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				}
			}
			else    // 两行
			{
				// 第一场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				}
			}
		}
		else        // R 在低位
		{
			// 前两行
			// 第一场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 第二场
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// 中间行
			for (int i=2; i<nHalfHalf; i++)
			{
				// 第一场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// 最后两行或一行（场为奇数行时）
			if (nHalfHalf & 0x01)       // 一行
			{
				// 第一场
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
					yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				}
			}
			else    // 两行
			{
				// 第一场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// 第二场
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				}
			}
		}
	}
	// 释放缓存
	TPSSE_FREE(pTmpU);
	TPSSE_FREE(pTmpV);
}

void yuv422_to_color32_frame(OUT BYTE* pDes32,           // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
							 IN int nDesPitch,
							 IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
							 IN int nSrcYPitch,
							 IN BYTE* pSrcU,             // U 输入（存储方式 U0 U2 U4 ……）
							 IN int nSrcUPitch,
							 IN BYTE* pSrcV,             // V 输入（存储方式 V0 V2 V4 ……）
							 IN int nSrcVPitch,
							 IN int nWidth,              // 目标大小
							 IN int nHeight,
							 IN BOOL bHD,                // 高标清标志
							 IN BOOL bDesRevert,         // 图像是否行翻转标志
							 IN BOOL bBGRA)              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	int na8 = ((nWidth - 1) >> 3), nm8 = (nWidth - (na8 << 3));
	BYTE* pDesLine = (bDesRevert) ? (pDes32 + (nHeight - 1) * nDesPitch) : pDes32;
	int nRealDesPitch = (bDesRevert) ? -nDesPitch : nDesPitch;

	if (bHD)        // 高清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				yuv422_to_color32_inner(pDesLine, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesLine += nRealDesPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				yuv422_to_color32_inner(pDesLine, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesLine += nRealDesPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
	}
	else            // 标清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				yuv422_to_color32_inner(pDesLine, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesLine += nRealDesPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHeight; i++)
			{
				yuv422_to_color32_inner(pDesLine, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesLine += nRealDesPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
	}
}
void yuv422_to_color32_split(OUT BYTE* pDesFirst,        // 第一场 *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
							 IN int nFirstPitch,
							 OUT BYTE* pDesSecond,       // 第二场 *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
							 IN int nSecondPitch,
							 IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
							 IN int nSrcYPitch,
							 IN BYTE* pSrcU,             // U 输入（存储方式 U0 U2 U4 ……）
							 IN int nSrcUPitch,
							 IN BYTE* pSrcV,             // V 输入（存储方式 V0 V2 V4 ……）
							 IN int nSrcVPitch,
							 IN int nWidth,              // 目标大小
							 IN int nHeight,
							 IN BOOL bHD,                // 高标清标志
							 IN BOOL bDesRevert,         // 图像是否行翻转标志
							 IN BOOL bBGRA)              // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	int na8 = ((nWidth - 1) >> 3), nm8 = (nWidth - (na8 << 3));
	int nHalfHeight = (nHeight >> 1);
	BYTE* pDesLineF = (bDesRevert) ? (pDesFirst + (nHalfHeight - 1) * nFirstPitch) : pDesFirst;
	BYTE* pDesLineS = (bDesRevert) ? (pDesSecond + (nHalfHeight - 1) * nSecondPitch) : pDesSecond;
	int nRealFirstPitch = (bDesRevert) ? -nFirstPitch : nFirstPitch;
	int nRealSecondPitch = (bDesRevert) ? -nSecondPitch : nSecondPitch;

	if (bHD)        // 高清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				yuv422_to_color32_inner(pDesLineF, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesLineF += nRealFirstPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
				// 第二场
				yuv422_to_color32_inner(pDesLineS, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesLineS += nRealSecondPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				yuv422_to_color32_inner(pDesLineF, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesLineF += nRealFirstPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
				// 第二场
				yuv422_to_color32_inner(pDesLineS, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesLineS += nRealSecondPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
	}
	else            // 标清
	{
		if (bBGRA)      // B 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				yuv422_to_color32_inner(pDesLineF, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesLineF += nRealFirstPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
				// 第二场
				yuv422_to_color32_inner(pDesLineS, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesLineS += nRealSecondPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
		else    // R 在低位
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// 第一场
				yuv422_to_color32_inner(pDesLineF, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesLineF += nRealFirstPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
				// 第二场
				yuv422_to_color32_inner(pDesLineS, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesLineS += nRealSecondPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
	}
}

// 重组数据

// 需要按场处理，在场内隔行插值，每次输出两行
// 前两行
void yuv420_to_yuyv_line_2_F(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN int yuyv_p,
							 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
							 IN int y_p_2,
							 IN BYTE* u,            // u 输入，存放方式 U0 U2 U4 U6 ……
							 IN int u_p_2,
							 IN BYTE* v,            // v 输入，存放方式 V0 V2 V4 V6 ……
							 IN int v_p_2,
							 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
							 IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         edx, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		add         edx, yuyv_p;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		movdqu      xmm1, [eax];        // Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movsd       xmm2, [ebx];        // 0000 0000  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movsd       xmm3, [esi];        // 0000 0000  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		movdqa      xmm4, xmm2;
		movdqa      xmm0, xmm1;
		punpcklbw   xmm2, xmm3;         // VV14 UU14 ……
		add			 eax, y_p_2;
		add			 ebx, u_p_2;
		add			esi, v_p_2;
		movdqu      xmm5, [eax];
		movsd	     xmm6, [ebx];
		movsd       xmm7, [esi];
		sub			 eax, y_p_2;
		sub			 ebx, u_p_2;
		sub			esi, v_p_2;
		// 结果为 UU_0
		punpcklbw   xmm0, xmm2;
		punpckhbw   xmm1, xmm2;
		pxor        xmm2, xmm2;         // 0
		punpcklbw   xmm4, xmm2;         // UU_0
		punpcklbw   xmm3, xmm2;         // VV_0
		punpcklbw   xmm6, xmm2;         // UU_2
		punpcklbw   xmm7, xmm2;         // VV_2
		// 结果为 UU_0 * 0.75 + UU_2 * 0.25
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm3;
		paddw       xmm4, xmm4;
		paddw       xmm3, xmm3;
		paddw       xmm6, gc_nRoundComp_2;
		paddw       xmm7, gc_nRoundComp_2;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm3;
		psrlw       xmm6, 0x02;
		psllw       xmm7, 0x06;
		movdqa      xmm4, xmm5;
		pand        xmm7, gc_dwMaskWHB;
		por         xmm6, xmm7;
		punpcklbw   xmm4, xmm6;
		punpckhbw   xmm5, xmm6;
		movdqu      [edi], xmm0;
		movdqu      [edx], xmm4;
		add         edi, 0x10;
		add         edx, 0x10;
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm5;
		add         eax, 0x10;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x10;
		add         edx, 0x10;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		movsd       xmm1, [eax];        // 0000 0000  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm2, [ebx];        // 0000 0000 0000 UU6 UU4 UU2 UU0
		movss       xmm3, [esi];        // 0000 0000 0000 VV6 VV4 VV2 VV0
		movdqa      xmm4, xmm2;
		pxor        xmm0, xmm0;         // 0
		punpcklbw    xmm2, xmm3;         // 0000 0000  VV6 UU6 ……
		add			 eax, y_p_2;
		add			 ebx, u_p_2;
		add			 esi, v_p_2;
		movsd       xmm5, [eax];
		movss	    xmm6, [ebx];
		movss       xmm7, [esi];
		sub			 eax, y_p_2;
		sub			 ebx, u_p_2;
		sub			 esi, v_p_2;
		// 结果为 UU_0
		punpcklbw   xmm1, xmm2;
		punpcklbw   xmm4, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// 结果为 UU_0 * 0.75 + UU_2 * 0.25
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm3;
		paddw       xmm4, xmm4;
		paddw       xmm3, xmm3;
		paddw       xmm6, gc_nRoundComp_2;
		paddw       xmm7, gc_nRoundComp_2;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm3;
		psrlw       xmm6, 0x02;
		psllw       xmm7, 0x06;
		pand        xmm7, gc_dwMaskWHB;
		por         xmm6, xmm7;
		punpcklbw   xmm5, xmm6;
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm5;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
		add         edx, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		pxor        xmm0, xmm0;         // 0
		pxor		xmm2, xmm2;
		pxor		xmm3, xmm3;
		movss       xmm1, [eax];        // 0000 0000 0000  Y3 Y2 Y1 Y0
		pinsrw      xmm2, [ebx], 0x00;  // UU2 UU0
		pinsrw      xmm3, [esi], 0x00;  // VV2 VV0
		movdqa      xmm4, xmm2;
		punpcklbw    xmm2, xmm3;         // 0000 0000 0000 VV2 UU2 VV0 UU0
		add			 eax, y_p_2;
		add			 ebx, u_p_2;
		add			 esi, v_p_2;
		movss       xmm5, [eax];
		pinsrw      xmm6, [ebx], 0x00;
		pinsrw      xmm7, [esi], 0x00;
		sub			 eax, y_p_2;
		sub			 ebx, u_p_2;
		sub			 esi, v_p_2;
		// 结果为 UU_0
		punpcklbw   xmm1, xmm2;
		punpcklbw   xmm4, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// 结果为 UU_0 * 0.75 + UU_2 * 0.25
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm3;
		paddw       xmm4, xmm4;
		paddw       xmm3, xmm3;
		paddw       xmm6, gc_nRoundComp_2;
		paddw       xmm7, gc_nRoundComp_2;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm3;
		psrlw       xmm6, 0x02;
		psllw       xmm7, 0x06;
		pand        xmm7, gc_dwMaskWHB;
		por         xmm6, xmm7;
		punpcklbw   xmm5, xmm6;
		movsd      [edi], xmm1;
		movsd      [edx], xmm5;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
		add         edx, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		push		edx;
		push		eax;
		xor			ecx, ecx;
		xor			edx, edx;
		xor			eax, eax;
		mov			ch, [ebx];		// UU_C
		mov			dl, [esi];		// VV_C
		add			ebx, u_p_2;
		add			esi, v_p_2;
		mov			al, [ebx];		// UU_N
		xor			ebx, ebx;
		mov			bl, [esi];		// VV_N
		xor			esi, esi;
		mov			esi, edx;
		shl			esi, 0x18;
		or			esi, ecx;		// VV_C 0 UU_C 0
		shr			ecx, 0x08;
		add			ax, cx;
		add			bx, dx;
		add			cx, cx;
		add			dx, dx;
		add			ax, 0x02;
		add			bx, 0x02;
		add			ax, cx;
		add			bx, dx;
		and			eax, 0xfffc;
		and			ebx, 0xfffc;
		shl			eax, 0x06;
		shl			ebx, 0x16;
		or			ebx, eax;		// VV_N 0 UU_N 0
		pop			eax;
		mov			cx, [eax];
		add			eax, y_p_2;
		mov			edx, ecx;
		and			ecx, 0x00ff;
		and			edx, 0xff00;
		or			ecx, esi;
		shl			edx, 0x08;
		or			ecx, edx;
		mov			[edi], ecx;
		pop			edx;
		mov			cx, [eax];
		mov			edi, ecx;
		and			ecx, 0x00ff;
		and			edi, 0xff00;
		or			ecx, ebx;
		shl			edi, 0x08;
		or			ecx, edi;
		mov			[edx], ecx;
loop_end:
	}
}
// 中间行
void yuv420_to_yuyv_line_2(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN int yuyv_p,
						   IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
						   IN int y_p_2,
						   IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
						   IN int u_p_2,
						   IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
						   IN int v_p_2,
						   IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
						   IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         edx, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		add         edx, yuyv_p;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		movsd       xmm4, [ebx];        // 0000 0000  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movsd       xmm5, [esi];        // 0000 0000  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		add         ebx, u_p_2;
		add         esi, v_p_2;
		movsd       xmm6, [ebx];    // UU_N
		movsd       xmm7, [esi];    // VV_N
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		movsd       xmm2, [ebx];        // UU_P
		movsd       xmm3, [esi];        // VV_P
		add         ebx, u_p_2;
		add         esi, v_p_2;
		pxor        xmm0, xmm0;
		punpcklbw   xmm2, xmm0;     // UU_P
		punpcklbw   xmm3, xmm0;     // VV_P
		punpcklbw   xmm4, xmm0;     // UU_C
		punpcklbw   xmm5, xmm0;     // VV_C
		punpcklbw   xmm6, xmm0;     // UU_N
		punpcklbw   xmm7, xmm0;     // VV_N
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75、第二个结果：UU_N * 0.25 + UU_C * 0.75
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm5;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm5;
		paddw       xmm4, xmm4;
		paddw       xmm5, xmm5;
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm5;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm5;
		paddw       xmm2, gc_nRoundComp_2;
		paddw       xmm3, gc_nRoundComp_2;
		paddw       xmm6, gc_nRoundComp_2;
		paddw       xmm7, gc_nRoundComp_2;
		psrlw		 xmm2, 0x02;
		psrlw		 xmm6, 0x02;
		psllw       xmm3, 0x06;
		psllw       xmm7, 0x06;
		pand        xmm3, gc_dwMaskWHB;
		pand        xmm7, gc_dwMaskWHB;
		por         xmm2, xmm3;
		por         xmm6, xmm7;
		movdqu      xmm0, [eax];        // Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		add			eax, y_p_2;
		movdqu      xmm4, [eax];
		sub			eax, y_p_2;
		movdqa      xmm1, xmm0;
		movdqa      xmm5, xmm4;
		punpcklbw   xmm0, xmm2;
		punpcklbw   xmm4, xmm6;
		punpckhbw   xmm1, xmm2;
		punpckhbw   xmm5, xmm6;
		movdqu      [edi], xmm0;
		movdqu      [edx], xmm4;
		add         edi, 0x10;
		add         edx, 0x10;
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm5;
		add         eax, 0x10;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x10;
		add         edx, 0x10;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		movss       xmm4, [ebx];        // 0000 0000 0000  UU6 UU4 UU2 UU0
		movss       xmm5, [esi];        // 0000 0000 0000  VV6 VV4 VV2 VV0
		add         ebx, u_p_2;
		add         esi, v_p_2;
		movss       xmm6, [ebx];    // UU_N
		movss       xmm7, [esi];    // VV_N
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		movss       xmm2, [ebx];        // UU_P
		movss       xmm3, [esi];        // VV_P
		add         ebx, u_p_2;
		add         esi, v_p_2;
		pxor        xmm0, xmm0;
		punpcklbw   xmm2, xmm0;     // UU_P
		punpcklbw   xmm3, xmm0;     // VV_P
		punpcklbw   xmm4, xmm0;     // UU_C
		punpcklbw   xmm5, xmm0;     // VV_C
		punpcklbw   xmm6, xmm0;     // UU_N
		punpcklbw   xmm7, xmm0;     // VV_N
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75、第二个结果：UU_N * 0.25 + UU_C * 0.75
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm5;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm5;
		paddw       xmm4, xmm4;
		paddw       xmm5, xmm5;
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm5;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm5;
		paddw       xmm2, gc_nRoundComp_2;
		paddw       xmm3, gc_nRoundComp_2;
		paddw       xmm6, gc_nRoundComp_2;
		paddw       xmm7, gc_nRoundComp_2;
		psrlw		 xmm2, 0x02;
		psrlw		 xmm6, 0x02;
		psllw       xmm3, 0x06;
		psllw       xmm7, 0x06;
		pand        xmm3, gc_dwMaskWHB;
		pand        xmm7, gc_dwMaskWHB;
		por         xmm2, xmm3;
		por         xmm6, xmm7;
		movsd      xmm0, [eax];        // 0000 0000  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		add			eax, y_p_2;
		movsd      xmm4, [eax];
		sub			eax, y_p_2;
		punpcklbw   xmm0, xmm2;
		punpcklbw   xmm4, xmm6;
		movdqu      [edi], xmm0;
		movdqu      [edx], xmm4;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
		add         edx, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		pxor        xmm0, xmm0;
		pxor		xmm2, xmm2;
		pxor		xmm3, xmm3;
		pxor		xmm4, xmm4;
		pxor		xmm5, xmm5;
		pxor		xmm6, xmm6;
		pxor		xmm7, xmm7;
		pinsrw      xmm4, [ebx], 0x00;        // 0000 0000 0000  0 0 UU2 UU0
		pinsrw      xmm5, [esi], 0x00;        // 0000 0000 0000  0 0 VV2 VV0
		add         ebx, u_p_2;
		add         esi, v_p_2;
		pinsrw      xmm6, [ebx], 0x00;    // UU_N
		pinsrw      xmm7, [esi], 0x00;    // VV_N
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		pinsrw      xmm2, [ebx], 0x00;        // UU_P
		pinsrw      xmm3, [esi], 0x00;        // VV_P
		add         ebx, u_p_2;
		add         esi, v_p_2;
		punpcklbw   xmm2, xmm0;     // UU_P
		punpcklbw   xmm3, xmm0;     // VV_P
		punpcklbw   xmm4, xmm0;     // UU_C
		punpcklbw   xmm5, xmm0;     // VV_C
		punpcklbw   xmm6, xmm0;     // UU_N
		punpcklbw   xmm7, xmm0;     // VV_N
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75、第二个结果：UU_N * 0.25 + UU_C * 0.75
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm5;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm5;
		paddw       xmm4, xmm4;
		paddw       xmm5, xmm5;
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm5;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm5;
		paddw       xmm2, gc_nRoundComp_2;
		paddw       xmm3, gc_nRoundComp_2;
		paddw       xmm6, gc_nRoundComp_2;
		paddw       xmm7, gc_nRoundComp_2;
		psrlw		 xmm2, 0x02;
		psrlw		 xmm6, 0x02;
		psllw       xmm3, 0x06;
		psllw       xmm7, 0x06;
		pand        xmm3, gc_dwMaskWHB;
		pand        xmm7, gc_dwMaskWHB;
		por         xmm2, xmm3;
		por         xmm6, xmm7;
		movss      xmm0, [eax];        // 0000 0000 0000  Y3 Y2 Y1 Y0
		add			eax, y_p_2;
		movss      xmm4, [eax];
		sub			eax, y_p_2;
		punpcklbw   xmm0, xmm2;
		punpcklbw   xmm4, xmm6;
		movsd      [edi], xmm0;
		movsd      [edx], xmm4;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
		add         edx, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		push		edx;
		push		edi;
		push		eax;
		xor			edx, edx;
		xor			eax, eax;
		mov			dl, [ebx];		// UU0
		mov			al, [esi];		// VV0
		mov			edi, edx;
		shl			eax, 0x10;
		or			edi, eax;		// 0 VV_C 0 UU_C
		add			ebx, u_p_2;
		add			esi, v_p_2;
		xor			edx, edx;
		xor			eax, eax;
		mov			dl, [ebx];		// UU0
		mov			al, [esi];		// VV0
		mov			ecx, edx;
		shl			eax, 0x10;
		or			ecx, eax;		// 0 VV_N 0 UU_N
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		xor			edx, edx;
		xor			eax, eax;
		mov			dl, [ebx];		// UU_P
		mov			al, [esi];		// VV_P
		mov			ebx, edi;		// UU_C
		mov			esi, ecx;		// UU_N
		shr			edi, 0x10;		// VV_C
		shr			ecx, 0x10;		// VV_N
		add			dx, bx;
		add			si, bx;
		add			ax, di;
		add			cx, di;
		add			bx, bx;
		add			di, di;
		add			dx, 0x02;
		add			si, 0x02;
		add			ax, 0x02;
		add			cx, 0x02;
		add			dx, bx;		// UU_1
		add			si, bx;		// UU_2
		add			ax, di;		// VV_1
		add			cx, di;		// VV_2
		and			edx, 0xfffc;
		and			esi, 0xfffc;
		and			eax, 0xfffc;
		and			ecx, 0xfffc;
		shl			edx, 0x06;
		shl			esi, 0x06;
		shl			eax, 0x16;
		shl			ecx, 0x16;
		or			edx, eax;	// VV_1 0 UU_1 0
		or			esi, ecx;	// VV_2 0 UU_2 0
		pop			eax;
		xor			ebx, ebx;
		xor			ecx, ecx;
		mov			bx, [eax];	// Y_1
		add			eax, y_p_2;
		mov			cx, [eax];	// Y_2
		mov			edi, ebx;
		mov			eax, ecx;
		and			ebx, 0x00ff;
		and			ecx, 0x00ff;
		and			edi, 0xff00;
		and			eax, 0xff00;
		shl			edi, 0x08;
		shl			eax, 0x08;
		or			ebx, edi;
		or			ecx, eax;
		or			ebx, edx;
		or			ecx, esi;
		pop			edi;
		pop			edx;
		mov			[edi], ebx;
		mov			[edx], ecx;
loop_end:
	}
}
// 最后两行
void yuv420_to_yuyv_line_2_L(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN int yuyv_p,
							 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
							 IN int y_p_2,
							 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
							 IN int u_p_2,
							 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
							 IN int v_p_2,
							 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
							 IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         edx, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		add         edx, yuyv_p;
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		movdqu      xmm1, [eax];        // Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movsd       xmm2, [ebx];        // 0000 0000  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movsd       xmm3, [esi];        // 0000 0000  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		add			 eax, y_p_2;
		add			 ebx, u_p_2;
		add			esi, v_p_2;
		movdqu      xmm5, [eax];
		movsd       xmm6, [ebx];
		movsd       xmm7, [esi];
		sub			 eax, y_p_2;
		sub			 ebx, u_p_2;
		sub 		 esi, v_p_2;
		movdqa      xmm0, xmm6;
		movdqa      xmm4, xmm5;
		punpcklbw   xmm0, xmm7;         // VV14 UU14 ……
		// 结果为 UU_2
		punpcklbw   xmm4, xmm0;
		punpckhbw   xmm5, xmm0;
		pxor        xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// 结果为 UU_0 * 0.25 + UU_2 * 0.75
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		paddw       xmm6, xmm6;
		paddw       xmm7, xmm7;
		paddw       xmm2, gc_nRoundComp_2;
		paddw       xmm3, gc_nRoundComp_2;
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		psrlw       xmm2, 0x02;
		psllw       xmm3, 0x06;
		movdqa      xmm0, xmm1;
		pand        xmm3, gc_dwMaskWHB;
		por         xmm2, xmm3;
		punpcklbw   xmm0, xmm2;
		punpckhbw   xmm1, xmm2;
		movdqu      [edi], xmm0;
		movdqu      [edx], xmm4;
		add         edi, 0x10;
		add         edx, 0x10;
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm5;
		add         eax, 0x10;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x10;
		add         edx, 0x10;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		pxor		 xmm0, xmm0;
		test        ecx, 0x08;
		jz          loop_m4;
		movsd       xmm1, [eax];        // 0000 0000  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm2, [ebx];        // 0000 0000 0000  UU6 UU4 UU2 UU0
		movss       xmm3, [esi];        // 0000 0000 0000  VV6 VV4 VV2 VV0
		add			 eax, y_p_2;
		add			 ebx, u_p_2;
		add			 esi, v_p_2;
		movsd       xmm5, [eax];
		movss       xmm6, [ebx];
		movss       xmm7, [esi];
		sub			 eax, y_p_2;
		sub			 ebx, u_p_2;
		sub 		 esi, v_p_2;
		movdqa      xmm4, xmm6;
		punpcklbw   xmm4, xmm7;         // 0000 0000  VV6 UU6 ……
		// 结果为 UU_2
		punpcklbw   xmm5, xmm4;
		punpcklbw   xmm2, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// 结果为 UU_0 * 0.25 + UU_2 * 0.75
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		paddw       xmm6, xmm6;
		paddw       xmm7, xmm7;
		paddw       xmm2, gc_nRoundComp_2;
		paddw       xmm3, gc_nRoundComp_2;
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		psrlw       xmm2, 0x02;
		psllw       xmm3, 0x06;
		pand        xmm3, gc_dwMaskWHB;
		por         xmm2, xmm3;
		punpcklbw   xmm1, xmm2;
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm5;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
		add         edx, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		movss       xmm1, [eax];        // 0000 0000 0000  Y3 Y2 Y1 Y0
		pinsrw      xmm2, [ebx], 0x00;   // 0000 0000 0000  0 0 UU2 UU0
		pinsrw      xmm3, [esi], 0x00;   // 0000 0000 0000  0 0 VV2 VV0
		add			 eax, y_p_2;
		add			 ebx, u_p_2;
		add			 esi, v_p_2;
		movsd       xmm5, [eax];
		pinsrw      xmm6, [ebx], 0x00;
		pinsrw      xmm7, [esi], 0x00;
		sub			 eax, y_p_2;
		sub			 ebx, u_p_2;
		sub 		 esi, v_p_2;
		movdqa      xmm4, xmm6;
		punpcklbw   xmm4, xmm7;         // 0000 0000 0000  VV2 UU2 VV0 UU0
		// 结果为 UU_2
		punpcklbw   xmm5, xmm4;
		punpcklbw   xmm2, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// 结果为 UU_0 * 0.25 + UU_2 * 0.75
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		paddw       xmm6, xmm6;
		paddw       xmm7, xmm7;
		paddw       xmm2, gc_nRoundComp_2;
		paddw       xmm3, gc_nRoundComp_2;
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		psrlw       xmm2, 0x02;
		psllw       xmm3, 0x06;
		pand        xmm3, gc_dwMaskWHB;
		por         xmm2, xmm3;
		punpcklbw   xmm1, xmm2;
		movsd       [edi], xmm1;
		movsd       [edx], xmm5;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
		add         edx, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		push		edx;
		push		eax;
		xor			ecx, ecx;
		xor			edx, edx;
		xor			eax, eax;
		mov			cl, [ebx];		// UU_P
		mov			dl, [esi];		// VV_P
		add			ebx, u_p_2;
		add			esi, v_p_2;
		mov			ah, [ebx];		// UU_C
		xor			ebx, ebx;
		mov			bl, [esi];		// VV_C
		xor			esi, esi;
		mov			esi, ebx;
		shl			esi, 0x18;
		or			esi, eax;		// VV_C 0 UU_C 0
		shr			eax, 0x08;
		add			cx, ax;
		add			dx, bx;
		add			ax, ax;
		add			bx, bx;
		add			cx, 0x02;
		add			dx, 0x02;
		add			ax, cx;
		add			bx, dx;
		and			eax, 0xfffc;
		and			ebx, 0xfffc;
		shl			eax, 0x06;
		shl			ebx, 0x16;
		or			ebx, eax;		// VV_N 0 UU_N 0
		pop			eax;
		mov			cx, [eax];
		add			eax, y_p_2;
		mov			edx, ecx;
		and			ecx, 0x00ff;
		and			edx, 0xff00;
		or			ecx, esi;
		shl			edx, 0x08;
		or			ecx, edx;
		mov			[edi], ecx;
		pop			edx;
		mov			cx, [eax];
		mov			edi, ecx;
		and			ecx, 0x00ff;
		and			edi, 0xff00;
		or			ecx, ebx;
		shl			edi, 0x08;
		or			ecx, edi;
		mov			[edx], ecx;
loop_end:
	}
}
// 最后一行
void yuv420_to_yuyv_line_1_L(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
							 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
							 IN int u_p_2,
							 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
							 IN int v_p_2,
							 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
							 IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		sub         ebx, u_p_2;
		sub         esi, v_p_2;
		pxor        xmm0, xmm0;
		movdqa      xmm4, gc_nRoundComp_2;
		movdqa      xmm5, gc_dwMaskWHB;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		movdqu      xmm1, [eax];        // Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movsd       xmm2, [ebx];        // 0000 0000  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movsd       xmm3, [esi];        // 0000 0000  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		add			 ebx, u_p_2;
		add			 esi, v_p_2;
		movsd       xmm6, [ebx];
		movsd       xmm7, [esi];
		sub			 ebx, u_p_2;
		sub			 esi, v_p_2;
		punpcklbw   xmm2, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// 结果为 UU_0 * 0.25 + UU_2 * 0.75
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		paddw       xmm6, xmm6;
		paddw       xmm7, xmm7;
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm4;
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		psrlw       xmm2, 0x02;
		psllw       xmm3, 0x06;
		movdqa      xmm7, xmm1;
		pand        xmm3, xmm5;
		por         xmm2, xmm3;
		punpcklbw   xmm1, xmm2;
		punpckhbw   xmm7, xmm2;
		movdqu      [edi], xmm1;
		add         eax, 0x10;
		add         edi, 0x10;
		movdqu      [edi], xmm7;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x10;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		movsd       xmm1, [eax];        // 0000 0000  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm2, [ebx];        // 0000 0000 0000  UU6 UU4 UU2 UU0
		movss       xmm3, [esi];        // 0000 0000 0000  VV6 VV4 VV2 VV0
		add			 ebx, u_p_2;
		add			 esi, v_p_2;
		movss       xmm6, [ebx];
		movss       xmm7, [esi];
		sub			 ebx, u_p_2;
		sub			 esi, v_p_2;
		punpcklbw   xmm2, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// 结果为 UU_0 * 0.25 + UU_2 * 0.75
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		paddw       xmm6, xmm6;
		paddw       xmm7, xmm7;
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm4;
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		psrlw       xmm2, 0x02;
		psllw       xmm3, 0x06;
		pand        xmm3, xmm5;
		por         xmm2, xmm3;
		punpcklbw   xmm1, xmm2;
		movdqu      [edi], xmm1;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		pxor		xmm2, xmm2;
		pxor		xmm3, xmm3;
		pxor		xmm6, xmm6;
		pxor		xmm7, xmm7;
		movss       xmm1, [eax];        // 0000 0000 0000  Y3 Y2 Y1 Y0
		pinsrw		xmm2, [ebx], 0x00;   // 0000 0000 0000  0 0 UU2 UU0
		pinsrw		xmm3, [esi], 0x00;   // 0000 0000 0000  0 0 VV2 VV0
		add			 ebx, u_p_2;
		add			 esi, v_p_2;
		pinsrw		xmm6, [ebx], 0x00;
		pinsrw		xmm7, [esi], 0x00;
		sub			 ebx, u_p_2;
		sub			 esi, v_p_2;
		punpcklbw   xmm2, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// 结果为 UU_0 * 0.25 + UU_2 * 0.75
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		paddw       xmm6, xmm6;
		paddw       xmm7, xmm7;
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm4;
		paddw       xmm2, xmm6;
		paddw       xmm3, xmm7;
		psrlw       xmm2, 0x02;
		psllw       xmm3, 0x06;
		pand        xmm3, xmm5;
		por         xmm2, xmm3;
		punpcklbw    xmm1, xmm2;
		movsd       [edi], xmm1;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		push		eax;
		xor			ecx, ecx;
		xor			edx, edx;
		xor			eax, eax;
		mov			cl, [ebx];
		mov			dl, [esi];
		add			ebx, u_p_2;
		add			esi, v_p_2;
		mov			al, [ebx];
		xor			ebx, ebx;
		mov			bl, [esi];
		add			cx, ax;
		add			dx, bx;
		add			ax, ax;
		add			bx, bx;
		add			cx, 0x02;
		add			dx, 0x02;
		add			cx, ax;
		add			dx, bx;
		and			ecx, 0xfffc;
		and			edx, 0xfffc;
		shl			ecx, 0x06;
		shl			edx, 0x16;
		pop			eax;
		xor			ebx, ebx;
		mov			bx, [eax];
		mov			esi, ebx;
		and			ebx, 0x00ff;
		and			esi, 0xff00;
		shl			esi, 0x08;
		or			ecx, edx;
		or			ebx, esi;
		or			ecx, ebx;
		mov			[edi], ecx;
loop_end:
	}
}

// XDCAM 插值方式：420->422 一行插值出两行（使用上下行），第一场公式为
// F1_Out0 = 0.125 * F1_Prev + 0.875 * F1_Curr    F1_Out1 = 0.625 * F1_Curr + 0.375 * F1_Next
// 第一行
void yuv420_to_yuyv_xdcam_F_line_2_F(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
									 IN int yuyv_p,
									 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
									 IN int y_p_2,
									 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
									 IN int u_p_2,
									 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
									 IN int v_p_2,
									 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
									 IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         edx, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		add         edx, yuyv_p;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		push		ecx;
		push		edx;
		push		edi;
		mov			ecx, y_p_2;
		mov			edx, u_p_2;
		mov			edi, v_p_2;
		movups		xmm1, dword ptr [eax];	// Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movlps      xmm2, dword ptr [ebx];	// 0000 0000  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movlps      xmm3, dword ptr [esi];	// 0000 0000  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		movups		xmm5, dword ptr [eax + ecx];
		movlps      xmm6, dword ptr [ebx + edx];
		movlps      xmm7, dword ptr [esi + edi];
		movaps		xmm4, xmm2;
		movaps		xmm0, xmm1;
		punpcklbw   xmm2, xmm3;         // VV14 UU14 ……
		// 结果为 UU_0
		punpcklbw   xmm0, xmm2;
		punpckhbw   xmm1, xmm2;
		xorps		xmm2, xmm2;         // 0
		punpcklbw   xmm4, xmm2;         // UU_0
		punpcklbw   xmm3, xmm2;         // VV_0
		punpcklbw   xmm6, xmm2;         // UU_2
		punpcklbw   xmm7, xmm2;         // VV_2
		// 结果为 UU_0 * 0.625 + UU_2 * 0.375
		paddw       xmm6, xmm4;		// N + C
		paddw       xmm7, xmm3;
		paddw       xmm4, xmm4;		// C * 2
		paddw       xmm3, xmm3;
		paddw		xmm4, xmm6;		// N + C * 3
		paddw		xmm3, xmm7;
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw		xmm7, xmm7;
		paddw       xmm3, gc_nRoundComp_3;
		paddw       xmm4, gc_nRoundComp_3;
		paddw       xmm7, xmm3;		// N * 3 + C * 5
		paddw       xmm6, xmm4;
		psllw       xmm7, 0x05;
		psrlw       xmm6, 0x03;
		movaps		xmm4, xmm5;
		andps		xmm7, gc_dwMaskWHB;
		orps		xmm7, xmm6;
		punpcklbw   xmm4, xmm7;
		punpckhbw   xmm5, xmm7;
		pop			edi;
		pop			edx;
		movups		dword ptr [edi], xmm0;
		movups		dword ptr [edx], xmm4;
		movups		dword ptr [edi + 0x10], xmm1;
		movups		dword ptr [edx + 0x10], xmm5;
		pop			ecx;
		add         eax, 0x10;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x20;
		add         edx, 0x20;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		push		ecx;
		push		edx;
		push		edi;
		mov			ecx, y_p_2;
		mov			edx, u_p_2;
		mov			edi, v_p_2;
		movlps		xmm1, dword ptr [eax];	// @@@@  @@@@  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm2, dword ptr [ebx];	// 0000 0000  0000  UU6 UU4 UU2 UU0
		movss		xmm3, dword ptr [esi];	// 0000 0000  0000  VV6 VV4 VV2 VV0
		movlps		xmm5, dword ptr [eax + ecx];
		movss		xmm6, dword ptr [ebx + edx];
		movss		xmm7, dword ptr [esi + edi];
		punpcklbw   xmm2, xmm3;         // 0000  0000  VV6 UU6 ……
		punpcklbw   xmm6, xmm7;         // 0000  0000  VV6 UU6 ……
		// 结果为 UU_0
		punpcklbw   xmm1, xmm2;
		xorps		xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // VV_0  UU_0  UV 相间
		punpcklbw   xmm6, xmm0;         // VV_2  UU_2  UV 相间
		// 结果为 UU_0 * 0.625 + UU_2 * 0.375
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm2, xmm6;		// N + C * 3
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw       xmm2, gc_nRoundComp_3;
		paddw       xmm2, xmm6;		// N * 3 + C * 5
		psrlw       xmm2, 0x03;
		packuswb	xmm2, xmm2;		// UV 相间
		punpcklbw   xmm5, xmm2;
		pop			edi;
		pop			edx;
		movups		dword ptr [edi], xmm1;
		movups		dword ptr [edx], xmm5;
		pop			ecx;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
		add         edx, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		pxor        xmm0, xmm0;         // 0
		pxor		xmm2, xmm2;
		pxor		xmm3, xmm3;
		movss       xmm1, [eax];        // 0000 0000 0000  Y3 Y2 Y1 Y0
		pinsrw      xmm2, [ebx], 0x00;  // UU2 UU0
		pinsrw      xmm3, [esi], 0x00;  // VV2 VV0
		movdqa      xmm4, xmm2;
		punpcklbw    xmm2, xmm3;         // 0000 0000 0000 VV2 UU2 VV0 UU0
		push		ecx;
		push		edx;
		push		edi;
		mov			ecx, y_p_2;
		mov			edx, u_p_2;
		mov			edi, v_p_2;
		movss		xmm1, dword ptr [eax];	// 0000 0000  0000  Y3 Y2 Y1 Y0
		pinsrw      xmm2, word ptr [ebx], 0x00;	// @@@@ @@@@ @@@@  @@ UU2 UU0
		pinsrw      xmm3, word ptr [esi], 0x00;	// @@@@ @@@@ @@@@  @@ VV2 VV0
		movss		xmm5, dword ptr [eax + ecx];
		pinsrw      xmm6, word ptr [ebx + edx], 0x00;
		pinsrw      xmm7, word ptr [esi + edi], 0x00;
		punpcklbw   xmm2, xmm3;         // @@@@  @@@@  @@@@  VV2 UU2 VV0 UU0
		punpcklbw   xmm6, xmm7;         // @@@@  @@@@  @@@@  VV2 UU2 VV0 UU0
		// 结果为 UU_0
		punpcklbw   xmm1, xmm2;
		xorps		xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // VV_0  UU_0  UV 相间
		punpcklbw   xmm6, xmm0;         // VV_2  UU_2  UV 相间
		// 结果为 UU_0 * 0.625 + UU_2 * 0.375
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm2, xmm6;		// N + C * 3
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw       xmm2, gc_nRoundComp_3;
		paddw       xmm2, xmm6;		// N * 3 + C * 5
		psrlw       xmm2, 0x03;
		packuswb	xmm2, xmm2;		// UV 相间
		punpcklbw   xmm5, xmm2;
		pop			edi;
		pop			edx;
		movlps		dword ptr [edi], xmm1;
		movlps		dword ptr [edx], xmm5;
		pop			ecx;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
		add         edx, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		push		edx;
		push		edi;
		mov			edi, v_p_2;
		mov			ecx, y_p_2;
		pinsrw      xmm1, word ptr [eax], 0x00;        // @@@@ @@@@ @@@@  @@ Y1 Y0
		pinsrw      xmm5, word ptr [eax + ecx], 0x00;
		movzx		edx, byte ptr [ebx];	// UU_C
		movzx		ecx, byte ptr [esi];	// VV_C
		mov			eax, u_p_2;
		mov			dh, cl;
		movd		xmm2, edx;			// 0000 0000 0000 00 VV0 UU0
		movzx		edx, byte ptr [ebx + eax];	// UU_N
		movzx		ecx, byte ptr [esi + edi];	// VV_N
		pinsrw      xmm6, edx, 0x00;        	// UU_N
		pinsrw      xmm6, ecx, 0x01;        	// VV_N
		xorps		xmm0, xmm0;
		// 结果为 UU_0
		punpcklbw	xmm1, xmm2;
		punpcklbw	xmm2, xmm0;		// C
		// 结果为 UU_0 * 0.625 + UU_2 * 0.375
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm2, xmm6;		// N + C * 3
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw       xmm2, gc_nRoundComp_3;
		paddw       xmm2, xmm6;		// N * 3 + C * 5
		psrlw       xmm2, 0x03;
		packuswb	xmm2, xmm2;		// UV 相间
		punpcklbw   xmm5, xmm2;
		pop			edi;
		pop			edx;
		movss		dword ptr [edi], xmm1;
		movss		dword ptr [edx], xmm5;
loop_end:
	}
}
// 中间行
void yuv420_to_yuyv_xdcam_F_line_2(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
								   IN int yuyv_p,
								   IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
								   IN int y_p_2,
								   IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
								   IN int u_p_2,
								   IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
								   IN int v_p_2,
								   IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
								   IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         edx, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		add         edx, yuyv_p;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		push		ecx;
		push		eax;
		mov			ecx, u_p_2;
		mov			eax, v_p_2;
		movlps		xmm4, dword ptr [ebx];	// @@@@  @@@@  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movlps		xmm5, dword ptr [esi];  // @@@@  @@@@  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		movlps		xmm6, dword ptr [ebx + ecx];	// UU_N
		movlps		xmm7, dword ptr [esi + eax];	// VV_N
		sub         ebx, ecx;
		sub         esi, eax;
		movlps		xmm2, dword ptr [ebx];	// UU_P
		movlps		xmm3, dword ptr [esi];	// VV_P
		xorps		xmm0, xmm0;
		punpcklbw   xmm4, xmm0;     // UU_C
		punpcklbw   xmm5, xmm0;     // VV_C
		punpcklbw   xmm2, xmm0;     // UU_P
		punpcklbw   xmm3, xmm0;     // VV_P
		punpcklbw   xmm6, xmm0;     // UU_N
		punpcklbw   xmm7, xmm0;     // VV_N
		// 第一个结果：UU_P * 0.125 + UU_C * 0.875、第二个结果：UU_N * 0.375 + UU_C * 0.625
		paddw       xmm2, xmm4;		// P + C
		paddw       xmm3, xmm5;
		paddw       xmm6, xmm4;		// N + C
		paddw       xmm7, xmm5;
		paddw		xmm4, xmm4;		// C * 2
		paddw		xmm5, xmm5;
		movaps		xmm0, xmm4;
		movaps		xmm1, xmm5;
		paddw		xmm2, xmm4;		// P + C * 3
		paddw		xmm3, xmm5;
		paddw		xmm4, xmm6;		// N + C * 3
		paddw		xmm5, xmm7;
		paddw		xmm0, xmm0;		// C * 4
		paddw		xmm1, xmm1;
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw		xmm7, xmm7;
		paddw		xmm3, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm5, gc_nRoundComp_3;
		paddw		xmm2, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm4, gc_nRoundComp_3;
		paddw		xmm3, xmm1;		// P + C * 7
		paddw		xmm7, xmm5;		// N * 3 + C * 5
		paddw		xmm2, xmm0;		// P + C * 7
		paddw		xmm6, xmm4;		// N * 3 + C * 5
		psllw       xmm3, 0x05;
		psllw       xmm7, 0x05;
		psrlw		xmm2, 0x03;				// U
		psrlw		xmm6, 0x03;
		andps		xmm3, gc_dwMaskWHB;		// V
		andps		xmm7, gc_dwMaskWHB;
		orps		xmm3, xmm2;
		orps		xmm7, xmm6;
		add         ebx, ecx;
		add         esi, eax;
		mov			ecx, y_p_2;
		pop			eax;
		movups		xmm0, dword ptr [eax];        // Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movups		xmm4, dword ptr [eax + ecx];
		movaps		xmm1, xmm0;
		movaps		xmm5, xmm4;
		punpcklbw   xmm0, xmm3;
		punpcklbw   xmm4, xmm7;
		punpckhbw   xmm1, xmm3;
		punpckhbw   xmm5, xmm7;
		movups		dword ptr [edi], xmm0;
		movups		dword ptr [edx], xmm4;
		movups		dword ptr [edi + 0x10], xmm1;
		movups		dword ptr [edx + 0x10], xmm5;
		pop			ecx;
		add         eax, 0x10;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x20;
		add         edx, 0x20;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		push		ecx;
		push		eax;
		mov			ecx, u_p_2;
		mov			eax, v_p_2;
		movss		xmm4, dword ptr [ebx];	// 0000 0000 0000  UU6 UU4 UU2 UU0
		movss		xmm5, dword ptr [esi];  // 0000 0000 0000  VV6 VV4 VV2 VV0
		movss		xmm6, dword ptr [ebx + ecx];	// UU_N
		movss		xmm7, dword ptr [esi + eax];	// VV_N
		sub         ebx, ecx;
		sub         esi, eax;
		movss		xmm2, dword ptr [ebx];	// UU_P
		movss		xmm3, dword ptr [esi];	// VV_P
		punpcklbw	xmm4, xmm5;		// UV_C 相间
		punpcklbw	xmm6, xmm7;		// UV_N 相间
		punpcklbw	xmm2, xmm3;		// UV_P 相间
		xorps		xmm0, xmm0;
		punpcklbw   xmm4, xmm0;     // VV_C  UU_C 相间
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P 相间
		punpcklbw   xmm6, xmm0;     // VV_N  UU_N 相间
		// 第一个结果：UU_P * 0.125 + UU_C * 0.875、第二个结果：UU_N * 0.375 + UU_C * 0.625
		paddw       xmm2, xmm4;		// P + C
		paddw       xmm6, xmm4;		// N + C
		paddw		xmm4, xmm4;		// C * 2
		movaps		xmm0, xmm4;
		paddw		xmm2, xmm4;		// P + C * 3
		paddw		xmm4, xmm6;		// N + C * 3
		paddw		xmm0, xmm0;		// C * 4
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw		xmm2, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm4, gc_nRoundComp_3;
		paddw		xmm2, xmm0;		// P + C * 7
		paddw		xmm6, xmm4;		// N * 3 + C * 5
		psrlw		xmm2, 0x03;
		psrlw		xmm6, 0x03;
		packuswb	xmm2, xmm2;
		packuswb	xmm6, xmm6;
		add         ebx, ecx;
		add         esi, eax;
		mov			ecx, y_p_2;
		pop			eax;
		movlps		xmm0, dword ptr [eax];        // @@@@ @@@@  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movlps		xmm4, dword ptr [eax + ecx];
		punpcklbw   xmm0, xmm2;
		punpcklbw   xmm4, xmm6;
		movups		dword ptr [edi], xmm0;
		movups		dword ptr [edx], xmm4;
		pop			ecx;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
		add         edx, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		push		ecx;
		push		eax;
		mov			ecx, u_p_2;
		mov			eax, v_p_2;
		pinsrw      xmm4, word ptr [ebx], 0x00;        // @@@@ @@@@ @@@@  @@ UU2 UU0
		pinsrw      xmm4, word ptr [esi], 0x02;        // @@@@ @@@@  @@ VV2 VV0  @@ UU2 UU0
		pinsrw      xmm6, word ptr [ebx + ecx], 0x00;    // UU_N
		pinsrw      xmm6, word ptr [esi + eax], 0x02;    // VV_N
		sub         ebx, ecx;
		sub         esi, eax;
		pinsrw      xmm2, word ptr [ebx], 0x00;        // UU_P
		pinsrw      xmm2, word ptr [esi], 0x02;        // VV_P
		xorps		xmm0, xmm0;
		punpcklbw   xmm4, xmm0;     // VV_C  UU_C
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P
		punpcklbw   xmm6, xmm0;     // VV_N  UU_N
		// 第一个结果：UU_P * 0.125 + UU_C * 0.875、第二个结果：UU_N * 0.375 + UU_C * 0.625
		paddw       xmm2, xmm4;		// P + C
		paddw       xmm6, xmm4;		// N + C
		paddw		xmm4, xmm4;		// C * 2
		movaps		xmm0, xmm4;
		paddw		xmm2, xmm4;		// P + C * 3
		paddw		xmm4, xmm6;		// N + C * 3
		paddw		xmm0, xmm0;		// C * 4
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw		xmm2, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm4, gc_nRoundComp_3;
		paddw		xmm2, xmm0;		// P + C * 7
		paddw		xmm6, xmm4;		// N * 3 + C * 5
		movhlps		xmm3, xmm2;
		movhlps		xmm7, xmm6;
		psllw       xmm3, 0x05;
		psllw       xmm7, 0x05;
		psrlw		xmm2, 0x03;				// U
		psrlw		xmm6, 0x03;
		andps		xmm3, gc_dwMaskWHB;		// V
		andps		xmm7, gc_dwMaskWHB;
		orps		xmm3, xmm2;
		orps		xmm7, xmm6;
		add         ebx, ecx;
		add         esi, eax;
		mov			ecx, y_p_2;
		pop			eax;
		movss		xmm0, dword ptr [eax];        // 0000 0000 0000  Y3 Y2 Y1 Y0
		movss		xmm4, dword ptr [eax + ecx];
		punpcklbw   xmm0, xmm3;
		punpcklbw   xmm4, xmm7;
		movlps		dword ptr [edi], xmm0;
		movlps		dword ptr [edx], xmm4;
		pop			ecx;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
		add         edx, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		push		edx;
		push		edi;
		push		eax;
		mov			eax, u_p_2;
		mov			edi, v_p_2;
		movzx		edx, byte ptr [ebx];	// UU_C
		movzx		ecx, byte ptr [esi];	// VV_C
		pinsrw      xmm4, edx, 0x00;        // @@@@ @@@@ @@@@  @@ 0 UU0
		pinsrw      xmm4, ecx, 0x01;        // @@@@ @@ @@@@  VV0 UU0
		movzx		edx, byte ptr [ebx + eax];	// UU_N
		movzx		ecx, byte ptr [esi + edi];	// VV_N
		pinsrw      xmm6, edx, 0x00;        	// UU_N
		pinsrw      xmm6, ecx, 0x01;        	// VV_N
		sub         ebx, eax;
		sub         esi, edi;
		movzx		edx, byte ptr [ebx];	// UU_P
		movzx		ecx, byte ptr [esi];	// VV_P
		pinsrw      xmm2, edx, 0x00;        // UU_P
		pinsrw      xmm2, ecx, 0x01;        // VV_P
		pop			eax;
		pop			edi;
		pop			edx;
		mov			ecx, y_p_2;
		// 第一个结果：UU_P * 0.125 + UU_C * 0.875、第二个结果：UU_N * 0.375 + UU_C * 0.625
		paddw       xmm2, xmm4;		// P + C
		paddw       xmm6, xmm4;		// N + C
		paddw		xmm4, xmm4;		// C * 2
		movaps		xmm0, xmm4;
		paddw		xmm2, xmm4;		// P + C * 3
		paddw		xmm4, xmm6;		// N + C * 3
		paddw		xmm0, xmm0;		// C * 4
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw		xmm2, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm4, gc_nRoundComp_3;
		paddw		xmm2, xmm0;		// P + C * 7
		paddw		xmm6, xmm4;		// N * 3 + C * 5
		psrlw		xmm2, 0x03;
		psrlw		xmm6, 0x03;
		packuswb	xmm2, xmm2;
		packuswb	xmm6, xmm6;
		pinsrw      xmm0, word ptr [eax], 0x00;        // @@@@ @@@@ @@@@  @@ Y1 Y0
		pinsrw      xmm4, word ptr [eax + ecx], 0x00;
		punpcklbw   xmm0, xmm3;
		punpcklbw   xmm4, xmm7;
		movss		dword ptr [edi], xmm0;
		movss		dword ptr [edx], xmm4;
loop_end:
	}
}
// 最后一行
void yuv420_to_yuyv_xdcam_F_line_1_L(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
									 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
									 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
									 IN int u_p_2,
									 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
									 IN int v_p_2,
									 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
									 IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		xorps		xmm0, xmm0;
		movaps		xmm4, gc_nRoundComp_3;
		movaps		xmm5, gc_dwMaskWHB;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		movups		xmm1, dword ptr [eax];        // Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movlps      xmm6, dword ptr [ebx];        // @@@@ @@@@  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movlps      xmm7, dword ptr [esi];        // @@@@ @@@@  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		sub			ebx, u_p_2;
		sub			esi, v_p_2;
		movlps      xmm2, dword ptr [ebx];
		movlps      xmm3, dword ptr [esi];
		add			ebx, u_p_2;
		add			esi, v_p_2;
		punpcklbw   xmm2, xmm0;         // UU_P
		punpcklbw   xmm3, xmm0;         // VV_P
		punpcklbw   xmm6, xmm0;         // UU_C
		punpcklbw   xmm7, xmm0;         // VV_C
		// 结果为 UU_P * 0.125 + UU_C * 0.875
		paddw       xmm2, xmm6;		// P + C
		paddw       xmm3, xmm7;
		paddw       xmm6, xmm6;		// C * 2
		paddw       xmm7, xmm7;
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm4;
		paddw       xmm2, xmm6;		// P + C * 3
		paddw       xmm3, xmm7;
		paddw       xmm6, xmm6;		// C * 4
		paddw       xmm7, xmm7;
		paddw       xmm2, xmm6;		// P + C * 7
		paddw       xmm3, xmm7;
		psrlw       xmm2, 0x03;
		psllw       xmm3, 0x05;
		movaps		xmm7, xmm1;
		andps		xmm3, xmm5;
		orps		xmm3, xmm2;
		punpcklbw   xmm1, xmm3;
		punpckhbw   xmm7, xmm3;
		movups		dword ptr [edi], xmm1;
		movups		dword ptr [edi + 0x10], xmm7;
		add         eax, 0x10;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x20;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		movlps		xmm1, dword ptr [eax];        // @@@@ @@@@  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm6, dword ptr [ebx];        // 0000 0000  0000  UU6 UU4 UU2 UU0
		movss       xmm7, dword ptr [esi];        // 0000 0000  0000  VV6 VV4 VV2 VV0
		sub			ebx, u_p_2;
		sub			esi, v_p_2;
		movss       xmm2, dword ptr [ebx];
		movss       xmm3, dword ptr [esi];
		add			ebx, u_p_2;
		add			esi, v_p_2;
		punpcklbw	xmm2, xmm3;		// VV_P  UU_P  相间
		punpcklbw	xmm6, xmm7;		// VV_C  UU_C  相间
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P  相间
		punpcklbw   xmm6, xmm0;     // VV_C  UU_C  相间
		// 结果为 UU_P * 0.125 + UU_C * 0.875
		paddw       xmm2, xmm6;		// P + C
		paddw       xmm6, xmm6;		// C * 2
		paddw       xmm2, xmm4;
		paddw       xmm2, xmm6;		// P + C * 3
		paddw       xmm6, xmm6;		// C * 4
		paddw       xmm2, xmm6;		// P + C * 7
		psrlw       xmm2, 0x03;
		packuswb	xmm2, xmm2;
		punpcklbw   xmm1, xmm2;
		movups		dword ptr [edi], xmm1;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		movss		xmm1, dword ptr [eax];        // 0000  0000  0000  Y3 Y2 Y1 Y0
		pinsrw		xmm6, word ptr [ebx], 0x00;   // @@@@  @@@@  @@@@  @@ UU2 UU0
		pinsrw		xmm6, word ptr [esi], 0x02;   // @@@@  @@@@  @@@@  VV2 VV0 UU2 UU0
		sub			ebx, u_p_2;
		sub			esi, v_p_2;
		pinsrw		xmm2, word ptr [ebx], 0x00;
		pinsrw		xmm2, word ptr [esi], 0x02;
		add			ebx, u_p_2;
		add			esi, v_p_2;
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P  相间
		punpcklbw   xmm6, xmm0;     // VV_C  UU_C  相间
		// 结果为 UU_P * 0.125 + UU_C * 0.875
		paddw       xmm2, xmm6;		// P + C
		paddw       xmm6, xmm6;		// C * 2
		paddw       xmm2, xmm4;
		paddw       xmm2, xmm6;		// P + C * 3
		paddw       xmm6, xmm6;		// C * 4
		paddw       xmm2, xmm6;		// P + C * 7
		movhlps		xmm3, xmm2;
		psrlw       xmm2, 0x03;
		psllw       xmm3, 0x05;
		andps		xmm3, xmm5;
		orps		xmm3, xmm2;
		punpcklbw   xmm1, xmm3;
		movlps		dword ptr [edi], xmm1;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		pinsrw      xmm1, word ptr [eax], 0x00;        // @@@@ @@@@ @@@@  @@ Y1 Y0
		movzx		edx, byte ptr [ebx];	// UU_C
		movzx		ecx, byte ptr [esi];	// VV_C
		sub			ebx, u_p_2;
		sub			esi, v_p_2;
		pinsrw      xmm2, edx, 0x00;        // UU_C
		pinsrw      xmm2, ecx, 0x01;        // VV_C
		movzx		edx, byte ptr [ebx];	// UU_P
		movzx		ecx, byte ptr [esi];	// VV_P
		pinsrw      xmm6, edx, 0x00;        // UU_P
		pinsrw      xmm6, ecx, 0x01;        // VV_P
		// 结果为 UU_P * 0.125 + UU_C * 0.875
		paddw       xmm6, xmm2;		// P + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm6, xmm2;		// P + C * 3
		paddw		xmm2, xmm2;		// C * 4
		paddw       xmm6, xmm4;
		paddw       xmm6, xmm2;		// N + C * 7
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV 相间
		punpcklbw   xmm1, xmm6;
		movss		dword ptr [edi], xmm1;
loop_end:
	}
}
// XDCAM 插值方式：420->422 一行插值出两行（使用上下行），第二场公式为
// F2_Out0 = 0.375 * F1_Prev + 0.625 * F1_Curr    F1_Out1 = 0.875 * F1_Curr + 0.125 * F1_Next
// 第一行
void yuv420_to_yuyv_xdcam_S_line_2_F(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
									 IN int yuyv_p,
									 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
									 IN int y_p_2,
									 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
									 IN int u_p_2,
									 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
									 IN int v_p_2,
									 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
									 IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         edx, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		add         edx, yuyv_p;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		push		ecx;
		push		edx;
		push		edi;
		mov			ecx, y_p_2;
		mov			edx, u_p_2;
		mov			edi, v_p_2;
		movups		xmm1, dword ptr [eax];	// Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movlps      xmm2, dword ptr [ebx];	// 0000 0000  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movlps      xmm3, dword ptr [esi];	// 0000 0000  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		movups		xmm5, dword ptr [eax + ecx];
		movlps      xmm6, dword ptr [ebx + edx];
		movlps      xmm7, dword ptr [esi + edi];
		movaps		xmm4, xmm2;
		movaps		xmm0, xmm1;
		punpcklbw   xmm2, xmm3;         // VV14 UU14 ……
		// 结果为 UU_0
		punpcklbw   xmm0, xmm2;
		punpckhbw   xmm1, xmm2;
		xorps		xmm2, xmm2;         // 0
		punpcklbw   xmm4, xmm2;         // UU_0
		punpcklbw   xmm3, xmm2;         // VV_0
		punpcklbw   xmm6, xmm2;         // UU_2
		punpcklbw   xmm7, xmm2;         // VV_2
		// 结果为 UU_0 * 0.875 + UU_2 * 0.125
		paddw       xmm6, xmm4;		// N + C
		paddw       xmm7, xmm3;
		paddw       xmm4, xmm4;		// C * 2
		paddw       xmm3, xmm3;
		paddw		xmm6, xmm4;		// N + C * 3
		paddw		xmm7, xmm3;
		paddw		xmm4, xmm4;		// C * 4
		paddw		xmm3, xmm3;
		paddw       xmm7, gc_nRoundComp_3;
		paddw       xmm6, gc_nRoundComp_3;
		paddw       xmm7, xmm3;		// N + C * 7
		paddw       xmm6, xmm4;
		psllw       xmm7, 0x05;
		psrlw       xmm6, 0x03;
		movaps		xmm4, xmm5;
		andps		xmm7, gc_dwMaskWHB;
		orps		xmm7, xmm6;
		punpcklbw   xmm4, xmm7;
		punpckhbw   xmm5, xmm7;
		pop			edi;
		pop			edx;
		movups		dword ptr [edi], xmm0;
		movups		dword ptr [edx], xmm4;
		movups		dword ptr [edi + 0x10], xmm1;
		movups		dword ptr [edx + 0x10], xmm5;
		pop			ecx;
		add         eax, 0x10;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x20;
		add         edx, 0x20;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		push		ecx;
		push		edx;
		push		edi;
		mov			ecx, y_p_2;
		mov			edx, u_p_2;
		mov			edi, v_p_2;
		movlps		xmm1, dword ptr [eax];	// @@@@  @@@@  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm2, dword ptr [ebx];	// 0000 0000  0000  UU6 UU4 UU2 UU0
		movss		xmm3, dword ptr [esi];	// 0000 0000  0000  VV6 VV4 VV2 VV0
		movlps		xmm5, dword ptr [eax + ecx];
		movss		xmm6, dword ptr [ebx + edx];
		movss		xmm7, dword ptr [esi + edi];
		punpcklbw   xmm2, xmm3;         // 0000  0000  VV6 UU6 ……
		punpcklbw   xmm6, xmm7;         // 0000  0000  VV6 UU6 ……
		// 结果为 UU_0
		punpcklbw   xmm1, xmm2;
		xorps		xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // VV_0  UU_0  UV 相间
		punpcklbw   xmm6, xmm0;         // VV_2  UU_2  UV 相间
		// 结果为 UU_0 * 0.875 + UU_2 * 0.125
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm6, xmm2;		// N + C * 3
		paddw		xmm2, xmm2;		// C * 4
		paddw       xmm6, gc_nRoundComp_3;
		paddw       xmm6, xmm2;		// N + C * 7
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV 相间
		punpcklbw   xmm5, xmm6;
		pop			edi;
		pop			edx;
		movups		dword ptr [edi], xmm1;
		movups		dword ptr [edx], xmm5;
		pop			ecx;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
		add         edx, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		pxor        xmm0, xmm0;         // 0
		pxor		xmm2, xmm2;
		pxor		xmm3, xmm3;
		movss       xmm1, [eax];        // 0000 0000 0000  Y3 Y2 Y1 Y0
		pinsrw      xmm2, [ebx], 0x00;  // UU2 UU0
		pinsrw      xmm3, [esi], 0x00;  // VV2 VV0
		movdqa      xmm4, xmm2;
		punpcklbw    xmm2, xmm3;         // 0000 0000 0000 VV2 UU2 VV0 UU0
		push		ecx;
		push		edx;
		push		edi;
		mov			ecx, y_p_2;
		mov			edx, u_p_2;
		mov			edi, v_p_2;
		movss		xmm1, dword ptr [eax];	// 0000 0000  0000  Y3 Y2 Y1 Y0
		pinsrw      xmm2, word ptr [ebx], 0x00;	// @@@@ @@@@ @@@@  @@ UU2 UU0
		pinsrw      xmm3, word ptr [esi], 0x00;	// @@@@ @@@@ @@@@  @@ VV2 VV0
		movss		xmm5, dword ptr [eax + ecx];
		pinsrw      xmm6, word ptr [ebx + edx], 0x00;
		pinsrw      xmm7, word ptr [esi + edi], 0x00;
		punpcklbw   xmm2, xmm3;         // @@@@  @@@@  @@@@  VV2 UU2 VV0 UU0
		punpcklbw   xmm6, xmm7;         // @@@@  @@@@  @@@@  VV2 UU2 VV0 UU0
		// 结果为 UU_0
		punpcklbw   xmm1, xmm2;
		xorps		xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // VV_0  UU_0  UV 相间
		punpcklbw   xmm6, xmm0;         // VV_2  UU_2  UV 相间
		// 结果为 UU_0 * 0.875 + UU_2 * 0.125
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm6, xmm2;		// N + C * 3
		paddw		xmm2, xmm2;		// C * 4
		paddw       xmm6, gc_nRoundComp_3;
		paddw       xmm6, xmm2;		// N + C * 7
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV 相间
		punpcklbw   xmm5, xmm6;
		pop			edi;
		pop			edx;
		movlps		dword ptr [edi], xmm1;
		movlps		dword ptr [edx], xmm5;
		pop			ecx;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
		add         edx, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		push		edx;
		push		edi;
		mov			edi, v_p_2;
		mov			ecx, y_p_2;
		pinsrw      xmm1, word ptr [eax], 0x00;        // @@@@ @@@@ @@@@  @@ Y1 Y0
		pinsrw      xmm5, word ptr [eax + ecx], 0x00;
		movzx		edx, byte ptr [ebx];	// UU_C
		movzx		ecx, byte ptr [esi];	// VV_C
		mov			eax, u_p_2;
		mov			dh, cl;
		movd		xmm2, edx;			// 0000 0000 0000 00 VV0 UU0
		movzx		edx, byte ptr [ebx + eax];	// UU_N
		movzx		ecx, byte ptr [esi + edi];	// VV_N
		pinsrw      xmm6, edx, 0x00;        	// UU_N
		pinsrw      xmm6, ecx, 0x01;        	// VV_N
		xorps		xmm0, xmm0;
		// 结果为 UU_0
		punpcklbw	xmm1, xmm2;
		punpcklbw	xmm2, xmm0;		// C
		// 结果为 UU_0 * 0.875 + UU_2 * 0.125
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm6, xmm2;		// N + C * 3
		paddw		xmm2, xmm2;		// C * 4
		paddw       xmm6, gc_nRoundComp_3;
		paddw       xmm6, xmm2;		// N + C * 7
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV 相间
		punpcklbw   xmm5, xmm6;
		pop			edi;
		pop			edx;
		movss		dword ptr [edi], xmm1;
		movss		dword ptr [edx], xmm5;
loop_end:
	}
}
// 中间行
void yuv420_to_yuyv_xdcam_S_line_2(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
								   IN int yuyv_p,
								   IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
								   IN int y_p_2,
								   IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
								   IN int u_p_2,
								   IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
								   IN int v_p_2,
								   IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
								   IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         edx, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		add         edx, yuyv_p;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		push		ecx;
		push		eax;
		mov			ecx, u_p_2;
		mov			eax, v_p_2;
		movlps		xmm4, dword ptr [ebx];	// @@@@  @@@@  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movlps		xmm5, dword ptr [esi];  // @@@@  @@@@  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		movlps		xmm6, dword ptr [ebx + ecx];	// UU_N
		movlps		xmm7, dword ptr [esi + eax];	// VV_N
		sub         ebx, ecx;
		sub         esi, eax;
		movlps		xmm2, dword ptr [ebx];	// UU_P
		movlps		xmm3, dword ptr [esi];	// VV_P
		xorps		xmm0, xmm0;
		punpcklbw   xmm4, xmm0;     // UU_C
		punpcklbw   xmm5, xmm0;     // VV_C
		punpcklbw   xmm2, xmm0;     // UU_P
		punpcklbw   xmm3, xmm0;     // VV_P
		punpcklbw   xmm6, xmm0;     // UU_N
		punpcklbw   xmm7, xmm0;     // VV_N
		// 第一个结果：UU_P * 0.375 + UU_C * 0.625、第二个结果：UU_N * 0.125 + UU_C * 0.875
		paddw       xmm2, xmm4;		// P + C
		paddw       xmm3, xmm5;
		paddw       xmm6, xmm4;		// N + C
		paddw       xmm7, xmm5;
		paddw		xmm4, xmm4;		// C * 2
		paddw		xmm5, xmm5;
		movaps		xmm0, xmm4;
		movaps		xmm1, xmm5;
		paddw		xmm6, xmm4;		// N + C * 3
		paddw		xmm7, xmm5;
		paddw		xmm4, xmm2;		// P + C * 3
		paddw		xmm5, xmm3;
		paddw		xmm2, xmm2;		// P * 2 + C * 2
		paddw		xmm3, xmm3;
		paddw		xmm0, xmm0;		// C * 4
		paddw		xmm1, xmm1;
		paddw		xmm3, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm7, gc_nRoundComp_3;
		paddw		xmm2, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm6, gc_nRoundComp_3;
		paddw		xmm3, xmm5;		// P * 3 + C * 5
		paddw		xmm7, xmm1;		// N + C * 7
		paddw		xmm2, xmm4;		// P * 3 + C * 5
		paddw		xmm6, xmm0;		// N + C * 7
		psllw       xmm3, 0x05;
		psllw       xmm7, 0x05;
		psrlw		xmm2, 0x03;				// U
		psrlw		xmm6, 0x03;
		andps		xmm3, gc_dwMaskWHB;		// V
		andps		xmm7, gc_dwMaskWHB;
		orps		xmm3, xmm2;
		orps		xmm7, xmm6;
		add         ebx, ecx;
		add         esi, eax;
		mov			ecx, y_p_2;
		pop			eax;
		movups		xmm0, dword ptr [eax];        // Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movups		xmm4, dword ptr [eax + ecx];
		movaps		xmm1, xmm0;
		movaps		xmm5, xmm4;
		punpcklbw   xmm0, xmm3;
		punpcklbw   xmm4, xmm7;
		punpckhbw   xmm1, xmm3;
		punpckhbw   xmm5, xmm7;
		movups		dword ptr [edi], xmm0;
		movups		dword ptr [edx], xmm4;
		movups		dword ptr [edi + 0x10], xmm1;
		movups		dword ptr [edx + 0x10], xmm5;
		pop			ecx;
		add         eax, 0x10;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x20;
		add         edx, 0x20;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		push		ecx;
		push		eax;
		mov			ecx, u_p_2;
		mov			eax, v_p_2;
		movss		xmm4, dword ptr [ebx];	// 0000 0000 0000  UU6 UU4 UU2 UU0
		movss		xmm5, dword ptr [esi];  // 0000 0000 0000  VV6 VV4 VV2 VV0
		movss		xmm6, dword ptr [ebx + ecx];	// UU_N
		movss		xmm7, dword ptr [esi + eax];	// VV_N
		sub         ebx, ecx;
		sub         esi, eax;
		movss		xmm2, dword ptr [ebx];	// UU_P
		movss		xmm3, dword ptr [esi];	// VV_P
		punpcklbw	xmm4, xmm5;		// UV_C 相间
		punpcklbw	xmm6, xmm7;		// UV_N 相间
		punpcklbw	xmm2, xmm3;		// UV_P 相间
		xorps		xmm0, xmm0;
		punpcklbw   xmm4, xmm0;     // VV_C  UU_C 相间
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P 相间
		punpcklbw   xmm6, xmm0;     // VV_N  UU_N 相间
		// 第一个结果：UU_P * 0.125 + UU_C * 0.875、第二个结果：UU_N * 0.375 + UU_C * 0.625
		paddw       xmm2, xmm4;		// P + C
		paddw       xmm6, xmm4;		// N + C
		paddw		xmm4, xmm4;		// C * 2
		movaps		xmm0, xmm4;
		paddw		xmm6, xmm4;		// N + C * 3
		paddw		xmm4, xmm2;		// P + C * 3
		paddw		xmm0, xmm0;		// C * 4
		paddw		xmm2, xmm2;		// P * 2 + C * 2
		paddw		xmm4, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm6, gc_nRoundComp_3;
		paddw		xmm2, xmm4;		// P * 3 + C * 5
		paddw		xmm6, xmm0;		// N + C * 7
		psrlw		xmm2, 0x03;
		psrlw		xmm6, 0x03;
		packuswb	xmm2, xmm2;
		packuswb	xmm6, xmm6;
		add         ebx, ecx;
		add         esi, eax;
		mov			ecx, y_p_2;
		pop			eax;
		movlps		xmm0, dword ptr [eax];        // @@@@ @@@@  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movlps		xmm4, dword ptr [eax + ecx];
		punpcklbw   xmm0, xmm2;
		punpcklbw   xmm4, xmm6;
		movups		dword ptr [edi], xmm0;
		movups		dword ptr [edx], xmm4;
		pop			ecx;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
		add         edx, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		push		ecx;
		push		eax;
		mov			ecx, u_p_2;
		mov			eax, v_p_2;
		pinsrw      xmm4, word ptr [ebx], 0x00;        // @@@@ @@@@ @@@@  @@ UU2 UU0
		pinsrw      xmm4, word ptr [esi], 0x02;        // @@@@ @@@@  @@ VV2 VV0  @@ UU2 UU0
		pinsrw      xmm6, word ptr [ebx + ecx], 0x00;    // UU_N
		pinsrw      xmm6, word ptr [esi + eax], 0x02;    // VV_N
		sub         ebx, ecx;
		sub         esi, eax;
		pinsrw      xmm2, word ptr [ebx], 0x00;        // UU_P
		pinsrw      xmm2, word ptr [esi], 0x02;        // VV_P
		xorps		xmm0, xmm0;
		punpcklbw   xmm4, xmm0;     // VV_C  UU_C
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P
		punpcklbw   xmm6, xmm0;     // VV_N  UU_N
		// 第一个结果：UU_P * 0.125 + UU_C * 0.875、第二个结果：UU_N * 0.375 + UU_C * 0.625
		paddw       xmm2, xmm4;		// P + C
		paddw       xmm6, xmm4;		// N + C
		paddw		xmm4, xmm4;		// C * 2
		movaps		xmm0, xmm4;
		paddw		xmm6, xmm4;		// N + C * 3
		paddw		xmm4, xmm2;		// P + C * 3
		paddw		xmm0, xmm0;		// C * 4
		paddw		xmm2, xmm2;		// P * 2 + C * 2
		paddw		xmm4, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm6, gc_nRoundComp_3;
		paddw		xmm2, xmm4;		// P * 3 + C * 5
		paddw		xmm6, xmm0;		// N + C * 7
		movhlps		xmm3, xmm2;
		movhlps		xmm7, xmm6;
		psllw       xmm3, 0x05;
		psllw       xmm7, 0x05;
		psrlw		xmm2, 0x03;				// U
		psrlw		xmm6, 0x03;
		andps		xmm3, gc_dwMaskWHB;		// V
		andps		xmm7, gc_dwMaskWHB;
		orps		xmm3, xmm2;
		orps		xmm7, xmm6;
		add         ebx, ecx;
		add         esi, eax;
		mov			ecx, y_p_2;
		pop			eax;
		movss		xmm0, dword ptr [eax];        // 0000 0000 0000  Y3 Y2 Y1 Y0
		movss		xmm4, dword ptr [eax + ecx];
		punpcklbw   xmm0, xmm3;
		punpcklbw   xmm4, xmm7;
		movlps		dword ptr [edi], xmm0;
		movlps		dword ptr [edx], xmm4;
		pop			ecx;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
		add         edx, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		push		edx;
		push		edi;
		push		eax;
		mov			eax, u_p_2;
		mov			edi, v_p_2;
		movzx		edx, byte ptr [ebx];	// UU_C
		movzx		ecx, byte ptr [esi];	// VV_C
		pinsrw      xmm4, edx, 0x00;        // @@@@ @@@@ @@@@  @@ 0 UU0
		pinsrw      xmm4, ecx, 0x01;        // @@@@ @@@@ @@@@  VV0 UU0
		movzx		edx, byte ptr [ebx + eax];	// UU_N
		movzx		ecx, byte ptr [esi + edi];	// VV_N
		pinsrw      xmm6, edx, 0x00;        	// UU_N
		pinsrw      xmm6, ecx, 0x01;        	// VV_N
		sub         ebx, eax;
		sub         esi, edi;
		movzx		edx, byte ptr [ebx];	// UU_P
		movzx		ecx, byte ptr [esi];	// VV_P
		pinsrw      xmm2, edx, 0x00;        // UU_P
		pinsrw      xmm2, ecx, 0x01;        // VV_P
		pop			eax;
		pop			edi;
		pop			edx;
		mov			ecx, y_p_2;
		// 第一个结果：UU_P * 0.125 + UU_C * 0.875、第二个结果：UU_N * 0.375 + UU_C * 0.625
		paddw       xmm2, xmm4;		// P + C
		paddw       xmm6, xmm4;		// N + C
		paddw		xmm4, xmm4;		// C * 2
		movaps		xmm0, xmm4;
		paddw		xmm6, xmm4;		// N + C * 3
		paddw		xmm4, xmm2;		// P + C * 3
		paddw		xmm0, xmm0;		// C * 4
		paddw		xmm2, xmm2;		// P * 2 + C * 2
		paddw		xmm4, gc_nRoundComp_3;		// 8 * 0.5 = 4
		paddw		xmm6, gc_nRoundComp_3;
		paddw		xmm2, xmm4;		// P * 3 + C * 5
		paddw		xmm6, xmm0;		// N + C * 7
		psrlw		xmm2, 0x03;
		psrlw		xmm6, 0x03;
		packuswb	xmm2, xmm2;
		packuswb	xmm6, xmm6;
		pinsrw      xmm0, word ptr [eax], 0x00;        // @@@@ @@@@ @@@@  @@ Y1 Y0
		pinsrw      xmm4, word ptr [eax + ecx], 0x00;
		punpcklbw   xmm0, xmm2;
		punpcklbw   xmm4, xmm6;
		movss		dword ptr [edi], xmm0;
		movss		dword ptr [edx], xmm4;
loop_end:
	}
}
// 最后一行
void yuv420_to_yuyv_xdcam_S_line_1_L(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
									 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
									 IN BYTE* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
									 IN int u_p_2,
									 IN BYTE* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
									 IN int v_p_2,
									 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
									 IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         eax, y;
		mov         ebx, u;
		mov         esi, v;
		xorps		xmm0, xmm0;
		movaps		xmm4, gc_nRoundComp_3;
		movaps		xmm5, gc_dwMaskWHB;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		movups		xmm1, dword ptr [eax];        // Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movlps      xmm6, dword ptr [ebx];        // @@@@ @@@@  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movlps      xmm7, dword ptr [esi];        // @@@@ @@@@  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		sub			ebx, u_p_2;
		sub			esi, v_p_2;
		movlps      xmm2, dword ptr [ebx];
		movlps      xmm3, dword ptr [esi];
		add			ebx, u_p_2;
		add			esi, v_p_2;
		punpcklbw   xmm2, xmm0;         // UU_P
		punpcklbw   xmm3, xmm0;         // VV_P
		punpcklbw   xmm6, xmm0;         // UU_C
		punpcklbw   xmm7, xmm0;         // VV_C
		// 结果为 UU_P * 0.375 + UU_C * 0.625
		paddw       xmm2, xmm6;		// P + C
		paddw       xmm3, xmm7;
		paddw       xmm6, xmm6;		// C * 2
		paddw       xmm7, xmm7;
		paddw       xmm6, xmm2;		// P + C * 3
		paddw       xmm7, xmm3;
		paddw       xmm2, xmm2;		// P * 2 + C * 2
		paddw       xmm3, xmm3;
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm4;
		paddw       xmm2, xmm6;		// P * 3 + C * 5
		paddw       xmm3, xmm7;
		psrlw       xmm2, 0x03;
		psllw       xmm3, 0x05;
		movaps		xmm7, xmm1;
		andps		xmm3, xmm5;
		orps		xmm3, xmm2;
		punpcklbw   xmm1, xmm3;
		punpckhbw   xmm7, xmm3;
		movups		dword ptr [edi], xmm1;
		movups		dword ptr [edi + 0x10], xmm7;
		add         eax, 0x10;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edi, 0x20;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		movlps		xmm1, dword ptr [eax];        // @@@@ @@@@  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm6, dword ptr [ebx];        // 0000 0000  0000  UU6 UU4 UU2 UU0
		movss       xmm7, dword ptr [esi];        // 0000 0000  0000  VV6 VV4 VV2 VV0
		sub			ebx, u_p_2;
		sub			esi, v_p_2;
		movss       xmm2, dword ptr [ebx];
		movss       xmm3, dword ptr [esi];
		add			ebx, u_p_2;
		add			esi, v_p_2;
		punpcklbw	xmm2, xmm3;		// VV_P  UU_P  相间
		punpcklbw	xmm6, xmm7;		// VV_C  UU_C  相间
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P  相间
		punpcklbw   xmm6, xmm0;     // VV_C  UU_C  相间
		// 结果为 UU_P * 0.375 + UU_C * 0.625
		paddw       xmm2, xmm6;		// P + C
		paddw       xmm6, xmm6;		// C * 2
		paddw       xmm6, xmm2;		// P + C * 3
		paddw       xmm2, xmm2;		// P * 2 + C * 2
		paddw       xmm6, xmm4;
		paddw       xmm2, xmm6;		// P * 3 + C * 5
		psrlw       xmm2, 0x03;
		packuswb	xmm2, xmm2;
		punpcklbw   xmm1, xmm2;
		movups		dword ptr [edi], xmm1;
		add         eax, 0x08;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edi, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		movss		xmm1, dword ptr [eax];        // 0000  0000  0000  Y3 Y2 Y1 Y0
		pinsrw		xmm6, word ptr [ebx], 0x00;   // @@@@  @@@@  @@@@  @@ UU2 UU0
		pinsrw		xmm6, word ptr [esi], 0x02;   // @@@@  @@@@  @@@@  VV2 VV0 UU2 UU0
		sub			ebx, u_p_2;
		sub			esi, v_p_2;
		pinsrw		xmm2, word ptr [ebx], 0x00;
		pinsrw		xmm2, word ptr [esi], 0x02;
		add			ebx, u_p_2;
		add			esi, v_p_2;
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P  相间
		punpcklbw   xmm6, xmm0;     // VV_C  UU_C  相间
		// 结果为 UU_P * 0.125 + UU_C * 0.875
		paddw       xmm2, xmm6;		// P + C
		paddw       xmm6, xmm6;		// C * 2
		paddw       xmm6, xmm2;		// P + C * 3
		paddw       xmm2, xmm2;		// P * 2 + C * 2
		paddw       xmm6, xmm4;
		paddw       xmm2, xmm6;		// P * 3 + C * 5
		movhlps		xmm3, xmm2;
		psrlw       xmm2, 0x03;
		psllw       xmm3, 0x05;
		andps		xmm3, xmm5;
		orps		xmm3, xmm2;
		punpcklbw   xmm1, xmm3;
		movlps		dword ptr [edi], xmm1;
		add         eax, 0x04;
		add         ebx, 0x02;
		add         esi, 0x02;
		add         edi, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		pinsrw      xmm1, word ptr [eax], 0x00;        // @@@@ @@@@ @@@@  @@ Y1 Y0
		movzx		edx, byte ptr [ebx];	// UU_C
		movzx		ecx, byte ptr [esi];	// VV_C
		sub			ebx, u_p_2;
		sub			esi, v_p_2;
		pinsrw      xmm2, edx, 0x00;        // UU_C
		pinsrw      xmm2, ecx, 0x01;        // VV_C
		movzx		edx, byte ptr [ebx];	// UU_P
		movzx		ecx, byte ptr [esi];	// VV_P
		pinsrw      xmm6, edx, 0x00;        // UU_P
		pinsrw      xmm6, ecx, 0x01;        // VV_P
		// 结果为 UU_P * 0.125 + UU_C * 0.875
		paddw       xmm6, xmm2;		// P + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm2, xmm6;		// P + C * 3
		paddw		xmm6, xmm6;		// P * 2 + C * 2
		paddw       xmm2, xmm4;
		paddw       xmm6, xmm2;		// P * 3 + C * 5
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV 相间
		punpcklbw   xmm1, xmm6;
		movss		dword ptr [edi], xmm1;
loop_end:
	}
}
// XDCAM 插值方式：422->420 两行插值出一行，第一场公式为 F1_Out = 0.75 * F1_0 + 0.25 * F1_1
void yuyv_to_yuv420_xdcam_F_line_2(OUT BYTE* y,         // 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
								   IN int y_pitch,		// 第二行 y 输出与第一行的偏移
								   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
								   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
								   IN BYTE* yuyv,		// 第一行 yuyv 输入，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
								   IN int yuyv_pitch,	// 第二行 yuyv 输入与第一行的偏移
								   IN int na8,          // 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
								   IN int nm8)
{
	__asm {
		mov         ecx, na8;
		mov         edi, y;
		mov         eax, u;
		mov         ebx, v;
		mov         esi, yuyv;
		mov         edx, yuyv_pitch;
		movaps		xmm0, gc_nRoundComp_2;
		movaps		xmm7, gc_dwMaskWLB;
		test        ecx, ecx;
		jnz         loop_a8;
		jmp         loop_m4;
loop_a8:
		movups		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movups		xmm2, dword ptr [esi + edx];	// 第二行 V0 Y1 U0 Y0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// V6 U6  V4 U4  V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm2, xmm1;
		paddw		xmm1, xmm1;
		paddw		xmm2, xmm0;				// 补偿
		paddw		xmm2, xmm1;
		psrlw		xmm2, 0x02;				// 放大了 4 倍
		packuswb	xmm3, xmm4;				// 第二行 Y    第一行 Y
		packuswb	xmm2, xmm2;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm1, xmm2;
		pand		xmm1, xmm7;				// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm2, 0x08;				// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm1, xmm2;				// V6V4V2V0  V6V4V2V0  U6U4U2U0  U6U4U2U0
		movhlps		xmm2, xmm1;
		mov			edx, y_pitch;
		movlps		dword ptr [edi], xmm3;	// 第一行 Y
		movhps		dword ptr [edi + edx], xmm3;	// 第二行 Y
		movd		dword ptr [eax], xmm1;	// U
		movd		dword ptr [ebx], xmm2;	// V
		mov         edx, yuyv_pitch;
		add			esi, 0x10;
		add			edi, 0x08;
		add			eax, 0x04;
		add			ebx, 0x04;
		sub			ecx, 0x01;
		jnz			loop_a8;
loop_m4:
		mov			ecx, nm8;
		test		ecx, 0x04;
		jz			loop_m2;
		movlps		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movlps		xmm2, dword ptr [esi + edx];	// 第二行 V0 Y1 U0 Y0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm2, xmm1;
		paddw		xmm1, xmm1;
		paddw		xmm2, xmm0;				// 补偿
		paddw		xmm2, xmm1;
		psrlw		xmm2, 0x02;				// 放大了 4 倍
		packuswb	xmm3, xmm4;				// @  Y3Y2Y1Y0  @  Y3Y2Y1Y0
		packuswb	xmm2, xmm2;				// @  V2U2V0U0  @  V2U2V0U0
		movhlps		xmm4, xmm3;
		movaps		xmm1, xmm2;
		pand		xmm1, xmm7;				// @  0 U2 0 U0  @  0 U2 0 U0
		psrlw		xmm2, 0x08;				// @  0 V2 0 V0  @  0 V2 0 V0
		packuswb	xmm1, xmm2;				// @@V2V0  @@V2V0  @@U2U0  @@U2U0
		mov			edx, y_pitch;
		push		ecx;
		push		esi;
		pextrw		esi, xmm1, 0x00;
		pextrw		ecx, xmm1, 0x04;
		movd		dword ptr [edi], xmm3;			// 第一行 Y
		movd		dword ptr [edi + edx], xmm4;	// 第二行 Y
		mov			word ptr [eax], si;		// U
		mov			word ptr [ebx], cx;		// V
		pop			esi;
		pop			ecx;
		mov         edx, yuyv_pitch;
		add			esi, 0x08;
		add			edi, 0x04;
		add			eax, 0x02;
		add			ebx, 0x02;
loop_m2:
		test		ecx, 0x02;
		jz			loop_end;
		movd		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movd		xmm2, dword ptr [esi + edx];	// 第二行 V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   @  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   @  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm2, xmm1;
		paddw		xmm1, xmm1;
		paddw		xmm2, xmm0;				// 补偿
		paddw		xmm2, xmm1;
		psrlw		xmm2, 0x01;				// 放在了 4 倍
		packuswb	xmm3, xmm4;				// @  @ Y1Y0  @  @ Y1Y0
		packuswb	xmm2, xmm2;				// @  @ V0U0  @  @ V0U0
		pextrw		ecx, xmm3, 0x00;		// 第一行 Y
		pextrw		esi, xmm3, 0x04;		// 第二行 Y
		mov			edx, y_pitch;
		mov			word ptr [edi], cx;			// 第一行 Y
		mov			word ptr [edi + edx], si;	// 第二行 Y
		pextrw		ecx, xmm2, 0x00;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], ch;		// V
loop_end:
	}
}
// XDCAM 插值方式：422->420 两行插值出一行，第二场公式为 F2_Out = 0.25 * F2_0 + 3 * F2_1
void yuyv_to_yuv420_xdcam_S_line_2(OUT BYTE* y,         // 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
								   IN int y_pitch,		// 第二行 y 输出与第一行的偏移
								   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
								   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
								   IN BYTE* yuyv,		// 第一行 yuyv 输入，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
								   IN int yuyv_pitch,	// 第二行 yuyv 输入与第一行的偏移
								   IN int na8,          // 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
								   IN int nm8)
{
	__asm {
		mov         ecx, na8;
		mov         edi, y;
		mov         eax, u;
		mov         ebx, v;
		mov         esi, yuyv;
		mov         edx, yuyv_pitch;
		movaps		xmm0, gc_nRoundComp_2;
		movaps		xmm7, gc_dwMaskWLB;
		test        ecx, ecx;
		jnz         loop_a8;
		jmp         loop_m4;
loop_a8:
		movups		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movups		xmm2, dword ptr [esi + edx];	// 第二行 V0 Y1 U0 Y0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// V6 U6  V4 U4  V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm2;
		paddw		xmm2, xmm2;
		paddw		xmm1, xmm0;				// 补偿
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x02;				// 放大了 4 倍
		packuswb	xmm3, xmm4;				// 第二行 Y    第一行 Y
		packuswb	xmm1, xmm1;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm2, xmm1;
		pand		xmm1, xmm7;				// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm2, 0x08;				// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm1, xmm2;				// V6V4V2V0  V6V4V2V0  U6U4U2U0  U6U4U2U0
		movhlps		xmm2, xmm1;
		mov			edx, y_pitch;
		movlps		dword ptr [edi], xmm3;	// 第一行 Y
		movhps		dword ptr [edi + edx], xmm3;	// 第二行 Y
		movd		dword ptr [eax], xmm1;	// U
		movd		dword ptr [ebx], xmm2;	// V
		mov         edx, yuyv_pitch;
		add			esi, 0x10;
		add			edi, 0x08;
		add			eax, 0x04;
		add			ebx, 0x04;
		sub			ecx, 0x01;
		jnz			loop_a8;
loop_m4:
		mov			ecx, nm8;
		test		ecx, 0x04;
		jz			loop_m2;
		movlps		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movlps		xmm2, dword ptr [esi + edx];	// 第二行 V0 Y1 U0 Y0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm2;
		paddw		xmm2, xmm2;
		paddw		xmm1, xmm0;				// 补偿
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x02;				// 放大了 4 倍
		packuswb	xmm3, xmm4;				// @  Y3Y2Y1Y0  @  Y3Y2Y1Y0
		packuswb	xmm1, xmm1;				// @  V2U2V0U0  @  V2U2V0U0
		movhlps		xmm4, xmm3;
		movaps		xmm2, xmm1;
		pand		xmm1, xmm7;				// @  0 U2 0 U0  @  0 U2 0 U0
		psrlw		xmm2, 0x08;				// @  0 V2 0 V0  @  0 V2 0 V0
		packuswb	xmm1, xmm2;				// @@V2V0  @@V2V0  @@U2U0  @@U2U0
		mov			edx, y_pitch;
		push		ecx;
		push		esi;
		pextrw		esi, xmm1, 0x00;
		pextrw		ecx, xmm1, 0x04;
		movd		dword ptr [edi], xmm3;			// 第一行 Y
		movd		dword ptr [edi + edx], xmm4;	// 第二行 Y
		mov			word ptr [eax], si;		// U
		mov			word ptr [ebx], cx;		// V
		pop			esi;
		pop			ecx;
		mov         edx, yuyv_pitch;
		add			esi, 0x08;
		add			edi, 0x04;
		add			eax, 0x02;
		add			ebx, 0x02;
loop_m2:
		test		ecx, 0x02;
		jz			loop_end;
		movd		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movd		xmm2, dword ptr [esi + edx];	// 第二行 V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   @  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   @  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm2;
		paddw		xmm2, xmm2;
		paddw		xmm1, xmm0;				// 补偿
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x01;				// 放在了 4 倍
		packuswb	xmm3, xmm4;				// @  @ Y1Y0  @  @ Y1Y0
		packuswb	xmm1, xmm1;				// @  @ V0U0  @  @ V0U0
		pextrw		ecx, xmm3, 0x00;		// 第一行 Y
		pextrw		esi, xmm3, 0x04;		// 第二行 Y
		mov			edx, y_pitch;
		mov			word ptr [edi], cx;			// 第一行 Y
		mov			word ptr [edi + edx], si;	// 第二行 Y
		pextrw		ecx, xmm1, 0x00;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], ch;		// V
loop_end:
	}
}

void yuv422_to_yuyv_line(OUT BYTE* yuyv,        // yuyv 输出，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						 IN BYTE* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
						 IN BYTE* u,            // u 输入，存放方式 U0 U2 U4 U6 ……
						 IN BYTE* v,            // v 输入，存放方式 V0 V2 V4 V6 ……
						 IN int na16,           // 一行像素宽度除以 16 的商与余数（注意目标为 16 位的缓存）
						 IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, yuyv;
		mov         edx, yuyv;
		mov         esi, y;
		mov         eax, u;
		mov         ebx, v;
		add         edx, 0x10;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		movdqu      xmm0, [esi];    // Y15 Y14 Y13 Y12  Y11 Y10 Y9 Y8  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movsd       xmm1, [eax];    // 0000 0000  U14 U12 U10 U8  U6 U4 U2 U0
		movsd       xmm2, [ebx];    // 0000 0000  V14 V12 V10 V8  V6 V4 V2 V0
		punpcklbw   xmm1, xmm2;     // V14 U14 V12 U12  V10 U10 V8 U8  V6 U6 V4 U4  V2 U2 V0 U0
		movdqa      xmm3, xmm0;
		punpcklbw   xmm0, xmm1;     // V6 Y7 U6 Y6  V4 Y5 U4 Y4  V2 Y3 U2 Y2  V0 Y1 U0 Y0
		punpckhbw   xmm3, xmm1;     // V14 Y15 U14 Y14  V12 Y13 U12 Y12  V10 Y11 U10 Y10  V8 Y9 U8 Y8
		movdqu      [edi], xmm0;
		movdqu      [edx], xmm3;
		add         esi, 0x10;
		add         eax, 0x08;
		add         ebx, 0x08;
		add         edi, 0x20;
		add         edx, 0x20;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		movsd       xmm0, [esi];    // 0000 0000  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm1, [eax];    // 0000 0000 0000  U6 U4 U2 U0
		movss       xmm2, [ebx];    // 0000 0000 0000  V6 V4 V2 V0
		punpcklbw   xmm1, xmm2;     // 0000 0000  V6 U6 V4 U4  V2 U2 V0 U0
		punpcklbw   xmm0, xmm1;     // V6 Y7 U6 Y6  V4 Y5 U4 Y4  V2 Y3 U2 Y2  V0 Y1 U0 Y0
		movdqu      [edi], xmm0;
		add         esi, 0x08;
		add         eax, 0x04;
		add         ebx, 0x04;
		add         edi, 0x10;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		movss       xmm0, [esi];    // 0000 0000 0000  Y3 Y2 Y1 Y0
		pxor        xmm1, xmm1;
		pxor        xmm2, xmm2;
		pinsrw      xmm1, [eax], 0x00;      // U2 U0
		pinsrw      xmm2, [ebx], 0x00;      // V2 V0
		punpcklbw   xmm1, xmm2;     // 0000 0000 0000  V2 U2 V0 U0
		punpcklbw   xmm0, xmm1;     // 0000 0000  V2 Y3 U2 Y2  V0 Y1 U0 Y0
		movsd       [edi], xmm0;
		add         esi, 0x04;
		add         eax, 0x02;
		add         ebx, 0x02;
		add         edi, 0x08;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		xor         edx, edx;
		xor         ecx, ecx;
		mov         dh, [ebx];      // V0
		mov         cx, [esi];
		shl         edx, 0x10;      // V0 0 0 0
		mov         ebx, ecx;       // 0 0 Y1 Y0
		mov         dh, [eax];      // V0 0 U0 0
		and         ecx, 0xff00;    // 0 0 Y1 0
		and         ebx, 0x00ff;    // 0 0 0 Y0
		shl         ecx, 0x08;      // 0 Y1 0 0
		or          edx, ebx;
		or          edx, ecx;
		mov         [edi], edx;
loop_end:
	}
}

void yuyv_to_yuv420_line_2_avr(OUT BYTE* y,         // 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
							   IN int y_pitch,		// 第二行 y 输出与第一行的偏移
							   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
							   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
							   IN BYTE* yuyv,		// 第一行 yuyv 输入，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							   IN int yuyv_pitch,	// 第二行 yuyv 输入与第一行的偏移
							   IN int na8,          // 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
							   IN int nm8)
{
	__asm {
		mov         ecx, na8;
		mov         edi, y;
		mov         eax, u;
		mov         ebx, v;
		mov         esi, yuyv;
		mov         edx, yuyv_pitch;
		movdqa		xmm0, gc_dwMaskWBIT;
		movdqa		xmm7, gc_dwMaskWLB;
		test        ecx, ecx;
		jnz         loop_a8;
		jmp         loop_m4;
loop_a8:
		movdqu		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movdqu		xmm2, dword ptr [esi + edx];	// 第二行 V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// V6 U6  V4 U4  V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm0;				// 补偿
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x01;				// 平均值
		packuswb	xmm1, xmm1;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		packuswb	xmm3, xmm4;				// 第二行 Y    第一行 Y
		movdqa		xmm2, xmm1;
		pand		xmm1, xmm7;				// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm2, 0x08;				// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm1, xmm2;				// V6V4V2V0  V6V4V2V0  U6U4U2U0  U6U4U2U0
		movhlps		xmm2, xmm1;
		mov			edx, y_pitch;
		movlps		dword ptr [edi], xmm3;	// 第一行 Y
		movhps		dword ptr [edi + edx], xmm3;	// 第二行 Y
		movd		dword ptr [eax], xmm1;	// U
		movd		dword ptr [ebx], xmm2;	// V
		mov         edx, yuyv_pitch;
		add			esi, 0x10;
		add			edi, 0x08;
		add			eax, 0x04;
		add			ebx, 0x04;
		sub			ecx, 0x01;
		jnz			loop_a8;
loop_m4:
		mov			ecx, nm8;
		test		ecx, 0x04;
		jz			loop_m2;
		movlps		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movlps		xmm2, dword ptr [esi + edx];	// 第二行 V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm0;				// 补偿
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x01;				// 平均值
		packuswb	xmm1, xmm1;				// @  V2U2V0U0  @  V2U2V0U0
		packuswb	xmm3, xmm4;				// @  Y3Y2Y1Y0  @  Y3Y2Y1Y0
		movdqa		xmm2, xmm1;
		movhlps		xmm4, xmm3;
		pand		xmm1, xmm7;				// @  0 U2 0 U0  @  0 U2 0 U0
		psrlw		xmm2, 0x08;				// @  0 V2 0 V0  @  0 V2 0 V0
		packuswb	xmm1, xmm2;				// @@V2V0  @@V2V0  @@U2U0  @@U2U0
		mov			edx, y_pitch;
		push		ecx;
		push		esi;
		pextrw		esi, xmm1, 0x00;
		pextrw		ecx, xmm1, 0x04;
		movd		dword ptr [edi], xmm3;			// 第一行 Y
		movd		dword ptr [edi + edx], xmm4;	// 第二行 Y
		mov			word ptr [eax], si;		// U
		mov			word ptr [ebx], cx;		// V
		pop			esi;
		pop			ecx;
		mov         edx, yuyv_pitch;
		add			esi, 0x08;
		add			edi, 0x04;
		add			eax, 0x02;
		add			ebx, 0x02;
loop_m2:
		test		ecx, 0x02;
		jz			loop_end;
		movd		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movd		xmm2, dword ptr [esi + edx];	// 第二行 V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   @  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   @  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm0;				// 补偿
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x01;				// 平均值
		packuswb	xmm3, xmm4;				// @  @ Y1Y0  @  @ Y1Y0
		packuswb	xmm1, xmm1;				// @  @ V0U0  @  @ V0U0
		pextrw		ecx, xmm3, 0x00;		// 第一行 Y
		pextrw		esi, xmm3, 0x04;		// 第二行 Y
		mov			edx, y_pitch;
		mov			word ptr [edi], cx;			// 第一行 Y
		mov			word ptr [edi + edx], si;	// 第二行 Y
		pextrw		ecx, xmm1, 0x00;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], ch;		// V
loop_end:
	}
}
void yuyv_to_yuv420_line_1(OUT BYTE* y,         // 一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
						   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
						   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
						   IN BYTE* yuyv,		// 一行 yuyv 输入，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN int na8,	        // 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
						   IN int nm8)
{
	__asm {
		mov         ecx, na8;
		mov         edi, y;
		mov         eax, u;
		mov         edx, v;
		mov         esi, yuyv;
		movdqa		xmm7, gc_dwMaskWLB;
		test        ecx, ecx;
		jnz         loop_a8;
		jmp         loop_m4;
loop_a8:
		movdqu		xmm1, dword ptr [esi];		// V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		pand		xmm1, xmm7;				// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		psrlw		xmm3, 0x08;				// V6 U6  V4 U4  V2 U2  V0 U0
		packuswb	xmm3, xmm1;				// Y  V6U6V4U4  V2U2V0U0
		movdqa		xmm2, xmm3;
		movdqa		xmm1, xmm3;
		psrlw		xmm2, 0x08;				// @  @  0 V6 0 V4  0 V2 0 V0
		pand		xmm1, xmm7;				// @  @  0 U6 0 U4  0 U2 0 U0
		packuswb	xmm1, xmm2;				// @  V6V4V2V0  @  U6U4U2U0
		movhlps		xmm2, xmm1;
		movhps		dword ptr [edi], xmm3;	// Y
		movd		dword ptr [eax], xmm1;	// U
		movd		dword ptr [edx], xmm2;	// V
		add			esi, 0x10;
		add			edi, 0x08;
		add			eax, 0x04;
		add			edx, 0x04;
		sub			ecx, 0x01;
		jnz			loop_a8;
loop_m4:
		mov			ecx, nm8;
		test		ecx, 0x04;
		jz			loop_m2;
		movlps		xmm1, dword ptr [esi];	// V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		pand		xmm1, xmm7;				// @  @   Y3 Y2  Y1 Y0
		psrlw		xmm3, 0x08;				// @  @   V2 U2  V0 U0
		packuswb	xmm3, xmm1;				// @  Y3Y2Y1Y0  @  V2U2V0U0
		movdqa		xmm2, xmm3;
		movhlps		xmm1, xmm3;
		psrlw		xmm3, 0x08;				// @  @  @  0 V2 0 V0
		pand		xmm2, xmm7;				// @  @  @  0 U2 0 U0
		packuswb	xmm2, xmm3;				// @  @@V2V0  @  @@U2U0
		push		ecx;
		pextrw		ecx, xmm1, 0x00;
		pextrw		ebx, xmm1, 0x04;
		movd		dword ptr [edi], xmm1;	// Y
		mov			word ptr [eax], cx;		// U
		mov			word ptr [edx], bx;		// V
		pop			ecx;
		add			esi, 0x08;
		add			edi, 0x04;
		add			eax, 0x02;
		add			edx, 0x02;
loop_m2:
		test		ecx, 0x02;
		jz			loop_end;
		movd		xmm1, dword ptr [esi];	// V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		pand		xmm1, xmm7;				// @  @   @  Y1 Y0
		psrlw		xmm3, 0x08;				// @  @   @  V0 U0
		packuswb	xmm3, xmm1;				// @  @ Y1Y0  @  @ V0U0
		pextrw		esi, xmm3, 0x04;		// Y
		pextrw		ecx, xmm3, 0x00;		// VU
		mov			word ptr [edi], si;		// Y
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [edx], ch;		// V
loop_end:
	}
}
// 采用场内四行 -1, 3, 3, -1 的系数对 UV 进行采样
// 第一行
void yuyv_to_yuv420_line_4_F(OUT BYTE* y,			// 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
							 IN int y_pitch,		// 第二行 y 输出与第一行的偏移
							 OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
							 OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
							 IN BYTE* yuyv1,		// 第一行 yuyv 输入（权重 -1+3），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN BYTE* yuyv2,		// 第二行 yuyv 输入（权重 3），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN int yuyv_pitch,		// 两行 yuyv 输入间的偏移
							 IN int na8,			// 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
							 IN int nm8)
{
	__asm {
		mov         ecx, na8;
		mov         edi, y;
		mov         eax, u;
		mov         ebx, v;
		mov         esi, yuyv1;
		mov         edx, yuyv2;
		movaps		xmm0, gc_dwMaskWLB;
		movaps		xmm7, gc_dwMaskWHB;
		test        ecx, ecx;
		jnz         loop_a8;
		jmp         loop_m4;
loop_a8:
		push		ecx;
		mov			ecx, yuyv_pitch;
		movups		xmm1, dword ptr [esi];			// 第一行 V0 Y1 U0 Y0
		movups		xmm2, dword ptr [edx];			// 第二行 V0 Y1 U0 Y0
		movups		xmm3, dword ptr [edx + ecx];	// 下一行 V0 Y1 U0 Y0
		movaps		xmm5, xmm1;
		movaps		xmm4, xmm2;
		movaps		xmm6, xmm2;
		pand		xmm1, xmm0;
		pand		xmm2, xmm0;		// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm5, xmm7;
		pand		xmm4, xmm7;		// V6 U6  V4 U4  V2 U2  V0 U0（放大 8 位）
		psrlw		xmm3, 0x08;		// V6 U6  V4 U4  V2 U2  V0 U0（-1 权重）
		psrlw		xmm6, 0x08;		// 3 权重
		psrlw		xmm5, 0x07;		// 放大 1 位
		psrlw		xmm4, 0x07;
		psubw		xmm6, xmm3;
		paddw		xmm4, xmm5;
		paddw		xmm6, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm6, xmm4;
		psrlw		xmm6, 0x02;				// VV6 UU6 VV4 UU4 VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm6, xmm4;
		packuswb	xmm1, xmm2;				// 第二行 Y    第一行 Y
		packuswb	xmm6, xmm6;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm5, xmm6;
		pand		xmm6, xmm0;		// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm5, 0x08;		// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm5, xmm6;		// U6U4U2U0  U6U4U2U0  V6V4V2V0  V6V4V2V0
		movhlps		xmm4, xmm5;
		mov			ecx, y_pitch;
		movlps		dword ptr [edi], xmm1;			// 第一行 Y
		movhps		dword ptr [edi + ecx], xmm1;	// 第二行 Y
		movd		dword ptr [eax], xmm4;	// U
		movd		dword ptr [ebx], xmm5;	// V
		pop			ecx;
		add			esi, 0x10;
		add			edx, 0x10;
		add			edi, 0x08;
		add			eax, 0x04;
		add			ebx, 0x04;
		sub			ecx, 0x01;
		jnz			loop_a8;
loop_m4:
		mov			ecx, nm8;
		test		ecx, 0x04;
		jz			loop_m2;
		push		ecx;
		mov			ecx, yuyv_pitch;
		movlps		xmm1, dword ptr [esi];			// @  第一行
		movhps		xmm1, dword ptr [edx];			// 第二行  第一行
		movlps		xmm4, dword ptr [edx + ecx];	// @  下一行
		movlhps		xmm4, xmm1;						// 第一行  下一行
		movaps		xmm2, xmm1;
		movaps		xmm3, xmm1;
		pand		xmm1, xmm0;		// 第二行 Y  第一行 Y
		pand		xmm2, xmm7;		// 第二行 UV  第一行 UV（放大 8 位）
		psrlw		xmm4, 0x08;		// 上一行 UV  下一行 UV（-1 权重）
		psrlw		xmm3, 0x08;		// 第二行 UV  第一行 UV（3 权重）
		psrlw		xmm2, 0x07;		// 放大 1 位
		psubw		xmm4, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm2, xmm3;
		psubw		xmm2, xmm4;
		movhlps		xmm3, xmm2;
		paddw		xmm2, xmm3;
		psrlw		xmm2, 0x02;				// @  VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm2, xmm4;
		packuswb	xmm1, xmm2;				// @  V2U2V0U0  第二行 Y  第一行 Y
		movhlps		xmm2, xmm1;
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		shufps		xmm1, xmm1, 0x01;		// @  @  @  第二行 Y
		pand		xmm2, xmm0;				// @  U2 U0  @  U2 U0
		psrlw		xmm3, 0x08;				// @  V2 V0  @  V2 V0
		packuswb	xmm3, xmm2;				// @ U2U0  @ U2V0  @ V2V0  @ V2V0
		push		esi;
		push		edx;
		mov			ecx, y_pitch;
		pextrw		esi, xmm3, 0x04;
		pextrw		edx, xmm3, 0x00;
		movd		dword ptr [edi], xmm0;			// 第一行 Y
		movd		dword ptr [edi + ecx], xmm1;	// 第二行 Y
		mov			word ptr [eax], si;		// U
		mov			word ptr [ebx], dx;		// V
		pop			ecx;
		pop			esi;
		pop			edx;
		add			esi, 0x08;
		add			edx, 0x08;
		add			edi, 0x04;
		add			eax, 0x02;
		add			ebx, 0x02;
loop_m2:
		test		ecx, 0x02;
		jz			loop_end;
		mov			ecx, yuyv_pitch;
		movd		xmm1, dword ptr [esi];	// 第一行 V0 Y1 U0 Y0
		movd		xmm2, dword ptr [edx];	// 第二行 V0 Y1 U0 Y0
		movd		xmm3, dword ptr [edx + ecx];	// 下一行 V0 Y1 U0 Y0
		movlhps		xmm1, xmm2;			// 第二行  第一行
		movlhps		xmm3, xmm1;			// 第一行  下一行
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm1;
		pand		xmm1, xmm0;		// 第二行 Y  第一行 Y
		pand		xmm4, xmm7;		// 第二行 UV  第一行 UV（放大 8 位）
		psrlw		xmm3, 0x08;				// 上一行 UV  下一行 UV（-1 权重）
		psrlw		xmm5, 0x08;				// 第二行 UV  第一行 UV（3 权重）
		psrlw		xmm4, 0x07;				// 放大 1 位
		psubw		xmm3, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm4, xmm5;
		psubw		xmm4, xmm3;
		movhlps		xmm5, xmm4;
		paddw		xmm4, xmm5;
		packuswb	xmm1, xmm1;				// 0 第二行Y  0 第一行Y  0 第二行Y  0 第一行Y
		psrlw		xmm4, 0x02;				// @  @  @  VV0 UU0
		pxor		xmm3, xmm3;
		pmaxsw		xmm4, xmm3;
		pextrw		esi, xmm1, 0x00;
		pextrw		edx, xmm1, 0x02;
		mov			ecx, y_pitch;
		mov			word ptr [edi], si;			// 第一行 Y
		mov			word ptr [edi + ecx], dx;	// 第二行 Y
		pextrw		ecx, xmm4, 0x00;
		pextrw		edx, xmm4, 0x01;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], dl;		// V
loop_end:
	}
}
// 中间行
void yuyv_to_yuv420_line_4(OUT BYTE* y,			// 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
						   IN int y_pitch,		// 第二行 y 输出与第一行的偏移
						   OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
						   OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
						   IN BYTE* yuyv0,		// 上一行 yuyv 输入（权重 -1），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN BYTE* yuyv2,		// 第二行 yuyv 输入（权重 3），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN int yuyv_pitch,	// 两行 yuyv 输入间的偏移
						   IN int na8,			// 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
						   IN int nm8)
{
	__asm {
		mov         ecx, na8;
		mov         edi, y;
		mov         eax, u;
		mov         ebx, v;
		mov         esi, yuyv0;
		mov         edx, yuyv2;
		test        ecx, ecx;
		jnz         loop_a8;
		jmp         loop_m4;
loop_a8:
		push		ecx;
		mov			ecx, yuyv_pitch;
		movups		xmm0, dword ptr [esi];			// 上一行 V0 Y1 U0 Y0
		movups		xmm2, dword ptr [edx];			// 第二行 V0 Y1 U0 Y0
		movups		xmm1, dword ptr [esi + ecx];	// 第一行 V0 Y1 U0 Y0
		movups		xmm3, dword ptr [edx + ecx];	// 下一行 V0 Y1 U0 Y0
		movaps		xmm4, xmm2;
		movaps		xmm6, xmm2;
		movaps		xmm5, xmm1;
		movaps		xmm7, xmm1;
		pand		xmm2, gc_dwMaskWLB;		// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm1, gc_dwMaskWLB;
		pand		xmm4, gc_dwMaskWHB;		// V6 U6  V4 U4  V2 U2  V0 U0（放大 8 位）
		pand		xmm5, gc_dwMaskWHB;
		psrlw		xmm0, 0x08;				// V6 U6  V4 U4  V2 U2  V0 U0（-1 权重）
		psrlw		xmm3, 0x08;
		psrlw		xmm6, 0x08;				// 3 权重
		psrlw		xmm7, 0x08;
		psrlw		xmm4, 0x07;				// 放大 1 位
		psrlw		xmm5, 0x07;
		paddw		xmm0, xmm3;
		paddw		xmm6, xmm7;
		paddw		xmm4, xmm5;
		psubw		xmm6, xmm0;
		paddw		xmm4, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm4, xmm6;
		psrlw		xmm4, 0x02;				// VV6 UU6 VV4 UU4 VV2 UU2 VV0 UU0
		pxor		xmm6, xmm6;
		pmaxsw		xmm4, xmm6;
		packuswb	xmm1, xmm2;				// 第二行 Y    第一行 Y
		packuswb	xmm4, xmm4;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm5, xmm4;
		pand		xmm4, gc_dwMaskWLB;		// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm5, 0x08;				// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm5, xmm4;				// U6U4U2U0  U6U4U2U0  V6V4V2V0  V6V4V2V0
		movhlps		xmm4, xmm5;
		mov			ecx, y_pitch;
		movlps		dword ptr [edi], xmm1;			// 第一行 Y
		movhps		dword ptr [edi + ecx], xmm1;	// 第二行 Y
		movd		dword ptr [eax], xmm4;	// U
		movd		dword ptr [ebx], xmm5;	// V
		pop			ecx;
		add			esi, 0x10;
		add			edx, 0x10;
		add			edi, 0x08;
		add			eax, 0x04;
		add			ebx, 0x04;
		sub			ecx, 0x01;
		jnz			loop_a8;
loop_m4:
		mov			ecx, nm8;
		test		ecx, 0x04;
		jz			loop_m2;
		push		ecx;
		mov			ecx, yuyv_pitch;
		movhps		xmm0, dword ptr [esi];			// 上一行  @
		movhps		xmm1, dword ptr [edx];			// 第二行  @
		movlps		xmm1, dword ptr [esi + ecx];	// 第二行  第一行
		movlps		xmm0, dword ptr [edx + ecx];	// 上一行  下一行
		movaps		xmm2, xmm1;
		movaps		xmm3, xmm1;
		pand		xmm1, gc_dwMaskWLB;		// 第二行 Y  第一行 Y
		pand		xmm2, gc_dwMaskWHB;		// 第二行 UV  第一行 UV（放大 8 位）
		psrlw		xmm0, 0x08;				// 上一行 UV  下一行 UV（-1 权重）
		psrlw		xmm3, 0x08;				// 第二行 UV  第一行 UV（3 权重）
		psrlw		xmm2, 0x07;				// 放大 1 位
		psubw		xmm0, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm2, xmm3;
		psubw		xmm2, xmm0;
		movhlps		xmm3, xmm2;
		paddw		xmm2, xmm3;
		psrlw		xmm2, 0x02;				// @  VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm2, xmm4;
		packuswb	xmm1, xmm2;				// @  V2U2V0U0  第二行 Y  第一行 Y
		movhlps		xmm2, xmm1;
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		shufps		xmm1, xmm1, 0x01;		// @  @  @  第二行 Y
		pand		xmm2, gc_dwMaskWLB;		// @  U2 U0  @  U2 U0
		psrlw		xmm3, 0x08;				// @  V2 V0  @  V2 V0
		packuswb	xmm3, xmm2;				// @ U2U0  @ U2V0  @ V2V0  @ V2V0
		push		esi;
		push		edx;
		mov			ecx, y_pitch;
		pextrw		esi, xmm3, 0x04;
		pextrw		edx, xmm3, 0x00;
		movd		dword ptr [edi], xmm0;			// 第一行 Y
		movd		dword ptr [edi + ecx], xmm1;	// 第二行 Y
		mov			word ptr [eax], si;		// U
		mov			word ptr [ebx], dx;		// V
		pop			ecx;
		pop			esi;
		pop			edx;
		add			esi, 0x08;
		add			edx, 0x08;
		add			edi, 0x04;
		add			eax, 0x02;
		add			ebx, 0x02;
loop_m2:
		test		ecx, 0x02;
		jz			loop_end;
		mov			ecx, yuyv_pitch;
		movd		xmm0, dword ptr [esi];	// 上一行 V0 Y1 U0 Y0
		movd		xmm2, dword ptr [edx];	// 第二行 V0 Y1 U0 Y0
		movd		xmm1, dword ptr [esi + ecx];	// 第一行 V0 Y1 U0 Y0
		movd		xmm3, dword ptr [edx + ecx];	// 下一行 V0 Y1 U0 Y0
		movlhps		xmm1, xmm2;			// 第二行  第一行
		movlhps		xmm0, xmm3;			// 上一行  下一行
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm1;
		pand		xmm1, gc_dwMaskWLB;		// 第二行 Y  第一行 Y
		pand		xmm4, gc_dwMaskWHB;		// 第二行 UV  第一行 UV（放大 8 位）
		psrlw		xmm0, 0x08;				// 上一行 UV  下一行 UV（-1 权重）
		psrlw		xmm5, 0x08;				// 第二行 UV  第一行 UV（3 权重）
		psrlw		xmm4, 0x07;				// 放大 1 位
		psubw		xmm0, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm4, xmm5;
		psubw		xmm4, xmm0;
		movhlps		xmm5, xmm4;
		paddw		xmm4, xmm5;
		packuswb	xmm1, xmm1;				// 0 第二行Y  0 第一行Y  0 第二行Y  0 第一行Y
		psrlw		xmm4, 0x02;				// @  @  @  VV0 UU0
		pxor		xmm3, xmm3;
		pmaxsw		xmm4, xmm3;
		pextrw		esi, xmm1, 0x00;
		pextrw		edx, xmm1, 0x02;
		mov			ecx, y_pitch;
		mov			word ptr [edi], si;			// 第一行 Y
		mov			word ptr [edi + ecx], dx;	// 第二行 Y
		pextrw		ecx, xmm4, 0x00;
		pextrw		edx, xmm4, 0x01;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], dl;		// V
loop_end:
	}
}
// 最后一行
void yuyv_to_yuv420_line_4_L(OUT BYTE* y,			// 第一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
							 IN int y_pitch,		// 第二行 y 输出与第一行的偏移
							 OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
							 OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
							 IN BYTE* yuyv0,		// 上一行 yuyv 输入（权重 -1），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN BYTE* yuyv2,		// 第二行 yuyv 输入（权重 3-1），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							 IN int yuyv_pitch,		// 两行 yuyv 输入间的偏移
							 IN int na8,			// 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
							 IN int nm8)
{
	__asm {
		mov         ecx, na8;
		mov         edi, y;
		mov         eax, u;
		mov         ebx, v;
		mov         esi, yuyv0;
		mov         edx, yuyv2;
		movaps		xmm0, gc_dwMaskWLB;
		movaps		xmm7, gc_dwMaskWHB;
		test        ecx, ecx;
		jnz         loop_a8;
		jmp         loop_m4;
loop_a8:
		push		ecx;
		mov			ecx, yuyv_pitch;
		movups		xmm3, dword ptr [esi];			// 上一行 V0 Y1 U0 Y0
		movups		xmm2, dword ptr [edx];			// 第二行 V0 Y1 U0 Y0
		movups		xmm1, dword ptr [esi + ecx];	// 第一行 V0 Y1 U0 Y0
		movaps		xmm5, xmm2;
		movaps		xmm4, xmm1;
		movaps		xmm6, xmm1;
		pand		xmm2, xmm0;		// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm1, xmm0;
		pand		xmm5, xmm7;
		pand		xmm4, xmm7;		// V6 U6  V4 U4  V2 U2  V0 U0（放大 8 位）
		psrlw		xmm3, 0x08;		// V6 U6  V4 U4  V2 U2  V0 U0（-1 权重）
		psrlw		xmm6, 0x08;		// 3 权重
		psrlw		xmm5, 0x07;		// 放大 1 位
		psrlw		xmm4, 0x07;
		psubw		xmm6, xmm3;
		paddw		xmm4, xmm5;
		paddw		xmm6, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm6, xmm4;
		psrlw		xmm6, 0x02;				// VV6 UU6 VV4 UU4 VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm6, xmm4;
		packuswb	xmm1, xmm2;				// 第二行 Y    第一行 Y
		packuswb	xmm6, xmm6;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm5, xmm6;
		pand		xmm6, xmm0;		// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm5, 0x08;		// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm5, xmm6;		// U6U4U2U0  U6U4U2U0  V6V4V2V0  V6V4V2V0
		movhlps		xmm4, xmm5;
		mov			ecx, y_pitch;
		movlps		dword ptr [edi], xmm1;			// 第一行 Y
		movhps		dword ptr [edi + ecx], xmm1;	// 第二行 Y
		movd		dword ptr [eax], xmm4;	// U
		movd		dword ptr [ebx], xmm5;	// V
		pop			ecx;
		add			esi, 0x10;
		add			edx, 0x10;
		add			edi, 0x08;
		add			eax, 0x04;
		add			ebx, 0x04;
		sub			ecx, 0x01;
		jnz			loop_a8;
loop_m4:
		mov			ecx, nm8;
		test		ecx, 0x04;
		jz			loop_m2;
		push		ecx;
		mov			ecx, yuyv_pitch;
		movhps		xmm4, dword ptr [esi];			// 上一行  @
		movhps		xmm1, dword ptr [edx];			// 第二行  @
		movlps		xmm1, dword ptr [esi + ecx];	// 第二行  第一行
		movhlps		xmm4, xmm1;						// 上一行  第二行
		movaps		xmm2, xmm1;
		movaps		xmm3, xmm1;
		pand		xmm1, xmm0;		// 第二行 Y  第一行 Y
		pand		xmm2, xmm7;		// 第二行 UV  第一行 UV（放大 8 位）
		psrlw		xmm4, 0x08;		// 上一行 UV  下一行 UV（-1 权重）
		psrlw		xmm3, 0x08;		// 第二行 UV  第一行 UV（3 权重）
		psrlw		xmm2, 0x07;		// 放大 1 位
		psubw		xmm4, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm2, xmm3;
		psubw		xmm2, xmm4;
		movhlps		xmm3, xmm2;
		paddw		xmm2, xmm3;
		psrlw		xmm2, 0x02;				// @  VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm2, xmm4;
		packuswb	xmm1, xmm2;				// @  V2U2V0U0  第二行 Y  第一行 Y
		movhlps		xmm2, xmm1;
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		shufps		xmm1, xmm1, 0x01;		// @  @  @  第二行 Y
		pand		xmm2, xmm0;				// @  U2 U0  @  U2 U0
		psrlw		xmm3, 0x08;				// @  V2 V0  @  V2 V0
		packuswb	xmm3, xmm2;				// @ U2U0  @ U2V0  @ V2V0  @ V2V0
		push		esi;
		push		edx;
		mov			ecx, y_pitch;
		pextrw		esi, xmm3, 0x04;
		pextrw		edx, xmm3, 0x00;
		movd		dword ptr [edi], xmm0;			// 第一行 Y
		movd		dword ptr [edi + ecx], xmm1;	// 第二行 Y
		mov			word ptr [eax], si;		// U
		mov			word ptr [ebx], dx;		// V
		pop			ecx;
		pop			esi;
		pop			edx;
		add			esi, 0x08;
		add			edx, 0x08;
		add			edi, 0x04;
		add			eax, 0x02;
		add			ebx, 0x02;
loop_m2:
		test		ecx, 0x02;
		jz			loop_end;
		mov			ecx, yuyv_pitch;
		movd		xmm3, dword ptr [esi];	// 上一行 V0 Y1 U0 Y0
		movd		xmm2, dword ptr [edx];	// 第二行 V0 Y1 U0 Y0
		movd		xmm1, dword ptr [edx + ecx];	// 第一行 V0 Y1 U0 Y0
		movlhps		xmm1, xmm2;			// 第二行  第一行
		movlhps		xmm2, xmm3;			// 上一行  第二行
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm1;
		pand		xmm1, xmm0;		// 第二行 Y  第一行 Y
		pand		xmm4, xmm7;		// 第二行 UV  第一行 UV（放大 8 位）
		psrlw		xmm2, 0x08;				// 上一行 UV  下一行 UV（-1 权重）
		psrlw		xmm5, 0x08;				// 第二行 UV  第一行 UV（3 权重）
		psrlw		xmm4, 0x07;				// 放大 1 位
		psubw		xmm2, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm4, xmm5;
		psubw		xmm4, xmm2;
		movhlps		xmm5, xmm4;
		paddw		xmm4, xmm5;
		packuswb	xmm1, xmm1;				// 0 第二行Y  0 第一行Y  0 第二行Y  0 第一行Y
		psrlw		xmm4, 0x02;				// @  @  @  VV0 UU0
		pxor		xmm3, xmm3;
		pmaxsw		xmm4, xmm3;
		pextrw		esi, xmm1, 0x00;
		pextrw		edx, xmm1, 0x02;
		mov			ecx, y_pitch;
		mov			word ptr [edi], si;			// 第一行 Y
		mov			word ptr [edi + ecx], dx;	// 第二行 Y
		pextrw		ecx, xmm4, 0x00;
		pextrw		edx, xmm4, 0x01;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], dl;		// V
loop_end:
	}
}
// 特殊的最后一行，一般不可能出现
void yuyv_to_yuv420_line_4_LS(OUT BYTE* y,			// 一行 y 输出，存放方式 Y0 Y1 Y2 Y3 ……
							  OUT BYTE* u,			// 一行 u 输出，存放方式 UU0 UU2 UU4 UU6 ……
							  OUT BYTE* v,			// 一行 v 输出，存放方式 VV0 VV2 VV4 VV6 ……
							  IN BYTE* yuyv0,		// 上一行 yuyv 输入（权重 -1-1），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							  IN BYTE* yuyv1,		// 第一行 yuyv 输入（权重 3+3），存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							  IN int na8,			// 一行像素宽度除以 8 的商与余数（注意目标为 16 位的缓存）
							  IN int nm8)
{
	__asm {
		mov         ecx, na8;
		mov         edi, y;
		mov         eax, u;
		mov         ebx, v;
		mov         esi, yuyv0;
		mov         edx, yuyv1;
		movaps		xmm0, gc_dwMaskWLB;
		movaps		xmm7, gc_dwMaskWHB;
		movaps		xmm6, gc_dwMaskWBIT;	// 0.5 * 2
		test        ecx, ecx;
		jnz         loop_a8;
		jmp         loop_m4;
loop_a8:
		movups		xmm1, dword ptr [esi];			// 上一行 V0 Y1 U0 Y0
		movups		xmm2, dword ptr [edx];			// 第一行 V0 Y1 U0 Y0
		movaps		xmm3, xmm2;
		movaps		xmm4, xmm2;
		pand		xmm2, xmm0;		// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm3, xmm7;		// V6 U6  V4 U4  V2 U2  V0 U0（放大 8 位）
		psrlw		xmm1, 0x08;		// V6 U6  V4 U4  V2 U2  V0 U0（-1 权重）
		psrlw		xmm4, 0x08;		// 3 权重
		psrlw		xmm3, 0x07;		// 放大 1 位
		psubw		xmm4, xmm1;
		paddw		xmm3, xmm6;
		paddw		xmm3, xmm4;
		psrlw		xmm3, 0x01;		// VV6 UU6 VV4 UU4 VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm3, xmm4;
		packuswb	xmm2, xmm3;		// V6U6V4U4  V2U2V0U0    第一行 Y
		movhlps		xmm3, xmm2;
		movaps		xmm5, xmm3;
		pand		xmm3, xmm0;		// @  @  0 U6 0 U4 0 U2 0 U0
		psrlw		xmm5, 0x08;		// @  @  0 V6 0 V4 0 V2 0 V0
		packuswb	xmm5, xmm3;		// @  U6U4U2U0  @  V6V4V2V0
		movhlps		xmm4, xmm5;
		movlps		dword ptr [edi], xmm1;			// 第一行 Y
		movd		dword ptr [eax], xmm4;	// U
		movd		dword ptr [ebx], xmm5;	// V
		add			esi, 0x10;
		add			edx, 0x10;
		add			edi, 0x08;
		add			eax, 0x04;
		add			ebx, 0x04;
		sub			ecx, 0x01;
		jnz			loop_a8;
loop_m4:
		mov			ecx, nm8;
		test		ecx, 0x04;
		jz			loop_m2;
		movlps		xmm1, dword ptr [esi];			// 上一行 V0 Y1 U0 Y0
		movlps		xmm2, dword ptr [edx];			// 第一行 V0 Y1 U0 Y0
		movaps		xmm3, xmm2;
		movaps		xmm4, xmm2;
		pand		xmm2, xmm0;		// @  @  Y3 Y2  Y1 Y0
		pand		xmm3, xmm7;		// @  @  V2 U2  V0 U0（放大 8 位）
		psrlw		xmm1, 0x08;		// @  @  V2 U2  V0 U0（-1 权重）
		psrlw		xmm4, 0x08;		// 3 权重
		psrlw		xmm3, 0x07;		// 放大 1 位
		psubw		xmm4, xmm1;
		paddw		xmm3, xmm6;
		paddw		xmm3, xmm4;
		psrlw		xmm3, 0x01;		// @  @  VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm3, xmm4;
		packuswb	xmm2, xmm3;		// @  V2U2V0U0  @  第一行 Y
		movhlps		xmm3, xmm2;
		movaps		xmm5, xmm3;
		pand		xmm3, xmm0;		// @  @  @  0 U2 0 U0
		psrlw		xmm5, 0x08;		// @  @  @  0 V2 0 V0
		packuswb	xmm5, xmm3;		// @  @  @  V6V4V2V0
		push		esi;
		push		edx;
		pextrw		esi, xmm3, 0x04;
		pextrw		edx, xmm3, 0x00;
		movd		dword ptr [edi], xmm2;	// 第一行 Y
		mov			word ptr [eax], si;		// U
		mov			word ptr [ebx], dx;		// V
		pop			esi;
		pop			edx;
		add			esi, 0x08;
		add			edx, 0x08;
		add			edi, 0x04;
		add			eax, 0x02;
		add			ebx, 0x02;
loop_m2:
		test		ecx, 0x02;
		jz			loop_end;
		movd		xmm1, dword ptr [esi];	// 上一行 V0 Y1 U0 Y0
		movd		xmm2, dword ptr [edx];	// 第一行 V0 Y1 U0 Y0
		movaps		xmm3, xmm2;
		movaps		xmm4, xmm2;
		pand		xmm3, xmm7;		// @  @  @  V0 U0（放大 8 位）
		psrlw		xmm1, 0x08;		// @  @  @  V0 U0（-1 权重）
		psrlw		xmm4, 0x08;		// 3 权重
		psrlw		xmm3, 0x07;		// 放大 1 位
		psubw		xmm4, xmm1;
		paddw		xmm3, xmm6;
		paddw		xmm3, xmm4;
		psrlw		xmm3, 0x01;		// @  @  @  VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm3, xmm4;
		pextrw		ecx, xmm2, 0x00;
		pextrw		edx, xmm2, 0x01;
		mov			byte ptr [edi], cl;			// 第一个 Y
		mov			byte ptr [edi + 0x01], dl;	// 第二个 Y
		pextrw		ecx, xmm3, 0x00;
		pextrw		edx, xmm3, 0x01;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], dl;		// V
loop_end:
	}
}

// 一行 Buffer 颜色空间转换

// 分片存储的源 YUV422 或 YUV420

// 前两行
void uv20_to_uv22_line_2_F(OUT void* u_o,         // 422 u 输出，存储方式 U0 U2 U4 U6 ……（两行）
						   IN int u_o_p,
						   OUT void* v_o,         // 422 v 输出，存储方式 V0 V2 V4 V6 ……（两行）
						   IN int v_o_p,
						   IN void* u_i,          // 420 u 输入，存储方式 UU0 UU2 UU4 UU6 ……
						   IN int u_i_p_2,
						   IN void* v_i,          // 420 v 输入，存储方式 VV0 VV2 VV4 VV6 ……
						   IN int v_i_p_2,
						   IN int na16,           // 一行像素宽度的一半（因为一行内的 UV 与 Y 为 422 的关系）除以 16 的商与余数
						   IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, u_o;
		mov         edx, v_o;
		mov         esi, u_i;
		mov         eax, v_i;
		pxor        xmm0, xmm0;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		movdqu      xmm1, [esi];        // UU30 UU28 UU26 UU24  UU22 UU20 UU18 UU16  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movdqu      xmm2, [eax];        // VV30 VV28 VV26 VV24  VV22 VV20 VV18 VV16  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		movdqa      xmm3, xmm1;
		movdqa      xmm4, xmm2;
		// 结果为 UU_0
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm2;
		add			esi, u_i_p_2;
		add			eax, v_i_p_2;
		add         edi, u_o_p;
		add         edx, v_o_p;
		// 结果为 UU_0 * 0.75 + UU_2 * 0.25
		movdqu      xmm5, [esi];        // UU_2
		movdqu      xmm6, [eax];        // VV_2
		movdqa      xmm7, xmm5;
		punpcklbw   xmm1, xmm0;         // UU14 UU12 UU10 UU8 UU6 UU4 UU2 UU0
		punpcklbw   xmm2, xmm0;         // VV14 VV12 VV10 VV8 VV6 VV4 VV2 VV0
		punpcklbw   xmm5, xmm0;         // UU_2  low
		punpckhbw   xmm7, xmm0;         // UU_2  high
		punpckhbw   xmm3, xmm0;         // UU30 UU28 UU26 UU24 UU22 UU20 UU18 UU16
		punpckhbw   xmm4, xmm0;         // VV30 VV28 VV26 VV24 VV22 VV20 VV18 VV16
		paddw       xmm5, xmm1;         // UU_2 + UU_0
		paddw       xmm7, xmm3;
		paddw       xmm1, xmm1;         // UU_0 * 2
		paddw       xmm3, xmm3;
		paddw       xmm1, xmm5;         // UU_0 * 3 + UU_2
		paddw       xmm3, xmm7;
		movdqa      xmm5, xmm6;
		movdqa      xmm7, gc_nRoundComp_2;
		punpcklbw   xmm5, xmm0;         // VV_2  low
		punpckhbw   xmm6, xmm0;         // VV_2  high
		paddw       xmm5, xmm2;         // VV_2 + VV_0
		paddw       xmm6, xmm4;
		paddw       xmm2, xmm2;         // VV_0 * 2
		paddw       xmm4, xmm4;
		paddw       xmm2, xmm5;         // VV_0 * 3 + VV_2
		paddw       xmm4, xmm6;
		paddw       xmm1, xmm7;         // UU_1  low
		paddw       xmm3, xmm7;         // UU_1  high
		paddw       xmm2, xmm7;         // VV_1  low
		paddw       xmm4, xmm7;         // VV_1  high
		psrlw       xmm1, 0x02;
		psrlw       xmm3, 0x02;
		psrlw       xmm2, 0x02;
		psrlw       xmm4, 0x02;
		packuswb    xmm1, xmm3;         // UU_1
		packuswb    xmm2, xmm4;         // VV_1
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm2;
		sub         esi, u_i_p_2;
		sub         eax, v_i_p_2;
		sub         edi, u_o_p;
		sub         edx, v_o_p;
		add         esi, 0x10;
		add         eax, 0x10;
		add         edi, 0x10;
		add         edx, 0x10;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		movsd       xmm1, [esi];        // 0000  0000  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movsd       xmm2, [eax];        // 0000  0000  VV14 VV12 VV10 VV8  VV6 VV4 VV2 VV0
		movdqa      xmm7, gc_nRoundComp_2;
		// 结果为 UU_0
		movsd       [edi], xmm1;
		movsd       [edx], xmm2;
		add			esi, u_i_p_2;
		add			eax, v_i_p_2;
		add         edi, u_o_p;
		add         edx, v_o_p;
		// 结果为 UU_0 * 0.75 + UU_2 * 0.25
		movsd       xmm5, [esi];        // UU_2
		movsd       xmm6, [eax];        // VV_2
		punpcklbw   xmm1, xmm0;         // UU14 UU12 UU10 UU8 UU6 UU4 UU2 UU0
		punpcklbw   xmm2, xmm0;         // VV14 VV12 VV10 VV8 VV6 VV4 VV2 VV0
		punpcklbw   xmm5, xmm0;         // UU_2
		punpcklbw   xmm6, xmm0;         // VV_2
		paddw       xmm5, xmm1;         // UU_2 + UU_0
		paddw       xmm6, xmm2;         // VV_2 + VV_0
		paddw       xmm1, xmm1;         // UU_0 * 2
		paddw       xmm2, xmm2;         // VV_0 * 2
		paddw       xmm1, xmm5;         // UU_0 * 3 + UU_2
		paddw       xmm2, xmm6;         // VV_0 * 3 + VV_2
		psrlw       xmm1, 0x02;
		psrlw       xmm2, 0x02;
		packuswb    xmm1, xmm2;         // VV_1  UU_1
		movsd       [edi], xmm1;
		movhps      [edx], xmm1;
		sub         esi, u_i_p_2;
		sub         eax, v_i_p_2;
		sub         edi, u_o_p;
		sub         edx, v_o_p;
		add         esi, 0x08;
		add         eax, 0x08;
		add         edi, 0x08;
		add         edx, 0x08;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		movss       xmm1, [esi];        // 0000  0000  0000  UU6 UU4 UU2 UU0
		movss       xmm2, [eax];        // 0000  0000  0000  VV6 VV4 VV2 VV0
		movdqa      xmm7, gc_nRoundComp_2;
		// 结果为 UU_0
		movss       [edi], xmm1;
		movss       [edx], xmm2;
		add			esi, u_i_p_2;
		add			eax, v_i_p_2;
		add         edi, u_o_p;
		add         edx, v_o_p;
		// 结果为 UU_0 * 0.75 + UU_2 * 0.25
		movss       xmm5, [esi];        // UU_2
		movss       xmm6, [eax];        // VV_2
		punpcklbw   xmm1, xmm0;         // 0 0 0 0 UU6 UU4 UU2 UU0
		punpcklbw   xmm2, xmm0;         // 0 0 0 0 VV6 VV4 VV2 VV0
		punpcklbw   xmm5, xmm0;         // UU_2
		punpcklbw   xmm6, xmm0;         // VV_2
		paddw       xmm5, xmm1;         // UU_2 + UU_0
		paddw       xmm6, xmm2;         // VV_2 + VV_0
		paddw       xmm1, xmm1;         // UU_0 * 2
		paddw       xmm2, xmm2;         // VV_0 * 2
		paddw       xmm1, xmm5;         // UU_0 * 3 + UU_2
		paddw       xmm2, xmm6;         // VV_0 * 3 + VV_2
		psrlw       xmm1, 0x02;
		psrlw       xmm2, 0x02;
		packuswb    xmm1, xmm2;         // VV_1  UU_1
		movhlps     xmm2, xmm1;         // VV_1
		movss       [edi], xmm1;
		movss       [edx], xmm2;
		sub         esi, u_i_p_2;
		sub         eax, v_i_p_2;
		sub         edi, u_o_p;
		sub         edx, v_o_p;
		add         esi, 0x04;
		add         eax, 0x04;
		add         edi, 0x04;
		add         edx, 0x04;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		xor         ebx, ebx;
		xor         ecx, ecx;
		mov         bx, [esi];      // UU_0
		mov         cx, [eax];      // VV_0
		// 结果为 UU_0
		mov         [edi], bx;
		mov         [edx], cx;
		add			esi, u_i_p_2;
		add			eax, v_i_p_2;
		add         edi, u_o_p;
		add         edx, v_o_p;
		push        edi;
		push        edx;
		xor         edi, edi;
		xor         edx, edx;
		mov         di, [esi];      // UU_2
		mov         dx, [eax];      // VV_2
		shl         ecx, 0x10;      // VV_0 0
		shl         edx, 0x10;      // VV_2 0
		or          ebx, ecx;       // VV_0 UU_0
		or          edx, edi;       // VV_2 UU_2
		xor         eax, eax;
		xor         ecx, ecx;
		mov         esi, ebx;
		mov         edi, edx;
		shr         ebx, 0x08;
		shr         edx, 0x08;
		and         esi, 0x00ff00ff;    // VV0_0  UU0_0
		and         edi, 0x00ff00ff;    // VV0_2  UU0_2
		and         ebx, 0x00ff00ff;    // VV2_0  UU2_0
		and         edx, 0x00ff00ff;    // VV2_2  UU2_2
		add         edi, esi;
		add         edx, ebx;
		add         esi, esi;
		add         ebx, ebx;
		add         edi, 0x00020002;
		add         edx, 0x00020002;
		add         esi, edi;
		add         ebx, edx;
		shr         esi, 0x02;
		shr         ebx, 0x02;
		and         esi, 0x00ff00ff;    // VV0_1  UU0_1
		and         ebx, 0x00ff00ff;    // VV2_1  UU2_1
		shl         ebx, 0x08;
		or          ebx, esi;           // VV2 VV0 UU2 UU0
		mov         eax, ebx;
		shr         ebx, 0x10;          // 0 0 VV2 VV0
		pop         edx;
		pop         edi;
		mov			[edx], bx;
		mov         [edi], ax;
loop_end:
	}
}
// 中间行
void uv20_to_uv22_line_2(OUT void* u_o,         // 422 u 输出，存储方式 U0 U2 U4 U6 ……（两行）
						 IN int u_o_p,
						 OUT void* v_o,         // 422 v 输出，存储方式 V0 V2 V4 V6 ……（两行）
						 IN int v_o_p,
						 IN void* u_i,          // 420 u 输入，存储方式 UU0 UU2 UU4 UU6 ……
						 IN int u_i_p_2,
						 IN void* v_i,          // 420 v 输入，存储方式 VV0 VV2 VV4 VV6 ……
						 IN int v_i_p_2,
						 IN int na16,           // 一行像素宽度的一半（因为一行内的 UV 与 Y 为 422 的关系）除以 16 的商与余数
						 IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, u_o;
		mov         edx, v_o;
		mov         esi, u_i;
		mov         ebx, u_i;
		mov         eax, v_i;
		sub         ebx, u_i_p_2;
		pxor        xmm0, xmm0;
		movdqa      xmm7, gc_nRoundComp_2;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		push        ecx;
		push        esi;
		push        eax;
		movdqu      xmm3, [esi];        // UU30 UU28 UU26 UU24  UU22 UU20 UU18 UU16  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movdqu      xmm1, [ebx];        // UU_P
		mov         ecx, edi;
		add         esi, u_i_p_2;       // 下一行
		add         ecx, u_o_p;
		movdqu      xmm5, [esi];        // UU_N
		movdqa      xmm2, xmm1;         // UU_P
		movdqa      xmm4, xmm3;         // UU_C
		movdqa      xmm6, xmm5;         // UU_N
		punpcklbw   xmm1, xmm0;         // UU_P  low
		punpcklbw   xmm3, xmm0;         // UU_C  low
		punpcklbw   xmm5, xmm0;         // UU_N  low
		punpckhbw   xmm2, xmm0;         // UU_P  high
		punpckhbw   xmm4, xmm0;         // UU_C  high
		punpckhbw   xmm6, xmm0;         // UU_N  high
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75、第二个结果：UU_N * 0.25 + UU_C * 0.75
		paddw       xmm1, xmm3;     // UU_P + UU_C
		paddw       xmm5, xmm3;     // UU_N + UU_C
		paddw       xmm2, xmm4;
		paddw       xmm6, xmm4;
		paddw       xmm3, xmm3;     // UU_C * 2
		paddw       xmm4, xmm4;
		paddw       xmm1, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm2, xmm7;
		paddw       xmm6, xmm7;
		paddw       xmm1, xmm3;     // UU_P + UU_C * 3
		paddw       xmm5, xmm3;     // UU_N + UU_C * 3
		paddw       xmm2, xmm4;
		paddw       xmm6, xmm4;
		psrlw       xmm1, 0x02;
		psrlw       xmm5, 0x02;
		psrlw       xmm2, 0x02;
		psrlw       xmm6, 0x02;
		packuswb    xmm1, xmm2;     // 第一个结果
		packuswb    xmm5, xmm6;     // 第二个结果
		movdqu      [edi], xmm1;
		movdqu      [ecx], xmm5;
		mov         esi, eax;
		mov         ecx, edx;
		sub         esi, v_i_p_2;   // 上一行
		movdqu      xmm3, [eax];    // VV_C
		movdqu      xmm1, [esi];    // VV_P
		add         eax, v_i_p_2;   // 下一行
		add         ecx, v_o_p;
		movdqu      xmm5, [eax];    // VV_N
		movdqa      xmm2, xmm1;         // VV_P
		movdqa      xmm4, xmm3;         // VV_C
		movdqa      xmm6, xmm5;         // VV_N
		punpcklbw   xmm1, xmm0;         // VV_P  low
		punpcklbw   xmm3, xmm0;         // VV_C  low
		punpcklbw   xmm5, xmm0;         // VV_N  low
		punpckhbw   xmm2, xmm0;         // VV_P  high
		punpckhbw   xmm4, xmm0;         // VV_C  high
		punpckhbw   xmm6, xmm0;         // VV_N  high
		// 第一个结果：VV_P * 0.25 + VV_C * 0.75、第二个结果：VV_N * 0.25 + VV_C * 0.75
		paddw       xmm1, xmm3;     // VV_P + VV_C
		paddw       xmm5, xmm3;     // VV_N + VV_C
		paddw       xmm2, xmm4;
		paddw       xmm6, xmm4;
		paddw       xmm3, xmm3;     // VV_C * 2
		paddw       xmm4, xmm4;
		paddw       xmm1, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm2, xmm7;
		paddw       xmm6, xmm7;
		paddw       xmm1, xmm3;     // VV_P + VV_C * 3
		paddw       xmm5, xmm3;     // VV_N + VV_C * 3
		paddw       xmm2, xmm4;
		paddw       xmm6, xmm4;
		psrlw       xmm1, 0x02;
		psrlw       xmm5, 0x02;
		psrlw       xmm2, 0x02;
		psrlw       xmm6, 0x02;
		packuswb    xmm1, xmm2;     // 第一个结果
		packuswb    xmm5, xmm6;     // 第二个结果
		movdqu      [edx], xmm1;
		movdqu      [ecx], xmm5;
		pop         eax;
		pop         esi;
		pop         ecx;
		add         eax, 0x10;
		add         ebx, 0x10;
		add         esi, 0x10;
		add         edx, 0x10;
		add         edi, 0x10;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		push        ecx;
		push        eax;
		push        esi;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		movsd       xmm1, [ebx];        // UU_P
		movsd       xmm3, [esi];        // 0000  0000  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movsd       xmm2, [ecx];        // VV_P
		movsd       xmm4, [eax];        // VV_C
		add         esi, u_i_p_2;       // 下一行
		add         eax, v_i_p_2;
		movsd       xmm5, [esi];        // UU_N
		movsd       xmm6, [eax];        // VV_N
		punpcklbw   xmm1, xmm0;         // UU_P
		punpcklbw   xmm2, xmm0;         // VV_P
		punpcklbw   xmm3, xmm0;         // UU_C
		punpcklbw   xmm4, xmm0;         // VV_C
		punpcklbw   xmm5, xmm0;         // UU_N
		punpcklbw   xmm6, xmm0;         // VV_N
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75、第二个结果：UU_N * 0.25 + UU_C * 0.75
		// 第一个结果：VV_P * 0.25 + VV_C * 0.75、第二个结果：VV_N * 0.25 + VV_C * 0.75
		paddw       xmm1, xmm3;     // UU_P + UU_C
		paddw       xmm2, xmm4;     // VV_P + VV_C
		paddw       xmm5, xmm3;     // UU_N + UU_C
		paddw       xmm6, xmm4;     // VV_N + VV_C
		paddw       xmm3, xmm3;     // UU_C * 2
		paddw       xmm4, xmm4;     // VV_C * 2
		paddw       xmm1, xmm7;
		paddw       xmm2, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm6, xmm7;
		paddw       xmm1, xmm3;     // UU_P + UU_C * 3
		paddw       xmm2, xmm4;     // VV_P + VV_C * 3
		paddw       xmm5, xmm3;     // UU_N + UU_C * 3
		paddw       xmm6, xmm4;     // VV_N + VV_C * 3
		psrlw       xmm1, 0x02;
		psrlw       xmm2, 0x02;
		psrlw       xmm5, 0x02;
		psrlw       xmm6, 0x02;
		packuswb    xmm1, xmm5;     // U2  U1
		packuswb    xmm2, xmm6;     // V2  V1
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		movsd       [edi], xmm1;
		movsd       [edx], xmm2;
		movhps      [eax], xmm1;
		movhps      [ecx], xmm2;
		pop         esi;
		pop         eax;
		pop         ecx;
		add         eax, 0x08;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edx, 0x08;
		add         edi, 0x08;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		push        ecx;
		push        eax;
		push        esi;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		movss       xmm1, [ebx];        // UU_P
		movss       xmm3, [esi];        // 0000  0000  0000  UU6 UU4 UU2 UU0
		movss       xmm2, [ecx];        // VV_P
		movss       xmm4, [eax];        // VV_C
		add         esi, u_i_p_2;       // 下一行
		add         eax, v_i_p_2;
		movss       xmm5, [esi];        // UU_N
		movss       xmm6, [eax];        // VV_N
		punpcklbw   xmm1, xmm0;         // UU_P
		punpcklbw   xmm2, xmm0;         // VV_P
		punpcklbw   xmm3, xmm0;         // UU_C
		punpcklbw   xmm4, xmm0;         // VV_C
		punpcklbw   xmm5, xmm0;         // UU_N
		punpcklbw   xmm6, xmm0;         // VV_N
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75、第二个结果：UU_N * 0.25 + UU_C * 0.75
		// 第一个结果：VV_P * 0.25 + VV_C * 0.75、第二个结果：VV_N * 0.25 + VV_C * 0.75
		paddw       xmm1, xmm3;     // UU_P + UU_C
		paddw       xmm2, xmm4;     // VV_P + VV_C
		paddw       xmm5, xmm3;     // UU_N + UU_C
		paddw       xmm6, xmm4;     // VV_N + VV_C
		paddw       xmm3, xmm3;     // UU_C * 2
		paddw       xmm4, xmm4;     // VV_C * 2
		paddw       xmm1, xmm7;
		paddw       xmm2, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm6, xmm7;
		paddw       xmm1, xmm3;     // UU_P + UU_C * 3
		paddw       xmm2, xmm4;     // VV_P + VV_C * 3
		paddw       xmm5, xmm3;     // UU_N + UU_C * 3
		paddw       xmm6, xmm4;     // VV_N + VV_C * 3
		psrlw       xmm1, 0x02;
		psrlw       xmm2, 0x02;
		psrlw       xmm5, 0x02;
		psrlw       xmm6, 0x02;
		packuswb    xmm1, xmm5;     // U2  U1
		packuswb    xmm2, xmm6;     // V2  V1
		movhlps     xmm3, xmm1;     // U2
		movhlps     xmm4, xmm2;     // V2
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		movss       [edi], xmm1;
		movss       [edx], xmm2;
		movss       [eax], xmm3;
		movss       [ecx], xmm4;
		pop         esi;
		pop         eax;
		pop         ecx;
		add         eax, 0x04;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edx, 0x04;
		add         edi, 0x04;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		pinsrw      xmm1, [ebx], 0x00;  // UU_P
		pinsrw      xmm3, [esi], 0x00;  // @@@@  @@@@  @@@@  @ @ UU2 UU0
		pinsrw      xmm1, [ecx], 0x02;  // VV_P
		pinsrw      xmm3, [eax], 0x02;  // VV_C
		add         esi, u_i_p_2;       // 下一行
		add         eax, v_i_p_2;
		pinsrw      xmm5, [esi], 0x00;  // UU_N
		pinsrw      xmm5, [eax], 0x02;  // VV_N
		punpcklbw   xmm1, xmm0;         // VV_P  UU_P
		punpcklbw   xmm3, xmm0;         // VV_C  UU_C
		punpcklbw   xmm5, xmm0;         // VV_N  UU_N
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75、第二个结果：UU_N * 0.25 + UU_C * 0.75
		// 第一个结果：VV_P * 0.25 + VV_C * 0.75、第二个结果：VV_N * 0.25 + VV_C * 0.75
		paddw       xmm1, xmm3;     // VV_P + VV_C    UU_P + UU_C
		paddw       xmm5, xmm3;     // VV_N + VV_C    UU_N + UU_C
		paddw       xmm3, xmm3;     // VV_C * 2    UU_C * 2
		paddw       xmm1, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm1, xmm3;     // VV_P + VV_C * 3    UU_P + UU_C * 3
		paddw       xmm5, xmm3;     // VV_N + VV_C * 3    UU_N + UU_C * 3
		psrlw       xmm1, 0x02;
		psrlw       xmm5, 0x02;
		packuswb    xmm1, xmm5;     // @ V2 @ U2  @ V1 @ U1
		pextrw      eax, xmm1, 0x00;    // U1
		pextrw      ebx, xmm1, 0x02;    // V1
		pextrw      ecx, xmm1, 0x04;    // U2
		pextrw      esi, xmm1, 0x06;    // V2
		mov         [edi], ax;
		mov         [edx], bx;
		add         edi, u_o_p;
		add         edx, v_o_p;
		mov         [edi], cx;
		mov         [edx], si;
loop_end:
	}
}
// 最后两行
void uv20_to_uv22_line_2_L(OUT void* u_o,         // 422 u 输出，存储方式 U0 U2 U4 U6 ……（两行）
						   IN int u_o_p,
						   OUT void* v_o,         // 422 v 输出，存储方式 V0 V2 V4 V6 ……（两行）
						   IN int v_o_p,
						   IN void* u_i,          // 420 u 输入，存储方式 UU0 UU2 UU4 UU6 ……
						   IN int u_i_p_2,
						   IN void* v_i,          // 420 v 输入，存储方式 VV0 VV2 VV4 VV6 ……
						   IN int v_i_p_2,
						   IN int na16,           // 一行像素宽度的一半（因为一行内的 UV 与 Y 为 422 的关系）除以 16 的商与余数
						   IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, u_o;
		mov         edx, v_o;
		mov         esi, u_i;
		mov         ebx, u_i;
		mov         eax, v_i;
		sub         ebx, u_i_p_2;
		pxor        xmm0, xmm0;
		movdqa      xmm7, gc_nRoundComp_2;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		push        eax;
		push        ecx;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		movdqu      xmm3, [esi];        // UU30 UU28 UU26 UU24  UU22 UU20 UU18 UU16  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movdqu      xmm1, [ebx];        // UU_P
		movdqu      xmm6, [eax];        // VV_C
		movdqu      xmm5, [ecx];        // VV_P
		movdqa      xmm2, xmm1;
		movdqa      xmm4, xmm3;
		// 第二个结果：UU_C
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		movdqu      [eax], xmm3;
		movdqu      [ecx], xmm6;
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75
		punpcklbw   xmm1, xmm0;         // UU_P  low
		punpcklbw   xmm3, xmm0;         // UU_C  low
		punpckhbw   xmm2, xmm0;         // UU_P  high
		punpckhbw   xmm4, xmm0;         // UU_C  high
		paddw       xmm1, xmm3;     // UU_P + UU_C
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm3;     // UU_C * 2
		paddw       xmm4, xmm4;
		paddw       xmm1, xmm7;
		paddw       xmm2, xmm7;
		paddw       xmm1, xmm3;     // UU_P + UU_C * 3
		paddw       xmm2, xmm4;
		movdqa      xmm3, xmm5;
		movdqa      xmm4, xmm6;
		punpcklbw   xmm3, xmm0;         // VV_P  low
		punpcklbw   xmm4, xmm0;         // VV_C  low
		punpckhbw   xmm5, xmm0;         // VV_P  high
		punpckhbw   xmm6, xmm0;         // VV_C  high
		paddw       xmm3, xmm4;     // VV_P + VV_C
		paddw       xmm5, xmm6;
		paddw       xmm4, xmm4;     // VV_C * 2
		paddw       xmm6, xmm6;
		paddw       xmm3, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm3, xmm4;     // VV_P + VV_C * 3
		paddw       xmm5, xmm6;
		psrlw       xmm1, 0x02;
		psrlw       xmm2, 0x02;
		psrlw       xmm3, 0x02;
		psrlw       xmm5, 0x02;
		packuswb    xmm1, xmm2;     // UU_1
		packuswb    xmm3, xmm5;     // VV_1
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm3;
		pop         ecx;
		pop         eax;
		add         eax, 0x10;
		add         ebx, 0x10;
		add         esi, 0x10;
		add         edx, 0x10;
		add         edi, 0x10;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		push        eax;
		push        ecx;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		movsd       xmm3, [esi];        // UU30 UU28 UU26 UU24  UU22 UU20 UU18 UU16  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movsd       xmm1, [ebx];        // UU_P
		movsd       xmm6, [eax];        // VV_C
		movsd       xmm5, [ecx];        // VV_P
		// 第二个结果：UU_C
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		movsd       [eax], xmm3;
		movsd       [ecx], xmm6;
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75
		punpcklbw   xmm1, xmm0;         // UU_P
		punpcklbw   xmm3, xmm0;         // UU_C
		punpcklbw   xmm5, xmm0;         // VV_P
		punpcklbw   xmm6, xmm0;         // VV_C
		paddw       xmm1, xmm3;     // UU_P + UU_C
		paddw       xmm5, xmm6;     // VV_P + VV_C
		paddw       xmm3, xmm3;     // UU_C * 2
		paddw       xmm6, xmm6;     // VV_C * 2
		paddw       xmm1, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm1, xmm3;     // UU_P + UU_C * 3
		paddw       xmm5, xmm6;     // VV_P + VV_C * 3
		psrlw       xmm1, 0x02;
		psrlw       xmm5, 0x02;
		packuswb    xmm1, xmm5;     // VV_1    UU_1
		movsd       [edi], xmm1;
		movhps      [edx], xmm1;
		pop         ecx;
		pop         eax;
		add         eax, 0x08;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edx, 0x08;
		add         edi, 0x08;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		push        eax;
		push        ecx;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		movss       xmm3, [esi];        // UU30 UU28 UU26 UU24  UU22 UU20 UU18 UU16  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movss       xmm1, [ebx];        // UU_P
		movss       xmm6, [eax];        // VV_C
		movss       xmm5, [ecx];        // VV_P
		// 第二个结果：UU_C
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		movss       [eax], xmm3;
		movss       [ecx], xmm6;
		// 第一个结果：UU_P * 0.25 + UU_C * 0.75
		punpcklbw   xmm1, xmm0;         // UU_P
		punpcklbw   xmm3, xmm0;         // UU_C
		punpcklbw   xmm5, xmm0;         // VV_P
		punpcklbw   xmm6, xmm0;         // VV_C
		paddw       xmm1, xmm3;     // UU_P + UU_C
		paddw       xmm5, xmm6;     // VV_P + VV_C
		paddw       xmm3, xmm3;     // UU_C * 2
		paddw       xmm6, xmm6;     // VV_C * 2
		paddw       xmm1, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm1, xmm3;     // UU_P + UU_C * 3
		paddw       xmm5, xmm6;     // VV_P + VV_C * 3
		psrlw       xmm1, 0x02;
		psrlw       xmm5, 0x02;
		packuswb    xmm1, xmm5;     // VV_1    UU_1
		movhlps     xmm3, xmm1;
		movss       [edi], xmm1;
		movss       [edx], xmm3;
		pop         ecx;
		pop         eax;
		add         eax, 0x04;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edx, 0x04;
		add         edi, 0x04;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		pinsrw      xmm1, [ebx], 0x00;  // UU_P
		pinsrw      xmm3, [esi], 0x00;  // @@@@  @@@@  @@@@  @ @ UU2 UU0
		pinsrw      xmm1, [ecx], 0x02;  // VV_P
		pinsrw      xmm3, [eax], 0x02;  // VV_C
		// 第二个结果：VV_C
		pextrw      ebx, xmm3, 0x00;
		pextrw      esi, xmm3, 0x02;
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		mov         [eax], bx;
		mov         [ecx], si;
		// 第一个结果：VV_P * 0.25 + VV_C * 0.75
		punpcklbw   xmm1, xmm0;         // VV_P  UU_P
		punpcklbw   xmm3, xmm0;         // VV_C  UU_C
		paddw       xmm1, xmm3;     // VV_P + VV_C    UU_P + UU_C
		paddw       xmm3, xmm3;     // VV_C * 2    UU_C * 2
		paddw       xmm1, xmm7;
		paddw       xmm1, xmm3;     // VV_P + VV_C * 3    UU_P + UU_C * 3
		psrlw       xmm1, 0x02;
		packuswb    xmm1, xmm1;     // @ V1 @ U1  @ V1 @ U1
		pextrw      eax, xmm1, 0x00;    // U1
		pextrw      ebx, xmm1, 0x02;    // V1
		mov         [edi], ax;
		mov         [edx], bx;
loop_end:
	}
}
// 最后一行
void uv20_to_uv22_line_1_L(OUT void* u_o,         // 422 u 输出，存储方式 U0 U2 U4 U6 ……（两行）
						   OUT void* v_o,         // 422 v 输出，存储方式 V0 V2 V4 V6 ……（两行）
						   IN void* u_i,          // 420 u 输入，存储方式 UU0 UU2 UU4 UU6 ……
						   IN int u_i_p_2,
						   IN void* v_i,          // 420 v 输入，存储方式 VV0 VV2 VV4 VV6 ……
						   IN int v_i_p_2,
						   IN int na16,           // 一行像素宽度的一半（因为一行内的 UV 与 Y 为 422 的关系）除以 16 的商与余数
						   IN int nm16)
{
	_asm {
		mov         ecx, na16;
		mov         edi, u_o;
		mov         edx, v_o;
		mov         esi, u_i;
		mov         ebx, u_i;
		mov         eax, v_i;
		sub         ebx, u_i_p_2;
		pxor        xmm0, xmm0;
		movdqa      xmm7, gc_nRoundComp_2;
		test        ecx, ecx;
		jnz         loop_a16;
		jmp         loop_m8;
loop_a16:
		push        eax;
		push        ecx;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		movdqu      xmm3, [esi];        // UU30 UU28 UU26 UU24  UU22 UU20 UU18 UU16  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movdqu      xmm1, [ebx];        // UU_P
		movdqu      xmm6, [eax];        // VV_C
		movdqu      xmm5, [ecx];        // VV_P
		movdqa      xmm2, xmm1;
		movdqa      xmm4, xmm3;
		// 结果：UU_P * 0.25 + UU_C * 0.75
		punpcklbw   xmm1, xmm0;         // UU_P  low
		punpcklbw   xmm3, xmm0;         // UU_C  low
		punpckhbw   xmm2, xmm0;         // UU_P  high
		punpckhbw   xmm4, xmm0;         // UU_C  high
		paddw       xmm1, xmm3;     // UU_P + UU_C
		paddw       xmm2, xmm4;
		paddw       xmm3, xmm3;     // UU_C * 2
		paddw       xmm4, xmm4;
		paddw       xmm1, xmm7;
		paddw       xmm2, xmm7;
		paddw       xmm1, xmm3;     // UU_P + UU_C * 3
		paddw       xmm2, xmm4;
		movdqa      xmm3, xmm5;
		movdqa      xmm4, xmm6;
		punpcklbw   xmm3, xmm0;         // VV_P  low
		punpcklbw   xmm4, xmm0;         // VV_C  low
		punpckhbw   xmm5, xmm0;         // VV_P  high
		punpckhbw   xmm6, xmm0;         // VV_C  high
		paddw       xmm3, xmm4;     // VV_P + VV_C
		paddw       xmm5, xmm6;
		paddw       xmm4, xmm4;     // VV_C * 2
		paddw       xmm6, xmm6;
		paddw       xmm3, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm3, xmm4;     // VV_P + VV_C * 3
		paddw       xmm5, xmm6;
		psrlw       xmm1, 0x02;
		psrlw       xmm2, 0x02;
		psrlw       xmm3, 0x02;
		psrlw       xmm5, 0x02;
		packuswb    xmm1, xmm2;     // UU_1
		packuswb    xmm3, xmm5;     // VV_1
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm3;
		pop         ecx;
		pop         eax;
		add         eax, 0x10;
		add         ebx, 0x10;
		add         esi, 0x10;
		add         edx, 0x10;
		add         edi, 0x10;
		sub         ecx, 0x01;
		jnz         loop_a16;
loop_m8:
		mov         ecx, nm16;
		test        ecx, 0x08;
		jz          loop_m4;
		push        eax;
		push        ecx;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		movsd       xmm3, [esi];        // UU30 UU28 UU26 UU24  UU22 UU20 UU18 UU16  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movsd       xmm1, [ebx];        // UU_P
		movsd       xmm6, [eax];        // VV_C
		movsd       xmm5, [ecx];        // VV_P
		// 结果：UU_P * 0.25 + UU_C * 0.75
		punpcklbw   xmm1, xmm0;         // UU_P
		punpcklbw   xmm3, xmm0;         // UU_C
		punpcklbw   xmm5, xmm0;         // VV_P
		punpcklbw   xmm6, xmm0;         // VV_C
		paddw       xmm1, xmm3;     // UU_P + UU_C
		paddw       xmm5, xmm6;     // VV_P + VV_C
		paddw       xmm3, xmm3;     // UU_C * 2
		paddw       xmm6, xmm6;     // VV_C * 2
		paddw       xmm1, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm1, xmm3;     // UU_P + UU_C * 3
		paddw       xmm5, xmm6;     // VV_P + VV_C * 3
		psrlw       xmm1, 0x02;
		psrlw       xmm5, 0x02;
		packuswb    xmm1, xmm5;     // VV_1    UU_1
		movsd       [edi], xmm1;
		movhps      [edx], xmm1;
		pop         ecx;
		pop         eax;
		add         eax, 0x08;
		add         ebx, 0x08;
		add         esi, 0x08;
		add         edx, 0x08;
		add         edi, 0x08;
loop_m4:
		test        ecx, 0x04;
		jz          loop_m2;
		push        eax;
		push        ecx;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		movss       xmm3, [esi];        // UU30 UU28 UU26 UU24  UU22 UU20 UU18 UU16  UU14 UU12 UU10 UU8  UU6 UU4 UU2 UU0
		movss       xmm1, [ebx];        // UU_P
		movss       xmm6, [eax];        // VV_C
		movss       xmm5, [ecx];        // VV_P
		// 结果：UU_P * 0.25 + UU_C * 0.75
		punpcklbw   xmm1, xmm0;         // UU_P
		punpcklbw   xmm3, xmm0;         // UU_C
		punpcklbw   xmm5, xmm0;         // VV_P
		punpcklbw   xmm6, xmm0;         // VV_C
		paddw       xmm1, xmm3;     // UU_P + UU_C
		paddw       xmm5, xmm6;     // VV_P + VV_C
		paddw       xmm3, xmm3;     // UU_C * 2
		paddw       xmm6, xmm6;     // VV_C * 2
		paddw       xmm1, xmm7;
		paddw       xmm5, xmm7;
		paddw       xmm1, xmm3;     // UU_P + UU_C * 3
		paddw       xmm5, xmm6;     // VV_P + VV_C * 3
		psrlw       xmm1, 0x02;
		psrlw       xmm5, 0x02;
		packuswb    xmm1, xmm5;     // VV_1    UU_1
		movhlps     xmm3, xmm1;
		movss       [edi], xmm1;
		movss       [edx], xmm3;
		pop         ecx;
		pop         eax;
		add         eax, 0x04;
		add         ebx, 0x04;
		add         esi, 0x04;
		add         edx, 0x04;
		add         edi, 0x04;
loop_m2:
		test        ecx, 0x02;
		jz          loop_end;
		mov         ecx, eax;
		sub         ecx, v_i_p_2;
		pinsrw      xmm1, [ebx], 0x00;  // UU_P
		pinsrw      xmm3, [esi], 0x00;  // @@@@  @@@@  @@@@  @ @ UU2 UU0
		pinsrw      xmm1, [ecx], 0x02;  // VV_P
		pinsrw      xmm3, [eax], 0x02;  // VV_C
		// 结果：VV_P * 0.25 + VV_C * 0.75
		punpcklbw   xmm1, xmm0;         // VV_P  UU_P
		punpcklbw   xmm3, xmm0;         // VV_C  UU_C
		paddw       xmm1, xmm3;     // VV_P + VV_C    UU_P + UU_C
		paddw       xmm3, xmm3;     // VV_C * 2    UU_C * 2
		paddw       xmm1, xmm7;
		paddw       xmm1, xmm3;     // VV_P + VV_C * 3    UU_P + UU_C * 3
		psrlw       xmm1, 0x02;
		packuswb    xmm1, xmm1;     // @ V1 @ U1  @ V1 @ U1
		pextrw      eax, xmm1, 0x00;    // U1
		pextrw      ebx, xmm1, 0x02;    // V1
		mov         [edi], ax;
		mov         [edx], bx;
loop_end:
	}
}

void yuv422_to_color32_line(OUT void* color32,      // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
							IN void* y,             // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
							IN void* u,             // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
							IN void* v,             // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
							IN int na16,            // 源字节宽度减 4 后除以 16 的商与余数
							IN int nm16,
							IN int bHD,             // 高标清标志
							IN BOOL bBGRA)          // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			yuv422_to_color32_inner(color32, y, v, u, na16, nm16, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
		}
		else       // R 在低位
		{
			yuv422_to_color32_inner(color32, y, u, v, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
	}
	else    // 标清
	{
		if (bBGRA)      // B 在低位
		{
			yuv422_to_color32_inner(color32, y, v, u, na16, nm16, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
		}
		else       // R 在低位
		{
			yuv422_to_color32_inner(color32, y, u, v, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
	}
}
void yuv422_to_color32_inner(OUT void* color32,     // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
							 IN void* y,            // y 输入，存放方式 Y0 Y1 Y2 Y3 ……
							 IN void* u,            // u 输入，存放方式 UU0 UU2 UU4 UU6 ……
							 IN void* v,            // v 输入，存放方式 VV0 VV2 VV4 VV6 ……
							 IN int na8,		    // 源像素宽度减 1 后除以 8 的商与余数
							 IN int nm8,
							 IN int* pnR_V,         // 系数
							 IN int* pnG_U,         // （通过交换 U 与 V 可以做到 RGBA 与 BGRA 的兼容）
							 IN int* pnG_V,         // （输入 u 与 v 交换，pnR_V 与 pnB_U 交换，pnG_V 与 pnG_U 交换即可实现）
							 IN int* pnB_U)
{
	_asm{       // 程序内部以 RGBA 为例进行说明
		mov			ebx, y;
		mov         edx, u;
		mov         esi, v;
		mov			ecx, na8;
		mov			edi, color32;
		pxor        xmm0, xmm0;
		test		ecx, ecx;
		jnz			loop_a8;
		jmp			loop_m8;
loop_a8:
		movsd       xmm1, [ebx];        // 0000  0000  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm7, [edx];        // 0000  0000  0000  U6 U4 U2 U0
		movss       xmm6, [esi];        // 0000  0000  0000  V6 V4 V2 V0
		push        edx;
		push        ecx;
		push        ebx;
		xor         eax, eax;
		xor         ecx, ecx;
		mov         al, [edx + 0x04];   // 000 U8
		mov         cl, [esi + 0x04];   // 000 V8
		punpcklbw   xmm7, xmm0;         // 0 0 0 0 U6 U4 U2 U0
		punpcklbw   xmm6, xmm0;         // 0 0 0 0 V6 V4 V2 V0
		punpcklbw   xmm1, xmm0;         // Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		punpcklwd   xmm7, xmm0;         // 0 U6 0 U4 0 U2 0 U0
		punpcklwd   xmm6, xmm0;         // 0 V6 0 V4 0 V2 0 V0
		movdqa      xmm5, xmm7;
		movdqa      xmm4, xmm6;
		pslld       xmm5, 0x10;         // U6 0 U4 0 U2 0 U0 0
		pslld       xmm4, 0x10;         // V6 0 V4 0 V2 0 V0 0
		movdqa      xmm3, xmm5;
		movdqa      xmm2, xmm4;
		shufps      xmm3, xmm3, 0x39;   // U0 0 U6 0 U4 0 U2 0
		shufps      xmm2, xmm2, 0x39;   // V0 0 V6 0 V4 0 V2 0
		pinsrw      xmm3, eax, 0x07;    // U8 0 U6 0 U4 0 U2 0
		pinsrw      xmm2, ecx, 0x07;    // V8 0 V6 0 V4 0 V2 0
		por         xmm5, xmm7;         // U6 U6 U4 U4 U2 U2 U0 U0
		por         xmm3, xmm7;         // U8 U6 U6 U4 U4 U2 U2 U0
		por         xmm4, xmm6;         // V6 V6 V4 V4 V2 V2 V0 V0
		por         xmm2, xmm6;         // V8 V6 V6 V4 V4 V2 V2 V0
		paddw       xmm5, xmm3;         // U7 U6 U5 U4 U3 U2 U1 U0
		paddw       xmm4, xmm2;         // V7 V6 V5 V4 V3 V2 V1 V0
		movdqa      xmm7, gc_nOff128_7;
		psllw       xmm5, 0x06;         // 总共放大 7 位
		psllw       xmm4, 0x06;
		psllw       xmm1, 0x05;         // 放大 5 位
		psubw       xmm5, xmm7;         // - 128
		psubw       xmm4, xmm7;
		mov         eax, pnR_V;
		mov         ebx, pnG_V;
		mov         ecx, pnG_U;
		mov         edx, pnB_U;
		movdqu		xmm2, [eax];
		movdqu		xmm3, [ebx];
		movdqu		xmm6, [ecx];
		movdqu		xmm7, [edx];
		pop         ebx;
		pop         ecx;
		pop         edx;
		pmulhw		xmm2, xmm4;         // 还放大 5 位
		pmulhw		xmm3, xmm4;
		pmulhw		xmm6, xmm5;
		pmulhw		xmm7, xmm5;
		movdqa      xmm4, gc_nRoundComp_5;
		movdqa      xmm5, gc_dwMaskWLB; // 255
		paddw       xmm2, xmm1;         // R
		paddw       xmm3, xmm1;         // G
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm1;         // B
		paddw       xmm2, xmm4;         // R
		paddw       xmm3, xmm6;         // G
		paddw       xmm7, xmm4;         // B
		pmaxsw		xmm2, xmm0;			// R
		pmaxsw		xmm3, xmm0;			// G
		pmaxsw		xmm7, xmm0;			// B
		psrlw		xmm2, 0x05;
		psrlw		xmm3, 0x05;
		psrlw		xmm7, 0x05;
		pminsw		xmm2, xmm5;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm3, xmm5;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm7, xmm5;		// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm3, 0x08;
		pxor        xmm4, xmm4;
		pxor        xmm5, xmm5;
		por			xmm2, xmm3;		// G7R7 G6R6 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		movdqa		xmm1, gc_dwMaskAlpha;	// A000 A000 A000 A000
		movdqa		xmm6, xmm2;
		punpcklwd	xmm2, xmm5;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpckhwd	xmm6, xmm5;			// 00 G7R7 00 G6R6 00 G5R5 00 G4R4
		punpcklwd	xmm4, xmm7;			// 0B3 00 0B2 00 0B1 00 0B0 00
		punpckhwd	xmm5, xmm7;			// 0B7 00 0B6 00 0B5 00 0B4 00
		por			xmm2, xmm1;
		por			xmm6, xmm1;
		por			xmm2, xmm4;
		por			xmm6, xmm5;
		movdqu		[edi], xmm2;
		add			ebx, 0x08;
		add			edi, 0x10;
		add         edx, 0x04;
		movdqu		[edi], xmm6;
		add			esi, 0x04;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a8;
loop_m8:
		mov			ecx, nm8;
		test		ecx, 0x08;
		jz			loop_m5;
		movsd       xmm1, [ebx];        // 0000  0000  Y7 Y6 Y5 Y4  Y3 Y2 Y1 Y0
		movss       xmm7, [edx];        // 0000  0000  0000  U6 U4 U2 U0
		movss       xmm6, [esi];        // 0000  0000  0000  V6 V4 V2 V0
		punpcklbw   xmm7, xmm0;         // 0 0 0 0 U6 U4 U2 U0
		punpcklbw   xmm6, xmm0;         // 0 0 0 0 V6 V4 V2 V0
		punpcklbw   xmm1, xmm0;         // Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		punpcklwd   xmm7, xmm0;         // 0 U6 0 U4 0 U2 0 U0
		punpcklwd   xmm6, xmm0;         // 0 V6 0 V4 0 V2 0 V0
		movdqa      xmm5, xmm7;
		movdqa      xmm4, xmm6;
		pslld       xmm5, 0x10;         // U6 0 U4 0 U2 0 U0 0
		pslld       xmm4, 0x10;         // V6 0 V4 0 V2 0 V0 0
		movdqa      xmm3, xmm5;
		movdqa      xmm2, xmm4;
		shufps      xmm3, xmm3, 0xf9;   // U6 0 U6 0 U4 0 U2 0
		shufps      xmm2, xmm2, 0xf9;   // V6 0 V6 0 V4 0 V2 0
		por         xmm5, xmm7;         // U6 U6 U4 U4 U2 U2 U0 U0
		por         xmm3, xmm7;         // U6 U6 U6 U4 U4 U2 U2 U0
		por         xmm4, xmm6;         // V6 V6 V4 V4 V2 V2 V0 V0
		por         xmm2, xmm6;         // V6 V6 V6 V4 V4 V2 V2 V0
		paddw       xmm5, xmm3;         // U7 U6 U5 U4 U3 U2 U1 U0
		paddw       xmm4, xmm2;         // V7 V6 V5 V4 V3 V2 V1 V0
		movdqa      xmm7, gc_nOff128_7;
		psllw       xmm5, 0x06;         // 总共放大 7 位
		psllw       xmm4, 0x06;
		psllw       xmm1, 0x05;         // 放大 5 位
		psubw       xmm5, xmm7;         // - 128
		psubw       xmm4, xmm7;
		mov         eax, pnR_V;
		mov         ebx, pnG_V;
		mov         ecx, pnG_U;
		mov         edx, pnB_U;
		movdqu		xmm2, [eax];
		movdqu		xmm3, [ebx];
		movdqu		xmm6, [ecx];
		movdqu		xmm7, [edx];
		pmulhw		xmm2, xmm4;         // 还放大 5 位
		pmulhw		xmm3, xmm4;
		pmulhw		xmm6, xmm5;
		pmulhw		xmm7, xmm5;
		movdqa      xmm4, gc_nRoundComp_5;
		movdqa      xmm5, gc_dwMaskWLB; // 255
		paddw       xmm2, xmm1;         // R
		paddw       xmm3, xmm1;         // G
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm1;         // B
		paddw       xmm2, xmm4;         // R
		paddw       xmm3, xmm6;         // G
		paddw       xmm7, xmm4;         // B
		pmaxsw		xmm2, xmm0;			// R
		pmaxsw		xmm3, xmm0;			// G
		pmaxsw		xmm7, xmm0;			// B
		psrlw		xmm2, 0x05;
		psrlw		xmm3, 0x05;
		psrlw		xmm7, 0x05;
		pminsw		xmm2, xmm5;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm3, xmm5;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm7, xmm5;		// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm3, 0x08;
		pxor        xmm4, xmm4;
		pxor        xmm5, xmm5;
		por			xmm2, xmm3;		// G7R7 G6R6 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		movdqa		xmm1, gc_dwMaskAlpha;	// A000 A000 A000 A000
		movdqa		xmm6, xmm2;
		punpcklwd	xmm2, xmm5;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpckhwd	xmm6, xmm5;			// 00 G7R7 00 G6R6 00 G5R5 00 G4R4
		punpcklwd	xmm4, xmm7;			// 0B3 00 0B2 00 0B1 00 0B0 00
		punpckhwd	xmm5, xmm7;			// 0B7 00 0B6 00 0B5 00 0B4 00
		por			xmm2, xmm1;
		por			xmm6, xmm1;
		por			xmm2, xmm4;
		por			xmm6, xmm5;
		movdqu		[edi], xmm2;
		add			edi, 0x10;
		movdqu		[edi], xmm6;
		jmp			loop_end;
loop_m5:
		cmp			ecx, 0x04;
		jle			loop_m4;
		pxor        xmm7, xmm7;
		movss       xmm1, [ebx];        // 0000  0000  0000  Y3 Y2 Y1 Y0
		pinsrw      xmm7, [edx], 0x00;  // 0000  0000  0000  00 U2 U0
		pinsrw      xmm7, [esi], 0x01;  // 0000  0000  0000  V2 V0 U2 U0
		push        edx;
		push        ecx;
		push        ebx;
		xor         eax, eax;
		xor         ecx, ecx;
		mov         al, [edx + 0x02];   // 000 U4
		mov         cl, [esi + 0x02];   // 000 V4
		punpcklbw   xmm7, xmm0;         // 0 0 0 0 V2 V0 U2 U0
		punpcklbw   xmm1, xmm0;         // 0 0 0 0 Y3 Y2 Y1 Y0
		punpcklwd   xmm7, xmm0;         // 0 V2 0 V0 0 U2 0 U0
		movdqa      xmm5, xmm7;
		pslld       xmm5, 0x10;         // V2 0 V0 0 U2 0 U0 0
		movdqa      xmm3, xmm5;
		movdqa      xmm4, gc_nOff128_7;
		shufps      xmm3, xmm3, 0xb1;   // V0 0 V2 0 U0 0 U2 0
		pinsrw      xmm3, eax, 0x03;    // V0 0 V2 0 U4 0 U2 0
		pinsrw      xmm3, ecx, 0x07;    // V4 0 V2 0 U4 0 U2 0
		por         xmm5, xmm7;         // V2 V2 V0 V0 U2 U2 U0 U0
		por         xmm3, xmm7;         // V4 V2 V2 V0 U4 U2 U2 U0
		paddw       xmm5, xmm3;         // V3 V2 V1 V0 U3 U2 U1 U0
		psllw       xmm5, 0x06;         // 总共放大 7 位
		psllw       xmm1, 0x05;         // 放大 5 位
		psubw       xmm5, xmm4;         // - 128
		movhlps     xmm4, xmm5;         // @ @ @ @ V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         ebx, pnG_V;
		mov         ecx, pnG_U;
		mov         edx, pnB_U;
		movdqu		xmm2, [eax];
		movdqu		xmm3, [ebx];
		movdqu		xmm6, [ecx];
		movdqu		xmm7, [edx];
		pop         ebx;
		pop         ecx;
		pop         edx;
		pmulhw		xmm2, xmm4;         // 还放大 5 位
		pmulhw		xmm3, xmm4;
		pmulhw		xmm6, xmm5;
		pmulhw		xmm7, xmm5;
		movdqa      xmm4, gc_nRoundComp_5;
		movdqa      xmm5, gc_dwMaskWLB; // 255
		paddw       xmm2, xmm1;         // R
		paddw       xmm3, xmm1;         // G
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm1;         // B
		paddw       xmm2, xmm4;         // R
		paddw       xmm3, xmm6;         // G
		paddw       xmm7, xmm4;         // B
		pmaxsw		xmm2, xmm0;			// R
		pmaxsw		xmm3, xmm0;			// G
		pmaxsw		xmm7, xmm0;			// B
		psrlw		xmm2, 0x05;
		psrlw		xmm3, 0x05;
		psrlw		xmm7, 0x05;
		pminsw		xmm2, xmm5;		// @ @ @ @ R3 R2 R1 R0
		pminsw		xmm3, xmm5;		// @ @ @ @ G3 G2 G1 G0
		pminsw		xmm7, xmm5;		// @ @ @ @ B3 B2 B1 B0
		psllw		xmm3, 0x08;
		pxor        xmm4, xmm4;
		por			xmm2, xmm3;		// @ @ @ @ G3R3 G2R2 G1R1 G0R0
		movdqa		xmm1, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm2, xmm0;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpcklwd	xmm4, xmm7;			// 0B3 00 0B2 00 0B1 00 0B0 00
		por			xmm2, xmm1;
		por			xmm2, xmm4;
		movdqu		[edi], xmm2;
		add         ebx, 0x04;
		add         edx, 0x02;
		add         esi, 0x02;
		add         edi, 0x10;
		jmp			loop_m3;
loop_m4:
		test		ecx, 0x08;
		jz			loop_m3;
		pxor        xmm7, xmm7;
		movss       xmm1, [ebx];        // 0000  0000  0000  Y3 Y2 Y1 Y0
		pinsrw      xmm7, [edx], 0x00;  // 0000  0000  0000  00 U2 U0
		pinsrw      xmm7, [esi], 0x01;  // 0000  0000  0000  V2 V0 U2 U0
		punpcklbw   xmm7, xmm0;         // 0 0 0 0 V2 V0 U2 U0
		punpcklbw   xmm1, xmm0;         // 0 0 0 0 Y3 Y2 Y1 Y0
		punpcklwd   xmm7, xmm0;         // 0 V2 0 V0 0 U2 0 U0
		movdqa      xmm5, xmm7;
		pslld       xmm5, 0x10;         // V2 0 V0 0 U2 0 U0 0
		movdqa      xmm3, xmm5;
		movdqa      xmm4, gc_nOff128_7;
		shufps      xmm3, xmm3, 0xf5;   // V2 0 V2 0 U2 0 U2 0
		por         xmm5, xmm7;         // V2 V2 V0 V0 U2 U2 U0 U0
		por         xmm3, xmm7;         // V2 V2 V2 V0 U2 U2 U2 U0
		paddw       xmm5, xmm3;         // V3 V2 V1 V0 U3 U2 U1 U0
		psllw       xmm5, 0x06;         // 总共放大 7 位
		psllw       xmm1, 0x05;         // 放大 5 位
		psubw       xmm5, xmm4;         // - 128
		movhlps     xmm4, xmm5;         // @ @ @ @ V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         ebx, pnG_V;
		mov         ecx, pnG_U;
		mov         edx, pnB_U;
		movdqu		xmm2, [eax];
		movdqu		xmm3, [ebx];
		movdqu		xmm6, [ecx];
		movdqu		xmm7, [edx];
		pmulhw		xmm2, xmm4;         // 还放大 5 位
		pmulhw		xmm3, xmm4;
		pmulhw		xmm6, xmm5;
		pmulhw		xmm7, xmm5;
		movdqa      xmm4, gc_nRoundComp_5;
		movdqa      xmm5, gc_dwMaskWLB; // 255
		paddw       xmm2, xmm1;         // R
		paddw       xmm3, xmm1;         // G
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm1;         // B
		paddw       xmm2, xmm4;         // R
		paddw       xmm3, xmm6;         // G
		paddw       xmm7, xmm4;         // B
		pmaxsw		xmm2, xmm0;			// R
		pmaxsw		xmm3, xmm0;			// G
		pmaxsw		xmm7, xmm0;			// B
		psrlw		xmm2, 0x05;
		psrlw		xmm3, 0x05;
		psrlw		xmm7, 0x05;
		pminsw		xmm2, xmm5;		// @ @ @ @ R3 R2 R1 R0
		pminsw		xmm3, xmm5;		// @ @ @ @ G3 G2 G1 G0
		pminsw		xmm7, xmm5;		// @ @ @ @ B3 B2 B1 B0
		psllw		xmm3, 0x08;
		pxor        xmm4, xmm4;
		por			xmm2, xmm3;		// @ @ @ @ G3R3 G2R2 G1R1 G0R0
		movdqa		xmm1, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm2, xmm0;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpcklwd	xmm4, xmm7;			// 0B3 00 0B2 00 0B1 00 0B0 00
		por			xmm2, xmm1;
		por			xmm2, xmm4;
		movdqu		[edi], xmm2;
		jmp			loop_end;
loop_m3:
		cmp         ecx, 0x03;
		jnz         loop_m2;
		pxor        xmm1, xmm1;
		pxor        xmm7, xmm7;
		pinsrw      xmm1, [ebx], 0x00;  // 0000  0000  0000  00 Y1 Y0
		pinsrw      xmm7, [edx], 0x00;  // 0000  0000  0000  00 U2 U0
		pinsrw      xmm7, [esi], 0x01;  // 0000  0000  0000  V2 V0 U2 U0
		xor         eax, eax;
		mov         al, [ebx + 0x02];   // 000 Y2
		pinsrw      xmm1, eax, 0x01;    // 0000  0000  0000  0 Y2 Y1 Y0
		punpcklbw   xmm7, xmm0;         // 0 0 0 0 V2 V0 U2 U0
		punpcklbw   xmm1, xmm0;         // 0 0 0 0 0 Y2 Y1 Y0
		punpcklwd   xmm7, xmm0;         // 0 V2 0 V0 0 U2 0 U0
		movdqa      xmm5, xmm7;
		pslld       xmm5, 0x10;         // V2 0 V0 0 U2 0 U0 0
		movdqa      xmm3, xmm5;
		movdqa      xmm4, gc_nOff128_7;
		shufps      xmm3, xmm3, 0xf5;   // V2 0 V2 0 U2 0 U2 0
		por         xmm5, xmm7;         // V2 V2 V0 V0 U2 U2 U0 U0
		por         xmm3, xmm7;         // V2 V2 V2 V0 U2 U2 U2 U0
		paddw       xmm5, xmm3;         // V2 V2 V1 V0 U2 U2 U1 U0
		psllw       xmm5, 0x06;         // 总共放大 7 位
		psllw       xmm1, 0x05;         // 放大 5 位
		psubw       xmm5, xmm4;         // - 128
		movhlps     xmm4, xmm5;         // @ @ @ @ V2 V2 V1 V0
		mov         eax, pnR_V;
		mov         ebx, pnG_V;
		mov         ecx, pnG_U;
		mov         edx, pnB_U;
		movdqu		xmm2, [eax];
		movdqu		xmm3, [ebx];
		movdqu		xmm6, [ecx];
		movdqu		xmm7, [edx];
		pmulhw		xmm2, xmm4;         // 还放大 5 位
		pmulhw		xmm3, xmm4;
		pmulhw		xmm6, xmm5;
		pmulhw		xmm7, xmm5;
		movdqa      xmm4, gc_nRoundComp_5;
		movdqa      xmm5, gc_dwMaskWLB; // 255
		paddw       xmm2, xmm1;         // R
		paddw       xmm3, xmm1;         // G
		paddw       xmm6, xmm4;
		paddw       xmm7, xmm1;         // B
		paddw       xmm2, xmm4;         // R
		paddw       xmm3, xmm6;         // G
		paddw       xmm7, xmm4;         // B
		pmaxsw		xmm2, xmm0;			// R
		pmaxsw		xmm3, xmm0;			// G
		pmaxsw		xmm7, xmm0;			// B
		psrlw		xmm2, 0x05;
		psrlw		xmm3, 0x05;
		psrlw		xmm7, 0x05;
		pminsw		xmm2, xmm5;		// @ @ @ @ R2 R2 R1 R0
		pminsw		xmm3, xmm5;		// @ @ @ @ G2 G2 G1 G0
		pminsw		xmm7, xmm5;		// @ @ @ @ B2 B2 B1 B0
		psllw		xmm3, 0x08;
		pxor        xmm4, xmm4;
		por			xmm2, xmm3;		// @ @ @ @ G2R2 G2R2 G1R1 G0R0
		movdqa		xmm1, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm2, xmm0;			// 00 G2R2 00 G2R2 00 G1R1 00 G0R0
		punpcklwd	xmm4, xmm7;			// 0B2 00 0B2 00 0B1 00 0B0 00
		por			xmm2, xmm1;
		por			xmm2, xmm4;     // D2 D2 D1 D1
		movhlps     xmm3, xmm2;     // @ @ D2 D2
		movsd		[edi], xmm2;
		add         edi, 0x08;
		movss		[edi], xmm3;
		jmp         loop_end;
loop_m2:
		test        ecx, 0x02;
		jz          loop_m1;
		xor         eax, eax;
		xor         ecx, ecx;
		mov         al, [edx];      // 000 U0
		mov         cl, [esi];      // 000 V0
		mov         ah, al;         // 00 U0 U0
		mov         ch, cl;         // 00 V0 V0
		pxor        xmm1, xmm1;
		pxor        xmm7, xmm7;
		pinsrw      xmm1, [ebx], 0x00;  // 0000  0000  0000  00 Y1 Y0
		pinsrw      xmm7, eax, 0x00;    // 0000  0000  0000  00 U1 U0
		pinsrw      xmm7, ecx, 0x02;     // 0 0 0 0 0 0 Y1 Y0
		punpcklbw   xmm7, xmm0;         // 0 0 V1 V0 0 0 U1 U0
		psllw       xmm1, 0x05;         // 放大 5 位
		psllw       xmm7, 0x07;         // 总共放大 7 位
		psubw       xmm7, gc_nOff128_7; // - 128
		movhlps     xmm4, xmm7;         // @ @ @ @ @ @ V1 V0
		mov         eax, pnR_V;
		mov         ebx, pnG_V;
		mov         ecx, pnG_U;
		mov         edx, pnB_U;
		movdqu		xmm2, [eax];
		movdqu		xmm3, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [edx];
		pmulhw		xmm2, xmm4;         // 还放大 5 位
		pmulhw		xmm3, xmm4;
		pmulhw		xmm5, xmm7;
		pmulhw		xmm6, xmm7;
		movdqa      xmm4, gc_nRoundComp_5;
		movdqa      xmm7, gc_dwMaskWLB; // 255
		paddw       xmm2, xmm1;         // R
		paddw       xmm3, xmm1;         // G
		paddw       xmm5, xmm4;
		paddw       xmm6, xmm1;         // B
		paddw       xmm2, xmm4;         // R
		paddw       xmm3, xmm5;         // G
		paddw       xmm6, xmm4;         // B
		pmaxsw		xmm2, xmm0;			// R
		pmaxsw		xmm3, xmm0;			// G
		pmaxsw		xmm6, xmm0;			// B
		psrlw		xmm2, 0x05;
		psrlw		xmm3, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm2, xmm7;		// @ @ @ @ @ @ R1 R0
		pminsw		xmm3, xmm7;		// @ @ @ @ @ @ G1 G0
		pminsw		xmm6, xmm7;		// @ @ @ @ @ @ B1 B0
		psllw		xmm3, 0x08;
		pxor        xmm4, xmm4;
		por			xmm2, xmm3;		// @ @ @ @ @ @ G1R1 G0R0
		movdqa		xmm1, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm2, xmm0;			// 00 @ 00 @ 00 G1R1 00 G0R0
		punpcklwd	xmm4, xmm6;			// 0@ 00 @ 00 0B1 00 0B0 00
		por			xmm2, xmm1;
		por			xmm2, xmm4;     // @ @ D1 D1
		movsd		[edi], xmm2;
		jmp         loop_end;
loop_m1:
		test        ecx, 0x01;
		jz          loop_end;
		xor         eax, eax;
		xor         ecx, ecx;
		mov         al, [ebx];      // 000 Y
		mov         cl, [esi];      // 000 V
		xor         ebx, ebx;
		mov         bl, [edx];      // 000 U
		pxor        xmm1, xmm1;
		pxor        xmm7, xmm7;
		pinsrw      xmm1, eax, 0x00;    // 0000  0000  0000  000 Y
		pinsrw      xmm7, ebx, 0x00;    // 0000  0000  0000  000 U
		pinsrw      xmm7, ecx, 0x04;    // 0000  000 V  0000  000 U
		psllw       xmm1, 0x05;
		psllw       xmm7, 0x07;     // 放大 7 位
		psubw       xmm7, gc_nOff128_7; // - 128
		mov         eax, pnR_V;
		mov         ebx, pnG_V;
		mov         ecx, pnG_U;
		mov         edx, pnB_U;
		movhlps     xmm4, xmm7;     // @ @ @ @ @ @ @ V
		movdqu		xmm2, [eax];
		movdqu		xmm3, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [edx];
		pmulhw		xmm2, xmm4;         // 还放大 5 位
		pmulhw		xmm3, xmm4;
		pmulhw		xmm5, xmm7;
		pmulhw		xmm6, xmm7;
		movdqa      xmm4, gc_nRoundComp_5;
		movdqa      xmm7, gc_dwMaskWLB; // 255
		paddw       xmm2, xmm1;         // R
		paddw       xmm3, xmm1;         // G
		paddw       xmm5, xmm4;
		paddw       xmm6, xmm1;         // B
		paddw       xmm2, xmm4;         // R
		paddw       xmm3, xmm5;         // G
		paddw       xmm6, xmm4;         // B
		pmaxsw		xmm2, xmm0;			// R
		pmaxsw		xmm3, xmm0;			// G
		pmaxsw		xmm6, xmm0;			// B
		psrlw		xmm2, 0x05;
		psrlw		xmm3, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm2, xmm7;		// @ @ @ @ @ @ @ R
		pminsw		xmm3, xmm7;		// @ @ @ @ @ @ @ G
		pminsw		xmm6, xmm7;		// @ @ @ @ @ @ @ B
		psllw		xmm3, 0x08;
		pxor        xmm4, xmm4;
		por			xmm2, xmm3;		// @ @ @ @ @ @ @ GR
		movdqa		xmm1, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm2, xmm0;			// 00 @ 00 @ 00 @ 00 GR
		punpcklwd	xmm4, xmm6;			// 0@ 00 @ 00 @ 00 0B 00
		por			xmm2, xmm1;
		por			xmm2, xmm4;     // @ @ @ D
		movss		[edi], xmm2;
loop_end:
	}
}

// 保留 Alpha 通道，直接丢弃第二个 UV
void color32_to_yuyva_line(OUT void* yuyv,		// 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   OUT void* alpha,	    // 目标 alpha 缓存，存放方式 A0 A1 A2 A3 ……
						   IN void* color32,	// 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN int na16,	    	// 源字节宽度除以 16 的商与余数
						   IN int nm16,
						   IN BOOL bHD,         // 高标清标志
						   IN BOOL bBGRA)       // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			color32_to_yuyva_inner(yuyv, alpha, color32, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
		}
		else    // R 在低位
		{
			color32_to_yuyva_inner(yuyv, alpha, color32, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
		}
	}
	else    // 标清
	{
		if (bBGRA)      // B 在低位
		{
			color32_to_yuyva_inner(yuyv, alpha, color32, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
		}
		else    // R 在低位
		{
			color32_to_yuyva_inner(yuyv, alpha, color32, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
		}
	}
}
void color32_to_yuyva_inner(OUT void* yuyv,	        // 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							OUT void* alpha,    	// 目标 alpha 缓存，存放方式 A0 A1 A2 A3 ……
							IN void* color32,	    // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
							IN int na16,    		// 源字节宽度除以 16 的商与余数
							IN int nm16,
							IN int* pnY32_BR,       // 系数（通过变更 R 分量与 B 分量的系数位置可以做到 BGRA 与 RGBA 的兼容）
							IN int* pnY32_G,        // BR 表示 R 在低位
							IN int* pnY32_BGR,
							IN int* pnUV_BGR,
							IN int* pnU_BGR,
							IN int* pnV_BGR)
{
	_asm{       // 程序内部以 RGBA 颜色为例做说明
		mov			edi, yuyv;
		mov			esi, color32;
		mov			ecx, na16;
		mov			edx, alpha;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m8;
loop_a16:
		mov         eax, pnU_BGR;
		mov         ebx, pnV_BGR;
		movdqu		xmm7, [esi];		// ABGR ABGR ABGR ABGR
		movdqu		xmm4, [eax];	    // U
		movdqu		xmm5, [ebx];  	    // V
		mov         eax, pnY32_BR;
		mov         ebx, pnY32_G;
		movdqu		xmm6, [eax];   	// Y
		movdqa		xmm1, xmm7;
		movdqa		xmm2, xmm7;
		movdqa		xmm3, xmm7;
		pxor		xmm0, xmm0;
		shufps		xmm7, xmm7, 0x08;	// 取 U0V0 与 U2V2
		pand		xmm1, gc_dwMaskWLB;	// 0B0R 0B0R 0B0R 0B0R
		pand		xmm2, gc_dwMaskWHB;	// A0G0 A0G0 A0G0 A0G0
		punpcklbw	xmm0, xmm7;			// A0B0 G0R0 A0B0 G0R0
		psrld		xmm3, 0x18;			// 000A 000A 000A 000A
		psllw		xmm1, 0x07;			// 放大 7 位
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		packssdw	xmm3, xmm3;			// 0A0A 0A0A 0A0A 0A0A
		movdqu		xmm7, [ebx];
		pmulhw		xmm6, xmm1;			// 还放大 5 位
		pmulhw		xmm7, xmm2;
		pmulhw		xmm4, xmm0;
		pmulhw		xmm5, xmm0;
		packuswb	xmm3, xmm3;			// AAAA AAAA AAAA AAAA
		paddsw		xmm7, xmm6;			// YR + YG
		pshufd		xmm1, xmm4, 0xb1;
		pshufd		xmm2, xmm5, 0xb1;
		psrld		xmm6, 0x10;			// YB
		movss		[edx], xmm3;		// 保存 Alpha
		paddsw		xmm1, xmm4;			// UR + UB
		paddsw		xmm2, xmm5;			// VR + VB
		paddsw		xmm7, xmm6;			// Y
		movdqa		xmm0, gc_nRoundComp_5;
		movdqa		xmm6, gc_nOff128_5;
		movdqa		xmm3, gc_dwMaskDLB;
		psrld		xmm4, 0x10;
		psrld		xmm5, 0x10;
		paddsw		xmm1, xmm0;
		paddsw		xmm2, xmm0;
		paddsw		xmm7, xmm0;
		paddsw		xmm4, xmm6;
		paddsw		xmm5, xmm6;
		paddsw		xmm1, xmm4;
		paddsw		xmm2, xmm5;
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm7, 0x05;
		shufps		xmm1, xmm1, 0x88;	// U2 U0 U2 U0
		shufps		xmm2, xmm2, 0x88;	// V2 V0 V2 V0
		pminsw		xmm7, xmm3;
		pminsw		xmm1, xmm3;
		pminsw		xmm2, xmm3;
		pand		xmm7, xmm3;			// 0 Y3 0 Y2 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 U2 0 U0 0 U2 0 U0
		pand		xmm2, xmm3;			// 0 V2 0 V0 0 V2 0 V0
		packssdw	xmm7, xmm7;			// Y3Y2 Y1Y0 Y3Y2 Y1Y0
		pslld		xmm1, 0x08;			// 00 U2 0  00 U0 0  00 U2 0  00 U0 0
		pslld		xmm2, 0x18;			// V2 000  V0 000  V2 000  V0 000
		por			xmm7, xmm1;			// 0 Y3U2Y2  0 Y1U0Y0  0 Y3U2Y2  0 Y1U0Y0
		por			xmm7, xmm2;			// V2Y3U2Y2 V0Y1U0Y0 V2Y3U2Y2 V0Y1U0Y0
		movsd		[edi], xmm7;
		add			esi, 0x10;
		add			edx, 0x04;
		add			edi, 0x08;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_m8:
		mov			ecx, nm16;
		mov         eax, pnY32_BGR;
		mov         ebx, pnUV_BGR;
		test		ecx, 0x08;
		jz			loop_m4;
		movsd		xmm7, [esi];		// 0000 0000 ABGR ABGR
		movdqa		xmm2, gc_nRoundComp_5;
		movdqa		xmm3, gc_dwMaskDLB;
		pshufd		xmm4, xmm7, 0x00;
		pxor		xmm0, xmm0;
		pxor		xmm1, xmm1;
		movdqu		xmm5, [eax];	    // Y
		movdqu		xmm6, [ebx]; 	    // UV
		punpcklbw	xmm0, xmm4;			// A0B0 G0R0 A0B0 G0R0
		punpcklbw	xmm1, xmm7;			// A0B0 G0R0 A0B0 G0R0
		psrld		xmm7, 0x18;			// 0000 0000 000A 000A
		psrlw		xmm0, 0x01;			// 放大 7 位
		psrlw		xmm1, 0x01;			// 放大 7 位
		packssdw	xmm7, xmm7;			// 0000 0A0A 0000 0A0A
		pmulhw		xmm5, xmm1;			// 还放大 5 位
		pmulhw		xmm6, xmm0;
		packuswb	xmm7, xmm7;			// 0000 00AA 0000 00AA
		pshufd		xmm0, xmm5, 0xb1;
		pshufd		xmm1, xmm6, 0xb1;
		paddsw		xmm0, xmm5;			// YR + YB
		paddsw		xmm1, xmm6;			// VR + VB  UR + UB
		psrld		xmm5, 0x10;
		psrld		xmm6, 0x10;
		paddsw		xmm0, xmm2;
		paddsw		xmm1, xmm2;
		paddsw		xmm6, gc_nOff128_5;
		paddsw		xmm0, xmm5;			// Y
		paddsw		xmm1, xmm6;			// U V
		psrlw		xmm0, 0x05;
		psrlw		xmm1, 0x05;
		pminsw		xmm0, xmm3;
		pminsw		xmm1, xmm3;
		shufps		xmm0, xmm0, 0x88;	// Y1 Y0 Y1 Y0
		shufps		xmm1, xmm1, 0x88;	// V0 U0 V0 U0
		pand		xmm0, xmm3;			// 0 Y1 0 Y0 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 V0 0 U0 0 V0 0 U0
		packssdw	xmm0, xmm0;			// Y1Y0 Y1Y0 Y1Y0 Y1Y0
		packssdw	xmm1, xmm1;			// V0U0 V0U0 V0U0 V0U0
		pslld		xmm1, 0x08;
		movd		eax, xmm7;			// 00AA
		por			xmm0, xmm1;			// V0Y1U0Y0
		mov			[edx], ax;
		movss		[edi], xmm0;
		add			esi, 0x08;
		add			edi, 0x04;
		add			edx, 0x02;
loop_m4:
		test		ecx, 0x04;
		jz			loop_end;
		movss		xmm7, [esi];		// 0000 0000 0000 ABGR
		movdqa		xmm2, gc_nRoundComp_5;
		movdqa		xmm3, gc_dwMaskDLB;
		pshufd		xmm4, xmm7, 0x00;
		pxor		xmm0, xmm0;
		movdqu		xmm5, [eax];  	    // Y
		movdqu		xmm6, [ebx];        // UV
		punpcklbw	xmm0, xmm4;			// A0B0 G0R0 A0B0 G0R0
		psrld		xmm7, 0x18;			// 0000 0000 0000 000A
		psrld		xmm4, 0x10;
		psrlw		xmm0, 0x01;			// 放大 7 位
		pand		xmm4, xmm3;			// 00A0 00A0 00A0 00A0
		pmulhw		xmm5, xmm0;			// 还放大 5 位
		pmulhw		xmm6, xmm0;
		por			xmm7, xmm4;			// 00A0 00A0 00A0 00AA
		pshufd		xmm0, xmm5, 0xb1;
		pshufd		xmm1, xmm6, 0xb1;
		paddsw		xmm0, xmm5;			// YR + YB
		paddsw		xmm1, xmm6;			// VR + VB  UR + UB
		psrld		xmm5, 0x10;
		psrld		xmm6, 0x10;
		paddsw		xmm0, xmm2;
		paddsw		xmm1, xmm2;
		paddsw		xmm6, gc_nOff128_5;
		paddsw		xmm0, xmm5;			// Y
		paddsw		xmm1, xmm6;			// U V
		psrlw		xmm0, 0x05;
		psrlw		xmm1, 0x05;
		pminsw		xmm0, xmm3;
		pminsw		xmm1, xmm3;
		shufps		xmm0, xmm0, 0x88;	// Y1 Y0 Y1 Y0
		shufps		xmm1, xmm1, 0x88;	// V0 U0 V0 U0
		pand		xmm0, xmm3;			// 0 Y1 0 Y0 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 V0 0 U0 0 V0 0 U0
		packssdw	xmm0, xmm0;			// Y1Y0 Y1Y0 Y1Y0 Y1Y0
		packssdw	xmm1, xmm1;			// V0U0 V0U0 V0U0 V0U0
		pslld		xmm1, 0x08;
		movd		eax, xmm7;			// 00AA
		por			xmm0, xmm1;			// V0Y1U0Y0
		mov			[edx], ax;
		movss		[edi], xmm0;
loop_end:
	}
}
// 丢弃 Alpha 通道，直接丢弃第二个 UV
void color32_to_yuyv_line(OUT void* yuyv,		// 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						  IN void* color32,	    // 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						  IN int na16,	    	// 源字节宽度除以 16 的商与余数
						  IN int nm16,
						  IN BOOL bHD,          // 高标清标志
						  IN BOOL bBGRA)        // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			color32_to_yuyv_inner(yuyv, color32, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
		}
		else        // R 在低位
		{
			color32_to_yuyv_inner(yuyv, color32, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
		}
	}
	else
	{
		if (bBGRA)      // B 在低位
		{
			color32_to_yuyv_inner(yuyv, color32, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
		}
		else        // R 在低位
		{
			color32_to_yuyv_inner(yuyv, color32, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
		}
	}
}
void color32_to_yuyv_inner(OUT void* yuyv, 	    // 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN void* color32,	// 源 *g@a 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN int na16,		    // 源字节宽度除以 16 的商与余数
						   IN int nm16,
						   IN int* pnY32_BR,    // 系数（通过变更 R 分量与 B 分量的系数位置可以做到 BGRA 与 RGBA 的兼容）
						   IN int* pnY32_G,     // BR 表示 R 在低位
						   IN int* pnY32_BGR,
						   IN int* pnUV_BGR,
						   IN int* pnU_BGR,
						   IN int* pnV_BGR)
{
	_asm{       // 程序内部以 RGBA 为例做说明
		mov			ecx, na16;
		mov			edi, yuyv;
		mov			esi, color32;
		mov         eax, pnY32_BR;
		mov         ebx, pnY32_G;
		mov         edx, pnV_BGR;
		movdqa		xmm3, gc_dwMaskDLB;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m8;
loop_a16:
		movdqu		xmm7, [esi];		// ABGR ABGR ABGR ABGR
		movdqu		xmm6, [eax];   	    // Y
		movdqa		xmm1, xmm7;
		movdqa		xmm2, xmm7;
		pxor		xmm0, xmm0;
		shufps		xmm7, xmm7, 0x08;	// 取 U0V0 与 U2V2
		pand		xmm1, gc_dwMaskWLB;	// 0B0R 0B0R 0B0R 0B0R
		pand		xmm2, gc_dwMaskWHB;	// A0G0 A0G0 A0G0 A0G0
		punpcklbw	xmm0, xmm7;			// A0B0 G0R0 A0B0 G0R0
		psllw		xmm1, 0x07;			// 放大 7 位
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		push        ecx;
		mov         ecx, pnU_BGR;
		movdqu		xmm7, [ebx];
		movdqu		xmm4, [ecx];  	    // U
		movdqu		xmm5, [edx];  	    // V
		pop         ecx;
		pmulhw		xmm6, xmm1;			// 还放大 5 位
		pmulhw		xmm7, xmm2;
		pmulhw		xmm4, xmm0;
		pmulhw		xmm5, xmm0;
		paddsw		xmm7, xmm6;			// YR + YG
		pshufd		xmm1, xmm4, 0xb1;
		pshufd		xmm2, xmm5, 0xb1;
		psrld		xmm6, 0x10;			// YB
		paddsw		xmm1, xmm4;			// UR + UB
		paddsw		xmm2, xmm5;			// VR + VB
		paddsw		xmm7, xmm6;			// Y
		movdqa		xmm0, gc_nRoundComp_5;
		movdqa		xmm6, gc_nOff128_5;
		psrld		xmm4, 0x10;
		psrld		xmm5, 0x10;
		paddsw		xmm1, xmm0;
		paddsw		xmm2, xmm0;
		paddsw		xmm7, xmm0;
		paddsw		xmm4, xmm6;
		paddsw		xmm5, xmm6;
		paddsw		xmm1, xmm4;
		paddsw		xmm2, xmm5;
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm7, 0x05;
		shufps		xmm1, xmm1, 0x88;	// U2 U0 U2 U0
		shufps		xmm2, xmm2, 0x88;	// V2 V0 V2 V0
		pminsw		xmm7, xmm3;
		pminsw		xmm1, xmm3;
		pminsw		xmm2, xmm3;
		pand		xmm7, xmm3;			// 0 Y3 0 Y2 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 U2 0 U0 0 U2 0 U0
		pand		xmm2, xmm3;			// 0 V2 0 V0 0 V2 0 V0
		packssdw	xmm7, xmm7;			// Y3Y2 Y1Y0 Y3Y2 Y1Y0
		pslld		xmm1, 0x08;			// 00 U2 0  00 U0 0  00 U2 0  00 U0 0
		pslld		xmm2, 0x18;			// V2 000  V0 000  V2 000  V0 000
		por			xmm7, xmm1;			// 0 Y3U2Y2  0 Y1U0Y0  0 Y3U2Y2  0 Y1U0Y0
		por			xmm7, xmm2;			// V2Y3U2Y2 V0Y1U0Y0 V2Y3U2Y2 V0Y1U0Y0
		movsd		[edi], xmm7;
		add			esi, 0x10;
		add			edi, 0x08;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_m8:
		mov			ecx, nm16;
		mov         eax, pnY32_BGR;
		mov         edx, pnUV_BGR;
		test		ecx, 0x08;
		jz			loop_m4;
		movsd		xmm7, [esi];		// 0000 0000 ABGR ABGR
		movdqa		xmm2, gc_nRoundComp_5;
		movdqu		xmm5, [eax];        // Y
		movdqu		xmm6, [edx];	    // UV
		pxor		xmm0, xmm0;
		pxor		xmm1, xmm1;
		punpcklbw	xmm0, xmm7;			// A0B0 G0R0 A0B0 G0R0
		shufps		xmm7, xmm7, 0x00;
		punpcklbw	xmm1, xmm7;			// A0B0 G0R0 A0B0 G0R0
		psrlw		xmm0, 0x01;			// 放大 7 位
		psrlw		xmm1, 0x01;			// 放大 7 位
		pmulhw		xmm5, xmm0;			// 还放大 5 位
		pmulhw		xmm6, xmm1;
		pshufd		xmm0, xmm5, 0xb1;
		pshufd		xmm1, xmm6, 0xb1;
		paddsw		xmm0, xmm5;			// YR + YB
		paddsw		xmm1, xmm6;			// VR + VB  UR + UB
		psrld		xmm5, 0x10;
		psrld		xmm6, 0x10;
		paddsw		xmm0, xmm2;
		paddsw		xmm1, xmm2;
		paddsw		xmm6, gc_nOff128_5;
		paddsw		xmm0, xmm5;			// Y
		paddsw		xmm1, xmm6;			// U V
		psrlw		xmm0, 0x05;
		psrlw		xmm1, 0x05;
		pminsw		xmm0, xmm3;
		pminsw		xmm1, xmm3;
		shufps		xmm0, xmm0, 0x88;	// Y1 Y0 Y1 Y0
		shufps		xmm1, xmm1, 0x88;	// V0 U0 V0 U0
		pand		xmm0, xmm3;			// 0 Y1 0 Y0 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 V0 0 U0 0 V0 0 U0
		packssdw	xmm0, xmm0;			// Y1Y0 Y1Y0 Y1Y0 Y1Y0
		packssdw	xmm1, xmm1;			// V0U0 V0U0 V0U0 V0U0
		pslld		xmm1, 0x08;
		por			xmm0, xmm1;			// V0Y1U0Y0
		movss		[edi], xmm0;
		add			esi, 0x08;
		add			edi, 0x04;
loop_m4:
		test		ecx, 0x04;
		jz			loop_end;
		movss		xmm7, [esi];		// 0000 0000 0000 ABGR
		movdqa		xmm2, gc_nRoundComp_5;
		movdqu		xmm5, [eax];  	    // Y
		movdqu		xmm6, [edx]; 	    // UV
		shufps		xmm7, xmm7, 0xf0;
		pxor		xmm0, xmm0;
		punpcklbw	xmm0, xmm7;			// A0B0 G0R0 A0B0 G0R0
		psrlw		xmm0, 0x01;			// 放大 7 位
		pmulhw		xmm5, xmm0;			// 还放大 5 位
		pmulhw		xmm6, xmm0;
		pshufd		xmm0, xmm5, 0xb1;
		pshufd		xmm1, xmm6, 0xb1;
		paddsw		xmm0, xmm5;			// YR + YB
		paddsw		xmm1, xmm6;			// VR + VB  UR + UB
		psrld		xmm5, 0x10;
		psrld		xmm6, 0x10;
		paddsw		xmm0, xmm2;
		paddsw		xmm1, xmm2;
		paddsw		xmm6, gc_nOff128_5;
		paddsw		xmm0, xmm5;			// Y
		paddsw		xmm1, xmm6;			// U V
		psrlw		xmm0, 0x05;
		psrlw		xmm1, 0x05;
		pminsw		xmm0, xmm3;
		pminsw		xmm1, xmm3;
		shufps		xmm0, xmm0, 0x88;	// Y1 Y0 Y1 Y0
		shufps		xmm1, xmm1, 0x88;	// V0 U0 V0 U0
		pand		xmm0, xmm3;			// 0 Y1 0 Y0 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 V0 0 U0 0 V0 0 U0
		packssdw	xmm0, xmm0;			// Y1Y0 Y1Y0 Y1Y0 Y1Y0
		packssdw	xmm1, xmm1;			// V0U0 V0U0 V0U0 V0U0
		pslld		xmm1, 0x08;
		por			xmm0, xmm1;			// V0Y1U0Y0
		movss		[edi], xmm0;
loop_end:
	}
}
// 使用 Alpha 通道，前后插值获得第二个 UV
void yuyva_to_color32_line(OUT void* color32,    // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN void* yuyv,	 	 // 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN void* alpha,		 // 源 alpha 缓存，存放方式 A0 A1 A2 A3 ……
						   IN int na16,		     // 源字节宽度减 4 后除以 16 的商与余数
						   IN int nm16,
						   IN BOOL bHD,          // 高标清标志
						   IN BOOL bBGRA)        // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			yuyva_to_bgra_inner(color32, yuyv, alpha, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
		else       // R 在低位
		{
			yuyva_to_rgba_inner(color32, yuyv, alpha, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
	}
	else    // 标清
	{
		if (bBGRA)      // B 在低位
		{
			yuyva_to_bgra_inner(color32, yuyv, alpha, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
		else       // R 在低位
		{
			yuyva_to_rgba_inner(color32, yuyv, alpha, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
	}
}
void yuyva_to_rgba_inner(OUT void* rgba,        // RGBA 缓存，存放方式 R0 G0 B0 A0 R1 G1 B1 A1 ……
						 IN void* yuyv, 		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						 IN void* alpha,		// 源 alpha 缓存，存放方式 A0 A1 A2 A3 ……
						 IN int na16,		    // 源字节宽度减 4 后除以 16 的商与余数
						 IN int nm16,
						 IN int* pnR_V,         // 系数（无法做到 RGBA 与 BGRA 的兼容）
						 IN int* pnG_U,       
						 IN int* pnG_V,
						 IN int* pnB_U)
{
	_asm{
		mov			esi, yuyv;
		mov			eax, yuyv;
		mov			edi, rgba;
		mov			ecx, na16;
		mov			edx, alpha;
		mov         ebx, pnG_V;
		add			eax, 0x10;			// 下一个 VYUV 用于 UV 插值
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// 下一个 VYUV 用于 UV 插值
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// 放大 6 位
		psrlw		xmm6, 2;			// 放大 6 位
		psllw		xmm7, 5;			// 放大 5 位
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// 减去偏移量 128
		psubsw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V8 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U8 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U8 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V8 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		push        esi;
		push        eax;
		push        ecx;
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pop         ecx;
		pop         eax;
		pop         esi;
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		movsd		xmm7, [edx];		// 0000 0000 A7A6A5A4 A3A2A1A0
		por			xmm1, xmm2;			// G7R7 G6R6 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		pxor		xmm2, xmm2;
		pxor		xmm4, xmm4;
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm1;
		punpcklbw	xmm2, xmm7;			// A7 A6 A5 A4 A3 A2 A1 A0
		punpcklwd	xmm3, xmm6;			// 0B3 00 0B2 00 0B1 00 0B0 00
		punpcklwd	xmm0, xmm5;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpckhwd	xmm1, xmm5;			// 00 G7R7 00 G6R6 00 G5R5 00 G4R4
		pxor		xmm7, xmm7;
		punpcklwd	xmm4, xmm2;			// A3 000 A2 000 A1 000 A0 000
		punpckhwd	xmm5, xmm6;			// 0B7 00 0B6 00 0B5 00 0B4 00
		punpckhwd	xmm7, xmm2;			// A7 000 A6 000 A5 000 A4 000
		por			xmm0, xmm3;
		por			xmm1, xmm5;
		por			xmm0, xmm4;
		por			xmm1, xmm7;
		movdqu		[edi], xmm0;
		add			eax, 0x10;
		add			edi, 0x10;
		add			esi, 0x10;
		movdqu		[edi], xmm1;
		add			edx, 0x08;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_m16:
		mov			ecx, nm16;
		test		ecx, 0x10;
		jz			loop_m12;
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V6 U6 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V6 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U6 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U6 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V6 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;		// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		movsd		xmm7, [edx];		// 0000 0000 A7A6A5A4 A3A2A1A0
		por			xmm1, xmm2;			// G7R7 G6R6 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		pxor		xmm2, xmm2;
		pxor		xmm4, xmm4;
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm1;
		punpcklbw	xmm2, xmm7;			// A7 A6 A5 A4 A3 A2 A1 A0
		punpcklwd	xmm3, xmm6;			// 0B3 00 0B2 00 0B1 00 0B0 00
		punpcklwd	xmm0, xmm5;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpckhwd	xmm1, xmm5;			// 00 G7R7 00 G6R6 00 G5R5 00 G4R4
		pxor		xmm7, xmm7;
		punpcklwd	xmm4, xmm2;			// A3 000 A2 000 A1 000 A0 000
		punpckhwd	xmm5, xmm6;			// 0B7 00 0B6 00 0B5 00 0B4 00
		punpckhwd	xmm7, xmm2;			// A7 000 A6 000 A5 000 A4 000
		por			xmm0, xmm3;
		por			xmm1, xmm5;
		por			xmm0, xmm4;
		por			xmm1, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		movdqu		[edi], xmm1;
		jmp			loop_end;
loop_m12:
		cmp			ecx, 0x0c;
		jnz			loop_m8;
		mov			ecx, edx;
		sub			eax, 0x08;
		add			edx, 0x04;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movss		xmm6, [eax];		// 最后两个像素 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U4 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V4 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V4 0 V4 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U4 0 U4 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U4 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V4 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U4 U4 U4 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U4 U4 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V4 V4 V4 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V4 V4 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U5 U4 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V5 V4 V5 V4 V3 V2 V1 V0
		push        ecx;
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pop         ecx;
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// R5 R4 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// G5 G4 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// B5 B4 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		movss		xmm7, [ecx];		// 0000 0000 0000 A3A2A1A0
		mov			ax, [edx];			// 00A5A4
		por			xmm1, xmm2;			// G5R5 G4R4 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		movd		xmm0, eax;			// 0000 0000 0000 00A5A4
		shufps		xmm0, xmm0, 0x01;
		pxor		xmm2, xmm2;
		pxor		xmm4, xmm4;
		pxor		xmm5, xmm5;
		por			xmm7, xmm0;			// **** **** 00A5A4 A3A2A1A0
		movdqa		xmm0, xmm1;
		punpcklbw	xmm2, xmm7;			// 0 0 A5 A4 A3 A2 A1 A0
		punpcklwd	xmm3, xmm6;			// 0B3 00 0B2 00 0B1 00 0B0 00
		punpcklwd	xmm0, xmm5;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpckhwd	xmm1, xmm5;			// 00 G5R5 00 G4R4 00 G5R5 00 G4R4
		pxor		xmm7, xmm7;
		punpcklwd	xmm4, xmm2;			// A3 000 A2 000 A1 000 A0 000
		punpckhwd	xmm5, xmm6;			// 0B5 00 0B4 00 0B5 00 0B4 00
		punpckhwd	xmm7, xmm2;			// 0000 0000 A5 000 A4 000
		por			xmm0, xmm3;
		por			xmm1, xmm5;
		por			xmm0, xmm4;
		por			xmm1, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		movsd		[edi], xmm1;
		jmp			loop_end;
loop_m8:
		test		ecx, 0x08;
		jz			loop_m4;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		pshufd		xmm2, xmm1, 0xf5;	// 0 0 0 0 V2 U2 V2 U2
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 0 0 0 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V2 0
		pslld		xmm4, 0x10;			// 0 0 0 0 U2 0 U2 0
		pslld		xmm5, 0x10;			// 0 0 0 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 0 0 0 0 V2 0 V0
		por			xmm4, xmm1;			// 0 0 0 0 U2 U2 U2 U0
		por			xmm5, xmm1;			// 0 0 0 0 U2 U2 U0 U0
		por			xmm2, xmm6;			// 0 0 0 0 V2 V2 V2 V0
		por			xmm3, xmm6;			// 0 0 0 0 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// 0 0 0 0 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// 0 0 0 0 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// 0 0 0 0 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		movss		xmm7, [edx];		// 0000 0000 0000 A3A2A1A0
		por			xmm1, xmm2;			// 00 00 00 00 G3R3 G2R2 G1R1 G0R0
		pxor		xmm2, xmm2;
		pxor		xmm4, xmm4;
		punpcklbw	xmm2, xmm7;			// 0 0 0 0 A3 A2 A1 A0
		punpcklwd	xmm1, xmm4;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpcklwd	xmm3, xmm6;			// 0B3 00 0B2 00 0B1 00 0B0 00
		punpcklwd	xmm4, xmm2;			// A3 000 A2 000 A1 000 A0 000
		por			xmm1, xmm3;
		por			xmm1, xmm4;
		movdqu		[edi], xmm1;
		jmp			loop_end;
loop_m4:
		test		ecx, 0x04;
		jz			loop_end;
		movss		xmm7, [esi];		// 0000 0000 0000 VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 0 0 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 0 0 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		movdqa		xmm3, xmm1;
		psrld		xmm1, 0x10;			// 0 0 0 0 0 0 0 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm4, xmm1;
		pmulhw		xmm2, xmm1;
		pmulhw		xmm5, xmm3;
		pmulhw		xmm6, xmm3;
		pxor		xmm1, xmm1;			// 0
		movdqa		xmm3, gc_nRoundComp_5;
		paddsw		xmm4, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm3;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm4, xmm3;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm3;			// B
		pmaxsw		xmm4, xmm1;			// R
		pmaxsw		xmm2, xmm1;			// G
		pmaxsw		xmm6, xmm1;			// B
		psrlw		xmm4, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm4, xmm0;			// 0 0 0 0 0 0 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 0 0 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 0 0 B1 B0
		psllw		xmm2, 0x08;
		mov			ax, [edx];			// 00A1A0
		por			xmm4, xmm2;			// 00 00 00 00 00 00 G1R1 G0R0
		movd		xmm7, eax;			// 0000 0000 0000 00A1A0
		pxor		xmm2, xmm2;
		pxor		xmm3, xmm3;
		punpcklbw	xmm2, xmm7;			// 0 0 0 0 0 0 A1 A0
		punpcklwd	xmm4, xmm1;			// 00 00 00 00 00 G1R1 00 G0R0
		punpcklwd	xmm3, xmm6;			// 00 00 00 00 0B1 00 0B0 00
		punpcklwd	xmm1, xmm2;			// 0000 0000  A1 000  A0 000
		por			xmm4, xmm3;
		por			xmm4, xmm1;
		movsd		[edi], xmm4;
loop_end:
	}
}
void yuyva_to_bgra_inner(OUT void* bgra,        // BGRA 缓存，存放方式 B0 G0 R0 A0 B1 G1 R1 A1 ……
						 IN void* yuyv, 		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						 IN void* alpha,		// 源 alpha 缓存，存放方式 A0 A1 A2 A3 ……
						 IN int na16,		    // 源字节宽度减 4 后除以 16 的商与余数
						 IN int nm16,
						 IN int* pnR_V,         // 系数（无法做到 RGBA 与 BGRA 的兼容）
						 IN int* pnG_U,       
						 IN int* pnG_V,
						 IN int* pnB_U)
{
	_asm{
		mov			esi, yuyv;
		mov			eax, yuyv;
		mov			edi, bgra;
		mov			ecx, na16;
		mov			edx, alpha;
		mov         ebx, pnG_V;
		add			eax, 0x10;			// 下一个 VYUV 用于 UV 插值
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// 下一个 VYUV 用于 UV 插值
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// 放大 6 位
		psrlw		xmm6, 2;			// 放大 6 位
		psllw		xmm7, 5;			// 放大 5 位
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// 减去偏移量 128
		psubsw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V8 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U8 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U8 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V8 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		push        esi;
		push        eax;
		push        ecx;
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pop         ecx;
		pop         eax;
		pop         esi;
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		movsd		xmm7, [edx];		// 0000 0000 A7A6A5A4 A3A2A1A0
		por			xmm6, xmm2;			// G7B7 G6B6 G5B5 G4B4 G3B3 G2B2 G1B1 G0B0
		pxor		xmm2, xmm2;
		pxor		xmm4, xmm4;
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm6;
		punpcklbw	xmm2, xmm7;			// A7 A6 A5 A4 A3 A2 A1 A0
		punpcklwd	xmm3, xmm1;			// 0R3 00 0R2 00 0R1 00 0R0 00
		punpcklwd	xmm0, xmm5;			// 00 G3B3 00 G2B2 00 G1B1 00 G0B0
		punpckhwd	xmm6, xmm5;			// 00 G7B7 00 G6B6 00 G5B5 00 G4B4
		pxor		xmm7, xmm7;
		punpcklwd	xmm4, xmm2;			// A3 000 A2 000 A1 000 A0 000
		punpckhwd	xmm5, xmm1;			// 0R7 00 0R6 00 0R5 00 0R4 00
		punpckhwd	xmm7, xmm2;			// A7 000 A6 000 A5 000 A4 000
		por			xmm0, xmm3;
		por			xmm6, xmm5;
		por			xmm0, xmm4;
		por			xmm6, xmm7;
		movdqu		[edi], xmm0;
		add			eax, 0x10;
		add			edi, 0x10;
		add			esi, 0x10;
		movdqu		[edi], xmm6;
		add			edx, 0x08;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_m16:
		mov			ecx, nm16;
		test		ecx, 0x10;
		jz			loop_m12;
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V6 U6 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V6 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U6 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U6 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V6 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;		// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		movsd		xmm7, [edx];		// 0000 0000 A7A6A5A4 A3A2A1A0
		por			xmm6, xmm2;			// G7B7 G6B6 G5B5 G4B4 G3B3 G2B2 G1B1 G0B0
		pxor		xmm2, xmm2;
		pxor		xmm4, xmm4;
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm6;
		punpcklbw	xmm2, xmm7;			// A7 A6 A5 A4 A3 A2 A1 A0
		punpcklwd	xmm3, xmm1;			// 0R3 00 0R2 00 0R1 00 0R0 00
		punpcklwd	xmm0, xmm5;			// 00 G3B3 00 G2B2 00 G1B1 00 G0B0
		punpckhwd	xmm6, xmm5;			// 00 G7B7 00 G6B6 00 G5B5 00 G4B4
		pxor		xmm7, xmm7;
		punpcklwd	xmm4, xmm2;			// A3 000 A2 000 A1 000 A0 000
		punpckhwd	xmm5, xmm1;			// 0R7 00 0R6 00 0R5 00 0R4 00
		punpckhwd	xmm7, xmm2;			// A7 000 A6 000 A5 000 A4 000
		por			xmm0, xmm3;
		por			xmm6, xmm5;
		por			xmm0, xmm4;
		por			xmm6, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		movdqu		[edi], xmm6;
		jmp			loop_end;
loop_m12:
		cmp			ecx, 0x0c;
		jnz			loop_m8;
		mov			ecx, edx;
		sub			eax, 0x08;
		add			edx, 0x04;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movss		xmm6, [eax];		// 最后两个像素 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U4 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V4 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V4 0 V4 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U4 0 U4 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U4 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V4 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U4 U4 U4 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U4 U4 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V4 V4 V4 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V4 V4 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U5 U4 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V5 V4 V5 V4 V3 V2 V1 V0
		push        ecx;
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pop         ecx;
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// R5 R4 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// G5 G4 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// B5 B4 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		movss		xmm7, [ecx];		// 0000 0000 0000 A3A2A1A0
		mov			ax, [edx];			// 00A5A4
		por			xmm6, xmm2;			// G5B5 G4B4 G5B5 G4B4 G3B3 G2B2 G1B1 G0B0
		movd		xmm0, eax;			// 0000 0000 0000 00A5A4
		shufps		xmm0, xmm0, 0x01;
		pxor		xmm2, xmm2;
		pxor		xmm4, xmm4;
		pxor		xmm5, xmm5;
		por			xmm7, xmm0;			// **** **** 00A5A4 A3A2A1A0
		movdqa		xmm0, xmm6;
		punpcklbw	xmm2, xmm7;			// 0 0 A5 A4 A3 A2 A1 A0
		punpcklwd	xmm3, xmm1;			// 0R3 00 0R2 00 0R1 00 0R0 00
		punpcklwd	xmm0, xmm5;			// 00 G3B3 00 G2B2 00 G1B1 00 G0B0
		punpckhwd	xmm6, xmm5;			// 00 G5B5 00 G4B4 00 G5B5 00 G4B4
		pxor		xmm7, xmm7;
		punpcklwd	xmm4, xmm2;			// A3 000 A2 000 A1 000 A0 000
		punpckhwd	xmm5, xmm1;			// 0R5 00 0R4 00 0R5 00 0R4 00
		punpckhwd	xmm7, xmm2;			// 0000 0000 A5 000 A4 000
		por			xmm0, xmm3;
		por			xmm6, xmm5;
		por			xmm0, xmm4;
		por			xmm6, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		movsd		[edi], xmm6;
		jmp			loop_end;
loop_m8:
		test		ecx, 0x08;
		jz			loop_m4;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		pshufd		xmm2, xmm1, 0xf5;	// 0 0 0 0 V2 U2 V2 U2
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 0 0 0 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V2 0
		pslld		xmm4, 0x10;			// 0 0 0 0 U2 0 U2 0
		pslld		xmm5, 0x10;			// 0 0 0 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 0 0 0 0 V2 0 V0
		por			xmm4, xmm1;			// 0 0 0 0 U2 U2 U2 U0
		por			xmm5, xmm1;			// 0 0 0 0 U2 U2 U0 U0
		por			xmm2, xmm6;			// 0 0 0 0 V2 V2 V2 V0
		por			xmm3, xmm6;			// 0 0 0 0 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// 0 0 0 0 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// 0 0 0 0 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// 0 0 0 0 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		movss		xmm7, [edx];		// 0000 0000 0000 A3A2A1A0
		por			xmm6, xmm2;			// 00 00 00 00 G3B3 G2B2 G1B1 G0B0
		pxor		xmm2, xmm2;
		pxor		xmm4, xmm4;
		punpcklbw	xmm2, xmm7;			// 0 0 0 0 A3 A2 A1 A0
		punpcklwd	xmm6, xmm4;			// 00 G3B3 00 G2B2 00 G1B1 00 G0B0
		punpcklwd	xmm3, xmm1;			// 0R3 00 0R2 00 0R1 00 0R0 00
		punpcklwd	xmm4, xmm2;			// A3 000 A2 000 A1 000 A0 000
		por			xmm6, xmm3;
		por			xmm6, xmm4;
		movdqu		[edi], xmm6;
		jmp			loop_end;
loop_m4:
		test		ecx, 0x04;
		jz			loop_end;
		movss		xmm7, [esi];		// 0000 0000 0000 VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 0 0 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 0 0 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		movdqa		xmm3, xmm1;
		psrld		xmm1, 0x10;			// 0 0 0 0 0 0 0 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm4, xmm1;
		pmulhw		xmm2, xmm1;
		pmulhw		xmm5, xmm3;
		pmulhw		xmm6, xmm3;
		pxor		xmm1, xmm1;			// 0
		movdqa		xmm3, gc_nRoundComp_5;
		paddsw		xmm4, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm3;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm4, xmm3;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm3;			// B
		pmaxsw		xmm4, xmm1;			// R
		pmaxsw		xmm2, xmm1;			// G
		pmaxsw		xmm6, xmm1;			// B
		psrlw		xmm4, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm4, xmm0;			// 0 0 0 0 0 0 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 0 0 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 0 0 B1 B0
		psllw		xmm2, 0x08;
		mov			ax, [edx];			// 00A1A0
		por			xmm6, xmm2;			// 00 00 00 00 00 00 G1B1 G0B0
		movd		xmm7, eax;			// 0000 0000 0000 00A1A0
		pxor		xmm2, xmm2;
		pxor		xmm3, xmm3;
		punpcklbw	xmm2, xmm7;			// 0 0 0 0 0 0 A1 A0
		punpcklwd	xmm6, xmm1;			// 00 00 00 00 00 G1B1 00 G0B0
		punpcklwd	xmm3, xmm4;			// 00 00 00 00 0R1 00 0R0 00
		punpcklwd	xmm1, xmm2;			// 0000 0000  A1 000  A0 000
		por			xmm6, xmm3;
		por			xmm6, xmm1;
		movsd		[edi], xmm6;
loop_end:
	}
}
// 补充 Alpha 通道为 255，前后插值获得第二个 UV
void yuyv_to_color32_line(OUT void* color32,    // *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
						  IN void* yuyv,		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						  IN int na16,			// 源字节宽度减 4 后除以 16 的商与余数
						  IN int nm16,
						  IN int bHD,           // 高标清标志
						  IN BOOL bBGRA)        // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	if (bHD)    // 高清
	{
		if (bBGRA)      // B 在低位
		{
			yuyv_to_bgra_inner(color32, yuyv, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
		else        // R 在低位
		{
			yuyv_to_rgba_inner(color32, yuyv, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
	}
	else    // 标清
	{
		if (bBGRA)      // B 在低位
		{
			yuyv_to_bgra_inner(color32, yuyv, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
		else        // R 在低位
		{
			yuyv_to_rgba_inner(color32, yuyv, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
	}
}
void yuyv_to_rgba_inner(OUT void* rgba,	        // RGBA 缓存，存放方式 R0 G0 B0 A0 R1 G1 B1 A1 ……
						IN void* yuyv, 		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						IN int na16,		    // 源字节宽度减 4 后除以 16 的商与余数
						IN int nm16,
						IN int* pnR_V,        // 系数（无法做到 RGBA 与 BGRA 的兼容）
						IN int* pnG_U,
						IN int* pnG_V,
						IN int* pnB_U)
{
	_asm{
		mov			eax, yuyv;
		mov			esi, yuyv;
		mov			ecx, na16;
		add			eax, 0x10;			// 下一个 VYUV 用于 UV 插值
		mov			edi, rgba;
		mov         ebx, pnG_V;
		mov         edx, pnG_U;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// 下一个 VYUV 用于 UV 插值
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// 放大 6 位
		psrlw		xmm6, 2;			// 放大 6 位
		psllw		xmm7, 5;			// 放大 5 位
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// 减去偏移量 128
		psubsw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V8 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U8 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U8 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V8 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		push        eax;
		push        esi;
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pop         esi;
		pop         eax;
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;		// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		por			xmm1, xmm2;			// G7R7 G6R6 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm1;
		movdqa		xmm7, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm0, xmm5;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpckhwd	xmm1, xmm5;			// 00 G7R7 00 G6R6 00 G5R5 00 G4R4
		punpcklwd	xmm3, xmm6;			// 0B3 00 0B2 00 0B1 00 0B0 00
		punpckhwd	xmm5, xmm6;			// 0B7 00 0B6 00 0B5 00 0B4 00
		por			xmm0, xmm3;
		por			xmm1, xmm5;
		por			xmm0, xmm7;
		por			xmm1, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		add			eax, 0x10;
		movdqu		[edi], xmm1;
		add			esi, 0x10;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_m16:
		mov			ecx, nm16;
		test		ecx, 0x10;
		jz			loop_m12;
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V6 U6 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V6 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U6 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U6 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V6 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;		// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		por			xmm1, xmm2;			// G7R7 G6R6 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm1;
		movdqa		xmm7, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm0, xmm5;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpckhwd	xmm1, xmm5;			// 00 G7R7 00 G6R6 00 G5R5 00 G4R4
		punpcklwd	xmm3, xmm6;			// 0B3 00 0B2 00 0B1 00 0B0 00
		punpckhwd	xmm5, xmm6;			// 0B7 00 0B6 00 0B5 00 0B4 00
		por			xmm0, xmm3;
		por			xmm1, xmm5;
		por			xmm0, xmm7;
		por			xmm1, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		movdqu		[edi], xmm1;
		jmp			loop_end;
loop_m12:
		cmp			ecx, 0x0c;
		jnz			loop_m8;
		sub			eax, 0x08;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movss		xmm6, [eax];		// 最后两个像素 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U4 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V4 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V4 0 V4 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U4 0 U4 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U4 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V4 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U4 U4 U4 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U4 U4 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V4 V4 V4 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V4 V4 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U5 U4 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V5 V4 V5 V4 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// R5 R4 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// G5 G4 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// B5 B4 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		por			xmm1, xmm2;			// G5R5 G4R4 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm1;
		movdqa		xmm7, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm0, xmm5;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpckhwd	xmm1, xmm5;			// 00 G5R5 00 G4R4 00 G5R5 00 G4R4
		punpcklwd	xmm3, xmm6;			// 0B3 00 0B2 00 0B1 00 0B0 00
		punpckhwd	xmm5, xmm6;			// 0B5 00 0B4 00 0B5 00 0B4 00
		por			xmm0, xmm3;
		por			xmm1, xmm5;
		por			xmm0, xmm7;
		por			xmm1, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		movsd		[edi], xmm1;
		jmp			loop_end;
loop_m8:
		test		ecx, 0x08;
		jz			loop_m4;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		pshufd		xmm2, xmm1, 0xf5;	// 0 0 0 0 V2 U2 V2 U2
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 0 0 0 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V2 0
		pslld		xmm4, 0x10;			// 0 0 0 0 U2 0 U2 0
		pslld		xmm5, 0x10;			// 0 0 0 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 0 0 0 0 V2 0 V0
		por			xmm4, xmm1;			// 0 0 0 0 U2 U2 U2 U0
		por			xmm5, xmm1;			// 0 0 0 0 U2 U2 U0 U0
		por			xmm2, xmm6;			// 0 0 0 0 V2 V2 V2 V0
		por			xmm3, xmm6;			// 0 0 0 0 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// 0 0 0 0 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// 0 0 0 0 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// 0 0 0 0 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		por			xmm1, xmm2;			// 00 00 00 00 G3R3 G2R2 G1R1 G0R0
		punpcklwd	xmm1, xmm3;			// 00 G3R3 00 G2R2 00 G1R1 00 G0R0
		punpcklwd	xmm3, xmm6;			// 0B3 00 0B2 00 0B1 00 0B0 00
		por			xmm1, gc_dwMaskAlpha;
		por			xmm1, xmm3;
		movdqu		[edi], xmm1;
		jmp			loop_end;
loop_m4:
		test		ecx, 0x04;
		jz			loop_end;
		movss		xmm7, [esi];		// 0000 0000 0000 VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 0 0 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 0 0 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		movdqa		xmm3, xmm1;
		psrld		xmm1, 0x10;			// 0 0 0 0 0 0 0 V0
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pmulhw		xmm4, xmm1;
		pmulhw		xmm2, xmm1;
		pmulhw		xmm5, xmm3;
		pmulhw		xmm6, xmm3;
		pxor		xmm1, xmm1;			// 0
		movdqa		xmm3, gc_nRoundComp_5;
		paddsw		xmm4, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm3;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm4, xmm3;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm3;			// B
		pmaxsw		xmm4, xmm1;			// R
		pmaxsw		xmm2, xmm1;			// G
		pmaxsw		xmm6, xmm1;			// B
		psrlw		xmm4, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm4, xmm0;			// 0 0 0 0 0 0 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 0 0 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 0 0 B1 B0
		psllw		xmm2, 0x08;
		por			xmm4, xmm2;			// 00 00 00 00 00 00 G1R1 G0R0
		punpcklwd	xmm4, xmm1;			// 00 00 00 00 00 G1R1 00 G0R0
		punpcklwd	xmm1, xmm6;			// 00 00 00 00 0B1 00 0B0 00
		por			xmm4, gc_dwMaskAlpha;
		por			xmm4, xmm1;
		movsd		[edi], xmm4;
loop_end:
	}
};
void yuyv_to_bgra_inner(OUT void* bgra,     // BGRA 缓存，存放方式 B0 G0 R0 A0 B1 G1 R1 A1 ……
						IN void* yuyv, 		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						IN int na16,		// 源字节宽度减 4 后除以 16 的商与余数
						IN int nm16,
						IN int* pnR_V,      // 系数（无法做到 RGBA 与 BGRA 的兼容）
						IN int* pnG_U,
						IN int* pnG_V,
						IN int* pnB_U)
{
	_asm{
		mov			eax, yuyv;
		mov			esi, yuyv;
		mov			ecx, na16;
		add			eax, 0x10;			// 下一个 VYUV 用于 UV 插值
		mov			edi, bgra;
		mov         ebx, pnG_V;
		mov         edx, pnG_U;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// 下一个 VYUV 用于 UV 插值
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// 放大 6 位
		psrlw		xmm6, 2;			// 放大 6 位
		psllw		xmm7, 5;			// 放大 5 位
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// 减去偏移量 128
		psubsw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V8 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U8 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U8 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V8 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		push        eax;
		push        esi;
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pop         esi;
		pop         eax;
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;		// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		por			xmm6, xmm2;		// G7B7 G6B6 G5B5 G4B4 G3B3 G2B2 G1B1 G0B0
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm6;
		movdqa		xmm7, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm0, xmm5;			// 00 G3B3 00 G2B2 00 G1B1 00 G0B0
		punpckhwd	xmm6, xmm5;			// 00 G7B7 00 G6B6 00 G5B5 00 G4B4
		punpcklwd	xmm3, xmm1;			// 0R3 00 0R2 00 0R1 00 0R0 00
		punpckhwd	xmm5, xmm1;			// 0R7 00 0R6 00 0R5 00 0R4 00
		por			xmm0, xmm3;
		por			xmm6, xmm5;
		por			xmm0, xmm7;
		por			xmm6, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		add			eax, 0x10;
		movdqu		[edi], xmm6;
		add			esi, 0x10;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_m16:
		mov			ecx, nm16;
		test		ecx, 0x10;
		jz			loop_m12;
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V6 U6 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V6 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U6 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U6 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V6 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;		// B7 B6 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		por			xmm6, xmm2;			// G7B7 G6B6 G5B5 G4B4 G3B3 G2B2 G1B1 G0B0
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm6;
		movdqa		xmm7, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm0, xmm5;			// 00 G3B3 00 G2B2 00 G1B1 00 G0B0
		punpckhwd	xmm6, xmm5;			// 00 G7B7 00 G6B6 00 G5B5 00 G4B4
		punpcklwd	xmm3, xmm1;			// 0R3 00 0R2 00 0R1 00 0R0 00
		punpckhwd	xmm5, xmm1;			// 0R7 00 0R6 00 0R5 00 0R4 00
		por			xmm0, xmm3;
		por			xmm6, xmm5;
		por			xmm0, xmm7;
		por			xmm6, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		movdqu		[edi], xmm6;
		jmp			loop_end;
loop_m12:
		cmp			ecx, 0x0c;
		jnz			loop_m8;
		sub			eax, 0x08;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movss		xmm6, [eax];		// 最后两个像素 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U4 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V4 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V4 0 V4 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U4 0 U4 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U4 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V4 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U4 U4 U4 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U4 U4 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V4 V4 V4 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V4 V4 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U5 U4 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V5 V4 V5 V4 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// R5 R4 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// G5 G4 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// B5 B4 B5 B4 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		por			xmm6, xmm2;			// G5B5 G4B4 G5B5 G4B4 G3B3 G2B2 G1B1 G0B0
		pxor		xmm5, xmm5;
		movdqa		xmm0, xmm6;
		movdqa		xmm7, gc_dwMaskAlpha;	// A000 A000 A000 A000
		punpcklwd	xmm0, xmm5;			// 00 G3B3 00 G2B2 00 G1B1 00 G0B0
		punpckhwd	xmm6, xmm5;			// 00 G5B5 00 G4B4 00 G5B5 00 G4B4
		punpcklwd	xmm3, xmm1;			// 0R3 00 0R2 00 0R1 00 0R0 00
		punpckhwd	xmm5, xmm1;			// 0R5 00 0R4 00 0R5 00 0R4 00
		por			xmm0, xmm3;
		por			xmm6, xmm5;
		por			xmm0, xmm7;
		por			xmm6, xmm7;
		movdqu		[edi], xmm0;
		add			edi, 0x10;
		movsd		[edi], xmm6;
		jmp			loop_end;
loop_m8:
		test		ecx, 0x08;
		jz			loop_m4;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		pshufd		xmm2, xmm1, 0xf5;	// 0 0 0 0 V2 U2 V2 U2
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 0 0 0 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V2 0
		pslld		xmm4, 0x10;			// 0 0 0 0 U2 0 U2 0
		pslld		xmm5, 0x10;			// 0 0 0 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 0 0 0 0 V2 0 V0
		por			xmm4, xmm1;			// 0 0 0 0 U2 U2 U2 U0
		por			xmm5, xmm1;			// 0 0 0 0 U2 U2 U0 U0
		por			xmm2, xmm6;			// 0 0 0 0 V2 V2 V2 V0
		por			xmm3, xmm6;			// 0 0 0 0 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// 0 0 0 0 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// 0 0 0 0 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// 0 0 0 0 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 B3 B2 B1 B0
		psllw		xmm2, 0x08;
		por			xmm6, xmm2;			// 00 00 00 00 G3B3 G2B2 G1B1 G0B0
		punpcklwd	xmm6, xmm3;			// 00 G3B3 00 G2B2 00 G1B1 00 G0B0
		punpcklwd	xmm3, xmm1;			// 0B3 00 0B2 00 0B1 00 0B0 00
		por			xmm6, gc_dwMaskAlpha;
		por			xmm6, xmm3;
		movdqu		[edi], xmm6;
		jmp			loop_end;
loop_m4:
		test		ecx, 0x04;
		jz			loop_end;
		movss		xmm7, [esi];		// 0000 0000 0000 VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 0 0 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 0 0 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		movdqa		xmm3, xmm1;
		psrld		xmm1, 0x10;			// 0 0 0 0 0 0 0 V0
		mov         eax, pnR_V;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [edx];
		movdqu		xmm6, [esi];
		pmulhw		xmm4, xmm1;
		pmulhw		xmm2, xmm1;
		pmulhw		xmm5, xmm3;
		pmulhw		xmm6, xmm3;
		pxor		xmm1, xmm1;			// 0
		movdqa		xmm3, gc_nRoundComp_5;
		paddsw		xmm4, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm3;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm4, xmm3;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm3;			// B
		pmaxsw		xmm4, xmm1;			// R
		pmaxsw		xmm2, xmm1;			// G
		pmaxsw		xmm6, xmm1;			// B
		psrlw		xmm4, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm4, xmm0;			// 0 0 0 0 0 0 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 0 0 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 0 0 B1 B0
		psllw		xmm2, 0x08;
		por			xmm6, xmm2;			// 00 00 00 00 00 00 G1B1 G0B0
		punpcklwd	xmm6, xmm1;			// 00 00 00 00 00 G1B1 00 G0B0
		punpcklwd	xmm1, xmm4;			// 00 00 00 00 0R1 00 0R0 00
		por			xmm6, gc_dwMaskAlpha;
		por			xmm6, xmm1;
		movsd		[edi], xmm6;
loop_end:
	}
}
// 直接丢弃第二个 UV
void color24_to_yuyv_line(OUT void* yuyv,		// 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						  IN void* color24,		// *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						  IN int na12,			// 源字节宽度减 4 后除以 12 的商
						  IN int nmp,			// 做完 na12 循环后剩余的像素
						  IN int bHD,           // 高标清标志
						  IN BOOL bBGR)         // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	if (bHD)    // 高清
	{
		if (bBGR)      // B 在低位
		{
			color24_to_yuyv_inner(yuyv, color24, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
		}
		else    // R 在低位
		{
			color24_to_yuyv_inner(yuyv, color24, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
		}
	}
	else    // 标清
	{
		if (bBGR)      // B 在低位
		{
			color24_to_yuyv_inner(yuyv, color24, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
		}
		else    // R 在低位
		{
			color24_to_yuyv_inner(yuyv, color24, na12, nmp, g_nY24_RGB_SD, g_nY32_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD, g_nUV_BGR_SD);
		}
	}
}
void color24_to_yuyv_inner(OUT void* yuyv,		// 目标 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN void* color24,	// *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN int na12,			// 源字节宽度减 4 后除以 12 的商
						   IN int nmp,			// 做完 na12 循环后剩余的像素'
						   IN int* pnY24_RGB,   // 系数（通过变更 R 分量与 B 分量的系数位置可以做到 BGRA 与 RGBA 的兼容）
						   IN int* pnY32_BGR,
						   IN int* pnU_BGR,
						   IN int* pnV_BGR,
						   IN int* pnUV_BGR)
{
	_asm{       // 程序内部以 RGB 为例进行说明
		mov			ecx, na12;
		mov			edi, yuyv;
		mov			esi, color24;
		mov         eax, pnU_BGR;
		mov         ebx, pnV_BGR;
		mov         edx, pnY24_RGB;
		test		ecx, ecx;
		jnz			loop_a12;
		jmp			loop_mp4;
loop_a12:		// 只做前四个像素
		movdqu		xmm7, [esi];		// RBGR BGRB GRBG RBGR
		movdqu		xmm4, [eax];      	// U
		movdqu		xmm5, [ebx];  	    // V
		movdqu		xmm6, [edx];  	    // YrYg YbYr YgYb YrYg
		movdqa		xmm1, xmm7;
		movdqa		xmm2, xmm7;
		movdqa		xmm3, xmm7;			// 为了取 U0V0 和 U2V2
		pxor		xmm0, xmm0;
		pand		xmm1, gc_dwMaskWLB;	// ** G3B2 R2G1 B0R0
		pand		xmm2, gc_dwMaskWHB;	// ** B3R3 G2B1 R1G0
		punpcklbw	xmm0, xmm7;			// G2R2 ** *B0 G0R0
		pslld		xmm3, 0x08;			// ** *B2 ** **
		pshufd		xmm7, xmm6, 0x12;	// YrYg YgYb YrYg YbYr
		shufps		xmm3, xmm0, 0xfa;	// G2R2 G2R2 *B2 *B2
		shufps		xmm0, xmm3, 0x24;	// *B2 G2R2 *B0 G0R0
		psllw		xmm1, 0x07;			// 放大 7 位
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		pmulhw		xmm6, xmm2;			// 还放大 5 位
		pmulhw		xmm7, xmm1;			// * *  YG3 YB2  YR2 YG1  YB0 YR0
		pmulhw		xmm4, xmm0;
		pmulhw		xmm5, xmm0;
		movdqa		xmm2, xmm6;			// * *  YB3 YR3  YG2 YB1  YR1 YG0
		paddsw		xmm6, xmm7;			// * *  YG3+YB3 *  YR2+YG2 YG1+YB1  * YR0+YG0
		shufps		xmm2, xmm7, 0x88;	// YG3 YB2  YB0 YR0  YB3 YR3  YR1 YG0
		movdqa		xmm3, xmm6;
		movdqa		xmm7, xmm2;
		pand		xmm2, gc_dwMaskDLW;	// 0 YB2  0 *  0 YR3  0 *
		psrld		xmm3, 0x10;			// 0 *  0 YG3+YB3  0 YR2+YG2  0 *
		psrld		xmm7, 0x10;			// 0 *  0 YB0  * 0  0 YR1
		shufps		xmm6, xmm3, 0x94;	// 0 YG3+YB3  0 YR2+YG2  * YG1+YB1  * YR0+YG0
		shufps		xmm7, xmm2, 0x72;	// 0 YR3  0 YB2  0 YR1  0 YB0
		pshufd		xmm1, xmm4, 0xb1;
		pshufd		xmm2, xmm5, 0xb1;
		paddsw		xmm7, xmm6;			// 0 Y3  0 Y2  * Y1  * Y0
		paddsw		xmm1, xmm4;			// UR + UB
		paddsw		xmm2, xmm5;			// VR + VB
		movdqa		xmm0, gc_nRoundComp_5;
		movdqa		xmm3, gc_dwMaskDLB;
		movdqa		xmm6, gc_nOff128_5;
		psrld		xmm4, 0x10;
		psrld		xmm5, 0x10;
		paddsw		xmm1, xmm0;
		paddsw		xmm2, xmm0;
		paddsw		xmm7, xmm0;
		paddsw		xmm4, xmm6;
		paddsw		xmm5, xmm6;
		paddsw		xmm1, xmm4;
		paddsw		xmm2, xmm5;
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm7, 0x05;
		shufps		xmm1, xmm1, 0x88;	// U2 U0 U2 U0
		shufps		xmm2, xmm2, 0x88;	// V2 V0 V2 V0
		pminsw		xmm7, xmm3;
		pminsw		xmm1, xmm3;
		pminsw		xmm2, xmm3;
		pand		xmm7, xmm3;			// 0 Y3 0 Y2 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 U2 0 U0 0 U2 0 U0
		pand		xmm2, xmm3;			// 0 V2 0 V0 0 V2 0 V0
		packssdw	xmm7, xmm7;			// Y3Y2 Y1Y0 Y3Y2 Y1Y0
		pslld		xmm1, 0x08;			// 00 U2 0  00 U0 0  00 U2 0  00 U0 0
		pslld		xmm2, 0x18;			// V2 000  V0 000  V2 000  V0 000
		por			xmm7, xmm1;			// 0 Y3U2Y2  0 Y1U0Y0  0 Y3U2Y2  0 Y1U0Y0
		por			xmm7, xmm2;			// V2Y3U2Y2 V0Y1U0Y0 V2Y3U2Y2 V0Y1U0Y0
		movsd		[edi], xmm7;
		add			esi, 0x0c;			// 12 字节
		add			edi, 0x08;
		sub			ecx, 0x01;
		jnz			loop_a12;
loop_mp4:		// 剩余四个像素
		mov			ecx, nmp;
		test		ecx, 0x04;
		jz			loop_mp3;
		push		eax;
		mov			eax, esi;
		add			eax, 0x08;
		movsd		xmm7, [esi];		// 0000 0000 GRBG RBGR
		movss		xmm6, [eax];		// 0000 0000 0000 BGRB
		pop			eax;
		pxor		xmm0, xmm0;
		movlhps		xmm7, xmm6;			// 0000 BGRB GRBG RBGR
		movdqu		xmm4, [eax];	    // U
		movdqu		xmm5, [ebx];	    // V
		movdqu		xmm6, [edx];	    // YrYg YbYr YgYb YrYg
		movdqa		xmm1, xmm7;
		movdqa		xmm2, xmm7;
		movdqa		xmm3, xmm7;			// 为了取 U0V0 和 U2V2
		pand		xmm1, gc_dwMaskWLB;		// ** G3B2 R2G1 B0R0
		pand		xmm2, gc_dwMaskWHB;	// ** B3R3 G2B1 R1G0
		punpcklbw	xmm0, xmm7;			// G2R2 ** *B0 G0R0
		pslld		xmm3, 0x08;			// ** *B2 ** **
		pshufd		xmm7, xmm6, 0x12;	// YrYg YgYb YrYg YbYr
		shufps		xmm3, xmm0, 0xfa;	// G2R2 G2R2 *B2 *B2
		shufps		xmm0, xmm3, 0x24;	// *B2 G2R2 *B0 G0R0
		psllw		xmm1, 0x07;			// 放大 7 位
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		pmulhw		xmm6, xmm2;			// 还放大 5 位
		pmulhw		xmm7, xmm1;			// * *  YG3 YB2  YR2 YG1  YB0 YR0
		pmulhw		xmm4, xmm0;
		pmulhw		xmm5, xmm0;
		movdqa		xmm2, xmm6;			// * *  YB3 YR3  YG2 YB1  YR1 YG0
		paddsw		xmm6, xmm7;			// * *  YG3+YB3 *  YR2+YG2 YG1+YB1  * YR0+YG0
		shufps		xmm2, xmm7, 0x88;	// YG3 YB2  YB0 YR0  YB3 YR3  YR1 YG0
		movdqa		xmm3, xmm6;
		movdqa		xmm7, xmm2;
		pand		xmm2, gc_dwMaskDLW;	// 0 YB2  0 *  0 YR3  0 *
		psrld		xmm3, 0x10;			// 0 *  0 YG3+YB3  0 YR2+YG2  0 *
		psrld		xmm7, 0x10;			// 0 *  0 YB0  * 0  0 YR1
		shufps		xmm6, xmm3, 0x94;	// 0 YG3+YB3  0 YR2+YG2  * YG1+YB1  * YR0+YG0
		shufps		xmm7, xmm2, 0x72;	// 0 YR3  0 YB2  0 YR1  0 YB0
		pshufd		xmm1, xmm4, 0xb1;
		pshufd		xmm2, xmm5, 0xb1;
		paddsw		xmm7, xmm6;			// 0 Y3  0 Y2  * Y1  * Y0
		paddsw		xmm1, xmm4;			// UR + UB
		paddsw		xmm2, xmm5;			// VR + VB
		movdqa		xmm0, gc_nRoundComp_5;
		movdqa		xmm3, gc_dwMaskDLB;
		movdqa		xmm6, gc_nOff128_5;
		psrld		xmm4, 0x10;
		psrld		xmm5, 0x10;
		paddsw		xmm1, xmm0;
		paddsw		xmm2, xmm0;
		paddsw		xmm7, xmm0;
		paddsw		xmm4, xmm6;
		paddsw		xmm5, xmm6;
		paddsw		xmm1, xmm4;
		paddsw		xmm2, xmm5;
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm7, 0x05;
		shufps		xmm1, xmm1, 0x88;	// U2 U0 U2 U0
		shufps		xmm2, xmm2, 0x88;	// V2 V0 V2 V0
		pminsw		xmm7, xmm3;
		pminsw		xmm1, xmm3;
		pminsw		xmm2, xmm3;
		pand		xmm7, xmm3;			// 0 Y3 0 Y2 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 U2 0 U0 0 U2 0 U0
		pand		xmm2, xmm3;			// 0 V2 0 V0 0 V2 0 V0
		packssdw	xmm7, xmm7;			// Y3Y2 Y1Y0 Y3Y2 Y1Y0
		pslld		xmm1, 0x08;			// 00 U2 0  00 U0 0  00 U2 0  00 U0 0
		pslld		xmm2, 0x18;			// V2 000  V0 000  V2 000  V0 000
		por			xmm7, xmm1;			// 0 Y3U2Y2  0 Y1U0Y0  0 Y3U2Y2  0 Y1U0Y0
		por			xmm7, xmm2;			// V2Y3U2Y2 V0Y1U0Y0 V2Y3U2Y2 V0Y1U0Y0
		movsd		[edi], xmm7;
		jmp			loop_end;
loop_mp3:		// 剩余三个像素
		cmp			ecx, 0x03;
		jnz			loop_mp2;
		push		eax;
		mov			eax, esi;
		add			eax, 0x08;
		movsd		xmm7, [esi];		// 0000 0000 GRBG RBGR
		movss		xmm6, [eax];		// 0000 0000 0000 000B
		pop			eax;
		pshufd		xmm0, xmm7, 0x9a;	// 0000 GRBG 0000 0000
		shufps		xmm6, xmm6, 0x45;	// 0000 000B 0000 0000
		pand		xmm0, gc_dwMaskDHW;	// 0000 GR00 0000 0000
		pand		xmm6, gc_dwMaskDLB;
		movdqa		xmm1, xmm6;
		pslld		xmm0, 0x08;			// 0000 0GR0 0000 0000
		pslld		xmm1, 0x18;			// 0000 B000 0000 0000
		por			xmm7, xmm6;
		por			xmm0, xmm1;
		por			xmm7, xmm0;			// 0000 BGRB GRBG RBGR
		movdqu		xmm4, [eax];  	    // U
		movdqu		xmm5, [ebx];  	    // V
		movdqu		xmm6, [edx];  	    // YrYg YbYr YgYb YrYg
		pxor		xmm0, xmm0;
		movdqa		xmm1, xmm7;
		movdqa		xmm2, xmm7;
		movdqa		xmm3, xmm7;			// 为了取 U0V0 和 U2V2
		pand		xmm1, gc_dwMaskWLB;		// ** G3B2 R2G1 B0R0
		pand		xmm2, gc_dwMaskWHB;	// ** B3R3 G2B1 R1G0
		punpcklbw	xmm0, xmm7;			// G2R2 ** *B0 G0R0
		pslld		xmm3, 0x08;			// ** *B2 ** **
		pshufd		xmm7, xmm6, 0x12;	// YrYg YgYb YrYg YbYr
		shufps		xmm3, xmm0, 0xfa;	// G2R2 G2R2 *B2 *B2
		shufps		xmm0, xmm3, 0x24;	// *B2 G2R2 *B0 G0R0
		psllw		xmm1, 0x07;			// 放大 7 位
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		pmulhw		xmm6, xmm2;			// 还放大 5 位
		pmulhw		xmm7, xmm1;			// * *  YG3 YB2  YR2 YG1  YB0 YR0
		pmulhw		xmm4, xmm0;
		pmulhw		xmm5, xmm0;
		movdqa		xmm2, xmm6;			// * *  YB3 YR3  YG2 YB1  YR1 YG0
		paddsw		xmm6, xmm7;			// * *  YG3+YB3 *  YR2+YG2 YG1+YB1  * YR0+YG0
		shufps		xmm2, xmm7, 0x88;	// YG3 YB2  YB0 YR0  YB3 YR3  YR1 YG0
		movdqa		xmm3, xmm6;
		movdqa		xmm7, xmm2;
		pand		xmm2, gc_dwMaskDLW;	// 0 YB2  0 *  0 YR3  0 *
		psrld		xmm3, 0x10;			// 0 *  0 YG3+YB3  0 YR2+YG2  0 *
		psrld		xmm7, 0x10;			// 0 *  0 YB0  * 0  0 YR1
		shufps		xmm6, xmm3, 0x94;	// 0 YG3+YB3  0 YR2+YG2  * YG1+YB1  * YR0+YG0
		shufps		xmm7, xmm2, 0x72;	// 0 YR3  0 YB2  0 YR1  0 YB0
		pshufd		xmm1, xmm4, 0xb1;
		pshufd		xmm2, xmm5, 0xb1;
		paddsw		xmm7, xmm6;			// 0 Y3  0 Y2  * Y1  * Y0
		paddsw		xmm1, xmm4;			// UR + UB
		paddsw		xmm2, xmm5;			// VR + VB
		movdqa		xmm0, gc_nRoundComp_5;
		movdqa		xmm3, gc_dwMaskDLB;
		movdqa		xmm6, gc_nOff128_5;
		psrld		xmm4, 0x10;
		psrld		xmm5, 0x10;
		paddsw		xmm1, xmm0;
		paddsw		xmm2, xmm0;
		paddsw		xmm7, xmm0;
		paddsw		xmm4, xmm6;
		paddsw		xmm5, xmm6;
		paddsw		xmm1, xmm4;
		paddsw		xmm2, xmm5;
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm7, 0x05;
		shufps		xmm1, xmm1, 0x88;	// U2 U0 U2 U0
		shufps		xmm2, xmm2, 0x88;	// V2 V0 V2 V0
		pminsw		xmm7, xmm3;
		pminsw		xmm1, xmm3;
		pminsw		xmm2, xmm3;
		pand		xmm7, xmm3;			// 0 Y3 0 Y2 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 U2 0 U0 0 U2 0 U0
		pand		xmm2, xmm3;			// 0 V2 0 V0 0 V2 0 V0
		packssdw	xmm7, xmm7;			// Y3Y2 Y1Y0 Y3Y2 Y1Y0
		pslld		xmm1, 0x08;			// 00 U2 0  00 U0 0  00 U2 0  00 U0 0
		pslld		xmm2, 0x18;			// V2 000  V0 000  V2 000  V0 000
		por			xmm7, xmm1;			// 0 Y3U2Y2  0 Y1U0Y0  0 Y3U2Y2  0 Y1U0Y0
		por			xmm7, xmm2;			// V2Y3U2Y2 V0Y1U0Y0 V2Y3U2Y2 V0Y1U0Y0
		movsd		[edi], xmm7;
		jmp			loop_end;
loop_mp2:
		mov         eax, pnY32_BGR;
		mov         ebx, pnUV_BGR;
		test		ecx, 0x02;
		jz			loop_mp1;
		movsd		xmm7, [esi];		// 0000 0000 00BG RBGR
		pshufd		xmm1, xmm7, 0xa6;	// 0000 0000 00BG 0000
		pshufd		xmm2, xmm7, 0x8a;	// 0000 0000 RBGR 0000
		pshufd		xmm3, xmm7, 0xa8;	// 0000 0000 0000 RBGR
		shufps		xmm7, xmm7, 0x00;	// RBGR RBGR RBGR RBGR    UV
		pslld		xmm1, 0x08;			// 0000 0000 0BG0 0000
		psrld		xmm2, 0x18;			// 0000 0000 000R 0000
		por			xmm3, xmm1;
		por			xmm2, xmm1;			// 0000 0000 0BGR RBGR
		pxor		xmm0, xmm0;			// 0000 0000 0000 0000
		pxor		xmm1, xmm1;
		movdqu		xmm5, [eax];	    // Y
		movdqu		xmm6, [ebx];	    // UV
		movdqa		xmm3, gc_dwMaskDLB;
		punpcklbw	xmm0, xmm7;			// 00B0 G0R0 R0B0 G0R0
		punpcklbw	xmm1, xmm7;			// R0B0 G0R0 R0B0 G0R0
		psrlw		xmm0, 0x01;			// 放大 7 位
		psrlw		xmm1, 0x01;
		pmulhw		xmm5, xmm0;			// 还放大 5 位
		pmulhw		xmm6, xmm1;
		pshufd		xmm0, xmm5, 0xb1;
		pshufd		xmm1, xmm6, 0xb1;
		paddsw		xmm0, xmm5;			// YR + YB
		paddsw		xmm1, xmm6;			// VR + VB  UR + UB
		psrld		xmm5, 0x10;
		psrld		xmm6, 0x10;
		paddsw		xmm0, xmm2;
		paddsw		xmm1, xmm2;
		paddsw		xmm6, gc_nOff128_5;
		paddsw		xmm0, xmm5;			// Y
		paddsw		xmm1, xmm6;			// U V
		psrlw		xmm0, 0x05;
		psrlw		xmm1, 0x05;
		pminsw		xmm0, xmm3;
		pminsw		xmm1, xmm3;
		shufps		xmm0, xmm0, 0x88;	// Y1 Y0 Y1 Y0
		shufps		xmm1, xmm1, 0x88;	// V0 U0 V0 U0
		pand		xmm0, xmm3;			// 0 Y1 0 Y0 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 V0 0 U0 0 V0 0 U0
		packssdw	xmm0, xmm0;			// Y1Y0 Y1Y0 Y1Y0 Y1Y0
		packssdw	xmm1, xmm1;			// V0U0 V0U0 V0U0 V0U0
		pslld		xmm1, 0x08;
		por			xmm0, xmm1;			// V0Y1U0Y0
		movss		[edi], xmm0;
		jmp			loop_end;
loop_mp1:
		test		ecx, 0x01;
		jz			loop_end;
		movss		xmm7, [esi];		// 0000 0000 0000 0BGR
		movdqa		xmm2, gc_nRoundComp_5;
		movdqu		xmm5, [eax];	    // Y
		movdqu		xmm6, [ebx]; 	    // UV
		shufps		xmm7, xmm7, 0xf0;
		pxor		xmm0, xmm0;
		punpcklbw	xmm0, xmm7;			// A0B0 G0R0 A0B0 G0R0
		psrlw		xmm0, 0x01;			// 放大 7 位
		pmulhw		xmm5, xmm0;			// 还放大 5 位
		pmulhw		xmm6, xmm0;
		pshufd		xmm0, xmm5, 0xb1;
		pshufd		xmm1, xmm6, 0xb1;
		paddsw		xmm0, xmm5;			// YR + YB
		paddsw		xmm1, xmm6;			// VR + VB  UR + UB
		psrld		xmm5, 0x10;
		psrld		xmm6, 0x10;
		paddsw		xmm0, xmm2;
		paddsw		xmm1, xmm2;
		paddsw		xmm6, gc_nOff128_5;
		paddsw		xmm0, xmm5;			// Y
		paddsw		xmm1, xmm6;			// U V
		psrlw		xmm0, 0x05;
		psrlw		xmm1, 0x05;
		pminsw		xmm0, xmm3;
		pminsw		xmm1, xmm3;
		shufps		xmm0, xmm0, 0x88;	// Y1 Y0 Y1 Y0
		shufps		xmm1, xmm1, 0x88;	// V0 U0 V0 U0
		pand		xmm0, xmm3;			// 0 Y1 0 Y0 0 Y1 0 Y0
		pand		xmm1, xmm3;			// 0 V0 0 U0 0 V0 0 U0
		packssdw	xmm0, xmm0;			// Y1Y0 Y1Y0 Y1Y0 Y1Y0
		packssdw	xmm1, xmm1;			// V0U0 V0U0 V0U0 V0U0
		pslld		xmm1, 0x08;
		por			xmm0, xmm1;			// V0Y1U0Y0
		movss		[edi], xmm0;
loop_end:
	}
}
// 前后插值获得第二个 UV
void yuyv_to_color24_line(OUT void* color24,	// *G@ 缓存，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						  IN void* yuyv,		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						  IN int na16,			// 源字节宽度减 4 后除以 16 的商与余数
						  IN int nm16,
						  IN int bHD,           // 高标清标志
						  IN BOOL bBGR)         // 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
{
	if (bHD)    // 高清
	{
		if (bBGR)       // B 在低位
		{
			yuyv_to_color24_inner(color24, yuyv, na16, nm16, g_nB_U_HD, g_nG_U_HD, g_nG_V_HD, g_nR_V_HD);
		}
		else        // R 在低位
		{
			yuyv_to_color24_inner(color24, yuyv, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
	}
	else    // 标清
	{
		if (bBGR)       // B 在低位
		{
			yuyv_to_color24_inner(color24, yuyv, na16, nm16, g_nB_U_SD, g_nG_U_SD, g_nG_V_SD, g_nR_V_SD);
		}
		else        // R 在低位
		{
			yuyv_to_color24_inner(color24, yuyv, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
	}
}
void yuyv_to_color24_inner(OUT void* color24,   // *G@ 缓存，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						   IN void* yuyv,		// 源 yuyv 缓存，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
						   IN int na16,			// 源字节宽度减 4 后除以 16 的商与余数
						   IN int nm16,
						   IN int* pnR_V,       // 系数（通过变更 R 分量与 B 分量的系数位置可以做到 BGRA 与 RGBA 的兼容）
						   IN int* pnG_U,       // （此处只需要将 pnR_V 与 pnB_U 的输入交换即可实现）
						   IN int* pnG_V,
						   IN int* pnB_U)
{
	_asm{       // 程序内部以 RGBA 为例做说明
		mov			esi, yuyv;
		mov			eax, yuyv;
		mov			edi, color24;
		mov			edx, color24;
		mov			ecx, na16;
		mov         ebx, pnG_V;
		add			eax, 0x10;			// 下一个 VYUV 用于 UV 插值
		add			edx, 0x10;			// 下半组 RGB 存储
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// 下一个 VYUV 用于 UV 插值
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// 放大 6 位
		psrlw		xmm6, 2;			// 放大 6 位
		psllw		xmm7, 5;			// 放大 5 位
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// 减去偏移量 128
		psubsw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V8 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U8 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U8 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V8 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		push        esi;
		push        eax;
		push        ecx;
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pop         ecx;
		pop         eax;
		pop         esi;
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;		// B7 B6 B5 B4 B3 B2 B1 B0
		movdqa		xmm4, xmm1;		// R7 R6 R5 R4 R3 R2 R1 R0
		movdqa		xmm5, xmm2;		// G7 G6 G5 G4 G3 G2 G1 G0
		movdqa		xmm7, xmm6;		// B7 B6 B5 B4 B3 B2 B1 B0
		psrld		xmm1, 0x08;		// 00 R7 0  00 R5 0  00 R3 0  00 R1 0
		psllw		xmm2, 0x08;		// G7 0 G6 0 G5 0 G4 0 G3 0 G2 0 G1 0 G0 0
		psllw		xmm6, 0x08;		// B7 0 B6 0 B5 0 B4 0 B3 0 B2 0 B1 0 B0 0
		por			xmm1, xmm7;		// 0 B7  R7B6  0 B5  R5B4  0 B3  R3B2  0 B1  R1B0
		por			xmm2, xmm4;		// G7R7 G6R6 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		por			xmm6, xmm5;		// B7G7 B6G6 B5G5 B4G4 B3G3 B2G2 B1G1 B0G0
		pand		xmm1, gc_dwMaskDLW;	// 00 R7B6 00 R5B4 00 R3B2 00 R1B0
		pand		xmm2, gc_dwMaskDLW;	// 00 G6R6 00 G4R4 00 G2R2 00 G0R0
		pand		xmm6, gc_dwMaskDHW;	// B7G7 00 B5G5 00 B3G3 00 B1G1 00
		movdqa		xmm4, xmm1;
		movdqa		xmm5, xmm2;
		movdqa		xmm7, xmm6;
		pslld		xmm1, 0x10;		// R7B6 00 R5B4 00 R3B2 00 R1B0 00
		pslld		xmm2, 0x10;		// G6R6 00 G4R4 00 G2R2 00 G0R0 00
		psrld		xmm6, 0x10;		// 00 B7G7 00 B5G5 00 B3G3 00 B1G1
		shufps		xmm2, xmm2, 0xb1;	// G4R4 00 G6R6 00 G0R0 00 G2R2 00
		por			xmm1, xmm5;		// **** R5B5G4R4 **** R1B0G0R0
		por			xmm2, xmm6;		// **** G6R6B5G5 **** G2R2B1G1
		por			xmm7, xmm4;		// B7G7R7B6 **** B3G3R3B2 ****
		shufps		xmm1, xmm2, 0x88;	// G6R6B5G5 G2R2B1G1 R5B5G4R4 R1B0G0R0
		shufps		xmm2, xmm7, 0xfa;	// B7G7R7B6 B7G7R7B6 G6R6B5G5 G6R6B5G5
		shufps		xmm7, xmm1, 0x55;	// R5B5G4R4 R5B5G4R4 B3G3R3B2 B3G3R3B2
		shufps		xmm2, xmm2, 0x88;	// B7G7R7B6 G6R6B5G5 B7G7R7B6 G6R6B5G5
		shufps		xmm1, xmm7, 0x88;	// R5B5G4R4 B3G3R3B2 G2R2B1G1 R1B0G0R0
		movsd		[edx], xmm2;
		movdqu		[edi], xmm1;
		add			esi, 0x10;
		add			eax, 0x10;
		add			edi, 0x18;
		add			edx, 0x18;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_m16:
		mov			ecx, nm16;
		test		ecx, 0x10;
		jz			loop_m12;
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V6 U6 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U6 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V6 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V6 0 V6 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U6 0 U6 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U6 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V6 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U6 U6 U6 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U6 U6 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V6 V6 V6 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V6 V6 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U7 U6 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V7 V6 V5 V4 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;		// R7 R6 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;		// G7 G6 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;		// B7 B6 B5 B4 B3 B2 B1 B0
		movdqa		xmm4, xmm1;		// R7 R6 R5 R4 R3 R2 R1 R0
		movdqa		xmm5, xmm2;		// G7 G6 G5 G4 G3 G2 G1 G0
		movdqa		xmm7, xmm6;		// B7 B6 B5 B4 B3 B2 B1 B0
		psrld		xmm1, 0x08;		// 00 R7 0  00 R5 0  00 R3 0  00 R1 0
		psllw		xmm2, 0x08;		// G7 0 G6 0 G5 0 G4 0 G3 0 G2 0 G1 0 G0 0
		psllw		xmm6, 0x08;		// B7 0 B6 0 B5 0 B4 0 B3 0 B2 0 B1 0 B0 0
		por			xmm1, xmm7;		// 0 B7  R7B6  0 B5  R5B4  0 B3  R3B2  0 B1  R1B0
		por			xmm2, xmm4;		// G7R7 G6R6 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		por			xmm6, xmm5;		// B7G7 B6G6 B5G5 B4G4 B3G3 B2G2 B1G1 B0G0
		pand		xmm1, gc_dwMaskDLW;	// 00 R7B6 00 R5B4 00 R3B2 00 R1B0
		pand		xmm2, gc_dwMaskDLW;	// 00 G6R6 00 G4R4 00 G2R2 00 G0R0
		pand		xmm6, gc_dwMaskDHW;	// B7G7 00 B5G5 00 B3G3 00 B1G1 00
		movdqa		xmm4, xmm1;
		movdqa		xmm5, xmm2;
		movdqa		xmm7, xmm6;
		pslld		xmm1, 0x10;		// R7B6 00 R5B4 00 R3B2 00 R1B0 00
		pslld		xmm2, 0x10;		// G6R6 00 G4R4 00 G2R2 00 G0R0 00
		psrld		xmm6, 0x10;		// 00 B7G7 00 B5G5 00 B3G3 00 B1G1
		shufps		xmm2, xmm2, 0xb1;	// G4R4 00 G6R6 00 G0R0 00 G2R2 00
		por			xmm1, xmm5;		// **** R5B5G4R4 **** R1B0G0R0
		por			xmm2, xmm6;		// **** G6R6B5G5 **** G2R2B1G1
		por			xmm7, xmm4;		// B7G7R7B6 **** B3G3R3B2 ****
		shufps		xmm1, xmm2, 0x88;	// G6R6B5G5 G2R2B1G1 R5B5G4R4 R1B0G0R0
		shufps		xmm2, xmm7, 0xfa;	// B7G7R7B6 B7G7R7B6 G6R6B5G5 G6R6B5G5
		shufps		xmm7, xmm1, 0x55;	// R5B5G4R4 R5B5G4R4 B3G3R3B2 B3G3R3B2
		shufps		xmm2, xmm2, 0x88;	// B7G7R7B6 G6R6B5G5 B7G7R7B6 G6R6B5G5
		shufps		xmm1, xmm7, 0x88;	// R5B5G4R4 B3G3R3B2 G2R2B1G1 R1B0G0R0
		movsd		[edx], xmm2;
		movdqu		[edi], xmm1;
		jmp			loop_end;
loop_m12:
		cmp			ecx, 0x0c;
		jnz			loop_m8;
		sub			eax, 0x08;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movss		xmm6, [eax];		// 最后两个像素 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// 减去偏移量 128
		psubw		xmm2, xmm5;			// 减去偏移量 128
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 U4 0 U4 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// V4 0 V4 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// V4 0 V4 0 V4 0 V2 0
		pslld		xmm4, 0x10;			// U4 0 U4 0 U4 0 U2 0
		pslld		xmm5, 0x10;			// U4 0 U4 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 V4 0 V4 0 V2 0 V0
		por			xmm4, xmm1;			// U4 U4 U4 U4 U4 U2 U2 U0
		por			xmm5, xmm1;			// U4 U4 U4 U4 U2 U2 U0 U0
		por			xmm2, xmm6;			// V4 V4 V4 V4 V4 V2 V2 V0
		por			xmm3, xmm6;			// V4 V4 V4 V4 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// U5 U4 U5 U4 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// V5 V4 V5 V4 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// R5 R4 R5 R4 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// G5 G4 G5 G4 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// B5 B4 B5 B4 B3 B2 B1 B0
		movdqa		xmm4, xmm1;		// R5 R4 R5 R4 R3 R2 R1 R0
		movdqa		xmm5, xmm2;		// G5 G4 G5 G4 G3 G2 G1 G0
		movdqa		xmm7, xmm6;		// B5 B4 B5 B4 B3 B2 B1 B0
		psrld		xmm1, 0x08;		// 00 R5 0  00 R5 0  00 R3 0  00 R1 0
		psllw		xmm2, 0x08;		// G5 0 G4 0 G5 0 G4 0 G3 0 G2 0 G1 0 G0 0
		psllw		xmm6, 0x08;		// B5 0 B4 0 B5 0 B4 0 B3 0 B2 0 B1 0 B0 0
		por			xmm1, xmm7;		// 0 B5  R5B4  0 B5  R5B4  0 B3  R3B2  0 B1  R1B0
		por			xmm2, xmm4;		// G5R5 G4R4 G5R5 G4R4 G3R3 G2R2 G1R1 G0R0
		por			xmm6, xmm5;		// B5G5 B4G4 B5G5 B4G4 B3G3 B2G2 B1G1 B0G0
		pand		xmm1, gc_dwMaskDLW;	// 00 R5B4 00 R5B4 00 R3B2 00 R1B0
		pand		xmm2, gc_dwMaskDLW;	// 00 G4R4 00 G4R4 00 G2R2 00 G0R0
		pand		xmm6, gc_dwMaskDHW;	// B5G5 00 B5G5 00 B3G3 00 B1G1 00
		movdqa		xmm4, xmm1;
		movdqa		xmm5, xmm2;
		movdqa		xmm7, xmm6;
		pslld		xmm1, 0x10;		// R5B4 00 R5B4 00 R3B2 00 R1B0 00
		pslld		xmm2, 0x10;		// G4R4 00 G4R4 00 G2R2 00 G0R0 00
		psrld		xmm6, 0x10;		// 00 B5G5 00 B5G5 00 B3G3 00 B1G1
		shufps		xmm2, xmm2, 0xb1;	// G4R4 00 G4R4 00 G0R0 00 G2R2 00
		por			xmm1, xmm5;		// **** R5B5G4R4 **** R1B0G0R0
		por			xmm2, xmm6;		// **** **B5G5 **** G2R2B1G1
		por			xmm7, xmm4;		// **** **** B3G3R3B2 ****
		shufps		xmm2, xmm2, 0x0a;	// G2R2B1G1 G2R2B1G1 **B5G5 **B5G5
		shufps		xmm7, xmm1, 0x5a;	// R5B5G4R4 R5B5G4R4 B3G3R3B2 B3G3R3B2
		shufps		xmm1, xmm2, 0xa0;	// G2R2B1G1 G2R2B1G1 R1B0G0R0 R1B0G0R0
		shufps		xmm1, xmm7, 0x88;	// R5B5G4R4 B3G3R3B2 G2R2B1G1 R1B0G0R0
		movss		[edx], xmm2;
		movdqu		[edi], xmm1;
		jmp			loop_end;
loop_m8:
		test		ecx, 0x08;
		jz			loop_m4;
		movsd		xmm7, [esi];		// 0000 0000 VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 V2 U2 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		pshufd		xmm2, xmm1, 0xf5;	// 0 0 0 0 V2 U2 V2 U2
		movdqa		xmm3, xmm1;
		movdqa		xmm5, xmm1;
		movdqa		xmm6, xmm1;
		movdqa		xmm4, xmm2;
		pand		xmm1, gc_dwMaskDLW;	// 0 0 0 0 0 U2 0 U0
		pand		xmm3, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V0 0
		pand		xmm2, gc_dwMaskDHW;	// 0 0 0 0 V2 0 V2 0
		pslld		xmm4, 0x10;			// 0 0 0 0 U2 0 U2 0
		pslld		xmm5, 0x10;			// 0 0 0 0 U2 0 U0 0
		psrld		xmm6, 0x10;			// 0 0 0 0 0 V2 0 V0
		por			xmm4, xmm1;			// 0 0 0 0 U2 U2 U2 U0
		por			xmm5, xmm1;			// 0 0 0 0 U2 U2 U0 U0
		por			xmm2, xmm6;			// 0 0 0 0 V2 V2 V2 V0
		por			xmm3, xmm6;			// 0 0 0 0 V2 V2 V0 V0
		paddw		xmm4, xmm5;			// 0 0 0 0 U3 U2 U1 U0
		paddw		xmm3, xmm2;			// 0 0 0 0 V3 V2 V1 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm1, xmm3;
		pmulhw		xmm2, xmm3;
		pmulhw		xmm5, xmm4;
		pmulhw		xmm6, xmm4;
		pxor		xmm3, xmm3;			// 0
		movdqa		xmm4, gc_nRoundComp_5;
		paddsw		xmm1, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm4;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm1, xmm4;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm4;			// B
		pmaxsw		xmm1, xmm3;			// R
		pmaxsw		xmm2, xmm3;			// G
		pmaxsw		xmm6, xmm3;			// B
		psrlw		xmm1, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm1, xmm0;			// 0 0 0 0 R3 R2 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 G3 G2 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 B3 B2 B1 B0
		movdqa		xmm4, xmm1;			// 0 0 0 0 R3 R2 R1 R0
		shufps		xmm1, xmm2, 0x44;	// G3 G2 G1 G0 R3 R2 R1 R0
		shufps		xmm2, xmm6, 0x44;	// B3 B2 B1 B0 G3 G2 G1 G0
		psrld		xmm4, 0x08;			// 0 0 0 0 0 R3 0 R1
		psllw		xmm2, 0x08;			// B3 B2 B1 B0 G3 G2 G1 G0
		por			xmm4, xmm6;			// 0 0 0 0 * R3B2 * R1B0
		por			xmm2, xmm1;			// B3G3 * B1G1 * * G2R2 * G0R0
		movdqa		xmm6, xmm4;
		movdqa		xmm1, xmm2;
		shufps		xmm4, xmm2, 0x50;	// * G2R2 * G2R2 * R1B0 * R1B0
		pand		xmm1, gc_dwMaskDLW;	// 0 * 0 * 0 * 0 G0R0
		pand		xmm2, gc_dwMaskDHW;	// B3G3 0 B1G1 0 * 0 * 0
		pand		xmm6, gc_dwMaskDLW;	// 0 0 0 0 0 R3B2 0 R1B0
		movdqa		xmm5, xmm2;
		pslld		xmm4, 0x10;			// G2R2 0 G2R2 0 R1B0 0 R1B0 0
		psrld		xmm5, 0x10;			// 0 0 0 B1G1 0 0 0 0
		shufps		xmm6, xmm6, 0x55;	// 0 R3B2 0 R3B2 0 R3B2 0 R3B2
		shufps		xmm1, xmm5, 0xa0;	// 0 B1G1 0 B1G1 0 G0R0 0 G0R0
		shufps		xmm2, xmm2, 0xff;	// B3G3 0 B3G3 0 B3G3 0 B3G3 0
		por			xmm1, xmm4;			// G2R2B1G1 G2R2B1G1 R1B0G0R0 R1B0G0R0
		por			xmm2, xmm6;			// B3G3R3B2 B3G3R3B2 B3G3R3B2 B3G3R3B2
		shufps		xmm1, xmm1, 0x88;	// G2R2B1G1 R1B0G0R0 G2R2B1G1 R1B0G0R0
		sub			edx, 0x08;
		movsd		[edi], xmm1;
		movss		[edx], xmm2;
		jmp			loop_end;
loop_m4:
		test		ecx, 0x04;
		jz			loop_end;
		movss		xmm7, [esi];		// 0000 0000 0000 VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		pand		xmm7, xmm0;			// 0 0 0 0 0 0 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// 0 0 0 0 0 0 V0 U0
		psllw		xmm7, 5;			// 放大 5 位
		psrlw		xmm1, 2;			// 放大 6 位
		psubw		xmm1, gc_nOff128_6;	// 减去偏移量 128
		movdqa		xmm3, xmm1;
		psrld		xmm1, 0x10;			// 0 0 0 0 0 0 0 V0
		mov         eax, pnR_V;
		mov         ecx, pnG_U;
		mov         esi, pnB_U;
		movdqu		xmm1, [eax];
		movdqu		xmm2, [ebx];
		movdqu		xmm5, [ecx];
		movdqu		xmm6, [esi];
		pmulhw		xmm4, xmm1;
		pmulhw		xmm2, xmm1;
		pmulhw		xmm5, xmm3;
		pmulhw		xmm6, xmm3;
		pxor		xmm1, xmm1;			// 0
		movdqa		xmm3, gc_nRoundComp_5;
		paddsw		xmm4, xmm7;			// R
		paddsw		xmm2, xmm7;			// G
		paddsw		xmm5, xmm3;
		paddsw		xmm6, xmm7;			// B
		paddsw		xmm4, xmm3;			// R
		paddsw		xmm2, xmm5;			// G
		paddsw		xmm6, xmm3;			// B
		pmaxsw		xmm4, xmm1;			// R
		pmaxsw		xmm2, xmm1;			// G
		pmaxsw		xmm6, xmm1;			// B
		psrlw		xmm4, 0x05;
		psrlw		xmm2, 0x05;
		psrlw		xmm6, 0x05;
		pminsw		xmm4, xmm0;			// 0 0 0 0 0 0 R1 R0
		pminsw		xmm2, xmm0;			// 0 0 0 0 0 0 G1 G0
		pminsw		xmm6, xmm0;			// 0 0 0 0 0 0 B1 B0
		movdqa		xmm5, xmm4;			// 0 0 0 0 0 0 R1 R0
		shufps		xmm4, xmm2, 0x00;	// G1 G0 G1 G0 R1 R0 R1 R0
		shufps		xmm2, xmm6, 0x00;	// B1 B0 B1 B0 G1 G0 G1 G0
		psrld		xmm5, 0x08;			// 0 0 0 0 0 0 0 R1
		psllw		xmm2, 0x08;			// B1 B0 B1 B0 G1 G0 G1 G0
		por			xmm5, xmm6;			// 0 0 0 0 0 0 * R1B0
		por			xmm2, xmm4;			// B1G1 * B1G1 * * G0R0 * G0R0
		movdqa		xmm1, xmm2;
		pslld		xmm5, 0x10;			// 0 0 0 0 0 0 R1B0 0
		psrld		xmm2, 0x10;			// 0 B1G1 0 B1G1 0 * 0 *
		pand		xmm1, gc_dwMaskDLW;	// 0 * 0 * 0 * 0 G0R0
		por			xmm1, xmm5;			// * * * R1B0G0R0
		shufps		xmm1, xmm2, 0xa0;	// **B1G1 **B1G1 R1B0G0R0 R1B0G0R0
		shufps		xmm1, xmm1, 0x88;	// **B1G1 R1B0G0R0 **B1G1 R1B0G0R0
		movsd		[edi], xmm1;
loop_end:
	}
}
// 补充 Alpha 通道为 0xff
void color24_to_color32(OUT void* color32,	// *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……
						IN void* color24,	// *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						IN int nap5,		// 源像素宽度除以 5 的商与余数（正好满足十六字节对齐）
						IN int nmp5)
{
	// LaiShoubo @ 2009-11-5
	__asm
	{
		pcmpeqb		xmm7, xmm7;
		mov			eax, 05h;
		mul			nap5;
		mov			esi, color24;
		mov			edi, color32;
		add			eax, nmp5;
		mov			ecx, eax;
		movd		eax, xmm7;
		mov			edx, ecx;
		and			ecx, 0fh;
		shr			edx, 04h;				// 一次处理16个像素
		movaps		xmm6, xmm7;
		pslld		xmm7, 18h;				// FF000000 FF000000 FF000000 FF000000
		psrld		xmm6, 08h;				// 00FFFFFF 00FFFFFF 00FFFFFF 00FFFFFF
		prefetchnta	[esi];
		test		edx, edx;
		jz			loop_1;
		test		edi, 0fh;
		jz			loop_a16;
		test		edi, 03h;
		jnz			loop_16;
		shl			edx, 04h;
		add			edx, ecx;
loop_pre_1:
		movsb
		movsb
		movsb
		stosb
		dec			edx;
		test		edi, 0fh;
		jnz			loop_pre_1;
		mov			ecx, edx;
		shr			edx, 04h;
		and			ecx, 0fh;
loop_a16:
		prefetchnta	[esi + 30h];
		movups		xmm0, [esi];			// RBGR BGRB GRBG RBGR
		movups		xmm2, [esi + 10h];		// GRBG RBGR BGRB GRBG
		movaps		xmm1, xmm0;
		movups		xmm3, [esi + 20h];		// BGRB GRBG RBGR BGRB
		shufpd		xmm1, xmm2, 01h;		// BGRB GRBG RBGR BGRB
		shufpd		xmm2, xmm3, 01h;		// RBGR BGRB GRBG RBGR
		shufps		xmm0, xmm0, 94h;		// BGRB GRBG GRBG RBGR
		shufps		xmm2, xmm2, 94h;
		shufps		xmm1, xmm1, 0e9h;		// BGRB GRBG GRBG RBGR
		shufps		xmm3, xmm3, 0e9h;
		pshuflw		xmm0, xmm0, 94h;
		pshuflw		xmm1, xmm1, 94h;
		pshuflw		xmm2, xmm2, 94h;
		pshuflw		xmm3, xmm3, 94h;
		pshufhw		xmm0, xmm0, 0e9h;
		pshufhw		xmm1, xmm1, 0e9h;
		pshufhw		xmm2, xmm2, 0e9h;
		pshufhw		xmm3, xmm3, 0e9h;		// BGRB RBGR BGRB RBGR
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm2;
		shufps		xmm0, xmm1, 88h;
		shufps		xmm2, xmm3, 88h;
		shufps		xmm4, xmm1, 0ddh;
		shufps		xmm5, xmm3, 0ddh;
		andpd		xmm0, xmm6;
		andpd		xmm2, xmm6;
		psrld		xmm4, 08h;
		psrld		xmm5, 08h;
		orpd		xmm0, xmm7;
		orpd		xmm4, xmm7;
		orpd		xmm2, xmm7;
		orpd		xmm5, xmm7;
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpckldq	xmm0, xmm4;
		punpckhdq	xmm1, xmm4;
		punpckldq	xmm2, xmm5;
		punpckhdq	xmm3, xmm5;
		movntps		[edi], xmm0;
		movntps		[edi + 10h], xmm1;
		movntps		[edi + 20h], xmm2;
		movntps		[edi + 30h], xmm3;
		add			edi, 40h;
		add			esi, 30h;
		dec			edx;
		jnz			loop_a16;
		sfence
		jmp			loop_1;
loop_16:
		prefetchnta	[esi + 30h];
		movups		xmm0, [esi];			// RBGR BGRB GRBG RBGR
		movups		xmm2, [esi + 10h];		// GRBG RBGR BGRB GRBG
		movaps		xmm1, xmm0;
		movups		xmm3, [esi + 20h];		// BGRB GRBG RBGR BGRB
		shufpd		xmm1, xmm2, 01h;		// BGRB GRBG RBGR BGRB
		shufpd		xmm2, xmm3, 01h;		// RBGR BGRB GRBG RBGR
		shufps		xmm0, xmm0, 94h;		// BGRB GRBG GRBG RBGR
		shufps		xmm2, xmm2, 94h;
		shufps		xmm1, xmm1, 0e9h;		// BGRB GRBG GRBG RBGR
		shufps		xmm3, xmm3, 0e9h;
		pshuflw		xmm0, xmm0, 94h;
		pshuflw		xmm1, xmm1, 94h;
		pshuflw		xmm2, xmm2, 94h;
		pshuflw		xmm3, xmm3, 94h;
		pshufhw		xmm0, xmm0, 0e9h;
		pshufhw		xmm1, xmm1, 0e9h;
		pshufhw		xmm2, xmm2, 0e9h;
		pshufhw		xmm3, xmm3, 0e9h;		// BGRB RBGR BGRB RBGR
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm2;
		shufps		xmm0, xmm1, 88h;
		shufps		xmm2, xmm3, 88h;
		shufps		xmm4, xmm1, 0ddh;
		shufps		xmm5, xmm3, 0ddh;
		andpd		xmm0, xmm6;
		andpd		xmm2, xmm6;
		psrld		xmm4, 08h;
		psrld		xmm5, 08h;
		orpd		xmm0, xmm7;
		orpd		xmm4, xmm7;
		orpd		xmm2, xmm7;
		orpd		xmm5, xmm7;
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpckldq	xmm0, xmm4;
		punpckhdq	xmm1, xmm4;
		punpckldq	xmm2, xmm5;
		punpckhdq	xmm3, xmm5;
		movups		[edi], xmm0;
		movups		[edi + 10h], xmm1;
		movups		[edi + 20h], xmm2;
		movups		[edi + 30h], xmm3;
		add			edi, 40h;
		add			esi, 30h;
		dec			edx;
		jnz			loop_16;
loop_1:
		test		ecx, ecx;
		jz			loop_0;
		movsb
		movsb
		movsb
		stosb
		dec			ecx;
		jnz			loop_1;
loop_0:
	}
//	_asm{       // 程序内部以 RGB 和 RGBA 为例做说明
//		mov			edi, color32;		// 前四个像素
//		mov			edx, color32;		// 最后一个像素
//		mov			ecx, nap5;
//		mov			esi, color24;
//		add			edx, 0x10;
//		movdqa		xmm0, gc_dwMaskAlpha;
//		test		ecx, ecx;
//		jnz			loop_a5;
//		jmp			loop_m4;
//loop_a5:		// 一次做五个像素
//		movdqu		xmm7, [esi];		// RBGR BGRB GRBG RBGR
//		movdqa		xmm4, xmm7;
//		movdqa		xmm5, xmm7;
//		movdqa		xmm6, xmm7;
//		pshufd		xmm3, xmm7, 0x00;	// RBGR RBGR RBGR RBGR
//		pshufd		xmm2, xmm7, 0xaa;	// BGRB BGRB BGRB BGRB
//		pslld		xmm4, 0x08;			// * *  *B1G1 0  *
//		psrld		xmm5, 0x10;			// * * 00G2R2 *
//		psrld		xmm6, 0x08;			// * 0B3G3R3 * *
//		psrld		xmm3, 0x18;			// 000R1 000R1 000R1 000R1
//		pslld		xmm2, 0x10;			// *B2 00  *B2 00  *B2 00  *B2 00
//		por			xmm4, xmm3;			// * * *B1G1R1 *
//		por			xmm5, xmm2;			// * * *B2G2R2 *
//		shufps		xmm4, xmm7, 0x05;	// *B0G0R0 *B0G0R0 *B1G1R1 *B1G1R1
//		shufps		xmm5, xmm6, 0xa5;	// 0B3G3R3 0B3G3R3 *B2G2R2 *B2G2R2
//		shufps		xmm7, xmm7, 0xff;	// *B4G4R4 *B4G4R4 *B4G4R4 *B4G4R4
//		shufps		xmm4, xmm5, 0x82;	// 0B3G3R3 *B2G2R2 *B1G1R1 *B0G0R0
//		por			xmm7, xmm0;
//		por			xmm4, xmm0;
//		movss		[edx], xmm7;
//		movdqu		[edi], xmm4;
//		add			esi, 0x0f;
//		add			edx, 0x14;
//		add			edi, 0x14;
//		sub			ecx, 0x01;
//		jnz			loop_a5;
//loop_m4:		// 剩余四个像素
//		mov			ecx, nmp5;
//		test		ecx, 0x04;
//		jz			loop_m3;
//		mov			eax, esi;
//		add			esi, 0x08;
//		movsd		xmm7, [eax];		// 0000 0000 GRBG RBGR
//		movss		xmm6, [esi];		// 0000 0000 0000 BGRB
//		pshufd		xmm2, xmm7, 0x92;	// 0000 GRBG RBGR 0000
//		shufps		xmm6, xmm6, 0x05;	// BGRB BGRB 0000 0000
//		movdqa		xmm4, xmm7;
//		movdqa		xmm5, xmm2;
//		movdqa		xmm3, xmm6;
//		psrld		xmm2, 0x10;			// 0  00G2R2  *  0
//		pslld		xmm4, 0x08;			// 0 0  *B1G1 0  *
//		psrld		xmm5, 0x18;			// 0 *  000R1  0
//		pslld		xmm3, 0x10;			// *  *B2 00  0 0
//		psrld		xmm6, 0x08;			// 0B3G3R3 * 0 0
//		por			xmm4, xmm5;			// 0 * *B1G1R1 *
//		por			xmm2, xmm3;			// * *B2G2R2 * 0
//		shufps		xmm4, xmm7, 0x05;	// *B0G0R0 *B0G0R0 *B1G1R1 *B1G1R1
//		shufps		xmm2, xmm6, 0xfa;	// 0B3G3R3 0B3G3R3 *B2G2R2 *B2G2R2
//		shufps		xmm4, xmm2, 0x82;	// 0B3G3R3 *B2G2R2 *B1G1R1 *B0G0R0
//		por			xmm4, xmm0;
//		movdqu		[edi], xmm4;
//		jmp			loop_end;
//loop_m3:		// 剩余三个像素
//		cmp			ecx, 0x03;
//		jnz			loop_m2;
//		mov			eax, esi;
//		add			esi, 0x08;
//		sub			edx, 0x08;
//		movsd		xmm7, [eax];		// 0000 0000 GRBG RBGR
//		movss		xmm6, [esi];		// 0000 0000 0000 000B
//		pshufd		xmm2, xmm7, 0xa1;	// 0000 0000 RBGR GRBG
//		movdqa		xmm5, xmm2;
//		movdqa		xmm4, xmm7;
//		psrld		xmm2, 0x10;			// 0 0 * 00G2R2
//		pslld		xmm6, 0x10;			// 0 0 0  0B2 00
//		pslld		xmm4, 0x08;			// 0 0  *B1G1 0  *
//		psrld		xmm5, 0x18;			// 0 0  000R1  *
//		por			xmm2, xmm6;			// 0 0 * 0B2G2R2
//		por			xmm4, xmm5;			// 0 0 *B1G1R1 *
//		shufps		xmm4, xmm7, 0x05;	// *B0G0R0 *B0G0R0 *B1G1R1 *B1G1R1
//		shufps		xmm4, xmm4, 0x22;	// *B1G1R1 *B0G0R0 *B1G1R1 *B0G0R0
//		por			xmm2, xmm0;
//		por			xmm4, xmm0;
//		movss		[edx], xmm2;
//		movsd		[edi], xmm4;
//		jmp			loop_end;
//loop_m2:		// 剩余两个像素
//		test		ecx, 0x02;
//		jz			loop_m1;
//		mov			eax, esi;
//		movsd		xmm7, [eax];		// 0000 0000 00BG RBGR
//		pshufd		xmm2, xmm7, 0xa2;	// 0000 0000 RBGR 0000
//		movdqa		xmm4, xmm7;
//		psrld		xmm2, 0x18;			// 0 0 000R1 0
//		pslld		xmm4, 0x08;			// 0 0  0B1G1 0  *
//		por			xmm4, xmm2;			// 0 0 0B1G1R1 *
//		shufps		xmm4, xmm7, 0x05;	// *B0G0R0 *B0G0R0 *B1G1R1 *B1G1R1
//		shufps		xmm4, xmm4, 0x22;	// *B1G1R1 *B0G0R0 *B1G1R1 *B0G0R0
//		por			xmm4, xmm0;
//		movsd		[edi], xmm4;
//		jmp			loop_end;
//loop_m1:		// 剩余一个像素
//		test		ecx, 0x01;
//		jz			loop_end;
//		movss		xmm7, [esi];
//		por			xmm7, xmm0;
//		movss		[edi], xmm7;
//loop_end:
//	}
}

// yuyv 到 bgra/rgba 的平均插值缩放变换，补充 Alpha 通道为 0xff
BOOL yuyv_to_color32_meanstretch(OUT BYTE* color32,		// *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）（Alpha 直接填 0xff）
								 IN int nDesPitch,
								 IN int nDesX,			// 目标起点
								 IN int nDesY,
								 IN int nDesWidth,		// 目标操作大小
								 IN int nDesHeight,
								 IN int nDesImgWidth,	// 目标图像大小
								 IN int nDesImgHeight,
								 IN BYTE* pSrcYUYV,		// YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
								 IN int nSrcPitch,
								 IN int nSrcX,			// 源起点
								 IN int nSrcY,
								 IN int nSrcWidth,		// 源操作大小
								 IN int nSrcHeight,
								 IN int nSrcImgWidth,	// 源图像大小（由于源为16位，要求此宽度为偶数）
								 IN int nSrcImgHeight,
								 IN BOOL bSrcHD,		// 源信号的高标清标志（决定使用颜色变换的公式）
								 IN BOOL bBGRA,			// 像素内颜色分量的存储顺序，为 TRUE 表示低 B 在低位
								 IN BOOL bDesRevert)	// 图像是否行翻转标志
{
	if (pSrcYUYV == NULL || color32 == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDesAddWidth(0), nLineIndex(0), nLineInc(1);

	// 获得有效的操作范围
	if (!TPMeanStretch_CurRange(nDesX, nDesY, nDesWidth, nDesHeight, nDesImgWidth, nDesImgHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcImgWidth, nSrcImgHeight, fScaleWidth, fScaleHeight,
		fSrcStartX, fSrcStartY, nDesAddWidth, nLineIndex, nLineInc, TP_PARITY_BOTH))
		return FALSE;

	// 源对齐起点，由于 YUYV 存储的原因，必须使得位置都是偶数。
	int nAlignSrcX = nSrcX - (nSrcX & 0x01);
	int nAlignSrcWidth = ((nSrcX + nSrcWidth) & 0x01) + nSrcWidth + (nSrcX & 0x01);
	float fScaleW_I = 1.0f / fScaleWidth, fScaleH_I = 1.0f / fScaleHeight;
	// 申请权重表、权重数量表及缓存空间
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDesWidth, int);		// X 方向上的权重数量表
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDesHeight, int);	// Y 方向上的权重数量表
	TPSSE_DECLARE(float*) pPowerX = TPSSE_NEW((nDesWidth << 1), float);	// X 方向上的权重表
	TPSSE_DECLARE(float*) pPowerY = TPSSE_NEW((nDesHeight << 1), float);	// Y 方向上的权重表
	BYTE *pDesLine = color32 + nDesX + nDesY * nDesPitch;		// Buffer 起始地址
	BYTE *pSrcLine = pSrcYUYV + nAlignSrcX + nSrcY * nSrcPitch;
	float *pTmpPowerY = pPowerY;		// Y 方向权重表地址
	TPSSE_DECLARE(float*) pTmpLine = TPSSE_NEW((nAlignSrcWidth << 2), float);	// 源行的临时缓存
	float *pTmpYUYVLine = pTmpLine + (nAlignSrcWidth << 1);
	TPSSE_DECLARE(float) fCoefW[4] = { fScaleW_I, fScaleW_I, fScaleW_I, fScaleW_I };
	TPSSE_DECLARE(float) fCoefH[4] = { fScaleH_I, fScaleH_I, fScaleH_I, fScaleH_I };
	float* pfRB_YUV = (bSrcHD) ? g_fRB_YUV_HD : g_fRB_YUV_SD;
	float* pfG_YUV = (bSrcHD) ? g_fG_YUV_HD : g_fG_YUV_SD;

	nDesAddWidth = bDesRevert ? -nDesPitch : nDesPitch;	// 翻转图像
	pDesLine += (bDesRevert ? (nDesHeight - 1) * nDesPitch : 0);
	// 计算权重
	strech_calc_power(pPowerX, pMapX, nDesWidth, fScaleWidth, fSrcStartX - (int)fSrcStartX);
	strech_calc_power(pPowerY, pMapY, nDesHeight, fScaleHeight, fSrcStartY - (int)fSrcStartY);
	// 逐行处理
	for (; nLineIndex<nDesHeight; nLineIndex+=nLineInc)
	{
		if ((nSrcY + pMapY[nLineIndex]) > nSrcImgHeight)	// 防止最后几行越界访问
		{
			pSrcLine = pSrcYUYV + nAlignSrcX + (nSrcImgHeight - pMapY[nLineIndex]) * nSrcPitch;
		}
		// 由于 YUYV 的 16 位特性，按 32 位处理时需要宽度折半。
		// 结果从临时缓冲的中间开始放，以方便后面进行 YUYV 到 YUVY 的扩展，注意第四通道重复了 Y。
		bgra32_mean_copy_add_y_quick(pTmpYUYVLine, pSrcLine, (nAlignSrcWidth >> 1),
			nSrcPitch, pTmpPowerY, fCoefH, pMapY[nLineIndex]);
		yuyv_mean_color32_midres(pTmpLine, pTmpYUYVLine, nAlignSrcWidth);
		// 水平加权时，需要考虑去掉左边偶数对齐的像素（一个像素占四个浮点数）。
		yuyv_mean_color32_add_x(pDesLine, nDesWidth, (pTmpLine + ((nSrcX - nAlignSrcX) << 2)),
			pPowerX, pMapX, fCoefW, pfRB_YUV, pfG_YUV, bBGRA);
		if (nLineInc == 1)
		{
			nSrcY += (pMapY[nLineIndex] - 1);
			pSrcLine += ((pMapY[nLineIndex] - 1) * nSrcPitch);
			pTmpPowerY += 2;
		}
		else
		{
			nSrcY += (pMapY[nLineIndex] - 2 + pMapY[nLineIndex + 1]);
			pSrcLine += ((pMapY[nLineIndex] - 2 + pMapY[nLineIndex + 1]) * nSrcPitch);
			pTmpPowerY += 4;
		}
		pDesLine += nDesAddWidth;
	}
	TPSSE_FREE(pMapX);
	TPSSE_FREE(pMapY);
	TPSSE_FREE(pPowerX);
	TPSSE_FREE(pPowerY);
	TPSSE_FREE(pTmpLine);

	return TRUE;
}
// 平均插值中临时缓存结果 YUYV 到 YUVY 的变换
void yuyv_mean_color32_midres(OUT void* yuvy,		// 浮点结果，存放方式 Y0 U0 V0 Y0 Y1 U1 V1 Y1 ……
							  IN void* yuyv,		// 浮点垂直叠加结果，存放方式 Y0 U0 Y1 V0 Y2 U2 Y3 V2 ……
							  IN int nCount)		// 像素点个数（必为偶数）
{
	__asm {
		mov			eax, nCount;
		mov			esi, yuyv;
		mov			edi, yuvy;
		mov			edx, eax;
		sub			eax, 0x02;
		shr			eax, 0x02;		// 一次做四个像素
		mov			ecx, eax;
		shl			eax, 0x02;
		sub			edx, eax;
		movaps		xmm7, gc_fRoundComp;	// 0.5f
		test		ecx, ecx;
		jnz			loop_a4;
		jmp			loop_m4;
loop_a4:
		movups		xmm0, dword ptr [esi];		// V0  Y1  U0  Y0
		movups		xmm1, dword ptr [esi + 0x10];	// V2  Y3  U2  Y2
		movups		xmm2, dword ptr [esi + 0x20];	// V4  Y5  U4  Y4
		movaps		xmm3, xmm0;
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm1;
		shufps		xmm2, xmm1, 0xdd;	// V2  U2  V4  U4
		shufps		xmm5, xmm0, 0xdd;	// V0  U0  V2  U2
		shufps		xmm0, xmm0, 0x34;	// Y0  V0  U0  Y0
		shufps		xmm1, xmm1, 0x34;	// Y2  V2  U2  Y2
		addps		xmm5, xmm2;		// V2+V0  U2+U0  V4+V2  U4+U2
		mulps		xmm5, xmm7;		// V1  U1  V3  U3
		shufps		xmm3, xmm5, 0xea;	// V1  U1  Y1  Y1
		shufps		xmm4, xmm5, 0x4a;	// V3  U3  Y3  Y3
		shufps		xmm3, xmm3, 0x38;	// Y1  V1  U1  Y1
		shufps		xmm4, xmm4, 0x38;	// Y3  V3  U3  Y3
		movups		dword ptr [edi], xmm0;
		movups		dword ptr [edi + 0x10], xmm3;
		movups		dword ptr [edi + 0x20], xmm1;
		movups		dword ptr [edi + 0x30], xmm4;
		add			esi, 0x20;
		add			edi, 0x40;
		sub			ecx, 0x01;
		jnz			loop_a4;
loop_m4:
		test		edx, 0x04;
		jz			loop_m2;
		movups		xmm0, dword ptr [esi];		// V0  Y1  U0  Y0
		movups		xmm2, dword ptr [esi + 0x10];	// V2  Y3  U2  Y2
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		addps		xmm1, xmm2;		// V0+V2  @  U0+U2  @
		mulps		xmm1, xmm7;		// V1  @  U1  @
		shufps		xmm1, xmm0, 0xda;	// V1  U1  Y1  Y1
		shufps		xmm0, xmm0, 0x34;	// Y0  V0  U0  Y0
		shufps		xmm2, xmm2, 0x34;	// Y2  V2  U2  Y2
		shufps		xmm1, xmm1, 0x38;	// Y1  V1  U1  Y1
		shufps		xmm3, xmm3, 0xb6;	// Y3  V2  U2  Y3
		movups		dword ptr [edi], xmm0;
		movups		dword ptr [edi + 0x10], xmm1;
		movups		dword ptr [edi + 0x20], xmm2;
		movups		dword ptr [edi + 0x30], xmm3;
		jmp			loop_end;
loop_m2:
		test		edx, 0x02;
		jz			loop_end;
		movups		xmm0, dword ptr [esi];
		movaps		xmm1, xmm0;
		shufps		xmm0, xmm0, 0x34;		// Y0  V0  U0  Y0
		shufps		xmm1, xmm1, 0xb6;		// Y1  V0  U0  Y1
		movups		dword ptr [edi], xmm0;
		movups		dword ptr [edi + 0x10], xmm1;
loop_end:
	}
}
// 平均插值中水平方向加权求和，并将最终 YUV 的像素变换为 BGRA/RGBA 颜色。
void yuyv_mean_color32_add_x(OUT void* des,			// 目标 32 颜色行
							 IN int dcw,			// 目标宽度
							 IN void* src,			// 临时缓存行（由行加权得到的结果）
							 IN void* pPowerX,		// 水平方向上的权重表
							 IN void* pMapX,		// 水平方向上的映射表
							 IN void* pCoefW,		// 水平方向上的缩放系数
							 IN float* pfRB_YUV,	// YUVY 到 RB 变换的系数（Ry Bu Rv By）
							 IN float* pfG_YUV,		// YUVY 到 G 变换的系数（Gy Gu Gv 0）
							 IN BOOL bBGRA)			// BGRA/RGBA 颜色区别
{
	_asm{
		mov			eax, pCoefW;
		mov			ebx, pfRB_YUV;
		mov			ecx, dcw;
		mov			edx, pfG_YUV;
		mov			edi, des;
		mov			esi, src;
		xorps		xmm0, xmm0;
		movaps		xmm1, gc_fOff128;		// 128.f
		movaps		xmm7, dword ptr [eax];	// 缩放系数 coef coef coef coef
		movaps		xmm5, dword ptr [ebx];	// RB 系数 By Rv Bu Ry
		movaps		xmm4, dword ptr [edx];	// G 系数 0 Gv Gu Gy
		shufps		xmm0, xmm1, 0x00;		// 128  128  0  0
		shufps		xmm0, xmm0, 0x28;		// 0  128  128  0
		mov			ebx, pMapX;
		mov			eax, pPowerX;
loop_pixel:
		mov			edx, dword ptr [ebx];	// 权重个数
		xorps		xmm6, xmm6;			// 存放结果
		movlps		xmm2, dword ptr [eax];	// 水平方向上的列首尾权重 @ @ powEnd powStart
		pshufd		xmm3, xmm2, 0x55;	// powerEnd
		shufps		xmm2, xmm2, 0x00;	// powerStart
		add			ebx, 0x04;
		add			eax, 0x08;
seg_first_col:		// 取得第一列的像素
		movaps		xmm1, dword ptr [esi];	// 源的一个像素
		mulps		xmm1, xmm2;
		addps		xmm6, xmm1;			// 加权求和
		add			esi, 0x10;
		sub			edx, 0x02;			// 判断占用的像素情况
		jb			map_end;			// 只占一个像素
		jnz			loop_mid_col;		// 占多于两个的像素
		jmp			seg_last_col;		// 只占两个像素
loop_mid_col:		// 取得中间列的像素
		movaps		xmm1, dword ptr [esi];	// 源的一个像素
		addps		xmm6, xmm1;			// 加权求和
		add			esi, 0x10;
		sub			edx, 0x01;
		jnz			loop_mid_col;
seg_last_col:		// 取得最后一列的像素
		movaps		xmm1, dword ptr [esi];		// 源的一个像素
		mulps		xmm1, xmm3;
		addps		xmm6, xmm1;			// 加权求和
		add			esi, 0x10;
map_end:		// 加权结束
		mulps		xmm6, xmm7;			// 平均 Y V U Y
		subps		xmm6, xmm0;			// 偏移 UV
		movaps		xmm1, xmm4;			// By Rv Bu Ry
		mulps		xmm1, xmm6;			// G
		mulps		xmm6, xmm5;			// RB
		movhlps		xmm2, xmm1;
		movhlps		xmm3, xmm6;
		addps		xmm2, xmm1;			// @  @  Gu  Gy+Gv
		addps		xmm3, xmm6;			// @  @  By+Bu  Ry+Rv
		shufps		xmm1, xmm1, 0x01;	// @  @  @  Gu
		addps		xmm1, xmm2;			// @  @  @  G
		movlhps		xmm1, xmm3;			// B  R  @  G
		mov			edx, bBGRA;			// 判断 BGRA 与 RGBA 的区别
		test		edx, edx;
		jnz			res_bgra;
res_rgba:
		shufps		xmm1, xmm1, 0x32;	// @  B  G  R
		jmp			res_write;
res_bgra:
		shufps		xmm1, xmm1, 0x23;	// @  R  G  B
res_write:
		movdqa		xmm2, gc_dwMaskAlpha;
		cvtps2dq	xmm1, xmm1;
		packssdw	xmm1, xmm1;
		packuswb	xmm1, xmm1;
		por			xmm1, xmm2;		// 填充 Alpha 通道
		movd		dword ptr [edi], xmm1;
		sub			esi, 0x10;		// 两个连续像素的共享，需要回退一个像素。
		add			edi, 0x04;
		sub			ecx, 0x01;
		jnz			loop_pixel;
	}
}
//
#define decl_align(type, n)			__declspec(align(n)) ##type
void rgb32_swaprb_line_sse2(OUT unsigned char *pucRGB32Out,	// 32 位输出缓冲（存储方式 X2 G X0 A）（R 与 B 可能互换）
							IN unsigned char *pucRGB32In,	// 32 位输入缓冲（存储方式 X0 G X2 A）（R 与 B 可能互换）
							IN int nWidth)					// 一行像素个数
{
	decl_align(unsigned int, 16) unMask_02[4] = { 0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff };
	decl_align(unsigned int, 16) unMask_13[4] = { 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00 };
	__asm {
		mov			ecx, nWidth;
		mov			esi, pucRGB32In;
		shr			ecx, 0x04;		// 一次处理 16 个像素
		mov			edi, pucRGB32Out;
		// 预取一部分的源数据到离处理器较近的 Cache 中，减少 Cache 污染，并提高命中
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		test		ecx, ecx;
		jnz			loop_p16;
		jmp			loop_m8;
loop_p16:
		// 预取一部分的源数据到离处理器较近的 Cache 中，减少 Cache 污染，并提高命中
		prefetchnta	byte ptr [esi + 0x40];
		movups		xmm0, xmmword ptr [esi];
		movups		xmm1, xmmword ptr [esi + 0x10];
		movups		xmm2, xmmword ptr [esi + 0x20];
		movups		xmm3, xmmword ptr [esi + 0x30];
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm1;
		movaps		xmm6, xmm2;
		movaps		xmm7, xmm3;
		pshuflw		xmm4, xmm4, 0xb1;		// 低 4 个字的交换
		pshuflw		xmm5, xmm5, 0xb1;
		pshuflw		xmm6, xmm6, 0xb1;
		pshuflw		xmm7, xmm7, 0xb1;
		pshufhw		xmm4, xmm4, 0xb1;		// 高 4 个字的交换
		pshufhw		xmm5, xmm5, 0xb1;
		pshufhw		xmm6, xmm6, 0xb1;
		pshufhw		xmm7, xmm7, 0xb1;
		andps		xmm0, unMask_13;		// A 0 G 0
		andps		xmm1, unMask_13;
		andps		xmm2, unMask_13;
		andps		xmm3, unMask_13;
		andps		xmm4, unMask_02;		// 0 X0 0 X2
		andps		xmm5, unMask_02;
		andps		xmm6, unMask_02;
		andps		xmm7, unMask_02;
		orps		xmm4, xmm0;
		orps		xmm5, xmm1;
		orps		xmm6, xmm2;
		orps		xmm7, xmm3;
		movups		xmmword ptr [edi], xmm4;
		movups		xmmword ptr [edi + 0x10], xmm5;
		movups		xmmword ptr [edi + 0x20], xmm6;
		movups		xmmword ptr [edi + 0x30], xmm7;
		add			esi, 0x40;
		add			edi, 0x40;
		sub			ecx, 0x01;
		jnz			loop_p16;
loop_m8:
		test		nWidth, 0x08;
		jz			loop_m4;
		movups		xmm0, xmmword ptr [esi];
		movups		xmm1, xmmword ptr [esi + 0x10];
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm1;
		pshuflw		xmm4, xmm4, 0xb1;		// 低 4 个字的交换
		pshuflw		xmm5, xmm5, 0xb1;
		pshufhw		xmm4, xmm4, 0xb1;		// 高 4 个字的交换
		pshufhw		xmm5, xmm5, 0xb1;
		andps		xmm0, unMask_13;		// A 0 G 0
		andps		xmm1, unMask_13;
		andps		xmm4, unMask_02;		// 0 X0 0 X2
		andps		xmm5, unMask_02;
		orps		xmm4, xmm0;
		orps		xmm5, xmm1;
		movups		xmmword ptr [edi], xmm4;
		movups		xmmword ptr [edi + 0x10], xmm5;
		add			esi, 0x20;
		add			edi, 0x20;
loop_m4:
		test		nWidth, 0x04;
		jz			loop_m2;
		movups		xmm0, xmmword ptr [esi];
		movaps		xmm4, xmm0;
		pshuflw		xmm4, xmm4, 0xb1;		// 低 4 个字的交换
		pshufhw		xmm4, xmm4, 0xb1;		// 高 4 个字的交换
		andps		xmm0, unMask_13;		// A 0 G 0
		andps		xmm4, unMask_02;		// 0 X0 0 X2
		orps		xmm4, xmm0;
		movups		xmmword ptr [edi], xmm4;
		add			esi, 0x10;
		add			edi, 0x10;
loop_m2:
		test		nWidth, 0x02;
		jz			loop_m1;
		movlps		xmm0, qword ptr [esi];
		movaps		xmm4, xmm0;
		pshuflw		xmm4, xmm4, 0xb1;		// 低 4 个字的交换
		andps		xmm0, unMask_13;		// A 0 G 0
		andps		xmm4, unMask_02;		// 0 X0 0 X2
		orps		xmm4, xmm0;
		movlps		qword ptr [edi], xmm4;
		add			esi, 0x08;
		add			edi, 0x08;
loop_m1:
		test		nWidth, 0x01;
		jz			loop_end;
		mov			eax, dword ptr [esi];
		mov			ecx, eax;
		shr			eax, 0x10;
		mov			byte ptr [edi], al;			// X2
		mov			byte ptr [edi + 0x01], ch;	// G
		mov			byte ptr [edi + 0x02], cl;	// X0
		mov			byte ptr [edi + 0x03], ah;	// A
loop_end:
	}
}

// 交换 RGBA 与 BGRA 的 R、B 通道
void rgb32_swaprbself(OUT void* rgb32,		// *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……
					  IN int pixelcount)	// 像素个数
{
	//assert(false);
	unsigned char *pucData = static_cast<unsigned char *>(rgb32);
	rgb32_swaprb_line_sse2(pucData,pucData,pixelcount);
}

//////////////////////////////////////////////////////////////////////////
//
// LaiShoubo @ 2009-11-5
//
// 按给定的A将24位转为32位
//
void color24_a_2_color32(OUT void* color32,			// 32位buffer
						 IN void* color24,			// 24位buffer
						 IN void* alpha,			// A通道
						 IN int count)				// 像素个数
{
	if(!color32 || !color24 || !alpha || count <= 0)
	{
		assert(false);
		return;
	}
	__asm
	{
		pcmpeqb		xmm7, xmm7;
		xorpd		xmm6, xmm6;
		psrld		xmm7, 08h;				// 00FFFFFF 00FFFFFF 00FFFFFF 00FFFFFF
		mov			ecx, count;
		mov			esi, color24;
		mov			edi, color32;
		mov			ebx, alpha;
		mov			edx, ecx;
		and			ecx, 7;
		shr			edx, 03h;				// 一次处理8个像素
		prefetchnta	[esi];
		test		edx, edx;
		jz			loop_1;
loop_8:
		prefetchnta	[esi + 18h];
		movlps		xmm0, [esi];			// **** **** GRBG RBGR
		movups		xmm1, [esi + 08h];		// BGRB GRBG RBGR BGRB
		movlps		xmm2, [ebx];			// **** **** AAAA AAAA
		movlhps		xmm0, xmm1;				// RBGR BGRB GRBG RBGR
		shufps		xmm1, xmm1, 0e9h;		// BGRB GRBG GRBG RBGR
		shufps		xmm0, xmm0, 94h;		// BGRB GRBG GRBG RBGR
		punpcklbw	xmm2, xmm6;				// 0A0A 0A0A 0A0A 0A0A
		pshuflw		xmm0, xmm0, 94h;
		pshuflw		xmm1, xmm1, 94h;
		pshufhw		xmm0, xmm0, 0e9h;
		pshufhw		xmm1, xmm1, 0e9h;		// BGRB RBGR BGRB RBGR
		movaps		xmm3, xmm0;
		shufps		xmm0, xmm1, 88h;
		shufps		xmm3, xmm1, 0ddh;
		movaps		xmm4, xmm2;
		punpcklwd	xmm2, xmm6;
		punpckhwd	xmm4, xmm6;
		pslld		xmm2, 18h;
		pslld		xmm4, 18h;
		andpd		xmm0, xmm7;
		psrld		xmm3, 08h;
		movaps		xmm1, xmm0;
		punpckldq	xmm0, xmm3;
		punpckhdq	xmm1, xmm3;
		orpd		xmm0, xmm2;
		orpd		xmm1, xmm4;
		movups		[edi], xmm0;
		movups		[edi + 10h], xmm1;
		add			edi, 20h;
		add			esi, 18h;
		add			ebx, 08h;
		dec			edx;
		jnz			loop_8;
loop_1:
		test		ecx, ecx;
		jz			loop_0;
		mov			al, [ebx];
		movsw
		movsb
		stosb
		inc			ebx;
		dec			ecx;
		jnz			loop_1;
loop_0:
	}
}

//////////////////////////////////////////////////////////////////////////
// file end