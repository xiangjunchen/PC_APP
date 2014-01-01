// TPArticleListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TPArticleListCtrl.h"

// CTPArticleListCtrl
#define WM_REFRESHITEM					(WM_USER + 10)

IMPLEMENT_DYNAMIC(CTPArticleListCtrl, CTPListCtrlEx)

CTPArticleListCtrl::CTPArticleListCtrl()
{
	m_pArticleMainDlg = NULL;
	m_eListViewType   = TP_LISTVIEW_UNKNOW;
	InitializeCriticalSection(&m_csTeamCmd);

	m_bOpenThread = FALSE;
	m_hOpenThread = CreateEvent(NULL,TRUE,0,NULL);

	m_nUpdateCount    = 0;
	m_nUpdateCountAll = 0;
}

CTPArticleListCtrl::~CTPArticleListCtrl()
{
	DeleteCriticalSection(&m_csTeamCmd); 

}


BEGIN_MESSAGE_MAP(CTPArticleListCtrl, CTPListCtrlEx)
	//listctrlex
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_REFRESHITEM, OnMsgRefreshItem)	

	ON_WM_CREATE()
END_MESSAGE_MAP()


// CTPArticleListCtrl message handlers

void CTPArticleListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!m_rtClient.PtInRect(point)) return ; // 如果不在客户区不处理
	int iDbClickItem = CTPListCtrlEx::HitTest(point);
	if(iDbClickItem >=0)
	{
		m_uState |= STATE_DBCLICK;
// 		if(m_pMsgWnd) m_pMsgWnd->PostMessage(WM_LIST_DBCLICK, iDbClickItem, (LPARAM)this);
//		if(m_bForList) return;
		CTPListCtrlEx::EndEdit();
		TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iDbClickItem);
		if(pItemData)
		{
			if(pItemData ->eResType == TP_RES_CHANNEL)
			{
				SetChannelChildData(pItemData->guidRes);
			}
			else if(pItemData->eResType == TP_RES_CHANNELNODE)
			{
				TPResDataArray aResData;
				g_stuArticleInterface.stuChannelInterface.TP_GetChannelChild(pItemData->guidRes, aResData);

				SetViewType(TP_RES_CHANNELNODE);
				SetResData(aResData);
			}

		}
	}
}
BOOL CTPArticleListCtrl::UpdateCurArticle(GUID guidRes)
{
	TPArticleData stuArticle;
	if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(guidRes,TP_GRADE_ALL,stuArticle))
	{
		CString sFileName = g_stuArticleInterface.stuArticleInterfce.TP_GetCurArticleHtmlPath();
		CTPArticleParser::SaveHtml(sFileName, stuArticle.cText, stuArticle.stuChannelItem.cItemTitle,stuArticle.stuChannelItem.cChannelTitle,stuArticle.stuChannelItem.cItemPubDate,TRUE,m_pArticleMainDlg->m_bFullScreenArticle);
		if(m_pArticleMainDlg)
		{
			m_pArticleMainDlg->AdjustHtml(sFileName);
			m_pArticleMainDlg->m_guidCurArticle = stuArticle.guidRes;
		}
	}
	return TRUE;
}
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
		UpdateCurArticle(pItemData->guidRes);
	}	
}
void CTPArticleListCtrl::GotoArticle(BOOL bForward)
{
	//TPArticleData stuArticle;
	//if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(m_guidCurArticle,TP_GRADE_ALL,stuArticle))
	//{
	//	TPChannelData stuChannel;
	//	if(S_OK == g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(stuArticle.guidNode,stuChannel))
	//	{
	//		GUID guidArticle = GUID_NULL;
	//		for (int l = stuChannel.aChannelItemAll.GetSize() - 1; l >= 0 ;  l--)
	//		{
	//			if (m_guidCurArticle == stuChannel.aChannelItemAll[l]->guidItem)
	//			{
	//				if(bForward && (l < (stuChannel.aChannelItemAll.GetSize()-1)))
	//					guidArticle = stuChannel.aChannelItemAll[l+1]->guidItem;
	//				else if(!bForward && l > 0)
	//					guidArticle = stuChannel.aChannelItemAll[l-1]->guidItem;
	//			}
	//		}
	//		if(guidArticle != GUID_NULL)
	//		{
	//			m_pArticleList->UpdateCurArticle(guidArticle);
	//		}
	//	}
	//}

	if(m_eListViewType == TP_LISTVIEW_PRIVATEARTILCE)
	{
		POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
		if (posSel)
		{
			int iSel = -1;
			iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
			if(iSel>=0)
			{				
				if(bForward)    iSel--;
				else		    iSel++;
				if(iSel <= 0)	iSel = 0;
				if(iSel >= GetItemCount())	iSel = GetItemCount() - 1;
				FunSelectItem(iSel,TRUE);
				EnsureVisible(iSel, FALSE);

			}
		}
	}
}
void CTPArticleListCtrl::SetChannelChildData(GUID guidChannel)
{
	TPChannelData stuChannel;
	g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(guidChannel,stuChannel);
	//AfxMessageBox(stuChannel.stuChannelBase.cChannelTitle);

	TPChannelBase *pChannelInfo = NULL;
	CTPChannelParser stuChannelParser;
	stuChannelParser.SetChannelAddress(stuChannel.stuChannelBase.cChannelAddress);
	stuChannelParser.GetChannelInfo(pChannelInfo);
	if(!pChannelInfo || pChannelInfo->aChannelItem.GetSize() <= 0)	{ASSERT(0); return ;}

	stuChannel.stuChannelBase = *pChannelInfo;
	stuChannel.AppendUpdateItem();

	//update
	UpdateLoadStatus(TRUE,TRUE);

	SetViewType(TP_RES_ARTICLE);
	TPResDataArray aResData;
	for (int l = stuChannel.aChannelItemAll.GetSize() - 1; l >= 0 ;  l--)
	{
		TPResData stuRes;
		TPArticleData stuArticle;
		GUID guidItem = stuChannel.aChannelItemAll[l]->guidItem;
		if((guidItem != GUID_NULL) && S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(guidItem,TP_GRADE_ALL,stuArticle))
		{
		}
		else
		{
			CoCreateGuid(&stuArticle.guidRes);
			stuArticle.guidNode = stuChannel.guidRes;
			stuArticle.stuChannelItem = *stuChannel.aChannelItemAll[l];

			g_stuArticleInterface.stuArticleInterfce.TP_SetArticleInfo(stuArticle.guidRes,TP_GRADE_ALL,stuArticle);
			stuChannel.aChannelItemAll[l]->guidItem = stuArticle.guidRes;
			m_nUpdateCountAll ++;
		}
		stuRes.guidRes = stuArticle.guidRes;
		stuRes.guidNode = stuArticle.guidNode;
		stuRes.eResType = TP_RES_ARTICLE;
		aResData.Add(stuRes);
	}

	SetResData(aResData);
	g_stuArticleInterface.stuChannelInterface.TP_SetChannelInfo(stuChannel.guidRes,stuChannel);
	GetArticleByThread(aResData);  

}
void CTPArticleListCtrl::SetViewType(TP_RES_TYPE eResType)
{
	if(eResType == TP_RES_CHANNEL)
	{
		m_lIconSize = 2;	
		m_szItemIcon = CSize(200,160);

		m_iViewType = VIEWTYPE_ICON;//VIEWTYPE_REPORT;//;//VIEWTYPE_REPORT;//;
		m_eListViewType = TP_LISTVIEW_PRIVATECHANNEL;
	}
 	else if(eResType == TP_RES_ARTICLE)
 	{
 		m_iViewType = VIEWTYPE_TEXTPICTURE;//VIEWTYPE_REPORT;//;//VIEWTYPE_REPORT;//;
		m_eListViewType = TP_LISTVIEW_PRIVATEARTILCE;
 	}
	else if(eResType == TP_RES_CHANNELNODE)
	{
 		m_iViewType = VIEWTYPE_TEXTPICTURE;//VIEWTYPE_REPORT;//;//VIEWTYPE_REPORT;//;
		m_eListViewType = TP_LISTVIEW_PUBLICCHANNEL;

	}
}

