/********************************************************************
	Copyright (c) 2004, Sobey���� ����������
	All rights reserved.
	����ʱ��:	2004��5��24��   16:01
	�ļ���: 	c:\ndsrc\chenyufeng_desktop_3\devvob\sobeydesktop\src\tpcommon\tpbufferoperate\tp_cputest.h
	��ǰ�汾��	1.0
	����:		�����
	ժҪ:		CPU�����Ϣ�Ļ�ȡ
*********************************************************************/
#ifndef __TP_CPU_TEST_INC__
#define __TP_CPU_TEST_INC__

// CPU��Ϣ�ṹ��
struct CPUInformation
{
	int	 m_Size;	// �ṹ��С��Ϊ��������
	bool m_bRDTSC : 1,	// Is RDTSC supported?
	m_bCMOV  : 1,		// Is CMOV supported?
	m_bFCMOV : 1,		// Is FCMOV supported?
	m_bSSE	  : 1,		// Is SSE supported?
	m_bSSE2  : 1,		// Is SSE2 Supported?
	m_b3DNow : 1,		// Is 3DNow! Supported?
	m_bMMX   : 1,		// Is MMX supported?
	m_bHT	  : 1;		// Is HyperThreading supported?

	unsigned char m_nLogicalProcessors,	// Number op logical processors.
		m_nPhysicalProcessors;		// Number of physical processors

	long  m_Speed;				// In cycles per second.

	char* m_szProcessorID;			// Processor vendor Identification.
};

const CPUInformation& GetCPUInformation();

#endif	// __TP_CPU_TEST_INC__