#pragma once

#include <afxtempl.h>

#include "..\src\TPCommonCtrl\TPCommonCtrlData.h"
#include "TPCommonCtrlNSData.h"

//窗口类型
#define	TP_WND_TOOLBAR      0x00000001
#define	TP_WND_CONTROLBAR   0x00000002
#define	TP_WND_CHLD         0x00000004
#define	TP_WND_FRAME        0x00000008
#define	TP_WND_TABFRAME     0x00000010
#define TP_WND_DIALOG       0x00000020
#define TP_WND_PROPERTY     0x00000100

#undef    HTPWND  
#undef    HTPDATA  
#define   HTPWND  void *
#define   HTPDATA  void *

typedef CArray<HTPWND, HTPWND &> HTPWNDARRAY ;
typedef CArray<HTPDATA, HTPDATA &> HTPDATAARRAY ;

#undef  S_ACCEPT
#undef  S_DECLINE
#define S_ACCEPT          0x00000002
#define S_DECLINE         0x00000004

#define TP_NSCtrlData_Edit_NULL   0x00000000
#define TP_NSCtrlData_Edit_BUTTON 0x00000001
#define TP_NSCtrlData_Edit_HOTKEY 0x00000002

typedef struct tag_TP_ControlData_Desc
{
	long nCount;
	TPControlData* pCtrlData;
	CString strResPath;		//TPSkin下的相对路径
	tag_TP_ControlData_Desc* pNext;
}TP_ControlData_Desc,*LPTP_ControlData_Desc;

//考虑到某一个界面可能包含几组快捷键定义
typedef struct tag_TPGuiHotKeyStu
{
	GUID    guidGroup;
	CString strHotKeyGroup;
	long     iHotKeyNum;
	TPHotKeyData* pHotKeyData;
	TP_ControlData_Desc* pControlData;		//与该HotkeyData相关的ControlData

	//tag_TPGuiHotKeyStu()
	//{
	//	guidGroup = GUID_NULL;
	//	strHotKeyGroup = _T("");
	//	iHotKeyNum = 0;
	//	pHotKeyData = NULL;
	//}
}TPGuiHotKeyStu,*LPTPGuiHotKeyStu;

typedef CArray<TPGuiHotKeyStu*, TPGuiHotKeyStu*> TPGuiHotKeyStuArray;


typedef struct tag_TPGuiTPControlDataStu
{
	GUID    guidGroup;  //保留
	CString strGroup;  //英文通用名称，用于索引
	CString strGroupDesc;//字典名称，用于显示
	CString strResPath; //资源路径,不包含sys\tpskin,例如material_preview\extwnd
	CSize    szButton;//保留 
	long     iControlDataNum;
	TPControlData* pControlData;

	long iDefControlUsed;	//默认可见的控件个数,-1表示不限制
	enum e_CtrlContainerType{normalWindow = 0, scrolledToolbar, fixedToolPanel};
	e_CtrlContainerType eCtrlContainerType;	//控件容器类型


}TPGuiTPControlDataStu,*LPTPGuiTPControlDataStu;

typedef CArray<TPGuiTPControlDataStu*, TPGuiTPControlDataStu*> TPGuiTPControlDataStuArray;





typedef struct tag_TPEdit_Data //当前处理数据
{
	ULONGLONG     dwDataType;   //数据类型	
	DWORD         dwChangeType; //变化类型
	HTPDATA       hEditData;    //当前正在编辑数据	
	GUID          guidEditData;
	LPARAM        lParam;

	tag_TPEdit_Data()
	{
		dwDataType   = 0; 
		hEditData    = NULL;    	
		guidEditData = GUID_NULL;
		lParam       = NULL;
	}
	tag_TPEdit_Data(ULONGLONG    dwInitDataType,HTPDATA   hInitEditData)
	{
		dwDataType = dwInitDataType;
		hEditData  = hInitEditData;
		lParam     = NULL;
	}	
}TPEditData,*LPTPEditData;

