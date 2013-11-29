#include "StdAfx.h"


#define  TP_FILECOUNT_TABLE _T("ClipFileCount")
#define  TP_FILEREFER_TABLE _T("clipfileRefer")

//////////////////////////////////////////////////////////////////////////
//CTPAccessDBInterface
CTPAccessDBInterface::CTPAccessDBInterface(void)
: m_pConnectionMdb(NULL)
, m_lDatabaseType(ET_emAccess)
{
	InitializeCriticalSection(&m_csLocalDB);
	m_lDatabase = ET_DATABASE_ID_LOCAL;
//	m_strMdbFileName = ETReadConfigParamValue(_T("Xpri_NS"),_T("LocalDBPath"), _T("S2LocalDB.mdb"));
	CString sBinPath = TP_GetBinPath() ;
	m_strMdbFileName = sBinPath + _T("\\S2LocalDB.mdb");
	OpenLocalDatabase(m_strMdbFileName);
}

CTPAccessDBInterface::~CTPAccessDBInterface(void)
{
	CloseLocalDatabase();
	DeleteCriticalSection(&m_csLocalDB);
}

BOOL CTPAccessDBInterface::OpenLocalDatabase(const CString& strFileName)
{
	if (strFileName.IsEmpty())
		return FALSE;

	HRESULT hr;
	CString strTemp,strConn;
	_bstr_t bstrConn,bstrUserID,bstrPassword;
	EnterCriticalSection(&m_csLocalDB);
	try
	{
		if(m_pConnectionMdb)
			m_pConnectionMdb.Release();
		hr = m_pConnectionMdb.CreateInstance(__uuidof(Connection));
		if (FAILED(hr))
		{
			LeaveCriticalSection(&m_csLocalDB);
			strTemp.Format(_T("初始化组件[Connection]失败!错误代码:0X%x"),hr);
//			//WriteErrorLog(strTemp, L"CTPAccessDBInterface::OpenLocalDatabase", _T(""),__LINE__);
			return FALSE;
		}
		strConn.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password= %s"),strFileName,_T("sobey"));
		bstrConn = strConn;
		bstrUserID = _T("");
		bstrPassword = _T("");
		hr =m_pConnectionMdb->Open(bstrConn,bstrUserID,bstrPassword,adModeUnknown);
		if (FAILED(hr))
		{
			if(m_pConnectionMdb)
				m_pConnectionMdb.Release();
			LeaveCriticalSection(&m_csLocalDB);
			strTemp.Format(_T("连接MDB数据库失败!连接字符串为:“%s”!错误代码:0X%x"),strConn,hr);
//			//WriteErrorLog(strTemp, L"CTPAccessDBInterface::OpenLocalDatabase", _T(""),__LINE__);
			return FALSE;
		}
	}	
	catch(_com_error e)
	{
//		//WriteErrorLog(&e, _T("CTPAccessDBInterface::OpenLocalDatabase"),_T(""),__LINE__);
		if(m_pConnectionMdb)
			m_pConnectionMdb.Release();
		LeaveCriticalSection(&m_csLocalDB);
		return FALSE;
	}
	catch (...) 
	{
		if(m_pConnectionMdb)
			m_pConnectionMdb.Release();
		LeaveCriticalSection(&m_csLocalDB);
		strTemp.Format(_T("发生未知错误！"));
//		//WriteErrorLog(strTemp, L"CTPAccessDBInterface::OpenLocalDatabase", _T(""),__LINE__);
		return FALSE;
	}
	LeaveCriticalSection(&m_csLocalDB);
	return TRUE;
}

BOOL CTPAccessDBInterface::CloseLocalDatabase()
{
	EnterCriticalSection(&m_csLocalDB);
	try
	{
		if(m_pConnectionMdb)
			m_pConnectionMdb.Release();
	}
	catch(_com_error e)
	{
		LeaveCriticalSection(&m_csLocalDB);
		//WriteErrorLog(&e, L"CTPAccessDBInterface::CloseLocalDatabase", _T(""),__LINE__);
		return FALSE;
	}
	catch (...) 
	{
		CString strTemp;
		LeaveCriticalSection(&m_csLocalDB);
		strTemp.Format(_T("发生未知错误！"));
		//WriteErrorLog(strTemp, L"CTPAccessDBInterface::CloseLocalDatabase", _T(""),__LINE__);
		return FALSE;
	}
	LeaveCriticalSection(&m_csLocalDB);
	return TRUE;
}

BOOL CTPAccessDBInterface::GetDatabaseType(long *plDatabaseType)
{
	// 本地库使用Access数据库
	*plDatabaseType = ET_emAccess;
	return TRUE;
}

