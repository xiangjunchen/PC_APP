#pragma once


// CTPListCtrlEx
#define  WC_LISTCTRLEX _L("ListCtrlEx")


#define VIEWTYPE_ICON         1 
#define VIEWTYPE_REPORT       3 
#define VIEWTYPE_LIST         4 
#define VIEWTYPE_INFO         5 
#define VIEWTYPE_BUFFER       6 
#define VIEWTYPE_TEXTPICTURE  7 
#define VIEWTYPE_MAX		  8 

#define ARRANGE_FREE          2 
#define ARRANGE_STORYBOARD    3
#define ARRANGE_AUTO          4 


#define LIST_SIZECOLUMN     0x00000001 
#define LIST_DRAGCOLUMN     0x00000002 
#define LIST_CLICKCOLUMN    0x00000004
#define LIST_SELMORE        0x00000008
#define LIST_DRAGITEM       0x00000010
#define LIST_HOTITEM        0x00000020
#define LIST_HOTHEAD        0x00000040
#define LIST_SELECTITEM     0x00000080
#define LIST_OWNHEAD        0x00000100

#define STATE_SELECTED      0x0000001 
#define STATE_DRAGITEM      0x0000002 
#define STATE_DROPOUT       0x0000004 
#define STATE_NEWCATALOG    0x0000008
#define STATE_DROPIN        0x0000010 
#define STATE_BEGINDROP     0x0000020 
#define STATE_SIZE          0x0000040 
#define STATE_NORETURN      0x0000080 
#define STATE_CTRLSHIFT     0x0000100//qushaojie20100416 摁ctrl选中item时置此标志
#define STATE_SCROLL        0x0000200 
#define STATE_NOSENDNOTIFY  0x0200000 
#define STATE_TOOLTIP       0x0400000 
#define STATE_FILETERTEMP   0x0800000 
#define STATE_DBCLICK       0x1000000 
#define STATE_TOPVIEW       0x2000000 
#define STATE_HASDOWN       0x4000000 
#define STATE_NOROOT        0x8000000 

//add by qsj 20090803 初始化
#define TP_LIST_COLNUMMAX   9999
#define TP_LIST_ROWNUMMAX   9999

//add by qsj 20090803
#define TP_CLIPMOVETYPE_LEFT                 0x000001
#define TP_CLIPMOVETYPE_RIGHT                0x000002
#define TP_CLIPMOVETYPE_LRNOMOVE             0x000004
#define TP_CLIPMOVETYPE_UP                   0x000010
#define TP_CLIPMOVETYPE_DOWN                 0x000020
#define TP_CLIPMOVETYPE_UDNOMOVE             0x000040
#define TP_CLIPDRAGTYPE_MOVE                 0x000080
#define TP_CLIPDRAGTYPE_COPY                 0x000100

typedef struct _tagTPListCtrlColumn
{
	int                    iIndex;               //当前索引
	int                    iIndexInit;           //初始化索引
	int                    iWidth;               //宽度
	int                    iImage;               //图片索引
	CString                sItem;                //文本
	CRect                  rtItem;               //位置	
	BOOL                   bShow;                //是否显示	
	DWORD                  dwData;               //缺省数据 
	DWORD                  dEditType;
	long                   lMax;
	UINT                   uFmt; 
	long                   lMinWidth;  
	long                   lMaxWidth;
	UINT                   uState;

	_tagTPListCtrlColumn()
	{
		bShow      = TRUE;	
		iIndex     = 0 ;
		iIndexInit = 0;
		iWidth     = 100;		 
		iImage     = -1;
		sItem      = _L("");
		rtItem     = CRect(0,0,0,0);		
		dwData     = 0;		
		uFmt       = 0;
		dEditType  = 0;
		lMax       = -1;
		lMinWidth  = 0;  
		lMaxWidth  = 0x7ffff;
		uState     = 0;
	}
	void CalcSize()
	{
	}
	~_tagTPListCtrlColumn()
	{
		sItem.Empty(); 		
		
	}
}TPListCtrlColumn;
typedef CArray<TPListCtrlColumn *,TPListCtrlColumn * &> TPListCtrlColumnArray;

