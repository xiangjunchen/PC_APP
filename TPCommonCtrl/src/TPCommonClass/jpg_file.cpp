// SG: JPG pSGImageDescriptor->pPrivateData ʹ�ã���ž��
 #include "stdafx.h"
#include "TPImgInterface.h"
#include "Jpegfile.h"
#include "afxmt.h"

extern CCriticalSection  g_CritSection;
extern unsigned char* g_buf;

int (* JPG_GetSimpleCustomWnd)(TPFileFormat* ,void* ,void*& );
int (* JPG_GetCustomWnd)(TPFileFormat* ,void* ,void*& );
int (* JPG_UpDataCustomData)(TPFileFormat* ,void* );
int (* JPG_ReleaseWnd)(TPFileFormat* ,void* );
extern HMODULE g_hFileSetModule;

// ��һ�д��Ҷ�ͼ������ת��Ϊ BGRA ͼ������
void NXGrayGetScanlieBGRAEx(BYTE *pSrcLine, int iWidthPix, BYTE *pDestLine)
{
	BYTE *pDestTrv = pDestLine;
	BYTE *pSrcTrv = pSrcLine;
	for(int i = 0; i < iWidthPix; ++i)
	{
		memset(pDestTrv, *pSrcTrv, 3);
		pDestTrv[3] = 0xff;
		pDestTrv += 4;
		++pSrcTrv;
	}
}

// ��һ�е� RGB ����ת��Ϊ BGRA
void NXRGBGetScanlineBGRAEx(BYTE *pSrcLine, int iWidthPix, BYTE *pDestLine)
{
	BYTE *pDestTrv = pDestLine;
	BYTE *pSrcTrv = pSrcLine;
	for(int i = 0; i < iWidthPix; ++i)
	{
		pDestTrv[0] = pSrcTrv[2];
		pDestTrv[1] = pSrcTrv[1];
		pDestTrv[2] = pSrcTrv[0];
		pDestTrv[3] = 0xff;
		pDestTrv += 4;
		pSrcTrv += 3;
	}
}
BOOL Convert32To24BlendAlpha(BYTE* pDst,BYTE* pSrc,int iWidth,int iHeight)
{
	if(!pDst || !pSrc || iWidth<=0 || iHeight<=0) return FALSE;
	for(INT l=0;l<iHeight;l++)
	{
		for(INT m=0;m<iWidth;m++)
		{
			pDst[l*iWidth*3+m*3+0] =(BYTE)(((double)pSrc[l*iWidth*4+m*4+0]) *((double) pSrc[l*iWidth*4+m*4+3])/255.0);
			pDst[l*iWidth*3+m*3+1] =(BYTE)(((double)pSrc[l*iWidth*4+m*4+1]) *((double) pSrc[l*iWidth*4+m*4+3])/255.0);
			pDst[l*iWidth*3+m*3+2] =(BYTE)(((double)pSrc[l*iWidth*4+m*4+2]) *((double) pSrc[l*iWidth*4+m*4+3])/255.0);
		}
	}
	return TRUE;
}

