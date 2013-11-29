// TPCommonClass.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TPCommonClass.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
HANDLE          g_hModule       = NULL;
static AFX_EXTENSION_MODULE TPCommonClassDLL = { NULL, NULL };


CTPNetInterface* g_pNetInterface = NULL;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TPCommonClass.DLL ���ڳ�ʼ����\n");

		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(TPCommonClassDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL(�� ActiveX �ؼ�)��ʽ���ӵ�
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		g_hModule = hInstance;

		WSADATA		wsaData;
		WORD wVersion	= MAKEWORD(2, 2);
		int  iCheck     = WSAStartup(wVersion, &wsaData);

		new CDynLinkLibrary(TPCommonClassDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TPCommonClass.DLL ������ֹ��\n");

		WSACleanup();

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(TPCommonClassDLL);
	}
	return 1;   // ȷ��
}
namespace _tp_commonclass
{
	void   TP_InitClass()
	{
		CTPMessageCenter::CreateWnd();

	}
	void   TP_ReleaseClass()
	{
		if(g_pNetInterface)
		{
			g_pNetInterface ->Release();
			delete g_pNetInterface;
			g_pNetInterface = NULL;
		}

		CTPMessageCenter::DestroyWnd();
	}
	CTPNetInterface *TP_GetNetInterface()
	{
		if(g_pNetInterface == NULL) g_pNetInterface = new CTPNetInterface();
		return g_pNetInterface;
	}

};