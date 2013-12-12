//#include "stdafx.h"
#include "stdafx.h"
#undef FAR        // gets rid of a warning
#define XMD_H     // fixes a type redefiniton issue
#include "JpegFile.h"

#include <stdio.h>
#include "jpeglib.h"

#if _MSC_VER >= 1500
#pragma comment (lib,"jpg2008.lib")
#else
#pragma comment (lib,"jpg2003.lib")
#endif


//struct my_error_mgr 
//{
//   struct jpeg_error_mgr pub;	//* "public" fields 
//   jmp_buf setjmp_buffer;	//* for return to caller 
//};
//
//typedef struct my_error_mgr * my_error_ptr;


METHODDEF(void) my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	char buffer[JMSG_LENGTH_MAX];

	/* Create the message */
	(*cinfo->err->format_message) (cinfo, buffer);
   	longjmp(myerr->setjmp_buffer, 1);
}

// store a scanline to our data buffer
void j_putRGBScanline(BYTE *jpegline, int widthPix, BYTE *outBuf, int row);
void j_putBGRAScanline(BYTE *jpegline, int widthPix, BYTE *outBuf, int row);

void j_putGrayScanlineToRGB(BYTE *jpegline,  int widthPix,BYTE *outBuf, int row);
void j_putGrayScanlineToBGRA(BYTE *jpegline,  int widthPix,BYTE *outBuf, int row);


//
//	constructor doesn't do much - there's no real class here...
//

JpegFile::JpegFile()
{

}

JpegFile::~JpegFile()
{

}

//
//	read a JPEG file
//

size_t g_buf_length = 0;
unsigned char* g_buf = NULL;

void mem_init_source(j_decompress_ptr cinfo)
{
	cinfo->src->bytes_in_buffer = g_buf_length;
	cinfo->src->next_input_byte = g_buf;
}

boolean mem_fill_input_buffer(j_decompress_ptr cinfo)
{
	return FALSE;
}

void mem_skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
	cinfo->src->bytes_in_buffer -= num_bytes;
	cinfo->src->next_input_byte += num_bytes;
}

boolean mem_resync_to_restart(j_decompress_ptr cinfo, int desired)
{
	return jpeg_resync_to_restart(cinfo, desired);
}

void mem_term_source(j_decompress_ptr cinfo)
{
}


BYTE * JpegFile::JpegFileToRGB(HANDLE infile,
							   UINT *width,
							   UINT *height)

{

	// basic code from IJG Jpeg Code v6 example.c

	*width=0;
	*height=0;

	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	/* More stuff */
	

	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
	

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
//////
	if (infile == NULL)
		return 0;
	g_buf_length = GetFileSize(infile, NULL);

	if(INVALID_FILE_SIZE == g_buf_length)
		return 0;
	g_buf = new unsigned char[g_buf_length];

	SetFilePointer(infile, 0, NULL, FILE_BEGIN);

	DWORD dwRealReadBytes = 0;
	ReadFile(infile, g_buf, g_buf_length, &dwRealReadBytes, NULL);

	jpeg_source_mgr jmemsrc;
	jmemsrc.bytes_in_buffer = g_buf_length;
	jmemsrc.next_input_byte = g_buf;
	jmemsrc.init_source = mem_init_source;
	jmemsrc.fill_input_buffer = mem_fill_input_buffer;
	jmemsrc.skip_input_data = mem_skip_input_data;
	jmemsrc.resync_to_restart = mem_resync_to_restart;
	jmemsrc.term_source = mem_term_source;  

	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer)) 
	{
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);
     cinfo.src = &jmemsrc;

	(void) jpeg_read_header(&cinfo, TRUE);
	(void) jpeg_start_decompress(&cinfo);

	BYTE *dataBuf;

	////////////////////////////////////////////////////////////
	// alloc and open our new buffer
	dataBuf=(BYTE *)new BYTE[cinfo.output_width * 3 * cinfo.output_height];
	if (dataBuf==NULL) 
	{
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}

	// how big is this thing gonna be?
	*width = cinfo.output_width;
	*height = cinfo.output_height;
	
	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	while (cinfo.output_scanline < cinfo.output_height)
	{
		
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);

		if (cinfo.out_color_components==3) 
		{
			
			j_putRGBScanline(buffer[0],	*width,	dataBuf,cinfo.output_scanline-1);

		} 
		else if (cinfo.out_color_components==1)
		{
			j_putGrayScanlineToRGB(buffer[0],*width,dataBuf,cinfo.output_scanline-1);

		}

	}

	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
    if(g_buf)
	{
		delete []g_buf;
		g_buf = NULL;
	}

	return dataBuf;
}

BYTE * JpegFile::JpegFileToBGRA(HANDLE infile,
							   UINT *width,
							   UINT *height)

