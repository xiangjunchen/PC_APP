#pragma once
#include "afxtempl.h"
#include <vector>
using namespace std;

typedef LRESULT (*LPET_CallBackForML)(UINT uMsg,WPARAM wParam,LPARAM lParam);

//权限定义
#define POPEDOM_ADMINISTRATOR		_T("ETAdministrator")

//{{2008/12/10 zxp:modify 修改并增加相应宏定义，实现PE和ML的权限分离
#define POPEDOM_IMPORT_MAT		   _T("ETImportMaterial")	// 引入素材	    是否可以引入素材（针对Material List）
#define POPEDOM_COPY_ITEM          _T("ETCopyItem")         // 拷贝Item     是否可以拷贝item（针对Material List）
#define POPEDOM_CREATE_FOLD		   _T("ETNewFolder")		// 新建文件夹	是否可以新建文件夹（针对于Material List）
#define POPEDOM_CREATE_PGM	       _T("ETNewSequence")		// 新建节目	    是否可以新建节目（针对Material List）
#define POPEDOM_MODIFY_FOLD		   _T("ETModifyFolder")		// 修改文件夹	是否可以修改文件夹（针对于Material List）
#define POPEDOM_MODIFY_MAT		   _T("ETModifyMaterial")	// 修改素材	    是否可以修改素材（针对Material List）
#define POPEDOM_MODIFY_PGM         _T("ETModifySequence")   // 修改节目     是否可以打开节目进行修改（针对Material List）
#define POPEDOM_DELETE_MAT		   _T("ETDeleteMaterial")	// 删除素材	    是否可以删除素材（针对Material List）
#define POPEDOM_DELETE_PGM		   _T("ETDeleteSequence")	// 删除节目	    是否可以删除节目（针对Material List）
#define POPEDOM_DELETE_FOLD		   _T("ETDeleteFolder")		// 删除文件夹	是否可以删除空文件夹（针对于Material List）
#define POPEDOM_DELETE_FOLDTREE	   _T("ETDeleteFolderTree")	// 删除文件夹树	是否可以删除文件夹树（针对于Material List）
#define POPEDOM_PURGE_MAT          _T("ETPurgefromTranCan") // 清空素材     是否可以清空素材（针对Material List）

#define POPEDOM_IMPORT_MAT_PE   _T("ETImportMaterialForPE") // 引入素材     是否可以引入素材（针对Project Explorer）
#define POPEDOM_INGEST_MAT      _T("ETIngestMaterial")      // 收录素材     是否可以收录素材（针对Project Explorer）
#define POPEDOM_COPY_ITEM_PE    _T("ETCopyItemForPE")       // 拷贝Item     是否可以拷贝item（针对Project Explorer）
#define POPEDOM_CREATE_TEMPL	_T("ETNewTemplate")			// 新建模版	    是否可以新建模版（针对Project Explorer）
#define POPEDOM_CREATE_GROUP	_T("ETNewProjectGroup")		// 新建项目组	是否可以新建项目组（针对Project Explorer）
#define POPEDOM_CREATE_PROJ		_T("ETNewProject")			// 新建项目	    是否可以新建项目，以及项目下的目录数据修改（针对Project Explorer）
#define POPEDOM_ADDNEW_BIN      _T("ETAddNewBin")           // 添加BIN文件  是否可以添加BIN文件（针对Project Explorer）
#define POPEDOM_ADDNEW_MAT      _T("ETAddNewMaterial")      // 添加素材     是否可以添加素材（针对Project Explorer）
#define POPEDOM_BUILD_PGM       _T("ETBuildNewSequence")    // 新建节目     是否可以新建节目（针对Project Explorer）
#define POPEDOM_MODIFY_TEMPL	_T("ETModifyTemplate")		// 修改模版	    是否可以修改模版（针对Project Explorer）
#define POPEDOM_MODIFY_GROUP	_T("ETModifyProjectGroup")	// 修改项目组	是否可以修改项目组数据（针对于Project Explore）
#define POPEDOM_MODIFY_PROJ		_T("ETModifyProject")		// 修改项目	    是否可以修改项目数据，以及项目下的目录数据修改（针对于Project Explore）
#define POPEDOM_MODIFY_BIN      _T("ETModifyBin")           // 修改BIN文件  是否可以修改BIN文件（针对Project Explorer）
#define POPEDOM_MODIFY_MAT_PE   _T("ETModifyMaterialForPE") // 修改素材     是否可以修改素材（针对Project Explorer）
#define POPEDOM_MODIFY_PGM_PE	_T("ETModifySequenceForPE")	// 修改节目	    是否可以打开节目进行修改（针对Project Explorer）
#define POPEDOM_DELETE_PGM_PE   _T("ETDeleteSequenceForPE") // 删除节目     是否可以删除节目（针对Project Explorer）
#define POPEDOM_DELETE_MAT_PE   _T("ETDeleteMaterialForPE") // 删除素材     是否可以删除素材（针对Project Explorer）
#define POPEDOM_DELETE_BIN      _T("ETDeleteBin")           // 删除BIN文件  是否可以删除BIN文件（针对Project Explorer）
#define POPEDOM_DELETE_PROJ		_T("ETDeleteProject")		// 删除项目	    是否可以删除项目，以及项目下的目录数据修改（针对于Project Explore）
#define POPEDOM_DELETE_GROUP	_T("ETDeleteProjectGroup")	// 删除项目组	是否可以删除项目组（针对于Project Explore）
#define POPEDOM_DELETE_TEMPL	_T("ETDeleteTemplate")		// 删除模版	    是否可以删除模版（针对Project Explorer）
#define POPEDOM_PURGE_MAT_PE    _T("ETPurgefromTranCanForPE")//清空素材     是否可以清空素材（针对Project Explorer）
//}}

#define POPEDOM_CAPTURE_MAT		_T("ETCaptureMaterial")		// 采集素材	是否可以采集素材
#define POPEDOM_CAN_USE_MAT		_T("ETCanUseMaterial")		// 能使用Material List	是否可以使用Material List程序
#define POPEDOM_CAN_USE_EDIT	_T("ETCanUseEdit")			// 能使用非编	是否可以使用非编程序
#define POPEDOM_CAN_USE_CENSOR	_T("ETCanUseCensor")		// 能使用审片	是否可以使用审片程序
#define POPEDOM_JUNIOR_CENSOR	_T("ETJuniorCensor")		// 节目初级审查权	可以直接打开别人做的节目进行修改、初级审查
#define POPEDOM_MIDDLE_CENSOR	_T("ETMiddleCensor")		// 节目中级审查权	可以直接打开别人做的节目进行修改、中级审查、发送合成
#define POPEDOM_SENIOR_CENSOR	_T("ETSeniorCensor")		// 节目终级审查权	可以打开别人的节目，能够看到合成的素材及自动技审的结果，审查通过后在演播室看到的节目状态就是审查通过的
#define POPEDOM_SENIOR_MODIFY	_T("ETSeniorModify")		// 节目终级修改权	可以修改通过了终级审查的节目
#define POPEDOM_DUB				_T("ETDubPopedom")			//	配音权限	可以给节目做配音操作

#define POPEDOM_OP_PLANLIST		_T("ETOperatePlanList")		//	PLANLIST操作权限	增加删除编辑




//控件的公共消息定义用于
#define WM_USER_SETSOM				(WM_USER + 2979)		//设置属性页的入点
#define WM_USER_SETEOM				(WM_USER + 2980)		//设置属性页的出点
#define WM_USER_UPDATE_MARK_INFO	(WM_USER + 2990)		//ClipTrimmer更新标记点消息
#define WM_USER_SET_CURRENT_MARK	(WM_USER + 2991)		//ClipTrimmer设置当前选中的标记点
#define WM_USER_SEEK_CLIPTRIMMER	(WM_USER + 2992)		//在标记点页面Seek编辑控件ClipTrimmer消息，第一个参数为帧位置
#define WM_USER_SET_PROPDATA		(WM_USER + 2993)		//设置属性页数据，目前定义为wParam表示数据项(1表示标题，2表示备注)，lParam是字符串数据指针
#define WM_USER_REFRESH_ML_OBJECT	(WM_USER + 2994)		//刷新素性页的显示
#define WM_USER_ML_PROPDATA_CHANGE	(WM_USER + 2995)		//素性页的数据发生改变
#define WM_USER_LOGGINGNOTYFY       (WM_USER + 2996)        //通知INGEST创建LOGGING任务 wParam 字符串数据指针
#define WM_USER_INGESTMESSAGE       (WM_USER + 2997)        //import list 通知ML打开Planlist
#define WM_USER_SEEK_SCENE_MARK		(WM_USER + 2998)		//在标记点页面Seek编辑控件ClipTrimmer消息，第一个参数为SceneIn,第二个参数为sceneOut帧位置


//调用者角色定义
typedef		long ET_PROP_CALLER_TYPE;//属性页调用者类型
#define		ET_PROP_UNKNOW			0 //未知调用者
#define		ET_PROP_MATERIALLIST	1 //MaterialList中调用属性页面
#define		ET_PROP_PROJECTEXPLORER	2 //ProjectExplorer中调用属性页面
#define		ET_PROP_CLIPTRIMMER		3 //ClipTrimmer中调用属性页面
#define		ET_PROP_INGEST			4 //Ingest中调用属性页面
#define		ET_PROP_REMOTEPREVIEW	5 //远程预览调用的属性页面
#define     ET_PROP_LOGGING         6 //logging终端调用的属性页面

// UDP消息：创建、删除、修改的引发者，用来区分是否需要接受广播的UDP消息
#define		ET_UDPMSG_CAUSER_UNKNOWN	ET_PROP_UNKNOW
#define		ET_UDPMSG_CAUSER_ML			ET_PROP_MATERIALLIST
#define		ET_UDPMSG_CAUSER_PE			ET_PROP_PROJECTEXPLORER
#define		ET_UDPMSG_CAUSER_CT			ET_PROP_CLIPTRIMMER
#define		ET_UDPMSG_CAUSER_ING		ET_PROP_INGEST
#define		ET_UDPMSG_CAUSER_NLE		(ET_PROP_INGEST + 1)
#define		ET_UDPMSG_CAUSER_OTHER		(ET_PROP_INGEST + 2)
#define		ET_UDPMSG_CAUSER_LOGGING    (ET_PROP_INGEST + 3)

//非编回调参数
#define ET_NLECALLBACK_OPEN_PROGRAM				0x0001
#define ET_NLECALLBACK_CREATE_PROGRAM			0x0002
#define ET_NLECALLBACK_CLOSE_ALL_PGM			0x0003//关闭所有节目,返回S_FALSE就是用户放弃了关闭文件,正常S_OK
#define ET_NLECALLBACK_OPEN_CLIP_TO_EDITVIEW	0x0004
#define ET_NLECALLBACK_CREATE_CLIP				0x0005
#define ET_NLECALLBACK_GET_STANDARD_LIST		0x0006//通过回调6，传入两个CStringArray参数，可以得到制式和音频格式的string信息
#define ET_NLECALLBACK_GET_STANDARD_INFO		0x0007//然后可以通过回调7，传入int指针和选择的index,可以取得制式信息，这个信息是可以直接入库的
#define ET_NLECALLBACK_GET_AUDIO_INFO			0x0008//通过回调8，传入ETMedia那个结构指针和选择的index，可以取得音频信息(音频信息里只有audiochannels,audiosamplesrate,audiobitspersample有用，其它的都无效)
#define ET_NLECALLBACK_SAVE_PROGRAM				0x0009//wparam为需要保存的节目guid
#define ET_NLECALLBACK_RENAME_PGM				0x000A//wparam为节目的guid指针，lparam为新的名字( CString指针 )
#define ET_NLECALLBACK_QUAERE_REFRESH_TIMELINE	0x000B//询问非编需不需要刷新时间线，wParam为素材GUID
#define ET_NLECALLBACK_REFRESH_TIMELINE			0x000C//刷新时间线，wParam为ETArrayClipInfo指针（需要更新的素材），lParam为CArray<GUID, GUID&>指针（已刷新的节目GUID）
#define ET_NLECALLBACK_GEALLUSEOBJ_FROM_NLE		0x001B//得到非编所有正在使用的对象，wparam为CArray<long, long>指针
#define ET_NLECALLBACK_SET_CURRENTPROJECT		0x001E//wparam为工程名字符串指针
#define ET_NLECALLBACK_GET_PGMVALIDLENGTH		0x001D//获取节目的有效长度,wparam为stuPgm指针
#define ET_NLECALLBACK_IMPORTEDL				0x001F//导入EDL，无参数
#define ET_NLECALLBACK_EXPORTEDL				0x0020//导出EDL，wParam为节目guid指针(GUID *)
#define ET_NLECALLBACK_MODIFYPGMFILEGUID		0x0021//修改节目文件的GUID，wParam为stuModifyPgm的指针
#define ET_NLECALLBACK_GETICON					0x0022//获取ICON buffer， wParam为stuGetIconParam的指针
#define ET_NLECALLBACK_NETNOTICEMSG				0x0023//网络通知消息，wParam为ETObject结构体指针，lParam为ETNetMsgCallbackHeader结构体指针，调用方自己会释放。



//拖拽协议中定义的数据
const long ET_DRAGPROTOCOL_VER			= 25;

// 对象类型掩码
typedef long ET_DRAG_OBJTYPEMASK_TYPE;
const long ET_DRAG_OBJTYPE_CLIP				= 0x0001;
const long ET_DRAG_OBJTYPE_PROGRAM			= 0x0002;
const long ET_DRAG_OBJTYPE_EDL				= 0x0004;
const long ET_DRAG_OBJTYPE_TEMPLATE			= 0x0008;
const long ET_DRAG_OBJTYPE_CD				= 0x0010;
const long ET_DRAG_OBJTYPE_MARK				= 0x0020;

// 拖拽来源
typedef long ET_DRAG_SOURCE_TYPE;
const long ET_DRAG_SOURCE_UNKNOWN       = 0x00000;      // 未知
const long ET_DRAG_SOURCE_EDITMAX		= 0x01000;		// 非编
const long ET_DRAG_SOURCE_PROJECTEXPOLOR= 0x01100;		// PE
const long ET_DRAG_SOURCE_MEDIA			= 0x02000;		// 媒资
const long ET_DRAG_SOURCE_INGEST		= 0x03000;		// 收录
const long ET_DRAG_SOURCE_STUDIO		= 0x04000;		// 演播室
const long ET_DRAG_SOURCE_CHANNEL		= 0x05000;		// 频道播出
const long ET_DRAG_SOURCE_ARCHIVE       = 0X06000;      // ML的Archive节点对象
const long ET_DRAG_SOURCE_ARCHIVE_REMOTE= 0X06001;      // 远程Archive站点
const long ET_DRAG_SOURCE_NETWORK       = 0X06100;      // ML的网络库节点对象
const long ET_DRAG_SOURCE_REMOTE	    = 0X06200;      // ML的远程库节点对象
const long ET_DRAG_SOURCE_LOCAL         = 0X06300;      // ML的本地库库节点对象
const long ET_DRAG_SOURCE_XDA           = 0X07000;      // XDA对象


//网络E7非编需要的锁
enum em_ETLockType
{
	ET_emObject						=	4000,		//object锁定
	ET_emObjEdit					=	4010,		//obj编辑锁定
};

//对象类型
enum emObjectType
{
	ET_ObjType_Unknow		 = 0x00,			//Invalid
	ET_ObjType_SysTemplate	 = 0x001,			//系统模版
	ET_ObjType_PublicTemplate = 0x002,			//公共模版
	ET_ObjType_PrivateTemplate = 0x004,			//私有模版
	ET_ObjType_Folder		 = 0x010,			//文件夹
	ET_ObjType_Clip			 = 0x020,			//素材
	ET_ObjType_Pgm			 = 0x040,			//节目
	ET_ObjType_Template		 = 0x080,			//模板
	ET_ObjType_EDL			 = 0x100,			//EDL
	ET_ObjType_CD			 = 0x200,			//光盘
	ET_ObjType_Event		 = 0x400,			//EventData（解析时临时使用）
	ET_ObjType_Plan			 = 0x800,			//增加planType 用于新建plan
	ET_ObjType_Mark			 = 0x1000,          //Mark类型 用于标记点搜索拖拽
};
//{{Add by Liu Jia 2008-11-27
//文件夹类型
enum emFolderType
{
	ET_ObjType_PublicFolder  = 0x00, //Public文件夹
	ET_ObjType_ArchiveFolder = 0x01, //ARCHIVE文件夹
	ET_ObjType_OAFolder		 = 0x02, //OA文件夹
	ET_ObjType_PEFolder      = 0x04, //PE文件夹
	ET_ObjType_UnknownFolder = 0x10, //未知文件夹  
};
//}}
enum emOriginType
{
	ET_OriginType_All		= 0x00, //All;
	ET_OriginType_Local		= 0x01, //Local Material 
	ET_OriginType_Remote	= 0x02, //Materials from remote site
};
enum EM_OA_DARG_ACCEPT_MODE
{
	ET_OA_DARG_MODE_PGM			= 1,//节目拖拽到空素材，需要发送合成
	ET_OA_DARG_MODE_CLIP		= 2,//素材拖拽到空素材，需要发送转码
	ET_OA_DARG_MODE_CLIP_NULL	= 4,//普通素材拖拽，
};
// 素材子类型
#define	ET_CLIPTYPE_HIDE		0x00	// 隐藏素材
#define	ET_CLIPTYPE_AV			0x01	// 普通视音频素材
#define	ET_CLIPTYPE_V			0x02	// 普通视频素材
#define	ET_CLIPTYPE_A			0x04	// 普通音频素材 
#define	ET_CLIPTYPE_XDCAM		0x08	// 蓝光素材
#define	ET_CLIPTYPE_TGA			0x10	// TGA素材
#define	ET_CLIPTYPE_IMAGE		0x20	// 图片
#define	ET_CLIPTYPE_P2			0x40	// P2素材
#define	ET_CLIPTYPE_SGCLIP		0x80	// SG素材，彩条素材
#define ET_CLIPTYPE_OA_EMPTYCLIP	0x000200	//OA FOLDER中的空素材
#define ET_CLIPTYPE_EX			0x400	//EX素材
#define ET_CLIPTYPE_SYSUDF      0x800	//XDCAM UDF素材

// 文件夹子类型,最高位0表示为MaterialList,1表示为Project
#define ET_SUBTYPE_FOLDER_ML_PUBLIC				0x000001
#define ET_SUBTYPE_FOLDER_ML_ARCHIVE			0x000002
#define ET_SUBTYPE_FOLDER_ML_SEARCHING_RESULT	0x000004
#define ET_SUBTYPE_FOLDER_ML_TRASH_CAN			0x000008
#define ET_SUBTYPE_FOLDER_ML_SPECIAL_FOLDER		0x000010
#define ET_SUBTYPE_FOLDER_SITE_ROOT             0x000020           //站点根目录,xsc,2007.7.14
#define ET_SUBTYPE_FOLDER_ML_OA_FOLDER		    0x000040
#define ET_SUBTYPE_FOLDER_ML_OA_STUDIO			0x000080
#define ET_SUBTYPE_FOLDER_ML_OA_MATRIAL			0x000100
#define ET_SUBTYPE_FOLDER_ML_OA_UTILITY			0x000200           //OA STUDIO 下 Utility Folder

//#define ET_CLIPTYPE_OA_EMPTYCLIP		0x000200

#define ET_SUBTYPE_FOLDER_PJ_GROUP				0x100001
#define ET_SUBTYPE_FOLDER_PJ_PROJECT			0x100002
#define ET_SUBTYPE_FOLDER_PJ_BIN				0x100004
#define ET_SUBTYPE_FOLDER_PJ_TRASH				0x100008
#define ET_SUBTYPE_FOLDER_PJ_ROOT				0x100010
#define ET_SUBTYPE_FOLDER_PJ_SEARCHING_RESULT	0x100020
#define ET_SUBTYPE_FOLDER_PJ_SYSTRASH			0x100040

//节目子类型
#define ET_SUBTYPE_PGM_TL						0x000001		//普通时间线
#define ET_SUBTYPE_PGM_CG_TEMPLATE				0x000002		//字幕时间线
#define ET_SUBTYPE_PGM_EDL						0x000003		//EDL时间线


// 素材的信号来源
typedef		ULONGLONG ET_CLIP_SOURCE_TYPE;          //信号来源
#define	 	ET_CLIP_SOURCE_TYPE_UNKNOW	0x00	// 未知信号
#define	 	ET_CLIP_SOURCE_TYPE_VTR		0x01	// VTR采集
#define	 	ET_CLIP_SOURCE_TYPE_DUB		0x02	// 配音采集
#define	 	ET_CLIP_SOURCE_TYPE_XDCAM	0x04	// 蓝光
#define	 	ET_CLIPSOURCE_P2			0x08	// P2
#define	 	ET_CLIPSOURCE_ILINK			0x10	// iLink
#define	 	ET_CLIPSOURCE_SGCLIP		0x20	// 彩条信号发生器
#define		ET_CLIPSOURCE_CDAUDIO		0x40
#define		ET_CLIP_SOURCE_TYPE_IMPORT	0x80	// 引入文件
#define	 	ET_CLIP_SOURCE_TYPE_VANOVTR	0x00000100
#define	 	ET_CLIP_SOURCE_TYPE_FILE	0x00000200
#define	 	ET_CLIP_SOURCE_TYPE_COMPILETL	0x00000400
#define	 	ET_CLIP_SOURCE_TYPE_INTRODEUCE	0x00000800
#define	 	ET_CLIP_SOURCE_TYPE_COMPILECLIP	0x00001000
#define	 	ET_CLIP_SOURCE_TYPE_LIVECLIPS	0x00002000
#define	 	ET_CLIP_SOURCE_TYPE_SPEEDCLIPS	0x00004000
#define	 	ET_CLIP_SOURCE_TYPE_MXFCLIPS	0x00008000
#define	 	ET_CLIP_SOURCE_TYPE_TITLECLIPS	0x00010000
#define		ET_CLIP_SOURCE_TYPE_SERVERCLIPS ET_CLIP_SOURCE_TYPE_TITLECLIPS
#define	 	ET_CLIP_SOURCE_TYPE_IMPORTCOMPILE	0x00020000
#define	 	ET_CLIP_SOURCE_TYPE_DIGITIZEH	0x00040000
#define	 	ET_CLIP_SOURCE_TYPE_DIGITIZEL	0x00080000
#define	 	ET_CLIP_SOURCE_TYPE_DIGITIZE (ET_CLIP_SOURCE_TYPE_DIGITIZEH|ET_CLIP_SOURCE_TYPE_DIGITIZEL)
#define	 	ET_CLIP_SOURCE_TYPE_SEQUENCE	0x00100000
#define	 	ET_CLIP_SOURCE_TYPE_SUBCLIPS	0x00200000
#define	 	ET_CLIP_SOURCE_TYPE_MP4     	0x00400000
#define		ET_CLIP_SOURCE_TYPE_BRDNEWCLIP	0x00800000
#define		ET_CLIP_SOURCE_TYPE_XDCAMEX	0x01000000
#define		ET_CLIP_SOURCE_TYPE_DIGEQUIPMENT (ET_CLIP_SOURCE_TYPE_XDCAM|ET_CLIP_SOURCE_TYPE_XDCAMEX|ET_CLIPSOURCE_P2)
#define		ET_CLIP_SOURCE_TYPE_HDXEXCHANGE 0x02000000
#define		ET_CLIP_SOURCE_TYPE_LOGGING		0x04000000
//xw 2009/12/23 应NLE要求增加定义
#define     ET_CLIP_SOURCE_TYPE_TITLEMOTIONANI   0x08000000   //TitleMotionAn
#define     ET_CLIP_SOURCE_TYPE_TITLEMOTIONSTA   0x10000000   //TitleMotionAn
#define     ET_CLIP_SOURCE_TYPE_LTC              0x20000000   //对应单机定义
#define     ET_CLIP_SOURCE_TYPE_VITC             0x40000000   //对应单机定义
#define     ET_CLIP_SOURCE_TYPE_CTL              0x80000000   //刚好用完32位了，再加就麻烦了
//xw 20011/2/14 应NLE要求增加定义
#define     ET_CLIP_SOURCE_TYPE_DVD              0x0000000100000000//DVD素材
#define     ET_CLIP_SOURCE_TYPE_SYSUDF			 0x0000000200000000//XDCAM UDF素材





//节目保存过滤
typedef		ULONGLONG ET_PGMMASK_TYPE;          //信号来源
#define	ET_PGMMASK_BASE				0x00000001			//OBJECT对象中的基本数据
#define	ET_PGMMASK_EXTINFO			0x00000002			//其他字段
#define	ET_PGMMASK_ALL				0x0000000F			//全部信息

//目录保存过滤
typedef		ULONGLONG ET_FOLDER_TYPE;          //信号来源
#define	ET_FOLDE_BASE				0x00000001			//OBJECT对象中的基本数据
#define	ET_FOLDE_EXTINFO			0x00000002			//其他字段
#define	ET_FOLDER_ALL				0x0000000F			//全部信息

//素材模块划分
typedef		ULONGLONG ET_CLIPMASK_TYPE;					//素材模块划分
#define	ET_CLIPMASK_BASE			0x00000001			//基本数据信息
#define	ET_CLIPMASK_CLIPINFO		0x00000002			//元数据信息
#define	ET_CLIPMASK_FILEINFO		0x00000004			//文件信息
#define	ET_CLIPMASK_MARKPOINT		0x00000008			//标记点信息
#define	ET_CLIPMASK_PLANNING		0x00000010			//计划信息
#define	ET_CLIPMASK_INGEST			0x00000020			//收录信息
#define ET_CLIPMASK_ORGMETADATA		0x00000040			//OrgMetaData
#define ET_CLIPMASK_EVENTMETADATA	0x00000080			//EventMetaData
#define ET_CLIPMASK_EXTDATA1		0x00000100			//非编用到的扩展数据
#define	ET_CLIPMASK_ALL				0x000000FF			//全部信息


//标记点类型定义
#define ET_MARKPOINT_KEYFRAME				0x000001		//关键帧
#define ET_MARKPOINT_SIMPLE					0x000002		//一般标记点
#define ET_MARKPOINT_SEGMENT				0x000004		//标记段
#define ET_MARKPOINT_ESSENCEMASK			0x000008		//EssenceMask
#define ET_MARKPOINT_CHANGEMASK			    0x0000010		//ChangeMask

// 素材的入库来源
#define	 	ET_CLIPSOURCE_CREATE	0	// 非编生成
#define	 	ET_CLIPSOURCE_NLE		1	// 非编采集上载
#define	 	ET_CLIPSOURCE_NLE_DUB	2	// 非编配音上载
#define	 	ET_CLIPSOURCE_IMPORT	3	// 非编引入素材
#define	 	ET_CLIPSOURCE_STUDIO	4	// 演播室回采
#define	 	ET_CLIPSOURCE_MPC		5	// MPC 入库
#define	 	ET_CLIPSOURCE_UPLOAD	7	// 独立上载
#define	 	ET_CLIPSOURCE_RECORD	9	// 收录入库
#define	 	ET_CLIPSOURCE_MAM		11	// 媒资入库
#define	 	ET_CLIPSOURCE_MAM_MOVE	12	// 媒资回迁
#define		ET_CLIPSOURCE_ML_IMPORT	13	// ML引入素材
#define     ET_CLIPSOURCE_XDA_RETRIEVE 14 //XDA回迁  

