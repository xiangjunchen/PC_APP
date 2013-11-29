#pragma once


#define  CRecordsetArray CStringArray



 class CTPDBInterface
 {
 public:
	 CTPDBInterface(void){};
 	virtual ~CTPDBInterface(void) {};
 
 	virtual BOOL GetDatabaseType(long *plDatabaseType) = 0;
 	virtual BOOL GetRecordset(LPCTSTR lpszSql, _Recordset **ppRs) = 0;
 	virtual BOOL ExecuteCommand(LPCTSTR lpszSql,long &lAffectCount) = 0;
 	virtual BOOL UpdateRecordset(_Recordset *pRs) = 0;
 	virtual BOOL LockItemData(const CString & strItemID, em_ETLockType emLockType, long lLoginID, long lUserID) = 0;
 	virtual BOOL GetNextID(LPCTSTR lpszTableName, long *plNextID) = 0;
 	virtual void SetDatabaseID(long lDatabaseID) = 0;
 	virtual long GetTableMaxID(const CString& strTableName) = 0;
 	virtual BOOL CheckDBStatus() = 0;
 };

 class CTPAccessDBInterface : public CTPDBInterface
{
public:
	CTPAccessDBInterface(void);
	virtual ~CTPAccessDBInterface(void);

	BOOL GetDatabaseType(long *plDatabaseType);
	BOOL GetRecordset(LPCTSTR lpszSql, _Recordset **ppRs);
	BOOL ExecuteCommand(LPCTSTR lpszSql,long &lAffectCount);
	BOOL UpdateRecordset(_Recordset *pRs);
	BOOL LockItemData(const CString & strItemID, em_ETLockType emLockType, long lLoginID, long lUserID) { return TRUE;}
	BOOL GetNextID(LPCTSTR lpszTableName, long *plNextID) {return 0;};
	void SetDatabaseID(long lDatabaseID);
	long GetTableMaxID(const CString& strTableName) {return 0;}; // Added by lvdunlin @2007.7.26
	BOOL CheckDBStatus();
protected:
	BOOL OpenLocalDatabase(const CString& strFileName);
	BOOL CloseLocalDatabase();

protected:
	_ConnectionPtr	m_pConnectionMdb;
	long			m_lDatabase;
	long			m_lDatabaseType;
	CString			m_strMdbFileName;
	CRITICAL_SECTION	m_csLocalDB;
};

//class AFX_EXT_CLASS CTPAccessDB : public CTPDBInterface
//{
//public:
//	CTPAccessDB();
//	~CTPAccessDB();
//
//	LRESULT OpenConnection(CString sDBPath);
//	LRESULT CloseConnection();
//
//	LRESULT GetRecordsetPtr(const CString sSqlCmd, _RecordsetPtr &pRecordset);
//	LRESULT CloseRecordsetPtr(_RecordsetPtr &pRecordset);
//
//	LONG    GetRecordsetCount(const CString sSqlCmd);
//
//	LRESULT ExcuteCommond(const CString sSqlCmd);
//
//	//LRESULT AddRecordset(const CString sTable, CRecordsetArray aRecordset);
//
//private:
//	_ConnectionPtr		   m_pConnection ;
//
//	CString   GetAdoError(_ConnectionPtr	pConFileCount);
//
//protected:
//};
