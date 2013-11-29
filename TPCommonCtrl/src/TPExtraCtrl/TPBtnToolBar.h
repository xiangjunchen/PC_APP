#pragma once

enum TP_BtnToolBar_Link
{
	TP_BtnToolBar_Unknow,
	TP_BtnToolBar_TimeLine,
	TP_BtnToolBar_Max,
};
class CTPBtnToolBar;

struct TP_BtnToolBarLink_Data
{	
	struct tag_dwState
	{
		UINT dwIndentifer;
		BOOL bCheck;
		BOOL bEnable;
	};
	CArray<TPControlData *,TPControlData *&> aCtrlData;
	CArray<int,int&>                         aCtrlSize;	
	CArray<POINT *,POINT *&>                 aLableData;		
	CArray<int,int &>                        aLableSize;	
	CArray<CTPBtnToolBar *,CTPBtnToolBar *&> aToolBar;
	CArray<tag_dwState,tag_dwState&>         aState;
	int                                      iIndexMax;
	TP_BtnToolBarLink_Data()
	{		
		iIndexMax = -1;
	}
	~TP_BtnToolBarLink_Data()
	{
		aCtrlData.RemoveAll();
		aCtrlSize.RemoveAll();	
		aLableData.RemoveAll();		
		aLableSize.RemoveAll();	
		aToolBar.RemoveAll();
		aState.RemoveAll();		
	}
	void Remove(int iIndex)
	{
		if(iIndex>=0 && iIndex<aCtrlData.GetSize())
		{		
			aCtrlData[iIndex]  = NULL;
			aCtrlSize[iIndex]  = 0;			
			aLableData[iIndex] = NULL;
			aLableSize[iIndex] = 0;	
			aToolBar[iIndex]   = NULL;
		}
	}
	BOOL GetState(UINT dwIndentifer,BOOL &bCheck,BOOL &bEnable)
	{
		for(INT l=0;l<aState.GetSize();l++)
		{
			if(aState[l].dwIndentifer == dwIndentifer)
			{
				bCheck  = aState[l].bCheck;				
				bEnable = aState[l].bEnable;				
				return TRUE;
			}
		}
		return FALSE;
	}
	BOOL GetCheck(UINT dwIndentifer)
	{
		for(INT l=0;l<aState.GetSize();l++)
		{
			if(aState[l].dwIndentifer == dwIndentifer)
			{
				return aState[l].bCheck;				
			}
		}
		return -1;
	}
	BOOL GetEnable(UINT dwIndentifer)
	{
		for(INT l=0;l<aState.GetSize();l++)
		{
			if(aState[l].dwIndentifer == dwIndentifer)
			{
				return aState[l].bEnable;				
			}
		}
		return -1;
	}
	void SetCheck(UINT dwIndentifer,BOOL bCheck)
	{
		for(INT l=0;l<aState.GetSize();l++)
		{
			if(aState[l].dwIndentifer == dwIndentifer)
			{
				aState[l].bCheck = bCheck;
				return;
			}
		}
		tag_dwState stuData;
		stuData.dwIndentifer = dwIndentifer;
		stuData.bCheck       = bCheck;
		aState.Add(stuData);
	}
	void Enable(UINT dwIndentifer,BOOL bEnable)
	{
		for(INT l=0;l<aState.GetSize();l++)
		{
			if(aState[l].dwIndentifer == dwIndentifer)
			{
				aState[l].bEnable = bEnable;
				return;
			}
		}
		tag_dwState stuData;
		stuData.dwIndentifer = dwIndentifer;
		stuData.bEnable      = bEnable;
		aState.Add(stuData);
	}
	int GetIndex()
	{
		SetData(-1,NULL,0,NULL,0,NULL);
		return ++iIndexMax ;
	}
	void SetData(int iIndex,TPControlData *pCtrlData,int lSize,POINT *pLable,int lLableSize,CTPBtnToolBar *pToolBar)
	{		
		if(iIndex>=0 && iIndex<aCtrlData.GetSize())
		{		
			aCtrlData[iIndex]  = pCtrlData;
			aCtrlSize[iIndex]  = lSize;			
			aLableData[iIndex] = pLable;
			aLableSize[iIndex] = lLableSize;
			aToolBar[iIndex]   = NULL;
		}
		else
		{
			aCtrlData.Add(pCtrlData);
			aCtrlSize.Add(lSize);			
			aLableData.Add(pLable);
			aLableSize.Add(lLableSize);
			aToolBar.Add(pToolBar);
		}
	}
};
// CTPBtnToolBar
#define WC_BTNTOOLBAR _L("BtnToolBar")
#define TP_BTNTOOLBAR_LINKCHECK  8

#define TPBTNTOOLSTATE_PALETTE   0x00000001

class CTPBtnToolBar;

