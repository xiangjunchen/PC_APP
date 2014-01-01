// SG: PCX pSGImageDescriptor->pPrivateData 使用
#include "stdafx.h"
#include "TPImgInterface.h"
#include "TPAVTgaFile.h"
#define PCX_MAGIC 0X0A  // PCX magic number
#define PCX_256_COLORS 0X0C  // magic number for 256 colors
#define PCX_HDR_SIZE 128  // size of PCX header
#define PCX_MAXCOLORS 256
#define PCX_MAXPLANES 4
#define PCX_MAXVAL 255

int ReadPcxFile(LPTPResource lpResource);
int WritePcxFile(TCHAR *output_file,int iWidth,int iHeight,int iBitsCount, BYTE* pBuffer) ;
int _SGWritePcxFile(TCHAR *output_file,int iWidth,int iHeight,int iBitsCount, BYTE* pBuffer);

extern int WriteBmp(TCHAR* ptszFileName,int iWidth,int iHeight,int iDepth,BYTE* pData);
extern BOOL Convert32To24BlendAlpha(BYTE* pDst,BYTE* pSrc,int iWidth,int iHeight);
void PCX_UnpackPixels(BYTE * pixels, BYTE * bitplanes, short bytesperline, short planes, short bitsperpixel);
void PCX_PlanesToPixels(BYTE * pixels, BYTE * bitplanes, short bytesperline, short planes, short bitsperpixel);
void Conver8_to_24(BYTE *pDesbuf, int width, int height, BYTE* pSrcbuf,BYTE pColorCap[PCX_MAXCOLORS][3]); 

typedef struct tagPCXHEADER
{
	char Manufacturer;	// always 0X0A
	char Version;			// version number
	char Encoding;		// always 1
	char BitsPerPixel;	// color bits
	WORD Xmin, Ymin;		// image origin
	WORD Xmax, Ymax;		// image dimensions
	WORD Hres, Vres;		// resolution values
	BYTE ColorMap[16][3];	// color palette
	char Reserved;
	char ColorPlanes;		// color planes
	WORD BytesPerLine;	// line buffer size
	WORD PaletteType;		// grey or color palette
	char Filter[58];
} PCXHEADER, *LPPCXHEADER;

//pcx
typedef struct  tag_TPPCXFileData
{
	CSGBufferFile pcxNXBufferedFile; // 提供具有缓冲的读写
	HANDLE hPcxFile; // PCX 句柄
	PCXHEADER stuPcxHeader;
	void tag_PCXFileData()
	{
		hPcxFile = NULL;
		TP_MemsetChar(&stuPcxHeader, 0, sizeof(PCXHEADER));
	}
}TPPCXFileData, *LPTPPCXFileData;

