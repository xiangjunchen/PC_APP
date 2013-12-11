/********************************************************************
	Copyright (c) 2004, Sobey数码 内容制作部
	All rights reserved.
	创建时间:	2004年5月24日   16:04
	文件名: 	c:\ndsrc\chenyufeng_desktop_3\devvob\sobeydesktop\src\tpcommon\tpbufferoperate\tp_cputest.cpp
	当前版本：	1.0
	作者:		陈玉峰
	摘要:		CPU相关信息的获取
*********************************************************************/
#define VC_EXTRALEAN		// 排除不太常用的API
#include "stdAfx.h"
#include "TP_CPUTest.h"

//////////////////////////////////////////////////////////////////////////
// 功能：取得CPU相关信息
// 参数：function：要取得的信息类别：0表示取出CPU厂商，1表示版本信息
//       out_eax：EAX中的数据
// 返回值：
//////////////////////////////////////////////////////////////////////////
static bool cpuid(unsigned long function, unsigned long& out_eax, unsigned long& out_ebx, unsigned long& out_ecx, unsigned long& out_edx)
{
	bool retval = true;
	unsigned long local_eax, local_ebx, local_ecx, local_edx;
	_asm pushad;

	__try
	{
		_asm
		{
			xor edx, edx		// EDX清0，将要使用
				mov eax, function	// set up CPUID to return processor version and features
				// 先设EAX值，0表示取出vendor string, 1表示version info, 2表示cache info
				cpuid			// code bytes = 0fh,  0a2h
				mov local_eax, eax	// features returned in eax
				mov local_ebx, ebx	// features returned in ebx
				mov local_ecx, ecx	// features returned in ecx
				mov local_edx, edx	// features returned in edx
		}
	} 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{ 
		retval = false; 
	}

	out_eax = local_eax;
	out_ebx = local_ebx;
	out_ecx = local_ecx;
	out_edx = local_edx;

	_asm popad

		return retval;
}

