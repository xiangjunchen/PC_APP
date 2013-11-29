// TPCommonCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TPCommonCtrlApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



namespace _tp_commonctrl
{
	extern LPTPNSControlData  g_pGlobalHotKey;
	extern long               g_lGlobalHotKey;
	extern long               g_lGlobalHotKeyHelp;

	__int64 _tmcount(SYSTEMTIME *psrc)
	{
		return ((((__int64)psrc ->wYear * 400 + psrc ->wMonth *31 + psrc ->wDay )*24 + psrc ->wHour) * 3600 + psrc ->wMinute *60 + psrc ->wSecond) *1000 + psrc ->wMilliseconds; 

	}
	int _tmcmp(SYSTEMTIME *psrc,SYSTEMTIME *pdes)
	{
		__int64 iSrc  = ((((__int64)psrc ->wYear * 400 + psrc ->wMonth *31 + psrc ->wDay )*24 + psrc ->wHour) * 3600 + psrc ->wMinute *60 + psrc ->wSecond) *1000 + psrc ->wMilliseconds; 
		__int64 iDesc = ((((__int64)pdes ->wYear * 400 + pdes ->wMonth *31 + pdes ->wDay )*24 + pdes ->wHour) * 3600 + pdes ->wMinute *60 + pdes ->wSecond) *1000 + pdes ->wMilliseconds; 
		if(iSrc >iDesc)        return 1;
		else if(iSrc == iDesc) return 0;
		else return -1;
	}

	void TP_ShowErrorMsg()
	{
#ifndef _DEBUG
		return;
#endif
		LPVOID lpMsgBuf;
		if (!FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL ))
		{
			// Handle the error.
			return;
		}

		// Process any inserts in lpMsgBuf.
		// ...

		// Display the string.
		MessageBox( NULL, (LPCTSTR)lpMsgBuf, _T("Please ask enginner"), MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}
	BOOL   TP_IsLButtonDown()
	{
		return GetKeyState(VK_LBUTTON) <0;
	}
	BOOL   TP_IsRButtonDown()
	{
		return GetKeyState(VK_RBUTTON) <0;
	}
	DWORD  g_dwSpecialForceSet = 0; 
	void   TP_SetSpecialKeyForce(DWORD dSet,DWORD  dRemove)
	{
		g_dwSpecialForceSet &= ~dRemove;
		g_dwSpecialForceSet |= dSet;
	}
	DWORD  g_dwSpecialForce = 0; 
	void   TP_SetSpecialKey(DWORD dSet,DWORD  dRemove)
	{
		g_dwSpecialForce &= ~dRemove;
		g_dwSpecialForce |= dSet;
	}
#define FR_HOUR_ND      108000L  // frames per hour in non-drop mode
#define FR_HOUR_DROP    107892L  // frames per hour in drop frame mode
#define FR_TENMIN_DROP  17982L   // frames per ten minutes in drop frame
#define FR_MINUTE_DROP  1798L    // frame per minute in drop frame mode
#define FR_MINUTE_ND    1800L    // frames per minute in non-drop mode
#define FR_SECOND       30L      // frames per second, both modes NTSC

#define FR_HOUR_PAL     90000L   // frames per hour in PAL
#define FR_MINUTE_PAL   1500L    // frames per minute in PAL
#define FR_SECOND_PAL   25L      // frames per second in PAL

	BOOL   TP_IsCtrlKeyDown()
	{
		if(g_dwSpecialForce & TP_CTRL)    return FALSE;
		if(g_dwSpecialForceSet & TP_CTRL) return TRUE;
		return GetKeyState(VK_LCONTROL) <0 || GetKeyState(VK_RCONTROL) <0 ;
	}
	BOOL   TP_IsAltKeyDown()
	{
		if(g_dwSpecialForce & TP_ALT)    return FALSE;
		if(g_dwSpecialForceSet & TP_ALT) return TRUE;
		return GetKeyState(VK_LMENU) <0 || GetKeyState(VK_RMENU) <0 ;
	}
	BOOL   TP_IsShiftKeyDown()
	{
		if(g_dwSpecialForce & TP_SHIFT)    return FALSE;
		if(g_dwSpecialForceSet & TP_SHIFT) return TRUE;
		return GetKeyState(VK_LSHIFT) <0   || GetKeyState(VK_RSHIFT) <0;
	}
	UINT    TP_GetKeyFlag(UINT vKey) //不能返回 TP_ALLKEY(0xFFFFFFFF),否则重新取
	{
		UINT uFlag  =  0;
		if(	GetAsyncKeyState(VK_CONTROL) <0 || GetKeyState(VK_CONTROL) <0) uFlag |= TP_CONTROL;
		if(	GetAsyncKeyState(VK_SHIFT) <0   || GetKeyState(VK_SHIFT) <0)   uFlag |= TP_SHIFT;
		if(	GetAsyncKeyState(VK_MENU) <0    || GetKeyState(VK_MENU) <0)    uFlag |= TP_MENU;
		
		uFlag &= ~g_dwSpecialForce;
		uFlag |=  g_dwSpecialForceSet;


		if(vKey>=5000)      uFlag |= TP_C5;
		else if(vKey>=4000) uFlag |= TP_C2;

		return uFlag;
	}
	BOOL   TP_IsPropertyKey(UINT nChar)
	{
		if ( (nChar == VK_1  || nChar == VK_NUMPAD1) && TP_IsCtrlKeyDown() || VK_Ctrl_1 == nChar) 		 
			return TRUE;		 
		else 
			return FALSE;	 
	}

	BOOL   TP_IsEditFocus(UINT vKey)
	{		
		if(vKey>400) return FALSE;
		CWnd *pWnd = CWnd::GetFocus();	
		if(pWnd  == NULL) return FALSE;
		TCHAR cName[MAX_PATH];ZeroMemory(cName,sizeof(cName));
		GetClassName(pWnd->GetSafeHwnd(),cName,MAX_PATH);
		if(lstrcmpi(cName,_T("Edit")) ==0 ||
			lstrcmpi(cName,_T("TPEditEx")) ==0 ||
			lstrcmpi(cName,_T("NumberEditClass")) ==0  ||
			lstrcmpi(cName,_T("TPTimeCodeEditNSClass")) ==0 ||
			lstrcmpi(cName,_T("NumericEdit")) ==0 ||
			lstrcmpi(cName,_T("TPTimeCodeEditExClass")) ==0	)
			return TRUE;

		return FALSE;
	}
	void TP_ClipPoint(CRect rtClip,CPoint &ptClip)
	{
		ptClip.y = max(ptClip.y,rtClip.top);
		ptClip.y = min(ptClip.y,rtClip.bottom);	
		ptClip.x = max(ptClip.x,rtClip.left);
		ptClip.x = min(ptClip.x,rtClip.right);
	}

