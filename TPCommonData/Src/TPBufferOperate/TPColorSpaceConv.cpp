//////////////////////////////////////////////////////////////////////////
// ������	������ɫ�ռ�֮���ת������
// ������	��־�� @ 2007-07-26
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TPColorSpaceConv.h"
#include "TPSSE2.h"
#include <assert.h>

//[[lgq 2009.11.18
//64λ��ʹ���µĿ�
#include "CoreInclude\CPUInclude\NXMemProc.h"
#include "CoreInclude\CPUInclude\NXVideoProc.h"
#include "CoreInclude\CPUInclude\NXColorSpace.h"
//]]

extern LPNXMEMPROCAPI g_lpMemProcAPI;
extern LPNXVIDEOPROCAPI g_lpVideoProcAPI;

//////////////////////////////////////////////////////////////////////////
// ��������

#define PAL_SD_HEIGHT			576			// PAL ��ʽ SD �����
#define SPECIAL_YUYV_VALUE		0x80108010

///////////////////////////////////////////////////////////////////////////////
// YUVA �� RGBA ֮���ת��

// ���嶨�壨HD��
static const float gc_matYUVA2RGBA_HD[][4] = {
    { 1.f,	    0.f,		    1.574722f,	    0.f },
    { 1.f,	    -0.187314f,	    -0.468207f, 	0.f },
    { 1.f,	    1.855615f,	    0.f,	    	0.f },
    { 0.f,	    0.f,	    	0.f,	    	1.f }
};

static const float gc_matRGBA2YUVA_HD[][4] = {
    { 0.212639f,	0.715169f,  	0.072192f,	    0.f },
    { -0.114592f,	-0.385409f,	    0.5f,		    0.f },
    { 0.5f,		    -0.454156f,	    -0.045844f,	    0.f },
    { 0.f,		    0.f,		    0.f,	    	1.f }
};

// ���嶨�壨SD����������Ϊ�����ź��� PAL �� NTSC ��ͬ��
static const float gc_matYUVA2RGBA_SD[][4] = {
    { 1.f,  	0.f,		    1.401978f,	    0.f },
    { 1.f,  	-0.344116f,	    -0.714111f, 	0.f },
    { 1.f,  	1.771973f,  	0.f,		    0.f },
    { 0.f,  	0.f,		    0.f,		    1.f }
};

static const float gc_matRGBA2YUVA_SD[][4] = {
    { 0.29895f, 	0.587036f,	    0.114014f,  	0.f },
    { -0.168701f,	-0.331299f,     0.5f,		    0.f },
    { 0.5f,	    	-0.418701f,	    -0.081299f, 	0.f },
    { 0.f,	    	0.f,		    0.f,		    1.f }
};

///////////////////////////////////////////////////////////////////////////////
// LUVA �� RGBA ֮���ת��

static const float gc_matRGBA2LUVA[][4] = {
    { 0.2988f,      0.5868f,        0.1144f,        0.f },
    { -0.1610824f,	-0.3162388f,	0.4773212f,		0.f },
    { 0.4773212f,	-0.3996969f,	-0.07762426f,	0.f },
    { 0.f,			0.f,			0.f,			1.f }
};

static const float gc_matLUVA2RGBA[][4] = {
    { 1.f,  	0.f,			1.468613f,		0.f },
    { 1.f,	    -0.3604871f,	-0.7480668f,	0.f },
    { 1.f,	    1.856193f,		0.f,			0.f },
    { 0.f,	    0.f,			0.f,			1.f }
};

//////////////////////////////////////////////////////////////////////////
// ������ʽ��ȡת������

float* CALLBACK TPCS_GetMatYUVA2RGBA(IN UINT uDefinition)     // ��ʽ
{
    if (uDefinition == HIGH_DEFINITION)
        return (float*)gc_matYUVA2RGBA_HD;

    if (uDefinition == STANDARD_DEFINITION)
        return (float*)gc_matYUVA2RGBA_SD;

    return NULL;
}

float CALLBACK TPCS_GetMatYUVA2RGBA_Cell(IN int nRow,             // ��
                                         IN int nCol,             // ��
                                         IN UINT uDefinition)     // ��ʽ
{
    assert(nRow >= 0 && nRow < 4 && nCol >= 0 && nCol < 4);

    if (uDefinition == HIGH_DEFINITION)
        return gc_matYUVA2RGBA_HD[nRow][nCol];

    if (uDefinition == STANDARD_DEFINITION)
        return gc_matYUVA2RGBA_SD[nRow][nCol];

    return 0.f;
}

float* CALLBACK TPCS_GetMatRGBA2YUVA(IN UINT uDefinition)     // ��ʽ
{
    if (uDefinition == HIGH_DEFINITION)
        return (float*)gc_matRGBA2YUVA_HD;

    if (uDefinition == STANDARD_DEFINITION)
        return (float*)gc_matRGBA2YUVA_SD;

    return NULL;
}

float CALLBACK TPCS_GetMatRGBA2YUVA_Cell(IN int nRow,             // ��
                                         IN int nCol,             // ��
                                         IN UINT uDefinition)     // ��ʽ
{
    assert(nRow >= 0 && nRow < 4 && nCol >= 0 && nCol < 4);

    if (uDefinition == HIGH_DEFINITION)
        return gc_matRGBA2YUVA_HD[nRow][nCol];

    if (uDefinition == STANDARD_DEFINITION)
        return gc_matRGBA2YUVA_SD[nRow][nCol];

    return 0.f;
}

///////////////////////////////////////////////////////////////////////////////
// LUVA �� RGBA ֮���ת��

float* CALLBACK TPCS_GetMatLUVA2RGBA()
{
    return (float*)gc_matLUVA2RGBA;
}

float* CALLBACK TPCS_GetMatRGBA2LUVA()
{
    return (float*)gc_matRGBA2LUVA;
}

///////////////////////////////////////////////////////////////////////////////

// ����֡��ת��

void TPCS_ComposeFields(OUT BYTE* pDes,             // Ŀ��
                        IN int nDesPitch,
                        IN BYTE* pSrcTop,           // �ϳ�Դ
                        IN int nTopPitch,
                        IN BYTE* pSrcBottom,        // �³�Դ
                        IN int nBottomPitch,
                        IN int nWidth,              // Ŀ���С
                        IN int nHeight,
                        IN int nPixelBytes)         // һ�����صĴ�С
{
    int nRealLen = nWidth * nPixelBytes;

    assert(pDes && pSrcTop && pSrcBottom);
    assert(nTopPitch >= nRealLen && nBottomPitch >= nRealLen && nDesPitch >= nRealLen);

    int na16 = (nRealLen >> 4), nm16 = (nRealLen & 0x0f);
    int nHalfHeight = (nHeight >> 1);

    for (int i=0; i<nHalfHeight; i++)
    {
        // ��һ�����
        memcpy_line(pDes, pSrcTop, na16, nm16);
        pDes += nDesPitch;
        pSrcTop += nTopPitch;
        // �ڶ������
        memcpy_line(pDes, pSrcBottom, na16, nm16);
        pDes += nDesPitch;
        pSrcBottom += nBottomPitch;
    }
}

//wangfengming 2011.4.6 for OpenFX plugins
void TPCvtBGRAandRGBA(OUT unsigned char *pucRGBOutFrame,	// ������壨�洢��ʽ X2 G X0��
					  IN INT_PTR nOutPitch,				// �������һ����ռ�õ��ֽ���
					  IN unsigned char *pucRGBInFrame,	// ���뻺�壨�洢��ʽ X0 G X2��
					  IN INT_PTR nInPitch,				// ���뻺��һ����ռ�õ��ֽ���
					  IN int nWidth,						// Դͼ���ؾ����С
					  IN int nHeight,
					  IN bool bOverturn)
{
#ifdef  __ADAPT_CPU_SDK__
	if(!g_NXColorSpace)
		return;
	else
		g_NXColorSpace->pStuRGB32toOthers->pfnRGB32_SwapRB_Frame(pucRGBOutFrame,  static_cast<intptr_t>(nOutPitch), \
		pucRGBInFrame,  static_cast<intptr_t>(nInPitch), nWidth, nHeight, bOverturn);
#endif
}

void TPCvtRGB32_2_YUVA(OUT unsigned char *pucYUVFrame,		// ������棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
					   IN INT_PTR nYUVPitch,				// �������һ����ռ�õ��ֽ���
					   IN unsigned char *pucRGBFrame,		// ���뻺�壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
					   IN INT_PTR nRGBPitch,				// ���뻺��һ����ռ�õ��ֽ���
					   IN int nWidth,						// Դͼ���ؾ����С
					   IN int nHeight,
					   IN NXVIDEO_DEFINITION eDefinition,	// ������
					   IN bool bBGR,						// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
					   IN bool bOverturn              // ��תͼ���־
					   )
{
#ifdef  __ADAPT_CPU_SDK__
	if(!g_NXColorSpace)
		return;
	else
		g_NXColorSpace->pStuRGB32toOthers->pfnRGB32_2_YUVA_Frame(pucYUVFrame,  static_cast<intptr_t>(nYUVPitch), \
		pucRGBFrame,  static_cast<intptr_t>(nRGBPitch), nWidth, nHeight, eDefinition, bBGR, bOverturn);
#endif
}

