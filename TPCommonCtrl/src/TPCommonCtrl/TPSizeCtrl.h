#pragma once


#include <afxtempl.h>

enum TPSizeMode		// 对齐方式
{
	TPSizeXNone  =	    0x00000001,					   //不移动
	TPSizeXA     =	    0x00000002,					   //左或上对齐
	TPSizeXB	 =	    0x00000004,					   //右或下对齐
	TPSizeXAB    =	    0x00000008,				       //左右或上下对齐
	TPSizeXACB   =      0x00000010,                     //居中
	TPSizeXAF    =      0x00000020,                     //左或上放大
	TPSizeXBF    =      0x00000040,                     //右或下放大
	TPSizeXAFBF  =      0x00000080,                     //左或上和右下放大
	TPSizeXABF   =      0x00000100,                     //左或上对齐 右或下放大
	TPSizeXAFB   =      0x00000200,                      //左或上放大 右或下对齐


	TPSizeYNone  =	    0x00010000,					   //不移动
	TPSizeYA   	 =	    0x00020000,					   //左或上对齐
	TPSizeYB	 =	    0x00040000,					   //右或下对齐
	TPSizeYAB    =	    0x00080000,				       //左右或上下对齐
	TPSizeYACB   =      0x00100000,                     //居中
	TPSizeYAF    =      0x00200000,                     //左或上放大
	TPSizeYBF    =      0x00400000,                     //右或下放大
	TPSizeYAFBF  =      0x00800000,                     //左或上和右下放大
	TPSizeYABF   =      0x01000000,                     //左或上对齐 右或下放大
	TPSizeYAFB   =      0x02000000,                      //左或上放大 右或下对齐

};
typedef struct tag_TPSizeCtrlData
{
	HWND         hWnd;
	CRect        rect;
	TPSizeMode   eModeX;
	TPSizeMode   eModeY; 
	BOOL         bEnable;
	tag_TPSizeCtrlData()
	{
		hWnd    = NULL;
		rect    = CRect(0,0,0,0);
		eModeX  = TPSizeXNone;
		eModeY  = TPSizeXNone; 
		bEnable = TRUE;
	}
} TPSizeCtrlData;

typedef CArray <TPSizeCtrlData ,TPSizeCtrlData &> TPSizeCtrlDataArray;


class AFX_EXT_CLASS CTPSizeCtrl  
{
public:
	CTPSizeCtrl();
	virtual ~CTPSizeCtrl();
public:
	void    InitWindow(CWnd *pWnd);
	void    OnSize(CSize *pszDef = NULL);
	void    AddChildWindow(CWnd *pWnd,TPSizeMode eModeX,TPSizeMode eModeY);
	CRect   GetChildWindowRect(CWnd *pWnd);
	void    RemoveAllWnd();
	void    RemoveWnd(CWnd *pWnd);	
	void    SetWndSizeMode(CWnd *pWnd,TPSizeMode eModeX,TPSizeMode eModeY);
	void    EnableChildWindow(CWnd *pWnd,BOOL bEnableSize);
	void    SetExtraSize(int iExLeft,int iExTop,int iExRight,int iExBottom);

	void    SetCotrolData(TPControlData *pCtrlData,int iCtrlSize);

private:
	CRect   CalcCtrlSize(TPSizeCtrlData &addData,CRect rtWnd,double fAmpX,double fAmpY);

private:
	HWND                 m_hWnd;
	CRect                m_rtWnd;
	CRect                m_rtExtra;
	TPSizeCtrlDataArray  m_arrControl;
private:

};