	DWORD TP_HitPoint(CRect rtClip,CPoint ptClip)
	{
		if(rtClip.PtInRect(ptClip)) return HTCLIENT;
		else if(ptClip.x<rtClip.left)
		{
			if(ptClip.y <rtClip.top)         return HTTOPLEFT;
			else if(ptClip.y >rtClip.bottom) return HTBOTTOMLEFT;
			else return HTLEFT;
		}
		else  if(ptClip.x>rtClip.right)
		{
			if(ptClip.y <rtClip.top)         return HTTOPRIGHT;
			else if(ptClip.y >rtClip.bottom) return HTBOTTOMRIGHT;
			else return HTRIGHT;
		}
		else
		{
			if(ptClip.y <rtClip.top)         return HTTOP;
			else if(ptClip.y >rtClip.bottom) return HTBOTTOM;
			else return HTCLIENT;
		}
	}
#define FR_HOUR_ND      108000L  // frames per hour in non-drop mode
#define FR_HOUR_DROP    107892L  // frames per hour in drop frame mode
#define FR_TENMIN_DROP  17982L   // frames per ten minutes in drop frame
#define FR_MINUTE_DROP  1798L    // frame per minute in drop frame mode
#define FR_MINUTE_ND    1800L    // frames per minute in non-drop mode
#define FR_SECOND       30L      // frames per second, both modes NTSC

#define FR_HOUR_PAL     90000L   // frames per hour in PAL
#define FR_MINUTE_PAL   1500L    // frames per minute in PAL
#define FR_SECOND_PAL   25L      // frames per second in PAL

	DWORD	TP_GetFrameFormTime( int nH, int nM , int nS , int nF,DWORD eVtrFormat)
	{
		switch(eVtrFormat)
		{
		case VIDEO_30M:
			if( (nM%10) != 0 ) {
				if( nS == 0 && (nF == 0 || nF == 1 ) )
					nF = 2;
			}
			return nH*FR_HOUR_DROP + (nM/10)*FR_TENMIN_DROP + (nM%10)*FR_MINUTE_DROP + nS*FR_SECOND + nF;
		case VIDEO_30:
			return nH*FR_HOUR_ND + nM*FR_MINUTE_ND + nS*FR_SECOND + nF;
		case VIDEO_25:
		case VIDEO_SECAM:
			return nH*FR_HOUR_PAL + nM*FR_MINUTE_PAL + nS*FR_SECOND_PAL + nF;
		}
		return 0;
	};

	DWORD	TP_GetFrameFormTimeAddRemove( int nH, int nM , int nS , int nF,DWORD eVtrFormat)//XN00022424 linhao 2009-11-03 add
	{
		switch(eVtrFormat)
		{
		case VIDEO_30M:
			return nH*FR_HOUR_DROP + (nM/10)*FR_TENMIN_DROP + (nM%10)*FR_MINUTE_DROP + nS*FR_SECOND + nF;
		case VIDEO_30:
			return nH*FR_HOUR_ND + nM*FR_MINUTE_ND + nS*FR_SECOND + nF;
		case VIDEO_25:
		case VIDEO_SECAM:
			return nH*FR_HOUR_PAL + nM*FR_MINUTE_PAL + nS*FR_SECOND_PAL + nF;
		}
		return 0;
	};
	



	DWORD  TP_IncreaseFrame( DWORD Frame,int nH, int nM , int nS ,int nF,DWORD eVtrFormat)
	{
		CString strFrame =  TP_StringFromFrame((VIDEO_TYPE)eVtrFormat,Frame,FALSE);	//暂时不处理跨天的时码
		int hour,minute,sec,frm,fps;
		fps  = TP_GetFrameFormTime(0,0,1,0,eVtrFormat) ;
		hour = (strFrame[0]-'0')*10 + (strFrame[1]-'0');
		minute = (strFrame[3]-'0')*10 + (strFrame[4]-'0') ;
		sec = (strFrame[6]-'0')*10 + (strFrame[7]-'0')  ;
		frm = (strFrame[9]-'0')*10 + (strFrame[10]-'0') ;


		if (nF!=0)
		{
			frm += nF;
			if (frm < 0)
			{

				sec -= (fps-frm)/fps;
				frm = (fps+frm)%fps;
				if(sec < 0)
				{
					minute -= ((60-sec)/60);
					sec = (sec+60)%60;
					if (minute < 0)
					{
						hour -= (60-minute)/60;
						minute = (minute+60)%60;
						if (hour < 0)
						{
							hour = 0;
							minute = 0;
							sec = 0;
							frm = 0;
						}
					}
				}
			}
			else if (frm > fps)
			{
				sec += (frm/fps);
				frm = (frm%fps);
				if (sec > 59)
				{		
					minute +=(sec/60);
					sec = (sec%60);
					if (minute > 59)
					{
						hour += minute/60;
						minute = (minute%60);
						if (hour > 23)
						{
							hour = 23;
							minute = 59;
							sec = 59;
							frm = fps - 1;
						}
					}
				}
			}
		}


		if (nS!=0)
		{
			sec += nS;
			if (sec < 0)
			{
				minute -= ((60-sec)/60);
				sec = (sec+60)%60;	
				if (minute < 0)
				{
					hour -= (60-minute)/60;
					minute = (minute+60)%60;
					if (hour < 0)
					{
						hour = 0;
						minute = 0;
						sec = 0;
						frm = 0;
					}
				}

			}
			else if (sec > 59)
			{
				minute +=(sec/60);
				sec = (sec%60);
				if (minute > 59)
				{
					hour += minute/60;
					minute = (minute%60);
					if (hour > 23)
					{
						hour = 23;
						minute = 59;
						sec = 59;
						frm = fps - 1;
					}
				}
			}
		}

		if (nM!=0)
		{
			minute += nM;
			if (minute < 0)
			{
				hour -= (60-minute)/60;
				minute = (minute+60)%60;
				if (hour < 0)
				{
					hour = 0;
					minute = 0;
					sec = 0;
					frm = 0;
				}
			}
			else if (minute > 59)
			{
				hour += minute/60;
				minute = (minute%60);
				if (hour > 23)
				{
					hour = 23;
					minute = 59;
					sec = 59;
					frm = fps - 1;
				}
			}
		}

		if (nH!=0)
		{
			hour += nH;
			if (hour < 0)
			{
				hour = 0;
				minute = 0;
				sec = 0;
				frm = 0;
			}
			else if (hour > 23)
			{
				hour = 23;
				minute = 59;
				sec = 59;
				frm = fps - 1;
			}
		}
		Frame = TP_GetFrameFormTime(hour,minute,sec,frm,eVtrFormat);

		return Frame;
	} 
	void   TP_GetTimeFormFrame( DWORD Frame , int& nH  , int& nM , int& nS , int& nF,DWORD eVtrFormat )
	{	
		DWORD	dwReste, hour, minute;
		BOOL    bDay = (VIDEO_DAY & eVtrFormat) == VIDEO_DAY;
		eVtrFormat &= ~VIDEO_DAY;
		switch(eVtrFormat)
		{
		case VIDEO_30M:
			hour = (Frame/FR_HOUR_DROP);
			dwReste = (Frame%FR_HOUR_DROP);
			minute = 10*(dwReste/FR_TENMIN_DROP);
			dwReste = (dwReste%FR_TENMIN_DROP);
			if( dwReste >= FR_MINUTE_ND ) {
				dwReste  -= FR_MINUTE_ND;
				minute += 1 + (dwReste / FR_MINUTE_DROP);
				dwReste %= FR_MINUTE_DROP;
				dwReste  += 2;
			}
			nH = hour;
			nM = minute;
			nS = (dwReste / FR_SECOND);
			nF = (dwReste % FR_SECOND);
			break;
		case VIDEO_30:
			hour = (Frame/FR_HOUR_ND);
			dwReste = (Frame%FR_HOUR_ND);
			minute = (dwReste/FR_MINUTE_ND);
			dwReste = (dwReste%FR_MINUTE_ND);
			nH = hour;
			nM = minute;
			nS = (dwReste / FR_SECOND);
			nF = (dwReste % FR_SECOND);
			break;
		case VIDEO_25:
		case VIDEO_SECAM:
			hour = (Frame/FR_HOUR_PAL);
			dwReste = (Frame%FR_HOUR_PAL);
			minute = (dwReste/FR_MINUTE_PAL);
			dwReste = (dwReste%FR_MINUTE_PAL);
			nH = hour;
			nM = minute;
			nS = (dwReste / FR_SECOND_PAL);
			nF = (dwReste % FR_SECOND_PAL);
			break;
		default:
			hour = (Frame/FR_HOUR_PAL);
			dwReste = (Frame%FR_HOUR_PAL);
			minute = (dwReste/FR_MINUTE_PAL);
			dwReste = (dwReste%FR_MINUTE_PAL);
			nH = hour;
			nM = minute;
			nS = (dwReste / FR_SECOND_PAL);
			nF = (dwReste % FR_SECOND_PAL);
			break;
		}
		if(!bDay)	nH %= 24;
	};

