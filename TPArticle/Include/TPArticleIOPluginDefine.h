#pragma once

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
	int					lUpdateInterval;		//刷新间隔时间（单位是分）
	int					lSaveNum;				//保存条目数

	TPChannelBase		stuChannelBase;
	CTPChannelItemArray	aChannelItemAll;
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
		eResType	= TP_RES_CHANNEL;
		eNodeType = TP_CHANNEL_UNKNOW;
		lUpdateInterval = 60;  
		lSaveNum		= 500;
		stuChannelBase.Reset();
		aChannelItemAll.RemoveAll();
	}
	void Release()
	{
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
		aChannelItemAll.Append(stuChannelBase.aChannelItem);
		stuChannelBase.aChannelItem.RemoveAll();//需要清除掉，不然aChannelItemAll，stuChannelBase.aChannelItem会同时释放
	}
}TPChannelData;

typedef struct _tagTPChannelNode : public TPResBaseInfo
{
	TCHAR *cNodeName;
	_tagTPChannelNode()
	{
		Reset();
	}
	~_tagTPChannelNode()
	{
		Release();
	}
	void Reset()
	{
		TPResBaseInfo::Reset();
		cNodeName = NULL;
	}
	void Release()
	{
		if(cNodeName)	{delete cNodeName; cNodeName = NULL;}
		Reset();
	}
}TPChannelNode;
//Manage interface 
typedef struct _tagTPChannelInterface
{
	LRESULT  (*TP_GetChannelInfo)(GUID guidRes,TPChannelData &stuChannelData); //
	LRESULT  (*TP_SetChannelInfo)(GUID guidRes,TPChannelData &stuChannelData); //
	LRESULT  (*TP_DelChannelInfo)(GUID guidRes);

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

}TPChannelPluginInterface;

typedef struct _tagTPCommentPluginInterface
{
	LRESULT  (*TP_GetCommentInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPCommentData &stuCommentData); //
	LRESULT  (*TP_SetCommentInfo)(GUID guidRes,TP_GRADE_TYPE eClipGrade,TPCommentData &stuCommentData); //
	LRESULT  (*TP_DelCommentInfo)(GUID guidRes);

}TPCommentPluginInterface;

typedef struct _tagTPArticleIOPluginInterface
{
	TCHAR cPluginName[PATH_MAX];

	TPArticlePluginInterface	stuArticleInterface;
	TPChannelPluginInterface	stuChannelInterface;
	TPCommentPluginInterface	stuCommentInterface;
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