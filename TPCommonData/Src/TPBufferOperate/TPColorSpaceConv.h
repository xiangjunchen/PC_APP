//////////////////////////////////////////////////////////////////////////
// 描述：	各个颜色空间之间的转换函数
// 创建：	赵志猛 @ 2007-07-26
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "..\..\..\Include\CoreInclude\CPUInclude\NXColorSpace.h"//add by wangfengming 2011.4.6

//////////////////////////////////////////////////////////////////////////
// 常量定义

#define CS_YUYV_BLACK_VALUE         0x80008000          // YUYV 的黑颜色值
#define CS_RGBA_BLACK_VALUE         0xff000000          // RGBA 或 BGRA 的黑颜色值

//////////////////////////////////////////////////////////////////////////
// 制式定义

enum Definition_Define{
    UNKNOWN_DEFINITION = 0,     // 未知定义
    HIGH_DEFINITION = 1,        // 高清
    STANDARD_DEFINITION = 2,    // 标清
};

//////////////////////////////////////////////////////////////////////////
// 根据制式获取转换矩阵

float* CALLBACK TPCS_GetMatYUVA2RGBA(IN UINT uDefinition);      // 制式
float CALLBACK TPCS_GetMatYUVA2RGBA_Cell(IN int nRow,             // 行
                                         IN int nCol,             // 列
                                         IN UINT uDefinition);    // 制式

float* CALLBACK TPCS_GetMatRGBA2YUVA(IN UINT uDefinition);     // 制式
float CALLBACK TPCS_GetMatRGBA2YUVA_Cell(IN int nRow,             // 行
                                         IN int nCol,             // 列
                                         IN UINT uDefinition);    // 制式

//////////////////////////////////////////////////////////////////////////
// LUVA 与 RGBA 之间的转换

float* CALLBACK TPCS_GetMatLUVA2RGBA();
float* CALLBACK TPCS_GetMatRGBA2LUVA();

//////////////////////////////////////////////////////////////////////////

// 场与帧的转换，会丢掉奇数行的最后一行

void TPCS_ComposeFields(OUT BYTE* pDes,             // 目标
                        IN int nDesPitch,
                        IN BYTE* pSrcTop,           // 上场源
                        IN int nTopPitch,
                        IN BYTE* pSrcBottom,        // 下场源
                        IN int nBottomPitch,
                        IN int nWidth,              // 目标大小
                        IN int nHeight,
                        IN int nPixelBytes);        // 一个像素的大小

//wangfengming 2011.4.6 for OpenFX effect
void TPCvtBGRAandRGBA(OUT unsigned char *pucRGBOutFrame,	// 输出缓冲（存储方式 X2 G X0）
					  IN INT_PTR nOutPitch,				// 输出缓冲一行所占用的字节数
					  IN unsigned char *pucRGBInFrame,	// 输入缓冲（存储方式 X0 G X2）
					  IN INT_PTR nInPitch,				// 输入缓冲一行所占用的字节数
					  IN int nWidth,						// 源图像素矩阵大小
					  IN int nHeight,
					  IN bool bOverturn);
void TPCvtRGB32_2_YUVA(OUT unsigned char *pucYUVFrame,		// 输出缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
					   IN INT_PTR nYUVPitch,				// 输出缓存一行所占用的字节数
					   IN unsigned char *pucRGBFrame,		// 输入缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
					   IN INT_PTR nRGBPitch,				// 输入缓存一行所占用的字节数
					   IN int nWidth,						// 源图像素矩阵大小
					   IN int nHeight,
					   IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
					   IN bool bBGR,						// 输入缓冲的像素格式为 R G B（B在低位）
					   IN bool bOverturn              // 翻转图像标志
					   );           
void TPCvtYUVA2RGB32(OUT unsigned char *pucRGBFrame,		// 输出缓冲（存储方式 X0 X1 X2 . X0 X1 X2 ...）
					 IN INT_PTR nRGBPitch,				// 输出缓存一行所占用的字节数
					 IN unsigned char *pucYUVFrame,		// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
					 IN INT_PTR nYUVAPitch,				// 输入缓存一行所占用的字节数
					 IN int nWidth,						// 源图像素矩阵大小
					 IN int nHeight,
					 IN NXVIDEO_DEFINITION eDefinition,	// 清晰度
					 IN bool bBGR,						// 输出缓冲的像素格式为 R G B（B在低位）的标志
					 IN bool bOverturn
					 );
