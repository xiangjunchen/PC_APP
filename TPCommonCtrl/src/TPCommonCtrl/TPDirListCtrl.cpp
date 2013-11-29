// TPDirListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPDirListCtrl.h"
#include ".\tpdirlistctrl.h"

#define WM_FILECHANGE_NOTIFY  WM_USER + 2000
#define TP_EVENT_ITEMSELECTCHANGE 1000 //add by qushaojie for onTimer
#define TP_EVENT_UPDATEFOLDER	  1001 // 目录 刷新


#define TP_TYPENAME_LEN				79
typedef struct _tagTPDirListItemData
{
	LPITEMIDLIST               lpi;
	LPITEMIDLIST			   pFullItemID;

	int                        iImage;	
	TCHAR                      sFileName[MAX_PATH];
	TCHAR                      sType[100];
	__int64                    iLength;
	DWORD                      dwFileAttributes; 
	FILETIME                   ftCreationTime;
	FILETIME                   ftLastAccessTime;
	FILETIME                   ftLastWriteTime;
	ULONG                      ulAttrs;	
	BOOL                       bFindFile ; 
	DWORD                      dwSelTick; 
	_tagTPDirListItemData()
	{
		Reset();
	}
	~_tagTPDirListItemData()
	{
		Release();
	}
	void Reset()
	{
		iImage       = -1;		
		lpi          = NULL;
		pFullItemID  = NULL;
		sFileName[0] = _T('\0');
		sType[0]     = _T('\0');

		iLength          = 0;		
		dwFileAttributes = 0;
		ulAttrs          = 0;
		bFindFile        = FALSE;	
		dwSelTick        = 0;
	}
	void Release()
	{
		if(lpi) {ILFree(lpi); lpi = NULL;}
		if(pFullItemID) {ILFree(pFullItemID); pFullItemID = NULL;}
	}
} TPDirListItemData, *LPTPDirListItemData;

BOOL TP_IsValidFileTime(FILETIME & stuFileTime)
{
	if(stuFileTime.dwHighDateTime <= 0 && stuFileTime.dwLowDateTime <= 0) return FALSE;
	return TRUE;
}
IMPLEMENT_DYNAMIC(CTPDirListCtrl, CTPListCtrl)
CTPDirListCtrl::CTPDirListCtrl()
{
	m_ulAttrs           = 0;
	m_eShconTf			= SHCONTF_FOLDERS| SHCONTF_NONFOLDERS;
	m_uSfGao			= SFGAO_FOLDER | SFGAO_FILESYSTEM;
	m_bParentPath		= TRUE;
	m_lpifqThis			= NULL;
	m_pShellFolder		= NULL;
	m_bDescSort			= FALSE;
	m_iSortIndex		= 0;
	m_iSelTick			= 0;
	m_uState			= 0;
	m_uNoftifyChangeID	= 0;
	m_bSelectSort       = FALSE;
	m_aFilter.Add(_L(".AVI"));

	InitExceptItemID();
}

CTPDirListCtrl::~CTPDirListCtrl()
{	
	if(m_lpifqThis)
	{
		ILFree(m_lpifqThis);
		m_lpifqThis = NULL;
	}
	m_aSelectItem.RemoveAll();
	ReleaseExceptItemID();
}


BEGIN_MESSAGE_MAP(CTPDirListCtrl, CTPListCtrl)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)

	ON_MESSAGE(WM_UNSUBCLASSWINDOW,OnMsgUnSubClassWindow)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHdnItemclick)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHdnItemclick)
	ON_WM_TIMER()
	//    ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_MESSAGE(WM_FILECHANGE_NOTIFY,OnFileChangeNotify)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void     CTPDirListCtrl::RegisterClass(BOOL bRegister)
{	
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_LISTVIEW,&wndClass);		
		wndClass.lpszClassName = WC_DIRLISTCTRL;			
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_DIRLISTCTRL,AfxGetInstanceHandle());
}
void   CTPDirListCtrl::SubWindow(BOOL bSub)
{	
	BOOL bSet = m_bSubWindow;
	CTPListCtrl::SubWindow(bSub);
	if(bSub)
	{
		if(bSet) return;
		m_uStyle  = GetStyle();
		if((m_uStyle &0x00000003) != LVS_REPORT)
			CTPListCtrl::ModifyStyle(m_uStyle&0x00000003,LVS_ICON|LVS_SHOWSELALWAYS|WS_BORDER,TRUE);
		m_uStyle  = GetStyle();
		SetImageList(TRUE);
		int iIndex[] = {HEAD_NAME,HEAD_TYPE,HEAD_LENGTH,HEAD_WRITETIME,0,0,0,0};
		SetHeadType(iIndex);	
		RegisterFileChangeNoftiy();
	}
	else
	{
		if(!bSet) return;
		SetImageList(FALSE);
		UnRegisterFileChangeNoftiy();
	}
}

void CTPDirListCtrl::SetImageList(BOOL bSet)
{	
	if(bSet)
	{			
		if(CTPDirTreeCtrl::m_hImagelist[0]!=NULL) 	
		{
			CListCtrl::SetImageList(CImageList::FromHandle(CTPDirTreeCtrl::m_hImagelist[0]),LVSIL_NORMAL);
			CListCtrl::SetImageList(CImageList::FromHandle(CTPDirTreeCtrl::m_hImagelist[1]),LVSIL_SMALL);		
			//CListCtrl::SetImageList(CImageList::FromHandle(CTPDirTreeCtrl::m_hImagelist[1]),LVSIL_STATE);					
		}
	}
	else
	{
		if(CTPDirTreeCtrl::m_hImagelist[0])
		{
			CListCtrl::SetImageList(NULL,LVSIL_NORMAL);
			CListCtrl::SetImageList(NULL,LVSIL_SMALL);
			//CListCtrl::SetImageList(NULL,LVSIL_STATE);				
		}
	}
}
void CTPDirListCtrl::SetSHCONTF(SHCONTF uSet)
{
	m_eShconTf  = uSet;
}
void CTPDirListCtrl::SetSFGO(ULONG uSet)
{
	m_uSfGao    = uSet;
}

void CTPDirListCtrl::RegisterFileChangeNoftiy(LPITEMIDLIST pItemID)
{
	HWND hWnd = GetSafeHwnd();
	if(NULL != pItemID)
	{
		SHChangeNotifyEntry shCNE;
		shCNE.pidl = pItemID;
		shCNE.fRecursive = TRUE;
		m_uNoftifyChangeID = SHChangeNotifyRegister(hWnd,SHCNRF_InterruptLevel|SHCNRF_ShellLevel,SHCNE_ALLEVENTS,WM_FILECHANGE_NOTIFY,1,&shCNE);  
		ASSERT(m_uNoftifyChangeID != 0);    // Shell notification failed
	}
	else
	{
		LPITEMIDLIST ppidl;
		if(SHGetSpecialFolderLocation(hWnd, CSIDL_DESKTOP, &ppidl) == NOERROR)
		{
			SHChangeNotifyEntry shCNE;
			shCNE.pidl = ppidl;
			shCNE.fRecursive = TRUE;
			m_uNoftifyChangeID = SHChangeNotifyRegister(hWnd,SHCNRF_InterruptLevel|SHCNRF_ShellLevel,SHCNE_ALLEVENTS,WM_FILECHANGE_NOTIFY,1,&shCNE);  
			ASSERT(m_uNoftifyChangeID != 0);    // Shell notification failed
			if(ppidl) {ILFree(ppidl); ppidl = NULL;}
		}
	}
}
void CTPDirListCtrl::UnRegisterFileChangeNoftiy()
{
	if(m_uNoftifyChangeID != 0) 
	{
		SHChangeNotifyDeregister(m_uNoftifyChangeID);
		m_uNoftifyChangeID = 0;
	}
}

