#pragma once
#include "TPBaseFrameToolTips.h"

class CTPMenuEx;

#define BOX_CLOSE   0x00000001
#define BOX_HELP    0x00000002
#define BOX_MIN     0x00000004
#define BOX_MAX     0x00000008

#define BOX_NAIL    0x00000020
#define BOX_ICON    0x00000040
#define BOX_CAPTION 0x00000080
#define BOX_BORDER  0x00000100
#define BOX_SIZE    0x00000200
#define BOX_SYSMENU 0x00000400
#define BOX_MODAL   0x00001000


#define CAPTION_BAR 0x00000800

struct TPMessageBoxParam
{
	HWND       hWnd;
	CString    lpText;
	CString    lpCaption;
	UINT       uType;
};

enum TPMODE_STATE 
{
	TPMODE_START,
	TPMODE_END,
	TPMODE_DLGBEGIN,
	TPMODE_DLGEND,
};
enum TPFRAME_RES
{	
	BORDER_TOP_A = 0,
	BORDER_TOP_N,
	BORDER_LEFT_A,
	BORDER_LEFT_N,
	BORDER_RIGHT_A,
	BORDER_RIGHT_N,
	BORDER_BOTTOM_A,
	BORDER_BOTTOM_N,
	BORDER_BAR_A,
	BORDER_BAR_N,
	BORDER_SIZEBOX_A,
	BORDER_SIZEBOX_N,

	BOX_RES_CLOSE,
	BOX_RES_MAX,
	BOX_RES_MIN,
	BOX_RES_RESTORE,
	BOX_RES_SYSMENU,
	BOX_RES_HELP,
	BOX_RES_NAIL_N,
	BOX_RES_NAIL_D,

	FRAME_MAX,
};
enum TPFrameState
{
	TPFrameState_Create         = 0 ,
	TPFrameState_Destroy        = 1,
	TPFrameState_Activate       = 2,
	TPFrameState_ActivatePanel  = 5,
	TPFrameState_Minimize       = 4,
	TPFrameState_ShowWindow     = 3,
	TPFrameState_TaskBar        = 6,

};

#define STATE_ACTIVATE     0x00000001
#define STATE_MINABLE      0x00000002
#define STATE_MAXABLE      0x00000004
#define STATE_HELPABLE     0x00000008
#define STATE_HELPSET      0x00000010
#define STATE_INVISIBLE    0x00000040
#define STATE_NOCALLBACK   0x00000080
#define STATE_NOSIZEEX     0x00000100
#define STATE_LBUTTONDOWN  0x00000200
#define STATE_NOLDB        0x00000400
#define STATE_DISABLE      0x00000800
#define STATE_NOACTWND     0x00001000
#define STATE_NOMIN        0x00002000
#define STATE_LOGOTEXT     0x00004000
#define STATE_DOMODL       0x00008000
#define STATE_FRAME        0x00010000
#define STATE_FRAMESET     0x00020000
#define STATE_TABORDER     0x00080000
//{2011-01-25:zcd:add
#define STATE_CHECKINGACTIVE 0x00100000
//}

#define FRAME_OTHER       		 1
#define FRAME_TIMELINE    		 2
#define FRAME_SV          		 3
#define FRAME_MV          		 4
#define FRAME_EXPLORER    		 5
#define FRAME_LOGGING     		 6
#define FRAME_MSGBOX      		 7
#define FRAME_AUDIOMIX    		 8
#define FRAME_AUDIOCLIPCTL		 9
#define FRAME_AUDIOOUTPUTTOOL	 10
#define FRAME_TRIM     		     11
#define FRAME_DIALOG   		     12
#define FRAME_DIALOGLESS	     13
#define FRAME_SOBEYSET    		 14
#define FRAME_BRDEXPLORER    	 15
#define FRAME_FXEXPLORER    	 16
#define FRAME_MULTICAMRE    	 17
#define FRAME_CD     		     18
#define FRAME_DVD     		     19
#define FRAME_VOICEOVER     	 20
#define FRAME_IMPORTFILE         21
#define FRAME_BATCHDIGITIZE      22
#define FRAME_SVDIGITIZE    	 23
#define FRAME_SG   		         24
#define FRAME_EFFECTEDIT  		 25
#define FRAME_SPEEDCTRL  		 26
#define FRAME_RECORDTOVTR  		 27
#define FRAME_DAILYNOTEEDIT      28
#define FRAME_DOCNOTEEDIT		 29
#define FRAME_PLANNOTEEDIT       30
#define FRAME_STARTPAGE          31
#define FRAME_APPEXPLORER    	 32



