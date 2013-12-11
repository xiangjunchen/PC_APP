//////////////////////////////////////////////////////////////////////////
// TPBufferOperateSSE2.h
//
// Lai Shoubo @ 2009-09-02
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <windef.h>
#include <process.h>
#include <algorithm>

#ifdef _buffer_operate_export
#define _export _declspec(dllexport)
#else
#define _export _declspec(dllimport)
#endif

//#define _USE_MULTI_CORE_

enum TPMosaicType
{
	TPMosaicRect,					// ����
	TPMosaicEllipse,				// ��Բ
};

struct TPMosaicParam
{
	int				iX;				// ������㣬�����ͼ�����Ͻ�
	int				iY;	
	int				iWidth;			// ������Χ
	int				iHeight;
	float			fBoxX;			// �����˷�����Բ�����Χ�ı�����[0��1]
	float			fBoxY;
	float			fOffsetX;		// �����˷����ȡ�������[0��1]
	float			fOffsetY;
	TPMosaicType	eType;			// ����������
};

enum TPShapeType
{
	TPShapeRect,					// ����
	TPShapeEllipse,					// ��Բ
};

enum TPDimensionType
{
	TPDimBoth,
	TPDimHorizontal,				// ˮƽ
	TPDimVertical,					// ��ֱ
};

struct TPGaussianBlurParam
{
	int				iX;				// ������㣬�����ͼ�����Ͻ�
	int				iY;
	int				iWidth;			// ������Χ
	int				iHeight;
	float			fBluriness;		// ģ���̶�
	TPShapeType		eShapeType;		// ģ������
	TPDimensionType eDimType;		// ģ������
};

//
// Y = Y * (235 - 16) / 255 + 16
// UV = (UV - 128) * (235 - 16) / 255 + 128
//
void TPYUYVConvertSafeColor(IN OUT void* pYUYV,
							IN int iPitch,				// һ�е��ֽ���
							IN int iHeight);


// A������
//
// RGB = RGB * (235 - 16) / 255 + 16
//
void TPBGRAConvertSafeColor(IN OUT void* pBGRA,
							IN int iPitch,				// һ�е��ֽ���
							IN int iHeight);

// ARGB <-> BGRA
void TPByteSwapARGBOrBGRA(OUT void* des,
						  IN void* src,
						  IN int count);

// ARGB -> YUYVA
void TPCvtARGB2YUYVALine(OUT BYTE* pYUYV,
						 OUT BYTE* pAlpha,
						 IN BYTE* pARGB,
						 IN int iPitch,
						 IN BYTE* pRGB2YUVCoef);

void TPCvtARGB2YUYVAFrame(OUT BYTE* pYUYV,
						  OUT BYTE* pAlpha,
						  IN BYTE* pARGB,
						  IN int iPitch,
						  IN int iWidth,
						  IN int iHeight,
						  IN bool bHD,
						  IN bool bReverse);			// ��ת��־

void TPMemCpyLine(OUT void* des,
				  IN void const* src,
				  IN int iPitch);

// ARGB -> YUYV
void TPCvtARGB2YUYVLine(OUT BYTE* pYUYV,
						IN BYTE* pARGB,
						IN int iPitch,
						IN BYTE* pRGB2YUVCoef);

void TPCvtARGB2YUYVFrame(OUT BYTE* pYUYV,
						 IN BYTE* pARGB,
						 IN int iPitch,
						 IN int iWidth,
						 IN int iHeight,
						 IN bool bHD,
						 IN bool bReverse);				// ��ת��־

// YUYV -> ARGB
void TPCvtYUYV2ARGBLine(OUT BYTE* pARGB,
						IN BYTE* pYUYV,
						IN int iPitch,
						IN BYTE* pYUV2RGBCoef);

void TPCvtYUYV2ARGBFrame(OUT BYTE* pARGB,
						 IN BYTE* pYUYV,
						 IN int iPitch,
						 IN int iWidth,
						 IN int iHeight,
						 IN bool bHD,
						 IN bool bReverse);

// YUYVA -> ARGB
void TPCvtYUYVA2ARGBLine(OUT BYTE* pARGB,
						 IN BYTE* pYUYV,
						 IN BYTE* pAlpha,
						 IN int iPitch,
						 IN BYTE* pYUV2RGBCoef);

void TPCvtYUYVA2ARGBFrame(OUT BYTE* pARGB,
						  IN BYTE* pYUYV,
						  IN BYTE* pAlpha,
						  IN int iPitch,
						  IN int iWidth,
						  IN int iHeight,
						  IN bool bHD,
						  IN bool bReverse);

