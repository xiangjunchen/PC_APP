// TPDirTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPDirTreeCtrl.h"
#include ".\tpdirtreectrl.h"
#include "shlobj.h"

#define WM_FILECHANGE_NOTIFY  WM_USER + 2000

typedef struct _tagTPDirTreeItemData
{
	BOOL					   bSetDataNew;
	LPITEMIDLIST			   pItemList;
	LPITEMIDLIST			   pFullItemID;
	BOOL					   bDestop;
	ULONG                      ulAttrs;
	ULONG					   uNoftifyChangeID;
	_tagTPDirTreeItemData()
	{
		Reset();
	}
	~_tagTPDirTreeItemData()
	{
		Release();
	}
	void Reset()
	{
		bDestop			 = FALSE;
		bSetDataNew		 = FALSE;
		pFullItemID		 = NULL;
		ulAttrs			 = 0;
		uNoftifyChangeID = 0;
	}
	void Release()
	{
		if(pItemList) {ILFree(pItemList); pItemList = NULL;}
		if(pFullItemID) {ILFree(pFullItemID); pFullItemID = NULL;}
	}
} TPDirTreeItemData, *LPTPDirTreeItemData;

// CTPDirTreeCtrl
HIMAGELIST CTPDirTreeCtrl::m_hImagelist[TP_SYSIMAGELIST]  = {NULL,NULL};

IMPLEMENT_DYNAMIC(CTPDirTreeCtrl, CTPTreeCtrl)
CTPDirTreeCtrl::CTPDirTreeCtrl()
{	
	m_eShconTf			= SHCONTF_FOLDERS ;	
	m_pShellDestop		= NULL;
	m_uNoftifyChangeID  = 0;
	m_uSfGao            =  SFGAO_FOLDER;
		
	InitExceptItemID();

	
}

CTPDirTreeCtrl::~CTPDirTreeCtrl()
{
	if(m_pShellDestop)
	{
		m_pShellDestop->Release();
		m_pShellDestop = NULL;
	}
	ReleaseExceptItemID();
	
}

BEGIN_MESSAGE_MAP(CTPDirTreeCtrl, CTPTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnTvnDeleteitem)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)
	ON_WM_DESTROY()

	ON_MESSAGE(WM_UNSUBCLASSWINDOW,OnMsgUnSubClassWindow)
	ON_MESSAGE(WM_FILECHANGE_NOTIFY,OnFileChangeNotify)
END_MESSAGE_MAP()

void     CTPDirTreeCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_TREEVIEW,&wndClass);		
		wndClass.lpszClassName = WC_DIRTREECTRL;			
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_DIRTREECTRL,AfxGetInstanceHandle());
}
HIMAGELIST TP_CopyIamgetList(HIMAGELIST &hSrc)
{
	int cx = 0,cy = 0,iCount = 0;
	ImageList_GetIconSize(hSrc,&cx,&cy);
	iCount = ImageList_GetImageCount(hSrc);
	HIMAGELIST hDest = ImageList_Create(cx,cy,ILC_COLOR24,iCount,1);
	for(INT l=0;l<iCount;l++)
		ImageList_Copy (hDest,l,hSrc,l,ILCF_MOVE);
	return hDest;
}

void     CTPDirTreeCtrl::LoadSysImageList(BOOL bLoad)
{
	if(bLoad)
	{	
		SHFILEINFO  sfi;		
		ZeroMemory(&sfi,sizeof(SHFILEINFO));
		m_hImagelist[0]        = (HIMAGELIST)SHGetFileInfo(_L(""),0,&sfi,sizeof(SHFILEINFO),SHGFI_SYSICONINDEX | SHGFI_LARGEICON);		
		ZeroMemory(&sfi,sizeof(SHFILEINFO));
		m_hImagelist[1]        = (HIMAGELIST)SHGetFileInfo(_L(""),0,&sfi,sizeof(SHFILEINFO),SHGFI_SYSICONINDEX | SHGFI_SMALLICON);	
		//iCount = ImageList_GetImageCount(hGetImage);
		//m_hImagelist[1]  = ImageList_Duplicate(hGetImage);	
#ifdef _DEBUG
		if(ImageList_GetImageCount(m_hImagelist[0]) <= 4) ASSERT(FALSE);
		if(ImageList_GetImageCount(m_hImagelist[1]) <= 4) ASSERT(FALSE);
#endif
	}
	else
	{
		int iCount = ImageList_GetImageCount(m_hImagelist[0] );
		ASSERT(iCount>0);
		for(INT l=0;l<TP_SYSIMAGELIST;l++)
		{
			//if(m_hImagelist[l]) ImageList_Destroy(m_hImagelist[l]);
			m_hImagelist[l] = NULL;
		}
	}
}
void CTPDirTreeCtrl::SubWindow(BOOL bSub)
{
	if(bSub )
	{	
		if(m_bSubWindow) return;
		CTPTreeCtrl::SubWindow(bSub);
		CTPTreeCtrl::ModifyStyle(0, TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_HASLINES|WS_BORDER,TRUE);		
		SetImageList(TRUE);
		GetClientRect(&m_rtClient);
		m_uNoftifyChangeID = RegisterFileChangeNoftiy();

	}
	else
	{		
		if(!m_bSubWindow) return;
		SetImageList(FALSE);
		UnRegisterFileChangeNoftiy(m_uNoftifyChangeID);
		m_uNoftifyChangeID = 0;
		CTPTreeCtrl::SubWindow(bSub);		
	}
}