struct TPBtnToolBarStateInfo
{
	UINT                     nID;
	TPButtonStateInfoArray   aState;
	TPBtnToolBarStateInfo &operator=(TPBtnToolBarStateInfo &other)
	{
		nID = other.nID;
		aState.Copy(other.aState);
		return *this;
	}
};
typedef CArray<TPBtnToolBarStateInfo,TPBtnToolBarStateInfo&> TPBtnToolBarStateInfoArray;

struct AFX_EXT_CLASS TPBtnToolBarSpecialCheck
{
	TPBtnToolBarSpecialCheck(CTPBtnToolBar *pToolBar,DWORD dwCheckKey);
	~TPBtnToolBarSpecialCheck();
private:
	DWORD m_dwCheckKey;
};

class AFX_EXT_CLASS CTPBtnToolBar : public CWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPBtnToolBar)

public:
	CTPBtnToolBar();
	virtual ~CTPBtnToolBar();

public:
	CWnd               *m_pMsgWnd;
	CTPBtnToolBar      *m_pParentWnd;
	CSize               m_szButton;	
	int                 m_iBtnSpace; 
	int                 m_iBackColor;	
	UINT                m_uBarStyle;
	int                 m_iTopX;
	double              m_dAmp;
	int                 m_iExLeft;
	int                 m_iLeftSpace;
	CWnd               *m_pAttachWnd; 
	CWnd               *m_pAttachWndEx; 
	int                 m_iTopOffset; 
	int                 m_iLeftOffset; 
	BOOL                m_bPaletteShow;
	TP_BtnToolBar_Link  m_eLinkType;
	int                 m_iLinkIndex;
	BOOL                m_bHotKey;

	UINT                m_uState;
	UINT                m_uControlCol;
	UINT                m_uControlLine;
	BOOL                m_bMouseWheel;
public:
	TPControlData      *m_pControlData;
	DWORD               m_dwControlCount;

	TPHotKeyData       *m_pHotKeyData;
	DWORD               m_dwHotKeyCount;
	TPHotKeyData       *m_pHotKeyDataEx;
	DWORD               m_dwHotKeyCountEx;
	TPHotKeyData       *m_pHotKeyDataExEx;
	 
protected:
	TPCommonRes        *m_pBarBack;
	BYTE                m_iBarBackEx[2];
	TPCommonRes        *m_pLogo;
	BOOL                m_bReLayOut;  
	int                 m_iTotalWidth;
	int                 m_iButtonSize[2]; 
	UINT_PTR            m_hContinueTimer;
	UINT                m_uContinueID;
	UINT                m_uContinueKey;
	UINT                m_uContinueFlag;
	UINT                m_uContinueState;

	CArray<CTPButton *, CTPButton *&> m_aTPButton;  
	CArray<CTPButton *, CTPButton *&> m_aTPButtonEx;  

	UINT                m_uPaletteID;
	UINT                m_uPaletteCol;
	UINT                m_uPaletteLine;
	CTPBtnToolBar      *m_pPaletteBar;
	void               *m_pWndCtrnData;
	POINT              *m_pControlLable;
	int                 m_iControlLable;
	CTPButton          *m_pBtnScroll[2];
	BYTE                m_bBtnScroll[2];		
	HRGN                m_hBtnScrollRgn;

	
private:
	BOOL               m_bSubWindow;
	BOOL               m_bCreate;		
	TPCommonRes        *m_pPaletteRes[4];

	TPControlData             *m_pControlDataSave;
	TPHotKeyData              *m_pHotKeyDataSave;	
	CWnd                      *m_pActWnd;  
	HWND                       m_hTopWnd;
	HWND                       m_hPressBtn;

	TPBtnToolBarStateInfoArray m_aBtnStateInfo;

