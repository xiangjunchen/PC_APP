#pragma once
#include "TPResListCtrl.h"

#define TP_EVENT_STATUSBAR		1000
#define TP_EVENT_FLASH			1000 + 1
#define TP_EVENT_SETVAOUT		1000 + 2
#define TP_EVENT_SELRES			1000 + 3
#define TP_ENENT_PREGIF			1000 + 4

#define TP_FLASHQUALITY_HIGH	0
#define TP_FLASHQUALITY_LOW		1
#define TP_FLASHQUALITY_BOTH	2
#define TP_FLASHQUALITY_NONE	3


#define TP_FLASHQUALITY_LOCK	_T("_FlashClipIcon_")

//#define WM_LIST_RESRENANME			WM_USER+1300
#define WM_LIST_RELAYOUT			WM_USER+1301

typedef struct tag_FlashResData
{
	GUID guidRes;
	DWORD dwQualityType;//0 high only, 1 low only ,2 high and low
	tag_FlashResData()
	{
		guidRes       = GUID_NULL;
		dwQualityType = 0;
	}
}TPFlashResData;
typedef CArray<TPFlashResData, TPFlashResData&> TPFlashResDataArray;

enum TP_RESTEXT_INDEX
{
	TP_RESTEXT_CLIP_NAME = 0,
	//{liubo add CD2009.10.01 2010-6-24 Material ID
#ifdef TP_NS_NET
	TP_RESTEXT_SONAPS_MATERIALID,
#endif
	//}
	TP_RESTEXT_CLIP_TYPE,	
	TP_RESTEXT_REEL_NAME,	
	TP_RESTEXT_DIGITIZE, 
	TP_RESTEXT_CLIPCOLOR,
	TP_RESTEXT_USED,		   
	TP_RESTEXT_CODEC,	    
	TP_RESTEXT_MARK_IN,
	TP_RESTEXT_MARK_OUT,	
	TP_RESTEXT_DURATION,		
	TP_RESTEXT_CLIP_LENGTH, 
	TP_RESTEXT_FTC,
	TP_RESTEXT_START,		
	TP_RESTEXT_END,	
	TP_RESTEXT_LTCIN,
	TP_RESTEXT_LTCOUT,
	TP_RESTEXT_VITCIN,
	TP_RESTEXT_VITCOUT,
	TP_RESTEXT_VTRINFO,
	TP_RESTEXT_SCENE_TAKE,
	TP_RESTEXT_STATUS,
	TP_RESTEXT_TRACK,	
	TP_RESTEXT_COMMENT,	
	TP_RESTEXT_RECDATE,
	TP_RESTEXT_CREATE_DATE,	
	TP_RESTEXT_UPDATE_DATE,	
	TP_RESTEXT_SUB_CLIPS,   
	TP_RESTEXT_PROXY,		
	TP_RESTEXT_CLIP_ID,
	TP_RESTEXT_RIGHT,		
	TP_RESTEXT_SOURCE_NAME,	
	TP_RESTEXT_MEDIA_FILE_LOCATION_VIDEO,		
	TP_RESTEXT_MEDIA_FILE_LOCATION_AUDIO,	
	TP_RESTEXT_FRAME_SIZE,
	TP_RESTEXT_FRAME_ASPECT_RATIO,
	TP_RESTEXT_NULLDATA,
	TP_RESTEXT_MAX,
};

enum TP_TEMPALTETEXT_INDEX
{
	TP_TEMPALTETEXT_NAME,        
	TP_TEMPALTETEXT_TYPE,         
	TP_TEMPALTETEXT_CATEGORY,     
	TP_TEMPALTETEXT_ORIGINAL,     
	TP_TEMPALTETEXT_DURATION,    
	TP_TEMPALTETEXT_ALIGNMENT,
};
//add by qsj 20100104 增加用户信息
enum TP_USEREXT_INDEX
{
	TP_USEREXT_NAME,
	TP_USEREXT_COMMENT,
};

enum TP_CATALOG_INDEX
{
	TP_CATALOG_NAME,
	TP_CATALOG_CLIPCOLOR,
};