typedef CArray<TPEditData ,TPEditData &> TPEditDataArray; 


typedef struct tag_TPMenu_Data //菜单数据
{
	CString                                     sGuiType;        //菜单来自哪个窗口接口
	CString                                     sMenuLabel;      //菜单显示
	CString                                     sMenuCommond;    //发送的命令
	DWORD                                       dwMenuData;      //临时参数
	GUID                                        guidData;        //临时参数
	CArray<tag_TPMenu_Data ,tag_TPMenu_Data &>  arrSubMenu;      //子菜单 
	tag_TPMenu_Data()
	{
		sGuiType     = _T("");    
		sMenuLabel   = _T("");  
		sMenuCommond = _T("");
		dwMenuData   = 0;  
		guidData     = GUID_NULL;
	}
	~tag_TPMenu_Data()
	{
		arrSubMenu.RemoveAll();
	}
}TPMenuData,*LPTPMenuData;

typedef CArray<TPMenuData ,TPMenuData &> TPMenuDataArray;


typedef struct tag_TPUIUserSettingInfo
{
    CString strGuiPlugIn;//gui plugin name;
    GUID guidUI; //唯一标识
    CString strGeneralName; //通用名称(english)
    CString strName;  //名称（从字典中取）
}TPUIUserSettingInfo,*LPTPUIUserSettingInfo;


typedef CArray<LPTPUIUserSettingInfo ,LPTPUIUserSettingInfo&> TPUIUserSettingArray;



typedef struct tag_TPGuiShowStu
{
	CString strGuiName;
	CString strWndType;
	long    lFrameLevel;
	long    lFrameIndex;
	CRect   rtWndPos;  
	long    lAttachType;
	BOOL    bShow;
	UINT    uState;

	tag_TPGuiShowStu()
	{
		strGuiName = _T("");
		strWndType = _T("");
		lFrameLevel = 1;
		lFrameIndex = 0;
		rtWndPos = CRect(0,0,100,100);  
		lAttachType = 0;
		bShow = TRUE;
		uState = 0;
	};
}TPGuiShowStu,*LPTPGuiShowStu;