typedef struct _tagTPListCtrlSubItem
{
	int                    iImage;       //图片
	CString                sText;        //文本
	CRect                  rtItem;       //位置
	CRect                  rtItemSave;       //位置
	CRect                  rtDrag;
	int                    iAlign;       //对齐方式	
	LPARAM                 dwData;       //数据
	int                    iBackColor;   //背景颜色		
	CImageList             hImagList;    //独立的图片系列  
	UINT                   uState;
	int                    iIndent;


	CRect                  rtIcon;       //Image 位置 
	CRect                  rtLable;      //文本位置
	CRect                  rtCheck;      //Check位置	
	_tagTPListCtrlSubItem()
	{
		iAlign     = DT_LEFT;
		iImage     = -1;
		sText      = _L("");
		rtItem     = CRect(0,0,0,0);
		iBackColor = -1; 
		dwData     = 0;		
		uState     = 0;
		iIndent    = 0;	

		rtItem     = CRect(0,0,0,0);
		rtIcon     = CRect(0,0,0,0);
		rtLable    = CRect(0,0,0,0);
		rtCheck    = CRect(0,0,0,0); 
		rtDrag     = CRect(0,0,0,0); 

	}
	void CalcSize()
	{
	}
	void EmptySubItem()
	{
		rtItem     = CRect(0,0,0,0);
		rtIcon     = CRect(0,0,0,0);
		rtLable    = CRect(0,0,0,0);
		rtCheck    = CRect(0,0,0,0); 
		rtDrag     = CRect(0,0,0,0); 
	}
	~_tagTPListCtrlSubItem()
	{
		sText.Empty();
		if(hImagList.m_hImageList) 		
			hImagList.DeleteImageList();
	}
}TPListCtrlSubItem;
typedef CArray<TPListCtrlSubItem *,TPListCtrlSubItem *&> TPListCtrlSubItemArray;
struct _tagTPListCtrlItemBase
{
	long           lIconIndex; 
	long           lReportIndex; 
	_tagTPListCtrlItemBase()
	{
		lIconIndex   = -1; 
		lReportIndex = -1;  
	}
};
typedef struct _tagTPListCtrlItem:public _tagTPListCtrlItemBase
{
	int                    iIndex;     //当前索引
	TPListCtrlSubItemArray aSubItem;   //子项
	BOOL                   bCheck;
	DWORD                  dwSelTick;  //选中的时间		
	LPARAM                 dwData;     //数据
	GUID                   guidData;   //GUID
	int                    iBackColor; //背景颜色		
	UINT                   uState;
	UINT                   uStateOld;
	BOOL                   bShow;
	UINT                   uNotify;
	int                    iItemHeight;  //ListItem 高度

	LPBYTE                 pIcon;       //Icon Buffer
	CSize                  szIcon;      //Icon 大小

	_tagTPListCtrlItem()
	{
		iIndex              = -1;
		bCheck              = FALSE; 
		dwData              = 0;
		guidData            = GUID_NULL; 
		iBackColor          = -1;
		pIcon               = NULL;
		szIcon              = CSize(0,0);	
		iItemHeight			= 0;
		uState              = 0;
		uStateOld           = 0;
		bShow               = TRUE;
		uNotify             = 0;
	}
	void DeleteSubItem(int iSub)
	{
		if(iSub<0 || iSub>= aSubItem.GetSize()) return ;
		delete aSubItem[iSub];
		aSubItem.RemoveAt(iSub);
	}
	void InsertSubItem(int iSub)
	{
		TPListCtrlSubItem *addData = new TPListCtrlSubItem;
		aSubItem.InsertAt(iSub,addData);
	}
	void SetSubItemCount(int iCount)
	{
		TPListCtrlSubItem *addData = NULL;
		for(int i=0;i<iCount;i++)
		{
			addData = new TPListCtrlSubItem();  
			aSubItem.Add(addData);
		}
	}
	void EmptySubItem()
	{
		for(INT i = 0; i < aSubItem.GetSize(); i++)
			aSubItem[i]->EmptySubItem();
	}
	void SetItemText(int iCol,CString str,int iAlign)
	{
		if( iCol >= aSubItem.GetSize() || iCol <0) return;
		aSubItem[iCol]->sText  = str;
		aSubItem[iCol]->iAlign  = iAlign;
	}
	int  GetRight()
	{
		int iMax = 0;
		for(INT l= (int)aSubItem.GetSize()-1;l>=0;l--)
		{
			iMax = max(iMax,aSubItem[l]->rtItem.right);
		}
		return iMax;
	}
	void SetItemImage(int iCol,int iImage)
	{
		if( iCol >= aSubItem.GetSize() || iCol <0) return;
		aSubItem[iCol]->iImage = iImage;		
	}
	COLORREF GetItemColor(int iCol)
	{
		if( iCol >= aSubItem.GetSize()|| iCol <0 ) return RGB(0,0,0);
		else return TP_GetSysColor(aSubItem[iCol]->iBackColor);
	}
	void SetItemColor(int iCol,int iColorIndex)
	{
		if( iCol >= aSubItem.GetSize()||iCol <0) return;
		aSubItem[iCol]->iBackColor  = iColorIndex;		
	}
	CString GetItemText(int iCol)
	{
		if( iCol >= aSubItem.GetSize() || iCol <0) return _L("");		
		else return aSubItem[iCol]->sText;
	}
	void	SetItemHeight(int iHeight)
	{
		iItemHeight = iHeight;
	}	
	~_tagTPListCtrlItem()
	{
		for(int i=0;i<aSubItem.GetSize();i++) delete aSubItem[i];
		aSubItem.RemoveAll(); 
		if(pIcon) delete   pIcon;
		pIcon = NULL;
	}
}TPListCtrlItem;