void CTPDirTreeCtrl::SetImageList(BOOL bSet)
{	
	if(bSet)
	{			
		if(m_hImagelist[0]!=NULL) 	
		{
			CTPTreeCtrl::SetImageList(CImageList::FromHandle(m_hImagelist[1]),TVSIL_NORMAL);
			CTPTreeCtrl::SetImageList(CImageList::FromHandle(m_hImagelist[1]),TVSIL_STATE);			
		}
	}
	else
	{
		if( m_hImagelist[0])
		{
			CTreeCtrl::SetImageList(NULL,TVSIL_NORMAL);
			CTreeCtrl::SetImageList(NULL,TVSIL_STATE);		
		}		
	}
}

void CTPDirTreeCtrl::SetSHCONTF(SHCONTF uSet)
{
	m_eShconTf  = uSet;
}
void CTPDirTreeCtrl::SetSFGO(ULONG uSet)
{
	m_uSfGao    = uSet;
}

void CTPDirTreeCtrl::OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;
	if(!TP_IsNull(pNMTreeView ->itemOld.lParam))
	{	
		LPTPDirTreeItemData pItemData  = (LPTPDirTreeItemData)pNMTreeView ->itemOld.lParam; 
		delete pItemData;
	}	
}
void CTPDirTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;	

	if(STATE_SETDATA & m_uState) return;
	if(!TP_IsNull(pNMTreeView ->itemNew.lParam))
	{
		TPDirTreeItemData *pItemData = (TPDirTreeItemData*)pNMTreeView->itemNew.lParam;
		if(!pItemData->bSetDataNew) SetDirTree(pNMTreeView->itemNew.hItem);
	}
}
HTREEITEM CTPDirTreeCtrl::SetPathName(CString sPathName)
{	
	HTREEITEM hTreeItem = NULL;
	LPITEMIDLIST pFullItemID = NULL;
	CString sTemp = GetPathName(GetRootItem());
	if(sPathName.Find(sTemp) >= 0 && (sPathName.CompareNoCase(sTemp) != 0) && PathFileExists(sPathName))
	{
		int iFind = sPathName.GetLength() - sTemp.GetLength() - 1;
		CString sTemp = sPathName.Right(iFind);
		TCHAR szParseName[MAX_PATH] = {0};
		lstrcpyn(szParseName, sTemp, MAX_PATH);
		m_pShellDestop->ParseDisplayName(m_hWnd, NULL, szParseName, NULL,&pFullItemID, NULL);
	}
	else
		SHILCreateFromPath(sPathName, &pFullItemID, NULL);
	if(NULL != pFullItemID)
	{
		hTreeItem = FindItemID(GetRootItem(),pFullItemID);
		if(NULL == hTreeItem) hTreeItem = GetRootItem();
		m_uState |= STATE_SETDATA;
		SelectItem(hTreeItem);
		m_uState &= ~STATE_SETDATA;
		EnsureVisible(hTreeItem);
		ILFree(pFullItemID);
	}
	
	return hTreeItem;
}
LPITEMIDLIST CTPDirTreeCtrl::GetPathIDLIST(HTREEITEM hItem)
{
	if(NULL == hItem) hItem = GetSelectedItem();
	if(NULL == hItem) hItem = GetRootItem();
	TPDirTreeItemData *pItemData = (TPDirTreeItemData*)GetItemData(hItem);
	if(NULL == pItemData) return NULL;
	if(pItemData->pFullItemID) {ILFree(pItemData->pFullItemID); pItemData->pFullItemID = NULL;}
	pItemData->pFullItemID = GetFullPathItemIdList(hItem);
	return pItemData->pFullItemID;	
}
LRESULT CTPDirTreeCtrl::SetPathIDLIST(LPITEMIDLIST  pItem,CString &sText)
{
	sText = _T("");
	if(NULL != pItem)
	{
		HTREEITEM hTreeItem = FindItemID(GetRootItem(),pItem);
		if(NULL == hTreeItem) hTreeItem = GetRootItem();
		m_uState |= STATE_SETDATA;
		SelectItem(hTreeItem);
		m_uState &= ~STATE_SETDATA;
		EnsureVisible(hTreeItem);
		sText = GetItemText(hTreeItem);
	}

	return S_OK;
}

