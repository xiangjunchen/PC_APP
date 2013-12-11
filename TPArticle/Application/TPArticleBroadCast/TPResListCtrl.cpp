// TPResListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include ".\TPResListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTPGetIcon     *g_pGetIconFun = NULL; 
#define TP_EVENT_DROPPINGITEM			1000 + 500
CString sTitle[M1][M2] =
{
	{_T("IDS_CLIPEXPLORER_NAME"),
	//{liubo add CD2009.10.01 2010-6-24 Material ID
#ifdef TP_NS_NET
		_T("IDS_CLIPEXPLORER_SONAPS_MATERIALID"),
#endif
	//}
		_T("IDS_CLIPEXPLORER_SEARCHCLIPTYPE"),	_T("IDS_CLIPEXPLORER_EDLREELNAME"),	_T("IDS_CLIPEXPLORER_DIGITIZE"),  _T("IDS_CLIPEXPLORER_CLIPCOLOR"), _T("IDS_CLIPEXPLORER_USED"),		   _T("IDS_CLIPEXPLORER_CODEC"),	    _T("IDS_CLIPEXPLORER_MARKIN"),
		_T("IDS_CLIPEXPLORER_MARKOUT"),	_T("IDS_CLIPEXPLORER_DURATION"),     _T("IDS_CLIPEXPLORER_CLIPLENGTH"),  _T("IDS_CLIPEXPLORER_FTCSTART"),    _T("IDS_CLIPEXPLORER_START"),   _T("IDS_CLIPEXPLORER_END"),       
		_T("IDS_CLIPEXPLORER_LTCIN"),   _T("IDS_CLIPEXPLORER_LTCOUT"),  //Not Support LTC
		_T("IDS_CLIPEXPLORER_VITCIN"),   _T("IDS_CLIPEXPLORER_VITCOUT"),_T("IDS_CLIPEXPLORER_VTRINFO"),_T("IDS_CLIPEXPLORER_SCENETAKE"),_T("IDS_CLIPEXPLORER_STATUS"),
		_T("IDS_CLIPEXPLORER_TRACKS"),	_T("IDS_CLIPEXPLORER_COMMENT"),		_T("IDS_CLIPEXPLORER_RECDATE"), _T("IDS_CLIPEXPLORER_CONFIGTIME"), 
		//{liubo modify CD2009.10.01 2010-9-9 
#ifdef TP_NS_NET
		_T("IDS_CLIPEXPLORER_TIMEMODIFY"),
#else
		_T("IDS_CLIPEXPLORER_ALTERTIME"),	
#endif
		//}
		_T("IDS_CLIPEXPLORER_SUBCLIPS"),   _T("IDS_CLIPEXPLORER_PROXY"),		_T("IDS_CLIPEXPLORER_CLIPID"),
		_T("IDS_CLIPEXPLORER_RIGHTS"),		_T("IDS_CLIPEXPLORER_SOURCENAME"),	_T("IDS_CLIPEXPLORER_MFLVIDEO"),		_T("IDS_CLIPEXPLORER_MFLAUDIO"),	_T("IDS_CLIPEXPLORER_FRAMESIZE"),_T("IDS_CLIPEXPLORER_FRAMEASPECTRATIO"),_T(""),},
	{_T("IDS_CLIPEXPLORER_SEARCHUSERNAME"),_T("IDS_CLIPEXPLORER_COMMENT"),      _T(""),             _T("")}, //add by qsj 20100104 PE直接修改comment
	{_T("IDS_CLIPEXPLORER_NAME"),        _T("IDS_CLIPEXPLORER_PROPERTYUSETYPE"),         _T("IDS_CLIPEXPLORER_CATEGORY"),     _T("IDS_CLIPEXPLORER_ORIGINAL"),     _T("IDS_CLIPEXPLORER_DURATION"),    _T("IDS_CLIPEXPLORER_ALIGNMENT")}, 	
	{_T("IDS_CLIPEXPLORER_NAME"),        _T("IDS_CLIPEXPLORER_CLIPCOLOR")},             
	{_T("IDS_CLIPEXPLORER_NAME"),       _T("IDS_CLIPEXPLORER_ORILOCATION"),_T("IDS_CLIPEXPLORER_DATEDEL"),_T("IDS_CLIPEXPLORER_DELBY"),},       
#ifdef TP_NS_NET
	{_T("IDS_CLIPEXPLORER_NAME"),       _T("IDS_CLIPEXPLORER_COMMENT"),          /*_T("Create Date")*/_T("IDS_CLIPEXPLORER_CONFIGTIME"), /*_T("Update Date")*/ _T("IDS_CLIPEXPLORER_TIMEMODIFY"),_T("IDS_CLIPEXPLORER_DURATION"),},       
	{_T("IDS_CLIPEXPLORER_NAME"),       _T("IDS_CLIPEXPLORER_COMMENT"),          /*_T("Create Date")*/_T("IDS_CLIPEXPLORER_CONFIGTIME"), /*_T("Update Date")*/_T("IDS_CLIPEXPLORER_TIMEMODIFY"),},
	{_T("IDS_CLIPEXPLORER_NAME"),       _T("IDS_CLIPEXPLORER_TIMEMODIFY"),	   _T("IDS_CLIPEXPLORER_VIDEOFORMAT"),		 _T("IDS_CLIPEXPLORER_FRAMEASPECTRATIO"),_T("IDS_CLIPEXPLORER_STATUS"),_T("IDS_CLIPEXPLORER_CLIPCOLOR")},
#else
	{_T("IDS_CLIPEXPLORER_NAME"),       _T("IDS_CLIPEXPLORER_COMMENT"),          _T("IDS_CLIPEXPLORER_CONFIGTIME"), _T("IDS_CLIPEXPLORER_ALTERTIME"),_T("IDS_CLIPEXPLORER_DURATION"),},       
	{_T("IDS_CLIPEXPLORER_NAME"),       _T("IDS_CLIPEXPLORER_COMMENT"),          _T("IDS_CLIPEXPLORER_CONFIGTIME"), _T("IDS_CLIPEXPLORER_ALTERTIME"),},       	
	{_T("IDS_CLIPEXPLORER_NAME"),       _T("IDS_CLIPEXPLORER_MODIFYTIME"),	   _T("IDS_CLIPEXPLORER_VIDEOFORMAT"),		 _T("IDS_CLIPEXPLORER_FRAMEASPECTRATIO"),_T("IDS_CLIPEXPLORER_STATUS"),_T("IDS_CLIPEXPLORER_CLIPCOLOR")},
#endif
	{_T("IDS_CLIPEXPLORER_NAME")},
};


void TP_LoadListCtrlStr()
{
	for(INT l=0;l<M1;l++)
	{
		for(INT k=0;k<M2;k++)
		{
			sTitle[l][k] = SBLoadString(sTitle[l][k]);
		}
	}
}

int  iColumnMax [M2] = {33,2,6,2,4,5,4,6,1,1};//changed by qsj 20100104 增加用户管理，数组最后加1
int  iColumnWidth[M1][M2] = 
{
	{300,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,150,250,100,150,150,100,100,100,100,100,100,100,100,100,100},
	{200,100},//增加comment 初始值加宽XN00027538 qsj
	{100,100,100,100,100,100},
	{100,100},
	{100,100,100,100},
	{100,100,100,100,100},
	{100,100,100,100},
	{100,100,100,100, 100,100},
	{100,},
};
BOOL  iColumnView[M1][M2] = 
{
	{TRUE,TRUE,TRUE,TRUE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE,TRUE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE},
	{TRUE,TRUE},//增加comment
	{TRUE,TRUE,TRUE,TRUE,TRUE,TRUE},
	{TRUE,TRUE},
	{TRUE,TRUE,TRUE,TRUE},
	{TRUE,TRUE,TRUE,TRUE,TRUE},
	{TRUE,TRUE,TRUE,TRUE},
	{TRUE,TRUE,TRUE,TRUE, TRUE,TRUE},
	{TRUE,},
};
int  iColumnOrder[M1][M2] = 
{
	{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33},
	{0,1},//增加comment 
	{0,1,2,3,4,5,6},
	{0,1},
	{0,1,2,3,4},
	{0,1,2,3,4},
	{0,1,2,3},
	{0,1,2,3,4,5},
	{0,},
};

int  TP_NodeTypeHeadIndex(TP_NODE_TYPE eNodeType)
{
	int iHeadIndex = 0;
	if((eNodeType & TP_NODE_TRASH))										  iHeadIndex = TP_HEADINDEX_TRASH;
	else if(eNodeType & TP_NODE_PROJECT)								  iHeadIndex = TP_HEADINDEX_PROJECT;
	else if(eNodeType & (TP_NODE_BIN | TP_NODE_SEARCH))                   iHeadIndex = TP_HEADINDEX_BIN;
	else if(eNodeType & TP_NODE_NETWORK)								  iHeadIndex = TP_HEADINDEX_NEWWORK;	
	else																  iHeadIndex = TP_HEADINDEX_PROJECT;

	return iHeadIndex;
}



int _tmcmpEx(SYSTEMTIME *psrc,SYSTEMTIME *pdes)
{
	__int64 iSrc  = ((((__int64)psrc ->wYear * 400 + psrc ->wMonth *31 + psrc ->wDay )*24 + psrc ->wHour) * 3600 + psrc ->wMinute *60 + psrc ->wSecond) *1000; 
	__int64 iDesc = ((((__int64)pdes ->wYear * 400 + pdes ->wMonth *31 + pdes ->wDay )*24 + pdes ->wHour) * 3600 + pdes ->wMinute *60 + pdes ->wSecond) *1000; 
	if(iSrc >iDesc)        return 1;
	else if(iSrc == iDesc) return 0;
	else return -1;
}
int CALLBACK TP_ResCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{	
	__int64 iReturn = 1;
	CTPResListCtrl *pList = (CTPResListCtrl *)lParamSort;
	if(pList == NULL) return -1;
	if(pList->GetColumnCount() <= 0) return 0;
	if(NULL == lParam1 || NULL == lParam2) return 0;
	TPListItemData  *pItemData1 = (TPListItemData *)lParam1;
	TPListItemData  *pItemData2 = (TPListItemData *)lParam2;

	pList->GetItemResData(pItemData1);
	pList->GetItemResData(pItemData2);

	if(pList->m_iSortIndex < 0) return 0;

	BOOL bSortAsc = pList ->m_bSortAsc;
	INT nIndex = pList->GetItemOrder(pList->m_iSortIndex);
	if(pItemData1->eNodeType & TP_NODE_TRASH) return bSortAsc ? -1 : 1;
	if(pItemData2->eNodeType & TP_NODE_TRASH) return bSortAsc ? 1 : -1;

	if((pItemData1->eResType & TP_RES_CATALOG) && !(pItemData2->eResType & TP_RES_CATALOG)) return bSortAsc ? -1 : 1;
	if(!(pItemData1->eResType & TP_RES_CATALOG) && (pItemData2->eResType & TP_RES_CATALOG)) return bSortAsc ? 1 : -1;

	BOOL bNeedSort = TRUE;
	INT nHeadIndex = TP_NodeTypeHeadIndex(pList->m_eNodeType);
	if(TP_HEADINDEX_BIN == nHeadIndex)
	{
		if(TP_RESTEXT_CREATE_DATE == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = _tmcmpEx(&pItemData1->tmCreateTime, &pItemData2->tmCreateTime);
		}
		else if(TP_RESTEXT_UPDATE_DATE == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = _tmcmpEx(&pItemData1->tmModifyTime, &pItemData2->tmModifyTime);
		}
		else if(TP_RESTEXT_RECDATE == nIndex)
		{
			bNeedSort = FALSE;
			if((pItemData1->eSingSourceType & TP_CLIP_SOURCE_TYPE_DIGEQUIPMENT)) 
				iReturn = 1;
			else if((pItemData2->eSingSourceType & TP_CLIP_SOURCE_TYPE_DIGEQUIPMENT))
				iReturn = -1;
			else iReturn = 0;
		}
		else if(TP_RESTEXT_CLIPCOLOR == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = (INT)pItemData1->cResColor - (INT)pItemData2->cResColor;
			if(iReturn != 0 &&  pItemData1->cResColor == COLOR_BASE_RES_DEF)        iReturn = 1;
			else if(iReturn != 0 && pItemData2->cResColor == COLOR_BASE_RES_DEF)	iReturn = -1;
		}
		else if(TP_RESTEXT_CLIP_ID == nIndex || TP_RESTEXT_SOURCE_NAME == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = lstrcmp(pItemData1->sText[nIndex], pItemData2->sText[nIndex]);
		}

	}
	else if (TP_HEADINDEX_CGTEMPLAGE == nHeadIndex)
	{
		if (TP_TEMPALTECGTEXT_CONFIGURATIONTIME == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = _tmcmpEx(&pItemData1->tmCreateTime, &pItemData2->tmCreateTime);
		}
		else if(TP_TEMPALTECGTEXT_ALTERATIONTIME == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = _tmcmpEx(&pItemData1->tmModifyTime, &pItemData2->tmModifyTime);
		}
	}
	else if(TP_HEADINDEX_PROJECT == nHeadIndex)
	{
		if(TP_CATALOG_CLIPCOLOR == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = (INT)pItemData1->cResColor - (INT)pItemData2->cResColor;
			if(iReturn != 0 &&  pItemData1->cResColor == COLOR_BASE_RES_DEF)        iReturn = 1;
			else if(iReturn != 0 && pItemData2->cResColor == COLOR_BASE_RES_DEF)	iReturn = -1;
		}
	}
	else if(TP_HEADINDEX_COMPUTER == nHeadIndex)
	{
	    if(TP_PROJECT_MODIFYTIME == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = _tmcmpEx(&pItemData1->tmModifyTime, &pItemData2->tmModifyTime);
		}
		else if(TP_PROJECT_CLIPCOLOR == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = (INT)pItemData1->cResColor - (INT)pItemData2->cResColor;
			if(iReturn != 0 &&  pItemData1->cResColor == COLOR_BASE_RES_DEF)        iReturn = 1;
			else if(iReturn != 0 && pItemData2->cResColor == COLOR_BASE_RES_DEF)	iReturn = -1;
		}
	}
	else if(TP_HEADINDEX_TRASH == nHeadIndex)
	{
		if(TP_TRASH_DELETEDATA == nIndex)
		{
			bNeedSort = FALSE;
			iReturn = _tmcmpEx(&pItemData1->tmCreateTime, &pItemData2->tmCreateTime);
		}
	}

    //按windows排序规则，2在12前
	if(bNeedSort)	iReturn = StrCmpLogicalW(pItemData1 ->sText[nIndex] ,pItemData2 ->sText[nIndex]);

	INT nSortIndexEx = pList->m_iSortIndexEx;
	if(0 == iReturn && nSortIndexEx >= 0 && nIndex != nSortIndexEx)
	{
		iReturn = StrCmpLogicalW(pItemData1 ->sText[nSortIndexEx] ,pItemData2 ->sText[nSortIndexEx]);
		return  iReturn;
	}
	return int(pList ->m_bSortAsc ? iReturn : -iReturn);
}
extern void TP_ResetClipExporerDroppingItem();
IMPLEMENT_DYNAMIC(CTPResListCtrl, CTPListCtrlEx)
CTPResListCtrl::CTPResListCtrl()
{
	//基本类的属性
	m_uPrivateStyle    =  LIST_SIZECOLUMN |  LIST_CLICKCOLUMN |LIST_SELMORE|LIST_HOTHEAD ;
	m_uStyleEx         =  LVS_EX_GRIDLINES;
	m_iViewType        =  VIEWTYPE_TEXTPICTURE;

	m_iColumnHeight    = g_iIconHeight/2 - 10;
	m_szItemList       = CSize(200,g_iIconHeight/2+2);
	m_szItemIcon       = CSize(g_iIconWidth + 4,g_iIconHeight + 28);
	m_szItemReport     = CSize(200,g_iIconHeight/2+2);	
	m_szItemInfo       = CSize(200,g_iIconHeight + 2);
	m_szItemBuffer     = CSize(720/2 + 4,576/2 + 28);

	m_szEdge           = CSize(10,10);
	m_szSpace          = CSize(7,7);
	m_ptMouseMove	   = CPoint(0, 0);
	m_bTrackLeave      = FALSE;
	m_iBackColor       = COLOR_BASE_LIST;
	m_hCacheData       = NULL;
	m_hCacheData       = NULL;
	m_pResTreeCtrl     = NULL;
	m_pResBrowserCtrl  = NULL;
	m_hCatalog         = NULL;
	m_hSaveData		   = NULL;
	m_bDragIn         = FALSE;

	m_eDragResType    = TP_RES_UNKNOW;
	m_iMenuItem       = -1;

	m_iSortIndex      = 0;
	m_lIconSize       = 4;
	m_bSortAsc        = FALSE;
	m_eShowNodeType      = 0;

	m_eNodeType       = 0x7fffffff;

	m_bForList        = FALSE;
	m_pMsgWnd		  = NULL;
	m_uState          = 0;	
	m_bCanEdit        = TRUE;
	m_pUserEnterInfo  = NULL;
	m_guidCatalog     = GUID_NULL;
	m_guidDBType      = GUID_NULL;
	m_guidEditRes	  = GUID_NULL;
	m_dwSortType      = 0;
	m_hDropCatalog    = NULL;
	m_iTopIndex       = 0;
	m_uListState	  = 0;
	CString sSkinPath = TP_GetSkinPath();

	for(INT l=0;l<3;l++) m_hDragIcon[l]   = NULL;
	for(INT l=0;l<4;l++) m_hDragTLIcon[l] = NULL;	
	m_hDragCursor[0] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\CursorArrow.cur"));
	m_hDragCursor[1] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorok.cur"));
	m_hDragCursor[2] = TP_LoadCursor(sSkinPath + _L("\\CommonCtrl\\Cursor\\Cursor2\\cursorno.cur"));

	m_hDragTLCursor[0] = TP_LoadCursor(sSkinPath + _L("\\TimeLineWnd\\Cursor\\Composer.cur"));
	m_hDragTLCursor[1] = TP_LoadCursor(sSkinPath + _L("\\TimeLineWnd\\Cursor\\SegmentES.cur"));
	m_hDragTLCursor[2] = TP_LoadCursor(sSkinPath + _L("\\TimeLineWnd\\Cursor\\SegmentLO.cur"));
	m_hDragTLCursor[3] = NULL;//TP_LoadCursor(sSkinPath + _L("\\TimeLineWnd\\Cursor\\RubBand.cur"));

	m_pDragImageList    = NULL;
	if(sTitle[0][0] == _L("Name"))
		for(INT l=0;l<M1;l++)	for(INT k=0;k<M2;k++)	sTitle[l][k] = TP_LoadStr(sTitle[l][k]);		

	for(INT i = 0; i < TP_LISTTIMER_MAX; i++)
		m_uListTimer[i]  = 0;

	//g_pGetIconFun  = TP_GetIconFun();
	m_iSortIndexEx = 0;
}
 

