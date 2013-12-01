#include "StdAfx.h"

CTPArticleDataBase::CTPArticleDataBase(void)
{
}

CTPArticleDataBase::~CTPArticleDataBase(void)
{
}
LRESULT CTPArticleDataBase::ReadChannel(GUID guidRes,TPChannelData &stuChannelData)
{
	CString  sFileName = _T("\\test.Chl");
	CTPMemFile hMemFile;
	hMemFile.ReadFile(sFileName);
	DWORD dwTemp = 0;
	//
	hMemFile.Read(&dwTemp,sizeof(DWORD));	

	//
	hMemFile.Close();
	return S_OK;
}
LRESULT CTPArticleDataBase::WriteChannel(GUID guidRes,TPChannelData &stuChannelData)
{
	CString  sFileName = _T("\\test.Chl");
	CTPMemFile hMemFile;
	DWORD dwTemp = 8;
	//
	hMemFile.Write(&dwTemp,sizeof(DWORD));
	//
	hMemFile.WriteFile(sFileName);
	hMemFile.Close();
	return S_OK;
}