// SG: 读取 PCX 图像数据，存入 lpResource，图像格式为 BGRA
int ReadPcxFile(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	LPTPPCXFileData pTPPCXFileData = (LPTPPCXFileData)pSGImageDescriptor->pPrivateData;
	if(NULL == pTPPCXFileData || NULL == pTPPCXFileData->hPcxFile)
	{
		return FIO_OtherErr;
	}

	BYTE *pcximage = NULL;
	BYTE *lpHead = NULL;
	int nBytes = pTPPCXFileData->stuPcxHeader.BytesPerLine * pTPPCXFileData->stuPcxHeader.ColorPlanes * pSGImageDescriptor->iHeight;
	lpHead = pcximage = new BYTE[nBytes];

	BYTE ColorMap[PCX_MAXCOLORS][3];
	BYTE c;
	int count;
	int innn =0;
	////解压缩 file
	DWORD dwRealReadBytes = 0;

	while(nBytes > 0)
	{
		pTPPCXFileData->pcxNXBufferedFile.SGReadFile(pTPPCXFileData->hPcxFile, &c, 1, &dwRealReadBytes, NULL);

		innn ++;
		if ((c & 0XC0) != 0XC0)
		{ // Repeated group
			*pcximage++ = c;
			--nBytes;
			continue;
		}
		count = c & 0X3F; //
		pTPPCXFileData->pcxNXBufferedFile.SGReadFile(pTPPCXFileData->hPcxFile, &c, 1, &dwRealReadBytes, NULL);
		if(count > nBytes)
			break;

		nBytes -= count;
		while(--count >= 0)
			*pcximage++ = c;
	}
	pcximage = lpHead;

	//下面开始读取 调色板
	for(int i = 0 ;i < 16; i ++) // SG: 16 色图像
	{
		ColorMap[i][0] = pTPPCXFileData->stuPcxHeader.ColorMap[i][0];
		ColorMap[i][1] = pTPPCXFileData->stuPcxHeader.ColorMap[i][1];
		ColorMap[i][2] = pTPPCXFileData->stuPcxHeader.ColorMap[i][2];
	}

	if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==8 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1) // SG: 256 色调色板图像
	{
		pTPPCXFileData->pcxNXBufferedFile.SGReadFile(pTPPCXFileData->hPcxFile, &c, 1, &dwRealReadBytes, NULL);
		if(c == PCX_256_COLORS )
		{
			for(int i=0 ; i< PCX_MAXCOLORS; i++)
			{
				pTPPCXFileData->pcxNXBufferedFile.SGReadFile(pTPPCXFileData->hPcxFile, ColorMap[i], 3, &dwRealReadBytes, NULL);
			}
		}
	}
	if(pTPPCXFileData->stuPcxHeader.BitsPerPixel == 1 && pTPPCXFileData->stuPcxHeader.ColorPlanes ==1) // SG: 黑白图像
	{
		ColorMap[0][0] = ColorMap[0][1]=  ColorMap[0][2]= 0;
		ColorMap[1][0] = ColorMap[1][1] = ColorMap[1][2]= 255;
	}
	//文件读写完毕
	//下面开始重新组织 buffer buffersize is ((ibitcount*iwidth +31)/32 )*4
	//int nRealRowByte = ((( pTPPCXFileData->stuPcxHeader.BitsPerPixel * pPcxHeader->ColorPlanes) * pSGImageDescriptor->iWidth +31)/32)*4;
	int nRealRowByte = ((24 * pSGImageDescriptor->iWidth +31)/32)*4;
	int ibufsize =  nRealRowByte * pSGImageDescriptor->iHeight;
	BYTE *pImgBuff = new BYTE[ibufsize];
	TP_MemsetChar(pImgBuff, 0, ibufsize);
	BYTE *lpHead2 = NULL;
	BYTE *pcxplanes, *pcxpixels;
	lpHead2 = pcxpixels = new BYTE[pSGImageDescriptor->iWidth + pTPPCXFileData->stuPcxHeader.BytesPerLine *8 ];
	int x, y ,y2;
	BYTE *pPaletteImageRow = pImgBuff; // SG: 每行的起始位置
	for(y =0 ; y < pSGImageDescriptor->iHeight; y ++)
	{
		y2 = pSGImageDescriptor->iHeight -1-y;
		pcxpixels = lpHead2;
		pcxplanes = pcximage + (y * pTPPCXFileData->stuPcxHeader.BytesPerLine * pTPPCXFileData->stuPcxHeader.ColorPlanes);

		if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==8 &&pTPPCXFileData->stuPcxHeader.ColorPlanes == 3)
		{
			BYTE* iDst = NULL;
			for (x = 0; x < pSGImageDescriptor->iWidth; x++)
			{
				iDst = pImgBuff + y * nRealRowByte  +x *3; 
				*iDst++ = pcxplanes[2*pTPPCXFileData->stuPcxHeader.BytesPerLine + x]; //B
				*iDst++ =pcxplanes[pTPPCXFileData->stuPcxHeader.BytesPerLine + x];  //G
				*iDst   =  pcxplanes[x];    //R
			}                           
			continue;
		}
		else if (pTPPCXFileData->stuPcxHeader.ColorPlanes == 1) 
		{
			PCX_UnpackPixels(pcxpixels, pcxplanes, pTPPCXFileData->stuPcxHeader.BytesPerLine, pTPPCXFileData->stuPcxHeader.ColorPlanes, pTPPCXFileData->stuPcxHeader.BitsPerPixel);
		} 
		else 
		{
			// SG: pcxpixels 中存放的是一个字节代表一个像素
			PCX_PlanesToPixels(pcxpixels, pcxplanes, pTPPCXFileData->stuPcxHeader.BytesPerLine, pTPPCXFileData->stuPcxHeader.ColorPlanes, pTPPCXFileData->stuPcxHeader.BitsPerPixel);
		}
		
		//colorplanes == 1; or else 将pcxpixels 拷贝到pImgBuffer中
		if(pTPPCXFileData->stuPcxHeader.BitsPerPixel == 8){
			TP_MemcpyLine(pPaletteImageRow, pcxpixels, pSGImageDescriptor->iWidth);
		}
		else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel == 4){
			for(int i = 0; i < pSGImageDescriptor->iWidth; ++i){
				pPaletteImageRow[i] = pcxpixels[i] & 0x0f;
			}
		}
		else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel == 1){
			for(int i = 0; i < pSGImageDescriptor->iWidth; ++i){
				pPaletteImageRow[i] = pcxpixels[i] & 0x01;
			}
		}
		pPaletteImageRow += pSGImageDescriptor->iWidth;
	}

	delete []lpHead2;
	delete []pcximage;
	if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==8 &&pTPPCXFileData->stuPcxHeader.ColorPlanes == 3)
	{
		pSGImageDescriptor->pImage = new BYTE[pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight * 4];

		TPCS_24_to_32_Frame(pSGImageDescriptor->pImage, pSGImageDescriptor->iWidth * 4, pImgBuff, \
			nRealRowByte, NULL, 0, pSGImageDescriptor->iWidth, pSGImageDescriptor->iHeight, FALSE, FALSE);
		delete []pImgBuff;
		return FIO_Succeed;
	}
	else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==8 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1) // SG: 256 色图像
	{

		BYTE  *pptem = new BYTE[pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight *3];
		Conver8_to_24(pptem,pSGImageDescriptor->iWidth,pSGImageDescriptor->iHeight,pImgBuff,ColorMap);  
		
		pSGImageDescriptor->pImage = new BYTE[pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight * 4];
		BYTE *pBGRTrv = pptem;
		BYTE *pBGRATrv = pSGImageDescriptor->pImage;
		for(int i = 0; i < pSGImageDescriptor->iHeight; ++i){
			for(int j = 0; j < pSGImageDescriptor->iWidth; ++j){
				TP_MemcpyLine(pBGRATrv, pBGRTrv, 3);
				pBGRATrv[3] = 0xff;
				pBGRATrv += 4;
				pBGRTrv += 3;
			}
		}
		delete []pptem;
		delete []pImgBuff;
		return FIO_Succeed;
	}
	else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==1 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1) // SG: 单色图像
	{

		BYTE  *pptem = new BYTE[pSGImageDescriptor->iWidth * 3 * pSGImageDescriptor->iHeight];
		Conver8_to_24(pptem,pSGImageDescriptor->iWidth ,pSGImageDescriptor->iHeight,pImgBuff,ColorMap);  

		pSGImageDescriptor->pImage = new BYTE[pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight * 4];
		BYTE *pBGRTrv = pptem;
		BYTE *pBGRATrv = pSGImageDescriptor->pImage ;
		for(int i = 0; i < pSGImageDescriptor->iHeight; ++i){
			for(int j = 0; j < pSGImageDescriptor->iWidth; ++j){
				TP_MemcpyLine(pBGRATrv, pBGRTrv, 3);
				pBGRATrv[3] = 0xff;
				pBGRATrv += 4;
				pBGRTrv += 3;
			}
		}
		delete []pptem;
		delete []pImgBuff;
		return FIO_Succeed;
	}
	else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==4 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1) // SG: 16 色图像
	{

		BYTE  *pptem = new BYTE[pSGImageDescriptor->iWidth * 3 * pSGImageDescriptor->iHeight];
		Conver8_to_24(pptem,pSGImageDescriptor->iWidth ,pSGImageDescriptor->iHeight,pImgBuff,ColorMap);  

		pSGImageDescriptor->pImage = new BYTE[pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight * 4];
		BYTE *pBGRTrv = pptem;
		BYTE *pBGRATrv = pSGImageDescriptor->pImage ;
		for(int i = 0; i < pSGImageDescriptor->iHeight; ++i){
			for(int j = 0; j < pSGImageDescriptor->iWidth; ++j){
				TP_MemcpyLine(pBGRATrv, pBGRTrv, 3);
				pBGRATrv[3] = 0xff;
				pBGRATrv += 4;
				pBGRTrv += 3;
			}
		}
		delete []pptem;
		delete []pImgBuff;
		return FIO_Succeed;
	}
	else
	{
		delete []pImgBuff;
		pSGImageDescriptor->pImage = NULL;
	}
	return FIO_OtherErr;
}

