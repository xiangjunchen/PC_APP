//////////////////////////////////////////////////////////////////////////
// ������	������ɫ�ռ�֮���ת������
// ������	��־�� @ 2007-07-26
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "..\..\..\Include\CoreInclude\CPUInclude\NXColorSpace.h"//add by wangfengming 2011.4.6

//////////////////////////////////////////////////////////////////////////
// ��������

#define CS_YUYV_BLACK_VALUE         0x80008000          // YUYV �ĺ���ɫֵ
#define CS_RGBA_BLACK_VALUE         0xff000000          // RGBA �� BGRA �ĺ���ɫֵ

//////////////////////////////////////////////////////////////////////////
// ��ʽ����

enum Definition_Define{
    UNKNOWN_DEFINITION = 0,     // δ֪����
    HIGH_DEFINITION = 1,        // ����
    STANDARD_DEFINITION = 2,    // ����
};

//////////////////////////////////////////////////////////////////////////
// ������ʽ��ȡת������

float* CALLBACK TPCS_GetMatYUVA2RGBA(IN UINT uDefinition);      // ��ʽ
float CALLBACK TPCS_GetMatYUVA2RGBA_Cell(IN int nRow,             // ��
                                         IN int nCol,             // ��
                                         IN UINT uDefinition);    // ��ʽ

float* CALLBACK TPCS_GetMatRGBA2YUVA(IN UINT uDefinition);     // ��ʽ
float CALLBACK TPCS_GetMatRGBA2YUVA_Cell(IN int nRow,             // ��
                                         IN int nCol,             // ��
                                         IN UINT uDefinition);    // ��ʽ

//////////////////////////////////////////////////////////////////////////
// LUVA �� RGBA ֮���ת��

float* CALLBACK TPCS_GetMatLUVA2RGBA();
float* CALLBACK TPCS_GetMatRGBA2LUVA();

//////////////////////////////////////////////////////////////////////////

// ����֡��ת�����ᶪ�������е����һ��

void TPCS_ComposeFields(OUT BYTE* pDes,             // Ŀ��
                        IN int nDesPitch,
                        IN BYTE* pSrcTop,           // �ϳ�Դ
                        IN int nTopPitch,
                        IN BYTE* pSrcBottom,        // �³�Դ
                        IN int nBottomPitch,
                        IN int nWidth,              // Ŀ���С
                        IN int nHeight,
                        IN int nPixelBytes);        // һ�����صĴ�С

//wangfengming 2011.4.6 for OpenFX effect
void TPCvtBGRAandRGBA(OUT unsigned char *pucRGBOutFrame,	// ������壨�洢��ʽ X2 G X0��
					  IN INT_PTR nOutPitch,				// �������һ����ռ�õ��ֽ���
					  IN unsigned char *pucRGBInFrame,	// ���뻺�壨�洢��ʽ X0 G X2��
					  IN INT_PTR nInPitch,				// ���뻺��һ����ռ�õ��ֽ���
					  IN int nWidth,						// Դͼ���ؾ����С
					  IN int nHeight,
					  IN bool bOverturn);
void TPCvtRGB32_2_YUVA(OUT unsigned char *pucYUVFrame,		// ������棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
					   IN INT_PTR nYUVPitch,				// �������һ����ռ�õ��ֽ���
					   IN unsigned char *pucRGBFrame,		// ���뻺�壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
					   IN INT_PTR nRGBPitch,				// ���뻺��һ����ռ�õ��ֽ���
					   IN int nWidth,						// Դͼ���ؾ����С
					   IN int nHeight,
					   IN NXVIDEO_DEFINITION eDefinition,	// ������
					   IN bool bBGR,						// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
					   IN bool bOverturn              // ��תͼ���־
					   );           
void TPCvtYUVA2RGB32(OUT unsigned char *pucRGBFrame,		// ������壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
					 IN INT_PTR nRGBPitch,				// �������һ����ռ�õ��ֽ���
					 IN unsigned char *pucYUVFrame,		// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
					 IN INT_PTR nYUVAPitch,				// ���뻺��һ����ռ�õ��ֽ���
					 IN int nWidth,						// Դͼ���ؾ����С
					 IN int nHeight,
					 IN NXVIDEO_DEFINITION eDefinition,	// ������
					 IN bool bBGR,						// �����������ظ�ʽΪ R G B��B�ڵ�λ���ı�־
					 IN bool bOverturn
					 );