{

	// basic code from IJG Jpeg Code v6 example.c

	*width=0;
	*height=0;

	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	/* More stuff */


	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */


	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	//////
	if (infile == NULL)
		return 0;
	SetFilePointer(infile, 0, NULL, FILE_BEGIN); // 
	g_buf_length = GetFileSize(infile, NULL);
	g_buf = new unsigned char[g_buf_length];
	SetFilePointer(infile, 0, NULL, FILE_BEGIN); // 
	DWORD dwRealReadBytes = 0;
	ReadFile(infile, g_buf, g_buf_length, &dwRealReadBytes, NULL);


	jpeg_source_mgr jmemsrc;
	jmemsrc.bytes_in_buffer = g_buf_length;
	jmemsrc.next_input_byte = g_buf;
	jmemsrc.init_source = mem_init_source;
	jmemsrc.fill_input_buffer = mem_fill_input_buffer;
	jmemsrc.skip_input_data = mem_skip_input_data;
	jmemsrc.resync_to_restart = mem_resync_to_restart;
	jmemsrc.term_source = mem_term_source;  

	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer)) 
	{
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);
	cinfo.src = &jmemsrc;

	(void) jpeg_read_header(&cinfo, TRUE);
	(void) jpeg_start_decompress(&cinfo);

	BYTE *dataBuf;

	////////////////////////////////////////////////////////////
	// alloc and open our new buffer
	dataBuf=(BYTE *)new BYTE[cinfo.output_width * 4 * cinfo.output_height];
	if (dataBuf==NULL) 
	{
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}

	// how big is this thing gonna be?
	*width = cinfo.output_width;
	*height = cinfo.output_height;

	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	int ilastScanline = -1;
	BOOL bFinish = TRUE;
	while (cinfo.output_scanline < cinfo.output_height)
	{

		(void) jpeg_read_scanlines(&cinfo, buffer, 1);

		if (cinfo.out_color_components==3) 
		{

			j_putBGRAScanline(buffer[0],	*width,	dataBuf,cinfo.output_scanline-1);

		} 
		else if (cinfo.out_color_components==1)
		{
			j_putGrayScanlineToBGRA(buffer[0],*width,dataBuf,cinfo.output_scanline-1);

		}
		if(ilastScanline == cinfo.output_scanline )
		{
			bFinish = FALSE;
			break;
		}
		ilastScanline = cinfo.output_scanline;
	}
	try
	{
		if(bFinish)
			(void) jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
	}
	catch(...)
	{
		ASSERT(0);
	}

	if(g_buf)
	{
		delete []g_buf;
		g_buf = NULL;
	}

	return dataBuf;
}

// 完成分块读取图像数据的准备工作 add by wgf @2010-4-21
void JpegFile::NXJpegPrepareGetImage(HANDLE infile, LPNXJpegBlockPrivateData pNXJpegBlockPrivateData)
{
	if(NULL == pNXJpegBlockPrivateData)
	{
		return ;
	}
	//struct my_error_mgr jerr;
	(pNXJpegBlockPrivateData->stuJpeg_decompress_struct).err = jpeg_std_error(&(pNXJpegBlockPrivateData->jerr).pub);
	(pNXJpegBlockPrivateData->jerr).pub.error_exit = my_error_exit;
	//////
	if (infile == NULL)
		return ;
	SetFilePointer(infile, 0, NULL, FILE_BEGIN); // 
	g_buf_length = GetFileSize(infile, NULL);
	g_buf = new unsigned char[g_buf_length];
	SetFilePointer(infile, 0, NULL, FILE_BEGIN); // 
	DWORD dwRealReadBytes = 0;
	ReadFile(infile, g_buf, g_buf_length, &dwRealReadBytes, NULL);


	//jpeg_source_mgr jmemsrc;
	(pNXJpegBlockPrivateData->jmemsrc).bytes_in_buffer = g_buf_length;
	(pNXJpegBlockPrivateData->jmemsrc).next_input_byte = g_buf;
	(pNXJpegBlockPrivateData->jmemsrc).init_source = mem_init_source;
	(pNXJpegBlockPrivateData->jmemsrc).fill_input_buffer = mem_fill_input_buffer;
	(pNXJpegBlockPrivateData->jmemsrc).skip_input_data = mem_skip_input_data;
	(pNXJpegBlockPrivateData->jmemsrc).resync_to_restart = mem_resync_to_restart;
	(pNXJpegBlockPrivateData->jmemsrc).term_source = mem_term_source;  

	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp((pNXJpegBlockPrivateData->jerr).setjmp_buffer)) 
	{
		jpeg_destroy_decompress(&(pNXJpegBlockPrivateData->stuJpeg_decompress_struct));
		return ;
	}

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&(pNXJpegBlockPrivateData->stuJpeg_decompress_struct));
	(pNXJpegBlockPrivateData->stuJpeg_decompress_struct).src = &(pNXJpegBlockPrivateData->jmemsrc);

	(void) jpeg_read_header(&(pNXJpegBlockPrivateData->stuJpeg_decompress_struct), TRUE);
	(void) jpeg_start_decompress(&(pNXJpegBlockPrivateData->stuJpeg_decompress_struct));


}