BOOL CTPDirListCtrl::UpdateListItem(INT nIndex, LPSHELLFOLDER pShellFolder, LPITEMIDLIST pParentID, LPITEMIDLIST pChildID)
{
	if(nIndex < 0) return FALSE;
	TPDirListItemData *pItemData =(TPDirListItemData*)GetItemData(nIndex);
	if(NULL == pItemData) return FALSE;

	TCHAR szBuff[MAX_PATH] = {0};
	LPTSTR lpDisPlayName   = NULL;

	LPITEMIDLIST pFullItemID = ILCombine(pParentID, pChildID);

	if(SHGetPathFromIDList(pFullItemID,szBuff)) // 转换路径
		lpDisPlayName = PathFindFileName(szBuff);

	SHFILEINFO    sfi;
	ZeroMemory(&sfi, sizeof(sfi));//获取文件信息
	SHGetFileInfo((LPCTSTR)pFullItemID,0,&sfi,sizeof(SHFILEINFO),SHGFI_PIDL|SHGFI_TYPENAME|SHGFI_DISPLAYNAME);

	lstrcpyn(pItemData->sType ,sfi.szTypeName, TP_TYPENAME_LEN);
	if(lpDisPlayName)	lstrcpyn(pItemData->sFileName, lpDisPlayName,MAX_PATH);
	else				lstrcpyn(pItemData ->sFileName,sfi.szDisplayName, MAX_PATH);					

	ILFree(pFullItemID);
	pFullItemID = NULL;

	WIN32_FIND_DATA findFileData;// 获取文件大小，创建时间
	if(S_OK == SHGetDataFromIDList(pShellFolder, pChildID, SHGDFIL_FINDDATA, &findFileData, sizeof(findFileData)))
	{	
		pItemData ->iLength           = findFileData.nFileSizeHigh;
		pItemData ->iLength         <<= 32;
		pItemData ->iLength          += findFileData.nFileSizeLow;						
		pItemData ->dwFileAttributes  = findFileData.dwFileAttributes;
		pItemData ->ftCreationTime    = findFileData.ftCreationTime;
		pItemData ->ftLastAccessTime  = findFileData.ftLastAccessTime;
		pItemData ->ftLastWriteTime   = findFileData.ftLastWriteTime;
		pItemData ->bFindFile         = TRUE;
	}

	return TRUE;
}
BOOL CTPDirListCtrl::UpdateFolder(LPSHELLFOLDER pShellFolder, LPITEMIDLIST pParentID)
{
	if(NULL == pShellFolder) return FALSE;
	if(m_uState & TP_DIRLIST_SETDATA) return FALSE;
	m_uState |= TP_DIRLIST_SETDATA;
	ULONG                       ulwork     = 0;
	HRESULT                     hResert    = S_FALSE;
	HWND                        hwnd       = NULL;
	LPENUMIDLIST                lpwork     = NULL;
	LPITEMIDLIST                lpi        = NULL;
	CArray<LPITEMIDLIST,LPITEMIDLIST&> aItemOfFolder;

	CWaitCursor stuWait;

	//开始列举数据
	hResert=pShellFolder->EnumObjects(hwnd, m_eShconTf, &lpwork);
	if (SUCCEEDED(hResert))
	{
		while(lpwork->Next(1, &lpi, &ulwork)==S_OK)
		{
			aItemOfFolder.Add(lpi);
		}
	}	
	if (lpwork) lpwork->Release();

	for(int l = GetItemCount()-1; l >= 0; l--)
	{
		LPTPDirListItemData pItemData = (LPTPDirListItemData)CTPListCtrl::GetItemData(l);
		if(NULL == pItemData)	continue;
		BOOL bExists = FALSE;
		for(int k=0; k < aItemOfFolder.GetSize(); k++)
		{
			LPITEMIDLIST pItemIDTemp = aItemOfFolder[k];
			if(NULL == pItemIDTemp) continue;
			if(ILIsEqual(pItemData->lpi,pItemIDTemp))
			{
				//UpdateListItem(l,pShellFolder,pParentID,pItemIDTemp);

				aItemOfFolder.RemoveAt(k);
				ILFree(pItemIDTemp);
				pItemIDTemp = NULL;

				bExists = TRUE;
				break;
			}
		}
		if(!bExists)
		{
			DeleteItem(l);
		}
	}
	for(int i = 0; i < aItemOfFolder.GetSize(); i++)
	{
		LPITEMIDLIST pItemTemp = aItemOfFolder[i];
		if(NULL == pItemTemp) continue;
		InsertListItem(GetItemCount(),pShellFolder,pParentID,pItemTemp);
		ILFree(pItemTemp);
		pItemTemp = NULL;
	}
	aItemOfFolder.RemoveAll();

	m_uState &= ~TP_DIRLIST_SETDATA;
	return TRUE;
}
INT	CTPDirListCtrl::InsertListItem(INT nIndex, LPSHELLFOLDER pShellFolder, LPITEMIDLIST pParentID, LPITEMIDLIST pChildID)
{
	ULONG ulAttrs = m_uSfGao|SFGAO_BROWSABLE|SFGAO_LINK;
	ULONG ulAttrsShow = m_uSfGao;
	TCHAR szBuff[MAX_PATH] = {0};
	LPTSTR lpDisPlayName   = NULL;
	pShellFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pChildID, &ulAttrs);
	if (!(ulAttrs & (m_uSfGao|SFGAO_LINK))) return -1;

	LPITEMIDLIST pFullItemID = ILCombine(pParentID, pChildID);
	if(IsExceptItemID(pFullItemID))
	{
		ILFree(pFullItemID);
		pFullItemID = NULL;
		return NULL;
	}
	if(SHGetPathFromIDList(pFullItemID,szBuff))
		lpDisPlayName = PathFindFileName(szBuff);

	BOOL bNormalFile = !(ulAttrs & SFGAO_FOLDER) || (ulAttrs & SFGAO_BROWSABLE) && !(ulAttrsShow & SFGAO_BROWSABLE);
	if(bNormalFile && (ulAttrs & SFGAO_LINK))// 检查目录快捷方式
	{
		if(IsFolderItemID(pFullItemID)) bNormalFile = FALSE;
	}
	if(bNormalFile)
	{						
		ulAttrs &= ~SFGAO_FOLDER;
		INT_PTR l = 0;
		CString sName = szBuff; 
		sName.MakeUpper();
		if(sName.Find(_T(".lnk"))>0)
		{
			ILFree(pFullItemID);
			return -1;
		}
		for(l=0;l<m_aFilter.GetSize();l++)
		{
			CString sFilter = m_aFilter[l];
			if(sFilter == _L("*.*")) break;			
			sFilter.Replace('*',' ');
			sFilter.Trim();
			LPTSTR lpExt = PathFindExtension(sName);
			if(NULL != lpExt && sFilter.CompareNoCase(lpExt) == 0) break;
		}
		if(l>=m_aFilter.GetSize())
		{				
			ILFree(pFullItemID);
			return -1;
		}
	}

	SHFILEINFO    sfi;
	ZeroMemory(&sfi, sizeof(sfi));
	TPDirListItemData *pItemData = new TPDirListItemData;
	pItemData->lpi = ILClone(pChildID);

	SHGetFileInfo((LPCTSTR)pFullItemID,0,&sfi,sizeof(SHFILEINFO),SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_TYPENAME|SHGFI_DISPLAYNAME);
	ILFree(pFullItemID);

	if(sfi.hIcon) DestroyIcon(sfi.hIcon);
	pItemData ->iImage = sfi.iIcon;
	lstrcpyn(pItemData->sType ,sfi.szTypeName,TP_TYPENAME_LEN);

	if(!bNormalFile) lpDisPlayName = NULL;

	if(lpDisPlayName)
		lstrcpyn(pItemData->sFileName, lpDisPlayName,MAX_PATH);
	else
		lstrcpyn(pItemData ->sFileName,sfi.szDisplayName,MAX_PATH);					
	pItemData->ulAttrs = ulAttrs;

	WIN32_FIND_DATA findFileData;
	if(S_OK == SHGetDataFromIDList(pShellFolder, pChildID, SHGDFIL_FINDDATA, &findFileData, sizeof(findFileData)))
	{
		pItemData ->iLength           = findFileData.nFileSizeHigh;
		pItemData ->iLength         <<= 32;
		pItemData ->iLength          += findFileData.nFileSizeLow;						
		pItemData ->dwFileAttributes  = findFileData.dwFileAttributes;
		pItemData ->ftCreationTime    = findFileData.ftCreationTime;
		pItemData ->ftLastAccessTime  = findFileData.ftLastAccessTime;
		pItemData ->ftLastWriteTime   = findFileData.ftLastWriteTime;
		pItemData ->bFindFile         = TRUE;
	}

	int nRetIndex =CTPListCtrl::InsertItem(nIndex,pItemData->sFileName,pItemData ->iImage);					
	CTPListCtrl::SetItemData(nRetIndex,(DWORD_PTR)pItemData);
	CTPDirListCtrl::SetItemText(nRetIndex);

	return nRetIndex;
}
void CTPDirListCtrl::SetDirList(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpifq)
{
	if(m_uState & TP_DIRLIST_SETDATA) return;
	m_uState |= TP_DIRLIST_SETDATA;
	ULONG                       ulwork     = 0;
	HRESULT                     hResert    = S_FALSE;
	HWND                        hwnd       = NULL;//::GetParent(m_hWnd);
	LPENUMIDLIST                lpwork     = NULL;
	LPITEMIDLIST                lpi = NULL;
	LPITEMIDLIST				pItemIDList =ILClone(lpifq);// 先复制，再释放
	INT nCount = GetItemCount();
	if(m_lpifqThis) {ILFree(m_lpifqThis); m_lpifqThis = NULL;}
	m_lpifqThis = pItemIDList;
	lpsf->AddRef();
	if(m_pShellFolder) {m_pShellFolder->Release(); m_pShellFolder = NULL;}
	m_pShellFolder = lpsf;

	UnRegisterFileChangeNoftiy();
	RegisterFileChangeNoftiy(pItemIDList);


	CWaitCursor stuWait;

	DeleteAllItems();
	if(m_bParentPath && pItemIDList){ CTPListCtrl::InsertItem(0,_L(".."),4);}
	//开始列举数据
	if (SUCCEEDED(hResert))
	{
		SetRedraw(FALSE);
		hResert=lpsf->EnumObjects(hwnd, m_eShconTf, &lpwork);
		if (SUCCEEDED(hResert))
		{
			while(lpwork->Next(1, &lpi, &ulwork)==S_OK)
			{
				InsertListItem(GetItemCount(), lpsf, m_lpifqThis, lpi);

				ILFree(lpi);
			}
		}	
		SetRedraw(TRUE);
	}
	if (lpwork) lpwork->Release();

	m_uState &= ~TP_DIRLIST_SETDATA;
}