void TPCvtYUV16_2_YUVA(OUT unsigned char *pucYUVAFrame,	// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
					   IN INT_PTR nYUVAPitch,			// 32 λ����һ����ռ�õ��ֽ���
					   IN unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
					   IN INT_PTR nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
					   IN int nWidth,					// Դͼ���ؾ����С
					   IN int nHeight,
					   IN bool bUYVY,					// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
					   IN unsigned char ucAlpha,			// ���� A ֵ
					   IN bool bOverturn
					   );
void TPCvtYUVA_2_YUV16(OUT unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
					   IN INT_PTR nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
					   IN unsigned char *pucYUVFrame,		// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
					   IN INT_PTR nYUVPitch,				// ����һ����ռ�õ��ֽ���
					   IN int nWidth,						// Դͼ���ؾ����С
					   IN int nHeight,
					   IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
					   IN bool bOverturn
					   );
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
							);
//Directly drag from E7 //wangfengming 2011.4.6 
//[[ljj@2010-2-4: YUYV+A->BGRA/RGBA.�����ҪRGBA�����bDstRGBA = TRUE��
BOOL NX_CvtYUYVA2BGRA_Frame(IN OUT BYTE* pBGRA, IN long lDstPitch, IN BYTE* pYUYV, IN long lSrcPitch, IN BYTE* pAlpha, IN long lAlphaPitch,
							IN long lWidth, IN long lHeight, IN BOOL bDstRGBA, IN BOOL bSrcRevert, IN BOOL bAlphaRevert);
//]]
//wangfengming 2011.4.3

void TPCS_SplitFrame(OUT BYTE* pDesTop,             // Ŀ���ϳ�
                     IN int nTopPitch,
                     OUT BYTE* pDesBottom,          // Ŀ���³�
                     IN int nBottomPitch,
                     IN BYTE* pSrc,                 // Դ֡
                     IN int nSrcPitch,
                     IN int nWidth,                 // Դ��С
                     IN int nHeight,
                     IN int nPixelBytes);           // һ�����صĴ�С

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
                           IN int nHeight);
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
                           IN int nHeight);
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
								 IN int nHeight);
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
								 IN int nHeight);

// ��������ƽ����ֵ UV
void TPCS_422_420_YUVField(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
						   IN int nDesYPitch,
						   OUT BYTE* pDesU,         // U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
						   IN int nDesUPitch,
						   OUT BYTE* pDesV,         // V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
						   IN int nDesVPitch,
						   OUT BYTE* pSrcYUYV,		// һ�� YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nSrcPitch,
						   IN int nWidth,           // Դͼ��С
						   IN int nFieldHeight);
void TPCS_422_420_YUVFrame(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
						   IN int nDesYPitch,
						   OUT BYTE* pDesU,         // U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
						   IN int nDesUPitch,
						   OUT BYTE* pDesV,         // V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
						   IN int nDesVPitch,
						   OUT BYTE* pSrcYUYV,		// һ֡ YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						   IN int nSrcPitch,
						   IN int nWidth,           // Դͼ��С
						   IN int nFrameHeight);
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
							 IN int nFieldHeight);
void TPCS_422_420_YUVFrame_4(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
							 IN int nDesYPitch,
							 OUT BYTE* pDesU,			// U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
							 IN int nDesUPitch,
							 OUT BYTE* pDesV,			// V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
							 IN int nDesVPitch,
							 OUT BYTE* pSrcYUYV,		// һ֡ YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							 IN int nSrcPitch,
							 IN int nWidth,				// Դͼ��С
							 IN int nFrameHeight);
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
								 IN BOOL bFristField);		// ��ż����־
void TPCS_422_420_YUVFrame_XDCAM(OUT BYTE* pDesY,			// Y ������洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
								 IN int nDesYPitch,
								 OUT BYTE* pDesU,			// U ������洢��ʽ UU0 UU2 UU4 �����������й���һ����
								 IN int nDesUPitch,
								 OUT BYTE* pDesV,			// V ������洢��ʽ VV0 VV2 VV4 �����������й���һ����
								 IN int nDesVPitch,
								 OUT BYTE* pSrcYUYV,		// һ֡ YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
								 IN int nSrcPitch,
								 IN int nWidth,				// Դͼ��С
								 IN int nFrameHeight);