	int TP_GetMaxFrameOfDay(VIDEO_TYPE eType)
	{
		if(eType == VIDEO_25 ) return TP_GetFrameFromString(eType,_T("23:59:59:25"));
		else                   return TP_GetFrameFromString(eType,_T("23:59:59:30"));
	}
	int TP_GetFrameFromString(VIDEO_TYPE eType,CString strFrame,BOOL bIsAddRemove)    
	{

		int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0;
		if(strFrame.GetLength()< 11) return 0;
		if(strFrame.Find(_T("--")) >= 0) return -1;// lihuili 2010-01-05 如果是--则返回 -1
		strFrame.Replace('-','0');
		dwHour     = (strFrame[0]-'0')*10 + (strFrame[1]-'0') ;
		dwMinute   = (strFrame[3]-'0')*10 + (strFrame[4]-'0') ;
		dwSecond   = (strFrame[6]-'0')*10 + (strFrame[7]-'0') ;
		dwFrame    = (strFrame[9]-'0')*10 + (strFrame[10]-'0') ;

		if(!bIsAddRemove)
		{
			return TP_GetFrameFormTime(dwHour,dwMinute,dwSecond,dwFrame,eType);
		}
		else
		{
			return TP_GetFrameFormTimeAddRemove(dwHour,dwMinute,dwSecond,dwFrame,eType);//XN00022424 linhao 2009-11-03
		}
	}
	__int64 TP_AddReduceFrame(VIDEO_TYPE eType, __int64 nFrameSrc, CString strFrameOffset, BOOL bAdd, DWORD dwFlag)    
	{
		int nFrameOffset = TP_GetFrameFromString(eType, strFrameOffset);
		if (eType != VIDEO_30M)	return nFrameSrc + (bAdd ? nFrameOffset:-nFrameOffset);

		BOOL bAddDay = !(dwFlag&0x01);
		int nAddDay	= 0;
		BOOL bSrcPositive = nFrameSrc>=0;

		if (bAddDay)
		{
			if (nFrameSrc<0)	{ nFrameSrc+=24*FR_HOUR_DROP; ++nAddDay; }
		}
		else
		{
			if ((bSrcPositive&&!bAdd || !bSrcPositive&&bAdd) && _abs64(nFrameSrc)<=_abs64(nFrameOffset))
			{
				__int64 nFrameRet = _abs64(nFrameOffset)-_abs64(nFrameSrc);
				return bSrcPositive ? -nFrameRet : nFrameRet;
			}
			else
			{
				nFrameSrc = _abs64(nFrameSrc);
				if (!bSrcPositive)	bAdd=!bAdd;
			}
		}

		int nH1, nM1, nS1, nF1;
		TP_GetTimeFormFrame(static_cast<DWORD>(nFrameSrc), nH1, nM1, nS1, nF1, eType|VIDEO_DAY);

		if(strFrameOffset.IsEmpty() || strFrameOffset.GetLength()<10) return nFrameSrc;
		strFrameOffset.Replace('-','0');
		int nH2, nM2, nS2, nF2;
		nH2 = (strFrameOffset[0]-'0')*10 + (strFrameOffset[1]-'0') ;
		nM2 = (strFrameOffset[3]-'0')*10 + (strFrameOffset[4]-'0') ;
		nS2 = (strFrameOffset[6]-'0')*10 + (strFrameOffset[7]-'0') ;
		nF2 = (strFrameOffset[9]-'0')*10 + (strFrameOffset[10]-'0') ;

		if(bAdd>0)
		{
			nH1 += nH2;
			nM1 += nM2;
			nS1 += nS2;
			nF1 += nF2;
			if (nF1>29) { ++nS1; nF1-=30; }
			if (nS1>59) { ++nM1; nS1-=60; }
			if (nM1>59) { ++nH1; nM1-=60; }
			if (nM1%10!=0 && nS1==0 && nF1<2)	nF1=2;
		}
		else
		{
			nF1-=nF2;
			nH1-=nH2;
			nM1-=nM2;
			nS1-=nS2;
			if (nF1<0)	{ --nS1; nF1+=30; }
			if (nS1<0)  { --nM1; nS1+=60; }
			if (nM1<0)  { --nH1; nM1+=60; }
			if (nH1<0)
			{
				if (bAddDay)	{ nH1+=24; ++nAddDay; }
				else			ASSERT(0);
			}

			if (nM1%10!=0 && nS1==0 && nF1<2)
			{
				--nS1; nF1=29;
				if (nS1<0)  { --nM1; nS1+=60; }
				if (nM1<0)  { --nH1; nM1+=60; }
				if (nH1<0)
				{
					if (bAddDay)	{ nH1+=24; ++nAddDay; }
					else			ASSERT(0);
				}
			}
		}

		__int64 nFrameRet = static_cast<__int64>(TP_GetFrameFormTime(nH1, nM1, nS1, nF1, eType));
		if (!bAddDay)	return bSrcPositive ? nFrameRet : -nFrameRet;
		else			return nFrameRet-nAddDay*24*FR_HOUR_DROP;
	}
	CString  TP_GetTimeString(SYSTEMTIME &tTime, BOOL bData, BOOL bTime)
	{
		CString sText = _T("");
		TCHAR szDataTime[MAX_PATH];
		if(bData)
		{
			GetDateFormat(						  // 获取日期字符串
				LOCALE_USER_DEFAULT,  // the locale for which the formatting is
				DATE_SHORTDATE,        // date format (long, short, ...)
				&tTime,               // the date to be formatted (here current
				NULL,                 // style of date format
				szDataTime,           // output buffer
				MAX_PATH              // size of output buffer
				);
			sText += szDataTime;
		}
		if(bTime)
		{

			GetTimeFormat(						  // 获取时间字符串
				LOCALE_USER_DEFAULT,  // the locale for which the formatting is
				0,  // date format (long, short, ...)
				&tTime,               // the date to be formatted (here current
				NULL,                 // style of date format
				szDataTime,           // output buffer
				MAX_PATH              // size of output buffer
				);
			if(bData) sText += _T(" ");
			sText += szDataTime;
		}
		return sText;
	}
	//long       TP_GetTimeRate(DWORD eVideoStandard)
	//{
	//	/*INT  stuTimeBase;
	//	stuTimeBase = TP_GetSysTimebase((TP_VIDEO_STANDARD)eVideoStandard);
	//	return stuTimeBase.lTimeRate;	*/
	//	re
	//}
	//VIDEO_TYPE TP_GetVideoType(DWORD eVideoStandard,BOOL bDropFrame)
	//{
	//	switch(eVideoStandard)
	//	{	
	//	case	TP_VIDEOSTANDARD_PAL:				
	//	case	TP_VIDEOSTANDARD_1920_1080_50i:					
	//	case	TP_VIDEOSTANDARD_1920_1080_25p:						
	//	case	TP_VIDEOSTANDARD_1440_1080_50i:		
	//	case    TP_VIDEOSTANDARD_PAL_16_9: //jw
	//		return VIDEO_25;
	//	case	TP_VIDEOSTANDARD_NTSC_2997:						
	//	case	TP_VIDEOSTANDARD_1920_1080_5994i:					
	//	case	TP_VIDEOSTANDARD_1920_1080_2997p:					
	//	case	TP_VIDEOSTANDARD_1440_1080_5994i:		
	//	case    TP_VIDEOSTANDARD_NTSC_2997_16_9:
	//		if(bDropFrame)	return VIDEO_30M;
	//		else            return VIDEO_30;
	//	case	TP_VIDEOSTANDARD_NTSC_30:							
	//	case	TP_VIDEOSTANDARD_1920_1080_60i:						
	//	case	TP_VIDEOSTANDARD_1440_1080_60i:						
	//	case	TP_VIDEOSTANDARD_1920_1080_30p:		
	//	case    TP_VIDEOSTANDARD_NTSC_30_16_9:
	//		return VIDEO_30;
	//	}	
	//	return VIDEO_25;
	//}