void CTPDirListCtrl::SetDesktopFolder()
{
	LPSHELLFOLDER pShellFolder =NULL;
	LPITEMIDLIST  pDestopID=NULL;
	HRESULT hr = SHGetDesktopFolder(&pShellFolder);	
	SHGetSpecialFolderLocation(NULL,	CSIDL_DESKTOP,	&pDestopID);

	if (pShellFolder && pDestopID)
	{
		SetDirList(pShellFolder,pDestopID);
	}	
	if(pShellFolder) {pShellFolder->Release(); pShellFolder = NULL;}
	if(pDestopID) {ILFree(pDestopID); pDestopID = NULL;}
}
void  CTPDirListCtrl::SetSpecialFolder(int csidl)
{
}

void CTPDirListCtrl::RefreshView()
{
	if(m_pShellFolder && m_lpifqThis)
		SetDirList(m_pShellFolder, m_lpifqThis);
}
void CTPDirListCtrl::InitExceptItemID()
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
void CTPDirListCtrl::ReleaseExceptItemID()
{
	for(INT i = 0; i < m_aExceptItemID.GetSize(); i++)
	{
		LPITEMIDLIST pItemIDTemp = (LPITEMIDLIST)m_aExceptItemID[i];
		if(NULL != pItemIDTemp)
		{
			ILFree(pItemIDTemp);
		}
	}
	m_aExceptItemID.RemoveAll();	
}
BOOL CTPDirListCtrl::IsExceptItemID(LPITEMIDLIST pItemID)
{
	for(INT i=0; i<m_aExceptItemID.GetSize(); i++)
	{
		LPITEMIDLIST pItemIDTemp = (LPITEMIDLIST)m_aExceptItemID[i];
		if(ILIsEqual(pItemIDTemp,pItemID))
			return TRUE;
	}
	return FALSE;
}
HRESULT CTPDirListCtrl::SetPathName(CString sPathName)
{
	DWORD        dwAttribates  = SFGAO_FILESYSTEM;
	LPITEMIDLIST pItemIDList   = NULL;
	HRESULT      hResult       = S_FALSE;
	LPMALLOC     lpMalloc      = NULL;
	if(FAILED(hResult))       return NULL;
	hResult = SHILCreateFromPath(sPathName,&pItemIDList,&dwAttribates);
	if (SUCCEEDED(hResult))
	{
		hResult = SetItemIDList(pItemIDList);	
		if(pItemIDList){ILFree(pItemIDList); pItemIDList = NULL;}		
	}		
	return hResult;	
}
LRESULT      CTPDirListCtrl::SetPathIDLIST(LPITEMIDLIST  pItem )
{
	return SetItemIDList(pItem);	;
}
LPITEMIDLIST CTPDirListCtrl::GetPathIDLIST(int iItem)
{
	if(iItem<0) return m_lpifqThis;	
	else
	{

		LPARAM  lParam = CTPListCtrl::GetItemData(iItem);
		if(!TP_IsNull(lParam))
		{
			LPTPDirListItemData pItemData    = (LPTPDirListItemData)lParam;
			if(pItemData->pFullItemID) {ILFree(pItemData->pFullItemID); pItemData->pFullItemID = NULL;}
			pItemData->pFullItemID = ILCombine(m_lpifqThis, pItemData->lpi);
			return pItemData->pFullItemID;
		}
		else return NULL;		
	}
}

