//

#include "stdafx.h"
#include "atlimage.h"
#include <list>
#include <iostream>
using namespace std;



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString           g_sSysSkinPath = _L("");
CString           g_sTemp;


CMapStringToPtr    g_aCommonRes;
TPCommonRes       *g_pDefaultImage   = NULL;
TPCommonRes       *g_pDefaultBitmap  = NULL;  
TPCommonRes       *g_pDefaultBmpByte = NULL;
TPCommonRes       *g_pDefaultIcon    = NULL;
TPCommonRes       *g_pDefaultCursor  = NULL;
HCURSOR           g_hArrowCursor     = NULL;
GUID              g_guidSkinFlag     = GUID_NULL; 

namespace _tp_commonctrl
{

	CString           g_sSysBinPath = _L("");
	CString           g_sSysSysPath = _L("");

	struct  tag_HeadColor g_stuColorSet[TP_PENLINE_MAX] = 
	{
		{ 0,  RGB(168,116,201), RGB(203,177,215), RGB(86,56,102),   RGB(97,63,114),   RGB(113,94,120),  RGB(58,36,66),  RGB(210,175,232)},
		{ 1,  RGB(175,151,185), RGB(201,183,209), RGB(86,65,94),    RGB(69,61,72),    RGB(81,77,84),    RGB(33,29,36),  RGB(213,192,226)},
		{ 2,  RGB(210,183,175), RGB(229,212,206), RGB(162,141,133), RGB(85,63,62),    RGB(103,86,85),   RGB(38,25,25),  RGB(237,224,223)},
		{ 3,  RGB(194,144,125), RGB(210,190,180), RGB(97,73,61),    RGB(92,67,56),    RGB(97,87,80),    RGB(53,40,32),  RGB(244,193,181)},
		{ 4,  RGB(179,169,156), RGB(204,197,187), RGB(89,81,69),    RGB(70,68,63),    RGB(82,80,78),    RGB(34,32,30),  RGB(220,208,197)},
		{ 5,  RGB(187,170,130), RGB(208,201,183), RGB(94,86,63),    RGB(106,97,72),   RGB(115,112,98),  RGB(61,57,41),  RGB(222,211,184)},
		{ 6,  RGB(223,216,94),  RGB(216,216,143), RGB(113,113,44),  RGB(77,77,36),    RGB(78,77,51),    RGB(42,44,21),  RGB(245,241,161)},
		{ 7,  RGB(211,206,107), RGB(223,220,163), RGB(115,112,52),  RGB(120,120,58),  RGB(130,132,94),  RGB(61,61,18),  RGB(233,231,181)},
		{ 8,  RGB(169,201,116), RGB(199,215,177), RGB(83,102,56),   RGB(73,89,52),    RGB(86,95,76),    RGB(40,50,29),  RGB(210,232,175)},
		{ 9,  RGB(170,185,151), RGB(198,209,183), RGB(81,94,65),    RGB(67,72,61),    RGB(81,84,77),    RGB(33,36,29),  RGB(214,226,192)},
		{ 10, RGB(151,185,161), RGB(183,209,190), RGB(65,94,74),    RGB(61,72,64),    RGB(77,84,79),    RGB(29,36,31),  RGB(192,226,199)},
		{ 11, RGB(116,201,172), RGB(177,215,204), RGB(56,102,89),   RGB(50,83,74),    RGB(72,89,85),    RGB(28,48,44),  RGB(175,232,213)},
		{ 12, RGB(164,178,173), RGB(200,206,204), RGB(98,105,103),  RGB(37,58,52),    RGB(52,63,60),    RGB(21,32,30),  RGB(205,214,211)},
		{ 13, RGB(149,157,186), RGB(189,194,211), RGB(79,83,104),   RGB(60,63,73),    RGB(77,79,85),    RGB(29,31,37),  RGB(189,202,230)},
		{ 14, RGB(116,136,201), RGB(177,184,215), RGB(56,64,102),   RGB(63,72,114),   RGB(94,97,120),   RGB(36,40,66),  RGB(175,188,232)},
		{ 15, RGB(130,167,187), RGB(183,197,208), RGB(63,81,94),    RGB(58,74,86),    RGB(79,86,93),    RGB(33,41,69),  RGB(184,208,222)},
	};



