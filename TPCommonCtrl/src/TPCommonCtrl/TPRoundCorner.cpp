#include "StdAfx.h"
#include ".\tproundcorner.h"


enum TLWND_ICON_INDEX
{
	TLWND_ICON_LEFT,	
	TLWND_ICON_LEFTTOP,
	TLWND_ICON_TOP,
	TLWND_ICON_RIGHTTOP,
	TLWND_ICON_RIGHT,
	TLWND_ICON_RIGHTBOTTOM,
	TLWND_ICON_BOTTOM,
	TLWND_ICON_LEFTBOTTOM,

	TLWND_ICON_MAX,
};


CTPRoundCorner::CTPRoundCorner(void)
{	
	m_iBackColor    = -1;
	m_dwWindowLevel = TPWNDLEVEL1;
	for(INT l=0;l<TP_ROUNDCORNER_MAX;l++)
	{
		m_aBmpres[l] = NULL;
	}
	for(INT l=0;l<2;l++)
	{
		for(INT k =0;k<16;k++)
		{			
			m_aVideores[l][k] = NULL;
		}
	}
}

CTPRoundCorner::~CTPRoundCorner(void)
{	
	UnloadRes();
	UnloadVideo();
}
void  CTPRoundCorner::UnloadRes()
{
	for(INT l=0;l<TP_ROUNDCORNER_MAX;l++)
	{
		if(m_aBmpres[l])
			TP_ReleaseCommonRes(m_aBmpres[l]);
		m_aBmpres[l] = NULL;		
	}
}
void  CTPRoundCorner::LoadVideo()
{
	if(m_aVideores[0][0]!=NULL) return;

	CString  sBmpName[2][16] = 
	{
		{
			_T("frame_left_top_nor.bmp"),     _T("frame_right_top_nor.bmp"),     _T("frame_right_bottom_nor.bmp"),    _T("frame_left_bottom_nor.bmp"),
			_T("frame_left_top_ver_nor.bmp"), _T("frame_right_top_ver_nor.bmp"), _T("frame_right_bottom_ver_nor.bmp"),_T("frame_left_bottom_ver_nor.bmp"),
			_T("frame_left_top_hor_nor.bmp"), _T("frame_right_top_hor_nor.bmp"), _T("frame_right_bottom_hor_nor.bmp"),_T("frame_left_bottom_hor_nor.bmp"),
			_T("frame_left_nor.bmp"),         _T("frame_right_nor.bmp"),         _T("frame_top_nor.bmp"),             _T("frame_bottom_nor.bmp"),
		},
		{
			_T("frame_left_top_down.bmp"),    _T("frame_right_top_down.bmp"),    _T("frame_right_bottom_down.bmp"),    _T("frame_left_bottom_down.bmp"),
			_T("frame_left_top_ver_down.bmp"),_T("frame_right_top_ver_down.bmp"),_T("frame_right_bottom_ver_down.bmp"),_T("frame_left_bottom_down.bmp"),
			_T("frame_left_top_hor_down.bmp"),_T("frame_right_top_hor_down.bmp"),_T("frame_right_bottom_hor_down.bmp"),_T("frame_left_bottom_hor_down.bmp"),
			_T("frame_left_down.bmp"),        _T("frame_right_down.bmp"),        _T("frame_top_down.bmp"),             _T("frame_bottom_down.bmp")
		}
		,
	};
	CString  sPath ;
	for(INT l=0;l<2;l++)
	{
		for(INT m=0;m<16;m++)
		{
			sPath.Format(_L("%s\\material_preview\\%s"),TP_GetSkinPath(),sBmpName[l][m]);
			m_aVideores[l][m] = TP_LoadBmpByte(sPath);		
		}
	}
}
void  CTPRoundCorner::UnloadVideo()
{
	for(INT l=0;l<2;l++)
	{
		for(INT k =0;k<16;k++)
		{
			if(m_aVideores[l][k])
				TP_ReleaseCommonRes(m_aVideores[l][k]);
			m_aVideores[l][k] = NULL;
		}
	}
}
void  CTPRoundCorner::LoadRes()
{

	//CString sIconName[8] =
	//{
	//	_L("Left"),_L("TopLeft"),_L("Top"),_L("TopRight"),_L("Right"),_L("RightBottom"),_L("Bottom"),_L("LeftBottom"),
	//};
	CString sPath[TP_ROUNDCORNER_MAX] =
	{
		_T("91"),_T("53"),	
	};
	for(INT l=0;l<TP_ROUNDCORNER_MAX;l++)
	{		
		m_aBmpres[l] = TP_LoadImage(g_sSysSkinPath + _L("\\CommonCtrl\\RoundCorner\\") + sPath[l] + _L("\\") + _T("RoundCorner.bmp"),32,RGB(0,128,128),FALSE,TRUE);
	}
}
void  CTPRoundCorner::PaintRect(CDC *pDC,CRect rtRoundFrame)
{	
	rtRoundFrame.right  = min(rtRoundFrame.right,rtRoundFrame.left + 3000);
	rtRoundFrame.bottom = min(rtRoundFrame.bottom,rtRoundFrame.top + 3000);

	HIMAGELIST hImageList;
	if(m_dwWindowLevel == TPWNDLEVEL1)   	hImageList =  m_aBmpres[0] ->GetImage();
	else if(m_dwWindowLevel == TPWNDLEVEL2)	hImageList =  m_aBmpres[1] ->GetImage();   
	else	                                hImageList =  m_aBmpres[0] ->GetImage();

	

	for(INT l = 	rtRoundFrame.left;l<= rtRoundFrame.right -32 ;l+=32	) 
		ImageList_Draw(hImageList,TLWND_ICON_TOP,pDC->GetSafeHdc(),l,rtRoundFrame.top,ILD_TRANSPARENT);
		//pDC ->DrawIcon(CPoint(l,rtRoundFrame.top),hIconCorner[TLWND_ICON_TOP]);
	for(INT l = 	rtRoundFrame.top;l<= rtRoundFrame.bottom -32 ;l+=32	) 
		ImageList_Draw(hImageList,TLWND_ICON_RIGHT,pDC->GetSafeHdc(),rtRoundFrame.right - 32 ,l,ILD_TRANSPARENT);
		//pDC ->DrawIcon(CPoint(rtRoundFrame.right - 32 ,l),hIconCorner[TLWND_ICON_RIGHT]);
	for(INT l = 	rtRoundFrame.left;l<= rtRoundFrame.right -32 ;l+=32	) 
		ImageList_Draw(hImageList,TLWND_ICON_BOTTOM,pDC->GetSafeHdc(),l,rtRoundFrame.bottom - 32,ILD_TRANSPARENT);
		//pDC ->DrawIcon(CPoint(l,rtRoundFrame.bottom - 32),hIconCorner[TLWND_ICON_BOTTOM]);
	for(INT l = 	rtRoundFrame.top;l<= rtRoundFrame.bottom -32 ;l+=32	) 
		ImageList_Draw(hImageList,TLWND_ICON_LEFT,pDC->GetSafeHdc(),rtRoundFrame.left,l,ILD_TRANSPARENT);
		//pDC ->DrawIcon(CPoint(rtRoundFrame.left,l),hIconCorner[TLWND_ICON_LEFT]);

	ImageList_Draw(hImageList,TLWND_ICON_LEFTTOP,pDC->GetSafeHdc(),rtRoundFrame.left       ,rtRoundFrame.top,ILD_TRANSPARENT);
	//pDC ->DrawIcon(CPoint(rtRoundFrame.left       ,rtRoundFrame.top),       hIconCorner[TLWND_ICON_LEFTTOP]);

	ImageList_Draw(hImageList,TLWND_ICON_RIGHTTOP,pDC->GetSafeHdc(),rtRoundFrame.right - 32 ,rtRoundFrame.top,ILD_TRANSPARENT);
	//pDC ->DrawIcon(CPoint(rtRoundFrame.right - 32 ,rtRoundFrame.top),       hIconCorner[TLWND_ICON_RIGHTTOP]);

	ImageList_Draw(hImageList,TLWND_ICON_RIGHTBOTTOM,pDC->GetSafeHdc(),rtRoundFrame.right - 32 ,rtRoundFrame.bottom -32,ILD_TRANSPARENT);
	//pDC ->DrawIcon(CPoint(rtRoundFrame.right - 32 ,rtRoundFrame.bottom -32),hIconCorner[TLWND_ICON_RIGHTBOTTOM]);

	ImageList_Draw(hImageList,TLWND_ICON_LEFTBOTTOM,pDC->GetSafeHdc(),rtRoundFrame.left       ,rtRoundFrame.bottom -32,ILD_TRANSPARENT);
	//pDC ->DrawIcon(CPoint(rtRoundFrame.left       ,rtRoundFrame.bottom -32),hIconCorner[TLWND_ICON_LEFTBOTTOM]);


}
void  CTPRoundCorner::PaintLine(CDC *pDC,CRect rtRoundFrame,UINT nLine)
{
	rtRoundFrame.right  = min(rtRoundFrame.right,rtRoundFrame.left + 3000);
	rtRoundFrame.bottom = min(rtRoundFrame.bottom,rtRoundFrame.top + 3000);

	if(rtRoundFrame.right - rtRoundFrame.left>1024*4 || rtRoundFrame.bottom - rtRoundFrame.top>768*4) return; 
	CRgn  rgn;rgn.CreateRectRgn(rtRoundFrame.left,rtRoundFrame.top,rtRoundFrame.right,rtRoundFrame.bottom);
	pDC ->SelectClipRgn(&rgn);

	HIMAGELIST hImageList;
	if(m_dwWindowLevel == TPWNDLEVEL1)   	hImageList =  m_aBmpres[0] ->GetImage();
	else if(m_dwWindowLevel == TPWNDLEVEL2)	hImageList =  m_aBmpres[1] ->GetImage();   
	else	                                hImageList =  m_aBmpres[0] ->GetImage();

	if(nLine & 0x00000001)
	{
		for(INT l = 	rtRoundFrame.left;l<= rtRoundFrame.right  ;l+=32	) 
			ImageList_Draw(hImageList,TLWND_ICON_TOP,pDC->GetSafeHdc(),l,rtRoundFrame.top,ILD_TRANSPARENT);
		//pDC ->DrawIcon(CPoint(l,rtRoundFrame.top),hIconCorner[TLWND_ICON_TOP]);
	}
	if(nLine & 0x00000002)
	{
		for(INT l = 	rtRoundFrame.top;l<= rtRoundFrame.bottom  ;l+=32	) 
			ImageList_Draw(hImageList,TLWND_ICON_RIGHT,pDC->GetSafeHdc(),rtRoundFrame.right - 32 ,l,ILD_TRANSPARENT);
		//pDC ->DrawIcon(CPoint(rtRoundFrame.right - 32 ,l),hIconCorner[TLWND_ICON_RIGHT]);
	}
	if(nLine & 0x00000004)
	{
		for(INT l = 	rtRoundFrame.left;l<= rtRoundFrame.right  ;l+=32	) 
			ImageList_Draw(hImageList,TLWND_ICON_BOTTOM,pDC->GetSafeHdc(),l,rtRoundFrame.bottom - 32,ILD_TRANSPARENT);
		//pDC ->DrawIcon(CPoint(l,rtRoundFrame.bottom - 32),hIconCorner[TLWND_ICON_BOTTOM]);
	}
	if(nLine & 0x00000008)
	{
		for(INT l = 	rtRoundFrame.top;l<= rtRoundFrame.bottom  ;l+=32	) 
			ImageList_Draw(hImageList,TLWND_ICON_LEFT,pDC->GetSafeHdc(),rtRoundFrame.left,l,ILD_TRANSPARENT);
		//pDC ->DrawIcon(CPoint(rtRoundFrame.left,l),hIconCorner[TLWND_ICON_LEFT]);
	}
	SelectClipRgn(pDC ->m_hDC,NULL);
	rgn.DeleteObject();
}
void  CTPRoundCorner::SetState(UINT uState)
{
	m_dwWindowLevel = uState;
}
void    CTPRoundCorner::PaintRect(CImageList *pImageList,CDC *pDC, CRect rtCorner)
{	
	rtCorner.right  = min(rtCorner.right,rtCorner.left + 3000);
	rtCorner.bottom = min(rtCorner.bottom,rtCorner.top + 3000);


	if(pImageList == NULL) return;
	IMAGEINFO imgInfo;	
	pImageList ->GetImageInfo(0,&imgInfo);
	int iWidth = imgInfo.rcImage.right  - imgInfo.rcImage.left;
	int iHight = imgInfo.rcImage.bottom - imgInfo.rcImage.top;

	for(int l=rtCorner.left + iWidth;l<rtCorner.right -iWidth;l+=iWidth)
	{
		pImageList ->Draw(pDC,1,CPoint(min(l,rtCorner.right -iWidth*2),rtCorner.top),ILD_TRANSPARENT);	
		pImageList ->Draw(pDC,5,CPoint(min(l,rtCorner.right -iWidth*2),rtCorner.bottom-iHight),ILD_TRANSPARENT);
	}
	for(int l=rtCorner.top + iHight;l<rtCorner.bottom -iHight;l+=iHight)
	{
		pImageList ->Draw(pDC,7,CPoint(rtCorner.left ,l),ILD_TRANSPARENT);
		pImageList ->Draw(pDC,3,CPoint(rtCorner.right -iWidth ,l),ILD_TRANSPARENT);
	}
	pImageList ->Draw(pDC,0,CPoint(rtCorner.left ,rtCorner.top),ILD_TRANSPARENT);
	pImageList ->Draw(pDC,2,CPoint(rtCorner.right - iWidth ,rtCorner.top),ILD_TRANSPARENT);
	pImageList ->Draw(pDC,4,CPoint(rtCorner.right - iWidth ,rtCorner.bottom - iHight),ILD_TRANSPARENT);
	pImageList ->Draw(pDC,6,CPoint(rtCorner.left           ,rtCorner.bottom - iHight),ILD_TRANSPARENT);	
}
void  CTPRoundCorner::PaintRect(TP_BORDER_STYLE eBorderStyle,CWnd *pWnd,TPScrollBarData *pScrollData)
{	

	CDC  *pDC         = pWnd ->GetWindowDC();	
	CTPDCExchange stuTemp(pDC);
	CRect rtWnd,rtClient,rtCorner;
	pWnd ->GetWindowRect(&rtWnd);
	pWnd ->GetClientRect(&rtClient);
	pWnd ->ClientToScreen(&rtClient);
	rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
	rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
	rtCorner = rtWnd;
	rtCorner.left   = rtClient.right;
	rtCorner.top    = rtClient.bottom;
	rtCorner.right  = rtWnd.right  + (rtWnd.left - rtClient.left);
	rtCorner.bottom = rtWnd.bottom + (rtWnd.top  - rtClient.top);

	rtCorner.right  = min(rtCorner.right,rtCorner.left + 3000);
	rtCorner.bottom = min(rtCorner.bottom,rtCorner.top + 3000);


	if(rtCorner.Width() >0)
	{
		if(m_dwWindowLevel == TPWNDLEVEL1)   		pDC ->FillSolidRect(rtCorner,TP_GetSysColor(COLOR_BASE_WINDOW));
		else if(m_dwWindowLevel == TPWNDLEVEL2) 	pDC ->FillSolidRect(rtCorner,TP_GetSysColor(COLOR_BASE_WINDOW2));		

	}
	if(rtWnd.right - rtClient.right>5 && pScrollData && !pScrollData ->bShowVScrollBar)
		pDC ->FillSolidRect(CRect(rtClient.right,rtWnd.top,rtWnd.right,rtWnd.bottom),TP_GetSysColor(COLOR_BASE_WINDOW));
	if(rtWnd.bottom - rtClient.bottom>5 && pScrollData && !pScrollData ->bShowHScrollBar)
		pDC ->FillSolidRect(CRect(rtWnd.right,rtClient.bottom,rtWnd.right,rtWnd.bottom),TP_GetSysColor(COLOR_BASE_WINDOW));
	if(rtClient.left- rtWnd.left >0)
	{	
		PaintRect(eBorderStyle,pDC,rtWnd,rtClient.left- rtWnd.left );		
	}	
	stuTemp.ReleaseDC();
	pWnd ->ReleaseDC(pDC);
}
void  CTPRoundCorner::PaintRect(TP_BORDER_STYLE eBorderStyle,CDC *pDC ,CRect rtWnd,int iWidth,UINT uFlag)
{
	rtWnd.right  = min(rtWnd.right,rtWnd.left + 3000);
	rtWnd.bottom = min(rtWnd.bottom,rtWnd.top + 3000);


	int iRightColor = COLOR_BASE_3DRIGHT,iLeftColor = COLOR_BASE_3DLEFT;
	if(m_dwWindowLevel == TPWNDLEVEL2)
	{
		iRightColor = COLOR_BASE_3DRIGHT2;iLeftColor = COLOR_BASE_3DLEFT;
	}

	if(eBorderStyle == TP_BORDER_NORMAL)
	{
		CPen pPen1(PS_SOLID|PS_ENDCAP_FLAT|PS_INSIDEFRAME,iWidth,TP_GetSysColor(iRightColor)) , pPen2(PS_INSIDEFRAME|PS_SOLID|PS_ENDCAP_FLAT,iWidth,TP_GetSysColor(iLeftColor)),*pSavePen;
		if(uFlag & CORNER_LEFT)
		{
			pSavePen = pDC ->SelectObject(&pPen1);
			pDC ->MoveTo(rtWnd.left,rtWnd.top+iWidth/2);
			pDC ->LineTo(rtWnd.right,rtWnd.top+iWidth/2);
			pDC ->SelectObject(&pPen2);
			pDC ->MoveTo(rtWnd.right-iWidth/2,rtWnd.top+iWidth/2);    
			pDC ->LineTo(rtWnd.right-iWidth/2,rtWnd.bottom);
			pDC ->MoveTo(rtWnd.right-iWidth/2,rtWnd.bottom-iWidth/2);
			pDC ->LineTo(rtWnd.left+iWidth/2-1,rtWnd.bottom-iWidth/2);
			pDC ->SelectObject(pSavePen);

		}
		else if(uFlag & CORNER_RIGHT)
		{
			pSavePen = pDC ->SelectObject(&pPen1);
			pDC ->MoveTo(rtWnd.left+iWidth/2,rtWnd.bottom-iWidth/2);
			pDC ->LineTo(rtWnd.left+iWidth/2,rtWnd.top);
			pDC ->MoveTo(rtWnd.left+iWidth/2,rtWnd.top+iWidth/2);
			pDC ->LineTo(rtWnd.right,rtWnd.top+iWidth/2);
			pDC ->SelectObject(&pPen2);			
			pDC ->MoveTo(rtWnd.right,rtWnd.bottom-iWidth/2);
			pDC ->LineTo(rtWnd.left+iWidth/2,rtWnd.bottom-iWidth/2);
			pDC ->SelectObject(pSavePen);

		}
	}
	else if(eBorderStyle == TP_BORDER_FLAT)
	{
		pDC ->Draw3dRect(rtWnd,TP_GetSysColor(iLeftColor),TP_GetSysColor(iLeftColor));
	}
	else if(eBorderStyle == TP_BORDER_ROUNDCONRER)
	{
	}
}
void  CTPRoundCorner::PaintRect(TP_BORDER_STYLE eBorderStyle,CDC *pDC ,CRect rtWnd,int iWidth)
{	
	rtWnd.right  = min(rtWnd.right,rtWnd.left + 3000);
	rtWnd.bottom = min(rtWnd.bottom,rtWnd.top + 3000);
	int iRightColor = COLOR_BASE_3DRIGHT,iLeftColor = COLOR_BASE_3DLEFT;
	if(m_dwWindowLevel == TPWNDLEVEL2)
	{
		iRightColor = COLOR_BASE_3DRIGHT2;iLeftColor = COLOR_BASE_3DLEFT2;
	}
	if(eBorderStyle == TP_BORDER_GROUPBOX)
	{
		pDC ->Draw3dRect(rtWnd,TP_GetSysColor(iRightColor),TP_GetSysColor(iLeftColor));
		rtWnd.DeflateRect(1,1);
		pDC ->Draw3dRect(rtWnd,TP_GetSysColor(iLeftColor),TP_GetSysColor(iRightColor));

	}
	else if(eBorderStyle == TP_BORDER_NORMAL)
	{		
		if(iWidth>1)
		{
			CPen pPen1(PS_SOLID|PS_ENDCAP_FLAT|PS_INSIDEFRAME,iWidth,TP_GetSysColor(iRightColor)) , pPen2(PS_INSIDEFRAME|PS_SOLID|PS_ENDCAP_FLAT,iWidth,TP_GetSysColor(iLeftColor)),*pSavePen;

			pSavePen = pDC ->SelectObject(&pPen1);
			pDC ->MoveTo(rtWnd.left+iWidth/2,rtWnd.bottom);
			pDC ->LineTo(rtWnd.left+iWidth/2,rtWnd.top);
			pDC ->MoveTo(rtWnd.left,rtWnd.top+iWidth/2);
			pDC ->LineTo(rtWnd.right,rtWnd.top+iWidth/2);
			pDC ->SelectObject(&pPen2);
			pDC ->MoveTo(rtWnd.right-iWidth/2,rtWnd.top+iWidth/2);    
			pDC ->LineTo(rtWnd.right-iWidth/2,rtWnd.bottom-iWidth/2);
			pDC ->MoveTo(rtWnd.right -iWidth/2,rtWnd.bottom-iWidth/2);
			pDC ->LineTo(rtWnd.left+iWidth/2,rtWnd.bottom-iWidth/2);
			pDC ->SelectObject(pSavePen);
		}
		else
		{
			pDC ->Draw3dRect(rtWnd,TP_GetSysColor(iRightColor),TP_GetSysColor(iLeftColor));
		}

	}
	else if(eBorderStyle == TP_BORDER_FLAT)
	{
		pDC ->Draw3dRect(rtWnd,TP_GetSysColor(iLeftColor),TP_GetSysColor(iLeftColor));
	}
	else if(eBorderStyle == TP_BORDER_ROUNDCONRER)
	{
		PaintRect(pDC,rtWnd);
	}
	else if(TP_BORDER_20040702 == eBorderStyle ||TP_BORDER_20090623  == eBorderStyle )
	{
		if(iWidth >1)
		{
			COLORREF clBack = TP_GetSysColor(COLOR_BASE_FRAME);
			if(TP_BORDER_20090623  == eBorderStyle) clBack = RGB(78,78,78);       
			else if(m_dwWindowLevel == TPWNDLEVEL3) clBack = RGB(0,0,0);       		
			else if(m_dwWindowLevel == TPWNDLEVEL2) clBack = TP_GetSysColor(COLOR_BASE_WINDOW2);
			pDC ->FillSolidRect(rtWnd.left,rtWnd.top,rtWnd.Width(),iWidth,clBack);
			pDC ->FillSolidRect(rtWnd.left,rtWnd.top,iWidth,rtWnd.Height(),clBack);
			pDC ->FillSolidRect(rtWnd.right - iWidth,rtWnd.top,iWidth,rtWnd.Height(),clBack);
			pDC ->FillSolidRect(rtWnd.left,rtWnd.bottom - iWidth,rtWnd.Width(),iWidth,clBack);
		}
		rtWnd.DeflateRect(iWidth-1,iWidth-1);
		pDC ->Draw3dRect(rtWnd,TP_GetSysColor(iRightColor),TP_GetSysColor(iLeftColor));
	}
}
void  CTPRoundCorner::PaintVideo(int iIndex,CRect &rtRect,CDC *pDC)
{
	return;

	TPCommonRes        *paVideores[16];
	for(INT l=0;l<16;l++) paVideores[l] = m_aVideores[iIndex][l];

	pDC	->FillSolidRect(rtRect,RGB(0,0,0));
	enum 
	{
		frame_left_top,     frame_right_top,    frame_right_bottom,    frame_left_bottom,
	    frame_left_top_ver, frame_right_top_ver,frame_right_bottom_ver,frame_left_bottom_ver,
	    frame_left_top_hor, frame_right_top_hor,frame_right_bottom_hor,frame_left_bottom_hor,
	    frame_left,         frame_right,        frame_top,             frame_bottom,
	};
	CRect rtTemp,rtTemp1;
	rtTemp	= rtRect;rtTemp.bottom = rtTemp.top; rtTemp.top = rtTemp.bottom - paVideores[frame_top]->Height();	rtTemp.InflateRect(2,0);
	TP_TileDIBLine(pDC,paVideores[frame_top]->GetByte(),paVideores[frame_top]->Width(),paVideores[frame_top]->Height(),rtTemp);

	rtTemp = rtRect  ; rtTemp.top = rtTemp.bottom; rtTemp.bottom = rtTemp.top + paVideores[frame_bottom]->Height();	rtTemp.InflateRect(2,0);
	TP_TileDIBLine(pDC,paVideores[frame_bottom]->GetByte(),paVideores[frame_bottom]->Width(),paVideores[frame_bottom]->Height(),rtTemp);

	rtTemp = rtRect  ; rtTemp.right = rtTemp.left; rtTemp.left = rtTemp.right - paVideores[frame_left]->Width();	rtTemp.InflateRect(0,2);
	TP_TileDIBLine(pDC,paVideores[frame_left]->GetByte(),paVideores[frame_left]->Width(),paVideores[frame_left]->Height(),rtTemp,TRUE);

	rtTemp = rtRect  ; rtTemp.left = rtTemp.right; rtTemp.right = rtTemp.left + paVideores[frame_right]->Width();	rtTemp.InflateRect(0,2);
	TP_TileDIBLine(pDC,paVideores[frame_right]->GetByte(),paVideores[frame_right]->Width(),paVideores[frame_right]->Height(),rtTemp,TRUE);

	rtTemp  = rtRect;
	rtTemp.right  = rtTemp.left;	rtTemp.left   = rtTemp.right  - paVideores[frame_left]   ->Width();
	rtTemp.bottom = rtTemp.top;     rtTemp.top    = rtTemp.bottom - paVideores[frame_top]   ->Height();		
	TP_SetDIBitsToDevice(pDC,paVideores[frame_left_top]->GetByte(),paVideores[frame_left_top]->Width(),paVideores[frame_left_top]->Height(),rtTemp);
	rtTemp1 = rtTemp; rtTemp1.left = rtTemp1.right; rtTemp1.right = rtTemp1.left + paVideores[frame_left_top_hor]   ->Width();
	TP_SetDIBitsToDevice(pDC,paVideores[frame_left_top_hor]->GetByte(),paVideores[frame_left_top_hor]->Width(),paVideores[frame_left_top_hor]->Height(),rtTemp1);
	rtTemp1 = rtTemp; rtTemp1.top = rtTemp1.bottom; rtTemp1.bottom = rtTemp1.top + paVideores[frame_left_top_ver]   ->Height();
	TP_SetDIBitsToDevice(pDC,paVideores[frame_left_top_ver]->GetByte(),paVideores[frame_left_top_ver]->Width(),paVideores[frame_left_top_ver]->Height(),rtTemp1);


	rtTemp = rtRect;
	rtTemp.left    = rtTemp.right;   rtTemp.right  =  rtTemp.left+paVideores[frame_right]   ->Width();
	rtTemp.bottom  = rtTemp.top;  rtTemp.top    = rtTemp.bottom-paVideores[frame_top]    ->Height();
	rtTemp.left    = rtTemp.right - paVideores[frame_right_top]   ->Width();
	rtTemp.bottom  = rtTemp.top   + paVideores[frame_right_bottom]   ->Height();

	TP_SetDIBitsToDevice(pDC,paVideores[frame_right_top]->GetByte(),paVideores[frame_right_top]->Width(),paVideores[frame_right_top]->Height(),rtTemp);
	rtTemp1 = rtTemp ;
	rtTemp1.right = rtTemp.left; rtTemp1.left = rtTemp.right-paVideores[frame_right_top_hor]    ->Width();
	TP_SetDIBitsToDevice(pDC,paVideores[frame_right_top_hor]    ->GetByte(),paVideores[frame_right_top_hor]    ->Width(),paVideores[frame_right_top_hor]    ->Height(),rtTemp1);
	rtTemp1 = rtTemp ;
	rtTemp1.top = rtTemp.bottom; rtTemp1.bottom = rtTemp.top + paVideores[frame_right_top_ver]    ->Height();
	TP_SetDIBitsToDevice(pDC,paVideores[frame_right_top_ver]    ->GetByte(),paVideores[frame_right_top_ver]    ->Width(),paVideores[frame_right_top_ver]    ->Height(),rtTemp1);

	//draw right-bottom corner
	rtTemp = rtRect;
	rtTemp.left = rtTemp.right;  rtTemp.right = rtTemp.left + paVideores[frame_right]   ->Width();
	rtTemp.top = rtTemp.bottom; rtTemp.bottom = rtTemp.top + paVideores[frame_bottom]   ->Height();
	rtTemp.left    = rtTemp.right    - paVideores[frame_right_top]   ->Width();
	rtTemp.top     = rtTemp.bottom   - paVideores[frame_right_bottom]   ->Height();

	TP_SetDIBitsToDevice(pDC,paVideores[frame_right_bottom]->GetByte(),paVideores[frame_right_bottom]->Width(),paVideores[frame_right_bottom]->Height(),rtTemp);
	rtTemp1 = rtTemp ;
	rtTemp1.bottom = rtTemp.top; rtTemp1.top = rtTemp.top-paVideores[frame_right_bottom_ver]   ->Height();
	TP_SetDIBitsToDevice(pDC,paVideores[frame_right_bottom_ver]    ->GetByte(),paVideores[frame_right_bottom_ver]    ->Width(),paVideores[frame_right_bottom_ver]    ->Height(),rtTemp1);

	rtTemp1 = rtTemp ;
	rtTemp1.right = rtTemp.left; rtTemp.left = rtTemp.left-paVideores[frame_right_bottom_hor]   ->Width();
	TP_SetDIBitsToDevice(pDC,paVideores[frame_right_bottom_hor]    ->GetByte(),paVideores[frame_right_bottom_hor]    ->Width(),paVideores[frame_right_bottom_hor]    ->Height(),rtTemp1);


	rtTemp = rtRect;
	rtTemp.right = rtTemp.left; rtTemp.left  = rtTemp.right - paVideores[frame_left]    ->Width();
	rtTemp.top   = rtTemp.bottom; rtTemp.bottom = rtTemp.top + paVideores[frame_bottom] ->Height();
	rtTemp.top     = rtTemp.bottom   - paVideores[frame_left_bottom]   ->Height();


	TP_SetDIBitsToDevice(pDC,paVideores[frame_left_bottom]->GetByte(),paVideores[frame_left_bottom]->Width(),paVideores[frame_left_bottom]->Height(),rtTemp);
	rtTemp1 = rtTemp ;
	rtTemp1.left = rtTemp.right; rtTemp1.left = rtTemp.right- paVideores[frame_left_bottom_hor]    ->Width();
	TP_SetDIBitsToDevice(pDC,paVideores[frame_left_bottom_hor]    ->GetByte(),paVideores[frame_left_bottom_hor]    ->Width(),paVideores[frame_left_bottom_hor]    ->Height(),rtTemp1);	  
	rtTemp1 = rtTemp ;
	rtTemp1.top = rtTemp.bottom; rtTemp1.bottom = rtTemp.top + paVideores[frame_left_bottom_ver]    ->Height();
	TP_SetDIBitsToDevice(pDC,paVideores[frame_left_bottom_ver]    ->GetByte(),paVideores[frame_left_bottom_ver]    ->Width(),paVideores[frame_left_bottom_ver]    ->Height(),rtTemp1);	  
}