#pragma once
#include <initguid.h>
#include "math.h"
#include "ETNetDeclaration.h"

#define __dbSmallerIndenty (0.00000001)
#define _DoubleEqualTest(a,b) (fabs((a)-(b))<=__dbSmallerIndenty)

//#define USE_FOR_CCTV

//���ļ���Ҫ��Ϊ����ϵͳ�ĺ궨���Լ��ڲ�ʹ�ÿ��ܿ�Խ��̬��Ľṹ�嶨��
//						�ⴺ��
//						2007��2��26��

#define	CLIP_ICON_WIDTH				160				// �ز�ͼ��Ŀ��
//defines
#define MPC_ST_RATE_25				50			// PAL field frequency
#define	MPC_ST_FRAME_RATE_25		25			// PAL frame  frequency
#define MPC_ST_SCALE_25				1			// PAL scale

#define MPC_ST_RATE_2997			60000		// NTSC field frequency
#define	MPC_ST_FRAME_RATE_2997		30000		// NTSC frame  frequency
#define MPC_ST_SCALE_2997			1001		// NTSC scale

#define MPC_ST_RATE_30				60			// 30-F field frequency
#define	MPC_ST_FRAME_RATE_30		30			// 30-F frame frequency
#define MPC_ST_SCALE_30				1			// 30-F scale

#define MPC_ST_RATE_24				48			// 24-F field frequency
#define	MPC_ST_FRAME_RATE_24		24			// 24-F field frequency
#define MPC_ST_SCALE_24			    1			// 24-F scale

#define MPC_ST_RATE_2398			48000		// 2398-F field frequency
#define	MPC_ST_FRAME_RATE_2398		24000		// 2398-F frame frequency
#define MPC_ST_SCALE_2398			1001        // 2398-F scale


#define MPC_FRAMES_SECOND_25			25L			// 25 Frame: frames per second    
#define MPC_FRAMES_MINUTE_25			1500L		// 25 Frame: frames per minute
#define MPC_FRAMES_HOUR_25				90000L		// 25 Frame: frames per hour

#define MPC_FRAMES_SECOND_24			24L			// 24 Frame: frames per second    
#define MPC_FRAMES_MINUTE_24			1440L		// 24 Frame: frames per minute
#define MPC_FRAMES_HOUR_24				86400L		// 24 Frame: frames per hour

#define MPC_FRAMES_SECOND_NODROP_30		30L			// 30 NO_DROP Frame: frames per second    
#define MPC_FRAMES_MINUTE_NODROP_30		1800L		// 30 NO_DROP Frame: frames per minute
#define MPC_FRAMES_HOUR_NODROP_30		108000L		// 30 NO_DROP Frame: frames per hour

#define MPC_FRAMES_MINUTE_30_DROP		1798L		// 30 DROP Frame: frames per minute
#define MPC_FRAMES_10MINUTES_30_DROP	17982L		// 30 DROP Frame: frames per 10 minutes
#define MPC_FRAMES_HOUR_30_DROP			107892L		// 30 DROP Frame: frames per hour

//Material List����ʾģʽ����
#define ET_EXPLORER_MODE_ML_UNKNOWN					-1
#define ET_EXPLORER_MODE_ML_LIST					1
#define ET_EXPLORER_MODE_ML_TREE					2
#define ET_EXPLORER_MODE_ML_METADATA				3
#define ET_EXPLORER_MODE_PM							4
#define ET_EXPLORER_MODE_ML_PE_TREE					5
#define ET_EXPLORER_MODE_ML_TREE_PUBLIC_ONLY		6
#define ET_EXPLORER_MODE_ML_PE_SELECTTREE			7

//ͳһ������Ϣ����ֵ��ĿǰS2ϵͳ��ʱ�涨Ϊ��WM_USER + 2001��WM_USER + 4000��
//						�ⴺ��
//						2007��2��26��

//����ؼ��ͽ�����˽����Ϣ
#define WM_ET_DRAGENTER										(WM_USER + 2010)
#define WM_ET_DRAGOVER										(WM_USER + 2011)
#define WM_ET_DRAGLEAVE										(WM_USER + 2012)
#define WM_ET_DROP											(WM_USER + 2013)
#define WM_USER_QUERY_UI_MODE								(WM_USER + 2014)  //ͨ����Ϣ��ѯģʽ

#define UM_SETMODE											(WM_USER + 2100)
#define UM_ML_CMD											(WM_USER + 2101)
#define UM_ML_MCM											(WM_USER + 2102)	//ModifyClipMetadata
#define UM_ML_SAVEMETADATA									(WM_USER + 2103)	//Save Metadata;
#define UM_ML_GETLOGICPATH									(WM_USER + 2104)	//	��ȡ�߼�·��
#define UM_ML_ADDTABPAGE									(WM_USER + 2105)	//Add one page
#define UM_ML_CLEARDATA										(WM_USER + 2106)	//��ս���
#define UM_ML_REFRESHRDATA									(WM_USER + 2107)	//ˢ�½���
#define UM_ML_SELECTPAGE									(WM_USER + 2108)	//ѡ��ҳ��	
#define UM_ML_GETLOGICPATHINFO								(WM_USER + 2109)	//��ȡ�߼�·����Ϣ(lpETObject)
#define UM_ML_SETSELFOLDERBYGUID							(WM_USER + 2110)	//���õ�ǰѡ�е��ļ���
#define UM_ML_SETESSENCEMARK								(WM_USER + 2111)	//���һ���زĵ�EssenceMark����
#define UM_ML_SETCALLER										(WM_USER + 2112)	//���õ�����
#define UM_ML_GETSYSPARAM									(WM_USER + 2113)	//��ȡϵͳ������Ϣ
#define UM_ML_SETREADONLY									(WM_USER + 2114)	//����ֻ������
#define UM_ICON_CREATED										(WM_USER + 2115)
#define WM_USER_UPDATE_TOOLBAR								(WM_USER + 2116)   //����Toolbar�ϵİ�ť״̬.xiashaochao, 2006.12.8
#define WM_USER_TREEITEM_SELECT_CHANGED						(WM_USER + 2117)   // ��Material List�е�Tree Itemѡ��仯ʱ, ���͸����ڵ�֪ͨ��Ϣ
#define UM_SETCLIPICON										(WM_USER + 2118)
#define UM_ENDEDIT											(WM_USER + 2119)
#define UM_CANCELEDIT										(WM_USER + 2120)
#define WM_REFRESH_MESSAGE									(WM_USER + 2121) 
#define	ET_NET_RECEIVE_MSG									(WM_USER + 2122)
#define WM_SET_STATUS										(WM_USER + 2123)
#define WM_USER_SET_METADATA                                (WM_USER + 2124)   //Ϊ������ǿ��ֱ������Ԫ������Ϣ
#define	UM_ML_SETPLANNINGDATA								(WM_USER + 2125)
#define UM_ML_DELTABPAGE									(WM_USER + 2126)	//ɾ��ҳ
#define UM_ML_GETCLIPINFOFROMPROPERTYDLG					(WM_USER + 2127)	//�����ԶԻ����л�ȡ�ز���Ϣ��ETClipInfoExt��
#define UM_ML_SETOPENPROJECT								(WM_USER + 2128)
#define WM_ML_SITEINIT_COMPLETE								(WM_USER + 2129)	//վ���ʼ�����
#define WM_ML_SEARCH_FINISH									(WM_USER + 2130)	//��ѯ���
#define WM_ML_REFRESHCTANDPROP                              (WM_USER + 3000)    //���������Ϣ�������У����ô���Ϣ��������ҳ��CT


// ������ҪΪArchive����������Ϣ
// Commented by xsc for V4.1
//��Archive��ص���Ϣ����
#define WM_USER_UPDATE_ARCHIVE_TREE							(WM_USER + 2130)   //����Ŀ¼��
#define WM_USER_UPDATE_ARCHIVE_CONTENT						(WM_USER + 2131)   //����Archive����
#define WM_USER_ARCHIVE_TASK_SENT							(WM_USER + 2132)   //�浵���������
#define WM_USER_RESTORE_TASK_SENT							(WM_USER + 2133)   //��Ǩ���������
#define WM_USER_ARCHIVE_TREE_REFRESH						(WM_USER + 2134)   //֪ͨ��������������Archive Tree
#define WM_USER_UPDATE_ARCHIVE_TRASHBIN						(WM_USER + 2135)   //֪ͨ������Archive����վ��ѯ���, Ҫ��ˢ��List
#define WM_USER_UPDATE_ARCHIVE_TRASHBIN_FAILED 				(WM_USER + 2136)   //����Archive����վʧ��. 2007.3.8 added
#define WM_USER_UPDATE_ARCHIVE_TREE_FAILED     				(WM_USER + 2137)   //����ArchiveĿ¼ʧ��
#define WM_USER_UPDATE_ARCHIVE_CONTENT_FAILED  				(WM_USER + 2138)   //�������ʧ��
#define WM_USER_ARCHIVE_TASK_SENTING						(WM_USER + 2139)   //���ڷ��͹鵵����
#define WM_USER_RESTORE_TASK_SENTING						(WM_USER + 2140)   //���ڷ��ͻ�Ǩ����
#define WM_USER_KILL_SUBWINDOW                              (WM_USER + 2141)   //���ٴ���
#define WM_USER_ARCHIVE_RESTORE_SETPOS						(WM_USER + 2143)   //ȡ���鵵/��Ǩ

// V4.1 �¶������Ϣ, by xsc.
//{
//#define WM_USER_UPDATE_TREEITEM                             (WM_USER + 2142)   // ֪ͨ������ˢ��ĳ��Ŀ¼�ڵ�
//#define WM_USER_UPDATE_CONTENT                              (WM_USER + 2143)   // ֪ͨ�����ڸ����б��еĶ���
#define WM_USER_UPDATE_UI_OBJECT							(WM_USER + 2142)
#define WM_USER_NOTIFY_SITECHANGED                          (WM_USER + 2144)   // վ���л�֪ͨ, wParam ��λΪ��һ��վ��ID, ��λΪ��վ��ID                                
#define WM_USER_UPDATE_PROPPAGE_COLLECTION                  (WM_USER + 2145)   // ֪ͨ��������ҳ
#define WM_USER_BEGIN_TRANSLATE                             (WM_USER + 2146)   // ֪ͨ����㿪ʼվ��վ��ת��
#define WM_USER_BEGIN_PASTE                                 (WM_USER + 2147)   // ֪ͨ����㿪ʼճ���ض�����
#define WM_USER_PROPERTYPAGE_MSGNOTIFY                      (WM_USER + 2148)   // ��֪ͨ��Ϣת��������ҳ
//}
#define WM_USER_OBJ_SPACE_GET_FINISH						(WM_USER + 2149)   // ��ȡ����ռ�ÿռ������Ϣ
 
// ֪ͨ�����ڳ�ʼ��CCC�ص�����
#define WM_SET_CCC_CALLBACK_FUN								(WM_USER + 2150)
#define WM_INIT_CCC_CALLBACK_FUN							(WM_USER + 2151)

#define UM_ML_SETSELFOLDERBYPATH							(WM_USER + 2152)	//���õ�ǰѡ�е��ļ���



//������Ϣ
#define WM_USER_UPDATE_MARKPOINT							(WM_USER + 2200)   //���±�ǵ����ʾ
#define WM_USER_MEDIA_PROCESSINFO           				(WM_USER + 2201)   //ý�崦��Ľ�����Ϣ
#define WM_USER_MEDIA_SEEKPROCESS           				(WM_USER + 2202)   //ý�崦���SEEK��Ϣ

#define WM_SETPROGRESSPOS									(WM_USER + 2203)
#define WM_SETPROGRESSMSG									(WM_USER + 2204)
#define WM_IDOK												(WM_USER + 2205)

#define UPDATE_TEXT_DATA									(WM_USER + 2206) //update child dialog data wParam��updatedata?true;false
#define SERVER_TEXT_MODIFY									(WM_USER + 2207) //Server data Modified wParam:�Ƿ�ΪServer Data�ı�
#define INVALIDATE_DBCOLOR									(WM_USER + 2208) 

#define WM_USER_GETMETADATACONTAINER                        (WM_USER + 2209) //ͨ������Ϣ��ѯ����ҳ�жԻ����е�Ԫ������������ָ��

#define WM_USER_ACTIVE                                      (WM_USER + 2210) //֪ͨ���ڼ��� 

//RichEdit�ؼ�����Ϣ����
#define ET_RICH_CHANGEINPUTMODE								(WM_USER + 2211)
#define ET_RICH_EDIT_RBUTTODWON								(WM_USER + 2212)
#define ET_RICH_EDIT_CHANGE									(WM_USER + 2213)
#define ET_RICH_EDIT_MSG									(WM_USER + 2214)
#define WM_DROP												(WM_USER + 2215)
#define WM_MODIFY_SERVER									(WM_USER + 2216)

