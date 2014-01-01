// TPAVTagFile.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "TPAVTgaFile.h"
#include "TgaFile.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DWORD g_dwBlockSize = 0;

typedef struct tag_TPAVTgaFileData
{
	CTgaFile* pTgaImage;
	DWORD* pBuffer;
	TCHAR ptszFileName[MAX_PATH];
}TPAVTgaFileData;

int TPAVTgaFileOpen(HAVTGAFILE& hFile,TCHAR* ptszFileName,DWORD dwOpenType)
{
	TPAVTgaFileData* pTgaFileData=new TPAVTgaFileData;
	pTgaFileData->pTgaImage=NULL;
	pTgaFileData->pBuffer=NULL;
	memset(pTgaFileData->ptszFileName,0,sizeof(TCHAR)*MAX_PATH);
	_tcscpy_s(pTgaFileData->ptszFileName,MAX_PATH,ptszFileName);
   	hFile=pTgaFileData;
	pTgaFileData->pTgaImage=new CTgaFile();
	if (!pTgaFileData->pTgaImage->OpenTgaFile(ptszFileName,dwOpenType))
	{
		//{2010-5-25:CD2009.10.01:chenzhi:add:23395,怀疑是打开tga文件失败，在这里加日志和测试补丁。
#ifdef TP_NS_NET
		CString str;
		INT iCount = 0;
		str.Format(_T("\nTPAVTgaFile.cpp::TPAVTgaFileOpen() open %s file failed for 1 time!\n"),ptszFileName);
		SB_LogManager::WriteLog(str);
		for (;iCount<3;iCount++)
		{
			if (pTgaFileData->pTgaImage->OpenTgaFile(ptszFileName,dwOpenType))
			{
				return 1;
			}
		}
		str.Format(_T("\nTPAVTgaFile.cpp::TPAVTgaFileOpen() open %s file failed for 4 times!!\n"),ptszFileName);
		SB_LogManager::WriteLog(str);
#endif //TP_NS_NET
		//}
		//打开失败
		delete pTgaFileData->pTgaImage;
		delete pTgaFileData;
		hFile = NULL;
		return 0;
	}
	return 1;
}

int TPAVTgaFileOpen(HAVTGAFILE& hFile,TCHAR* ptszFileName)
{
	return TPAVTgaFileOpen(hFile,ptszFileName,TP_IMG_FILE_READ);
}

int TPAVTgaFileClose(HAVTGAFILE hFile)
{
	TPAVTgaFileData* pTgaFileData=(TPAVTgaFileData*)hFile;
	if(NULL != pTgaFileData)
	{
		if (NULL != pTgaFileData->pTgaImage)
		{
			pTgaFileData->pTgaImage->CloseTgaFile();
			delete pTgaFileData->pTgaImage;
		}
		delete pTgaFileData;
		return 1;
	}
	return 0;
}

// 返回此图像是否含有 alpha 
bool NXImageHaveAlpha(HAVTGAFILE hFile)
{
	TPAVTgaFileData* pTgaFileData=(TPAVTgaFileData*)hFile;
	if(pTgaFileData == NULL || pTgaFileData->pTgaImage == NULL)
	{
		return false;
	}

	if(pTgaFileData->pTgaImage->_NXHaveAlpha() == TRUE)
	{
		return true;
	}
	return false;
}

// 返回此图像是否可以分块读取 by ljj@2011-4-28
bool NXTgaSupportBlockRead(HAVTGAFILE hFile)
{
	TPAVTgaFileData* pTgaFileData=(TPAVTgaFileData*)hFile;
	if(pTgaFileData == NULL || pTgaFileData->pTgaImage == NULL)
	{
		return false;
	}

	if(pTgaFileData->pTgaImage->_NXSupportBlockRead() == TRUE)
	{
		return true;
	}
	return false;
}

int TPAVTgaFileGetImageInfo(HAVTGAFILE hFile,int& iX, int& iY, int& iWidth,int& iHeight,int &iDepth,BYTE*& pBuf, long& lBufSize)
{
	TPAVTgaFileData* pTgaFileData=(TPAVTgaFileData*)hFile;;
	IMAGE_INFO stImageInfo;
	if(NULL != pTgaFileData && NULL != pTgaFileData->pTgaImage)
	{
		memset(&stImageInfo,0,sizeof(IMAGE_INFO)); 
		if(pTgaFileData->pTgaImage->GetTGAInfo(stImageInfo,pBuf, lBufSize))
		{
			iWidth=stImageInfo.Width;
			iHeight=stImageInfo.Height;
			iDepth = stImageInfo.bits;
			iX = stImageInfo.Xstart;
			iY = stImageInfo.Ystart;
			return stImageInfo.llFileSize;
		}
	}
	return 0;
}

