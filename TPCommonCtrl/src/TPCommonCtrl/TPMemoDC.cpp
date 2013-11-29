#include "StdAfx.h"
#include ".\tpmemodc.h"


CTPDCExchange::CTPDCExchange(CDC *pDC)
{
	m_hDC   = NULL;
	if(pDC)
	{
		m_hDC         = pDC->m_hDC;
		m_hSaveFont   = (HFONT)::GetCurrentObject(m_hDC, OBJ_FONT);
		m_hSavePen    = (HPEN)::GetCurrentObject(m_hDC, OBJ_PEN);
		m_hSaveBrush  = (HBRUSH)::GetCurrentObject(m_hDC, OBJ_BRUSH);
		m_hSaveBitmap = (HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP);	
	}
}
CTPDCExchange::CTPDCExchange(HDC hDC)
{
	ReleaseDC();
}
void CTPDCExchange::ReleaseDC()
{
	if(m_hDC)
	{	
		if(m_hSaveFont)   ::SelectObject(m_hDC,(HFONT)m_hSaveFont);     m_hSaveFont   = NULL;
		if(m_hSavePen)    ::SelectObject(m_hDC,(HPEN)m_hSavePen);       m_hSavePen    = NULL;
		if(m_hSaveBrush)  ::SelectObject(m_hDC,(HBRUSH)m_hSaveBrush);   m_hSaveBrush  = NULL;
		if(m_hSaveBitmap && m_hSaveBitmap != (HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP)) 
			::SelectObject(m_hDC,(HBITMAP)m_hSaveBitmap); m_hSaveBitmap = NULL;
		m_hDC = NULL;
	}
}
CTPDCExchange::~CTPDCExchange(void)
{
	//ASSERT(m_hDC==NULL);
}


CTPMemoDCEx::CTPMemoDCEx(CDC *pSrcDC,CDC *pSrcDCEx,int iWidth,int iHeight)
{	
	BOOL bReturn = CDC::CreateCompatibleDC(pSrcDC);	
	ASSERT(bReturn == TRUE);
	ASSERT(iWidth * iHeight != 0);
	if(pSrcDCEx)
		bReturn = m_hbitmap.CreateCompatibleBitmap(pSrcDCEx,iWidth,iHeight);	
	else 
		bReturn = m_hbitmap.CreateCompatibleBitmap(pSrcDC,iWidth,iHeight);	
	ASSERT(bReturn == TRUE);
	m_pBitmap = SelectObject(&m_hbitmap); 
}

CTPMemoDCEx::~CTPMemoDCEx(void)
{
	if(m_pBitmap)
	{
		SelectObject(m_pBitmap);
		m_pBitmap = NULL;
		m_hbitmap.DeleteObject();		
	}
}
BOOL CTPMemoDCEx::DeleteDC()
{
	if(m_pBitmap)
	{
		SelectObject(m_pBitmap);
		m_pBitmap = NULL;
		m_hbitmap.DeleteObject();		
	}
	return CDC::DeleteDC();
}