CTPResListCtrl::~CTPResListCtrl() 
{	
	//TP_RemoveResDataArray(m_hDragData,TRUE);

	for(INT l=0;l<3;l++)
	{
		if(m_hDragCursor[l] != NULL)   TP_ReleaseCommonRes(m_hDragCursor[l]);
	}
	for(INT l=0;l<4;l++)
	{
		if(m_hDragTLCursor[l] != NULL)   TP_ReleaseCommonRes(m_hDragTLCursor[l]);
	}
	for(INT l=0;l<3;l++) 
	{
		if(m_hDragIcon[l]) ::DestroyIcon(m_hDragIcon[l]);
	}
	for(INT l=0;l<4;l++) 
	{
		if(m_hDragTLIcon[l]) ::DestroyIcon(m_hDragTLIcon[l]);
	}
	if(m_pDragImageList)
	{
		m_pDragImageList ->DeleteImageList();
		delete m_pDragImageList;
		m_pDragImageList = NULL;
	}
	m_aCatalogViewType.RemoveAll();
}

BEGIN_MESSAGE_MAP(CTPResListCtrl, CTPListCtrlEx)
	ON_WM_LBUTTONDBLCLK()

	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnLvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
//	ON_COMMAND_RANGE(IDC_SEARCH,ID_SYSCMD_MAX,OnMenuCmd)
	ON_NOTIFY_RANGE(HDN_ITEMCLICK, 0,0x7fffffff, OnHdnItemclick)
//	ON_MESSAGE(WM_UPDATEVIEW,OnMsgUpdateView)
	ON_MESSAGE(WM_LIST_DROPFILE,OnMsgDropFile)

	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff,       OnToolTipNotify )
	ON_WM_DESTROY()
//	ON_WM_SIZE()
//	ON_WM_KEYDOWN()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CAPTURECHANGED()	   
	ON_WM_TIMER()
END_MESSAGE_MAP()



void  CTPResListCtrl::SelectItemGuid(CGuidArray &aItem)
{
	int iCount = GetItemCount();
	for(INT l=0;l<iCount;l++)
	{
		TPListItemData  *pItemData = (TPListItemData *)GetItemData(l);
		if(pItemData)
		{
			for(INT k= (INT)aItem.GetSize()-1;k>=0;k--)
			{
				if(aItem[k] == pItemData->guidRes)
				{
					CTPListCtrlEx::SelectItem(l,TRUE,FALSE);
					aItem.RemoveAt(k);
					break;
				}
			}
		}
	}
	Invalidate();
}

void  CTPResListCtrl::SortItem(int iIndex,BOOL bAsc)
{
	if(iIndex >= 0)
	{
		NMHEADER phdr;
		LRESULT  hResult;
		phdr.iItem = iIndex;
        m_bSortAsc = !bAsc;
		OnHdnItemclick(0,(NMHDR *)&phdr,&hResult);
	}
	else
	{
		CTPListCtrlEx::SortItems(TP_ResCompareProc,(DWORD)(DWORD_PTR)this);	
	}
}
void  CTPResListCtrl::SetResData(TPResData& stuShow, TPResDataArray &hChildRes)
{
	
}

//nSubItem表示列移动后界面显示列，返回列移动前初始列字符串 by qushaojie 20091023
CString    CTPResListCtrl::GetItemText(int nItem, int nSubItem) const
{
	TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(nItem);
	if(pItemData == NULL) return _T("");
	CTPListCtrlEx *pListCtrl = (CTPListCtrlEx*)this;
	INT	 iSubItem = pListCtrl->GetItemOrder(nSubItem);
    if(iSubItem < 0) return _T("");
	return pItemData ->sText[iSubItem]; 
}
void       CTPResListCtrl::BeginEdit()
{
	if (VIEWTYPE_ICON == m_iViewType)
	{
		m_iItemSubLast = GetItemByOrder(0);
		if(m_iItemSubLast <0 )return;
	}
	CTPListCtrlEx::BeginEdit();
	// li song mimg 2010-01-15 Name栏不允许输入特殊字符，而Comment栏可以输入特殊字符。
	BOOL  bSetToolTip = TRUE;
	int   iSelectItem = 0;
	int   iSubItem = 0;
	CWnd *pWnd = NULL;
	CTPListCtrlEx::GetEditItem(iSelectItem,iSubItem,pWnd);
	INT nEditSub = GetItemOrder(iSubItem);
	if(nEditSub <0) return;
	int  iHeadIndex = TP_NodeTypeHeadIndex(m_eShowNodeType);
	TPListItemData   *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSelectItem);

	if(m_pEdit && bSetToolTip) 
	{
		CString sInvalidChar = WC_NAMEEXCLUDE;
		CString sKeyNoteTip  = _T("Invalid Char ") + sInvalidChar;
		((CTPEditEx *)m_pEdit) ->SetExcludeKey(sInvalidChar);
		((CTPEditEx *)m_pEdit) ->SetToolTips(sKeyNoteTip);	
	}
}

void   CTPResListCtrl::GetItemResData(TPListItemData *pItemData,BOOL bGetData)
{

}

BOOL CTPResListCtrl::ExpandItem(int item)
{
	if(item >=0)
	{
		TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(item);
		if(pItemData)
		{
			if((pItemData ->eResType & (TP_RES_CATALOG)) && pItemData ->hResData && m_pResTreeCtrl)
			{				
				m_pResTreeCtrl->SelectCatalog(pItemData->hResData);
			}
		}
	}
	m_bLeftDrag = FALSE;
	return TRUE;
}

/**/
void CTPResListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int iDbClickItem = CTPListCtrlEx::HitTest(point);	
	if(!m_bCanEdit)
	{
		TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iDbClickItem);
		if(pItemData)
		{
			if(pItemData ->eResType == TP_RES_CATALOG && pItemData ->hResData && m_pResTreeCtrl)
			{
				ExpandItem(iDbClickItem);
			}
		}
		return;
	}
	if(iDbClickItem >=0 && (!m_bForList || m_bForList == 4))
	{
		m_uState |= STATE_DBCLICK;
		CTPResListCtrl::EndEdit();		
		TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iDbClickItem);
		if(pItemData)
		{
			if((pItemData ->eResType & (TP_RES_CATALOG)) && pItemData ->hResData && m_pResTreeCtrl)
			{
				ExpandItem(iDbClickItem);
			}
		}
	}
	else
	{
		CTPListCtrlEx::OnLButtonDblClk(nFlags, point);
	}
}

int CTPResListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPListCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CTPListCtrlEx::SetFont(TP_GetSysFont());   
	m_iBackColor      = COLOR_BASE_LIST;

	return 0;
}

BOOL CTPResListCtrl::CreateDragCursor(CPoint point)
{
	for(INT l=0;l<3;l++) if(m_hDragIcon[l]) ::DestroyIcon(m_hDragIcon[l]);
	m_hDragIcon[0] = CopyCursor(m_hDragCursor[0] ->GetCursor());
	m_hDragIcon[1] = CopyCursor(m_hDragCursor[1] ->GetCursor());
	m_hDragIcon[2] = CopyCursor(m_hDragCursor[2] ->GetCursor());	
	for(INT l=0;l<4;l++) if(m_hDragTLIcon[l]) ::DestroyIcon(m_hDragTLIcon[l]);
	m_hDragTLIcon[0] = CopyCursor(m_hDragTLCursor[0] ->GetCursor());
	m_hDragTLIcon[1] = CopyCursor(m_hDragTLCursor[1] ->GetCursor());
	m_hDragTLIcon[2] = CopyCursor(m_hDragTLCursor[2] ->GetCursor());	
	//m_hDragTLIcon[3] = CopyCursor(m_hDragTLCursor[3] ->GetCursor());	

	if(m_pDragImageList)
	{
		m_pDragImageList ->DeleteImageList();
		delete m_pDragImageList;
		m_pDragImageList = NULL;
	}
	m_pDragImageList = CreateDragImage(-1,&point);
	return TRUE;
}
LRESULT CTPResListCtrl::OnMsgUpdateView(WPARAM wParam ,LPARAM lParam)
{
	if(lParam == NULL) return S_FALSE;
	 
	TPResUpdateView *pData = (TPResUpdateView *)lParam;
	UpdateView(pData->guidDBType, pData ->eResType,pData->eNodeType,&pData->aGuidList);
	pData ->aGuidList.RemoveAll();
	delete pData;
	return S_OK;
}

void CTPResListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV    = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if(!m_bForList)
	{	
		CPoint ptTest;
		GetCursorPos(&ptTest);
		ScreenToClient(&ptTest);

		PreviewGif(-1);
		if(m_eDragResType & TP_RES_RBUTTONDRAG) return;
		int             nItem     = -1;
		TPListItemData *pItemData = NULL;
		TP_RES_TYPE     resType   = TP_RES_UNKNOW;   
		BOOL            bCatalog  = FALSE; 

		TP_RemoveResDataArray(m_hDragData,TRUE);
		POSITION posGet = GetFirstSelectedItemPosition(TRUE);
		while (posGet)
		{
			TPResData      stuResData;
			nItem = GetNextSelectedItem(posGet);
			TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(nItem);
			if(pItemData)
			{
				GetItemResData(pItemData);			 
				if(pItemData ->eResType & (TP_RES_CATALOG|TP_RES_PROJECT))
				{
					TPCatalogData stuCatalog;
					g_pClipManageInterface ->stuCatalogInterface.TP_GetCatalogInfo(pItemData->hResData,&stuCatalog);

					TP_RES_TYPE resTypeTemp = TP_NodeTypeToResType(stuCatalog.eNodeType );
					if(resType == TP_RES_UNKNOW) resType = resTypeTemp;						

					bCatalog = TRUE;
				}
				else 
				{
					resType = pItemData ->eResType;		
				}

				pItemData->GetResBaseData(stuResData);
				stuResData.guidParam = m_guidCatalog;//纪录回收站的目录guid;
				stuResData.lParam    = NULL;
				stuResData.hResCache = m_hCacheData;
				stuResData.hResData  = TP_DuplicateRes(stuResData.hResData, stuResData.eResType, TP_RESLOCK_VIEW|TP_RESLOCK_NOUPDATE);
				stuResData.eNodeType = m_eNodeType;

				m_hDragData.Add(stuResData);
			}
		}
		if(bCatalog) 	resType = TP_RES_CATALOG;
		if(m_hDragData.GetSize() <= 0) return;
		m_bDragIn		= TRUE;
		m_eDragResType  = resType;
		if(!m_bLeftDrag) m_eDragResType |= TP_RES_RBUTTONDRAG;
		int iHitTest = CTPListCtrlEx::HitTest(ptTest);
		nItem = iHitTest>=0 ? iHitTest : nItem;
		CreateDragCursor(ptTest);
		::SetCursor(m_hDragIcon[0]);	
		m_uState &= ~STATE_DROPOUT;
#ifdef TP_DRAG_IMAGLIST
		if(m_pDragImageList)
		{
			CPoint ptDrag = ptTest;
			ClientToScreen(&ptDrag);
			ptTest.Offset(-m_szTemp.cx-8,-m_szTemp.cy-8);
			m_pDragImageList ->BeginDrag(0,ptTest);			
			m_pDragImageList ->DragEnter(NULL,ptDrag);
		}
#endif
	}
}

void CTPResListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{	 
	if(m_eDragResType != TP_RES_UNKNOW)
	{
		m_ptDragPos = point;
		if(0 == m_uListTimer[TP_LISTTIMER_DROPPING]) m_uListTimer[TP_LISTTIMER_DROPPING] = SetTimer(TP_EVENT_DROPPINGITEM, 50, NULL);
		CPoint ptScreen = point;
		ClientToScreen(&ptScreen);
		if(m_rtClient.PtInRect(point) && WindowFromPoint(ptScreen) == this)//search)		
		{	
			m_bDragIn = TRUE;
			{
				if(TP_IsCtrlKeyDown())
				{
					LRESULT hResult = g_stuClipExpolrerCallBack.TP_DragItemDropping(m_eDragResType,(LPARAM)&m_hDragData);
					//{2010年09月01日: Zhang ShunHong:Add BugID: 0x00002259
					if((hResult == S_OK || hResult == S_ACCEPT) /*&& TP_GetResRight(NULL, TP_RES_UNKNOW, TP_RIGHT_COPY)*/)
						SetCursor(m_hDragIcon[1]);
					else
						SetCursor(m_hDragIcon[2]);
					if(m_uState & STATE_DROPOUT)
					{
						g_stuClipExpolrerCallBack.TP_DragItemDropping(m_eDragResType,(LPARAM)NULL);
						m_uState &= ~STATE_DROPOUT;
					}
				}
				else
				{
					int iItem = CTPListCtrlEx::HitTest(point);					
					if(iItem >=0)
					{		
						TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iItem);
						if(pItemData)//XN00024840 changed by qsj 20100105 如果拖动到不是文件夹的目录，禁止拖动
						{
							if (pItemData ->eResType & TP_RES_CATALOG)
							{
								LRESULT hResult = g_stuClipExpolrerCallBack.TP_DragItemDropping(m_eDragResType,(LPARAM)&m_hDragData);
								if(hResult == S_DECLINE)				      SetCursor(m_hDragIcon[2]);
								else if(hResult == S_ACCEPT)     		      SetCursor(m_hDragIcon[1]);
								else if(hResult == S_OK || hResult ==S_FALSE) SetCursor(m_hDragIcon[0]);
							}
							else 
							{
								SetCursor(m_hDragIcon[0]);
							}
						}
					}
					else
					{
						if(m_uState & STATE_DROPOUT)
						{
							g_stuClipExpolrerCallBack.TP_DragItemDropping(m_eDragResType,(LPARAM)NULL);
							m_uState &= ~STATE_DROPOUT;
						}
						SetCursor(m_hDragIcon[0]);			
					}
				}						
			}
		}
		else
		{
			m_bDragIn = FALSE;
			LRESULT hResult = S_FALSE;
			if(g_stuClipExpolrerCallBack.TP_DragItemDropping)
			{
				m_uState |= STATE_DROPOUT;
				hResult = g_stuClipExpolrerCallBack.TP_DragItemDropping(m_eDragResType,(LPARAM)&m_hDragData);
				if(hResult == S_DECLINE|| hResult == S_FALSE) 
				{
					::SetCursor(m_hDragIcon[2]);
				}
				else
				{
					//DWORD dwIconState = g_stuClipExpolrerCallBack.TP_TimeLineCallBack(TP_TIMELINE_GETICONSTATE,0);					
					//if(dwIconState != TP_TLDONE_UNKNOW)
					//{

					//	if(dwIconState == TP_TLDONE_COMPOSER )                                       ::SetCursor(m_hDragTLIcon[0]);			
					//	else if(dwIconState == TP_TLDONE_SEGMENTEXTRACE)                             ::SetCursor(m_hDragTLIcon[1]);			
					//	else if(dwIconState == TP_TLDONE_SEGMENTLIFTWRITE)                           ::SetCursor(m_hDragTLIcon[2]);	
					//	//else if(dwIconState == TP_TLDONE_RUBBERBAND)                                 ::SetCursor(m_hDragTLIcon[3]);	
					//	else if(dwIconState == TP_TLDONE_ADVANCECCR)                                 ::SetCursor(m_hDragCursor[1]->GetCursor());									
					//}
					//else
					{
						if(hResult == S_ACCEPT) 				  ::SetCursor(m_hDragIcon[1]);
						else if(hResult == S_OK )			      ::SetCursor(m_hDragIcon[0]);			
					}
				}
			}
		}
#ifdef TP_DRAG_IMAGLIST
		if(m_pDragImageList && m_rtClient.PtInRect(point))
		{
			CPoint ptScreen = point;			
			ClientToScreen(&ptScreen);
			if(m_bDragInWnd == FALSE)			
				m_pDragImageList ->DragEnter(NULL,ptScreen);			
			m_pDragImageList ->DragMove(ptScreen);
			m_bDragInWnd = TRUE;
		}		
		else if(m_bDragInWnd == TRUE)
		{
			m_pDragImageList ->DragLeave(NULL);
			m_bDragInWnd = FALSE;
			//m_bReDraw = TRUE;
			//InvalidateRect(NULL);
		}
#endif
	}

	else
	{
		// 鼠标第一次移入窗口时， 请求一个WM_MOUSELEAVE消息 ,以便于离开时响应WM_MOUSELEAVE消息
		if (!m_bTrackLeave) 
		{
			TRACKMOUSEEVENT t = {sizeof(TRACKMOUSEEVENT),TME_LEAVE,m_hWnd,0};
			_TrackMouseEvent(&t);
			m_bTrackLeave = TRUE;
		}
// 		if(TP_WND_UNKONWEXPLORER == m_dwWndType)
// 		{
// 			if(m_ptMouseMove != point)
// 			{
// 				INT64 nNodeEffect = TP_NODE_VEFFECT|TP_NODE_SYSTEM;
// 				if((m_eNodeType & (TP_NODE_SYSTEMPLATETR|TP_NODE_SYSTEMPLATECLIP)) || (m_eNodeType & nNodeEffect) == nNodeEffect) // 目前只是过度特技，和系统过渡模板显示gif
// 					m_uListTimer[TP_LISTTIMER_PREVIEWGIF] = SetTimer(TP_ENENT_PREGIF, 300, NULL);
// 				m_ptMouseMove = point;
// 			}
// 		}
	}
	CTPListCtrlEx::OnMouseMove(nFlags, point);
}

void CTPResListCtrl::ReleaseDragImage()
{
	if(m_pDragImageList)
	{		
		m_pDragImageList ->DragLeave(NULL);
		m_pDragImageList ->EndDrag();
		m_pDragImageList ->DeleteImageList();		
		delete m_pDragImageList;
		m_pDragImageList = NULL;
	}
}
void CTPResListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_eDragResType != TP_RES_UNKNOW)
	{
		TP_ResetClipExporerDroppingItem();
		
		CPoint ptScreen = point;
		ClientToScreen(&ptScreen);
		TP_RES_TYPE eDragRes = m_eDragResType;
		m_eDragResType       = TP_RES_UNKNOW;	

        ReleaseDragImage();
		//在本窗口拖动		
		if(m_rtClient.PtInRect(point) && WindowFromPoint(ptScreen) == this)		
		{	
			BOOL bDoDefault = TRUE;
			if(TP_IsCtrlKeyDown() || eDragRes &TP_RES_RBUTTONDRAG)     //Drag by rightButton or Ctrl button
			{
				bDoDefault = FALSE;
			}
			else
			{
				int iItem = CTPListCtrlEx::HitTest(point);
				if(iItem >=0)
				{
					TPListItemData  *pListData = (TPListItemData *)GetItemData(iItem);
					if(pListData && pListData->eResType == TP_RES_CATALOG) 
					{
						bDoDefault = FALSE;
					}
				}
			}	

			::SetCursor(m_hCursorDrag[0] ->GetCursor());
			ReleaseCapture();

			UINT uPrivateStyle =  m_uPrivateStyle;
			if(bDoDefault)	m_uPrivateStyle   |=  LIST_DRAGITEM;
			// DQAdb00006648 LI HUI LI 2009-04-30 Dropping完之后需要 Drop清g_pDropEditWnd, 因为随着Array的增长，g_pDropEditWnd指向的地址可能被释放了
			
			if(g_stuClipExpolrerCallBack.TP_DragItemDrop)
				g_stuClipExpolrerCallBack.TP_DragItemDrop(eDragRes,!bDoDefault ?(LPARAM)&m_hDragData:(LPARAM)NULL);								

			TP_RemoveResDataArray(m_hDragData,TRUE);
			CTPListCtrlEx::OnLButtonUp(nFlags, point);
			m_uPrivateStyle    = uPrivateStyle;
			m_iDragMode        = TP_DRAG_UNKNOW;
			m_eDragResType     = TP_RES_UNKNOW;	

			if(g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogSortType(m_hCatalog,NULL) & TP_BY_AUTO)
			{
				m_uState &=  ~STATE_DRAGITEM;
				CTPListCtrlEx::FunCalcItemSize();
				CTPListCtrlEx::Invalidate();
			}
			return;
		}
		else 
		{
			CTPListCtrlEx::OnLButtonUp(nFlags, point);
			if(g_stuClipExpolrerCallBack.TP_DragItemDrop &&  eDragRes!=0)
			{
				m_uListState |= TP_LISTSTATE_NODROP;
				if(g_stuClipExpolrerCallBack.TP_DragItemDrop(eDragRes,(LPARAM)&m_hDragData) == S_OK)
				{
					m_eDragResType     = TP_RES_UNKNOW;	
					m_iDragMode        = TP_DRAG_UNKNOW;
					m_uListState      &= ~TP_LISTSTATE_NODROP; 
					TP_RemoveResDataArray(m_hDragData,TRUE);
					return;
				}
				TP_RemoveResDataArray(m_hDragData,TRUE);
				m_uListState      &= ~TP_LISTSTATE_NODROP; 
			}
			
			//TP_SetRecentClip(m_hDragData);
			m_eDragResType     = TP_RES_UNKNOW;	
			m_iDragMode        = TP_DRAG_UNKNOW;

			if(g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogSortType(m_hCatalog,NULL) & TP_BY_AUTO)
			{
				m_uState &=  ~STATE_DRAGITEM;
				CTPListCtrlEx::FunCalcItemSize();
				CTPListCtrlEx::Invalidate();
			}
			return;
		}
	}	

	if(g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogSortType(m_hCatalog,NULL) & TP_BY_AUTO)
	{
		CTPListCtrlEx::FunCalcItemSize();
		CTPListCtrlEx::Invalidate();
	}
	CTPListCtrlEx::OnLButtonUp(nFlags, point);
}


void CTPResListCtrl::UpdateView(GUID guidDBType,TP_RES_TYPE eResType,TP_NODE_TYPE eNodeType,CGuidArray *hResDataList)
{		
	ASSERT(0);	 
}


