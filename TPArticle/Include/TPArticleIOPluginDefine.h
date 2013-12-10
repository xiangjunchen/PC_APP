#pragma once

const GUID guidPublicChannelNode		= { 0xA78AFD2D, 0x9fca, 0x4f77, { 0xbe, 0x80, 0xe6, 0xdc, 0xf3, 0x4d, 0xea, 0x5a } };
const GUID guidPublicTechChannelNode	= { 0xA78AFD2D, 0xdb5b, 0x4a6a, { 0xb5, 0x6c, 0x11, 0xe6, 0xac, 0x50, 0xd, 0x18 } };
const GUID guidPublicSportChannelNode	= { 0xA78AFD2D, 0xc34b, 0x4246, { 0x97, 0xdd, 0xe4, 0x1d, 0xe6, 0x27, 0x1d, 0x5b } };

const GUID guidPrivateChannelNode  = { 0xA78AFFFF, 0x9fca, 0x4f77, { 0xbe, 0x80, 0xe6, 0xdc, 0xf3, 0x4d, 0xeb, 0x5b } };

#include "..\..\Include\TPCommonInclude.h"
typedef CArray<GUID,GUID &> CGUIDArray;

#define PATH_MAX					512

#define  TP_RES_TYPE                ULONGLONG
#define  TP_RES_UNKNOW              0x00000000
#define  TP_RES_CHANNELNODE			0x00000001
#define  TP_RES_CHANNEL				0x00000002
#define  TP_RES_ARTICLE             0x00000004
#define  TP_RES_COMMENT				0x00000008

#define  TP_GRADE_TYPE              ULONGLONG
#define  TP_GRADE_UNKNOW			0x00000000
#define  TP_GRADE_BASE				0x00000001
#define  TP_GRADE_COMMENT			0x00000002
#define  TP_GRADE_ALL			    0x007FFFFF

#define  TP_CHANNEL_NODETYPE		ULONGLONG
#define  TP_CHANNEL_UNKNOW			0x00000000
#define  TP_CHANNEL_SYSTEM			0x00000001
#define  TP_CHANNEL_PERSON			0x00000002
#define  TP_CHANNEL_TECH			0x00000004
#define  TP_CHANNEL_SPORT			0x00000008
#define  TP_CHANNEL_LIFE			0x00000010
#define  TP_CHANNEL_TYPEALL			(TP_CHANNEL_TECH|TP_CHANNEL_SPORT|TP_CHANNEL_LIFE)


#define  HRESDATA void *
typedef CArray<void *,void *&> HRESDATALIST;