void TPCS_ComposeYUV422_Frame(OUT BYTE* pDes,          // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                              IN int nDesPitch,
                              IN BYTE* pSrcY,          // Y ���루�洢��ʽ Y0 Y1 Y2 Y3 Y4 ������
                              IN int nSrcYPitch,
                              IN BYTE* pSrcU,          // U ���루�洢��ʽ U0 U2 U4 ������
                              IN int nSrcUPitch,
                              IN BYTE* pSrcV,          // V ���루�洢��ʽ V0 V2 V4 ������
                              IN int nSrcVPitch,
                              IN int nWidth,           // Դͼ��С
                              IN int nHeight);
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
                              IN int nHeight);

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
                            IN BOOL bDesRevert);        // ͼ���Ƿ��з�ת��־
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
                            IN BOOL bDesRevert);        // ͼ���Ƿ��з�ת��־

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
                            IN BOOL bDesRevert);        // ͼ���Ƿ��з�ת��־
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
                            IN BOOL bDesRevert);        // ͼ���Ƿ��з�ת��־

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
                            IN BOOL bDesRevert);        // ͼ���Ƿ��з�ת��־
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
                            IN BOOL bDesRevert);        // ͼ���Ƿ��з�ת��־

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
                            IN BOOL bDesRevert);        // ͼ���Ƿ��з�ת��־
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
                            IN BOOL bDesRevert);        // ͼ���Ƿ��з�ת��־

// YUYVA �� RGBA��BGRA ֮���ת��

void TPCS_RGBA2YUYVA_Frame(OUT BYTE* pDesYUYV,          // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitch,
                           OUT BYTE* pDesAlpha,         // Alpha ������洢��ʽ A0 A1 A2 A3 A4 ������
                           IN int nAlphaPitch,
                           IN BYTE* pSrcRGBA,           // RGBA ���루�洢��ʽ R G B A R G B A R G B A ������
                           IN int nSrcPitch,            // Դһ�е��ֽڿ��
                           IN int nWidth,			    // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                 // �߱����־
                           IN BOOL bSrcRevert);         // ͼ���Ƿ��з�ת��־
void TPCS_RGBA2YUYVA_Split(OUT BYTE* pDesYUYVF,         // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesAlphaF,        // Alpha ��һ��������洢��ʽ A0 A1 A2 A3 A4 ������
                           IN int nAlphaPitchF,
                           OUT BYTE* pDesYUYVS,         // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchS,
                           OUT BYTE* pDesAlphaS,        // Alpha �ڶ���������洢��ʽ A0 A1 A2 A3 A4 ������
                           IN int nAlphaPitchS,
                           IN BYTE* pSrcRGBA,           // RGBA ���루�洢��ʽ R G B A R G B A R G B A ������
                           IN int nSrcPitch,            // Դһ�е��ֽڿ��
                           IN int nWidth,               // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                 // �߱����־
                           IN BOOL bSrcRevert);         // ͼ���Ƿ��з�ת��־
void TPCS_RGBA2YUYV_Frame(OUT BYTE* pDesYUYV,           // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nYUYVPitch,
                          IN BYTE* pSrcRGBA,            // RGBA ���루�洢��ʽ R G B A R G B A R G B A ������
                          IN int nSrcPitch,             // Դһ�е��ֽڿ��
                          IN int nWidth,			    // Դͼ��С
                          IN int nHeight,
                          IN BOOL bHD,                  // �߱����־
                          IN BOOL bSrcRevert);          // ͼ���Ƿ��з�ת��־
void TPCS_RGBA2YUYV_Split(OUT BYTE* pDesYUYVF,          // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nYUYVPitchF,
                          OUT BYTE* pDesYUYVS,          // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nYUYVPitchS,
                          IN BYTE* pSrcRGBA,            // RGBA ���루�洢��ʽ R G B A R G B A R G B A ������
                          IN int nSrcPitch,             // Դһ�е��ֽڿ��
                          IN int nWidth,                // Դͼ��С
                          IN int nHeight,
                          IN BOOL bHD,                  // �߱����־
                          IN BOOL bSrcRevert);          // ͼ���Ƿ��з�ת��־