BOOL CTPArticleListCtrl::SetResData(TPResDataArray &aResData)
{
	//column
// 	while(CTPListCtrlEx::DeleteColumn(0));
	DeleteAllItems();
	int iIndex = 0; 

	for (int l = 0 ; l < aResData.GetSize(); l++)
	{
		CTPListCtrlEx::InsertItem(iIndex,_L("         "));
		//InsertItem(iIndex,stuArticle.stuChannelItem.cItemTitle);//CTPListCtrlEx::InsertItem(iResNeed,_L("         "));
		TPListItemData *pItemData = new TPListItemData();
	//	pItemData ->SetResBaseData(hChildRes[l]);
		pItemData->guidRes = aResData[l].guidRes;
		pItemData->guidNode = aResData[l].guidNode;
		pItemData ->pItem = m_aItem[iIndex];
		pItemData->eResType = aResData[l].eResType;
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
void CTPArticleListCtrl::PaintHead(CDC *pDC)
{
	return ;
	if(m_aColumn.GetSize() <= 0)	return;

	CString sText = _T("头部显示位置");
	pDC ->SetTextColor(TP_GetSysColor(COLOR_TL_END));	
	pDC ->DrawText(sText,m_aColumn[0]->rtItem ,/*DT_SINGLELINE|*/DT_END_ELLIPSIS|DT_WORDBREAK|DT_LEFT|DT_VCENTER);

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
	int iOffset = 35;
	CRect rtComment = CRect(0, 0, 0, 0), rtCommentText = CRect(0, 0, 0, 0);

	CRect rtClient ;
	GetClientRect(&rtClient);
	for(int i = 0; i < iColumnCount; i++)
	{
		if(!m_aColumn[i]->bShow) continue;
		CRect rcSub(0,0,0,0);
		GetSubItemRect(iItem,i,LVIR_BOUNDS,rcSub);

		rcSub.right -= 10;
		if(rcSub.right < m_rtClient.left || rcSub.left > m_rtClient.right) continue;
		pDC ->FillSolidRect(rcSub,TP_GetSysColor(m_iBackColor));

		CRect rtAdd = rcSub;
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

			// 绘制节目或素材图标
			if(pItemData ->pIcon && pItemData->szIcon.cx > 0 && pItemData->szIcon.cy > 0)
			{					
				if(0/*pItemData->bIs0403Icon*/)
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
			rtCommentText |= rcSub;
		}
		rtComment |= rcSub;
		//画选中状态下各列的选中区域

		rcSub.bottom = iButton;
		if(uState & LVIS_SELECTED) 
		{
			if(GetFocus() == this) pDC ->FillSolidRect(rcSub,TP_GetSysColor(COLOR_BASE_SELECTITEMA));	
			else                   pDC ->FillSolidRect(rcSub,TP_GetSysColor(COLOR_BASE_SELECTITEM));	
		}

		rcSub.bottom = rcSub.top + iOffset;
		iLeft = min(rcSub.left,iLeft);
		rcSub.DeflateRect(2,0);
		if(rcSub.right < rcSub .left) {rcSub.right +=2,rcSub.left = rcSub.right - 1;}

		rcSub.top += 5;
		if(pItemData->eResType & TP_RES_CHANNEL)
		{
			CString sCare = _T("关注中");
			rtAdd.left = rtAdd.right - 2*rtAdd.Height();
			pDC ->MoveTo(rtAdd.left,rtAdd.top); pDC ->LineTo(rtAdd.left,rtAdd.bottom);		
			rtAdd.left += 5;
			pDC ->DrawText(pItemData ->sText[TP_RESTEXT_STATE],rtAdd ,/*DT_SINGLELINE|*/DT_WORDBREAK|DT_LEFT|DT_VCENTER);

		}
 		m_aItem[iItem]->aSubItem[i]->rtLable        = rcSub;
 		m_aItem[iItem]->aSubItem[i]->rtDrag         = m_aItem[iItem] ->aSubItem[i]->rtLable | m_aItem[iItem] ->aSubItem[i]->rtIcon;
 		m_aItem[iItem]->aSubItem[i]->rtDrag .bottom = max(iButton,m_aItem[iItem]->aSubItem[i]->rtDrag .bottom); 
		rDTRect |= rcSub;
		{
			pDC ->DrawText(pItemData ->sText[iColumnIndex[i]],m_aItem[iItem] ->aSubItem[i]->rtLable ,/*DT_SINGLELINE|*/DT_END_ELLIPSIS|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
		}	
	}	

	//如果列数少，没有填满屏幕，将选中列空余部分画上蓝色选中标志
// 	if(rDTRect.right < m_rtClient.right && (uState & LVIS_SELECTED))
// 	{		
// 		if(GetFocus() == this) pDC ->FillSolidRect(CRect(rDTRect.right,rDTRect.top, m_rtClient.right,rDTRect.top + 20),TP_GetSysColor(COLOR_BASE_SELECTITEMA));	
// 		else                   pDC ->FillSolidRect(CRect(rDTRect.right,rDTRect.top, m_rtClient.right,rDTRect.top + 20),TP_GetSysColor(COLOR_BASE_SELECTITEM));		
// 	}	

	rtComment.DeflateRect(2,2);
	rtComment.OffsetRect(2,0);
	rtComment.left  = iLeft;
	rtComment.top  += iOffset;
	rtComment.DeflateRect(0,0,2,0);	

	//XN00024373 qushaojie 
	if (rtCommentText.Width() > 0) rtCommentText = rtComment; 

	//画右下的贯穿name右边所有列的黑框
//	if(rtComment.right > rtComment.left) pDC->Draw3dRect(rtComment,RGB(0,0,0),RGB(0,0,0));
	rtCommentText.DeflateRect(3,3);
}
BOOL CTPArticleListCtrl::GetArticleData(GUID guidArticle)
{
	TPArticleData stuArticle;
	if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(guidArticle,TP_GRADE_ALL,stuArticle))
	{
		if(stuArticle.cText == NULL)
		{
			TPChannelData stuChannel;
			if(S_OK == g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(stuArticle.guidNode,stuChannel))
			{
				TCHAR *cItemText = NULL, *cImgPath = NULL;
				CTPArticleParser stuArticleParser;
				stuArticleParser.SetChannelItem(&stuArticle.stuChannelItem, stuChannel.cKeyDiv);
				stuArticleParser.GetItemInfo(cItemText, cImgPath);
				if(cItemText)
				{
					ASSERT(TP_StrLen(cItemText) > 50);
					TP_StrCpy(stuArticle.cText, cItemText, TP_StrLen(cItemText));
				}
				if(stuArticle.aPictureItem.GetSize() <= 0 && cImgPath)
				{
					CString sImgSourcePath = g_stuArticleInterface.stuArticleInterfce.TP_GetArticleResourcePath(stuArticle.guidRes, cImgPath);
					int iWidth = 0 ; int iHeight = 0;
					CTPImgDecode stuDec;
					if(FIO_Succeed == stuDec.Open(sImgSourcePath.GetBuffer(),iWidth,iHeight))
					{
						LPBYTE pByte = new BYTE[iWidth * iHeight * sizeof(DWORD)];
						ZeroMemory(pByte, iWidth * iHeight * sizeof(DWORD));

						if(FIO_Succeed == stuDec.GetImage(pByte))
						{
							int iIconWidth = TP_RESICON_WIDTH_HD,  iIconHeight = TP_RESICON_HIGH_HD;
							LPBYTE pIcon = new BYTE[iIconWidth * iIconHeight * sizeof(DWORD)];
							ZeroMemory(pIcon,iIconWidth * iIconHeight * sizeof(DWORD));	
							TP_StretchBlt_Mean((LPDWORD)pIcon, 0, 0, iIconWidth, iIconHeight, iIconWidth, iIconHeight, (LPDWORD)pByte, 0, 0, iWidth, iHeight, iWidth, iHeight, TP_BUFFER_COPY, TP_PARITY_BOTH);

							TPPictureItem *pPicture = new TPPictureItem;
							pPicture->pIconBuf  = pIcon;
							pPicture->szIcon.cx = iIconWidth;
							pPicture->szIcon.cy = iIconHeight;
							TP_StrCpy(pPicture->cPicPath,sImgSourcePath.GetBuffer(),sImgSourcePath.GetLength());
							stuArticle.aPictureItem.Add(pPicture);
						}
						if(pByte) {delete pByte; pByte = NULL;}
					}
					g_stuArticleInterface.stuArticleInterfce.TP_SetArticleInfo(guidArticle,TP_GRADE_ALL,stuArticle);
				}
			}
		}
	}
	return TRUE;
}
void  CTPArticleListCtrl::GetItemResData(TPListItemData *pItemData,BOOL bGetData)
{
	if(!pItemData ->bGetData)
	{
		if(pItemData->eResType == TP_RES_ARTICLE)
		{
			TPArticleData stuArticle;
			if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(pItemData->guidRes,TP_GRADE_ALL,stuArticle))
			{
				pItemData->sText[TP_RESTEXT_NAME] = stuArticle.stuChannelItem.cItemTitle;
				pItemData->eNodeType = stuArticle.eNodeType;
				if(stuArticle.aPictureItem.GetSize() > 0 && stuArticle.aPictureItem[0]->pIconBuf)
				{
					pItemData->ReleaseIconBuf();
					pItemData->pIcon = stuArticle.aPictureItem[0]->pIconBuf;
					pItemData->szIcon = stuArticle.aPictureItem[0]->szIcon;
					stuArticle.aPictureItem[0]->bDelBuf = FALSE;
					pItemData->bDelIcon = TRUE;
					pItemData->sFileName = stuArticle.aPictureItem[0]->cPicPath;
					
					POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
					if (posSel)
					{
						int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
						if(iSel>=0)
						{
							TPListItemData *pItemDataSelect = (TPListItemData *)CTPListCtrlEx::GetItemData(iSel);
							if(pItemDataSelect && pItemData->guidRes == pItemDataSelect->guidRes)
								UpdateCurArticle(pItemDataSelect->guidRes);
						}
					}
				}
// 				else
// 				{
// 					TPResDataArray aResData;
// 					TPResData stuRes;
// 					stuRes.guidRes = stuArticle.guidRes;
// 					aResData.Add(stuRes);
// 					GetArticleByThread(aResData);  
// 				}

				pItemData ->bGetData = TRUE;
			}
		}
		else if(pItemData->eResType == TP_RES_CHANNEL)
		{
			TPChannelData stuChannel;
			if(S_OK == g_stuArticleInterface.stuChannelInterface.TP_GetChannelInfo(pItemData->guidRes,stuChannel))
			{
				pItemData->sText[TP_RESTEXT_NAME]  = stuChannel.stuChannelBase.cChannelTitle;
				if(stuChannel.eNodeType & TP_CHANNEL_SYSTEM)
				{
					GUID guidExist = GUID_NULL;
					if(g_stuArticleInterface.stuChannelInterface.TP_IsChannelExist(guidPrivateChannelNode,&stuChannel.stuChannelBase,guidExist))
						pItemData->sText[TP_RESTEXT_STATE] = _T("已关注");				
					else
						pItemData->sText[TP_RESTEXT_STATE] = _T("关注");				
				}
				pItemData->eNodeType = stuChannel.eNodeType;

				int iArticleSize = stuChannel.aChannelItemAll.GetSize();
				if(iArticleSize > 0)
				{
					TPArticleData stuArticle;
					if(S_OK == g_stuArticleInterface.stuArticleInterfce.TP_GetArticleInfo(stuChannel.aChannelItemAll[iArticleSize-1]->guidItem,TP_GRADE_ALL,stuArticle))
					{
						if(stuArticle.aPictureItem.GetSize() > 0 && stuArticle.aPictureItem[0]->pIconBuf)
						{
							pItemData->pIcon = stuArticle.aPictureItem[0]->pIconBuf;
							pItemData->szIcon = stuArticle.aPictureItem[0]->szIcon;
							stuArticle.aPictureItem[0]->bDelBuf = FALSE;
							pItemData->bDelIcon = TRUE;
							pItemData->sFileName = stuArticle.aPictureItem[0]->cPicPath;

						}
					}
				}
				pItemData ->bGetData = TRUE;
			}
		}
		else if(pItemData->eResType == TP_RES_CHANNELNODE)
		{
			TPChannelNodeData stuChannelNode;
			if(S_OK == g_stuArticleInterface.stuChannelNodeInterface.TP_GetChannelNodeInfo(pItemData->guidRes,stuChannelNode))
			{
				pItemData->sText[TP_RESTEXT_NAME] = stuChannelNode.cNodeName;
				pItemData->eNodeType = stuChannelNode.eNodeType;

// 				pItemData->pIcon = stuArticle.aPictureItem[0]->pIconBuf;
// 				pItemData->szIcon = stuArticle.aPictureItem[0]->szIcon;
// 				stuArticle.aPictureItem[0]->bDelBuf = FALSE;
// 				pItemData->bDelIcon = TRUE;
// 				pItemData->sFileName = stuArticle.aPictureItem[0]->cPicPath;
// 				pItemData ->bGetData = TRUE;
			}
		}
	}
}
void CTPArticleListCtrl::DrawReport(CDC *pDC,int iItem)
{
	TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iItem);
	if(pItemData == NULL) return ;
	pItemData ->rtIcon = CRect(0,0,0,0);
	GetItemResData(pItemData);
	if(!pItemData ->pItem->bShow) return;
// 	if(m_bViewFile) GetItemResClipFile(pItemData); 
	CRect rcSub;
	rcSub  = CRect(0,0,0,0);
	UINT uState = CTPListCtrlEx::GetItemState(iItem,LVIS_SELECTED |  LVIS_FOCUSED);	
	int iColumnCount = this ->GetColumnCount();
	if(iColumnCount <=0) return;
	int iIndex       = 0; 	
	if(uState & LVIS_SELECTED )    pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));	
	else                           pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));	
	CRect  rDRect;//存储colomu总长度的矩形 li song ming 2008-6-12 XN00012383 
	rDRect = CRect(0,0,0,0);
	int iColumnIndex[M2];
	int a =0;
	GetColumnOrderArray(iColumnIndex,iColumnCount);
	INT nHeadIndex = 0;//TP_NodeTypeHeadIndex(m_eShowNodeType);
	for(int i=iColumnCount - 1;i>=0;i--)
	{
		if(!m_aColumn[i]->bShow) continue;
		GetSubItemRect(iItem,i,LVIR_BOUNDS,rcSub);
		if(rcSub.right<m_rtClient.left || rcSub.left > m_rtClient.right) continue;
		if(uState & LVIS_SELECTED) 
		{
			if(GetFocus() == this) pDC ->FillSolidRect(rcSub,TP_GetSysColor(COLOR_BASE_SELECTITEMA));	
			else                   pDC ->FillSolidRect(rcSub,TP_GetSysColor(COLOR_BASE_SELECTITEM));	
		}
		else                     pDC ->FillSolidRect(rcSub,TP_GetSysColor(m_iBackColor));				
		if(iColumnIndex[i]==0)
		{
			CRect rtIcon = rcSub;
			rtIcon.DeflateRect(1,1);	
			rtIcon.right = rtIcon.left + 0;//m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Width();
			rtIcon.OffsetRect(2,0);		
			CRect rtIconView = rtIcon & rcSub;
			if(rtIconView.Width() > 0)
			{
// 				if(m_dwWndType & TP_WND_CLIPEXPLORER)
// 				{
// 					INT nIndex = TP_GetClipTypeIconIndex(pItemData->eResType, pItemData->eSingSourceType, pItemData);
// 					ImageList_DrawEx(m_pCommonRes[TP_CLIPICON_TYPEINDICATORS] ->GetImage(),nIndex,pDC->m_hDC,rtIcon.left,rtIcon.top + (rtIcon.Height() - m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Height())/2, rtIconView.Width(), rtIconView.Height(),CLR_NONE, CLR_NONE, ILD_TRANSPARENT);	
// 				}
// 				else if(m_dwWndType & TP_WND_FXEXPLORER)
// 				{
// 					INT nIndex = TP_GetClipTypeIconIndex(pItemData->eResType, pItemData->eSingSourceType, pItemData);
// 					if(nIndex < 0) nIndex = TP_CLIPTYPE_EFFECTTEMPLATE;
// 					ImageList_DrawEx(m_pCommonRes[TP_CLIPICON_TYPEINDICATORS] ->GetImage(),nIndex,pDC->m_hDC,rtIcon.left,rtIcon.top + (rtIcon.Height() - m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Height())/2,rtIconView.Width(), rtIconView.Height(), CLR_NONE, CLR_NONE, ILD_TRANSPARENT);
// 				}
// 				pItemData ->rtIcon = rtIcon;
			}

			m_aItem[iItem]->aSubItem[i]->rtIcon = rtIcon;			
			rcSub.left = rtIcon.right + 5;
		}
		rcSub.DeflateRect(2,0);

		//CRect rtDraw = rcSub;
		//rtDraw.left += rcSub.Width()/2- rcSub.Height()/2;
		//rtDraw.left = max(rtDraw.left, rcSub.left);
		//rtDraw.right = rtDraw.left + rcSub.Height() + 20;
		//if(rtDraw.right >= rcSub.right)
		//	rtDraw.right = rcSub.right;
		//COLORREF cColor = TP_GetResColor((TP_RES_COLOR)pItemData->cResColor, FALSE, TRUE);
		//rtDraw.DeflateRect(1,4,1,4);
		//pDC->Draw3dRect(rtDraw, cColor, cColor);
		//rtDraw.DeflateRect(1,1,1,1);
		//pDC->FillSolidRect(rtDraw, cColor);
		pDC ->DrawText(pItemData ->sText[iColumnIndex[i]],rcSub,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS);
// 			else
// 				pDC ->DrawText(pItemData ->sText[iColumnIndex[i]],rcSub,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
//		}
		m_aItem[iItem] ->aSubItem[i]->rtLable = rcSub;
		m_aItem[iItem]->aSubItem[i]->rtDrag = m_aItem[iItem] ->aSubItem[i]->rtLable | m_aItem[iItem] ->aSubItem[i]->rtIcon;
		rDRect |=  rcSub;
	}

	if(rDRect.right < m_rtClient.right)
	{		
		if(uState & LVIS_SELECTED) 
		{
			if(GetFocus() == this) pDC ->FillSolidRect(CRect(rDRect.right,rDRect.top, m_rtClient.right,rDRect.bottom),TP_GetSysColor(COLOR_BASE_SELECTITEMA));	
			else                   pDC ->FillSolidRect(CRect(rDRect.right,rDRect.top, m_rtClient.right,rDRect.bottom),TP_GetSysColor(COLOR_BASE_SELECTITEM));	
		}
	}
}
void CTPArticleListCtrl::DrawIcon(CDC *pDC,int iItem)
{	
	CRect rtItem,rtOK,rtText,rtClipType,rtState,rtQuality,rtEdge,rtIcon,rtOtherOpen,rtClose;

	INT nOffset = 0;
	INT nTypeOffset = 3;
	int   iIndex = TP_CLIPICON_N4;
	if(m_lIconSize == 1)        {nOffset = 0;}
	else if(m_lIconSize == 2)   {nOffset = 3;}
	else if(m_lIconSize == 3)	{nOffset = 2;}
	else if(m_lIconSize == 4)	{nOffset = 1;}
	GetItemRect(iItem,&rtItem,LVIR_BOUNDS);	
	m_aItem[iItem] ->aSubItem[0]->rtDrag  = rtItem;
	rtEdge = CRect(6+nOffset,25,6+nOffset,25);

	rtClose			= rtItem;
	rtClose.left    = rtClose.right - 40;
	rtClose.bottom  = rtClose.top + 30;

	rtText          = rtItem;
	rtText.top      = rtText.bottom - rtEdge.bottom;
	rtText.DeflateRect(rtEdge.left,0);

	rtIcon         = rtItem;
	rtIcon.DeflateRect(rtEdge.left,rtEdge.top,rtEdge.right,rtEdge.bottom);

	rtIcon.top      = rtIcon.top + (rtIcon.Height() - rtIcon.Width() *9/16)/2;
	rtIcon.bottom   = rtIcon.top + rtIcon.Width() *9/16;
	CRect rtIconEx  = rtIcon;

	m_aItem[iItem]->aSubItem[0]->rtLable = rtText;
	m_aItem[iItem]->aSubItem[0]->rtIcon  = rtIcon;

	UINT uState  = CTPListCtrlEx::GetItemState(iItem,LVIS_SELECTED|  LVIS_FOCUSED);

	TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iItem);
	if(pItemData == NULL) return ;
	GetItemResData(pItemData);