void CTPResListCtrl::OnRButtonDown(UINT nFlags, CPoint setpoint)
{
}
void  CTPResListCtrl::OnMenuCmd(UINT nID, LPARAM lParam /*= 0*/)
{
	if(!m_bCanEdit) return;
//	switch(nID)
//	{	
//	case ID_VIEWTYPE_SELALL:
//		{
//			CTPListCtrlEx::SetRedraw(FALSE);
//			int iCount = CTPListCtrlEx::GetItemCount();
//			for(int l=0;l<iCount;l++)
//			{	
//				if(!IsViewItem(l))continue;
//				m_aItem[l]->dwSelTick = GetTickCount() + l;	
//				SelectItem(l, TRUE, FALSE);
//			}
//			CTPListCtrlEx::SetRedraw(TRUE);
//			CTPListCtrlEx::Invalidate();
//		}
//		break;
//	case ID_VIEWTYPE_INVERTSEL:
//		{
//			CTPListCtrlEx::SetRedraw(FALSE);
//			int iCount = CTPListCtrlEx::GetItemCount();
//			for(int l=0;l<iCount;l++)
//			{
//				if(!IsViewItem(l))continue;
//				UINT nState = 	CTPListCtrlEx::GetItemState(l,LVIS_SELECTED);
//				if(nState & LVIS_SELECTED)
//				{
//					SelectItem(l, FALSE, FALSE);
//				}
//				else	
//				{
//					m_aItem[l]->dwSelTick = GetTickCount() + l;	
//					SelectItem(l, TRUE, FALSE);
//				}
//			}			
//			CTPListCtrlEx::SetRedraw(TRUE);			
//		}
//		break;
//
//	case IDC_LISTCOPY:
//	case IDC_CATALOG_COPY:
//		{				
//			if (!TP_GetResRight(NULL, TP_RES_UNKNOW, TP_RIGHT_COPY))break;
//			TPResDataArray  aSelData;
//			int             iSel = -1;
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(NULL == pItemData) continue;
//				GetItemResData(pItemData);
//
//				if(!g_bEnableALLMenu &&(((m_eNodeType & TP_NODE_SYSTEM) || pItemData->dwOperateFlag & TP_OPERATEFLAG_SYSTEM) && !(pItemData ->eResType & TP_RES_ALLTEMPLATE))) continue;
//				if(pItemData->eNodeType & TP_NODE_RECYCLE)  continue; // 回收站内的资源不能复制
//				if(m_eNodeType & TP_NODE_SEARCH)   continue; // Search内的资源不能复制  
//				if(m_eNodeType & TP_NODE_USERINFO) continue; // 用户不能复制
//			//	if (pItemData->dwCaptureFlag & TP_CLIPCAPTURE_UPLOADING) continue; // 采集中素材不允许剪切
//
//				TPResData  stuData;
//				stuData.eResType      = pItemData ->eResType;
//#ifdef TP_NS_NET	
//				if (pItemData->eNodeType & TP_NODE_PROJECTITEM)	stuData.eResType	|= TP_RES_PROJECT;	
//#endif
//				stuData.guidRes       = pItemData ->guidRes;	
//				stuData.guidDBType    = pItemData ->guidDBType;	
//				stuData.hResCache     = m_hCacheData;
//				stuData.guidNode      = pItemData ->guidNode;
//				stuData.hResData      = pItemData->hResData;                    
//				stuData.dwOperateFlag = pItemData->dwOperateFlag;
//				stuData.eNodeType	  = pItemData->eNodeType;
//
//				aSelData.Add(stuData);					
//			}	
//
//			if(aSelData.GetSize() >0) g_pClipManageInterface ->stuFuncitonInterface.TP_CopyResData(aSelData);
//			aSelData.RemoveAll();
//		}
//		break;	
//	case IDC_CATALOG_CUT:
//	case IDC_LISTCUT:
//		{			
//			BOOL            bShift  = TP_IsShiftKeyDown();					
//			TPResDataArray  aSelData;
//			CGuidArray aGuidGrpFolder;
//			HRESULT         lResult = S_FALSE;
//			POSITION        posSel  = CTPListCtrlEx::GetFirstSelectedItemPosition(FALSE);
//			while (posSel)
//			{
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData == NULL) continue;	
//				GetItemResData(pItemData);
//#ifdef TP_NS_NET
//				//{liubo add CD2009.10.01 2010-7-6 Download editing
//				if (pItemData->guidDBType != g_pClipManageInterface->TP_GetCurDBType())
//				{
//					return;
//				}
//				//}
//
//				// 采集中素材不允许剪切
//				if ( pItemData->dwCaptureFlag & TP_CLIPCAPTURE_UPLOADING )
//				{
//					continue;
//				}
//#endif
//
//
//				if(pItemData->eNodeType & TP_NODE_RECYCLE) continue;
//				if(m_eNodeType & TP_NODE_SEARCH)   continue; // Search内的资源不能剪切
//				//Wu_E700025885
//				if((m_eNodeType & TP_NODE_SYSTEM) || pItemData->dwOperateFlag & TP_OPERATEFLAG_SYSTEM) continue;
//
//				TPResData  stuData;
//				stuData.eResType         = pItemData ->eResType;
//				stuData.guidRes          = pItemData ->guidRes;
//				stuData.guidDBType       = pItemData ->guidDBType;	
//				stuData.guidNode         = pItemData->guidNode;
//				stuData.hResCache        = m_hCacheData;
//				stuData.hResData         = pItemData->hResData;
//				stuData.dwOperateFlag    = pItemData->dwOperateFlag;
//#ifdef TP_NS_NET
//				stuData.eNodeType		 = pItemData->eNodeType;
//#endif
//				if((stuData.eResType & TP_RES_CATALOG) && (pItemData ->eNodeType & (TP_NODE_COMPUTER | TP_NODE_PROJECTITEM)))
//					aGuidGrpFolder.Add(stuData.guidRes);
//				aSelData.Add(stuData);
//			}	
//#ifdef TP_NS_NET
//			for (int cnt=0; cnt<aSelData.GetSize(); cnt++)
//			{
//				TPResData stuSubResData = aSelData.GetAt(cnt);
//				if (!g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(stuSubResData.hResData, stuSubResData.eResType, TP_RIGHT_EDIT))
//					return;
//			}
//#endif
//			//The project folder is not allowed deleted when it includes opened project
//			BOOL bHasOpenedPrj = FALSE;
//			HRESDATA hCatlog = g_pClipManageInterface ->TP_GetCurProject();
//			HRESDATA hParentCat;
//			TPCatalogData stuCatData;
//			do
//			{
//				if(hCatlog == NULL)  break;
//				g_pClipManageInterface->stuCatalogInterface.TP_GetParentCatalog(hCatlog,hParentCat);
//				g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(hParentCat,&stuCatData);
//				for(INT i = 0; i < aGuidGrpFolder.GetSize(); i++)
//				{
//					if(stuCatData.guidRes == aGuidGrpFolder[i])
//					{
//						bHasOpenedPrj = TRUE;  break;
//					}
//				}
//				if(bHasOpenedPrj)  break;
//
//				hCatlog = hParentCat;
//			}while(stuCatData.eNodeType != (TP_NODE_TABLE |TP_NODE_COMPUTER|TP_NODE_RES));
//			if(bHasOpenedPrj) {aSelData.RemoveAll(); return; }
//
//			if(aSelData.GetSize()>0) 
//			{
//				if(g_pClipManageInterface->stuFuncitonInterface.TP_CutResData(aSelData)!=S_OK)
//				{				
//					CString sErrorInfo = g_pClipManageInterface ->TP_GetLastErrorInfo();
//					if(sErrorInfo.GetLength()>0) MessageBox(sErrorInfo ,SBLoadString(_T("IDS_CLIPEXPLORER_CUTNOTE")),MB_OK);
//					g_pClipManageInterface->TP_ClearErrorInfo();
//				}
//			}
//			aSelData.RemoveAll();		
//
//		}
//		break;
//	case IDC_CATALOG_DELETE:
//	case IDC_LISTDEL:	
//		{			
//		/*	if(g_pSystemInfo->uSystemState & TP_SYSTEMSTATE_LOGING) 
//			{
//				AfxMessageBox(TP_LoadStr(_T("IDS_TIMELINEWND_LOGINING")));	
//				return ;
//			}	*/
//
//			DWORD           dwOperateFlag    = 0;
//			TPResDataArray  aSelData;int  iSel;
//			TPResLockView   stuAutoStop; 
//
////			TP_TransMessage(WM_UPDATEVIEW); // 清除所有消息， 包括UpateView, 在弹出对话框时，会处理消息，导致素材句柄不正确
//
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			BOOL bHaveProject = FALSE;
//			while (posSel)
//			{
//				iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData)
//				{	
//					GetItemResData(pItemData);
//					// li song ming 2008-7-7 XN00012809 将Trash排除在删除范围之外
//					if((m_eNodeType & (TP_NODE_SYSTEM|TP_NODE_MASKRPREFABRICATE)) || pItemData->dwOperateFlag & TP_OPERATEFLAG_SYSTEM) continue;
//					//]]]]]]
//					TPResData  stuData;
//					stuData.eNodeType   = pItemData ->eNodeType;
//					if(pItemData->eNodeType & TP_NODE_PROJECT) stuData.eResType  = TP_NodeTypeToResType(pItemData ->eNodeType);
//					else									   stuData.eResType  = pItemData ->eResType;
//
//					stuData.guidRes     = pItemData ->guidRes;	
//					stuData.guidDBType  = pItemData ->guidDBType;	
//					stuData.hResCache   = m_hCacheData;
//					stuData.guidNode    = pItemData ->guidNode;
//					stuData.hResData    = pItemData->hResData;
//					dwOperateFlag      |= pItemData ->dwOperateFlag;
//					stuData.lParam      = (LPARAM)pItemData->dwBookIndex;
//					aSelData.Add(stuData);
//					
//				}
//			}			
//				if(aSelData.GetSize()<=0) return;
//#ifdef TP_NS_NET
//
//			extern BOOL TP_ResDeleteTestRight(TPResDataArray &aResData);
//			if(!TP_ResDeleteTestRight(aSelData)) //V4.5Feature CD2009.10.01
//			{
//				AfxMessageBox(SBLoadString(_T("EDITOR_IMPROVEMENT_NO_DELETE_RIGHT")));
//				return;			
//			}
//#endif
//
//			CString sTempInfo; 
//			if(aSelData.GetSize()> 1)		 sTempInfo = SBLoadString(_T("IDS_CLIPEXPLORER_LISTAREYOUSUREDELETES"));
//			else							 sTempInfo = SBLoadString(_T("IDS_CLIPEXPLORER_LISTAREYOUSUREDELETE"));
//#ifndef TP_NS_NET
//			if(IDYES != AfxMessageBox(sTempInfo, MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2))		return ;
//#endif
//			if((m_eNodeType & TP_NODE_SYSTEM) || dwOperateFlag & TP_OPERATEFLAG_SYSTEM) return;
//			
//			// li hui li 2009-09-16 **因为弹出对话框时，可能会刷新，导致句柄不正确，所以重新获取一下数据
//			aSelData.RemoveAll();
//			posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData)
//				{	
//					if((m_eNodeType & TP_NODE_SYSTEM) || pItemData->dwOperateFlag & TP_OPERATEFLAG_SYSTEM) continue;
//					TPResData  stuData;
//					stuData.eNodeType   = pItemData ->eNodeType;
//					if(pItemData->eNodeType & TP_NODE_PROJECT) stuData.eResType  = TP_NodeTypeToResType(pItemData ->eNodeType);
//					else									   stuData.eResType  = pItemData ->eResType;
//
//					stuData.guidRes     = pItemData ->guidRes;	
//					stuData.guidDBType  = pItemData ->guidDBType;	
//					stuData.hResCache   = m_hCacheData;
//					stuData.guidNode    = pItemData ->guidNode;
//					stuData.hResData    = pItemData->hResData;
//					dwOperateFlag      |= pItemData ->dwOperateFlag;
//					stuData.lParam      = (LPARAM)pItemData->dwBookIndex;
//					aSelData.Add(stuData);
//
//				}
//			}		
//			ASSERT(aSelData.GetSize() > 0);
//			/************                      ****************/
//			if(aSelData.GetSize()<=0) return;
//#ifdef	TP_NS_NET
//			CString sTemp1;
//			sTemp1.Format(SBLoadString(aSelData.GetCount()>1?_T("IDS_CLIPEXPLORER_LISTDELETEITEMS"):_T("IDS_CLIPEXPLORER_LISTDELETEITEMS")), aSelData.GetCount());
//			if(IDYES != ::MessageBox(NULL, sTemp1, SBLoadString(_T("IDS_CLIPEXPLORER_TEXT_DELCONFIRM")), MB_YESNO | MB_DEFBUTTON2))
//				return ;
//#endif
//			BOOL  bToRecycle = TRUE;
//			CString sTemp;
//
//			if(m_eNodeType & TP_NODE_RECYCLE)
//			{
//                sTemp.Format(SBLoadString(_T("IDS_CLIPEXPLORER_LISTDELSELITEMS")));
//				UINT uID = MessageBox(sTemp,SBLoadString(_T("IDS_CLIPEXPLORER_DELMSG")),MB_YESNOCANCEL);	
//				if(uID == IDCANCEL || uID == IDNO)  return;
//				else if(uID == IDYES ) bToRecycle = FALSE;
//				else  return;
//				TP_SaveCurrentTL(TRUE);
//				g_pClipManageInterface->stuFuncitonInterface.TP_RemoveRecycleRes(aSelData);
//				return;
//			}
//
//#ifdef TP_NS_NET
//			BOOL  bPrompt = FALSE;
//			if ( m_eNodeType & TP_NODE_COMPUTER || m_eNodeType & TP_NODE_PROJECTFLODER )
//			{
//				bPrompt = TRUE;
//			}
//
//			if ( bPrompt || TP_IsShiftKeyDown() )
//#else
//#ifdef TP_MINI_EDIT
//			if(1)
//#else
//			if(TP_IsShiftKeyDown())
//#endif 
//			{
//				sTemp.Format(SBLoadString(_T("IDS_CLIPEXPLORER_LISTDELSELITEMS")));
//				UINT uID =  MessageBox(sTemp,SBLoadString(_T("IDS_CLIPEXPLORER_DELMSG")),MB_YESNO|MB_DEFBUTTON2);
//				if(uID == IDCANCEL || uID == IDNO)  return;
//				else if(uID == IDYES ) bToRecycle = FALSE;
//				else return;
//			}
//			else
//			{
//				bToRecycle = TRUE;
//			}
//#endif	
//
//			if(TP_NODE_SEARCH & m_eNodeType)
//			{
//				INT iSize = aSelData.GetSize();
//				for(INT i = iSize - 1; i >= 0; i--)
//				{
//					GUID	   guidDBType  = GUID_NULL;
//					GUID       guidCatalog = GUID_NULL;
//					HRESDATA   hResData    = NULL;
//					TPResData  stuResData  = aSelData[i];
//					guidDBType = stuResData.guidDBType;
//					guidCatalog = stuResData.guidNode;
//					TPCatalogData stuCatalog;
//					
//					if(TP_RES_NOTE & stuResData.eResType)
//					{
//						TPClipData stuClipData;
//						g_pClipManageInterface->stuClipInterfce.TP_GetNoteInfo(stuResData.hResData, TP_GRADE_BASE, stuClipData);
//						guidCatalog = stuClipData.guidNode;
//					}
//					else if(TP_RES_ALLPROGRAM & stuResData.eResType)
//					{
//						TPProgramData stuProgramData;
//						g_pClipManageInterface->stuProgramInterface.TP_GetProgramInfo(stuResData.hResData, TP_GRADE_BASE, stuProgramData);
//						guidCatalog = stuProgramData.guidNode;
//
//					}
//					else if((TP_RES_CATALOG | TP_RES_PROJECT) & stuResData.eResType)
//					{
//						TPCatalogData stuCatalogData;
//						g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(stuResData.hResData, &stuCatalogData);
//						guidCatalog = stuCatalogData.guidNode;
//					}
//					else if((TP_RES_ALLTEMPLATE & stuResData.eResType))
//					{
//						TPTemplateData stuTemplate;
//						g_pClipManageInterface->stuTemplateInterface.TP_GetTemplateInfo(stuResData.hResData, TP_GRADE_BASE, stuTemplate);
//						guidCatalog = stuTemplate.guidNode;
//					}
//					else
//					{
//						aSelData.RemoveAt(i);
//						continue;
//					}
//					if(S_FALSE == g_pClipManageInterface->stuCatalogInterface.TP_GetCatalog(guidDBType, GUID_NULL, guidCatalog, hResData) || NULL == hResData)
//					{
//						// 获取失败则删除
//						aSelData.RemoveAt(i);
//						continue;
//					}
//					do
//					{
//						BOOL bFound = FALSE;
//						for(INT j = aSelData.GetSize() - 1; j >= 0; j--)
//						{
//							if(guidCatalog == aSelData[j].guidRes)
//							{
//								aSelData.RemoveAt(i);
//								bFound = TRUE;
//								break;
//							}
//						}
//						if(bFound) break;
//
//						HRESDATA hParent = NULL;
//						if(S_FALSE == g_pClipManageInterface->stuCatalogInterface.TP_GetParentCatalog(hResData, hParent) || NULL == hParent)
//						{
//							break;
//						}
//						
//						if(S_FALSE == g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(hParent, &stuCatalog))
//						{
//							aSelData.RemoveAt(i);
//							break;
//						}
//						hResData = hParent;
//						guidCatalog = stuCatalog.guidRes;
//					} while((NULL != hResData) && (stuCatalog.eNodeType != (TP_NODE_TABLE |TP_NODE_COMPUTER|TP_NODE_RES)));
//				}
//			}
//			void *pDelData[5];
//			TP_GetDeleteParam(pDelData, bToRecycle, FALSE);	 
//			LRESULT lRet = S_FALSE;
//#ifdef TP_NS_NET
//			extern LRESULT TP_DeleteResForNet(GUID guidDBType,TPResDataArray* paResData,void* pDelData[]);  ///2009-11-6 modified by Guo Rui : S2_NLE : CD2009.10.01
//			 lRet = TP_DeleteResForNet(m_guidDBType, &aSelData, pDelData);
//			 if(lRet != S_OK)
//			 {
//				 CString strError = g_pClipManageInterface->TP_GetLastErrorInfo();
//				 if(strError.GetLength() > 0) AfxMessageBox(strError);
//				 g_pClipManageInterface->TP_ClearErrorInfo();			
//			 }
//#else
//			TP_SaveCurrentTL(TRUE);
//			lRet = g_pClipManageInterface->stuFuncitonInterface.TP_DeleteResData(m_guidDBType,&aSelData,pDelData);
//#endif
//			aSelData.RemoveAll();
//				
//
//		}
//		break;
//	case IDC_LISTPASTE:
//	case IDC_CATALOG_PASTE:
//		{
//			/*if(g_pSystemInfo->uSystemState & TP_SYSTEMSTATE_LOGING) 
//			{
//				AfxMessageBox(TP_LoadStr(_T("IDS_TIMELINEWND_LOGINING")));	
//				return ;
//			}	*/
//
//			if((m_eNodeType & (TP_NODE_SYSCOMMANDBUTTON|TP_NODE_COMMANDBUTTON3)) == (TP_NODE_SYSCOMMANDBUTTON|TP_NODE_COMMANDBUTTON3))
//			{
//				AfxMessageBox(SBLoadString(_T("IDS_CLIPEXPLORER_TREEONLYUSERSCANCHANGE")));
//				return;
//			}
//			// XN00012389 LI HUI LI 2008-09-01  //FX Explorer，Proxy editor need same function with Craftor
//#ifndef TP_NS_NET
//			if((m_eNodeType & TP_NODE_EFECT) && TP_IsProxyEditVersion()) return;
//#endif
//			if(TP_NODE_TRASH & m_eNodeType) return; // Do not allow paste in Trash // LI HUI LI 2007-12-01
//
//			TP_NODE_TYPE eNodeType = m_eNodeType;
//			HRESDATA hCatalog = m_hCatalog;
//			if(0 != lParam) // 保存粘贴目录
//			{
//				hCatalog = (HRESDATA)lParam;
//				TPCatalogData stuCatalog;
//				g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(hCatalog, &stuCatalog);
//				eNodeType = stuCatalog.eNodeType;
//			}
//			m_iMenuItem = -1;
//
//			if(S_ACCEPT != g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(hCatalog, NULL, FALSE)) return;
//			if((eNodeType & (TP_NODE_USERCOMMANDBUTTON | TP_NODE_COMMANDBUTTON3)) == (TP_NODE_USERCOMMANDBUTTON | TP_NODE_COMMANDBUTTON3))
//			{
//				LRESULT LRetVal = TP_DeleteTemplateResFromUserCommandButton(hCatalog);
//				if(LRetVal != S_OK)  return;			
//			}
//			//POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//
//			if(m_hCatalog)
//			{
//				TPAutoBeep stuBeep(BeepNoSound,BeepNoSound);//20091028beep
//				if(g_pClipManageInterface->stuFuncitonInterface.TP_PasteResData(hCatalog)!=S_OK)
//				{
//					CString sErrorInfo = g_pClipManageInterface ->TP_GetLastErrorInfo();
//#ifdef TP_NS_NET //CD2009.10.01 V4.5Feature
//					if(sErrorInfo.GetLength()>0) MessageBox(sErrorInfo ,SBLoadString(_T("PUBLIC_LIMIT_WARNING")),MB_OK|MB_ICONWARNING);
//#else
//					if(sErrorInfo.GetLength()>0) MessageBox(sErrorInfo ,SBLoadString(_T("IDS_CLIPEXPLORER_PASTETIP")),MB_OK);
//#endif
//					g_pClipManageInterface->TP_ClearErrorInfo();
//				}
//				else
//				{
//					TPCatalogData stuCatalog;
//					TPResDataArray aReturn;
//					m_aGuidReturn.RemoveAll();
//					g_pClipManageInterface->stuFuncitonInterface.TP_GetReturnRes(aReturn, TRUE);
//					g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(hCatalog, &stuCatalog);
//					for(INT l=0;l<aReturn.GetSize();l++)
//					{
//						if(aReturn[l].guidNode == stuCatalog.guidRes)
//							m_aGuidReturn.Add(aReturn[l].guidRes);
//					}
//					CTPListCtrlEx::SelectItem(-1, TRUE, TRUE);
//#ifdef TP_NS_NET
//					//{liubo modify 2010-10-13 snp4100028323
//					if (IsKindOf(RUNTIME_CLASS(CTPNSResListCtrl)))
//					{
//						((CTPNSResListCtrl*)this)->m_aGuidSel.RemoveAll();
//						((CTPNSResListCtrl*)this)->m_aGuidSel.Append(m_aGuidReturn);
//					}
//					SelectItemGuid(m_aGuidReturn);
//					//}
//#endif
//				}
//				return ;
//			}
//		}
//		break;	
//	case IDC_CLIP_DOWNLOADTOBRD:
//		{
//			CStringArray    aError;
//			TPResGuidArray  aGuidRes;
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//
//				if(pItemData && (pItemData ->eResType & TP_RES_NOTE))
//				{
//					TPClipData  stuClipData;
//					g_pClipManageInterface->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData,TP_GRADE_FILE|TP_GRADE_BASE,stuClipData);					
//					aGuidRes.Add(TPResGuid(pItemData->guidDBType,pItemData->eResType,pItemData->guidRes));
//				}
//			}
//			if(aError.GetSize()>0)	TP_MessageShow(aError);	aError.RemoveAll();
//
//
//			TPBrdDownLoadClipArray aClip;
//			g_stuBrdManage.TP_GetDownLoadClipFromRes(aGuidRes,aClip);			
//			if(g_stuBrdExplorer.TP_GdiCallBack)
//				g_stuBrdExplorer.TP_GdiCallBack(TP_BRD_DWONLOADCLIP,(LPARAM)&aClip);
//		}
//		break;	
//	case IDC_CLIP_EDIT:		
//	case IDC_CLIP_NEWEDIT:
//		{
//			if(m_iMenuItem>=0)
//			{
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//				if(pItemData && (pItemData ->eResType == TP_RES_NOTE ||pItemData ->eResType == TP_RES_ALLCGTEMPLATE)  && g_stuClipExpolrerCallBack.TP_SendResToClipDisplay)
//				{
//					if(g_stuClipExpolrerCallBack.TP_TimeLineCallBack(TP_TIMELINE_GETEDITRESSTATE,NULL) == S_OK) return;///8613 
//					TPResDataArray aResData;
//					TPResData resData;
//					resData.eResType = pItemData ->eResType;
//					resData.guidRes  = pItemData ->guidRes;
//					resData.hResData = pItemData ->hResData;
//
//					aResData.Add(resData);
//					g_stuClipExpolrerCallBack.TP_SendResToClipDisplay(aResData);
//					TP_SetRecentClip(aResData);
//					aResData.RemoveAll();
//
//				}
//			}
//		}
//		break;
//	case IDC_CLIP_DIRTOTRACK:		
//	case IDC_CLIP_THREETRACK:		
//	case IDC_CLIP_FOURTRACK:
//	case IDC_CLIP_BETWENMARK:
//	case IDC_CLIP_FILLINOUT:
//		{		
//			//if(TP_IsSystemLoggingOperator()) return;
//			TPResDataArray  aSelData;	
//
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				if(iSel>=0)
//				{				
//					TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//					if(NULL == pItemData) continue;
//					if(pItemData && (pItemData ->eResType & (TP_RES_NOTE | TP_RES_ALLPROGRAM|TP_RES_ALLCGTEMPLATE)))
//					{
//						TPResData  stuData;
//						stuData.eResType   = pItemData ->eResType;
//						stuData.guidRes    = pItemData ->guidRes;
//						stuData.hResData   = pItemData ->hResData;
//						stuData.guidDBType = pItemData ->guidDBType;
//						aSelData.Add(stuData);						
//					}					
//				}
//			}
//			if(aSelData.GetSize()<=0)  return;
//			TP_StopCurDataState();
//			BOOL bFixPos = FALSE;
//			if(IDC_CLIP_DIRTOTRACK == nID) bFixPos = TRUE;
//			BOOL bSplicIn = TRUE;
//			TPManageData* pManageData = (TPManageData*)TP_GetCurManageData();
//			if(NULL != pManageData && NULL != pManageData ->hEditData)
//			{
//				if(g_pTimeLineDataEdit->TP_TLData_GetEditType(pManageData->hEditData) == TP_TLEDIT_TV) bSplicIn = FALSE;
//			}
//			LRESULT lResult = S_OK;
//			if(g_stuClipExpolrerCallBack.TP_SendResToTimeLineNS)
//				lResult = g_stuClipExpolrerCallBack.TP_SendResToTimeLineNS(bSplicIn, TP_ADDCLIP_CURPOS, aSelData, bFixPos);
//
//			if(S_OK != lResult)
//			{
//				CString sError = g_pTimeLineDataEdit->TP_TLData_GetLastError(NULL);
//				if(!sError.IsEmpty()) AfxMessageBox(sError);
//			}
//			TP_SetRecentClip(aSelData);
//		}
//		break;	
//	case IDC_PROGRAM_RECAPTURECLIP:
//		{		
//			TPResDataArray  aSelData;		
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData ->eResType & TP_RES_ALLPROGRAM)
//				{
//					TPResData  stuData;
//					stuData.eResType = pItemData ->eResType;
//					stuData.guidRes  = pItemData ->guidRes;
//					stuData.hResData = pItemData ->hResData;
//					aSelData.Add(stuData);
//				}						
//			}
//			if(aSelData.GetSize()<=0)  return;			
//		}
//		break;	
//	 
//	case IDC_TEMPLATE_EDIT:
//		{
//#ifndef TP_NS_NET
//			if(TP_IsProxyEditVersion()) return;
//#endif
//			if(m_iMenuItem>=0)
//			{
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//				if(pItemData && (pItemData ->eResType & (TP_RES_ALLVATEMPLATE | TP_RES_ALLCGTEMPLATE)))
//				{
//					CGuidArray aGuid;
//					aGuid.Add(pItemData ->guidRes);
//					// LI HUI LI 2007-11-07 因为TP_NODE_COMMANDBUTTON3 与 TP_NODE_CLIPTEMPLATE相同,具有 TP_NODE_SYSCOMMANDBUTTON属性的不可以编辑 XN00006244
//					if((m_eNodeType & (TP_NODE_PUBLICTEMPLATE | TP_NODE_USERTEMPLATE) || (m_eNodeType & (TP_NODE_COMMANDBUTTON3 | TP_NODE_USERCOMMANDBUTTON)) == (TP_NODE_COMMANDBUTTON3 | TP_NODE_USERCOMMANDBUTTON)|| g_bEnableALLMenu) 
//						&&  g_stuClipExpolrerCallBack.TP_VATemplate_Open) 	
//						g_stuClipExpolrerCallBack.TP_VATemplate_Open(m_hCacheData,pItemData ->eResType,pItemData ->guidDBType,&aGuid);					
//					else if((m_eNodeType & TP_NODE_ALLCGTEMPLATE) &&   g_stuClipExpolrerCallBack.TP_CGTemplate_Open)
//						g_stuClipExpolrerCallBack.TP_CGTemplate_Open(m_hCacheData,pItemData ->eResType,pItemData ->guidDBType, &aGuid);			
//				}
//			}				
//		}
//		break 	;
//	case IDC_TEMPLATE_READ:
//		if(m_iMenuItem>=0)
//		{
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(pItemData && (pItemData ->eResType & TP_RES_CGTEMPLATE) && g_stuClipExpolrerCallBack.TP_SendResToClipDisplay)
//			{
//				GetItemResData(pItemData);
//				if(g_pClipManageInterface ->TP_GetResLockCount(pItemData ->hResData,(TP_RESLOCK_FLAG)(TP_RESLOCK_WRITE|TP_RESLOCK_ALL))>0)
//				{
//					CString sError = pItemData ->sText[0];					
//					sError += _L("\n");
//					sError += SBLoadString(_T("IDS_CLIPEXPLORER_HASOPENE"));
//					MessageBox(sError,SBLoadString(_T("IDS_CLIPEXPLORER_MENUOPENPROGRAM")),MB_OK);
//					return;
//				}
//				TPResDataArray aResData;
//				TPResData resData;
//				resData.eResType = TP_RES_CGTEMPLATE;
//				resData.guidRes  = pItemData ->guidRes;
//				resData.hResData = pItemData ->hResData;
//				aResData.Add(resData);
//				g_stuClipExpolrerCallBack.TP_SendResToClipDisplay(aResData);
//				aResData.RemoveAll();								
//			}
//		}
//		break;	
//	case ID_UPLOAD_HIGH:
//	case ID_UPLOAD_LOW:
//		{
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(NULL != pItemData) GetItemResData(pItemData);
//			if(pItemData && (pItemData ->uResSource & TP_RESSOURCE_BRD))
//			{ 
//				TPBrdClipImportArray aClipImp;			
//				TPBrdClipImportArray aEdlImp;		
//				TPBrdClipImportArray aPgmImp;		
//				int                  iSel = -1;
//
//				DWORD            dwAudio     = g_stuBrdManage.TP_GetCurDeviceLinkAudio();
//				TPBrdSysSetData *pBrdSysData = NULL;			
//				g_stuBrdManage.TP_GetBrdSysSetData(pBrdSysData);
//				g_stuBrdManage.TP_InitCache();
//
//				TPBrdClipImport    stuImport;	
//
//				stuImport.bImpClip         = TRUE;
//				if(pBrdSysData)
//				{
//					stuImport.guidResClipPath  = pBrdSysData->guidResClipPath;
//					stuImport.guidResPgmPath   = pBrdSysData->guidResPgmPath;
//					stuImport.guidResCGPgmPath = pBrdSysData->guidResCGPgmPath;
//					stuImport.sVideoPath       = pBrdSysData->sVideoPath;
//					stuImport.sAudioPath       = pBrdSysData->sAudioPath;								
//				}
//				stuImport.aQuality[0].bImp       = nID ==ID_UPLOAD_HIGH;
//				stuImport.aQuality[1].bImp       = nID ==ID_UPLOAD_LOW;
//				stuImport.iAudioChannle          = 8;
//
//				POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//				while (posSel)
//				{
//					iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//					TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//					if(pItemData && (pItemData ->eResType & TP_RES_NOTE))
//					{		
//						GetItemResData(pItemData);
//						TPClipData           stuClipData; 
//						g_pClipManageInterface ->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData,TP_GRADE_BASE|TP_GRADE_SOURCE,stuClipData);
//						stuImport.eResType      = TP_BRD_CLIP;
//						stuImport.lLength       = (long)pItemData ->lLength;
//						stuImport.guidBrdRes    = pItemData ->guidRes;;
//						stuImport.sName         = stuClipData.sSignalSourceMark;
//						//stuImport.sAlias        = stuClipData.sName;
//						stuImport.sName         = stuClipData.sResSource;
//						stuImport.guidDisk      = stuClipData.guidDevice;
//						stuImport.sDiskName     = stuClipData.sSignalSourceDesc;
//
//						stuImport.aQuality[0].iInPoint  = 0;
//						stuImport.aQuality[0].iOutPoint = (long)pItemData ->lLength;
//						stuImport.aQuality[1].iInPoint  = 0;
//						stuImport.aQuality[1].iOutPoint = (long)pItemData ->lLength;
//						if(stuClipData.uClipFlag & TP_CLIPFLAG_GROUPCLIPXDCAM)   stuImport.uImpType |= TP_IMPTYPE_GROUP;
//						aClipImp.Add(stuImport);					
//					}
//				} 	
//				if(pBrdSysData ->bPopupUploadCenter&&g_stuBrdExplorer.TP_GdiCallBack)  g_stuBrdExplorer.TP_GdiCallBack(TP_BRD_UPLOAD,1);
//				g_stuBrdManage.TP_ApplyReImportRes(aClipImp,aEdlImp,aPgmImp);
//				
//			}
//			else if(pItemData && (pItemData ->uResSource & TP_RESSOURCE_P2))
//			{	   
//				TPP2BaseImport      stuImport;
//				TPP2BaseImportArray aClipImp;
//				TPP2SysSetData     *pP2SysData;
//				DWORD               dwAudio  = TP_CLIP_CLASS_A1 | TP_CLIP_CLASS_A2 |TP_CLIP_CLASS_A3 |TP_CLIP_CLASS_A4; 
//				int                 iSel = -1;
//
//				g_stuP2Manage.TP_InitCache(0);
//				g_stuP2Manage.TP_GetP2SysSetData(pP2SysData);	
//
//				stuImport.sVideoPath         = pP2SysData->sVideoPath;
//				stuImport.sAudioPath         = pP2SysData->sAudioPath;
//				stuImport.guidResClipPath    = pP2SysData->guidResClipPath;
//
//				stuImport.aQuality[0].bImp       = nID ==ID_UPLOAD_HIGH;
//				stuImport.aQuality[0].eClipClass = TP_CLIP_CLASS_V | dwAudio;	
//				stuImport.aQuality[1].bImp       = nID ==ID_UPLOAD_LOW;
//				stuImport.aQuality[1].eClipClass = TP_CLIP_CLASS_V | dwAudio;
//
//				POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//				while (posSel)
//				{
//					iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//					TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//					if(pItemData && (pItemData ->eResType & TP_RES_NOTE))
//					{		
//						GetItemResData(pItemData);
//						TPClipData          stuClipData;
//						g_pClipManageInterface ->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData,TP_GRADE_BASE|TP_GRADE_SOURCE,stuClipData);
//						stuImport.eResType                 = TP_BRD_CLIP;
//						stuImport.lLength                = (long)stuClipData.lClipLength;  
//						stuImport.aQuality[0].iInPoint   = 0;
//						stuImport.aQuality[0].iOutPoint  = (long)stuClipData.lClipLength;
//						stuImport.aQuality[1].iInPoint   = 0;
//						stuImport.aQuality[1].iOutPoint  = (long)stuClipData.lClipLength;
//						stuImport.guidP2Res              = stuClipData.guidRes;			
//						stuImport.sName                  = stuClipData.sSignalSourceMark;									
//						stuImport.uImpState              = 0;	
//						stuImport.sDiskName              = stuClipData.sSignalSourceDesc;	
//						stuImport.guidTop                = stuClipData.guidDevice;					
//						stuImport.lTrimIn                = 0; 
//						stuImport.lTrimOut               = 0;						
//						aClipImp.Add(stuImport);			
//					}
//				}		
//				g_stuP2Manage.TP_ReImportRes(aClipImp);
//			}
//		}
//		break;		
//	case IDC_CLEARCRECYCLE:
//		{
//			//if(TP_IsSystemLoggingOperator()) return;
//			if(m_iMenuItem>0)
//			{
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//				if(pItemData)
//					g_pClipManageInterface ->stuFuncitonInterface.TP_ClearRecRes(pItemData->hResData);
//			}
//			else  
//			{
//				g_pClipManageInterface ->stuFuncitonInterface.TP_ClearRecRes(m_hCatalog);
//			}
//		}
//		break;
//	case IDC_RESTORERECYCLE:
//	case IDC_DELRECYCLE:
//		{			
//			//if(TP_IsSystemLoggingOperator()) return;
//#ifdef TP_NS_NET
//			if (IDC_RESTORERECYCLE != nID && !TP_GetResRight(NULL, TP_RES_UNKNOW, TP_RIGHT_PURGETRASH))
//			{
//				break;
//			}
//#endif
//			BOOL bUserTemplate = FALSE;     
//			TPResDataArray  aSelData,aSelPrj;
//			int  iSel = -1;
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData)
//				{		
//					GetItemResData(pItemData);
//					TPResData  stuData;
//					stuData.eNodeType   = pItemData ->eNodeType;
//					stuData.eResType    = pItemData ->eResType;
//					stuData.guidRes     = pItemData ->guidRes;	
//					stuData.guidDBType  = pItemData ->guidDBType;
//					stuData.hResCache   = m_hCacheData;
//					stuData.hResData    = m_hCatalog;
//					stuData.guidNode    = pItemData ->guidNode;
//					stuData.guidParam   = m_guidCatalog;
//					//{liubo add CD2009.10.01 2010-9-6 
//#ifdef TP_NS_NET
//					stuData.guidDBType	= pItemData->guidDBType;
//					if (nID == IDC_DELRECYCLE && stuData.guidDBType != g_pClipManageInterface->TP_GetCurDBType())
//					{
//						return;
//					}
//
//					if (pItemData->guidDBType != g_pClipManageInterface->TP_GetCurDBType() && pItemData->guidDBType == g_pClipManageInterface->stuSonapsInterface.TP_GetSonapsDBType())
//					{
//						 g_pClipManageInterface->TP_GetResCache(pItemData->hResData, pItemData->eResType, stuData.hResCache);
//					}
//#endif
//					//}
//					if((stuData.eResType & TP_RES_PROJECT) || (stuData.eNodeType & TP_NODE_PROJECTFLODER))  aSelPrj.Add(stuData);
//					else                                                                                    aSelData.Add(stuData);					
//
//					if((stuData.eNodeType &(TP_NODE_USERCOMMANDBUTTON|TP_NODE_COMMANDBUTTON3))  == (TP_NODE_USERCOMMANDBUTTON|TP_NODE_COMMANDBUTTON3))
//					{
//						bUserTemplate = TRUE;
//					}
//				}
//			}
//			if(aSelData.GetSize()<=0 && aSelPrj.GetSize() <= 0) return;
//
//			if(nID == IDC_DELRECYCLE)
//			{
//                //{liubo modify CD2009.10.01 2010-1-27 snp4100021415
//#ifdef TP_NS_NET
//				UINT uID =  MessageBox(_T("Are you sure want to delete the item that were selected? Data will be permanently loss."),TP_LoadStr(_L("Delete Notify")),MB_YESNO | MB_ICONWARNING|MB_DEFBUTTON2);
//#else
//				UINT uID =  MessageBox(SBLoadString(_T("IDS_CLIPEXPLORER_LISTDATAWILLLOSS")),SBLoadString(_T("IDS_CLIPEXPLORER_DELNOTIFY")),MB_YESNO | MB_ICONWARNING|MB_DEFBUTTON2);
//#endif
//				//}
//				if(IDYES != uID)  return;
//
//				aSelData.Append(aSelPrj);
//				TP_SaveCurrentTL(TRUE);
//				g_pClipManageInterface ->stuFuncitonInterface.TP_RemoveRecycleRes(aSelData);
//			}
//			else
//			{
//				if(aSelPrj.GetSize() > 0)
//					g_pClipManageInterface ->stuFuncitonInterface.TP_RevertProject(aSelPrj);
//
//				if(aSelData.GetSize() > 0)
//				{
//					g_pClipManageInterface ->stuFuncitonInterface.TP_RevertRecycleRes(aSelData,NULL);
//
//					HMANAGEDATA pManageData = TP_GetCurManageData();
//					if(pManageData != NULL)
//					{
//						HTLDATA hTLData = ((TPManageData *)pManageData)->hEditData;
//						g_pTimeLineDataEdit -> TP_TLData_SetResUserData(hTLData,TRUE);
//					}
//
//					if(bUserTemplate) g_pClipManageInterface->stuFuncitonInterface.TP_SetCommonFxFlag();
//				}
//			}
//		}
//		break;
//	default:
//		if(nID >= ID_FILEEDIT_BASE && nID <= ID_AEPLUGIN_CMDMAX)
//		{
//			TPFileEditToolStu pStu;long lSupportNum = 0;LPTPFileEditToolSupportStu pSupportStu = NULL;
//			TPResDataArray    aResData;
//			pStu.lDataType  = 0;
//			pStu.pResData   = &aResData;
//
//			//  li hui li
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{				
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//
//				if(pItemData && (pItemData->eResType & (TP_RES_NOTE | TP_RES_ALLPROGRAM)))
//				{
//					GetItemResData(pItemData);
//					TPResData stuResData;				
//					stuResData.eResType		 = pItemData->eResType;
//					stuResData.guidDBType	 = pItemData->guidDBType;					
//					stuResData.guidRes		 = pItemData->guidRes;					
//					stuResData.hResData		 = pItemData->hResData;
//					stuResData.guidNode         = pItemData->guidNode;
//
//					aResData.Add(stuResData);
//				}
//			}
//			if(aResData.GetSize() <= 0) return;		
//			g_pFileManageInterface->stFileToolEditInterface.TPFileEdit_GetSupport(&pStu,lSupportNum,pSupportStu);
//			if(pSupportStu)	g_pFileManageInterface->stFileToolEditInterface.TPFileEdit_EditFile(&pStu,pSupportStu[nID-ID_FILEEDIT_BASE].guidTool);
//		}
//		break;
//	}
	m_iMenuItem = -1;
}