//�༭�����Ϣ
#define ET_MULTIEDIT_CHANGE									(WM_USER + 2217)		//�༭��ı���Ϣ
#define ET_MULTIEDIT_KILLFOCUS								(WM_USER + 2218)		//��EditExʧȥ�����ʱ���򸸴��ڷ��͵�ʧȥ�������Ϣ
#define WM_EDIT_KEDOWN										(WM_USER + 2219)		//����ѹ���¼�
#define WM_EDIT_CHANGE										(WM_USER + 2220)		//�༭��ı���Ϣ
#define ET_TRACKEDIT_UPDATE									(WM_USER + 2221)		//TrackEdit���ݷ����޸���Ϣ
#define WM_CHECKBOX_CHANGE									(WM_USER + 2222)		//��ѡ��ı���Ϣ
#define WM_EDIT_KILLFOCUS                   				(WM_USER + 2223)        //�Զ���ʧȥ����

#define UM_OPENSELECTEDPROGRAM								(WM_USER + 2240)
#define	WM_ET_CLASSCASE_CHANGEBAR							(WM_USER + 2241)

//��Ա��
#define WM_CHECK_DEAL										(WM_USER + 2250)
#define WM_TREE_CHECK_CHANGE								(WM_USER + 2251)
#define WM_TREE_GET_FINISH									(WM_USER + 2252)
//��Ա���ĺ�̨�߳��¼�
#define	 WM_CTRL_ADD_PERSON									(WM_USER + 2253)
#define	 WM_CTRL_ADD_DEPT									(WM_USER + 2254)
#define	 WM_CTRL_DELETE_DEPTWITHOUTUSER						(WM_USER + 2255)
#define	 WM_CTRL_EXPANDDEPT									(WM_USER + 2256)
#define	 WM_CTRL_CHECKITEM									(WM_USER + 2257)
#define	 WM_EXPANDUSERDEPT									(WM_USER + 2258)
#define	 WM_CTRL_CLICKTREE									(WM_USER + 2259)

//MXF����Ϣ����
#define WM_TRAYNOTIFY										(WM_USER + 2270)
#define	WM_SET_INFO											(WM_USER + 2271)

//����SOCKET����Ϣ��SERVERʹ�õ���Ϣ
#define	WM_USER_IOCPSERVERMSG								(WM_USER + 2300)
#define WM_USER_UDPMSG										(WM_USER + 2301)
#define WM_USER_OBJDATAMGRMSG								(WM_USER + 2302)
#define WM_USER_SERVERCLOSE									(WM_USER + 2303)
#define WM_USER_UPDATEACTIVE								(WM_USER + 2304)
#define WM_USER_UPDATELOG									(WM_USER + 2305)
//ML Server�����̸߳����淢����Ϣ
#define WM_USER_THREAD_MSG									(WM_USER + 2306)
#define WM_ETPUB_CNTMENU_CLIPPEWVIEW_OG						(WM_USER + 2307)	// preview clip message for one generation dialog box


//֪ͨMLtreeѡ����Ŀ
#define WM_MLTREE_SELECTITEM								(WM_USER + 2350)		
#define WM_USER_BEGIN_COPYTOPE								(WM_USER + 2352)

// ��ȡ�ڲ����������ָ��
#define WM_USER_GETSELFDRAGITEMREF                          (WM_USER + 2353)

//��ȡ��ǰCETObjExplorerDlg����ָ��
#define WM_USER_FINDCLIP									(WM_USER + 2354)
#define WM_USER_CREATETHUMBNAILPICTURE						(WM_USER + 2355)

//�����û���Ϣ
#define WM_USER_EXCHANGEUSERIFNO                            (WM_USER + 2356)
#define WM_USER_MESSAGE										(WM_USER + 2357)

//�ؼ����ڹر���Ϣ
#define WM_USER_CLOSEWINDOW									(WM_USER + 2358)


//�ӿ�֪ͨ����LOGING��Ϣ
#define WM_USER_LOGGINGFROMCT								(WM_USER + 2362)

//����زĵ�RetrieveList
#define WM_USER_ADDTORETRIEVELIST                           (WM_USER + 2359)
//����RetrieveList������Ϣ
#define WM_USER_UPDATEORIGINALRETRIEVETASK                  (WM_USER + 2360)
//����CT Overwrite ��RetrieveList/ArchiveList������Ϣ
#define WM_USER_CTOVERWRITEMESSAGE                           (WM_USER + 2361)

//������ҳ��Ϣ
#define WM_USER_OPENPROPERTIES								(WM_USER + 2366)


//��������ҳ��Ϣ
#define WM_USER_CHANGEPROPERTIES							(WM_USER + 2367)

//ʱ��༭����Ϣ
#define WM_CODE_CHANGED										(WM_USER + 2400)

//�û��Զ���ʱ�������Ϣ
#define WM_USER_CUSTOM_FILTER								(WM_USER + 2500)

//��ȡ���ָ��Ķ���
#define WM_GET_FOLDER_FROM_PT								(WM_USER + 2600)

#define UM_ML_EDITOBJECT_REQUEST							(WM_USER + 2700)

//�ؼ����ڴ�planlist��Ϣ
#define WM_USER_OPENPLANLIST								(WM_USER + 2701)

#define WM_USER_ADDTOEXPORTLIST                             (WM_USER + 2702)

#define WM_PLAY_CALLBACK									(WM_USER + 2703)


//�Ǳ�ص�����
#define OM_OPEN_PROGRAM				0x0001
#define OM_CREATE_PROGRAM			0x0002
#define OM_CLOSE_ALL_PGM			0x0003//�ر����н�Ŀ,����S_FALSE�����û������˹ر��ļ�,����S_OK
#define OM_OPEN_CLIP_TO_EDITVIEW	0x0004
#define OM_CREATE_CLIP				0x0005
#define OM_GET_STANDARD_LIST		0x0006//ͨ���ص�6����������CStringArray���������Եõ���ʽ����Ƶ��ʽ��string��Ϣ
#define OM_GET_STANDARD_INFO		0x0007//Ȼ�����ͨ���ص�7������intָ���ѡ���index,����ȡ����ʽ��Ϣ�������Ϣ�ǿ���ֱ������
#define OM_GET_AUDIO_INFO			0x0008//ͨ���ص�8������ETMedia�Ǹ��ṹָ���ѡ���index������ȡ����Ƶ��Ϣ(��Ƶ��Ϣ��ֻ��audiochannels,audiosamplesrate,audiobitspersample���ã������Ķ���Ч)
#define OM_SAVE_PROGRAM				0x0009//wparamΪ��Ҫ����Ľ�Ŀguid
#define OM_GET_RETRIEVE_CLIP		0x0010//wparamΪstuNeedRetrieveClip��ָ��
#define OM_RENAME_PGM				0x000A//wparamΪ��Ŀ��guidָ�룬lparamΪ�µ�����( CStringָ�� )
#define OM_QUAERE_REFRESH_TIMELINE	0x000B//ѯ�ʷǱ��費��Ҫˢ��ʱ���ߣ�wParamΪ�ز�GUID
#define OM_REFRESH_TIMELINE			0x000C//ˢ��ʱ���ߣ�wParamΪETArrayClipInfoָ�루��Ҫ���µ��زģ���lParamΪCArray<GUID, GUID&>ָ�루��ˢ�µĽ�ĿGUID��
#define OM_GEALLUSEOBJ_FROM_NLE		0x001B//�õ��Ǳ���������ʹ�õĶ���wparamΪCArray<long, long>ָ��
#define OM_SET_CURRENTPROJECT		0x001E//wparamΪ�������ַ���ָ��
#define OM_GET_PGMVALIDLENGTH		0x001D//��ȡ��Ŀ����Ч����,wparamΪstuPgmָ��
#define OM_IMPORTEDL				0x001F//����EDL���޲���
#define OM_EXPORTEDL				0x0020//����EDL��wParamΪ��Ŀguidָ��(GUID *)
#define OM_MODIFYPGMFILEGUID		0x0021//�޸Ľ�Ŀ�ļ���GUID��wParamΪstuModifyPgm��ָ��
#define OM_GETICON					0x0022//��ȡICON buffer�� wParamΪstuGetIconParam��ָ��
#define OM_DROPINGFORPE				0x0024//wParamΪstuMLDropingParam��ָ��
#define OM_DROPFORPE				0x0025//wParamΪstuMLDropParam��ָ��
#define OM_INCLUDECG				0x0026//��Ŀ���Ƿ������CG��wParamΪstuIncludeCG��ָ��
#define OM_RENDERUSEDEF				0x0027//�����Ƿ�����ʱ�������ã��Ƿ���Ҫ��ѡ��Ի���wParamΪstuIsPopSelForRenderDef��ָ��
#define OM_RENDER_SENDTOSTUDIO		0x0028//wParam stuRenderMsgboxSetting ָ��
#define OM_RENDER_COMPILECG			0x0029//wParam stuRenderMsgboxSetting ָ��
#define OM_RENDER_NEEDRETRIEVE		0x0030//wParam stuRenderMsgboxSetting ָ��
#define OM_RENDER_OVERWRITE			0x0031//wParam stuRenderMsgboxSetting ָ��
#define E7_CB_ML_GETMETADATA        0x0032//2009/04/27 zxp:add ��ȡ��Ŀ�ϳ�ʱ��mark���FTC,wparamΪstuMetaData��ָ��
#define E7_CB_ML_SETVAOUT        0x0034//wParam ETObjectArray;��ǰѡ�еĽ�Ŀ���زĵ�ETObject����
#define OM_RENDER_PROXY_CONFIRM		0x003C//wParam stuRenderMsgboxSetting ָ��
#define OM_RENDER_OFFLINE_CONFIRM	0x003D//wParam stuRenderMsgboxSetting ָ��
#define OM_OPEN_SEQUENCE_LINK		0x0033//wParamΪstuSequenceLink �´�һ��PE��Ȼ��λ���ý�Ŀ
#define OM_RENDER_BINDSEQANDEVENT	0x0035//v4.6 register wnd ֪ͨNLE EVENT������Ϣ
//#define OM_RENDER_RESET_TIMELINE	0x003E//wParam stuRenderMsgboxSetting ָ��



// Archive��Ȩ��
#define POPEDOM_ARCHIVE_CREATE_FOLDER		 _T("ACCreateFolder")
#define POPEDOM_ARCHIVE_RENAME_FOLDER		 _T("ACRenameFolder")
#define POPEDOM_ARCHIVE_DELETE_FOLDER		 _T("ACDeleteFolder")
#define POPEDOM_ARCHIVE_CUT_FOLDER           _T("ACCutFolder")
#define POPEDOM_ARCHIVE_COPY_FOLDER          _T("ACCopyFolder")

#define POPEDOM_ARCHIVE_RETRIEVE             _T("ACRetrieveMaterial")
#define POPEDOM_ARCHIVE_CUT_MATERIAL         _T("ACCutMaterial")
#define POPEDOM_ARCHIVE_COPY_MATERIAL        _T("ACCopyMaterial")
#define POPEDOM_ARCHIVE_MODIFY_MATERIAL      _T("ACModifyMaterial")
#define POPEDOM_ARCHIVE_DELETE_MATERIAL      _T("ACDeleteMaterial")
#define POPEDOM_ARCHIVE_PRIVIEW              _T("ACPreviewMaterial")
#define POPEDOM_ARCHIVE_RENAME_MATERIAL      _T("ACRenameMaterial")

#define POPEDOM_ARCHIVE_DELETE_MATERIAL_FROM_TRASH _T("ACDelMatFromTrash")
#define POPEDOM_ARCHIVE_DELETE_FOLDER_FROM_TRASH   _T("ACDelFolderFromTrash")
#define POPEDOM_ARCHIVE_RESTORE_MATERIAL_FROM_TRASH  _T("AcRstMatFromTrash")
#define POPEDOM_ARCHIVE_RESTORE_FOLDER_FROM_TRASH  _T("AcRstFolderFromTrash")

#define POPEDOM_ARCHIVE_ACCESS               _T("ACArchiveAccess")
#define POPEDOM_ARCHIVE_CLIPS                _T("ETArchiveClips")

// Զ��վ���Ȩ��
#define POPEDOM_REMOTESITE_RETRIEVE_FROM_REMOTEARCHIVE _T("RS_RetrieveFromRemoteArchive")
#define POPEDOM_REMOTESITE_DOWNLOAD_FROM_REMOTENETWORK _T("RS_DownloadFromRemoteNetwork")
#define POPEDOM_REMOTESITE_UPLOAD_TO_REMOTENETWORK     _T("RS_UploadToRemoteNetwork")

//�����Ϣ����
#define ET_MIB_EDIT_SOFTWARETYPE	TEXT("Sobey.software.edit.info.General.softwaretype")	//�ַ���
#define ET_MIB_EDIT_SOFTWAREVERSION	TEXT("Sobey.software.edit.info.General.softwareVersion")//�ַ���


