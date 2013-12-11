//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��ɫ�ռ�ת��.
//
// Created: ZhaoZhimeng @ 2010-05-17
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_COLOR_SPACE_DEF_H__
#define __NX_COLOR_SPACE_DEF_H__

#include ".\NXBasic.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#include <crtdefs.h>
#else	// VC6
#include <stddef.h>		// for intptr_t
#ifndef _INTPTR_T_DEFINED
typedef ptrdiff_t		intptr_t;
#define _INTPTR_T_DEFINED
#endif
#endif		// #if defined(_MSC_VER) && (_MSC_VER >= 1300)

//////////////////////////////////////////////////////////////////////////
// ��������

#ifndef IN
#define IN
#endif		// #ifndef IN
#ifndef OUT
#define OUT
#endif		// #ifndef OUT

#ifndef __in
#define __in
#endif

#ifndef __out
#define __out
#endif

#ifndef __inout
#define __inout
#endif

//////////////////////////////////////////////////////////////////////////
// �ⶨ��

#define NXCOLORSPACE_DLL_NAME			_T("NXColorSpace")
#define NXCOLORSPACE_API_EXT_NAME		"NCS_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// ��Ƶ�źŵ������ȶ���
enum NXVIDEO_DEFINITION
{
	NXVIDEO_DEFINITION_HIGH,				// ����
	NXVIDEO_DEFINITION_STANDARD,			// ����
	NXVIDEO_DEFINITION_ULTRA_HIGH = 3,		// �����壨�Ǻ㶨���ȣ���ʹ���� SDTV �� HDTV ������ͬ�Ĳ����ǹ����ص�ʱ��
	NXVIDEO_DEFINITION_UNKNOWN,
};

// ���÷ֱ��ʶ���
#define NXVIDEO_WIDTH_8K			7680
#define NXVIDEO_WIDTH_4K			3840
#define NXVIDEO_WIDTH_HD			1920
#define NXVIDEO_WIDTH_HDV			1440
#define NXVIDEO_WIDTH_SD			720
#define NXVIDEO_HEIGHT_8K			4320
#define NXVIDEO_HEIGHT_4K			2160
#define NXVIDEO_HEIGHT_HD			1080
#define NXVIDEO_HEIGHT_SD			540
#define NXVIDEO_HEIGHT_SD_PAL		576
#define NXVIDEO_HEIGHT_SD_NTSC		486
#define NXVIDEO_HEIGHT_SD_NTSC_E	480


//////////////////////////////////////////////////////////////////////////
// ��Ƶ�źŵ����ظ�ʽ
enum NXVIDEO_PIXEL_FMT
{
	// 32 λ����
	NXVIDEO_PIXEL_FMT_BGRA,		// A R G B��B �ڵ�λ��
	NXVIDEO_PIXEL_FMT_RGBA,		// A B G R��R �ڵ�λ��
	NXVIDEO_PIXEL_FMT_YUVA,		// A V U Y��Y �ڵ�λ��
	// 24 λ����
	NXVIDEO_PIXEL_FMT_BGR,		// R G B��B �ڵ�λ��
	NXVIDEO_PIXEL_FMT_RGB,		// B G R��R �ڵ�λ��
	NXVIDEO_PIXEL_FMT_YUV,		// V U Y��Y �ڵ�λ��
	// 16 λ����
	NXVIDEO_PIXEL_FMT_YUYV,		// V0 Y1 U0 Y0��Y0 �ڵ�λ��
	NXVIDEO_PIXEL_FMT_UYVY,		// Y1 V0 Y0 U0��U0 �ڵ�λ��
	// ԭʼ�������ݣ����ظ�ͨ����Ƭ�洢��Y һƬ��U һƬ��V һƬ
	NXVIDEO_PIXEL_FMT_YUV422,	// �������ڵ��������ع���
	NXVIDEO_PIXEL_FMT_YUV420,	// �������ڵ�ˮƽ��ֱ�����ĸ����ع���
	// 8 λ����
	NXVIDEO_PIXEL_FMT_KEY,		// ��ֵ
	// V1.0.8.9 ע���о�ָͨ���İ����洢˳��
	NXVIDEO_PIXEL_FMT_ARGB = NXVIDEO_PIXEL_FMT_KEY + 2,	// A R G B *** (for AE)
	NXVIDEO_PIXEL_FMT_ABGR,		// A B G R ***
	NXVIDEO_PIXEL_FMT_YUV444,	// 8bit Y��U��V ��Ƭ���� 4��4��4
	// 8bit ֻ�� UV �ĸ������ʷ�Ƭ���ݣ��� U һƬ��V һƬ
	NXVIDEO_PIXEL_FMT_UV44,
	NXVIDEO_PIXEL_FMT_UV22,
	NXVIDEO_PIXEL_FMT_UV20,
	// 8bit YUV 4:1:1 ��Ƭ����
	NXVIDEO_PIXEL_FMT_YUV411,
	NXVIDEO_PIXEL_FMT_NV12,			// �洢˳��Ϊ Y һƬ��UV �����U �ڵ�λ��
	NXVIDEO_PIXEL_FMT_YV12,			// ��Ƭ�洢��Y һƬ��V һƬ��U һƬ
	NXVIDEO_PIXEL_FMT_UV11,			// YUV 4:1:1 ��Ƭ�洢��U һƬ��V һƬ
	NXVIDEO_PIXEL_FMT_YUV420_P,		// ����֡��ˮƽ��ֱ����Ķ��β���
	NXVIDEO_PIXEL_FMT_UV20_P,
	//////////////////////////////////////////////////////////////////////////
	// YUYV 10bit 8-2 �ͻ������ݣ�Y �ڵ�λ��320bit Ϊһ�� block���� 16 �����أ�
	// ����ǰ 256bit �洢��ͨ���� 8 λ���ݣ��� 64bit �洢��ͨ���� 2 λ���ݣ�
	NXVIDEO_PIXEL_FMT_YUYV10_8_2 = NXVIDEO_PIXEL_FMT_ABGR + 1000,
	// 10bit ���ظ�ʽ���壬ʹ�� 16bit �ĸ� 10 λ�洢
	NXVIDEO_PIXEL_FMT_YUYV10,		// Y0 U0 Y1 V0 ***
	NXVIDEO_PIXEL_FMT_UYVY10,		// U0 Y0 V0 Y1 ***
	NXVIDEO_PIXEL_FMT_YUVA10,		// Y U V A ***
	NXVIDEO_PIXEL_FMT_RGBA10,		// R G B A ***
	NXVIDEO_PIXEL_FMT_BGRA10,		// B G R A ***
	NXVIDEO_PIXEL_FMT_ARGB10,		// A R G B ***
	NXVIDEO_PIXEL_FMT_ABGR10,		// A B G R ***
	NXVIDEO_PIXEL_FMT_RGB10,		// R G B ***
	NXVIDEO_PIXEL_FMT_BGR10,		// B G R ***
	NXVIDEO_PIXEL_FMT_YUV10,		// Y U V ***
	NXVIDEO_PIXEL_FMT_YUV422P10,	// Y��U��V ��Ƭ���� 4��2��2
	NXVIDEO_PIXEL_FMT_YUV420P10,	// Y��U��V ��Ƭ���� 4��2��0
	NXVIDEO_PIXEL_FMT_10BIT_LEAST,	// ʹ�� 16bit �ĵ� 10bit
	NXVIDEO_PIXEL_FMT_10BIT_MOST,	// ʹ�� 16bit �ĸ� 10bit
	NXVIDEO_PIXEL_FMT_YUV444P10,	// 10bit Y��U��V ��Ƭ���� 4��4��4
	// 10bit ֻ�� UV �ĸ������ʷ�Ƭ���ݣ��� U һƬ��V һƬ
	NXVIDEO_PIXEL_FMT_UV44_10,
	NXVIDEO_PIXEL_FMT_UV22_10,
	NXVIDEO_PIXEL_FMT_UV20_10,
	// U Y V Y *** 10bit ��ʽ�洢��ʹ�� 32bit �ĵ� 30bit���� 2bit �� 0����С�ˣ�
	NXVIDEO_PIXEL_FMT_V210_LE,
	NXVIDEO_PIXEL_FMT_KEY10,		// ��ֵ
	NXVIDEO_PIXEL_FMT_UV11_10,		// YUV 4:1:1 ��Ƭ�洢��U һƬ��V һƬ
	NXVIDEO_PIXEL_FMT_YUV420P10_P,	// ����֡��ˮƽ��ֱ����Ķ��β���
	NXVIDEO_PIXEL_FMT_UV20_10_P,
	//////////////////////////////////////////////////////////////////////////
	// �°汾��������￪ʼ
	NXVIDEO_PIXEL_FMT_UNKNOWN = 0x7fffffff
};

// ��ɫ�ռ��װ����
// ������ݣ�ʹ�� color �洢����
// ��Ƭ���ݣ��� color��cb��cr ��˳��洢����
struct color_wrapper
{
	unsigned char*		color;			// ��ɫͨ��
	intptr_t			color_pitch;	// ��ɫͨ��һ�е��ֽ���
	unsigned char*		cb;				// U ��Ƭ����
	intptr_t			cb_pitch;		// U ��Ƭ����һ�е��ֽ���
	unsigned char*		cr;				// V ��Ƭ����
	intptr_t			cr_pitch;		// V ��Ƭ����һ�е��ֽ���
	unsigned char*		key;			// A ͨ�����������ظ�ʽδ�ã��粻�� A �����޷���� A ͨ����
	intptr_t			key_pitch;		// A ͨ��һ�е��ֽ���
	NXVIDEO_PIXEL_FMT	pixel_fmt;		// ���ظ�ʽ
};

//////////////////////////////////////////////////////////////////////////
// �����ӿ�ͨ�ö���

// 24/32 λ RGB/BGR ��ɫ���� R��B ͨ��
typedef void (*DefRGB_SwapRB_Frame)(OUT unsigned char *pucRGBOutFrame,	// ������壨�洢��ʽ X2 G X0��
									IN intptr_t nOutPitch,				// �������һ����ռ�õ��ֽ���
									IN unsigned char *pucRGBInFrame,	// ���뻺�壨�洢��ʽ X0 G X2��
									IN intptr_t nInPitch,				// ���뻺��һ����ռ�õ��ֽ���
									IN int nWidth,						// Դͼ���ؾ����С
									IN int nHeight,
									IN bool bOverturn);					// ��תͼ���־
typedef void (*DefRGB_SwapRB_Split)(OUT unsigned char *pucRGBOutTopField,		// ����������洢��ʽ X2 G X0��
									IN intptr_t nOutTopPitch,					// �������һ����ռ�õ��ֽ���
									OUT unsigned char *pucRGBOutBottomField,	// ����׳����洢��ʽ X2 G X0��
									IN intptr_t nOutBottomPitch,				// ����׳�һ����ռ�õ��ֽ���
									IN unsigned char *pucRGBInFrame,			// ���뻺�壨�洢��ʽ X0 G X2��
									IN intptr_t nInPitch,						// ���뻺��һ����ռ�õ��ֽ���
									IN int nWidth,								// Դͼ���ؾ����С
									IN int nHeight,
									IN bool bOverturn);							// ��תͼ���־
typedef void (*DefRGB_SwapRB_Compose)(OUT unsigned char *pucRGBOutFrame,		// ������壨�洢��ʽ X2 G X0��
									  IN intptr_t nOutPitch,					// �������һ����ռ�õ��ֽ���
									  IN unsigned char *pucRGBInTopField,		// ���붥�����洢��ʽ X0 G X2��
									  IN intptr_t nInTopPitch,					// ���붥��һ����ռ�õ��ֽ���
									  IN unsigned char *pucRGBInBottomField,	// ����׳����洢��ʽ X0 G X2��
									  IN intptr_t nInBottomPitch,				// ����׳�һ����ռ�õ��ֽ���
									  IN int nWidth,							// Դͼ���ؾ����С
									  IN int nHeight,
									  IN bool bOverturn);						// ��תͼ���־

// 24/32 λ BGR/RGB ��ɫ�� 24 λ YUV ��ɫ�ı任������� A ��ֱ�Ӷ���
// 32 λ BGRA/RGBA ��ɫ�� 32 λ YUVA ��ɫ�ı任
typedef void (*DefRGB_2_YUV_Frame)(OUT unsigned char *pucYUVFrame,		// ������棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
								   IN intptr_t nYUVPitch,				// �������һ����ռ�õ��ֽ���
								   IN unsigned char *pucRGBFrame,		// ���뻺�壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
								   IN intptr_t nRGBPitch,				// ���뻺��һ����ռ�õ��ֽ���
								   IN int nWidth,						// Դͼ���ؾ����С
								   IN int nHeight,
								   IN NXVIDEO_DEFINITION eDefinition,	// ������
								   IN bool bBGR,						// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
								   IN bool bOverturn);					// ��תͼ���־
typedef void (*DefRGB_2_YUV_Split)(OUT unsigned char *pucYUVTopField,		// ����������洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
								   IN intptr_t nYUVTopPitch,				// �������һ����ռ�õ��ֽ���
								   OUT unsigned char *pucYUVBottomField,	// ����׳����洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
								   IN intptr_t nYUVBottomPitch,				// ����׳�һ����ռ�õ��ֽ���
								   IN unsigned char *pucRGBFrame,			// ����֡���洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
								   IN intptr_t nRGBPitch,					// ����֡һ����ռ�õ��ֽ���
								   IN int nWidth,							// Դͼ���ؾ����С
								   IN int nHeight,
								   IN NXVIDEO_DEFINITION eDefinition,		// ������
								   IN bool bBGR,							// ����֡�����ظ�ʽΪ R G B��B�ڵ�λ��
								   IN bool bOverturn);						// ��תͼ���־
