#pragma once

#define		TPBSKIN_NONE               0x00000001   //平板风格
#define		TPBSKIN_SQUARE1            0x00000010   //方按钮
#define		TPBSKIN_SQUARE2            0x00000020   //方按钮

#define		TPBSKIN_CUSTOM             0x00010000   //用自定义图片
#define		TPBSKIN_IMAGELIST          0x00020000   //用自定义图片列表
#define		TPBSKIN_STRETCH            0x00040000   //图片拉伸
#define		TPBSKIN_AUTOSIZE           0x00080000   //根据图片自动设置按钮大小
#define		TPBSKIN_NOACT              0x00020000   // 
#define		TPBSKIN_DESTROY            0x00040000   // 

enum BUTTON_TYPE
{
	BUTTON_TYPE_CHECK,
	BUTTON_TYPE_RADIO,
	BUTTON_TYPE_SQUARE1,
	BUTTON_TYPE_SQUARE2,
	BUTTON_TYPE_MAX,
};
enum BUTTON_INDEX
{	
	BUTTON_NOR = 0,
	BUTTON_DOWN, 
	BUTTON_OVER,
	BUTTON_FOCUS,
	BUTTON_DIS,

	CHECK_NOR,
	CHECK_DOWN,
	CHECK_OVER,
	CHECK_FOCUS,
	CHECK_DIS,

	STATE_NOR,
	STATE_DOWN,
	STATE_OVER,
	STATE_FOCUS,
	STATE_DIS,

	BUTTON_MAX,
};


// CTPButton
#define TP_BUTTON_RDOWN  0x00000001  
#define TP_BUTTON_RUP    0x00000002

#define		TPBTNTYPE_CHECK            0x01000000   
#define		TPBTNTYPE_RADIO            0x02000000   
#define		TPBTNTYPE_BUTTON           0x04000000   
#define		TPBTNTYPE_GROUPBOX         0x08000000   
#define		TPBTNTYPE_DEFAULT          0x10000000   
#define		TPBTNTYPE_RDFOCUS          0x20000000   
#define		TPBTNTYPE_TOOLBAR          0x40000000   

#define WM_BUTTON WM_USER +55

#define     WS_CHECHBUTTON  (WS_CHILD | WS_VISIBLE|BS_AUTOCHECKBOX|BS_PUSHLIKE)

struct TPButtonStateInfo
{
	UINT    uFlag;
	CString sFace;
	CString sToolTips;
	TPButtonStateInfo()
	{
		uFlag = -1;
	}
};
typedef CArray<TPButtonStateInfo,TPButtonStateInfo&> TPButtonStateInfoArray;
class AFX_EXT_CLASS CTPButton : public CButton ,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPButton)

public:
	CTPButton();
	virtual ~CTPButton();
public:
	CWnd                   *m_pMsgWnd; 
	UINT                    m_eBackColor;	
	UINT                    m_eTextColor;
	TPControlData          *m_pControlData;
	BOOL                    m_bLButtonDown;	
	BOOL                    m_bVisible;
	BOOL                   *m_pbForceEnable;
	static HWND             m_hPressBtn;
	static UINT             m_iAccBtnID;

protected:	
	BOOL                    m_bSubWindow; 
	BOOL                    m_bCreate;
	UINT                    m_uState;
	UINT                    m_uStyle; 
	UINT                    m_uPrivateStyle; 
	UINT_PTR                m_hTimerOver;
	UINT_PTR                m_hTimerAcc;
	COLORREF                m_clMask;


	int                     m_iImageIndex;
	BOOL                    m_bOver;
	UINT                    m_uDelay;
	UINT                    m_uInterval; 	
	UINT                    m_iDelayCount;	
	COLORREF                m_cBackIndex;
	COLORREF                m_cTextIndex;


	CString                 m_sToolTips[2]; 
	CRgn                    m_rgnWnd;
	CString                 m_sText;
	UINT                    m_uControlState;
	CPoint                  m_ptStretchAlign;
	TPButtonStateInfoArray  m_aButtonState;
	int                     m_iStateIndex; 

	TPCommonRes            *m_pBtnRes[BUTTON_MAX]; 	
	BOOL                    m_bDefaultData;
	TPHotKeyData           *m_pHotKeyData;
	UINT                    m_vHotKey;
	UINT                    m_vKeyFlag;

public:
	void                  (*m_pButtonAccletCallback)(HWND hWnd,LPVOID pVoid);
	LPVOID                  m_pCallBackParam;
	CString               (*m_pButtonToolTipCallBack)(LPVOID pVoid);
	LPVOID                  m_pToolTipParam;