// SG: �õ�ͼ��� �� & ��
int jpgOpen(LPTPResource lpResource)
{ 
	if(NULL == lpResource)
	{
		return FIO_OpenErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	HANDLE hJgpFile = CreateFile(lpResource->ptszfileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == hJgpFile)
	{
		return FIO_OpenErr;
	}

	CSingleLock singleLock(&g_CritSection);
	singleLock.Lock();
	JpegFile::GetHeadSize(hJgpFile, (UINT *)&pSGImageDescriptor->iWidth, (UINT *)&pSGImageDescriptor->iHeight);
	if(singleLock.IsLocked())
		singleLock.Unlock();
	pSGImageDescriptor->pPrivateData = hJgpFile; // SG: ���� JPG ������ǵùر�
	return FIO_Succeed;
}

int jpgClose(SGImageDescriptor *pSGImageDescriptor)
{
	if(NULL != pSGImageDescriptor && NULL != pSGImageDescriptor->pPrivateData) // SG: ���洢�����дû��ʹ��
	{
		CloseHandle(pSGImageDescriptor->pPrivateData);
		pSGImageDescriptor->pPrivateData = NULL;
	}
	return FIO_Succeed; 
}

// SG: �õ� JPG �� BGRA ͼ��
int jpgGetImage(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OpenErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	CSingleLock singleLock(&g_CritSection);
	singleLock.Lock();

	UINT uWidth =0;
	UINT uHeight =0;
	if(NULL != pSGImageDescriptor->pPrivateData){
		 pSGImageDescriptor->pImage = JpegFile::JpegFileToBGRA(pSGImageDescriptor->pPrivateData,&uWidth,&uHeight); // SG: ֱ�ӵõ� BGRA ��ʽͼ��buf ��Ҫ�������ͷ�
		if(!pSGImageDescriptor->pImage) 
		{
			if(singleLock.IsLocked())
				singleLock.Unlock();
			return FIO_OtherErr;
		}
	}
	else{
		if(singleLock.IsLocked())
			singleLock.Unlock();
		return FIO_OtherErr;
	}
	return FIO_Succeed;
}

// ��ζ�ȡһ��ͼ��ÿ�ζ�ȡ�����У��Լ��ٶԳ���ͼ���ȡ���ڴ����� Begin
// �˺�������ͼ���ȡǰ��׼������������ȡͼ������
// pSGImageDescriptor->pImage ���ڴ��ɵ���������
int NXjpegPrepareGetImage(LPTPResource lpResource, UINT *pGrayPlatte, void **ppPrivateInfo)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	LPNXJpegBlockPrivateData pNXJpegBlockPrivateData = new NXJpegBlockPrivateData;
	pNXJpegBlockPrivateData->pSingleLock = new CSingleLock(&g_CritSection);
	pNXJpegBlockPrivateData->pSingleLock->Lock();

	UINT uWidth =0;
	UINT uHeight =0;
	if(NULL != pSGImageDescriptor->pPrivateData)
	{
		JpegFile::NXJpegPrepareGetImage(pSGImageDescriptor->pPrivateData, pNXJpegBlockPrivateData); // SG: ֱ�ӵõ� BGRA ��ʽͼ��buf ��Ҫ�������ͷ�
	}
	else
	{
		if(pNXJpegBlockPrivateData->pSingleLock->IsLocked())
			pNXJpegBlockPrivateData->pSingleLock->Unlock();
		delete pNXJpegBlockPrivateData->pSingleLock;
		delete pNXJpegBlockPrivateData;
		return FIO_OtherErr;
	}
	*ppPrivateInfo = pNXJpegBlockPrivateData;
	return FIO_Succeed;
}
// ����ȡָ��������ͼ�����ݣ���ȡ���Ϊ BGRA
// iBeginRow >= 0	& iEndRow <= iImageHeight -1 
// ��ȡ����Ϊ iEndRow - iBeginRow + 1���� [iBeginRow, iEndRow]
int NXjpegReadImageBlock(LPTPResource lpResource, int iBeginRow, int iEndRow, UINT * pGrayPlatte, void *pPrivateInfo)
{
	if(NULL == lpResource)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	
	LPNXJpegBlockPrivateData pNXJpegBlockPrivateData = (LPNXJpegBlockPrivateData)pPrivateInfo;
	
	BYTE *pDestTrv = pSGImageDescriptor->pImage; // ��ָ��
	if(iBeginRow <= 0)
	{
		int row_stride;		/* physical row width in output buffer */
		/* JSAMPLEs per row in output buffer */
		row_stride = (pNXJpegBlockPrivateData->stuJpeg_decompress_struct).output_width * (pNXJpegBlockPrivateData->stuJpeg_decompress_struct).output_components;
		/* Make a one-row-high sample array that will go away when done with image */
		pNXJpegBlockPrivateData->pBuf = (*(pNXJpegBlockPrivateData->stuJpeg_decompress_struct).mem->alloc_sarray)
			((j_common_ptr) &(pNXJpegBlockPrivateData->stuJpeg_decompress_struct), JPOOL_IMAGE, row_stride, 1);
	}
	for(; iBeginRow <= iEndRow; ++iBeginRow)
	{
		(void) jpeg_read_scanlines(&(pNXJpegBlockPrivateData->stuJpeg_decompress_struct), pNXJpegBlockPrivateData->pBuf, 1);

		if ((pNXJpegBlockPrivateData->stuJpeg_decompress_struct).out_color_components==3) 
		{

			NXRGBGetScanlineBGRAEx(pNXJpegBlockPrivateData->pBuf[0], pSGImageDescriptor->iWidth,	pDestTrv);

		} 
		else if ((pNXJpegBlockPrivateData->stuJpeg_decompress_struct).out_color_components==1)
		{
			NXGrayGetScanlieBGRAEx(pNXJpegBlockPrivateData->pBuf[0], pSGImageDescriptor->iWidth, pDestTrv);

		}
		pDestTrv += (pSGImageDescriptor->iWidth * 4); // �Ƶ���һ��
	}
	if(iEndRow + 1 >= pSGImageDescriptor->iHeight)
	{
		// ���һ������ʱ�������ƺ���

		(void) jpeg_finish_decompress(&(pNXJpegBlockPrivateData->stuJpeg_decompress_struct));
		jpeg_destroy_decompress(&(pNXJpegBlockPrivateData->stuJpeg_decompress_struct));
		if(g_buf)
		{
			delete []g_buf;
			g_buf = NULL;
		}
		if(pNXJpegBlockPrivateData->pSingleLock->IsLocked())
			pNXJpegBlockPrivateData->pSingleLock->Unlock();
		// ��� JPEG �����˽������
		delete pNXJpegBlockPrivateData->pSingleLock;
		delete pNXJpegBlockPrivateData;
	}
	
	return FIO_Succeed;
}
// ��ζ�ȡһ��ͼ��ÿ�ζ�ȡ�����У��Լ��ٶԳ���ͼ���ȡ���ڴ����� End

