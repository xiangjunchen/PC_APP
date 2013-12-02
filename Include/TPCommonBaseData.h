#pragma	  once

#include "shlwapi.h"
#include <afxmt.h>
#include <math.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Rpcrt4.lib")


#define  TP_NS_DBE
#define  TP_NS_LOG
#define  TP_NS_GOPOPMODE      1

#include <Afxtempl.h>
 
#ifdef   TP_NS_NET
#define  TP_NS_EDIT
#endif

#define  TP_CREATEMP4

#define  TP_RESICON_WIDTH     75
#define  TP_RESICON_HIGH      60

#define  TP_RESICON_WIDTH_HD 160
#define  TP_RESICON_HIGH_HD   90


#define TP_EFFICON_WIDTH  75
#define TP_EFFICON_HIGH   60

#define MB_POSTMESSAGE    0x04000000L

#define MAX_INT64  0x7fffffffffffffff

typedef CArray<CSize , CSize &>       CTPSizeArray;
typedef CArray<int,    int    >       CTPIntArray;
typedef CArray<void *,void *&>        CTPWndList;
typedef CArray<CRect ,CRect &>        CTPRectArray;
typedef CArray<CWnd *,CWnd *&>        CWndArray;
typedef CArray<LPTSTR  ,LPTSTR&>      CStrArray;
typedef CArray<GUID,GUID &>           CGuidArray;
typedef CArray<ULONGLONG,ULONGLONG >  CULongLongArray;
typedef CArray<double     ,double&>   CDoubleArray;
typedef CArray<CPoint,CPoint&>        CPointArray;
typedef CArray<DWORD,DWORD&>          CDWORDArray;

#undef  CInt64Array
#define CInt64Array CArray<__int64,__int64 >


#undef  _L
#define _L _T

#define  TPFILEHEAD_FILENOTE 10
typedef struct stu_TPFileHead  //文件头部
{
	DWORD     dwVersion;    //版本号
	DWORD     dwFileType;   //文件类型
	TCHAR     sFileNote[TPFILEHEAD_FILENOTE]; //文件说明
	DWORD     dwFileSize;   //文件大小
	DWORD     dwCount;      //文件大小
	DWORD     dwCountVer;   //文件大小
	DWORD     dwRes[18];
	stu_TPFileHead()
	{
		dwVersion    = 0;
		dwFileType   = 0;
		sFileNote[0] = '\0';
		dwFileSize   = 0;
		dwCount      = 0;
		dwCountVer   = 1;
		ZeroMemory(dwRes,sizeof(dwRes));
	}
}TPFileHead;

typedef struct stu_TPFileNode  //文件节点
{
	DWORD     dwFlag;         //节点起始标志
	DWORD     dwVersion;      //节点版本
	DWORD     dwDataType;     //节点数据类型
	DWORD     dwBeginPos;     //文件起始位置
	DWORD     dwEndPos;       //文件结束位置
	long      lNumber;        //数目
	long      lNumberV;       //数目校验
	//DWORD     dwUserData;   //用户自定义数据
	stu_TPFileNode()
	{
		dwFlag        = 0xFEFE;
		dwVersion     = 0;
		dwDataType    = 0;
		dwBeginPos    = 0;
		dwEndPos      = 0;
		lNumber       = 0;
		lNumberV      = 0;
		//dwUserData    = 0;
	}
}TPFileNode;

typedef CArray<TPFileNode,TPFileNode&> TPFileNodeArray;
typedef struct stu_TPMarkPoint  //全局参数
{
	DWORD     dwLevel;
	__int64   dwFrame;
	CString   sNote;
	BYTE      cType;
	LPARAM    pParam;
	DWORD     cListType;
	stu_TPMarkPoint()
	{
		dwLevel  = 2;
		dwFrame  = 0;
		cType    = 0;
		sNote    = _L("");
		pParam   = NULL;
		cListType = 0;
	}
	stu_TPMarkPoint(int dwFrame,LPTSTR sNote)
	{
		this->dwLevel   = 2;
		this->dwFrame   = dwFrame;
		this->cType     = 0;
		this->sNote     = sNote;
		this->pParam    = NULL;
		this->cListType	= 0;
	}
	stu_TPMarkPoint(__int64 dwFrame,LPTSTR sNote)
	{
		this->dwLevel   = 2;
		this->dwFrame   = dwFrame;
		this->cType     = 0;
		this->sNote     = sNote;
		this->pParam    = NULL;
		this->cListType	= 0;
	}
	DWORD GetSize()
	{
		return sizeof(DWORD) *3 + ( sNote.GetLength() +1) * sizeof(TCHAR) + sizeof(__int64) + sizeof(LPARAM) + sizeof(BYTE);
	}
	void operator=(const stu_TPMarkPoint& srcMark)
	{
		dwLevel = srcMark.dwLevel;
		cType    = srcMark.cType;
		dwFrame = srcMark.dwFrame;
		sNote   = srcMark.sNote;
		pParam  = srcMark.pParam;
		cListType=srcMark.cListType;
	}
	BOOL operator==(const stu_TPMarkPoint& srcMark)
	{
		return dwFrame==srcMark.dwFrame && pParam==srcMark.pParam && sNote==srcMark.sNote;
	}
	BOOL operator!=(const stu_TPMarkPoint& srcMark)
	{
		return !operator==(srcMark);
	}
	DWORD Save(BYTE *pBuffer)
	{
		*((DWORD *)pBuffer)     = 2/*dwLevel*/;     pBuffer += sizeof(DWORD);
		*((__int64 *)pBuffer)   = dwFrame;          pBuffer += sizeof(__int64);
		*((DWORD *)pBuffer)     = sNote.GetLength();pBuffer += sizeof(DWORD);
		lstrcpyn((TCHAR *)pBuffer,(LPCTSTR)sNote,sNote.GetLength()+1);
		*((TCHAR *)(pBuffer + (sNote.GetLength())* sizeof(TCHAR))) = 0;
		pBuffer += (sNote.GetLength()+1)* sizeof(TCHAR);
		*((LPARAM *)pBuffer)     = pParam;		    pBuffer += sizeof(LPARAM);
		*((BYTE *)pBuffer)       = cType;		    pBuffer += sizeof(BYTE);
		*((DWORD*)pBuffer)       = 2/*cListType*/;  pBuffer += sizeof(DWORD);
		return GetSize();
	}
	DWORD Read(BYTE *pBuffer)
	{
		BYTE *pBufferBase = pBuffer;
		dwLevel = *((DWORD *)pBuffer); pBuffer += sizeof(DWORD);
#ifdef TP_NS_NET //系统非编4.5.1把颜色值存放在dwLevel，所以在4.6做一些对4.5.1的兼容工作
		switch (dwLevel)
		{
		case RGB(161, 192, 223):
			dwLevel	= 1;
			cType	= 1;//LIGHT_BLUE
			break;
		case RGB(237,  80,  80):
			dwLevel	= 1;
			cType	= 2;//Red
			break;
		case RGB(170, 223,  80):
			dwLevel	= 1;
			cType	= 3;//PEAK_GREEN
			break;
		case RGB(182, 140, 255):
			dwLevel	= 1;
			cType	= 4;//LAVENDER
			break;
		case RGB(242, 238, 129):
			dwLevel	= 1;
			cType	= 5;//LIGHT_YELLOW
			break;
		case RGB(0  , 255,   0):
			dwLevel	= 1;
			cType	= 0;//ORANGE
			break;
		case RGB(77  , 77,  77):
			dwLevel	= 1;
			cType	= 0;//ORANGE
			break;
		case RGB(89 , 167, 238):
			dwLevel	= 1;
			cType	= 0;//ORANGE
			break;
		default:
			cType	= 0;//ORANGE
			break;
		}
#endif
		dwFrame       = *((__int64 *)pBuffer);    pBuffer += sizeof(__int64);
		DWORD sLength = *((DWORD *)pBuffer);	  pBuffer += sizeof(DWORD);
		sNote = (TCHAR *)pBuffer;
		sNote = sNote.Left(sLength);
		pBuffer += sNote.GetLength()* sizeof(TCHAR);
		if(dwLevel>1) pBuffer += sizeof(TCHAR);//从2开始多加了一个结束符 20101126
		
		if(dwLevel>1)
		{
			pParam = *((LPARAM *)pBuffer); pBuffer += sizeof(LPARAM);
		}
		else //if(dwLevel<=1)
		{
			if(dwLevel >0) {pParam = *((LPARAM *)pBuffer); pBuffer += sizeof(LPARAM);}
			pBuffer += sizeof(TCHAR);//2之前的有一个空TCHAR
		}

		if(dwLevel>1)
		{
			cType        = *((BYTE *)pBuffer);   pBuffer += sizeof(BYTE);
			DWORD dwTemp = *((DWORD *)pBuffer);  pBuffer += sizeof(DWORD);//cListType
		}

		return (DWORD)(pBuffer - pBufferBase);

	}
}TPMarkPoint,TPMarkTemp;
typedef CArray<TPMarkPoint,TPMarkPoint &> TPMarkPointArray;

