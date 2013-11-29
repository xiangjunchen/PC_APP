#pragma once


typedef struct _tagTPWinFileItem
{
	LPITEMIDLIST               lpi;
	LPITEMIDLIST			   pFullItemID;

	int                        iImage;	
	TCHAR                      sFileName[MAX_PATH];
	TCHAR                      sFilePath[MAX_PATH];
	TCHAR                      sType[MAX_PATH];
	__int64                    iLength;
	DWORD                      dwFileAttributes; 
	FILETIME                   ftCreationTime;
	FILETIME                   ftLastAccessTime;
	FILETIME                   ftLastWriteTime;
	ULONG                      ulAttrs;	
	BOOL                       bFindFile ; 
	DWORD                      dwSelTick; 
	_tagTPWinFileItem()
	{
		Reset();
	}
	~_tagTPWinFileItem()
	{
		Release();
	}
	void Reset()
	{
		iImage       = -1;		
		lpi          = NULL;
		pFullItemID  = NULL;
		sFileName[0] = _T('\0');
		sFilePath[0] = _T('\0');
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
}TPWinFileItem, *LPTPWinFileItem;

typedef CArray<TPWinFileItem*, TPWinFileItem *&>	 CTPWinFileItemArray;