// TPArticleListCtrl.cpp : implementation file
//

#include "stdafx.h"

// CTPArticleListCtrl

IMPLEMENT_DYNAMIC(CTPArticleListCtrl, CTPListCtrlEx)

CTPArticleListCtrl::CTPArticleListCtrl()
{
	m_pArticleMainDlg = NULL;
}

CTPArticleListCtrl::~CTPArticleListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTPArticleListCtrl, CTPListCtrlEx)
	//listctrlex
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)

END_MESSAGE_MAP()


// CTPArticleListCtrl message handlers


void CTPArticleListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 1;
	//if(!m_bCanEdit)  return;
	if(m_uBaseState & TP_BASESTATE_DELETEITEM) return;
	if((pNMLV ->uNewState & LVIS_SELECTED )  && pNMLV ->lParam)
	{ 
		TPListItemData *pItemData = (TPListItemData *)pNMLV ->lParam;
		//if(pItemData/* && (pItemData ->dwState & TP_RESSTATE_HIDE)*/) return;
	//	GetItemResData(pItemData);

		//m_pArticleList->GetItemData(iCurIndex);
		TPArticleData stuArticle;
		if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(pItemData->guidRes,TP_GRADE_ALL,stuArticle))
		{
			TPChannelData stuChannel;
			g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(stuArticle.guidNode,stuChannel);
			CString sFileName = g_stuArticleInterface.stuArticleInterfce.TP_GetCurArticleHtmlPath();
			CTPArticleParser::SaveHtml(sFileName, stuArticle.cText, stuArticle.stuChannelItem.cItemTitle,stuChannel.stuChannelBase.cChannelTitle,stuArticle.stuChannelItem.cItemPubDate);
			if(m_pArticleMainDlg)
				m_pArticleMainDlg->AdjustHtml(sFileName);
		}
	}	
}

