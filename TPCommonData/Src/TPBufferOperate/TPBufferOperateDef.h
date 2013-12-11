//////////////////////////////////////////////////////////////////////////
// �����ļ�
#ifndef __TP_BUFFER_OPERATE_DEF_INC__
#define __TP_BUFFER_OPERATE_DEF_INC__

#include <math.h>

#define TP_BUFFER_MIX		0	// ���ģʽ��Mix����
#define TP_BUFFER_COPY		1	// ���Ʋ���
#define TP_BUFFER_NOTMIX	2	// NotMix����

#define TP_PARITY_BOTH		0	// ��ż��
#define TP_PARITY_ODD		1	// �泡
#define TP_PARITY_EVEN		2	// ż��


//////////////////////////////////////////////////////////////////////////
// SSE �ڴ�����������������ͷź궨��
#define TPSSE_DECLARE(TYPE)		__declspec(align(16)) ##TYPE
#define TPSSE_NEW(nSize, TYPE)	(##TYPE*)_mm_malloc(sizeof(##TYPE) * (nSize), 16)
#define TPSSE_FREE(p)			_mm_free((p))
#define TPSSE_SAFEFREE(p)		if((p)) _mm_free((p))

//////////////////////////////////////////////////////////////////////////
// ��������

#define COEF_SCALE_16384		16384	// ����Ҫ����

#endif

//////////////////////////////////////////////////////////////////////////
// �ļ�����