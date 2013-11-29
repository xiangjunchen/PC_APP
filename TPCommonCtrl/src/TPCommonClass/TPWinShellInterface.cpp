#include "StdAfx.h"
#include <shellapi.h>

#define TP_TYPENAME_LEN				79

INT g_Temp  = 0;
void TP_PathRemoveExtension(LPTSTR &lpFileName)
{
	//CString sFileName = _T("");
	//PathRemoveExtension(lpFileName);
	//sFileName = lpFileName;
	//if(sFileName.Find(_T(".")) != -1)
	//	return TP_PathRemoveExtension(lpFileName);
	return ;

};
CTPWinShellInterface::CTPWinShellInterface(void)
{
	m_pShellFolder	 = NULL;
	m_uSfGao		 = SFGAO_FOLDER | SFGAO_FILESYSTEM;
	m_lpifqThis      = NULL;
	m_aFilter.RemoveAll();
	InitExceptItemID();
}

CTPWinShellInterface::~CTPWinShellInterface(void)
{
	if(m_pShellFolder) {m_pShellFolder->Release(); m_pShellFolder = NULL;}
	if(m_lpifqThis) {ILFree(m_lpifqThis); m_lpifqThis = NULL;}
}
BOOL CTPWinShellInterface::IsEqualItem(LPITEMIDLIST pItemID1, LPITEMIDLIST pItemID2)
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
void CTPWinShellInterface::InitExceptItemID()
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
void CTPWinShellInterface::ReleaseExceptItemID()
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
BOOL CTPWinShellInterface::IsExceptItemID(LPITEMIDLIST pItemID)
{
	for(INT i=0; i<m_aExceptItemID.GetSize(); i++)
	{
		LPITEMIDLIST pItemIDTemp = (LPITEMIDLIST)m_aExceptItemID[i];
		if(ILIsEqual(pItemIDTemp,pItemID))
			return TRUE;
	}
	return FALSE;
}

