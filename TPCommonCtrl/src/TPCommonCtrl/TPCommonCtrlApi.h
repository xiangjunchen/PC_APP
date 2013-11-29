#pragma once

#include <afxtempl.h>

#define TP_PointDistance(a,b) (abs(a.x - b.x)>6 || abs(a.y - b.y)>6)

inline int	TP_DoubleToInt(double x)	
{
	return int((x)>=0 ? (int)((x)+0.5) : (int)((x)-0.5)); 
}

inline int	TP_DoubleToIntEx(double x)	
{
	return int((x)>=0 ? (int)((x)+0.9999999) : - (int)(-(x)+0.999999999)); 
}

inline __int64	TP_DoubleToInt64(long double x)	
{
	return __int64((x)>=0 ? ((x)+0.5) : ((x)-0.5)); 
}

inline __int64	TP_DoubleToInt64Ex(long double x)	
{
	return __int64((x)>=0 ? ((x)+0.9999999) : ((x)-0.9999999)); 
}

#define		TP_IsNull(x)	    (BOOL( (x) == 0 || (x) == 0xcccccccc) )

enum VIDEO_TYPE
{
	VIDEO_25 = 0,
	VIDEO_30M,
	VIDEO_30,
	VIDEO_SECAM,
};

#define   VIDEO_DAY 0x10000000

#define   TP_SOBEYSET                   0x10000037

AFX_EXT_API BOOL    TP_IsCtrlKeyDown();
AFX_EXT_API BOOL    TP_IsAltKeyDown();
AFX_EXT_API BOOL    TP_IsShiftKeyDown();
AFX_EXT_API UINT    TP_GetKeyFlag(UINT vKey = 0);
AFX_EXT_API BOOL    TP_IsLButtonDown();
AFX_EXT_API BOOL    TP_IsRButtonDown();

AFX_EXT_API void    TP_SetSpecialKey(DWORD dSet,DWORD  dRemove);
AFX_EXT_API void    TP_SetSpecialKeyForce(DWORD dSet,DWORD  dRemove);
AFX_EXT_API void    TP_ShowErrorMsg();
AFX_EXT_API BOOL    TP_IsEditFocus(UINT vKey);
AFX_EXT_API BOOL    TP_IsPropertyKey(UINT vKey);
AFX_EXT_API void    TP_ClipPoint(CRect rtClip,CPoint &ptClip);
AFX_EXT_API DWORD   TP_HitPoint(CRect rtClip,CPoint ptClip);


AFX_EXT_API __int64  _tmcount(SYSTEMTIME *psrc);
AFX_EXT_API int      _tmcmp(SYSTEMTIME *psrc,SYSTEMTIME *pdes);

AFX_EXT_API long       TP_GetTimeRate(DWORD eVideoStandard);
AFX_EXT_API VIDEO_TYPE TP_GetVideoType(DWORD eVideoStandard,BOOL bDropFrame = TRUE);
AFX_EXT_API __int64    TP_FrameFromDay(VIDEO_TYPE enVideoType,int iDay);   
AFX_EXT_API CString    TP_StringFromFrame(VIDEO_TYPE eVideoType,__int64 iFrame,BOOL bData = FALSE);   
AFX_EXT_API CString    TP_StringFromFrameMin(VIDEO_TYPE eVideoType,__int64 iFrame,BOOL bData = FALSE);   
AFX_EXT_API int        TP_GetFrameFromString(VIDEO_TYPE eType,CString strFrame,BOOL bIsAddRemove=FALSE);    //XN00022424 linhao 2009-11-03 change
AFX_EXT_API __int64    TP_AddReduceFrame(VIDEO_TYPE eType, __int64 nFrameSrc, CString strFrameOffset, BOOL bAdd, DWORD dwFlag=0);
AFX_EXT_API int        TP_GetMaxFrameOfDay(VIDEO_TYPE eType);
AFX_EXT_API DWORD	   TP_GetFrameFormTime( int nH, int nM , int nS , int nF,DWORD eVtrFormat);
AFX_EXT_API DWORD	   TP_GetFrameFormTimeAddRemove( int nH, int nM , int nS , int nF,DWORD eVtrFormat);//XN00022424 linhao 2009-11-03 add
 AFX_EXT_API void       TP_GetTimeFormFrame( DWORD Frame , int& nH  , int& nM , int& nS , int& nF,DWORD eVtrFormat );
AFX_EXT_API CString    TP_GetTimeString(SYSTEMTIME &tTime, BOOL bData = TRUE, BOOL bTime = TRUE);
AFX_EXT_API DWORD	   TP_IncreaseFrame( DWORD Frame,int nH, int nM , int nS ,int nF,DWORD eVtrFormat);

AFX_EXT_API BOOL       TP_PathIsValidate(CString &sFile);
AFX_EXT_API void       TP_DrawTextInRect(CDC *pDC, CString szString, LPRECT lpRect, UINT uFormat);

AFX_EXT_API BOOL       TP_CreateDirectory(CString sPath);
AFX_EXT_API BOOL       TP_DeleteDirectory(CString sPath,BOOL bRoot = TRUE);

AFX_EXT_API  LRESULT  TP_CheckRename(CString sName,BOOL bMessageBox = TRUE);
AFX_EXT_API  CString  TP_GetValidName(CString sName);
AFX_EXT_API  void     TP_GetProperImageSize(CRect rtShow,int iWidth,int iHeight,CRect &rtRight);

AFX_EXT_API  void     TP_GetViewScreenSize(DWORD eStd,CPoint &pt);

AFX_EXT_API  long     TP_GetMMTimeMinSecond();
AFX_EXT_API  void     TP_ConvertRGB2YUVField(IN OUT BYTE* pDest,IN const int nDestPitch,IN  BYTE* pSrcRGB, IN const int nSrcPitch,IN int nWidth,IN int nHeight);		
AFX_EXT_API  void     TP_DestoryFloatTable();
AFX_EXT_API  void     TP_CreateFloatTable();
AFX_EXT_API  void     TP_MessageShow(CStringArray &aError);

AFX_EXT_API  void     TP_SetCommonData(int iData,void *pData);
AFX_EXT_API  BOOL     TP_GetCommonData(int iData,void *&pData);

AFX_EXT_API  HWND     TP_GetTopFrame(HWND hWnd);
AFX_EXT_API  BOOL     TP_GetValidateWnd(HWND hWnd);
AFX_EXT_API  BOOL     TP_IsCtrlHotKeyHelp(UINT uKey);

AFX_EXT_API  LPCTSTR  TP_GetSoftWareVersion();


AFX_EXT_API  CString  TP_GetVKName(UINT uChar,UINT uFlag);


AFX_EXT_API  void  TP_InitVld();
AFX_EXT_API  void  TP_ReleaseVld();
 
AFX_EXT_API  BOOL  TP_PathFileExistsEx(LPCTSTR pszPath);