// 素材的公开状态0-公开,1-私有,2-本组,3-条件
#define		ET_PUBLIC_STATE_ALL		0	// 公开
#define		ET_PUBLIC_STATE_PRIVATE	1	// 私有
#define		ET_PUBLIC_STATE_GROUP	2	// 本组
#define		ET_PUBLIC_STATE_SELECT	3	// 条件

typedef long  ET_CLIP_CAPTURESTATUS;			//素材采集状态
#define  ET_CLIP_CAPTURESTATUS_NORMAL			0x00000000		//为正常状态
#define  ET_CLIP_CAPTURESTATUS_CAPTURE			0x00000001		//为正在采集状态
#define  ET_CLIP_CAPTURESTATUS_LOOPCAPTURE		0x00000002		//为循环采集状态
#define  ET_CLIP_CAPTURESTATUS_CREATELOW		0x00000010		//为素材生成低码率状态
#define  ET_CLIP_CAPTURESTATUS_CLIPTRIM			0x00000020		//为素材TRIM过程中的状态
#define  ET_CLIP_CAPTURESTATUS_RENDERING		0x00000040		//为节目合成状态
#define  ET_CLIP_CAPTURESTATUS_TRANSCODEING		0x00000080		//为素材转码状态
#define  ET_CLIP_CAPTURESTATUS_XDCAMUPLOW		0x00000100		//为蓝光上载完低码率状态
#define  ET_CLIP_CAPTURESTATUS_XDCAMUPHIGHT		0x00000200		//为蓝光上载完高码率状态
#define  ET_CLIP_CAPTURESTATUS_XDCAMCREATELOW	0x00000400		//为蓝光上载完高码率音频转码MP3完成状态
#define  ET_CLIP_CAPTURESTATUS_COPYING			0x00000800		//复制中的素材
#define  ET_CLIP_CAPTURESTATUS_RENDERPGMTOOA	0x00001000		//新建的空素材，用于非编MASTERVIEW拖拽到此素材上合成节目
#define  ET_CLIP_CAPTURESTATUS_FROM3RD			0x00002000		//正在从第三方导入素材
#define  ET_CLIP_CAPTURESTATUS_TRANSMITTING     0x00004000      //正在从其它系统传输
#define  ET_CLIP_CAPTURESTATUS_TRANSMIT_FINISHED       0x00008000      //素材传输完成
#define  ET_CLIP_CAPTURESTATUS_NLE_RENDERING	0x00010000		//非编本地Render的素材,正在Render时,Clip Status为NLE Rendering
#define  ET_CLIP_CAPTURESTATUS_FAILEDFROM3RD    0x00020000      //从第三方导入素材失败，此状态素材的操作和ET_CLIP_CAPTURESTATUS_FROM3RD一样



typedef ULONGLONG  ET_CLIP_IMAGETYPE;				//素材文件状态
#define  ET_CLIP_IMAGETYPE_ORIGINAL			0x0000000000000000		//为素材原始比例
#define  ET_CLIP_IMAGETYPE_4_3				0x0000000000000001		//图像为4:3的方式
#define  ET_CLIP_IMAGETYPE_16_9SD			0x0000000000000002		//图像为16:9的方式，16:9SD
#define  ET_CLIP_IMAGETYPE_16_9HD			0x0000000000000004		//图像为16:9的方式，16:9HD
#define  ET_CLIP_IMAGETYPE_NG				0x0000000000000008
#define  ET_CLIP_IMAGETYPE_LOGGING			0x0000000000000010
#define  ET_CLIP_IMAGETYPE_16_9SQ			0x0000000000000020
#define  ET_CLIP_IMAGETYPE_SPEEDRES			0x0000000000000040
#define  ET_CLIP_IMAGETYPE_NOSAVEAUDIOCLIPCONTROL			0x0000000000000080
#define  ET_CLIP_IMAGETYPE_16_9SG			0x0000000000000100
#define  ET_CLIP_IMAGETYPE_16_9				(ET_CLIP_IMAGETYPE_16_9SQ| ET_CLIP_IMAGETYPE_16_9SG| ET_CLIP_IMAGETYPE_16_9SD)
#define  ET_CLIP_IMAGETYPE_LOCKVIDEOSTAND   0x0000000000000200 
#define  ET_CLIP_IMAGETYPE_ABRIDGEH         0x0000000000000400
#define  ET_CLIP_IMAGETYPE_ABRIDGEL         0x0000000000000800
#define  ET_CLIP_IMAGETYPE_ABRIDGE			(ET_CLIP_IMAGETYPE_ABRIDGEH | ET_CLIP_IMAGETYPE_ABRIDGEL)
#define  ET_CLIP_IMAGETYPE_SUBHIGH 			0x0000000000001000
#define  ET_CLIP_IMAGETYPE_RENDERCG			0x0000000000002000

//imagetype 新增素材高低质量标志位Sonaps加了半高半低之类的判断，需要把未定义完全的情况定义了
#define  ET_CLIP_IMAGETYPE_FULLHIGH         0x0000000000100000    //
#define  ET_CLIP_IMAGETYPE_NOHIGH           0x0000000000200000   //
#define  ET_CLIP_IMAGETYPE_FULLLOW          0x0000000000400000   //
#define  ET_CLIP_IMAGETYPE_SUBLOW           0x0000000000800000   //
#define  ET_CLIP_IMAGETYPE_NOLOW            0x0000000001000000   //

//{{2010/4/22 niejiagang:应非编的要求，增加以下三个宏定义
#define ET_CLIP_IMAGETYPE_GROUPCLIPXDCAM	0x0000000002000000 
#define ET_CLIP_IMAGETYPE_GROUPCLIPPEV		0x0000000004000000
#define ET_CLIP_IMAGETYPE_GROUPCLIPPEH		0x0000000008000000
//}}
//{{2010/9/06 niejiagang:应非编的要求，增加以下六个宏定义
#define ET_CLIP_IMAGETYPE_P2NATIVE			0x0000000010000000 // P2 Native
#define ET_CLIP_IMAGETYPE_STILLPICTURE      0x0000000020000000 //Still Picture
#define ET_CLIP_IMAGETYPE_SCENECLIPFORMSV   0x0000000040000000 //SV中Make Scene In/Out
#define ET_CLIP_IMAGETYPE_EXAVI             0x0000000080000000 //EX 盘中的AVI文件
#define ET_CLIP_IMAGETYPE_CREATETIME        0x0000000100000000 // 素材是否有采集时间
#define ET_CLIP_IMAGETYPE_P2TRANSCODE		0x0000000200000000 // P2 TransCode
//}}
//{{2010/9/21 niejiagang:应非编的要求，增加以下几个宏定义
#define ET_CLIP_IMAGETYPE_ZOOMPROXY				0x0000000400000000
#define ET_CLIP_IMAGETYPE_NOADPTVIDEOSTAND		0x0000000800000000
#define ET_CLIP_IMAGETYPE_LEVELCONVERSION		0x0000001000000000 // level conversion
#define ET_CLIP_IMAGETYPE_SUBCLIPFROMSV			0x0000002000000000 // level conversion
#define ET_CLIP_IMAGETYPE_RENDERTLDEFAULTSET	0x0000004000000000 //时间线RenderToBin是否使用默认设置
#define ET_CLIP_IMAGETYPE_STILLFRAME			0x0000008000000000 //是否静帧素材 静帧静帧静帧静帧
#define ET_CLIP_IMAGETYPE_FIXEDASPECT			0x0000010000000000 //是否是固定宽高比
#define ET_CLIP_IMAGETYPE_VIRUALSPEEDRES		0x0000020000000000 //虚拟变速
#define ET_CLIP_IMAGETYPE_MEDIAOFFLINE			0x0000040000000000 //Media Off Line

//}}




//素材归档状态定义
typedef long  ET_CLIP_ARCHIVE;				//素材归档状态
#define  ET_CLIP_ARCHIVE_NORMAL				0x00000000		//素材处于非归档状态
#define  ET_CLIP_ARCHIVE_SUBMIT				0x00000001		//素材处于提交归档状态
#define  ET_CLIP_ARCHIVE_FINISHED			0x00000002		//素材处于归档完成状态
#define  ET_CLIP_ARCHIVE_FAILED				0x00000003		//素材处于归档失败状态
#define  ET_CLIP_ARCHIVE_SCHEDULE           0X00000004      //素材处于计划归档状态

#define  ET_CLIP_ARCHIVE_MAM				0x00000010		//素材存在于媒资中
#define  ET_CLIP_ARCHIVE_RETRIEVE_SUBMIT	0x00000011		//素材处于提交回迁状态
#define  ET_CLIP_ARCHIVE_RETRIEVE_FINISHED	0x00000012		//素材处于回迁完成状态
#define  ET_CLIP_ARCHIVE_RETRIEVE_FAILED	0x00000013		//素材处于回迁失败状态
#define  ET_CLIP_ARCHIVE_MAM_3RD            0x00000014      //素材下于第三方媒资下
#define  ET_CLIP_ARCHIVE_RETRIEVE_DOING     0x00000015      //该素材正在回迁之中, 不用再发回迁

//素材特殊归档状态定义（Tobe Archive + Not auto-Archive）
#define	 ET_CLIP_ARCHIVE_SPECIAL_NONE					0x00000000 //没有设置状态
#define	 ET_CLIP_ARCHIVE_SPECIAL_TOBE_ARCHIVE_ONLY		0x00000001 //素材处于即将归档状态
#define	 ET_CLIP_ARCHIVE_SPECIAL_NOT_AUTOMATICALLY_ONLY	0x00000002 //素材处于不允许自动归档状态
#define	 ET_CLIP_ARCHIVE_SPECIAL_ALL					0x00000003 //素材同时处于即将归档和不允许自动归档状态
#define  ET_CLIP_ARCHIVE_SPECIAL_TOBE_RETRIEVE_ONLY     0x00000004 //处于媒资库中的素材打上的即将回迁标志,与前面的四种状态互斥.

//文件夹属性页里，To be Archive和Not to be Auto－archive 设置保存的定义
#define ET_FOLDER_ARCHIVE_TOBE_ARCHIVE_ONLY_MODIFY_TRUE			0x00000010	//只对TOBE_ARCHIVE设置为TRUE，其他不做修改
#define ET_FOLDER_ARCHIVE_TOBE_ARCHIVE_ONLY_MODIFY_FALSE		0x00000011	//只对TOBE_ARCHIVE设置为FALSE，其他不做修改
#define ET_FOLDER_ARCHIVE_NOT_AUTOMATICALLY_ONLY_MODIFY_TRUE	0x00000012	//只对Not to be autoarchive设置为TRUE，其他不做修改
#define ET_FOLDER_ARCHIVE_NOT_AUTOMATICALLY_ONLY_MODIFY_FALSE	0x00000013	//只对Not to be autoarchive设置为FALSE，其他不做修改


//素材文件质量状态
#define ET_Obj_FS_Unknow	0x00000000
#define ET_Obj_FS_HV_ALL  	0x00000001
#define ET_Obj_FS_HV_SEG  	0x00000002
#define ET_Obj_FS_LV_ALL	0x00000004
#define ET_Obj_FS_LV_SEG	0x00000008
#define ET_Obj_FS_MV_ALL	0x00000010
#define ET_Obj_FS_MV_SEG	0x00000020
#define ET_Obj_FS_HA_ALL	0x00000040
#define ET_Obj_FS_HA_SEG	0x00000100
#define ET_Obj_FS_LA_ALL	0x00000200
#define ET_Obj_FS_LA_SEG	0x00000400
#define ET_Obj_FS_MA_ALL	0x00000800
#define ET_Obj_FS_MA_SEG	0x00001000
#define ET_Obj_FS_EDL		0x00002000
#define ET_Obj_FS_Pgm		0x00004000
#define ET_Obj_FS_MXF		0x00008000	//表示素材是MXF素材
#define ET_Obj_FS_H264		0x00010000	//表示素材已生成了H.264文件。
#define ET_Obj_FS_H264_Creating 0x00020000	//表示素材正在生成了H.264文件。

// 素材文件状态定义
#define ET_ClipFileState_Unknown       -1
#define ET_ClipFileState_NormalOnline   0
#define ET_ClipFileState_Ingesting      1
#define ET_ClipFileState_NearLine       2
#define ET_ClipFileState_OffLine        3

//站点类型定义
typedef long ETVML_PLUGIN_TYPE;
#define ETVML_PLUGIN_TYPE_NETWORK			0X01             //网络(中心存储)站点
#define ETVML_PLUGIN_TYPE_LOCAL				0X02             //本地库
#define ETVML_PLUGIN_TYPE_REMOTE			0X04             //远程站点
#define ETVML_PLUGIN_TYPE_ARCHIVE			0X08             //媒资站点
#define ETVML_PLUGIN_TYPE_REMOTEARCHIVE		0X10             //远程媒资站点
#define ETVML_PLUGIN_TYPE_VSEARCHLIST		0X20             //虚拟的公共的SearchResult

//终端类型
//0：未知，1：为ML站点终端，2：为PE站点终端
typedef long ET_TERMINAL_TYPE;
#define ET_TERMINAL_TYPE_UNKNOW 0
#define ET_TERMINAL_TYPE_ML		1
#define ET_TERMINAL_TYPE_PE		2

//数据库的站点ID
//0：为主数据库或者缺省数据库，1：为本地数据库的站点ID，其他：为指定的数据库的站点ID
#define ET_DATABASE_ID_DEFAULT	0
#define ET_DATABASE_ID_LOCAL	1


#define	ET_OBJECT_POPEDOMTYPE_READ		1		// 访问权限
#define	ET_OBJECT_POPEDOMTYPE_MODIFY	2		// 修改权限

typedef enum emObjectPopedomType
{	
	ET_OBJECT_POPEDOM_PUBLIC		 = 0,		// 公共
	ET_OBJECT_POPEDOM_PRIVATE		 = 1,		// 私有
	ET_OBJECT_POPEDOM_GROUP			 = 2,		// 本组
	ET_OBJECT_POPEDOM_CONDITION      = 3		// 自定义
}ET_OBJECT_POPEDOM;

#define ETMAKEULONGLONG(a,b)	((ULONGLONG)(((DWORD)(a)) | ((ULONGLONG)((DWORD)(b))) <<32))

//媒体类型枚举定义
enum emMediaFormatDefType
{
	ET_emMediaFmt_Unknow = 0,	//未知类型
	ET_emMediaFmt_Video  = 1,	//视频
	ET_emMediaFmt_Audio  = 2,	//音频
	ET_emMediaFmt_Key	 = 3,	//键
	ET_emMediaFmt_Picture= 4	//图片
};

//对象恢复选项
enum ET_Object_Resume_Type
{
	ET_Object_Resume_ValidateOnly				= 0,			//仅仅是效验对象是否被删除
	ET_Object_Resume_TrashCanOnly				= 1,			//仅仅恢复被删除到回收站的对象，对所有对象有效
	ET_Object_Resume_All						= 2,			//可以恢复所有的数据，对于目录可以恢复出被彻底删除的数据
};
//获取对象路径选项
enum ET_Object_GetPath_Type
{
	ET_Object_GetPath_Normal					= 0,			//获取正常对象的路径,即被删除的对象将取不到路径
	ET_Object_GetPath_Really					= 1,			//获取对象的真正路径,包括回收站在内;
	ET_Object_GetPath_Original					= 2,			//获取对象的避开回收站后的原始路径,可用于对象恢复
	ET_Object_GetPath_All						= 3,			//获取对象的原始路径,即使对象被侧底删除
	ET_Object_GetPath_WithSiteName              = 4,            //返回含站点名称的路径
};

enum ET_NM_OBJECT_TYPE
{
	// 频道节点
	ET_NM_OBJECT_TYPE_CHANNEL_NODE				= 3, 
	// 频道
	ET_NM_OBJECT_TYPE_CHANNEL					= 4, 
	// 栏目节点
	ET_NM_OBJECT_TYPE_COLUMN_NODE				= 5, 
	// 栏目
	ET_NM_OBJECT_TYPE_COLUMN					= 6, 
};
/////////////////////////////mosServer插件处理的定义们////////////////////////////////////////////////
enum ServerMsg_ObjectStatus
{
	ServerMsg_ObjectStatus_Add			=	0x00000005,		//前3个为ml内部状态
	ServerMsg_ObjectStatus_Update		=	0x00000006,
	ServerMsg_ObjectStatus_Delete		=	0x00000007,
	ET_INGEST_STATUS_FILLING_STARTED	=	0x00000001,		//Recording of a Media Object has started
	ET_INGEST_STATUS_FILLING_CANCELED	=	0x00000002,		//Recording of a Media Object has been canceled
	ET_INGEST_STATUS_FILLING_ERROR		=	0x00000004,	    //Recording of a Media Object has been stopped because of error
	ET_INGEST_STATUS_FINISHED			=	0x00000008,		//Recording of a Media Object has been finished
	ServerMsg_ObjectStatus_AddPlan      =   0x00000009,
	ServerMsg_ObjectStatus_UpdatePlan   =   0x00000010,
	ServerMsg_ObjectStatus_DeletePlan	=   0x00000011,
	ServerMsg_ObjectStatus_AddPlan_1	=   0x00000012
};
typedef long ET_EMPTY_PLAN_STATUS;								//空plan状态
#define	ET_EMPTY_PLAN_CREATE				=   0x00000010,		//空素材创建
#define	ET_EMPTY_PLAN_MOVE_TRASH			=   0x00000011,		//空素材移动到回收站
#define	ET_EMPTY_PLAN_MOVE					=   0x00000012,		//空素材移动到普通文件夹
#define	ET_EMPTY_PLAN_DELETE				=   0x00000013,		//空素材彻底删除


//！节目状态定义
typedef DWORD ETPgmStatus;
#define ET_PGM_STATUS_EDIT				0					//编辑状态													//对应的PGMSTATUS图标位置29
#define ET_PGM_STATUS_SUBMIT			10					//提交节目						节目初编完成				//对应的PGMSTATUS图标位置31
#define ET_PGM_STATUS_JUNIORCENSOR		20					//初级审查结束					节目初编完成				//对应的PGMSTATUS图标位置31
#define ET_PGM_STATUS_MIDDLECENSOR		30					//中级审查结束，提交合成		节目精编完成，配音、字幕编辑结束	//对应的PGMSTATUS图标位置32
#define ET_PGM_STATUS_COMPOSING			40					//正在合成													//对应的PGMSTATUS图标位置33
#define ET_PGM_STATUS_COMPOSEFAIL		45					//合成失败													//对应的PGMSTATUS图标位置44
#define ET_PGM_STATUS_COMPOSED			50					//合成完毕				需要审片的合成结束，等待审片		//对应的PGMSTATUS图标位置34
#define ET_PGM_STATUS_COMPOSELOW		60					//完成低码率合成		MPEG4格式							//对应的PGMSTATUS图标位置35
#define ET_PGM_STATUS_AUTOTECHCENSOR	70					//完成自动技审												//对应的PGMSTATUS图标位置35
#define ET_PGM_STATUS_SENIORCENSOR		80					//节目审查通过												//对应的PGMSTATUS图标位置36

//！节目辅助状态定义
typedef DWORD ETPgmSecStatus;
#define ET_PGM_SECSTATUS_NODUB			0					//未配音													//对应的PGMSTATUS图标位置36
#define ET_PGM_SECSTATUS_DUB			10					//配音完成													//对应的PGMSTATUS图标位置37

//!文稿状态定义
typedef DWORD ETNewsStatus;
#define ET_NEWS_STATUS_EDIT             1                   //正在写稿
#define ET_NEWS_STATUS_WAITFIRSTCENSOR  3                   //等待初审
#define ET_NEWS_STATUS_WAITSECENDCENSOR 4					//等待终审
#define ET_NEWS_STATUS_PASS             5                   //通过审查

//!串联单
typedef DWORD ETPlayListStatus;
#define ET_PL_STATUS_EDIT             1                   //正在写稿
#define ET_PL_STATUS_WAITFIRSTCENSOR  3                   //等待初审
#define ET_PL_STATUS_WAITSECENDCENSOR 4					//等待终审
#define ET_PL_STATUS_PASS             5                   //通过审查


//错误号定义
#define ETErrorNum		 					int
//error num list
#define ET_ERROR_NO_FAULT					0
#define ET_ERROR_CONDITION_INVALIDATION		1
#define ET_ERROR_QUERY_FAILED				2
#define ET_ERROR_RECORD_EMPTY				3
#define ET_ERROR_UPDATE_FAILED				4
#define ET_ERROR_EXCEPTION					5
#define ET_ERROR_EXECUTE_FAILED				6
#define ET_ERROR_PARAMETER_INVALIDATION		7
#define ET_ERROR_INIT_FAILED				8
#define ET_ERROR_SEQUENCE_FAILED			9
#define ET_ERROR_INVALID_POPEDOM			10
// -- for clip trim save
#define ET_ERROR_CLIPSAVE_GIVEUP			11

//-- For find clip/sequence
#define  ET_ERROR_PARAM						1//参数错误
#define  ET_ERROR_NO_FOLDER					2//没有找到对应的文件夹
#define  ET_ERROR_NO_POPEDOM				4//没有权限
#define  ET_ERROR_NO_DATA					8//数据为空
#define  ET_ERROR_OPEN_FOLDER				16//打不开文件夹，文件夹被删除到回收站
#define  ET_ERROR_NO_OBJECT					32//目录找到了，CLIP/SEQUENCE可能被删除


// 保存操作掩码相关定义
// 掩码操作的宏
#define FIELDMASK(field)				((ULONGLONG)1 << (field-1))
#define FIELD_ISSET(_mask, _field)		( ( (_mask) & FIELDMASK(_field) ) == FIELDMASK(_field) )

// ETObject结构体中的字段掩码
enum MASK_ETOBJECT
{
	OBJ_FIELD_ObjectID			= 1,		//对象ID
	OBJ_FIELD_ObjectGuid		= 2,		//对象GUID
	OBJ_FIELD_ObjectType		= 3,		//对象类型
	OBJ_FIELD_SubType			= 4,		//对象子类型
	OBJ_FIELD_FolderID			= 5,		//文件夹ID
	OBJ_FIELD_PopdomID			= 6,		//权限ID
	OBJ_FIELD_CDID				= 7,		//光盘ID
	OBJ_FIELD_ObjectName		= 8,		//对象名称
	OBJ_FIELD_KeyWord			= 9,		//关键词
	OBJ_FIELD_IconFrame			= 10,		//图标帧
	OBJ_FIELD_IconFileName		= 11,		//图标文件
	OBJ_FIELD_Length			= 12,		//长度
	OBJ_FIELD_TrimIn			= 13,		//入点
	OBJ_FIELD_TrimOut			= 14,		//出点
	OBJ_FIELD_CreaterID			= 15,		//创建者ID
	OBJ_FIELD_CreateDate		= 16,		//创建日期
	OBJ_FIELD_MenderID			= 17,		//修改人ID	
	OBJ_FIELD_ModifyDate		= 18,		//修改日期
	OBJ_FIELD_DeleteFlag		= 19,		//删除标志
	OBJ_FIELD_DelaterID			= 20,		//删除人ID
	OBJ_FIELD_DelateDate		= 21,		//删除日期
	OBJ_FIELD_RecordDate		= 22,		//录制日期
	OBJ_FIELD_ShowDate			= 23,		//播出日期
	OBJ_FIELD_DistributeDate	= 24,		//发布日期
	OBJ_FIELD_DistributerID		= 25,		//发布人
	OBJ_FIELD_ArchiveDate		= 26,		//归档日期
	OBJ_FIELD_ArchiverID		= 27,		//归档日期
	OBJ_FIELD_ArchiveFlag		= 28,		//归档标志
	OBJ_FIELD_ColumnID			= 29,		//栏目ID
	OBJ_FIELD_VideoStandard		= 30,		//制式
	OBJ_FIELD_Status			= 31,		//状态
	OBJ_FIELD_UMID				= 32,		//对象的UMID
	OBJ_FIELD_UsedFlag			= 33,		//使用标志
	OBJ_FIELD_LockFlag			= 34,		//锁定标志
	OBJ_FIELD_FileStatus		= 35,		//文件状态
	OBJ_FIELD_Rights			= 36,		//版权信息
	OBJ_FIELD_JournalList		= 37,		//记者
	OBJ_FIELD_MediaChannel		= 38,		//媒体通道
	OBJ_FIELD_ObjectNote		= 39,		//对象备注
	OBJ_FIELD_Reserved1			= 40,		//备用数据1
	OBJ_FIELD_Reserved2			= 41,		//备用数据2
	OBJ_FIELD_Reserved3			= 42,		//备用数据3
	OBJ_FIELD_Reserved4			= 43,		//备用数据4
	OBJ_FIELD_CaptureStatus		= 44,		//采集状态
	OBJ_FIELD_ImageType			= 45,		//图像类型
	OBJ_FIELD_EditTerminal		= 46,		//编辑终端
	OBJ_FIELD_ModifyTerminal	= 47,		//修改终端
	OBJ_FIELD_DeviceGUID		= 48,		//设备GUID
	OBJ_FIELD_TobeArchiveState  = 49,		//特殊归档标志
	OBJ_FIELD_AccessDate		= 50,		//访问时间标志
	OBJ_FIELD_3rdArchiveFlag	= 51,		//3方归档标志
    OBJ_FIELD_DBEStreamChannel     = 52,       //DBE音频通道
	OBJ_FIELD_NTSCTCMode		= 53,		//时码计算模式
	OBJ_FIELD_MaterialID        = 54,      //2010/05/24 zxp:add MaterialID
	OBJ_FIELD_BKColor          = 55,      //背景颜色
	OBJ_FIELD_Reserved20		= 56,        //XDCAMCOMPILE备用
	OBJ_FIELD_StoryID			= 57,        //STORYID
	OBJ_FIELD_ResSourceGuid		= 58,	//源素材GUID
};

enum MASK_ETOBJECTEx
{
	OBJ_FIELDEX_Reserved5			= 1,		//备用数据5
	OBJ_FIELDEX_Reserved6			= 2,		//备用数据6
	OBJ_FIELDEX_Reserved7			= 3,		//备用数据7
	OBJ_FIELDEX_Reserved8			= 4,		//备用数据8
	OBJ_FIELDEX_Reserved9			= 5,		//备用数据9
	OBJ_FIELDEX_Reserved10		= 6,		//备用数据10
	OBJ_FIELDEX_Reserved11		= 7,		//备用数据11
	OBJ_FIELDEX_Reserved12		= 8,		//备用数据12
	OBJ_FIELDEX_Reserved13		= 9,		//备用数据13
	OBJ_FIELDEX_Reserved14		= 10,		//备用数据14
	OBJ_FIELDEX_XDAArchiveFlag  = 11,       //XDA归档标志
};

