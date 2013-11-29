#include "StdAfx.h"
#include ".\tpautowaitcursor.h"

CTPAutoWaitCursor::CTPAutoWaitCursor(void)
{
}

CTPAutoWaitCursor::~CTPAutoWaitCursor(void)
{
}

//------------------------------------------------------------------------------------
// CTPRegUtils - Registry utility class
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
// Constructor
//

CTPRegUtils::CTPRegUtils(HKEY hRootKey)
: m_hKey(NULL)
{
	m_hRootKey = hRootKey;
}

//------------------------------------------------------------------------------------
// Destructor
//
CTPRegUtils::~CTPRegUtils()
{
	Close();
}

//------------------------------------------------------------------------------------
// Open
// Opens the specified subkey
LONG CTPRegUtils::Open(LPCTSTR lpSubKey)
{
	Close();
#ifdef TP_NS_EDIT
	m_subKeyName = _T("Software\\Sony\\");
#else
	m_subKeyName = _T("Software\\SobeyEditMax7\\");
#endif
	m_subKeyName += lpSubKey;
	return RegOpenKeyEx(m_hRootKey, m_subKeyName, 0, KEY_ALL_ACCESS, &m_hKey);
}
LONG  CTPRegUtils::OpenEx(HKEY hRootKey,LPCTSTR lpSubKey)
{
	m_hRootKey   = hRootKey;
	m_subKeyName = lpSubKey;
	return RegOpenKeyEx(m_hRootKey, m_subKeyName, 0, KEY_ALL_ACCESS, &m_hKey);
}

//------------------------------------------------------------------------------------
// Create
// Creates the specified subkey
LONG CTPRegUtils::Create(LPCTSTR lpSubKey)
{
	Close();
#ifdef TP_NS_EDIT
	m_subKeyName = _T("Software\\Sony\\");
#else
	m_subKeyName = _T("Software\\SobeyEditMax7\\");
#endif
	m_subKeyName += lpSubKey;
	return RegCreateKeyEx(m_hRootKey, m_subKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, NULL);
}

//------------------------------------------------------------------------------------
// Close
// Closes the subkey
void CTPRegUtils::Close()
{
	RegCloseKey(m_hKey);
	m_hKey = NULL;
	m_subKeyName.Empty();
}

//------------------------------------------------------------------------------------
// SetString
// Set the key for the named value of type string
LONG CTPRegUtils::SetString(LPCTSTR valueName, const BYTE* lpData, DWORD cbData)
{
	return RegSetValueEx(m_hKey, valueName, 0, REG_SZ, lpData, cbData);
}

//------------------------------------------------------------------------------------
// GetString
// Query the key for the named value of type string
LONG CTPRegUtils::GetString(LPCTSTR valueName, LPBYTE lpData, DWORD cbData)
{
	LONG ret = ERROR_SUCCESS;
	DWORD size = cbData, type = 0;
	ret = RegQueryValueEx(m_hKey, valueName, NULL, &type, lpData, &size);
	if ((REG_SZ != type))
	{
		ret = ERROR_INVALID_PARAMETER;
	}
	return ret;
}

//------------------------------------------------------------------------------------
// SetBinary
// Set the key for the named value of type binary
LONG CTPRegUtils::SetBinary(LPCTSTR valueName, const BYTE* lpData, DWORD cbData)
{
	return RegSetValueEx(m_hKey, valueName, 0, REG_BINARY, lpData, cbData);
}

//------------------------------------------------------------------------------------
// GetBinary
// Query the key for the named value of type binary
LONG CTPRegUtils::GetBinary(LPCTSTR valueName, LPBYTE lpData, DWORD cbData)
{
	LONG ret = ERROR_SUCCESS;
	DWORD size = cbData, type = 0;
	ret = RegQueryValueEx(m_hKey, valueName, NULL, &type, lpData, &size);
	if ((size != cbData) || (REG_BINARY != type))
	{
		ret = ERROR_INVALID_PARAMETER;
	}
	return ret;
}

//------------------------------------------------------------------------------------
// SetDword
// Set the key for the named value of type DWORD
LONG CTPRegUtils::SetDword(LPCTSTR valueName, const DWORD* lpData, DWORD cbData)
{
	return RegSetValueEx(m_hKey, valueName, 0, REG_DWORD, (LPBYTE)lpData, cbData);
}

//------------------------------------------------------------------------------------
// GetDword
// Query the key for the named value of type DWORD
LONG CTPRegUtils::GetDword(LPCTSTR valueName, DWORD* lpData, DWORD cbData)
{
	LONG ret = ERROR_SUCCESS;
	DWORD size = cbData, type = 0;
	ret = RegQueryValueEx(m_hKey, valueName, NULL, &type, (LPBYTE)lpData, &size);
	if ((size != cbData) || (REG_DWORD != type))
	{
		ret = ERROR_INVALID_PARAMETER;
	}
	return ret;
}

//------------------------------------------------------------------------------------
// GetDword
// Query CTPRegUtils key for the named value of type DWORD
DWORD CTPRegUtils::GetDword(LPCTSTR valueName, DWORD cbData)
{
	LONG ret = ERROR_SUCCESS;
	DWORD size = sizeof(cbData), type = 0,dwData;
	ret = RegQueryValueEx(m_hKey, valueName, NULL,&type,(LPBYTE)&dwData, &size);
	if ((size != sizeof(cbData)) || (REG_DWORD != type)) return cbData;	
	else                                                 return dwData;	
}
LONG CTPRegUtils::SetDword(LPCTSTR valueName, DWORD cbData)
{
	return RegSetValueEx(m_hKey, valueName, 0, REG_DWORD, (LPBYTE)&cbData,sizeof(cbData));
}

LONG CTPRegUtils::Open(LPVOID lpSubKey)
{
	return Open((LPCTSTR)lpSubKey);
}

LONG CTPRegUtils::Create(LPVOID lpSubKey)
{
	return Create((LPCTSTR)lpSubKey);
}

LONG CTPRegUtils::SetDword(LPVOID valueName, const DWORD* lpData, DWORD cbData)
{
	return SetDword((LPCTSTR)valueName, lpData, cbData);
}

LONG CTPRegUtils::GetDword(LPVOID valueName, DWORD* lpData, DWORD cbData)
{
	return GetDword((LPCTSTR)valueName, lpData, cbData);
}