// ֻ�õ��泡��ż��
void TPSplitFrame2Field(OUT BYTE* pField,
						IN int iFieldPitch,
						IN BYTE* pFrame,
						IN int iFramePitch,
						IN int iHeight,
						IN bool bOddField);				// ��ż����־

void TPMemSetByteLine(IN OUT void* pucDes,
					  IN int iPitch,
					  IN BYTE byVal);

void TPMemSetDwordLine(IN OUT void* des,
					   IN int iPitch,
					   IN DWORD val);

void TPMemSetDwordRect(IN OUT void* des,
					   IN int iPitch,
					   IN DWORD val,
					   IN int iWidth,
					   IN int iHeight);

// ֻ��ͼ����������Ч�������Ҫ��������򳬳���ԭͼ���򳬳����ֲ��账��
void TPMosaic(IN OUT BYTE* pDes,
			  IN int iPitch,							// һ�е��ֽ���
			  IN int iWidth,							// ͼ����
			  IN int iHeight,
			  IN int iBitCount,							// 16ΪYUYV��32ΪBGRA
			  IN TPMosaicParam const& paramMosaic);		// �����˽ṹ�����

void TPInitGaussianBlurCoef(IN OUT float* fCoef,
							IN int iCount,
							IN float fTotal);

void TPBGRAGaussianBlurRow(OUT BYTE* pDst,				// Ŀ��
						   IN BYTE* pSrc,				// Դ
						   IN int iWidth,				// Դ�Ŀ��
						   IN float* fCoef,				// Ȩ�ز���
						   IN int iCoef,				// Ȩ�ز�������
						   IN int iBegin,				// ��ǰ�е�ģ�����
						   IN int iEnd);				// ��ǰ�е�ģ���յ�

void TPBGRAGaussianBlurCol(OUT BYTE* pDst,				// Ŀ��
						   IN int iDstPitch,			// Ŀ��һ�е��ֽ���
						   IN BYTE* pSrc,				// Դ
						   IN int iSrcPitch,			// Դһ�е��ֽ���
						   IN int iHeight,				// Դ�ĸ߶�
						   IN float* fCoef,				// Ȩ�ز���
						   IN int iCoef,				// Ȩ�ز����ĸ���
						   IN int iBegin,				// ��ǰ�е�ģ�����
						   IN int iEnd);				// ��ǰ�е�ģ���յ�

void TPCvtYUYV2YUVAFrame(OUT void* pDes,
						 IN int iDstPitch,
						 IN void* pSrc,
						 IN int iSrcPitch,
						 IN int iWidth,
						 IN int iHeight);

void TPCvtYUVA2YUYVFrame(OUT void* pDst,
						 IN int iDstPitch,
						 IN void* pSrc,
						 IN int iSrcPitch,
						 IN int iWidth,
						 IN int iHeight);

// ֻ��ͼ����������Ч�������Ҫ��������򳬳���ԭͼ���򳬳����ֲ��账��
void TPGaussianBlur(IN OUT BYTE* pucDes,
					IN int iPitch,								// һ�е��ֽ���
					IN int iWidth,								// ͼ����
					IN int iHeight,
					IN int iBitCount,							// 16ΪYUYV��32ΪBGRA
					IN TPGaussianBlurParam const& paramBlur);	// ��˹ģ������

// תΪ�ǰ�ȫɫ
//
// RGB = max(0, min(255, (RGB - 16) * 255 / (235 - 16)))
//
void TPBGRACvtNotSafeColor(IN OUT void* pBGRA,
						   IN size_t iByteCount);				// �ֽ���

//
// Y = max(0, min(255, (Y - 16) * 255 / (235 - 16)))
// UV = max(0, min(255, (UV - 128) * 255 / (235 - 16) + 128))
//
void TPYUYVCvtNotSafeColor(IN OUT void* pYUYV,
						   IN size_t iByteCount);				// �ֽ���
void TPYUYVCvtNotSafeColorCPU(IN OUT BYTE* pYUYV,int nWidth,int nHeidht,BYTE* pSrc);				// �ֽ���

// BGRA -> RGB
void cr32_blend_a_2_cr24_swaprb(OUT void* des,				// Ŀ��
								IN void* src,				// Դ
								IN size_t count);			// ���ظ���

// BGRA -> BGR
void cr32_blend_a_2_cr24(OUT void* des,						// Ŀ��
						 IN void* src,						// Դ
						 IN size_t count);					// ���ظ���

// Դ��Ŀ�Ŀ�����ͬ
void TPCvt32To24BlendAlpha(OUT void* des,					// Ŀ��
						   IN void* src,					// Դ
						   IN size_t count,					// ���ظ���
						   IN bool swap_rb);				// ����RBͨ���ı�־

void cr32_blend_alpha_rb(OUT void* des,
						 IN void* src,
						 IN size_t count);

void cr32_blend_alpha(OUT void* des,
					  IN void* src,
					  IN size_t count);

// 32λ��ɫ���Alpha��A���ı䣬RGB�ı�
// Դ��Ŀ�Ŀ�����ͬ
void TPColor32BlendAlpha(OUT void* des,						// Ŀ��
						 IN void* src,						// Դ
						 IN size_t count,					// ���ظ���
						 IN bool swap_rb);					// ����RBͨ���ı�־

//////////////////////////////////////////////////////////////////////////
// 32 λ˫���Բ�ֵ���ţ�����ֱ�ӿ�����ֻˮƽ���ţ�ֻ��ֱ���ţ�ˮƽ�Ҵ�ֱ����
// ���� 16 λ�� YUYV ����ģ��� 32 λ����
// �ڲ�Ĭ�ϲ�����ȷ
//////////////////////////////////////////////////////////////////////////
void tp_memcpy(void* des,		// 32 λ���
			   int des_pitch,	// Ŀ��һ�е��ֽ���
			   int des_x,		// Ŀ��д���ڴ��������
			   int des_y,
			   int des_width,	// Ŀ��д���ڴ���
			   int des_height,
			   void const* src,	// 32 λ����
			   int src_pitch,	// Դһ�е��ֽ���
			   int src_x,		// Դ��ȡ�ڴ���������
			   int src_y,
			   int src_width,	// Դ��ȡ�ڴ�Ŀ��
			   int src_height);

_export void yuva_2_yuyv_and_a_line(void* yuyv, void* alpha, void const* yuva, size_t width);
_export void yuva_2_yuyv_line(void* yuyv, void const* yuva, size_t width);

_export void memcpy_line(void* des, void const* src, size_t count);
_export void memset_line(void* des, int c, size_t count);
_export void dmemset_line(void* des, int c, size_t count);

namespace bo
{
	class thread_task
	{
	public:
		virtual void do_task(size_t index) = 0;
	};

	class _export thread_manager
	{
	private:
		thread_manager(thread_manager const&);
		thread_manager& operator=(thread_manager const&);
	public:
		thread_manager();
		~thread_manager();
		void set_info(thread_task* task_spec, size_t count);
		void lock();
		void unlock();
	private:
		unsigned worker_proc();
		static unsigned _stdcall worker_thread(void* data);
	private:
		size_t				index;
		HANDLE*				thread;
		HANDLE				semevt[2];
		HANDLE				event;
		thread_task*		task;
		CRITICAL_SECTION	cs;
		HANDLE				mutex;
		HANDLE				semaphore;
	};
}

namespace bo
{
	class bl_hor_out : public bo::thread_task
	{
	public:
		bl_hor_out(unsigned char* des, size_t dpitch, unsigned char* src, size_t spitch, int* power, unsigned short* map, size_t count);
		virtual void do_task(size_t index);
	private:
		unsigned char* des_in;
		size_t des_pitch;
		unsigned char* src_in;
		size_t src_pitch;
		int* power_in;
		unsigned short* map_in;
		size_t count_in;
	};

	class bl_hor_ver_out : public bo::thread_task
	{
	public:
		bl_hor_ver_out(unsigned char* des, size_t dp, unsigned char* src, size_t sp, int* px, int* py, unsigned short* mx, unsigned short* my, size_t count);
		virtual void do_task(size_t index);
	private:
		unsigned char* des_in;
		size_t des_pitch;
		unsigned char* src_in;
		size_t src_pitch;
		int* power_x;
		int* power_y;
		unsigned short* map_x;
		unsigned short* map_y;
		size_t count_in;
	};

	class bl_ver_out : public bo::thread_task
	{
	public:
		bl_ver_out(unsigned char* des, size_t dp, unsigned char* src, size_t sp, int* py, unsigned short* my, size_t count);
		virtual void do_task(size_t index);
	private:
		unsigned char* des_in;
		size_t des_pitch;
		unsigned char* src_in;
		size_t src_pitch;
		int* power_y;
		unsigned short* map_y;
		size_t width;
	};
}

_export void yuv422_to_yuyv_line(void* yuyv, void* y, void* u, void* v, size_t width);
_export void yuyv_to_yuv422_line(void* y, void* u, void* v, void* yuyv, size_t width);
