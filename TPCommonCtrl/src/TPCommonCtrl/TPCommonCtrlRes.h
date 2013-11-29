#pragma once

enum TP_COMMONRES_TYPE
{
	TP_COMMONRES_UNKNOW,
	TP_COMMONRES_BITMAP,
	TP_COMMONRES_BMPBYE,
	TP_COMMONRES_ICON,
	TP_COMMONRES_CURSOR,	
	TP_COMMONRES_PEN,
	TP_COMMONRES_BRUSH,
	TP_COMMONRES_IMAGELIST,
};
#define TP_RESEMBLERES   0x00000001

typedef struct tag_TPCommonRes
{	
	DWORD               dwSafeFlag;
	TP_COMMONRES_TYPE   eCommonResType;
	BOOL                bLoadSucess;
	INT64               dwRefCount; 
	BOOL                bExclude;
	CString             sResName;
	UINT                uState; 

	BOOL               (*funRes)(tag_TPCommonRes *pRes); 
	void               (*TransBuffer)(LPDWORD , UINT , UINT , LPDWORD , UINT , UINT );		
	void               (*SetDIBitsOverTurn)(LPBYTE, INT, INT);


	union
	{
		struct 
		{
			HIMAGELIST hResImage;			
			CSize      szResImage;
			int        iResWidth;
			COLORREF   clMask;
		};
		struct 
		{			
			HBITMAP   hResBitmap;
			BITMAP    stuResBmp; 
		};
		struct 
		{		
			LPBYTE    pResBitmap;
			CSize     szResBmp; 
		};
		struct 
		{	
			CSize     szResWidth;
			HICON     hResIcon;
		};
		struct 
		{	
			CSize     szResWidth;
			HCURSOR   hResCursor;
		};
		struct 
		{		
			TP_SYSTEM_COLOR	 clPenIndex;		
			int              iPenStyle;
			int              iPenWidth;		
			HPEN             hPen;  
		};
		struct 
		{		
			TP_SYSTEM_COLOR	 clBrIndex;
			HBRUSH           hBrush;  
		};		
	};
public:
	tag_TPCommonRes()
	{
		dwSafeFlag      = 0x12345678;
		eCommonResType  = TP_COMMONRES_UNKNOW;
		bLoadSucess     = TRUE;
		uState          = 0;
		dwRefCount      = 0;
		bExclude        = FALSE;
		sResName        = _L("");
		funRes          = NULL;
	}
	tag_TPCommonRes(TP_COMMONRES_TYPE   eCommonResTypeSet)
	{
		eCommonResType  = eCommonResTypeSet;
		dwRefCount      = 0;
		sResName        = _L("");

		switch(eCommonResType)
		{
		case TP_COMMONRES_BITMAP:			
			hResBitmap = NULL;
			break;
		case TP_COMMONRES_BMPBYE:			
			pResBitmap = NULL;
			break;
		case TP_COMMONRES_ICON:					
			hResIcon =NULL;
			break;
		case TP_COMMONRES_CURSOR:			
			hResCursor = NULL;
			break;
		case TP_COMMONRES_PEN:			
			hPen  = NULL;
			break;
		case TP_COMMONRES_BRUSH:			
			hBrush  = NULL;
			break;
		case TP_COMMONRES_IMAGELIST:			
			hResImage = NULL;
			break;			
		default:
			break;
		}
	}
	void     GetBuffer(LPBYTE pBuffer,int iWidth,int iHeight,BOOL bReverse = FALSE)
	{
		if(pBuffer)
		{
			LPBYTE pSrcBuf = this->GetByte();
			if(pSrcBuf==NULL) return;
			CSize szOrigIcon = CSize(this->Width(),this->Height());
			TransBuffer((LPDWORD)pBuffer, iWidth,iHeight, (LPDWORD)(pSrcBuf),szOrigIcon.cx,szOrigIcon.cy);
			if(bReverse)
				SetDIBitsOverTurn(pBuffer, iWidth, iHeight);
		}
	}
	HBITMAP GetBitmap()
	{
		if(hResBitmap == NULL && funRes) funRes(this);
		return hResBitmap;			
	}
	LPBYTE GetByte()
	{
		if(pResBitmap == NULL && funRes) funRes(this);
		return pResBitmap;			
	}
	HIMAGELIST GetImage()
	{
		if(hResImage == NULL && funRes) funRes(this);
		return hResImage;	
	}
	HICON     GetIcon()
	{
		if(hResIcon == NULL && funRes) funRes(this);
		return hResIcon;	
	};
	HCURSOR     GetCursor()
	{
		if(hResCursor == NULL && funRes) funRes(this);
		return hResCursor;	
	};
	int Width()
	{
		switch(eCommonResType)
		{
		case TP_COMMONRES_BITMAP:
			if(hResBitmap == NULL && funRes) funRes(this);
			return stuResBmp.bmWidth;			
			break;
		case TP_COMMONRES_BMPBYE:
			if(pResBitmap == NULL && funRes) funRes(this);
			return szResBmp.cx;
			break;
		case TP_COMMONRES_IMAGELIST:
			if(hResImage == NULL && funRes) funRes(this);
			return iResWidth;
			break;			
		case TP_COMMONRES_ICON:					
			return 32;
		case TP_COMMONRES_CURSOR:	
			return 32;
		default:
			return 0;
		}
	}
	int Height()
	{
		switch(eCommonResType)
		{
		case TP_COMMONRES_BITMAP:
			return stuResBmp.bmHeight;			
			break;
		case TP_COMMONRES_BMPBYE:
			if(pResBitmap == NULL && funRes) funRes(this);
			return szResBmp.cy;
			break;
		case TP_COMMONRES_IMAGELIST:
			if(hResImage == NULL && funRes) funRes(this);
			return szResImage.cy;
			break;			
		case TP_COMMONRES_ICON:					
			return 32;
		case TP_COMMONRES_CURSOR:	
			return 32;
		default:
			return 0;
		}
	}
	~tag_TPCommonRes()
	{
		dwSafeFlag = 0;
		sResName.Empty();
		if(bLoadSucess)
		{		
			switch(eCommonResType)
			{
			case TP_COMMONRES_BITMAP:			
				::DeleteObject(hResBitmap);			
				hResBitmap = NULL;
				break;
			case TP_COMMONRES_BMPBYE:			
				if(pResBitmap) delete pResBitmap;			
				break;
			case TP_COMMONRES_ICON:			
				::DestroyIcon(hResIcon);
				hResIcon =NULL;
				break;
			case TP_COMMONRES_CURSOR:			
				::DestroyCursor(hResCursor);
				hResCursor = NULL;
				break;
			case TP_COMMONRES_PEN:					
				::DeleteObject(hPen);
				break;
			case TP_COMMONRES_BRUSH:						
				::DeleteObject(hBrush);
				break;
			case TP_COMMONRES_IMAGELIST:			
				ImageList_Destroy(hResImage);	
				break;
			default:
				break;
			}
		}
	}
}TPCommonRes;
typedef CArray<TPCommonRes *,TPCommonRes *&> TPCommonResArray;

