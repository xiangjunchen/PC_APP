//////////////////////////////////////////////////////////////////////////
// 描述：	各个颜色空间之间的转换函数
// 创建：	赵志猛 @ 2007-07-26
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TPColorSpaceConv.h"
#include "TPSSE2.h"
#include <assert.h>

//[[lgq 2009.11.18
//64位下使用新的库
#include "CoreInclude\CPUInclude\NXMemProc.h"
#include "CoreInclude\CPUInclude\NXVideoProc.h"
#include "CoreInclude\CPUInclude\NXColorSpace.h"
//]]

extern LPNXMEMPROCAPI g_lpMemProcAPI;
extern LPNXVIDEOPROCAPI g_lpVideoProcAPI;

//////////////////////////////////////////////////////////////////////////
// 常量定义

#define PAL_SD_HEIGHT			576			// PAL 制式 SD 幅面高
#define SPECIAL_YUYV_VALUE		0x80108010

///////////////////////////////////////////////////////////////////////////////
// YUVA 与 RGBA 之间的转换

// 高清定义（HD）
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

// 标清定义（SD）（索尼认为数字信号下 PAL 与 NTSC 相同）
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
// LUVA 与 RGBA 之间的转换

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
// 根据制式获取转换矩阵

float* CALLBACK TPCS_GetMatYUVA2RGBA(IN UINT uDefinition)     // 制式
{
    if (uDefinition == HIGH_DEFINITION)
        return (float*)gc_matYUVA2RGBA_HD;

    if (uDefinition == STANDARD_DEFINITION)
        return (float*)gc_matYUVA2RGBA_SD;

    return NULL;
}

float CALLBACK TPCS_GetMatYUVA2RGBA_Cell(IN int nRow,             // 行
                                         IN int nCol,             // 列
                                         IN UINT uDefinition)     // 制式
{
    assert(nRow >= 0 && nRow < 4 && nCol >= 0 && nCol < 4);

    if (uDefinition == HIGH_DEFINITION)
        return gc_matYUVA2RGBA_HD[nRow][nCol];

    if (uDefinition == STANDARD_DEFINITION)
        return gc_matYUVA2RGBA_SD[nRow][nCol];

    return 0.f;
}

float* CALLBACK TPCS_GetMatRGBA2YUVA(IN UINT uDefinition)     // 制式
{
    if (uDefinition == HIGH_DEFINITION)
        return (float*)gc_matRGBA2YUVA_HD;

    if (uDefinition == STANDARD_DEFINITION)
        return (float*)gc_matRGBA2YUVA_SD;

    return NULL;
}

float CALLBACK TPCS_GetMatRGBA2YUVA_Cell(IN int nRow,             // 行
                                         IN int nCol,             // 列
                                         IN UINT uDefinition)     // 制式
{
    assert(nRow >= 0 && nRow < 4 && nCol >= 0 && nCol < 4);

    if (uDefinition == HIGH_DEFINITION)
        return gc_matRGBA2YUVA_HD[nRow][nCol];

    if (uDefinition == STANDARD_DEFINITION)
        return gc_matRGBA2YUVA_SD[nRow][nCol];

    return 0.f;
}

///////////////////////////////////////////////////////////////////////////////
// LUVA 与 RGBA 之间的转换

float* CALLBACK TPCS_GetMatLUVA2RGBA()
{
    return (float*)gc_matLUVA2RGBA;
}

float* CALLBACK TPCS_GetMatRGBA2LUVA()
{
    return (float*)gc_matRGBA2LUVA;
}

///////////////////////////////////////////////////////////////////////////////

// 场与帧的转换

void TPCS_ComposeFields(OUT BYTE* pDes,             // 目标
                        IN int nDesPitch,
                        IN BYTE* pSrcTop,           // 上场源
                        IN int nTopPitch,
                        IN BYTE* pSrcBottom,        // 下场源
                        IN int nBottomPitch,
                        IN int nWidth,              // 目标大小
                        IN int nHeight,
                        IN int nPixelBytes)         // 一个像素的大小
{
    int nRealLen = nWidth * nPixelBytes;

    assert(pDes && pSrcTop && pSrcBottom);
    assert(nTopPitch >= nRealLen && nBottomPitch >= nRealLen && nDesPitch >= nRealLen);

    int na16 = (nRealLen >> 4), nm16 = (nRealLen & 0x0f);
    int nHalfHeight = (nHeight >> 1);

    for (int i=0; i<nHalfHeight; i++)
    {
        // 第一场填充
        memcpy_line(pDes, pSrcTop, na16, nm16);
        pDes += nDesPitch;
        pSrcTop += nTopPitch;
        // 第二场填充
        memcpy_line(pDes, pSrcBottom, na16, nm16);
        pDes += nDesPitch;
        pSrcBottom += nBottomPitch;
    }
}

