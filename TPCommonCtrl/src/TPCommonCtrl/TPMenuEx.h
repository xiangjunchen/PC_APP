#pragma once
#include "afxwin.h"

#define TP_MENU_LEFTBAR  0x00000001
#define TP_MENU_UNSHADER 0x00000002
#define TP_MENU_UNCHECK  0x00000004

enum TP_MENU_STYLE
{
	TP_MENU_STYLE_DEFAULT,
	TP_MENU_STYLE_SLIDE
};

class CTPMenuEx;

#ifdef TP_NS_EDIT

typedef CArray<UINT ,UINT &> CUintArray;
class AFX_EXT_CLASS tag_TPMenuTriangle : public CWnd
{
	DECLARE_DYNAMIC(tag_TPMenuTriangle)
public:
	tag_TPMenuTriangle();
	virtual ~tag_TPMenuTriangle();
public:
	UINT                    m_uState;
	CRect                   m_rtWnd;
	CTPMenuEx              *m_pMenu; 
public:
	void                    PaintDC(CDC *pDC);
protected:
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnPaint();

};
#endif



typedef CArray<CTPMenuEx *,CTPMenuEx *&> CTPMenuExArray;

class AFX_EXT_CLASS CTPMenuEx :	public CMenu , public CTPBaseCtrl
{ 
public:
	CTPMenuEx(void);
	~CTPMenuEx(void);
public:
	DWORD         m_dwVertify;
	int           m_iBackColor;
	int           m_iTopLeft;
	int           m_iRightBottom;

	int           m_iText;
	int           m_iTextDisable;
	int           m_iTextHight;
	int           m_iSelectID;
	BOOL          m_bComMenu;  

	DWORD         m_dwType;
	long          m_lBarWidth;
	long          m_lCheckWidth;   
	HWND          m_hAttachWnd;
	long          m_lMinWidth; 	
	long          m_lMaxExtWidth; 
	int           m_iExtAddWidth; 
	CRect         m_rtMenuBox;
	long          m_iMenucy;
	BOOL          m_bLoadString;

	BOOL		  m_bIsFocus ;

	struct  tag_TPMenuItemData
	{
		DWORD            dwType;
		BOOL             bPopUp;
		HMENU            hParentMenu;
		HMENU            hMenu;
		COLORREF         clText;
		CTPMenuEx       *pMenuEx;
		LPBYTE           pIcon;
		CSize            szIcon;     		
		CString          sText;
		CString          sExtText;
		UINT_PTR         uID; 
		long             lMinWidth;
		DWORD_PTR        dwData; 
		CTPMenuEx       *pParentMenuEx;
		BOOL             bAutoDestory; 
		BOOL             bButton; 
		TPCommonRes     *pImage;
		int              iImageIndex;
		COLORREF         cImage;
		BYTE             bTextMode;
		BOOL             bCheck;
		CRect            rtItem;
		void            *pWndData; 
#ifdef TP_NS_EDIT
		tag_TPMenuTriangle *pTriangle;
#endif
		tag_TPMenuItemData(CTPMenuEx *pParentMenu)
		{
			dwType        = 0x7654321;
			bPopUp        = FALSE;
			hParentMenu   = NULL;
			hMenu         = NULL;
			pMenuEx       = NULL;
			clText        = GetSysColor(COLOR_MENUTEXT); 
			pIcon         = NULL;
			szIcon        = CSize(0,0);     			

			sText         = _L("");
			sExtText      = _T("");
			uID           = 0;
			lMinWidth     = 0;
			dwData        = 0; 
			pParentMenuEx = pParentMenu;
			bAutoDestory  = FALSE;
			bButton       = FALSE;  
			pImage        = NULL;
			iImageIndex   = 0;
			bCheck        = FALSE;
			rtItem        = CRect(0,0,0,0);
			cImage        = 0;			
			bTextMode     = 0; 
			pWndData      = NULL;
#ifdef TP_NS_EDIT
			pTriangle     = NULL;
#endif			
		}
		~tag_TPMenuItemData()
		{
			dwType = 0;
			if(bPopUp && pMenuEx)
			{
				if(bAutoDestory) pMenuEx ->DestroyMenu();
				else             pMenuEx ->Detach();
				delete pMenuEx;
				pMenuEx = NULL;
				bPopUp  = FALSE;
			}			
			if(pIcon)
			{
				delete pIcon;
				pIcon = NULL;				
			}
			dwData = 0;
		}
	};
private:
	DRAWITEMSTRUCT      m_stuSelectItem;
public:
	CArray<tag_TPMenuItemData *,tag_TPMenuItemData *&> m_aMenuItem;