int TPAVTgaFileGetImageInfo(HAVTGAFILE hFile,int& iWidth,int& iHeight,int &iDepth)
{
	BYTE* pBuf = NULL;
	long lBufSize = 0;
	int iX = 0, iY = 0;
	return TPAVTgaFileGetImageInfo(hFile,iX,iY,iWidth,iHeight,iDepth,pBuf,lBufSize);
}
int TPAVTgaFilePutImageData (HAVTGAFILE hFile, int iX, int iY, int iWidth, int iHeight, int iDepth, BYTE *pData)
{
	return TPAVTgaFilePutImageData(hFile,iX,iY,iWidth,iHeight,iDepth,pData,NULL,0);
}

int TPAVTgaFilePutImageData (HAVTGAFILE hFile, int iX, int iY, int iWidth, int iHeight, int iDepth, BYTE *pData, BYTE* pCustomData, long lCustomDataSize)
{
	TPAVTgaFileData* pTgaFileData=(TPAVTgaFileData*)hFile;
	IMAGE_INFO img_info;
	img_info.Xstart = iX;
	img_info.Ystart = iY;
	img_info.Width= iWidth;
	img_info.Height = iHeight;
	img_info.bits = iDepth;
	img_info.fCompression = FALSE;
	img_info.dwBmpType = 0;
	if(NULL != pTgaFileData && NULL != pTgaFileData->pTgaImage)
	{
		pTgaFileData->pTgaImage->WriteTGAImage (img_info, (DWORD*)pData,pCustomData,lCustomDataSize);
		return 1;
	}
	return 0;
}

int TPAVTgaFileGetImageData(HAVTGAFILE hFile,BYTE* pData)
{
	TPAVTgaFileData* pTgaFileData=(TPAVTgaFileData*)hFile;
	if(NULL != pTgaFileData && NULL != pTgaFileData->pTgaImage)
	{
		LARGE_INTEGER begin_tick ,end_tick,freq;
		QueryPerformanceFrequency (&freq);
		QueryPerformanceCounter (&begin_tick);
		pTgaFileData->pTgaImage->LoadTGAImage((DWORD*)pData);
		QueryPerformanceCounter (&end_tick);
		double dbTime = ((double) (end_tick.LowPart - begin_tick.LowPart)*1000.0) / (double) (freq.LowPart);
		//TRACE(_T("\n----Get Tga time = %.3f!------\n"),dbTime);
		return 1;
	}
	else
	{
		return 0;
	}
}
// ##### Start by wanggaofei @ 2010/04/27 10:33 #####
// Comments: 分块读取图像数据，分两个步骤，先为数据读取做准备，然后开始真正读取图像数据
//					  主要是读取调色板。

// ###### End by wanggaofei @ 2010/04/27 10:33 ######
// 分块读取图像数据
int NXAVTgaFilePrepareGetImageData(HAVTGAFILE hFile,BYTE* pData,BOOL &bLeftBottom)
{
	TPAVTgaFileData* pTgaFileData=(TPAVTgaFileData*)hFile;
	if(NULL != pTgaFileData && NULL != pTgaFileData->pTgaImage)
	{
		LARGE_INTEGER begin_tick ,end_tick,freq;
		QueryPerformanceFrequency (&freq);
		QueryPerformanceCounter (&begin_tick);
		pTgaFileData->pTgaImage->NXPrepareLoadTGAImage((DWORD*)pData);
		bLeftBottom = pTgaFileData->pTgaImage->_NXImageBeginLeftBottom(); // TRUE - 图象数据从左下角开始 FALSE - 图象数据从左上角开始
		QueryPerformanceCounter (&end_tick);
		double dbTime = ((double) (end_tick.LowPart - begin_tick.LowPart)*1000.0) / (double) (freq.LowPart);
		//TRACE(_T("\n----Get Tga time = %.3f!------\n"),dbTime);
		return 1;
	}
	else
	{
		return 0;
	}
}