void TPCvtYUVA2RGB32(OUT unsigned char *pucRGBFrame,		// ������壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
					 IN INT_PTR nRGBPitch,				// �������һ����ռ�õ��ֽ���
					 IN unsigned char *pucYUVFrame,		// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
					 IN INT_PTR nYUVAPitch,				// ���뻺��һ����ռ�õ��ֽ���
					 IN int nWidth,						// Դͼ���ؾ����С
					 IN int nHeight,
					 IN NXVIDEO_DEFINITION eDefinition,	// ������
					 IN bool bBGR,						// �����������ظ�ʽΪ R G B��B�ڵ�λ���ı�־
					 IN bool bOverturn
					 )
{
#ifdef  __ADAPT_CPU_SDK__
	if(!g_NXColorSpace)
		return;
	else
		g_NXColorSpace->pStuYUVAtoOthers->pfnYUVA_2_RGB32_Frame(pucRGBFrame,  static_cast<intptr_t>(nRGBPitch), \
		pucYUVFrame,  static_cast<intptr_t>(nYUVAPitch), nWidth, nHeight, eDefinition, bBGR, bOverturn);
#endif
}
void TPCvtYUV16_2_YUVA(OUT unsigned char *pucYUVAFrame,	// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
					   IN INT_PTR nYUVAPitch,			// 32 λ����һ����ռ�õ��ֽ���
					   IN unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
					   IN INT_PTR nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
					   IN int nWidth,					// Դͼ���ؾ����С
					   IN int nHeight,
					   IN bool bUYVY,					// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
					   IN unsigned char ucAlpha,			// ���� A ֵ
					   IN bool bOverturn
					   )
{
#ifdef  __ADAPT_CPU_SDK__
	if(!g_NXColorSpace)
		return;
	else
		g_NXColorSpace->pStuYUV16toOthers->pfnYUV16_2_YUVA_Frame(pucYUVAFrame,  static_cast<intptr_t>(nYUVAPitch), \
		pucYUV16Frame,  static_cast<intptr_t>(nYUV16Pitch), nWidth, nHeight, bUYVY, ucAlpha, bOverturn);
#endif
}
void TPCvtYUVA_2_YUV16(OUT unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
					   IN INT_PTR nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
					   IN unsigned char *pucYUVFrame,		// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
					   IN INT_PTR nYUVPitch,				// ����һ����ռ�õ��ֽ���
					   IN int nWidth,						// Դͼ���ؾ����С
					   IN int nHeight,
					   IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
					   IN bool bOverturn
					   )
{
#ifdef  __ADAPT_CPU_SDK__
	if(!g_NXColorSpace)
		return;
	else
		g_NXColorSpace->pStuYUVAtoOthers->pfnYUVA_2_YUV16_Frame(pucYUV16Frame,  static_cast<intptr_t>(nYUV16Pitch), \
		pucYUVFrame,  static_cast<intptr_t>(nYUVPitch), nWidth, nHeight, bUYVY, bOverturn);
#endif
}

void TPCvtRGB24_2_YUYVFrame(OUT unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
							IN INT_PTR nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
							IN unsigned char *pucRGBFrame,		// ���뻺�壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
							IN INT_PTR nRGBPitch,				// ���뻺��һ����ռ�õ��ֽ���
							IN int nWidth,						// Դͼ���ؾ����С
							IN int nHeight,
							IN NXVIDEO_DEFINITION eDefinition,	// ������
							IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
							IN bool bBGR,						// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
							IN bool bOverturn
							)
{
#ifdef  __ADAPT_CPU_SDK__
	if(!g_NXColorSpace)
		return;
	else
		g_NXColorSpace->pStuRGB24toOthers->pfnRGB24_2_YUV16_Frame(pucYUV16Frame,  static_cast<intptr_t>(nYUV16Pitch), \
		pucRGBFrame,  static_cast<intptr_t>(nRGBPitch), nWidth, nHeight, eDefinition, bUYVY, bBGR, bOverturn);
#endif
}

//Directly drag from E7 //wangfengming 2011.4.6
//[[ljj@2010-2-4: YUYV+A->BGRA/RGBA.�����ҪRGBA�����bDstRGBA = TRUE��
BOOL NX_CvtYUYVA2BGRA_Frame(IN OUT BYTE* pBGRA, IN long lDstPitch, IN BYTE* pYUYV, IN long lSrcPitch, IN BYTE* pAlpha, IN long lAlphaPitch,
							IN long lWidth, IN long lHeight, IN BOOL bDstRGBA, IN BOOL bSrcRevert, IN BOOL bAlphaRevert)
{
	if (NULL == pBGRA || NULL == pYUYV || lDstPitch <= 0 || lSrcPitch <= 0 || lHeight <= 0 || lWidth <= 0)
	{
		return FALSE;
	}

	// ��֤Ŀǰ�Ƿ���ֹ� Key �� ��Ƶ��ͼ����ֲ�ͬ������������������Ʋ��ܳ���
	assert(pAlpha && ((bSrcRevert == FALSE) == (bAlphaRevert == FALSE)));

	long lRealSrcPitch = bSrcRevert ? -lSrcPitch : lSrcPitch;
	long lRealDstPitch = lDstPitch;
	long lRealAlphaPitch = bAlphaRevert ? -lAlphaPitch : lAlphaPitch; 
	BYTE* pSrc = bSrcRevert ? pYUYV + lSrcPitch * (lHeight -1) : pYUYV;
	BYTE* pDst = pBGRA;
	BYTE* pA = NULL;
	if (NULL != pAlpha)
	{
		pA = bAlphaRevert ? pAlpha + lAlphaPitch * (lHeight - 1) : pAlpha;
	}
	BOOL bHD = lWidth >= 1280;
	int na16 = (lWidth*2 - 4 ) / 16;
	int nm16 = (lWidth*2 - 4 ) % 16;
	for (long lH = 0; lH < lHeight; lH ++)
	{
		yuyva_to_color32_line(pDst,pSrc,pA,na16,nm16,bHD,!bDstRGBA);
		pDst += lRealDstPitch;
		pSrc += lRealSrcPitch;
		if (NULL != pA)
		{
			pA += lRealAlphaPitch;
		}
	}
	return TRUE;
}
//]]
//wangfengming 2011.4.6

void TPCS_SplitFrame(OUT BYTE* pDesTop,             // Ŀ���ϳ�
                     IN int nTopPitch,
                     OUT BYTE* pDesBottom,          // Ŀ���³�
                     IN int nBottomPitch,
                     IN BYTE* pSrc,                 // Դ֡
                     IN int nSrcPitch,
                     IN int nWidth,                 // Դ��С
                     IN int nHeight,
                     IN int nPixelBytes)            // һ�����صĴ�С
{
    int nRealLen = nWidth * nPixelBytes;

    assert(pDesTop && pSrc && pDesBottom);
    assert(nTopPitch >= nRealLen && nBottomPitch >= nRealLen && nSrcPitch >= nRealLen);

    int na16 = (nRealLen >> 4), nm16 = (nRealLen & 0x0f);
    int nHalfHeight = (nHeight >> 1);

    for (int i=0; i<nHalfHeight; i++)
    {
        // ��һ�����
        memcpy_line(pDesTop, pSrc, na16, nm16);
        pSrc += nSrcPitch;
        pDesTop += nTopPitch;
        // �ڶ������
        memcpy_line(pDesBottom, pSrc, na16, nm16);
        pDesBottom += nBottomPitch;
        pSrc += nSrcPitch;
    }
}

// YUV �ڲ���ʽת�� 420 <-> 422 <-> 444