typedef struct tag_TPPluginGui_Fun  //窗口提供的回调函数
{
	//移动等窗口操作
	HTPWND  (*TP_Create)(CWnd *pParent,CWnd *pOwner,CRect rtInit);
	BOOL    (*TP_DestroyWindow)(HTPWND hTPWnd);
	void    (*TP_MoveWindow)(HTPWND hTPWnd,CRect rtWnd);
	void    (*TP_MoveWindowEx)(HTPWND hTPWnd,CSize szPos);
	void    (*TP_ShowWindow)(HTPWND hTPWnd,UINT swType);
	BOOL    (*TP_ShowWindowEx)(HTPWND hTPWnd,UINT swType);

	void    (*TP_EnableWindow)(HTPWND hTPWnd,BOOL bEnable);
	void    (*TP_SetWindowText)(HTPWND hTPWnd,LPTSTR strText);
	LPTSTR  (*TP_GetWindowText)(HTPWND hTPWnd);
	BOOL    (*TP_SizingWindow)(HTPWND hTPWnd,UINT fwSide,LPRECT lpRect);
	BOOL    (*TP_GetMinMaxSizeInfo)(HTPWND hTPWnd,MINMAXINFO& wndSizeInfo);
	BOOL    (*TP_GetDefaultSizeInfo)(HTPWND hTPWnd,int& iWidth,int& iHeight);
	

	void    (*TP_SetActivate)(HTPWND hTPWnd ,BOOL bActivate);
	void    (*TP_SetFous)(HTPWND hTPWnd);
	void    (*TP_SetFrameIndex)(int iIndex);


	//数据通知机制
	HTPWND (*TP_SetViewData)(void* pManageData,TPEditDataArray *pEditDataArray);            // 设置编辑数据
	BOOL   (*TP_UpdateView)(HTPWND hWnd,DWORD dwCommond,void*  pManageData,LPARAM lParam);   // 修改编辑数据
	void   (*TP_DeleteData)(HTPWND hWnd,void*  pManageData,TPEditDataArray *pArrData);       // 删除或停止编辑数据
	void   (*TP_SaveData)(HTPWND hWnd,void*  pManageData,TPEditDataArray *pArrData);         // 保存数据


	//询问是否能够编辑的菜单
	void   (*TP_GetEditMenu)(void*  pManageData,TPEditDataArray *pArrData,TPMenuDataArray *pMenuArray);      
	HTPWND (*TP_SendMenuCommond)(DWORD dwMenuCommond,void*  pManageData,TPEditDataArray *pArrData);



	//编辑和播放
	void   (*TP_SetRendState)(void* pState);
	void   (*TP_SetRendPosVU)(HTPWND hWnd,long lPos,void* pVUTable);
	void   (*TP_SetRendVU)(HTPWND hWnd,void* pEditData,long lPos,void* pVUTable);//发VU表特殊函数


	void   (*TP_SetEngineState)(void* pEditData,void* pState);
	void   (*TP_SetEnginePosVU)(HTPWND hWnd,void* pEditData,long lPos,void* pVUTable);
	void   (*TP_SetEnginePosVUScrub)(PVOID pData); ////@@IFrameScrum
	//释放
	void    (*TP_Release)();    //释放本类窗口 
	//TAB
	CString (*TP_GetTabLabel)(HTPWND hTPWnd);
	//拖放操作
	LRESULT (*TP_Dropping)(ULONGLONG dwDropType,LPARAM lParam);
	LRESULT (*TP_Drop)(ULONGLONG dwDropType,LPARAM lParam);

	//键盘和鼠标消息
	LRESULT (*TP_OnKeyDown)(HTPWND hWnd,UINT uChar,UINT uFlag);   //如果处理了 返回S_OK
	LRESULT (*TP_OnKeyUp)(HTPWND hWnd,UINT uChar,UINT uFlag);   //如果处理了 返回S_OK
	LRESULT (*TP_OnChar)(HTPWND hWnd,UINT nChar,UINT nRepCnt,UINT nFlags);   //如果处理了 返回S_OK

	LRESULT (*TP_SaveState)(LPBYTE &pBuffer,int &iBufLength);    //取得退出时的各种状态 
	LRESULT (*TP_ResumeState)(LPBYTE pBuffer,int iBufLength);      //设置退出时的各种状态




	LRESULT (*TP_GdiCallBack)(DWORD dwCmd,LPARAM lParam); 

	LRESULT (*TP_SetCustomHotKey)(int iHotKeyGroupNum,LPTPGuiHotKeyStu* pHotKeyStu);
	LRESULT (*TP_GetDefaultHotKey)(int& iHotKeyGroupNum,LPTPGuiHotKeyStu& pHotKeyStu);
	LRESULT (*TP_GetSysHotKey)(int& iHotKeyGroupNum,LPTPGuiHotKeyStu& pHotKeyStu);  //不允许用户设置
	LRESULT (*TP_GetOneDefaultHotKey)(LPCTSTR lptszGroupName,UINT  nID,TPHotKeyData* pHotKeydata);


	LRESULT (*TP_SetCustomCtrlData)(int iCtrlDataNum,LPTPGuiTPControlDataStu* pCtrlDataStu);
	LRESULT (*TP_GetDefaultCtrlData)(int& iGroupNum,LPTPGuiTPControlDataStu& pCtrlDataStu);
	LRESULT (*TP_GetAllCtrlData)(int& iGroupNum,LPTPGuiTPControlDataStu& pCtrlDataStu);  //目前不


	LRESULT (*TP_GetNSCtrlData)(long& iGroupNum,TPNSWndControlData*& pCtrlDataStu);
	LRESULT (*TP_SetNSCtrlData)(long iGroupNum,TPNSWndControlData* pCtrlDataStu,long lEditType);//设置时候请自行拷贝
	//#define TP_NSCtrlData_Edit_NULL   0x00000000
	//#define TP_NSCtrlData_Edit_BUTTON 0x00000001
	//#define TP_NSCtrlData_Edit_HOTKEY 0x00000002




	LRESULT (*TP_SaveUserState)(LPBYTE &pBuffer,int &iBufLength);    //取得退出时的各种状态 
	LRESULT (*TP_ResumeUserState)(LPBYTE pBuffer,int iBufLength);      //设置退出时的各种状态
	LRESULT (*TP_ShowSysMenu)(HTPWND hTPWnd,CPoint &ptScreen);
	LRESULT (*TP_DoHelp)(HTPWND hTPWnd);




//界面设置user函数
    LRESULT (*TP_GetUserSetInfo)(long& lGroupNum,TPUIUserSettingInfo*& pSetInfo);//获取有几个设置模块
    LRESULT (*TP_SetDataToUserSetInfo)(GUID guidSetInfo,long lDataSize,BYTE* pUserData);//设置数据
	LRESULT (*TP_SetDataToUserSetComment)(GUID guidSetInfo,LPCTSTR sComent);//设置数据
    LRESULT (*TP_GetDataFromUserSetInfo)(GUID guidSetInfo,long& lDataSize,BYTE*& pUserData);//获取数据
    LRESULT (*TP_ResetUserSetInfo)(GUID guidSetInfo);  //reset,相应画面要刷新
    LRESULT (*TP_ResetUserSetInfoEx)(GUID guidSetInfo,long& lBufSize,BYTE*& pUserData);  //reset,相应不要刷新，将传进去的数据改成缺省值
    LRESULT (*TP_SetUserSetInfo)(GUID guidSetInfo);  //打开调整对话框
	LRESULT (*TP_SetUserSetInfoEx)(GUID guidSetInfo,long& lBufSize,BYTE*& pUserData);  //打开调整对话框,修改数据并返回，但不刷新界面

//界面设置user函数

	LRESULT (*TP_ShowSubGUI)(TPGuiShowStu* pGuiShowStu);
	BOOL    (*TP_GetIsCanShow)();

	//added chenxiaolong 2010.5.7 for bug XN00028757
	BOOL	(*TP_CheckExistWnd)(BOOL bWarn,LPTSTR strWndType);

	tag_TPPluginGui_Fun()
	{
		TP_Create           = NULL;
		TP_DestroyWindow    = NULL;
		TP_MoveWindow       = NULL;
		TP_MoveWindowEx     = NULL;
		TP_ShowWindow       = NULL;
		TP_ShowWindowEx     = NULL;
		TP_EnableWindow     = NULL;
		TP_SetWindowText    = NULL;
		TP_GetWindowText    = NULL;
		TP_SizingWindow     = NULL;
		TP_GetMinMaxSizeInfo = NULL;
		TP_GetDefaultSizeInfo = NULL;

		TP_SetActivate      = NULL;
		TP_SetFous          = NULL;	
		TP_SetFrameIndex	= NULL;

		TP_UpdateView       = NULL;
		TP_SetViewData      = NULL;
		TP_DeleteData       = NULL;
		TP_SaveData         = NULL;		

		TP_GetEditMenu      = NULL;
		TP_SendMenuCommond  = NULL;

		TP_SetRendState     = NULL;
		TP_SetRendPosVU     = NULL;
		TP_SetRendVU        = NULL;

		TP_SetEngineState   = NULL;
		TP_SetEnginePosVU   = NULL;
		TP_SetEnginePosVUScrub = NULL ;//@@IFrameScrum


		TP_Release          = NULL;  

		TP_GetTabLabel      = NULL;

		TP_Dropping         = NULL;
		TP_Drop             = NULL;

		TP_OnKeyDown        = NULL;  
		TP_OnKeyUp          = NULL;
		TP_OnChar           = NULL;

		TP_SaveState        = NULL;
		TP_ResumeState      = NULL;

		TP_GdiCallBack      = NULL;

		TP_SetCustomHotKey    =  NULL;
		TP_GetDefaultHotKey   =  NULL;
		TP_GetOneDefaultHotKey = NULL;
		TP_GetSysHotKey = NULL;

		TP_SetCustomCtrlData = NULL;
		TP_GetDefaultCtrlData = NULL;
		TP_GetAllCtrlData = NULL;


		TP_SaveUserState         = NULL;
		TP_ResumeUserState       = NULL;
		TP_ShowSysMenu           = NULL;

		TP_GetNSCtrlData      = NULL;
		TP_SetNSCtrlData      = NULL;

        //界面设置user函数
        TP_GetUserSetInfo = NULL;
        TP_SetDataToUserSetInfo = NULL;
		TP_SetDataToUserSetComment = NULL;
        TP_GetDataFromUserSetInfo = NULL;
        TP_ResetUserSetInfo = NULL;
        TP_ResetUserSetInfoEx = NULL;
        TP_SetUserSetInfo = NULL;
		TP_SetUserSetInfoEx = NULL;
        //界面设置user函数

		TP_ShowSubGUI = NULL;
		TP_GetIsCanShow = NULL;
		TP_DoHelp = NULL;

		TP_CheckExistWnd=NULL;

	}
}TPPluginGuiFun ,*LPTPPluginGuiFun;