int NXAVTgaFileGetImageData(HAVTGAFILE hFile, int iBeginRow, int iEndRow)
{
	TPAVTgaFileData* pTgaFileData=(TPAVTgaFileData*)hFile;
	if(NULL != pTgaFileData && NULL != pTgaFileData->pTgaImage)
	{
		LARGE_INTEGER begin_tick ,end_tick,freq;
		QueryPerformanceFrequency (&freq);
		QueryPerformanceCounter (&begin_tick);
		pTgaFileData->pTgaImage->NXReadImage(iBeginRow, iEndRow);
		QueryPerformanceCounter (&end_tick);
		double dbTime = ((double) (end_tick.LowPart - begin_tick.LowPart)*1000.0) / (double) (freq.LowPart);
		//TRACE(_T("\n----Get Tga time = %.3f!------\n"),dbTime);
		return 1;
	}
	else
	{
		return 0;
	}
}
// 分块读取图像数据
int TPAVTgaFileUpdateCustomData (HAVTGAFILE hFile,BYTE* pCustomData, long lCustomDataSize)//add by ljj @ 2009-3-4
{
	TPAVTgaFileData* pTgaFileData=NULL;
	if(hFile)
	{
		pTgaFileData=(TPAVTgaFileData*)hFile;
		pTgaFileData->pTgaImage->UpdateCustomData(pCustomData,lCustomDataSize);
		return 1;
	}
	else
	{
		return 0;
	}

}
// 拷贝一行 Buffer
//////////////////////////////////////////////////////////////////////////
void memcpy_line(OUT void *des,		// 目标
				 IN void *src,		// 源
				 IN int na16,		// 宽度除以 16 的商
				 IN int nm16)		// 宽度除以 16 的余数
{
	_asm{
		mov			edi, des;
		mov			esi, src;
		mov			ecx, na16;
		mov			edx, nm16;
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_a8;
loop_a16:
		movups		xmm0, [esi];
		movups		[edi], xmm0;
		add			esi, 0x10;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_a8:
		test		edx, 0x00000008;
		jz			loop_a4;
		movlps		xmm0, [esi];
		movlps		[edi], xmm0;
		add			esi, 0x08;
		add			edi, 0x08;
loop_a4:
		test		edx, 0x00000004;
		jz			loop_a2;
		mov			eax, [esi];
		add			esi, 0x04;
		mov			[edi], eax;
		add			edi, 0x04;
loop_a2:
		test		edx, 0x00000002;
		jz			loop_a1;
		mov			ax, [esi];
		add			esi, 0x02;
		mov			[edi], ax;
		add			edi, 0x02;
loop_a1:
		test		edx, 0x00000001;
		jz			loop_end;
		mov			al, [esi];
		mov			[edi], al;
loop_end:
	}
}

//////////////////////////////////////////////////////////////////////////
// 以字节为单位设置一行 Buffer
//////////////////////////////////////////////////////////////////////////
void memset_char_line(OUT void *des,	// 目标
					  IN char value,	// 设置值
					  IN int na16,		// 宽度除以 16 的商
					  IN int nm16)		// 宽度除以 16 的余数
{
	_asm{
		mov			edi, des;
		mov			ecx, na16;
		movzx		eax, value;		// 000c
		mov			edx, nm16;
		movd		xmm0, eax;		// 0000 0000 0000 000c
		pshuflw		xmm0, xmm0, 0x00;	// 0000 0000 0c0c 0c0c
		packuswb	xmm0, xmm0;		// 0000 cccc 0000 cccc
		pshufd		xmm0, xmm0, 0x00;	// cccc cccc cccc cccc
		movd		eax, xmm0;		// cccc
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_a8;
loop_a16:
		movups		[edi], xmm0;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_a8:
		test		edx, 0x00000008;
		jz			loop_a4;
		movlps		[edi], xmm0;
		add			edi, 0x08;
loop_a4:
		test		edx, 0x00000004;
		jz			loop_a2;
		mov			[edi], eax;
		add			edi, 0x04;
loop_a2:
		test		edx, 0x00000002;
		jz			loop_a1;
		mov			[edi], ax;
		add			edi, 0x02;
loop_a1:
		test		edx,0x00000001;
		jz			loop_end;
		mov			[edi], al; 
loop_end:
	}
}
//////////////////////////////////////////////////////////////////////////
// 以双字为单位设置一行 Buffer
//////////////////////////////////////////////////////////////////////////
void memset_dword_line(OUT void *des,	// 目标
					   IN DWORD value,	// 设置值
					   IN int na16,		// 宽度除以 16 的商
					   IN int nm16)		// 宽度除以 16 的余数
{
	_asm{
		mov			edi, des;
		mov			ecx, na16;
		mov			eax, value;		// v
		mov			edx, nm16;
		movd		xmm0, eax;		// 0 0 0 v
		shufps		xmm0, xmm0, 0x00;	// v v v v
		test		ecx, ecx;
		jnz			loop_a16;
		jmp			loop_a8;
loop_a16:
		movups		[edi], xmm0;
		add			edi, 0x10;
		sub			ecx, 0x01;
		jnz			loop_a16;
loop_a8:
		test		edx, 0x00000008;
		jz			loop_a4;
		movlps		[edi], xmm0;
		add			edi, 0x08;
loop_a4:
		test		edx, 0x00000004;
		jz			loop_end;
		mov			[edi], eax;
loop_end:
	}
}
// 拷贝一行 Buffer
LRESULT TP_MemcpyLine(OUT void *des,		// 目标
							   IN void *src,		// 源
							   IN int nlen)			// 字节长度
{
	memcpy_line(des, src, (nlen >> 4), (nlen & 0x0f));

	return TRUE;
}