// ETClipInfo结构体中的字段掩码
enum MASK_ETCLIPINFO
{
	CLP_FIELD_SignalSourceType	= 1,		//采集信号类型
	CLP_FIELD_SignalSourceDesc	= 2,		//采集信号描述
	CLP_FIELD_SignalSourceMark	= 3,		//采集信号标签
	CLP_FIELD_VtrInPoint		= 4,		//磁带入点
	CLP_FIELD_VtrOutPoint		= 5,		//磁带出点
	CLP_FIELD_VtrInNext			= 6,		//磁带扩展入点
	CLP_FIELD_VtrOutNext		= 7,		//磁带扩展出点
	CLP_FIELD_StorageCode		= 8,		//存储编码
	CLP_FIELD_KeepDays			= 9,		//素材在非编库中的保留天数
	CLP_FIELD_ClipSource		= 10,		//素材的入库来源
	CLP_FIELD_IngestTaskID		= 11,		//收录任务ID
	CLP_FIELD_PlanningID		= 12,		//计划ID
	CLP_FIELD_TC				= 13,		//时码
	CLP_FIELD_Version			= 14,		//版本
	CLP_FIELD_ClipLanguage		= 15,		//语种
	CLP_FIELD_SerialData		= 16,		//TGA数据
	CLP_FIELD_SerialSize		= 17,		//TGA长度
	CLP_FIELD_IngestMetaData	= 18,		//收录元数据
	CLP_FIELD_PlanningMetaData	= 19,		//计划元数据
	CLP_FIELD_ClipFile			= 20,		//文件列表
	CLP_FIELD_ClipMarkPoint		= 21,		//素材标记点
	CLP_FIELD_SrcPgmGUID		= 22,		//源节目GUID
	CLP_FIELD_ItemName			= 23,
	CLP_FIELD_Category			= 24,
	CLP_FIELD_ProgramName       = 25,
	CLP_FIELD_SrcSiteID			= 26,
	CLP_FIELD_RestrictViewContent = 27,
	CLP_FIELD_OrgMetadata		= 28,
	CLP_FIELD_EventMetadata		= 29,
	CLP_FIELD_OriginalMaterial  = 30,       //2009/04/21 zxp:add 源文件名
};

// ETIngestMetaData结构体中的字段掩码
enum MASK_ETINGEST
{
	ING_FIELD_TaskID			= 1,		//任务ID
	ING_FIELD_TaskName			= 2,		//任务名称
	ING_FIELD_StartDate			= 3,		//收录开始时间
	ING_FIELD_EndDate			= 4,		//收录结束时间
	ING_FIELD_DurDate			= 5,		//收录保持时间
	ING_FIELD_Status			= 6,		//收录状态
	ING_FIELD_Process			= 7,		//收录进度
	ING_FIELD_SourceTapeID      = 8,        //Source Tape ID
	ING_FIELD_BackupTapeID      = 9,        //Backup Tape ID
	ING_FIELD_UserName			= 10,		//收录用户名
	ING_FIELD_Terminal			= 11,		//收录终端
	//{{2009/04/22 zxp:add 添加元数据
	ING_FIELD_ChannelName       = 12,
	ING_FIELD_DeviceID          = 13,
	ING_FIELD_AudioChannelCount = 14,
	ING_FIELD_SourceTapeBarcode = 15,
};


// ETPlanningMetaData结构体中的字段掩码
enum MASK_ETPLANNING{

	PLN_FIELD_PlanningID		= 1,		//计划ID
	PLN_FIELD_PlanningGUID		= 2,		//计划GUID
	PLN_FIELD_PlanName			= 3,		//计划名
	PLN_FIELD_CreatorName		= 4,		//创建人名
	PLN_FIELD_CreateDate		= 5,		//创建日期
	PLN_FIELD_ModifyName		= 6,		//修改人名
	PLN_FIELD_ModifyDate		= 7,		//修改日期
	PLN_FIELD_Version			= 8,		//版本
	PLN_FIELD_Place				= 9,		//地点
	PLN_FIELD_PlanningDate		= 10,		//计划日期
	PLN_FIELD_Director			= 11,		//导演
	PLN_FIELD_InputBy			= 12,		//录入
	PLN_FIELD_PhotoGrapher		= 13,		//摄影
	PLN_FIELD_Reporter			= 14,		//记者
	PLN_FIELD_Other				= 15,		//其他人员
	PLN_FIELD_Equipment			= 16,		//设备
	PLN_FIELD_ContactInfo		= 17,		//联系信息
	PLN_FIELD_PlanningXml		= 18,		//计划XML
	PLN_FIELD_InputSource		= 19,		//计划输入源
	PLN_FIELD_StartDate			= 20,		//计划收录开始时间
	PLN_FIELD_DurDate			= 21,		//计划收录保持时间
	PLN_FIELD_SourceType		= 22,       //采集源类型
	PLN_FIELD_IngestExtendXml	= 23,       //采集分段Planning信息
};

//{{2009/02/27 zxp:add ETOrgMetaData结构体中的字段掩码
enum MASK_ETORG{
	ORG_FIELD_ClipID            = 1,
	ORG_FIELD_UserMediaID       = 2,
	ORG_FIELD_MainTitle         = 3,
	ORG_FIELD_OrgComment        = 4,
	ORG_FIELD_ClipTitle         = 5,
	ORG_FIELD_Duration          = 6,
	ORG_FIELD_STC               = 7,
	ORG_FIELD_VideoFormat       = 8,
	ORG_FIELD_AudioFormat       = 9,
	ORG_FIELD_Description       = 10,
	ORG_FIELD_OrgUMID           = 11,
	ORG_FIELD_BasicUMID         = 12,
	ORG_FIELD_Status            = 13,
	ORG_FIELD_CreatorName       = 14,
	ORG_FIELD_CreateDate        = 15,
	ORG_FIELD_OrgAttribute      = 16,
};//}}

// ETTemplateObject结构体中的掩码
enum MASK_ETTEMPLATEOBJECT{
	TMP_FIELD_TypeGuid			= 1,		//模版类型GUID	
	TMP_FIELD_FilePath			= 2,		//模版引用的文件名	
	TMP_FIELD_Resource			= 3,		//模版来源类型
	TMP_FIELD_TempData			= 4,		//模版数据
	TMP_FIELD_TempSize			= 5,		//模版数据大小
};

// ETPgmMaster结构体中的掩码
enum MASK_PGMMASTER{
	PGM_FIELD_ClipPath			= 1,		//素材存放路径
	PGM_FIELD_VideoTrack		= 2,		//视频轨数
	PGM_FIELD_AudioTrack		= 3,		//音频轨数
	PGM_FIELD_GraphTrack		= 4,		//字幕轨数
	PGM_FIELD_BackTrack			= 5,		//背景轨数
	PGM_FIELD_AudioChannel		= 6,		//输出音频通道数
	PGM_FIELD_TLBlock			= 7,		//是否有子时间线
	PGM_FIELD_FilePath			= 8,		//文件路径
	PGM_FIELD_BackFilePath		= 9,		//备份文件路径
	PGM_FIELD_UndoFilePath		= 10,		//UNDO数据路径
	PGM_FIELD_VideoFormatID		= 11,		//视频格式ID
	PGM_FIELD_AudioFormatID		= 12,		//音频格式ID
	PGM_FIELD_SaveState			= 13,		//节目保存状态
	PGM_FIELD_EditTime			= 14,		//编辑累计时长
	PGM_FIELD_DubUserID			= 15,		//配音操作员ID
	PGM_FIELD_DubAnnouncerID	= 16,		//配音员ID
	PGM_FIELD_SECStatus			= 17,		//节目辅助状态
	PGM_FIELD_SysVersion		= 18,		//编辑系统版本
	PGM_FIELD_EditVersion		= 19,		//节目修改的版本号
};

// ETTemplateObject结构体中的掩码
enum MASK_FOLDERINFO{
	FOLDER_FIELD_TypeGuid			= 1,		//类型GUID
	FOLDER_FIELD_VideoFormatI		= 2,		//高视频格式ID
	FOLDER_FIELD_AudioFormatID		= 3,		//高音频格式ID
	FOLDER_FIELD_LVideoFormatID		= 4,		//低视频格式ID
	FOLDER_FIELD_LAudioFormatID		= 5,		//低音频格式ID
	FOLDER_FIELD_RHVideoFormatI		= 6,		//打包高视频格式ID
	FOLDER_FIELD_RHAudioFormatID	= 7,		//打包高音频格式ID
	FOLDER_FIELD_RLVideoFormatID	= 8,		//打包低视频格式ID
	FOLDER_FIELD_RLAudioFormatID	= 9,		//打包低音频格式ID
	FOLDER_FIELD_UseOriginalTC		= 10,		//使用原始时码ID
	FOLDER_FIELD_AudioChannel		= 11,		//输出音频通道数ID
};

//ETGTMAgentData结构体中的掩码
enum MASK_GTMAGENT{
	GTM_FIELD_OperationID           = 1,
	GTM_FIELD_OperationStatus       = 2,
	GTM_FIELD_SourceIPAddr          = 3,
	GTM_FIELD_SubSystem             = 4,
	GTM_FIELD_Connection            = 5,
	GTM_FIELD_CommandType           = 6,
	GTM_FIELD_ParaNodeType          = 7,
	GTM_FIELD_Result                = 8,
	GTM_FIELD_Info                  = 9,
};

//****************************************************************************************************************
//继承E7非编里面的定义数据
//****************************************************************************************************************
//资源类型定义
typedef  ULONGLONG					ET_RES_TYPE;
#define  ET_RES_UNKNOW              0x000000000
#define  ET_RES_CATALOG             0x000000001
#define  ET_RES_CLIP                0x000000002
#define  ET_RES_TLPROGRAM           0x000000004
#define  ET_RES_CGPROGRAM           0x000000008
#define  ET_RES_PROJECT             0x000000010
#define  ET_RES_VEFFECT             0x000000020
#define  ET_RES_AEFFECT             0x000000040
#define  ET_RES_CGEFFECT            0x000000080
#define  ET_RES_VTEMPLATE           0x000000100
#define  ET_RES_BTEMPLATE           0x000000200
#define  ET_RES_VTRTEMPLATE         0x000000400
#define  ET_RES_ATEMPLATE           0x000000800	
#define  ET_RES_ATRTEMPLATE         0x000010000	
#define  ET_RES_AMIXTEMPLATE        0x000020000	
#define  ET_RES_AOUTTEMPLATE        0x000040000	
#define  ET_RES_INTEMPLATE          0x000080000
#define  ET_RES_OUTTEMPLATE         0x000100000
#define  ET_RES_STAYTEMPLATE        0x000200000
#define  ET_RES_TTEMPLATE           0x000400000
#define  ET_RES_COLORPREFABRICATE   0x001000000
#define  ET_RES_PUBLICPREFABRICATE  0x002000000
#define  ET_RES_OBJECTPREFABRICATE  0x004000000
#define  ET_RES_CGTEMPLATE          0x0010000000
#define  ET_RES_CGFILTER            0x0020000000
#define  ET_RES_CGPREFABRICATE      0x0040000000
#define  ET_RES_HIDECLIP            0x0080000000
#define  ET_RES_CATALOGCHILD        0x0100000000
#define  ET_RES_CATALOGRES          0x0200000000
#define  ET_RES_VIEWLIST            0x0400000000
#define  ET_RES_RESNAME             0x0800000000
#define  ET_RES_CUT                 0x1000000000
#define  ET_RES_PROJECTRES          0x2000000000

#define  ET_RES_ALLCLIP			 ( ET_RES_CLIP|ET_RES_HIDECLIP)
#define  ET_RES_ALLPROGRAM       ( ET_RES_TLPROGRAM|ET_RES_CGPROGRAM)
#define  ET_RES_ALLVATEMPLATE    ( ET_RES_VTEMPLATE|ET_RES_BTEMPLATE|ET_RES_VTRTEMPLATE|ET_RES_AMIXTEMPLATE|ET_RES_AOUTTEMPLATE|ET_RES_ATEMPLATE|ET_RES_ATRTEMPLATE|ET_RES_INTEMPLATE|ET_RES_OUTTEMPLATE|ET_RES_STAYTEMPLATE|ET_RES_TTEMPLATE)
#define  ET_RES_ALLVAEFFECT      ( ET_RES_VEFFECT|ET_RES_AEFFECT)
#define  ET_RES_ALLCGTEMPLATE    ( ET_RES_CGTEMPLATE)
#define  ET_RES_ALLPREFABRICATE  ( ET_RES_CGPREFABRICATE | ET_RES_OBJECTPREFABRICATE | ET_RES_PUBLICPREFABRICATE | ET_RES_COLORPREFABRICATE)
#define  ET_RES_ALLTEMPLATE      ( ET_RES_ALLPREFABRICATE | ET_RES_ALLVATEMPLATE | ET_RES_ALLCGTEMPLATE)
#define  ET_RES_ALLVTEMPLATE     ( ET_RES_VTEMPLATE|ET_RES_BTEMPLATE|ET_RES_VTRTEMPLATE)
#define  ET_RES_ALLATEMPLATE     ( ET_RES_AMIXTEMPLATE|ET_RES_AOUTTEMPLATE|ET_RES_ATEMPLATE|ET_RES_ATRTEMPLATE)
#define  ET_RES_ALLTTEMPLATE     ( ET_RES_INTEMPLATE|ET_RES_OUTTEMPLATE|ET_RES_STAYTEMPLATE|ET_RES_TTEMPLATE)

#define  ET_RES_ALL                 0xFFFFFFFFFFFFFFFF


typedef DWORD  ET_CLIP_CLASS_TYPE;                //素材类型
#define  ET_CLIP_CLASS_UNKNOW 0x00000000
#define  ET_CLIP_CLASS_V      0x00000001
#define  ET_CLIP_CLASS_A1     0x00000002
#define  ET_CLIP_CLASS_A2     0x00000004
#define  ET_CLIP_CLASS_A3     0x00000008
#define  ET_CLIP_CLASS_A4     0x00000010
#define  ET_CLIP_CLASS_A5     0x00000020
#define  ET_CLIP_CLASS_A6     0x00000040
#define  ET_CLIP_CLASS_A7     0x00000080
#define  ET_CLIP_CLASS_A8     0x00000100

#define  ET_CLIP_CLASS_A      (ET_CLIP_CLASS_A1 | ET_CLIP_CLASS_A2 | ET_CLIP_CLASS_A3 | ET_CLIP_CLASS_A4 | ET_CLIP_CLASS_A5 | ET_CLIP_CLASS_A6 | ET_CLIP_CLASS_A7 | ET_CLIP_CLASS_A8)
#define  ET_CLIP_CLASS_ALL     0x0000FFFF

#define  ET_CLIP_CLASS_G      0x00040000
#define  ET_CLIP_CLASS_KEY    0x00080000
#define  ET_CLIP_CLASS_EFF    0x00100000

#define  ET_CLIP_QUALITY_MAX    5

typedef  DWORD  ET_CLIP_STRATEGY_TYPE;
#define  ET_CLIP_STRATEGY_HIGH   0
#define  ET_CLIP_STRATEGY_LOW    1
#define  ET_CLIP_STRATEGY_THIS   2

typedef  DWORD  ET_CLIP_QUALITY_TYPE;
#define  ET_CLIP_QUALITY_HIGH   0
#define  ET_CLIP_QUALITY_LOW    1
#define  ET_CLIP_QUALITY_MID    2
#define  ET_CLIP_QUALITY_SM		3

#define  ET_CLIP_QUALITY_ALL    ET_CLIP_QUALITY_MAX


#define  ET_CLIP_FILE_MAX    16
#define  ET_CLIP_FILE_VIDEO  0
#define  ET_CLIP_FILE_A1     1
#define  ET_CLIP_FILE_A2     2
#define  ET_CLIP_FILE_A3     3
#define  ET_CLIP_FILE_A4     4
#define  ET_CLIP_FILE_A5     5
#define  ET_CLIP_FILE_A6     6
#define  ET_CLIP_FILE_A7     7
#define  ET_CLIP_FILE_A8     8

#define  ET_CLIP_FILE_KEY    9    //键  
#define  ET_CLIP_FILE_GRAPH  10   //图文
#define  ET_CLIP_FILE_VBI    11   //

//制式标准枚举定义
enum emETVideoStandard
{	
	ET_VideoStandard_UNKNOW				= 0x00000000,		//Invalid

	ET_VideoStandard_PAL			    = 0x00000001,		//PAL size:720*576 f/s : 25
	ET_VideoStandard_NTSC_2997			= 0x00000002,		//NTSC size:720*486  f/s : 29.97
	ET_VideoStandard_NTSC_30			= 0x00000004,		//NTSC size:720*486 f/s : 30 
	ET_VideoStandard_SECAM				= 0x00000008,		//SECAM

	ET_VideoStandard_1920_1080_50i		= 0x00000010,		//HDTV size:1920*1080 f/s : 25  interlaced
	ET_VideoStandard_1920_1080_5994i    = 0x00000020,		//HDTV size:1920*1080 f/s : 29.97 interlaced
	ET_VideoStandard_1920_1080_60i		= 0x00000040,		//HDTV size:1920*1080 f/s : 30 interlaced

	ET_VideoStandard_1920_1080_2398p    = 0x00000080,		//HDTV size:1920*1080 f/s : 23.98 progressive
	ET_VideoStandard_1920_1080_24p		= 0x00000100,		//HDTV size:1920*1080 f/s : 24 progressive
	ET_VideoStandard_1920_1080_25p		= 0x00000200,		//HDTV size:1920*1080 f/s : 25 progressive
	ET_VideoStandard_1920_1080_2997p	= 0x00000400,		//HDTV size:1920*1080 f/s : 29.97 progressive
	ET_VideoStandard_1920_1080_30p		= 0x00000800,		//HDTV size:1920*1080 f/s : 30 progressive

	ET_VideoStandard_1280_720_2398p		= 0x00001000,	    //HDTV size:1280*720 f/s : 23.98 progressive
	ET_VideoStandard_1280_720_24p		= 0x00002000,		//HDTV size:1280*720 f/s : 24 progressive
	ET_VideoStandard_1280_720_50p		= 0x00004000,		//HDTV size:1280*720 f/s : 50 progressive
	ET_VideoStandard_1280_720_5994p		= 0x00008000,		//HDTV size:1280*720 f/s : 59.94 progressive

	ET_VideoStandard_1440_1080_50i		= 0x00010000,	    //HD  size:1440*1080 f/s : 25 interlaced
	ET_VideoStandard_1440_1080_5994i    = 0x00020000,	    //HD  size:1440*1080 f/s : 29.97 interlaced
	ET_VideoStandard_1440_1080_60i		= 0x00040000,	    //HD  size:1440*1080 f/s : 30 interlaced	

	ET_VIDEOSTANDARD_PAL_16_9			= 0x00080000,		//PAL size:720*576 f/s : 25  
	ET_VIDEOSTANDARD_NTSC_2997_16_9		= 0x00100000,		//NTSC size:720*486  f/s : 29.97
	ET_VIDEOSTANDARD_NTSC_30_16_9		= 0x00200000,		//NTSC size:720*486 f/s : 30 
	ET_VIDEOSTANDARD_NTSC_2997_480		= 0x00400000,		//NTSC size:720*480  f/s : 29.97  //先留着，防止以后特殊情况再改成480
	ET_VIDEOSTANDARD_NTSC_30_480		= 0x00800000,		//NTSC size:720*480 f/s : 30 //先留着，防止以后特殊情况再改成480

	ET_VIDEOSTANDARD_1280_1080_50i      = 0x01000000,       //HD  size:1280*1080 f/s : 25 interlaced
	ET_VIDEOSTANDARD_1280_1080_5994i    = 0x02000000,       //HD  size:1280*1080 f/s : 29.97 interlaced
	ET_VIDEOSTANDARD_1280_1080_60i      = 0x04000000,       //HD  size:1280*1080 f/s : 30 interlaced

};


#define ET_OLEDATA_DRAGCLIP					_T("SobeyETClip")


//关于从ETObject结构体继承的对象判断，实际的结构体类型
#define ET_RT_CLASS_ETOBJECT						0x0001
#define ET_RT_CLASS_ETFOLDER						0x0002
#define ET_RT_CLASS_ETCLIPINFO						0x0003
#define ET_RT_CLASS_ETPGMMASTER						0x0004
#define ET_RT_CLASS_ETTEMPLATEOBJECT				0x0005
#define ET_RT_CLASS_ETCDINFO						0x0006
#define ET_RT_CLASS_ETEDLINFO						0x0007
#define ET_RT_CLASS_ETCLIPINFOEX					0x0008

#define ET_RUNTIME_CLASS( __BaseClass, __ClassID )									\
	public :																		\
	\
	/* 取得对象的类ID */														    \
	static long GetClassID() { return __ClassID; }						            \
	\
	/* 取得对象的类ID */															\
	virtual long VirtualGetClassID() { return __ClassID; }							\
	\
	/* 检测对象是否是某个类的子类或同类 */										    \
	virtual BOOL IsKindOf( long lClassID )									        \
{																			        \
	if ( __ClassID == lClassID )										            \
	return TRUE;														        \
	\
	return __BaseClass::IsKindOf( lClassID );								        \
}

#define ET_RUNTIME_CLASS_BASE(__ClassID)                                            \
	public :																		\
	\
	/* 取得对象的类ID */														    \
	static long GetClassID() { return __ClassID; }						            \
	\
	/* 取得对象的类ID */															\
	virtual long VirtualGetClassID() { return __ClassID; }							\
	\
	/* 检测对象是否是某个类的子类或同类 */										    \
	virtual BOOL IsKindOf( long lClassID )									        \
{																			        \
	return ( __ClassID == lClassID );										        \
}

//CheckInstallKey的返回值
#define KEY_RIGHT					0	//Key正确
#define KEY_RIGHT_RECORD_FAILED		1	//Key正确但记录Key失败
#define KEY_WRONG					2	//Key错误
#define LOAD_VALIDATE_FAILED		3	//加载Validate.dll失败
typedef int ( *pCheckInstallKey)(LPCTSTR strKey);

 
//OA_MATRIAL_SPECIAL_FOLDERID 
//#define OA_MATRIAL_FOLDERID			10000

//拖拽信息头的解析
typedef struct stu_ETDragHeaderDef
{
	long						m_lVersion;			//协议版本号
	ET_DRAG_OBJTYPEMASK_TYPE	m_lObjectType;		//对象类型
	ET_DRAG_SOURCE_TYPE			m_lSourceType;		//数据来源
	CString						m_strUserCode;		//用户编码
	CString						m_strIPAddress;		//IP地址
	long						m_lSiteID;			//站点ID
	stu_ETDragHeaderDef()
	{
		m_lVersion		= 25;
		m_lObjectType	= 0;
		m_lSourceType	= 0;
		m_strUserCode	= _T("");
		m_strIPAddress	= _T("");
		m_lSiteID		= 0;
	}
	~stu_ETDragHeaderDef()
	{

	}

	stu_ETDragHeaderDef& operator=(const stu_ETDragHeaderDef& src)
	{
		m_lVersion		= src.m_lVersion;
		m_lObjectType	= src.m_lObjectType;
		m_lSourceType	= src.m_lSourceType;
		m_strUserCode	= src.m_strUserCode;
		m_strIPAddress	= src.m_strIPAddress;
		m_lSiteID		= src.m_lSiteID;
	}
}ETDragHeaderDef, *LPETDragHeaderDef;

//同于数据库结构的站点描述
//对于这地方的站点做一个简单的规则规定：
//0：为主数据库或者缺省数据库
//1：为本地数据库的站点ID
//对于其他的数据都是由网管输入的。
typedef struct stu_ETSiteDef
{
	long				m_lSiteID;			//站点ID,也就是数据库ID。
	CString				m_strPluginDLLName;	//站点对应的处理插件DLL,对于PE无效
	CString				m_strSiteName;		//站点名称
	CString				m_strSiteDesc;		//站点描述
	long				m_lSerialNo;		//站点排序号
	ETVML_PLUGIN_TYPE	m_lPlugInType;		//插件类型

	stu_ETSiteDef()
	{
		m_lSiteID			= -1;
		m_strPluginDLLName	= _T("");
		m_strSiteName		= _T("");
		m_strSiteDesc		= _T("");
		m_lSerialNo         = 0;
		m_lPlugInType       = ETVML_PLUGIN_TYPE_NETWORK;
	}
	~stu_ETSiteDef()
	{

	}

	stu_ETSiteDef(stu_ETSiteDef& src)
	{
		*this = src;
	}

	stu_ETSiteDef& operator=(const stu_ETSiteDef& src)
	{
		m_lSiteID            = src.m_lSiteID;
		m_strPluginDLLName   = src.m_strPluginDLLName;
		m_strSiteName		 = src.m_strSiteDesc;
		m_strSiteDesc		 = src.m_strSiteDesc;
		m_lSerialNo          = src.m_lSerialNo;
		m_lPlugInType		 = src.m_lPlugInType;
	}
}ETSiteDef, *LPETSiteDef;
typedef CArray<LPETSiteDef, LPETSiteDef> ETArraySiteDef;

//用户信息定义结构体
typedef struct stu_ETUserInfo
{
	long		m_lUserID;		//用户ID
	CString		m_strUserCode;	//用户编码
	CString		m_strUserName;	//用户名称
	long		m_lDeptID;		//部门ID
	BOOL        m_bIsValid;     //用户是否有效
	BOOL        m_bCanUseSystem; //用户能否使用系统
	stu_ETUserInfo()
	{
		Reset();
	}
	stu_ETUserInfo & operator = (const stu_ETUserInfo & src)
	{
		m_lUserID		= src.m_lUserID;
		m_strUserCode	= src.m_strUserCode;
		m_strUserName	= src.m_strUserName;
		m_lDeptID		= src.m_lDeptID;
		m_bIsValid      = src.m_bIsValid;
		m_bCanUseSystem = src.m_bCanUseSystem;
		return *this;
	}
	void Reset()
	{
		m_lUserID		= 0;
		m_strUserCode	= _T("");
		m_strUserName	= _T("");
		m_lDeptID = 0;
		m_bIsValid = TRUE;
		m_bCanUseSystem = TRUE;
	}
}ETUserInfo,*lpETUserInfo;
typedef CArray<lpETUserInfo,lpETUserInfo &> ETArrayUserInfo;

//部门信息定义结构体
typedef struct stu_ETDeptInfo
{
	long		  m_lDeptID;		//部门ID
	CString		  m_strDeptCode;	//部门编码
	CString		  m_strDeptName;	//部门名称
	CString		  m_strInputCode;	//部门录入码
	long		  m_lSuperID;		//上级部门ID
	stu_ETDeptInfo()
	{
		Reset();
	}
	stu_ETDeptInfo & operator = (const stu_ETDeptInfo & src)
	{
		m_lDeptID		= src.m_lDeptID;
		m_strDeptCode	= src.m_strDeptCode;
		m_strDeptName	= src.m_strDeptName;
		m_strInputCode	= src.m_strInputCode;
		m_lSuperID		= src.m_lSuperID;
		return *this;
	}
	void Reset()
	{
		m_lDeptID		= 0;
		m_strDeptCode	= _T("");
		m_strDeptName	= _T("");
		m_strInputCode	= _T("");
		m_lSuperID		= 0;
	}
}ETDeptInfo,*LPETDeptInfo;
typedef CArray<LPETDeptInfo,LPETDeptInfo &> ETArrayDeptInfo;

//频道信息定义结构体
typedef struct stu_ETChannelInfo
{
	long			  m_lChannelID;		//频道ID
	CString			  m_strChannelCode;	//频道编码
	CString			  m_strChannelName;	//频道名称
	ET_NM_OBJECT_TYPE m_emType;			//类型
	long			  m_lSuperID;		//上级频道ID
	stu_ETChannelInfo()
	{
		Reset();
	}
	stu_ETChannelInfo & operator = (const stu_ETChannelInfo & src)
	{
		m_lChannelID		= src.m_lChannelID;
		m_strChannelCode	= src.m_strChannelCode;
		m_strChannelName	= src.m_strChannelName;
		m_emType			= src.m_emType;
		m_lSuperID			= src.m_lSuperID;
		return *this;
	}
	void Reset()
	{
		m_lChannelID		= 0;
		m_strChannelCode	= _T("");
		m_strChannelName	= _T("");
		m_emType			= ET_NM_OBJECT_TYPE_CHANNEL;
		m_lSuperID			= 0;
	}
}ETChannelInfo,*lpETChannelInfo;
typedef CArray<lpETChannelInfo,lpETChannelInfo &> ETArrayChannelInfo;