enum TP_TEMPALTECGTEXT_INDEX
{
	TP_TEMPALTECGTEXT_NAME,        
	TP_TEMPALTECGTEXT_COMMENT,         
	TP_TEMPALTECGTEXT_CONFIGURATIONTIME,     
	TP_TEMPALTECGTEXT_ALTERATIONTIME,     
	TP_TEMPALTECGTEXT_DURATION,    
};
enum TP_PROJECTHEAD_INDEX
{
	TP_PROJECT_NAME = 0,        
	TP_PROJECT_MODIFYTIME,         
	TP_PROJECT_VIDEOFORMAT,     
	TP_PROJECT_ASPECTRATION,     
	TP_PROJECT_STATUS,  
	TP_PROJECT_CLIPCOLOR,
};	
enum TP_PROJECTTEXT_INDEX
{
	TP_PROJECTTEXT_NAME,        
	TP_PROJECTTEXT_MODIFYTIME,         
	TP_PROJECTTEXT_VIDEOFORMAT,     
	TP_PROJECTTEXT_ASPECTRATION,     
	TP_PROJECTTEXT_STATUS,  
	TP_PROJECTTEXT_CLIPCOLOR,	
};	

enum TP_PROJECTTEXTEX_INDEX
{
	TP_PROJECTTEXTEX_NAME	      = TP_RESTEXT_CLIP_NAME,        
	TP_PROJECTTEXTEX_MODIFYTIME   = TP_RESTEXT_UPDATE_DATE,         
	TP_PROJECTTEXTEX_VIDEOFORMAT  = TP_RESTEXT_CODEC,     
	TP_PROJECTTEXTEX_ASPECTRATION = TP_RESTEXT_FRAME_ASPECT_RATIO,     
	TP_PROJECTTEXTEX_STATUS		  = TP_RESTEXT_STATUS,  
	TP_PROJECTTEXTEX_CLIPCOLOR	  = TP_RESTEXT_CLIPCOLOR,
};	

enum TP_TRASHTEXT_INDEX
{
	TP_TRASH_NAME = 0,        
	TP_TRASH_CREATEUSER,         
	TP_TRASH_DELETEDATA,     
	TP_TRASH_DELETEUSER,     
};
enum TP_CLIPICON_INDEX
{
	TP_CLIPICON_N1,
	TP_CLIPICON_S1,
	TP_CLIPICON_C1,
	TP_CLIPICON_D1,
	TP_CLIPICON_N2,
	TP_CLIPICON_S2,
	TP_CLIPICON_C2,
	TP_CLIPICON_D2,
	TP_CLIPICON_N3,
	TP_CLIPICON_S3,
	TP_CLIPICON_C3,
	TP_CLIPICON_D3,
	TP_CLIPICON_N4,
	TP_CLIPICON_S4,
	TP_CLIPICON_C4,
	TP_CLIPICON_D4,

	TP_CLIPICON1_N1,
	TP_CLIPICON1_S1,
	TP_CLIPICON1_C1,
	TP_CLIPICON1_D1,
	TP_CLIPICON1_N2,
	TP_CLIPICON1_S2,
	TP_CLIPICON1_C2,
	TP_CLIPICON1_D2,
	TP_CLIPICON1_N3,
	TP_CLIPICON1_S3,
	TP_CLIPICON1_C3,
	TP_CLIPICON1_D3,
	TP_CLIPICON1_N4,
	TP_CLIPICON1_S4,
	TP_CLIPICON1_C4,
	TP_CLIPICON1_D4,


	TP_CLIPICON2_N1,
	TP_CLIPICON2_S1,
	TP_CLIPICON2_C1,
	TP_CLIPICON2_D1,
	TP_CLIPICON2_N2,
	TP_CLIPICON2_S2,
	TP_CLIPICON2_C2,
	TP_CLIPICON2_D2,
	TP_CLIPICON2_N3,
	TP_CLIPICON2_S3,
	TP_CLIPICON2_C3,
	TP_CLIPICON2_D3,
	TP_CLIPICON2_N4,
	TP_CLIPICON2_S4,
	TP_CLIPICON2_C4,
	TP_CLIPICON2_D4,