// 以字节为单位设置一行 Buffer
LRESULT TP_MemsetChar(OUT void *des,		// 目标
							   IN char value,		// 字节值
							   IN int nlen)			// 字节长度
{
	memset_char_line(des, value, (nlen >> 4), (nlen & 0x0f));

	return TRUE;
}

// 一行 Buffer 的双字设值操作
LRESULT TP_MemsetDWORD(OUT DWORD *des,		// 目标
								IN DWORD value,		// 源
								IN int nsize)		// 双字的个数
{
	memset_dword_line(des, value, (nsize >> 2), ((nsize & 0x03) << 2));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void color24_a_2_color32(OUT void* color32,			// 32位buffer
						 IN void* color24,			// 24位buffer
						 IN void* alpha,			// A通道
						 IN int count)				// 像素个数
{
	if(!color32 || !color24 || !alpha || count <= 0)
	{
		assert(false);
		return;
	}
	__asm
	{
		pcmpeqb		xmm7, xmm7;
		xorpd		xmm6, xmm6;
		psrld		xmm7, 08h;				// 00FFFFFF 00FFFFFF 00FFFFFF 00FFFFFF
		mov			ecx, count;
		mov			esi, color24;
		mov			edi, color32;
		mov			ebx, alpha;
		mov			edx, ecx;
		and			ecx, 7;
		shr			edx, 03h;				// 一次处理8个像素
		prefetchnta	[esi];
		test		edx, edx;
		jz			loop_1;
loop_8:
		prefetchnta	[esi + 18h];
		movlps		xmm0, [esi];			// **** **** GRBG RBGR
		movups		xmm1, [esi + 08h];		// BGRB GRBG RBGR BGRB
		movlps		xmm2, [ebx];			// **** **** AAAA AAAA
		movlhps		xmm0, xmm1;				// RBGR BGRB GRBG RBGR
		shufps		xmm1, xmm1, 0e9h;		// BGRB GRBG GRBG RBGR
		shufps		xmm0, xmm0, 94h;		// BGRB GRBG GRBG RBGR
		punpcklbw	xmm2, xmm6;				// 0A0A 0A0A 0A0A 0A0A
		pshuflw		xmm0, xmm0, 94h;
		pshuflw		xmm1, xmm1, 94h;
		pshufhw		xmm0, xmm0, 0e9h;
		pshufhw		xmm1, xmm1, 0e9h;		// BGRB RBGR BGRB RBGR
		movaps		xmm3, xmm0;
		shufps		xmm0, xmm1, 88h;
		shufps		xmm3, xmm1, 0ddh;
		movaps		xmm4, xmm2;
		punpcklwd	xmm2, xmm6;
		punpckhwd	xmm4, xmm6;
		pslld		xmm2, 18h;
		pslld		xmm4, 18h;
		andpd		xmm0, xmm7;
		psrld		xmm3, 08h;
		movaps		xmm1, xmm0;
		punpckldq	xmm0, xmm3;
		punpckhdq	xmm1, xmm3;
		orpd		xmm0, xmm2;
		orpd		xmm1, xmm4;
		movups		[edi], xmm0;
		movups		[edi + 10h], xmm1;
		add			edi, 20h;
		add			esi, 18h;
		add			ebx, 08h;
		dec			edx;
		jnz			loop_8;
loop_1:
		test		ecx, ecx;
		jz			loop_0;
		mov			al, [ebx];
		movsw
			movsb
			stosb
			inc			ebx;
		dec			ecx;
		jnz			loop_1;
loop_0:
	}
}
// 补充 Alpha 通道为 0xff
void color24_to_color32(OUT void* color32,	// *G@A 缓存，存放方式 *0 G0 @0 A0 *1 G1 @1 A1 ……
						IN void* color24,	// *G@ 输入，存放方式 *0 G0 @0 *1 G1 @1 ……（* 与 @ 表示 R 或 B，由像素的存储方式确定）
						IN int nap5,		// 源像素宽度除以 5 的商与余数（正好满足十六字节对齐）
						IN int nmp5)
{
	// LaiShoubo @ 2009-11-5
	__asm
	{
		pcmpeqb		xmm7, xmm7;
		mov			eax, 05h;
		mul			nap5;
		mov			esi, color24;
		mov			edi, color32;
		add			eax, nmp5;
		mov			ecx, eax;
		movd		eax, xmm7;
		mov			edx, ecx;
		and			ecx, 0fh;
		shr			edx, 04h;				// 一次处理16个像素
		movaps		xmm6, xmm7;
		pslld		xmm7, 18h;				// FF000000 FF000000 FF000000 FF000000
		psrld		xmm6, 08h;				// 00FFFFFF 00FFFFFF 00FFFFFF 00FFFFFF
		prefetchnta	[esi];
		test		edx, edx;
		jz			loop_1;
		test		edi, 0fh;
		jz			loop_a16;
		test		edi, 03h;
		jnz			loop_16;
		shl			edx, 04h;
		add			edx, ecx;
loop_pre_1:
		movsb
			movsb
			movsb
			stosb
			dec			edx;
		test		edi, 0fh;
		jnz			loop_pre_1;
		mov			ecx, edx;
		shr			edx, 04h;
		and			ecx, 0fh;
loop_a16:
		prefetchnta	[esi + 30h];
		movups		xmm0, [esi];			// RBGR BGRB GRBG RBGR
		movups		xmm2, [esi + 10h];		// GRBG RBGR BGRB GRBG
		movaps		xmm1, xmm0;
		movups		xmm3, [esi + 20h];		// BGRB GRBG RBGR BGRB
		shufpd		xmm1, xmm2, 01h;		// BGRB GRBG RBGR BGRB
		shufpd		xmm2, xmm3, 01h;		// RBGR BGRB GRBG RBGR
		shufps		xmm0, xmm0, 94h;		// BGRB GRBG GRBG RBGR
		shufps		xmm2, xmm2, 94h;
		shufps		xmm1, xmm1, 0e9h;		// BGRB GRBG GRBG RBGR
		shufps		xmm3, xmm3, 0e9h;
		pshuflw		xmm0, xmm0, 94h;
		pshuflw		xmm1, xmm1, 94h;
		pshuflw		xmm2, xmm2, 94h;
		pshuflw		xmm3, xmm3, 94h;
		pshufhw		xmm0, xmm0, 0e9h;
		pshufhw		xmm1, xmm1, 0e9h;
		pshufhw		xmm2, xmm2, 0e9h;
		pshufhw		xmm3, xmm3, 0e9h;		// BGRB RBGR BGRB RBGR
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm2;
		shufps		xmm0, xmm1, 88h;
		shufps		xmm2, xmm3, 88h;
		shufps		xmm4, xmm1, 0ddh;
		shufps		xmm5, xmm3, 0ddh;
		andpd		xmm0, xmm6;
		andpd		xmm2, xmm6;
		psrld		xmm4, 08h;
		psrld		xmm5, 08h;
		orpd		xmm0, xmm7;
		orpd		xmm4, xmm7;
		orpd		xmm2, xmm7;
		orpd		xmm5, xmm7;
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpckldq	xmm0, xmm4;
		punpckhdq	xmm1, xmm4;
		punpckldq	xmm2, xmm5;
		punpckhdq	xmm3, xmm5;
		movntps		[edi], xmm0;
		movntps		[edi + 10h], xmm1;
		movntps		[edi + 20h], xmm2;
		movntps		[edi + 30h], xmm3;
		add			edi, 40h;
		add			esi, 30h;
		dec			edx;
		jnz			loop_a16;
		sfence
			jmp			loop_1;
loop_16:
		prefetchnta	[esi + 30h];
		movups		xmm0, [esi];			// RBGR BGRB GRBG RBGR
		movups		xmm2, [esi + 10h];		// GRBG RBGR BGRB GRBG
		movaps		xmm1, xmm0;
		movups		xmm3, [esi + 20h];		// BGRB GRBG RBGR BGRB
		shufpd		xmm1, xmm2, 01h;		// BGRB GRBG RBGR BGRB
		shufpd		xmm2, xmm3, 01h;		// RBGR BGRB GRBG RBGR
		shufps		xmm0, xmm0, 94h;		// BGRB GRBG GRBG RBGR
		shufps		xmm2, xmm2, 94h;
		shufps		xmm1, xmm1, 0e9h;		// BGRB GRBG GRBG RBGR
		shufps		xmm3, xmm3, 0e9h;
		pshuflw		xmm0, xmm0, 94h;
		pshuflw		xmm1, xmm1, 94h;
		pshuflw		xmm2, xmm2, 94h;
		pshuflw		xmm3, xmm3, 94h;
		pshufhw		xmm0, xmm0, 0e9h;
		pshufhw		xmm1, xmm1, 0e9h;
		pshufhw		xmm2, xmm2, 0e9h;
		pshufhw		xmm3, xmm3, 0e9h;		// BGRB RBGR BGRB RBGR
		movaps		xmm4, xmm0;
		movaps		xmm5, xmm2;
		shufps		xmm0, xmm1, 88h;
		shufps		xmm2, xmm3, 88h;
		shufps		xmm4, xmm1, 0ddh;
		shufps		xmm5, xmm3, 0ddh;
		andpd		xmm0, xmm6;
		andpd		xmm2, xmm6;
		psrld		xmm4, 08h;
		psrld		xmm5, 08h;
		orpd		xmm0, xmm7;
		orpd		xmm4, xmm7;
		orpd		xmm2, xmm7;
		orpd		xmm5, xmm7;
		movaps		xmm1, xmm0;
		movaps		xmm3, xmm2;
		punpckldq	xmm0, xmm4;
		punpckhdq	xmm1, xmm4;
		punpckldq	xmm2, xmm5;
		punpckhdq	xmm3, xmm5;
		movups		[edi], xmm0;
		movups		[edi + 10h], xmm1;
		movups		[edi + 20h], xmm2;
		movups		[edi + 30h], xmm3;
		add			edi, 40h;
		add			esi, 30h;
		dec			edx;
		jnz			loop_16;
loop_1:
		test		ecx, ecx;
		jz			loop_0;
		movsb
			movsb
			movsb
			stosb
			dec			ecx;
		jnz			loop_1;
loop_0:
	}
	//	_asm{       // 程序内部以 RGB 和 RGBA 为例做说明
	//		mov			edi, color32;		// 前四个像素
	//		mov			edx, color32;		// 最后一个像素
	//		mov			ecx, nap5;
	//		mov			esi, color24;
	//		add			edx, 0x10;
	//		movdqa		xmm0, gc_dwMaskAlpha;
	//		test		ecx, ecx;
	//		jnz			loop_a5;
	//		jmp			loop_m4;
	//loop_a5:		// 一次做五个像素
	//		movdqu		xmm7, [esi];		// RBGR BGRB GRBG RBGR
	//		movdqa		xmm4, xmm7;
	//		movdqa		xmm5, xmm7;
	//		movdqa		xmm6, xmm7;
	//		pshufd		xmm3, xmm7, 0x00;	// RBGR RBGR RBGR RBGR
	//		pshufd		xmm2, xmm7, 0xaa;	// BGRB BGRB BGRB BGRB
	//		pslld		xmm4, 0x08;			// * *  *B1G1 0  *
	//		psrld		xmm5, 0x10;			// * * 00G2R2 *
	//		psrld		xmm6, 0x08;			// * 0B3G3R3 * *
	//		psrld		xmm3, 0x18;			// 000R1 000R1 000R1 000R1
	//		pslld		xmm2, 0x10;			// *B2 00  *B2 00  *B2 00  *B2 00
	//		por			xmm4, xmm3;			// * * *B1G1R1 *
	//		por			xmm5, xmm2;			// * * *B2G2R2 *
	//		shufps		xmm4, xmm7, 0x05;	// *B0G0R0 *B0G0R0 *B1G1R1 *B1G1R1
	//		shufps		xmm5, xmm6, 0xa5;	// 0B3G3R3 0B3G3R3 *B2G2R2 *B2G2R2
	//		shufps		xmm7, xmm7, 0xff;	// *B4G4R4 *B4G4R4 *B4G4R4 *B4G4R4
	//		shufps		xmm4, xmm5, 0x82;	// 0B3G3R3 *B2G2R2 *B1G1R1 *B0G0R0
	//		por			xmm7, xmm0;
	//		por			xmm4, xmm0;
	//		movss		[edx], xmm7;
	//		movdqu		[edi], xmm4;
	//		add			esi, 0x0f;
	//		add			edx, 0x14;
	//		add			edi, 0x14;
	//		sub			ecx, 0x01;
	//		jnz			loop_a5;
	//loop_m4:		// 剩余四个像素
	//		mov			ecx, nmp5;
	//		test		ecx, 0x04;
	//		jz			loop_m3;
	//		mov			eax, esi;
	//		add			esi, 0x08;
	//		movsd		xmm7, [eax];		// 0000 0000 GRBG RBGR
	//		movss		xmm6, [esi];		// 0000 0000 0000 BGRB
	//		pshufd		xmm2, xmm7, 0x92;	// 0000 GRBG RBGR 0000
	//		shufps		xmm6, xmm6, 0x05;	// BGRB BGRB 0000 0000
	//		movdqa		xmm4, xmm7;
	//		movdqa		xmm5, xmm2;
	//		movdqa		xmm3, xmm6;
	//		psrld		xmm2, 0x10;			// 0  00G2R2  *  0
	//		pslld		xmm4, 0x08;			// 0 0  *B1G1 0  *
	//		psrld		xmm5, 0x18;			// 0 *  000R1  0
	//		pslld		xmm3, 0x10;			// *  *B2 00  0 0
	//		psrld		xmm6, 0x08;			// 0B3G3R3 * 0 0
	//		por			xmm4, xmm5;			// 0 * *B1G1R1 *
	//		por			xmm2, xmm3;			// * *B2G2R2 * 0
	//		shufps		xmm4, xmm7, 0x05;	// *B0G0R0 *B0G0R0 *B1G1R1 *B1G1R1
	//		shufps		xmm2, xmm6, 0xfa;	// 0B3G3R3 0B3G3R3 *B2G2R2 *B2G2R2
	//		shufps		xmm4, xmm2, 0x82;	// 0B3G3R3 *B2G2R2 *B1G1R1 *B0G0R0
	//		por			xmm4, xmm0;
	//		movdqu		[edi], xmm4;
	//		jmp			loop_end;
	//loop_m3:		// 剩余三个像素
	//		cmp			ecx, 0x03;
	//		jnz			loop_m2;
	//		mov			eax, esi;
	//		add			esi, 0x08;
	//		sub			edx, 0x08;
	//		movsd		xmm7, [eax];		// 0000 0000 GRBG RBGR
	//		movss		xmm6, [esi];		// 0000 0000 0000 000B
	//		pshufd		xmm2, xmm7, 0xa1;	// 0000 0000 RBGR GRBG
	//		movdqa		xmm5, xmm2;
	//		movdqa		xmm4, xmm7;
	//		psrld		xmm2, 0x10;			// 0 0 * 00G2R2
	//		pslld		xmm6, 0x10;			// 0 0 0  0B2 00
	//		pslld		xmm4, 0x08;			// 0 0  *B1G1 0  *
	//		psrld		xmm5, 0x18;			// 0 0  000R1  *
	//		por			xmm2, xmm6;			// 0 0 * 0B2G2R2
	//		por			xmm4, xmm5;			// 0 0 *B1G1R1 *
	//		shufps		xmm4, xmm7, 0x05;	// *B0G0R0 *B0G0R0 *B1G1R1 *B1G1R1
	//		shufps		xmm4, xmm4, 0x22;	// *B1G1R1 *B0G0R0 *B1G1R1 *B0G0R0
	//		por			xmm2, xmm0;
	//		por			xmm4, xmm0;
	//		movss		[edx], xmm2;
	//		movsd		[edi], xmm4;
	//		jmp			loop_end;
	//loop_m2:		// 剩余两个像素
	//		test		ecx, 0x02;
	//		jz			loop_m1;
	//		mov			eax, esi;
	//		movsd		xmm7, [eax];		// 0000 0000 00BG RBGR
	//		pshufd		xmm2, xmm7, 0xa2;	// 0000 0000 RBGR 0000
	//		movdqa		xmm4, xmm7;
	//		psrld		xmm2, 0x18;			// 0 0 000R1 0
	//		pslld		xmm4, 0x08;			// 0 0  0B1G1 0  *
	//		por			xmm4, xmm2;			// 0 0 0B1G1R1 *
	//		shufps		xmm4, xmm7, 0x05;	// *B0G0R0 *B0G0R0 *B1G1R1 *B1G1R1
	//		shufps		xmm4, xmm4, 0x22;	// *B1G1R1 *B0G0R0 *B1G1R1 *B0G0R0
	//		por			xmm4, xmm0;
	//		movsd		[edi], xmm4;
	//		jmp			loop_end;
	//loop_m1:		// 剩余一个像素
	//		test		ecx, 0x01;
	//		jz			loop_end;
	//		movss		xmm7, [esi];
	//		por			xmm7, xmm0;
	//		movss		[edi], xmm7;
	//loop_end:
	//	}
}

// RGB Alpha 与 RGBA、BGR Alpha 与 BGRA 之间的转换

void TPCS_24_to_32_Frame(OUT BYTE* pDes32,				// 输出（存储方式 X0 X1 X2 A X0 X1 X2 A ……）
						 IN int nDesPitch,
						 IN BYTE* pSrc24,               // 24 位源输入（存储方式 X0 X1 X2 X0 X1 X2 ……）
						 IN int nSrc24Pitch,            // 源一行的字节宽度（必须为 4 的倍数）
						 IN BYTE* pSrcAlpha,            // 源 Alpha 输入（存储方式 A0 A1 A2 A3 ……）（如果输入为 NULL 则直接填 0xff）
						 IN int nSrcAlphaPitch,
						 IN int nWidth,                 // 源图大小
						 IN int nHeight,
						 IN BOOL bSrc24Revert,          // 图像是否行翻转标志
						 IN BOOL bSrcAlphaRevert)       // Alpha 是否行翻转标志
{
	assert(pDes32 && pSrc24);
	assert(nDesPitch >= ((nWidth + (nWidth & 0x01)) << 1));
	assert(nSrc24Pitch >= (nWidth * 3));
	assert(pSrcAlpha == NULL || nSrcAlphaPitch >= nWidth);

	int nap5 = (nWidth / 5), nmp5 = (nWidth - 5 * nap5);
	BYTE* pSrc24Line = (bSrc24Revert) ? (pSrc24 + (nHeight - 1) * nSrc24Pitch) : pSrc24;
	int nRealSrc24Pitch = (bSrc24Revert) ? -nSrc24Pitch : nSrc24Pitch;

	if (pSrcAlpha)      // 有 Alpha 输入
	{
		BYTE* pSrcAlphaLine = (bSrcAlphaRevert) ? (pSrcAlpha + (nHeight - 1) * nSrcAlphaPitch) : pSrcAlpha;
		int nRealSrcAlphaPitch = (bSrcAlphaRevert) ? -nSrcAlphaPitch : nSrcAlphaPitch;
		for(int i = 0; i < nHeight; ++i)
		{
			color24_a_2_color32(pDes32, pSrc24Line, pSrcAlphaLine, nWidth);
			pDes32 += nDesPitch;
			pSrc24Line += nRealSrc24Pitch;
			pSrcAlphaLine += nRealSrcAlphaPitch;
		}
	}
	else    // 无 Alpha 输入则直接填充 0xff
	{
		for (int i=0; i<nHeight; i++)
		{
			color24_to_color32(pDes32, pSrc24Line, nap5, nmp5);
			pSrc24Line += nRealSrc24Pitch;
			pDes32 += nDesPitch;
		}
	}
}

#define TPSSE_NEW(nSize, TYPE)	(##TYPE*)_mm_malloc(sizeof(##TYPE) * (nSize), 16)
#define TPSSE_FREE(p)			_mm_free((p))

// 翻转
BOOL TP_OverTurnSelf(IN OUT void* pBuf,    // 内存
					 IN int nPitch,        // 一行字节宽度（四字节对齐）
					 IN int nHeight)       // 高度
{
	assert(pBuf != NULL);

	if (pBuf == NULL || nPitch < 0 || nHeight < 0)
		return FALSE;

	int nHalfHeight = (nHeight >> 1);
	BYTE* pTop = (BYTE*)pBuf, *pBottom = pTop + (nHeight - 1) * nPitch;
	int na16 = (nPitch >> 4), nm16 = (nPitch & 0x0f);
	BYTE* pTmpLine = TPSSE_NEW(nPitch, BYTE);

	for (int i=0; i<nHalfHeight; i++)
	{
		memcpy_line(pTmpLine, pTop, na16, nm16);
		memcpy_line(pTop, pBottom, na16, nm16);
		memcpy_line(pBottom, pTmpLine, na16, nm16);
		pTop += nPitch;
		pBottom -= nPitch;
	}
	TPSSE_FREE(pTmpLine);

	return TRUE;
}