typedef void (*DefRGB_2_YUV_Compose)(OUT unsigned char *pucYUVFrame,		// ���֡���洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
									 IN intptr_t nYUVAPitch,				// ���֡һ����ռ�õ��ֽ���
									 IN unsigned char *pucRGBTopField,		// ���붥�����洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
									 IN intptr_t nRGBTopPitch,				// ���붥��һ����ռ�õ��ֽ���
									 IN unsigned char *pucRGBBottomField,	// ����׳����洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
									 IN intptr_t nRGB24BottomPitch,			// ����׳�һ����ռ�õ��ֽ���
									 IN int nWidth,							// Դͼ���ؾ����С
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,		// ������
									 IN bool bBGR,							// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
									 IN bool bOverturn);					// ��תͼ���־

// 24/32 λ BGR/RGB ��ɫ�� 16 λ YUYV/UYVY ��ɫ�ı任������� A ֱ�Ӷ�������ֱ�Ӷ����ڶ��� UV
typedef void (*DefRGB_2_YUV16_Frame)(OUT unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									 IN intptr_t nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucRGBFrame,		// ���뻺�壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
									 IN intptr_t nRGBPitch,				// ���뻺��һ����ռ�õ��ֽ���
									 IN int nWidth,						// Դͼ���ؾ����С
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,	// ������
									 IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									 IN bool bBGR,						// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
									 IN bool bOverturn);				// ��תͼ���־
typedef void (*DefRGB_2_YUV16_Split)(OUT unsigned char *pucYUV16TopField,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									 IN intptr_t nYUV16TopPitch,				// 16 λ����һ����ռ�õ��ֽ���
									 OUT unsigned char *pucYUV16BottomField,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									 IN intptr_t nYUV16BottomPitch,				// 16 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucRGBFrame,				// ���뻺�壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
									 IN intptr_t nRGBPitch,						// ���뻺��һ����ռ�õ��ֽ���
									 IN int nWidth,								// Դͼ���ؾ����С
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,			// ������
									 IN bool bUYVY,								// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									 IN bool bBGR,								// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
									 IN bool bOverturn);						// ��תͼ���־
typedef void (*DefRGB_2_YUV16_Compose)(OUT unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									   IN intptr_t nYUV16Pitch,				// 16 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucRGBTopField,	// ���붥�����洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
									   IN intptr_t nRGB32TopPitch,			// ���붥��һ����ռ�õ��ֽ���
									   IN unsigned char *pucRGBBottomField,	// ����׳����洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
									   IN intptr_t nRGBBottomPitch,			// ����׳�һ����ռ�õ��ֽ���
									   IN int nWidth,						// Դͼ���ؾ����С
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,	// ������
									   IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									   IN bool bBGR,						// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
									   IN bool bOverturn);					// ��תͼ���־

// 24/32 λ BGR/RGB ��ɫ����Ƭ YUV422/YUV420 �ı任������� A ��ֱ�Ӷ�������ֱ�Ӷ����ڶ��� UV��YUV420 ʱ UV �����в�ֵ��һ��
typedef void (*DefRGB_2_YUVSlice_Frame)(OUT unsigned char *pucYFrame,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										IN intptr_t nYPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucUUFrame,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										IN intptr_t nUUPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucVVFrame,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										IN intptr_t nVVPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										IN unsigned char *pucRGBFrame,		// ���뻺�壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
										IN intptr_t nRGBPitch,				// ���뻺��һ����ռ�õ��ֽ���
										IN int nWidth,						// Դͼ���ؾ����С
										IN int nHeight,
										IN NXVIDEO_DEFINITION eDefinition,	// ������
										IN bool bBGR,						// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ���ı�־
										IN bool bOverturn);					// ��תͼ���־
typedef void (*DefRGB_2_YUVSlice_Split)(OUT unsigned char *pucYTopField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										IN intptr_t nYTopPitch,					// ��Ƭ Y ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucYBottomField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										IN intptr_t nYBottomPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucUUTopField,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										IN intptr_t nUUTopPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucUUBottomField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										IN intptr_t nUUBottomPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucVVTopField,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										IN intptr_t nVVTopPitch,					// ��Ƭ V ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucVVBottomField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										IN intptr_t nVVBottomPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										IN unsigned char *pucRGBFrame,			// ����֡���洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
										IN intptr_t nRGBPitch,					// ����֡һ����ռ�õ��ֽ���
										IN int nWidth,							// Դͼ���ؾ����С
										IN int nHeight,
										IN NXVIDEO_DEFINITION eDefinition,		// ������
										IN bool bBGR,							// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ���ı�־
										IN bool bOverturn);						// ��תͼ���־
typedef void (*DefRGB_2_YUVSlice_Compose)(OUT unsigned char *pucYFrame,			// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYPitch,					// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucUUFrame,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUPitch,					// ��Ƭ U ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucVVFrame,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVPitch,					// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucRGBTopField,		// ���붥�����洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
										  IN intptr_t nRGBTopPitch,				// ���붥��һ����ռ�õ��ֽ���
										  IN unsigned char *pucRGBBottomField,	// ����׳����洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
										  IN intptr_t nRGBBottomPitch,			// ����׳�һ����ռ�õ��ֽ���
										  IN int nWidth,						// Դͼ���ؾ����С
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,	// ������
										  IN bool bBGRA,						// ���뻺������ظ�ʽΪ R G B��B�ڵ�λ���ı�־
										  IN bool bOverturn);					// ��תͼ���־

// 32 λ YUVA ��ɫ�� 32 λ BGRA/RGBA ��ɫ�ı任
// 24/32 λ YUV/YUVA ��ɫ�� 24 λ BGR/RGB ��ɫ�ı任������� A ֱ�Ӷ���
typedef void (*DefYUV_2_RGB_Frame)(OUT unsigned char *pucRGBFrame,		// ������壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
								   IN intptr_t nRGBPitch,				// �������һ����ռ�õ��ֽ���
								   IN unsigned char *pucYUVFrame,		// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
								   IN intptr_t nYUVAPitch,				// ���뻺��һ����ռ�õ��ֽ���
								   IN int nWidth,						// Դͼ���ؾ����С
								   IN int nHeight,
								   IN NXVIDEO_DEFINITION eDefinition,	// ������
								   IN bool bBGR,						// �����������ظ�ʽΪ R G B��B�ڵ�λ���ı�־
								   IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUV_2_RGB_Split)(OUT unsigned char *pucRGBTopField,		// ����������洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
								   IN intptr_t nRGBTopPitch,				// �������һ����ռ�õ��ֽ���
								   OUT unsigned char *pucRGBBottomField,	// ����׳����洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
								   IN intptr_t nRGBBottomPitch,				// ����׳�һ����ռ�õ��ֽ���
								   IN unsigned char *pucYUVAFrame,			// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
								   IN intptr_t nYUVAPitch,					// ���뻺��һ����ռ�õ��ֽ���
								   IN int nWidth,							// Դͼ���ؾ����С
								   IN int nHeight,
								   IN NXVIDEO_DEFINITION eDefinition,		// ������
								   IN bool bBGR,							// �����������ظ�ʽΪ R G B��B�ڵ�λ��
								   IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUV_2_RGB_Compose)(OUT unsigned char *pucRGBFrame,		// ������壨�洢��ʽ X0 X1 X2 . X0 X1 X2 ...��
									 IN intptr_t nRGBPitch,					// �������һ����ռ�õ��ֽ���
									 IN unsigned char *pucYUVATopField,		// ���붥�����洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
									 IN intptr_t nYUVATopPitch,				// ���붥��һ����ռ�õ��ֽ���
									 IN unsigned char *pucYUVABottomField,	// ����׳����洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
									 IN intptr_t nYUVABottomPitch,			// ����׳�һ����ռ�õ��ֽ���
									 IN int nWidth,							// Դͼ���ؾ����С
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,		// ������
									 IN bool bBGR,							// �����������ظ�ʽΪ R G B��B�ڵ�λ��
									 IN bool bOverturn);					// ��תͼ���־

// 24/32 λ YUV/YUVA ��ɫ�� 16 λ YUYV/UYVY ��ɫ�ı任��ֱ�Ӷ����ڶ��� UV������� A ��ֱ�Ӷ���
typedef void (*DefYUV_2_YUV16_Frame)(OUT unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									 IN intptr_t nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucYUVFrame,		// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
									 IN intptr_t nYUVPitch,				// ����һ����ռ�õ��ֽ���
									 IN int nWidth,						// Դͼ���ؾ����С
									 IN int nHeight,
									 IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									 IN bool bOverturn);				// ��תͼ���־
typedef void (*DefYUV_2_YUV16_Split)(OUT unsigned char *pucYUV16TopField,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									 IN intptr_t nYUV16TopPitch,				// 16 λ����һ����ռ�õ��ֽ���
									 OUT unsigned char *pucYUV16BottomField,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									 IN intptr_t nYUV16BottomPitch,				// 16 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucYUVFrame,				// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
									 IN intptr_t nYUVPitch,						// ���뻺��һ����ռ�õ��ֽ���
									 IN int nWidth,								// Դͼ���ؾ����С
									 IN int nHeight,
									 IN bool bUYVY,								// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									 IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUV_2_YUV16_Compose)(OUT unsigned char *pucYUV16Frame,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									   IN intptr_t nYUV16Pitch,					// 16 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUVTopField,		// ���붥�����洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
									   IN intptr_t nYUVTopPitch,				// ���붥��һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUVBottomField,		// ����׳����洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
									   IN intptr_t nYUVBottomPitch,				// ����׳�һ����ռ�õ��ֽ���
									   IN int nWidth,							// Դͼ���ؾ����С
									   IN int nHeight,
									   IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									   IN bool bOverturn);						// ��תͼ���־

// ��Ƭ YUV422/YUV420(8Bit/10Bit) �� 16 λ YUYV/UYVY ��ɫ�ı任���ڶ��� UV ֵ���������� UV ���в�ֵ
typedef void (*DefYUVSlice_2_YUV16_Frame)(OUT unsigned char *pucYUV16Frame,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										  IN intptr_t nYUV16Pitch,				// 16 λ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucYFrame,			// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYPitch,					// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucUUFrame,			// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUPitch,					// ��Ƭ U ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucVVFrame,			// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVPitch,					// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN int nWidth,						// Դͼ���ؾ����С
										  IN int nHeight,
										  IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
										  IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUVSlice_2_YUV16_Split)(OUT unsigned char *pucYUV16TopField,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										  IN intptr_t nYUV16TopPitch,				// 16 λ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucYUV16BottomField,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										  IN intptr_t nYUV16BottomPitch,			// 16 λ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucYFrame,				// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYPitch,						// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucUUFrame,				// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUPitch,						// ��Ƭ U ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucVVFrame,				// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVPitch,						// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN int nWidth,							// Դͼ���ؾ����С
										  IN int nHeight,
										  IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
										  IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUVSlice_2_YUV16_Compose)(OUT unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
											IN intptr_t nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
											IN unsigned char *pucYTopField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
											IN intptr_t nYTopPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
											IN unsigned char *pucYBottomField,	// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
											IN intptr_t nYBottomPitch,			// ��Ƭ Y ����һ����ռ�õ��ֽ���
											IN unsigned char *pucUUTopField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUTopPitch,			// ��Ƭ U ����һ����ռ�õ��ֽ���
											IN unsigned char *pucUUBottomField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUBottomPitch,			// ��Ƭ U ����һ����ռ�õ��ֽ���
											IN unsigned char *pucVVTopField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVTopPitch,			// ��Ƭ V ����һ����ռ�õ��ֽ���
											IN unsigned char *pucVVBottomField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVBottomPitch,			// ��Ƭ V ����һ����ռ�õ��ֽ���
											IN int nWidth,						// Դͼ���ؾ����С
											IN int nHeight,
											IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
											IN bool bOverturn);					// ��תͼ���־

// ��Ƭ YUV422/YUV420 �� 32 λ YUVA ��ɫ�ı任��A �����Ϊͳһֵ���ڶ��� UV ֵ����ǰ�� UV ���в�ֵ���ڶ��� UV ֵ���������� UV ���в�ֵ
typedef void (*DefYUVSlice_2_YUVA_Frame)(OUT unsigned char *pucYUVAFrame,	// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
										 IN intptr_t nYUVAPitch,			// 32 λ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucYFrame,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										 IN intptr_t nYPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucUUFrame,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										 IN intptr_t nUUPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucVVFrame,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										 IN intptr_t nVVPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										 IN int nWidth,						// Դͼ���ؾ����С
										 IN int nHeight,
										 IN unsigned char cAlpha,			// ���� A ֵ
										 IN bool bOverturn);				// ��תͼ���־
typedef void (*DefYUVSlice_2_YUVA_Split)(OUT unsigned char *pucYUVATopField,		// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
										 IN intptr_t nYUVATopPitch,					// 32 λ����һ����ռ�õ��ֽ���
										 OUT unsigned char *pucYUVABottomField,		// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
										 IN intptr_t nYUVABottomPitch,				// 32 λ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucYFrame,				// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										 IN intptr_t nYPitch,						// ��Ƭ Y ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucUUFrame,				// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										 IN intptr_t nUUPitch,						// ��Ƭ U ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucVVFrame,				// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										 IN intptr_t nVVPitch,						// ��Ƭ V ����һ����ռ�õ��ֽ���
										 IN int nWidth,								// Դͼ���ؾ����С
										 IN int nHeight,
										 IN unsigned char cAlpha,					// ���� A ֵ
										 IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUVSlice_2_YUVA_Compose)(OUT unsigned char *pucYUVAFrame,		// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
										   IN intptr_t nYUVAPitch,				// 32 λ����һ����ռ�õ��ֽ���
										   IN unsigned char *pucYTopField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										   IN intptr_t nYTopPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
										   IN unsigned char *pucYBottomField,	// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										   IN intptr_t nYBottomPitch,			// ��Ƭ Y ����һ����ռ�õ��ֽ���
										   IN unsigned char *pucUUTopField,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										   IN intptr_t nUUTopPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										   IN unsigned char *pucUUBottomField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										   IN intptr_t nUUBottomPitch,			// ��Ƭ U ����һ����ռ�õ��ֽ���
										   IN unsigned char *pucVVTopField,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										   IN intptr_t nVVTopPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										   IN unsigned char *pucVVBottomField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										   IN intptr_t nVVBottomPitch,			// ��Ƭ V ����һ����ռ�õ��ֽ���
										   IN int nWidth,						// Դͼ���ؾ����С
										   IN int nHeight,
										   IN unsigned char cAlpha,				// ���� A ֵ
										   IN bool bOverturn);					// ��תͼ���־