LPTPWinFileItem	 	CTPWinShellInterface::GetWinFileItem(LPSHELLFOLDER pShellFolder, LPITEMIDLIST pParentID, LPITEMIDLIST pChildID)
{
	if(NULL == pShellFolder || NULL == pParentID || NULL == pChildID)	return	NULL;
	ULONG ulAttrsShow				= m_uSfGao;
	ULONG ulAttrs					= m_uSfGao|SFGAO_BROWSABLE|SFGAO_LINK;
	TCHAR szBuff[MAX_PATH]    		= {0};
	TCHAR szOriginalName[MAX_PATH]    		= {0};
	TCHAR szLinkPath[MAX_PATH]    		= {0};
	LPTSTR lpDisPlayFileName  		= NULL;
	LPTSTR lpTypeName         		= NULL;
	LPTSTR lpOriginalName            = NULL;
	LPTSTR lpLinkPath         		= NULL;
	//CString sOriginalName = _T("");
	BOOL	bAppType          		= FALSE;
	BOOL	bDisplayFileName  		= FALSE;
	LPTPWinFileItem pWinFileItem	= NULL;

	pShellFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pChildID, &ulAttrs);
	if (!(ulAttrs & (m_uSfGao|SFGAO_LINK))) return pWinFileItem;

	LPITEMIDLIST pFullItemID = ILCombine(pParentID, pChildID);
 	if(IsExceptItemID(pFullItemID))
 	{
 		ILFree(pFullItemID);
 		pFullItemID = NULL;
 		return NULL;
 	}
	if(SHGetPathFromIDList(pFullItemID,szBuff))
		lpDisPlayFileName = PathFindFileName(szBuff);

	BOOL bNormalFile = !(ulAttrs & SFGAO_FOLDER) || (ulAttrs & SFGAO_BROWSABLE) && !(ulAttrsShow & SFGAO_BROWSABLE);
	if(bNormalFile && (ulAttrs & SFGAO_LINK))// ¼ì²éÄ¿Â¼¿ì½Ý·½Ê½
	{
		if(IsFolderItemID(pFullItemID)) bNormalFile = FALSE;
		if(IsAppItemID(pFullItemID,lpTypeName,lpOriginalName,lpLinkPath))		bAppType = TRUE;
		lstrcpyn(szOriginalName ,lpOriginalName,MAX_PATH);
		lstrcpyn(szLinkPath ,lpLinkPath,MAX_PATH);
	}
	
	SHFILEINFO    sfi;
	ZeroMemory(&sfi, sizeof(sfi));
	pWinFileItem = new TPWinFileItem;
	pWinFileItem->lpi = ILClone(pChildID);

	SHGetFileInfo((LPCTSTR)pFullItemID,0,&sfi,sizeof(SHFILEINFO),SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_TYPENAME|SHGFI_DISPLAYNAME);
	ILFree(pFullItemID);

	if(sfi.hIcon) DestroyIcon(sfi.hIcon);
	pWinFileItem ->iImage = sfi.iIcon;

	if(bAppType && lpTypeName)
	{
		lstrcpyn(pWinFileItem->sType ,lpTypeName,MAX_PATH);
		lstrcpyn(pWinFileItem->sFilePath ,szLinkPath,MAX_PATH);
	}
	else
	{
		lstrcpyn(pWinFileItem->sFilePath ,szBuff,MAX_PATH);
		lstrcpyn(pWinFileItem->sType ,sfi.szTypeName,MAX_PATH);
	}


	if(!bNormalFile) lpDisPlayFileName = NULL;

	if(bDisplayFileName && lpDisPlayFileName)
		lstrcpyn(pWinFileItem->sFileName, lpDisPlayFileName,MAX_PATH);
	//else if(sOriginalName)
	//	lstrcpyn(pWinFileItem ->sFileName,sOriginalName,MAX_PATH);					
	else
		lstrcpyn(pWinFileItem ->sFileName,sfi.szDisplayName,MAX_PATH);	

	LPTSTR lpTemp = pWinFileItem ->sFileName;
	TP_PathRemoveExtension(lpTemp);
	lstrcpyn(pWinFileItem ->sFileName,lpTemp,MAX_PATH);	

	pWinFileItem->ulAttrs = ulAttrs;

	WIN32_FIND_DATA findFileData;
	if(S_OK == SHGetDataFromIDList(pShellFolder, pChildID, SHGDFIL_FINDDATA, &findFileData, sizeof(findFileData)))
	{
		pWinFileItem ->iLength           = findFileData.nFileSizeHigh;
		pWinFileItem ->iLength         <<= 32;
		pWinFileItem ->iLength          += findFileData.nFileSizeLow;						
		pWinFileItem ->dwFileAttributes  = findFileData.dwFileAttributes;
		pWinFileItem ->ftCreationTime    = findFileData.ftCreationTime;
		pWinFileItem ->ftLastAccessTime  = findFileData.ftLastAccessTime;
		pWinFileItem ->ftLastWriteTime   = findFileData.ftLastWriteTime;
		pWinFileItem ->bFindFile         = TRUE;
	}
	return pWinFileItem;
}
LRESULT	 CTPWinShellInterface::FilterAppItem(CTPWinFileItemArray &aFileItemArray)
{
	LRESULT lRet = S_OK;
	CString sAppFilterChinese = _T("应用程序");
	CString sAppFilterEnglish = _T("Application");
	CString sTypeName         = _T("");
	CString sFileName         = _T("");
	for (INT l = aFileItemArray.GetSize() - 1 ; l >=0 ; l--)
	{
		sTypeName = aFileItemArray[l]->sType;
		sFileName = aFileItemArray[l]->sFileName;
		if(sAppFilterEnglish.CompareNoCase(sTypeName)!=0 && sAppFilterChinese.CompareNoCase(sTypeName) != 0)
		{
			delete aFileItemArray[l];
			aFileItemArray[l] = NULL;
			aFileItemArray.RemoveAt(l);
			continue;
		}
	}
	return lRet;
}
BOOL CTPWinShellInterface::IsAppItemID(LPITEMIDLIST pItemID, LPTSTR &sTypeName, LPTSTR &lpOriginalName,LPTSTR &sLinkPath)
{
	IPersistFile* psf = NULL;
	IShellLink*   psl = NULL;
	ULONG ulAttrsLink = 0;
	LPITEMIDLIST pItemIDLink = NULL;
	CString		sAppType = _T(".exe");
	CString     sOriginalName = _T("");
	GetLinkItem(pItemID, psf, psl);
	if(NULL != psl)
	{
		TCHAR szTempPath[MAX_PATH] = {0};
		psl->GetPath(szTempPath, MAX_PATH, NULL, SLGP_UNCPRIORITY);
		if(lstrlen(szTempPath) > 0)
		{
			sLinkPath = szTempPath;
			sTypeName = PathFindExtension(szTempPath);
			if(sAppType.CompareNoCase(sTypeName) == 0)
			{
				sTypeName = _T("Application");
				lpOriginalName = PathFindFileName(szTempPath);				
				TP_PathRemoveExtension(lpOriginalName);
				return TRUE;
			}
		}
	}
	return FALSE;
}
BOOL CTPWinShellInterface::IsFolderItemID(LPITEMIDLIST pItemID)
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
void CTPWinShellInterface::GetLinkItem(LPITEMIDLIST pItemID,IPersistFile*& psf, IShellLink*& psl)
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
LRESULT CTPWinShellInterface::GetComputerApp(CTPWinFileItemArray &aFileItemArray)
{
	//桌面
	GetDestopFolderChild(aFileItemArray);
	//开始菜单
	GetProgramsChild(aFileItemArray);

	//m_aFileItemArray = &aFileItemArray;
	return S_OK;
}
LRESULT CTPWinShellInterface::GetProgramsChild(CTPWinFileItemArray &aFileItemArray)
{
	LRESULT lRet = S_FALSE;
	HRESULT hResult = S_FALSE;
	LPSHELLFOLDER pShellPrograms =NULL;
	LPITEMIDLIST  pItemPrograms=NULL;
	LPTPWinFileItem	pWinFileItem = NULL;
	LPSHELLFOLDER psfParent    = NULL; 
	LPCITEMIDLIST pidlRelative = NULL; 	

	BOOL bSetFolder = FALSE;
	hResult = SHGetSpecialFolderLocation(NULL,	CSIDL_PROGRAMS,	&pItemPrograms);//CSIDL_COMMON_PROGRAMS
	if(!SUCCEEDED(hResult))		return lRet;

	hResult = SHBindToParent(pItemPrograms,IID_IShellFolder,(void**) &psfParent,&pidlRelative);
	if(!SUCCEEDED(hResult))		return lRet;

	ULONG ulAttrs = SFGAO_LINK |m_uSfGao ;
	psfParent->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlRelative, &ulAttrs);
	hResult = psfParent->BindToObject(pidlRelative,0, IID_IShellFolder,(LPVOID *)&pShellPrograms);
	if(!SUCCEEDED(hResult))		return lRet;

	bSetFolder = TRUE;
	if (pShellPrograms && pItemPrograms)
	{
		ULONG               ulwork     = 0;
		HRESULT             hResert    = S_FALSE;
		HWND                hwnd       = NULL;//::GetParent(m_hWnd);
		LPENUMIDLIST        lpwork     = NULL;
		LPITEMIDLIST        lpi = NULL;
		LPITEMIDLIST		pItemIDList =ILClone(pItemPrograms);// ÏÈ¸´ÖÆ£¬ÔÙÊÍ·Å
		TCHAR szPathTemp[MAX_PATH] = {0};
		if(m_lpifqThis) {ILFree(m_lpifqThis); m_lpifqThis = NULL;}
		m_lpifqThis = pItemIDList;
		pShellPrograms->AddRef();
		if(m_pShellFolder) {m_pShellFolder->Release(); m_pShellFolder = NULL;}
		m_pShellFolder = pShellPrograms;

		//¿ªÊ¼ÁÐ¾ÙÊý¾Ý
		if (SUCCEEDED(hResert))
		{
			hResert=pShellPrograms->EnumObjects(hwnd, SHCONTF_FOLDERS| SHCONTF_NONFOLDERS, &lpwork);
			if (SUCCEEDED(hResert))
			{
				while(lpwork->Next(1, &lpi, &ulwork)==S_OK)
				{
					pWinFileItem = GetWinFileItem(pShellPrograms,m_lpifqThis,lpi);
					if(pWinFileItem)		aFileItemArray.Add(pWinFileItem);
					ILFree(lpi);
				}
				lRet = S_OK;
			}				
		}
		if (lpwork) lpwork->Release();
	}	
	if(pShellPrograms) {pShellPrograms->Release(); pShellPrograms = NULL;}
	if(pItemPrograms) {ILFree(pItemPrograms); pItemPrograms = NULL;}
	return lRet;
}

