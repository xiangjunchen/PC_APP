#pragma once
#include "afxtempl.h"
#include <vector>
using namespace std;

typedef LRESULT (*LPET_CallBackForML)(UINT uMsg,WPARAM wParam,LPARAM lParam);

//Ȩ�޶���
#define POPEDOM_ADMINISTRATOR		_T("ETAdministrator")

//{{2008/12/10 zxp:modify �޸Ĳ�������Ӧ�궨�壬ʵ��PE��ML��Ȩ�޷���
#define POPEDOM_IMPORT_MAT		   _T("ETImportMaterial")	// �����ز�	    �Ƿ���������زģ����Material List��
#define POPEDOM_COPY_ITEM          _T("ETCopyItem")         // ����Item     �Ƿ���Կ���item�����Material List��
#define POPEDOM_CREATE_FOLD		   _T("ETNewFolder")		// �½��ļ���	�Ƿ�����½��ļ��У������Material List��
#define POPEDOM_CREATE_PGM	       _T("ETNewSequence")		// �½���Ŀ	    �Ƿ�����½���Ŀ�����Material List��
#define POPEDOM_MODIFY_FOLD		   _T("ETModifyFolder")		// �޸��ļ���	�Ƿ�����޸��ļ��У������Material List��
#define POPEDOM_MODIFY_MAT		   _T("ETModifyMaterial")	// �޸��ز�	    �Ƿ�����޸��زģ����Material List��
#define POPEDOM_MODIFY_PGM         _T("ETModifySequence")   // �޸Ľ�Ŀ     �Ƿ���Դ򿪽�Ŀ�����޸ģ����Material List��
#define POPEDOM_DELETE_MAT		   _T("ETDeleteMaterial")	// ɾ���ز�	    �Ƿ����ɾ���زģ����Material List��
#define POPEDOM_DELETE_PGM		   _T("ETDeleteSequence")	// ɾ����Ŀ	    �Ƿ����ɾ����Ŀ�����Material List��
#define POPEDOM_DELETE_FOLD		   _T("ETDeleteFolder")		// ɾ���ļ���	�Ƿ����ɾ�����ļ��У������Material List��
#define POPEDOM_DELETE_FOLDTREE	   _T("ETDeleteFolderTree")	// ɾ���ļ�����	�Ƿ����ɾ���ļ������������Material List��
#define POPEDOM_PURGE_MAT          _T("ETPurgefromTranCan") // ����ز�     �Ƿ��������زģ����Material List��

#define POPEDOM_IMPORT_MAT_PE   _T("ETImportMaterialForPE") // �����ز�     �Ƿ���������زģ����Project Explorer��
#define POPEDOM_INGEST_MAT      _T("ETIngestMaterial")      // ��¼�ز�     �Ƿ������¼�زģ����Project Explorer��
#define POPEDOM_COPY_ITEM_PE    _T("ETCopyItemForPE")       // ����Item     �Ƿ���Կ���item�����Project Explorer��
#define POPEDOM_CREATE_TEMPL	_T("ETNewTemplate")			// �½�ģ��	    �Ƿ�����½�ģ�棨���Project Explorer��
#define POPEDOM_CREATE_GROUP	_T("ETNewProjectGroup")		// �½���Ŀ��	�Ƿ�����½���Ŀ�飨���Project Explorer��
#define POPEDOM_CREATE_PROJ		_T("ETNewProject")			// �½���Ŀ	    �Ƿ�����½���Ŀ���Լ���Ŀ�µ�Ŀ¼�����޸ģ����Project Explorer��
#define POPEDOM_ADDNEW_BIN      _T("ETAddNewBin")           // ���BIN�ļ�  �Ƿ�������BIN�ļ������Project Explorer��
#define POPEDOM_ADDNEW_MAT      _T("ETAddNewMaterial")      // ����ز�     �Ƿ��������زģ����Project Explorer��
#define POPEDOM_BUILD_PGM       _T("ETBuildNewSequence")    // �½���Ŀ     �Ƿ�����½���Ŀ�����Project Explorer��
#define POPEDOM_MODIFY_TEMPL	_T("ETModifyTemplate")		// �޸�ģ��	    �Ƿ�����޸�ģ�棨���Project Explorer��
#define POPEDOM_MODIFY_GROUP	_T("ETModifyProjectGroup")	// �޸���Ŀ��	�Ƿ�����޸���Ŀ�����ݣ������Project Explore��
#define POPEDOM_MODIFY_PROJ		_T("ETModifyProject")		// �޸���Ŀ	    �Ƿ�����޸���Ŀ���ݣ��Լ���Ŀ�µ�Ŀ¼�����޸ģ������Project Explore��
#define POPEDOM_MODIFY_BIN      _T("ETModifyBin")           // �޸�BIN�ļ�  �Ƿ�����޸�BIN�ļ������Project Explorer��
#define POPEDOM_MODIFY_MAT_PE   _T("ETModifyMaterialForPE") // �޸��ز�     �Ƿ�����޸��زģ����Project Explorer��
#define POPEDOM_MODIFY_PGM_PE	_T("ETModifySequenceForPE")	// �޸Ľ�Ŀ	    �Ƿ���Դ򿪽�Ŀ�����޸ģ����Project Explorer��
#define POPEDOM_DELETE_PGM_PE   _T("ETDeleteSequenceForPE") // ɾ����Ŀ     �Ƿ����ɾ����Ŀ�����Project Explorer��
#define POPEDOM_DELETE_MAT_PE   _T("ETDeleteMaterialForPE") // ɾ���ز�     �Ƿ����ɾ���زģ����Project Explorer��
#define POPEDOM_DELETE_BIN      _T("ETDeleteBin")           // ɾ��BIN�ļ�  �Ƿ����ɾ��BIN�ļ������Project Explorer��
#define POPEDOM_DELETE_PROJ		_T("ETDeleteProject")		// ɾ����Ŀ	    �Ƿ����ɾ����Ŀ���Լ���Ŀ�µ�Ŀ¼�����޸ģ������Project Explore��
#define POPEDOM_DELETE_GROUP	_T("ETDeleteProjectGroup")	// ɾ����Ŀ��	�Ƿ����ɾ����Ŀ�飨�����Project Explore��
#define POPEDOM_DELETE_TEMPL	_T("ETDeleteTemplate")		// ɾ��ģ��	    �Ƿ����ɾ��ģ�棨���Project Explorer��
#define POPEDOM_PURGE_MAT_PE    _T("ETPurgefromTranCanForPE")//����ز�     �Ƿ��������زģ����Project Explorer��
//}}

#define POPEDOM_CAPTURE_MAT		_T("ETCaptureMaterial")		// �ɼ��ز�	�Ƿ���Բɼ��ز�
#define POPEDOM_CAN_USE_MAT		_T("ETCanUseMaterial")		// ��ʹ��Material List	�Ƿ����ʹ��Material List����
#define POPEDOM_CAN_USE_EDIT	_T("ETCanUseEdit")			// ��ʹ�÷Ǳ�	�Ƿ����ʹ�÷Ǳ����
#define POPEDOM_CAN_USE_CENSOR	_T("ETCanUseCensor")		// ��ʹ����Ƭ	�Ƿ����ʹ����Ƭ����
#define POPEDOM_JUNIOR_CENSOR	_T("ETJuniorCensor")		// ��Ŀ�������Ȩ	����ֱ�Ӵ򿪱������Ľ�Ŀ�����޸ġ��������
#define POPEDOM_MIDDLE_CENSOR	_T("ETMiddleCensor")		// ��Ŀ�м����Ȩ	����ֱ�Ӵ򿪱������Ľ�Ŀ�����޸ġ��м���顢���ͺϳ�
#define POPEDOM_SENIOR_CENSOR	_T("ETSeniorCensor")		// ��Ŀ�ռ����Ȩ	���Դ򿪱��˵Ľ�Ŀ���ܹ������ϳɵ��زļ��Զ�����Ľ�������ͨ�������ݲ��ҿ����Ľ�Ŀ״̬�������ͨ����
#define POPEDOM_SENIOR_MODIFY	_T("ETSeniorModify")		// ��Ŀ�ռ��޸�Ȩ	�����޸�ͨ�����ռ����Ľ�Ŀ
#define POPEDOM_DUB				_T("ETDubPopedom")			//	����Ȩ��	���Ը���Ŀ����������

#define POPEDOM_OP_PLANLIST		_T("ETOperatePlanList")		//	PLANLIST����Ȩ��	����ɾ���༭




//�ؼ��Ĺ�����Ϣ��������
#define WM_USER_SETSOM				(WM_USER + 2979)		//��������ҳ�����
#define WM_USER_SETEOM				(WM_USER + 2980)		//��������ҳ�ĳ���
#define WM_USER_UPDATE_MARK_INFO	(WM_USER + 2990)		//ClipTrimmer���±�ǵ���Ϣ
#define WM_USER_SET_CURRENT_MARK	(WM_USER + 2991)		//ClipTrimmer���õ�ǰѡ�еı�ǵ�
#define WM_USER_SEEK_CLIPTRIMMER	(WM_USER + 2992)		//�ڱ�ǵ�ҳ��Seek�༭�ؼ�ClipTrimmer��Ϣ����һ������Ϊ֡λ��
#define WM_USER_SET_PROPDATA		(WM_USER + 2993)		//��������ҳ���ݣ�Ŀǰ����ΪwParam��ʾ������(1��ʾ���⣬2��ʾ��ע)��lParam���ַ�������ָ��
#define WM_USER_REFRESH_ML_OBJECT	(WM_USER + 2994)		//ˢ������ҳ����ʾ
#define WM_USER_ML_PROPDATA_CHANGE	(WM_USER + 2995)		//����ҳ�����ݷ����ı�
#define WM_USER_LOGGINGNOTYFY       (WM_USER + 2996)        //֪ͨINGEST����LOGGING���� wParam �ַ�������ָ��
#define WM_USER_INGESTMESSAGE       (WM_USER + 2997)        //import list ֪ͨML��Planlist
#define WM_USER_SEEK_SCENE_MARK		(WM_USER + 2998)		//�ڱ�ǵ�ҳ��Seek�༭�ؼ�ClipTrimmer��Ϣ����һ������ΪSceneIn,�ڶ�������ΪsceneOut֡λ��


//�����߽�ɫ����
typedef		long ET_PROP_CALLER_TYPE;//����ҳ����������
#define		ET_PROP_UNKNOW			0 //δ֪������
#define		ET_PROP_MATERIALLIST	1 //MaterialList�е�������ҳ��
#define		ET_PROP_PROJECTEXPLORER	2 //ProjectExplorer�е�������ҳ��
#define		ET_PROP_CLIPTRIMMER		3 //ClipTrimmer�е�������ҳ��
#define		ET_PROP_INGEST			4 //Ingest�е�������ҳ��
#define		ET_PROP_REMOTEPREVIEW	5 //Զ��Ԥ�����õ�����ҳ��
#define     ET_PROP_LOGGING         6 //logging�ն˵��õ�����ҳ��

// UDP��Ϣ��������ɾ�����޸ĵ������ߣ����������Ƿ���Ҫ���ܹ㲥��UDP��Ϣ
#define		ET_UDPMSG_CAUSER_UNKNOWN	ET_PROP_UNKNOW
#define		ET_UDPMSG_CAUSER_ML			ET_PROP_MATERIALLIST
#define		ET_UDPMSG_CAUSER_PE			ET_PROP_PROJECTEXPLORER
#define		ET_UDPMSG_CAUSER_CT			ET_PROP_CLIPTRIMMER
#define		ET_UDPMSG_CAUSER_ING		ET_PROP_INGEST
#define		ET_UDPMSG_CAUSER_NLE		(ET_PROP_INGEST + 1)
#define		ET_UDPMSG_CAUSER_OTHER		(ET_PROP_INGEST + 2)
#define		ET_UDPMSG_CAUSER_LOGGING    (ET_PROP_INGEST + 3)