#define FRAME_ATTACH_NULL   0x00000000
#define FRAME_ATTACH_USER   0x00000001
#define FRAME_ATTACH_PRJ    0x00000002
#define FRAME_ATTACH_TL     0x00000004
#define FRAME_ATTACH_CGTL   0x00000008

#define FRAME_STYLE_LOCKWND     0x00000001
#define FRAME_STYLE_LOCKLEFT    0x00000002
#define FRAME_STYLE_LOCKRIGHT   0x00000004
#define FRAME_STYLE_LOCKTOP     0x00000008
#define FRAME_STYLE_LOCKBOTTOM  0x00000010
#define FRAME_STYLE_C5TOPARENT  0x00000020   //m_uFrameStyle
#define FRAME_STYLE_NOESC       0x00000040
#define FRAME_STYLE_X1CARD      0x00000080
#define FRAME_STYLE_SHOWACT     0x00000100
#define FRAME_STYLE_NOACTPRE    0x00000200
#define FRAME_STYLE_CGEDIT      0x00000400
#define FRAME_STYLE_NOGLOBLE    0x00000800
#define FRAME_STYLE_VNUMB       0x00001000
#define FRAME_STYLE_NOCALCSIZE  0x00002000
#define FRAME_STYLE_NOTOPMOST   0x00004000

#define  TP_GUISHOW_MINMIZE  0x00000001
#define  TP_GUISHOW_MAXMIZE  0x00000002
#define  TP_GUISHOW_ENABLE   0x00000004
#define  TP_GUISHOW_SHOW     0x00000008


typedef struct tag_TPDockData
{
	WORD     wDockX;	
	WORD     wDockY;	
	HWND     hChildWnd; 
	tag_TPDockData()
	{
		wDockX      = 0;		
		wDockY      = 0;	
		hChildWnd    = NULL; 
	}
}TPDockData;
typedef CArray<TPDockData,TPDockData &> TPDockDataArray;
class CTPBaseFrame;
typedef CArray<CTPBaseFrame*,CTPBaseFrame*&> CTPBaseFrameArray;

#define ON_EN_KILLFOCUS_RANGE(id, idLast, memberFxn) \
{ WM_COMMAND, EN_KILLFOCUS, (WORD)id, (WORD)idLast, AfxSigCmd_RANGE, \
	(AFX_PMSG) \
	(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(UINT) > \
	(memberFxn)) },

#define ON_EN_SETFOCUS_RANGE(id, idLast, memberFxn) \
{ WM_COMMAND, EN_SETFOCUS, (WORD)id, (WORD)idLast, AfxSigCmd_RANGE, \
	(AFX_PMSG) \
	(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(UINT) > \
	(memberFxn)) },

#define ON_CBN_KILLFOCUS_RANGE(id, idLast, memberFxn) \
{ WM_COMMAND, CBN_KILLFOCUS, (WORD)id, (WORD)idLast, AfxSigCmd_RANGE, \
	(AFX_PMSG) \
	(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(UINT) > \
	(memberFxn)) },

#define ON_CBN_SETFOCUS_RANGE(id, idLast, memberFxn) \
{ WM_COMMAND, CBN_SETFOCUS, (WORD)id, (WORD)idLast, AfxSigCmd_RANGE, \
	(AFX_PMSG) \
	(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(UINT) > \
	(memberFxn)) },


