#pragma once

#include "..\..\Include\TPCommonInclude.h"
typedef CArray<GUID,GUID &> CGUIDArray;

#define PATH_MAX					512

#define  TP_RES_TYPE                ULONGLONG
#define  TP_RES_UNKNOW              0x00000000
#define  TP_RES_CHANNEL				0x00000001
#define  TP_RES_ARTICLE             0x00000002
#define  TP_RES_COMMENT				0x00000004

#define  TP_GRADE_TYPE              ULONGLONG
#define  TP_GRADE_UNKNOW			0x00000000
#define  TP_GRADE_BASE				0x00000001
#define  TP_GRADE_COMMENT			0x00000002

#define  TP_CHANNEL_TYPE            ULONGLONG
#define  TP_CHANNEL_UNKNOW			0x00000000
#define  TP_CHANNEL_TECH			0x00000001
#define  TP_CHANNEL_SPORT			0x00000002

typedef struct _tagTPResBaseInfo
{
	DWORD		dwVersion;
	DWORD		dwState;
	GUID		guidRes; //唯一索引
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
		guidDBType = GUID_NULL;  
		GetLocalTime(&tmCreate);
		GetLocalTime(&tmModify);
		GetLocalTime(&tmRead);
		eResType   = TP_RES_UNKNOW;
	}
	
}TPResBaseInfo;

typedef struct _tagTPPictureItem
{
	LPBYTE     pIconBuf; //图标
	SIZE       szIcon;   //图标尺寸
	LPTSTR	   sPicPath; //图标路径
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
		sPicPath = NULL;
		if(pIconBuf){delete pIconBuf; pIconBuf = NULL;}
		szIcon = CSize(0,0);
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

typedef struct _tagTPArticleData : public TPResBaseInfo
{
	GUID			guidChannel;
	LPTSTR			sName;  
	LPTSTR			sTitle;    //标题
	LPTSTR			sText;     //正文
	INT64			lCommentSize;
	CTPPictureItemArray  aPictureItem;
	_tagTPArticleData()
	{
		Reset();
	}
	void Reset()
	{
		eResType = TP_RES_ARTICLE;
		guidChannel = GUID_NULL;
		sName    = NULL;
		sTitle   = NULL;  
		sText    = NULL;   
		lCommentSize = 0;
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

typedef struct _tagTPChannelData : public TPResBaseInfo
{
	TP_CHANNEL_TYPE		eChannelType;
	int					lUpdateInterval;		//刷新间隔时间（单位是分）
	int					lSaveNum;				//保存条目数

	TPChannelBase		stuChannelBase;
	_tagTPChannelData()
	{
		Reset();
	}
	~_tagTPChannelData()
	{
		eResType	= TP_RES_CHANNEL;
		eChannelType = TP_CHANNEL_UNKNOW;
		lUpdateInterval = 60;  
		lSaveNum		= 500;
		stuChannelBase.Release();
	}
	void Reset()
	{
		eResType	= TP_RES_CHANNEL;
		eChannelType = TP_CHANNEL_UNKNOW;
		lUpdateInterval = 60;  
		lSaveNum		= 500;
		stuChannelBase.Reset();
	}
}TPChannelData;

typedef struct _tagTPArticlePluginInterface
{
	LRESULT  (*TP_GetArticleInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPArticleData &stuArtData); //
	LRESULT  (*TP_SetArticleInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPArticleData &stuArtData); //
	LRESULT  (*TP_DelArticleInfo)(GUID guidRes);

}TPArticlePluginInterface;

typedef struct _tagTPColumePluginInterface
{
	LRESULT  (*TP_GetColumeInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPChannelData &stuChannelData); //
	LRESULT  (*TP_SetColumeInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPChannelData &stuChannelData); //
	LRESULT  (*TP_DelColumeInfo)(GUID guidRes);
	
}TPColumePluginInterface;

typedef struct _tagTPCommentPluginInterface
{
	LRESULT  (*TP_GetCommentInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPCommentData &stuCommentData); //
	LRESULT  (*TP_SetCommentInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPCommentData &stuCommentData); //
	LRESULT  (*TP_DelCommentInfo)(GUID guidRes);
	
}TPCommentPluginInterface;


typedef struct _tagTPArticleManageInterface
{
	TCHAR cManageName[PATH_MAX];

	_tagTPArticleManageInterface()
	{
		ZeroMemory(this,sizeof(_tagTPArticleManageInterface));
	} 

}TPArticleManageInterface;

typedef struct _tagTPArticleIOPluginInterface
{
	TCHAR cPluginName[PATH_MAX];

	TPArticlePluginInterface  stuArticleInterface;
	TPColumePluginInterface   stuColumeInterface;
	TPCommentPluginInterface  stuCommentInterface;
	_tagTPArticleIOPluginInterface()
	{
		ZeroMemory(this,sizeof(_tagTPArticleIOPluginInterface));
	} 

}TPArticleIOPluginInterface;

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