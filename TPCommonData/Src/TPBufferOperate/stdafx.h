// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include "TPBaseDef.h"
#include "TPBufferOperateDef.h"

#if defined ( __w64 )		// 64 λ�� 32 λƽ̨����ָ��ļĴ�����ͬ

#define EAX_PTR			RAX
#define EBX_PTR			RBX
#define ECX_PTR			RCX
#define EDX_PTR			RDX
#define EDI_PTR			RDI
#define ESI_PTR			RSI
#define PTR
#define ptrword			qword		// ����ָ���ָ����ڴ���������
#define PTR_BYTES_1		0x08		// ָ������ռ�õ��ڴ��С
#define __USE_64_INS__		// ����������±���ʹ�� 64 λָ��

#else		// #if defined ( __w64 )

#define EAX_PTR			EAX
#define EBX_PTR			EBX
#define ECX_PTR			ECX
#define EDX_PTR			EDX
#define EDI_PTR			EDI
#define ESI_PTR			ESI
#define ptrword			dword		// ����ָ���ָ����ڴ���������
#define PTR_BYTES_1		0x04		// ָ������ռ�õ��ڴ��С

#endif		// #if defined ( __w64 )

#define eax_ptr			EAX_PTR
#define ebx_ptr			EBX_PTR
#define ecx_ptr			ECX_PTR
#define edx_ptr			EDX_PTR
#define edi_ptr			EDI_PTR
#define esi_ptr			ESI_PTR

#define _buffer_operate_export
#define __ADAPT_CPU_SDK__