//�Ǳ�ص�����
#define ET_NLECALLBACK_OPEN_PROGRAM				0x0001
#define ET_NLECALLBACK_CREATE_PROGRAM			0x0002
#define ET_NLECALLBACK_CLOSE_ALL_PGM			0x0003//�ر����н�Ŀ,����S_FALSE�����û������˹ر��ļ�,����S_OK
#define ET_NLECALLBACK_OPEN_CLIP_TO_EDITVIEW	0x0004
#define ET_NLECALLBACK_CREATE_CLIP				0x0005
#define ET_NLECALLBACK_GET_STANDARD_LIST		0x0006//ͨ���ص�6����������CStringArray���������Եõ���ʽ����Ƶ��ʽ��string��Ϣ
#define ET_NLECALLBACK_GET_STANDARD_INFO		0x0007//Ȼ�����ͨ���ص�7������intָ���ѡ���index,����ȡ����ʽ��Ϣ�������Ϣ�ǿ���ֱ������
#define ET_NLECALLBACK_GET_AUDIO_INFO			0x0008//ͨ���ص�8������ETMedia�Ǹ��ṹָ���ѡ���index������ȡ����Ƶ��Ϣ(��Ƶ��Ϣ��ֻ��audiochannels,audiosamplesrate,audiobitspersample���ã������Ķ���Ч)
#define ET_NLECALLBACK_SAVE_PROGRAM				0x0009//wparamΪ��Ҫ����Ľ�Ŀguid
#define ET_NLECALLBACK_RENAME_PGM				0x000A//wparamΪ��Ŀ��guidָ�룬lparamΪ�µ�����( CStringָ�� )
#define ET_NLECALLBACK_QUAERE_REFRESH_TIMELINE	0x000B//ѯ�ʷǱ��費��Ҫˢ��ʱ���ߣ�wParamΪ�ز�GUID
#define ET_NLECALLBACK_REFRESH_TIMELINE			0x000C//ˢ��ʱ���ߣ�wParamΪETArrayClipInfoָ�루��Ҫ���µ��زģ���lParamΪCArray<GUID, GUID&>ָ�루��ˢ�µĽ�ĿGUID��
#define ET_NLECALLBACK_GEALLUSEOBJ_FROM_NLE		0x001B//�õ��Ǳ���������ʹ�õĶ���wparamΪCArray<long, long>ָ��
#define ET_NLECALLBACK_SET_CURRENTPROJECT		0x001E//wparamΪ�������ַ���ָ��
#define ET_NLECALLBACK_GET_PGMVALIDLENGTH		0x001D//��ȡ��Ŀ����Ч����,wparamΪstuPgmָ��
#define ET_NLECALLBACK_IMPORTEDL				0x001F//����EDL���޲���
#define ET_NLECALLBACK_EXPORTEDL				0x0020//����EDL��wParamΪ��Ŀguidָ��(GUID *)
#define ET_NLECALLBACK_MODIFYPGMFILEGUID		0x0021//�޸Ľ�Ŀ�ļ���GUID��wParamΪstuModifyPgm��ָ��
#define ET_NLECALLBACK_GETICON					0x0022//��ȡICON buffer�� wParamΪstuGetIconParam��ָ��
#define ET_NLECALLBACK_NETNOTICEMSG				0x0023//����֪ͨ��Ϣ��wParamΪETObject�ṹ��ָ�룬lParamΪETNetMsgCallbackHeader�ṹ��ָ�룬���÷��Լ����ͷš�



//��קЭ���ж��������
const long ET_DRAGPROTOCOL_VER			= 25;

// ������������
typedef long ET_DRAG_OBJTYPEMASK_TYPE;
const long ET_DRAG_OBJTYPE_CLIP				= 0x0001;
const long ET_DRAG_OBJTYPE_PROGRAM			= 0x0002;
const long ET_DRAG_OBJTYPE_EDL				= 0x0004;
const long ET_DRAG_OBJTYPE_TEMPLATE			= 0x0008;
const long ET_DRAG_OBJTYPE_CD				= 0x0010;
const long ET_DRAG_OBJTYPE_MARK				= 0x0020;

// ��ק��Դ
typedef long ET_DRAG_SOURCE_TYPE;
const long ET_DRAG_SOURCE_UNKNOWN       = 0x00000;      // δ֪
const long ET_DRAG_SOURCE_EDITMAX		= 0x01000;		// �Ǳ�
const long ET_DRAG_SOURCE_PROJECTEXPOLOR= 0x01100;		// PE
const long ET_DRAG_SOURCE_MEDIA			= 0x02000;		// ý��
const long ET_DRAG_SOURCE_INGEST		= 0x03000;		// ��¼
const long ET_DRAG_SOURCE_STUDIO		= 0x04000;		// �ݲ���
const long ET_DRAG_SOURCE_CHANNEL		= 0x05000;		// Ƶ������
const long ET_DRAG_SOURCE_ARCHIVE       = 0X06000;      // ML��Archive�ڵ����
const long ET_DRAG_SOURCE_ARCHIVE_REMOTE= 0X06001;      // Զ��Archiveվ��
const long ET_DRAG_SOURCE_NETWORK       = 0X06100;      // ML�������ڵ����
const long ET_DRAG_SOURCE_REMOTE	    = 0X06200;      // ML��Զ�̿�ڵ����
const long ET_DRAG_SOURCE_LOCAL         = 0X06300;      // ML�ı��ؿ��ڵ����
const long ET_DRAG_SOURCE_XDA           = 0X07000;      // XDA����


//����E7�Ǳ���Ҫ����
enum em_ETLockType
{
	ET_emObject						=	4000,		//object����
	ET_emObjEdit					=	4010,		//obj�༭����
};

//��������
enum emObjectType
{
	ET_ObjType_Unknow		 = 0x00,			//Invalid
	ET_ObjType_SysTemplate	 = 0x001,			//ϵͳģ��
	ET_ObjType_PublicTemplate = 0x002,			//����ģ��
	ET_ObjType_PrivateTemplate = 0x004,			//˽��ģ��
	ET_ObjType_Folder		 = 0x010,			//�ļ���
	ET_ObjType_Clip			 = 0x020,			//�ز�
	ET_ObjType_Pgm			 = 0x040,			//��Ŀ
	ET_ObjType_Template		 = 0x080,			//ģ��
	ET_ObjType_EDL			 = 0x100,			//EDL
	ET_ObjType_CD			 = 0x200,			//����
	ET_ObjType_Event		 = 0x400,			//EventData������ʱ��ʱʹ�ã�
	ET_ObjType_Plan			 = 0x800,			//����planType �����½�plan
	ET_ObjType_Mark			 = 0x1000,          //Mark���� ���ڱ�ǵ�������ק
};
//{{Add by Liu Jia 2008-11-27
//�ļ�������
enum emFolderType
{
	ET_ObjType_PublicFolder  = 0x00, //Public�ļ���
	ET_ObjType_ArchiveFolder = 0x01, //ARCHIVE�ļ���
	ET_ObjType_OAFolder		 = 0x02, //OA�ļ���
	ET_ObjType_PEFolder      = 0x04, //PE�ļ���
	ET_ObjType_UnknownFolder = 0x10, //δ֪�ļ���  
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
	ET_OA_DARG_MODE_PGM			= 1,//��Ŀ��ק�����زģ���Ҫ���ͺϳ�
	ET_OA_DARG_MODE_CLIP		= 2,//�ز���ק�����زģ���Ҫ����ת��
	ET_OA_DARG_MODE_CLIP_NULL	= 4,//��ͨ�ز���ק��
};
// �ز�������
#define	ET_CLIPTYPE_HIDE		0x00	// �����ز�
#define	ET_CLIPTYPE_AV			0x01	// ��ͨ����Ƶ�ز�
#define	ET_CLIPTYPE_V			0x02	// ��ͨ��Ƶ�ز�
#define	ET_CLIPTYPE_A			0x04	// ��ͨ��Ƶ�ز� 
#define	ET_CLIPTYPE_XDCAM		0x08	// �����ز�
#define	ET_CLIPTYPE_TGA			0x10	// TGA�ز�
#define	ET_CLIPTYPE_IMAGE		0x20	// ͼƬ
#define	ET_CLIPTYPE_P2			0x40	// P2�ز�
#define	ET_CLIPTYPE_SGCLIP		0x80	// SG�زģ������ز�
#define ET_CLIPTYPE_OA_EMPTYCLIP	0x000200	//OA FOLDER�еĿ��ز�
#define ET_CLIPTYPE_EX			0x400	//EX�ز�
#define ET_CLIPTYPE_SYSUDF      0x800	//XDCAM UDF�ز�

// �ļ���������,���λ0��ʾΪMaterialList,1��ʾΪProject
#define ET_SUBTYPE_FOLDER_ML_PUBLIC				0x000001
#define ET_SUBTYPE_FOLDER_ML_ARCHIVE			0x000002
#define ET_SUBTYPE_FOLDER_ML_SEARCHING_RESULT	0x000004
#define ET_SUBTYPE_FOLDER_ML_TRASH_CAN			0x000008
#define ET_SUBTYPE_FOLDER_ML_SPECIAL_FOLDER		0x000010
#define ET_SUBTYPE_FOLDER_SITE_ROOT             0x000020           //վ���Ŀ¼,xsc,2007.7.14
#define ET_SUBTYPE_FOLDER_ML_OA_FOLDER		    0x000040
#define ET_SUBTYPE_FOLDER_ML_OA_STUDIO			0x000080
#define ET_SUBTYPE_FOLDER_ML_OA_MATRIAL			0x000100
#define ET_SUBTYPE_FOLDER_ML_OA_UTILITY			0x000200           //OA STUDIO �� Utility Folder

//#define ET_CLIPTYPE_OA_EMPTYCLIP		0x000200

#define ET_SUBTYPE_FOLDER_PJ_GROUP				0x100001
#define ET_SUBTYPE_FOLDER_PJ_PROJECT			0x100002
#define ET_SUBTYPE_FOLDER_PJ_BIN				0x100004
#define ET_SUBTYPE_FOLDER_PJ_TRASH				0x100008
#define ET_SUBTYPE_FOLDER_PJ_ROOT				0x100010
#define ET_SUBTYPE_FOLDER_PJ_SEARCHING_RESULT	0x100020
#define ET_SUBTYPE_FOLDER_PJ_SYSTRASH			0x100040

//��Ŀ������
#define ET_SUBTYPE_PGM_TL						0x000001		//��ͨʱ����
#define ET_SUBTYPE_PGM_CG_TEMPLATE				0x000002		//��Ļʱ����
#define ET_SUBTYPE_PGM_EDL						0x000003		//EDLʱ����


// �زĵ��ź���Դ
typedef		ULONGLONG ET_CLIP_SOURCE_TYPE;          //�ź���Դ
#define	 	ET_CLIP_SOURCE_TYPE_UNKNOW	0x00	// δ֪�ź�
#define	 	ET_CLIP_SOURCE_TYPE_VTR		0x01	// VTR�ɼ�
#define	 	ET_CLIP_SOURCE_TYPE_DUB		0x02	// �����ɼ�
#define	 	ET_CLIP_SOURCE_TYPE_XDCAM	0x04	// ����
#define	 	ET_CLIPSOURCE_P2			0x08	// P2
#define	 	ET_CLIPSOURCE_ILINK			0x10	// iLink
#define	 	ET_CLIPSOURCE_SGCLIP		0x20	// �����źŷ�����
#define		ET_CLIPSOURCE_CDAUDIO		0x40
#define		ET_CLIP_SOURCE_TYPE_IMPORT	0x80	// �����ļ�
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
//xw 2009/12/23 ӦNLEҪ�����Ӷ���
#define     ET_CLIP_SOURCE_TYPE_TITLEMOTIONANI   0x08000000   //TitleMotionAn
#define     ET_CLIP_SOURCE_TYPE_TITLEMOTIONSTA   0x10000000   //TitleMotionAn
#define     ET_CLIP_SOURCE_TYPE_LTC              0x20000000   //��Ӧ��������
#define     ET_CLIP_SOURCE_TYPE_VITC             0x40000000   //��Ӧ��������
#define     ET_CLIP_SOURCE_TYPE_CTL              0x80000000   //�պ�����32λ�ˣ��ټӾ��鷳��
//xw 20011/2/14 ӦNLEҪ�����Ӷ���
#define     ET_CLIP_SOURCE_TYPE_DVD              0x0000000100000000//DVD�ز�
#define     ET_CLIP_SOURCE_TYPE_SYSUDF			 0x0000000200000000//XDCAM UDF�ز�





//��Ŀ�������
typedef		ULONGLONG ET_PGMMASK_TYPE;          //�ź���Դ
#define	ET_PGMMASK_BASE				0x00000001			//OBJECT�����еĻ�������
#define	ET_PGMMASK_EXTINFO			0x00000002			//�����ֶ�
#define	ET_PGMMASK_ALL				0x0000000F			//ȫ����Ϣ

//Ŀ¼�������
typedef		ULONGLONG ET_FOLDER_TYPE;          //�ź���Դ
#define	ET_FOLDE_BASE				0x00000001			//OBJECT�����еĻ�������
#define	ET_FOLDE_EXTINFO			0x00000002			//�����ֶ�
#define	ET_FOLDER_ALL				0x0000000F			//ȫ����Ϣ

//�ز�ģ�黮��
typedef		ULONGLONG ET_CLIPMASK_TYPE;					//�ز�ģ�黮��
#define	ET_CLIPMASK_BASE			0x00000001			//����������Ϣ
#define	ET_CLIPMASK_CLIPINFO		0x00000002			//Ԫ������Ϣ
#define	ET_CLIPMASK_FILEINFO		0x00000004			//�ļ���Ϣ
#define	ET_CLIPMASK_MARKPOINT		0x00000008			//��ǵ���Ϣ
#define	ET_CLIPMASK_PLANNING		0x00000010			//�ƻ���Ϣ
#define	ET_CLIPMASK_INGEST			0x00000020			//��¼��Ϣ
#define ET_CLIPMASK_ORGMETADATA		0x00000040			//OrgMetaData
#define ET_CLIPMASK_EVENTMETADATA	0x00000080			//EventMetaData
#define ET_CLIPMASK_EXTDATA1		0x00000100			//�Ǳ��õ�����չ����
#define	ET_CLIPMASK_ALL				0x000000FF			//ȫ����Ϣ


//��ǵ����Ͷ���
#define ET_MARKPOINT_KEYFRAME				0x000001		//�ؼ�֡
#define ET_MARKPOINT_SIMPLE					0x000002		//һ���ǵ�
#define ET_MARKPOINT_SEGMENT				0x000004		//��Ƕ�
#define ET_MARKPOINT_ESSENCEMASK			0x000008		//EssenceMask
#define ET_MARKPOINT_CHANGEMASK			    0x0000010		//ChangeMask

