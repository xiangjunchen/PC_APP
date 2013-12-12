#pragma once

// CTPImgPreviewWnd message handlers
class AFX_EXT_CLASS CTPImgDecode
{
public:
	CTPImgDecode();
	~CTPImgDecode();
	// JPG

	int Decode(CString sFileNmae, BYTE *&pBuffer, int &iWidth, int &iHeight);
	// 	int jpgOpen(LPTPResource lpResource);
	// 	int jpgClose(SGImageDescriptor *pSGImageDescriptor);
	// 	int jpgGetImage(LPTPResource lpResource);
	// 	int jpgPutImage(LPTPResource lpResource);
	// 
	// 	int jpgGetbNeedCustomSet(TPFileFormat* pFileFormat);  //
	// 	int jpgGetSimpleCustomWnd(TPFileFormat* pFileFormat,void* pParentWnd,void*& pWnd);
	// 	int jpgGetCustomWnd(TPFileFormat* pFileFormat,void* pParentWnd,void*& pWnd);
	// 	int jpgUpdateCustomData(TPFileFormat* pFileFormat,void* pWnd);
	// 	int jpgReleaseWnd(TPFileFormat* pFileFormat,void* pWnd);
	// 
	// 	// 分块读取图像数据 Begin
	// 	int NXjpegPrepareGetImage(LPTPResource lpResource, UINT *pGrayPlatte, void **ppPrivateInfo);
	// 	int NXjpegReadImageBlock(LPTPResource lpResource, int iBeginRow, int iEndRow, UINT *pGrayPlatte, void *pPrivateInfo);
	// 	// 分块读取图像数据 End

};