void TPCS_420_422_YUVFrame(OUT BYTE* pDes,          // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nDesPitch,
                           IN BYTE* pSrcY,          // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
                           IN int nSrcYPitch,
                           IN BYTE* pSrcU,          // U ���루�洢��ʽ UU0 UU2 UU4 �����������й���һ����
                           IN int nSrcUPitch,
                           IN BYTE* pSrcV,          // V ���루�洢��ʽ VV0 VV2 VV4 �����������й���һ����
                           IN int nSrcVPitch,
                           IN int nWidth,           // Դͼ��С
                           IN int nHeight)
{
	g_NXColorSpace->pStuYUV420toOthers->pfnYUV420_2_YUV16_Frame(pDes, nDesPitch, \
		pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, false, false);
	return;

    assert(pDes && pSrcY && pSrcU && pSrcV);
    assert(nDesPitch >= (nWidth << 1) && nSrcYPitch >= nWidth);
    assert(nSrcUPitch >= (nWidth >> 1) && nSrcVPitch >= (nWidth >> 1));

    int nHalfHeight = (nHeight >> 1), nHalfHalf = ((nHalfHeight + 1) >> 1);
    int na16 = (nWidth >> 4), nm16 = (nWidth & 0x0f);
    int nYP2 = (nSrcYPitch << 1), nUP2 = (nSrcUPitch << 1), nVP2 = (nSrcVPitch << 1);
    int nDP2 = (nDesPitch << 1), nDP3 = (nDP2 + nDesPitch), nYP3 = (nYP2 + nSrcYPitch);

    // ǰ����
    // ��һ��
    yuv420_to_yuyv_line_2_F(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
    pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
    // �ڶ���
    yuv420_to_yuyv_line_2_F(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
    pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
    // �м���
    for (int i=2; i<nHalfHalf; i++)
    {
        // ��һ��
        yuv420_to_yuyv_line_2(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
        pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
        // �ڶ���
        yuv420_to_yuyv_line_2(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
        pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
    }
    // ������л�һ�У���Ϊ������ʱ��
    if (nHalfHeight & 0x01)       // һ��
    {
        // ��һ��
        yuv420_to_yuyv_line_1_L(pDes, pSrcY, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
        pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
        // �ڶ���
		if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
		{
			memset_dword_line(pDes, SPECIAL_YUYV_VALUE, (nDesPitch >> 4), (nDesPitch & 0x0f));
		}
		else
		{
			yuv420_to_yuyv_line_1_L(pDes, pSrcY, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		}
    }
    else    // ����
    {
        // ��һ��
        yuv420_to_yuyv_line_2_L(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
        pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
        // �ڶ���
		yuv420_to_yuyv_line_2_L(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
		{
			memset_dword_line((pDes + nDP2), SPECIAL_YUYV_VALUE, (nDesPitch >> 4), (nDesPitch & 0x0f));
		}
    }
}
void TPCS_420_422_YUVSplit(OUT BYTE* pDesFirst,        // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nFirstPitch,
                           OUT BYTE* pDesSecond,       // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nSecondPitch,
                           IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
                           IN int nSrcYPitch,
                           IN BYTE* pSrcU,             // U ���루�洢��ʽ UU0 UU2 UU4 �����������й���һ����
                           IN int nSrcUPitch,
                           IN BYTE* pSrcV,             // V ���루�洢��ʽ VV0 VV2 VV4 �����������й���һ����
                           IN int nSrcVPitch,
                           IN int nWidth,              // Դͼ��С
                           IN int nHeight)
{
	g_NXColorSpace->pStuYUV420toOthers->pfnYUV420_2_YUV16_Split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch, \
		pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, false, false);
	return;

    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert(nFirstPitch >= (nWidth << 1) && nSecondPitch >= (nWidth << 1) && nSrcYPitch >= nWidth);
    assert(nSrcUPitch >= (nWidth >> 1) && nSrcVPitch >= (nWidth >> 1));

    int nHalfHeight = (nHeight >> 1), nHalfHalf = ((nHalfHeight + 1) >> 1);
    int na16 = (nWidth >> 4), nm16 = (nWidth & 0x0f);
    BYTE* pYF = pSrcY, *pYS = pSrcY + nSrcYPitch;
    BYTE* pUF = pSrcU, *pUS = pSrcU + nSrcUPitch;
    BYTE* pVF = pSrcV, *pVS = pSrcV + nSrcVPitch;
    int nYP2 = (nSrcYPitch << 1), nUP2 = (nSrcUPitch << 1), nVP2 = (nSrcVPitch << 1);
    int nDFP2 = (nFirstPitch << 1), nDSP2 = (nSecondPitch << 1), nYP4 = (nYP2 << 1);

    // ǰ����
    // ��һ��
    yuv420_to_yuyv_line_2_F(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
    pDesFirst += nDFP2; pYF += nYP4; pUF += nUP2; pVF += nVP2;
    // �ڶ���
    yuv420_to_yuyv_line_2_F(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
    pDesSecond += nDSP2; pYS += nYP4; pUS += nUP2; pVS += nVP2;
    // �м���
    for (int i=2; i<nHalfHalf; i++)
    {
        // ��һ��
        yuv420_to_yuyv_line_2(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
        pDesFirst += nDFP2; pYF += nYP4; pUF += nUP2; pVF += nVP2;
        // �ڶ���
        yuv420_to_yuyv_line_2(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
        pDesSecond += nDSP2; pYS += nYP4; pUS += nUP2; pVS += nVP2;
    }
    // ������л�һ�У���Ϊ������ʱ��
    if (nHalfHeight & 0x01)       // һ��
    {
        // ��һ��
        yuv420_to_yuyv_line_1_L(pDesFirst, pYF, pUF, nUP2, pVF, nVP2, na16, nm16);
        // �ڶ���
		if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
		{
			memset_dword_line(pDesSecond, SPECIAL_YUYV_VALUE, (nSecondPitch >> 4), (nSecondPitch & 0x0f));
		}
		else
		{
			yuv420_to_yuyv_line_1_L(pDesSecond, pYS, pUS, nUP2, pVS, nVP2, na16, nm16);
		}
    }
    else    // ����
    {
        // ��һ��
        yuv420_to_yuyv_line_2_L(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
        // �ڶ���
		yuv420_to_yuyv_line_2_L(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
		if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
		{
			memset_dword_line((pDesSecond + nSecondPitch), SPECIAL_YUYV_VALUE, (nSecondPitch >> 4), (nSecondPitch & 0x0f));
		}
	}
}
// XDCAM ��ֵ��ʽ
void TPCS_420_422_YUVFrame_XDCAM(OUT BYTE* pDes,          // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
								 IN int nDesPitch,
								 IN BYTE* pSrcY,          // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
								 IN int nSrcYPitch,
								 IN BYTE* pSrcU,          // U ���루�洢��ʽ UU0 UU2 UU4 �����������й���һ����
								 IN int nSrcUPitch,
								 IN BYTE* pSrcV,          // V ���루�洢��ʽ VV0 VV2 VV4 �����������й���һ����
								 IN int nSrcVPitch,
								 IN int nWidth,           // Դͼ��С
								 IN int nHeight)
{
	assert(pDes && pSrcY && pSrcU && pSrcV);
	assert(nDesPitch >= (nWidth << 1) && nSrcYPitch >= nWidth);
	assert(nSrcUPitch >= (nWidth >> 1) && nSrcVPitch >= (nWidth >> 1));

	int nHalfHeight = (nHeight >> 1), nHalfHalf = ((nHalfHeight + 1) >> 1);
	int na16 = (nWidth >> 4), nm16 = (nWidth & 0x0f);
	int nYP2 = (nSrcYPitch << 1), nUP2 = (nSrcUPitch << 1), nVP2 = (nSrcVPitch << 1);
	int nDP2 = (nDesPitch << 1), nDP3 = (nDP2 + nDesPitch), nYP3 = (nYP2 + nSrcYPitch);

	// ǰ����
	// ��һ��
	yuv420_to_yuyv_xdcam_F_line_2_F(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
	pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
	// �ڶ���
	yuv420_to_yuyv_xdcam_S_line_2_F(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
	pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
	// �м���
	for (int i=2; i<nHalfHalf; i++)
	{
		// ��һ��
		yuv420_to_yuyv_xdcam_F_line_2(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		// �ڶ���
		yuv420_to_yuyv_xdcam_S_line_2(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		//////////////////////////////////////////////////////////////////////////
		// ��һ����ڶ����ڴ�ֱ�����Ͼ���ǿ�ҵĶԳ��ԣ���ڶ�����ʹ�õ�һ���ĺ���
		// ֻ��Ҫ�ڵ�һ�������󣬽������ڴ�λ��ָ��ԳƵ�λ�ã�����
		//pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		//yuv420_to_yuyv_xdcam_F_line_2(pDes, -nDP2, pSrcY, -nYP2, pSrcU, -nUP2, pSrcV, -nVP2, na16, nm16);
		//pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
	}
	// ������л�һ�У���Ϊ������ʱ��
	if (nHalfHeight & 0x01)       // һ��
	{
		// ��һ��
		yuv420_to_yuyv_xdcam_F_line_1_L(pDes, pSrcY, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		// �ڶ���
		if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
		{
			memset_dword_line(pDes, SPECIAL_YUYV_VALUE, (nDesPitch >> 4), (nDesPitch & 0x0f));
		}
		else
		{
			yuv420_to_yuyv_xdcam_S_line_1_L(pDes, pSrcY, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		}
	}
	else    // ����
	{
		// ��һ�������öԳ��ԣ�ʹ�õڶ�����ǰ���У�
		pDes += nDP2; pSrcY += nYP2;
		yuv420_to_yuyv_xdcam_S_line_2_F(pDes, -nDP2, pSrcY, -nYP2, pSrcU, -nUP2, pSrcV, -nVP2, na16, nm16);
		pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		// �ڶ��������öԳ��ԣ�ʹ�õ�һ����ǰ���У�
		yuv420_to_yuyv_xdcam_F_line_2_F(pDes, -nDP2, pSrcY, -nYP2, pSrcU, -nUP2, pSrcV, -nVP2, na16, nm16);
		if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
		{
			memset_dword_line((pDes + nDP2), SPECIAL_YUYV_VALUE, (nDesPitch >> 4), (nDesPitch & 0x0f));
		}
	}
}
void TPCS_420_422_YUVSplit_XDCAM(OUT BYTE* pDesFirst,        // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
								 IN int nFirstPitch,
								 OUT BYTE* pDesSecond,       // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
								 IN int nSecondPitch,
								 IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
								 IN int nSrcYPitch,
								 IN BYTE* pSrcU,             // U ���루�洢��ʽ UU0 UU2 UU4 �����������й���һ����
								 IN int nSrcUPitch,
								 IN BYTE* pSrcV,             // V ���루�洢��ʽ VV0 VV2 VV4 �����������й���һ����
								 IN int nSrcVPitch,
								 IN int nWidth,              // Դͼ��С
								 IN int nHeight)
{
	assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
	assert(nFirstPitch >= (nWidth << 1) && nSecondPitch >= (nWidth << 1) && nSrcYPitch >= nWidth);
	assert(nSrcUPitch >= (nWidth >> 1) && nSrcVPitch >= (nWidth >> 1));

	int nHalfHeight = (nHeight >> 1), nHalfHalf = ((nHalfHeight + 1) >> 1);
	int na16 = (nWidth >> 4), nm16 = (nWidth & 0x0f);
	BYTE* pYF = pSrcY, *pYS = pSrcY + nSrcYPitch;
	BYTE* pUF = pSrcU, *pUS = pSrcU + nSrcUPitch;
	BYTE* pVF = pSrcV, *pVS = pSrcV + nSrcVPitch;
	int nYP2 = (nSrcYPitch << 1), nUP2 = (nSrcUPitch << 1), nVP2 = (nSrcVPitch << 1);
	int nDFP2 = (nFirstPitch << 1), nDSP2 = (nSecondPitch << 1), nYP4 = (nYP2 << 1);

	// ǰ����
	// ��һ��
	yuv420_to_yuyv_xdcam_F_line_2_F(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
	pDesFirst += nDFP2; pYF += nYP4; pUF += nUP2; pVF += nVP2;
	// �ڶ���
	yuv420_to_yuyv_xdcam_S_line_2_F(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
	pDesSecond += nDSP2; pYS += nYP4; pUS += nUP2; pVS += nVP2;
	// �м���
	for (int i=2; i<nHalfHalf; i++)
	{
		// ��һ��
		yuv420_to_yuyv_xdcam_F_line_2(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
		pDesFirst += nDFP2; pYF += nYP4; pUF += nUP2; pVF += nVP2;
		// �ڶ���
		yuv420_to_yuyv_xdcam_S_line_2(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
		pDesSecond += nDSP2; pYS += nYP4; pUS += nUP2; pVS += nVP2;
	}
	// ������л�һ�У���Ϊ������ʱ��
	if (nHalfHeight & 0x01)       // һ��
	{
		// ��һ��
		yuv420_to_yuyv_xdcam_F_line_1_L(pDesFirst, pYF, pUF, nUP2, pVF, nVP2, na16, nm16);
		// �ڶ���
		if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
		{
			memset_dword_line(pDesSecond, SPECIAL_YUYV_VALUE, (nSecondPitch >> 4), (nSecondPitch & 0x0f));
		}
		else
		{
			yuv420_to_yuyv_xdcam_S_line_1_L(pDesSecond, pYS, pUS, nUP2, pVS, nVP2, na16, nm16);
		}
	}
	else    // ����
	{
		// ��һ�������öԳ��ԣ�ʹ�õڶ�����ǰ���У�
		pDesFirst += nFirstPitch; pYF += nYP2;
		yuv420_to_yuyv_xdcam_S_line_2_F(pDesFirst, -nFirstPitch, pYF, -nYP2, pUF, -nUP2, pVF, -nVP2, na16, nm16);
		// �ڶ��������öԳ��ԣ�ʹ�õ�һ����ǰ���У�
		pDesSecond += nSecondPitch; pYS += nYP2;
		yuv420_to_yuyv_xdcam_F_line_2_F(pDesSecond, -nSecondPitch, pYS, -nYP2, pUS, -nUP2, pVS, -nVP2, na16, nm16);
		if (nHeight == PAL_SD_HEIGHT)	// ����Ĵ��� DV PAL SD �����һ�У���ڣ�
		{
			memset_dword_line((pDesSecond + nSecondPitch), SPECIAL_YUYV_VALUE, (nSecondPitch >> 4), (nSecondPitch & 0x0f));
		}
	}
}

// ���ó������� -1, 3, 3, -1 ��ϵ���� UV ���в���
void TPCS_422_420_YUVField_4(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
							 IN int nDesYPitch,
							 OUT BYTE* pDesU,			// U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
							 IN int nDesUPitch,
							 OUT BYTE* pDesV,			// V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
							 IN int nDesVPitch,
							 OUT BYTE* pSrcYUYV,		// һ�� YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							 IN int nSrcPitch,
							 IN int nWidth,				// Դͼ��С
							 IN int nFieldHeight)
{
	assert(pDesY != NULL && pDesU != NULL && pDesV != NULL && pSrcYUYV != NULL);
	assert(nDesUPitch >= (nWidth >> 1) && nDesVPitch >= (nWidth >> 1));
	assert(nDesYPitch >= nWidth && nSrcPitch >= (nWidth << 1));
	assert(nFieldHeight >= 4);

	int nHalfHeight = ((nFieldHeight + 1) >> 1);
	int na8 = (nWidth >> 3), nm8 = (nWidth & 0x07);
	int nYUYVP2 = (nSrcPitch << 1), nYP2 = (nDesYPitch << 1);
	BYTE *pSrcYUYV0 = pSrcYUYV, *pSrcYUYV2 = pSrcYUYV + nSrcPitch;

	// ��һ��
	yuyv_to_yuv420_line_4_F(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nSrcPitch, na8, nm8);
	pDesY += nYP2;
	pDesU += nDesUPitch;
	pDesV += nDesVPitch;
	pSrcYUYV0 = pSrcYUYV2;
	pSrcYUYV2 += nYUYVP2;
	// �м���
	for (int i=2; i<nHalfHeight; i++)		// һ�δ�������
	{
		yuyv_to_yuv420_line_4(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nSrcPitch, na8, nm8);
		pDesY += nYP2;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 = pSrcYUYV2;
		pSrcYUYV2 += nYUYVP2;
	}
	// ���һ��
	if ((nFieldHeight & 0x01) != 0)		// �����У�һ�㲻���ܳ���
	{
		yuyv_to_yuv420_line_4_LS(pDesY, pDesU, pDesV, pSrcYUYV0, pSrcYUYV0 + nSrcPitch, na8, nm8);
	}
	else
	{
		yuyv_to_yuv420_line_4_L(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nSrcPitch, na8, nm8);
	}
}
void TPCS_422_420_YUVFrame_4(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
							 IN int nDesYPitch,
							 OUT BYTE* pDesU,			// U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
							 IN int nDesUPitch,
							 OUT BYTE* pDesV,			// V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
							 IN int nDesVPitch,
							 OUT BYTE* pSrcYUYV,		// һ֡ YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							 IN int nSrcPitch,
							 IN int nWidth,				// Դͼ��С
							 IN int nFrameHeight)
{
	assert(pDesY != NULL && pDesU != NULL && pDesV != NULL && pSrcYUYV != NULL);
	assert(nDesUPitch >= (nWidth >> 2) && nDesVPitch >= (nWidth >> 2));
	assert(nDesYPitch >= nWidth && nSrcPitch >= (nWidth << 1));
	assert(nFrameHeight >= 8);

	int nHalfHeight = (nFrameHeight >> 1), nHalfHalfHeight = ((nHalfHeight + 1) >> 1);
	int na8 = (nWidth >> 3), nm8 = (nWidth & 0x07);
	int nYUYVP2 = (nSrcPitch << 1), nYUYVP3 = nSrcPitch + nYUYVP2;
	int nYP2 = (nDesYPitch << 1), nYP3 = nDesYPitch + nYP2;
	BYTE *pSrcYUYV0 = pSrcYUYV, *pSrcYUYV2 = pSrcYUYV + nYUYVP2;

	// ��һ���ĵ�һ��
	yuyv_to_yuv420_line_4_F(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
	pDesY += nDesYPitch;
	pDesU += nDesUPitch;
	pDesV += nDesVPitch;
	pSrcYUYV0 += nSrcPitch;
	pSrcYUYV2 += nSrcPitch;
	// �ڶ����ĵ�һ��
	yuyv_to_yuv420_line_4_F(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
	pDesY += nYP3;
	pDesU += nDesUPitch;
	pDesV += nDesVPitch;
	pSrcYUYV0 += nSrcPitch;
	pSrcYUYV2 += nYUYVP3;
	// �м���
	for (int i=2; i<nHalfHalfHeight; i++)		// һ�δ�������
	{
		// ��һ��������
		yuyv_to_yuv420_line_4(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 += nSrcPitch;
		pSrcYUYV2 += nSrcPitch;
		// �ڶ���������
		yuyv_to_yuv420_line_4(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
		pDesY += nYP3;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 += nYUYVP3;
		pSrcYUYV2 += nYUYVP3;
	}
	// ���һ��
	if ((nHalfHeight & 0x01) != 0)		// �������У�һ�㲻���ܳ���
	{
		// ��һ�����һ��
		yuyv_to_yuv420_line_4_LS(pDesY, pDesU, pDesV, pSrcYUYV0, pSrcYUYV0 + nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 += nSrcPitch;
		// �ڶ������һ��
		yuyv_to_yuv420_line_4_LS(pDesY, pDesU, pDesV, pSrcYUYV0, pSrcYUYV0 + nYUYVP2, na8, nm8);
	}
	else
	{
		// ��һ�����һ��
		yuyv_to_yuv420_line_4_L(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 += nSrcPitch;
		pSrcYUYV2 += nSrcPitch;
		// �ڶ������һ��
		yuyv_to_yuv420_line_4_L(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
	}
}
// ���ó�������ƽ�����˲���ʽ�� UV ���в���
void TPCS_422_420_YUVField(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
						   IN int nDesYPitch,
						   OUT BYTE* pDesU,         // U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
						   IN int nDesUPitch,
						   OUT BYTE* pDesV,         // V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
						   IN int nDesVPitch,
						   OUT BYTE* pSrcYUYV,		// һ�� YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nSrcPitch,
						   IN int nWidth,           // Դͼ��С
						   IN int nFieldHeight)
{
	assert(pDesY != NULL && pDesU != NULL && pDesV != NULL && pSrcYUYV != NULL);
	assert(nDesUPitch >= (nWidth >> 1) && nDesVPitch >= (nWidth >> 1));
	assert(nDesYPitch >= nWidth && nSrcPitch >= (nWidth << 1));

	int nHalfHeight = (nFieldHeight >> 1);
	int na8 = (nWidth >> 3), nm8 = (nWidth & 0x07);
	int nYUYVP2 = (nSrcPitch << 1), nYP2 = (nDesYPitch << 1);
	
	for (int i=0; i<nHalfHeight; i++)		// һ�δ�������
	{
		yuyv_to_yuv420_line_2_avr(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV, nSrcPitch, na8, nm8);
		pDesY += nYP2;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nYUYVP2;
	}
	if ((nFieldHeight & 0x01) != 0)		// �����У����һ��ֱ����䣬һ�㲻���ܳ���
	{
		yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
	}
}
void TPCS_422_420_YUVFrame(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
						   IN int nDesYPitch,
						   OUT BYTE* pDesU,         // U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
						   IN int nDesUPitch,
						   OUT BYTE* pDesV,         // V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
						   IN int nDesVPitch,
						   OUT BYTE* pSrcYUYV,		// һ֡ YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nSrcPitch,
						   IN int nWidth,           // Դͼ��С
						   IN int nFrameHeight)
{
	color_wrapper des_frame;
	des_frame.color = pDesY;
	des_frame.color_pitch = nDesYPitch;
	des_frame.cb = pDesU;
	des_frame.cb_pitch = nDesUPitch;
	des_frame.cr = pDesV;
	des_frame.cr_pitch = nDesVPitch;
	des_frame.pixel_fmt = NXVIDEO_PIXEL_FMT_YUV420;
	color_wrapper src_frame;
	src_frame.color = pSrcYUYV;
	src_frame.color_pitch = nSrcPitch;
	src_frame.pixel_fmt = NXVIDEO_PIXEL_FMT_YUYV;
	g_NXColorSpace->pStuColorGenericAPI->any_to_any_frame(&des_frame,&src_frame,nWidth > 720 ? NXVIDEO_DEFINITION_HIGH:NXVIDEO_DEFINITION_STANDARD,nWidth,nFrameHeight,0);
	return;
	assert(pDesY != NULL && pDesU != NULL && pDesV != NULL && pSrcYUYV != NULL);
	assert(nDesUPitch >= (nWidth >> 2) && nDesVPitch >= (nWidth >> 2));
	assert(nDesYPitch >= nWidth && nSrcPitch >= (nWidth << 1));

	int nHalfHeight = (nFrameHeight >> 1), nHalfHalfHeight = (nHalfHeight >> 1);
	int na8 = (nWidth >> 3), nm8 = (nWidth & 0x07);
	int nYUYVP2 = (nSrcPitch << 1), nYUYVP3 = nSrcPitch + nYUYVP2;
	int nYP2 = (nDesYPitch << 1), nYP3 = nDesYPitch + nYP2;
	
	for (int i=0; i<nHalfHalfHeight; i++)		// һ�δ�������
	{
		// ��һ��������
		yuyv_to_yuv420_line_2_avr(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nSrcPitch;
		// �ڶ���������
		yuyv_to_yuv420_line_2_avr(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		pDesY += nYP3;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nYUYVP3;
	}
	if ((nHalfHeight & 0x01) != 0)		// �������У��ڶ���ֱ�����
	{
		// ��һ�����һ��
		if ((nFrameHeight & 0x01) != 0)		// ֡�����У����һ��Ϊż����
		{
			yuyv_to_yuv420_line_2_avr(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		}
		else
		{
			yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
		}
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nSrcPitch;
		// �ڶ������һ��
		yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
	}
}
// XDCAM ��ֵ��ʽ
void TPCS_422_420_YUVField_XDCAM(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
								 IN int nDesYPitch,
								 OUT BYTE* pDesU,			// U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
								 IN int nDesUPitch,
								 OUT BYTE* pDesV,			// V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
								 IN int nDesVPitch,
								 OUT BYTE* pSrcYUYV,		// һ�� YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
								 IN int nSrcPitch,
								 IN int nWidth,				// Դͼ��С
								 IN int nFieldHeight,
								 IN BOOL bFristField)		// ��ż����־
{
	assert(pDesY != NULL && pDesU != NULL && pDesV != NULL && pSrcYUYV != NULL);
	assert(nDesUPitch >= (nWidth >> 1) && nDesVPitch >= (nWidth >> 1));
	assert(nDesYPitch >= nWidth && nSrcPitch >= (nWidth << 1));

	int nHalfHeight = (nFieldHeight >> 1);
	int na8 = (nWidth >> 3), nm8 = (nWidth & 0x07);
	int nYUYVP2 = (nSrcPitch << 1), nYP2 = (nDesYPitch << 1);

	if (bFristField)
	{
		for (int i=0; i<nHalfHeight; i++)		// һ�δ�������
		{
			yuyv_to_yuv420_xdcam_F_line_2(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV, nSrcPitch, na8, nm8);
			pDesY += nYP2;
			pDesU += nDesUPitch;
			pDesV += nDesVPitch;
			pSrcYUYV += nYUYVP2;
		}
	}
	else
	{
		for (int i=0; i<nHalfHeight; i++)		// һ�δ�������
		{
			yuyv_to_yuv420_xdcam_S_line_2(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV, nSrcPitch, na8, nm8);
			pDesY += nYP2;
			pDesU += nDesUPitch;
			pDesV += nDesVPitch;
			pSrcYUYV += nYUYVP2;
		}
	}
	if ((nFieldHeight & 0x01) != 0)		// �����У����һ��ֱ����䣬һ�㲻���ܳ���
	{
		yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
	}
}
void TPCS_422_420_YUVFrame_XDCAM(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
								 IN int nDesYPitch,
								 OUT BYTE* pDesU,			// U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
								 IN int nDesUPitch,
								 OUT BYTE* pDesV,			// V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
								 IN int nDesVPitch,
								 OUT BYTE* pSrcYUYV,		// һ֡ YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
								 IN int nSrcPitch,
								 IN int nWidth,				// Դͼ��С
								 IN int nFrameHeight)
{
	assert(pDesY != NULL && pDesU != NULL && pDesV != NULL && pSrcYUYV != NULL);
	assert(nDesUPitch >= (nWidth >> 2) && nDesVPitch >= (nWidth >> 2));
	assert(nDesYPitch >= nWidth && nSrcPitch >= (nWidth << 1));

	int nHalfHeight = (nFrameHeight >> 1), nHalfHalfHeight = (nHalfHeight >> 1);
	int na8 = (nWidth >> 3), nm8 = (nWidth & 0x07);
	int nYUYVP2 = (nSrcPitch << 1), nYUYVP3 = nSrcPitch + nYUYVP2;
	int nYP2 = (nDesYPitch << 1), nYP3 = nDesYPitch + nYP2;

	for (int i=0; i<nHalfHalfHeight; i++)		// һ�δ�������
	{
		// ��һ��
		yuyv_to_yuv420_xdcam_F_line_2(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nSrcPitch;
		// �ڶ���
		yuyv_to_yuv420_xdcam_S_line_2(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		pDesY += nYP3;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nYUYVP3;
	}
	if ((nHalfHeight & 0x01) != 0)		// �������У��ڶ���ֱ�����
	{
		// ��һ�����һ��
		if ((nFrameHeight & 0x01) != 0)		// ֡�����У����һ��Ϊż����
		{
			yuyv_to_yuv420_xdcam_F_line_2(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		}
		else
		{
			yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
		}
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nSrcPitch;
		// �ڶ������һ��
		yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
	}
}

void TPCS_ComposeYUV422_Frame(OUT BYTE* pDes,          // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                              IN int nDesPitch,
                              IN BYTE* pSrcY,          // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
                              IN int nSrcYPitch,
                              IN BYTE* pSrcU,          // U ���루�洢��ʽ U0 U2 U4 ������
                              IN int nSrcUPitch,
                              IN BYTE* pSrcV,          // V ���루�洢��ʽ V0 V2 V4 ������
                              IN int nSrcVPitch,
                              IN int nWidth,           // Դͼ��С
                              IN int nHeight)
{
    assert(pDes && pSrcY && pSrcU && pSrcV);
    assert(nDesPitch >= (nWidth << 1) && nSrcYPitch >= nWidth);
    assert(nSrcUPitch >= (nWidth >> 1) && nSrcVPitch >= (nWidth >> 1));
    
    int na16 = (nWidth >> 4), nm16 = (nWidth & 0x0f);

    for (int i=0; i<nHeight; i++)
    {
        yuv422_to_yuyv_line(pDes, pSrcY, pSrcU, pSrcV, na16, nm16);
        pDes += nDesPitch;
        pSrcY += nSrcYPitch;
        pSrcU += nSrcUPitch;
        pSrcV += nSrcVPitch;
    }
}
void TPCS_ComposeYUV422_Split(OUT BYTE* pDesFirst,        // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                              IN int nFirstPitch,
                              OUT BYTE* pDesSecond,       // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                              IN int nSecondPitch,
                              IN BYTE* pSrcY,             // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
                              IN int nSrcYPitch,
                              IN BYTE* pSrcU,             // U ���루�洢��ʽ U0 U2 U4 ������
                              IN int nSrcUPitch,
                              IN BYTE* pSrcV,             // V ���루�洢��ʽ V0 V2 V4 ������
                              IN int nSrcVPitch,
                              IN int nWidth,              // Դͼ��С
                              IN int nHeight)
{
    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert(nFirstPitch >= (nWidth << 1) && nSecondPitch >= (nWidth << 1) && nSrcYPitch >= nWidth);
    assert(nSrcUPitch >= (nWidth >> 1) && nSrcVPitch >= (nWidth >> 1));

    int nHalfHeight = (nHeight >> 1);
    int na16 = (nWidth >> 4), nm16 = (nWidth & 0x0f);

    for (int i=0; i<nHalfHeight; i++)
    {
        yuv422_to_yuyv_line(pDesFirst, pSrcY, pSrcU, pSrcV, na16, nm16);
        pDesFirst += nFirstPitch;
        pSrcY += nSrcYPitch;
        pSrcU += nSrcUPitch;
        pSrcV += nSrcVPitch;
        yuv422_to_yuyv_line(pDesSecond, pSrcY, pSrcU, pSrcV, na16, nm16);
        pDesSecond += nSecondPitch;
        pSrcY += nSrcYPitch;
        pSrcU += nSrcUPitch;
        pSrcV += nSrcVPitch;
    }
    if (nHeight & 0x01)     // ��������ڶ�������
    {
        yuv422_to_yuyv_line(pDesFirst, pSrcY, pSrcU, pSrcV, na16, nm16);
        memcpy_line(pDesSecond, pDesFirst, (nWidth >> 3), ((nWidth & 0x07) << 1));
    }
}

// YUV 420��422 Ƭ�洢��ʽ�� RGBA��BGRA ֮���ת��

void TPCS_YUV420_RGBA_Frame(OUT BYTE* pDesRGBA,         // RGBA ������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
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
                            IN BOOL bDesRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesRGBA && pSrcY && pSrcU && pSrcV);
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth) && (nDesPitch >= (nWidth << 2)));

    yuv420_to_color32_frame(pDesRGBA, nDesPitch, pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, FALSE);
}
void TPCS_YUV420_RGBA_Split(OUT BYTE* pDesFirst,        // RGBA ��һ��������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
                            IN int nFirstPitch,
                            OUT BYTE* pDesSecond,       // RGBA �ڶ���������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
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
                            IN BOOL bDesRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert((nFirstPitch >= (nWidth << 2)) && (nSecondPitch >= (nWidth << 2)));
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth));

    yuv420_to_color32_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch,
        pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, FALSE);
}

void TPCS_YUV422_RGBA_Frame(OUT BYTE* pDesRGBA,         // RGBA ������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
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
                            IN BOOL bDesRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesRGBA && pSrcY && pSrcU && pSrcV);
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth) && (nDesPitch >= (nWidth << 2)));

    yuv422_to_color32_frame(pDesRGBA, nDesPitch, pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, FALSE);
}
void TPCS_YUV422_RGBA_Split(OUT BYTE* pDesFirst,        // RGBA ��һ��������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
                            IN int nFirstPitch,
                            OUT BYTE* pDesSecond,       // RGBA �ڶ���������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
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
                            IN BOOL bDesRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert((nFirstPitch >= (nWidth << 2)) && (nSecondPitch >= (nWidth << 2)));
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth));

    yuv422_to_color32_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch,
        pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, FALSE);
}

void TPCS_YUV420_BGRA_Frame(OUT BYTE* pDesBGRA,         // BGRA ������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
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
                            IN BOOL bDesRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesBGRA && pSrcY && pSrcU && pSrcV);
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth) && (nDesPitch >= (nWidth << 2)));

    yuv420_to_color32_frame(pDesBGRA, nDesPitch, pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, TRUE);
}
void TPCS_YUV420_BGRA_Split(OUT BYTE* pDesFirst,        // BGRA ��һ��������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
                            IN int nFirstPitch,
                            OUT BYTE* pDesSecond,       // RGBA �ڶ���������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
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
                            IN BOOL bDesRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert((nFirstPitch >= (nWidth << 2)) && (nSecondPitch >= (nWidth << 2)));
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth));

    yuv420_to_color32_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch,
        pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, TRUE);
}

void TPCS_YUV422_BGRA_Frame(OUT BYTE* pDesBGRA,         // BGRA ������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
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
                            IN BOOL bDesRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesBGRA && pSrcY && pSrcU && pSrcV);
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth) && (nDesPitch >= (nWidth << 2)));

    yuv422_to_color32_frame(pDesBGRA, nDesPitch, pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, TRUE);
}
void TPCS_YUV422_BGRA_Split(OUT BYTE* pDesFirst,        // BGRA ��һ��������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
                            IN int nFirstPitch,
                            OUT BYTE* pDesSecond,       // RGBA �ڶ���������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
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
                            IN BOOL bDesRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert((nFirstPitch >= (nWidth << 2)) && (nSecondPitch >= (nWidth << 2)));
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth));

    yuv422_to_color32_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch,
        pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, TRUE);
}

// YUYVA �� RGBA��BGRA ֮���ת��

void  TPCS_RGBA2YUYVA_Frame(OUT BYTE* pDesYUYV,         // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nYUYVPitch,
                            OUT BYTE* pDesAlpha,        // Alpha ������洢��ʽ A0 A1 A2 A3 A4 ������
                            IN int nAlphaPitch,
                            IN BYTE* pSrcRGBA,          // RGBA ���루�洢��ʽ R G B A R G B A R G B A ������
                            IN int nSrcPitch,           // Դһ�е��ֽڿ��
                            IN int nWidth,			    // Դͼ��С
                            IN int nHeight,
                            IN BOOL bHD,                // �߱����־
                            IN BOOL bSrcRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYV && pDesAlpha && pSrcRGBA);
    assert((nYUYVPitch >= (nWidth << 1)) && (nAlphaPitch >= nWidth) && (nSrcPitch >= (nWidth << 2)));

    color32_to_yuyva_frame(pDesYUYV, nYUYVPitch, pDesAlpha, nAlphaPitch, pSrcRGBA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}
void  TPCS_RGBA2YUYVA_Split(OUT BYTE* pDesYUYVF,        // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nYUYVPitchF,
                            OUT BYTE* pDesAlphaF,       // Alpha ��һ��������洢��ʽ A0 A1 A2 A3 A4 ������
                            IN int nAlphaPitchF,
                            OUT BYTE* pDesYUYVS,        // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nYUYVPitchS,
                            OUT BYTE* pDesAlphaS,       // Alpha �ڶ���������洢��ʽ A0 A1 A2 A3 A4 ������
                            IN int nAlphaPitchS,
                            IN BYTE* pSrcRGBA,          // RGBA ���루�洢��ʽ R G B A R G B A R G B A ������
                            IN int nSrcPitch,           // Դһ�е��ֽڿ��
                            IN int nWidth,              // Դͼ��С
                            IN int nHeight,
                            IN BOOL bHD,                // �߱����־
                            IN BOOL bSrcRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYVF && pDesAlphaF && pDesAlphaS && pDesYUYVS && pSrcRGBA);
    assert(nYUYVPitchF >= (nWidth << 1) && nAlphaPitchF >= nWidth && nYUYVPitchS >= (nWidth << 1) && nAlphaPitchS >= nWidth);
    assert(nSrcPitch >= (nWidth << 2));

    color32_to_yuyva_split(pDesYUYVF, nYUYVPitchF, pDesAlphaF, nAlphaPitchF, pDesYUYVS, nYUYVPitchS, pDesAlphaS, nAlphaPitchS,
        pSrcRGBA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}
void  TPCS_RGBA2YUYV_Frame(OUT BYTE* pDesYUYV,          // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitch,
                           IN BYTE* pSrcRGBA,           // RGBA ���루�洢��ʽ R G B A R G B A R G B A ������
                           IN int nSrcPitch,            // Դһ�е��ֽڿ��
                           IN int nWidth,			    // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                 // �߱����־
                           IN BOOL bSrcRevert)          // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYV && pSrcRGBA);
    assert(nYUYVPitch >= (nWidth << 1) && nSrcPitch >= (nWidth << 2));

    color32_to_yuyv_frame(pDesYUYV, nYUYVPitch, pSrcRGBA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}
void  TPCS_RGBA2YUYV_Split(OUT BYTE* pDesYUYVF,         // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesYUYVS,         // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchS,
                           IN BYTE* pSrcRGBA,           // RGBA ���루�洢��ʽ R G B A R G B A R G B A ������
                           IN int nSrcPitch,            // Դһ�е��ֽڿ��
                           IN int nWidth,               // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                 // �߱����־
                           IN BOOL bSrcRevert)          // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYVF && pDesYUYVS && pSrcRGBA);
    assert(nYUYVPitchF >= (nWidth << 1) && nYUYVPitchS >= (nWidth << 1) && nSrcPitch >= (nWidth << 2));

    color32_to_yuyv_split(pDesYUYVF, nYUYVPitchF, pDesYUYVS, nYUYVPitchS, pSrcRGBA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}

void TPCS_YUYV2RGBA_Compose(OUT BYTE* pDesRGBA,         // RGBA ������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
                            IN int nDesPitch,
                            IN BYTE* pSrcYUYVF,         // YUYV ��һ�����루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nFirstPitch,
                            IN BYTE* pSrcYUYVS,         // YUYV �ڶ������루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nSecondPitch,
                            IN int nWidth,              // Ŀ���С
                            IN int nHeight,
                            IN BOOL bHD,                // �߱����־
							IN BOOL bDstRevert)			// ͼ���Ƿ��з�ת��־

{
    assert(pDesRGBA && pSrcYUYVF && pSrcYUYVS);
    assert((nDesPitch >= (nWidth << 2)) && (nFirstPitch >= (nWidth << 1)) && (nSecondPitch >= (nWidth << 1)));

    yuyv_to_color32_compose(pDesRGBA, nDesPitch, pSrcYUYVF, nFirstPitch, pSrcYUYVS, nSecondPitch, nWidth, nHeight, bHD, FALSE, bDstRevert);
}


void  TPCS_BGRA2YUYVA_Frame(OUT BYTE* pDesYUYV,         // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nYUYVPitch,
                            OUT BYTE* pDesAlpha,        // Alpha ������洢��ʽ A0 A1 A2 A3 A4 ������
                            IN int nAlphaPitch,
                            IN BYTE* pSrcBGRA,          // BGRA ���루�洢��ʽ B G R A B G R A B G R A ������
                            IN int nSrcPitch,           // Դһ�е��ֽڿ��
                            IN int nWidth,			    // Դͼ��С
                            IN int nHeight,
                            IN BOOL bHD,                // �߱����־
                            IN BOOL bSrcRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYV && pDesAlpha && pSrcBGRA);
    assert((nYUYVPitch >= (nWidth << 1)) && (nAlphaPitch >= nWidth) && (nSrcPitch >= (nWidth << 2)));

    color32_to_yuyva_frame(pDesYUYV, nYUYVPitch, pDesAlpha, nAlphaPitch, pSrcBGRA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}
void  TPCS_BGRA2YUYVA_Split(OUT BYTE* pDesYUYVF,        // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nYUYVPitchF,
                            OUT BYTE* pDesAlphaF,       // Alpha ��һ��������洢��ʽ A0 A1 A2 A3 A4 ������
                            IN int nAlphaPitchF,
                            OUT BYTE* pDesYUYVS,        // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nYUYVPitchS,
                            OUT BYTE* pDesAlphaS,       // Alpha �ڶ���������洢��ʽ A0 A1 A2 A3 A4 ������
                            IN int nAlphaPitchS,
                            IN BYTE* pSrcBGRA,          // BGRA ���루�洢��ʽ B G R A B G R A B G R A ������
                            IN int nSrcPitch,           // Դһ�е��ֽڿ��
                            IN int nWidth,			    // Դͼ��С
                            IN int nHeight,
                            IN BOOL bHD,                // �߱����־
                            IN BOOL bSrcRevert)         // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYVF && pDesAlphaF && pDesAlphaS && pDesYUYVS && pSrcBGRA);
    assert(nYUYVPitchF >= (nWidth << 1) && nAlphaPitchF >= nWidth && nYUYVPitchS >= (nWidth << 1) && nAlphaPitchS >= nWidth);
    assert(nSrcPitch >= (nWidth << 2));

    color32_to_yuyva_split(pDesYUYVF, nYUYVPitchF, pDesAlphaF, nAlphaPitchF, pDesYUYVS, nYUYVPitchS, pDesAlphaS, nAlphaPitchS,
        pSrcBGRA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}
void  TPCS_BGRA2YUYV_Frame(OUT BYTE* pDesYUYV,          // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitch,
                           IN BYTE* pSrcBGRA,           // BGRA ���루�洢��ʽ B G R A B G R A B G R A ������
                           IN int nSrcPitch,            // Դһ�е��ֽڿ��
                           IN int nWidth,			    // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                 // �߱����־
                           IN BOOL bSrcRevert)          // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYV && pSrcBGRA);
    assert(nYUYVPitch >= (nWidth << 1) && nSrcPitch >= (nWidth << 2));
	g_NXColorSpace->pStuRGB32toOthers->pfnRGB32_2_YUV16_Frame(pDesYUYV,  static_cast<intptr_t>(nYUYVPitch), \
		pSrcBGRA,  static_cast<intptr_t>(nSrcPitch), nWidth, nHeight,bHD ? NXVIDEO_DEFINITION_HIGH :NXVIDEO_DEFINITION_STANDARD ,0,1,bSrcRevert);

    //color32_to_yuyv_frame(pDesYUYV, nYUYVPitch, pSrcBGRA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}
void  TPCS_BGRA2YUYV_Split(OUT BYTE* pDesYUYVF,         // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesYUYVS,         // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchS,
                           IN BYTE* pSrcBGRA,           // BGRA ���루�洢��ʽ B G R A B G R A B G R A ������
                           IN int nSrcPitch,            // Դһ�е��ֽڿ��
                           IN int nWidth,               // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                 // �߱����־
                           IN BOOL bSrcRevert)          // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYVF && pDesYUYVS && pSrcBGRA);
    assert(nYUYVPitchF >= (nWidth << 1) && nYUYVPitchS >= (nWidth << 1) && nSrcPitch >= (nWidth << 2));

    color32_to_yuyv_split(pDesYUYVF, nYUYVPitchF, pDesYUYVS, nYUYVPitchS, pSrcBGRA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}

void TPCS_YUYV2BGRA_Compose(OUT BYTE* pDesBGRA,         // BGRA ������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
                            IN int nDesPitch,
                            IN BYTE* pSrcYUYVF,         // YUYV ��һ�����루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nFirstPitch,
                            IN BYTE* pSrcYUYVS,         // YUYV �ڶ������루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                            IN int nSecondPitch,
                            IN int nWidth,              // Ŀ���С
                            IN int nHeight,
                            IN BOOL bHD,                // �߱����־
							IN BOOL bDstRevert)			// ͼ���Ƿ��з�ת��־
{
    assert(pDesBGRA && pSrcYUYVF && pSrcYUYVS);
    assert((nDesPitch >= (nWidth << 2)) && (nFirstPitch >= (nWidth << 1)) && (nSecondPitch >= (nWidth << 1)));

    yuyv_to_color32_compose(pDesBGRA, nDesPitch, pSrcYUYVF, nFirstPitch, pSrcYUYVS, nSecondPitch, nWidth, nHeight, bHD, TRUE, bDstRevert);
}
void TPCS_YUYV2BGRA_Frame(OUT BYTE* pDesBGRA,         // BGRA ������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
						  IN int nDesPitch,
						  IN BYTE* pSrcYUYV,         // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						  IN int nSrcPitch,
						  IN int nWidth,              // Ŀ���С
						  IN int nHeight,
						  IN BOOL bHD,                // �߱����־
						  IN BOOL bDstRevert)			// Ŀ���Ƿ�ת
{
	assert(pDesBGRA && pSrcYUYV );
	assert((nDesPitch >= (nWidth << 2)) && (nSrcPitch >= (nWidth << 1)));
	g_NXColorSpace->pStuYUV16toOthers->pfnYUV16_2_RGB32_Frame(pDesBGRA,  static_cast<intptr_t>(nDesPitch), \
		pSrcYUYV,  static_cast<intptr_t>(nSrcPitch), nWidth, nHeight, bHD ? NXVIDEO_DEFINITION_HIGH :NXVIDEO_DEFINITION_STANDARD,1,0,255,bDstRevert);

	//yuyv_to_color32_frame(pDesBGRA, nDesPitch, pSrcYUYV, nSrcPitch,  nWidth, nHeight, bHD, TRUE, bDstRevert);
}

void TPCS_YUYV2BGR_Frame(OUT BYTE* pDesBGR,         // BGR ������洢��ʽ B G R B G R B G R ������
						 IN int nDesPitch,
						 IN BYTE* pSrcYUYV,         // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						 IN int nSrcPitch,
						 IN int nWidth,              // Ŀ���С
						 IN int nHeight,
						 IN BOOL bHD,                // �߱����־
						 IN BOOL bDstRevert)		// ͼ���Ƿ��з�ת��־
{
	assert(pDesBGR && pSrcYUYV );
	assert((nDesPitch >= (nWidth*3)) && (nSrcPitch >= (nWidth << 1)));

	yuyv_to_color24_frame(pDesBGR, nDesPitch, pSrcYUYV, nSrcPitch,  nWidth, nHeight, bHD, TRUE, bDstRevert);
}
void TPCS_YUYV2RGB_Frame(OUT BYTE* pDesRGB,         // BGR ������洢��ʽ R G B R G B R G B������
						 IN int nDesPitch,
						 IN BYTE* pSrcYUYV,         // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						 IN int nSrcPitch,
						 IN int nWidth,              // Ŀ���С
						 IN int nHeight,
						 IN BOOL bHD,                // �߱����־
						 IN BOOL bDstRevert)		// ͼ���Ƿ��з�ת��־
{
	assert(pDesRGB && pSrcYUYV );
	assert((nDesPitch >= (nWidth*3)) && (nSrcPitch >= (nWidth << 1)));

	yuyv_to_color24_frame(pDesRGB, nDesPitch, pSrcYUYV, nSrcPitch,  nWidth, nHeight, bHD, FALSE, bDstRevert);
}

void TPCS_YUYV2RGBA_Frame(OUT BYTE* pDesRGBA,         // RGBA ������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
							IN int nDesPitch,
							IN BYTE* pSrcYUYV,         // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							IN int nSrcPitch,
							IN int nWidth,              // Ŀ���С
							IN int nHeight,
							IN BOOL bHD,                // �߱����־
							IN BOOL bDstRevert)			// ͼ���Ƿ��з�ת��־
{
	assert(pDesRGBA && pSrcYUYV );
	assert((nDesPitch >= (nWidth << 2)) && (nSrcPitch >= (nWidth << 1)));

	yuyv_to_color32_frame(pDesRGBA, nDesPitch, pSrcYUYV, nSrcPitch,  nWidth, nHeight, bHD, FALSE, bDstRevert);
}

void TPCS_YUYV2BGRA_MeanStretch(OUT BYTE* pDesBGRA,		// BGRA ������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
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
								IN BOOL bSrcHD,			// Դ�źŵĸ߱����־������ʹ����ɫ�任�Ĺ�ʽ��
								IN BOOL bDesRevert)		// ͼ���Ƿ��з�ת��־
{
	assert(pDesBGRA && pSrcYUYV);
	assert((nDesPitch >= (nDesImgWidth << 2)) && (nSrcPitch >= (nSrcImgWidth << 1)));

	yuyv_to_color32_meanstretch(pDesBGRA, nDesPitch, nDesX, nDesY, nDesWidth, nDesHeight, nDesImgWidth, nDesImgHeight,
		pSrcYUYV, nSrcPitch, nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcImgWidth, nSrcImgHeight, bSrcHD, TRUE, bDesRevert);
}

void TPCS_YUYV2RGBA_MeanStretch(OUT BYTE* pDesRGBA,		// RGBA ������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
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
								IN BOOL bSrcHD,			// Դ�źŵĸ߱����־������ʹ����ɫ�任�Ĺ�ʽ��
								IN BOOL bDesRevert)		// ͼ���Ƿ��з�ת��־
{
	assert(pDesRGBA && pSrcYUYV);
	assert((nDesPitch >= (nDesImgWidth << 2)) && (nSrcPitch >= (nSrcImgWidth << 1)));
	assert((nDesImgWidth >= 0) && (nDesImgHeight >= 0) && (nSrcImgWidth >= 0) && (nSrcImgHeight >= 0));

	yuyv_to_color32_meanstretch(pDesRGBA, nDesPitch, nDesX, nDesY, nDesWidth, nDesHeight, nDesImgWidth, nDesImgHeight,
		pSrcYUYV, nSrcPitch, nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcImgWidth, nSrcImgHeight, bSrcHD, FALSE, bDesRevert);
}

// YUYV �� RGB��BGR ֮���ת��

void  TPCS_RGB2YUYV_Frame(OUT BYTE* pDesYUYV,           // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nYUYVPitch,
                          IN BYTE* pSrcRGB,             // RGB ���루�洢��ʽ R G B R G B R G B ������
                          IN int nSrcPitch,             // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                          IN int nWidth,			    // Դͼ��С
                          IN int nHeight,
                          IN BOOL bHD,                  // �߱����־
                          IN BOOL bSrcRevert)           // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYV && pSrcRGB);
    assert(nYUYVPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSrcPitch >= (nWidth * 3));

    color24_to_yuyv_frame(pDesYUYV, nYUYVPitch, pSrcRGB, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}
void  TPCS_RGB2YUYV_Split(OUT BYTE* pDesFirst,          // ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nFirstPitch,
                          OUT BYTE* pDesSecond,         // �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nSecondPitch,
                          IN BYTE* pSrcRGB,             // RGB ���루�洢��ʽ R G B R G B R G B ������
                          IN int nSrcPitch,             // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                          IN int nWidth,			    // Դͼ��С
                          IN int nHeight,
                          IN BOOL bHD,                  // �߱����־
                          IN BOOL bSrcRevert)           // ͼ���Ƿ��з�ת��־
{
    assert(pDesFirst && pDesSecond && pSrcRGB);
    assert(nFirstPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSecondPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSrcPitch >= (nWidth * 3));

    color24_to_yuyv_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch, pSrcRGB, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}

void  TPCS_BGR2YUYV_Frame(OUT BYTE* pDesYUYV,           // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nYUYVPitch,
                          IN BYTE* pSrcBGR,             // BGR ���루�洢��ʽ B G R B G R B G R ������
                          IN int nSrcPitch,             // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                          IN int nWidth,			    // Դͼ��С
                          IN int nHeight,
                          IN BOOL bHD,                  // �߱����־
                          IN BOOL bSrcRevert)           // ͼ���Ƿ��з�ת��־
{
    assert(pDesYUYV && pSrcBGR);
    assert(nYUYVPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSrcPitch >= (nWidth * 3));

    color24_to_yuyv_frame(pDesYUYV, nYUYVPitch, pSrcBGR, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}
void  TPCS_BGR2YUYV_Split(OUT BYTE* pDesFirst,          // ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nFirstPitch,
                          OUT BYTE* pDesSecond,         // �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nSecondPitch,
                          IN BYTE* pSrcBGR,             // BGR ���루�洢��ʽ B G R B G R B G R ������
                          IN int nSrcPitch,             // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                          IN int nWidth,			    // Դͼ��С
                          IN int nHeight,
                          IN BOOL bHD,                  // �߱����־
                          IN BOOL bSrcRevert)           // ͼ���Ƿ��з�ת��־
{
    assert(pDesFirst && pDesSecond && pSrcBGR);
    assert(nFirstPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSecondPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSrcPitch >= (nWidth * 3));

    color24_to_yuyv_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch, pSrcBGR, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}

// RGB Alpha �� RGBA��BGR Alpha �� BGRA ֮���ת��

void TPCS_24_to_32_Frame(OUT BYTE* pDes32,				// ������洢��ʽ X0 X1 X2 A X0 X1 X2 A ������
						 IN int nDesPitch,
						 IN BYTE* pSrc24,               // 24 λԴ���루�洢��ʽ X0 X1 X2 X0 X1 X2 ������
						 IN int nSrc24Pitch,            // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
						 IN BYTE* pSrcAlpha,            // Դ Alpha ���루�洢��ʽ A0 A1 A2 A3 ���������������Ϊ NULL ��ֱ���� 0xff��
						 IN int nSrcAlphaPitch,
						 IN int nWidth,                 // Դͼ��С
						 IN int nHeight,
						 IN BOOL bSrc24Revert,          // ͼ���Ƿ��з�ת��־
						 IN BOOL bSrcAlphaRevert)       // Alpha �Ƿ��з�ת��־
{
	assert(pDes32 && pSrc24);
	assert(nDesPitch >= ((nWidth + (nWidth & 0x01)) << 1));
	assert(nSrc24Pitch >= (nWidth * 3));
	assert(pSrcAlpha == NULL || nSrcAlphaPitch >= nWidth);

	int nap5 = (nWidth / 5), nmp5 = (nWidth - 5 * nap5);
	BYTE* pSrc24Line = (bSrc24Revert) ? (pSrc24 + (nHeight - 1) * nSrc24Pitch) : pSrc24;
	int nRealSrc24Pitch = (bSrc24Revert) ? -nSrc24Pitch : nSrc24Pitch;

	if (pSrcAlpha)      // �� Alpha ����
	{
		BYTE* pSrcAlphaLine = (bSrcAlphaRevert) ? (pSrcAlpha + (nHeight - 1) * nSrcAlphaPitch) : pSrcAlpha;
		int nRealSrcAlphaPitch = (bSrcAlphaRevert) ? -nSrcAlphaPitch : nSrcAlphaPitch;
		for(int i = 0; i < nHeight; ++i)
		{
			color24_a_2_color32(pDes32, pSrc24Line, pSrcAlphaLine, nWidth);
			pDes32 += nDesPitch;
			pSrc24Line += nRealSrc24Pitch;
			pSrcAlphaLine += nRealSrcAlphaPitch;
		}
	}
	else    // �� Alpha ������ֱ����� 0xff
	{
		for (int i=0; i<nHeight; i++)
		{
			color24_to_color32(pDes32, pSrc24Line, nap5, nmp5);
			pSrc24Line += nRealSrc24Pitch;
			pDes32 += nDesPitch;
		}
	}
}
void TPCS_24_to_32_Split(OUT BYTE* pDes32First,         // ��һ��������洢��ʽ X0 X1 X2 A X0 X1 X2 A ������
                         IN int nFirstPitch,
                         OUT BYTE* pDes32Second,        // �ڶ���������洢��ʽ X0 X1 X2 A X0 X1 X2 A ������
                         IN int nSecondPitch,
                         IN BYTE* pSrc24,               // 24 λԴ���루�洢��ʽ X0 X1 X2 X0 X1 X2 ������
                         IN int nSrc24Pitch,            // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                         IN BYTE* pSrcAlpha,            // Դ Alpha ���루�洢��ʽ A0 A1 A2 A3 ���������������Ϊ NULL ��ֱ���� 0xff��
                         IN int nSrcAlphaPitch,
                         IN int nWidth,                 // Դͼ��С
                         IN int nHeight,
                         IN BOOL bSrc24Revert,          // ͼ���Ƿ��з�ת��־
                         IN BOOL bSrcAlphaRevert)       // Alpha �Ƿ��з�ת��־
{
    assert(pDes32First && pDes32Second && pSrc24);
    assert(nFirstPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSecondPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSrc24Pitch >= (nWidth * 3));
    assert(pSrcAlpha == NULL || nSrcAlphaPitch >= nWidth);

    int nap5 = (nWidth / 5), nmp5 = (nWidth - 5 * nap5);
    int nHalfHeight = (nHeight >> 1);
    BYTE* pSrc24Line = (bSrc24Revert) ? (pSrc24 + (nHeight - 1) * nSrc24Pitch) : pSrc24;
    int nRealSrc24Pitch = (bSrc24Revert) ? -nSrc24Pitch : nSrc24Pitch;

    if (pSrcAlpha)      // �� Alpha ����
    {
        BYTE* pSrcAlphaLine = (bSrcAlphaRevert) ? (pSrcAlpha + (nHeight - 1) * nSrcAlphaPitch) : pSrcAlpha;
        int nRealSrcAlphaPitch = (bSrcAlphaRevert) ? -nSrcAlphaPitch : nSrcAlphaPitch;
		for(int i = 0; i < nHalfHeight; ++i)
		{
			color24_a_2_color32(pDes32First, pSrc24Line, pSrcAlphaLine, nWidth);
			pDes32First += nFirstPitch;
			pSrc24Line += nRealSrc24Pitch;
			pSrcAlphaLine += nRealSrcAlphaPitch;
			color24_a_2_color32(pDes32Second, pSrc24Line, pSrcAlphaLine, nWidth);
			pDes32Second += nSecondPitch;
			pSrc24Line += nRealSrc24Pitch;
			pSrcAlphaLine += nRealSrcAlphaPitch;
		}
		if(nHeight & 1)
		{
			color24_a_2_color32(pDes32First, pSrc24Line, pSrcAlphaLine, nWidth);
			nSecondPitch = min(nFirstPitch, nSecondPitch);
			nap5 = (nSecondPitch >> 4), nmp5 = (nSecondPitch & 0x0f);
			memcpy_line(pDes32Second, pDes32First, nap5, nmp5);
		}
    }
    else    // �� Alpha ������ֱ����� 0xff
    {
        for (int i=0; i<nHalfHeight; i++)
        {
            // ��һ��
            color24_to_color32(pDes32First, pSrc24Line, nap5, nmp5);
            pSrc24Line += nRealSrc24Pitch;
            pDes32First += nFirstPitch;
            // �ڶ���
            color24_to_color32(pDes32Second, pSrc24Line, nap5, nmp5);
            pSrc24Line += nRealSrc24Pitch;
            pDes32Second += nSecondPitch;
        }
        if (nHeight & 0x01)     // ���Ϊ�����У���Ҫ���⴦����һ���վɣ��ڶ�����ڻ򿽱�
        {
            // ��һ��
            color24_to_color32(pDes32First, pSrc24Line, nap5, nmp5);
            // �ڶ���
            nSecondPitch = min(nFirstPitch, nSecondPitch);      // ������
            nap5 = (nSecondPitch >> 4), nmp5 = (nSecondPitch & 0x0f);
            //memset_dword_line(pDes32Second, CS_RGBA_BLACK_VALUE, na12, nmp);
            memcpy_line(pDes32Second, pDes32First, nap5, nmp5);
        }
    }
}

void TPCS_RGB32_SwapRBSelf(IN OUT unsigned char *pucRGB32,	// RGB32 ����������洢��ʽ X0 X1 X2 A X0 X1 X2 A ��������R��B �ɽ�����
						   IN INT_PTR nRGB32Pitch,			// һ����ռ�ֽڿ��
						   IN int nWidth,					// Դͼ���С
						   IN int nHeight)
{
	//assert(false);
	assert(pucRGB32);
	for (int i=0; i<nHeight; i++)
	{
		rgb32_swaprbself(pucRGB32, nWidth);
		pucRGB32 += nRGB32Pitch;
	}
}

// 24 λ RGB/BGR ��ɫ���� R��B ͨ��
void TPCS_RGB24_SwapRBFrame(OUT unsigned char *pucRGBOutFrame,	// ������壨�洢��ʽ X2 G X0��
						   IN intptr_t nOutPitch,				// �������һ����ռ�õ��ֽ���
						   IN unsigned char *pucRGBInFrame,	// ���뻺�壨�洢��ʽ X0 G X2��
						   IN intptr_t nInPitch,				// ���뻺��һ����ռ�õ��ֽ���
						   IN int nWidth,						// Դͼ���ؾ����С
						   IN int nHeight,
						   IN bool bOverturn)
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnRGB24_SwapRB_Frame(pucRGBOutFrame, nOutPitch, pucRGBInFrame, nInPitch, nWidth, nHeight, bOverturn);
	}
}
// 32 λ RGBA/BGRA/YUVA ��ɫ�� 24 λ RGB/BGR/YUV ��ɫ�ı任��ֱ�Ӷ��� A
void TPCS_32_to_24_Frame(OUT unsigned char *pucCr24Frame,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
						 IN INT_PTR nCr24Pitch,				// 24 λ����һ����ռ�õ��ֽ���
						 IN unsigned char *pucCr32Frame,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
						 IN INT_PTR nCr32Pitch,				// 32 λ����һ����ռ�õ��ֽ���
						 IN int nWidth,						// Դͼ���ؾ����С
						 IN int nHeight,
						 IN bool bExchangeRB,				// ���� 0��2 �ֽڣ�R��B ͨ�����ı�־
						 IN bool bOverturn)					// ��תͼ���־
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnCr32_2_Cr24_Frame(pucCr24Frame, nCr24Pitch, pucCr32Frame, nCr32Pitch, nWidth, nHeight, bExchangeRB, bOverturn);
	}
}

// 16 λ YUYV/UYVY ��ɫ�� 32 λ BGRA/RGBA ��ɫ�ı任��A �����Ϊͳһֵ
void TPCS_YUV16_2_RGB32_Frame(OUT unsigned char *pucRGB32Frame,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
							  IN intptr_t nRGB32Pitch,				// 32 λ����һ����ռ�õ��ֽ���
							  IN unsigned char *pucYUV16Frame,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
							  IN intptr_t nYUV16Pitch,				// 16 λ����һ����ռ�õ��ֽ���
							  IN int nWidth,						// Դͼ���ؾ����С
							  IN int nHeight,
							  IN bool bHD,	// ������
							  IN bool bBGRA,						// 32 λ��������ظ�ʽΪ A R G B��B�ڵ�λ��
							  IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
							  IN unsigned char cAlpha,				// ���� A ֵ
							  IN bool bOverturn)					// ��תͼ���־

				// ��תͼ���־
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnYUV16_2_RGB32_Frame(pucRGB32Frame, nRGB32Pitch, pucYUV16Frame,nYUV16Pitch,nWidth, nHeight,
			bHD ? NXVIDEO_DEFINITION_HIGH : NXVIDEO_DEFINITION_STANDARD, bBGRA, bUYVY, cAlpha, bOverturn);
	}
}

void TPCS_YUV16_Swap_Split(OUT unsigned char *pucYUV16OutTopField,		// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
						   IN intptr_t nOutTopPitch,					// 16 λ�������һ����ռ�õ��ֽ���
						   OUT unsigned char *pucYUV16OutBottomField,	// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
						   IN intptr_t nOutBottomPitch,				// 16 λ�������һ����ռ�õ��ֽ���
						   IN unsigned char *pucYUV16InFrame,			// 16 λ���뻺�棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
						   IN intptr_t nInPitch,						// 16 λ���뻺��һ����ռ�õ��ֽ���
						   IN int nWidth,								// Դͼ���ؾ����С
						   IN int nHeight,
						   IN bool bOverturn)							// ��תͼ���
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnYUV16_Swap_Split(pucYUV16OutTopField, nOutTopPitch, pucYUV16OutBottomField,nOutBottomPitch,pucYUV16InFrame,nInPitch, nWidth,
			nHeight, bOverturn);
	}
}
//YUVY---uyvy֮��ת����copy
void TPCS_YUV16_Swap_Frame(OUT unsigned char *pucYUV16OutFrame,	// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
						   IN intptr_t nOutPitch,										// 16 λ�������һ����ռ�õ��ֽ���
						   IN unsigned char *pucYUV16InFrame,							// 16 λ���뻺�棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
						   IN intptr_t nInPitch,										// 16 λ���뻺��һ����ռ�õ��ֽ���
						   IN int nWidth,												// Դͼ���ؾ����С
						   IN int nHeight,
						   IN bool bOverturn)										// ��תͼ���־
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnYUV16_Swap_Frame(pucYUV16OutFrame, nOutPitch, pucYUV16InFrame,nInPitch,nWidth,
			nHeight, bOverturn);
	}
}
//////////////////////////////////////////////////////////////////////////
// �ļ�����