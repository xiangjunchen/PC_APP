//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: ��Ƶ��Ч�㷨�ӿڶ��壬�絭�뵭���������������ס����Ż����.
//
// Created: ZhaoZhimeng @ 2009-06-09
//////////////////////////////////////////////////////////////////////////
#ifndef __NX_VIDEO_EFFECT_PROC_DEF_H__
#define __NX_VIDEO_EFFECT_PROC_DEF_H__

#include ".\NXBasic.h"
#include ".\NXVideoProcDef.h"

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


//////////////////////////////////////////////////////////////////////////
// �ⶨ��

#define NXVIDEOEFFECTPROC_DLL_NAME			_T("NXVideoEffectProc")
#define NXVIDEOEFFECTPROC_API_EXT_NAME		"NVEP_GetAPIStuPtr"


//////////////////////////////////////////////////////////////////////////
// �����ؼ���������

// ��1�����Ͷ�������ڲ����е����ͼ�񣬱�ʾ���ȶ���ʽ�����ͼ�񻮳����ұ�ͼ���롣
// ��2�����ϣ������λ������ߵ�ͼ�񰴼ȶ���ʽ���룬���Ե����ؼ��ӿ�ʱ��������Ϊ�������ұ�ͼ���롣
enum NXVE_WIPE_TYPE
{
	NXVE_WIPE_L_2_R_SLIDE,			// ��ߵ��ұߵĴ�ֱ�߻���
	NXVE_WIPE_R_2_L_SLIDE,			// �ұߵ���ߵĴ�ֱ�߻���
	NXVE_WIPE_T_2_B_SLIDE,			// �ϱߵ��±ߵ�ˮƽ�߻���
	NXVE_WIPE_B_2_T_SLIDE,			// �±ߵ��ϱߵ�ˮƽ�߻���

	NXVE_WIPE_C_2_LR_SLIDE,			// ���ĵ��������ߵĴ�ֱ�߻���
	NXVE_WIPE_LR_2_C_SLIDE,			// �������ߵ����ĵĴ�ֱ�߻���
	NXVE_WIPE_C_2_TB_SLIDE,			// ���ĵ��������ߵ�ˮƽ�߻���
	NXVE_WIPE_TB_2_C_SLIDE,			// �������ߵ����ĵ�ˮƽ�߻���

	NXVE_WIPE_C_2_LT_RB_SLIDE,		// ���ĵ����Ͻ������½ǵĶԽ��߻���
	NXVE_WIPE_LT_RB_2_C_SLIDE,		// ���Ͻ������½ǵ����ĵĶԽ��߻���
	NXVE_WIPE_C_2_RT_LB_SLIDE,		// ���ĵ����Ͻ������½ǵĶԽ��߻���
	NXVE_WIPE_RT_LB_2_C_SLIDE,		// ���Ͻ������½ǵ����ĵĶԽ��߻���

	NXVE_WIPE_RT_2_LB_SLIDE,		// ���ϵ����µĶԽ��߻���
	NXVE_WIPE_LB_2_RT_SLIDE,		// ���µ����ϵĶԽ��߻���
	NXVE_WIPE_LT_2_RB_SLIDE,		// ���ϵ����µĶԽ��߻���
	NXVE_WIPE_RB_2_LT_SLIDE,		// ���µ����ϵĶԽ��߻���

	NXVE_WIPE_RT_2_LB_CORNER,		// ���ϵ����µ�ֱ�ǻ���
	NXVE_WIPE_LB_2_RT_CORNER,		// ���µ����ϵ�ֱ�ǻ���
	NXVE_WIPE_RB_2_LT_CORNER,		// ���µ����ϵ�ֱ�ǻ���
	NXVE_WIPE_LT_2_RB_CORNER,		// ���ϵ����µ�ֱ�ǻ���

