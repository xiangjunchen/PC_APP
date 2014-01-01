// SG: GIF pSGImageDescriptor->pPrivateData 使用
#include "stdafx.h"
#include "TPImgInterface.h"
#include "gif.h"
#include "TPAVTgaFile.h"

#ifdef _M_X64
#pragma comment (lib,"gif64.lib")
#else
#if _MSC_VER >= 1500
#pragma comment (lib,"gif2008.lib")
#else
#pragma comment (lib,"gif2003.lib")
#endif
#endif

#ifdef _M_X64
#pragma comment (lib,"RGBCluster64.lib")
#else
#pragma comment (lib,"RGBCluster32.lib")
#endif

// GIF图像使用带缓冲的文件读写函数，减少磁盘IO，目前，NS中只取第一帧 add by wgf@21010-11-15
BOOL SGBufferReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	CSGBufferFile *pBufferFile = (CSGBufferFile *)hFile;
	return pBufferFile->SGReadFile(pBufferFile->m_hFile, lpBuffer, dwNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

BOOL SGBufferWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	CSGBufferFile *pBufferFile = (CSGBufferFile *)hFile;
	return pBufferFile->SGWriteFile(pBufferFile->m_hFile, lpBuffer, dwNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}



// 加速GIF帧数目探测速度 begin
BOOL _SGGetGifFrameNum(CSGBufferFile *pBufferFile, WORD *pwFirstFrameDelayTime, int *piFrameNum)
{
	if(pBufferFile == NULL || pwFirstFrameDelayTime == NULL || piFrameNum == NULL)
	{
		return FALSE;
	} 

	int iGifFrameNum = 0; // 整个GIF图像含有的帧数目
	*pwFirstFrameDelayTime = 1;
	*piFrameNum = 0;

	LARGE_INTEGER llBeginMovePos;
	llBeginMovePos.QuadPart = 6;
	pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_BEGIN); // 移到到LSD开始位置
	// 读取LSD
	BYTE byteLSD[8];
	DWORD dwRealReadBytes = 0;
	pBufferFile->SGReadFile(pBufferFile->m_hFile, byteLSD, 7, &dwRealReadBytes, NULL);
	int iGCTSize = 0;
	if((byteLSD[4] & 0x80) != 0x00)
	{
		iGCTSize = 1;
		iGCTSize <<= ((byteLSD[4] & 0x07) + 1);
	}
	// 跳过全局颜色表
	LARGE_INTEGER llCurrentFilePointer;
	llCurrentFilePointer.QuadPart = 0;
	if(iGCTSize != 0)
	{
		llBeginMovePos.QuadPart = iGCTSize * 3;
		pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, &llCurrentFilePointer, FILE_CURRENT);
	}

	BOOL bFirstFrame = TRUE; // 第一帧数据，需要保存延迟时间
	// 循环读取GIF数据块
	while(true)
	{
		BYTE byteBlockType = 0x00;
		pBufferFile->SGReadFile(pBufferFile->m_hFile, &byteBlockType, 1, &dwRealReadBytes, NULL);
		if(dwRealReadBytes == 0)
		{
			*piFrameNum = iGifFrameNum;
			return TRUE;
		}

		switch(byteBlockType)
		{
		case 0x2c: // 图像描述
			{
				// 读取描述信息，判断是否有局部颜色表
				llBeginMovePos.QuadPart = 8;
				pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT);
				BYTE byteImageDescriptor = 0x00;
				pBufferFile->SGReadFile(pBufferFile->m_hFile, &byteImageDescriptor, 1, &dwRealReadBytes, NULL);
				if(dwRealReadBytes != 1)
				{
					*piFrameNum = iGifFrameNum;
					return TRUE;
				}

				// 跳过局部颜色表
				if((byteImageDescriptor & 0x80) != 0x00)
				{
					int iLCTSize = 1;
					iLCTSize <<= (byteImageDescriptor & 0x07);
					llBeginMovePos.QuadPart = iLCTSize * 3;
					pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // 跳过局部颜色表
				}

				// 跳过图像数据
				llBeginMovePos.QuadPart = 1;
				pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // 跳过LZW编码长度
				while(true)
				{
					BYTE byteBlockSize = 0x00;
					pBufferFile->SGReadFile(pBufferFile->m_hFile, &byteBlockSize, 1, &dwRealReadBytes, NULL);
					if(byteBlockSize == 0x00)
					{
						// 图像数据块结束标示
						break;
					}
					llBeginMovePos.QuadPart = byteBlockSize;
					pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // 跳过此块数据
				}
				++iGifFrameNum;
			}
			break;
		case 0x21: // 扩展块
			{
				BYTE byteExtensionType = 0x00; // 扩展块类型
				pBufferFile->SGReadFile(pBufferFile->m_hFile, &byteExtensionType, 1, &dwRealReadBytes, NULL);
				if(dwRealReadBytes == 0)
				{
					*piFrameNum = iGifFrameNum;
					return TRUE;
				}

				switch(byteExtensionType)
				{
				case 0xf9: // 图像控制块
					if(bFirstFrame == TRUE)
					{
						llBeginMovePos.QuadPart = 2;
						pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // 跳过此块数据
						pBufferFile->SGReadFile(pBufferFile->m_hFile, pwFirstFrameDelayTime, 2, &dwRealReadBytes, NULL);
						llBeginMovePos.QuadPart = 2;
						pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // 跳过此块数据
						bFirstFrame = FALSE;
						break;	
					}
				case 0xfe: // 注释扩展块

				case 0x01: // 文本图形扩展块

				case 0xff: // 应用程序扩展块

				default:
					{
						// 这些扩展块不识别，跳过即可
						while(true)
						{
							BYTE byteBlockSize = 0x00;
							pBufferFile->SGReadFile(pBufferFile->m_hFile, &byteBlockSize, 1, &dwRealReadBytes, NULL);
							if(byteBlockSize == 0x00)
							{
								// 扩展数据块结束标示
								break;
							}
							llBeginMovePos.QuadPart = byteBlockSize;
							pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // 跳过此块数据
						}
					}
					break;
				}
			}
			break;
		default:
			{
				*piFrameNum = iGifFrameNum;
				return TRUE;
			}	
		}
	}
	return TRUE;
}
// 加速GIF帧数目探测速度 end
// 将调色板数据转换为BGRA数据，返回的BGRA数据内存是此函数申请的，调用者负责释放
BYTE *_SGPaletteImage2BGRA(LPNXGOPGifFrame pNXGOPGifFrame)
{
	if(pNXGOPGifFrame == NULL 
		|| pNXGOPGifFrame->iFrameImageHeight <= 0 
		|| pNXGOPGifFrame-> iFrameImageWidth <= 0
		|| pNXGOPGifFrame->pFrameData == NULL)
	{
		return NULL;
	}

	BYTE *pBGRA = new BYTE[pNXGOPGifFrame->iFrameImageWidth * pNXGOPGifFrame->iFrameImageHeight * 4];

	BYTE *pPaletteImageTrv = pNXGOPGifFrame->pFrameData;
	BYTE *pBGRATrv = pBGRA;

	for(int i = 0; i < pNXGOPGifFrame->iFrameImageHeight; ++i)
	{
		for(int j = 0; j < pNXGOPGifFrame->iFrameImageWidth; ++j)
		{
			pBGRATrv[0] = pNXGOPGifFrame->framePalette[(unsigned char)pPaletteImageTrv[0]].Blue;
			pBGRATrv[1] = pNXGOPGifFrame->framePalette[(unsigned char)pPaletteImageTrv[0]].Green;
			pBGRATrv[2] = pNXGOPGifFrame->framePalette[(unsigned char)pPaletteImageTrv[0]].Red;
			pBGRATrv[3] = 0xff;

			++pPaletteImageTrv;
			pBGRATrv += 4;
		}
	}

	return pBGRA;
}
/*********************************************************************************************************
*	判断指定的帧号在 GOP 内位置，如果 GOPn <= iAimFrameIndex < GOPn_1 则返回 GOPn			*
*	如果目前的 GOP 不能包含 iAimFrameIndex，则返回 -1，即需要扩充 GOP										*
*********************************************************************************************************/
int IndexInGOP(LPNXGOPGifFrame pNXGOPGifFrame, int iGOPSize, int iAimFrameIndex)
{
	// 使用折半查找
	int iBeginIndex = 0;
	int iEndIndex = iGOPSize - 1;
	int iMidIndex = 0;

	if(pNXGOPGifFrame == NULL || pNXGOPGifFrame[iEndIndex].iFrameIndex < iAimFrameIndex || iGOPSize <= 0)
	{
		return -1; // 需要扩充 GOP
	}

	while(iBeginIndex <= iEndIndex)
	{
		iMidIndex = (iBeginIndex + iEndIndex) / 2;
		if(pNXGOPGifFrame[iMidIndex].iFrameIndex == iAimFrameIndex)
		{
			// 要找的帧时关键帧
			return iMidIndex;
		}
		else if(pNXGOPGifFrame[iMidIndex].iFrameIndex < iAimFrameIndex)
		{
			iBeginIndex = iMidIndex + 1;
		}
		else
		{
			iEndIndex = iMidIndex - 1;
		}
	}
	return iBeginIndex < iEndIndex ? iBeginIndex : iEndIndex; // 需要从此帧开始从文件中读取直到取到目标帧
}