///返回width和height
 BOOL  JpegFile::GetHeadSize(HANDLE infile , UINT *width,UINT *height)
 {

	 *width=0;
	 *height=0; 
	 struct jpeg_decompress_struct cinfo; 
	 struct my_error_mgr jerr;
	 
	 cinfo.err = jpeg_std_error(&jerr.pub);
	 jerr.pub.error_exit = my_error_exit;
	 //////
	 if (infile == NULL)
		 return 0;

	g_buf_length = GetFileSize(infile, NULL);
	 g_buf = new unsigned char[g_buf_length];
	 SetFilePointer(infile, 0, NULL, FILE_BEGIN); // 
	 DWORD dwRealReadBytes = 0;
	 ReadFile(infile, g_buf, g_buf_length, &dwRealReadBytes, NULL);

	 jpeg_source_mgr jmemsrc;

	 jmemsrc.bytes_in_buffer = g_buf_length;
	 jmemsrc.next_input_byte = g_buf;
	 jmemsrc.init_source = mem_init_source;
	 jmemsrc.fill_input_buffer = mem_fill_input_buffer;
	 jmemsrc.skip_input_data = mem_skip_input_data;
	 jmemsrc.resync_to_restart = mem_resync_to_restart;
	 jmemsrc.term_source = mem_term_source;  

	 
	 if (setjmp(jerr.setjmp_buffer))
	 { 
		 jpeg_destroy_decompress(&cinfo);

		 if(g_buf)
		 {
			 delete []g_buf;
			 g_buf = NULL;
		 }
		 return NULL;
	 }

	 
	 jpeg_create_decompress(&cinfo);
	 cinfo.src = &jmemsrc;

	 (void) jpeg_read_header(&cinfo, TRUE);
	 *width = cinfo.image_width;
	 *height = cinfo.image_height;

	 jpeg_destroy_decompress(&cinfo);

	 if(g_buf)
	 {
		 delete g_buf;
		 g_buf = NULL;
	 }

	 return TRUE;
 }


//
//
//
size_t  g_writebuf_length;
size_t  g_RealLen;
unsigned char* g_write_buf = NULL;
void mem_init_destination(j_compress_ptr cinfo)
{
	cinfo->dest->free_in_buffer = g_writebuf_length;
	cinfo->dest->next_output_byte = g_write_buf;
}

boolean mem_empty_output_buffer(j_compress_ptr cinfo)
{
	return true;
}

void mem_term_destination(j_compress_ptr cinfo)
{
	jpeg_destination_mgr *dmgr = cinfo->dest;
	g_RealLen = g_writebuf_length - dmgr->free_in_buffer;
	dmgr->free_in_buffer = 0;
}

BOOL JpegFile::RGBToJpegFile(HANDLE outfile, 
							BYTE *dataBuf,
							UINT widthPix,
							UINT height,
							BOOL color, 
							int iBitsPerPixel,
							int quality)
{

	g_RealLen =0;
	g_writebuf_length = (widthPix * height *iBitsPerPixel)/8;
	g_write_buf = new unsigned char[g_writebuf_length];

	jpeg_destination_mgr jmemdest;
	jmemdest.free_in_buffer = g_writebuf_length;
	jmemdest.next_output_byte = g_write_buf;
	jmemdest.init_destination = mem_init_destination;
	jmemdest.empty_output_buffer = mem_empty_output_buffer;
	jmemdest.term_destination = mem_term_destination;


	LPBYTE tmp;
	if (!color)
	{
		tmp = (BYTE*)new BYTE[widthPix*height];
		if (tmp==NULL) 
			return FALSE;

		UINT row,col;
		for (row=0;row<height;row++) 
		{
			for (col=0;col<widthPix;col++) 
			{
				LPBYTE pRed, pGrn, pBlu;
				pRed = dataBuf + row * widthPix * 3 + col * 3;
				pGrn = dataBuf + row * widthPix * 3 + col * 3 + 1;
				pBlu = dataBuf + row * widthPix * 3 + col * 3 + 2;

				// luminance
				int lum = (int)(.299 * (double)(*pRed) + .587 * (double)(*pGrn) + .114 * (double)(*pBlu));
				LPBYTE pGray;
				pGray = tmp + row * widthPix + col;
				*pGray = (BYTE)lum;
			}
		}
	}

	struct jpeg_compress_struct cinfo;
	
	int row_stride;			

	struct my_error_mgr jerr;

	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	
	if (setjmp(jerr.setjmp_buffer))
	{

		jpeg_destroy_compress(&cinfo); 

		if (!color) 
		{
			delete [] tmp;
		}
		return FALSE;
	}


	jpeg_create_compress(&cinfo);
    cinfo.dest = &jmemdest;

	//jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = widthPix; 	
	cinfo.image_height = height;
	if (color) 
	{
		cinfo.input_components = 3;	
		cinfo.in_color_space = JCS_RGB; 	
	} 
	else 
	{
		cinfo.input_components = 1;	
		cinfo.in_color_space = JCS_GRAYSCALE;
	}


  jpeg_set_defaults(&cinfo);
  
  jpeg_set_quality(&cinfo, quality, TRUE );

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = widthPix * 3;	
 JSAMPROW row_pointer[1];
  while (cinfo.next_scanline < cinfo.image_height) 
  {
		//LPBYTE outRow;
		if (color) 
		{
			row_pointer[0] = dataBuf + (cinfo.next_scanline * widthPix * 3);
		} 
		else
		{
			row_pointer[0] = tmp + (cinfo.next_scanline * widthPix);
		}

		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1); 
  }


  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  DWORD dwRealReadBytes = 0;
  WriteFile(outfile, g_write_buf, g_RealLen, &dwRealReadBytes, NULL);

  if(g_write_buf)
  {
	  delete []g_write_buf;
	  g_write_buf = NULL;
  }
  if (!color)
	  delete [] tmp;
 
 
  return TRUE;
}