typedef struct stu_TPResData
{
	HRESDATA       hResData;
	HRESDATA       hResCache;
	TP_RES_TYPE    eResType;
	DWORD          dwOperateFlag;
	GUID           guidRes;		
	GUID           guidNode;
	ULONGLONG      eNodeType; 
	GUID           guidDBType; 
	GUID           guidReturn;

	WPARAM         wParam;	
	LPARAM         lParam;	
	GUID           guidParam;
	UINT           uState;
	LPARAM		   lEffParam;
	DWORD		   dwVideoStandard;
	stu_TPResData()
	{
		Reset();
	}
	void Reset()
	{	
		hResData      = NULL;
		hResCache     = NULL;
		eResType      = TP_RES_UNKNOW;
		dwOperateFlag = 0;
		guidRes       = GUID_NULL;			
		guidNode      = GUID_NULL;
		guidDBType    = GUID_NULL;
		guidReturn    = GUID_NULL;
		eNodeType     = 0;
		wParam        = NULL;
		guidParam     = GUID_NULL;
		lParam        = NULL;
		uState        = 0;
		lEffParam	  = NULL;
		dwVideoStandard	= 0;
	}
	stu_TPResData(GUID guidDBType,GUID guidCatalog,ULONGLONG eNodeType,TP_RES_TYPE eResType,GUID guidRes, GUID guidReturn)
	{
		this ->hResData       = NULL;
		this ->hResCache      = NULL;
		this ->eResType       = eResType;
		this ->dwOperateFlag  = dwOperateFlag;
		this ->guidRes        = guidRes;
		this ->guidNode       = guidCatalog;
		this ->guidDBType     = guidDBType;
		this ->guidReturn     = guidReturn;
		this ->eNodeType      = eNodeType;
		this ->lParam         = NULL;
		this ->wParam         = NULL;
		this ->uState         = 0;
		this ->guidParam      = GUID_NULL;
		this ->dwVideoStandard = 0;
	}
	stu_TPResData(GUID guidDBType,GUID guidCatalog,TP_RES_TYPE eResType,GUID guidRes,DWORD dwOperateFlag)
	{
		this ->hResData       = NULL;
		this ->hResCache      = NULL;
		this ->eResType       = eResType;
		this ->dwOperateFlag  = dwOperateFlag;
		this ->guidRes        = guidRes;
		this ->guidNode       = guidCatalog;
		this ->guidDBType     = guidDBType;
		this ->guidReturn     = GUID_NULL;
		this ->eNodeType      = 0;
		this ->lParam         = NULL;
		this ->wParam         = NULL;
		this ->uState         = 0;
		this ->guidParam      = GUID_NULL;
		this ->dwVideoStandard = 0;
	}
	stu_TPResData(GUID guidDBType,HRESDATA hResData,TP_RES_TYPE eResType,GUID guidRes)
	{
		this ->hResData       = hResData;
		this ->hResCache      = NULL;
		this ->eResType       = eResType;
		this ->dwOperateFlag  = 0;
		this ->guidRes        = guidRes;
		this ->guidNode       = GUID_NULL;
		this ->guidDBType     = guidDBType;
		this ->guidReturn     = GUID_NULL;
		this ->eNodeType      = 0;
		this ->lParam         = NULL;
		this ->wParam         = NULL;
		this ->uState         = 0;
		this ->guidParam      = GUID_NULL;
		this ->dwVideoStandard = 0;
	}
	stu_TPResData(GUID guidDBType,HRESDATA hResData,TP_RES_TYPE eResType,GUID guidRes,HRESDATA hResCache)
	{
		this ->hResData       = hResData;
		this ->hResCache      = hResCache;
		this ->eResType       = eResType;
		this ->dwOperateFlag  = 0;
		this ->guidRes        = guidRes;
		this ->guidNode       = GUID_NULL;
		this ->guidDBType     = guidDBType;
		this ->guidReturn     = GUID_NULL;
		this ->eNodeType      = 0;
		this ->lParam         = NULL;
		this ->wParam         = NULL;
		this ->uState         = 0;
		this ->guidParam      = GUID_NULL;
		this ->dwVideoStandard = 0;
	}
	void Copyto(stu_TPResData *pResData)
	{
		pResData->eResType     = eResType;
		pResData->guidNode     = guidNode;
		pResData->guidRes      = guidRes;
		pResData->hResCache    = hResCache;
		pResData->hResData     = hResData;
		pResData->eNodeType    = eNodeType;
		pResData->guidDBType   = guidDBType;		
		pResData->guidReturn   = guidReturn;
		pResData->lParam       = lParam;
		pResData->wParam       = wParam;
		pResData ->uState      = uState;
		pResData->guidParam    = guidParam;
		pResData->dwVideoStandard = dwVideoStandard;
	}
	bool  operator < (const stu_TPResData &stuTemp) const
	{
		int nComparePre = memcmp(&guidDBType,&stuTemp.guidDBType,sizeof(GUID));
		if (nComparePre != 0)
			return nComparePre<0;
		else
			return memcmp(&guidRes,&stuTemp.guidRes,sizeof(GUID))<0;
	}

}TPResData;
typedef CArray<TPResData ,TPResData &> TPResDataArray;

