#include "StdAfx.h"

CTPMemFile::CTPMemFile(void)
{
}

CTPMemFile::~CTPMemFile(void)
{
}

LRESULT CTPMemFile::ReadFile(LPCTSTR sFileName)
{
	CFile    cRead;
	CFileException  eFail;
	if(!cRead.Open(sFileName,CFile::modeRead,&eFail))
	{
		return S_FALSE;
	}
	try
	{
		long lReadSize = cRead.GetLength();
		BYTE *pBuf = new BYTE[lReadSize];
		DWORD nSize = cRead.Read(pBuf,lReadSize);
 		Attach(pBuf,lReadSize,0);
	}
	catch (...)
	{
		ASSERT(0);
	}	
	cRead.Close();

	return S_OK;
}
LRESULT CTPMemFile::WriteFile(LPCTSTR sFileName)
{
	CFile cFile;
	UINT    nLong       = m_nPosition;
	LPBYTE pBufferRet   = m_lpBuffer;
	if(!cFile.Open(sFileName, CFile::modeCreate | CFile::modeWrite))
	{
		return S_FALSE;
	}
	try
	{
		cFile.SeekToBegin();
		cFile.Write(pBufferRet, nLong);
		cFile.Flush();
	}
	catch (...)
	{
		ASSERT(0);
	}	
	cFile.Close();
	return S_OK;
}
void CTPMemFile::Close()
{
	LPBYTE lpByte = Detach();
	if(lpByte)
	{
		delete lpByte;
		lpByte = NULL;
	}
	CMemFile::Close();
}
