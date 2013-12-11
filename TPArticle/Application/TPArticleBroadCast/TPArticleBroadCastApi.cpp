#include "StdAfx.h"


HMODULE g_hArticleManage = NULL;  
TPArticleManageProcess   g_stuPress;
TPArticleManageInterface g_stuArticleInterface;

LRESULT TP_InitArticleCenter()
{
	CString sFile   = _T("TPArticleManage");
#ifdef _DEBUG
	g_hArticleManage = ::LoadLibrary((CString)sFile + _T("ud.dll"));
#else
	g_hArticleManage = ::LoadLibrary((CString)sFile + _T("u.dll"));
#endif
	if(g_hArticleManage == NULL){ASSERT(0); return S_FALSE;}

	void (*TP_GetManageProcess)(TPArticleManageProcess * ) = NULL;
	TP_GetManageProcess = (void (* )(TPArticleManageProcess *)) ::GetProcAddress(g_hArticleManage,"TP_GetManageProcess");
	if(TP_GetManageProcess == NULL)
	{
		ASSERT(0);
		::FreeLibrary(g_hArticleManage);
		g_hArticleManage = NULL;
		return S_FALSE;
	}
	TP_GetManageProcess(&g_stuPress);	
	g_stuPress.TP_GetManageFunction(&g_stuArticleInterface);
	g_stuPress.TP_InitData(NULL);
	return S_OK;
}
LRESULT TP_ReleaseArticleCenter()
{
	if(g_stuPress.TP_ReleaseData)
		g_stuPress.TP_ReleaseData(NULL);
	if(g_hArticleManage)
	{
		::FreeLibrary(g_hArticleManage);
		g_hArticleManage = NULL;
		return S_OK;
	}
	return S_FALSE;
}






//////////////////////////////////////////////////////////////////////////
void TP_TransBufferEx(LPDWORD lpDestBuffer, UINT nDestWidth, UINT nDestHeight,LPDWORD lpSrcBuffer, UINT nSrcWidth, UINT nSrcHeight)
{
	TP_StretchBlt_Mean((LPDWORD)lpDestBuffer, 0, 0, nDestWidth, nDestHeight, nDestWidth, nDestHeight,(LPDWORD)lpSrcBuffer, 0, 0, nSrcWidth, nSrcHeight, nSrcWidth, nSrcHeight,TP_BUFFER_COPY, TP_PARITY_BOTH);
}
typedef struct tag_TempBuffer
{
	tag_TempBuffer()
	{
		pBuffer = NULL;
		iSize   = 0;
	}
	~ tag_TempBuffer()
	{
		if(pBuffer)
		{
			delete pBuffer;
			pBuffer = NULL;
		}
	}

	LPBYTE GetBuffer(INT iSizeNew)
	{
		if(pBuffer && iSizeNew > iSize)
		{
			delete pBuffer;
			pBuffer = NULL;
			iSize   = 0;
		}

		if(NULL == pBuffer)
		{
			pBuffer = new BYTE[iSizeNew];
			iSize   = iSizeNew;
		}

		return pBuffer;
	}
protected:
	LPBYTE pBuffer;
	INT    iSize;
}TP_TempBuffer;
TP_TempBuffer    g_stuTempBuffer;
CCriticalSection g_cStretchLock;
void    TP_StretchDIBitsEx(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn/* = FALSE*/)
{
	if(pBuffer == NULL)  return;
	INT iDesWidth  = rtDest.Width();
	INT iDesHeight = rtDest.Height();
	if(iDesWidth <= 0 || iDesHeight <= 0) return;

	LPBYTE pDesBuf = pBuffer;
	CTPSyncLock stuLock(&g_cStretchLock);
	if(iSrcWidth != iDesWidth || iSrcHeight != iDesHeight)
	{
		pDesBuf = g_stuTempBuffer.GetBuffer(iDesWidth * iDesHeight * 4);
		TP_TransBufferEx((DWORD*)pDesBuf, iDesWidth, iDesHeight,(DWORD*)pBuffer, iSrcWidth, iSrcHeight);
	}

	TP_SetDIBitsToDevice(pDC, pDesBuf, iDesWidth, iDesHeight, rtDest, bOverTurn);
}

void    TP_StretchDIBitsEx(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDestTrans,int iDesWidthShow,int iDesHightShow,BOOL bOverTurn/* = FALSE*/)
{
	if(pBuffer == NULL)  return;
	INT iDesWidth  = rtDestTrans.Width();
	INT iDesHeight = rtDestTrans.Height();
	if(iDesWidth <= 0 || iDesHeight <= 0) return;

	LPBYTE pDesBuf = pBuffer;
	CTPSyncLock stuLock(&g_cStretchLock);
	if(iSrcWidth != iDesWidth || iSrcHeight != iDesHeight)
	{
		pDesBuf = g_stuTempBuffer.GetBuffer(iDesWidth * iDesHeight * 4);
		TP_TransBufferEx((DWORD*)pDesBuf, iDesWidth, iDesHeight,(DWORD*)pBuffer, iSrcWidth, iSrcHeight);
	}

	CRect rtDestShow  = rtDestTrans;
	rtDestShow.right  = rtDestShow.left + iDesWidthShow;
	rtDestShow.bottom = rtDestShow.top  + iDesHightShow;
	TP_SetDIBitsToDevice(pDC, pDesBuf, iDesWidth, iDesHeight, rtDestShow, bOverTurn);
}

