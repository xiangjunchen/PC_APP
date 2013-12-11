#include "StdAfx.h"
#include ".\TPNSResListCtrl.h"

//////////////////////////////////////////////////////////////////////////

extern CTPGetIcon					  *g_pGetIconFun;
extern TPFlashResDataArray			   g_aFlashResData;
extern CCriticalSection				   g_crFlashResLock;
extern CTPNSResExplorerPanel         *g_pFocusClipExplorerPanel;
TPResGuid   						   g_guidOpenWithThisBin;


#define TP_FASHICON_ESCIP			 800

#define TP_COLOR_ACTIVEUSER		RGB(242, 47, 2)

enum TP_CLIPTYPE_INDEX
{
	TP_CLIPTYPE_SEQUENCE,
	TP_CLIPTYPE_CDAUDIOCLIP,
	TP_CLIPTYPE_ALPHACOPYCLIP,
	TP_CLIPTYPE_LIVECLIP,
	TP_CLIPTYPE_REELCLIPDIGITIZEDCLIP,
	TP_CLIPTYPE_SERVERCLIPNETWORK,
	TP_CLIPTYPE_VOICEOVERCLIP,
	TP_CLIPTYPE_TITLECLIPCG,
	TP_CLIPTYPE_MXFCLIP,
	TP_CLIPTYPE_RENDERTOBINCLIP,
	TP_CLIPTYPE_SGCLIP,
	TP_CLIPTYPE_SPEEDCLIP,
	TP_CLIPTYPE_EFFECTTEMPLATE,
	TP_CLIPTYPE_PROJECTGROUP,
	TP_CLIPTYPE_BIN,
	TP_CLIPTYPE_TRASH,
	TP_CLIPTYPE_PROJECTSD,
	TP_CLIPTYPE_PROJECTHD,
	TP_CLIPTYPE_USER,
	TP_CLIPTYPE_XDCAM,
	TP_CLIPTYPE_MP4,
	TP_CLIPTYPE_P2,
	TP_CLIPTYPE_XDCAMEX,
	TP_CLIPTYPE_STAYIN,
	TP_CLIPTYPE_STAY,
	TP_CLIPTYPE_STAYOUT,
	TP_CLIPTYPE_TITLE,
	TP_CLIPTYPE_CGTEMPLATE,
	TP_CLIPTYPE_VTEMPLATE,
	TP_CLIPTYPE_ATEMPLATE,
	TP_CLIPTYPE_VTRANSITION,
	TP_CLIPTYPE_ATRANSITION,
	TP_CLIPTYPE_HGROUPCLIP,
	TP_CLIPTYPE_XDCAMGROUPCLIP,
	TP_CLIPTYPE_VGROUPCLIP,
	TP_CLIPTYPE_DVDVCD,
	TP_CLIPTYPE_VCD,
	TP_CLIPTYPE_MULTISEQ,
	TP_CLIPTYPE_SYSUDF, 
	TP_CLIPTYPE_MYUSERSEQUENCE,
	TP_CLIPTYPE_OTHERUSERSEQUENCE,
	TP_CLIPTYPE_LINKXDCAM,
	TP_CLIPTYPE_LINKP2,
	TP_CLIPTYPE_LINKXDCAMEX,
};

enum TP_CLIPSTATE_INDEX
{
	TP_CLIPSTATE_CLIPISINUSESTATUS1,
	TP_CLIPSTATE_CLIPISNOTINUSE,
	TP_CLIPSTATE_CLIPISINUSESTATUS2,
	TP_CLIPSTATE_NOHIGHANDPROXYCLIPS, 
	TP_CLIPSTATE_NOHIGHCLIPBUTHASPROXYCLIP,
	TP_CLIPSTATE_NOPROXYCLIPBUTHASHIGHCLIP,
	TP_CLIPSTATE_HASBOTHOFHIGHPROXYCLIPS,
	TP_CLIPSTATE_UNKNOWN,
	TP_CLIPSTATE_INDIGITIZINGUP,
	TP_CLIPSTATE_INDIGITIZINGDOWN,
	TP_CLIPSTATE_SUBHIGHNOLOW,
	TP_CLIPSTATE_SUBHIGHWITHLOW,
};
enum TP_CLIPFLODER_INDEX
{
	TP_CLIPFLODER_BIN,
	TP_CLIPFLODER_PROJECTGROUP,
	TP_CLIPFLODER_SDPROJECT,
	TP_CLIPFLODER_HDPROJECT,
	TP_CLIPFLODER_TRASH,
	TP_CLIPFLODER_DATASOURCE,
	TP_CLIPFLODER_SEARCHRESULT,
	TP_CLIPFLODER_USERCONFIGURE,
	TP_CLIPFLODER_NETWORK,
	TP_CLIPFLODER_CG,
};
enum TP_CLIPUSED_INDEX
{
	TP_CLIPUSED_NOUSEDOK,
	TP_CLIPUSED_NOUSEDNG,
	TP_CLIPUSED_USEDOK,
	TP_CLIPUSED_USEDNG,
};

extern int     TP_NodeTypeHeadIndex(TP_NODE_TYPE eResType);

INT TP_GetNoteTypeIconIndex(TP_RES_TYPE eResType, TP_CLIP_SOURCE_TYPE eSingleSourceType, TPListItemData *pListItemData)
{
	DWORD dwIndex = 0;
	eSingleSourceType &= ~(TP_CLIP_SOURCE_TYPE_DIGITIZE | TP_CLIP_SOURCE_TYPE_INTRODEUCE | TP_CLIP_SOURCE_TYPE_SUBCLIPS);

	if(TP_RES_NOTE & eResType)
	{
		dwIndex = TP_CLIPTYPE_ALPHACOPYCLIP;

		if(TP_CLIP_SOURCE_TYPE_SPEEDCLIPS & eSingleSourceType)
			dwIndex = TP_CLIPTYPE_SPEEDCLIP;
		else if(TP_CLIP_SOURCE_TYPE_UNKNOW == eSingleSourceType)
			dwIndex = TP_CLIPTYPE_ALPHACOPYCLIP;
		else if(TP_CLIP_SOURCE_TYPE_VAVTR & eSingleSourceType)
			dwIndex = TP_CLIPTYPE_REELCLIPDIGITIZEDCLIP;
		else if(TP_CLIP_SOURCE_TYPE_VANOVTR & eSingleSourceType)
			dwIndex = TP_CLIPTYPE_REELCLIPDIGITIZEDCLIP;
		else if(TP_CLIP_SOURCE_TYPE_MXFCLIPS & eSingleSourceType)
			dwIndex = TP_CLIPTYPE_MXFCLIP;
		else if(TP_CLIP_SOURCE_TYPE_FILE & eSingleSourceType)
			dwIndex = TP_CLIPTYPE_ALPHACOPYCLIP;
		else if(TP_CLIP_SOURCE_TYPE_DUB & eSingleSourceType)
			dwIndex = TP_CLIPTYPE_VOICEOVERCLIP;

	}
	else if(eResType & (TP_RES_CATALOG | TP_RES_PROJECT))
	{
		if(NULL == pListItemData) return TP_CLIPTYPE_BIN;
		if(TP_NODE_TRASH & pListItemData->eNodeType)
		{
			dwIndex = TP_CLIPTYPE_TRASH;
		}
		else if(TP_NODE_PROJECT & pListItemData->eNodeType)
		{
			if(TP_CLIPFLODER_SDPROJECT == pListItemData->iNPIndex)
				dwIndex = TP_CLIPTYPE_PROJECTSD;
			else
				dwIndex = TP_CLIPTYPE_PROJECTHD;
		}
		else
		{
			dwIndex = TP_CLIPTYPE_BIN;
		}
	}


	return dwIndex;
}



BOOL TP_IsCurProjectRes(GUID guidProject)
{
//	if(g_guidResProject.guidRes != GUID_NULL && g_guidResProject.guidRes == guidProject) return TRUE;

	return FALSE;
}

int           CTPNSResListCtrl::m_iRefCount = 0;
TPCommonRes  *CTPNSResListCtrl::m_pCommonRes[TP_CLIPICON_MAX] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, NULL,NULL};

enum TP_DICTIONARY_INDEX
{
	TP_DICTIONARY_CLIPNAME,
	TP_DICTIONARY_CLIPLENGTH,
	TP_DICTIONARY_CLIPTYPE,
	TP_DICTIONARY_CLIPCREATETIME,
	TP_DICTIONARY_CLIPUPDATETIME,
	TP_DICTIONARY_MAX,
};
CString g_sDictionaryCache[TP_DICTIONARY_MAX];

LPCTSTR TP_GetDictionaryCache(TP_DICTIONARY_INDEX eIndex)
{
	return (LPCTSTR)g_sDictionaryCache[eIndex];
}

CTPNSResListCtrl::CTPNSResListCtrl(void)
{
	m_iBackColor        =  COLOR_CLIP_LIST;
	m_dwWndType         = TP_WND_RESEXPLORER;
	m_szItemReport      = CSize(20,20);
	m_szItemTextPicture = CSize(20,60);
	m_iLockColumn       = -1;
	m_iLockDragColumn   = -1;
	m_uPrivateStyle    |= LIST_DRAGCOLUMN; 
	m_bDrawFocus        = TRUE;  
	m_bHeadMenu         = TRUE; 
	m_pResExplorer     = NULL;
	m_bFlashIcon		= FALSE;
	m_iArrType			= ARRANGE_AUTO; // XN00008028 LI HUI LI 2008-01-02 Set Init ArrType
	m_bViewFile         = FALSE;
	m_iViewType         = VIEWTYPE_ICON;

	m_iRefCount ++;
	if(m_iRefCount ==1)
	{
		CString sSkinPath = TP_GetSkinPath() + _T("\\ClipExplorer");
		CString sBmp[] = {_T("\\ClipBKExtLarge_Normal.bmp"),   _T("\\ClipBKExtLarge_Selected.bmp"),   _T("\\ClipBKExtLarge_CutReserve.bmp"),  _T("\\ClipBKExtLarge_DeFocus.bmp"),
			_T("\\ClipBKLarge_Normal.bmp"),      _T("\\ClipBKLarge_Selected.bmp"),      _T("\\ClipBKLarge_CutReserve.bmp"),     _T("\\ClipBKLarge_DeFocus.bmp"),
			_T("\\ClipBKMiddle_Normal.bmp"),     _T("\\ClipBKMiddle_Selected.bmp"),     _T("\\ClipBKMiddle_CutReserve.bmp"),    _T("\\ClipBKMiddle_DeFocus.bmp"),
			_T("\\ClipBKSmall_Normal.bmp"),      _T("\\ClipBKSmall_Selected.bmp"),      _T("\\ClipBKSmall_CutReserve.bmp"),     _T("\\ClipBKSmall_DeFocus.bmp"),

			_T("\\ClipBKExtLarge_Normal1.bmp"),   _T("\\ClipBKExtLarge_Selected1.bmp"),   _T("\\ClipBKExtLarge_CutReserve1.bmp"),  _T("\\ClipBKExtLarge_DeFocus1.bmp"),
			_T("\\ClipBKLarge_Normal1.bmp"),      _T("\\ClipBKLarge_Selected1.bmp"),      _T("\\ClipBKLarge_CutReserve1.bmp"),     _T("\\ClipBKLarge_DeFocus1.bmp"),
			_T("\\ClipBKMiddle_Normal1.bmp"),     _T("\\ClipBKMiddle_Selected1.bmp"),     _T("\\ClipBKMiddle_CutReserve1.bmp"),    _T("\\ClipBKMiddle_DeFocus1.bmp"),
			_T("\\ClipBKSmall_Normal1.bmp"),      _T("\\ClipBKSmall_Selected1.bmp"),      _T("\\ClipBKSmall_CutReserve1.bmp"),     _T("\\ClipBKSmall_DeFocus1.bmp"),

			_T("\\ClipBKExtLarge_Normal2.bmp"),   _T("\\ClipBKExtLarge_Selected2.bmp"),   _T("\\ClipBKExtLarge_CutReserve2.bmp"),  _T("\\ClipBKExtLarge_DeFocus2.bmp"),
			_T("\\ClipBKLarge_Normal2.bmp"),      _T("\\ClipBKLarge_Selected2.bmp"),      _T("\\ClipBKLarge_CutReserve2.bmp"),     _T("\\ClipBKLarge_DeFocus2.bmp"),
			_T("\\ClipBKMiddle_Normal2.bmp"),     _T("\\ClipBKMiddle_Selected2.bmp"),     _T("\\ClipBKMiddle_CutReserve2.bmp"),    _T("\\ClipBKMiddle_DeFocus2.bmp"),
			_T("\\ClipBKSmall_Normal2.bmp"),      _T("\\ClipBKSmall_Selected2.bmp"),      _T("\\ClipBKSmall_CutReserve2.bmp"),     _T("\\ClipBKSmall_DeFocus2.bmp"),

			_T("\\ClipBKExtLarge_Normal3.bmp"),   _T("\\ClipBKExtLarge_Selected3.bmp"),   _T("\\ClipBKExtLarge_CutReserve3.bmp"),  _T("\\ClipBKExtLarge_DeFocus3.bmp"),
			_T("\\ClipBKLarge_Normal3.bmp"),      _T("\\ClipBKLarge_Selected3.bmp"),      _T("\\ClipBKLarge_CutReserve3.bmp"),     _T("\\ClipBKLarge_DeFocus3.bmp"),
			_T("\\ClipBKMiddle_Normal3.bmp"),     _T("\\ClipBKMiddle_Selected3.bmp"),     _T("\\ClipBKMiddle_CutReserve3.bmp"),    _T("\\ClipBKMiddle_DeFocus3.bmp"),
			_T("\\ClipBKSmall_Normal3.bmp"),      _T("\\ClipBKSmall_Selected3.bmp"),      _T("\\ClipBKSmall_CutReserve3.bmp"),     _T("\\ClipBKSmall_DeFocus3.bmp"),

			_T("\\ClipBKExtLarge_Normal4.bmp"),   _T("\\ClipBKExtLarge_Selected4.bmp"),   _T("\\ClipBKExtLarge_CutReserve4.bmp"),  _T("\\ClipBKExtLarge_DeFocus4.bmp"),
			_T("\\ClipBKLarge_Normal4.bmp"),      _T("\\ClipBKLarge_Selected4.bmp"),      _T("\\ClipBKLarge_CutReserve4.bmp"),     _T("\\ClipBKLarge_DeFocus4.bmp"),
			_T("\\ClipBKMiddle_Normal4.bmp"),     _T("\\ClipBKMiddle_Selected4.bmp"),     _T("\\ClipBKMiddle_CutReserve4.bmp"),    _T("\\ClipBKMiddle_DeFocus4.bmp"),
			_T("\\ClipBKSmall_Normal4.bmp"),      _T("\\ClipBKSmall_Selected4.bmp"),      _T("\\ClipBKSmall_CutReserve4.bmp"),     _T("\\ClipBKSmall_DeFocus4.bmp"),

			_T("\\ClipBKExtLarge_Normal5.bmp"),   _T("\\ClipBKExtLarge_Selected5.bmp"),   _T("\\ClipBKExtLarge_CutReserve5.bmp"),  _T("\\ClipBKExtLarge_DeFocus5.bmp"),
			_T("\\ClipBKLarge_Normal5.bmp"),      _T("\\ClipBKLarge_Selected5.bmp"),      _T("\\ClipBKLarge_CutReserve5.bmp"),     _T("\\ClipBKLarge_DeFocus5.bmp"),
			_T("\\ClipBKMiddle_Normal5.bmp"),     _T("\\ClipBKMiddle_Selected5.bmp"),     _T("\\ClipBKMiddle_CutReserve5.bmp"),    _T("\\ClipBKMiddle_DeFocus5.bmp"),
			_T("\\ClipBKSmall_Normal5.bmp"),      _T("\\ClipBKSmall_Selected5.bmp"),      _T("\\ClipBKSmall_CutReserve5.bmp"),     _T("\\ClipBKSmall_DeFocus5.bmp"),

			_T("\\ClipBKExtLarge_Normal6.bmp"),   _T("\\ClipBKExtLarge_Selected6.bmp"),   _T("\\ClipBKExtLarge_CutReserve6.bmp"),  _T("\\ClipBKExtLarge_DeFocus6.bmp"),
			_T("\\ClipBKLarge_Normal6.bmp"),      _T("\\ClipBKLarge_Selected6.bmp"),      _T("\\ClipBKLarge_CutReserve6.bmp"),     _T("\\ClipBKLarge_DeFocus6.bmp"),
			_T("\\ClipBKMiddle_Normal6.bmp"),     _T("\\ClipBKMiddle_Selected6.bmp"),     _T("\\ClipBKMiddle_CutReserve6.bmp"),    _T("\\ClipBKMiddle_DeFocus6.bmp"),
			_T("\\ClipBKSmall_Normal6.bmp"),      _T("\\ClipBKSmall_Selected6.bmp"),      _T("\\ClipBKSmall_CutReserve6.bmp"),     _T("\\ClipBKSmall_DeFocus6.bmp"),

			_T("\\ClipBKExtLarge_Normal7.bmp"),   _T("\\ClipBKExtLarge_Selected7.bmp"),   _T("\\ClipBKExtLarge_CutReserve7.bmp"),  _T("\\ClipBKExtLarge_DeFocus7.bmp"),
			_T("\\ClipBKLarge_Normal7.bmp"),      _T("\\ClipBKLarge_Selected7.bmp"),      _T("\\ClipBKLarge_CutReserve7.bmp"),     _T("\\ClipBKLarge_DeFocus7.bmp"),
			_T("\\ClipBKMiddle_Normal7.bmp"),     _T("\\ClipBKMiddle_Selected7.bmp"),     _T("\\ClipBKMiddle_CutReserve7.bmp"),    _T("\\ClipBKMiddle_DeFocus7.bmp"),
			_T("\\ClipBKSmall_Normal7.bmp"),      _T("\\ClipBKSmall_Selected7.bmp"),      _T("\\ClipBKSmall_CutReserve7.bmp"),     _T("\\ClipBKSmall_DeFocus7.bmp"),

			_T("\\ClipBKExtLarge_Normal8.bmp"),   _T("\\ClipBKExtLarge_Selected8.bmp"),   _T("\\ClipBKExtLarge_CutReserve8.bmp"),  _T("\\ClipBKExtLarge_DeFocus8.bmp"),
			_T("\\ClipBKLarge_Normal8.bmp"),      _T("\\ClipBKLarge_Selected8.bmp"),      _T("\\ClipBKLarge_CutReserve8.bmp"),     _T("\\ClipBKLarge_DeFocus8.bmp"),
			_T("\\ClipBKMiddle_Normal8.bmp"),     _T("\\ClipBKMiddle_Selected8.bmp"),     _T("\\ClipBKMiddle_CutReserve8.bmp"),    _T("\\ClipBKMiddle_DeFocus8.bmp"),
			_T("\\ClipBKSmall_Normal8.bmp"),      _T("\\ClipBKSmall_Selected8.bmp"),      _T("\\ClipBKSmall_CutReserve8.bmp"),     _T("\\ClipBKSmall_DeFocus8.bmp"),

			_T("\\ClipBKExtLarge_Normal9.bmp"),   _T("\\ClipBKExtLarge_Selected9.bmp"),   _T("\\ClipBKExtLarge_CutReserve9.bmp"),  _T("\\ClipBKExtLarge_DeFocus9.bmp"),
			_T("\\ClipBKLarge_Normal9.bmp"),      _T("\\ClipBKLarge_Selected9.bmp"),      _T("\\ClipBKLarge_CutReserve9.bmp"),     _T("\\ClipBKLarge_DeFocus9.bmp"),
			_T("\\ClipBKMiddle_Normal9.bmp"),     _T("\\ClipBKMiddle_Selected9.bmp"),     _T("\\ClipBKMiddle_CutReserve9.bmp"),    _T("\\ClipBKMiddle_DeFocus9.bmp"),
			_T("\\ClipBKSmall_Normal9.bmp"),      _T("\\ClipBKSmall_Selected9.bmp"),      _T("\\ClipBKSmall_CutReserve9.bmp"),     _T("\\ClipBKSmall_DeFocus9.bmp"),

			_T("\\ClipBKExtLarge_Normal10.bmp"),   _T("\\ClipBKExtLarge_Selected10.bmp"),   _T("\\ClipBKExtLarge_CutReserve10.bmp"),  _T("\\ClipBKExtLarge_DeFocus10.bmp"),
			_T("\\ClipBKLarge_Normal10.bmp"),      _T("\\ClipBKLarge_Selected10.bmp"),      _T("\\ClipBKLarge_CutReserve10.bmp"),     _T("\\ClipBKLarge_DeFocus10.bmp"),
			_T("\\ClipBKMiddle_Normal10.bmp"),     _T("\\ClipBKMiddle_Selected10.bmp"),     _T("\\ClipBKMiddle_CutReserve10.bmp"),    _T("\\ClipBKMiddle_DeFocus10.bmp"),
			_T("\\ClipBKSmall_Normal10.bmp"),      _T("\\ClipBKSmall_Selected10.bmp"),      _T("\\ClipBKSmall_CutReserve10.bmp"),     _T("\\ClipBKSmall_DeFocus10.bmp"),

			_T("\\ClipBKExtLarge_Normal11.bmp"),   _T("\\ClipBKExtLarge_Selected11.bmp"),   _T("\\ClipBKExtLarge_CutReserve11.bmp"),  _T("\\ClipBKExtLarge_DeFocus11.bmp"),
			_T("\\ClipBKLarge_Normal11.bmp"),      _T("\\ClipBKLarge_Selected11.bmp"),      _T("\\ClipBKLarge_CutReserve11.bmp"),     _T("\\ClipBKLarge_DeFocus11.bmp"),
			_T("\\ClipBKMiddle_Normal11.bmp"),     _T("\\ClipBKMiddle_Selected11.bmp"),     _T("\\ClipBKMiddle_CutReserve11.bmp"),    _T("\\ClipBKMiddle_DeFocus11.bmp"),
			_T("\\ClipBKSmall_Normal11.bmp"),      _T("\\ClipBKSmall_Selected11.bmp"),      _T("\\ClipBKSmall_CutReserve11.bmp"),     _T("\\ClipBKSmall_DeFocus11.bmp"),
		};

		for(int l= TP_CLIPICON_N1;l<=TP_CLIPICON11_D4;l++) m_pCommonRes[l] = TP_LoadBmpByte(sSkinPath + _T("\\ImageList") + sBmp[l-TP_CLIPICON_N1],FALSE,FALSE);
		m_pCommonRes[TP_CLIPICON_OFFLINE]             = TP_LoadBmpByte(sSkinPath + _T("\\DefaultIcon\\Abridge.bmp"),FALSE,FALSE);
		m_pCommonRes[TP_CLIPICON_AUDIO]               = TP_LoadBmpByte(sSkinPath + _T("\\DefaultIcon\\AClip.bmp"),FALSE,FALSE);
		m_pCommonRes[TP_CLIPICON_OFFLINELARGE]        = TP_LoadBmpByte(sSkinPath + _T("\\DefaultIcon\\AbridgeLarge.bmp"),FALSE,FALSE);
		m_pCommonRes[TP_CLIPICON_STATUSINDICATORS]    = TP_LoadImage(sSkinPath   + _T("\\ImageList\\ClipStatusIndicators_20_18.bmp"),   20,    RGB(255,0,255),FALSE,FALSE);
		m_pCommonRes[TP_CLIPICON_STATUSTEAM]          = TP_LoadImage(sSkinPath   + _T("\\ImageList\\ResStatusIndicators_20_18.bmp"),    20,    RGB(255,0,255),FALSE,FALSE);

 		m_pCommonRes[TP_CLIPICON_TYPEINDICATORS]      = TP_LoadImage(sSkinPath + _T("\\ImageList\\ClipTypeIndicators_20_21.bmp"),     20,    RGB(255,0,255),FALSE,FALSE);
		LPBYTE lpByte = m_pCommonRes[TP_CLIPICON_TYPEINDICATORS]->GetByte();
		m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE] = TP_LoadImage(sSkinPath + _T("\\ImageList\\ClipTypeIndicators_20_21.bmp"),     20,    RGB(255,0,255),FALSE,FALSE);
		m_pCommonRes[TP_CLIPICON_OKNG]                = TP_LoadImage(sSkinPath + _T("\\ImageList\\OK_NG.bmp"),                        17,    RGB(0,0,0),FALSE,FALSE);
		m_pCommonRes[TP_CLIPICON_FLODER]              = TP_LoadImage(sSkinPath + _T("\\ImageList\\ListIcons_56_56.bmp"),              56,    RGB(255,0,255),FALSE,FALSE);				
		m_pCommonRes[TP_CLIPICON_ICONOVERLAY]		  = TP_LoadImage(sSkinPath + _T("\\ImageList\\ListIcons_InOutDur.bmp"),           15,    RGB(255,0,255),FALSE,FALSE);
		m_pCommonRes[TP_CLIPICON_DefaultNote]		  = TP_LoadBmpByte(sSkinPath + _T("\\DefaultIcon\\Note.bmp"),    FALSE,FALSE);
		CString stemp1 = SBLoadString(_T("IDS_CLIPEXPLORER_NAME"));
		CString stemp2 = SBLoadString(_T("IDS_CLIPEXPLORER_LENGTH"));
		CString stemp3 = SBLoadString(_T("IDS_CLIPEXPLORER_TYPE"));
		CString stemp4 = SBLoadString(_T("IDS_CLIPEXPLORER_CREATETIME"));
		CString stemp5 = SBLoadString(_T("IDS_CLIPEXPLORER_UPDATETIME"));
		g_sDictionaryCache[TP_DICTIONARY_CLIPNAME]			  = stemp1;
		g_sDictionaryCache[TP_DICTIONARY_CLIPLENGTH]		  = stemp2;
		g_sDictionaryCache[TP_DICTIONARY_CLIPTYPE]		      = stemp3;
		g_sDictionaryCache[TP_DICTIONARY_CLIPCREATETIME]	  = stemp4;
		g_sDictionaryCache[TP_DICTIONARY_CLIPUPDATETIME]	  = stemp5;
	}	
	int iIndex = TP_CLIPICON_N1;
	if(m_lIconSize == 4)       iIndex = TP_CLIPICON_N4;
	else if(m_lIconSize == 3)  iIndex = TP_CLIPICON_N3;
	else if(m_lIconSize == 2)  iIndex = TP_CLIPICON_N2;
	else if(m_lIconSize == 1)  iIndex = TP_CLIPICON_N1;
	m_szItemIcon = CSize(m_pCommonRes[iIndex] ->Width(),m_pCommonRes[iIndex] ->Height());
	m_iCreateCatalogType = 0;
	m_hSelTimer          = NULL;  
	m_uFlashTimer		 = 0;
	m_uResFilter         = 0x7fffffffffffffff;

	m_pGifProxyView		 = NULL;
	m_hResProxyView		 = NULL;
    m_guidProjectRev     = GUID_NULL;
	m_dwTimeCodeType	 = 0;

	m_pDropTarget = NULL;
}
CTPNSResListCtrl::~CTPNSResListCtrl(void)
{
	m_iRefCount --;
	if(m_iRefCount ==0)
	{
		for(INT l=0;l<TP_CLIPICON_MAX;l++) 
		{
			TP_ReleaseCommonRes(m_pCommonRes[l]);
			m_pCommonRes[l] = NULL;
		}
	}
	TP_RemoveResDataArray(m_aDropData, TRUE);
}

void CTPNSResListCtrl::GetItemResDataInfo(TPListItemData *pItemData,TP_RES_TYPE eResType,void *pResData,VIDEO_TYPE eVideoType)
{
//	BOOL    bUsed = TP_IsResUsed(pItemData->iNPIndex, pItemData->lRefCount);
//
	if(eResType & TP_RES_NOTE)
	{
		TPNoteData *pstuGetClipData = (TPNoteData *)pResData;
		BOOL bUseOrginalTimeCode = FALSE;
		INT64 lStart(0), lEnd(0), lTrimIn(0), lTrimOut(0), lDuration(0);

		DWORD dwTimeCode = bUseOrginalTimeCode;

		if(lEnd > 0)	 lEnd     = lEnd - 1;
		if(lTrimOut > 0) lTrimOut = lTrimOut - 1;

		pItemData ->sText[TP_RESTEXT_CLIP_NAME]      = pstuGetClipData->sName;
//		pItemData ->sText[TP_RESTEXT_CLIP_TYPE]		 = g_pClipManageInterface->stuClipInterfce.TP_GetNoteSourceType(pstuGetClipData->eSignalSourceType, pstuGetClipData->uClipFlag);
//		pItemData ->sText[TP_RESTEXT_REEL_NAME]      = pstuGetClipData->sSignalSourceMark;
//		pItemData ->sText[TP_RESTEXT_MARK_IN]        = TP_StringFromFrame(eVideoType,lTCTrimIn);
//		pItemData ->sText[TP_RESTEXT_MARK_OUT]       = TP_StringFromFrame(eVideoType,lTCTrimOut);
//		pItemData ->sText[TP_RESTEXT_DURATION]       = TP_StringFromFrame(eVideoType,lDuration);
//		pItemData ->sText[TP_RESTEXT_FTC]			 = TP_StringFromFrame(eVideoType,pstuGetClipData->lFTCStart);
//		pItemData ->sText[TP_RESTEXT_VITCIN]         = TP_StringFromFrame(eVideoType,lVITCIn);
//		pItemData ->sText[TP_RESTEXT_VITCOUT]        = TP_StringFromFrame(eVideoType,lVITCOut);
//		pItemData ->sText[TP_RESTEXT_VTRINFO]        = TP_GetVtrInfo(pstuGetClipData->eSignalSourceType,pstuGetClipData->eUseVTR);   
//
//		CString sDigitize=_T("");
//		if(pstuGetClipData->eSignalSourceType & TP_CLIP_SOURCE_TYPE_DIGITIZE)	sDigitize= SBLoadString(_T("IDS_CLIPEXPLORER_DONE"));
//		pItemData ->sText[TP_RESTEXT_DIGITIZE]		 = sDigitize;
//		pItemData ->sText[TP_RESTEXT_LTCIN]          = TP_StringFromFrame(eVideoType,lLTCIn);
//		pItemData ->sText[TP_RESTEXT_LTCOUT]         = TP_StringFromFrame(eVideoType,lLTCOut);
//		pItemData ->sText[TP_RESTEXT_START]          = TP_StringFromFrame(eVideoType,lTCStart);
//		pItemData ->sText[TP_RESTEXT_END]            = TP_StringFromFrame(eVideoType,lTCEnd);
//
//		//BEGIN	2010-01-25 15:28:14	CD2009.01.01 V4.5Feature	LiuGang	LTC 這裡附近有一塊ifdef被我刪掉了，使用這一塊
//#ifdef TP_NS_NET
//
//		//if (pstuGetClipData->dwSourceType == E7_CLIPSOURCE_RECORD || pstuGetClipData->dwSourceType == E7_CLIPSOURCE_NLE)
//		//{
//		//	pItemData ->sText[TP_RESTEXT_START]      = TP_StringFromFrame(eVideoType,pstuGetClipData->dwVTRInPoint);
//		//	pItemData ->sText[TP_RESTEXT_END]        = TP_StringFromFrame(eVideoType,pstuGetClipData->dwVTROutPoint);
//		//}
//		pItemData ->sText[TP_RESTEXT_DIGITIZE]		 = pstuGetClipData->eSignalSourceType & TP_CLIP_SOURCE_TYPE_LOGGING ? _T("") : _T("Done");
//
//#endif
//		//END	2010-01-25 15:28:14	LiuGang
//
//		pItemData ->sText[TP_RESTEXT_CLIP_LENGTH]    = TP_StringFromFrame(eVideoType,lCliplength);
//		CString sScene = pstuGetClipData->sScene, sTake = pstuGetClipData->sLens;
//		if(sScene.IsEmpty())						   sScene = _T("0000/");
//		else										   sScene.Format(_T("%04d/"),StrToInt(sScene));
//		if(sTake.IsEmpty())							   sTake  = _T("0000/");
//		else										   sTake.Format(_T("%04d/"), StrToInt(sTake));
//		pItemData ->sText[TP_RESTEXT_SCENE_TAKE]     = sScene + sTake;
//		pItemData ->sText[TP_RESTEXT_COMMENT]        = pstuGetClipData->sDescription;
//		pItemData ->sText[TP_RESTEXT_CREATE_DATE]    =  TP_GetTimeString(pstuGetClipData->tmCreate);
//		pItemData ->sText[TP_RESTEXT_UPDATE_DATE]    =  TP_GetTimeString(pstuGetClipData->tmLastModify);
//		if(pstuGetClipData->uClipFlag & TP_CLIPFLAG_CREATETIME)
//			  pItemData->sText[TP_RESTEXT_RECDATE]	  =  TP_GetTimeString(pstuGetClipData->tmRecord);
//		else 
//			  pItemData->sText[TP_RESTEXT_RECDATE]	  = _T("");
//
//		if(pstuGetClipData->eSignalSourceType & TP_CLIP_SOURCE_TYPE_SUBCLIPS)  	pItemData ->sText[TP_RESTEXT_SUB_CLIPS] = _T("TRUE");
//		else																	pItemData ->sText[TP_RESTEXT_SUB_CLIPS] = _T("");
//		pItemData ->sText[TP_RESTEXT_PROXY]          =  (pstuGetClipData->aQuality[1].eClipClass!=0) && !(pstuGetClipData->uClipFlag & TP_CLIPFLAG_ABRIDGEL) ? _T("Done") : _T("");
//		pItemData ->sText[TP_RESTEXT_RIGHT]          =  pstuGetClipData->sRights;
//
//		BOOL    bFound = FALSE;
//		CString sTemp  = _T("");
//		for(int j = TP_CLIP_QUALITY_HIGH; j <= TP_CLIP_QUALITY_LOW; j++)
//		{
//			for(DWORD i = TP_CLIP_FILE_VIDEO; i < TP_CLIP_FILE_A8; i++)
//			{
//				if(pstuGetClipData->aQuality[j].aQualityItem[i].GetSize() > 0)
//				{
//					sTemp = pstuGetClipData->aQuality[j].aQualityItem[i][0]->sDataFilePath;
//					sTemp += "\\";
//					sTemp += pstuGetClipData->aQuality[j].aQualityItem[i][0]->sDataFileName;
//					bFound = TRUE;
//					break;
//				}
//			}
//			if(bFound) break;
//		}
//		pItemData ->sText[TP_RESTEXT_SOURCE_NAME]                        =  sTemp;
//		pItemData ->sText[TP_RESTEXT_MEDIA_FILE_LOCATION_VIDEO]          =  pstuGetClipData->aQuality[0].aQualityItem[0].GetSize()>0? pstuGetClipData->aQuality[0].aQualityItem[0][0] ->sDataFilePath : (pstuGetClipData->aQuality[TP_CLIP_QUALITY_LOW].aQualityItem[0].GetSize()>0? pstuGetClipData->aQuality[TP_CLIP_QUALITY_LOW].aQualityItem[0][0] ->sDataFilePath:_T(""));
//		pItemData ->sText[TP_RESTEXT_MEDIA_FILE_LOCATION_AUDIO]          =  pstuGetClipData->aQuality[0].aQualityItem[1].GetSize()>0? pstuGetClipData->aQuality[0].aQualityItem[1][0] ->sDataFilePath : (pstuGetClipData->aQuality[TP_CLIP_QUALITY_LOW].aQualityItem[1].GetSize()>0? pstuGetClipData->aQuality[TP_CLIP_QUALITY_LOW].aQualityItem[1][0] ->sDataFilePath:_T(""));
//		pItemData ->sText[TP_RESTEXT_USED]								 = bUsed ? SBLoadString(_T("IDS_CLIPEXPLORER_USED")) : _T("");
//
//		sTemp = _T("");
//		if(pstuGetClipData->aQuality[0].eClipClass & TP_CLIP_CLASS_V)
//			sTemp.Format(_T("%d * %d"), pstuGetClipData->aQuality[0].stuClipVAFormat.stuVideoFormat.tpImageFormat.iWidth, pstuGetClipData->aQuality[0].stuClipVAFormat.stuVideoFormat.tpImageFormat.iHeight);
//		else if(pstuGetClipData->aQuality[1].eClipClass & TP_CLIP_CLASS_V)
//			sTemp.Format(_T("%d * %d"), pstuGetClipData->aQuality[1].stuClipVAFormat.stuVideoFormat.tpImageFormat.iWidth, pstuGetClipData->aQuality[1].stuClipVAFormat.stuVideoFormat.tpImageFormat.iHeight);
//
//		pItemData ->sText[TP_RESTEXT_FRAME_SIZE]                         = sTemp;
//
//		sTemp = _T("");
//		DWORD dwFileType = 0;		
//		INT64 lAllClipType = pstuGetClipData->aQuality[0].eClipClass|pstuGetClipData->aQuality[1].eClipClass;
//		INT64 lAllDBEType  = pstuGetClipData->aQuality[0].eDBEClass | pstuGetClipData->aQuality[1].eDBEClass;
//
//		CString sFormat;
//		BOOL bHasDBE = lAllDBEType ? TRUE : FALSE;
//		if(pstuGetClipData->aQuality[0].eClipClass & TP_CLIP_CLASS_V)
//		{
//			sFormat = TP_GetVideoFormatTitle(pstuGetClipData->aQuality[0].stuClipVAFormat.stuVideoFormat, TP_CLIP_QUALITY_HIGH, pstuGetClipData->eSignalSourceType, pstuGetClipData->guidDevice);
//		}
//		else if(pstuGetClipData->aQuality[1].eClipClass & TP_CLIP_CLASS_V)
//		{
//			sFormat = TP_GetVideoFormatTitle(pstuGetClipData->aQuality[1].stuClipVAFormat.stuVideoFormat, TP_CLIP_QUALITY_LOW, pstuGetClipData->eSignalSourceType, pstuGetClipData->guidDevice);
//		}
//		else if(pstuGetClipData->aQuality[0].eClipClass & TP_CLIP_CLASS_A)
//			sFormat = TP_GetAudioFormatTitle(pstuGetClipData->aQuality[0].stuClipVAFormat.stuAudioFormat, bHasDBE);
//		else
//			sFormat = TP_GetAudioFormatTitle(pstuGetClipData->aQuality[1].stuClipVAFormat.stuAudioFormat, bHasDBE);
//
//		pItemData ->sText[TP_RESTEXT_CODEC]          = sFormat;
//
//		if(lAllClipType & TP_CLIP_CLASS_V) // 有视频
//		{
//			if((pstuGetClipData->dwFileType &  (TP_MEDIA_CLASS_V_SERIAL | TP_MEDIA_CLASS_V_P)) && (!(TP_CLIP_SOURCE_TYPE_SG & pstuGetClipData->eSignalSourceType)))
//			{
//				INT iWidth  = 0; INT iHeight = 0;
//				if(pstuGetClipData->aQuality[TP_CLIP_QUALITY_HIGH].eClipClass & TP_MEDIA_CLASS_V)
//				{
//					iWidth  = pstuGetClipData->aQuality[TP_CLIP_QUALITY_HIGH].stuClipVAFormat.stuVideoFormat.tpImageFormat.iWidth;
//					iHeight = pstuGetClipData->aQuality[TP_CLIP_QUALITY_HIGH].stuClipVAFormat.stuVideoFormat.tpImageFormat.iHeight;
//				}
//				else if(pstuGetClipData->aQuality[TP_CLIP_QUALITY_LOW].eClipClass & TP_MEDIA_CLASS_V)
//				{
//					iWidth  = pstuGetClipData->aQuality[TP_CLIP_QUALITY_LOW].stuClipVAFormat.stuVideoFormat.tpImageFormat.iWidth;
//					iHeight = pstuGetClipData->aQuality[TP_CLIP_QUALITY_LOW].stuClipVAFormat.stuVideoFormat.tpImageFormat.iHeight;
//				}
//
//				if(iWidth == 1920 && (iHeight == 1080))
//				{
//					sTemp = _T("16:9");
//				}
//				else if(TP_IsSDWidthAndHeight(0,iWidth,iHeight))
//				{
//					if(pstuGetClipData->uClipFlag & TP_CLIPFLAG_1609) sTemp = _T("16:9");
//					else											  sTemp = _T("4:3");
//				}
//			}
//			else
//			{
//				TP_VIDEO_STANDARD eVideoStandard = pstuGetClipData->eVideoStandard;
//				if(pstuGetClipData-> uClipFlag & (TP_CLIPFLAG_1609SD | TP_CLIPFLAG_1609SQ | TP_CLIPFLAG_1609SG))
//					eVideoStandard = (TP_VIDEO_STANDARD)TP_GetVideoStandard1609(eVideoStandard, TRUE);
//
//				TP_ASPECTRATIO Ratio = TP_GetAspectRatioFromVideoStandard(eVideoStandard);
//				if(TP_AspectRatio_16_9 == Ratio)	 sTemp = _T("16:9");
//				else if(TP_AspectRatio_4_3 == Ratio) sTemp = _T("4:3");
//			}
//		}
//
//		pItemData ->sText[TP_RESTEXT_FRAME_ASPECT_RATIO]                =  sTemp;
//		pItemData ->VAString                                            = TP_GetVAString(lAllClipType,lAllDBEType, dwFileType);
//		pItemData ->sText[TP_RESTEXT_STATUS]							= (pstuGetClipData->uClipFlag & TP_CLIPFLAG_NG) ? _T("NG") : _T("OK");
//		pItemData->sText[TP_RESTEXT_TRACK]								= TP_GetTrackString(lAllClipType, lAllDBEType);
//		pItemData->sText[TP_RESTEXT_CLIP_ID]							= TP_GuidToString(&pstuGetClipData->guidRes);
	}
//	else if(eResType & TP_RES_ALLPROGRAM)
//	{
//		TPProgramData *pstuGetProgramData   = (TPProgramData *)pResData;
//
//		INT64 lTrimIn(-1), lTrimOut(-1), lStart(-1), lEnd(-1), lDuration(-1), lLength(-1);
//		lTrimIn		 = pstuGetProgramData->lInPoint;
//		lTrimOut	 = pstuGetProgramData->lOutPoint;
//		lStart		 = pstuGetProgramData->lStart;
//		lEnd		 = pstuGetProgramData->lEnd;
//		if(lEnd > 0) lEnd = lEnd - 1;
//		if(lTrimOut > 0) lTrimOut = lTrimOut - 1; 
//
//		lLength = TP_GetNoteDuration(lStart, lEnd, lStart, lEnd);
//		lDuration = TP_GetNoteDuration(lStart, lEnd, lTrimIn, lTrimOut);  
//
//		pItemData ->sText[TP_RESTEXT_CLIP_NAME]      = pstuGetProgramData->sName;
//		//{liubo add CD2009.10.01 2010-6-24 Material ID
//#ifdef TP_NS_NET
//		pItemData ->sText[TP_RESTEXT_SONAPS_MATERIALID] = _T("");
//#endif
//		//}
//		pItemData ->sText[TP_RESTEXT_CLIP_TYPE]		 = g_pClipManageInterface->stuClipInterfce.TP_GetNoteSourceType(TP_CLIP_SOURCE_TYPE_SEQUENCE, 0);
//		pItemData ->sText[TP_RESTEXT_REEL_NAME]      = _T("");
//		pItemData ->sText[TP_RESTEXT_DIGITIZE]       = _T("");
//		TCHAR szTemp[MAX_PATH];
//		ZeroMemory(szTemp, sizeof(TCHAR) * MAX_PATH);
//		g_pFileManageInterface->TPFile_GetVideoFormatTitle(pstuGetProgramData->stuVideoFormat, szTemp);
//
//		pItemData ->sText[TP_RESTEXT_CODEC]          = szTemp;
//		pItemData ->sText[TP_RESTEXT_MARK_IN]        = TP_StringFromFrame(eVideoType,lTrimIn);
//		pItemData ->sText[TP_RESTEXT_MARK_OUT]       = TP_StringFromFrame(eVideoType,lTrimOut);
//		pItemData ->sText[TP_RESTEXT_DURATION]       = TP_StringFromFrame(eVideoType,lDuration);
//		pItemData ->sText[TP_RESTEXT_START]          = TP_StringFromFrame(eVideoType,lStart);
//		pItemData ->sText[TP_RESTEXT_END]            = TP_StringFromFrame(eVideoType,lEnd);
//		pItemData ->sText[TP_RESTEXT_CLIP_LENGTH]    = TP_StringFromFrame(eVideoType,lLength);
//		pItemData ->sText[TP_RESTEXT_FTC]			 = TP_StringFromFrame(eVideoType,(INT64)pstuGetProgramData->lStart);
//		pItemData ->sText[TP_RESTEXT_LTCIN]          = TP_StringFromFrame(eVideoType,(INT64)-1);
//		pItemData ->sText[TP_RESTEXT_LTCOUT]         = TP_StringFromFrame(eVideoType,(INT64)-1);
//		pItemData ->sText[TP_RESTEXT_VITCIN]         = TP_StringFromFrame(eVideoType,(INT64)-1);
//		pItemData ->sText[TP_RESTEXT_VITCOUT]        = TP_StringFromFrame(eVideoType,(INT64)-1);
//
//		pItemData ->sText[TP_RESTEXT_SCENE_TAKE]     = _T("0000/0000/");
//
//		DWORD dwClipClass = pstuGetProgramData->eClipClassType & 0xffffffff;
//		DWORD dwDBEClass  = pstuGetProgramData->eClipClassType >> 32;
//
//		pItemData ->sText[TP_RESTEXT_TRACK]          = TP_GetTrackString(dwClipClass, dwDBEClass);
//		pItemData ->sText[TP_RESTEXT_COMMENT]        = pstuGetProgramData->sDescription;
//		pItemData ->sText[TP_RESTEXT_CREATE_DATE]    =  TP_GetTimeString(pstuGetProgramData->tmCreate);
//		pItemData ->sText[TP_RESTEXT_UPDATE_DATE]    =  TP_GetTimeString(pstuGetProgramData->tmLastModify);
//
//		pItemData ->sText[TP_RESTEXT_SUB_CLIPS]      =  _T("");
//		pItemData ->sText[TP_RESTEXT_PROXY]          =  _T("");
//		pItemData ->sText[TP_RESTEXT_RIGHT]          =  pstuGetProgramData->sRights;
//
//
//		pItemData ->sText[TP_RESTEXT_SOURCE_NAME]                        =  _T("");
//		pItemData ->sText[TP_RESTEXT_MEDIA_FILE_LOCATION_VIDEO]          =  _T("");;
//		pItemData ->sText[TP_RESTEXT_MEDIA_FILE_LOCATION_AUDIO]          =  _T("");;
//
//		pItemData ->sText[TP_RESTEXT_FRAME_SIZE].Format(_T("%d * %d"),TP_GetSysVideoWidth(pstuGetProgramData->eVideoStandard),TP_GetSysVideoHeight(pstuGetProgramData->eVideoStandard));
//		pItemData ->sText[TP_RESTEXT_FRAME_ASPECT_RATIO]                 =  pstuGetProgramData->eVideoStandard <= TP_VIDEOSTANDARD_SECAM ? _T("4:3") : _T("16:9");//stuTimeBase.lTimeRate == 25?_T("25"):(stuTimeBase.lTimeRate == 30?_T("30"):_T("29.97"));
//
//		pItemData->sText[TP_RESTEXT_CLIP_ID]							 = TP_GuidToString(&pstuGetProgramData->guidRes);
//		pItemData->sText[TP_RESTEXT_STATUS]								 = SBLoadString(_T("IDS_CLIPEXPLORER_OKSTATUS"));
//		pItemData ->sText[TP_RESTEXT_USED]								 = bUsed ? SBLoadString(_T("IDS_CLIPEXPLORER_USED")) : _T("");
//
//	}
//	else if(eResType & TP_RES_ALLTEMPLATE)
//	{
//		TPTemplateData *pstuTemplateData = (TPTemplateData *)pResData;
//
//		if((TP_RES_ALLCGTEMPLATE|TP_RES_ALLPREFABRICATE) & eResType) /// CG|User Perfabricate|Sys Perfabricate
//		{
//			pItemData ->sText[TP_TEMPALTECGTEXT_NAME]				= pstuTemplateData->sName;	
//			pItemData ->sText[TP_TEMPALTECGTEXT_COMMENT]			= pstuTemplateData->sDescription;	
//			pItemData ->sText[TP_TEMPALTECGTEXT_CONFIGURATIONTIME]  = TP_GetTimeString(pstuTemplateData->tmCreate);
//			pItemData ->sText[TP_TEMPALTECGTEXT_ALTERATIONTIME]		= TP_GetTimeString(pstuTemplateData->tmLastModify);
//			pItemData ->sText[TP_TEMPALTECGTEXT_DURATION]			= TP_StringFromFrame(eVideoType,pstuTemplateData->lLength);	
//		}
//		else // FX
//		{
//			pItemData ->sText[TP_TEMPALTETEXT_NAME]      = pstuTemplateData->sName;	
//			pItemData ->sText[TP_TEMPALTETEXT_TYPE]      = _T("");	
//			pItemData ->sText[TP_TEMPALTETEXT_CATEGORY]  = _T("");	
//			pItemData ->sText[TP_TEMPALTETEXT_ORIGINAL]  = _T("");	
//			pItemData ->sText[TP_TEMPALTETEXT_DURATION]  = TP_StringFromFrame(eVideoType,pstuTemplateData->lLength);	
//			pItemData ->sText[TP_TEMPALTETEXT_ALIGNMENT] = _T("");	
//		}
//	}
//	else if(eResType & TP_RES_PROJECT)
//	{
//		TPProjectData *pProjectData = (TPProjectData*)pResData;
//
//		CString sAspectRation = _T("");
//		TP_ASPECTRATIO Ratio = TP_GetAspectRatioFromVideoStandard(pProjectData->eVideoStandard);
//		if(TP_AspectRatio_16_9 == Ratio)		sAspectRation = _T("16:9");
//		else if(TP_AspectRatio_4_3 == Ratio)	sAspectRation = _T("4:3");
//		else									sAspectRation = _T("4:3");
//
//		TCHAR szFormat[MAX_PATH];
//		ZeroMemory(szFormat, sizeof(TCHAR) * MAX_PATH);
//		if(g_pFileManageInterface ->TPFile_GetVideoFormatTitle)
//			g_pFileManageInterface ->TPFile_GetVideoFormatTitle(pProjectData->stuCompression.stuVideoFormat,szFormat);
//
//		pItemData ->sText[TP_PROJECTTEXT_NAME]          = pProjectData->sName;	
//		pItemData ->sText[TP_PROJECTTEXT_MODIFYTIME]    = TP_GetTimeString(pProjectData->tmLastModify);	
//		pItemData ->sText[TP_PROJECTTEXT_VIDEOFORMAT]	= szFormat;	
//		pItemData ->sText[TP_PROJECTTEXT_ASPECTRATION]  = sAspectRation;	
//		pItemData ->sText[TP_PROJECTTEXT_STATUS]		= SBLoadString(_T("IDS_CLIPEXPLORER_AVAIL"));
//	}
}
void CTPNSResListCtrl::GetItemResClipFile(TPListItemData *pItemData)
{
	if(pItemData == NULL)    return;
	if(pItemData ->bGetFile) return;
	pItemData ->bGetFile = TRUE;
	if(pItemData ->eResType & (TP_NODE_TRASH)){}
	else if(pItemData ->eResType & TP_RES_NOTE)
	{
		BOOL bDefaultNote = TRUE;
		if(pItemData->lListItemFlag  & TP_ITEMFLAG_DEFAULTNOTE)	bDefaultNote = TRUE;
			// 计算素材Icon文件名
		//	INT64 lClipIn       = pItemData ->iFrame;
		//	LPTSTR sName = NULL, sKey = NULL; long lFileIn = 0;TPClipVAFormat *pVAFmt = NULL; 
		//	if(lClipIn == pItemData ->lLength) lClipIn--;
		//	if(!bDefaultNote 
		//		&& S_OK == g_pClipManageInterface ->stuClipInterfce.TP_GetNoteIconFile(pItemData ->hResData,TP_CLIP_STRATEGY_HIGH,TP_CLIP_QUALITY_HIGH,TP_CLIP_CLASS_V,(long)lClipIn,lFileIn,sName,sKey,&pVAFmt)
		//		&& PathFileExists(sName))
		//	{
		//	   	pItemData ->sFileName       = sName;
		//		pItemData ->sKeyFileName    = sKey;
		//		pItemData ->iFrame          = lFileIn;
		//		pItemData ->bGetBuffer      = FALSE;
		//	}
		//	else
		//	{
		//		pItemData ->sFileName       = _T("");
		//		pItemData ->sKeyFileName    = _T("");	
		//		// LI HUI LI 2008-08-06 因为采集指定图标需要显示一致,所以做一下特殊处理
		//		if((TP_CLIP_SOURCE_TYPE_VTR & pItemData ->eSingSourceType) && (pItemData->iFrame < 0))
		//			pItemData->iFrame = -1;
		//		else if(!TP_IsClipUploading(pItemData->dwCaptureFlag))
		//			bDefaultNote = TRUE;
		//	}	
		//	pItemData->lListItemFlag |= TP_ITEMFLAG_VIDEO;
		//	if(NULL == pItemData->pIcon) bDefaultNote = TRUE;
		//	// 如果素材正在上载，显示MediaOffline(Brd， XDCAN, EX 上载过程中需要显示图标)
		//	if(!bDefaultNote && TP_IsClipUploading(pItemData->dwCaptureFlag) && !(pItemData->dwCaptureFlag & TP_CLIPCAPTURE_EDIICON) && !(pItemData ->eSingSourceType & TP_CLIP_SOURCE_TYPE_DIGEQUIPMENT))
		//		bDefaultNote = TRUE;
		//}
		if(bDefaultNote)// Show Media OffLine Icon
		{
			pItemData->ReleaseIconBuf();
			pItemData ->bDelIcon      = FALSE;
			pItemData ->dwState      |= TP_RESSTATE_OVERTURNICON;
			pItemData ->pIcon         = m_pCommonRes[TP_CLIPICON_OFFLINE] ->GetByte();
			pItemData ->szIcon        = CSize(m_pCommonRes[TP_CLIPICON_OFFLINE]->Width(),m_pCommonRes[TP_CLIPICON_OFFLINE]->Height());
			pItemData->lListItemFlag |= TP_ITEMFLAG_DEFAULTNOTE;

			pItemData ->sFileName  = _L(""); 
			pItemData ->bGetBuffer = FALSE;
		}
		else 		
			pItemData->lListItemFlag &= ~TP_ITEMFLAG_DEFAULTNOTE;

		//TPNoteData stuGetClipData;	 
		//if(g_pClipManageInterface ->stuClipInterfce.TP_GetNoteInfo(pItemData ->hResData, TP_GRADE_BASE  | TP_GRADE_FILE,stuGetClipData) == S_OK)
		//{
		//	if(pItemData ->sText[TP_RESTEXT_SOURCE_NAME].IsEmpty())
		//	{
		//		BOOL    bFound = FALSE;
		//		CString sTemp  = _T("");
		//		for(int j = TP_CLIP_QUALITY_HIGH; j <= TP_CLIP_QUALITY_LOW; j++)
		//		{
		//			for(DWORD i = TP_CLIP_FILE_VIDEO; i < TP_CLIP_FILE_A8; i++)
		//			{
		//				if(stuGetClipData.aQuality[j].aQualityItem[i].GetSize() > 0)
		//				{
		//					sTemp = stuGetClipData.aQuality[j].aQualityItem[i][0]->sDataFilePath;
		//					sTemp += "\\";
		//					sTemp += stuGetClipData.aQuality[j].aQualityItem[i][0]->sDataFileName;
		//					bFound = TRUE;
		//					break;
		//				}
		//			}
		//			if(bFound) break;
		//		}
		//		pItemData ->sText[TP_RESTEXT_SOURCE_NAME]                        =  sTemp;
		//	}
		//}
	}

}

void TP_GetRecycleIndex(HRESDATA hCatalog,TPListItemData *pItemData)
{
//	if(pItemData ->bGetIndex) return;
//	TPRecycleResData stuRecycleData;
//	stuRecycleData.eResType  = TP_RES_UNKNOW;
//	stuRecycleData.eNodeType = TP_NODE_UNKNOW;
//	if(g_pClipManageInterface->stuFuncitonInterface.TP_GetRes(hCatalog,pItemData->guidRes,pItemData->guidNode, stuRecycleData)==S_OK)
//	{
//		//if(lstrlen(stuRecycleData.sName)<=0) return;
//		pItemData ->ReleaseIconBuf(); // 需要释放以前的IconBuffer
//		pItemData ->dwBookIndex   = 0;
//		pItemData ->eNodeType     = stuRecycleData.eNodeType | TP_NODE_RECYCLE;
//		pItemData ->eResType      = stuRecycleData.eResType;
//		pItemData->guidRes        = stuRecycleData.guidRes;
//
//		pItemData->guidDBType     = pItemData->guidDBType;
//		pItemData->guidRecyleNode = stuRecycleData.guidCatalog; 
//		pItemData ->sFileName     = stuRecycleData.sFilePath;
//		pItemData ->dwBookIndex   = stuRecycleData.dwBookIndex;
//		pItemData ->tmCreateTime  = stuRecycleData.tmDelete;
//
//
//#ifdef TP_NS_NET
//		pItemData ->pIcon         = stuRecycleData.lpIcon;						
//		pItemData ->szIcon        = stuRecycleData.szIcon;
//		pItemData ->bDelIcon      = FALSE;	
//		pItemData ->dwState      |= TP_RESSTATE_OVERTURNICON;
//		pItemData ->sText[TP_TRASH_NAME] = stuRecycleData.sName;
//#else
//		pItemData ->bDelIcon      = TRUE;
//#endif
//		if(pItemData ->pItem->lIconIndex < 0)
//		{
//			pItemData ->pItem->lIconIndex                = stuRecycleData.lIconIndex;
//			pItemData ->lListIndex[0]					 = stuRecycleData.lIconIndex;
//		}
//		if(pItemData ->pItem->lReportIndex < 0)
//		{
//			pItemData ->pItem->lReportIndex              = stuRecycleData.lReportIndex;
//			pItemData ->lListIndex[1]					 = stuRecycleData.lReportIndex;
//		}
//
//		pItemData ->sText[TP_TRASH_CREATEUSER]      = stuRecycleData.sFile;
//		pItemData ->sText[TP_TRASH_DELETEDATA]		= TP_GetTimeString(stuRecycleData.tmDelete);														
//		pItemData ->sText[TP_TRASH_DELETEUSER]      = stuRecycleData.sDelBy;
//		pItemData ->sText[M2-1]                     = stuRecycleData.sFilePath;
//	}
//	else
//		pItemData ->sText[0] = _T("");
//
//	pItemData ->bGetIndex = TRUE;
}
void  CTPNSResListCtrl::GetItemResIndex(TPListItemData *pItemData)
{

	//if(pItemData == NULL)    return;
	//if(m_eNodeType & TP_NODE_TRASH)
	//{
	//	TP_GetRecycleIndex(m_hCatalog,pItemData);
	//}
	//else if((pItemData ->eResType & (TP_RES_USER|TP_RES_ALLVAEFFECT | TP_RES_CGEFFECT)))
	//{
	//	GetItemResData(pItemData);	 
	//}	  
	//else if(pItemData ->eResType &( TP_RES_NOTE|TP_RES_ALLPROGRAM|TP_RES_ALLTEMPLATE|TP_RES_PROJECT | TP_RES_CATALOG))//判断pItemData
	//{
	//	TPResViewIndex stuIndex;
	//	if(pItemData ->hResData == NULL) 
	//	{
	//		if(pItemData ->eResType  & (TP_RES_PROJECT | TP_RES_CATALOG))//判断pItemData
	//		g_pClipManageInterface ->stuCatalogInterface.TP_GetCatalog( pItemData ->guidDBType ,GUID_NULL,pItemData ->guidRes, pItemData ->hResData);
	//		else if(pItemData ->eResType &   TP_RES_NOTE)//判断pItemData
	//			g_pClipManageInterface ->stuClipInterfce.TP_GetNote( pItemData ->guidDBType ,GUID_NULL,pItemData ->guidRes,TP_RESLOCK_VIEW,pItemData ->hResData);
	//		else if(pItemData ->eResType  &  TP_RES_ALLPROGRAM)//判断pItemData
	//			g_pClipManageInterface ->stuProgramInterface.TP_GetProgram( pItemData ->guidDBType ,GUID_NULL,pItemData ->guidRes,TP_RESLOCK_VIEW,pItemData ->hResData);
	//		else if(pItemData ->eResType  &  TP_RES_ALLTEMPLATE)//判断pItemData
	//			g_pClipManageInterface ->stuTemplateInterface.TP_GetTemplate( pItemData ->guidDBType ,GUID_NULL,pItemData ->guidRes,TP_RESLOCK_VIEW,pItemData ->hResData);
	//	}
	//	if(g_pClipManageInterface -> stuCatalogInterface.TP_GetResIndex(pItemData ->hResData ,stuIndex) == S_OK)
	//	{
	//		if(pItemData ->pItem->lIconIndex < 0)
	//			pItemData ->pItem->lIconIndex    = stuIndex.lIconIndex;
	//		if(pItemData ->pItem->lReportIndex < 0)
	//			pItemData ->pItem->lReportIndex  = stuIndex.lReportIndex;

	//		pItemData ->lListIndex[0] = pItemData ->pItem->lIconIndex;
	//		pItemData ->lListIndex[1] = pItemData ->pItem->lReportIndex;
	//	}

	//}

}
void  CTPNSResListCtrl::GetItemResData(TPListItemData *pItemData,BOOL bGetData)
{
	if(pItemData == NULL)    return;
	if(pItemData ->bGetData /*&& !(pItemData ->bGetData  & TP_RESUSEFLAG_RELEASE)*/) return;

	pItemData ->bGetData = TRUE;
	if(m_eNodeType & TP_NODE_TRASH)
	{	
//		pItemData ->bGetData = TRUE;
//		TPRecycleResData stuRecycleData;
//		stuRecycleData.eResType  = TP_RES_UNKNOW;
//		stuRecycleData.eNodeType = TP_NODE_UNKNOW;
//		if(pItemData ->bGetIndex || g_pClipManageInterface->stuFuncitonInterface.TP_GetRes(m_hCatalog,pItemData->guidRes,pItemData->guidNode, stuRecycleData)==S_OK)
//		{
//			if(!pItemData ->bGetIndex)
//			{
//				pItemData ->bGetIndex = TRUE;
//				//if(lstrlen(stuRecycleData.sName)<=0) return;
//				pItemData ->ReleaseIconBuf(); // 需要释放以前的IconBuffer
//				pItemData ->dwBookIndex   = 0;
//				pItemData ->eNodeType     = stuRecycleData.eNodeType | TP_NODE_RECYCLE;
//				pItemData ->eResType      = stuRecycleData.eResType;
//				pItemData->guidRes        = stuRecycleData.guidRes;
//
//				pItemData->guidDBType     = pItemData->guidDBType;
//				pItemData->guidRecyleNode = stuRecycleData.guidCatalog; 
//				pItemData ->sFileName     = stuRecycleData.sFilePath;
//				pItemData ->dwBookIndex   = stuRecycleData.dwBookIndex;
//				pItemData ->tmCreateTime  = stuRecycleData.tmDelete;
//				pItemData ->bDelIcon      = TRUE;
//				if(pItemData ->pItem->lIconIndex < 0)
//				{
//					pItemData ->pItem->lIconIndex                = stuRecycleData.lIconIndex;
//					pItemData ->lListIndex[0]					 = stuRecycleData.lIconIndex;
//				}
//				if(pItemData ->pItem->lReportIndex < 0)
//				{
//					pItemData ->pItem->lReportIndex              = stuRecycleData.lReportIndex;
//					pItemData ->lListIndex[1]					 = stuRecycleData.lReportIndex;
//				}
//
//				pItemData ->sText[TP_TRASH_CREATEUSER]      = stuRecycleData.sFile;
//				pItemData ->sText[TP_TRASH_DELETEDATA]		= TP_GetTimeString(stuRecycleData.tmDelete);														
//				pItemData ->sText[TP_TRASH_DELETEUSER]      = stuRecycleData.sDelBy;
//				pItemData ->sText[M2-1]                     = stuRecycleData.sFilePath;
//			}
//			else 
//			{
//				 stuRecycleData.guidRes     = pItemData ->guidRes;
//				 stuRecycleData.eResType    = pItemData ->eResType;
//				 stuRecycleData.guidCatalog = pItemData ->guidRecyleNode;
//				 lstrcpyn(stuRecycleData.sFilePath,pItemData ->sText[M2-1],MAX_PATH);
//			}
//			if(stuRecycleData.eResType & TP_RES_NOTE)
//			{
//				TPClipData stuGetClipData;
//				BOOL bDefaultNote = FALSE;
//				//{liubo modify CD2009.10.01 2010-3-31 snp4100021919 由于网络版中没有实现TP_ResInRecycle接口，默认返回值是FALSE，所以强制通过判断以取得信息。
//#ifdef TP_NS_NET
//				if(TRUE || g_pClipManageInterface->TP_ResInRecycle(pItemData->guidDBType, stuRecycleData.guidRes, stuRecycleData.guidCatalog, stuRecycleData.eResType))
//#else
//				if(g_pClipManageInterface->TP_ResInRecycle(pItemData->guidDBType, stuRecycleData.guidRes, stuRecycleData.guidCatalog, stuRecycleData.eResType))
//#endif
//					//}
//				{
//					if(pItemData ->hResData == NULL) 
//						g_pClipManageInterface ->stuClipInterfce.TP_GetNote( pItemData ->guidDBType ,GUID_NULL,pItemData ->guidRes,TP_RESLOCK_VIEW,pItemData ->hResData);
//
//					if(g_pClipManageInterface ->stuFuncitonInterface.TP_SetResUseFlag)
//						g_pClipManageInterface ->stuFuncitonInterface.TP_SetResUseFlag(pItemData ->hResData,&pItemData ->bGetData);
//					if(g_pClipManageInterface ->stuClipInterfce.TP_GetNoteInfo(pItemData ->hResData,TP_GRADE_REFCOUNT|TP_GRADE_VIEW|TP_GRADE_BASE|TP_GRADE_ICON|TP_GRADE_SOURCE|TP_GRADE_STATEINFO|TP_GRADE_FILEBASE | TP_GRADE_EXTINFO | TP_GRADE_SCENE,stuGetClipData) != S_OK)
//					{
//						pItemData ->sText[0]      = SBLoadString(_T("IDS_CLIPEXPLORER_UNKNOW"));	
//					}
//
//					// 计算素材Icon文件名
//					pItemData->iFrame = stuGetClipData.lIconFrame;
//#ifndef TP_NS_NET
//					pItemData->szIcon   = stuGetClipData.szIcon;
//					pItemData->pIcon    = stuGetClipData.pIconBuf;  
//					pItemData->bDelIcon = FALSE;
//#endif
//				}
//				else
//				{
//					CString sRecPath = stuRecycleData.sFilePath;
//					CString sClipPath = sRecPath.Left(sRecPath.ReverseFind('.')) + _T(".Clp");
//					g_pClipManageInterface->stuFuncitonInterface.TP_GetRecycleResData(sClipPath,stuRecycleData.eResType,&stuGetClipData);
//					pItemData->iFrame = stuGetClipData.lIconFrame;
//#ifndef TP_NS_NET
//					pItemData->szIcon       = stuGetClipData.szIcon;
//					pItemData->pIcon        = stuGetClipData.pIconBuf;  
//					pItemData->bDelIcon     = TRUE;
//					stuGetClipData.pIconBuf = NULL; 
//#endif
//				}
//				pItemData->iNPIndex = stuGetClipData.iNPIndex;
//				BOOL    bUsed = pItemData->iNPIndex > 0?TRUE:FALSE;
//				if(stuGetClipData.uClipFlag & TP_CLIPFLAG_NG)
//				{
//					if(bUsed)    pItemData ->dwUseState   = TP_CLIPUSED_USEDNG;
//					else         pItemData ->dwUseState   = TP_CLIPUSED_NOUSEDNG;
//				}
//				else                              
//				{
//					if(bUsed)    pItemData ->dwUseState   = TP_CLIPUSED_USEDOK;
//					else         pItemData ->dwUseState   = TP_CLIPUSED_NOUSEDOK;
//				}
//				pItemData->lRefCount = -1;
//				if(stuGetClipData.dwRefCount > 0)
//					pItemData->lRefCount = stuGetClipData.dwRefCount;
//
//				pItemData ->sText[TP_TRASH_NAME] = stuGetClipData.sName;
//				pItemData->eSingSourceType = stuGetClipData.eSignalSourceType; // Clip Source Type
//				pItemData ->eClipClass[0]  = stuGetClipData.aQuality[0].eClipClass;
//				pItemData ->eClipClass[1]  = stuGetClipData.aQuality[1].eClipClass;
//				pItemData ->uResFlag       = stuGetClipData.uClipFlag; 
//				pItemData ->cResColor	   = stuGetClipData.lColorindex;
//				pItemData ->guidDBType	   = pItemData->guidDBType; 
//				pItemData ->dwCaptureFlag  = stuGetClipData.dwCaptureFlag;   
//				pItemData ->dwVideoStand   = stuGetClipData.eVideoStandard;
// 
//				//}
//				DWORD dwClipClassH  = stuGetClipData.aQuality[0].eClipClass;
//				DWORD dwClipClassL  = stuGetClipData.aQuality[1].eClipClass;
//				pItemData->lListItemFlag =0;
//				if(TP_MEDIA_CLASS_V & (dwClipClassH | dwClipClassL))// Have Video Track
//				{
//					pItemData->sFileName = _T("");
//					pItemData->bGetFile  = FALSE;
//					// 检查Icon大小 4:3, 16:9
//					pItemData->bIs0403Icon = (pItemData ->szIcon.cx != TP_RESICON_WIDTH_HD && !(pItemData->uResFlag & TP_CLIPFLAG_1609)); 
//
//					// 检查素材是否是MediaOffline
//					if(stuGetClipData.uClipFlag & TP_CLIPFLAG_MEDIAOFFLINE) bDefaultNote = TRUE;
//					if(!bDefaultNote) bDefaultNote = TP_IsMediaOffLine(stuGetClipData.uClipFlag,dwClipClassH,dwClipClassL);
//					pItemData->lListItemFlag |= TP_ITEMFLAG_VIDEO;
//				}
//				else if(TP_MEDIA_CLASS_A & (dwClipClassH | dwClipClassL)) // Only have Audio Track
//				{
//					pItemData->bIs0403Icon = TRUE;
//					pItemData->lListItemFlag |= TP_ITEMFLAG_AUDIO;
//					bDefaultNote = TP_IsMediaOffLine(stuGetClipData.uClipFlag,dwClipClassH,dwClipClassL);
//					if(!bDefaultNote)
//					{
//						pItemData->ReleaseIconBuf();
//						pItemData ->bDelIcon   = FALSE;
//						pItemData ->pIcon      = m_pCommonRes[TP_CLIPICON_AUDIO] ->GetByte();
//						pItemData ->szIcon     = CSize(m_pCommonRes[TP_CLIPICON_AUDIO] ->Width(),m_pCommonRes[TP_CLIPICON_AUDIO] ->Height());
//
//						pItemData ->sFileName  = _L(""); 
//						pItemData ->bGetBuffer = FALSE;
//						pItemData ->dwState   |= TP_RESSTATE_OVERTURNICON;
//					}
//				}					
//				if(bDefaultNote)// Show Media OffLine Icon
//				{
//					pItemData->ReleaseIconBuf(); // 需要释放以前的IconBuffer
//					pItemData->bDelIcon    = FALSE;
//					pItemData ->pIcon      = m_pCommonRes[TP_CLIPICON_OFFLINE] ->GetByte();
//					pItemData ->szIcon     = CSize(m_pCommonRes[TP_CLIPICON_OFFLINE] ->Width(),m_pCommonRes[TP_CLIPICON_OFFLINE] ->Height());
//					pItemData ->sFileName  = _L(""); 
//					pItemData ->bGetBuffer = FALSE;
//					pItemData->dwState    |= TP_RESSTATE_OVERTURNICON;
//					pItemData->lListItemFlag |= TP_ITEMFLAG_DEFAULTNOTE;
//				}
//			}
//			else if(stuRecycleData.eResType & (TP_RES_PROJECT | TP_RES_CATALOG))
//			{
//				TPResCatalog resCatalog;
//				//{liubo modify CD2009.10.01 2010-9-20 snp4100027322 网络版不能由路径信息取到数据。
//#ifdef TP_NS_NET
//				if (NULL == pItemData->hResData)
//				{
//					if(TP_RES_PROJECT & pItemData->eResType)
//					{
//						g_pClipManageInterface->stuProjectInterface.TP_GetProject(pItemData->guidDBType,GUID_NULL,pItemData->guidRes,pItemData->hResData);
//					}
//					else
//					{
//						g_pClipManageInterface->stuCatalogInterface.TP_GetCatalog(pItemData->guidDBType,GUID_NULL,pItemData->guidRes,pItemData->hResData);
//					}
//				}
//
//				g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(pItemData->hResData, &resCatalog);
//#else
//				CString sRecPath = stuRecycleData.sFilePath;
//				CString sResPath = sRecPath.Left(sRecPath.ReverseFind('.')) + _T(".Cat");
//				g_pClipManageInterface->stuFuncitonInterface.TP_GetRecycleResData(sResPath,TP_RES_CATALOG,&resCatalog);
//#endif
//				//}
//				pItemData ->sText[TP_TRASH_NAME]			= resCatalog.sName;	
//				pItemData->dwVideoStand   = resCatalog.eVideoStandard;
//				pItemData->cResColor      = resCatalog.lColorindex;
//				pItemData ->iNPIndex      = GetFloderImageIndex(pItemData->guidRes,pItemData->eNodeType,(TP_VIDEO_STANDARD)pItemData->dwVideoStand);
//			}
//			else if(stuRecycleData.eResType & TP_RES_ALLPROGRAM) 
//			{
//				TPProgramData stuProgram;
//				if(g_pClipManageInterface->TP_ResInRecycle(pItemData->guidDBType, stuRecycleData.guidRes, stuRecycleData.guidCatalog, stuRecycleData.eResType))
//				{
//					if(pItemData ->hResData == NULL) 
//						g_pClipManageInterface ->stuProgramInterface.TP_GetProgram( pItemData ->guidDBType ,GUID_NULL,pItemData ->guidRes,TP_RESLOCK_VIEW,pItemData ->hResData);
//
//					if(g_pClipManageInterface ->stuFuncitonInterface.TP_SetResUseFlag)
//						g_pClipManageInterface ->stuFuncitonInterface.TP_SetResUseFlag(pItemData ->hResData,&pItemData ->bGetData);
//					if(g_pClipManageInterface ->stuProgramInterface.TP_GetProgramInfo(pItemData ->hResData,TP_GRADE_REFCOUNT|TP_GRADE_VIEW | TP_GRADE_BASE | TP_GRADE_ICON | TP_GRADE_EXTINFO,stuProgram) != S_OK)
//					{
//						pItemData ->sText[0]      = SBLoadString(_T("IDS_CLIPEXPLORER_UNKNOW"));	
//					}
//#ifndef TP_NS_NET
//					pItemData->szIcon   = stuProgram.szIcon;
//					pItemData->pIcon    = stuProgram.pIconBuf;  
//					pItemData->bDelIcon = FALSE;
//#endif					
//				}
//				else
//				{
//					CString sRecPath = stuRecycleData.sFilePath;
//					CString sResPath = sRecPath.Left(sRecPath.ReverseFind('.')) + _T(".Pro");
//					g_pClipManageInterface->stuFuncitonInterface.TP_GetRecycleResData(sResPath,stuRecycleData.eResType,&stuProgram);
//#ifndef TP_NS_NET
//					pItemData->szIcon   = stuProgram.szIcon;
//					pItemData->pIcon    = stuProgram.pIconBuf;  
//					pItemData->bDelIcon = TRUE;	
//					stuProgram.pIconBuf = NULL;
//#endif 
//				}  
// 
//				pItemData->guidDBType     = pItemData->guidDBType; 
//				pItemData ->iFrame        = stuProgram.uPgmFlag; // LI HUI LI 2007-12-22 XN00008328 Set flag
//				pItemData ->iNPIndex	  = stuProgram.iNPIndex;				
//				pItemData ->uResFlag      = stuProgram.eSubProgramType;
//				pItemData ->dwVideoStand  = stuProgram.eVideoStandard;
//
//				BOOL    bUsed = pItemData->iNPIndex > 0?TRUE:FALSE;
//				if(bUsed)    pItemData ->dwUseState   = TP_CLIPUSED_USEDOK;
//				else		 pItemData ->dwUseState   = TP_CLIPUSED_NOUSEDOK;
//
//				pItemData->lRefCount = -1;
//				if(stuProgram.dwRefCount > 0)
//					pItemData->lRefCount = stuProgram.dwRefCount;
//
//				pItemData ->sText[TP_TRASH_NAME] = stuProgram.sName;
//				pItemData->cResColor = stuProgram.lColorindex;
//				pItemData->bIs0403Icon = (pItemData ->szIcon.cx != TP_RESICON_WIDTH_HD && !(pItemData->uResFlag & TP_CLIPFLAG_1609)); 
//			}
//			else if(stuRecycleData.eResType & TP_RES_ALLTEMPLATE)
//			{
//				TPTemplateData stuTemplate;
//#ifndef TP_NS_NET
//				CString sRecPath = stuRecycleData.sFilePath;
//				CString sResPath = sRecPath.Left(sRecPath.ReverseFind('.')) + _T(".TMP");
//				g_pClipManageInterface->stuFuncitonInterface.TP_GetRecycleResData(sResPath,TP_RES_ALLTEMPLATE,&stuTemplate);
//				pItemData->szIcon    = stuTemplate.szIcon;
//				pItemData->pIcon     = stuTemplate.pIconBuf;  
//				pItemData->bDelIcon  = TRUE;	
//				stuTemplate.pIconBuf = NULL;
//#else
//				g_pClipManageInterface->stuTemplateInterface.TP_GetTemplateInfoEx(stuRecycleData.guidDBType,stuRecycleData.guidRes,TP_GRADE_ALL,stuTemplate);
//#endif
//				pItemData->bIs0403Icon = pItemData ->szIcon.cx != TP_RESICON_WIDTH_HD;
//				if (pItemData->eResType & TP_RES_ALLVATEMPLATE) pItemData->bIs0403Icon = FALSE;
//				pItemData ->sText[TP_TRASH_NAME] = stuTemplate.sName;	
//				pItemData->dwVideoStand			 = stuTemplate.eVideoStandard;
//				pItemData->dwState				 = stuTemplate.dwState;
//				pItemData->dwState				&=~ TP_RESSTATE_CUT; // li song ming 2010-12-29 XN00030523 修改为回收站中去掉TP_RESSTATE_CUT标志
//			}
//		}
//		else
//		{
//			pItemData ->sText[0]      = SBLoadString(_T("IDS_CLIPEXPLORER_UNKNOW"));					
//		}
	}	
	else if(pItemData ->eResType & (TP_RES_PROJECT | TP_RES_CATALOG))
	{
		TPCatalogData resCatalog;
		CString      sCatalogName;
		if(pItemData ->hResData == NULL) 
		{
			//if(TP_RES_PROJECT & pItemData->eResType)
			//	g_pClipManageInterface->stuProjectInterface.TP_GetProject(pItemData ->guidDBType,GUID_NULL,pItemData->guidRes,pItemData ->hResData);
			//else
				g_pClipManageInterface->stuCatalogInterface.TP_GetCatalog(pItemData ->guidDBType,GUID_NULL,pItemData->guidRes,pItemData ->hResData);
		}
		if(pItemData ->hResData == NULL || !CTPNSResTreeCtrl::GetIsShow(pItemData ->hResData ))
		{
			pItemData ->pItem ->bShow = FALSE;
			return ;
		}

		HRESDATA     hResCatalog =  pItemData ->hResData;
		pItemData ->bGetData = TRUE;
		if(hResCatalog)
		{		
			g_pClipManageInterface ->stuCatalogInterface.TP_GetCatalogInfo(hResCatalog,&resCatalog);			
			pItemData ->sText[0] = resCatalog.sName;			

		}
		else if(lstrlen(pItemData ->sText[0])<=0)
		{
			pItemData ->sText[0]  = SBLoadString(_T("IDS_CLIPEXPLORER_UNKNOWFLODER"));		
		}

		pItemData ->eResType      = resCatalog.eResType;
		pItemData ->eNodeType     = resCatalog.eNodeType;	
		pItemData ->guidDBType    = resCatalog.guidDBType;
		pItemData ->dwOperateFlag = resCatalog.dwOperateFlag;
		pItemData ->dwBookIndex   = resCatalog.dwBookMarkIndex;
		pItemData ->dwState       = resCatalog.dwState;
		pItemData ->tmCreateTime  = resCatalog.tmCreate;
		pItemData ->tmModifyTime  = resCatalog.tmLastModify;
		pItemData ->cResColor     = resCatalog.lColorindex;
		pItemData ->pIcon         = NULL;		
		pItemData ->iNPIndex      = GetFloderImageIndex(pItemData->guidRes,pItemData->eNodeType);

		//if(pItemData ->pItem->lIconIndex < 0)
		//{
		//	pItemData ->pItem->lIconIndex    = resCatalog.lIconIndex;
		//	pItemData ->lListIndex[0]		 = resCatalog.lIconIndex;
		//}
		//if(pItemData ->pItem->lReportIndex < 0)
		//{
		//	pItemData ->pItem->lReportIndex  = resCatalog.lReportIndex;
		//	pItemData ->lListIndex[1]		 = resCatalog.lReportIndex;
		//}

		//if(TP_RES_PROJECT & pItemData->eResType)
		//{
		//	TPProjectData stuProject;
		//	if(S_OK == g_pClipManageInterface->stuProjectInterface.TP_GetProjectInfo(pItemData->hResData, TP_GRADE_BASE, stuProject))
		//	{
		//		VIDEO_TYPE eVideoType = TP_GetVideoType(stuProject.eVideoStandard, stuProject.bDropFrame);
		//		GetItemResDataInfo(pItemData, TP_RES_PROJECT, (LPVOID)&stuProject, eVideoType);
		//	}

		//}
	}
	else if(pItemData ->eResType & TP_RES_NOTE)//判断pItemData
	{
		if(pItemData ->hResData == NULL) 
			g_pClipManageInterface ->stuNoteInterface.TP_GetNote( pItemData ->guidDBType ,GUID_NULL,pItemData ->guidRes,TP_RESLOCK_VIEW,pItemData ->hResData);
		TPNoteData stuGetClipData;

		if(g_pClipManageInterface ->stuFuncitonInterface.TP_SetResUseFlag)
			g_pClipManageInterface ->stuFuncitonInterface.TP_SetResUseFlag(pItemData ->hResData,&pItemData ->bGetData);
		if(g_pClipManageInterface ->stuNoteInterface.TP_GetNoteInfo(pItemData ->hResData,TP_GRADE_REFCOUNT|TP_GRADE_VIEW|TP_GRADE_BASE|TP_GRADE_ICON|TP_GRADE_SOURCE|TP_GRADE_STATEINFO|TP_GRADE_FILEBASE | TP_GRADE_EXTINFO | TP_GRADE_SCENE | TP_GRADE_GROUP,stuGetClipData) == S_OK)
		{
			pItemData ->bGetData		= TRUE;
// 			pItemData ->lLength			= stuGetClipData.lClipLength;  //长度
// 			pItemData ->iNPIndex		= stuGetClipData.iNPIndex;	  //是否正在被使用			
			pItemData ->guidDBType		= stuGetClipData.guidDBType;
//			pItemData ->uResSource		= stuGetClipData.uResSource ;  //ResType
			pItemData ->pIcon			= stuGetClipData.pIconBuf;
			pItemData ->szIcon			= stuGetClipData.szIcon;				
			pItemData ->dwState			= stuGetClipData.dwState;
//			pItemData ->uResFlag		= stuGetClipData.uClipFlag;	
			pItemData ->tmCreateTime	= stuGetClipData.tmCreate;
			pItemData ->tmModifyTime	= stuGetClipData.tmLastModify;
//			pItemData ->eSingSourceType = stuGetClipData.eSignalSourceType;//素材类型	
			pItemData->lListItemFlag	= 0; // 附加标志
//			pItemData->dwVideoStand     = stuGetClipData.eVideoStandard;
//			pItemData->bDropFrame		= stuGetClipData.bDropFrame;
			pItemData ->cResColor       = stuGetClipData.lColorindex;
//			pItemData->tmRecTime		= stuGetClipData.tmRecord;
// 			pItemData->iFrame           = stuGetClipData.lIconFrame;
//			pItemData->dwCaptureFlag    = stuGetClipData.dwCaptureFlag; 
			pItemData ->lRefCount = -1;
			if(stuGetClipData.dwRefCount -1 > 0)
				pItemData ->lRefCount = stuGetClipData.dwRefCount - 1;
			//if(pItemData ->pItem->lIconIndex < 0)
			//{
			//	pItemData ->pItem->lIconIndex    = stuGetClipData.lIconIndex;
			//	pItemData ->lListIndex[0]        = stuGetClipData.lIconIndex;
			//}
			//if(pItemData ->pItem->lReportIndex < 0)
			//{
			//	pItemData ->pItem->lReportIndex  = stuGetClipData.lReportIndex;
			//	pItemData ->lListIndex[1]        = stuGetClipData.lReportIndex;
			//}

			//NG or GOOD Clip
			BOOL    bUsed = pItemData->iNPIndex > 0?TRUE:FALSE;
			//if(stuGetClipData.uClipFlag & TP_CLIPFLAG_NG)
			//{
			//	if(bUsed)    pItemData ->dwUseState   = TP_CLIPUSED_USEDNG;
			//	else         pItemData ->dwUseState   = TP_CLIPUSED_NOUSEDNG;
			//}
			//else                              
			{
				if(bUsed)    pItemData ->dwUseState   = TP_CLIPUSED_USEDOK;
				else         pItemData ->dwUseState   = TP_CLIPUSED_NOUSEDOK;
			}
			BOOL bDefaultIcon = FALSE;
			if(NULL == pItemData->pIcon) bDefaultIcon = TRUE;
			if(bDefaultIcon)// Show Default Icon
			{
				pItemData->ReleaseIconBuf();
				pItemData ->bDelIcon   = FALSE;
				pItemData ->dwState   |= TP_RESSTATE_OVERTURNICON;
 				pItemData ->pIcon      = m_pCommonRes[TP_CLIPICON_DefaultNote] ->GetByte();
 				pItemData ->szIcon     = CSize(m_pCommonRes[TP_CLIPICON_DefaultNote]->Width(),m_pCommonRes[TP_CLIPICON_DefaultNote]->Height());

				pItemData ->sFileName  = _L(""); 
				pItemData ->bGetBuffer = FALSE;
				pItemData->lListItemFlag |= TP_ITEMFLAG_MEDIAOFFLINE;
			}
			pItemData ->eNodeType     = stuGetClipData.eNodeType;
			pItemData ->dwBookIndex   = stuGetClipData.dwBookMarkIndex;
			pItemData ->dwOperateFlag = stuGetClipData.dwOperateFlag;

			//VIDEO_TYPE eVideoType = TP_GetVideoType(stuGetClipData.eVideoStandard, stuGetClipData.bDropFrame);
			GetItemResDataInfo(pItemData,TP_RES_NOTE,&stuGetClipData,VIDEO_30M);

			if((pItemData ->dwCaptureFlag & TP_CLIPCAPTURE_UPLOADING) && m_uFlashTimer == NULL)
				m_uFlashTimer = SetTimer(TP_EVENT_FLASH,TP_FASHICON_ESCIP, NULL);
		}	
	}
	else CTPResListCtrl::GetItemResData(pItemData,bGetData);

	if(pItemData &&(pItemData ->dwState & TP_RESSTATE_HIDE)) pItemData->pItem->bShow = FALSE;
	//if(pItemData && pItemData->pItem && m_pResExplorer && m_pResExplorer->m_pSearchBar && !m_sResFilter.IsEmpty())
	//{  
	//	BOOL bShow = pItemData->pItem->bShow;
	//	CString sName = pItemData ->sText[0];
	//	pItemData->pItem->bShow = FileterResMothed(m_sResFilter,sName);	
	//	if(!m_bRelayout && pItemData->pItem->bShow != bShow)
	//	{
	//		if(!pItemData->pItem->bShow) pItemData->pItem-> EmptySubItem();
	//		m_bRelayout = TRUE;
	//		PostMessage(WM_LIST_RELAYOUT,0,0);
	//	}		
	//}	
}
BEGIN_MESSAGE_MAP(CTPNSResListCtrl, CTPResListCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND_RANGE(ID_MINCMD,ID_MAXCMD,OnCmdMsg)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()

	//ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnLvnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnLvnBegindrag)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)

	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_NCPAINT()
	ON_WM_CAPTURECHANGED()
	ON_MESSAGE(WM_OLEDROPTARGET_MSG,OnMsgOleDrag)
	ON_MESSAGE(WM_LIST_RELAYOUT,OnMsgRelayout)
//	ON_MESSAGE(WM_LIST_RESRENANME, OnResRename)
END_MESSAGE_MAP()

int  CTPNSResListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPResListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_iBackColor =  COLOR_CLIP_LIST;

	//HDX Exchange 也要使用
	if (m_dwWndType == TP_WND_RESEXPLORER)
	{
		m_pDropTarget = new CTPOleDropTarget();
		m_pDropTarget ->m_pMsgWnd = this;
		m_pDropTarget ->Register(this);	

	}

	return 0;
}

int  CTPNSResListCtrl::GetFloderImageIndex(GUID guidCatalog,ULONGLONG dwNodeType)
{
	int iCurrentIcon = 0;	
	if(dwNodeType & TP_NODE_BIN)         iCurrentIcon = TP_CLIPFLODER_BIN;
	else if(dwNodeType & TP_NODE_TRASH)  iCurrentIcon = TP_CLIPFLODER_TRASH;	
	else if(dwNodeType & TP_NODE_COMPUTER)                   iCurrentIcon = TP_CLIPFLODER_DATASOURCE;		
	else if(dwNodeType & TP_NODE_NETWORK) 	        	     iCurrentIcon = TP_CLIPFLODER_NETWORK;		
	else if(dwNodeType & TP_NODE_SEARCH)                     iCurrentIcon = TP_CLIPFLODER_SEARCHRESULT;		
	else if(dwNodeType & TP_NODE_CG)						 iCurrentIcon = TP_CLIPFLODER_CG;
	return iCurrentIcon;
}
void CTPNSResListCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{	
	*pResult = 0;

}

void CTPNSResListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//if(!m_rtClient.PtInRect(point)) return ; // 如果不在客户区不处理
	//int iDbClickItem = CTPListCtrlEx::HitTest(point);
	//if(iDbClickItem >=0)
	//{
	//	m_uState |= STATE_DBCLICK;
	//	CTPResListCtrl::EndEdit();		
	//	if(m_pMsgWnd) m_pMsgWnd->PostMessage(WM_LIST_DBCLICK, iDbClickItem, (LPARAM)this);
	//	if(m_bForList) return;
	//	TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iDbClickItem);
	//	if(pItemData && pItemData ->eResType == TP_RES_USER && g_pUserEnterInfo ->guidUserConfigration != pItemData ->guidRes)
	//	{
	//		OnCmdMsgClip(ID_USERINFO_ACTIVE);			
	//		return;
	//	}		
	//	else if(pItemData && (pItemData ->eNodeType & TP_NODE_SEARCH))
	//	{
	//		TP_RES_TYPE eRes = pItemData->eResType;
	//		HTREEITEM hTree  = m_pResTreeCtrl ->FindCatalog(pItemData->guidDBType,pItemData->guidNode,NULL);
	//		if(hTree)
	//		{
	//			m_iMenuItem = -1;
	//			GUID guidItem = pItemData->guidRes;
	//			m_pResTreeCtrl ->SelectItem(hTree);
	//			m_pResTreeCtrl ->SelectItemChange(hTree);
	//			for(INT l=0;l<m_aItem.GetSize();l++)
	//			{
	//				if(((TPListItemData *)m_aItem[l]->dwData) ->guidRes == guidItem)
	//				{
	//					SelectItem(l,TRUE,TRUE);
	//					m_iMenuItem = l;
	//					break;
	//				}
	//			}
	//			if(m_iMenuItem>=0)
	//			{
	//				if(eRes == TP_RES_PROJECT)                        return OnCmdMsgClip(ID_SYSCMD_OPENPROJECT);
	//				else if(eRes & (TP_RES_NOTE | TP_RES_ALLPROGRAM)) return OnCmdMsgClip(ID_RESLISTSELECT_OPEN);					
	//			}
	//		}
	//	}
	//	else if(pItemData && (pItemData ->eResType & (TP_RES_NOTE | TP_RES_ALLPROGRAM)))
	//	{
	//		m_iMenuItem = iDbClickItem;
	//		return OnCmdMsgClip(ID_RESLISTSELECT_OPEN);
	//	}
	//	else if(pItemData && (pItemData ->eResType & TP_RES_PROJECT) && TP_IsActTeamDB(pItemData ->guidDBType))
	//	{
	//		m_iMenuItem = iDbClickItem;
	//		return OnCmdMsgClip(ID_SYSCMD_OPENPROJECT);
	//	}
	//	else if(pItemData && (pItemData ->eResType == TP_RES_CGTEMPLATE))
	//	{
	//		m_iMenuItem = iDbClickItem;
	//		return OnCmdMsgClip(ID_RESLISTSELECT_OPEN);
	//	}
	//}
	CTPResListCtrl::OnLButtonDblClk(nFlags, point);
}
void  TP_ChangeSearchProjectColum(int *iColumnIndex)
{
	for(INT l=0;l<M2;l++)
	{
		if(iColumnIndex[l] == TP_PROJECTTEXTEX_NAME)              iColumnIndex[l] = TP_PROJECTTEXT_NAME;
		else if(iColumnIndex[l] == TP_PROJECTTEXTEX_MODIFYTIME)   iColumnIndex[l] = TP_PROJECTTEXT_MODIFYTIME;
		else if(iColumnIndex[l] == TP_PROJECTTEXTEX_VIDEOFORMAT)  iColumnIndex[l] = TP_PROJECTTEXT_VIDEOFORMAT;
		else if(iColumnIndex[l] == TP_PROJECTTEXTEX_ASPECTRATION) iColumnIndex[l] = TP_PROJECTTEXT_ASPECTRATION;
		else if(iColumnIndex[l] == TP_PROJECTTEXTEX_STATUS)       iColumnIndex[l] = TP_PROJECTTEXT_STATUS;
		else if(iColumnIndex[l] == TP_PROJECTTEXTEX_CLIPCOLOR)    iColumnIndex[l] = TP_PROJECTTEXT_CLIPCOLOR;
		else                                                      iColumnIndex[l] = TP_RESTEXT_MAX -1;
	}
}
void  CTPNSResListCtrl::DrawReport(CDC *pDC,int iItem)
{
	TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iItem);
	if(pItemData == NULL) return ;
	GetItemResData(pItemData);
	if(!pItemData ->pItem->bShow) return;
	if(pItemData ->dwState & TP_RESSTATE_HIDE) return;
	if(m_bViewFile) GetItemResClipFile(pItemData); 
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
	if((m_eNodeType & TP_NODE_SEARCH)) 
		TP_ChangeSearchProjectColum(iColumnIndex);
	INT nHeadIndex = TP_NodeTypeHeadIndex(m_eShowNodeType);
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
			rtIcon.right = rtIcon.left + m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Width();
			rtIcon.OffsetRect(2,0);		
			CRect rtIconView = rtIcon & rcSub;
			if(rtIconView.Width() > 0)
			{
				if(m_dwWndType & TP_WND_RESEXPLORER)
				{
					DWORD dwIndex = TP_GetNoteTypeIconIndex(pItemData->eResType, pItemData->eSingSourceType, pItemData);
					ImageList_DrawEx(m_pCommonRes[TP_CLIPICON_TYPEINDICATORS] ->GetImage(),dwIndex,pDC->m_hDC,rtIcon.left,rtIcon.top + (rtIcon.Height() - m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Height())/2, rtIconView.Width(), rtIconView.Height(),CLR_NONE, CLR_NONE, ILD_TRANSPARENT);	
				}
				else if(m_dwWndType & TP_WND_UNKONWEXPLORER)
				{
					ImageList_DrawEx(m_pCommonRes[TP_CLIPICON_TYPEINDICATORS] ->GetImage(),TP_CLIPTYPE_EFFECTTEMPLATE,pDC->m_hDC,rtIcon.left,rtIcon.top + (rtIcon.Height() - m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Height())/2,rtIconView.Width(), rtIconView.Height(), CLR_NONE, CLR_NONE, ILD_TRANSPARENT);
				}
			}

			m_aItem[iItem]->aSubItem[i]->rtIcon = rtIcon;			
			rcSub.left = rtIcon.right + 5;
		}
		rcSub.DeflateRect(2,0);
		if((pItemData->eResType &(TP_RES_NOTE | TP_RES_CATALOG)) && nHeadIndex == TP_HEADINDEX_BIN && iColumnIndex[i] == TP_RESTEXT_CLIPCOLOR ||\
			(pItemData->eResType &(TP_RES_CATALOG | TP_RES_PROJECT)) && nHeadIndex == TP_HEADINDEX_COMPUTER && iColumnIndex[i] == TP_PROJECT_CLIPCOLOR||\
			(pItemData->eResType & TP_RES_CATALOG) && nHeadIndex == TP_HEADINDEX_PROJECT && iColumnIndex[i] == TP_CATALOG_CLIPCOLOR||\
			((m_eNodeType & TP_NODE_SEARCH) && (pItemData->eResType & TP_RES_PROJECT) && (iColumnIndex[i] == TP_PROJECTTEXT_CLIPCOLOR)))
		{
			CRect rtDraw = rcSub;
			rtDraw.left += rcSub.Width()/2- rcSub.Height()/2;
			rtDraw.left = max(rtDraw.left, rcSub.left);
			rtDraw.right = rtDraw.left + rcSub.Height() + 20;
			if(rtDraw.right >= rcSub.right)
				rtDraw.right = rcSub.right;
			COLORREF cColor = TP_GetResColor((TP_RES_COLOR)pItemData->cResColor, FALSE, TRUE);
			rtDraw.DeflateRect(1,4,1,4);
			pDC->Draw3dRect(rtDraw, cColor, cColor);
			rtDraw.DeflateRect(1,1,1,1);
			pDC->FillSolidRect(rtDraw, cColor);
		}
		else
		{
			if(iColumnIndex[i] == TP_RESTEXT_CLIP_NAME)
				pDC ->DrawText(pItemData ->sText[iColumnIndex[i]],rcSub,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS);
			else
				pDC ->DrawText(pItemData ->sText[iColumnIndex[i]],rcSub,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		}
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
void CTPNSResListCtrl::DrawTextPicture(CDC *pDC,int iItem)
{	
	TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iItem);
	if(pItemData == NULL) return ;
	GetItemResData(pItemData);
	if(pItemData ->dwState & TP_RESSTATE_HIDE) return;
	if(!pItemData ->pItem->bShow) return;

	int iColumnCount = this ->GetColumnCount();
	if(iColumnCount <=0) return;
	if(m_bViewFile) GetItemResClipFile(pItemData); 
	int iColumnIndex[M2];
	GetColumnOrderArray(iColumnIndex,iColumnCount);

	INT nHeadIndex = TP_NodeTypeHeadIndex(m_eShowNodeType);
	UINT uState = CTPListCtrlEx::GetItemState(iItem,LVIS_SELECTED |  LVIS_FOCUSED);	
	if(uState & LVIS_SELECTED )    pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));	
	else                           pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));

	int iLeft = 10000,iButton = 0;
	CRect rDTRect(0,0,0,0);//右下黑框
	int iOffset = 20;
	CRect rtComment = CRect(0, 0, 0, 0), rtCommentText = CRect(0, 0, 0, 0);
	if((m_eNodeType & TP_NODE_SEARCH) && (pItemData ->eResType & TP_RES_PROJECT)) 
		TP_ChangeSearchProjectColum(iColumnIndex);

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

			BOOL bOverTurn = FALSE;
			if(pItemData->dwState & TP_RESSTATE_OVERTURNICON) bOverTurn = TRUE;

			// 绘制目录图标
			rtIcon = rtSub_Name;
			rtSub_Name &= rcSub;
			if(pItemData->eResType & (TP_RES_CATALOG | TP_RES_PROJECT ))
			{
				int   iNPIndex  = pItemData ->iNPIndex;
				rtIcon.left    = rtIcon.left + (rtIcon.Width() - m_pCommonRes[TP_CLIPICON_FLODER]->Width())/2;
				rtIcon.top     = rtIcon.top  + (rtIcon.Height()- m_pCommonRes[TP_CLIPICON_FLODER]->Height())/2;
				rtIcon.right   = rtIcon.left + m_pCommonRes[TP_CLIPICON_FLODER]->Width();
				rtIcon.bottom  = rtIcon.top  + m_pCommonRes[TP_CLIPICON_FLODER]->Height();	
				//绘制文件夹大图标
				CRect rtIconView = rcSub & rtIcon;
				if(rtIconView.Width()>0) // 可见时绘制
					ImageList_DrawEx(m_pCommonRes[TP_CLIPICON_FLODER] ->GetImage(),iNPIndex,pDC->m_hDC,rtIcon.left,rtIcon.top ,rtIconView.Width(),rtIconView.Height(),CLR_NONE,CLR_NONE,ILD_TRANSPARENT);
			}
			else
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
						TP_StretchDIBitsEx(pDC,pItemData ->pIcon,pItemData ->szIcon.cx,pItemData ->szIcon.cy,rtIcon,rtIconView.Width(),rtIconView.Height(),!bOverTurn);	
				}
				else 
				{
					//如果找不到绘制的图片，则绘制全黑
					if(rtSub_Name.Width() > 0)
						pDC ->FillSolidRect(rtSub_Name,RGB(0,0,0));
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
			rtSmallIcon.DeflateRect(1,1);	
			rtSmallIcon.right = rtSmallIcon.left + m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Width();
			rtSmallIcon.OffsetRect(2,0);
			rtSmallIcon.top    += (iOffset - m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Height())/2;
			rtSmallIcon.bottom  = rtSmallIcon.top + m_pCommonRes[TP_CLIPICON_TYPEINDICATORSLARGE]->Height();
			int nIndex = TP_GetNoteTypeIconIndex(pItemData->eResType, pItemData->eSingSourceType, pItemData);
			//画name栏右上的小图标
			CRect rtSmallIconView = rcSub & rtSmallIcon;
			if(rtSmallIconView.Width() >0)
				ImageList_DrawEx(m_pCommonRes[TP_CLIPICON_TYPEINDICATORS] ->GetImage(),nIndex,pDC->m_hDC,rtSmallIcon.left,rtSmallIcon.top ,rtSmallIconView.Width(),rtSmallIconView.Height(),CLR_NONE,CLR_NONE,ILD_TRANSPARENT);
			rcSub.left = rtSmallIcon.right + 5;
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

		// 绘制Text
		 if((pItemData->eResType &(TP_RES_NOTE |TP_RES_CATALOG)) && nHeadIndex == TP_HEADINDEX_BIN && iColumnIndex[i] == TP_RESTEXT_CLIPCOLOR ||\
			(pItemData->eResType &(TP_RES_CATALOG | TP_RES_PROJECT)) && nHeadIndex == TP_HEADINDEX_COMPUTER && iColumnIndex[i] == TP_PROJECT_CLIPCOLOR||\
			(pItemData->eResType & TP_RES_CATALOG) && nHeadIndex == TP_HEADINDEX_PROJECT && iColumnIndex[i] == TP_CATALOG_CLIPCOLOR ||\
			((m_eNodeType & TP_NODE_SEARCH) && (pItemData->eResType & TP_RES_PROJECT) && (iColumnIndex[i] == TP_PROJECTTEXT_CLIPCOLOR)))
		{
			CRect rtDraw = rcSub;
			rtDraw.left += rcSub.Width()/2- rcSub.Height()/2;
			rtDraw.left = max(rtDraw.left, rcSub.left);
			rtDraw.right = rtDraw.left + rcSub.Height() + 20;
			if(rtDraw.right >= rcSub.right)
				rtDraw.right = rcSub.right;
			COLORREF cColor = TP_GetResColor((TP_RES_COLOR)pItemData->cResColor, FALSE, TRUE);
			rtDraw.DeflateRect(1,4,1,4);
			pDC->Draw3dRect(rtDraw, cColor, cColor);
			rtDraw.DeflateRect(1,1,1,1);
			pDC->FillSolidRect(rtDraw, cColor);
		}
		else
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
	if(pItemData ->eResType & (TP_RES_NOTE ))
	{		
		pDC->DrawText(pItemData->sText[TP_RESTEXT_COMMENT],rtCommentText,DT_VCENTER|DT_SINGLELINE);
	}
	//else if((pItemData ->eResType & TP_RES_ALLCGTEMPLATE))
	//{

	//	pDC->DrawText(pItemData->sText[TP_TEMPALTECGTEXT_COMMENT],rtCommentText,DT_VCENTER|DT_SINGLELINE);
	//}
	//else if(pItemData->eResType & TP_RES_USER)
	//{
	//	pDC->DrawText(pItemData->sText[TP_USEREXT_COMMENT],rtCommentText,DT_VCENTER|DT_SINGLELINE);
	//}
}


CString TP_GetMetadataText(TPListItemData* pItemData,long uColumn)
{
	//for(int i = 0 ; i < 12; i++)
	//{
	//	// 以后换贴图区分类型
	//	switch(uColumn)
	//	{
	//	case TP_INDEXSHOW_MARKIN:
	//		return /*CString(_T("I:")) + */pItemData->sText[TP_RESTEXT_MARK_IN];
	//	case TP_INDEXSHOW_MARKOUT:
	//		return /*CString(_T("O:")) +*/pItemData->sText[TP_RESTEXT_MARK_OUT];
	//	case TP_INDEXSHOW_DUATION:
	//		return /*CString(_T("D:")) +*/pItemData->sText[TP_RESTEXT_DURATION];
	//	case TP_INDEXSHOW_CLIPLENGTH:
	//		return /*CString(_T("L:")) +*/pItemData->sText[TP_RESTEXT_CLIP_LENGTH];
	//	default:
	//		{
	//			if(pItemData->bDropFrame)
	//				return CString(_T("--:--.--:--"));
	//			else
	//				return CString(_T("--:--:--:--"));
	//		}
	//	}
	//}

	//if(pItemData->bDropFrame)
	//	return CString(_T("--:--.--:--"));
	//else
	//	return CString(_T("--:--:--:--"));
	return CString(_T("--:--:--:--"));
}

int TP_GetMetadataIndex(TPListItemData* pItemData,long uColumn)
{
	//for(int i = 0 ; i < 12; i++)
	//{
	//	// 以后换贴图区分类型
	//	switch(uColumn)
	//	{
	//	case TP_INDEXSHOW_MARKIN:
	//		return 0;
	//	case TP_INDEXSHOW_MARKOUT:
	//		return 1;
	//	case TP_INDEXSHOW_DUATION:
	//		return 2;
	//	case TP_INDEXSHOW_CLIPLENGTH:
	//		return 3;
	//	default:
	//		return -1;
	//	}
	//}
	return -1;
}

void CTPNSResListCtrl::DrawIconOverlay(CDC *pDC,int iItem, CRect rtIcon)
{

	if(iItem < 0) return;
	//TPListItemData* pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(iItem);
	//if(NULL == pItemData) return;
	//GUID guidRecycle = TP_RECYCLE_GUID;
	//if(pItemData->eNodeType & TP_NODE_RECYCLE) return;

	//CRect  rtOverlay = CRect(0, 0, rtIcon.Width(), rtIcon.Height());
	//BOOL bShowOver = g_stuMetadataPropertiesSet.bShowTC;
	//CString sText[4];
	//int iIndex[4];

	//if(!bShowOver) return;
	//BOOL bShow = FALSE; 
	//for(int i = 0 ; i < 4; i++)
	//{			
	//	if(g_stuMetadataPropertiesSet.lTCType[i] >= 0)	bShow = TRUE;
	//	sText[i] = TP_GetMetadataText(pItemData,g_stuMetadataPropertiesSet.lTCType[i]);
	//	iIndex[i] = TP_GetMetadataIndex(pItemData,g_stuMetadataPropertiesSet.lTCType[i]);
	//}

	//if(!bShow) return;
	//int iOffsetX = 0,iOffsetY = 13;


	//BLENDFUNCTION blf;
	//ZeroMemory(&blf, sizeof(&blf));
	//blf.AlphaFormat = AC_SRC_OVER;
	//blf.BlendFlags = 0;
	//blf.BlendOp = 0;
	//blf.SourceConstantAlpha = 100;


	//if (m_lIconSize == 4)
	//{
	//	if(rtIcon.Width() < 96)
	//	{
	//		rtOverlay.SetRect(0, 0, rtIcon.Width()+10, 26);
	//		iOffsetX = -5;
	//	}
	//	else
	//	{
	//		rtOverlay.SetRect(0, 0, rtIcon.Width() - 10, 26);
	//		iOffsetX = 5;
	//	}
	//	CTPMemoDC  cdcInOut(pDC,rtOverlay.Width(),rtOverlay.Height());
	//	cdcInOut.SetTextColor(RGB(255, 255, 255));
	//	CRect  rtOverTmp =  CRect(0, 0, rtIcon.Width() - iOffsetX*2, iOffsetY);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[0],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(15);
	//	cdcInOut.DrawText(sText[0], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	rtOverTmp.MoveToXY(0,iOffsetY);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[1],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);
	//	rtOverTmp.MoveToX(15);
	//	cdcInOut.DrawText(sText[1], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	AlphaBlend(pDC ->m_hDC, rtIcon.left + iOffsetX, rtIcon.top + rtIcon.Height() - iOffsetY*2, rtIcon.Width()-iOffsetX*2, iOffsetY*2,
	//		cdcInOut.m_hDC, 0, 0, rtOverlay.Width(), rtOverlay.Height(), blf);
	//	cdcInOut.DeleteDC();
	//}
	//else if (m_lIconSize == 3)
	//{
	//	if(rtIcon.Width() < 120)
	//	{
	//		rtOverlay.SetRect(0, 0, rtIcon.Width() - 16, 39);
	//		iOffsetX = 8;
	//	}
	//	else
	//	{
	//		rtOverlay.SetRect(0, 0, rtIcon.Width() - 60, 39);
	//		iOffsetX = 30;
	//	}
	//	CTPMemoDC  cdcInOut(pDC,rtOverlay.Width(),rtOverlay.Height());
	//	cdcInOut.SetTextColor(RGB(255, 255, 255));
	//	CRect  rtOverTmp =  CRect(0, 0, rtIcon.Width() - iOffsetX*2, iOffsetY);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[0],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(15);
	//	cdcInOut.DrawText(sText[0], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	
	//	rtOverTmp.MoveToXY(0,iOffsetY);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[1],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(15);
	//	cdcInOut.DrawText(sText[1], rtOverTmp, DT_SINGLELINE|DT_LEFT);

	//	rtOverTmp.MoveToXY(0,iOffsetY*2);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[2],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);
	//	rtOverTmp.MoveToX(15);
	//	cdcInOut.DrawText(sText[2], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	AlphaBlend(pDC ->m_hDC, rtIcon.left + iOffsetX, rtIcon.top + rtIcon.Height() -iOffsetY*3, rtIcon.Width()-iOffsetX*2, iOffsetY*3,
	//		cdcInOut.m_hDC, 0, 0, rtOverlay.Width(), rtOverlay.Height(), blf);
	//	cdcInOut.DeleteDC();
	//}
	//else if (m_lIconSize == 2)
	//{
	//	if(rtIcon.Width() < 150)
	//	{
	//		rtOverlay.SetRect(0, 0, rtIcon.Width() + 28, 26);
	//		iOffsetX = -14;
	//	}
	//	else
	//	{
	//		rtOverlay.SetRect(0, 0, rtIcon.Width() - 20, 26);
	//		iOffsetX = 10;
	//	}
	//	CTPMemoDC  cdcInOut(pDC,rtOverlay.Width(),rtOverlay.Height());
	//	cdcInOut.SetTextColor(RGB(255, 255, 255));
	//	CRect  rtOverTmp =  CRect(0, 0, rtIcon.Width() - iOffsetX*2, iOffsetY);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[0],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(15);
	//	cdcInOut.DrawText(sText[0], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	rtOverTmp.MoveToX(rtOverTmp.Width() / 2);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[1],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(rtOverTmp.Width() / 2+15);
	//	cdcInOut.DrawText(sText[1], rtOverTmp, DT_SINGLELINE|DT_LEFT);

	//	rtOverTmp.MoveToXY(0,iOffsetY);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[2],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(15);
	//	cdcInOut.DrawText(sText[2], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	rtOverTmp.MoveToX(rtOverTmp.Width() / 2);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[3],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(rtOverTmp.Width() / 2+15);
	//	cdcInOut.DrawText(sText[3], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	AlphaBlend(pDC ->m_hDC, rtIcon.left + iOffsetX, rtIcon.top + rtIcon.Height() -iOffsetY*2, rtIcon.Width()-iOffsetX*2, iOffsetY*2,
	//		cdcInOut.m_hDC, 0, 0, rtOverlay.Width(), rtOverlay.Height(), blf);
	//	cdcInOut.DeleteDC();
	//}
	//else if (m_lIconSize == 1)
	//{
	//	if(rtIcon.Width() < 220)
	//	{
	//		rtOverlay.SetRect(0, 0, rtIcon.Width() - 20, 26);
	//		iOffsetX = 10;
	//	}
	//	else
	//	{
	//		rtOverlay.SetRect(0, 0, rtIcon.Width() - 100, 26);
	//		iOffsetX = 50;
	//	}
	//	CTPMemoDC  cdcInOut(pDC,rtOverlay.Width(),rtOverlay.Height());
	//	cdcInOut.SetTextColor(RGB(255, 255, 255));
	//	CRect  rtOverTmp =  CRect(0, 0, rtIcon.Width() - iOffsetX*2, iOffsetY);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[0],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(15);
	//	cdcInOut.DrawText(sText[0], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	rtOverTmp.MoveToX(rtOverTmp.Width() / 2);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[1],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(rtOverTmp.Width() / 2+15);
	//	cdcInOut.DrawText(sText[1], rtOverTmp, DT_SINGLELINE|DT_LEFT);

	//	rtOverTmp.MoveToXY(0,iOffsetY);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[2],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(15);
	//	cdcInOut.DrawText(sText[2], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	rtOverTmp.MoveToX(rtOverTmp.Width() / 2);
	//	ImageList_Draw(m_pCommonRes[TP_CLIPICON_ICONOVERLAY] ->GetImage(),iIndex[3],(&cdcInOut)->m_hDC,rtOverTmp.left,rtOverTmp.top+3,ILD_TRANSPARENT);	
	//	rtOverTmp.MoveToX(rtOverTmp.Width() / 2+15);
	//	cdcInOut.DrawText(sText[3], rtOverTmp, DT_SINGLELINE|DT_LEFT);
	//	AlphaBlend(pDC ->m_hDC, rtIcon.left + iOffsetX, rtIcon.top + rtIcon.Height() -iOffsetY*2, rtIcon.Width()-iOffsetX*2, iOffsetY*2,
	//		cdcInOut.m_hDC, 0, 0, rtOverlay.Width(), rtOverlay.Height(), blf);
	//	cdcInOut.DeleteDC();
	//}
}

INT TP_GetResColorIndex(DWORD dwColor)
{
	INT nIndex = 0;
	if((dwColor >= TP_CLIPCOLOR_BASE) && (dwColor < TP_CLIPCOLOR_MAX))
	{
		nIndex = dwColor - TP_CLIPCOLOR_BASE+1;
	}
	return nIndex;
}
void CTPNSResListCtrl::DrawIcon(CDC *pDC,int iItem)
{	
	CRect rtItem,rtOK,rtText,rtClipType,rtState,rtQuality,rtEdge,rtIcon,rtOtherOpen;

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

	rtOK = rtItem;
	rtOK.left += rtEdge.left;
	rtOK.top   = rtOK.top  + (rtEdge.top - m_pCommonRes[TP_CLIPICON_OKNG]->Height())/2 + nTypeOffset;
	rtOK.right = rtOK.left + m_pCommonRes[TP_CLIPICON_OKNG]->Width();	

	rtClipType = rtItem;
	rtClipType.left  = rtOK.right +2;
	rtClipType.top   = rtClipType.top  + (rtEdge.top - m_pCommonRes[TP_CLIPICON_TYPEINDICATORS]->Height())/2 + nTypeOffset;
	rtClipType.right = rtClipType.left + m_pCommonRes[TP_CLIPICON_TYPEINDICATORS]->Width();

	rtOtherOpen = rtItem;
	rtOtherOpen.left  = rtClipType.right +2;
	rtOtherOpen.top   = rtOtherOpen.top  + (rtEdge.top - m_pCommonRes[TP_CLIPICON_TYPEINDICATORS]->Height())/2 + nTypeOffset;
	rtOtherOpen.right = rtOtherOpen.left + m_pCommonRes[TP_CLIPICON_TYPEINDICATORS]->Width();


	rtQuality       = rtItem;
	rtQuality.right-= rtEdge.right;
	rtQuality.left  = rtQuality.right - m_pCommonRes[TP_CLIPICON_STATUSINDICATORS]->Width();
	rtQuality.top   = rtQuality.top  + (rtEdge.top - m_pCommonRes[TP_CLIPICON_STATUSINDICATORS]->Height())/2 + nTypeOffset;

	rtState         = rtItem;
	rtState.right   = rtQuality.left -0;
	rtState.left    = rtState.right - m_pCommonRes[TP_CLIPICON_STATUSINDICATORS]->Width();
	rtState.top     = rtState.top  + (rtEdge.top - m_pCommonRes[TP_CLIPICON_STATUSINDICATORS]->Height())/2 + nTypeOffset;

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
	if(m_lIconSize < 4) GetItemResClipFile(pItemData);   // lihuili 大图标时取文件 
	if(!pItemData ->pItem->bShow) return;
	if(pItemData ->dwState & TP_RESSTATE_HIDE) return;

	CTPGuidLock guidLock(pItemData->guidRes,TP_CLIP_ICON,(pItemData->eResType & TP_RES_NOTE) && m_lIconSize == 4);//防止和底下SetClipDataInfo将ICON删除了 swx

	pDC ->SetBkMode(TRANSPARENT);
	INT nTextColor = COLOR_BASE_TEXT;
	if(pItemData ->dwState & TP_RESSTATE_CUT)	iIndex = TP_CLIPICON_C4;
	else if(uState & LVIS_SELECTED )           
	{
		iIndex     = TP_CLIPICON_S4;
		nTextColor = COLOR_TRIM_BACK;
		if(m_pResExplorer && m_pResExplorer ->m_pFocus != this)
		{
			iIndex     = TP_CLIPICON_D4;
			nTextColor = COLOR_BASE_TEXTFOCUS;
		}
	}
	if(m_lIconSize == 1)      iIndex -= 3*4;
	else if(m_lIconSize == 2) iIndex -= 2*4;
	else if(m_lIconSize == 3) iIndex -= 1*4;
	else if(m_lIconSize == 4) iIndex -= 0*4;
	else                      iIndex -= 3*4;

	pDC->SetTextColor(TP_GetSysColor(nTextColor));

	rtItem.right  = rtItem.left + m_pCommonRes[iIndex] ->Width();
	rtItem.bottom = rtItem.top  + m_pCommonRes[iIndex] ->Height();

	if(pItemData->cResColor > 0) iIndex = iIndex+ 16* TP_GetResColorIndex(pItemData->cResColor);
	TP_SetDIBitsToDevice(pDC,m_pCommonRes[iIndex] ->GetByte(),m_pCommonRes[iIndex] ->Width(),m_pCommonRes[iIndex] ->Height(),rtItem);
	//TP_GetItemRefCount(pItemData);
	if(pItemData && (pItemData ->eResType & TP_RES_CATALOG ))
	{
		int   iIndex  = pItemData ->iNPIndex;
		CRect rtImage = rtIcon;
		rtImage.left  = rtImage.left + (rtImage.Width() - m_pCommonRes[TP_CLIPICON_FLODER]->Width())/2;
		rtImage.top   = rtImage.top  + (rtImage.Height()- m_pCommonRes[TP_CLIPICON_FLODER]->Height())/2;
		rtImage.right  = rtImage.left + m_pCommonRes[TP_CLIPICON_FLODER]->Width();
		rtImage.bottom  = rtImage.top + m_pCommonRes[TP_CLIPICON_FLODER]->Height();		
		ImageList_Draw(m_pCommonRes[TP_CLIPICON_FLODER] ->GetImage(),iIndex,pDC->m_hDC,rtImage.left,rtImage.top ,ILD_TRANSPARENT);
		pDC ->DrawText(pItemData ->sText[0],rtText,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS);
	}
	else if(pItemData && (pItemData ->eResType & TP_RES_NOTE))
	{
		if(m_lIconSize <4) //大buffer
		{
			if(pItemData ->bIs0403Icon)
			{
				pDC->FillSolidRect(rtIcon,RGB(0,0,0));
				rtIcon.left  = rtIcon.left + (rtIcon.Width() - rtIcon.Height() *4/3)/2;
				rtIcon.right = rtIcon.left + rtIcon.Height() *4/3;
			}

			LPBYTE pDrawBuffer  = NULL;
			CSize  szDrawBuffer = CSize(0, 0);
			if(pItemData->lListItemFlag & TP_ITEMFLAG_DEFAULTNOTE)
			{
				pDrawBuffer  = m_pCommonRes[TP_CLIPICON_OFFLINELARGE]->GetByte();
				szDrawBuffer = CSize(m_pCommonRes[TP_CLIPICON_OFFLINELARGE]->Width(), m_pCommonRes[TP_CLIPICON_OFFLINELARGE]->Height());
			}
			else if(pItemData->lListItemFlag & TP_ITEMFLAG_VIDEO)
			{
				if(!pItemData->bGetBuffer)
				{
					TPGetIconItem stuGet;
					if(pItemData ->pBuffer && (pItemData ->szBuffer.cx != rtIcon.Width() || pItemData ->szBuffer.cy != rtIcon.Height()))
					{
						delete [] pItemData ->pBuffer;
						pItemData ->pBuffer = NULL;
					}
					if(pItemData ->pBuffer == NULL)
					{										
						stuGet.pBuffer  = new BYTE[rtIcon.Width() * rtIcon.Height() *sizeof(DWORD)];
						ZeroMemory(stuGet.pBuffer,rtIcon.Width() * rtIcon.Height() *sizeof(DWORD));
						pItemData ->pBuffer  = stuGet.pBuffer;				
						pItemData ->szBuffer = CSize(rtIcon.Width(),rtIcon.Height());
						stuGet.szBuffer      = pItemData ->szBuffer; 
					}
					else
					{
						stuGet.pBuffer  = pItemData ->pBuffer; 
						stuGet.szBuffer = pItemData ->szBuffer; 
					}

					// 保存首帧图像
					if(pItemData ->szIcon.cx > 0 && pItemData ->szIcon.cy > 0 && pItemData->pIcon && pItemData->iFrame < 0)
					{
						pItemData ->bGetBuffer = TRUE;
						TP_TransBuffer((LPDWORD)pItemData ->pBuffer,pItemData ->szBuffer.cx,pItemData ->szBuffer.cy,(LPDWORD)pItemData ->pIcon,pItemData ->szIcon.cx,pItemData ->szIcon.cy);  
					}
					else if(pItemData ->sFileName.GetLength() > 0 && pItemData->iFrame >= 0)
					{
						stuGet.sName    = pItemData ->sFileName;
						stuGet.sKeyName = pItemData ->sKeyFileName;
						stuGet.iFrame   = pItemData ->iFrame;
						stuGet.hWnd     = GetSafeHwnd();
						GetClientRect(stuGet.rtDraw);
						if(pItemData->pGraphScaleEx)
						{
							TPGetIconScale *pScale = new TPGetIconScale();
							pScale->b1609 = FALSE;
// 							pScale->dwPicturePos = pItemData->pGraphScaleEx->ePictureType;
							pScale->dCenterX   = pItemData->pGraphScaleEx->dCenterX;
							pScale->dCenterY   = pItemData->pGraphScaleEx->dCenterY;
							pScale->dScaleX    = pItemData->pGraphScaleEx->dScaleX;
							pScale->dScaleY    = pItemData->pGraphScaleEx->dScaleY;
							pScale->ptChipTopLeft     = pItemData->pGraphScaleEx->ptChipTopLeft;
							pScale->ptChipRightBottom = pItemData->pGraphScaleEx->ptChipRightBottom;
							pScale->eVideoStandard    =	pItemData->dwVideoStand;
							stuGet.pScale = pScale;
						}
						if(g_pGetIconFun ->GetIcon(&stuGet)) pItemData ->bGetBuffer = TRUE;
					}
				}
				pDrawBuffer  = pItemData->pBuffer;
				szDrawBuffer = pItemData->szBuffer;
			}
			else if(pItemData->lListItemFlag & TP_ITEMFLAG_AUDIO)
			{
				pDrawBuffer  = m_pCommonRes[TP_CLIPICON_AUDIO] ->GetByte();;
				szDrawBuffer = CSize(m_pCommonRes[TP_CLIPICON_AUDIO] ->Width(),m_pCommonRes[TP_CLIPICON_AUDIO] ->Height());
			}

			if(NULL != pDrawBuffer)
			{

				TP_StretchDIBits(pDC,pDrawBuffer,szDrawBuffer.cx,szDrawBuffer.cy,rtIcon,!(pItemData->dwState & TP_RESSTATE_OVERTURNICON));		
			}
			else
			{
				pDC->FillSolidRect(rtIcon,RGB(0,0,0));
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
			TP_StretchDIBits(pDC,pItemData ->pIcon,pItemData ->szIcon.cx,pItemData ->szIcon.cy,rtIcon,!(pItemData->dwState & TP_RESSTATE_OVERTURNICON));		
		}
		else pDC->FillSolidRect(rtIcon,RGB(0,0,0));

		//Draw OK NG 
		ImageList_Draw(m_pCommonRes[TP_CLIPICON_OKNG] ->GetImage(),pItemData ->dwUseState,pDC->m_hDC,rtOK.left,rtOK.top ,ILD_TRANSPARENT);

		// Draw Clip Type
		int nIndex = TP_GetNoteTypeIconIndex((TP_RES_TYPE)pItemData->eResType, pItemData->eSingSourceType, pItemData);
		ImageList_Draw(m_pCommonRes[TP_CLIPICON_TYPEINDICATORS] ->GetImage(), nIndex,pDC->m_hDC,rtClipType.left,rtClipType.top ,ILD_TRANSPARENT);

		int iQuality = TP_CLIPSTATE_NOHIGHANDPROXYCLIPS;		
		ImageList_Draw(m_pCommonRes[TP_CLIPICON_STATUSINDICATORS] ->GetImage(),iQuality,pDC->m_hDC,rtQuality.left,rtQuality.top ,ILD_TRANSPARENT);
		// Draw Used Flags

		// Draw Name
		pDC ->DrawText(pItemData ->sText[0],rtText,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS);

		// (liupingTFS1472) 建议把在素材选中时时码显示稍微往上移动.
		if((uState & LVIS_SELECTED && m_pResExplorer && m_pResExplorer ->m_pFocus == this)|| (pItemData && pItemData ->dwState & TP_RESSTATE_CUT))           
			rtIconEx.bottom -= 2;
		DrawIconOverlay(pDC,iItem, rtIconEx); 
	}

}

void CTPNSResListCtrl::FlashTheQualityInIcon(int iSize)
{
	//TPMutex stuMute(TP_FLASHQUALITY_LOCK);
	//if(bStart)
	//{
	//	if(m_uFlashTimer == 0 && g_aFlashResData.GetSize() > 0)
	//		m_uFlashTimer = SetTimer(TP_EVENT_FLASH,TP_FASHICON_ESCIP, NULL);
	//}
	//else
	//{
	//	if(g_aFlashResData.GetSize() <= 0)
	//	{
	//		KillTimer(TP_EVENT_FLASH);
	//		m_uFlashTimer = 0;
	//		InvalidateRect(NULL);//XN00026288
	//	}
	//}
}

void CTPNSResListCtrl::OnCmdMsg(UINT uID)
{
	if(m_dwWndType == TP_WND_RESEXPLORER)      OnCmdMsgClip(uID);
//	else if(m_dwWndType == TP_WND_UNKONWEXPLORER)   OnCmdMsgFx(uID);
}
void CTPNSResListCtrl::OnCmdMsgClip(UINT uID)
{	
	switch(uID)
	{
	case ID_RESLIST_NEWBIN:
		{		
			HRESDATA     hCatalogData = m_hCatalog;
			m_hSaveData  = m_hCatalog;
			CString   sName = _T("");
			sName = g_pClipManageInterface ->stuCatalogInterface.TP_CheckCatalogName(hCatalogData,TP_LoadStr(_L("New Bin")));

			TPCatalogData stuParentCatalogInfo,stuCatalogInfo;
			if(g_pClipManageInterface ->stuCatalogInterface.TP_GetCatalogInfo(m_hSaveData,&stuParentCatalogInfo)!=S_OK)
			{ m_hSaveData = NULL; return;}

			stuCatalogInfo.guidNode              = stuParentCatalogInfo.guidRes;
			CoCreateGuid(&stuCatalogInfo.guidRes);   				
			stuCatalogInfo.eNodeType             = stuParentCatalogInfo.eNodeType ;
			stuCatalogInfo.eNodeType |= TP_NODE_BIN;;	
			stuCatalogInfo.guidUserCreate        = m_pUserEnterInfo->guidUserConfigration;
			stuCatalogInfo.sName                 = (LPTSTR)(LPCTSTR)sName;
			::GetLocalTime(&stuCatalogInfo.tmCreate);
			stuCatalogInfo.sKeyWord            = _L("");
			stuCatalogInfo.sDescription        = _L("");	
			stuCatalogInfo.stuShareInfo        = stuParentCatalogInfo.stuShareInfo;
			if(g_pClipManageInterface ->stuCatalogInterface.TP_CreateCatalog(&stuCatalogInfo,m_hSaveData, hCatalogData)!=S_OK)
			{ m_hSaveData = NULL; return;}
			m_hSaveData = NULL;
			m_guidEditRes = stuCatalogInfo.guidRes;
			SelectItem(-1,FALSE,TRUE);
		}
		break;
	case ID_VIEWMODE_PICTUREVIEW:
		{		
			if(m_iViewType != VIEWTYPE_ICON)
			{
				EndEdit();
				m_iTopX = 0; m_iTopY = 0;
				m_iViewType = VIEWTYPE_ICON;
				if(m_aColumn.GetSize()>0) m_aColumn[GetItemByOrder(0)]->lMinWidth = 0;
				FunItemIconIndex(FALSE);
				ReLayOut();		
				m_aCatalogViewType.SetAt(m_hCatalog,(WORD)m_iViewType);	
				g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
				if(m_pResExplorer)		m_pResExplorer->SetToolBarState();
			}
			else m_pResExplorer->m_pToolBar->SetCheck(ID_VIEWMODE_PICTUREVIEW,TRUE);
		}
		break;
	case ID_VIEWMODE_TEXTVIEW:
		{		
			if(m_iViewType != VIEWTYPE_REPORT)
			{
				EndEdit();
				m_iTopX = 0;m_iTopY = 0;
				if(m_aColumn.GetSize()>0) m_aColumn[GetItemByOrder(0)]->lMinWidth = 0;

				m_iViewType = VIEWTYPE_REPORT;
				FunItemIconIndex(FALSE);

				ReLayOut();		
				m_aCatalogViewType.SetAt(m_hCatalog,(WORD)m_iViewType);		
				g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
				if(m_pResExplorer)		m_pResExplorer->SetToolBarState();
			}
			else m_pResExplorer->m_pToolBar->SetCheck(ID_VIEWMODE_TEXTVIEW,TRUE);
		}
		break;
	case ID_VIEWMODE_PICTUREANDTEXTVIEW:
		{		
			if(m_iViewType != VIEWTYPE_TEXTPICTURE)
			{
				m_iTopX = 0; m_iTopY = 0;
				EndEdit();
				m_iViewType = VIEWTYPE_TEXTPICTURE;
				FunItemIconIndex(FALSE);
				ReLayOut();		
				m_aCatalogViewType.SetAt(m_hCatalog,(WORD)m_iViewType);	
				g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
				if(m_pResExplorer)	m_pResExplorer->SetToolBarState();
			}
			else m_pResExplorer->m_pToolBar->SetCheck(ID_VIEWMODE_PICTUREANDTEXTVIEW,TRUE);
		}
		break;	
	case ID_PCITUREICONSIZE_EXTRALARGE:
	case ID_PCITUREICONSIZE_LARGE:
	case ID_PCITUREICONSIZE_MEDIUM:
	case ID_PCITUREICONSIZE_SMALL:
		{		
			int iIndxe = 0;
			if(uID ==  ID_PCITUREICONSIZE_EXTRALARGE) {	iIndxe = TP_CLIPICON_N1;	m_lIconSize = 1;}
			else if(uID ==  ID_PCITUREICONSIZE_LARGE) {	iIndxe = TP_CLIPICON_N2;	m_lIconSize = 2;}
			else if(uID ==  ID_PCITUREICONSIZE_MEDIUM){	iIndxe = TP_CLIPICON_N3;	m_lIconSize = 3;}
			else if(uID ==  ID_PCITUREICONSIZE_SMALL) {	iIndxe = TP_CLIPICON_N4;	m_lIconSize = 4;}
			g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
			m_szItemIcon = CSize(m_pCommonRes[iIndxe]->Width(),m_pCommonRes[iIndxe]->Height());

			for(INT l=0;l<m_aItem.GetSize();l++)
			{
				TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(l);
				if(pItemData )pItemData ->bGetBuffer = FALSE;
			}
			ReLayOut();
			Invalidate();
		}
		break;
	case  ID_SYSCMD_EDITDAILYNOTE:
		{
			((CTPNSResTreeCtrl *)m_pResTreeCtrl)->OnCmdMsg(uID);
		}
		break;
	case  ID_SYSCMD_NEWDAILYNOTE:
		{

			break;
		}
	default:
		break;
	}
}
//	switch(uID)
//	{	
//	case ID_DROPMENU_COPY:
//		{
//			//Get the destinative catalog
//
//			int  iItem = -1;
//			iItem = CTPListCtrlEx::HitTest(m_ptRButtonUp);
//			TPListItemData *pItemData = (TPListItemData *)GetItemData(iItem);	
//			INT nOffset = 0;
//
//			TPResDataArray aResData;
//			aResData.Copy(m_aDropData);
//
//			HRESDATA hCatalog = m_hCatalog;
//
//			if(pItemData && (pItemData ->eResType & TP_RES_CATALOG)) hCatalog = pItemData->hResData;
//			TP_LogEditNotify(g_pSystemInfo,logLevelInfo,logLevel_0,logLevelOperte, _T("Begin Copy Res 2"), _T("CopyResData"));
//			CopyResTo(hCatalog, aResData, m_ptRButtonUp);
//			TP_LogEditNotify(g_pSystemInfo,logLevelInfo,logLevel_0,logLevelOperte, _T("End Copy Res 2"), _T("CopyResData"));
//			
//			CTPListCtrlEx::SelectItem(-1, TRUE, TRUE);
//			TP_RemoveResDataArray(m_aDropData, TRUE);
//			if(m_eNodeType & TP_NODE_USERINFO)
//				((CTPNSResTreeCtrl *) m_pResTreeCtrl) ->SelectItemChange(m_pResTreeCtrl->GetSelectedItem());
//		}
//		return;
//	case ID_DROPMENU_CUT:
//		{		
//			//Get the destinative catalog
//			int  iItem = -1;
//			iItem = CTPListCtrlEx::HitTest(m_ptRButtonUp);
//			TPListItemData *pListData = (TPListItemData *)GetItemData(iItem);	
//
//			if(NULL != pListData && (TP_NODE_TRASH & pListData->eNodeType))
//			{
//				LRESULT lResult = S_FALSE;
//				if(m_aDropData.GetSize() <= 0) return;
//				if(TP_NODE_RECYCLE & m_aDropData[0].eNodeType) return;
//				lResult = g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(pListData ->hResData,&m_aDropData,TRUE);
//
//				if(lResult == S_DECLINE) 
//				{
//					if(lstrlen(g_pClipManageInterface->TP_GetLastErrorInfo())>0)
//					{
//						AfxMessageBox(g_pClipManageInterface->TP_GetLastErrorInfo());
//					}
//					TP_RemoveResDataArray(m_aDropData, TRUE);
//					return;
//				}
//				void *pDelData[5];
//				TP_GetDeleteParam(pDelData, TRUE, FALSE);
//
//				lResult = g_pClipManageInterface->stuFuncitonInterface.TP_DeleteResDataTest(m_guidDBType,(TPResDataArray*)&m_aDropData,pDelData);
//				if(S_OK == lResult) 
//					lResult = g_pClipManageInterface->stuFuncitonInterface.TP_DeleteResData(m_guidDBType,(TPResDataArray*)&m_aDropData,pDelData);
//
//				TP_RemoveResDataArray(m_aDropData, TRUE);
//			}
//			else
//			{
//				HRESDATA hCatalog = m_hCatalog;
//				if(pListData && (pListData ->eResType & TP_RES_CATALOG)) hCatalog = pListData ->hResData;
//				LRESULT lResult = g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(hCatalog,&m_aDropData,TRUE);
//				if(lResult == S_ACCEPT)  // Cut
//				{					
//					TP_LogEditNotify(g_pSystemInfo,logLevelInfo,logLevel_0,logLevelOperte, _T("Begin Move Res To Catalog 4"), _T("CopyResData"));
//					g_pClipManageInterface->stuFuncitonInterface.TP_MoveResTo(hCatalog,m_aDropData );
//					TP_LogEditNotify(g_pSystemInfo,logLevelInfo,logLevel_0,logLevelOperte, _T("End Move Res To Catalog 4"), _T("CopyResData"));
//				}
//				else  //Move
//				{
//					BOOL bStoryBoard = TRUE;
//					if(m_iArrType != ARRANGE_STORYBOARD||m_iViewType != VIEWTYPE_ICON) bStoryBoard = FALSE;
//					if(bStoryBoard && m_bDragIn) // bStoryBoard模式 
//					{
//						CRect rtRect(9999,9999,0,0);
//						FunMoveClip(m_ptRButtonUp,FALSE,rtRect,TP_CLIPDRAGTYPE_MOVE);
//						FunSortbyIconIndex();
//						FunCalcSize();
//						Invalidate(FALSE);
//					}
//				}
//				TP_RemoveResDataArray(m_aDropData, TRUE);
//			}
//		}
//		return;
//	case ID_DROPMENU_CANCLE:
//		{
//			m_uPrivateStyle &= ~LIST_DRAGITEM;
//			TP_RemoveResDataArray(m_aDropData, TRUE);
//		}
//		return;
//	case ID_SYSCMD_NEWPROJECTGROUP:		
//	case ID_RESLIST_NEWPROJECTGROUP:
//	case ID_RESLIST_NEWBIN:
//		{		
//			if (!TP_GetResRight(NULL, TP_RES_CATALOG, TP_RIGHT_NEW))	return;
//			BOOL bFolder = uID != ID_RESLIST_NEWBIN ? TRUE : FALSE;
//			if(TP_CanCreateBin(m_eNodeType, bFolder))
//			{
//				CString   sName = _T("");
//				if(bFolder)
//					sName = g_pClipManageInterface ->stuCatalogInterface.TP_CheckCatalogName(m_hCatalog,TP_LoadStr(_L("New Project Group")));
//				else
//					sName = g_pClipManageInterface ->stuCatalogInterface.TP_CheckCatalogName(m_hCatalog,TP_LoadStr(_L("New Bin")));
//
//				// Driectly Create Catalog
//				HRESDATA     hCatalogData = NULL;
//				m_hSaveData  = m_hCatalog;
//				TPResCatalog stuParentCatalogInfo,stuCatalogInfo;
//				if(g_pClipManageInterface ->stuCatalogInterface.TP_GetCatalogInfo(m_hSaveData,&stuParentCatalogInfo)!=S_OK)
//				{ m_hSaveData = NULL; return;}
//
//				stuCatalogInfo.guidNode              = stuParentCatalogInfo.guidRes;
//				CoCreateGuid(&stuCatalogInfo.guidRes);   				
//				stuCatalogInfo.eNodeType             = stuParentCatalogInfo.eNodeType ;
//				if(bFolder)      
//				{
//					stuCatalogInfo.eNodeType            |= TP_NODE_PROJECTFLODER;			
//					stuCatalogInfo.guidUserCreate        = m_pUserEnterInfo->guidTeamSpaceUser;
//				}
//				else													
//				{
//					stuCatalogInfo.eNodeType |= TP_NODE_BIN;;	
//					stuCatalogInfo.guidUserCreate        = m_pUserEnterInfo->guidUserConfigration;
//				}
//				stuCatalogInfo.sName                 = (LPTSTR)(LPCTSTR)sName;
//				::GetLocalTime(&stuCatalogInfo.tmCreate);
//				stuCatalogInfo.sKeyWord            = _L("");
//				stuCatalogInfo.sDescription        = _L("");	
//				stuCatalogInfo.stuSecurityInfo     = stuParentCatalogInfo.stuSecurityInfo;
//				stuCatalogInfo.stuShareInfo        = stuParentCatalogInfo.stuShareInfo;
//				if(g_pClipManageInterface ->stuCatalogInterface.TP_CreateCatalog(&stuCatalogInfo,m_hSaveData, hCatalogData)!=S_OK)
//				{ m_hSaveData = NULL; return;}
//				if(m_pResExplorer && m_pResExplorer->m_pResTreeCtrl) 
//				{
//					m_pResExplorer->m_pResTreeCtrl->m_guidEditItem   = stuCatalogInfo.guidRes;
//					m_pResExplorer->m_pResTreeCtrl->m_guidEditDBType = stuCatalogInfo.guidDBType;
//				}
//				m_hSaveData = NULL;
//				m_guidEditRes = stuCatalogInfo.guidRes;
//				SelectItem(-1,FALSE,TRUE);
//			}
//		}
//		return;
//	case ID_RESLIST_NEWSEQUENCE:
//		if (!TP_GetResRight(NULL, TP_RES_ALLPROGRAM, TP_RIGHT_NEW))	return;
//		((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->m_hMenuItem = m_pResTreeCtrl ->GetSelectedItem();
//		((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->OnCmdMsg(ID_BASEPATH_NEWSEQUENCE);		
//		SelectItem(-1,FALSE,TRUE);
//		return;
//	case ID_CGLIST_ADDEMPTYTEMPLATE:
//		{
//			HRESDATA hProject = g_pClipManageInterface ->TP_GetCurProject();
//			if(hProject == NULL)  return;	
//
//			TPResTemplateNewData stuNewTemplate;
//			CoCreateGuid(&stuNewTemplate.guidRes);
//			CString  sCheckName = g_pClipManageInterface->stuTemplateInterface.TP_CheckTemplateName(m_hCatalog,(LPTSTR)(LPCTSTR)SBLoadString(_T("IDS_CLIPEXPLORER_TEMPLATE")),NULL);
//			stuNewTemplate.sName           = (LPTSTR)(LPCTSTR)sCheckName;
//			stuNewTemplate.eResType        = TP_RES_CGTEMPLATE;
//			stuNewTemplate.eVideoStandard  = g_pSystemInfo->tpVideoStandard;
//
//			//Add new CG template
//			LRESULT lResult = g_pClipManageInterface->stuTemplateInterface.TP_AddTemplate(m_hCatalog,stuNewTemplate);
//
//			HRESDATA hResTempalte;
//			if(g_pClipManageInterface->stuTemplateInterface.TP_GetTemplate(g_pClipManageInterface->TP_GetCurDBType(),GUID_NULL,stuNewTemplate.guidRes, TP_RESLOCK_ALL,hResTempalte) != S_OK) return ;
//			TPTemplateData stuGetTemplateData;
//			if(g_pClipManageInterface->stuTemplateInterface.TP_GetTemplateInfo(hResTempalte,TP_GRADE_BASE|TP_GRADE_EXTDATA,stuGetTemplateData) != S_OK)
//			{
//				g_pClipManageInterface->stuTemplateInterface.TP_ReleaseTemplate(hResTempalte);
//				return;
//			}
//
//			//Create new TL
//			TPTLInitData tpInitData;
//			tpInitData.lVideoTrack     = 0;    
//			tpInitData.lAudioTrack     = 0;    
//			tpInitData.lVideoGrapTrack = 0;
//			tpInitData.lVideoBackTrack = 0;
//			tpInitData.bAudioOut       = FALSE;      
//			tpInitData.dwTLType        = TP_PROGRAM_CGTIMELINE;		
//			tpInitData.dwSubType       = TP_TLSUB_CGTEMPLATE;
//			tpInitData.bVideoAudio     = FALSE; 
//			tpInitData.lAudioPerTrack  = 0;
//			tpInitData.bVideoKey       = FALSE;
//			tpInitData.bVideoEffect    = FALSE;
//
//			HTLDATA hTLData = NULL;
//			CString strFileName  = stuGetTemplateData.sFilePath;
//			strFileName += _T("\\");
//			strFileName += stuGetTemplateData.sFileName;
//
//			LRESULT hResult = g_pTimeLineDataEdit->TP_TLData_New(strFileName,_L(""),_L(""),stuGetTemplateData.guidRes,stuGetTemplateData.guidDBType, tpInitData,hTLData,TP_TLOPEN_NONE);
//			if(hResult !=S_OK)
//			{
//				g_pClipManageInterface->stuTemplateInterface.TP_ReleaseTemplate(hResTempalte);
//				return;
//			}		
//			g_pTimeLineDataEdit->TP_TLData_SetAlias(hTLData,stuGetTemplateData.sName);
//
//			//Set the TL format
//			TPTLFormat stuTLFormat;
//			stuTLFormat.eVideoStandard               = stuGetTemplateData.eVideoStandard;
//			TPProjectData stuProject;
//			g_pClipManageInterface->stuProjectInterface.TP_GetProjectInfo(hProject, TP_GRADE_BASE | TP_GRADE_NS, stuProject);
//			if(stuProject.eVideoStandard >= TP_VIDEOSTANDARD_1920_1080_50i) //写上项目的格式 by zhh
//			{
//				stuTLFormat.guidVideoFileType  = stuProject.stuHighQualityFmt.guidVideoFileType; 
//				stuTLFormat.guidAudioFileType  = stuProject.stuHighQualityFmt.guidAudioFileType; 
//				TP_CopyAudioFormatEx(stuProject.stuHighQualityFmt.stuAudioFormat,stuTLFormat.stuAudioFormat);
//				TP_CopyVideoFormat(stuProject.stuHighQualityFmt.stuVideoFormat,stuTLFormat.stuVideoFormat);
//			}
//			else
//			{
//				stuTLFormat.guidVideoFileType  = stuProject.stuLowQualityFmt.guidVideoFileType; 
//				stuTLFormat.guidAudioFileType  = stuProject.stuLowQualityFmt.guidAudioFileType; 
//				TP_CopyAudioFormatEx(stuProject.stuLowQualityFmt.stuAudioFormat,stuTLFormat.stuAudioFormat);
//				TP_CopyVideoFormat(stuProject.stuLowQualityFmt.stuVideoFormat,stuTLFormat.stuVideoFormat);
//			}
//			stuTLFormat.bDropFrame             = stuProject.bDropFrame;
//			stuTLFormat.guidVideoFileTypePack  = stuProject.stuCompression.guidVideoFileType; 
//			stuTLFormat.guidAudioFileTypePack  = stuProject.stuCompression.guidAudioFileType; 
//			TP_CopyAudioFormatEx(stuProject.stuCompression.stuAudioFormat,stuTLFormat.stuAudioFormatPack);
//			TP_CopyVideoFormat(stuProject.stuCompression.stuVideoFormat,stuTLFormat.stuVideoFormatPack);
//
//			g_pTimeLineDataEdit->TP_TLData_SetTLFormat (hTLData,stuTLFormat);
//			g_pTimeLineDataEdit->TP_TLData_Save(strFileName,NULL,NULL,hTLData);
//			g_pTimeLineDataEdit->TP_TLData_Release(hTLData);
//			hTLData = NULL;
//
//			//Change the tmemplate infomation
//			stuGetTemplateData.lDeviceFileIndex  = 1;
//			stuGetTemplateData.lLength           = 100;
//			g_pClipManageInterface->stuTemplateInterface.TP_SetTemplateInfo(hResTempalte,TP_GRADE_BASE,stuGetTemplateData);
//			g_pClipManageInterface->stuTemplateInterface.TP_SaveTemplate(hResTempalte,TP_GRADE_BASE);
//			g_pClipManageInterface->stuTemplateInterface.TP_ReleaseTemplate(hResTempalte);
//
//		}
//		break;
//	case ID_OUTPUT_RECORDTOVTR:
//	case ID_OUTPUT_EXPORTTOXDCAM:
//	case ID_OUTPUT_RECORDTODVD:
//	case ID_OUTPUT_RECORDTOBIN:
//	case ID_OUTPUT_EXPORT:
//	case ID_OUTPUT_EXPORTAAF:
//	case ID_OUTPUT_EXPORTEDL:
//	case ID_OUTPUT_EXPORTOMF:
//	case ID_OUTPUT_BACKUPTOFILE:
//		TP_SaveCurrentTL(TRUE);
//		((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->m_hMenuItem = m_pResTreeCtrl ->GetSelectedItem();
//		((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->OnCmdMsg(uID);
//		return;
//	case ID_SYSCMD_SGCLIP:
//	case ID_SYSCMD_CREATETITLEMOTION:
//	case ID_SYSCMD_LOGGING:
//	case ID_SYSCMD_IMPORTFILE:
//	case ID_SYSCMD_IMPORTCDAUDIO:
//	case ID_SYSCMD_IMPORTDVD:
//	case ID_SYSCMD_IMPORTMXF:
//	case ID_INPUT_IMPORTEDL:
//	case ID_INPUT_IMPORTALE:
//	case ID_INPUT_IMPORTOMF:
//	case ID_INPUT_IMPORTAAF:
//	case ID_INPUT_RESTOREFROMFILE:
//	case ID_SYSCMD_NEWPROJECT:
//		((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->m_hMenuItem = m_pResTreeCtrl ->GetSelectedItem();
//		((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->OnCmdMsg(uID);
//		return;
//	case ID_OUTPUT_EXPORTHDXSBF:
//		{
//			TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(m_iMenuItem);
//#ifdef TP_NS_NET
//			// 获取节目信息
//			LRESULT         lResult     = S_FALSE;
//			HRESDATA        hPrograData = NULL;
//			TPProgramData	stuProgramData;
//
//			lResult = g_pClipManageInterface->stuProgramInterface.TP_GetProgram( pItemData->guidDBType, GUID_NULL, pItemData->guidRes, TP_RESLOCK_VIEW, hPrograData);
//			if(S_FALSE == lResult || NULL == hPrograData) return;
//			lResult = g_pClipManageInterface->stuProgramInterface.TP_GetProgramInfo(hPrograData, TP_GRADE_BASE, stuProgramData);
//			if(S_FALSE == lResult) return;
//			g_pClipManageInterface->stuProgramInterface.TP_ReleaseProgram(hPrograData);  ///{ 2009-11-6 modified by Guo Rui : S2_NLE : CD2009.10.01
//
//			// 提示子时间线的结构信息无法导出
//			if ( stuProgramData.lTLBlock )
//			{
//				MessageBox( SBLoadString(_T( "TPNSEXPORTXML_SUBSEQUENCE_INFO" )),SBLoadString(_T("IDS_CLIPEXPLORER_INFORMATION")),MB_OK| MB_ICONWARNING );
//			}			
//			if(pItemData && (pItemData->eResType & TP_RES_TLPROGRAM))
//			{			
//				CString sFileFilter =  _T("(*.XML)|*.XML||");
//				CTPFileDialog fileDlg(FALSE,_L(""),_L(""),OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_ENABLESIZING , sFileFilter, NULL);
//				fileDlg.m_sInitFileName = pItemData->sText[0] + _T(".XML");
//				int IDFlag = fileDlg.DoModal();
//				if ( IDFlag == IDOK || IDFlag == IDNO )
//				{
//					CString strFileName = fileDlg.GetPathName();
//					if ( IDNO == IDFlag )
//					{
//						strFileName.Delete( strFileName.GetLength()-4,4 );
//						strFileName += _T("(1).XML");
//						int i=2;
//						while( PathFileExists( strFileName ) ) 
//						{
//							CString stmpstr = strFileName;
//							int indx = stmpstr.ReverseFind('(');
//							stmpstr.Delete( indx,stmpstr.GetLength()-indx );
//							strFileName.Format(_T("%s(%d).XML"),stmpstr,i);
//							i++;
//						}
//					}
//					CString stmpstr = strFileName;
//					stmpstr.MakeLower();
//					stmpstr.TrimLeft();
//					stmpstr.TrimRight();
//					if(stmpstr.Right(4) != _T(".xml"))	strFileName = strFileName + _T(".xml") ;
//
//					// Save TL data Firstly
//					TPManageData* pManageData = (TPManageData*)TP_GetCurManageData();
//					if(NULL != pManageData && NULL != pManageData ->hEditData) 	TP_TLData_Save(pManageData,FALSE);
//
//					// Save To HDXEx
//					g_pEdlManageInterface->TP_SaveTimeLineToHDXEx((LPCTSTR)strFileName,pItemData->guidRes,pItemData->guidDBType); 
//				}
//			}
//#else
//			if(pItemData && (pItemData->eResType & TP_RES_TLPROGRAM))
//			{			
//				CString sFileFilter =  _T("(*.XML)|*.XML||");
//				CTPFileDialog fileDlg(FALSE,_L(""),_L(""),OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_ENABLESIZING , sFileFilter, NULL);
//				fileDlg.m_sInitFileName = pItemData->sText[0] + _T(".XML");
//				if(fileDlg.DoModal() == IDOK)
//				{
//					CString strFileName = fileDlg.GetPathName();
//					CString stmpstr = strFileName;
//					stmpstr.MakeLower();
//					stmpstr.TrimLeft();
//					stmpstr.TrimRight();
//					if(stmpstr.Right(4) != _T(".xml"))	strFileName = strFileName + _T(".xml") ;
//
//					// Save TL data Firstly
//					TPManageData* pManageData = (TPManageData*)TP_GetCurManageData();
//					if(NULL != pManageData && NULL != pManageData ->hEditData) 	TP_TLData_Save(pManageData,FALSE);
//
//					// Save To HDXEx
//					g_pEdlManageInterface->TP_SaveTimeLineToHDXEx((LPCTSTR)strFileName,pItemData->guidRes,pItemData->guidDBType); 
//				}
//			}
//#endif
//		}
//		break;
//	case ID_OUTPUT_IMPORTHDXSBF:
//		{
//			CString sFileFilter =  _T("(*.XML)|*.XML||");
//			CTPFileDialog fileDlg(TRUE,_L(""),_L(""),OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_ENABLESIZING , sFileFilter, NULL);
//			CTPNSHDXSbfImport *pHDXOption = new CTPNSHDXSbfImport();
//			pHDXOption->Create(IDD_NS_HDXSBFIMPORT);
//			fileDlg.SetAdvanceWnd(pHDXOption);
//			if(fileDlg.DoModal() == IDOK)
//			{
//				BOOL bCreateCatalog = pHDXOption->IsCreateCatalog();
//				int          iCout   = fileDlg.GetSelectedCount();
//				if(iCout >0)
//				{				
//					POSITION     posFile = fileDlg.GetStartPosition();
//					CString      sFileName;
//					UINT         uState  = 0;
//					CTPResExplorerPanel::DataStateCallBack(iCout,_L(""),SBLoadString(_L("IDS_CLIPEXPLORER_HDXXMLIMPORT")),TP_STATACALLBACK_SETRANGE);
//					while(posFile)
//					{
//						sFileName = fileDlg.GetNextPathName(posFile);					
//						CTPResExplorerPanel::DataStateCallBack(0,sFileName,SBLoadString(_L("IDS_CLIPEXPLORER_HDXXMLIMPORT")),TP_STATACALLBACK_STEPITUP);	
//						uState = CTPResExplorerPanel::DataStateCallBack(0,_L(""),_L(""), TP_STATACALLBACK_GETSTATE);
//						if(uState == TP_PROCESS_CANCEL || uState == TP_PROCESS_QUIT)  break;						
//						g_pEdlManageInterface->TP_ReadHDXToTimeLineEx(sFileName,m_hCatalog,/*FALSE*/bCreateCatalog);
//					}
//					CTPResExplorerPanel::DataStateCallBack(0,_L(""),_L(""),TP_STATACALLBACK_OK);
//				}
//			}
//
//			if(pHDXOption)
//			{
//				if(NULL != pHDXOption->GetSafeHwnd()) pHDXOption->DestroyWindow();
//				delete pHDXOption;
//				pHDXOption = NULL;
//			}
//		}
//		break;
//	case ID_OUTPUT_EXPORTHDX:
//		{
//			TPResDataArray aResData;
//			POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while(pos)
//			{
//				TPResData stuResData;
//				int nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
//				TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);
//				if(NULL == pItemData) continue;
//				if((TP_RES_NOTE | TP_RES_ALLPROGRAM) & pItemData->eResType)
//				{
//					stuResData.guidRes	  = pItemData->guidRes;
//					stuResData.guidDBType = pItemData->guidDBType;
//					stuResData.eResType   = pItemData->eResType;
//					aResData.Add(stuResData);
//				}
//			}
//
//			if(aResData.GetSize() > 0 && g_stuBrdExplorer.TP_GdiCallBack)
//				g_stuBrdExplorer.TP_GdiCallBack(TP_BRD_COMPILEHDX,(LPARAM)&aResData);
//		}
//		break;
//	case ID_OUTPUT_EXPORTAI3:
//		{
//			if(g_pSystemInfo ->uSystemState & TP_SYSTEMSTATE_XDA)
//			{
//				TPResDataArray aResData;
//				
//				POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//				while(pos)
//				{
//					TPResData stuResData;
//					int nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
//					TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);
//					if(NULL == pItemData) continue;
//			
//					if((TP_RES_NOTE | TP_RES_ALLPROGRAM) & pItemData->eResType)
//					{
//						stuResData.guidRes	= pItemData->guidRes;
//						stuResData.guidDBType = pItemData->guidDBType;
//						stuResData.eResType = pItemData->eResType;
//						aResData.Add(stuResData);
//					}
//				}
//
//				if(aResData.GetSize() > 0 && g_stuBrdExplorer.TP_GdiCallBack)
//					g_stuBrdExplorer.TP_GdiCallBack(TP_BRD_COMPILEAI3,(LPARAM)&aResData);
//			}
//		}
//		break;
//	case ID_OUTPUT_EXPORTMXF: 
//		{
//			if(g_pClipManageInterface->TP_GetCurProject() == NULL || TP_IsProxyEditVersion())   return;
//			TPCatalogData  stuCatalogData;
//			g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(m_hCatalog,&stuCatalogData);
//			if(stuCatalogData.guidProject != g_guidResProject.guidRes)		return;
//			TP_SaveCurrentTL();
//			int   iClip = 0,iSequence = 0,iBin = 0;
//			TPResDataArray aResData;
//			
//			POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while(pos)
//			{
//				TPResData stuResData;
//				int nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
//				TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);
//				if(NULL == pItemData) continue;
//				if(pItemData && (pItemData ->eResType & TP_RES_NOTE)) iClip++;
//				else if(pItemData && (pItemData ->eResType & TP_RES_ALLPROGRAM)) iSequence++;
//				else if(pItemData && (pItemData ->eResType & TP_RES_CATALOG))    iBin++;
//
//				
//				if(pItemData && ((TP_RES_NOTE | TP_RES_ALLPROGRAM) & pItemData->eResType))
//				{
//					stuResData.guidRes	= pItemData->guidRes;
//					stuResData.guidDBType = pItemData->guidDBType;
//					stuResData.eResType = pItemData->eResType;
//					aResData.Add(stuResData);
//				}
//			}
//			if(iBin >= 1)		return;
//
//			if(aResData.GetSize() > 0 && g_stuBrdExplorer.TP_GdiCallBack)
//				g_stuBrdExplorer.TP_GdiCallBack(TP_BRD_COMPILEMXF,(LPARAM)&aResData);
//		}
//		break;
//	case ID_SYSCMD_PASTE:
//		CTPResListCtrl::OnMenuCmd(IDC_LISTPASTE);
//		if(m_pResExplorer)   m_pResExplorer->SetToolBarState();
//		break;
//	case ID_SELECTION_SELECTA:
//		//DQAdb00008692       del by qushaojie 20091209 整合到CTPResListCtrl::OnMenuCmd(ID_VIEWTYPE_SELALL)中 
//		//{  
//		//	CTPListCtrlEx::SetRedraw(FALSE);
//		//	int iCount = CTPListCtrlEx::GetItemCount();
//		//	for(int l=0;l<iCount;l++)
//		//	{		
//		//		// DQAdb00000726 LI HUI LI 2008-06-13 需要计算选中的时间,以便根据选择的顺序排序
//		//		m_aItem[l]->dwSelTick = GetTickCount();
//		//		SelectItem(l, TRUE, FALSE);
//		//	}
//		//	CTPListCtrlEx::SetRedraw(TRUE);
//		//	CTPListCtrlEx::Invalidate();
//		//}
//		CTPResListCtrl::OnMenuCmd(ID_VIEWTYPE_SELALL);
//		return;
//	case ID_SELECTION_INVERTSELECTION:
//		CTPResListCtrl::OnMenuCmd(ID_VIEWTYPE_INVERTSEL);
//		return;
//	case ID_BASEPATH_OPENPROJECTEXSPLORERWITHTHISFOLDER:
//		{
//			g_guidOpenWithThisBin.guidRes = GUID_NULL; // li hui li 2008-09-16 XN00015359
//			POSITION pos = GetFirstSelectedItemPosition();
//			if(pos)
//			{
//				int nItem = GetNextSelectedItem(pos);
//				TPListItemData *pItemData = (TPListItemData*)GetItemData(nItem);
//				if(NULL != pItemData) g_guidOpenWithThisBin.SetAt(pItemData->guidDBType,pItemData->guidRes) ;
//			}
//			((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->OnCmdMsg(uID);
//			g_guidOpenWithThisBin.guidRes = GUID_NULL;
//		}
//		return;
//	case ID_BASEPATH_SEARCH:
//	case ID_SYSCMD_SEARCH:
//		{			
//			((CTPNSResTreeCtrl*)m_pResTreeCtrl)->OnCmdMsg(uID);
//		}
//		return;
//	case ID_BASEPATH_PROJECTPROPERTIES:
//		{
//			if(this->m_iMenuItem == -1) return;
//			TPListItemData *pListItemData = ( TPListItemData *)CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(pListItemData && (pListItemData ->eResType & TP_RES_PROJECT))
//			{
//				CTPNSCreateProjectSheet dlgDo(SBLoadString(_T("IDS_CLIPEXPLORER_PROJECTPROPERTY")));
//				dlgDo.m_hCatalogData = pListItemData->hResData;
//				dlgDo.m_guidProject  = pListItemData ->guidRes;	
//				dlgDo.m_guidDBType	 = pListItemData ->guidDBType;
//				dlgDo.DoModal();
//			}
//			// 修改属性时刷新当前选中的Tree节点
//			for(INT l=0;l<g_aNSClipExplorerPanel.GetSize();l++)
//			{	
//				if(g_aNSClipExplorerPanel[l]->m_dwWndType == TP_WND_RESEXPLORER)
//				{
//					HTREEITEM hSelect = g_aNSClipExplorerPanel[l]->m_pResTreeCtrl ->GetSelectedItem();
//					if(NULL == hSelect) continue;
//					g_aNSClipExplorerPanel[l]->m_pResTreeCtrl->SelectItemChange(hSelect);
//				}
//			}
//		}
//		return;
//	case ID_RESLIST_ARRANGETOPLEFT:
//		{
//			FunItemIconIndex(2);
//			if(m_iViewType == VIEWTYPE_ICON) ReLayOut();
//		}
//		return;
//	case ID_VIEWMODE_ARRANGEWAY:
//		{
//			if(m_iArrType == ARRANGE_AUTO)              this ->OnCmdMsgClip(ID_VIEWMODE_STORYBOARDVIEW);
//			else if(m_iArrType == ARRANGE_STORYBOARD)   this ->OnCmdMsgClip(ID_VIEWMODE_AUTOARRANGE);
//		}
//		break;
//	case ID_VIEWMODE_AUTOARRANGE:
//		if(m_iArrType != ARRANGE_AUTO)
//		{	
//			m_iTopX = 0; m_iTopY = 0;
//			m_iArrType = ARRANGE_AUTO;
//			FunItemIconIndex(FALSE);
//			if(m_iViewType == VIEWTYPE_ICON) ReLayOut();
//			g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
//			if(m_pResExplorer) m_pResExplorer->SetToolBarState(); 
//		}
//		else m_pResExplorer->m_pToolBar->SetCheck(ID_VIEWMODE_AUTOARRANGE,TRUE);
//		break;
//	case ID_VIEWMODE_STORYBOARDVIEW:
//		if(m_iArrType != ARRANGE_STORYBOARD)
//		{		
//			m_iTopX = 0; m_iTopY = 0;
//			m_iArrType = ARRANGE_STORYBOARD;
//			FunItemIconIndex(FALSE);
//			if(m_iViewType == VIEWTYPE_ICON) ReLayOut();
//			g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
//			if(m_pResExplorer)		m_pResExplorer->SetToolBarState();// LI HUI LI 2007-10-31
//		}
//		else m_pResExplorer->m_pToolBar->SetCheck(ID_VIEWMODE_STORYBOARDVIEW,TRUE);
//		break;
//	case ID_VIEWMODE_PICTUREVIEW:
//		{		
//			if(m_iViewType != VIEWTYPE_ICON)
//			{
//				EndEdit();
//				m_iTopX = 0; m_iTopY = 0;
//				m_iViewType = VIEWTYPE_ICON;
//				if(m_aColumn.GetSize()>0) m_aColumn[GetItemByOrder(0)]->lMinWidth = 0;
//				FunItemIconIndex(FALSE);
//				ReLayOut();		
//				m_aCatalogViewType.SetAt(m_hCatalog,(WORD)m_iViewType);	
//				g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
//				if(m_pResExplorer)		m_pResExplorer->SetToolBarState();
//			}
//			else m_pResExplorer->m_pToolBar->SetCheck(ID_VIEWMODE_PICTUREVIEW,TRUE);
//		}
//		break;
//	case ID_VIEWMODE_TEXTVIEW:
//		{		
//			if(m_iViewType != VIEWTYPE_REPORT)
//			{
//				EndEdit();
//				m_iTopX = 0;m_iTopY = 0;
//				if(m_aColumn.GetSize()>0) m_aColumn[GetItemByOrder(0)]->lMinWidth = 0;
//
//				m_iViewType = VIEWTYPE_REPORT;
//				FunItemIconIndex(FALSE);
//
//				ReLayOut();		
//				m_aCatalogViewType.SetAt(m_hCatalog,(WORD)m_iViewType);		
//				g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
//				if(m_pResExplorer)		m_pResExplorer->SetToolBarState();
//			}
//			else m_pResExplorer->m_pToolBar->SetCheck(ID_VIEWMODE_TEXTVIEW,TRUE);
//		}
//		break;
//	case ID_VIEWMODE_PICTUREANDTEXTVIEW:
//		{		
//			if(m_iViewType != VIEWTYPE_TEXTPICTURE)
//			{
//				m_iTopX = 0; m_iTopY = 0;
//				EndEdit();
//				m_iViewType = VIEWTYPE_TEXTPICTURE;
//				FunItemIconIndex(FALSE);
//				ReLayOut();		
//				m_aCatalogViewType.SetAt(m_hCatalog,(WORD)m_iViewType);	
//				g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
//				if(m_pResExplorer)	m_pResExplorer->SetToolBarState();
//			}
//			else m_pResExplorer->m_pToolBar->SetCheck(ID_VIEWMODE_PICTUREANDTEXTVIEW,TRUE);
//		}
//		break;	
//	case ID_PCITUREICONSIZE_EXTRALARGE:
//	case ID_PCITUREICONSIZE_LARGE:
//	case ID_PCITUREICONSIZE_MEDIUM:
//	case ID_PCITUREICONSIZE_SMALL:
//		{		
//			int iIndxe = 0;
//			if(uID ==  ID_PCITUREICONSIZE_EXTRALARGE) {	iIndxe = TP_CLIPICON_N1;	m_lIconSize = 1;}
//			else if(uID ==  ID_PCITUREICONSIZE_LARGE) {	iIndxe = TP_CLIPICON_N2;	m_lIconSize = 2;}
//			else if(uID ==  ID_PCITUREICONSIZE_MEDIUM){	iIndxe = TP_CLIPICON_N3;	m_lIconSize = 3;}
//			else if(uID ==  ID_PCITUREICONSIZE_SMALL) {	iIndxe = TP_CLIPICON_N4;	m_lIconSize = 4;}
//			g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogViewType(m_hCatalog,(long)m_iViewType,m_iArrType,m_lIconSize,FALSE);
//			m_szItemIcon = CSize(m_pCommonRes[iIndxe]->Width(),m_pCommonRes[iIndxe]->Height());
//
//			for(INT l=0;l<m_aItem.GetSize();l++)
//			{
//				TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(l);
//				if(pItemData )pItemData ->bGetBuffer = FALSE;
//			}
//			ReLayOut();
//			Invalidate();
//		}
//		break;
//	case ID_RESLIST_UPDATE:
//		((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->SelectItemChange(m_pResTreeCtrl ->GetSelectedItem());
//		return;
//	case ID_RESLISTSELECT_OPEN:	
//		{		
//			int iState = -1; //0:IDC_CLIP_EDIT 1:ID_RESLISTSELECT_PICTUREICONPLAY
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(NULL == pItemData || pItemData->eNodeType & TP_NODE_RECYCLE) return;
//
//			CTPAutoWaitCursor stuWaitCur;
//			if(pItemData ->eResType & (TP_RES_NOTE| TP_RES_ALLCGTEMPLATE)) 
//			{
//				HMANAGEDATA pManageData = TP_GetCurManageData();
//				if(pManageData == NULL)
//				{
//					iState = 1;
//				}
//				else
//				{
//					if(TP_IsCtrlKeyDown())
//					{
//						if(g_pClipManageSet->cDbCtrlClip == 1)	         iState = 0;  
//						else if(g_pClipManageSet->cDbCtrlClip == 0)      iState = 1;
//					}
//					else
//					{
//						if(g_pClipManageSet->cDbClip == 1)	             iState = 0; 
//						else if(g_pClipManageSet->cDbClip == 0)          iState = 1;
//					}
//
//					//{liubo add CD2009.10.01 2010-8-23 如果是Download Editing模式，Public下的素材只能从Picture Icon Play打开。
//#ifdef TP_NS_NET
//					if (pItemData->guidDBType != g_pClipManageInterface->TP_GetCurDBType())
//					{
//						iState = 1;
//					}
//#endif
//					//}
//
//				}
//			}
//			else if(pItemData ->eResType & TP_RES_ALLPROGRAM) 
//			{
//				//Get current opened project
//				TPCatalogData stuPrjData;
//				HRESDATA hProject = g_pClipManageInterface ->TP_GetCurProject();
//				g_pClipManageInterface ->stuCatalogInterface.TP_GetCatalogInfo(hProject,&stuPrjData);
//				//Get the program infomation
//				TPProgramData stuProData;		
//				g_pClipManageInterface ->stuProgramInterface.TP_GetProgramInfo(pItemData ->hResData,TP_GRADE_BASE,stuProData);			
//
//				//The program doesn't belong the current opened project
//				if(stuPrjData.guidRes != stuProData.guidProject)
//				{
//					if(TP_IsCtrlKeyDown())
//					{
//						if(0 == g_pClipManageSet->cDbCtrlSequence)       iState = 1;
//					}
//					else
//					{
//						if(0 == g_pClipManageSet->cDbSequence)		     iState = 1;
//					}
//				}
//				else
//				{
//					if(TP_IsCtrlKeyDown())
//					{
//						if(g_pClipManageSet->cDbCtrlSequence == 1)       iState = 2;
//						else if(g_pClipManageSet->cDbCtrlSequence == 0)  iState = 1;
//					}
//					else
//					{
//						if(g_pClipManageSet->cDbSequence == 1)           iState = 2;
//						else if(g_pClipManageSet->cDbSequence == 0)      iState = 1;
//					}
//				}
//			}
//
//			if ( 0 == iState)
//			{
//				//选中多个item时，屏蔽回车快捷键响应，但不能屏蔽Ctrl+double.为了与A-CODE一致 qushaojie XN00024380
//				if(!TP_IsCtrlKeyDown())
//				{
//					int iCount = CTPListCtrlEx::GetSelectedCount();	
//					if (iCount > 1) return;
//				}
//				CTPResListCtrl::OnMenuCmd(IDC_CLIP_EDIT);
//			}
//			else if ( 1 == iState)
//			{
//				OnCmdMsgClip(ID_RESLISTSELECT_PICTUREICONPLAY);
//			}
//			else if ( 2 == iState)
//			{
//#ifdef TP_NS_NET
//				if (g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(pItemData->hResData, TP_RES_TLPROGRAM, TP_RIGHT_EDIT))
//#endif				 
//				{
//					if(pItemData->  uResFlag & TP_TLSUB_MUTECARMRE )
//					{
//						TP_TLData_OpenMultiView(pItemData->guidRes,pItemData->guidDBType);
//					}
//					else
//					{
//						CGuidArray aOpenGuid;
//						aOpenGuid.Add(pItemData->guidRes);
//						CTPBaseFrame::m_uBaseFlag |=TP_BASESTATE_NOACTPREWND ; 
//						g_stuClipExpolrerCallBack.TP_TLData_Open(TP_PROGRAM_TIMELINE,NULL,pItemData->guidDBType,&aOpenGuid);	
//						CTPBaseFrame::m_uBaseFlag &=~TP_BASESTATE_NOACTPREWND ;
//						CWnd *pWnd = TP_GetEditGuiWnd(_T("TimeLineWnd"));
//						if(pWnd) pWnd->PostMessage(WM_ACTIVATE,WA_ACTIVE ,0);
//					}
//				} 
//			}
//		}
//
//		return;
//	case ID_RESLISTSELECT_MULTICAMERA:
//		{
//			TPResLockView    stuLock;
//
//			CString sErrorMsg = _T("");
//			TPResDataArray aResData;
//			if(TP_MC_Edit(aResData)!=S_OK)	 
//				return;
//			
//			BOOL bSystemLog = FALSE;
//			POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while(NULL != pos && aResData.GetSize()<9)//XN00029159 jw 20101103
//			{
//				int nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
//				if(nItem < 0) continue;
//				TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);
//				if(NULL == pItemData) continue;
//				if(pItemData->eResType & (TP_RES_NOTE|TP_RES_ALLPROGRAM))
//				{
//					// XN00029261 如果正在采集的VTR素材，不做MutilCamera
//					if((TP_RES_NOTE & pItemData->eResType) && ((TP_CLIP_SOURCE_TYPE_LIVECLIPS|TP_CLIP_SOURCE_TYPE_VAVTR|TP_CLIP_SOURCE_TYPE_VANOVTR) & pItemData->eSingSourceType))
//					{
//						TPClipData stuClipData;
//						if(S_OK != g_pClipManageInterface->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData, TP_GRADE_STATEINFO, stuClipData)) continue;
//						if(TP_IsClipUploading(stuClipData.dwCaptureFlag)) {bSystemLog = TRUE; continue;}
//					}
//					TPResData stuResData;
//					stuResData.eResType   = pItemData->eResType;
//					stuResData.guidRes    = pItemData->guidRes;
//					stuResData.guidDBType = pItemData->guidDBType;
//					stuResData.hResData   = pItemData->hResData;
//					stuResData.wParam     = (WPARAM)m_pResExplorer->m_pCurFrame->m_hBaseWnd;
//					aResData.Add(stuResData);
//				}
//			}
//			if(aResData.GetSize() > 0)
//			{
//				TP_StopCurDataState();
//				HMANAGEDATA hCurManageData = TP_GetCurManageData();
//				if(NULL != hCurManageData) TP_EditEngine_StopSeek_Ex(hCurManageData);
//				TP_SaveCurrentTL(TRUE);
//				TP_MC_Edit(aResData);
//			}
//			else if(bSystemLog)	   // XN00029810
//			{
//				//TP_IsSystemLoggingOperator();
//			}
// 
//		}
//		break;
//	case ID_LOAD_INTOSOURCEVIEWER:
//		{	
//			// XN00008613 LI HUI LI 2008-04-18 Load to SV 之前检查是否正在Trim
//			if(S_OK == g_stuClipExpolrerCallBack.TP_TimeLineCallBack(TP_TIMELINE_GETEDITRESSTATE,NULL)) return;
//
//			CString sErrorMsg = _T("");
//			TPResDataArray aResData;
//			POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while(NULL != pos)
//			{
//				int nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
//				if(nItem < 0) continue;
//				TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);
//				if(NULL == pItemData) continue;
//				if(pItemData->eNodeType & TP_NODE_RECYCLE) continue;  // li hui li XN00028826
//				if(pItemData->eResType & TP_RES_NOTE)
//				{				
//					TPResData stuResData;
//					stuResData.eResType		= pItemData->eResType;
//					stuResData.guidRes		= pItemData->guidRes;
//					stuResData.guidDBType   = pItemData->guidDBType;
//					stuResData.hResData     = pItemData->hResData;
//					aResData.Add(stuResData);
//				}
//				else if(TP_RES_ALLPROGRAM & pItemData->eResType)
//				{
//					GetItemResData(pItemData);
//					//,如果没有打开项目或时间线,则禁用Load Into Source View XN00006122
//					if(GUID_NULL == g_guidResProject.guidRes || NULL == TP_GetCurManageData())	return ;
//					/*	if(g_pClipManageInterface ->TP_GetResLockCount(pItemData ->hResData,(TP_RESLOCK_FLAG)(TP_RESLOCK_WRITE|TP_RESLOCK_ALL))>0)
//					{
//					CString sError = pItemData ->sText[0];					
//					sError += _L("\n");
//					sError += TP_LoadStr(_L("已经打开"));
//					sErrorMsg += sError + _T("\n");
//					continue;
//					}*///XN00006121
//					TPResData stuResData;
//					stuResData.eResType		= pItemData->eResType;
//					stuResData.guidRes		= pItemData->guidRes;
//					stuResData.guidDBType   = pItemData->guidDBType;
//					stuResData.hResData     = pItemData->hResData;
//					aResData.Add(stuResData);
//				}
//			}
//
//			if(aResData.GetSize() > 0)
//			{
//				TP_StopCurDataState();
//				HMANAGEDATA hCurManageData = TP_GetCurManageData();
//				if(NULL != hCurManageData) TP_EditEngine_StopSeek_Ex(hCurManageData);
//				if(S_OK == TP_Res_Edit(aResData))
//				{
//					extern void  TP_SetRecentClip(TPResDataArray &aClip);
//					TP_SetRecentClip(aResData);
//				}
//				aResData.RemoveAll();
//			}
//
//			if(!sErrorMsg.IsEmpty())
//			{
//				MessageBox(sErrorMsg,SBLoadString(_T("IDS_CLIPEXPLORER_MENUOPENPROGRAM")),MB_OK|MB_ICONINFORMATION);
//			}
//		}
//		return;
//	case ID_RESLISTSELECT_BACKTOCREATOR:
//		{
//			//qushaojie 20090923 DQAdb00007283
//			if(m_eNodeType & TP_NODE_SEARCH) return;
//			if(CTPListCtrlEx::GetSelectedCount() != 1) return;
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(pItemData == NULL)    return;
//
//			TP_SaveCurrentTL();
//
//			if(pItemData &&(pItemData ->eResType & TP_RES_NOTE)) 
//			{
//				m_pResExplorer ->m_uState |= TP_BACKTOCREATE;
//				extern LRESULT TP_BackToCreateClip(HRESDATA hClip, BOOL bFromPE);
//				TP_BackToCreateClip(pItemData->hResData, TRUE);
//				m_pResExplorer ->m_uState &= ~TP_BACKTOCREATE;
//			}		
//			else if(pItemData && pItemData ->eResType & TP_RES_ALLPROGRAM) 
//			{
//				CGuidArray aOpenGuid;
//				aOpenGuid.Add(pItemData->guidRes);
//				CTPBaseFrame::m_uBaseFlag |=TP_BASESTATE_NOACTPREWND ; 
//				g_stuClipExpolrerCallBack.TP_TLData_Open(TP_PROGRAM_TIMELINE,NULL,pItemData->guidDBType,&aOpenGuid);	
//				CTPBaseFrame::m_uBaseFlag &=~TP_BASESTATE_NOACTPREWND ;
//				CWnd *pWnd = TP_GetEditGuiWnd(_T("TimeLineWnd"));
//				if(pWnd) pWnd->PostMessage(WM_ACTIVATE,WA_ACTIVE ,0);
//			}
//			m_iMenuItem = -1;
//		}
//		return;
//	case ID_CLIPLIST_ICONPLAY:
//		if(m_pResExplorer->m_pClipIconPlay)
//		{
//			m_pResExplorer->m_pClipIconPlay ->DestroyWindow();
//			m_pResExplorer->m_pClipIconPlay = NULL;
//			m_pResExplorer->m_pToolBar->m_uBaseState &= ~TP_BASESTATE_APPLY;
//		}
//		return;
//	case ID_RESLISTSELECT_GROUPCLIP:
//		{
//			CArray<_tagGroupItem2,_tagGroupItem2&> aGroup;
//			POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while(NULL != pos)
//			{
//				int nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
//				if(nItem < 0) continue;
//				TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);
//				if(NULL == pItemData) continue;
//				if(pItemData->eResType & TP_RES_NOTE)
//				{
//					_tagGroupItem2 stuGroupItem;
//					stuGroupItem.guidRes = pItemData->guidRes;
//					stuGroupItem.guidDBType = pItemData->guidDBType;
//					aGroup.Add(stuGroupItem);
//				}
//			}
//			if(aGroup.GetSize() > 0)
//			{
//				CTPNSCreateGroupClip dlgDo;
//				dlgDo.SetInfo(m_hCatalog,m_guidDBType, aGroup, CTPNSCreateGroupClip::modeCreate);
//				dlgDo.DoModal();
//			}
//		}
//		return ;
//	case ID_RESLISTSELECT_PICTUREICONPLAY:
//		{
//			//if(TP_IsSystemLoggingOperator()) return;
//			int iIndex = m_iMenuItem;
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iIndex);	
//			if(NULL == pItemData || TP_NODE_RECYCLE & pItemData->eNodeType) return;
//			TP_ASPECTRATIO eClipAspectRatio = TP_AspectRatio_4_3;
//			if(pItemData && pItemData->hResData)
//			{	
//				//Determine the res's corresponding project is SD or HD
//				if(pItemData ->eResType & TP_RES_NOTE)
//				{
//					TPClipData stuClipData;
//					g_pClipManageInterface ->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData,TP_GRADE_BASE|TP_GRADE_SOURCE|TP_GRADE_STATEINFO,stuClipData);
//					// snp4100030039 lihuili 如果Logging tool正在采集在素材不让做IconPlay（长度可能会不正确）
//					if((stuClipData.eSignalSourceType & TP_CLIP_SOURCE_TYPE_NOEDI) && (stuClipData.dwCaptureFlag & TP_CLIPCAPTURE_UPLOADING) && !(stuClipData.dwCaptureFlag & TP_CLIPCAPTURE_EDI))
//						return;
//
//					if(!TP_GetbIsCompatible(stuClipData.eVideoStandard,g_pSystemInfo->tpVideoStandard)) return ;
//
//					TP_VIDEO_STANDARD eVideoStandard =  stuClipData.eVideoStandard;
//					if(stuClipData.uClipFlag & TP_CLIPFLAG_1609)
//						eVideoStandard = (TP_VIDEO_STANDARD)TP_GetVideoStandard1609(eVideoStandard,TRUE);
//					eClipAspectRatio = TP_GetAspectRatioFromVideoStandard(eVideoStandard);
//					TPManageData* pManageData = (TPManageData*)TP_GetCurManageData();
//					if(TP_AspectRatio_4_3 == eClipAspectRatio && NULL != pManageData && NULL != pManageData ->hEditData)//snp4100029619 LI HUI LI 如果素材或当前时间线有一个是16:9就建16:9的窗口
//					{
//						TPTLFormat  stuTLFormat;
//						g_pTimeLineDataEdit ->TP_TLData_GetTLFormat(pManageData ->hEditData,stuTLFormat);
//						eClipAspectRatio = TP_GetAspectRatioFromVideoStandard(stuTLFormat.eVideoStandard);
//					}
//				}
//				else if(pItemData ->eResType & TP_RES_ALLPROGRAM)
//				{
//					TPProgramData stuProData;
//					g_pClipManageInterface ->stuProgramInterface.TP_GetProgramInfo(pItemData->hResData,TP_GRADE_BASE,stuProData);
//					if(!TP_GetbIsCompatible(stuProData.eVideoStandard,g_pSystemInfo->tpVideoStandard)) return ;
//
//					eClipAspectRatio = TP_GetAspectRatioFromVideoStandard(stuProData.eVideoStandard);
//				}
//				else if(pItemData ->eResType & TP_RES_CGTEMPLATE)
//				{
//					TPTemplateData stuProData;
//					g_pClipManageInterface ->stuTemplateInterface.TP_GetTemplateInfo(pItemData->hResData,TP_GRADE_BASE,stuProData);
//					if(!TP_GetbIsCompatible(stuProData.eVideoStandard,g_pSystemInfo->tpVideoStandard)) return ;
//					eClipAspectRatio = TP_GetAspectRatioFromVideoStandard(stuProData.eVideoStandard);
//				}	
//				else
//				{
//					return;
//				}
//
//				//Show the icon play window
//				if(m_pResExplorer->m_pClipIconPlay)
//				{				
//					m_pResExplorer->m_pClipIconPlay ->DestroyWindow();
//					m_pResExplorer->m_pClipIconPlay = NULL;
//				}
//
//				if(m_pResExplorer->m_pClipIconPlay == NULL)
//				{
//					g_stuClipExpolrerCallBack.TP_SVCallBack(TP_NET_SAVESVFORRENDER,(LPARAM)&pItemData->guidRes);
//
//					m_pResExplorer->m_pClipIconPlay = new CTPNSClipIconPlay();
//					m_pResExplorer->m_pClipIconPlay ->m_pResExplorer = m_pResExplorer;
//
//					//Calculate the position
//					CSize szSize(0, 0);
//					CRect rtItem = CRect(0,0,0,0);  CRect rtShow = CRect(0,0,400,300);
//					GetItemRect(iIndex, &rtItem,LVIR_BOUNDS);	
//					ClientToScreen(&rtItem);
//					//HD,SD has different window size
//					if(TP_AspectRatio_4_3 == eClipAspectRatio)   //SD 
//						szSize = CSize(300, 240);
//					else  //HD
//						szSize = CSize(432, 243);
//
//					m_pResExplorer->m_pClipIconPlay->m_eAspectRatio = eClipAspectRatio;
//					//The topLeft position change via the view type
//					if(m_iViewType == VIEWTYPE_ICON)
//					{
//						rtShow.top    = rtItem.top  + rtItem.Height()/2  - szSize.cy/2;
//						rtShow.left   = rtItem.left + rtItem.Width()/2   - szSize.cx/2;
//						rtShow.bottom = rtShow.top  + szSize.cy;
//						rtShow.right  = rtShow.left + szSize.cx;
//					}
//					else
//					{
//						rtShow.top    = rtItem.bottom;
//						rtShow.left   = rtItem.left;
//						rtShow.bottom = rtShow.top  + szSize.cy;
//						rtShow.right  = rtShow.left + szSize.cx;
//					}
//					//Create the icon play window
//					m_pResExplorer->m_pClipIconPlay->m_pResListCtrl = this;
//					m_pResExplorer->m_pToolBar->m_uBaseState |= TP_BASESTATE_APPLY;
//					BOOL bRet = m_pResExplorer->m_pClipIconPlay ->Create(NULL,NULL,WS_POPUP ,rtShow,this,NULL);
//					if(!bRet)
//					{
//						m_pResExplorer->m_pClipIconPlay = NULL;
//						return;
//					}
//				}
//
//				LRESULT lResult = m_pResExplorer->m_pClipIconPlay->SetData(pItemData->eResType,pItemData->hResData);
//				if(lResult == S_OK)
//				{				
//					m_pResExplorer->m_pClipIconPlay->ShowWindow(SW_SHOW);
//					m_pResExplorer->m_pClipIconPlay->BringWindowToTop();
//				}
//				else 
//				{
//					m_pResExplorer->m_pClipIconPlay ->DestroyWindow();
//					m_pResExplorer->m_pClipIconPlay = NULL;
//				}				
//			}
//		}
//		return;
//	case ID_RESLISTSELECT_CLEAREDI:
//		{		
//			if(MessageBox(TP_LoadStr(_T("IDS_CLIPEXPLORER_CLEAREDIASK")),_T(""),MB_YESNO|MB_ICONASTERISK) == IDNO) 
//				return;
//
//
//			POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			if(NULL != pos && 1 == CTPListCtrlEx::GetSelectedCount())
//			{
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//				if (pItemData && pItemData ->hResData)
//				{
//					pItemData ->dwCaptureFlag &= ~(TP_CLIPCAPTURE_UPLOADING|TP_CLIPCAPTURE_EDI|TP_CLIPCAPTURE_FAILED);
//					g_pClipManageInterface->stuClipInterfce.TP_SetClipEdiFlag(pItemData ->hResData,0,TP_CLIPCAPTURE_FAILED|TP_CLIPCAPTURE_UPLOADING|TP_CLIPCAPTURE_EDI);
//				}			
//			}
//
//			m_iMenuItem = -1;
//		}
//
//		break;
//	case ID_SYSCMD_RENAME:	
//	case ID_BASEPATH_RENAME: // LI HUI LI 2007-08-07
//	case ID_RESLISTSELECT_RENAME:
//		{	
//			BOOL bCanEdit = TRUE;
//			// lihuili 2010-11-04 只有选中一个时才允许重命名XN00029355
//			POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			if(NULL != pos && 1 == CTPListCtrlEx::GetSelectedCount())
//			{
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//#ifdef TP_NS_NET
//				TP_RES_TYPE eResType = TP_RES_UNKNOW;
//				if (pItemData)
//				{
//					eResType = pItemData->eResType;
//					if(pItemData ->eNodeType & TP_NODE_PROJECTFLODER)
//					{
//						eResType |= TP_NODE_PROJECTFLODER;
//					}
//				}
//				if (pItemData && !g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(pItemData->hResData, eResType, TP_RIGHT_EDIT)) bCanEdit = FALSE;
//#endif			
//
//				if(bCanEdit) EditLable(m_iMenuItem,0);
//			}
//
//			m_iMenuItem = -1;
//		}
//		return;
//	case ID_RESLISTSELECT_ABRIDGE:
//		{
//			//{liubo modify CD2009.10.01 2010-9-27 snp4100027947
//#ifdef TP_NS_NET
//			return;
//#endif
//			//}
//			//qushaojie 20090923 DQAdb00007128 search文件夹禁该快捷键
//			if(m_eNodeType & TP_NODE_SEARCH) return;
//			TP_StopCurDataState();
//			TPResDataArray  aSelData;   int iSel;
//			// LI HUI LI 2008-01-04 if not open project, do not allow abridge
//			HRESDATA hPrj = g_pClipManageInterface->TP_GetCurProject();
//			if(NULL == hPrj) return;
//
//			// 2009-11-24 li song ming 如果不是SysTrash,所有没打开工程的Trash中的素材不能做Abridge。
//			TPResCatalog stuCatalogInfo;
//			g_pClipManageInterface ->stuCatalogInterface.TP_GetCatalogInfo(m_hCatalog,&stuCatalogInfo);
//			if((stuCatalogInfo.guidProject != g_guidResProject.guidRes) &&  !(m_eNodeType & TP_NODE_TRASHSYS))
//				return;
//
//			// LI SONG MING 2009-2-16 DQAdb00004378 当什么素材都没选或者在选择的素材中没有高低址标志，则按CTR+Shit+B不能调出abridge窗口
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			if(posSel == NULL)  return;                
//			while(posSel)
//			{
//				iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData && (TP_RES_NOTE & pItemData->eResType))
//				{		
//					GetItemResData(pItemData);
//					TPResData  stuData;
//					stuData.eNodeType   = pItemData ->eNodeType;
//					stuData.eResType    = pItemData ->eResType;
//					stuData.guidRes     = pItemData ->guidRes;	
//					stuData.guidDBType  = pItemData ->guidDBType;	
//					stuData.hResCache   = m_hCacheData;
//					stuData.guidNode    = pItemData ->guidNode;
//					stuData.hResData    = pItemData->hResData;
//					stuData.lParam      = (LPARAM)pItemData->dwBookIndex;
//					BOOL  bAbridge = FALSE;
//					if(pItemData->eClipClass[0] != 0 && (pItemData->eSingSourceType & TP_CLIP_SOURCE_TYPE_DIGITIZEH) && !(pItemData->uResFlag & TP_CLIPFLAG_ABRIDGEH)) //如果有高画质，则bHaveHigh制为true
//						bAbridge  = TRUE;
//					else if(pItemData->eClipClass[1] != 0 && (pItemData->eSingSourceType & TP_CLIP_SOURCE_TYPE_DIGITIZEL) && !(pItemData->uResFlag & TP_CLIPFLAG_ABRIDGEL))
//						bAbridge  = TRUE;
//					if(bAbridge) 	aSelData.Add(stuData);					
//				}
//			}
//
//			CStringArray aClipName, aReason;
//			if(aSelData.GetSize() <= 0) return;
//			LRESULT lResult = g_pClipManageInterface->stuFuncitonInterface.TP_AbrideResData(aSelData, aClipName, aReason);	
//
//			int iClipSize = aClipName.GetSize();	
//			int iReasonSize = aReason.GetSize()	;
//
//			if(iClipSize > 0)
//			{
//				CTPNSClipAbridgeDlg dlgAbridge;
//				TPNSAbridgeMsgArray aAbridgeMsg;
//				if(S_OK != lResult) dlgAbridge.m_sTitleInfo = SBLoadString(_T("IDS_CLIPEXPLORER_DELETEABORT"));
//				else				dlgAbridge.m_sTitleInfo = SBLoadString(_T("IDS_CLIPEXPLORER_DELETECOMPLETE"));
//				for(int i = 0; i < iClipSize && i < iReasonSize; i++)
//				{
//					TPNSAbridgeMsg stuMsg;
//					stuMsg.strClipName = aClipName.GetAt(i);
//					stuMsg.strReason   = aReason.GetAt(i);
//					aAbridgeMsg.Add(stuMsg);
//				}
//
//				dlgAbridge.SetInfo(aAbridgeMsg);
//				dlgAbridge.DoModal();	
//			}		
//		}
//		return;
//	case ID_RESLISTSELECT_ABRIDGE_H:
//	case ID_RESLISTSELECT_ABRIDGE_L:
//		{
//			//{liubo modify CD2009.10.01 2010-9-27 snp4100027947
//#ifdef TP_NS_NET
//			return;
//#endif
//			//}
//			int nItem = -1;
//			POSITION pos = NULL;
//			TPResDataArray aResData;
//			pos = GetFirstSelectedItemPosition();
//			while(NULL != pos)
//			{
//				nItem = GetNextSelectedItem(pos);
//				TPListItemData* pItemData = NULL;
//				pItemData= (TPListItemData*)GetItemData(nItem);
//				if(NULL == pItemData ||
//					!(pItemData->eResType & TP_RES_NOTE)
//					|| NULL == pItemData->hResData) continue;
//
//				TPResData stuResData;
//				stuResData.hResData			= pItemData->hResData;
//				stuResData.hResCache		= NULL;
//				stuResData.eResType			= pItemData->eResType;
//				stuResData.dwOperateFlag	= pItemData->dwOperateFlag;
//				stuResData.guidRes			= pItemData->guidRes;		
//				stuResData.guidNode			= pItemData->guidNode;
//				stuResData.eNodeType		= pItemData->eNodeType; 
//				stuResData.guidDBType		= pItemData->guidDBType; 
//				stuResData.guidReturn		= GUID_NULL;
//				stuResData.lParam			= 0;
//
//				aResData.Add(stuResData);
//			}
//
//			if(aResData.GetSize() > 0)
//			{
//				GUID  guidDBType = aResData[0].guidDBType;
//				TP_CLIP_QUALITY_TYPE dwQuality  = TP_CLIP_QUALITY_HIGH;
//				if(ID_RESLISTSELECT_ABRIDGE_H == uID)
//					dwQuality = TP_CLIP_QUALITY_HIGH;
//				else if(ID_RESLISTSELECT_ABRIDGE_L == uID)	
//					dwQuality = TP_CLIP_QUALITY_LOW;
//				else 
//					return;
//				TP_StopCurDataState();
//				CMapStringToString aResult;
//				LRESULT lResult = g_pClipManageInterface->stuFuncitonInterface.TP_DeleteClipQualit(guidDBType, &aResData, dwQuality, aResult);
//				if(aResult.GetSize() > 0)
//				{
//					CTPNSClipAbridgeDlg dlgAbridge;
//					TPNSAbridgeMsgArray aAbridgeMsg;
//					if(S_OK != lResult) dlgAbridge.m_sTitleInfo = SBLoadString(_T("IDS_CLIPEXPLORER_DELETEABORT"));
//					else				dlgAbridge.m_sTitleInfo = SBLoadString(_T("IDS_CLIPEXPLORER_DELETECOMPLETE"));
//					POSITION pos = aResult.GetStartPosition();
//					while(pos)
//					{
//						CString sKey, sVaule;
//						aResult.GetNextAssoc(pos, sKey, sVaule);
//						TPNSAbridgeMsg stuMsg;
//						stuMsg.strClipName = sKey;
//						stuMsg.strReason   = sVaule;
//						aAbridgeMsg.Add(stuMsg);
//					}
//
//					dlgAbridge.SetInfo(aAbridgeMsg);
//					dlgAbridge.DoModal();	
//				}
//			}
//		}
//
//		break;
//	case ID_SYSCMD_DELETE:
//	case ID_RESLISTSELECT_DELETE:
//		{
//			//The project folder is not allowed deleted when it includes opened project
//			CGuidArray aGuidGrpFolder;
//			POSITION pos = GetFirstSelectedItemPosition();
//			while(NULL != pos)
//			{
//				int iSel = GetNextSelectedItem(pos);
//				TPListItemData *pItemData = (TPListItemData*)GetItemData(iSel);
//				if((NULL != pItemData) && (TP_NODE_PROJECTFLODER & pItemData->eNodeType))
//					aGuidGrpFolder.Add(pItemData->guidRes);
//			}
//			if(aGuidGrpFolder.GetSize() > 0)
//			{
//				BOOL bHasOpenedPrj = FALSE;
//				HRESDATA hCatlog = g_pClipManageInterface ->TP_GetCurProject();
//				HRESDATA hParentCat;
//				TPCatalogData stuCatData;
//				do
//				{
//					if(hCatlog == NULL)  break;
//					g_pClipManageInterface->stuCatalogInterface.TP_GetParentCatalog(hCatlog,hParentCat);
//					g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(hParentCat,&stuCatData);
//					for(INT i = 0; i < aGuidGrpFolder.GetSize(); i++)
//					{
//						if(stuCatData.guidRes == aGuidGrpFolder[i])
//						{
//							bHasOpenedPrj = TRUE;  break;
//						}
//					}
//					if(bHasOpenedPrj) break;
//					hCatlog = hParentCat;
//				}while(stuCatData.eNodeType != (TP_NODE_TABLE |TP_NODE_COMPUTER|TP_NODE_RES));
//				if(bHasOpenedPrj)
//				{
//					// li song ming 2010-11-17 应何莉要求，这里加一个提示框，提示用户当前工程组不能删除
//					CString sMsg = _T("The Selected project folder is not allowed deleted when it includes opened project");
//					AfxMessageBox(sMsg, MB_OK | MB_ICONSTOP);
//					return;
//				}
//			}	
//		}
//
//		CTPResListCtrl::OnMenuCmd(IDC_LISTDEL);
//		break;
//	case ID_BASEPATH_CUT:
//	case ID_RESLISTSELECT_CUT:
//		CTPResListCtrl::OnMenuCmd(IDC_LISTCUT);
//		if(m_pResExplorer)   m_pResExplorer->SetToolBarState();// 刷新一下ToolBar的状态
//		break;
//	case ID_BASEPATH_COPY:
//	case ID_RESLISTSELECT_COPY:
//		CTPResListCtrl::OnMenuCmd(IDC_LISTCOPY);
//		if(m_pResExplorer)   m_pResExplorer->SetToolBarState();// 刷新一下ToolBar的状态
//		break;
//	case ID_BASEPATH_PASTE:	
//		{
//			HRESDATA hCatalog = NULL;
//			if(m_iMenuItem >= 0)
//			{
//				TPListItemData* pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(m_iMenuItem);
//				if(pItemData && (pItemData->eResType & (TP_RES_PROJECT|TP_RES_CATALOG))) hCatalog = pItemData->hResData;
//			}
//			OnMenuCmd(IDC_LISTPASTE, (LPARAM)hCatalog);
//			m_iMenuItem = -1;
//		}
//		return;
//	case ID_BASEPATH_DELETE:
//		{
//			CTPResListCtrl::OnMenuCmd(IDC_CATALOG_DELETE);			
//		}
//		return;
//	case ID_RESLISTSELECT_SEARCHWITHINTHISSEQUENCR:		
//		{
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(pItemData == NULL) return;
//
//			CTPNSClipSearch *pClipSearchWnd = CTPNSClipSearch::CreateSearchWnd(pItemData ->guidDBType,this);
//			if(NULL == pClipSearchWnd) return;
//
//			GUID guidDBType = GUID_NULL;
//			CGuidArray aGuidCatalog;
//			TPResDataArray aResData;
//			if(g_pTimeLineDataEdit->TP_TLData_GetCurData() == pItemData->guidRes)// 如果是当前编辑的时间线， 先保存一下DQAdb00008829
//			{
//				if(g_stuClipExpolrerCallBack.TP_TimeLineCallBack)
//				{	
//					CGuidArray aGuidTL;
//					aGuidTL.Add(pItemData->guidRes);
//					g_stuClipExpolrerCallBack.TP_TimeLineCallBack(TP_TIMELINE_SAVETL, (LPARAM)&aGuidTL);
//				}
//			}
//			// DQAdb00005115 2009-09-17 li song ming
//			g_pClipManageInterface->TP_GetResRefer(pItemData->guidDBType,pItemData->guidRes,pItemData->eResType | TP_RES_NOTE |TP_RES_CGPROGRAM |TP_RES_CGTEMPLATE,aResData);
//
//			guidDBType = pItemData->guidDBType;
//			aGuidCatalog.Add(pItemData->guidRes);
//			pClipSearchWnd->m_pResTreeCtrl = (CTPNSResTreeCtrl*)m_pResTreeCtrl;
//			pClipSearchWnd->SetSearchData(guidDBType, TP_RES_ALLPROGRAM, aGuidCatalog);
//			pClipSearchWnd->SetFindRes(aResData);
//			pClipSearchWnd->ShowWindow(SW_SHOW);
//			if(pClipSearchWnd->IsIconic())pClipSearchWnd->SendMessage(WM_SYSCOMMAND, SC_RESTORE);
//			pClipSearchWnd->BringWindowToTop();
//		}		
//		break;	
//	case ID_RESLISTSELECT_TEAMTRANS:
//		{
//
//			int      nItem = 0;
//			POSITION pos   = NULL;
//			TPResDataArray aResData;
//			pos = GetFirstSelectedItemPosition();
//			while(NULL != pos)
//			{
//				nItem = GetNextSelectedItem(pos);
//				TPListItemData* pItemData = NULL;
//				pItemData= (TPListItemData*)GetItemData(nItem);
//				if(NULL == pItemData) continue;
//
//				TPResData stuResData;
//				stuResData.hResData			= pItemData->hResData;
//				stuResData.hResCache		= NULL;
//				stuResData.eResType			= pItemData->eResType;
//				stuResData.dwOperateFlag	= pItemData->dwOperateFlag;
//				stuResData.guidRes			= pItemData->guidRes;		
//				stuResData.guidNode			= pItemData->guidNode;
//				stuResData.eNodeType		= pItemData->eNodeType; 
//				stuResData.guidDBType		= pItemData->guidDBType; 
//				stuResData.guidReturn		= GUID_NULL;
//				stuResData.lParam			= 0;
//
//				aResData.Add(stuResData);
//			}
//			if(aResData.GetSize() > 0)
//			{
//				CTPNSClipTeamTrans stuTeans;
//				const TPResData& stuResData = aResData[0];
//				stuTeans.m_guidDBType  = stuResData.guidDBType;
//				stuTeans.m_aTransRes.Copy(aResData);
//				if(stuTeans.DoModal()==IDOK)
//				{
//					TP_SaveCurrentTL(TRUE);
//					TPResDataArray aResDataTL;//需要刷新的时间线
//					g_pClipManageInterface ->stuFuncitonInterface.TP_TeamTrans(stuTeans.m_hSelCatalog,g_pClipManageSysSet->dwTeamTransFlag,aResData,aResDataTL);
//				}
//			}
//		}
//		break;
//	case ID_RESLISTSELECT_EXTRAPROPERTIES:
//		{
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(pItemData == NULL) return;
//			if(pItemData->eResType & TP_RES_NOTE)
//			{
//				CTPNSClipProperties *pClipProperties = TP_CreateClipProperties(NULL);
//				if(pClipProperties) pClipProperties->Refresh(pItemData->guidRes,pItemData->guidDBType);
//			}
//			else if(pItemData->eResType & TP_RES_ALLPROGRAM)
//			{
//				CTPNSTLProperties *pTLProperties = TP_CreateTLProperties(NULL);
//				if(NULL != pTLProperties)
//					pTLProperties ->Refresh(pItemData->guidRes,pItemData->guidDBType);
//			}			
//		}
//		break;
//	case ID_RESLISTSELECT_VOICEOVER:
//		{
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(pItemData == NULL)              return;
//			if(pItemData ->hResData == NULL)   return;
//			if(!(pItemData->eResType & TP_RES_NOTE) || !(pItemData->eSingSourceType & TP_CLIP_SOURCE_TYPE_DUB))   return;
//
//			TPClipData stuClipData;
//			g_pClipManageInterface->stuClipInterfce.TP_GetNoteInfo(pItemData ->hResData,TP_GRADE_BASE|TP_GRADE_FILE,stuClipData);
//
//			CTPNSClipDubProperties dlgDubProperty;
//			dlgDubProperty.m_pClipData  = &stuClipData;
//			dlgDubProperty.m_hResData   = pItemData ->hResData;
//			dlgDubProperty.DoModal();
//		}
//		break;
//	case ID_EXTRAPROPERTIES_VOICEOVERTAKE1:
//	case ID_EXTRAPROPERTIES_VOICEOVERTAKE2:
//	case ID_EXTRAPROPERTIES_VOICEOVERTAKE3:
//	case ID_EXTRAPROPERTIES_VOICEOVERTAKE4:
//		{
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(pItemData == NULL) return;
//			if(!(pItemData->eResType & TP_RES_NOTE) || !(pItemData->eSingSourceType & TP_CLIP_SOURCE_TYPE_DUB))   return;
//
//			//Change the clip data
//			INT aIntTake[4] = {0,0,0,0};
//			if(uID == ID_EXTRAPROPERTIES_VOICEOVERTAKE1)         for(INT i = 0; i < 4; i++) aIntTake[i] = 0;
//			else if(uID == ID_EXTRAPROPERTIES_VOICEOVERTAKE2)    for(INT i = 0; i < 4; i++) aIntTake[i] = 1;
//			else if(uID == ID_EXTRAPROPERTIES_VOICEOVERTAKE3)    for(INT i = 0; i < 4; i++) aIntTake[i] = 2;
//			else if(uID == ID_EXTRAPROPERTIES_VOICEOVERTAKE4)    for(INT i = 0; i < 4; i++) aIntTake[i] = 3;
//
//			// Must Stop play state before Select Take
//			TP_StopCurDataState();
//			g_pClipManageInterface ->stuClipInterfce.TP_SelectTakeForVoice( pItemData ->hResData, aIntTake);
//
//			struct _temp
//			{
//				GUID guidRes;
//				GUID guidDBType;
//				INT  *pTake;
//			}temp;
//
//			temp.guidDBType = pItemData->guidDBType;
//			temp.guidRes    = pItemData->guidRes;
//			temp.pTake		= aIntTake;
//			g_stuClipExpolrerCallBack.TP_CaptureCallBack(TP_TIMELINEWND_TAKECHANGE,(LPARAM)&temp);
//
//			//Refresh the TL snd SV
//
//			TPRefreshRes stuRefresh(pItemData ->guidDBType ,pItemData->guidRes,FALSE);
//			stuRefresh.eType = TP_RefreshRes_VOTake;	 
//			g_pClipManageInterface ->stuClipInterfce. TP_RefreshClipFile(stuRefresh);  
// 
//		}
//		break;
//	case ID_RESTRASHLIST_EMPTYTRASH:
//		{
//			if(m_eNodeType & TP_NODE_TRASH)
//			{
//				((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->m_hMenuItem = m_pResTreeCtrl ->GetSelectedItem();
//				((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->OnCmdMsg(ID_SYSCMD_EMPTYTRASH);	
//			}
//			else
//			{
//				int nItem = m_iMenuItem;
//				if(nItem < 0)
//				{
//					POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//					if(NULL == pos) return;
//					nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
//				}
//				if(nItem < 0 || nItem >= CTPListCtrlEx::GetItemCount()) return;
//
//				TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);
//				if(NULL == pItemData) return;
//				HTREEITEM hTreeItem= m_pResTreeCtrl->FindCatalog(pItemData->guidDBType,pItemData->guidRes, NULL);
//				if(hTreeItem < 0) return;
//				((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->m_hMenuItem = hTreeItem;
//				((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->OnCmdMsg(ID_SYSCMD_EMPTYTRASH);
//			}
//		}
//		break;
//	case ID_RESTRASHLIST_ABRIDGE:
//
//		break;
//	case ID_RESTRASHLIST_SHOWALL:
//
//		break;
//	case ID_RESTRASHLISTSELECT_RETURNTOORIGINALLOCATION:		
//		CTPResListCtrl::OnMenuCmd(IDC_RESTORERECYCLE);
//		break;
//	case ID_RESTRASHLISTSELECT_DELETE:
//		CTPResListCtrl::OnMenuCmd(IDC_DELRECYCLE);		
//		if(m_pResExplorer)
//		{
//			m_pResExplorer  ->SetToolBarState();
//			if(m_pResExplorer ->m_pStatusBar){	if(!m_hSelTimer) m_hSelTimer = SetTimer(TP_EVENT_STATUSBAR,500,NULL);}
//		}
//		break;
//	case ID_BATCHDIGITIZE_ALLCLIPS:
//		{
//			if(g_pSystemInfo->uSystemState & TP_SYSTEMSTATE_LOGING) 
//			{
//				AfxMessageBox(TP_LoadStr(_T("IDS_TIMELINEWND_LOGINING")));	
//				return ;
//			}	
//			if(TP_IsProxyEditVersion())   return;
//			g_pSystemInfo->lParam = (LPARAM)(ID_BATCHDIGITIZE_ALLCLIPS);
//			if(g_pClipManageInterface->TP_GetCurProject() == NULL)       return;
//
//			if(g_stuClipExpolrerCallBack.TP_CaptureCallBack)
//				g_stuClipExpolrerCallBack.TP_CaptureCallBack(TP_BATCH_BACKTOCREATOR,0);
//
//			TP_CreateGui(_T("Batch Digitize"));		
//			g_pSystemInfo->lParam = 0;
//		}
//		break;
//	case ID_BATCHDIGITIZE_SELECTEDCLIPS:
//		{
//			if(g_pSystemInfo->uSystemState & TP_SYSTEMSTATE_LOGING) 
//			{
//				AfxMessageBox(TP_LoadStr(_T("IDS_TIMELINEWND_LOGINING")));	
//				return ;
//			}	
//			if(TP_IsProxyEditVersion())   return;
//			g_pSystemInfo->lParam = (LPARAM)(ID_BATCHDIGITIZE_SELECTEDCLIPS);
//			if(g_pClipManageInterface->TP_GetCurProject() == NULL)       return;		
//
//			if(g_stuClipExpolrerCallBack.TP_CaptureCallBack)
//				g_stuClipExpolrerCallBack.TP_CaptureCallBack(TP_BATCH_BACKTOCREATOR,0);
//
//			TP_CreateGui(_T("Batch Digitize"));
//
//			g_pSystemInfo->lParam = 0;
//		}
//		break;
//	case ID_BATCHDIGITIZE_NON:
//		{
//			if(g_pSystemInfo->uSystemState & TP_SYSTEMSTATE_LOGING) 
//			{
//				AfxMessageBox(TP_LoadStr(_T("IDS_TIMELINEWND_LOGINING")));	
//				return ;
//			}	
//			if(TP_IsProxyEditVersion())   return;
//			g_pSystemInfo->lParam = (LPARAM)(ID_BATCHDIGITIZE_NON);
//			if(g_pClipManageInterface->TP_GetCurProject() == NULL)       return;	
//
//			if(g_stuClipExpolrerCallBack.TP_CaptureCallBack)
//				g_stuClipExpolrerCallBack.TP_CaptureCallBack(TP_BATCH_BACKTOCREATOR,0);
//
//			TP_CreateGui(_T("Batch Digitize"));
//			g_pSystemInfo->lParam = 0;
//		}
//		break;
//	case ID_BATCHDIGITIZE_OKCLIPS:
//		{	
//			if(g_pSystemInfo->uSystemState & TP_SYSTEMSTATE_LOGING) 
//			{
//				AfxMessageBox(TP_LoadStr(_T("IDS_TIMELINEWND_LOGINING")));	
//				return ;
//			}	
//			if(TP_IsProxyEditVersion())   return;
//			g_pSystemInfo->lParam = (LPARAM)(ID_BATCHDIGITIZE_OKCLIPS);
//			if(g_pClipManageInterface->TP_GetCurProject() == NULL)       return;	
//
//			if(g_stuClipExpolrerCallBack.TP_CaptureCallBack)
//				g_stuClipExpolrerCallBack.TP_CaptureCallBack(TP_BATCH_BACKTOCREATOR,0);
//
//			TP_CreateGui(_T("Batch Digitize"));			
//			g_pSystemInfo->lParam = 0;
//		}
//		break;		
//	case ID_USERINFO_ACTIVE:		
//		{	
//			if(g_pSystemInfo->uSystemState & TP_SYSTEMSTATE_LOGING) 
//			{
//				AfxMessageBox(TP_LoadStr(_T("IDS_TIMELINEWND_LOGINING")));	
//				return ;
//			}	
//			int nItem = -1;
//			POSITION pos = GetFirstSelectedItemPosition();
//			if(NULL == pos) return;
//			nItem = GetNextSelectedItem(pos);
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(nItem);
//			if(pItemData && (pItemData ->eResType & TP_RES_USER) && (g_pUserEnterInfo ->guidUserConfigration != pItemData ->guidRes))
//			{	
//				if(g_stuClipExpolrerCallBack.TP_ActivateUser)
//				{
//					GUID guid1,guid2;BOOL bLogin;
//					g_pUserMangeInterface->TP_NS_GetDefUser(g_pUserEnterInfo->guidComputerMac,guid1,guid2 ,bLogin);
//					if(S_OK == g_stuClipExpolrerCallBack.TP_ActivateUser(pItemData ->guidRes))
//						g_pUserMangeInterface->TP_NS_SetDefUser(g_pUserEnterInfo->guidComputerMac, guid1,pItemData->guidRes, bLogin);
//
//				}
//			}
//
//		}
//		break;
//	case ID_USERINFO_PASTE:
//		g_pUserMangeInterface ->TP_NS_PasteUserInfo(FALSE);
//		((CTPNSResTreeCtrl *) m_pResTreeCtrl) ->SelectItemChange(m_pResTreeCtrl->GetSelectedItem());
//		break;
//	case ID_USERINFO_COPY:		
//		{
//			POSITION posSel   = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			int      iCount   = CTPListCtrlEx::GetSelectedCount(),iSelect = 0;
//			if(posSel == NULL) break;
//			TPUserDepartmentState stuState(g_pUserEnterInfo,TP_TPUSERD_NONETMSG);
//			while (posSel)
//			{
//				iSelect ++;
//				if(iCount == iSelect) stuState.Reset();
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData && (pItemData->eResType & TP_RES_USER))
//				{
//					GUID guidUser = pItemData->guidRes;
//					//Save the user info befor copy it, otherwise you will lost some important info,such as lay out.
//					g_stuClipExpolrerCallBack.TP_SaveUserInfo(guidUser);
//					TPUserDepartmentBaseInfo stuUserInfo;
//					if(g_pUserMangeInterface->TP_NS_GetUserInfo(guidUser, stuUserInfo) == S_OK)
//					{
//						GUID    guidSame   = GUID_NULL;
//						CString sCheckName = stuUserInfo.sUserConfigrationName;
//						stuUserInfo.sUserConfigrationName = g_pUserMangeInterface->TP_NS_CheckUserName((LPTSTR)(LPCTSTR)sCheckName, guidSame);
//						g_pUserMangeInterface->TP_NS_SaveAsUserInfo(guidUser, stuUserInfo);
//					}
//				}
//			}
//
//			if(m_pResExplorer && m_pResExplorer ->m_pSearchBar)	
//				m_pResExplorer ->m_pSearchBar->ClearSearchText();
//
//
//			HTREEITEM hItem = m_pResTreeCtrl->GetSelectedItem();
//			if(hItem != (HTREEITEM)-1)m_pResTreeCtrl->SelectItemChange(hItem); 
//		}
//		break;
//	case ID_USERINFO_DELTET:
//		{	
//			if(this->GetSelectedCount() <= 0) return;
//			if(IDYES != MessageBox(SBLoadString(_T("IDS_CLIPEXPLORER_USERCONFDEL")), _T(""), MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2))    return;
//        	int      iCount  = CTPListCtrlEx::GetSelectedCount(),iSelect = 0;
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			TPUserDepartmentState stuState(g_pUserEnterInfo,TP_TPUSERD_NONETMSG);
//			while (posSel)
//			{				
//				iSelect ++;
//				if(iCount == iSelect) stuState.Reset();
//
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData && (pItemData ->eResType & TP_RES_USER))
//				{
//					if(pItemData ->guidRes == g_pUserEnterInfo->guidUserConfigration)
//					{
//						AfxMessageBox(SBLoadString(_T("IDS_CLIPEXPLORER_DELUSERCONFAIL")));
//						continue;
//					}
//					else 
//					{
//						g_pUserMangeInterface->TP_NS_DelUserInfo(pItemData ->guidRes);
//						DeleteItem(iSel);
//					}
//				}
//			}		 
//			if(m_pResExplorer)   m_pResExplorer->SetToolBarState();//刷新一下ToolBar
//		}
//		break;
//	case ID_USERPATH_CREATENEW:
//		m_pResTreeCtrl ->m_hMenuItem = m_pResTreeCtrl->GetSelectedItem();
//		((CTPNSResTreeCtrl *)m_pResTreeCtrl) ->OnCmdMsg(ID_USERPATH_CREATENEW);
//		m_pResTreeCtrl ->m_hMenuItem = NULL;
//		return;
//	case ID_USERINFO_RENAME:
//		{		
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(pItemData && (pItemData ->eResType & TP_RES_USER))
//			{
//				pItemData->dwOperateFlag |= TP_OPERATEFLAG_EDITINFO;
//				CTPListCtrlEx::EnsureVisible(m_iMenuItem,TRUE);
//				CTPListCtrlEx::EditLable(m_iMenuItem,0);						
//			}
//			m_iMenuItem = -1;
//		}
//		break;
//	case ID_USERINFO_PROPERTIES:			
//		{		
//			TPListItemData    *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			TPPluginGuiData   *pGuidata  = TP_GetPluginGuiData(_T("UserExplorer"));
//			if(pGuidata && pItemData)
//			{
//				if(pGuidata ->funPluginGui.TP_GdiCallBack(TP_USER_EDIT,(LPARAM)&pItemData ->guidRes) == S_OK)											
//				{
//					pItemData ->bGetData = FALSE;
//					GetItemResData(pItemData);
//					TPUserDepartmentBaseInfo stuUseInfo;
//					g_pUserMangeInterface->TP_NS_GetUserInfo(pItemData->guidRes,stuUseInfo); 
//					if(g_pUserEnterInfo->guidUserConfigration == pItemData->guidRes)
//						TP_RefreshUserName(&stuUseInfo);	
//				}
//			}
//		}
//		break;
//	case ID_BATCHDIGITIZE_MXFBATCHTOOL:
//
//		return;
//	case ID_RESTRASHLISTSELECT_ABRIDGE:
//		this->OnCmdMsg(ID_RESLISTSELECT_ABRIDGE); 
//		break;			
//	case ID_SYSCMD_OPENPROJECT:
//		{		
//			if(m_eNodeType & TP_NODE_TRASH) return;
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//			if(pItemData && (pItemData->eResType & TP_RES_PROJECT) && pItemData ->guidRes != g_guidResProject.guidRes)
//			{
//				if(!TP_IsActTeamDB(pItemData ->guidDBType)) return;
//				m_pResTreeCtrl ->m_hMenuItem = m_pResTreeCtrl ->FindCatalog(pItemData->guidDBType,pItemData->guidRes,NULL);
//				if(m_pResTreeCtrl ->m_hMenuItem != (HTREEITEM)-1) ((CTPNSResTreeCtrl *)m_pResTreeCtrl)->OnCmdMsg(ID_SYSCMD_OPENPROJECT);
//			}
//		}
//		break;		
//	case IDC_CLIP_DIRTOTRACK:		
//	case IDC_CLIP_THREETRACK:		
//	case IDC_CLIP_FOURTRACK:
//	case IDC_CLIP_BETWENMARK:
//	case IDC_CLIP_FILLINOUT:
//		{
//			// 如果正在Trim则不允许上线
//			if(S_OK == g_stuClipExpolrerCallBack.TP_TimeLineCallBack(TP_TIMELINE_GETEDITRESSTATE,NULL)) return;
//			TPManageData *pManageData = (TPManageData *)TP_GetCurManageData();
//			if(pManageData == NULL || pManageData->hEditData == NULL) return ;
//			CTPResListCtrl::OnMenuCmd(uID);
//		}
//		return;
//	case IDC_SYSCMD_RETURN:
//		{
//			if(GetSelectedCount() <=0) return;
//			POSITION pos = GetFirstSelectedItemPosition();
//			if(pos == NULL) return;
//			int iDbClickItem = GetNextSelectedItem(pos);
//			if(iDbClickItem <0) return;
//			TPListItemData *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(iDbClickItem);
//			if(pItemData == NULL) return;			
//			CTPResListCtrl::EndEdit();					
//			if(pItemData && pItemData ->eResType == TP_RES_USER && g_pUserEnterInfo ->guidUserConfigration != pItemData ->guidRes)
//			{
//				OnCmdMsgClip(ID_USERINFO_ACTIVE);			
//				return;
//			}		
//			else if(pItemData && (pItemData ->eNodeType & TP_NODE_SEARCH))
//			{
//				TP_RES_TYPE eRes = pItemData->eResType;
//				HTREEITEM hTree  = m_pResTreeCtrl ->FindCatalog(pItemData->guidDBType,pItemData->guidNode,NULL);
//				if(hTree)
//				{
//					m_iMenuItem = -1;
//					GUID guidItem = pItemData->guidRes;
//					m_pResTreeCtrl ->SelectItem(hTree);
//					m_pResTreeCtrl ->SelectItemChange(hTree);
//					for(INT l=0;l<m_aItem.GetSize();l++)
//					{
//						if(((TPListItemData *)m_aItem[l]->dwData) ->guidRes == guidItem)
//						{
//							SelectItem(l,TRUE,TRUE);
//							m_iMenuItem = l;
//							break;
//						}
//					}
//					if(m_iMenuItem>=0)
//					{
//						if(eRes == TP_RES_PROJECT)                        OnCmdMsgClip(ID_SYSCMD_OPENPROJECT);
//						else if(eRes & (TP_RES_NOTE | TP_RES_ALLPROGRAM)) OnCmdMsgClip(ID_RESLISTSELECT_OPEN);					
//					}
//				}
//			}
//			else if(pItemData && (pItemData ->eResType & TP_RES_CATALOG))
//			{			
//				ExpandItem(iDbClickItem);
//			}
//			else if(pItemData && (pItemData ->eResType & (TP_RES_NOTE | TP_RES_ALLPROGRAM|TP_RES_CGTEMPLATE)))//XN00027868 qushaojie20100201
//			{
//				m_iMenuItem = iDbClickItem;
//				OnCmdMsgClip(ID_RESLISTSELECT_OPEN);
//			}
//			else if(pItemData && (pItemData ->eResType & TP_RES_PROJECT))
//			{
//				m_iMenuItem = iDbClickItem;
//				OnCmdMsgClip(ID_SYSCMD_OPENPROJECT);
//			}
//			m_iMenuItem = -1;
//		}
//		return;
//	case IDC_SEPARATE + 1014: 
//		{
//			this->SendMessage(WM_KEYDOWN, VK_LEFT, 0);
//		}
//		break;
//	case IDC_SEPARATE + 1015: 
//		{
//			this->SendMessage(WM_KEYDOWN, VK_RIGHT, 0);
//		}
//		break;
//	case ID_SYSCMD_OPENCONTAINCATALOG:
//		{
//			if(CTPListCtrlEx::GetSelectedCount() > 1) return;
//			POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			if(NULL == pos) return;
//			int nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
//			if(nItem < 0) return;
//
//			TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);
//			if(NULL == pItemData ||  NULL == m_pResTreeCtrl) return;
//
//			GUID guidCatalog = GUID_NULL;
//			GUID guidRes     = pItemData->guidRes;
//			if(TP_RES_NOTE & pItemData->eResType)
//			{
//				TPClipData stuClipData;
//				g_pClipManageInterface->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData, TP_GRADE_BASE, stuClipData);
//				guidCatalog = stuClipData.guidNode;
//			}
//			else if(TP_RES_ALLPROGRAM & pItemData->eResType)
//			{
//				TPProgramData stuProgramData;
//				g_pClipManageInterface->stuProgramInterface.TP_GetProgramInfo(pItemData->hResData, TP_GRADE_BASE, stuProgramData);
//				guidCatalog = stuProgramData.guidNode;
//
//			}
//			else if((TP_RES_CATALOG | TP_RES_PROJECT) & pItemData->eResType)
//			{
//				TPCatalogData stuCatalogData;
//				g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(pItemData->hResData, &stuCatalogData);
//				guidCatalog = stuCatalogData.guidNode;
//			}
//			else return;
//			//{XpriNS_Chengdu:2007.12.20:liguozheng:没搜到则从数据库中查找，然后展开
//
//			CTPNSResTreeCtrl *pTreeCtrl = (CTPNSResTreeCtrl*)m_pResTreeCtrl;
//#ifdef TP_NS_NET
//			HTREEITEM hItem = pTreeCtrl->FindCatalogNet(pItemData->guidDBType,guidCatalog, NULL);
//#else
//			HTREEITEM hItem = pTreeCtrl->FindCatalog(pItemData->guidDBType,guidCatalog, NULL);
//#endif
//			if((HTREEITEM)-1 != hItem)
//			{
//				//{liubo add CD2009.10.01 2010-9-20 snp4100027457
//#ifdef TP_NS_NET
//				m_aGuidSel.RemoveAll();
//				m_aGuidSel.Add(guidRes);
//#endif
//				//}
//
//				pTreeCtrl->SelectItem(hItem);
//				pTreeCtrl->SelectItemChange(hItem);
//
//				CGuidArray aGuidItem;
//				aGuidItem.Add(guidRes);
//				this->SelectItemGuid(aGuidItem);
//			}			
//			return;
//		}
//		break;
//	case IDM_XDCAM_IMPORTLOW:
//	case IDM_XDCAM_IMPORTHIGH:
//	case IDM_XDCAM_IMPORTALL:
//		{
//			TPClipData           stuClipData;
//			int                  iSel = -1;
//			TPBrdClipImportArray aClipImp;			
//			TPBrdClipImportArray aEdlImp;		
//			TPBrdClipImportArray aPgmImp;			
//
//			g_stuBrdManage.TP_InitCache();
//			TPBrdSysSetData *pBrdSysData = NULL;			
//			g_stuBrdManage.TP_GetBrdSysSetData(pBrdSysData);
//
//			DWORD dwHighQuality = 0, dwLowQaulity = 0;
//			if(IDM_XDCAM_IMPORTALL == uID)
//			{
//				dwHighQuality = TP_CLIP_CLASS_V;
//				dwLowQaulity  = TP_CLIP_CLASS_V;
//			}
//			else if(IDM_XDCAM_IMPORTHIGH == uID)
//			{
//				dwHighQuality = TP_CLIP_CLASS_V;
//				dwLowQaulity  = 0;
//			}
//			else if(IDM_XDCAM_IMPORTLOW == uID)
//			{
//				dwHighQuality = 0;
//				dwLowQaulity  = TP_CLIP_CLASS_V;
//			}
//			else return;			
//
//			TPBrdClipImport    stuImport;	
//			stuImport.bImpClip         = TRUE;
//			if(pBrdSysData)
//			{
//				stuImport.guidResClipPath  = pBrdSysData->guidResClipPath;
//				stuImport.guidResPgmPath   = pBrdSysData->guidResPgmPath;
//				stuImport.guidResCGPgmPath = pBrdSysData->guidResCGPgmPath;
//				stuImport.sVideoPath       = pBrdSysData->sVideoPath;
//				stuImport.sAudioPath       = pBrdSysData->sAudioPath;	
//			}							
//			stuImport.aQuality[0].bImp       = dwHighQuality > 0 ? TRUE : FALSE;
//			stuImport.aQuality[0].eClipClass = dwHighQuality;	
//			stuImport.aQuality[1].bImp       = dwLowQaulity > 0 ? TRUE : FALSE;
//			stuImport.aQuality[1].eClipClass = dwLowQaulity;
//			stuImport.iAudioChannle          = 8;
//
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData && (pItemData ->eResType & TP_RES_NOTE) && (pItemData->eSingSourceType & (TP_CLIP_SOURCE_TYPE_BRD|TP_CLIP_SOURCE_TYPE_P2|TP_CLIP_SOURCE_TYPE_XDCAMEX|TP_CLIP_SOURCE_TYPE_SYSUDF|TP_CLIP_SOURCE_TYPE_BRDNEWCLIP)))
//				{						
//					g_pClipManageInterface ->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData,TP_GRADE_BASE|TP_GRADE_SOURCE|TP_GRADE_RESSOURCE|TP_GRADE_FILEBASE,stuClipData);
//
//					//{liubo add CD2009.10.01 2010-2-4 
//#ifdef TP_NS_NET
//					if (stuClipData.guidDevice == GUID_NULL || (stuClipData.guidResSource == GUID_NULL && !(stuClipData.uClipFlag & TP_CLIPFLAG_GROUPCLIP)))
//						continue;
//#endif
//					//}
//
//					if(stuClipData.eSignalSourceType & TP_CLIP_SOURCE_TYPE_BRD)          stuImport.uImpType = TP_IMPTYPE_NORMAL;
//					else if(stuClipData.eSignalSourceType & TP_CLIP_SOURCE_TYPE_P2)      
//					{
//						GUID guidTop;CString sName;long lLength;
//						g_stuP2Manage.TP_GetNoteImportInfo(stuClipData.guidDevice,stuClipData.guidResSource,guidTop,sName,lLength,stuImport.aQuality[1].bImp); 
//						stuImport.uImpType = TP_IMPTYPE_P2;						
//					}
//					else if(stuClipData.eSignalSourceType & TP_CLIP_SOURCE_TYPE_XDCAMEX) 
//					{
//						stuImport.uImpType = TP_IMPTYPE_XDCAMEX;
//						if(stuClipData.uClipFlag & TP_CLIPFLAG_EXAVI)   stuImport.uImpType |= TP_IMPTYPE_XDCAMEXAVI;							
//					}
//					else if(stuClipData.eSignalSourceType & TP_CLIP_SOURCE_TYPE_SYSUDF) 
//					{
//						stuImport.uImpType = TP_IMPTYPE_SYSUDF;
// 					}
//
//					if((stuClipData.eSignalSourceType & TP_CLIP_SOURCE_TYPE_BRDNEWCLIP) && dwHighQuality == 0) continue;
//					if(stuClipData.eSignalSourceType & TP_CLIP_SOURCE_TYPE_BRDNEWCLIP) stuImport.aQuality[1].bImp = FALSE;
//
//					stuImport.eResType      = TP_BRD_CLIP;
//
//					stuImport.lLength       = stuClipData.lResLength;
//					stuImport.guidBrdRes    = stuClipData.guidResSource;
//					stuImport.guidRes		= stuClipData.guidRes;
//					stuImport.sAlias        = stuClipData.sName;    
//					stuImport.sName         = stuClipData.sResSource;
//					if(stuClipData.uClipFlag &  TP_CLIPFLAG_GROUPCLIPXDCAM)   stuImport.sName = stuClipData.sName;
//					stuImport.guidDisk      = stuClipData.guidDevice;
//					stuImport.sDiskName     = stuClipData.sSignalSourceDesc;
//					stuImport.guidDBType	   = stuClipData.guidDBType;
//					stuImport.dwVideoStandard  = g_stuBrdManage.TP_GetVideoStandRate(stuClipData.eVideoStandard); 
//					stuImport.bDropFrame       = stuClipData.bDropFrame;
//					stuImport.iOrignalStart    = stuClipData.lStart;
//					stuImport.eDBEClass        = stuClipData.aQuality[0].eDBEClass | stuClipData.aQuality[1].eDBEClass;
//
//					stuImport.aQuality[0].eClipClass = pItemData->eClipClass[0]|pItemData->eClipClass[1];						
//					stuImport.aQuality[1].eClipClass = pItemData->eClipClass[0]|pItemData->eClipClass[1];
//
//					stuImport.aQuality[0].iInPoint  = 0;
//					stuImport.aQuality[0].iOutPoint = (stuClipData.eSignalSourceType & TP_CLIP_SOURCE_TYPE_SPEEDCLIPS) ? stuClipData.dwVTROutPoint - stuClipData.dwVTRInPoint + stuClipData.dwVTRInExt + stuClipData.dwVTROutExt : stuClipData.lClipLength;
//					stuImport.aQuality[1].iInPoint  = 0;
//					stuImport.aQuality[1].iOutPoint = stuClipData.lClipLength;
//					if(stuClipData.uClipFlag &  TP_CLIPFLAG_GROUPCLIPXDCAM)   stuImport.uImpType |= TP_IMPTYPE_GROUP;
//					stuImport.dwAspectRatio = (stuClipData.uClipFlag & TP_CLIPFLAG_1609) ? 1 : 0;
//					stuImport.eVideoStandard        = stuClipData.eVideoStandard;
//
//					aClipImp.Add(stuImport);					
//				}
//			} 	
//
//			if(pBrdSysData && pBrdSysData ->bPopupUploadCenter && g_stuBrdExplorer.TP_GdiCallBack) g_stuBrdExplorer.TP_GdiCallBack(TP_BRD_UPLOAD,1);
//			g_stuBrdManage.TP_ApplyReImportRes(aClipImp,aEdlImp,aPgmImp);
//
//		}
//		break;	
//		//{XpriNS_Chengdu:Fengxiaoping add,2007.07.16
//#ifdef TP_NS_NET
//	case MENU_SENDTO_MPC:
//		if ( g_pClipManageInterface->stuSonapsInterface.TP_SendProgramToMPC )
//		{
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(m_iMenuItem);
//
//			//liubo add CD2009.10.01 2010-3-16 snp4100022501 增加空指针判断
//			if (pItemData == NULL)
//			{
//				return;
//			}
//			//}
//
//			//{2010-6-30:CD2009.10.01:chenzhi:add:register wnd;ML需要这个句柄来定位窗口
//			HWND hWnd = GetParent()->m_hWnd;
//			//}
//
//			GUID   guidNoAsk = { 0x947bb88b, 0xc79e, 0x4a1c, { 0xbe, 0xe2, 0x27, 0xd2, 0x72, 0xe6, 0x4f, 0xe7 } };
//			LPBYTE pNoAsk = NULL;
//			long   lNoAskSize = (long)sizeof(BOOL);
//			g_pUserMangeInterface->TP_GetPivateData( g_pUserEnterInfo->guidDBType, g_pUserEnterInfo->guidUserConfigration, guidNoAsk, pNoAsk, lNoAskSize );
//			BOOL bNoAsk = pNoAsk ? *((BOOL*)pNoAsk) : FALSE;
//			if ( pNoAsk )
//			{
//				delete pNoAsk;
//				pNoAsk = NULL;
//			}
//			BOOL bShiftDown = (GetKeyState( VK_SHIFT ) < 0);
//			BOOL bShowFolderDlg = (bNoAsk && !bShiftDown) ? FALSE : TRUE;
//
//			struct GETMLCATALOGPATH_PARAM {
//				CString sFolderPath;
//				BOOL    bShowNoAsk;
//			};
//			GETMLCATALOGPATH_PARAM stFolderDlgParam = { _T(""), TRUE };
//
//			if ( pItemData->hResData != NULL )
//			{
//				TPPluginGuiData  *pGdi = TP_GetPluginGuiData(_T("MaterialList"));
//				if(pGdi)	
//				{
//					if ( !bShowFolderDlg || (pGdi->funPluginGui.TP_GdiCallBack( TP_NET_GETMLCATALOGPATH, (LPARAM)&stFolderDlgParam) == S_OK) )
//					{
//						g_pClipManageInterface->stuSonapsInterface.TP_SendProgramToMPC( pItemData->hResData, stFolderDlgParam.sFolderPath, hWnd);//register wnd添加hWnd
//					}
//				}
//			}
//			else if ( pItemData->guidDBType != GUID_NULL && pItemData->guidRes != GUID_NULL )
//			{
//				HRESDATA hProgramData;
//				g_pClipManageInterface->stuProgramInterface.TP_GetProgram(pItemData->guidDBType,GUID_NULL,pItemData->guidRes,TP_RESLOCK_VIEW,hProgramData);
//
//				if ( hProgramData != NULL )
//				{
//					CString sFolderPath = _T("");
//					TPPluginGuiData  *pGdi = TP_GetPluginGuiData(_T("MaterialList"));
//					if(pGdi)	
//					{
//						if ( !bShowFolderDlg || (pGdi->funPluginGui.TP_GdiCallBack( TP_NET_GETMLCATALOGPATH, (LPARAM)&stFolderDlgParam) == S_OK) )
//						{
//							g_pClipManageInterface->stuSonapsInterface.TP_SendProgramToMPC( hProgramData, stFolderDlgParam.sFolderPath, hWnd);//register wnd添加hWnd
//						}
//					}
//				}
//				g_pClipManageInterface->stuProgramInterface.TP_ReleaseProgram( hProgramData );
//			}			
//		}
//		return;
//	case MENU_SENDTO_ML:
//		{
//			if ( g_pClipManageInterface->stuSonapsInterface.TP_SendResToML == NULL )
//				return;
//			if(g_pClipManageInterface->stuSonapsInterface.TP_IsMLValid() != S_OK)     return ;
//
//			DWORD           dwOperateFlag    = 0;
//			TPResDataArray  aSelData;int  iSel;
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//
//			while (posSel)
//			{
//				iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData)
//				{					
//					TPResData  stuData;
//					//{liubo add CD2009.10.01 2010-7-15 Download editing
//					stuData.guidDBType	= pItemData->guidDBType;
//					//}
//					stuData.eNodeType   = pItemData ->eNodeType;
//					stuData.eResType    = pItemData ->eResType|TP_NodeTypeToResType(pItemData ->eNodeType);
//					stuData.guidRes     = pItemData ->guidRes;	
//					stuData.hResCache   = m_hCacheData;
//					stuData.guidNode    = pItemData ->guidNode;
//					stuData.hResData    = pItemData->hResData;
//					dwOperateFlag      |= pItemData ->dwOperateFlag;
//					stuData.lParam      = (LPARAM)pItemData->dwBookIndex;
//					aSelData.Add(stuData);					
//				}
//			}			
//			if(aSelData.GetSize()<=0) return;
//
//			if((m_eNodeType & TP_NODE_SYSTEM) || dwOperateFlag & TP_OPERATEFLAG_SYSTEM) return;
//
//			struct tag_TPTemp
//			{
//				TP_RES_TYPE  eResType;
//				HRESDATA     hInitCatalog;
//				HRESDATA     hSelectCatalog;
//			}stuTemp;
//			stuTemp.hInitCatalog   = NULL;
//			stuTemp.eResType	   = TP_RES_NOTE;
//
//			TPDatabaseType emDBType = g_pClipManageInterface->stuSonapsInterface.TP_GetDatabaseType(g_pClipManageInterface->TP_GetCurDBType());
//			if(TPDATABASETYPE_NETWORK == emDBType)
//			{
//				if ( TP_GdiCallBack( TP_RES_SELECTRESPATH, (LPARAM)&stuTemp ) != S_OK )
//					return;
//			}
//			else if(TPDATABASETYPE_LOCAL == emDBType && S_OK == g_pClipManageInterface->stuSonapsInterface.TP_IsMLValid())
//			{
//				TPPluginGuiData  *pGdi = TP_GetPluginGuiData(_T("MaterialList"));
//				if ( pGdi->funPluginGui.TP_GdiCallBack( TP_NET_GETCATALOGPATH, (LPARAM)&stuTemp ) != S_OK ) return;
//			}
//			else return;						
//			g_pClipManageInterface->stuSonapsInterface.TP_SendResToML( stuTemp.hSelectCatalog, aSelData );
//		}
//		return;
//#endif	
//		//}
//	default:
//		if(uID >= ID_FILEEDIT_BASE && uID <= ID_AEPLUGIN_CMDMAX)
//		{
//			CTPResListCtrl::OnMenuCmd(uID);
//		}
//		else if(uID >= ID_RESLIST_SORTBYMIN && uID <= ID_RESLIST_SORTBYMAX)
//		{
//			NMHEADER phdr;
//			LRESULT  hResult;
//			phdr.iItem = uID - ID_RESLIST_SORTBYMIN;
//			OnHdnItemclick(0,(NMHDR *)&phdr,&hResult);
//		}
//		else if(uID >= ID_RESLISTSELECT_COLORBASE && uID <= ID_RESLISTSELECT_COLORMAX)
//		{
//			BYTE clcolor = uID - ID_RESLISTSELECT_COLORBASE + TP_CLIPCOLOR_BASE;
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData && (pItemData ->eResType & (TP_RES_NOTE |TP_RES_ALLPROGRAM|TP_RES_CATALOG | TP_RES_PROJECT)))
//				{
//					g_pClipManageInterface->stuFuncitonInterface.TP_SerResColorIndex(pItemData ->hResData,pItemData->eResType,clcolor);
//					pItemData ->cResColor = clcolor;
//					g_pTimeLineDataEdit->TP_TLData_ResetBaseResInfo(pItemData ->guidDBType,pItemData ->guidNode,pItemData->guidRes,pItemData->eResType,TP_SET_BASERESINFO_COLOR,(LPARAM)clcolor);
//				}			
//			}
//
//			TP_UpdateCurTLWnd(TP_TL_PAINTCOMPOSE);			
//			//			TP_CommonGdiCallBack(TP_TIMELINE_REFRESHRESCOLOR,NULL);
//			InvalidateRect(NULL);
//		}
//		else if(uID == ID_RESLISTSELECT_COLORDEF)
//		{
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData && (pItemData ->eResType & (TP_RES_NOTE |TP_RES_ALLPROGRAM|TP_RES_CATALOG | TP_RES_PROJECT)))
//				{
//					g_pClipManageInterface->stuFuncitonInterface.TP_SerResColorIndex(pItemData ->hResData,pItemData->eResType,0);
//					pItemData ->cResColor = 0;
//					g_pTimeLineDataEdit->TP_TLData_ResetBaseResInfo(pItemData ->guidDBType,pItemData ->guidNode,pItemData->guidRes,pItemData->eResType,TP_SET_BASERESINFO_COLOR,LPARAM(pItemData ->cResColor));
//				}
//			}
//			TP_UpdateCurTLWnd(TP_TL_PAINTCOMPOSE);
//			//	TP_CommonGdiCallBack(TP_TIMELINE_REFRESHRESCOLOR,NULL);
//			InvalidateRect(NULL);
//		}
//		else if(ID_RESLISTSELECT_CLIPMODIFY == uID)
//		{
//			//if(TP_IsSystemLoggingOperator()) return;
//			TPResDataArray aResData;
//			POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//			while (posSel)
//			{
//				int iSel = CTPListCtrlEx::GetNextSelectedItem(posSel);
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//				if(pItemData && (pItemData ->eResType & (TP_RES_NOTE|TP_RES_ALLPROGRAM)))
//				{
//					TPResData stuResData;
//					stuResData.guidRes	= pItemData->guidRes;
//					stuResData.eResType = pItemData->eResType;
//					stuResData.hResData = pItemData->hResData;
//					aResData.Add(stuResData);
//				}
//			}
//			if(aResData.GetSize() > 0)
//			{
//				CTPNSClipModifySheet dlg(_T(""));
//				dlg.SetResData(aResData,m_hCatalog);
//				dlg.DoModal();
//			
//				SetTimer(TP_EVENT_SETVAOUT, 700, NULL);
//			}
//		}
//	}
//}
void CTPNSResListCtrl::OnRButtonDownUnknow(UINT nFlags, CPoint point)
{


//{
//	CTPMenuEx menuLoad,*pSubMenu = NULL;
//	menuLoad.LoadMenu(IDR_RBUTNMENU_NS);		
//	int iIndex  = HitTest(point);
//	TPListItemData *pItemDataSelect = NULL;
//
//	if(iIndex >=0)	pItemDataSelect = (TPListItemData *) CTPListCtrlEx::GetItemData(iIndex);
//
//	TPCatalogData  stuCatalogData;
//	g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(m_hCatalog,&stuCatalogData);
//	if(m_eNodeType & TP_NODE_TRASH) 
//	{
//		if(GetSelectedCount() >0)
//		{
//			pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_FXRESTRASHLISTSEL);	
//		}
//		else
//		{
//			pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_FXRESTRASHLIST);
//			BOOL bDisable = g_pClipManageInterface->TP_IsTrashEmpty(stuCatalogData.guidDBType, stuCatalogData.guidRes,m_hCatalog);
//			if(bDisable)pSubMenu->EnableMenuItem(ID_RESTRASHLIST_EMPTYTRASH, MF_GRAYED | MF_DISABLED);
//
//			SetSortMenu(pSubMenu,NULL,0);
//		}		
//	}
//	//{fengxiaoping modify, 2007.09.16
//#ifdef TP_NS_NET
//	else if (m_eNodeType & TP_NODE_CG)
//	{
//		if(m_eNodeType & TP_NODE_CGPREFABRICATE)//
//		{
//			pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_CGPRRESLIST);	
//
//			if(GetSelectedCount() > 0)
//			{
//				// XN00026754
//				//UINT uIDCmd1[] = {ID_BASEPATH_OPENPROJECTEXSPLORERWITHTHISFOLDER,ID_SYSCMD_SEARCH,};
//				//pSubMenu ->EnableMenuItemsEx(uIDCmd1,sizeof(uIDCmd1)/sizeof(UINT),MF_BYCOMMAND);	
//			}
//			pSubMenu ->DeleteMenuEx(2, MF_BYPOSITION);//Separator
//			pSubMenu ->DeleteMenuEx(ID_BASEPATH_OPENPROJECTEXSPLORERWITHTHISFOLDER,MF_BYCOMMAND);
//			pSubMenu ->DeleteMenuEx(ID_SYSCMD_SEARCH,MF_BYCOMMAND);
//			pSubMenu ->DeleteMenuEx(ID_SYSCMD_PASTE,MF_BYCOMMAND);	//snp4100033185
//
//			//if( g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(m_hCatalog,NULL,FALSE)!=S_ACCEPT)
//			//	pSubMenu ->EnableMenuItem(ID_SYSCMD_PASTE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//			//CG里不要picture and text view
//			pSubMenu->DeleteMenuEx(ID_VIEWMODE_PICTUREANDTEXTVIEW,MF_BYCOMMAND);
//			SetSortMenu(pSubMenu,NULL,0);
//		}
//		else if(m_eNodeType & TP_NODE_CGTEMPLATE)//
//		{		
//			int iCount = CTPListCtrlEx::GetSelectedCount();
//			if(iCount > 0 && iIndex >=0)
//			{
//				pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_CGRESLISTSEL);
//
//				if(pSubMenu)
//
//				{
//					if(iCount > 1)
//					{
//						UINT uID[] = {ID_RESLISTSELECT_RENAME,ID_RESLISTSELECT_PICTUREICONPLAY,ID_RESLISTSELECT_OPEN};
//						pSubMenu->EnableMenuItemsEx(uID, sizeof(uID) / sizeof(UINT), MF_BYCOMMAND);
//					}
//					else
//					{
//						// LI HUI LI 2007-10-15 Disable open menu (add by qsj TL 打开情况下允许open)
//						POSITION Pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//						while(NULL != Pos)
//						{
//							int nItem = CTPListCtrlEx::GetNextSelectedItem(Pos);
//							TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(nItem);
//							if(!(pItemData->eResType & TP_RES_ALLCGTEMPLATE))
//							{
//								UINT uID[] = {ID_RESLISTSELECT_OPEN,ID_RESLISTSELECT_PICTUREICONPLAY};
//								pSubMenu->EnableMenuItemsEx(uID, sizeof(uID) / sizeof(UINT), MF_BYCOMMAND);		
//							}
//							else
//							{
//								//XN00028438 跟素材一样，可以open
//								//if(NULL == TP_GetCurManageData())
//								//{
//								//	UINT uID[] = {ID_RESLISTSELECT_OPEN};
//								//	pSubMenu->EnableMenuItemsEx(uID, sizeof(uID) / sizeof(UINT), MF_BYCOMMAND);		
//								//}
//							}
//							if(NULL != pItemData && !g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(pItemData->hResData, TP_RES_ALLTEMPLATE, TP_RIGHT_DELETE))
//							{
//								pSubMenu->EnableMenuItem(ID_RESLISTSELECT_DELETE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//								break;
//							}
//						}
//						if(pItemDataSelect && !g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(pItemDataSelect->hResData, TP_RES_ALLTEMPLATE, TP_RIGHT_EDIT))
//						{
//							pSubMenu->EnableMenuItem(ID_RESLISTSELECT_RENAME,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//							pSubMenu->EnableMenuItem(ID_RESLISTSELECT_CUT,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//						}
//					}				
//					pSubMenu->DeleteMenu(2, MF_BYPOSITION);				
//				}
//
//			}
//			else
//			{
//				pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_CGRESLIST);
//
//				if(pSubMenu)
//				{
//					// pSubMenu->DeleteMenu(ID_RESLIST_NEWBIN, MF_BYCOMMAND);
//					SetSortByMenu(pSubMenu,13);
//					pSubMenu ->DeleteMenu(ID_RESLIST_NEWSEQUENCE, MF_BYCOMMAND);
//					pSubMenu ->DeleteMenu(5, MF_BYPOSITION);//Separator
//					pSubMenu ->DeleteMenu(ID_BASEPATH_OPENPROJECTEXSPLORERWITHTHISFOLDER,MF_BYCOMMAND);
//					pSubMenu ->DeleteMenu(ID_SYSCMD_SEARCH,MF_BYCOMMAND);
//					//CG里不要picture and text view
//					pSubMenu ->DeleteMenu(ID_VIEWMODE_PICTUREANDTEXTVIEW,MF_BYCOMMAND);
//					if(g_pClipManageInterface->TP_GetCurProject() == NULL)// 如果不打开项目不允许创建模块。因为无法取到格式信息
//					{
//						UINT uID[] = {ID_CGLIST_ADDEMPTYTEMPLATE};
//						pSubMenu->EnableMenuItemsEx(uID, sizeof(uID) / sizeof(UINT), MF_BYCOMMAND);
//					}
//					if( g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(m_hCatalog,NULL,FALSE)!=S_ACCEPT)
//						pSubMenu ->EnableMenuItem(ID_SYSCMD_PASTE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//
//					if(!g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(NULL, TP_RES_ALLTEMPLATE, TP_RIGHT_NEW))
//						pSubMenu->EnableMenuItem(ID_CGLIST_ADDEMPTYTEMPLATE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//				}
//			}
//
//			SetSortMenu(pSubMenu,NULL,0);
//		}
//	}
//	
//	//else if(m_eNodeType & TP_NODE_CGTEMPLATE)
//	//{
//	//	if ( m_eNodeType & TP_NODE_SYSTEM ) 	return;
//
//	//	POSITION posSel = CTPListCtrlEx::GetFirstSelectedItemPosition();
//	//	int iCount = 0;
//	//	while(NULL != posSel)
//	//	{
//	//		iCount++;
//	//		CTPListCtrlEx::GetNextSelectedItem(posSel);
//	//	}
//
//	//	if(iCount > 0)
//	//	{
//	//		pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_CGRESLISTSEL);
//
//	//		pSubMenu->DeleteMenu(2, MF_BYPOSITION);
//	//		if(iCount > 1)
//	//		{
//	//			UINT uID[] = {ID_RESLISTSELECT_RENAME};
//	//			pSubMenu->EnableMenuItemsEx(uID, sizeof(uID) / sizeof(UINT), MF_BYCOMMAND);
//	//		}
//	//		POSITION Pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//	//		while(NULL != Pos)
//	//		{
//	//			int nItem = CTPListCtrlEx::GetNextSelectedItem(Pos);
//	//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(nItem);
//	//			if(NULL != pItemData && !g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(pItemData->hResData, TP_RES_ALLTEMPLATE, TP_RIGHT_DELETE))
//	//			{
//	//				pSubMenu->EnableMenuItem(ID_RESLISTSELECT_DELETE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//	//				break;
//	//			}
//	//		}
//	//		pSubMenu->EnableMenuItem(ID_RESLISTSELECT_OPEN, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//	//	}
//	//	else
//	//	{
//	//		pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_CGRESLIST);
//
//	//		pSubMenu->DeleteMenu(6, MF_BYPOSITION);//Separator
//	//		pSubMenu->DeleteMenu(ID_RESLIST_NEWBIN, MF_BYCOMMAND);
//	//		pSubMenu->DeleteMenu(ID_BASEPATH_OPENPROJECTEXSPLORERWITHTHISFOLDER, MF_BYCOMMAND);
//	//		pSubMenu->DeleteMenu(ID_SYSCMD_SEARCH, MF_BYCOMMAND);
//	//		pSubMenu->DeleteMenu(ID_RESLIST_NEWSEQUENCE, MF_BYCOMMAND);
//	//		pSubMenu->DeleteMenu(ID_RESLIST_ARRANGETOPLEFT, MF_BYCOMMAND);
//
//	//		SetSortByMenu(pSubMenu,7);
//	//		if( g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(m_hCatalog,NULL,FALSE)!=S_ACCEPT)
//	//			pSubMenu ->EnableMenuItem(ID_SYSCMD_PASTE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//
//	//		if(!g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(NULL, TP_RES_ALLTEMPLATE, TP_RIGHT_NEW))
//	//			pSubMenu->EnableMenuItem(ID_CGLIST_ADDEMPTYTEMPLATE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//	//	}
//	//}
//#endif
//	//}
//	else
//	{
//		int nItem = CTPListCtrlEx::HitTest(point, 0);
//		int iSel = GetSelectedCount();
//		BOOL bDisable = TRUE;
//		DWORD dwNodeIndex = TP_GetNodeIndex(stuCatalogData.eNodeType);
//		if(iSel >= 1 && nItem != -1)//有选中项
//		{
//			if(TP_FXINDEX_SYSEFFECTV == dwNodeIndex)		    
//			{
//			}
//			else if(TP_FXINDEX_SYSEFFECTA == dwNodeIndex)		
//			{
//			}
//			else 
//			{
//				pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_FXRESLISTSEL);	
//				SetSortMenu(pSubMenu,NULL,0);
//
//				//ID_FXRESLISTSELECT_EDIT
//				BOOL bRight = TP_GetResRight(NULL, TP_RES_ALLTEMPLATE, TP_RIGHT_EDIT);
//				if(bRight && iSel == 1)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_FXRESLISTSELECT_EDIT, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_BASEPATH_CUT
//				if(bRight)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_BASEPATH_CUT, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//				//ID_BASEPATH_COPY
//				if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//				if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				if(TP_FXINDEX_SYSTEMPLATECLIP  == dwNodeIndex)		bDisable = FALSE;
//				if(TP_FXINDEX_SYSTEMPLATETR  == dwNodeIndex)		bDisable = FALSE;
//				if(TP_FXINDEX_SYSCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_BASEPATH_COPY, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_BASEPATH_RENAME
//				if(bRight && iSel == 1)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_BASEPATH_RENAME, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_BASEPATH_DELETE
//				if(bRight)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_BASEPATH_DELETE, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_ICON_ORIGINALIMAGE
//				if(NULL != pItemDataSelect)
//				{
//					if(!(pItemDataSelect ->dwState & TP_RESSTATE_VASORCE))	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_ICON_ORIGINALIMAGE, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_ICON_IMAGEFILE
//				if(iSel == 1)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_ICON_IMAGEFILE, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//#ifndef TP_NS_NET
//				if(iSel == 1 && TP_IsProxyEditVersion() && pSubMenu)
//				{
//					int nPos = pSubMenu->GetMenuItemPos(ID_FXRESLISTSELECT_EDIT);// 删除分融条
//					if(nPos >= 0 && nPos + 1 < (int)pSubMenu->GetMenuItemCount())	pSubMenu->DeleteMenu(nPos + 1, MF_BYPOSITION);
//					pSubMenu->DeleteMenu(ID_FXRESLISTSELECT_EDIT, MF_BYCOMMAND);
//				}
//#endif
//
//#ifdef TP_NS_NET
//				POSITION Pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//				while(NULL != Pos)
//				{
//					int nItem = CTPListCtrlEx::GetNextSelectedItem(Pos);
//					TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(nItem);
//					if(NULL != pItemData && !g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(pItemData->hResData, TP_RES_ALLTEMPLATE, TP_RIGHT_DELETE))
//					{
//						pSubMenu->EnableMenuItem(ID_BASEPATH_DELETE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//						break;
//					}
//				}
//#endif
//			}
//		}
//		else // 没有选中项
//		{
//			if(TP_FXINDEX_SYSCOMMANDBUTTON1 == dwNodeIndex)    			
//			{
//			}
//			else if(TP_FXINDEX_SYSCOMMANDBUTTON2 == dwNodeIndex)       
//			{ 
//			}
//			else if(TP_FXINDEX_USERCOMMANDBUTTON2 == dwNodeIndex)      
//			{ 
//			}
//			else
//			{
//				pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_FXRESLIST);	
//				SetSortMenu(pSubMenu,NULL,0);
//
//				//ID_FXRESLIST_NEWEFFECT
//				BOOL bRight = TP_GetResRight(NULL, TP_RES_ALLTEMPLATE, TP_RIGHT_NEW);
//				if(bRight)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	
//					{
//						TPResDataArray aChildRes;
//						g_pClipManageInterface->stuCatalogInterface.TP_GetChildRes(m_hCatalog,aChildRes,TP_RESLOCK_VIEW);
//						if(aChildRes.GetSize() <= 0)	bDisable = FALSE;
//					}
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_FXRESLIST_NEWEFFECT, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//				//ID_BASEPATH_PASTE
//				BOOL bPasteFXData = g_pClipManageInterface ->stuFuncitonInterface.TP_PasteResDataTest(m_hCatalog,NULL,FALSE) == S_ACCEPT;
//				BOOL bPasteTLData = TP_PasteTLFXDataTest(m_eNodeType) == S_ACCEPT;
//				if(bPasteFXData || bPasteTLData)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}			
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_BASEPATH_PASTE, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;			
//				//界面排列
//				//ID_RESLIST_ARRANGETOPLEFT
//				//ID_SELECTION_SELECTA
//				//ID_SELECTION_INVERTSELECTION
//				//ID_VIEWMODE_PICTUREVIEW
//				//ID_VIEWMODE_TEXTVIEW
//				//ID_PCITUREICONSIZE_EXTRALARGE
//				//ID_PCITUREICONSIZE_LARGE
//				//ID_PCITUREICONSIZE_MEDIUM
//				//ID_PCITUREICONSIZE_SMALL
//			}
//		}
//


	//if(pSubMenu)
	//{
	//	m_iMenuItem = iIndex;
	//	TP_LoadMenuStr(pSubMenu);
	//	GetCursorPos(&point);
	//	pSubMenu ->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	//}
	//menuLoad.DestroyMenu();	
}
void CTPNSResListCtrl::SetSortMenu(CTPMenuEx *pSubMenu,CTPMenuEx *pSrCTPMenuEx,int iMode)  //通过添加对号标志来决定该项目是否正在选择
{
	//if(m_iViewType == VIEWTYPE_ICON)             pSubMenu ->CheckMenuItemEx(ID_VIEWMODE_PICTUREVIEW,MF_BYCOMMAND|MF_CHECKED);
	//else if(m_iViewType == VIEWTYPE_REPORT)      pSubMenu ->CheckMenuItemEx(ID_VIEWMODE_TEXTVIEW,MF_BYCOMMAND|MF_CHECKED);
	//else if(m_iViewType == VIEWTYPE_TEXTPICTURE) pSubMenu ->CheckMenuItemEx(ID_VIEWMODE_PICTUREANDTEXTVIEW,MF_BYCOMMAND|MF_CHECKED);

	//if(m_iViewType != VIEWTYPE_ICON)             pSubMenu ->EnableMenuItem(ID_VIEWMODE_ARRANGEWAY,MF_BYCOMMAND|MF_GRAYED|MF_DISABLED);
	//else if(m_iArrType == ARRANGE_AUTO)          pSubMenu ->CheckMenuItemEx(ID_VIEWMODE_ARRANGEWAY,MF_BYCOMMAND|MF_CHECKED);

	//if(m_lIconSize == 1 )          pSubMenu ->CheckMenuItemEx(ID_PCITUREICONSIZE_EXTRALARGE,MF_BYCOMMAND|MF_CHECKED);
	//else if(m_lIconSize == 2 )     pSubMenu ->CheckMenuItemEx(ID_PCITUREICONSIZE_LARGE,MF_BYCOMMAND|MF_CHECKED);
	//else if(m_lIconSize == 3 )     pSubMenu ->CheckMenuItemEx(ID_PCITUREICONSIZE_MEDIUM,MF_BYCOMMAND|MF_CHECKED);
	//else if(m_lIconSize == 4 )     pSubMenu ->CheckMenuItemEx(ID_PCITUREICONSIZE_SMALL,MF_BYCOMMAND|MF_CHECKED);

	//if(m_iViewType != VIEWTYPE_ICON)
	//{
	//	UINT uIDCmd1[] = {ID_PCITUREICONSIZE_EXTRALARGE,ID_PCITUREICONSIZE_LARGE,ID_PCITUREICONSIZE_MEDIUM,ID_PCITUREICONSIZE_SMALL,ID_RESLIST_ARRANGETOPLEFT,};
	//	pSubMenu ->EnableMenuItemsEx(uIDCmd1,sizeof(uIDCmd1)/sizeof(UINT),MF_BYCOMMAND);
	//}
}

void  TP_DeleteRestList(CTPMenuEx *pSubMenu)
{
	//pSubMenu->GetSubMenu(2)  ->DeleteMenu(ID_SYSCMD_IMPORTMXF, MF_BYCOMMAND);
	//pSubMenu ->GetSubMenu(3) ->DeleteMenu(4,MF_BYPOSITION);//Seperate Bar
	//pSubMenu ->GetSubMenu(3) ->DeleteMenu(ID_BATCHDIGITIZE_MXFBATCHTOOL,MF_BYCOMMAND);

	//pSubMenu ->GetSubMenu(4) ->DeleteMenu(ID_INPUT_IMPORTALE,MF_BYCOMMAND);
	//pSubMenu ->GetSubMenu(4) ->DeleteMenu(ID_INPUT_IMPORTOMF,MF_BYCOMMAND);

}

void  TP_DeleteRestListSel(CTPMenuEx *pSubMenu)
{
	//pSubMenu ->GetSubMenu(2) ->DeleteMenu(4,MF_BYPOSITION);//Seperate Bar
	//pSubMenu ->GetSubMenu(2) ->DeleteMenu(ID_BATCHDIGITIZE_MXFBATCHTOOL,MF_BYCOMMAND);


	//pSubMenu ->GetSubMenu(3) ->DeleteMenu(ID_OUTPUT_EXPORTOMF,MF_BYCOMMAND);
}

INT64 CTPNSResListCtrl::HitTestToolBarState()
{

	if(TP_WND_RESEXPLORER == m_dwWndType)
	{
		TPResDataArray aSelData;
		TPCatalogData  stuCatalogData;
		g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(m_hCatalog,&stuCatalogData);
	}
	//	//XN00026404 XN00023084  qushaojie 
	//	int iClip = 0,iSequence = 0,iBin = 0, nSelectCount = 0, iGroupFloder = 0,iProject = 0,iTrash = 0;

	//	TPListItemData * pItemDataSelect = NULL;
	//	POSITION posSel = GetFirstSelectedItemPosition();
	//	while (posSel)
	//	{		
	//		nSelectCount++;
	//		int iSel = GetNextSelectedItem(posSel);
	//		pItemDataSelect = (TPListItemData *) GetItemData(iSel);
	//		if(pItemDataSelect && (pItemDataSelect ->eResType & TP_RES_NOTE))            iClip++;
	//		else if(pItemDataSelect && (pItemDataSelect ->eResType & TP_RES_ALLPROGRAM)) iSequence++;
	//		else if(pItemDataSelect && (pItemDataSelect ->eResType & TP_RES_CATALOG))    iBin++;
	//		else if(pItemDataSelect && (pItemDataSelect ->eResType & TP_RES_PROJECT))	 iProject++;
	//		else if(pItemDataSelect && (pItemDataSelect ->eNodeType & TP_NODE_TRASH))     iTrash++;

	//	}
	//	INT64 uHitTest = 0;

	//	BOOL bTestCopy = FALSE;
	//	BOOL bOpenPrj  = FALSE;
	//	DWORD dwPEIndex = TP_GetNodeIndex(m_eNodeType);
	//	if(GUID_NULL != g_guidResProject.guidRes) bOpenPrj = TRUE;
	//	uHitTest = TP_GRADEMENU_NEWPROJECT|TP_GRADEMENU_NEWBIN|TP_GRADEMENU_NEWSEQUENCE|TP_GRADEMENU_LOGGING|TP_GRADEMENU_RECORDTOVTR|TP_GRADEMENU_SEARCH|TP_GRADEMENU_CUT|TP_GRADEMENU_COPY|TP_GRADEMENU_PASTE|TP_GRADEMENU_DELETE;
	//	if(TP_PEINDEX_USER == dwPEIndex)  // User Info
	//	{
	//		bTestCopy = TRUE;
	//		if(nSelectCount > 0)
	//			uHitTest &= ~(TP_GRADEMENU_DELETE|TP_GRADEMENU_COPY);
	//	}
	//	else if(TP_PEINDEX_TRASH == dwPEIndex) //回收站
	//	{
	//		if (nSelectCount > 0)
	//			uHitTest &= ~TP_GRADEMENU_DELETE;
	//	}
	//	else if(TP_PEINDEX_SEARCH == dwPEIndex)	// Search 
	//	{
	//		if(nSelectCount > 0)
	//			uHitTest &= ~TP_GRADEMENU_DELETE;
	//	}
	//	else if(TP_PEINDEX_NETWORK == dwPEIndex)  // Net Work
	//	{
	//	}
	//	else if(TP_PEINDEX_CGTEMPLATE == dwPEIndex)// 选择CG Template 目录
	//	{
	//		bTestCopy = TRUE;
	//		uHitTest &= ~(TP_GRADEMENU_NEWBIN|TP_GRADEMENU_SEARCH);
	//		if(nSelectCount > 0)  // 没有选择资源
	//			uHitTest &= ~(TP_GRADEMENU_CUT|TP_GRADEMENU_COPY|TP_GRADEMENU_DELETE);
	//	}
	//	else if(TP_PEINDEX_CGUSERTEMPLATE == dwPEIndex) // CG User Object 目录
	//	{
	//	}
	//	else if(TP_PEINDEX_CGSYSTEMPLATE == dwPEIndex) // CG Sys Object 目录
	//	{
	//	}
	//	else if(TP_PEINDEX_CG == dwPEIndex) // CG 根结点
	//	{

	//	}
	//	else if(TP_PEINDEX_PEBIN == dwPEIndex)//如果为 BIN
	//	{
	//		bTestCopy = TRUE;
	//		BOOL bCurPrj = stuCatalogData.guidProject == g_guidResProject.guidRes && stuCatalogData.guidDBType == g_guidResProject.guidDBType;
	//		uHitTest &= ~(TP_GRADEMENU_NEWBIN|TP_GRADEMENU_NEWPROJECT|TP_GRADEMENU_SEARCH);
	//		if(iClip + iSequence + iBin > 0)
	//		{
	//			uHitTest &= ~(TP_GRADEMENU_COPY|TP_GRADEMENU_CUT|TP_GRADEMENU_DELETE);
	//			if(iBin <= 0 && bCurPrj)
	//				uHitTest &= ~TP_GRADEMENU_RECORDTOVTR;
	//		}
	//		if(bCurPrj) uHitTest &= ~TP_GRADEMENU_NEWSEQUENCE;
	//	}
	//	else if(TP_PEINDEX_PRJFOLDER == dwPEIndex)//如果为 PROJECTFLODER
	//	{
	//		bTestCopy = TRUE;
	//		uHitTest &= ~(TP_GRADEMENU_NEWPROJECT|TP_GRADEMENU_SEARCH);
	//		if(nSelectCount > 0)
	//			uHitTest &= ~(TP_GRADEMENU_COPY|TP_GRADEMENU_CUT|TP_GRADEMENU_DELETE);
	//	}
	//	else if(TP_PEINDEX_PROJECTITEM == dwPEIndex)//如果为 PROJECTF
	//	{
	//		bTestCopy = TRUE;
	//		uHitTest &= ~(TP_GRADEMENU_NEWPROJECT|TP_GRADEMENU_NEWBIN|TP_GRADEMENU_SEARCH);
	//		if(nSelectCount == 1 && pItemDataSelect && (TP_NODE_TRASH & pItemDataSelect->eNodeType))  //只选中回收站
	//		{ 			
	//		}
	//		else if(nSelectCount > 0)
	//			uHitTest &= ~(TP_GRADEMENU_COPY|TP_GRADEMENU_CUT|TP_GRADEMENU_DELETE);
	//	}
	//	else if(TP_PEINDEX_COMPUTER == dwPEIndex)   // Computer
	//	{	
	//		bTestCopy = TRUE;
	//		uHitTest &= ~(TP_GRADEMENU_NEWPROJECT|TP_GRADEMENU_SEARCH);
	//		if(nSelectCount == 1 && pItemDataSelect && (TP_NODE_TRASH & pItemDataSelect->eNodeType))  //只选中回收站
	//		{ 			
	//		}
	//		else if(nSelectCount > 0)
	//			uHitTest &= ~(TP_GRADEMENU_COPY|TP_GRADEMENU_CUT|TP_GRADEMENU_DELETE);	
	//	}

	//	if(bOpenPrj)
	//		uHitTest &= ~TP_GRADEMENU_LOGGING;

	//	if(bTestCopy && g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(m_hCatalog,NULL,FALSE)== S_ACCEPT)
	//		uHitTest &= ~(TP_GRADEMENU_PASTE);

	//	return uHitTest;
	//}
	//else if(TP_WND_UNKONWEXPLORER == m_dwWndType)
	//{
	//	BOOL bTestPaste = FALSE;
	//	INT64 uHitTest = TP_GRADEMENU_NEWBIN|TP_GRADEMENU_CUT|TP_GRADEMENU_COPY|TP_GRADEMENU_PASTE|TP_GRADEMENU_DELETE;
	//	DWORD dwFXIndex = TP_GetNodeIndex(m_eNodeType);
	//	switch(dwFXIndex)
	//	{
	//	case TP_FXINDEX_BIN:
	//	case TP_FXINDEX_USERTEMPLATE:
	//	case TP_FXINDEX_PUBLICTEMPLATE:
	//		{
	//			bTestPaste = TRUE;
	//			if(CTPListCtrlEx::GetSelectedCount() > 0)
	//			{
	//				uHitTest &= ~(TP_GRADEMENU_CUT|TP_GRADEMENU_COPY|TP_GRADEMENU_DELETE);
	//			}
	//		}
	//		break;
	//	case TP_FXINDEX_SYSTEMPLATECLIP:
	//	case TP_FXINDEX_SYSTEMPLATETR:
	//		{
	//		}
	//		break;
	//	case TP_FXINDEX_SYSEFFECTV:
	//	case TP_FXINDEX_SYSEFFECTA:
	//	case TP_FXINDEX_SYSCOMMANDBUTTON1:
	//	case TP_FXINDEX_SYSCOMMANDBUTTON2:
	//	case TP_FXINDEX_USERCOMMANDBUTTON2:
	//		{
	//			// default
	//		}
	//		break;
	//	case TP_FXINDEX_SYSCOMMANDBUTTON3:
	//		{
	//			if(CTPListCtrlEx::GetSelectedCount() > 0)
	//				uHitTest &= ~(TP_GRADEMENU_COPY);
	//		}
	//		break;
	//	case TP_FXINDEX_USERCOMMANDBUTTON3:
	//		{
	//			bTestPaste = TRUE;
	//			if(CTPListCtrlEx::GetSelectedCount() > 0)
	//				uHitTest &= ~(TP_GRADEMENU_CUT|TP_GRADEMENU_COPY|TP_GRADEMENU_DELETE);
	//		}
	//		break;
	//	case TP_FXINDEX_TRASH:
	//		{
	//			if(CTPListCtrlEx::GetSelectedCount() > 0)
	//				uHitTest &= ~(TP_GRADEMENU_DELETE);
	//		}
	//		break;
	//	case TP_PEINDEX_CGTEMPLATE:
	//		{
	//			uHitTest &= ~TP_GRADEMENU_NEWBIN;
	//			POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
	//			if(NULL != pos) uHitTest &= ~(TP_GRADEMENU_CUT|TP_GRADEMENU_DELETE);
	//			while(NULL != pos)
	//			{
	//				int nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
	//				TPListItemData *pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);
	//				if(NULL == pItemData) continue;
	//				if(!TP_GetResRight(pItemData->hResData, pItemData->eResType, TP_RIGHT_DELETE))
	//				{
	//					uHitTest |= TP_GRADEMENU_DELETE;
	//					break;
	//				}
	//				if(!TP_GetResRight(pItemData->hResData, pItemData->eResType, TP_RIGHT_EDIT))
	//				{
	//					uHitTest |= TP_GRADEMENU_CUT; 
	//					break;
	//				}
	//			}
	//			bTestPaste = TRUE;
	//		}
	//		break;
	//	case TP_PEINDEX_CG:
	//	case TP_PEINDEX_CGUSERTEMPLATE:
	//	case TP_PEINDEX_CGSYSTEMPLATE:
	//		{

	//		}	// 
	//		break;
	//	}
	//	if(bTestPaste)
	//	{
	//		// li song ming 2008-11-12 DQAdb00003231
	//		BOOL bPasteFXData = g_pClipManageInterface ->stuFuncitonInterface.TP_PasteResDataTest(m_hCatalog,NULL,FALSE) == S_ACCEPT;
	//		BOOL bPasteTLData = FALSE;
	//		if(!bPasteFXData) bPasteTLData = TP_PasteTLFXDataTest(m_eNodeType) == S_ACCEPT;
	//		if(bPasteTLData || bPasteFXData)
	//			uHitTest &= ~TP_GRADEMENU_PASTE;
	//	}

	//	return uHitTest;
	//}

	return 0x7fffff;
}

void TP_AddExtraMenu(CTPMenuEx *pMenu, TPListItemData *pItemData)
{
//	CTPMenuEx *pSubMenu = pMenu ->GetSubMenu(3);
//	if(pSubMenu)
//	{
//		UINT uState = pSubMenu ->GetMenuState(ID_OUTPUT_EXPORTMXF,MF_BYCOMMAND);
//		if(uState != 0xFFFFFFFF && !(uState & MF_GRAYED))
//		{
//#ifndef TP_NS_NET
//			if(PathFileExists(_T("P:\\Browse\\Browse.exe")))
//			{
//				if(pItemData &&(pItemData ->eResType & TP_RES_TLPROGRAM))
//					pSubMenu ->InsertMenu(6,MF_BYPOSITION|MF_STRING,ID_OUTPUT_EXPORTHDXSBF,SBLoadString(_T("IDS_CLIPEXPLORER_EXPORTHDXSB")));		
//				pSubMenu ->InsertMenu(4,MF_BYPOSITION|MF_STRING,ID_OUTPUT_EXPORTHDX,SBLoadString(_T("IDS_CLIPEXPLORER_EXPORTHDXCHANGE")));
//			}
//#endif
//			if(g_pSystemInfo ->uSystemState & TP_SYSTEMSTATE_XDA)
//				pSubMenu ->InsertMenu(4,MF_BYPOSITION|MF_STRING,ID_OUTPUT_EXPORTAI3,SBLoadString(_T("IDS_CLIPEXPLORER_EXPORTAI3"))); 
//		}
//	}		
//	pSubMenu = pMenu ->GetSubMenu(4);
//	if(pSubMenu)
//	{
//		UINT uState = pSubMenu ->GetMenuState(ID_INPUT_IMPORTEDL,MF_BYCOMMAND);
//		if(uState != 0xFFFFFFFF)
//		{		
//#ifndef TP_NS_NET
//			if(!(uState & MF_GRAYED)  && PathFileExists(_T("P:\\Browse\\Browse.exe")))
//			{
//				pSubMenu ->InsertMenu(4,MF_BYPOSITION|MF_STRING,ID_OUTPUT_IMPORTHDXSBF,SBLoadString(_T("IDS_CLIPEXPLORER_EXPORTHDXSB")));					
//			}	
//#endif
//		}
//	}
}

void TP_AddDubExtraPropertiesMenu(CTPMenuEx *pSubMenu, TPListItemData *pItemData,TP_NODE_TYPE eNodeType,BOOL bShowVOICEOVER, BOOL bSpeed) 
{
	//if(NULL == pSubMenu) return;
	//if(NULL != pItemData && bShowVOICEOVER)
	//{
	//	if((pItemData->eSingSourceType & TP_CLIP_SOURCE_TYPE_DUB) && (pItemData ->eResType & TP_RES_NOTE) && !(eNodeType & TP_NODE_SEARCH))
	//	{
	//		TPClipData stuClipData;
	//		if(S_OK ==g_pClipManageInterface->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData, TP_GRADE_FILEBASE, stuClipData))
	//		{
	//			UINT uID[] = {ID_EXTRAPROPERTIES_VOICEOVERTAKE1, ID_EXTRAPROPERTIES_VOICEOVERTAKE2, ID_EXTRAPROPERTIES_VOICEOVERTAKE3, ID_EXTRAPROPERTIES_VOICEOVERTAKE4};
	//			for(int i = 0; i < 4; i++)
	//			{
	//				DWORD dwTempType = TP_CLIP_CLASS_A1 | TP_CLIP_CLASS_A2 | TP_CLIP_CLASS_A3 | TP_CLIP_CLASS_A4;
	//				DWORD eClipClass = stuClipData.aQuality[TP_CLIP_QUALITY_4].lSubIndex;
	//				if(eClipClass <= 0 || MAX_LONG == eClipClass) eClipClass = stuClipData.aQuality[TP_CLIP_QUALITY_4].eClipClass;

	//				dwTempType = dwTempType << (i * 4);
	//				if(!(eClipClass & dwTempType))
	//					pSubMenu->EnableMenuItem(uID[i], MF_GRAYED | MF_DISABLED | MF_BYCOMMAND);
	//			}
	//		}
	//		else
	//		{
	//			UINT uID[] = {ID_EXTRAPROPERTIES_VOICEOVERTAKE1, ID_EXTRAPROPERTIES_VOICEOVERTAKE2, ID_EXTRAPROPERTIES_VOICEOVERTAKE3, ID_EXTRAPROPERTIES_VOICEOVERTAKE4};
	//			pSubMenu->EnableMenuItemsEx(uID, sizeof(uID),MF_BYCOMMAND);
	//		}
	//		pSubMenu->DeleteMenu(ID_RESLISTSELECT_EXTRAPROPERTIES, MF_BYCOMMAND);

	//		if(!bSpeed)	// XN00029788 不是变速的要 灰BackToCreate
	//		{
	//			UINT uID[] = {ID_RESLISTSELECT_BACKTOCREATOR,};
	//			pSubMenu->EnableMenuItemsEx(uID, sizeof(uID) / sizeof(UINT), MF_BYCOMMAND);	  
	//		}
	//	}
	//	else
	//	{
	//		int nPos = pSubMenu->GetMenuItemPos(ID_RESLISTSELECT_EXTRAPROPERTIES);
	//		if(nPos >= 0 && nPos + 1 < (int)pSubMenu->GetMenuItemCount())
	//			pSubMenu->DeleteMenu(nPos + 1, MF_BYPOSITION);
	//	}
	//}
	//else
	//{
	//	int nPos = pSubMenu->GetMenuItemPos(ID_RESLISTSELECT_EXTRAPROPERTIES);
	//	if(nPos >= 0 && nPos + 1 < (int)pSubMenu->GetMenuItemCount())
	//		pSubMenu->DeleteMenu(nPos + 1, MF_BYPOSITION);
	//}
}

void CTPNSResListCtrl::OnRButtonDownClip(UINT nFlags, CPoint point)
{	
	//////////////////////////////////////////////////////////////////////////
	//IDS_CLIPEXPLORER_MENU_NEWBIN			
	//IDS_CLIPEXPLORER_MENU_NEWSEQUENCE	
	//IDS_CLIPEXPLORER_MENU_NEWCLIP
	//IDS_CLIPEXPLORER_MENU_SGCLIP		
	//IDS_CLIPEXPLORER_MENU_CREATETM		
	//IDS_CLIPEXPLORER_MENU_LOGGING		
	//IDS_CLIPEXPLORER_MENU_DELETE		
	//IDS_CLIPEXPLORER_MENU_CUT			
	//IDS_CLIPEXPLORER_MENU_COPY			
	//IDS_CLIPEXPLORER_MENU_PASTE			
	//IDS_CLIPEXPLORER_MENU_SELECTION
	//IDS_CLIPEXPLORER_MENU_SELECTA		
	//IDS_CLIPEXPLORER_MENU_INVERTSELECTION
	//IDS_CLIPEXPLORER_MENU_SEARCH		
	//IDS_CLIPEXPLORER_MENU_RENAME		
	//IDS_CLIPEXPLORER_MENU_VIEWMODE		
	//IDS_CLIPEXPLORER_MENU_PICTUREVIEW	
	//IDS_CLIPEXPLORER_MENU_TEXTVIEW		
	//IDS_CLIPEXPLORER_MENU_PICTUREANDTEXTVIEW	
	//////
	CTPMenuEx menuLoad,*pSubMenu = NULL;
	menuLoad.LoadMenu(IDR_RBUTNMENU_NS);		
	int iIndex  = HitTest(point);
	TPListItemData *pItemDataSelect = NULL;

	if(iIndex >=0)	pItemDataSelect = (TPListItemData *) CTPListCtrlEx::GetItemData(iIndex);

	TPCatalogData  stuCatalogData;
	g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(m_hCatalog,&stuCatalogData);

	if(m_eNodeType & TP_NODE_TRASH) 
	{
		//if(GetSelectedCount() >0)
		//{
		//	pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_FXRESTRASHLISTSEL);	
		//}
		//else
		//{
		//	pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_FXRESTRASHLIST);
		//	BOOL bDisable = g_pClipManageInterface->TP_IsTrashEmpty(stuCatalogData.guidDBType, stuCatalogData.guidRes,m_hCatalog);
		//	if(bDisable)pSubMenu->EnableMenuItem(ID_RESTRASHLIST_EMPTYTRASH, MF_GRAYED | MF_DISABLED);

		//	SetSortMenu(pSubMenu,NULL,0);
		//}		
	}
	else
	{
		int nItem = CTPListCtrlEx::HitTest(point, 0);
		int iSel = GetSelectedCount();
		BOOL bDisable = TRUE;
		DWORD dwNodeIndex = TP_GetNodeIndex(stuCatalogData.eNodeType);
		if(iSel >= 1 && nItem != -1)//有选中项
		{
//			if(TP_FXINDEX_SYSEFFECTV == dwNodeIndex)		    
//			{
//			}
//			else if(TP_FXINDEX_SYSEFFECTA == dwNodeIndex)		
//			{
//			}
//			else 
//			{
//				pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_FXRESLISTSEL);	
//				SetSortMenu(pSubMenu,NULL,0);
//
//				//ID_FXRESLISTSELECT_EDIT
//				BOOL bRight = TP_GetResRight(NULL, TP_RES_ALLTEMPLATE, TP_RIGHT_EDIT);
//				if(bRight && iSel == 1)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_FXRESLISTSELECT_EDIT, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_BASEPATH_CUT
//				if(bRight)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_BASEPATH_CUT, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//				//ID_BASEPATH_COPY
//				if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//				if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				if(TP_FXINDEX_SYSTEMPLATECLIP  == dwNodeIndex)		bDisable = FALSE;
//				if(TP_FXINDEX_SYSTEMPLATETR  == dwNodeIndex)		bDisable = FALSE;
//				if(TP_FXINDEX_SYSCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_BASEPATH_COPY, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_BASEPATH_RENAME
//				if(bRight && iSel == 1)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_BASEPATH_RENAME, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_BASEPATH_DELETE
//				if(bRight)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_BASEPATH_DELETE, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_ICON_ORIGINALIMAGE
//				if(NULL != pItemDataSelect)
//				{
//					if(!(pItemDataSelect ->dwState & TP_RESSTATE_VASORCE))	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_ICON_ORIGINALIMAGE, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//				//ID_ICON_IMAGEFILE
//				if(iSel == 1)
//				{
//					if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
//					if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
//				}
//				if(bDisable) 
//					pSubMenu->EnableMenuItem(ID_ICON_IMAGEFILE, MF_GRAYED|MF_DISABLED);
//				bDisable = TRUE;
//
//#ifndef TP_NS_NET
//				if(iSel == 1 && TP_IsProxyEditVersion() && pSubMenu)
//				{
//					int nPos = pSubMenu->GetMenuItemPos(ID_FXRESLISTSELECT_EDIT);// 删除分融条
//					if(nPos >= 0 && nPos + 1 < (int)pSubMenu->GetMenuItemCount())	pSubMenu->DeleteMenu(nPos + 1, MF_BYPOSITION);
//					pSubMenu->DeleteMenu(ID_FXRESLISTSELECT_EDIT, MF_BYCOMMAND);
//				}
//#endif
//
//#ifdef TP_NS_NET
//				POSITION Pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
//				while(NULL != Pos)
//				{
//					int nItem = CTPListCtrlEx::GetNextSelectedItem(Pos);
//					TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(nItem);
//					if(NULL != pItemData && !g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(pItemData->hResData, TP_RES_ALLTEMPLATE, TP_RIGHT_DELETE))
//					{
//						pSubMenu->EnableMenuItem(ID_BASEPATH_DELETE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//						break;
//					}
//				}
//#endif
//			}
		}
		else // 没有选中项
		{
			//if(TP_FXINDEX_SYSCOMMANDBUTTON1 == dwNodeIndex)    			
			//{
			//}
			//else if(TP_FXINDEX_SYSCOMMANDBUTTON2 == dwNodeIndex)       
			//{ 
			//}
			//else if(TP_FXINDEX_USERCOMMANDBUTTON2 == dwNodeIndex)      
			//{ 
			//}
			//else
			//{
				pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_RESLIST);	
				SetSortMenu(pSubMenu,NULL,0);

			//	//ID_FXRESLIST_NEWEFFECT
			//	BOOL bRight = TP_GetResRight(NULL, TP_RES_ALLTEMPLATE, TP_RIGHT_NEW);
			//	if(bRight)
			//	{
			//		if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
			//		if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	
			//		{
			//			TPResDataArray aChildRes;
			//			g_pClipManageInterface->stuTreeInterface.TP_GetChildRes(m_hCatalog,aChildRes,TP_RESLOCK_VIEW);
			//			if(aChildRes.GetSize() <= 0)	bDisable = FALSE;
			//		}
			//	}
			//	if(bDisable) 
			//		pSubMenu->EnableMenuItem(ID_FXRESLIST_NEWEFFECT, MF_GRAYED|MF_DISABLED);
			//	bDisable = TRUE;
			//	//ID_BASEPATH_PASTE
			//	BOOL bPasteFXData = g_pClipManageInterface ->stuFuncitonInterface.TP_PasteResDataTest(m_hCatalog,NULL,FALSE) == S_ACCEPT;
			//	BOOL bPasteTLData = TP_PasteTLFXDataTest(m_eNodeType) == S_ACCEPT;
			//	if(bPasteFXData || bPasteTLData)
			//	{
			//		if(TP_FXINDEX_BIN                 == dwNodeIndex)	bDisable = FALSE;
			//		if(TP_FXINDEX_USERCOMMANDBUTTON3  == dwNodeIndex)	bDisable = FALSE;
			//	}			
			//	if(bDisable) 
			//		pSubMenu->EnableMenuItem(ID_BASEPATH_PASTE, MF_GRAYED|MF_DISABLED);
			//	bDisable = TRUE;			
			//	//界面排列
			//	//ID_RESLIST_ARRANGETOPLEFT
			//	//ID_SELECTION_SELECTA
			//	//ID_SELECTION_INVERTSELECTION
			//	//ID_VIEWMODE_PICTUREVIEW
			//	//ID_VIEWMODE_TEXTVIEW
			//	//ID_PCITUREICONSIZE_EXTRALARGE
			//	//ID_PCITUREICONSIZE_LARGE
			//	//ID_PCITUREICONSIZE_MEDIUM
			//	//ID_PCITUREICONSIZE_SMALL
			//}
		}
	}
	if(pSubMenu)
	{
		m_iMenuItem = iIndex;
		TP_LoadMenuStr(pSubMenu);
		GetCursorPos(&point);	

		pSubMenu ->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	}
	menuLoad.DestroyMenu();


	//	else // nothing select
	//	{
	//		psubmenu = menuload.getsubmenu(tp_nsmenu_searchpath);

	//		uint uid[] = {id_basepath_delete};
	//		psubmenu->enablemenuitemsex(uid, sizeof(uid)/sizeof(uint));


	//		setsortbymenu(psubmenu,3);

	//	}
	//}
	//if(psubmenu)
	//{
	//	m_imenuitem = iindex;
	//	tp_loadmenustr(psubmenu);
	//	getcursorpos(&point);	

	//	psubmenu ->trackpopupmenu(tpm_leftalign,point.x,point.y,this);
	//}
	//menuload.destroymenu();	
}

void CTPNSResListCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CTPResListCtrl::OnSetFocus(pOldWnd);
	if(m_pResExplorer) 
	{
		CWnd *pFocus = m_pResExplorer  ->m_pFocus;
		m_pResExplorer  ->m_pFocus =this;	
		if(pFocus && pFocus != this) 
		{
			if(pFocus == m_pResTreeCtrl) ((CTPNSResTreeCtrl *) m_pResTreeCtrl)->OnNcPaint();
			else if(pFocus == m_pResBrowserCtrl) ((CTPNSResBrowserCtrl *) m_pResBrowserCtrl)->OnNcPaint();
			else                      pFocus ->Invalidate();
		}		
	}
	OnNcPaint();
	CTPListCtrlEx::Invalidate();
	if(m_pResExplorer) m_pResExplorer ->SetToolBarState();

	//{liubo add 2011-4-12 snp4100035279 如果正在处于命名操作，则焦点设给EditBox，不然OnLButtonDown中会直接返回。
	if (NULL != m_pEditWnd)
	{
		m_pEditWnd->SetFocus();
	}
	//}
}
void CTPNSResListCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CTPResListCtrl::OnKillFocus(pNewWnd);
	if(m_pResExplorer && m_pResExplorer  ->m_pFocus == this){ OnNcPaint(); CTPListCtrlEx::Invalidate();}
	PreviewGif(-1); // 没有焦点信息
}
void CTPNSResListCtrl::EditItem(GUID guidItem)
{
	for(INT l=0;l<GetItemCount();l++)
	{	
		TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(l);
		if(pItemData ->guidRes == guidItem)
		{
			if(!IsViewItem(l)){ m_guidEditRes = GUID_NULL; break;}
			GetItemResData(pItemData);
			EnsureVisible(l,TRUE);
			EditLable(l,0);
			break;
		}
	}
}
void CTPNSResListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV    = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	CTPResListCtrl::OnLvnBegindrag(pNMHDR, pResult);	
}
void CTPNSResListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 1;
	if(!m_bCanEdit)  return;
	if(m_uBaseState & TP_BASESTATE_DELETEITEM) return;
	if((pNMLV ->uNewState & LVIS_SELECTED )  && pNMLV ->lParam)
	{ 
		TPListItemData *pItemData = (TPListItemData *)pNMLV ->lParam;
		if(pItemData && (pItemData ->dwState & TP_RESSTATE_HIDE)) return;
		GetItemResData(pItemData);

		SetTimer(TP_EVENT_SELRES, 100, NULL);
		//	if(m_pMsgWnd) m_pMsgWnd->PostMessage(WM_LIST_STATECHANGE,pNMLV ->iItem,(LPARAM)pNMLV ->uNewState);
	}	
	else
	{
		TPListItemData *pItemData = (TPListItemData *)pNMLV ->lParam;
		if(pItemData && (pItemData ->dwState & TP_RESSTATE_HIDE)) return;

		if(pItemData && !(pNMLV ->uNewState & LVIS_SELECTED )  && (pItemData ->eResType &(TP_RES_NOTE )))//29029，模板在取消选中时清掉上线源,和PE选择素材保持一致
			SetTimer(TP_EVENT_SETVAOUT,100,NULL);

		SetTimer(TP_EVENT_SELRES, 300, NULL);
	}
}

void CTPNSResListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	TPListItemData  *pItemData = (TPListItemData *)pNMLV ->lParam;
	if(pItemData)
	{
		// DQAdb00003944 LI HUI LI 2008-12-26 需要将Flag再设置成NULL, ReleaseData时，会设置UseFlag，报错
		if(g_pClipManageInterface ->stuFuncitonInterface.TP_SetResUseFlag)
			g_pClipManageInterface ->stuFuncitonInterface.TP_SetResUseFlag(pItemData ->hResData,NULL);

		//// 清除Info View 显示
		//if(m_pResExplorer && m_pResExplorer->m_pBottomPanel && pItemData->guidRes == m_pResExplorer->m_pBottomPanel->m_guidRes)
		//	m_pResExplorer->m_pBottomPanel->ClearInfo();

		if(m_hResProxyView == pItemData->hResData) PreviewGif(-1);
		TP_ReleaseRes(pItemData->hResData, pItemData->eResType);

		delete pItemData;	
	}
	if(m_iMenuItem == pNMLV->iItem) m_iMenuItem = -1;
	*pResult = 0;
}

void CTPNSResListCtrl::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{	
	LPNMLISTVIEW pDispInfo1 = reinterpret_cast<NMLISTVIEW *>(pNMHDR); //ID:13374
	*pResult = 1;
	//if(!m_bCanEdit) return;	
	//m_sEditLabel = _L("");
	//m_hSaveData  = NULL;

	//TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(pDispInfo1->iItem);
	//if(NULL == pItemData) _return;
	//if(pItemData ->dwState & TP_RESSTATE_HIDE) _return;
	//INT nEditSub = GetItemOrder(pDispInfo1->iSubItem);
	//if(nEditSub <0) _return;
	//if(pItemData && (!((TP_RES_NOTE|TP_RES_ALLPROGRAM) & pItemData->eResType))&&(nEditSub == TP_RESTEXT_COMMENT)) _return;
	//if(pItemData->dwCaptureFlag & TP_CLIPCAPTURE_UPLOADING) _return;

	//if(m_eNodeType & TP_NODE_SEARCH)	g_pClipManageInterface->stuCatalogInterface.TP_GetCatalog(pItemData->guidDBType, GUID_NULL, pItemData->guidNode, m_hSaveData);
	//else								m_hSaveData = m_hCatalog;

	//if(m_hSaveData == NULL){ ASSERT(0);_return;}
	//if((m_eNodeType & TP_NODE_CG) && (pItemData && (pItemData->eResType & TP_RES_CATALOG)) && (nEditSub == TP_TEMPALTECGTEXT_COMMENT)) _return;
	//if(pItemData)  m_sEditLabel = pItemData->sText[0];
	//else  _return;
	//const INT64 dwNodeType = (TP_NODE_COMMANDBUTTON3 |TP_NODE_SYSCOMMANDBUTTON);
	//if((pItemData->eNodeType & dwNodeType) == dwNodeType) _return;
	//if(!g_bEnableALLMenu && ((pItemData->eNodeType & TP_NODE_SYSTEM) || (pItemData->eNodeType & TP_NODE_CGPREFABRICATE))) _return;
	//if(!(pItemData->dwOperateFlag & TP_OPERATEFLAG_EDITINFO)) _return; 
	//m_guidEditRes = GUID_NULL;//已改为单个素材刷新，先不用此变量
	//*pResult = 0;
}

void CTPNSResListCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

//	m_guidEditRes = GUID_NULL;
//	if(m_pResExplorer)
//	{
//		m_pResExplorer->m_uState &= ~TP_NEWSEQUENCE;
//	}
//	if(!m_bCanEdit) return;
//	int  iHeadIndex     = TP_NodeTypeHeadIndex(m_eShowNodeType);
//	CWnd      *pWnd     = NULL;
//	CString    sTemp;
//	int        iSubItem = 0;
//	BOOL       bEdit    = FALSE;
//	CTPListCtrlEx::GetEditItem(pDispInfo->item.iItem,iSubItem,pWnd);
//	INT nEditSub = GetItemOrder(iSubItem);
//	if(nEditSub <0) return;
//	int iItem = pDispInfo->item.iItem;
//	TPListItemData  *pItemData = (TPListItemData *)GetItemData(pDispInfo->item.iItem);
//	//liubo add PE rename refresh issue
//#ifdef TP_NS_NET
//	g_pClipManageInterface->stuSonapsInterface.TP_LockRefresh(0);
//	//RENAME锁定
//	if(pItemData != NULL)
//		g_pClipManageInterface->stuSonapsInterface.TP_LockResInMLForRename(pItemData->hResData, pItemData->eResType, FALSE);
//#endif
//	if(pWnd) pWnd->GetWindowText(sTemp);sTemp.Trim();pDispInfo->item.pszText = (LPTSTR)(LPCTSTR)sTemp;
////	TPNetLock stuLock(guidName);	
//
//	if(pItemData == NULL) return;
//	TP_RES_TYPE eResType = pItemData->eResType;	
//	if(/*lstrlen(pDispInfo->item.pszText) != 0 && */lstrcmp(m_sEditLabel,pDispInfo->item.pszText) !=0 && !(TP_BASESTATE_CANCLEEDIT & m_uBaseState))
//	{
//		GUID guidSame = GUID_NULL;
//		CString sText = pDispInfo->item.pszText,sGet,sCheck;
//		sText.TrimLeft();  sText.TrimRight();
//		if(m_sEditLabel.CompareNoCase(pDispInfo->item.pszText) == 0)
//		{
//			if(m_sEditLabel.Compare(pDispInfo->item.pszText) == 0)	pItemData = NULL;//不修改
//		}
//		else if(NULL != pItemData) 
//		{
//			if(TP_RESTEXT_CLIP_NAME == nEditSub)
//			{
//				if(sText.IsEmpty() )
//				{
//					CString sMsg = _T("");
//					if(pItemData->eResType & TP_RES_NOTE)
//						sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_ENTERCLIPNAME"));
//					else if(pItemData->eResType & TP_RES_ALLPROGRAM)
//						sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_ENTERSEQNAME"));
//					else if(pItemData->eResType & TP_RES_ALLTEMPLATE)
//					{
//						if (pItemData->eResType & TP_RES_ALLCGTEMPLATE)
//							sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_ENTERCGNAME"));
//						else
//							sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_ENTEREFFECTNAME"));
//
//					}else if(pItemData->eResType & TP_RES_CATALOG )
//					{
//						if(pItemData->eNodeType & TP_NODE_PROJECTFLODER)
//							sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_ENTERPROGROUPNAME"));
//						else
//							sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_ENTERBINNAME"));
//					}
//					else if(pItemData->eResType & TP_RES_PROJECT)
//						sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_ENTERPRONAME"));
//					else if(pItemData->eResType & TP_RES_USER)
//						sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_ENTERUSERNAME"));
//					pItemData = NULL; // 不修改
//					if(!sMsg.IsEmpty())
//						AfxMessageBox(sMsg, MB_OK | MB_ICONSTOP);//]]]	
//
//					*pResult = 1;
//				}
//				else
//				{
//					if(pItemData->eResType & TP_RES_NOTE)
//						sCheck = g_pClipManageInterface->stuClipInterfce.TP_CheckClipName(m_hSaveData,sText,NULL);
//					else if(pItemData->eResType & TP_RES_ALLPROGRAM)
//						sCheck = g_pClipManageInterface->stuProgramInterface.TP_CheckProgramName(m_hSaveData,(LPTSTR)(LPCTSTR)sText,NULL);
//					else if(pItemData->eResType & TP_RES_ALLTEMPLATE)
//						sCheck = g_pClipManageInterface->stuTemplateInterface.TP_CheckTemplateName(m_hSaveData,sText,NULL);
//					else if(pItemData->eResType & (TP_RES_CATALOG | TP_RES_PROJECT))
//						sCheck = g_pClipManageInterface->stuCatalogInterface.TP_CheckCatalogName(m_hSaveData,sText);
//					else if(pItemData->eResType & TP_RES_USER)
//						sCheck = g_pUserMangeInterface->TP_NS_CheckUserName((LPTSTR)(LPCTSTR)sText,guidSame);
//					//modified by chenxiaolong 2009.09.28;for bug 12419
//					if(sText.CompareNoCase(sCheck) != 0 && TP_RESTEXT_COMMENT!= m_aColumn[iSubItem]->iIndexInit) // && !(pItemData->eResType & TP_RES_USER))
//					{
//						if (TP_RESTEXT_COMMENT!= GetItemOrder(iSubItem))
//						{
//							CString sMsg = _T("");
//							if(pItemData->eResType & TP_RES_NOTE)
//								sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_CLIPNAMEE"));
//							else if(pItemData->eResType & TP_RES_ALLPROGRAM)
//								sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_SEQNAMEE"));
//							else if(pItemData->eResType & TP_RES_ALLTEMPLATE)
//								sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_EFFECTNAMEE"));
//							else if(pItemData->eResType & TP_RES_CATALOG)
//							{
//								if(pItemData->eNodeType & TP_NODE_PROJECTFLODER)
//									sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_PROGROUPNAMEE"));
//								else if( pItemData->eNodeType & TP_NODE_PROJECT)
//									sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_PROJECTE"));
//								else
//									sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_BINNAMEE"));
//							}
//							else if(pItemData->eResType & TP_RES_PROJECT)
//								sMsg = SBLoadString(_T("IDS_CLIPEXPLORER_PROJECTE"));
//							else if(pItemData->eResType & TP_RES_USER)
//								sMsg=SBLoadString(_T("IDS_CLIPEXPLORER_USERNAMEE"));
//
//							pItemData = NULL;// 不修改
//
//							*pResult = 1;
//
//							if(!sMsg.IsEmpty())	AfxMessageBox(sMsg, MB_OK | MB_ICONSTOP);
//						}		
//					} 		
//				}
//				sText = TP_GetValidName(sText);	
//			}
//		}	
//
//
//		if(pItemData && (pItemData->hResData || (pItemData ->eResType & TP_RES_USER)))
//		{
//			if((TP_TEMPALTECGTEXT_COMMENT == nEditSub) && (TP_HEADINDEX_CGTEMPLAGE == iHeadIndex))
//			{
//				if(pItemData->eResType & TP_RES_ALLTEMPLATE) 
//				{
//					TPTemplateData  stuTemplateData;
//					g_pClipManageInterface->stuTemplateInterface.TP_GetTemplateInfo(pItemData->hResData,TP_GRADE_EXTINFO,stuTemplateData);
//					stuTemplateData.sDescription = (LPTSTR)(LPCTSTR)sText;
//					g_pClipManageInterface->stuTemplateInterface.TP_SetTemplateInfo(pItemData->hResData,TP_GRADE_EXTINFO,stuTemplateData);
//					g_pClipManageInterface->stuTemplateInterface.TP_SaveTemplate(pItemData->hResData,TP_GRADE_EXTINFO);
//					pItemData->sText[nEditSub] =  sText;
//					bEdit = TRUE;
//			}
//			}
//			else if(TP_RESTEXT_COMMENT == nEditSub && (TP_HEADINDEX_BIN == iHeadIndex))
//			{
//				if(pItemData->eResType & TP_RES_NOTE)
//				{
//					TPClipData  stuClipData;
//					g_pClipManageInterface->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData,TP_GRADE_EXTINFO,stuClipData);
//					stuClipData.sDescription = (LPTSTR)(LPCTSTR)sText;	
//					g_pClipManageInterface->stuClipInterfce.TP_SetClipInfo(pItemData->hResData,TP_GRADE_EXTINFO,stuClipData);
//					g_pClipManageInterface->stuClipInterfce.TP_SaveClip(pItemData->hResData,TP_GRADE_EXTINFO);
//					pItemData->sText[nEditSub] =  sText;
//					bEdit = TRUE;
//				}
//				else if(pItemData->eResType & TP_RES_ALLPROGRAM)
//				{
//					TPProgramData  stuProgramData;
//					g_pClipManageInterface->stuProgramInterface.TP_GetProgramInfo(pItemData->hResData,TP_GRADE_EXTINFO,stuProgramData);
//					stuProgramData.sDescription = (LPTSTR)(LPCTSTR)sText;
//					g_pClipManageInterface->stuProgramInterface.TP_SetProgramInfo(pItemData->hResData,TP_GRADE_EXTINFO,stuProgramData);
//					g_pClipManageInterface->stuProgramInterface.TP_SaveProgram(pItemData->hResData,TP_GRADE_EXTINFO);
//					pItemData->sText[nEditSub] =  sText;
//					bEdit = TRUE;
//				}
//			}
//			else if(TP_USEREXT_COMMENT == nEditSub && (TP_HEADINDEX_USER ==iHeadIndex ))//add by qsj 20100104 用户信息PE中修改XN00027491
//			{
//				TPUserDepartmentBaseInfo stuInfo;	
//				if (S_OK == g_pUserMangeInterface ->TP_NS_GetUserInfo(pItemData->guidRes,stuInfo))
//				{
//					stuInfo.sUserConfigrationDes	    = (LPTSTR)(LPCTSTR)sText;
//					g_pUserMangeInterface ->TP_NS_SetUserInfo(pItemData->guidRes,stuInfo);
//					pItemData->sText[nEditSub] = sText;
//					bEdit = TRUE;
//				}       
//			}
//			else if(TP_RESTEXT_CLIP_NAME == nEditSub)
//			{
//				if(pItemData ->eResType & TP_RES_USER)
//				{
//					TPUserDepartmentBaseInfo stuUseInfo;
//					g_pUserMangeInterface->TP_NS_GetUserInfo(pItemData->guidRes,stuUseInfo);
//					stuUseInfo.sUserConfigrationName = (LPTSTR)(LPCTSTR)sText;
//					g_pUserMangeInterface->TP_NS_SetUserInfo(pItemData->guidRes,stuUseInfo);
//					pItemData->sText[0] =  sText;
//					if(g_pUserEnterInfo->guidUserConfigration == pItemData->guidRes) 
//						TP_RefreshUserName(&stuUseInfo); 
//					pItemData ->bGetData = FALSE;
//					GetItemResData(pItemData);
//					bEdit = TRUE;
//				}
//				else if(pItemData->eResType & TP_RES_NOTE)
//				{
//					TPClipData  stuClipData;
//					g_pClipManageInterface->stuClipInterfce.TP_GetNoteInfo(pItemData->hResData,TP_GRADE_BASE,stuClipData);
//					stuClipData.sName = (LPTSTR)(LPCTSTR)sText;
//					g_pClipManageInterface->stuClipInterfce.TP_SetClipInfo(pItemData->hResData,TP_GRADE_BASE,stuClipData);
//					g_pClipManageInterface->stuClipInterfce.TP_SaveClip(pItemData->hResData,TP_GRADE_BASE);
//					pItemData->sText[nEditSub] =  sText;
//					bEdit = TRUE;
//			}
//				else if(pItemData->eResType & TP_RES_ALLPROGRAM)
//				{
//					TPProgramData  stuProgramData;
//					g_pClipManageInterface->stuProgramInterface.TP_GetProgramInfo(pItemData->hResData,TP_GRADE_BASE,stuProgramData);
//					stuProgramData.sName = (LPTSTR)(LPCTSTR)sText;
//					g_pClipManageInterface->stuProgramInterface.TP_SetProgramInfo(pItemData->hResData,TP_GRADE_BASE,stuProgramData);
//					g_pClipManageInterface->stuProgramInterface.TP_SaveProgram(pItemData->hResData,TP_GRADE_BASE);
//					pItemData->sText[nEditSub] =  sText;
//					bEdit = TRUE;
//				}
//				else if(pItemData->eResType & TP_RES_ALLTEMPLATE)
//				{
//					TPTemplateData  stuTemplateData;
//					g_pClipManageInterface->stuTemplateInterface.TP_GetTemplateInfo(pItemData->hResData,TP_GRADE_BASE,stuTemplateData);
//					stuTemplateData.sName = (LPTSTR)(LPCTSTR)sText;
//					g_pClipManageInterface->stuTemplateInterface.TP_SetTemplateInfo(pItemData->hResData,TP_GRADE_BASE,stuTemplateData);
//					g_pClipManageInterface->stuTemplateInterface.TP_SaveTemplate(pItemData->hResData,TP_GRADE_BASE);
//					pItemData->sText[nEditSub] =  sText;
//					bEdit = TRUE;
//				}
//				else if(pItemData->eResType & TP_RES_CATALOG)
//				{
//					TPCatalogData stuData;
//					g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(pItemData->hResData,&stuData);
//					stuData.sName = (LPTSTR)(LPCTSTR)sText;
//					g_pClipManageInterface->stuCatalogInterface.TP_SetCatalogInfo(pItemData->hResData,&stuData);
//					g_pClipManageInterface->stuCatalogInterface.TP_SaveCatalog(pItemData->hResData,&stuData);
//					pItemData->sText[nEditSub] =  sText;
//					bEdit = TRUE;
//				}
//				else if(pItemData->eResType & TP_RES_PROJECT)
//				{
//					TPProjectData stuProject;
//					g_pClipManageInterface->stuProjectInterface.TP_GetProjectInfo(pItemData->hResData, TP_GRADE_BASE, stuProject);
//					stuProject.sName = (LPTSTR)(LPCTSTR)sText;
//					g_pClipManageInterface->stuProjectInterface.TP_SetProjectInfo(pItemData->hResData, TP_GRADE_BASE, stuProject);
//					g_pClipManageInterface->stuProjectInterface.TP_SaveProject(pItemData->hResData, TP_GRADE_BASE);
//					pItemData->sText[nEditSub] = sText;
//					bEdit = TRUE;
//				}
//			}
//		}		
//	}
//
//	if(bEdit|| TP_RESTEXT_CLIP_NAME != nEditSub)
//	{
//		m_sEditLabel = _L("");
//		m_hSaveData  = NULL;
//	}
//
//	if(bEdit && pItemData && m_pResExplorer && m_pResExplorer->m_pSearchBar && !m_sResFilter.IsEmpty()) // 重命名后，也需要重新过虑一下
//	{  
//		BOOL bShow = pItemData->pItem->bShow;
//		CString sName = pItemData ->sText[0];
//		pItemData->pItem->bShow = FileterResMothed(m_sResFilter,sName);	
//		if(!m_bRelayout && pItemData->pItem->bShow != bShow)
//		{
//			if(!pItemData->pItem->bShow) pItemData->pItem->EmptySubItem();
//			m_bRelayout = TRUE;
//			PostMessage(WM_LIST_RELAYOUT,0,0);
//		}		
//	}	
}
extern CString sTitle[M1][M2];
extern int     iColumnMax [M2];
extern int     iColumnWidth[M1][M2];
extern BOOL    iColumnView[M1][M2]; 
extern int     iColumnOrder[M1][M2];
extern int     iColumniIndexInit[M1][M2];

void CTPNSResListCtrl::SetSortByMenu(CTPMenuEx *pSubMenu,int iIndex)
{
	//int        iHeadIndex = TP_NodeTypeHeadIndex(m_eShowNodeType);
	//CTPMenuEx *pSortMenu  = pSubMenu ->GetSubMenu(iIndex);
	//if(pSortMenu == NULL && iHeadIndex >=0)
	//{
	//	pSortMenu  = new CTPMenuEx();
	//	pSortMenu ->CreatePopupMenu();
	//	pSubMenu ->InsertMenu(iIndex,MF_BYPOSITION|MF_POPUP,(UINT_PTR)pSortMenu->m_hMenu,TP_LoadStr(_T("IDS_CLIPEXPLORER_MENU_SORTBY")));
	//	pSubMenu ->SetMenuItemAttachMenu(iIndex,MF_BYPOSITION,pSortMenu,TRUE);	
	//}

	//if(iHeadIndex >=0 && iColumnMax[iHeadIndex] >0 && pSortMenu)
	//{
	//	while(pSortMenu ->GetMenuItemCount()>0) pSortMenu ->DeleteMenu(0,MF_BYPOSITION);		
	//	for(int l=0;l<m_aColumn.GetSize();l++)
	//	{
	//		if(!m_aColumn[l] ->bShow) continue;
	//		INT nID = ID_RESLIST_SORTBYMIN + l;
	//		pSortMenu ->AppendMenu(MF_STRING|MF_BYCOMMAND, nID,m_aColumn[l]->sItem);			
	//		if(m_iSortIndex == l)			pSortMenu ->CheckMenuItem(nID,MF_BYCOMMAND|MF_CHECKED);
	//	}
	//}
	//else if(pSortMenu)
	//{
	//	pSubMenu ->DeleteMenu(iIndex,MF_BYCOMMAND);
	//}
}
void  CTPNSResListCtrl::DelResData(TPResDataArray &hChildRes)
{
	for(INT l = m_aItem.GetSize()-1;l>=0;l--)
	{
		if(m_aItem[l]->dwData == NULL) continue;
		TPListItemData* pItemData = (TPListItemData*)m_aItem[l]->dwData;
		for(INT k= hChildRes.GetSize()-1;k>=0;k--)
		{
			if(hChildRes[k].guidRes == pItemData ->guidRes && hChildRes[k].guidDBType == pItemData ->guidDBType)
			{
				hChildRes.RemoveAt(k);
				DeleteItem(l);
				break;
			}
		}
	}
}
void  CTPNSResListCtrl::SetResData(TPResData& stuShow, TPResDataArray &hChildRes)
{
	m_bReDraw         = FALSE;
	m_bRelayout       = FALSE;

	SetRedraw(FALSE);
	if(m_hSelTimer) KillTimer(m_hSelTimer);
	m_hSelTimer = NULL;
	
	INT iResNeed = 0;
	if(m_guidCatalog != stuShow.guidRes || m_guidDBType != stuShow.guidDBType)// 切换目录了
	{
		SaveResIconIndex();
		CTPListCtrlEx::DeleteAllItems();
		if(m_eShowNodeType != stuShow.eNodeType)
			SaveListState(TRUE, 0); // 保存表头信息

		m_hCatalog			   = stuShow.hResData;
		m_eNodeType			   = stuShow.eNodeType;
		m_guidDBType		   = stuShow.guidDBType;
		m_guidCatalog		   = stuShow.guidRes;
		m_guidProjectRev	   = GUID_NULL;
		SaveListState(FALSE, stuShow.eNodeType);// 重新设计表头
		FunCloumnHide();
	}
	else
	{
		for(INT i = CTPListCtrlEx::GetItemCount() - 1; i >= 0; i--)
		{
			BOOL bDelete = TRUE;
			TPListItemData* pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(i);
			if(NULL != pItemData)
			{
				for(INT j = hChildRes.GetSize()-1; j >= 0; j--)	//查找是否存在
				{
					if(pItemData->guidRes == hChildRes[j].guidRes && pItemData->guidDBType == hChildRes[j].guidDBType)
					{
						bDelete = FALSE;
						pItemData->bGetData = FALSE;
						TP_ReleaseRes(hChildRes[j].hResData, hChildRes[j].eResType);
						hChildRes.RemoveAt(j);
						break;
					}
				}
			}
			if(bDelete) CTPListCtrlEx::DeleteItem(i);  // 被删除了
		}
		iResNeed = CTPListCtrlEx::GetItemCount();
	}

	for(int l=0;l<hChildRes.GetSize();l++)
	{		
		if(!(m_uResFilter & hChildRes[l].eResType))
		{
			TP_ReleaseRes(hChildRes[l].hResData, hChildRes[l].eResType);
			continue;
		}
		TPListItemData *pItemData = NULL;
		CTPListCtrlEx::InsertItem(iResNeed,_L("         "));
		pItemData = new TPListItemData();
		pItemData ->SetResBaseData(hChildRes[l]);
		pItemData ->pItem = m_aItem[iResNeed];
		//GetItemResData(pItemData);		
		GetItemResIndex(pItemData);
		CTPListCtrlEx::SetItemData(iResNeed,(DWORD_PTR)pItemData);
		iResNeed ++;
		//}end
	}
 //线程后台取资源
	g_pClipManageInterface ->stuFuncitonInterface.TP_GetResDataThread(&hChildRes,0);
	//处理完毕
	hChildRes.RemoveAll();	

	if (VIEWTYPE_ICON == m_iViewType && ARRANGE_STORYBOARD == m_iArrType)
	{
		for (int j=0;j<m_aItemIndexSave.GetSize();j++)
		{	
			for (int i=0;i<m_aItem.GetSize();i++)
			{
				TPListItemData * pItemData1 = (TPListItemData*)m_aItem[i]->dwData;
				if (pItemData1->guidRes == m_aItemIndexSave[j].guidRes)
				{
					pItemData1->pItem->lIconIndex = m_aItemIndexSave[j].wParam;
					break;
				}
			}
		}
	}
	m_aItemIndexSave.RemoveAll();
//	FunItemIconIndex(FALSE, TRUE);

	FunCalcSize();
	SetRedraw(TRUE);
}


void   CTPNSResListCtrl::ChangeColume(int iType)
{
	//int    iHeadIndex = TP_NodeTypeHeadIndex(m_eShowNodeType);
	//if(iHeadIndex >=0)
	//{
	//	CRect rtItem;
	//	for(int l=0;l<GetColumnCount();l++)
	//	{									
	//		iColumnWidth[iHeadIndex][l] = GetColumnWidth(l);	
	//		iColumnView[iHeadIndex][l]  = m_aColumn[l]->bShow;
	//	}
	//	GetColumnOrderArray(iColumnOrder[iHeadIndex],iColumnMax[iHeadIndex]);
	//}
	//if(m_iSortIndex >= 0)
	//{
	//	for(INT i = 0;i< m_aColumn.GetSize(); i++)
	//	{
	//		if(m_aColumn[i]->uFmt & (HDF_SORTDOWN|HDF_SORTUP))
	//		{
	//			m_iSortIndex = i;
	//			g_pClipManageInterface ->stuCatalogInterface.TP_SetCatalogSortType(m_hCatalog,0,m_bSortAsc,m_iSortIndex);
	//			break;
	//		}
	//	}
	//}
}
void CTPNSResListCtrl::UpdateView(GUID guidDBType,TP_RES_TYPE eResType,TP_NODE_TYPE eNodeTypel,CGuidArray *hResDataList)
{	
//	BOOL bPaint = FALSE;
//	if(TP_RES_USER &eResType) //用户信息修改
//	{
//		if(!(m_eNodeType & TP_NODE_USERINFO)) return;
//		if(m_dwWndType != TP_WND_RESEXPLORER) return;
//
//		for(INT l=0;l<hResDataList->GetSize();l++)
//		{ 
//			for(INT k=CTPListCtrlEx::GetItemCount()-1;k>=0;k--)
//			{								
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(k);
//				if(pItemData && pItemData ->guidRes == hResDataList ->GetAt(l))
//				{
//					pItemData ->bGetData = FALSE;
//					bPaint = TRUE;
//					break;
//				}				 
//			}
//		}
//	}
//	else if(eResType & TP_RES_UPDATEALLRES)
//	{
//		for(INT l=0;l<hResDataList->GetSize();l++)
//		{
//			INT k = -1;
//			for(k=CTPListCtrlEx::GetItemCount()-1;k>=0;k--)
//			{								
//				TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(k);
//				if(pItemData && pItemData ->guidRes == hResDataList ->GetAt(l))
//				{
//					bPaint = TRUE;
//					pItemData ->bGetData = FALSE;					
//					if(pItemData ->pItem)
//						pItemData ->pItem ->bShow    = TP_LIST_COLNUMMAX;	//临时使用
//					if(m_pResExplorer->m_bInfoView && m_pResExplorer->m_pBottomPanel && m_pResExplorer->m_pBottomPanel->m_guidRes == pItemData->guidRes)
//					{
//						if(m_pResExplorer->m_pBottomPanel->m_bUpdateView)
//						{
//							GetItemResData(pItemData);
//							m_pResExplorer->m_pBottomPanel->SetRes(pItemData->eResType,pItemData,TRUE);
//						}
//						else
//						{
//							m_pResExplorer->m_pBottomPanel->m_bUpdateView = TRUE;
//						}
//					}
//#ifdef TP_NS_NET
//					// 质量标志闪烁控制
//					if ( pItemData->eResType & TP_RES_NOTE )
//					{
//						TPResFlashIcon  stuTemp;
//						stuTemp.guidRes = pItemData->guidRes;
//						TPClipData stuClipData;
//						g_pClipManageInterface->stuClipInterfce.TP_GetNoteInfo( pItemData->hResData, TP_GRADE_BASE, stuClipData );
//						if ( stuClipData.dwCaptureFlag & TP_CLIPCAPTURE_UPLOADING )
//						{
//
//							stuTemp.bStart        = TRUE;
//							stuTemp.dwQualityType = 2;//0 high only, 1 low only ,2 high and low 默认一起闪
//
//							if ( !(stuClipData.dwCaptureFlag & TP_CLIPCAPTURE_UPLOADINGL) ) // 高质量闪
//							{
//								stuTemp.dwQualityType    = 0;//0 high only, 1 low only ,2 high and low
//							}
//							else if ( !(stuClipData.dwCaptureFlag & TP_CLIPCAPTURE_UPLOADINGH) ) // 低质量闪
//							{
//								stuTemp.dwQualityType    = 1;//0 high only, 1 low only ,2 high and low
//							}
//						}
//						else
//						{
//							stuTemp.bStart = FALSE;
//						}
//
//						extern LRESULT TP_GdiCallBackForCenter(DWORD dwCmd,LPARAM lParam);
//						TP_GdiCallBackForCenter( TP_RES_FLASHQUALITYICON, (LPARAM)&stuTemp);
//					}
//
//					break;
//#endif
//				}
//			}
//
//			//{liubo modify CD2009.10.01 2010-7-30 S2CX00001985 当要更新一个还没有显示出来的Item时，就将它交给刷新线程处理。
//#ifdef TP_NS_NET
//			if (k < 0)
//			{
//				AddUpdateRes(hResDataList->GetAt(l));
//			}
//#endif
//			//}
//		}
//	}	
//	if(bPaint)
//		{
//			m_bReDraw = TRUE;//		
//			Invalidate();
//		}

}
void CTPNSResListCtrl::OnTimer(UINT nIDEvent)
{
//	if(nIDEvent == TP_EVENT_STATUSBAR)
//	{
//		KillTimer(m_hSelTimer);
//		m_hSelTimer = NULL;
//		int iClip = 0,iTL =0,iTmp =0,iBin =0;
//		POSITION pos  = GetFirstSelectedItemPosition();
//		int      iSel = -1;
//		while(pos)
//		{
//			iSel = GetNextSelectedItem(pos);
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);
//			if(pItemData)
//			{
//				if(pItemData->eResType & TP_RES_NOTE)                          iClip++;
//				else if(pItemData->eResType & TP_RES_ALLPROGRAM)               iTL++;
//				else if(pItemData->eResType & TP_RES_ALLTEMPLATE)              iTmp++;
//				else if(pItemData->eResType & TP_RES_CATALOG | TP_RES_PROJECT) iBin++;
//			}
//		}
//		CString sText,sTemp;
//		if(iBin>0)	{sTemp.Format(SBLoadString(_T("IDS_CLIPEXPLORER_BIND")),iBin);sText+=sTemp;}
//		if(iTL>0)	{sTemp.Format(SBLoadString(_T("IDS_CLIPEXPLORER_TLD")),iTL);sText+=sTemp;}
//		if(iClip>0)	{sTemp.Format(SBLoadString(_T("IDS_CLIPEXPLORER_CLIPD")),iClip);sText+=sTemp;}
//		if(iTmp>0)  {sTemp.Format(SBLoadString(_T("IDS_CLIPEXPLORER_TPLD")),iTmp);sText+=sTemp;}
//		m_pResExplorer->m_pStatusBar->SetPaneText(0,sText,TRUE);
//
//	}
//	else if(TP_ENENT_PREGIF == nIDEvent)
//	{
//		KillTimer(TP_ENENT_PREGIF);
//		m_uListTimer[TP_LISTTIMER_PREVIEWGIF] = 0;
//		CPoint ptCursor;
//		GetCursorPos(&ptCursor);
//		ScreenToClient(&ptCursor);
//		INT nItem = HitTest(ptCursor);
//		PreviewGif(nItem);
//	}
//	else if(TP_EVENT_FLASH == nIDEvent)
//	{
//		if(VIEWTYPE_ICON == m_iViewType)
//		{					
//			TPListItemData *pItemData = NULL;
//			CDC            *pDC       = GetDC();
//			BOOL            bFlash    = FALSE;
//			ASSERT(NULL != pDC);
//			for(int l=0;l< m_aItem.GetSize();l++)
//			{					
//				pItemData = (TPListItemData*)m_aItem[l]->dwData;
//				if(pItemData == NULL || (pItemData ->dwCaptureFlag & TP_CLIPCAPTURE_UPLOADING) == 0) continue;
//
//				bFlash = TRUE;
//
//				CRect rtItem, rtQuality;
//
//				INT nOffset = 0;
//				INT nTypeOffset = 3;
//				int   iIndex = TP_CLIPICON_N4;
//				if(m_lIconSize == 1)        {nOffset = 0;}
//				else if(m_lIconSize == 2)   {nOffset = 3;}
//				else if(m_lIconSize == 3)	{nOffset = 2;}
//				else if(m_lIconSize == 4)	{nOffset = 1;}
//				CRect rtEdge = CRect(6+nOffset,25,6+nOffset,25);
//
//				this->GetItemRect(l, &rtItem, LVIR_BOUNDS);
//				INT nCurIndex = TP_CLIPSTATE_NOHIGHANDPROXYCLIPS;
//				switch(pItemData ->dwCaptureFlag & TP_CLIPCAPTURE_UPLOADING)
//				{
//				case TP_CLIPCAPTURE_UPLOADING:
//					nCurIndex = TP_CLIPSTATE_HASBOTHOFHIGHPROXYCLIPS;
//					break;
//				case TP_CLIPCAPTURE_UPLOADINGH:
//					if(pItemData && pItemData->eClipClass[1] && (pItemData->eSingSourceType & TP_CLIP_SOURCE_TYPE_DIGITIZEL))   
//						nCurIndex = TP_CLIPSTATE_HASBOTHOFHIGHPROXYCLIPS;
//					else
//						nCurIndex = TP_CLIPSTATE_NOPROXYCLIPBUTHASHIGHCLIP;
//					break;
//				case TP_CLIPCAPTURE_UPLOADINGL:
//					if(pItemData && pItemData->eClipClass[0] && (pItemData->eSingSourceType & TP_CLIP_SOURCE_TYPE_DIGITIZEH))
//						nCurIndex = TP_CLIPSTATE_HASBOTHOFHIGHPROXYCLIPS;
//					else
//						nCurIndex = TP_CLIPSTATE_NOHIGHCLIPBUTHASPROXYCLIP;
//					break;
//				default:
//					nCurIndex = TP_CLIPSTATE_NOHIGHANDPROXYCLIPS;
//				}
//				// 
//				if(m_bFlashIcon)	nCurIndex = TP_CLIPSTATE_NOHIGHANDPROXYCLIPS;
//
//				rtQuality       = rtItem;
//				rtQuality.right-= rtEdge.right;
//				rtQuality.left  = rtQuality.right - m_pCommonRes[TP_CLIPICON_STATUSINDICATORS]->Width();
//				rtQuality.top   = rtQuality.top  + (rtEdge.top - m_pCommonRes[TP_CLIPICON_STATUSINDICATORS]->Height())/2+nTypeOffset;
//
//				ImageList_Draw(m_pCommonRes[TP_CLIPICON_STATUSINDICATORS] ->GetImage(),nCurIndex ,pDC->m_hDC,rtQuality.left,rtQuality.top ,ILD_TRANSPARENT);
//			}			
//			ReleaseDC(pDC);			
//			m_bFlashIcon      = !m_bFlashIcon;
//
//
//			if(!bFlash && m_uFlashTimer)
//			{
//				m_uFlashTimer = NULL;
//				KillTimer(m_uFlashTimer);
//			}
//		}
//	}
//	else if(TP_EVENT_SETVAOUT == nIDEvent)
//	{
//		KillTimer(TP_EVENT_SETVAOUT);
//
//		// LI HUI LI 2008-11-26只有当前激活的窗口是PE时，才设置上线源, DQA3740
//		if(g_pFocusClipExplorerPanel == NULL) return;
//		//{2010-11-23:CD2009.10.01:chenzhi:add:29029,网络版用户特技模板在FX explorer,因此，也需要设置上线源
//#ifdef TP_NS_NET
//		//{2011-01-21:zcd:add
//		//Windows有BUG，操作任务栏时Active状态出问题
//		DWORD dwProcessID;
//		HWND hWnd = ::GetForegroundWindow();
//		if (hWnd)
//		{
//			::GetWindowThreadProcessId(hWnd, &dwProcessID);
//			if (dwProcessID != ::GetCurrentProcessId())
//				return;
//		}
//		//}
//		if(g_pFocusClipExplorerPanel->m_dwWndType != TP_WND_RESEXPLORER && g_pFocusClipExplorerPanel->m_dwWndType != TP_WND_UNKONWEXPLORER) return;
//#else	
//		if(g_pFocusClipExplorerPanel->m_dwWndType != TP_WND_RESEXPLORER) return;	
//#endif //TP_NS_NET
//		//}
//	
//		if(g_pFocusClipExplorerPanel != this->m_pResExplorer) return;
//		if(NULL == g_stuClipExpolrerCallBack.TP_TimeLineCallBack) return;
//
//		CTPBaseFrame* pCurFrame = CTPBaseFrame::GetBaseFrame(GetParentOwner());
//		if(pCurFrame && !(pCurFrame->m_uState & STATE_ACTIVATE))  return;
//
//		DWORD    eClipClass = 0;
//		DWORD    eDbeClass  = 0;
//		int      iV = 0,iA=0,iCG= 0,iDbe=0;
//		POSITION pos        = GetFirstSelectedItemPosition();
//		int      iSel       = -1;
//		BOOL     bClipOnly	= TRUE;
//		BOOL     bCurrentTL = FALSE; 
//		HTLDATA  hCurrentTL = NULL;    
//		BOOL     bVersion   = g_stuClipExpolrerCallBack.TP_TimeLineCallBack(TP_TIMELINE_BNESTING,NULL) != S_OK; //7605
//		while(pos)
//		{
//			iSel = GetNextSelectedItem(pos);
//			TPListItemData *pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel); //7605
//			if(pItemData == NULL || !(pItemData  ->eResType & (TP_RES_NOTE|TP_RES_ALLPROGRAM|TP_RES_ALLCGTEMPLATE))) continue;
//			if(pItemData  ->eResType & TP_RES_ALLPROGRAM) bCurrentTL = TP_IsCurrenTL(pItemData->guidRes,hCurrentTL);
//			else bCurrentTL = FALSE;
//
//			if(pItemData)
//			{
//				if(bCurrentTL)
//				{
//				}
//				else
//				{				
//					eClipClass |= (pItemData ->eClipClass[0] | pItemData ->eClipClass[1]) & ~(pItemData->eDbeClass[0] | pItemData->eDbeClass[1]);	
//					eDbeClass  |= pItemData->eDbeClass[0] | pItemData->eDbeClass[1];
//				}
//			}
//
//			if(bVersion)
//			{
//				if(pItemData &&  pItemData  ->eResType & TP_RES_NOTE)
//				{
//					TPTLVASelect stuVASelectEx;
//					stuVASelectEx.SetEnable(pItemData ->eClipClass[0] | pItemData ->eClipClass[1]);					
//					stuVASelectEx.SetDbeEnable(pItemData->eDbeClass[0]   | pItemData->eDbeClass[1]);	
//					iV   = max(iV,  stuVASelectEx.iVCount);
//					iA   = max(iA,  stuVASelectEx.iACount);
//					iCG  = max(iCG, stuVASelectEx.iGCount);
//					iDbe = max(iDbe,stuVASelectEx.iDbeCount);
//				}
//				else if(pItemData &&  pItemData  ->eResType & TP_RES_ALLPROGRAM)
//				{
//					bClipOnly = FALSE;
//					if(bCurrentTL)
//					{
//					}
//					else
//					{					
//						TPProgramData stuProgramData;
//						g_pClipManageInterface->stuProgramInterface.TP_GetProgramInfo(pItemData->hResData,TP_GRADE_BASE,stuProgramData);
//						iV   = max(iV,  stuProgramData.lVideoTrack);
//						iA   = max(iA,  stuProgramData.lAudioTrack);
//						iCG  = max(iCG, stuProgramData.lGraphTrack);
//						iDbe = max(iDbe,stuProgramData.lADbeTrack);	
//					}
//				}
//				else if(pItemData && pItemData ->eResType & TP_RES_ALLCGTEMPLATE)
//				{
//					bClipOnly = TRUE;
//					iCG  = max(iCG, 1);
//				}
//			}
//		}
//		TPTLVASelect stuVASelect;stuVASelect.bSrcView = FALSE;
//		if(bVersion && !bClipOnly)
//		{
//			stuVASelect.bNest = FALSE; //7605 jw 20080505
//			stuVASelect.SetEnable(iV,iA,iCG,iDbe);			
//		}
//		else
//		{	
//			stuVASelect.bNest = TRUE; //7605 jw 20080505
//			stuVASelect.SetEnable(eClipClass);					
//			stuVASelect.SetDbeEnable(eDbeClass,FALSE);//XN00013868 20080801				
//		}
//		g_stuClipExpolrerCallBack.TP_TimeLineCallBack(TP_TIMELINE_VAOUT,(LPARAM)&stuVASelect);
//		return ;
//	}
	/*else*/
	if(TP_EVENT_SELRES == nIDEvent)
	{
		KillTimer(TP_EVENT_SELRES);
		POSITION pos    = GetFirstSelectedItemPosition();
		int      iSel   = -1;
		TPListItemData *pItemData  = NULL;
		while(pos)
		{
			iSel = GetNextSelectedItem(pos);
			pItemData = (TPListItemData *) CTPListCtrlEx::GetItemData(iSel);			
		}

		UINT uState = CTPListCtrlEx::GetItemState(iSel,LVIS_SELECTED |  LVIS_FOCUSED);
		if(m_pMsgWnd) m_pMsgWnd->PostMessage(WM_LIST_STATECHANGE,iSel,(LPARAM)uState);

		if(m_pResExplorer)
		{
			// 更新toolBar状态
			m_pResExplorer  ->SetToolBarState();
			if(m_pResExplorer ->m_pStatusBar){	if(!m_hSelTimer) m_hSelTimer = SetTimer(TP_EVENT_STATUSBAR,500,NULL);}

			// 刷新选中的素材
			if(m_dwWndType == TP_WND_RESEXPLORER && pItemData)
			{	
				if(pItemData ->eResType & TP_RES_NOTE)
				{
					if(m_pResExplorer && m_pResExplorer->m_pResBrowserCtrl)
					{
						//m_pResExplorer->m_pResBrowserCtrl->m_bUpdateView = TRUE; 
						TPResData stuResData ;
						stuResData.guidDBType = pItemData->guidDBType ;
						stuResData.guidRes    = pItemData->guidRes;
						stuResData.eResType   = pItemData->eResType;
						stuResData.hResData   = pItemData->hResData;
						m_pResExplorer ->m_pResBrowserCtrl ->SetInfo(stuResData);
					}
				}
				//else
				//{
				//	if(m_pResExplorer->m_pResBrowserCtrl)		m_pResExplorer->m_pResBrowserCtrl->ClearInfo();
				//}
			}
			else 
			{
				if(m_pResExplorer->m_pResBrowserCtrl)		m_pResExplorer->m_pResBrowserCtrl->ClearInfo();
			}
		}

		return ;

	}
//	//{liubo add 2010-12-15 snp4100031924 重写ListCtrlEx的点两次名字重命名功能，如果用户没有修改权限,则不允许用户重命名
//#ifdef TP_NS_NET
//	else if (nIDEvent == 100)
//	{
//		CWnd *pFocus = GetFocus();
//		if(m_iItemLast>=0 && this == pFocus) 
//		{
//			HRESDATA hResData = NULL;
//			TP_RES_TYPE eResType = TP_RES_UNKNOW;
//
//			if (m_aItem[m_iItemLast]->dwData)
//			{
//				TPListItemData *pListItemData = (TPListItemData *)m_aItem[m_iItemLast]->dwData;
//
//				hResData = pListItemData->hResData;
//				eResType = pListItemData->eResType;
//				if (pListItemData->eNodeType & TP_NODE_PROJECTFLODER)
//					eResType |= TP_NODE_PROJECTFLODER;
//			}
//
//			if (!g_pClipManageInterface->stuSonapsInterface.TP_GetResRight(hResData, eResType, TP_RIGHT_EDIT))
//			{
//				KillTimer(100);
//				return;
//			}
//		}
//
//	}
//#endif
//	//}

	CTPResListCtrl::OnTimer(nIDEvent);
}

void CTPNSResListCtrl::SelectItemGuid(CGuidArray &aItem, BOOL bUnSelAll /*= TRUE*/)
{
	if(bUnSelAll)
	{
		CTPListCtrlEx::SelectItem(-1,TRUE,TRUE);
	}
	BOOL bSet = FALSE;
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
					if(!bSet)
					{
						bSet = TRUE;
						FunNotifyParent(LVN_ITEMCHANGED,l,0);
						EnsureVisible(l, FALSE);;
					}
					break;
				}
			}
		}
	}

	Invalidate();
}
void CTPNSResListCtrl::OnDestroy()
{
	SaveResIconIndex();
	SaveListState(TRUE, 0);
	if(m_pDropTarget)
	{
		m_pDropTarget ->Revoke();
		delete m_pDropTarget;
		m_pDropTarget = NULL;
	}	
	KillTimer(TP_EVENT_STATUSBAR);
	KillTimer(TP_EVENT_FLASH);
	KillTimer(TP_EVENT_SETVAOUT);
	KillTimer(TP_EVENT_SELRES);

	if(m_hSelTimer) KillTimer(m_hSelTimer);
	m_hSelTimer = NULL;
	CTPResListCtrl::OnDestroy();
}

void CTPNSResListCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	// 因为Update 消息还没有响应完,所以这时记下的hMenuItem会不正确
	//TP_TransMessage(WM_UPDATEVIEW);
	m_ptRButtonUp = point;
	if(m_rtClient.PtInRect(point) && !(m_eDragResType & TP_RES_RBUTTONDRAG))
	{
		CTPListCtrlEx::OnLButtonUp(nFlags,point);
		m_iDragMode = TP_DRAG_UNKNOW;	
		if((VIEWTYPE_REPORT == m_iViewType || VIEWTYPE_TEXTPICTURE == m_iViewType) && (point.y <= m_iColumnHeight))
		{
			CTPListCtrlEx::OnRButtonUp(nFlags,point);
			return;
		}
		if(!m_bCanEdit) return;
		if(m_dwWndType == TP_WND_RESEXPLORER)     OnRButtonDownClip(nFlags,point);
		else if(m_dwWndType == TP_WND_UNKONWEXPLORER)  OnRButtonDownUnknow(nFlags,point);
	}
	else
	{
		CTPResListCtrl::OnLButtonUp(nFlags,point);
	}
}
void CTPNSResListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(GetFocus() != this) SetFocus();

	m_hitTest.iItem = -1;
	m_ptRButtonDown = point;

	if((m_iViewType ==VIEWTYPE_REPORT || m_iViewType ==VIEWTYPE_TEXTPICTURE) && point.y <m_iColumnHeight)
		CTPListCtrlEx::OnRButtonDown(nFlags,point);
	else if(m_iDragMode == TP_DRAG_ITEMOK && (nFlags & MK_LBUTTON))
	{
		// 当左键拖动时，按下右键
		ReleaseDragImage();//capturechange中已处理，确认释放一次
		CTPListCtrlEx::OnLButtonUp(nFlags,point);
	}
	else
	{
		if(!m_rtClient.PtInRect(point)) return ; // 如果不在客户区不处理
		if(m_dwWndType == TP_WND_UNKONWEXPLORER)
		{
			int nItem = CTPListCtrlEx::HitTest(point, 0);
			if(nItem == -1)
			{
				m_bLeftDrag     = FALSE;
				return;
			}				
		}
		CTPListCtrlEx::OnLButtonDown(nFlags,point);
	}

	m_bLeftDrag     = FALSE;
}

void CTPNSResListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(TP_DRAG_ITEMOK == m_iDragMode && (MK_RBUTTON & nFlags))
	{
		// 当右键拖动时，按下左键
		ReleaseDragImage();//capturechange中已处理，确认释放一次
		CTPListCtrlEx::OnLButtonUp(nFlags,point);
	}
	else
	{
		if(!m_rtClient.PtInRect(point)) return ; // 如果不在客户区不处理
		CTPResListCtrl::OnLButtonDown(nFlags,point);
	}
}

LRESULT CTPNSResListCtrl::Drop(ULONGLONG dwFrom,TP_RES_TYPE dwDropType,LPARAM lParam)
{
	//if(m_dwWndType != TP_WND_RESEXPLORER || !(dwFrom & TP_DATA_RBUTTONDRAG) || (dwFrom & TP_DATA_BRDEXPLORER))  return CTPResListCtrl::Drop(dwFrom,dwDropType,lParam);
	//if(!(dwFrom & TP_DATA_RBUTTONDRAG) || (dwFrom & TP_DATA_BRDEXPLORER)) return CTPResListCtrl::Drop(dwFrom,dwDropType,lParam);
	//if(dwFrom & TP_DATA_CLIPEDIT)          return S_DECLINE;
	//if(m_uListState & TP_LISTSTATE_NODROP) return S_DECLINE; // XN00023034 

	//TPResDataArray *paResData = (TPResDataArray *)lParam;
	//if(paResData == NULL || paResData ->GetSize() <=0) return S_OK;
	//TP_NODE_TYPE eNodeType = paResData->GetAt(0).eNodeType;
	//if((dwFrom & TP_DATA_RBUTTONDRAG) && m_guidDBType == paResData->GetAt(0).guidDBType)
	//{		
	//	if(!m_eNodeType) return S_OK;

	//	CTPMenuEx menuLoad,*pSubMenu = NULL;
	//	menuLoad.LoadMenu(IDR_RBUTNMENU_NS);
	//	pSubMenu = menuLoad.GetSubMenu(TP_NSMENU_DROPMENU);	
	//	if(NULL == pSubMenu)
	//	{
	//		menuLoad.DestroyMenu();
	//		return S_DECLINE;
	//	}

	//	CPoint pt;
	//	GetCursorPos(&pt); 
	//	ScreenToClient(&pt);
	//	if(!m_rtClient.PtInRect(pt)) return S_DECLINE; // 如果不在客户区不处理
	//	TPListItemData *pItemData = NULL;
	//	int nItem = CTPListCtrlEx::HitTest(pt, 0);
	//	if(nItem >= 0)
	//		pItemData = (TPListItemData*)CTPListCtrlEx::GetItemData(nItem);

	//	HRESDATA hResData = NULL;
	//	if((NULL != pItemData) && (TP_RES_CATALOG & pItemData->eResType) && (NULL != pItemData->hResData))
	//		hResData = pItemData->hResData;
	//	if(NULL == hResData)	hResData = m_hCatalog;

	//	// Test Can be Cut or Copy
	//	BOOL bCut	= g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(hResData, paResData, TRUE) == S_ACCEPT;
	//	BOOL bCopy	= g_pClipManageInterface->stuFuncitonInterface.TP_PasteResDataTest(hResData, paResData, FALSE) == S_ACCEPT;
	//	BOOL bStoryBoard = TRUE;
	//	if(m_iArrType != ARRANGE_STORYBOARD||m_iViewType != VIEWTYPE_ICON) bStoryBoard = FALSE;

	//	if(m_bDragIn) // 窗口内移动
	//	{
	//		if(bStoryBoard){}
	//		else if(!bCut)    pSubMenu->EnableMenuItem(ID_DROPMENU_CUT,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//	}
	//	else
	//	{
	//		if(!bCut)   pSubMenu->EnableMenuItem(ID_DROPMENU_CUT,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//		if(!bCut && !bCopy) return S_OK; // Not Allow Copy and Cut
	//	} 

	//	if(!bCopy) pSubMenu->EnableMenuItem(ID_DROPMENU_COPY,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//	//if(!bCut)
	//	//{
	//	//	if(!bStoryBoard) 
	//	//		pSubMenu->EnableMenuItem(ID_DROPMENU_CUT,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//	//	else if(!m_bDragIn)
	//	//		pSubMenu->EnableMenuItem(ID_DROPMENU_CUT,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//	//}

	//	GetCursorPos(&pt); 
	//	TP_LoadMenuStr(pSubMenu);	
	//	UINT uCmd = pSubMenu ->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	//	if(uCmd > 0)
	//	{
	//		TP_RemoveResDataArray(m_aDropData, TRUE);
	//		for(INT i = 0; i < paResData->GetSize(); i++)
	//		{
	//			TPResData stuResData = paResData->GetAt(i);
	//			stuResData.hResData  = TP_DuplicateRes(stuResData.hResData, stuResData.eResType, TP_RESLOCK_VIEW|TP_RESLOCK_NOUPDATE);
	//			stuResData.lParam	 = NULL;
	//			stuResData.lEffParam = NULL;
	//			m_aDropData.Add(stuResData);
	//		}
	//	}

	//	menuLoad.DestroyMenu();
	//}	
	return S_OK;
}

void CTPNSResListCtrl::OnNcPaint()
{	
	CDC   *pDC        = GetWindowDC();	
	CTPDCExchange stuTemp(pDC);
	CRect rtWnd;
	GetWindowRect(&rtWnd);
	rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);	
	{
		CRect rtWnd,rtClient;
		GetWindowRect(&rtWnd);
		GetClientRect(&rtClient);
		ClientToScreen(&rtClient);
		rtClient.OffsetRect(-rtWnd.left,-rtWnd.top);
		rtWnd.OffsetRect(-rtWnd.left,-rtWnd.top);
		if(rtWnd.bottom - rtClient.bottom >2)
		{	
			pDC ->FillSolidRect(0,rtClient.bottom,rtWnd.Width(),rtWnd.bottom -rtClient.bottom,TP_GetSysColor(m_iBackColor));

		}
	}
	if(!m_bForList && m_bDrawFocus  && (GetFocus() == this || m_pFocusWnd && *m_pFocusWnd == this))
	{	
		pDC->Draw3dRect(rtWnd,RGB(255,255,255),RGB(255,255,255));			
	}
	else if(m_bForList)
	{	
		pDC->Draw3dRect(rtWnd,TP_GetSysColor(COLOR_BASE_3DRIGHT),TP_GetSysColor(COLOR_BASE_3DLEFT));						
	}		
	else
	{	
		pDC->Draw3dRect(rtWnd,TP_GetSysColor(m_iBackColor),TP_GetSysColor(m_iBackColor));						
	}		
	stuTemp.ReleaseDC();
	ReleaseDC(pDC);
}

BOOL CTPNSResListCtrl::OnKeyEnter(UINT uKey)
{
	if(uKey != VK_RETURN) return FALSE;

	return FALSE;
}

void  CTPNSResListCtrl::FunSortExItem(TPListCtrlItemArray &aItem, DWORD dwMode)
{
	for (int i=0;i<aItem.GetSize();i++)
	{
		for (int j=i+1;j<aItem.GetSize();j++)
		{
			TPListItemData *pItemData1 = NULL,*pItemData2 = NULL;
			pItemData1 = (TPListItemData*)aItem[i]->dwData;
			pItemData2 = (TPListItemData*)aItem[j]->dwData;
			if(_tmcmp(&pItemData1->tmCreateTime, &pItemData2->tmCreateTime)>0)
			{
				TPListCtrlItem * itemp = NULL;
				itemp    = aItem[i];
				aItem[i] = aItem[j];
				aItem[j] = itemp;
			}
		}
	}
}
BOOL CTPNSResListCtrl::PreviewGif(INT nIndex)
{
	//if(nIndex < 0)
	//{
	//	if(m_pGifProxyView)
	//	{
	//		delete m_pGifProxyView;			
	//		m_pGifProxyView = NULL;
	//		Invalidate();
	//	}
	//	m_hResProxyView = NULL;
	//	KillTimer(TP_ENENT_PREGIF);
	//	m_uListTimer[TP_LISTTIMER_PREVIEWGIF] = 0;
	//}
	//else
	//{
	//	if(m_iViewType != VIEWTYPE_ICON) return FALSE;
	//	TPListItemData* pItemData = (TPListItemData*)GetItemData(nIndex);
	//	if(NULL == pItemData) return FALSE;
	//	if(pItemData->hResData == m_hResProxyView && NULL != m_pGifProxyView) return TRUE;

	//	PreviewGif(-1);
	//	CString sFile = _T("");
	//	if(NULL == pItemData->hResData) return FALSE;
	//	if((pItemData ->eResType & (TP_RES_ALLVAEFFECT | TP_RES_CGEFFECT)))
	//	{
	//		LPTPEffectID hEffectID = (LPTPEffectID)pItemData ->hResData;
	//		LPTPEffectTable  pEffTable = g_pEffectManageInterface ->TPEffect_GetEffectInfoByID(hEffectID);

	//		//	if(!(pEffTable->dwEffectType & TPEFFECT_TYPE_TRANSITION_VIDEO)) return FALSE;
	//		sFile.Format(_T("%s\\%s\\%s.gif"), TP_GetSkinPath(),_T("ClipExplorer\\EffectGIF"), pEffTable->ptszGenerallEffectName);

	//	}
	//	else if(pItemData->eResType & (TP_RES_VTRTEMPLATE|TP_RES_VTEMPLATE))
	//	{
	//		if(!(pItemData->eNodeType & (TP_NODE_SYSTEMPLATETR|TP_NODE_SYSTEMPLATECLIP))) return FALSE;
	//		sFile.Format(_T("%s\\%s\\%s.gif"), TP_GetSkinPath(),_T("ClipExplorer\\TemplateGIF"), pItemData->sText[0]);
	//	}

	//	if(sFile.IsEmpty()) return FALSE;

	//	m_pGifProxyView = new CTPNSGifProxyView(sFile);
	//	CRect rtRect(0,0,0,0);
	//	GetItemRect(nIndex, rtRect, LVIR_ICON);
	//	m_pGifProxyView->Play(m_hWnd, rtRect);
	//	m_hResProxyView = pItemData->hResData;
	//}
	return TRUE;
}

void CTPNSResListCtrl::AddSubColorMenu(CTPMenuEx *pMenu,int iPosition,COLORREF cResColor)
{				
	//AddMeunColor(pMenu,iPosition,ID_RESLISTSELECT_COLORBASE);
	//CTPMenuEx *pMenuEx = pMenu ->GetSubMenu(iPosition);
	//if(pMenuEx->GetMenuItemCount()> 0)
	//{
	//	for(INT l= TP_CLIPCOLOR_BASE;l<TP_CLIPCOLOR_MAX;l++)
	//	{
	//		pMenuEx->ModifyMenu(ID_RESLISTSELECT_COLORBASE +(l- TP_CLIPCOLOR_BASE), MF_BYCOMMAND|MF_STRING, ID_RESLISTSELECT_COLORBASE +(l- TP_CLIPCOLOR_BASE), (LPCTSTR)TP_GetResColorName((TP_RES_COLOR)l));
	//		pMenuEx->SetMenuItemImage(l- TP_CLIPCOLOR_BASE,MF_BYPOSITION,(DWORD)TP_GetResColor((TP_RES_COLOR)l, FALSE, TRUE),cResColor == l,1);
	//	}
	//	int j = (int)pMenuEx->GetMenuItemCount();
	//	pMenuEx->InsertMenu(j, MF_BYPOSITION, ID_SEPARATOR);
	//	pMenuEx->AppendMenu(MF_BYCOMMAND|MF_STRING,ID_RESLISTSELECT_COLORBASE + 32,(LPCTSTR)TP_GetResColorName((TP_RES_COLOR)COLOR_BASE_RES_DEF));
	//	DWORD dwDefColor = (DWORD)TP_GetResColor((TP_RES_COLOR)COLOR_BASE_RES_DEF, FALSE, TRUE);
	//	pMenuEx->SetMenuItemImage(j + 1,MF_BYPOSITION,dwDefColor,cResColor == COLOR_BASE_RES_DEF,1);
	//}
}
void CTPNSResListCtrl::FunCloumnHide()
{
	m_bViewFile = FALSE;
	for(INT l=0;l<m_aColumn.GetSize();l++)
	{
		if(m_aColumn[l]->bShow && (m_aColumn[l]->iIndexInit == TP_RESTEXT_SOURCE_NAME || m_aColumn[l]->iIndexInit == TP_RESTEXT_MEDIA_FILE_LOCATION_VIDEO||m_aColumn[l]->iIndexInit == TP_RESTEXT_MEDIA_FILE_LOCATION_AUDIO))
		{
			m_bViewFile = TRUE;
			break;
		}
	}

 }
void CTPNSResListCtrl::FunAdjColumnWidth(int iColumnIndex)
{
	if(iColumnIndex<0 || m_aItem.IsEmpty()) return;
	CDC   *pDC   = GetDC();
	CFont *pFont = pDC->SelectObject(TP_GetSysFont(m_iFontIndex));

	int     nMaxWidth = 0;
	int     nLeft     = 0;
	int     nWidth    = 0;	
	CRect   rcSub     = CRect(0,0,0,0);
	CRect   rtTemp    = CRect(0,0,0,0);
	CString sText;
	INT nHeadIndex = TP_NodeTypeHeadIndex(m_eShowNodeType);
	for (int iItem=0; iItem<m_aItem.GetCount(); ++iItem)
	{		
		CTPListCtrlEx::GetItemResData(iItem);
		GetSubItemRect(iItem,iColumnIndex,LVIR_BOUNDS,rcSub);
		nLeft = rcSub.left;
		if (iColumnIndex == 0)
		{
			rtTemp = rcSub;
			rtTemp.DeflateRect(1,1);	
			rtTemp.right = rtTemp.left + rtTemp.Height() * TP_RESICON_WIDTH / TP_RESICON_HIGH; 	
			rtTemp.OffsetRect(2,0);
			rcSub.left = rtTemp.right + 5;
		}
		rcSub.DeflateRect(2,0);
		sText = GetItemText(iItem,iColumnIndex);
		pDC ->DrawText(sText,rcSub,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_CALCRECT);


		if((nHeadIndex == TP_HEADINDEX_COMPUTER && GetItemOrder(iColumnIndex) == TP_PROJECT_CLIPCOLOR) || (nHeadIndex == TP_HEADINDEX_BIN && GetItemOrder(iColumnIndex) == TP_RESTEXT_CLIPCOLOR) || nHeadIndex == TP_HEADINDEX_PROJECT && GetItemOrder(iColumnIndex) == TP_CATALOG_CLIPCOLOR)
			nWidth = 50; 
		else
			nWidth = rcSub.right - nLeft;
		if(VIEWTYPE_TEXTPICTURE == m_iViewType) nWidth += rtTemp.right - rtTemp.left; 
		if (nWidth > nMaxWidth)	nMaxWidth = nWidth + 10;		
	}
	if (nMaxWidth < 10) 	nMaxWidth = 10;	
	if (GetColumnWidth(iColumnIndex) != nMaxWidth)	SetColumnWidth(iColumnIndex, nMaxWidth);
	ChangeColume(0);
	pDC->SelectObject(pFont);
	ReleaseDC(pDC);
}
LRESULT CTPNSResListCtrl::OnMsgRelayout(WPARAM wp,LPARAM lp)
{
	if(m_bRelayout)
	{
		m_bRelayout = FALSE;
		POSITION pos = CTPListCtrlEx::GetFirstSelectedItemPosition();
		while(pos)
		{
			INT nItem = CTPListCtrlEx::GetNextSelectedItem(pos);
			if(!IsViewItem(nItem) && (GetItemState(nItem, LVIS_SELECTED))) SelectItem(nItem, FALSE, FALSE);
		}

		FunCalcSize();
		Invalidate();
	}
	return S_OK;
}
void CTPNSResListCtrl::FunCalcItemSize()
{
	PreviewGif(-1);
	CTPResListCtrl::FunCalcItemSize();
}
UINT SobeyMamClipboardFormat = RegisterClipboardFormat(_T("SobeyETClip"));
 LRESULT  CTPNSResListCtrl::OnMsgOleDrag(WPARAM wp,LPARAM lp)
{
	TPOleDropData *pOleData = (TPOleDropData *)lp;	
	BOOL           bHDrop   = pOleData ->CheckType(CF_HDROP,NULL),bAI3 = FALSE;

	if(g_pSystemInfo ->uSystemState & TP_SYSTEMSTATE_XDA)
	{
		if(!bHDrop)    bAI3     = pOleData ->CheckType(SobeyMamClipboardFormat,_T("<"));
	}

	if(pOleData == NULL || pOleData ->pDataObject == NULL || (!bHDrop && !bAI3))
	{
		if(pOleData) pOleData ->eRetrun = DROPEFFECT_NONE;
		return DROPEFFECT_NONE;		 
	}
	if(wp == 0 || wp == 1)
	{
		if(GetDropCatalog(pOleData ->point) == NULL) 
		{
			pOleData ->eRetrun = DROPEFFECT_NONE;		
			return DROPEFFECT_COPY;
		}
		else
		{
			pOleData ->eRetrun = DROPEFFECT_COPY;		
			return DROPEFFECT_COPY;
		}
	}
	else  if(wp == 2)
	{
		pOleData ->eRetrun = DROPEFFECT_COPY;	
		if(bHDrop)
		{		
			HGLOBAL hGlobal   = pOleData ->pDataObject->GetGlobalData(CF_HDROP,NULL);;
			LPBYTE lpData     = (LPBYTE)GlobalLock(hGlobal);	
			OnDropFiles((HDROP)lpData);
			GlobalUnlock(hGlobal);				
			return DROPEFFECT_COPY;
		}
		else if(bAI3)
		{			
			//暂时这样改一下吧，拖动与消息之间还是有时间差的
			m_aDropFile.RemoveAll();
			HGLOBAL hGlobal   = pOleData ->pDataObject->GetGlobalData(SobeyMamClipboardFormat,NULL);;
			LPBYTE lpData     = (LPBYTE)GlobalLock(hGlobal);	
			CString      sFileName; 
			sFileName   =  (LPCTSTR)(LPTSTR)lpData;
			m_aDropFile.Add(sFileName);	
			GlobalUnlock(hGlobal);			

			PostMessage(WM_LIST_DROPAI3, 0, 0);
			return DROPEFFECT_COPY;
		}
	}
	else  if(wp == 3)
	{
		pOleData ->eRetrun = DROPEFFECT_NONE;
		return DROPEFFECT_NONE;		
	}
	// 0 enter 1 over 2 drop 3  level
	return S_OK;		 
}
void CTPNSResListCtrl::OnCaptureChanged(CWnd *pWnd)
{ 
	//snp4100028512
	CTPResListCtrl::OnCaptureChanged(pWnd); 
}
void CTPNSResListCtrl::FilterRes(CString &sFilter)
{	
	m_sResFilter = sFilter;
	//BOOL bReDraw  = FALSE;
	//INT nBin     = 0;
	//INT nObject  = 0;
	//for(INT i = 0; i < GetItemCount(); i++)
	//{
	//	TPListItemData* pItemData = (TPListItemData*)GetItemData(i);
	//	if(NULL == pItemData) continue;
	//	if(NULL == pItemData->pItem) continue;

	//	GetItemResData(pItemData);

	//	pItemData->pItem->bShow = TRUE;
	//	CString sName = pItemData->sText[TP_RESTEXT_CLIP_NAME];
	//	BOOL bShow =  FileterResMothed(sFilter, sName);
	//	if(!bShow)	SelectItem(i, FALSE, FALSE);
	//	pItemData->pItem->bShow = bShow;
	//	if(pItemData->pItem->bShow)
	//	{
	//		if(pItemData->eResType & (TP_RES_CATALOG|TP_RES_PROJECT)) nBin++;
	//		else													  nObject++;
	//	}
	//}
	//if(m_bReDraw)
	//{
	//	m_iTopX = 0; m_iTopY = 0;// XN00029482 lihuili 重新设置滚动条
	//	FunCalcSize();
	//	Invalidate();
	//}

	//if(m_pResExplorer) // 更新显示			
	//{		
	//	TPCatalogData stuCatalog;
	//	g_pClipManageInterface->stuCatalogInterface.TP_GetCatalogInfo(m_hCatalog, &stuCatalog);
	//	if(m_pResExplorer->m_pStatusBar)
	//	{			
	//		if(stuCatalog.guidProject ==g_guidCurCatalog.guidRes)		m_pResExplorer->m_pStatusBar->SetPaneImage(0,0);
	//		else                                               m_pResExplorer->m_pStatusBar->SetPaneImage(0,1);
	//		CString sText,sTemp;
	//		if(nBin>0)   { sTemp.Format(_T("Bins %d "),nBin);sText += sTemp;}
	//		if(nBin+nObject>0){ sTemp.Format(_T("Objects %d "),nBin+nObject);sText += sTemp;}
	//		m_pResExplorer->m_pStatusBar->SetPaneText(0,sText);
	//	}

	//	m_pResExplorer ->SetToolBarState();
	//}

}
BOOL CTPNSResListCtrl::FileterResMothed(CString &sFilter, CString &sName)
{
	sFilter.MakeUpper();
	sName.MakeUpper();
	if(sName.Find(sFilter) >= 0) return TRUE;

	return FALSE;
}

void CTPNSResListCtrl::SaveResIconIndex()
{
//	TP_NODE_TYPE eNodeFilter = TP_NODE_TRASHSYS|TP_NODE_SEARCH;
//	if(m_eNodeType & eNodeFilter) return;
//	for(INT i = 0; i < CTPListCtrlEx::GetItemCount(); i++)
//	{
//		TPListItemData  *pItemData = (TPListItemData *)CTPListCtrlEx::GetItemData(i);
//		if(NULL == pItemData) continue;
//		if(TP_RES_NORESDATA & pItemData->eResType) continue;
//
//		if(pItemData ->lListIndex[0] == pItemData ->pItem->lIconIndex && 	pItemData ->lListIndex[1] == pItemData ->pItem->lReportIndex) continue;
//
//		if(m_eNodeType & TP_NODE_TRASH)
//		{
//			if(TP_GetSafeAdress(m_hCatalog, TP_SAFETYPE_RESCATALOG))				
//				g_pClipManageInterface ->stuFuncitonInterface.TP_SetResIndex(m_hCatalog,pItemData->guidRes,pItemData->guidNode, pItemData->pItem->lIconIndex,pItemData->pItem->lReportIndex);
//		}
//		else if(m_eNodeType & TP_NODE_USERINFO) // User 信息
//		{
//			TPUserDepartmentBaseInfo stuUseInfo;	
//			TPUserDepartmentState    stuState(g_pUserEnterInfo,TP_TPUSERD_NONETMSG);
//			if(g_pUserMangeInterface ->TP_NS_GetUserInfo(pItemData->guidRes,stuUseInfo)==S_OK)
//			{
//				stuUseInfo.lIconIndex   = pItemData->pItem->lIconIndex;
//				stuUseInfo.lReportIndex = pItemData->pItem->lReportIndex;
// 				g_pUserMangeInterface->TP_NS_SetUserInfo(pItemData->guidRes,stuUseInfo);
//			}
//		}
//		else // 素材
//		{
//				g_pClipManageInterface->stuCatalogInterface.TP_SetResIndex(pItemData->hResData,pItemData->pItem->lIconIndex,1);
//				g_pClipManageInterface->stuCatalogInterface.TP_SetResIndex(pItemData->hResData,pItemData->pItem->lReportIndex,2);
//		}
//	}
}

void CTPNSResListCtrl::SaveListState(BOOL bSave, INT64 eNodeShow)
{
	if(bSave)
	{
		if(m_eShowNodeType != 0)
		{
			// 关闭的时候存一下 排序箭头qushaojie
			if(TP_GetSafeAdress(m_hCatalog,TP_SAFETYPE_RESCATALOG)) 
				g_pClipManageInterface ->stuCatalogInterface.TP_SetCatalogSortType(m_hCatalog,0,m_bSortAsc,m_iSortIndex);

			// 保存 Colum List
			int    iHeadIndex = TP_NodeTypeHeadIndex(m_eShowNodeType);
			if(iHeadIndex >=0)
			{
				CRect rtItem;
				for(int l=0;l<GetColumnCount();l++)
				{									
					iColumnWidth[iHeadIndex][l] = GetColumnWidth(l);	
					iColumnView[iHeadIndex][l]  =m_aColumn[l]->bShow;
				}
				GetColumnOrderArray(iColumnOrder[iHeadIndex],iColumnMax[iHeadIndex]);
			}
			//
		}

	}
	else
	{
		//个窗口的显示方式互相不影响
		WORD lViewType = VIEWTYPE_MAX,lViewTypeGet =  VIEWTYPE_MAX,lIconSize = m_lIconSize;
		m_aCatalogViewType.Lookup(m_hCatalog,lViewType);
		lViewTypeGet = (WORD)g_pClipManageInterface ->stuCatalogInterface.TP_GetCatalogViewType(m_hCatalog,&m_iArrType,&m_lIconSize);
		if(m_lIconSize<1 || m_lIconSize>4) m_lIconSize = 4;
		if(m_iArrType <= ARRANGE_FREE) m_iArrType = ARRANGE_AUTO;

		if(lViewType == 0)	lViewType = lViewTypeGet;
		m_iViewType = lViewType;
		if(m_iViewType >=VIEWTYPE_MAX)  m_iViewType = VIEWTYPE_ICON;
		m_aCatalogViewType.SetAt(m_hCatalog,(WORD)m_iViewType);
		if(m_lIconSize != lIconSize)
		{
			int iIndxe = 0;
			if(m_lIconSize ==1)       iIndxe = TP_CLIPICON_N1;
			else if(m_lIconSize == 2) iIndxe = TP_CLIPICON_N2;	
			else if(m_lIconSize == 3) iIndxe = TP_CLIPICON_N3;
			else if(m_lIconSize == 4) iIndxe = TP_CLIPICON_N4;		
			m_szItemIcon = CSize(m_pCommonRes[iIndxe]->Width(),m_pCommonRes[iIndxe]->Height());							
		}
		int iSortIndex = -2;
		DWORD dwSortType =  g_pClipManageInterface ->stuCatalogInterface.TP_GetCatalogSortType(m_hCatalog,&iSortIndex);
		m_bSortAsc       =  (dwSortType & TP_BY_ASC) > 0 ? TRUE : FALSE;

		m_iTopIndex       = -1;
		if(eNodeShow != m_eShowNodeType)
		{
			m_eShowNodeType = eNodeShow;
			//重新设计表头
			INT iHeadIndex = TP_NodeTypeHeadIndex(m_eShowNodeType);

			if(iHeadIndex>=0)
			{
				while(CTPListCtrlEx::DeleteColumn(0));
				for(int l=0;l<iColumnMax[iHeadIndex];l++)
				{
					CTPListCtrlEx::InsertColumn(l,sTitle[iHeadIndex][l],LVCFMT_CENTER,iColumnWidth[iHeadIndex][l]);					
				}

				SetColumnOrderArray(iColumnOrder[iHeadIndex],iColumnMax[iHeadIndex]);
				// 因为iColumnView记录上次显示顺序的状态，所以需要在设置顺序之后设置
				for(int l=0;l<iColumnMax[iHeadIndex];l++)
				{
					m_aColumn[l]->bShow = iColumnView[iHeadIndex][l];
					m_aColumn[l]->iWidth = iColumnWidth[iHeadIndex][l];
				}
			}

			//changed by qsj 20100104 用户信息PE中修改XN00027491
			int iClipNameOrder = GetItemByOrder(TP_RESTEXT_CLIP_NAME);
			if(iClipNameOrder >= 0 ) SetColumeEditType(iClipNameOrder,TP_COLOUM_EDIT,TP_RESNAME_MAX);

			if(iHeadIndex == TP_HEADINDEX_CGTEMPLAGE)
			{
				int iCGCommentOrder  = GetItemByOrder(TP_TEMPALTECGTEXT_COMMENT);
				if(iCGCommentOrder >= 0) SetColumeEditType(iCGCommentOrder,TP_COLOUM_EDIT,TP_RESDESC_MAX);
			}
			else if(iHeadIndex == TP_HEADINDEX_USER)
			{
				int iUserCommentOrder = GetItemByOrder(TP_USEREXT_COMMENT); 
				if (iUserCommentOrder >=0) SetColumeEditType(iUserCommentOrder,TP_COLOUM_EDIT,TP_RESDESC_MAX);
			}
			else
			{	
				int iCommentOrder  = GetItemByOrder(TP_RESTEXT_COMMENT);
				if(TP_HEADINDEX_BIN == iHeadIndex && iCommentOrder >= 0) SetColumeEditType(iCommentOrder,TP_COLOUM_EDIT,TP_RESDESC_MAX);
			}
		}
		if(iSortIndex >= 0)
		{
			m_iSortIndex = iSortIndex;
			SetColumnFmt(iSortIndex,m_bSortAsc ? HDF_SORTUP : HDF_SORTDOWN ,(HDF_SORTDOWN | HDF_SORTUP));
		}
		else
		{
			SetColumnFmt(m_iSortIndex,0, (HDF_SORTUP | HDF_SORTDOWN));
			m_iSortIndex = -1;
		}
	}
}

LRESULT CTPNSResListCtrl::OnResRename(WPARAM wParam, LPARAM lParam)
{
	if(NULL == wParam) return S_FALSE;
	struct tempData
	{
		GUID guidRes;
		CString sName;
	};

	tempData* pData = (tempData*)wParam;
	ResRenameForOrder(pData->guidRes, pData->sName);

	return S_OK;
}