void CTPDirListCtrl::GetLinkItem(LPITEMIDLIST pItemID,IPersistFile*& psf, IShellLink*& psl)
{
	TCHAR szLinkPath[MAX_PATH] = {0};
	SHGetPathFromIDList(pItemID, szLinkPath);

	psl = NULL;
	psf = NULL;

	HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,IID_IShellLink, (LPVOID*)& psl);
	if (SUCCEEDED(hres))
	{
		hres = psl->QueryInterface( IID_IPersistFile, (LPVOID *)& psf);
		if(SUCCEEDED(hres))
		{
			/*WORD wsz[MAX_PATH];
			USES_CONVERSION;
			LPCSTR lpcs = W2A(szLinkPath); 
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpcs, -1, wsz,  MAX_PATH);*/
			hres = psf->Load(szLinkPath, 0);
		}
	}
	
	if(!(SUCCEEDED(hres)))
	{
		if(psl){psl->Release();	psl = NULL;}
		if(psf){psf->Release(); psf = NULL;}
	}
}
CString  CTPDirListCtrl::GetLinkPath(LPITEMIDLIST pItemID, LPITEMIDLIST* pItemIDLink)
{
	TCHAR szLinkPath[MAX_PATH] = {0};
	SHGetPathFromIDList(pItemID, szLinkPath);
	CString sLinkPathName = szLinkPath;

	IShellLink* psl = NULL;
	IPersistFile* psf = NULL;
	GetLinkItem(pItemID, psf, psl);
	if(NULL != psl)
	{
		TCHAR szTempPath[MAX_PATH] = {0};
		psl->GetPath(szTempPath, MAX_PATH, NULL, SLGP_UNCPRIORITY);
		sLinkPathName = szTempPath;
		if(NULL != pItemIDLink) psl->GetIDList(pItemIDLink);
	}
	if(psl) psl->Release();
	if(psf) psf->Release();
	psl = NULL;
	psf = NULL;

	return sLinkPathName;
}
CString CTPDirListCtrl::GetPathName(int iItem)
{
	LPITEMIDLIST pItemID = m_lpifqThis;
	if(iItem >= 0)	pItemID = GetPathIDLIST(iItem);

	if(NULL == pItemID) return _T("");

	CString szFolderPath = _L("");
	TCHAR   szBuff[MAX_PATH];	
	if(SHGetPathFromIDList(pItemID,szBuff))	szFolderPath = szBuff;	

	while(TRUE)
	{
		if(szFolderPath.IsEmpty()) break;
		if(szFolderPath.ReverseFind('\\') == (lstrlen(szFolderPath)-1))	
			szFolderPath = szFolderPath.Mid(0,lstrlen(szFolderPath)-1);
		else
			break;
	}

	ULONG ulShowAttrs = 0;
	if(iItem < 0)
	{
		ulShowAttrs = m_ulAttrs;
	}
	else
	{
		TPDirListItemData *pItemData = (TPDirListItemData *)CTPListCtrl::GetItemData(iItem);
		if(pItemData)
			ulShowAttrs = pItemData->ulAttrs;
	}
	if ((ulShowAttrs & SFGAO_LINK) == SFGAO_LINK) // 如果不是Link(快捷方式)
		szFolderPath = GetLinkPath(pItemID);
	if(szFolderPath.GetLength() >= MAX_PATH) szFolderPath = _T("");
	return szFolderPath;
}
HRESULT CTPDirListCtrl::SetItemIDList(LPITEMIDLIST pItemIDList)
{
	LPSHELLFOLDER psfParent    = NULL; 
	LPSHELLFOLDER lpsfTemp     = NULL;	
	LPCITEMIDLIST pidlRelative = NULL; 	
	HRESULT      hResult       = S_FALSE;

	BOOL bSetFolder = FALSE;
	hResult = SHBindToParent(pItemIDList,IID_IShellFolder,(void**) &psfParent,&pidlRelative);
	if(SUCCEEDED(hResult))
	{
		ULONG ulAttrs = SFGAO_LINK |m_uSfGao ;
		psfParent->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlRelative, &ulAttrs);
		m_ulAttrs = ulAttrs;
		hResult = psfParent->BindToObject(pidlRelative,0, IID_IShellFolder,(LPVOID *)&lpsfTemp);
		if (SUCCEEDED(hResult))	
		{			
			bSetFolder = TRUE;
			SetDirList(lpsfTemp,(LPITEMIDLIST)pItemIDList);
		}
	}	

	if(!bSetFolder)	SetDesktopFolder();

	if(lpsfTemp) lpsfTemp->Release();
	if(psfParent) psfParent->Release();

	return S_OK;
}
void CTPDirListCtrl::ClearData()
{
	for(INT l=0;l<GetItemCount();l++)
	{		
		LPTPDirListItemData pItemData   =  (LPTPDirListItemData)GetItemData(l); 
		if(pItemData)
		{
			delete pItemData;
			SetItemData(l,NULL);
		}		
	}
}
BOOL CTPDirListCtrl::DeleteAllItems()
{
	ClearData();
	return CTPListCtrl::DeleteAllItems();
}
void CTPDirListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);	
	*pResult = 0;	

	if(!TP_IsNull(pNMLV ->lParam))
	{
		LPTPDirListItemData pItemData   =  (LPTPDirListItemData)pNMLV ->lParam; 
		delete pItemData;
	}	
}
void CTPDirListCtrl::SetItemText(int iItem)
{
	CString sText;
	int     iOrder = 0;
	LPARAM  lParam = CTPListCtrl::GetItemData(iItem);
	if(!TP_IsNull(lParam))
	{
		LPTPDirListItemData lptvid    = (LPTPDirListItemData)lParam;
		if(m_pHeadCtrl)
		{	
			for(INT l=0;l<HEAD_MAX;l++)
			{
				iOrder = m_pHeadCtrl ->OrderToIndex(l);
				if(m_iHeadIndex[iOrder] == HEAD_NAME)             CTPListCtrl::SetItemText(iItem,iOrder,lptvid ->sFileName);
				else if(m_iHeadIndex[iOrder] == HEAD_TYPE)        CTPListCtrl::SetItemText(iItem,iOrder,lptvid ->sType);
				else if(m_iHeadIndex[iOrder] == HEAD_LENGTH) 
				{					
					if(!(lptvid->ulAttrs &SFGAO_FOLDER) && lptvid ->bFindFile )
					{	
						if(lptvid ->iLength>1024)
						{
							CString sTemp;
							__int64 iTemp = lptvid ->iLength/1024;
							while(iTemp / 1000 > 0)
							{
								sTemp.Format(_L(",%03d"),iTemp%1000);
								sText  = sTemp + sText;
								iTemp /= 1000;
							}
							sTemp.Format(_L("%d"),iTemp);
							sText = sTemp + sText + _L(" k");
						}
						else
						{
							sText.Format(_L("%d b"),lptvid ->iLength); 
						}
						CTPListCtrl::SetItemText(iItem,iOrder,sText);
					}
				}
				else if(m_iHeadIndex[iOrder] == HEAD_CREATETIME) 
				{
					if(lptvid ->bFindFile && TP_IsValidFileTime(lptvid->ftCreationTime))
					{		
						CTime cTime(lptvid->ftCreationTime);
						SYSTEMTIME sysCreate;
						cTime.GetAsSystemTime(sysCreate);
						sText = TP_GetTimeString(sysCreate);    
						CTPListCtrl::SetItemText(iItem,iOrder,sText);					
					}
				}
				else if(m_iHeadIndex[iOrder] == HEAD_WRITETIME) 
				{
					if(lptvid ->bFindFile && TP_IsValidFileTime(lptvid->ftLastWriteTime))
					{			
						CTime cTime(lptvid->ftLastWriteTime);
						SYSTEMTIME sysModify;
						cTime.GetAsSystemTime(sysModify);
						sText = TP_GetTimeString(sysModify);
						CTPListCtrl::SetItemText(iItem,iOrder,sText);					
					}
				}
				else if(m_iHeadIndex[iOrder] == HEAD_ACCESSTIME)
				{
					if(lptvid ->bFindFile && TP_IsValidFileTime(lptvid->ftLastAccessTime))
					{				
						CTime cTime(lptvid->ftLastAccessTime);
						SYSTEMTIME sysAccess;
						cTime.GetAsSystemTime(sysAccess);
						sText = TP_GetTimeString(sysAccess);    
						CTPListCtrl::SetItemText(iItem,iOrder,sText);					
					}
				}
				else if(m_iHeadIndex[iOrder] == HEAD_ATTRIBUTE)
				{
					if(lptvid ->bFindFile)
					{					
						sText = _L("");
						if(lptvid->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)    sText+= _L("H");
						else sText+= _L(" ");
						if(lptvid->dwFileAttributes & FILE_ATTRIBUTE_READONLY)  sText+= _L("R");
						else sText+= _L(" ");
						if(lptvid->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)    sText+= _L("S");
						else sText+= _L(" ");
						if(lptvid->dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) sText+= _L("T");
						else sText+= _L(" ");
						if(lptvid->dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)   sText+= _L("O");
						else sText+= _L(" ");
						CTPListCtrl::SetItemText(iItem,iOrder,sText);					
					}
				}
			}
		}
		else
		{
			CTPListCtrl::SetItemText(iItem,0,lptvid ->sFileName);
		}
	}
}
void CTPDirListCtrl::SetHeadType(int iIndex[HEAD_MAX])
{
	CString sText;
	CRect   rtCleint;
	UINT    uFormat;
	GetClientRect(&rtCleint);
	rtCleint.DeflateRect(9,2);
	memcpy_s(m_iHeadIndex,sizeof(int)*HEAD_MAX, iIndex,sizeof(int)*HEAD_MAX);
	CTPListCtrl::DeleteAllItems();
	while (CTPListCtrl::DeleteColumn(0));

	for(INT l=0;l<HEAD_MAX;l++)
	{
		if(iIndex[l] == HEAD_NAME)             { sText = TP_LoadStr(_L("IDS_COMMONCTRL_NAME"));     uFormat = LVCFMT_LEFT ;}
		else if(iIndex[l] == HEAD_TYPE)        { sText = TP_LoadStr(_L("IDS_COMMONCTRL_TYPE"));     uFormat = LVCFMT_LEFT ;}
		else if(iIndex[l] == HEAD_LENGTH)      { sText = TP_LoadStr(_L("IDS_COMMONCTRL_SIZE"));     uFormat = LVCFMT_RIGHT ;}
		else if(iIndex[l] == HEAD_CREATETIME)  { sText = TP_LoadStr(_L("IDS_COMMONCTRL_CREATETIME")); uFormat = LVCFMT_RIGHT ;}
		else if(iIndex[l] == HEAD_WRITETIME)   { sText = TP_LoadStr(_L("IDS_COMMONCTRL_MODIFYTIME")); uFormat = LVCFMT_RIGHT ;}
		else if(iIndex[l] == HEAD_ACCESSTIME)  { sText = TP_LoadStr(_L("IDS_COMMONCTRL_VIEWTIME")); uFormat = LVCFMT_RIGHT ;}
		else if(iIndex[l] == HEAD_ATTRIBUTE)   { sText = TP_LoadStr(_L("IDS_COMMONCTRL_PROPERTY"));     uFormat = LVCFMT_LEFT ;}
		else return;
		CTPListCtrl::InsertColumn(l,sText,uFormat,rtCleint.Width()/4);
	}
}
LRESULT CTPDirListCtrl::OnMsgUnSubClassWindow(WPARAM wp,LPARAM lp)
{
	CTPListCtrl::DeleteAllItems();
	return S_OK;
}
void CTPDirListCtrl::OnDestroy()
{
	KillTimer(TP_EVENT_ITEMSELECTCHANGE);
	KillTimer(TP_EVENT_UPDATEFOLDER);
	if(DestoryNotify) DestoryNotify();	
	CTPListCtrl::DeleteAllItems();
	SubWindow(FALSE);
	CTPListCtrl::OnDestroy();
}
LPITEMIDLIST GetNextItemID(LPCITEMIDLIST pidl) 
{ 
	if(pidl == NULL)	return NULL;	
	int cb = pidl->mkid.cb; 
	if (cb == 0)    	return NULL; 
	pidl = (LPITEMIDLIST) (((LPBYTE) pidl) + cb); 
	return (pidl->mkid.cb == 0) ? NULL : (LPITEMIDLIST) pidl; 
} 
BOOL GetParentID(LPITEMIDLIST &pidl)
{
	BOOL fRemoved = FALSE;
	if (pidl == NULL)	return(FALSE);
	if (pidl->mkid.cb)
	{
		LPITEMIDLIST pidlNext = pidl;
		while (pidlNext)
		{
			pidl = pidlNext;
			pidlNext = GetNextItemID(pidl);
		}
		pidl->mkid.cb = 0; 
		fRemoved = TRUE;
	}

	return fRemoved;
}

