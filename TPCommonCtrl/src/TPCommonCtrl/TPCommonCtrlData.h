//MessageRange 
//IDRange     
#pragma once



#include "..\..\include\TPCommonCtrlNSData.h"



#define TP_OWNERSET  0



enum TP_Control_State
{
	TP_Control_Hide   = 0x00000001,
	TP_Control_Lock   = 0x00000002,
	TP_Control_NoMove = 0x00000004,
	TP_Control_NoSize = 0x00000008,
};

#define TP_CONTROL_DOUBLE      0x00000001
#define TP_CONTROL_HIDE        0x00000002
#define TP_CONTROL_LOCK        0x00000004
#define TP_CONTROL_UNSET       0x00000008
#define TP_CONTROL_SELECT      0x00000010
#define TP_CONTROL_NOHIDE      0x00000020
#define TP_CONTROL_HIDEFORCE   0x00000040
#define TP_CONTROL_NOUSE       0x00000080 
#define TP_CONTROL_CONTINUES   0x00000100
#define TP_CONTROL_PUSHLIKE    0x00000200
#define TP_CONTROL_LINK        0x00000400
#define TP_CONTROL_NOALT       0x00000800
#define TP_CONTROL_NOCTRL      0x00001000
#define TP_CONTROL_NOSHIFT     0x00002000
#define TP_CONTROL_NOALTCTRL   0x00004000
#define TP_CONTROL_NOAPPLY     0x00008000
#define TP_CONTROL_DOUBLEBMP   0x00010000
#define TP_CONTROL_NOSTATE     0x00040000
#define TP_CONTROL_DRAG        0x00080000
#define TP_CONTROL_NOGLOBAL    0x00100000

#define TP_CONTROL_LINKCTRL    0x10000000
#define TP_CONTROL_NOCONTINUE  0x20000000
#define TP_CONTROL_STATEKEY    0x40000000

#define TPHOTKEYDATA_NAME      30
#define TPHOTKEYDATA_DESCRIP   60

typedef struct tag_TPHotKeyData
{
	DWORD          dwIndentify;
	UINT           nID;
	UINT           vkChar;
	UINT           uFlag;	
	TCHAR          sName[TPHOTKEYDATA_NAME];
	TCHAR          sDescrip[TPHOTKEYDATA_DESCRIP];

	DWORD          uState;   
	UINT           vkCharSet;
	UINT           uFlagSet;
	void          *pControlData;
}TPHotKeyData;

#ifndef TPCONTROLDATA_CLASSTYPE
#define TPCONTROLDATA_CLASSTYPE     40 
#define TPCONTROLDATA_FACERES       MAX_PATH
#define TPCONTROLDATA_TEXT          40
#define TPCONTROLDATA_NOTIFYUP      80
#define TPCONTROLDATA_NOTIFYDOWN    80
#endif

 
typedef struct tag_TPControlData
{ 
	DWORD       dwIndentifer;         //唯一标识 
	UINT		nID;		          //ID值
	DWORD		dwStyle;	          //	
	DWORD		dwStyleEx;	          //	
	TCHAR       sClassType[TPCONTROLDATA_CLASSTYPE];       //控件类型
	TCHAR		sFaceRes[TPCONTROLDATA_FACERES];   //资源名称	
	TCHAR		sText[TPCONTROLDATA_TEXT];            //文本内容
	RECT        rtCtrl;               //控件位置 
	DWORD       dwAlign;              //对齐方式

	TCHAR         sNotifyUp[TPCONTROLDATA_NOTIFYUP];      //按纽等 抬起时的提示信息 
	TCHAR         sNotifyDown[TPCONTROLDATA_NOTIFYDOWN];    //按下时的提示信息 
	DWORD         dwLinkIndentifer;   //配对唯一标识 当拖动控件时，所有配对的控件一起移动
	DWORD         dwState;            //缩定 隐藏标志  

	RECT          rtCtrlSet;          //控件位置 
	DWORD         dwAlignSet;         //对齐方式
	BOOL          dwStateSet;         //是否显示 

	WORD          wAlign;
	HWND          hWnd; 
	TPHotKeyData *pHotKeyData;
}TPControlData;

struct TPOSCtrlInterfce
{
	BOOL (*GetIsVista)();
	BOOL (*GetIsEffectByUAC)();
	BOOL (*GetUACStatus)();
	BOOL (*IsAffectedPath)(const CString &strPath);
	BOOL (*GetDWMStatus)(BOOL bForce);
	void (*SetDWMStatus)(BOOL bOpened);
	void (*GetProgramDataSysPath)(CString &strPath);
	void (*GetProgramDataAppPath)(CString &strPath);
	void (*GetProgramDataPath)(CString &strPath);
	void (*GetProgramDataTempPath)(CString &strPath);	

