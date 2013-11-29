#pragma once


// CTPDirTreeCtrl
#define WC_DIRTREECTRL _L("DirTreeCtrl")
#define TP_SYSIMAGELIST   2

typedef CArray<HTREEITEM, HTREEITEM> CHTREEITEMArray;

class AFX_EXT_CLASS CTPDirTreeCtrl : public CTPTreeCtrl
{
	DECLARE_DYNAMIC(CTPDirTreeCtrl)

public:
	CTPDirTreeCtrl();
	virtual ~CTPDirTreeCtrl();
private:	
	SHCONTF                 m_eShconTf;
	ULONG                   m_uSfGao;
	ULONG					m_uNoftifyChangeID;
	LPSHELLFOLDER			m_pShellDestop;
	CArray<LPVOID, LPVOID>  m_aExceptItemID;

public:
	static HIMAGELIST       m_hImagelist[TP_SYSIMAGELIST];
public:
	static  void     RegisterClass(BOOL bRegister);
	static  void     LoadSysImageList(BOOL bLoad);

	void             SetSHCONTF(SHCONTF uSet); 
	void             SetSFGO(ULONG uSet);

	CString          GetPathName(HTREEITEM hItem = NULL);
	CString          GetLinkPath(CString sPathName);
	HTREEITEM        SetPathName(CString sPathName);
	LPITEMIDLIST     GetPathIDLIST(HTREEITEM hItem = NULL); 
	LRESULT          SetPathIDLIST(LPITEMIDLIST  pItem ,CString &sText);
	ULONG			 RegisterFileChangeNoftiy();
	void			 UnRegisterFileChangeNoftiy(ULONG uNotifyID);
	void             SetDesktopFolder();
	BOOL             DeleteAllItems();
protected:
	virtual void     SubWindow(BOOL bSub);
	LRESULT          OnMsgUnSubClassWindow(WPARAM wp,LPARAM lp);
	void             SetImageList(BOOL bSet);
private:
	void			 SetDestopPath();
	void			 SetDirTree(HTREEITEM hItem);
	void			 SetDirTree(IShellFolder* pParentFolder, LPITEMIDLIST pParentItem, HTREEITEM hParentItem);
	void			 FreeItemIDList(LPITEMIDLIST pItemIDList);
	BOOL			 IsExceptItemID(LPITEMIDLIST pItemID);
	void			 InitExceptItemID();
	void			 ReleaseExceptItemID();
	void			 UpdateExpandItem(HTREEITEM hItem);
	HTREEITEM		 InsertTreeItem(HTREEITEM hParent, LPSHELLFOLDER pParentFolder, LPCITEMIDLIST pParentItemID, LPITEMIDLIST pChildItem);
	LPITEMIDLIST	 GetFullPathItemIdList(HTREEITEM hItem);
	HTREEITEM        FindItemID(HTREEITEM hItem, LPITEMIDLIST hFindItemID,BOOL bID = FALSE);
	void             FindItemID(HTREEITEM hItem, LPITEMIDLIST hFindItemID, CHTREEITEMArray& aFoundItem,BOOL bID = FALSE);
	LPSHELLFOLDER	 GetItemFolder(HTREEITEM hItem);
	void             UpDateItem(HTREEITEM hItem);
	BOOL             DeleteItemData(HTREEITEM hItem);
	BOOL			 IsEqualItem(LPITEMIDLIST pItemID1, LPITEMIDLIST pItemID2, BOOL bID);
	BOOL			 IsParentItem(LPITEMIDLIST pParentItem, LPITEMIDLIST pChildItem, BOOL bID);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnFileChangeNotify(WPARAM wParam, LPARAM lParam);
};


