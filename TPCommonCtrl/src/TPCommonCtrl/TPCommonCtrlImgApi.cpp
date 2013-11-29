//

#include "stdafx.h"
#include "TPCommonCtrlImgApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CTPRoundCorner   g_stuRoundCorner;
extern TPCommonRes     *g_pDefaultBitmap;

namespace _tp_commonctrl
{




HBITMAP TP_CreateDIBBitmap(LPBYTE pBuffer,int iDestWidth,int iDestHeight,CDC *pDC)
{
	BOOL bCreateDC = FALSE;
	if(pDC == NULL)
	{
		pDC = CWnd::FromHandle(GetDesktopWindow()) -> GetDC(); 	
		bCreateDC = TRUE;
	}	
	BITMAPINFO bpi;
	bpi.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
	bpi.bmiHeader.biWidth    =  iDestWidth;
	bpi.bmiHeader.biHeight   =  iDestHeight;
	bpi.bmiHeader.biPlanes   = 1;
	bpi.bmiHeader.biBitCount = 32;
	bpi.bmiHeader.biCompression = BI_RGB;
	bpi.bmiHeader.biSizeImage = 0;
	bpi.bmiHeader.biXPelsPerMeter = 0;
	bpi.bmiHeader.biYPelsPerMeter = 0;
	bpi.bmiHeader.biClrUsed = 0;
	bpi.bmiHeader.biClrImportant = 0;

	HBITMAP hBmp =::CreateDIBitmap(pDC->GetSafeHdc(), (LPBITMAPINFOHEADER)&bpi, CBM_INIT, 
		pBuffer, &bpi, DIB_RGB_COLORS);
	if(bCreateDC) CWnd::FromHandle(GetDesktopWindow()) -> ReleaseDC(pDC); 
	return hBmp;
}
BYTE *TP_GetBitmapBits(HBITMAP hBitmap,CSize &szBmp,CDC *pDC,WORD biBitCount)
{
	if(hBitmap == NULL) return NULL;
	CBitmap *pBitmap = CBitmap::FromHandle(hBitmap);	
	if(pBitmap ==NULL)  return NULL;
	else
	{
		BOOL  bReleaseDC = FALSE;
		if(pDC == NULL) 
		{
			pDC = CWnd::FromHandle(GetDesktopWindow()) -> GetDC();
			bReleaseDC  = TRUE;
		}

		BITMAP bitmap; 
		pBitmap->GetBitmap(&bitmap);
		szBmp.cx = bitmap.bmWidth;
		szBmp.cy = bitmap.bmHeight;

		BYTE    *pByteTemp = new BYTE[bitmap.bmWidth*bitmap.bmHeight*4];

		BITMAPINFO bpi;
		bpi.bmiHeader.biSize  = sizeof(BITMAPINFOHEADER);
		bpi.bmiHeader.biWidth =  bitmap.bmWidth;
		bpi.bmiHeader.biHeight = bitmap.bmHeight;
		bpi.bmiHeader.biPlanes = 1;
		bpi.bmiHeader.biBitCount = biBitCount;
		bpi.bmiHeader.biCompression = BI_RGB;
		bpi.bmiHeader.biSizeImage = 0;
		bpi.bmiHeader.biXPelsPerMeter = 0;
		bpi.bmiHeader.biYPelsPerMeter = 0;
		bpi.bmiHeader.biClrUsed = 0;
		bpi.bmiHeader.biClrImportant = 0;
		GetDIBits(
			pDC->m_hDC,
			hBitmap,	
			0,
			bitmap.bmHeight,
			(LPVOID)pByteTemp,	  
			&bpi,	   
			DIB_RGB_COLORS
			);
		if(bReleaseDC) CWnd::FromHandle(GetDesktopWindow()) ->ReleaseDC(pDC);
		return pByteTemp;
	}	
}
BYTE *g_pbufTemp = NULL;
int   g_ibufTemp = 0;
void  TP_SetDIBitsOverTurn(LPBYTE pBuffer,int iWidth,int iHeight)
{
	if(g_ibufTemp <iWidth *4)
	{
		if(g_pbufTemp) delete [] g_pbufTemp;
		g_pbufTemp = new BYTE[iWidth *4];
		g_ibufTemp = iWidth *4;
	}	
	for(INT k=0;k<iHeight/2;k++)
	{
		memcpy(g_pbufTemp,pBuffer + k*iWidth *4,iWidth *4);
		memcpy(pBuffer + k*iWidth *4,pBuffer + (iHeight - k - 1)*iWidth *4,iWidth *4);
		memcpy(pBuffer + (iHeight - k-1)*iWidth *4,g_pbufTemp,iWidth *4);
	}
}
void  TP_SetDIBitsOverTurnByPitch(LPBYTE pBuffer,int iWidthPitch,int iHeight)
{
	if(g_ibufTemp <iWidthPitch)
	{
		if(g_pbufTemp) delete [] g_pbufTemp;
		g_pbufTemp = new BYTE[iWidthPitch];
		g_ibufTemp = iWidthPitch;
	}
	for(INT k=0;k<iHeight/2;k++)
	{
		memcpy(g_pbufTemp,pBuffer + k*iWidthPitch,iWidthPitch);
		memcpy(pBuffer + k*iWidthPitch,pBuffer + (iHeight - k - 1)*iWidthPitch,iWidthPitch);
		memcpy(pBuffer + (iHeight - k-1)*iWidthPitch,g_pbufTemp,iWidthPitch);
	}
}
void    TP_SetDIBitsToDevice(CDC *pDC,TPCommonRes *pCommRes ,CRect rtDest,BOOL bOverTurn)
{
	if(pCommRes == NULL) return;
	TP_SetDIBitsToDevice(pDC,pCommRes->GetByte(),pCommRes->Width(),pCommRes->Height(),rtDest,bOverTurn);
}
void    TP_SetDIBitsToDeviceTrans(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDest,COLORREF clMask)
{
	HBITMAP hBitmap = TP_CreateDIBBitmap(pBuffer,iSrcWidth,iSrcHeight,pDC);
	CImageList *pImageList = new CImageList;
	pImageList ->Create(iSrcWidth,iSrcHeight,ILC_COLORDDB|ILC_MASK,0,1);
	int iIndex = pImageList ->Add(CBitmap::FromHandle(hBitmap),clMask);
	pImageList ->Draw(pDC,0,CPoint(rtDest.left,rtDest.top),ILD_TRANSPARENT);
	::DeleteObject(hBitmap);
	pImageList ->DeleteImageList();
	
}
void    TP_SetDIBitsToDevice(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn)
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
void    TP_SetDIBitsToDevice(CDC *pDC,LPBYTE pBuffer,int iSrcLeft, int iSrcTop, int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn)
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
		iSrcLeft, iSrcTop ,
		0 ,iSrcHeight, 
		pBuffer, 
		&bpi, 
		DIB_RGB_COLORS
		);	
}
void    TP_StretchDIBitsExEx  (CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,int iDestWidth,CRect rtDest,BOOL bOverTurn,int iType)
{
	if(pBuffer == NULL)  return;

	if(iType==0) TP_StretchDIBits(pDC,pBuffer,iSrcWidth,iSrcHeight,rtDest,bOverTurn);
	else 
	{
		BITMAPINFO bpi;
		bpi.bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
		bpi.bmiHeader.biPlanes = 1;
		bpi.bmiHeader.biBitCount = 32;
		bpi.bmiHeader.biCompression = BI_RGB;
		bpi.bmiHeader.biSizeImage = 0; 
		bpi.bmiHeader.biClrUsed = 0; 
		bpi.bmiHeader.biClrImportant = 0;
		bpi.bmiHeader.biWidth       = iSrcWidth;
		bpi.bmiHeader.biHeight      = bOverTurn ? -iSrcHeight :iSrcHeight;
		bpi.bmiHeader.biSizeImage   = iSrcWidth*iSrcHeight*4;

		int iWidth = min(iSrcWidth * rtDest.Width()/iDestWidth,iSrcWidth);
		if(iType==1)
		{
			StretchDIBits(pDC->m_hDC, 
				rtDest.left ,rtDest.top ,
				rtDest.Width(),rtDest.Height(),
				0 ,0,
				iWidth,iSrcHeight, 
				pBuffer, 
				&bpi, 
				DIB_RGB_COLORS,
				SRCCOPY);
		}
		else
		{
			StretchDIBits(pDC->m_hDC, 
				rtDest.left ,rtDest.top ,
				rtDest.Width(),rtDest.Height(),
				iSrcWidth- iWidth,0,
				iWidth,iSrcHeight, 
				pBuffer, 
				&bpi, 
				DIB_RGB_COLORS,
				SRCCOPY);
		}
	}
}
void    TP_StretchDIBits    (CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn)
{
	if(pBuffer == NULL)  return;

	
	BITMAPINFO bpi;
	bpi.bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
	bpi.bmiHeader.biPlanes = 1;
	bpi.bmiHeader.biBitCount = 32;
	bpi.bmiHeader.biCompression = BI_RGB;
	bpi.bmiHeader.biSizeImage = 0; 
	bpi.bmiHeader.biClrUsed = 0; 
	bpi.bmiHeader.biClrImportant = 0;
	bpi.bmiHeader.biWidth       = iSrcWidth;
	bpi.bmiHeader.biHeight      = bOverTurn ? -iSrcHeight :iSrcHeight;
	bpi.bmiHeader.biSizeImage   = iSrcWidth*iSrcHeight*4;

	StretchDIBits(pDC->m_hDC, 
		rtDest.left ,rtDest.top ,
		rtDest.Width(),rtDest.Height(),
		0 ,0,
		iSrcWidth,iSrcHeight, 
		pBuffer, 
		&bpi, 
		DIB_RGB_COLORS,
		SRCCOPY);
}
void    TP_StretchDIBits    (CDC *pDC,LPBYTE pBuffer,CRect rtSource, int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn)
{
	if(pBuffer == NULL)  return;

	BITMAPINFO bpi;
	bpi.bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
	bpi.bmiHeader.biPlanes = 1;
	bpi.bmiHeader.biBitCount = 32;
	bpi.bmiHeader.biCompression = BI_RGB;
	bpi.bmiHeader.biSizeImage = 0; 
	bpi.bmiHeader.biClrUsed = 0; 
	bpi.bmiHeader.biClrImportant = 0;
	bpi.bmiHeader.biWidth       = iSrcWidth;
	bpi.bmiHeader.biHeight      = bOverTurn ? -iSrcHeight :iSrcHeight;
	bpi.bmiHeader.biSizeImage   = iSrcWidth*iSrcHeight*4;

	StretchDIBits(pDC->m_hDC, 
		rtDest.left ,rtDest.top ,
		rtDest.Width(),rtDest.Height(),
		rtSource.left ,rtSource.top ,
		rtSource.Width(),rtSource.Height(), 
		pBuffer, 
		&bpi, 
		DIB_RGB_COLORS,
		SRCCOPY);
}
void    TP_StretchDIBRect(CDC *pDC, LPBYTE pdwBuf, int iSrcWidth,int iSrcHeight,int iTopLeft,int iRightBottom,CRect rtDest,BOOL bVert,BOOL bOverTurn,BOOL bStretchForce,BYTE cExtType)
{
	TP_StretchDIBRect(pDC, pdwBuf, CRect(0,0,iSrcWidth,iSrcHeight),iSrcWidth,iSrcHeight,iTopLeft,iRightBottom,rtDest,bVert,bOverTurn,bStretchForce,cExtType);
}
void    TP_StretchDIBRect(CDC *pDC, LPBYTE pdwBuf, CRect rtSource,int iSrcWidth,int iSrcHeight,int iTopLeft,int iRightBottom,CRect rtDest,BOOL bVert,BOOL bOverTurn,BOOL bStretchForce,BYTE cExtType)
{
	ASSERT(pDC);
	ASSERT(pdwBuf);
	BITMAPINFO bpi;
	bpi.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
	bpi.bmiHeader.biPlanes       = 1;
	bpi.bmiHeader.biBitCount     = 32;
	bpi.bmiHeader.biCompression  = BI_RGB;
	bpi.bmiHeader.biSizeImage    = 0; 
	bpi.bmiHeader.biClrUsed      = 0; 
	bpi.bmiHeader.biClrImportant = 0;
	bpi.bmiHeader.biWidth        =  iSrcWidth;
	bpi.bmiHeader.biHeight       = bOverTurn ? -iSrcHeight: iSrcHeight;	
	bpi.bmiHeader.biSizeImage    = iSrcWidth*iSrcHeight*4;
	if(bVert)
	{
		BOOL bHStretch = BOOL(rtSource.Width() != rtDest.Width()); //垂直方向是否需要拉伸
		if(bStretchForce == 1)      bHStretch = TRUE;
		else if(bStretchForce == 2) bHStretch = FALSE;
		int  iMidHeight  = rtSource.Height() - iTopLeft - iRightBottom;		
		int  iDrawHeight = 0;
		int  iSrctop     = 0;
		if(iMidHeight >0)
		{
			iSrctop         = rtSource.bottom - iTopLeft;	
			for(INT l=rtDest.top;l<rtDest.bottom;l+= iMidHeight)
			{
				iDrawHeight = min(iMidHeight,rtDest.bottom-l);
				if(bHStretch)
				{									
					//画中部
					StretchDIBits(
						pDC ->m_hDC,
						rtDest.left,l,rtDest.Width() ,iDrawHeight,
						rtSource.left , iSrctop -iDrawHeight ,rtSource.Width(),iDrawHeight,
						pdwBuf, 
						&bpi, 
						DIB_RGB_COLORS,
						SRCCOPY);
				}
				else
				{					
					SetDIBitsToDevice(
						pDC->m_hDC,
						rtDest.left, l,rtDest.Width() ,iDrawHeight,
						rtSource.left ,iSrctop  -iDrawHeight ,0 ,iSrcHeight, 
						pdwBuf, 
						&bpi, 
						DIB_RGB_COLORS
						);
				}
			}
		}

		//画头部
		if(iTopLeft >0)
		{
			iDrawHeight = min(iTopLeft,rtDest.Height()/2);
			int iSrctop = rtSource.bottom - iDrawHeight;		
			if(bHStretch)
			{
				StretchDIBits(
					pDC ->m_hDC,
					rtDest.left,rtDest.top,rtDest.Width() ,iDrawHeight,
					rtSource.left ,iSrctop,rtSource.Width(),iDrawHeight,
					pdwBuf, 
					&bpi, 
					DIB_RGB_COLORS,
					SRCCOPY);
			}
			else
			{
				SetDIBitsToDevice(
					pDC->m_hDC,
					rtDest.left,rtDest.top,rtDest.Width(), iDrawHeight,
					rtSource.left ,iSrctop,0, iSrcHeight,
					pdwBuf, 
					&bpi, 
					DIB_RGB_COLORS
					);
			}
		}
		//画尾部
		if(iRightBottom>0)
		{		
			iDrawHeight = min(iRightBottom,rtDest.Height()/2);			
			int iSrctop =  rtSource.top ;			
			if(bHStretch)
			{
				StretchDIBits(
					pDC ->m_hDC,
					rtDest.left,rtDest.bottom - iDrawHeight,rtDest.Width() ,iDrawHeight,
					rtSource.left ,iSrctop, rtSource.Width(),iDrawHeight,
					pdwBuf, 
					&bpi, 
					DIB_RGB_COLORS,
					SRCCOPY);			
			}
			else
			{
				SetDIBitsToDevice(
					pDC->m_hDC,
					rtDest.left,rtDest.bottom - iDrawHeight + (cExtType == 1 ? (rtDest.Height()%iMidHeight%2==0 ? 0:1) : 0) ,rtDest.Width() ,iDrawHeight,
					rtSource.left ,iSrctop,0,iSrcHeight,		
					pdwBuf, 
					&bpi, 
					DIB_RGB_COLORS
					);		
			}	
		}

	}
	else
	{
		BOOL bVStretch   = BOOL(rtSource.Height() != rtDest.Height()); //垂直方向是否需要拉伸
		if(bStretchForce == 1)      bVStretch = TRUE;
		else if(bStretchForce == 2) bVStretch = FALSE;
		int  iMidWidth   = rtSource.Width() - iTopLeft - iRightBottom;
		int  iDrawWidth  = 0;
		if(iMidWidth >0)
		{		
			for(INT l=rtDest.left;l<rtDest.right && l<1920*2;l+= iMidWidth)
			{
				iDrawWidth = min(iMidWidth,rtDest.right-l-1);
				if(bVStretch)
				{
					//画中部
					StretchDIBits(
						pDC ->m_hDC,
						l,rtDest.top,iDrawWidth+1,rtDest.Height(),
						rtSource.left + iTopLeft,rtSource.top,iDrawWidth,rtSource.Height(),
						pdwBuf, 
						&bpi, 
						DIB_RGB_COLORS,
						SRCCOPY);
				}
				else
				{
					SetDIBitsToDevice(
						pDC->m_hDC,
						l,rtDest.top,iDrawWidth+1,rtDest.Height(),
						rtSource.left + iTopLeft,rtSource.top,0,rtDest.Height(),
						pdwBuf, 
						&bpi, 
						DIB_RGB_COLORS
						);
				}
			}
		}

		//画左部
		if(iTopLeft >0)
		{

			if(bVStretch)
			{
				StretchDIBits(
					pDC ->m_hDC,
					rtDest.left	,rtDest.top,min(iTopLeft,rtDest.Width()-3),rtDest.Height(),
					rtSource.left,rtSource.top,min(iTopLeft,rtSource.Width()-3),rtSource.Height(),
					pdwBuf, 
					&bpi, 
					DIB_RGB_COLORS,
					SRCCOPY);
			}
			else
			{
				SetDIBitsToDevice(
					pDC->m_hDC,
					rtDest.left,rtDest.top,min(iTopLeft,rtDest.Width()-3),rtDest.Height(),
					rtSource.left,rtSource.top,0,rtSource.Height(),
					pdwBuf, 
					&bpi, 
					DIB_RGB_COLORS
					);
			}
		}
		//画右部
		if(iRightBottom>0)
		{		
			int iRight = min(iRightBottom,rtDest.Width() -3);
			if(bVStretch)
			{
				StretchDIBits(
					pDC ->m_hDC,
					rtDest.right - 	iRight,rtDest.top,iRight,rtDest.Height(),
					rtSource.right - iRight,rtSource.top,iRight,rtSource.Height(),
					pdwBuf, 
					&bpi, 
					DIB_RGB_COLORS,
					SRCCOPY);			
			}
			else
			{
				SetDIBitsToDevice(
					pDC->m_hDC,
					rtDest.right - iRight,rtDest.top,iRight,rtDest.Height(),
					rtSource.right - iRight,rtSource.top,0,rtSource.Height(),
					pdwBuf, 
					&bpi, 
					DIB_RGB_COLORS
					);		
			}		
		}
	}
}
void    TP_SetDIBRect(CDC *pDC, LPBYTE pdwBuf,int iSrcWidth,int iSrcHeight,int iLeft,CRect rtDest,BOOL bVert,BOOL bOverTurn,BOOL bDir)
{
	ASSERT(pDC);
	ASSERT(pdwBuf);
	BITMAPINFO bpi;
	bpi.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
	bpi.bmiHeader.biPlanes       = 1;
	bpi.bmiHeader.biBitCount     = 32;
	bpi.bmiHeader.biCompression  = BI_RGB;
	bpi.bmiHeader.biSizeImage    = 0; 
	bpi.bmiHeader.biClrUsed      = 0; 
	bpi.bmiHeader.biClrImportant = 0;
	bpi.bmiHeader.biWidth        = iSrcWidth;
	bpi.bmiHeader.biHeight       = bOverTurn ? -iSrcHeight: iSrcHeight;	
	bpi.bmiHeader.biSizeImage    = iSrcWidth*iSrcHeight*4;
	if(bVert)
	{
		long lHeight = min(iLeft,rtDest.Height());		
		if(bDir)
		{
			SetDIBitsToDevice(
				pDC->m_hDC,
				rtDest.left,rtDest.bottom - lHeight,rtDest.Width() ,lHeight,
				0 ,0,0,iSrcHeight,		
				pdwBuf, 
				&bpi, 
				DIB_RGB_COLORS
				);		

			for(INT l=rtDest.bottom - lHeight;l>rtDest.top ;l-=iSrcHeight -iLeft)
			{
				long lHeight = min(iSrcHeight -iLeft,l-rtDest.top);				
				SetDIBitsToDevice(
					pDC->m_hDC,
					rtDest.left,l - lHeight,rtDest.Width() ,lHeight,
					0 ,iLeft,0,iSrcHeight,		
					pdwBuf, 
					&bpi, 
					DIB_RGB_COLORS
					);		
			}
		}
		else
		{
		}
	}
}