	CString TP_GetVKName(UINT uChar,UINT uFlag)
	{  
		CString sText;
#ifdef TP_NS_EDIT
		if(uChar>=4000)                    return _T(""); //不是键盘上的快捷键,不显示 //20071211
		if(uFlag & TP_CONTROL) sText += _T("Ctrl");
		if(uFlag & TP_SHIFT)   {if(uFlag & TP_CONTROL)            sText += _T("+");sText += _T("Shift");}
		if(uFlag & TP_ALT)     {if(uFlag & (TP_CONTROL|TP_SHIFT)) sText += _T("+");sText += _T("Alt");}
#else
		if(uFlag & TP_CONTROL) sText += _T("CTL");
		if(uFlag & TP_SHIFT)   {if(uFlag & TP_CONTROL)            sText += _T("+");sText += _T("SHT");}
		if(uFlag & TP_ALT)     {if(uFlag & (TP_CONTROL|TP_SHIFT)) sText += _T("+");sText += _T("ALT");}
#endif
		if(uChar<=0) return sText;
		short   sTemp  = 0;
		if(uChar>=VK_NUMPAD0&&uChar<=VK_NUMPAD9  )	                    sTemp  =0;	
		else if(uChar>=VK_PRIOR&&uChar<=VK_HELP || uChar == VK_NUMLOCK)	sTemp  =65; 	
		TCHAR   lptStr[256];
		short   sScanValue=   MapVirtualKey(uChar, 0); 
		if(sScanValue <=0) return  sText;	
		else
		{	
			LPARAM lTemp      =0;
			WORD   wTempLow   =1; 
			WORD   wTempHigh  =MAKEWORD(sScanValue,sTemp);
			lTemp             =MAKELPARAM(wTempLow,wTempHigh );
			GetKeyNameText((long)lTemp,lptStr,256);  
		}

		if(uFlag & (TP_CONTROL|TP_ALT|TP_SHIFT)) sText += _T("+");
		return  sText + lptStr;  
	}