//	if(m_lIconSize < 4) GetItemResClipFile(pItemData);   // lihuili 大图标时取文件 
	if(!pItemData ->pItem->bShow) return;
//	if(pItemData ->dwState & TP_RESSTATE_HIDE) return;

	//CTPGuidLock guidLock(pItemData->guidRes,TP_CLIP_ICON,(pItemData->eResType & TP_RES_CLIP) && m_lIconSize == 4);//防止和底下SetClipDataInfo将ICON删除了 swx

	pDC ->SetBkMode(TRANSPARENT);
	INT nTextColor = COLOR_BASE_TEXT;
//	if(pItemData ->dwState & TP_RESSTATE_CUT)	iIndex = TP_CLIPICON_C4;
//	else 
	if(uState & LVIS_SELECTED )           
	{
		iIndex     = TP_CLIPICON_S4;
		nTextColor = COLOR_TRIM_BACK;
// 		if(m_pClipExplorer && m_pClipExplorer ->m_pFocus != this)
// 		{
// 			iIndex     = TP_CLIPICON_D4;
// 			nTextColor = COLOR_BASE_TEXTFOCUS;
// 		}
	}
	if(m_lIconSize == 1)      iIndex -= 3*4;
	else if(m_lIconSize == 2) iIndex -= 2*4;
	else if(m_lIconSize == 3) iIndex -= 1*4;
	else if(m_lIconSize == 4) iIndex -= 0*4;
	else                      iIndex -= 3*4;

	pDC->SetTextColor(TP_GetSysColor(nTextColor));

