#pragma once

#include <afxtempl.h>




#define TP_CONTROL_GROUPNUM     4
#define TP_CONTROL_CTRLSIZE     CRect(0,0,32,32)
#define TP_CONTROL_CTRLSIZEEX     CSize(32,32)
 

#define TP_CONTROL_GENERALBUTTON (WS_VISIBLE|WS_CHILD|WS_TABSTOP)    
#define TP_CONTROL_CHECKBUTTON (WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX|BS_PUSHLIKE)    


#define TP_NSCONTROL_NULL      0x00000000
#define TP_NSCONTROL_HOTKEY    0x00000001
#define TP_NSCONTROL_BUTTON    0x00000002

#define TP_CONTROL_INDEX_PLAY    0
#define TP_CONTROL_INDEX_EDIT    1
#define TP_CONTROL_INDEX_SPECIAL 2
#define TP_CONTROL_INDEX_GENERAL 3


#define TP_CONTROL_IDDEN_BASE_PLAY    1000
#define TP_CONTROL_IDDEN_BASE_EDIT    2000
#define TP_CONTROL_IDDEN_BASE_SPECIAL 3000
#define TP_CONTROL_IDDEN_BASE_GENERAL 4000


#define TP_CONTROL_ID_BASE_PLAY    1000
#define TP_CONTROL_ID_BASE_EDIT    2000
#define TP_CONTROL_ID_BASE_SPECIAL 3000
#define TP_CONTROL_ID_BASE_GENERAL 4000


#define TPCONTROLDATA_CLASSTYPE     40 
#define TPCONTROLDATA_FACERES       MAX_PATH
#define TPCONTROLDATA_TEXT          40
#define TPCONTROLDATA_NOTIFYUP      80
#define TPCONTROLDATA_NOTIFYDOWN    80

typedef struct tag_TPNSControlData
{ 
	DWORD       dwIdentifer;         //唯一标识 在整个表中
	UINT		nID;		          //ID值
	UINT        vkChar;               //快捷键 VK_A
	UINT        uFlag;	              //TP_SHIFT TP_CTRL TP_ALT 
	DWORD		dwStyle;	          //属性		
	DWORD		dwStyleEx;	          //扩展属性	
	TCHAR       sClassType[TPCONTROLDATA_CLASSTYPE];       //控件类型 WC_BUTTON 
	TCHAR		sFaceRes[MAX_PATH];   //资源名称 图片位置	
	TCHAR		sText[TPCONTROLDATA_TEXT];            //文本内容 文本 Button Edit 等上面写的字
	TCHAR       sNotifyUp[TPCONTROLDATA_NOTIFYUP];        //按纽等 抬起时的提示信息 
	TCHAR       sNotifyDown[TPCONTROLDATA_NOTIFYDOWN];      //按下时的提示信息 
	DWORD       dwState;              //缩定 隐藏标志  TP_CONTROL_DOUBLESIZE
	RECT        rtCtrl;               // 按钮大小
	DWORD       dwReserved;           //保留
}TPNSControlData,*LPTPNSControlData;



typedef struct tag_TPNSWndControlData
{
	CString          sGuiPluginName;  //GUI插件名称
	CString          sWndName;  //界面名称（英文通用名称，用于索引）
	CString          sWndNameDesc;  //界面名称（字典名称，用于显示）

	GUID             guidWnd;   //GUID

	long             lCtrlType; //按钮和快捷键数量等不同，需要分别设置,有些只支持快捷键设置
//#define TP_CONTROL_HOTKEY    0x00000001
//#define TP_CONTROL_BUTTON    0x00000002

	DWORD            dwCtrlIdentiferNum[TP_CONTROL_GROUPNUM];   //按钮4个组信息
	DWORD           *pDwCtrlGroupIdentifer[TP_CONTROL_GROUPNUM];//按钮4个组信息-个数
	

	DWORD            dwHotkeyIdentiferNum[TP_CONTROL_GROUPNUM];//快捷键4个组信息
	DWORD           *pDwHotkeyGroupIdentifer[TP_CONTROL_GROUPNUM];//快捷键4个组信息-个数
	
	POINT            *pDwIdentifer;  //ID对应组
	DWORD            dwCount;

	long            lControlSize;  //按钮
	TPNSControlData *pControlData; //每个ID或identifier只能有一个
	

	long            lHotkeySize;  //快捷键
	TPNSControlData *pHotkeyData;  //每个ID或identifier可以有多个,即同一个按钮可以由多个快捷键支持
	
	//按钮和快捷键数量等不同，需要分别设置

	long            lToolBarCol;     //工具栏支持的列
	long            lToolBarRow;     //工具栏支持的行



	BOOL            bChangeHotKey;  /////临时
	BOOL            bChangeCtrl;

}TPNSWndControlData,*LPTPNSWndControlData;

typedef CArray<TPNSControlData*,TPNSControlData*&> TPNSControlDataArr;
typedef CArray<TPNSWndControlData*,TPNSWndControlData*&> TPNSWndControlDataArr;
