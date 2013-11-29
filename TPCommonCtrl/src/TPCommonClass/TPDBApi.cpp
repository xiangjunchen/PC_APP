#include "StdAfx.h"
#include "TPDBApi.h"

long TP_GetDatabaseType(long lDatabase /* = 0 */)
{
	try
	{
		long lDatabaseType;
		CTPDBInterface* pDBInterface = CTPDBManage::instance().GetDBInterface(lDatabase);
		if (pDBInterface != NULL)
		{
			if (!pDBInterface->GetDatabaseType(&lDatabaseType))
			{
				return ET_emORACLE;
			}

			return lDatabaseType;
		}
		else
		{
			return ET_emORACLE;
		}
	}
	catch (_com_error &e)
	{
		CString strTemp;
		strTemp.Format(_T("ExecuteCommand throw COM exception:%s"),(LPCSTR)e.Description());
		//WriteDebugLog(strTemp,__FILE__,__LINE__);
		return ET_emORACLE;
	}
	catch(...)
	{
		//WriteDebugLog(_T("ExecuteCommand throw exception!"),__FILE__,__LINE__);
		return ET_emORACLE;
	}
	//	return ET_emORACLE;
}
BOOL	TP_GetRecordset(LPCTSTR lpszSql, _Recordset **ppRs, long lDatabase)
{
	try
	{
		CTPDBInterface* pDBInterface = CTPDBManage::instance().GetDBInterface(lDatabase);
		if (pDBInterface != NULL)
		{
			if (!pDBInterface->GetRecordset(lpszSql, ppRs))
			{
				//ET_TraceEx2(_T("ET_ML"), _T("GetRecordset"), ET_logLevelError, 20000001, L"%s", lpszSql);
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error &e)
	{
		//ET_TraceEx2(_T("ET_ML"), _T("GetRecordset"), ET_logLevelError, 20000001, _T("GetRecordset happen com exception:%s"),(LPCSTR)e.Description());
		CString strTemp;
		strTemp.Format(_T("GetRecordset throw COM exception:%s"),(LPCSTR)e.Description());
		//WriteDebugLog(strTemp,__FILE__,__LINE__);
		return FALSE;
	}
	catch(...)
	{
		//ET_TraceEx2(_T("ET_ML"), _T("GetRecordset"), ET_logLevelError, 20000001, _T("GetRecordset happen exception"));
		//WriteDebugLog(_T("GetRecordset throw exception!"),__FILE__,__LINE__);
		return FALSE;
	}
	return FALSE;
}
BOOL TP_UpdateRecordset(_Recordset *pRs, long lDatabase /* = 0 */)
{
	try
	{
		CTPDBInterface* pDBInterface = CTPDBManage::instance().GetDBInterface(lDatabase);
		if (pDBInterface != NULL)
		{
			if (!pDBInterface->UpdateRecordset(pRs))
			{
				//ET_TraceEx2(_T("ET_ML"), _T("UpdateRecordset"), ET_logLevelError, 20000002, _T(""));
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error &e)
	{
		//ET_TraceEx2(_T("ET_ML"), _T("UpdateRecordset"), ET_logLevelError, 20000002, _T("UpdateRecordset happen com exception:%s"),(LPCSTR)e.Description());
		CString strTemp;
		strTemp.Format(_T("UpdateRecordset throw COM exception:%s"),(LPCSTR)e.Description());
		//WriteDebugLog(strTemp,__FILE__,__LINE__);
		return FALSE;
	}
	catch(...)
	{
		//ET_TraceEx2(_T("ET_ML"), _T("UpdateRecordset"), ET_logLevelError, 20000002, _T("UpdateRecordset happen exception"));
		//WriteDebugLog(_T("UpdateRecordset throw exception!"),__FILE__,__LINE__);
		return FALSE;
	}
	return FALSE;
}

BOOL ExecuteCommand(LPCTSTR lpszSql,long &lAffectCount, long lDatabase /* = 0 */)
{
	BOOL bDel = FALSE;
	try
	{
		CString strSql = lpszSql;
		if(strSql.GetLength() >= 6 && strSql.Left(6).CompareNoCase(_T("Delete")) == 0)
			bDel = TRUE;
		CTPDBInterface* pDBInterface = CTPDBManage::instance().GetDBInterface(lDatabase);
		if (pDBInterface != NULL)
		{
			if (!pDBInterface->ExecuteCommand(lpszSql, lAffectCount))
			{
				//if(bDel)
				//	ET_TraceEx2(_T("ET_ML"), _T("ExecuteCommand"), ET_logLevelError, 20000003, L"%s", lpszSql);
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error &e)
	{
		//if(bDel)
		//	ET_TraceEx2(_T("ET_ML"), _T("ExecuteCommand"), ET_logLevelError, 20000003, _T("ExecuteCommand happen com exception:%s"),(LPCSTR)e.Description());
		CString strTemp;
		strTemp.Format(_T("ExecuteCommand throw COM exception:%s"),(LPCSTR)e.Description());
		//WriteDebugLog(strTemp,__FILE__,__LINE__);
		return FALSE;
	}
	catch(...)
	{
		//if(bDel)
		//	ET_TraceEx2(_T("ET_ML"), _T("ExecuteCommand"), ET_logLevelError, 20000003, _T("ExecuteCommand happen exception"));
		//WriteDebugLog(_T("ExecuteCommand throw exception!"),__FILE__,__LINE__);
		return FALSE;
	}
	return FALSE;
}
