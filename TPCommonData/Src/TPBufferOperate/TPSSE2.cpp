// ���� SSE2 �Ż��㷨

#include "stdafx.h"
#include "TPSSE2.h"
#include "TPSSE2Static.h"
#include "TPColorSpaceConv.h"
#include <assert.h>
#include "CoreInclude\CPUInclude\NXColorSpace.h"
//////////////////////////////////////////////////////////////////////////
// ��������

#define PAL_SD_HEIGHT			576			// PAL ��ʽ SD �����
#define SPECIAL_RGBA_VALUE		0xff101010

//////////////////////////////////////////////////////////////////////////
// ��ʼ������
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
// ����һ�� Buffer
//////////////////////////////////////////////////////////////////////////
void memcpy_line(OUT void *des,		// Ŀ��
				 IN void *src,		// Դ
				 IN int na16,		// ��ȳ��� 16 ����
				 IN int nm16)		// ��ȳ��� 16 ������
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
// ���ֽ�Ϊ��λ����һ�� Buffer
//////////////////////////////////////////////////////////////////////////
void memset_char_line(OUT void *des,	// Ŀ��
					  IN char value,	// ����ֵ
					  IN int na16,		// ��ȳ��� 16 ����
					  IN int nm16)		// ��ȳ��� 16 ������
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
// ��˫��Ϊ��λ����һ�� Buffer
//////////////////////////////////////////////////////////////////////////
void memset_dword_line(OUT void *des,	// Ŀ��
					   IN DWORD value,	// ����ֵ
					   IN int na16,		// ��ȳ��� 16 ����
					   IN int nm16)		// ��ȳ��� 16 ������
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
// �����ֽ�
// ���Ѻ����������򣺵�һ�� n ��ʾ���Ѷ�����ֽڳ����ڶ��� n ��ʾ������ʼ�㣬
//		������ n ��ʾ�����ֽ��������� n ʱ��ʾ�������
//////////////////////////////////////////////////////////////////////////
void memsplit_n_n_n(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount,		// ���Ѷ���ĸ���
					IN int nUnitBytes,		// ���Ѷ�����ֽڳ�
					IN int nCellStart,		// ������ʼ��
					IN int nCellCount)		// �����ֽ���
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
		add			esi, ebx;		// �������
		sub			eax, ecx;		// ��һ�� unit ���������һ�� unit ��������λ�ò�
loop_unit:
		rep movsb;
		mov			ecx, nCellCount;
		add			esi, eax;
		sub			edx, 0x01;
		jnz			loop_unit;