typedef struct tag_TPPluginGui_Data //窗口必须提供数据
{
	GUID                 guidWndType;         //窗口GUID 
	BOOL                 bBindManageData;    //是否和数据中心当前数据绑定
	BOOL                 bSendCenterCommond; //是否发送中心数据改变命令 
	BOOL                 bCanNew;            //允许菜单项出现新建 
	CString              strGuiName;         //窗口名
	DWORD                dwCapWndType;       //帽子窗口类型
	BOOL                 bDuplicate;         //是否允许多窗口
	BOOL                 bDestroyExist;      //关闭时是否删除窗口
	BOOL                 bActCap;            //创建的是否是否允许激活帽子
	BOOL                 bGetCustomPos;
	TPPluginGuiFun  	 funPluginGui;       //回调函数

	void Reset()
	{
		guidWndType        = GUID_NULL;
		bBindManageData    = TRUE;
		bSendCenterCommond = FALSE;
		bCanNew            = FALSE; 
		strGuiName         = _T("");
		dwCapWndType       = 0;   
		bActCap            = TRUE;
		bDuplicate         = FALSE;  
		bDestroyExist      = FALSE;
		bGetCustomPos      = FALSE;
	}

	tag_TPPluginGui_Data()
	{
		bBindManageData    = TRUE;
		bSendCenterCommond = FALSE;
		bCanNew            = FALSE; 
		strGuiName         = _T("");
		dwCapWndType       = 0;    
		bDuplicate         = FALSE;
		bDestroyExist      = FALSE;
		bActCap            = TRUE;
		bGetCustomPos      = FALSE;
	}

	~tag_TPPluginGui_Data()
	{
		Reset();
	}


}TPPluginGuiData,*LPTPPluginGuiData;
typedef CArray<LPTPPluginGuiData ,LPTPPluginGuiData &> TPPluginGuiDataArray;


