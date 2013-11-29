#pragma once



// #include "ETNetDefine.h"
// #include "ETNetMsgDefines.h"
// #import "c:\Program Files\Common Files\System\ado\msado15.dll"\
// 	rename_namespace("AdoNS")\
// 	rename("EOF","adoEOF")
// using namespace AdoNS;
// ET_BASICLOGIC_BEGIN
//rename_namespace ("ADOCG")\ //rename ("EOF","EndOfFile") 
#import "msado15.dll"\
	rename_namespace("AdoNS")\
	rename("EOF","adoEOF")

using namespace AdoNS;

#import "msxml4.dll" 
using namespace MSXML2;


//网络E7非编需要的锁
enum em_ETLockType
{
	ET_emObject						=	4000,		//object锁定
	ET_emObjEdit					=	4010,		//obj编辑锁定
};

//!数据库类型定义
enum em_ETDatabaseType
{
	ET_emSQLSERVER = 1,
	ET_emORACLE = 2,
	ET_emSYBASE = 3,
	ET_emDB2 = 4,
	ET_emAccess = 5
};

//站点类型定义
typedef long ETVML_PLUGIN_TYPE;
#define ETVML_PLUGIN_TYPE_NETWORK			0X01             //网络(中心存储)站点
#define ETVML_PLUGIN_TYPE_LOCAL				0X02             //本地库
#define ETVML_PLUGIN_TYPE_REMOTE			0X04             //远程站点
#define ETVML_PLUGIN_TYPE_ARCHIVE			0X08             //媒资站点
#define ETVML_PLUGIN_TYPE_REMOTEARCHIVE		0X10             //远程媒资站点
#define ETVML_PLUGIN_TYPE_VSEARCHLIST		0X20             //虚拟的公共的SearchResult


//数据库的站点ID
//0：为主数据库或者缺省数据库，1：为本地数据库的站点ID，其他：为指定的数据库的站点ID
#define ET_DATABASE_ID_DEFAULT	0
#define ET_DATABASE_ID_LOCAL	1


//AFX_EXT_API BOOL	TP_InitDBSystem();
//数据库相关函数
AFX_EXT_API long	TP_GetDatabaseType(long lDatabase = 0);
AFX_EXT_API BOOL	TP_GetRecordset(LPCTSTR lpszSql, _Recordset **ppRs, long lDatabase = 0);
AFX_EXT_API BOOL	TP_UpdateRecordset(_Recordset *pRs, long lDatabase = 0);
AFX_EXT_API BOOL	TP_ExecuteCommand(LPCTSTR lpszSql,long &lAffectCount, long lDatabase = 0);


//关于数据锁定的函数系列
//取数据库表中的最大ID,输入参数是表名
AFX_EXT_API DWORD	ET_GetDBTableNextID(const CString & strTableName, long lDatabase = 0);
//!清除过期的锁定信息
AFX_EXT_API BOOL	ET_ClearLockData(long lDatabase = 0);
//!锁定信息
AFX_EXT_API BOOL	ET_LockDataItem(const CString & strItemID,em_ETLockType emLockType,DWORD dwLoginID,DWORD dwUserID, long lDatabase = 0);
//读取锁定信息
AFX_EXT_API BOOL	ET_GetLockInfo(const CString & strItemID, long lLockType, long& lLockID, CString& strUserName, CString& strIPAddress, BOOL& bIsCurProcess, long lDatabase = 0);
//!解除锁定信息
AFX_EXT_API BOOL	ET_UnLockDataItem(const CString & strItemID,em_ETLockType emLockType,DWORD dwLoginID,long lDatabase = 0);


// 做Xml, html中节点的数据转义
AFX_EXT_API CString XmlConv(CString strInput);
AFX_EXT_API CString HtmlConv(const CString& strInput);

AFX_EXT_API void GetSpeicalCountFromPos(const CString& strSearchString,int nPos,int& nCount,TCHAR cFind = '/');
AFX_EXT_API void DeleteInValidChar(CString& strSearchString);
AFX_EXT_API void DeleteRepeatChar(CString& strSearchString,int nPosEnd,int nCount);
AFX_EXT_API BOOL ET_IsPal(ULONGLONG ullVideoStandard);

//BMP文件的缩放
AFX_EXT_API BOOL ET_ZoomBitmap(CBitmap& bmpSrc, CBitmap& bmpDes, CSize szDes, COLORREF colorBK);