void    TP_TileDIBLine(CDC *pDC,LPBYTE pdwBuf, int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bVert,BOOL bOverTurn)
{

	ASSERT(pDC);
	ASSERT(pdwBuf);
	BITMAPINFO bpi;
	bpi.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
	bpi.bmiHeader.biPlanes       = 1;
	bpi.bmiHeader.biBitCount     = 32;
	bpi.bmiHeader.biCompression  = BI_RGB;
	bpi.bmiHeader.biSizeImage    = 0; 
	bpi.bmiHeader.biClrUsed      = 0; 
	bpi.bmiHeader.biClrImportant = 0;
	bpi.bmiHeader.biWidth        = iSrcWidth;
	bpi.bmiHeader.biHeight       = bOverTurn ? -iSrcHeight: iSrcHeight;	
	bpi.bmiHeader.biSizeImage    = iSrcWidth*iSrcHeight*4;

	if(bVert)
	{
		int iDrawHeight = 0;
		for(INT l=rtDest.top;l<rtDest.bottom;l+= iSrcHeight)
		{
			iDrawHeight = min(iSrcHeight,rtDest.bottom-l);

			SetDIBitsToDevice(
				pDC->m_hDC,
				rtDest.left, l,iSrcWidth ,iDrawHeight,
				0 ,0,0 ,iDrawHeight, 
				pdwBuf, 
				&bpi, 
				DIB_RGB_COLORS
				);
		}
	}
	else
	{
		int iDrawWidth  = 0;

		for(INT l=rtDest.left;l<rtDest.right;l+= iSrcWidth)
		{
			iDrawWidth = min(iSrcWidth,rtDest.right-l);

			SetDIBitsToDevice(
				pDC->m_hDC,
				l,rtDest.top,iDrawWidth,iSrcHeight,
				0,0,0,iSrcHeight,
				pdwBuf, 
				&bpi, 
				DIB_RGB_COLORS
				);
		}
	}
}
void TP_BitmapToRgn(HRGN &rgnSet,HBITMAP hBitmap,COLORREF backcolor,CDC *pDC,CRect rtExclude)
{	
	BYTE     rValue = GetRValue(backcolor);
	BYTE     gValue = GetGValue(backcolor);
	BYTE     bValue = GetBValue(backcolor);
	CRgn     rgntemp;
	CSize    szBmp  = CSize(0,0);
	int      iIndex = 0;
	LPBYTE pBuf = TP_GetBitmapBits(hBitmap,szBmp,pDC);

	if(pBuf)
	{
		BOOL  bFirst     = FALSE,bLast;
		BOOL  bMakeRgn   = FALSE; 
		DWORD maxRects   = 100;
		HANDLE hData = malloc(sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
		RGNDATA *pData = (RGNDATA *)hData;
		pData->rdh.dwSize = sizeof(RGNDATAHEADER);
		pData->rdh.iType = RDH_RECTANGLES;
		pData->rdh.nCount = pData->rdh.nRgnSize = 0;
		SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
		for(int height=0;height<szBmp.cy;height++)
		{
			bFirst = TRUE;
			bLast  = FALSE;
			for(int width=0;width<szBmp.cx;width++)
			{				
				if(rtExclude.PtInRect(CPoint(width,height))) continue;			
				iIndex = (width + (szBmp.cy - height -1) * szBmp.cx)*4;
				bMakeRgn = (pBuf[iIndex] == rValue) &&  (pBuf[iIndex+1] == gValue) && (pBuf[iIndex+2] == bValue);
				if(bMakeRgn || bFirst)
				{  		
					if(!bMakeRgn) bFirst = FALSE;					
					if (pData->rdh.nCount >= maxRects-1)
					{					
						maxRects += 100;
						hData = realloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
						pData = (RGNDATA *)hData;					
					}					
					RECT *pr = (RECT *)&pData->Buffer;
					SetRect(&pr[pData->rdh.nCount],width,height,width+1,height+1);	
					if (width < pData->rdh.rcBound.left)	  pData->rdh.rcBound.left = width;
					if (height < pData->rdh.rcBound.top)  	  pData->rdh.rcBound.top = height;
					if (width+1 > pData->rdh.rcBound.right)   pData->rdh.rcBound.right = width+1;
					if (height+1 > pData->rdh.rcBound.bottom) pData->rdh.rcBound.bottom = height+1;
					pData->rdh.nCount++;
					if(!bLast && !bFirst && height<16)
					{
						bLast = TRUE;
						RECT *pr = (RECT *)&pData->Buffer;
						SetRect(&pr[pData->rdh.nCount],width-1,height-1,width,height);							
						pData->rdh.nCount++;
					}
					if(pData->rdh.nCount >=200000) break;
				} 				
			}		
		}
		HRGN rgnTemp1 = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);		
		HRGN rgnTemp2 = CreateRectRgn(0,0,szBmp.cx,szBmp.cy);
		rgnSet = CreateRectRgn(0,0,szBmp.cx,szBmp.cy);
		CombineRgn(rgnSet,rgnTemp2,rgnTemp1,RGN_DIFF);	
		DeleteObject(rgnTemp1);
		DeleteObject(rgnTemp2);
		free(hData);
		delete pBuf;
	}
}
UINT gWidthTable[1920],gHeightTable[1080];UINT nSDestWidth = 0, nSDestHeight =0,nSSrcWidth=0,nSSrcHeight = 0;

