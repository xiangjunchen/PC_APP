//////////////////////////////////////////////////////////////////////////
// 描述：内存操作接口 SSE 优化
// 创建者：赵志猛
//////////////////////////////////////////////////////////////////////////
#ifndef __TP_BUFFER_OPERATE_SSE_INC__
#define __TP_BUFFER_OPERATE_SSE_INC__

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer Copy 接口
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_Copy_SSE(OUT DWORD* pDest,			// 目标 Buffer
					 IN int nDestX,				// 目标起始点
					 IN int nDestY,
					 IN int nDestWidth,			// 目标 Mix 大小
					 IN int nDestHeight,
					 IN int nDestBufferWidth,	// 目标 Buffer 大小
					 IN int nDestBufferHeight,
					 IN DWORD* pSrc,			// 源 Buffer
					 IN int nSrcX,				// 源起始点
					 IN int nSrcY,
					 IN int nSrcWidth,			// 源 Mix 大小
					 IN int nSrcHeight,
					 IN int nSrcBufferWidth,	// 源 Buffer 大小
					 IN int nSrcBufferHeight,
					 IN int nParity);			// 奇偶场类型

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer Mix 接口
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_Mix_SSE(IN OUT DWORD* pDest,		// 目标 Buffer
					IN int nDestX,				// 目标起始点
					IN int nDestY,
					IN int nDestWidth,			// 目标 Mix 大小
					IN int nDestHeight,
					IN int nDestBufferWidth,	// 目标 Buffer 大小
					IN int nDestBufferHeight,
					IN DWORD* pSrc,				// 源 Buffer
					IN int nSrcX,				// 源起始点
					IN int nSrcY,
					IN int nSrcWidth,			// 源 Mix 大小
					IN int nSrcHeight,
					IN int nSrcBufferWidth,		// 源 Buffer 大小
					IN int nSrcBufferHeight,
					IN int nParity);			// 奇偶场类型

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer NotMix 接口
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_NotMix_SSE(IN OUT DWORD* pDest,			// 目标 Buffer
					   IN int nDestX,				// 目标起始点
					   IN int nDestY,
					   IN int nDestWidth,			// 目标 Mix 大小
					   IN int nDestHeight,
					   IN int nDestBufferWidth,		// 目标 Buffer 大小
					   IN int nDestBufferHeight,
					   IN DWORD* pSrc,				// 源 Buffer
					   IN int nSrcX,				// 源起始点
					   IN int nSrcY,
					   IN int nSrcWidth,			// 源 Mix 大小
					   IN int nSrcHeight,
					   IN int nSrcBufferWidth,		// 源 Buffer 大小
					   IN int nSrcBufferHeight,
					   IN int nParity);				// 奇偶场类型

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 双线性插值缩放拷贝
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchCopy_SSE(OUT DWORD* pDest,			// 目标 Buffer
							  IN int nDestX,			// 目标起点
							  IN int nDestY,
							  IN int nDestWidth,		// 目标缩放区域
							  IN int nDestHeight,
							  IN int nDestBufferWidth,	// 目标 Buffer 大小
							  IN int nDestBufferHeight,
							  IN DWORD* pSrc,			// 源 Buffer
							  IN int nSrcX,				// 源起点
							  IN int nSrcY,
							  IN int nSrcWidth,			// 源缩放区域
							  IN int nSrcHeight,
							  IN int nSrcBufferWidth,	// 源 Buffer 大小
							  IN int nSrcBufferHeight,
							  IN int nParity);			// 奇偶场类型

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 加权平均插值缩放拷贝
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchCopy_SSE(OUT DWORD* pDest,			// 目标 Buffer
								IN int nDestX,				// 目标起点
								IN int nDestY,
								IN int nDestWidth,			// 目标缩放区域
								IN int nDestHeight,
								IN int nDestBufferWidth,	// 目标 Buffer 大小
								IN int nDestBufferHeight,
								IN DWORD* pSrc,				// 源 Buffer
								IN int nSrcX,				// 源起点
								IN int nSrcY,
								IN int nSrcWidth,			// 源缩放区域
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		// 源 Buffer 大小
								IN int nSrcBufferHeight,
								IN int nParity);			// 奇偶场类型

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 双线性插值缩放 Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchMix_SSE(OUT DWORD* pDest,			// 目标 Buffer
							 IN int nDestX,				// 目标起点
							 IN int nDestY,
							 IN int nDestWidth,			// 目标缩放区域
							 IN int nDestHeight,
							 IN int nDestBufferWidth,	// 目标 Buffer 大小
							 IN int nDestBufferHeight,
							 IN DWORD* pSrc,			// 源 Buffer
							 IN int nSrcX,				// 源起点
							 IN int nSrcY,
							 IN int nSrcWidth,			// 源缩放区域
							 IN int nSrcHeight,
							 IN int nSrcBufferWidth,	// 源 Buffer 大小
							 IN int nSrcBufferHeight,
							 IN int nParity);			// 奇偶场类型

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 双线性插值缩放 NotMix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_BLStretchNotMix_SSE(OUT DWORD* pDest,			// 目标 Buffer
								IN int nDestX,				// 目标起点
								IN int nDestY,
								IN int nDestWidth,			// 目标缩放区域
								IN int nDestHeight,
								IN int nDestBufferWidth,	// 目标 Buffer 大小
								IN int nDestBufferHeight,
								IN DWORD* pSrc,				// 源 Buffer
								IN int nSrcX,				// 源起点
								IN int nSrcY,
								IN int nSrcWidth,			// 源缩放区域
								IN int nSrcHeight,
								IN int nSrcBufferWidth,		// 源 Buffer 大小
								IN int nSrcBufferHeight,
								IN int nParity);			// 奇偶场类型

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 加权平均插值缩放 Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchMix_SSE(OUT DWORD* pDest,			// 目标 Buffer
							   IN int nDestX,				// 目标起点
							   IN int nDestY,
							   IN int nDestWidth,			// 目标缩放区域
							   IN int nDestHeight,
							   IN int nDestBufferWidth,		// 目标 Buffer 大小
							   IN int nDestBufferHeight,
							   IN DWORD* pSrc,				// 源 Buffer
							   IN int nSrcX,				// 源起点
							   IN int nSrcY,
							   IN int nSrcWidth,			// 源缩放区域
							   IN int nSrcHeight,
							   IN int nSrcBufferWidth,		// 源 Buffer 大小
							   IN int nSrcBufferHeight,
							   IN int nParity);				// 奇偶场类型

//////////////////////////////////////////////////////////////////////////
// BGRA 32 位色 Buffer 加权平均插值缩放 Mix
//////////////////////////////////////////////////////////////////////////
BOOL TPBGRA_MeanStretchNotMix_SSE(OUT DWORD* pDest,				// 目标 Buffer
								  IN int nDestX,				// 目标起点
								  IN int nDestY,
								  IN int nDestWidth,			// 目标缩放区域
								  IN int nDestHeight,
								  IN int nDestBufferWidth,		// 目标 Buffer 大小
								  IN int nDestBufferHeight,
								  IN DWORD* pSrc,				// 源 Buffer
								  IN int nSrcX,					// 源起点
								  IN int nSrcY,
								  IN int nSrcWidth,				// 源缩放区域
								  IN int nSrcHeight,
								  IN int nSrcBufferWidth,		// 源 Buffer 大小
								  IN int nSrcBufferHeight,
								  IN int nParity);				// 奇偶场类型

#endif
//////////////////////////////////////////////////////////////////////////
// 文件结束