typedef struct tag_TPEdit_Wnd //编辑的窗口
{
	BOOL                 bActivated;    //是否当前激活窗口 
	HTPWND               hTPWnd;        //窗口	
	BOOL                 bSendCmd;      
	TPPluginGuiFun      *pPluginGuiFun; //窗口所属回调函数
	TPEditDataArray      arrEditData;   //当前正在编辑数据
	void*                pManageData;   //编辑数据所属文档
	TPPluginGuiData*     pPluginGuiData;
	void*                pPrivateData;
	tag_TPEdit_Wnd()
	{
		bActivated    = FALSE;
		bSendCmd      = TRUE;
		hTPWnd        = NULL;
		pPluginGuiFun = NULL;
		pManageData   = NULL;
		pPrivateData  = NULL;
		pPluginGuiData = NULL;
	}
	tag_TPEdit_Wnd& operator =(tag_TPEdit_Wnd &src)
	{
		hTPWnd        = src.hTPWnd;
		pPluginGuiFun = src.pPluginGuiFun;
		pManageData   = src.pManageData;
		pPrivateData  = src.pPrivateData;
		pPluginGuiData = src.pPluginGuiData;
		arrEditData.Copy(src.arrEditData);
		return *this;
	}
	~tag_TPEdit_Wnd()
	{
		arrEditData.RemoveAll();
	}
}TPEditWnd,*LPTPEditWnd;