// �زĵ������Դ
#define	 	ET_CLIPSOURCE_CREATE	0	// �Ǳ�����
#define	 	ET_CLIPSOURCE_NLE		1	// �Ǳ�ɼ�����
#define	 	ET_CLIPSOURCE_NLE_DUB	2	// �Ǳ���������
#define	 	ET_CLIPSOURCE_IMPORT	3	// �Ǳ������ز�
#define	 	ET_CLIPSOURCE_STUDIO	4	// �ݲ��һز�
#define	 	ET_CLIPSOURCE_MPC		5	// MPC ���
#define	 	ET_CLIPSOURCE_UPLOAD	7	// ��������
#define	 	ET_CLIPSOURCE_RECORD	9	// ��¼���
#define	 	ET_CLIPSOURCE_MAM		11	// ý�����
#define	 	ET_CLIPSOURCE_MAM_MOVE	12	// ý�ʻ�Ǩ
#define		ET_CLIPSOURCE_ML_IMPORT	13	// ML�����ز�
#define     ET_CLIPSOURCE_XDA_RETRIEVE 14 //XDA��Ǩ  

// �زĵĹ���״̬0-����,1-˽��,2-����,3-����
#define		ET_PUBLIC_STATE_ALL		0	// ����
#define		ET_PUBLIC_STATE_PRIVATE	1	// ˽��
#define		ET_PUBLIC_STATE_GROUP	2	// ����
#define		ET_PUBLIC_STATE_SELECT	3	// ����

typedef long  ET_CLIP_CAPTURESTATUS;			//�زĲɼ�״̬
#define  ET_CLIP_CAPTURESTATUS_NORMAL			0x00000000		//Ϊ����״̬
#define  ET_CLIP_CAPTURESTATUS_CAPTURE			0x00000001		//Ϊ���ڲɼ�״̬
#define  ET_CLIP_CAPTURESTATUS_LOOPCAPTURE		0x00000002		//Ϊѭ���ɼ�״̬
#define  ET_CLIP_CAPTURESTATUS_CREATELOW		0x00000010		//Ϊ�ز����ɵ�����״̬
#define  ET_CLIP_CAPTURESTATUS_CLIPTRIM			0x00000020		//Ϊ�ز�TRIM�����е�״̬
#define  ET_CLIP_CAPTURESTATUS_RENDERING		0x00000040		//Ϊ��Ŀ�ϳ�״̬
#define  ET_CLIP_CAPTURESTATUS_TRANSCODEING		0x00000080		//Ϊ�ز�ת��״̬
#define  ET_CLIP_CAPTURESTATUS_XDCAMUPLOW		0x00000100		//Ϊ���������������״̬
#define  ET_CLIP_CAPTURESTATUS_XDCAMUPHIGHT		0x00000200		//Ϊ���������������״̬
#define  ET_CLIP_CAPTURESTATUS_XDCAMCREATELOW	0x00000400		//Ϊ�����������������Ƶת��MP3���״̬
#define  ET_CLIP_CAPTURESTATUS_COPYING			0x00000800		//�����е��ز�
#define  ET_CLIP_CAPTURESTATUS_RENDERPGMTOOA	0x00001000		//�½��Ŀ��زģ����ڷǱ�MASTERVIEW��ק�����ز��Ϻϳɽ�Ŀ
#define  ET_CLIP_CAPTURESTATUS_FROM3RD			0x00002000		//���ڴӵ����������ز�
#define  ET_CLIP_CAPTURESTATUS_TRANSMITTING     0x00004000      //���ڴ�����ϵͳ����
#define  ET_CLIP_CAPTURESTATUS_TRANSMIT_FINISHED       0x00008000      //�زĴ������
#define  ET_CLIP_CAPTURESTATUS_NLE_RENDERING	0x00010000		//�Ǳ౾��Render���ز�,����Renderʱ,Clip StatusΪNLE Rendering
#define  ET_CLIP_CAPTURESTATUS_FAILEDFROM3RD    0x00020000      //�ӵ����������ز�ʧ�ܣ���״̬�زĵĲ�����ET_CLIP_CAPTURESTATUS_FROM3RDһ��



typedef ULONGLONG  ET_CLIP_IMAGETYPE;				//�ز��ļ�״̬
#define  ET_CLIP_IMAGETYPE_ORIGINAL			0x0000000000000000		//Ϊ�ز�ԭʼ����
#define  ET_CLIP_IMAGETYPE_4_3				0x0000000000000001		//ͼ��Ϊ4:3�ķ�ʽ
#define  ET_CLIP_IMAGETYPE_16_9SD			0x0000000000000002		//ͼ��Ϊ16:9�ķ�ʽ��16:9SD
#define  ET_CLIP_IMAGETYPE_16_9HD			0x0000000000000004		//ͼ��Ϊ16:9�ķ�ʽ��16:9HD
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

//imagetype �����زĸߵ�������־λSonaps���˰�߰��֮����жϣ���Ҫ��δ������ȫ�����������
#define  ET_CLIP_IMAGETYPE_FULLHIGH         0x0000000000100000    //
#define  ET_CLIP_IMAGETYPE_NOHIGH           0x0000000000200000   //
#define  ET_CLIP_IMAGETYPE_FULLLOW          0x0000000000400000   //
#define  ET_CLIP_IMAGETYPE_SUBLOW           0x0000000000800000   //
#define  ET_CLIP_IMAGETYPE_NOLOW            0x0000000001000000   //

//{{2010/4/22 niejiagang:Ӧ�Ǳ��Ҫ���������������궨��
#define ET_CLIP_IMAGETYPE_GROUPCLIPXDCAM	0x0000000002000000 
#define ET_CLIP_IMAGETYPE_GROUPCLIPPEV		0x0000000004000000
#define ET_CLIP_IMAGETYPE_GROUPCLIPPEH		0x0000000008000000
//}}
//{{2010/9/06 niejiagang:Ӧ�Ǳ��Ҫ���������������궨��
#define ET_CLIP_IMAGETYPE_P2NATIVE			0x0000000010000000 // P2 Native
#define ET_CLIP_IMAGETYPE_STILLPICTURE      0x0000000020000000 //Still Picture
#define ET_CLIP_IMAGETYPE_SCENECLIPFORMSV   0x0000000040000000 //SV��Make Scene In/Out
#define ET_CLIP_IMAGETYPE_EXAVI             0x0000000080000000 //EX ���е�AVI�ļ�
#define ET_CLIP_IMAGETYPE_CREATETIME        0x0000000100000000 // �ز��Ƿ��вɼ�ʱ��
#define ET_CLIP_IMAGETYPE_P2TRANSCODE		0x0000000200000000 // P2 TransCode
//}}
//{{2010/9/21 niejiagang:Ӧ�Ǳ��Ҫ���������¼����궨��
#define ET_CLIP_IMAGETYPE_ZOOMPROXY				0x0000000400000000
#define ET_CLIP_IMAGETYPE_NOADPTVIDEOSTAND		0x0000000800000000
#define ET_CLIP_IMAGETYPE_LEVELCONVERSION		0x0000001000000000 // level conversion
#define ET_CLIP_IMAGETYPE_SUBCLIPFROMSV			0x0000002000000000 // level conversion
#define ET_CLIP_IMAGETYPE_RENDERTLDEFAULTSET	0x0000004000000000 //ʱ����RenderToBin�Ƿ�ʹ��Ĭ������
#define ET_CLIP_IMAGETYPE_STILLFRAME			0x0000008000000000 //�Ƿ�֡�ز� ��֡��֡��֡��֡
#define ET_CLIP_IMAGETYPE_FIXEDASPECT			0x0000010000000000 //�Ƿ��ǹ̶���߱�
#define ET_CLIP_IMAGETYPE_VIRUALSPEEDRES		0x0000020000000000 //�������
#define ET_CLIP_IMAGETYPE_MEDIAOFFLINE			0x0000040000000000 //Media Off Line

//}}




//�زĹ鵵״̬����
typedef long  ET_CLIP_ARCHIVE;				//�زĹ鵵״̬
#define  ET_CLIP_ARCHIVE_NORMAL				0x00000000		//�زĴ��ڷǹ鵵״̬
#define  ET_CLIP_ARCHIVE_SUBMIT				0x00000001		//�زĴ����ύ�鵵״̬
#define  ET_CLIP_ARCHIVE_FINISHED			0x00000002		//�زĴ��ڹ鵵���״̬
#define  ET_CLIP_ARCHIVE_FAILED				0x00000003		//�زĴ��ڹ鵵ʧ��״̬
#define  ET_CLIP_ARCHIVE_SCHEDULE           0X00000004      //�زĴ��ڼƻ��鵵״̬

#define  ET_CLIP_ARCHIVE_MAM				0x00000010		//�زĴ�����ý����
#define  ET_CLIP_ARCHIVE_RETRIEVE_SUBMIT	0x00000011		//�زĴ����ύ��Ǩ״̬
#define  ET_CLIP_ARCHIVE_RETRIEVE_FINISHED	0x00000012		//�زĴ��ڻ�Ǩ���״̬
#define  ET_CLIP_ARCHIVE_RETRIEVE_FAILED	0x00000013		//�زĴ��ڻ�Ǩʧ��״̬
#define  ET_CLIP_ARCHIVE_MAM_3RD            0x00000014      //�ز����ڵ�����ý����
#define  ET_CLIP_ARCHIVE_RETRIEVE_DOING     0x00000015      //���ز����ڻ�Ǩ֮��, �����ٷ���Ǩ

//�ز�����鵵״̬���壨Tobe Archive + Not auto-Archive��
#define	 ET_CLIP_ARCHIVE_SPECIAL_NONE					0x00000000 //û������״̬
#define	 ET_CLIP_ARCHIVE_SPECIAL_TOBE_ARCHIVE_ONLY		0x00000001 //�زĴ��ڼ����鵵״̬
#define	 ET_CLIP_ARCHIVE_SPECIAL_NOT_AUTOMATICALLY_ONLY	0x00000002 //�زĴ��ڲ������Զ��鵵״̬
#define	 ET_CLIP_ARCHIVE_SPECIAL_ALL					0x00000003 //�ز�ͬʱ���ڼ����鵵�Ͳ������Զ��鵵״̬
#define  ET_CLIP_ARCHIVE_SPECIAL_TOBE_RETRIEVE_ONLY     0x00000004 //����ý�ʿ��е��زĴ��ϵļ�����Ǩ��־,��ǰ�������״̬����.

//�ļ�������ҳ�To be Archive��Not to be Auto��archive ���ñ���Ķ���
#define ET_FOLDER_ARCHIVE_TOBE_ARCHIVE_ONLY_MODIFY_TRUE			0x00000010	//ֻ��TOBE_ARCHIVE����ΪTRUE�����������޸�
#define ET_FOLDER_ARCHIVE_TOBE_ARCHIVE_ONLY_MODIFY_FALSE		0x00000011	//ֻ��TOBE_ARCHIVE����ΪFALSE�����������޸�
#define ET_FOLDER_ARCHIVE_NOT_AUTOMATICALLY_ONLY_MODIFY_TRUE	0x00000012	//ֻ��Not to be autoarchive����ΪTRUE�����������޸�
#define ET_FOLDER_ARCHIVE_NOT_AUTOMATICALLY_ONLY_MODIFY_FALSE	0x00000013	//ֻ��Not to be autoarchive����ΪFALSE�����������޸�


//�ز��ļ�����״̬
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
#define ET_Obj_FS_MXF		0x00008000	//��ʾ�ز���MXF�ز�
#define ET_Obj_FS_H264		0x00010000	//��ʾ�ز���������H.264�ļ���
#define ET_Obj_FS_H264_Creating 0x00020000	//��ʾ�ز�����������H.264�ļ���

// �ز��ļ�״̬����
#define ET_ClipFileState_Unknown       -1
#define ET_ClipFileState_NormalOnline   0
#define ET_ClipFileState_Ingesting      1
#define ET_ClipFileState_NearLine       2
#define ET_ClipFileState_OffLine        3

//վ�����Ͷ���
typedef long ETVML_PLUGIN_TYPE;
#define ETVML_PLUGIN_TYPE_NETWORK			0X01             //����(���Ĵ洢)վ��
#define ETVML_PLUGIN_TYPE_LOCAL				0X02             //���ؿ�
#define ETVML_PLUGIN_TYPE_REMOTE			0X04             //Զ��վ��
#define ETVML_PLUGIN_TYPE_ARCHIVE			0X08             //ý��վ��
#define ETVML_PLUGIN_TYPE_REMOTEARCHIVE		0X10             //Զ��ý��վ��
#define ETVML_PLUGIN_TYPE_VSEARCHLIST		0X20             //����Ĺ�����SearchResult

//�ն�����
//0��δ֪��1��ΪMLվ���նˣ�2��ΪPEվ���ն�
typedef long ET_TERMINAL_TYPE;
#define ET_TERMINAL_TYPE_UNKNOW 0
#define ET_TERMINAL_TYPE_ML		1
#define ET_TERMINAL_TYPE_PE		2

//���ݿ��վ��ID
//0��Ϊ�����ݿ����ȱʡ���ݿ⣬1��Ϊ�������ݿ��վ��ID��������Ϊָ�������ݿ��վ��ID
#define ET_DATABASE_ID_DEFAULT	0
#define ET_DATABASE_ID_LOCAL	1