void TPCS_BGRA2YUYVA_Frame(OUT BYTE* pDesYUYV,          // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitch,
                           OUT BYTE* pDesAlpha,         // Alpha ������洢��ʽ A0 A1 A2 A3 A4 ������
                           IN int nAlphaPitch,
                           IN BYTE* pSrcBGRA,           // BGRA ���루�洢��ʽ B G R A B G R A B G R A ������
                           IN int nSrcPitch,            // Դһ�е��ֽڿ��
                           IN int nWidth,			    // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                 // �߱����־
                           IN BOOL bSrcRevert);         // ͼ���Ƿ��з�ת��־
void TPCS_BGRA2YUYVA_Split(OUT BYTE* pDesYUYVF,         // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchF,
                           OUT BYTE* pDesAlphaF,        // Alpha ��һ��������洢��ʽ A0 A1 A2 A3 A4 ������
                           IN int nAlphaPitchF,
                           OUT BYTE* pDesYUYVS,         // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                           IN int nYUYVPitchS,
                           OUT BYTE* pDesAlphaS,        // Alpha �ڶ���������洢��ʽ A0 A1 A2 A3 A4 ������
                           IN int nAlphaPitchS,
                           IN BYTE* pSrcBGRA,           // BGRA ���루�洢��ʽ B G R A B G R A B G R A ������
                           IN int nSrcPitch,            // Դһ�е��ֽڿ��
                           IN int nWidth,			    // Դͼ��С
                           IN int nHeight,
                           IN BOOL bHD,                 // �߱����־
                           IN BOOL bSrcRevert);         // ͼ���Ƿ��з�ת��־
void TPCS_BGRA2YUYV_Frame(OUT BYTE* pDesYUYV,           // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nYUYVPitch,
                          IN BYTE* pSrcBGRA,            // BGRA ���루�洢��ʽ B G R A B G R A B G R A ������
                          IN int nSrcPitch,             // Դһ�е��ֽڿ��
                          IN int nWidth,			    // Դͼ��С
                          IN int nHeight,
                          IN BOOL bHD,                  // �߱����־
                          IN BOOL bSrcRevert);          // ͼ���Ƿ��з�ת��־
void TPCS_BGRA2YUYV_Split(OUT BYTE* pDesYUYVF,          // YUYV ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nYUYVPitchF,
                          OUT BYTE* pDesYUYVS,          // YUYV �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                          IN int nYUYVPitchS,
                          IN BYTE* pSrcBGRA,            // BGRA ���루�洢��ʽ B G R A B G R A B G R A ������
                          IN int nSrcPitch,             // Դһ�е��ֽڿ��
                          IN int nWidth,                // Դͼ��С
                          IN int nHeight,
                          IN BOOL bHD,                  // �߱����־
						  IN BOOL bSrcRevert);          // ͼ���Ƿ��з�ת��־

void TPCS_YUYV2RGBA_Compose(OUT BYTE* pDesRGBA,         // RGBA ������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
							IN int nDesPitch,
							IN BYTE* pSrcYUYVF,         // YUYV ��һ�����루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							IN int nFirstPitch,
							IN BYTE* pSrcYUYVS,         // YUYV �ڶ������루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							IN int nSecondPitch,
							IN int nWidth,              // Ŀ���С
							IN int nHeight,
							IN BOOL bHD,               // �߱����־
							IN BOOL bDstRevert = TRUE);	// ͼ���Ƿ��з�ת��־
void TPCS_YUYV2RGBA_Frame(OUT BYTE* pDesRGBA,         // RGBA ������洢��ʽ R G B A R G B A R G B A ��������Alpha ֱ���� 0xff��
						  IN int nDesPitch,
						  IN BYTE* pSrcYUYV,         // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						  IN int nSrcPitch,
						  IN int nWidth,              // Ŀ���С
						  IN int nHeight,
						  IN BOOL bHD,                // �߱����־
						  IN BOOL bDstRevert = TRUE);	// ͼ���Ƿ��з�ת��־