#define HUE_BACKGROUND		RGB(125,125,125)
#define HUE_WINDOW			RGB(125,125,125)
#define HUE_HILIGHT			RGB(104,104,104)
#define HUE_SHADOW			RGB(59,59,59)
#define HUE_DARKSHADOW		RGB(33,33,33)
#define HUE_3DFACE			RGB(125,125,125)
#define HUE_TEXT			RGB(120, 120, 120)
#define HUE_HILIGHTTEXT		RGB(180, 180, 180)
#define HUE_SELBORDER		RGB(165, 195, 225)
#define HUE_CONTROL			RGB(87,87,87)
#define HUE_MENUBAR			RGB(169, 169, 169)

typedef  long ETVML_SETOBJFLAG_TYPE;
#define ETVML_SETOBJFLAG_READONLY		0x01			//ֻ�����
#define ETVML_SETOBJFLAG_ARCHIVE		0x02			//�鵵���
#define ETVML_SETOBJFLAG_DELETE			0x04			//ɾ�����
#define ETVML_SETOBJFLAG_STATUS			0x08			//״̬���
#define ETVML_SETOBJFLAG_TOBESTATUS		0x10			//�鵵��Tobe״̬���

// �˶ζ����ETNETUI�������Ƴ�, commented by xsc for V4.1, 2007.6.16
// ��󳤶�����, added by xsc 2007.2.27
#define ET_MAX_TITLE_LENGTH				50
#define ET_MAX_COMMENT_LENGTH           1000
#define ET_MAX_FILEPATH_LENGTH          256
#define ET_MAX_ADDRESS_LENGTH           256
#define ET_MAX_COPYRIGHTS_LENGTH        256
#define ET_MAX_MATERIALID_LENGTH        24

// ����ҳ�Ķ���, ��Դ��ETPropertyPageDef.h
// added by xsc, 2007.6.26
//{
// ����ҳ����
#define ET_PROPPAGE_TYPE_UNKNOWN                    0x0000      //δ֪�Ĳ���֧�ֵ�ҳ����ʾ
#define ET_PROPPAGE_FOLDER							0x0001      //Ŀ¼ҳ
//#define ET_PROPPAGE_OBJECT_BASE						0x0002      //��������ҳ
#define ET_PROPPAGE_OBJECT_MULTILIST                0x0002
#define ET_PROPPAGE_OBJECT_CLIP						0x0004      //�ز���Ϣ
#define ET_PROPPAGE_OBJECT_PLANNING					0x0008      //Planning����
#define ET_PROPPAGE_OBJECT_FILLING					0x0010      //��¼
#define ET_PROPPAGE_OBJECT_ENSSENCEMARK				0x0020      //��ǵ�
#define ET_PROPPAGE_OBJECT_HISTORY					0x0040      //��ʷ��¼
#define ET_PROPPAGE_OBJECT_ORIGNAL					0x0080		//ԭʼ��Ϣ
#define ET_PROPPAGE_OBJECT_POSTIL					0x0100		//Postil��Ϣ
#define ET_PROPPAGE_OBJECT_RIGHTS					0x0200      //
#define ET_PROPPAGE_OBJECT_ORGMETADATA				0x0400		//
#define ET_PROPPAGE_OBJECT_ORGUMID					0x0800		//
#define ET_PROPPAGE_OBJECT_OTHER					0x1000		//
#define ET_PROPPAGE_OBJECT_FILELIST					0x2000		//
#define ET_PROPPAGE_OBJECT_CUSTOM					0x8000		// һ������ҳ����, ��������������ҳ������Զ��������. ��������Զ����ҳ��, ����ͨ�� AddCustomPage�ķ������
#define ET_PROPPAGE_OBJECT_SRCMATERIAL				0x10000		//one generation of material metadata
#define ET_PROPPAGE_OBJECT_SITE                     0x20000     //վ����ڵ�����ҳ
#define ET_PRPPPAGE_OBJECT_POSTILEX                 0x40000     //��չPostil��Ϣ

//{{2008/1/7 niejiagang:�Ǳ�Ҫ��
#define ET_PICTURE_LENGTH_DEF		2160000//1080000			//ͼƬ������ļ�����
#define ET_PICTURE_LENGTH_CLIP		1//100				//ͼƬ������زĳ���
//}}
// ����ҳ����
typedef long ET_PROPPAGE_TYPE;

// ����Ŀ¼������ҳ����
#define ET_PROPPAGE_TYPE_FOLDER       {ET_PROPPAGE_FOLDER,ET_PROPPAGE_OBJECT_RIGHTS}

// �����زĵ�����ҳ����
#define ET_PROPPAGE_TYPE_CLIP          {ET_PROPPAGE_OBJECT_CLIP,ET_PROPPAGE_OBJECT_PLANNING, \
										ET_PROPPAGE_OBJECT_FILLING,ET_PROPPAGE_OBJECT_ENSSENCEMARK,								\
										ET_PROPPAGE_OBJECT_HISTORY,ET_PROPPAGE_OBJECT_ORIGNAL,									\
										ET_PROPPAGE_OBJECT_POSTIL, ET_PRPPPAGE_OBJECT_POSTILEX,ET_PROPPAGE_OBJECT_RIGHTS,		\
										ET_PROPPAGE_OBJECT_ORGMETADATA, ET_PROPPAGE_OBJECT_ORGUMID,								\
										ET_PROPPAGE_OBJECT_OTHER, ET_PROPPAGE_OBJECT_FILELIST, \
										ET_PROPPAGE_OBJECT_SRCMATERIAL}

#define ET_PROPPAGE_TYPE_PGM          {ET_PROPPAGE_OBJECT_CLIP, ET_PROPPAGE_OBJECT_RIGHTS}

// ����Project������ҳ����
#define ET_PROPPAGE_TYPE_MULTI        {ET_PROPPAGE_OBJECT_MULTILIST, ET_PROPPAGE_OBJECT_RIGHTS}

// ����վ�������ҳ����
#define ET_PROPPAGE_TYPE_SITE         {ET_PROPPAGE_OBJECT_SITE}




// ����ҳģʽ
#define  PROPERTIES_UNKNOWN 0
#define  PROPERTIES_ARCHIVE 1
#define  PROPERTIES_PUBLIC  2
#define  PROPERTIES_INGEST  3
#define  PROPERTIES_SITE    4
//}


#define REGWINDOW_DDTOEVENT		0
#define REGWINDOW_RENDERTOML	1
#define REGWINDOW_DDTOOA		2
#define REGWINDOW_MVMENU		3
#define REGWINDOW_CT            4


#define NDF_MPC_VALUE           0

//!���ݿ����Ͷ���
enum em_ETDatabaseType
{
	ET_emSQLSERVER = 1,
	ET_emORACLE = 2,
	ET_emSYBASE = 3,
	ET_emDB2 = 4,
	ET_emAccess = 5
};

enum ET_emSiteItem_Status
{
	ET_SiteItemStatus_Uninited,  // δ��ʼ��  
	ET_SiteItemStatus_Initing,   // ���ڳ�ʼ��
	ET_SiteItemStatus_Inited,    // ��ʼ���ɹ�
	ET_SiteItemStatus_Failed     // ��ʼ��ʧ��
};


DEFINE_GUID(GUID_FILE_GENERAL_AVI, 0x7ae4febe, 0x7b95, 0x45ed, 0x8f, 0x64, 0x4f, 0xbf, 0x66, 0xd0, 0x7f, 0x34);
DEFINE_GUID(GUID_FILE_RIFF_AVI, 0x3256143e, 0x17ae, 0x418d, 0xb6, 0xc1, 0x54, 0x25, 0x2e, 0x5c, 0x91, 0x4b);
DEFINE_GUID(GUID_FILE_OPDML_AVI, 0xc75307f7, 0x40ad, 0x4bb4, 0xb2, 0x32, 0x8d, 0xc8, 0x6f, 0x2b, 0xf8, 0x2b);
DEFINE_GUID(GUID_FILE_MATROX_AVI, 0x4da4b1cb, 0xa1f8, 0x4c12, 0x81, 0x3b, 0xf6, 0x31, 0x81, 0xf5, 0xd3, 0x4d);
DEFINE_GUID(GUID_FILE_MATROX_AVI_HD,0xffaac63, 0x9d3, 0x48d9, 0xa8, 0x29, 0xab, 0x1d, 0xbf, 0x61, 0x61, 0xa5);

DEFINE_GUID(GUID_FILE_WAV, 0x8002619d, 0x461b, 0x44e3, 0x8e, 0x4, 0x11, 0x52, 0xbe, 0x41, 0xc1, 0xed);
DEFINE_GUID(GUID_FILE_MP3, 0x7312827b, 0xd9a, 0x4851, 0x96, 0xee, 0xba, 0xd4, 0x15, 0xfb, 0xcc, 0xf3);
DEFINE_GUID(GUID_FILE_TGA, 0x4440191b, 0x7619, 0x4a4e, 0xaa, 0x7d, 0x49, 0x1a, 0xc8, 0xf0, 0xfc, 0xfa);
DEFINE_GUID(GUID_FILE_BMP, 0xe6cd8cb7, 0x31a9, 0x4253, 0xbf, 0x51, 0xb4, 0x68, 0xfe, 0xbc, 0xfd, 0xbb);
DEFINE_GUID(GUID_FILE_JPG, 0x4c44c70d, 0x3d7e, 0x4312, 0x8b, 0xb6, 0xaf, 0x1a, 0xad, 0x3d, 0x94, 0x2b);
DEFINE_GUID(GUID_FILE_YUV, 0x7e554ee7, 0x84e9, 0x487e, 0xa4, 0xf, 0x65, 0x55, 0x87, 0x8e, 0xb7, 0x6e);
DEFINE_GUID(GUID_FILE_PKEY,0xe18d47b3, 0x7852, 0x4245, 0x81, 0x6, 0x61, 0xa5, 0xdf, 0x16, 0xbf, 0x9); 
DEFINE_GUID(GUID_FILE_TIF, 0x228d4f93, 0x21db, 0x439c, 0xa0, 0xa9, 0x6c, 0x51, 0xc2, 0x63, 0x85, 0xb);
DEFINE_GUID(GUID_FILE_GIF, 0xc1b53415, 0xa961, 0x4df0, 0xa8, 0x3c, 0xf7, 0x73, 0xe, 0xd8, 0xf2, 0xeb);
DEFINE_GUID(GUID_FILE_PCX, 0x92a3d675, 0x5011, 0x487d, 0xa0, 0x38, 0x17, 0x55, 0xa9, 0x6c, 0xdb, 0x26);
DEFINE_GUID(GUID_FILE_TIMELINE, 0x628a0a5, 0x9c0f, 0x43dd,  0x89, 0x1d, 0xed, 0x7c, 0x80, 0x16, 0xe8, 0x2e );

