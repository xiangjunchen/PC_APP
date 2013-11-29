#pragma once


// CTPListPropertyCtrl
#define  WC_LISTPROPERTY _L("ListProperty")
#define  TP_LISTPROPERTY_COLUMN 2
typedef struct _tagTPListPropertyItem
{
	int                    iIndex;     //当前索引	
	LPARAM                 dwData;     //数据
	GUID                   guidData;   //GUID
	int                    iBackColor; //背景颜色		
	UINT                   uState;

	int                    iHeight;    //高度
	DWORD                  dwSelTick;
	long                   lUserState;

	struct tagSubItem
	{	
		CString                sText;      //文本
		DWORD                  dwEditType; //编辑模式
		long                   lEditMax;
		int                    iAlign;
		int                    iImage;
		CRect                  rtLable;
		CRect                  rtItem;
		int                    iBackColor;
		void CalcSize()
		{
			rtLable  = rtItem;
		}
	}aSubItem[TP_LISTPROPERTY_COLUMN];

	_tagTPListPropertyItem()
	{
		iIndex     = -1;     //当前索引	
		dwData     = 0;     //数据
		guidData   = GUID_NULL;   //GUID
		iBackColor = -1; //背景颜色		
		uState     = 0;
		iHeight    = 20;    //高度
		dwSelTick  = 0; 
		lUserState = -1;
		for(INT l=0;l<TP_LISTPROPERTY_COLUMN;l++)
		{
			aSubItem[l].sText      = _T("");      //文本
			aSubItem[l].dwEditType = 0; //编辑模式
			aSubItem[l].lEditMax   = 0x7fffffff;
			aSubItem[l].iAlign     = DT_LEFT;
			aSubItem[l].iImage     = -1;   
			aSubItem[l].rtLable    = CRect(0,0,0,0);
			aSubItem[l].iBackColor = -1;
		}
	}	
	void SetItemText(int iCol,CString str,int iAlignSet)
	{
		if( iCol >= TP_LISTPROPERTY_COLUMN || iCol <0) return;
		aSubItem[iCol].sText   = str;
		aSubItem[iCol].iAlign  = iAlignSet;
	}
	void SetItemImage(int iCol,int iImageSet)
	{
		if( iCol >= TP_LISTPROPERTY_COLUMN || iCol <0) return;
		aSubItem[iCol].iImage = iImageSet;		
	}
}TPListPropertyItem;

typedef  CArray<TPListPropertyItem *,TPListPropertyItem *&> TPListPropertyItemArray;


class AFX_EXT_CLASS CTPListPropertyCtrl : public CTPWnd,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPListPropertyCtrl)

public:
	CTPListPropertyCtrl();
	virtual ~CTPListPropertyCtrl();
public:	
	UINT                        m_uPrivateStyle;
	CWnd                       *m_pMsgWnd; 	
	BOOL                        m_bCalcEdit;
	BOOL                        m_bCanEdit;
protected:	
	BOOL                        m_bSubWindow; 
	BOOL                        m_bCreate;
	TP_BORDER_STYLE             m_eBorderStyle;
	int                         m_iBackColor;	 

//编辑专用
	int                         m_iItemSave;
	int                         m_iItemSubSave;
	int                         m_iItemLast;
	int                         m_iItemSubLast;
	CRect                       m_rtItemSubSave;	
	BOOL                        m_bEditItem;
	BOOL                        m_bReDraw;
	BOOL                        m_bCalcScroll;
	BOOL                        m_bDrag;
	int                         m_iColumn; 
	CString                     m_sEditLabel;

	CWnd                       *m_pEditWnd;	
public:	
	TPListPropertyItemArray     m_aItem;        //每条记录
	UINT                        m_uState;
protected:	
	CSize                       m_szItemReport;
	CRect                       m_rtClient;
	int                         m_iTotalHeight;
	int                         m_iTopY;
	int                         m_iTotalWidth;
	int                         m_iTopX;
	int                         m_iBarWidth;
	TPListCtrlHitTest           m_hitTest;
	VIDEO_TYPE                  m_enVideoType;          //当前视频制式
	CFont                      *m_pFont;
	CString                     m_sTextSave;
	INT64                       m_iFrameSave;



	int                         m_iColumnWidth[TP_LISTPROPERTY_COLUMN]; //宽度
	DWORD                       m_iColumnAlign[TP_LISTPROPERTY_COLUMN]; //对齐方式
	