	TP_CLIPICON3_N1,
	TP_CLIPICON3_S1,
	TP_CLIPICON3_C1,
	TP_CLIPICON3_D1,
	TP_CLIPICON3_N2,
	TP_CLIPICON3_S2,
	TP_CLIPICON3_C2,
	TP_CLIPICON3_D2,
	TP_CLIPICON3_N3,
	TP_CLIPICON3_S3,
	TP_CLIPICON3_C3,
	TP_CLIPICON3_D3,
	TP_CLIPICON3_N4,
	TP_CLIPICON3_S4,
	TP_CLIPICON3_C4,
	TP_CLIPICON3_D4,

	TP_CLIPICON4_N1,
	TP_CLIPICON4_S1,
	TP_CLIPICON4_C1,
	TP_CLIPICON4_D1,
	TP_CLIPICON4_N2,
	TP_CLIPICON4_S2,
	TP_CLIPICON4_C2,
	TP_CLIPICON4_D2,
	TP_CLIPICON4_N3,
	TP_CLIPICON4_S3,
	TP_CLIPICON4_C3,
	TP_CLIPICON4_D3,
	TP_CLIPICON4_N4,
	TP_CLIPICON4_S4,
	TP_CLIPICON4_C4,
	TP_CLIPICON4_D4,

	TP_CLIPICON5_N1,
	TP_CLIPICON5_S1,
	TP_CLIPICON5_C1,
	TP_CLIPICON5_D1,
	TP_CLIPICON5_N2,
	TP_CLIPICON5_S2,
	TP_CLIPICON5_C2,
	TP_CLIPICON5_D2,
	TP_CLIPICON5_N3,
	TP_CLIPICON5_S3,
	TP_CLIPICON5_C3,
	TP_CLIPICON5_D3,
	TP_CLIPICON5_N4,
	TP_CLIPICON5_S4,
	TP_CLIPICON5_C4,
	TP_CLIPICON5_D4,

	TP_CLIPICON6_N1,
	TP_CLIPICON6_S1,
	TP_CLIPICON6_C1,
	TP_CLIPICON6_D1,
	TP_CLIPICON6_N2,
	TP_CLIPICON6_S2,
	TP_CLIPICON6_C2,
	TP_CLIPICON6_D2,
	TP_CLIPICON6_N3,
	TP_CLIPICON6_S3,
	TP_CLIPICON6_C3,
	TP_CLIPICON6_D3,
	TP_CLIPICON6_N4,
	TP_CLIPICON6_S4,
	TP_CLIPICON6_C4,
	TP_CLIPICON6_D4,

	TP_CLIPICON7_N1,
	TP_CLIPICON7_S1,
	TP_CLIPICON7_C1,
	TP_CLIPICON7_D1,
	TP_CLIPICON7_N2,
	TP_CLIPICON7_S2,
	TP_CLIPICON7_C2,
	TP_CLIPICON7_D2,
	TP_CLIPICON7_N3,
	TP_CLIPICON7_S3,
	TP_CLIPICON7_C3,
	TP_CLIPICON7_D3,
	TP_CLIPICON7_N4,
	TP_CLIPICON7_S4,
	TP_CLIPICON7_C4,
	TP_CLIPICON7_D4,

	TP_CLIPICON8_N1,
	TP_CLIPICON8_S1,
	TP_CLIPICON8_C1,
	TP_CLIPICON8_D1,
	TP_CLIPICON8_N2,
	TP_CLIPICON8_S2,
	TP_CLIPICON8_C2,
	TP_CLIPICON8_D2,
	TP_CLIPICON8_N3,
	TP_CLIPICON8_S3,
	TP_CLIPICON8_C3,
	TP_CLIPICON8_D3,
	TP_CLIPICON8_N4,
	TP_CLIPICON8_S4,
	TP_CLIPICON8_C4,
	TP_CLIPICON8_D4,