#define  TP_PENLINE_MAX           16

struct  tag_HeadColor
{
	int iIndex;
	COLORREF clFace;
	COLORREF clFaceLeft;
	COLORREF clFaceRight;

	COLORREF clUnFace;
	COLORREF clUnFaceLeft;
	COLORREF clUnFaceRight;

	COLORREF clFaceSel;
};


class CTPMenuEx;




AFX_EXT_API  TPCommonRes *TP_LoadImage(CString sImageName,int iWidth,COLORREF clMask,BOOL bExclude = FALSE,BOOL bLoad = FALSE);
AFX_EXT_API  TPCommonRes *TP_LoadBitmap(CString sBmpName,BOOL bExclude = FALSE,BOOL bLoad = FALSE);
AFX_EXT_API  TPCommonRes *TP_LoadBmpByte(CString sBmpName,BOOL bExclude = FALSE,BOOL bLoad = FALSE);
AFX_EXT_API  TPCommonRes *TP_LoadBmpByte(CBitmap *pBitmapLoad);
AFX_EXT_API  TPCommonRes *TP_LoadIcon(CString sIconName,BOOL bExclude = FALSE,BOOL bLoad = FALSE);
AFX_EXT_API  TPCommonRes *TP_LoadCursor(CString sCursorName,BOOL bExclude = FALSE,BOOL bLoad = FALSE);
AFX_EXT_API  TPCommonRes *TP_LoadPen(TP_SYSTEM_COLOR	 clIndex,int iPenStyle,int iPenWidth,BOOL bExclude = FALSE);
AFX_EXT_API  TPCommonRes *TP_LoadBrush(TP_SYSTEM_COLOR	 clIndex,BOOL bExclude = FALSE);
AFX_EXT_API  void        TP_ReleaseCommonRes(TPCommonRes *&pCommonRes);
AFX_EXT_API  void        TP_ReloadCommonRes(TPCommonRes *pCommonRes);
AFX_EXT_API  BOOL        TP_LoadCommonRes(TPCommonRes *pCommonRes);
AFX_EXT_API  BOOL        TP_IsDefault(TPCommonRes *pCommonRes);

