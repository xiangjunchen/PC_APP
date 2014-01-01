// SG: GIF pSGImageDescriptor->pPrivateData ʹ��
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

// GIFͼ��ʹ�ô�������ļ���д���������ٴ���IO��Ŀǰ��NS��ֻȡ��һ֡ add by wgf@21010-11-15
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



// ����GIF֡��Ŀ̽���ٶ� begin
BOOL _SGGetGifFrameNum(CSGBufferFile *pBufferFile, WORD *pwFirstFrameDelayTime, int *piFrameNum)
{
	if(pBufferFile == NULL || pwFirstFrameDelayTime == NULL || piFrameNum == NULL)
	{
		return FALSE;
	} 

	int iGifFrameNum = 0; // ����GIFͼ���е�֡��Ŀ
	*pwFirstFrameDelayTime = 1;
	*piFrameNum = 0;

	LARGE_INTEGER llBeginMovePos;
	llBeginMovePos.QuadPart = 6;
	pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_BEGIN); // �Ƶ���LSD��ʼλ��
	// ��ȡLSD
	BYTE byteLSD[8];
	DWORD dwRealReadBytes = 0;
	pBufferFile->SGReadFile(pBufferFile->m_hFile, byteLSD, 7, &dwRealReadBytes, NULL);
	int iGCTSize = 0;
	if((byteLSD[4] & 0x80) != 0x00)
	{
		iGCTSize = 1;
		iGCTSize <<= ((byteLSD[4] & 0x07) + 1);
	}
	// ����ȫ����ɫ��
	LARGE_INTEGER llCurrentFilePointer;
	llCurrentFilePointer.QuadPart = 0;
	if(iGCTSize != 0)
	{
		llBeginMovePos.QuadPart = iGCTSize * 3;
		pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, &llCurrentFilePointer, FILE_CURRENT);
	}

	BOOL bFirstFrame = TRUE; // ��һ֡���ݣ���Ҫ�����ӳ�ʱ��
	// ѭ����ȡGIF���ݿ�
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
		case 0x2c: // ͼ������
			{
				// ��ȡ������Ϣ���ж��Ƿ��оֲ���ɫ��
				llBeginMovePos.QuadPart = 8;
				pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT);
				BYTE byteImageDescriptor = 0x00;
				pBufferFile->SGReadFile(pBufferFile->m_hFile, &byteImageDescriptor, 1, &dwRealReadBytes, NULL);
				if(dwRealReadBytes != 1)
				{
					*piFrameNum = iGifFrameNum;
					return TRUE;
				}

				// �����ֲ���ɫ��
				if((byteImageDescriptor & 0x80) != 0x00)
				{
					int iLCTSize = 1;
					iLCTSize <<= (byteImageDescriptor & 0x07);
					llBeginMovePos.QuadPart = iLCTSize * 3;
					pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // �����ֲ���ɫ��
				}

				// ����ͼ������
				llBeginMovePos.QuadPart = 1;
				pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // ����LZW���볤��
				while(true)
				{
					BYTE byteBlockSize = 0x00;
					pBufferFile->SGReadFile(pBufferFile->m_hFile, &byteBlockSize, 1, &dwRealReadBytes, NULL);
					if(byteBlockSize == 0x00)
					{
						// ͼ�����ݿ������ʾ
						break;
					}
					llBeginMovePos.QuadPart = byteBlockSize;
					pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // �����˿�����
				}
				++iGifFrameNum;
			}
			break;
		case 0x21: // ��չ��
			{
				BYTE byteExtensionType = 0x00; // ��չ������
				pBufferFile->SGReadFile(pBufferFile->m_hFile, &byteExtensionType, 1, &dwRealReadBytes, NULL);
				if(dwRealReadBytes == 0)
				{
					*piFrameNum = iGifFrameNum;
					return TRUE;
				}

				switch(byteExtensionType)
				{
				case 0xf9: // ͼ����ƿ�
					if(bFirstFrame == TRUE)
					{
						llBeginMovePos.QuadPart = 2;
						pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // �����˿�����
						pBufferFile->SGReadFile(pBufferFile->m_hFile, pwFirstFrameDelayTime, 2, &dwRealReadBytes, NULL);
						llBeginMovePos.QuadPart = 2;
						pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // �����˿�����
						bFirstFrame = FALSE;
						break;	
					}
				case 0xfe: // ע����չ��

				case 0x01: // �ı�ͼ����չ��

				case 0xff: // Ӧ�ó�����չ��

				default:
					{
						// ��Щ��չ�鲻ʶ����������
						while(true)
						{
							BYTE byteBlockSize = 0x00;
							pBufferFile->SGReadFile(pBufferFile->m_hFile, &byteBlockSize, 1, &dwRealReadBytes, NULL);
							if(byteBlockSize == 0x00)
							{
								// ��չ���ݿ������ʾ
								break;
							}
							llBeginMovePos.QuadPart = byteBlockSize;
							pBufferFile->SGSetFilePointerEx(pBufferFile->m_hFile, llBeginMovePos, NULL, FILE_CURRENT); // �����˿�����
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
// ����GIF֡��Ŀ̽���ٶ� end
// ����ɫ������ת��ΪBGRA���ݣ����ص�BGRA�����ڴ��Ǵ˺�������ģ������߸����ͷ�
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
*	�ж�ָ����֡���� GOP ��λ�ã���� GOPn <= iAimFrameIndex < GOPn_1 �򷵻� GOPn			*
*	���Ŀǰ�� GOP ���ܰ��� iAimFrameIndex���򷵻� -1������Ҫ���� GOP										*
*********************************************************************************************************/
int IndexInGOP(LPNXGOPGifFrame pNXGOPGifFrame, int iGOPSize, int iAimFrameIndex)
{
	// ʹ���۰����
	int iBeginIndex = 0;
	int iEndIndex = iGOPSize - 1;
	int iMidIndex = 0;

	if(pNXGOPGifFrame == NULL || pNXGOPGifFrame[iEndIndex].iFrameIndex < iAimFrameIndex || iGOPSize <= 0)
	{
		return -1; // ��Ҫ���� GOP
	}

	while(iBeginIndex <= iEndIndex)
	{
		iMidIndex = (iBeginIndex + iEndIndex) / 2;
		if(pNXGOPGifFrame[iMidIndex].iFrameIndex == iAimFrameIndex)
		{
			// Ҫ�ҵ�֡ʱ�ؼ�֡
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
	return iBeginIndex < iEndIndex ? iBeginIndex : iEndIndex; // ��Ҫ�Ӵ�֡��ʼ���ļ��ж�ȡֱ��ȡ��Ŀ��֡
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
*	���� GOP��iEndFrameNum ��ʾ��������һ֡֡�ţ������ GIF �ļ���									*
*	��������У���Ҫ��Ŀ��֡���ݱ����� stuNXPreGifFrame �ṹ�У������Ч�ʡ�						*
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
	unsigned char ucTransparentColorIndex = 0; // ͸��ɫ����
	bool bUseTransparentColorIndex = false; // ��֡ͼ���Ƿ�ʹ��͸������

	if(pSGGifData->pNXGOPGifFrame == NULL)
	{
		// ��һ�ε��ã�����GOP�ڴ�
		int iGOPNum = pSGGifData->iFrameNum / NX_GIF_GOP_SIZE + 1;
		if(pSGGifData->iFrameNum % NX_GIF_GOP_SIZE != 0)
		{
			++iGOPNum; // �������һ֡
		}

		pSGGifData->pNXGOPGifFrame = new NXGOPGifFrame[iGOPNum];
		pSGGifData->iAlreadyGetGOPNum = 0;
		pSGGifData->iNXGOPGifFrameNum = iGOPNum;
	}

	int iLastGOPIndex = pSGGifData->iAlreadyGetGOPNum - 1; // ĿǰGOP�����֡

	NXGOPGifFrame stuNXTempPreGifFrame; // ��������GOP֡����ʱ��ǰһ֡����

	int iBeginFrameIndex = -1;

	// ���ļ�ָ���ƶ�����һ֡ͼ���ȡ���ʱ��λ��
	if(iLastGOPIndex >= 0)
	{
		// �ǵ�һ�ε��ã�ʹ�����е�GOP֡������Ϊǰһ֡����
		pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, pSGGifData->pNXGOPGifFrame[iLastGOPIndex].llFrameCursor, NULL, FILE_BEGIN);
		iBeginFrameIndex = pSGGifData->pNXGOPGifFrame[iLastGOPIndex].iFrameIndex; // �ն�ȡ��֡��

		if(stuNXTempPreGifFrame.pFrameData == NULL)
		{
			stuNXTempPreGifFrame.pFrameData 
				= new BYTE[pSGGifData->pNXGOPGifFrame[iLastGOPIndex].iFrameImageWidth 
				* pSGGifData->pNXGOPGifFrame[iLastGOPIndex].iFrameImageHeight];
		}
		
		// ���˺��ʵĿռ䣬ֱ�ӿ���
		if(_SGCopyGOPGifFrame(pSGGifData->pNXGOPGifFrame + iLastGOPIndex, &stuNXTempPreGifFrame) == FALSE)
		{
			return FALSE;
		}
	}
	else
	{
		// ��һ�ε��ã�û��ǰһ֡����
	}
	

	BYTE *pImage = new BYTE[GifFile->SWidth * GifFile->SHeight]; // ��¼�ն�ȡ�ĵ�ɫ������

	do {
		if (DSGGetRecordType(GifFile, &eRecordType) == GIF_ERROR) {
			return FALSE;
		}
		switch (eRecordType) {
			case IMAGE_DESC_RECORD_TYPE: // ͼ��������
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
					Width = GifFile->Image.Width; // ͼ����
					Height = GifFile->Image.Height;

					BYTE *pTrv = pImage;
					if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
						GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) { // ͼ��Խ�磬һ�㲻�ᷢ��
							if(pImage != NULL)
							{
								delete []pImage;
								pImage = NULL;
							}
							return FALSE;
					}
					if (GifFile->Image.Interlace) { // �������
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
					else { // ˳�����
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

					// ���е�ɫ��ͼ�����
					BYTE *pPaletteTrv = pImage;
					int nEveryRowDistance = (GifFile->SWidth - GifFile->Image.Width); // ͼ�������β����һ�����׵ľ���

					// ����֡���ݵ��ӵ�ǰһ֡����
					// ��͸��ɫ����ֵ��Ϊ��Ӧ��ǰһ֡����ͼ������
					// ��������ŵ� if else ��ʹ�� stuNXTempPreGifFrame Ϊ��ǰ��ȡ��֡����
					int iPixelNum = GifFile->SWidth * GifFile->SHeight;
					if(stuNXTempPreGifFrame.pFrameData == NULL)
					{
						// ��һ�ε��ã�������֡���ݵ�ǰһ֡
						// ��ע��������Ҫ���ǵ�һ֡ͼ����� LSD
						stuNXTempPreGifFrame.pFrameData = new BYTE[iPixelNum];
						TP_MemsetChar(stuNXTempPreGifFrame.pFrameData, 0, iPixelNum); // ��䱳��ɫ
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
						TP_MemcpyLine(stuNXTempPreGifFrame.framePalette, pColorMap->Colors, sizeof(SGPalette) * pColorMap->ColorCount); // ��ɫ��
						stuNXTempPreGifFrame.iFrameImageHeight = GifFile->SHeight; // ���
						stuNXTempPreGifFrame.iFrameImageWidth = GifFile->SWidth;
					}
					else
					{
						// ��ͼ����е���
						BYTE *pSrcTrv = pImage;
						BYTE *pDestTRv = stuNXTempPreGifFrame.pFrameData + GifFile->Image.Top * GifFile->SWidth + GifFile->Image.Left; // ��֡����ͼ����ǰһ֡����ʼλ��
						for(int i = 0; i < Height; ++i)
						{
							for(int j = 0; j < Width; ++j)
							{
								if(*pSrcTrv != ucTransparentColorIndex && bUseTransparentColorIndex == true)
								{
									// �����ص��ӵ�ǰһ֡��Ӧλ��
									*pDestTRv = *pSrcTrv;
								}
								++pDestTRv;
								++pSrcTrv;
							}
							pDestTRv += nEveryRowDistance; // ��һ����ʼλ��
						}
					}

					// �����֡�ĵ�ɫ���
					if(GifFile->Image.ColorMap != NULL)
					{
						// ��֡ʹ�õ����ĵ�ɫ�壬��Ҫ���е�ɫ�����
					}

					++iBeginFrameIndex;

					stuNXTempPreGifFrame.iFrameIndex = iBeginFrameIndex; // ����֡��
					LARGE_INTEGER llCurrentFilePointer;
					LARGE_INTEGER llMoveDistance;
					llMoveDistance.QuadPart = 0;
					pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, llMoveDistance, &llCurrentFilePointer, FILE_CURRENT); // ���浱ǰ�ļ�ָ��λ��
					stuNXTempPreGifFrame.llFrameCursor = llCurrentFilePointer; // �����ļ�λ��ָ��

					// ����õ��˴�֡���ݣ��ж��Ƿ񱣴�ΪĿ��֡�򱣴�Ϊ�ؼ�֡
					if(iBeginFrameIndex == iAimFrameIndex)
					{
						// ��֡����Ҫ��Ŀ��֡
						if(pSGGifData->stuNXPreGifFrame.pFrameData != NULL)
						{
							if(pSGGifData->stuNXPreGifFrame.iFrameImageHeight != stuNXTempPreGifFrame.iFrameImageHeight
								|| pSGGifData->stuNXPreGifFrame.iFrameImageWidth != stuNXTempPreGifFrame.iFrameImageWidth)
							{
								// ��С����ͬ���ͷ�Ȼ������
								delete []pSGGifData->stuNXPreGifFrame.pFrameData;
								pSGGifData->stuNXPreGifFrame.pFrameData = NULL;
							}
						}

						if(pSGGifData->stuNXPreGifFrame.pFrameData == NULL)
						{
							pSGGifData->stuNXPreGifFrame.pFrameData 
								= new BYTE[stuNXTempPreGifFrame.iFrameImageWidth * stuNXTempPreGifFrame.iFrameImageHeight];
						}

						_SGCopyGOPGifFrame(&stuNXTempPreGifFrame, &pSGGifData->stuNXPreGifFrame); // ����Ŀ��֡
					}

					if(iBeginFrameIndex % NX_GIF_GOP_SIZE == 0 || iBeginFrameIndex == pSGGifData->iFrameNum - 1)
					{
						// �ն�ȡ��֡�ǹؼ�֡ �� �����һ֡�������� GOP ��
						pSGGifData->pNXGOPGifFrame[pSGGifData->iAlreadyGetGOPNum].pFrameData 
								= new BYTE[stuNXTempPreGifFrame.iFrameImageWidth * stuNXTempPreGifFrame.iFrameImageHeight];

						// ����ؼ�֡
						_SGCopyGOPGifFrame(&stuNXTempPreGifFrame, &pSGGifData->pNXGOPGifFrame[pSGGifData->iAlreadyGetGOPNum]);
						++pSGGifData->iAlreadyGetGOPNum;
					}

					// �ж��Ƿ��Ѿ���ɴ˴���������
					if(iBeginFrameIndex >= iEndFrameNum)
					{
						// ���������
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
					// Ѱ��ͼ����ƿ��ǩ 0xf9
					if(ExtCode == 0x000000f9)
					{
						// �˿��¼�����ŵ�ͼ����Ϣ�����ӳ٣�͸��ɫ������
						SGByteType byteBlockSize = Extension[0]; // �˿��С
						bUseTransparentColorIndex = Extension[1] & 0x01 ? true : false;
						if(bUseTransparentColorIndex == true)
						{
							// ����͸��ɫ����
							ucTransparentColorIndex = (unsigned char)Extension[4];
						}
						stuNXTempPreGifFrame.usFrameDelayTime = *(unsigned short *)(&Extension[2]); // ����ͼ���ӳ�ʱ�䣬��λΪ 1/100s
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
*	��ָ����GOP֡��Ϊǰһ֡�����ļ��ж�ȡĿ��֡���ݣ�����stuNXPreGifFrame�ṹ��				*
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
	unsigned char ucTransparentColorIndex = 0; // ͸��ɫ����
	bool bUseTransparentColorIndex = false; // ��֡ͼ���Ƿ�ʹ��͸������

	// ���ļ�ָ���ƶ�����һ֡ͼ���ȡ���ʱ��λ��
	int iBeginFrameIndex = -1;
	BOOL bUsePreFrameData = (iAimFrameIndex == (pSGGifData->stuNXPreGifFrame.iFrameIndex + 1) ? TRUE : FALSE);
	if(bUsePreFrameData)
	{
		// ������ȡ֡�����ش�GOP�ؼ�֡��ʼ��
		pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, pSGGifData->stuNXPreGifFrame.llFrameCursor, NULL, FILE_BEGIN);
		iBeginFrameIndex = pSGGifData->stuNXPreGifFrame.iFrameIndex; // �ն�ȡ��֡��
	}
	else
	{
		pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, pSGGifData->pNXGOPGifFrame[iGOPIndex].llFrameCursor, NULL, FILE_BEGIN);
		iBeginFrameIndex = pSGGifData->pNXGOPGifFrame[iGOPIndex].iFrameIndex; // �ն�ȡ��֡��
	}

	// ����ǰһ֡��ɫ��ͼ������
	if(pSGGifData->stuNXPreGifFrame.pFrameData != NULL)
	{
		if(bUsePreFrameData == FALSE 
			&& (pSGGifData->stuNXPreGifFrame.iFrameImageHeight != pSGGifData->pNXGOPGifFrame[iGOPIndex].iFrameImageHeight
			|| pSGGifData->stuNXPreGifFrame.iFrameImageWidth != pSGGifData->pNXGOPGifFrame[iGOPIndex].iFrameImageWidth)
			)
		{
			// ��С����ͬ���ͷ�Ȼ������
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
		// ���˺��ʵĿռ䣬ֱ�ӿ���
		if(_SGCopyGOPGifFrame(pSGGifData->pNXGOPGifFrame + iGOPIndex, &pSGGifData->stuNXPreGifFrame) == FALSE)
		{
			return FALSE;
		}
	}
	
	BYTE *pImage = new BYTE[GifFile->SWidth * GifFile->SHeight]; // ��¼�ն�ȡ�ĵ�ɫ������

	do {
		if (DSGGetRecordType(GifFile, &eRecordType) == GIF_ERROR) {
			return NULL;
		}
		switch (eRecordType) {
			case IMAGE_DESC_RECORD_TYPE: // ͼ��������
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
					Width = GifFile->Image.Width; // ͼ����
					Height = GifFile->Image.Height;

					BYTE *pTrv = pImage;
					if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
						GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) { // ͼ��Խ�磬һ�㲻�ᷢ��
							if(pImage != NULL)
							{
								delete []pImage;
								pImage = NULL;
							}
							return NULL;
					}
					if (GifFile->Image.Interlace) { // �������
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
					else { // ˳�����
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

					// ���е�ɫ��ͼ�����
					BYTE *pPaletteTrv = pImage;
					int nEveryRowDistance = (GifFile->SWidth - GifFile->Image.Width); // ͼ�������β����һ�����׵ľ���

					// ����֡���ݵ��ӵ�ǰһ֡����
					// ��͸��ɫ����ֵ��Ϊ��Ӧ��ǰһ֡����ͼ������
					int iPixelNum = GifFile->SWidth * GifFile->SHeight;
					BYTE *pSrcTrv = pImage;
					BYTE *pDestTRv = pSGGifData->stuNXPreGifFrame.pFrameData + GifFile->Image.Top * GifFile->SWidth + GifFile->Image.Left; // ��֡����ͼ����ǰһ֡����ʼλ��
					for(int i = 0; i < Height; ++i)
					{
						for(int j = 0; j < Width; ++j)
						{
							if(*pSrcTrv != ucTransparentColorIndex && bUseTransparentColorIndex == true)
							{
								// �����ص��ӵ�ǰһ֡��Ӧλ��
								*pDestTRv = *pSrcTrv;
							}
							++pDestTRv;
							++pSrcTrv;
						}
						pDestTRv += nEveryRowDistance; // ��һ����ʼλ��
					}
					// �����֡�ĵ�ɫ���
					if(GifFile->Image.ColorMap != NULL)
					{
						// ��֡ʹ�õ����ĵ�ɫ�壬��Ҫ���е�ɫ�����
					}

					++iBeginFrameIndex;
					if(iBeginFrameIndex == iAimFrameIndex)
					{
						// ��Ҫȡ��ͼ��֡
						pSGGifData->stuNXPreGifFrame.iFrameIndex = iAimFrameIndex; // ����֡��
						LARGE_INTEGER llCurrentFilePointer;
						LARGE_INTEGER llMoveDistance;
						llMoveDistance.QuadPart = 0;
						pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, llMoveDistance, &llCurrentFilePointer, FILE_CURRENT); // ���浱ǰ�ļ�ָ��λ��
						pSGGifData->stuNXPreGifFrame.llFrameCursor = llCurrentFilePointer; // �����ļ�λ��ָ��
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
					// Ѱ��ͼ����ƿ��ǩ 0xf9
					if(ExtCode == 0x000000f9)
					{
						// �˿��¼�����ŵ�ͼ����Ϣ�����ӳ٣�͸��ɫ������
						SGByteType byteBlockSize = Extension[0]; // �˿��С
						bUseTransparentColorIndex = Extension[1] & 0x01 ? true : false;
						if(bUseTransparentColorIndex == true)
						{
							// ����͸��ɫ����
							ucTransparentColorIndex = (unsigned char)Extension[4];
						}
						pSGGifData->stuNXPreGifFrame.usFrameDelayTime = *(unsigned short *)(&Extension[2]); // ����ͼ���ӳ�ʱ�䣬��λΪ 1/100s
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
*	����ָ����֡�ţ���ȡ��Ӧ��֡ͼ��BGRA���ݡ�																																		*
*	���� Play ���� Seek����ʹ������������˺����ڽ��� GOP�ؼ�֡��ȡ�Ȳ�����																							*
****************************************************************************************************************************************************/
BYTE *SGGetGifImage(SGGIFFileData* pSGGifData, int nImageIndex, int *parWidth, int *parHeight, unsigned short *parDelayTime)
{
	if(NULL == pSGGifData || pSGGifData->pGifFile == NULL)
		return NULL;

	// �жϴ�֡�Ƿ�Ϊ�ϴθյ��õ�
	if(nImageIndex == pSGGifData->stuNXPreGifFrame.iFrameIndex)
	{
		// ��������ͬһ֡��ֱ�ӽ���֡��ɫ������ת��ΪBGRA����
		*parWidth = pSGGifData->stuNXPreGifFrame.iFrameImageWidth;
		*parHeight = pSGGifData->stuNXPreGifFrame.iFrameImageHeight;
		*parDelayTime = pSGGifData->stuNXPreGifFrame.usFrameDelayTime;

		return _SGPaletteImage2BGRA(&pSGGifData->stuNXPreGifFrame);
	}

	// �жϴ�֡�Ƿ���Ŀǰ GOP �ؼ�֡��
	int iIndexInGOP = IndexInGOP(pSGGifData->pNXGOPGifFrame, pSGGifData->iAlreadyGetGOPNum, nImageIndex);
	if(iIndexInGOP == -1)
	{
		// ���� GOP������������У��õ�Ŀ��֡ͼ��
		// ȷ���������֡��ԭ���������֡һ��Ҫ����Ŀ��֡
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
		
		iEndFrameNum = (iEndFrameNum > pSGGifData->iFrameNum - 1) ? pSGGifData->iFrameNum - 1 : iEndFrameNum; // ��������ֹ�������һ֡
		if(_SGExternGOP(pSGGifData, iEndFrameNum, nImageIndex) == FALSE)
		{
			return NULL;
		}

		// ��������GOP����Ŀ��֡���ݱ����ڼ�¼�ն�ȡ��֡�ṹ�ֱ��ת������
		*parWidth = pSGGifData->stuNXPreGifFrame.iFrameImageWidth;
		*parHeight = pSGGifData->stuNXPreGifFrame.iFrameImageHeight;
		*parDelayTime = pSGGifData->stuNXPreGifFrame.usFrameDelayTime;

		return _SGPaletteImage2BGRA(&pSGGifData->stuNXPreGifFrame);
	}
	else
	{
		if(pSGGifData->pNXGOPGifFrame[iIndexInGOP].iFrameIndex == nImageIndex)
		{
			// ��Ҫȡ��֡ǡ���ǹؼ�֡
			*parWidth = pSGGifData->pNXGOPGifFrame[iIndexInGOP].iFrameImageWidth;
			*parHeight = pSGGifData->pNXGOPGifFrame[iIndexInGOP].iFrameImageHeight;
			*parDelayTime = pSGGifData->pNXGOPGifFrame[iIndexInGOP].usFrameDelayTime;

			return _SGPaletteImage2BGRA(pSGGifData->pNXGOPGifFrame + iIndexInGOP);
		}
		else
		{
			// �ӹؼ�֡ iIndexInGOP ��ʼ���ļ���ȡ֡
			if(_SGReadPaletteImageByGOP(pSGGifData, iIndexInGOP, nImageIndex) == FALSE)
			{
				return NULL;
			}
			else
			{
				// ����ȡ��֡���ݣ�ת������
				*parWidth = pSGGifData->stuNXPreGifFrame.iFrameImageWidth;
				*parHeight = pSGGifData->stuNXPreGifFrame.iFrameImageHeight;
				*parDelayTime = pSGGifData->stuNXPreGifFrame.usFrameDelayTime;

				return _SGPaletteImage2BGRA(&pSGGifData->stuNXPreGifFrame);
			}
		}
	}
}