	TP_CLIPICON9_N1,
	TP_CLIPICON9_S1,
	TP_CLIPICON9_C1,
	TP_CLIPICON9_D1,
	TP_CLIPICON9_N2,
	TP_CLIPICON9_S2,
	TP_CLIPICON9_C2,
	TP_CLIPICON9_D2,
	TP_CLIPICON9_N3,
	TP_CLIPICON9_S3,
	TP_CLIPICON9_C3,
	TP_CLIPICON9_D3,
	TP_CLIPICON9_N4,
	TP_CLIPICON9_S4,
	TP_CLIPICON9_C4,
	TP_CLIPICON9_D4,

	TP_CLIPICON10_N1,
	TP_CLIPICON10_S1,
	TP_CLIPICON10_C1,
	TP_CLIPICON10_D1,
	TP_CLIPICON10_N2,
	TP_CLIPICON10_S2,
	TP_CLIPICON10_C2,
	TP_CLIPICON10_D2,
	TP_CLIPICON10_N3,
	TP_CLIPICON10_S3,
	TP_CLIPICON10_C3,
	TP_CLIPICON10_D3,
	TP_CLIPICON10_N4,
	TP_CLIPICON10_S4,
	TP_CLIPICON10_C4,
	TP_CLIPICON10_D4,

	TP_CLIPICON11_N1,
	TP_CLIPICON11_S1,
	TP_CLIPICON11_C1,
	TP_CLIPICON11_D1,
	TP_CLIPICON11_N2,
	TP_CLIPICON11_S2,
	TP_CLIPICON11_C2,
	TP_CLIPICON11_D2,
	TP_CLIPICON11_N3,
	TP_CLIPICON11_S3,
	TP_CLIPICON11_C3,
	TP_CLIPICON11_D3,
	TP_CLIPICON11_N4,
	TP_CLIPICON11_S4,
	TP_CLIPICON11_C4,
	TP_CLIPICON11_D4,


	TP_CLIPICON_STATUSINDICATORS,
	TP_CLIPICON_TYPEINDICATORS,
	TP_CLIPICON_STATUSTEAM,
	TP_CLIPICON_OKNG,
	TP_CLIPICON_FLODER,
	TP_CLIPICON_OFFLINE,
	TP_CLIPICON_AUDIO,
	TP_CLIPICON_TYPEINDICATORSLARGE,
	TP_CLIPICON_OFFLINELARGE,
	TP_CLIPICON_DELPROTECT,
	TP_CLIPICON_ICONOVERLAY,
	TP_CLIPICON_DefaultNote,
	TP_CLIPICON_MAX,
};
class CTPNSGifProxyView;
class CTPNSResExplorerPanel;







#define TP_GRADEMENU_SEARCH				0x00000000000001
#define TP_GRADEMENU_NEWBIN				0x00000000000002
#define TP_GRADEMENU_NEWSEQUENCE		0x00000000000004
#define TP_GRADEMENU_LOGGING			0x00000000000008
#define TP_GRADEMENU_IMPORTMXF			0x00000000000010
#define TP_GRADEMENU_EXPORTXDCAM		0x00000000000020
#define TP_GRADEMENU_IMPORTFX			0x00000000000040
#define TP_GRADEMENU_RECORDTOVTR		0x00000000000080
#define TP_GRADEMENU_RECORDTODVD		0x00000000000100
#define TP_GRADEMENU_CUT				0x00000000000200
#define TP_GRADEMENU_COPY				0x00000000000400
#define TP_GRADEMENU_PASTE				0x00000000000800
#define TP_GRADEMENU_DELETE				0x00000000001000
#define TP_GRADEMENU_REFRESH			0x00000000002000
#define TP_GRADEMENU_CLEARSYSTRASH		0x00000000004000 
#define TP_GRADEMENU_NEWPROJECT			0x00000000008000 
class CTPNSResListCtrl :public CTPResListCtrl
{
public:
	CTPNSResListCtrl(void);
	~CTPNSResListCtrl(void);

public:

	static  TPCommonRes  *m_pCommonRes[TP_CLIPICON_MAX];