//
//	stash a scanline
//
 
void j_putRGBScanline(BYTE *jpegline, 
					 int widthPix,
					 BYTE *outBuf,
					 int row)
{
	int offset = row * widthPix * 3;
	int count;
	for (count=0;count<widthPix;count++) 
	{
		*(outBuf + offset + count * 3 + 0) = *(jpegline + count * 3 + 0);
		*(outBuf + offset + count * 3 + 1) = *(jpegline + count * 3 + 1);
		*(outBuf + offset + count * 3 + 2) = *(jpegline + count * 3 + 2);
	}
}

void j_putBGRAScanline(BYTE *jpegline, 
					  int widthPix,
					  BYTE *outBuf,
					  int row)
{
	int offset = row * widthPix * 4;
	int count;
	for (count=0;count<widthPix;count++) 
	{
		*(outBuf + offset + count * 4 + 0) = *(jpegline + count * 3 + 2);
		*(outBuf + offset + count * 4 + 1) = *(jpegline + count * 3 + 1);
		*(outBuf + offset + count * 4 + 2) = *(jpegline + count * 3 + 0);
		*(outBuf + offset + count * 4 + 3) = 0xff;
	}
}

// 将一行的 RGB 像素转换为 BGRA
void NXRGBGetScanlineBGRA(BYTE *pSrcLine, int iWidthPix, BYTE *pDestLine)
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
//
//	stash a gray scanline
//

void j_putGrayScanlineToRGB(BYTE *jpegline, 
							 int widthPix,
							 BYTE *outBuf,
							 int row)
{
	int offset = row * widthPix * 3;
	int count;
	for (count=0;count<widthPix;count++) {

		BYTE iGray;

		// get our grayscale value
		iGray = *(jpegline + count);

		*(outBuf + offset + count * 3 + 0) = iGray;
		*(outBuf + offset + count * 3 + 1) = iGray;
		*(outBuf + offset + count * 3 + 2) = iGray;
	}
}

void j_putGrayScanlineToBGRA(BYTE *jpegline, 
							 int widthPix,
							 BYTE *outBuf,
							 int row)
{
	int offset = row * widthPix * 4;
	int count;
	for (count=0;count<widthPix;count++) {

		BYTE iGray;

		// get our grayscale value
		iGray = *(jpegline + count);

		memset(outBuf+offset + count*4 , iGray,3);
		memset(outBuf+offset + count*4 +3, 0xff,1);
	}
}

// 将一行纯灰度图像数据转换为 BGRA 图像数据
void NXGrayGetScanlieBGRA(BYTE *pSrcLine, int iWidthPix, BYTE *pDestLine)
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
  


BOOL JpegFile::BGRFromRGB(BYTE *buf, UINT widthPix, UINT height)
{
	if (buf==NULL)
		return FALSE;

	UINT col, row;
	for (row=0;row<height;row++)
	{
		for (col=0;col<widthPix;col++)
		{
			LPBYTE pRed, pGrn, pBlu;
			pRed = buf + row * widthPix * 3 + col * 3;
			pGrn = buf + row * widthPix * 3 + col * 3 + 1;
			pBlu = buf + row * widthPix * 3 + col * 3 + 2;

			// swap red and blue
			BYTE tmp;
			tmp = *pRed;
			*pRed = *pBlu;
			*pBlu = tmp;
		}
	}
	return TRUE;
}