// ��Ƭ YUV422/YUV420 �� 24 λ YUV ��ɫ�ı任���ڶ��� UV ֵ����ǰ�� UV ���в�ֵ���ڶ��� UV ֵ���������� UV ���в�ֵ
typedef void (*DefYUVSlice_2_YUV_Frame)(OUT unsigned char *pucYUVFrame,		// 24 λ���棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
										IN intptr_t nYUVPitch,				// 24 λ����һ����ռ�õ��ֽ���
										IN unsigned char *pucYFrame,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										IN intptr_t nYPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
										IN unsigned char *pucUUFrame,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										IN intptr_t nUUPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										IN unsigned char *pucVVFrame,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										IN intptr_t nVVPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										IN int nWidth,						// Դͼ���ؾ����С
										IN int nHeight,
										IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUVSlice_2_YUV_Split)(OUT unsigned char *pucYUVTopField,		// 24 λ���棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
										IN intptr_t nYUVTopPitch,				// 24 λ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucYUVBottomField,	// 24 λ���棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
										IN intptr_t nYUVBottomPitch,			// 24 λ����һ����ռ�õ��ֽ���
										IN unsigned char *pucYFrame,			// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										IN intptr_t nYPitch,					// ��Ƭ Y ����һ����ռ�õ��ֽ���
										IN unsigned char *pucUUFrame,			// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										IN intptr_t nUUPitch,					// ��Ƭ U ����һ����ռ�õ��ֽ���
										IN unsigned char *pucVVFrame,			// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										IN intptr_t nVVPitch,					// ��Ƭ V ����һ����ռ�õ��ֽ���
										IN int nWidth,							// Դͼ���ؾ����С
										IN int nHeight,
										IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUVSlice_2_YUV_Compose)(OUT unsigned char *pucYUVFrame,		// 24 λ���棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
										  IN intptr_t nYUVPitch,				// 24 λ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucYTopField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYTopPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucYBottomField,	// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYBottomPitch,			// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucUUTopField,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUTopPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucUUBottomField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUBottomPitch,			// ��Ƭ U ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucVVTopField,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVTopPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucVVBottomField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVBottomPitch,			// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN int nWidth,						// Դͼ���ؾ����С
										  IN int nHeight,
										  IN bool bOverturn);					// ��תͼ���־

// ��Ƭ YUV422/YUV420 ֮��� UV �任�������в�ֵ��һ��
typedef void (*DefYUVSlice_ConvertUV_Frame)(OUT unsigned char *pucUUOutFrame,	// ��Ƭ U ������壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUOutPitch,			// ��Ƭ U �������һ����ռ�õ��ֽ���
											OUT unsigned char *pucVVOutFrame,	// ��Ƭ V ������壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVOutPitch,			// ��Ƭ V �������һ����ռ�õ��ֽ���
											IN unsigned char *pucUUInFrame,		// ��Ƭ U ���뻺�壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUInPitch,				// ��Ƭ U ���뻺��һ����ռ�õ��ֽ���
											OUT unsigned char *pucVVInFrame,	// ��Ƭ V ���뻺�壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVInPitch,				// ��Ƭ V ���뻺��һ����ռ�õ��ֽ���
											IN int nWidth,						// Դͼ���ؾ����С
											IN int nHeight,
											IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUVSlice_ConvertUV_Split)(OUT unsigned char *pucUUOutTopField,	// ��Ƭ U ������壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUOutTopPitch,				// ��Ƭ U �������һ����ռ�õ��ֽ���
											OUT unsigned char *pucUUOutBottomField,	// ��Ƭ U ������壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUOutBottomPitch,			// ��Ƭ U �������һ����ռ�õ��ֽ���
											OUT unsigned char *pucVVOutTopField,	// ��Ƭ V ������壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVOutTopPitch,				// ��Ƭ V �������һ����ռ�õ��ֽ���
											OUT unsigned char *pucVVOutBottomField,	// ��Ƭ V ������壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVOutBottomPitch,			// ��Ƭ V �������һ����ռ�õ��ֽ���
											IN unsigned char *pucUUInFrame,			// ��Ƭ U ���뻺�壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUInPitch,					// ��Ƭ U ���뻺��һ����ռ�õ��ֽ���
											OUT unsigned char *pucVVInFrame,		// ��Ƭ V ���뻺�壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVInPitch,					// ��Ƭ V ���뻺��һ����ռ�õ��ֽ���
											IN int nWidth,							// Դͼ���ؾ����С
											IN int nHeight,
											IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUVSlice_ConvertUV_Compose)(OUT unsigned char *pucUUOutFrame,			// ��Ƭ U ������壨�洢��ʽ UU0 UU2 UU4 ������
											  IN intptr_t nUUOutPitch,					// ��Ƭ U �������һ����ռ�õ��ֽ���
											  OUT unsigned char *pucVVOutFrame,			// ��Ƭ V ������壨�洢��ʽ VV0 VV2 VV4 ������
											  IN intptr_t nVVOutPitch,					// ��Ƭ V �������һ����ռ�õ��ֽ���
											  IN unsigned char *pucUUInTopField,		// ��Ƭ U ���뻺�壨�洢��ʽ UU0 UU2 UU4 ������
											  IN intptr_t nUUInTopPitch,				// ��Ƭ U ���뻺��һ����ռ�õ��ֽ���
											  IN unsigned char *pucUUInBottomField,		// ��Ƭ U ���뻺�壨�洢��ʽ UU0 UU2 UU4 ������
											  IN intptr_t nUUInBottomPitch,				// ��Ƭ U ���뻺��һ����ռ�õ��ֽ���
											  IN unsigned char *pucVVInTopField,		// ��Ƭ V ���뻺�壨�洢��ʽ VV0 VV2 VV4 ������
											  IN intptr_t nVVInTopPitch,				// ��Ƭ V ���뻺��һ����ռ�õ��ֽ���
											  IN unsigned char *pucVVInBottomField,		// ��Ƭ V ���뻺�壨�洢��ʽ VV0 VV2 VV4 ������
											  IN intptr_t nVVInBottomPitch,				// ��Ƭ V ���뻺��һ����ռ�õ��ֽ���
											  IN int nWidth,							// Դͼ���ؾ����С
											  IN int nHeight,
											  IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUVSlice_ConvertUV_Field)(OUT unsigned char *pucUUOutField,	// ��Ƭ U ������壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUOutPitch,			// ��Ƭ U �������һ����ռ�õ��ֽ���
											OUT unsigned char *pucVVOutField,	// ��Ƭ V ������壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVOutPitch,			// ��Ƭ V �������һ����ռ�õ��ֽ���
											IN unsigned char *pucUUInField,		// ��Ƭ U ���뻺�壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUInPitch,				// ��Ƭ U ���뻺��һ����ռ�õ��ֽ���
											OUT unsigned char *pucVVInField,	// ��Ƭ V ���뻺�壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVInPitch,				// ��Ƭ V ���뻺��һ����ռ�õ��ֽ���
											IN int nWidth,						// Դͼ�����ؾ����С
											IN int nFieldHeight,
											IN bool bTopField,					// �����任��־��������׳��ı任��ͬ��
											IN bool bOverturn);					// ��תͼ���־

// ��Ƭ YUV422/YUV420 �� 32 λ BGRA/RGBA ��ɫ�ı任��A �����Ϊͳһֵ���ڶ��� UV ֵ����ǰ�� UV ���в�ֵ���ڶ��� UV ֵ���������� UV ���в�ֵ
typedef void (*DefYUVSlice_2_RGB32_Frame)(OUT unsigned char *pucRGB32Frame,		// 32 λ������壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
										  IN intptr_t nRGB32Pitch,				// 32 λ�������һ����ռ�õ��ֽ���
										  IN unsigned char *pucYFrame,			// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYPitch,					// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucUUFrame,			// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUPitch,					// ��Ƭ U ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucVVFrame,			// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVPitch,					// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN int nWidth,						// Դͼ���ؾ����С
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,	// ������
										  IN bool bBGRA,						// 32 λ�����������ظ�ʽΪ A R G B��B�ڵ�λ���ı�־
										  IN unsigned char cAlpha,				// ���� A ֵ
										  IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUVSlice_2_RGB32_Split)(OUT unsigned char *pucRGB32TopField,		// 32 λ������壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
										  IN intptr_t nRGB32TopPitch,				// 32 λ�������һ����ռ�õ��ֽ���
										  OUT unsigned char *pucRGB32BottomField,	// 32 λ������壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
										  IN intptr_t nRGB32BottomPitch,			// 32 λ�������һ����ռ�õ��ֽ���
										  IN unsigned char *pucYFrame,				// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYPitch,						// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucUUFrame,				// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUPitch,						// ��Ƭ U ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucVVFrame,				// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVPitch,						// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN int nWidth,							// Դͼ���ؾ����С
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,		// ������
										  IN bool bBGRA,							// 32 λ�����������ظ�ʽΪ A R G B��B�ڵ�λ���ı�־
										  IN unsigned char cAlpha,					// ���� A ֵ
										  IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUVSlice_2_RGB32_Compose)(OUT unsigned char *pucRGB32Frame,	// 32 λ������壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
											IN intptr_t nRGB32Pitch,			// 32 λ�������һ����ռ�õ��ֽ���
											IN unsigned char *pucYTopField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
											IN intptr_t nYTopPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
											IN unsigned char *pucYBottomField,	// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
											IN intptr_t nYBottomPitch,			// ��Ƭ Y ����һ����ռ�õ��ֽ���
											IN unsigned char *pucUUTopField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUTopPitch,			// ��Ƭ U ����һ����ռ�õ��ֽ���
											IN unsigned char *pucUUBottomField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUBottomPitch,			// ��Ƭ U ����һ����ռ�õ��ֽ���
											IN unsigned char *pucVVTopField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVTopPitch,			// ��Ƭ V ����һ����ռ�õ��ֽ���
											IN unsigned char *pucVVBottomField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVBottomPitch,			// ��Ƭ V ����һ����ռ�õ��ֽ���
											IN int nWidth,						// Դͼ���ؾ����С
											IN int nHeight,
											IN NXVIDEO_DEFINITION eDefinition,	// ������
											IN bool bBGRA,						// 32 λ�����������ظ�ʽΪ A R G B��B�ڵ�λ���ı�־
											IN unsigned char cAlpha,			// ���� A ֵ
											IN bool bOverturn);					// ��תͼ���־

// ��Ƭ YUV422/YUV420 �� 24 λ BGR/RGB ��ɫ�ı任���ڶ��� UV ֵ����ǰ�� UV ���в�ֵ���ڶ��� UV ֵ���������� UV ���в�ֵ
typedef void (*DefYUVSlice_2_RGB24_Frame)(OUT unsigned char *pucRGB24Frame,		// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
										  IN intptr_t nRGB24Pitch,				// 24 λ�������һ����ռ�õ��ֽ���
										  IN unsigned char *pucYFrame,			// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYPitch,					// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucUUFrame,			// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUPitch,					// ��Ƭ U ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucVVFrame,			// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVPitch,					// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN int nWidth,						// Դͼ���ؾ����С
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,	// ������
										  IN bool bBGR,							// 24 λ�����������ظ�ʽΪ R G B��B�ڵ�λ���ı�־
										  IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUVSlice_2_RGB24_Split)(OUT unsigned char *pucRGB24TopField,		// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
										  IN intptr_t nRGB24TopPitch,				// 24 λ�������һ����ռ�õ��ֽ���
										  OUT unsigned char *pucRGB24BottomField,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
										  IN intptr_t nRGB24BottomPitch,			// 24 λ�������һ����ռ�õ��ֽ���
										  IN unsigned char *pucYFrame,				// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYPitch,						// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucUUFrame,				// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUPitch,						// ��Ƭ U ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucVVFrame,				// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVPitch,						// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN int nWidth,							// Դͼ���ؾ����С
										  IN int nHeight,
										  IN NXVIDEO_DEFINITION eDefinition,		// ������
										  IN bool bBGR,								// 24 λ�����������ظ�ʽΪ R G B��B�ڵ�λ���ı�־
										  IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUVSlice_2_RGB24_Compose)(OUT unsigned char *pucRGB24Frame,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
											IN intptr_t nRGB24Pitch,			// 24 λ�������һ����ռ�õ��ֽ���
											IN unsigned char *pucYTopField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
											IN intptr_t nYTopPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
											IN unsigned char *pucYBottomField,	// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
											IN intptr_t nYBottomPitch,			// ��Ƭ Y ����һ����ռ�õ��ֽ���
											IN unsigned char *pucUUTopField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUTopPitch,			// ��Ƭ U ����һ����ռ�õ��ֽ���
											IN unsigned char *pucUUBottomField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUBottomPitch,			// ��Ƭ U ����һ����ռ�õ��ֽ���
											IN unsigned char *pucVVTopField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVTopPitch,			// ��Ƭ V ����һ����ռ�õ��ֽ���
											IN unsigned char *pucVVBottomField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVBottomPitch,			// ��Ƭ V ����һ����ռ�õ��ֽ���
											IN int nWidth,						// Դͼ���ؾ����С
											IN int nHeight,
											IN NXVIDEO_DEFINITION eDefinition,	// ������
											IN bool bBGR,						// 24 λ�����������ظ�ʽΪ R G B��B�ڵ�λ���ı�־
											IN bool bOverturn);					// ��תͼ���־