void TPCvtYUV16_2_YUVA(OUT unsigned char *pucYUVAFrame,	// 32 位缓存（存储方式 Y0 U0 V0 A Y1 U1 V1 A ...）
					   IN INT_PTR nYUVAPitch,			// 32 位缓存一行所占用的字节数
					   IN unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
					   IN INT_PTR nYUV16Pitch,			// 16 位缓存一行所占用的字节数
					   IN int nWidth,					// 源图像素矩阵大小
					   IN int nHeight,
					   IN bool bUYVY,					// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
					   IN unsigned char ucAlpha,			// 填充的 A 值
					   IN bool bOverturn
					   );
void TPCvtYUVA_2_YUV16(OUT unsigned char *pucYUV16Frame,	// 16 位缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
					   IN INT_PTR nYUV16Pitch,			// 16 位缓存一行所占用的字节数
					   IN unsigned char *pucYUVFrame,		// 输入缓存（存储方式 Y0 U0 V0 . Y1 U1 V1 ...）
					   IN INT_PTR nYUVPitch,				// 缓存一行所占用的字节数
					   IN int nWidth,						// 源图像素矩阵大小
					   IN int nHeight,
					   IN bool bUYVY,						// 16 位缓存的像素格式为 Y1 V0 Y0 U0（U0 在低位）
					   IN bool bOverturn
					   );
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
							);
//Directly drag from E7 //wangfengming 2011.4.6 
//[[ljj@2010-2-4: YUYV+A->BGRA/RGBA.如果是要RGBA，请把bDstRGBA = TRUE。
BOOL NX_CvtYUYVA2BGRA_Frame(IN OUT BYTE* pBGRA, IN long lDstPitch, IN BYTE* pYUYV, IN long lSrcPitch, IN BYTE* pAlpha, IN long lAlphaPitch,
							IN long lWidth, IN long lHeight, IN BOOL bDstRGBA, IN BOOL bSrcRevert, IN BOOL bAlphaRevert);
//]]
//wangfengming 2011.4.3

void TPCS_SplitFrame(OUT BYTE* pDesTop,             // 目标上场
                     IN int nTopPitch,
                     OUT BYTE* pDesBottom,          // 目标下场
                     IN int nBottomPitch,
                     IN BYTE* pSrc,                 // 源帧
                     IN int nSrcPitch,
                     IN int nWidth,                 // 源大小
                     IN int nHeight,
                     IN int nPixelBytes);           // 一个像素的大小

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
                           IN int nHeight);
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
                           IN int nHeight);
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
								 IN int nHeight);
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
								 IN int nHeight);

// 场内两行平均插值 UV
void TPCS_422_420_YUVField(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
						   IN int nDesYPitch,
						   OUT BYTE* pDesU,         // U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
						   IN int nDesUPitch,
						   OUT BYTE* pDesV,         // V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
						   IN int nDesVPitch,
						   OUT BYTE* pSrcYUYV,		// 一场 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nSrcPitch,
						   IN int nWidth,           // 源图大小
						   IN int nFieldHeight);
void TPCS_422_420_YUVFrame(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
						   IN int nDesYPitch,
						   OUT BYTE* pDesU,         // U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
						   IN int nDesUPitch,
						   OUT BYTE* pDesV,         // V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
						   IN int nDesVPitch,
						   OUT BYTE* pSrcYUYV,		// 一帧 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						   IN int nSrcPitch,
						   IN int nWidth,           // 源图大小
						   IN int nFrameHeight);
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
							 IN int nFieldHeight);
void TPCS_422_420_YUVFrame_4(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
							 IN int nDesYPitch,
							 OUT BYTE* pDesU,			// U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
							 IN int nDesUPitch,
							 OUT BYTE* pDesV,			// V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
							 IN int nDesVPitch,
							 OUT BYTE* pSrcYUYV,		// 一帧 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							 IN int nSrcPitch,
							 IN int nWidth,				// 源图大小
							 IN int nFrameHeight);
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
								 IN BOOL bFristField);		// 奇偶场标志
