//

#include "stdafx.h"

#include "TPCommonCtrlData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



namespace _tp_commonctrl
{
void TP_AssembledHotKeyControlData(int iCtrlData,TPControlData *pCtrlData,int iHotKey,TPHotKeyData *pHotKey)
{
	if(pHotKey)
	{	
		for(INT k=0;k<iHotKey;k++)
		{
			if(pHotKey[k].vkCharSet==0)			pHotKey[k].vkCharSet = pHotKey[k].vkChar;
			if(pHotKey[k].uFlagSet==0)			pHotKey[k].uFlagSet  = pHotKey[k].uFlag;		

			lstrcpyn(pHotKey[k].sName,TP_LoadStr(pHotKey[k].sName),30);
			lstrcpyn(pHotKey[k].sDescrip,TP_LoadStr(pHotKey[k].sDescrip),60);
		}
	}
	if(pCtrlData)
	{	
		for(INT l=0;l<iCtrlData;l++)
		{
			if(pCtrlData[l].rtCtrlSet.bottom ==0)    pCtrlData[l].rtCtrlSet  = pCtrlData[l].rtCtrl;
			if(pCtrlData[l].dwAlignSet == 0)         pCtrlData[l].dwAlignSet = pCtrlData[l].dwAlign;

			lstrcpyn(pCtrlData[l].sNotifyUp,TP_LoadStr(pCtrlData[l].sNotifyUp),80);
			lstrcpyn(pCtrlData[l].sNotifyDown,TP_LoadStr(pCtrlData[l].sNotifyDown),80);
		}
	}
	if(pCtrlData && pHotKey)
	{
		for(INT l=0;l<iCtrlData;l++)
		{
			for(INT k=0;k<iHotKey;k++)
			{
				if(pCtrlData[l].dwIndentifer == pHotKey[k] .dwIndentify)
				{
					pCtrlData[l] .pHotKeyData = &pHotKey[k];
					if(lstrlen(pHotKey[k].sName)<=0) lstrcpyn(pHotKey[k].sName,pCtrlData[l] .sNotifyUp,30);
				}
			}
		}
	}

}

}