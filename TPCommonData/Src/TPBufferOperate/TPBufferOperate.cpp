// TPBufferOperate.cpp : 定义 DLL 的初始化例程。
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

// 全局的二维表缓冲区，DLL加载时创建，DLL卸载时释放
_declspec(align(16)) float *g_pfTable = NULL;
// 全局的一维表
_declspec(align(16)) float* g_pfPosTable = NULL;
// 全局二维增益表
_declspec(align(16)) DWORD *g_AlphaGainTable = NULL;

//////////////////////////////////////////////////////////////////////////
// 为BitBlt建二维表, SA/(float)KA
// 表大小：256×256个浮点数 , [SA][KA]
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
// 先算出j的倒数
	_declspec(align(16)) float *parRecTable = (float*)_mm_malloc(sizeof(float)*256, 16);
	if(parRecTable == NULL)
		return FALSE;
	__asm
	{
		XOR ECX, ECX
		MOV EAX, parRecTable
		INC ECX
		MOVD MM0, ECX
		PUNPCKLDQ MM0, MM0   // 包含1,1
		MOVQ2DQ XMM0, MM0
		INC ECX
		MOVD MM0, ECX
		INC ECX
		MOVD MM1, ECX
		PUNPCKLDQ MM0, MM1  // 包含3, 2
		MOVQ2DQ XMM1, MM0
		PUNPCKLQDQ XMM0, XMM1 // 包含3, 2, 1, 1
		CVTDQ2PS XMM2, XMM0
		RCPPS XMM1, XMM2	  // 取倒数
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
		PUNPCKLDQ MM0, MM1  // 包含1, 0
		MOVD MM1, ECX
		INC ECX
		MOVD MM2, ECX
		PUNPCKLDQ MM1, MM2  // 包含3, 2
		MOVQ2DQ XMM0, MM0
		MOVQ2DQ XMM1, MM1
		PUNPCKLQDQ XMM0, XMM1 // 包含3, 2, 1, 0
		CVTDQ2PS XMM2, XMM0
		RCPPS XMM1, XMM2	  // 取倒数
		INC ECX
		MOVAPS [EAX], XMM1
		ADD EAX, 10H
		JMP LOOPX
LOOPXEND:
		EMMS
	}
// 算出i*1.0/j
	int nAddWidth = 256<<2;
	__asm
	{
		XOR ECX, ECX
		MOV EAX, g_pfTable		// 存储目标缓冲区指针
LOOPI:
		CMP	 ECX, 256
		JGE  LOOPIEND
		MOV  EDX, EAX			// 缓冲区指针
		// 存储i值的Float型在XMM0中
		MOVD MM0, ECX
		PUNPCKLDQ MM0, MM0
		MOVQ2DQ XMM0, MM0
		PUNPCKLQDQ XMM0, XMM0	// 变成iiii
		CVTDQ2PS XMM1, XMM0		// 转换成Float型
		// 循环j
		PUSH ECX
		XOR  ECX, ECX
		PUSH EAX
		MOV  EAX, parRecTable	// j倒数缓冲指针
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
		ADD EAX, nAddWidth	// 指向下一行
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
// 创建二维增益表
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

// 创建Pos表，i/255, 0 =< i <=255
extern "C" BOOL CreateRecTable()
{
	if(g_pfPosTable == NULL)
		g_pfPosTable = (float*)_mm_malloc(sizeof(float)*256, 16);
	if(g_pfPosTable == NULL)
		return FALSE;
	// 建Pos表
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
		PUNPCKLDQ MM0, MM1  // 包含1, 0
		MOVD MM1, ECX
		INC ECX
		MOVD MM2, ECX

		PUNPCKLDQ MM1, MM2  // 包含3, 2
		MOVQ2DQ XMM0, MM0
		MOVQ2DQ XMM1, MM1
		PUNPCKLQDQ XMM0, XMM1 // 包含3, 2, 1, 0
		CVTDQ2PS XMM2, XMM0
		MULPS XMM2, XMM7
		INC ECX
		MOVAPS [EDX], XMM2

		ADD EDX, 10H
		JMP LOOPX
LOOPXEND:
		EMMS
	}
	// 创建二维增益表
	CreateAlphaGainTable();
	return TRUE;
}

// 释放表空间
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
//新库的接口定义
LPNXMEMPROCAPI g_lpMemProcAPI = NULL;
LPNXVIDEOPROCAPI g_lpVideoProcAPI = NULL;
LPNXCOLORSPACEAPI g_pColorSace= NULL;
//新库的module
HMODULE g_hMemProcModule = NULL;
HMODULE g_hVideoProcModule = NULL;
HMODULE g_hcolorModule = NULL;

//导入新的sdk库, 用里面的函数进行替换
void LoadNXCPUProcSDK()
{
// 	TCHAR szchDir[MAX_PATH];
// 	GetModuleFileName(NULL, szchDir, MAX_PATH);

// 	// '\' 字符的执行次数, 需要找到debug或release目录之前的那个目录
// 	int iCount = 0;		
// 
// 	//依次查找
// 	for (int i = (int)_tcslen(szchDir) - 1; i >= 0; i--)
// 	{
// 		if (szchDir[i] == _T('\\'))
// 		{
// 			iCount++;
// 			if (iCount >= 2)		// 找到了对应的目录
// 			{
// 				szchDir[i] = _T('\0');
// 
// 				//需要将CPUProc所在的目录连接上
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
// 	//路径出错了
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

	//将目录作为参数load相应的dll
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
	// 如果使用 lpReserved，请将此移除
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TPBufferOperate.DLL Initializing!\n");
		
		// 初始化DLL时建BitBlt表
		if(!TPBuffer_BitBltCreateTable())
			return 0;
		if(!CreateRecTable())
			return 0;

        // 初始化 SSE 变量
        InitSSEStaticCoef();

		//LoadCPUProc库
		LoadNXCPUProcSDK();

		// 扩展 DLL 一次性初始化
		if (!AfxInitExtensionModule(TPBufferOperateDLL, hInstance))
			return 0;

		// 将此 DLL 插入到资源链中
		// 注意: 如果此扩展 DLL 由
		//  MFC 规则 DLL(如 ActiveX 控件)隐式链接到
		//  而不是由 MFC 应用程序链接到，则需要
		//  将此行从 DllMain 中移除并将其放置在一个
		//  从此扩展 DLL 导出的单独的函数中。使用此扩展 DLL 的
		//  规则 DLL 然后应显式
		//  调用该函数以初始化此扩展 DLL。否则，
		//  CDynLinkLibrary 对象不会附加到
		//  规则 DLL 的资源链，并将导致严重的
		//  问题。

		new CDynLinkLibrary(TPBufferOperateDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TPBufferOperate.DLL 正在终止！\n");
		
		// 退出库时释放表空间
		TPBuffer_BitBltFreeTable();

		//FreeCPUProc库
		FreeNXCPUProcSDK();

		// 在调用析构函数之前终止该库
		AfxTermExtensionModule(TPBufferOperateDLL);
	}
	return 1;   // 确定
}