HRESULT CTPDirListCtrl::SetParentItem()
{
	if(NULL != m_lpifqThis)
	{
		LPITEMIDLIST  pItemID    = ILClone(m_lpifqThis); 
		ILRemoveLastID(pItemID);
		SetItemIDList(pItemID);
		ILFree(pItemID);
	}

	return S_OK;
}
void CTPDirListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CTPListCtrl::OnLButtonDblClk(nFlags, point);
	int iItem = CTPListCtrl::HitTest(point,&nFlags);
	KillTimer(TP_EVENT_ITEMSELECTCHANGE);////add by qsj 20100106 双击干掉定时器
	if(iItem>=0)
	{
		LPARAM  lParam = CTPListCtrl::GetItemData(iItem);
		if(!TP_IsNull(lParam))
		{
			LPTPDirListItemData pItemData = (LPTPDirListItemData)lParam;
			POSITION pos  = CTPListCtrl::GetFirstSelectedItemPosition();//add by qsj 20100106双击只使一个元素保持选中状态
			int      iSel = -1;
			while(pos)
			{
				iSel = CTPListCtrl::GetNextSelectedItem(pos);
				if(iSel>=0) CTPListCtrl::SelectItem(iSel,FALSE);
			}
			CTPListCtrl::SelectItem(iItem,TRUE);
			CWnd* pParent = GetParent();
			if(pItemData ->ulAttrs & SFGAO_FOLDER)
			{	
				LPSHELLFOLDER pChildFolder = NULL;
				HRESULT hResult = m_pShellFolder->BindToObject(pItemData->lpi, NULL, IID_IShellFolder, (void**)&pChildFolder);
				if(SUCCEEDED(hResult))
				{
					LPITEMIDLIST pItemID = ILClone(GetPathIDLIST(iItem));
					SetItemIDList(pItemID);
					if(pItemID) ILFree(pItemID);
					if(pChildFolder) pChildFolder->Release();

					if(pParent) pParent->PostMessage(WM_LISTLBUTTONDBLCLK,0,0);
				}
				else
				{
					CString sErrMsg = FormatErrorMsg(hResult);
					if(!sErrMsg.IsEmpty())
					{
						MessageBox(sErrMsg, _T(""), MB_OK|MB_ICONERROR);
					}
				}
				return ;
			}
			else
			{
				if(IsFolderItem(iItem))
				{
					LPITEMIDLIST pItemID = GetPathIDLIST(iItem);
					LPITEMIDLIST pItemLink = NULL;
					GetLinkPath(pItemID, &pItemLink);
					if(NULL != pItemLink)
					{
						SetItemIDList(pItemLink);
						ILFree(pItemLink); 
						pItemLink = NULL;
					}
					
					if(pParent) pParent->PostMessage(WM_LISTLBUTTONDBLCLK,0,0);
				}
				else
				{
					if(pParent) pParent->PostMessage(WM_LISTLBUTTONDBLCLK,1,0);
				}
			}
		}		
		else if(CTPListCtrl::GetItemText(iItem,0) == _L(".."))
		{		
			SetParentItem();
		}
	}
}
int CALLBACK TPDirListCtrlCompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
	if(TP_IsNull(lParam1)) return -1;
	if(TP_IsNull(lParam2)) return 1;
	BOOL bDesc   = LOWORD(lParamSort);
	int  iType   = HIWORD(lParamSort);
	int  iReturn = 0;
	LPTPDirListItemData lptvid1 = (LPTPDirListItemData)lParam1;
	LPTPDirListItemData lptvid2 = (LPTPDirListItemData)lParam2;

	if((FILE_ATTRIBUTE_DIRECTORY & lptvid1->dwFileAttributes) &&!(FILE_ATTRIBUTE_DIRECTORY & lptvid2->dwFileAttributes))      iReturn =  1;
	else if(!(FILE_ATTRIBUTE_DIRECTORY & lptvid1->dwFileAttributes) &&(FILE_ATTRIBUTE_DIRECTORY & lptvid2->dwFileAttributes)) iReturn = -1;

	else if(iType == HEAD_NAME)        iReturn = StrCmpLogicalW(lptvid1->sFileName,lptvid2->sFileName);
	else if(iType == HEAD_TYPE)        iReturn = StrCmpLogicalW(lptvid1->sType,lptvid2->sType);
	else if(iType == HEAD_LENGTH)      iReturn = lptvid1 ->iLength > lptvid2 ->iLength ? 1:-1;
	else if(iType == HEAD_CREATETIME) 
	{
		if(!lptvid1->bFindFile) return -1;
		if(!lptvid2->bFindFile) return 1;
		CTime ctime1 = CTime(lptvid1->ftCreationTime);					
		CTime ctime2 = CTime(lptvid2->ftCreationTime);					
		iReturn = ctime1>ctime2 ? 1 : -1;
	}
	else if(iType == HEAD_WRITETIME) 
	{
		if(!lptvid1->bFindFile) return -1;
		if(!lptvid2->bFindFile) return 1;
		CTime ctime1 = CTime(lptvid1->ftLastWriteTime);					
		CTime ctime2 = CTime(lptvid2->ftLastWriteTime);					
		iReturn = ctime1>ctime2 ?1: -1;
	}	
	else if(iType == HEAD_ACCESSTIME)
	{
		if(!lptvid1->bFindFile) return -1;
		if(!lptvid2->bFindFile) return 1;
		CTime ctime1 = CTime(lptvid1->ftLastAccessTime);					
		CTime ctime2 = CTime(lptvid2->ftLastAccessTime);					
		iReturn = ctime1>ctime2? 1:-1;
	}
	else if(iType == HEAD_ATTRIBUTE)
	{
		if(!lptvid1->bFindFile) return -1;
		if(!lptvid2->bFindFile) return 1;
		iReturn = lptvid1 ->dwFileAttributes > lptvid2 ->dwFileAttributes ? 1:-1;
	}
	return	 bDesc  ? -iReturn : iReturn;
}

