/********************************************************************
	Copyright (c) 2004, Sobey���� ����������
	All rights reserved.
	����ʱ��:	2004��8��3��   10:11
	�ļ���: 	g:\e7\chenyufeng_desktop_cg\desktopsobey\tpcommondata\src\tpbufferoperate\tpmaskoperateapi.h
	��ǰ�汾��	1.0
	����:		�����
	ժҪ:		�ڴ滺��������
*********************************************************************/
#ifndef __TP_MEMORY_OPERATE_INC__
#define __TP_MEMORY_OPERATE_INC__

// �����ڴ�Ϊĳһֵ
void* CALLBACK TP_MemSet(IN OUT void* pDest,
						 IN int nChar,
						 IN int nSize);
// �ڴ濽��
void* CALLBACK TP_Memcpy(IN OUT void* pDest,
			IN const void* pSrc,
			IN int nSize);

#endif	// __TP_MEMORY_OPERATE_INC__