	TPOSCtrlInterfce()
	{
		ZeroMemory(this,sizeof(TPOSCtrlInterfce));
	}
};

AFX_EXT_API void TP_AssembledHotKeyControlData(int iCtrlData,TPControlData *pCtrlData,int iHotKey,TPHotKeyData *pHotKey);

#define  VK_0   0x30
#define  VK_1   0x31
#define  VK_2   0x32
#define  VK_3   0x33
#define  VK_4   0x34
#define  VK_5   0x35
#define  VK_6   0x36
#define  VK_7   0x37
#define  VK_8   0x38
#define  VK_9   0x39


#define  VK_A   0x41
#define  VK_B   0x42
#define  VK_C   0x43
#define  VK_D   0x44
#define  VK_E   0x45
#define  VK_F   0x46
#define  VK_G   0x47
#define  VK_H   0x48
#define  VK_I   0x49
#define  VK_J   0x4A
#define  VK_K   0x4B
#define  VK_L   0x4C
#define  VK_M   0x4D
#define  VK_N   0x4E
#define  VK_O   0x4F
#define  VK_P   0x50
#define  VK_Q   0x51
#define  VK_R   0x52
#define  VK_S   0x53
#define  VK_T   0x54
#define  VK_U   0x55
#define  VK_V   0x56
#define  VK_W   0x57
#define  VK_X   0x58
#define  VK_Y   0x59
#define  VK_Z   0x5A


#define  VK_Ctrl_1   5021


#define  TP_LSHIFT   0x01
#define  TP_RSHIFT   0x02
#define  TP_SHIFT    (TP_LSHIFT|TP_RSHIFT)
#define  TP_LCTRL    0x04
#define  TP_RCTRL    0x08
#define  TP_CTRL     (TP_LCTRL|TP_RCTRL)
#define  TP_CONTROL  TP_CTRL
#define  TP_LALT     0x10
#define  TP_RALT     0x20
#define  TP_ALT      (TP_LALT|TP_RALT)
#define  TP_MENU     TP_ALT
#define  TP_C2       0x40
#define  TP_C5       0x80

#define  TP_STATE      0x100
#define  TP_NUMLOCK    0x200
#define  TP_TEMP       0x400

#define  TP_ALLKEY   0xFFFFFFFF

#define  TP_CKEY    (TP_C2|TP_C5)

#define  VK_VNUM0   0x3000
#define  VK_VNUM1   0x3100
#define  VK_VNUM2   0x3200
#define  VK_VNUM3   0x3300
#define  VK_VNUM4   0x3400
#define  VK_VNUM5   0x3500
#define  VK_VNUM6   0x3600
#define  VK_VNUM7   0x3700
#define  VK_VNUM8   0x3800
#define  VK_VNUM9   0x3900