inline CString TP_IntToString(__int64 iInit)
{
	CString sText;
	sText.Format(_T("%ld"),iInit);
	return sText;
}

inline  BOOL TP_EqualMark(TPMarkPointArray &src,TPMarkPointArray &dest)
{
	if(src.GetSize() != dest.GetSize()) return FALSE;
	for(INT l =(INT)src.GetSize()-1;l>=0;l--)
	{
		if(src[l].dwFrame != dest[l].dwFrame) return FALSE;
		if(src[l].sNote   != dest[l].sNote)   return FALSE;
	}
	return TRUE;
}
inline DWORD TP_GetVertify(LPBYTE pBuffer,long lSize)
{
	DWORD    dwVerftify = 0;
	LPDWORD  pDword = (LPDWORD)pBuffer;
	for(INT l=0;l<lSize/4;l++)
		dwVerftify ^= pDword[l];
	return dwVerftify;
}
inline BOOL TP_IsDoubleEqual(double dA, double dB, INT iScale = 6)
{
	return fabs(dA-dB)<=(fabs(dA) + fabs(dB))/2*pow(10.0,-iScale);
}

typedef struct stu_TPSceneInout  //全局参数
{
	DWORD     dwLevel;
	__int64   lIn;
	__int64   lOut;
	CString   sNote;
	BYTE      cType;
	LPARAM    pParam;
	stu_TPSceneInout(__int64 lInSet = -1,__int64 lOutSet = -1)
	{
		dwLevel  = 0;
		lIn      = 0;
		lOut     = 0;
		cType    = 0;
		sNote    = _L("");
		pParam   = NULL;
	}
	DWORD GetSize()
	{
		return sizeof(DWORD) *3 + ( sNote.GetLength() +1) * sizeof(TCHAR) + sizeof(__int64)*2+ sizeof(LPARAM);
	}
	void operator=(const stu_TPSceneInout& srcMark)
	{
		dwLevel  = srcMark.dwLevel;
		cType    = srcMark.cType;
		lIn      = srcMark.lIn;
		lOut     = srcMark.lOut;
		sNote    = srcMark.sNote;
		pParam   = srcMark.pParam;
	}
	DWORD Save(BYTE *pBuffer)
	{
		*((DWORD *)pBuffer)     = 0;	        pBuffer += sizeof(DWORD);//Version
		*((DWORD *)pBuffer)     = dwLevel;	    pBuffer += sizeof(DWORD);
		*((__int64 *)pBuffer)   = lIn;		    pBuffer += sizeof(__int64);
		*((__int64 *)pBuffer)   = lOut;		    pBuffer += sizeof(__int64);
		*((DWORD *)pBuffer)     = sNote.GetLength();pBuffer += sizeof(DWORD);
		lstrcpyn((TCHAR *)pBuffer,(LPCTSTR)sNote,sNote.GetLength()+1);
		*((TCHAR *)(pBuffer + (sNote.GetLength())* sizeof(TCHAR))) = 0;
		pBuffer += (sNote.GetLength())* sizeof(TCHAR);
		*((LPARAM *)pBuffer)    = pParam;       pBuffer += sizeof(LPARAM);
		return GetSize();
	}
	DWORD Read(BYTE *pBuffer)
	{
		DWORD dwVer= *((DWORD *)pBuffer);      pBuffer += sizeof(DWORD);
		dwLevel    = *((DWORD *)pBuffer);      pBuffer += sizeof(DWORD);
		lIn        = *((__int64 *)pBuffer);    pBuffer += sizeof(__int64);
		lOut       = *((__int64 *)pBuffer);    pBuffer += sizeof(__int64);
		DWORD sLength = *((DWORD *)pBuffer);   pBuffer += sizeof(DWORD);
		sNote = (TCHAR *)pBuffer;
		sNote = sNote.Left(sLength);
		pBuffer += (sNote.GetLength())* sizeof(TCHAR);
		if(dwLevel >0)	pParam = *((LPARAM *)pBuffer);
		return GetSize();
	}
}TPSceneInout;
typedef CArray<TPSceneInout,TPSceneInout &> TPSceneInoutArray;

enum MarkPointInfoCmd
{
	 MarkPointInfo_None,
	 MarkPointInfo_Create,
	 MarkPointInfo_Destroy,
	 MarkPointInfo_Refresh,
	 MarkPointInfo_DelMark,
	 MarkPointInfo_DelAll,
	 MarkPointInfo_Comment,
	 MarkPointInfo_GetSize,
	 MarkPointInfo_SysMenu,
	 MarkPointInfo_StopThread,
	 MarkPointInfo_SetActivate,
	 MarkPointInfo_SetTypeEnable,
};
typedef struct stu_MarkPointInfo 
{
	MarkPointInfoCmd  dwCmd;//1 显示 2 关闭  3 刷新 4 删除部分  5 删除全部  6 Comment 7 GetSize 8 SysMenu
	WPARAM            wParam;
	LPARAM            lParam;
	CWnd             *pWnd;
	TPMarkPointArray *pMarkPoint;
	stu_MarkPointInfo()
	{
		dwCmd      = MarkPointInfo_None;
		wParam     = NULL;
		lParam     = NULL;
		pWnd       = NULL;
		pMarkPoint = NULL;
	}
}TPMarkPointInfo;