	NXVE_WIPE_RT_2_LB_BOX,			// ���ϵ����µķ��񻮳�
	NXVE_WIPE_LB_2_RT_BOX,			// ���µ����ϵķ��񻮳�
	NXVE_WIPE_RB_2_LT_BOX,			// ���µ����ϵķ��񻮳�
	NXVE_WIPE_LT_2_RB_BOX,			// ���ϵ����µķ��񻮳�

	NXVE_WIPE_LTRB_2_C_CORNER,		// �ıߵ����ĵ���ֱ�ǻ���
	NXVE_WIPE_C_2_LTRB_CORNER,		// ���ĵ��ıߵ���ֱ�ǻ���
	NXVE_WIPE_LTRB_2_C_BOX,			// �ıߵ����ĵ��ķ��񻮳�
	NXVE_WIPE_C_2_LTRB_BOX,			// ���ĵ��ıߵķ��񻮳�

	NXVE_WIPE_LTRB_2_C_CIRCLE,		// �ĸ��ǵ����ĵ�Բ�λ���
	NXVE_WIPE_C_2_LTRB_CIRCLE,		// ���ĵ��ĸ��ǵ�Բ�λ���

	NXVE_WIPE_TYPE_COUNT
};

typedef struct tag_nxvewipeparam
{
	NXVE_WIPE_TYPE		eType;			// ��������
	double				dPercent;		// ������ȣ�ȡֵ [0, 1]
	// ��ͬ���͵�˽�в���
	union{
	};
} NXVEWipeParam, *LPNXVEWIPEPARAM;

//////////////////////////////////////////////////////////////////////////
// �ؼ������������״����
enum NXVE_SHAPE_TYPE
{
	NXVE_SHAPE_RECT,				// ����
	NXVE_SHAPE_ELLIPSE				// ���֣�Բ
};

//////////////////////////////////////////////////////////////////////////
// �������ؼ���������

enum NXVE_TEX_PATCH_ARITH
{
	NXVE_TEX_PATCH_ARITH_GSSORR,	// Gauss-seidel + SOR + Roberts

	NXVE_TEX_PATCH_ARITH_COUNT
};

typedef struct tagnxvetexpatchparam
{
	unsigned char			*pucSrc;		// Դ��Ч��㣨Ϊ NULL ʱ��ʾ������������Ϊ NULL ʱ��ʾ�����¡��
	intptr_t				nSrcPitch;		// Դһ����ռ�õ��ֽ���
	unsigned char			*pucMask;		// ���� 8 λ������Ч��㣨Ϊ NULL ʱ��ʾ�����������򣬲�Ϊ NULL ʱֻ���������ֵ������
	intptr_t				nMaskStrideX;	// һ�����ص�������ռ�õ��ֽ���
	intptr_t				nMaskPitch;		// ����һ����ռ�õ��ֽ���
	unsigned int			unMaskThr;		// ������ֵ [0, 255]�����ڴ���ֵ��Ч��
	float					*pfCache0;		// ������ٻ��壬����Ϊ unProcWidth * unProcHeight * nPixelBytes��Ϊ NULL �����ڲ���������
	float					*pfCache1;
	int						nProcWidth;		// �����С
	int						nProcHeight;
	NXVE_TEX_PATCH_ARITH	nAirth;			// ��ⷽ��
} NXVETexPatchParam, *LPNXVETEXPATCHPARAM;

//////////////////////////////////////////////////////////////////////////
// �������ؼ��㷨

