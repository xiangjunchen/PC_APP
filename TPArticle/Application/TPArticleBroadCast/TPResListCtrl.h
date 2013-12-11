#pragma once
#define LIST_COLUMN_MAX 4

#ifdef TP_NS_EDIT
#define M1   10//qsj 20100104 change 8->9 加了用户管理
//{liubo add CD2009.10.01 2010-6-24 Material ID
#ifdef TP_NS_NET
#define M2   37
#else
#define M2   36
#endif
//}
#else
#define M1   10
#define M2   4
#endif

#define TP_ITEMFLAG_UNKNOW					0x0000000000
#define TP_ITEMFLAG_DEFAULTNOTE			    0x0000000001
#define TP_ITEMFLAG_AUDIO					0x0000000002
#define TP_ITEMFLAG_VIDEO					0x0000000004
#define TP_ITEMFLAG_MEDIAOFFLINE		    0x0000000008
#define TP_DRAG_IMAGLIST

#define TP_DRAG_ICON                        4

#define TP_CLIPCOLOR_BASE COLOR_BASE_RES_1
#define TP_CLIPCOLOR_MAX  (COLOR_BASE_RES_11+1)

enum TP_HEAD_INDEX
{
	TP_HEADINDEX_BIN = 0,
	TP_HEADINDEX_USER,//qsj 20100104第二位放用户以和控制数组对应，将undef放第八
	TP_HEADINDEX_EFFECT,
	TP_HEADINDEX_PROJECT,
	TP_HEADINDEX_TRASH,
	TP_HEADINDEX_CGTEMPLAGE,
	TP_HEADINDEX_CGPREFABRICATE,
	TP_HEADINDEX_COMPUTER,
	TP_HEADINDEX_NEWWORK,
	TP_HEADINDEX_UNDEF,
	TP_HEADINDEX_MAX,
};
enum TP_RESSTATE_INDEX
{
	TP_RESSTATE_PROJECTOPEN,
	TP_RESSTATE_PROJENOSHARE,
	TP_RESSTATE_PROJEHIDE,