//栏目信息定义结构体
typedef struct stu_ETColumnInfo
{
	long			  m_lColumnID;		//栏目ID
	CString			  m_strColumnCode;	//栏目编码
	CString			  m_strColumnName;	//栏目名称
	ET_NM_OBJECT_TYPE m_emType;			//类型
	long			  m_lSuperID;		//上级ID
	stu_ETColumnInfo()
	{
		Reset();
	}
	stu_ETColumnInfo & operator = (const stu_ETColumnInfo & src)
	{
		m_lColumnID		= src.m_lColumnID;
		m_strColumnCode	= src.m_strColumnCode;
		m_strColumnName	= src.m_strColumnName;
		m_emType		= src.m_emType;
		m_lSuperID		= src.m_lSuperID;
		return *this;
	}
	void Reset()
	{
		m_lColumnID		= 0;
		m_strColumnCode	= _T("");
		m_strColumnName	= _T("");
		m_emType		= ET_NM_OBJECT_TYPE_COLUMN;
		m_lSuperID		= 0;
	}
}ETColumnInfo,*lpETColumnInfo;
typedef CArray<lpETColumnInfo,lpETColumnInfo &> ETArrayColumnInfo;

//版次信息定义结构体
typedef struct stu_ETEditionInfo
{
	long		  m_lEditionID;		//版次ID
	CString		  m_strEditionCode;	//版次编码
	CString		  m_strEditionName;	//版次名称
	long		  m_lTimeLength;	//长度
	DATE		  m_dtBeginTime;	//版次播出时间
	long		  m_lColumnID;		//栏目ID
	stu_ETEditionInfo()
	{
		Reset();
	}
	stu_ETEditionInfo & operator = (const stu_ETEditionInfo & src)
	{
		m_lEditionID		= src.m_lEditionID;
		m_strEditionCode	= src.m_strEditionCode;
		m_strEditionName	= src.m_strEditionName;
		m_lColumnID			= src.m_lColumnID;
		return *this;
	}
	void Reset()
	{
		m_lEditionID		= 0;
		m_strEditionCode	= _T("");
		m_strEditionName	= _T("");
		m_lColumnID			= 0;
	}
}ETEditionInfo,*lpETEditionInfo;
typedef CArray<lpETEditionInfo,lpETEditionInfo &> ETArrayEditionInfo;

enum emETMenuType
{
	ET_emMenuType_Separator = 1, //隔断菜单
	ET_emMenuType_Menu		= 2, //一级菜单
	ET_emMenuType_Child		= 4  //子菜单
};

//菜单定义结构体
typedef struct stu_ETMenuData
{
	DWORD			m_dwMenuID;		//菜单的命令ID
	CString			m_strMenuName;	//菜单显示的名称
	emETMenuType	m_emMenuType;	//菜单类型
	DWORD			m_dwPositionX;	//菜单放在第几列，如果为子菜单，此参数忽略
	DWORD			m_dwPositionY;	//菜单放在该列的第几行，如果为子菜单，此参数忽略
	stu_ETMenuData *m_pChildNode;	//子菜单指针
	stu_ETMenuData *m_pNextNode;	//下一个菜单指针

	stu_ETMenuData()
	{
		Reset();
	}

	~stu_ETMenuData()
	{
		if ( m_pNextNode )
		{
			delete	m_pNextNode;
			m_pNextNode	= NULL;
		}

		if ( m_pChildNode )
		{
			delete	m_pChildNode;
			m_pChildNode	= NULL;
		}
	}

	stu_ETMenuData & operator = (const stu_ETMenuData & src)
	{
		m_dwMenuID		= src.m_dwMenuID;
		m_strMenuName	= src.m_strMenuName;
		m_emMenuType	= src.m_emMenuType;
		m_dwPositionX	= src.m_dwPositionX;
		m_dwPositionY	= src.m_dwPositionY;
		m_pChildNode	= src.m_pChildNode;
		m_pNextNode		= src.m_pNextNode;
		return *this;
	}
	void Reset()
	{
		m_dwMenuID		= 0;
		m_strMenuName	= _T("");
		m_emMenuType	= ET_emMenuType_Separator;
		m_dwPositionX	= 0;
		m_dwPositionY	= 0;
		m_pChildNode	= NULL;
		m_pNextNode		= NULL;
	}
}ETMenuData,*LPETMenuData;

//媒体格式定义结构体
typedef struct stu_ETMediaFormatDef
{
	long					m_lMiediaFmtID;			//格式ID
	emMediaFormatDefType	m_emType;				//1：视频格式，2：音频格式，3：键格式，4：图片格式
	GUID					m_guidFileType;			//文件格式的GUID
	long					m_lDataRate;			//码率
	long					m_lFOURCC;				//媒体类型
	long					m_lMediaType;			//媒体文件类型
	long					m_lSubMediaType;		//媒体文件子类型
	long					m_lAudioChannels;		//音频通道数
	long					m_lAudioSamplesRate;	//音频采样率
	long					m_lAudioBitsperSample;	//量化位数
	long					m_lVideoImgWidth;		//素材画面宽
	long					m_lVideoImgHeight;		//素材画面高
	emETVideoStandard		m_emVideoStandard;		//制式
	long					m_lVPixFmt;				//像素格式
	long					m_lVBitsPerPixel;		//每像素位数
	double					m_dblVFrameRate;		//帧率
	long					m_lVCompression;		//数据压缩方式
	long					m_lVScanMode;			//扫描模式
	long					m_lVGOPSize;			//GOP长度
	long					m_lVBFrameCount;		//B帧个数
	long					m_lVPFrameCount;		//P帧个数
	__int64					m_lHashKey;				//格式HASH值
	//构造函数
	stu_ETMediaFormatDef()
	{
		Reset();
	}
	//复位函数
	void Reset()
	{
		m_lMiediaFmtID		= 0;
		m_emType			= ET_emMediaFmt_Unknow;
		m_guidFileType		= GUID_NULL;
		m_lDataRate			= 0;
		m_lFOURCC			= 0;
		m_lMediaType		= 0;
		m_lSubMediaType		= 0;
		m_lAudioChannels	= 0;
		m_lAudioSamplesRate	= 0;
		m_lAudioBitsperSample= 0;
		m_lVideoImgWidth	= 0;
		m_lVideoImgHeight	= 0;
		m_emVideoStandard	= ET_VideoStandard_UNKNOW;
		m_lVPixFmt			= 0;
		m_lVBitsPerPixel	= 0;
		m_dblVFrameRate		= 0;
		m_lVCompression		= 0;
		m_lVScanMode		= 0;
		m_lVGOPSize			= 0;
		m_lVBFrameCount		= 0;
		m_lVPFrameCount		= 0;
		m_lHashKey			= 0;
	}
	//判断是否相等
	BOOL operator == (const stu_ETMediaFormatDef & src)
	{
		if( m_emType			== src.m_emType&&
			m_guidFileType		== src.m_guidFileType&&
			m_lFOURCC			== src.m_lFOURCC&&
			m_lMediaType		== src.m_lMediaType&&
			m_lSubMediaType		== src.m_lSubMediaType&&
			m_lAudioChannels	== src.m_lAudioChannels&&
			m_lAudioSamplesRate	== src.m_lAudioSamplesRate&&
			m_lAudioBitsperSample== src.m_lAudioBitsperSample&&
			m_lVideoImgWidth	== src.m_lVideoImgWidth&&
			m_lVideoImgHeight	== src.m_lVideoImgHeight&&
			m_emVideoStandard	== src.m_emVideoStandard&&
			m_lVPixFmt			== src.m_lVPixFmt&&
			m_lVBitsPerPixel	== src.m_lVBitsPerPixel&&
			m_lVCompression		== src.m_lVCompression&&
			m_lVScanMode		== src.m_lVScanMode&&
			m_lVGOPSize			== src.m_lVGOPSize&&
			m_lVBFrameCount		== src.m_lVBFrameCount&&
			m_lVPFrameCount		== src.m_lVPFrameCount)		
		{
			double dblDif = m_dblVFrameRate	- src.m_dblVFrameRate ;
			if(dblDif < 0)
				dblDif = -dblDif;
			if(dblDif < 0.005 && m_lDataRate - src.m_lDataRate < 1000000)
                return TRUE;
		}
		return FALSE;			
	}

	//赋值函数
	stu_ETMediaFormatDef & operator = (const stu_ETMediaFormatDef & src)
	{
		m_lMiediaFmtID		= src.m_lMiediaFmtID;
		m_emType			= src.m_emType;
		m_guidFileType		= src.m_guidFileType;
		m_lDataRate			= src.m_lDataRate;
		m_lFOURCC			= src.m_lFOURCC;
		m_lMediaType		= src.m_lMediaType;
		m_lSubMediaType		= src.m_lSubMediaType;
		m_lAudioChannels	= src.m_lAudioChannels;
		m_lAudioSamplesRate	= src.m_lAudioSamplesRate;
		m_lAudioBitsperSample= src.m_lAudioBitsperSample;
		m_lVideoImgWidth	= src.m_lVideoImgWidth;
		m_lVideoImgHeight	= src.m_lVideoImgHeight;
		m_emVideoStandard	= src.m_emVideoStandard;
		m_lVPixFmt			= src.m_lVPixFmt;
		m_lVBitsPerPixel	= src.m_lVBitsPerPixel;
		m_dblVFrameRate		= src.m_dblVFrameRate;
		m_lVCompression		= src.m_lVCompression;
		m_lVScanMode		= src.m_lVScanMode;
		m_lVGOPSize			= src.m_lVGOPSize;
		m_lVBFrameCount		= src.m_lVBFrameCount;
		m_lVPFrameCount		= src.m_lVPFrameCount;
		m_lHashKey			= src.m_lHashKey;
		return *this;
	}

	//获取HASH值
	__int64 GetHashKey()
	{
		/*long lDataRate;
		lDataRate = m_lDataRate;
		lDataRate /= 1000*1000;*/

		__int64 lKey = 0;
		lKey += m_emType;
		lKey += m_lDataRate;
		lKey += m_lFOURCC;
		lKey += m_lMediaType;
		lKey += m_lSubMediaType;
		lKey += m_lAudioChannels;
		lKey += m_lAudioSamplesRate;
		lKey += m_lAudioBitsperSample;
		lKey += m_lVideoImgWidth;
		lKey += m_lVideoImgHeight;
		lKey += m_emVideoStandard;
		lKey += m_lVPixFmt;
		lKey += m_lVBitsPerPixel;
		lKey += (long)m_dblVFrameRate;
		lKey += m_lVCompression;
		lKey += m_lVScanMode;
		lKey += m_lVGOPSize;
		lKey += m_lVBFrameCount;
		lKey += m_lVPFrameCount;

		//处理GUID
		lKey += m_guidFileType.Data1;
		lKey += m_guidFileType.Data2;
		lKey += m_guidFileType.Data3;
		lKey += m_guidFileType.Data4[0];
		lKey += m_guidFileType.Data4[1];
		lKey += m_guidFileType.Data4[2];
		lKey += m_guidFileType.Data4[3];
		lKey += m_guidFileType.Data4[4];
		lKey += m_guidFileType.Data4[5];
		lKey += m_guidFileType.Data4[6];
		lKey += m_guidFileType.Data4[7];

		m_lHashKey = lKey;
		return lKey;
	}

	//析构函数
	virtual ~stu_ETMediaFormatDef()
	{
		Reset();
	}
}ETMediaFormatDef,*lpETMediaFormatDef;
typedef CArray<lpETMediaFormatDef,lpETMediaFormatDef&> ETArrayMediaFmt;

//资源引用保存数据结构体
typedef struct stu_ETResourceRefer
{
	GUID			m_guidRes;
	long			m_lDatabase;
	ET_RES_TYPE     m_eResType;
	long			m_lTrimIn;
	long			m_lTrimOut;
}ETResourceRefer,*lpETResourceRefer;
typedef CArray<lpETResourceRefer,lpETResourceRefer> ETArrayResourceRefer;

//one generation数据表结构体
typedef struct stu_ETClipSource
{
	GUID				m_guidClipGuid;		//对象GUID
	GUID				m_guidSrcClipGuid;	//源对象GUID
	long                m_lSiteID;			//SITEID
	CString				m_strObjectName;	//对象名称
	CString				m_strObjectPath;	//路径
	long				m_lLength;			//长度
	long				m_lTrimIn;			//入点
	long				m_lTrimOut;			//出点
	emETVideoStandard	m_emVideoStandard;	//制式
	DATE				m_dtModifyDate;		//修改日期
	long				m_lSrcGuidIsValid;   //源对象GUID是否有效 1有效 0无效
	long                m_lNTSCTCMode;
	
	stu_ETClipSource()
	{
		Reset();
	}
	const stu_ETClipSource& operator = (const stu_ETClipSource &src)
	{
		m_guidClipGuid			= src.m_guidClipGuid;
		m_guidSrcClipGuid		= src.m_guidSrcClipGuid;
		m_lSiteID				= src.m_lSiteID;
		m_strObjectName			= src.m_strObjectName;
		m_strObjectPath			= src.m_strObjectName;
		m_lLength				= src.m_lLength;
		m_lTrimIn				= src.m_lTrimIn;
		m_lTrimOut				= src.m_lTrimOut;
		m_emVideoStandard		= src.m_emVideoStandard;
		m_dtModifyDate			= src.m_dtModifyDate;
		m_lSrcGuidIsValid		= src.m_lSrcGuidIsValid;
        m_lNTSCTCMode			= src.m_lNTSCTCMode;
		return *this;
	}
	void Reset()
	{
		m_guidClipGuid			= GUID_NULL;
		m_guidSrcClipGuid		= GUID_NULL;
		m_lSiteID				= 0;
		m_strObjectName			= _T("");
		m_strObjectPath			= _T("");
		m_lLength				= 0;
		m_lTrimIn				= 0;
		m_lTrimOut				= 0;
		m_emVideoStandard		= ET_VideoStandard_UNKNOW;
		m_dtModifyDate			= 0.00;
		m_lSrcGuidIsValid		= 0;
		m_lNTSCTCMode			= 0;
	
	}
	virtual ~stu_ETClipSource()
	{
		Reset();
	}

}ETClipSource,*lpETClipSource;
typedef CArray<lpETClipSource,lpETClipSource> ETArrayClipSource;

typedef struct stu_ETRemoteScheduleTask
{
	CString	m_strTaskGuid; //计划下载任务GUID
	CString	m_strSrcClipGUID; //源素材GUID
	LONG	m_lSourceSiteID; //源站点ID
	CString m_strSourceSiteName; //源站点名
	CString m_strSourceFolderPath; //源素材路径
	CString m_strMaterialName;//素材名称
	CString m_strRights;
	CString m_strComment;
	LONG	m_lTargetSiteID; //目标站点ID
	CString m_strTargetSiteName; //目标站点名
	CString m_strTargetFolderPath; //目标素材路径
	ET_CLIP_QUALITY_TYPE m_lQuanlity;//质量
	LONG	m_lPriority;			//优先级
	BOOL	m_lType;			//下载m_lType = 0、上载m_lType = 1
	LONG	m_lCreatorID;			//任务创建者ID	
	LONG	m_lStatus;				//任务状态：0 新增任务； 1 发送失败
	stu_ETRemoteScheduleTask()
	{
		Reset();
	}
	const stu_ETRemoteScheduleTask& operator = (const stu_ETRemoteScheduleTask &src)
	{
		m_strTaskGuid			= src.m_strTaskGuid;
		m_strSrcClipGUID		= src.m_strSrcClipGUID;
		m_lSourceSiteID			= src.m_lSourceSiteID;
		m_strSourceSiteName		= src.m_strSourceSiteName;
		m_strSourceFolderPath	= src.m_strSourceFolderPath;
		m_strMaterialName		= src.m_strMaterialName;
		m_strRights				= src.m_strRights;
		m_strComment			= src.m_strComment;
		m_lTargetSiteID			= src.m_lTargetSiteID;
		m_strTargetSiteName		= src.m_strTargetSiteName;
		m_strTargetFolderPath	= src.m_strTargetFolderPath;
		m_lQuanlity				= src.m_lQuanlity;
		m_lPriority				= src.m_lPriority;
		m_lType					= src.m_lType;
		m_lCreatorID			= src.m_lCreatorID;
		m_lStatus				= src.m_lStatus;
		return *this;
	}
	void Reset()
	{
		m_strTaskGuid			= _T("");
		m_strSrcClipGUID		= _T("");
		m_lSourceSiteID			= 0;
		m_strSourceSiteName		= _T("");
		m_strSourceFolderPath	= _T("");
		m_strMaterialName		= _T("");
		m_strRights				= _T("");
		m_strComment			= _T("");
		m_lTargetSiteID			= 0;
		m_strTargetSiteName		= _T("");
		m_strTargetFolderPath	= _T("");
		m_lQuanlity				= ET_CLIP_QUALITY_ALL;
		m_lPriority				= 50;
		m_lType					= 0;
		m_lCreatorID			= 0;
		m_lStatus				= 0;
	}
	virtual ~stu_ETRemoteScheduleTask()
	{
		Reset();
	}
}ETRemoteScheduleTask,*lpETRemoteScheduleTask;
typedef CArray<lpETRemoteScheduleTask,lpETRemoteScheduleTask> ETArrayRemoteScheduleTask;


typedef struct stu_ETObjectEx{
	CString				m_strReserved5;		    //备用数据5
	CString				m_strReserved6;		    //备用数据6
	CString				m_strReserved7;		    //备用数据7
	CString				m_strReserved8;		    //备用数据8
	CString				m_strReserved9;		    //备用数据9
	CString				m_strReserved10;		//备用数据10
	CString				m_strReserved11;		//备用数据11
	CString				m_strReserved12;		//备用数据12
	CString				m_strReserved13;		//备用数据13
	CString				m_strReserved14;		//备用数据14 
	ET_CLIP_ARCHIVE		m_lXDAArchiveFlag;		//XDA归档标志
	ULONGLONG           m_ullObjDataMask;       //掩码
	const stu_ETObjectEx& operator = (const stu_ETObjectEx &src)
	{
		m_strReserved5		= src.m_strReserved5;
		m_strReserved6		= src.m_strReserved6;
		m_strReserved7		= src.m_strReserved7;
		m_strReserved8		= src.m_strReserved8;
		m_strReserved9		= src.m_strReserved9;
		m_strReserved10		= src.m_strReserved10;
		m_strReserved11		= src.m_strReserved11;
		m_strReserved12		= src.m_strReserved12;
		m_strReserved13		= src.m_strReserved13;
		m_strReserved14		= src.m_strReserved14;
		m_lXDAArchiveFlag   = src.m_lXDAArchiveFlag;
		return *this;
	}
	void CopyByMask(const stu_ETObjectEx &src)
	{

		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved5))
			m_strReserved5		= src.m_strReserved5;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved6))
			m_strReserved6		= src.m_strReserved6;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved7))
			m_strReserved7		= src.m_strReserved7;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved8))
			m_strReserved8		= src.m_strReserved8;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved9))
			m_strReserved9		= src.m_strReserved9;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved10))
			m_strReserved10		= src.m_strReserved10;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved11))
			m_strReserved11		= src.m_strReserved11;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved12))
			m_strReserved12		= src.m_strReserved12;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved13))
			m_strReserved13		= src.m_strReserved13;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_Reserved14))
			m_strReserved14		= src.m_strReserved14;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELDEX_XDAArchiveFlag))
			m_lXDAArchiveFlag	= src.m_lXDAArchiveFlag;
	}
	stu_ETObjectEx()
	{
		Reset();
	}
	void Reset()
	{
		m_ullObjDataMask    = 0XFFFFFFFFFFFFFFFF;
		m_strReserved5		= _T("");
		m_strReserved6		= _T("");
		m_strReserved7		= _T("");
		m_strReserved8		= _T("");
		m_strReserved9		= _T("");
		m_strReserved10		= _T("");
		m_strReserved11		= _T("");
		m_strReserved12		= _T("");
		m_strReserved13		= _T("");
		m_strReserved14		= _T("");
		m_lXDAArchiveFlag	= ET_CLIP_ARCHIVE_NORMAL;
	}
	virtual ~stu_ETObjectEx()
	{
		Reset();
	}
}ETObjectEx,*lpETObjectEx;