	__int64     TP_FrameFromDay(VIDEO_TYPE enVideoType,int iDay)   
	{
		switch(enVideoType)
		{
		case VIDEO_30M :
			return TP_GetFrameFormTime(24,0,0,0,enVideoType) *iDay;
		case VIDEO_30:
			return 24*3600*30*iDay;		
		case VIDEO_25:
			return 24*3600*25*iDay;
		default:
			return 24*3600*25*iDay;
		}
		return 0;
	}
	CString TP_StringFromFrame(VIDEO_TYPE enVideoType,__int64 iFrame,BOOL bData)   
	{
		CString strTemp;
		if(iFrame <0)
		{
#ifdef TP_NS_EDIT
			if(VIDEO_30M == enVideoType)	strTemp.Format(_L("--:--.--:--"));
			else 	                        strTemp.Format(_L("--:--:--:--"));
#else
			if(VIDEO_30M == enVideoType)	strTemp.Format(_L("--:--.--:--"));
			else 	                        strTemp.Format(_L("--:--:--:--"));
#endif
		}
		else
		{
			int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0,dwDay = 0;
			TP_GetTimeFormFrame((DWORD)iFrame,dwHour,dwMinute,dwSecond,dwFrame,enVideoType);
			CString sDay;
			switch(enVideoType)
			{
			case VIDEO_30M :		
				dwDay   = (DWORD)(iFrame * 1001/30000)/3600/24;		
				break;
			case VIDEO_25 :
				dwDay    = (DWORD)(iFrame/25)/3600/24;
				break;
			case VIDEO_30:
				dwDay   = (DWORD)(iFrame/30)/3600/24;
				break;
			default:
				dwDay    = (DWORD)(iFrame/25)/3600/24;
				break;
			} 
#ifdef TP_NS_EDIT
			if(VIDEO_30M == enVideoType)	strTemp.Format(_L("%02d:%02d.%02d:%02d"),(long)dwHour,(long)dwMinute,(long)dwSecond,(long)dwFrame);
			else 	                    strTemp.Format(_L("%02d:%02d:%02d:%02d"),(long)dwHour,(long)dwMinute,(long)dwSecond,(long)dwFrame);
#else
			if(VIDEO_30M == enVideoType)	strTemp.Format(_L("%02d:%02d.%02d:%02d"),(long)dwHour,(long)dwMinute,(long)dwSecond,(long)dwFrame);
			else 	                    strTemp.Format(_L("%02d:%02d:%02d:%02d"),(long)dwHour,(long)dwMinute,(long)dwSecond,(long)dwFrame);
#endif
			if(bData && dwDay>0){ sDay.Format(_L("%02d:"),(long)dwDay);strTemp = sDay + strTemp;}
		}
		return (LPTSTR)(LPCTSTR)strTemp;
	}
	CString TP_StringFromFrameMin(VIDEO_TYPE enVideoType,__int64 iFrame,BOOL bData)   
	{
		CString strTemp;
		if(iFrame <0)
		{
			if(VIDEO_30M == enVideoType)	strTemp.Format(_L("-:-.-:-"));
			else 	                        strTemp.Format(_L("-:-:-:-"));
		}
		else
		{
			int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0,dwDay = 0;
			TP_GetTimeFormFrame((DWORD)iFrame,dwHour,dwMinute,dwSecond,dwFrame,enVideoType);
			CString sDay;
			switch(enVideoType)
			{
			case VIDEO_30M :		
				dwDay   = (DWORD)(iFrame * 1001/30000)/3600/24;		
				break;
			case VIDEO_25 :
				dwDay    = (DWORD)(iFrame/25)/3600/24;
				break;
			case VIDEO_30:
				dwDay   = (DWORD)(iFrame/30)/3600/24;
				break;
			default:
				dwDay    = (DWORD)(iFrame/25)/3600/24;
				break;
			} 
			if(VIDEO_30M == enVideoType)	strTemp.Format(_L("%02d:%02d.%02d:%02d"),(long)dwHour,(long)dwMinute,(long)dwSecond,(long)dwFrame);
			else 	                        strTemp.Format(_L("%02d:%02d:%02d:%02d"),(long)dwHour,(long)dwMinute,(long)dwSecond,(long)dwFrame);
			while (strTemp.GetLength()>3 && strTemp.Mid(0, 2) == _L("00")) strTemp = strTemp.Mid(3);
			if (strTemp.Mid(0, 1) == _L("0")) strTemp = strTemp.Mid(1);
		}
		return (LPTSTR)(LPCTSTR)strTemp;
	}
	void  TP_DrawTextInRect(CDC *pDC, CString szString, LPRECT lpRect, UINT uFormat)
	{
		lpRect ->top +=2;	
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		int tmpWidth=tm.tmAveCharWidth, tmpHeight=tm.tmHeight;
		int nMaxLineChar = abs(lpRect->right - lpRect->left) / tmpWidth ; 
		short theLineLength=0; 
		unsigned short halfChinese=0;

		for(int i=0; i<=szString.GetLength()-1; i++)
		{
			if(((unsigned char)szString.GetAt(i) == 0x0d) && ((unsigned char)szString.GetAt(i+1) == 0x0a))
				theLineLength=0;
			if((unsigned char)szString.GetAt(i) >= 0xA1)
				halfChinese++;
			theLineLength++;

			if(theLineLength > nMaxLineChar)
			{
				if(halfChinese%2)
				{
					szString.Insert(i,(unsigned char)0x0a);
					szString.Insert(i,(unsigned char)0x0d);
				}
				else
				{
					szString.Insert(i-1,(unsigned char)0x0a);
					szString.Insert(i-1,(unsigned char)0x0d);
				}

				theLineLength = 0;
			}
		}	
		pDC->DrawText(szString, lpRect,uFormat);	
	}
	BOOL        TP_PathIsValidate(CString &sFile)
	{
		CString sInvalidChar = WC_NAMEEXCLUDE;
		if(sFile.GetLength()<=0) return FALSE;
		if(sFile[1] !=':' && sFile[1] !='\\') return FALSE;
		for(INT l=0;l<sInvalidChar.GetLength();l++)
		{
			for(INT k=2;k<sFile.GetLength();k++)
			{
				if(sFile[k] == sInvalidChar[l]) return FALSE;
			}
		}	
		return TRUE;
	}
	LPCTSTR  TP_GetSoftWareVersion()
	{
		if(CTPBaseFrame::m_pMainFrameCmd)
		{
			g_sTemp.Empty();
			 CTPBaseFrame::m_pMainFrameCmd(0, (LPARAM)&g_sTemp);
			 return g_sTemp;
		}
		else return NULL;
	}
	BOOL TP_CreateDirectory(CString sPath)
	{	
	/*	int iPos = sPath.Find('\\');
		while (iPos!=-1)
		{
			CString  sTemp = sPath.Mid(0,iPos);
			if(!PathFileExists(sTemp)) 
			{
				if(!CreateDirectory(sTemp,NULL)) return FALSE;
			}
			iPos =sPath.Find('\\',iPos+1);
		}
		if(!PathFileExists(sPath)) 
		{
			if(!CreateDirectory(sPath,NULL)) return FALSE;
		}
		return TRUE;*/
		// lihuili 2010-10-26 XN0029113 网络路径创建不正确
		UINT uRet = SHCreateDirectory(NULL, sPath);
		if(ERROR_SUCCESS == uRet) return TRUE;
		if(ERROR_FILE_EXISTS == uRet) return TRUE;
		if(ERROR_ALREADY_EXISTS == uRet) return TRUE;
		
		return FALSE;
	}
	BOOL TP_IsCtrlHotKeyHelp(UINT uKey)
	{
		UINT uFlag=0;
		if(GetKeyState(VK_SHIFT)<0)
			uFlag |= TP_SHIFT;
		if(GetKeyState(VK_CONTROL)<0)
			uFlag |= TP_CTRL;
		if(GetKeyState(VK_MENU)<0)
			uFlag |= TP_MENU;
		if(g_pGlobalHotKey == NULL || g_lGlobalHotKey<=0) return FALSE;
		for(INT l=0;l<g_lGlobalHotKey;l++)
			if(g_pGlobalHotKey[l].dwIdentifer ==g_lGlobalHotKeyHelp&&
				g_pGlobalHotKey[l].vkChar ==uKey &&
				g_pGlobalHotKey[l].uFlag==uFlag) 
				return TRUE;
		return FALSE;
	}
	BOOL TP_DeleteDirectory(CString sPath,BOOL bRoot)
	{
		CStringArray aPath;
		TP_FindFile(sPath,_L("\\*.*"), FALSE,FALSE,aPath);
		for(INT_PTR l=0;l<aPath.GetSize();l++)
		{
			if(!DeleteFile(aPath[l])) return FALSE;
		}
		aPath.RemoveAll();
		TP_FindFile(sPath,_L("\\*.*"), TRUE,FALSE,aPath);
		for(INT_PTR l=0;l<aPath.GetSize();l++) 
		{
			BOOL bReturn = TP_DeleteDirectory(aPath[l],TRUE);
			if(bReturn == FALSE) return FALSE;
		}	
		BOOL bReturn = TRUE;
		if(bRoot) bReturn = ::RemoveDirectory(sPath);	
		return bReturn;
	}
	/*
	void   TP_FindFile(CString strPath,CString strFile,BOOL bPath,BOOL bRecursion,CStringArray &aFile)
	{
	WIN32_FIND_DATA findData;
	HANDLE          hFind   = FindFirstFile(strPath + strFile ,&findData);	
	if(hFind != INVALID_HANDLE_VALUE)
	{
	if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
	{
	if(bPath) aFile.Add(strPath + _L("\\") + findData.cFileName);
	if(bRecursion)  TP_FindFile(strPath + _L("\\") + findData.cFileName,_L("\\*.*"),bPath,bRecursion,aFile);
	}
	else if(!bPath && !((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
	aFile.Add(strPath + _L("\\") + findData.cFileName);

	while(FindNextFile(hFind,&findData))
	{
	if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0] != '.')
	{
	if(bPath) aFile.Add(strPath + _L("\\") + findData.cFileName);
	if(bRecursion)  TP_FindFile(strPath + _L("\\") + findData.cFileName,_L("\\*.*"),bPath,bRecursion,aFile);
	}
	else if(!bPath && !((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
	aFile.Add(strPath + _L("\\") + findData.cFileName);
	}
	FindClose(hFind);
	}
	}
	*/