protected:	
	TPCommonRes                *m_pLinePen[1];	
	TPCommonRes                *m_pCursor[2];	
	TPScrollBarData            *m_pScrollBar;
	SCROLLINFO                  m_sVScrollInfo;
	SCROLLINFO                  m_sHScrollInfo;

	int                         m_iDragMode;	
	CPoint                      m_ptDrag;	
	CPoint                      m_ptSave;
	CPoint                      m_ptMiddle;
	int                         m_iDragIndex;
	CRect                       m_rtInvert;
	CTPEdit                    *m_pEdit;
	CTPTimeCodeEditEx          *m_pTimeCode;
	CTPComboBox                *m_pComboBox;
	

public:	
//	树的标准函数
	virtual BOOL    Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);


	int             InsertItem(int nItem, LPCTSTR lpszItem);
	int             InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
	void            SetItemToolTip(int nItem,BOOL bEnable);



	virtual CString GetItemText(int nItem, int nSubItem) const;
	virtual int     GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;
	virtual BOOL    SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);


	int             GetItemCount() const;	

	int             GetTopIndex() const;
	void            SetTopIndex(int iIndex) ;

	BOOL            SetItemData(int nItem, DWORD_PTR dwData);
	DWORD_PTR       GetItemData(int nItem) const;


	BOOL            DeleteItem(int nItem);
	BOOL            DeleteAllItems();


	BOOL            EnsureVisible(int nItem, BOOL bPartialOK);	
	
	void                 SetRedraw(BOOL bRedraw = TRUE ); 
	BOOL                 SortItems(PFNLVCOMPARE pfnCompare, DWORD_PTR dwData); 

	
	void                 EditLable(int iItem,int iSubItem);	
	void                 GetEditItem(int &iItem,int &iSubItem,CWnd *&pWnd);
	void                 SetColumeEditType(int iIndex,int iCol,DWORD dEditType,long lMax=MAX_PATH);	
	BOOL                 SetColumnWidth(int nCol, int cx,BOOL bPaint = TRUE);
	TPListPropertyItem  *GetItem(int iIndex); 

	BOOL                 SetItemState(int nItem, UINT nState, UINT nMask);	
	UINT                 GetItemState(int nItem, UINT nMask) const;

	int                  HitTest(CPoint &point,int *iSubItem = NULL);
	BOOL                 HitTestDrag(CPoint &point,int &iColumn,int &iItem);

	void                 SetUserState(int iItem,long lState);  
	long                 GetUserState(int iItem);  

	CWnd                *CreateEditWnd(DWORD dwEditType);
	void                 DestoryEditWnd(CWnd *pWnd);


	void                 SetFont(CFont &pFont);

	int                  GetSelectItem();

protected:
	virtual void    SubWindow(BOOL bSub);
protected:
	void            PaintDC(CDC *pDC = NULL);
	void            ReLayOut();
	
	virtual void    DrawReport(CDC *pDC,int iItem);


	LRESULT         FunNotifyParent(UINT nNotify,int iItem,int iSubItem) ;
	
	virtual void    BeginEdit();
public:
	virtual void    EndEdit();
	virtual void    FunCalcSize();
protected:
	CRect           GetItemEditRect(int iItem,int iSubItem);
	int             GetColumnMaxWidth(int nCol);


	virtual void    FunCalcItemSize();
	virtual void    FunCalcScrollBar(); 
	virtual void    FunCalcEditItem();

	LRESULT         OnMessageSize(WPARAM wp,LPARAM lp); 

	void            SelectItem(int iItem,int iSubItem);

	LRESULT         OnMsgEditChange(WPARAM wp,LPARAM lp); 
	LRESULT         OnMsgtimeCodeChange(WPARAM wp,LPARAM lp);
	LRESULT         OnMsgScrollBar(WPARAM wp,LPARAM lp);
	BOOL            OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );

public:
	static  void    RegisterClass(BOOL bRegister);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEnKillFocus();
	afx_msg void OnEnSetFocus();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
};