void  TP_TransBuffer(LPDWORD lpDestBuffer, UINT nDestWidth, UINT nDestHeight, LPDWORD lpSrcBuffer, UINT nSrcWidth, UINT nSrcHeight)					
{
	TPMutex stuMutex(_T("TP_TransBuffer"));
    if(lpDestBuffer == NULL || lpSrcBuffer == NULL) return;
	if(	nDestWidth < 2 && nDestHeight < 2)	return ;
	if( nDestWidth == nSrcWidth && nDestHeight == nSrcHeight )
	{
		memcpy( lpDestBuffer, lpSrcBuffer, sizeof(DWORD) * nSrcWidth * nSrcHeight ) ;
		return ;
	}
	UINT i,j;
	//	计算各参考点的值。预先把参考点的值计算完毕可以提高运算速度。
	if(nSDestWidth!=nDestWidth||nSDestHeight!=nDestHeight||nSSrcWidth!=nSrcWidth||nSSrcHeight!=nSrcHeight)
	{
		nSDestWidth=nDestWidth;
		nSDestHeight=nDestHeight;
		nSSrcWidth=nSrcWidth;
		nSSrcHeight=nSrcHeight;
		for(i = 0;i < nDestHeight; i++)		gHeightTable[i] = (i*nSrcHeight/nDestHeight)*nSrcWidth;
		for(i = 0;i < nDestWidth; i++)		gWidthTable[i] = i*nSrcWidth/nDestWidth ;
	}
	for(i = 0; i < nDestHeight; i++)
		for(j = 0; j < nDestWidth; j++)
			lpDestBuffer[i*nDestWidth+j] = lpSrcBuffer[gHeightTable[i] + gWidthTable[j]] ;
}
void  TP_TransBufferYUTV(LPWORD lpDestBuffer, UINT nDestWidth, UINT nDestHeight, LPWORD lpSrcBuffer, UINT nSrcWidth, UINT nSrcHeight)					
{
	if(	nDestWidth < 2 && nDestHeight < 2)	return ;
	if( nDestWidth == nSrcWidth && nDestHeight == nSrcHeight )
	{
		memcpy( lpDestBuffer, lpSrcBuffer, sizeof(DWORD) * nSrcWidth * nSrcHeight ) ;
		return ;
	}
	UINT i,j;
	//	计算各参考点的值。预先把参考点的值计算完毕可以提高运算速度。
	if(nSDestWidth!=nDestWidth||nSDestHeight!=nDestHeight||nSSrcWidth!=nSrcWidth||nSSrcHeight!=nSrcHeight)
	{
		nSDestWidth=nDestWidth;
		nSDestHeight=nDestHeight;
		nSSrcWidth=nSrcWidth;
		nSSrcHeight=nSrcHeight;
		for(i = 0;i < nDestHeight; i++)		gHeightTable[i] = (i*nSrcHeight/nDestHeight)*nSrcWidth;
		for(i = 0;i < nDestWidth; i++)		gWidthTable[i] = i*nSrcWidth/nDestWidth ;
	}
	for(i = 0; i < nDestHeight; i++)
		for(j = 0; j < nDestWidth; j++)
			lpDestBuffer[i*nDestWidth+j] = lpSrcBuffer[gHeightTable[i] + gWidthTable[j]] ;
}
void    TP_SetDIBA(LPBYTE &pBuffer,int iWidth,int iHeight,BYTE bAlpha)
{
	if(iWidth<=0 || iHeight<=0)  return;
	for(INT l=0;l<iHeight;l++)
	{
		for(INT m=0;m<iWidth;m++)
		{
			pBuffer[l*iWidth*4+m*4+3] = bAlpha;
		}
	}
}
void    TP_BlendDIBBalck(LPBYTE &pBuffer,int iWidth,int iHeight)
{
	if(iWidth<=0 || iHeight<=0 || pBuffer == NULL)  return;
	for(INT l=0;l<iHeight;l++)
	{
		for(INT m=0;m<iWidth;m++)
		{
			pBuffer[l*iWidth*4+m*4+0] = pBuffer[l*iWidth*4+m*4+0] * pBuffer[l*iWidth*4+m*4+3]/255;
			pBuffer[l*iWidth*4+m*4+1] = pBuffer[l*iWidth*4+m*4+1] * pBuffer[l*iWidth*4+m*4+3]/255;
			pBuffer[l*iWidth*4+m*4+2] = pBuffer[l*iWidth*4+m*4+2] * pBuffer[l*iWidth*4+m*4+3]/255;	
			pBuffer[l*iWidth*4+m*4+3] = 255;
		}
	}
}
/*
函数名: DrawGraden
功   能: 画矩形渐变色, 可以使用多种颜色
参   数: hdc - 上下文句柄
Rect - 整个画渐变的矩形范围
cl - 颜色数组
Num - 颜色数组中元素个数
dwMode - 模式, 可以使用GRADIENT_FILL_RECT_H 和 GRADIENT_FILL_RECT_V
返回值: 成功返回TRUE, 失败返回FALSE, GetLastError 错误信息获取;
*/
/*
BOOL __fastcall DrawGraden(HDC hdc, CONST RECT *pRect, CONST DWORD *cl, int Num, DWORD dwMode)
{
	int Width;
	int Height;
	TRIVERTEX *pvert;
	GRADIENT_RECT    *pgRect;

	if (cl == NULL || Num < 1 || pRect == NULL || dwMode == GRADIENT_FILL_TRIANGLE)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return TRUE;
	}

	if (Num == 1)
	{
		HBRUSH hBrush = CreateSolidBrush(cl[0]);
		SelectObject(hdc, hBrush);
		FillRect(hdc, pRect, hBrush);
		DeleteObject(hBrush);
		return TRUE;
	}

	pvert = new TRIVERTEX[Num * 2 - 2];
	pgRect = new GRADIENT_RECT[Num];

	Width = pRect->right - pRect->left;
	Height = pRect->bottom - pRect->top;
	for (int i = 0; i < Num - 1; i++)
	{
		if (dwMode == GRADIENT_FILL_RECT_V)
		{
			pvert[i * 2].x = pRect->left;
			pvert[i * 2].y = pRect->top + Height / (Num - 1) * i;

			pvert[i * 2 + 1].x = pRect->right;
			pvert[i * 2 + 1].y = pRect->top + Height / (Num - 1) * (i + 1);
		}
		else if (dwMode == GRADIENT_FILL_RECT_H)
		{
			pvert[i * 2].x = pRect->left + Width / (Num - 1) * i;
			pvert[i * 2].y = pRect->top;

			pvert[i * 2 + 1].x = pRect->left + Width / (Num - 1) * (i + 1);
			pvert[i * 2 + 1].y = pRect->bottom;
		}

		pvert[i * 2] .Red    = (WORD)GetRValue((cl[i])) << 8;
		pvert[i * 2] .Green  = (WORD)GetGValue((cl[i])) << 8;
		pvert[i * 2] .Blue   = (WORD)GetBValue((cl[i])) << 8;
		pvert[i * 2] .Alpha  = 0x0000;

		pvert[i * 2 + 1] .Red    = (WORD)GetRValue((cl[i + 1])) << 8;
		pvert[i * 2 + 1] .Green  = (WORD)GetGValue((cl[i + 1])) << 8;
		pvert[i * 2 + 1] .Blue   = (WORD)GetBValue((cl[i + 1])) << 8;
		pvert[i * 2 + 1] .Alpha  = 0x0000;

		pgRect[i].UpperLeft  = i * 2;
		pgRect[i].LowerRight = i * 2 + 1;
	}

	BOOL bRet = ::GradientFill(hdc, pvert, Num * 2, pgRect, Num - 1, dwMode);

	delete []pvert;
	delete []pgRect;

	return bRet;
}*/