#define ON_FOCUS_RANGE(id, idLast, memberFxn) \
{ WM_COMMAND, EN_KILLFOCUS, (WORD)id, (WORD)idLast, AfxSigCmd_RANGE, (AFX_PMSG) (static_cast< void (AFX_MSG_CALL CCmdTarget::*)(UINT) > (memberFxn))  },\
{ WM_COMMAND, EN_SETFOCUS, (WORD)id, (WORD)idLast, AfxSigCmd_RANGE,  (AFX_PMSG) (static_cast< void (AFX_MSG_CALL CCmdTarget::*)(UINT) > (memberFxn))  },\
{ WM_COMMAND, CBN_KILLFOCUS, (WORD)id, (WORD)idLast, AfxSigCmd_RANGE,(AFX_PMSG) (static_cast< void (AFX_MSG_CALL CCmdTarget::*)(UINT) > (memberFxn))  },\
{ WM_COMMAND, CBN_SETFOCUS, (WORD)id, (WORD)idLast, AfxSigCmd_RANGE, (AFX_PMSG) (static_cast< void (AFX_MSG_CALL CCmdTarget::*)(UINT) > (memberFxn))  },

struct _tagDlgTabItem
{
	UINT    uPreCtrl;
	UINT    uNextCtrl;
	HWND    pNextWnd; 
	HWND    pPreWnd; 
	_tagDlgTabItem()
	{
		uPreCtrl  = 0;
		uNextCtrl = 0;
		pNextWnd  = NULL;
		pPreWnd   = NULL;
	}
	_tagDlgTabItem(UINT uPreCtrl,UINT uNextCtrl,CWnd  *pNextWnd = NULL,CWnd  *pPreWnd = NULL)
	{
		this->uPreCtrl  = uPreCtrl;
		this->uNextCtrl = uNextCtrl;
		this->pNextWnd  = pNextWnd?pNextWnd->GetSafeHwnd():NULL; 
		this->pPreWnd   = pPreWnd ?pPreWnd->GetSafeHwnd() :NULL;
	}
};
typedef CArray<_tagDlgTabItem,_tagDlgTabItem&> TPDlgTabItemArray;

struct TPBaseFrameState
{
	UINT   uState;
	RECT   rtState;
	double dSplitV; 
	double dSplitH;
	DWORD  dwSplitHEx;
	DWORD  dwShowBottom;
	DWORD  dwMinWnd;
	DWORD  dRes[7];
	LPARAM lParam;
	TPBaseFrameState()
	{
		ZeroMemory(this,sizeof(TPBaseFrameState));
	}
};
class AFX_EXT_CLASS CTPBaseFrame : public CTPBaseCtrl
{
public:
	CTPBaseFrame(void);
	~CTPBaseFrame(void);

private: //此处顺序千万不要修改
	WORD                    m_dwSafeFlag;
	WORD                    m_dwSafeFlagEx;
public:
	static CRect             m_rtScreen;
	static CRect             m_rtEdge;
	static ULONGLONG         m_uBaseFlag; 
	static BOOL              m_bFixWnd;

	static CTPBaseFrameArray m_aBaseFrame;
	static void            (*m_pBaseFrameCmd)(TPFrameState nCmd,CTPBaseFrame *pFrame);	
	static void            (*m_pBaseFrameAct)(CTPBaseFrame *pFrame,BOOL bAct);
	static LRESULT         (*m_pBaseFrameGetPos)(LPCTSTR sGuiName,LPCTSTR  sWndType,int iFrameIndex,CRect &rtGet,void *pState);	//TPBaseFrameState
	static LRESULT         (*m_pBaseFrameAppKeyDown)(UINT nChar,UINT nFlag);
	static BOOL            (*m_pCanActWnd)(HWND hWnd);
	static BOOL            (*m_pGetActWnd)(HWND &hWnd,CTPBaseFrame *pThis);
	static BOOL            (*m_pUserConfigrationCmd)(LPCTSTR sGuiName,LPCTSTR  sWndType,UINT uCmd,LPARAM lParam);
	static void           *(*m_pMainFrameCmd)(UINT uCmd,LPARAM lParam);
	static LRESULT         (*m_pTMCmd)(UINT uCmd,LPARAM lParam);

	UINT            m_uState;   //状态
	CString         m_sText;    //标题

	CString         m_sIconFile;//Icon
	CString         m_sTaskBarName;//Icon
	CString     	m_sGuiName;
	CString         m_sWndType;

