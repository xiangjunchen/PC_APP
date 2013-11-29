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


//����E7�Ǳ���Ҫ����
enum em_ETLockType
{
	ET_emObject						=	4000,		//object����
	ET_emObjEdit					=	4010,		//obj�༭����
};

//!���ݿ����Ͷ���
enum em_ETDatabaseType
{
	ET_emSQLSERVER = 1,
	ET_emORACLE = 2,
	ET_emSYBASE = 3,
	ET_emDB2 = 4,
	ET_emAccess = 5
};

//վ�����Ͷ���
typedef long ETVML_PLUGIN_TYPE;
#define ETVML_PLUGIN_TYPE_NETWORK			0X01             //����(���Ĵ洢)վ��
#define ETVML_PLUGIN_TYPE_LOCAL				0X02             //���ؿ�
#define ETVML_PLUGIN_TYPE_REMOTE			0X04             //Զ��վ��
#define ETVML_PLUGIN_TYPE_ARCHIVE			0X08             //ý��վ��
#define ETVML_PLUGIN_TYPE_REMOTEARCHIVE		0X10             //Զ��ý��վ��
#define ETVML_PLUGIN_TYPE_VSEARCHLIST		0X20             //����Ĺ�����SearchResult


//���ݿ��վ��ID
//0��Ϊ�����ݿ����ȱʡ���ݿ⣬1��Ϊ�������ݿ��վ��ID��������Ϊָ�������ݿ��վ��ID
#define ET_DATABASE_ID_DEFAULT	0
#define ET_DATABASE_ID_LOCAL	1


//AFX_EXT_API BOOL	TP_InitDBSystem();
//���ݿ���غ���
AFX_EXT_API long	TP_GetDatabaseType(long lDatabase = 0);
AFX_EXT_API BOOL	TP_GetRecordset(LPCTSTR lpszSql, _Recordset **ppRs, long lDatabase = 0);
AFX_EXT_API BOOL	TP_UpdateRecordset(_Recordset *pRs, long lDatabase = 0);
AFX_EXT_API BOOL	TP_ExecuteCommand(LPCTSTR lpszSql,long &lAffectCount, long lDatabase = 0);


//�������������ĺ���ϵ��
//ȡ���ݿ���е����ID,��������Ǳ���
AFX_EXT_API DWORD	ET_GetDBTableNextID(const CString & strTableName, long lDatabase = 0);
//!������ڵ�������Ϣ
AFX_EXT_API BOOL	ET_ClearLockData(long lDatabase = 0);
//!������Ϣ
AFX_EXT_API BOOL	ET_LockDataItem(const CString & strItemID,em_ETLockType emLockType,DWORD dwLoginID,DWORD dwUserID, long lDatabase = 0);
//��ȡ������Ϣ
AFX_EXT_API BOOL	ET_GetLockInfo(const CString & strItemID, long lLockType, long& lLockID, CString& strUserName, CString& strIPAddress, BOOL& bIsCurProcess, long lDatabase = 0);
//!���������Ϣ
AFX_EXT_API BOOL	ET_UnLockDataItem(const CString & strItemID,em_ETLockType emLockType,DWORD dwLoginID,long lDatabase = 0);


// ��Xml, html�нڵ������ת��
AFX_EXT_API CString XmlConv(CString strInput);
AFX_EXT_API CString HtmlConv(const CString& strInput);

AFX_EXT_API void GetSpeicalCountFromPos(const CString& strSearchString,int nPos,int& nCount,TCHAR cFind = '/');
AFX_EXT_API void DeleteInValidChar(CString& strSearchString);
AFX_EXT_API void DeleteRepeatChar(CString& strSearchString,int nPosEnd,int nCount);
AFX_EXT_API BOOL ET_IsPal(ULONGLONG ullVideoStandard);

//BMP�ļ�������
AFX_EXT_API BOOL ET_ZoomBitmap(CBitmap& bmpSrc, CBitmap& bmpDes, CSize szDes, COLORREF colorBK);


