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
	DWORD       dwIdentifer;         //Ψһ��ʶ ����������
	UINT		nID;		          //IDֵ
	UINT        vkChar;               //��ݼ� VK_A
	UINT        uFlag;	              //TP_SHIFT TP_CTRL TP_ALT 
	DWORD		dwStyle;	          //����		
	DWORD		dwStyleEx;	          //��չ����	
	TCHAR       sClassType[TPCONTROLDATA_CLASSTYPE];       //�ؼ����� WC_BUTTON 
	TCHAR		sFaceRes[MAX_PATH];   //��Դ���� ͼƬλ��	
	TCHAR		sText[TPCONTROLDATA_TEXT];            //�ı����� �ı� Button Edit ������д����
	TCHAR       sNotifyUp[TPCONTROLDATA_NOTIFYUP];        //��Ŧ�� ̧��ʱ����ʾ��Ϣ 
	TCHAR       sNotifyDown[TPCONTROLDATA_NOTIFYDOWN];      //����ʱ����ʾ��Ϣ 
	DWORD       dwState;              //���� ���ر�־  TP_CONTROL_DOUBLESIZE
	RECT        rtCtrl;               // ��ť��С
	DWORD       dwReserved;           //����
}TPNSControlData,*LPTPNSControlData;



typedef struct tag_TPNSWndControlData
{
	CString          sGuiPluginName;  //GUI�������
	CString          sWndName;  //�������ƣ�Ӣ��ͨ�����ƣ�����������
	CString          sWndNameDesc;  //�������ƣ��ֵ����ƣ�������ʾ��

	GUID             guidWnd;   //GUID

	long             lCtrlType; //��ť�Ϳ�ݼ������Ȳ�ͬ����Ҫ�ֱ�����,��Щֻ֧�ֿ�ݼ�����
//#define TP_CONTROL_HOTKEY    0x00000001
//#define TP_CONTROL_BUTTON    0x00000002

	DWORD            dwCtrlIdentiferNum[TP_CONTROL_GROUPNUM];   //��ť4������Ϣ
	DWORD           *pDwCtrlGroupIdentifer[TP_CONTROL_GROUPNUM];//��ť4������Ϣ-����
	

	DWORD            dwHotkeyIdentiferNum[TP_CONTROL_GROUPNUM];//��ݼ�4������Ϣ
	DWORD           *pDwHotkeyGroupIdentifer[TP_CONTROL_GROUPNUM];//��ݼ�4������Ϣ-����
	
	POINT            *pDwIdentifer;  //ID��Ӧ��
	DWORD            dwCount;

	long            lControlSize;  //��ť
	TPNSControlData *pControlData; //ÿ��ID��identifierֻ����һ��
	

	long            lHotkeySize;  //��ݼ�
	TPNSControlData *pHotkeyData;  //ÿ��ID��identifier�����ж��,��ͬһ����ť�����ɶ����ݼ�֧��
	
	//��ť�Ϳ�ݼ������Ȳ�ͬ����Ҫ�ֱ�����

	long            lToolBarCol;     //������֧�ֵ���
	long            lToolBarRow;     //������֧�ֵ���



	BOOL            bChangeHotKey;  /////��ʱ
	BOOL            bChangeCtrl;

}TPNSWndControlData,*LPTPNSWndControlData;

typedef CArray<TPNSControlData*,TPNSControlData*&> TPNSControlDataArr;
typedef CArray<TPNSWndControlData*,TPNSWndControlData*&> TPNSWndControlDataArr;