int pcxOpen(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	LPTPPCXFileData pTPPCXFileData = new TPPCXFileData;
	pTPPCXFileData->hPcxFile = pTPPCXFileData->pcxNXBufferedFile.SGCreateFile(lpResource->ptszfileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == pTPPCXFileData->hPcxFile)
	{
		delete pTPPCXFileData;
		return FIO_OpenErr;
	}

	DWORD dwRealReadBytes = 0;
	if(pTPPCXFileData->pcxNXBufferedFile.SGReadFile(pTPPCXFileData->hPcxFile, &pTPPCXFileData->stuPcxHeader, sizeof(PCXHEADER), &dwRealReadBytes, NULL) == FALSE)
	{
		CloseHandle(pTPPCXFileData->hPcxFile);
		delete pTPPCXFileData;
		return FIO_OtherErr;
	}
	if(pTPPCXFileData->stuPcxHeader.Manufacturer != PCX_MAGIC)
	{
		CloseHandle(pTPPCXFileData->hPcxFile);
		delete pTPPCXFileData;
		return FIO_OtherErr;
	}

	pSGImageDescriptor->iWidth = (pTPPCXFileData->stuPcxHeader.Xmax - pTPPCXFileData->stuPcxHeader.Xmin) + 1;
	pSGImageDescriptor->iHeight = (pTPPCXFileData->stuPcxHeader.Ymax - pTPPCXFileData->stuPcxHeader.Ymin) + 1;
	pSGImageDescriptor->pPrivateData = pTPPCXFileData; // SG: pcxGetImg() 使用
	
	// 保存PCX 的 alpha 信息 add by wgf@2010-5-31
	if(pTPPCXFileData->stuPcxHeader.BitsPerPixel == 8 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 4)
	{
		pSGImageDescriptor->bHaveAlpha = true;
	}
	else
	{
		pSGImageDescriptor->bHaveAlpha = false;
	}
	
	return FIO_Succeed;

}

