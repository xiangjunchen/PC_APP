/********************************************************************
	Copyright (c) 2004, Sobey数码 内容制作部
	All rights reserved.
	创建时间:	2004年5月24日   16:01
	文件名: 	c:\ndsrc\chenyufeng_desktop_3\devvob\sobeydesktop\src\tpcommon\tpbufferoperate\tp_cputest.h
	当前版本：	1.0
	作者:		陈玉峰
	摘要:		CPU相关信息的获取
*********************************************************************/
#ifndef __TP_CPU_TEST_INC__
#define __TP_CPU_TEST_INC__

// CPU信息结构体
struct CPUInformation
{
	int	 m_Size;	// 结构大小：为了向后兼容
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