void CTPResListCtrl::OnHdnItemclick(UINT id,NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	//切换点击列，第一次按升序排
	if (m_iSortIndex != phdr ->iItem) m_bSortAsc = TRUE;
	else m_bSortAsc = !m_bSortAsc;
	m_iSortIndex = phdr ->iItem;
	SetColumnFmt(m_iSortIndex,m_bSortAsc ? HDF_SORTUP : HDF_SORTDOWN ,(HDF_SORTDOWN | HDF_SORTUP));
	g_pClipManageInterface ->stuCatalogInterface.TP_SetCatalogSortType(m_hCatalog,0,m_bSortAsc,m_iSortIndex);

	CTPListCtrlEx::SortItems(TP_ResCompareProc,(DWORD)(DWORD_PTR)this);	

	INT bOrderType = TRUE;
	if(VIEWTYPE_ICON == m_iViewType && ARRANGE_STORYBOARD == m_iArrType) bOrderType = 2;

	FunItemIconIndex(bOrderType);    FunCalcItemSize();
	*pResult = 0;
}
void CTPResListCtrl::SubWindow(BOOL bSub)
{
	CTPListCtrlEx::SubWindow(bSub);
	if(bSub)
	{
		for(INT l=0;l<2;l++) TP_ReleaseCommonRes(m_pLinePen[l]);
		m_pLinePen[0] = TP_LoadPen(COLOR_LIST_BOTTOM,PS_SOLID,1); 
		m_pLinePen[1] = TP_LoadPen(COLOR_LIST_TOP,PS_SOLID,1); 	
	}
}


