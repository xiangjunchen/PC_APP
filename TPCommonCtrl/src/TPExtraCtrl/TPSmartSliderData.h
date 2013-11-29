#pragma once
#define SE_SLIDER_ID				100					// スライダのＩＤ
#define MACSLIDERTICK  RGB(100,100,200)
#define MACWINDOWBACKGROUND RGB(200,100,100)
#define WM_SE_PARAMETER				(WM_APP + 0x500)		// パラメータＵＩ
#define	WM_SE_ENDPARAMETER			(WM_APP + 0x505)		// パラメータＵＩの終了
#define	WM_SE_MOUSEHOOK				(WM_APP + 0x518)		// MouseHook

const SIZE SE_SLIDER_EXPAND_HITTEST	= { 4, 0 };

typedef struct _tag_DPoint
{
	double x;
	double y;
	_tag_DPoint()
	{
		x=0.0;
		y=0.0;
	}
	_tag_DPoint(_tag_DPoint& src)
	{
		x=src.x;
		y=src.y;
	}
	_tag_DPoint(double x1,double y1)
	{
		x=x1;
		y=y1;
	}

}SEPOINTREAL;