CString CTPDirTreeCtrl::GetLinkPath(CString sPathName)
{
	CString sLinkPathName = _T("");
	TCHAR szTempPath[MAX_PATH] = {0};
	 wcscpy_s (szTempPath, sPathName);
	TCHAR szLinkPath[MAX_PATH] = {0};
	 wcscpy_s (szLinkPath, sPathName);
	TCHAR szDesPath[MAX_PATH] = {0};
	IShellLink* psl;

	HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,IID_IShellLink, (LPVOID*) &psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;
		hres = psl->QueryInterface( IID_IPersistFile, (LPVOID *) &ppf);
		if(SUCCEEDED(hres))
		{
			hres = ppf->Load(szLinkPath, 0);
			if (SUCCEEDED(hres))
			{
				psl->GetPath(szTempPath, MAX_PATH, NULL, SLGP_UNCPRIORITY);
				 wcscpy_s (szDesPath, szTempPath);	
			}
		}
		if(ppf)
			ppf->Release();
	}
	if(psl)
		psl->Release();
	sLinkPathName = szDesPath;
	return sLinkPathName;
}

CString CTPDirTreeCtrl::GetPathName(HTREEITEM hItem)
{
	CString szFolderPath = _L("");
	if(NULL == hItem) hItem = GetSelectedItem();
	LPITEMIDLIST pFullItemID = GetFullPathItemIdList(hItem);
	if(NULL != pFullItemID)
	{
		TCHAR szPath[MAX_PATH] = {0};
		SHGetPathFromIDList(pFullItemID, szPath);
		szFolderPath = szPath;
		ILFree(pFullItemID);
	
		TPDirTreeItemData *pItemData = (TPDirTreeItemData *)GetItemData(hItem);
		if(pItemData)
		{
			if ((pItemData->ulAttrs & SFGAO_LINK) != SFGAO_LINK) // 如果不是Link(快捷方式)
				return szFolderPath;
			else
				return GetLinkPath(szFolderPath);
		}
	}
	return szFolderPath;	
}

BOOL CTPDirTreeCtrl::IsExceptItemID(LPITEMIDLIST pItemID)
{
	for(INT i = 0; i < m_aExceptItemID.GetSize(); i++)
	{
		LPITEMIDLIST pItemIDTemp = (LPITEMIDLIST)m_aExceptItemID[i];
		if(ILIsEqual(pItemIDTemp,pItemID))
			return TRUE;	
	}
	return FALSE;
}
void CTPDirTreeCtrl::InitExceptItemID()
{
	LPITEMIDLIST  pItemIDNetWork  = NULL;
	LPCITEMIDLIST pItemIDRelative = NULL;
	LPSHELLFOLDER pShellNetWork   = NULL;
	HRESULT hResult = S_FALSE;

	DWORD dwExceptItemID[] = { CSIDL_BITBUCKET , CSIDL_CONTROLS };
	for(DWORD i=0; i<sizeof(dwExceptItemID)/sizeof(DWORD); i++)
	{
		LPITEMIDLIST pItemIDTemp = NULL;
		hResult = SHGetSpecialFolderLocation(NULL,dwExceptItemID[i],&pItemIDTemp);
		if(SUCCEEDED(hResult))
			m_aExceptItemID.Add((LPVOID)pItemIDTemp);
	}
	hResult = SHGetSpecialFolderLocation(NULL,CSIDL_NETWORK,&pItemIDNetWork);
	if(SUCCEEDED(hResult))
	{
		hResult = SHBindToParent(pItemIDNetWork,IID_IShellFolder,(LPVOID*)&pShellNetWork,&pItemIDRelative);
		if(NULL == pShellNetWork)	return ;
		if(SUCCEEDED(hResult))
		{
			LPSHELLFOLDER pShellEntireNet = NULL;
			hResult = pShellNetWork->BindToObject(pItemIDRelative,0,IID_IShellFolder,(LPVOID*)&pShellEntireNet);
			if(SUCCEEDED(hResult))
			{
				LPITEMIDLIST pItemIDEntireNet = NULL;
				TCHAR szEntireNetName[] = _T("EntireNetwork");
				pShellEntireNet->ParseDisplayName(NULL,0,szEntireNetName,0,&pItemIDEntireNet,0);
				LPITEMIDLIST pItemFull = ILCombine(pItemIDNetWork,pItemIDEntireNet);
				m_aExceptItemID.Add((LPVOID)pItemFull);
				
				ILFree(pItemIDEntireNet);
				pItemIDEntireNet = NULL;
			}
			if(pShellEntireNet)	 pShellEntireNet->Release();
		}
		ILFree(pItemIDNetWork);
	}
	if(pShellNetWork)	pShellNetWork->Release();
}
void CTPDirTreeCtrl::ReleaseExceptItemID()
{
	for(INT i = 0; i < m_aExceptItemID.GetSize(); i++)
	{
		LPITEMIDLIST pItemID = (LPITEMIDLIST)m_aExceptItemID[i];
		if(NULL != pItemID)
		{
			ILFree(pItemID);
		}
	}
	m_aExceptItemID.RemoveAll();
}
void CTPDirTreeCtrl::SetDesktopFolder()
{
	DeleteAllItems();
	LPITEMIDLIST pidlItem = NULL;  // The item's PIDL.
	if(NULL == m_pShellDestop)	SHGetDesktopFolder(&m_pShellDestop);
	if(NULL == m_pShellDestop) return;
	HRESULT hres = SHGetSpecialFolderLocation(NULL,	CSIDL_DESKTOP,	&pidlItem);
	if (SUCCEEDED(hres))
	{
		HTREEITEM hItem = InsertTreeItem(NULL, m_pShellDestop, NULL,pidlItem);
		TPDirTreeItemData *pItemData = (TPDirTreeItemData*)GetItemData(hItem);
		pItemData->bDestop = TRUE;
		SetDirTree(hItem);
		Expand(hItem,TVE_EXPAND);
	}
	if(pidlItem)
	{
		ILFree(pidlItem); 
		pidlItem = NULL;
	}
}
LRESULT CTPDirTreeCtrl::OnMsgUnSubClassWindow(WPARAM wp,LPARAM lp)
{
	CTPTreeCtrl::DeleteAllItems();
	return S_OK;
}