	INT64                 m_uResFilter;
	CString               m_sResFilter;
private:
	static  int           m_iRefCount;
	int                   m_iCreateCatalogType;
	UINT_PTR              m_hSelTimer;
	UINT_PTR			  m_uFlashTimer;
	CPoint                m_ptRButtonDown;
	CPoint                m_ptRButtonUp;
	BOOL				  m_bFlashIcon;

	TPResDataArray        m_aDropData;
	CTPNSGifProxyView	 *m_pGifProxyView;
	HRESDATA			  m_hResProxyView;
	BOOL                  m_bRelayout;
	GUID				  m_guidProjectRev;
	DWORD				  m_dwTimeCodeType;
	BOOL                  m_bViewFile;


public:
	CTPOleDropTarget     *m_pDropTarget;
	
public:
	void              OnCmdMsg(UINT uID);
	void              EditItem(GUID guidItem);
	void              FlashTheQualityInIcon(int iSize);
	void			  SelectItemGuid(CGuidArray &aItem, BOOL bUnSelAll = TRUE);
	virtual void      UpdateView(GUID guidDBType,TP_RES_TYPE eResUdataType ,TP_NODE_TYPE dwType,CGuidArray *hResDataList);
	virtual void      SetResData(TPResData& stuShow, TPResDataArray &hChildRes);//qsj 20090727
	void              DelResData(TPResDataArray &hChildRes);
	void              SetSortMenu(CTPMenuEx *pSubMenu,CTPMenuEx *pSrCTPMenuEx,int iMode = 0);
	INT64			  HitTestToolBarState();

	BOOL              OnKeyEnter(UINT uKey);
	void              AddSubColorMenu(CTPMenuEx *pMenu,int iPosition,COLORREF cResColor);
	void			  FunAdjColumnWidth(int iColumnIndex);
	void			  FilterRes(CString &sFilter);

protected:
	virtual void      GetItemResIndex(TPListItemData *pData);
	virtual void      GetItemResData(TPListItemData *pData,BOOL bGetData = TRUE);
	virtual void      GetItemResClipFile(TPListItemData *pData );

	virtual void      DrawIcon(CDC *pDC,int iItem);
	void              DrawIconOverlay(CDC *pDC,int iItem, CRect rtIcon);
	virtual void      DrawTextPicture(CDC *pDC,int iItem);
	virtual void      DrawReport(CDC *pDC,int iItem);
	int               GetFloderImageIndex(GUID guidCatalog,ULONGLONG dwNodeType);
	void              OnCmdMsgClip(UINT uID);
	void              OnCmdMsgFx(UINT uID);

	void              OnRButtonDownClip(UINT nFlags, CPoint point);
	void              OnRButtonDownUnknow(UINT nFlags, CPoint point);

	virtual void      SetSortByMenu(CTPMenuEx *pSubMenu,int iIndex);
	virtual void      FunSortExItem(TPListCtrlItemArray &aItem, DWORD dwMode);
	virtual void      FunCalcItemSize();
	virtual void      FunCloumnHide();

	virtual LRESULT   Drop(ULONGLONG dwFrom,TP_RES_TYPE dwDropType,LPARAM lParam);
	virtual BOOL	  PreviewGif(INT nIndex);
	BOOL			  FileterResMothed(CString &sFilter, CString &sName);
	void			  SaveResIconIndex();
	void              GetItemResDataInfo(TPListItemData *pItemData,TP_RES_TYPE eResType,void *pResData,VIDEO_TYPE eVideoType);
public:
	void			  SaveListState(BOOL bSave, INT64 eNodeShow);	

public:///OLE 拖动专用
	LRESULT            OnMsgOleDrag(WPARAM wp,LPARAM lp);
	LRESULT            OnMsgRelayout(WPARAM wp,LPARAM lp);

#ifdef TP_NS_NET
	void               DragThread();
	LRESULT            OnMsgThreadDrag(WPARAM wp,LPARAM lp);
#endif

protected:	
	virtual void  ChangeColume(int iType);
public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnResRename(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNcPaint();
	afx_msg void OnCaptureChanged(CWnd *pWnd);	
};