BOOL CTPAccessDBInterface::GetRecordset(LPCTSTR lpszSql, _Recordset **ppRs)
{
	if (m_pConnectionMdb == NULL)
	{
		if (!OpenLocalDatabase(m_strMdbFileName))
			return FALSE;
	}

	HRESULT hr;
	VARIANT var;
	_bstr_t bstrSql;
	CString strTemp;
	_CommandPtr spAdoCmd = NULL;
	_RecordsetPtr spAdoRs = NULL;
	EnterCriticalSection(&m_csLocalDB);
	try
	{
		VariantInit(&var);
		bstrSql=lpszSql;
		hr = spAdoRs.CreateInstance(__uuidof(Recordset));
		if(FAILED(hr))
		{
			LeaveCriticalSection(&m_csLocalDB);
			return FALSE;
		}
		spAdoRs->PutCursorLocation(adUseClientBatch);
		spAdoRs->Open(bstrSql, m_pConnectionMdb.GetInterfacePtr(), adOpenStatic, adLockBatchOptimistic, -1);

		var.vt = VT_DISPATCH;
		var.pdispVal = NULL;
		spAdoRs->PutActiveConnection(var);
		*ppRs = spAdoRs.Detach();
		VariantClear(&var);
	}
	catch(_com_error e)
	{
		if(spAdoRs)		
			spAdoRs.Release();

		LeaveCriticalSection(&m_csLocalDB);
		//WriteErrorLog(&e, L"CTPAccessDBInterface::GetRecordset", _T(""),__LINE__, lpszSql);
		return FALSE;
	}
	catch(...)
	{
		LeaveCriticalSection(&m_csLocalDB);
		//WriteErrorLog(_T("出现异常失败!"), _T("CTPAccessDBInterface::GetRecordset"),_T(""),__LINE__);
		return	FALSE;
	}
	LeaveCriticalSection(&m_csLocalDB);
	return TRUE;
}

BOOL CTPAccessDBInterface::ExecuteCommand(LPCTSTR lpszSql,long &lAffectCount)
{
	if (m_pConnectionMdb == NULL)
	{
		if (!OpenLocalDatabase(m_strMdbFileName))
			return FALSE;
	}

	CString strTemp;
	_variant_t var;
	_bstr_t bstrCmd;
	EnterCriticalSection(&m_csLocalDB);
	try
	{
		bstrCmd=lpszSql;
		m_pConnectionMdb->Execute(bstrCmd, &var, adCmdText | adExecuteNoRecords);
		if(var.vt != VT_NULL)
		{
			var.ChangeType(VT_I4);
		}
		lAffectCount = abs(var.lVal);
	}
	catch(_com_error e)
	{
		LeaveCriticalSection(&m_csLocalDB);
		//WriteErrorLog(&e, L"CTPAccessDBInterface::ExecuteCommand", _T(""),__LINE__, lpszSql);
		return FALSE;
	}
	catch(...)
	{
		LeaveCriticalSection(&m_csLocalDB);
		//WriteErrorLog(_T("出现异常失败!"), _T("CTPAccessDBInterface::ExecuteCommand"),_T(""),__LINE__);
		return	FALSE;
	}
	LeaveCriticalSection(&m_csLocalDB);
	return TRUE;
}

BOOL CTPAccessDBInterface::UpdateRecordset(_Recordset *pRs)
{
	if (m_pConnectionMdb == NULL)
	{
		if (!OpenLocalDatabase(m_strMdbFileName))
			return FALSE;
	}

	HRESULT hr;
	VARIANT var;
	VariantInit(&var);
	EnterCriticalSection(&m_csLocalDB);
	try
	{
		pRs->PutActiveConnection(m_pConnectionMdb.GetInterfacePtr());
		hr=pRs->UpdateBatch(adAffectAll);
		if(FAILED(hr))
		{
			LeaveCriticalSection(&m_csLocalDB);
			return FALSE;
		}
		var.vt = VT_DISPATCH;
		var.pdispVal = NULL;
		pRs->PutActiveConnection(var);
		VariantClear(&var);
	}
	catch(_com_error e)
	{
		LeaveCriticalSection(&m_csLocalDB);
		var.vt = VT_DISPATCH;
		var.pdispVal = NULL;
		pRs->PutActiveConnection(var);
		VariantClear(&var);
		//WriteErrorLog(&e, L"CTPAccessDBInterface::UpdateRecordset", _T(""),__LINE__);
		return FALSE;
	}
	catch(...)
	{
		LeaveCriticalSection(&m_csLocalDB);
		//WriteErrorLog(_T("出现异常失败!"), _T("CTPAccessDBInterface::UpdateRecordset"),_T(""),__LINE__);
		return	FALSE;
	}
	LeaveCriticalSection(&m_csLocalDB);
	return TRUE;
}

