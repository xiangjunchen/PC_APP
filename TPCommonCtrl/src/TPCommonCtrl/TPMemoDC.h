#pragma once

class AFX_EXT_CLASS CTPDCExchange
{
public:
	CTPDCExchange(CDC *pDC); 	
	CTPDCExchange(HDC hDC);	
	~CTPDCExchange();	
	void ReleaseDC();	
private:
	HDC          m_hDC;

	HBITMAP      m_hSaveBitmap;
	HFONT        m_hSaveFont;  
	HPEN         m_hSavePen;
	HBRUSH       m_hSaveBrush;
};
class AFX_EXT_CLASS CTPDCObject
{
public:
	CTPDCObject(CDC *pDC,CFont *pFont)
	{
		m_pDC   = pDC->GetSafeHdc();
		m_pFont = (HFONT)::GetCurrentObject(m_pDC, OBJ_FONT);
		SelectObject(pDC ->GetSafeHdc(),pFont->GetSafeHandle());
		m_pPen  = NULL;
	}
	CTPDCObject(CDC *pDC,CPen  *pPen)
	{
		m_pDC   = pDC->GetSafeHdc();
		m_pFont = NULL;
		m_pPen  = (HPEN)::GetCurrentObject(m_pDC, OBJ_PEN);
		SelectObject(pDC ->GetSafeHdc(),pPen->GetSafeHandle());
	}
	void Release()
	{
		if(m_pFont) SelectObject(m_pDC,m_pFont);
		m_pFont = NULL;
		if(m_pPen) SelectObject(m_pDC,m_pPen);
		m_pPen = NULL;
		m_pDC  = NULL;
	}
	~CTPDCObject()
	{
		Release();
	}
private:
	HFONT     m_pFont;
	HPEN      m_pPen;
	HDC       m_pDC;
};
class AFX_EXT_CLASS CTPMemoDCEx  :public CDC
{
public:	
	CTPMemoDCEx(CDC *pSrcDC,CDC *pSrcDCEx,int iWidth,int iHeight);
	~CTPMemoDCEx(void);
public:	
	BOOL           DeleteDC();
private:
	CBitmap   m_hbitmap;
	CBitmap  *m_pBitmap; 
};
class AFX_EXT_CLASS CTPMemoDC  :public CDC//,public CTPBaseCtrl
{
public:
	CTPMemoDC(void);
	CTPMemoDC(CDC *pSrcDC,int iWidth,int iHeight);
	~CTPMemoDC(void);
protected:
	CBitmap      m_bmpCreate;
	HBITMAP      m_hSaveBitmap;
	HFONT        m_hSaveFont;  
	HPEN         m_hSavePen;
	HBRUSH       m_hSaveBrush;
	BOOL         m_bDeleteDC;


	int             m_iWidth;
	int             m_iHeight;
	TP_SYSTEM_FONT  m_iFontIndex;
	/*HGDIOBJ      m_hSelectBitmap;
	HGDIOBJ      m_hSelectFont;  
	HGDIOBJ      m_hSelectPen;
	HGDIOBJ      m_hSelectBrush;*/

public:
	/*CBitmap      * SelectObject(CBitmap* pBitmap);
	CPen         * SelectObject(CPen* pPen );
	CBrush       * SelectObject(CBrush* pBrush );
	virtual CFont* SelectObject(CFont* pFont );
	int            SelectObject(CRgn* pRgn );*/
	BOOL           DeleteDC();

	void           DrawBitmap(CBitmap *pBitmap,CPoint &ptPos);

	CBitmap      *GetBitmap(BOOL bDetach = TRUE);

	void          PaintRect(CRect &rt,CPen *pPen1,CPen *pPen2);   

	static void   FillSolidRectEx(CDC *pDC,LPCRECT lpRect,COLORREF clr1,COLORREF clr2,BOOL bHorz);


};