//wangfengming 2011.4.6 for OpenFX plugins
void TPCvtBGRAandRGBA(OUT unsigned char *pucRGBOutFrame,	// 输出缓冲（存储方式 X2 G X0）
					  IN INT_PTR nOutPitch,				// 输出缓冲一行所占用的字节数
					  IN unsigned char *pucRGBInFrame,	// 输入缓冲（存储方式 X0 G X2）
					  IN INT_PTR nInPitch,				// 输入缓冲一行所占用的字节数
					  IN int nWidth,						// 源图像素矩阵大小
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

void TPCvtRGB32_2_YUVA(OUT unsigned char *pucYUVFrame,		// 输出缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
					   IN INT_PTR nYUVPitch,				// 输出缓存一行所占用的字节数
					   IN unsigned char *pucRGBFrame,		// 输入缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
					   IN INT_PTR nRGBPitch,				// 输入缓存一行所占用的字节数
					   IN int nWidth,						// 源图像素矩阵大小
					   IN int nHeight,
					   IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
					   IN bool bBGR,						// 输入缓冲的像素格式为 R G B（B在低位）
					   IN bool bOverturn              // 翻转图像标志
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

void TPCvtYUVA2RGB32(OUT unsigned char *pucRGBFrame,		// 输出缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
					 IN INT_PTR nRGBPitch,				// 输出缓存一行所占用的字节数
					 IN unsigned char *pucYUVFrame,		// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
					 IN INT_PTR nYUVAPitch,				// 输入缓存一行所占用的字节数
					 IN int nWidth,						// 源图像素矩阵大小
					 IN int nHeight,
					 IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
					 IN bool bBGR,						// 输出缓冲的像素格式为 R G B（B在低位）的标志
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
void TPCvtYUV16_2_YUVA(OUT unsigned char *pucYUVAFrame,	// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
					   IN INT_PTR nYUVAPitch,			// 32 位缓存一行所占用的字节数
					   IN unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
					   IN INT_PTR nYUV16Pitch,			// 16 位缓存一行所占用的字节数
					   IN int nWidth,					// 源图像素矩阵大小
					   IN int nHeight,
					   IN bool bUYVY,					// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
					   IN unsigned char ucAlpha,			// 填充的 A 值
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
void TPCvtYUVA_2_YUV16(OUT unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
					   IN INT_PTR nYUV16Pitch,			// 16 位缓存一行所占用的字节数
					   IN unsigned char *pucYUVFrame,		// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
					   IN INT_PTR nYUVPitch,				// 缓存一行所占用的字节数
					   IN int nWidth,						// 源图像素矩阵大小
					   IN int nHeight,
					   IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
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

void TPCvtRGB24_2_YUYVFrame(OUT unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
							IN INT_PTR nYUV16Pitch,			// 16 位缓存一行所占用的字节数
							IN unsigned char *pucRGBFrame,		// 输入缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
							IN INT_PTR nRGBPitch,				// 输入缓存一行所占用的字节数
							IN int nWidth,						// 源图像素矩阵大小
							IN int nHeight,
							IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
							IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
							IN bool bBGR,						// 输入缓冲的像素格式为 R G B（B在低位）
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
//[[ljj@2010-2-4: YUYV+A->BGRA/RGBA.如果是要RGBA，请把bDstRGBA = TRUE。
BOOL NX_CvtYUYVA2BGRA_Frame(IN OUT BYTE* pBGRA, IN long lDstPitch, IN BYTE* pYUYV, IN long lSrcPitch, IN BYTE* pAlpha, IN long lAlphaPitch,
							IN long lWidth, IN long lHeight, IN BOOL bDstRGBA, IN BOOL bSrcRevert, IN BOOL bAlphaRevert)
{
	if (NULL == pBGRA || NULL == pYUYV || lDstPitch <= 0 || lSrcPitch <= 0 || lHeight <= 0 || lWidth <= 0)
	{
		return FALSE;
	}

	// 验证目前是否出现过 Key 与 视频的图像出现不同方向，这种情况将被限制不能出现
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

void TPCS_SplitFrame(OUT BYTE* pDesTop,             // 目标上场
                     IN int nTopPitch,
                     OUT BYTE* pDesBottom,          // 目标下场
                     IN int nBottomPitch,
                     IN BYTE* pSrc,                 // 源帧
                     IN int nSrcPitch,
                     IN int nWidth,                 // 源大小
                     IN int nHeight,
                     IN int nPixelBytes)            // 一个像素的大小
{
    int nRealLen = nWidth * nPixelBytes;

    assert(pDesTop && pSrc && pDesBottom);
    assert(nTopPitch >= nRealLen && nBottomPitch >= nRealLen && nSrcPitch >= nRealLen);

    int na16 = (nRealLen >> 4), nm16 = (nRealLen & 0x0f);
    int nHalfHeight = (nHeight >> 1);

    for (int i=0; i<nHalfHeight; i++)
    {
        // 第一场填充
        memcpy_line(pDesTop, pSrc, na16, nm16);
        pSrc += nSrcPitch;
        pDesTop += nTopPitch;
        // 第二场填充
        memcpy_line(pDesBottom, pSrc, na16, nm16);
        pDesBottom += nBottomPitch;
        pSrc += nSrcPitch;
    }
}

// YUV 内部格式转换 420 <-> 422 <-> 444

void TPCS_420_422_YUVFrame(OUT BYTE* pDes,          // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nDesPitch,
                           IN BYTE* pSrcY,          // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
                           IN int nSrcYPitch,
                           IN BYTE* pSrcU,          // U 输入（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
                           IN int nSrcUPitch,
                           IN BYTE* pSrcV,          // V 输入（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
                           IN int nSrcVPitch,
                           IN int nWidth,           // 源图大小
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

    // 前两行
    // 第一场
    yuv420_to_yuyv_line_2_F(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
    pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
    // 第二场
    yuv420_to_yuyv_line_2_F(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
    pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
    // 中间行
    for (int i=2; i<nHalfHalf; i++)
    {
        // 第一场
        yuv420_to_yuyv_line_2(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
        pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
        // 第二场
        yuv420_to_yuyv_line_2(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
        pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
    }
    // 最后两行或一行（场为奇数行时）
    if (nHalfHeight & 0x01)       // 一行
    {
        // 第一场
        yuv420_to_yuyv_line_1_L(pDes, pSrcY, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
        pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
        // 第二场
		if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
		{
			memset_dword_line(pDes, SPECIAL_YUYV_VALUE, (nDesPitch >> 4), (nDesPitch & 0x0f));
		}
		else
		{
			yuv420_to_yuyv_line_1_L(pDes, pSrcY, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		}
    }
    else    // 两行
    {
        // 第一场
        yuv420_to_yuyv_line_2_L(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
        pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
        // 第二场
		yuv420_to_yuyv_line_2_L(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
		{
			memset_dword_line((pDes + nDP2), SPECIAL_YUYV_VALUE, (nDesPitch >> 4), (nDesPitch & 0x0f));
		}
    }
}
void TPCS_420_422_YUVSplit(OUT BYTE* pDesFirst,        // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nFirstPitch,
                           OUT BYTE* pDesSecond,       // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nSecondPitch,
                           IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
                           IN int nSrcYPitch,
                           IN BYTE* pSrcU,             // U 输入（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
                           IN int nSrcUPitch,
                           IN BYTE* pSrcV,             // V 输入（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
                           IN int nSrcVPitch,
                           IN int nWidth,              // 源图大小
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

    // 前两行
    // 第一场
    yuv420_to_yuyv_line_2_F(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
    pDesFirst += nDFP2; pYF += nYP4; pUF += nUP2; pVF += nVP2;
    // 第二场
    yuv420_to_yuyv_line_2_F(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
    pDesSecond += nDSP2; pYS += nYP4; pUS += nUP2; pVS += nVP2;
    // 中间行
    for (int i=2; i<nHalfHalf; i++)
    {
        // 第一场
        yuv420_to_yuyv_line_2(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
        pDesFirst += nDFP2; pYF += nYP4; pUF += nUP2; pVF += nVP2;
        // 第二场
        yuv420_to_yuyv_line_2(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
        pDesSecond += nDSP2; pYS += nYP4; pUS += nUP2; pVS += nVP2;
    }
    // 最后两行或一行（场为奇数行时）
    if (nHalfHeight & 0x01)       // 一行
    {
        // 第一场
        yuv420_to_yuyv_line_1_L(pDesFirst, pYF, pUF, nUP2, pVF, nVP2, na16, nm16);
        // 第二场
		if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
		{
			memset_dword_line(pDesSecond, SPECIAL_YUYV_VALUE, (nSecondPitch >> 4), (nSecondPitch & 0x0f));
		}
		else
		{
			yuv420_to_yuyv_line_1_L(pDesSecond, pYS, pUS, nUP2, pVS, nVP2, na16, nm16);
		}
    }
    else    // 两行
    {
        // 第一场
        yuv420_to_yuyv_line_2_L(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
        // 第二场
		yuv420_to_yuyv_line_2_L(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
		if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
		{
			memset_dword_line((pDesSecond + nSecondPitch), SPECIAL_YUYV_VALUE, (nSecondPitch >> 4), (nSecondPitch & 0x0f));
		}
	}
}
// XDCAM 插值方式
void TPCS_420_422_YUVFrame_XDCAM(OUT BYTE* pDes,          // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
								 IN int nDesPitch,
								 IN BYTE* pSrcY,          // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
								 IN int nSrcYPitch,
								 IN BYTE* pSrcU,          // U 输入（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
								 IN int nSrcUPitch,
								 IN BYTE* pSrcV,          // V 输入（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
								 IN int nSrcVPitch,
								 IN int nWidth,           // 源图大小
								 IN int nHeight)
{
	assert(pDes && pSrcY && pSrcU && pSrcV);
	assert(nDesPitch >= (nWidth << 1) && nSrcYPitch >= nWidth);
	assert(nSrcUPitch >= (nWidth >> 1) && nSrcVPitch >= (nWidth >> 1));

	int nHalfHeight = (nHeight >> 1), nHalfHalf = ((nHalfHeight + 1) >> 1);
	int na16 = (nWidth >> 4), nm16 = (nWidth & 0x0f);
	int nYP2 = (nSrcYPitch << 1), nUP2 = (nSrcUPitch << 1), nVP2 = (nSrcVPitch << 1);
	int nDP2 = (nDesPitch << 1), nDP3 = (nDP2 + nDesPitch), nYP3 = (nYP2 + nSrcYPitch);

	// 前两行
	// 第一场
	yuv420_to_yuyv_xdcam_F_line_2_F(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
	pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
	// 第二场
	yuv420_to_yuyv_xdcam_S_line_2_F(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
	pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
	// 中间行
	for (int i=2; i<nHalfHalf; i++)
	{
		// 第一场
		yuv420_to_yuyv_xdcam_F_line_2(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		// 第二场
		yuv420_to_yuyv_xdcam_S_line_2(pDes, nDP2, pSrcY, nYP2, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		//////////////////////////////////////////////////////////////////////////
		// 第一场与第二场在垂直方向上具有强烈的对称性，则第二场能使用第一场的函数
		// 只需要在第一场结束后，将各个内存位置指向对称的位置，如下
		//pDes += nDP3; pSrcY += nYP3; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		//yuv420_to_yuyv_xdcam_F_line_2(pDes, -nDP2, pSrcY, -nYP2, pSrcU, -nUP2, pSrcV, -nVP2, na16, nm16);
		//pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
	}
	// 最后两行或一行（场为奇数行时）
	if (nHalfHeight & 0x01)       // 一行
	{
		// 第一场
		yuv420_to_yuyv_xdcam_F_line_1_L(pDes, pSrcY, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		// 第二场
		if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
		{
			memset_dword_line(pDes, SPECIAL_YUYV_VALUE, (nDesPitch >> 4), (nDesPitch & 0x0f));
		}
		else
		{
			yuv420_to_yuyv_xdcam_S_line_1_L(pDes, pSrcY, pSrcU, nUP2, pSrcV, nVP2, na16, nm16);
		}
	}
	else    // 两行
	{
		// 第一场（采用对称性，使用第二场的前两行）
		pDes += nDP2; pSrcY += nYP2;
		yuv420_to_yuyv_xdcam_S_line_2_F(pDes, -nDP2, pSrcY, -nYP2, pSrcU, -nUP2, pSrcV, -nVP2, na16, nm16);
		pDes += nDesPitch; pSrcY += nSrcYPitch; pSrcU += nSrcUPitch; pSrcV += nSrcVPitch;
		// 第二场（采用对称性，使用第一场的前两行）
		yuv420_to_yuyv_xdcam_F_line_2_F(pDes, -nDP2, pSrcY, -nYP2, pSrcU, -nUP2, pSrcV, -nVP2, na16, nm16);
		if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
		{
			memset_dword_line((pDes + nDP2), SPECIAL_YUYV_VALUE, (nDesPitch >> 4), (nDesPitch & 0x0f));
		}
	}
}
void TPCS_420_422_YUVSplit_XDCAM(OUT BYTE* pDesFirst,        // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
								 IN int nFirstPitch,
								 OUT BYTE* pDesSecond,       // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
								 IN int nSecondPitch,
								 IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
								 IN int nSrcYPitch,
								 IN BYTE* pSrcU,             // U 输入（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
								 IN int nSrcUPitch,
								 IN BYTE* pSrcV,             // V 输入（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
								 IN int nSrcVPitch,
								 IN int nWidth,              // 源图大小
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

	// 前两行
	// 第一场
	yuv420_to_yuyv_xdcam_F_line_2_F(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
	pDesFirst += nDFP2; pYF += nYP4; pUF += nUP2; pVF += nVP2;
	// 第二场
	yuv420_to_yuyv_xdcam_S_line_2_F(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
	pDesSecond += nDSP2; pYS += nYP4; pUS += nUP2; pVS += nVP2;
	// 中间行
	for (int i=2; i<nHalfHalf; i++)
	{
		// 第一场
		yuv420_to_yuyv_xdcam_F_line_2(pDesFirst, nFirstPitch, pYF, nYP2, pUF, nUP2, pVF, nVP2, na16, nm16);
		pDesFirst += nDFP2; pYF += nYP4; pUF += nUP2; pVF += nVP2;
		// 第二场
		yuv420_to_yuyv_xdcam_S_line_2(pDesSecond, nSecondPitch, pYS, nYP2, pUS, nUP2, pVS, nVP2, na16, nm16);
		pDesSecond += nDSP2; pYS += nYP4; pUS += nUP2; pVS += nVP2;
	}
	// 最后两行或一行（场为奇数行时）
	if (nHalfHeight & 0x01)       // 一行
	{
		// 第一场
		yuv420_to_yuyv_xdcam_F_line_1_L(pDesFirst, pYF, pUF, nUP2, pVF, nVP2, na16, nm16);
		// 第二场
		if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
		{
			memset_dword_line(pDesSecond, SPECIAL_YUYV_VALUE, (nSecondPitch >> 4), (nSecondPitch & 0x0f));
		}
		else
		{
			yuv420_to_yuyv_xdcam_S_line_1_L(pDesSecond, pYS, pUS, nUP2, pVS, nVP2, na16, nm16);
		}
	}
	else    // 两行
	{
		// 第一场（采用对称性，使用第二场的前两行）
		pDesFirst += nFirstPitch; pYF += nYP2;
		yuv420_to_yuyv_xdcam_S_line_2_F(pDesFirst, -nFirstPitch, pYF, -nYP2, pUF, -nUP2, pVF, -nVP2, na16, nm16);
		// 第二场（采用对称性，使用第一场的前两行）
		pDesSecond += nSecondPitch; pYS += nYP2;
		yuv420_to_yuyv_xdcam_F_line_2_F(pDesSecond, -nSecondPitch, pYS, -nYP2, pUS, -nUP2, pVS, -nVP2, na16, nm16);
		if (nHeight == PAL_SD_HEIGHT)	// 特殊的处理 DV PAL SD 的最后一行（填黑）
		{
			memset_dword_line((pDesSecond + nSecondPitch), SPECIAL_YUYV_VALUE, (nSecondPitch >> 4), (nSecondPitch & 0x0f));
		}
	}
}

// 采用场内四行 -1, 3, 3, -1 的系数对 UV 进行采样
void TPCS_422_420_YUVField_4(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
							 IN int nDesYPitch,
							 OUT BYTE* pDesU,			// U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
							 IN int nDesUPitch,
							 OUT BYTE* pDesV,			// V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
							 IN int nDesVPitch,
							 OUT BYTE* pSrcYUYV,		// 一场 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							 IN int nSrcPitch,
							 IN int nWidth,				// 源图大小
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

	// 第一行
	yuyv_to_yuv420_line_4_F(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nSrcPitch, na8, nm8);
	pDesY += nYP2;
	pDesU += nDesUPitch;
	pDesV += nDesVPitch;
	pSrcYUYV0 = pSrcYUYV2;
	pSrcYUYV2 += nYUYVP2;
	// 中间行
	for (int i=2; i<nHalfHeight; i++)		// 一次处理两行
	{
		yuyv_to_yuv420_line_4(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nSrcPitch, na8, nm8);
		pDesY += nYP2;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 = pSrcYUYV2;
		pSrcYUYV2 += nYUYVP2;
	}
	// 最后一行
	if ((nFieldHeight & 0x01) != 0)		// 奇数行，一般不可能出现
	{
		yuyv_to_yuv420_line_4_LS(pDesY, pDesU, pDesV, pSrcYUYV0, pSrcYUYV0 + nSrcPitch, na8, nm8);
	}
	else
	{
		yuyv_to_yuv420_line_4_L(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nSrcPitch, na8, nm8);
	}
}
void TPCS_422_420_YUVFrame_4(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
							 IN int nDesYPitch,
							 OUT BYTE* pDesU,			// U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
							 IN int nDesUPitch,
							 OUT BYTE* pDesV,			// V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
							 IN int nDesVPitch,
							 OUT BYTE* pSrcYUYV,		// 一帧 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							 IN int nSrcPitch,
							 IN int nWidth,				// 源图大小
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

	// 第一场的第一行
	yuyv_to_yuv420_line_4_F(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
	pDesY += nDesYPitch;
	pDesU += nDesUPitch;
	pDesV += nDesVPitch;
	pSrcYUYV0 += nSrcPitch;
	pSrcYUYV2 += nSrcPitch;
	// 第二场的第一行
	yuyv_to_yuv420_line_4_F(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
	pDesY += nYP3;
	pDesU += nDesUPitch;
	pDesV += nDesVPitch;
	pSrcYUYV0 += nSrcPitch;
	pSrcYUYV2 += nYUYVP3;
	// 中间行
	for (int i=2; i<nHalfHalfHeight; i++)		// 一次处理四行
	{
		// 第一场的两行
		yuyv_to_yuv420_line_4(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 += nSrcPitch;
		pSrcYUYV2 += nSrcPitch;
		// 第二场的两行
		yuyv_to_yuv420_line_4(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
		pDesY += nYP3;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 += nYUYVP3;
		pSrcYUYV2 += nYUYVP3;
	}
	// 最后一行
	if ((nHalfHeight & 0x01) != 0)		// 场奇数行，一般不可能出现
	{
		// 第一场最后一行
		yuyv_to_yuv420_line_4_LS(pDesY, pDesU, pDesV, pSrcYUYV0, pSrcYUYV0 + nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 += nSrcPitch;
		// 第二场最后一行
		yuyv_to_yuv420_line_4_LS(pDesY, pDesU, pDesV, pSrcYUYV0, pSrcYUYV0 + nYUYVP2, na8, nm8);
	}
	else
	{
		// 第一场最后一行
		yuyv_to_yuv420_line_4_L(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV0 += nSrcPitch;
		pSrcYUYV2 += nSrcPitch;
		// 第二场最后一行
		yuyv_to_yuv420_line_4_L(pDesY, nYP2, pDesU, pDesV, pSrcYUYV0, pSrcYUYV2, nYUYVP2, na8, nm8);
	}
}
// 采用场内两行平均的滤波方式对 UV 进行采样
void TPCS_422_420_YUVField(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
						   IN int nDesYPitch,
						   OUT BYTE* pDesU,         // U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
						   IN int nDesUPitch,
						   OUT BYTE* pDesV,         // V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
						   IN int nDesVPitch,
						   OUT BYTE* pSrcYUYV,		// 一场 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nSrcPitch,
						   IN int nWidth,           // 源图大小
						   IN int nFieldHeight)
{
	assert(pDesY != NULL && pDesU != NULL && pDesV != NULL && pSrcYUYV != NULL);
	assert(nDesUPitch >= (nWidth >> 1) && nDesVPitch >= (nWidth >> 1));
	assert(nDesYPitch >= nWidth && nSrcPitch >= (nWidth << 1));

	int nHalfHeight = (nFieldHeight >> 1);
	int na8 = (nWidth >> 3), nm8 = (nWidth & 0x07);
	int nYUYVP2 = (nSrcPitch << 1), nYP2 = (nDesYPitch << 1);
	
	for (int i=0; i<nHalfHeight; i++)		// 一次处理两行
	{
		yuyv_to_yuv420_line_2_avr(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV, nSrcPitch, na8, nm8);
		pDesY += nYP2;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nYUYVP2;
	}
	if ((nFieldHeight & 0x01) != 0)		// 奇数行，最后一行直接填充，一般不可能出现
	{
		yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
	}
}
void TPCS_422_420_YUVFrame(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
						   IN int nDesYPitch,
						   OUT BYTE* pDesU,         // U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
						   IN int nDesUPitch,
						   OUT BYTE* pDesV,         // V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
						   IN int nDesVPitch,
						   OUT BYTE* pSrcYUYV,		// 一帧 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nSrcPitch,
						   IN int nWidth,           // 源图大小
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
	
	for (int i=0; i<nHalfHalfHeight; i++)		// 一次处理四行
	{
		// 第一场的两行
		yuyv_to_yuv420_line_2_avr(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nSrcPitch;
		// 第二场的两行
		yuyv_to_yuv420_line_2_avr(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		pDesY += nYP3;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nYUYVP3;
	}
	if ((nHalfHeight & 0x01) != 0)		// 场奇数行，第二场直接填充
	{
		// 第一场最后一行
		if ((nFrameHeight & 0x01) != 0)		// 帧奇数行，则第一场为偶数行
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
		// 第二场最后一行
		yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
	}
}
// XDCAM 插值方式
void TPCS_422_420_YUVField_XDCAM(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
								 IN int nDesYPitch,
								 OUT BYTE* pDesU,			// U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
								 IN int nDesUPitch,
								 OUT BYTE* pDesV,			// V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
								 IN int nDesVPitch,
								 OUT BYTE* pSrcYUYV,		// 一场 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
								 IN int nSrcPitch,
								 IN int nWidth,				// 源图大小
								 IN int nFieldHeight,
								 IN BOOL bFristField)		// 奇偶场标志
{
	assert(pDesY != NULL && pDesU != NULL && pDesV != NULL && pSrcYUYV != NULL);
	assert(nDesUPitch >= (nWidth >> 1) && nDesVPitch >= (nWidth >> 1));
	assert(nDesYPitch >= nWidth && nSrcPitch >= (nWidth << 1));

	int nHalfHeight = (nFieldHeight >> 1);
	int na8 = (nWidth >> 3), nm8 = (nWidth & 0x07);
	int nYUYVP2 = (nSrcPitch << 1), nYP2 = (nDesYPitch << 1);

	if (bFristField)
	{
		for (int i=0; i<nHalfHeight; i++)		// 一次处理两行
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
		for (int i=0; i<nHalfHeight; i++)		// 一次处理两行
		{
			yuyv_to_yuv420_xdcam_S_line_2(pDesY, nDesYPitch, pDesU, pDesV, pSrcYUYV, nSrcPitch, na8, nm8);
			pDesY += nYP2;
			pDesU += nDesUPitch;
			pDesV += nDesVPitch;
			pSrcYUYV += nYUYVP2;
		}
	}
	if ((nFieldHeight & 0x01) != 0)		// 奇数行，最后一行直接填充，一般不可能出现
	{
		yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
	}
}
void TPCS_422_420_YUVFrame_XDCAM(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
								 IN int nDesYPitch,
								 OUT BYTE* pDesU,			// U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
								 IN int nDesUPitch,
								 OUT BYTE* pDesV,			// V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
								 IN int nDesVPitch,
								 OUT BYTE* pSrcYUYV,		// 一帧 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
								 IN int nSrcPitch,
								 IN int nWidth,				// 源图大小
								 IN int nFrameHeight)
{
	assert(pDesY != NULL && pDesU != NULL && pDesV != NULL && pSrcYUYV != NULL);
	assert(nDesUPitch >= (nWidth >> 2) && nDesVPitch >= (nWidth >> 2));
	assert(nDesYPitch >= nWidth && nSrcPitch >= (nWidth << 1));

	int nHalfHeight = (nFrameHeight >> 1), nHalfHalfHeight = (nHalfHeight >> 1);
	int na8 = (nWidth >> 3), nm8 = (nWidth & 0x07);
	int nYUYVP2 = (nSrcPitch << 1), nYUYVP3 = nSrcPitch + nYUYVP2;
	int nYP2 = (nDesYPitch << 1), nYP3 = nDesYPitch + nYP2;

	for (int i=0; i<nHalfHalfHeight; i++)		// 一次处理四行
	{
		// 第一场
		yuyv_to_yuv420_xdcam_F_line_2(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		pDesY += nDesYPitch;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nSrcPitch;
		// 第二场
		yuyv_to_yuv420_xdcam_S_line_2(pDesY, nYP2, pDesU, pDesV, pSrcYUYV, nYUYVP2, na8, nm8);
		pDesY += nYP3;
		pDesU += nDesUPitch;
		pDesV += nDesVPitch;
		pSrcYUYV += nYUYVP3;
	}
	if ((nHalfHeight & 0x01) != 0)		// 场奇数行，第二场直接填充
	{
		// 第一场最后一行
		if ((nFrameHeight & 0x01) != 0)		// 帧奇数行，则第一场为偶数行
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
		// 第二场最后一行
		yuyv_to_yuv420_line_1(pDesY, pDesU, pDesV, pSrcYUYV, na8, nm8);
	}
}

void TPCS_ComposeYUV422_Frame(OUT BYTE* pDes,          // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                              IN int nDesPitch,
                              IN BYTE* pSrcY,          // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
                              IN int nSrcYPitch,
                              IN BYTE* pSrcU,          // U 输入（存储方式 U0 U2 U4 ……）
                              IN int nSrcUPitch,
                              IN BYTE* pSrcV,          // V 输入（存储方式 V0 V2 V4 ……）
                              IN int nSrcVPitch,
                              IN int nWidth,           // 源图大小
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
void TPCS_ComposeYUV422_Split(OUT BYTE* pDesFirst,        // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                              IN int nFirstPitch,
                              OUT BYTE* pDesSecond,       // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                              IN int nSecondPitch,
                              IN BYTE* pSrcY,             // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
                              IN int nSrcYPitch,
                              IN BYTE* pSrcU,             // U 输入（存储方式 U0 U2 U4 ……）
                              IN int nSrcUPitch,
                              IN BYTE* pSrcV,             // V 输入（存储方式 V0 V2 V4 ……）
                              IN int nSrcVPitch,
                              IN int nWidth,              // 源图大小
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
    if (nHeight & 0x01)     // 奇数行则第二场拷贝
    {
        yuv422_to_yuyv_line(pDesFirst, pSrcY, pSrcU, pSrcV, na16, nm16);
        memcpy_line(pDesSecond, pDesFirst, (nWidth >> 3), ((nWidth & 0x07) << 1));
    }
}

// YUV 420、422 片存储格式与 RGBA、BGRA 之间的转换

void TPCS_YUV420_RGBA_Frame(OUT BYTE* pDesRGBA,         // RGBA 输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
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
                            IN BOOL bDesRevert)         // 图像是否行翻转标志
{
    assert(pDesRGBA && pSrcY && pSrcU && pSrcV);
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth) && (nDesPitch >= (nWidth << 2)));

    yuv420_to_color32_frame(pDesRGBA, nDesPitch, pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, FALSE);
}
void TPCS_YUV420_RGBA_Split(OUT BYTE* pDesFirst,        // RGBA 第一场输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
                            IN int nFirstPitch,
                            OUT BYTE* pDesSecond,       // RGBA 第二场输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
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
                            IN BOOL bDesRevert)         // 图像是否行翻转标志
{
    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert((nFirstPitch >= (nWidth << 2)) && (nSecondPitch >= (nWidth << 2)));
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth));

    yuv420_to_color32_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch,
        pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, FALSE);
}

void TPCS_YUV422_RGBA_Frame(OUT BYTE* pDesRGBA,         // RGBA 输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
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
                            IN BOOL bDesRevert)         // 图像是否行翻转标志
{
    assert(pDesRGBA && pSrcY && pSrcU && pSrcV);
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth) && (nDesPitch >= (nWidth << 2)));

    yuv422_to_color32_frame(pDesRGBA, nDesPitch, pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, FALSE);
}
void TPCS_YUV422_RGBA_Split(OUT BYTE* pDesFirst,        // RGBA 第一场输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
                            IN int nFirstPitch,
                            OUT BYTE* pDesSecond,       // RGBA 第二场输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
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
                            IN BOOL bDesRevert)         // 图像是否行翻转标志
{
    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert((nFirstPitch >= (nWidth << 2)) && (nSecondPitch >= (nWidth << 2)));
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth));

    yuv422_to_color32_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch,
        pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, FALSE);
}

void TPCS_YUV420_BGRA_Frame(OUT BYTE* pDesBGRA,         // BGRA 输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
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
                            IN BOOL bDesRevert)         // 图像是否行翻转标志
{
    assert(pDesBGRA && pSrcY && pSrcU && pSrcV);
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth) && (nDesPitch >= (nWidth << 2)));

    yuv420_to_color32_frame(pDesBGRA, nDesPitch, pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, TRUE);
}
void TPCS_YUV420_BGRA_Split(OUT BYTE* pDesFirst,        // BGRA 第一场输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
                            IN int nFirstPitch,
                            OUT BYTE* pDesSecond,       // RGBA 第二场输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
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
                            IN BOOL bDesRevert)         // 图像是否行翻转标志
{
    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert((nFirstPitch >= (nWidth << 2)) && (nSecondPitch >= (nWidth << 2)));
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth));

    yuv420_to_color32_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch,
        pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, TRUE);
}

void TPCS_YUV422_BGRA_Frame(OUT BYTE* pDesBGRA,         // BGRA 输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
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
                            IN BOOL bDesRevert)         // 图像是否行翻转标志
{
    assert(pDesBGRA && pSrcY && pSrcU && pSrcV);
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth) && (nDesPitch >= (nWidth << 2)));

    yuv422_to_color32_frame(pDesBGRA, nDesPitch, pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, TRUE);
}
void TPCS_YUV422_BGRA_Split(OUT BYTE* pDesFirst,        // BGRA 第一场输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
                            IN int nFirstPitch,
                            OUT BYTE* pDesSecond,       // RGBA 第二场输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
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
                            IN BOOL bDesRevert)         // 图像是否行翻转标志
{
    assert(pDesFirst && pDesSecond && pSrcY && pSrcU && pSrcV);
    assert((nFirstPitch >= (nWidth << 2)) && (nSecondPitch >= (nWidth << 2)));
    assert((nSrcYPitch >= nWidth) && ((nSrcUPitch << 1) >= nWidth) && ((nSrcVPitch << 1) >= nWidth));

    yuv422_to_color32_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch,
        pSrcY, nSrcYPitch, pSrcU, nSrcUPitch, pSrcV, nSrcVPitch, nWidth, nHeight, bHD, bDesRevert, TRUE);
}

// YUYVA 与 RGBA、BGRA 之间的转换

void  TPCS_RGBA2YUYVA_Frame(OUT BYTE* pDesYUYV,         // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nYUYVPitch,
                            OUT BYTE* pDesAlpha,        // Alpha 输出（存储方式 A0 A1 A2 A3 A4 ……）
                            IN int nAlphaPitch,
                            IN BYTE* pSrcRGBA,          // RGBA 输入（存储方式 R G B A R G B A R G B A ……）
                            IN int nSrcPitch,           // 源一行的字节宽度
                            IN int nWidth,			    // 源图大小
                            IN int nHeight,
                            IN BOOL bHD,                // 高标清标志
                            IN BOOL bSrcRevert)         // 图像是否行翻转标志
{
    assert(pDesYUYV && pDesAlpha && pSrcRGBA);
    assert((nYUYVPitch >= (nWidth << 1)) && (nAlphaPitch >= nWidth) && (nSrcPitch >= (nWidth << 2)));

    color32_to_yuyva_frame(pDesYUYV, nYUYVPitch, pDesAlpha, nAlphaPitch, pSrcRGBA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}
void  TPCS_RGBA2YUYVA_Split(OUT BYTE* pDesYUYVF,        // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nYUYVPitchF,
                            OUT BYTE* pDesAlphaF,       // Alpha 第一场输出（存储方式 A0 A1 A2 A3 A4 ……）
                            IN int nAlphaPitchF,
                            OUT BYTE* pDesYUYVS,        // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nYUYVPitchS,
                            OUT BYTE* pDesAlphaS,       // Alpha 第二场输出（存储方式 A0 A1 A2 A3 A4 ……）
                            IN int nAlphaPitchS,
                            IN BYTE* pSrcRGBA,          // RGBA 输入（存储方式 R G B A R G B A R G B A ……）
                            IN int nSrcPitch,           // 源一行的字节宽度
                            IN int nWidth,              // 源图大小
                            IN int nHeight,
                            IN BOOL bHD,                // 高标清标志
                            IN BOOL bSrcRevert)         // 图像是否行翻转标志
{
    assert(pDesYUYVF && pDesAlphaF && pDesAlphaS && pDesYUYVS && pSrcRGBA);
    assert(nYUYVPitchF >= (nWidth << 1) && nAlphaPitchF >= nWidth && nYUYVPitchS >= (nWidth << 1) && nAlphaPitchS >= nWidth);
    assert(nSrcPitch >= (nWidth << 2));

    color32_to_yuyva_split(pDesYUYVF, nYUYVPitchF, pDesAlphaF, nAlphaPitchF, pDesYUYVS, nYUYVPitchS, pDesAlphaS, nAlphaPitchS,
        pSrcRGBA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}
void  TPCS_RGBA2YUYV_Frame(OUT BYTE* pDesYUYV,          // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitch,
                           IN BYTE* pSrcRGBA,           // RGBA 输入（存储方式 R G B A R G B A R G B A ……）
                           IN int nSrcPitch,            // 源一行的字节宽度
                           IN int nWidth,			    // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                 // 高标清标志
                           IN BOOL bSrcRevert)          // 图像是否行翻转标志
{
    assert(pDesYUYV && pSrcRGBA);
    assert(nYUYVPitch >= (nWidth << 1) && nSrcPitch >= (nWidth << 2));

    color32_to_yuyv_frame(pDesYUYV, nYUYVPitch, pSrcRGBA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}
void  TPCS_RGBA2YUYV_Split(OUT BYTE* pDesYUYVF,         // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesYUYVS,         // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchS,
                           IN BYTE* pSrcRGBA,           // RGBA 输入（存储方式 R G B A R G B A R G B A ……）
                           IN int nSrcPitch,            // 源一行的字节宽度
                           IN int nWidth,               // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                 // 高标清标志
                           IN BOOL bSrcRevert)          // 图像是否行翻转标志
{
    assert(pDesYUYVF && pDesYUYVS && pSrcRGBA);
    assert(nYUYVPitchF >= (nWidth << 1) && nYUYVPitchS >= (nWidth << 1) && nSrcPitch >= (nWidth << 2));

    color32_to_yuyv_split(pDesYUYVF, nYUYVPitchF, pDesYUYVS, nYUYVPitchS, pSrcRGBA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}

void TPCS_YUYV2RGBA_Compose(OUT BYTE* pDesRGBA,         // RGBA 输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
                            IN int nDesPitch,
                            IN BYTE* pSrcYUYVF,         // YUYV 第一场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nFirstPitch,
                            IN BYTE* pSrcYUYVS,         // YUYV 第二场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nSecondPitch,
                            IN int nWidth,              // 目标大小
                            IN int nHeight,
                            IN BOOL bHD,                // 高标清标志
							IN BOOL bDstRevert)			// 图像是否行翻转标志

{
    assert(pDesRGBA && pSrcYUYVF && pSrcYUYVS);
    assert((nDesPitch >= (nWidth << 2)) && (nFirstPitch >= (nWidth << 1)) && (nSecondPitch >= (nWidth << 1)));

    yuyv_to_color32_compose(pDesRGBA, nDesPitch, pSrcYUYVF, nFirstPitch, pSrcYUYVS, nSecondPitch, nWidth, nHeight, bHD, FALSE, bDstRevert);
}


void  TPCS_BGRA2YUYVA_Frame(OUT BYTE* pDesYUYV,         // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nYUYVPitch,
                            OUT BYTE* pDesAlpha,        // Alpha 输出（存储方式 A0 A1 A2 A3 A4 ……）
                            IN int nAlphaPitch,
                            IN BYTE* pSrcBGRA,          // BGRA 输入（存储方式 B G R A B G R A B G R A ……）
                            IN int nSrcPitch,           // 源一行的字节宽度
                            IN int nWidth,			    // 源图大小
                            IN int nHeight,
                            IN BOOL bHD,                // 高标清标志
                            IN BOOL bSrcRevert)         // 图像是否行翻转标志
{
    assert(pDesYUYV && pDesAlpha && pSrcBGRA);
    assert((nYUYVPitch >= (nWidth << 1)) && (nAlphaPitch >= nWidth) && (nSrcPitch >= (nWidth << 2)));

    color32_to_yuyva_frame(pDesYUYV, nYUYVPitch, pDesAlpha, nAlphaPitch, pSrcBGRA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}
void  TPCS_BGRA2YUYVA_Split(OUT BYTE* pDesYUYVF,        // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nYUYVPitchF,
                            OUT BYTE* pDesAlphaF,       // Alpha 第一场输出（存储方式 A0 A1 A2 A3 A4 ……）
                            IN int nAlphaPitchF,
                            OUT BYTE* pDesYUYVS,        // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nYUYVPitchS,
                            OUT BYTE* pDesAlphaS,       // Alpha 第二场输出（存储方式 A0 A1 A2 A3 A4 ……）
                            IN int nAlphaPitchS,
                            IN BYTE* pSrcBGRA,          // BGRA 输入（存储方式 B G R A B G R A B G R A ……）
                            IN int nSrcPitch,           // 源一行的字节宽度
                            IN int nWidth,			    // 源图大小
                            IN int nHeight,
                            IN BOOL bHD,                // 高标清标志
                            IN BOOL bSrcRevert)         // 图像是否行翻转标志
{
    assert(pDesYUYVF && pDesAlphaF && pDesAlphaS && pDesYUYVS && pSrcBGRA);
    assert(nYUYVPitchF >= (nWidth << 1) && nAlphaPitchF >= nWidth && nYUYVPitchS >= (nWidth << 1) && nAlphaPitchS >= nWidth);
    assert(nSrcPitch >= (nWidth << 2));

    color32_to_yuyva_split(pDesYUYVF, nYUYVPitchF, pDesAlphaF, nAlphaPitchF, pDesYUYVS, nYUYVPitchS, pDesAlphaS, nAlphaPitchS,
        pSrcBGRA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}
void  TPCS_BGRA2YUYV_Frame(OUT BYTE* pDesYUYV,          // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitch,
                           IN BYTE* pSrcBGRA,           // BGRA 输入（存储方式 B G R A B G R A B G R A ……）
                           IN int nSrcPitch,            // 源一行的字节宽度
                           IN int nWidth,			    // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                 // 高标清标志
                           IN BOOL bSrcRevert)          // 图像是否行翻转标志
{
    assert(pDesYUYV && pSrcBGRA);
    assert(nYUYVPitch >= (nWidth << 1) && nSrcPitch >= (nWidth << 2));
	g_NXColorSpace->pStuRGB32toOthers->pfnRGB32_2_YUV16_Frame(pDesYUYV,  static_cast<intptr_t>(nYUYVPitch), \
		pSrcBGRA,  static_cast<intptr_t>(nSrcPitch), nWidth, nHeight,bHD ? NXVIDEO_DEFINITION_HIGH :NXVIDEO_DEFINITION_STANDARD ,0,1,bSrcRevert);

    //color32_to_yuyv_frame(pDesYUYV, nYUYVPitch, pSrcBGRA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}
void  TPCS_BGRA2YUYV_Split(OUT BYTE* pDesYUYVF,         // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesYUYVS,         // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchS,
                           IN BYTE* pSrcBGRA,           // BGRA 输入（存储方式 B G R A B G R A B G R A ……）
                           IN int nSrcPitch,            // 源一行的字节宽度
                           IN int nWidth,               // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                 // 高标清标志
                           IN BOOL bSrcRevert)          // 图像是否行翻转标志
{
    assert(pDesYUYVF && pDesYUYVS && pSrcBGRA);
    assert(nYUYVPitchF >= (nWidth << 1) && nYUYVPitchS >= (nWidth << 1) && nSrcPitch >= (nWidth << 2));

    color32_to_yuyv_split(pDesYUYVF, nYUYVPitchF, pDesYUYVS, nYUYVPitchS, pSrcBGRA, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}

void TPCS_YUYV2BGRA_Compose(OUT BYTE* pDesBGRA,         // BGRA 输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
                            IN int nDesPitch,
                            IN BYTE* pSrcYUYVF,         // YUYV 第一场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nFirstPitch,
                            IN BYTE* pSrcYUYVS,         // YUYV 第二场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                            IN int nSecondPitch,
                            IN int nWidth,              // 目标大小
                            IN int nHeight,
                            IN BOOL bHD,                // 高标清标志
							IN BOOL bDstRevert)			// 图像是否行翻转标志
{
    assert(pDesBGRA && pSrcYUYVF && pSrcYUYVS);
    assert((nDesPitch >= (nWidth << 2)) && (nFirstPitch >= (nWidth << 1)) && (nSecondPitch >= (nWidth << 1)));

    yuyv_to_color32_compose(pDesBGRA, nDesPitch, pSrcYUYVF, nFirstPitch, pSrcYUYVS, nSecondPitch, nWidth, nHeight, bHD, TRUE, bDstRevert);
}
void TPCS_YUYV2BGRA_Frame(OUT BYTE* pDesBGRA,         // BGRA 输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
						  IN int nDesPitch,
						  IN BYTE* pSrcYUYV,         // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						  IN int nSrcPitch,
						  IN int nWidth,              // 目标大小
						  IN int nHeight,
						  IN BOOL bHD,                // 高标清标志
						  IN BOOL bDstRevert)			// 目标是否反转
{
	assert(pDesBGRA && pSrcYUYV );
	assert((nDesPitch >= (nWidth << 2)) && (nSrcPitch >= (nWidth << 1)));
	g_NXColorSpace->pStuYUV16toOthers->pfnYUV16_2_RGB32_Frame(pDesBGRA,  static_cast<intptr_t>(nDesPitch), \
		pSrcYUYV,  static_cast<intptr_t>(nSrcPitch), nWidth, nHeight, bHD ? NXVIDEO_DEFINITION_HIGH :NXVIDEO_DEFINITION_STANDARD,1,0,255,bDstRevert);

	//yuyv_to_color32_frame(pDesBGRA, nDesPitch, pSrcYUYV, nSrcPitch,  nWidth, nHeight, bHD, TRUE, bDstRevert);
}

void TPCS_YUYV2BGR_Frame(OUT BYTE* pDesBGR,         // BGR 输出（存储方式 B G R B G R B G R ……）
						 IN int nDesPitch,
						 IN BYTE* pSrcYUYV,         // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						 IN int nSrcPitch,
						 IN int nWidth,              // 目标大小
						 IN int nHeight,
						 IN BOOL bHD,                // 高标清标志
						 IN BOOL bDstRevert)		// 图像是否行翻转标志
{
	assert(pDesBGR && pSrcYUYV );
	assert((nDesPitch >= (nWidth*3)) && (nSrcPitch >= (nWidth << 1)));

	yuyv_to_color24_frame(pDesBGR, nDesPitch, pSrcYUYV, nSrcPitch,  nWidth, nHeight, bHD, TRUE, bDstRevert);
}
void TPCS_YUYV2RGB_Frame(OUT BYTE* pDesRGB,         // BGR 输出（存储方式 R G B R G B R G B……）
						 IN int nDesPitch,
						 IN BYTE* pSrcYUYV,         // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						 IN int nSrcPitch,
						 IN int nWidth,              // 目标大小
						 IN int nHeight,
						 IN BOOL bHD,                // 高标清标志
						 IN BOOL bDstRevert)		// 图像是否行翻转标志
{
	assert(pDesRGB && pSrcYUYV );
	assert((nDesPitch >= (nWidth*3)) && (nSrcPitch >= (nWidth << 1)));

	yuyv_to_color24_frame(pDesRGB, nDesPitch, pSrcYUYV, nSrcPitch,  nWidth, nHeight, bHD, FALSE, bDstRevert);
}

void TPCS_YUYV2RGBA_Frame(OUT BYTE* pDesRGBA,         // RGBA 输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
							IN int nDesPitch,
							IN BYTE* pSrcYUYV,         // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							IN int nSrcPitch,
							IN int nWidth,              // 目标大小
							IN int nHeight,
							IN BOOL bHD,                // 高标清标志
							IN BOOL bDstRevert)			// 图像是否行翻转标志
{
	assert(pDesRGBA && pSrcYUYV );
	assert((nDesPitch >= (nWidth << 2)) && (nSrcPitch >= (nWidth << 1)));

	yuyv_to_color32_frame(pDesRGBA, nDesPitch, pSrcYUYV, nSrcPitch,  nWidth, nHeight, bHD, FALSE, bDstRevert);
}

void TPCS_YUYV2BGRA_MeanStretch(OUT BYTE* pDesBGRA,		// BGRA 输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
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
								IN BOOL bSrcHD,			// 源信号的高标清标志（决定使用颜色变换的公式）
								IN BOOL bDesRevert)		// 图像是否行翻转标志
{
	assert(pDesBGRA && pSrcYUYV);
	assert((nDesPitch >= (nDesImgWidth << 2)) && (nSrcPitch >= (nSrcImgWidth << 1)));

	yuyv_to_color32_meanstretch(pDesBGRA, nDesPitch, nDesX, nDesY, nDesWidth, nDesHeight, nDesImgWidth, nDesImgHeight,
		pSrcYUYV, nSrcPitch, nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcImgWidth, nSrcImgHeight, bSrcHD, TRUE, bDesRevert);
}

void TPCS_YUYV2RGBA_MeanStretch(OUT BYTE* pDesRGBA,		// RGBA 输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
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
								IN BOOL bSrcHD,			// 源信号的高标清标志（决定使用颜色变换的公式）
								IN BOOL bDesRevert)		// 图像是否行翻转标志
{
	assert(pDesRGBA && pSrcYUYV);
	assert((nDesPitch >= (nDesImgWidth << 2)) && (nSrcPitch >= (nSrcImgWidth << 1)));
	assert((nDesImgWidth >= 0) && (nDesImgHeight >= 0) && (nSrcImgWidth >= 0) && (nSrcImgHeight >= 0));

	yuyv_to_color32_meanstretch(pDesRGBA, nDesPitch, nDesX, nDesY, nDesWidth, nDesHeight, nDesImgWidth, nDesImgHeight,
		pSrcYUYV, nSrcPitch, nSrcX, nSrcY, nSrcWidth, nSrcHeight, nSrcImgWidth, nSrcImgHeight, bSrcHD, FALSE, bDesRevert);
}

// YUYV 与 RGB、BGR 之间的转换

void  TPCS_RGB2YUYV_Frame(OUT BYTE* pDesYUYV,           // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nYUYVPitch,
                          IN BYTE* pSrcRGB,             // RGB 输入（存储方式 R G B R G B R G B ……）
                          IN int nSrcPitch,             // 源一行的字节宽度（必须为 4 的倍数）
                          IN int nWidth,			    // 源图大小
                          IN int nHeight,
                          IN BOOL bHD,                  // 高标清标志
                          IN BOOL bSrcRevert)           // 图像是否行翻转标志
{
    assert(pDesYUYV && pSrcRGB);
    assert(nYUYVPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSrcPitch >= (nWidth * 3));

    color24_to_yuyv_frame(pDesYUYV, nYUYVPitch, pSrcRGB, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}
void  TPCS_RGB2YUYV_Split(OUT BYTE* pDesFirst,          // 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nFirstPitch,
                          OUT BYTE* pDesSecond,         // 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nSecondPitch,
                          IN BYTE* pSrcRGB,             // RGB 输入（存储方式 R G B R G B R G B ……）
                          IN int nSrcPitch,             // 源一行的字节宽度（必须为 4 的倍数）
                          IN int nWidth,			    // 源图大小
                          IN int nHeight,
                          IN BOOL bHD,                  // 高标清标志
                          IN BOOL bSrcRevert)           // 图像是否行翻转标志
{
    assert(pDesFirst && pDesSecond && pSrcRGB);
    assert(nFirstPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSecondPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSrcPitch >= (nWidth * 3));

    color24_to_yuyv_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch, pSrcRGB, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, FALSE);
}

void  TPCS_BGR2YUYV_Frame(OUT BYTE* pDesYUYV,           // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nYUYVPitch,
                          IN BYTE* pSrcBGR,             // BGR 输入（存储方式 B G R B G R B G R ……）
                          IN int nSrcPitch,             // 源一行的字节宽度（必须为 4 的倍数）
                          IN int nWidth,			    // 源图大小
                          IN int nHeight,
                          IN BOOL bHD,                  // 高标清标志
                          IN BOOL bSrcRevert)           // 图像是否行翻转标志
{
    assert(pDesYUYV && pSrcBGR);
    assert(nYUYVPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSrcPitch >= (nWidth * 3));

    color24_to_yuyv_frame(pDesYUYV, nYUYVPitch, pSrcBGR, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}
void  TPCS_BGR2YUYV_Split(OUT BYTE* pDesFirst,          // 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nFirstPitch,
                          OUT BYTE* pDesSecond,         // 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nSecondPitch,
                          IN BYTE* pSrcBGR,             // BGR 输入（存储方式 B G R B G R B G R ……）
                          IN int nSrcPitch,             // 源一行的字节宽度（必须为 4 的倍数）
                          IN int nWidth,			    // 源图大小
                          IN int nHeight,
                          IN BOOL bHD,                  // 高标清标志
                          IN BOOL bSrcRevert)           // 图像是否行翻转标志
{
    assert(pDesFirst && pDesSecond && pSrcBGR);
    assert(nFirstPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSecondPitch >= ((nWidth + (nWidth & 0x01)) << 1));
    assert(nSrcPitch >= (nWidth * 3));

    color24_to_yuyv_split(pDesFirst, nFirstPitch, pDesSecond, nSecondPitch, pSrcBGR, nSrcPitch, nWidth, nHeight, bHD, bSrcRevert, TRUE);
}

// RGB Alpha 与 RGBA、BGR Alpha 与 BGRA 之间的转换

void TPCS_24_to_32_Frame(OUT BYTE* pDes32,				// 输出（存储方式 X0 X1 X2 A X0 X1 X2 A ……）
						 IN int nDesPitch,
						 IN BYTE* pSrc24,               // 24 位源输入（存储方式 X0 X1 X2 X0 X1 X2 ……）
						 IN int nSrc24Pitch,            // 源一行的字节宽度（必须为 4 的倍数）
						 IN BYTE* pSrcAlpha,            // 源 Alpha 输入（存储方式 A0 A1 A2 A3 ……）（如果输入为 NULL 则直接填 0xff）
						 IN int nSrcAlphaPitch,
						 IN int nWidth,                 // 源图大小
						 IN int nHeight,
						 IN BOOL bSrc24Revert,          // 图像是否行翻转标志
						 IN BOOL bSrcAlphaRevert)       // Alpha 是否行翻转标志
{
	assert(pDes32 && pSrc24);
	assert(nDesPitch >= ((nWidth + (nWidth & 0x01)) << 1));
	assert(nSrc24Pitch >= (nWidth * 3));
	assert(pSrcAlpha == NULL || nSrcAlphaPitch >= nWidth);

	int nap5 = (nWidth / 5), nmp5 = (nWidth - 5 * nap5);
	BYTE* pSrc24Line = (bSrc24Revert) ? (pSrc24 + (nHeight - 1) * nSrc24Pitch) : pSrc24;
	int nRealSrc24Pitch = (bSrc24Revert) ? -nSrc24Pitch : nSrc24Pitch;

	if (pSrcAlpha)      // 有 Alpha 输入
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
	else    // 无 Alpha 输入则直接填充 0xff
	{
		for (int i=0; i<nHeight; i++)
		{
			color24_to_color32(pDes32, pSrc24Line, nap5, nmp5);
			pSrc24Line += nRealSrc24Pitch;
			pDes32 += nDesPitch;
		}
	}
}
void TPCS_24_to_32_Split(OUT BYTE* pDes32First,         // 第一场输出（存储方式 X0 X1 X2 A X0 X1 X2 A ……）
                         IN int nFirstPitch,
                         OUT BYTE* pDes32Second,        // 第二场输出（存储方式 X0 X1 X2 A X0 X1 X2 A ……）
                         IN int nSecondPitch,
                         IN BYTE* pSrc24,               // 24 位源输入（存储方式 X0 X1 X2 X0 X1 X2 ……）
                         IN int nSrc24Pitch,            // 源一行的字节宽度（必须为 4 的倍数）
                         IN BYTE* pSrcAlpha,            // 源 Alpha 输入（存储方式 A0 A1 A2 A3 ……）（如果输入为 NULL 则直接填 0xff）
                         IN int nSrcAlphaPitch,
                         IN int nWidth,                 // 源图大小
                         IN int nHeight,
                         IN BOOL bSrc24Revert,          // 图像是否行翻转标志
                         IN BOOL bSrcAlphaRevert)       // Alpha 是否行翻转标志
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

    if (pSrcAlpha)      // 有 Alpha 输入
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
    else    // 无 Alpha 输入则直接填充 0xff
    {
        for (int i=0; i<nHalfHeight; i++)
        {
            // 第一场
            color24_to_color32(pDes32First, pSrc24Line, nap5, nmp5);
            pSrc24Line += nRealSrc24Pitch;
            pDes32First += nFirstPitch;
            // 第二场
            color24_to_color32(pDes32Second, pSrc24Line, nap5, nmp5);
            pSrc24Line += nRealSrc24Pitch;
            pDes32Second += nSecondPitch;
        }
        if (nHeight & 0x01)     // 如果为奇数行，需要特殊处理，第一场照旧，第二场清黑或拷贝
        {
            // 第一场
            color24_to_color32(pDes32First, pSrc24Line, nap5, nmp5);
            // 第二场
            nSecondPitch = min(nFirstPitch, nSecondPitch);      // 拷贝用
            nap5 = (nSecondPitch >> 4), nmp5 = (nSecondPitch & 0x0f);
            //memset_dword_line(pDes32Second, CS_RGBA_BLACK_VALUE, na12, nmp);
            memcpy_line(pDes32Second, pDes32First, nap5, nmp5);
        }
    }
}

void TPCS_RGB32_SwapRBSelf(IN OUT unsigned char *pucRGB32,	// RGB32 输入输出（存储方式 X0 X1 X2 A X0 X1 X2 A ……）（R、B 可交换）
						   IN INT_PTR nRGB32Pitch,			// 一行所占字节宽度
						   IN int nWidth,					// 源图像大小
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

// 24 位 RGB/BGR 颜色交换 R、B 通道
void TPCS_RGB24_SwapRBFrame(OUT unsigned char *pucRGBOutFrame,	// 输出缓冲（存储方式 X2 G X0）
						   IN intptr_t nOutPitch,				// 输出缓冲一行所占用的字节数
						   IN unsigned char *pucRGBInFrame,	// 输入缓冲（存储方式 X0 G X2）
						   IN intptr_t nInPitch,				// 输入缓冲一行所占用的字节数
						   IN int nWidth,						// 源图像素矩阵大小
						   IN int nHeight,
						   IN bool bOverturn)
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnRGB24_SwapRB_Frame(pucRGBOutFrame, nOutPitch, pucRGBInFrame, nInPitch, nWidth, nHeight, bOverturn);
	}
}
// 32 位 RGBA/BGRA/YUVA 颜色到 24 位 RGB/BGR/YUV 颜色的变换，直接丢弃 A
void TPCS_32_to_24_Frame(OUT unsigned char *pucCr24Frame,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
						 IN INT_PTR nCr24Pitch,				// 24 位缓存一行所占用的字节数
						 IN unsigned char *pucCr32Frame,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
						 IN INT_PTR nCr32Pitch,				// 32 位缓存一行所占用的字节数
						 IN int nWidth,						// 源图像素矩阵大小
						 IN int nHeight,
						 IN bool bExchangeRB,				// 交换 0、2 字节（R、B 通道）的标志
						 IN bool bOverturn)					// 翻转图像标志
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnCr32_2_Cr24_Frame(pucCr24Frame, nCr24Pitch, pucCr32Frame, nCr32Pitch, nWidth, nHeight, bExchangeRB, bOverturn);
	}
}

// 16 位 YUYV/UYVY 颜色到 32 位 BGRA/RGBA 颜色的变换，A 可填充为统一值
void TPCS_YUV16_2_RGB32_Frame(OUT unsigned char *pucRGB32Frame,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
							  IN intptr_t nRGB32Pitch,				// 32 位缓存一行所占用的字节数
							  IN unsigned char *pucYUV16Frame,		// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
							  IN intptr_t nYUV16Pitch,				// 16 位缓存一行所占用的字节数
							  IN int nWidth,						// 源图像素矩阵大小
							  IN int nHeight,
							  IN bool bHD,	// 清晰度
							  IN bool bBGRA,						// 32 位缓冲的像素格式为 A R G B（B在低位）
							  IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
							  IN unsigned char cAlpha,				// 填充的 A 值
							  IN bool bOverturn)					// 翻转图像标志

				// 翻转图像标志
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnYUV16_2_RGB32_Frame(pucRGB32Frame, nRGB32Pitch, pucYUV16Frame,nYUV16Pitch,nWidth, nHeight,
			bHD ? NXVIDEO_DEFINITION_HIGH : NXVIDEO_DEFINITION_STANDARD, bBGRA, bUYVY, cAlpha, bOverturn);
	}
}

void TPCS_YUV16_Swap_Split(OUT unsigned char *pucYUV16OutTopField,		// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
						   IN intptr_t nOutTopPitch,					// 16 位输出缓冲一行所占用的字节数
						   OUT unsigned char *pucYUV16OutBottomField,	// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
						   IN intptr_t nOutBottomPitch,				// 16 位输出缓冲一行所占用的字节数
						   IN unsigned char *pucYUV16InFrame,			// 16 位输入缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
						   IN intptr_t nInPitch,						// 16 位输入缓冲一行所占用的字节数
						   IN int nWidth,								// 源图像素矩阵大小
						   IN int nHeight,
						   IN bool bOverturn)							// 翻转图像标
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnYUV16_Swap_Split(pucYUV16OutTopField, nOutTopPitch, pucYUV16OutBottomField,nOutBottomPitch,pucYUV16InFrame,nInPitch, nWidth,
			nHeight, bOverturn);
	}
}
//YUVY---uyvy之间转换并copy
void TPCS_YUV16_Swap_Frame(OUT unsigned char *pucYUV16OutFrame,	// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
						   IN intptr_t nOutPitch,										// 16 位输出缓冲一行所占用的字节数
						   IN unsigned char *pucYUV16InFrame,							// 16 位输入缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
						   IN intptr_t nInPitch,										// 16 位输入缓冲一行所占用的字节数
						   IN int nWidth,												// 源图像素矩阵大小
						   IN int nHeight,
						   IN bool bOverturn)										// 翻转图像标志
{
	if(g_lpVideoProcAPI)
	{
		g_lpVideoProcAPI->pStuVideoColorAPI->pfnYUV16_Swap_Frame(pucYUV16OutFrame, nOutPitch, pucYUV16InFrame,nInPitch,nWidth,
			nHeight, bOverturn);
	}
}
//////////////////////////////////////////////////////////////////////////
// 文件结束