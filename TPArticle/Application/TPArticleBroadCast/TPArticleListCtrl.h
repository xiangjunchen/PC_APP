#pragma once


// CTPArticleListCtrl
typedef  struct tag_TPListItemData
{

	HRESDATA       hResData;			//  材句柄
	TP_RES_TYPE    eResType;            //  素材类型
	GUID           guidRes;				//  素材ID
	GUID           guidDBType;			//  数据库ID
	GUID           guidNode;            //  所在目录ID
	GUID           guidRecyleNode;      //  所在目录ID
//	TP_NODE_TYPE   eNodeType;			//  目录类型
	DWORD          dwOperateFlag;       //  操作类型
	LPARAM         lParam;
	DWORD          dwState;
	//显示数据
	UINT           uResSource;			//  来源附加标志 U1
	DWORD          dwUseState;			//  使用状态标志
	int            iNPIndex;			//  图标索引
	long           lRefCount;			//  引用计数
	CString        VAString;	
//	CString        sText[M2];			//  附加信息
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

	BOOL				bDropFrame;		//  丢帧标志
	COLORREF            cResColor;
	TP_VIDEO_STANDARD   eVideoStand; 
	INT64               uResFlag;	
	INT64				lLength;
	INT64               lListItemFlag;
	SYSTEMTIME			tmCreateTime;	//  创建时间
	SYSTEMTIME          tmModifyTime;   //  修改时间
	SYSTEMTIME			tmRecTime;		//  拍摄时间
	long                lListIndex[2];
	CRect               rtIcon;

	TPListCtrlItem *pItem;
	tag_TPListItemData()
	{
		hResData				= NULL;
		eResType				= TP_RES_UNKNOW;
		guidRes					= GUID_NULL;		
		guidDBType				= GUID_NULL;
		guidNode				= GUID_NULL;
		guidRecyleNode			= GUID_NULL;
// 		eNodeType				= 0;
		dwOperateFlag			= 0;
		lParam					= NULL;
		dwState					= 0;

// 		lListItemFlag			= TP_ITEMFLAG_UNKNOW;
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
		bIs0403Icon				= TRUE;
		pBuffer					= NULL; 
		szBuffer				= CSize(0,0);
		sFileName				= _L("");
		sKeyFileName			= _L("");
		iFrame					= 0;
		lLength                 = 0;
		bGetBuffer				= FALSE;
		pItem					= NULL;
		bDropFrame              = FALSE;
		cResColor               = 0;
		eVideoStand				= TP_VIDEOSTANDARD_UNKNOW;
		uResFlag				= 0;
		ZeroMemory(&tmCreateTime, sizeof(tmCreateTime));
		ZeroMemory(&tmModifyTime, sizeof(tmModifyTime));
		ZeroMemory(&tmRecTime, sizeof(tmRecTime));
		lListIndex[0]          = -1;
		lListIndex[1]          = -1;
		rtIcon                 = CRect(0,0,0,0);

	}
	void SetResBaseData(TPResData &hSetResData)
	{
		hResData      = hSetResData.hResData;
		eResType      = hSetResData.eResType;
		guidRes       = hSetResData.guidRes;
		guidDBType    = hSetResData.guidDBType;		
		guidNode      = hSetResData.guidNode;
//		eNodeType     = hSetResData.eNodeType;
		dwOperateFlag = hSetResData.dwOperateFlag;
		lParam        = hSetResData.lParam;		
	}
	void GetResBaseData(TPResData &hGetResData)
	{
		hGetResData.hResData       = hResData;
		hGetResData.eResType       = eResType;
		hGetResData.guidRes        = guidRes;
		hGetResData.guidDBType     = guidDBType;		
		hGetResData.guidNode       = guidNode;
		hGetResData.dwOperateFlag  = dwOperateFlag;
//		hGetResData.eNodeType      = eNodeType;
		hGetResData.lParam         = lParam;
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

class CTPArticleMainDlg;
class CTPArticleListCtrl : public CTPListCtrlEx
{
	DECLARE_DYNAMIC(CTPArticleListCtrl)

public:
	CTPArticleListCtrl();
	virtual ~CTPArticleListCtrl();

	CTPArticleMainDlg *m_pArticleMainDlg;
public:
	BOOL	SetResData(CGUIDArray &aArticleList, TPChannelData &stuChannel);
public:
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void  GetItemResData(TPListItemData *pItemData,BOOL bGetData = TRUE);
	virtual void    DrawTextPicture(CDC *pDC,int iItem);
protected:
	DECLARE_MESSAGE_MAP()
};


