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
	TPMosaicRect,					// 矩形
	TPMosaicEllipse,				// 椭圆
};

struct TPMosaicParam
{
	int				iX;				// 操作起点，相对于图像左上角
	int				iY;	
	int				iWidth;			// 操作范围
	int				iHeight;
	float			fBoxX;			// 马赛克方格相对操作范围的比例，[0，1]
	float			fBoxY;
	float			fOffsetX;		// 马赛克方格的取点比例，[0，1]
	float			fOffsetY;
	TPMosaicType	eType;			// 马赛克类型
};

enum TPShapeType
{
	TPShapeRect,					// 矩形
	TPShapeEllipse,					// 椭圆
};

enum TPDimensionType
{
	TPDimBoth,
	TPDimHorizontal,				// 水平
	TPDimVertical,					// 垂直
};

struct TPGaussianBlurParam
{
	int				iX;				// 操作起点，相对于图像左上角
	int				iY;
	int				iWidth;			// 操作范围
	int				iHeight;
	float			fBluriness;		// 模糊程度
	TPShapeType		eShapeType;		// 模糊类型
	TPDimensionType eDimType;		// 模糊方向
};

//
// Y = Y * (235 - 16) / 255 + 16
// UV = (UV - 128) * (235 - 16) / 255 + 128
//
void TPYUYVConvertSafeColor(IN OUT void* pYUYV,
							IN int iPitch,				// 一行的字节数
							IN int iHeight);


// A不处理
//
// RGB = RGB * (235 - 16) / 255 + 16
//
void TPBGRAConvertSafeColor(IN OUT void* pBGRA,
							IN int iPitch,				// 一行的字节数
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
						  IN bool bReverse);			// 翻转标志

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
						 IN bool bReverse);				// 翻转标志

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

// 只得到奇场或偶场
void TPSplitFrame2Field(OUT BYTE* pField,
						IN int iFieldPitch,
						IN BYTE* pFrame,
						IN int iFramePitch,
						IN int iHeight,
						IN bool bOddField);				// 奇偶场标志

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

// 只对图像内区域有效，如果需要处理的区域超出了原图，则超出部分不予处理
void TPMosaic(IN OUT BYTE* pDes,
			  IN int iPitch,							// 一行的字节数
			  IN int iWidth,							// 图像宽高
			  IN int iHeight,
			  IN int iBitCount,							// 16为YUYV，32为BGRA
			  IN TPMosaicParam const& paramMosaic);		// 马赛克结构体参数

void TPInitGaussianBlurCoef(IN OUT float* fCoef,
							IN int iCount,
							IN float fTotal);

void TPBGRAGaussianBlurRow(OUT BYTE* pDst,				// 目的
						   IN BYTE* pSrc,				// 源
						   IN int iWidth,				// 源的宽度
						   IN float* fCoef,				// 权重参数
						   IN int iCoef,				// 权重参数个数
						   IN int iBegin,				// 当前行的模糊起点
						   IN int iEnd);				// 当前行的模糊终点

void TPBGRAGaussianBlurCol(OUT BYTE* pDst,				// 目的
						   IN int iDstPitch,			// 目的一行的字节数
						   IN BYTE* pSrc,				// 源
						   IN int iSrcPitch,			// 源一行的字节数
						   IN int iHeight,				// 源的高度
						   IN float* fCoef,				// 权重参数
						   IN int iCoef,				// 权重参数的个数
						   IN int iBegin,				// 当前列的模糊起点
						   IN int iEnd);				// 当前列的模糊终点

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

// 只对图像内区域有效，如果需要处理的区域超出了原图，则超出部分不予处理
void TPGaussianBlur(IN OUT BYTE* pucDes,
					IN int iPitch,								// 一行的字节数
					IN int iWidth,								// 图像宽高
					IN int iHeight,
					IN int iBitCount,							// 16为YUYV，32为BGRA
					IN TPGaussianBlurParam const& paramBlur);	// 高斯模糊参数

// 转为非安全色
//
// RGB = max(0, min(255, (RGB - 16) * 255 / (235 - 16)))
//
void TPBGRACvtNotSafeColor(IN OUT void* pBGRA,
						   IN size_t iByteCount);				// 字节数

//
// Y = max(0, min(255, (Y - 16) * 255 / (235 - 16)))
// UV = max(0, min(255, (UV - 128) * 255 / (235 - 16) + 128))
//
void TPYUYVCvtNotSafeColor(IN OUT void* pYUYV,
						   IN size_t iByteCount);				// 字节数
void TPYUYVCvtNotSafeColorCPU(IN OUT BYTE* pYUYV,int nWidth,int nHeidht,BYTE* pSrc);				// 字节数

// BGRA -> RGB
void cr32_blend_a_2_cr24_swaprb(OUT void* des,				// 目的
								IN void* src,				// 源
								IN size_t count);			// 像素个数

// BGRA -> BGR
void cr32_blend_a_2_cr24(OUT void* des,						// 目的
						 IN void* src,						// 源
						 IN size_t count);					// 像素个数

// 源与目的可以相同
void TPCvt32To24BlendAlpha(OUT void* des,					// 目的
						   IN void* src,					// 源
						   IN size_t count,					// 像素个数
						   IN bool swap_rb);				// 交换RB通道的标志

void cr32_blend_alpha_rb(OUT void* des,
						 IN void* src,
						 IN size_t count);

void cr32_blend_alpha(OUT void* des,
					  IN void* src,
					  IN size_t count);

// 32位颜色混合Alpha，A不改变，RGB改变
// 源与目的可以相同
void TPColor32BlendAlpha(OUT void* des,						// 目的
						 IN void* src,						// 源
						 IN size_t count,					// 像素个数
						 IN bool swap_rb);					// 交换RB通道的标志

//////////////////////////////////////////////////////////////////////////
// 32 位双线性插值缩放，包括直接拷贝，只水平缩放，只垂直缩放，水平且垂直缩放
// 对于 16 位如 YUYV 可以模拟成 32 位处理
// 内部默认参数正确
//////////////////////////////////////////////////////////////////////////
void tp_memcpy(void* des,		// 32 位输出
			   int des_pitch,	// 目的一行的字节数
			   int des_x,		// 目的写入内存起点坐标
			   int des_y,
			   int des_width,	// 目的写入内存宽高
			   int des_height,
			   void const* src,	// 32 位输入
			   int src_pitch,	// 源一行的字节数
			   int src_x,		// 源读取内存的起点坐标
			   int src_y,
			   int src_width,	// 源读取内存的宽高
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
