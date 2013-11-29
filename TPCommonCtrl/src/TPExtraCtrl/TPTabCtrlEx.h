#pragma once


// CTPTabCtrlEx
#define WC_TABCTRLEX _L("TabCtrlExEx")

#define  TP_TLTABCTRL_HEIGHT    37

enum  enTabDir
{
	enTabCtrlTop     = 0,
	enTabCtrlBottom  = 1,
	enTabCtrlLeft    = 2,
	enTabCtrlRight   = 3
};
typedef struct tag_TPTabCtrlIcon
{
	UINT         nID;
	CString      strNote;	
	CRect        rtIcon;
	TPCommonRes *pImage;

	tag_TPTabCtrlIcon()
	{
		nID           = 0;
		strNote       = _L("") ;
		rtIcon        = CRect(0,0,0,0);
		pImage        = NULL;  
	}
	tag_TPTabCtrlIcon & operator= (tag_TPTabCtrlIcon other)
	{
		if(this == &other)	return *this;		
		nID           = other.nID;
		strNote       = other.strNote;		
		pImage        = other.pImage;
		return *this;
	}
} TPTabCtrlIcon;

typedef CArray<TPTabCtrlIcon,TPTabCtrlIcon &> TPTabCtrlIconArray; 
typedef struct tag_TPTabCtrlItem
{ 
	int		            nIndex;	        //索引   
	int                 nLineIndex;     //第几行
	CString             sText;		//文本
	CSize               szText;         //文本高度
	CRect               rtItem;			//位置 
	CRect               rtImage;
	CRect               rtText;
	int		            iImage;	    	//0 Normal 1 Diszble 2 Down 3 down disable
	CWnd*	            pWnd;			//窗口
	BOOL                uState;         //是否显示  	
	UINT                uFlag;
	LPARAM              dwData;
	TPTabCtrlIconArray  aIcon;

	tag_TPTabCtrlItem()
	{
		nIndex     = -1;	
		nLineIndex = 0;
		sText    = _L("");	
		szText     = CSize(0,0);         
		rtItem     = CRect(0,0,0,0);		
		iImage     = 0;	
		pWnd       = NULL;		
		uState     = 0;     
		uFlag      = 0;
		dwData     = 0;
	}
	tag_TPTabCtrlItem & operator= (tag_TPTabCtrlItem &other)
	{
		nIndex     = other.nIndex;	    
		nLineIndex = other.nLineIndex;
		sText    = other.sText;
		szText     = other.szText;     
		rtItem     = other.rtItem;			
		iImage     = other.iImage; 
		pWnd       = other.pWnd;	
		uState     = other.uState;    
		dwData     = other.dwData;	
		aIcon.RemoveAll();
		aIcon.Copy(other.aIcon);
		return *this;
	}
	CRect GetTextRect(enTabDir enumPos,CRect rtHead,int iTops)
	{

		CRect rtGet = rtText;
		rtGet.OffsetRect(rtItem.left,rtItem.top);
		if(enumPos == enTabCtrlTop || enumPos == enTabCtrlBottom )
		{
			rtGet.OffsetRect(-iTops + rtHead.left,0); 
			return rtGet;
		}
		else 
		{
			rtGet.OffsetRect(0,-iTops + rtHead.top); 
			return rtGet;
		}
		return CRect(0,0,0,0);
	}
	CRect GetImageRect(enTabDir enumPos,CRect rtHead,int iTops)
	{
		CRect rtGet = rtImage;
		rtGet.OffsetRect(rtItem.left,rtItem.top);
		if(enumPos == enTabCtrlTop || enumPos == enTabCtrlBottom )
		{
			rtGet.OffsetRect(-iTops + rtHead.left,0); 
			return rtGet;
		}
		else 
		{
			rtGet.OffsetRect(0,-iTops + rtHead.top); 
			return rtGet;
		}
		return CRect(0,0,0,0);
	}
	CRect GetItemRect(enTabDir enumPos,CRect rtHead,int iTops)
	{
		CRect rtGet = rtItem;
		if(enumPos == enTabCtrlTop || enumPos == enTabCtrlBottom )
		{
			rtGet.OffsetRect(-iTops + rtHead.left,0); 
			return rtGet;
		}
		else 
		{
			rtGet.OffsetRect(0,-iTops + rtHead.top); 
			return rtGet;
		}
		return CRect(0,0,0,0);
	}
	CRect GetItemCmdRect(enTabDir enumPos,CRect rtHead,int iCmd,int iTops)
	{
		if(iCmd <0 || iCmd >= aIcon.GetSize()) return   CRect(0,0,0,0);
		CRect rtGet = aIcon[iCmd].rtIcon;
		rtGet.OffsetRect(rtItem.left,rtItem.top);

		if(enumPos == enTabCtrlTop || enumPos == enTabCtrlBottom )
		{
			rtGet.OffsetRect(-iTops + rtHead.left,0); 
			return rtGet;
		}
		else 
		{
			rtGet.OffsetRect(0,-iTops + rtHead.top); 
			return rtGet;
		}
	}
	void OffsetRect(enTabDir enumPos,int iOffset)
	{
		if(enumPos == enTabCtrlTop || enumPos == enTabCtrlBottom )
		{
			rtImage.OffsetRect(iOffset,0);
			rtText.OffsetRect(iOffset,0);
			int iCmd = (int)aIcon.GetSize();
			for(int i=0;i<iCmd;i++)
				aIcon[i].rtIcon.OffsetRect(iOffset,0); 
		}
		else 
		{
			rtImage.OffsetRect(0,iOffset);
			rtText.OffsetRect(0,iOffset);
			int iCmd = (int)aIcon.GetSize();
			for(int i=0;i<iCmd;i++)
				aIcon[i].rtIcon.OffsetRect(0,iOffset);
		}
	}
	~tag_TPTabCtrlItem()
	{
		aIcon.RemoveAll();
	}
}TPTabCtrlItem;
typedef CArray<TPTabCtrlItem *,TPTabCtrlItem *&>TPTabCtrlItemArray;