void CTPDirListCtrl::OnHdnItemclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if((m_uStyle & LVS_NOSORTHEADER) || m_pHeadCtrl == NULL) return;
	int iColumn = phdr->iItem;
	if(iColumn <0) return;
	m_bDescSort  = !m_bDescSort;
	m_pHeadCtrl ->SetSortFlag(iColumn,m_bDescSort ? HDF_SORTDOWN:HDF_SORTUP);
	m_iSortIndex = iColumn;
	CTPListCtrl::SortItems(TPDirListCtrlCompareFunc,MAKELONG((WORD)m_bDescSort,(WORD)m_iHeadIndex[iColumn]));
}
void CTPDirListCtrl::SortItem(int iItem,BOOL bDesc)
{
	m_bDescSort = bDesc;
	m_iSortIndex = iItem;
	m_pHeadCtrl ->SetSortFlag(m_iSortIndex,(m_bDescSort) ? HDF_SORTDOWN:HDF_SORTUP);	
	CTPListCtrl::SortItems(TPDirListCtrlCompareFunc,MAKELONG((WORD)m_bDescSort,(WORD)m_iHeadIndex[iItem]));
}

DWORD CTPDirListCtrl::GetFileAttr(int iItem)
{
	if(iItem < 0)  return 0;
	TPDirListItemData *pItemData = (TPDirListItemData *)CTPListCtrl::GetItemData(iItem);
	if(pItemData) return pItemData ->dwFileAttributes;
	else return 0;
}

void CTPDirListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;	
	if((pNMLV ->uNewState & LVIS_SELECTED) != (pNMLV ->uOldState & LVIS_SELECTED))
	{
		SetTimer(TP_EVENT_ITEMSELECTCHANGE,10,NULL);////add by qsj 20100106，10，如果时间太长，双击import素材没有反应
	}	
	if((pNMLV ->uNewState & LVIS_SELECTED)  && pNMLV ->lParam)
	{
		LPTPDirListItemData pItemData = (LPTPDirListItemData)pNMLV ->lParam; 
		if(NULL != pItemData)
		{
			int iSelTick = GetTickCount();
			if(iSelTick <= m_iSelTick) iSelTick   = ++m_iSelTick;
			else				 m_iSelTick = iSelTick;
			pItemData->dwSelTick = iSelTick;
		}
	}
}

void CTPDirListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTPListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	if(nChar == VK_A && TP_IsCtrlKeyDown())
	{
		SetRedraw(FALSE);
		for(INT l=0;l<GetItemCount();l++)
		{
			SelectItem(l,TRUE);
		}
		SetRedraw(TRUE);
		if(GetSelectedCount()>0) GetParent() ->PostMessage(WM_LISTCTRL,LIST_SELECT,0);
	}
}