enum TP_WINDOWSPATH_TYPE
{
	TP_WINDOWSPATH_RECYCLE				= CSIDL_BITBUCKET,//CSIDL_BITBUCKET   回收站
	TP_WINDOWSPATH_CONTROLS				= CSIDL_CONTROLS,// CSIDL_CONTROLS   控制面板
	TP_WINDOWSPATH_DESKTOP				= CSIDL_DESKTOP,// CSIDL_DESKTOP   Windows  桌面Desktop   
	TP_WINDOWSPATH_DESKTOPDIRECTORY		= CSIDL_DESKTOPDIRECTORY,// CSIDL_DESKTOPDIRECTORY   Desktop的目录  
	TP_WINDOWSPATH_DRIVES				= CSIDL_DRIVES,// CSIDL_DRIVES   我的电脑 
	TP_WINDOWSPATH_FONTS				= CSIDL_FONTS,// CSIDL_FONTS   字体目录  
	TP_WINDOWSPATH_NETHOOD				= CSIDL_NETHOOD,// CSIDL_NETHOOD   网上邻居  
	TP_WINDOWSPATH_NETWORK				= CSIDL_NETWORK,// CSIDL_NETWORK   网上邻居虚拟目录 
	TP_WINDOWSPATH_PERSONAL				= CSIDL_PERSONAL,// CSIDL_PERSONAL   我的文档
	TP_WINDOWSPATH_PRINTERS				= CSIDL_PRINTERS,// CSIDL_PRINTERS   打印机 
	TP_WINDOWSPATH_PROGRAMS				= CSIDL_PROGRAMS,// CSIDL_PROGRAMS   程序组  
	TP_WINDOWSPATH_RECENT				= CSIDL_RECENT,// CSIDL_RECENT   最近打开的文档 
	TP_WINDOWSPATH_SENDTO				= CSIDL_SENDTO,// CSIDL_SENDTO   “发送到”菜单项   
	TP_WINDOWSPATH_STARTMENU			= CSIDL_STARTMENU,// CSIDL_STARTMENU   任务条启动菜单项 
	TP_WINDOWSPATH_STARTUP				= CSIDL_STARTUP,// CSIDL_STARTUP   启动目录
	TP_WINDOWSPATH_TEMPLATES			= CSIDL_TEMPLATES,// CSIDL_TEMPLATES   文档模板  
};
//TP_TIMELINE_VOSCMD
enum TPVosCmd
{
	TPVosCmd_None,
	TPVosCmd_Create,
	TPVosCmd_Destroy,
	TPVosCmd_Save,
	TPVosCmd_Refresh,
	TPVosCmd_GetSize,
	TPVosCmd_SysMenu,
	TPVosCmd_StopThread,
	TPVosCmd_SetActivate,
};
typedef struct stu_VosInfo 
{
	TPVosCmd  dwCmd;//1 显示 2 关闭  3 刷新 4 删除部分  5 删除全部  6 Comment 7 GetSize 8 SysMenu
	WPARAM    wParam;
	LPARAM    lParam;
	CWnd     *pWnd;
 	stu_VosInfo(TPVosCmd dwCmdSet = TPVosCmd_None)
	{
		dwCmd   = dwCmdSet;
		wParam  = NULL;
		lParam  = NULL;
		pWnd    = NULL;
	}
}TPVosInfo;

//TP_TIMELINE_FMTCMD
enum TPFmtCmd
{
	TPFmtCmd_None,
	TPFmtCmd_Create,
	TPFmtCmd_Destroy,
	TPFmtCmd_Refresh,
	TPFmtCmd_GetInfo,
	TPFmtCmd_SysMenu,
	TPFmtCmd_StopThread,
	TPFmtCmd_SetActivate,
};
typedef struct stu_FmtInfo 
{
	TPFmtCmd   dwCmd;//1 显示 2 关闭  3 刷新 4 删除部分  5 删除全部  6 Comment 7 GetSize 8 SysMenu
	WPARAM     wParam;
	LPARAM     lParam;
	LPVOID     pVoid;
	CWnd      *pWnd;
	stu_FmtInfo(TPFmtCmd dwCmdSet = TPFmtCmd_None)
	{
		dwCmd  = dwCmdSet;
		wParam = NULL;
		lParam = NULL;
		pVoid  = NULL;
		pWnd   = NULL;
	}
}TPFmtInfo;

typedef struct stu_TPAutoValue
{
	stu_TPAutoValue(double    *pdValue,double dSetValue)    {m_iType = 1;m_pdValueF   = pdValue;m_dSetValueF   = *pdValue;*pdValue = dSetValue;}
	stu_TPAutoValue(DWORD     *pdValue,DWORD dSetValue)     {m_iType = 2;m_pdValueDW  = pdValue;m_dSetValueDW  = *pdValue;*pdValue = dSetValue;}
	stu_TPAutoValue(ULONGLONG *pdValue,ULONGLONG dSetValue) {m_iType = 3;m_pdValueULL = pdValue;m_dSetValueULL = *pdValue;*pdValue = dSetValue;}
	stu_TPAutoValue(int       *pdValue,int dSetValue)       {m_iType = 4;m_pdValueI   = pdValue;m_dSetValueI   = *pdValue;*pdValue = dSetValue;}
	stu_TPAutoValue(INT64     *pdValue,INT64 dSetValue)     {m_iType = 5;m_pdValueI64 = pdValue;m_dSetValueI64 = *pdValue;*pdValue = dSetValue;}
	stu_TPAutoValue(long      *pdValue,long dSetValue)      {m_iType = 6;m_pdValueL   = pdValue;m_dSetValueL   = *pdValue;*pdValue = dSetValue;}
	~stu_TPAutoValue()
	{
		if(m_iType == 1)      *m_pdValueF   = m_dSetValueF;    else if(m_iType == 2) *m_pdValueDW = m_dSetValueDW;
		else if(m_iType == 3) *m_pdValueULL = m_dSetValueULL;  else if(m_iType == 4) *m_pdValueI  = m_dSetValueI;
		else if(m_iType == 5) *m_pdValueI64 = m_dSetValueI64;  else if(m_iType == 6) *m_pdValueL  = m_dSetValueL;
	}
private:
	int        m_iType;
	double    *m_pdValueF;
	double     m_dSetValueF;
	DWORD     *m_pdValueDW;
	DWORD      m_dSetValueDW;
	ULONGLONG *m_pdValueULL;
	ULONGLONG  m_dSetValueULL;
	int       *m_pdValueI;
	int        m_dSetValueI;
	INT64     *m_pdValueI64;
	INT64      m_dSetValueI64;
	long      *m_pdValueL;
	long       m_dSetValueL;
}TPAutoValue;