loop_end:
	}
}
void memsplit_2_0_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			ecx, 0x04;		// һ���� 16 �� unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x40];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		andps		xmm0, xmm7;		// 16 λ�ĵ� 8 λ
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
void memsplit_2_1_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		shr			ecx, 0x04;		// һ���� 16 �� unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x40];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		psrlw		xmm0, 0x08;		// 16 λ�ĸ� 8 λ
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
void memsplit_4_0_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			ecx, 0x04;		// һ���� 16 �� unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		andps		xmm0, xmm7;		// 32 λ�ĵ� 8 λ
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
void memsplit_4_0_2(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			ecx, 0x04;		// һ���� 16 �� unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		andps		xmm0, xmm7;		// 32 λ�ĵ� 16 λ
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
void memsplit_4_0_3(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		shr			ecx, 0x03;		// һ���� 8 �� unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		movaps		xmm7, gc_dwMaskRGB;
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		andps		xmm0, xmm7;		// 32 λ�ĵ� 24 λ
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
void memsplit_4_1_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			edx, 0x04;		// һ���� 16 �� unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		psrlw		xmm0, 0x08;		// 32 λ�ĵ� 16 λ�ĸ� 8 λ
		psrlw		xmm1, 0x08;
		psrlw		xmm2, 0x08;
		psrlw		xmm3, 0x08;
		packssdw	xmm0, xmm1;		// 16 λ�ĸ� 8 λ��Ч
		packssdw	xmm2, xmm3;
		andps		xmm0, xmm7;		// 32 λ�ĵ� 16 λ�ĸ� 8 λ
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
void memsplit_4_1_2(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			edx, 0x04;		// һ���� 16 �� unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		pslld		xmm0, 0x08;		// 32 λ���� 16 λ
		pslld		xmm1, 0x08;
		pslld		xmm2, 0x08;
		pslld		xmm3, 0x08;
		psrld		xmm0, 0x10;		// 32 λ���� 16 λ
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
void memsplit_4_1_3(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
{
	__asm {
		mov			ecx, nUnitCount;
		test		ecx, ecx;
		jz			loop_end;
		mov			edi, des;
		mov			esi, src;
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		shr			ecx, 0x03;		// һ���� 8 �� unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		psrld		xmm0, 0x08;		// 32 λ�ĸ� 24 λ
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
void memsplit_4_2_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			edx, 0x04;		// һ���� 16 �� unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		andps		xmm0, xmm7;		// 32 λ�ĸ� 16 λ�ĵ� 8 λ
		andps		xmm1, xmm7;
		andps		xmm2, xmm7;
		andps		xmm3, xmm7;
		packssdw	xmm0, xmm1;		// 16 λ�ĵ� 8 λ��Ч
		packssdw	xmm2, xmm3;
		psrlw		xmm0, 0x08;		// 32 λ�ĸ� 16 λ�ĵ� 8 λ
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
void memsplit_4_2_2(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			edx, 0x04;		// һ���� 16 �� unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		psrld		xmm0, 0x10;		// 32 λ�ĸ� 16 λ
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
void memsplit_4_3_1(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			edx, 0x04;		// һ���� 16 �� unit
		test		ecx, ecx;
		jnz			loop_unit_16;
		jmp			loop_unit_m;
loop_unit_16:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		psrld		xmm0, 0x18;		// 32 λ�ĸ� 8 λ
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
void memsplit_8_0_4(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			edx, 0x03;		// һ���� 8 �� unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		shufps		xmm0, xmm1, 0x88;		// 64 λ�ĵ� 32 λ
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
void memsplit_8_4_4(OUT void *des,			// Ŀ��
					IN void *src,			// Դ
					IN int nUnitCount)		// ���Ѷ���ĸ���
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
		shr			edx, 0x03;		// һ���� 8 �� unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x80];
		movups		xmm0, dword ptr [esi];
		movups		xmm1, dword ptr [esi + 0x10];
		movups		xmm2, dword ptr [esi + 0x20];
		movups		xmm3, dword ptr [esi + 0x30];
		shufps		xmm0, xmm1, 0xdd;		// 64 λ�ĸ� 32 λ
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
// �ϳ��ֽ�
// �ϳɺ����������򣺵�һ�� n ��ʾ�ϳɶ�����ֽڳ����ڶ��� n ��ʾ�ϳ���ʼ�㣬
//		������ n ��ʾ�ϳ��ֽ��������� n ʱ��ʾ����ϳ�
//////////////////////////////////////////////////////////////////////////
void memcompose_n_n_n(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount,		// �ϳɶ���ĸ���
					  IN int nUnitBytes,		// �ϳɶ�����ֽڳ�
					  IN int nCellStart,		// �ϳ���ʼ��
					  IN int nCellCount)		// �ϳ��ֽ���
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
		add			edi, ebx;		// �ϳ����
		sub			eax, ecx;		// ��һ�� unit �ϳ������һ�� unit �ϳ�����λ�ò�
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
void memcompose_2_0_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			ecx, 0x04;		// һ���� 16 �� unit
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
		andps		xmm2, xmm7;		// ���� 16 λ�ĸ� 8 λ
		andps		xmm3, xmm7;
		punpcklbw	xmm1, xmm0;
		punpckhbw	xmm4, xmm0;
		orps		xmm1, xmm2;		// д�� 16 λ�ĵ� 8 λ
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
void memcompose_2_1_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			ecx, 0x04;		// һ���� 16 �� unit
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
		andps		xmm2, xmm7;		// ���� 16 λ�ĵ� 8 λ
		andps		xmm3, xmm7;
		punpcklbw	xmm4, xmm1;
		punpckhbw	xmm5, xmm1;
		orps		xmm4, xmm2;		// д�� 16 λ�ĸ� 8 λ
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
void memcompose_4_0_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			ecx, 0x03;		// һ���� 8 �� unit
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
		andps		xmm2, xmm7;		// ���� 32 λ�ĸ� 24 λ
		andps		xmm3, xmm7;
		movaps		xmm4, xmm1;
		punpcklwd	xmm1, xmm0;
		punpckhwd	xmm4, xmm0;
		orps		xmm1, xmm2;		// д�� 32 λ�ĵ� 8 λ
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
void memcompose_4_0_2(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			ecx, 0x03;		// һ���� 8 �� unit
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
		andps		xmm2, xmm7;		// ���� 32 λ�ĸ� 16 λ
		andps		xmm3, xmm7;
		punpcklwd	xmm1, xmm0;
		punpckhwd	xmm4, xmm0;
		orps		xmm1, xmm2;		// д�� 32 λ�ĵ� 16 λ
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
void memcompose_4_1_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			ecx, 0x03;		// һ���� 8 �� unit
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
		andps		xmm2, xmm7;		// ���� 32 λ�ĸ� 16 λ��� 16 λ�ĵ� 8 λ
		andps		xmm3, xmm7;
		movaps		xmm1, xmm4;
		punpcklwd	xmm4, xmm0;
		punpckhwd	xmm1, xmm0;
		orps		xmm4, xmm2;		// д�� 32 λ�ĵ� 16 λ�ĸ� 8 λ
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
void memcompose_4_2_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			ecx, 0x03;		// һ���� 8 �� unit
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
		andps		xmm2, xmm7;		// ���� 32 λ�ĸ� 16 λ�ĸ� 8 λ��� 16 λ
		andps		xmm3, xmm7;
		xorps		xmm4, xmm4;
		xorps		xmm5, xmm5;
		punpcklwd	xmm4, xmm1;
		punpckhwd	xmm5, xmm1;
		orps		xmm4, xmm2;		// д�� 32 λ�ĸ� 16 λ�ĵ� 8 λ
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
void memcompose_4_2_2(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			ecx, 0x03;		// һ���� 8 �� unit
		test		ecx, ecx;
		jnz			loop_unit_8;
		jmp			loop_unit_m;
loop_unit_8:
		prefetchnta	byte ptr [esi + 0x40];
		prefetchnta	byte ptr [edi + 0x40];
		movups		xmm1, dword ptr [esi];
		movups		xmm2, dword ptr [edi];
		movups		xmm3, dword ptr [edi + 0x10];
		andps		xmm2, xmm7;		// ���� 32 λ�ĵ� 16 λ
		andps		xmm3, xmm7;
		xorps		xmm4, xmm4;
		xorps		xmm5, xmm5;
		punpcklwd	xmm4, xmm1;
		punpckhwd	xmm5, xmm1;
		orps		xmm4, xmm2;		// д�� 32 λ�ĸ� 16 λ
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
void memcompose_4_3_1(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			ecx, 0x03;		// һ���� 8 �� unit
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
		andps		xmm2, xmm7;		// ���� 32 λ�ĵ� 24 λ
		andps		xmm3, xmm7;
		xorps		xmm4, xmm4;
		xorps		xmm5, xmm5;
		punpcklwd	xmm4, xmm0;
		punpckhwd	xmm5, xmm0;
		orps		xmm4, xmm2;		// д�� 32 λ�ĸ� 8 λ
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
void memcompose_8_0_4(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			edx, 0x03;		// һ���� 8 �� unit
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
		shufps		xmm4, xmm0, 0x4d;		// ���� 64 λ�ĸ� 32 λ��д��� 32 λ
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
void memcompose_8_4_4(OUT void *des,			// Ŀ��
					  IN void *src,				// Դ
					  IN int nUnitCount)		// �ϳɶ���ĸ���
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
		shr			edx, 0x03;		// һ���� 8 �� unit
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
		shufps		xmm4, xmm0, 0x48;		// ���� 64 λ�ĵ� 32 λ��д��� 32 λ
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
// ��һ�� BGRA32 Buffer ���� Alpha ��ɫ
// �Ż�����͸���� MA = SA + DA - SA * DA / 255 = SA + DA - (DA * SA + 255) >> 8
// ���õ����� KA = SA / MA = SA * 255 / (SA * 255 + DA * 255 - SA * DA)
// �Ż�������ɫֵ MC = (SC * SA + DC * (1 - SA / 255) * DA) / MA = (SC - DC) * KA + DC
// ������ƫ�Ƶ�ַ Offset = (SA * 256 + DA) * 4
//////////////////////////////////////////////////////////////////////////
void bgra32_line_mix(IN OUT void *des,		// Ŀ��
					 IN void *src,			// Դ
					 IN int na16,			// ��ȳ��� 16 ����
					 IN int nm16,			// ��ȳ��� 16 ������
					 IN void *ka_table)		// 256 * 256 * sizeof(float) �� KA ��
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
		// ���� (sa * 256 + da) * 4 ����������ڱ�Ϊ DWORD �ͣ�ת���ֽ�ʱ��Ҫ���Գ���
		movdqu		xmm6, [esi];	// src(A3R3G3B3 A2R2G2B2 A1R1G1B1 A0R0G0B0)
		movdqu		xmm7, [edi];	// des(A3R3G3B3 A2R2G2B2 A1R1G1B1 A0R0G0B0)
		movdqa		xmm4, xmm6;
		movdqa		xmm5, xmm7;
		psrld		xmm4, 0x18;		// sa(src alpha A3 A2 A1 A0)
		psrld		xmm5, 0x18;		// da(des alpha A3 A2 A1 A0)

		movd		xmm1, edx;		// ��ͷ 0 0 0 table
		pshufd		xmm1, xmm1, 0x00;	// table table table table

		movdqa		xmm2, xmm4;		// sa
		movdqa		xmm3, xmm4;		// sa
		pmullw		xmm4, xmm5;		// sa * da
		paddd		xmm3, xmm5;		// sa + da

		pslld		xmm2, 0x0a;		// sa * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// (sa * 256 + da) * 4 ��� Offset3 Offset2 Offset1 Offset0
		paddd		xmm2, xmm1;		// �����ַ ptr3 ptr2 ptr1 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		psubd		xmm3, xmm4;		// ���� MA = SA + DA - (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// ��Ž�� A000 A000 A000 A000
		//////////////////////////////////////////////////////////////////////////
		// ��������ȡ������ֵ
		movd		eax, xmm2;		// �� 0 �����ص����� ptr0
		pshufd		xmm2, xmm2, 0x39;	// ptr0 ptr3 ptr2 ptr1
		movd		xmm4, [eax];	// ��� 0 0 0 K0
		movd		eax, xmm2;		// �� 1 �����ص����� ptr1
		pshufd		xmm2, xmm2, 0x39;	// ptr1 ptr0 ptr3 ptr2
		movd		xmm5, [eax];	// ��� 0 0 0 K1
		movd		eax, xmm2;		// �� 2 �����ص����� ptr2
		pshufd		xmm2, xmm2, 0x39;	// ptr2 ptr1 ptr0 ptr3
		movd		xmm1, [eax];	// ��� 0 0 0 K2
		movd		eax, xmm2;		// �� 3 �����ص����� ptr3
		shufps		xmm4, xmm5, 0x00;	// K1 K1 K0 K0
		movd		xmm2, [eax];	// ��� 0 0 0 K3
		shufps		xmm1, xmm2, 0x00;	// K3 K3 K2 K2
		packssdw	xmm4, xmm1;		// K3K3 K2K2 K1K1 K0K0
		//////////////////////////////////////////////////////////////////////////
		// �ȼ��� R B �ٵ������� G
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
		paddw		xmm0, xmm0;		// (srb - drb + compRB) * 2 ����˷������ͳ˷���ת��
		paddw		xmm1, xmm1;		// (sg - dg + compG) * 2 ����˷������ͳ˷���ת��
		pmulhw		xmm0, xmm4;		// (srb - drb) * KA
		pmulhw		xmm1, xmm4;		// (sg - dg) * KA

		paddw		xmm0, xmm5;		// (srb - drb) * KA + drb
		paddw		xmm1, xmm7;		// (sg - dg) * KA + dg
		pslld		xmm1, 0x08;
		por			xmm3, xmm0;		// mix |= (mrb) ��� AR0B AR0B AR0B AR0B
		por			xmm3, xmm1;		// mix |= (mg << 8) ��� ARGB ARGB ARGB ARGB
		//////////////////////////////////////////////////////////////////////////
		// д�� Mix �������ʼ��һ���������ĸ����ؼ���
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

		movd		xmm1, edx;		// ��ͷ 0 0 0 table
		pshufd		xmm1, xmm1, 0x00;	// table table table table

		movdqa		xmm2, xmm4;		// sa
		movdqa		xmm3, xmm4;		// sa
		pmullw		xmm4, xmm5;		// sa * da
		paddd		xmm3, xmm5;		// sa + da

		pslld		xmm2, 0x0a;		// sa * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// (sa * 256 + da) * 4 ��� 0 0 Offset1 Offset0
		paddd		xmm2, xmm1;		// �����ַ 0 0 ptr1 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		psubd		xmm3, xmm4;		// ���� MA = SA + DA - (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// ��Ž�� 0000 0000 A000 A000
		//////////////////////////////////////////////////////////////////////////
		// ��������ȡ������ֵ
		movd		eax, xmm2;		// �� 0 �����ص����� ptr0
		pshufd		xmm2, xmm2, 0x39;	// ptr0 0 0 ptr1
		movd		xmm4, [eax];	// ��� 0 0 0 K0
		movd		eax, xmm2;		// �� 1 �����ص����� ptr1
		movd		xmm5, [eax];	// ��� 0 0 0 K1
		shufps		xmm4, xmm4, 0x00;	// K0 K0 K0 K0
		shufps		xmm5, xmm5, 0x00;	// K1 K1 K1 K1
		packssdw	xmm4, xmm5;		// K1K1 K1K1 K0K0 K0K0
		//////////////////////////////////////////////////////////////////////////
		// ͬʱ���� R G B
		pand		xmm6, gc_dwMaskRGB;// src RGB(0000 0000 0RGB 0RGB)
		punpcklbw	xmm6, xmm0;		// 000R 0G0B 000R 0G0B
		pand		xmm7, gc_dwMaskRGB;// des RGB(0000 0000 0RGB 0RGB)
		punpcklbw	xmm7, xmm0;		// 000R 0G0B 000R 0G0B
		movdqa		xmm2, xmm6;		// src RGB
		pcmpgtw		xmm2, xmm7;		// (src > des) ? -1 : 0
		psubw		xmm6, xmm7;		// src - des
		psubw		xmm6, xmm2;		// src - des + compRGB
		paddw		xmm6, xmm6;		// (src - des + compRGB) * 2 ����˷������ͳ˷���ת��
		pmulhw		xmm6, xmm4;		// (src - des) * KA
		paddw		xmm6, xmm7;		// (src - des) * KA + des
		packuswb	xmm6, xmm0;		// 0000 0000 0RGB 0RGB
		por			xmm3, xmm6;		// mix |= (RGB) ��� 0000 0000 ARGB ARGB
		//////////////////////////////////////////////////////////////////////////
		// д�� Mix ���
		movsd		[edi], xmm3;
		add			esi, 0x08;
		add			edi, 0x08;
loop_a4:
		test		ecx, 0x00000004;
		jz			loop_end;
		//////////////////////////////////////////////////////////////////////////
		movd		xmm1, edx;		// ��ͷ 0 0 0 table
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
		paddd		xmm2, xmm5;		// (sa * 256 + da) * 4 ��� 0 0 0 Offset0
		paddd		xmm2, xmm1;		// �����ַ 0 0 0 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		psubd		xmm3, xmm4;		// ���� MA = SA + DA - (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// ��Ž�� 0000 0000 0000 A000
		//////////////////////////////////////////////////////////////////////////
		// ��������ȡ������ֵ
		movd		eax, xmm2;		// �� 0 �����ص����� ptr0
		movd		xmm4, [eax];	// ��� 0 0 0 K0
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
		paddw		xmm6, xmm6;		// (src - des + compRGB) * 2 ����˷������ͳ˷���ת��
		pmulhw		xmm6, xmm4;		// (src - des) * KA
		paddw		xmm6, xmm7;		// (src - des) * KA + des
		packuswb	xmm6, xmm0;		// 0000 0000 0000 0RGB
		por			xmm3, xmm6;		// mix |= (RGB) ��� 0000 0000 0000 ARGB
		//////////////////////////////////////////////////////////////////////////
		// д�� Mix ���
		movd		[edi], xmm3;
loop_end:
	}
}

//////////////////////////////////////////////////////////////////////////
// ��һ�� BGRA32 Buffer ���� Alpha ����ɫ
// �Ż�����͸���� MA = (255 - SA) + DA - (255 - SA) * DA / 255 = 255 - SA + (DA * SA + 255) >> 8
// ���õ����� KA = (255 - SA) / MA = (65535 - SA * 255) / (65535 - SA * 255 + SA * DA)
// �Ż�������ɫֵ MC = (SC * (255 - SA) + DC * (1 - (255 - SA) / 255) * DA) / MA = (SC - DC) * KA + DC
// ������ƫ�Ƶ�ַ Offset = ((255 - SA) * 256 + DA) * 4
//////////////////////////////////////////////////////////////////////////
void bgra32_line_notmix(IN OUT void *des,		// Ŀ��
						IN void *src,			// Դ
						IN int na16,			// ��ȳ��� 16 ����
						IN int nm16,			// ��ȳ��� 16 ������
						IN void *ka_table)		// 256 * 256 * sizeof(float) �� KA ��
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
		// ���� ((255 - sa) * 256 + da) * 4 ����������ڱ�Ϊ DWORD �ͣ�ת���ֽ�ʱ��Ҫ���Գ���
		movdqu		xmm6, [esi];	// src(A3R3G3B3 A2R2G2B2 A1R1G1B1 A0R0G0B0)
		movdqu		xmm7, [edi];	// des(A3R3G3B3 A2R2G2B2 A1R1G1B1 A0R0G0B0)
		movdqa		xmm4, xmm6;
		movdqa		xmm5, xmm7;
		movdqa		xmm2, gc_dwMaskDLB;	// 255
		psrld		xmm4, 0x18;		// sa(src alpha A3 A2 A1 A0)
		psrld		xmm5, 0x18;		// da(des alpha A3 A2 A1 A0)

		movd		xmm1, edx;		// ��ͷ 0 0 0 table
		pshufd		xmm1, xmm1, 0x00;	// table table table table

		psubd		xmm2, xmm4;		// 255 - sa
		pmullw		xmm4, xmm5;		// sa * da
		movdqa		xmm3, xmm2;		// 255 - sa

		pslld		xmm2, 0x0a;		// (255 - sa) * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// ((255 - sa) * 256 + da) * 4 ��� Offset3 Offset2 Offset1 Offset0
		paddd		xmm2, xmm1;		// �����ַ ptr3 ptr2 ptr1 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		paddd		xmm3, xmm4;		// ���� MA = 255 - SA + (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// ��Ž�� A000 A000 A000 A000
		//////////////////////////////////////////////////////////////////////////
		// ��������ȡ������ֵ
		movd		eax, xmm2;		// �� 0 �����ص����� ptr0
		pshufd		xmm2, xmm2, 0x39;	// ptr0 ptr3 ptr2 ptr1
		movd		xmm4, [eax];	// ��� 0 0 0 K0
		movd		eax, xmm2;		// �� 1 �����ص����� ptr1
		pshufd		xmm2, xmm2, 0x39;	// ptr1 ptr0 ptr3 ptr2
		movd		xmm5, [eax];	// ��� 0 0 0 K1
		movd		eax, xmm2;		// �� 2 �����ص����� ptr2
		pshufd		xmm2, xmm2, 0x39;	// ptr2 ptr1 ptr0 ptr3
		movd		xmm1, [eax];	// ��� 0 0 0 K2
		movd		eax, xmm2;		// �� 3 �����ص����� ptr3
		shufps		xmm4, xmm5, 0x00;	// K1 K1 K0 K0
		movd		xmm2, [eax];	// ��� 0 0 0 K3
		shufps		xmm1, xmm2, 0x00;	// K3 K3 K2 K2
		packssdw	xmm4, xmm1;		// K3K3 K2K2 K1K1 K0K0
		//////////////////////////////////////////////////////////////////////////
		// �ȼ��� R B �ٵ������� G
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
		paddw		xmm0, xmm0;		// (srb - drb + compRB) * 2 ����˷������ͳ˷���ת��
		paddw		xmm1, xmm1;		// (sg - dg + compG) * 2 ����˷������ͳ˷���ת��
		pmulhw		xmm0, xmm4;		// (srb - drb) * KA
		pmulhw		xmm1, xmm4;		// (sg - dg) * KA

		paddw		xmm0, xmm5;		// (srb - drb) * KA + drb
		paddw		xmm1, xmm7;		// (sg - dg) * KA + dg
		pslld		xmm1, 0x08;
		por			xmm3, xmm0;		// mix |= (mrb) ��� AR0B AR0B AR0B AR0B
		por			xmm3, xmm1;		// mix |= (mg << 8) ��� ARGB ARGB ARGB ARGB
		//////////////////////////////////////////////////////////////////////////
		// д�� Mix �������ʼ��һ���������ĸ����ؼ���
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
		// ���� ((255 - sa) * 256 + da) * 4 ����������ڱ�Ϊ DWORD �ͣ�ת���ֽ�ʱ��Ҫ���Գ���
		movsd		xmm6, [esi];	// src(0000 0000 A1R1G1B1 A0R0G0B0)
		movsd		xmm7, [edi];	// des(0000 0000 A1R1G1B1 A0R0G0B0)
		movdqa		xmm4, xmm6;
		movdqa		xmm5, xmm7;
		movdqa		xmm2, gc_dwMaskDLB;	// 255
		psrld		xmm4, 0x18;		// sa(src alpha 0 0 A1 A0)
		psrld		xmm5, 0x18;		// da(des alpha 0 0 A1 A0)

		movd		xmm1, edx;		// ��ͷ 0 0 0 table
		pshufd		xmm1, xmm1, 0x00;	// table table table table

		psubd		xmm2, xmm4;		// 255 - sa
		pmullw		xmm4, xmm5;		// sa * da
		movdqa		xmm3, xmm2;		// 255 - sa

		pslld		xmm2, 0x0a;		// (255 - sa) * 256 * 4
		pslld		xmm5, 0x02;		// da * 4
		paddd		xmm2, xmm5;		// ((255 - sa) * 256 + da) * 4 ��� 0 0 Offset1 Offset0
		paddd		xmm2, xmm1;		// �����ַ 0 0 ptr1 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		paddd		xmm3, xmm4;		// ���� MA = 255 - SA + (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// ��Ž�� 0000 0000 A000 A000
		//////////////////////////////////////////////////////////////////////////
		// ��������ȡ������ֵ
		movd		eax, xmm2;		// �� 0 �����ص����� ptr0
		pshufd		xmm2, xmm2, 0x39;	// ptr0 0 0 ptr1
		movd		xmm4, [eax];	// ��� 0 0 0 K0
		movd		eax, xmm2;		// �� 1 �����ص����� ptr1
		movd		xmm5, [eax];	// ��� 0 0 0 K1
		shufps		xmm4, xmm4, 0x00;	// K0 K0 K0 K0
		shufps		xmm5, xmm5, 0x00;	// K1 K1 K1 K1
		packssdw	xmm4, xmm5;		// K1K1 K1K1 K0K0 K0K0
		//////////////////////////////////////////////////////////////////////////
		// ͬʱ���� R G B
		pand		xmm6, gc_dwMaskRGB;// src RGB(0000 0000 0RGB 0RGB)
		punpcklbw	xmm6, xmm0;		// 000R 0G0B 000R 0G0B
		pand		xmm7, gc_dwMaskRGB;// des RGB(0000 0000 0RGB 0RGB)
		punpcklbw	xmm7, xmm0;		// 000R 0G0B 000R 0G0B
		movdqa		xmm2, xmm6;		// src RGB
		pcmpgtw		xmm2, xmm7;		// (src > des) ? -1 : 0
		psubw		xmm6, xmm7;		// src - des
		psubw		xmm6, xmm2;		// src - des + compRGB
		paddw		xmm6, xmm6;		// (src - des + compRGB) * 2 ����˷������ͳ˷���ת��
		pmulhw		xmm6, xmm4;		// (src - des) * KA
		paddw		xmm6, xmm7;		// (src - des) * KA + des
		packuswb	xmm6, xmm0;		// 0000 0000 0RGB 0RGB
		por			xmm3, xmm6;		// mix |= (RGB) ��� 0000 0000 ARGB ARGB
		//////////////////////////////////////////////////////////////////////////
		// д�� Mix ���
		movsd		[edi], xmm3;
		add			esi, 0x08;
		add			edi, 0x08;
loop_a4:
		test		ecx, 0x00000004;
		jz			loop_end;
		//////////////////////////////////////////////////////////////////////////
		// ���� ((255 - sa) * 256 + da) * 4 ����������ڱ�Ϊ DWORD �ͣ�ת���ֽ�ʱ��Ҫ���Գ���
		movd		xmm1, edx;		// ��ͷ 0 0 0 table
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
		paddd		xmm2, xmm5;		// ((255 - sa) * 256 + da) * 4 ��� 0 0 0 Offset0
		paddd		xmm2, xmm1;		// �����ַ 0 0 0 ptr0

		paddd		xmm4, gc_dwMaskDLB;	// sa * da + 255
		psrld		xmm4, 0x08;		// (sa * da + 255) >> 8
		paddd		xmm3, xmm4;		// ���� MA = 255 - SA + (SA * DA + 255) >> 8
		pslld		xmm3, 0x18;		// ��Ž�� 0000 0000 0000 A000
		//////////////////////////////////////////////////////////////////////////
		// ��������ȡ������ֵ
		movd		eax, xmm2;		// �� 0 �����ص����� ptr0
		movd		xmm4, [eax];	// ��� 0 0 0 K0
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
		paddw		xmm6, xmm6;		// (src - des + compRGB) * 2 ����˷������ͳ˷���ת��
		pmulhw		xmm6, xmm4;		// (src - des) * KA
		paddw		xmm6, xmm7;		// (src - des) * KA + des
		packuswb	xmm6, xmm0;		// 0000 0000 0000 0RGB
		por			xmm3, xmm6;		// mix |= (RGB) ��� 0000 0000 0000 ARGB
		//////////////////////////////////////////////////////////////////////////
		// д�� Mix ���
		movd		[edi], xmm3;
loop_end:
	}
}

//////////////////////////////////////////////////////////////////////////
// ����˫���Բ�ֵ���� BGRA32 Buffer �������ţ�ʹ��Ȩ��������ӳ����������������ش�
// ͨ������ӳ���õ�Ŀ��������ӳ�䵽Դ�е���ʼλ�ã���������Ϊ�׵��ĸ��������ؽ��м�Ȩ��ֵ
// ���㹫ʽ		S00 * x * y				S10 * (1 - x) * y
//				S01 * x * (1 - y)		S11 * (1 - x) * (1 - y)
// D = S11 + (S00 - S10 - S01 + S11) * x * y + (S01 - S11) * x + (S10 - S11) * y
//
// lsb @ 2009-11-26 �㷨����
// S00 * (1 - x) * (1 - y)			S10 * x * (1 - y)
// S01 * x * (1 - y)				S11 * x * y
//////////////////////////////////////////////////////////////////////////
void bgra32_bl_copy_line(OUT void* des,		// Ŀ�꿽����
						 IN int dcw,		// Ŀ�꿽���еĿ��
						 IN void* src,		// Դ������
						 IN int src_pitch,	// Դͼһ�е��ֽ���
						 IN void* pPowerX,	// Ŀ��ӳ�䵽Դ�� X �����ϵ���Ȩ��
						 IN void* pMapX,	// Ŀ��ӳ�䵽Դ�� X �����ϵ�������
						 IN DWORD powerY)	// Ŀ����ӳ�䵽Դ�е���Ȩ��
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
		add			esi_ptr, src;		// 00�ĵ�ַ
		movsd		xmm0, [esi_ptr];	// 10 00
		add			esi_ptr, src_pitch;
		movsd		xmm1, [esi_ptr];	// 11 01
		punpcklbw	xmm0, xmm7;
		punpcklbw	xmm1, xmm7;
		psllw		xmm0, 2;
		psllw		xmm1, 2;			// �Ŵ�2λ
		pmulhuw		xmm0, xmm5;			// S@0 * (1 - y)
		pmulhuw		xmm1, xmm6;			// S@1 * y
		paddusw		xmm0, xmm4;
		movd		xmm2, [eax_ptr];	// x(1-x)
		paddusw		xmm0, xmm1;			// S0@ + S1@ �Ŵ�1λ
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
//		movd		xmm7, powerY;		// ��Ȩ�� 0000 0000 0000 0y0y
//		movdqa		xmm6, gc_dwMaskAll;	// ffffffff ffffffff ffffffff ffffffff
//		shufps		xmm7, xmm7, 0x00;	// 0y0y 0y0y 0y0y 0y0y
//		psubusw		xmm6, xmm7;			// 1 - y
//
//		mov 		edi, des;			// Ŀ����
//		mov 		esi, pPowerX;		// X �����ϵ�Ȩ�ر�
//		mov 		ecx, dcw;			// �п�
//		mov 		edx, pMapX;			// X �����ϵ�������
//		test		ecx, ecx;
//		jnz			loop_pixel;
//		jmp			loop_end;
//loop_pixel:
//		mov			eax, [edx];			// Ŀ�����ϵ�ӳ������ map
//		add			eax, src;			// ��һ���صĵ�ַ src + map
//
//		movsd		xmm1, [eax];		// 0 0 S10 S00
//		punpcklbw	xmm1, xmm0;			// 0a0r 0g0b 0a0r 0g0b
//		paddw		xmm1, gc_dwMaskWBIT;	// ����
//		pmulhuw		xmm1, xmm7;			// ((S@0 + comp) * y) >> 16
//
//		add			eax, src_pitch;		// ��һ�еĵ�ַ
//		add			edx, 0x04;
//
//		movsd		xmm2, [eax];		// 0 0 S11 S01
//		punpcklbw	xmm2, xmm0;			// 0a0r 0g0b 0a0r 0g0b
//		paddw		xmm2, gc_dwMaskWBIT;	// ����
//		pmulhuw		xmm2, xmm6;			// ((S@1 + comp) * (65535 - y)) >> 16
//
//		movd		xmm4, [esi];		// Ŀ�����ϵ�ӳ��Ȩ�� 0 0 0 (1-x)x
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
// ���ü�Ȩ��ֵ�˲����� BGRA32 Buffer �������ţ�ʹ��Ȩ��������ӳ����������������ش���
// �ȸ��ݴ�ֱ�����ϵ�ӳ���Դ�����м�Ȩƽ�����㣬������������ʱ������
// �������ˮƽ�����ϵ�ӳ�����ʱ��������м�Ȩƽ�����㣬�õ�ÿ�����ص�ֵ
//////////////////////////////////////////////////////////////////////////
void bgra32_mean_copy_add_y(OUT void* des,			// ��ʱ�����У���Դ�п�����ȵ��ı���
							IN void* src,			// Դ������ʼ��
							IN int scw,				// Դ�������
							IN int src_pitch,		// Դ���ֽ���
							IN void* pPowerY,		// Ŀ��ĳ��ӳ��Ĵ�ֱ�����ϵ�Ȩ�ر�
							IN void* pCoefH,		// ��ֱ�����ϵ�����ϵ��
							IN int nMapY)			// Ŀ��һ��ӳ�䵽Դ������
{
	_asm{
		mov			edi, des;
		mov			esi, src;
		mov			ecx, scw;
		mov			edx, pPowerY;
		movss		xmm2, [edx];		// ��ֱ�����ϵ���Ȩ�� 0 0 0 powStart
		movss		xmm3, [edx + 0x04];	// ��ֱ�����ϵ�βȨ�� 0 0 0 powEnd
		shufps		xmm2, xmm2, 0x00;	// power power power power
		shufps		xmm3, xmm3, 0x00;	// power power power power
		mov			eax, pCoefH;
		pxor		xmm0, xmm0;			// 0 0 0 0
		movaps		xmm7, [eax];		// ����ϵ�� coef coef coef coef
loop_pixel:
		mov			eax, nMapY;			// Ŀ����ӳ�䵽Դ������
		mov			edx, esi;
		pxor		xmm6, xmm6;			// ���Դ��һ�����ؼ�Ȩ���
		pxor		xmm5, xmm5;			// ����м��е����ͺ�
seg_first_row:
		// ȡ�õ�һ�е�����
		movss		xmm1, [edx];		// Դ���� 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		cvtdq2ps	xmm1, xmm1;
		mulps		xmm1, xmm2;
		add			edx, src_pitch;		// ��һ�е�ͬһ������
		addps		xmm6, xmm1;			// ��Ȩ���
		// �ж�ռ�õ��������
		sub			eax, 0x02;
		jb			map_end;			// ֻռһ������
		jnz			loop_mid_row;		// ռ��������������
		jmp			seg_last_row;		// ֻռ��������
loop_mid_row:
		// ȡ���м��е�����
		movd		xmm1, [edx];		// Դ���� 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		add			edx, src_pitch;		// ��һ�е�ͬһ������
		paddd		xmm5, xmm1;
		sub			eax, 0x01;
		jnz			loop_mid_row;
seg_last_row:
		// ȡ�����һ�е�����
		movss		xmm1, [edx];		// Դ���� 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		cvtdq2ps	xmm1, xmm1;
		mulps		xmm1, xmm3;
		cvtdq2ps	xmm5, xmm5;
		addps		xmm6, xmm5;			// ��Ȩ���
		addps		xmm6, xmm1;			// ��Ȩ���
map_end:
		// ��Ȩ����
		mulps		xmm6, xmm7;			// ƽ��
		movaps		[edi], xmm6;
		add			esi, 0x04;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_pixel;
	}
}

// �����ű���С��255�ĸ�Ч�㷨��
void bgra32_mean_copy_add_y_quick(OUT void* des,		// ��ʱ�����У���Դ�п�����ȵ��ı���
								  IN void* src,			// Դ������ʼ��
								  IN int scw,			// Դ�������
								  IN int src_pitch,		// Դ���ֽ���
								  IN void* pPowerY,		// Ŀ��ĳ��ӳ��Ĵ�ֱ�����ϵ�Ȩ�ر�
								  IN void* pCoefH,		// ��ֱ�����ϵ�����ϵ��
								  IN int nMapY)			// Ŀ��һ��ӳ�䵽Դ������
{
	_asm{
		mov			edi, des;
		mov			esi, src;
		mov			ecx, scw;
		mov			edx, pPowerY;
		movss		xmm2, [edx];		// ��ֱ�����ϵ���Ȩ�� 0 0 0 powStart
		movss		xmm3, [edx + 0x04];	// ��ֱ�����ϵ�βȨ�� 0 0 0 powEnd
		shufps		xmm2, xmm2, 0x00;	// power power power power
		shufps		xmm3, xmm3, 0x00;	// power power power power
		mov			eax, pCoefH;
		pxor		xmm0, xmm0;			// 0 0 0 0
		movaps		xmm7, [eax];		// ����ϵ�� coef coef coef coef
loop_pixel:
		mov			eax, nMapY;			// Ŀ����ӳ�䵽Դ������
		mov			edx, esi;
		pxor		xmm6, xmm6;			// ���Դ��һ�����ؼ�Ȩ���
		pxor		xmm5, xmm5;			// ����м��е����ͺ�
seg_first_row:
		// ȡ�õ�һ�е�����
		movss		xmm1, [edx];		// Դ���� 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		cvtdq2ps	xmm1, xmm1;
		mulps		xmm1, xmm2;
		add			edx, src_pitch;		// ��һ�е�ͬһ������
		addps		xmm6, xmm1;			// ��Ȩ���
		// �ж�ռ�õ��������
		sub			eax, 0x02;
		jb			map_end;			// ֻռһ������
		jnz			loop_mid_row;		// ռ��������������
		jmp			seg_last_row;		// ֻռ��������
loop_mid_row:
		// ȡ���м��е�����
		movd		xmm1, [edx];		// Դ���� 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		add			edx, src_pitch;		// ��һ�е�ͬһ������
		paddusw		xmm5, xmm1;
		sub			eax, 0x01;
		jnz			loop_mid_row;
seg_last_row:
		// ȡ�����һ�е�����
		movss		xmm1, [edx];		// Դ���� 0 0 0 argb
		punpcklbw	xmm1, xmm0;			// 0000 0000 0a0r 0g0b
		punpcklwd	xmm1, xmm0;			// 000a 000r 000g 000b
		cvtdq2ps	xmm1, xmm1;
		punpcklwd	xmm5, xmm0;			// 000a 000r 000g 000b
		mulps		xmm1, xmm3;
		cvtdq2ps	xmm5, xmm5;
		addps		xmm6, xmm5;			// ��Ȩ���
		addps		xmm6, xmm1;			// ��Ȩ���
map_end:
		// ��Ȩ����
		mulps		xmm6, xmm7;			// ƽ��
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
		movaps		xmm7, [eax];		// ����ϵ�� coef coef coef coef
		mov			edi, des;
		mov			esi, src;
		mov			edx, pMapX;
		mov			eax, pPowerX;
loop_pixel:
		push		ecx;				// ����һ��ʣ����
		pxor		xmm6, xmm6;			// ��Ž��
		mov			ecx, [edx];			// Ȩ�ظ���
		add			edx, 0x04;
		movss		xmm2, [eax];		// ˮƽ�����ϵ�����Ȩ�� 0 0 0 powStart
		movss		xmm3, [eax + 0x04];	// ˮƽ�����ϵ���βȨ�� 0 0 0 powEnd
		add			eax, 0x08;
		shufps		xmm2, xmm2, 0x00;	// power power power power
		shufps		xmm3, xmm3, 0x00;	// power power power power
seg_first_col:
		// ȡ�õ�һ�е�����
		movaps		xmm1, [esi];		// Դ��һ������
		mulps		xmm1, xmm2;
		add			esi, 0x10;
		addps		xmm6, xmm1;			// ��Ȩ���
		// �ж�ռ�õ��������
		sub			ecx, 0x02;
		jb			map_end;			// ֻռһ������
		jnz			loop_mid_col;		// ռ��������������
		jmp			seg_last_col;		// ֻռ��������
loop_mid_col:
		// ȡ���м��е�����
		movaps		xmm1, [esi];		// Դ��һ������
		add			esi, 0x10;
		addps		xmm6, xmm1;			// ��Ȩ���
		sub			ecx, 0x01;
		jnz			loop_mid_col;
seg_last_col:
		// ȡ�����һ�е�����
		movaps		xmm1, [esi];		// Դ��һ������
		mulps		xmm1, xmm3;
		add			esi, 0x10;
		addps		xmm6, xmm1;			// ��Ȩ���
map_end:
		// ��Ȩ����
		mulps		xmm6, xmm7;			// ƽ�� 000a 000r 000g 000b
		pop			ecx;				// ȡ��һ��ʣ����
		cvtps2dq	xmm6, xmm6;
		packssdw	xmm6, xmm6;			// ѹ����� 0a0r 0g0b 0a0r 0g0b
		packuswb	xmm6, xmm6;			// argb argb argb argb
		movd		[edi], xmm6;
		sub			esi, 0x10;
		add			edi, 0x04;
		sub			ecx, 0x01;
		jnz			loop_pixel;
	}
}

//////////////////////////////////////////////////////////////////////////

// ��ɫ�任���𳡣������߽��жϣ�

void color32_to_yuyva_frame(OUT BYTE* pDesYUYV,         // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							IN int nYUYVPitch,
							OUT BYTE* pDesAlpha,        // Alpha ������洢��ʽ A0 A1 A2 A3 A4 ������
							IN int nAlphaPitch,
							IN BYTE* pSrc32,            // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
							IN int nSrcPitch,           // Դһ�е��ֽڿ��
							IN int nWidth,		        // Դͼ��С
							IN int nHeight,
							IN BOOL bHD,                // �߱����־
							IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
							IN BOOL bBGRA)              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	int na16 = (nWidth >> 2), nm16 = ((nWidth & 0x03) << 2);
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc32 + (nHeight - 1) * nSrcPitch) : pSrc32;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;

	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyva_inner(pDesYUYV, pDesAlpha, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYV += nYUYVPitch;
				pDesAlpha += nAlphaPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
		else    // R �ڵ�λ
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
	else    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyva_inner(pDesYUYV, pDesAlpha, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYV += nYUYVPitch;
				pDesAlpha += nAlphaPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
		else    // R �ڵ�λ
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
void color32_to_yuyva_split(OUT BYTE* pDesYUYVF,        // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							IN int nYUYVPitchF,
							OUT BYTE* pDesAlphaF,       // Alpha ��һ��������洢��ʽ A0 A1 A2 A3 A4 ������
							IN int nAlphaPitchF,
							OUT BYTE* pDesYUYVS,        // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							IN int nYUYVPitchS,
							OUT BYTE* pDesAlphaS,       // Alpha �ڶ���������洢��ʽ A0 A1 A2 A3 A4 ������
							IN int nAlphaPitchS,
							IN BYTE* pSrc32,            // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
							IN int nSrcPitch,           // Դһ�е��ֽڿ��
							IN int nWidth,              // Դͼ��С
							IN int nHeight,
							IN BOOL bHD,                // �߱����־
							IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
							IN BOOL bBGRA)              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	int na16 = (nWidth >> 2), nm16 = ((nWidth & 0x03) << 2);
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc32 + (nHeight - 1) * nSrcPitch) : pSrc32;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;
	int nHalfHeight = (nHeight >> 1);

	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYVF += nYUYVPitchF;
				pDesAlphaF += nAlphaPitchF;
				pSrcLine += nRealSrcPitch;
				// �ڶ���
				color32_to_yuyva_inner(pDesYUYVS, pDesAlphaS, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYVS += nYUYVPitchS;
				pDesAlphaS += nAlphaPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				// �ڶ���
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // ������
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
				nAlphaPitchS = min(nAlphaPitchF, nAlphaPitchS);      // ������
				na16 = (nAlphaPitchS >> 4), nm16 = (nAlphaPitchS & 0x0f);
				memcpy_line(pDesAlphaS, pDesAlphaF, na16, nm16);
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYVF += nYUYVPitchF;
				pDesAlphaF += nAlphaPitchF;
				pSrcLine += nRealSrcPitch;
				// �ڶ���
				color32_to_yuyva_inner(pDesYUYVS, pDesAlphaS, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYVS += nYUYVPitchS;
				pDesAlphaS += nAlphaPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				// �ڶ���
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // ������
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
				nAlphaPitchS = min(nAlphaPitchF, nAlphaPitchS);      // ������
				na16 = (nAlphaPitchS >> 4), nm16 = (nAlphaPitchS & 0x0f);
				memcpy_line(pDesAlphaS, pDesAlphaF, na16, nm16);
			}
		}
	}
	else    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYVF += nYUYVPitchF;
				pDesAlphaF += nAlphaPitchF;
				pSrcLine += nRealSrcPitch;
				// �ڶ���
				color32_to_yuyva_inner(pDesYUYVS, pDesAlphaS, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYVS += nYUYVPitchS;
				pDesAlphaS += nAlphaPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				// �ڶ���
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // ������
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
				nAlphaPitchS = min(nAlphaPitchF, nAlphaPitchS);      // ������
				na16 = (nAlphaPitchS >> 4), nm16 = (nAlphaPitchS & 0x0f);
				memcpy_line(pDesAlphaS, pDesAlphaF, na16, nm16);
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYVF += nYUYVPitchF;
				pDesAlphaF += nAlphaPitchF;
				pSrcLine += nRealSrcPitch;
				// �ڶ���
				color32_to_yuyva_inner(pDesYUYVS, pDesAlphaS, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYVS += nYUYVPitchS;
				pDesAlphaS += nAlphaPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color32_to_yuyva_inner(pDesYUYVF, pDesAlphaF, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				// �ڶ���
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // ������
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
				nAlphaPitchS = min(nAlphaPitchF, nAlphaPitchS);      // ������
				na16 = (nAlphaPitchS >> 4), nm16 = (nAlphaPitchS & 0x0f);
				memcpy_line(pDesAlphaS, pDesAlphaF, na16, nm16);
			}
		}
	}
}

void color32_to_yuyv_frame(OUT BYTE* pDesYUYV,         // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nYUYVPitch,
						   IN BYTE* pSrc32,            // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN int nSrcPitch,           // Դһ�е��ֽڿ��
						   IN int nWidth,		        // Դͼ��С
						   IN int nHeight,
						   IN BOOL bHD,                // �߱����־
						   IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
						   IN BOOL bBGRA)              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	int na16 = (nWidth >> 2), nm16 = ((nWidth & 0x03) << 2);
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc32 + (nHeight - 1) * nSrcPitch) : pSrc32;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;

	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyv_inner(pDesYUYV, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYV += nYUYVPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyv_inner(pDesYUYV, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYV += nYUYVPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
	}
	else    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				color32_to_yuyv_inner(pDesYUYV, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYV += nYUYVPitch;
				pSrcLine += nRealSrcPitch;
			}
		}
		else    // R �ڵ�λ
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
void color32_to_yuyv_split(OUT BYTE* pDesYUYVF,        // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nYUYVPitchF,
						   OUT BYTE* pDesYUYVS,        // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nYUYVPitchS,
						   IN BYTE* pSrc32,            // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN int nSrcPitch,           // Դһ�е��ֽڿ��
						   IN int nWidth,              // Դͼ��С
						   IN int nHeight,
						   IN BOOL bHD,                // �߱����־
						   IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
						   IN BOOL bBGRA)              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	int na16 = (nWidth >> 2), nm16 = ((nWidth & 0x03) << 2);
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc32 + (nHeight - 1) * nSrcPitch) : pSrc32;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;
	int nHalfHeight = (nHeight >> 1);

	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYVF += nYUYVPitchF;
				pSrcLine += nRealSrcPitch;
				// �ڶ���
				color32_to_yuyv_inner(pDesYUYVS, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				pDesYUYVS += nYUYVPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
				// �ڶ���
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // ������
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYVF += nYUYVPitchF;
				pSrcLine += nRealSrcPitch;
				// �ڶ���
				color32_to_yuyv_inner(pDesYUYVS, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				pDesYUYVS += nYUYVPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
				// �ڶ���
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // ������
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
			}
		}
	}
	else    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYVF += nYUYVPitchF;
				pSrcLine += nRealSrcPitch;
				// �ڶ���
				color32_to_yuyv_inner(pDesYUYVS, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				pDesYUYVS += nYUYVPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
				// �ڶ���
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // ������
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYVF += nYUYVPitchF;
				pSrcLine += nRealSrcPitch;
				// �ڶ���
				color32_to_yuyv_inner(pDesYUYVS, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				pDesYUYVS += nYUYVPitchS;
				pSrcLine += nRealSrcPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color32_to_yuyv_inner(pDesYUYVF, pSrcLine, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
				// �ڶ���
				nYUYVPitchS = min(nYUYVPitchF, nYUYVPitchS);      // ������
				na16 = (nYUYVPitchS >> 4), nm16 = (nYUYVPitchS & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesYUYVS, pDesYUYVF, na16, nm16);
			}
		}
	}
}

void yuyv_to_color32_compose(OUT BYTE* pDes32,          // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
							 IN int nDesPitch,
							 IN BYTE* pSrcYUYVF,        // YUYV ��һ�����루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							 IN int nFirstPitch,
							 IN BYTE* pSrcYUYVS,        // YUYV �ڶ������루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							 IN int nSecondPitch,
							 IN int nWidth,             // Ŀ���С
							 IN int nHeight,
							 IN BOOL bHD,               // �߱����־
							 IN BOOL bBGRA,             // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
							 IN BOOL bDstRevert)		// ͼ���Ƿ��з�ת��־
{
	int nRealYUYVPitch = ((nWidth + (nWidth & 0x01)) << 1);
	int na16 = ((nRealYUYVPitch - 4) >> 4), nm16 = (nRealYUYVPitch - (na16 << 4));
	int nHalfHeight = (nHeight >> 1);

	BYTE* pDstLine = (bDstRevert) ? (pDes32 + (nHeight - 1) * nDesPitch) : pDes32;
	int nRealDstPitch = (bDstRevert) ? -nDesPitch : nDesPitch;

	if (bHD)        // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYVF += nFirstPitch;
				// �ڶ���
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVS, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYVS += nSecondPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦��ֻ���õ�һ��
			{
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYVF += nFirstPitch;
				// �ڶ���
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVS, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYVS += nSecondPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦��ֻ���õ�һ��
			{
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
		}
	}
	else            // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYVF += nFirstPitch;
				// �ڶ���
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVS, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYVS += nSecondPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦��ֻ���õ�һ��
			{
				yuyv_to_bgra_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYVF += nFirstPitch;
				// �ڶ���
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVS, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYVS += nSecondPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦��ֻ���õ�һ��
			{
				yuyv_to_rgba_inner(pDstLine, pSrcYUYVF, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			}
		}
	}
}

void yuyv_to_color32_frame(OUT BYTE* pDes32,          // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
						   IN int nDesPitch,
						   IN BYTE* pSrcYUYV,        // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nSrcPitch,
						   IN int nWidth,             // Ŀ���С
						   IN int nHeight,
						   IN BOOL bHD,               // �߱����־
						   IN BOOL bBGRA,			// ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
						   IN BOOL bDstRevert)		// ͼ���Ƿ��з�ת��־
{
	int nRealYUYVPitch = ((nWidth + (nWidth & 0x01)) << 1);
	int na16 = ((nRealYUYVPitch - 4) >> 4), nm16 = (nRealYUYVPitch - (na16 << 4));
	BYTE* pDstLine = (bDstRevert) ? (pDes32 + (nHeight - 1) * nDesPitch) : pDes32;
	int nRealDstPitch = (bDstRevert) ? -nDesPitch : nDesPitch;

	if (bHD)        // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				yuyv_to_bgra_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				// ��һ��
				yuyv_to_rgba_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
	}
	else            // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				// ��һ��
				yuyv_to_bgra_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				// ��һ��
				yuyv_to_rgba_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
	}
}

void color24_to_yuyv_frame(OUT BYTE* pDesYUYV,         // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nYUYVPitch,
						   IN BYTE* pSrc24,            // *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN int nSrcPitch,           // Դһ�е��ֽڿ��
						   IN int nWidth,		       // Դͼ��С
						   IN int nHeight,
						   IN BOOL bHD,                // �߱����־
						   IN BOOL bSrcRevert,         // ͼ���Ƿ��з�ת��־
						   IN BOOL bBGR)               // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	int na12 = ((nSrcPitch - 4) / 12), nmp = (nWidth - (na12 << 2));
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc24 + (nHeight - 1) * nSrcPitch) : pSrc24;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;

	if (bHD)       // ����
	{
		if (bBGR)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				color24_to_yuyv_inner(pDesYUYV, pSrcLine, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
				pSrcLine += nRealSrcPitch;
				pDesYUYV += nYUYVPitch;
			}
		}
		else        // R �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				color24_to_yuyv_inner(pDesYUYV, pSrcLine, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
				pSrcLine += nRealSrcPitch;
				pDesYUYV += nYUYVPitch;
			}
		}
	}
	else        // ����
	{
		if (bBGR)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				color24_to_yuyv_inner(pDesYUYV, pSrcLine, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
				pSrcLine += nRealSrcPitch;
				pDesYUYV += nYUYVPitch;
			}
		}
		else        // R �ڵ�λ
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
void color24_to_yuyv_split(OUT BYTE* pDesFirst,          // ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nFirstPitch,
						   OUT BYTE* pDesSecond,         // �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nSecondPitch,
						   IN BYTE* pSrc24,              // *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN int nSrcPitch,             // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
						   IN int nWidth,			     // Դͼ��С
						   IN int nHeight,
						   IN BOOL bHD,                  // �߱����־
						   IN BOOL bSrcRevert,           // ͼ���Ƿ��з�ת��־
						   IN BOOL bBGR)                 // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	int na12 = ((nSrcPitch - 4) / 12), nmp = (nWidth - (na12 << 2));
	BYTE* pSrcLine = (bSrcRevert) ? (pSrc24 + (nHeight - 1) * nSrcPitch) : pSrc24;
	int nRealSrcPitch = (bSrcRevert) ? -nSrcPitch : nSrcPitch;
	int nHalfHeight = (nHeight >> 1);

	if (bHD)       // ����
	{
		if (bBGR)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
				pSrcLine += nRealSrcPitch;
				pDesFirst += nFirstPitch;
				// �ڶ���
				color24_to_yuyv_inner(pDesSecond, pSrcLine, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
				pSrcLine += nRealSrcPitch;
				pDesSecond += nSecondPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
				// �ڶ���
				nSecondPitch = min(nFirstPitch, nSecondPitch);      // ������
				na12 = (nSecondPitch >> 4), nmp = (nSecondPitch & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesSecond, pDesFirst, na12, nmp);
			}
		}
		else        // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
				pSrcLine += nRealSrcPitch;
				pDesFirst += nFirstPitch;
				// �ڶ���
				color24_to_yuyv_inner(pDesSecond, pSrcLine, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
				pSrcLine += nRealSrcPitch;
				pDesSecond += nSecondPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
				// �ڶ���
				nSecondPitch = min(nFirstPitch, nSecondPitch);      // ������
				na12 = (nSecondPitch >> 4), nmp = (nSecondPitch & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesSecond, pDesFirst, na12, nmp);
			}
		}
	}
	else        // ����
	{
		if (bBGR)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
				pSrcLine += nRealSrcPitch;
				pDesFirst += nFirstPitch;
				// �ڶ���
				color24_to_yuyv_inner(pDesSecond, pSrcLine, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
				pSrcLine += nRealSrcPitch;
				pDesSecond += nSecondPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
				// �ڶ���
				nSecondPitch = min(nFirstPitch, nSecondPitch);      // ������
				na12 = (nSecondPitch >> 4), nmp = (nSecondPitch & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesSecond, pDesFirst, na12, nmp);
			}
		}
		else        // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_RGB_SD, g_nY32_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD, g_nUV_BGR_SD);
				pSrcLine += nRealSrcPitch;
				pDesFirst += nFirstPitch;
				// �ڶ���
				color24_to_yuyv_inner(pDesSecond, pSrcLine, na12, nmp, g_nY24_RGB_SD, g_nY32_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD, g_nUV_BGR_SD);
				pSrcLine += nRealSrcPitch;
				pDesSecond += nSecondPitch;
			}
			if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
			{
				// ��һ��
				color24_to_yuyv_inner(pDesFirst, pSrcLine, na12, nmp, g_nY24_RGB_SD, g_nY32_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD, g_nUV_BGR_SD);
				// �ڶ���
				nSecondPitch = min(nFirstPitch, nSecondPitch);      // ������
				na12 = (nSecondPitch >> 4), nmp = (nSecondPitch & 0x0f);
				//memset_dword_line(pDesSecond, CS_YUYV_BLACK_VALUE, na12, nmp);
				memcpy_line(pDesSecond, pDesFirst, na12, nmp);
			}
		}
	}
}