ULONG CTPDirTreeCtrl::RegisterFileChangeNoftiy()
{
	ULONG uNotifyID = 0;
	HWND hWnd = GetSafeHwnd();
	LPITEMIDLIST ppidl;
	if(SHGetSpecialFolderLocation(hWnd, CSIDL_DESKTOP, &ppidl) == NOERROR)
	{
		SHChangeNotifyEntry shCNE;
		shCNE.pidl = ppidl;
		shCNE.fRecursive = TRUE;
		uNotifyID = SHChangeNotifyRegister(hWnd,SHCNRF_InterruptLevel|SHCNRF_ShellLevel,/*SHCNE_MKDIR|SHCNE_RMDIR|SHCNE_RENAMEFOLDER|SHCNE_UPDATEDIR*/0x7fffffff,WM_FILECHANGE_NOTIFY,1,&shCNE);  
		ASSERT(uNotifyID != 0);    // Shell notification failed
		if(ppidl) {ILFree(ppidl); ppidl = NULL;}
	}
	return uNotifyID;
}
void CTPDirTreeCtrl::UnRegisterFileChangeNoftiy(ULONG uNotifyID)
{
	if(uNotifyID != 0) 
	{
		SHChangeNotifyDeregister(uNotifyID);
		uNotifyID = 0;
	}
}
void CTPDirTreeCtrl::SetDirTree(HTREEITEM hItem)
{
	if(NULL == hItem) return;
	TPDirTreeItemData *pItemData = (TPDirTreeItemData *)GetItemData(hItem);
	if(NULL == pItemData) return;
	if(pItemData->bDestop)
	{
		SetDirTree(m_pShellDestop, NULL, hItem);
	}
	else
	{
		LPITEMIDLIST pFullItemID = GetFullPathItemIdList(hItem);
		LPSHELLFOLDER pShellFolder = NULL;
		HRESULT hRet = m_pShellDestop->BindToObject(pFullItemID, NULL, IID_IShellFolder, (LPVOID*)&pShellFolder);
		if(SUCCEEDED(hRet))
		{
			SetDirTree(pShellFolder, pFullItemID, hItem);
		}
		if(pShellFolder) 	pShellFolder->Release();
		ILFree(pFullItemID);
	}	
	pItemData->bSetDataNew = TRUE;
}
void CTPDirTreeCtrl::SetDirTree(IShellFolder* pParentFolder, LPITEMIDLIST pParentItem, HTREEITEM hParentItem)
{	
	LPENUMIDLIST   pItemEnumList  = NULL;
	SHCONTF        eEnumFlag      = SHCONTF_FOLDERS;

	m_uState |= STATE_SETDATA;
	HTREEITEM hChildItem = NULL;
	while(hChildItem = GetChildItem(hParentItem))  DeleteItem(hChildItem);

	HWND hWnd = NULL;/*m_hWnd*/;
	HRESULT hRet = pParentFolder->EnumObjects(hWnd, eEnumFlag, &pItemEnumList);
	if(!(SUCCEEDED(hRet))) goto Done;

	LPITEMIDLIST pItemList = NULL;
	ULONG        ulwork    = 0;
	if(NULL == pItemEnumList) goto Done;
	while(pItemEnumList->Next(1, &pItemList, &ulwork)==S_OK)
	{
		InsertTreeItem(hParentItem, pParentFolder, pParentItem, pItemList);
		ILFree(pItemList);
	}
Done:
	if(pItemEnumList) pItemEnumList->Release();
	m_uState &= ~STATE_SETDATA;
	SortChildren(hParentItem);
}
HTREEITEM CTPDirTreeCtrl::InsertTreeItem(HTREEITEM hParent, LPSHELLFOLDER pParentFolder, LPCITEMIDLIST pParentItemID, LPITEMIDLIST pChildItem)
{
	ULONG ulAttrs = SFGAO_FOLDER | SFGAO_LINK|SFGAO_COMPRESSED|SFGAO_BROWSABLE;
	pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pChildItem, &ulAttrs);
    BOOL bCanInsert = (ulAttrs & (m_uSfGao|SFGAO_LINK)) ? TRUE : FALSE;

	if(ulAttrs & (SFGAO_COMPRESSED|SFGAO_BROWSABLE)) 
		bCanInsert = FALSE;
	if(!bCanInsert) return NULL;

	LPITEMIDLIST pItemFullID = NULL;
	pItemFullID = ILCombine(pParentItemID, pChildItem);
	if(IsExceptItemID(pItemFullID))
	{
		ILFree(pItemFullID);
		pItemFullID = NULL;
		return NULL;
	}
	TVINSERTSTRUCT tvInsert;
	ZeroMemory(&tvInsert,sizeof(TVINSERTSTRUCT));
	tvInsert.hParent        = hParent;
	tvInsert.item.mask      = TVIF_TEXT|TVIF_IMAGE|TVIF_PARAM|TVIF_STATE|TVIF_SELECTEDIMAGE;
	tvInsert.item.lParam    = NULL;

	TPDirTreeItemData * pItemData = new TPDirTreeItemData;
	pItemData->pItemList = ILClone(pChildItem);
	pItemData->ulAttrs   = ulAttrs;
	STRRET str;
	TCHAR szPathTemp[MAX_PATH] = {0};
	if (NOERROR==pParentFolder->GetDisplayNameOf(pChildItem,SHGDN_NORMAL, &str))
		StrRetToBuf(&str,pItemData->pItemList,szPathTemp,MAX_PATH);

	tvInsert.item.pszText    = szPathTemp;
	tvInsert.item.cchTextMax = lstrlen(szPathTemp);		

	SHFILEINFO    sfi;
	SHGetFileInfo((LPCTSTR)pItemFullID ,0,&sfi,sizeof(SHFILEINFO),SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON);					
	if(sfi.hIcon) DestroyIcon(sfi.hIcon);
	tvInsert.item.iImage = sfi.iIcon;
	SHGetFileInfo((LPCTSTR)pItemFullID ,0,&sfi,sizeof(SHFILEINFO), SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON);					  
	if(sfi.hIcon) DestroyIcon(sfi.hIcon);

	ILFree(pItemFullID);
	tvInsert.item.iSelectedImage = sfi.iIcon;					
	tvInsert.item.lParam  = (LPARAM)pItemData;	

	HTREEITEM hTreeItem = CTreeCtrl::InsertItem(&tvInsert);	
	//if(ulAttrs & SFGAO_HASSUBFOLDER)   InsertItem(_T(""), hTreeItem);
	InsertItem(_T(""), hTreeItem);
	return hTreeItem;
}
LPITEMIDLIST CTPDirTreeCtrl::GetFullPathItemIdList(HTREEITEM hItem)
{
	// 需要外面释放ItemIDList
	if(NULL == hItem) return NULL;
	LPITEMIDLIST pItemIdList = NULL;
	TPDirTreeItemData *pItemData = NULL;
	HTREEITEM hParentItem = GetParentItem(hItem);

	pItemData = (TPDirTreeItemData*)GetItemData(hItem);
	if(NULL == pItemData) return NULL;

	if(NULL == hParentItem) // 已经到达顶端
	{
		pItemIdList = ILClone(pItemData->pItemList);
		return pItemIdList;
	}
	LPITEMIDLIST pItemListNew = GetFullPathItemIdList(GetParentItem(hItem));
	if(NULL != pItemListNew)
	{
		pItemIdList = ILCombine(pItemListNew, pItemData->pItemList);
		FreeItemIDList(pItemListNew);
		pItemListNew = NULL;
	}
	return pItemIdList;
}

