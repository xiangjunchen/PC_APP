#pragma once

extern HMODULE g_hArticleManage ;
extern TPArticleManageProcess   g_stuPress;
extern TPArticleManageInterface g_stuArticleInterface;

LRESULT TP_InitArticleCenter();
LRESULT TP_ReleaseArticleCenter();

void     TP_StretchDIBitsEx(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDest,BOOL bOverTurn = FALSE);
void     TP_StretchDIBitsEx(CDC *pDC,LPBYTE pBuffer,int iSrcWidth,int iSrcHeight,CRect rtDestTrans,int iDesWidthShow,int iDesHightShow,BOOL bOverTurn/* = FALSE*/);