// 24/32 λ YUV/YUVA ��ɫ����Ƭ YUV422/YUV420 �ı任��ֱ�Ӷ����ڶ��� UV��YUV420 ʱ UV �����в�ֵ��һ�У������ A ��ֱ�Ӷ���
typedef void (*DefYUV_2_YUVSlice_Frame)(OUT unsigned char *pucYFrame,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										IN intptr_t nYPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucUUFrame,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										IN intptr_t nUUPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucVVFrame,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										IN intptr_t nVVPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										IN unsigned char *pucYUVFrame,		// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
										IN intptr_t nYUVPitch,				// ���뻺��һ����ռ�õ��ֽ���
										IN int nWidth,						// Դͼ���ؾ����С
										IN int nHeight,
										IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUV_2_YUVSlice_Split)(OUT unsigned char *pucYTopField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										IN intptr_t nYTopPitch,					// ��Ƭ Y ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucYBottomField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										IN intptr_t nYBottomPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucUUTopField,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										IN intptr_t nUUTopPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucUUBottomField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										IN intptr_t nUUBottomPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucVVTopField,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										IN intptr_t nVVTopPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										OUT unsigned char *pucVVBottomField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										IN intptr_t nVVBottomPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										IN unsigned char *pucYUVFrame,			// ���뻺�棨�洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
										IN intptr_t nYUVPitch,					// ���뻺��һ����ռ�õ��ֽ���
										IN int nWidth,							// Դͼ���ؾ����С
										IN int nHeight,
										IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUV_2_YUVSlice_Compose)(OUT unsigned char *pucYFrame,				// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYPitch,						// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucUUFrame,			// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUPitch,						// ��Ƭ U ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucVVFrame,			// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVPitch,						// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucYUVTopField,			// ���붥�����洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
										  IN intptr_t nYUVTopPitch,					// ���붥��һ����ռ�õ��ֽ���
										  IN unsigned char *pucYUVBottomField,		// ����׳����洢��ʽ Y0 U0 V0 . Y1 U1 V1 ...��
										  IN intptr_t nYUVBottomPitch,				// ����׳�һ����ռ�õ��ֽ���
										  IN int nWidth,							// Դͼ���ؾ����С
										  IN int nHeight,
										  IN bool bOverturn);						// ��תͼ���־


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// �ӿڽṹ����

//////////////////////////////////////////////////////////////////////////
// ͨ�ýӿ�

// ͳһ�ӿ�
// ��1��ʵ�ֳ������ظ�ʽ֮�����ɫ�任����Ϊ��֡�任���𳡱任���ϳ��任���֡�
// �����ֳ�����ʱ��Ҫ�� nHeight Ϊż�������������ж�ʧ��
// ��2������ NXVIDEO_PIXEL_FMT_YUV422��NXVIDEO_PIXEL_FMT_YUV420 ����ԭʼ��������
// ��ʽ���ϸ�Ҫ����洢��ʽ������ NXVIDEO_PIXEL_FMT_KEY ��ʽ��һ��ֻ������Ŀ�꣬
// ��ʾȡԴͼ�е� KEY ͨ�������Դͼû�� KEY ͨ���������Ϊ 0xff��
// ��3�������� eDefinition �������漰 YUV ��ɫ�ռ�任ʱ��Ч��YUV ��ɫ�ռ����ָ߱��塣
// ��4������ false ��ʾ�任ʧ�ܣ������������Ч������£�˵����ʽ��֧�֡�
// ��5�����������ظ�ʽ�������������������
typedef bool (*DefColorConvFrame)(OUT unsigned char *pucDesFrame,		// Ŀ��֡
								  IN intptr_t nDesPitch,				// Ŀ��֡һ����ռ�õ��ֽ���
								  IN NXVIDEO_PIXEL_FMT eDesPixelFmt,	// Ŀ�����ظ�ʽ
								  IN unsigned char *pucSrcFrame,		// Դ֡
								  IN intptr_t nSrcPitch,				// Դ֡һ����ռ�õ��ֽ���
								  IN NXVIDEO_PIXEL_FMT eSrcPixelFmt,	// Դ���ظ�ʽ
								  IN NXVIDEO_DEFINITION eDefinition,	// ������
								  IN int nWidth,						// ���ؾ����С
								  IN int nHeight,
								  IN bool bOverturn);					// ��תͼ���־
typedef bool (*DefColorConvSplit)(OUT unsigned char *pucDesTopField,	// Ŀ�궥��
								  IN intptr_t nDesTopPitch,				// Ŀ�궥��һ����ռ�õ��ֽ���
								  OUT unsigned char *pucDesBottomField,	// Ŀ��׳�
								  IN intptr_t nDesBottomPitch,			// Ŀ��׳�һ����ռ�õ��ֽ���
								  IN NXVIDEO_PIXEL_FMT eDesPixelFmt,	// Ŀ�����ظ�ʽ
								  IN unsigned char *pucSrcFrame,		// Դ֡
								  IN intptr_t nSrcPitch,				// Դ֡һ����ռ�õ��ֽ���
								  IN NXVIDEO_PIXEL_FMT eSrcPixelFmt,	// Դ���ظ�ʽ
								  IN NXVIDEO_DEFINITION eDefinition,	// ������
								  IN int nWidth,						// ���ؾ����С
								  IN int nHeight,
								  IN bool bOverturn);					// ��תͼ���־
typedef bool (*DefColorConvCompose)(OUT unsigned char *pucDesFrame,			// Ŀ��֡
									IN intptr_t nDesPitch,					// Ŀ��֡һ����ռ�õ��ֽ���
									IN NXVIDEO_PIXEL_FMT eDesPixelFmt,		// Ŀ�����ظ�ʽ
									IN unsigned char *pucSrcTopField,		// Դ����
									IN intptr_t nSrcTopPitch,				// Դ����һ����ռ�õ��ֽ���
									IN unsigned char *pucSrcBottomField,	// Դ�׳�
									IN intptr_t nSrcNottomPitch,			// Դ�׳�һ����ռ�õ��ֽ���
									IN NXVIDEO_PIXEL_FMT eSrcPixelFmt,		// Դ���ظ�ʽ
									IN NXVIDEO_DEFINITION eDefinition,		// ������
									IN int nWidth,							// ���ؾ����С
									IN int nHeight,
									IN bool bOverturn);						// ��תͼ���־
// 32 λ BGRA/RGBA/YUVA ��ɫ�е�ͨ���ֽ�
typedef void (*DefCr32_Split_Channel_Frame)(OUT unsigned char *pucOutFrame,		// ͨ���������
											IN intptr_t nOutPitch,				// �������һ����ռ�õ��ֽ���
											IN unsigned char *pucCr32Frame,		// 32 λ���壨�洢��ʽ X0 X1 X2 A0 X0 X1 X2 A1 ������
											IN intptr_t nCr32Pitch,				// 32 λ����һ����ռ�õ��ֽ���
											IN int nWidth,						// Դͼ���ؾ����С
											IN int nHeight,
											IN int nChannel,					// ��Ҫ�ֽ��ͨ��������ȡֵΪ (0 ~ 3)
											IN bool bOverturn);					// ��תͼ���־
typedef void (*DefCr32_Split_Channel_Split)(OUT unsigned char *pucOutTopField,		// ͨ���������
											IN intptr_t nOutTopPitch,				// �������һ����ռ�õ��ֽ���
											OUT unsigned char *pucOutBottomField,	// ͨ���������
											IN intptr_t nOutBottomPitch,			// �������һ����ռ�õ��ֽ���
											IN unsigned char *pucCr32Frame,			// 32 λ���壨�洢��ʽ X0 X1 X2 A0 X0 X1 X2 A1 ������
											IN intptr_t nCr32Pitch,					// 32 λ����һ����ռ�õ��ֽ���
											IN int nWidth,							// Դͼ���ؾ����С
											IN int nHeight,
											IN int nChannel,						// ��Ҫ�ֽ��ͨ��������ȡֵΪ (0 ~ 3)
											IN bool bOverturn);						// ��תͼ���־
typedef void (*DefCr32_Split_Channel_Compose)(OUT unsigned char *pucOutFrame,		// ͨ���������
											  IN intptr_t nOutPitch,				// �������һ����ռ�õ��ֽ���
											  IN unsigned char *pucCr32TopField,	// 32 λ���壨�洢��ʽ X0 X1 X2 A0 X0 X1 X2 A1 ������
											  IN intptr_t nCr32TopPitch,			// 32 λ����һ����ռ�õ��ֽ���
											  IN unsigned char *pucCr32BottomField,	// 32 λ���壨�洢��ʽ X0 X1 X2 A0 X0 X1 X2 A1 ������
											  IN intptr_t nCr32BottomPitch,			// 32 λ����һ����ռ�õ��ֽ���
											  IN int nWidth,						// Դͼ���ؾ����С
											  IN int nHeight,
											  IN int nChannel,						// ��Ҫ�ֽ��ͨ��������ȡֵΪ (0 ~ 3)
											  IN bool bOverturn);					// ��תͼ���־

// 24 λ RGB/BGR/YUV ��ɫ�� 32 λ RGBA/BGRA/YUVA ��ɫ�ı任��A �����Ϊͳһֵ��ͬʱ֧�� R��B ����ͨ���Ļ���
typedef void (*DefCr24_2_Cr32_Frame)(OUT unsigned char *pucCr32Frame,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									 IN intptr_t nCr32Pitch,			// 32 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucCr24Frame,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									 IN intptr_t nCr24Pitch,			// 24 λ����һ����ռ�õ��ֽ���
									 IN int nWidth,						// Դͼ���ؾ����С
									 IN int nHeight,
									 IN bool bExchangeRB,				// ���� 0��2 �ֽڣ�R��B ͨ�����ı�־
									 IN unsigned char ucAlpha,			// ���� A ֵ
									 IN bool bOverturn);				// ��תͼ���־
typedef void (*DefCr24_2_Cr32_Split)(OUT unsigned char *pucCr32TopField,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									 IN intptr_t nCr32TopPitch,				// 32 λ����һ����ռ�õ��ֽ���
									 OUT unsigned char *pucCr32BottomField,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									 IN intptr_t nCr32BottomPitch,			// 32 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucCr24Frame,		// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									 IN intptr_t nCr24Pitch,				// 24 λ����һ����ռ�õ��ֽ���
									 IN int nWidth,							// Դͼ���ؾ����С
									 IN int nHeight,
									 IN bool bExchangeRB,					// ���� 0��2 �ֽڣ�R��B ͨ�����ı�־
									 IN unsigned char ucAlpha,				// ���� A ֵ
									 IN bool bOverturn);					// ��תͼ���־
typedef void (*DefCr24_2_Cr32_Compose)(OUT unsigned char *pucCr32Frame,			// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									   IN intptr_t nCr32Pitch,					// 32 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucCr24TopField,		// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									   IN intptr_t nCr24TopPitch,				// 24 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucCr24BottomField,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									   IN intptr_t nCr24BottomPitch,			// 24 λ����һ����ռ�õ��ֽ���
									   IN int nWidth,							// Դͼ���ؾ����С
									   IN int nHeight,
									   IN bool bExchangeRB,						// ���� 0��2 �ֽڣ�R��B ͨ�����ı�־
									   IN unsigned char ucAlpha,				// ���� A ֵ
									   IN bool bOverturn);						// ��תͼ���־
// 32 λ RGBA/BGRA/YUVA ��ɫ�� 24 λ RGB/BGR/YUV ��ɫ�ı任��ֱ�Ӷ��� A��ͬʱ֧�� R��B ����ͨ���Ļ���
typedef void (*DefCr32_2_Cr24_Frame)(OUT unsigned char *pucCr24Frame,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									 IN intptr_t nCr24Pitch,			// 24 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucCr32Frame,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									 IN intptr_t nCr32Pitch,			// 32 λ����һ����ռ�õ��ֽ���
									 IN int nWidth,						// Դͼ���ؾ����С
									 IN int nHeight,
									 IN bool bExchangeRB,				// ���� 0��2 �ֽڣ�R��B ͨ�����ı�־
									 IN bool bOverturn);				// ��תͼ���־
typedef void (*DefCr32_2_Cr24_Split)(OUT unsigned char *pucCr24TopField,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									 IN intptr_t nCr24TopPitch,				// 24 λ����һ����ռ�õ��ֽ���
									 OUT unsigned char *pucCr24BottomField,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									 IN intptr_t nCr24BottomPitch,			// 24 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucCr32Frame,		// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									 IN intptr_t nCr32Pitch,				// 32 λ����һ����ռ�õ��ֽ���
									 IN int nWidth,							// Դͼ���ؾ����С
									 IN int nHeight,
									 IN bool bExchangeRB,					// ���� 0��2 �ֽڣ�R��B ͨ�����ı�־
									 IN bool bOverturn);					// ��תͼ���־
typedef void (*DefCr32_2_Cr24_Compose)(OUT unsigned char *pucCr24Frame,			// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									   IN intptr_t nCr24Pitch,					// 24 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucCr32TopField,		// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									   IN intptr_t nCr32TopPitch,				// 32 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucCr32BottomField,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									   IN intptr_t nCr32BottomPitch,			// 32 λ����һ����ռ�õ��ֽ���
									   IN int nWidth,							// Դͼ���ؾ����С
									   IN int nHeight,
									   IN bool bExchangeRB,						// ���� 0��2 �ֽڣ�R��B ͨ�����ı�־
									   IN bool bOverturn);						// ��תͼ���־

// ֡�źŵĲ���ϳ�
// ��1����� nHeight Ϊ����ʱ���ᶪʧһ�С�
// ��2��ͼ��ת��־Ҫ��ȷʹ�ã�ȷ��������׳�����������Ƶ����ʽ�Ⱥϡ�
// ��3����������ͼ��ı�׼Ϊͼ������Ͻǵ����ڴ������롣
typedef void (*DefSplitFrame)(OUT unsigned char *pucTopField,		// �������
							  IN intptr_t nTopPitch,				// һ����ռ�ֽ���
							  OUT unsigned char *pucBottomField,	// �׳����
							  IN intptr_t nBottomPitch,				// һ����ռ�ֽ���
							  IN unsigned char *pucFrame,			// Դ֡����
							  IN intptr_t nFramePitch,				// һ����ռ�ֽ���
							  IN int nWidth,						// Դͼ���ؾ����С
							  IN int nHeight,
							  IN int nPixelBytes,					// һ��������ռ�ֽ���
							  IN bool bOverturn);					// ͼ��ת��־