	CStringArray    m_sHotKey;
	CStringArray    m_sControlData;
	LPVOID          m_pMonitorInfo;


	long             m_lPreHotkeySize;  
	HWND             m_hPreHotkeyWnd;


	BOOL             m_bAttachTaskBar;
	UINT             m_uFrameLevel;  //1 2 3
	UINT             m_uFrameType;   //类型
	UINT             m_uFrameStyle;   //类型
	int              m_iFrameIndex;	
	long             m_lAttachType;	
	HWND             m_hAttachWnd;
	BOOL             m_bShow;

	CRect            m_rtMoving;
	CRect            m_rtBeforeMov;
	CRect            m_rtSize;
	TPBaseFrameState m_stuFrameState;

	HELPINFO                 m_stuHelpInfo;
	static int               m_iRefCount;
	static CTPBaseFrameTips *m_pToolTip;
	static BOOL              m_bToolTips;
	static CRect             m_rtMenuBox;
	static HWND              m_hActMsgWnd;
	static CWnd             *m_pActMsgWnd;


	HWND                     m_hActWnd; 
	HWND                     m_hActPreFrame; 
	CRect                    m_rtRestore;	

	TPCommonRes            *m_pAttachImage;
	int                     m_iAttachImage;
	CTPBaseFrame           *m_pUnActFrame;
	HWND                    m_hUnActFrame;
public:
	WNDPROC      	         m_pfnBaseSuper;
	HWND         	         m_hBaseWnd; 

protected:
	TPDlgTabItemArray        m_aForceOrder;
	BOOL                     m_bNoMsg;
 

public:

	virtual BOOL    PaintFrame(CDC *pDC,CRect rtWnd);                           //画标题栏的回调函数 ，以方便用户重新处理标题栏的部分工作 
	virtual BOOL    PushPinProc(UINT nState, CWnd* pWndOther, BOOL bMinimized); //PushPin回调函数 ，以方便用户重新定义钉子的行为 

	virtual BOOL    DoSysMenu(CPoint &ptScreen);


	void            SetTips(CString sTips);
	void            SetForceOrder(TPDlgTabItemArray &ForceOrder);
	void            SetForceOrder(CTPBaseFrame **pWnd,int iCount);
	void            SetForceOrder(int iPre,int iNext);

	static void           SetDefAct(BOOL bAct);
	static CTPBaseFrame  *GetDefAct();

	static CTPBaseFrame  *GetBaseFrame(CWnd *pWnd,int iType = 0);
	static CString        GetActiveFrameCaption();
	static CTPBaseFrame  *GetActiveFrame();
	static void           SetModeState(CWnd *pWnd,TPMODE_STATE eMode);
	void                  Destroy(CWnd *pWnd); 
	static void           ShowWnd(CWnd *pFrame);
	static UINT           GetVNumbKey(UINT uKey,UINT uFlag);

 

	void                  GetUITemplateInfo(CRect &rtWnd,TPBaseFrameState &rtState);
	void                  DoSysMenu();
	void                  AddUseConfigrationMenu(CString sDlgName,CTPMenuEx *pMenu,int iPos,UINT uBaseID);
	BOOL                  SetUseConfigrationID(CString sDlgName,int iIndex);
protected:
	void                  ShowSysMenu();
	void                  SetPreActWnd();
	BOOL                  FunVistaCmd(int iCmd,LPARAM lParam = NULL);

protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	UINT                    m_uStyleEx; 
	UINT                    m_uBoxAmount;  
	UINT                    m_uBoxAmountEx;  
	int                     m_iNailState;
	UINT                    m_uHitTest;
	UINT                    m_uDownHitTest;

	TPCommonRes            *m_pResources[FRAME_MAX];    //资源
	HRGN                    m_hRgnWnd;                  //窗口的
	CRect                   m_rtIconBox;
	CRect                   m_rtCloseBox;
	CRect                   m_rtClient;
	CRect                   m_rtMaxBox;
	CRect                   m_rtMinBox;	
	CRect                   m_rtHelpBox;
	CRect                   m_rtNailBox;
	CRect                   m_rtSysMenuBox;
	CRect                   m_rtText;
	CRect                   m_rtBotton;

