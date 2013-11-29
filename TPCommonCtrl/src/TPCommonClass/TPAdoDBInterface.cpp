#include "StdAfx.h"


#define  TP_FILECOUNT_TABLE _T("ClipFileCount")
#define  TP_FILEREFER_TABLE _T("clipfileRefer")

CTPAdoDB::CTPAdoDB()
{
	m_pConnection = NULL;

}
CTPAdoDB::~CTPAdoDB()
{
	m_pConnection = NULL;

}
CString CTPAdoDB::GetAdoError(_ConnectionPtr	pConFileCount)
{ 
	CString   sError = _T("");
	if( pConFileCount == NULL)
	{
		ASSERT(FALSE);
		return sError;
	}

	INT  nErrorCount = pConFileCount->GetErrors()->GetCount(); 
	if(nErrorCount <=0 )	return sError;
	_bstr_t   add; 
	for(INT l = 0; l < nErrorCount; l++)
	{ 
		add = pConFileCount->GetErrors()->GetItem(l)->GetDescription();
		sError += (LPTSTR)(LPCTSTR)add; 
		sError += "\r\n "; 
	} 
	pConFileCount->GetErrors()-> Clear(); 
	return   sError; 
}
LRESULT CTPAdoDB::OpenConnection(CString sDBPath)
{
	LRESULT lResult = S_FALSE;
	if(m_pConnection == NULL)
	{
		try
		{
			CString strConn = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + sDBPath;
			m_pConnection.CreateInstance("ADODB.Connection");
			m_pConnection ->ConnectionTimeout = 15;
			m_pConnection ->Mode = (ConnectModeEnum)adModeReadWrite;
			lResult = m_pConnection->Open((LPCTSTR)strConn, "", "", adConnectUnspecified);
		}
		catch(_com_error &ce)
		{
#ifdef _DEBUG
			CString sError = GetAdoError(m_pConnection);
			AfxMessageBox(sError);
#endif
			TRACE((LPCTSTR)ce.Description());
			ASSERT(FALSE);
			m_pConnection = NULL;
			lResult = S_FALSE;
		}
	}
	return lResult;
}
LRESULT CTPAdoDB::CloseConnection()
{
	try //2006-08-14
	{
		if(m_pConnection != NULL)
		{
			m_pConnection ->Close();
			m_pConnection = NULL;
		}
	}
	catch(_com_error &ce)
	{
#ifdef _DEBUG
		CString sError = GetAdoError(m_pConnection);
		AfxMessageBox(sError);
#endif
		TRACE((LPCTSTR)ce.Description());
		ASSERT(FALSE);
		m_pConnection = NULL;
	}
	return S_OK;
}
inline CString TP_RevertFileName(CString &sFileName)
{
	CString sTemp = sFileName;
	sTemp.Replace(_T('<'), _T('\''));
	return sTemp;
}

LRESULT  CTPAdoDB::GetItemDBData()
{
	LRESULT lResult = S_FALSE;
	if( m_pConnection == NULL)		return lResult; 

	CStringArray aFileName ;
	_RecordsetPtr pRecordset = NULL;
	pRecordset.CreateInstance(__uuidof(Recordset));
	GUID guidres = GUID_NULL;
	CString strGuid = TP_UuidToString(&guidres);
	CString strSql = _T(" select distinct MEDIAFILE from ") + (CString)TP_FILECOUNT_TABLE + _T(" where GUID = '") + strGuid + _T("'");

	try{
		if( S_OK == pRecordset ->Open((LPCTSTR)strSql,(_variant_t)(IDispatch *)m_pConnection,adOpenKeyset,adLockReadOnly,adCmdText))
		{
			CString      sTemp;
			VARIANT_BOOL vbEOF;

			pRecordset ->get_EndOfFile(&vbEOF);

			while ( vbEOF != VARIANT_TRUE)
			{
				sTemp = (CString)pRecordset ->GetCollect(_variant_t((short)0)).bstrVal;//  在数据库中通过GUID=guidRes来寻找源文件的地址
				// ********************** XN00015546 LI HUI LI 2008-09-05 ******************/
				sTemp = TP_RevertFileName(sTemp);
				//**************************************************************************
				aFileName.Add( sTemp);

				pRecordset ->MoveNext();
				pRecordset ->get_EndOfFile(&vbEOF);
			}

			pRecordset ->Close();
		}
	}
	catch(_com_error &ce)
	{
		TRACE((LPCTSTR)ce.Description());
		ASSERT(FALSE);
	}

	pRecordset = NULL;
	return lResult;
}
BOOL CTPAdoDB::IsItemExist()
{
	_RecordsetPtr pRecordset = NULL;
	pRecordset.CreateInstance(__uuidof(Recordset));

	BOOL    bDelFile   = FALSE;	
	CString strSql = _T("");
	CString sFileName = _T("");

	GUID guidRes = GUID_NULL;
	try
	{
		strSql.Format(_T("select count(*) from %s where MEDIAFILE = '%s' and  GUID <> '%s' and GUID <> '%s'"), TP_FILECOUNT_TABLE, sFileName, TP_GuidToString(&guidRes), TP_GuidToString(&guidRes));

		if( S_OK == pRecordset ->Open((LPCTSTR)strSql,(_variant_t)(IDispatch *)m_pConnection,adOpenKeyset,adLockReadOnly,adCmdText))
		{
			long lCount = (long)pRecordset ->GetCollect(_variant_t((short)0));
			bDelFile = ( lCount == 0);
			pRecordset ->Close();
		}
	}
	catch(_com_error &ce)
	{
		TRACE((LPCTSTR)ce.Description());
		ASSERT(FALSE);
	}

	pRecordset = NULL;
	return TRUE;
}
CTPAdoDBInterface::CTPAdoDBInterface(void)
{
}

CTPAdoDBInterface::~CTPAdoDBInterface(void)
{
}