LRESULT CTPWinShellInterface::GetDestopFolderChild(CTPWinFileItemArray &aFileItemArray)
{
	LRESULT lRet = S_FALSE;
	LPSHELLFOLDER pShellFolder =NULL;
	LPITEMIDLIST  pDestopID=NULL;
	LPTPWinFileItem	pWinFileItem = NULL;
	HRESULT hr = SHGetDesktopFolder(&pShellFolder);	
	if(!SUCCEEDED(hr))	return lRet;
	SHGetSpecialFolderLocation(NULL,	CSIDL_DESKTOP,	&pDestopID);
	if (pShellFolder && pDestopID)
	{
		ULONG               ulwork     = 0;
		HRESULT             hResert    = S_FALSE;
		HWND                hwnd       = NULL;//::GetParent(m_hWnd);
		LPENUMIDLIST        lpwork     = NULL;
		LPITEMIDLIST        lpi = NULL;
		LPITEMIDLIST		pItemIDList =ILClone(pDestopID);// ÏÈ¸´ÖÆ£¬ÔÙÊÍ·Å
		TCHAR szPathTemp[MAX_PATH] = {0};
		if(m_lpifqThis) {ILFree(m_lpifqThis); m_lpifqThis = NULL;}
		m_lpifqThis = pItemIDList;
		pShellFolder->AddRef();
		if(m_pShellFolder) {m_pShellFolder->Release(); m_pShellFolder = NULL;}
		m_pShellFolder = pShellFolder;

		CStringArray aDesktopChild;

		//¿ªÊ¼ÁÐ¾ÙÊý¾Ý
		if (SUCCEEDED(hResert))
		{
			hResert=pShellFolder->EnumObjects(hwnd, SHCONTF_FOLDERS| SHCONTF_NONFOLDERS, &lpwork);
			if (SUCCEEDED(hResert))
			{
				while(lpwork->Next(1, &lpi, &ulwork)==S_OK)
				{
					pWinFileItem = GetWinFileItem(pShellFolder,m_lpifqThis,lpi);
					if(pWinFileItem)		aFileItemArray.Add(pWinFileItem);
					ILFree(lpi);
				}
				lRet = S_OK;
			}				
		}
		if (lpwork) lpwork->Release();
	}	
	if(pShellFolder) {pShellFolder->Release(); pShellFolder = NULL;}
	if(pDestopID) {ILFree(pDestopID); pDestopID = NULL;}
	return lRet;
}
//GetChildFolder
//STRRET str;
//pItemListTemp = ILClone(lpi);
//if (NOERROR==m_pShellFolder->GetDisplayNameOf(lpi,SHGDN_NORMAL, &str))
//	StrRetToBuf(&str,pItemListTemp,szPathTemp,MAX_PATH);
//aDesktopChild.Add(szPathTemp);
//ILFree(pItemListTemp);