//实体对象表结构体：表ET_OBJECT中的所有字段
typedef struct stu_ETObject
{
	long				m_lObjectID;		//对象ID
	GUID				m_guidObjectGuid;	//对象GUID
	emObjectType		m_emObjectType;	//对象类型
	__int64				m_llSubType;		//对象子类型
	long				m_lFolderID;		//文件夹ID
	long				m_lPopdomID;		//权限ID
	long				m_lCDID;			//光盘ID
	CString				m_strObjectName;	//对象名称
	CString				m_strKeyWord;		//关键词
	long				m_lIconFrame;		//图标帧
	CString				m_strIconFileName;	//图标文件
	long				m_lLength;			//长度
	long				m_lTrimIn;			//入点
	long				m_lTrimOut;			//出点
	long				m_lCreaterID;		//创建者ID
	DATE				m_dtCreateDate;		//创建日期
	long				m_lMenderID;		//修改人ID	
	DATE				m_dtModifyDate;		//修改日期
	DATE				m_dtAccessDate;		//访问日期
	long				m_lDelateFlag;		//删除标志
	long				m_lDelaterID;		//删除人ID
	DATE				m_dtDelateDate;		//删除日期
	DATE				m_dtRecordDate;		//录制日期
	DATE				m_dtShowDate;		//播出日期
	DATE				m_dtDistributeDate;	//发布日期
	long				m_lDistributerID;	//发布人
	DATE				m_dtArchiveDate;	//归档日期
	long				m_lArchiverID;		//归档日期
	ET_CLIP_ARCHIVE		m_lArchiveFlag;		//归档标志
	ET_CLIP_ARCHIVE		m_l3rdArchiveFlag;	//第3方归档标志
	long				m_lColumnID;		//栏目ID
	emETVideoStandard	m_emVideoStandard;	//制式
	int					m_iStatus;			//状态
	CString				m_strUMID;			//对象的UMID
	long				m_lUsedFlag;		//使用标志
	int					m_iLockFlag;		//锁定标志
	long				m_lFileStatus;		//文件状态
	CString				m_strRights;		//版权信息
	CString				m_strJournalList;	//记者
	CString				m_strMediaChannel;	//媒体通道
	CString				m_strObjectNote;	//对象备注
	CString				m_strReserved1;		//备用数据1
	CString				m_strReserved2;		//备用数据2
	CString				m_strReserved3;		//备用数据3
	CString				m_strReserved4;		//备用数据4
	ULONGLONG			m_ullObjDataMask;	//数据有效位定义
	ET_CLIP_CAPTURESTATUS m_lCaptureStatus;	//采集状态
	ET_CLIP_IMAGETYPE	m_lImageType;		//图像类型;0、为素材原始比例，1、图像为4:3的方式，2：图像为16:9的方式(对于S2，0、为素材原始比例，2为16:9SD, 4为HD，1不使用)
	CString				m_strEditTerminal;	//编辑终端
	CString				m_strModifyTerminal;//修改终端
	GUID				m_guidDeviceID;		//光盘GUID
	long				m_lDatabase;		//站点实例
	long				m_lMarkIn;			//标记入点
	long				m_lMarkOut;			//标记出点
	long				m_lTobeArchiveState;//特殊归档标志
	CString             m_strStorageState;  //2009/03/26 zxp:add Archive节点下的Archive Status状态
	long                m_lDBEStreamChannel;   //DBE音频通道放在素材的哪两轨上
	long                m_lNTSCTCMode;      //时码计算方式：0 按素材原始格式 1 用非丢帧模式
	CString             m_strMaterialID; //V4.6 Material ID
	long                m_lBKColor;   //对象背景颜色
	CString             m_strReserved20;        //XDCAMCOMPILE备用
	CString             m_strStoryID;           //STORYID
	GUID				m_guidResSource; //源素材GUID
	ETObjectEx			m_stObjectEx;    //ETOBJECT扩展字段

	ET_RUNTIME_CLASS_BASE(ET_RT_CLASS_ETOBJECT)

	stu_ETObject()
	{
		Reset();
	}

	void CopyByMask(const stu_ETObject &src)
	{
		m_lObjectID			= src.m_lObjectID;
		m_guidObjectGuid	= src.m_guidObjectGuid;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ObjectType))
			m_emObjectType		= src.m_emObjectType;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_SubType))
			m_llSubType			= src.m_llSubType;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_FolderID))
			m_lFolderID			= src.m_lFolderID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_PopdomID))
			m_lPopdomID			= src.m_lPopdomID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_CDID))
			m_lCDID				= src.m_lCDID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ObjectName))
			m_strObjectName		= src.m_strObjectName;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_KeyWord))
			m_strKeyWord		= src.m_strKeyWord;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_IconFrame))
			m_lIconFrame		= src.m_lIconFrame;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_IconFileName))
			m_strIconFileName	= src.m_strIconFileName;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_Length))
			m_lLength			= src.m_lLength;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_TrimIn))
			m_lTrimIn			= src.m_lTrimIn;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_TrimOut))
			m_lTrimOut			= src.m_lTrimOut;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_CreaterID))
			m_lCreaterID		= src.m_lCreaterID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_CreateDate))
			m_dtCreateDate		= src.m_dtCreateDate;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_MenderID))
			m_lMenderID			= src.m_lMenderID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ModifyDate))
			m_dtModifyDate		= src.m_dtModifyDate;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_AccessDate))
			m_dtAccessDate		= src.m_dtAccessDate;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_DeleteFlag))
			m_lDelateFlag		= src.m_lDelateFlag;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_DelaterID))
			m_lDelaterID		= src.m_lDelaterID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_DelateDate))
			m_dtDelateDate		= src.m_dtDelateDate;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_RecordDate))
			m_dtRecordDate		= src.m_dtRecordDate;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ShowDate))
			m_dtShowDate		= src.m_dtShowDate;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_DistributeDate))
			m_dtDistributeDate	= src.m_dtDistributeDate;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_DistributerID))
			m_lDistributerID	= src.m_lDistributerID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ArchiveDate))
			m_dtArchiveDate		= src.m_dtArchiveDate;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ArchiverID))
			m_lArchiverID		= src.m_lArchiverID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ArchiveFlag))
			m_lArchiveFlag		= src.m_lArchiveFlag;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_3rdArchiveFlag))
			m_l3rdArchiveFlag		= src.m_l3rdArchiveFlag;
		if (FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_TobeArchiveState))
			m_lTobeArchiveState		= src.m_lTobeArchiveState;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ColumnID))
			m_lColumnID			= src.m_lColumnID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_VideoStandard))
			m_emVideoStandard	= src.m_emVideoStandard;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_Status))
			m_iStatus			= src.m_iStatus;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_UMID))
			m_strUMID			= src.m_strUMID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_UsedFlag))
			m_lUsedFlag			= src.m_lUsedFlag;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_LockFlag))
			m_iLockFlag			= src.m_iLockFlag;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_FileStatus))
			m_lFileStatus		= src.m_lFileStatus;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_Rights))
			m_strRights			= src.m_strRights;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_JournalList))
			m_strJournalList	= src.m_strJournalList;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_MediaChannel))
			m_strMediaChannel	= src.m_strMediaChannel;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ObjectNote))
			m_strObjectNote		= src.m_strObjectNote;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_Reserved1))
			m_strReserved1		= src.m_strReserved1;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_Reserved2))
			m_strReserved2		= src.m_strReserved2;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_Reserved3))
			m_strReserved3		= src.m_strReserved3;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_Reserved4))
			m_strReserved4		= src.m_strReserved4;
		m_ullObjDataMask	= src.m_ullObjDataMask;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_CaptureStatus))
			m_lCaptureStatus	= src.m_lCaptureStatus;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ImageType))
			m_lImageType		= src.m_lImageType;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_EditTerminal))
			m_strEditTerminal	= src.m_strEditTerminal;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ModifyTerminal))
			m_strModifyTerminal	= src.m_strModifyTerminal;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_DeviceGUID))
			m_guidDeviceID		= src.m_guidDeviceID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_DBEStreamChannel))
			m_lDBEStreamChannel	= src.m_lDBEStreamChannel;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_NTSCTCMode))
			m_lNTSCTCMode	= src.m_lNTSCTCMode;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_MaterialID))
			m_strMaterialID	= src.m_strMaterialID;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_BKColor))
			m_lBKColor = src.m_lBKColor;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_Reserved20))
			m_strReserved20		= src.m_strReserved20;
		if(FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_StoryID))
			m_strStoryID		= src.m_strStoryID;

		if (FIELD_ISSET(src.m_ullObjDataMask, OBJ_FIELD_ResSourceGuid))
			m_guidResSource = src.m_guidResSource;
        m_stObjectEx.CopyByMask(src.m_stObjectEx);
		m_lDatabase				= src.m_lDatabase;
		m_lMarkIn				= src.m_lMarkIn;
		m_lMarkOut				= src.m_lMarkOut;
		m_strStorageState       = src.m_strStorageState;//2009/03/26 zxp:add Archive节点下的Archive Status状态
	}

	const stu_ETObject& operator = (const stu_ETObject &src)
	{
		m_lObjectID			= src.m_lObjectID;
		m_guidObjectGuid	= src.m_guidObjectGuid;
		m_emObjectType		= src.m_emObjectType;
		m_llSubType			= src.m_llSubType;
		m_lFolderID			= src.m_lFolderID;
		m_lPopdomID			= src.m_lPopdomID;
		m_lCDID				= src.m_lCDID;
		m_strObjectName		= src.m_strObjectName;
		m_strKeyWord		= src.m_strKeyWord;
		m_lIconFrame		= src.m_lIconFrame;
		m_strIconFileName	= src.m_strIconFileName;
		m_lLength			= src.m_lLength;
		m_lTrimIn			= src.m_lTrimIn;
		m_lTrimOut			= src.m_lTrimOut;
		m_lCreaterID		= src.m_lCreaterID;
		m_dtCreateDate		= src.m_dtCreateDate;
		m_lMenderID			= src.m_lMenderID;
		m_dtModifyDate		= src.m_dtModifyDate;
		m_dtAccessDate		= src.m_dtAccessDate;
		m_lDelateFlag		= src.m_lDelateFlag;
		m_lDelaterID		= src.m_lDelaterID;
		m_dtDelateDate		= src.m_dtDelateDate;
		m_dtRecordDate		= src.m_dtRecordDate;
		m_dtShowDate		= src.m_dtShowDate;
		m_dtDistributeDate	= src.m_dtDistributeDate;
		m_lDistributerID	= src.m_lDistributerID;
		m_dtArchiveDate		= src.m_dtArchiveDate;
		m_lArchiverID		= src.m_lArchiverID;
		m_lArchiveFlag		= src.m_lArchiveFlag;
		m_l3rdArchiveFlag   = src.m_l3rdArchiveFlag;
		m_lColumnID			= src.m_lColumnID;
		m_emVideoStandard	= src.m_emVideoStandard;
		m_iStatus			= src.m_iStatus;
		m_strUMID			= src.m_strUMID;
		m_lUsedFlag			= src.m_lUsedFlag;
		m_iLockFlag			= src.m_iLockFlag;
		m_lFileStatus		= src.m_lFileStatus;
		m_strRights			= src.m_strRights;
		m_strJournalList	= src.m_strJournalList;
		m_strMediaChannel	= src.m_strMediaChannel;
		m_strObjectNote		= src.m_strObjectNote;
		m_strReserved1		= src.m_strReserved1;
		m_strReserved2		= src.m_strReserved2;
		m_strReserved3		= src.m_strReserved3;
		m_strReserved4		= src.m_strReserved4;
		m_ullObjDataMask	= src.m_ullObjDataMask;
		m_lCaptureStatus	= src.m_lCaptureStatus;
		m_lImageType		= src.m_lImageType;
		m_strEditTerminal	= src.m_strEditTerminal;
		m_strModifyTerminal	= src.m_strModifyTerminal;
		m_guidDeviceID		= src.m_guidDeviceID;
		m_lDatabase			= src.m_lDatabase;
		m_lMarkIn			= src.m_lMarkIn;
		m_lMarkOut			= src.m_lMarkOut;
		m_lTobeArchiveState = src.m_lTobeArchiveState;
		m_strStorageState   = src.m_strStorageState;//2009/03/26 zxp:add Archive节点下的Archive Status状态
		m_lDBEStreamChannel    = src.m_lDBEStreamChannel;
		m_lNTSCTCMode		= src.m_lNTSCTCMode;
		m_strMaterialID     = src.m_strMaterialID;
		m_lBKColor          = src.m_lBKColor;
		m_strReserved20		= src.m_strReserved20;
		m_strStoryID		= src.m_strStoryID;
		m_guidResSource		= src.m_guidResSource;
		m_stObjectEx        = src.m_stObjectEx;
		return *this;
	}
	void Reset()
	{
		m_lObjectID			= 0;
		m_guidObjectGuid	= GUID_NULL;
		m_emObjectType		= ET_ObjType_Unknow;
		m_llSubType			= 0;
		m_lFolderID			= 0;
		m_lPopdomID			= 0;
		m_lCDID				= 0;
		m_strObjectName		= _T("");
		m_strKeyWord		= _T("");
		m_lIconFrame		= 0;
		m_strIconFileName	= _T("");
		m_lLength			= 0;
		m_lTrimIn			= 0;
		m_lTrimOut			= 0;
		m_lCreaterID		= 0;
		m_dtCreateDate		= 0.00;
		m_lMenderID			= 0;
		m_dtModifyDate		= 0.00;
		m_dtAccessDate		= 0.00;
		m_lDelateFlag		= 0;
		m_lDelaterID		= 0;
		m_dtDelateDate		= 0.0;
		m_dtRecordDate		= 0.0;
		m_dtShowDate		= 0.0;
		m_dtDistributeDate	= 0.0;
		m_lDistributerID	= 0;
		m_dtArchiveDate		= 0.0;
		m_lArchiverID		= 0;
		m_lArchiveFlag		= ET_CLIP_ARCHIVE_NORMAL;
		m_l3rdArchiveFlag   = ET_CLIP_ARCHIVE_NORMAL;
		m_lColumnID			= 0;
		m_emVideoStandard	= ET_VideoStandard_UNKNOW;
		m_iStatus			= 0;
		m_strUMID			= _T("");
		m_lUsedFlag			= 0;
		m_iLockFlag			= 0;
		m_lFileStatus		= 0;
		m_strRights			= _T("");
		m_strJournalList	= _T("");
		m_strMediaChannel	= _T("");
		m_strObjectNote		= _T("");
		m_strReserved1		= _T("");
		m_strReserved2		= _T("");
		m_strReserved3		= _T("");
		m_strReserved4		= _T("");
		m_ullObjDataMask	= 0XFFFFFFFFFFFFFFFF;
		m_lCaptureStatus	= ET_CLIP_CAPTURESTATUS_NORMAL;
		m_lImageType		= ET_CLIP_IMAGETYPE_ORIGINAL;
		m_strEditTerminal	= _T("");
		m_strModifyTerminal	= _T("");
		m_guidDeviceID		= GUID_NULL;
		m_lDatabase			= 0;
		m_lMarkIn			= -1;
		m_lMarkOut			= -1;
		m_lTobeArchiveState = ET_CLIP_ARCHIVE_SPECIAL_NONE;
		m_strStorageState   = _T("");//2009/03/26 zxp:add Archive节点下的Archive Status状态
		m_lDBEStreamChannel    = 0;
		m_lNTSCTCMode		= 0;
		m_strMaterialID     = _T("");
		m_lBKColor          = 0;
		m_strReserved20		= _T("");
		m_strStoryID		= _T("");
		m_guidResSource		= GUID_NULL;
		m_stObjectEx.Reset();
		
	}
	virtual ~stu_ETObject()
	{
		Reset();
	}
}ETObject,*lpETObject;
typedef CArray<lpETObject,lpETObject> ETArrayObject;

//文件夹表结构体：表ET_FOLDER中的所有字段
typedef struct  stu_ETFolder:public ETObject
{
	GUID			m_guidTypeID;		//类型GUID
	long			m_lVideoFormatID;	//高视频格式ID
	long			m_lAudioFormatID;	//高音频格式ID
	long			m_lLVideoFormatID;	//低视频格式ID
	long			m_lLAudioFormatID;	//低音频格式ID
	long			m_lRHVideoFormatID;	//打包高视频格式ID
	long			m_lRHAudioFormatID;	//打包高音频格式ID
	long			m_lRLVideoFormatID;	//打包低视频格式ID
	long			m_lRLAudioFormatID;	//打包低音频格式ID
	long			m_lUseOriginalTC;	//使用原始时码

	int				m_iAudioChannel;	//输出音频通道数

	ET_RUNTIME_CLASS(ETObject, ET_RT_CLASS_ETFOLDER)

	stu_ETFolder()
	{
		Reset();
	}
	const stu_ETFolder& operator = (const stu_ETObject &src)
	{
		stu_ETObject::operator =(src);
		return *this;
	}
	const stu_ETFolder& operator = (const stu_ETFolder &src)
	{
		stu_ETObject::operator =(src);

		m_guidTypeID		= src.m_guidTypeID;
		m_lVideoFormatID	= src.m_lVideoFormatID;
		m_lAudioFormatID	= src.m_lAudioFormatID;
		m_lLVideoFormatID	= src.m_lLVideoFormatID;
		m_lLAudioFormatID	= src.m_lLAudioFormatID;
		m_lRHVideoFormatID	= src.m_lRHVideoFormatID;
		m_lRHAudioFormatID	= src.m_lRHAudioFormatID;
		m_lRLVideoFormatID	= src.m_lRLVideoFormatID;
		m_lRLAudioFormatID	= src.m_lRLAudioFormatID;
		m_lUseOriginalTC	= src.m_lUseOriginalTC;

		m_iAudioChannel		= src.m_iAudioChannel;

		return *this;
	}
	void Reset()
	{
		ETObject::Reset();
		m_guidTypeID		= GUID_NULL;
		m_lVideoFormatID	= 0;
		m_lAudioFormatID	= 0;
		m_lLVideoFormatID	= 0;
		m_lLAudioFormatID	= 0;
		m_lRHVideoFormatID	= 0;
		m_lRHAudioFormatID	= 0;
		m_lRLVideoFormatID	= 0;
		m_lRLAudioFormatID	= 0;
		m_lUseOriginalTC	= 0;
		m_iAudioChannel		= 2;
	}
	virtual ~stu_ETFolder()
	{
		Reset();
	}
}ETFolder,*lpETFolder;
typedef CArray<lpETFolder,lpETFolder> ETArrayFolder;

//素材文件列表结构体：表ET_CLIPFILE中的所有字段
typedef struct stu_ETClipFile
{
	long			m_lClipID;				//素材ID
	long			m_lQualityType;			//素材质量
	long			m_lClipClass;			//素材类型
	long			m_lClipIn;				//子素材入点
	long			m_lClipOut;				//子素材出点
	long			m_lFileIn;				//文件入点
	long			m_lFileOut;				//文件出点
	long			m_lLength;				//文件长度
	long			m_lMediaChannel;		//媒体通道
	__int64			m_lFileSize;			//文件大小
	CString			m_strFilePath;			//文件路径
	long			m_lFormatID;			//媒体格式ID
	long			m_lFileStatus;			//文件状态
	long			m_lCalcSpaceFlag;		//是否计入空间
	CString			m_strVerfiyCode;		//文件效验码
	DATE			m_dtCreateTime;			//文件创建时间
	CString			m_strStoreProtocol;		//文件存储协议
	stu_ETClipFile()
	{
		Reset();
	}
	const stu_ETClipFile& operator = (const stu_ETClipFile &src)
	{
		m_lClipID			= src.m_lClipID;
		m_lQualityType		= src.m_lQualityType;
		m_lClipClass		= src.m_lClipClass;
		m_lClipIn			= src.m_lClipIn;
		m_lClipOut			= src.m_lClipOut;
		m_lFileIn			= src.m_lFileIn;
		m_lFileOut			= src.m_lFileOut;
		m_lLength			= src.m_lLength;
		m_lMediaChannel		= src.m_lMediaChannel;
		m_lFileSize			= src.m_lFileSize;
		m_strFilePath		= src.m_strFilePath;
		m_lFormatID			= src.m_lFormatID;
		m_lFileStatus		= src.m_lFileStatus;
		m_lCalcSpaceFlag	= src.m_lCalcSpaceFlag;
		m_strVerfiyCode		= src.m_strVerfiyCode;
		m_dtCreateTime		= src.m_dtCreateTime;
		m_strStoreProtocol	= src.m_strStoreProtocol;
		return *this;
	}
	void Reset()
	{
		m_lClipID			= 0;
		m_lQualityType		= 0;
		m_lClipClass		= 0;
		m_lClipIn			= 0;
		m_lClipOut			= 0;
		m_lFileIn			= 0;
		m_lFileOut			= 0;
		m_lLength			= 0;
		m_lMediaChannel		= 0;
		m_lFileSize			= 0;
		m_strFilePath		= _T("");
		m_lFormatID			= 0;
		m_lFileStatus		= 0;
		m_lCalcSpaceFlag	= -1;
		m_strVerfiyCode		= _T("");
		m_dtCreateTime		= 0;
		m_strStoreProtocol	= _T("");
	}
	virtual ~stu_ETClipFile()
	{
		Reset();
	}
}ETClipFile,*lpETClipFile;
typedef CArray<lpETClipFile,lpETClipFile> ETArrayClipFile;

//素材标记点表结构体：表ET_CLIPMARKPOINT中的所有字段
typedef struct stu_ETClipMarkPoint
{
	long			m_lClipID;			//素材ID
	long			m_lKeyFrame;		//标记帧位置
	long			m_lEndFrame;		//标记结束点
	long			m_lAbsKeyFrame;		//标记帧位置(绝对时码)
	long			m_lAbsEndFrame;		//标记结束点(绝对时码)
	int				m_iMarkType;		//标记类型
	long			m_lIconFrame;		//标记点图标帧
	CString			m_IconFileName;		//图标文件
	int				m_iIconLevel;		//标记层次
	long			m_lMarkColor;		//标记点颜色
	CString			m_strNote;			//备注
	CString         m_strMarkGuid;      //MARK点GUID    
    int 			m_iSelected;        //是否选中 0:未选中 1:选中
	long			m_lDatabase;		//DATABASEID
	stu_ETClipMarkPoint()
	{
		Reset();
	}
	const stu_ETClipMarkPoint& operator = (const stu_ETClipMarkPoint &src)
	{
		m_lClipID			= src.m_lClipID;
		m_lKeyFrame			= src.m_lKeyFrame;
		m_lEndFrame			= src.m_lEndFrame;
		m_lAbsKeyFrame		= src.m_lAbsKeyFrame;
		m_lAbsEndFrame		= src.m_lAbsEndFrame;
		m_iMarkType			= src.m_iMarkType;
		m_lIconFrame		= src.m_lIconFrame;
		m_IconFileName		= src.m_IconFileName;
		m_iIconLevel		= src.m_iIconLevel;
		m_lMarkColor		= src.m_lMarkColor;
		m_strNote			= src.m_strNote;
		m_strMarkGuid       = src.m_strMarkGuid;
		m_iSelected			= src.m_iSelected;
		m_lDatabase			= src.m_lDatabase;
		return *this;
	}
	void Reset()
	{
		m_lClipID			= 0;
		m_lKeyFrame			= 0;
		m_lEndFrame			= 0;
		m_lAbsKeyFrame		= 0;
		m_lAbsEndFrame		= 0;
		m_iMarkType			= ET_MARKPOINT_KEYFRAME;
		m_lIconFrame		= 0;
		m_IconFileName		= _T("");
		m_iIconLevel		= 0;
		m_lMarkColor		= RGB(0, 255, 0);
		m_strNote			= _T("");
		m_strMarkGuid       = _T("");
		m_iSelected			= 0;
		m_lDatabase			= 0;
	}
	virtual ~stu_ETClipMarkPoint()
	{
		Reset();
	}
}ETClipMarkPoint,*lpETClipMarkPoint;
typedef CArray<lpETClipMarkPoint,lpETClipMarkPoint> ETArrayClipMarkPoint;

//EM Comments检索结果结构体
typedef struct stu_EMSearchResult
{
	LONG		m_lVtrInPoint;
	ETObject	m_stObjectInfo;
	ETArrayClipMarkPoint m_arClipMarkPoint;
	stu_EMSearchResult()
	{
		Reset();
	}
	const stu_EMSearchResult& operator = (const stu_EMSearchResult &src)
	{
		m_lVtrInPoint		= src.m_lVtrInPoint;
		m_stObjectInfo		= src.m_stObjectInfo;
		for (INT_PTR indexDel = 0; indexDel < m_arClipMarkPoint.GetCount(); indexDel++)
		{
			lpETClipMarkPoint pDelClipMarkPoint = m_arClipMarkPoint.GetAt(indexDel);
			if (pDelClipMarkPoint != NULL)
			{
				delete pDelClipMarkPoint;
				pDelClipMarkPoint = NULL;
			}
		}
		m_arClipMarkPoint.RemoveAll();
		for (INT_PTR indexAdd = 0; indexAdd < src.m_arClipMarkPoint.GetCount(); indexAdd++)
		{
			lpETClipMarkPoint pAddClipMarkPoint = new ETClipMarkPoint;
			*pAddClipMarkPoint = *(src.m_arClipMarkPoint.GetAt(indexAdd));
			m_arClipMarkPoint.Add(pAddClipMarkPoint);
		}
		return *this;
	}
	void Reset()
	{
		m_lVtrInPoint = 0;
		m_stObjectInfo.Reset();
		for (INT_PTR index = 0; index < m_arClipMarkPoint.GetCount(); index++)
		{
			lpETClipMarkPoint pClipMarkPoint = m_arClipMarkPoint.GetAt(index);
			if (pClipMarkPoint != NULL)
			{
				delete pClipMarkPoint;
				pClipMarkPoint = NULL;
			}
		}
		m_arClipMarkPoint.RemoveAll();
	}
	virtual ~stu_EMSearchResult()
	{
		Reset();
	}
}ETEMSearchResult, *lpETEMSearchResult;
typedef CArray<lpETEMSearchResult,lpETEMSearchResult> ETArrayEMSearchResult;

//计划信息元数据表结构体：表ET_PLANNINGMETADATA中的所有字段
typedef struct stu_ETPlanningMetaData
{
	long			m_lPlanningID;			//计划ID
	GUID			m_guidPlanningID;		//计划GUID
	CString			m_strPlanName;			//计划名
	CString			m_strCreatorName;		//创建人名
	DATE			m_dtCreateDate;			//创建日期
	CString			m_strModifyName;		//修改人名
	DATE			m_dtModifyDate;			//修改日期
	long			m_lVersion;				//版本
	CString			m_strPlace;				//地点
	DATE			m_dtPlanningDate;		//计划日期
	CString			m_strDirector;			//导演
	CString			m_strInputBy;			//录入
	CString			m_strPhotoGrapher;		//摄影
	CString			m_strReporter;			//记者
	CString			m_strOther;				//其他人员
	CString			m_strEquipment;			//设备
	CString			m_strContactInfo;		//联系信息
	CString			m_strPlanningXml;		//计划XML
	CString			m_strUserXml;
	CString			m_strInputSource;		//计划输入源
	DATE			m_dtStartDate;			//计划收录开始时间
	DATE			m_dtDurDate;			//计划收录保持时间
	ULONGLONG		m_ullDataMask;			//数据有效位定义
	LONG			m_lSourceType;			//采集源类型
	CString			m_strIngestExtendXml;         //采集分段Planning信息
	stu_ETPlanningMetaData()
	{
		Reset();
	}
	const stu_ETPlanningMetaData& operator = (const stu_ETPlanningMetaData &src)
	{
		m_lPlanningID		= src.m_lPlanningID;
		m_guidPlanningID	= src.m_guidPlanningID;
		m_strPlanName		= src.m_strPlanName;
		m_strCreatorName	= src.m_strCreatorName;
		m_dtCreateDate		= src.m_dtCreateDate;
		m_strModifyName		= src.m_strModifyName;
		m_dtModifyDate		= src.m_dtModifyDate;
		m_lVersion			= src.m_lVersion;
		m_strPlace			= src.m_strPlace;
		m_dtPlanningDate	= src.m_dtPlanningDate;
		m_strDirector		= src.m_strDirector;
		m_strInputBy		= src.m_strInputBy;
		m_strPhotoGrapher	= src.m_strPhotoGrapher;
		m_strReporter		= src.m_strReporter;
		m_strOther			= src.m_strOther;
		m_strEquipment		= src.m_strEquipment;
		m_strContactInfo	= src.m_strContactInfo;
		m_strPlanningXml	= src.m_strPlanningXml;
		m_strUserXml		= src.m_strUserXml;
		m_strInputSource	= src.m_strInputSource;
		m_dtStartDate		= src.m_dtStartDate;
		m_dtDurDate			= src.m_dtDurDate;
		m_ullDataMask		= src.m_ullDataMask;
		m_lSourceType       = src.m_lSourceType;
		m_strIngestExtendXml= src.m_strIngestExtendXml;
		return *this;
	}
	void Reset()
	{
		m_lPlanningID		=0;
		m_guidPlanningID	=GUID_NULL;
		m_strPlanName		=_T("");
		m_strCreatorName	=_T("");
		m_dtCreateDate		=0;
		m_strModifyName		=_T("");
		m_dtModifyDate		=0;
		m_lVersion			=0;
		m_strPlace			=_T("");			
		m_dtPlanningDate	=0.00;
		m_strDirector		=_T("");
		m_strInputBy		=_T("");
		m_strPhotoGrapher	=_T("");
		m_strReporter		=_T("");
		m_strOther			=_T("");
		m_strEquipment		=_T("");
		m_strContactInfo	=_T("");
		m_strPlanningXml	=_T("");
		m_strUserXml		=_T("");
		m_strInputSource	=_T("");
		m_dtStartDate		=0.00;
		m_dtDurDate			=0.00;
		m_ullDataMask		= 0XFFFFFFFFFFFFFFFF;
		m_lSourceType       = 0;
		m_strIngestExtendXml= _T("");
	}
	virtual ~stu_ETPlanningMetaData()
	{
		Reset();
	}
}ETPlanningMetaData,*LPETPlanningMetaData;
//素材收录任务元数据表结构体：表ET_INGESTMETADATA中的所有字段
typedef struct stu_ETIngestMetaData
{
	long			m_lTaskID;				//任务ID
	CString			m_strTaskName;			//任务名称
	DATE			m_dtStartDate;			//收录开始时间
	DATE			m_dtEndDate;			//收录结束时间
	DATE			m_dtDurDate;			//收录保持时间
	int				m_iStatus;				//收录状态
	int				m_iProcess;				//收录进度

	CString         m_strSourceTapeID;      //源磁带ID
	CString         m_strBackupTapeID;      //备份磁带ID
	CString			m_strUserName;			//收录用户名
	CString			m_strTerminal;			//收录终端
	//{{2009/04/22 zxp:add 添加元数据
    CString         m_strChannelName;       //收录通道名
	long            m_lDeviceID;            //收录设备ID
	int             m_iAudioChannelCount;   //音频通道数
	//}}
	CString         m_strSourceTapeBarcode; //源磁带Barcode信息

	ULONGLONG		m_ullDataMask;			//数据有效位定义

	stu_ETIngestMetaData()
	{
		Reset();
	}
	const stu_ETIngestMetaData& operator = (const stu_ETIngestMetaData &src)
	{
		m_lTaskID			= src.m_lTaskID;
		m_strTaskName		= src.m_strTaskName;
		m_dtStartDate		= src.m_dtStartDate;
		m_dtEndDate			= src.m_dtEndDate;
		m_dtDurDate			= src.m_dtDurDate;
		m_iStatus			= src.m_iStatus;
		m_iProcess			= src.m_iProcess;
		m_ullDataMask		= src.m_ullDataMask;

		m_strSourceTapeID   = src.m_strSourceTapeID;
		m_strBackupTapeID   = src.m_strBackupTapeID;
		m_strUserName		= src.m_strUserName;
		m_strTerminal		= src.m_strTerminal;
		//{{2009/04/22 zxp:add 添加元数据
		m_strChannelName    = src.m_strChannelName;
		m_lDeviceID         = src.m_lDeviceID;
		m_iAudioChannelCount= src.m_iAudioChannelCount;
		//}}
		m_strSourceTapeBarcode = src.m_strSourceTapeBarcode;

		return *this;
	}
	void Reset()
	{
		m_lTaskID			= 0;
		m_strTaskName		= _T("");
		m_dtStartDate		= 0.00;
		m_dtEndDate			= 0.00;
		m_dtDurDate			= 0.00;
		m_iStatus			= 0;
		m_iProcess			= 0;
		m_ullDataMask		= 0XFFFFFFFFFFFFFFFF;

		m_strSourceTapeID   = _T("");
		m_strBackupTapeID   = _T("");
		m_strUserName		= _T("");
		m_strTerminal		= _T("");
		//{{2009/04/22 zxp:add 添加元数据
		m_strChannelName    = _T("");
		m_lDeviceID         = 0;
		m_iAudioChannelCount= 0;
		//}}
		m_strSourceTapeBarcode = _T("");
	}
	virtual ~stu_ETIngestMetaData()
	{
		Reset();
	}
}ETIngestMetaData,*LPETIngestMetaData;