LRESULT CTPResListCtrl::ResetDroppingItem()
{
	if(0 != m_uListTimer[TP_LISTTIMER_DROPPING])
	{
		KillTimer(TP_EVENT_DROPPINGITEM);
		m_uListTimer[TP_LISTTIMER_DROPPING] = 0;
	}

	InvalidateRect(m_rtClient);

	return S_OK;
}


LRESULT CTPResListCtrl::Dropping(ULONGLONG dwFrom,TP_RES_TYPE dwDropType,LPARAM lParam)
{
//	if(dwFrom & (TP_DATA_CLIPEDIT|TP_DATA_DESKTOP))    return S_DECLINE;
//	TPResDataArray *paResData = (TPResDataArray *)lParam;
//	if(paResData == NULL || paResData ->GetSize() <=0) return S_DECLINE;
//	if(!TP_GetSafeAdress(m_hCatalog,TP_SAFETYPE_RESCATALOG))				   return S_DECLINE;
//	if(0 == m_uListTimer[TP_LISTTIMER_DROPPING]) m_uListTimer[TP_LISTTIMER_DROPPING] = SetTimer(TP_EVENT_DROPPINGITEM, 50, NULL);
//	
//	CPoint ptScreen;
//	GetCursorPos(&ptScreen);
//	ScreenToClient(&ptScreen);	
//
//	//先判断资源类型是否相同
//	int iItem = CTPListCtrlEx::HitTest(ptScreen);
//	TPListItemData *pListData = NULL;
//	pListData = (TPListItemData *)CTPListCtrlEx::GetItemData(iItem);
//	if(NULL != pListData) GetItemResData(pListData,FALSE);
//
	LRESULT lRet = S_FALSE;
//
//	if(dwDropType & TP_RES_EFFECTLIST)
//	{
//		if(pListData && pListData->eResType == TP_RES_CATALOG)
//		{
//			lRet = DropEffList(pListData->hResData,dwFrom,dwDropType,lParam,TRUE);			
//		}//在窗口移动
//		else
//		{
//			lRet = DropEffList(m_hCatalog,dwFrom,dwDropType,lParam,TRUE);
//		}//非目录资源上移动 		
//	}
//	else
//	{	
//		if(TP_DATA_BRDEXPLORER & dwFrom)
//		{
//			if(m_eNodeType & TP_NODE_TRASH)  
//				return S_DECLINE;
//			lRet = S_ACCEPT;
//			TPCatalogData stuCatalog;			
//			g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(m_hCatalog, &stuCatalog);
//			HRESDATA hCatalogData = m_hCatalog;
//			if(GUID_NULL != g_guidResProject.guidRes && stuCatalog.guidProject == g_guidResProject.guidRes)
//			{
//				if(!g_pClipManageInterface->stuCatalogInterface.TP_GetAddItemBin(hCatalogData,stuCatalog.guidDBType, TP_RES_NOTE, FALSE))	lRet = S_DECLINE;
//				for(int i = 0; i < paResData->GetSize(); i++)
//				{
//					(*paResData)[i].guidNode = stuCatalog.guidRes;
//				}
//			}
//			else
//			{
//				lRet = S_DECLINE;
//			}
//		}
//		else
//		{
//			//if(TP_IsSystemLoggingOperator(FALSE)) return S_DECLINE;	// XN00030562 lihuili 2011-01-14 
//
//			HRESDATA hDesCatalog = m_hCatalog;
//			BOOL     bCut		 = TRUE;
//			if(pListData && pListData->eResType == TP_RES_CATALOG)
//			{
//				hDesCatalog = pListData->hResData;
//				bCut        = TP_IsDropCutRes(paResData->GetAt(0).guidDBType, paResData->GetAt(0).eNodeType, pListData->guidDBType, pListData->eNodeType, dwFrom & TP_DATA_RBUTTONDRAG ? TRUE : FALSE);
//			}
//			else
//			{
//				hDesCatalog = m_hCatalog;
//				bCut        = TP_IsDropCutRes(paResData->GetAt(0).guidDBType, paResData->GetAt(0).eNodeType, m_guidDBType, m_eNodeType, dwFrom & TP_DATA_RBUTTONDRAG ? TRUE : FALSE);
//			}
//				 
//			//{liubo add 2011-4-1 snp4100034923
//#ifdef TP_NS_NET
//			Afx_SetParam(_T("PASTETEST_DROPPING"),TRUE);
//#endif
//			//}
//
//			lRet = g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(hDesCatalog,paResData,bCut);
//
//			//{liubo add 2011-4-1 snp4100034923
//#ifdef TP_NS_NET
//			Afx_SetParam(_T("PASTETEST_DROPPING"),FALSE);
//			Afx_RemoveParam(_T("PASTETEST_DROPPING"));
//#endif
//			//}
//
//		}
//	}

	return lRet;
}

