#pragma once


// CTPArticleListCtrl
typedef  struct tag_TPListItemData
{

	HRESDATA       hResData;			//  �ľ��
	TP_RES_TYPE    eResType;            //  �ز�����
	GUID           guidRes;				//  �ز�ID
	GUID           guidDBType;			//  ���ݿ�ID
	GUID           guidNode;            //  ����Ŀ¼ID
	GUID           guidRecyleNode;      //  ����Ŀ¼ID
//	TP_NODE_TYPE   eNodeType;			//  Ŀ¼����
	DWORD          dwOperateFlag;       //  ��������
	LPARAM         lParam;
	DWORD          dwState;
	//��ʾ����
	UINT           uResSource;			//  ��Դ���ӱ�־ U1
	DWORD          dwUseState;			//  ʹ��״̬��־
	int            iNPIndex;			//  ͼ������
	long           lRefCount;			//  ���ü���
	CString        VAString;	
//	CString        sText[M2];			//  ������Ϣ
	DWORD          dwBookIndex; 

	// ͼ����Ϣ
	BOOL           bIs0403Icon;         //  �Ƿ�Ϊ4��3�ĸ�ʽ
	BOOL           bDelIcon;			
	LPBYTE         pIcon;				//  Сͼ��
	CSize          szIcon;
	BOOL           bGetData;			// 
	BYTE           bGetIndex; 
	BOOL           bGetFile;	
	DWORD          dwCaptureFlag;
	LPBYTE         pBuffer;             //  ��ͼ������
	CSize          szBuffer;
	CString        sFileName;			//  ͼ���ļ���
	CString        sKeyFileName;		//  KYE�ļ���
	INT64          iFrame;				//  ͼ��λ��			
	BOOL           bGetBuffer;			

	BOOL				bDropFrame;		//  ��֡��־
	COLORREF            cResColor;
	TP_VIDEO_STANDARD   eVideoStand; 
	INT64               uResFlag;	
	INT64				lLength;
	INT64               lListItemFlag;
	SYSTEMTIME			tmCreateTime;	//  ����ʱ��
	SYSTEMTIME          tmModifyTime;   //  �޸�ʱ��
	SYSTEMTIME			tmRecTime;		//  ����ʱ��
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