DEFINE_GUID(GUID_FILE_AVMIXED_AVI2,0x2ed036fa, 0xcc48, 0x4263, 0x9d, 0x56, 0xbd, 0xe5, 0x74, 0x4a, 0x5a, 0x40);
DEFINE_GUID(GUID_FILE_DV_DIFF,    0x19b0d82b, 0x170, 0x4b28, 0x93, 0x2, 0x59, 0x21, 0xd2, 0xb, 0xa1, 0x6a);
DEFINE_GUID(GUID_FILE_MPEG,	      0x30605386, 0xcac4, 0x4116, 0x80, 0x54, 0x93, 0xcb, 0xe8, 0xa2, 0x12, 0x71);
DEFINE_GUID(GUID_FILE_MPEG_ES,	  0x23b1fb57, 0x5dd6, 0x4f58, 0x96, 0x25, 0x50, 0x61, 0x9f, 0xb4, 0xeb, 0x8f);
DEFINE_GUID(GUID_FILE_MPEG_PS,    0x26a33c12, 0x78de, 0x472b, 0x93, 0x1f, 0x1d, 0x3a, 0x9e, 0x45, 0x49, 0x8f);
DEFINE_GUID(GUID_FILE_MPEG_TS,    0xc53ff94c, 0x1c72, 0x4462, 0x87, 0xd7, 0x7d, 0x65, 0xa5, 0x35, 0x31, 0xa9);
DEFINE_GUID(GUID_FILE_MPEG_DVD,   0x22799e6d, 0xae80, 0x4c44, 0xb3, 0x49, 0x12, 0x4c, 0x9f, 0x7b, 0xac, 0x3b);
DEFINE_GUID(GUID_FILE_MPEG_VCD,   0xddea1ddd, 0xfd25, 0x4243, 0x8f, 0x44, 0xb1, 0xb4, 0x97, 0xd4, 0xd, 0x7f);
DEFINE_GUID(GUID_FILE_MPEG_SVCD,  0x59e4629, 0x6c63, 0x400c, 0x85, 0xd6, 0x79, 0xac, 0x62, 0xae, 0x33, 0x96);
DEFINE_GUID(GUID_FILE_MPEG_MP4,   0x2c0ea791, 0x9ab3, 0x4a47, 0xa9, 0x92, 0x14, 0x0, 0x3d, 0x7, 0x4a, 0xac);
DEFINE_GUID(GUID_FILE_MS_WMV, 	  0x45150931, 0x9cb0, 0x408e, 0xaa, 0x47, 0x15, 0xcf, 0xfa, 0x17, 0xe4, 0xaa);
DEFINE_GUID(GUID_FILE_MS_ASF,     0x7c52ed46, 0x6f27, 0x4751, 0xbc, 0xd3, 0x5b, 0xa4, 0xb3, 0xc1, 0xf4, 0x10);
DEFINE_GUID(GUID_FILE_REAL_RM,	  0x3a179dbb, 0xaf3e, 0x457d, 0x86, 0xa4, 0x38, 0x48, 0x8a, 0xc3, 0xb4, 0x41);
DEFINE_GUID(GUID_FILE_QT_MOV,	  0x26f558bc, 0x15f1, 0x4774, 0xb1, 0xdf, 0xa8, 0x1b, 0xa6, 0x96, 0x91, 0x9a);
DEFINE_GUID(GUID_FILE_PROMPEG_MXF, 0x24cac9fe, 0x2538, 0x48a0, 0x80, 0x45, 0x19, 0xcc, 0x1, 0x53, 0x44, 0x90);
DEFINE_GUID(GUID_FILE_IMX_MXF,	  0x5da36028, 0x84de, 0x4b8c, 0xa9, 0x61, 0xac, 0xab, 0xa7, 0x4b, 0x4c, 0x15);
DEFINE_GUID(GUID_FILE_SONY_IMX,	  0x637989b5, 0xe4bd, 0x4799, 0x84, 0xd0, 0xc4, 0x24, 0x76, 0x94, 0x5c, 0xd7);
DEFINE_GUID(GUID_FILE_SONY_MAV70, 0xf0c206f1, 0x91ef, 0x49f8, 0xa0, 0x50, 0xdb, 0xfe, 0xd8, 0xc1, 0xb, 0xf9);
DEFINE_GUID(GUID_FILE_SEACHANGE,  0x32d072f9, 0xfdeb, 0x42ac, 0xb1, 0xd9, 0xe3, 0x7, 0x62, 0xfb, 0x6b, 0xcd);
DEFINE_GUID(GUID_FILE_GVG_GXF,	  0x6a1720f0, 0xf248, 0x4a9b, 0xa0, 0x56, 0xe3, 0xe7, 0x61, 0x9, 0x9c, 0x61);
DEFINE_GUID(GUID_FILE_MP1,		  0x52f73977, 0x1ec2, 0x46fd, 0x9e, 0x1e, 0x0, 0xd9, 0xc0, 0x7c, 0xa3, 0x94);
DEFINE_GUID(GUID_FILE_MP2,		  0xe594b376, 0x416d, 0x4533, 0x97, 0xaf, 0x92, 0x1, 0x5b, 0x55, 0xf8, 0xf7);
DEFINE_GUID(GUID_FILE_AAC,        0x5923edff, 0x9104, 0x4988, 0xad, 0xbd, 0xdd, 0x4, 0x11, 0x97, 0x8, 0xc8);
DEFINE_GUID(GUID_FILE_WMA,        0x20086c6e, 0x685f, 0x4035, 0x80, 0x2e, 0x12, 0xc6, 0x8, 0x0, 0x20, 0x6b);
DEFINE_GUID(GUID_FILE_RMA,        0xf0064a78, 0xde97, 0x4482, 0xb6, 0x0, 0xf8, 0xd3, 0x59, 0xcd, 0x2, 0x1e);

DEFINE_GUID(GUID_FILE_SOBEY_SD, 	0x846455e9, 0xde84, 0x4048, 0xb3, 0x93, 0xa3, 0x7e, 0x79, 0x90, 0x1f, 0xff);
DEFINE_GUID(GUID_FILE_SOBEY_HD,		0x3db5160b, 0xea02, 0x4ea4, 0x8f, 0x94, 0x16, 0x8c, 0xf6, 0x4a, 0x68, 0xb0);
DEFINE_GUID(GUID_FILE_MATROX_SD,	0x73935804, 0x1bff, 0x4ff9, 0x84, 0x2d, 0xf0, 0xcd, 0xee, 0x6a, 0xde, 0x80);
DEFINE_GUID(GUID_FILE_MATROX_HD,	0x13d11681, 0x636d, 0x417d, 0xbd, 0x98, 0x2e, 0x7c, 0xd8, 0x51, 0x96, 0x88);
DEFINE_GUID(GUID_FILE_MXF,        	0xd619b87f, 0x8f4b, 0x4a09, 0x99, 0x2a, 0xf4, 0x2b, 0xc1, 0x46, 0x39, 0x18);
DEFINE_GUID(GUID_FILE_STREAMS,		0x6ae1a190, 0xb2d0, 0x48fe, 0xb0, 0xc5, 0xc9, 0x39, 0x95, 0x45, 0x7, 0x4c); //��ý��
DEFINE_GUID(GUID_FILE_PICSEQ,		0xd56b0985, 0xf476, 0x4b8d, 0xb0, 0xe1, 0xad, 0x12, 0xcb, 0xc1, 0x57, 0xda);//NO USE

DEFINE_GUID(GUID_FILE_SOBEY_AVI2_SD, 		0x1c373abe, 0x6f84, 0x47ad, 0x97, 0xb4, 0x64, 0xd7, 0x36, 0x65, 0xe5, 0xcc);
DEFINE_GUID(GUID_FILE_SOBEY_AVI2_HD, 		0xfad1c4da, 0x786e, 0x4bbe, 0x9f, 0xf, 0x94, 0xea, 0xdb, 0x33, 0x4b, 0xfb);
DEFINE_GUID(GUID_FILE_SOBEY_MXF_SD, 		0x30108ebf, 0x86b4, 0x4a77, 0x81, 0x96, 0xf7, 0x9c, 0xd5, 0xe0, 0x56, 0xec);
DEFINE_GUID(GUID_FILE_SOBEY_MXF_HD, 		0x1504fa58, 0x8db5, 0x4316, 0xb7, 0x76, 0x12, 0x46, 0x61, 0x33, 0xba, 0x48);
DEFINE_GUID(GUID_FIO_PLUGIN_MXF, 			0x78b16cef, 0x73bf, 0x4541, 0x96, 0x42, 0x90, 0x79, 0x5b, 0xfd, 0x17, 0x0);
DEFINE_GUID(GUID_FILE_P2MXF_AUDIO, 			0x8c58c5a0, 0x2f54, 0x465e, 0x8f, 0xdb, 0xa7, 0x86, 0xdb, 0xf0, 0x7b, 0x12);
DEFINE_GUID(GUID_FILE_SONY_PROXY_MXF, 		0x33b4e132, 0x4790, 0x427f, 0xbd, 0xe, 0xf8, 0x43, 0x1b, 0x44, 0xf3, 0x2);
DEFINE_GUID(GUID_FILE_SOBEY_MXF_DVCAM,		0x9ad6b91f, 0x4c7a, 0x4c45, 0x8f, 0xdd, 0x10, 0x9e, 0x6c, 0x44, 0x23, 0xdd);
DEFINE_GUID(GUID_FILE_P2MXF_VIDEO, 			0x499e07bf, 0xf432, 0x4a81, 0xb8, 0x3f, 0x14, 0x4d, 0xd4, 0xc8, 0x2e, 0x93);

#define ET_INVALID_TITLE			_T("*?/\\<>|:\"\x1\x2\x3\x4\x5\x6\x7\x8\x9\xA\xB\xC\xD\xE\xF\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x7F")
//#define ET_VALID_MATERIALID         _T("\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a")
#define ET_VALID_MATERIALID			_T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")

#define ET_LOGIC_OPERATOR_AND		_T(' ')
#define ET_LOGIC_OPERATOR_OR		_T('|')
//#define ET_LOGIC_OPERATOR_NOT		_T('-')  //Modified by Liu Jia 20090331 Snp4.4�����޸�:'-'����'!'<<Sonaps_V4.4_SPEC_Search methods improvement.doc>>
#define ET_LOGIC_OPERATOR_NOT		_T('!')
#define ET_LOGIC_OPERATOR_ESC		_T('/')//ת���ַ�


// Ly Add
//#define PROTECTION		3
#define PGMPROTECTION   1
#define TRASHFOLDERID   4

#define ALLPERMISSION      0
#define VIEWPERMISSION     1
#define EDITPERMISSION     2
#define DELETEPERMISSION   4

#define PERMISSIONUSER     1
#define PERMISSIONGROUP    2 

#define CHECKVIEW		   1
#define CHECKEDIT		   2
#define CHECKDELETE        4

// ��Ӧ���ݿ���ֶ�
// Commented by xsc
// ʹ��ETNetDeclaration�еĶ���
// #define NOARCHIVTE	    0
// #define TOBEARCHIVTE	1
// #define ARCHIVTED		2
// #define ARCHIVEALL		4

#define TOBEDELETE		      1
#define NORMALDELETE	      2
#define DELETEPROTECTION      4
#define NODELETE              0

#define LOCKFLAG		1
#define NOLOCK			0

#define NOPROTECTION    0

#define GETPERMISSIONNUM 8

// ���ܸı�Ȩ��
#define NoModityPermission 2

//m_lUsedFlag
#define	USED_PLAYOUT	0x02

struct stuSequenceLink
{
	LONG           lDatabase;//վ��ID
	GUID           guidRes;  //Sequence guid
	DWORD          dErrorCode;//���󷵻�ֵ
	LONG           lEventID;
    CString		   strEventName;
	LONG		   lType;
};
struct stuMLDropingParam
{
	ULONGLONG ullDropType;
	LPARAM lParam;
	ETObject stuCatalog;
};

struct stuMLDropParam
{
	DWORD dwDropType;
	LPARAM lParam;
	ETObject stuCatalog;
};

struct stuGetIconParam
{
	CString strFileName;
	__int64 lPos;
	int iIconWith;
	int iIconHeight;
	LPBYTE lpIcon; //32λ
};

struct stuModifyPgm
{
	GUID guidNewProgram;
	long lDatabaseID;
	__int64 lIn;
	__int64 lOut;
	CString strProgramFile;
};

//��ȡ��Ŀ��Ч���ȵĽṹ��
struct stuPgm
{
	GUID guidProgram;
	__int64 lIn;
	__int64 lOut;
};

//REGWINDOW֪ͨNLE�ӿ�
struct stuRegToEvent                              //wParam
{
	LONG         lDataBase;
	GUID         guidRes;       
	DWORD         dErrorCode;
	LONG          lEventID;//register wnd
};


//��ȡ��Ҫ��Ǩ���زĺͶ�ʧ���ز���
struct stuNeedRetrieveClip
{
	//2009/12/15 ӦNLEҪ���޸� xw
	GUID guidPgm;
	long lDatabase;
	__int64 iTrimIn;
	__int64 iTrimOut;
	ETArrayClipInfo* pClipArray; //��Ҫ�ⲿdelete,��Ҫ��Ǩ����
	CStringArray * parHighFileMiss;//��������ʧ����
	CStringArray * parLowFileMiss;//��������ʧ����
	CStringArray * parProxyOnly;//ProxyOnly����
	DWORD		   dwDBEClass;
	CString strError;

	//GUID guidPgm;
	//long lDatabase;
	//__int64 iTrimIn;
	//__int64 iTrimOut;
	//ETArrayClipInfo* pClipArray; //��Ҫ�ⲿdelete
	//CStringArray * parNotExistFile;
	//CStringArray * parProxyOnly;
	//DWORD		   dwDBEClass;
	//CString strError;
};

//{{2009/04/27 zxp:add ��ȡ��Ŀ��mark��Ϣ��FTC
struct stuMetaData                              //wParam
{
	GUID                    guidTL;             //[in]��Ŀ��guid
	long                    lDatabase;          //[in]��Ŀ��database
	__int64                 iFTC;               //[out]��ĿFTC
	ETArrayClipMarkPoint    arClipMarkPoint;    //[out]��Ŀ�����ı�ǵ�,���Ͷ�����etnetdeclaration.h,��ML�ͷ�
	CString                 strError;           //[out]������Ϣ
};
//}}

//�����ǵ㴫�ݽӿڽṹ��
typedef struct stu_ETClipEssenceMark
{
	long			m_lMarkType;		//�������
	long			m_lMarkColor;		//��ǵ���ɫ
	long			m_lKeyFrame;		//���֡λ��
	long			m_lEndFrame;		//��ǽ�����
	long			m_lAbsKeyFrame;		//���֡λ��(����ʱ��)
	long			m_lAbsEndFrame;		//��ǽ�����(����ʱ��)
	LPBYTE			m_lpIconBuffer;		//ͼ������
	CString         m_strMarkGuid;
	stu_ETClipEssenceMark()
	{
		m_lMarkType = ET_MARKPOINT_ESSENCEMASK;
		m_lKeyFrame = 0;
		m_lEndFrame = 0;
		m_lAbsKeyFrame = 0;
		m_lAbsEndFrame = 0;
		m_lpIconBuffer = NULL;
		m_strMarkGuid = _T("");
	}
}ETClipEssenceMark,*lpETClipEssenceMark;