// BOOL CTPAccessDBInterface::GetNextID(LPCTSTR lpszTableName, long *plNextID)
// {
// 	try
// 	{
// 		CString strSeqName = lpszTableName;
// 		strSeqName += _T("_SEQ");
// 		CString strSql;
// 		strSql.Format(_T("select TYPENAME, PARANAME, PARAVALUE from SMM_SYSTEMSETTING where PARANAME = '%s' and TYPENAME = 'SEQUENCE'"), strSeqName);
// 		// 从参数表获取序列号
// 		CString strParamType, strParamName, strParamValue, strParamNote;
// 		strParamType = _T("SEQUENCE");
// 		strParamName.Format(_T("%s_SEQ"), lpszTableName);
// 		strSql.Format(_T("SELECT * FROM SMM_SYSTEMSETTING WHERE TYPENAME='%s' AND PARANAME='%s'"), strParamType, strParamName);
// 		CETDBRecordset rs;
// 		long lNewValue = -1L;
// 		if ( !this->GetRecordset(strSql, &rs) )
// 		{
// 			CString strError;
// 			strError.Format(_T("获取序号：%s出错！"), strSeqName);
// 			//WriteErrorLog(strError, L"CTPAccessDBInterface::GetNextID", _T(""),__LINE__);
// 			return FALSE;
// 		}
// 		else if( !rs.IsEOF() )
// 			lNewValue = rs.FieldLong(_T("ParaValue"));
// 		else // 取得该表的关键字段的最大值
// 			lNewValue = GetTableMaxID(lpszTableName);
// 		lNewValue = lNewValue<10000 ? 10000: lNewValue+1;
// 		rs.Close();
// 
// 		// 并用最大值在参数表中添加记录
// 		strParamValue.Format(_T("%d"), lNewValue);
// 		strSql.Format(_T("SELECT * FROM SMM_SYSTEMSETTING WHERE TYPENAME='%s' AND PARANAME='%s'"),strParamType,strParamName);
// 		if (!this->GetRecordset(strSql, &rs))
// 		{
// 			return FALSE;
// 		}
// 
// 		if(!rs.IsEOF())
// 			rs.SetField(_T("ParaValue"),strParamValue);
// 		else
// 		{
// 			rs.AddNew();
// 			rs.SetField(_T("TYPENAME"),strParamType);
// 			rs.SetField(_T("PARANAME"),strParamName);
// 			rs.SetField(_T("PARAVALUE"),strParamValue);		
// 		}
// 		if (!this->UpdateRecordset(rs))
// 		{
// 			return FALSE;
// 		}
// 		*plNextID = lNewValue;
// 	}
// 	catch(_com_error &e)
// 	{
// 		//WriteErrorLog(&e, L"CTPAccessDBInterface::GetNextID", _T(""),__LINE__);
// 		return FALSE;
// 	}
// 	catch(...)
// 	{
// 		//WriteErrorLog(_T("出现异常失败!"), _T("CTPAccessDBInterface::GetNextID"),_T(""),__LINE__);
// 		return	FALSE;
// 	}
// 	return TRUE;
// }

void CTPAccessDBInterface::SetDatabaseID(long lDatabaseID)
{
	m_lDatabase = lDatabaseID;
}