typedef CArray<TPListCtrlItem *,TPListCtrlItem * &> TPListCtrlItemArray;

typedef struct _tagTPListCtrlHitTest
{
	BOOL   bHead;
	BOOL   bImage;
	BOOL   bLable;	
	BOOL   bCheck;
	int    iHeadFlag;
	int    iItem;
	int    iSubItem;
	BOOL   bDrag; 
	LPVOID pData;
	_tagTPListCtrlHitTest()
	{
		Clear();
	}
	void Clear()
	{
		bHead     = FALSE; 
		bImage    = FALSE;
		bLable    = FALSE;
		bCheck    = FALSE;	
		iHeadFlag = -1;
		iItem     = -1;
		iSubItem  = -1;
		pData     = NULL;
		bDrag     = TRUE;
	}
} TPListCtrlHitTest;

#undef CRectArray
typedef CArray<CRect,CRect&> CRectArray;

class CTPEditEx;
class AFX_EXT_CLASS CTPListCtrlEx : public CTPWnd,public CTPBaseCtrl 
{
	DECLARE_DYNAMIC(CTPListCtrlEx)

public:
	CTPListCtrlEx();
	virtual ~CTPListCtrlEx();
public:	
	UINT                    m_uPrivateStyle;
	CWnd                   *m_pMsgWnd; 	
	long                    m_iViewType;   //显示类型
	long                    m_iArrType;    //排列类型
	long                    m_lIconSize;

	CTPEditEx              *m_pEdit;
	BOOL                    m_bEditItem;	
	BOOL                    m_bDrawFocus;
	CWnd                  **m_pFocusWnd;  

	static	BOOL		   m_bCreateLargeIcon;

protected:	
	BOOL                    m_bSubWindow; 
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	UINT                    m_uStyleEx;
	TP_BORDER_STYLE         m_eBorderStyle;
	int                     m_iBackColor;	 
	UINT                    m_uState;
	UINT                    m_iCheckIndex;
	CTPToolTipCtrl         *m_pToolTipCtrl; 
	CString                 m_sEditLabel;
	int                     m_iWidthCheck;
	


//编辑专用
	int                     m_iItemSave;
	int                     m_iItemSubSave;
	int                     m_iItemLast;
	int                     m_iItemSubLast;
	CRect                   m_rtItemSubSave;		

	CWnd                   *m_pEditWnd;
	CRectArray              m_aInvertRect; 
	BOOL                    m_bLeftDrag;
	CMutex                  m_hMutex; 
	CBitmap                *m_pDragBitmap;

public:
	TPListCtrlColumnArray   m_aColumn;      //表头
	TPListCtrlItemArray     m_aItem;        //每条记录
protected:
	TPListCtrlItemArray     m_aSelItem;     //选中记录
	int                     m_iSelItem;

	CImageList              m_imgNormal;    //位图
	CImageList              m_imgSmall;
	CImageList              m_imgState;
	CImageList              m_imgHead;


	int                     m_iHotItem;     //当前Tracking 的位置
	BOOL                    m_bReDraw;      //是否需要重画
	BOOL                    m_bHeadMenu;
	BOOL                    m_iHeadMenu;
	