BOOL CTPDirListCtrl::IsEqualItem(LPITEMIDLIST pItemID1, LPITEMIDLIST pItemID2)
{
	if(NULL == pItemID1 || NULL == pItemID2) return FALSE;

	LPSHELLFOLDER pShellDestop = NULL;
	SHGetDesktopFolder(&pShellDestop);
	if(NULL == pShellDestop) return FALSE;

	STRRET strRet;
	TCHAR szParseName[MAX_PATH] = {0};
	CString sParseName = _T("");
	pShellDestop->GetDisplayNameOf(pItemID1, SHGDN_FORPARSING, &strRet);
	StrRetToBuf(&strRet, pItemID1, szParseName, MAX_PATH);
	sParseName = szParseName;

	pShellDestop->GetDisplayNameOf(pItemID2, SHGDN_FORPARSING, &strRet);
	StrRetToBuf(&strRet, pItemID2, szParseName, MAX_PATH);

	pShellDestop->Release();
	pShellDestop = NULL;
	if(sParseName.CompareNoCase(szParseName) == 0)
		return TRUE;
	return FALSE;
}

int CTPDirListCtrl::FindItemByID(LPITEMIDLIST hFindItemID)
{
	for(int i = 0 ; i< GetItemCount(); i++)
	{
		LPITEMIDLIST pFullItemID = GetPathIDLIST(i);
		if(IsEqualItem(pFullItemID,hFindItemID))	return i;
	}
	return -1;
}

LRESULT CTPDirListCtrl::OnFileChangeNotify(WPARAM wParam, LPARAM lParam)
{
	typedef struct
	{
		DWORD dwItem1;    // dwItem1 contains the previous PIDL or name of the folder. 
		DWORD dwItem2;    // dwItem2 contains the new PIDL or name of the folder. 
	} SHNOTIFYSTRUCT;

	SHNOTIFYSTRUCT *shns = (SHNOTIFYSTRUCT *)wParam;

	LPITEMIDLIST pItemIDList1 = reinterpret_cast<struct _ITEMIDLIST *>((DWORD_PTR)shns->dwItem1);
	LPITEMIDLIST pItemIDList2 = reinterpret_cast<struct _ITEMIDLIST *>((DWORD_PTR)shns->dwItem2);

	TCHAR szPath1[MAX_PATH]= {0};
	TCHAR szPath2[MAX_PATH] = {0};
	SHGetPathFromIDList(pItemIDList1, szPath1);
	SHGetPathFromIDList(pItemIDList2, szPath2);

	switch(lParam)
	{
	case SHCNE_MKDIR:
	case SHCNE_CREATE:
		{
			if(m_pShellFolder == NULL) break;

			LPITEMIDLIST pItemIDNew = ILClone(pItemIDList1);
			ILRemoveLastID(pItemIDNew);
			BOOL bShow = IsEqualItem(m_lpifqThis,pItemIDNew);                
			ILFree(pItemIDNew);
			pItemIDNew = NULL;		
			if(!bShow) break;

			int iItem = FindItemByID(pItemIDList1);       // 可能新建两次,防止重复新建
			if(iItem >=0) break;

			STRRET strRet;                                //  当前要添加的文件的名称
			TCHAR szParseName[MAX_PATH] = {0};
			LPSHELLFOLDER pParentFolder = NULL;
			LPITEMIDLIST pLastItem = NULL;
			SHBindToParent(pItemIDList1, IID_IShellFolder, (LPVOID*)&pParentFolder, (LPCITEMIDLIST*)&pLastItem);
			if(NULL == pParentFolder) break;

			pParentFolder->GetDisplayNameOf(pLastItem, SHGDN_INFOLDER|SHGDN_FORPARSING, &strRet);
			StrRetToBuf(&strRet, pLastItem, szParseName, MAX_PATH);
			pParentFolder->Release();
			pLastItem = NULL;

			LPITEMIDLIST pItemIDListNew = NULL;
			m_pShellFolder->ParseDisplayName(m_hWnd, NULL, szParseName, NULL,&pItemIDListNew, NULL);
			if(NULL != pItemIDListNew) 
				InsertListItem(GetItemCount(), m_pShellFolder, m_lpifqThis, pItemIDListNew);
			ILFree(pItemIDListNew);
			pItemIDListNew = NULL;
		}
		break;
	case SHCNE_RMDIR:
	case SHCNE_DELETE:
		{
			int iItem = FindItemByID(pItemIDList1);
			if(iItem >= 0)
			{
				CTPListCtrl::DeleteItem(iItem);
			}
		}
		break;
	case SHCNE_RENAMEFOLDER:
	case SHCNE_RENAMEITEM:
		{
			if(m_pShellFolder == NULL) break;
			LPITEMIDLIST pItemIDOld = ILClone(pItemIDList1);
			LPITEMIDLIST pItemIDNew = ILClone(pItemIDList2);
			ILRemoveLastID(pItemIDOld);
			ILRemoveLastID(pItemIDNew);
			BOOL bShow = IsEqualItem(m_lpifqThis,pItemIDOld); 
			if(!bShow) bShow = IsEqualItem(m_lpifqThis, pItemIDNew);

			BOOL bRename = ILIsEqual(pItemIDOld, pItemIDNew);
			ILFree(pItemIDOld);
			ILFree(pItemIDNew);
			pItemIDOld = NULL;
			pItemIDNew = NULL;
			if(!bShow)break;

			if(FindItemByID(pItemIDList2) >= 0) break;

			if(bRename)
			{
				int iItem = FindItemByID(pItemIDList1);
				if(iItem >= 0)
				{
					STRRET strRet;
					TCHAR szParseName[MAX_PATH] = {0};
					LPSHELLFOLDER pParentFolder = NULL;
					LPITEMIDLIST  pLastItem = NULL;
					SHBindToParent(pItemIDList2, IID_IShellFolder, (LPVOID*)&pParentFolder, (LPCITEMIDLIST*)&pLastItem);  // 名字
					if(NULL == pParentFolder) break;

					pParentFolder->GetDisplayNameOf(pLastItem, SHGDN_INFOLDER|SHGDN_FORPARSING, &strRet);
					StrRetToBuf(&strRet, pLastItem, szParseName, MAX_PATH);
					pParentFolder->Release();
					pLastItem = NULL;

					TPDirListItemData *pItemData = (TPDirListItemData *)CTPListCtrl::GetItemData(iItem);
					if(NULL == pItemData) break;

					LPITEMIDLIST pItemIDListNew = NULL;
					m_pShellFolder->ParseDisplayName(m_hWnd, NULL, szParseName, NULL,&pItemIDListNew, NULL);
					pItemData->lpi = ILClone(pItemIDListNew);
					CString sPath ;
					TCHAR szTemp[MAX_PATH] = {0};
					if(SHGetPathFromIDList(pItemIDList2,szTemp))
					{					
						sPath = PathFindFileName(szTemp);
						lstrcpyn(pItemData ->sFileName,(LPTSTR)(LPCTSTR)sPath,MAX_PATH);					
					}
					CTPListCtrl::SetItemText(iItem,0,(LPTSTR)(LPCTSTR)sPath);
					ILFree(pItemIDListNew);
					pItemIDListNew = NULL;
				}
			}
			else
			{
				UpdateFolder(m_pShellFolder, m_lpifqThis);
			}
		}
		break;
	case SHCNE_UPDATEDIR:
	case SHCNE_UPDATEITEM:
		{
			LPITEMIDLIST pItemDestop = NULL;
			SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pItemDestop);
			BOOL bDestop = IsEqualItem(pItemDestop, pItemIDList1);
			BOOL bShow   = IsEqualItem(m_lpifqThis,pItemIDList1); 
			ILFree(pItemDestop);
			pItemDestop = NULL;
			if(bShow || bDestop)// 桌面更新，也要刷新，桌面子目录刷新，是通知桌面刷新
			{
				SetTimer(TP_EVENT_UPDATEFOLDER, 700, NULL);
			}
		}
		break;
	}
	return  S_OK;

}
POSITION CTPDirListCtrl::GetFirstSelectedItemPosition(BOOL bSort) 
{
	if(bSort)
	{
		m_aSelectItem.RemoveAll();
		POSITION pos  = CTPListCtrl::GetFirstSelectedItemPosition();
		int      iSel = -1;
		while(pos)
		{
			iSel = CTPListCtrl::GetNextSelectedItem(pos);
			if(iSel>=0) m_aSelectItem.Add(iSel);
		}
		int iSelCount = (int)m_aSelectItem.GetSize();
		if(iSelCount <=0) return NULL;

		//add by qsj 20100106 元素getitemdata只取一次，节省时间
		CInt64Array adwSelTick;
		for (int i=0;i<iSelCount;i++)
		{
			LPTPDirListItemData pItemData1 = (LPTPDirListItemData)CTPListCtrl::GetItemData(m_aSelectItem[i]);
		    if (pItemData1)
		    {
				adwSelTick.Add(pItemData1->dwSelTick);
		    }
			else
			{
				adwSelTick.Add(0);
			}

		}
		for(int l=0; l<iSelCount; l++)
		{
			for(int k=l+1;k<iSelCount;k++)
			{
				if(adwSelTick[l] > adwSelTick[k])
				{
					int iSwap = m_aSelectItem[k];
					m_aSelectItem[k] = m_aSelectItem[l];
					m_aSelectItem[l] = iSwap;
					INT64 iExistTemp2 = adwSelTick[k];
					adwSelTick[k] = adwSelTick[l];
					adwSelTick[l] = iExistTemp2;
				}
			}
		}
		m_bSelectSort = bSort;	
		return (POSITION)1;
	}
	m_bSelectSort = FALSE;
	return CTPListCtrl::GetFirstSelectedItemPosition();
}
int CTPDirListCtrl::GetNextSelectedItem(POSITION& pos) 
{
	if(m_bSelectSort)
	{
		INT_PTR iSel    = (INT_PTR)pos;
		int     iReturn = m_aSelectItem[iSel-1];
		pos  = (POSITION)(iSel + 1);
		if(iSel>=m_aSelectItem.GetSize()) pos = 0;
		return iReturn;
	}

	return CTPListCtrl::GetNextSelectedItem(pos);
}