class CTPRegUtilsEx
{
public:
	CTPRegUtilsEx(HKEY hRootKey = HKEY_CURRENT_USER)
		: m_hKey(NULL)
	{
		m_hRootKey = hRootKey;
	}
	~CTPRegUtilsEx()
	{
		Close();
	}
	LONG Open(LPCTSTR lpSubKey)
	{
		Close();
		m_subKeyName = _T("Software\\ANote\\");
		m_subKeyName += lpSubKey;
		return RegOpenKeyEx(m_hRootKey, m_subKeyName, 0, KEY_ALL_ACCESS, &m_hKey);
	}
	LONG Create(LPCTSTR lpSubKey)
	{
		Close();
		m_subKeyName = _T("Software\\ANote\\");
		m_subKeyName += lpSubKey;
		return RegCreateKeyEx(m_hRootKey, m_subKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, NULL);
	}
	void Close()
	{
		RegCloseKey(m_hKey);
		m_hKey = NULL;
		m_subKeyName.Empty();
	}
	LONG SetString(LPCTSTR valueName, const BYTE* lpData, DWORD cbData)
	{
		return RegSetValueEx(m_hKey, valueName, 0, REG_SZ, lpData, cbData);
	}
	LONG GetString(LPCTSTR valueName, LPBYTE lpData, DWORD cbData)
	{
		LONG ret = ERROR_SUCCESS;
		DWORD size = cbData, type = 0;
		ret = RegQueryValueEx(m_hKey, valueName, NULL, &type, lpData, &size);
		if ((REG_SZ != type))
		{
			ret = ERROR_INVALID_PARAMETER;
		}
		return ret;
	}
	LONG SetBinary(LPCTSTR valueName, const BYTE* lpData, DWORD cbData)
	{
		return RegSetValueEx(m_hKey, valueName, 0, REG_BINARY, lpData, cbData);
	}
	LONG GetBinary(LPCTSTR valueName, LPBYTE lpData, DWORD cbData)
	{
		LONG ret = ERROR_SUCCESS;
		DWORD size = cbData, type = 0;
		ret = RegQueryValueEx(m_hKey, valueName, NULL, &type, lpData, &size);
		if ((size != cbData) || (REG_BINARY != type))
		{
			ret = ERROR_INVALID_PARAMETER;
		}
		return ret;
	}
	LONG SetDword(LPCTSTR valueName, const DWORD* lpData, DWORD cbData)
	{
		return RegSetValueEx(m_hKey, valueName, 0, REG_DWORD, (LPBYTE)lpData, cbData);
	}
	LONG GetDword(LPCTSTR valueName, DWORD* lpData, DWORD cbData)
	{
		LONG ret = ERROR_SUCCESS;
		DWORD size = cbData, type = 0;
		ret = RegQueryValueEx(m_hKey, valueName, NULL, &type, (LPBYTE)lpData, &size);
		if ((size != cbData) || (REG_DWORD != type))
		{
			ret = ERROR_INVALID_PARAMETER;
		}
		return ret;
	}
	DWORD GetDword(LPCTSTR valueName, DWORD cbData)
	{
		LONG ret = ERROR_SUCCESS;
		DWORD size = sizeof(cbData), type = 0,dwData;
		ret = RegQueryValueEx(m_hKey, valueName, NULL,&type,(LPBYTE)&dwData, &size);
		if ((size != sizeof(cbData)) || (REG_DWORD != type)) return cbData;	
		else                                                 return dwData;	
	}
	LONG SetDword(LPCTSTR valueName, DWORD cbData)
	{
		return RegSetValueEx(m_hKey, valueName, 0, REG_DWORD, (LPBYTE)&cbData,sizeof(cbData));
	}
	LONG Open(LPVOID lpSubKey)
	{
		return Open((LPCTSTR)lpSubKey);
	}
	LONG Create(LPVOID lpSubKey)
	{
		return Create((LPCTSTR)lpSubKey);
	}
	LONG SetDword(LPVOID valueName, const DWORD* lpData, DWORD cbData)
	{
		return SetDword((LPCTSTR)valueName, lpData, cbData);
	}
	LONG GetDword(LPVOID valueName, DWORD* lpData, DWORD cbData)
	{
		return GetDword((LPCTSTR)valueName, lpData, cbData);
	}

private:
	CString m_subKeyName;
	HKEY    m_hKey;
	HKEY    m_hRootKey;
};

inline CString TP_GetProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry,LPCTSTR lpszDefault,HKEY hRootKey = HKEY_CURRENT_USER)
{
	CTPRegUtilsEx stu(hRootKey);
	CString     sGet;
	UINT		uType;
	if( ERROR_SUCCESS == stu.Open(lpszSection))
	{
		uType = stu.GetString(lpszEntry,(LPBYTE)sGet.GetBuffer(MAX_PATH),MAX_PATH * sizeof(TCHAR));
		sGet.ReleaseBuffer();
		if( uType != ERROR_SUCCESS) sGet = lpszDefault;
	}
	else if( ERROR_SUCCESS == stu.Create(lpszSection))
	{
		stu.SetString(lpszEntry,(LPBYTE)lpszDefault, lstrlen(lpszDefault) * sizeof(TCHAR));
		sGet = lpszDefault;
	}

	stu.Close();
	return sGet;
};

inline BOOL TP_WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,LPCTSTR lpszValue,HKEY hRootKey = HKEY_CURRENT_USER)
{
	CTPRegUtilsEx stu(hRootKey);
	if(ERROR_SUCCESS != stu.Open(lpszSection)) 
	{
		if(ERROR_SUCCESS != stu.Create(lpszSection)) return 0;
	}
	UINT uType = stu.SetString(lpszEntry,(LPBYTE)lpszValue,lstrlen(lpszValue) * sizeof(TCHAR));
	stu.Close();
	return uType == ERROR_SUCCESS;
}

inline CString	TP_GuidToString( UUID *pUuid)
{
	CString  sTemp;
	unsigned short *pUuidString = NULL;
	RPC_STATUS ret = UuidToString(pUuid, &pUuidString);
	if(pUuidString)
	{
		sTemp= (LPTSTR) pUuidString ;
		RpcStringFree((unsigned short**) &pUuidString );
	} 
	else
		sTemp = _L("");
	return sTemp;
}
inline GUID    TP_GuidFromString( LPCTSTR lpszUuid)
{
	GUID    guidGet = GUID_NULL;
	CString	strGuid = CString(lpszUuid);
	UuidFromString( (unsigned short*)((LPCTSTR)(strGuid.Left(36))), &guidGet );
	return guidGet;
}

#define TP_UuidToString     TP_GuidToString
#define TP_UuidFromString   TP_GuidFromString