// long CTPAccessDBInterface::GetTableMaxID(const CString& strTableName) // Added by lvdunlin @2007.7.26
// {
// 	long lMaxID = -1L;
// 	CString strIDFldName, strSql;
// 	CETDBRecordset rs;
// 	strIDFldName.Empty();
// 	EnterCriticalSection(&m_csLocalDB);
// 	try
// 	{
// 		// 取得第一个字段的字段名
// 		strSql.Format(_T("SELECT * FROM %s WHERE 1=0"), strTableName);
// 		if( this->GetRecordset(strSql, &rs) )
// 		{
// 			strIDFldName = rs.FieldName(0);
// 			rs.Close();
// 		}
// 
// 		using namespace AdoX_NS;
// 		// 取得主健字段的字段名
// 		_variant_t varTableName = (_bstr_t)strTableName;
// 		_CatalogPtr pCat(__uuidof(Catalog));
// 		pCat->PutActiveConnection((IUnknown*)m_pConnectionMdb);
// 		_TablePtr pTable;
// 		HRESULT hr = pCat->Tables->get_Item(varTableName, &pTable);
// 		if( !SUCCEEDED(hr) || pTable->Columns->Count<1 )
// 		{
// 			LeaveCriticalSection(&m_csLocalDB);
// 			return lMaxID;
// 		}
// 
// 		long lKeyCount = pTable->Keys->Count;
// 		for(short iKey=0; iKey<lKeyCount; iKey++)
// 		{
// 			_KeyPtr pKey = pTable->Keys->Item[iKey];
// 			if( pKey->Type==adKeyPrimary )
// 			{
// 				if( pKey->Columns->Count>1 )
// 					strIDFldName.Empty();
// 				else
// 					strIDFldName = (LPCTSTR)pKey->Columns->Item[0L]->Name;
// 			}
// 		}
// 	}
// 	catch ( _com_error & e )
// 	{
// 		//WriteErrorLog(&e, _T("CTPAccessDBInterface::GetTableMaxID"), _T(THIS_FILE), __LINE__);
// 		strIDFldName.Empty();
// 	}
// 	catch(...)
// 	{
// 		//WriteErrorLog(_T("出现异常失败!"), _T("CTPAccessDBInterface::GetTableMaxID"),_T(""),__LINE__);
// 		strIDFldName.Empty();
// 	}
// 	LeaveCriticalSection(&m_csLocalDB);
// 	if(!strIDFldName.IsEmpty())
// 	{
// 		strSql.Format(_T("SELECT MAX(%s) FROM %s"), strIDFldName, strTableName);
// 		if( this->GetRecordset(strSql, &rs) )
// 		{
// 			if( !rs.IsEOF() )
// 				lMaxID = (long)rs.FieldLong(0);
// 			else
// 				lMaxID = 1000;
// 			rs.Close();
// 		}
// 	}
// 	return lMaxID;
// }
// 
BOOL CTPAccessDBInterface::CheckDBStatus()
{
	if(m_pConnectionMdb == NULL)
		return FALSE;
	return TRUE;
}