AFX_EXT_API  CString     TP_GetSkinPath();
AFX_EXT_API  CString     TP_GetSkinPath(HMODULE hDllHandle);
AFX_EXT_API  void        TP_SetSkinPath(CString sPath,BOOL bResetAll);
AFX_EXT_API  HCURSOR     TP_GetCtrlCursor();

AFX_EXT_API  void        TP_InitCommonRes();
AFX_EXT_API  void        TP_ReleaseCommonRes();

AFX_EXT_API  BOOL        TP_LoadPrepare();
AFX_EXT_API  BOOL        TP_ReleasePrepare();

AFX_EXT_API  void        TP_ClearMenuBar(CMenu *pMenu);
AFX_EXT_API  void        TP_ClearMenu(CMenu *pMenu);
AFX_EXT_API  void        TP_ClearMessage(HWND hWnd,BOOL bExtMsg = TRUE,BOOL bPreTrans = FALSE);
AFX_EXT_API  void        TP_ClearMessageEx(HWND hWnd, BOOL bPreTrans = FALSE);
AFX_EXT_API  void        TP_ClearMessageExEx(HWND hWnd,UINT uStart,UINT uEnd,BOOL bPreTrans = FALSE);
AFX_EXT_API  void        TP_RemoveMessage(UINT uMsg);
AFX_EXT_API  void        TP_TransMessage(UINT uMsg);
AFX_EXT_API  void        TP_ClearPaintMessage();
AFX_EXT_API  void        TP_ClearAllMessage(HWND hWnd);
AFX_EXT_API  void        TP_RemoveMessage(HWND hWnd,UINT uMsg = 0);
AFX_EXT_API  void        TP_RemoveSameMessage(HWND hWnd, UINT uMsg, WPARAM* arrWParamFiler, int nSizeWParamFiler);
AFX_EXT_API  LRESULT     TP_GetPaintRgn(CWnd *pParentWnd, CRgn *rgnPaint, BOOL bVisible);
AFX_EXT_API  void        TP_ClearSocketMessage();
AFX_EXT_API  void        TP_RemoveKeyMouseMessage();

AFX_EXT_API  CString        TP_LoadStr(LPCTSTR sText);
AFX_EXT_API  void           TP_LoadMenuStr(CTPMenuEx *pMenu);

AFX_EXT_API  tag_HeadColor *TP_GetHeadColor();