inline GUID    TP_GetBrdGUID(CString sGuid,int iIndex = 32)
{
	CString  sTemp = sGuid;	sTemp = _T("000000000000000000000000000000") + sTemp;
	sTemp = sTemp.Mid(sTemp.GetLength() - iIndex);
	sGuid.Empty();
	sGuid    = sTemp.Left(8); sGuid += '-'; sTemp = sTemp.Mid(8);
	sGuid   += sTemp.Left(4); sGuid += '-'; sTemp = sTemp.Mid(4);
	sGuid   += sTemp.Left(4); sGuid += '-'; sTemp = sTemp.Mid(4);
	sGuid   += sTemp.Left(4); sGuid += '-'; sTemp = sTemp.Mid(4);
	sGuid   += sTemp.Left(12);
	return TP_UuidFromString(sGuid);
}
inline void TP_FindFile(CString strPath,CStringArray &arrFile)
{
	WIN32_FIND_DATA findData;
	HANDLE  hFind = FindFirstFile(strPath + _T("\\*.dll"),&findData);
	if(hFind)
	{
		if(!PathIsDirectory(strPath + _T("\\") + findData.cFileName) && PathFileExists(strPath + _T("\\") + findData.cFileName))
		{
			arrFile.Add(strPath + _T("\\") + findData.cFileName);
		}
		while(FindNextFile(hFind,&findData))
		{
			if(PathIsDirectory(strPath + _T("\\") + findData.cFileName)) continue;
			if(PathFileExists(strPath + _T("\\") + findData.cFileName))  arrFile.Add(strPath + _T("\\") + findData.cFileName);
		}
		FindClose(hFind);
	}
}

inline BOOL TP_FindFileName(CString strPath,CString strFile,CStringArray &aFile)
{
	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(strPath + strFile ,&findData);	
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
		{
			//no operate
		}
		else
			aFile.Add(findData.cFileName);

		while(FindNextFile(hFind,&findData))
		{
			if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
			{
				//no operate
			}
			else
				aFile.Add(findData.cFileName);
		}
		FindClose(hFind);
		return TRUE;
	}
	return FALSE;
}