//���崴������ͼ���ݽӿڽṹ��
typedef struct stu_ETClipThumbnailPicture
{
	CString			m_strFilePath;		//�����ļ���·��
	CString			m_strClipGUID;		//�ز�GUID
	long			m_lIconFrame;		//ͼ��֡��
	LPBYTE			m_lpIconBuffer;		//ͼ������
	stu_ETClipThumbnailPicture()
	{
		m_strFilePath = _T("");
		m_strClipGUID = _T("");
		m_lIconFrame  = 0;
		m_lpIconBuffer = NULL;
	}
}ETClipThumbnailPicture,*lpETClipThumbnailPicture;

//�����������ҳ�ṹ��
typedef struct stu_ETMetadataTabPage
{
	CWnd *			m_pParaWnd;
	LONG			m_lCtrlID;
	CString			m_strTitle;
	LONG			m_lIndex;
}ETMetadataTabPage,*lpETMetadataTabPage;

typedef struct stu_ETMosDragItemContent
{
	CString strPlanningGUID;
	CString	strObjTB;
	CString strTitle;
	CString strNote;
	CString strObjectGUID;
	long    lLength;
	long    lTrimIn;
	long    lTrimOut;
	long    lSource;
	CString strMediaChannel;
	CString strMosID;
	COleDateTime  dtCreation;
}ETMosDragItemContent,*lpETMosDragItemContent;

struct ET_RICHEDIT_CG_DATA
{
	long m_nNumber;
	CString m_strCGText;
	CString m_strTemplateName;
	CString m_strXmlContent;
	BOOL m_bTemplate;
};

struct stuIncludeCG
{
	GUID guidPgm;
	long lDatabase;
	__int64 iTrimIn;
	__int64 iTrimOut;
	BOOL bIncludeCG;//[out]
	CString strError;
};

struct stuIsPopSelForRenderDef
{
	BOOL bRenderUseDef;//[out]
};


struct stuRenderMsgboxSetting

{
	BYTE bret;//[out]
};

typedef struct stuRegWindowInfo
{
	HINSTANCE m_hInstance;
	HWND m_hWnd ;    //�����ߴ��ھ��
	//Targetҳ
	long m_lEventID;   //EVENTID
	long m_lStoryID;
	long m_lRunDownID;

	CString m_strClipGUID;  //
	CString m_strMaterialID; 
	LONG m_lTargetFolderID;
	CString m_strFolder; 
	long m_lRegWndMode;//
	BOOL m_bMatch;                    //ƥ��MaterID	
	BOOL m_bNewMaterID;     //�Ƿ���MaterID���������ز�
	int m_nVerType;
	CString m_strPgmGuid;

	long m_lSOF;
	long m_lEOF;
	long m_lDbe;

	//Settingҳ
	BOOL m_bCompCG;
	BOOL m_bResetTL;
	BOOL m_bSendToMpc;
	BOOL m_bUpdate;
	BOOL m_bEnableCompCG;
	BOOL m_bEnableResetTL;
	BOOL m_bEnableSendToMpc;
	BOOL m_bEnableUpdate;
	CArray<CString ,CString &> m_arStrPgmName;  //��Ŀ�б�

	//StoryIDҳ

	CString m_strStoryIDFULL;
	


	stuRegWindowInfo()
	{
		m_hInstance = NULL;
		m_hWnd = NULL;    //�����ߴ��ھ��
		//Targetҳ
		m_lEventID = 0;   //EVENTID
		m_lStoryID = 0;
		m_lRunDownID = 0;
		m_strClipGUID = _T("");  //
		m_strMaterialID  = _T(""); 
		m_lTargetFolderID = 0;
		m_strFolder = _T(""); 
		m_lRegWndMode = 0;//
		m_bMatch = FALSE;                    //ƥ��MaterID	
		m_bNewMaterID = FALSE;
		m_nVerType = 0;
		m_strPgmGuid = _T("");

		m_lSOF = 0;
		m_lSOF = 0;
		m_lDbe = 0;

		//Settingҳ
		m_bCompCG  = FALSE;
		m_bResetTL = FALSE;
		m_bSendToMpc = FALSE;
		m_bUpdate = FALSE;
		m_bEnableCompCG = FALSE;
		m_bEnableResetTL = FALSE;
		m_bEnableSendToMpc = FALSE;
		m_bEnableUpdate = FALSE;

		m_strStoryIDFULL = _T("");

	}
}ETRegWindowInfo;
/*--------------------------------------------------------------------
//	�ز����ڵ���Ϣ
--------------------------------------------------------------------*/
class AFX_EXT_CLASS CETMLTreeItem
{
public:
	typedef CList<CETMLTreeItem*, CETMLTreeItem*> MLTreeNodeList;

public:
	//	���캯���������ڴ�ռ�
	CETMLTreeItem( DWORD lObjectID)
	{
		objFolder.m_lObjectID	= lObjectID;
		pParent = NULL;
		bHasQueriedSubFolder = FALSE;
	}

	CETMLTreeItem(lpETObject pSrc)
	{
	//	pFolder = pSrc;
		pParent = NULL;
		bHasQueriedSubFolder = FALSE;
	}

	long GetObjectID()
	{
		return objFolder.m_lObjectID;
	/*	if(pFolder)
			return pFolder->m_lObjectID;
		else
			return 0;*/
	}

	//	�����������ͷ���ռ�õ��ڴ�ռ�
	~CETMLTreeItem()
	{
	}

	CETMLTreeItem* AppendChild(CETMLTreeItem* const pChild)
	{
		childs.AddTail(pChild);
		pChild->pParent	= this;

		return this;
	}

public:
	ETObject			objFolder;
//	lpETObject          pFolder;       // For V4.1, ʹ��ָ��.�ɲ���㻺������
	CETMLTreeItem		*pParent;	//	���ڵ�
	MLTreeNodeList		childs;		//	�ӽڵ��б�
	BOOL                bHasQueriedSubFolder; //�Ƿ�̽���˸�Ŀ¼���¼�Ŀ¼, For V4.1

private:
	CETMLTreeItem();		//	��ֹʹ��Ĭ�Ϲ��캯��
};

typedef	struct stu_ResourceUsed{
	CString		m_strPGMID		;
	CString		m_strResGUID	;
	ULONGLONG	m_ullResType	;
	stu_ResourceUsed()
	{
		m_ullResType	=	0	;
	}
	stu_ResourceUsed(const stu_ResourceUsed &_C)
	{
		m_strPGMID		=	_C.m_strPGMID	;
		m_strResGUID	=	_C.m_strResGUID	;
		m_ullResType	=	_C.m_ullResType	;
	}
	stu_ResourceUsed&operator=(const stu_ResourceUsed &_C)
	{
		m_strPGMID		=	_C.m_strPGMID	;
		m_strResGUID	=	_C.m_strResGUID	;
		m_ullResType	=	_C.m_ullResType	;
		return	*this	;
	}
}ET_ResourceUsed;

typedef		CArray<ET_ResourceUsed,ET_ResourceUsed&>ArrayResourceUsed; 


inline BOOL ETGetImageSizeByVS(ULONGLONG ullVideoStandard,long &lWidth,long &lHeight)
{
	lWidth = 720;
	lHeight = 576;
	switch(ullVideoStandard)
	{
	case ET_VideoStandard_UNKNOW:
	case ET_VideoStandard_PAL:
		lWidth = 720;
		lHeight = 576;
		break;
	case ET_VideoStandard_NTSC_2997:
	case ET_VideoStandard_NTSC_30:
	case ET_VideoStandard_SECAM:
		lWidth = 720;
		lHeight = 486;
		break;
	case ET_VideoStandard_1920_1080_50i:
	case ET_VideoStandard_1920_1080_5994i:
	case ET_VideoStandard_1920_1080_60i:
	case ET_VideoStandard_1920_1080_2398p:
	case ET_VideoStandard_1920_1080_24p:
	case ET_VideoStandard_1920_1080_25p:
	case ET_VideoStandard_1920_1080_2997p:
	case ET_VideoStandard_1920_1080_30p:
		lWidth = 1920;
		lHeight = 1080;
		break;
	case ET_VideoStandard_1280_720_2398p:
	case ET_VideoStandard_1280_720_24p:
	case ET_VideoStandard_1280_720_50p:
	case ET_VideoStandard_1280_720_5994p:
		lWidth = 1280;
		lHeight = 720;
		break;
	case ET_VideoStandard_1440_1080_50i:
	case ET_VideoStandard_1440_1080_5994i:
	case ET_VideoStandard_1440_1080_60i:
		lWidth = 1440;
		lHeight = 1080;
		break;
	case ET_VIDEOSTANDARD_1280_1080_50i:
	case ET_VIDEOSTANDARD_1280_1080_5994i:
	case ET_VIDEOSTANDARD_1280_1080_60i:
		lWidth = 1280;
		lHeight = 1080;
		break;
	default:
		lWidth = 720;
		lHeight = 576;
		break;
	}
	return TRUE;
}
inline emETVideoStandard ETGetVideoStandardByFrameRate(double dbFrameRate)
{
	if(_DoubleEqualTest(dbFrameRate, 25.0))
		return ET_VideoStandard_PAL;
	else if(_DoubleEqualTest(dbFrameRate, 29.97))
		return ET_VideoStandard_NTSC_2997;
	else if(_DoubleEqualTest(dbFrameRate, 30.0))
		return ET_VideoStandard_NTSC_30;
	else if(_DoubleEqualTest(dbFrameRate, 23.98))
		return ET_VideoStandard_1920_1080_2398p;
	else if(_DoubleEqualTest(dbFrameRate, 24.0))
		return ET_VideoStandard_1920_1080_24p;
	else if(_DoubleEqualTest(dbFrameRate, 50.0))
		return ET_VideoStandard_1280_720_50p;
	else 
		return ET_VideoStandard_PAL;
}


inline double ETGetFrameRateByVS(ULONGLONG ullVideoStandard ,long lNtscMode = 0,BOOL bCalNDF = FALSE)
{

	double dFrameRate = 25;
	if(bCalNDF)
	{
		//TRUE ȡ�Ƕ�֡֡�ʽ���������ʾ��
		switch(ullVideoStandard)
		{
		case ET_VideoStandard_PAL:
		case ET_VIDEOSTANDARD_PAL_16_9:
		case ET_VideoStandard_1920_1080_50i:
		case ET_VideoStandard_1920_1080_25p:
		case ET_VideoStandard_1440_1080_50i:
		case ET_VIDEOSTANDARD_1280_1080_50i:
			{
				dFrameRate = 25;
			}
			break;
		case ET_VideoStandard_NTSC_2997:
		case ET_VideoStandard_1920_1080_5994i:
		case ET_VideoStandard_1920_1080_2997p:
		case ET_VideoStandard_1440_1080_5994i:
		case ET_VIDEOSTANDARD_1280_1080_5994i:
		case ET_VIDEOSTANDARD_NTSC_2997_16_9:
		case ET_VIDEOSTANDARD_NTSC_2997_480:
			{
				if(lNtscMode)
				{
					dFrameRate = 30;
				}else
				{
					dFrameRate = 29.97;
				}
				//dFrameRate = 30000.0 / 1001.0;	// 29.97
			}
			break;
		case ET_VideoStandard_NTSC_30:
		case ET_VideoStandard_1920_1080_60i:
		case ET_VideoStandard_1920_1080_30p:
		case ET_VideoStandard_1440_1080_60i:
		case ET_VIDEOSTANDARD_1280_1080_60i:
		case ET_VIDEOSTANDARD_NTSC_30_16_9:
		case ET_VIDEOSTANDARD_NTSC_30_480:
			{
				if(lNtscMode)
				{
					dFrameRate = 30;
				}else
				{
					dFrameRate = 29.97;
				}
				//dFrameRate = 30;
			}
			break;
		case ET_VideoStandard_1920_1080_2398p:
		case ET_VideoStandard_1280_720_2398p:
			dFrameRate = 23.98;
			break;
		case ET_VideoStandard_1920_1080_24p:
		case ET_VideoStandard_1280_720_24p:
			dFrameRate = 24;
			break;
		case ET_VideoStandard_1280_720_50p:
			dFrameRate = 50;
			break;
		case ET_VideoStandard_1280_720_5994p:
			dFrameRate = 59.94;
			break;
		default:
			break;
		}
	}else
	{
		switch(ullVideoStandard)
		{
		case ET_VideoStandard_PAL:
		case ET_VIDEOSTANDARD_PAL_16_9:
		case ET_VideoStandard_1920_1080_50i:
		case ET_VideoStandard_1920_1080_25p:
		case ET_VideoStandard_1440_1080_50i:
		case ET_VIDEOSTANDARD_1280_1080_50i:
			{
				dFrameRate = 25;
			}
			break;
		case ET_VideoStandard_NTSC_2997:
		case ET_VideoStandard_1920_1080_5994i:
		case ET_VideoStandard_1920_1080_2997p:
		case ET_VideoStandard_1440_1080_5994i:
        case ET_VIDEOSTANDARD_1280_1080_5994i:
		case ET_VIDEOSTANDARD_NTSC_2997_16_9:
		case ET_VIDEOSTANDARD_NTSC_2997_480:

			{
				dFrameRate = 30000.0 / 1001.0;	// 29.97
			}
			break;
		case ET_VideoStandard_NTSC_30:
		case ET_VideoStandard_1920_1080_60i:
		case ET_VideoStandard_1920_1080_30p:
		case ET_VideoStandard_1440_1080_60i:
		case ET_VIDEOSTANDARD_1280_1080_60i:
		case ET_VIDEOSTANDARD_NTSC_30_16_9:
		case ET_VIDEOSTANDARD_NTSC_30_480:

			{
				dFrameRate = 30;
			}
			break;
		case ET_VideoStandard_1920_1080_2398p:
		case ET_VideoStandard_1280_720_2398p:
			dFrameRate = 23.98;
			break;
		case ET_VideoStandard_1920_1080_24p:
		case ET_VideoStandard_1280_720_24p:
			dFrameRate = 24;
			break;
		case ET_VideoStandard_1280_720_50p:
			dFrameRate = 50;
			break;
		case ET_VideoStandard_1280_720_5994p:
			dFrameRate = 59.94;
			break;
		default:
			break;
		}


	}

	return dFrameRate;
}



