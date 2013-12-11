// TPBufferOperate.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>
#include <string>
#include "TPSSE2.h"

// #include "NXAssambleDef.h"
#include "CoreInclude\CPUInclude\NXMemProc.h"
#include "CoreInclude\CPUInclude\NXVideoProc.h"
#include "CoreInclude\CPUInclude\NXColorSpace.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE TPBufferOperateDLL = { NULL, NULL };

// ȫ�ֵĶ�ά��������DLL����ʱ������DLLж��ʱ�ͷ�
_declspec(align(16)) float *g_pfTable = NULL;
// ȫ�ֵ�һά��
_declspec(align(16)) float* g_pfPosTable = NULL;
// ȫ�ֶ�ά�����
_declspec(align(16)) DWORD *g_AlphaGainTable = NULL;

//////////////////////////////////////////////////////////////////////////
// ΪBitBlt����ά��, SA/(float)KA
// ���С��256��256�������� , [SA][KA]
//////////////////////////////////////////////////////////////////////////
extern "C"  BOOL TPBuffer_BitBltCreateTable()
{
	if(g_pfTable == NULL)
		g_pfTable = (float*)_mm_malloc(sizeof(float)*256*256, 16);
	if(g_pfTable == NULL)
		return FALSE;
/*	int nTemp = 0;
	for(int i=0; i<256; i++)
	{
		g_pfTable[nTemp] = (float)i;
		for(int j=1; j<256; j++)
		{
			g_pfTable[nTemp+j] = ((float)i)/j;
		}
		nTemp += 256;
	}
*/
// �����j�ĵ���
	_declspec(align(16)) float *parRecTable = (float*)_mm_malloc(sizeof(float)*256, 16);
	if(parRecTable == NULL)
		return FALSE;
	__asm
	{
		XOR ECX, ECX
		MOV EAX, parRecTable
		INC ECX
		MOVD MM0, ECX
		PUNPCKLDQ MM0, MM0   // ����1,1
		MOVQ2DQ XMM0, MM0
		INC ECX
		MOVD MM0, ECX
		INC ECX
		MOVD MM1, ECX
		PUNPCKLDQ MM0, MM1  // ����3, 2
		MOVQ2DQ XMM1, MM0
		PUNPCKLQDQ XMM0, XMM1 // ����3, 2, 1, 1
		CVTDQ2PS XMM2, XMM0
		RCPPS XMM1, XMM2	  // ȡ����
		INC ECX
		MOVAPS [EAX], XMM1
		ADD EAX, 10H
LOOPX:
		ADD ECX, 4
		CMP ECX, 256
		JG  LOOPXEND
		SUB ECX, 4
		MOVD MM0, ECX
		INC ECX
		MOVD MM1, ECX
		INC ECX
		PUNPCKLDQ MM0, MM1  // ����1, 0
		MOVD MM1, ECX
		INC ECX
		MOVD MM2, ECX
		PUNPCKLDQ MM1, MM2  // ����3, 2
		MOVQ2DQ XMM0, MM0
		MOVQ2DQ XMM1, MM1
		PUNPCKLQDQ XMM0, XMM1 // ����3, 2, 1, 0
		CVTDQ2PS XMM2, XMM0
		RCPPS XMM1, XMM2	  // ȡ����
		INC ECX
		MOVAPS [EAX], XMM1
		ADD EAX, 10H
		JMP LOOPX
LOOPXEND:
		EMMS
	}
// ���i*1.0/j
	int nAddWidth = 256<<2;
	__asm
	{
		XOR ECX, ECX
		MOV EAX, g_pfTable		// �洢Ŀ�껺����ָ��
LOOPI:
		CMP	 ECX, 256
		JGE  LOOPIEND
		MOV  EDX, EAX			// ������ָ��
		// �洢iֵ��Float����XMM0��
		MOVD MM0, ECX
		PUNPCKLDQ MM0, MM0
		MOVQ2DQ XMM0, MM0
		PUNPCKLQDQ XMM0, XMM0	// ���iiii
		CVTDQ2PS XMM1, XMM0		// ת����Float��
		// ѭ��j
		PUSH ECX
		XOR  ECX, ECX
		PUSH EAX
		MOV  EAX, parRecTable	// j��������ָ��
LOOPJ:
		ADD ECX, 4
		CMP ECX, 256
		JG LOOPJEND

		MOVAPS XMM2, [EAX]
		MULPS  XMM2, XMM1		// i*1/j
		ADD EAX, 10H
		MOVAPS [EDX], XMM2
		ADD EDX, 10H
		
		JMP LOOPJ
LOOPJEND:
		POP EAX
		ADD EAX, nAddWidth	// ָ����һ��
		POP ECX
		INC ECX
		JMP LOOPI
LOOPIEND:
		EMMS
	}
	_mm_free(parRecTable);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// ������ά�����
extern "C" BOOL CreateAlphaGainTable()
{
	int nIndex(0), nTmp;

	g_AlphaGainTable = (DWORD*)_mm_malloc(65536 * sizeof(DWORD), 16);
	for (int i=0; i<256; i++)
	{
		for (int j=0; j<256; j++,nIndex++)
		{
			nTmp = 255 * (i + j) - i * j;
			if (nTmp == 0)
			{
				g_AlphaGainTable[nIndex] = 0;
			}
			else
			{
				g_AlphaGainTable[nIndex] = (DWORD)(8355585 * i / nTmp);
			}
		}
	}
	return TRUE;
}

// ����Pos��i/255, 0 =< i <=255
extern "C" BOOL CreateRecTable()
{
	if(g_pfPosTable == NULL)
		g_pfPosTable = (float*)_mm_malloc(sizeof(float)*256, 16);
	if(g_pfPosTable == NULL)
		return FALSE;
	// ��Pos��
	float fRec = 1.0f/255;
	__asm
	{
		XOR ECX, ECX
		MOV EDX, g_pfPosTable
		MOVSS XMM7, fRec
		SHUFPS XMM7, XMM7, 0
LOOPX:
		ADD ECX, 4
		CMP ECX, 256
		JG  LOOPXEND
		SUB ECX, 4

		MOVD MM0, ECX
		INC ECX
		MOVD MM1, ECX
		INC ECX
		PUNPCKLDQ MM0, MM1  // ����1, 0
		MOVD MM1, ECX
		INC ECX
		MOVD MM2, ECX

		PUNPCKLDQ MM1, MM2  // ����3, 2
		MOVQ2DQ XMM0, MM0
		MOVQ2DQ XMM1, MM1
		PUNPCKLQDQ XMM0, XMM1 // ����3, 2, 1, 0
		CVTDQ2PS XMM2, XMM0
		MULPS XMM2, XMM7
		INC ECX
		MOVAPS [EDX], XMM2

		ADD EDX, 10H
		JMP LOOPX
LOOPXEND:
		EMMS
	}
	// ������ά�����
	CreateAlphaGainTable();
	return TRUE;
}

// �ͷű�ռ�
extern "C" BOOL TPBuffer_BitBltFreeTable()
{
	if(g_pfTable != NULL)
	{
		_mm_free(g_pfTable);
		g_pfTable = NULL;
	}
	if(g_pfPosTable != NULL)
	{
		_mm_free(g_pfPosTable);
		g_pfPosTable = NULL;
	}
	if (g_AlphaGainTable != NULL)
	{
		_mm_free(g_AlphaGainTable);
		g_AlphaGainTable = NULL;
	}
	return TRUE;
}

//[[lgq 2009.11.17
//�¿�Ľӿڶ���
LPNXMEMPROCAPI g_lpMemProcAPI = NULL;
LPNXVIDEOPROCAPI g_lpVideoProcAPI = NULL;
LPNXCOLORSPACEAPI g_pColorSace= NULL;
//�¿��module
HMODULE g_hMemProcModule = NULL;
HMODULE g_hVideoProcModule = NULL;
HMODULE g_hcolorModule = NULL;

//�����µ�sdk��, ������ĺ��������滻
void LoadNXCPUProcSDK()
{
// 	TCHAR szchDir[MAX_PATH];
// 	GetModuleFileName(NULL, szchDir, MAX_PATH);

// 	// '\' �ַ���ִ�д���, ��Ҫ�ҵ�debug��releaseĿ¼֮ǰ���Ǹ�Ŀ¼
// 	int iCount = 0;		
// 
// 	//���β���
// 	for (int i = (int)_tcslen(szchDir) - 1; i >= 0; i--)
// 	{
// 		if (szchDir[i] == _T('\\'))
// 		{
// 			iCount++;
// 			if (iCount >= 2)		// �ҵ��˶�Ӧ��Ŀ¼
// 			{
// 				szchDir[i] = _T('\0');
// 
// 				//��Ҫ��CPUProc���ڵ�Ŀ¼������
// #if _MSC_VER >= 1500
// 				_tcscat_s(szchDir, _T("\\dll"));
// #else
// 				_tcscat(szchDir, _T("\\dll"));
// #endif
// 				break;
// 			}
// 		}
// 	}
// 
// 	//·��������
// 	if (iCount < 2)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}

	std::basic_string<TCHAR> _Path(MAX_PATH, TCHAR());
	_Path.resize(GetModuleFileName(NULL, const_cast<TCHAR*>(_Path.c_str()), MAX_PATH) + 1);
	_Path.erase(_Path.rfind(_T('\\')));
	_Path.erase(_Path.rfind(_T('\\')) + 1);
	_Path.append(_T("dll"));

	//��Ŀ¼��Ϊ����load��Ӧ��dll
	g_hMemProcModule = NXMemProc_LoadDLL(_Path.c_str(), (LPNXMEMPROCAPI*)&g_lpMemProcAPI);
	ASSERT(g_hMemProcModule != NULL);
	g_hVideoProcModule = NXVideoProc_LoadDLL(_Path.c_str(), (LPNXVIDEOPROCAPI*)&g_lpVideoProcAPI);
	ASSERT(g_hVideoProcModule != NULL);
	if(NULL== g_hcolorModule)
		g_hcolorModule= NXColorSpace_LoadDLL(NULL,&g_pColorSace);

}

void FreeNXCPUProcSDK()
{
	if (g_hVideoProcModule)
	{
		BOOL bRet = FreeLibrary(g_hVideoProcModule);
		g_hVideoProcModule = NULL;
	}
	if (g_hMemProcModule)
	{
		BOOL bRet = FreeLibrary(g_hMemProcModule);
		g_hMemProcModule = NULL;
	}
	if(g_hcolorModule)
		FreeLibrary(g_hcolorModule);
}
//]]

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TPBufferOperate.DLL Initializing!\n");
		
		// ��ʼ��DLLʱ��BitBlt��
		if(!TPBuffer_BitBltCreateTable())
			return 0;
		if(!CreateRecTable())
			return 0;

        // ��ʼ�� SSE ����
        InitSSEStaticCoef();

		//LoadCPUProc��
		LoadNXCPUProcSDK();

		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(TPBufferOperateDLL, hInstance))
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

		new CDynLinkLibrary(TPBufferOperateDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TPBufferOperate.DLL ������ֹ��\n");
		
		// �˳���ʱ�ͷű�ռ�
		TPBuffer_BitBltFreeTable();

		//FreeCPUProc��
		FreeNXCPUProcSDK();

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(TPBufferOperateDLL);
	}
	return 1;   // ȷ��
}
