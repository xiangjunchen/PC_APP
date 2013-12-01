#pragma once

AFX_EXT_API	void           File_FindFile(CString strPath,CString sFileName, BOOL bPath,CStringArray &aFile);
AFX_EXT_API CString        File_FindFile(CString strPath,GUID guidFind);
AFX_EXT_API void           File_FindFile(CString strPath,CString sType,CStringArray &aFile);
AFX_EXT_API BOOL           TP_FindFile(CString& sFindPath);
AFX_EXT_API void           TP_FindFileOnly(CString strPath,CStringArray &aFile);
AFX_EXT_API BOOL		   TP_GetStatus(LPCTSTR lpszFileName, CFileStatus& rStatus);



