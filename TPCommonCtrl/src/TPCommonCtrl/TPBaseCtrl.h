#pragma once

#define TP_BASESTATE_DELETEITEM     0x00000001
#define TP_BASESTATE_ERASE          0x00000002
#define TP_BASESTATE_NOACTWND       0x00000004
#define TP_BASESTATE_HIDE           0x00000008
#define TP_BASESTATE_LOCKSCREEN     0x00000010
#define TP_BASESTATE_ENDEDIT        0x00000020
#define TP_BASESTATE_NOSCROLL       0x00000040
#define TP_BASESTATE_TEMPLATE       0x00000080 //¡Ÿ ± π”√
#define TP_BASESTATE_MAINMENU       0x00000100 
#define TP_BASESTATE_APPLY          0x00000200 
#define TP_BASESTATE_NOACTNOTIFY    0x00000400
#define TP_BASESTATE_COMBOBOXMENU   0x00000800 
#define TP_BASESTATE_TOPMOST        0x00001000 
#define TP_BASESTATE_SELCHANGE      0x00002000 
#define TP_BASESTATE_ENABLETOOLTIP  0x00004000 
#define TP_BASESTATE_NOACTPREWND    0x00008000 
#define TP_BASESTATE_CANCELKEY      0x00010000 
#define TP_BASESTATE_CANCLEEDIT     0x00020000
#define TP_BASESTATE_TOPMOSTDLG     0x00040000
#define TP_BASESTATE_TOPMOSTTM      0x00080000
#define TP_BASESTATE_MODESTATE      0x00100000
#define TP_BASESTATE_ENABLEMAIN     0x00200000


#define TP_BASESTATE_CENTER         TP_BASESTATE_NOSCROLL  
#define TP_BASESTATE_NOSPARTEBAR    TP_BASESTATE_APPLY  

struct SBDictionary
{
	LPCTSTR (*SBLoadString)(LPCTSTR strFind);
	LPVOID  (*SBLoadStringEx)(LPVOID strFind);
	void    (*SBSetDictionaryPart)(int iPart);
	int     (*SBGetDictionaryPart)();
	SBDictionary()
	{
		ZeroMemory(this,sizeof(SBDictionary));
	}
};

class AFX_EXT_CLASS  CTPBaseCtrl
{
public:
	CTPBaseCtrl(void);
	~CTPBaseCtrl(void);
public:
	DWORD                m_dwWindowLevel;
	TP_SYSTEM_FONT       m_iFontIndex;
	ULONGLONG            m_uBaseState;
	LPARAM               m_lParam;  
	HFONT                m_hBaseFont;	
	LPVOID               m_pRes;
	long                 m_lRes;

	static SBDictionary  m_stuSBDictionary;
public:
	void            (*DestoryNotify)(); 

	void              SaveFont(HWND hWnd);
	void              RestoreFont(HWND hWnd);
	static HFONT      GetBaseFont(HWND hWnd);
	static void       SetBaseFont(HWND hWnd,HFONT &hFont);

};
