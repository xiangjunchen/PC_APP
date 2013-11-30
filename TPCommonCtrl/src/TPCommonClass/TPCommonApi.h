#pragma once

AFX_EXT_API	void           File_FindFile(CString strPath,CString sFileName, BOOL bPath,CStringArray &aFile);
AFX_EXT_API CString        File_FindFile(CString strPath,GUID guidFind);
AFX_EXT_API void           File_FindFile(CString strPath,CString sType,CStringArray &aFile);
AFX_EXT_API BOOL           TP_FindFile(CString& sFindPath);
AFX_EXT_API void           TP_FindFileOnly(CString strPath,CStringArray &aFile);
AFX_EXT_API BOOL		   TP_GetStatus(LPCTSTR lpszFileName, CFileStatus& rStatus);

//string
AFX_EXT_API void		   TP_StrCpy(TCHAR *&cDst, TCHAR *cSrc, size_t nSrcSize, BOOL bNewBuf = TRUE);
AFX_EXT_API int 	       TP_StrCmp(TCHAR *cS1, TCHAR *cS2, size_t nMaxSize = MAX_PATH);
AFX_EXT_API	int			   TP_StrLen(const TCHAR *cStr,	size_t nMaxSize = MAX_PATH);