	TPCommonRes        *m_pResCheck;
	TPCommonRes        *m_pLeftBar;	
	TPCommonRes        *m_pPopMenu;	

public:

	BOOL LoadMenu(LPCTSTR lpszResourceName);
	BOOL LoadMenu(UINT nIDResource);
	BOOL DestroyMenu();
	BOOL DeleteMenu(UINT nPosition, UINT nFlags);
	BOOL CopyMenu(HMENU hMenu);


	BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem ,LPCTSTR lpszNewItem ,LPCTSTR sExtText);
	BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem = 0,LPCTSTR lpszNewItem = NULL);
	BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const CBitmap* pBmp);
		
	CTPMenuEx* GetSubMenu(int nPos) ;
	BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0,LPCTSTR lpszNewItem = NULL);
	BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem,const CBitmap* pBmp);
	BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0,LPCTSTR lpszNewItem = NULL);
	BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem,const CBitmap* pBmp);
	BOOL SetMenuItemBitmaps(UINT nPosition, UINT nFlags,const CBitmap* pBmpUnchecked, const CBitmap* pBmpChecked);

	BOOL TrackPopupMenu(UINT nFlags, int x, int y,CWnd* pWnd, LPCRECT lpRect = NULL);

	int  GetMenuString(UINT nIDItem, CString& rString, UINT nFlags);
	void SetMenuItemTextColor(UINT nPosition, UINT nFlags,COLORREF clText);
	

	void SetType(DWORD dwType);
	void SetType();
	void PaintMenu(CDC *pDC,CRect rtClient); 
	void SetBarRes(CString sFileName);


	void AppendMenu(int iIndex,CTPMenuEx *pSubMenu);
	BOOL SetMenuExtText(UINT nFlags, UINT_PTR nIDNewItem ,LPCTSTR sExtText);

	void CopyTo(CTPMenuEx *pSubMenu);

	void  SetMenuItemWidth(UINT nPosition, UINT nFlags,long lWidth);
	void  EnableMenuItems(UINT *puID,int iCoun,UINT uFlag= MF_BYCOMMAND );
	void  EnableMenuItemsEx(UINT *puID,int iCoun,UINT uFlag = MF_BYCOMMAND);

	void  SetMenuItemIcon(UINT nPosition, UINT nFlags,LPBYTE pIcon,CSize szIcon,BOOL bButton = FALSE);
	BOOL  SetMenuItemImage(UINT nPosition, UINT nFlags,TPCommonRes *pImage,int iImageIndex = 0,BOOL bCheck = TRUE);//Õ‚≤ø  Õ∑≈
	BOOL  SetMenuItemImage(UINT nPosition, UINT nFlags,COLORREF eColor,BOOL bCheck,BYTE bTextMode = 0);	
	
	UINT  GetMenuItemPos(UINT nID);
	long  GetMenuWidth(CString sText);


	BOOL         SetItemData(UINT uiId, DWORD_PTR dwItemData, BOOL fByPos = FALSE);	
	DWORD_PTR    GetItemData(UINT uiId, BOOL fByPos) ;

	void         InitMenu(HMENU hMenu,CDC *pDCSetEx = NULL);
	void         ClearMenu();
	int          GetMenuMinWidth();
	void         SetMenuItemAttachMenu(UINT nPosition, UINT nFlags,CTPMenuEx *pMenu,BOOL bAutoDestory);

	void         SetHotKeyData(TPHotKeyData *pHotKeyData,int iCount,CPoint *pIndex = NULL,int iIndexCount = 0); 

	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	UINT         CheckMenuItemEx(UINT nIDCheckItem, UINT nCheck);
	UINT         EnableMenuItemEx(UINT nIDEnableItem, UINT nEnable);
	BOOL         DeleteMenuEx(UINT nPosition, UINT nFlags);
	void         SetCheckImg(CString sCheck);
	void         SetEnableID(CUintArray  &aID); 
	void         AddColorMenu(CDWordArray &aColor,DWORD iSelColor,int iPosition,UINT uBaseID);
	void         SetMenuWndData(HWND hWnd,ULONG_PTR uItemData);
	BOOL         ApplyKey(UINT vKey);        
	static BOOL  IsMenuPopup();
	void         CheckPopUp();

private:
	ULONG_PTR    GetMenuItemData(UINT_PTR uItem,BOOL byPositon);
	void         SetMenuItemData(UINT_PTR uItem,ULONG_PTR dwData,BOOL byPositon);

};
AFX_EXT_API void TP_CloseMenu(HWND hWnd = NULL);