enum TP_DRAG_TYPE
{
	TP_DRAG_UNKNOW,
	TP_DRAG_RIGHT,
	TP_DRAG_RIGHTOK,
	TP_DRAG_LEFT,
	TP_DRAG_LEFTOK,
	TP_DRAG_TOP,
	TP_DRAG_TOPOK,
	TP_DRAG_BOTTOM,
	TP_DRAG_BOTTOMOK,
	TP_DRAG_MIDDLE,
	TP_DRAG_MIDDLEOK,
	TP_DRAG_BAR,
	TP_DRAG_BAROK,
	TP_DRAG_TRACKIN,
	TP_DRAG_TRACKINOK,
	TP_DRAG_TRACKOUT,
	TP_DRAG_TRACKOUTOK,
	TP_DRAG_TRIMIN,
	TP_DRAG_TRIMINOK,
	TP_DRAG_TRIMOUT,
	TP_DRAG_TRIMOUTOK,	
	TP_DRAG_QUICKASIDE,
	TP_DRAG_QUICKASIDEOK,
	TP_DRAG_QUICKBSIDE,
	TP_DRAG_QUICKBSIDEOK,
	TP_DRAG_QUICKATREFF,
	TP_DRAG_QUICKATREFFOK,
	TP_DRAG_QUICKBTREFF,
	TP_DRAG_QUICKBTREFFOK,
	TP_DRAG_QUICKABSIDE,
	TP_DRAG_QUICKABSIDEOK,
	TP_DRAG_QUICKSLIP,
	TP_DRAG_QUICKSLIPOK,	
	TP_DRAG_QUICKSLIDEEX,
	TP_DRAG_QUICKSLIDEEXOK,	
	TP_DRAG_QUICKSLIDE,
	TP_DRAG_QUICKSLIDEOK,	
	TP_DRAG_EFFIN,
	TP_DRAG_EFFINOK,
	TP_DRAG_EFFOUT,
	TP_DRAG_EFFOUTOK,
	TP_DRAG_STAYEFFIN,
	TP_DRAG_STAYEFFINOK,
	TP_DRAG_STAYEFFOUT,
	TP_DRAG_STAYEFFOUTOK,
	TP_DRAG_FLASHIN,
	TP_DRAG_FLASHINOK,
	TP_DRAG_FLASHOUT,
	TP_DRAG_FLASHOUTOK,
	TP_DRAG_TREFFIN,
	TP_DRAG_TREFFINOK,
	TP_DRAG_TREFFOUT,
	TP_DRAG_TREFFOUTOK,
	TP_DRAG_TIMELINE,
	TP_DRAG_TIMELINEOK,
	TP_DRAG_TRACKTOPING,
	TP_DRAG_TRACKTOP,
	TP_DRAG_TRACKBOTTOMING,
	TP_DRAG_TRACKBOTTOM,	
	TP_DRAG_TRACKSPLITING,
	TP_DRAG_TRACKSPLIT,	
	TP_DRAG_TRACKMOVING,
	TP_DRAG_TRACKMOVE,
	TP_DRAG_ITEMING,
	TP_DRAG_ITEM,
	TP_DRAG_ITEMSINGLE,
	TP_DRAG_ITEMOK,
	TP_DRAG_TRIMOK,
	TP_DRAG_FADEOK,
	TP_DRAG_FLASHOK,
	TP_DRAG_CLIPTROK,
	TP_DRAG_RECT,
	TP_DRAG_RECTOK,
	TP_DRAG_HEADRECT,
	TP_DRAG_HEADRECTOK,
	TP_DRAG_CLIPLEVEL,
	TP_DRAG_CLIPLEVELOK,
	TP_DRAG_PANEL,
	TP_DRAG_PANELOK,
	TP_DRAG_LEFTLINE,
	TP_DRAG_LEFTLINEOK,
	TP_DRAG_KEYFRAME,
	TP_DRAG_KEYFRAMEOK,
	TP_DRAG_KEYFRAMESEL,
	TP_DRAG_KEYFRAMESELOK,
	TP_DRAG_SINGLEFRAME,
	TP_DRAG_SINGLEFRAMEOK,
	TP_DRAG_EFFECTLIST,
	TP_DRAG_EFFECTLISTOK,
	TP_DRAG_TREFFECT,
	TP_DRAG_TREFFECTOK,
	TP_DRAG_MARKPOINT,
	TP_DRAG_MARKPOINTOK,
	TP_DRAG_SCENEIN,
	TP_DRAG_SCENEINOK,
	TP_DRAG_SCENEOUT,
	TP_DRAG_SCENEOUTOK,
	TP_DRAG_SCENEINOUT,
	TP_DRAG_SCENEINOUTOK,
	TP_DRAG_SONGIN        = 0x00010000,
	TP_DRAG_SONGINOK      = 0x00020000,
	TP_DRAG_SONGOUT       = 0x00040000,
	TP_DRAG_SONGOUTOK     = 0x00080000,
	TP_DRAG_SONGITEM      = 0x00100000,
	TP_DRAG_SONGITEMOK    = 0x00200000,
	TP_DRAG_SONGEFFIN     = 0x00400000,
	TP_DRAG_SONGEFFINOK   = 0x00800000,
	TP_DRAG_SONGEFFOUT    = 0x01000000,
	TP_DRAG_SONGEFFOUTOK  = 0x02000000,


	