typedef CArray<TPEditWnd ,TPEditWnd &> TPEditWndArray; 


typedef struct tag_TPCapGui_Fun //帽子窗口提供的回调函数
{
	HTPWND  (* TP_GetCapWindow)(DWORD dwWndType,BOOL bCreateChild,LPTSTR strGuiName,LPTSTR strCapType,BOOL bCreateCap,BOOL bFromTemplate);
	void    (* TP_DestroyCapWindow)(HTPWND hTPWnd);
	void    (* TP_ShowCapWindow)(HTPWND hTPWnd,UINT nShowType);
	void    (* TP_MoveCapWindow)(HTPWND hTPWnd,RECT rtWnd);
	void    (* TP_EnableCapWindow)(HTPWND hTPWnd,BOOL bEnable);
	void    (* TP_SetCapWindowText)(HTPWND hTPWnd,LPTSTR strText);
	LPTSTR  (* TP_GetCapWindowText)(HTPWND hTPWnd);	
	BOOL    (* TP_SetCurSel)(HTPWND hTPWnd);			
	BOOL    (* TP_SetLockItem)(HTPWND hTPWnd,int iItemIndex,BOOL bLock);			

	int     (* TP_DialogDoModal)(HTPWND hTPWnd);
	LRESULT (* TP_GdiCallBack)(HTPWND hTPWnd,DWORD dwCmd,LPARAM lParam); 

	LRESULT (*TP_GetSysHotKey)(long& iHotKeyGroupNum,LPTPGuiHotKeyStu& pHotKeyStu);  //不允许用户设置
	LRESULT (*TP_SetCustomHotKey)(long iHotKeyGroupNum,LPTPGuiHotKeyStu* pHotKeyStu);
	LRESULT (*TP_GetDefaultHotKey)(long& iHotKeyGroupNum,LPTPGuiHotKeyStu& pHotKeyStu);

	LRESULT (*TP_GetNSCtrlData)(long& iGroupNum,TPNSWndControlData*& pCtrlDataStu);
	LRESULT (*TP_SetNSCtrlData)(long iGroupNum,TPNSWndControlData* pCtrlDataStu,long lEditType);//设置时候请自行拷贝
//#define TP_NSCtrlData_Edit_NULL   0x00000000
//#define TP_NSCtrlData_Edit_BUTTON 0x00000001
//#define TP_NSCtrlData_Edit_HOTKEY 0x00000002


	LRESULT (*TP_SaveState)(LPBYTE &pBuffer,int &iBufLength);    //取得退出时的各种状态 
	LRESULT (*TP_ResumeState)(LPBYTE pBuffer,int iBufLength);      //设置退出时的各种状态

	LRESULT (*TP_SetEditUITemplate)(long lEditType,long lIndex,BOOL bShowOtherWin);
	LRESULT	(*TP_SaveLastScene)();

	//键盘和鼠标消息,通过该函数，某个窗口可以调用其他类型窗口的hotkey处理
	LRESULT (*TP_KeyDown)(LPTSTR strGuiName,UINT uChar,UINT uFlag);   //如果处理了 返回S_OK
	LRESULT (*TP_KeyUp)(LPTSTR strGuiName,UINT uChar,UINT uFlag);   //如果处理了 返回S_OK


	LRESULT (*TP_ReleaseGui)();
	//拖放操作
	LRESULT (*TP_Dropping)(ULONGLONG dwDropType,LPARAM lParam);
	LRESULT (*TP_Drop)(ULONGLONG dwDropType,LPARAM lParam);

	void    (*TP_LogNotify)(LPCTSTR lpLogInfo,WPARAM wParam,LPARAM lParam,LPVOID lpCallbackParam);
	HTPWND  (*TP_FindCapWindow)(LPCTSTR strCapType);


	tag_TPCapGui_Fun()
	{
		TP_GetCapWindow       =  NULL;
		TP_DestroyCapWindow   =  NULL;
		TP_ShowCapWindow      =  NULL;
		TP_MoveCapWindow      =  NULL;
		TP_EnableCapWindow    =  NULL;
		TP_SetCapWindowText   =  NULL;
		TP_GetCapWindowText   =  NULL;
		TP_SetCurSel          =  NULL;
		TP_SetLockItem        =  NULL;

		TP_DialogDoModal      =  NULL;
		TP_GdiCallBack        =  NULL;

		TP_SaveState          =  NULL;
		TP_ResumeState        =  NULL;
		TP_ReleaseGui         =  NULL;
		TP_SetEditUITemplate  =  NULL;
		TP_Dropping           =  NULL;
		TP_Drop               =  NULL;
		TP_LogNotify		  =  NULL;
		TP_KeyDown			  =  NULL;	
		TP_KeyUp			  =  NULL;
		TP_GetSysHotKey       =  NULL;
		TP_SetCustomHotKey    =  NULL;
		TP_GetDefaultHotKey   =  NULL;
		TP_FindCapWindow	  =  NULL;
		TP_SaveLastScene      =  NULL;

		TP_GetNSCtrlData      = NULL;
		TP_SetNSCtrlData      = NULL;
		

	}

}TPCapGuiFun,*LPTPCapGuiFun;