	LRESULT  TP_CheckRename(CString sName,BOOL bMessageBox)
	{
		if(sName.Find('*')>=0 ||sName.Find('/')>=0||sName.Find('\\')>=0||sName.Find('<')>=0||sName.Find('>')>=0
			||sName.Find('|')>=0||sName.Find('?')>=0||sName.Find(':')>=0||sName.Find('"')>=0||sName.Find('&')>=0)
		{

			AfxMessageBox(CTPBaseCtrl::m_stuSBDictionary.SBLoadString(_T("IDS_COMMON_CHARACTERNOTE")));
			return S_FALSE;
		}
		return S_OK;
	}
	CString TP_GetValidName(CString sName)
	{
		CString sRet;
		sName.Replace('*','-'); sName.Replace('/','-');sName.Replace('\\','-'); sName.Replace('<','-');
		sName.Replace('>','-'); sName.Replace('|','-');sName.Replace('?','-');  sName.Replace(':','-');
		sName.Replace('"','-');	sName.Replace('&','-');sName.Replace(',','-');
		sName.Trim();
		return sName;
	}
	void  TP_InitVld()
	{ 
#ifdef _DEBUG
 	//	VLDEnable();
#endif

	}
	void  TP_ReleaseVld()
	{

#ifdef _DEBUG
 //		VLDDisable();
 #endif
 
	}
 
	long TP_GetMMTimeMinSecond()
	{
		MMTIME mn;
		mn.wType = 	TIME_MS ;
		timeGetSystemTime(&mn,sizeof(MMTIME));
		return mn.u.ms;
	}
	//void     TP_GetViewScreenSize(DWORD  eStd,CPoint &pt)
	//{
	//	TP_VIDEO_STANDARD eSet = (TP_VIDEO_STANDARD)eStd;

	//	switch(eSet) 
	//	{
	//	case TP_VIDEOSTANDARD_UNKNOW:
	//		{
	//			pt = CPoint(720,576);
	//			break;
	//		}
	//	case TP_VIDEOSTANDARD_PAL:
	//		{
	//			pt = CPoint(720,576);
	//			break;
	//		}
	//	case TP_VIDEOSTANDARD_NTSC_2997:
	//	case TP_VIDEOSTANDARD_NTSC_30:
	//		{
	//			pt = CPoint(720,480);
	//			break;
	//		}
	//	case TP_VIDEOSTANDARD_SECAM:
	//	case TP_VIDEOSTANDARD_1920_1080_50i:
	//	case TP_VIDEOSTANDARD_1920_1080_5994i:
	//	case TP_VIDEOSTANDARD_1920_1080_60i:
	//	case TP_VIDEOSTANDARD_1920_1080_2398p:
	//	case TP_VIDEOSTANDARD_1920_1080_24p:
	//	case TP_VIDEOSTANDARD_1920_1080_25p:
	//	case TP_VIDEOSTANDARD_1920_1080_2997p:
	//	case TP_VIDEOSTANDARD_1920_1080_30p:
	//	case TP_VIDEOSTANDARD_1280_720_2398p:
	//	case TP_VIDEOSTANDARD_1280_720_24p:
	//	case TP_VIDEOSTANDARD_1280_720_50p:
	//	case TP_VIDEOSTANDARD_1280_720_5994p:
	//	case TP_VIDEOSTANDARD_1440_1080_50i:
	//	case TP_VIDEOSTANDARD_1440_1080_5994i:
	//	case TP_VIDEOSTANDARD_1440_1080_60i:
	//	default:
	//		{
	//			pt = CPoint(1920,1080);
	//			break;
	//		}
	//	}
	//}
	void TP_GetProperImageSize(CRect rtShow,int iWidth,int iHeight,CRect &rtRight)
	{
		rtRight = rtShow;
		if(iWidth == 0 || iHeight == 0) return;

		float ftemp  = (float)(iWidth *1.0/iHeight);
		float ftemp2 = (float)(rtShow.Width()*1.0/rtShow.Height());
		if(ftemp-ftemp2>0.00001)
		{
			float lHeight    = (float)( rtShow.Width() * iHeight*1.0/iWidth);
			rtRight.top      = (long)(float)( rtShow.CenterPoint().y - lHeight/2);
			rtRight.bottom = (long)(rtRight.top + lHeight);
			//Wu : 2006_8_14
			if(rtRight.top    < rtShow.top)        rtRight.top      = rtShow.top;
			if(rtRight.bottom > rtShow.bottom)     rtRight.bottom   = rtShow.bottom;
		}
		else
		{
			float lWidth     = (float)( rtShow.Height() * iWidth*1.0/ iHeight);
			rtRight.left   = (long)(rtShow.CenterPoint().x - lWidth/2);
			rtRight.right  =(long)( rtRight.left + lWidth);
			//Wu : 2006_8_14
			if(rtRight.left   < rtShow.left)         rtRight.left   = rtShow.left;
			if(rtRight.right  > rtShow.right)        rtRight.right  = rtShow.right;
		}
	}

