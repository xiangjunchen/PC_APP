/********************************************************************
	Copyright (c) 2004, Sobey数码 内容制作部
	All rights reserved.
	创建时间:	2004年8月3日   10:11
	文件名: 	g:\e7\chenyufeng_desktop_cg\desktopsobey\tpcommondata\src\tpbufferoperate\tpmaskoperateapi.h
	当前版本：	1.0
	作者:		陈玉峰
	摘要:		内存缓冲区操作
*********************************************************************/
#ifndef __TP_MEMORY_OPERATE_INC__
#define __TP_MEMORY_OPERATE_INC__

// 设置内存为某一值
void* CALLBACK TP_MemSet(IN OUT void* pDest,
						 IN int nChar,
						 IN int nSize);
// 内存拷贝
void* CALLBACK TP_Memcpy(IN OUT void* pDest,
			IN const void* pSrc,
			IN int nSize);

#endif	// __TP_MEMORY_OPERATE_INC__