typedef struct stu_TPResGuid
{ 
	TP_RES_TYPE    eResType;
	GUID           guidRes;		 
	GUID           guidDBType; 	
	stu_TPResGuid()
	{
		eResType      = TP_RES_UNKNOW;
		guidRes       = GUID_NULL;			
		guidDBType    = GUID_NULL;
	}
	stu_TPResGuid(GUID guidDBType1,TP_RES_TYPE eResType1,GUID guidRes1)
	{
		eResType      = eResType1;
		guidRes       = guidRes1;			
		guidDBType    = guidDBType1;
	}
	stu_TPResGuid(GUID guidDBType1, GUID guidRes1)
	{
		eResType      = TP_RES_UNKNOW;
		guidRes       = guidRes1;			
		guidDBType    = guidDBType1;
	}
	void SetAt(GUID guidDBType1, GUID guidRes1)
	{		 
		guidRes       = guidRes1;			
		guidDBType    = guidDBType1;
	}
	stu_TPResGuid & operator= (stu_TPResGuid stuTemp)
	{
		eResType      = stuTemp.eResType;
		guidRes       = stuTemp.guidRes;			
		guidDBType    = stuTemp.guidDBType;
		return *this;
	} 
	BOOL  operator != (stu_TPResGuid &stuTemp)
	{
		if(guidDBType != stuTemp.guidDBType) return TRUE;
		if(guidRes    != stuTemp.guidRes)    return TRUE;
		return FALSE;
	}
	BOOL  operator == (stu_TPResGuid &stuTemp)
	{
		if(guidDBType != stuTemp.guidDBType) return FALSE;
		if(guidRes    != stuTemp.guidRes)    return FALSE;
		return TRUE;
	}
	BOOL SameGuid(GUID guidDBType1, GUID guidRes1)
	{
		if(guidDBType != guidDBType1) return FALSE;
		if(guidRes    != guidRes1)    return FALSE;
		return TRUE;
	}
}TPResGuid;
typedef CArray<TPResGuid ,TPResGuid &> TPResGuidArray;


typedef struct _tagTPResBaseInfo
{
	DWORD		dwVersion;
	DWORD		dwState;
	GUID		guidRes; //唯一索引
	GUID		guidNode; //所属节点
	GUID		guidDBType; 
	SYSTEMTIME	tmCreate;
	SYSTEMTIME	tmModify;
	SYSTEMTIME	tmRead;
	TP_RES_TYPE eResType;

	_tagTPResBaseInfo()
	{
		Reset();
	}
	void Reset()
	{
		dwVersion = 0;
		dwState   = 0;
		guidRes   = GUID_NULL;
		guidNode  = GUID_NULL;
		guidDBType = GUID_NULL;  
		GetLocalTime(&tmCreate);
		GetLocalTime(&tmModify);
		GetLocalTime(&tmRead);
		eResType   = TP_RES_UNKNOW;
	}
	
}TPResBaseInfo;

typedef struct _tagTPPictureItem
{
	//LPBYTE     pIconBuf; //图标
	SIZE       szIcon;   //图标尺寸
	TCHAR      *cPicPath;//图标路径 
	_tagTPPictureItem()
	{
		Reset();
	}
	~_tagTPPictureItem()
	{
		Reset();
	}
	void Reset()
	{
		cPicPath = NULL;
//		if(pIconBuf){delete pIconBuf; pIconBuf = NULL;}
		szIcon = CSize(0,0);
	}
	void SaveFile(CFile &cFileWrite)
	{
		cFileWrite.Write(&szIcon,sizeof(SIZE));
		TP_WriteStrToFile(cPicPath, cFileWrite);
	}
	void ReadFile(CFile &cFileRead)
	{
		Release();

		cFileRead.Read(&szIcon,sizeof(SIZE));
		TP_ReadStrFromFile(cPicPath, cFileRead);
	}
	void Release()
	{
		if(cPicPath)  {delete cPicPath ; cPicPath = NULL;}

		Reset();
	}

}TPPictureItem;
typedef CArray<TPPictureItem *, TPPictureItem *&>  CTPPictureItemArray;

