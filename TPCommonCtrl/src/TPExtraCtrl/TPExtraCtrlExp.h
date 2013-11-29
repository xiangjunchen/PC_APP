//MessageRange 
//IDRange     
#pragma once

#include "TPExtraCtrlHead.h"


#define WM_SPLITTERMOVE    WM_USER + 1222
#define WM_CLASSBARCMD     WM_USER + 1223
#define WM_TIMELINE        WM_USER + 1224
#define WM_TOOLBARCLICK    WM_USER + 1225
#define WM_TABCTRLEX       WM_USER + 1226
#define WM_TOOLBARRCLICK   WM_USER + 1227
#define WM_VUSLIDERCTRL    WM_USER + 1228
#define WM_DRAGLISTITEM    WM_USER + 1229
#define WM_TPEDITEX        WM_USER + 1230
#define WM_SCROLLBARZOOM   WM_USER + 1231

#define WM_TP_TIMELINE   WM_TIMELINE
#define WM_LOGGINGTOOL_JOGSHUTTLE WM_USER + 1232 //XN00029345 jiajia 2010/11/11

AFX_EXT_API	void    TP_InitCtrl();
AFX_EXT_API	void    TP_ReleaseCtrl();