void TPCS_YUYV2BGRA_Compose(OUT BYTE* pDesBGRA,         // BGRA ������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
							IN int nDesPitch,
							IN BYTE* pSrcYUYVF,         // YUYV ��һ�����루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							IN int nFirstPitch,
							IN BYTE* pSrcYUYVS,         // YUYV �ڶ������루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
							IN int nSecondPitch,
							IN int nWidth,              // Ŀ���С
							IN int nHeight,
							IN BOOL bHD,               // �߱����־
							IN BOOL bDstRevert = TRUE);	// ͼ���Ƿ��з�ת��־
void TPCS_YUYV2BGRA_Frame(OUT BYTE* pDesBGRA,         // BGRA ������洢��ʽ B G R A B G R A B G R A ��������Alpha ֱ���� 0xff��
						  IN int nDesPitch,
						  IN BYTE* pSrcYUYV,         // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						  IN int nSrcPitch,
						  IN int nWidth,              // Ŀ���С
						  IN int nHeight,
						  IN BOOL bHD,                // �߱����־
						  IN BOOL bDstRevert = TRUE);	// ͼ���Ƿ��з�ת��־

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
								IN BOOL bDesRevert);	// ͼ���Ƿ��з�ת��־

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
								IN BOOL bDesRevert);	// ͼ���Ƿ��з�ת��־


// YUYV �� RGB��BGR ֮���ת��

void TPCS_RGB2YUYV_Frame(OUT BYTE* pDesYUYV,            // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                         IN int nYUYVPitch,
                         IN BYTE* pSrcRGB,              // RGB ���루�洢��ʽ R G B R G B R G B ������
                         IN int nSrcPitch,              // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                         IN int nWidth,		    	    // Դͼ��С
                         IN int nHeight,
                         IN BOOL bHD,                   // �߱����־
                         IN BOOL bSrcRevert);           // ͼ���Ƿ��з�ת��־
void TPCS_RGB2YUYV_Split(OUT BYTE* pDesFirst,           // ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                         IN int nFirstPitch,
                         OUT BYTE* pDesSecond,          // �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                         IN int nSecondPitch,
                         IN BYTE* pSrcRGB,              // RGB ���루�洢��ʽ R G B R G B R G B ������
                         IN int nSrcPitch,              // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                         IN int nWidth, 			    // Դͼ��С
                         IN int nHeight,
                         IN BOOL bHD,                   // �߱����־
                         IN BOOL bSrcRevert);           // ͼ���Ƿ��з�ת��־

void TPCS_BGR2YUYV_Frame(OUT BYTE* pDesYUYV,            // YUYV ������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                         IN int nYUYVPitch,
                         IN BYTE* pSrcBGR,              // BGR ���루�洢��ʽ B G R B G R B G R ������
                         IN int nSrcPitch,              // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                         IN int nWidth,	    		    // Դͼ��С
                         IN int nHeight,
                         IN BOOL bHD,                   // �߱����־
                         IN BOOL bSrcRevert);           // ͼ���Ƿ��з�ת��־
void TPCS_BGR2YUYV_Split(OUT BYTE* pDesFirst,           // ��һ��������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                         IN int nFirstPitch,
                         OUT BYTE* pDesSecond,          // �ڶ���������洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
                         IN int nSecondPitch,
                         IN BYTE* pSrcBGR,              // BGR ���루�洢��ʽ B G R B G R B G R ������
                         IN int nSrcPitch,              // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
                         IN int nWidth, 			    // Դͼ��С
                         IN int nHeight,
                         IN BOOL bHD,                   // �߱����־
                         IN BOOL bSrcRevert);           // ͼ���Ƿ��з�ת��־

void TPCS_YUYV2BGR_Frame(OUT BYTE* pDesBGR,         // BGR ������洢��ʽ B G R B G R  B G R  ������
						 IN int nDesPitch,
						 IN BYTE* pSrcYUYV,         // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						 IN int nSrcPitch,
						 IN int nWidth,              // Ŀ���С
						 IN int nHeight,
						 IN BOOL bHD,                // �߱����־
						 IN BOOL bDstRevert = TRUE);	// ͼ���Ƿ��з�ת��־