//素材元数据表结构体：表ET_CLIPINFO中的所有字段
typedef struct stu_ETClipInfo:public ETObject
{
	ET_CLIP_SOURCE_TYPE	m_lSignalSourceType;	//采集信号类型
	CString			m_strSignalSourceDesc;	//采集信号描述
	CString			m_strSignalSourceMark;	//采集信号标签
	long			m_lVtrInPoint;			//磁带入点
	long			m_lVtrOutPoint;			//磁带出点
	long			m_lVtrINext;			//磁带扩展入点
	long			m_lVtrOutNext;			//磁带扩展出点
	CString			m_strStorageCode;		//存储编码
	int				m_iKeepDays;			//素材在非编库中的保留天数
	ULONGLONG		m_lClipSource;			//素材的入库来源
	long			m_lIngestTaskID;		//收录任务ID
	long			m_lPlanningID;			//计划ID
	CString			m_strTC;				//时码
	int				m_iVersion;				//版本
	CString			m_strClipLanguage;		//语种
	LPBYTE			m_lpSerialData;			//TGA数据
	long			m_lSerialSize;			//TGA长度
	GUID			m_guidSrcPgmID;			//原节目ID
	CString			m_strItemName;			//
	CString			m_strCategory;			//分类
	CString			m_strProgramname;		// 
	ETArrayClipFile								m_arClipFile;			//文件列表
	ETArrayClipMarkPoint						m_arClipMarkPoint;		//素材标记点
	ULONGLONG		m_ullDataMask;			//数据有效位定义
	long			m_lSrcSiteID;			//来源站点ID,为0表示本地站点。
	long			m_lRestrictViewContent;	//限制察看素材内容
	CString         m_strOriginalMaterial;  //2009/04/21 zxp:add 源文件名

	ET_RUNTIME_CLASS(ETObject, ET_RT_CLASS_ETCLIPINFO)
	stu_ETClipInfo()
	{
		m_lpSerialData = NULL;
		m_lSerialSize = 0;
		Reset();
	}
	void CopyByMask(const stu_ETClipInfo &src)
	{
		stu_ETObject::CopyByMask(src);
		ULONGLONG ullDataMask = src.m_ullDataMask;
		//采集信号类型
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SignalSourceType))
			m_lSignalSourceType = src.m_lSignalSourceType;

		//采集信号描述
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SignalSourceDesc))
			m_strSignalSourceDesc = src.m_strSignalSourceDesc;

		//采集信号标签
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SignalSourceMark))
			m_strSignalSourceMark = src.m_strSignalSourceMark;

		//磁带入点
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_VtrInPoint))
			m_lVtrInPoint = src.m_lVtrInPoint;

		//磁带出点
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_VtrOutPoint))
			m_lVtrOutPoint = src.m_lVtrOutPoint;

		//磁带扩展入点
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_VtrInNext))
			m_lVtrINext = src.m_lVtrINext;

		//磁带扩展出点
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_VtrOutNext))
			m_lVtrOutNext = src.m_lVtrOutNext;

		//存储编码
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_StorageCode))
			m_strStorageCode = src.m_strStorageCode;

		//素材在非编库中的保留天数
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_KeepDays))
			m_iKeepDays = src.m_iKeepDays;

		//素材的入库来源
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_ClipSource))
			m_lClipSource = src.m_lClipSource;

		//收录任务ID
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_IngestTaskID))
			m_lIngestTaskID = src.m_lIngestTaskID;

		//计划ID
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_PlanningID))
			m_lPlanningID = src.m_lPlanningID;

		//时码
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_TC))
			m_strTC = src.m_strTC;

		//版本
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_Version))
			m_iVersion = src.m_iVersion;

		//语种
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_ClipLanguage))
			m_strClipLanguage = src.m_strClipLanguage;

		//TGA数据
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SerialData))
		{
			m_lpSerialData = src.m_lpSerialData;
		}

		//TGA长度
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SerialSize))
			m_lSerialSize = src.m_lSerialSize;

		if (FIELD_ISSET(ullDataMask, CLP_FIELD_ItemName))
			m_strItemName = src.m_strItemName;

		if (FIELD_ISSET(ullDataMask, CLP_FIELD_ProgramName))
			m_strProgramname = src.m_strProgramname;

		if (FIELD_ISSET(ullDataMask, CLP_FIELD_Category))
			m_strCategory = src.m_strCategory;
		
		//2009/04/21 zxp:add 源文件名
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_OriginalMaterial))
			m_strOriginalMaterial = src.m_strOriginalMaterial;
	}

	const stu_ETClipInfo& operator = (const stu_ETObject &src)
	{
		stu_ETObject::operator =(src);
		return *this;
	}
	const stu_ETClipInfo& operator = (const stu_ETClipInfo &src)
	{
		stu_ETObject::operator =(src);

		m_lSignalSourceType		=src.m_lSignalSourceType;
		m_strSignalSourceDesc	=src.m_strSignalSourceDesc;
		m_strSignalSourceMark	=src.m_strSignalSourceMark;
		m_lVtrInPoint			=src.m_lVtrInPoint;
		m_lVtrOutPoint			=src.m_lVtrOutPoint;
		m_lVtrINext				=src.m_lVtrINext;
		m_lVtrOutNext			=src.m_lVtrOutNext;
		m_strStorageCode		=src.m_strStorageCode;
		m_iKeepDays				=src.m_iKeepDays;
		m_lClipSource			=src.m_lClipSource;
		m_lIngestTaskID			=src.m_lIngestTaskID;
		m_lPlanningID			=src.m_lPlanningID;
		m_strTC					=src.m_strTC;
		m_iVersion				=src.m_iVersion;
		m_strClipLanguage		=src.m_strClipLanguage;
		m_guidSrcPgmID			=src.m_guidSrcPgmID;
		m_ullDataMask			=src.m_ullDataMask;
		m_strItemName			=src.m_strItemName; //
		m_strCategory           =src.m_strCategory;//分类
		m_strProgramname        =src.m_strProgramname;		// 
		m_lSrcSiteID			=src.m_lSrcSiteID;
		m_lRestrictViewContent	=src.m_lRestrictViewContent;
		m_strOriginalMaterial   =src.m_strOriginalMaterial;//2009/04/21 zxp:add 源文件名

		if (m_lpSerialData != NULL)
		{
			delete []m_lpSerialData;
			m_lpSerialData = NULL;
			m_lSerialSize = 0;
		}
		if(src.m_lSerialSize > 0&&src.m_lpSerialData != NULL)
		{
			m_lSerialSize           =src.m_lSerialSize;
			m_lpSerialData			= new BYTE[m_lSerialSize];
			memcpy(m_lpSerialData,src.m_lpSerialData,m_lSerialSize);
		}

		int i,nCount;
		lpETClipFile lpFile,lpNewFile;
		nCount = (int)m_arClipFile.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpFile = m_arClipFile.GetAt(i);
			delete lpFile;
		}
		m_arClipFile.RemoveAll();

		nCount = (int)src.m_arClipFile.GetCount();
		for(i = 0;i < nCount;i++)
		{
			lpFile = src.m_arClipFile.GetAt(i);
			lpNewFile = new ETClipFile;
			*lpNewFile = *lpFile;
			m_arClipFile.Add(lpNewFile);
		}

		lpETClipMarkPoint lpMarkPoint,lpNewMarkPoint;
		nCount = (int)m_arClipMarkPoint.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpMarkPoint = m_arClipMarkPoint.GetAt(i);
			delete lpMarkPoint;
		}
		m_arClipMarkPoint.RemoveAll();

		nCount = (int)src.m_arClipMarkPoint.GetCount();
		for(i = 0;i < nCount;i++)
		{
			lpMarkPoint = src.m_arClipMarkPoint.GetAt(i);
			lpNewMarkPoint = new ETClipMarkPoint;
			*lpNewMarkPoint = *lpMarkPoint;
			m_arClipMarkPoint.Add(lpNewMarkPoint);
		}
		return *this;
	}
	void Reset()
	{
		ETObject::Reset();
		if (m_lpSerialData != NULL)
		{
			delete []m_lpSerialData;
			m_lpSerialData = NULL;
			m_lSerialSize = 0;
		}
		m_lSignalSourceType		=	0;
		m_strSignalSourceDesc	=	_T("");
		m_strSignalSourceMark	=	_T("");
		m_lVtrInPoint			=	0;
		m_lVtrOutPoint			=	0;
		m_lVtrINext				=	0;
		m_lVtrOutNext			=	0;
		m_strStorageCode		=	_T("");
		m_iKeepDays				=	0;
		m_lClipSource			=	0;
		m_lIngestTaskID			=	0;
		m_lPlanningID			=	0;
		m_strTC					=	_T("");
		m_iVersion				=   0;
		m_strClipLanguage		=   _T("");
		m_strRights				=	_T("");
		m_guidSrcPgmID			= GUID_NULL;
		m_strItemName			=	_T(""); //
		m_strCategory           =	_T("");//分类
		m_strProgramname        =	_T("");		// 
		m_ullDataMask			= 0XFFFFFFFFFFFFFFFF;
		m_lSrcSiteID			= 0;
		m_lRestrictViewContent	= 0;
		m_strOriginalMaterial   = _T("");//2009/04/21 zxp:add 源文件名

		
		int i,nCount;
		lpETClipFile lpFile;
		nCount = (int)m_arClipFile.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpFile = m_arClipFile.GetAt(i);
			delete lpFile;
		}
		m_arClipFile.RemoveAll();

		lpETClipMarkPoint lpMarkPoint;
		nCount = (int)m_arClipMarkPoint.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpMarkPoint = m_arClipMarkPoint.GetAt(i);
			delete lpMarkPoint;
		}
		m_arClipMarkPoint.RemoveAll();
	}
	virtual ~stu_ETClipInfo()
	{	
		Reset();
	}
}ETClipInfo,*lpETClipInfo;
typedef CArray<lpETClipInfo,lpETClipInfo&> ETArrayClipInfo;

//节目元数据结构体：表ET_PGMMASTER中的所有字段
typedef struct stu_ETPgmMaster:public ETObject
{
	CString			m_strClipPath;			//素材存放路径
	int				m_iVideoTrack;			//视频轨数
	int				m_iAudioTrack;			//音频轨数
	int				m_iGraphTrack;			//字幕轨数
	int				m_iBackTrack;			//背景轨数
	int				m_iAudioChannel;		//输出音频通道数
	int				m_iTLBlock;				//是否有子时间线
	CString			m_strFilePath;			//文件路径
	CString			m_strBackFilePath;		//备份文件路径
	CString			m_strUndoFilePath;		//UNDO数据路径
	long			m_lVideoFormatID;		//视频格式ID
	long			m_lAudioFormatID;		//音频格式ID
	int				m_iSaveState;			//节目保存状态
	long			m_lEditTime;			//编辑累计时长
	long			m_lDubUserID;			//配音操作员ID
	long			m_lDubAnnouncerID;		//配音员ID
	int				m_iSECStatus;			//节目辅助状态
	long			m_lSysVersion;			//编辑系统版本
	long			m_lEditVersion;			//节目修改的版本号
	ULONGLONG		m_ullDataMask;			//数据有效位定义
	long			m_lForceRefreshFlag;	//强制刷新标志

	ET_RUNTIME_CLASS(ETObject, ET_RT_CLASS_ETPGMMASTER)
	stu_ETPgmMaster()
	{
		Reset();
	}
	const stu_ETPgmMaster& operator = (const stu_ETObject &src)
	{
		stu_ETObject::operator =(src);

		return *this;
	}
	const stu_ETPgmMaster& operator = (const stu_ETPgmMaster &src)
	{
		stu_ETObject::operator =(src);

		m_strClipPath		= src.m_strClipPath;
		m_iVideoTrack		= src.m_iVideoTrack;
		m_iAudioTrack		= src.m_iAudioTrack;
		m_iGraphTrack		= src.m_iGraphTrack;
		m_iBackTrack		= src.m_iBackTrack;
		m_iAudioChannel		= src.m_iAudioChannel;
		m_iTLBlock			= src.m_iTLBlock;
		m_strFilePath		= src.m_strFilePath;
		m_strBackFilePath	= src.m_strBackFilePath;
		m_strUndoFilePath	= src.m_strUndoFilePath;
		m_lVideoFormatID	= src.m_lVideoFormatID;
		m_lAudioFormatID	= src.m_lAudioFormatID;
		m_iSaveState		= src.m_iSaveState;
		m_lEditTime			= src.m_lEditTime;
		m_lDubUserID		= src.m_lDubUserID;
		m_lDubAnnouncerID	= src.m_lDubAnnouncerID;
		m_iSECStatus		= src.m_iSECStatus;
		m_lSysVersion		= src.m_lSysVersion;
		m_lEditVersion		= src.m_lEditVersion;
		m_ullDataMask		= src.m_ullDataMask;
		m_lForceRefreshFlag = src.m_lForceRefreshFlag;
		return *this;
	}
	void Reset()
	{
		ETObject::Reset();
		m_strClipPath		= _T("");
		m_iVideoTrack		= 0;	
		m_iAudioTrack		= 0;
		m_iGraphTrack		= 0;
		m_iBackTrack		= 0;
		m_iAudioChannel		= 0;
		m_iTLBlock			= 0;
		m_strFilePath		= _T("");
		m_strBackFilePath	= _T("");
		m_strUndoFilePath	= _T("");
		m_lVideoFormatID	= 0;
		m_lAudioFormatID	= 0;
		m_iSaveState		= 0;
		m_lEditTime			= 0;
		m_lDubUserID		= 0;
		m_lDubAnnouncerID	= 0;
		m_iSECStatus		= 0;
		m_lSysVersion		= 0;
		m_lEditVersion		= 0;
		m_ullDataMask		= 0XFFFFFFFFFFFFFFFF;
		m_lForceRefreshFlag = 0;
	}
	virtual ~stu_ETPgmMaster()
	{
		Reset();
	}
}ETPgmMaster,*LPETPgmMaster;
typedef CArray<LPETPgmMaster,LPETPgmMaster> ETArrayPgmMaster;

//非编的模版对象信息表结构体：表ET_TEMPLATEOBJECT中的所有字段
typedef struct stu_ETTemplateObject:public ETObject
{
	
	GUID			m_guidTypeGuid;		//模版类型GUID	
	CString			m_strFilePath;		//模版引用的文件名	
	long			m_lResource;		//模版来源类型
	LPBYTE			m_lpTempData;		//模版数据
	long			m_lTempSize;		//模版数据大小
	ULONGLONG		m_ullDataMask;		//数据有效位定义

	ET_RUNTIME_CLASS(ETObject, ET_RT_CLASS_ETTEMPLATEOBJECT)
	stu_ETTemplateObject()
	{
		m_lpTempData = NULL;
		Reset();
	}
	const stu_ETTemplateObject& operator = (const stu_ETObject &src)
	{
		stu_ETObject::operator =(src);
		return *this;
	}
	const stu_ETTemplateObject& operator = (const stu_ETTemplateObject &src)
	{
		stu_ETObject::operator =(src);

		m_guidTypeGuid	= src.m_guidTypeGuid;
		m_strFilePath	= src.m_strFilePath;
		m_lResource		= src.m_lResource;
		m_ullDataMask	= src.m_ullDataMask;
		if (m_lpTempData != NULL)
		{
			delete [] m_lpTempData;
			m_lpTempData = NULL;
			m_lTempSize = 0;
		}
		if(src.m_lTempSize > 0 && src.m_lpTempData != NULL)
		{
			m_lTempSize		= src.m_lTempSize;
			m_lpTempData	= new BYTE[m_lTempSize];
			memcpy(m_lpTempData,src.m_lpTempData,m_lTempSize);
		}
		return *this;
	}
	void Reset()
	{
		ETObject::Reset();
		if (m_lpTempData != NULL)
		{
			delete [] m_lpTempData;
			m_lpTempData = NULL;
		}		
		m_guidTypeGuid	= GUID_NULL;			
		m_strFilePath	= _T("");	
		m_lResource		= 0;
		m_lpTempData	= NULL;
		m_lTempSize		= 0;
		m_ullDataMask		= 0XFFFFFFFFFFFFFFFF;
	}
	virtual ~stu_ETTemplateObject()
	{
		Reset();
	}
}ETTemplateObject,*lpETTemplateObject;
typedef CArray<lpETTemplateObject,lpETTemplateObject> ETArrayTemplateObject;

//蓝光盘信息结构体：表ET_CDINFO中的所有字段
typedef struct stu_ETCDInfo:public ETObject
{
	CString			m_strUpLoadIp;		//光盘上载地址
	long			m_lDeviceID;		//设备ID
	CString			m_strMainTitle;		//光盘标题

	ET_RUNTIME_CLASS(ETObject, ET_RT_CLASS_ETCDINFO)
	stu_ETCDInfo()
	{
		Reset();
	}
	const stu_ETCDInfo& operator = (const stu_ETObject &src)
	{
		stu_ETObject::operator =(src);
		return *this;
	}
	const stu_ETCDInfo& operator = (const stu_ETCDInfo &src)
	{
		stu_ETObject::operator =(src);

		m_strUpLoadIp	=	src.m_strUpLoadIp;
		m_lDeviceID		=	src.m_lDeviceID;
		m_strMainTitle	=	src.m_strMainTitle;	
		return *this;
	}
	void Reset()
	{
		ETObject::Reset();
		m_strUpLoadIp	=	_T("");
		m_lDeviceID		=	0;
		m_strMainTitle	=	_T("");
	}
	virtual ~stu_ETCDInfo()
	{
		Reset();
	}
}ETCDInfo,*lpETCDInfo;
typedef CArray<lpETCDInfo,lpETCDInfo> ETArrayCDInfo;

//权限定义表
typedef struct stu_ETObjectPopedom
{
	long				m_lPopedomID;		// 权限  ID
	long				m_lOwnObjectID;		// 拥有者ID
	emObjectType		m_emOwnObjectType;	// ET_ObjType_Folder
	ET_OBJECT_POPEDOM	m_emReadPopedom;	// 访问权限 0-公开,1-私有,2-本组,3-条件
	ET_OBJECT_POPEDOM	m_emModifyPopedom;	// 修改权限 0-公开,1-私有,2-本组,3-条件
	CMap<ULONGLONG,ULONGLONG,long,long> 	m_mapReadData;
	CMap<ULONGLONG,ULONGLONG,long,long>		m_mapModifyData;
	stu_ETObjectPopedom(void)
	{
		ReSet();
	}
	void ReSet()
	{
		m_lPopedomID		= 0;
		m_lOwnObjectID		= 0;
		m_emOwnObjectType	= ET_ObjType_Unknow;
		m_emReadPopedom		= ET_OBJECT_POPEDOM_PUBLIC;
		m_emModifyPopedom	= ET_OBJECT_POPEDOM_PUBLIC;
		m_mapReadData.RemoveAll();
		m_mapModifyData.RemoveAll();
	}
	~stu_ETObjectPopedom(void)
	{
		m_mapReadData.RemoveAll();
		m_mapModifyData.RemoveAll();
	}
	const stu_ETObjectPopedom& operator = (const stu_ETObjectPopedom &src)
	{
		POSITION pos;
		long lVisitorID;
		ULONGLONG ullKey;
		m_lPopedomID			= src.m_lPopedomID;
		m_lOwnObjectID			= src.m_lOwnObjectID;
		m_emOwnObjectType		= src.m_emOwnObjectType;
		m_emReadPopedom			= src.m_emReadPopedom;
		m_emModifyPopedom		= src.m_emModifyPopedom;
		pos = src.m_mapReadData.GetStartPosition();
		if(pos != NULL)
		{
			do
			{
				src.m_mapReadData.GetNextAssoc(pos,ullKey,lVisitorID);
				m_mapReadData.SetAt(ullKey,lVisitorID);
			} while(pos);
		}
		pos = src.m_mapModifyData.GetStartPosition();
		if(pos != NULL)
		{
			do
			{
				src.m_mapModifyData.GetNextAssoc(pos,ullKey,lVisitorID);
				m_mapModifyData.SetAt(ullKey,lVisitorID);
			} while(pos);
		}
		return *this;
	}
	void AddPopedomData(long lVisitorID,long lVisitorType,long lOperateType)
	{
		ULONGLONG ulKey = ETMAKEULONGLONG(lVisitorID,lVisitorType);
		if(lOperateType == ET_OBJECT_POPEDOMTYPE_READ)
			m_mapReadData.SetAt(ulKey,lVisitorID);
		else if(lOperateType == ET_OBJECT_POPEDOMTYPE_MODIFY)
			m_mapModifyData.SetAt(ulKey,lVisitorID);
	}
}ETObjectPopedom, *LPETObjectPopedom;

//EDL元数据表结构体：表ET_EDLINFO中的所有字段
typedef struct stu_ETEDLInfo:public ETObject
{
	CString			m_strEDLFileName;		//EDL数据
	CString			m_strVideoFormat;		//视频格式
	int				m_iVideoTrack;			//视频轨数
	int				m_iAudioTrack;			//音频轨数
	int				m_iGraphTrack;			//字幕轨数

	ET_RUNTIME_CLASS(ETObject, ET_RT_CLASS_ETEDLINFO)
	stu_ETEDLInfo()
	{
		Reset();
	}
	const stu_ETEDLInfo& operator = (const stu_ETObject &src)
	{
		stu_ETObject::operator =(src);
		return *this;
	}
	const stu_ETEDLInfo& operator = (const stu_ETEDLInfo &src)
	{
		stu_ETObject::operator =(src);

		m_strEDLFileName	= src.m_strEDLFileName;
		m_strVideoFormat	= src.m_strVideoFormat;
		m_iVideoTrack		= src.m_iVideoTrack;
		m_iAudioTrack		= src.m_iAudioTrack;
		m_iGraphTrack		= src.m_iGraphTrack;
		return *this;
	}
	void Reset()
	{
		ETObject::Reset();
		m_strEDLFileName		= _T("");
		m_strVideoFormat		= _T("");
		m_iVideoTrack			= 0;
		m_iAudioTrack			= 0;
		m_iGraphTrack			= 0;
	}
	virtual ~stu_ETEDLInfo()
	{
		Reset();
	}
}ETEDLInfo,*lpETEDLInfo;
typedef CArray<lpETEDLInfo,lpETEDLInfo> ETArrayEDLInfo;

//回收站对象表结构体：表ET_DELETEOBJECT中的所有字段
typedef struct stu_ETDeleteObject
{
	long			m_lObjectID;		//对象ID
	long			m_lUserID;			//用户ID
	long			m_lSrcFolderID;		//原文件夹ID
	stu_ETDeleteObject()
	{
		Reset();
	}
	const stu_ETDeleteObject& operator = (const stu_ETDeleteObject &src)
	{
		m_lObjectID			= src.m_lObjectID;
		m_lUserID			= src.m_lUserID;
		m_lSrcFolderID		= src.m_lSrcFolderID;
		return *this;
	}
	void Reset()
	{
		m_lObjectID			= 0;
		m_lUserID			= 0;
		m_lSrcFolderID		= 0;
	}
	virtual ~stu_ETDeleteObject()
	{
		Reset();
	}
}ETDeleteObject,*lpETDeleteObject;

//我的常用对象表结构体：表ETMyCommonObject中的所有字段
typedef struct stu_ETMyCommonObject
{
	long			m_lObjectID;		//对象ID
	long			m_lUserID;			//用户ID
	stu_ETMyCommonObject()
	{
		Reset();
	}
	const stu_ETMyCommonObject& operator = (const stu_ETMyCommonObject &src)
	{
		m_lObjectID			= src.m_lObjectID;
		m_lUserID			= src.m_lUserID;
		return *this;
	}
	void Reset()
	{
		m_lObjectID			= 0;
		m_lUserID			= 0;
	}
	virtual ~stu_ETMyCommonObject()
	{
		Reset();
	}
}ETMyCommonObject,*lpETMyCommonObject;

//节目的MPC处理结果表结构体：表ET_PGMRENDERDATA中的所有字段
typedef struct  stu_ETPgmRenderData
{
	long			m_lPgmID;			//节目ID
	CString			m_strGroupType;		//组类型
	CString			m_strMediaType;		//媒体类型
	CString			m_strFilePath;		//文件路径
	long			m_lTrimIn;			//入点
	long			m_lTrimOut;			//出点
	long			m_lLength;			//长度
	__int64			m_llFileSize;		//文件大小
	long			m_lVideoMediaFormatID;	//视频媒体格式ID
	long			m_lAudioMediaFormatID;	//音频媒体格式ID


	stu_ETPgmRenderData()
	{
		Reset();
	}

	stu_ETPgmRenderData(const stu_ETPgmRenderData &src)
	{
		m_lPgmID			= src.m_lPgmID;
		m_strGroupType		= src.m_strGroupType;		 
		m_strMediaType		= src.m_strMediaType;
		m_strFilePath		= src.m_strFilePath;
		m_lTrimIn			= src.m_lTrimIn;
		m_lTrimOut			= src.m_lTrimOut;
		m_lLength			= src.m_lLength;
		m_llFileSize		= src.m_llFileSize;
		m_lVideoMediaFormatID	= src.m_lVideoMediaFormatID;
		m_lAudioMediaFormatID	= src.m_lAudioMediaFormatID;
	}

	const stu_ETPgmRenderData& operator = (const stu_ETPgmRenderData &src)
	{
		m_lPgmID			= src.m_lPgmID;
		m_strGroupType		= src.m_strGroupType;		 
		m_strMediaType		= src.m_strMediaType;
		m_strFilePath		= src.m_strFilePath;
		m_lTrimIn			= src.m_lTrimIn;
		m_lTrimOut			= src.m_lTrimOut;
		m_lLength			= src.m_lLength;
		m_llFileSize		= src.m_llFileSize;
		m_lVideoMediaFormatID	= src.m_lVideoMediaFormatID;
		m_lAudioMediaFormatID	= src.m_lAudioMediaFormatID;
		return *this;
	}
	void Reset()
	{
		m_lPgmID			= 0;
		m_strGroupType		= _T("");		 
		m_strMediaType		= _T("");
		m_strFilePath		= _T("");
		m_lTrimIn			= 0;
		m_lTrimOut			= 0;
		m_lLength			= 0;
		m_llFileSize		= 0;
		m_lVideoMediaFormatID	= 0;
		m_lAudioMediaFormatID	= 0;
	}
	virtual ~stu_ETPgmRenderData()
	{
		Reset();
	}
}ETPgmRenderData,*lpETPgmRenderData;
//add by dyc
typedef struct stu_ETMPCPolicyData
{
	long	m_lPolicyID;
	CString m_strPolicyName;
	stu_ETMPCPolicyData()
	{
		Reset();
	}
	const stu_ETMPCPolicyData& operator = (const stu_ETMPCPolicyData &src)
	{
		m_lPolicyID = src.m_lPolicyID;
		m_strPolicyName = src.m_strPolicyName;
		return *this;
	}
	void Reset()
	{
		m_lPolicyID = 0;
		m_strPolicyName = _T("");
	}
	virtual ~stu_ETMPCPolicyData()
	{
		Reset();
	}
}ETMPCPolicyData,*lpETMPCPolicyData;
typedef CArray<lpETMPCPolicyData,lpETMPCPolicyData> ETArraylpMPCPolicyData;
typedef CArray<ETMPCPolicyData,ETMPCPolicyData&> ETArrayMPCPolicyData;
//节目的自动技审结果表结构体：表ET_TECHCENSORDATA中的所有字段
typedef struct stu_ETTechCensorData
{
	long			m_lPgmID;			//节目ID
	long 			m_lSerialID;		//序号
	long			m_lCensorType;		//技审问题类型
	long			m_lBeginFrame;		//开始帧
	long			m_lEndFrame;		//结束帧
	stu_ETTechCensorData()
	{
		Reset();
	}
	const stu_ETTechCensorData& operator = (const stu_ETTechCensorData &src)
	{
		m_lPgmID			= src.m_lPgmID;
		m_lSerialID			= src.m_lSerialID;
		m_lCensorType		= src.m_lCensorType;
		m_lBeginFrame		= src.m_lBeginFrame;
		m_lEndFrame			= src.m_lEndFrame;
		return *this;
	}
	void Reset()
	{
		m_lPgmID			= 0;
		m_lSerialID			= 0;
		m_lCensorType		= 0;
		m_lBeginFrame		= 0;
		m_lEndFrame			= 0;
	}
	virtual ~stu_ETTechCensorData()
	{
		Reset();
	}
}ETTechCensorData,*lpETTechCensorData;
//节目操作流程表结构体：表ET_PGMFLOW中的所有字段
typedef struct stu_ETPgmFlow
{
	long			m_lPgmID;			//节目ID
	long			m_lSerialID;		//操作序号
	long			m_lUserID;			//操作用户ID
	DATE			m_dtOperDate;		//操作日期
	CString			m_strUserAction;	//操作动作
	stu_ETPgmFlow()
	{
		Reset();
	}
	const stu_ETPgmFlow& operator = (const stu_ETPgmFlow &src)
	{
		m_lPgmID			= src.m_lPgmID;
		m_lSerialID			= src.m_lSerialID;
		m_lUserID			= src.m_lUserID;
		m_dtOperDate		= src.m_dtOperDate;
		m_strUserAction		= src.m_strUserAction;
		return *this;
	}
	void Reset()
	{
		m_lPgmID			= 0;
		m_lSerialID			= 0;
		m_lUserID			= 0;
		m_dtOperDate		= 0.00;
		m_strUserAction		= _T("");
	}
	virtual ~stu_ETPgmFlow()
	{
		Reset();
	}
}ETPgmFlow,*lpETPgmFlow;

