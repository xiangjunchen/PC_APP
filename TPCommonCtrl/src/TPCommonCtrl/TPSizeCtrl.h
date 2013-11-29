#pragma once


#include <afxtempl.h>

enum TPSizeMode		// ���뷽ʽ
{
	TPSizeXNone  =	    0x00000001,					   //���ƶ�
	TPSizeXA     =	    0x00000002,					   //����϶���
	TPSizeXB	 =	    0x00000004,					   //�һ��¶���
	TPSizeXAB    =	    0x00000008,				       //���һ����¶���
	TPSizeXACB   =      0x00000010,                     //����
	TPSizeXAF    =      0x00000020,                     //����ϷŴ�
	TPSizeXBF    =      0x00000040,                     //�һ��·Ŵ�
	TPSizeXAFBF  =      0x00000080,                     //����Ϻ����·Ŵ�
	TPSizeXABF   =      0x00000100,                     //����϶��� �һ��·Ŵ�
	TPSizeXAFB   =      0x00000200,                      //����ϷŴ� �һ��¶���


	TPSizeYNone  =	    0x00010000,					   //���ƶ�
	TPSizeYA   	 =	    0x00020000,					   //����϶���
	TPSizeYB	 =	    0x00040000,					   //�һ��¶���
	TPSizeYAB    =	    0x00080000,				       //���һ����¶���
	TPSizeYACB   =      0x00100000,                     //����
	TPSizeYAF    =      0x00200000,                     //����ϷŴ�
	TPSizeYBF    =      0x00400000,                     //�һ��·Ŵ�
	TPSizeYAFBF  =      0x00800000,                     //����Ϻ����·Ŵ�
	TPSizeYABF   =      0x01000000,                     //����϶��� �һ��·Ŵ�
	TPSizeYAFB   =      0x02000000,                      //����ϷŴ� �һ��¶���

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