//ת��֡��100����
inline LONGLONG ETConvertFrameTo100NS(long frame, double frameRate)
{

	//LONGLONG llLength, lFrameRate;

	////Add by Liu Jia 2009/01/08
	//if ((dFrameRate >= 29.99999) && (dFrameRate <= 30.00001))
	//{
	//	dFrameRate = (LONGLONG)lFrame * 1000 * 1000 * 10 * 1001 / 30000;
	//	return dFrameRate;
	//}

	//llLength = lFrame;
	//lFrameRate = (long)(dFrameRate * 100 + 0.5);
	//llLength = llLength * 1000 * 1000 * 10;
	//llLength = (llLength/lFrameRate)*100;

	//return llLength;

	LONGLONG ns = 0;
	
	if (frame <= 0)
	{
		return ns;
	}

	if (fabs((frameRate - (30000.0 / 1001.0))) < 0.01)
	{
		// NTSC 29.97
		ns = (LONGLONG)frame * 1000 * 1000 * 10 * 1001 / 30000;
		return ns;
	}

	// otherwise
	//ns = (LONGLONG)(((double)frame / frameRate) * 1000 * 1000 * 10);
	ns = ((LONGLONG)frame * 1000 * 1000 * 10) / (int)frameRate;
	return ns;
}

//ת��100���뵽֡
inline LONGLONG ETConvert100NSToFrame(const LONGLONG& ll100ns, const double& dbFrameRate)
{
	LONGLONG	llRate	= MPC_ST_FRAME_RATE_25;
	LONGLONG	llScale	= MPC_ST_SCALE_25;
	if(fabs(dbFrameRate - (double)MPC_ST_FRAME_RATE_25 / (double)MPC_ST_SCALE_25) < 0.001)
	{
		llRate = MPC_ST_FRAME_RATE_25;
		llScale = MPC_ST_SCALE_25;
	}
	else if(fabs(dbFrameRate - (double)MPC_ST_FRAME_RATE_2997 / (double)MPC_ST_SCALE_2997) < 0.001) 
	{
		llRate = MPC_ST_FRAME_RATE_2997;
		llScale = MPC_ST_SCALE_2997;
	} 
	else if(fabs(dbFrameRate - (double)MPC_ST_FRAME_RATE_30 / (double)MPC_ST_SCALE_30) < 0.001) 
	{
		llRate = MPC_ST_FRAME_RATE_30;
		llScale = MPC_ST_SCALE_30;
	}
	else if(fabs(dbFrameRate - (double)MPC_ST_FRAME_RATE_24 / (double)MPC_ST_SCALE_24) < 0.001) 
	{
		llRate = MPC_ST_FRAME_RATE_24;
		llScale = MPC_ST_SCALE_24;
	}
	else if(fabs(dbFrameRate - (double)MPC_ST_FRAME_RATE_2398 / (double)MPC_ST_SCALE_2398) < 0.001)
	{
		llRate = MPC_ST_FRAME_RATE_2398;
		llScale = MPC_ST_SCALE_2398;
	}

	return static_cast<DWORD>((ll100ns * llRate / 1000.0 / 1000.0 / 10.0 / llScale) + 0.5);
}

inline BOOL ET_IsRunInEditMax()
{
	static long bIsRunInEditMax = -1;
	if(bIsRunInEditMax == -1)
	{
		TCHAR lpBuffer[256];
		CString strModuleFileName;
		memset(lpBuffer,0,256*sizeof(TCHAR));
		GetModuleFileName(NULL,lpBuffer,255);
		strModuleFileName = lpBuffer;
		strModuleFileName.MakeUpper();
		if(strModuleFileName.Right(16) == _T("XPRIEDITMAIN.EXE"))
			bIsRunInEditMax = 1;
		else
		{
			//	if(GetExplorerUIMode(this) == ET_EXPLORER_MODE_PM)
			//		bIsRunInEditMax = 1;
			//	else
			bIsRunInEditMax = 0;
		}
	}
	return bIsRunInEditMax;
}

static CString				g_strSegmentLeftBracket = _T("��Ļ");
static CString				g_strSegmentRightBracket = _T("��");
static CString				g_strCGSegmentName = _T("��");

inline BOOL ParseCGData(LPCTSTR lpszText, ET_RICHEDIT_CG_DATA *pCgData)
{
	int nPos,n,nLen;
	CString strText,strTemp;
	if(pCgData)
	{
		pCgData->m_bTemplate = FALSE;
		pCgData->m_nNumber = 0;
		pCgData->m_strCGText = _T("");
		pCgData->m_strXmlContent = _T("");
	}

	strText = lpszText;
	nPos = strText.Find(g_strSegmentLeftBracket);
	nPos += g_strSegmentLeftBracket.GetLength();
	nLen = g_strCGSegmentName.GetLength();
	strTemp = strText.Mid(nPos,nLen);
	if(strTemp != g_strCGSegmentName)//������Ļ
		return FALSE;
	nPos += nLen;
	strTemp = strText.Mid(nPos,1);
	if(strTemp != _T(":"))//������Ļ
		return FALSE;
	nPos ++;
	n = strText.Find(_T(":"),nPos);
	if(n == -1)//������Ļ
		return FALSE;
	if(pCgData)
	{
		strTemp = strText.Mid(nPos,n - nPos);
		pCgData->m_nNumber = _ttol(strTemp);
		nPos = n +1;
		n = strText.Find(g_strSegmentRightBracket,nPos);
		pCgData->m_strCGText = strText.Mid(nPos,n - nPos);
	}
	return TRUE;
}

//!ϵͳ��������
enum emETSystemParamType
{
	ET_emSysParamType_Unknow = 0,
	ET_emSysParamType_NLE = 1,//!sobey EditMax(�Ǳ�)
	ET_emSysParamType_NEWS = 2,//!sobey �ĸ�
	ET_emSysParamType_NM = 3//!sobey ����
};

//!ϵͳ������Ϣ
typedef struct stu_ETSystemSetting
{
	//CString m_strParamName;
	CString m_strParamValue;
}ETSystemSetting,*lpETSystemSetting;


//!��Դ��������
enum emETResourceQualityType
{
	ET_emETResourceQualityType_High = 0,
	ET_emETResourceQualityType_Low = 1,
	ET_emETResourceQualityType_Mid = 2,
};

//!��½ϵͳ����
enum emETLoginSystemType
{
	ET_emLoginSystem_UnKnow = 0,
	ET_emLoginSystem_EditMax = 1,
	ET_emLoginSystem_Censor = 2,
	ET_emLoginSystem_TechCensor = 3,
	ET_emLoginSystem_News = 4
};

//!�������Ͷ���(xsc,2007.7.2)
enum enumTransmitMode
{
	ET_TransMode_Unknown = 0,
	ET_TransMode_Archive = 1,//����⵽ý�ʿ�Ĺ鵵
	ET_TransMode_Retrieve,  //ý���������Ļ�Ǩ
	ET_TransMode_Exchang    //�����֮����زĽ���
};

//!Ȩ��������Ŀ
enum emETPopedomColumnType
{
	ET_em_PC_Unknow = 0,
	ET_em_PC_PlayList_MT = 1,//!������ά��Ȩ��
	ET_em_PC_PlayList_Query = 2,//!��������ѯ
	ET_em_PC_Clip_Query = 3,//!�زĲ�ѯ
	ET_em_PC_News_Query = 4,//!�ĸ��ѯ
	ET_em_PC_BroadCast = 5,//!������Ŀ
	ET_em_PC_Cover_Query = 6,//!�ɷ���Ŀ
	ET_em_PC_Cover_BroadCast = 7,//!�ɷñ�����Ŀ
	ET_em_PC_News_MT =8 //!����ά��, added by xiashaochao, 2006.10.11, for new popedom rule.
};

//!Ƶ��Ȩ������
// added by xiashaochao, 2006.10.11,for new popedom rule.
enum emETPopedomChannelType
{
	ET_em_PCH_Unknown	= 0,  
	ET_em_PCH_MT		= 1, //!Ƶ��ά��
	ET_em_PCH_QUERY		= 2  //!Ƶ����ѯ
};

//!�ز�·������
enum emETClipPathType
{
	ET_em_ClipPath_High = 0,
	ET_em_ClipPath_Low = 1,
	ET_em_ClipPath_Mid =2
};

//!��Ŀ�ز�·��
enum emETPgmClipPathType
{
	ET_emPgmCPT_V_High = 1,//video
	ET_emPgmCPT_V_Mid  = 2,
	ET_emPgmCPT_V_Low  = 3,
	ET_emPgmCPT_A_High = 4,//audio
	ET_emPgmCPT_A_Mid  = 5,
	ET_emPgmCPT_A_Low  = 6
};
//!��Ŀ����
typedef DWORD ETProgramType;

#define ET_ProgramType_UnKnow		0x00000000
#define ET_ProgramType_TIMELINE		0x00000001//��ͨ��Ŀ
#define ET_ProgramType_CGTIMELINE	0x00000002//��Ļʱ����
#define ET_ProgramType_CGTEMPALTE	0x00000004//��Ļ����
#define ET_ProgramType_CLIP			0x00000008

//!��Ŀ�������򿪣�����
enum emETPgmLockType
{
	ET_emPgmLockType_Unknow = 0,
	ET_emPgmLockType_Read = 1,
	ET_emPgmLockType_Dub = 2,
	ET_emPgmLockType_NoDub = 3,
	ET_emPgmLockType_All = 4
};


//!��Ŀ����ʽ��׼ö�ٶ���
enum emETPgmVideoStandard
{	
	ET_emPgm_VS_UNKNOW				= 0x00000000,		//Invalid

	ET_emPgm_VS_PAL				    = 0x00000001,		//PAL size:720*576 f/s : 25
	ET_emPgm_VS_NTSC_2997			= 0x00000002,		//NTSC size:720*486  f/s : 29.97
	ET_emPgm_VS_NTSC_30			    = 0x00000004,		//NTSC size:720*486 f/s : 30 
	ET_emPgm_VS_SECAM				= 0x00000008,		//SECAM

	ET_emPgm_VS_1920_1080_50i		= 0x00000010,		//HDTV size:1920*1080 f/s : 25  interlaced
	ET_emPgm_VS_1920_1080_5994i	    = 0x00000020,		//HDTV size:1920*1080 f/s : 29.97 interlaced
	ET_emPgm_VS_1920_1080_60i		= 0x00000040,		//HDTV size:1920*1080 f/s : 30 interlaced

	ET_emPgm_VS_1920_1080_2398p	    = 0x00000080,		//HDTV size:1920*1080 f/s : 23.98 progressive
	ET_emPgm_VS_1920_1080_24p		= 0x00000100,		//HDTV size:1920*1080 f/s : 24 progressive
	ET_emPgm_VS_1920_1080_25p		= 0x00000200,		//HDTV size:1920*1080 f/s : 25 progressive
	ET_emPgm_VS_1920_1080_2997p	    = 0x00000400,		//HDTV size:1920*1080 f/s : 29.97 progressive
	ET_emPgm_VS_1920_1080_30p		= 0x00000800,		//HDTV size:1920*1080 f/s : 30 progressive

	ET_emPgm_VS_1280_720_2398p		= 0x00001000,	    //HDTV size:1280*720 f/s : 23.98 progressive
	ET_emPgm_VS_1280_720_24p		= 0x00002000,		//HDTV size:1280*720 f/s : 24 progressive
	ET_emPgm_VS_1280_720_50p		= 0x00004000,		//HDTV size:1280*720 f/s : 50 progressive
	ET_emPgm_VS_1280_720_5994p		= 0x00008000,		//HDTV size:1280*720 f/s : 59.94 progressive