	int                     m_iHeadOver;
	int                     m_iHeadDown;


	CSize                   m_szItemList;
	CSize                   m_szItemIcon;
	CSize                   m_szItemReport;
	CSize                   m_szItemTextPicture;
	CSize                   m_szItemSmallIcon;
	CSize                   m_szItemInfo;
	CSize                   m_szItemBuffer;

	CSize                   m_szEdge;
	CSize                   m_szSpace;
	CRect                   m_rtClient;
public:
	int                    m_iTopX;
	int                    m_iTopY;
protected:

	int                    m_iTotalHeight;
	int                    m_iTotalWidth;
	int                    m_iBarHeight;
	int                    m_iBarWidth;
	int                    m_iColumnHeight;
	int                    m_iLockColumn;
	int                    m_iLockDragColumn;
	int                    m_iCountPerLine;
	CSize                  m_szTextSize;
	CSize                  m_szTemp;
	TPListCtrlHitTest      m_hitTest;

	int                    m_iDragMode;
	CRect                  m_rtInvert;
	CPoint                 m_ptDrag;
	CPoint                 m_ptMiddle;
	CPoint                 m_ptSave;
	int                    m_iDragIndex;
	CImageList            *m_pDragImageList; 
	BYTE                   m_bDragInWnd;
	CPoint                 m_ptDragPos;
	HICON                  m_hDragCursor; 
	UINT_PTR               m_hTimer[5];
	BOOL                   m_bCalcScroll;      //是否需要重画

public:

	TPScrollBarData         *m_pScrollBar;

protected:
	TPCommonRes             *m_pImageRes[2];
	TPCommonRes             *m_pHeaderBmp[5]; 
	TPCommonRes             *m_pCheckBox;
	TPCommonRes             *m_pLinePen[2];
	TPCommonRes             *m_hCursorDrag[4];
	SCROLLINFO              m_sVScrollInfo;
	SCROLLINFO              m_sHScrollInfo;