void  TP_PaintRoundRect(CDC *pDC,CRect rtRoundFrame,DWORD dwWndType)
{
	DWORD dwLevelSave = g_stuRoundCorner.m_dwWindowLevel;
	g_stuRoundCorner.m_dwWindowLevel = dwWndType;
	g_stuRoundCorner.PaintRect(pDC,rtRoundFrame);
	g_stuRoundCorner.m_dwWindowLevel = dwLevelSave;
}
int TP_WriteBimap(CString ptszFileName,int iWidth,int iHeight,int iDepth,BYTE* pData)
{
	BITMAPFILEHEADER stFileHdr;
	stFileHdr.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	stFileHdr.bfSize=sizeof(BITMAPFILEHEADER);
	stFileHdr.bfType=0x4d42; 
	BITMAPINFOHEADER stInfoHdr;
	memset(&stInfoHdr,0,sizeof(BITMAPINFOHEADER));
	stInfoHdr.biSize=sizeof(BITMAPINFOHEADER);
	stInfoHdr.biPlanes=1;
	stInfoHdr.biWidth=iWidth;
	stInfoHdr.biHeight=iHeight;
	stInfoHdr.biBitCount=iDepth;
	stInfoHdr.biSizeImage=stInfoHdr.biWidth*stInfoHdr.biHeight*stInfoHdr.biBitCount/8;

	FILE* fp= NULL;
	_wfopen_s(&fp,(LPCTSTR)ptszFileName,_T("wb"));
	fwrite(&stFileHdr,1,sizeof(BITMAPFILEHEADER),fp);
	fwrite(&stInfoHdr,1,sizeof(BITMAPINFOHEADER),fp);
	fwrite(pData,1,stInfoHdr.biSizeImage,fp);
	fclose(fp);
	return 1;
}
int TP_ReadBimap(CString ptszFileName,int iWidth,int iHeight,int iDepth,BYTE* pData,BOOL bOverTurn)
{
	BITMAPFILEHEADER stFileHdr;		ZeroMemory(&stFileHdr,sizeof(stFileHdr));	
	BITMAPINFOHEADER stInfoHdr;		ZeroMemory(&stInfoHdr,sizeof(stInfoHdr));
	FILE* fp= NULL;
	_wfopen_s(&fp,ptszFileName,_T("rb"));
	if(fp == NULL) return 0;
	if(fread(&stFileHdr,1,sizeof(BITMAPFILEHEADER),fp) != sizeof(BITMAPFILEHEADER))
	{
		fclose(fp); 
		return 0;
	}
	fread(&stInfoHdr,1,sizeof(BITMAPINFOHEADER),fp);
	if(iWidth != stInfoHdr.biWidth || iHeight != stInfoHdr.biHeight || stInfoHdr.biBitCount !=32)	
	{
		fclose(fp); 
		return 0;
	}
	fread(pData,1,stInfoHdr.biSizeImage,fp);
	fclose(fp);	
	if(bOverTurn) TP_SetDIBitsOverTurn(pData,stInfoHdr.biWidth,stInfoHdr.biHeight);
	return 1;
}
int TP_ReadBimapEx(CString ptszFileName,long &iWidth,long &iHeight,long iDepth,BYTE* pData,BOOL bOverTurn)
{
	BITMAPFILEHEADER stFileHdr;		ZeroMemory(&stFileHdr,sizeof(stFileHdr));	
	BITMAPINFOHEADER stInfoHdr;		ZeroMemory(&stInfoHdr,sizeof(stInfoHdr));
	FILE* fp= NULL;
	_wfopen_s(&fp,ptszFileName,_T("rb"));
	if(fp == NULL) return 0;
	if(fread(&stFileHdr,1,sizeof(BITMAPFILEHEADER),fp) != sizeof(BITMAPFILEHEADER))
	{
		fclose(fp); 
		return 0;
	}
	fread(&stInfoHdr,1,sizeof(BITMAPINFOHEADER),fp);
	iWidth  = stInfoHdr.biWidth;
	iHeight = stInfoHdr.biHeight;		
	fread(pData,1,stInfoHdr.biSizeImage,fp);
	fclose(fp);	
	if(bOverTurn) TP_SetDIBitsOverTurn(pData,stInfoHdr.biWidth,stInfoHdr.biHeight);
	return 1;
}
void TP_DrawGradient(CDC* pDC,CRect& Rect, COLORREF StartColor,COLORREF EndColor,  BOOL bHorizontal,BOOL bUseSolid)
{
	int Count = pDC->GetDeviceCaps(NUMCOLORS);

	if(Count==-1)	bUseSolid = FALSE;

	if(!bUseSolid )
	{
		TRIVERTEX vert[2] ;
		GRADIENT_RECT gRect;

		vert [0].y = Rect.top;
		vert [0].x = Rect.left;

		vert [0].Red    = COLOR16(COLOR16(GetRValue(StartColor))<<8);
		vert [0].Green  = COLOR16(COLOR16(GetGValue(StartColor))<<8);
		vert [0].Blue   = COLOR16(COLOR16(GetBValue(StartColor))<<8);
		vert [0].Alpha  = 0x0000;

		vert [1].y = Rect.bottom;
		vert [1].x = Rect.right;

		vert [1].Red    = COLOR16(COLOR16(GetRValue(EndColor))<<8);
		vert [1].Green  = COLOR16(COLOR16(GetGValue(EndColor))<<8);
		vert [1].Blue   = COLOR16(COLOR16(GetBValue(EndColor))<<8);
		vert [1].Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;

		if(bHorizontal)
		{
			::GradientFill(pDC->m_hDC,vert,2,&gRect,1,GRADIENT_FILL_RECT_H);
		}
		else
		{
			::GradientFill(pDC->m_hDC,vert,2,&gRect,1,GRADIENT_FILL_RECT_V);
		}
	}
	else
	{
		BYTE StartRed   = GetRValue(StartColor);
		BYTE StartGreen = GetGValue(StartColor);
		BYTE StartBlue  = GetBValue(StartColor);

		BYTE EndRed    = GetRValue(EndColor);
		BYTE EndGreen  = GetGValue(EndColor);
		BYTE EndBlue   = GetBValue(EndColor);

		int n = (bHorizontal)?Rect.Width():Rect.Height();		
		{
			if(bUseSolid)
			{				
				pDC->FillSolidRect(Rect,pDC->GetNearestColor(EndColor));
			}
			else
			{			
				CBrush TempBrush(EndColor);
				pDC->FillRect(Rect,&TempBrush);
			}
		}
		int dy = 2;
		n-=dy;
		for(int dn=0;dn<=n;dn+=dy)
		{
			BYTE ActRed = (BYTE)(MulDiv(int(EndRed)-StartRed,dn,n)+StartRed);
			BYTE ActGreen = (BYTE)(MulDiv(int(EndGreen)-StartGreen,dn,n)+StartGreen);
			BYTE ActBlue = (BYTE)(MulDiv(int(EndBlue)-StartBlue,dn,n)+StartBlue);

			CRect TempRect; 
			if(bHorizontal)
			{
				TempRect = CRect(CPoint(Rect.left+dn,Rect.top),CSize(dy,Rect.Height()));
			}
			else
			{
				TempRect = CRect(CPoint(Rect.left,Rect.top+dn),CSize(Rect.Width(),dy));
			}
			if(bUseSolid)
			{
				pDC->FillSolidRect(TempRect,pDC->GetNearestColor(RGB(ActRed,ActGreen,ActBlue)));
			}
			else
			{
				CBrush TempBrush(RGB(ActRed,ActGreen,ActBlue));
				pDC->FillRect(TempRect,&TempBrush);
			}
		}
	}
}




};
