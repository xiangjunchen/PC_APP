//////////////////////////////////////////////////////////////////////////
// 定义文件
#ifndef __TP_BUFFER_OPERATE_DEF_INC__
#define __TP_BUFFER_OPERATE_DEF_INC__

#include <math.h>

#define TP_BUFFER_MIX		0	// 混叠模式：Mix操作
#define TP_BUFFER_COPY		1	// 复制操作
#define TP_BUFFER_NOTMIX	2	// NotMix操作

#define TP_PARITY_BOTH		0	// 奇偶场
#define TP_PARITY_ODD		1	// 奇场
#define TP_PARITY_EVEN		2	// 偶场


//////////////////////////////////////////////////////////////////////////
// SSE 内存对齐声明、申请与释放宏定义
#define TPSSE_DECLARE(TYPE)		__declspec(align(16)) ##TYPE
#define TPSSE_NEW(nSize, TYPE)	(##TYPE*)_mm_malloc(sizeof(##TYPE) * (nSize), 16)
#define TPSSE_FREE(p)			_mm_free((p))
#define TPSSE_SAFEFREE(p)		if((p)) _mm_free((p))

//////////////////////////////////////////////////////////////////////////
// 常量定义

#define COEF_SCALE_16384		16384	// 不需要补偿

#endif

//////////////////////////////////////////////////////////////////////////
// 文件结束