int jpgPutImage(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OpenErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	if(NULL == pSGImageDescriptor)
	{
		return FIO_OtherErr;
	}
	if(NULL == lpResource->ptszfileName)
	{
		return FIO_OpenErr;
	}
	HANDLE hJpgFile = CreateFile(lpResource->ptszfileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == hJpgFile)
	{
		return FIO_OpenErr;
	}
	TPJPGCustomStu sgJpgStu;

	CSingleLock singleLock(&g_CritSection);
	singleLock.Lock();

	BYTE * pBGR = new BYTE[pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight * 3];
	Convert32To24BlendAlpha(pBGR,pSGImageDescriptor->pImage, pSGImageDescriptor->iWidth , pSGImageDescriptor->iHeight);
	JpegFile::BGRFromRGB(pBGR, pSGImageDescriptor->iWidth , pSGImageDescriptor->iHeight);			
	JpegFile::RGBToJpegFile(hJpgFile, pBGR, pSGImageDescriptor->iWidth , pSGImageDescriptor->iHeight,TRUE, 24, sgJpgStu.iQuality);
	delete []pBGR;
	
	if(singleLock.IsLocked())
		singleLock.Unlock();
	CloseHandle(hJpgFile);
	return FIO_Succeed;
}

////add by ljj @ 2007-3-19 for Jpg -Quality set
int jpgGetbNeedCustomSet(TPFileFormat* pFileFormat)  //
{
	if(NULL == pFileFormat) return 0;
	return 1;
}

int jpgGetSimpleCustomWnd(TPFileFormat* pFileFormat,void* pParentWnd,void*& pWnd)
{
	if(!pFileFormat || !g_hFileSetModule) return 0;
	JPG_GetSimpleCustomWnd=	(int (*)(TPFileFormat* ,void* ,void*& ))GetProcAddress( g_hFileSetModule, "JPG_GetSimpleCustomWnd" );
	return JPG_GetSimpleCustomWnd(pFileFormat,pParentWnd,pWnd);
}

int jpgGetCustomWnd(TPFileFormat* pFileFormat,void* pParentWnd,void*& pWnd)
{
	if(!pFileFormat || !g_hFileSetModule) return 0;
	JPG_GetSimpleCustomWnd=	(int (*)(TPFileFormat* ,void* ,void*& ))GetProcAddress( g_hFileSetModule, "JPG_GetSimpleCustomWnd" );
	return JPG_GetSimpleCustomWnd(pFileFormat,pParentWnd,pWnd);
}

int jpgUpdateCustomData(TPFileFormat* pFileFormat,void* pWnd)
{
	if(!pFileFormat || !g_hFileSetModule) return 0;
	JPG_UpDataCustomData=	(int (*)(TPFileFormat* ,void* ))GetProcAddress( g_hFileSetModule, "JPG_UpDataCustomData" );
	return JPG_UpDataCustomData(pFileFormat,pWnd);
}

int jpgReleaseWnd(TPFileFormat* pFileFormat,void* pWnd)
{
	if(!pFileFormat || !g_hFileSetModule) return 0;
	JPG_ReleaseWnd=	(int (*)(TPFileFormat* ,void* ))GetProcAddress( g_hFileSetModule, "JPG_ReleaseWnd" );
	return  JPG_ReleaseWnd(pFileFormat,pWnd);
}


