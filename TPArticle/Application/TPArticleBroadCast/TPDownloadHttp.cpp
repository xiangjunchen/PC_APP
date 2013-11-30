#include "StdAfx.h"
#include "TPDownloadHttp.h"

CTPDownloadHttp::CTPDownloadHttp(void)
{
	m_cDownloadPath = NULL;
	m_cHttpUrl = NULL;
	m_pDownloadBuffer = NULL;
}
DWORD g_dwTempSize = 0;
CTPDownloadHttp::~CTPDownloadHttp(void)
{
	if(m_pDownloadBuffer)		{delete m_pDownloadBuffer; m_pDownloadBuffer = NULL;}
}
UINT CTPDownloadHttp::InternetGetFile(HINTERNET IN hOpen, // Handle from InternetOpen()
                 const TCHAR *szUrl,        // Full URL
                 const TCHAR *szFileName,
				 HWND hwndProgress,
				 int idStatusText,
				 int idProgressBar)
{
	DWORD dwSize;
	TCHAR   szHead[] = _T("Accept: */*\r\n\r\n");
	VOID* szTemp[16384];
	HINTERNET  hConnect;
	FILE * pFile;

	if ( !(hConnect = InternetOpenUrl ( hOpen, szUrl, szHead,
		TP_StrLen(szHead), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
	{
		return INTERNET_ERROR_OPENURL;
	}

	if  ( !(pFile = _tfopen (szFileName, _T("wb") ) ) )
	{
		return INTERNET_ERROR_FILEOPEN;
	}

	DWORD dwByteToRead = 0;
	DWORD dwSizeOfRq = 4;
	DWORD dwBytes = 0;

	if (!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
		(LPVOID)&dwByteToRead, &dwSizeOfRq, NULL))
	{
		dwByteToRead = 0;
	}
	DWORD start;
	DWORD end;
	DWORD time;
	time = 10;
	start = GetTickCount();
	do
	{
		// Keep coping in 16 KB chunks, while file has any data left.
		// Note: bigger buffer will greatly improve performance.
		if (!InternetReadFile (hConnect, szTemp, 16384,  &dwSize) )
		{
			fclose (pFile);
			return INTERNET_ERROR_READFILE;
		}
		if (!dwSize)
			break;  // Condition of dwSize=0 indicate EOF. Stop.
		else
			fwrite(szTemp, sizeof (char), dwSize , pFile);
		dwBytes+=dwSize;
		if(dwByteToRead && hwndProgress)
		{
			SendDlgItemMessageA(hwndProgress, idProgressBar, WM_USER+2, (dwBytes*100)/dwByteToRead, 0);
			UpdateWindow(hwndProgress);
		}
		FLOAT fSpeed = 0;
		fSpeed = (float)dwBytes;
		fSpeed /= ((float)time)/1000.0f;
		fSpeed /= 1024.0f;
		if(hwndProgress)
		{
			char s[260];
			sprintf(s, "%d KB / %d KB @ %1.1f KB/s", dwBytes/1024, dwByteToRead/1024, fSpeed);
			SetDlgItemTextA(hwndProgress, idStatusText, s);
			UpdateWindow(hwndProgress);
		}
		end = GetTickCount();
		time = end - start;
		if(time == 0)
			time = 10;
	}   // do
	while (TRUE);
	fflush (pFile);
	fclose (pFile);
	g_dwTempSize = dwSize;
	return 0;
}

int CTPDownloadHttp::InternetDownloadFile(HWND progressWindow, int idStatusText, int idProgressBar, const TCHAR *URL, TCHAR *FileDest)
{
	DWORD dwFlags;
	DWORD dwResult = INTERNET_ERROR_OPEN;
	InternetGetConnectedState(&dwFlags, 0);
	if(dwFlags & INTERNET_CONNECTION_OFFLINE)//take appropriate steps
		return INTERNET_ERROR_OPEN;
	CHAR strAgent[64];
	sprintf(strAgent, "Agent%ld", GetTickCount());
	HINTERNET hOpen;
	if(!(dwFlags & INTERNET_CONNECTION_PROXY))
	hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	else
	hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(hOpen)
	{
		dwResult = InternetGetFile(hOpen, URL, FileDest, progressWindow, idStatusText, idProgressBar);
		InternetCloseHandle(hOpen);
	}
	else return INTERNET_ERROR_OPEN;
	return dwResult;
}
BOOL CTPDownloadHttp::SetHttpUrl(const TCHAR *cHttpUrl, TCHAR *cDownloadPath)
{
	int nLen = TP_StrLen(cHttpUrl) ;
	if(!cHttpUrl || (nLen <= 0))			return FALSE;

	m_cHttpUrl = cHttpUrl;
	m_cDownloadPath = cDownloadPath;
	TCHAR cPath[MAX_PATH];
	::GetModuleFileName(NULL,cPath,MAX_PATH);
	PathRemoveFileSpec(cPath);	
	lstrcat(cPath,_T("\\temp.xml"));
	lstrcpy(m_cDownloadPath, cPath);
	return TRUE;
}
BOOL CTPDownloadHttp::Download()
{
	if(!m_cHttpUrl)			return FALSE;

	if(TP_StrLen(m_cDownloadPath) <= 0)	ASSERT(0);

	InternetDownloadFile(/*AfxGetMainWnd()->m_hWnd*/NULL,4,4,m_cHttpUrl,m_cDownloadPath);

	if(!PathFileExists(m_cDownloadPath))	ASSERT(0);
	return TRUE;
}

LPBYTE CTPDownloadHttp::GetBuffer()
{
	//FILE* pFile ;
	//pFile = _tfopen(m_cDownloadPath,_T("rb"));
	//m_pDownloadBuffer = new BYTE[g_dwTempSize];
	//fread(m_pDownloadBuffer,sizeof(TCHAR),g_dwTempSize,pFile);
	//fclose(pFile);
	return NULL;
}