BOOL CTPArticleListCtrl::SetResData(CGUIDArray &aArticleList, TPChannelData &stuChannel)
{
	aArticleList.RemoveAll();
	DeleteAllItems();
	int iIndex = 0; 

	for (int l = stuChannel.aChannelItemAll.GetSize() - 1; l >= 0 ;  l--)
	{
		TPArticleData stuArticle;
		GUID guidItem = stuChannel.aChannelItemAll[l]->guidItem;
		if((guidItem != GUID_NULL) && S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(guidItem,TP_GRADE_ALL,stuArticle))
		{
		}
		else
		{
			TCHAR *cItemText = NULL, *cImgPath = NULL;
			TPChannelItem *pItemInfo = NULL;

			CTPArticleParser stuArticleParser;
			stuArticleParser.SetChannelItem(stuChannel.aChannelItemAll[l], stuChannel.cKeyDiv);
			stuArticleParser.GetItemInfo(cItemText, cImgPath);
			ASSERT(TP_StrLen(cItemText) > 50);

			CoCreateGuid(&stuArticle.guidRes);
			stuArticle.guidNode = stuChannel.guidRes;
			stuArticle.stuChannelItem = *stuChannel.aChannelItemAll[l];
			TP_StrCpy(stuArticle.cText, cItemText, TP_StrLen(cItemText));
			//////////////////////////////////////////////////////////////////////////icon
			if(cImgPath)
			{
				int iIconWidth = TP_RESICON_WIDTH_HD,  iIconHeight = TP_RESICON_HIGH_HD;
				LPBYTE pIcon = new BYTE[iIconWidth * iIconHeight * sizeof(DWORD)];
				ZeroMemory(pIcon,iIconWidth * iIconHeight * sizeof(DWORD));	
				int iWidth = 0 ; int iHeight = 0;
				CTPImgDecode stuDec;
				stuDec.Open(cImgPath,iWidth,iHeight);
				LPBYTE pByte = new BYTE[iWidth * iWidth * sizeof(DWORD)];
				ZeroMemory(pByte, iWidth * iHeight * sizeof(DWORD));

				if(FIO_Succeed == stuDec.GetImage(pByte))
				{
					TP_StretchBlt_Mean((LPDWORD)pIcon, 0, 0, iIconWidth, iIconHeight, iIconWidth, iIconHeight, (LPDWORD)pByte, 0, 0, iWidth, iHeight, iWidth, iHeight, TP_BUFFER_COPY, TP_PARITY_BOTH);

					TPPictureItem *pPicture = new TPPictureItem;
					pPicture->pIconBuf  = pIcon;
					pPicture->szIcon.cx = iIconWidth;
					pPicture->szIcon.cy = iIconHeight;
					TP_StrCpy(pPicture->cPicPath,cImgPath,TP_StrLen(cImgPath));
					stuArticle.aPictureItem.Add(pPicture);
				}
				else
				{
					if(pIcon) {delete pIcon; pIcon = NULL;}
					if(pByte) {delete pByte; pByte = NULL;}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			
			g_stuArticleInterface.stuArticleInterfce.TP_SetArticleInfo(stuArticle.guidRes,TP_GRADE_ALL,stuArticle);
			stuChannel.aChannelItemAll[l]->guidItem = stuArticle.guidRes;
		}
		aArticleList.Add(stuArticle.guidRes);
		
		CTPListCtrlEx::InsertItem(iIndex,_L("         "));
		//InsertItem(iIndex,stuArticle.stuChannelItem.cItemTitle);//CTPListCtrlEx::InsertItem(iResNeed,_L("         "));
		TPListItemData *pItemData = new TPListItemData();
	//	pItemData ->SetResBaseData(hChildRes[l]);
		pItemData->guidRes = stuArticle.guidRes;
		pItemData->guidNode = stuArticle.guidNode;
		pItemData ->pItem = m_aItem[iIndex];
		//GetItemResData(pItemData);		
		CTPListCtrlEx::SetItemData(iIndex,(DWORD_PTR)pItemData);
		iIndex++;

	}
	return TRUE;
}
void CTPArticleListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	TPListItemData  *pItemData = (TPListItemData *)pNMLV ->lParam;
//	if(m_iItemSave == pNMLV->iItem) EndEdit();
	if(pItemData)
	{

	//	TP_ReleaseRes(pItemData->hResData, pItemData->eResType);
		delete pItemData;
		pItemData = NULL;
	}
//	if(m_iMenuItem == pNMLV->iItem) m_iMenuItem = -1;
	*pResult = 0;
}
void CTPArticleListCtrl::DrawTextPicture(CDC *pDC,int iItem)
{
	int iColumnCount = this ->GetColumnCount();
	if(iColumnCount <=0) return;

	TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iItem);
	if(pItemData == NULL) return ;
	pItemData ->rtIcon = CRect(0,0,0,0);

	GetItemResData(pItemData);

	if(!pItemData ->pItem->bShow) return;

	//	if(m_bViewFile) GetItemResClipFile(pItemData); 
	int iColumnIndex[M2];
	GetColumnOrderArray(iColumnIndex,iColumnCount);

	INT nHeadIndex = 1;//TP_NodeTypeHeadIndex(m_eShowNodeType);
	UINT uState = CTPListCtrlEx::GetItemState(iItem,LVIS_SELECTED |  LVIS_FOCUSED);	
	if(uState & LVIS_SELECTED )    pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));	
	else                           pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));

	int iLeft = 10000,iButton = 0;
	CRect rDTRect(0,0,0,0);//右下黑框
	int iOffset = 20;
	CRect rtComment = CRect(0, 0, 0, 0), rtCommentText = CRect(0, 0, 0, 0);

	for(int i = 0; i < iColumnCount; i++)
	{
		if(!m_aColumn[i]->bShow) continue;
		CRect rcSub(0,0,0,0);
		GetSubItemRect(iItem,i,LVIR_BOUNDS,rcSub);
		if(rcSub.right < m_rtClient.left || rcSub.left > m_rtClient.right) continue;
		pDC ->FillSolidRect(rcSub,TP_GetSysColor(m_iBackColor));

		if(iColumnIndex[i]==0) // 绘制图标
		{
			CRect rtSub_Name,rtIcon,rtSmallIcon;
			rtSub_Name = rcSub;

			rtSub_Name.DeflateRect(1,1);	
			rtSub_Name.right = rtSub_Name.left + rtSub_Name.Height() * TP_RESICON_WIDTH_HD /TP_RESICON_HIGH_HD; 	
			rtSub_Name.OffsetRect(2,0);
			rtSub_Name.DeflateRect(1,1);

			BOOL bOverTurn = TRUE;//(pItemData ->eResType &(TP_RES_CGFILTER | TP_RES_ALLVAEFFECT | TP_RES_CGEFFECT)) !=0;
			//if(pItemData->dwState & TP_RESSTATE_OVERTURNICON) bOverTurn = TRUE;

			// 绘制目录图标
			rtIcon = rtSub_Name;
			rtSub_Name &= rcSub;
			// 			if(pItemData->eResType & (TP_RES_CATALOG | TP_RES_PROJECT | TP_RES_USER))
			// 			{
			// 				int   iNPIndex  = pItemData ->iNPIndex;
			// 				rtIcon.left    = rtIcon.left + (rtIcon.Width() - m_pCommonRes[TP_CLIPICON_FLODER]->Width())/2;
			// 				rtIcon.top     = rtIcon.top  + (rtIcon.Height()- m_pCommonRes[TP_CLIPICON_FLODER]->Height())/2;
			// 				rtIcon.right   = rtIcon.left + m_pCommonRes[TP_CLIPICON_FLODER]->Width();
			// 				rtIcon.bottom  = rtIcon.top  + m_pCommonRes[TP_CLIPICON_FLODER]->Height();	
			// 				//绘制文件夹大图标
			// 				CRect rtIconView = rcSub & rtIcon;
			// 				if(rtIconView.Width()>0) // 可见时绘制
			// 					ImageList_DrawEx(m_pCommonRes[TP_CLIPICON_FLODER] ->GetImage(),iNPIndex,pDC->m_hDC,rtIcon.left,rtIcon.top ,rtIconView.Width(),rtIconView.Height(),CLR_NONE,CLR_NONE,ILD_TRANSPARENT);
			// 				pItemData ->rtIcon = rtSub_Name;
			// 			}
			// 			else
			{
				// 绘制节目或素材图标
				if(pItemData ->pIcon && pItemData->szIcon.cx > 0 && pItemData->szIcon.cy > 0)
				{					
					if(pItemData->bIs0403Icon)
					{	
						if(rtSub_Name.Width() > 0)
							pDC->FillSolidRect(rtSub_Name,RGB(0,0,0));
						rtIcon.left  = rtIcon.left + (rtIcon.Width() - rtIcon.Height() *4/3)/2;
						rtIcon.right = rtIcon.left + rtIcon.Height() *4/3;
					}
					//绘制节目或素材大图标
					CRect rtIconView = rtIcon & rcSub;
					if(rtIconView.Width()>0)
						TP_StretchDIBitsEx(pDC,pItemData ->pIcon,pItemData ->szIcon.cx,pItemData ->szIcon.cy,rtIcon,rtIconView.Width(),rtIconView.Height(),TRUE);	
					pItemData ->rtIcon = rtSub_Name;
				}
				else 
				{
					//如果找不到绘制的图片，则绘制全黑
					if(rtSub_Name.Width() > 0)
						pDC ->FillSolidRect(rtSub_Name,RGB(0,0,0));
					pItemData ->rtIcon = rtSub_Name;
				}
			}

			//如果选中画橘黄色边框，没有选中画黑色边框,如果列右边和黑线框右边重叠，消除右黑边
			if(uState & LVIS_SELECTED) pDC ->Draw3dRect(rtSub_Name,RGB(246, 152, 11),RGB(246, 152, 11)); 
			else                       pDC ->Draw3dRect(rtSub_Name,RGB(0,0,0),RGB(0,0,0));	
			if(rcSub.right ==  rtSub_Name.right) 
				pDC ->Draw3dRect(rtSub_Name.right,rtSub_Name.top,0,rtSub_Name.Height(),TP_GetSysColor(m_iBackColor),TP_GetSysColor(m_iBackColor));	

			m_aItem[iItem]->aSubItem[i]->rtIcon = rtSub_Name;				

			rcSub.left = rtSub_Name.right + 5;
			iLeft   = rcSub.left;
			iButton = rtSub_Name.bottom;

			rtSmallIcon = rcSub;
			// 			rtSmallIcon.DeflateRect(1,1);	
			// 			rtSmallIcon.right = rtSmallIcon.left + m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Width();
			// 			rtSmallIcon.OffsetRect(2,0);
			// 			rtSmallIcon.top    += (iOffset - m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Height())/2;
			// 			rtSmallIcon.bottom  = rtSmallIcon.top + m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Height();
			// 			int nIndex = TP_GetClipTypeIconIndex(pItemData->eResType, pItemData->eSingSourceType, pItemData);
			// 			//画name栏右上的小图标
			// 			CRect rtSmallIconView = rcSub & rtSmallIcon;
			// 			if(rtSmallIconView.Width() >0)
			// 				ImageList_DrawEx(m_pCommonRes[TP_CLIPICON_TYPEINDICATORS] ->GetImage(),nIndex,pDC->m_hDC,rtSmallIcon.left,rtSmallIcon.top ,rtSmallIconView.Width(),rtSmallIconView.Height(),CLR_NONE,CLR_NONE,ILD_TRANSPARENT);
			//rcSub.left = rtSmallIcon.right + 5;
			rtCommentText |= rcSub;
		}
		rtComment |= rcSub;
		//画选中状态下各列的选中区域

		rcSub.bottom = rcSub.top + iOffset;
		if(uState & LVIS_SELECTED) 
		{
			if(GetFocus() == this) pDC ->FillSolidRect(rcSub,TP_GetSysColor(COLOR_BASE_SELECTITEMA));	
			else                   pDC ->FillSolidRect(rcSub,TP_GetSysColor(COLOR_BASE_SELECTITEM));	
		}
		iLeft = min(rcSub.left,iLeft);
		rcSub.DeflateRect(2,0);
		if(rcSub.right < rcSub .left) {rcSub.right +=2,rcSub.left = rcSub.right - 1;}

 		m_aItem[iItem]->aSubItem[i]->rtLable        = rcSub;
 		m_aItem[iItem]->aSubItem[i]->rtDrag         = m_aItem[iItem] ->aSubItem[i]->rtLable | m_aItem[iItem] ->aSubItem[i]->rtIcon;
 		m_aItem[iItem]->aSubItem[i]->rtDrag .bottom = max(iButton,m_aItem[iItem]->aSubItem[i]->rtDrag .bottom); 
		rDTRect |= rcSub;
		{
			pDC ->DrawText(pItemData ->sText[iColumnIndex[i]],m_aItem[iItem] ->aSubItem[i]->rtLable ,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		}	
	}	

	//如果列数少，没有填满屏幕，将选中列空余部分画上蓝色选中标志
	if(rDTRect.right < m_rtClient.right && (uState & LVIS_SELECTED))
	{		
		if(GetFocus() == this) pDC ->FillSolidRect(CRect(rDTRect.right,rDTRect.top, m_rtClient.right,rDTRect.top + 20),TP_GetSysColor(COLOR_BASE_SELECTITEMA));	
		else                   pDC ->FillSolidRect(CRect(rDTRect.right,rDTRect.top, m_rtClient.right,rDTRect.top + 20),TP_GetSysColor(COLOR_BASE_SELECTITEM));		
	}	

	rtComment.DeflateRect(2,2);
	rtComment.OffsetRect(2,0);
	rtComment.left  = iLeft;
	rtComment.top  += iOffset;
	rtComment.DeflateRect(0,0,2,0);	

	//XN00024373 qushaojie 
	if (rtCommentText.Width() > 0) rtCommentText = rtComment; 

	//画右下的贯穿name右边所有列的黑框
	if(rtComment.right > rtComment.left) pDC->Draw3dRect(rtComment,RGB(0,0,0),RGB(0,0,0));
	rtCommentText.DeflateRect(3,3);
	// 	if(pItemData ->eResType & (TP_RES_CLIP | TP_RES_ALLPROGRAM))
	// 	{		
	// 		pDC->DrawText(pItemData->sText[TP_RESTEXT_COMMENT],rtCommentText,DT_VCENTER|DT_SINGLELINE);
	// 	}
	// 	else if((pItemData ->eResType & TP_RES_ALLCGTEMPLATE))
	// 	{
	// 
	// 		pDC->DrawText(pItemData->sText[TP_TEMPALTECGTEXT_COMMENT],rtCommentText,DT_VCENTER|DT_SINGLELINE);
	// 	}
	// 	else if(pItemData->eResType & TP_RES_USER)
	// 	{
	// 		pDC->DrawText(pItemData->sText[TP_USEREXT_COMMENT],rtCommentText,DT_VCENTER|DT_SINGLELINE);
	// 	}
}
void  CTPArticleListCtrl::GetItemResData(TPListItemData *pItemData,BOOL bGetData)
{
	if(!pItemData ->bGetData)
	{
		TPArticleData stuArticle;
		if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(pItemData->guidRes,TP_GRADE_ALL,stuArticle))
		{
			pItemData->sText[0] = stuArticle.stuChannelItem.cItemTitle;
			if(stuArticle.aPictureItem.GetSize() > 0 && stuArticle.aPictureItem[0]->pIconBuf)
			{
				pItemData->pIcon = stuArticle.aPictureItem[0]->pIconBuf;
				pItemData->szIcon = stuArticle.aPictureItem[0]->szIcon;
				stuArticle.aPictureItem[0]->bDelBuf = FALSE;
				pItemData->bDelIcon = TRUE;
			}
			pItemData ->bGetData = TRUE;
		}
	}
}