typedef struct _tagTPCommentData : public TPResBaseInfo
{
	GUID			guidArticle;
	GUID			guidCommentNode;
	LPTSTR			sComment;
	INT64			lCommentIndex;
	_tagTPCommentData()
	{
		Reset();
	}
	void Reset()
	{
		guidArticle      = GUID_NULL;
		guidCommentNode  = GUID_NULL;
		sComment         = NULL;
		lCommentIndex    = 0;
	}

}TPCommentData;

#define TP_ARTICLE_VERSION1		1  //
#define TP_ARTICLE_VERSION		TP_ARTICLE_VERSION1


typedef struct _tagTPArticleData : public TPResBaseInfo
{
	TCHAR					*cText;     //正文
	INT64					lCommentSize;
	TPChannelItem			stuChannelItem;
	CTPPictureItemArray		aPictureItem;
	_tagTPArticleData()
	{
		Reset();
	}
	~_tagTPArticleData()
	{
		Release();
	}
	void Release()
	{
		if(cText)	{delete cText; cText = NULL;}
		stuChannelItem.Release();
		ReleasPicture();

		Reset();
	}
	void Reset()
	{
		eResType = TP_RES_ARTICLE;
		cText    = NULL;
		lCommentSize = 0;
		stuChannelItem.Reset();
		aPictureItem.RemoveAll();
	}
	void ReleasPicture()
	{
		for (int l = 0 ; l < aPictureItem.GetSize(); l++)
		{
			delete aPictureItem[l];
			aPictureItem[l] = NULL;
		}
		aPictureItem.RemoveAll();
	}
}TPArticleData;

#define TP_CHANNEL_VERSION1		1  //
#define TP_CHANNEL_VERSION		TP_CHANNEL_VERSION1

typedef struct _tagTPChannelData : public TPResBaseInfo
{
	TP_CHANNEL_NODETYPE		eNodeType;
	int						lUpdateInterval;		//刷新间隔时间（单位是分）
	int						lSaveNum;				//保存条目数
	TCHAR					*cKeyDiv;
	TPChannelBase			stuChannelBase;
	CTPChannelItemArray		aChannelItemAll;
	_tagTPChannelData()
	{
		Reset();
	}
	~_tagTPChannelData()
	{
		Release();
	}
	void Reset()
	{
		TPResBaseInfo::Reset();
		cKeyDiv     = NULL;
		eResType	= TP_RES_CHANNEL;
		eNodeType = TP_CHANNEL_UNKNOW;
		lUpdateInterval = 60;  
		lSaveNum		= 500;
		stuChannelBase.Reset();
		aChannelItemAll.RemoveAll();
	}
	void Release()
	{
		if(cKeyDiv)	{delete cKeyDiv; cKeyDiv = NULL;}
		stuChannelBase.Release();
		for (int l=0;l<aChannelItemAll.GetSize();l++)	
		{
			delete aChannelItemAll[l];
			aChannelItemAll[l] = NULL;
		}
		aChannelItemAll.RemoveAll();	

		Reset();
	}
	void AppendUpdateItem()
	{
		int nUpdate = stuChannelBase.aChannelItem.GetSize();
		int nAllSize = aChannelItemAll.GetSize();
		for (int i = nAllSize - 1; i >= 0; i--)
		{
			if(nAllSize - i > nUpdate)	break;

			int l = 0;
			for (; l < stuChannelBase.aChannelItem.GetSize(); l++)
			{
				if(aChannelItemAll[i]->IsEqual(stuChannelBase.aChannelItem[l]))
					break;
			}
			if(l < stuChannelBase.aChannelItem.GetSize())
			{
				int iRemoveCount = 0;
				for (int k = stuChannelBase.aChannelItem.GetSize() - 1; k >= 0; k--)
				{
					delete stuChannelBase.aChannelItem[k];
					stuChannelBase.aChannelItem[k] = NULL;
					stuChannelBase.aChannelItem.RemoveAt(k);
					if(l == k)
						break;
				}
				break;
			}
		}
 		for (int l = stuChannelBase.aChannelItem.GetSize() - 1 ; l >= 0; l--)
 		{
			aChannelItemAll.Add(stuChannelBase.aChannelItem[l]);
			stuChannelBase.aChannelItem.RemoveAt(l);//需要清除掉，不然aChannelItemAll，stuChannelBase.aChannelItem会同时释放
 		}
	}
protected:
	_tagTPChannelData& operator = (const _tagTPChannelData & stuSrc);
	_tagTPChannelData(const _tagTPChannelData & stuSrc);
}TPChannelData;
typedef CArray<TPChannelData ,TPChannelData &> TPChannelDataArray;

