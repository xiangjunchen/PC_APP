#pragma once
#include <wininet.h>

enum INTERNET_ERROR
{
	INTERNET_ERROR_OPENURL=1,
	INTERNET_ERROR_FILEOPEN,
	INTERNET_ERROR_READFILE,
	INTERNET_ERROR_OPEN
};

class CTPDownloadHttp
{
public:
	CTPDownloadHttp(void);
	~CTPDownloadHttp(void);

private:
	TCHAR *m_cDownloadPath;
	const TCHAR *m_cHttpUrl;
	BYTE  *m_pDownloadBuffer;
	UINT InternetGetFile (HINTERNET IN hOpen, // Handle from InternetOpen()
		const TCHAR *szUrl,        // Full URL
		const TCHAR *szFileName,
		HWND hwndProgress,
		int idStatusText,
		int idProgressBar);

	int InternetDownloadFile(HWND progressWindow, int idStatusText, int idProgressBar, const TCHAR *URL, TCHAR *FileDest);

public:
	BOOL SetHttpUrl(const TCHAR *cHttpUrl, TCHAR *cDownloadPath);
	BOOL Download();	
	LPBYTE GetBuffer();
};