void TPCS_422_420_YUVFrame_XDCAM(OUT BYTE* pDesY,			// Y 输出（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
								 IN int nDesYPitch,
								 OUT BYTE* pDesU,			// U 输出（存储方式 UU0 UU2 UU4 ……）（两行共用一个）
								 IN int nDesUPitch,
								 OUT BYTE* pDesV,			// V 输出（存储方式 VV0 VV2 VV4 ……）（两行共用一个）
								 IN int nDesVPitch,
								 OUT BYTE* pSrcYUYV,		// 一帧 YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
								 IN int nSrcPitch,
								 IN int nWidth,				// 源图大小
								 IN int nFrameHeight);

void TPCS_ComposeYUV422_Frame(OUT BYTE* pDes,          // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                              IN int nDesPitch,
                              IN BYTE* pSrcY,          // Y 输入（存储方式 Y0 Y1 Y2 Y3 Y4 ……）
                              IN int nSrcYPitch,
                              IN BYTE* pSrcU,          // U 输入（存储方式 U0 U2 U4 ……）
                              IN int nSrcUPitch,
                              IN BYTE* pSrcV,          // V 输入（存储方式 V0 V2 V4 ……）
                              IN int nSrcVPitch,
                              IN int nWidth,           // 源图大小
                              IN int nHeight);
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
                              IN int nHeight);

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
                            IN BOOL bDesRevert);        // 图像是否行翻转标志
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
                            IN BOOL bDesRevert);        // 图像是否行翻转标志

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
                            IN BOOL bDesRevert);        // 图像是否行翻转标志
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
                            IN BOOL bDesRevert);        // 图像是否行翻转标志

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
                            IN BOOL bDesRevert);        // 图像是否行翻转标志
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
                            IN BOOL bDesRevert);        // 图像是否行翻转标志

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
                            IN BOOL bDesRevert);        // 图像是否行翻转标志
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
                            IN BOOL bDesRevert);        // 图像是否行翻转标志

// YUYVA 与 RGBA、BGRA 之间的转换

void TPCS_RGBA2YUYVA_Frame(OUT BYTE* pDesYUYV,          // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitch,
                           OUT BYTE* pDesAlpha,         // Alpha 输出（存储方式 A0 A1 A2 A3 A4 ……）
                           IN int nAlphaPitch,
                           IN BYTE* pSrcRGBA,           // RGBA 输入（存储方式 R G B A R G B A R G B A ……）
                           IN int nSrcPitch,            // 源一行的字节宽度
                           IN int nWidth,			    // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                 // 高标清标志
                           IN BOOL bSrcRevert);         // 图像是否行翻转标志
void TPCS_RGBA2YUYVA_Split(OUT BYTE* pDesYUYVF,         // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesAlphaF,        // Alpha 第一场输出（存储方式 A0 A1 A2 A3 A4 ……）
                           IN int nAlphaPitchF,
                           OUT BYTE* pDesYUYVS,         // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchS,
                           OUT BYTE* pDesAlphaS,        // Alpha 第二场输出（存储方式 A0 A1 A2 A3 A4 ……）
                           IN int nAlphaPitchS,
                           IN BYTE* pSrcRGBA,           // RGBA 输入（存储方式 R G B A R G B A R G B A ……）
                           IN int nSrcPitch,            // 源一行的字节宽度
                           IN int nWidth,               // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                 // 高标清标志
                           IN BOOL bSrcRevert);         // 图像是否行翻转标志
void TPCS_RGBA2YUYV_Frame(OUT BYTE* pDesYUYV,           // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nYUYVPitch,
                          IN BYTE* pSrcRGBA,            // RGBA 输入（存储方式 R G B A R G B A R G B A ……）
                          IN int nSrcPitch,             // 源一行的字节宽度
                          IN int nWidth,			    // 源图大小
                          IN int nHeight,
                          IN BOOL bHD,                  // 高标清标志
                          IN BOOL bSrcRevert);          // 图像是否行翻转标志