	TP_DRAG_SONGITEMALL   = (TP_DRAG_SONGITEMOK|TP_DRAG_SONGITEM|TP_DRAG_SONGOUTOK|TP_DRAG_SONGOUT|TP_DRAG_SONGINOK|TP_DRAG_SONGIN|TP_DRAG_SONGEFFIN|TP_DRAG_SONGEFFINOK|TP_DRAG_SONGEFFOUT|TP_DRAG_SONGEFFOUTOK),
};
inline void TP_ExchangeControlData(TPNSControlData *pSrc,TPControlData *&pDest,int iCountSrc,DWORD &iCountDest,POINT *pDwIdentifer,DWORD dwCountIdentifer)
{
	if(pDest)
	{
		delete []pDest;
		pDest = NULL;
	}

	while(iCountSrc>=2 && (pSrc[iCountSrc-1].dwIdentifer <= 0 || pSrc[iCountSrc-1].dwIdentifer >= 32767 || pSrc[iCountSrc-1].nID>= 32767 )) iCountSrc--;

	if(iCountSrc >= 0)
	{
		pDest  = new TPControlData[iCountSrc+1];
		pDest[0].dwIndentifer = 0;
	}
	
	iCountDest = iCountSrc;
	for(INT l=0;l<iCountSrc;l++)
	{
		pDest[l].dwIndentifer     = pSrc[l].dwIdentifer;       
		pDest[l].nID              = pSrc[l].nID;		          
		pDest[l].dwStyle          = pSrc[l].dwStyle;	          
		pDest[l].dwStyleEx        = pSrc[l].dwStyleEx;	      
		lstrcpyn(pDest[l].sClassType,pSrc[l].sClassType,TPCONTROLDATA_CLASSTYPE);     
		lstrcpyn(pDest[l].sFaceRes,pSrc[l].sFaceRes,TPCONTROLDATA_FACERES); 
		lstrcpyn(pDest[l].sText,pSrc[l].sText,TPCONTROLDATA_TEXT);          
		lstrcpyn(pDest[l].sNotifyUp,pSrc[l].sNotifyUp,TPCONTROLDATA_NOTIFYUP);   
		lstrcpyn(pDest[l].sNotifyDown,pSrc[l].sNotifyDown,TPCONTROLDATA_NOTIFYDOWN);  
		pDest[l].dwState          = pSrc[l].dwState | TP_CONTROL_PUSHLIKE;   
		pDest[l].dwStateSet       = 0; 
		if(pSrc[l].dwStyleEx & TP_CONTROL_NOSTATE)
			pDest[l].dwState |= TP_CONTROL_NOSTATE;
		if(pSrc[l].dwStyleEx & TP_CONTROL_STATEKEY)
			pDest[l].dwState |= TP_CONTROL_STATEKEY;		
		for(DWORD k=0;k<dwCountIdentifer;k++)
		{
			if(pDwIdentifer[k].x == pSrc[l].dwIdentifer)
			{
				pDest[l].nID = pDwIdentifer[k].y;
				break;
			}
		}
	}
}

inline void TP_ExchangeHotKeyData(TPNSControlData *pSrc,TPHotKeyData *&pDest,int iCountSrc,DWORD &iCountDest,POINT *pDwIdentifer,DWORD dwCountIdentifer)
{
	if(pDest) 
	{
		delete []pDest;
		pDest = NULL;
	}

	while(iCountSrc>=2 && (pSrc[iCountSrc-1].dwIdentifer <= 0 || pSrc[iCountSrc-1].dwIdentifer >= 32767 || pSrc[iCountSrc-1].nID>= 32767 )) iCountSrc--;

	if(iCountSrc > 0)
	{
		pDest = new TPHotKeyData[iCountSrc];
	}
	
	iCountDest = iCountSrc;
	for(INT l=0;l<iCountSrc;l++)
	{	
		pDest[l].dwIndentify    = pSrc[l].dwIdentifer;
		pDest[l].nID            = pSrc[l].nID;
		pDest[l].vkChar         = pSrc[l].vkChar;
		pDest[l].uFlag          = pSrc[l].uFlag;	
		lstrcpyn(pDest[l].sName,pSrc[l].sText,TPHOTKEYDATA_NAME);
		lstrcpyn(pDest[l].sDescrip,pSrc[l].sNotifyUp,TPHOTKEYDATA_DESCRIP);
		pDest[l].uState        = pSrc[l].dwState;   
		pDest[l].vkCharSet     = pSrc[l].vkChar;
		pDest[l].uFlagSet       = pSrc[l].uFlag;
		if(pSrc[l].dwStyleEx & TP_CONTROL_NOSTATE)
			pDest[l].uState |= TP_CONTROL_NOSTATE;
		if(pSrc[l].dwStyleEx & TP_CONTROL_STATEKEY)
			pDest[l].uState |= TP_CONTROL_STATEKEY;		
		for(DWORD k=0;k<dwCountIdentifer;k++)
		{
			if(pDwIdentifer[k].x == pSrc[l].dwIdentifer)
			{
				pDest[l].nID = pDwIdentifer[k].y;
				break;
			}
		}
	}
}

#define          MF_LOADSTR    0x10000000L



#define  TP_WINDOW_SMALL         (0x00000000)
#define  TP_WINDOW_RIGHT         (0x00000001)
#define  TP_WINDOW_BOTTOM        (0x00000010)
#define  TP_WINDOW_LARGE         (0x00000011)

typedef struct tag_TPWindowSize
{
	BYTE    eWindowType;

	INT     nMinRight;
	INT     nMinBottom;
	INT     nMaxRigth;
	INT		nMaxBottom;

	tag_TPWindowSize()
	{
		eWindowType = TP_WINDOW_SMALL;

		nMinBottom		   = 0;
		nMinRight		   = 0;
		nMaxBottom		   = 0;
		nMaxRigth		   = 0;
	}
}TPWindowSize;

