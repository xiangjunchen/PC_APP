//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ�źŵĻ������壬����������ɨ�跽ʽ��������.
//				��Ƶ����ӿڵĻ�������.
//
// Created: ZhaoZhimeng @ 2008-11-27
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_VIDEO_PROC_DEF_H__
#define __NX_VIDEO_PROC_DEF_H__

#include ".\NXBasic.h"
#include ".\NXColorSpaceDef.h"

//////////////////////////////////////////////////////////////////////////
// ��������

#ifndef interface
#define interface	struct
#endif		// #ifndef interface

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

#define NXVIDEOPROC_DLL_NAME		_T("NXVideoProc")
#define NXVIDEOPROC_API_EXT_NAME	"NVP_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// ��Ƶ�źŵļ���������������
enum NXVIDEO_ASPECT
{
	NXVIDEO_ASPECT_16_9,			// 16:9
	NXVIDEO_ASPECT_4_3,				// 4:3
	NXVIDEO_ASPECT_ARBITRARY = 3,	// �������
	NXVIDEO_ASPECT_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// ��Ƶ�źŵ����䷽ʽ
enum NXVIDEO_ADAPTION
{
	NXVIDEO_ADAPTION_HEIGHT,		// ���߶�����
	NXVIDEO_ADAPTION_WIDTH,			// ���������
	NXVIDEO_ADAPTION_NONE,			// ������
	NXVIDEO_ADAPTION_CENTER = 4,	// ����
	NXVIDEO_ADAPTION_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// ��Ƶ�źŵ�ɨ�跽ʽ
enum NXVIDEO_SCAN
{
	NXVIDEO_SCAN_INTERLACED_TOP,		// ����ɨ�趥��
	NXVIDEO_SCAN_INTERLACED = NXVIDEO_SCAN_INTERLACED_TOP,
	NXVIDEO_SCAN_INTERLACED_BOTTOM,		// ����ɨ��׳�

	NXVIDEO_SCAN_PROGRESSIVE,			// ����ɨ��

	NXVIDEO_SCAN_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// ���������
enum NXVIDEO_ANTIALIAS
{
	NXVIDEO_ANTIALIAS_4,		// ʹ�ý�ֹƵ�ʱ�Ϊ 0.5 �� 3x3 ������˲���
	NXVIDEO_ANTIALIAS_NONE,		// ��ʹ�ÿ����

	NXVIDEO_ANTIALIAS_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// ��Ƶ�ź��ز����ıƽ����������봰�ڴ�С
enum NXVIDEO_SAM_WINDOW
{
	NXVIDEO_SAM_WINDOW_SONY,
	NXVIDEO_SAM_WINDOW_KAISER,		// �Ƽ�ʹ��
	NXVIDEO_SAM_WINDOW_HAMMING,
	NXVIDEO_SAM_WINDOW_LANCZOS,
	NXVIDEO_SAM_WINDOW_BLACKMAN,

	NXVIDEO_SAM_WINDOW_UNKNOWN
};

enum NXVIDEO_SAM_TAPS
{
	NXVIDEO_SAM_TAPS_4,
	NXVIDEO_SAM_TAPS_8,			// �Ƽ�ʹ��
	NXVIDEO_SAM_TAPS_16,
	NXVIDEO_SAM_TAPS_24,

	NXVIDEO_SAM_TAPS_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////
// �˲������Ͷ���
enum NXVIDEO_FILTER_TYPE
{
	NXVIDEO_FILTER_NONE,
	NXVIDEO_FILTER_POINT,			// �ٽ���
	NXVIDEO_FILTER_MEAN,			// ƽ��ֵ
	NXVIDEO_FILTER_MEDIAN,			// ��ֵ
	NXVIDEO_FILTER_LINEAR,			// ���Բ�ֵ
	NXVIDEO_FILTER_CUBIC,			// ���β�ֵ
	NXVIDEO_FILTER_GAUSS,			// ��˹
};

//////////////////////////////////////////////////////////////////////////
// ��άͼ�������ڴ����ݽṹ������ָ������һ�����ڴ������Ч����
typedef struct tag_nxbuf2dImg
{
	unsigned char*		pucBuf;			// �����ڴ���ʼ��ַ
	int					nBufBytes;		// �����ڴ���ܳ���С�ڵ���ʵ��ռ�ó��ȣ����ڵ��� nBaseHeight * nPitch��
	int					nPitch;			// һ�з�����ֽڳ��ȣ����ڵ��� nBaseWidth * (nBitCount >> 3)��
	int					nBitCount;		// һ�����ص���ɫλ��
	NXVIDEO_PIXEL_FMT	ePixelFmt;		// ���صĸ�ʽ
	int					nBaseWidth;		// 2D �ڴ�һ������֧�ֵ�������ظ���
	int					nBaseHeight;	// 2D �ڴ�����֧�ֵ��������
	NXVIDEO_SCAN		eScanType;		// �ź�ɨ�跽ʽ
	double				dX;				// ָ��������㣨����� 2D �ڴ��������꣬���������� nWidth��nHeight ���ָ����������
	double				dY;				// ��С�����ֱ�ʾ��������㣬��ʵ���������ز�����һ��С�����ֵ�ֵΪ 0��
	int					nWidth;			// ָ�������С���� nX��nY ���ָ���������򣩣�һ������±�ʾͼ����棩
	int					nHeight;
	NXVIDEO_ASPECT		eAspect;		// �������
	NXVIDEO_DEFINITION	eDefinition;	// ������
} NXBuf2DImg, *LPNXBUF2DIMG;


//////////////////////////////////////////////////////////////////////////
// ��Ƶ�ź� deinterlacing ʱ�Ĵ��������𣨲�ͬ�Ĳ��������ڲ�ͬ�ĳ��ϣ�
enum NXVIDEO_DEINTERLACE
{
	NXVIDEO_DEINTERLACE_REMOVE_OTHER,		// ȥ�����쳡��Ӱ�죬��������ʽת��/���±任���򵥱��ٵ�
	NXVIDEO_DEINTERLACE_RESERVE_OTHER,		// �������쳡��Ӱ�죬������ 50i->25p �Ķ��� IPC ��

	NXVIDEO_DEINTERLACE_UNKNOWN,
};

typedef int NXVIDEO_DEINTERLACE_ARITH;
enum
{
	// 8bit
	NXVIDEO_DEINTERLACE_TAPS_4,			// ���� V1.0.8.0 ��ǰ�汾 NXVIDEO_SAM_TAPS_4
	NXVIDEO_DEINTERLACE_TAPS_8,			// ���� V1.0.8.0 ��ǰ�汾 NXVIDEO_SAM_TAPS_8
	NXVIDEO_DEINTERLACE_MELA_YUYV,
	NXVIDEO_DEINTERLACE_MELA_UYVY,
	NXVIDEO_DEINTERLACE_MELA_32BIT,
	NXVIDEO_DEINTERLACE_MELA_KEY,
	// 10bit
	NXVIDEO_DEINTERLACE_TAPS_4_10B = NXVIDEO_DEINTERLACE_MELA_KEY + 1000,
	NXVIDEO_DEINTERLACE_TAPS_8_10B,
	NXVIDEO_DEINTERLACE_MELA_YUYV_10B,
	NXVIDEO_DEINTERLACE_MELA_UYVY_10B,
	NXVIDEO_DEINTERLACE_MELA_32BIT_10B,
	NXVIDEO_DEINTERLACE_MELA_KEY_10B,

	NXVIDEO_DEINTERLACE_ARITH_UNKNOWN
};


//////////////////////////////////////////////////////////////////////////
// ��Ƶ�ź�һ֡�ڳ�������Ķ���
enum NXVIDEO_FIELD_PLAY
{
	NXVIDEO_FIELD_PLAY_FIRST,		// ��һ��
	NXVIDEO_FIELD_PLAY_SECOND,		// �ڶ���

	NXVIDEO_FIELD_PLAY_COUNT		// һ֡�ĳ���
};


//////////////////////////////////////////////////////////////////////////
// ������Ƶ�ź��ж�����׳��Ļ���ת���㷨����
enum NXVIDEO_FIELDCONV_ARITH
{
	NXVIDEO_FIELDCONV_AVER_2,	// Ч������
	NXVIDEO_FIELDCONV_SINC_4,	// ��ֱ������������
	NXVIDEO_FIELDCONV_MELA_3,	// ˮƽ������������

	NXVIDEO_FIELDCONV_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////
// ͼ��ϳ�����
enum NX_BLEND_TYPE
{
	NX_BLEND_ALPHA,					// Alpha �ϳ�
	NX_BLEND_NORMAL,				// ����
	NX_BLEND_ADD,					// ���
	NX_BLEND_SUB,					// ���
	NX_BLEND_MUL,					// ��Ƭ����
	NX_BLEND_DIV,					// �ų�
	NX_BLEND_DARKEN,				// �䰵
	NX_BLEND_LIGHTEN,				// ����
	NX_BLEND_SCREEN,				// ��ɫ
	NX_BLEND_HARD_LIGHT,			// ǿ��
	NX_BLEND_SOFT_LIGHT,			// ���
	NX_BLEND_COLOR_BURN,			// ��ɫ����
	NX_BLEND_COLOR_DODGE,			// ��ɫ����
	NX_BLEND_OVERLAY,				// ����
};

//////////////////////////////////////////////////////////////////////////
// �ؼ�����������
enum NXVE_ORIENTATION_TYPE
{
	NXVE_ORIENTATION_HOR,				// ˮƽ
	NXVE_ORIENTATION_VER,				// ��ֱ
	NXVE_ORIENTATION_HOR_VER,			// ˮƽ�ʹ�ֱ
};

//////////////////////////////////////////////////////////////////////////
// �����ӿڶ���

// ��Ƶͼ��任

// ��Ƶ��ʽ�任���Ĵ������ͷ�
interface INXVideoFmtConv;
typedef INXVideoFmtConv* (*DefCreateFmtConv)();
typedef void (*DefReleaseFmtConv)(INXVideoFmtConv* &pFmtConv);
// ��Ƶ��ʽ�任�����ܽӿ���Ĵ������ͷ�
interface INXVideoFmtConvFunc;
typedef INXVideoFmtConvFunc* (*pfnvp_create_fmtconvfunc)();
typedef void (*pfnvp_release_fmtconvfunc)(INXVideoFmtConvFunc*& pFmtConvFunc);
// �򵥱������ӿ���Ĵ������ͷ�
interface INXVideoSpeedProc;
typedef INXVideoSpeedProc* (*DefCreateSpeedProc)();
typedef void (*DefReleaseSpeedProc)(INXVideoSpeedProc* &pSpeedProc);
// ���ٹ��ܽӿ���Ĵ������ͷ�
interface INXVideoSpeedFunc;
typedef INXVideoSpeedFunc* (*DefCreateSpeedFunc)();
typedef void (*DefReleaseSpeedFunc)(INXVideoSpeedFunc* &pSpeedFunc);
// ����������ü������� false ˵������ü������������Ч��
// ��1�����ڼ�����޷���ȷ���ö����Ʊ��ĳЩС�������ĳЩӳ��������Ӷ���ɽ��ͼ����λƫ�ơ�
// �������㾫��ֻ�ܱ�ʾ e^-6�����ڿ� 1920������ϵ������ 64 �������������ۼ������ܻ���ɽ��ƫ�
typedef bool (*DefAspectAdaptCut)(OUT double *pdHorScale,			// ��ʼˮƽ���ű� srcW / desW
								  OUT double *pdVerScale,			// ��ʼ��ֱ���ű� srcH / desH
								  OUT double *pdSrcX,				// ����ü����Դ������㣨��Ŀ���������Ӧ��
								  OUT double *pdSrcY,
								  OUT int *pnSrcWidth,				// ����ü����Դ������Ч�����С����Ŀ�������Ч�����Ӧ��
								  OUT int *pnSrcHeight,
								  OUT int *pnDesX,					// ����ü����Ŀ�������㣨����Ϊ���ͣ�
								  OUT int *pnDesY,
								  OUT int *pnDesWidth,				// ����ü����Ŀ�������Ч�����С
								  OUT int *pnDesHeight,
								  IN const NXBuf2DImg *pSrcInfo,	// Դ�����λ����Ϣ
								  IN const NXBuf2DImg *pDesInfo,	// Ŀ�껺���λ����Ϣ
								  IN NXVIDEO_ADAPTION eAdaption);	// ���䷽ʽ
// Interlacing �� progressive ���źű任������ǰһ֡�����˶�����벹����
// ��1���������������ͼ�������ͬ�����ڲ�����֮��Ĳ�ֵ�����ÿ������ظ�ʽ��
//		��ע��	�ڣ�1����ֻ������ V1.0.8.0 �汾��ǰ�������㷨���� MELA �㷨��ʼ������Ҫ�������ظ�ʽ��
//				Ϊ�˼�����ǰ�İ汾�����㷨�Ķ���������ظ�ʽ�ķ��ࡣ
//				���� nLineProcBytes �ĺ�����Ȼ���䣬�㷨�ڲ�����ͨ��������Ƶó�Դͼ��Ŀ�ȡ�
// ��2������ǰһ֡�뵱ǰ֡��ͼ�������ǰ֡��������ֵ֡���Լ�֡���������ڲ����֡��
// ��3��eSamTaps ����Ŀǰֻ֧�ֵ� NXVIDEO_SAM_TAPS_8��
//		��ע��	�ڣ�3������ eSamTaps ���������� V1.0.8.0 �� NXVIDEO_SAM_TAPS ��Ϊ NXVIDEO_DEINTERLACE_ARITH��
//				�°汾��ȫ���¼�����ǰ���㷨���ͣ��¶���������㷨����չ��
typedef void (*DefDeInterlacing)(OUT unsigned char *pucTopFrame,		// ������ֵ֡
								 IN intptr_t nTopFramePitch,			// һ����ռ�ֽ���
								 OUT unsigned char *pucBottomFrame,		// �׳���ֵ֡
								 IN intptr_t nBottomFramePitch,			// һ����ռ�ֽ���
								 OUT unsigned char *pucFrameSub,		// ֡���
								 IN intptr_t nFrameSubPitch,			// һ����ռ�ֽ���
								 OUT unsigned char *pucFieldSub,		// �����
								 IN intptr_t nFieldSubPitch,			// һ����ռ�ֽ���
								 OUT unsigned char *pucVMove,			// ���ڲ�
								 IN intptr_t nVMovePitch,				// һ����ռ�ֽ���
								 OUT unsigned char *pucImaginary,		// ����֡
								 IN intptr_t nImaginaryPitch,			// һ����ռ�ֽ���
								 IN unsigned char *pucCurrFrame,		// ��ǰ֡
								 IN intptr_t nCurrFramePitch,			// һ����ռ�ֽ���
								 IN unsigned char *pucPrevFrame,		// ǰһ֡
								 IN intptr_t nPrevFramePitch,			// һ����ռ�ֽ���
								 IN int nLineProcBytes,					// һ����������ֽ�����С���������е�pitch��
								 IN int nLineCount,						// �ܹ����������
								 IN NXVIDEO_DEINTERLACE_ARITH eArith,	// �����㷨
								 IN NXVIDEO_DEINTERLACE eDeinterlace);	// ��������
// �����˶��������� KFrameSub��KFieldSub��KVMove���Ŵ� 16 λ��
// ��1���ýӿ����������˶���������Ҫ���ʹ���߲����ã������Ӱ����Ƶ�任������
typedef void (*DefSetMotionDetectCoef)(IN unsigned short usKFrameSub,	// ֡���ϵ��
									   IN unsigned short usKFieldSub,	// �����ϵ��
									   IN unsigned short usKVMove);		// ���ڲ�ϵ��
// ͼ���źŵ��ز���
// ��1���� eSamWindow = NXVIDEO_SAM_WINDOW_SONY ʱ��eSamTaps ֻ֧�ֵ� NXVIDEO_SAM_TAPS_8�����Ҳ���ֻ�������ϲ�����
typedef bool (*DefResampling)(IN OUT LPNXBUF2DIMG pDesImg,			// Ŀ��ͼ��
							  IN const LPNXBUF2DIMG pSrcImg,		// Դͼ��
							  IN NXVIDEO_ADAPTION eAdaption,		// ���䷽ʽ
							  IN NXVIDEO_ANTIALIAS eAntiAlias,		// �����������
							  IN unsigned char *pucAACache,			// ��������壨���ڼ��٣�Ϊ NULL �����ڲ��Զ����룩
							  IN intptr_t nAACachePitch,			// ���������һ����ռ�ֽ���
							  IN NXVIDEO_SAM_WINDOW eSamWindow,		// �ز�����������
							  IN NXVIDEO_SAM_TAPS eSamTaps);		// �ز������ڿ��
// ֡�ʱ任�м���ʵ�ʿɱ任�õ���Ŀ��֡��
// ��1�����ڹ㲥�źŵ�֡�����죬��Ҫ�ֿ����ǣ�ͨ������ NXVIDEO_SCAN_INTERLACED �� NXVIDEO_SCAN_PROGRESSIVE �����ơ�
// ��2��FRC ��ʾ Frame Rate Conversion����ʽת����Ҫ���Դ˺���Ϊ֡�������׼��
typedef int (*DefGetOutDurationFRC)(IN int nInDuration,			// ����֡��
									IN double dInFrameRate,		// ����֡��
									IN NXVIDEO_SCAN eInScan,	// �����ź�ɨ�跽ʽ
									IN double dOutFrameRate,	// ���֡��
									IN NXVIDEO_SCAN eOutScan);	// ����ź�ɨ�跽ʽ
// �������׳���Ļ���ת��
// ��1����Դ��Ŀ��ĳ�λ����ͬʱֱ�ӿ�������ͬ�����ת����
// ��2��NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 ���ַ�Ƭ��ʽҪ��nDesPitch �� nSrcPitch ��ʾ
// Y Ƭ��һ��ռ���ֽ������� U��V Ƭ��ֵΪ Y Ƭֵ��һ�롣
typedef int (*DefFieldConv)(OUT unsigned char *pucDesField,		// Ŀ�곡
							IN intptr_t nDesPitch,				// Ŀ��һ����ռ�õ��ֽ���
							IN bool bDesTopField,				// Ŀ�곡Ϊ������־
							IN unsigned char *pucSrcField,		// Դ��
							IN intptr_t nSrcPitch,				// Դһ����ռ�õ��ֽ���
							IN bool bSrcTopField,				// Դ��Ϊ������־
							IN NXVIDEO_PIXEL_FMT ePixelFmt,		// ���ظ�ʽ
							IN int nWidth,						// ���ؿ��
							IN int nFieldHeight,				// ���߶�
							IN NXVIDEO_FIELDCONV_ARITH eArith);	// ת���㷨
// ��һ��������һ�����ͼ�񿽱�������
// ��Դ��Ŀ�ĵķ�����ͬʱ��ֱ�ӿ����������������
typedef void (*pfnvp_zoom)(__out unsigned char* des,			// Ŀ��
						   __in int des_pitch,					// Ŀ��һ�е��ֽ���
						   __in int des_x,						// Ŀ����ʼ������
						   __in int des_y,
						   __in int des_width,					// Ŀ�Ĳ�������
						   __in int des_height,
						   __in unsigned char const* src,		// Դ
						   __in int src_pitch,					// Դһ�е��ֽ���
						   __in int src_x,						// Դ��ʼ������
						   __in int src_y,
						   __in int src_width,					// Դ��������
						   __in int src_height,
						   __in NXVIDEO_FILTER_TYPE filter,		// Ŀǰֻ֧�� NXVIDEO_FILTER_LINEAR
						   __in NXVIDEO_PIXEL_FMT pixel_fmt);	// ���ظ�ʽ��Դ��Ŀ�ı�����ͬ��
//////////////////////////////////////////////////////////////////////////
// ͼ��ϳ�
typedef NXRESULT (*pfnvp_blend)(__inout unsigned char* des,			// �ײ�ͼ����ɫ
								__in intptr_t des_pitch,			// �ײ�ͼ����ɫһ�е��ֽ���
								__inout unsigned char* des_alpha,	// �ײ�ͼ�� A ͨ��������Ϊ NULL
								__in intptr_t des_alpha_pitch,		// �ײ�ͼ�� A ͨ��һ�е��ֽ���
								__in int des_x,						// �ײ�ͼ�����������
								__in int des_y,
								__in int des_total_width,			// �ײ�ͼ���ܷ���
								__in int des_total_height,
								__in unsigned char* src,			// ����ͼ����ɫ
								__in intptr_t src_pitch,			// ����ͼ����ɫһ�е��ֽ���
								__in unsigned char* src_alpha,		// ����ͼ�� A ͨ��������Ϊ NULL
								__in intptr_t src_alpha_pitch,		// ����ͼ�� A ͨ��һ�е��ֽ���
								__in int src_x,						// ����ͼ�����������
								__in int src_y,
								__in int src_total_width,			// ����ͼ���ܷ���
								__in int src_total_height,
								__in NXVIDEO_PIXEL_FMT pixel_fmt,	// ͼ�����ظ�ʽ��Դ��Ŀ����ͬ
								__in NX_BLEND_TYPE blend_type,		// ͼ��ϳ�����
								__in int proc_width,				// ������棬Դ��Ŀ����ͬ
								__in int proc_height);
//////////////////////////////////////////////////////////////////////////
// �Ӷ�������������һ���»���
// ˮƽ����src_x �ķ�ΧΪ [0, count * src_width)
// ��ֱ����src_y �ķ�ΧΪ [0, count * src_height)
typedef NXRESULT (*pfnvp_pages_roll)(__out unsigned char* des,		// ��������
									 __in intptr_t des_pitch,		// ��������һ�е��ֽ���
									 __in int des_x,				// �����������
									 __in int des_y,
									 __in int des_width,			// �����������
									 __in int des_height,
									 __in unsigned char** src,		// ��������������׵�ַ
									 __in intptr_t* src_pitch,		// ��������һ�е��ֽ����������׵�ַ
									 __in int count,				// �����������
									 __in int src_width,			// ��������ÿ���ķ���
									 __in int src_height,
									 __in double proc_x,			// ��������
									 __in double proc_y,
									 __in int proc_width,
									 __in int proc_height,
									 __in NXVIDEO_PIXEL_FMT pixel_fmt,
									 __in NXVIDEO_SAM_WINDOW window,
									 __in NXVIDEO_SAM_TAPS taps,
									 __in NXVE_ORIENTATION_TYPE orientation);

//////////////////////////////////////////////////////////////////////////
typedef struct  tag_nxvideoconvapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	// ��Ƶ��ʽת�����Ĵ������ͷ�
	DefCreateFmtConv		pfnCreateFmtConv;
	DefReleaseFmtConv		pfnReleaseFmtConv;
	// �򵥱������Ĵ������ͷ�
	DefCreateSpeedProc		pfnCreateSpeedProc;
	DefReleaseSpeedProc		pfnReleaseSpeedProc;
	// ����������ü�
	DefAspectAdaptCut		pfnAspectAdaptCut;
	// �任
	DefDeInterlacing		pfnDeInterlacing;
	DefSetMotionDetectCoef	pfnSetMotionDetectCoef;		// Ҫ������߲�ʹ�øýӿ�
	DefResampling			pfnResampling;
	// ����ϳ�
	DefSplitFrame			pfnSplitFrame;
	DefComposeFields		pfnComposeFields;
	// ֡�ʻ���
	DefGetOutDurationFRC	pfnGetOutDurationFRC;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.7.0
	// ���ٹ��ܽӿ���Ĵ������ͷ�
	DefCreateSpeedFunc		pfnCreateSpeedFunc;
	DefReleaseSpeedFunc		pfnReleaseSpeedFunc;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// ������׳���Ļ���ת������
	DefFieldConv			pfnFieldConv;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.8
	pfnvp_zoom				zoom;
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.21
	pfnvp_blend				blend;		// ͼ��ϳ�
	// V1.1.0.3
	pfnvp_create_fmtconvfunc	create_fmtconvfunc;
	pfnvp_release_fmtconvfunc	release_fmtconvfunc;
	pfnvp_pages_roll		pages_roll;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����￪ʼ
} NXVideoConvAPI, *LPNXVIDEOCONVAPI;


// ��ɫ�任

typedef struct  tag_nxvideocolorapi
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	// ͳһ�ӿ�
	DefColorConvFrame		pfnColorConvFrame;
	DefColorConvSplit		pfnColorConvSplit;
	DefColorConvCompose		pfnColorConvCompose;
	// YUVA <-> YUYV/UYVY
	DefYUV16_2_YUVA_Frame	pfnYUV16_2_YUVA_Frame;
	DefYUV16_2_YUVA_Split	pfnYUV16_2_YUVA_Split;
	DefYUV16_2_YUVA_Compose	pfnYUV16_2_YUVA_Compose;
	DefYUVA_2_YUV16_Frame	pfnYUVA_2_YUV16_Frame;
	DefYUVA_2_YUV16_Split	pfnYUVA_2_YUV16_Split;
	DefYUVA_2_YUV16_Compose	pfnYUVA_2_YUV16_Compose;
	// YUV <-> YUYV/UYVY
	DefYUV16_2_YUV_Frame	pfnYUV16_2_YUV_Frame;
	DefYUV16_2_YUV_Split	pfnYUV16_2_YUV_Split;
	DefYUV16_2_YUV_Compose	pfnYUV16_2_YUV_Compose;
	DefYUV_2_YUV16_Frame	pfnYUV_2_YUV16_Frame;
	DefYUV_2_YUV16_Split	pfnYUV_2_YUV16_Split;
	DefYUV_2_YUV16_Compose	pfnYUV_2_YUV16_Compose;
	// RGB/BGR/YUV <-> RGBA/BGRA/YUVA
	DefCr24_2_Cr32_Frame	pfnCr24_2_Cr32_Frame;
	DefCr24_2_Cr32_Split	pfnCr24_2_Cr32_Split;
	DefCr24_2_Cr32_Compose	pfnCr24_2_Cr32_Compose;
	DefCr32_2_Cr24_Frame	pfnCr32_2_Cr24_Frame;
	DefCr32_2_Cr24_Split	pfnCr32_2_Cr24_Split;
	DefCr32_2_Cr24_Compose	pfnCr32_2_Cr24_Compose;
	// YUYV/UYVY <-> RGBA/BGRA
	DefYUV16_2_RGB32_Frame		pfnYUV16_2_RGB32_Frame;
	DefYUV16_2_RGB32_Split		pfnYUV16_2_RGB32_Split;
	DefYUV16_2_RGB32_Compose	pfnYUV16_2_RGB32_Compose;
	DefRGB32_2_YUV16_Frame		pfnRGB32_2_YUV16_Frame;
	DefRGB32_2_YUV16_Split		pfnRGB32_2_YUV16_Split;
	DefRGB32_2_YUV16_Compose	pfnRGB32_2_YUV16_Compose;
	// YUYV/UYVY <-> RGB/BGR
	DefYUV16_2_RGB24_Frame		pfnYUV16_2_RGB24_Frame;
	DefYUV16_2_RGB24_Split		pfnYUV16_2_RGB24_Split;
	DefYUV16_2_RGB24_Compose	pfnYUV16_2_RGB24_Compose;
	DefRGB24_2_YUV16_Frame		pfnRGB24_2_YUV16_Frame;
	DefRGB24_2_YUV16_Split		pfnRGB24_2_YUV16_Split;
	DefRGB24_2_YUV16_Compose	pfnRGB24_2_YUV16_Compose;
	// RGBA/BGRA <-> YUVA
	DefYUVA_2_RGB32_Frame		pfnYUVA_2_RGB32_Frame;
	DefYUVA_2_RGB32_Split		pfnYUVA_2_RGB32_Split;
	DefYUVA_2_RGB32_Compose		pfnYUVA_2_RGB32_Compose;
	DefRGB32_2_YUVA_Frame		pfnRGB32_2_YUVA_Frame;
	DefRGB32_2_YUVA_Split		pfnRGB32_2_YUVA_Split;
	DefRGB32_2_YUVA_Compose		pfnRGB32_2_YUVA_Compose;
	// RGB/BGR <-> YUVA
	DefYUVA_2_RGB24_Frame		pfnYUVA_2_RGB24_Frame;
	DefYUVA_2_RGB24_Split		pfnYUVA_2_RGB24_Split;
	DefYUVA_2_RGB24_Compose		pfnYUVA_2_RGB24_Compose;
	DefRGB24_2_YUVA_Frame		pfnRGB24_2_YUVA_Frame;
	DefRGB24_2_YUVA_Split		pfnRGB24_2_YUVA_Split;
	DefRGB24_2_YUVA_Compose		pfnRGB24_2_YUVA_Compose;
	// YUV <-> RGBA/BGRA
	DefYUV_2_RGB32_Frame		pfnYUV_2_RGB32_Frame;
	DefYUV_2_RGB32_Split		pfnYUV_2_RGB32_Split;
	DefYUV_2_RGB32_Compose		pfnYUV_2_RGB32_Compose;
	DefRGB32_2_YUV_Frame		pfnRGB32_2_YUV_Frame;
	DefRGB32_2_YUV_Split		pfnRGB32_2_YUV_Split;
	DefRGB32_2_YUV_Compose		pfnRGB32_2_YUV_Compose;
	// YUV <-> RGB/BGR
	DefYUV_2_RGB24_Frame		pfnYUV_2_RGB24_Frame;
	DefYUV_2_RGB24_Split		pfnYUV_2_RGB24_Split;
	DefYUV_2_RGB24_Compose		pfnYUV_2_RGB24_Compose;
	DefRGB24_2_YUV_Frame		pfnRGB24_2_YUV_Frame;
	DefRGB24_2_YUV_Split		pfnRGB24_2_YUV_Split;
	DefRGB24_2_YUV_Compose		pfnRGB24_2_YUV_Compose;
	// RGBA <-> BGRA
	DefRGB32_SwapRB_Frame		pfnRGB32_SwapRB_Frame;
	DefRGB32_SwapRB_Split		pfnRGB32_SwapRB_Split;
	DefRGB32_SwapRB_Compose		pfnRGB32_SwapRB_Compose;
	// RGB <-> BGR
	DefRGB24_SwapRB_Frame		pfnRGB24_SwapRB_Frame;
	DefRGB24_SwapRB_Split		pfnRGB24_SwapRB_Split;
	DefRGB24_SwapRB_Compose		pfnRGB24_SwapRB_Compose;
	// UYVY <-> YUYV
	DefYUV16_Swap_Frame			pfnYUV16_Swap_Frame;
	DefYUV16_Swap_Split			pfnYUV16_Swap_Split;
	DefYUV16_Swap_Compose		pfnYUV16_Swap_Compose;
	// YUV422 <-> YUYV/UYVY
	DefYUVSlice_2_YUV16_Frame	pfnYUV422_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV422_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV422_2_YUV16_Compose;
	DefYUV16_2_YUVSlice_Frame	pfnYUV16_2_YUV422_Frame;
	DefYUV16_2_YUVSlice_Split	pfnYUV16_2_YUV422_Split;
	DefYUV16_2_YUVSlice_Compose	pfnYUV16_2_YUV422_Compose;
	// YUV420 <-> YUYV/UYVY
	DefYUVSlice_2_YUV16_Frame	pfnYUV420_2_YUV16_Frame;
	DefYUVSlice_2_YUV16_Split	pfnYUV420_2_YUV16_Split;
	DefYUVSlice_2_YUV16_Compose	pfnYUV420_2_YUV16_Compose;
	DefYUV16_2_YUVSlice_Frame	pfnYUV16_2_YUV420_Frame;
	DefYUV16_2_YUVSlice_Split	pfnYUV16_2_YUV420_Split;
	DefYUV16_2_YUVSlice_Compose	pfnYUV16_2_YUV420_Compose;
	// YUV422 <-> YUVA
	DefYUVSlice_2_YUVA_Frame	pfnYUV422_2_YUVA_Frame;
	DefYUVSlice_2_YUVA_Split	pfnYUV422_2_YUVA_Split;
	DefYUVSlice_2_YUVA_Compose	pfnYUV422_2_YUVA_Compose;
	DefYUVA_2_YUVSlice_Frame	pfnYUVA_2_YUV422_Frame;
	DefYUVA_2_YUVSlice_Split	pfnYUVA_2_YUV422_Split;
	DefYUVA_2_YUVSlice_Compose	pfnYUVA_2_YUV422_Compose;
	// YUV420 <-> YUVA
	DefYUVSlice_2_YUVA_Frame	pfnYUV420_2_YUVA_Frame;
	DefYUVSlice_2_YUVA_Split	pfnYUV420_2_YUVA_Split;
	DefYUVSlice_2_YUVA_Compose	pfnYUV420_2_YUVA_Compose;
	DefYUVA_2_YUVSlice_Frame	pfnYUVA_2_YUV420_Frame;
	DefYUVA_2_YUVSlice_Split	pfnYUVA_2_YUV420_Split;
	DefYUVA_2_YUVSlice_Compose	pfnYUVA_2_YUV420_Compose;
	// YUV422 <-> YUV
	DefYUVSlice_2_YUV_Frame		pfnYUV422_2_YUV_Frame;
	DefYUVSlice_2_YUV_Split		pfnYUV422_2_YUV_Split;
	DefYUVSlice_2_YUV_Compose	pfnYUV422_2_YUV_Compose;
	DefYUV_2_YUVSlice_Frame		pfnYUV_2_YUV422_Frame;
	DefYUV_2_YUVSlice_Split		pfnYUV_2_YUV422_Split;
	DefYUV_2_YUVSlice_Compose	pfnYUV_2_YUV422_Compose;
	// YUV420 <-> YUV
	DefYUVSlice_2_YUV_Frame		pfnYUV420_2_YUV_Frame;
	DefYUVSlice_2_YUV_Split		pfnYUV420_2_YUV_Split;
	DefYUVSlice_2_YUV_Compose	pfnYUV420_2_YUV_Compose;
	DefYUV_2_YUVSlice_Frame		pfnYUV_2_YUV420_Frame;
	DefYUV_2_YUVSlice_Split		pfnYUV_2_YUV420_Split;
	DefYUV_2_YUVSlice_Compose	pfnYUV_2_YUV420_Compose;
	// YUV422 <-> RGBA/BGRA
	DefYUVSlice_2_RGB32_Frame	pfnYUV422_2_RGB32_Frame;
	DefYUVSlice_2_RGB32_Split	pfnYUV422_2_RGB32_Split;
	DefYUVSlice_2_RGB32_Compose	pfnYUV422_2_RGB32_Compose;
	DefRGB32_2_YUVSlice_Frame	pfnRGB32_2_YUV422_Frame;
	DefRGB32_2_YUVSlice_Split	pfnRGB32_2_YUV422_Split;
	DefRGB32_2_YUVSlice_Compose	pfnRGB32_2_YUV422_Compose;
	// YUV420 <-> RGBA/BGRA
	DefYUVSlice_2_RGB32_Frame	pfnYUV420_2_RGB32_Frame;
	DefYUVSlice_2_RGB32_Split	pfnYUV420_2_RGB32_Split;
	DefYUVSlice_2_RGB32_Compose	pfnYUV420_2_RGB32_Compose;
	DefRGB32_2_YUVSlice_Frame	pfnRGB32_2_YUV420_Frame;
	DefRGB32_2_YUVSlice_Split	pfnRGB32_2_YUV420_Split;
	DefRGB32_2_YUVSlice_Compose	pfnRGB32_2_YUV420_Compose;
	// YUV422 <-> RGB/BGR
	DefYUVSlice_2_RGB24_Frame	pfnYUV422_2_RGB24_Frame;
	DefYUVSlice_2_RGB24_Split	pfnYUV422_2_RGB24_Split;
	DefYUVSlice_2_RGB24_Compose	pfnYUV422_2_RGB24_Compose;
	DefRGB24_2_YUVSlice_Frame	pfnRGB24_2_YUV422_Frame;
	DefRGB24_2_YUVSlice_Split	pfnRGB24_2_YUV422_Split;
	DefRGB24_2_YUVSlice_Compose	pfnRGB24_2_YUV422_Compose;
	// YUV420 <-> RGB/BGR
	DefYUVSlice_2_RGB24_Frame	pfnYUV420_2_RGB24_Frame;
	DefYUVSlice_2_RGB24_Split	pfnYUV420_2_RGB24_Split;
	DefYUVSlice_2_RGB24_Compose	pfnYUV420_2_RGB24_Compose;
	DefRGB24_2_YUVSlice_Frame	pfnRGB24_2_YUV420_Frame;
	DefRGB24_2_YUVSlice_Split	pfnRGB24_2_YUV420_Split;
	DefRGB24_2_YUVSlice_Compose	pfnRGB24_2_YUV420_Compose;
	// YUV420 <-> YUV422 ֮ UV �任
	DefYUVSlice_ConvertUV_Frame		pfnYUV422_2_YUV420_UV_Frame;
	DefYUVSlice_ConvertUV_Split		pfnYUV422_2_YUV420_UV_Split;
	DefYUVSlice_ConvertUV_Compose	pfnYUV422_2_YUV420_UV_Compose;
	DefYUVSlice_ConvertUV_Frame		pfnYUV420_2_YUV422_UV_Frame;
	DefYUVSlice_ConvertUV_Split		pfnYUV420_2_YUV422_UV_Split;
	DefYUVSlice_ConvertUV_Compose	pfnYUV420_2_YUV422_UV_Compose;
	// RGBA/BGRA/YUVA -> Channel
	DefCr32_Split_Channel_Frame		pfnCr32_Split_Channel_Frame;
	DefCr32_Split_Channel_Split		pfnCr32_Split_Channel_Split;
	DefCr32_Split_Channel_Compose	pfnCr32_Split_Channel_Compose;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXVideoColorAPI, *LPNXVIDEOCOLORAPI;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// �ӿڽṹ����

typedef struct tag_nxvideoinnerapi *LPNXVIDEOINNERAPI;

typedef struct tag_nxvideoprocapi
{
	// ��Ƶͼ��任�ӿ�
	LPNXVIDEOCONVAPI		pStuVideoConvAPI;
	// ��ɫ�任�ӿ�
	LPNXVIDEOCOLORAPI		pStuVideoColorAPI;
	// Private Ȩ�ޣ������Ż����ڲ��Ľӿ�
	LPNXVIDEOINNERAPI		pStuInnerAPI;
} NXVideoProcAPI, *LPNXVIDEOPROCAPI;


#endif		// #ifndef __NX_VIDEO_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of File.