void TPCS_YUYV2RGB_Frame(OUT BYTE* pDesRGB,         // RGB ������洢��ʽ R G B R G B R G B ������
						 IN int nDesPitch,
						 IN BYTE* pSrcYUYV,         // YUYV ���루�洢��ʽ Y0 U0 Y1 V0 Y2 U2 Y3 V3 ������
						 IN int nSrcPitch,
						 IN int nWidth,              // Ŀ���С
						 IN int nHeight,
						 IN BOOL bHD,                // �߱����־
						 IN BOOL bDstRevert = TRUE);	// ͼ���Ƿ��з�ת��־


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
						 IN BOOL bSrcAlphaRevert);      // Alpha �Ƿ��з�ת��־
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
                         IN BOOL bSrcAlphaRevert);      // Alpha �Ƿ��з�ת��־

void TPCS_RGB32_SwapRBSelf(IN OUT unsigned char *pucRGB32,	// RGB32 ����������洢��ʽ X0 X1 X2 A X0 X1 X2 A ��������R��B �ɽ�����
						   IN INT_PTR nRGB32Pitch,			// һ����ռ�ֽڿ��
						   IN int nWidth,					// Դͼ���С
						   IN int nHeight);
// 24 λ RGB/BGR ��ɫ���� R��B ͨ��
void TPCS_RGB24_SwapRBFrame(OUT unsigned char *pucRGBOutFrame,	// ������壨�洢��ʽ X2 G X0��
						   IN intptr_t nOutPitch,				// �������һ����ռ�õ��ֽ���
						   IN unsigned char *pucRGBInFrame,		// ���뻺�壨�洢��ʽ X0 G X2��
						   IN intptr_t nInPitch,				// ���뻺��һ����ռ�õ��ֽ���
						   IN int nWidth,						// Դͼ���ؾ����С
						   IN int nHeight,
						   IN bool bOverturn);
// 32 λ RGBA/BGRA/YUVA ��ɫ�� 24 λ RGB/BGR/YUV ��ɫ�ı任��ֱ�Ӷ��� A
void TPCS_32_to_24_Frame(OUT unsigned char *pucCr24Frame,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
						 IN INT_PTR nCr24Pitch,				// 24 λ����һ����ռ�õ��ֽ���
						 IN unsigned char *pucCr32Frame,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
						 IN INT_PTR nCr32Pitch,				// 32 λ����һ����ռ�õ��ֽ���
						 IN int nWidth,						// Դͼ���ؾ����С
						 IN int nHeight,
						 IN bool bExchangeRB,				// ���� 0��2 �ֽڣ�R��B ͨ�����ı�־
						 IN bool bOverturn);				// ��תͼ���־

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
									   IN bool bOverturn);					// ��תͼ���־

//////////////////////////////////////////////////////////////////////////

//YUVY---uyvy֮��ת������
 void TPCS_YUV16_Swap_Split(OUT unsigned char *pucYUV16OutTopField,		// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									IN intptr_t nOutTopPitch,					// 16 λ�������һ����ռ�õ��ֽ���
									OUT unsigned char *pucYUV16OutBottomField,	// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									IN intptr_t nOutBottomPitch,				// 16 λ�������һ����ռ�õ��ֽ���
									IN unsigned char *pucYUV16InFrame,			// 16 λ���뻺�棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									IN intptr_t nInPitch,						// 16 λ���뻺��һ����ռ�õ��ֽ���
									IN int nWidth,								// Դͼ���ؾ����С
									IN int nHeight,
									IN bool bOverturn);							// ��תͼ���
 //YUVY---uyvy֮��ת����copy
 void TPCS_YUV16_Swap_Frame(OUT unsigned char *pucYUV16OutFrame,	// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
	 IN intptr_t nOutPitch,										// 16 λ�������һ����ռ�õ��ֽ���
	 IN unsigned char *pucYUV16InFrame,							// 16 λ���뻺�棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
	 IN intptr_t nInPitch,										// 16 λ���뻺��һ����ռ�õ��ֽ���
	 IN int nWidth,												// Դͼ���ؾ����С
	 IN int nHeight,
	 IN bool bOverturn);										// ��תͼ���־

// �ļ�����