// TPArticleListCtrl.cpp : implementation file
//

#include "stdafx.h"

// CTPArticleListCtrl

IMPLEMENT_DYNAMIC(CTPArticleListCtrl, CTPListCtrlEx)

CTPArticleListCtrl::CTPArticleListCtrl()
{

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
		if(pItemData/* && (pItemData ->dwState & TP_RESSTATE_HIDE)*/) return;
		GetItemResData(pItemData);
	}	
}
void  CTPArticleListCtrl::GetItemResData(TPListItemData *pItemData,BOOL bGetData)
{
	if(pItemData == NULL)    return;
	if(pItemData ->bGetData/* && !(pItemData ->bGetData  & TP_RESUSEFLAG_RELEASE)*/) return;
	pItemData ->bGetData = TRUE;
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
			TCHAR *cItemText = NULL;
			TPChannelItem *pItemInfo = NULL;
			CTPArticleParser stuArticleParser;
			stuArticleParser.SetChannelItem(stuChannel.aChannelItemAll[l], stuChannel.cKeyDiv);
			stuArticleParser.GetItemInfo(cItemText);
			ASSERT(TP_StrLen(cItemText) > 50);

			CoCreateGuid(&stuArticle.guidRes);
			stuArticle.guidNode = stuChannel.guidRes;
			stuArticle.stuChannelItem = *stuChannel.aChannelItemAll[l];
			TP_StrCpy(stuArticle.cText, cItemText, TP_StrLen(cItemText));
			g_stuArticleInterface.stuArticleInterfce.TP_SetArticleInfo(stuArticle.guidRes,TP_GRADE_ALL,stuArticle);
			stuChannel.aChannelItemAll[l]->guidItem = stuArticle.guidRes;
		}
		aArticleList.Add(stuArticle.guidRes);
		
		InsertItem(iIndex,stuArticle.stuChannelItem.cItemTitle);//CTPListCtrlEx::InsertItem(iResNeed,_L("         "));
		TPListItemData *pItemData = new TPListItemData();
	//	pItemData ->SetResBaseData(hChildRes[l]);
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
