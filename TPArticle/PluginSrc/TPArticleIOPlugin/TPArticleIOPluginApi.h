#pragma once

CString TP_GetSysPath();
void    File_FindFile(CString strPath,CString strFile,BOOL bPath,CStringArray &aFile);
void    File_FindFile(CString strPath,CString sType,CStringArray &aFile);
class CTPMemFile : public CMemFile
{
public:
	CTPMemFile(void);
	~CTPMemFile(void);

	LRESULT ReadFile(LPCTSTR sFileName);
	LRESULT WriteFile(LPCTSTR sFileName);
public:
	virtual void Close();

};

class  CTPMapStringToString :public CMapStringToString
{
public:
	CTPMapStringToString();
	~CTPMapStringToString();

	void    Lock();
	void    Unlock();

public:

	BOOL    Lookup(LPCTSTR key, CString& rValue)  ;
	void    SetAt(LPCTSTR key, LPCTSTR newValue);
	void    SetAtEx(LPCTSTR key, LPCTSTR newValue);
	BOOL    RemoveKey(LPCTSTR key);
	void    RemoveAll();
	INT_PTR GetSize();

public:
	CRITICAL_SECTION  m_pCriticalSection;
};