LRESULT CTPResListCtrl::Drop(ULONGLONG dwFrom,TP_RES_TYPE dwDropType,LPARAM lParam)
{
//	TP_ResetClipExporerDroppingItem();
//	if(dwFrom & (TP_DATA_CLIPEDIT|TP_DATA_DESKTOP)) return S_DECLINE;
//	if(!TP_GetSafeAdress(m_hCatalog,TP_SAFETYPE_RESCATALOG)) return S_DECLINE;
//
//	TPResDataArray *paResData = (TPResDataArray *)lParam;
//	if(paResData == NULL || paResData ->GetSize() <=0) return S_OK;
//	TP_NODE_TYPE eNodeType = paResData->GetAt(0).eNodeType;
//	//取得Drop项的
//	CPoint Point;	
//	GetCursorPos(&Point); 
//	CWnd *pDropWnd = WindowFromPoint(Point);
//	ScreenToClient(&Point);
//
//	if(m_hCatalog == NULL ) return S_FALSE;
//	int  iItem = -1;
//	iItem = CTPListCtrlEx::HitTest(Point);
//	TPListItemData *pListData = (TPListItemData *)GetItemData(iItem);	
//	if(NULL != pListData) GetItemResData(pListData);
//	LRESULT lResult  = S_FALSE;
//
//	if(dwDropType & TP_RES_EFFECTLIST)
//	{
//		if(pListData && pListData->eResType == TP_RES_CATALOG)
//		{
//			lResult = DropEffList(pListData->hResData,dwFrom,dwDropType,lParam,FALSE);			
//		}
//		else 
//		{
//			lResult = DropEffList(m_hCatalog,dwFrom,dwDropType,lParam,FALSE);
//		}
//	}
//	else if(TP_DATA_BRDEXPLORER & dwFrom)
//	{
//		// Drop Res Test
//		if(m_eNodeType & TP_NODE_TRASH)  
//			return S_DECLINE;
//		TPCatalogData stuCatalog;
//		g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(m_hCatalog, &stuCatalog);
//		if(GUID_NULL == g_guidResProject.guidRes || g_guidResProject.guidRes != stuCatalog.guidProject) return S_FALSE;
//		HRESDATA hCatalogData = m_hCatalog;
//		if(!g_pClipManageInterface->stuCatalogInterface.TP_GetAddItemBin(hCatalogData,stuCatalog.guidDBType, TP_RES_NOTE, FALSE)) return S_FALSE;
//		if(paResData->GetAt(0).guidNode == m_guidCatalog) 
//		{
//			lResult = S_OK;
//		}
//		else
//		{
//			return lResult;
//		}
//		
//		TPResData stuResData = paResData->GetAt(0);
//		BOOL bCut = TRUE;
//		HRESDATA hDesCatalog = m_hCatalog;
//		if(pListData && pListData->eResType & TP_RES_CATALOG)
//		{
//			hDesCatalog = pListData->hResData;
//			bCut = TP_IsDropCutRes(stuResData.guidDBType, stuResData.eNodeType, pListData->guidDBType, pListData->eNodeType, dwFrom & TP_DATA_RBUTTONDRAG ? TRUE : FALSE);
//		}
//		else 
//		{
//			hDesCatalog = m_hCatalog;
//			bCut = TP_IsDropCutRes(stuResData.guidDBType, stuResData.eNodeType, m_guidDBType, m_eNodeType, dwFrom & TP_DATA_RBUTTONDRAG ? TRUE : FALSE);
//		}
//
//		lResult = g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(hDesCatalog,paResData,bCut);
//#ifdef TP_NS_NET
//		Afx_SetParam(_T("PASTETEST_DROPPING"),FALSE);
//		Afx_RemoveParam(_T("PASTETEST_DROPPING"));
//#endif
//
//		if(lResult == S_DECLINE) return S_FALSE;
//
//		if(!bCut)     
//		{
//			HRESDATA hCatalogPaste = m_hCatalog;
//			if(pListData && pListData->eResType == TP_RES_CATALOG) hCatalogPaste = pListData->hResData; 
//			lResult  = CopyResTo(hCatalogPaste, *paResData, Point);
//			CTPListCtrlEx::SelectItem(-1, TRUE, TRUE);
//
//#ifdef TP_NS_NET
//			SelectItemGuid(m_aGuidReturn);
//#endif
//		}
//		else
//		{
//			CTPResExplorerPanel::DataStateCallBack(0,_L(""),_L(""),TP_STATECALLBACK_SETGLOBAL);
//			LRESULT lResult = S_FALSE;UINT uRes = 0;
//
//#ifdef TP_NS_NET
//			Afx_SetParam(_T("PASTETEST_DROPPING"),TRUE);
//#endif
//
//			if(TP_DeleteTemplateResFromUserCommandButton(hDesCatalog) == S_FALSE) return S_FALSE;
//			lResult = g_pClipManageInterface->stuFuncitonInterface.TP_MoveResTo(hDesCatalog,*paResData);
//
//#ifdef TP_NS_NET
//			Afx_SetParam(_T("PASTETEST_DROPPING"),FALSE);
//			Afx_RemoveParam(_T("PASTETEST_DROPPING"));
//#endif
//		}	
//	}

	LRESULT lResult = S_FALSE;
	return lResult;
}

void CTPResListCtrl::AddRes(GUID guidDBType, GUID guidCatalog,HRESDATA hResData,TP_RES_TYPE eResType,GUID guidRes)
{
	if(m_guidCatalog == guidCatalog)
	{	
		int l= m_aItem.GetSize();
		CTPListCtrlEx::InsertItem(l,_L("         "));//LPSTR_TEXTCALLBACK);
		TPListItemData *pItemData = new TPListItemData();
		pItemData->eResType		= eResType;
		pItemData->guidRes		= guidRes;
		pItemData->guidDBType	= guidDBType;
		pItemData->guidNode		= guidCatalog;
		pItemData->hResData		= hResData;
//在这里不设定将会报错,现在还暂时这么修改,目的仅仅是为了不报错,因为在这之前系统重新刷新了
//所有的图标,所有图标都按照从0开始的顺序排列,所有这里这么写是对的,但是这已经破坏了storyboard的定义,如果以后要重新实现storyboard,那么这里要重新想办法实现,并且
//之前也不能刷新从而丢掉了之前保存的位置信息.
		CTPListCtrlEx::SetItemData(l,(DWORD_PTR)pItemData);
		pItemData ->pItem = m_aItem[l];

	}
}
void CTPResListCtrl::DeleteRes(GUID guidDBType,HRESDATA hResData,TP_RES_TYPE eResType,GUID guidRes)
{
	for(INT k=CTPListCtrlEx::GetItemCount()-1;k>=0;k--)
	{								
		TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(k);
 		if(pItemData && pItemData ->guidRes == guidRes && pItemData->guidDBType == guidDBType)
 		{		
			CTPResListCtrl::DeleteItem(k);
			break;
		}		
	}
}
HRESDATA CTPResListCtrl::GetDropCatalog(CPoint &pPoint)
{
	int iDropItem = CTPListCtrlEx::HitTest(pPoint);
	HRESDATA  hCatalog = m_hCatalog;
	//if(iDropItem>=0)
	//{
	//	TPListItemData *pListData = (TPListItemData *)CTPListCtrlEx::GetItemData(iDropItem);
	//	if(pListData &&pListData ->hResData && (pListData ->eResType & TP_RES_CATALOG) == TP_RES_CATALOG)
	//	{
	//		hCatalog = pListData ->hResData;
	//	}
	//}

	//TPResCatalog stuResCatalog;
	//g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(hCatalog, &stuResCatalog);
	//if(GUID_NULL == g_guidResProject.guidRes || GUID_NULL == stuResCatalog.guidProject || stuResCatalog.guidProject != g_guidResProject.guidRes) 
	//	return NULL;

	//// add by qsj for check
	//if(!(stuResCatalog.dwOperateFlag & TP_OPERATEFLAG_INSERTITEM))
	//	return NULL;

	//if(stuResCatalog.eNodeType & (TP_NODE_TRASH|TP_NODE_EFECT|TP_NODE_CG)) return NULL;

	//// 检查目录是否允许
	//if(!g_pClipManageInterface->stuCatalogInterface.TP_GetAddItemBin(hCatalog,stuResCatalog.guidDBType, TP_RES_NOTE, FALSE))	return NULL;
	//m_hDropCatalog = hCatalog;
	//// end qsj
	//
	return hCatalog;
}
void CTPResListCtrl::OnDropFiles(HDROP hDropInfo)
{
//	if(g_bDropFiles)  return;

	TCHAR   lpszFileName[MAX_PATH];
	int     iFileCount;
	CPoint  point;
	iFileCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount<=0)  { ::DragFinish(hDropInfo) ; return ;}
	::DragQueryPoint(hDropInfo, &point);

	CStringArray aHDXSel,aXMLSel,aOtherSel;
	CString      sFileName; 

	m_aDropFile.RemoveAll();
	for(int l=0;l<iFileCount;l++)
	{
		TCHAR   lpsztmpName[MAX_PATH];  //  zhaofei 2008-08-26 把类似“c:\progra~1\”的格式，改成“c:\program Files”的形式
		memset(lpsztmpName,0,sizeof(TCHAR)*MAX_PATH);
		memset(lpszFileName,0,sizeof(TCHAR)*MAX_PATH);
		UINT uChar = ::DragQueryFile(hDropInfo, l, lpsztmpName,MAX_PATH);
		::GetLongPathName(lpsztmpName,lpszFileName,MAX_PATH);		
		sFileName = lpszFileName;
		m_aDropFile.Add(sFileName);	
	}
	::DragFinish (hDropInfo);
   
	PostMessage(WM_LIST_DROPFILE, 0, 0); 
}

LRESULT CTPResListCtrl::OnMsgDropFile(WPARAM wParam ,LPARAM lParam)
{
	//if(m_aDropFile.GetSize() <= 0) return S_FALSE;
	///*if(g_pSystemInfo->uSystemState & TP_SYSTEMSTATE_LOGING) 
	//{
	//	m_aDropFile.RemoveAll();
	//	AfxMessageBox(TP_LoadStr(_T("IDS_TIMELINEWND_LOGINING")));	
	//	return S_FALSE ;
	//}	*/
	//CStringArray aHDXSel, aXMLSel, aOtherSel;
	//for(int l=0;l<m_aDropFile.GetSize();l++)
	//{	
	//	CString sFileName = m_aDropFile[l];
	//	sFileName.MakeUpper();
	//	if(g_stuBrdManage.TP_IsHDXClip(sFileName,NULL) == S_OK) 
	//	{
	//		aHDXSel.Add(m_aDropFile[l]);
	//	}
	//	//else if(sFileName.Find(_T(".XML")) >=0)    
	//	else if(sFileName.Right(4).CompareNoCase(_T(".XML")) == 0)	   //qushaojie20110401 1.xml.txt 
	//	{
	//		aXMLSel.Add(m_aDropFile[l]);
	//	}
	//	else
	//	{
	//		aOtherSel.Add(m_aDropFile[l]);
	//	}
	//}

	//HRESDATA hCatalog = m_hDropCatalog;
	//if(!TP_GetSafeAdress(hCatalog,TP_SAFETYPE_RESCATALOG)) return S_FALSE;
	//if(aHDXSel.GetSize()>0)
	//{
	//	CGuidArray        aClip;
	//	g_bDropFiles       = TRUE;
	//	TP_ClearMessage(NULL);
	//	g_stuBrdManage.TP_ImpHDXClip(aHDXSel,hCatalog,aClip);
	//	g_bDropFiles       = FALSE;
	//}

	//if(aXMLSel.GetSize()>0)
	//{
	//	TPBrdXmlImpResArray aClip;
	//	g_bDropFiles       = TRUE;
	//	TP_ClearMessage(NULL);
	//	g_stuBrdManage.TP_ImpXml(aXMLSel,hCatalog,aClip);
	//	g_bDropFiles       = FALSE;
	//}
	//if(aOtherSel.GetSize()>0)
	//{
	//	//判断有无解码器
	//	if(g_pFileManageInterface == NULL || g_pFileManageInterface->TPFile_OpenFile == NULL)
	//	{
	//		MessageBox(SBLoadString(_T("IDS_CLIPEXPLORER_SELDECODEFISTLY")));
	//		return S_FALSE;
	//	}

	//	g_bDropFiles       = TRUE;
	//	TP_ClearMessage(NULL);
	//	TP_CLIP_MOVE_STYLE eMoveStyle = TP_CLIP_IMP_GROUP;
	//	if(g_pClipManageSet->bCopyDrag) eMoveStyle |= TP_CLIP_MOVE_COPY;
	//	else							eMoveStyle |= TP_CLIP_MOVE_NONE;

	//	// li song ming 2009-12-07 
	//	CWnd *pFocus = GetFocus();
	//	if(pFocus == NULL) SetFocus();

	//	g_pClipManageInterface->stuClipInterfce.TP_ImportDirectory(hCatalog,aOtherSel,eMoveStyle);
	//	g_bDropFiles       = FALSE;		
	//}

	return S_OK;


}
BOOL CTPResListCtrl::OnKeyDown(UINT vKey)
{
	return FALSE;
}
BOOL CTPResListCtrl::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{	
	//if(g_pClipManageSet->cToolTip == 0)    return FALSE;
	//TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	//CPoint point;
	//GetCursorPos(&point);
	//ScreenToClient(&point);
	//if(!m_rtClient.PtInRect(point)) return FALSE; // 如果不在客户区不处理
	//int iItem =CTPListCtrlEx:: HitTest(point,NULL);	

	//TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iItem);
	//if(pItemData && m_iItemSave != iItem ) 
	//{
	//	CString sToolTips = _T("");
	//	lstrcpyn( pTTT->szText ,_L(" "), 80);
	//	if(pItemData ->eResType & TP_RES_USER)
	//	{		
	//		sToolTips.Format(_L("%s:%s "),SBLoadString(_T("IDS_CLIPEXPLORER_NAME")),pItemData ->sText[0]);
	//	}
	//	else if(m_eNodeType & TP_NODE_TRASH)
	//	{	
	//		sToolTips.Format(_L("%s:%s "), SBLoadString(_T("IDS_CLIPEXPLORER_NAME")),pItemData ->sText[0]);
	//	}	
	//	else if(pItemData ->eResType & (TP_RES_PROJECT | TP_RES_CATALOG))
	//	{			
	//		sToolTips.Format(_L("%s:%s "), SBLoadString(_T("IDS_CLIPEXPLORER_NAME")),pItemData ->sText[0]);
	//	}
	//	else if(pItemData ->eResType == TP_RES_NOTE)//判断pItemData
	//	{			
	//		sToolTips.Format(_L("%s:%s \n%s:%s \n%s:  %s \n%s:%s "),SBLoadString(_T("IDS_CLIPEXPLORER_NAME")),pItemData ->sText[0],SBLoadString(_T("IDS_CLIPEXPLORER_TYPE")),pItemData ->VAString,SBLoadString(_T("IDS_CLIPEXPLORER_LENGTH")),pItemData ->sText[TP_RESTEXT_CLIP_LENGTH],SBLoadString(_T("IDS_CLIPEXPLORER_CREATETIME")),pItemData ->sText[TP_RESTEXT_CREATE_DATE]);
	//		if(pItemData ->dwCaptureFlag &TP_CLIPCAPTURE_EDI)
	//		{
	//			long lFileIndex,lCapLen;;
	//			lCapLen = g_pClipManageInterface->stuClipInterfce.TP_GetNoteEdiLen(pItemData ->hResData,lFileIndex);
	//			if(lCapLen >=0)
	//			{
	//				VIDEO_TYPE eVideoType = TP_GetVideoType(pItemData ->dwVideoStand,pItemData ->bDropFrame);
	//				CString sCapLen;
	//				sCapLen.Format(_T("\n%s:  %s"), SBLoadString(_T("IDS_CLIPEXPLORER_CAPLEN")),TP_StringFromFrame(eVideoType,lCapLen));
	//				sToolTips += sCapLen;
	//			}
	//		}
	//	}
	//	else if(pItemData && pItemData ->eResType & (TP_RES_ALLVAEFFECT | TP_RES_CGEFFECT))
	//	{

	//		sToolTips.Format(_L("%s:%s "), SBLoadString(_T("IDS_CLIPEXPLORER_NAME")),pItemData ->sText[0]);
	//	}
	//	else if(pItemData && pItemData ->eResType & TP_RES_ALLPROGRAM)
	//	{
	//		sToolTips.Format(_T("%s:%s\n%s:%s\n%s:%s\n%s:%s"),SBLoadString(_T("IDS_CLIPEXPLORER_NAME")), pItemData ->sText[0],SBLoadString(_T("IDS_CLIPEXPLORER_LENGTH")), pItemData ->sText[TP_RESTEXT_CLIP_LENGTH],SBLoadString(_T("IDS_CLIPEXPLORER_CREATETIME")), pItemData->sText[TP_RESTEXT_CREATE_DATE], SBLoadString(_T("IDS_CLIPEXPLORER_UPDATETIME")), pItemData->sText[TP_RESTEXT_UPDATE_DATE]);
	//	}
	//	else if(pItemData && pItemData ->eResType & TP_RES_CGFILTER)
	//	{
	//	}	
	//	else if(pItemData ->eResType & TP_RES_CGPREFABRICATE)///wfp
	//	{
	//	}
	//	else if(pItemData && pItemData ->eResType & TP_RES_ALLTEMPLATE )
	//	{			
	//		SYSTEMTIME tmLastModify = pItemData->tmModifyTime;
	//		tmLastModify.wMilliseconds = pItemData->tmCreateTime.wMilliseconds;
	//		if(tmLastModify.wYear>=2007 && memcmp(&tmLastModify,&pItemData->tmCreateTime,sizeof(SYSTEMTIME))!=0)
	//			sToolTips.Format(_T("%s:%s\n%s:%s\n%s:%s "),SBLoadString(_T("IDS_CLIPEXPLORER_NAME")), pItemData ->sText[0], SBLoadString(_T("IDS_CLIPEXPLORER_CREATETIME")), TP_GetTimeString(pItemData->tmCreateTime), SBLoadString(_T("IDS_CLIPEXPLORER_UPDATETIME")), TP_GetTimeString(tmLastModify));
	//		else
	//			sToolTips.Format(_T("%s:%s\n%s:%s"), SBLoadString(_T("IDS_CLIPEXPLORER_NAME")), pItemData ->sText[0],SBLoadString(_T("IDS_CLIPEXPLORER_CREATETIME")), TP_GetTimeString(pItemData->tmCreateTime));
	//	}

	//	if(TP_IsTeamMode() && !(m_eNodeType & TP_NODE_TRASH) && (pItemData ->eResType &(TP_RES_PROJECT | TP_RES_ALLTEMPLATE|TP_RES_ALLPROGRAM)))
	//	{
	//		CString sInfo;
	//		if(g_pClipManageInterface ->stuProgramInterface.TP_GetOpenResInfo(pItemData->hResData,pItemData->eResType,sInfo))
	//		{
	//			sToolTips += sInfo;
	//		}
	//	}

	//	m_pToolTipCtrl-> m_sToolTips = sToolTips;

	//	return  TRUE;		
	//}

	return FALSE;
}

