#pragma once

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