void yuyv_to_color24_frame(OUT BYTE* pDes24,          // *G@A ���棬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN int nDesPitch,
						   IN BYTE* pSrcYUYV,        // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nSrcPitch,
						   IN int nWidth,             // Ŀ���С
						   IN int nHeight,
						   IN BOOL bHD,               // �߱����־
						   IN BOOL bBGR,				// ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
						   IN BOOL bDstRevert)			// ͼ���Ƿ��з�ת��־
{
	int nRealYUYVPitch = ((nWidth + (nWidth & 0x01)) << 1);
	int na16 = ((nRealYUYVPitch - 4) >> 4), nm16 = (nRealYUYVPitch - (na16 << 4));
	BYTE* pDstLine = (bDstRevert) ? (pDes24 + (nHeight - 1) * nDesPitch) : pDes24;
	int nRealDstPitch = (bDstRevert) ? -nDesPitch : nDesPitch;

	if (bHD)        // ����
	{
		if (bBGR)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				yuyv_to_color24_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				// ��һ��
				yuyv_to_color24_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
	}
	else            // ����
	{
		if (bBGR)      // B �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				// ��һ��
				yuyv_to_color24_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHeight; i++)
			{
				// ��һ��
				yuyv_to_color24_inner(pDstLine, pSrcYUYV, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDstLine += nRealDstPitch;
				pSrcYUYV += nSrcPitch;
			}
		}
	}
}