	TP_RESSTATE_CLIPISNOTINUSE,  //素材没有被当前时间线使用 
	TP_RESSTATE_CLIPISINUSE,     //素材被当前时间线使用
};
typedef  struct tag_TPListItemData
{

	HRESDATA       hResData;			//  材句柄
	TP_RES_TYPE    eResType;            //  素材类型
	GUID           guidRes;				//  素材ID
	GUID           guidDBType;			//  数据库ID
	GUID           guidNode;            //  所在目录ID
	GUID           guidRecyleNode;      //  所在目录ID
	ULONGLONG      eNodeType;			//  目录类型
	DWORD          dwOperateFlag;       //  操作类型
	LPARAM         lParam;
	DWORD          dwState;
	//显示数据
	UINT           uResSource;			//  来源附加标志 U1
	DWORD          dwUseState;			//  使用状态标志
	int            iNPIndex;			//  图标索引
	long           lRefCount;			//  引用计数
	CString        VAString;	
	CString        sText[M2];			//  附加信息
	DWORD          dwBookIndex; 

	// 图标信息
	BOOL           bIs0403Icon;         //  是否为4：3的格式
	BOOL           bDelIcon;			
	LPBYTE         pIcon;				//  小图标
	CSize          szIcon;
	BOOL           bGetData;			// 
	BYTE           bGetIndex; 
	BOOL           bGetFile;	
	DWORD          dwCaptureFlag;
	LPBYTE         pBuffer;             //  大图标数据
	CSize          szBuffer;
	CString        sFileName;			//  图标文件名
	CString        sKeyFileName;		//  KYE文件名
	INT64          iFrame;				//  图标位置			
	BOOL           bGetBuffer;			

	TP_CLIP_SOURCE_TYPE eSingSourceType;//  素材来源
	TPClipGraphScale  *pGraphScaleEx; //  图片调整信息

	BOOL				bDropFrame;		//  丢帧标志
	COLORREF            cResColor;
	DWORD               dwVideoStand; 
	INT64               uResFlag;	
	INT64				lLength;
	INT64               lListItemFlag;
	SYSTEMTIME			tmCreateTime;	//  创建时间
	SYSTEMTIME          tmModifyTime;   //  修改时间
	SYSTEMTIME			tmRecTime;		//  拍摄时间
	long                lListIndex[2];

	TPListCtrlItem *pItem;
	tag_TPListItemData()
	{
		hResData				= NULL;
		eResType				= TP_RES_UNKNOW;
		guidRes					= GUID_NULL;		
		guidDBType				= GUID_NULL;
		guidNode				= GUID_NULL;
		guidRecyleNode			= GUID_NULL;
		eNodeType				= 0;
		dwOperateFlag			= 0;
		lParam					= NULL;
		dwState					= 0;

		lListItemFlag			= TP_ITEMFLAG_UNKNOW;
		uResSource				= 0;
		dwUseState				= 0;
		iNPIndex				= 0;
		lRefCount				= -1; 
		bDelIcon				= FALSE; 
		pIcon					 = NULL; 
		szIcon					= CSize(0,0);
		VAString				= _L("");	
		dwBookIndex				= 0; 
		bGetData				= FALSE;
		bGetIndex               = FALSE;  
		bGetFile                = FALSE;
		dwCaptureFlag           = TP_CLIPCAPTURE_OK;
        bIs0403Icon				= TRUE;
		pBuffer					= NULL; 
		szBuffer				= CSize(0,0);
		sFileName				= _L("");
		sKeyFileName			= _L("");
		iFrame					= -1;
		bGetBuffer				= FALSE;
		pItem					= NULL;
		bDropFrame              = FALSE;
		cResColor               = 0;
		eSingSourceType         = TP_CLIP_SOURCE_TYPE_UNKNOW;
		pGraphScaleEx			= NULL;
		uResFlag				= 0;
		ZeroMemory(&tmCreateTime, sizeof(tmCreateTime));
		ZeroMemory(&tmModifyTime, sizeof(tmModifyTime));
		ZeroMemory(&tmRecTime, sizeof(tmRecTime));
		lListIndex[0]          = -1;
		lListIndex[1]          = -1;
	}
	void SetResBaseData(TPResData &hSetResData)
	{
		hResData      = hSetResData.hResData;
		eResType      = hSetResData.eResType;
		guidRes       = hSetResData.guidRes;
		guidDBType    = hSetResData.guidDBType;		
		guidNode      = hSetResData.guidNode;
		eNodeType     = hSetResData.eNodeType;
		dwOperateFlag = hSetResData.dwOperateFlag;
		lParam        = hSetResData.lParam;		
#ifdef TP_NS_NET
		bDeleteProtection = hSetResData.bDeleteProtection;
#endif
	}
	void GetResBaseData(TPResData &hGetResData)
	{
		hGetResData.hResData       = hResData;
		hGetResData.eResType       = eResType;
		hGetResData.guidRes        = guidRes;
		hGetResData.guidDBType     = guidDBType;		
		hGetResData.guidNode       = guidNode;
		hGetResData.dwOperateFlag  = dwOperateFlag;
		hGetResData.eNodeType      = eNodeType;
		hGetResData.lParam         = lParam;
#ifdef TP_NS_NET
		hGetResData.bDeleteProtection = bDeleteProtection;
#endif
	}
	void ReleaseIconBuf()
	{
		if(bDelIcon && pIcon)
		{
			delete []pIcon;
			bDelIcon = FALSE;
		}
		pIcon  = NULL;
		szIcon = CSize(0, 0);
	}

	LPBYTE ResizeBuffer(CSize &szSizeNew)
	{
		if(pBuffer && szBuffer.cx * szBuffer.cy < szSizeNew.cx * szSizeNew.cy)
		{
			delete pBuffer;
			pBuffer = NULL;
			szBuffer = CSize(0, 0);
		}

		if(NULL == pBuffer)
		{
			szBuffer = szSizeNew;
			pBuffer  = new BYTE[szBuffer.cy * szBuffer.cx * 4];
		}
		return pBuffer;
	}
	~tag_TPListItemData()
	{
		bGetBuffer = FALSE;
		if(bDelIcon &&pIcon && pIcon != pBuffer) delete pIcon;		
		if(pBuffer)
		{
			delete pBuffer;
			pBuffer = NULL;
		}
		szBuffer = CSize(0,0);
		pIcon    = NULL;
	}

}TPListItemData;
typedef CArray<TPListItemData *,TPListItemData *&> TPListItemDataArray;
// CTPResListCtrl
#define  WM_LIST_STATECHANGE WM_USER + 111
#define  WM_LIST_DROPFILE	 WM_USER + 113
#define  WM_LIST_DROPAI3	 WM_USER + 114
#define  WM_LIST_DBCLICK	 WM_USER + 117


#define  ID_FILEEDIT_BASE    ID_AEPLUGIN_CMDMIN
#define  ID_FILEEDIT_MAX	 ID_AEPLUGIN_CMDMAX

#define TP_LISTSTATE_NODROP			0x000000001


enum
{
	TP_LISTTIMER_DROPPING = 0,
	TP_LISTTIMER_PREVIEWGIF,
	TP_LISTTIMER_MAX,
};
class CTPResTreeCtrl;
class CTPNSResBrowserCtrl;
class CTPNSResExplorerPanel;
class CTPResListCtrl : public CTPListCtrlEx
{
	DECLARE_DYNAMIC(CTPResListCtrl)

public:
	CTPResListCtrl();
	virtual ~CTPResListCtrl();
public:
	CTPNSResExplorerPanel *m_pResExplorer;