class AFX_EXT_CLASS CTPTabCtrlEx : public CTabCtrl,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPTabCtrlEx)

public:
	CTPTabCtrlEx();
	virtual ~CTPTabCtrlEx();
public:
	DWORD                   m_dwWindowLevel;
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	TP_BORDER_STYLE         m_eBorderStyle;
	 
	UINT                    m_uPrivateState; 
	int                     m_iBackColor;
	TPCommonRes             *m_pTabBmp[4];
	TPCommonRes             *m_pCloseBox;
	CTPSliderBar            *m_pSliderBar; 


protected:
	CRect                   m_rtTab;        
	CRect                   m_rtWnd; 
	CRect                   m_rtClient; 
	CRect                   m_rtBar; 

	TPTabCtrlItemArray      m_aItem;
	CImageList              m_hImageList; 
	CSize                   m_szImageList;
	enTabDir                m_eTabDir;
	int                     m_iLockSize;
	int                     m_iTabSize;
	int                     m_iBarSize;
	int                     m_iTopSize;
	int                     m_iDownItem;  
	CSize                   m_szSelItem;
	TPTabCtrlIcon          *m_pDownIcon;
	TPTabCtrlItem          *m_pSelItem;

	CToolTipCtrl          *m_pShowTips;            //工具提示
	


public:
	static void             RegisterClass(BOOL bRegister);
	void                    SubWindow(BOOL bSub);
	void                    SetItemText(int nIndex,LPCTSTR sText);
	void                    SetLockItem(int  IIndex,BOOL bLock); 

	int                     GetMinHeight();
protected:
	LRESULT                 OnMsgInsertItem(WPARAM wp,LPARAM lp);
	LRESULT                 OnMsgDeleteAllItems(WPARAM wp,LPARAM lp);
	LRESULT                 OnMsgDeleteItem(WPARAM wp,LPARAM lp);	

	LRESULT                 OnMsgSetImageList(WPARAM wp,LPARAM lp);
	LRESULT                 OnMsgGetImageList(WPARAM wp,LPARAM lp);
	
	LRESULT                 OnMsgGetItemCount(WPARAM wp,LPARAM lp);
	LRESULT                 OnMsgSetCurFocus(WPARAM wp,LPARAM lp);
	LRESULT                 OnMsgGetCurFocus(WPARAM wp,LPARAM lp);
	LRESULT                 OnMsgGetCurSel(WPARAM wp,LPARAM lp);
	LRESULT                 OnMsgSetCurSel(WPARAM wp,LPARAM lp);
	LRESULT                 OnMsgGetRowCount(WPARAM wp,LPARAM lp);


protected:
	void                    PaintDC(CDC *pDC);
	void                    FunCalcSize(BOOL bSlider = TRUE);
	void                    FunCalcSizeMutilLine(BOOL bSlider = TRUE);
	void                    FunCalcSizeSingLine(BOOL bSlider = TRUE);
	virtual void            DrawItem(LPDRAWITEMSTRUCT pDrawItemStruct);
	virtual void            LoadSkin(CString sPath);
	LRESULT                 HitTest(CPoint pt,int &iItem,int &iIcon);

	BOOL                    OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult);


protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