#define	ET_OBJECT_POPEDOMTYPE_READ		1		// ����Ȩ��
#define	ET_OBJECT_POPEDOMTYPE_MODIFY	2		// �޸�Ȩ��

typedef enum emObjectPopedomType
{	
	ET_OBJECT_POPEDOM_PUBLIC		 = 0,		// ����
	ET_OBJECT_POPEDOM_PRIVATE		 = 1,		// ˽��
	ET_OBJECT_POPEDOM_GROUP			 = 2,		// ����
	ET_OBJECT_POPEDOM_CONDITION      = 3		// �Զ���
}ET_OBJECT_POPEDOM;

#define ETMAKEULONGLONG(a,b)	((ULONGLONG)(((DWORD)(a)) | ((ULONGLONG)((DWORD)(b))) <<32))

//ý������ö�ٶ���
enum emMediaFormatDefType
{
	ET_emMediaFmt_Unknow = 0,	//δ֪����
	ET_emMediaFmt_Video  = 1,	//��Ƶ
	ET_emMediaFmt_Audio  = 2,	//��Ƶ
	ET_emMediaFmt_Key	 = 3,	//��
	ET_emMediaFmt_Picture= 4	//ͼƬ
};

//����ָ�ѡ��
enum ET_Object_Resume_Type
{
	ET_Object_Resume_ValidateOnly				= 0,			//������Ч������Ƿ�ɾ��
	ET_Object_Resume_TrashCanOnly				= 1,			//�����ָ���ɾ��������վ�Ķ��󣬶����ж�����Ч
	ET_Object_Resume_All						= 2,			//���Իָ����е����ݣ�����Ŀ¼���Իָ���������ɾ��������
};
//��ȡ����·��ѡ��
enum ET_Object_GetPath_Type
{
	ET_Object_GetPath_Normal					= 0,			//��ȡ���������·��,����ɾ���Ķ���ȡ����·��
	ET_Object_GetPath_Really					= 1,			//��ȡ���������·��,��������վ����;
	ET_Object_GetPath_Original					= 2,			//��ȡ����ıܿ�����վ���ԭʼ·��,�����ڶ���ָ�
	ET_Object_GetPath_All						= 3,			//��ȡ�����ԭʼ·��,��ʹ���󱻲��ɾ��
	ET_Object_GetPath_WithSiteName              = 4,            //���غ�վ�����Ƶ�·��
};

enum ET_NM_OBJECT_TYPE
{
	// Ƶ���ڵ�
	ET_NM_OBJECT_TYPE_CHANNEL_NODE				= 3, 
	// Ƶ��
	ET_NM_OBJECT_TYPE_CHANNEL					= 4, 
	// ��Ŀ�ڵ�
	ET_NM_OBJECT_TYPE_COLUMN_NODE				= 5, 
	// ��Ŀ
	ET_NM_OBJECT_TYPE_COLUMN					= 6, 
};
/////////////////////////////mosServer�������Ķ�����////////////////////////////////////////////////
enum ServerMsg_ObjectStatus
{
	ServerMsg_ObjectStatus_Add			=	0x00000005,		//ǰ3��Ϊml�ڲ�״̬
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
typedef long ET_EMPTY_PLAN_STATUS;								//��plan״̬
#define	ET_EMPTY_PLAN_CREATE				=   0x00000010,		//���زĴ���
#define	ET_EMPTY_PLAN_MOVE_TRASH			=   0x00000011,		//���ز��ƶ�������վ
#define	ET_EMPTY_PLAN_MOVE					=   0x00000012,		//���ز��ƶ�����ͨ�ļ���
#define	ET_EMPTY_PLAN_DELETE				=   0x00000013,		//���زĳ���ɾ��


//����Ŀ״̬����
typedef DWORD ETPgmStatus;
#define ET_PGM_STATUS_EDIT				0					//�༭״̬													//��Ӧ��PGMSTATUSͼ��λ��29
#define ET_PGM_STATUS_SUBMIT			10					//�ύ��Ŀ						��Ŀ�������				//��Ӧ��PGMSTATUSͼ��λ��31
#define ET_PGM_STATUS_JUNIORCENSOR		20					//����������					��Ŀ�������				//��Ӧ��PGMSTATUSͼ��λ��31
#define ET_PGM_STATUS_MIDDLECENSOR		30					//�м����������ύ�ϳ�		��Ŀ������ɣ���������Ļ�༭����	//��Ӧ��PGMSTATUSͼ��λ��32
#define ET_PGM_STATUS_COMPOSING			40					//���ںϳ�													//��Ӧ��PGMSTATUSͼ��λ��33
#define ET_PGM_STATUS_COMPOSEFAIL		45					//�ϳ�ʧ��													//��Ӧ��PGMSTATUSͼ��λ��44
#define ET_PGM_STATUS_COMPOSED			50					//�ϳ����				��Ҫ��Ƭ�ĺϳɽ������ȴ���Ƭ		//��Ӧ��PGMSTATUSͼ��λ��34
#define ET_PGM_STATUS_COMPOSELOW		60					//��ɵ����ʺϳ�		MPEG4��ʽ							//��Ӧ��PGMSTATUSͼ��λ��35
#define ET_PGM_STATUS_AUTOTECHCENSOR	70					//����Զ�����												//��Ӧ��PGMSTATUSͼ��λ��35
#define ET_PGM_STATUS_SENIORCENSOR		80					//��Ŀ���ͨ��												//��Ӧ��PGMSTATUSͼ��λ��36

//����Ŀ����״̬����
typedef DWORD ETPgmSecStatus;
#define ET_PGM_SECSTATUS_NODUB			0					//δ����													//��Ӧ��PGMSTATUSͼ��λ��36
#define ET_PGM_SECSTATUS_DUB			10					//�������													//��Ӧ��PGMSTATUSͼ��λ��37

//!�ĸ�״̬����
typedef DWORD ETNewsStatus;
#define ET_NEWS_STATUS_EDIT             1                   //����д��
#define ET_NEWS_STATUS_WAITFIRSTCENSOR  3                   //�ȴ�����
#define ET_NEWS_STATUS_WAITSECENDCENSOR 4					//�ȴ�����
#define ET_NEWS_STATUS_PASS             5                   //ͨ�����

//!������
typedef DWORD ETPlayListStatus;
#define ET_PL_STATUS_EDIT             1                   //����д��
#define ET_PL_STATUS_WAITFIRSTCENSOR  3                   //�ȴ�����
#define ET_PL_STATUS_WAITSECENDCENSOR 4					//�ȴ�����
#define ET_PL_STATUS_PASS             5                   //ͨ�����


//����Ŷ���
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
#define  ET_ERROR_PARAM						1//��������
#define  ET_ERROR_NO_FOLDER					2//û���ҵ���Ӧ���ļ���
#define  ET_ERROR_NO_POPEDOM				4//û��Ȩ��
#define  ET_ERROR_NO_DATA					8//����Ϊ��
#define  ET_ERROR_OPEN_FOLDER				16//�򲻿��ļ��У��ļ��б�ɾ��������վ
#define  ET_ERROR_NO_OBJECT					32//Ŀ¼�ҵ��ˣ�CLIP/SEQUENCE���ܱ�ɾ��


// �������������ض���
// ��������ĺ�
#define FIELDMASK(field)				((ULONGLONG)1 << (field-1))
#define FIELD_ISSET(_mask, _field)		( ( (_mask) & FIELDMASK(_field) ) == FIELDMASK(_field) )

// ETObject�ṹ���е��ֶ�����
enum MASK_ETOBJECT
{
	OBJ_FIELD_ObjectID			= 1,		//����ID
	OBJ_FIELD_ObjectGuid		= 2,		//����GUID
	OBJ_FIELD_ObjectType		= 3,		//��������
	OBJ_FIELD_SubType			= 4,		//����������
	OBJ_FIELD_FolderID			= 5,		//�ļ���ID
	OBJ_FIELD_PopdomID			= 6,		//Ȩ��ID
	OBJ_FIELD_CDID				= 7,		//����ID
	OBJ_FIELD_ObjectName		= 8,		//��������
	OBJ_FIELD_KeyWord			= 9,		//�ؼ���
	OBJ_FIELD_IconFrame			= 10,		//ͼ��֡
	OBJ_FIELD_IconFileName		= 11,		//ͼ���ļ�
	OBJ_FIELD_Length			= 12,		//����
	OBJ_FIELD_TrimIn			= 13,		//���
	OBJ_FIELD_TrimOut			= 14,		//����
	OBJ_FIELD_CreaterID			= 15,		//������ID
	OBJ_FIELD_CreateDate		= 16,		//��������
	OBJ_FIELD_MenderID			= 17,		//�޸���ID	
	OBJ_FIELD_ModifyDate		= 18,		//�޸�����
	OBJ_FIELD_DeleteFlag		= 19,		//ɾ����־
	OBJ_FIELD_DelaterID			= 20,		//ɾ����ID
	OBJ_FIELD_DelateDate		= 21,		//ɾ������
	OBJ_FIELD_RecordDate		= 22,		//¼������
	OBJ_FIELD_ShowDate			= 23,		//��������
	OBJ_FIELD_DistributeDate	= 24,		//��������
	OBJ_FIELD_DistributerID		= 25,		//������
	OBJ_FIELD_ArchiveDate		= 26,		//�鵵����
	OBJ_FIELD_ArchiverID		= 27,		//�鵵����
	OBJ_FIELD_ArchiveFlag		= 28,		//�鵵��־
	OBJ_FIELD_ColumnID			= 29,		//��ĿID
	OBJ_FIELD_VideoStandard		= 30,		//��ʽ
	OBJ_FIELD_Status			= 31,		//״̬
	OBJ_FIELD_UMID				= 32,		//�����UMID
	OBJ_FIELD_UsedFlag			= 33,		//ʹ�ñ�־
	OBJ_FIELD_LockFlag			= 34,		//������־
	OBJ_FIELD_FileStatus		= 35,		//�ļ�״̬
	OBJ_FIELD_Rights			= 36,		//��Ȩ��Ϣ
	OBJ_FIELD_JournalList		= 37,		//����
	OBJ_FIELD_MediaChannel		= 38,		//ý��ͨ��
	OBJ_FIELD_ObjectNote		= 39,		//����ע
	OBJ_FIELD_Reserved1			= 40,		//��������1
	OBJ_FIELD_Reserved2			= 41,		//��������2
	OBJ_FIELD_Reserved3			= 42,		//��������3
	OBJ_FIELD_Reserved4			= 43,		//��������4
	OBJ_FIELD_CaptureStatus		= 44,		//�ɼ�״̬
	OBJ_FIELD_ImageType			= 45,		//ͼ������
	OBJ_FIELD_EditTerminal		= 46,		//�༭�ն�
	OBJ_FIELD_ModifyTerminal	= 47,		//�޸��ն�
	OBJ_FIELD_DeviceGUID		= 48,		//�豸GUID
	OBJ_FIELD_TobeArchiveState  = 49,		//����鵵��־
	OBJ_FIELD_AccessDate		= 50,		//����ʱ���־
	OBJ_FIELD_3rdArchiveFlag	= 51,		//3���鵵��־
    OBJ_FIELD_DBEStreamChannel     = 52,       //DBE��Ƶͨ��
	OBJ_FIELD_NTSCTCMode		= 53,		//ʱ�����ģʽ
	OBJ_FIELD_MaterialID        = 54,      //2010/05/24 zxp:add MaterialID
	OBJ_FIELD_BKColor          = 55,      //������ɫ
	OBJ_FIELD_Reserved20		= 56,        //XDCAMCOMPILE����
	OBJ_FIELD_StoryID			= 57,        //STORYID
	OBJ_FIELD_ResSourceGuid		= 58,	//Դ�ز�GUID
};

enum MASK_ETOBJECTEx
{
	OBJ_FIELDEX_Reserved5			= 1,		//��������5
	OBJ_FIELDEX_Reserved6			= 2,		//��������6
	OBJ_FIELDEX_Reserved7			= 3,		//��������7
	OBJ_FIELDEX_Reserved8			= 4,		//��������8
	OBJ_FIELDEX_Reserved9			= 5,		//��������9
	OBJ_FIELDEX_Reserved10		= 6,		//��������10
	OBJ_FIELDEX_Reserved11		= 7,		//��������11
	OBJ_FIELDEX_Reserved12		= 8,		//��������12
	OBJ_FIELDEX_Reserved13		= 9,		//��������13
	OBJ_FIELDEX_Reserved14		= 10,		//��������14
	OBJ_FIELDEX_XDAArchiveFlag  = 11,       //XDA�鵵��־
};

// ETClipInfo�ṹ���е��ֶ�����
enum MASK_ETCLIPINFO
{
	CLP_FIELD_SignalSourceType	= 1,		//�ɼ��ź�����
	CLP_FIELD_SignalSourceDesc	= 2,		//�ɼ��ź�����
	CLP_FIELD_SignalSourceMark	= 3,		//�ɼ��źű�ǩ
	CLP_FIELD_VtrInPoint		= 4,		//�Ŵ����
	CLP_FIELD_VtrOutPoint		= 5,		//�Ŵ�����
	CLP_FIELD_VtrInNext			= 6,		//�Ŵ���չ���
	CLP_FIELD_VtrOutNext		= 7,		//�Ŵ���չ����
	CLP_FIELD_StorageCode		= 8,		//�洢����
	CLP_FIELD_KeepDays			= 9,		//�ز��ڷǱ���еı�������
	CLP_FIELD_ClipSource		= 10,		//�زĵ������Դ
	CLP_FIELD_IngestTaskID		= 11,		//��¼����ID
	CLP_FIELD_PlanningID		= 12,		//�ƻ�ID
	CLP_FIELD_TC				= 13,		//ʱ��
	CLP_FIELD_Version			= 14,		//�汾
	CLP_FIELD_ClipLanguage		= 15,		//����
	CLP_FIELD_SerialData		= 16,		//TGA����
	CLP_FIELD_SerialSize		= 17,		//TGA����
	CLP_FIELD_IngestMetaData	= 18,		//��¼Ԫ����
	CLP_FIELD_PlanningMetaData	= 19,		//�ƻ�Ԫ����
	CLP_FIELD_ClipFile			= 20,		//�ļ��б�
	CLP_FIELD_ClipMarkPoint		= 21,		//�زı�ǵ�
	CLP_FIELD_SrcPgmGUID		= 22,		//Դ��ĿGUID
	CLP_FIELD_ItemName			= 23,
	CLP_FIELD_Category			= 24,
	CLP_FIELD_ProgramName       = 25,
	CLP_FIELD_SrcSiteID			= 26,
	CLP_FIELD_RestrictViewContent = 27,
	CLP_FIELD_OrgMetadata		= 28,
	CLP_FIELD_EventMetadata		= 29,
	CLP_FIELD_OriginalMaterial  = 30,       //2009/04/21 zxp:add Դ�ļ���
};

// ETIngestMetaData�ṹ���е��ֶ�����
enum MASK_ETINGEST
{
	ING_FIELD_TaskID			= 1,		//����ID
	ING_FIELD_TaskName			= 2,		//��������
	ING_FIELD_StartDate			= 3,		//��¼��ʼʱ��
	ING_FIELD_EndDate			= 4,		//��¼����ʱ��
	ING_FIELD_DurDate			= 5,		//��¼����ʱ��
	ING_FIELD_Status			= 6,		//��¼״̬
	ING_FIELD_Process			= 7,		//��¼����
	ING_FIELD_SourceTapeID      = 8,        //Source Tape ID
	ING_FIELD_BackupTapeID      = 9,        //Backup Tape ID
	ING_FIELD_UserName			= 10,		//��¼�û���
	ING_FIELD_Terminal			= 11,		//��¼�ն�
	//{{2009/04/22 zxp:add ���Ԫ����
	ING_FIELD_ChannelName       = 12,
	ING_FIELD_DeviceID          = 13,
	ING_FIELD_AudioChannelCount = 14,
	ING_FIELD_SourceTapeBarcode = 15,
};


// ETPlanningMetaData�ṹ���е��ֶ�����
enum MASK_ETPLANNING{