CTPMemoDC::CTPMemoDC(void)
{
	m_hSaveBitmap = NULL;
	m_hSaveFont   = NULL;  
	m_hSavePen    = NULL;
	m_hSaveBrush  = NULL;
	m_iWidth      = 0;
	m_iHeight     = 0;
	m_bDeleteDC   = FALSE;
	m_iFontIndex  = FONT_BASE_TEXT;

	/*m_hSelectBitmap  = NULL;
	m_hSelectFont    = NULL;  
	m_hSelectPen     = NULL;
	m_hSelectBrush   = NULL;*/

}
CTPMemoDC::CTPMemoDC(CDC *pSrcDC,int iWidth,int iHeight)
{	
	m_iFontIndex  = FONT_BASE_TEXT;

	m_hSaveBitmap = NULL;
	m_hSaveFont   = NULL;  
	m_hSavePen    = NULL;
	m_hSaveBrush  = NULL;
	m_iWidth      = 0;
	m_iHeight     = 0;



	CDC::CreateCompatibleDC(pSrcDC);	

	if(iWidth * iHeight == 0) return;

	m_hSaveFont   = (HFONT)::GetCurrentObject(m_hDC, OBJ_FONT);
	m_hSavePen    = (HPEN)::GetCurrentObject(m_hDC, OBJ_PEN);
	m_hSaveBrush  = (HBRUSH)::GetCurrentObject(m_hDC, OBJ_BRUSH);
	m_hSaveBitmap = (HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP);


	m_bmpCreate.CreateCompatibleBitmap(pSrcDC,iWidth,iHeight);
	::SelectObject(m_hDC,(HBITMAP)m_bmpCreate.GetSafeHandle());
	::SelectObject(m_hDC,(HFONT)TP_GetSysFont(m_iFontIndex)->GetSafeHandle()); 

	CDC::SetBkMode(TRANSPARENT);
	CDC::SetStretchBltMode(COLORONCOLOR);


	m_hSavePen    = NULL;
	m_hSaveBrush  = NULL;
	m_iWidth      = iWidth;
	m_iHeight     = iHeight;

	/*m_hSelectBitmap  = m_bmpCreate.m_hObject;
	m_hSelectFont    = TP_GetSysFont(m_iFontIndex)->m_hObject;  
	m_hSelectPen     = NULL;
	m_hSelectBrush   = NULL;*/

	m_bDeleteDC   = FALSE;
}

CTPMemoDC::~CTPMemoDC(void)
{
	if(!m_bDeleteDC)
	{	
		if(m_hSaveFont)   ::SelectObject(m_hDC,(HFONT)m_hSaveFont);     m_hSaveFont   = NULL;
		if(m_hSavePen)    ::SelectObject(m_hDC,(HPEN)m_hSavePen);       m_hSavePen    = NULL;
		if(m_hSaveBrush)  ::SelectObject(m_hDC,(HBRUSH)m_hSaveBrush);   m_hSaveBrush  = NULL;
		if(m_hSaveBitmap) ::SelectObject(m_hDC,(HBITMAP)m_hSaveBitmap); m_hSaveBitmap = NULL;
		m_bmpCreate.DeleteObject();
	}	
}
BOOL CTPMemoDC::DeleteDC()
{
	if(!m_bDeleteDC)
	{	
		if(m_hSaveFont)   ::SelectObject(m_hDC,(HFONT)m_hSaveFont);     m_hSaveFont   = NULL;
		if(m_hSavePen)    ::SelectObject(m_hDC,(HPEN)m_hSavePen);       m_hSavePen    = NULL;
		if(m_hSaveBrush)  ::SelectObject(m_hDC,(HBRUSH)m_hSaveBrush);   m_hSaveBrush  = NULL;
		if(m_hSaveBitmap) ::SelectObject(m_hDC,(HBITMAP)m_hSaveBitmap); m_hSaveBitmap = NULL;
		m_bmpCreate.DeleteObject();
		m_bDeleteDC = TRUE;
	}
	return CDC::DeleteDC();
}
//CBitmap      * CTPMemoDC::SelectObject(CBitmap* pBitmap)
//{
//	if(pBitmap == NULL) return NULL;
//	if(m_hSelectBitmap == pBitmap->m_hObject) return NULL;
//	m_hSelectBitmap = pBitmap->m_hObject;
//	if(m_hSaveBitmap == NULL)
//	{
//		CBitmap *pSaveBitmap = CDC::SelectObject(pBitmap); 
//		if(pSaveBitmap) m_hSaveBitmap = (HBITMAP)pSaveBitmap->GetSafeHandle(); 
//		return pSaveBitmap;
//	}
//	else return CDC ::SelectObject(pBitmap);
//}
//CPen         * CTPMemoDC::SelectObject(CPen* pPen )
//{
//	if(pPen == NULL) return NULL;
//	if(m_hSelectPen == pPen->m_hObject) return NULL;
//	m_hSelectPen = pPen->m_hObject;
//	if(m_hSavePen == NULL)
//	{
//		CPen *pSavePen = CDC::SelectObject(pPen); 
//		if(pSavePen) m_hSavePen = (HPEN)pSavePen->GetSafeHandle();
//		return pSavePen;
//	}
//	else return CDC::SelectObject(pPen);
//}
//CBrush       * CTPMemoDC::SelectObject(CBrush* pBrush )
//{
//	if(pBrush == NULL) return NULL;
//	if(m_hSelectBrush == pBrush->m_hObject) return NULL;
//	m_hSelectBrush = pBrush->m_hObject;
//	if(m_hSaveBrush == NULL)
//	{
//		CBrush *pSaveBrush = CDC::SelectObject(pBrush); 
//		if(pSaveBrush) m_hSaveBrush = (HBRUSH)pSaveBrush->GetSafeHandle();
//		return pSaveBrush ;
//	}
//	else return CDC::SelectObject(pBrush);
//}
//CFont* CTPMemoDC::SelectObject(CFont* pFont )
//{
//	if(pFont == NULL) return NULL;
//	if(m_hSelectFont == pFont->m_hObject) return NULL;
//	m_hSelectFont = pFont->m_hObject;
//	if(m_hSaveFont == NULL) 
//	{
//		CFont *pSaveFont = CDC::SelectObject(pFont); 
//		if(pSaveFont) m_hSaveFont = (HFONT)pSaveFont->GetSafeHandle();
//		return pSaveFont ;
//	}
//	else return CDC::SelectObject(pFont);
//}
//int  CTPMemoDC::SelectObject(CRgn* pRgn )
//{
//	return CDC::SelectObject(pRgn);
//}