// 	rtItem.right  = rtItem.left + m_pCommonRes[iIndex] ->Width();
// 	rtItem.bottom = rtItem.top  + m_pCommonRes[iIndex] ->Height();

// 	if(pItemData->cResColor > 0) iIndex = iIndex+ 16* TP_GetResColorIndex(pItemData->cResColor);
// 	TP_SetDIBitsToDevice(pDC,m_pCommonRes[iIndex] ->GetByte(),m_pCommonRes[iIndex] ->Width(),m_pCommonRes[iIndex] ->Height(),rtItem);
// 	TP_GetItemRefCount(pItemData);
	if(pItemData && (pItemData ->eResType & TP_RES_CHANNEL))
	{
		if(m_lIconSize <4) //大buffer
		{
			if(pItemData ->bIs0403Icon)
			{
				pDC->FillSolidRect(rtIcon,RGB(0,0,0));
// 				rtIcon.left  = rtIcon.left + (rtIcon.Width() - rtIcon.Height() *4/3)/2;
// 				rtIcon.right = rtIcon.left + rtIcon.Height() *4/3;
			}

			LPBYTE pDrawBuffer  = NULL;
			CSize  szDrawBuffer = CSize(0, 0);
// 			if(pItemData->lListItemFlag & TP_ITEMFLAG_MEDIAOFFLINE)
// 			{
// 				pDrawBuffer  = m_pCommonRes[TP_CLIPICON_OFFLINELARGE]->GetByte();
// 				szDrawBuffer = CSize(m_pCommonRes[TP_CLIPICON_OFFLINELARGE]->Width(), m_pCommonRes[TP_CLIPICON_OFFLINELARGE]->Height());
// 			}
// 			else if(pItemData->lListItemFlag & TP_ITEMFLAG_VIDEO)
			{
				if(!pItemData->bGetBuffer)
				{
					if(!pItemData->sFileName.IsEmpty())
					{
						int iWidth = 0 ; int iHeight = 0;
						CTPImgDecode stuDec;
						if(FIO_Succeed == stuDec.Open(pItemData->sFileName.GetBuffer(),iWidth,iHeight))
						{
							LPBYTE pByte = new BYTE[iWidth * iHeight * sizeof(DWORD)];
							ZeroMemory(pByte, iWidth * iHeight * sizeof(DWORD));
							if(FIO_Succeed == stuDec.GetImage(pByte))
							{
								int iIconWidth = rtIcon.Width(),  iIconHeight = rtIcon.Height();
								LPBYTE pIcon = new BYTE[iIconWidth * iIconHeight * sizeof(DWORD)];
								ZeroMemory(pIcon,iIconWidth * iIconHeight * sizeof(DWORD));	
								TP_StretchBlt_Mean((LPDWORD)pIcon, 0, 0, iIconWidth, iIconHeight, iIconWidth, iIconHeight, (LPDWORD)pByte, 0, 0, iWidth, iHeight, iWidth, iHeight, TP_BUFFER_COPY, TP_PARITY_BOTH);
								pItemData->pBuffer       = pIcon;
								pItemData->szBuffer.cx = iIconWidth;
								pItemData->szBuffer.cy = iIconHeight;
							}
							if(pByte) {delete pByte; pByte = NULL;}
						}

					}
					pItemData ->bGetBuffer = TRUE;
				}
				pDrawBuffer  = pItemData->pBuffer;
				szDrawBuffer = pItemData->szBuffer;
				if(NULL != pDrawBuffer)
				{
					TP_StretchDIBits(pDC,pDrawBuffer,szDrawBuffer.cx,szDrawBuffer.cy,rtIcon,TRUE/*!(pItemData->dwState & TP_RESSTATE_OVERTURNICON)*/);		
				}
				else
				{
					pDC->FillSolidRect(rtIcon,RGB(0,0,0));
				}
			}
		}
		else if(pItemData ->pIcon && pItemData ->szIcon.cx > 0 && pItemData ->szIcon.cy >0)
		{
			if(pItemData->bIs0403Icon)
			{
				pDC->FillSolidRect(rtIcon,RGB(0,0,0));
				rtIcon.left  = rtIcon.left + (rtIcon.Width() - rtIcon.Height() *4/3)/2;
				rtIcon.right = rtIcon.left + rtIcon.Height() *4/3;
			}
			TP_StretchDIBits(pDC,pItemData ->pIcon,pItemData ->szIcon.cx,pItemData ->szIcon.cy,rtIcon,TRUE);		
		}
		else pDC->FillSolidRect(rtIcon,RGB(0,0,0));

		//Draw OK NG 
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_OKNG] ->GetImage(),pItemData ->dwUseState,pDC->m_hDC,rtOK.left,rtOK.top ,ILD_TRANSPARENT);

		// Draw Clip Type
	//	int nIndex = TP_GetClipTypeIconIndex((TP_CLIP_TYPE)pItemData->eResType, pItemData->eSingSourceType, pItemData);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_TYPEINDICATORS] ->GetImage(), nIndex,pDC->m_hDC,rtClipType.left,rtClipType.top ,ILD_TRANSPARENT);


	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_STATUSINDICATORS] ->GetImage(),iQuality,pDC->m_hDC,rtQuality.left,rtQuality.top ,ILD_TRANSPARENT);
		// Draw Used Flags

		// Draw Name
		pDC ->DrawText(pItemData ->sText[0],rtText,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS);

		if(m_eListViewType == TP_LISTVIEW_PRIVATECHANNEL)
		{
			pDC ->DrawText(_T("取消"),rtClose,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS);
		}

