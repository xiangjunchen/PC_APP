//MessageRange 
//IDRange     
#pragma once


#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

enum EXTERACTRL_RMENU_INDEX
{
	EXTERACTRL_RMENU_CLASSCASEBAR,
	
};


AFX_EXT_API int    TPX_DBToPos(double dDB);
AFX_EXT_API int    TPX_ValueToPos(int iValue,WORD wBitsPerSample = 16);
AFX_EXT_API double TPX_ValueToDB(double iValue,WORD wBitsPerSample = 16);