// ���뵭��
// ��1���������ͼ�񡢵����ұ�ͼ��
// ��2�������ͼ��Ϊ��ʱ����ڳ�������Ϊ�ұ�ͼ���롣
// ��3�����ұ�ͼ��Ϊ��ʱ����ڳ�������Ϊ���ͼ�񵭳���
// ��4��ePixelFmt ���Ϊ NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 ��ʽ��
// Ҫ�����������ϸ��� Y һƬ��U һƬ��V һƬ˳��洢����ͬ����������� RAW_DATA��
// ��ʱ nPitch ��ʾ Y һ��ռ�õ��ֽڿ�ȣ��� U/V ����ռ���ֽڿ��Ϊ Y ��һ�롣
typedef bool (*DefVEFade)(OUT unsigned char* pucDes,		// Ŀ�����
						  IN intptr_t nDesPitch,			// Ŀ��һ����ռ�õ��ֽ���
						  IN unsigned char* pucSrcLeft,		// ���Դ
						  IN intptr_t nSrcLeftPitch,		// ���Դһ����ռ�õ��ֽ���
						  IN unsigned char* pucSrcRight,	// �ұ�Դ
						  IN intptr_t nSrcRightPitch,		// �ұ�Դһ����ռ�õ��ֽ���
						  IN NXVIDEO_PIXEL_FMT ePixelFmt,	// ���ظ�ʽ
						  IN double dLeftWeight,			// ���Դ��Ȩ��
						  IN int nWidth,					// ͼ�����ط���
						  IN int nHeight,
						  IN bool bOverTurn);				// �Ƿ�ת
// ��������ߣ�
// ��1���������ͼ�񡢻����ұ�ͼ�񣻵� dPercent = 0.0 ʱ�����ͼ����ȫ��ʾ���ұ�ͼ����ȫ�ڵ���
// �� dPercent = 1.0 ʱ�����ͼ����ȫ�ڵ����ұ�ͼ����ȫ��ʾ��
// ��2�������ͼ��Ϊ��ʱ����ڳ�������Ϊ�ұ�ͼ���롣
// ��3�����ұ�ͼ��Ϊ��ʱ����ڳ�������Ϊ���ͼ�񻮳���
// ��4��ePixelFmt ���Ϊ NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 ��ʽ��
// Ҫ�����������ϸ��� Y һƬ��U һƬ��V һƬ˳��洢����ͬ����������� RAW_DATA��
// ��ʱ nPitch ��ʾ Y һ��ռ�õ��ֽڿ�ȣ��� U/V ����ռ���ֽڿ��Ϊ Y ��һ�롣
typedef bool (*DefVEWipe)(OUT unsigned char* pucDes,		// Ŀ��
						  IN intptr_t nDesPitch,			// Ŀ��һ����ռ�õ��ֽ���
						  IN unsigned char* pucSrcLeft,		// ���Դ
						  IN intptr_t nSrcLeftPitch,		// ���Դһ����ռ�õ��ֽ���
						  IN unsigned char* pucSrcRight,	// �ұ�Դ
						  IN intptr_t nSrcRightPitch,		// �ұ�Դһ����ռ�õ��ֽ���
						  IN NXVIDEO_PIXEL_FMT ePixelFmt,	// ���ظ�ʽ
						  IN LPNXVEWIPEPARAM pParam,		// �������
						  IN int nWidth,					// ͼ�����ط���
						  IN int nHeight,
						  IN bool bOverTurn);				// �Ƿ�ת

//////////////////////////////////////////////////////////////////////////

typedef struct tag_nxvetransitionapi		// �������ؼ�
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	DefVEFade			pfnVEFade;			// ���뵭��
	DefVEWipe			pfnVEWipe;			// ����
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXVETransitionAPI, *LPNXVETRANSITIONAPI;

//////////////////////////////////////////////////////////////////////////
// ��ɫ�����ؼ��㷨