#define TP_CHANNELNODE_VERSION1		1  //
#define TP_CHANNELNODE_VERSION		TP_CHANNELNODE_VERSION1

typedef struct _tagTPChannelNodeData : public TPResBaseInfo
{
	TCHAR					*cNodeName;
	TP_CHANNEL_NODETYPE		eNodeType;
	_tagTPChannelNodeData()
	{
		Reset();
	}
	~_tagTPChannelNodeData()
	{
		Release();
	}
	void Reset()
	{
		TPResBaseInfo::Reset();
		cNodeName = NULL;
		eNodeType = TP_CHANNEL_UNKNOW;
	}
	void Release()
	{
		if(cNodeName)	{delete cNodeName; cNodeName = NULL;}
		Reset();
	}
}TPChannelNodeData;
//Manage interface 
typedef struct _tagTPChannelNodeInterface
{
	LRESULT  (*TP_GetChannelNodeInfo)(GUID guidRes,TPChannelNodeData &stuChannelData); //
	LRESULT  (*TP_SetChannelNodeInfo)(GUID guidRes,TPChannelNodeData &stuChannelData); //
	LRESULT  (*TP_DelChannelNodeInfo)(GUID guidRes);
	LRESULT  (*TP_GetChannelNodeChild)(GUID guidRes, TPResDataArray &hChildRes);
	//LRESULT  (*TP_GetChannelNodeChild)(HRESDATA hCatalogData,TPResDataArray &hChildRes,DWORD eLockType);
	_tagTPChannelNodeInterface()
	{
		TP_GetChannelNodeInfo = NULL;
		TP_SetChannelNodeInfo = NULL;
		TP_DelChannelNodeInfo = NULL;
	}
}TPChannelNodeInterface;
typedef struct _tagTPChannelInterface
{
	LRESULT  (*TP_GetChannelInfo)(GUID guidRes,TPChannelData &stuChannelData); //
	LRESULT  (*TP_SetChannelInfo)(GUID guidRes,TPChannelData &stuChannelData); //
	LRESULT  (*TP_DelChannelInfo)(GUID guidRes);
	LRESULT  (*TP_GetChannelChild)(GUID guidRes, TPResDataArray &hChildRes);
	BOOL     (*TP_IsChannelExist)(GUID guidChannelNode, TPChannelBase *pChannelInfo);

	_tagTPChannelInterface()
	{
		TP_GetChannelInfo = NULL;
		TP_SetChannelInfo = NULL;
		TP_DelChannelInfo = NULL;
	}
}TPChannelInterface;
typedef struct _tagTPArticleInterface
{
	LRESULT  (*TP_GetArticleInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade, TPArticleData &stuArticleData); //
	LRESULT  (*TP_SetArticleInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade, TPArticleData &stuArticleData); //
	LRESULT  (*TP_DelArticleInfo)(GUID guidRes);

	_tagTPArticleInterface()
	{
		TP_GetArticleInfo = NULL;
		TP_SetArticleInfo = NULL;
		TP_DelArticleInfo = NULL;
	}
}TPArticleInterface;
typedef struct _tagTPCommentInterface
{
	_tagTPCommentInterface()
	{

	}
}TPCommentInterface;
typedef struct _tagTPFuncionInterface
{
	_tagTPFuncionInterface()
	{

	}
}TPFuncionInterface;
typedef struct _tagTPArticleManageInterface
{
	TCHAR cManageName[PATH_MAX];

	TPChannelNodeInterface	stuChannelNodeInterface;
	TPChannelInterface		stuChannelInterface;
	TPArticleInterface      stuArticleInterfce; 
	TPCommentInterface      stuCommentInterfce; 
	TPFuncionInterface		stuFuncitonInterface;

	_tagTPArticleManageInterface()
	{
		ZeroMemory(this,sizeof(_tagTPArticleManageInterface));
	} 

}TPArticleManageInterface;
//