//////////////////////////////////////////////////////////////////////////
// 功能：检查是否支持MMX:EDX的第23位:CPU是否支持IA MMX
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
bool CheckMMXTechnology(void)
{
	unsigned long eax,ebx,edx,unused;
	if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

	return ( edx & 0x800000 ) != 0;
}
//////////////////////////////////////////////////////////////////////////
// 功能：检查是否支持SSE：EDX的第26位:CPU是否支持IA SSE
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
bool CheckSSETechnology(void)
{
	unsigned long eax,ebx,edx,unused;
	if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

	return ( edx & 0x2000000L ) != 0;
}
//////////////////////////////////////////////////////////////////////////
// 功能：检查是否支持SSE2：EDX的第27位是否支持SSE2
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
bool CheckSSE2Technology(void)
{
	unsigned long eax,ebx,edx,unused;
	if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

	return ( edx & 0x04000000 ) != 0;
}
//////////////////////////////////////////////////////////////////////////
// 功能：检查是否支持3DNow
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
bool Check3DNowTechnology(void)
{
	unsigned long eax, unused;
	// EAX=80000001H,专门用于检测AMD的
	if( !cpuid(0x80000000,eax,unused,unused,unused) )
		return false;

	if ( eax > 0x80000000L )
	{
		if( !cpuid(0x80000001,unused,unused,unused,eax) )
			return false;

		return ( eax & 1<<31 ) != 0;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
// 检查是否支持条件MOV：EDX中第15位
//
//////////////////////////////////////////////////////////////////////////
bool CheckCMOVTechnology()
{
	unsigned long eax,ebx,edx,unused;
	if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

	return ( edx & (1<<15) ) != 0;
}

bool CheckFCMOVTechnology(void)
{
	unsigned long eax,ebx,edx,unused;
	if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

	return ( edx & (1<<16) ) != 0;
}

bool CheckRDTSCTechnology(void)
{
	unsigned long eax,ebx,edx,unused;
	if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

	return ( edx & 0x10 ) != 0;
}

// 获取CPU厂商标识串
const char* GetProcessorVendorId()
{
	unsigned long unused, VendorIDRegisters[3];

	static char VendorID[13];

	memset( VendorID, 0, sizeof(VendorID) );
	if( !cpuid(0,unused, VendorIDRegisters[0], VendorIDRegisters[2], VendorIDRegisters[1] ) )
	{
		lstrcpyA( VendorID, "Generic_x86" ); 
	}
	else
	{
		memcpy_s( VendorID+0,sizeof( VendorIDRegisters[0] ), &(VendorIDRegisters[0]), sizeof( VendorIDRegisters[0] ) );
		memcpy_s( VendorID+4,sizeof( VendorIDRegisters[1] ), &(VendorIDRegisters[1]), sizeof( VendorIDRegisters[1] ) );
		memcpy_s( VendorID+8,sizeof( VendorIDRegisters[2] ), &(VendorIDRegisters[2]), sizeof( VendorIDRegisters[2] ) );
	}


	return VendorID;
}

// Returns non-zero if Hyper-Threading Technology is supported on the processors and zero if not.  This does not mean that 
// Hyper-Threading Technology is necessarily enabled.
//////////////////////////////////////////////////////////////////////////
// 功能：是否支持超线程（HT）
// 参数：
// 返回值：
//////////////////////////////////////////////////////////////////////////
static bool HTSupported(void)
{
	const unsigned int HT_BIT		 = 0x10000000;  // EDX[28] - Bit 28 set indicates Hyper-Threading Technology is supported in hardware.
	const unsigned int FAMILY_ID     = 0x0f00;      // EAX[11:8] - Bit 11 thru 8 contains family processor id
	const unsigned int EXT_FAMILY_ID = 0x0f00000;	// EAX[23:20] - Bit 23 thru 20 contains extended family  processor id
	const unsigned int PENTIUM4_ID   = 0x0f00;		// Pentium 4 family processor id

	unsigned long unused,
		reg_eax = 0, 
		reg_edx = 0,
		vendor_id[3] = {0, 0, 0};

	// verify cpuid instruction is supported
	if( !cpuid(0,unused, vendor_id[0],vendor_id[2],vendor_id[1]) 
		|| !cpuid(1,reg_eax,unused,unused,reg_edx) )
		return false;

	//  Check to see if this is a Pentium 4 or later processor
	if (((reg_eax & FAMILY_ID) ==  PENTIUM4_ID) || (reg_eax & EXT_FAMILY_ID))
		if (vendor_id[0] == 'uneG' && vendor_id[1] == 'Ieni' && vendor_id[2] == 'letn')
			return (reg_edx & HT_BIT) != 0;	// Genuine Intel Processor with Hyper-Threading Technology

	return false;  // This is not a genuine Intel processor.
}

// Returns the number of logical processors per physical processors.
//////////////////////////////////////////////////////////////////////////
// 功能：对于HT获取单个CPU对应的逻辑CPU的数目
//
//////////////////////////////////////////////////////////////////////////
static unsigned char LogicalProcessorsPerPackage(void)
{
	const unsigned NUM_LOGICAL_BITS = 0x00FF0000; // EBX[23:16] indicate number of logical processors per package

	unsigned long unused,
		reg_ebx = 0;
	if (!HTSupported()) 
		return 1; 

	if( !cpuid(1,unused,reg_ebx,unused,unused) )
		return 1;

	return (unsigned char) ((reg_ebx & NUM_LOGICAL_BITS) >> 16);
}

// Measure the processor clock speed by sampling the cycle count, waiting
// for some fraction of a second, then measuring the elapsed number of cycles.
//////////////////////////////////////////////////////////////////////////
// 功能：计算时钟速度
//
//////////////////////////////////////////////////////////////////////////
static long CalculateClockSpeed()
{
	//LARGE_INTEGER waitTime, startCount, curCount;
	//QueryPerformanceCounter(&startCount);
	//QueryPerformanceFrequency(&waitTime);
	//int scale = 5;		// Take 1/32 of a second for the measurement.
	//waitTime.QuadPart >>= scale;

	//CCycleCount start, end;
	//start.Sample();
	//do
	//{
	//	QueryPerformanceCounter(&curCount);
	//}
	//while(curCount.QuadPart - startCount.QuadPart < waitTime.QuadPart);
	//end.Sample();

	//return (end.m_Int64 - start.m_Int64) << scale;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 功能：获取所有CPU信息
//
//////////////////////////////////////////////////////////////////////////
const CPUInformation& GetCPUInformation()
{
	static CPUInformation pi;

	// Has the structure already been initialized and filled out?
	if( pi.m_Size == sizeof(pi) )
		return pi;

	// Redundant, but just in case the user somehow messes with the size.
	ZeroMemory(&pi, sizeof(pi));

	// Fill out the structure, and return it:
	pi.m_Size = sizeof(pi);

	// Grab the processor frequency:
	pi.m_Speed = CalculateClockSpeed();


	// Get the logical and physical processor counts:
	pi.m_nLogicalProcessors = LogicalProcessorsPerPackage();

	SYSTEM_INFO si;
	ZeroMemory( &si, sizeof(si) );

	GetSystemInfo( &si );

	pi.m_nPhysicalProcessors = (unsigned char)(si.dwNumberOfProcessors / pi.m_nLogicalProcessors);
	pi.m_nLogicalProcessors *= pi.m_nPhysicalProcessors;

	// Make sure I always report at least one, when running WinXP with the /ONECPU switch, 
	// it likes to report 0 processors for some reason.
	if( pi.m_nPhysicalProcessors == 0 && pi.m_nLogicalProcessors == 0 )
	{
		pi.m_nPhysicalProcessors = 1;
		pi.m_nLogicalProcessors = 1;
	}

	// Determine Processor Features:
	pi.m_bRDTSC = CheckRDTSCTechnology();
	pi.m_bCMOV  = CheckCMOVTechnology();
	pi.m_bFCMOV = CheckFCMOVTechnology();
	pi.m_bMMX   = CheckMMXTechnology();
	pi.m_bSSE   = CheckSSETechnology();
	pi.m_bSSE2  = CheckSSE2Technology();
	pi.m_b3DNow = Check3DNowTechnology();

	pi.m_szProcessorID = (char*)GetProcessorVendorId();
	return pi;
}