void CTPDirListCtrl::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == TP_EVENT_ITEMSELECTCHANGE)
	{
		KillTimer(TP_EVENT_ITEMSELECTCHANGE);
		POSITION pos    = GetFirstSelectedItemPosition(TRUE);
		int      iSel   = -1;
		while(pos)
		{
			iSel = GetNextSelectedItem(pos);	
		}	
		CWnd * pWnd = GetParent();
		if (pWnd)  
		{
			if(iSel >= 0)
				pWnd->PostMessage(WM_LISTCTRL,LIST_SELECT,iSel);
			else 
				pWnd->PostMessage(WM_LISTCTRL,LIST_UNSELECT,0);
		}
	}
	else if(nIDEvent == TP_EVENT_UPDATEFOLDER)
	{
		KillTimer(TP_EVENT_UPDATEFOLDER);
		UpdateFolder(m_pShellFolder,m_lpifqThis);
	}
}

BOOL CTPDirListCtrl::IsFolderItem(INT nIndex)
{
	if(nIndex < 0) return FALSE;
	if(nIndex >= GetItemCount()) return FALSE;
	TPDirListItemData *pItemData = (TPDirListItemData*)GetItemData(nIndex);
	if(pItemData && (SFGAO_FOLDER & pItemData->ulAttrs)) return TRUE;
	if(pItemData->ulAttrs & SFGAO_LINK)
	{
		LPITEMIDLIST pItemID = GetPathIDLIST(nIndex);
		return IsFolderItemID(pItemID);
	}
	return FALSE;
}

BOOL CTPDirListCtrl::IsFolderItemID(LPITEMIDLIST pItemID)
{
	IPersistFile* psf = NULL;
	IShellLink*   psl = NULL;
	ULONG ulAttrsLink = 0;
	LPITEMIDLIST pItemIDLink = NULL;
	GetLinkItem(pItemID, psf, psl);
	if(NULL != psl)
	{
		TCHAR szTempPath[MAX_PATH] = {0};
		psl->GetPath(szTempPath, MAX_PATH, NULL, SLGP_UNCPRIORITY);
		if(lstrlen(szTempPath) > 0)
		{
			psl->GetIDList(&pItemIDLink);			
			pItemID = pItemIDLink;
		}
	}


	LPSHELLFOLDER  psfParent    = NULL;
	LPITEMIDLIST  pidlRelative = NULL;
	HRESULT hResult = SHBindToParent(pItemID,IID_IShellFolder,(void**) &psfParent,(LPCITEMIDLIST*)&pidlRelative);
	if(SUCCEEDED(hResult))
	{
		ulAttrsLink = SFGAO_FOLDER;
		psfParent->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlRelative, &ulAttrsLink);
		psfParent->Release();
	}
	
	if(NULL != pItemIDLink) {ILFree(pItemIDLink); pItemIDLink = NULL;}
	if(NULL != psf) {psf->Release();psf = NULL;}
	if(NULL != psl) {psl->Release(); psl = NULL;}

	if(ulAttrsLink & SFGAO_FOLDER) return TRUE;

	return FALSE;
}

CString CTPDirListCtrl::FormatErrorMsg(DWORD dwErrorCode)
{
	CString sErrMsg;
	LPVOID lpMsgBuf = NULL;
	DWORD  dwFlags  = FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS;

	if(::FormatMessage(dwFlags,	NULL, dwErrorCode,	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0,	NULL))
	{
		sErrMsg = (LPCTSTR)lpMsgBuf;
	}

	if(lpMsgBuf != NULL)	::LocalFree(lpMsgBuf);

	return sErrMsg;
}

BOOL CTPDirListCtrl::CanMakeFolder()
{
	if(m_uState & STATE_SETDATA) return FALSE;
	CString sPathName = GetPathName();
	if(!sPathName.IsEmpty())
	{
		if(!PathIsDirectory(sPathName)) return FALSE;
		CString sDrive = sPathName.Left(2);
		UINT uDriveType = ::GetDriveType(sDrive);
		if(DRIVE_CDROM == uDriveType)
		{
			ULARGE_INTEGER uFreeSpace;
			uFreeSpace.QuadPart = 0;
			GetDiskFreeSpaceEx(sDrive, NULL, NULL, &uFreeSpace);
			if(uFreeSpace.QuadPart <= 0) return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}