	ET_emPgm_VS_1440_1080_50i		= 0x00010000,	    //HD  size:1440*1080 f/s : 25 interlaced
	ET_emPgm_VS_1440_1080_5994i	    = 0x00020000,	    //HD  size:1440*1080 f/s : 29.97 interlaced
	ET_emPgm_VS_1440_1080_60i		= 0x00040000,	    //HD  size:1440*1080 f/s : 30 interlaced	
};
//!��Դ���
typedef struct stu_ET_RESOURCECACHEDATA
{
	void *         m_hResData;
	ULONGLONG	   m_eResType;
	GUID           m_guidRes;
	LPARAM         m_lParam;
	DWORD		   m_RefCount;
	DWORD		   m_dwCreateTime;
	DWORD		   m_dwUsedCount;
	DWORD		   m_dwLastTime;
	stu_ET_RESOURCECACHEDATA()
	{
		m_eResType = 0;
		m_hResData = NULL;
		m_guidRes = GUID_NULL;
		m_lParam = NULL;
		m_RefCount = 0;
		m_dwCreateTime = 0;
		m_dwUsedCount = 0;
		m_dwLastTime = 0;
	}
} ET_RESOURCECACHEDATA,*LPET_RESOURCECACHEDATA;

//!
//!�ĸ������Ϣ
enum emSegmentType
{
	ET_emSegmentType_Text = 0,	//����
	ET_emSegmentType_Guide = 1,	//����
	ET_emSegmentType_Other = 2	//����
};
typedef struct stu_ETSegmentInfo
{
	DWORD			m_dwSegmentID;		//����ID
	CString			m_strSegmentName;	//������
	CString			m_strChannelCode;	//Ƶ������
	emSegmentType	m_lSegmentType;		//��������
	BOOL			m_bUsable;			//�ܷ�ʹ��
	stu_ETSegmentInfo()
	{
		m_dwSegmentID = 0;
		m_strSegmentName = _T("");
		m_strSegmentName = _T("");
		m_lSegmentType = ET_emSegmentType_Other;
		m_bUsable = FALSE;
	}
	stu_ETSegmentInfo & operator = (const stu_ETSegmentInfo & et)
	{
		m_dwSegmentID		= et.m_dwSegmentID;
		m_strSegmentName	= et.m_strSegmentName;
		m_strChannelCode	= et.m_strChannelCode;
		m_lSegmentType		= et.m_lSegmentType;
		m_bUsable			= et.m_bUsable;
		return *this;
	}
}ETSegmentInfo,*lpETSegmentInfo;
typedef CArray<lpETSegmentInfo,lpETSegmentInfo& > ETArraySegmentInfo;
typedef struct stu_ETSegment_TextData
{
	lpETSegmentInfo m_lpSegmentInfo;
	CString			m_strText;
	DWORD			m_dwWordNum;
	stu_ETSegment_TextData()
	{
		m_lpSegmentInfo = NULL;
		m_strText = _T("");
		m_dwWordNum = 0;
	}
	stu_ETSegment_TextData & operator = (const stu_ETSegment_TextData & et)
	{
		m_lpSegmentInfo		= et.m_lpSegmentInfo;
		m_strText			= et.m_strText;
		m_dwWordNum			= et.m_dwWordNum;
		return *this;
	}
}ETSegmentTextData,*lpETSegmentTextData;
typedef CArray<ETSegmentTextData,ETSegmentTextData& > ETArraySegmentTextData;

//�ж���
typedef struct tagETColumnCustom
{
	CString strColumnName;//�������������ݵĹ�������Ϊ���ݿ��е��ֶ���
	CString strColumnTitle;
	int		iColumnIndex;
	int		iColumnWidth;
	int		iColumnFormat;
	tagETColumnCustom()
	{
		Reset();
	}
	void Reset()
	{
		strColumnName = _T("");
		strColumnTitle = _T("");
		iColumnIndex = 0;
		iColumnWidth = 0;
		iColumnFormat = 0;
	}
	tagETColumnCustom& operator = (const tagETColumnCustom& src)
	{
		strColumnName = src.strColumnName;
		strColumnTitle = src.strColumnTitle;
		iColumnIndex = src.iColumnIndex;
		iColumnWidth = src.iColumnWidth;
		iColumnFormat = src.iColumnFormat;
		return *this;
	}
}ETColumnCustom, *lpETColumnCustom;

typedef CArray<lpETColumnCustom, lpETColumnCustom> ETArrayColumnCustom;



//PEĬ��·��
typedef struct tagETUserSelectPEPath
{
	long			m_lUserId;
	CString			m_strPath;
	DATE			m_dtUseTime;

	tagETUserSelectPEPath()
	{
		Reset();
	}
	void Reset()
	{
		m_strPath		= _T("");
		m_lUserId		= 0;
		m_dtUseTime		= 0.0;
	}
	tagETUserSelectPEPath& operator = (const tagETUserSelectPEPath& src)
	{
		m_dtUseTime = src.m_dtUseTime;
		m_lUserId	= src.m_lUserId;
		m_strPath   = src.m_strPath;
		return *this;
	}
}ETUserSelectPEPath, *lpETUserSelectPEPath;

typedef CArray<lpETUserSelectPEPath, lpETUserSelectPEPath> ETArrayUserSelectPEPath;


typedef struct tagETThumbnail
{
	LONG       biWidth;
	LONG       biHeight;
	WORD       biBitCount;
	LPBYTE		pBuffer;
	LONG		lIconFrame;

	tagETThumbnail(int nWdith = 0, int nHeight = 0, int nBitCount = 32, bool bInitialize = true) :
	biWidth(nWdith), biHeight(nHeight), biBitCount(nBitCount)
	{
		lIconFrame = 0;
		if ( nWdith != 0 && nHeight != 0)
		{
			ASSERT(nBitCount % 32 == 0 && nBitCount > 0);

			int		nBufferSize;

			biWidth		= nWdith;
			biHeight	= nHeight;
			biBitCount	= nBitCount;
			nBufferSize	= nWdith * nHeight * biBitCount / 8;
			pBuffer		= new BYTE[nBufferSize];
			if ( bInitialize )
				memset(pBuffer, 0, nBufferSize);
		}
		else
			pBuffer		= NULL;
	}

	~tagETThumbnail()
	{
		try
		{
			if ( pBuffer != NULL)
			{
				delete[] pBuffer;
			}
		}
		catch(...)
		{
		}
	}
}
ETThumbnail;

class CETFreeParameter
{
	typedef struct stu_ETMLParameter
	{
		CString m_strParamName;
		CString m_strParamValue;
		stu_ETMLParameter()
		{
			m_strParamName	= _T("");
			m_strParamValue	= _T("");
		}
		stu_ETMLParameter(LPCTSTR lpszName, LPCTSTR lpszValue)
		{
			m_strParamName = lpszName;
			m_strParamValue = lpszValue;
		}
		const stu_ETMLParameter& operator = (const stu_ETMLParameter &src)
		{
			m_strParamName		=src.m_strParamName;
			m_strParamValue		=src.m_strParamValue;
			return *this;
		}
	}ETMLParameter, *LPETMLParameter;    

public:
	CETFreeParameter()
	{
		Reset();
	}
	CETFreeParameter(const CETFreeParameter& src)
	{
		*this = src;
	}
	~CETFreeParameter()
	{
		Reset();
	}

	CETFreeParameter& operator= (const CETFreeParameter& src)
	{
        Reset();

		for(INT_PTR i=0;i<src.m_arDatas.GetCount();i++)
		{

			LPETMLParameter pParam = new ETMLParameter;

			*pParam = *src.m_arDatas[i];
			m_arDatas.Add(pParam);
		}

		return *this;
	}
	void Reset()
	{
		for(INT_PTR i=0;i<m_arDatas.GetCount();i++)
		{
			delete m_arDatas[i];
		}
		m_arDatas.RemoveAll();
	}

	void SetParameter(LPCTSTR lpszName, LPCTSTR lpszValue)
	{
		for(INT_PTR i=0;i<m_arDatas.GetCount();i++)
		{
			if(m_arDatas[i]->m_strParamName.CompareNoCase(lpszName) == 0)
			{
				m_arDatas[i]->m_strParamValue = lpszValue;
				return;
			}
		}
		LPETMLParameter pParam = new ETMLParameter;
		pParam->m_strParamName = lpszName;
		pParam->m_strParamValue = lpszValue;

		m_arDatas.Add(pParam);
	}

	void SetParameter(LPCTSTR lpszName, GUID guidValue)
	{
		CString strGUID;
		//ET_FromGUIDToString(guidValue, strValue);

		TCHAR lpBuffer[40];
		memset(lpBuffer,0,40*sizeof(TCHAR));
		StringFromGUID2(guidValue,lpBuffer,40*sizeof(40));
		strGUID = lpBuffer;
		strGUID.Replace(_T("{"),_T(""));
		strGUID.Replace(_T("-"),_T(""));
		strGUID.Replace(_T("}"),_T(""));
		strGUID.MakeLower();

		SetParameter(lpszName, strGUID);

	}

	void SetParameter(LPCTSTR lpszName, long lValue)
	{
		CString strValue;
		strValue.Format(_T("%ld"), lValue);

		SetParameter(lpszName, strValue);
	}

	void SetParameter(LPCTSTR lpszName, COleDateTime dt)
	{
		CString strValue;

		strValue = dt.Format(_T("%y-%m-%d %H:%M:%S"));

		SetParameter(lpszName, strValue);
	}