	PLN_FIELD_PlanningID		= 1,		//�ƻ�ID
	PLN_FIELD_PlanningGUID		= 2,		//�ƻ�GUID
	PLN_FIELD_PlanName			= 3,		//�ƻ���
	PLN_FIELD_CreatorName		= 4,		//��������
	PLN_FIELD_CreateDate		= 5,		//��������
	PLN_FIELD_ModifyName		= 6,		//�޸�����
	PLN_FIELD_ModifyDate		= 7,		//�޸�����
	PLN_FIELD_Version			= 8,		//�汾
	PLN_FIELD_Place				= 9,		//�ص�
	PLN_FIELD_PlanningDate		= 10,		//�ƻ�����
	PLN_FIELD_Director			= 11,		//����
	PLN_FIELD_InputBy			= 12,		//¼��
	PLN_FIELD_PhotoGrapher		= 13,		//��Ӱ
	PLN_FIELD_Reporter			= 14,		//����
	PLN_FIELD_Other				= 15,		//������Ա
	PLN_FIELD_Equipment			= 16,		//�豸
	PLN_FIELD_ContactInfo		= 17,		//��ϵ��Ϣ
	PLN_FIELD_PlanningXml		= 18,		//�ƻ�XML
	PLN_FIELD_InputSource		= 19,		//�ƻ�����Դ
	PLN_FIELD_StartDate			= 20,		//�ƻ���¼��ʼʱ��
	PLN_FIELD_DurDate			= 21,		//�ƻ���¼����ʱ��
	PLN_FIELD_SourceType		= 22,       //�ɼ�Դ����
	PLN_FIELD_IngestExtendXml	= 23,       //�ɼ��ֶ�Planning��Ϣ
};

//{{2009/02/27 zxp:add ETOrgMetaData�ṹ���е��ֶ�����
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

// ETTemplateObject�ṹ���е�����
enum MASK_ETTEMPLATEOBJECT{
	TMP_FIELD_TypeGuid			= 1,		//ģ������GUID	
	TMP_FIELD_FilePath			= 2,		//ģ�����õ��ļ���	
	TMP_FIELD_Resource			= 3,		//ģ����Դ����
	TMP_FIELD_TempData			= 4,		//ģ������
	TMP_FIELD_TempSize			= 5,		//ģ�����ݴ�С
};

// ETPgmMaster�ṹ���е�����
enum MASK_PGMMASTER{
	PGM_FIELD_ClipPath			= 1,		//�زĴ��·��
	PGM_FIELD_VideoTrack		= 2,		//��Ƶ����
	PGM_FIELD_AudioTrack		= 3,		//��Ƶ����
	PGM_FIELD_GraphTrack		= 4,		//��Ļ����
	PGM_FIELD_BackTrack			= 5,		//��������
	PGM_FIELD_AudioChannel		= 6,		//�����Ƶͨ����
	PGM_FIELD_TLBlock			= 7,		//�Ƿ�����ʱ����
	PGM_FIELD_FilePath			= 8,		//�ļ�·��
	PGM_FIELD_BackFilePath		= 9,		//�����ļ�·��
	PGM_FIELD_UndoFilePath		= 10,		//UNDO����·��
	PGM_FIELD_VideoFormatID		= 11,		//��Ƶ��ʽID
	PGM_FIELD_AudioFormatID		= 12,		//��Ƶ��ʽID
	PGM_FIELD_SaveState			= 13,		//��Ŀ����״̬
	PGM_FIELD_EditTime			= 14,		//�༭�ۼ�ʱ��
	PGM_FIELD_DubUserID			= 15,		//��������ԱID
	PGM_FIELD_DubAnnouncerID	= 16,		//����ԱID
	PGM_FIELD_SECStatus			= 17,		//��Ŀ����״̬
	PGM_FIELD_SysVersion		= 18,		//�༭ϵͳ�汾
	PGM_FIELD_EditVersion		= 19,		//��Ŀ�޸ĵİ汾��
};

// ETTemplateObject�ṹ���е�����
enum MASK_FOLDERINFO{
	FOLDER_FIELD_TypeGuid			= 1,		//����GUID
	FOLDER_FIELD_VideoFormatI		= 2,		//����Ƶ��ʽID
	FOLDER_FIELD_AudioFormatID		= 3,		//����Ƶ��ʽID
	FOLDER_FIELD_LVideoFormatID		= 4,		//����Ƶ��ʽID
	FOLDER_FIELD_LAudioFormatID		= 5,		//����Ƶ��ʽID
	FOLDER_FIELD_RHVideoFormatI		= 6,		//�������Ƶ��ʽID
	FOLDER_FIELD_RHAudioFormatID	= 7,		//�������Ƶ��ʽID
	FOLDER_FIELD_RLVideoFormatID	= 8,		//�������Ƶ��ʽID
	FOLDER_FIELD_RLAudioFormatID	= 9,		//�������Ƶ��ʽID
	FOLDER_FIELD_UseOriginalTC		= 10,		//ʹ��ԭʼʱ��ID
	FOLDER_FIELD_AudioChannel		= 11,		//�����Ƶͨ����ID
};

//ETGTMAgentData�ṹ���е�����
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
//�̳�E7�Ǳ�����Ķ�������
//****************************************************************************************************************
//��Դ���Ͷ���
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


typedef DWORD  ET_CLIP_CLASS_TYPE;                //�ز�����
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

#define  ET_CLIP_FILE_KEY    9    //��  
#define  ET_CLIP_FILE_GRAPH  10   //ͼ��
#define  ET_CLIP_FILE_VBI    11   //

//��ʽ��׼ö�ٶ���
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
	ET_VIDEOSTANDARD_NTSC_2997_480		= 0x00400000,		//NTSC size:720*480  f/s : 29.97  //�����ţ���ֹ�Ժ���������ٸĳ�480
	ET_VIDEOSTANDARD_NTSC_30_480		= 0x00800000,		//NTSC size:720*480 f/s : 30 //�����ţ���ֹ�Ժ���������ٸĳ�480

	ET_VIDEOSTANDARD_1280_1080_50i      = 0x01000000,       //HD  size:1280*1080 f/s : 25 interlaced
	ET_VIDEOSTANDARD_1280_1080_5994i    = 0x02000000,       //HD  size:1280*1080 f/s : 29.97 interlaced
	ET_VIDEOSTANDARD_1280_1080_60i      = 0x04000000,       //HD  size:1280*1080 f/s : 30 interlaced

};


#define ET_OLEDATA_DRAGCLIP					_T("SobeyETClip")


//���ڴ�ETObject�ṹ��̳еĶ����жϣ�ʵ�ʵĽṹ������
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
	/* ȡ�ö������ID */														    \
	static long GetClassID() { return __ClassID; }						            \
	\
	/* ȡ�ö������ID */															\
	virtual long VirtualGetClassID() { return __ClassID; }							\
	\
	/* �������Ƿ���ĳ����������ͬ�� */										    \
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
	/* ȡ�ö������ID */														    \
	static long GetClassID() { return __ClassID; }						            \
	\
	/* ȡ�ö������ID */															\
	virtual long VirtualGetClassID() { return __ClassID; }							\
	\
	/* �������Ƿ���ĳ����������ͬ�� */										    \
	virtual BOOL IsKindOf( long lClassID )									        \
{																			        \
	return ( __ClassID == lClassID );										        \
}

//CheckInstallKey�ķ���ֵ
#define KEY_RIGHT					0	//Key��ȷ
#define KEY_RIGHT_RECORD_FAILED		1	//Key��ȷ����¼Keyʧ��
#define KEY_WRONG					2	//Key����
#define LOAD_VALIDATE_FAILED		3	//����Validate.dllʧ��
typedef int ( *pCheckInstallKey)(LPCTSTR strKey);

 
//OA_MATRIAL_SPECIAL_FOLDERID 
//#define OA_MATRIAL_FOLDERID			10000