inline void TP_FindFile(CString strPath,CString strFile,BOOL bPath,BOOL bRecursion,CStringArray &aFile)
{
	WIN32_FIND_DATA findData;
	HANDLE  hFind = FindFirstFile(strPath + strFile ,&findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
		{
			if(bPath) aFile.Add(strPath + _L("\\") + findData.cFileName);
			if(bRecursion)  TP_FindFile(strPath + _L("\\") + findData.cFileName,_L("\\*.*"),bPath,bRecursion,aFile);
		}
		else if(!bPath && !((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
		{
			aFile.Add(strPath + _L("\\") + findData.cFileName);
		}
		else
		{
			//no operate
		}

		while(FindNextFile(hFind,&findData))
		{
			if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
			{
				if(bPath) aFile.Add(strPath + _L("\\") + findData.cFileName);
				if(bRecursion)  TP_FindFile(strPath + _L("\\") + findData.cFileName,_L("\\*.*"),bPath,bRecursion,aFile);
			}
			else if(!bPath && !((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
			{
				aFile.Add(strPath + _L("\\") + findData.cFileName);
			}
			else
			{
				//no operate
			}
		}
		FindClose(hFind);
	}
}
inline long TP_GetSamplePreFrame(long iAudioFrame,DWORD eVideoStand,DWORD nSamplesPerSec)
{	
	if(eVideoStand == 2997)
	{
		if(nSamplesPerSec == 48000)
		{
			long lSampleEx[5] = {1,0,1,0,1};
			long lSample = nSamplesPerSec /30+1;
			return lSample + lSampleEx[iAudioFrame%5];
		}
		else
		{
			return	long(nSamplesPerSec /29.97);
		}
	}
	else if(eVideoStand == 30)
	{
		return	nSamplesPerSec/30;
	}
	else if(eVideoStand == 25)
	{
		return	nSamplesPerSec/25;
	}
	else
	{
		ASSERT(0);
		return 0;
	}
}
inline INT64 TP_GetSampleFromFrame(INT64 iAudioFrame,DWORD eVideoStand,DWORD nSamplesPerSec)
{
	if(eVideoStand == 25)         return (INT64)iAudioFrame *nSamplesPerSec/25;
	else if(eVideoStand == 30)    return (INT64)iAudioFrame *nSamplesPerSec/30;
	else if(eVideoStand == 2997)
	{
		if(nSamplesPerSec == 48000)
		{
			long lSampleEx[6] = {0,0,0,0,0,0};
			for(INT l=0;l<5;l++) lSampleEx[l+1] = lSampleEx[l] + TP_GetSamplePreFrame(l,eVideoStand,nSamplesPerSec);
			return (INT64)iAudioFrame/5 * lSampleEx[5] + lSampleEx[iAudioFrame%5];
		}
		else return (INT64)(iAudioFrame * nSamplesPerSec /29.97);
	}
	else
	{
		ASSERT(0);
		return 0;
	}
}
inline INT64 TP_GetFrameFromSample(INT64 iAudioSample,DWORD eVideoStand,DWORD nSamplesPerSec)
{
	if(eVideoStand == 25)         return iAudioSample *25/nSamplesPerSec;
	else if(eVideoStand == 30)    return iAudioSample *30/nSamplesPerSec;
	else if(eVideoStand == 2997)
	{
		if(nSamplesPerSec == 48000)
		{
			INT64 lSampleEx[6] = {0,0,0,0,0,0},lLeft = 0,lFrame = 0;
			for(INT l=0;l<5;l++) lSampleEx[l+1] = lSampleEx[l] + TP_GetSamplePreFrame(l,eVideoStand,nSamplesPerSec);
			lLeft = iAudioSample % lSampleEx[5];
			for(INT l=1;l<=5;l++)
			{
				if(lLeft<lSampleEx[l])
				{
					lFrame = l-1;
					break;
				}
				else if(lLeft ==lSampleEx[l])
				{
					lFrame = l;
					break;
				}
			}
			return iAudioSample/ lSampleEx[5] *5 + lFrame;
		}
		else
		{
			return long(iAudioSample *29.97/nSamplesPerSec + 0.5);
		}
	}
	else
	{
		ASSERT(0);
		return 0;
	}
}

inline GUID TP_GetGuidByFileName(CString sText)
{
	GUID guidBaseUser = { 0xA78AFD2D, 0xE859, 0x4AC1, {0x9A, 0xDB, 0xDF, 0xE9, 0x63, 0x88, 0xD7, 0x3C}};
	sText            += _T("12345678901234567890123456789012345678901234567890");
	for(INT k=0;k<sText.GetLength()/11*11;k+=11)
	{
		guidBaseUser.Data1 += sText[k];
		guidBaseUser.Data2 += sText[k+1];
		// LI HUI LI 2008-10-06
		//guidBaseUser. Data3 += sText[k+3];
		guidBaseUser.Data3 += sText[k+2];
		//]]]
		for(INT m=0;m<8;m++) guidBaseUser.Data4[m] += sText[k+3+m];
	}
	return guidBaseUser;
}
//TP_Str
inline int		TP_StrLen(const TCHAR *cStr,	size_t nMaxSize = MAX_PATH)
{
	if(NULL == cStr)				{ASSERT(0);}

	return _tcsnlen(cStr, nMaxSize);
}
inline void    TP_StrCpy(TCHAR *&cDst, TCHAR *cSrc, size_t nSrcSize , BOOL bNewBuf = TRUE)
{
	if(NULL == cSrc)			{	ASSERT(0); return;				}
	if(bNewBuf && cDst)			{	delete []cDst;  cDst = NULL;	}
	if(NULL == cDst)			{   cDst = new TCHAR[nSrcSize + 1]; ZeroMemory(cDst,sizeof(TCHAR)*(nSrcSize + 1));}

	_tcsncpy_s(cDst , (nSrcSize + 1), cSrc , nSrcSize);
}
inline int		TP_StrCmp(TCHAR *cS1, TCHAR *cS2, size_t nMaxSize = MAX_PATH)
{
	if(NULL == cS1 || NULL == cS2)	{ASSERT(0);}

	size_t nSizeS1 = _tcsnlen(cS1, nMaxSize);
	size_t nSizeS2 = _tcsnlen(cS2, nMaxSize);
	if(nSizeS1 != nSizeS2)			{	return (int)(nSizeS1-nSizeS2);	}

	return _tcsncmp(cS1,cS2,nSizeS1);
}
inline CString TP_GetWindowsSysPath(TP_WINDOWSPATH_TYPE eType)
{
	TCHAR MyDir[_MAX_PATH];   
	SHGetSpecialFolderPath(NULL,MyDir,eType,0); 
	return MyDir;
}

class CTraceEx
{
public:
	CTraceEx(const char *pszFileName, int nLineNo)
		: m_pszFileName(pszFileName), m_nLineNo(nLineNo)
	{}

	void __cdecl operator()(DWORD_PTR dwCategory, UINT nLevel, const char *pszFmt, ...) const
	{
	}
	void __cdecl operator()(DWORD_PTR dwCategory, UINT nLevel, const wchar_t *pszFmt, ...) const
	{
	}
	void __cdecl operator()(const char *pszFmt, ...) const
	{
	}
	void __cdecl operator()(const wchar_t *pszFmt, ...) const
	{
	}

private:
	const char *const m_pszFileName;
	const int m_nLineNo;
};

//指针安全性校验
#define TP_SAFETYPE_TLCLIPEFF            0x0000000000000001
#define TP_SAFETYPE_MENUWNDDATA          0x0000000000000002

#define TP_SAFETYPE_P2DISK               0x0000000000000008
#define TP_SAFETYPE_P2CLIP               0x0000000000000010
#define TP_SAFETYPE_P2EDL                0x0000000000000020
#define TP_SAFETYPE_P2PGM                0x0000000000000040
#define TP_SAFETYPE_RESNOTE              0x0000000000000080
#define TP_SAFETYPE_RESNOTECACHE         0x0000000000000100
#define TP_SAFETYPE_RESPGM               0x0000000000000200
#define TP_SAFETYPE_RESPGMCACHE          0x0000000000000400
#define TP_SAFETYPE_RESTEMP              0x0000000000000800
#define TP_SAFETYPE_RESTEMPCACHE         0x0000000000001000
#define TP_SAFETYPE_RESPROJECT           0x0000000000002000
#define TP_SAFETYPE_RESCATALOG          (0x0000000000004000|TP_SAFETYPE_RESPROJECT)
#define TP_SAFETYPE_RESCACHE             0x0000000000008000
#define TP_SAFETYPE_RESCATALOGMIX        0x0000000000010000

#define TP_SAFETYPE_BRDDISK              0x0000000000020000
#define TP_SAFETYPE_BRDCLIP              0x0000000000040000
#define TP_SAFETYPE_BRDEDL               0x0000000000080000
#define TP_SAFETYPE_BRDPGM               0x0000000000100000
#define TP_SAFETYPE_BRDCACHE             0x0000000000200000


#define TP_SAFETYPE_TLCLIP               0x0000000000400000
#define TP_SAFETYPE_TLBASETRACK          0x0000000000800000
#define TP_SAFETYPE_TLTRACK              0x0000000001000000
#define TP_SAFETYPE_TLDATA               0x0000000002000000
#define TP_SAFETYPE_TLLISTBASETRACK      0x0000000004000000
#define TP_SAFETYPE_TLLISTCLIP           0x0000000008000000
#define TP_SAFETYPE_TLGROUP              0x0000000010000000


#define TP_SAFETYPE_TLWNDCLIP            0x0000000020000000
#define TP_SAFETYPE_TLWNDBASETRACK       0x0000000040000000
#define TP_SAFETYPE_TLWNDTRACK           0x0000000080000000
#define TP_SAFETYPE_TLWND                0x0000000100000000
#define TP_SAFETYPE_TLWNDSBBASETRACK     0x0000000200000000
#define TP_SAFETYPE_TLWNDSBCLIP          0x0000000400000000
#define TP_SAFETYPE_TLSBWND              0x0000000800000000
#define TP_SAFETYPE_NOERROR              0x0000001000000000

#define TP_SAFETYPE_RESALL              (TP_SAFETYPE_RESCATALOGMIX|TP_SAFETYPE_RESNOTE |TP_SAFETYPE_RESNOTECACHE|TP_SAFETYPE_RESPGM|TP_SAFETYPE_RESPGMCACHE|TP_SAFETYPE_RESTEMP|TP_SAFETYPE_RESTEMPCACHE|TP_SAFETYPE_RESPROJECT|TP_SAFETYPE_RESCATALOG|TP_SAFETYPE_RESCACHE)
#define TP_SAFETYPE_BRDALL              (TP_SAFETYPE_BRDDISK|TP_SAFETYPE_BRDCLIP|TP_SAFETYPE_BRDEDL|TP_SAFETYPE_BRDPGM|TP_SAFETYPE_BRDCACHE)
#define TP_SAFETYPE_TLALL               (TP_SAFETYPE_TLGROUP|TP_SAFETYPE_TLLISTCLIP|TP_SAFETYPE_TLLISTBASETRACK|TP_SAFETYPE_TLCLIP|TP_SAFETYPE_TLBASETRACK|TP_SAFETYPE_TLTRACK|TP_SAFETYPE_TLDATA)
#define TP_SAFETYPE_TLWNDALL            (TP_SAFETYPE_TLSBWND|TP_SAFETYPE_TLWNDCLIP|TP_SAFETYPE_TLWNDBASETRACK|TP_SAFETYPE_TLWNDTRACK|TP_SAFETYPE_TLWND|TP_SAFETYPE_TLWNDSBBASETRACK|TP_SAFETYPE_TLWNDSBCLIP)
#define TP_SAFETYPE_P2ALL               (TP_SAFETYPE_P2DISK |TP_SAFETYPE_P2CLIP |TP_SAFETYPE_P2EDL |TP_SAFETYPE_P2PGM)

class CTPBaseObject
{
public:
	CTPBaseObject()
	{
		m_dwSafeFlag    = rand();
		m_dwSafeFlagEx  = m_dwSafeFlag + 1;
		m_dwBaseType    = 0;
		m_dwSafeType    = 0;
		m_pRes          = NULL;
		m_lRes          = 0;
	}
	virtual ~CTPBaseObject()
	{
		m_dwSafeFlag   = 0;
		m_dwSafeFlagEx = 0;
	}

private: //此处顺序千万不要修改
	WORD    m_dwSafeFlag;
	WORD    m_dwSafeFlagEx;

public:
	DWORD   m_dwBaseType;
	INT64   m_dwSafeType;

	LPVOID  m_pRes;
	long    m_lRes;

public:
	void    ReleaseObject()
	{
		m_dwSafeFlag   = 0;
		m_dwSafeFlagEx = m_dwSafeFlag+2;
	}
	BOOL    IsObject()
	{
		return m_dwSafeFlag == 0;
	}
	BOOL    IsSafeType(DWORD dwType)
	{
		if(m_dwSafeFlagEx!=m_dwSafeFlag+1)    return FALSE;
		if((dwType & m_dwBaseType) != dwType) return FALSE;
		return TRUE;
	}
};
typedef CArray<CTPBaseObject *,CTPBaseObject *&> CTPBaseObjectArray;

static  ULONGLONG lMinMemory = 0;
static  ULONGLONG lMaxMemory = 0;
inline BOOL TP_GetSafeAdress(void *  hTLData,INT64 wSafeType,int iExt = 1)
{
	if(hTLData == NULL) return FALSE;
	if(lMaxMemory == 0)
	{
		SYSTEM_INFO sinf;
		GetSystemInfo(&sinf);
		lMinMemory = (ULONGLONG) sinf.lpMinimumApplicationAddress;
		lMaxMemory = (ULONGLONG) sinf.lpMaximumApplicationAddress;
	}
	try
	{
		hTLData = (BYTE *)hTLData +8;
#ifndef TP_NS_RELEASE
		if((ULONGLONG)hTLData<lMinMemory || (ULONGLONG)hTLData>=  lMaxMemory)// !AfxIsValidAddress((void *)hTLData,4))
		{
			ASSERT(0); return FALSE;
		}
		long len=sizeof(MEMORY_BASIC_INFORMATION);
		MEMORY_BASIC_INFORMATION mbiData;
		VirtualQuery(hTLData,&mbiData,len);
		if(mbiData.State != MEM_COMMIT)
		{
			ASSERT(0);
			return FALSE;
		}
		if(*(WORD *)hTLData == *((WORD *)hTLData +1) -iExt)
		{
			hTLData = (BYTE *)hTLData +8;
			if(*((INT64 *)hTLData)  & wSafeType)
				return TRUE;
			else
			{
				if(!(TP_SAFETYPE_NOERROR & wSafeType))
					ASSERT(0);
				return FALSE;
			}
		}
		else
		{
			if(!(TP_SAFETYPE_NOERROR & wSafeType))
				ASSERT(0);
			return FALSE;
		}
#else
		if((ULONGLONG)hTLData<lMinMemory || (ULONGLONG)hTLData>=  lMaxMemory)
		//if(!AfxIsValidAddress((void *)hTLData,4))
			return FALSE;
		long len=sizeof(MEMORY_BASIC_INFORMATION);
		MEMORY_BASIC_INFORMATION mbiData;
		VirtualQuery(hTLData,&mbiData,len);
		if(mbiData.State != MEM_COMMIT)
		{
			return FALSE;
		}
		return *(WORD *)hTLData == *((WORD *)hTLData +1) -iExt;
#endif

	}
	catch(...)
	{
		return FALSE;			 
	}
}

typedef struct tag_TPEncodeParam
{
	//入
	LONG             biHeight;
	LONG             biWidth;
	WORD             biBitCount;
	DWORD            eVideoStandard;
	INT64            lLength; 
	//出 
	int              nGopSize;
	DWORD            eCodeType;	
	BOOL             bAppendFrame;
	tag_TPEncodeParam()
	{
		bAppendFrame = TRUE;
	}

}TPEncodeParam;

#define TP_ENCODETYPE_DV      0x00000001
#define TP_ENCODETYPE_IMX30   0x00000002
#define TP_ENCODETYPE_IMX40   0x00000004
#define TP_ENCODETYPE_IMX50   0x00000008
#define TP_ENCODETYPE_HDV15   0x00000010
#define TP_ENCODETYPE_HDV25   0x00000020
#define TP_ENCODETYPE_HDV35   0x00000040
#define TP_ENCODETYPE_HD50    0x00000080

#define TP_ENCODETYPE_IMX    (TP_ENCODETYPE_IMX40 | TP_ENCODETYPE_IMX50 |TP_ENCODETYPE_IMX30)
#define TP_ENCODETYPE_HD     (TP_ENCODETYPE_HD50| TP_ENCODETYPE_HDV15 | TP_ENCODETYPE_HDV25 |TP_ENCODETYPE_HDV35)

typedef struct tag_TPEncode
{
	void      *m_pUserCtrl;
	UINT       m_uEncodeType;

	LRESULT (*TP_Open)();
	LRESULT (*TP_Close)();

	LRESULT (*TP_InitEncode)(TPEncodeParam *pParam);
	LRESULT (*TP_ReleaseEncode)();

	BOOL	(*TP_IsEmpty)();
	BOOL	(*TP_IsSupportMutliThread)();

	BOOL	(*TP_EndEncode)();

	void    (CALLBACK *TP_Mpeg2Encode_CB)(UINT unType, BYTE* pData, int nSize, void* pUserCtrl);
	LRESULT (*TP_SetVideoInputBuffer)(BYTE *pBufferIn, int nBufLenIn);
	tag_TPEncode()
	{
		ZeroMemory(this,sizeof(tag_TPEncode));
	}
}TPEncode;

struct TPSyncLock
{
public:
	TPSyncLock(CMutex *pMutex)
	{
		m_pSection = NULL;
		m_pMutex   = pMutex;
		if(m_pMutex)	m_pMutex ->Lock();
	}
	TPSyncLock(CCriticalSection *pLock)
	{
		m_pSection = pLock;
		m_pMutex   = NULL;
		if(m_pSection)	m_pSection ->Lock();
	}
	~TPSyncLock()
	{
		if(m_pMutex)	m_pMutex ->Unlock();
		if(m_pSection)	m_pSection ->Unlock();
		m_pMutex   = NULL;
		m_pSection = NULL;
	}
private:
	CMutex            *m_pMutex;
	CCriticalSection  *m_pSection;
};
typedef struct stu_TPMutex :public CMutex
{
	stu_TPMutex(CString sLock,BOOL bMutex = TRUE):CMutex(FALSE,sLock)
	{
		m_bMutex = bMutex;
		if(bMutex)	CMutex::Lock();
	}
	stu_TPMutex(UINT_PTR uLock,BOOL bMutex = TRUE):CMutex(FALSE,GetMutexText(uLock))
	{
		m_bMutex = bMutex;
		if(bMutex)	Lock();
	}
	BOOL Lock(DWORD dwTimeOut = INFINITE)
	{
		if(m_bMutex) return TRUE;
		m_bMutex = TRUE;
		return CMutex::Lock();
	}
	CString GetMutexText(UINT_PTR uLock)
	{
		CString sText;
		sText.Format(_T("TPMutex 0X%x"),uLock);
		return sText;
	}
	~stu_TPMutex()
	{
		if(m_bMutex) Unlock();
	}
private:
	BOOL   m_bMutex;
}TPMutex;

typedef struct tagTPRECTD
{
	long double    left;
	long double    top;
	long double    right;
	long double    bottom;

	tagTPRECTD()
	{
		left   = 0;
		top    = 0;
		right  = 0;
		bottom = 0;
	}
	void OffsetRect(long double x, long double y)
	{
		left   += x;
		top    += y;
		right  += x;
		bottom += y;
	}
	void NormalizeRect()
	{
		if(left >right)
		{
			long double dTemp = left;
			left = right;
			right = dTemp;
		}
		if(top >bottom)
		{
			long double dTemp = top;
			top    = bottom;
			bottom = dTemp;
		}
	}
}TPRectD;

typedef CArray<TPRectD,TPRectD &> TPRectDArray;

#define	 _HDXMYBIN	 _T("M:\\MEDIA\\REELS")
#define	 _HDXLIB  	 _T("\\\\HDXCHANGESERVER")
#define	 _HDXBROWSE  _T("P:\\Browse\\Browse.exe")


#define TP_GetInOutLength(x,y) abs(int(y-x))

#ifdef TP_NS_EDIT
#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h>
#endif

#define IDD_EXTRACTRL_BASE                 1700
#define IDD_COMMONCTRL_BASE                1800
#define IDD_COMMONWND_BASE                 1900
#define IDD_CLIPEXPLORER_BASE              2000
#define IDD_BRDEXPLORER_BASE               2500
#define IDD_TIMELINEWND_BASE               3000
#define IDD_FXADJUST_BASE                  3500
/*
normal（普通）、client（客户端）或 CRT（运行时）；本文例子中是：
_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
_CrtDumpMemoryLeaks。
_CrtSetReportMode */

//////////////////////////////////////////////////////////////////////////

typedef struct _tagTPChannelItem
{
	TCHAR	*cItemTitle;
	TCHAR	*cItemLink;
	TCHAR	*cItemDescription;
	TCHAR	*cItemPubDate;
	_tagTPChannelItem()
	{
		Reset();
	}
	~_tagTPChannelItem()
	{
		Release();
	}
	void Reset()
	{
		cItemTitle = NULL;
		cItemLink = NULL;
		cItemDescription = NULL;
		cItemPubDate = NULL;
	}
	void Release()
	{
		if(cItemTitle)  {delete cItemTitle ; cItemTitle = NULL;}
		if(cItemLink)  {delete cItemLink ; cItemLink = NULL;}
		if(cItemDescription)  {delete cItemDescription ; cItemDescription = NULL;}
		if(cItemPubDate)  {delete cItemPubDate ; cItemPubDate = NULL;}
	}
	_tagTPChannelItem &operator = (_tagTPChannelItem &oTher)
	{
		Release();

		TP_StrCpy(cItemTitle, oTher.cItemTitle, TP_StrLen(oTher.cItemTitle));
		TP_StrCpy(cItemLink, oTher.cItemLink, TP_StrLen(oTher.cItemLink));
		TP_StrCpy(cItemDescription, oTher.cItemDescription, TP_StrLen(oTher.cItemDescription));
		TP_StrCpy(cItemPubDate, oTher.cItemPubDate, TP_StrLen(oTher.cItemPubDate));
		return *this;
	}

}TPChannelItem;
typedef CArray<TPChannelItem *, TPChannelItem *&>	CTPChannelItemArray;

typedef struct _tagTPChannelImage
{
	TCHAR	*cImageUrl;
	TCHAR	*cImageTitle;
	TCHAR	*cImageLink;
	_tagTPChannelImage()
	{
		Reset();
	}
	~_tagTPChannelImage()
	{
		Release();
	}
	void Reset()
	{
		cImageUrl	= NULL;
		cImageTitle = NULL;
		cImageLink	= NULL;		
	}
	void Release()
	{
		if(cImageUrl)  {delete cImageUrl ; cImageUrl = NULL;}
		if(cImageTitle)  {delete cImageTitle ; cImageTitle = NULL;}
		if(cImageLink)  {delete cImageLink ; cImageLink = NULL;}
	}
	_tagTPChannelImage &operator =(_tagTPChannelImage &oTher)
	{
		Release();

		TP_StrCpy(cImageUrl, oTher.cImageUrl, TP_StrLen(oTher.cImageUrl));
		TP_StrCpy(cImageTitle, oTher.cImageTitle, TP_StrLen(oTher.cImageTitle));
		TP_StrCpy(cImageLink, oTher.cImageLink, TP_StrLen(oTher.cImageLink));
		return *this;
	}
}TPChannelImage;
typedef struct _tagTPChannelBase
{
	TCHAR	*cChannelAddress;       //频道rss地址
	TCHAR	*cChannelTitle;			//频道名
	TCHAR	*cChannelDescription;	//频道描述
	TCHAR	*cChannelLink;			//频道主页
	TCHAR	*cChannelGenerator;     //频道创始者
	TPChannelImage		stuChannelImage;
	CTPChannelItemArray	aChannelItem;
	_tagTPChannelBase()
	{	
		Reset();
	}
	~_tagTPChannelBase()
	{
		Release();	
	}
	void Release()
	{
		if(cChannelTitle)     {delete cChannelTitle ; cChannelTitle = NULL;}
		if(cChannelDescription)     {delete cChannelDescription ; cChannelDescription = NULL;}
		if(cChannelLink)     {delete cChannelLink ; cChannelLink = NULL;}
		if(cChannelAddress)     {delete cChannelAddress ; cChannelAddress = NULL;}
		if(cChannelGenerator)     {delete cChannelGenerator ; cChannelGenerator = NULL;}
		stuChannelImage.Release();
		for (int l=0;l<aChannelItem.GetSize();l++)	
		{
			delete aChannelItem[l];
			aChannelItem[l] = NULL;
		}
		aChannelItem.RemoveAll();	
	}
	void Reset()
	{
		cChannelTitle = NULL;
		cChannelDescription = NULL;
		cChannelLink  = NULL;
		cChannelAddress = NULL;
		cChannelGenerator = NULL;
		stuChannelImage.Reset();
		aChannelItem.RemoveAll();
	}
	_tagTPChannelBase &operator = (_tagTPChannelBase &oTher)
	{
		Release();

		TP_StrCpy(cChannelTitle, oTher.cChannelTitle, TP_StrLen(oTher.cChannelTitle));
		TP_StrCpy(cChannelDescription, oTher.cChannelDescription, TP_StrLen(oTher.cChannelDescription));
		TP_StrCpy(cChannelLink, oTher.cChannelLink, TP_StrLen(oTher.cChannelLink));
		TP_StrCpy(cChannelAddress, oTher.cChannelAddress, TP_StrLen(oTher.cChannelAddress));
		TP_StrCpy(cChannelGenerator, oTher.cChannelGenerator, TP_StrLen(oTher.cChannelGenerator));
		stuChannelImage = oTher.stuChannelImage;
		for (int l = 0 ; l < oTher.aChannelItem.GetSize(); l++)
		{
			TPChannelItem *pItem = new TPChannelItem;
			*pItem = *oTher.aChannelItem[l];
			aChannelItem.Add(pItem);
		}
		
		return *this;
	}
}TPChannelBase, *LPTPChannelBase;
typedef CArray<TPChannelBase *, TPChannelBase*&> CTPChannelBase;