	void    TP_MessageShow(CStringArray &aError)
	{
		if(aError.GetSize()>0)
		{
			CString sTemp;
			for(INT l=0;l<aError.GetSize();l++) sTemp += aError[l] + _L("\n");
			if(sTemp.GetLength()>0) AfxMessageBox(sTemp);
		}
		aError.RemoveAll();
		return ;
	}
	LPVOID g_pCommonData[7] = {0,0,0,0,0,0,0};
	//6 Out put
	void     TP_SetCommonData(int iData,void *pData)
	{
		// 0 EncodeByX1
		if(iData<0 || iData>7) return;
		g_pCommonData[iData] = pData;
	}
	BOOL     TP_GetCommonData(int iData,void *&pData)
	{
		pData = NULL;
		if(iData<0 || iData>7) return FALSE;
		ASSERT(iData != 2);
		pData = g_pCommonData[iData];
		return TRUE;
	}
	HWND     TP_GetTopFrame(HWND hWnd)
	{
		DWORD dwStype = 0;
		while(hWnd != NULL)
		{
			dwStype = (DWORD)GetWindowLong(hWnd, GWL_STYLE);
			if(dwStype & WS_POPUP) return hWnd;
			hWnd = GetParent(hWnd);
		}
		return NULL;
	}
	BOOL   TP_GetValidateWnd(HWND hWnd)
	{
		if(hWnd == NULL) return FALSE;
		DWORD dwPIDAct = 0L;
		GetWindowThreadProcessId( hWnd, &dwPIDAct );
		if ( hWnd && dwPIDAct != GetCurrentProcessId() ) return FALSE;
		return TRUE;
	}

	float *g_pfRY,*g_pfGY,*g_pfBY,*g_pfRU,*g_pfGU,*g_pfBU,*g_pfRV,*g_pfGV,*g_pfBV;
	void TP_CreateFloatTable()
	{
		if(g_pfRY != NULL) return;
		g_pfRY = (float*)_mm_malloc(sizeof(float)*256, 16);
		g_pfGY = (float*)_mm_malloc(sizeof(float)*256, 16);
		g_pfBY = (float*)_mm_malloc(sizeof(float)*256, 16);
		g_pfRU = (float*)_mm_malloc(sizeof(float)*256, 16);
		g_pfGU = (float*)_mm_malloc(sizeof(float)*256, 16);
		g_pfBU = (float*)_mm_malloc(sizeof(float)*256, 16);
		g_pfRV = (float*)_mm_malloc(sizeof(float)*256, 16);
		g_pfGV = (float*)_mm_malloc(sizeof(float)*256, 16);
		g_pfBV = (float*)_mm_malloc(sizeof(float)*256, 16);

		g_pfRY[0] = g_pfGY[0] = g_pfBY[0] = 0.0f;
		g_pfRU[0] = g_pfGU[0] = g_pfBU[0] = 0.0f;
		g_pfRV[0] = g_pfGV[0] = g_pfBV[0] = 0.0f;
		for(int i=1; i<256; i++)
		{
			g_pfRY[i] = 0.257f + g_pfRY[i-1];
			g_pfGY[i] = 0.504f + g_pfGY[i-1];
			g_pfBY[i] = 0.09813f + g_pfBY[i-1];

			g_pfRU[i] = -0.148f + g_pfRU[i-1];
			g_pfGU[i] = -0.291f + g_pfGU[i-1];
			g_pfBU[i] = 0.439f + g_pfBU[i-1];

			g_pfRV[i] = 0.439f + g_pfRV[i-1];
			g_pfGV[i] = -0.368f + g_pfGV[i-1];
			g_pfBV[i] = -0.071f + g_pfBV[i-1];
		}
	}

	/************************************************************************/
	/* 释放加速表
	/************************************************************************/
	void TP_DestoryFloatTable()
	{
		if(g_pfRY == NULL) return;
		_mm_free(g_pfRY);
		g_pfRY = NULL;
		_mm_free(g_pfGY);
		g_pfGY = NULL;
		_mm_free(g_pfBY);
		g_pfBY = NULL;
		_mm_free(g_pfRU);
		g_pfRU = NULL;
		_mm_free(g_pfGU);
		g_pfGU = NULL;
		_mm_free(g_pfBU);
		g_pfBU = NULL;
		_mm_free(g_pfRV);
		g_pfRV = NULL;
		_mm_free(g_pfGV);
		g_pfGV = NULL;
		_mm_free(g_pfBV);
		g_pfBV = NULL;
	}
	/************************************************************************/
	/* 转换24位RGB到YUYV格式,并拆场
	/************************************************************************/