void yuv420_to_color32_frame(OUT BYTE* pDes32,           // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
							 IN int nDesPitch,
							 IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
							 IN int nSrcYPitch,
							 IN BYTE* pSrcU,             // U ���루�洢��ʽ UU0 UU2 UU4 �����������й���һ����
							 IN int nSrcUPitch,
							 IN BYTE* pSrcV,             // V ���루�洢��ʽ VV0 VV2 VV4 �����������й���һ����
							 IN int nSrcVPitch,
							 IN int nWidth,              // Ŀ���С
							 IN int nHeight,
							 IN BOOL bHD,                // �߱����־
							 IN BOOL bDesRevert,         // ͼ���Ƿ��з�ת��־
							 IN BOOL bBGRA)              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	g_NXColorSpace->pStuYUV420toOthers->pfnYUV420_2_RGB32_Frame(pDes32, nDesPitch, \
		pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD ? NXVIDEO_DEFINITION_HIGH : NXVIDEO_DEFINITION_STANDARD, \
		bBGRA != FALSE, 0xff, bDesRevert != FALSE);
	return;

	int nHalfHeight = (nHeight >> 1), nHalfHalf = ((nHalfHeight + 0) >> 1);
	int nUVPitch = min(nSrcUPitch, nSrcVPitch);
	int na16 = (nWidth >> 5), nm16 = ((nWidth >> 1) & 0x0f);	// һ�����ؿ�ȵ�һ�루��Ϊһ���ڵ� UV �� Y Ϊ 422 �Ĺ�ϵ��
	int na8 = ((nWidth - 1) >> 3), nm8 = (nWidth - (na8 << 3));
	int nYP2 = (nSrcYPitch << 1), nUP2 = (nSrcUPitch << 1), nVP2 = (nSrcVPitch << 1);
	// ���û��棬�Ȳ�ֵ����� 422 �� UV �źţ����У�����ʹ�� 422 �� color32 �Ľӿ�
	BYTE* pTmpU = TPSSE_NEW((nUVPitch << 1), BYTE);
	BYTE* pTmpV = TPSSE_NEW((nUVPitch << 1), BYTE);
	BYTE *pU1 = pTmpU, *pU2 = pTmpU + nUVPitch, *pV1 = pTmpV, *pV2 = pTmpV + nUVPitch;
	// ��ת
	BYTE* pDesF = (bDesRevert) ? (pDes32 + (nHeight - 1) * nDesPitch) : pDes32;
	int nRealDesPitch = (bDesRevert) ? -nDesPitch : nDesPitch;
	BYTE* pDesS = pDesF + nRealDesPitch;
	int nRealDP2 = (bDesRevert) ? -(nDesPitch << 1) : (nDesPitch << 1);
	BYTE *pYF = pSrcY, *pYS = pSrcY + nSrcYPitch;

	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			// ǰ����
			// ��һ��
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesF += nRealDP2; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesF += nRealDP2; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �ڶ���
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesS += nRealDP2; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesS += nRealDP2; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �м���
			for (int i=2; i<nHalfHalf; i++)
			{
				// ��һ��
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealDP2; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealDP2; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// ������л�һ�У���Ϊ������ʱ��
			if (nHalfHalf & 0x01)       // һ��
			{
				// ��һ��
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			}
			else    // ����
			{
				// ��һ��
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			}
		}
		else        // R �ڵ�λ
		{
			// ǰ����
			// ��һ��
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesF += nRealDP2; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesF += nRealDP2; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �ڶ���
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesS += nRealDP2; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesS += nRealDP2; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �м���
			for (int i=2; i<nHalfHalf; i++)
			{
				// ��һ��
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealDP2; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealDP2; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// ������л�һ�У���Ϊ������ʱ��
			if (nHalfHalf & 0x01)       // һ��
			{
				// ��һ��
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
			else    // ����
			{
				// ��һ��
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
		}
	}
	else        // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			// ǰ����
			// ��һ��
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesF += nRealDP2; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesF += nRealDP2; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �ڶ���
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesS += nRealDP2; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesS += nRealDP2; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �м���
			for (int i=2; i<nHalfHalf; i++)
			{
				// ��һ��
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealDP2; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealDP2; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// ������л�һ�У���Ϊ������ʱ��
			if (nHalfHalf & 0x01)       // һ��
			{
				// ��һ��
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
					yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				}
			}
			else    // ����
			{
				// ��һ��
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealDP2; pYS += nYP2;
				if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				}
			}
		}
		else        // R �ڵ�λ
		{
			// ǰ����
			// ��һ��
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesF += nRealDP2; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesF += nRealDP2; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �ڶ���
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesS += nRealDP2; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesS += nRealDP2; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �м���
			for (int i=2; i<nHalfHalf; i++)
			{
				// ��һ��
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealDP2; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealDP2; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealDP2; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// ������л�һ�У���Ϊ������ʱ��
			if (nHalfHalf & 0x01)       // һ��
			{
				// ��һ��
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
					yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				}
			}
			else    // ����
			{
				// ��һ��
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealDP2; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealDP2; pYS += nYP2;
				if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
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
	// �ͷŻ���
	TPSSE_FREE(pTmpU);
	TPSSE_FREE(pTmpV);
}
void yuv420_to_color32_split(OUT BYTE* pDesFirst,        // ��һ�� *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
							 IN int nFirstPitch,
							 OUT BYTE* pDesSecond,       // �ڶ��� *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
							 IN int nSecondPitch,
							 IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
							 IN int nSrcYPitch,
							 IN BYTE* pSrcU,             // U ���루�洢��ʽ UU0 UU2 UU4 �����������й���һ����
							 IN int nSrcUPitch,
							 IN BYTE* pSrcV,             // V ���루�洢��ʽ VV0 VV2 VV4 �����������й���һ����
							 IN int nSrcVPitch,
							 IN int nWidth,              // Ŀ���С
							 IN int nHeight,
							 IN BOOL bHD,                // �߱����־
							 IN BOOL bDesRevert,         // ͼ���Ƿ��з�ת��־
							 IN BOOL bBGRA)              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	int nHalfHeight = (nHeight >> 1), nHalfHalf = ((nHalfHeight + 1) >> 1);
	int nUVPitch = min(nSrcUPitch, nSrcVPitch);
	int na16 = (nWidth >> 4), nm16 = (nWidth & 0x0f);
	int na8 = ((nWidth - 1) >> 3), nm8 = (nWidth - (na8 << 3));
	int nYP2 = (nSrcYPitch << 1), nUP2 = (nSrcUPitch << 1), nVP2 = (nSrcVPitch << 1);
	// ���û��棬�Ȳ�ֵ����� 422 �� UV �źţ����У�����ʹ�� 422 �� color32 �Ľӿ�
	BYTE* pTmpU = TPSSE_NEW((nUVPitch << 1), BYTE);
	BYTE* pTmpV = TPSSE_NEW((nUVPitch << 1), BYTE);
	BYTE *pU1 = pTmpU, *pU2 = pTmpU + nUVPitch, *pV1 = pTmpV, *pV2 = pTmpV + nUVPitch;
	// ��ת
	BYTE* pDesF = (bDesRevert) ? (pDesFirst + (nHalfHeight - 1) * nFirstPitch) : pDesFirst;
	int nRealFirstPitch = (bDesRevert) ? -nFirstPitch : nFirstPitch;
	BYTE* pDesS = (bDesRevert) ? (pDesSecond + (nHalfHeight - 1) * nSecondPitch) : pDesSecond;
	int nRealSecondPitch = (bDesRevert) ? -nSecondPitch : nSecondPitch;
	BYTE *pYF = pSrcY, *pYS = pSrcY + nSrcYPitch;

	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			// ǰ����
			// ��һ��
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �ڶ���
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �м���
			for (int i=2; i<nHalfHalf; i++)
			{
				// ��һ��
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// ������л�һ�У���Ϊ������ʱ��
			if (nHalfHalf & 0x01)       // һ��
			{
				// ��һ��
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			}
			else    // ����
			{
				// ��һ��
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
			}
		}
		else        // R �ڵ�λ
		{
			// ǰ����
			// ��һ��
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �ڶ���
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �м���
			for (int i=2; i<nHalfHalf; i++)
			{
				// ��һ��
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// ������л�һ�У���Ϊ������ʱ��
			if (nHalfHalf & 0x01)       // һ��
			{
				// ��һ��
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
			else    // ����
			{
				// ��һ��
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
			}
		}
	}
	else        // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			// ǰ����
			// ��һ��
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �ڶ���
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �м���
			for (int i=2; i<nHalfHalf; i++)
			{
				// ��һ��
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// ������л�һ�У���Ϊ������ʱ��
			if (nHalfHalf & 0x01)       // һ��
			{
				// ��һ��
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
					yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				}
			}
			else    // ����
			{
				// ��һ��
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pV1, pU1, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					yuv422_to_color32_inner(pDesS, pYS, pV2, pU2, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				}
			}
		}
		else        // R �ڵ�λ
		{
			// ǰ����
			// ��һ��
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesF += nRealFirstPitch; pYF += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �ڶ���
			uv20_to_uv22_line_2_F(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
			yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
			pDesS += nRealSecondPitch; pYS += nYP2;
			pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			// �м���
			for (int i=2; i<nHalfHalf; i++)
			{
				// ��һ��
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				yuv422_to_color32_inner(pDesS, pYS, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
			}
			// ������л�һ�У���Ϊ������ʱ��
			if (nHalfHalf & 0x01)       // һ��
			{
				// ��һ��
				uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
				{
					memset_dword_line(pDesS, SPECIAL_RGBA_VALUE, (nWidth >> 2), ((nWidth & 0x03) << 2));
				}
				else
				{
					uv20_to_uv22_line_1_L(pTmpU, pTmpV, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
					yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				}
			}
			else    // ����
			{
				// ��һ��
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesF, pYF, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesF += nRealFirstPitch; pYF += nYP2;
				yuv422_to_color32_inner(pDesF, pYF, pU2, pV2, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
				// �ڶ���
				uv20_to_uv22_line_2_L(pTmpU, nUVPitch, pTmpV, nUVPitch, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
				yuv422_to_color32_inner(pDesS, pYS, pU1, pV1, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesS += nRealSecondPitch; pYS += nYP2;
				if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
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
	// �ͷŻ���
	TPSSE_FREE(pTmpU);
	TPSSE_FREE(pTmpV);
}

void yuv422_to_color32_frame(OUT BYTE* pDes32,           // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
							 IN int nDesPitch,
							 IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
							 IN int nSrcYPitch,
							 IN BYTE* pSrcU,             // U ���루�洢��ʽ U0 U2 U4 ������
							 IN int nSrcUPitch,
							 IN BYTE* pSrcV,             // V ���루�洢��ʽ V0 V2 V4 ������
							 IN int nSrcVPitch,
							 IN int nWidth,              // Ŀ���С
							 IN int nHeight,
							 IN BOOL bHD,                // �߱����־
							 IN BOOL bDesRevert,         // ͼ���Ƿ��з�ת��־
							 IN BOOL bBGRA)              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	int na8 = ((nWidth - 1) >> 3), nm8 = (nWidth - (na8 << 3));
	BYTE* pDesLine = (bDesRevert) ? (pDes32 + (nHeight - 1) * nDesPitch) : pDes32;
	int nRealDesPitch = (bDesRevert) ? -nDesPitch : nDesPitch;

	if (bHD)        // ����
	{
		if (bBGRA)      // B �ڵ�λ
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
		else    // R �ڵ�λ
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
	else            // ����
	{
		if (bBGRA)      // B �ڵ�λ
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
		else    // R �ڵ�λ
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
void yuv422_to_color32_split(OUT BYTE* pDesFirst,        // ��һ�� *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
							 IN int nFirstPitch,
							 OUT BYTE* pDesSecond,       // �ڶ��� *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
							 IN int nSecondPitch,
							 IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
							 IN int nSrcYPitch,
							 IN BYTE* pSrcU,             // U ���루�洢��ʽ U0 U2 U4 ������
							 IN int nSrcUPitch,
							 IN BYTE* pSrcV,             // V ���루�洢��ʽ V0 V2 V4 ������
							 IN int nSrcVPitch,
							 IN int nWidth,              // Ŀ���С
							 IN int nHeight,
							 IN BOOL bHD,                // �߱����־
							 IN BOOL bDesRevert,         // ͼ���Ƿ��з�ת��־
							 IN BOOL bBGRA)              // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	int na8 = ((nWidth - 1) >> 3), nm8 = (nWidth - (na8 << 3));
	int nHalfHeight = (nHeight >> 1);
	BYTE* pDesLineF = (bDesRevert) ? (pDesFirst + (nHalfHeight - 1) * nFirstPitch) : pDesFirst;
	BYTE* pDesLineS = (bDesRevert) ? (pDesSecond + (nHalfHeight - 1) * nSecondPitch) : pDesSecond;
	int nRealFirstPitch = (bDesRevert) ? -nFirstPitch : nFirstPitch;
	int nRealSecondPitch = (bDesRevert) ? -nSecondPitch : nSecondPitch;

	if (bHD)        // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				yuv422_to_color32_inner(pDesLineF, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesLineF += nRealFirstPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
				// �ڶ���
				yuv422_to_color32_inner(pDesLineS, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
				pDesLineS += nRealSecondPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				yuv422_to_color32_inner(pDesLineF, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesLineF += nRealFirstPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
				// �ڶ���
				yuv422_to_color32_inner(pDesLineS, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
				pDesLineS += nRealSecondPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
	}
	else            // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				yuv422_to_color32_inner(pDesLineF, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesLineF += nRealFirstPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
				// �ڶ���
				yuv422_to_color32_inner(pDesLineS, pSrcY, pSrcV, pSrcU, na8, nm8, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
				pDesLineS += nRealSecondPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
		else    // R �ڵ�λ
		{
			for (int i=0; i<nHalfHeight; i++)
			{
				// ��һ��
				yuv422_to_color32_inner(pDesLineF, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesLineF += nRealFirstPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
				// �ڶ���
				yuv422_to_color32_inner(pDesLineS, pSrcY, pSrcU, pSrcV, na8, nm8, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
				pDesLineS += nRealSecondPitch;
				pSrcY += nSrcYPitch;
				pSrcU += nSrcUPitch;
				pSrcV += nSrcVPitch;
			}
		}
	}
}

// ��������

// ��Ҫ���������ڳ��ڸ��в�ֵ��ÿ���������
// ǰ����
void yuv420_to_yuyv_line_2_F(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN int yuyv_p,
							 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
							 IN int y_p_2,
							 IN BYTE* u,            // u ���룬��ŷ�ʽ U0 U2 U4 U6 ����
							 IN int u_p_2,
							 IN BYTE* v,            // v ���룬��ŷ�ʽ V0 V2 V4 V6 ����
							 IN int v_p_2,
							 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		punpcklbw   xmm2, xmm3;         // VV14 UU14 ����
		add			 eax, y_p_2;
		add			 ebx, u_p_2;
		add			esi, v_p_2;
		movdqu      xmm5, [eax];
		movsd	     xmm6, [ebx];
		movsd       xmm7, [esi];
		sub			 eax, y_p_2;
		sub			 ebx, u_p_2;
		sub			esi, v_p_2;
		// ���Ϊ UU_0
		punpcklbw   xmm0, xmm2;
		punpckhbw   xmm1, xmm2;
		pxor        xmm2, xmm2;         // 0
		punpcklbw   xmm4, xmm2;         // UU_0
		punpcklbw   xmm3, xmm2;         // VV_0
		punpcklbw   xmm6, xmm2;         // UU_2
		punpcklbw   xmm7, xmm2;         // VV_2
		// ���Ϊ UU_0 * 0.75 + UU_2 * 0.25
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
		punpcklbw    xmm2, xmm3;         // 0000 0000  VV6 UU6 ����
		add			 eax, y_p_2;
		add			 ebx, u_p_2;
		add			 esi, v_p_2;
		movsd       xmm5, [eax];
		movss	    xmm6, [ebx];
		movss       xmm7, [esi];
		sub			 eax, y_p_2;
		sub			 ebx, u_p_2;
		sub			 esi, v_p_2;
		// ���Ϊ UU_0
		punpcklbw   xmm1, xmm2;
		punpcklbw   xmm4, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// ���Ϊ UU_0 * 0.75 + UU_2 * 0.25
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
		// ���Ϊ UU_0
		punpcklbw   xmm1, xmm2;
		punpcklbw   xmm4, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// ���Ϊ UU_0 * 0.75 + UU_2 * 0.25
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
// �м���
void yuv420_to_yuyv_line_2(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN int yuyv_p,
						   IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
						   IN int y_p_2,
						   IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
						   IN int u_p_2,
						   IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
						   IN int v_p_2,
						   IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		// ��һ�������UU_P * 0.25 + UU_C * 0.75���ڶ��������UU_N * 0.25 + UU_C * 0.75
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
		// ��һ�������UU_P * 0.25 + UU_C * 0.75���ڶ��������UU_N * 0.25 + UU_C * 0.75
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
		// ��һ�������UU_P * 0.25 + UU_C * 0.75���ڶ��������UU_N * 0.25 + UU_C * 0.75
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
// �������
void yuv420_to_yuyv_line_2_L(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN int yuyv_p,
							 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
							 IN int y_p_2,
							 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
							 IN int u_p_2,
							 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
							 IN int v_p_2,
							 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		punpcklbw   xmm0, xmm7;         // VV14 UU14 ����
		// ���Ϊ UU_2
		punpcklbw   xmm4, xmm0;
		punpckhbw   xmm5, xmm0;
		pxor        xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// ���Ϊ UU_0 * 0.25 + UU_2 * 0.75
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
		punpcklbw   xmm4, xmm7;         // 0000 0000  VV6 UU6 ����
		// ���Ϊ UU_2
		punpcklbw   xmm5, xmm4;
		punpcklbw   xmm2, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// ���Ϊ UU_0 * 0.25 + UU_2 * 0.75
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
		// ���Ϊ UU_2
		punpcklbw   xmm5, xmm4;
		punpcklbw   xmm2, xmm0;         // UU_0
		punpcklbw   xmm3, xmm0;         // VV_0
		punpcklbw   xmm6, xmm0;         // UU_2
		punpcklbw   xmm7, xmm0;         // VV_2
		// ���Ϊ UU_0 * 0.25 + UU_2 * 0.75
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
// ���һ��
void yuv420_to_yuyv_line_1_L(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
							 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
							 IN int u_p_2,
							 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
							 IN int v_p_2,
							 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		// ���Ϊ UU_0 * 0.25 + UU_2 * 0.75
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
		// ���Ϊ UU_0 * 0.25 + UU_2 * 0.75
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
		// ���Ϊ UU_0 * 0.25 + UU_2 * 0.75
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

// XDCAM ��ֵ��ʽ��420->422 һ�в�ֵ�����У�ʹ�������У�����һ����ʽΪ
// F1_Out0 = 0.125 * F1_Prev + 0.875 * F1_Curr    F1_Out1 = 0.625 * F1_Curr + 0.375 * F1_Next
// ��һ��
void yuv420_to_yuyv_xdcam_F_line_2_F(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
									 IN int yuyv_p,
									 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
									 IN int y_p_2,
									 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
									 IN int u_p_2,
									 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
									 IN int v_p_2,
									 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		punpcklbw   xmm2, xmm3;         // VV14 UU14 ����
		// ���Ϊ UU_0
		punpcklbw   xmm0, xmm2;
		punpckhbw   xmm1, xmm2;
		xorps		xmm2, xmm2;         // 0
		punpcklbw   xmm4, xmm2;         // UU_0
		punpcklbw   xmm3, xmm2;         // VV_0
		punpcklbw   xmm6, xmm2;         // UU_2
		punpcklbw   xmm7, xmm2;         // VV_2
		// ���Ϊ UU_0 * 0.625 + UU_2 * 0.375
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
		punpcklbw   xmm2, xmm3;         // 0000  0000  VV6 UU6 ����
		punpcklbw   xmm6, xmm7;         // 0000  0000  VV6 UU6 ����
		// ���Ϊ UU_0
		punpcklbw   xmm1, xmm2;
		xorps		xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // VV_0  UU_0  UV ���
		punpcklbw   xmm6, xmm0;         // VV_2  UU_2  UV ���
		// ���Ϊ UU_0 * 0.625 + UU_2 * 0.375
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm2, xmm6;		// N + C * 3
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw       xmm2, gc_nRoundComp_3;
		paddw       xmm2, xmm6;		// N * 3 + C * 5
		psrlw       xmm2, 0x03;
		packuswb	xmm2, xmm2;		// UV ���
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
		// ���Ϊ UU_0
		punpcklbw   xmm1, xmm2;
		xorps		xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // VV_0  UU_0  UV ���
		punpcklbw   xmm6, xmm0;         // VV_2  UU_2  UV ���
		// ���Ϊ UU_0 * 0.625 + UU_2 * 0.375
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm2, xmm6;		// N + C * 3
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw       xmm2, gc_nRoundComp_3;
		paddw       xmm2, xmm6;		// N * 3 + C * 5
		psrlw       xmm2, 0x03;
		packuswb	xmm2, xmm2;		// UV ���
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
		// ���Ϊ UU_0
		punpcklbw	xmm1, xmm2;
		punpcklbw	xmm2, xmm0;		// C
		// ���Ϊ UU_0 * 0.625 + UU_2 * 0.375
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm2, xmm6;		// N + C * 3
		paddw		xmm6, xmm6;		// N * 2 + C * 2
		paddw       xmm2, gc_nRoundComp_3;
		paddw       xmm2, xmm6;		// N * 3 + C * 5
		psrlw       xmm2, 0x03;
		packuswb	xmm2, xmm2;		// UV ���
		punpcklbw   xmm5, xmm2;
		pop			edi;
		pop			edx;
		movss		dword ptr [edi], xmm1;
		movss		dword ptr [edx], xmm5;
loop_end:
	}
}
// �м���
void yuv420_to_yuyv_xdcam_F_line_2(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
								   IN int yuyv_p,
								   IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
								   IN int y_p_2,
								   IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
								   IN int u_p_2,
								   IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
								   IN int v_p_2,
								   IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		// ��һ�������UU_P * 0.125 + UU_C * 0.875���ڶ��������UU_N * 0.375 + UU_C * 0.625
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
		punpcklbw	xmm4, xmm5;		// UV_C ���
		punpcklbw	xmm6, xmm7;		// UV_N ���
		punpcklbw	xmm2, xmm3;		// UV_P ���
		xorps		xmm0, xmm0;
		punpcklbw   xmm4, xmm0;     // VV_C  UU_C ���
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P ���
		punpcklbw   xmm6, xmm0;     // VV_N  UU_N ���
		// ��һ�������UU_P * 0.125 + UU_C * 0.875���ڶ��������UU_N * 0.375 + UU_C * 0.625
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
		// ��һ�������UU_P * 0.125 + UU_C * 0.875���ڶ��������UU_N * 0.375 + UU_C * 0.625
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
		// ��һ�������UU_P * 0.125 + UU_C * 0.875���ڶ��������UU_N * 0.375 + UU_C * 0.625
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
// ���һ��
void yuv420_to_yuyv_xdcam_F_line_1_L(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
									 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
									 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
									 IN int u_p_2,
									 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
									 IN int v_p_2,
									 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		// ���Ϊ UU_P * 0.125 + UU_C * 0.875
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
		punpcklbw	xmm2, xmm3;		// VV_P  UU_P  ���
		punpcklbw	xmm6, xmm7;		// VV_C  UU_C  ���
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P  ���
		punpcklbw   xmm6, xmm0;     // VV_C  UU_C  ���
		// ���Ϊ UU_P * 0.125 + UU_C * 0.875
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
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P  ���
		punpcklbw   xmm6, xmm0;     // VV_C  UU_C  ���
		// ���Ϊ UU_P * 0.125 + UU_C * 0.875
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
		// ���Ϊ UU_P * 0.125 + UU_C * 0.875
		paddw       xmm6, xmm2;		// P + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm6, xmm2;		// P + C * 3
		paddw		xmm2, xmm2;		// C * 4
		paddw       xmm6, xmm4;
		paddw       xmm6, xmm2;		// N + C * 7
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV ���
		punpcklbw   xmm1, xmm6;
		movss		dword ptr [edi], xmm1;
loop_end:
	}
}
// XDCAM ��ֵ��ʽ��420->422 һ�в�ֵ�����У�ʹ�������У����ڶ�����ʽΪ
// F2_Out0 = 0.375 * F1_Prev + 0.625 * F1_Curr    F1_Out1 = 0.875 * F1_Curr + 0.125 * F1_Next
// ��һ��
void yuv420_to_yuyv_xdcam_S_line_2_F(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
									 IN int yuyv_p,
									 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
									 IN int y_p_2,
									 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
									 IN int u_p_2,
									 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
									 IN int v_p_2,
									 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		punpcklbw   xmm2, xmm3;         // VV14 UU14 ����
		// ���Ϊ UU_0
		punpcklbw   xmm0, xmm2;
		punpckhbw   xmm1, xmm2;
		xorps		xmm2, xmm2;         // 0
		punpcklbw   xmm4, xmm2;         // UU_0
		punpcklbw   xmm3, xmm2;         // VV_0
		punpcklbw   xmm6, xmm2;         // UU_2
		punpcklbw   xmm7, xmm2;         // VV_2
		// ���Ϊ UU_0 * 0.875 + UU_2 * 0.125
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
		punpcklbw   xmm2, xmm3;         // 0000  0000  VV6 UU6 ����
		punpcklbw   xmm6, xmm7;         // 0000  0000  VV6 UU6 ����
		// ���Ϊ UU_0
		punpcklbw   xmm1, xmm2;
		xorps		xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // VV_0  UU_0  UV ���
		punpcklbw   xmm6, xmm0;         // VV_2  UU_2  UV ���
		// ���Ϊ UU_0 * 0.875 + UU_2 * 0.125
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm6, xmm2;		// N + C * 3
		paddw		xmm2, xmm2;		// C * 4
		paddw       xmm6, gc_nRoundComp_3;
		paddw       xmm6, xmm2;		// N + C * 7
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV ���
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
		// ���Ϊ UU_0
		punpcklbw   xmm1, xmm2;
		xorps		xmm0, xmm0;         // 0
		punpcklbw   xmm2, xmm0;         // VV_0  UU_0  UV ���
		punpcklbw   xmm6, xmm0;         // VV_2  UU_2  UV ���
		// ���Ϊ UU_0 * 0.875 + UU_2 * 0.125
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm6, xmm2;		// N + C * 3
		paddw		xmm2, xmm2;		// C * 4
		paddw       xmm6, gc_nRoundComp_3;
		paddw       xmm6, xmm2;		// N + C * 7
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV ���
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
		// ���Ϊ UU_0
		punpcklbw	xmm1, xmm2;
		punpcklbw	xmm2, xmm0;		// C
		// ���Ϊ UU_0 * 0.875 + UU_2 * 0.125
		paddw       xmm6, xmm2;		// N + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm6, xmm2;		// N + C * 3
		paddw		xmm2, xmm2;		// C * 4
		paddw       xmm6, gc_nRoundComp_3;
		paddw       xmm6, xmm2;		// N + C * 7
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV ���
		punpcklbw   xmm5, xmm6;
		pop			edi;
		pop			edx;
		movss		dword ptr [edi], xmm1;
		movss		dword ptr [edx], xmm5;
loop_end:
	}
}
// �м���
void yuv420_to_yuyv_xdcam_S_line_2(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
								   IN int yuyv_p,
								   IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
								   IN int y_p_2,
								   IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
								   IN int u_p_2,
								   IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
								   IN int v_p_2,
								   IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		// ��һ�������UU_P * 0.375 + UU_C * 0.625���ڶ��������UU_N * 0.125 + UU_C * 0.875
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
		punpcklbw	xmm4, xmm5;		// UV_C ���
		punpcklbw	xmm6, xmm7;		// UV_N ���
		punpcklbw	xmm2, xmm3;		// UV_P ���
		xorps		xmm0, xmm0;
		punpcklbw   xmm4, xmm0;     // VV_C  UU_C ���
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P ���
		punpcklbw   xmm6, xmm0;     // VV_N  UU_N ���
		// ��һ�������UU_P * 0.125 + UU_C * 0.875���ڶ��������UU_N * 0.375 + UU_C * 0.625
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
		// ��һ�������UU_P * 0.125 + UU_C * 0.875���ڶ��������UU_N * 0.375 + UU_C * 0.625
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
		// ��һ�������UU_P * 0.125 + UU_C * 0.875���ڶ��������UU_N * 0.375 + UU_C * 0.625
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
// ���һ��
void yuv420_to_yuyv_xdcam_S_line_1_L(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
									 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
									 IN BYTE* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
									 IN int u_p_2,
									 IN BYTE* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
									 IN int v_p_2,
									 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		// ���Ϊ UU_P * 0.375 + UU_C * 0.625
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
		punpcklbw	xmm2, xmm3;		// VV_P  UU_P  ���
		punpcklbw	xmm6, xmm7;		// VV_C  UU_C  ���
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P  ���
		punpcklbw   xmm6, xmm0;     // VV_C  UU_C  ���
		// ���Ϊ UU_P * 0.375 + UU_C * 0.625
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
		punpcklbw   xmm2, xmm0;     // VV_P  UU_P  ���
		punpcklbw   xmm6, xmm0;     // VV_C  UU_C  ���
		// ���Ϊ UU_P * 0.125 + UU_C * 0.875
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
		// ���Ϊ UU_P * 0.125 + UU_C * 0.875
		paddw       xmm6, xmm2;		// P + C
		paddw       xmm2, xmm2;		// C * 2
		paddw		xmm2, xmm6;		// P + C * 3
		paddw		xmm6, xmm6;		// P * 2 + C * 2
		paddw       xmm2, xmm4;
		paddw       xmm6, xmm2;		// P * 3 + C * 5
		psrlw       xmm6, 0x03;
		packuswb	xmm6, xmm6;		// UV ���
		punpcklbw   xmm1, xmm6;
		movss		dword ptr [edi], xmm1;
loop_end:
	}
}
// XDCAM ��ֵ��ʽ��422->420 ���в�ֵ��һ�У���һ����ʽΪ F1_Out = 0.75 * F1_0 + 0.25 * F1_1
void yuyv_to_yuv420_xdcam_F_line_2(OUT BYTE* y,         // ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
								   IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
								   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
								   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
								   IN BYTE* yuyv,		// ��һ�� yuyv ���룬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
								   IN int yuyv_pitch,	// �ڶ��� yuyv �������һ�е�ƫ��
								   IN int na8,          // һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		movups		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movups		xmm2, dword ptr [esi + edx];	// �ڶ��� V0 Y1 U0 Y0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// V6 U6  V4 U4  V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm2, xmm1;
		paddw		xmm1, xmm1;
		paddw		xmm2, xmm0;				// ����
		paddw		xmm2, xmm1;
		psrlw		xmm2, 0x02;				// �Ŵ��� 4 ��
		packuswb	xmm3, xmm4;				// �ڶ��� Y    ��һ�� Y
		packuswb	xmm2, xmm2;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm1, xmm2;
		pand		xmm1, xmm7;				// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm2, 0x08;				// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm1, xmm2;				// V6V4V2V0  V6V4V2V0  U6U4U2U0  U6U4U2U0
		movhlps		xmm2, xmm1;
		mov			edx, y_pitch;
		movlps		dword ptr [edi], xmm3;	// ��һ�� Y
		movhps		dword ptr [edi + edx], xmm3;	// �ڶ��� Y
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
		movlps		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movlps		xmm2, dword ptr [esi + edx];	// �ڶ��� V0 Y1 U0 Y0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm2, xmm1;
		paddw		xmm1, xmm1;
		paddw		xmm2, xmm0;				// ����
		paddw		xmm2, xmm1;
		psrlw		xmm2, 0x02;				// �Ŵ��� 4 ��
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
		movd		dword ptr [edi], xmm3;			// ��һ�� Y
		movd		dword ptr [edi + edx], xmm4;	// �ڶ��� Y
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
		movd		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movd		xmm2, dword ptr [esi + edx];	// �ڶ��� V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   @  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   @  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm2, xmm1;
		paddw		xmm1, xmm1;
		paddw		xmm2, xmm0;				// ����
		paddw		xmm2, xmm1;
		psrlw		xmm2, 0x01;				// ������ 4 ��
		packuswb	xmm3, xmm4;				// @  @ Y1Y0  @  @ Y1Y0
		packuswb	xmm2, xmm2;				// @  @ V0U0  @  @ V0U0
		pextrw		ecx, xmm3, 0x00;		// ��һ�� Y
		pextrw		esi, xmm3, 0x04;		// �ڶ��� Y
		mov			edx, y_pitch;
		mov			word ptr [edi], cx;			// ��һ�� Y
		mov			word ptr [edi + edx], si;	// �ڶ��� Y
		pextrw		ecx, xmm2, 0x00;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], ch;		// V
loop_end:
	}
}
// XDCAM ��ֵ��ʽ��422->420 ���в�ֵ��һ�У��ڶ�����ʽΪ F2_Out = 0.25 * F2_0 + 3 * F2_1
void yuyv_to_yuv420_xdcam_S_line_2(OUT BYTE* y,         // ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
								   IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
								   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
								   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
								   IN BYTE* yuyv,		// ��һ�� yuyv ���룬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
								   IN int yuyv_pitch,	// �ڶ��� yuyv �������һ�е�ƫ��
								   IN int na8,          // һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		movups		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movups		xmm2, dword ptr [esi + edx];	// �ڶ��� V0 Y1 U0 Y0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// V6 U6  V4 U4  V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm2;
		paddw		xmm2, xmm2;
		paddw		xmm1, xmm0;				// ����
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x02;				// �Ŵ��� 4 ��
		packuswb	xmm3, xmm4;				// �ڶ��� Y    ��һ�� Y
		packuswb	xmm1, xmm1;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm2, xmm1;
		pand		xmm1, xmm7;				// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm2, 0x08;				// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm1, xmm2;				// V6V4V2V0  V6V4V2V0  U6U4U2U0  U6U4U2U0
		movhlps		xmm2, xmm1;
		mov			edx, y_pitch;
		movlps		dword ptr [edi], xmm3;	// ��һ�� Y
		movhps		dword ptr [edi + edx], xmm3;	// �ڶ��� Y
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
		movlps		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movlps		xmm2, dword ptr [esi + edx];	// �ڶ��� V0 Y1 U0 Y0
		movaps		xmm3, xmm1;
		movaps		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm2;
		paddw		xmm2, xmm2;
		paddw		xmm1, xmm0;				// ����
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x02;				// �Ŵ��� 4 ��
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
		movd		dword ptr [edi], xmm3;			// ��һ�� Y
		movd		dword ptr [edi + edx], xmm4;	// �ڶ��� Y
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
		movd		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movd		xmm2, dword ptr [esi + edx];	// �ڶ��� V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   @  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   @  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm2;
		paddw		xmm2, xmm2;
		paddw		xmm1, xmm0;				// ����
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x01;				// ������ 4 ��
		packuswb	xmm3, xmm4;				// @  @ Y1Y0  @  @ Y1Y0
		packuswb	xmm1, xmm1;				// @  @ V0U0  @  @ V0U0
		pextrw		ecx, xmm3, 0x00;		// ��һ�� Y
		pextrw		esi, xmm3, 0x04;		// �ڶ��� Y
		mov			edx, y_pitch;
		mov			word ptr [edi], cx;			// ��һ�� Y
		mov			word ptr [edi + edx], si;	// �ڶ��� Y
		pextrw		ecx, xmm1, 0x00;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], ch;		// V
loop_end:
	}
}

void yuv422_to_yuyv_line(OUT BYTE* yuyv,        // yuyv �������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						 IN BYTE* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
						 IN BYTE* u,            // u ���룬��ŷ�ʽ U0 U2 U4 U6 ����
						 IN BYTE* v,            // v ���룬��ŷ�ʽ V0 V2 V4 V6 ����
						 IN int na16,           // һ�����ؿ�ȳ��� 16 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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

void yuyv_to_yuv420_line_2_avr(OUT BYTE* y,         // ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
							   IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
							   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
							   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
							   IN BYTE* yuyv,		// ��һ�� yuyv ���룬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							   IN int yuyv_pitch,	// �ڶ��� yuyv �������һ�е�ƫ��
							   IN int na8,          // һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		movdqu		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movdqu		xmm2, dword ptr [esi + edx];	// �ڶ��� V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// V6 U6  V4 U4  V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm0;				// ����
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x01;				// ƽ��ֵ
		packuswb	xmm1, xmm1;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		packuswb	xmm3, xmm4;				// �ڶ��� Y    ��һ�� Y
		movdqa		xmm2, xmm1;
		pand		xmm1, xmm7;				// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm2, 0x08;				// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm1, xmm2;				// V6V4V2V0  V6V4V2V0  U6U4U2U0  U6U4U2U0
		movhlps		xmm2, xmm1;
		mov			edx, y_pitch;
		movlps		dword ptr [edi], xmm3;	// ��һ�� Y
		movhps		dword ptr [edi + edx], xmm3;	// �ڶ��� Y
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
		movlps		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movlps		xmm2, dword ptr [esi + edx];	// �ڶ��� V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   V2 U2  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   Y3 Y2  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm0;				// ����
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x01;				// ƽ��ֵ
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
		movd		dword ptr [edi], xmm3;			// ��һ�� Y
		movd		dword ptr [edi + edx], xmm4;	// �ڶ��� Y
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
		movd		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movd		xmm2, dword ptr [esi + edx];	// �ڶ��� V0 Y1 U0 Y0
		movdqa		xmm3, xmm1;
		movdqa		xmm4, xmm2;
		psrlw		xmm1, 0x08;				// @  @   @  V0 U0
		psrlw		xmm2, 0x08;
		pand		xmm3, xmm7;				// @  @   @  Y1 Y0
		pand		xmm4, xmm7;
		paddw		xmm1, xmm0;				// ����
		paddw		xmm1, xmm2;
		psrlw		xmm1, 0x01;				// ƽ��ֵ
		packuswb	xmm3, xmm4;				// @  @ Y1Y0  @  @ Y1Y0
		packuswb	xmm1, xmm1;				// @  @ V0U0  @  @ V0U0
		pextrw		ecx, xmm3, 0x00;		// ��һ�� Y
		pextrw		esi, xmm3, 0x04;		// �ڶ��� Y
		mov			edx, y_pitch;
		mov			word ptr [edi], cx;			// ��һ�� Y
		mov			word ptr [edi + edx], si;	// �ڶ��� Y
		pextrw		ecx, xmm1, 0x00;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], ch;		// V
loop_end:
	}
}
void yuyv_to_yuv420_line_1(OUT BYTE* y,         // һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
						   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
						   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
						   IN BYTE* yuyv,		// һ�� yuyv ���룬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN int na8,	        // һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
// ���ó������� -1, 3, 3, -1 ��ϵ���� UV ���в���
// ��һ��
void yuyv_to_yuv420_line_4_F(OUT BYTE* y,			// ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
							 IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
							 OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
							 OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
							 IN BYTE* yuyv1,		// ��һ�� yuyv ���루Ȩ�� -1+3������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN BYTE* yuyv2,		// �ڶ��� yuyv ���루Ȩ�� 3������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN int yuyv_pitch,		// ���� yuyv ������ƫ��
							 IN int na8,			// һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		movups		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movups		xmm2, dword ptr [edx];			// �ڶ��� V0 Y1 U0 Y0
		movups		xmm3, dword ptr [edx + ecx];	// ��һ�� V0 Y1 U0 Y0
		movaps		xmm5, xmm1;
		movaps		xmm4, xmm2;
		movaps		xmm6, xmm2;
		pand		xmm1, xmm0;
		pand		xmm2, xmm0;		// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm5, xmm7;
		pand		xmm4, xmm7;		// V6 U6  V4 U4  V2 U2  V0 U0���Ŵ� 8 λ��
		psrlw		xmm3, 0x08;		// V6 U6  V4 U4  V2 U2  V0 U0��-1 Ȩ�أ�
		psrlw		xmm6, 0x08;		// 3 Ȩ��
		psrlw		xmm5, 0x07;		// �Ŵ� 1 λ
		psrlw		xmm4, 0x07;
		psubw		xmm6, xmm3;
		paddw		xmm4, xmm5;
		paddw		xmm6, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm6, xmm4;
		psrlw		xmm6, 0x02;				// VV6 UU6 VV4 UU4 VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm6, xmm4;
		packuswb	xmm1, xmm2;				// �ڶ��� Y    ��һ�� Y
		packuswb	xmm6, xmm6;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm5, xmm6;
		pand		xmm6, xmm0;		// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm5, 0x08;		// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm5, xmm6;		// U6U4U2U0  U6U4U2U0  V6V4V2V0  V6V4V2V0
		movhlps		xmm4, xmm5;
		mov			ecx, y_pitch;
		movlps		dword ptr [edi], xmm1;			// ��һ�� Y
		movhps		dword ptr [edi + ecx], xmm1;	// �ڶ��� Y
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
		movlps		xmm1, dword ptr [esi];			// @  ��һ��
		movhps		xmm1, dword ptr [edx];			// �ڶ���  ��һ��
		movlps		xmm4, dword ptr [edx + ecx];	// @  ��һ��
		movlhps		xmm4, xmm1;						// ��һ��  ��һ��
		movaps		xmm2, xmm1;
		movaps		xmm3, xmm1;
		pand		xmm1, xmm0;		// �ڶ��� Y  ��һ�� Y
		pand		xmm2, xmm7;		// �ڶ��� UV  ��һ�� UV���Ŵ� 8 λ��
		psrlw		xmm4, 0x08;		// ��һ�� UV  ��һ�� UV��-1 Ȩ�أ�
		psrlw		xmm3, 0x08;		// �ڶ��� UV  ��һ�� UV��3 Ȩ�أ�
		psrlw		xmm2, 0x07;		// �Ŵ� 1 λ
		psubw		xmm4, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm2, xmm3;
		psubw		xmm2, xmm4;
		movhlps		xmm3, xmm2;
		paddw		xmm2, xmm3;
		psrlw		xmm2, 0x02;				// @  VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm2, xmm4;
		packuswb	xmm1, xmm2;				// @  V2U2V0U0  �ڶ��� Y  ��һ�� Y
		movhlps		xmm2, xmm1;
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		shufps		xmm1, xmm1, 0x01;		// @  @  @  �ڶ��� Y
		pand		xmm2, xmm0;				// @  U2 U0  @  U2 U0
		psrlw		xmm3, 0x08;				// @  V2 V0  @  V2 V0
		packuswb	xmm3, xmm2;				// @ U2U0  @ U2V0  @ V2V0  @ V2V0
		push		esi;
		push		edx;
		mov			ecx, y_pitch;
		pextrw		esi, xmm3, 0x04;
		pextrw		edx, xmm3, 0x00;
		movd		dword ptr [edi], xmm0;			// ��һ�� Y
		movd		dword ptr [edi + ecx], xmm1;	// �ڶ��� Y
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
		movd		xmm1, dword ptr [esi];	// ��һ�� V0 Y1 U0 Y0
		movd		xmm2, dword ptr [edx];	// �ڶ��� V0 Y1 U0 Y0
		movd		xmm3, dword ptr [edx + ecx];	// ��һ�� V0 Y1 U0 Y0
		movlhps		xmm1, xmm2;			// �ڶ���  ��һ��
		movlhps		xmm3, xmm1;			// ��һ��  ��һ��
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm1;
		pand		xmm1, xmm0;		// �ڶ��� Y  ��һ�� Y
		pand		xmm4, xmm7;		// �ڶ��� UV  ��һ�� UV���Ŵ� 8 λ��
		psrlw		xmm3, 0x08;				// ��һ�� UV  ��һ�� UV��-1 Ȩ�أ�
		psrlw		xmm5, 0x08;				// �ڶ��� UV  ��һ�� UV��3 Ȩ�أ�
		psrlw		xmm4, 0x07;				// �Ŵ� 1 λ
		psubw		xmm3, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm4, xmm5;
		psubw		xmm4, xmm3;
		movhlps		xmm5, xmm4;
		paddw		xmm4, xmm5;
		packuswb	xmm1, xmm1;				// 0 �ڶ���Y  0 ��һ��Y  0 �ڶ���Y  0 ��һ��Y
		psrlw		xmm4, 0x02;				// @  @  @  VV0 UU0
		pxor		xmm3, xmm3;
		pmaxsw		xmm4, xmm3;
		pextrw		esi, xmm1, 0x00;
		pextrw		edx, xmm1, 0x02;
		mov			ecx, y_pitch;
		mov			word ptr [edi], si;			// ��һ�� Y
		mov			word ptr [edi + ecx], dx;	// �ڶ��� Y
		pextrw		ecx, xmm4, 0x00;
		pextrw		edx, xmm4, 0x01;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], dl;		// V
loop_end:
	}
}
// �м���
void yuyv_to_yuv420_line_4(OUT BYTE* y,			// ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
						   IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
						   OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
						   OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
						   IN BYTE* yuyv0,		// ��һ�� yuyv ���루Ȩ�� -1������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN BYTE* yuyv2,		// �ڶ��� yuyv ���루Ȩ�� 3������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN int yuyv_pitch,	// ���� yuyv ������ƫ��
						   IN int na8,			// һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		movups		xmm0, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movups		xmm2, dword ptr [edx];			// �ڶ��� V0 Y1 U0 Y0
		movups		xmm1, dword ptr [esi + ecx];	// ��һ�� V0 Y1 U0 Y0
		movups		xmm3, dword ptr [edx + ecx];	// ��һ�� V0 Y1 U0 Y0
		movaps		xmm4, xmm2;
		movaps		xmm6, xmm2;
		movaps		xmm5, xmm1;
		movaps		xmm7, xmm1;
		pand		xmm2, gc_dwMaskWLB;		// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm1, gc_dwMaskWLB;
		pand		xmm4, gc_dwMaskWHB;		// V6 U6  V4 U4  V2 U2  V0 U0���Ŵ� 8 λ��
		pand		xmm5, gc_dwMaskWHB;
		psrlw		xmm0, 0x08;				// V6 U6  V4 U4  V2 U2  V0 U0��-1 Ȩ�أ�
		psrlw		xmm3, 0x08;
		psrlw		xmm6, 0x08;				// 3 Ȩ��
		psrlw		xmm7, 0x08;
		psrlw		xmm4, 0x07;				// �Ŵ� 1 λ
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
		packuswb	xmm1, xmm2;				// �ڶ��� Y    ��һ�� Y
		packuswb	xmm4, xmm4;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm5, xmm4;
		pand		xmm4, gc_dwMaskWLB;		// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm5, 0x08;				// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm5, xmm4;				// U6U4U2U0  U6U4U2U0  V6V4V2V0  V6V4V2V0
		movhlps		xmm4, xmm5;
		mov			ecx, y_pitch;
		movlps		dword ptr [edi], xmm1;			// ��һ�� Y
		movhps		dword ptr [edi + ecx], xmm1;	// �ڶ��� Y
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
		movhps		xmm0, dword ptr [esi];			// ��һ��  @
		movhps		xmm1, dword ptr [edx];			// �ڶ���  @
		movlps		xmm1, dword ptr [esi + ecx];	// �ڶ���  ��һ��
		movlps		xmm0, dword ptr [edx + ecx];	// ��һ��  ��һ��
		movaps		xmm2, xmm1;
		movaps		xmm3, xmm1;
		pand		xmm1, gc_dwMaskWLB;		// �ڶ��� Y  ��һ�� Y
		pand		xmm2, gc_dwMaskWHB;		// �ڶ��� UV  ��һ�� UV���Ŵ� 8 λ��
		psrlw		xmm0, 0x08;				// ��һ�� UV  ��һ�� UV��-1 Ȩ�أ�
		psrlw		xmm3, 0x08;				// �ڶ��� UV  ��һ�� UV��3 Ȩ�أ�
		psrlw		xmm2, 0x07;				// �Ŵ� 1 λ
		psubw		xmm0, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm2, xmm3;
		psubw		xmm2, xmm0;
		movhlps		xmm3, xmm2;
		paddw		xmm2, xmm3;
		psrlw		xmm2, 0x02;				// @  VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm2, xmm4;
		packuswb	xmm1, xmm2;				// @  V2U2V0U0  �ڶ��� Y  ��һ�� Y
		movhlps		xmm2, xmm1;
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		shufps		xmm1, xmm1, 0x01;		// @  @  @  �ڶ��� Y
		pand		xmm2, gc_dwMaskWLB;		// @  U2 U0  @  U2 U0
		psrlw		xmm3, 0x08;				// @  V2 V0  @  V2 V0
		packuswb	xmm3, xmm2;				// @ U2U0  @ U2V0  @ V2V0  @ V2V0
		push		esi;
		push		edx;
		mov			ecx, y_pitch;
		pextrw		esi, xmm3, 0x04;
		pextrw		edx, xmm3, 0x00;
		movd		dword ptr [edi], xmm0;			// ��һ�� Y
		movd		dword ptr [edi + ecx], xmm1;	// �ڶ��� Y
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
		movd		xmm0, dword ptr [esi];	// ��һ�� V0 Y1 U0 Y0
		movd		xmm2, dword ptr [edx];	// �ڶ��� V0 Y1 U0 Y0
		movd		xmm1, dword ptr [esi + ecx];	// ��һ�� V0 Y1 U0 Y0
		movd		xmm3, dword ptr [edx + ecx];	// ��һ�� V0 Y1 U0 Y0
		movlhps		xmm1, xmm2;			// �ڶ���  ��һ��
		movlhps		xmm0, xmm3;			// ��һ��  ��һ��
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm1;
		pand		xmm1, gc_dwMaskWLB;		// �ڶ��� Y  ��һ�� Y
		pand		xmm4, gc_dwMaskWHB;		// �ڶ��� UV  ��һ�� UV���Ŵ� 8 λ��
		psrlw		xmm0, 0x08;				// ��һ�� UV  ��һ�� UV��-1 Ȩ�أ�
		psrlw		xmm5, 0x08;				// �ڶ��� UV  ��һ�� UV��3 Ȩ�أ�
		psrlw		xmm4, 0x07;				// �Ŵ� 1 λ
		psubw		xmm0, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm4, xmm5;
		psubw		xmm4, xmm0;
		movhlps		xmm5, xmm4;
		paddw		xmm4, xmm5;
		packuswb	xmm1, xmm1;				// 0 �ڶ���Y  0 ��һ��Y  0 �ڶ���Y  0 ��һ��Y
		psrlw		xmm4, 0x02;				// @  @  @  VV0 UU0
		pxor		xmm3, xmm3;
		pmaxsw		xmm4, xmm3;
		pextrw		esi, xmm1, 0x00;
		pextrw		edx, xmm1, 0x02;
		mov			ecx, y_pitch;
		mov			word ptr [edi], si;			// ��һ�� Y
		mov			word ptr [edi + ecx], dx;	// �ڶ��� Y
		pextrw		ecx, xmm4, 0x00;
		pextrw		edx, xmm4, 0x01;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], dl;		// V
loop_end:
	}
}
// ���һ��
void yuyv_to_yuv420_line_4_L(OUT BYTE* y,			// ��һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
							 IN int y_pitch,		// �ڶ��� y ������һ�е�ƫ��
							 OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
							 OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
							 IN BYTE* yuyv0,		// ��һ�� yuyv ���루Ȩ�� -1������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN BYTE* yuyv2,		// �ڶ��� yuyv ���루Ȩ�� 3-1������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							 IN int yuyv_pitch,		// ���� yuyv ������ƫ��
							 IN int na8,			// һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		movups		xmm3, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movups		xmm2, dword ptr [edx];			// �ڶ��� V0 Y1 U0 Y0
		movups		xmm1, dword ptr [esi + ecx];	// ��һ�� V0 Y1 U0 Y0
		movaps		xmm5, xmm2;
		movaps		xmm4, xmm1;
		movaps		xmm6, xmm1;
		pand		xmm2, xmm0;		// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm1, xmm0;
		pand		xmm5, xmm7;
		pand		xmm4, xmm7;		// V6 U6  V4 U4  V2 U2  V0 U0���Ŵ� 8 λ��
		psrlw		xmm3, 0x08;		// V6 U6  V4 U4  V2 U2  V0 U0��-1 Ȩ�أ�
		psrlw		xmm6, 0x08;		// 3 Ȩ��
		psrlw		xmm5, 0x07;		// �Ŵ� 1 λ
		psrlw		xmm4, 0x07;
		psubw		xmm6, xmm3;
		paddw		xmm4, xmm5;
		paddw		xmm6, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm6, xmm4;
		psrlw		xmm6, 0x02;				// VV6 UU6 VV4 UU4 VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm6, xmm4;
		packuswb	xmm1, xmm2;				// �ڶ��� Y    ��һ�� Y
		packuswb	xmm6, xmm6;				// V6U6V4U4  V2U2V0U0  V6U6V4U4  V2U2V0U0
		movaps		xmm5, xmm6;
		pand		xmm6, xmm0;		// 0 U6 0 U4 0 U2 0 U0 0 U6 0 U4 0 U2 0 U0
		psrlw		xmm5, 0x08;		// 0 V6 0 V4 0 V2 0 V0 0 V6 0 V4 0 V2 0 V0
		packuswb	xmm5, xmm6;		// U6U4U2U0  U6U4U2U0  V6V4V2V0  V6V4V2V0
		movhlps		xmm4, xmm5;
		mov			ecx, y_pitch;
		movlps		dword ptr [edi], xmm1;			// ��һ�� Y
		movhps		dword ptr [edi + ecx], xmm1;	// �ڶ��� Y
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
		movhps		xmm4, dword ptr [esi];			// ��һ��  @
		movhps		xmm1, dword ptr [edx];			// �ڶ���  @
		movlps		xmm1, dword ptr [esi + ecx];	// �ڶ���  ��һ��
		movhlps		xmm4, xmm1;						// ��һ��  �ڶ���
		movaps		xmm2, xmm1;
		movaps		xmm3, xmm1;
		pand		xmm1, xmm0;		// �ڶ��� Y  ��һ�� Y
		pand		xmm2, xmm7;		// �ڶ��� UV  ��һ�� UV���Ŵ� 8 λ��
		psrlw		xmm4, 0x08;		// ��һ�� UV  ��һ�� UV��-1 Ȩ�أ�
		psrlw		xmm3, 0x08;		// �ڶ��� UV  ��һ�� UV��3 Ȩ�أ�
		psrlw		xmm2, 0x07;		// �Ŵ� 1 λ
		psubw		xmm4, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm2, xmm3;
		psubw		xmm2, xmm4;
		movhlps		xmm3, xmm2;
		paddw		xmm2, xmm3;
		psrlw		xmm2, 0x02;				// @  VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm2, xmm4;
		packuswb	xmm1, xmm2;				// @  V2U2V0U0  �ڶ��� Y  ��һ�� Y
		movhlps		xmm2, xmm1;
		movaps		xmm0, xmm1;
		movaps		xmm3, xmm2;
		shufps		xmm1, xmm1, 0x01;		// @  @  @  �ڶ��� Y
		pand		xmm2, xmm0;				// @  U2 U0  @  U2 U0
		psrlw		xmm3, 0x08;				// @  V2 V0  @  V2 V0
		packuswb	xmm3, xmm2;				// @ U2U0  @ U2V0  @ V2V0  @ V2V0
		push		esi;
		push		edx;
		mov			ecx, y_pitch;
		pextrw		esi, xmm3, 0x04;
		pextrw		edx, xmm3, 0x00;
		movd		dword ptr [edi], xmm0;			// ��һ�� Y
		movd		dword ptr [edi + ecx], xmm1;	// �ڶ��� Y
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
		movd		xmm3, dword ptr [esi];	// ��һ�� V0 Y1 U0 Y0
		movd		xmm2, dword ptr [edx];	// �ڶ��� V0 Y1 U0 Y0
		movd		xmm1, dword ptr [edx + ecx];	// ��һ�� V0 Y1 U0 Y0
		movlhps		xmm1, xmm2;			// �ڶ���  ��һ��
		movlhps		xmm2, xmm3;			// ��һ��  �ڶ���
		movaps		xmm4, xmm1;
		movaps		xmm5, xmm1;
		pand		xmm1, xmm0;		// �ڶ��� Y  ��һ�� Y
		pand		xmm4, xmm7;		// �ڶ��� UV  ��һ�� UV���Ŵ� 8 λ��
		psrlw		xmm2, 0x08;				// ��һ�� UV  ��һ�� UV��-1 Ȩ�أ�
		psrlw		xmm5, 0x08;				// �ڶ��� UV  ��һ�� UV��3 Ȩ�أ�
		psrlw		xmm4, 0x07;				// �Ŵ� 1 λ
		psubw		xmm2, gc_nRoundComp_2;	// 0.5 * 4
		paddw		xmm4, xmm5;
		psubw		xmm4, xmm2;
		movhlps		xmm5, xmm4;
		paddw		xmm4, xmm5;
		packuswb	xmm1, xmm1;				// 0 �ڶ���Y  0 ��һ��Y  0 �ڶ���Y  0 ��һ��Y
		psrlw		xmm4, 0x02;				// @  @  @  VV0 UU0
		pxor		xmm3, xmm3;
		pmaxsw		xmm4, xmm3;
		pextrw		esi, xmm1, 0x00;
		pextrw		edx, xmm1, 0x02;
		mov			ecx, y_pitch;
		mov			word ptr [edi], si;			// ��һ�� Y
		mov			word ptr [edi + ecx], dx;	// �ڶ��� Y
		pextrw		ecx, xmm4, 0x00;
		pextrw		edx, xmm4, 0x01;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], dl;		// V
loop_end:
	}
}
// ��������һ�У�һ�㲻���ܳ���
void yuyv_to_yuv420_line_4_LS(OUT BYTE* y,			// һ�� y �������ŷ�ʽ Y0 Y1 Y2 Y3 ����
							  OUT BYTE* u,			// һ�� u �������ŷ�ʽ UU0 UU2 UU4 UU6 ����
							  OUT BYTE* v,			// һ�� v �������ŷ�ʽ VV0 VV2 VV4 VV6 ����
							  IN BYTE* yuyv0,		// ��һ�� yuyv ���루Ȩ�� -1-1������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							  IN BYTE* yuyv1,		// ��һ�� yuyv ���루Ȩ�� 3+3������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							  IN int na8,			// һ�����ؿ�ȳ��� 8 ������������ע��Ŀ��Ϊ 16 λ�Ļ��棩
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
		movups		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movups		xmm2, dword ptr [edx];			// ��һ�� V0 Y1 U0 Y0
		movaps		xmm3, xmm2;
		movaps		xmm4, xmm2;
		pand		xmm2, xmm0;		// Y7 Y6  Y5 Y4  Y3 Y2  Y1 Y0
		pand		xmm3, xmm7;		// V6 U6  V4 U4  V2 U2  V0 U0���Ŵ� 8 λ��
		psrlw		xmm1, 0x08;		// V6 U6  V4 U4  V2 U2  V0 U0��-1 Ȩ�أ�
		psrlw		xmm4, 0x08;		// 3 Ȩ��
		psrlw		xmm3, 0x07;		// �Ŵ� 1 λ
		psubw		xmm4, xmm1;
		paddw		xmm3, xmm6;
		paddw		xmm3, xmm4;
		psrlw		xmm3, 0x01;		// VV6 UU6 VV4 UU4 VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm3, xmm4;
		packuswb	xmm2, xmm3;		// V6U6V4U4  V2U2V0U0    ��һ�� Y
		movhlps		xmm3, xmm2;
		movaps		xmm5, xmm3;
		pand		xmm3, xmm0;		// @  @  0 U6 0 U4 0 U2 0 U0
		psrlw		xmm5, 0x08;		// @  @  0 V6 0 V4 0 V2 0 V0
		packuswb	xmm5, xmm3;		// @  U6U4U2U0  @  V6V4V2V0
		movhlps		xmm4, xmm5;
		movlps		dword ptr [edi], xmm1;			// ��һ�� Y
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
		movlps		xmm1, dword ptr [esi];			// ��һ�� V0 Y1 U0 Y0
		movlps		xmm2, dword ptr [edx];			// ��һ�� V0 Y1 U0 Y0
		movaps		xmm3, xmm2;
		movaps		xmm4, xmm2;
		pand		xmm2, xmm0;		// @  @  Y3 Y2  Y1 Y0
		pand		xmm3, xmm7;		// @  @  V2 U2  V0 U0���Ŵ� 8 λ��
		psrlw		xmm1, 0x08;		// @  @  V2 U2  V0 U0��-1 Ȩ�أ�
		psrlw		xmm4, 0x08;		// 3 Ȩ��
		psrlw		xmm3, 0x07;		// �Ŵ� 1 λ
		psubw		xmm4, xmm1;
		paddw		xmm3, xmm6;
		paddw		xmm3, xmm4;
		psrlw		xmm3, 0x01;		// @  @  VV2 UU2 VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm3, xmm4;
		packuswb	xmm2, xmm3;		// @  V2U2V0U0  @  ��һ�� Y
		movhlps		xmm3, xmm2;
		movaps		xmm5, xmm3;
		pand		xmm3, xmm0;		// @  @  @  0 U2 0 U0
		psrlw		xmm5, 0x08;		// @  @  @  0 V2 0 V0
		packuswb	xmm5, xmm3;		// @  @  @  V6V4V2V0
		push		esi;
		push		edx;
		pextrw		esi, xmm3, 0x04;
		pextrw		edx, xmm3, 0x00;
		movd		dword ptr [edi], xmm2;	// ��һ�� Y
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
		movd		xmm1, dword ptr [esi];	// ��һ�� V0 Y1 U0 Y0
		movd		xmm2, dword ptr [edx];	// ��һ�� V0 Y1 U0 Y0
		movaps		xmm3, xmm2;
		movaps		xmm4, xmm2;
		pand		xmm3, xmm7;		// @  @  @  V0 U0���Ŵ� 8 λ��
		psrlw		xmm1, 0x08;		// @  @  @  V0 U0��-1 Ȩ�أ�
		psrlw		xmm4, 0x08;		// 3 Ȩ��
		psrlw		xmm3, 0x07;		// �Ŵ� 1 λ
		psubw		xmm4, xmm1;
		paddw		xmm3, xmm6;
		paddw		xmm3, xmm4;
		psrlw		xmm3, 0x01;		// @  @  @  VV0 UU0
		pxor		xmm4, xmm4;
		pmaxsw		xmm3, xmm4;
		pextrw		ecx, xmm2, 0x00;
		pextrw		edx, xmm2, 0x01;
		mov			byte ptr [edi], cl;			// ��һ�� Y
		mov			byte ptr [edi + 0x01], dl;	// �ڶ��� Y
		pextrw		ecx, xmm3, 0x00;
		pextrw		edx, xmm3, 0x01;
		mov			byte ptr [eax], cl;		// U
		mov			byte ptr [ebx], dl;		// V
loop_end:
	}
}

// һ�� Buffer ��ɫ�ռ�ת��

// ��Ƭ�洢��Դ YUV422 �� YUV420

// ǰ����
void uv20_to_uv22_line_2_F(OUT void* u_o,         // 422 u ������洢��ʽ U0 U2 U4 U6 ���������У�
						   IN int u_o_p,
						   OUT void* v_o,         // 422 v ������洢��ʽ V0 V2 V4 V6 ���������У�
						   IN int v_o_p,
						   IN void* u_i,          // 420 u ���룬�洢��ʽ UU0 UU2 UU4 UU6 ����
						   IN int u_i_p_2,
						   IN void* v_i,          // 420 v ���룬�洢��ʽ VV0 VV2 VV4 VV6 ����
						   IN int v_i_p_2,
						   IN int na16,           // һ�����ؿ�ȵ�һ�루��Ϊһ���ڵ� UV �� Y Ϊ 422 �Ĺ�ϵ������ 16 ����������
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
		// ���Ϊ UU_0
		movdqu      [edi], xmm1;
		movdqu      [edx], xmm2;
		add			esi, u_i_p_2;
		add			eax, v_i_p_2;
		add         edi, u_o_p;
		add         edx, v_o_p;
		// ���Ϊ UU_0 * 0.75 + UU_2 * 0.25
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
		// ���Ϊ UU_0
		movsd       [edi], xmm1;
		movsd       [edx], xmm2;
		add			esi, u_i_p_2;
		add			eax, v_i_p_2;
		add         edi, u_o_p;
		add         edx, v_o_p;
		// ���Ϊ UU_0 * 0.75 + UU_2 * 0.25
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
		// ���Ϊ UU_0
		movss       [edi], xmm1;
		movss       [edx], xmm2;
		add			esi, u_i_p_2;
		add			eax, v_i_p_2;
		add         edi, u_o_p;
		add         edx, v_o_p;
		// ���Ϊ UU_0 * 0.75 + UU_2 * 0.25
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
		// ���Ϊ UU_0
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
// �м���
void uv20_to_uv22_line_2(OUT void* u_o,         // 422 u ������洢��ʽ U0 U2 U4 U6 ���������У�
						 IN int u_o_p,
						 OUT void* v_o,         // 422 v ������洢��ʽ V0 V2 V4 V6 ���������У�
						 IN int v_o_p,
						 IN void* u_i,          // 420 u ���룬�洢��ʽ UU0 UU2 UU4 UU6 ����
						 IN int u_i_p_2,
						 IN void* v_i,          // 420 v ���룬�洢��ʽ VV0 VV2 VV4 VV6 ����
						 IN int v_i_p_2,
						 IN int na16,           // һ�����ؿ�ȵ�һ�루��Ϊһ���ڵ� UV �� Y Ϊ 422 �Ĺ�ϵ������ 16 ����������
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
		add         esi, u_i_p_2;       // ��һ��
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
		// ��һ�������UU_P * 0.25 + UU_C * 0.75���ڶ��������UU_N * 0.25 + UU_C * 0.75
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
		packuswb    xmm1, xmm2;     // ��һ�����
		packuswb    xmm5, xmm6;     // �ڶ������
		movdqu      [edi], xmm1;
		movdqu      [ecx], xmm5;
		mov         esi, eax;
		mov         ecx, edx;
		sub         esi, v_i_p_2;   // ��һ��
		movdqu      xmm3, [eax];    // VV_C
		movdqu      xmm1, [esi];    // VV_P
		add         eax, v_i_p_2;   // ��һ��
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
		// ��һ�������VV_P * 0.25 + VV_C * 0.75���ڶ��������VV_N * 0.25 + VV_C * 0.75
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
		packuswb    xmm1, xmm2;     // ��һ�����
		packuswb    xmm5, xmm6;     // �ڶ������
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
		add         esi, u_i_p_2;       // ��һ��
		add         eax, v_i_p_2;
		movsd       xmm5, [esi];        // UU_N
		movsd       xmm6, [eax];        // VV_N
		punpcklbw   xmm1, xmm0;         // UU_P
		punpcklbw   xmm2, xmm0;         // VV_P
		punpcklbw   xmm3, xmm0;         // UU_C
		punpcklbw   xmm4, xmm0;         // VV_C
		punpcklbw   xmm5, xmm0;         // UU_N
		punpcklbw   xmm6, xmm0;         // VV_N
		// ��һ�������UU_P * 0.25 + UU_C * 0.75���ڶ��������UU_N * 0.25 + UU_C * 0.75
		// ��һ�������VV_P * 0.25 + VV_C * 0.75���ڶ��������VV_N * 0.25 + VV_C * 0.75
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
		add         esi, u_i_p_2;       // ��һ��
		add         eax, v_i_p_2;
		movss       xmm5, [esi];        // UU_N
		movss       xmm6, [eax];        // VV_N
		punpcklbw   xmm1, xmm0;         // UU_P
		punpcklbw   xmm2, xmm0;         // VV_P
		punpcklbw   xmm3, xmm0;         // UU_C
		punpcklbw   xmm4, xmm0;         // VV_C
		punpcklbw   xmm5, xmm0;         // UU_N
		punpcklbw   xmm6, xmm0;         // VV_N
		// ��һ�������UU_P * 0.25 + UU_C * 0.75���ڶ��������UU_N * 0.25 + UU_C * 0.75
		// ��һ�������VV_P * 0.25 + VV_C * 0.75���ڶ��������VV_N * 0.25 + VV_C * 0.75
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
		add         esi, u_i_p_2;       // ��һ��
		add         eax, v_i_p_2;
		pinsrw      xmm5, [esi], 0x00;  // UU_N
		pinsrw      xmm5, [eax], 0x02;  // VV_N
		punpcklbw   xmm1, xmm0;         // VV_P  UU_P
		punpcklbw   xmm3, xmm0;         // VV_C  UU_C
		punpcklbw   xmm5, xmm0;         // VV_N  UU_N
		// ��һ�������UU_P * 0.25 + UU_C * 0.75���ڶ��������UU_N * 0.25 + UU_C * 0.75
		// ��һ�������VV_P * 0.25 + VV_C * 0.75���ڶ��������VV_N * 0.25 + VV_C * 0.75
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
// �������
void uv20_to_uv22_line_2_L(OUT void* u_o,         // 422 u ������洢��ʽ U0 U2 U4 U6 ���������У�
						   IN int u_o_p,
						   OUT void* v_o,         // 422 v ������洢��ʽ V0 V2 V4 V6 ���������У�
						   IN int v_o_p,
						   IN void* u_i,          // 420 u ���룬�洢��ʽ UU0 UU2 UU4 UU6 ����
						   IN int u_i_p_2,
						   IN void* v_i,          // 420 v ���룬�洢��ʽ VV0 VV2 VV4 VV6 ����
						   IN int v_i_p_2,
						   IN int na16,           // һ�����ؿ�ȵ�һ�루��Ϊһ���ڵ� UV �� Y Ϊ 422 �Ĺ�ϵ������ 16 ����������
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
		// �ڶ��������UU_C
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		movdqu      [eax], xmm3;
		movdqu      [ecx], xmm6;
		// ��һ�������UU_P * 0.25 + UU_C * 0.75
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
		// �ڶ��������UU_C
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		movsd       [eax], xmm3;
		movsd       [ecx], xmm6;
		// ��һ�������UU_P * 0.25 + UU_C * 0.75
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
		// �ڶ��������UU_C
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		movss       [eax], xmm3;
		movss       [ecx], xmm6;
		// ��һ�������UU_P * 0.25 + UU_C * 0.75
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
		// �ڶ��������VV_C
		pextrw      ebx, xmm3, 0x00;
		pextrw      esi, xmm3, 0x02;
		mov         eax, edi;
		mov         ecx, edx;
		add         eax, u_o_p;
		add         ecx, v_o_p;
		mov         [eax], bx;
		mov         [ecx], si;
		// ��һ�������VV_P * 0.25 + VV_C * 0.75
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
// ���һ��
void uv20_to_uv22_line_1_L(OUT void* u_o,         // 422 u ������洢��ʽ U0 U2 U4 U6 ���������У�
						   OUT void* v_o,         // 422 v ������洢��ʽ V0 V2 V4 V6 ���������У�
						   IN void* u_i,          // 420 u ���룬�洢��ʽ UU0 UU2 UU4 UU6 ����
						   IN int u_i_p_2,
						   IN void* v_i,          // 420 v ���룬�洢��ʽ VV0 VV2 VV4 VV6 ����
						   IN int v_i_p_2,
						   IN int na16,           // һ�����ؿ�ȵ�һ�루��Ϊһ���ڵ� UV �� Y Ϊ 422 �Ĺ�ϵ������ 16 ����������
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
		// �����UU_P * 0.25 + UU_C * 0.75
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
		// �����UU_P * 0.25 + UU_C * 0.75
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
		// �����UU_P * 0.25 + UU_C * 0.75
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
		// �����VV_P * 0.25 + VV_C * 0.75
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

void yuv422_to_color32_line(OUT void* color32,      // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
							IN void* y,             // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
							IN void* u,             // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
							IN void* v,             // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
							IN int na16,            // Դ�ֽڿ�ȼ� 4 ����� 16 ����������
							IN int nm16,
							IN int bHD,             // �߱����־
							IN BOOL bBGRA)          // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			yuv422_to_color32_inner(color32, y, v, u, na16, nm16, g_nB_U_HD, g_nG_V_HD, g_nG_U_HD, g_nR_V_HD);
		}
		else       // R �ڵ�λ
		{
			yuv422_to_color32_inner(color32, y, u, v, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
	}
	else    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			yuv422_to_color32_inner(color32, y, v, u, na16, nm16, g_nB_U_SD, g_nG_V_SD, g_nG_U_SD, g_nR_V_SD);
		}
		else       // R �ڵ�λ
		{
			yuv422_to_color32_inner(color32, y, u, v, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
	}
}
void yuv422_to_color32_inner(OUT void* color32,     // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
							 IN void* y,            // y ���룬��ŷ�ʽ Y0 Y1 Y2 Y3 ����
							 IN void* u,            // u ���룬��ŷ�ʽ UU0 UU2 UU4 UU6 ����
							 IN void* v,            // v ���룬��ŷ�ʽ VV0 VV2 VV4 VV6 ����
							 IN int na8,		    // Դ���ؿ�ȼ� 1 ����� 8 ����������
							 IN int nm8,
							 IN int* pnR_V,         // ϵ��
							 IN int* pnG_U,         // ��ͨ������ U �� V �������� RGBA �� BGRA �ļ��ݣ�
							 IN int* pnG_V,         // ������ u �� v ������pnR_V �� pnB_U ������pnG_V �� pnG_U ��������ʵ�֣�
							 IN int* pnB_U)
{
	_asm{       // �����ڲ��� RGBA Ϊ������˵��
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
		psllw       xmm5, 0x06;         // �ܹ��Ŵ� 7 λ
		psllw       xmm4, 0x06;
		psllw       xmm1, 0x05;         // �Ŵ� 5 λ
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
		pmulhw		xmm2, xmm4;         // ���Ŵ� 5 λ
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
		psllw       xmm5, 0x06;         // �ܹ��Ŵ� 7 λ
		psllw       xmm4, 0x06;
		psllw       xmm1, 0x05;         // �Ŵ� 5 λ
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
		pmulhw		xmm2, xmm4;         // ���Ŵ� 5 λ
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
		psllw       xmm5, 0x06;         // �ܹ��Ŵ� 7 λ
		psllw       xmm1, 0x05;         // �Ŵ� 5 λ
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
		pmulhw		xmm2, xmm4;         // ���Ŵ� 5 λ
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
		psllw       xmm5, 0x06;         // �ܹ��Ŵ� 7 λ
		psllw       xmm1, 0x05;         // �Ŵ� 5 λ
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
		pmulhw		xmm2, xmm4;         // ���Ŵ� 5 λ
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
		psllw       xmm5, 0x06;         // �ܹ��Ŵ� 7 λ
		psllw       xmm1, 0x05;         // �Ŵ� 5 λ
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
		pmulhw		xmm2, xmm4;         // ���Ŵ� 5 λ
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
		psllw       xmm1, 0x05;         // �Ŵ� 5 λ
		psllw       xmm7, 0x07;         // �ܹ��Ŵ� 7 λ
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
		pmulhw		xmm2, xmm4;         // ���Ŵ� 5 λ
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
		psllw       xmm7, 0x07;     // �Ŵ� 7 λ
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
		pmulhw		xmm2, xmm4;         // ���Ŵ� 5 λ
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

// ���� Alpha ͨ����ֱ�Ӷ����ڶ��� UV
void color32_to_yuyva_line(OUT void* yuyv,		// Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   OUT void* alpha,	    // Ŀ�� alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
						   IN void* color32,	// Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN int na16,	    	// Դ�ֽڿ�ȳ��� 16 ����������
						   IN int nm16,
						   IN BOOL bHD,         // �߱����־
						   IN BOOL bBGRA)       // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			color32_to_yuyva_inner(yuyv, alpha, color32, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
		}
		else    // R �ڵ�λ
		{
			color32_to_yuyva_inner(yuyv, alpha, color32, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
		}
	}
	else    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			color32_to_yuyva_inner(yuyv, alpha, color32, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
		}
		else    // R �ڵ�λ
		{
			color32_to_yuyva_inner(yuyv, alpha, color32, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
		}
	}
}
void color32_to_yuyva_inner(OUT void* yuyv,	        // Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							OUT void* alpha,    	// Ŀ�� alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
							IN void* color32,	    // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
							IN int na16,    		// Դ�ֽڿ�ȳ��� 16 ����������
							IN int nm16,
							IN int* pnY32_BR,       // ϵ����ͨ����� R ������ B ������ϵ��λ�ÿ������� BGRA �� RGBA �ļ��ݣ�
							IN int* pnY32_G,        // BR ��ʾ R �ڵ�λ
							IN int* pnY32_BGR,
							IN int* pnUV_BGR,
							IN int* pnU_BGR,
							IN int* pnV_BGR)
{
	_asm{       // �����ڲ��� RGBA ��ɫΪ����˵��
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
		shufps		xmm7, xmm7, 0x08;	// ȡ U0V0 �� U2V2
		pand		xmm1, gc_dwMaskWLB;	// 0B0R 0B0R 0B0R 0B0R
		pand		xmm2, gc_dwMaskWHB;	// A0G0 A0G0 A0G0 A0G0
		punpcklbw	xmm0, xmm7;			// A0B0 G0R0 A0B0 G0R0
		psrld		xmm3, 0x18;			// 000A 000A 000A 000A
		psllw		xmm1, 0x07;			// �Ŵ� 7 λ
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		packssdw	xmm3, xmm3;			// 0A0A 0A0A 0A0A 0A0A
		movdqu		xmm7, [ebx];
		pmulhw		xmm6, xmm1;			// ���Ŵ� 5 λ
		pmulhw		xmm7, xmm2;
		pmulhw		xmm4, xmm0;
		pmulhw		xmm5, xmm0;
		packuswb	xmm3, xmm3;			// AAAA AAAA AAAA AAAA
		paddsw		xmm7, xmm6;			// YR + YG
		pshufd		xmm1, xmm4, 0xb1;
		pshufd		xmm2, xmm5, 0xb1;
		psrld		xmm6, 0x10;			// YB
		movss		[edx], xmm3;		// ���� Alpha
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
		psrlw		xmm0, 0x01;			// �Ŵ� 7 λ
		psrlw		xmm1, 0x01;			// �Ŵ� 7 λ
		packssdw	xmm7, xmm7;			// 0000 0A0A 0000 0A0A
		pmulhw		xmm5, xmm1;			// ���Ŵ� 5 λ
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
		psrlw		xmm0, 0x01;			// �Ŵ� 7 λ
		pand		xmm4, xmm3;			// 00A0 00A0 00A0 00A0
		pmulhw		xmm5, xmm0;			// ���Ŵ� 5 λ
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
// ���� Alpha ͨ����ֱ�Ӷ����ڶ��� UV
void color32_to_yuyv_line(OUT void* yuyv,		// Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						  IN void* color32,	    // Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						  IN int na16,	    	// Դ�ֽڿ�ȳ��� 16 ����������
						  IN int nm16,
						  IN BOOL bHD,          // �߱����־
						  IN BOOL bBGRA)        // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			color32_to_yuyv_inner(yuyv, color32, na16, nm16, g_nY32_RB_HD, g_nY32_G_HD, g_nY32_RGB_HD, g_nUV_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD);
		}
		else        // R �ڵ�λ
		{
			color32_to_yuyv_inner(yuyv, color32, na16, nm16, g_nY32_BR_HD, g_nY32_G_HD, g_nY32_BGR_HD, g_nUV_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD);
		}
	}
	else
	{
		if (bBGRA)      // B �ڵ�λ
		{
			color32_to_yuyv_inner(yuyv, color32, na16, nm16, g_nY32_RB_SD, g_nY32_G_SD, g_nY32_RGB_SD, g_nUV_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD);
		}
		else        // R �ڵ�λ
		{
			color32_to_yuyv_inner(yuyv, color32, na16, nm16, g_nY32_BR_SD, g_nY32_G_SD, g_nY32_BGR_SD, g_nUV_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD);
		}
	}
}
void color32_to_yuyv_inner(OUT void* yuyv, 	    // Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN void* color32,	// Դ *g@a ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN int na16,		    // Դ�ֽڿ�ȳ��� 16 ����������
						   IN int nm16,
						   IN int* pnY32_BR,    // ϵ����ͨ����� R ������ B ������ϵ��λ�ÿ������� BGRA �� RGBA �ļ��ݣ�
						   IN int* pnY32_G,     // BR ��ʾ R �ڵ�λ
						   IN int* pnY32_BGR,
						   IN int* pnUV_BGR,
						   IN int* pnU_BGR,
						   IN int* pnV_BGR)
{
	_asm{       // �����ڲ��� RGBA Ϊ����˵��
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
		shufps		xmm7, xmm7, 0x08;	// ȡ U0V0 �� U2V2
		pand		xmm1, gc_dwMaskWLB;	// 0B0R 0B0R 0B0R 0B0R
		pand		xmm2, gc_dwMaskWHB;	// A0G0 A0G0 A0G0 A0G0
		punpcklbw	xmm0, xmm7;			// A0B0 G0R0 A0B0 G0R0
		psllw		xmm1, 0x07;			// �Ŵ� 7 λ
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		push        ecx;
		mov         ecx, pnU_BGR;
		movdqu		xmm7, [ebx];
		movdqu		xmm4, [ecx];  	    // U
		movdqu		xmm5, [edx];  	    // V
		pop         ecx;
		pmulhw		xmm6, xmm1;			// ���Ŵ� 5 λ
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
		psrlw		xmm0, 0x01;			// �Ŵ� 7 λ
		psrlw		xmm1, 0x01;			// �Ŵ� 7 λ
		pmulhw		xmm5, xmm0;			// ���Ŵ� 5 λ
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
		psrlw		xmm0, 0x01;			// �Ŵ� 7 λ
		pmulhw		xmm5, xmm0;			// ���Ŵ� 5 λ
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
// ʹ�� Alpha ͨ����ǰ���ֵ��õڶ��� UV
void yuyva_to_color32_line(OUT void* color32,    // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN void* yuyv,	 	 // Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN void* alpha,		 // Դ alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
						   IN int na16,		     // Դ�ֽڿ�ȼ� 4 ����� 16 ����������
						   IN int nm16,
						   IN BOOL bHD,          // �߱����־
						   IN BOOL bBGRA)        // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			yuyva_to_bgra_inner(color32, yuyv, alpha, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
		else       // R �ڵ�λ
		{
			yuyva_to_rgba_inner(color32, yuyv, alpha, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
	}
	else    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			yuyva_to_bgra_inner(color32, yuyv, alpha, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
		else       // R �ڵ�λ
		{
			yuyva_to_rgba_inner(color32, yuyv, alpha, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
	}
}
void yuyva_to_rgba_inner(OUT void* rgba,        // RGBA ���棬��ŷ�ʽ R0 G0 B0 A0 R1 G1 B1 A1 ����
						 IN void* yuyv, 		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						 IN void* alpha,		// Դ alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
						 IN int na16,		    // Դ�ֽڿ�ȼ� 4 ����� 16 ����������
						 IN int nm16,
						 IN int* pnR_V,         // ϵ�����޷����� RGBA �� BGRA �ļ��ݣ�
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
		add			eax, 0x10;			// ��һ�� VYUV ���� UV ��ֵ
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// ��һ�� VYUV ���� UV ��ֵ
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psrlw		xmm6, 2;			// �Ŵ� 6 λ
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubsw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		movss		xmm6, [eax];		// ����������� 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
void yuyva_to_bgra_inner(OUT void* bgra,        // BGRA ���棬��ŷ�ʽ B0 G0 R0 A0 B1 G1 R1 A1 ����
						 IN void* yuyv, 		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						 IN void* alpha,		// Դ alpha ���棬��ŷ�ʽ A0 A1 A2 A3 ����
						 IN int na16,		    // Դ�ֽڿ�ȼ� 4 ����� 16 ����������
						 IN int nm16,
						 IN int* pnR_V,         // ϵ�����޷����� RGBA �� BGRA �ļ��ݣ�
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
		add			eax, 0x10;			// ��һ�� VYUV ���� UV ��ֵ
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// ��һ�� VYUV ���� UV ��ֵ
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psrlw		xmm6, 2;			// �Ŵ� 6 λ
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubsw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		movss		xmm6, [eax];		// ����������� 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
// ���� Alpha ͨ��Ϊ 255��ǰ���ֵ��õڶ��� UV
void yuyv_to_color32_line(OUT void* color32,    // *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
						  IN void* yuyv,		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						  IN int na16,			// Դ�ֽڿ�ȼ� 4 ����� 16 ����������
						  IN int nm16,
						  IN int bHD,           // �߱����־
						  IN BOOL bBGRA)        // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	if (bHD)    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			yuyv_to_bgra_inner(color32, yuyv, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
		else        // R �ڵ�λ
		{
			yuyv_to_rgba_inner(color32, yuyv, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
	}
	else    // ����
	{
		if (bBGRA)      // B �ڵ�λ
		{
			yuyv_to_bgra_inner(color32, yuyv, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
		else        // R �ڵ�λ
		{
			yuyv_to_rgba_inner(color32, yuyv, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
	}
}
void yuyv_to_rgba_inner(OUT void* rgba,	        // RGBA ���棬��ŷ�ʽ R0 G0 B0 A0 R1 G1 B1 A1 ����
						IN void* yuyv, 		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						IN int na16,		    // Դ�ֽڿ�ȼ� 4 ����� 16 ����������
						IN int nm16,
						IN int* pnR_V,        // ϵ�����޷����� RGBA �� BGRA �ļ��ݣ�
						IN int* pnG_U,
						IN int* pnG_V,
						IN int* pnB_U)
{
	_asm{
		mov			eax, yuyv;
		mov			esi, yuyv;
		mov			ecx, na16;
		add			eax, 0x10;			// ��һ�� VYUV ���� UV ��ֵ
		mov			edi, rgba;
		mov         ebx, pnG_V;
		mov         edx, pnG_U;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// ��һ�� VYUV ���� UV ��ֵ
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psrlw		xmm6, 2;			// �Ŵ� 6 λ
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubsw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		movss		xmm6, [eax];		// ����������� 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
void yuyv_to_bgra_inner(OUT void* bgra,     // BGRA ���棬��ŷ�ʽ B0 G0 R0 A0 B1 G1 R1 A1 ����
						IN void* yuyv, 		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						IN int na16,		// Դ�ֽڿ�ȼ� 4 ����� 16 ����������
						IN int nm16,
						IN int* pnR_V,      // ϵ�����޷����� RGBA �� BGRA �ļ��ݣ�
						IN int* pnG_U,
						IN int* pnG_V,
						IN int* pnB_U)
{
	_asm{
		mov			eax, yuyv;
		mov			esi, yuyv;
		mov			ecx, na16;
		add			eax, 0x10;			// ��һ�� VYUV ���� UV ��ֵ
		mov			edi, bgra;
		mov         ebx, pnG_V;
		mov         edx, pnG_U;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// ��һ�� VYUV ���� UV ��ֵ
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psrlw		xmm6, 2;			// �Ŵ� 6 λ
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubsw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		movss		xmm6, [eax];		// ����������� 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
// ֱ�Ӷ����ڶ��� UV
void color24_to_yuyv_line(OUT void* yuyv,		// Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						  IN void* color24,		// *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						  IN int na12,			// Դ�ֽڿ�ȼ� 4 ����� 12 ����
						  IN int nmp,			// ���� na12 ѭ����ʣ�������
						  IN int bHD,           // �߱����־
						  IN BOOL bBGR)         // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	if (bHD)    // ����
	{
		if (bBGR)      // B �ڵ�λ
		{
			color24_to_yuyv_inner(yuyv, color24, na12, nmp, g_nY24_BGR_HD, g_nY32_RGB_HD, g_nU_RGB_HD, g_nV_RGB_HD, g_nUV_RGB_HD);
		}
		else    // R �ڵ�λ
		{
			color24_to_yuyv_inner(yuyv, color24, na12, nmp, g_nY24_RGB_HD, g_nY32_BGR_HD, g_nU_BGR_HD, g_nV_BGR_HD, g_nUV_BGR_HD);
		}
	}
	else    // ����
	{
		if (bBGR)      // B �ڵ�λ
		{
			color24_to_yuyv_inner(yuyv, color24, na12, nmp, g_nY24_BGR_SD, g_nY32_RGB_SD, g_nU_RGB_SD, g_nV_RGB_SD, g_nUV_RGB_SD);
		}
		else    // R �ڵ�λ
		{
			color24_to_yuyv_inner(yuyv, color24, na12, nmp, g_nY24_RGB_SD, g_nY32_BGR_SD, g_nU_BGR_SD, g_nV_BGR_SD, g_nUV_BGR_SD);
		}
	}
}
void color24_to_yuyv_inner(OUT void* yuyv,		// Ŀ�� yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN void* color24,	// *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN int na12,			// Դ�ֽڿ�ȼ� 4 ����� 12 ����
						   IN int nmp,			// ���� na12 ѭ����ʣ�������'
						   IN int* pnY24_RGB,   // ϵ����ͨ����� R ������ B ������ϵ��λ�ÿ������� BGRA �� RGBA �ļ��ݣ�
						   IN int* pnY32_BGR,
						   IN int* pnU_BGR,
						   IN int* pnV_BGR,
						   IN int* pnUV_BGR)
{
	_asm{       // �����ڲ��� RGB Ϊ������˵��
		mov			ecx, na12;
		mov			edi, yuyv;
		mov			esi, color24;
		mov         eax, pnU_BGR;
		mov         ebx, pnV_BGR;
		mov         edx, pnY24_RGB;
		test		ecx, ecx;
		jnz			loop_a12;
		jmp			loop_mp4;
loop_a12:		// ֻ��ǰ�ĸ�����
		movdqu		xmm7, [esi];		// RBGR BGRB GRBG RBGR
		movdqu		xmm4, [eax];      	// U
		movdqu		xmm5, [ebx];  	    // V
		movdqu		xmm6, [edx];  	    // YrYg YbYr YgYb YrYg
		movdqa		xmm1, xmm7;
		movdqa		xmm2, xmm7;
		movdqa		xmm3, xmm7;			// Ϊ��ȡ U0V0 �� U2V2
		pxor		xmm0, xmm0;
		pand		xmm1, gc_dwMaskWLB;	// ** G3B2 R2G1 B0R0
		pand		xmm2, gc_dwMaskWHB;	// ** B3R3 G2B1 R1G0
		punpcklbw	xmm0, xmm7;			// G2R2 ** *B0 G0R0
		pslld		xmm3, 0x08;			// ** *B2 ** **
		pshufd		xmm7, xmm6, 0x12;	// YrYg YgYb YrYg YbYr
		shufps		xmm3, xmm0, 0xfa;	// G2R2 G2R2 *B2 *B2
		shufps		xmm0, xmm3, 0x24;	// *B2 G2R2 *B0 G0R0
		psllw		xmm1, 0x07;			// �Ŵ� 7 λ
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		pmulhw		xmm6, xmm2;			// ���Ŵ� 5 λ
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
		add			esi, 0x0c;			// 12 �ֽ�
		add			edi, 0x08;
		sub			ecx, 0x01;
		jnz			loop_a12;
loop_mp4:		// ʣ���ĸ�����
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
		movdqa		xmm3, xmm7;			// Ϊ��ȡ U0V0 �� U2V2
		pand		xmm1, gc_dwMaskWLB;		// ** G3B2 R2G1 B0R0
		pand		xmm2, gc_dwMaskWHB;	// ** B3R3 G2B1 R1G0
		punpcklbw	xmm0, xmm7;			// G2R2 ** *B0 G0R0
		pslld		xmm3, 0x08;			// ** *B2 ** **
		pshufd		xmm7, xmm6, 0x12;	// YrYg YgYb YrYg YbYr
		shufps		xmm3, xmm0, 0xfa;	// G2R2 G2R2 *B2 *B2
		shufps		xmm0, xmm3, 0x24;	// *B2 G2R2 *B0 G0R0
		psllw		xmm1, 0x07;			// �Ŵ� 7 λ
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		pmulhw		xmm6, xmm2;			// ���Ŵ� 5 λ
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
loop_mp3:		// ʣ����������
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
		movdqa		xmm3, xmm7;			// Ϊ��ȡ U0V0 �� U2V2
		pand		xmm1, gc_dwMaskWLB;		// ** G3B2 R2G1 B0R0
		pand		xmm2, gc_dwMaskWHB;	// ** B3R3 G2B1 R1G0
		punpcklbw	xmm0, xmm7;			// G2R2 ** *B0 G0R0
		pslld		xmm3, 0x08;			// ** *B2 ** **
		pshufd		xmm7, xmm6, 0x12;	// YrYg YgYb YrYg YbYr
		shufps		xmm3, xmm0, 0xfa;	// G2R2 G2R2 *B2 *B2
		shufps		xmm0, xmm3, 0x24;	// *B2 G2R2 *B0 G0R0
		psllw		xmm1, 0x07;			// �Ŵ� 7 λ
		psrlw		xmm2, 0x01;
		psrlw		xmm0, 0x01;
		pmulhw		xmm6, xmm2;			// ���Ŵ� 5 λ
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
		psrlw		xmm0, 0x01;			// �Ŵ� 7 λ
		psrlw		xmm1, 0x01;
		pmulhw		xmm5, xmm0;			// ���Ŵ� 5 λ
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
		psrlw		xmm0, 0x01;			// �Ŵ� 7 λ
		pmulhw		xmm5, xmm0;			// ���Ŵ� 5 λ
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
// ǰ���ֵ��õڶ��� UV
void yuyv_to_color24_line(OUT void* color24,	// *G@ ���棬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						  IN void* yuyv,		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						  IN int na16,			// Դ�ֽڿ�ȼ� 4 ����� 16 ����������
						  IN int nm16,
						  IN int bHD,           // �߱����־
						  IN BOOL bBGR)         // ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
{
	if (bHD)    // ����
	{
		if (bBGR)       // B �ڵ�λ
		{
			yuyv_to_color24_inner(color24, yuyv, na16, nm16, g_nB_U_HD, g_nG_U_HD, g_nG_V_HD, g_nR_V_HD);
		}
		else        // R �ڵ�λ
		{
			yuyv_to_color24_inner(color24, yuyv, na16, nm16, g_nR_V_HD, g_nG_U_HD, g_nG_V_HD, g_nB_U_HD);
		}
	}
	else    // ����
	{
		if (bBGR)       // B �ڵ�λ
		{
			yuyv_to_color24_inner(color24, yuyv, na16, nm16, g_nB_U_SD, g_nG_U_SD, g_nG_V_SD, g_nR_V_SD);
		}
		else        // R �ڵ�λ
		{
			yuyv_to_color24_inner(color24, yuyv, na16, nm16, g_nR_V_SD, g_nG_U_SD, g_nG_V_SD, g_nB_U_SD);
		}
	}
}
void yuyv_to_color24_inner(OUT void* color24,   // *G@ ���棬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						   IN void* yuyv,		// Դ yuyv ���棬��ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
						   IN int na16,			// Դ�ֽڿ�ȼ� 4 ����� 16 ����������
						   IN int nm16,
						   IN int* pnR_V,       // ϵ����ͨ����� R ������ B ������ϵ��λ�ÿ������� BGRA �� RGBA �ļ��ݣ�
						   IN int* pnG_U,       // ���˴�ֻ��Ҫ�� pnR_V �� pnB_U �����뽻������ʵ�֣�
						   IN int* pnG_V,
						   IN int* pnB_U)
{
	_asm{       // �����ڲ��� RGBA Ϊ����˵��
		mov			esi, yuyv;
		mov			eax, yuyv;
		mov			edi, color24;
		mov			edx, color24;
		mov			ecx, na16;
		mov         ebx, pnG_V;
		add			eax, 0x10;			// ��һ�� VYUV ���� UV ��ֵ
		add			edx, 0x10;			// �°��� RGB �洢
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_m16;
loop_a16:
		movdqu		xmm7, [esi];		// VYUY VYUY VYUY VYUY
		movss		xmm6, [eax];		// ��һ�� VYUV ���� UV ��ֵ
		movdqa		xmm1, xmm7;
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm3, gc_dwMaskWHB;
		movdqa		xmm4, gc_dwMaskSpecUV;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, xmm3;			// V6 U6 V4 U4 V2 U2 V0 U0
		pand		xmm6, xmm3;			// 0 0 0 0 0 0 V8 U8
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psrlw		xmm6, 2;			// �Ŵ� 6 λ
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		pshufd		xmm2, xmm1, 0x39;	// V0 U0 V6 U6 V4 U4 V2 U2
		shufps		xmm6, xmm6, 0x15;	// V8 U8 0 0 0 0 0 0
		pand		xmm2, xmm4;			// 0 0 V6 U6 V4 U4 V2 U2
		por			xmm2, xmm6;			// V8 U8 V6 U6 V4 U4 V2 U2
		psubsw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubsw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V6 U6 V6 U6 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		movss		xmm6, [eax];		// ����������� 0000 0000 0000 VYUY
		shufps		xmm6, xmm6, 0x00;
		movlhps		xmm7, xmm6;			// VYUY VYUY VYUY VYUY
		movdqa		xmm0, gc_dwMaskWLB;
		movdqa		xmm1, xmm7;
		movdqa		xmm5, gc_nOff128_6;
		pand		xmm7, xmm0;			// Y5 Y4 Y5 Y4 Y3 Y2 Y1 Y0
		pand		xmm1, gc_dwMaskWHB;	// V4 U4 V4 U4 V2 U2 V0 U0
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		pshufd		xmm2, xmm1, 0xf9;	// V4 U4 V4 U4 V4 U4 V2 U2
		psubw		xmm1, xmm5;			// ��ȥƫ���� 128
		psubw		xmm2, xmm5;			// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
		psllw		xmm7, 5;			// �Ŵ� 5 λ
		psrlw		xmm1, 2;			// �Ŵ� 6 λ
		psubw		xmm1, gc_nOff128_6;	// ��ȥƫ���� 128
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
// ���� Alpha ͨ��Ϊ 0xff
void color24_to_color32(OUT void* color32,	// *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ����
						IN void* color24,	// *G@ ���룬��ŷ�ʽ *0 G0 @0 *1 G1 @1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ����
						IN int nap5,		// Դ���ؿ�ȳ��� 5 ��������������������ʮ���ֽڶ��룩
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
		shr			edx, 04h;				// һ�δ���16������
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
//	_asm{       // �����ڲ��� RGB �� RGBA Ϊ����˵��
//		mov			edi, color32;		// ǰ�ĸ�����
//		mov			edx, color32;		// ���һ������
//		mov			ecx, nap5;
//		mov			esi, color24;
//		add			edx, 0x10;
//		movdqa		xmm0, gc_dwMaskAlpha;
//		test		ecx, ecx;
//		jnz			loop_a5;
//		jmp			loop_m4;
//loop_a5:		// һ�����������
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
//loop_m4:		// ʣ���ĸ�����
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
//loop_m3:		// ʣ����������
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
//loop_m2:		// ʣ����������
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
//loop_m1:		// ʣ��һ������
//		test		ecx, 0x01;
//		jz			loop_end;
//		movss		xmm7, [esi];
//		por			xmm7, xmm0;
//		movss		[edi], xmm7;
//loop_end:
//	}
}

// yuyv �� bgra/rgba ��ƽ����ֵ���ű任������ Alpha ͨ��Ϊ 0xff
BOOL yuyv_to_color32_meanstretch(OUT BYTE* color32,		// *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ������* �� @ ��ʾ R �� B�������صĴ洢��ʽȷ������Alpha ֱ���� 0xff��
								 IN int nDesPitch,
								 IN int nDesX,			// Ŀ�����
								 IN int nDesY,
								 IN int nDesWidth,		// Ŀ�������С
								 IN int nDesHeight,
								 IN int nDesImgWidth,	// Ŀ��ͼ���С
								 IN int nDesImgHeight,
								 IN BYTE* pSrcYUYV,		// YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
								 IN int nSrcPitch,
								 IN int nSrcX,			// Դ���
								 IN int nSrcY,
								 IN int nSrcWidth,		// Դ������С
								 IN int nSrcHeight,
								 IN int nSrcImgWidth,	// Դͼ���С������ԴΪ16λ��Ҫ��˿��Ϊż����
								 IN int nSrcImgHeight,
								 IN BOOL bSrcHD,		// Դ�źŵĸ߱����־������ʹ����ɫ�任�Ĺ�ʽ��
								 IN BOOL bBGRA,			// ��������ɫ�����Ĵ洢˳��Ϊ TRUE ��ʾ�� B �ڵ�λ
								 IN BOOL bDesRevert)	// ͼ���Ƿ��з�ת��־
{
	if (pSrcYUYV == NULL || color32 == NULL)
		return FALSE;

	float fScaleWidth(1.0f), fScaleHeight(1.0f), fSrcStartX(0.0f), fSrcStartY(0.0f);
	int nDesAddWidth(0), nLineIndex(0), nLineInc(1);

	// �����Ч�Ĳ�����Χ
	if (!TPMeanStretch_CurRange(nDesX, nDesY, nDesWidth, nDesHeight, nDesImgWidth, nDesImgHeight,
		nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcImgWidth, nSrcImgHeight, fScaleWidth, fScaleHeight,
		fSrcStartX, fSrcStartY, nDesAddWidth, nLineIndex, nLineInc, TP_PARITY_BOTH))
		return FALSE;

	// Դ������㣬���� YUYV �洢��ԭ�򣬱���ʹ��λ�ö���ż����
	int nAlignSrcX = nSrcX - (nSrcX & 0x01);
	int nAlignSrcWidth = ((nSrcX + nSrcWidth) & 0x01) + nSrcWidth + (nSrcX & 0x01);
	float fScaleW_I = 1.0f / fScaleWidth, fScaleH_I = 1.0f / fScaleHeight;
	// ����Ȩ�ر�Ȩ������������ռ�
	TPSSE_DECLARE(int*) pMapX = TPSSE_NEW(nDesWidth, int);		// X �����ϵ�Ȩ��������
	TPSSE_DECLARE(int*) pMapY = TPSSE_NEW(nDesHeight, int);	// Y �����ϵ�Ȩ��������
	TPSSE_DECLARE(float*) pPowerX = TPSSE_NEW((nDesWidth << 1), float);	// X �����ϵ�Ȩ�ر�
	TPSSE_DECLARE(float*) pPowerY = TPSSE_NEW((nDesHeight << 1), float);	// Y �����ϵ�Ȩ�ر�
	BYTE *pDesLine = color32 + nDesX + nDesY * nDesPitch;		// Buffer ��ʼ��ַ
	BYTE *pSrcLine = pSrcYUYV + nAlignSrcX + nSrcY * nSrcPitch;
	float *pTmpPowerY = pPowerY;		// Y ����Ȩ�ر��ַ
	TPSSE_DECLARE(float*) pTmpLine = TPSSE_NEW((nAlignSrcWidth << 2), float);	// Դ�е���ʱ����
	float *pTmpYUYVLine = pTmpLine + (nAlignSrcWidth << 1);
	TPSSE_DECLARE(float) fCoefW[4] = { fScaleW_I, fScaleW_I, fScaleW_I, fScaleW_I };
	TPSSE_DECLARE(float) fCoefH[4] = { fScaleH_I, fScaleH_I, fScaleH_I, fScaleH_I };
	float* pfRB_YUV = (bSrcHD) ? g_fRB_YUV_HD : g_fRB_YUV_SD;
	float* pfG_YUV = (bSrcHD) ? g_fG_YUV_HD : g_fG_YUV_SD;

	nDesAddWidth = bDesRevert ? -nDesPitch : nDesPitch;	// ��תͼ��
	pDesLine += (bDesRevert ? (nDesHeight - 1) * nDesPitch : 0);
	// ����Ȩ��
	strech_calc_power(pPowerX, pMapX, nDesWidth, fScaleWidth, fSrcStartX - (int)fSrcStartX);
	strech_calc_power(pPowerY, pMapY, nDesHeight, fScaleHeight, fSrcStartY - (int)fSrcStartY);
	// ���д���
	for (; nLineIndex<nDesHeight; nLineIndex+=nLineInc)
	{
		if ((nSrcY + pMapY[nLineIndex]) > nSrcImgHeight)	// ��ֹ�����Խ�����
		{
			pSrcLine = pSrcYUYV + nAlignSrcX + (nSrcImgHeight - pMapY[nLineIndex]) * nSrcPitch;
		}
		// ���� YUYV �� 16 λ���ԣ��� 32 λ����ʱ��Ҫ����۰롣
		// �������ʱ������м俪ʼ�ţ��Է��������� YUYV �� YUVY ����չ��ע�����ͨ���ظ��� Y��
		bgra32_mean_copy_add_y_quick(pTmpYUYVLine, pSrcLine, (nAlignSrcWidth >> 1),
			nSrcPitch, pTmpPowerY, fCoefH, pMapY[nLineIndex]);
		yuyv_mean_color32_midres(pTmpLine, pTmpYUYVLine, nAlignSrcWidth);
		// ˮƽ��Ȩʱ����Ҫ����ȥ�����ż����������أ�һ������ռ�ĸ�����������
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
// ƽ����ֵ����ʱ������ YUYV �� YUVY �ı任
void yuyv_mean_color32_midres(OUT void* yuvy,		// ����������ŷ�ʽ Y0 U0 V0 Y0 Y1 U1 V1 Y1 ����
							  IN void* yuyv,		// ���㴹ֱ���ӽ������ŷ�ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V2 ����
							  IN int nCount)		// ���ص��������Ϊż����
{
	__asm {
		mov			eax, nCount;
		mov			esi, yuyv;
		mov			edi, yuvy;
		mov			edx, eax;
		sub			eax, 0x02;
		shr			eax, 0x02;		// һ�����ĸ�����
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
// ƽ����ֵ��ˮƽ�����Ȩ��ͣ��������� YUV �����ر任Ϊ BGRA/RGBA ��ɫ��
void yuyv_mean_color32_add_x(OUT void* des,			// Ŀ�� 32 ��ɫ��
							 IN int dcw,			// Ŀ����
							 IN void* src,			// ��ʱ�����У����м�Ȩ�õ��Ľ����
							 IN void* pPowerX,		// ˮƽ�����ϵ�Ȩ�ر�
							 IN void* pMapX,		// ˮƽ�����ϵ�ӳ���
							 IN void* pCoefW,		// ˮƽ�����ϵ�����ϵ��
							 IN float* pfRB_YUV,	// YUVY �� RB �任��ϵ����Ry Bu Rv By��
							 IN float* pfG_YUV,		// YUVY �� G �任��ϵ����Gy Gu Gv 0��
							 IN BOOL bBGRA)			// BGRA/RGBA ��ɫ����
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
		movaps		xmm7, dword ptr [eax];	// ����ϵ�� coef coef coef coef
		movaps		xmm5, dword ptr [ebx];	// RB ϵ�� By Rv Bu Ry
		movaps		xmm4, dword ptr [edx];	// G ϵ�� 0 Gv Gu Gy
		shufps		xmm0, xmm1, 0x00;		// 128  128  0  0
		shufps		xmm0, xmm0, 0x28;		// 0  128  128  0
		mov			ebx, pMapX;
		mov			eax, pPowerX;
loop_pixel:
		mov			edx, dword ptr [ebx];	// Ȩ�ظ���
		xorps		xmm6, xmm6;			// ��Ž��
		movlps		xmm2, dword ptr [eax];	// ˮƽ�����ϵ�����βȨ�� @ @ powEnd powStart
		pshufd		xmm3, xmm2, 0x55;	// powerEnd
		shufps		xmm2, xmm2, 0x00;	// powerStart
		add			ebx, 0x04;
		add			eax, 0x08;
seg_first_col:		// ȡ�õ�һ�е�����
		movaps		xmm1, dword ptr [esi];	// Դ��һ������
		mulps		xmm1, xmm2;
		addps		xmm6, xmm1;			// ��Ȩ���
		add			esi, 0x10;
		sub			edx, 0x02;			// �ж�ռ�õ��������
		jb			map_end;			// ֻռһ������
		jnz			loop_mid_col;		// ռ��������������
		jmp			seg_last_col;		// ֻռ��������
loop_mid_col:		// ȡ���м��е�����
		movaps		xmm1, dword ptr [esi];	// Դ��һ������
		addps		xmm6, xmm1;			// ��Ȩ���
		add			esi, 0x10;
		sub			edx, 0x01;
		jnz			loop_mid_col;
seg_last_col:		// ȡ�����һ�е�����
		movaps		xmm1, dword ptr [esi];		// Դ��һ������
		mulps		xmm1, xmm3;
		addps		xmm6, xmm1;			// ��Ȩ���
		add			esi, 0x10;
map_end:		// ��Ȩ����
		mulps		xmm6, xmm7;			// ƽ�� Y V U Y
		subps		xmm6, xmm0;			// ƫ�� UV
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
		mov			edx, bBGRA;			// �ж� BGRA �� RGBA ������
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
		por			xmm1, xmm2;		// ��� Alpha ͨ��
		movd		dword ptr [edi], xmm1;
		sub			esi, 0x10;		// �����������صĹ�����Ҫ����һ�����ء�
		add			edi, 0x04;
		sub			ecx, 0x01;
		jnz			loop_pixel;
	}
}
//
#define decl_align(type, n)			__declspec(align(n)) ##type
void rgb32_swaprb_line_sse2(OUT unsigned char *pucRGB32Out,	// 32 λ������壨�洢��ʽ X2 G X0 A����R �� B ���ܻ�����
							IN unsigned char *pucRGB32In,	// 32 λ���뻺�壨�洢��ʽ X0 G X2 A����R �� B ���ܻ�����
							IN int nWidth)					// һ�����ظ���
{
	decl_align(unsigned int, 16) unMask_02[4] = { 0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff };
	decl_align(unsigned int, 16) unMask_13[4] = { 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00 };
	__asm {
		mov			ecx, nWidth;
		mov			esi, pucRGB32In;
		shr			ecx, 0x04;		// һ�δ��� 16 ������
		mov			edi, pucRGB32Out;
		// Ԥȡһ���ֵ�Դ���ݵ��봦�����Ͻ��� Cache �У����� Cache ��Ⱦ�����������
		prefetchnta	byte ptr [esi];
		prefetchnta	byte ptr [esi + 0x40];
		test		ecx, ecx;
		jnz			loop_p16;
		jmp			loop_m8;
loop_p16:
		// Ԥȡһ���ֵ�Դ���ݵ��봦�����Ͻ��� Cache �У����� Cache ��Ⱦ�����������
		prefetchnta	byte ptr [esi + 0x40];
		movups		xmm0, xmmword ptr [esi];
		movups		xmm1, xmmword ptr [esi + 0x10];
		movups		xmm2, xmmword ptr [esi + 0x20];
		movups		xmm3, xmmword ptr [esi + 0x30];
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm1;
		movaps		xmm6, xmm2;
		movaps		xmm7, xmm3;
		pshuflw		xmm4, xmm4, 0xb1;		// �� 4 ���ֵĽ���
		pshuflw		xmm5, xmm5, 0xb1;
		pshuflw		xmm6, xmm6, 0xb1;
		pshuflw		xmm7, xmm7, 0xb1;
		pshufhw		xmm4, xmm4, 0xb1;		// �� 4 ���ֵĽ���
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
		pshuflw		xmm4, xmm4, 0xb1;		// �� 4 ���ֵĽ���
		pshuflw		xmm5, xmm5, 0xb1;
		pshufhw		xmm4, xmm4, 0xb1;		// �� 4 ���ֵĽ���
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
		pshuflw		xmm4, xmm4, 0xb1;		// �� 4 ���ֵĽ���
		pshufhw		xmm4, xmm4, 0xb1;		// �� 4 ���ֵĽ���
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
		pshuflw		xmm4, xmm4, 0xb1;		// �� 4 ���ֵĽ���
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

// ���� RGBA �� BGRA �� R��B ͨ��
void rgb32_swaprbself(OUT void* rgb32,		// *G@A ���棬��ŷ�ʽ *0 G0 @0 A0 *1 G1 @1 A1 ����
					  IN int pixelcount)	// ���ظ���
{
	//assert(false);
	unsigned char *pucData = static_cast<unsigned char *>(rgb32);
	rgb32_swaprb_line_sse2(pucData,pucData,pixelcount);
}

//////////////////////////////////////////////////////////////////////////
//
// LaiShoubo @ 2009-11-5
//
// ��������A��24λתΪ32λ
//
void color24_a_2_color32(OUT void* color32,			// 32λbuffer
						 IN void* color24,			// 24λbuffer
						 IN void* alpha,			// Aͨ��
						 IN int count)				// ���ظ���
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
		shr			edx, 03h;				// һ�δ���8������
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