void CTPDirTreeCtrl::FreeItemIDList(LPITEMIDLIST pItemIDList)
{
	if(NULL != pItemIDList)
	{
		ILFree(pItemIDList);
		pItemIDList = NULL;
	}
}

BOOL CTPDirTreeCtrl::IsParentItem(LPITEMIDLIST pParentItem, LPITEMIDLIST pChildItem, BOOL bID)
{
	if(bID)
	{
		return ILIsParent(pParentItem, pChildItem, FALSE);
	}
	else
	{
		if(ILIsParent(pParentItem, pChildItem, FALSE)) return TRUE;

		TCHAR szParseName[MAX_PATH] = {0};
		CString sParseName = _T("");
		STRRET strRet;
		m_pShellDestop->GetDisplayNameOf(pChildItem, SHGDN_FORPARSING, &strRet);
		StrRetToBuf(&strRet, pChildItem, szParseName, MAX_PATH);
		sParseName = szParseName;

		m_pShellDestop->GetDisplayNameOf(pParentItem, SHGDN_FORPARSING, &strRet);
		StrRetToBuf(&strRet, pParentItem, szParseName, MAX_PATH);
		if((sParseName.Find(szParseName) >= 0))// 查找是否是父目录
		{
			return TRUE;
		}
	}

	return FALSE;
}
BOOL CTPDirTreeCtrl::IsEqualItem(LPITEMIDLIST pItemID1, LPITEMIDLIST pItemID2, BOOL bID)
{
	if(bID)
	{
		return ILIsEqual(pItemID1, pItemID2);
	}
	else
	{
		if(ILIsEqual(pItemID1, pItemID2)) return TRUE;
		TCHAR szParseName[MAX_PATH] = {0};
		CString sParseName = _T("");
		STRRET strRet;
		m_pShellDestop->GetDisplayNameOf(pItemID1, SHGDN_FORPARSING, &strRet);
		StrRetToBuf(&strRet, pItemID1, szParseName, MAX_PATH);
		sParseName = szParseName;

		m_pShellDestop->GetDisplayNameOf(pItemID2, SHGDN_FORPARSING, &strRet);
		StrRetToBuf(&strRet, pItemID2, szParseName, MAX_PATH);
		if((sParseName.CompareNoCase(szParseName) == 0) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

HTREEITEM CTPDirTreeCtrl::FindItemID(HTREEITEM hItem, LPITEMIDLIST hFindItemID,BOOL bID)
{
	if(NULL == hItem) return NULL;
	LPITEMIDLIST pFullID = GetFullPathItemIdList(hItem);

	BOOL bParent = ILIsParent(pFullID, hFindItemID, FALSE);
	BOOL bEqual = FALSE;

	bEqual  = IsEqualItem(pFullID, hFindItemID, bID);
	ILFree(pFullID);
	pFullID = NULL;
	if(bEqual)
	{
		return hItem;	
	}
	if(bParent)
	{
		TPDirTreeItemData* pItemData = (TPDirTreeItemData*)GetItemData(hItem);
		if(!pItemData->bSetDataNew) SetDirTree(hItem);
		return FindItemID(GetChildItem(hItem), hFindItemID);
	}

	return FindItemID(GetNextSiblingItem(hItem), hFindItemID);
}

void CTPDirTreeCtrl::FindItemID(HTREEITEM hItem, LPITEMIDLIST hFindItemID, CHTREEITEMArray& aFoundItem,BOOL bID)
{
	if(NULL == hItem) return;
	LPITEMIDLIST pFullID = GetFullPathItemIdList(hItem);

	BOOL bParent = ILIsParent(pFullID, hFindItemID, FALSE);
	BOOL bEqual = FALSE;

	bEqual  = IsEqualItem(pFullID, hFindItemID, bID);
	ILFree(pFullID);
	pFullID = NULL;
	if(bEqual)
	{
		aFoundItem.Add(hItem);
	}
	if(bParent)
	{
		FindItemID(GetChildItem(hItem), hFindItemID, aFoundItem);
	}
	FindItemID(GetNextSiblingItem(hItem), hFindItemID, aFoundItem);
}

LPSHELLFOLDER CTPDirTreeCtrl::GetItemFolder(HTREEITEM hItem)
{
	LPSHELLFOLDER pShellFolder = NULL;
	if(NULL == hItem) hItem = GetRootItem();
	TPDirTreeItemData* pItemData = (TPDirTreeItemData*)GetItemData(hItem);
	if(NULL == pItemData) return pShellFolder;
	if(pItemData->bDestop) 
	{
		pShellFolder = m_pShellDestop;
		pShellFolder->AddRef();
	}
	else
	{
		LPITEMIDLIST pFullItemID = GetFullPathItemIdList(hItem);
		m_pShellDestop->BindToObject(pFullItemID, NULL, IID_IShellFolder, (LPVOID*)&pShellFolder);
		ILFree(pFullItemID);
	}

	return pShellFolder;
}

void	CTPDirTreeCtrl::UpdateExpandItem(HTREEITEM hItem)
{
	if(NULL == hItem)	return;
	if(GetItemState(hItem,TVIS_EXPANDED) || GetChildItem(hItem) == NULL)
	{
		UpDateItem(hItem);
		HTREEITEM hChildItem = GetChildItem(hItem);
		UpdateExpandItem(hChildItem);
	}

	HTREEITEM hSiblingItem = GetNextSiblingItem(hItem);
	return UpdateExpandItem(hSiblingItem);
}
LRESULT CTPDirTreeCtrl::OnFileChangeNotify(WPARAM wParam, LPARAM lParam)
{
	typedef struct
	{
		DWORD dwItem1;    // dwItem1 contains the previous PIDL or name of the folder. 
		DWORD dwItem2;    // dwItem2 contains the new PIDL or name of the folder. 
	} SHNOTIFYSTRUCT;

	SHNOTIFYSTRUCT *shns = (SHNOTIFYSTRUCT *)wParam;

	LPITEMIDLIST pItemIDList1 = reinterpret_cast<struct _ITEMIDLIST *>((DWORD_PTR)shns->dwItem1);
	LPITEMIDLIST pItemIDList2 = reinterpret_cast<struct _ITEMIDLIST *>((DWORD_PTR)shns->dwItem2);

	if(STATE_SETDATA & m_uState) return S_OK;
	switch(lParam)
	{
	case SHCNE_MKDIR:
		{
			CHTREEITEMArray aFoundItem;
			FindItemID(GetRootItem(), pItemIDList1, aFoundItem);
			if(aFoundItem.GetSize() > 0)	break;		

			m_uState |= STATE_SETDATA;
			UpdateExpandItem(GetRootItem());
			m_uState &=~STATE_SETDATA;
		}
		break;
	case SHCNE_RMDIR:
	case SHCNE_UPDATEDIR:
		{
			CHTREEITEMArray aFoundItem;
			FindItemID(GetRootItem(), pItemIDList1, aFoundItem);
			if(aFoundItem.GetSize() <= 0)	break;		

			m_uState |= STATE_SETDATA;
			UpdateExpandItem(GetRootItem());
			m_uState &=~STATE_SETDATA;	
		}
		break;
	case SHCNE_RENAMEFOLDER:
		{
			CHTREEITEMArray aFoundItem;
			STRRET strRet;
			TCHAR szParseName[MAX_PATH] = {0};
			LPSHELLFOLDER pParentFolder = NULL;
			LPITEMIDLIST  pLastItem = NULL;
			FindItemID(GetRootItem(), pItemIDList1, aFoundItem);
			if(aFoundItem.GetSize() <= 0)	break;		

			LPITEMIDLIST pItemSrcParent = ILClone(pItemIDList1);
			LPITEMIDLIST pItemDesParent = ILClone(pItemIDList2);
			ILRemoveLastID(pItemSrcParent);
			ILRemoveLastID(pItemDesParent);
			BOOL bRename = ILIsEqual(pItemSrcParent, pItemDesParent);
			ILFree(pItemSrcParent);
			ILFree(pItemDesParent);
			pItemSrcParent = NULL;
			pItemDesParent = NULL;
			if(bRename)
			{
				SHBindToParent(pItemIDList2, IID_IShellFolder, (LPVOID*)&pParentFolder, (LPCITEMIDLIST*)&pLastItem);
				if(NULL == pParentFolder) break;
				pParentFolder->GetDisplayNameOf(pLastItem, SHGDN_INFOLDER|SHGDN_FORPARSING, &strRet);
				StrRetToBuf(&strRet, pLastItem, szParseName, MAX_PATH);
				pParentFolder->Release();
				pLastItem = NULL;
				for(INT i = 0; i < aFoundItem.GetSize(); i++)
				{
					LPSHELLFOLDER pShellFolder = GetItemFolder(GetParentItem(aFoundItem[i]));
					if(NULL ==pShellFolder) continue;
					TPDirTreeItemData* pItemData = (TPDirTreeItemData*)GetItemData(aFoundItem[i]);
					LPITEMIDLIST pItemIDListNew = NULL;
					pShellFolder->ParseDisplayName(m_hWnd, NULL, szParseName, NULL,&pItemIDListNew, NULL);
					pItemData->pItemList = ILClone(pItemIDListNew);
					pShellFolder->GetDisplayNameOf(pItemIDListNew, SHGDN_NORMAL, &strRet);
					TCHAR szTemp[MAX_PATH] = {0};
					StrRetToBuf(&strRet, pItemIDListNew, szTemp, MAX_PATH);
					SetItemText(aFoundItem[i], szTemp);
					pShellFolder->Release();
					ILFree(pItemIDListNew);
				}
			}
			else
			{
				m_uState |= STATE_SETDATA;
				UpdateExpandItem(GetRootItem());
				m_uState &=~STATE_SETDATA;
			}
		}
		break;
	}
	return S_OK;
}

void CTPDirTreeCtrl::OnDestroy()
{
	if(DestoryNotify) DestoryNotify();	
	CTPTreeCtrl::DeleteAllItems();
	SubWindow(FALSE);
	CTPTreeCtrl::OnDestroy();
}


void CTPDirTreeCtrl::UpDateItem(HTREEITEM hItem)
{
	// 获取当前hItem的所有子节点
	CHTREEITEMArray aChildItem;
    if(NULL == hItem) return;
    if(ItemHasChildren(hItem))
	{
        HTREEITEM  hChildItem = GetChildItem(hItem);     
        while(hChildItem != NULL)
        {
             aChildItem.Add(hChildItem);
             hChildItem = GetNextItem(hChildItem, TVGN_NEXT); 
        }
    }
	if(aChildItem.GetSize() < 0) 	return;

	// 比较hItem下所有的pidl路径
	LPSHELLFOLDER pShellFolder = NULL;
	pShellFolder = GetItemFolder(hItem);
    if(!pShellFolder) return;

	LPENUMIDLIST pItemEnumList  = NULL;
	HRESULT hRet = pShellFolder->EnumObjects(NULL, SHCONTF_FOLDERS, &pItemEnumList);
	if(!SUCCEEDED(hRet))
	{
		if(pShellFolder)   pShellFolder->Release();
		if(pItemEnumList)  pItemEnumList->Release();
		return;
	}

	LPITEMIDLIST pFullItemID = GetFullPathItemIdList(hItem);
	LPITEMIDLIST pItemList = NULL;
	ULONG        ulwork    = 0;
	CArray<LPITEMIDLIST,LPITEMIDLIST&>  aItemOfTree;
	while(pItemEnumList->Next(1, &pItemList, &ulwork)==S_OK)
	{
		aItemOfTree.Add(pItemList);
	}
	for(int l = (INT)aChildItem.GetSize()-1; l >= 0; l--)
	{
		TPDirTreeItemData *pItemData = (TPDirTreeItemData*)GetItemData(aChildItem[l]);
		if(NULL == pItemData)	continue;
		BOOL bExists = FALSE;
		for(int k = 0; k < aItemOfTree.GetSize(); k++)
		{
			LPITEMIDLIST pItemIDTemp = aItemOfTree[k];
			if(NULL == pItemIDTemp)	continue;
			if(ILIsEqual(pItemData->pItemList,pItemIDTemp))
			{
				aItemOfTree.RemoveAt(k);
				ILFree(pItemIDTemp);
				pItemIDTemp = NULL;

				bExists = TRUE;
				break;
			}
		}
		if(!bExists)
		{
			DeleteItem(aChildItem[l]);
		}
	}
	for(int i = 0; i < aItemOfTree.GetSize(); i++)
	{
		LPITEMIDLIST pItemTemp = aItemOfTree[i];
		if(NULL == pItemTemp)	continue;
		InsertTreeItem(hItem,pShellFolder,pFullItemID,pItemTemp);
		ILFree(pItemTemp);
		pItemTemp = NULL;
	}
	aItemOfTree.RemoveAll();

	if(pShellFolder)   pShellFolder->Release();
	if(pItemEnumList)  pItemEnumList->Release();
	ILFree(pFullItemID);
}
BOOL CTPDirTreeCtrl::DeleteItemData(HTREEITEM hItem)
{
	HTREEITEM   hChildItem = GetChildItem(hItem);     
	while(hChildItem != NULL)
	{		
		TPDirTreeItemData *pItemData = (TPDirTreeItemData*)GetItemData(hChildItem);
		if(pItemData) delete pItemData;
		SetItemData(hChildItem,NULL);
		DeleteItemData(hChildItem);
		hChildItem  = GetNextItem(hChildItem, TVGN_NEXT); 
	}
	if(hItem)
	{	
		TPDirTreeItemData *pItemData = (TPDirTreeItemData*)GetItemData(hItem);
		if(pItemData) delete pItemData;
		SetItemData(hItem,NULL);
	}
	return TRUE;
}
BOOL CTPDirTreeCtrl::DeleteAllItems()
{
	 DeleteItemData(GetRootItem());
	 return  CTPTreeCtrl::DeleteAllItems();
}