	CPoint                  m_ptScrollPos;
	


public:	
//	树的标准函数
	virtual BOOL    Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);	
	
	DWORD           SetExtendedStyle(DWORD dwNewStyle); 
	CImageList*     GetImageList(int nImageList) const;
	CImageList*     SetImageList(CImageList* pImageList, int nImageListType);
	
	int             GetItemCount() const;	
	
	BOOL            SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,int nImage, UINT nState, UINT nStateMask, LPARAM lParam,int iIndent = 0);

	int             GetNextItem(int nItem, int nFlags);
	
	POSITION        GetFirstSelectedItemPosition(BOOL bSort = TRUE) ;
	int             GetNextSelectedItem(POSITION& pos) ;
	POSITION        GetLastSelectedItemPosition();
	void            OnRButtonUp(UINT nFlags, CPoint point);

	BOOL            GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const;

	BOOL            SetItemPosition(int nItem, POINT pt);
	BOOL            GetItemPosition(int nItem, LPPOINT lpPoint) const;

	int             GetStringWidth(LPCTSTR lpsz) ;

	int             GetItemOrder(int nItem);
	int             GetItemByOrder(int nOrder);
	BOOL            GetColumnOrderArray(LPINT piArray, int iCount = -1 );
	BOOL            SetColumnOrderArray(LPINT piArray, int iCount );
	int             GetColumnWidth(int nCol) const;
	BOOL            SetColumnWidth(int nCol, int cx);
	void            SetColumnText(int iItem,LPCTSTR sText);
	void            SetColumnFormat(int nCol,UINT uFormat,UINT uMask);

	BOOL            SetColumnState(int nItem, UINT nState, UINT nMask);
	UINT            GetColumnState(int nItem, UINT nMask) const;

	int             GetTopIndex() const;
	void            SetTopIndex(int iIndex) ;
	int             GetCountPerPage() const;

	BOOL            SetItemState(int nItem, UINT nState, UINT nMask);
	UINT            GetItemState(int nItem, UINT nMask) const;

	virtual CString GetItemText(int nItem, int nSubItem) const;
	virtual int     GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;
	virtual BOOL    SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);

	void            SetItemCount(int nItems);
	BOOL            SetItemData(int nItem, DWORD_PTR dwData);

	DWORD_PTR       GetItemData(int nItem) const;

	UINT            GetSelectedCount() const;

	BOOL            GetSubItemRect(int iItem, int iSubItem, int nArea, CRect& ref);

	int             GetHotItem();
	int             SetHotItem(int iIndex);

	int             SubItemHitTest(LPLVHITTESTINFO pInfo);

	BOOL            GetCheck(int nItem) const;
	BOOL            SetCheck(int nItem, BOOL fCheck = TRUE);

	int             InsertItem(int nItem, LPCTSTR lpszItem, int iHeight = -1);
	int             InsertItem(int nItem, LPCTSTR lpszItem, int nImage,int iHeight = -1);

	BOOL            DeleteItem(int nItem);
	BOOL            DeleteAllItems();

	int             HitTest(CPoint pt, UINT* pFlags = NULL) const;

	BOOL            EnsureVisible(int nItem, BOOL bPartialOK);

	int             InsertColumn(int nCol, LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	int             InsertColumn(int nCol, const LVCOLUMN* pColumn);
	BOOL            DeleteColumn(int nCol);
	void            GetItem(int nItem,HDITEM *hdi);
	int             GetColumnCount();
	
	CImageList*     CreateDragImage(int nItem, LPPOINT lpPoint);
	HCURSOR         CreateDragIcon(int nItem, COLORREF clMask,BOOL bEndCreate,LPPOINT lpPoint = NULL,HCURSOR hCursor = NULL,BYTE bTrans = 0x4f);
	CImageList*     CreateHeadDragImage(int nItem);
	void			ReleaseDragBitmap();

	BOOL            SortItems(PFNLVCOMPARE pfnCompare, DWORD_PTR dwData); 
	//{liubo add CD2009.10.01 PE 刷新功能优化 V4.5Feature
#ifdef TP_NS_NET
	BOOL            SortItems(PFNLVCOMPARE pfnCompare, DWORD_PTR dwData, int index); 
#endif
	//}

	void            SetRedraw(BOOL bRedraw = TRUE ); 
	void            ReLayOut();
	void            EditLable(int iItem,int iSubItem);
	void            SetColumeEditType(int iCol,DWORD dEditType,long lMax = -1);
	void            GetEditItem(int &iItem,int &iSubItem,CWnd *&pWnd);
	TPListCtrlItem *GetItem(int iIndex); 
	
	void            SetColumnFmt(int iColumn,UINT uFlagAdd,UINT uFlagRemove);

	void            SetScrollPosKeep(BOOL bSave);  

	void            SelectItem(int iIndex,BOOL bSelect,BOOL bUnSelAll);

	void           SetSelectedColumn(int iCol);
	void           SetItemTextColor(int iItem,TP_SYSTEM_COLOR clText);

	void           SetItemView(int iItem,BOOL bShow);

	static HCURSOR     CreateDragIcon(LPBYTE pBuffer,CSize szBuffer,COLORREF clMask, LPPOINT lpPoint,HCURSOR hCursor,BYTE bTrans);
	static CImageList *CreateDragImage(LPBYTE pSrcBuffer,CSize szBuffer,COLORREF clMask,BYTE bTrans);

protected:
	virtual void    SubWindow(BOOL bSub);

	virtual void    DrawIcon(CDC *pDC,int iItem);	
	virtual void    DrawBuffer(CDC *pDC,int iItem);	
	virtual void    DrawList(CDC *pDC,int iItem);	
	virtual void    DrawInfo(CDC *pDC,int iItem);	
	virtual void    DrawReport(CDC *pDC,int iItem);
	virtual void    DrawTextPicture(CDC *pDC,int iItem);
	virtual void    DrawColumn(CDC *pDC,int iColumn);	

	virtual void    BeginEdit();
	virtual void    EndEdit();
	virtual void    CancleEdit();
	void            BeginEditTest(int iItem,int iSubItem);
	CRect           GetItemEditRect(int iItem,int iSubItem);	
	CRect           GetIconRect(int iItem);
	virtual void    ChangeColume(int iType);
	virtual void    ClearSortFlag(INT iItem);

	void            OnMenuCmd(UINT nID);



	virtual void    SortItem(int iHeadIndex,BOOL bAsc);
	virtual void    GetItemResData(int iItem);

protected:

	void            LoadRes();
	virtual void    PaintHead(CDC *pDC);
	virtual void    PaintDC(CDC *pDC = NULL);
	void            PaintInvert(CRectArray &aInvert);

	LRESULT         FunNotifyParent(UINT nNotify,int iItem,int iSubItem) ;
	LRESULT         FunNotifyParentAll() ;

	int             FunGetHeadInsertIndex(CPoint point);

    CBitmap        *CreateDragBitmap(int nItem, LPPOINT lpPoint);
	
	virtual void    FunCalcHeadSize();
	virtual void    FunCalcItemSize();
	virtual void    FunCalcScrollBar(); 
	virtual void    FunCalcSize();
	virtual void    FunCalcEditItem();
	virtual void    FunCloumnHide();

	void            FunMoveColumn(int iSrcIndex,int iDestIndex);
	int             FunGetCurSel(CTPIntArray &aSel);
	void            FunSelectItem(int iHitTest,BOOL bDown);
	void            FunMoveSelectItemTo(int iItem);
	void            FunDrawCorner(CDC *pDC, CRect rtCorner,DWORD dwMode);
	void            FunReSortItem();
	void            FunDrawLine(CDC *pDC,CRect rtItem);
	virtual void    FunAdjColumnWidth(int iColumnIndex);

	virtual void    FunSortExItem(TPListCtrlItemArray &aItem, DWORD dwMode);
	void            FunSortbyIconIndex(void);
	int             FunPerLineItemCount(void);
	CRect           FunItemRectByIndex(long IconIndex);
	int             FunHitTestIcon(CPoint ptTest);
	void            FunItemIconIndex(BOOL bReOrder /*= FALSE*/, BOOL bCheckSort = FALSE);
	BOOL            FunHitTest(CPoint point, TPListCtrlHitTest *pData) const;
	int             FunMoveClip(POINT point, BOOL bHitTest, CRect& SquareRect,UINT uDragType);
	int				FunEndSel(UINT nChar, int iSel);
	void            FunSortItemByIndex(DWORD dwViewType);
	void            FunGetItemInScreen(TPListCtrlItemArray &aList,CRect &rtClient);
	void            FunGetSelectItemInScreen(int nItem,TPListCtrlItemArray &aList,CRect &rtClient,LPPOINT ptCutsor,int iLevel = 1);

	void            OnLButtonDblClkLargeIcon(UINT nFlags, CPoint point);	
	void            OnLButtonDblClkReport(UINT nFlags, CPoint point);
	void            OnLButtonDblClkBuffer(UINT nFlags, CPoint point);
	void            OnLButtonDblClkInfo(UINT nFlags, CPoint point);
	void            OnLButtonDblClkList(UINT nFlags, CPoint point);

	void            OnLButtonDownLargeIcon(UINT nFlags, CPoint point);
	void            OnLButtonDownInfo(UINT nFlags, CPoint point);
	void            OnLButtonDownReport(UINT nFlags, CPoint point);
	void            OnLButtonDownBuffer(UINT nFlags, CPoint point);
	void            OnLButtonDownList(UINT nFlags, CPoint point);

	void            OnLButtonUpLargeIcon(UINT nFlags, CPoint point);
	void            OnLButtonUpInfo(UINT nFlags, CPoint point);
	void            OnLButtonUpReport(UINT nFlags, CPoint point);
	void            OnLButtonUpBuffer(UINT nFlags, CPoint point);
	void            OnLButtonUpList(UINT nFlags, CPoint point);

	void            OnMouseMoveLargeIcon(UINT nFlags, CPoint point);
	void            OnMouseMoveInfo(UINT nFlags, CPoint point);
	void            OnMouseMoveReport(UINT nFlags, CPoint point);
	void            OnMouseMoveBuffer(UINT nFlags, CPoint point);
	void            OnMouseMoveList(UINT nFlags, CPoint point);


	LRESULT         OnMessageSize(WPARAM wp,LPARAM lp); 
	LRESULT         OnMessageToolTops(WPARAM wp,LPARAM lp); 
	LRESULT         OnMessageTimeCode(WPARAM wp,LPARAM lp); 
	
	INT				GetPreViewItem(INT nIndex);
	INT				GetNextViewItem(INT nIndex);
	BOOL			IsViewItem(INT nIndex);




public:
	static  void            RegisterClass(BOOL bRegister);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStyleChanged(int nStyleType,	LPSTYLESTRUCT lpStyleStruct);
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
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnEnKillfocus();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