typedef struct tag_TPDragDropData
{
	GUID			guidDataType;
	LPARAM         lParam;
}TPDragDropData;
typedef CArray<TPDragDropData ,TPDragDropData &> TPDragDropDataArray;

//基本拖放数据类型GUID定义

#define TP_DROPDATA_UNKNOW_GUID   GUID_NULL
#define TP_DROPDATA_VAEFF_GUID    { 0xeba5af14, 0x6166, 0x4452, { 0xa5, 0xed, 0x30, 0x3e, 0x1a, 0x68, 0xb1, 0x18 } }
#define TP_DROPDATA_CGEFF_GUID    { 0x4e4833c0, 0xb1dc, 0x4cee, { 0xb6, 0xbf, 0x2b, 0x9a, 0x10, 0x7c, 0x7, 0xa } }
#define TP_DROPDATA_VAEFFTEMPLATE_GUID  { 0x8d61b16d, 0x5f88, 0x4c3a, { 0x95, 0xb1, 0x75, 0xc8, 0xbb, 0x6a, 0x4e, 0x50 } }
#define TP_DROPDATA_CGTEMPLATE_GUID  { 0x23583c3e, 0x86aa, 0x495e, { 0x8c, 0x27, 0x87, 0x70, 0x16, 0x68, 0x95, 0xa0 } }
#define	TP_DROPDATA_CLIP_GUID     { 0x217993f, 0x92c, 0x47aa, { 0xa7, 0x88, 0x1f, 0xa5, 0x7, 0x52, 0xd2, 0x9e } }
#define	TP_DROPDATA_TIMELINE_GUID  { 0xcf0ab9a0, 0xc4f7, 0x42c8, { 0xbc, 0x31, 0x21, 0xac, 0x61, 0x2b, 0x2a, 0x90 } }
//#define	
//FILTER的数据类型GUID定义有FILTER插件管理提供