	BOOL                    m_bBaseWnd;

	HICON                   m_hIcon;

	int                     m_iScaleWidth; 
	int                     m_iScaleHeight;
	CRect                   m_rtSrcScale; 
	int                     m_iWndWidth;
	int                     m_iWndHeight;

	int                     m_iBackColor;
	CTPSizeCtrl            *m_pCtrlSize;
	TPDockDataArray         m_aDockWnd;
	HWND                    m_hDockParentWnd; 
	DWORD                   m_dwDockState; 	
	BOOL                    m_bChild;
	CString                 m_sTips;
	CTPBaseFrame           *m_pBaseFrame;
	CTPMenuEx              *m_pMenuSys;

	BOOL                    m_bVistaFillWindow;
	UINT					m_uRoundStyle;

public:
	BOOL            ModifyStyleExEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	void            SetSkin(CString sSkinPath);
	void            SetSkinEx(CString sSkinPath);

	void            SetNail(BOOL bNail);
	void            SetCaptionBar(BOOL bBar);
	void            SetAutoSize(BOOL bAutoSize,TPControlData *pCtrlData = NULL,int iCtrlCount = 0);
	void            SetChildWindow(CWnd *pWnd,TPSizeMode eModeX,TPSizeMode eModeY);
	void            EnableChildWindow(CWnd *pWnd,BOOL bEnableSize);
	void            SetExtraSize(int iExLeft,int iExTop,int iExRight,int iExBottom);
	void            SetDockParentWnd(CWnd *pParentWnd); 

	void            SetFlex(int iScaleWidth,int iScaleHeight,int iSrcLeft,int iSrcTop,int iSrcRight,int iSrcBottom);
	void            SetFlexWidth(int iWidth);
	void            GetFlexRect(CRect &rtFlex);
	void            ModifyBoxAmount(UINT uAdd,UINT uRemove);
	CRect           GetBoxRect(DWORD dAmoutBox);

	LRESULT     	DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CRect           GetFrameEdge(HWND hWnd = NULL);
	static    void  GetMaxSizeInfo(POINT *pPoint);

	void            RestoreWnd(int iType = 0);

	static void     SetTaskBarState(BOOL bShow,BOOL bTop);  
	static void     Restore();  
	static void     SortZorder(CTPBaseFrameArray &aBaseFrame); 
	static void     SortZorder(CStringArray &sGuidName);
 
	virtual BOOL    PreTranslateMessageEx(MSG* pMsg);

protected:
	void            SubWindow(BOOL bSub = TRUE);  
	void            PaintFrameNS(CDC *pDC);
	void            PaintFrame();	

protected:
	void            FunSize(); 
	void            FunSizing(UINT fwSide, LPRECT pRect) ;
	UINT            FunNcHitTest(CPoint point);

	void            BitmapToRgn(HRGN &rgnSet,HBITMAP hBitmap,COLORREF backcolor,CDC *pDC,CRect rtExclude);

	void            OnMoving(CRect *rtWnd);
	void            OnMove(); 
	void            OnSizeEx(); 
	void            OnSizingEx(UINT uType,CRect *rtWnd); 
private:
	CString         GetAMendMenuString(CString strMenu,int uWidth);
};

struct TPBaseFrameSetPre
{
	TPBaseFrameSetPre()
	{
		CTPBaseFrame::m_uBaseFlag |= TP_BASESTATE_NOACTPREWND; 
	}
	~TPBaseFrameSetPre()
	{
		CTPBaseFrame::m_uBaseFlag &= ~TP_BASESTATE_NOACTPREWND;
	}
};
class AFX_EXT_CLASS CTPBaseWnd : public CWnd
{
	DECLARE_DYNAMIC(CTPBaseWnd)

public:
	CTPBaseWnd();
	virtual ~CTPBaseWnd();
public:
	int         m_iBackColor;
	int         m_iBackBrush; 
public:
	static BOOL EraseBkgnd(CDC* pDC,CWnd *pWnd);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
};