//��ק��Ϣͷ�Ľ���
typedef struct stu_ETDragHeaderDef
{
	long						m_lVersion;			//Э��汾��
	ET_DRAG_OBJTYPEMASK_TYPE	m_lObjectType;		//��������
	ET_DRAG_SOURCE_TYPE			m_lSourceType;		//������Դ
	CString						m_strUserCode;		//�û�����
	CString						m_strIPAddress;		//IP��ַ
	long						m_lSiteID;			//վ��ID
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

//ͬ�����ݿ�ṹ��վ������
//������ط���վ����һ���򵥵Ĺ���涨��
//0��Ϊ�����ݿ����ȱʡ���ݿ�
//1��Ϊ�������ݿ��վ��ID
//�������������ݶ�������������ġ�
typedef struct stu_ETSiteDef
{
	long				m_lSiteID;			//վ��ID,Ҳ�������ݿ�ID��
	CString				m_strPluginDLLName;	//վ���Ӧ�Ĵ�����DLL,����PE��Ч
	CString				m_strSiteName;		//վ������
	CString				m_strSiteDesc;		//վ������
	long				m_lSerialNo;		//վ�������
	ETVML_PLUGIN_TYPE	m_lPlugInType;		//�������

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

//�û���Ϣ����ṹ��
typedef struct stu_ETUserInfo
{
	long		m_lUserID;		//�û�ID
	CString		m_strUserCode;	//�û�����
	CString		m_strUserName;	//�û�����
	long		m_lDeptID;		//����ID
	BOOL        m_bIsValid;     //�û��Ƿ���Ч
	BOOL        m_bCanUseSystem; //�û��ܷ�ʹ��ϵͳ
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

//������Ϣ����ṹ��
typedef struct stu_ETDeptInfo
{
	long		  m_lDeptID;		//����ID
	CString		  m_strDeptCode;	//���ű���
	CString		  m_strDeptName;	//��������
	CString		  m_strInputCode;	//����¼����
	long		  m_lSuperID;		//�ϼ�����ID
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

//Ƶ����Ϣ����ṹ��
typedef struct stu_ETChannelInfo
{
	long			  m_lChannelID;		//Ƶ��ID
	CString			  m_strChannelCode;	//Ƶ������
	CString			  m_strChannelName;	//Ƶ������
	ET_NM_OBJECT_TYPE m_emType;			//����
	long			  m_lSuperID;		//�ϼ�Ƶ��ID
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

//��Ŀ��Ϣ����ṹ��
typedef struct stu_ETColumnInfo
{
	long			  m_lColumnID;		//��ĿID
	CString			  m_strColumnCode;	//��Ŀ����
	CString			  m_strColumnName;	//��Ŀ����
	ET_NM_OBJECT_TYPE m_emType;			//����
	long			  m_lSuperID;		//�ϼ�ID
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

//�����Ϣ����ṹ��
typedef struct stu_ETEditionInfo
{
	long		  m_lEditionID;		//���ID
	CString		  m_strEditionCode;	//��α���
	CString		  m_strEditionName;	//�������
	long		  m_lTimeLength;	//����
	DATE		  m_dtBeginTime;	//��β���ʱ��
	long		  m_lColumnID;		//��ĿID
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
	ET_emMenuType_Separator = 1, //���ϲ˵�
	ET_emMenuType_Menu		= 2, //һ���˵�
	ET_emMenuType_Child		= 4  //�Ӳ˵�
};

//�˵�����ṹ��
typedef struct stu_ETMenuData
{
	DWORD			m_dwMenuID;		//�˵�������ID
	CString			m_strMenuName;	//�˵���ʾ������
	emETMenuType	m_emMenuType;	//�˵�����
	DWORD			m_dwPositionX;	//�˵����ڵڼ��У����Ϊ�Ӳ˵����˲�������
	DWORD			m_dwPositionY;	//�˵����ڸ��еĵڼ��У����Ϊ�Ӳ˵����˲�������
	stu_ETMenuData *m_pChildNode;	//�Ӳ˵�ָ��
	stu_ETMenuData *m_pNextNode;	//��һ���˵�ָ��

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

//ý���ʽ����ṹ��
typedef struct stu_ETMediaFormatDef
{
	long					m_lMiediaFmtID;			//��ʽID
	emMediaFormatDefType	m_emType;				//1����Ƶ��ʽ��2����Ƶ��ʽ��3������ʽ��4��ͼƬ��ʽ
	GUID					m_guidFileType;			//�ļ���ʽ��GUID
	long					m_lDataRate;			//����
	long					m_lFOURCC;				//ý������
	long					m_lMediaType;			//ý���ļ�����
	long					m_lSubMediaType;		//ý���ļ�������
	long					m_lAudioChannels;		//��Ƶͨ����
	long					m_lAudioSamplesRate;	//��Ƶ������
	long					m_lAudioBitsperSample;	//����λ��
	long					m_lVideoImgWidth;		//�زĻ����
	long					m_lVideoImgHeight;		//�زĻ����
	emETVideoStandard		m_emVideoStandard;		//��ʽ
	long					m_lVPixFmt;				//���ظ�ʽ
	long					m_lVBitsPerPixel;		//ÿ����λ��
	double					m_dblVFrameRate;		//֡��
	long					m_lVCompression;		//����ѹ����ʽ
	long					m_lVScanMode;			//ɨ��ģʽ
	long					m_lVGOPSize;			//GOP����
	long					m_lVBFrameCount;		//B֡����
	long					m_lVPFrameCount;		//P֡����
	__int64					m_lHashKey;				//��ʽHASHֵ
	//���캯��
	stu_ETMediaFormatDef()
	{
		Reset();
	}
	//��λ����
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
	//�ж��Ƿ����
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

	//��ֵ����
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

	//��ȡHASHֵ
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

		//����GUID
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

	//��������
	virtual ~stu_ETMediaFormatDef()
	{
		Reset();
	}
}ETMediaFormatDef,*lpETMediaFormatDef;
typedef CArray<lpETMediaFormatDef,lpETMediaFormatDef&> ETArrayMediaFmt;

//��Դ���ñ������ݽṹ��
typedef struct stu_ETResourceRefer
{
	GUID			m_guidRes;
	long			m_lDatabase;
	ET_RES_TYPE     m_eResType;
	long			m_lTrimIn;
	long			m_lTrimOut;
}ETResourceRefer,*lpETResourceRefer;
typedef CArray<lpETResourceRefer,lpETResourceRefer> ETArrayResourceRefer;

//one generation���ݱ�ṹ��
typedef struct stu_ETClipSource
{
	GUID				m_guidClipGuid;		//����GUID
	GUID				m_guidSrcClipGuid;	//Դ����GUID
	long                m_lSiteID;			//SITEID
	CString				m_strObjectName;	//��������
	CString				m_strObjectPath;	//·��
	long				m_lLength;			//����
	long				m_lTrimIn;			//���
	long				m_lTrimOut;			//����
	emETVideoStandard	m_emVideoStandard;	//��ʽ
	DATE				m_dtModifyDate;		//�޸�����
	long				m_lSrcGuidIsValid;   //Դ����GUID�Ƿ���Ч 1��Ч 0��Ч
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
	CString	m_strTaskGuid; //�ƻ���������GUID
	CString	m_strSrcClipGUID; //Դ�ز�GUID
	LONG	m_lSourceSiteID; //Դվ��ID
	CString m_strSourceSiteName; //Դվ����
	CString m_strSourceFolderPath; //Դ�ز�·��
	CString m_strMaterialName;//�ز�����
	CString m_strRights;
	CString m_strComment;
	LONG	m_lTargetSiteID; //Ŀ��վ��ID
	CString m_strTargetSiteName; //Ŀ��վ����
	CString m_strTargetFolderPath; //Ŀ���ز�·��
	ET_CLIP_QUALITY_TYPE m_lQuanlity;//����
	LONG	m_lPriority;			//���ȼ�
	BOOL	m_lType;			//����m_lType = 0������m_lType = 1
	LONG	m_lCreatorID;			//���񴴽���ID	
	LONG	m_lStatus;				//����״̬��0 �������� 1 ����ʧ��
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
	CString				m_strReserved5;		    //��������5
	CString				m_strReserved6;		    //��������6
	CString				m_strReserved7;		    //��������7
	CString				m_strReserved8;		    //��������8
	CString				m_strReserved9;		    //��������9
	CString				m_strReserved10;		//��������10
	CString				m_strReserved11;		//��������11
	CString				m_strReserved12;		//��������12
	CString				m_strReserved13;		//��������13
	CString				m_strReserved14;		//��������14 
	ET_CLIP_ARCHIVE		m_lXDAArchiveFlag;		//XDA�鵵��־
	ULONGLONG           m_ullObjDataMask;       //����
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


//ʵ������ṹ�壺��ET_OBJECT�е������ֶ�
typedef struct stu_ETObject
{
	long				m_lObjectID;		//����ID
	GUID				m_guidObjectGuid;	//����GUID
	emObjectType		m_emObjectType;	//��������
	__int64				m_llSubType;		//����������
	long				m_lFolderID;		//�ļ���ID
	long				m_lPopdomID;		//Ȩ��ID
	long				m_lCDID;			//����ID
	CString				m_strObjectName;	//��������
	CString				m_strKeyWord;		//�ؼ���
	long				m_lIconFrame;		//ͼ��֡
	CString				m_strIconFileName;	//ͼ���ļ�
	long				m_lLength;			//����
	long				m_lTrimIn;			//���
	long				m_lTrimOut;			//����
	long				m_lCreaterID;		//������ID
	DATE				m_dtCreateDate;		//��������
	long				m_lMenderID;		//�޸���ID	
	DATE				m_dtModifyDate;		//�޸�����
	DATE				m_dtAccessDate;		//��������
	long				m_lDelateFlag;		//ɾ����־
	long				m_lDelaterID;		//ɾ����ID
	DATE				m_dtDelateDate;		//ɾ������
	DATE				m_dtRecordDate;		//¼������
	DATE				m_dtShowDate;		//��������
	DATE				m_dtDistributeDate;	//��������
	long				m_lDistributerID;	//������
	DATE				m_dtArchiveDate;	//�鵵����
	long				m_lArchiverID;		//�鵵����
	ET_CLIP_ARCHIVE		m_lArchiveFlag;		//�鵵��־
	ET_CLIP_ARCHIVE		m_l3rdArchiveFlag;	//��3���鵵��־
	long				m_lColumnID;		//��ĿID
	emETVideoStandard	m_emVideoStandard;	//��ʽ
	int					m_iStatus;			//״̬
	CString				m_strUMID;			//�����UMID
	long				m_lUsedFlag;		//ʹ�ñ�־
	int					m_iLockFlag;		//������־
	long				m_lFileStatus;		//�ļ�״̬
	CString				m_strRights;		//��Ȩ��Ϣ
	CString				m_strJournalList;	//����
	CString				m_strMediaChannel;	//ý��ͨ��
	CString				m_strObjectNote;	//����ע
	CString				m_strReserved1;		//��������1
	CString				m_strReserved2;		//��������2
	CString				m_strReserved3;		//��������3
	CString				m_strReserved4;		//��������4
	ULONGLONG			m_ullObjDataMask;	//������Чλ����
	ET_CLIP_CAPTURESTATUS m_lCaptureStatus;	//�ɼ�״̬
	ET_CLIP_IMAGETYPE	m_lImageType;		//ͼ������;0��Ϊ�ز�ԭʼ������1��ͼ��Ϊ4:3�ķ�ʽ��2��ͼ��Ϊ16:9�ķ�ʽ(����S2��0��Ϊ�ز�ԭʼ������2Ϊ16:9SD, 4ΪHD��1��ʹ��)
	CString				m_strEditTerminal;	//�༭�ն�
	CString				m_strModifyTerminal;//�޸��ն�
	GUID				m_guidDeviceID;		//����GUID
	long				m_lDatabase;		//վ��ʵ��
	long				m_lMarkIn;			//������
	long				m_lMarkOut;			//��ǳ���
	long				m_lTobeArchiveState;//����鵵��־
	CString             m_strStorageState;  //2009/03/26 zxp:add Archive�ڵ��µ�Archive Status״̬
	long                m_lDBEStreamChannel;   //DBE��Ƶͨ�������زĵ���������
	long                m_lNTSCTCMode;      //ʱ����㷽ʽ��0 ���ز�ԭʼ��ʽ 1 �÷Ƕ�֡ģʽ
	CString             m_strMaterialID; //V4.6 Material ID
	long                m_lBKColor;   //���󱳾���ɫ
	CString             m_strReserved20;        //XDCAMCOMPILE����
	CString             m_strStoryID;           //STORYID
	GUID				m_guidResSource; //Դ�ز�GUID
	ETObjectEx			m_stObjectEx;    //ETOBJECT��չ�ֶ�

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
		m_strStorageState       = src.m_strStorageState;//2009/03/26 zxp:add Archive�ڵ��µ�Archive Status״̬
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
		m_strStorageState   = src.m_strStorageState;//2009/03/26 zxp:add Archive�ڵ��µ�Archive Status״̬
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
		m_strStorageState   = _T("");//2009/03/26 zxp:add Archive�ڵ��µ�Archive Status״̬
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

//�ļ��б�ṹ�壺��ET_FOLDER�е������ֶ�
typedef struct  stu_ETFolder:public ETObject
{
	GUID			m_guidTypeID;		//����GUID
	long			m_lVideoFormatID;	//����Ƶ��ʽID
	long			m_lAudioFormatID;	//����Ƶ��ʽID
	long			m_lLVideoFormatID;	//����Ƶ��ʽID
	long			m_lLAudioFormatID;	//����Ƶ��ʽID
	long			m_lRHVideoFormatID;	//�������Ƶ��ʽID
	long			m_lRHAudioFormatID;	//�������Ƶ��ʽID
	long			m_lRLVideoFormatID;	//�������Ƶ��ʽID
	long			m_lRLAudioFormatID;	//�������Ƶ��ʽID
	long			m_lUseOriginalTC;	//ʹ��ԭʼʱ��

	int				m_iAudioChannel;	//�����Ƶͨ����

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

//�ز��ļ��б�ṹ�壺��ET_CLIPFILE�е������ֶ�
typedef struct stu_ETClipFile
{
	long			m_lClipID;				//�ز�ID
	long			m_lQualityType;			//�ز�����
	long			m_lClipClass;			//�ز�����
	long			m_lClipIn;				//���ز����
	long			m_lClipOut;				//���زĳ���
	long			m_lFileIn;				//�ļ����
	long			m_lFileOut;				//�ļ�����
	long			m_lLength;				//�ļ�����
	long			m_lMediaChannel;		//ý��ͨ��
	__int64			m_lFileSize;			//�ļ���С
	CString			m_strFilePath;			//�ļ�·��
	long			m_lFormatID;			//ý���ʽID
	long			m_lFileStatus;			//�ļ�״̬
	long			m_lCalcSpaceFlag;		//�Ƿ����ռ�
	CString			m_strVerfiyCode;		//�ļ�Ч����
	DATE			m_dtCreateTime;			//�ļ�����ʱ��
	CString			m_strStoreProtocol;		//�ļ��洢Э��
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

//�زı�ǵ��ṹ�壺��ET_CLIPMARKPOINT�е������ֶ�
typedef struct stu_ETClipMarkPoint
{
	long			m_lClipID;			//�ز�ID
	long			m_lKeyFrame;		//���֡λ��
	long			m_lEndFrame;		//��ǽ�����
	long			m_lAbsKeyFrame;		//���֡λ��(����ʱ��)
	long			m_lAbsEndFrame;		//��ǽ�����(����ʱ��)
	int				m_iMarkType;		//�������
	long			m_lIconFrame;		//��ǵ�ͼ��֡
	CString			m_IconFileName;		//ͼ���ļ�
	int				m_iIconLevel;		//��ǲ��
	long			m_lMarkColor;		//��ǵ���ɫ
	CString			m_strNote;			//��ע
	CString         m_strMarkGuid;      //MARK��GUID    
    int 			m_iSelected;        //�Ƿ�ѡ�� 0:δѡ�� 1:ѡ��
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

//EM Comments��������ṹ��
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

//�ƻ���ϢԪ���ݱ�ṹ�壺��ET_PLANNINGMETADATA�е������ֶ�
typedef struct stu_ETPlanningMetaData
{
	long			m_lPlanningID;			//�ƻ�ID
	GUID			m_guidPlanningID;		//�ƻ�GUID
	CString			m_strPlanName;			//�ƻ���
	CString			m_strCreatorName;		//��������
	DATE			m_dtCreateDate;			//��������
	CString			m_strModifyName;		//�޸�����
	DATE			m_dtModifyDate;			//�޸�����
	long			m_lVersion;				//�汾
	CString			m_strPlace;				//�ص�
	DATE			m_dtPlanningDate;		//�ƻ�����
	CString			m_strDirector;			//����
	CString			m_strInputBy;			//¼��
	CString			m_strPhotoGrapher;		//��Ӱ
	CString			m_strReporter;			//����
	CString			m_strOther;				//������Ա
	CString			m_strEquipment;			//�豸
	CString			m_strContactInfo;		//��ϵ��Ϣ
	CString			m_strPlanningXml;		//�ƻ�XML
	CString			m_strUserXml;
	CString			m_strInputSource;		//�ƻ�����Դ
	DATE			m_dtStartDate;			//�ƻ���¼��ʼʱ��
	DATE			m_dtDurDate;			//�ƻ���¼����ʱ��
	ULONGLONG		m_ullDataMask;			//������Чλ����
	LONG			m_lSourceType;			//�ɼ�Դ����
	CString			m_strIngestExtendXml;         //�ɼ��ֶ�Planning��Ϣ
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
//�ز���¼����Ԫ���ݱ�ṹ�壺��ET_INGESTMETADATA�е������ֶ�
typedef struct stu_ETIngestMetaData
{
	long			m_lTaskID;				//����ID
	CString			m_strTaskName;			//��������
	DATE			m_dtStartDate;			//��¼��ʼʱ��
	DATE			m_dtEndDate;			//��¼����ʱ��
	DATE			m_dtDurDate;			//��¼����ʱ��
	int				m_iStatus;				//��¼״̬
	int				m_iProcess;				//��¼����

	CString         m_strSourceTapeID;      //Դ�Ŵ�ID
	CString         m_strBackupTapeID;      //���ݴŴ�ID
	CString			m_strUserName;			//��¼�û���
	CString			m_strTerminal;			//��¼�ն�
	//{{2009/04/22 zxp:add ���Ԫ����
    CString         m_strChannelName;       //��¼ͨ����
	long            m_lDeviceID;            //��¼�豸ID
	int             m_iAudioChannelCount;   //��Ƶͨ����
	//}}
	CString         m_strSourceTapeBarcode; //Դ�Ŵ�Barcode��Ϣ

	ULONGLONG		m_ullDataMask;			//������Чλ����

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
		//{{2009/04/22 zxp:add ���Ԫ����
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
		//{{2009/04/22 zxp:add ���Ԫ����
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

//�ز�Ԫ���ݱ�ṹ�壺��ET_CLIPINFO�е������ֶ�
typedef struct stu_ETClipInfo:public ETObject
{
	ET_CLIP_SOURCE_TYPE	m_lSignalSourceType;	//�ɼ��ź�����
	CString			m_strSignalSourceDesc;	//�ɼ��ź�����
	CString			m_strSignalSourceMark;	//�ɼ��źű�ǩ
	long			m_lVtrInPoint;			//�Ŵ����
	long			m_lVtrOutPoint;			//�Ŵ�����
	long			m_lVtrINext;			//�Ŵ���չ���
	long			m_lVtrOutNext;			//�Ŵ���չ����
	CString			m_strStorageCode;		//�洢����
	int				m_iKeepDays;			//�ز��ڷǱ���еı�������
	ULONGLONG		m_lClipSource;			//�زĵ������Դ
	long			m_lIngestTaskID;		//��¼����ID
	long			m_lPlanningID;			//�ƻ�ID
	CString			m_strTC;				//ʱ��
	int				m_iVersion;				//�汾
	CString			m_strClipLanguage;		//����
	LPBYTE			m_lpSerialData;			//TGA����
	long			m_lSerialSize;			//TGA����
	GUID			m_guidSrcPgmID;			//ԭ��ĿID
	CString			m_strItemName;			//
	CString			m_strCategory;			//����
	CString			m_strProgramname;		// 
	ETArrayClipFile								m_arClipFile;			//�ļ��б�
	ETArrayClipMarkPoint						m_arClipMarkPoint;		//�زı�ǵ�
	ULONGLONG		m_ullDataMask;			//������Чλ����
	long			m_lSrcSiteID;			//��Դվ��ID,Ϊ0��ʾ����վ�㡣
	long			m_lRestrictViewContent;	//���Ʋ쿴�ز�����
	CString         m_strOriginalMaterial;  //2009/04/21 zxp:add Դ�ļ���

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
		//�ɼ��ź�����
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SignalSourceType))
			m_lSignalSourceType = src.m_lSignalSourceType;

		//�ɼ��ź�����
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SignalSourceDesc))
			m_strSignalSourceDesc = src.m_strSignalSourceDesc;

		//�ɼ��źű�ǩ
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SignalSourceMark))
			m_strSignalSourceMark = src.m_strSignalSourceMark;

		//�Ŵ����
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_VtrInPoint))
			m_lVtrInPoint = src.m_lVtrInPoint;

		//�Ŵ�����
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_VtrOutPoint))
			m_lVtrOutPoint = src.m_lVtrOutPoint;

		//�Ŵ���չ���
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_VtrInNext))
			m_lVtrINext = src.m_lVtrINext;

		//�Ŵ���չ����
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_VtrOutNext))
			m_lVtrOutNext = src.m_lVtrOutNext;

		//�洢����
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_StorageCode))
			m_strStorageCode = src.m_strStorageCode;

		//�ز��ڷǱ���еı�������
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_KeepDays))
			m_iKeepDays = src.m_iKeepDays;

		//�زĵ������Դ
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_ClipSource))
			m_lClipSource = src.m_lClipSource;

		//��¼����ID
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_IngestTaskID))
			m_lIngestTaskID = src.m_lIngestTaskID;

		//�ƻ�ID
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_PlanningID))
			m_lPlanningID = src.m_lPlanningID;

		//ʱ��
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_TC))
			m_strTC = src.m_strTC;

		//�汾
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_Version))
			m_iVersion = src.m_iVersion;

		//����
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_ClipLanguage))
			m_strClipLanguage = src.m_strClipLanguage;

		//TGA����
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SerialData))
		{
			m_lpSerialData = src.m_lpSerialData;
		}

		//TGA����
		if (FIELD_ISSET(ullDataMask, CLP_FIELD_SerialSize))
			m_lSerialSize = src.m_lSerialSize;

		if (FIELD_ISSET(ullDataMask, CLP_FIELD_ItemName))
			m_strItemName = src.m_strItemName;

		if (FIELD_ISSET(ullDataMask, CLP_FIELD_ProgramName))
			m_strProgramname = src.m_strProgramname;

		if (FIELD_ISSET(ullDataMask, CLP_FIELD_Category))
			m_strCategory = src.m_strCategory;
		
		//2009/04/21 zxp:add Դ�ļ���
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
		m_strCategory           =src.m_strCategory;//����
		m_strProgramname        =src.m_strProgramname;		// 
		m_lSrcSiteID			=src.m_lSrcSiteID;
		m_lRestrictViewContent	=src.m_lRestrictViewContent;
		m_strOriginalMaterial   =src.m_strOriginalMaterial;//2009/04/21 zxp:add Դ�ļ���

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
		m_strCategory           =	_T("");//����
		m_strProgramname        =	_T("");		// 
		m_ullDataMask			= 0XFFFFFFFFFFFFFFFF;
		m_lSrcSiteID			= 0;
		m_lRestrictViewContent	= 0;
		m_strOriginalMaterial   = _T("");//2009/04/21 zxp:add Դ�ļ���

		
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