	void  TP_ConvertRGB2YUVField(IN OUT BYTE* pDest,
		IN const int nDestPitch,
		IN  BYTE* pSrcRGB,
		IN const int nSrcPitch,
		IN int nWidth,			// Src的宽高(像素个数)
		IN int nHeight)		
	{
		pSrcRGB = pSrcRGB + (nHeight-1) * nSrcPitch;

		/************************************************************************/
		/* 利用查表法，如 R*4+浮点表起点, 取出该地址对应的浮点值
		/************************************************************************/
		//	_declspec(align(16)) float fhalf[4] = {0.5f, 0.5f, 0.5f, 0.5f};
		_declspec(align(16)) DWORD dwY[4] = {0x10101010, 0x10101010, 0x10101010, 0x10101010};
		_declspec(align(16)) DWORD dwU[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
		int nDest = nDestPitch - (nWidth<<1);
		int nSrc = nSrcPitch;//- nWidth*3;
		__asm {
			mov eax,pSrcRGB
				mov edi,pDest
				xor ecx, ecx
loopHeight2:
			inc ecx
				cmp ecx,nHeight
				jg	skipLoopHeight2

				push eax
				//		push edi
				//		push esi
				push ecx
				xor ecx, ecx
loopWidth4:
			add ecx, 4
				cmp ecx, nWidth
				jg skipLoopWidth4

				movdqu xmm0,[eax]
				pxor xmm1,xmm1
					movdqa xmm7, xmm0
					movdqa xmm6, xmm0
					movdqa xmm5, xmm0
					punpcklbw xmm5, xmm1
					punpcklwd xmm5, xmm1		// 000R1000G1000B1
					psrldq xmm0, 3
					punpcklbw xmm0, xmm1
					punpcklwd xmm0, xmm1		// 000R2000G2000B2
					psrldq xmm7, 6
					punpcklbw xmm7, xmm1
					punpcklwd xmm7, xmm1		// 000R3000G3000B3
					psrldq xmm6, 9
					punpcklbw xmm6, xmm1
					punpcklwd xmm6, xmm1		// 000R4000G4000B4
					// 拼接
					movdqa xmm2, xmm5
					movdqa xmm3, xmm7
					punpckhdq xmm2, xmm0		// 000R2000R1
					punpckhdq xmm3, xmm6		// 000R4000R3
					punpcklqdq xmm2, xmm3		// xmm2=000R4000R3000R2000R1
					punpckldq xmm5, xmm0		// 000B2000B1
					punpckldq xmm7, xmm6		// 000B4000B3
					movdqa xmm4, xmm5
					punpcklqdq xmm5, xmm7		// xmm5=000B4000B3000B2000B1
					punpckhqdq xmm4, xmm7		// xmm4=000G4000G3000G2000G1
					// 查表,求Y
					pslld xmm2, 2
					pslld xmm4, 2
					pslld xmm5, 2
					movdq2q mm0, xmm4
					movdqa xmm3, xmm4
					psrldq xmm3, 8
					movdq2q mm1, xmm3
					punpckldq mm0, mm1			// mm0 = 000G3000G1
					movdq2q mm1, xmm5
					movdqa xmm3, xmm5
					psrldq xmm3, 8
					movdq2q mm2, xmm3			
					punpckldq mm1, mm2			// mm1 = 000B3000B1
					movdq2q mm2, xmm2
					movdqa xmm3, xmm2
					psrldq xmm3, 8
					movdq2q mm3, xmm3			
					punpckldq mm2, mm3			// mm2 = 000R3000R1

					mov esi, g_pfRY
					movd xmm1, esi
					punpckldq xmm1, xmm1
					punpcklqdq xmm1, xmm1
					paddd xmm2, xmm1
					mov esi, g_pfGY
					movd xmm1, esi
					punpckldq xmm1, xmm1
					punpcklqdq xmm1, xmm1
					paddd xmm4, xmm1
					mov esi, g_pfBY
					movd xmm1, esi
					punpckldq xmm1, xmm1
					punpcklqdq xmm1, xmm1
					paddd xmm5, xmm1
					// 取出对应的值
					movdqa xmm7, xmm2
					movd esi, xmm7
					movss xmm0, [esi]
					psrldq xmm7, 4
						movd esi, xmm7
						movss xmm3, [esi]
						punpckldq xmm0, xmm3
							psrldq xmm7, 4
							movd esi, xmm7
							movss xmm3, [esi]
							psrldq xmm7, 4
								movd esi, xmm7
								movss xmm6, [esi]
								punpckldq xmm3, xmm6
									punpcklqdq xmm0, xmm3		// xmm0=R对应的float

									movdqa xmm7, xmm4
									movd esi, xmm7
									movss xmm3, [esi]
									psrldq xmm7, 4
										movd esi, xmm7
										movss xmm6, [esi]
										punpckldq xmm3, xmm6
											psrldq xmm7, 4
											movd esi, xmm7
											movss xmm6, [esi]
											psrldq xmm7, 4
												movd esi, xmm7
												movss xmm7, [esi]
												punpckldq xmm6, xmm7
													punpcklqdq xmm3, xmm6		// xmm3=G对应的float

													movdqa xmm7, xmm5
													movd esi, xmm7
													movss xmm2, [esi]
													psrldq xmm7, 4
														movd esi, xmm7
														movss xmm4, [esi]
														punpckldq xmm2, xmm4
															psrldq xmm7, 4
															movd esi, xmm7
															movss xmm4, [esi]
															psrldq xmm7, 4
																movd esi, xmm7
																movss xmm5, [esi]
																punpckldq xmm4, xmm5
																	punpcklqdq xmm2, xmm4		// xmm2=B对应的float

																	addps xmm0, xmm3
																	addps xmm0, xmm2			// xmm0=fR+fB+fG  4个Y
																	cvttps2dq xmm0, xmm0			// 转成整数
																	packssdw xmm0, xmm0
																	packuswb xmm0, xmm0
																	paddusb xmm0, dwY

																	movq  mm3, mm0				// g
																	movq  mm4, mm1				// b
																	movq  mm5, mm2				// r
																	mov esi, g_pfRU
																	movd mm6, esi
																	punpckldq mm6, mm6
																	paddd mm2, mm6
																	mov esi, g_pfGU
																	movd mm6, esi
																	punpckldq mm6, mm6
																	paddd mm0, mm6
																	mov esi, g_pfBU
																	movd mm6, esi
																	punpckldq mm6, mm6
																	paddd mm1, mm6
																	// 取出对应的值
																	movq mm6, mm2
																	movd esi, mm6
																	movss xmm2, [esi]
																	psrlq mm6, 32
																		movd esi, mm6
																		movss xmm3, [esi]
																		punpckldq xmm2, xmm3	// xmm2=R对应的float

																			movq mm6, mm0
																			movd esi, mm6
																			movss xmm3, [esi]
																			psrlq mm6, 32
																				movd esi, mm6
																				movss xmm4, [esi]
																				punpckldq xmm3, xmm4	// xmm3=G对应的float

																					movq mm6, mm1
																					movd esi, mm6
																					movss xmm4, [esi]
																					psrlq mm6, 32
																						movd esi, mm6
																						movss xmm5, [esi]
																						punpckldq xmm4, xmm5	// xmm4=B对应的float

																							addps xmm2, xmm3
																							addps xmm2, xmm4		// xmm2=fR+fB+fG  2个U
																							cvttps2dq xmm2, xmm2		// 转成整数
																							packssdw xmm2, xmm2
																							packsswb xmm2, xmm2
																							paddb xmm2, dwU

																							mov esi, g_pfRV
																							movd mm6, esi
																							punpckldq mm6, mm6
																							paddd mm5, mm6
																							mov esi, g_pfGV
																							movd mm6, esi
																							punpckldq mm6, mm6
																							paddd mm3, mm6
																							mov esi, g_pfBV
																							movd mm6, esi
																							punpckldq mm6, mm6
																							paddd mm4, mm6
																							// 取出对应的值
																							movd esi, mm5
																							movss xmm3, [esi]
																							psrlq mm5, 32
																								movd esi, mm5
																								movss xmm4, [esi]
																								punpckldq xmm3, xmm4	// xmm3=R对应的float

																									movd esi, mm3
																									movss xmm4, [esi]
																									psrlq mm3, 32
																										movd esi, mm3
																										movss xmm5, [esi]
																										punpckldq xmm4, xmm5	// xmm4=G对应的float

																											movd esi, mm4
																											movss xmm5, [esi]
																											psrlq mm4, 32
																												movd esi, mm4
																												movss xmm6, [esi]
																												punpckldq xmm5, xmm6	// xmm5=B对应的float

																													addps xmm3, xmm4
																													addps xmm3, xmm5		// xmm3=fR+fB+fG  2个V
																													cvttps2dq xmm3, xmm3		// 转成整数
																													packssdw xmm3, xmm3
																													packsswb xmm3, xmm3
																													paddb xmm3, dwU
																													punpcklbw xmm2, xmm3	// VUVU
																													punpcklbw xmm0, xmm2	// VYUYVYUY
																													movdq2q mm7, xmm0
																													movq [edi], mm7

																													add eax, 12
																													add esi, 8
																													add edi, 8
																													jmp loopWidth4
skipLoopWidth4:
																												sub ecx, 4
																													cmp ecx, nWidth
																													jge skipLoopWidth2

skipLoopWidth2:

																												pop ecx
																													add edi, nDest
																													pop eax
																													sub eax, nSrc
																													jmp loopHeight2
skipLoopHeight2:
																												emms
		}
	}

}