typedef void (*DefComposeFields)(OUT unsigned char *pucFrame,		// Ŀ��֡���
								 IN intptr_t nFramePitch,			// һ����ռ�ֽ���
								 IN unsigned char *pucTopField,		// ��������
								 IN intptr_t nTopPitch,				// һ����ռ�ֽ���
								 IN unsigned char *pucBottomField,	// �׳�����
								 IN intptr_t nBottomPitch,			// һ����ռ�ֽ���
								 IN int nWidth,						// Դͼ���ؾ����С
								 IN int nHeight,
								 IN int nPixelBytes,				// һ��������ռ�ֽ���
								 IN bool bOverturn);				// ͼ��ת��־

// ��ȫɫת��
// ��1��8bit ��ɫ RGBY ������ֵ�� [16, 235] �� [0, 255]��10bit ��ɫ��δ��������չ
// ��2��8bit ��ɫ UV ������ֵ�� [16, 240] ~ [0, 255], 10bit ��ɫ��δ��������չ
// ��3�����ڴ��� Alpha ͨ�������أ����� Alpha ����
// ��4��Դ��Ŀ�Ŀ�����ͬ
typedef void (*DefPackSafeRGB)(OUT unsigned char *pucDes,		// Ŀ��
							   IN intptr_t nDesPitch,			// Ŀ��һ����ռ�õ��ֽ���
							   IN const unsigned char *pucSrc,	// Դ
							   IN intptr_t nSrcPitch,			// Դһ����ռ�õ��ֽ���
							   IN NXVIDEO_PIXEL_FMT ePixFmt,	// ���ظ�ʽ
							   IN int nWidth,					// ���ؾ����С
							   IN int nHeight,
							   IN bool bOverturn);				// ��תͼ���־
typedef DefPackSafeRGB		DefUnpackSafeRGB;

//////////////////////////////////////////////////////////////////////////
// ��ɫת��ͨ�ýӿڣ�����һ��ʽ����һ��ʽ����ɫת��
// ��ҪΪ֧�� 10bit��12bit��16bit �����ظ�ʽ��չ
// �ڲ�ʵ�ֽ�����Դ��Ŀ�ĵ����ظ�ʽ��������
typedef void (*pfncs_any_to_any_frame)(__out color_wrapper* des_frame,			// Ŀ��֡
									   __in	color_wrapper* src_frame,			// Դ֡
									   __in NXVIDEO_DEFINITION definition,		// ������
									   __in int width,							// ֡��
									   __in int height,							// ֡��
									   __in bool overturn);						// ��ת��־
typedef void (*pfncs_any_to_any_split)(__out color_wrapper* des_top_field,		// Ŀ�Ķ���
									   __out color_wrapper* des_bottom_field,	// Ŀ�ĵ׳�
									   __in color_wrapper* src_frame,			// Դ֡
									   __in NXVIDEO_DEFINITION definition,		// ������
									   __in int width,							// ֡��
									   __in int height,							// ֡��
									   __in bool overturn);						// ��ת��־
typedef void (*pfncs_any_to_any_compose)(__out color_wrapper* des_frame,		// Ŀ��֡
										 __in color_wrapper* src_top_field,		// Դ����
										 __in color_wrapper* src_bottom_field,	// Դ�׳�
										 __in NXVIDEO_DEFINITION definition,	// ������
										 __in int width,						// ֡��
										 __in int height,						// ֡��
										 __in bool overturn);					// ��ת��־
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxcolorgenericapi
{
	// ͳһ�ӿ�
	DefColorConvFrame		pfnColorConvFrame;
	DefColorConvSplit		pfnColorConvSplit;
	DefColorConvCompose		pfnColorConvCompose;
	// RGBA/BGRA/YUVA -> Channel
	DefCr32_Split_Channel_Frame		pfnCr32_Split_Channel_Frame;
	DefCr32_Split_Channel_Split		pfnCr32_Split_Channel_Split;
	DefCr32_Split_Channel_Compose	pfnCr32_Split_Channel_Compose;
	// RGB/BGR/YUV <-> RGBA/BGRA/YUVA
	DefCr24_2_Cr32_Frame		pfnCr24_2_Cr32_Frame;
	DefCr24_2_Cr32_Split		pfnCr24_2_Cr32_Split;
	DefCr24_2_Cr32_Compose		pfnCr24_2_Cr32_Compose;
	DefCr32_2_Cr24_Frame		pfnCr32_2_Cr24_Frame;
	DefCr32_2_Cr24_Split		pfnCr32_2_Cr24_Split;
	DefCr32_2_Cr24_Compose		pfnCr32_2_Cr24_Compose;
	// ��/�ϳ�
	DefSplitFrame			pfnSplitFrame;
	DefComposeFields		pfnComposeFields;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.6
	// ��ȫɫת��
	DefPackSafeRGB			pfnPackSafeRGB;
	DefUnpackSafeRGB		pfnUnpackSafeRGB;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.9
	pfncs_any_to_any_frame		any_to_any_frame;
	pfncs_any_to_any_split		any_to_any_split;
	pfncs_any_to_any_compose	any_to_any_compose;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴӴ˿�ʼ����
} NXColorGenericAPI, *LPNXCOLORGENERICAPI;

//////////////////////////////////////////////////////////////////////////
// RGB32(RGBA/BGRA) => Others

// 32 λ RGBA/BGRA ��ɫ���� R��B ͨ��
typedef DefRGB_SwapRB_Frame			DefRGB32_SwapRB_Frame;
typedef DefRGB_SwapRB_Split			DefRGB32_SwapRB_Split;
typedef DefRGB_SwapRB_Compose		DefRGB32_SwapRB_Compose;
// 32 λ BGRA/RGBA ��ɫ�� 32 λ YUVA ��ɫ�ı任
typedef DefRGB_2_YUV_Frame			DefRGB32_2_YUVA_Frame;
typedef DefRGB_2_YUV_Split			DefRGB32_2_YUVA_Split;
typedef DefRGB_2_YUV_Compose		DefRGB32_2_YUVA_Compose;
// 32 λ BGRA/RGBA ��ɫ�� 16 λ YUYV/UYVY ��ɫ�ı任��ֱ�Ӷ��� A ��ڶ��� UV
typedef DefRGB_2_YUV16_Frame		DefRGB32_2_YUV16_Frame;
typedef DefRGB_2_YUV16_Split		DefRGB32_2_YUV16_Split;
typedef DefRGB_2_YUV16_Compose		DefRGB32_2_YUV16_Compose;
// 32 λ BGRA/RGBA ��ɫ����Ƭ YUV422/YUV420 �ı任��ֱ�Ӷ��� A ��ڶ��� UV��YUV420 ʱ UV �����в�ֵ��һ��
typedef DefRGB_2_YUVSlice_Frame		DefRGB32_2_YUVSlice_Frame;
typedef DefRGB_2_YUVSlice_Split		DefRGB32_2_YUVSlice_Split;
typedef DefRGB_2_YUVSlice_Compose	DefRGB32_2_YUVSlice_Compose;
// 32 λ BGRA/RGBA ��ɫ�� 24 λ YUV ��ɫ�ı任��ֱ�Ӷ��� A
typedef DefRGB_2_YUV_Frame			DefRGB32_2_YUV_Frame;
typedef DefRGB_2_YUV_Split			DefRGB32_2_YUV_Split;
typedef DefRGB_2_YUV_Compose		DefRGB32_2_YUV_Compose;
// RGBA/BGRA => Gray
typedef void (*pfncs_rgb32_to_gray_frame)(__out unsigned char* gray,			// �ҶȻ���
										  __in intptr_t gray_pitch,				// �ҶȻ���һ�е��ֽ���
										  __in unsigned char* rgb32,			// RGBA/BGRA 32 λ��ɫ����
										  __in intptr_t rgb32_pitch,			// RGBA/BGRA 32 λ��ɫ����һ�е��ֽ���
										  __in int width,						// �������
										  __in int height,
										  __in NXVIDEO_DEFINITION definition,	// ������
										  __in bool isbgra,						// �Ƿ�Ϊ 32 λ BGRA
										  __in bool overturn);					// �Ƿ�ת
typedef void (*pfncs_rgb32_to_gray_split)(__out unsigned char* gray_top,		// �Ҷȶ�������
										  __in intptr_t gray_top_pitch,			// �Ҷȶ�������һ�е��ֽ���
										  __out unsigned char* gray_bottom,		// �Ҷȵ׳�����
										  __in intptr_t gray_bottom_pitch,		// �Ҷȵ׳�����һ�е��ֽ���
										  __in unsigned char* rgb32,			// RGBA/BGRA 32 λ��ɫ����
										  __in intptr_t rgb32_pitch,			// RGBA/BGRA 32 λ��ɫ����һ�е��ֽ���
										  __in int width,						// �������
										  __in int height,
										  __in NXVIDEO_DEFINITION definition,	// ������
										  __in bool isbgra,						// �Ƿ�Ϊ 32 λ BGRA
										  __in bool overturn);					// �Ƿ�ת
