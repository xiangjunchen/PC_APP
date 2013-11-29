#pragma once

#define IMAGELIST_DISABLED    0x00000001
#define IMAGELIST_FOCUS       0x00000002
#define IMAGELIST_CHECKED     0x00000004
#define IMAGELIST_HOTLIGHT    0x00000008
#define IMAGELIST_PUSHED      0x00000010


// CTPImageButton

enum IMAGELIST_INDEX
{	
	IMAGELIST_NOR_NOR = 0,
	IMAGELIST_NOR_DOWN, 
	IMAGELIST_NOR_OVER,
	IMAGELIST_NOR_FOCUS,
	IMAGELIST_NOR_DIS,

	IMAGELIST_CHECK_NOR,
	IMAGELIST_CHECK_DOWN,
	IMAGELIST_CHECK_OVER,
	IMAGELIST_CHECK_FOCUS,
	IMAGELIST_CHECK_DIS,

	IMAGELIST_MAX,
};


#define WC_IMAGEBUTTON _L("ImageButton")

class AFX_EXT_CLASS CTPImageButton : public CWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPImageButton)

public:
	CTPImageButton();
	virtual ~CTPImageButton();
private:	
	CRect             m_rtImage;
	UINT_PTR          m_hTimer;
	TP_BORDER_STYLE   m_eBorderStyle;
	UINT              m_uState;
	UINT              m_uAlign;
	HIMAGELIST        m_hImagelist;
	CSize             m_szImage;
	int               m_iImageListIndex[IMAGELIST_MAX];
	BOOL              m_bAutoCheck; 
	BOOL              m_bAutoDel;
public:
	BOOL           Create(UINT uStyle,RECT &rtWnd,CWnd *pParentWnd,UINT nID);

	void           SetAlign(UINT uAlign);	
	void           SetImageList(HIMAGELIST hImageList,BOOL bAutoDel = FALSE);
	UINT           GetState();
	void           SetState(UINT uState);
	void           SetAutoCheck(BOOL bSet);


	void		   SetImage(int iNor,int iDown,int iOver,int iDis,int iFocus);			  		
	void		   SetImage(int iNorNor,  int iNorDown,  int iNorOver  ,int iNorDis,  int iNorFocus,
		                    int iCheckNor,int iCheckDown,int iCheckOver,int iCheckDis,int iCheckFocus);   

	static  void  RegisterClass(BOOL bRegister);
private:
	int            GetImageIndex(); 
	void           ResembleImageIndex(); 

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