//��ĿԪ���ݽṹ�壺��ET_PGMMASTER�е������ֶ�
typedef struct stu_ETPgmMaster:public ETObject
{
	CString			m_strClipPath;			//�زĴ��·��
	int				m_iVideoTrack;			//��Ƶ����
	int				m_iAudioTrack;			//��Ƶ����
	int				m_iGraphTrack;			//��Ļ����
	int				m_iBackTrack;			//��������
	int				m_iAudioChannel;		//�����Ƶͨ����
	int				m_iTLBlock;				//�Ƿ�����ʱ����
	CString			m_strFilePath;			//�ļ�·��
	CString			m_strBackFilePath;		//�����ļ�·��
	CString			m_strUndoFilePath;		//UNDO����·��
	long			m_lVideoFormatID;		//��Ƶ��ʽID
	long			m_lAudioFormatID;		//��Ƶ��ʽID
	int				m_iSaveState;			//��Ŀ����״̬
	long			m_lEditTime;			//�༭�ۼ�ʱ��
	long			m_lDubUserID;			//��������ԱID
	long			m_lDubAnnouncerID;		//����ԱID
	int				m_iSECStatus;			//��Ŀ����״̬
	long			m_lSysVersion;			//�༭ϵͳ�汾
	long			m_lEditVersion;			//��Ŀ�޸ĵİ汾��
	ULONGLONG		m_ullDataMask;			//������Чλ����
	long			m_lForceRefreshFlag;	//ǿ��ˢ�±�־

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

//�Ǳ��ģ�������Ϣ��ṹ�壺��ET_TEMPLATEOBJECT�е������ֶ�
typedef struct stu_ETTemplateObject:public ETObject
{
	
	GUID			m_guidTypeGuid;		//ģ������GUID	
	CString			m_strFilePath;		//ģ�����õ��ļ���	
	long			m_lResource;		//ģ����Դ����
	LPBYTE			m_lpTempData;		//ģ������
	long			m_lTempSize;		//ģ�����ݴ�С
	ULONGLONG		m_ullDataMask;		//������Чλ����

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

//��������Ϣ�ṹ�壺��ET_CDINFO�е������ֶ�
typedef struct stu_ETCDInfo:public ETObject
{
	CString			m_strUpLoadIp;		//�������ص�ַ
	long			m_lDeviceID;		//�豸ID
	CString			m_strMainTitle;		//���̱���

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

//Ȩ�޶����
typedef struct stu_ETObjectPopedom
{
	long				m_lPopedomID;		// Ȩ��  ID
	long				m_lOwnObjectID;		// ӵ����ID
	emObjectType		m_emOwnObjectType;	// ET_ObjType_Folder
	ET_OBJECT_POPEDOM	m_emReadPopedom;	// ����Ȩ�� 0-����,1-˽��,2-����,3-����
	ET_OBJECT_POPEDOM	m_emModifyPopedom;	// �޸�Ȩ�� 0-����,1-˽��,2-����,3-����
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

//EDLԪ���ݱ�ṹ�壺��ET_EDLINFO�е������ֶ�
typedef struct stu_ETEDLInfo:public ETObject
{
	CString			m_strEDLFileName;		//EDL����
	CString			m_strVideoFormat;		//��Ƶ��ʽ
	int				m_iVideoTrack;			//��Ƶ����
	int				m_iAudioTrack;			//��Ƶ����
	int				m_iGraphTrack;			//��Ļ����

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

//����վ�����ṹ�壺��ET_DELETEOBJECT�е������ֶ�
typedef struct stu_ETDeleteObject
{
	long			m_lObjectID;		//����ID
	long			m_lUserID;			//�û�ID
	long			m_lSrcFolderID;		//ԭ�ļ���ID
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

//�ҵĳ��ö����ṹ�壺��ETMyCommonObject�е������ֶ�
typedef struct stu_ETMyCommonObject
{
	long			m_lObjectID;		//����ID
	long			m_lUserID;			//�û�ID
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

//��Ŀ��MPC��������ṹ�壺��ET_PGMRENDERDATA�е������ֶ�
typedef struct  stu_ETPgmRenderData
{
	long			m_lPgmID;			//��ĿID
	CString			m_strGroupType;		//������
	CString			m_strMediaType;		//ý������
	CString			m_strFilePath;		//�ļ�·��
	long			m_lTrimIn;			//���
	long			m_lTrimOut;			//����
	long			m_lLength;			//����
	__int64			m_llFileSize;		//�ļ���С
	long			m_lVideoMediaFormatID;	//��Ƶý���ʽID
	long			m_lAudioMediaFormatID;	//��Ƶý���ʽID


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
//��Ŀ���Զ���������ṹ�壺��ET_TECHCENSORDATA�е������ֶ�
typedef struct stu_ETTechCensorData
{
	long			m_lPgmID;			//��ĿID
	long 			m_lSerialID;		//���
	long			m_lCensorType;		//������������
	long			m_lBeginFrame;		//��ʼ֡
	long			m_lEndFrame;		//����֡
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
//��Ŀ�������̱�ṹ�壺��ET_PGMFLOW�е������ֶ�
typedef struct stu_ETPgmFlow
{
	long			m_lPgmID;			//��ĿID
	long			m_lSerialID;		//�������
	long			m_lUserID;			//�����û�ID
	DATE			m_dtOperDate;		//��������
	CString			m_strUserAction;	//��������
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

//��Ŀ��������ṹ�壺��ET_CENSORINFO�е������ֶ�
typedef struct stu_ETCensorInfo
{
	long			m_lPgmID;			//��ĿID
	long			m_lCensorType;		//�������
	long			m_lCensorUserID;	//����û�ID
	DATE			m_dtCensorDate;		//�������
	CString			m_strContent;		//����������
	CString			m_strAudioContent;	//��������������·��
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
//�洢�����ṹ�壺��ET_STORAGEDEF�е������ֶ�
typedef struct stu_ETStorageDef
{
	CString			m_strStorageCode;		//�洢����
	CString			m_strStorageName;		//�洢����
	CString			m_strStorageNote;		//�洢˵��
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
//�����豸��ṹ�壺��ET_DEVICEINFO�е������ֶ�
typedef struct stu_ETDeviceInfo
{
	long			m_lDeviceID;		//�豸ID
	CString			m_strDeviceName;	//�豸��	
	CString			m_strDeviceIP;		//�豸IP
	CString			m_strLoginName;		//��¼��
	CString			m_strPassword;		//��¼����
	GUID			m_guidDeviceID;		//�豸guid
	long			m_lAudioChannel;	//
	long			m_lDatabase;
	long			m_lDeviceType;		//�豸����

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
	ULONGLONG m_ullDataMask; //2009/02/27 zxp:add ������Чλ����

	// function
	stu_ETOrgMetaData()
		: m_lClipID(0),
		m_lDuration(0),
		m_lSTC(0),
		m_dtCreateDate(0.0f),
		m_ullDataMask(0XFFFFFFFFFFFFFFFF)//2009/02/27 zxp:add ������Чλ��ʼ��
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
		m_ullDataMask = src.m_ullDataMask;//2009/02/27 zxp:add ������Чλ
	}

} ETOrgMetaData, *lpETOrgMetaData;

typedef struct stu_ETNetMsgCallbackHeader
{
	long		m_lOperation;			// �������ͣ�1���½���2���޸ģ�3��ɾ��
	LONGLONG	m_llRootSubType;		// ���ڵ��SubType��
	__int64		m_llObjDataMask;		// ����ETObject�ṹ�����޸ĵ�������
	__int64		m_llSubDataMask;		// ����ETObject�����������޸ĵ�������
	long		m_lLoginID;
	long		m_lExtra;
	long		m_lDatabase;
}ETNetMsgCallbackHeader, *lpETNetMsgCallbackHeader;

typedef struct stu_ETEventMetaData 
{
	long			m_lClipID;			// �ز�ID
	CString			m_strEventID;		// EVENTID
	CString			m_strEventTitle;	// ����
	CString			m_strComments;		// ��ע
	long			m_lSOM;				// ���
	long			m_lEOM;				// ����
	long			m_lDuration;		// ����
	long			m_lChannel;			// ͨ��
	long			m_lVersion;			// �汾
	CString			m_strMOSID;			// MOS��ID
	DATE			m_dtLastUpdate;		// ����޸�����
	DATE			m_dtCreateDate;		// ����ʱ��
	CString			m_strUpdateUser;	// �޸���
	CString			m_strUpdateTerminal;// �޸��ն�
	DATE			m_dtPlayoutStart;	// ������ʼʱ��
	DATE			m_dtPlayoutEnd;		// ��������ʱ��
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
		m_lClipID			= event.m_lClipID;			// �ز�ID
		m_strEventID		= event.m_strEventID;		// EVENTID
		m_strEventTitle		= event.m_strEventTitle;	// ����
		m_strComments		= event.m_strComments;		// ��ע
		m_lSOM				= event.m_lSOM;				// ���
		m_lEOM				= event.m_lEOM;				// ����
		m_lDuration			= event.m_lDuration;		// ����
		m_lChannel			= event.m_lChannel;			// ͨ��
		m_lVersion			= event.m_lVersion;			// �汾
		m_strMOSID			= event.m_strMOSID;			// MOS��ID
		m_dtLastUpdate		= event.m_dtLastUpdate;		// ����޸�����
		m_dtCreateDate		= event.m_dtCreateDate;		// ����ʱ��
		m_strUpdateUser		= event.m_strUpdateUser;	// �޸���
		m_strUpdateTerminal = event.m_strUpdateTerminal;// �޸��ն�
		m_dtPlayoutStart	= event.m_dtPlayoutStart;	// ������ʼʱ��
		m_dtPlayoutEnd		= event.m_dtPlayoutEnd;		// ��������ʱ��
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

//�ز�Trim����㶨��
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

//! ���ݿ����ݲ�ѯ��������
enum em_ETDBQueryDataType
{
	ET_UnKnow = 0,
	ET_emNumber = 1,
	ET_emString = 2,
	ET_emDate = 3,
	ET_emCLOB = 4,
	//!�Զ�������
	ET_emCustomType = 5,
	//�����ִ�Сд
	ET_emAllString  = 6,
	//�ݹ��ѯ
	ET_emRecursive	= 7
};
/**
* @���ݿ����ݲ�ѯ�����ṹ�嶨��
*/
typedef struct stu_ETDBQueryCondition
{
	//!��ǰһ���������ӷ���������"AND"��"OR"��"("��"AND("��"OR("���߼����ӷ�
	CString m_strPrefix;
	//!��ѯ��֧�ֵ��ֶ���
	CString m_strKeyword;
	//!��ѯ������ֵ
	CString m_strValue;
	//!�ֶ������ֶ�ֵ֮����߼���ϵ��������">"��">="��"<"��"<="��"="��"<>"��"LIKE"��"IN"��"NOT IN"�ȹ�ϵ�ȽϷ�
	CString m_strRelation;
	//!��������
	em_ETDBQueryDataType m_emDataType;
	//!�������Ľ�β����������")"����
	CString m_strSuffix;
	//!�Ƿ��������վ
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
	* @param strPrefix ��ǰһ���������ӷ���������"AND"��"OR"��"("��"AND("��"OR("���߼����ӷ�
	* @param strKeyword ��ѯ��֧�ֵ��ֶ���
	* @param strValue ��ѯ������ֵ
	* @param strRelation �ֶ������ֶ�ֵ֮����߼���ϵ��������">"��">="��"<"��"<="��"="��"<>"��"LIKE"��"IN"��"NOT IN"�ȹ�ϵ�ȽϷ�
	* @param emDataType ��������
	* @param strSuffix �������Ľ�β����������")"����
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

	// ���������ļ���OBJECTID�ݹ��ѯ�����Ӷ����������add by djq 2007-7-26
	// strPrefix		��ǰһ���������ӷ�
	// strRelation		��ݹ��������߼���ϵ��ֻ���ǣ�IN��NOT IN
	// lValue			��Ҫ�ݹ���ļ��е�OBJECTID
	// strSuffix		�������Ľ�����
	// bIncludeTrash	�Ƿ��������վ�Ķ���
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

	//!���������
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
* @��ȡ�ļ�·������
*/
enum em_PathType
{
	ET_PathType_emHVideo	=	1,	//��������Ƶ
	ET_PathType_emHAudio	=	2,	//��������Ƶ
	ET_PathType_emLVideo	=	3,	//��������Ƶ
	ET_PathType_emLAudio	=	4,	//��������Ƶ
	ET_PathType_emPgm		=	5,	//��Ŀ·��
	ET_PathType_emEDL		=	6,	//EDL·��
	ET_PathType_emCD		=	7,	//CD·��
};

/************************************************************************/
/* ��Ե�����ý�ʵĻ�Ǩ�ӿ�����                                         */
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

//��ʷ����
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
	long				m_lSiteID;		//-1��ʾ�ǵ�����ý��
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
	ET_CMGW_TASK_TUPE_FROM_ARCHIVE,				//������ý�ʵ��زĻ�Ǩ
	ET_CMGW_TASK_TUPE_FROM_NLE,	
	ET_CMGW_TASK_TYPE_UNKNOW,
	ET_CMGW_TASK_TUPE_FROM_INTERFACE,           //ͨ��openinterface�ĵ���
}ET_CmGWTaskType;

//Ԫ����ӳ��
typedef enum
{
	ET_MAP_FIELD_PLANNING,
}ET_MAP_FieldOfMdType;

typedef enum
{
	ET_MAP_CONVER_NORMAL,			//����ת��
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
	CString					m_strMetadataEsc;	//Ԫ��������
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

//2010/4/22 niejiagang:Add �ز������ز������زĵĹ�ϵ
typedef struct stu_ETGroupItem
{
	GUID	guidRes;			//���ز�GUID
	GUID	guidResGroup;		//���ز�GUID
	int     iQualityIndex;      //��������
	INT64	lClipIn;			//���ز������ز��е���ʼλ�� 
	INT64	lFileIn;			//ʹ�õ����ز��еĿ�ʼλ�� 
	INT64	lFileOut;			//ʹ�õ����ز��еĽ���λ��
	INT64   lFileLen;           //ʹ�õ����ز��еĳ��� 
	BOOL    bAVInterleave;      //����Ƶͬһ�ļ�
	long	lVideoFormatID;		//���ز���Ƶ��ʽ
	long	lAudioFormatID;		//���ز���Ƶ��ʽ
	long    lKeyFormatID;       //���ز�Key��ʽ
	INT64	lClipClass;			//���ز����ڹ������Video, A1, A2...
	long    lReserve1;          //�����ֶ�
	long    lReserve2;          //�����ֶ�
	long    lReserve3;          //�����ֶ�

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

//DEB��Ƶ���
#define ET_CLIP_DBE_A1  0x00000001
#define ET_CLIP_DBE_A2  0x00000002
#define ET_CLIP_DBE_A3  0x00000004
#define ET_CLIP_DBE_A4  0x00000008
#define ET_CLIP_DBE_A5  0x00000010
#define ET_CLIP_DBE_A6  0x00000020
#define ET_CLIP_DBE_A7  0x00000040
#define ET_CLIP_DBE_A8  0x00000080

//�������ʱ������Ҫ��
typedef		ULONGLONG ET_SAVEOBJECT_SPECIAL_TYPE;	
#define ET_SAVEOBJECT_SPECIAL_NOTNOTIFYSERVER		0x00000001	//��֪ͨMLS���������ն˲����Զ�ˢ�£�
#define ET_SAVEOBJECT_SPECIAL_NOTCHECKSTANDARD		0x00000002	//���Զ������ļ��б������������زĵ���ʽ

typedef enum
{
	ET_CMGW_FileTilte_GUID         = 1,	//GUID
	ET_CMGW_FileTilte_ClipName     = 2,	//clip name
	ET_CMGW_FileTilte_MaterialID   = 3,	//Material ID
}ET_CmFileTitle;

//CommonGW��������
enum emTaskStatus
{
	ET_TaskStatus_Unknown	   = 0,
	ET_TaskStatus_Sending      = 1,
	ET_TaskStatus_Doing        = 2,
	ET_TaskStatus_FailtoSend   = 3,
	ET_TaskStatus_Failed       = 4,
	ET_TaskStatus_NotSupport   = 5,//����Ż��д�״̬
	ET_TaskStatus_Deleted      = 6,
	ET_TaskStatus_Success      = 7,
	ET_TaskStatus_Lauch_Task   = 8,//�����Ż��д�����״̬
	ET_TaskStatus_Export_Task  = 9,//�������д�״̬
	ET_TaskStatus_DeletedClip  = 10,//�������д�״̬
};

//2010/06/07 zxp:Add GTMAgent�ṹ
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
	CString                m_strOperationID;      // ������ID
	eu_OperationStatus     m_euOperationStatus;   // ��ϵͳ��GTM�Ļر������Ľ׶�
	CString                m_strSourceIPAddr;     // ���Ͷ�IP��ַ
	eu_SubSystem           m_euSubSystem;         // ��ϵͳ�ı�ʶ
	CString                m_strConnection;       // Agent��֪��ϵͳ��GTM����������״̬
	eu_CommondType         m_euCommandType;       // GTM����ϵͳ���ͷ��͵Ķ���ָ��
	eu_ParamNodeType       m_euParaNodeType;      // ��Ч�����Ľ����, ModuleTasks��TaskSteps��ѡһ
	ETArrayGTMModuleTasks  m_arModuleTasks;       // ModuleTasks���
	ETArrayGTMTaskSteps    m_arTaskSteps;         // TaskSteps���
	ETArrayGTMWorkFlows    m_arWorkFlows;         // WorkFlows���
	CString                m_strResult;           // ����ִ�н��
	CString                m_strInfo;             // ����ִ�е���Ϣ����ʧ��ԭ��
	ULONGLONG			   m_ullDataMask;	      //������Чλ����

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

//�����ṩ��NLE�Ľӿ���Ҫ��
typedef  std::pair<int/*event id*/, CString/*description*/> ITEM;
typedef  std::vector<ITEM >  ITEMS;
//}}

//�����ṩ��xml exchange�ӿ���Ҫ
typedef struct _tagTEMPLATEINFO
{
	long lTemplateID;
	CString strTemplateName;
}TEMPLATEINFO;

typedef std::vector<TEMPLATEINFO> TEMPLATEINFOS;