public:

	void			        SetBitmaps(LPCTSTR lpText,BOOL bStrech,BOOL bSimple = TRUE);

	void			        SetBitmaps(LPCTSTR lpNor,
		                               LPCTSTR lpDown,
									   LPCTSTR lpOver,
									   LPCTSTR lpDis,
									   BOOL bStrech = FALSE);		  


	void		  	        SetBitmaps(CBitmap* pNor,		      
                             		   CBitmap* pDown,				  
									   CBitmap* pOver,				  
									   CBitmap* pDis,			  		
									   BOOL bStrech = FALSE);	

	void			        SetBitmaps(LPCTSTR lpNor,
		                               LPCTSTR lpNorDown,
		                               LPCTSTR lpNorOver,
		                               LPCTSTR lpNorDis,
		                               LPCTSTR lpNorFocus,				

		                               LPCTSTR lpCheckNor = NULL,     
		                               LPCTSTR lpCheckDown = NULL,    
		                               LPCTSTR lpCheckOver = NULL,    
		                               LPCTSTR lpCheckDis = NULL,     
		                               LPCTSTR lpCheckFocus = NULL,   

		                               LPCTSTR lpStateNor = NULL,  
		                               LPCTSTR lpStateDown = NULL, 
									   LPCTSTR lpStateOver = NULL,    
									   LPCTSTR lpStateDis = NULL,     
									   LPCTSTR lpFocus = NULL,   
		                               BOOL bStrech = FALSE);		  

	void		  	        SetBitmaps(CBitmap* pNor,		      
	                                   CBitmap* pNorDown,				  
		                               CBitmap* pNorOver,				  
		                               CBitmap* pNorDis,			  
		                               CBitmap* pNorFocus,				  

                                   	   CBitmap* pCheckNor = NULL,     
		                               CBitmap* pCheckDown = NULL,    
		                               CBitmap* pCheckOver = NULL,    
		                               CBitmap* pCheckDis = NULL,     
		                               CBitmap* pCheckFocus = NULL,   

		                               CBitmap* pStateNor = NULL,  
		                               CBitmap* pStateDown = NULL, 
									   CBitmap* pStateOver = NULL,    
									   CBitmap* pStateDis = NULL,     
									   CBitmap* pStateFocus = NULL,   
									   BOOL bStrech = FALSE);	

	void                    SetToolTipText( LPCTSTR lpText,LPCTSTR lpTextCheck = NULL);
	void                    SetToolTipText2( LPVOID lpText,LPVOID lpTextCheck = NULL); //by zhh
	void                    SetBackColor(TP_SYSTEM_COLOR eBackIndex); 
	void                    SetBackColor(COLORREF eBackIndex); 
	

	void                    SetAccelerateTime(UINT uDelay,UINT uInterval); 
	void                    SetSkinStyle(UINT uStyle);
	void                    SetAutoSize(BOOL bAuto);
	void                    SetResSize();
	void                    SetStretch(BOOL bSet);
	static void             SetSkinStyle(CWnd *pParent,UINT uID,UINT uStyle);
	void                    SetAccelerateCallBack(void (*f)(HWND hWnd,LPVOID pVoid),LPVOID pVoid);    
	BOOL                    IsWindowVisible();
	BOOL                    ShowWindow(int nCmdShow);
	void                    SetCheck(int nCheck);
	void                    SetTextColor(COLORREF clSet);   
	void                    SetHotKeyData(TPHotKeyData *pHotKeyData);
	void                    SetHotKeyData(UINT vHotKey,UINT vKeyFlag);

	TPCommonRes             *GetBtnRes(BUTTON_INDEX eIndex); 
	int                      GetBtnResWidth(); 
	int                      GetBtnResHeight();
	void                     SetStretch(UINT uCtrlState,int iLeft,int iRight); 
	void                     SetMask(COLORREF clMask);
	void                     SetPrivateStyle(UINT uAdd,UINT uRemove); 

	void                     SetStateInfo(TPButtonStateInfoArray &aState);
	void                     SetStateIndex(int iIndex);	
	void                     SetStateFlag(UINT uFlag);

	static  void            LoadDefRes(BOOL bLoad);    
protected:
	void                    LoadRes();
	static  void            ResembleRes(TPCommonRes **pResData);
	void                    ResembleRes(TPCommonRes **pResData,int iImage);

	void                    GetImageIndex(CDC *pDC);
	void                    PaintRadio(CDC *pDC);
	void                    PaintCheck(CDC *pDC);
	void                    PaintGroup(CDC *pDC);
	void                    PaintButton(CDC *pDC);
private:
	virtual void            SubWindow(BOOL bSub);
	LRESULT                 OnPrint(WPARAM wp,LPARAM lp);
	LRESULT                 OnSetCheck(WPARAM wp,LPARAM lp);
	LRESULT                 OnSetState(WPARAM wp,LPARAM lp);

protected:
	virtual int             OnToolHitTest(CPoint point,TOOLINFO* pTI ) const;
	virtual BOOL            OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnStyleChanged(int nStyleType,  LPSTYLESTRUCT pStruct);
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