	void TP_TestDC()
	{
#ifndef TP_NS_RELEASE
		HDC hDC = GetDC(NULL);
		INT iError = 0;
		if(NULL == hDC)
		{
			iError = GetLastError();
		}

		ASSERT(FALSE);
#endif
	}
	extern HBITMAP	TP_LoadImageBitmapFile(LPCTSTR	sFileName);
	CString         TP_ReLoadFile(CString &sFile)
	{
		CString sPath = (CString)TP_GetTempPath() + _T("\\Icon") + sFile.Mid(sFile.ReverseFind('\\'));
		::DeleteFile(sPath);
		::CopyFile(sFile,sPath,FALSE);
		return sPath;
	}
	tag_HeadColor *TP_GetHeadColor()
	{
		return g_stuColorSet;
	}
	BOOL         TP_LoadCommonRes(TPCommonRes *pCommonRes)
	{	
		if(pCommonRes ->eCommonResType == TP_COMMONRES_BITMAP &&pCommonRes ->hResBitmap == NULL )
		{
			HBITMAP hResBitmap = (HBITMAP)::LoadImage(g_hThisInstance,pCommonRes ->sResName,IMAGE_BITMAP,0,0, LR_DEFAULTSIZE|LR_CREATEDIBSECTION | LR_LOADFROMFILE);			
			if(hResBitmap == NULL)
			{
				pCommonRes ->bLoadSucess    = FALSE;
				pCommonRes ->hResBitmap     = g_pDefaultBitmap ->hResBitmap;
				pCommonRes ->stuResBmp         = g_pDefaultBitmap ->stuResBmp;			
			}
			else
			{
				pCommonRes ->bLoadSucess = TRUE;
				pCommonRes ->hResBitmap     = hResBitmap;
				CBitmap::FromHandle(pCommonRes ->hResBitmap)->GetBitmap(&pCommonRes ->stuResBmp);
			}				
		}
		else if(pCommonRes ->eCommonResType == TP_COMMONRES_BMPBYE &&pCommonRes ->pResBitmap == NULL )
		{
			LPDWORD pBuffer = NULL; BITMAP bmpInfo;
			BOOL blGet = TP_LoadBitmap(pCommonRes ->sResName,pBuffer,bmpInfo);
			if(pBuffer == NULL)
			{
				pCommonRes ->bLoadSucess = FALSE; 
				pCommonRes ->pResBitmap     = g_pDefaultBmpByte ->pResBitmap;
				pCommonRes ->szResBmp       = g_pDefaultBmpByte ->szResBmp;			
			}
			else 
			{
				pCommonRes ->bLoadSucess = TRUE;
				pCommonRes ->pResBitmap     = (LPBYTE)pBuffer;
				pCommonRes ->szResBmp       = CSize(bmpInfo.bmWidth,bmpInfo.bmHeight);

			}
		}
		else if(pCommonRes ->eCommonResType == TP_COMMONRES_IMAGELIST && pCommonRes ->hResImage == NULL)
		{
			HBITMAP hResBitmap = TP_LoadImageBitmapFile(pCommonRes ->sResName);/* (HBITMAP)::LoadImage(g_hThisInstance,pCommonRes ->sResName,IMAGE_BITMAP,0,0, LR_DEFAULTSIZE|LR_CREATEDIBSECTION | LR_LOADFROMFILE);			*/			
			if(hResBitmap  == NULL && PathFileExists(pCommonRes ->sResName)) 
			{
				hResBitmap = (HBITMAP)::LoadImage(g_hThisInstance,TP_ReLoadFile(pCommonRes ->sResName),IMAGE_BITMAP,0,0, LR_DEFAULTSIZE|LR_CREATEDIBSECTION | LR_LOADFROMFILE);					
			}			
			if(hResBitmap == NULL) 
			{
				pCommonRes ->iResWidth         = g_pDefaultImage ->iResWidth;
				pCommonRes ->clMask            = g_pDefaultImage ->clMask;
				pCommonRes ->szResImage        = g_pDefaultImage ->szResImage;
				pCommonRes ->hResImage         = g_pDefaultImage ->hResImage;
				pCommonRes ->bLoadSucess       = FALSE; 
			}
			else
			{
				BITMAP  bitmap;	
				ZeroMemory(&bitmap,sizeof(BITMAP));
				::GetObject(hResBitmap, sizeof(BITMAP),&bitmap);					
				pCommonRes ->szResImage        = CSize(bitmap.bmWidth,bitmap.bmHeight);
				pCommonRes ->hResImage         = ImageList_Create(pCommonRes ->iResWidth,bitmap.bmHeight,(bitmap.bmBitsPixel == 24 ? ILC_COLOR24 : ILC_COLOR32)|ILC_MASK ,1,1);
				pCommonRes ->bLoadSucess       = TRUE;
				ImageList_AddMasked(pCommonRes ->hResImage,hResBitmap,pCommonRes ->clMask);
				::DeleteObject(hResBitmap);				
			}		
		}
		else if(pCommonRes ->eCommonResType == TP_COMMONRES_CURSOR && pCommonRes ->hResCursor == NULL)
		{
			HCURSOR hResIcon = (HCURSOR)::LoadImage(g_hThisInstance, pCommonRes ->sResName,IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_LOADFROMFILE);
			if(hResIcon == NULL && PathFileExists(pCommonRes ->sResName))
			{
				TP_TestDC();
				hResIcon = (HCURSOR)::LoadImage(g_hThisInstance,TP_ReLoadFile(pCommonRes ->sResName),IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_LOADFROMFILE);
			}
			if(hResIcon == NULL) 
			{
				pCommonRes ->bLoadSucess      = FALSE;
				pCommonRes ->hResCursor          = g_pDefaultCursor ->hResCursor;			
			}
			else
			{
				pCommonRes ->bLoadSucess      = TRUE;
				pCommonRes ->hResCursor          = hResIcon;
				//pCommonRes ->hResCursor          = CopyCursor(hResIcon);
				//::DestroyCursor(hResIcon);
			}
		}
		else if(pCommonRes ->eCommonResType == TP_COMMONRES_ICON && pCommonRes ->hResIcon == NULL)		
		{		
			HICON hResIcon = (HICON)::LoadImage(g_hThisInstance, pCommonRes ->sResName,IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_LOADFROMFILE);		
			if(hResIcon == NULL && PathFileExists(pCommonRes ->sResName))
			{
				TP_TestDC();
				 hResIcon = (HICON)::LoadImage(g_hThisInstance, TP_ReLoadFile(pCommonRes ->sResName),IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_LOADFROMFILE);		
			}
			if(hResIcon == NULL)       
			{
				pCommonRes ->bLoadSucess    = FALSE;
				pCommonRes ->hResIcon          = g_pDefaultIcon->hResIcon;
			}
			else
			{
				pCommonRes ->bLoadSucess    = TRUE;
				pCommonRes ->hResIcon          = hResIcon;
				//pCommonRes ->hResIcon          = ::CopyIcon(hResIcon);
				//::DestroyIcon(hResIcon);
			}			
		}	

		return TRUE;
	}