// ����/����
// ��1�����ֵ���ɫҪ���� BGRA(0xARGB) �ĸ�ʽ�������ڲ���Ҫ���� ePixelFmt + eDefinition �жϴ���ʽ��
// ��2���� ulCrBGRA = 0xff000000 ʱ�������ڣ��� ulCrBGRA = 0xffffffff ʱ�������ס�
// ��3��ePixelFmt ���Ϊ NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 ��ʽ��
// Ҫ�����������ϸ��� Y һƬ��U һƬ��V һƬ˳��洢����ͬ����������� RAW_DATA��
// ��ʱ nPitch ��ʾ Y һ��ռ�õ��ֽڿ�ȣ��� U/V ����ռ���ֽڿ��Ϊ Y ��һ�롣
typedef bool (*DefVEFlash)(OUT unsigned char* pucDes,			// Ŀ��
						   IN intptr_t nDesPitch,				// Ŀ��һ����ռ�õ��ֽ���
						   IN unsigned char* pucSrc,			// Դ
						   IN intptr_t nSrcPitch,				// Դһ����ռ�õ��ֽ���
						   IN NXVIDEO_PIXEL_FMT ePixelFmt,		// ���ظ�ʽ
						   IN NXVIDEO_DEFINITION eDefinition,	// �ź������ȣ�������ɫ�任��
						   IN bool bFlashIn,					// ����/������־
						   IN unsigned long ulCrBGRA,			// ���ֵ���ɫ��BGRA ��ʽ
						   IN double dPercent,					// ���ȣ�ȡֵ [0, 1]
						   IN int nWidth,						// ͼ�����ط���
						   IN int nHeight,
						   IN bool bOverTurn);					// �Ƿ�ת

//////////////////////////////////////////////////////////////////////////
// �������ؼ�
// ����Ч�ʣ�Դ��Ŀ����ͬ��ֻ��д��Ҫ���������
// ��������������λ��֣�Բ���Ƽ����Σ�Ч�ʸ���
// �ؼ����Ȳ���ָ���ؼ���ϸ���ȣ�����ԽС��Ч��Խ�ӽ�ԭ����
// ����Ƿ� 32 λ��ͼ�񣬽���任Ϊ 32 λͼ��Ŀ��� YUYV ��ԭ����� 2
//////////////////////////////////////////////////////////////////////////
typedef bool (*DefVEMosaic)(IN OUT unsigned char* pucDes,		// Դ��Ŀ����ͬ
							IN intptr_t nDesPitch,				// Ŀ��һ�е��ֽ���
							IN int nWidth,						// ��������
							IN int nHeight,
							IN int nX,							// ��������ƫ��
							IN int nY,
							IN int nValidWidth,					// ����������
							IN int nValidHeight,
							IN NXVE_SHAPE_TYPE eShape,			// ����������״����
							IN float fGranularity);				// �����˵����� [0, 1]
//////////////////////////////////////////////////////////////////////////
// ģ���ؼ�
// ����Ч�ʣ�Դ��Ŀ����ͬ��ֻ��д��Ҫ���������
// ��������������λ��֣�Բ���Ƽ����Σ�Ч�ʸ���
// ģ���뾶Խ��Ч��Խģ��
// �����������ϴ����ⲿ��Ƶ�����ã������ⲿ������ʱ����
// ��ʱ�������Сֵ���������������
// ��1����ˮƽ����ģ����proc_width * 8 * ͨ����
// ��1������ֱ����ģ����proc_width * proc_height * 8 * ͨ����
// ��1��ˮƽ�ʹ�ֱ����ģ����proc_width * proc_height * 8 * ͨ����
//////////////////////////////////////////////////////////////////////////
typedef NXRESULT (*pfnve_blur)(__inout unsigned char* des,				// Դ��Ŀ����ͬ
							   __in intptr_t des_pitch,					// һ�е��ֽ���
							   __in int proc_x,							// ģ���������
							   __in int proc_y,
							   __in int proc_width,						// ģ����Χ
							   __in int proc_height,
							   __in NXVIDEO_PIXEL_FMT pixel_fmt,		// ���ظ�ʽ
							   __in NXVE_SHAPE_TYPE shape,				// ����������״����
							   __in NXVIDEO_FILTER_TYPE filter,			// Ŀǰֻ֧�� NXVIDEO_FILTER_GAUSS
							   __in NXVE_ORIENTATION_TYPE orientation,	// ������
							   __in int radius,							// ģ���뾶 [0, 500]
							   __in unsigned char* cache_buf,			// �����ڲ���ʱ���壬Ϊ NULL ʱ�ڲ�����
							   __in intptr_t cache_pitch);				// ��ʱ����һ�е��ֽ���
//////////////////////////////////////////////////////////////////////////
// �������ؼ�
// ��1���������ֹ��ܣ����������������¡����μ� NXVETexPatchParam �е� pucSrc ���塣
// ��2��������������ϽǶ�������ĵ�ַ��㿪ʼ�������С��СΪ 4x3����Ҫ���ڴ������ȫ�Ϸ���
// ��3����֧�� NXVIDEO_PIXEL_FMT_YUV422 �� NXVIDEO_PIXEL_FMT_YUV420 ���ַ�Ƭ��ʽ��
// �����Ҫ���������ָ�ʽ��������߷ֱ� Y��U��V �� NXVIDEO_PIXEL_FMT_KEY �ĸ�ʽ����
// ��ʱ��Ҫע����������Ĺ�ϵ��UV ������������Ҫ�۰롣
typedef bool (*DefVETexPatch)(IN OUT unsigned char* pucDes,		// Ŀ����Ч���
							  IN intptr_t nDesPitch,			// Ŀ��һ����ռ�õ��ֽ���
							  IN NXVIDEO_PIXEL_FMT ePixelFmt,	// ���ظ�ʽ
							  IN LPNXVETEXPATCHPARAM pParam);	// ������
//////////////////////////////////////////////////////////////////////////
// ͼ��ȥ��
typedef NXRESULT (*pfnve_denoising)(__out unsigned char* des,
									__in intptr_t des_pitch,
									__in unsigned char* src,
									__in intptr_t src_pitch,
									__in NXVIDEO_PIXEL_FMT pixel_fmt,
									__in int width,
									__in int height,
									__in double space_sigma,			// [0, 1]
									__in double range_sigma);			// [0, 1]
//////////////////////////////////////////////////////////////////////////
typedef struct tag_nxvecolorapi		// ��ɫ�����ؼ�
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.0.0
	DefVEFlash			pfnVEFlash;			// ����/����
	DefVEMosaic			pfnVEMosaic;		// ������
	DefVETexPatch		pfnVETexPatch;		// ������
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.21
	pfnve_blur			blur;				// ģ���ؼ�
	// V1.1.0.5
	pfnve_denoising		denoising;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����￪ʼ
} NXVEColorAPI, *LPNXVECOLORAPI;

//////////////////////////////////////////////////////////////////////////
// �����ؼ�������

// Video Stabilization �㷨������
interface INXVSAProc;
typedef INXVSAProc* (*DefCreateVSAProc)();
typedef void (*DefReleaseVSAProc)(INXVSAProc* &pVSAProc);

typedef struct tag_nxveplantapi		// ����������
{
	//////////////////////////////////////////////////////////////////////////
	// V1.0.8.0
	// Video Stabilization �㷨������
	DefCreateVSAProc		pfnCreateVSAProc;
	DefReleaseVSAProc		pfnReleaseVSAProc;
	//////////////////////////////////////////////////////////////////////////
	// �°汾�ӿڴ����ʼ����
} NXVEPlantAPI, *LPNXVEPLANTAPI;

//////////////////////////////////////////////////////////////////////////
// ��ֵ�����ؼ��㷨


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// �ӿڽṹ����

typedef struct tag_nxveinnerapi *LPNXVEINNERAPI;

typedef struct tag_nxvideoeffectprocapi
{
	// �������ؼ��㷨�ӿ�
	LPNXVETRANSITIONAPI		pStuTransitionAPI;
	// ��ɫ�����ؼ��㷨�ӿ�
	LPNXVECOLORAPI			pStuColorAPI;
	// �����������ӿ�
	LPNXVEPLANTAPI			pStuPlantAPI;
	// Private Ȩ�ޣ������Ż����ڲ��Ľӿ�
	LPNXVEINNERAPI			pStuInnerAPI;
} NXVideoEffectProcAPI, *LPNXVIDEOEFFECTPROCAPI;


#endif		// #ifndef __NX_VIDEO_EFFECT_PROC_DEF_H__
//////////////////////////////////////////////////////////////////////////
// End of file.