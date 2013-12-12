#pragma once
#include "afxmt.h"
#include <stdio.h>
#include "jpeglib.h"
#include <windows.h>
#include <setjmp.h>
  
//#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

struct my_error_mgr 
{
	struct jpeg_error_mgr pub;	//* "public" fields 
	jmp_buf setjmp_buffer;	//* for return to caller 
};

typedef struct my_error_mgr * my_error_ptr;

// 在分块读取 JPEG 图像时，保存 LOCK 和 CINFO
typedef struct tag_NXJpegBlockPrivateData
{
	CSingleLock *pSingleLock;
	jpeg_decompress_struct stuJpeg_decompress_struct;
	JSAMPARRAY pBuf; // 存储库函数读到的图像数据行指针
	struct my_error_mgr jerr; // 
	jpeg_source_mgr jmemsrc;
	tag_NXJpegBlockPrivateData()
	{
		pSingleLock = NULL;
		pBuf = NULL;
	}
}NXJpegBlockPrivateData, *LPNXJpegBlockPrivateData;

void NXGrayGetScanlieBGRA(BYTE *pSrcLine, int iWidthPix, BYTE *pDestLine); // Gray -> BGRA
void NXRGBGetScanlineBGRA(BYTE *pSrcLine, int iWidthPix, BYTE *pDestLine); // RGB -> BGRA
class JpegFile 
{
public:

	////////////////////////////////////////////////////////////////
	// read a JPEG file to an RGB buffer - 3 bytes per pixel
	// returns a ptr to a buffer .
	// caller is responsible for cleanup!!!
	// BYTE *buf = JpegFile::JpegFileToRGB(....);
	// delete [] buf;

	static BYTE * JpegFileToRGB(HANDLE infile ,			// path to image
							   UINT *width,					// image width in pixels
							   UINT *height);				// image height
	static BYTE * JpegFileToBGRA(HANDLE infile ,			// path to image
							   UINT *width,					// image width in pixels
							   UINT *height);				// image height
	static void NXJpegPrepareGetImage(HANDLE infile, LPNXJpegBlockPrivateData pNXJpegBlockPrivateData); // 完成分块读取图像数据的准备工作，此函数并不读取图像数据

	static BOOL  GetHeadSize(HANDLE infile ,			// path to image
		                  UINT *width,					// image width in pixels
		                  UINT *height);				// image height

	////////////////////////////////////////////////////////////////
	// write a JPEG file from a 3-component, 1-byte per component buffer

	static BOOL RGBToJpegFile(HANDLE outfile,				// path
							BYTE *dataBuf,					// RGB buffer
							UINT width,						// pixels
							UINT height,					// rows
							BOOL color,						// TRUE = RGB	// FALSE = Grayscale
							int iBitsPerPixel,              
							int quality);					// 0 - 100



	


	////////////////////////////////////////////////////////////////
	// swap Red and Blue bytes
	// in-place
	
	static BOOL BGRFromRGB(BYTE *buf,							// input buf
					UINT widthPix,								// width in pixels
					UINT height);								// lines

	////////////////////////////////////////////////////////////////
	// these do nothing
	JpegFile();		// creates an empty object
	~JpegFile();	// destroys nothing
};