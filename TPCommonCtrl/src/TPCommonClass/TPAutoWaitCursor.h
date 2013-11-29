#pragma once

class AFX_EXT_CLASS CTPAutoWaitCursor :public CWaitCursor
{
public:
	CTPAutoWaitCursor(void);
	~CTPAutoWaitCursor(void);
};

class AFX_EXT_CLASS CTPRegUtils
{
public:
	CTPRegUtils(HKEY hRootKey = HKEY_CURRENT_USER);
	~CTPRegUtils();

	LONG Open(LPCTSTR lpSubKey);
	LONG OpenEx(HKEY hRootKey,LPCTSTR lpSubKey);
	LONG Create(LPCTSTR lpSubKey);
	void Close();

	LONG  SetString(LPCTSTR valueName, const BYTE* lpData, DWORD cbData);
	LONG  GetString(LPCTSTR valueName, LPBYTE lpData, DWORD cbData);

	LONG  SetBinary(LPCTSTR valueName, const BYTE* lpData, DWORD cbData);
	LONG  GetBinary(LPCTSTR valueName, LPBYTE lpData, DWORD cbData);

	LONG  SetDword(LPCTSTR valueName, const DWORD* lpData, DWORD cbData);
	LONG  GetDword(LPCTSTR valueName, DWORD* lpData, DWORD cbData);

	DWORD GetDword(LPCTSTR valueName, DWORD cbData);	
	LONG  SetDword(LPCTSTR valueName, DWORD cbData);

	//for xmio
	LONG Open(LPVOID lpSubKey);
	LONG Create(LPVOID lpSubKey);

	LONG  SetDword(LPVOID valueName, const DWORD* lpData, DWORD cbData);
	LONG  GetDword(LPVOID valueName, DWORD* lpData, DWORD cbData);

private:
	CString m_subKeyName;
	HKEY    m_hKey;
	HKEY    m_hRootKey;
};