//CTPAccessDB::CTPAccessDB()
//{
//	m_pConnection = NULL;
//
//}
//CTPAccessDB::~CTPAccessDB()
//{
//	m_pConnection = NULL;
//
//}
//CString CTPAccessDB::GetAdoError(_ConnectionPtr	pConFileCount)
//{ 
//	CString   sError = _T("");
//	if( pConFileCount == NULL)
//	{
//		ASSERT(FALSE);
//		return sError;
//	}
//
//	INT  nErrorCount = pConFileCount->GetErrors()->GetCount(); 
//	if(nErrorCount <=0 )	return sError;
//	_bstr_t   add; 
//	for(INT l = 0; l < nErrorCount; l++)
//	{ 
//		add = pConFileCount->GetErrors()->GetItem(l)->GetDescription();
//		sError += (LPTSTR)(LPCTSTR)add; 
//		sError += "\r\n "; 
//	} 
//	pConFileCount->GetErrors()-> Clear(); 
//	return   sError; 
//}
//LRESULT CTPAccessDB::OpenConnection(CString sDBPath)
//{
//	LRESULT lResult = S_FALSE;
//	if(PathFileExists(sDBPath) && m_pConnection == NULL)
//	{
//		try
//		{
//			CString strConn = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + sDBPath;
//			m_pConnection.CreateInstance("ADODB.Connection");
//			m_pConnection ->ConnectionTimeout = 15;
//			m_pConnection ->Mode = (ConnectModeEnum)adModeReadWrite;
//			lResult = m_pConnection->Open((LPCTSTR)strConn, "", "", adConnectUnspecified);
//		}
//		catch(_com_error &ce)
//		{
//#ifdef _DEBUG
//			CString sError = GetAdoError(m_pConnection);
//			AfxMessageBox(sError);
//#endif
//			TRACE((LPCTSTR)ce.Description());
//			ASSERT(FALSE);
//			m_pConnection = NULL;
//			lResult = S_FALSE;
//		}
//	}
//	return lResult;
//}
//LRESULT CTPAccessDB::CloseConnection()
//{
//	try 
//	{
//		if(m_pConnection != NULL)
//		{
//			m_pConnection ->Close();
//			m_pConnection = NULL;
//		}
//	}
//	catch(_com_error &ce)
//	{
//#ifdef _DEBUG
//		CString sError = GetAdoError(m_pConnection);
//		AfxMessageBox(sError);
//#endif
//		TRACE((LPCTSTR)ce.Description());
//		ASSERT(FALSE);
//		m_pConnection = NULL;
//	}
//	return S_OK;
//}
//inline CString TP_RevertFileName(CString &sFileName)
//{
//	CString sTemp = sFileName;
//	sTemp.Replace(_T('<'), _T('\''));
//	return sTemp;
//}
//
//LRESULT  CTPAccessDB::GetRecordsetPtr(const CString sSqlCmd, _RecordsetPtr &pRecordset)
//{
//	LRESULT lResult = S_FALSE;
//	if(NULL != m_pConnection)
//	{
//		pRecordset = NULL;
//		pRecordset.CreateInstance(__uuidof(Recordset));
//		GUID guidres = GUID_NULL;
//		CString strGuid = TP_UuidToString(&guidres);
//		try
//		{
//			lResult = pRecordset ->Open((LPCTSTR)sSqlCmd,(_variant_t)(IDispatch *)m_pConnection,adOpenKeyset,adLockReadOnly,adCmdText);
//		}
//		catch(_com_error &ce)
//		{
//			TRACE((LPCTSTR)ce.Description());
//			ASSERT(FALSE);
//		}
//	}
//	pRecordset = NULL;
//	return lResult;
//}
//LRESULT  CTPAccessDB::CloseRecordsetPtr(_RecordsetPtr &pRecordset)
//{
//	LRESULT lResult = S_FALSE;
//	if(NULL == pRecordset)	return lResult;
//
//	try
//	{
//		lResult = pRecordset ->Close();
//	}
//	catch(_com_error &ce)
//	{
//		TRACE((LPCTSTR)ce.Description());
//		ASSERT(FALSE);
//	}
//	pRecordset = NULL;
//	return lResult;
//}
//LONG CTPAccessDB::GetRecordsetCount(const CString sSqlCmd)
//{
//	LONG lRecordCount = 0;
//	if(NULL != m_pConnection)
//	{
//		_RecordsetPtr pRecordset = NULL;
//		pRecordset.CreateInstance(__uuidof(Recordset));
//		try
//		{
//			if( S_OK == pRecordset ->Open((LPCTSTR)sSqlCmd,(_variant_t)(IDispatch *)m_pConnection,adOpenKeyset,adLockReadOnly,adCmdText))
//			{
//				_variant_t stuRefCount = pRecordset->GetCollect(_variant_t((long)0));
//				if(stuRefCount.vt!=VT_NULL)  
//				{  
//					if(stuRefCount.llVal > 0) lRecordCount = stuRefCount.llVal;  
//				}   
//			}
//			else	ASSERT(0);
//			pRecordset->Close();
//
//		}
//		catch(_com_error &ce)
//		{
//			TRACE((LPCTSTR)ce.Description());
//			ASSERT(FALSE);
//		}
//	}
//	return lRecordCount;
//}
//LRESULT CTPAccessDB::ExcuteCommond(const CString sSqlCmd)
//{
//	LRESULT lResult = S_FALSE;
//	if(NULL != m_pConnection)
//	{
//		try
//		{
//			_variant_t vAff;
//			m_pConnection ->Execute((LPCTSTR)sSqlCmd, &vAff, adCmdText + adExecuteNoRecords);
//		}
//		catch(_com_error &ce)
//		{
//#ifdef _DEBUG
//			CString sError = GetAdoError(m_pConnection);
//			AfxMessageBox(sError);
//#endif
//
//			TRACE((LPCTSTR)ce.Description());
//			ASSERT(FALSE);
//		}	
//		lResult = S_OK;
//	}
//	return lResult;
//}

//LRESULT CTPAccessDB::AddRecordset(const CString sTable, CRecordsetArray aRecordset)
//{
//	LRESULT lResult = S_FALSE;
//
//	if(NULL != m_pConnection)
//	{
//		CString       sFileName  = _T("");
//		_RecordsetPtr pRecordset = NULL;
//		pRecordset.CreateInstance(__uuidof(Recordset));
//		try
//		{
//			_variant_t vAff;
//			if( S_OK == pRecordset ->Open((LPCTSTR)sTable, (_variant_t)(IDispatch *)m_pConnection, adOpenKeyset, adLockPessimistic, adCmdTable))
//			{
//				for(INT l=0;l<aRecordset.GetSize();l++)
//				{
//					sFileName = aRecordset[l];
//					pRecordset ->AddNew();		
//					pRecordset ->put_Collect((_variant_t)(short)1, (_variant_t)(LPCTSTR)strGuid);		
//					pRecordset ->put_Collect((_variant_t)(short)2, (_variant_t)(LPCTSTR)sFileName);	
//					pRecordset ->Update();
//
//				}
//				pRecordset ->Close();
//			}
//		}
//		catch(_com_error &ce)
//		{
//			TRACE((LPCTSTR)ce.Description());
//			ASSERT(FALSE);
//		}
//		pRecordset = NULL;
//	}
//	return lResult;
//}