int pcxClose(SGImageDescriptor *pSGImageDescriptor)
{
	if(NULL == pSGImageDescriptor)
	{
		return FIO_Succeed;
	}
	LPTPPCXFileData pTPPCXFileData = (LPTPPCXFileData)pSGImageDescriptor->pPrivateData;
	if(NULL != pTPPCXFileData)
	{
		// 文件句柄的关闭有类的析构函数完成
		delete pTPPCXFileData;
		pSGImageDescriptor->pPrivateData = NULL;
	}
	return FIO_Succeed;
}

int pcxGetImage(LPTPResource lpResource)
{
	return ReadPcxFile(lpResource);
}

// 多次读取一幅图像，每次读取若干行，以减少对超大图像读取的内存消耗 Begin
// 此函数仅做图像读取前的准备工作，不读取图像数据
// pSGImageDescriptor->pImage 的内存由调用者申请
// 读取 256 色图像的调色板数据，并将指针移动到图像数据位置
int NXpcxPrepareGetImage(LPTPResource lpResource, UINT *pGrayPlatte, void **ppPrivateInfo)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	LPTPPCXFileData pTPPCXFileData = (LPTPPCXFileData)pSGImageDescriptor->pPrivateData;

	if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==8 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 3)
	{
		// 24 位真彩色不需要调色板，直接返回
		return FIO_Succeed;
	}

	if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==8 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1) // SG: 256 色图像
	{
		LARGE_INTEGER llCurrentPos; // 保存当前指针位置
		llCurrentPos.QuadPart = 0;
		LARGE_INTEGER llMoveDistance;
		llMoveDistance.QuadPart = 0;
		pTPPCXFileData->pcxNXBufferedFile.SGSetFilePointerEx(pTPPCXFileData->hPcxFile, llMoveDistance, &llCurrentPos, FILE_CURRENT); // 得到当前指针位置
		// 移动到调色板数据，文件末尾的 769 字节
		pTPPCXFileData->pcxNXBufferedFile.SGSetFilePointerEx(pTPPCXFileData->hPcxFile, llMoveDistance, NULL, FILE_END); // 移到文件末尾
		llMoveDistance.QuadPart = -769;
		pTPPCXFileData->pcxNXBufferedFile.SGSetFilePointerEx(pTPPCXFileData->hPcxFile, llMoveDistance, NULL, FILE_END); // 移动调色板位置
		BYTE c = 0;
		DWORD dwRealReadBytes = 0;
		pTPPCXFileData->pcxNXBufferedFile.SGReadFile(pTPPCXFileData->hPcxFile, &c, 1, &dwRealReadBytes, NULL);
		if(c == PCX_256_COLORS )
		{
			pTPPCXFileData->pcxNXBufferedFile.SGReadFile(pTPPCXFileData->hPcxFile, pGrayPlatte, PCX_MAXCOLORS * 3, &dwRealReadBytes, NULL);
		}
		else
		{
			// 没有找到调色板
			pTPPCXFileData->pcxNXBufferedFile.SGSetFilePointerEx(pTPPCXFileData->hPcxFile, llCurrentPos, NULL, FILE_BEGIN); // 移到图像数据起始位置
			return FIO_OtherErr;
		}
		pTPPCXFileData->pcxNXBufferedFile.SGSetFilePointerEx(pTPPCXFileData->hPcxFile, llCurrentPos, NULL, FILE_BEGIN); // 移到图像数据起始位置
	}
	else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel == 4 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1) // SG: 16 色图像
	{
		// 已经从文件头结构中得到
	}
	else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel == 1 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1) // 黑白图像
	{
		pTPPCXFileData->stuPcxHeader.ColorMap[0][0] = pTPPCXFileData->stuPcxHeader.ColorMap[0][1] = pTPPCXFileData->stuPcxHeader.ColorMap[0][2] = 0;
		pTPPCXFileData->stuPcxHeader.ColorMap[1][0] = pTPPCXFileData->stuPcxHeader.ColorMap[1][1] = pTPPCXFileData->stuPcxHeader.ColorMap[1][2] = 0xff;
	}
	return FIO_Succeed;
}
// 仅读取指定行数的图像数据，读取结果为 BGRA
// iBeginRow >= 0	& iEndRow <= iImageHeight -1 
// 读取行数为 iEndRow - iBeginRow + 1，即 [iBeginRow, iEndRow]
int NXpcxReadImageBlock(LPTPResource lpResource, int iBeginRow, int iEndRow, UINT * pGrayPlatte, void *pPrivateInfo)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	
	LPTPPCXFileData pTPPCXFileData = (LPTPPCXFileData)pSGImageDescriptor->pPrivateData;
	if(NULL == pTPPCXFileData || NULL == pTPPCXFileData->hPcxFile)
	{
		return FIO_OtherErr;
	}

	int iRowBytes = pTPPCXFileData->stuPcxHeader.BytesPerLine * pTPPCXFileData->stuPcxHeader.ColorPlanes; // 一行数据
	BYTE *pcximage = new BYTE[iRowBytes];
	TP_MemsetChar(pcximage, 0, iRowBytes);
	// 读取压缩数据，并还原为图像数据的字节流
	TP_MemsetChar(pSGImageDescriptor->pImage, (char)0xff, pSGImageDescriptor->iWidth * (iEndRow - iBeginRow + 1) * 4);
	BYTE *pDestRowImage = pSGImageDescriptor->pImage;
	for(; iBeginRow <= iEndRow; ++iBeginRow)
	{
		// 一次处理一行
		int iRowByteTrv = iRowBytes;
		BYTE *pCompressTrv = pcximage;
		while(iRowByteTrv > 0)
		{ 
			BYTE c;
			DWORD dwRealReadBytes = 0;
			pTPPCXFileData->pcxNXBufferedFile.SGReadFile(pTPPCXFileData->hPcxFile, &c, 1, &dwRealReadBytes, NULL);

			if ((c & 0XC0) != 0XC0)
			{ // [0, 63] 的单个数据
				*pCompressTrv++ = c;
				--iRowByteTrv;
				continue;
			}
			int count = c & 0X3F; // 此数据重复次数
			pTPPCXFileData->pcxNXBufferedFile.SGReadFile(pTPPCXFileData->hPcxFile, &c, 1, &dwRealReadBytes, NULL);
			if(count > iRowByteTrv)
				break;

			iRowByteTrv -= count;
			while(--count >= 0)
				*pCompressTrv++ = c;
		}

		// 将读取的图像数据还原为 BGRA 
		if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==8 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 3)
		{
			// 24位真彩色图像，图像数据排列为 RGB，每个分量一行
			BYTE *pR = pcximage;
			BYTE *pG = pcximage + pTPPCXFileData->stuPcxHeader.BytesPerLine;
			BYTE *pB= pcximage + pTPPCXFileData->stuPcxHeader.BytesPerLine * 2;
			for(int k = 0; k < pSGImageDescriptor->iWidth; ++k)
			{
				pDestRowImage[0] = *pB++;
				pDestRowImage[1] = *pG++;
				pDestRowImage[2] = *pR++;
				pDestRowImage += 4;
			}
		}
		else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==8 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1)
		{
			// 256色索引图像
			BYTE *pRGBPalette = (BYTE *)pGrayPlatte;
			for(int k = 0; k < pSGImageDescriptor->iWidth; ++k)
			{
				//TP_MemcpyLine(pDestRowImage, pRGBPalette + (UINT)pcximage[k] * 3, 3);
				pDestRowImage[0] = pRGBPalette[(UINT)pcximage[k] * 3 + 2];
				pDestRowImage[1] = pRGBPalette[(UINT)pcximage[k] * 3 + 1];
				pDestRowImage[2] = pRGBPalette[(UINT)pcximage[k] * 3];
				pDestRowImage += 4;
			}
		}
		else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==4 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1)
		{
			int iPixelNum = pSGImageDescriptor->iWidth / 2;
			for(int i = 0; i < iPixelNum; ++i)
			{
				BYTE byteMask = 0xf0;
				for(int k = 0; k < 2; ++k)
				{
					BYTE bytePaletteIndex = byteMask & pcximage[i];
					if(byteMask == 0xf0)
					{
						bytePaletteIndex >>= 4;
					}
					pDestRowImage[0] = pTPPCXFileData->stuPcxHeader.ColorMap[bytePaletteIndex][0];
					pDestRowImage[1] = pTPPCXFileData->stuPcxHeader.ColorMap[bytePaletteIndex][1];
					pDestRowImage[2] = pTPPCXFileData->stuPcxHeader.ColorMap[bytePaletteIndex][2];
					pDestRowImage += 4;
					byteMask = 0x0f;
				}
			}
			// 处理奇数列的最后一列图像数据
			if(pSGImageDescriptor->iWidth % 2 == 1)
			{
				BYTE bytePaletteIndex = 0xf0 & pcximage[pSGImageDescriptor->iWidth / 2 + 1];
				bytePaletteIndex >>= 4;
				pDestRowImage[0] = pTPPCXFileData->stuPcxHeader.ColorMap[bytePaletteIndex][0];
				pDestRowImage[1] = pTPPCXFileData->stuPcxHeader.ColorMap[bytePaletteIndex][1];
				pDestRowImage[2] = pTPPCXFileData->stuPcxHeader.ColorMap[bytePaletteIndex][2];
				pDestRowImage += 4;
			}
		}
		else if(pTPPCXFileData->stuPcxHeader.BitsPerPixel ==1 && pTPPCXFileData->stuPcxHeader.ColorPlanes == 1)
		{
			BYTE bitMask = 0x80;
			BYTE *pBitTrv = pcximage;
			for(int i = 0; i < pSGImageDescriptor->iWidth; ++i)
			{
				BYTE pixelVal = (*pBitTrv & bitMask) ? 0xff : 0;
				TP_MemsetChar(pDestRowImage, pixelVal, 3);
				pDestRowImage += 4;
				bitMask >>= 1;
				if(bitMask == 0)
				{
					bitMask = 0x80;
					++pBitTrv;
				}
			}
		}
		else
		{
			// 其它位数的图像暂不支持
		}
	}
	delete []pcximage;
	return FIO_Succeed;
}
// 多次读取一幅图像，每次读取若干行，以减少对超大图像读取的内存消耗 End