//节目审查意见表结构体：表ET_CENSORINFO中的所有字段
typedef struct stu_ETCensorInfo
{
	long			m_lPgmID;			//节目ID
	long			m_lCensorType;		//审查类型
	long			m_lCensorUserID;	//审查用户ID
	DATE			m_dtCensorDate;		//审查日期
	CString			m_strContent;		//审查意见内容
	CString			m_strAudioContent;	//审查声音意见保存路径
	stu_ETCensorInfo()
	{
		Reset();
	}
	const stu_ETCensorInfo& operator = (const stu_ETCensorInfo &src)
	{
		m_lPgmID			= src.m_lPgmID;
		m_lCensorType		= src.m_lCensorType;
		m_lCensorUserID		= src.m_lCensorUserID;
		m_dtCensorDate		= src.m_dtCensorDate;
		m_strContent		= src.m_strContent;
		m_strAudioContent	= src.m_strAudioContent;
		return *this;
	}
	void Reset()
	{
		m_lPgmID			= 0;
		m_lCensorType		= 0;
		m_lCensorUserID		= 0;
		m_dtCensorDate		= 0.00;
		m_strContent		= _T("");
		m_strAudioContent	= _T("");
	}
	virtual ~stu_ETCensorInfo()
	{
		Reset();
	}
}ETCensorInfo,*lpETCensorInfo;
//存储定义表结构体：表ET_STORAGEDEF中的所有字段
typedef struct stu_ETStorageDef
{
	CString			m_strStorageCode;		//存储编码
	CString			m_strStorageName;		//存储名称
	CString			m_strStorageNote;		//存储说明
	stu_ETStorageDef()
	{
		Reset();
	}
	const stu_ETStorageDef& operator = (const stu_ETStorageDef &src)
	{
		m_strStorageCode		= src.m_strStorageCode;
		m_strStorageName		= src.m_strStorageName;
		m_strStorageNote		= src.m_strStorageNote;
		return *this;
	}
	void Reset()
	{
		m_strStorageCode		= _T("");
		m_strStorageName		= _T("");
		m_strStorageNote		= _T("");
	}
	virtual ~stu_ETStorageDef()
	{
		Reset();
	}
}ETStorageDef,*lpETStorageDef;
//蓝光设备表结构体：表ET_DEVICEINFO中的所有字段
typedef struct stu_ETDeviceInfo
{
	long			m_lDeviceID;		//设备ID
	CString			m_strDeviceName;	//设备名	
	CString			m_strDeviceIP;		//设备IP
	CString			m_strLoginName;		//登录名
	CString			m_strPassword;		//登录口令
	GUID			m_guidDeviceID;		//设备guid
	long			m_lAudioChannel;	//
	long			m_lDatabase;
	long			m_lDeviceType;		//设备类型

	stu_ETDeviceInfo()
	{
		Reset();
	}
	const stu_ETDeviceInfo& operator = (const stu_ETDeviceInfo &src)
	{
		m_lDeviceID			= src.m_lDeviceID;
		m_strDeviceName		= src.m_strDeviceName;
		m_strDeviceIP		= src.m_strDeviceIP;
		m_strLoginName		= src.m_strLoginName;
		m_strPassword		= src.m_strPassword;
		m_guidDeviceID		= src.m_guidDeviceID;
		m_lAudioChannel		= src.m_lAudioChannel;
		m_lDatabase			= src.m_lDatabase;
		m_lDeviceType		= src.m_lDeviceType;
		return *this;
	}
	void Reset()
	{
		m_lDeviceID			= 0;
		m_strDeviceName		= _T("");
		m_strDeviceIP		= _T("");
		m_strLoginName		= _T("");
		m_strPassword		= _T("");
		m_guidDeviceID		= GUID_NULL;
		m_lAudioChannel		= 0;
		m_lDatabase			= 0;
		m_lDeviceType		= 0;
	}
	virtual ~stu_ETDeviceInfo()
	{
		Reset();
	}
}ETDeviceInfo,*lpETDeviceInfo;
typedef CArray<lpETDeviceInfo,lpETDeviceInfo> ETArrayDeviceInfo;


typedef struct stu_ETOrgAttribute
{
	// data
	CString m_strAttribute;
	CString m_strItem;
	CString m_strValue;


	// function
	stu_ETOrgAttribute()
	{

	}

	stu_ETOrgAttribute(const stu_ETOrgAttribute &src)
	{
		DeepCopy(src);
	}

	virtual const stu_ETOrgAttribute &operator = (const stu_ETOrgAttribute &src)
	{
		DeepCopy(src);
		return *this;
	}

	void DeepCopy(const stu_ETOrgAttribute &src)
	{
		m_strAttribute = src.m_strAttribute;
		m_strItem = src.m_strItem;
		m_strValue = src.m_strValue;
	}

} ETOrgAttribute;

typedef struct stu_ETOrgMetaData
{
	// data
	long m_lClipID;
	CString m_strUserMediaID;
	CString m_strMainTitle;
	CString m_strOrgComment;
	CString m_strClipTitle;
	long m_lDuration;
	long m_lSTC;
	CString m_strVideoFormat;
	CString m_strAudioFormat;
	CString m_strDescription;
	CString m_strOrgUMID;
	CString m_strBasicUMID;
	CString m_strStatus;
	CString	m_strCreatorName;
	DATE	m_dtCreateDate;
	CArray<ETOrgAttribute, ETOrgAttribute &> m_arOrgAttribute;
	ULONGLONG m_ullDataMask; //2009/02/27 zxp:add 数据有效位定义

	// function
	stu_ETOrgMetaData()
		: m_lClipID(0),
		m_lDuration(0),
		m_lSTC(0),
		m_dtCreateDate(0.0f),
		m_ullDataMask(0XFFFFFFFFFFFFFFFF)//2009/02/27 zxp:add 数据有效位初始化
	{
	}

	stu_ETOrgMetaData(const stu_ETOrgMetaData &src)
	{
		DeepCopy(src);
	}

	virtual const stu_ETOrgMetaData &operator = (const stu_ETOrgMetaData &src)
	{
		DeepCopy(src);
		return *this;
	}

	void DeepCopy(const stu_ETOrgMetaData &src)
	{
		m_lClipID = src.m_lClipID;
		m_strUserMediaID = src.m_strUserMediaID;
		m_strMainTitle = src.m_strMainTitle;
		m_strOrgComment = src.m_strOrgComment;
		m_strClipTitle = src.m_strClipTitle;
		m_lDuration = src.m_lDuration;
		m_lSTC = src.m_lSTC;
		m_strVideoFormat = src.m_strVideoFormat;
		m_strAudioFormat = src.m_strAudioFormat;
		m_strDescription = src.m_strDescription;
		m_strOrgUMID = src.m_strOrgUMID;
		m_strBasicUMID = src.m_strBasicUMID;
		m_strStatus = src.m_strStatus;
		m_strCreatorName = src.m_strCreatorName;
		m_dtCreateDate = src.m_dtCreateDate;
		m_arOrgAttribute.Copy(src.m_arOrgAttribute);
		m_ullDataMask = src.m_ullDataMask;//2009/02/27 zxp:add 数据有效位
	}

} ETOrgMetaData, *lpETOrgMetaData;

typedef struct stu_ETNetMsgCallbackHeader
{
	long		m_lOperation;			// 操作类型，1：新建，2：修改，3：删除
	LONGLONG	m_llRootSubType;		// 根节点的SubType。
	__int64		m_llObjDataMask;		// 标明ETObject结构体中修改的数据项
	__int64		m_llSubDataMask;		// 标明ETObject的派生类中修改的数据项
	long		m_lLoginID;
	long		m_lExtra;
	long		m_lDatabase;
}ETNetMsgCallbackHeader, *lpETNetMsgCallbackHeader;

typedef struct stu_ETEventMetaData 
{
	long			m_lClipID;			// 素材ID
	CString			m_strEventID;		// EVENTID
	CString			m_strEventTitle;	// 标题
	CString			m_strComments;		// 备注
	long			m_lSOM;				// 入点
	long			m_lEOM;				// 出点
	long			m_lDuration;		// 长度
	long			m_lChannel;			// 通道
	long			m_lVersion;			// 版本
	CString			m_strMOSID;			// MOS项ID
	DATE			m_dtLastUpdate;		// 最后修改日期
	DATE			m_dtCreateDate;		// 创建时间
	CString			m_strUpdateUser;	// 修改人
	CString			m_strUpdateTerminal;// 修改终端
	DATE			m_dtPlayoutStart;	// 播出开始时间
	DATE			m_dtPlayoutEnd;		// 播出结束时间
	CString			m_strStoryTitle;	// STORYTITLE
	CString			m_strProgramTitle;	// PROGRAMTITLE

	stu_ETEventMetaData()
		: m_lClipID(0)
		, m_lSOM(0)
		, m_lEOM(0)
		, m_lDuration(0)
		, m_lChannel(0)
		, m_lVersion(0)
		, m_dtLastUpdate(0.0f)
		, m_dtCreateDate(0.0f)
		, m_dtPlayoutStart(0.0f)
		, m_dtPlayoutEnd(0.0f)
	{}
	
	stu_ETEventMetaData(const stu_ETEventMetaData& event)
	{
		operator = (event);
	}

	stu_ETEventMetaData& operator = (const stu_ETEventMetaData& event)
	{
		m_lClipID			= event.m_lClipID;			// 素材ID
		m_strEventID		= event.m_strEventID;		// EVENTID
		m_strEventTitle		= event.m_strEventTitle;	// 标题
		m_strComments		= event.m_strComments;		// 备注
		m_lSOM				= event.m_lSOM;				// 入点
		m_lEOM				= event.m_lEOM;				// 出点
		m_lDuration			= event.m_lDuration;		// 长度
		m_lChannel			= event.m_lChannel;			// 通道
		m_lVersion			= event.m_lVersion;			// 版本
		m_strMOSID			= event.m_strMOSID;			// MOS项ID
		m_dtLastUpdate		= event.m_dtLastUpdate;		// 最后修改日期
		m_dtCreateDate		= event.m_dtCreateDate;		// 创建时间
		m_strUpdateUser		= event.m_strUpdateUser;	// 修改人
		m_strUpdateTerminal = event.m_strUpdateTerminal;// 修改终端
		m_dtPlayoutStart	= event.m_dtPlayoutStart;	// 播出开始时间
		m_dtPlayoutEnd		= event.m_dtPlayoutEnd;		// 播出结束时间
		m_strStoryTitle		= event.m_strStoryTitle;	// STORYTITLE
		m_strProgramTitle	= event.m_strProgramTitle;	// PROGRAMTITLE
		return *this;
	}

}ETEventMetaData, *LPETEventMetaData;

typedef struct tagETClipInfoEx : public ETClipInfo
{
	LPETPlanningMetaData	lpPlanning;
	LPETIngestMetaData		lpIngest;
	lpETOrgMetaData			lpOrgMetaData;
	LPETEventMetaData		lpEventMetaData;

	ET_RUNTIME_CLASS(ETClipInfo, ET_RT_CLASS_ETCLIPINFOEX)
	tagETClipInfoEx() 
		: ETClipInfo()
		, lpPlanning(NULL)
		, lpIngest(NULL)
		, lpOrgMetaData(NULL)
		, lpEventMetaData(NULL)
	{
	}

	tagETClipInfoEx(const tagETClipInfoEx& stClipInfoEx)
	{
		*this = stClipInfoEx;
	}

	virtual ~tagETClipInfoEx()
	{
		Reset();
		Release();
	}

	void Release()
	{
		if (lpPlanning != NULL)
		{
			delete lpPlanning;
			lpPlanning = NULL;
		}
		if (lpIngest != NULL)
		{
			delete lpIngest;
			lpIngest = NULL;
		}

		if (lpOrgMetaData != NULL)
		{
			delete lpOrgMetaData;
			lpOrgMetaData = NULL;
		}

		if (lpEventMetaData != NULL)
		{
			delete lpEventMetaData;
			lpEventMetaData = NULL;
		}
	}

	tagETClipInfoEx& operator = (const ETClipInfo& stClipInfo)
	{
		Release();
		ETClipInfo::operator = (stClipInfo);
		return *this;
	}

	tagETClipInfoEx& operator = (const tagETClipInfoEx& stClipInfoEx)
	{
		Release();

		ETClipInfo::operator = (stClipInfoEx);
		if (stClipInfoEx.lpPlanning != NULL)
		{
			lpPlanning = new ETPlanningMetaData;
			*lpPlanning = *stClipInfoEx.lpPlanning;
		}
		if (stClipInfoEx.lpIngest != NULL)
		{
			lpIngest = new ETIngestMetaData;
			*lpIngest = *stClipInfoEx.lpIngest;
		}

		if (stClipInfoEx.lpOrgMetaData != NULL)
		{
			lpOrgMetaData = new ETOrgMetaData;
			*lpOrgMetaData = *stClipInfoEx.lpOrgMetaData;
		}

		if (stClipInfoEx.lpEventMetaData != NULL)
		{
			lpEventMetaData = new ETEventMetaData;
			*lpEventMetaData = *stClipInfoEx.lpEventMetaData;
		}

		return *this;
	}
}ETClipInfoEx, *LPETClipInfoEx;
typedef CArray<LPETClipInfoEx, LPETClipInfoEx> ETArrayClipInfoEx;

//素材Trim入出点定义
typedef struct stu_ETClipTrimInfo
{
	long			m_lTrimIn;
	long			m_lTrimOut;

	stu_ETClipTrimInfo()
		: m_lTrimIn(-1), m_lTrimOut(-1)
	{
		
	}

	stu_ETClipTrimInfo& operator = (const stu_ETClipTrimInfo& src)
	{
		m_lTrimIn = src.m_lTrimIn; 
		m_lTrimOut = src.m_lTrimOut;

		return *this;
	}
}ETClipTrimInfo,*lpETClipTrimInfo;
typedef CArray<ETClipTrimInfo,ETClipTrimInfo &> ETArrayClipTrimInfo;

//! 数据库数据查询数据类型
enum em_ETDBQueryDataType
{
	ET_UnKnow = 0,
	ET_emNumber = 1,
	ET_emString = 2,
	ET_emDate = 3,
	ET_emCLOB = 4,
	//!自定义类型
	ET_emCustomType = 5,
	//不区分大小写
	ET_emAllString  = 6,
	//递归查询
	ET_emRecursive	= 7
};
/**
* @数据库数据查询条件结构体定义
*/
typedef struct stu_ETDBQueryCondition
{
	//!与前一条件的连接符；可以是"AND"、"OR"、"("、"AND("、"OR("等逻辑连接符
	CString m_strPrefix;
	//!查询所支持的字段名
	CString m_strKeyword;
	//!查询的条件值
	CString m_strValue;
	//!字段名与字段值之间的逻辑关系，可以是">"、">="、"<"、"<="、"="、"<>"、"LIKE"、"IN"、"NOT IN"等关系比较符
	CString m_strRelation;
	//!数据类型
	em_ETDBQueryDataType m_emDataType;
	//!本条件的结尾符；可以是")"符号
	CString m_strSuffix;
	//!是否包含回收站
	BOOL m_bIncludeTrash;

	stu_ETDBQueryCondition()
	{
		m_emDataType = ET_UnKnow;
		m_bIncludeTrash = FALSE;
	}
}ETDBQueryCondition,*lpETDBQueryCondition;

typedef CArray<ETDBQueryCondition*,ETDBQueryCondition*&> ETArrayQueryCondition;

class CETQueryConditionManager
{
public:
	CETQueryConditionManager()
	{
		Reset();
	}
	virtual ~CETQueryConditionManager()
	{
		Reset();
	}
	BOOL FromGUIDToString(const GUID & gGUID,CString & strGUID)
	{
		TCHAR lpBuffer[40];
		memset(lpBuffer,0,40*sizeof(TCHAR));
		StringFromGUID2(gGUID,lpBuffer,40*sizeof(40));
		strGUID = lpBuffer;
		strGUID.Replace(_T("{"),_T(""));
		strGUID.Replace(_T("-"),_T(""));
		strGUID.Replace(_T("}"),_T(""));
		strGUID.MakeLower();
		return TRUE;
	}
	/**
	* @param strPrefix 与前一条件的连接符；可以是"AND"、"OR"、"("、"AND("、"OR("等逻辑连接符
	* @param strKeyword 查询所支持的字段名
	* @param strValue 查询的条件值
	* @param strRelation 字段名与字段值之间的逻辑关系，可以是">"、">="、"<"、"<="、"="、"<>"、"LIKE"、"IN"、"NOT IN"等关系比较符
	* @param emDataType 数据类型
	* @param strSuffix 本条件的结尾符；可以是")"符号
	*/
	void AddItem(const CString & strPrefix,const CString & strKeyword, 
		const CString & strValue,const CString & strRelation, 
		em_ETDBQueryDataType emDataType,const CString &strSuffix)
	{
		lpETDBQueryCondition lp = new ETDBQueryCondition;
		if(NULL == lp)
			return;
		lp->m_emDataType = emDataType;
		lp->m_strKeyword = strKeyword;
		lp->m_strPrefix = strPrefix;
		lp->m_strRelation = strRelation;
		lp->m_strSuffix = strSuffix;
		lp->m_strValue = strValue;
		m_arCondition.Add(lp);
	}
	void AddItem(const CString &strPrefix,const CString & strKeyword, 
		DATE date,const CString & strRelation,const CString & strSuffix)
	{
		CString strDate;
		strDate.Format(TEXT("%f"),date);
		AddItem(strPrefix,strKeyword,strDate,strRelation,ET_emDate,strSuffix);
	}
	void AddItem(const CString & strPrefix,const CString & strKeyword,
		DWORD dwValue,const CString & strRelation,const CString & strSuffix)
	{
		CString strValue;
		strValue.Format(TEXT("%d"),dwValue);
		AddItem(strPrefix,strKeyword,strValue,strRelation,ET_emNumber,strSuffix);
	}
	void AddItem(const CString & strPrefix,const CString & strKeyword,
		long lValue,const CString & strRelation,const CString & strSuffix)
	{
		CString strValue;
		strValue.Format(TEXT("%d"),lValue);
		AddItem(strPrefix,strKeyword,strValue,strRelation,ET_emNumber,strSuffix);
	}
	void AddItem(const CString & strPrefix,const CString & strKeyword,
		ULONGLONG ullValue,const CString &strRelation,const CString & strSuffix)
	{
		CString strValue;
		strValue.Format(TEXT("%I64d"),ullValue);
		AddItem(strPrefix,strKeyword,strValue,strRelation,ET_emNumber,strSuffix);
	}
	void AddItem(const CString & strPrefix,const CString & strKeyword,
		const GUID guid,const CString &strRelation,const CString & strSuffix)
	{
		CString strValue;
		FromGUIDToString(guid,strValue);
		AddItem(strPrefix,strKeyword,strValue,strRelation,ET_emString,strSuffix);
	}

	void AddItem(lpETDBQueryCondition lpCondition)
	{
		if(NULL == lpCondition)
			return;
		m_arCondition.Add(lpCondition);
	}

	// 产生根据文件夹OBJECTID递归查询所有子对象的条件，add by djq 2007-7-26
	// strPrefix		与前一条件的连接符
	// strRelation		与递归条件的逻辑关系，只能是：IN或NOT IN
	// lValue			需要递归的文件夹的OBJECTID
	// strSuffix		该条件的结束符
	// bIncludeTrash	是否包含回收站的对象
	void AddRecursiveItemByFolderID(const CString & strPrefix,const CString &strRelation,long lValue,
		const CString & strSuffix, BOOL bIncludeTrash = FALSE)
	{
		CString strValue;
		strValue.Format(TEXT("%d"),lValue);

		lpETDBQueryCondition lp = new ETDBQueryCondition;
		if(NULL == lp)
			return;
		lp->m_emDataType = ET_emRecursive;
		lp->m_strKeyword = _T("OBJECTID");
		lp->m_strPrefix = strPrefix;
		lp->m_strRelation = strRelation;
		lp->m_strSuffix = strSuffix;
		lp->m_strValue = strValue;
		lp->m_bIncludeTrash = bIncludeTrash;
		m_arCondition.Add(lp);
	}
	// add by djq end

	//!清除所有项
	void Reset()
	{
		lpETDBQueryCondition lp = NULL;
		for(int i = 0;i<m_arCondition.GetSize();i++)
		{
			lp = m_arCondition.GetAt(i);
			delete lp;
			lp = NULL;
		}
		m_arCondition.RemoveAll();
	}

	ETArrayQueryCondition & GetArrayQueryCondition()
	{
		return m_arCondition;
	}
private:
	CETQueryConditionManager(const CETQueryConditionManager &);
	CETQueryConditionManager & operator = (const CETQueryConditionManager &);

	ETArrayQueryCondition m_arCondition;
};

/**
* @所取文件路径类型
*/
enum em_PathType
{
	ET_PathType_emHVideo	=	1,	//高码率视频
	ET_PathType_emHAudio	=	2,	//高码率音频
	ET_PathType_emLVideo	=	3,	//低码率视频
	ET_PathType_emLAudio	=	4,	//低码率音频
	ET_PathType_emPgm		=	5,	//节目路径
	ET_PathType_emEDL		=	6,	//EDL路径
	ET_PathType_emCD		=	7,	//CD路径
};

/************************************************************************/
/* 针对第三方媒资的回迁接口数据                                         */
/************************************************************************/
/*
typedef struct tag_ETRetrieveItem
{
	GUID m_guidMediaID;
	CString m_strMediaLabel;
	CString m_strObjID;
	CString m_strHRPath;
	long    m_lStartFrame;
	long    m_lTransferSize;
	CString m_strDestPath;
}ETRetrieveItem,*lpETRetrieveItem;

typedef CArray<lpETRetrieveItem, lpETRetrieveItem> ETArrayRetrieveItem;

typedef struct tag_ETRetrieveRequest
{
	GUID m_guidMsgID;
	DATE m_dtItemStamp;
	CString m_strOrderID;
	CString m_strUserToken;
	CString m_strTerminal;
	int     m_nPriority;
}ETRetrieveRequest, *lpETRetrieveRequest;
*/
typedef struct stu_ET3rdRetrieveItem
{
	CString strMediaID;
	CString strMediaLabel;
	CString strObjID;
	CString strHRPath;
	long    lStartFrame;
	long    lTransferSize;
	CString strDestinationPath;
	stu_ET3rdRetrieveItem()
	{
		Reset();
	}
	virtual ~stu_ET3rdRetrieveItem()
	{
		Reset();
	}

	stu_ET3rdRetrieveItem& operator=(const stu_ET3rdRetrieveItem& src)
	{
		strMediaID = src.strMediaID;
		strMediaLabel = strMediaLabel;
		strObjID = src.strObjID;
		strHRPath = src.strHRPath;
		strDestinationPath = src.strDestinationPath;
		lStartFrame = src.lStartFrame;
		lTransferSize = src.lTransferSize;

		return *this;
	}
	CString GetXmlContent()
	{
		CString strTemp;
		strTemp.Format(_T("<Retrieve>\n<MediaID>%s</MediaID>\n<MediaLabel>%s</MediaLabel>\n<objID>%s</objID>\n<HRPath>%s</HRPath>\n<StartFrame>%ld</StartFrame>\n<TransferSize>%ld</TransferSize>\n<DestinationPath>%s</DestinationPath>\n</Retrieve>"),
			strMediaID,
			strMediaLabel,
			strObjID,
			strHRPath,
			lStartFrame,
			lTransferSize,
			strDestinationPath
			);

		return strTemp;
	}

	void Reset()
	{
		strMediaID = strMediaLabel = strObjID = strHRPath = strDestinationPath = _T("");
		lStartFrame = lTransferSize = 0L;
	}

}ET3rdRetrieveItem,  *lpET3rdRetrieveItem;

typedef CArray<lpET3rdRetrieveItem, lpET3rdRetrieveItem> ETArray3rdRetrieveItem;

typedef struct stu_ET3rdRetrievalRequest
{
	CString strMessageID;
	DATE	dtTimeStamp;
	CString strOrderID;
	CString strUser;
	CString strTermainal;
	long    lPriority;
	ETArray3rdRetrieveItem arRetrieveItem;

	stu_ET3rdRetrievalRequest()
	{
		Reset();
	}
	virtual ~stu_ET3rdRetrievalRequest()
	{
		Reset();

	}

	stu_ET3rdRetrievalRequest& operator=(const stu_ET3rdRetrievalRequest& src)
	{
		Reset();
		strMessageID = src.strMessageID;
		strOrderID   = src.strOrderID;
		strUser		 = src.strUser;
		strTermainal = src.strTermainal;
		dtTimeStamp  = src.dtTimeStamp;
		lPriority    = src.lPriority;

		for(int i=0;i<src.arRetrieveItem.GetCount();i++)
		{
			lpET3rdRetrieveItem pItem = new ET3rdRetrieveItem;
			*pItem = *src.arRetrieveItem[i];
			arRetrieveItem.Add(pItem);			
		}

		return *this;

	}

	CString GetXmlContent()
	{
		CString strMsg;
		COleDateTime dt(dtTimeStamp);

		strMsg.Format(_T("<RetrievalRequest>\n<messageID>%s</messageID>\n<timeStamp>%s</timeStamp>\n<orderID>%s</orderID>\n<User>%s</User>\n<Terminal>%s</Terminal>\n<Priority>%ld</Priority>\n"),
			strMessageID, 
			dt.Format(_T("%y-%m-%dT%H:%M:%S")),
			strOrderID,
			strUser,
			strTermainal,
			lPriority);

		if(arRetrieveItem.GetCount()>0)
		{
			for(int i=0;i<arRetrieveItem.GetCount() && i<256;i++)
			{
				strMsg += arRetrieveItem[i]->GetXmlContent();
			}
		}

		strMsg += _T("\nRetrievalRequest");

	}

	void Reset()
	{
		strMessageID = strOrderID = strUser = strTermainal = _T("");
		dtTimeStamp  = 0L;
		lPriority    = 0;

		for(int i=0;i<arRetrieveItem.GetCount();i++)
		{
			delete arRetrieveItem[i];
		}
		arRetrieveItem.RemoveAll();

	}
}ET3rdRetrievalRequest, *lpET3rdRetrievalRequest;

//历史任务
enum emHistoryTaskType
{
	ET_TaskType_Unknow,
	ET_TaskType_Archive,
	ET_TaskType_Retrieve,
	ET_TaskType_Download,
};

typedef struct stu_ETHistoryTask
{
	GUID				m_guidTask;
	long				m_lPgmID;
	long				m_lSerialNo;
	GUID				m_guidClip;
	long				m_lStatus;
	long				m_lTrimIn;
	long				m_lTrimOut;
	CString				m_strDes;
	long				m_lEntityID;
	DATE				m_dtCreateDate;
	CString				m_str3rdGUID;
	long				m_lSiteID;		//-1标示是第三方媒资
	emHistoryTaskType	m_lTaskType;
	stu_ETHistoryTask()
	{
		m_lSiteID = 0;
		m_lTaskType = ET_TaskType_Unknow;
	}
}ETHistoryTask, *lpETHistoryTask;
typedef CArray<lpETHistoryTask,lpETHistoryTask> ETArrayHistoryTask;
//

