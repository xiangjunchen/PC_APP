#if !defined(TPCOMMONIMPORT_INCLUDED_)
#define TPCOMMONIMPORT_INCLUDED_


//AFX_EXT_CLASS����������һ��DLL���е�����һ����AFX_EXT_CLASS���ε�DLL 
//��������Ƕ���ÿ�����̲�ͬ�����η�����AFX_EXT_CLASS_A,AFX_EXT_CLASS_B�ȣ�Ĭ��Ϊ���룬�ڹ��̵�stdafx.h��������Ϊ������

#undef	AFX_EXT_CLASS
#undef	AFX_EXT_DATA
#undef	AFX_EXT_API


//#define AFX_EXT_CLASS	AFX_CLASS_IMPORT
//#define AFX_EXT_DATA		AFX_DATA_IMPORT
//#define AFX_EXT_API		AFX_API_IMPORT



#define AFX_EXT_CLASS	__declspec(dllimport)
#define AFX_EXT_DATA	__declspec(dllimport)
#define AFX_EXT_API		__declspec(dllimport)



#endif // TPCOMMONIMPORT_INCLUDED_

/*
Resource.h ��ID�ֲ�ͼ

��������ҳ             1000  - 1500
CG                     1500  - 6500
/----
TPCommonCtrl(7������)  6500  - 7500
TPComonCtrl			   6500	 - 6650
TPCommonWnd			   6650	 - 6850
TPExtendCtrl		   6850	 - 7000
TPExtraCtrl			   7000	 - 7150
TPNewsCtrl			   7150	 - 7250	
----/
Enging                 7500  - 8500
�ؼ��ڲ���������       8500  - 10500
�ؼ�����               11500 - 13000
USER EXPLORER          13000 - 14000
CLIP EXPLORER          14000 - 16000
SV/MV                  16000 - 17000
Brd  EXPLORER          19000 - 21000
Capture Render         21000 - 23000
System Set ,APP        23000 - 24000
Other                  24000 - 26000
ACCR                   26000 - 27000
TimeLineWnd            27000 - 30000
TPEditMax			   30000 - 30300
TPMovImpExport		   30300 - 30500
DVD Capture			   30500 - 32000
*/