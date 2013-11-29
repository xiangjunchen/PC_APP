

#pragma once

AFX_EXT_API  HBITMAP TP_CreateDIBBitmap(LPBYTE pBuffer,int iDestWidth,int iDestHeight,CDC *pDC);
AFX_EXT_API  BYTE   *TP_GetBitmapBits(HBITMAP hBitmap,CSize &szBmp,CDC *pDC,WORD biBitCount = 32);
AFX_EXT_API  void    TP_SetDIBitsToDevice(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn = FALSE);
AFX_EXT_API  void    TP_SetDIBitsToDeviceTrans(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDest,COLORREF clMask);
AFX_EXT_API  void    TP_SetDIBitsToDevice(CDC *pDC,TPCommonRes *pCommRes ,CRect rtDest,BOOL bOverTurn = FALSE);
AFX_EXT_API  void    TP_SetDIBitsToDevice(CDC *pDC,LPBYTE pBuffer,int iSrcLeft, int iSrcTop, int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn = FALSE);
AFX_EXT_API  void    TP_StretchDIBits    (CDC *pDC,LPBYTE pBuffer,CRect rtSource, int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn =FALSE);
AFX_EXT_API  void    TP_StretchDIBitsExEx(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,int iDestWidth,CRect rtDest,BOOL bOverTurn = FALSE,int iType = 0);
AFX_EXT_API  void    TP_StretchDIBits    (CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn = FALSE);
AFX_EXT_API  void    TP_StretchDIBRect   (CDC *pDC,LPBYTE pdwBuf, CRect rtSource,int iSrcWidth,int iSrcHeight,int iTopLeft,int iRightBottom,CRect rtDest,BOOL bVert = FALSE,BOOL bOverTurn = FALSE,BOOL bStretchForce = 0,BYTE cExtType = 0);
AFX_EXT_API  void    TP_StretchDIBRect   (CDC *pDC,LPBYTE pdwBuf, int iSrcWidth,int iSrcHeight,int iTopLeft,int iRightBottom,CRect rtDest,BOOL bVert = FALSE,BOOL bOverTurn = FALSE,BOOL bStretchForce = 0,BYTE cExtType = 0);
AFX_EXT_API  void    TP_SetDIBRect       (CDC *pDC, LPBYTE pdwBuf,int iSrcWidth,int iSrcHeight,int iLeft,CRect rtDest,BOOL bVert,BOOL bOverTurn,BOOL bDir);
AFX_EXT_API  void    TP_TileDIBLine      (CDC *pDC,LPBYTE pdwBuf, int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bVert = FALSE,BOOL bOverTurn = FALSE);
AFX_EXT_API  void    TP_BitmapToRgn      (HRGN &rgnSet,HBITMAP hBitmap,COLORREF backcolor,CDC *pDC,CRect rtExclude = CRect(0,0,0,0));
AFX_EXT_API  BOOL	 TP_LoadBitmap       (LPCTSTR  sFileName,LPDWORD &lpdwBuffer,BITMAP &pBitmap);
AFX_EXT_API  BOOL    TP_LoadWav          (LPCTSTR sFileName,LPBYTE &pByffer,long &lDataSize,WAVEFORMATEX &stuWavFmt);
AFX_EXT_API  void    TP_SetDIBitsOverTurn(LPBYTE pBuffer,int iWidth,int iHeight);
AFX_EXT_API  void    TP_SetDIBitsOverTurnByPitch(LPBYTE pBuffer,int iWidthPitch,int iHeight);
AFX_EXT_API  void    TP_TransBuffer(LPDWORD lpDestBuffer, UINT nDestWidth, UINT nDestHeight,LPDWORD lpSrcBuffer, UINT nSrcWidth, UINT nSrcHeight);
AFX_EXT_API  void    TP_TransBufferYUTV(LPWORD lpDestBuffer, UINT nDestWidth, UINT nDestHeight, LPWORD lpSrcBuffer, UINT nSrcWidth, UINT nSrcHeight);					
AFX_EXT_API  void    TP_BlendDIBBalck(LPBYTE &pBuffer,int iWidth,int iHeight);
AFX_EXT_API  void    TP_SetDIBA(LPBYTE &pBuffer,int iWidth,int iHeight,BYTE bAlpha);
AFX_EXT_API  void    TP_PaintRoundRect(CDC *pDC,CRect rtRoundFrame,DWORD dwWndType = 04);
AFX_EXT_API  int     TP_WriteBimap(CString ptszFileName,int iWidth,int iHeight,int iDepth,BYTE* pData);
AFX_EXT_API  int     TP_ReadBimap(CString ptszFileName,int iWidth,int iHeight,int iDepth,BYTE* pData,BOOL bOverTurn = TRUE);
AFX_EXT_API  int     TP_ReadBimapEx(CString ptszFileName,long &iWidth,long &iHeight,long iDepth,BYTE* pData,BOOL bOverTurn = TRUE);
AFX_EXT_API  void    TP_DrawGradient(CDC* pDC,CRect& Rect, COLORREF StartColor,COLORREF EndColor,  BOOL bHorizontal,BOOL bUseSolid = FALSE);