void TPCS_RGBA2YUYV_Split(OUT BYTE* pDesYUYVF,          // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nYUYVPitchF,
                          OUT BYTE* pDesYUYVS,          // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nYUYVPitchS,
                          IN BYTE* pSrcRGBA,            // RGBA 输入（存储方式 R G B A R G B A R G B A ……）
                          IN int nSrcPitch,             // 源一行的字节宽度
                          IN int nWidth,                // 源图大小
                          IN int nHeight,
                          IN BOOL bHD,                  // 高标清标志
                          IN BOOL bSrcRevert);          // 图像是否行翻转标志

void TPCS_BGRA2YUYVA_Frame(OUT BYTE* pDesYUYV,          // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitch,
                           OUT BYTE* pDesAlpha,         // Alpha 输出（存储方式 A0 A1 A2 A3 A4 ……）
                           IN int nAlphaPitch,
                           IN BYTE* pSrcBGRA,           // BGRA 输入（存储方式 B G R A B G R A B G R A ……）
                           IN int nSrcPitch,            // 源一行的字节宽度
                           IN int nWidth,			    // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                 // 高标清标志
                           IN BOOL bSrcRevert);         // 图像是否行翻转标志
void TPCS_BGRA2YUYVA_Split(OUT BYTE* pDesYUYVF,         // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesAlphaF,        // Alpha 第一场输出（存储方式 A0 A1 A2 A3 A4 ……）
                           IN int nAlphaPitchF,
                           OUT BYTE* pDesYUYVS,         // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                           IN int nYUYVPitchS,
                           OUT BYTE* pDesAlphaS,        // Alpha 第二场输出（存储方式 A0 A1 A2 A3 A4 ……）
                           IN int nAlphaPitchS,
                           IN BYTE* pSrcBGRA,           // BGRA 输入（存储方式 B G R A B G R A B G R A ……）
                           IN int nSrcPitch,            // 源一行的字节宽度
                           IN int nWidth,			    // 源图大小
                           IN int nHeight,
                           IN BOOL bHD,                 // 高标清标志
                           IN BOOL bSrcRevert);         // 图像是否行翻转标志
void TPCS_BGRA2YUYV_Frame(OUT BYTE* pDesYUYV,           // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nYUYVPitch,
                          IN BYTE* pSrcBGRA,            // BGRA 输入（存储方式 B G R A B G R A B G R A ……）
                          IN int nSrcPitch,             // 源一行的字节宽度
                          IN int nWidth,			    // 源图大小
                          IN int nHeight,
                          IN BOOL bHD,                  // 高标清标志
                          IN BOOL bSrcRevert);          // 图像是否行翻转标志
void TPCS_BGRA2YUYV_Split(OUT BYTE* pDesYUYVF,          // YUYV 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nYUYVPitchF,
                          OUT BYTE* pDesYUYVS,          // YUYV 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                          IN int nYUYVPitchS,
                          IN BYTE* pSrcBGRA,            // BGRA 输入（存储方式 B G R A B G R A B G R A ……）
                          IN int nSrcPitch,             // 源一行的字节宽度
                          IN int nWidth,                // 源图大小
                          IN int nHeight,
                          IN BOOL bHD,                  // 高标清标志
						  IN BOOL bSrcRevert);          // 图像是否行翻转标志

void TPCS_YUYV2RGBA_Compose(OUT BYTE* pDesRGBA,         // RGBA 输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
							IN int nDesPitch,
							IN BYTE* pSrcYUYVF,         // YUYV 第一场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							IN int nFirstPitch,
							IN BYTE* pSrcYUYVS,         // YUYV 第二场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							IN int nSecondPitch,
							IN int nWidth,              // 目标大小
							IN int nHeight,
							IN BOOL bHD,               // 高标清标志
							IN BOOL bDstRevert = TRUE);	// 图像是否行翻转标志
void TPCS_YUYV2RGBA_Frame(OUT BYTE* pDesRGBA,         // RGBA 输出（存储方式 R G B A R G B A R G B A ……）（Alpha 直接填 0xff）
						  IN int nDesPitch,
						  IN BYTE* pSrcYUYV,         // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						  IN int nSrcPitch,
						  IN int nWidth,              // 目标大小
						  IN int nHeight,
						  IN BOOL bHD,                // 高标清标志
						  IN BOOL bDstRevert = TRUE);	// 图像是否行翻转标志