//IOPlugin interface 
typedef struct _tagTPArticlePluginInterface
{
	LRESULT  (*TP_GetArticleInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPArticleData &stuArtData); //
	LRESULT  (*TP_SetArticleInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPArticleData &stuArtData); //
	LRESULT  (*TP_DelArticleInfo)(GUID guidRes);

}TPArticlePluginInterface;

typedef struct _tagTPChannelPluginInterface
{
	LRESULT  (*TP_GetChannelInfo)(GUID guidRes,TPChannelData &stuChannelData); //
	LRESULT  (*TP_SetChannelInfo)(GUID guidRes,TPChannelData &stuChannelData); //
	LRESULT  (*TP_DelChannelInfo)(GUID guidRes);
	LRESULT  (*TP_GetChannelChild)(GUID guidRes, TPResDataArray &hChildRes);
	BOOL     (*TP_IsChannelExist)(GUID guidChannelNode, TPChannelBase *pChannelInfo);

}TPChannelPluginInterface;

typedef struct _tagTPChannelNodePluginInterface
{
	LRESULT  (*TP_GetChannelNodeInfo)(GUID guidRes,TPChannelNodeData &stuChannelNode); //
	LRESULT  (*TP_SetChannelNodeInfo)(GUID guidRes,TPChannelNodeData &stuChannelNode); //
	LRESULT  (*TP_DelChannelNodeInfo)(GUID guidRes);
	LRESULT  (*TP_GetChannelNodeChild)(GUID guidRes, TPResDataArray &hChildRes);
	_tagTPChannelNodePluginInterface()
	{
		TP_GetChannelNodeChild = NULL;
	}

}TPChannelNodePluginInterface;

typedef struct _tagTPCommentPluginInterface
{
	LRESULT  (*TP_GetCommentInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPCommentData &stuCommentData); //
	LRESULT  (*TP_SetCommentInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPCommentData &stuCommentData); //
	LRESULT  (*TP_DelCommentInfo)(GUID guidRes);

}TPCommentPluginInterface;

typedef struct _tagTPArticleIOPluginInterface
{
	TCHAR cPluginName[PATH_MAX];

	TPArticlePluginInterface		stuArticleInterface;
	TPChannelNodePluginInterface	stuChannelNodeInterface;
	TPChannelPluginInterface		stuChannelInterface;
	TPCommentPluginInterface		stuCommentInterface;
	_tagTPArticleIOPluginInterface()
	{
		ZeroMemory(this,sizeof(_tagTPArticleIOPluginInterface));
	} 

}TPArticleIOPluginInterface;
//

typedef struct _tagTPBaseProcess
{
	CString strProcessName;
	LRESULT (*TP_GetProcessName)(LPTSTR &sProcessName);
	LRESULT (*TP_InitData)(void *pData);
	LRESULT (*TP_ReleaseData)(void *pData);
	_tagTPBaseProcess()
	{
		strProcessName = _T("");
		TP_GetProcessName	 = NULL;
		TP_InitData			 = NULL;
		TP_ReleaseData		 = NULL;
	}
}TPBaseProcess;

typedef struct _tagTPArticleManageProcess : public TPBaseProcess
{
	LRESULT (*TP_GetManageFunction)(TPArticleManageInterface *pInterface);
	_tagTPArticleManageProcess()
	{
		TP_GetManageFunction = NULL;
	}
}TPArticleManageProcess;

typedef struct _tagTPArticleIOPluginProcess : public TPBaseProcess
{
	LRESULT (*TP_GetPlugInFunction)(TPArticleIOPluginInterface *pInterface);
	_tagTPArticleIOPluginProcess()
	{
		TP_GetPlugInFunction = NULL;
	}
}TPArticleIOPluginProcess;