	BOOL GetParameter(const CString& lpszName, CString& strValue) const
	{
		for(INT_PTR i=0;i<m_arDatas.GetCount();i++)
		{
			if(m_arDatas[i]->m_strParamName.CompareNoCase(lpszName) == 0)
			{
				strValue =  m_arDatas[i]->m_strParamValue;
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL  GetParameterLong(LPCTSTR lpszName, long& lValue) const
	{
		
		CString strValue;
		if(!GetParameter(lpszName, strValue))
			return FALSE;

		if(strValue.IsEmpty())
			return FALSE;
		else
		{
			lValue = _wtol(strValue);
			return TRUE;
		}
	}

	BOOL GetParameterDate(LPCTSTR lpszName, COleDateTime& dt) const
	{
		CString strValue;
		if(!GetParameter(lpszName, strValue))
			return FALSE;

		if(strValue.IsEmpty())
			return FALSE;
		else
		{
			dt.ParseDateTime(lpszName);
			return TRUE;
		}
	}

	int GetCount() const
	{
		return (int)m_arDatas.GetCount();
	}

	BOOL GetParameter(long nIndex, CString& strName, CString& strValue) const
	{
		LPETMLParameter lpParameter = NULL;
		if(nIndex >= m_arDatas.GetSize())
			return FALSE;
		lpParameter = m_arDatas.GetAt(nIndex);
		if(lpParameter == NULL)
			return FALSE;

		strName = lpParameter->m_strParamName;
		strValue = lpParameter->m_strParamValue;
		return TRUE;
	}

	BOOL GetParameter(int nIndex, CString& strName, CString& strValue) const
	{
		if(nIndex<GetCount() && nIndex>=0)
		{
			strName = m_arDatas[nIndex]->m_strParamName;
			strValue = m_arDatas[nIndex]->m_strParamValue;

			return TRUE;
		}
		else
			return FALSE;

	}

protected:
	CArray<LPETMLParameter,LPETMLParameter &> m_arDatas;	 
};
/*

typedef struct stu_TransmitTaskDef
{
	IETVirtualML * pSource;      //Դ
	IETVirtualML * pTarget;      //Ŀ��

	ETArrayObject* pObjArray;    //��������
	enumTransmitMode emTransmode;//����ģʽ
    CETFreeParameter arParams;  //�������

	stu_TransmitTaskDef()
	{
		pSource = NULL;
		pTarget = NULL;

		pObjArray = NULL;
		emTransmode = ET_TransMode_Unknown;
		arParams.Reset();
	}

	~stu_TransmitTaskDef()
	{
		pSource = NULL;
		pTarget = NULL;

		pObjArray = NULL;
		emTransmode = ET_TransMode_Unknown;
		arParams.Reset();
	}
}ETTransmitTaskDef, *lpETTransmitTaskDef;*/


typedef struct tagETMLServerInfo
{
	CString			m_strMachineIp;		//MACHINEIP                      VARCHAR2(20) NOT NULL,
	USHORT			m_usPort;			//PORT                           NUMBER(10,0),
	CString			m_strMachineName;	//MACHINENAME                    VARCHAR2(128),
	long			m_lServerType;		//SERVERTYPE                     NUMBER(10,0),
	CString			m_strSubNet;		//SUBNET                         VARCHAR2(20),
	CString			m_strLoginUser;		//LOGINUSER                      VARCHAR2(128),
	DATE			m_dtCreateData;		//CREATEDATE                     DATE,
	long			m_lMemorySize;		//MEMORYSIZE                     NUMBER(10,0),
	CString			m_strMSMQName;		//MSMQNAME                       VARCHAR2(128),

	tagETMLServerInfo()
		: m_strMachineIp(_T("127.0.0.1"))
		, m_usPort(10000)
		, m_strMachineName(_T(""))
		, m_lServerType(1)
		, m_strSubNet(_T(""))
		, m_strLoginUser(_T(""))
		, m_dtCreateData(0.0f)
		, m_lMemorySize(0)
		, m_strMSMQName(_T(""))
	{
	}
	~tagETMLServerInfo(){}

	tagETMLServerInfo& operator = (const tagETMLServerInfo& info)
	{
		m_strMachineIp		= info.m_strMachineIp;
		m_usPort			= info.m_usPort;
		m_strMachineName	= info.m_strMachineName;
		m_lServerType		= info.m_lServerType;
		m_strSubNet			= info.m_strSubNet;
		m_strLoginUser		= info.m_strLoginUser;
		m_dtCreateData		= info.m_dtCreateData;
		m_lMemorySize		= info.m_lMemorySize;
		m_strMSMQName		= info.m_strMSMQName;
		return *this;
	}


}ETMLServerInfo, *LPETMLServerInfo;
typedef CArray<LPETMLServerInfo, LPETMLServerInfo>		ETMLServerInfoArray;

typedef int (__cdecl *GenericCompareFunc)(const void *elem1, const void *elem2);
typedef int (__cdecl *ObjectCompareFunc)(lpETObject *pObject1, lpETObject *pObject2);


typedef AFX_EXT_CLASS struct stu_ETObjSearch
{
public:
	CString			m_strTitle;
	CString         m_strMaterialID;//2010/06/03 zxp:add V4.6 ����Material ID���������������titleһ��
	CString			m_strComments;
	CString			m_strRights;
	CString			m_strDepartmentID;
	CString			m_strEditorID;
	CString			m_strEditor;
	CString			m_strIngestID;
	CString			m_strMarkComments;
	DATE			m_dtBegin;
	DATE			m_dtEnd;
	DATE			m_dtUpdateBegin;
	DATE			m_dtUpdateEnd;
	BOOL			m_bBegin;
	BOOL			m_bEnd;
	BOOL			m_bUpdateBegin;
	BOOL			m_bUpdateEnd;
	CString			m_strPath;
	long			m_lPath;
	int				m_iType;
	int				m_iOriginType;
	long			m_lSubType;
	BOOL			m_bToBeDelete;
	BOOL			m_bProtection;
	BOOL			m_bToBeArchive;
	BOOL			m_bArchived;
	BOOL			m_bToBeRetrieve;
	CArray<long ,long>m_arlPath;// ����·��
	CString			m_strSource;
	int				m_iSource;
	CString         m_strReserved1;//��ĿID

	// ��̬�İ�ť
	int             m_nToBeRetrieveFlag; 
	int             m_nToBeDeleteFlag;
	int             m_nToBeArchiveFlag;
	int             m_nArchivedFlag;
	int             m_bProtectionFlag;

	CString         m_strKeywords;

	// Search Type
	BOOL			m_paramsearch;

	BOOL            m_bNobody;//V4.5.1����nobody�û�������

	stu_ETObjSearch()
	{
		Reset();
	}
	void stu_ETObjSearch::Reset()
	{
		m_strDepartmentID	= _T("");
		m_strEditorID		= _T("");
		m_strEditor			= _T("");
		m_strTitle			= _T("");
		m_strMaterialID     = _T("");
		m_strComments		= _T("");
		m_strRights			= _T("");
		m_strIngestID		= _T("");
		m_strMarkComments	= _T("");
		m_dtBegin			= 0.00;	
		m_dtEnd				= 0.00;
		m_dtUpdateBegin		= 0.00;
		m_dtUpdateEnd		= 0.00;
		m_bBegin			= FALSE;
		m_bEnd				= FALSE;
		m_bUpdateBegin		= FALSE;
		m_bUpdateEnd		= FALSE;
		m_strPath			= _T("");
		m_lPath				= -1;
		m_iType				= -1;
		m_iOriginType		= ET_OriginType_All;
		m_lSubType			= -1;
		m_bToBeDelete		= FALSE;
		m_bProtection		= FALSE;
		m_bToBeArchive		= FALSE;
		m_bArchived			= FALSE;
		m_bToBeRetrieve		= FALSE;
		m_iSource			= -1;
		m_strReserved1		= _T("");

		m_nToBeRetrieveFlag = BST_INDETERMINATE; 
		m_nToBeDeleteFlag   = BST_INDETERMINATE;
		m_nToBeArchiveFlag  = BST_INDETERMINATE;
		m_bProtectionFlag   = BST_INDETERMINATE;
		m_nArchivedFlag     = BST_INDETERMINATE;

		m_strKeywords       = _T("");

		m_paramsearch = FALSE;

		m_bNobody = FALSE;
	}
	const stu_ETObjSearch& operator = (const stu_ETObjSearch &src)
	{
		m_strTitle			= src.m_strTitle;
		m_strMaterialID     = src.m_strMaterialID;
		m_strComments		= src.m_strComments;
		m_strRights			= src.m_strRights;
		m_strDepartmentID	= src.m_strDepartmentID;
		m_strEditorID		= src.m_strEditorID;
		m_strEditor			= src.m_strEditor;
		m_strIngestID		= src.m_strIngestID;		
		m_strMarkComments	= src.m_strMarkComments;
		m_dtBegin			= src.m_dtBegin;
		m_dtEnd				= src.m_dtEnd;
		m_dtUpdateBegin		= src.m_dtUpdateBegin;
		m_dtUpdateEnd		= src.m_dtUpdateEnd;
		m_bBegin			= src.m_bBegin;
		m_bEnd				= src.m_bEnd;
		m_bUpdateBegin		= src.m_bUpdateBegin;
		m_bUpdateEnd		= src.m_bUpdateEnd;
		m_strPath			= src.m_strPath;
		m_lPath				= src.m_lPath;
		m_iType				= src.m_iType;
		m_iOriginType		= src.m_iOriginType;
		m_lSubType			= src.m_lSubType;
		m_bToBeDelete		= src.m_bToBeDelete;
		m_bProtection		= src.m_bProtection;
		m_bToBeArchive		= src.m_bToBeArchive;	// ȱ�ٸ��е���ָ���������鵵����ѯ��Ч
		m_bArchived			= src.m_bArchived;
		m_strSource			= src.m_strSource;
		m_iSource			= src.m_iSource;
		m_bToBeRetrieve		= src.m_bToBeRetrieve;
		m_strReserved1		= src.m_strReserved1;

		m_nToBeRetrieveFlag = src.m_nToBeRetrieveFlag; 
		m_nToBeDeleteFlag   = src.m_nToBeDeleteFlag;
		m_nToBeArchiveFlag  = src.m_nToBeArchiveFlag;
		m_nArchivedFlag     = src.m_nArchivedFlag;
		m_bProtectionFlag   = src.m_bProtectionFlag;

		m_strKeywords       = src.m_strKeywords;

		m_arlPath.RemoveAll();// ����·��		
		for(int i= 0; i < src.m_arlPath.GetCount(); i++)
		{
			m_arlPath.Add(src.m_arlPath[i]);
		}

		m_paramsearch = src.m_paramsearch;

		m_bNobody = src.m_bNobody;

		return *this;
	}	
}ETObjSearch,*lpETObjSearch;

class AFX_EXT_CLASS ETFullTextSearchData
{
public:
	typedef vector<CString> SiteVector;
	typedef vector<CString>::const_iterator SiteIterator;

	ETFullTextSearchData() : _searchText(TEXT(""))
	{
	}

	~ETFullTextSearchData()
	{
	}

	void SetSearchText(const CString text)
	{
		_searchText = text;
	}

	const CString& GetSearchText()
	{
		return _searchText;
	}

	void ClearSites()
	{
		_sites.erase(_sites.begin(), _sites.end());
	}

	void AddSite(const CString &siteName)
	{
		_sites.push_back(siteName);
	}

	SiteIterator SiteBegin()
	{
		return _sites.begin();
	}

	SiteIterator SiteEnd()
	{
		return _sites.end();
	}

private:
	ETFullTextSearchData (const ETFullTextSearchData&);
	ETFullTextSearchData& operator =(const ETFullTextSearchData&);

private:
	CString _searchText;
	SiteVector _sites;
};


class CETObjectCollection
{
public:
	typedef CArray<lpETObject, lpETObject>							ItemArray;
	typedef CMap<lpETObject, lpETObject, lpETObject, lpETObject>	ItemMap;

public:
	CETObjectCollection(void)
	{
		m_arrayItems.SetSize(0, 128);
		m_lContentType	= 0;
		m_lSource		= 0;
	}
	~CETObjectCollection(void)
	{
		RemoveAll();
	}

	inline void Add(lpETObject pItem)
	{
		m_arrayItems.Add(pItem);
	}

	inline void Append(const ETArrayObject &arObject)
	{
		m_arrayItems.Append(arObject);
	}

	inline void InsertAt(int nIndex, lpETObject pItem)
	{
		m_arrayItems.InsertAt(nIndex, pItem);
	}

	inline void AddReference(lpETObject pItem)
	{
		m_mapAttached[pItem]	= pItem;
		m_arrayItems.Add(pItem);
	}

	inline void Delete(int nIndex)
	{
		lpETObject		pItem;
		if( nIndex < m_arrayItems.GetCount())
		{
			//	�ڶ�������ʱ���ͷ���Attach��ʽ��ӵ�Ԫ��
			if(! m_mapAttached.Lookup(m_arrayItems[nIndex], pItem))		
			{
				pItem = m_arrayItems[nIndex];
				if(NULL != pItem) //added by xiashaochao, 2006.11.1
				{
					delete pItem;

				}
				m_arrayItems.RemoveAt(nIndex);
			}
			else
			{
				m_mapAttached.RemoveKey(m_arrayItems[nIndex]);
				m_arrayItems.RemoveAt(nIndex);
			}
		}
	}

	inline void DetachAll()
	{
		m_arrayItems.RemoveAll();
		m_mapAttached.RemoveAll();
	}

	inline lpETObject operator [](int nIndex) const
	{
		return m_arrayItems[nIndex];
	}
	
	
	inline void RemoveAll()
	{
		int			nCount = (int)m_arrayItems.GetCount();
		lpETObject	pItem;

		for ( int i = 0; i < nCount; i ++ )
		{
			if ( ! m_mapAttached.Lookup(m_arrayItems[i], pItem))
				if(NULL != m_arrayItems[i])  //added by xiashaochao ,2006.11.1
				{
					delete m_arrayItems[i];

				}
		}

		m_arrayItems.RemoveAll();
		m_mapAttached.RemoveAll();
		m_lContentType	= 0;
		m_lSource		= 0;
	}
	// ��õ�ǰ���������ж�����б�,����ָ���ķָ����ָ�
	inline CString GetObjectNameList(CString strSplit = _T("\n"))  const
	{
		CString strNameList = _T("");
		if(GetCount()>0)
		{
			CString strItem;
			for(int i=0;i<GetCount();i++)
			{
				if(30 == i)
				{
					strNameList = strNameList + strSplit + _T("...");
					break;
				}
				strItem = (*this)[i]->m_strObjectName;
				if(strNameList.IsEmpty())
					strNameList = strItem;
				else
					strNameList = strNameList + strSplit + strItem;
			}

			strNameList.TrimRight(strSplit);

			return strNameList;
		}
		else
			return _T("");
	}
	// ��������Դ
	void SetSource(long lSource = ET_DRAG_SOURCE_UNKNOWN) {m_lSource = lSource;}
	// �޸�����Դ
	long GetSource(void){return m_lSource;}

public:
	inline int GetCount() const
	{
		return (int)m_arrayItems.GetCount();
	}

protected:
	ItemArray		m_arrayItems;	//	���ڴ洢���нڵ�����
	ItemMap			m_mapAttached;	//	���ڴ洢����ͨ��Attach��ʽ��ӵ�Ԫ��ӳ��

private:
	long			m_lSource;
	long			m_lContentType;
};


//�ص��������Ͷ���
#define ET_FUNPTR_NLE_CALLBACKFORML						1	//�Ǳ�ص�����
#define ET_FUNPTR_PARSE_DRAGPROTOCOLINFO				2	//��קЭ�����
#define ET_FUNPTR_SELECTPATH_TREE						5	//ȡѡ��·����
#define ET_FUNPTR_SELECTPEPATH_TREE						6	//ȡѡ��·����
#define ET_FUNPTR_DOWNLOADINFO_TREE						7   //


//�ص���������
typedef long (WINAPI *InvokeWSMethodByXMLExPtr)(BSTR lpEndPointURL, BSTR lpSoapAction, BSTR lpXmlContent,BOOL bOutputSoapMsg, BSTR *  lpResult);
typedef BOOL (WINAPI *ETParseDragProtocolInfoPtr)(CString strXml, ETArrayObject & arObjectInfo, BOOL bHasExtInfo);
typedef BOOL (WINAPI *ETSelectPathTreePtr)(CWnd* pParentWnd, long &lFolderID,CString &strSelectPath,BOOL bAllowOA);
typedef BOOL (WINAPI *ETSelectPEPathTreePtr)(CWnd* pParentWnd, long &lFolderID,CString &strSelectPath);
typedef BOOL (WINAPI *ETSelectDownloadInfoPtr)(CWnd* pParentWnd, long &lFolderID,CString& strTitle, CString &strSelectPath, CString &strRights, CString &strComment, BOOL bMultiClip);