	HRESDATA                m_hCacheData;
	TPUserEnterInfo        *m_pUserEnterInfo;      //用户信息 
	CTPResTreeCtrl        *m_pResTreeCtrl;
	CTPNSResBrowserCtrl    *m_pResBrowserCtrl;
	HRESDATA                m_hCatalog;
	GUID                    m_guidCatalog;
	GUID                    m_guidDBType;
	HRESDATA                m_hSaveData;
	TP_NODE_TYPE            m_eNodeType;

	BOOL                 m_bSortAsc;
	TP_NODE_TYPE         m_eShowNodeType;
	int                  m_iSortIndex;
	int					 m_iSortIndexEx;	
	BOOL                 m_bForList;
	DWORD                m_dwSortType;
	CWnd*				 m_pMsgWnd;

	CString              m_sEditLabel;
	GUID				 m_guidEditRes;
	CMapPtrToWord        m_aCatalogViewType;	
	BOOL                 m_bCanEdit;
	int                  m_iTopIndex;
	int                  m_iMenuItem;

	TP_RES_TYPE          m_eDragResType;
	TPResDataArray       m_hDragData;
	CStringArray         m_aDropFile;
	HRESDATA             m_hDropCatalog; 
	CGuidArray           m_aGuidReturn;
	BOOL                 m_bDragIn;//初始值为False，窗内拖动TRue，窗外拖False
	INT64				 m_uListState;
	DWORD				 m_dwWndType;
	CPoint				 m_ptMouseMove;
	BOOL                 m_bTrackLeave;
protected:

	HICON                m_hDragIcon[3];  
	TPCommonRes         *m_hDragCursor[3];

	HICON                m_hDragTLIcon[4];  
	TPCommonRes         *m_hDragTLCursor[4];
	UINT				 m_uListTimer[TP_LISTTIMER_MAX];
	TPResDataArray       m_aItemIndexSave;//存放右键copy到指定位置时的元素的index信息
	
public:
	virtual void       SetResData(TPResData& stuShow, TPResDataArray &hChildRes);
	virtual void       UpdateView(GUID guidDBType,TP_RES_TYPE eResUdataType ,TP_NODE_TYPE dwType,CGuidArray *hResDataList);
	virtual void       GetItemResData(TPListItemData *pData,BOOL bGetData = TRUE);
	virtual void       GetItemResClipFile(TPListItemData *pData){}
	virtual CString    GetItemText(int nItem, int nSubItem) const;
	virtual void       AddRes(GUID guidDBType,GUID guidCatalog,HRESDATA hResData,TP_RES_TYPE eResType,GUID guidRes);
	virtual void       ClearSortFlag(INT iItem);
	void               OnMenuCmd(UINT nID,  LPARAM lParam = 0);
	void               ReleaseDragImage();//add by qushaojie


	LRESULT            Dropping(ULONGLONG dwFrom,TP_RES_TYPE dwDropType,LPARAM lParam);
	virtual LRESULT    Drop(ULONGLONG dwFrom,TP_RES_TYPE dwDropType,LPARAM lParam);

	void               DeleteRes(GUID guidDBType,HRESDATA hResData,TP_RES_TYPE eResType,GUID guidRes);
	BOOL               OnKeyDown(UINT vKey);
	void               SelectItemGuid(CGuidArray &aItem);	  

	LRESULT            DropEffList(HRESDATA hCatalogData,DWORD dwFrom,TP_RES_TYPE dwDropType,LPARAM lParam,BOOL bTest);
	LRESULT			   ResetDroppingItem();

	void               GetSelectRes(GUID &guidMenu,CGuidArray &aSelect);
	void               SetSelectRes(GUID  guidMenu,CGuidArray &aSelect, BOOL bEnsureVisible);
	BOOL			   ResRenameForOrder(GUID guidRes, CString sNewName);
	virtual BOOL	   PreviewGif(INT nIndex);
	//{2010-8-25:CD2009.10.01:chenzhi:add
	#ifdef TP_NS_NET
	LRESULT			   OnNewSequence(WPARAM wParam,LPARAM lParam);	
	LRESULT			   OnOpenSequence(WPARAM wParam,LPARAM lParam);	
	#endif //TP_NS_NET
	//}
	

protected:
	virtual void    SubWindow(BOOL bSub);
	virtual void    BeginEdit();

	void            SortItem(int iIndex,BOOL bAsc);
	BOOL            CreateDragCursor(CPoint point);
	BOOL            ExpandItem(int item);

	LRESULT         OnMsgUpdateView(WPARAM wParam ,LPARAM lParam);
	LRESULT         OnMsgDropFile(WPARAM wParam ,LPARAM lParam);

	BOOL            OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );

	HRESDATA        GetDropCatalog(CPoint &pPoint);

	void            AddMeunColor(CTPMenuEx *pMenu,int iPosition,UINT uBaseID);

	INT				IsResExist(GUID guidRes);
	LRESULT			CopyResTo(HRESDATA hCatalog, TPResDataArray& aResData,CPoint ptPoint);

	virtual void    GetItemResData(int iItem);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHdnItemclick(UINT id,NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HRESULT OnMouseLeave(WPARAM wp, LPARAM lp);
};