public:
	BOOL            Create(DWORD uStyle,RECT &rtWnd,CWnd *pWnd,UINT nID);
	
	void            SetCheckEx(UINT nID,UINT *pnIDExtend,int iCountExtend,BOOL bCheck);
	void            SetCheck(UINT nID,BOOL bCheck);
	void            SetCheckIndex(int iIndex,BOOL bCheck);
	BOOL            GetCheck(UINT nID);
	BOOL            GetCheckIndex(int iIndex);
	void            Enable(UINT nID,BOOL bEnable);
	void            EnableEx(UINT *pnID,int iCount,UINT *pnIDExtend,int iCountExtend,BOOL bEnable);
	void            EnableExEx(UINT *pnID,int iCount,BOOL bEnable);
	void            EnableIndex(int iIndex,BOOL bEnable);
	void            Show(UINT nID,BOOL bShow);
	void            ShowControlData(TPControlData *pControlData,DWORD dwControlCount,UINT uID,BOOL bShow); 
	void            ShowHotKeyData(TPHotKeyData *pHotKeyData, DWORD dwHotKeyCount,UINT uID,BOOL bUse); 
	void            ShowIndex(int iIndex,BOOL bShow);
	void            SetToolTip(UINT nID,CString sUp,CString sDown);
	void            SetToolTipCallBack(UINT nID,CString (*p)(LPVOID),LPVOID pParam);	

	LRESULT         SetControlLable(POINT *pLable,int iLable); 
	LRESULT         SetControlData(TPControlData   *pControlData,DWORD dwControlCount,CString sCtrlPath,BOOL bSaveData = FALSE); 
	LRESULT         SetControlData(TPNSControlData  *pControlData, DWORD dwControlCount,CString sCtrlPath,POINT *pIndentify,int lIndentify);
	LRESULT         FunTest(TPControlData* pCtrl);
	LRESULT         SetLinkData(); 	
	LRESULT         SetHotKeyData(TPNSControlData  *pHotKeyData, DWORD dwHotKeyCount,POINT *pIndentify,int lIndentify);
	LRESULT         SetHotKeyData(TPHotKeyData    *pHotKeyData, DWORD dwHotKeyCount,BOOL bSaveData = FALSE); 
	LRESULT         SetHotKeyDataEx(TPHotKeyData    *pHotKeyData, DWORD dwHotKeyCount ); 
 	void            SetWndCtrlData(void *pWndCtrnData);
	static LRESULT  SetHotKeyData(TPNSControlData    *pHotKeyData, DWORD dwHotKeyCount,CTPButton *pButton,POINT *pIndentify,long  lIndentify);

	LRESULT         OnKeyDown(UINT vKey);   
	static int      OnKeyDownEx(TPNSControlData *pHotkeyData,long lHotkeySize ,UINT vKey);

 
	void            ReLayOut(BOOL bForce = FALSE);
	void            OnMouseWheel(int iRot); 
	void            SetLogo(CString sFileName); 
	void            SetBarBack(CString sBack,int iLeftTop,int iRightBottom);  

	void            SetSize(BOOL bHideWnd);
	void            SetScrollBtn();
	void            OnButtonClicked(UINT nID);	

	LRESULT         SetPaletteID(UINT nID,UINT uPaletteCol,UINT uPaletteLine);
	LRESULT         SetPaletteCtlData(TPControlData   *pControlData,DWORD dwControlCount,CString sCtrlPath,BOOL bSaveData = FALSE); 
	LRESULT         SetPaletteCtlData(TPNSControlData  *pControlData, DWORD dwControlCount,CString sCtrlPath,POINT *pIndentify,int lIndentify); 
	LRESULT         SetPaletteHotData(TPHotKeyData    *pHotKeyData, DWORD dwHotKeyCount,BOOL bSaveData = FALSE); 
	LRESULT         SetPaletteHotData(TPNSControlData  *pHotKeyData, DWORD dwHotKeyCount,POINT *pIndentify,int lIndentify);
	LRESULT         SetPaletteWndCtrlData(void *pWndCtrnData); 

	void            ShowPalette(); 
	void            LoadPalette();
	void            SetColAndRow(int  iCol,int iRow);
	CSize           GetWndSize();            
	
	void            SetAccelerate(UINT nID,UINT uDelay,UINT uInterval);

	void            SetMsgWnd(CWnd *pMsgWnd);
	void            SetScrollBtnBmp(CString sPathLeft,CString sPathRight); 

	void            SetBtnToolBarStateInfo(TPBtnToolBarStateInfoArray &aInfo);
	void            SetBtnToolBarStateIndex(UINT nID,int iIndex);
	void            SetBtnToolBarStateFlag(UINT nID,UINT uFlag);

	UINT            GetIndentify(UINT nID);
	UINT            GetID(UINT uIndentify);
	static   void   SetGlobalWndControlData(TPNSWndControlDataArr* pDataArrSrc);
	BOOL            IsDefaultKey(UINT nID,UINT vKey,UINT uFlag = 0);
	BOOL            IsHotKey(UINT vKey,UINT uFlag,BOOL bSatte = TRUE);

	CTPButton     *GetPressButton();
 
	CTPBtnToolBar  *GetPaletteBar(){ return m_pPaletteBar;}

	static   void   SetCheckLink(TPControlData *pCtrlData,UINT uIndentify,BOOL bCheck,TP_BtnToolBar_Link eType);
	static   void   EnableLink(TPControlData *pCtrlData,UINT uIndentify,BOOL bEnable,TP_BtnToolBar_Link eType);

	BOOL           (*SetCheckLinkCallBack)(UINT nID,BOOL bCheck,LPVOID pVoid); 
	BOOL           (*EnableLinkCallBack)(UINT nID,BOOL bEnable,LPVOID pVoid); 
	void            *m_pLinkCallBack;

	BOOL           (*PreTranslateMessageEx)(MSG* pMsg,LPVOID pData);
	void            *m_pPreTranslateMessageData;
private:

	LRESULT         SetInitState();
	void            FunCalcSize();

	virtual void    SubWindow(BOOL bSub);
	LRESULT         OnMsgButton(WPARAM lp,LPARAM wp);
	LRESULT         OnMsgGetBaseframe(WPARAM lp,LPARAM wp);

public:
	static  void    RegisterClass(BOOL bRegister);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};