void CTPResListCtrl::OnDestroy()
{
//	TP_TransMessage(WM_UPDATEVIEW);
//	TP_TransMessage(WM_LIST_RESRENANME);
	CTPListCtrlEx::OnDestroy();
}


void CTPResListCtrl::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CTPListCtrlEx::OnWindowPosChanged(lpwndpos);
}

void CTPResListCtrl::OnCaptureChanged(CWnd *pWnd)
{
	CTPListCtrlEx::OnCaptureChanged(pWnd);
	if(m_eDragResType     != TP_RES_UNKNOW)
	{
		m_iHeadDown = -1;
		m_iDragMode = TP_DRAG_UNKNOW;
		Invalidate(FALSE);
		::SetCursor(m_hCursorDrag[0] ->GetCursor()); 

		m_eDragResType     = TP_RES_UNKNOW;	
		m_iDragMode        = TP_DRAG_UNKNOW;
		ReleaseDragImage();
		TP_RemoveResDataArray(m_hDragData,TRUE);
	}
}

void CTPResListCtrl::GetItemResData(int iItem)
{
	TPListItemData *pItemData = NULL;
	pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iItem);
	if(pItemData == NULL) return;
	if(pItemData ->bGetData == FALSE) GetItemResData(pItemData);
}

void CTPResListCtrl::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == TP_EVENT_DROPPINGITEM)
	{
		CPoint ptCursorPos;
		GetCursorPos(&ptCursorPos);
		ScreenToClient(&ptCursorPos);
		CRect rtClient;
		GetClientRect(rtClient);

		if((ptCursorPos.y < rtClient.top + 20) && rtClient.PtInRect(ptCursorPos))
		{
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, NULL), NULL);
		}
		else if((ptCursorPos.y > rtClient.bottom - 20)  && rtClient.PtInRect(ptCursorPos))
		{
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, NULL), NULL);
		}
		else if((ptCursorPos.x < rtClient.left + 20) && rtClient.PtInRect(ptCursorPos))
		{
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, NULL), NULL);
		}
		else if((ptCursorPos.x > rtClient.right - 20) && rtClient.PtInRect(ptCursorPos))
		{
			SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, NULL), NULL);
		}
		else
		{
			if(!rtClient.PtInRect(ptCursorPos))
				ResetDroppingItem();
		}
	}
	CTPListCtrlEx::OnTimer(nIDEvent);
}

void CTPResListCtrl::GetSelectRes(GUID &guidMenu,CGuidArray &aSelect)
{
	// 把当前选中的GUID保存到guidMenu
	guidMenu = GUID_NULL;
	aSelect.RemoveAll();
	if(-1 !=m_iMenuItem)
	{
		TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
		if(pItemData == NULL)    return;
		guidMenu = pItemData->guidRes;
    }
	// 把List所有选中的GUID保存到aSelect
    POSITION pos  = GetFirstSelectedItemPosition();
	int      iSel = -1;
	while(pos)
	{
		iSel = GetNextSelectedItem(pos);
		TPListItemData  *pItemData = (TPListItemData *)GetItemData(iSel);
		if(pItemData)
		{
			if(pItemData->guidRes != GUID_NULL) 
			   aSelect.Add(pItemData->guidRes);
		}		
	}
}
void CTPResListCtrl::SetSelectRes(GUID  guidMenu,CGuidArray &aSelect, BOOL bEnsureVisible)
{
	//m_hMenuItem = guidMenu选中的Item
	m_iMenuItem = -1;
	if(guidMenu != GUID_NULL)
	{
        int iCount = GetItemCount();
		for(INT l=0;l<iCount;l++)
		{
			TPListItemData  *pItemData = (TPListItemData *)GetItemData(l);
			if(pItemData && pItemData->guidRes == guidMenu)
			{
					m_iMenuItem = l;
					break;
			}
		}
	}
    // 把aSelect记Guid都选中,
	if(aSelect.GetSize() > 0)
	{
		INT nItemCount = GetItemCount();
		for(INT i =  aSelect.GetSize() - 1; i >= 0; i--)
		{
			for(INT j = 0; j < nItemCount; j++)
			{
				TPListItemData  *pItemData = (TPListItemData *)GetItemData(j);
				if(pItemData && aSelect[i] == pItemData->guidRes)
				{
					CTPListCtrlEx::SelectItem(j,TRUE,FALSE);
					aSelect.RemoveAt(i);
					break;
				}
			}
		}	
		if(bEnsureVisible)
		{
			POSITION pos = GetFirstSelectedItemPosition();
			if(NULL != pos)
			{
				int nItem = GetNextSelectedItem(pos);
				EnsureVisible(nItem, TRUE);
			}
		}
	}
}
//add by qushaojie 20091027 清除某一列的排序标志
void CTPResListCtrl::ClearSortFlag(INT iItem)
{
	//if(m_iSortIndex >= 0)
	//{
 //       SetColumnFmt(m_iSortIndex,0,(HDF_SORTUP|HDF_SORTDOWN));
	//	m_iSortIndex = -1;
	//	g_pClipManageInterface ->stuCatalogInterface.TP_SetCatalogSortType(m_hCatalog,0,FALSE,-1);
	//}
}
void CTPResListCtrl::AddMeunColor(CTPMenuEx *pMenu,int iPosition,UINT uBaseID)
{
	CDWordArray aColor;
	for(INT l= TP_CLIPCOLOR_BASE;l<TP_CLIPCOLOR_MAX;l++)
		aColor.Add((DWORD)TP_GetResColor((TP_RES_COLOR)l));
	pMenu ->AddColorMenu(aColor,0,iPosition,uBaseID);
}

BOOL CTPResListCtrl::ResRenameForOrder(GUID guidRes, CString sNewName)
{
	int  iNameCol = - 1;
	BOOL bSortAsc = FALSE;
	//获取name列当前显示列号,并判断当前排序方式
	iNameCol = GetItemByOrder(TP_RESTEXT_CLIP_NAME);
	if(iNameCol < 0) return FALSE;
	if (m_iSortIndex != iNameCol) return FALSE;
	else
	{
		if (m_aColumn[iNameCol]->uFmt & HDF_SORTDOWN) bSortAsc = FALSE;
		else if(m_aColumn[iNameCol]->uFmt & HDF_SORTUP) bSortAsc = TRUE;
		else return FALSE;
	}


	for (int i=0;i<m_aItem.GetSize();i++)
	{
		TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(i);
		if(NULL == pItemData) continue;
		GetItemResData(pItemData);
		if (guidRes == pItemData->guidRes)
		{
		   if(i-1>=0) 
		   {
		      TPListItemData *pItemData1 = (TPListItemData *)CTPListCtrlEx::GetItemData(i-1);
			  GetItemResData(pItemData1,FALSE);
			  if (StrCmpLogicalW(sNewName,pItemData1->sText[TP_RESTEXT_CLIP_NAME])>0 && !bSortAsc ||StrCmpLogicalW(sNewName,pItemData1->sText[TP_RESTEXT_CLIP_NAME])<0 && bSortAsc)
			  {
				  ClearSortFlag(0);
				  return TRUE;
			  }
		   }
		   if(i+1<m_aItem.GetSize()) 
		   {
			   TPListItemData *pItemData1 = (TPListItemData *)CTPListCtrlEx::GetItemData(i+1);
			   GetItemResData(pItemData1,FALSE);
			   if (StrCmpLogicalW(sNewName,pItemData1->sText[TP_RESTEXT_CLIP_NAME])>0 && bSortAsc ||StrCmpLogicalW(sNewName,pItemData1->sText[TP_RESTEXT_CLIP_NAME])<0 && !bSortAsc)
			   {
				   ClearSortFlag(0);
				   return TRUE;
			   }
		   }
		}
	}
	return TRUE;
}
INT CTPResListCtrl::IsResExist(GUID guidRes)
{
	INT nCount = GetItemCount();
	for(INT i = 0; i < nCount; i++)
	{
		TPListItemData* pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(i);
		if(NULL != pItemData && guidRes == pItemData->guidRes) return i;
	}

	return -1;
}

BOOL CTPResListCtrl::PreviewGif(INT nIndex)
{
	return FALSE;
}

LRESULT CTPResListCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	PreviewGif(-1);
	m_bTrackLeave = FALSE;
	return S_OK ;
}
LRESULT CTPResListCtrl::CopyResTo(HRESDATA hCatalog, TPResDataArray& aResData, CPoint ptPoint)
{
	//LRESULT lRetsult = S_FALSE;
	//if(NULL == hCatalog) return S_FALSE;
	//if(aResData.GetSize() <= 0) return FALSE;
	//m_aItemIndexSave.RemoveAll();
	//m_aGuidReturn.RemoveAll();

	//INT nOffset = 0;
	//BOOL bCalcIndex = TRUE;
	//if(m_iArrType != ARRANGE_STORYBOARD||m_iViewType != VIEWTYPE_ICON) bCalcIndex = FALSE;
	//if(!m_bDragIn)													   bCalcIndex = FALSE;// 窗口外拖拽不处理index
	//if(bCalcIndex)
	//{
	//	CRect rtRect(9999,9999,0,0);
	//	nOffset = FunMoveClip(ptPoint,TRUE,rtRect,TP_CLIPDRAGTYPE_COPY);
	//}

	//TPResDataArray aReturn;
	//if(m_eNodeType & TP_NODE_USERINFO)
	//{
	//	for(int i = 0 ; i< aResData.GetSize() ; i++)
	//	{
	//		GUID guidUser = aResData[i].guidRes;
	//		g_stuClipExpolrerCallBack.TP_SaveUserInfo(guidUser);
	//		TPUserDepartmentBaseInfo stuUserInfo;
	//		if(g_pUserMangeInterface->TP_NS_GetUserInfo(guidUser, stuUserInfo) == S_OK)
	//		{
	//			GUID    guidSame   = GUID_NULL;
	//			CString sCheckName = stuUserInfo.sUserConfigrationName;
	//			stuUserInfo.sUserConfigrationName = g_pUserMangeInterface->TP_NS_CheckUserName((LPTSTR)(LPCTSTR)sCheckName, guidSame);
	//			lRetsult = g_pUserMangeInterface->TP_NS_SaveAsUserInfo(guidUser, stuUserInfo);
	//			aResData[i].guidReturn = aResData[i].guidRes;
	//			aResData[i].guidRes    = stuUserInfo.guidUserConfigration;
	//			aReturn.Add(aResData[i]);
	//		}
	//	}
	//}
	//else
	//{
	//	lRetsult = g_pClipManageInterface->stuFuncitonInterface.TP_CopyResTo(hCatalog,aResData);
	//	g_pClipManageInterface->stuFuncitonInterface.TP_GetReturnRes(aReturn,TRUE);
	//}
	//if(hCatalog == m_hCatalog)
	//{
	//	TPCatalogData stuCatalog;
	//	g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(hCatalog, &stuCatalog);
	//	for(INT l=0;l<aReturn.GetSize();l++)
	//	{
	//		if(aReturn[l].guidNode == stuCatalog.guidRes)	
	//		{
	//			m_aGuidReturn.Add(aReturn[l].guidRes);
	//			if (bCalcIndex && nOffset != 0)
	//			{
	//				for (int i=0; i<m_aItem.GetSize();i++)
	//				{
	//					TPListItemData * pItemData = (TPListItemData *)m_aItem[i]->dwData;
	//					if (aReturn[l].guidReturn == pItemData->guidRes)
	//					{
	//						TPListItemData * pItemData = (TPListItemData *)m_aItem[i]->dwData;
	//						aReturn[l].wParam = nOffset + pItemData->pItem->lIconIndex;
	//						m_aItemIndexSave.Add(aReturn[l]);
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//if(bCalcIndex && aReturn.GetSize() > 0)
	//{
	//	CRect rtRect(9999,9999,0,0);
	//	FunMoveClip(ptPoint,FALSE,rtRect,TP_CLIPDRAGTYPE_COPY);
	//}

	LRESULT lResult = FALSE;
	return lResult;
}
