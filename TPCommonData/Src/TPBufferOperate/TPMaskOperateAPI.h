/********************************************************************
	Copyright (c) 2004, Sobey数码 内容制作部
	All rights reserved.
	创建时间:	2004年8月3日   10:11
	文件名: 	g:\e7\chenyufeng_desktop_cg\desktopsobey\tpcommondata\src\tpbufferoperate\tpmaskoperateapi.h
	当前版本：	1.0
	作者:		陈玉峰
	摘要:		掩模缓冲区操作
*********************************************************************/
#ifndef __TP_MASK_OPERATE_INC__
#define __TP_MASK_OPERATE_INC__

// 两MASK相减:DA=DA-SA,饱和相减
LRESULT CALLBACK TP_SubMask(IN OUT BYTE* pbyDestMask,
							IN int nDestX,
							IN int nDestY,
							IN int nDestMaskWidth,
							IN int nDestMaskHeight,
							IN BYTE* pbySrcMask,
							IN int nSrcX,
							IN int nSrcY,
							IN int nSrcMaskWidth,
							IN int nSrcMaskHeight,
							IN int nOperateWidth,	// 要操作的宽、高
							IN int nOperateHeight,
							IN int nType);			// 饱和度处理类型

#endif // __TP_MASK_OPERATE_INC__