	TPCommonRes *TP_LoadImage(CString sResName,int iWidth,COLORREF clMask,BOOL bExclude,BOOL bLoad)
	{
		if(!bExclude)
		{
			TPCommonRes *pFind =NULL;
			if(g_aCommonRes.Lookup(sResName,(void *&)pFind) && pFind)
			{
				pFind ->dwRefCount ++; 
				return pFind;
			}			
		}
		if(bLoad)
		{		
			HBITMAP hResBitmap = TP_LoadImageBitmapFile(sResName);/*(HBITMAP)::LoadImage(g_hThisInstance,sResName,IMAGE_BITMAP,0,0, LR_DEFAULTSIZE|LR_CREATEDIBSECTION | LR_LOADFROMFILE);*/
			if(hResBitmap  == NULL && PathFileExists(sResName))
			{
				hResBitmap = (HBITMAP)::LoadImage(g_hThisInstance,TP_ReLoadFile(sResName),IMAGE_BITMAP,0,0, LR_DEFAULTSIZE|LR_CREATEDIBSECTION | LR_LOADFROMFILE);					
			}
			TPCommonRes *pImage = new TPCommonRes();  
			pImage ->funRes         = TP_LoadCommonRes;
			pImage ->eCommonResType = TP_COMMONRES_IMAGELIST;
			pImage ->sResName       = sResName;
			pImage ->dwRefCount     = 1;
			pImage ->bExclude       = bExclude;
			if(hResBitmap == NULL) 
			{
				pImage ->iResWidth      = g_pDefaultImage ->iResWidth;
				pImage ->clMask         = g_pDefaultImage ->clMask;
				pImage ->szResImage     = g_pDefaultImage ->szResImage;
				pImage ->hResImage      = g_pDefaultImage ->hResImage;
				pImage ->bLoadSucess    = FALSE; 
			}
			else
			{
				BITMAP  bitmap;	
				ZeroMemory(&bitmap,sizeof(BITMAP));
				::GetObject(hResBitmap, sizeof(BITMAP),&bitmap);	
				pImage ->iResWidth         = iWidth;
				pImage ->clMask            = clMask;
				pImage ->szResImage        = CSize(bitmap.bmWidth,bitmap.bmHeight);
				pImage ->hResImage         = ImageList_Create(iWidth,bitmap.bmHeight,(bitmap.bmBitsPixel == 24 ? ILC_COLOR24 : ILC_COLOR32)|ILC_MASK ,1,1);
				pImage ->bLoadSucess       = TRUE;
				ImageList_AddMasked(pImage ->hResImage,hResBitmap,clMask);
				::DeleteObject(hResBitmap);				
			}		
			if(!bExclude)	g_aCommonRes.SetAt(sResName,pImage);
			return pImage;
		}
		else
		{
			TPCommonRes *pImage = new TPCommonRes();  
			pImage ->funRes         = TP_LoadCommonRes;
			pImage ->eCommonResType = TP_COMMONRES_IMAGELIST;
			pImage ->sResName       = sResName;
			pImage ->dwRefCount     = 1;
			pImage ->bExclude       = bExclude; 	
			pImage ->hResImage      = NULL;
			pImage ->bLoadSucess    = FALSE;   

			pImage ->iResWidth      = iWidth;
			pImage ->clMask         = clMask;
			if(!bExclude)	g_aCommonRes.SetAt(sResName,pImage);
			return pImage;
		}
	}
  	TPCommonRes *TP_LoadBitmap(CString sResName,BOOL bExclude,BOOL bLoad)
	{
		if(!bExclude)		
		{
			TPCommonRes *pFind =NULL;
			if(g_aCommonRes.Lookup(sResName,(void *&)pFind) && pFind)
			{
				pFind ->dwRefCount ++; 
				return pFind;
			}		
		}
		if(bLoad)
		{		
			HBITMAP hResBitmap = (HBITMAP)::LoadImage(g_hThisInstance,sResName,IMAGE_BITMAP,0,0, LR_DEFAULTSIZE|LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			TPCommonRes *pResBitmap = new TPCommonRes();  
			pResBitmap ->eCommonResType = TP_COMMONRES_BITMAP;
			pResBitmap ->sResName       = sResName;
			pResBitmap ->dwRefCount     = 1;
			pResBitmap ->bExclude       = bExclude;
			pResBitmap ->TransBuffer   = TP_TransBuffer;
			pResBitmap ->SetDIBitsOverTurn   = TP_SetDIBitsOverTurn;
			if(hResBitmap == NULL)
			{
				pResBitmap ->bLoadSucess = FALSE;
				pResBitmap ->hResBitmap     = g_pDefaultBitmap ->hResBitmap;
				pResBitmap ->stuResBmp         = g_pDefaultBitmap ->stuResBmp;			
			}
			else
			{
				pResBitmap ->bLoadSucess = TRUE;
				pResBitmap ->hResBitmap     = hResBitmap;
				CBitmap::FromHandle(pResBitmap ->hResBitmap)->GetBitmap(&pResBitmap ->stuResBmp);
			}
			if(!bExclude)	g_aCommonRes.SetAt(sResName,pResBitmap);
			return pResBitmap;		
		}
		else
		{
			TPCommonRes *pResBitmap = new TPCommonRes();  
			pResBitmap ->eCommonResType = TP_COMMONRES_BITMAP;
			pResBitmap ->sResName       = sResName;
			pResBitmap ->dwRefCount     = 1;
			pResBitmap ->bExclude       = bExclude;
			pResBitmap ->TransBuffer   = TP_TransBuffer;
			pResBitmap ->SetDIBitsOverTurn   = TP_SetDIBitsOverTurn;
			pResBitmap ->bLoadSucess         = FALSE;
			pResBitmap ->hResBitmap          = NULL;

			if(!bExclude)	g_aCommonRes.SetAt(sResName,pResBitmap);
			return pResBitmap;		
		}
	}
	void        TP_ReloadCommonRes(TPCommonRes *pCommonRes)
	{
		if(pCommonRes == NULL) return;
		if(!pCommonRes ->bLoadSucess) return;
		switch(pCommonRes ->eCommonResType)
		{
		case TP_COMMONRES_BITMAP:			
			break;
		case TP_COMMONRES_BMPBYE:			
			break;
		case TP_COMMONRES_ICON:						
			break;
		case TP_COMMONRES_CURSOR:						
			break;
		case TP_COMMONRES_PEN:					
			::DeleteObject(pCommonRes ->hPen);
			pCommonRes ->hPen = CreatePen(pCommonRes->iPenStyle,pCommonRes->iPenWidth,TP_GetSysColor(pCommonRes->clPenIndex));
			break;
		case TP_COMMONRES_BRUSH:									
			break;
		case TP_COMMONRES_IMAGELIST:						
			break;
		default:
			break;
		}
		
	}
	void        TP_ReleaseCommonRes(TPCommonRes *&pCommonRes)
	{	
		if(TP_IsNull((DWORD_PTR)pCommonRes)) 
			return;
		pCommonRes ->dwRefCount --;	
		if(pCommonRes ->dwRefCount <=0)
		{
			if(!pCommonRes ->bExclude)
				g_aCommonRes.RemoveKey(pCommonRes->sResName); 			
			delete pCommonRes;			
			pCommonRes = NULL;
		}
	}
	TPCommonRes *TP_LoadBmpByte(CBitmap *pBitmapLoad)
	{
		if(pBitmapLoad == NULL || pBitmapLoad ->m_hObject == NULL)  return NULL;
		TPCommonRes *pResBitmap      = new TPCommonRes();  
		pResBitmap ->eCommonResType = TP_COMMONRES_BMPBYE;		
		pResBitmap ->bLoadSucess    = TRUE; 
		pResBitmap ->pResBitmap        = TP_GetBitmapBits((HBITMAP)pBitmapLoad ->GetSafeHandle(),pResBitmap ->szResBmp,NULL);	
		pResBitmap ->sResName       = _T("Private");
		pResBitmap ->dwRefCount     = 1;	
		pResBitmap ->bExclude       = 1;
		return pResBitmap;
	}

	TPCommonRes *TP_LoadBmpByte(CString sResName,BOOL bExclude,BOOL bLoad)
	{
		if(!bExclude)
		{
			TPCommonRes *pFind =NULL;
			if(g_aCommonRes.Lookup(sResName,(void *&)pFind) && pFind)
			{				
				if (pFind->pResBitmap != NULL)
				{
					pFind ->dwRefCount ++; 
					return pFind;
				}				
			}		
		}
		if(bLoad)
		{		
			LPDWORD pBuffer = NULL; BITMAP bmpInfo;
			ZeroMemory(&bmpInfo,sizeof(BITMAP));
			BOOL blGet = TP_LoadBitmap(sResName,pBuffer,bmpInfo);

			TPCommonRes *pResBitmap     = new TPCommonRes();  
			pResBitmap ->funRes         = TP_LoadCommonRes;
			pResBitmap ->eCommonResType = TP_COMMONRES_BMPBYE;		
			pResBitmap ->sResName       = sResName;
			pResBitmap ->dwRefCount     = 1;
			pResBitmap ->bExclude       = bExclude;
			pResBitmap ->TransBuffer   = TP_TransBuffer;
			pResBitmap ->SetDIBitsOverTurn   = TP_SetDIBitsOverTurn;
			if(pBuffer == NULL)
			{
				pResBitmap ->bLoadSucess = FALSE; 
				pResBitmap ->pResBitmap     = g_pDefaultBmpByte ->pResBitmap;
				pResBitmap ->szResBmp       = g_pDefaultBmpByte ->szResBmp;			
			}
			else 
			{
				pResBitmap ->bLoadSucess = TRUE;
				pResBitmap ->pResBitmap     = (LPBYTE)pBuffer;
				pResBitmap ->szResBmp       = CSize(bmpInfo.bmWidth,bmpInfo.bmHeight);

			}
			if(!bExclude)	g_aCommonRes.SetAt(sResName,pResBitmap);

			return pResBitmap;
		}
		else
		{
			TPCommonRes *pResBitmap     = new TPCommonRes();  
			pResBitmap ->funRes         = TP_LoadCommonRes;
			pResBitmap ->eCommonResType = TP_COMMONRES_BMPBYE;		
			pResBitmap ->sResName       = sResName;
			pResBitmap ->dwRefCount     = 1;
			pResBitmap ->bExclude       = bExclude;
			pResBitmap ->TransBuffer   = TP_TransBuffer;
			pResBitmap ->SetDIBitsOverTurn   = TP_SetDIBitsOverTurn;
			pResBitmap ->bLoadSucess    = FALSE;   
			pResBitmap ->pResBitmap        = NULL;
			pResBitmap ->szResBmp          = CSize(0,0);		

			if(!bExclude)	g_aCommonRes.SetAt(sResName,pResBitmap);

			return pResBitmap;
		}
		
	}
	TPCommonRes *TP_LoadIcon(CString sResName,BOOL bExclude,BOOL bLoad)
	{
		if(!bExclude)
		{
			TPCommonRes *pFind =NULL;
			if(g_aCommonRes.Lookup(sResName,(void *&)pFind) && pFind)
			{
				pFind ->dwRefCount ++; 
				return pFind;
			}		
		}
		if(bLoad)
		{		
			HICON hResIcon = (HICON)::LoadImage(g_hThisInstance, sResName,IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_LOADFROMFILE);
			if(hResIcon == NULL && PathFileExists(sResName)) 
			{
				TP_TestDC();
				hResIcon = (HICON)::LoadImage(g_hThisInstance, TP_ReLoadFile(sResName),IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_LOADFROMFILE);				
			}

			TPCommonRes *pCommonres     = new TPCommonRes();  
			pCommonres  ->funRes        = TP_LoadCommonRes;
			pCommonres ->eCommonResType = TP_COMMONRES_ICON;	
			pCommonres ->sResName       = sResName;
			pCommonres ->dwRefCount     = 1;
			pCommonres ->bExclude       = bExclude;

			if(hResIcon == NULL)       
			{
				pCommonres ->bLoadSucess    = FALSE;
				pCommonres ->hResIcon          = g_pDefaultIcon->hResIcon;
			}
			else
			{
				pCommonres ->bLoadSucess    = TRUE;
				pCommonres ->hResIcon          = hResIcon;
				//pCommonres ->hResIcon          = ::CopyIcon(hResIcon);
				//::DestroyIcon(hResIcon);
			}

			if(!bExclude)	g_aCommonRes.SetAt(sResName,pCommonres);
			return pCommonres;
		}
		else
		{		

			TPCommonRes *pCommonres     = new TPCommonRes();  
			pCommonres  ->funRes        = TP_LoadCommonRes;
			pCommonres ->eCommonResType = TP_COMMONRES_ICON;	
			pCommonres ->sResName       = sResName;
			pCommonres ->dwRefCount     = 1;
			pCommonres ->bExclude       = bExclude;
			pCommonres ->bLoadSucess    = FALSE;   
			pCommonres ->hResIcon          = NULL;
			if(!bExclude)	g_aCommonRes.SetAt(sResName,pCommonres);
			return pCommonres;
		}
	}
	TPCommonRes *TP_LoadCursor(CString sResName,BOOL bExclude,BOOL bLoad)
	{
		if(!bExclude)
		{
			TPCommonRes *pFind =NULL;
			if(g_aCommonRes.Lookup(sResName,(void *&)pFind) && pFind)
			{
				pFind ->dwRefCount ++; 
				return pFind;
			}		
		}
		if(bLoad)
		{		
			HCURSOR hResIcon = (HCURSOR)::LoadImage(g_hThisInstance, sResName,IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_LOADFROMFILE);
			if(hResIcon == NULL && PathFileExists(sResName)) 
			{
				TP_TestDC();
				hResIcon = (HCURSOR)::LoadImage(g_hThisInstance, TP_ReLoadFile(sResName),IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_LOADFROMFILE);
			}
			TPCommonRes *pCommonres      = new TPCommonRes();  
			pCommonres ->funRes          = TP_LoadCommonRes;
			pCommonres ->eCommonResType  = TP_COMMONRES_CURSOR;	
			pCommonres ->sResName        = sResName;
			pCommonres ->dwRefCount      = 1;
			pCommonres ->bExclude        = bExclude;
			if(hResIcon == NULL) 
			{
				pCommonres ->bLoadSucess      = FALSE;
				pCommonres ->hResCursor          = g_pDefaultCursor ->hResCursor;			
			}
			else
			{
				pCommonres ->bLoadSucess      = TRUE;			
				pCommonres ->hResCursor          = hResIcon;
				//pCommonres ->hResCursor          = CopyCursor(hResIcon);
				//::DestroyCursor(hResIcon);
			}
			if(!bExclude)	g_aCommonRes.SetAt(sResName,pCommonres);
			return pCommonres;
		}
		else
		{			

			TPCommonRes *pCommonres      = new TPCommonRes(); 
			pCommonres ->funRes          = TP_LoadCommonRes;
			pCommonres ->eCommonResType  = TP_COMMONRES_CURSOR;	
			pCommonres ->sResName        = sResName;
			pCommonres ->dwRefCount      = 1;
			pCommonres ->bExclude        = bExclude;
			pCommonres ->bLoadSucess     = FALSE;   
			pCommonres ->hResCursor         = NULL;			
			if(!bExclude)	g_aCommonRes.SetAt(sResName,pCommonres);
			return pCommonres;
		}
	}
	TPCommonRes *TP_LoadPen(TP_SYSTEM_COLOR	 clIndex,int iPenStyle,int iPenWidth,BOOL bExclude)
	{
		CString sName;sName.Format(_L("%d-%d-%d"),clIndex,iPenStyle,iPenWidth);
		if(!bExclude)
		{
			TPCommonRes *pFind =NULL;
			if(g_aCommonRes.Lookup(sName,(void *&)pFind) && pFind)
			{
				pFind ->dwRefCount ++; 
				return pFind;
			}	
		}

		TPCommonRes *pCommonres      = new TPCommonRes();  
		pCommonres ->eCommonResType = TP_COMMONRES_PEN;	

		pCommonres ->sResName    = sName;
		pCommonres ->iPenStyle   = iPenStyle;
		pCommonres ->iPenWidth   = iPenWidth;
		pCommonres ->clPenIndex  = clIndex;
		if(!(iPenStyle&PS_ENDCAP_MASK))
			pCommonres ->hPen = ::CreatePen(iPenStyle,iPenWidth,TP_GetSysColor(clIndex));
		else
		{
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = TP_GetSysColor(clIndex);
			pCommonres ->hPen = ::ExtCreatePen(iPenStyle|PS_GEOMETRIC, iPenWidth, &logBrush, 0, NULL);
		}

		pCommonres ->dwRefCount       = 1;
		if(!bExclude)   g_aCommonRes.SetAt(sName,pCommonres);

		return pCommonres;
	}
	TPCommonRes *TP_LoadBrush(TP_SYSTEM_COLOR	 clIndex,BOOL bExclude)
	{
		CString sName;sName.Format(_L("%d-%d"),clIndex,1234567);
		if(!bExclude)
		{			
			TPCommonRes *pFind =NULL;
			if(g_aCommonRes.Lookup(sName,(void *&)pFind) && pFind)
			{
				pFind ->dwRefCount ++; 
				return pFind;
			}	
		}
		TPCommonRes *pCommonres      = new TPCommonRes();  
		pCommonres ->eCommonResType = TP_COMMONRES_BRUSH;	

		pCommonres ->sResName    = sName;
		pCommonres ->clBrIndex   = clIndex;
		pCommonres ->hBrush      = ::CreateSolidBrush(TP_GetSysColor(clIndex));
		pCommonres ->dwRefCount  = 1;

		if(!bExclude)   g_aCommonRes.SetAt(sName,pCommonres);
		return pCommonres;
		
	}
	CString     TP_GetSkinPath()
	{
		return g_sSysSkinPath;
	}
	CString     TP_GetSkinPath(HMODULE hDllHandle)
	{
		if(hDllHandle == NULL) return g_sSysSkinPath;

		TCHAR cPath[MAX_PATH];
		::GetModuleFileName(hDllHandle,cPath,MAX_PATH);
		PathRemoveFileSpec(cPath);
		CString sSysSkinPath = cPath;
		int iFind = sSysSkinPath.ReverseFind('\\');
		sSysSkinPath = sSysSkinPath.Left(iFind);
		sSysSkinPath += _L("\\Sys\\TPSkin");
		return sSysSkinPath;
	}
	void        TP_SetSkinPath(CString sPath,BOOL bResetAll)
	{
		g_sSysSkinPath = sPath;	
	}
	BOOL        TP_IsDefault(TPCommonRes *pCommonRes)
	{
		if(TP_IsNull((DWORD_PTR)pCommonRes)) return TRUE;
//		if(pCommonRes ->bLoadSucess == FALSE) return TRUE;
		switch(pCommonRes ->eCommonResType)
		{
		case TP_COMMONRES_BITMAP:		
			return BOOL(pCommonRes ->hResBitmap == g_pDefaultBitmap ->hResBitmap);			
		case TP_COMMONRES_BMPBYE:					
			return BOOL(pCommonRes ->GetByte() == g_pDefaultBmpByte->pResBitmap);
		case TP_COMMONRES_ICON:					
			return BOOL(pCommonRes->GetIcon()    == g_pDefaultIcon ->hResIcon);
		case TP_COMMONRES_CURSOR:						
			return BOOL(pCommonRes ->GetCursor() == g_pDefaultCursor->hResCursor);
		case TP_COMMONRES_IMAGELIST:						
			return BOOL(pCommonRes ->GetImage()  == g_pDefaultImage->hResImage);
		default:
			return FALSE;			
		}
		return FALSE;
	}
	void        TP_InitCommonRes()
	{

		g_pDefaultImage = new TPCommonRes();  
		g_pDefaultImage ->eCommonResType = TP_COMMONRES_IMAGELIST;
		g_pDefaultImage ->szResImage     = CSize(80,48);	
		g_pDefaultImage ->iResWidth      = 80; 
		HINSTANCE hInst = AfxFindResourceHandle((LPCTSTR)(DWORD_PTR)_L("IDB_DEFAULTBMP"), RT_BITMAP);
		g_pDefaultImage ->hResImage = ImageList_LoadBitmap(hInst,(LPCTSTR)(DWORD_PTR)_L("IDB_DEFAULTBMP"), 80, 1, RGB(255,255,255));
		g_pDefaultImage ->dwRefCount    = 1;



		g_pDefaultBitmap = new TPCommonRes();  
		g_pDefaultBitmap ->eCommonResType = TP_COMMONRES_BITMAP;
		g_pDefaultBitmap ->hResBitmap = LoadBitmap(g_hThisInstance,_L("IDB_DEFAULTBMP"));
		CBitmap::FromHandle(g_pDefaultBitmap ->hResBitmap) ->GetBitmap(&g_pDefaultBitmap ->stuResBmp);
		g_pDefaultBitmap ->dwRefCount    = 1;



		g_pDefaultBmpByte = new TPCommonRes();  
		g_pDefaultBmpByte ->eCommonResType = TP_COMMONRES_BMPBYE;	
		CBitmap hBmpTemp;
		hBmpTemp.LoadBitmap(_L("IDB_DEFAULTBMP"));	
		g_pDefaultBmpByte ->pResBitmap = TP_GetBitmapBits((HBITMAP)hBmpTemp,g_pDefaultBmpByte ->szResBmp,NULL);
		hBmpTemp.DeleteObject();
		g_pDefaultBmpByte ->dwRefCount    = 1;


		g_pDefaultIcon = new TPCommonRes();  
		g_pDefaultIcon ->eCommonResType = TP_COMMONRES_ICON;	
		g_pDefaultIcon ->hResIcon =  ::LoadIcon(g_hThisInstance,_L("IDI_DEFAULTICON"));
		g_pDefaultIcon ->dwRefCount    = 1;

		g_pDefaultCursor = new TPCommonRes();  
		g_pDefaultCursor ->eCommonResType = TP_COMMONRES_CURSOR;	
		g_pDefaultCursor ->hResCursor =  ::LoadCursor(g_hThisInstance,_L("IDC_DEFAULTCURSOR"));	
		g_pDefaultCursor ->dwRefCount    = 1;

#ifndef TP_NS_EDIT
		g_hArrowCursor             =  ::LoadCursor(g_hThisInstance,_L("IDC_DEFAULTCURSOR"));	
#else
		g_hArrowCursor             =  ::LoadCursor(g_hThisInstance,_L("IDC_DEFAULTCURSORNS"));	
#endif


		TCHAR cPath[MAX_PATH];
		::GetModuleFileName(g_hThisInstance,cPath,MAX_PATH);
		PathRemoveFileSpec(cPath);
		g_sSysSkinPath = cPath;
		g_sSysBinPath  = cPath;
		if(PathFileExists(g_sSysSkinPath + _L("\\Sys")))
		{
			g_sSysSkinPath += _L("\\Sys");
			g_sSysSysPath = g_sSysSkinPath;
		}
		else
		{
			int  iFind = g_sSysSkinPath.ReverseFind('\\');
			if(iFind >0)
			{
				g_sSysSkinPath = g_sSysSkinPath.Left(iFind);		
			}
			g_sSysSkinPath += _L("\\Sys");
			g_sSysSysPath = g_sSysSkinPath;			

			if(!PathFileExists(g_sSysSysPath))	    ::CreateDirectory(g_sSysSysPath,NULL);	
		}
		CString SysTempPath = g_sSysSysPath + _L("\\Temp");
		if(!PathFileExists(SysTempPath))
		{
			::CreateDirectory(SysTempPath ,NULL);
			::CreateDirectory(SysTempPath + _L("\\TimeLine"),NULL);
			::CreateDirectory(SysTempPath + _L("\\Icon"),NULL);
			::CreateDirectory(SysTempPath + _L("\\Res"),NULL);			
		}	
		else
		{
			if(!PathFileExists(SysTempPath + _L("\\TimeLine")))
				::CreateDirectory(SysTempPath + _L("\\TimeLine"),NULL);
			if(!PathFileExists(SysTempPath + _L("\\Icon")))
				::CreateDirectory(SysTempPath + _L("\\Icon"),NULL);
			if(!PathFileExists(SysTempPath + _L("\\Res")))
				::CreateDirectory(SysTempPath + _L("\\Res"),NULL);
		}		
#ifdef TP_NS_EDIT
		g_sSysSkinPath += _L("\\TPSkin_NS");
#else
		g_sSysSkinPath += _L("\\TPSkin");
#endif
		CoCreateGuid(&g_guidSkinFlag);

	}
	void        TP_ReleaseCommonRes()
	{
		POSITION     posGet = g_aCommonRes.GetStartPosition();
		CString      sName  = _L("");   
		TPCommonRes *pFind  = NULL;
		while (posGet)
		{
			g_aCommonRes.GetNextAssoc(posGet,sName,(void *&)pFind);
			if(pFind)	
			{
#ifdef _DEBUG
				CString sText;sText.Format(_T("   %d"),pFind->dwRefCount);
				AfxMessageBox(pFind ->sResName + sText);	
#endif
				//delete pFind;
			}
		}	
		g_aCommonRes.RemoveAll();

		if(g_pDefaultImage) delete g_pDefaultImage;
		g_pDefaultImage = NULL;
		if(g_pDefaultBitmap)  delete g_pDefaultBitmap;
		g_pDefaultBitmap = NULL;
		if(g_pDefaultBmpByte) delete g_pDefaultBmpByte;
		g_pDefaultBmpByte = NULL;
		if(g_pDefaultIcon)    delete g_pDefaultIcon;
		g_pDefaultIcon = NULL;
		if(g_pDefaultCursor)  delete g_pDefaultCursor;
		g_pDefaultCursor = NULL;

		::DestroyCursor(g_hArrowCursor);
		g_hArrowCursor = NULL;		
	}

    void        TP_RemoveMessage(HWND hWnd,UINT uMsg)
	{
		MSG GetMsg;
		while(PeekMessage(&GetMsg,hWnd,uMsg,uMsg,PM_REMOVE))
		{		
		//	TranslateMessage(&GetMsg);
		//	DispatchMessage(&GetMsg);
		}				
	}	
	void        TP_ClearAllMessage(HWND hWnd)
	{
		UINT uMsg[] = {WM_LBUTTONDOWN,WM_LBUTTONUP,WM_MOUSEMOVE,WM_NCLBUTTONDOWN,WM_NCLBUTTONUP,WM_NCMOUSEMOVE,WM_SYSCOMMAND,WM_KEYDOWN,WM_KEYUP,WM_RBUTTONDOWN,WM_RBUTTONUP};
		MSG GetMsg;
		while(PeekMessage(&GetMsg,hWnd,0,0,PM_REMOVE))
		{	
			for(INT l=0;l<sizeof(uMsg)/sizeof(UINT);l++)
			{
				if(uMsg[l] == GetMsg.message)
				{
					if(!(hWnd == GetMsg.hwnd || hWnd == GetParent(GetMsg.hwnd))) goto NextMsg;
				}
			}
			TranslateMessage(&GetMsg);
			DispatchMessage(&GetMsg);
NextMsg:
			continue;
		}				
	}
	BOOL g_bRemoveSameMessage = FALSE;
	void        TP_RemoveSameMessage(HWND hWnd, UINT uMsg, WPARAM* arrWParamFiler, int nSizeWParamFiler)
	{
		if(g_bRemoveSameMessage) return  ;
		g_bRemoveSameMessage = TRUE;//进入时加锁，防止DispatchMessage时死循环

		static list<MSG>	listMSG;
		MSG					msgPeek;
		while(PeekMessage(&msgPeek,hWnd, uMsg, uMsg, PM_REMOVE))
		{
			if(arrWParamFiler)
			{
				INT l=0;
				for(; l<nSizeWParamFiler && arrWParamFiler[l]!=msgPeek.wParam; ++l) {	} // Check if match
				if(l>=nSizeWParamFiler)
				{
					TranslateMessage(&msgPeek);
					DispatchMessage(&msgPeek);
					continue;
				}
			}
			list<MSG>::iterator itFind = listMSG.begin();
			for (; itFind!=listMSG.end() && itFind->wParam!=msgPeek.wParam; ++itFind)	{	} // Check if match
			if (itFind!=listMSG.end())	listMSG.erase(itFind);
			listMSG.push_back(msgPeek);
		}
		while (!listMSG.empty())
		{
			MSG msgCurrent = *listMSG.begin();
			listMSG.pop_front();
			TranslateMessage(&msgCurrent);
			DispatchMessage(&msgCurrent);
		}

		g_bRemoveSameMessage = FALSE;//解锁	 
	}

    void        TP_RemoveMessage(UINT uMsg)
	{
		MSG GetMsg;
		while(PeekMessage(&GetMsg,NULL,uMsg,uMsg,PM_REMOVE))
		{					
		}		
	}
	void        TP_TransMessage(UINT uMsg)
	{
		MSG GetMsg;
		while(PeekMessage(&GetMsg,NULL,uMsg,uMsg,PM_REMOVE))
		{				
			TranslateMessage(&GetMsg);
			DispatchMessage(&GetMsg);
		}					
	}
	void        TP_ClearMessageEx(HWND hWnd, BOOL bPreTrans)
	{
		MSG GetMsg;
		while(PeekMessage(&GetMsg,hWnd,0,0,PM_REMOVE))
		{		
			if(GetMsg.message == WM_TIMER) continue;
			if(bPreTrans && AfxPreTranslateMessage(&GetMsg)) continue;

			::TranslateMessage(&GetMsg);
			::DispatchMessage(&GetMsg);

		}			
		UINT nMsg[4] = {WM_LBUTTONDOWN,WM_LBUTTONUP,WM_NCLBUTTONDOWN,WM_NCLBUTTONUP};			
		for(INT l=0;l<sizeof(nMsg)/sizeof(UINT);l++)
		{
			while(PeekMessage(&GetMsg,NULL,nMsg[l],nMsg[l],PM_REMOVE))
			{
			}
		}
	/*	while(PeekMessage(&GetMsg,NULL,WM_WINDOWPOSCHANGING,WM_WINDOWPOSCHANGED,PM_REMOVE))
		{		
			TranslateMessage(&GetMsg);
			DispatchMessage(&GetMsg);
		}								*/
		while(PeekMessage(&GetMsg,NULL,0,0,PM_QS_PAINT))
		{		
			TranslateMessage(&GetMsg);
			DispatchMessage(&GetMsg);
		}		
	}
	void        TP_RemoveKeyMouseMessage()
	{
		UINT nMsg[] = {WM_LBUTTONDOWN,WM_LBUTTONUP,WM_NCLBUTTONDOWN,WM_NCLBUTTONUP,WM_MOUSEMOVE,WM_MOUSEWHEEL,WM_KEYDOWN,WM_KEYUP,WM_SYSKEYDOWN,WM_SYSKEYUP};	
		MSG		stMsg;	
		for(INT l=0;l<sizeof(nMsg)/sizeof(UINT);l++)
		{		
			while(PeekMessage(&stMsg, NULL,nMsg[l],nMsg[l],PM_REMOVE))
			{			
			}
		}
	}
	void        TP_ClearSocketMessage()
	{	
		MSG		stMsg;	
		while(PeekMessage(&stMsg, NULL, WM_SOCKET_NOTIFY,WM_SOCKET_DEAD, PM_REMOVE))
		{
			::TranslateMessage(&stMsg);
			::DispatchMessage(&stMsg);
		}
		while(PeekMessage(&stMsg,NULL,0,0,PM_QS_PAINT))
		{		
			TranslateMessage(&stMsg);
			DispatchMessage(&stMsg);
		}		
	}
	void        TP_ClearMessageExEx(HWND hWnd,UINT uStart,UINT uEnd,BOOL bPreTrans)
	{
		MSG  GetMsg;
		while(PeekMessage(&GetMsg,hWnd,uStart,uEnd,PM_REMOVE))
		{			
			if( AfxPreTranslateMessage(&GetMsg))
				continue;
			TranslateMessage(&GetMsg);
			DispatchMessage(&GetMsg);
		}			
	}
    void        TP_ClearMessage(HWND hWnd,BOOL bExtMsg,BOOL bPreTrans)
	{
		MSG  GetMsg;
		while(PeekMessage(&GetMsg,hWnd,0,0,PM_REMOVE))
		{		
			if(GetMsg.message == WM_TIMER) continue;
			//if(pWnd && pWnd->PreTranslateMessage(&GetMsg))qushaojie20090902
			if(bPreTrans && AfxPreTranslateMessage(&GetMsg))
				continue;
			TranslateMessage(&GetMsg);
			DispatchMessage(&GetMsg);
		}			
		if(hWnd != NULL && bExtMsg)
		{	
			//UINT nMsg[4] = {WM_LBUTTONDOWN,WM_LBUTTONUP,WM_NCLBUTTONDOWN,WM_NCLBUTTONUP};
/*			UINT nMsg[] = {WM_LBUTTONUP,WM_NCLBUTTONUP};
			for(INT l=0;l<sizeof(nMsg)/sizeof(UINT);l++)
			{
				while(PeekMessage(&GetMsg,NULL,nMsg[l],nMsg[l],PM_REMOVE))
				{
				}
			}			*/
			while(PeekMessage(&GetMsg,NULL,WM_WINDOWPOSCHANGING,WM_WINDOWPOSCHANGED,PM_REMOVE))
			{		
				TranslateMessage(&GetMsg);
				DispatchMessage(&GetMsg);
			}								
			while(PeekMessage(&GetMsg,NULL,0,0,PM_QS_PAINT))
			{		
				TranslateMessage(&GetMsg);
				DispatchMessage(&GetMsg);
			}
		}
	}
	void        TP_ClearPaintMessage()
	{
		MSG GetMsg;
		while(PeekMessage(&GetMsg,NULL,0,0,PM_QS_PAINT))
		{		
			TranslateMessage(&GetMsg);
			DispatchMessage(&GetMsg);
		}				
	}	

	void  TP_ClearMenu(CMenu *pMenu)
	{
		if(pMenu == NULL) return;
		int  iCount  = pMenu->GetMenuItemCount();
		UINT nID     = 0; 
		BOOL bChild  = FALSE;
		BOOL bSparte = FALSE;
		int  i       = 0;
		for(i=iCount-1;i>=0;i--)
		{
			nID = pMenu ->GetMenuItemID(i);
			if(nID !=0)
			{
				UINT  uState = pMenu ->GetMenuState(i,MF_BYPOSITION);
				if((uState & MF_DISABLED) ==  MF_DISABLED)
					pMenu ->DeleteMenu(i,MF_BYPOSITION);
				else if(nID != 0xffffffff)  bChild = TRUE;
			}			
		}
		iCount = pMenu ->GetMenuItemCount();
		for(i=iCount-1;i>=0;i--)
		{
			nID = pMenu ->GetMenuItemID(i);
			if(nID ==0)
			{
				if(i == 0 ||i== (int)pMenu ->GetMenuItemCount()-1 )
					pMenu ->DeleteMenu(i,MF_BYPOSITION);
				else
				{	
					nID = pMenu ->GetMenuItemID(i-1);
					if(nID ==0) pMenu ->DeleteMenu(i,MF_BYPOSITION);
					else bSparte = TRUE;
				}
			}
		}
		if(!bSparte && !bChild && pMenu ->GetMenuItemCount()>2) ((CTPMenuEx *)pMenu) ->InsertMenu(1,MF_BYPOSITION|MF_SEPARATOR,0,_T(""));
	}
	void  TP_ClearMenuBar(CMenu *pMenu)
	{
		if(pMenu == NULL) return;
		int  iCount  = pMenu->GetMenuItemCount();
		UINT nID     = 0; 
		BOOL bChild  = FALSE;
		BOOL bSparte = FALSE;
		int  i       = 0;
		for(i=iCount-1;i>=0;i--)
		{
			nID = pMenu ->GetMenuItemID(i);
			if(nID !=0)
			{
				if(nID != 0xffffffff)  bChild = TRUE;
			}			
		}
		iCount = pMenu ->GetMenuItemCount();
		for(i=iCount-1;i>=0;i--)
		{
			nID = pMenu ->GetMenuItemID(i);
			if(nID ==0)
			{
				if(i == 0 ||i== (int)pMenu ->GetMenuItemCount()-1 )
					pMenu ->DeleteMenu(i,MF_BYPOSITION);
				else
				{	
					nID = pMenu ->GetMenuItemID(i-1);
					if(nID ==0) pMenu ->DeleteMenu(i,MF_BYPOSITION);
					else bSparte = TRUE;
				}
			}
		}
		if(!bSparte && !bChild && pMenu ->GetMenuItemCount()>2) ((CTPMenuEx *)pMenu) ->InsertMenu(1,MF_BYPOSITION|MF_SEPARATOR,0,_T(""));
	}

	LRESULT TP_GetPaintRgn(CWnd *pParentWnd, CRgn *rgnPaint, BOOL bVisible)
	{
		if(pParentWnd == NULL ||rgnPaint == NULL) return S_FALSE;
		CRect rtTemp ;
		CRgn  rgnTemp,rgnWnd;
		CWnd *pThisWnd,*pThisParentWnd;

		HWND hWnd    =::GetTopWindow(pParentWnd->m_hWnd);
		while(hWnd != NULL)
		{
			pThisWnd=CWnd::FromHandle(hWnd);
			if(pThisWnd==NULL|| bVisible && (!pThisWnd->IsWindowVisible())) 
			{
				hWnd=::GetNextWindow(hWnd,GW_HWNDNEXT);
				continue;
			}
			pThisParentWnd = pThisWnd->GetParent();
			if(pThisParentWnd == pParentWnd)
			{
				pThisWnd->GetWindowRect(&rtTemp);
				pParentWnd->ScreenToClient(&rtTemp);
				rgnWnd.CreateRectRgn(rtTemp.left,rtTemp.top,rtTemp.right,rtTemp.bottom);
				rgnTemp.CreateRectRgn(0,0,0,0);
				rgnTemp.CombineRgn(rgnPaint,&rgnWnd,RGN_DIFF);
				rgnPaint->CopyRgn(&rgnTemp);
				rgnWnd.DeleteObject();
				rgnTemp.DeleteObject();
			}
			hWnd=::GetNextWindow(hWnd,GW_HWNDNEXT);
		}	
		return S_OK;
	}
	HCURSOR     TP_GetCtrlCursor()
	{
		return g_hArrowCursor;
	}
	BOOL        TP_LoadPrepare()
	{
		TCHAR cPath[MAX_PATH];
		::GetModuleFileName(g_hThisInstance,cPath,MAX_PATH);
		PathRemoveFileSpec(cPath);
		g_sSysSkinPath  = cPath;
		g_sSysSkinPath  = g_sSysSkinPath.Left(g_sSysSkinPath.ReverseFind('\\'));
		g_sSysSkinPath += + _L("\\Sys\\TPSkin_NS");
		//g_pRoundCorner->LoadRes();
		//g_sSysSkinPath = _T("");

		//BYTE              g_bCreateLargeIcon = TRUE; 
		return TRUE;
	}
	BOOL        TP_ReleasePrepare()
	{
		return TRUE;
	}	
	CString TP_LoadStr(LPCTSTR sText)
	{	
		if(lstrlen(sText) <=0) return sText;		
		if(CTPBaseCtrl::m_stuSBDictionary.SBLoadString == NULL)
			return (LPCTSTR)sText;
		return CTPBaseCtrl::m_stuSBDictionary.SBLoadString(sText);	
	}
	void TP_LoadMenuStr(CTPMenuEx *pMenu)
	{		
		if(pMenu == NULL || CTPBaseCtrl::m_stuSBDictionary.SBLoadString == NULL) return;	

		if(pMenu->m_bLoadString) return;
		pMenu->m_bLoadString = TRUE;

		int     iCount = pMenu->GetMenuItemCount();
		UINT    nID    = 0; 	
		int     iFind  = -1;
		CString sText  = _L("");
		pMenu ->m_lMinWidth    = 10;
		pMenu ->m_lMaxExtWidth = 0;
		for(int i=iCount-1;i>=0;i--)
		{
			nID = pMenu ->GetMenuItemID(i);
			if(nID !=0)
			{
				pMenu ->GetMenuString(i,sText,MF_BYPOSITION);
				if(sText.GetLength()<=0) continue;						
				iFind  = sText.Find('\t');
				if(iFind>0) sText = sText.Left(iFind);
				sText  = CTPBaseCtrl::m_stuSBDictionary.SBLoadString(sText);				
				pMenu ->ModifyMenu(i,MF_BYPOSITION|MF_STRING/*|MF_LOADSTR*/,nID,sText);
 				if(nID == 0xffffffff)
				{			
					pMenu ->m_lMinWidth = max(pMenu ->GetMenuWidth(sText)+10,pMenu ->m_lMinWidth); 
					CTPMenuEx  *pMenuTemp = NULL;
					pMenuTemp = pMenu->GetSubMenu(i);
					if(pMenuTemp) 
					{
						TP_LoadMenuStr(pMenuTemp);
					}
				}
			}
		}
	}
};
//