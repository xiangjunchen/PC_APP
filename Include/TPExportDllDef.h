#if !defined(TPCOMMONEXPORT_INCLUDED_)
#define TPCOMMONEXPORT_INCLUDED_

//////////////////////////////////////////////////////////////////////
////////////////////// 模块特性定义 //////////////////////////////////

#undef	AFX_EXT_CLASS
#undef	AFX_EXT_DATA
#undef	AFX_EXT_API

//ceshi
//#define AFX_EXT_CLASS	AFX_CLASS_EXPORT
//#define AFX_EXT_DATA	AFX_DATA_EXPORT
//#define AFX_EXT_API		AFX_API_EXPORT
//

#define AFX_EXT_CLASS	__declspec(dllexport)
#define AFX_EXT_DATA	__declspec(dllexport)
#define AFX_EXT_API	__declspec(dllexport)


#endif // TPCOMMONEXPORT_INCLUDED_