// 		// (liupingTFS1472) 建议把在素材选中时时码显示稍微往上移动.
// 		if((uState & LVIS_SELECTED && m_pClipExplorer && m_pClipExplorer ->m_pFocus == this)|| (pItemData && pItemData ->dwState & TP_RESSTATE_CUT))           
// 			rtIconEx.bottom -= 2;
// 		DrawIconOverlay(pDC,iItem, rtIconEx); 
	}
}
void CTPArticleListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_rtClient.PtInRect(point)) return ; // 如果不在客户区不处理
	//snp4100041595
	int nItem = CTPListCtrlEx::HitTest(point, 0);
	if(m_eListViewType == TP_LISTVIEW_PUBLICCHANNEL)
	{
		CRect rcSub(0,0,0,0);
		GetSubItemRect(nItem,0,LVIR_BOUNDS,rcSub);
		CRect rtAdd = rcSub;
		rtAdd.left = rcSub.right - 2*rcSub.Height();
		if(rtAdd.left < point.x && rtAdd.right > point.x)
		{
			//公共频道添加到个人频道

			TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(nItem);
			if(pItemData == NULL) return ;
			GetItemResData(pItemData);

			m_pArticleMainDlg ->AddChannel(guidPrivateChannelNode, pItemData->guidRes);
			pItemData->bGetData = FALSE;
			Invalidate();
	// 		return ;
		}
	}
	else if(m_eListViewType == TP_LISTVIEW_PRIVATECHANNEL)
	{
		CRect rtItem, rtClose;
		GetItemRect(nItem,&rtItem,LVIR_BOUNDS);	
		rtClose			= rtItem;
		rtClose.left    = rtClose.right - 40;
		rtClose.bottom  = rtClose.top + 30;
		if(rtClose.left < point.x && rtClose.right > point.x && rtClose.top < point.y && rtClose.bottom > point.y)
		{
			TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(nItem);
			if(pItemData == NULL) return ;
			GetItemResData(pItemData);

			if(!m_pArticleMainDlg ->AddChannel(guidPrivateChannelNode, pItemData->guidRes))
			{
				CTPListCtrlEx::DeleteItem(nItem);
			}
	// 		return ;
		}
	}
	CTPListCtrlEx::OnLButtonDown(nFlags,point);
}


