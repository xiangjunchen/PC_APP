/********************************************************************
	Copyright (c) 2004, Sobey数码 内容制作部
	All rights reserved.
	创建时间:	2004年8月3日   10:10
	文件名: 	g:\e7\chenyufeng_desktop_cg\desktopsobey\tpcommondata\src\tpbufferoperate\tpmaskoperateapi.cpp
	当前版本：	1.0
	作者:		陈玉峰
	摘要:		掩模缓冲区操作
*********************************************************************/
#include "stdAfx.h"
#include "TPMaskOperateAPI.h"
#include "TPBufferOperateAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// DA = DA-SA
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
							IN int nOperateWidth,
							IN int nOperateHeight,
							IN int nType)			// 处理类型
{
	// 数据范围检测
	if(nDestX >= nDestMaskWidth || nDestY >= nDestMaskHeight)
		return E_FAIL;
	if(nDestMaskWidth <= 0||nDestMaskHeight <= 0 || nSrcMaskHeight<=0||nSrcMaskWidth<=0)
		return E_FAIL;
	if(nSrcX >= nSrcMaskWidth || nSrcY >= nSrcMaskHeight)
		return E_FAIL;

	// 得到起点坐标和要拷贝的长宽
	if(nDestX < 0)
	{
		nOperateWidth += nDestX;
		nSrcX -= nDestX;
		nDestX = 0;
	}
	if(nDestY < 0)
	{
		nOperateHeight += nDestY;
		nSrcY -= nDestY;
		nDestY = 0;
	}

	if(nSrcX < 0)
	{
		nOperateWidth += nSrcX;
		nDestX -= nSrcX;
		nSrcX = 0;
	}
	if(nSrcY < 0)
	{
		nOperateHeight += nSrcY;
		nDestY -= nSrcY;
		nSrcY = 0;
	}
	int nTempWidth = nSrcX+nOperateWidth;
	int nTempHeight = nSrcY+nOperateHeight;
	if(nTempWidth > nSrcMaskWidth)
		nOperateWidth -= (nTempWidth-nSrcMaskWidth);
	if(nTempHeight > nSrcMaskHeight)
		nOperateHeight -= (nTempHeight-nSrcMaskHeight);
	nTempWidth = nDestX+nOperateWidth;
	nTempHeight = nDestY+nOperateHeight;
	if(nTempWidth > nDestMaskWidth)
		nOperateWidth -= (nTempWidth-nDestMaskWidth);
	if(nTempHeight > nDestMaskHeight)
		nOperateHeight -= (nTempHeight-nDestMaskHeight);
	if(nOperateWidth <= 0||nOperateHeight<= 0)
		return E_FAIL;

	// 得到目标缓冲区起始点和源缓冲区起始点
	BYTE* pDes = pbyDestMask + nDestY*nDestMaskWidth + nDestX;
	BYTE* pSrc = pbySrcMask + nSrcY*nSrcMaskWidth + nSrcX;
	if(pSrc == NULL||pDes == NULL)
		return E_FAIL;

#if 1
	// 一行行赋值,SSE2优化
	_declspec(align(16)) DWORD dwMask1[]={0x00FF00FF,0x00FF00FF, 0x00FF00FF, 0x00FF00FF};
	_declspec(align(16)) DWORD dwMask2[]={0x000000FF,0x000000FF, 0x000000FF, 0x000000FF};
	_asm
	{
		XOR ECX, ECX	// i清0
LOOP1:
		CMP ECX, nOperateHeight // 第一层循环(i<nDestHeight)
		JGE	EXITLOOP1	 // 退出第一层循环(i>=nDestHeight)
		PUSH ECX
		XOR ECX, ECX	 // j清0， 第二层循环计数器
		MOV EAX, pSrc	 // 源和目标缓冲区指针 
		MOV EDX, pDes
LOOP2:
		ADD ECX, 16			// 每次计算16个值，不足16时跳到循环体2中
		CMP ECX, nOperateWidth
		JG LOOP2BODY2		

		MOVDQU XMM0, [EDX]	// 取Dest的16个Byte值，128位
		MOVDQU XMM1, [EAX]	// 取Src的16个Byte值
		PMAXUB XMM0, XMM1	// 取两者最大值
		PSUBB  XMM0, XMM1	// 最大值－SRC，保证饱和相减
		MOVDQU [EDX], XMM0

		ADD EAX, 16			// 指针后移
		ADD EDX, 16
		JMP LOOP2
LOOP2BODY2:
		SUB ECX, 16			// 不足16时
LOOP2BODY3:
		ADD ECX, 4			// 每次计算4个值，不足4时跳到循环体4中
		CMP ECX, nOperateWidth
		JG LOOP2BODY4	

		MOVD XMM0, [EDX]	// 取Dest的4个Byte值，32位
		MOVD XMM1, [EAX]	// 取Src的4个Byte值，32位
		PMAXUB XMM0, XMM1
		PSUBB  XMM0, XMM1	// 最大值－SRC，保证饱和相减
		MOVD  [EDX], XMM0	// 将结果赋给Dest

		ADD EAX, 4
		ADD EDX, 4
		JMP LOOP2BODY3
LOOP2BODY4:
		SUB ECX, 4			// 不足4时
LOOP2BODY5:
		ADD ECX, 2			// 每次计算2个值， 不足2时跳到循环体6中
		CMP ECX, nOperateWidth
		JG LOOP2BODY6

		PUSH EAX
		XOR EAX, EAX
		MOV AX, [EDX]
		MOVD XMM0, EAX
		POP EAX

		PUSH EDX
		XOR EDX, EDX
		MOV DX, [EAX]
		MOVD XMM1, EDX
		POP EDX

		PMAXUB XMM0, XMM1
		PSUBB  XMM0, XMM1	// 最大值－SRC，保证饱和相减

		PUSH EAX
		MOVD EAX, XMM0
		MOV  [EDX], AX		// 将结果赋给Dest
		POP EAX

		ADD EAX, 2
		ADD EDX, 2
		JMP LOOP2BODY5
LOOP2BODY6:
		SUB ECX, 2
		CMP ECX, nOperateWidth
		JGE LOOP2END

		XOR ECX, ECX
		MOV CL, [EAX]	// SA放在ECX
		XOR EAX, EAX
		MOV AL, [EDX]	// DA放在EAX				
		SUB AL, CL		// DA-SA
		JNC ZERO
		MOV AL, 0
ZERO:
		MOV BYTE PTR [EDX], AL
LOOP2END:
		MOV EAX, nDestMaskWidth
		ADD [pDes], EAX

		MOV EAX, nSrcMaskWidth
		ADD [pSrc], EAX

		POP ECX
		INC	ECX			 // 计数器自增，返回循环1 
		JMP LOOP1
EXITLOOP1:
										EMMS			// 清空
	}
#endif
#if 0
	for(int i=0; i<nOperateHeight; i++)
	{
		for(int j=0; j<nOperateWidth; j++)
		{
			int nMax = max(pSrc[j], pDes[j]);
			pDes[j] = nMax - pSrc[j];		// 保证如果SA大时为0：饱和相减
		}
		pDes += nDestMaskWidth;
		pSrc += nSrcMaskWidth;
	}
#endif
	return S_OK;
}