void TPCS_YUYV2BGRA_Compose(OUT BYTE* pDesBGRA,         // BGRA 输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
							IN int nDesPitch,
							IN BYTE* pSrcYUYVF,         // YUYV 第一场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							IN int nFirstPitch,
							IN BYTE* pSrcYUYVS,         // YUYV 第二场输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
							IN int nSecondPitch,
							IN int nWidth,              // 目标大小
							IN int nHeight,
							IN BOOL bHD,               // 高标清标志
							IN BOOL bDstRevert = TRUE);	// 图像是否行翻转标志
void TPCS_YUYV2BGRA_Frame(OUT BYTE* pDesBGRA,         // BGRA 输出（存储方式 B G R A B G R A B G R A ……）（Alpha 直接填 0xff）
						  IN int nDesPitch,
						  IN BYTE* pSrcYUYV,         // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						  IN int nSrcPitch,
						  IN int nWidth,              // 目标大小
						  IN int nHeight,
						  IN BOOL bHD,                // 高标清标志
						  IN BOOL bDstRevert = TRUE);	// 图像是否行翻转标志

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
								IN BOOL bDesRevert);	// 图像是否行翻转标志

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
								IN BOOL bDesRevert);	// 图像是否行翻转标志


// YUYV 与 RGB、BGR 之间的转换

void TPCS_RGB2YUYV_Frame(OUT BYTE* pDesYUYV,            // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                         IN int nYUYVPitch,
                         IN BYTE* pSrcRGB,              // RGB 输入（存储方式 R G B R G B R G B ……）
                         IN int nSrcPitch,              // 源一行的字节宽度（必须为 4 的倍数）
                         IN int nWidth,		    	    // 源图大小
                         IN int nHeight,
                         IN BOOL bHD,                   // 高标清标志
                         IN BOOL bSrcRevert);           // 图像是否行翻转标志
void TPCS_RGB2YUYV_Split(OUT BYTE* pDesFirst,           // 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                         IN int nFirstPitch,
                         OUT BYTE* pDesSecond,          // 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                         IN int nSecondPitch,
                         IN BYTE* pSrcRGB,              // RGB 输入（存储方式 R G B R G B R G B ……）
                         IN int nSrcPitch,              // 源一行的字节宽度（必须为 4 的倍数）
                         IN int nWidth, 			    // 源图大小
                         IN int nHeight,
                         IN BOOL bHD,                   // 高标清标志
                         IN BOOL bSrcRevert);           // 图像是否行翻转标志

void TPCS_BGR2YUYV_Frame(OUT BYTE* pDesYUYV,            // YUYV 输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                         IN int nYUYVPitch,
                         IN BYTE* pSrcBGR,              // BGR 输入（存储方式 B G R B G R B G R ……）
                         IN int nSrcPitch,              // 源一行的字节宽度（必须为 4 的倍数）
                         IN int nWidth,	    		    // 源图大小
                         IN int nHeight,
                         IN BOOL bHD,                   // 高标清标志
                         IN BOOL bSrcRevert);           // 图像是否行翻转标志
void TPCS_BGR2YUYV_Split(OUT BYTE* pDesFirst,           // 第一场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                         IN int nFirstPitch,
                         OUT BYTE* pDesSecond,          // 第二场输出（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
                         IN int nSecondPitch,
                         IN BYTE* pSrcBGR,              // BGR 输入（存储方式 B G R B G R B G R ……）
                         IN int nSrcPitch,              // 源一行的字节宽度（必须为 4 的倍数）
                         IN int nWidth, 			    // 源图大小
                         IN int nHeight,
                         IN BOOL bHD,                   // 高标清标志
                         IN BOOL bSrcRevert);           // 图像是否行翻转标志

void TPCS_YUYV2BGR_Frame(OUT BYTE* pDesBGR,         // BGR 输出（存储方式 B G R B G R  B G R  ……）
						 IN int nDesPitch,
						 IN BYTE* pSrcYUYV,         // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						 IN int nSrcPitch,
						 IN int nWidth,              // 目标大小
						 IN int nHeight,
						 IN BOOL bHD,                // 高标清标志
						 IN BOOL bDstRevert = TRUE);	// 图像是否行翻转标志

