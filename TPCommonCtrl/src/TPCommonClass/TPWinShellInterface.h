#pragma once

class AFX_EXT_CLASS CTPWinShellInterface
{
public:
	CTPWinShellInterface(void);
	~CTPWinShellInterface(void);

private:
	ULONG             m_uSfGao;
	LPSHELLFOLDER     m_pShellFolder;
	LPITEMIDLIST      m_lpifqThis; 
	CStringArray      m_aFilter;
	CArray<LPVOID, LPVOID> m_aExceptItemID;

public:

	void 				GetLinkItem(LPITEMIDLIST pItemID,IPersistFile*& psf, IShellLink*& psl);
	BOOL 				IsFolderItemID(LPITEMIDLIST pItemID);
	BOOL				IsAppItemID(LPITEMIDLIST pItemID, LPTSTR &sTypeName, LPTSTR &sOriginalName,LPTSTR &sLinkPath);
	BOOL			 	IsExceptItemID(LPITEMIDLIST pItemID);
	void			 	InitExceptItemID();
	void			 	ReleaseExceptItemID();
	BOOL			 	IsEqualItem(LPITEMIDLIST pItemID1, LPITEMIDLIST pItemID2);
	LRESULT				GetDestopFolderChild(CTPWinFileItemArray &aFileItemArray);
	LRESULT				GetProgramsChild(CTPWinFileItemArray &aFileItemArray);
	LRESULT				GetComputerApp(CTPWinFileItemArray &aFileItemArray);
	LRESULT				FilterAppItem(CTPWinFileItemArray &aFileItemArray);
	LPTPWinFileItem	 	GetWinFileItem(LPSHELLFOLDER pShellFolder, LPITEMIDLIST pParentID, LPITEMIDLIST pChildID);
	
};