typedef enum
{
	ET_CMGW_TASK_TYPE_FROM_IMPORT,
	ET_CMGW_TASK_TYPE_TO_EXPORT,
	ET_CMGW_TASK_TYPE_TO_ARCHIVE,
	ET_CMGW_TASK_TYPE_TO_RETRIEVAL_REQUEST,
	ET_CMGW_TASK_TUPE_FROM_ARCHIVE,				//第三方媒资的素材回迁
	ET_CMGW_TASK_TUPE_FROM_NLE,	
	ET_CMGW_TASK_TYPE_UNKNOW,
	ET_CMGW_TASK_TUPE_FROM_INTERFACE,           //通过openinterface的导入
}ET_CmGWTaskType;

//元数据映射
typedef enum
{
	ET_MAP_FIELD_PLANNING,
}ET_MAP_FieldOfMdType;

typedef enum
{
	ET_MAP_CONVER_NORMAL,			//不用转换
	ET_MAP_CONVER_STRING2INT,
	ET_MAP_CONVER_INT2STRING,
	ET_MAP_CONVER_STRING2DATE,
}ET_MAP_ConversionType;

typedef enum
{
	ET_MAP_SYS_XDCAM,
	ET_MAP_SYS_NRCS,
}ET_MAP_OtherSysType;

typedef struct stu_ETMetadataInfo
{
	long					m_lMetadataID;
	ET_MAP_FieldOfMdType	m_emFieldType;
	CString					m_strMetadataName;
	CString					m_strMetadataEsc;	//元数据描述
	stu_ETMetadataInfo()
	{
		m_emFieldType = ET_MAP_FIELD_PLANNING;
	}
}ETMetadataInfo, *lpETMetadataInfo;
typedef CArray<lpETMetadataInfo,lpETMetadataInfo> ETArrayMetadataInfo;

typedef struct stu_ETMetadataMapping
{
	ET_MAP_OtherSysType			m_emOtherSysType;
	lpETMetadataInfo			m_pMetadataInfo;
	CString						m_strMapOtherMdName;
	ET_MAP_ConversionType		m_emConversionType;
	stu_ETMetadataMapping()
	{
		m_emConversionType = ET_MAP_CONVER_NORMAL;
	}
}ETMetadataMapping, *lpETMetadataMapping;
typedef CArray<lpETMetadataMapping,lpETMetadataMapping> ETArrayMetadataMapping;

typedef struct stu_ETMapOtherSysInfo
{
	ET_MAP_OtherSysType			m_emOtherSysType;
	long						m_lSonapsMdID;
	CString						m_strMapOtherMdName;
	ET_MAP_ConversionType		m_emConversionType;
}ETMapOtherSysInfo, *lpETMapOtherSysInfo;
typedef CArray<lpETMapOtherSysInfo,lpETMapOtherSysInfo> ETArrayMapOtherSysInfo;
//
typedef struct stu_ETUserLayoutData
{
	long    m_lUserID;
	int     m_ActiveType;
	CString m_strActiveInfo;
	CString m_strUserData;
	stu_ETUserLayoutData()
	{
		m_lUserID = 0;
		m_ActiveType = 0;
		m_strActiveInfo = _T("");
		m_strUserData = _T("");
	}

	stu_ETUserLayoutData(const stu_ETUserLayoutData &src)
	{
		m_lUserID			= src.m_lUserID;
		m_ActiveType		= src.m_ActiveType;		 
		m_strActiveInfo		= src.m_strActiveInfo;
		m_strUserData		= src.m_strUserData;

	}

	const stu_ETUserLayoutData& operator = (const stu_ETUserLayoutData &src)
	{
		m_lUserID			= src.m_lUserID;
		m_ActiveType		= src.m_ActiveType;		 
		m_strActiveInfo		= src.m_strActiveInfo;
		m_strUserData		= src.m_strUserData;
		return *this;
	}
}ETUserLayoutData, *lpETUserLayoutData;
typedef CArray<lpETUserLayoutData, lpETUserLayoutData> ETArrayUserLayoutData;

typedef struct stu_ETUserLayoutTemplate
{
	long m_lTemplateID;
	long m_lUserID;
	CString m_strTemplateName;
	DATE m_dtCreateTime;
	CString m_strTemplateInfo;
	stu_ETUserLayoutTemplate()
	{
		m_lTemplateID = 0;
		m_lUserID =0;
		m_strTemplateName = _T("");
		m_dtCreateTime = 0.0;
		m_strTemplateInfo = _T("");
	}
	stu_ETUserLayoutTemplate(const stu_ETUserLayoutTemplate &src)
	{
		m_lTemplateID		= src.m_lTemplateID;
		m_lUserID		    = src.m_lUserID;		 
		m_strTemplateName	= src.m_strTemplateName;
		m_dtCreateTime		= src.m_dtCreateTime;
		m_strTemplateInfo	= src.m_strTemplateInfo;
	}

	const stu_ETUserLayoutTemplate& operator = (const stu_ETUserLayoutTemplate &src)
	{
		m_lTemplateID		= src.m_lTemplateID;
		m_lUserID		    = src.m_lUserID;		 
		m_strTemplateName	= src.m_strTemplateName;
		m_dtCreateTime		= src.m_dtCreateTime;
		m_strTemplateInfo	= src.m_strTemplateInfo;
		return *this;
	}

}ETUserLayoutTemplate, *lpETUserLayoutTemplate;
typedef CArray<lpETUserLayoutTemplate, lpETUserLayoutTemplate> ETArrayUserLayoutTemplate;

//2010/4/22 niejiagang:Add 素材组子素材与组素材的关系
typedef struct stu_ETGroupItem
{
	GUID	guidRes;			//子素材GUID
	GUID	guidResGroup;		//组素材GUID
	int     iQualityIndex;      //质量索引
	INT64	lClipIn;			//子素材在组素材中的起始位置 
	INT64	lFileIn;			//使用的子素材中的开始位置 
	INT64	lFileOut;			//使用的子素材中的结束位置
	INT64   lFileLen;           //使用的子素材中的长度 
	BOOL    bAVInterleave;      //视音频同一文件
	long	lVideoFormatID;		//子素材视频格式
	long	lAudioFormatID;		//子素材音频格式
	long    lKeyFormatID;       //子素材Key格式
	INT64	lClipClass;			//子素材所在轨道，如Video, A1, A2...
	long    lReserve1;          //保留字段
	long    lReserve2;          //保留字段
	long    lReserve3;          //保留字段

	stu_ETGroupItem()
	{
		Reset();
	}
	const stu_ETGroupItem& operator = (const stu_ETGroupItem &src)
	{
		guidRes         = src.guidRes;
		guidResGroup    = src.guidResGroup;
		iQualityIndex   = src.iQualityIndex;
		lClipIn         = src.lClipIn;
		lFileIn         = src.lFileIn;
		lFileOut        = src.lFileOut;
		lFileLen        = src.lFileLen;
		bAVInterleave   = src.bAVInterleave;
		lVideoFormatID  = src.lVideoFormatID;
		lAudioFormatID  = src.lAudioFormatID;
		lKeyFormatID    = src.lKeyFormatID;
		lClipClass      = src.lClipClass;
		lReserve1       = src.lReserve1;
		lReserve2       = src.lReserve2;
		lReserve3       = src.lReserve3;
		return *this;

	}
	void Reset()
	{
		guidRes         = GUID_NULL;
		guidResGroup    = GUID_NULL;
		iQualityIndex   = 0;
		lClipIn         = 0;
		lFileIn         = 0;
		lFileOut        = 0;
		lFileLen        = 0;
		bAVInterleave   = FALSE;
		lVideoFormatID  = 0;
		lAudioFormatID  = 0;
		lKeyFormatID    = 0;
		lClipClass      = 0;
		lReserve1       = 0;
		lReserve2       = 0;
		lReserve3       = 0;

	}
	virtual ~stu_ETGroupItem()
	{
		Reset();
	}
}ETGroupItem,*lpETGroupItem;
typedef CArray<lpETGroupItem,lpETGroupItem> ETArrayGroupItem;

#pragma pack(push, 4)

typedef struct cannot_delete_reason
{
	ETObject *obj;			// current object data
	unsigned long reason;	// reason
	CString extra;			// extra xml text
} CANNOT_DELETE_REASON;

typedef void (*DELETE_DETAIL_CALLBACK)(const CANNOT_DELETE_REASON*, void*);

typedef struct delete_detail_param
{
	BOOL silent;					// show message box?
	BOOL confirmed;					// default confirmed?
	DELETE_DETAIL_CALLBACK proc;	// callback function to handle delete reason
	void *userdata;					// user data for the callback function
} DELETE_DETAIL_PARAM;

#pragma pack(pop)

// set highest bit to 1 means the clip is not allowed to be removed
#define NODEL_NOT_ALLOWED			0x80000000

#define NODEL_REASON_SYS_FOLDER		0x00000001
#define NODEL_REASON_SRV_INUSE		0x00000002
#define NODEL_REASON_OBJ_LOCKED		0x00000004
#define NODEL_REASON_OBJ_PLAYOUT	0x00000008
#define NODEL_REASON_CHILD_LOCKED	0x00000010
#define NODEL_REASON_CHILD_PLAYOUT	0x00000020
#define NODEL_REASON_CHILD_PGM		0x00000040
#define NODEL_REASON_OBJ_PGM		0x00000080
#define NODEL_REASON_CHILD_LIVING	0x00000100
#define NODEL_REASON_ARC_DOING		0x00000200
#define NODEL_REASON_ARC_FAILED		0x00000400
#define NODEL_REASON_RET_DOING		0x00000800
#define NODEL_REASON_OBJ_CAPTURE	0x00001000
#define NODEL_REASON_NO_RIGHT		0x00002000
#define NODEL_REASON_ARC_ARCHIVING  0x00004000
#define NODEL_REASON_ARC_RETRIEVING 0x00008000
#define NODEL_REASON_ARC_NO_STATE   0x00010000
#define NODEL_REASON_ARC_XDCAM      0x00020000

//DEB音频轨道
#define ET_CLIP_DBE_A1  0x00000001
#define ET_CLIP_DBE_A2  0x00000002
#define ET_CLIP_DBE_A3  0x00000004
#define ET_CLIP_DBE_A4  0x00000008
#define ET_CLIP_DBE_A5  0x00000010
#define ET_CLIP_DBE_A6  0x00000020
#define ET_CLIP_DBE_A7  0x00000040
#define ET_CLIP_DBE_A8  0x00000080

//对象入库时的特殊要求
typedef		ULONGLONG ET_SAVEOBJECT_SPECIAL_TYPE;	
#define ET_SAVEOBJECT_SPECIAL_NOTNOTIFYSERVER		0x00000001	//不通知MLS（即其他终端不会自动刷新）
#define ET_SAVEOBJECT_SPECIAL_NOTCHECKSTANDARD		0x00000002	//不自动根据文件列表来重新设置素材的制式

typedef enum
{
	ET_CMGW_FileTilte_GUID         = 1,	//GUID
	ET_CMGW_FileTilte_ClipName     = 2,	//clip name
	ET_CMGW_FileTilte_MaterialID   = 3,	//Material ID
}ET_CmFileTitle;

//CommonGW任务类型
enum emTaskStatus
{
	ET_TaskStatus_Unknown	   = 0,
	ET_TaskStatus_Sending      = 1,
	ET_TaskStatus_Doing        = 2,
	ET_TaskStatus_FailtoSend   = 3,
	ET_TaskStatus_Failed       = 4,
	ET_TaskStatus_NotSupport   = 5,//导入才会有此状态
	ET_TaskStatus_Deleted      = 6,
	ET_TaskStatus_Success      = 7,
	ET_TaskStatus_Lauch_Task   = 8,//导出才会有此特殊状态
	ET_TaskStatus_Export_Task  = 9,//导出才有此状态
	ET_TaskStatus_DeletedClip  = 10,//导出才有此状态
};

//2010/06/07 zxp:Add GTMAgent结构
enum eu_SubSystem
{
	SYSTEM_UNKNOWN = 0,
	SYSTEM_MPC = 1,
	SYSTEM_ML,
	SYSTEM_PL,
	SYSTEM_COMMONGW,
	SYSTEM_XPRINS,
};
enum eu_CommondType
{
	COMMONDTYPE_UNKNOWN = 0,
	COMMONDTYPE_Create,
	COMMONDTYPE_Update,
	COMMONDTYPE_Start,
	COMMONDTYPE_Stop,
	COMMONDTYPE_Pause,
	COMMONDTYPE_Delete,
	COMMONDTYPE_Priority,
};
enum eu_OperationStatus
{
	OPERATIONSTATUS_UNKNOWN = 0,
	OPERATIONSTATUS_Start,
	OPERATIONSTATUS_Accept,
	OPERATIONSTATUS_Update,
	OPERATIONSTATUS_Complete,
};

enum eu_ParamNodeType
{
	PARAMNODETYPE_UNKNOWN = 0,
	PARAMNODETYPE_WORKFLOWS,
	PARAMNODETYPE_MODULETASKS,
	PARAMNODETYPE_TASKSTEPS,
};

typedef struct stu_ETGTMModuleTasks
{
	CString m_strTaskID;
	CString m_strWorkFlowID;
	CString m_strWorkFlowType;
	CString m_strWorkFlowSubType;
	CString m_strWorkFlowName;
	CString m_strModuleName;
	CString m_strTaskName;
	CString m_strMaterialID;
	CString m_strCreator;
	CString m_strCreateTime;
	CString m_strStartTime;
	CString m_strEndTime;
	CString m_strIsSchedulTask;
	CString m_strScheduleStartTime;
	CString m_strWorkFlowDetails;
	CString m_strDetails;
	CString m_strPriority;
	CString m_strStatus;
	
	stu_ETGTMModuleTasks()
	{
		Reset();
	}
	const stu_ETGTMModuleTasks& operator = (const stu_ETGTMModuleTasks &src)
	{
		m_strTaskID			   = src.m_strTaskID;
		m_strWorkFlowID		   = src.m_strWorkFlowID;
		m_strWorkFlowType      = src.m_strWorkFlowType;
		m_strWorkFlowSubType   = src.m_strWorkFlowSubType;
		m_strWorkFlowName      = src.m_strWorkFlowName;
		m_strModuleName		   = src.m_strModuleName;
		m_strTaskName          = src.m_strTaskName;
		m_strMaterialID		   = src.m_strMaterialID;
		m_strCreator		   = src.m_strCreator;
		m_strCreateTime		   = src.m_strCreateTime;
		m_strStartTime		   = src.m_strStartTime;
		m_strEndTime		   = src.m_strEndTime;
		m_strIsSchedulTask	   = src.m_strIsSchedulTask;
		m_strScheduleStartTime = src.m_strScheduleStartTime;
		m_strWorkFlowDetails   = src.m_strWorkFlowDetails;
		m_strDetails           = src.m_strDetails;
		m_strPriority          = src.m_strPriority;
		m_strStatus            = src.m_strStatus;
		return *this;
	}
	void Reset()
	{
		m_strTaskID			   = _T("");
		m_strWorkFlowID		   = _T("");
		m_strWorkFlowType      = _T("");
		m_strWorkFlowSubType   = _T("");
		m_strWorkFlowName      = _T("");
		m_strModuleName		   = _T("");
		m_strTaskName          = _T("");
		m_strMaterialID		   = _T("");
		m_strCreator		   = _T("");
		m_strCreateTime		   = _T("");
		m_strStartTime		   = _T("");
		m_strEndTime		   = _T("");
		m_strIsSchedulTask	   = _T("");
		m_strScheduleStartTime = _T("");
		m_strWorkFlowDetails   = _T("");
		m_strDetails           = _T("");
		m_strPriority          = _T("");
		m_strStatus            = _T("");
	}
	virtual ~stu_ETGTMModuleTasks()
	{
		Reset();
	}

}ETGTMModuleTasks,*lpETGTMModuleTasks;

typedef CArray<lpETGTMModuleTasks,lpETGTMModuleTasks> ETArrayGTMModuleTasks;

typedef struct stu_ETGTMTaskSteps
{
	CString m_strStepID;
	CString m_strTaskID;
	CString m_strStepName;
	CString m_strDependentStepID;
	CString m_strStartTime;
	CString m_strEndTime;
	CString m_strPriority;
	CString m_strStatus;
	CString m_strServerName;
	CString m_strServerIP;
	CString m_strProgress;
	CString m_strErrorMessage;
	CString m_strStepGuid;
	CString m_strComponent;

	stu_ETGTMTaskSteps()
	{
		Reset();
	}
	const stu_ETGTMTaskSteps& operator = (const stu_ETGTMTaskSteps &src)
	{
		m_strStepID			 = src.m_strStepID;
		m_strTaskID			 = src.m_strTaskID;
		m_strStepName		 = src.m_strStepName;
		m_strDependentStepID = src.m_strDependentStepID;
		m_strStartTime		 = src.m_strStartTime;
		m_strEndTime		 = src.m_strEndTime;
		m_strPriority		 = src.m_strPriority;
		m_strStatus		     = src.m_strStatus;
		m_strServerName		 = src.m_strServerName;
		m_strServerIP		 = src.m_strServerIP;
		m_strProgress	     = src.m_strProgress;
		m_strErrorMessage    = src.m_strErrorMessage;
		m_strStepGuid        = src.m_strStepGuid;
		m_strComponent       = src.m_strComponent;
		return *this;
	}
	void Reset()
	{
		m_strStepID			 = _T("");
		m_strTaskID			 = _T("");
		m_strStepName		 = _T("");
		m_strDependentStepID = _T("");
		m_strStartTime		 = _T("");
		m_strEndTime		 = _T("");
		m_strPriority		 = _T("");
		m_strStatus		     = _T("");
		m_strServerName		 = _T("");
		m_strServerIP		 = _T("");
		m_strProgress		 = _T("");
		m_strErrorMessage    = _T("");
		m_strStepGuid        = _T("");
		m_strComponent       = _T("");
	}
	virtual ~stu_ETGTMTaskSteps()
	{
		Reset();
	}

}ETGTMTaskSteps,*lpETGTMTaskSteps;

typedef CArray<lpETGTMTaskSteps,lpETGTMTaskSteps> ETArrayGTMTaskSteps;

typedef struct stu_ETGTMWorkFlows
{
	CString m_strWorkFlowID;
	CString m_strFlowType;
	CString m_strSubType;
	CString m_strFlowName;
	CString m_strMaterialID;
	CString m_strCreator;
	CString m_strCreateTime;
	CString m_strStartTime;
	CString m_strEndTime;
	CString m_strIsSchedulTask;
	CString m_strScheduleTime;
	CString m_strDetails;
	CString m_strPriority;
	CString m_strStatus;

	stu_ETGTMWorkFlows()
	{
		Reset();
	}
	const stu_ETGTMWorkFlows& operator = (const stu_ETGTMWorkFlows &src)
	{
		m_strWorkFlowID		   = src.m_strWorkFlowID;
		m_strFlowType          = src.m_strFlowType;
		m_strSubType           = src.m_strSubType;
		m_strFlowName	       = src.m_strFlowName;
		m_strMaterialID		   = src.m_strMaterialID;
		m_strCreator		   = src.m_strCreator;
		m_strCreateTime		   = src.m_strCreateTime;
		m_strStartTime		   = src.m_strStartTime;
		m_strEndTime		   = src.m_strEndTime;
		m_strIsSchedulTask	   = src.m_strIsSchedulTask;
		m_strScheduleTime      = src.m_strScheduleTime;
		m_strDetails           = src.m_strDetails;
		m_strPriority          = src.m_strPriority;
		m_strStatus            = src.m_strStatus;
		return *this;
	}
	void Reset()
	{
		m_strWorkFlowID		   = _T("");
		m_strFlowType          = _T("");
		m_strSubType           = _T("");
		m_strFlowName          = _T("");
		m_strMaterialID		   = _T("");
		m_strCreator		   = _T("");
		m_strCreateTime		   = _T("");
		m_strStartTime		   = _T("");
		m_strEndTime		   = _T("");
		m_strIsSchedulTask	   = _T("");
		m_strScheduleTime      = _T("");
		m_strDetails           = _T("");
		m_strPriority          = _T("");
		m_strStatus            = _T("");
	}
	virtual ~stu_ETGTMWorkFlows()
	{
		Reset();
	}

}ETGTMWorkFlows,*lpETGTMWorkFlows;

typedef CArray<lpETGTMWorkFlows,lpETGTMWorkFlows> ETArrayGTMWorkFlows;

typedef struct stu_ETGTMAgentData
{
	CString                m_strOperationID;      // 操作的ID
	eu_OperationStatus     m_euOperationStatus;   // 子系统向GTM的回报所处的阶段
	CString                m_strSourceIPAddr;     // 发送端IP地址
	eu_SubSystem           m_euSubSystem;         // 子系统的标识
	CString                m_strConnection;       // Agent告知子系统和GTM的网络连接状态
	eu_CommondType         m_euCommandType;       // GTM向子系统发送发送的动作指令
	eu_ParamNodeType       m_euParaNodeType;      // 有效参数的结点名, ModuleTasks和TaskSteps二选一
	ETArrayGTMModuleTasks  m_arModuleTasks;       // ModuleTasks结点
	ETArrayGTMTaskSteps    m_arTaskSteps;         // TaskSteps结点
	ETArrayGTMWorkFlows    m_arWorkFlows;         // WorkFlows结点
	CString                m_strResult;           // 动作执行结果
	CString                m_strInfo;             // 动作执行的信息，如失败原因
	ULONGLONG			   m_ullDataMask;	      //数据有效位定义

	stu_ETGTMAgentData()
	{
		Reset();
	}
	stu_ETGTMAgentData(const stu_ETGTMAgentData& src)
	{
		operator = (src);
	}

	void CopyByMask(const stu_ETGTMAgentData &src)
	{
		ULONGLONG ullDataMask = src.m_ullDataMask;

		if (FIELD_ISSET(ullDataMask, GTM_FIELD_OperationID))
			m_strOperationID = src.m_strOperationID;
		if (FIELD_ISSET(ullDataMask, GTM_FIELD_OperationStatus))
			m_euOperationStatus = src.m_euOperationStatus;
		if (FIELD_ISSET(ullDataMask, GTM_FIELD_SourceIPAddr))
			m_strSourceIPAddr = src.m_strSourceIPAddr;
		if (FIELD_ISSET(ullDataMask, GTM_FIELD_SubSystem))
			m_euSubSystem = src.m_euSubSystem;
		if (FIELD_ISSET(ullDataMask, GTM_FIELD_Connection))
			m_strConnection = src.m_strConnection;
		if (FIELD_ISSET(ullDataMask, GTM_FIELD_CommandType))
			m_euCommandType = src.m_euCommandType;
		if (FIELD_ISSET(ullDataMask, GTM_FIELD_ParaNodeType))
			m_euParaNodeType = src.m_euParaNodeType;
		if (FIELD_ISSET(ullDataMask, GTM_FIELD_Result))
			m_strResult = src.m_strResult;
		if (FIELD_ISSET(ullDataMask, GTM_FIELD_Info))
			m_strInfo = src.m_strInfo;
	}

	const stu_ETGTMAgentData& operator = (const stu_ETGTMAgentData &src)
	{
		m_strOperationID	   = src.m_strOperationID;
		m_euOperationStatus	   = src.m_euOperationStatus;
		m_strSourceIPAddr	   = src.m_strSourceIPAddr;
		m_euSubSystem          = src.m_euSubSystem;
		m_strConnection		   = src.m_strConnection;
		m_euCommandType		   = src.m_euCommandType;
		m_euParaNodeType       = src.m_euParaNodeType;
		m_strResult            = src.m_strResult;
		m_strInfo              = src.m_strInfo;
		m_ullDataMask          = src.m_ullDataMask;

		int i,nCount;
		lpETGTMModuleTasks lpModule,lpNewModule;
		nCount = (int)m_arModuleTasks.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpModule = m_arModuleTasks.GetAt(i);
			delete lpModule;
		}
		m_arModuleTasks.RemoveAll();

		nCount = (int)src.m_arModuleTasks.GetCount();
		for(i = 0;i < nCount;i++)
		{
			lpModule = src.m_arModuleTasks.GetAt(i);
			lpNewModule = new ETGTMModuleTasks;
			*lpNewModule = *lpModule;
			m_arModuleTasks.Add(lpNewModule);
		}

		lpETGTMTaskSteps lpTask,lpNewTask;
		nCount = (int)m_arTaskSteps.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpTask = m_arTaskSteps.GetAt(i);
			delete lpTask;
		}
		m_arTaskSteps.RemoveAll();

		nCount = (int)src.m_arTaskSteps.GetCount();
		for(i = 0;i < nCount;i++)
		{
			lpTask = src.m_arTaskSteps.GetAt(i);
			lpNewTask = new ETGTMTaskSteps;
			*lpNewTask = *lpTask;
			m_arTaskSteps.Add(lpNewTask);
		}
		
		lpETGTMWorkFlows lpWorkFlow,lpNewWorkFlow;
		nCount = (int)m_arWorkFlows.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpWorkFlow = m_arWorkFlows.GetAt(i);
			delete lpWorkFlow;
		}
		m_arWorkFlows.RemoveAll();

		nCount = (int)src.m_arWorkFlows.GetCount();
		for(i = 0;i < nCount;i++)
		{
			lpWorkFlow = src.m_arWorkFlows.GetAt(i);
			lpNewWorkFlow = new ETGTMWorkFlows;
			*lpNewWorkFlow = *lpWorkFlow;
			m_arWorkFlows.Add(lpNewWorkFlow);
		}

		return *this;
	}
	void Reset()
	{
		m_strOperationID	   = _T("");
		m_euOperationStatus	   = OPERATIONSTATUS_UNKNOWN;
		m_strSourceIPAddr	   = _T("");
		m_euSubSystem          = SYSTEM_UNKNOWN;
		m_strConnection		   = _T("");
		m_euCommandType		   = COMMONDTYPE_UNKNOWN;
		m_euParaNodeType       = PARAMNODETYPE_UNKNOWN;
		m_strResult            = _T("");
		m_strInfo              = _T("");
		m_ullDataMask		   = 0XFFFFFFFFFFFFFFFF;

		int i,nCount;
		lpETGTMModuleTasks lpModule;
		nCount = (int)m_arModuleTasks.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpModule = m_arModuleTasks.GetAt(i);
			delete lpModule;
		}
		m_arModuleTasks.RemoveAll();

		lpETGTMTaskSteps lpTask;
		nCount = (int)m_arTaskSteps.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpTask = m_arTaskSteps.GetAt(i);
			delete lpTask;
		}
		m_arTaskSteps.RemoveAll();

		lpETGTMWorkFlows lpWorkFlow;
		nCount = (int)m_arWorkFlows.GetCount();
		for(i = 0;i<nCount;i++)
		{
			lpWorkFlow = m_arWorkFlows.GetAt(i);
			delete lpWorkFlow;
		}
		m_arWorkFlows.RemoveAll();
	}
	virtual ~stu_ETGTMAgentData()
	{
		Reset();
	}

}ETGTMAgentData,*lpETGTMAgentData;

//播出提供给NLE的接口需要的
typedef  std::pair<int/*event id*/, CString/*description*/> ITEM;
typedef  std::vector<ITEM >  ITEMS;
//}}

//播出提供的xml exchange接口需要
typedef struct _tagTEMPLATEINFO
{
	long lTemplateID;
	CString strTemplateName;
}TEMPLATEINFO;

typedef std::vector<TEMPLATEINFO> TEMPLATEINFOS;

