#include "StdAfx.h"
#include "TPCommonCtrlImgApi.h"
#include <mmsystem.h>
#include <aviriff.h>

namespace _tp_commonctrl
{

typedef union union_RGBBIT { 
	RGBQUAD RGBbits;
	DWORD	RGBdword;
} RGBBIT;
 void	TP_FastMemset(LPDWORD	lpdwDes,DWORD	dwColor,int	nSize)		
{
	_asm
	{	
		
		   mov			edi,	lpdwDes
			mov			eax,	dwColor
			mov			ecx,	nSize
			cmp			ecx,	16
			ja			__SSESET
			rep			stosd	
			jmp 		__RETURN
__SSESET:
		test		edi,	0xf
			jz			__PARSSET
			mov			edx,	edi
			not			edx
			inc			edx
			and			edx,	0xf
			shr			edx,	2
			sub			ecx,	edx
__HEADERLOOP:
		mov			[edi],	eax
			add			edi,	4
			dec			edx
			ja			__HEADERLOOP
__PARSSET:
		mov			edx,	ecx
			and			edx,	0x3
			shr			ecx,	2
			jz			__REMAINDER
			movd		xmm3,	eax
			pshufd		xmm3,	xmm3,	0
__LOOP:
		movntdq		[edi],	xmm3
			add			edi,	16
			dec			ecx
			ja			__LOOP
__REMAINDER:
		cmp			edx,	0
			jz			__RETURN
			mov			ecx,	edx
			rep			stosd

__RETURN:
		emms
	}
	return;
}
void    TP_SetDIBitsToDC(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn)
{
	if(pBuffer == NULL)  return;

	BITMAPINFO bpi;
	bpi.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
	bpi.bmiHeader.biPlanes       = 1;
	bpi.bmiHeader.biBitCount     = 32;
	bpi.bmiHeader.biCompression  = BI_RGB;
	bpi.bmiHeader.biSizeImage    = 0; 
	bpi.bmiHeader.biClrUsed      = 0; 
	bpi.bmiHeader.biClrImportant = 0;
	bpi.bmiHeader.biWidth        = iSrcWidth;
	bpi.bmiHeader.biHeight       = bOverTurn  ? -iSrcHeight : iSrcHeight;	
	SetDIBitsToDevice(
		pDC->m_hDC,
		rtDest.left   , rtDest.top ,
		rtDest.Width(), rtDest.Height(), 
		0,0 ,
		0 , iSrcHeight, 
		pBuffer, 
		&bpi, 
		DIB_RGB_COLORS
		);	
}

BOOL	TP_ReadBitmapDate(LPDWORD	pdbuf,CFile *fileRead,RGBQUAD	rgbPalette[256],DWORD dwWidth,DWORD	dwHeight,WORD	dwColorDepth,	DWORD	dwCompressType);
BOOL  	TP_ReadPaletteData(CFile *pReadFile,int	biClrused,int biBitCount,RGBQUAD	palette[256]);

#ifdef  TP_RESOURCELOAD
extern	CStringArray g_aStr;
#endif

BOOL	TP_LoadBitmap(LPCTSTR	sFileName,LPDWORD &lpdwBuffer,BITMAP &pBitmap)
{
#ifdef  TP_RESOURCELOAD
	g_aStr.Add(sFileName);
#endif

	BITMAPINFO			stuBitmapInfo;
	BITMAPFILEHEADER	stuBitmapHeader;
	RGBQUAD	 			rgbPalette[256];

	ZeroMemory(&pBitmap,sizeof(BITMAP));

	lpdwBuffer       = NULL;
	pBitmap.bmWidth  = 0;
	pBitmap.bmHeight = 0;		


	CFile fileRead;
	CFileException e;

	if (!fileRead.Open(sFileName, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone,&e))
	{
#ifdef _DEBUG
		if(PathFileExists(sFileName)) 
		{
			TP_ShowErrorMsg();
			AfxMessageBox(e.m_cause);
		}
#endif
		return FALSE;
	}
	try
	{
		// 读文件错误
		if (fileRead.Read(&stuBitmapHeader, sizeof(stuBitmapHeader)) != sizeof(stuBitmapHeader))
		{
			fileRead.Close();
			return FALSE;
		}

		if(fileRead.Read(&stuBitmapInfo.bmiHeader,sizeof(stuBitmapInfo.bmiHeader)) != sizeof(stuBitmapInfo.bmiHeader))
		{
			fileRead.Close();
			return FALSE;
		}
	}
	catch(...)
	{
		fileRead.Close();
		return FALSE;
	}


	// 不是BMP文件
	if (stuBitmapHeader.bfType != 0x4D42)
	{
		fileRead.Close();
		return FALSE;
	}
	// 读调色板错误
	if (!TP_ReadPaletteData(&fileRead,stuBitmapInfo.bmiHeader.biClrUsed,stuBitmapInfo.bmiHeader.biBitCount,rgbPalette))
	{
		fileRead.Close();
		return FALSE;
	}


	LPDWORD lpdwImage = NULL;
	lpdwImage = new DWORD[stuBitmapInfo.bmiHeader.biWidth * stuBitmapInfo.bmiHeader.biHeight];

	if (!TP_ReadBitmapDate(lpdwImage,&fileRead,
		rgbPalette,
		stuBitmapInfo.bmiHeader.biWidth,
		stuBitmapInfo.bmiHeader.biHeight,
		stuBitmapInfo.bmiHeader.biBitCount,
		stuBitmapInfo.bmiHeader.biCompression))
	{
		delete [] lpdwImage;
		fileRead.Close();
		return FALSE;
	}
	lpdwBuffer = lpdwImage;
	pBitmap.bmWidth      = stuBitmapInfo.bmiHeader.biWidth;
	pBitmap.bmHeight     = stuBitmapInfo.bmiHeader.biHeight;		
	pBitmap.bmBitsPixel  = 32;//stuBitmapInfo.bmiHeader.biBitCount;
	pBitmap.bmPlanes     = stuBitmapInfo.bmiHeader.biPlanes;
	pBitmap.bmWidthBytes = pBitmap.bmWidth * pBitmap.bmBitsPixel/8;
	pBitmap.bmType       = 0;

	fileRead.Close();
	return TRUE;
}
HBITMAP	TP_LoadImageBitmapFile(LPCTSTR	sFileName)
{
	LPDWORD lpdwBuffer;
	BITMAP pBitmap;
	ZeroMemory(&pBitmap,sizeof(BITMAP));
	lpdwBuffer = NULL;
	if(!TP_LoadBitmap(sFileName,lpdwBuffer,pBitmap)) return NULL;
	pBitmap.bmBits       = lpdwBuffer;	
	pBitmap.bmPlanes     = 1;
	pBitmap.bmBitsPixel  = 32;
	
	TP_SetDIBitsOverTurn((LPBYTE)lpdwBuffer,pBitmap.bmWidth,pBitmap.bmHeight);
	for(INT l=0;l<pBitmap.bmWidth*pBitmap.bmHeight;l++) ((LPBYTE)lpdwBuffer)[l*4+3] = 0;
	HBITMAP hBmp =CreateBitmap(pBitmap.bmWidth,pBitmap.bmHeight,pBitmap.bmPlanes,pBitmap.bmBitsPixel,lpdwBuffer);	
	if(lpdwBuffer) delete []lpdwBuffer;
	return hBmp;
}
BOOL  		TP_ReadPaletteData(CFile *pReadFile,int	biClrused,int biBitCount,RGBQUAD	palette[256])
{
	if(biBitCount==1)
	{
		pReadFile->Read(palette,2*sizeof(RGBQUAD));
	}
	else if(biBitCount==4)
	{
		int		count;
		if(biClrused==0)
			count=16;
		else
			count=biClrused;

		if(pReadFile->Read(palette,count*sizeof(RGBQUAD))!=count*sizeof(RGBQUAD))
			return FALSE;
	}
	else if(biBitCount==8)//8位深度色
	{
		int		count;
		if(biClrused==0)
			count=256;
		else
			count=biClrused;

		if(pReadFile->Read(palette,count*sizeof(RGBQUAD))!=count*sizeof(RGBQUAD))
			return FALSE;
	}
	return TRUE;

}
BOOL		TP_ReadBitmapDate(LPDWORD	pdbuf,CFile *fileRead,RGBQUAD	rgbPalette[256],DWORD dwWidth,DWORD	dwHeight,WORD	dwColorDepth,	DWORD	dwCompressType)
{
	BYTE	*pBbuf=NULL;
	DWORD	dwidth=0;
	DWORD	AllHeight=0;
	DWORD	AllWidth=0;

	RGBBIT  bit_color;
	switch(dwColorDepth)
	{
	case 1:
		{
			DWORD	BitCount;
			DWORD	DIndex=0;
			DWORD	WidthCount;
			if(dwWidth%8!=0)
			{	
				WidthCount	=dwWidth/8+1;
				WidthCount	=WidthCount+(WidthCount%4==0?0:(4-WidthCount%4));
				BitCount	=WidthCount*dwHeight;
			}
			else
			{
				WidthCount	=dwWidth/8;
				WidthCount	=WidthCount+(WidthCount%4==0?0:(4-WidthCount%4));
				BitCount	=WidthCount*dwHeight;
			}
			pBbuf=new BYTE[BitCount];
			try
			{
				if(fileRead->Read(pBbuf,BitCount)!=BitCount)
				{
					delete [] pBbuf;
					return FALSE;
				}
			}
			catch(...)
			{
				delete [] pBbuf;
				return FALSE;
			}

			for(DWORD h=0;h<dwHeight;h++)
			{
				DIndex=0;
				DWORD h1=(dwHeight - 1 - h)*dwWidth;
				for(DWORD i=0;i<WidthCount;i++)
				{
					DWORD	i1=h1+i*8;
					BYTE	CurBit=pBbuf[h*WidthCount+i];
					for(DWORD j=0;j<8;j++)
					{
						if(DIndex>=dwWidth)		goto LOOP;
						BYTE CurColor=CurBit&0x80;
						if(CurColor==0x80)
						{
							bit_color.RGBbits.rgbBlue	=rgbPalette[1].rgbBlue;
							bit_color.RGBbits.rgbGreen	=rgbPalette[1].rgbGreen;
							bit_color.RGBbits.rgbRed	=rgbPalette[1].rgbRed;
							bit_color.RGBbits.rgbReserved=0xff;
							pdbuf[i1+j]=bit_color.RGBdword;
						}
						else if(CurColor==0)
						{
							bit_color.RGBbits.rgbBlue	=rgbPalette[0].rgbBlue;
							bit_color.RGBbits.rgbGreen	=rgbPalette[0].rgbGreen;
							bit_color.RGBbits.rgbRed	=rgbPalette[0].rgbRed;
							bit_color.RGBbits.rgbReserved=0xff;
							pdbuf[i1+j]=bit_color.RGBdword;
						}
						CurBit=CurBit<<1;
						DIndex=DIndex+1;
					}
				}
LOOP:			;
			}
			delete [] pBbuf;
		}
		break;
	case 4:
		{
			DWORD dWidth;
			switch(dwCompressType)
			{
			case BI_BITFIELDS:
			case BI_RGB:
				{
					DWORD theWidth;
					if((dwWidth*4)%8==0)
					{
						theWidth=dwWidth*4/8;
					}
					else
					{
						theWidth=dwWidth*4/8+1;
					}
					if((dwWidth*4)%32==0)
					{
						dWidth=dwWidth*4/8;
					}
					else
					{
						dWidth=theWidth+4-theWidth%4;
					}

					pBbuf	=new	BYTE[dWidth*dwHeight];
					try
					{
						if(dWidth*dwHeight!=fileRead->Read(pBbuf,dWidth*dwHeight))
						{
							delete [] pBbuf;
							return FALSE;
						}
					}
					catch(...)
					{
						delete [] pBbuf;
						return FALSE;
					}				

					DWORD LineCount=0;
					for(DWORD row=0;row<dwHeight;row++)
					{
						LineCount=0;
						DWORD	CurLine=row*dWidth;
						DWORD	CurLine1=(dwHeight - 1 - row) * dwWidth;
						for(DWORD col=0;col<theWidth;col++)
						{
							BYTE	first	=pBbuf[CurLine+col]&0x0f;
							BYTE	sencond=(pBbuf[CurLine+col]>>4)&0x0f;
							bit_color.RGBbits.rgbBlue	=rgbPalette[first].rgbBlue;
							bit_color.RGBbits.rgbGreen	=rgbPalette[first].rgbGreen;
							bit_color.RGBbits.rgbRed	=rgbPalette[first].rgbRed;
							bit_color.RGBbits.rgbReserved=0xff;
							pdbuf[CurLine1+LineCount]=bit_color.RGBdword;
							if(LineCount<dwWidth-1)
								LineCount++;
							else
								break;
							bit_color.RGBbits.rgbBlue	=rgbPalette[sencond].rgbBlue;
							bit_color.RGBbits.rgbGreen	=rgbPalette[sencond].rgbGreen;
							bit_color.RGBbits.rgbRed	=rgbPalette[sencond].rgbRed;
							bit_color.RGBbits.rgbReserved=0xff;
							pdbuf[CurLine1+LineCount]=bit_color.RGBdword;
							if(LineCount<dwWidth-1)
								LineCount++;
							else	break;
						}
					}
					delete [] pBbuf;
				}
				break;
			case BI_RLE4:
				{
					DWORD	number=0;
					DWORD	count= (DWORD)(fileRead->GetLength()-fileRead->GetPosition( ));
					pBbuf=new BYTE[count];
					try
					{
						if(fileRead->Read(pBbuf,count)!=count)
						{
							delete [] pBbuf;
							return FALSE;
						}
					}
					catch(...)
					{
						delete [] pBbuf;
						return FALSE;
					}
					DWORD	i=0;
					DWORD	k=0;
					DWORD	OfFlag=0;
					DWORD	Byte=0;
					DWORD	Row=0;
					DWORD	dwBufSize	= dwWidth * dwHeight;
					LPDWORD lpdwTmpBuf	= new DWORD[dwBufSize];
					while (i < count && k < dwBufSize)
					{
						if(i%2!=0)
						{
							delete [] pBbuf;
							delete lpdwTmpBuf;
							return FALSE;
						}
						number=pBbuf[i]&0x000000ff;
						if(number==0x00)
						{
							i=i+1;
							DWORD UnReplace=pBbuf[i];
							UnReplace=UnReplace&0x000000ff;
							if(UnReplace==0x00)
							{

								while(Byte+1<dwWidth)
								{
									bit_color.RGBbits.rgbBlue	=rgbPalette[0].rgbBlue;
									bit_color.RGBbits.rgbGreen	=rgbPalette[0].rgbGreen;
									bit_color.RGBbits.rgbRed	=rgbPalette[0].rgbRed;
									bit_color.RGBbits.rgbReserved=0xff;
									lpdwTmpBuf[k]=bit_color.RGBdword;

									k++;
									Byte=Byte+1;
								}
								i++;
								Byte=0;
								continue;
							}
							else if(UnReplace==0x01)
							{
								break;
							}
							else if(UnReplace==0x02)
							{
								DWORD setcount=(pBbuf[i+2]*dwWidth+pBbuf[i+1])&0x000000ff;
								for(DWORD l=0;l<setcount;l++)
								{
									bit_color.RGBbits.rgbBlue	=rgbPalette[0].rgbBlue;
									bit_color.RGBbits.rgbGreen	=rgbPalette[0].rgbGreen;
									bit_color.RGBbits.rgbRed	=rgbPalette[0].rgbRed;
									bit_color.RGBbits.rgbReserved=0xff;
									lpdwTmpBuf[k]=bit_color.RGBdword;
									k++;
								}
								Byte=Byte+pBbuf[i+1];
								i=i+3;
								continue;
							}
							i=i+1;
							if(UnReplace%2==0)
								UnReplace=UnReplace/2;
							else
								UnReplace=UnReplace/2+1;
							for(DWORD l=0;l<UnReplace;l++)
							{
								BYTE first=pBbuf[i+l]&0x0f;
								BYTE sencond=(pBbuf[i+l]>>4)&0x0f;
								bit_color.RGBbits.rgbBlue	=rgbPalette[first].rgbBlue;
								bit_color.RGBbits.rgbGreen	=rgbPalette[first].rgbGreen;
								bit_color.RGBbits.rgbRed	=rgbPalette[first].rgbRed;
								bit_color.RGBbits.rgbReserved=0xff;
								lpdwTmpBuf[k]=bit_color.RGBdword;
								k++;
								if(Byte+1<dwWidth)
									Byte=Byte+1;
								else
								{
									break;
								}

								bit_color.RGBbits.rgbBlue	=rgbPalette[sencond].rgbBlue;
								bit_color.RGBbits.rgbGreen	=rgbPalette[sencond].rgbGreen;
								bit_color.RGBbits.rgbRed	=rgbPalette[sencond].rgbRed;
								bit_color.RGBbits.rgbReserved=0xff;
								lpdwTmpBuf[k]=bit_color.RGBdword;
								k++;
								if(Byte+1<dwWidth)
									Byte=Byte+1;
								else
								{
									break;
								}
							}
						}
						else
						{
							i=i+1;
							if(number%2==0)
								number=number/2;
							else
								number=number/2+1;
							BYTE first=pBbuf[i]&0x0f;
							BYTE sencond=(pBbuf[i]>>4)&0x0f;
							bit_color.RGBbits.rgbBlue	=rgbPalette[first].rgbBlue;
							bit_color.RGBbits.rgbGreen	=rgbPalette[first].rgbGreen;
							bit_color.RGBbits.rgbRed	=rgbPalette[first].rgbRed;
							bit_color.RGBbits.rgbReserved=0xff;
							DWORD bitfirst=(DWORD )bit_color.RGBdword;

							bit_color.RGBbits.rgbBlue	=rgbPalette[sencond].rgbBlue;
							bit_color.RGBbits.rgbGreen	=rgbPalette[sencond].rgbGreen;
							bit_color.RGBbits.rgbRed	=rgbPalette[sencond].rgbRed;
							bit_color.RGBbits.rgbReserved=0xff;
							DWORD bitsencond=(DWORD )bit_color.RGBdword;

							for(DWORD l=0;l<number;l++)
							{
								lpdwTmpBuf[k]=bitfirst;
								k++;
								if(Byte+1<dwWidth)
									Byte=Byte+1;
								else
								{

									break;
								}
								lpdwTmpBuf[k]=bitsencond;
								k++;
								if(Byte+1<dwWidth)
									Byte=Byte+1;
								else
								{

									break;
								}
							}
							i=i+1;
						}
					}
					if (k < dwBufSize)	TP_FastMemset(lpdwTmpBuf + k, 0xff000000, dwBufSize - k);				
					//					MirrorBuffer(pdbuf, dwWidth, dwHeight, lpdwTmpBuf, dwWidth, dwHeight);
				}
				break;
			}
			delete [] pBbuf;
		}
		break;
	case 8:

		dwidth=((dwWidth)%4!=0)?(4-(dwWidth)%4):0;
		switch(dwCompressType)
		{
		case BI_BITFIELDS:
		case BI_RGB:
			{
				pBbuf=new BYTE[(dwWidth+dwidth)*dwHeight];
				try
				{
					if (fileRead->Read(pBbuf,(dwWidth+dwidth)*dwHeight) != (dwWidth+dwidth)*dwHeight)
					{
						delete [] pBbuf;
						return FALSE;
					}
				}
				catch(...)
				{
					delete [] pBbuf;
					return FALSE;
				}

				AllWidth=dwWidth+dwidth;
				AllHeight=dwHeight;
				for(DWORD k=0;k<dwHeight;k++)
				{
					DWORD CurLine=k*AllWidth;
					DWORD CurLine1= k * dwWidth;
					for(DWORD i=0;i<dwWidth;i++)
					{
						DWORD CurBytes=CurLine+i;
						bit_color.RGBbits.rgbBlue	=rgbPalette[pBbuf[CurBytes]].rgbBlue;
						bit_color.RGBbits.rgbGreen	=rgbPalette[pBbuf[CurBytes]].rgbGreen;
						bit_color.RGBbits.rgbRed	=rgbPalette[pBbuf[CurBytes]].rgbRed;
						bit_color.RGBbits.rgbReserved=0xff;
						pdbuf[CurLine1+i]=(DWORD )bit_color.RGBdword;
					}
				}
				delete [] pBbuf;
			}
			break;
			//压缩方式START
		case BI_RLE8:
			{
				DWORD number=0;
				DWORD count=(DWORD)(fileRead->GetLength()-fileRead->GetPosition( ));
				pBbuf=new BYTE[count];
				try
				{
					if (fileRead->Read(pBbuf,count) != count)
					{
						delete [] pBbuf;
						return FALSE;
					}
				}
				catch(...)
				{
					delete [] pBbuf;
					return FALSE;
				}

				DWORD i=0;
				DWORD k=0;
				DWORD OfFlag=0;
				DWORD Byte=0;
				DWORD Row=0;
				DWORD	dwBufSize	= dwWidth * dwHeight;
				LPDWORD lpdwTmpBuf	= new DWORD[dwBufSize];
				while (i < count && k < dwBufSize)
				{
					if(i%2!=0)
					{
						delete [] pBbuf;
						delete lpdwTmpBuf;
						return FALSE;
					}
					number=pBbuf[i]&0x000000ff;
					if(number==0x00)
					{
						i=i+1;
						DWORD UnReplace=pBbuf[i];
						UnReplace=UnReplace&0x000000ff;
						if(UnReplace==0x00)
						{
							bit_color.RGBbits.rgbBlue	=rgbPalette[0].rgbBlue;
							bit_color.RGBbits.rgbGreen	=rgbPalette[0].rgbGreen;
							bit_color.RGBbits.rgbRed	=rgbPalette[0].rgbRed;
							bit_color.RGBbits.rgbReserved=0xff;
							while(Byte<dwWidth)
							{
								lpdwTmpBuf[k]=bit_color.RGBdword;
								k++;
								Byte=Byte+1;
							}
							i++;
							Byte=0;
							continue;
						}
						else if(UnReplace==0x01)
						{
							break;
						}
						else if(UnReplace==0x02)
						{
							DWORD	setcount=(pBbuf[i+2]*dwWidth+pBbuf[i+1])&0x000000ff;
							bit_color.RGBbits.rgbBlue	=rgbPalette[0].rgbBlue;
							bit_color.RGBbits.rgbGreen	=rgbPalette[0].rgbGreen;
							bit_color.RGBbits.rgbRed	=rgbPalette[0].rgbRed;
							bit_color.RGBbits.rgbReserved=0xff;
							for(DWORD l=0;l<setcount;l++)
							{
								lpdwTmpBuf[k]=bit_color.RGBdword;
								k++;
							}
							Byte=Byte+pBbuf[i+1];
							i=i+3;
							continue;
						}
						i=i+1;
						for(DWORD l=0;l<UnReplace;l++)
						{
							DWORD CurBytes=i+l;
							bit_color.RGBbits.rgbBlue	=rgbPalette[pBbuf[CurBytes]].rgbBlue;
							bit_color.RGBbits.rgbGreen	=rgbPalette[pBbuf[CurBytes]].rgbGreen;
							bit_color.RGBbits.rgbRed	=rgbPalette[pBbuf[CurBytes]].rgbRed;
							bit_color.RGBbits.rgbReserved=0xff;
							lpdwTmpBuf[k]=bit_color.RGBdword;
							k++;
							Byte=Byte+1;
						}
						if(UnReplace%2!=0)
						{
							i=i+UnReplace+1;
						}
						else
						{
							i=i+UnReplace;
						}
					}
					else
					{
						i=i+1;
						bit_color.RGBbits.rgbBlue	=rgbPalette[pBbuf[i]].rgbBlue;
						bit_color.RGBbits.rgbGreen	=rgbPalette[pBbuf[i]].rgbGreen;
						bit_color.RGBbits.rgbRed	=rgbPalette[pBbuf[i]].rgbRed;
						bit_color.RGBbits.rgbReserved=0xff;
						for(DWORD l=0;l<number;l++)
						{
							lpdwTmpBuf[k]=bit_color.RGBdword;
							k++;
							Byte=Byte+1;
						}
						i=i+1;
					}
				}
				if (k < dwBufSize)	TP_FastMemset(lpdwTmpBuf + k, 0xff000000, dwBufSize - k);				
				//				MirrorBuffer(pdbuf, dwWidth, dwHeight, lpdwTmpBuf, dwWidth, dwHeight);

				delete [] pBbuf;
			}

			break;
		}
		//压缩方式END
		break;
	case 24:
		{
			dwidth	=((3*dwWidth)%4!=0)?(4-(3*dwWidth)%4):0;
			pBbuf	=new BYTE[(4*dwWidth+dwidth)*dwHeight];
			try
			{
				if (fileRead->Read(pBbuf,(3*dwWidth+dwidth)*dwHeight) != (3*dwWidth+dwidth)*dwHeight)
				{
					delete [] pBbuf;
					return FALSE;
				}
			}
			catch(...)
			{
				delete [] pBbuf;
				return FALSE;
			}		
			for(DWORD k=0;k<dwHeight;k++)
			{
				DWORD	CurLine = (dwWidth *3 +dwidth)* k;
				DWORD	CurLine1= dwWidth * k;
				for(DWORD i=0;i<dwWidth;i++)
				{
					DWORD CurBytes=CurLine+3*i;
					bit_color.RGBbits.rgbBlue	=pBbuf[CurBytes];
					bit_color.RGBbits.rgbGreen	=pBbuf[CurBytes+1];
					bit_color.RGBbits.rgbRed	=pBbuf[CurBytes+2];
					bit_color.RGBbits.rgbReserved=0xff;
					pdbuf[CurLine1+i]=bit_color.RGBdword;
				}
			}
			delete [] pBbuf;
		}
		break;
	case 32:			// add by zhaozhimeng @ 2007-06-18 for loading translucent bitmap
		{
			dwidth	=(dwWidth << 2);
			pBbuf	=new BYTE[dwidth*dwHeight];
			try
			{
				if (fileRead->Read(pBbuf,dwidth*dwHeight) != (dwidth*dwHeight))
				{
					delete [] pBbuf;
					return FALSE;
				}
			}
			catch(...)
			{
				delete [] pBbuf;
				return FALSE;
			}
			BYTE *pTmp = pBbuf;
			for(DWORD k=0; k<dwHeight; k++)
			{
				memcpy(pdbuf, pTmp, dwidth);
				pTmp += dwidth;
				pdbuf += dwWidth;
			}
			delete [] pBbuf;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
BOOL TP_LoadWav(LPCTSTR sFileName,LPBYTE &pByffer,long &lDataSize,WAVEFORMATEX &stuWavFmt)
{
	pByffer = NULL;
	CFile       fileRead;
	RIFFLIST	list;
	RIFFCHUNK	chunk;
	BOOL        bSuccess  =FALSE;
	

	if(!fileRead.Open(sFileName,CFile::modeRead|CFile::typeBinary)) return FALSE;
	bSuccess = fileRead.Read(&list, sizeof(list));
	if (!bSuccess || list.fcc != FCC('RIFF') || list.fccListType != FCC('WAVE'))
	{
		fileRead.Close();
		return FALSE;
	}
	bSuccess = fileRead.Read(&chunk, sizeof(chunk));
	if (!bSuccess || chunk.fcc != FCC('fmt '))
	{
		fileRead.Close();
		return FALSE;
	}
	bSuccess = fileRead.Read(&stuWavFmt, chunk.cb);
	if (!bSuccess)
	{
		fileRead.Close();
		return FALSE;
	}
	while (TRUE)
	{
		bSuccess = fileRead.Read(&chunk, sizeof(chunk));
		if (!bSuccess)
		{
			fileRead.Close();
			return FALSE;	// file format error
		}

		// may JUNK exist for align (eg. Matrox WAV)
		if (chunk.fcc == FCC('JUNK'))
		{
			fileRead.Seek(fileRead.GetPosition() + chunk.cb,CFile::begin);
		}
		else if (chunk.fcc == FCC('data'))
		{
			// OK! I complete the header, reach real data begin position
			lDataSize = chunk.cb;
			break;
		}
		else if (chunk.fcc == FCC('fact'))
		{
			fileRead.Seek(fileRead.GetPosition() + chunk.cb,CFile::begin);
		}
		else
		{
			return FALSE;	// file format error
		}
	}
	pByffer = new BYTE[lDataSize];
	fileRead.Read(pByffer,lDataSize);
	fileRead.Close();
	return TRUE;
}

};