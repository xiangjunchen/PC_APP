#pragma once


// CTPDirListCtrl
#define WC_DIRLISTCTRL _L("DirListCtrl")

#define HEAD_NONE         0x0000
#define HEAD_NAME         0x0001
#define HEAD_TYPE         0x0002
#define HEAD_LENGTH       0x0003
#define HEAD_CREATETIME   0x0004
#define HEAD_WRITETIME    0x0005
#define HEAD_ACCESSTIME   0x0006
#define HEAD_ATTRIBUTE    0x0007

#define HEAD_MAX          0x0007

#define LIST_SELECT       0x00000001
#define LIST_UNSELECT     0x00000002

#define TP_DIRLIST_CTRLALL  0x00000001
#define TP_DIRLIST_SETDATA  0x00000002

class AFX_EXT_CLASS CTPDirListCtrl : public CTPListCtrl
{
	DECLARE_DYNAMIC(CTPDirListCtrl)

public:
	CTPDirListCtrl();
	virtual ~CTPDirListCtrl();
public:
	BOOL             m_bParentPath;
	BOOL             m_bDescSort;
	int              m_iSortIndex;
	CStringArray	 m_aFilter;

private:
	SHCONTF          m_eShconTf;
	ULONG            m_uSfGao;
	int              m_iHeadIndex[HEAD_MAX];
	LPITEMIDLIST     m_lpifqThis; 
	UINT             m_uState;  
	LPSHELLFOLDER    m_pShellFolder;
	ULONG			 m_uNoftifyChangeID;
	ULONG            m_ulAttrs;
	int				 m_iSelTick;

	CArray<LPVOID, LPVOID> m_aExceptItemID;
	CArray<int,int&>       m_aSelectItem;
	BOOL                   m_bSelectSort;

public:
	static void      RegisterClass(BOOL bRegister);
	void             SetHeadType(int iIndex[HEAD_MAX]);

	void             SetSHCONTF(SHCONTF uSet); 
	void             SetSFGO(ULONG uSet); 
	CString          GetPathName(int iItem = -1);
	CString          GetLinkPath(LPITEMIDLIST pItemID, LPITEMIDLIST* pItemIDLink = NULL);
	HRESULT          SetItemIDList(LPITEMIDLIST pItemIDList);	
	void             SortItem(int iItem,BOOL bDesc); 
	void             ClearData(); 
	DWORD            GetFileAttr(int iItem);
	LRESULT          SetPathIDLIST(LPITEMIDLIST  pItem );
	LPITEMIDLIST     GetPathIDLIST(int iItem = -1); 
	HRESULT          SetPathName(CString sPathName);
	void			 RefreshView();
	void			 RegisterFileChangeNoftiy(LPITEMIDLIST pItemID = NULL);
	void			 UnRegisterFileChangeNoftiy();
	void			 InitExceptItemID();
	void			 ReleaseExceptItemID();
	BOOL			 IsExceptItemID(LPITEMIDLIST pItemID);
	BOOL			 IsEqualItem(LPITEMIDLIST pItemID1, LPITEMIDLIST pItemID2);
	int              FindItemByID(LPITEMIDLIST hFindItemID);
	BOOL             DeleteAllItems();
	BOOL			 IsFolderItem(INT nIndex);
	BOOL			 CanMakeFolder();
	POSITION         GetFirstSelectedItemPosition(BOOL bSort = FALSE) ;
	int              GetNextSelectedItem(POSITION& pos) ;
	void             OnTimer(UINT nIDEvent);
	void			 GetLinkItem(LPITEMIDLIST pItemID,IPersistFile*& psf, IShellLink*& psl);

protected:
	HRESULT          SetParentItem();
	void             SetDesktopFolder();
	void             SetSpecialFolder(int csidl = CSIDL_DRIVES);	
	BOOL			 IsFolderItemID(LPITEMIDLIST pItemID);
protected:
	virtual void     SubWindow(BOOL bSub);
	LRESULT          OnMsgUnSubClassWindow(WPARAM wp,LPARAM lp);
	void             SetImageList(BOOL bSet);
	CString			 FormatErrorMsg(DWORD dwErrorCode);
private:	
	void             SetDirList(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpifq);
	void             SetItemText(int iItem);
	INT				 InsertListItem(INT nIndex, LPSHELLFOLDER pShellFolder, LPITEMIDLIST pParentID, LPITEMIDLIST pChildID);
	BOOL			 UpdateListItem(INT nIndex, LPSHELLFOLDER pShellFolder, LPITEMIDLIST pParentID, LPITEMIDLIST pChildID);
	BOOL             UpdateFolder(LPSHELLFOLDER pShellFolder, LPITEMIDLIST pParentID);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHdnItemclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnFileChangeNotify(WPARAM wParam, LPARAM lParam);
};