LRESULT  CTPArticleListCtrl::OnMsgRefreshItem(WPARAM wp,LPARAM lp)
{
	//ASSERT(lp);
	TPResData *pResData = (TPResData*)lp;
	for(INT i = CTPListCtrlEx::GetItemCount() - 1; i >= 0; i--)
	{
		TPListItemData* pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(i);
		if(NULL != pItemData && pItemData->guidRes == pResData->guidRes)
		{
			pItemData->bGetData = FALSE;

			m_nUpdateCount ++;
			UpdateLoadStatus();

			Invalidate();
			break;
		}
	}
	delete pResData;
	pResData = NULL;
	return S_OK;
}

UINT CTPArticleListCtrl::TP_GetArticleItemThread(LPVOID pVoid)
{
	CTPArticleListCtrl	*pArticleList = (CTPArticleListCtrl *)pVoid;
	while(pArticleList->m_bOpenThread)
	{
		TPResDataArray aNeedGetData;
		EnterCriticalSection(&pArticleList->m_csTeamCmd);
		aNeedGetData.Copy(pArticleList->m_aNeedGetData);
		pArticleList->m_aNeedGetData.RemoveAll();
		LeaveCriticalSection(&pArticleList->m_csTeamCmd);

		if(aNeedGetData.GetSize() <=0) break;

		for (int l = 0 ; l < aNeedGetData.GetSize(); l++)
		{
			pArticleList->GetArticleData(aNeedGetData[l].guidRes);
			TPResData *pResData = new TPResData;
			pResData->guidRes = aNeedGetData[l].guidRes;
			pArticleList->PostMessage(WM_REFRESHITEM, NULL,(LPARAM)pResData);
		}
	}
	::SetEvent(pArticleList->m_hOpenThread);

	return 1;
}