BOOL _SGCopyGOPGifFrame(LPNXGOPGifFrame pSrc, LPNXGOPGifFrame pDest)
{
	if(pSrc == NULL || pDest == NULL 
		|| pSrc->pFrameData == NULL
		|| pDest->pFrameData == NULL)
	{
		return FALSE;
	}
	TP_MemcpyLine(pDest->framePalette, pSrc->framePalette, 256 * sizeof(SGPalette));
	pDest->iFrameImageHeight = pSrc->iFrameImageHeight;
	pDest->iFrameImageWidth= pSrc->iFrameImageWidth;
	pDest->iFrameIndex= pSrc->iFrameIndex;
	pDest->llFrameCursor = pSrc->llFrameCursor;
	pDest->usFrameDelayTime = pSrc->usFrameDelayTime;
	TP_MemcpyLine(pDest->pFrameData, pSrc->pFrameData, pSrc->iFrameImageHeight * pSrc->iFrameImageWidth);
	return TRUE;
}
/******************************************************************************************************
*	扩充 GOP，iEndFrameNum 表示扩充的最后一帧帧号，相对于 GIF 文件。									*
*	扩充过程中，需要将目的帧数据保存在 stuNXPreGifFrame 结构中，以提高效率。						*
******************************************************************************************************/
BOOL _SGExternGOP(SGGIFFileData* pSGGifData, int iEndFrameNum, int iAimFrameIndex)
{
	if(NULL == pSGGifData || pSGGifData->pGifFile == NULL)
		return FALSE;

	SGFileType* GifFile = pSGGifData->pGifFile;

	static int  InterlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
	static int InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */
	SGRecordType eRecordType;
	SGWord  Width;
	SGWord  Height;
	unsigned char ucTransparentColorIndex = 0; // 透明色索引
	bool bUseTransparentColorIndex = false; // 此帧图像是否使用透明索引

	if(pSGGifData->pNXGOPGifFrame == NULL)
	{
		// 第一次调用，申请GOP内存
		int iGOPNum = pSGGifData->iFrameNum / NX_GIF_GOP_SIZE + 1;
		if(pSGGifData->iFrameNum % NX_GIF_GOP_SIZE != 0)
		{
			++iGOPNum; // 保存最后一帧
		}

		pSGGifData->pNXGOPGifFrame = new NXGOPGifFrame[iGOPNum];
		pSGGifData->iAlreadyGetGOPNum = 0;
		pSGGifData->iNXGOPGifFrameNum = iGOPNum;
	}

	int iLastGOPIndex = pSGGifData->iAlreadyGetGOPNum - 1; // 目前GOP中最后帧

	NXGOPGifFrame stuNXTempPreGifFrame; // 保存生成GOP帧数据时的前一帧数据

	int iBeginFrameIndex = -1;

	// 将文件指针移动到上一帧图像读取完毕时的位置
	if(iLastGOPIndex >= 0)
	{
		// 非第一次调用，使用已有的GOP帧数据作为前一帧数据
		pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, pSGGifData->pNXGOPGifFrame[iLastGOPIndex].llFrameCursor, NULL, FILE_BEGIN);
		iBeginFrameIndex = pSGGifData->pNXGOPGifFrame[iLastGOPIndex].iFrameIndex; // 刚读取的帧号

		if(stuNXTempPreGifFrame.pFrameData == NULL)
		{
			stuNXTempPreGifFrame.pFrameData 
				= new BYTE[pSGGifData->pNXGOPGifFrame[iLastGOPIndex].iFrameImageWidth 
				* pSGGifData->pNXGOPGifFrame[iLastGOPIndex].iFrameImageHeight];
		}
		
		// 有了合适的空间，直接拷贝
		if(_SGCopyGOPGifFrame(pSGGifData->pNXGOPGifFrame + iLastGOPIndex, &stuNXTempPreGifFrame) == FALSE)
		{
			return FALSE;
		}
	}
	else
	{
		// 第一次调用，没有前一帧数据
	}
	

	BYTE *pImage = new BYTE[GifFile->SWidth * GifFile->SHeight]; // 记录刚读取的调色板数据

	do {
		if (DSGGetRecordType(GifFile, &eRecordType) == GIF_ERROR) {
			return FALSE;
		}
		switch (eRecordType) {
			case IMAGE_DESC_RECORD_TYPE: // 图像描述符
				{
					if (DSGGetImageDesc(GifFile) == GIF_ERROR) 
					{
						if(pImage != NULL)
						{
							delete []pImage;
							pImage = NULL;
						}
						return FALSE;
					}
					Width = GifFile->Image.Width; // 图像宽高
					Height = GifFile->Image.Height;

					BYTE *pTrv = pImage;
					if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
						GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) { // 图像越界，一般不会发生
							if(pImage != NULL)
							{
								delete []pImage;
								pImage = NULL;
							}
							return FALSE;
					}
					if (GifFile->Image.Interlace) { // 交叉编码
						for (int i = 0; i < 4; i++){
							pTrv = pImage + Width * InterlacedOffset[i];
							for (int j =InterlacedOffset[i]; j < Height; j += InterlacedJumps[i]) {
								if (DSGGetLine(GifFile, pTrv, Width) == GIF_ERROR) {
									if(pImage != NULL)
									{
										delete []pImage;
										pImage = NULL;
									}
									return FALSE;
								}
								pTrv += Width * InterlacedJumps[i];
							}
						}
					}
					else { // 顺序编码
						for (int i = 0; i < Height; i++) {
							if (DSGGetLine(GifFile, pTrv, Width) == GIF_ERROR) {
								if(pImage != NULL)
								{
									delete []pImage;
									pImage = NULL;
								}

								return FALSE;
							}
							pTrv += Width;
						}
					}

					// 进行调色板图像叠加
					BYTE *pPaletteTrv = pImage;
					int nEveryRowDistance = (GifFile->SWidth - GifFile->Image.Width); // 图像此行行尾与下一行行首的距离

					// 将此帧数据叠加到前一帧数据
					// 将透明色索引值换为对应的前一帧索引图像数据
					// 下面紧接着的 if else 是使得 stuNXTempPreGifFrame 为当前读取的帧数据
					int iPixelNum = GifFile->SWidth * GifFile->SHeight;
					if(stuNXTempPreGifFrame.pFrameData == NULL)
					{
						// 第一次调用，拷贝此帧数据到前一帧
						// 【注】这里需要考虑第一帧图像填不满 LSD
						stuNXTempPreGifFrame.pFrameData = new BYTE[iPixelNum];
						TP_MemsetChar(stuNXTempPreGifFrame.pFrameData, 0, iPixelNum); // 填充背景色
						BYTE *pSrcTrv = pImage;
						BYTE *pDestTrv = stuNXTempPreGifFrame.pFrameData + (GifFile->Image.Top * GifFile->SWidth + GifFile->Image.Left);
						for(int i = 0; i < GifFile->Image.Height; ++i)
						{
								TP_MemcpyLine(pDestTrv, pSrcTrv, GifFile->Image.Width);
								pSrcTrv += GifFile->Image.Width;
								pDestTrv += GifFile->SWidth;
						}
						SGColorMap *pColorMap = GifFile->Image.ColorMap 
							? GifFile->Image.ColorMap : GifFile->SColorMap;
						TP_MemcpyLine(stuNXTempPreGifFrame.framePalette, pColorMap->Colors, sizeof(SGPalette) * pColorMap->ColorCount); // 调色板
						stuNXTempPreGifFrame.iFrameImageHeight = GifFile->SHeight; // 宽高
						stuNXTempPreGifFrame.iFrameImageWidth = GifFile->SWidth;
					}
					else
					{
						// 对图像进行叠加
						BYTE *pSrcTrv = pImage;
						BYTE *pDestTRv = stuNXTempPreGifFrame.pFrameData + GifFile->Image.Top * GifFile->SWidth + GifFile->Image.Left; // 此帧索引图像在前一帧的起始位置
						for(int i = 0; i < Height; ++i)
						{
							for(int j = 0; j < Width; ++j)
							{
								if(*pSrcTrv != ucTransparentColorIndex && bUseTransparentColorIndex == true)
								{
									// 此像素叠加到前一帧相应位置
									*pDestTRv = *pSrcTrv;
								}
								++pDestTRv;
								++pSrcTrv;
							}
							pDestTRv += nEveryRowDistance; // 下一行起始位置
						}
					}

					// 保存此帧的调色板等
					if(GifFile->Image.ColorMap != NULL)
					{
						// 此帧使用单独的调色板，需要进行调色板叠加
					}

					++iBeginFrameIndex;

					stuNXTempPreGifFrame.iFrameIndex = iBeginFrameIndex; // 保存帧号
					LARGE_INTEGER llCurrentFilePointer;
					LARGE_INTEGER llMoveDistance;
					llMoveDistance.QuadPart = 0;
					pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, llMoveDistance, &llCurrentFilePointer, FILE_CURRENT); // 保存当前文件指针位置
					stuNXTempPreGifFrame.llFrameCursor = llCurrentFilePointer; // 保存文件位置指针

					// 上面得到了此帧数据，判断是否保存为目的帧或保存为关键帧
					if(iBeginFrameIndex == iAimFrameIndex)
					{
						// 此帧是需要的目的帧
						if(pSGGifData->stuNXPreGifFrame.pFrameData != NULL)
						{
							if(pSGGifData->stuNXPreGifFrame.iFrameImageHeight != stuNXTempPreGifFrame.iFrameImageHeight
								|| pSGGifData->stuNXPreGifFrame.iFrameImageWidth != stuNXTempPreGifFrame.iFrameImageWidth)
							{
								// 大小不相同，释放然后申请
								delete []pSGGifData->stuNXPreGifFrame.pFrameData;
								pSGGifData->stuNXPreGifFrame.pFrameData = NULL;
							}
						}

						if(pSGGifData->stuNXPreGifFrame.pFrameData == NULL)
						{
							pSGGifData->stuNXPreGifFrame.pFrameData 
								= new BYTE[stuNXTempPreGifFrame.iFrameImageWidth * stuNXTempPreGifFrame.iFrameImageHeight];
						}

						_SGCopyGOPGifFrame(&stuNXTempPreGifFrame, &pSGGifData->stuNXPreGifFrame); // 保存目的帧
					}

					if(iBeginFrameIndex % NX_GIF_GOP_SIZE == 0 || iBeginFrameIndex == pSGGifData->iFrameNum - 1)
					{
						// 刚读取的帧是关键帧 或 是最后一帧，保存在 GOP 中
						pSGGifData->pNXGOPGifFrame[pSGGifData->iAlreadyGetGOPNum].pFrameData 
								= new BYTE[stuNXTempPreGifFrame.iFrameImageWidth * stuNXTempPreGifFrame.iFrameImageHeight];

						// 保存关键帧
						_SGCopyGOPGifFrame(&stuNXTempPreGifFrame, &pSGGifData->pNXGOPGifFrame[pSGGifData->iAlreadyGetGOPNum]);
						++pSGGifData->iAlreadyGetGOPNum;
					}

					// 判断是否已经完成此次扩充任务
					if(iBeginFrameIndex >= iEndFrameNum)
					{
						// 完成清理工作
						if(pImage != NULL)
						{
							delete []pImage;
						}
						if(stuNXTempPreGifFrame.pFrameData != NULL)
						{
							delete []stuNXTempPreGifFrame.pFrameData;
						}

						return TRUE;
					}
					break;
				}
			case EXTENSION_RECORD_TYPE:
				{
					int ExtCode;
					SGByteType *Extension;
					if (DSGGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR)
					{
						return NULL;
					}
					// 寻找图像控制块标签 0xf9
					if(ExtCode == 0x000000f9)
					{
						// 此块记录紧跟着的图像信息，如延迟，透明色索引等
						SGByteType byteBlockSize = Extension[0]; // 此块大小
						bUseTransparentColorIndex = Extension[1] & 0x01 ? true : false;
						if(bUseTransparentColorIndex == true)
						{
							// 设置透明色索引
							ucTransparentColorIndex = (unsigned char)Extension[4];
						}
						stuNXTempPreGifFrame.usFrameDelayTime = *(unsigned short *)(&Extension[2]); // 紧跟图像延迟时间，单位为 1/100s
					}
					while (Extension != NULL)
					{
						if (DSGGetExtensionNext(GifFile, &Extension) == GIF_ERROR)
						{
							return FALSE;
						}	
					}
					break;
				}
			case TERMINATE_RECORD_TYPE:
				if(pImage != NULL)
				{
					delete []pImage;
					pImage = NULL;
				}
				return FALSE;
			default:		   
				if(pImage)
				{
					delete []pImage;
					pImage = NULL;
				}
				return FALSE;
		}
	} while (eRecordType != TERMINATE_RECORD_TYPE);
	if(pImage)
	{
		delete []pImage;
		pImage = NULL;
	}
	return FALSE;
}
/*******************************************************************************************************
*	将指定的GOP帧作为前一帧，从文件中读取目的帧数据，存入stuNXPreGifFrame结构。				*
*******************************************************************************************************/
BOOL _SGReadPaletteImageByGOP(SGGIFFileData* pSGGifData, int iGOPIndex, int iAimFrameIndex)
{
	if(NULL == pSGGifData || pSGGifData->pGifFile == NULL)
		return NULL;

	SGFileType* GifFile = pSGGifData->pGifFile;

	static int  InterlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
	static int InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */
	SGRecordType eRecordType;
	SGWord  Width;
	SGWord  Height;
	unsigned char ucTransparentColorIndex = 0; // 透明色索引
	bool bUseTransparentColorIndex = false; // 此帧图像是否使用透明索引

	// 将文件指针移动到上一帧图像读取完毕时的位置
	int iBeginFrameIndex = -1;
	BOOL bUsePreFrameData = (iAimFrameIndex == (pSGGifData->stuNXPreGifFrame.iFrameIndex + 1) ? TRUE : FALSE);
	if(bUsePreFrameData)
	{
		// 是连续取帧，不必从GOP关键帧开始读
		pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, pSGGifData->stuNXPreGifFrame.llFrameCursor, NULL, FILE_BEGIN);
		iBeginFrameIndex = pSGGifData->stuNXPreGifFrame.iFrameIndex; // 刚读取的帧号
	}
	else
	{
		pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, pSGGifData->pNXGOPGifFrame[iGOPIndex].llFrameCursor, NULL, FILE_BEGIN);
		iBeginFrameIndex = pSGGifData->pNXGOPGifFrame[iGOPIndex].iFrameIndex; // 刚读取的帧号
	}

	// 设置前一帧调色板图像数据
	if(pSGGifData->stuNXPreGifFrame.pFrameData != NULL)
	{
		if(bUsePreFrameData == FALSE 
			&& (pSGGifData->stuNXPreGifFrame.iFrameImageHeight != pSGGifData->pNXGOPGifFrame[iGOPIndex].iFrameImageHeight
			|| pSGGifData->stuNXPreGifFrame.iFrameImageWidth != pSGGifData->pNXGOPGifFrame[iGOPIndex].iFrameImageWidth)
			)
		{
			// 大小不相同，释放然后申请
			delete []pSGGifData->stuNXPreGifFrame.pFrameData;
			pSGGifData->stuNXPreGifFrame.pFrameData = NULL;
		}
	}

	if(pSGGifData->stuNXPreGifFrame.pFrameData == NULL)
	{
		pSGGifData->stuNXPreGifFrame.pFrameData 
			= new BYTE[pSGGifData->pNXGOPGifFrame[iGOPIndex].iFrameImageWidth 
			* pSGGifData->pNXGOPGifFrame[iGOPIndex].iFrameImageHeight];
	}

	if(bUsePreFrameData == FALSE)
	{
		// 有了合适的空间，直接拷贝
		if(_SGCopyGOPGifFrame(pSGGifData->pNXGOPGifFrame + iGOPIndex, &pSGGifData->stuNXPreGifFrame) == FALSE)
		{
			return FALSE;
		}
	}
	
	BYTE *pImage = new BYTE[GifFile->SWidth * GifFile->SHeight]; // 记录刚读取的调色板数据

	do {
		if (DSGGetRecordType(GifFile, &eRecordType) == GIF_ERROR) {
			return NULL;
		}
		switch (eRecordType) {
			case IMAGE_DESC_RECORD_TYPE: // 图像描述符
				{
					if (DSGGetImageDesc(GifFile) == GIF_ERROR) 
					{
						if(pImage != NULL)
						{
							delete []pImage;
							pImage = NULL;
						}
						return NULL;
					}
					Width = GifFile->Image.Width; // 图像宽高
					Height = GifFile->Image.Height;

					BYTE *pTrv = pImage;
					if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
						GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) { // 图像越界，一般不会发生
							if(pImage != NULL)
							{
								delete []pImage;
								pImage = NULL;
							}
							return NULL;
					}
					if (GifFile->Image.Interlace) { // 交叉编码
						for (int i = 0; i < 4; i++){
							pTrv = pImage + Width * InterlacedOffset[i];
							for (int j =InterlacedOffset[i]; j < Height; j += InterlacedJumps[i]) {
								if (DSGGetLine(GifFile, pTrv, Width) == GIF_ERROR) {
									if(pImage != NULL)
									{
										delete []pImage;
										pImage = NULL;
									}
									return NULL;
								}
								pTrv += Width * InterlacedJumps[i];
							}
						}
					}
					else { // 顺序编码
						for (int i = 0; i < Height; i++) {
							if (DSGGetLine(GifFile, pTrv, Width) == GIF_ERROR) {
								if(pImage != NULL)
								{
									delete []pImage;
									pImage = NULL;
								}

								return NULL;
							}
							pTrv += Width;
						}
					}

					// 进行调色板图像叠加
					BYTE *pPaletteTrv = pImage;
					int nEveryRowDistance = (GifFile->SWidth - GifFile->Image.Width); // 图像此行行尾与下一行行首的距离

					// 将此帧数据叠加到前一帧数据
					// 将透明色索引值换为对应的前一帧索引图像数据
					int iPixelNum = GifFile->SWidth * GifFile->SHeight;
					BYTE *pSrcTrv = pImage;
					BYTE *pDestTRv = pSGGifData->stuNXPreGifFrame.pFrameData + GifFile->Image.Top * GifFile->SWidth + GifFile->Image.Left; // 此帧索引图像在前一帧的起始位置
					for(int i = 0; i < Height; ++i)
					{
						for(int j = 0; j < Width; ++j)
						{
							if(*pSrcTrv != ucTransparentColorIndex && bUseTransparentColorIndex == true)
							{
								// 此像素叠加到前一帧相应位置
								*pDestTRv = *pSrcTrv;
							}
							++pDestTRv;
							++pSrcTrv;
						}
						pDestTRv += nEveryRowDistance; // 下一行起始位置
					}
					// 保存此帧的调色板等
					if(GifFile->Image.ColorMap != NULL)
					{
						// 此帧使用单独的调色板，需要进行调色板叠加
					}

					++iBeginFrameIndex;
					if(iBeginFrameIndex == iAimFrameIndex)
					{
						// 是要取的图像帧
						pSGGifData->stuNXPreGifFrame.iFrameIndex = iAimFrameIndex; // 保存帧号
						LARGE_INTEGER llCurrentFilePointer;
						LARGE_INTEGER llMoveDistance;
						llMoveDistance.QuadPart = 0;
						pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, llMoveDistance, &llCurrentFilePointer, FILE_CURRENT); // 保存当前文件指针位置
						pSGGifData->stuNXPreGifFrame.llFrameCursor = llCurrentFilePointer; // 保存文件位置指针
						if(pImage != NULL)
						{
							delete []pImage;
						}
						return TRUE;
					}
					break;
				}
			case EXTENSION_RECORD_TYPE:
				{
					int ExtCode;
					SGByteType *Extension;
					if (DSGGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR)
					{
						return NULL;
					}
					// 寻找图像控制块标签 0xf9
					if(ExtCode == 0x000000f9)
					{
						// 此块记录紧跟着的图像信息，如延迟，透明色索引等
						SGByteType byteBlockSize = Extension[0]; // 此块大小
						bUseTransparentColorIndex = Extension[1] & 0x01 ? true : false;
						if(bUseTransparentColorIndex == true)
						{
							// 设置透明色索引
							ucTransparentColorIndex = (unsigned char)Extension[4];
						}
						pSGGifData->stuNXPreGifFrame.usFrameDelayTime = *(unsigned short *)(&Extension[2]); // 紧跟图像延迟时间，单位为 1/100s
					}
					while (Extension != NULL)
					{
						if (DSGGetExtensionNext(GifFile, &Extension) == GIF_ERROR)
						{
							return NULL;
						}	
					}
					break;
				}
			case TERMINATE_RECORD_TYPE:
				if(pImage != NULL)
				{
					delete []pImage;
					pImage = NULL;
				}
				return FALSE;
			default:		   
				if(pImage)
				{
					delete []pImage;
					pImage = NULL;
				}
				return FALSE;
		}
	} while (eRecordType != TERMINATE_RECORD_TYPE);
	if(pImage)
	{
		delete []pImage;
		pImage = NULL;
	}
	return FALSE;
}
/****************************************************************************************************************************************************
*	根据指定的帧号，读取相应的帧图像BGRA数据。																																		*
*	无论 Play 还是 Seek，都使用这个函数，此函数内进行 GOP关键帧读取等操作。																							*
****************************************************************************************************************************************************/
BYTE *SGGetGifImage(SGGIFFileData* pSGGifData, int nImageIndex, int *parWidth, int *parHeight, unsigned short *parDelayTime)
{
	if(NULL == pSGGifData || pSGGifData->pGifFile == NULL)
		return NULL;

	// 判断此帧是否为上次刚调用的
	if(nImageIndex == pSGGifData->stuNXPreGifFrame.iFrameIndex)
	{
		// 连续调用同一帧，直接将此帧调色板数据转换为BGRA数据
		*parWidth = pSGGifData->stuNXPreGifFrame.iFrameImageWidth;
		*parHeight = pSGGifData->stuNXPreGifFrame.iFrameImageHeight;
		*parDelayTime = pSGGifData->stuNXPreGifFrame.usFrameDelayTime;

		return _SGPaletteImage2BGRA(&pSGGifData->stuNXPreGifFrame);
	}

	// 判断此帧是否在目前 GOP 关键帧内
	int iIndexInGOP = IndexInGOP(pSGGifData->pNXGOPGifFrame, pSGGifData->iAlreadyGetGOPNum, nImageIndex);
	if(iIndexInGOP == -1)
	{
		// 扩充 GOP，在扩充过程中，得到目的帧图像
		// 确定扩充多少帧，原则是扩充的帧一定要包括目的帧
		int iEndFrameNum = 0;
		if(pSGGifData->pNXGOPGifFrame == NULL)
		{
			iEndFrameNum = NX_GET_GOP_INFO_NUM_ONCE;
		}
		else
		{
			iEndFrameNum = nImageIndex
				> (NX_GET_GOP_INFO_NUM_ONCE + pSGGifData->pNXGOPGifFrame[pSGGifData->iAlreadyGetGOPNum - 1].iFrameIndex)
				? nImageIndex
				: (NX_GET_GOP_INFO_NUM_ONCE + pSGGifData->pNXGOPGifFrame[pSGGifData->iAlreadyGetGOPNum - 1].iFrameIndex);
		}
		
		iEndFrameNum = (iEndFrameNum > pSGGifData->iFrameNum - 1) ? pSGGifData->iFrameNum - 1 : iEndFrameNum; // 保护，防止超过最后一帧
		if(_SGExternGOP(pSGGifData, iEndFrameNum, nImageIndex) == FALSE)
		{
			return NULL;
		}

		// 正常扩充GOP，将目的帧数据保存在记录刚读取的帧结构里，直接转换即可
		*parWidth = pSGGifData->stuNXPreGifFrame.iFrameImageWidth;
		*parHeight = pSGGifData->stuNXPreGifFrame.iFrameImageHeight;
		*parDelayTime = pSGGifData->stuNXPreGifFrame.usFrameDelayTime;

		return _SGPaletteImage2BGRA(&pSGGifData->stuNXPreGifFrame);
	}
	else
	{
		if(pSGGifData->pNXGOPGifFrame[iIndexInGOP].iFrameIndex == nImageIndex)
		{
			// 将要取的帧恰巧是关键帧
			*parWidth = pSGGifData->pNXGOPGifFrame[iIndexInGOP].iFrameImageWidth;
			*parHeight = pSGGifData->pNXGOPGifFrame[iIndexInGOP].iFrameImageHeight;
			*parDelayTime = pSGGifData->pNXGOPGifFrame[iIndexInGOP].usFrameDelayTime;

			return _SGPaletteImage2BGRA(pSGGifData->pNXGOPGifFrame + iIndexInGOP);
		}
		else
		{
			// 从关键帧 iIndexInGOP 开始从文件中取帧
			if(_SGReadPaletteImageByGOP(pSGGifData, iIndexInGOP, nImageIndex) == FALSE)
			{
				return NULL;
			}
			else
			{
				// 正常取到帧数据，转换返回
				*parWidth = pSGGifData->stuNXPreGifFrame.iFrameImageWidth;
				*parHeight = pSGGifData->stuNXPreGifFrame.iFrameImageHeight;
				*parDelayTime = pSGGifData->stuNXPreGifFrame.usFrameDelayTime;

				return _SGPaletteImage2BGRA(&pSGGifData->stuNXPreGifFrame);
			}
		}
	}
}