CBitmap *CTPMemoDC::GetBitmap(BOOL bDetach)
{	
	//if(bDetach)
	//{
	//	if(m_hSaveBitmap)::SelectObject(m_hDC,m_hSaveBitmap);
	//	//m_hSelectBitmap = m_hSaveBitmap;
	//	m_hSaveBitmap   = NULL;
	//}
	return &m_bmpCreate;
}
void CTPMemoDC::PaintRect(CRect &rt,CPen *pPen1,CPen *pPen2)
{
	CPen *pPenSave = NULL;
	if(pPen1) pPenSave = CDC::SelectObject(pPen1);
	CDC::MoveTo(rt.left,rt.bottom);
	CDC::LineTo(rt.left,rt.top);
	CDC::LineTo(rt.right,rt.top);
	if(pPen2) CDC::SelectObject(pPen2);
	CDC::LineTo(rt.right,rt.bottom);
	CDC::LineTo(rt.left,rt.bottom);
	if(pPenSave) CDC::SelectObject(pPenSave);
}
void CTPMemoDC::DrawBitmap(CBitmap *pBitmap,CPoint &ptPos)
{
	CDC cdcMem;BITMAP bmpinfo;
	pBitmap->GetBitmap(&bmpinfo);
	cdcMem.CreateCompatibleDC(this);
	CBitmap *pSave = cdcMem.SelectObject(pBitmap);
	this->BitBlt(ptPos.x,ptPos.y,bmpinfo.bmWidth,bmpinfo.bmHeight,&cdcMem,0,0,SRCCOPY);
	cdcMem.SelectObject(pSave);
	cdcMem.DeleteDC();
}
void CTPMemoDC::FillSolidRectEx(CDC *pDC,LPCRECT lpRect,COLORREF clr1,COLORREF clr2,BOOL bHorz)
{
	CRect rtFill;
	if(bHorz)
	{
		rtFill = lpRect;
		rtFill.right = rtFill.CenterPoint().x;
		TP_DrawGradient(pDC,rtFill,clr1,clr2,TRUE,FALSE);
		rtFill = lpRect;
		rtFill.left  = rtFill.CenterPoint().x;
		TP_DrawGradient(pDC,rtFill,clr2,clr1,TRUE,FALSE);
	}
	else
	{
		rtFill = lpRect;
		rtFill.bottom = rtFill.CenterPoint().y;
		TP_DrawGradient(pDC,rtFill,clr1,clr2,FALSE,FALSE);
		rtFill = lpRect;
		rtFill.top = rtFill.CenterPoint().y;
		TP_DrawGradient(pDC,rtFill,clr2,clr1,FALSE,FALSE);
	}
	
//TP_DrawGradient
}