BOOL CTPArticleListCtrl::GetArticleByThread(TPResDataArray	&aNeedGetData)
{
	EnterCriticalSection(&m_csTeamCmd);
	m_aNeedGetData.Append(aNeedGetData);
	LeaveCriticalSection(&m_csTeamCmd);

// 	if(m_bOpenThread == FALSE)
	{
		m_bOpenThread =  TRUE;
		::ResetEvent(m_hOpenThread);
		m_pRunThread = AfxBeginThread((AFX_THREADPROC )TP_GetArticleItemThread,this);	
	} 

	return TRUE;
}

int CTPArticleListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPListCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	UpdateLoadStatus(TRUE);
	// TODO:  Add your specialized creation code here
	return 0;
}
void  CTPArticleListCtrl::UpdateLoadStatus(BOOL bPlus, BOOL bReset)
{
	if(bReset)
	{
		m_nUpdateCount    = 0;
		m_nUpdateCountAll = 0;
	}
	int nTotal = CTPListCtrlEx::GetItemCount();
	if(m_nUpdateCount > m_nUpdateCountAll)	
		m_nUpdateCount = m_nUpdateCountAll;
	if(m_nUpdateCount < 0)
		m_nUpdateCount = 0;

	CString sStatue = _T("");
	if(bPlus)
	{
		sStatue.Format(_T("更新状态：%d / %d"), m_nUpdateCount, m_nUpdateCountAll);
	}
	CWnd *pWnd = m_pArticleMainDlg->GetDlgItem(IDC_STATIC_LOADSTATUS);
	pWnd->SetWindowText(sStatue);

	return ;
}