typedef void (*pfncs_rgb32_to_gray_compose)(__out unsigned char* gray,			// �ҶȻ���
											__in intptr_t gray_pitch,			// �ҶȻ���һ�е��ֽ���
											__in unsigned char* rgb32_top,		// RGBA/BGRA 32 λ��ɫ��������
											__in intptr_t rgb32_top_pitch,		// RGBA/BGRA 32 λ��ɫ��������һ�е��ֽ���
											__in unsigned char* rgb32_bottom,	// RGBA/BGRA 32 λ��ɫ�׳�����
											__in intptr_t rgb32_bottom_pitch,	// RGBA/BGRA 32 λ��ɫ�׳�����һ�е��ֽ���
											__in int width,						// �������
											__in int height,
											__in NXVIDEO_DEFINITION definition,	// ������
											__in bool isbgra,					// �Ƿ�Ϊ 32 λ BGRA
											__in bool overturn);				// �Ƿ�ת
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxrgb32toothers
{
	DefRGB32_SwapRB_Frame		pfnRGB32_SwapRB_Frame;
	DefRGB32_SwapRB_Split		pfnRGB32_SwapRB_Split;
	DefRGB32_SwapRB_Compose		pfnRGB32_SwapRB_Compose;

	DefRGB32_2_YUVA_Frame		pfnRGB32_2_YUVA_Frame;
	DefRGB32_2_YUVA_Split		pfnRGB32_2_YUVA_Split;
	DefRGB32_2_YUVA_Compose		pfnRGB32_2_YUVA_Compose;

	DefRGB32_2_YUV16_Frame		pfnRGB32_2_YUV16_Frame;
	DefRGB32_2_YUV16_Split		pfnRGB32_2_YUV16_Split;
	DefRGB32_2_YUV16_Compose	pfnRGB32_2_YUV16_Compose;

	DefRGB32_2_YUVSlice_Frame	pfnRGB32_2_YUV422_Frame;
	DefRGB32_2_YUVSlice_Split	pfnRGB32_2_YUV422_Split;
	DefRGB32_2_YUVSlice_Compose	pfnRGB32_2_YUV422_Compose;

	DefRGB32_2_YUVSlice_Frame	pfnRGB32_2_YUV420_Frame;
	DefRGB32_2_YUVSlice_Split	pfnRGB32_2_YUV420_Split;
	DefRGB32_2_YUVSlice_Compose	pfnRGB32_2_YUV420_Compose;

	DefRGB32_2_YUV_Frame		pfnRGB32_2_YUV_Frame;
	DefRGB32_2_YUV_Split		pfnRGB32_2_YUV_Split;
	DefRGB32_2_YUV_Compose		pfnRGB32_2_YUV_Compose;
	// V1.1.0.5
	pfncs_rgb32_to_gray_frame	rgb32_to_gray_frame;
	pfncs_rgb32_to_gray_split	rgb32_to_gray_split;
	pfncs_rgb32_to_gray_compose	rgb32_to_gray_compose;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXRGB32toOthers, *LPNXRGB32TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// RGB24(RGB/BGR) => Others

// 24 λ RGB/BGR ��ɫ���� R��B ͨ��
typedef DefRGB_SwapRB_Frame			DefRGB24_SwapRB_Frame;
typedef DefRGB_SwapRB_Split			DefRGB24_SwapRB_Split;
typedef DefRGB_SwapRB_Compose		DefRGB24_SwapRB_Compose;
// 24 λ BGR/RGB ��ɫ�� 32 λ YUVA ��ɫ�ı任��A �����Ϊͳһֵ
typedef void (*DefRGB24_2_YUVA_Frame)(OUT unsigned char *pucYUVAFrame,		// 32 λ������棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
									  IN intptr_t nYUVAPitch,				// 32 λ�������һ����ռ�õ��ֽ���
									  IN unsigned char *pucRGB24Frame,		// 24 λ���뻺�壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									  IN intptr_t nRGB24Pitch,				// 24 λ���뻺��һ����ռ�õ��ֽ���
									  IN int nWidth,						// Դͼ���ؾ����С
									  IN int nHeight,
									  IN NXVIDEO_DEFINITION eDefinition,	// ������
									  IN bool bBGR,							// 24 λ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
									  IN unsigned char cAlpha,				// ���� A ֵ
									  IN bool bOverturn);					// ��תͼ���־
typedef void (*DefRGB24_2_YUVA_Split)(OUT unsigned char *pucYUVATopField,		// 32 λ������棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
									  IN intptr_t nYUVATopPitch,				// 32 λ�������һ����ռ�õ��ֽ���
									  OUT unsigned char *pucYUVABottomField,	// 32 λ������棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
									  IN intptr_t nYUVABottomPitch,				// 32 λ�������һ����ռ�õ��ֽ���
									  IN unsigned char *pucRGB24Frame,			// 24 λ���뻺�壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									  IN intptr_t nRGB24Pitch,					// 24 λ���뻺��һ����ռ�õ��ֽ���
									  IN int nWidth,							// Դͼ���ؾ����С
									  IN int nHeight,
									  IN NXVIDEO_DEFINITION eDefinition,		// ������
									  IN bool bBGR,								// 24 λ���뻺������ظ�ʽΪ R G B��B�ڵ�λ��
									  IN unsigned char cAlpha,					// ���� A ֵ
									  IN bool bOverturn);						// ��תͼ���־
typedef void (*DefRGB24_2_YUVA_Compose)(OUT unsigned char *pucYUVAFrame,		// 32 λ������棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
										IN intptr_t nYUVAPitch,					// 32 λ�������һ����ռ�õ��ֽ���
										IN unsigned char *pucRGB24TopField,		// 24 λ���뻺�壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
										IN intptr_t nRGB24TopPitch,				// 24 λ���뻺��һ����ռ�õ��ֽ���
										IN unsigned char *pucRGB24BottomField,	// 24 λ���뻺�壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
										IN intptr_t nRGB24BottomPitch,			// 24 λ���뻺��һ����ռ�õ��ֽ���
										IN int nWidth,							// Դͼ���ؾ����С
										IN int nHeight,
										IN NXVIDEO_DEFINITION eDefinition,		// ������
										IN bool bBGR,							// 24 λ�������������ظ�ʽΪ R G B��B�ڵ�λ��
										IN unsigned char cAlpha,				// ���� A ֵ
										IN bool bOverturn);						// ��תͼ���־
// 24 λ BGR/RGB ��ɫ�� 16 λ YUYV/UYVY ��ɫ�ı任��ֱ�Ӷ����ڶ��� UV
typedef DefRGB_2_YUV16_Frame		DefRGB24_2_YUV16_Frame;
typedef DefRGB_2_YUV16_Split		DefRGB24_2_YUV16_Split;
typedef DefRGB_2_YUV16_Compose		DefRGB24_2_YUV16_Compose;
// 24 λ BGR/RGB ��ɫ����Ƭ YUV422/YUV420 �ı任��ֱ�Ӷ����ڶ��� UV��YUV420 ʱ UV �����в�ֵ��һ��
typedef DefRGB_2_YUVSlice_Frame		DefRGB24_2_YUVSlice_Frame;
typedef DefRGB_2_YUVSlice_Split		DefRGB24_2_YUVSlice_Split;
typedef DefRGB_2_YUVSlice_Compose	DefRGB24_2_YUVSlice_Compose;
// 24 λ BGR/RGB ��ɫ�� 24 λ YUV ��ɫ�ı任
typedef DefRGB_2_YUV_Frame			DefRGB24_2_YUV_Frame;
typedef DefRGB_2_YUV_Split			DefRGB24_2_YUV_Split;
typedef DefRGB_2_YUV_Compose		DefRGB24_2_YUV_Compose;
// 24λ BGR/RGB => HSV
typedef void (*pfncs_rgb24_to_hsv_frame)(__out short* hue,					// ɫ��
										 __in intptr_t hue_pitch,			// ɫ��һ�е��ֽ���
										 __out unsigned char* saturation,	// ���Ͷ�
										 __in intptr_t saturation_pitch,	// ���Ͷ�һ�е��ֽ���
										 __out unsigned char* value,		// ����
										 __in intptr_t value_pitch,			// ����һ�е��ֽ���
										 __in unsigned char* rgb24,			// 24 λ��ɫ����
										 __in intptr_t rgb24_pitch,			// 24 λ��ɫ����һ�е��ֽ���
										 __in int width,					// �����
										 __in int height,					// �����
										 __in bool isbgr,					// �Ƿ�Ϊ BGR
										 __in bool overturn);				// �Ƿ�ת
typedef void (*pfncs_rgb24_to_hsv_split)(__out short* hue_top,					// ����ɫ��
										 __in intptr_t hue_top_pitch,			// ����ɫ��һ�е��ֽ���
										 __out short* hue_bottom,				// �׳�ɫ��
										 __in intptr_t hue_bottom_pitch,		// �׳�ɫ��һ�е��ֽ���
										 __out unsigned char* saturation_top,	// �������Ͷ�
										 __in intptr_t saturation_top_pitch,	// �������Ͷ�һ�е��ֽ���
										 __out unsigned char* saturation_bottom,// �׳����Ͷ�
										 __in intptr_t saturation_bottom_pitch,	// �׳����Ͷ�һ�е��ֽ���
										 __out unsigned char* value_top,		// ��������
										 __in intptr_t value_top_pitch,			// ��������һ�е��ֽ���
										 __out unsigned char* value_bottom,		// �׳�����
										 __in intptr_t value_bottom_pitch,		// �׳�����һ�е��ֽ���
										 __in unsigned char* rgb24,				// 24 λ��ɫ����
										 __in intptr_t rgb24_pitch,				// 24 λ��ɫ����һ�е��ֽ���
										 __in int width,						// �����
										 __in int height,						// �����
										 __in bool isbgr,						// �Ƿ�Ϊ BGR
										 __in bool overturn);					// �Ƿ�ת
typedef void (*pfncs_rgb24_to_hsv_compose)(__out short* hue,				// ɫ��
										   __in intptr_t hue_pitch,			// ɫ��һ�е��ֽ���
										   __out unsigned char* saturation,	// ���Ͷ�
										   __in intptr_t saturation_pitch,	// ���Ͷ�һ�е��ֽ���
										   __out unsigned char* value,		// ����
										   __in intptr_t value_pitch,		// ����һ�е��ֽ���
										   __in unsigned char* rgb24_top,	// ���� 24 λ��ɫ����
										   __in intptr_t rgb24_top_pitch,	// ���� 24 λ��ɫ����һ�е��ֽ���
										   __in unsigned char* rgb24_bottom,// �׳� 24 λ��ɫ����
										   __in intptr_t rgb24_bottom_pitch,// �׳� 24 λ��ɫ����һ�е��ֽ���
										   __in int width,					// �����
										   __in int height,					// �����
										   __in bool isbgr,					// �Ƿ�Ϊ BGR
										   __in bool overturn);				// �Ƿ�ת
// RGB => Gray
typedef void (*pfncs_rgb24_to_gray_frame)(__out unsigned char* gray,			// �ҶȻ���
										  __in intptr_t gray_pitch,				// �ҶȻ���һ�е��ֽ���
										  __in unsigned char* rgb24,			// RGB/BGR 24 λ��ɫ����
										  __in intptr_t rgb24_pitch,			// RGB/BGR 24 λ��ɫ����һ�е��ֽ���
										  __in int width,						// �������
										  __in int height,
										  __in NXVIDEO_DEFINITION definition,	// ������
										  __in bool isbgr,						// �Ƿ�Ϊ 24 λ BGR
										  __in bool overturn);					// �Ƿ�ת
typedef void (*pfncs_rgb24_to_gray_split)(__out unsigned char* gray_top,		// �Ҷȶ�������
										  __in intptr_t gray_top_pitch,			// �Ҷȶ�������һ�е��ֽ���
										  __out unsigned char* gray_bottom,		// �Ҷȵ׳�����
										  __in intptr_t gray_bottom_pitch,		// �Ҷȵ׳�����һ�е��ֽ���
										  __in unsigned char* rgb24,			// RGB/BGR 24 λ��ɫ����
										  __in intptr_t rgb24_pitch,			// RGB/BGR 24 λ��ɫ����һ�е��ֽ���
										  __in int width,						// �������
										  __in int height,
										  __in NXVIDEO_DEFINITION definition,	// ������
										  __in bool isbgr,						// �Ƿ�Ϊ 24 λ BGR
										  __in bool overturn);					// �Ƿ�ת
typedef void (*pfncs_rgb24_to_gray_compose)(__out unsigned char* gray,			// �ҶȻ���
											__in intptr_t gray_pitch,			// �ҶȻ���һ�е��ֽ���
											__in unsigned char* rgb24_top,		// RGB/BGR 24 λ��ɫ��������
											__in intptr_t rgb24_top_pitch,		// RGB/BGR 24 λ��ɫ��������һ�е��ֽ���
											__in unsigned char* rgb24_bottom,	// RGB/BGR 24 λ��ɫ�׳�����
											__in intptr_t rgb24_bottom_pitch,	// RGB/BGR 24 λ��ɫ�׳�����һ�е��ֽ���
											__in int width,						// �������
											__in int height,
											__in NXVIDEO_DEFINITION definition,	// ������
											__in bool isbgr,					// �Ƿ�Ϊ 24 λ BGR
											__in bool overturn);				// �Ƿ�ת
// RGB/BGR 24 λ��ɫֱ��ͼͳ�ƣ�Ͱ��Ϊ 256
typedef void (*pfncs_rgb24_histogram)(__out int* rhist,				// ��ɫͨ���ֲ�
									  __out int* ghist,				// ��ɫͨ���ֲ�
									  __out int* bhist,				// ��ɫͨ���ֲ�
									  __in unsigned char* rgb24,	// RGB/BGR 24 λ��ɫ����
									  __in intptr_t rgb24_pitch,	// RGB/BGR 24 λ��ɫ����һ�е��ֽ���
									  __in int width,				// �������
									  __in int height,
									  __in bool isbgr);				// �Ƿ�Ϊ 24 λ BGR

//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxrgb24toothers
{
	DefRGB24_SwapRB_Frame		pfnRGB24_SwapRB_Frame;
	DefRGB24_SwapRB_Split		pfnRGB24_SwapRB_Split;
	DefRGB24_SwapRB_Compose		pfnRGB24_SwapRB_Compose;

	DefRGB24_2_YUVA_Frame		pfnRGB24_2_YUVA_Frame;
	DefRGB24_2_YUVA_Split		pfnRGB24_2_YUVA_Split;
	DefRGB24_2_YUVA_Compose		pfnRGB24_2_YUVA_Compose;

	DefRGB24_2_YUV16_Frame		pfnRGB24_2_YUV16_Frame;
	DefRGB24_2_YUV16_Split		pfnRGB24_2_YUV16_Split;
	DefRGB24_2_YUV16_Compose	pfnRGB24_2_YUV16_Compose;

	DefRGB24_2_YUVSlice_Frame	pfnRGB24_2_YUV422_Frame;
	DefRGB24_2_YUVSlice_Split	pfnRGB24_2_YUV422_Split;
	DefRGB24_2_YUVSlice_Compose	pfnRGB24_2_YUV422_Compose;

	DefRGB24_2_YUVSlice_Frame	pfnRGB24_2_YUV420_Frame;
	DefRGB24_2_YUVSlice_Split	pfnRGB24_2_YUV420_Split;
	DefRGB24_2_YUVSlice_Compose	pfnRGB24_2_YUV420_Compose;

	DefRGB24_2_YUV_Frame		pfnRGB24_2_YUV_Frame;
	DefRGB24_2_YUV_Split		pfnRGB24_2_YUV_Split;
	DefRGB24_2_YUV_Compose		pfnRGB24_2_YUV_Compose;

	// V1.1.0.0
	pfncs_rgb24_to_hsv_frame	rgb24_to_hsv_frame;
	pfncs_rgb24_to_hsv_split	rgb24_to_hsv_split;
	pfncs_rgb24_to_hsv_compose	rgb24_to_hsv_compose;
	// V1.1.0.1
	pfncs_rgb24_to_gray_frame	rgb24_to_gray_frame;
	pfncs_rgb24_to_gray_split	rgb24_to_gray_split;
	pfncs_rgb24_to_gray_compose	rgb24_to_gray_compose;
	pfncs_rgb24_histogram		rgb24_histogram;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXRGB24toOthers, *LPNXRGB24TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUVA => Others

// 32 λ YUVA ��ɫ�� 32 λ BGRA/RGBA ��ɫ�ı任
typedef DefYUV_2_RGB_Frame			DefYUVA_2_RGB32_Frame;
typedef DefYUV_2_RGB_Split			DefYUVA_2_RGB32_Split;
typedef DefYUV_2_RGB_Compose		DefYUVA_2_RGB32_Compose;
// 32 λ YUVA ��ɫ�� 24 λ BGR/RGB ��ɫ�ı任��ֱ�Ӷ��� A
typedef DefYUV_2_RGB_Frame			DefYUVA_2_RGB24_Frame;
typedef DefYUV_2_RGB_Split			DefYUVA_2_RGB24_Split;
typedef DefYUV_2_RGB_Compose		DefYUVA_2_RGB24_Compose;
// 32 λ YUVA ��ɫ�� 16 λ YUYV/UYVY ��ɫ�ı任��ֱ�Ӷ��� A �� �ڶ��� UV
typedef DefYUV_2_YUV16_Frame		DefYUVA_2_YUV16_Frame;
typedef DefYUV_2_YUV16_Split		DefYUVA_2_YUV16_Split;
typedef DefYUV_2_YUV16_Compose		DefYUVA_2_YUV16_Compose;
// 32 λ YUVA ��ɫ����Ƭ YUV422/YUV420 �ı任��ֱ�Ӷ��� A ��ڶ��� UV��YUV420 ʱ UV �����в�ֵ��һ��
typedef DefYUV_2_YUVSlice_Frame		DefYUVA_2_YUVSlice_Frame;
typedef DefYUV_2_YUVSlice_Split		DefYUVA_2_YUVSlice_Split;
typedef DefYUV_2_YUVSlice_Compose	DefYUVA_2_YUVSlice_Compose;

typedef struct tag_nxyuvatoothers
{
	DefYUVA_2_RGB32_Frame		pfnYUVA_2_RGB32_Frame;
	DefYUVA_2_RGB32_Split		pfnYUVA_2_RGB32_Split;
	DefYUVA_2_RGB32_Compose		pfnYUVA_2_RGB32_Compose;

	DefYUVA_2_RGB24_Frame		pfnYUVA_2_RGB24_Frame;
	DefYUVA_2_RGB24_Split		pfnYUVA_2_RGB24_Split;
	DefYUVA_2_RGB24_Compose		pfnYUVA_2_RGB24_Compose;

	DefYUVA_2_YUV16_Frame		pfnYUVA_2_YUV16_Frame;
	DefYUVA_2_YUV16_Split		pfnYUVA_2_YUV16_Split;
	DefYUVA_2_YUV16_Compose		pfnYUVA_2_YUV16_Compose;

	DefYUVA_2_YUVSlice_Frame	pfnYUVA_2_YUV422_Frame;
	DefYUVA_2_YUVSlice_Split	pfnYUVA_2_YUV422_Split;
	DefYUVA_2_YUVSlice_Compose	pfnYUVA_2_YUV422_Compose;

	DefYUVA_2_YUVSlice_Frame	pfnYUVA_2_YUV420_Frame;
	DefYUVA_2_YUVSlice_Split	pfnYUVA_2_YUV420_Split;
	DefYUVA_2_YUVSlice_Compose	pfnYUVA_2_YUV420_Compose;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXYUVAtoOthers, *LPNXYUVATOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUV16(YUYV/UYVY) => Others

// 16 λ UYVY �� YUYV ��ɫ�ı任������һ�����ڵ������ֽڼ���ʵ�ֻ���
typedef void (*DefYUV16_Swap_Frame)(OUT unsigned char *pucYUV16OutFrame,	// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									IN intptr_t nOutPitch,					// 16 λ�������һ����ռ�õ��ֽ���
									IN unsigned char *pucYUV16InFrame,		// 16 λ���뻺�棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									IN intptr_t nInPitch,					// 16 λ���뻺��һ����ռ�õ��ֽ���
									IN int nWidth,							// Դͼ���ؾ����С
									IN int nHeight,
									IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUV16_Swap_Split)(OUT unsigned char *pucYUV16OutTopField,		// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									IN intptr_t nOutTopPitch,					// 16 λ�������һ����ռ�õ��ֽ���
									OUT unsigned char *pucYUV16OutBottomField,	// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									IN intptr_t nOutBottomPitch,				// 16 λ�������һ����ռ�õ��ֽ���
									IN unsigned char *pucYUV16InFrame,			// 16 λ���뻺�棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									IN intptr_t nInPitch,						// 16 λ���뻺��һ����ռ�õ��ֽ���
									IN int nWidth,								// Դͼ���ؾ����С
									IN int nHeight,
									IN bool bOverturn);							// ��תͼ���־
typedef void (*DefYUV16_Swap_Compose)(OUT unsigned char *pucYUV16OutFrame,		// 16 λ������棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									  IN intptr_t nOutPitch,					// 16 λ�������һ����ռ�õ��ֽ���
									  IN unsigned char *pucYUV16InTopField,		// 16 λ���뻺�棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									  IN intptr_t nInTopPitch,					// 16 λ���뻺��һ����ռ�õ��ֽ���
									  IN unsigned char *pucYUV16InBottomField,	// 16 λ���뻺�棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									  IN intptr_t nInBottomPitch,				// 16 λ���뻺��һ����ռ�õ��ֽ���
									  IN int nWidth,							// Դͼ���ؾ����С
									  IN int nHeight,
									  IN bool bOverturn);						// ��תͼ���־
// 16 λ YUYV/UYVY ��ɫ�� 32 λ BGRA/RGBA ��ɫ�ı任��A �����Ϊͳһֵ
typedef void (*DefYUV16_2_RGB32_Frame)(OUT unsigned char *pucRGB32Frame,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									   IN intptr_t nRGB32Pitch,				// 32 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUV16Frame,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									   IN intptr_t nYUV16Pitch,				// 16 λ����һ����ռ�õ��ֽ���
									   IN int nWidth,						// Դͼ���ؾ����С
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,	// ������
									   IN bool bBGRA,						// 32 λ��������ظ�ʽΪ A R G B��B�ڵ�λ��
									   IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									   IN unsigned char cAlpha,				// ���� A ֵ
									   IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUV16_2_RGB32_Split)(OUT unsigned char *pucRGB32TopField,		// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									   IN intptr_t nRGB32TopPitch,				// 32 λ����һ����ռ�õ��ֽ���
									   OUT unsigned char *pucRGB32BottomField,	// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									   IN intptr_t nRGB32BottomPitch,			// 32 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUV16Frame,			// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									   IN intptr_t nYUV16Pitch,					// 16 λ����һ����ռ�õ��ֽ���
									   IN int nWidth,							// Դͼ���ؾ����С
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,		// ������
									   IN bool bBGRA,							// 32 λ��������ظ�ʽΪ A R G B��B�ڵ�λ��
									   IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									   IN unsigned char cAlpha,					// ���� A ֵ
									   IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUV16_2_RGB32_Compose)(OUT unsigned char *pucRGB32Frame,		// 32 λ���壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
										 IN intptr_t nRGB32Pitch,				// 32 λ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucYUV16TopField,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										 IN intptr_t nYUV16TopPitch,			// 16 λ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucYUV16BottomField,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										 IN intptr_t nYUV16BottomPitch,			// 16 λ����һ����ռ�õ��ֽ���
										 IN int nWidth,							// Դͼ���ؾ����С
										 IN int nHeight,
										 IN NXVIDEO_DEFINITION eDefinition,		// ������
										 IN bool bBGRA,							// 32 λ��������ظ�ʽΪ A R G B��B�ڵ�λ��
										 IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
										 IN unsigned char cAlpha,				// ���� A ֵ
										 IN bool bOverturn);					// ��תͼ���־
// 16 λ YUYV/UYVY ��ɫ�� 24 λ BGR/RGB ��ɫ�ı任
typedef void (*DefYUV16_2_RGB24_Frame)(OUT unsigned char *pucRGB24Frame,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									   IN intptr_t nRGB24Pitch,				// 24 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUV16Frame,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									   IN intptr_t nYUV16Pitch,				// 16 λ����һ����ռ�õ��ֽ���
									   IN int nWidth,						// Դͼ���ؾ����С
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,	// ������
									   IN bool bBGR,						// 24 λ��������ظ�ʽΪ R G B��B�ڵ�λ��
									   IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									   IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUV16_2_RGB24_Split)(OUT unsigned char *pucRGB24TopField,		// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									   IN intptr_t nRGB24TopPitch,				// 24 λ����һ����ռ�õ��ֽ���
									   OUT unsigned char *pucRGB24BottomField,	// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
									   IN intptr_t nRGB24BottomPitch,			// 24 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUV16Frame,			// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									   IN intptr_t nYUV16Pitch,					// 16 λ����һ����ռ�õ��ֽ���
									   IN int nWidth,							// Դͼ���ؾ����С
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,		// ������
									   IN bool bBGR,							// 24 λ��������ظ�ʽΪ R G B��B�ڵ�λ��
									   IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									   IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUV16_2_RGB24_Compose)(OUT unsigned char *pucRGB24Frame,		// 24 λ���壨�洢��ʽ X0 X1 X2 X0 X1 X2 ...��
										 IN intptr_t nRGB24Pitch,				// 24 λ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucYUV16TopField,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										 IN intptr_t nYUV16TopPitch,			// 16 λ����һ����ռ�õ��ֽ���
										 IN unsigned char *pucYUV16BottomField,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										 IN intptr_t nYUV16BottomPitch,			// 16 λ����һ����ռ�õ��ֽ���
										 IN int nWidth,							// Դͼ���ؾ����С
										 IN int nHeight,
										 IN NXVIDEO_DEFINITION eDefinition,		// ������
										 IN bool bBGR,							// 24 λ��������ظ�ʽΪ R G B��B�ڵ�λ��
										 IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
										 IN bool bOverturn);					// ��תͼ���־
// 16 λ YUYV/UYVY ��ɫ�� 32 λ YUVA ��ɫ�ı任��A �����Ϊͳһֵ���ڶ��� UV ֵ����ǰ�� UV ���в�ֵ
typedef void (*DefYUV16_2_YUVA_Frame)(OUT unsigned char *pucYUVAFrame,	// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
									  IN intptr_t nYUVAPitch,			// 32 λ����һ����ռ�õ��ֽ���
									  IN unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									  IN intptr_t nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
									  IN int nWidth,					// Դͼ���ؾ����С
									  IN int nHeight,
									  IN bool bUYVY,					// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									  IN unsigned char ucAlpha,			// ���� A ֵ
									  IN bool bOverturn);				// ��תͼ���־
typedef void (*DefYUV16_2_YUVA_Split)(OUT unsigned char *pucYUVATopField,		// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
									  IN intptr_t nYUVATopPitch,				// 32 λ����һ����ռ�õ��ֽ���
									  OUT unsigned char *pucYUVABottomField,	// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
									  IN intptr_t nYUVABottomPitch,				// 32 λ����һ����ռ�õ��ֽ���
									  IN unsigned char *pucYUV16Frame,			// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									  IN intptr_t nYUV16Pitch,					// 16 λ����һ����ռ�õ��ֽ���
									  IN int nWidth,							// Դͼ���ؾ����С
									  IN int nHeight,
									  IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									  IN unsigned char ucAlpha,					// ���� A ֵ
									  IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUV16_2_YUVA_Compose)(OUT unsigned char *pucYUVAFrame,		// 32 λ���棨�洢��ʽ Y0 U0 V0 A Y1 U1 V1 A ...��
										IN intptr_t nYUVAPitch,					// 32 λ����һ����ռ�õ��ֽ���
										IN unsigned char *pucYUV16TopField,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										IN intptr_t nYUV16TopPitch,				// 16 λ����һ����ռ�õ��ֽ���
										IN unsigned char *pucYUV16BottomField,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										IN intptr_t nYUV16BottomPitch,			// 16 λ����һ����ռ�õ��ֽ���
										IN int nWidth,							// Դͼ���ؾ����С
										IN int nHeight,
										IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
										IN unsigned char ucAlpha,				// ���� A ֵ
										IN bool bOverturn);						// ��תͼ���־
// 16 λ YUYV/UYVY ��ɫ����Ƭ YUV422/YUV420 �ı任��YUV420 ʱ UV �����в�ֵ��һ��
typedef void (*DefYUV16_2_YUVSlice_Frame)(OUT unsigned char *pucYFrame,			// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYPitch,					// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucUUFrame,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUPitch,					// ��Ƭ U ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucVVFrame,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVPitch,					// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucYUV16Frame,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										  IN intptr_t nYUV16Pitch,				// 16 λ����һ����ռ�õ��ֽ���
										  IN int nWidth,						// Դͼ���ؾ����С
										  IN int nHeight,
										  IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
										  IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUV16_2_YUVSlice_Split)(OUT unsigned char *pucYTopField,		// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYTopPitch,				// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucYBottomField,	// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
										  IN intptr_t nYBottomPitch,			// ��Ƭ Y ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucUUTopField,		// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUTopPitch,				// ��Ƭ U ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucUUBottomField,	// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
										  IN intptr_t nUUBottomPitch,			// ��Ƭ U ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucVVTopField,		// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVTopPitch,				// ��Ƭ V ����һ����ռ�õ��ֽ���
										  OUT unsigned char *pucVVBottomField,	// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
										  IN intptr_t nVVBottomPitch,			// ��Ƭ V ����һ����ռ�õ��ֽ���
										  IN unsigned char *pucYUV16Frame,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
										  IN intptr_t nYUV16Pitch,				// 16 λ����һ����ռ�õ��ֽ���
										  IN int nWidth,						// Դͼ���ؾ����С
										  IN int nHeight,
										  IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
										  IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUV16_2_YUVSlice_Compose)(OUT unsigned char *pucYFrame,				// ��Ƭ Y ���壨�洢��ʽ Y0 Y1 Y2 Y3 ������
											IN intptr_t nYPitch,						// ��Ƭ Y ����һ����ռ�õ��ֽ���
											OUT unsigned char *pucUUFrame,				// ��Ƭ U ���壨�洢��ʽ UU0 UU2 UU4 ������
											IN intptr_t nUUPitch,						// ��Ƭ U ����һ����ռ�õ��ֽ���
											OUT unsigned char *pucVVFrame,				// ��Ƭ V ���壨�洢��ʽ VV0 VV2 VV4 ������
											IN intptr_t nVVPitch,						// ��Ƭ V ����һ����ռ�õ��ֽ���
											IN unsigned char *pucYUV16TopField,			// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
											IN intptr_t nYUV16TopPitch,					// 16 λ����һ����ռ�õ��ֽ���
											IN unsigned char *pucYUV16BottomField,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
											IN intptr_t nYUV16BottomPitch,				// 16 λ����һ����ռ�õ��ֽ���
											IN int nWidth,								// Դͼ���ؾ����С
											IN int nHeight,
											IN bool bUYVY,								// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
											IN bool bOverturn);							// ��תͼ���־
// 16 λ YUYV/UYVY ��ɫ�� 24 λ YUV ��ɫ�ı任���ڶ��� UV ֵ����ǰ�� UV ���в�ֵ
typedef void (*DefYUV16_2_YUV_Frame)(OUT unsigned char *pucYUVFrame,	// 24 λ���棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
									 IN intptr_t nYUVPitch,				// 24 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucYUV16Frame,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									 IN intptr_t nYUV16Pitch,			// 16 λ����һ����ռ�õ��ֽ���
									 IN int nWidth,						// Դͼ���ؾ����С
									 IN int nHeight,
									 IN bool bUYVY,						// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									 IN bool bOverturn);				// ��תͼ���־
typedef void (*DefYUV16_2_YUV_Split)(OUT unsigned char *pucYUVTopField,		// 24 λ���棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
									 IN intptr_t nYUVTopPitch,				// 24 λ����һ����ռ�õ��ֽ���
									 OUT unsigned char *pucYUVBottomField,	// 24 λ���棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
									 IN intptr_t nYUVBottomPitch,			// 24 λ����һ����ռ�õ��ֽ���
									 IN unsigned char *pucYUV16Frame,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									 IN intptr_t nYUV16Pitch,				// 16 λ����һ����ռ�õ��ֽ���
									 IN int nWidth,							// Դͼ���ؾ����С
									 IN int nHeight,
									 IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									 IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUV16_2_YUV_Compose)(OUT unsigned char *pucYUVFrame,			// 24 λ���棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
									   IN intptr_t nYUVPitch,					// 24 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUV16TopField,		// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									   IN intptr_t nYUV16TopPitch,				// 16 λ����һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUV16BottomField,	// 16 λ���棨�洢��ʽ Y0 U0 Y1 V0 / U0 Y0 V0 Y1��
									   IN intptr_t nYUV16BottomPitch,			// 16 λ����һ����ռ�õ��ֽ���
									   IN int nWidth,							// Դͼ���ؾ����С
									   IN int nHeight,
									   IN bool bUYVY,							// 16 λ��������ظ�ʽΪ Y1 V0 Y0 U0��U0 �ڵ�λ��
									   IN bool bOverturn);						// ��תͼ���־
// 10Bit YUV422/YUV420 ��Ƭ�� 8Bit YUV422/YUV420 ��Ƭ�Ļ�ת
typedef void (*DefYUVSlice_2_YUVSlice_Frame)(OUT unsigned char* pucY8Bit,	// 8Bit Y ���
											 IN intptr_t nYPitch8B,			// 8Bit Y һ�е��ֽ���
											 OUT unsigned char* pucU8Bit,	// 8Bit U ���
											 IN intptr_t nUPitch8B,			// 8Bit U һ�е��ֽ���
											 OUT unsigned char* pucVBit,	// 8Bit V ���
											 IN intptr_t nVPitch8B,			// 8Bit V һ�е��ֽ���
											 IN unsigned char* pucY10Bit,	// 10Bit Y ����
											 IN intptr_t nYPitch10B,		// 10Bit Y һ�е��ֽ���
											 IN unsigned char* pucU10Bit,	// 10Bit U ����
											 IN intptr_t nUPitch10B,		// 10Bit U һ�е��ֽ���
											 IN unsigned char* pucV10Bit,	// 10Bit V ����
											 IN intptr_t nVPitch10B,		// 10Bit V һ�е��ֽ���
											 IN int nWidth,					// ͼ����
											 IN int nHeight,
											 IN bool bOverTurn);			// ��ת��־

typedef struct tag_nxyuv16toothers
{
	DefYUV16_Swap_Frame			pfnYUV16_Swap_Frame;
	DefYUV16_Swap_Split			pfnYUV16_Swap_Split;
	DefYUV16_Swap_Compose		pfnYUV16_Swap_Compose;

	DefYUV16_2_RGB32_Frame		pfnYUV16_2_RGB32_Frame;
	DefYUV16_2_RGB32_Split		pfnYUV16_2_RGB32_Split;
	DefYUV16_2_RGB32_Compose	pfnYUV16_2_RGB32_Compose;

	DefYUV16_2_RGB24_Frame		pfnYUV16_2_RGB24_Frame;
	DefYUV16_2_RGB24_Split		pfnYUV16_2_RGB24_Split;
	DefYUV16_2_RGB24_Compose	pfnYUV16_2_RGB24_Compose;

	DefYUV16_2_YUVA_Frame		pfnYUV16_2_YUVA_Frame;
	DefYUV16_2_YUVA_Split		pfnYUV16_2_YUVA_Split;
	DefYUV16_2_YUVA_Compose		pfnYUV16_2_YUVA_Compose;

	DefYUV16_2_YUVSlice_Frame	pfnYUV16_2_YUV422_Frame;
	DefYUV16_2_YUVSlice_Split	pfnYUV16_2_YUV422_Split;
	DefYUV16_2_YUVSlice_Compose	pfnYUV16_2_YUV422_Compose;

	DefYUV16_2_YUVSlice_Frame	pfnYUV16_2_YUV420_Frame;
	DefYUV16_2_YUVSlice_Split	pfnYUV16_2_YUV420_Split;
	DefYUV16_2_YUVSlice_Compose	pfnYUV16_2_YUV420_Compose;

	DefYUV16_2_YUV_Frame		pfnYUV16_2_YUV_Frame;
	DefYUV16_2_YUV_Split		pfnYUV16_2_YUV_Split;
	DefYUV16_2_YUV_Compose		pfnYUV16_2_YUV_Compose;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXYUV16toOthers, *LPNXYUV16TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUV422 => Others

typedef struct tag_nxyuv422toothers
{
	DefYUVSlice_2_RGB32_Frame	pfnYUV422_2_RGB32_Frame;
	DefYUVSlice_2_RGB32_Split	pfnYUV422_2_RGB32_Split;
	DefYUVSlice_2_RGB32_Compose	pfnYUV422_2_RGB32_Compose;

	DefYUVSlice_2_RGB24_Frame	pfnYUV422_2_RGB24_Frame;
	DefYUVSlice_2_RGB24_Split	pfnYUV422_2_RGB24_Split;
	DefYUVSlice_2_RGB24_Compose	pfnYUV422_2_RGB24_Compose;

	DefYUVSlice_2_YUVA_Frame	pfnYUV422_2_YUVA_Frame;
	DefYUVSlice_2_YUVA_Split	pfnYUV422_2_YUVA_Split;
	DefYUVSlice_2_YUVA_Compose	pfnYUV422_2_YUVA_Compose;

	DefYUVSlice_2_YUV16_Frame	pfnYUV422_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV422_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV422_2_YUV16_Compose;

	DefYUVSlice_2_YUV16_Frame	pfnYUV422_10Bit_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV422_10Bit_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV422_10Bit_2_YUV16_Compose;

	DefYUVSlice_2_YUV_Frame		pfnYUV422_2_YUV_Frame;
	DefYUVSlice_2_YUV_Split		pfnYUV422_2_YUV_Split;
	DefYUVSlice_2_YUV_Compose	pfnYUV422_2_YUV_Compose;

	DefYUVSlice_ConvertUV_Frame		pfnYUV422_2_YUV420_UV_Frame;
	DefYUVSlice_ConvertUV_Split		pfnYUV422_2_YUV420_UV_Split;
	DefYUVSlice_ConvertUV_Compose	pfnYUV422_2_YUV420_UV_Compose;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.7.0
	DefYUVSlice_2_YUVSlice_Frame	pfnYUV422_10Bit_2_YUV422_8Bit_Frame;
	DefYUVSlice_ConvertUV_Field		pfnYUV422_2_YUV420_UV_Field;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXYUV422toOthers, *LPNXYUV422TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUV420 => Others

typedef struct tag_nxyuv420toothers
{
	DefYUVSlice_2_RGB32_Frame	pfnYUV420_2_RGB32_Frame;
	DefYUVSlice_2_RGB32_Split	pfnYUV420_2_RGB32_Split;
	DefYUVSlice_2_RGB32_Compose	pfnYUV420_2_RGB32_Compose;

	DefYUVSlice_2_RGB24_Frame	pfnYUV420_2_RGB24_Frame;
	DefYUVSlice_2_RGB24_Split	pfnYUV420_2_RGB24_Split;
	DefYUVSlice_2_RGB24_Compose	pfnYUV420_2_RGB24_Compose;

	DefYUVSlice_2_YUVA_Frame	pfnYUV420_2_YUVA_Frame;
	DefYUVSlice_2_YUVA_Split	pfnYUV420_2_YUVA_Split;
	DefYUVSlice_2_YUVA_Compose	pfnYUV420_2_YUVA_Compose;

	DefYUVSlice_2_YUV16_Frame	pfnYUV420_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV420_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV420_2_YUV16_Compose;

	DefYUVSlice_2_YUV16_Frame	pfnYUV420_10Bit_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV420_10Bit_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV420_10Bit_2_YUV16_Compose;

	DefYUVSlice_2_YUV_Frame		pfnYUV420_2_YUV_Frame;
	DefYUVSlice_2_YUV_Split		pfnYUV420_2_YUV_Split;
	DefYUVSlice_2_YUV_Compose	pfnYUV420_2_YUV_Compose;

	DefYUVSlice_ConvertUV_Frame	pfnYUV420_2_YUV422_UV_Frame;
	DefYUVSlice_ConvertUV_Split	pfnYUV420_2_YUV422_UV_Split;
	DefYUVSlice_ConvertUV_Compose pfnYUV420_2_YUV422_UV_Compose;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.7.0
	DefYUVSlice_2_YUVSlice_Frame	pfnYUV420_10Bit_2_YUV420_8Bit_Frame;
	DefYUVSlice_ConvertUV_Field		pfnYUV420_2_YUV422_UV_Field;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXYUV420toOthers, *LPNXYUV420TOOTHERS;

//////////////////////////////////////////////////////////////////////////
// YUV => Others

// 24 λ YUV ��ɫ�� 32 λ BGRA/RGBA ��ɫ�ı任��A �����Ϊͳһֵ
typedef void (*DefYUV_2_RGB32_Frame)(OUT unsigned char *pucRGB32Frame,		// 32 λ������壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									 IN intptr_t nRGB32Pitch,				// 32 λ�������һ����ռ�õ��ֽ���
									 IN unsigned char *pucYUVFrame,			// 24 λ���뻺�棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
									 IN intptr_t nYUVPitch,					// 24 λ���뻺��һ����ռ�õ��ֽ���
									 IN int nWidth,							// Դͼ���ؾ����С
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,		// ������
									 IN bool bBGRA,							// 32 λ�����������ظ�ʽΪ A R G B��B�ڵ�λ���ı�־
									 IN unsigned char cAlpha,				// ���� A ֵ
									 IN bool bOverturn);					// ��תͼ���־
typedef void (*DefYUV_2_RGB32_Split)(OUT unsigned char *pucRGB32TopField,		// 32 λ������壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									 IN intptr_t nRGB32TopPitch,				// 32 λ�������һ����ռ�õ��ֽ���
									 OUT unsigned char *pucRGB32BottomField,	// 32 λ������壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									 IN intptr_t nRGB32BottomPitch,				// 32 λ�������һ����ռ�õ��ֽ���
									 IN unsigned char *pucYUVFrame,				// 24 λ���뻺�棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
									 IN intptr_t nYUVPitch,						// 24 λ���뻺��һ����ռ�õ��ֽ���
									 IN int nWidth,								// Դͼ���ؾ����С
									 IN int nHeight,
									 IN NXVIDEO_DEFINITION eDefinition,			// ������
									 IN bool bBGRA,								// 32 λ�����������ظ�ʽΪ A R G B��B�ڵ�λ��
									 IN unsigned char cAlpha,					// ���� A ֵ
									 IN bool bOverturn);						// ��תͼ���־
typedef void (*DefYUV_2_RGB32_Compose)(OUT unsigned char *pucRGB32Frame,		// 32 λ������壨�洢��ʽ X0 X1 X2 A X0 X1 X2 A ...��
									   IN intptr_t nRGB32Pitch,					// 32 λ�������һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUVTopField,		// 24 λ���뻺�棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
									   IN intptr_t nYUVTopPitch,				// 24 λ���뻺��һ����ռ�õ��ֽ���
									   IN unsigned char *pucYUVBottomField,		// 24 λ���뻺�棨�洢��ʽ Y0 U0 V0 Y1 U1 V1 ...��
									   IN intptr_t nYUVBottomPitch,				// 24 λ���뻺��һ����ռ�õ��ֽ���
									   IN int nWidth,							// Դͼ���ؾ����С
									   IN int nHeight,
									   IN NXVIDEO_DEFINITION eDefinition,		// ������
									   IN bool bBGRA,							// 32 λ�����������ظ�ʽΪ A R G B��B�ڵ�λ��
									   IN unsigned char cAlpha,					// ���� A ֵ
									   IN bool bOverturn);						// ��תͼ���־
// 24 λ YUV ��ɫ�� 24 λ BGR/RGB ��ɫ�ı任
typedef DefYUV_2_RGB_Frame			DefYUV_2_RGB24_Frame;
typedef DefYUV_2_RGB_Split			DefYUV_2_RGB24_Split;
typedef DefYUV_2_RGB_Compose		DefYUV_2_RGB24_Compose;

typedef struct tag_nxyuvtoothers
{
	DefYUV_2_RGB32_Frame		pfnYUV_2_RGB32_Frame;
	DefYUV_2_RGB32_Split		pfnYUV_2_RGB32_Split;
	DefYUV_2_RGB32_Compose		pfnYUV_2_RGB32_Compose;

	DefYUV_2_RGB24_Frame		pfnYUV_2_RGB24_Frame;
	DefYUV_2_RGB24_Split		pfnYUV_2_RGB24_Split;
	DefYUV_2_RGB24_Compose		pfnYUV_2_RGB24_Compose;

	DefYUV_2_YUV16_Frame		pfnYUV_2_YUV16_Frame;
	DefYUV_2_YUV16_Split		pfnYUV_2_YUV16_Split;
	DefYUV_2_YUV16_Compose		pfnYUV_2_YUV16_Compose;

	DefYUV_2_YUVSlice_Frame		pfnYUV_2_YUV422_Frame;
	DefYUV_2_YUVSlice_Split		pfnYUV_2_YUV422_Split;
	DefYUV_2_YUVSlice_Compose	pfnYUV_2_YUV422_Compose;

	DefYUV_2_YUVSlice_Frame		pfnYUV_2_YUV420_Frame;
	DefYUV_2_YUVSlice_Split		pfnYUV_2_YUV420_Split;
	DefYUV_2_YUVSlice_Compose	pfnYUV_2_YUV420_Compose;

	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXYUVtoOthers, *LPNXYUVTOOTHERS;

typedef struct tag_nxcolorinnerapi *LPNXCOLORINNERAPI;

typedef struct tag_nxcolorspaceapi
{
	LPNXCOLORGENERICAPI		pStuColorGenericAPI;
	LPNXRGB32TOOTHERS		pStuRGB32toOthers;
	LPNXRGB24TOOTHERS		pStuRGB24toOthers;
	LPNXYUVATOOTHERS		pStuYUVAtoOthers;
	LPNXYUV16TOOTHERS		pStuYUV16toOthers;
	LPNXYUV422TOOTHERS		pStuYUV422toOthers;
	LPNXYUV420TOOTHERS		pStuYUV420toOthers;
	LPNXYUVTOOTHERS			pStuYUVtoOthers;
	// Private Ȩ�ޣ������Ż����ڲ��Ľӿ�
	LPNXCOLORINNERAPI		pStuInnerAPI;
} NXColorSpaceAPI, *LPNXCOLORSPACEAPI;


#endif		// #ifndef __NX_COLOR_SPACE_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