void TPCS_YUYV2RGB_Frame(OUT BYTE* pDesRGB,         // RGB 输出（存储方式 R G B R G B R G B ……）
						 IN int nDesPitch,
						 IN BYTE* pSrcYUYV,         // YUYV 输入（存储方式 Y0 U0 Y1 V0 Y2 U2 Y3 V3 ……）
						 IN int nSrcPitch,
						 IN int nWidth,              // 目标大小
						 IN int nHeight,
						 IN BOOL bHD,                // 高标清标志
						 IN BOOL bDstRevert = TRUE);	// 图像是否行翻转标志


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
						 IN BOOL bSrcAlphaRevert);      // Alpha 是否行翻转标志
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
                         IN BOOL bSrcAlphaRevert);      // Alpha 是否行翻转标志

void TPCS_RGB32_SwapRBSelf(IN OUT unsigned char *pucRGB32,	// RGB32 输入输出（存储方式 X0 X1 X2 A X0 X1 X2 A ……）（R、B 可交换）
						   IN INT_PTR nRGB32Pitch,			// 一行所占字节宽度
						   IN int nWidth,					// 源图像大小
						   IN int nHeight);
// 24 位 RGB/BGR 颜色交换 R、B 通道
void TPCS_RGB24_SwapRBFrame(OUT unsigned char *pucRGBOutFrame,	// 输出缓冲（存储方式 X2 G X0）
						   IN intptr_t nOutPitch,				// 输出缓冲一行所占用的字节数
						   IN unsigned char *pucRGBInFrame,		// 输入缓冲（存储方式 X0 G X2）
						   IN intptr_t nInPitch,				// 输入缓冲一行所占用的字节数
						   IN int nWidth,						// 源图像素矩阵大小
						   IN int nHeight,
						   IN bool bOverturn);
// 32 位 RGBA/BGRA/YUVA 颜色到 24 位 RGB/BGR/YUV 颜色的变换，直接丢弃 A
void TPCS_32_to_24_Frame(OUT unsigned char *pucCr24Frame,	// 24 位缓冲（存储方式 X0 X1 X2 X0 X1 X2 ...）
						 IN INT_PTR nCr24Pitch,				// 24 位缓存一行所占用的字节数
						 IN unsigned char *pucCr32Frame,	// 32 位缓冲（存储方式 X0 X1 X2 A X0 X1 X2 A ...）
						 IN INT_PTR nCr32Pitch,				// 32 位缓存一行所占用的字节数
						 IN int nWidth,						// 源图像素矩阵大小
						 IN int nHeight,
						 IN bool bExchangeRB,				// 交换 0、2 字节（R、B 通道）的标志
						 IN bool bOverturn);				// 翻转图像标志

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
									   IN bool bOverturn);					// 翻转图像标志

//////////////////////////////////////////////////////////////////////////

//YUVY---uyvy之间转换并拆场
 void TPCS_YUV16_Swap_Split(OUT unsigned char *pucYUV16OutTopField,		// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									IN intptr_t nOutTopPitch,					// 16 位输出缓冲一行所占用的字节数
									OUT unsigned char *pucYUV16OutBottomField,	// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									IN intptr_t nOutBottomPitch,				// 16 位输出缓冲一行所占用的字节数
									IN unsigned char *pucYUV16InFrame,			// 16 位输入缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
									IN intptr_t nInPitch,						// 16 位输入缓冲一行所占用的字节数
									IN int nWidth,								// 源图像素矩阵大小
									IN int nHeight,
									IN bool bOverturn);							// 翻转图像标
 //YUVY---uyvy之间转换并copy
 void TPCS_YUV16_Swap_Frame(OUT unsigned char *pucYUV16OutFrame,	// 16 位输出缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
	 IN intptr_t nOutPitch,										// 16 位输出缓冲一行所占用的字节数
	 IN unsigned char *pucYUV16InFrame,							// 16 位输入缓存（存储方式 Y0 U0 Y1 V0 / U0 Y0 V0 Y1）
	 IN intptr_t nInPitch,										// 16 位输入缓冲一行所占用的字节数
	 IN int nWidth,												// 源图像素矩阵大小
	 IN int nHeight,
	 IN bool bOverturn);										// 翻转图像标志

// 文件结束