int pcxPutImage(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	BYTE * pBGR = new BYTE[pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight * 3];
	Convert32To24BlendAlpha(pBGR,pSGImageDescriptor->pImage, pSGImageDescriptor->iWidth , pSGImageDescriptor->iHeight);
	_SGWritePcxFile(lpResource->ptszfileName, pSGImageDescriptor->iWidth , pSGImageDescriptor->iHeight, 24, pBGR);
	delete []pBGR;
	return FIO_Succeed;
}


///////////////////////////////////////
/////////pcx file
///////////////////////////////////////
void PCX_PlanesToPixels(BYTE * pixels, BYTE * bitplanes, short bytesperline, short planes, short bitsperpixel)
{
	int i, j, npixels;
	BYTE * p;
	if (planes > 4) throw "Can't handle more than 4 planes";
	if (bitsperpixel != 1) throw "Can't handle more than 1 bit per pixel";

	// Clear the pixel buffer
	npixels = (bytesperline * 8) / bitsperpixel;
	p = pixels;
	while (--npixels >= 0)
		*p++ = 0;

	// Do the format conversion
	for (i = 0; i < planes; i++	)
	{
		int pixbit, bits, mask;
		p = pixels;
		pixbit = (1 << i);  // pixel bit for this plane
		for (j = 0; j < bytesperline; j++)
		{
			bits = *bitplanes++;
			for (mask = 0X80; mask != 0; mask >>= 1, p++)
				if (bits & mask) *p |= pixbit;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
// convert packed pixel format into 1 pixel per byte
// from unpacked file data bitplanes[] into pixel row pixels[]
// image Height rows, with each row having planes image planes each
// bytesperline bytes
void PCX_UnpackPixels(BYTE * pixels, BYTE * bitplanes, short bytesperline, short planes, short bitsperpixel)
{
	register int bits;
	if (planes != 1) throw "Can't handle packed pixels with more than 1 plane.";

	if (bitsperpixel == 8)
	{  // 8 bits/pixels, no unpacking needed
		while (bytesperline-- > 0) *pixels++ = *bitplanes++;
	} 
	else if (bitsperpixel == 4)
	{  // 4 bits/pixel, two pixels per byte
		while (bytesperline-- > 0)
		{
			bits = *bitplanes++;
			*pixels++ = (BYTE)((bits >> 4) & 0X0F);
			*pixels++ = (BYTE)((bits) & 0X0F);
		}
	} 
	else if (bitsperpixel == 2)
	{  // 2 bits/pixel, four pixels per byte
		while (bytesperline-- > 0)
		{
			bits = *bitplanes++;
			*pixels++ = (BYTE)((bits >> 6) & 0X03);
			*pixels++ = (BYTE)((bits >> 4) & 0X03);
			*pixels++ = (BYTE)((bits >> 2) & 0X03);
			*pixels++ = (BYTE)((bits) & 0X03);
		}
	}
	else if (bitsperpixel == 1)
	{  // 1 bits/pixel, 8 pixels per byte
		while (bytesperline-- > 0)
		{
			bits = *bitplanes++;
			*pixels++ = ((bits & 0X80) != 0);
			*pixels++ = ((bits & 0X40) != 0);
			*pixels++ = ((bits & 0X20) != 0);
			*pixels++ = ((bits & 0X10) != 0);
			*pixels++ = ((bits & 0X08) != 0);
			*pixels++ = ((bits & 0X04) != 0);
			*pixels++ = ((bits & 0X02) != 0);
			*pixels++ = ((bits & 0X01) != 0);
		}
	}
}///




void PCX_PackPixels(const long p,BYTE &c, BYTE &n,int &iBufsize,BYTE * pBuf)
{
	if (p!=c && n)
	{
		if (n==1 && c<0xC0)
		{ 
			*(pBuf + iBufsize) = c;
			iBufsize ++;
		} 
		else 
		{

			*(pBuf + iBufsize) = 0xC0|n;
			iBufsize ++;
			*(pBuf + iBufsize) = c;
			iBufsize ++; 
		}
		n=0;
	}
	if (n==0x3F)
	{

		*(pBuf + iBufsize) = 0xFF;
		iBufsize ++;

		*(pBuf + iBufsize) = c;
		iBufsize ++;
		n = 0;

	}
	if (p==-2)
	{
		*(pBuf + iBufsize) = 0;
		iBufsize ++;
	} 
	c=(BYTE)p;
	n++;


}

/************************************************************************************************************************
*	修改写PCX文件，使其可以支持奇数列的图像。																									*
*	只支持 24 位真彩色图像写。																															*
************************************************************************************************************************/
int _SGWritePcxFile(TCHAR *output_file,int iWidth,int iHeight,int iBitsCount, BYTE* pBuffer)
{
	if(output_file == NULL || *output_file == '\0' || pBuffer == NULL || iBitsCount != 24)
	{
		return FIO_OtherErr;
	} 

	HANDLE hPcxHandle = CreateFile(output_file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == hPcxHandle)
	{
		FIO_OtherErr;
	}

	if(NULL != hPcxHandle)
	{
		PCXHEADER pcxHeader;
		TP_MemsetChar(&pcxHeader,0,sizeof(pcxHeader));
		pcxHeader.Manufacturer = PCX_MAGIC;
		pcxHeader.Version = 5;
		pcxHeader.Encoding = 1;
		pcxHeader.Xmin =0;
		pcxHeader.Ymin =0;
		pcxHeader.Xmax = iWidth -1;
		pcxHeader.Ymax = iHeight -1;  
		pcxHeader.Hres = iWidth;
		pcxHeader.Vres = iHeight;
		pcxHeader.Reserved = 0;
		pcxHeader.PaletteType = 1;

		pcxHeader.BitsPerPixel =8;
		pcxHeader.ColorPlanes = 3;
		pcxHeader.BytesPerLine = (iWidth % 2 == 0) ? (iWidth) : (iWidth + 1); // 这个要求是偶数

		DWORD dwRealReadBytes = 0;
		WriteFile(hPcxHandle, &pcxHeader, sizeof(pcxHeader), &dwRealReadBytes, NULL);
		BYTE c,n; 
		BYTE * pTembuf = new BYTE[pcxHeader.BytesPerLine * iHeight *8]; //使用RLE编码，最大应为2倍大小缓冲区。by ljj @ 2006-7-3
		int  ibufsize = 0;
		for(int y = 0; y < iHeight ; y++) 
		{
			for(int iRgb = 0 ; iRgb < 3 ; iRgb ++)
			{
				c=n=0;
				for(int x = 0; x < iWidth; x ++) 
				{
					//pcx 文件中的 排列顺序是 B G R
					if(iRgb ==0 )
						PCX_PackPixels( *(pBuffer + y *iWidth*3  + x*3 + iRgb + 2),c,n,ibufsize,pTembuf);
					if(iRgb == 1)
						PCX_PackPixels( *(pBuffer + y *iWidth*3  + x*3 + iRgb),c,n,ibufsize,pTembuf);
					if(iRgb == 2)
						PCX_PackPixels( *(pBuffer + y *iWidth*3  + x*3 + iRgb- 2),c,n,ibufsize,pTembuf);
				}

				PCX_PackPixels(-1-(iWidth&0x1),c,n,ibufsize,pTembuf);
			}
		} 
		WriteFile(hPcxHandle, pTembuf, ibufsize, &dwRealReadBytes, NULL);
		delete []pTembuf; 
		CloseHandle(hPcxHandle);
		return 1;
	}

	return 0;
}

///////////////////////////////
//提示：8位的写不支持，因为pBuffer没有调色板的信息
////
////
int WritePcxFile(TCHAR *output_file,int iWidth,int iHeight,int iBitsCount, BYTE* pBuffer)
{
	HANDLE hPcxHandle = CreateFile(output_file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == hPcxHandle)
		return 0;
	//FILE *fp = _tfopen(output_file,_T("wb"));
	if(NULL != hPcxHandle)
	{
		PCXHEADER pcxHeader;
		TP_MemsetChar(&pcxHeader,0,sizeof(pcxHeader));
		pcxHeader.Manufacturer = PCX_MAGIC;
		pcxHeader.Version = 5;
		pcxHeader.Encoding = 1;
		pcxHeader.Xmin =0;
		pcxHeader.Ymin =0;
		pcxHeader.Xmax = iWidth -1;
		pcxHeader.Ymax = iHeight -1;  
		pcxHeader.Hres = iWidth;
		pcxHeader.Vres = iHeight;
		pcxHeader.Reserved = 0;
		pcxHeader.PaletteType = 1;

		switch(iBitsCount )
		{
		case 24:
			pcxHeader.BitsPerPixel =8;
			pcxHeader.ColorPlanes = 3;
			pcxHeader.BytesPerLine = iWidth;
			break;
		case 8://
			pcxHeader.BitsPerPixel =8;
			pcxHeader.ColorPlanes = 1;
			pcxHeader.BytesPerLine = iWidth;
			break;
		case 4:
		case 1:
			pcxHeader.BitsPerPixel = 1;
			pcxHeader.ColorPlanes = iBitsCount;
			pcxHeader.BytesPerLine = (WORD)((iWidth * iBitsCount+ 7)>>3);
			break;
		}

		if (pcxHeader.BitsPerPixel == 1 && pcxHeader.ColorPlanes == 1)
		{
			pcxHeader.ColorMap[0][0] = pcxHeader.ColorMap[0][1] = pcxHeader.ColorMap[0][2] = 0;
			pcxHeader.ColorMap[1][0] = pcxHeader.ColorMap[1][1] = pcxHeader.ColorMap[1][2] = 255;
		}
		//写入pcx的文件头，因为没有调色板信息，所以只能写入24位的图像
		DWORD dwRealReadBytes = 0;
		WriteFile(hPcxHandle, &pcxHeader, sizeof(pcxHeader), &dwRealReadBytes, NULL);
		BYTE c,n; 
		if(iBitsCount == 24)
		{ 
			BYTE * pTembuf = new BYTE[iWidth * iHeight *8]; //使用RLE编码，最大应为2倍大小缓冲区。by ljj @ 2006-7-3
			int  ibufsize = 0;
			for(int y = 0; y < iHeight ; y++) 
			{
				for(int iRgb = 0 ; iRgb < 3 ; iRgb ++)
				{
					c=n=0;
					for(int x = 0; x < iWidth; x ++) 
					{
						//pcx 文件中的 排列顺序是 B G R
						if(iRgb ==0 )
							PCX_PackPixels( *(pBuffer + y *iWidth*3  + x*3 + iRgb + 2),c,n,ibufsize,pTembuf);
						if(iRgb == 1)
							PCX_PackPixels( *(pBuffer + y *iWidth*3  + x*3 + iRgb),c,n,ibufsize,pTembuf);
						if(iRgb == 2)
							PCX_PackPixels( *(pBuffer + y *iWidth*3  + x*3 + iRgb- 2),c,n,ibufsize,pTembuf);
					}

					PCX_PackPixels(-1-(iWidth&0x1),c,n,ibufsize,pTembuf);
				}
			} 
			WriteFile(hPcxHandle, pTembuf, ibufsize, &dwRealReadBytes, NULL);
			delete []pTembuf; 
		}
		CloseHandle(hPcxHandle);
		return 1;
	}

	return 0;
}
void Conver8_to_24(BYTE *pDesbuf, int width, int height, BYTE* pSrcbuf,BYTE pColorCap[PCX_MAXCOLORS][3])
{
	if(!pDesbuf || !pSrcbuf)
		return ;
	
	BYTE *pDesTrv = pDesbuf;
	BYTE *pSrcTrv	= pSrcbuf;

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			*pDesTrv++ = pColorCap[*pSrcTrv][2];
			*pDesTrv++ = pColorCap[*pSrcTrv][1];
			*pDesTrv++ = pColorCap[*pSrcTrv][0];
			++pSrcTrv;
		}
	}
} 

void NXConver8_to_24(BYTE *pDesbuf, int width, int height, BYTE* pSrcbuf,BYTE *pColorCap)
{
	if(!pDesbuf || !pSrcbuf)
		return ;

	BYTE *pDesTrv = pDesbuf;
	BYTE *pSrcTrv	= pSrcbuf;

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			*pDesTrv++ = pColorCap[(UINT)*pSrcTrv * 3];
			*pDesTrv++ = pColorCap[(UINT)*pSrcTrv * 3 + 1];
			*pDesTrv++ = pColorCap[(UINT)*pSrcTrv * 3 + 2];
			++pSrcTrv;
		}
	}
} 
