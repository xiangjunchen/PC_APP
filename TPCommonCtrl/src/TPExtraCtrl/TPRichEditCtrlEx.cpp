

// TPRichEditCtrlEx.cpp : 实现文件
#include "stdafx.h"
#include <vector>
#include <algorithm>
#include "TPExtraCtrl.h"
#include "TPRichEditCtrlEx.h"


using namespace std;


// CRichEditCtrlEx
#define ID_RICH_UNDO                    101
#define ID_RICH_CUT                     102
#define ID_RICH_COPY                    103
#define ID_RICH_PASTE                   104
#define ID_RICH_CLEAR                   105
#define ID_RICH_SELECTALL               106
#define ID_RICH_SETFONT                 107
IMPLEMENT_DYNAMIC(CTPRichEditCtrlEx, CTPRichEditCtrl)
#ifdef _AFXDLL 
LPUNKNOWN AFXAPI _AfxQueryInterface(LPUNKNOWN lpUnknown, REFIID iid)
{
	ASSERT(lpUnknown != NULL);

	LPUNKNOWN lpW = NULL;
	if (lpUnknown->QueryInterface(iid, (void **)&lpW) != S_OK)
		return NULL;

	return lpW;
}
#endif
CTPRichEditCtrlEx::CTPRichEditCtrlEx()
{
	m_type=CHATMSG;
	m_pDataSource = NULL;
}

CTPRichEditCtrlEx::~CTPRichEditCtrlEx()
{
	if (m_pDataSource != NULL)
	{
		delete m_pDataSource;
		m_pDataSource = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTPRichEditCtrlEx, CTPRichEditCtrl)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_RICH_COPY, OnCopy)
	ON_COMMAND(ID_RICH_CUT, OnCut)
	ON_COMMAND(ID_RICH_PASTE, OnPaste)
	ON_COMMAND(ID_RICH_SELECTALL, OnSelectall)
	ON_COMMAND(ID_RICH_UNDO, OnUndo)
	ON_COMMAND(ID_RICH_CLEAR, OnClear)
	ON_COMMAND(ID_RICH_SETFONT, OnSelectfont)
	ON_WM_CREATE()
	//ON_WM_PAINT()
	ON_NOTIFY_REFLECT(EN_LINK, &CTPRichEditCtrlEx::OnEnLink)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CRichEditCtrlEx 消息处理程序
void CTPRichEditCtrlEx::SetType(int type)
{
	if (type==CHATLOG)
	{
		m_type=CHATLOG;
		this->SetReadOnly(TRUE);
	}
}
///插入位图,从文件中读入
void CTPRichEditCtrlEx::InsertBitmap(CString szFileName)
{
	USES_CONVERSION;
	SCODE sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &m_lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(m_lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(m_lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &m_lpStorage);
	if (sc != S_OK)
	{
		VERIFY(m_lpLockBytes->Release() == 0);
		m_lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}

	//CString szFileName( _T("C:\\Dev\\RTFBitmap\\reportplot.bmp") );

	// attempt to create the object
	sc = ::OleCreateFromFile(CLSID_NULL, T2COLE(szFileName),
		IID_IUnknown, OLERENDER_DRAW, NULL, NULL, 
		m_lpStorage, (void **)&m_lpObject);
	if ( sc != S_OK )
	{
		TCHAR * lpMsgBuf;
		::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM, NULL, 
			::GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf, 0, NULL );
		CString msg( lpMsgBuf );
		msg += _T("\n\n\nThe following file, created in\nSimulation->Plot, may be missing due\nto not doing a File->Save Workspace:\n\n" );
		msg += szFileName;
		AfxMessageBox( msg, MB_OK );
		::LocalFree( lpMsgBuf );
		return;
	}

	// m_lpObject is currently an IUnknown, convert to IOleObject
	if (m_lpObject != NULL)
	{
		LPUNKNOWN lpUnk = m_lpObject;
		m_lpObject = QUERYINTERFACE(lpUnk, IOleObject);
		lpUnk->Release();
		if (m_lpObject == NULL)
			AfxThrowOleException(E_OUTOFMEMORY);
	}

	// cache the IViewObject interface
	m_lpViewObject = QUERYINTERFACE(m_lpObject, IViewObject2);
	if (m_lpViewObject == NULL)
		return;

	// setup for advises; we assume that OLE cleans them up properly
	LPADVISESINK lpAdviseSink =
		(LPADVISESINK)GetInterface(&IID_IAdviseSink);

	// set up view advise
	VERIFY(m_lpViewObject->SetAdvise(DVASPECT_CONTENT, 0, lpAdviseSink)
		== S_OK);

	// the server shows these in its user-interface
	//  (as document title and in File Exit menu)
	m_lpObject->SetHostNames(T2COLE(AfxGetAppName()),
		T2COLE(_T("Test")));

	// all items are "contained" -- this makes our reference to this object
	//  weak -- which is needed for links to embedding silent update.
	OleSetContainedObject(m_lpObject, TRUE);

	CHARRANGE cr;
	this->GetSel( cr );
	cr.cpMin = cr.cpMax -1;
	this->SetSel( cr );

	REOBJECT reo;
	memset( &reo, 0, sizeof( reo ) );
	reo.cbStruct = sizeof( reo );
	CLSID classID;
	if ( m_lpObject->GetUserClassID( &classID ) != S_OK)
		classID = CLSID_NULL;
	reo.clsid = classID;
	reo.cp = REO_CP_SELECTION;
	reo.poleobj = m_lpObject;
	reo.pstg = m_lpStorage;
	LPOLECLIENTSITE lpClientSite;
	this->GetIRichEditOle()->GetClientSite( &lpClientSite );
	reo.polesite = lpClientSite;
	SIZEL sizel;
	sizel.cx = sizel.cy = 0; // let richedit determine initial size
	reo.sizel = sizel;
	reo.dvaspect = DVASPECT_CONTENT;
	reo.dwFlags = REO_RESIZABLE;
	reo.dwUser = 0;
	HRESULT hr = this->GetIRichEditOle()->InsertObject( &reo );
	
}

///生成右键菜单
void CTPRichEditCtrlEx::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//设置为焦点
// 	SetFocus();
// 	//创建一个弹出式菜单
// 	CMenu popmenu;
// 	popmenu.CreatePopupMenu();
// 	if (m_type==CHATMSG)
// 	{
// 		//添加菜单项目
// 		popmenu.AppendMenu(0, ID_RICH_UNDO, _T("撤销(&U)"));
// 		popmenu.AppendMenu(0, MF_SEPARATOR);
// 		popmenu.AppendMenu(0, ID_RICH_CUT, _T("剪切(&X)"));
// 		popmenu.AppendMenu(0, ID_RICH_COPY, _T("复制(&C)"));
// 		popmenu.AppendMenu(0, ID_RICH_PASTE, _T("粘贴(&P)"));		
// 		popmenu.AppendMenu(0, MF_SEPARATOR);
// 		popmenu.AppendMenu(0, ID_RICH_SELECTALL, _T("全部选择(&A)"));
// 		popmenu.AppendMenu(0, MF_SEPARATOR);
// 		popmenu.AppendMenu(0, ID_RICH_SETFONT, _T("字体选择(&F)"));
// 
// 		//初始化菜单项
// 		UINT nUndo=(CanUndo() ? 0 : MF_GRAYED );
// 		popmenu.EnableMenuItem(ID_RICH_UNDO, MF_BYCOMMAND|nUndo);
// 
// 		UINT nSel=((GetSelectionType()!=SEL_EMPTY) ? 0 : MF_GRAYED) ;
// 		popmenu.EnableMenuItem(ID_RICH_CUT, MF_BYCOMMAND|nSel);
// 		popmenu.EnableMenuItem(ID_RICH_COPY, MF_BYCOMMAND|nSel);		
// 
// 		UINT nPaste=(CanPaste() ? 0 : MF_GRAYED) ;
// 		popmenu.EnableMenuItem(ID_RICH_PASTE, MF_BYCOMMAND|nPaste);
// 	} 
// 	else
// 	{		
// 		popmenu.AppendMenu(0, ID_RICH_COPY, _T("复制(&C)"));		
// 		popmenu.AppendMenu(0, ID_RICH_SELECTALL, _T("全部选择(&A)"));
// 		popmenu.AppendMenu(0, MF_SEPARATOR);
// 		popmenu.AppendMenu(0, ID_RICH_CLEAR, _T("清屏(&T)"));		
// 
// 		//初始化菜单项		
// 
// 		UINT nSel=((GetSelectionType()!=SEL_EMPTY) ? 0 : MF_GRAYED) ;		
// 		popmenu.EnableMenuItem(ID_RICH_COPY, MF_BYCOMMAND|nSel);		
// 	}
// 	
// 
// 	//显示菜单
// 	CPoint pt;
// 	GetCursorPos(&pt);
// 	popmenu.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
// 	popmenu.DestroyMenu();
// 	CRichEditCtrl::OnRButtonDown(nFlags, point);
	CRichEditCtrl::OnRButtonUp(nFlags, point);
}
void CTPRichEditCtrlEx::OnPaste()
{
// 	this->PasteSpecial(CF_TEXT);
// 	CHARFORMAT cf;
// 	memset(&cf, 0, sizeof(CHARFORMAT));
// 	GetDefaultCharFormat(cf);
// 	SetDefaultCharFormat(cf);
}
void CTPRichEditCtrlEx::OnCopy()
{	
	Copy();	
}
//////////////////////////////////////////////////////////////////////////
///设置默认的字体
//////////////////////////////////////////////////////////////////////////
void CTPRichEditCtrlEx::OnSelectfont() 
{
	// TODO: Add your command handler code here
	CHARFORMAT cf;
	LOGFONT lf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	memset(&lf, 0, sizeof(LOGFONT));
	//判断是否选择了内容
	/*BOOL m_bSelect = (GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE;
	if (m_bSelect)
	{
		GetSelectionCharFormat(cf);
	}
	else
	{*/
		GetDefaultCharFormat(cf);
	//}
	//得到相关字体属性
	BOOL bIsBold = cf.dwEffects & CFE_BOLD;
	BOOL bIsItalic = cf.dwEffects & CFE_ITALIC;
	BOOL bIsUnderline = cf.dwEffects & CFE_UNDERLINE;
	BOOL bIsStrickout = cf.dwEffects & CFE_STRIKEOUT;//删除线
	//设置属性
	lf.lfCharSet = cf.bCharSet;	
	lf.lfHeight = cf.yHeight/15;
	lf.lfPitchAndFamily = cf.bPitchAndFamily;
	lf.lfItalic = bIsItalic;
	lf.lfWeight = (bIsBold ? FW_BOLD : FW_NORMAL);
	lf.lfUnderline = bIsUnderline;
	lf.lfStrikeOut = bIsStrickout;
	_stprintf_s(lf.lfFaceName,32, cf.szFaceName);

	CFontDialog dlg(&lf);
	dlg.m_cf.rgbColors = cf.crTextColor;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCharFormat(cf);//获得所选字体的属性
		//if (m_bSelect) 
		//	SetSelectionCharFormat(cf);	//为选定的内容设定所选字体
		//else
		//	SetWordCharFormat(cf);	//为将要输入的内容设定字体
		SetDefaultCharFormat(cf);
	}


}



int CTPRichEditCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	this->SetBackgroundColor(FALSE,RGB(81,81,81));
	this->SetFont(GetParent()->GetFont());
	return 0;
}
/*
 *	函数名：InsertBitmap
 *	参数：位图句柄和位图索引
 *	作用：位图句柄用于插入到RichEdit
 *		   位图索引用于填入转义字符信息到OLE图像对象
 */
//////////////////////////////////////////////////////////////////////////
void CTPRichEditCtrlEx::InsertBitmap(HBITMAP hBitmap,int index)
{
	STGMEDIUM stgm;
	stgm.tymed = TYMED_GDI;	   // Storage medium = HBITMAP handle
	stgm.hBitmap = hBitmap;
	stgm.pUnkForRelease = NULL; // Use ReleaseStgMedium

	FORMATETC fm;
	fm.cfFormat = CF_BITMAP;				// Clipboard format = CF_BITMAP
	fm.ptd = NULL;							// Target Device = Screen
	fm.dwAspect = DVASPECT_CONTENT;			// Level of detail = Full content
	fm.lindex = -1;							// Index = Not applicaple
	fm.tymed = TYMED_GDI;		

	////创建输入数据源
	IStorage *pStorage;	

	///分配内存
	LPLOCKBYTES lpLockBytes = NULL;
	SCODE sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &pStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(pStorage != NULL);

	if (m_pDataSource != NULL)
	{
		delete m_pDataSource;
		m_pDataSource = NULL;
	}
	m_pDataSource = new COleDataSource;
	m_pDataSource->CacheData(CF_BITMAP, &stgm);
	LPDATAOBJECT lpDataObject = 
		(LPDATAOBJECT)m_pDataSource->GetInterface(&IID_IDataObject);

	///获取RichEdit的OLEClientSite
	LPOLECLIENTSITE lpClientSite;
	this->GetIRichEditOle()->GetClientSite( &lpClientSite );


	///创建OLE对象
	IOleObject *pOleObject;
	sc = OleCreateStaticFromData(lpDataObject,IID_IOleObject,OLERENDER_FORMAT,
		&fm,lpClientSite,pStorage,(void **)&pOleObject);
	if(sc!=S_OK)
		AfxThrowOleException(sc);

	///插入OLE对象

	REOBJECT reobject;
	ZeroMemory(&reobject, sizeof(REOBJECT));
	reobject.cbStruct = sizeof(REOBJECT);

	CLSID clsid;
	sc = pOleObject->GetUserClassID(&clsid);
	if (sc != S_OK)
		AfxThrowOleException(sc);

	reobject.clsid = clsid;
	reobject.cp = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.poleobj = pOleObject;
	reobject.polesite = lpClientSite;
	reobject.pstg = pStorage;
	reobject.dwUser=index;
	HRESULT hr = this->GetIRichEditOle()->InsertObject( &reobject );
// 	delete pDataSource;
// 	pDataSource = NULL;
}
//////////////////////////////////////////////////////////////////////////
////返回RTF格式化字符串
//////////////////////////////////////////////////////////////////////////
CString CTPRichEditCtrlEx::GetRTF()
{
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = RTFFormatToCString;		// Set the callback

	CString sRTF = _T("");

	es.dwCookie = (DWORD) &sRTF;	// so sRTF receives the string

	StreamOut(SF_RTF, es);			// Call CRichEditCtrl::StreamOut to get the string.
	///

	return sRTF;

}

void CTPRichEditCtrlEx::SetRTF(CString strRTF)
{
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CStringFormatToRTF;
	es.dwCookie = (DWORD) &strRTF;
	StreamIn(SF_RTF, es);	// Do it.
	
}
//////////////////////////////////////////////////////////////////////////
////StreamOut的回调函数
//////////////////////////////////////////////////////////////////////////
DWORD CALLBACK CTPRichEditCtrlEx::RTFFormatToCString(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString *psEntry = (CString*) dwCookie;


	///把数据存放在一个临时变量里
	CString tmpEntry = _T("");
	tmpEntry = (CString) pbBuff;

	// And write it!!!
	//把实际有效的数据返回
	*psEntry += tmpEntry.Left(cb);

	return 0;
}
//////////////////////////////////////////////////////////////////////////
////StreamIn的回调函数 *pcb是实际输入的字节数,cb是一次输入的最大字节数
//////////////////////////////////////////////////////////////////////////
DWORD CALLBACK CTPRichEditCtrlEx::CStringFormatToRTF(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString *pstr = (CString *) dwCookie;

	////若要存放到RichEdit的数据长度小于一次能存放的最大长度,则
	////一次性输入到RichEdit中
	if (pstr->GetLength() < cb)
	{
		*pcb = pstr->GetLength();
		memcpy(pbBuff, (LPCTSTR) *pstr, *pcb);
		pstr->Empty();
	}

	////若输入的数据太多,则一次输入cb字节到RichEdit中
	///StreamIn回递归调用这个函数
	else
	{
		*pcb = cb;
		memcpy(pbBuff, (LPCTSTR) *pstr, *pcb);
		*pstr = pstr->Right(pstr->GetLength() - cb);
	}


	return 0;
}

/************************************************************************/
/* 处理超链接事件                                              */
/************************************************************************/
void CTPRichEditCtrlEx::OnEnLink(NMHDR *pNMHDR, LRESULT *pResult)
{
	ENLINK *pEnLink = reinterpret_cast<ENLINK *>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString	  URL ;
	CHARRANGE CharRange ;

	*pResult = 0;

	switch (pNMHDR->code)
	{
	case EN_LINK:
		pEnLink = (ENLINK *) pNMHDR;

		switch (pEnLink->msg)
		{
		case WM_LBUTTONDOWN:
			GetSel(CharRange);
			SetSel(pEnLink->chrg);
			URL = GetSelText();
			SetSel(CharRange);
			{
				CWaitCursor WaitCursor;

				ShellExecute(GetSafeHwnd(), _T("open"), URL, NULL, NULL, SW_SHOWNORMAL);
				*pResult = 1;
			}

			break;

		case WM_LBUTTONUP:
			*pResult = 1;
			break ;
		}

		break;
	}
}
/************************************************************************/
/* 将控件上的带表情图片内容编码成带转义字符的文本              */
/************************************************************************/
CString CTPRichEditCtrlEx::EncodeRichMessage()
{
	int i,j,lineCount,index=-1;
	vector <int> vecLine;
	CString strText,strFace;	
	GetWindowText(strText);
	if (strText.IsEmpty())
		return _T("");
	while (1)
	{
		index=strText.Find(((TCHAR)13),index+1);
		if (index!=-1)
			vecLine.push_back(index); 
		else
			break;
	}
	
	lineCount=vecLine.size();
	stFace faceNode;
	vector <stFace> vecFace;

	IRichEditOle *pREO = NULL;   
	pREO = GetIRichEditOle();
	int nCount = pREO->GetObjectCount();

	for (i = 0; i < nCount; i++)
	{
		REOBJECT object;  //位图信息存在这里
		memset(&object,0,sizeof(REOBJECT));
		object.cbStruct = sizeof(REOBJECT);
		GetIRichEditOle()->GetObject(i,&object,REO_GETOBJ_ALL_INTERFACES);

		faceNode.nPos = object.cp ; //位图的位置信息
		DWORD dwUSer  =object.dwUser; ///位图的信息，之前应用程序设置的，应有程序当然知道什么意思了
		faceNode.nAtFace=dwUSer;
		faceNode.strFace.Format(_T("{%d}"),dwUSer);		
		faceNode.nLength=faceNode.strFace.GetLength();
		for (j=0;j<lineCount;j++)
		{
			if (faceNode.nPos<vecLine[j])
				break;
			else
				faceNode.nPos+=1;
		}
		vecFace.push_back(faceNode);
	} 
	
	int vecSize=vecFace.size();index=0;
	for (i=0;i<vecSize;i++)
	{		
		strText.Delete(index+vecFace[i].nPos,1);
		strText.Insert(index+vecFace[i].nPos,vecFace[i].strFace);	
		index+=vecFace[i].nLength-1;
	}
	vecLine.clear();vecFace.clear();
	return strText;
}
//泛型算法sort()的排序条件函数，实现按stFace.nPos域排序
bool less_than(stFace &face1, stFace &face2)
{
	return face1.nPos < face2.nPos ? true : false;
}
/****************************************************************************************/
/* 将带转义字符的文本，解码成带表情图片的内容，需要一个位图拾取器的指针对象  */
/*****************************************************************************************/
void CTPRichEditCtrlEx::DecodeRichMessage(CString strText,CBmpPicker* m_BmpPicker)
{
	//SetWindowText(strText);
	long n_index;
	SetSel(-1, -1);
	GetSel(n_index,n_index);
	ReplaceSel(strText+_T("\r\n"));

	if (strText.IsEmpty())
		return ;
	int i,j,lineCount,index=-1,bmpCount,temp,faceCount;
	CString strFace;	
	vector <int> vecLine;	
	while (1)
	{
		index=strText.Find(((TCHAR)13),index+1);
		if (index!=-1)
			vecLine.push_back(index); 
		else
			break;
	}
	lineCount=vecLine.size();

	stFace FaceNode;
	vector <stFace> vecFace;
	if (m_BmpPicker != NULL)
	{
		bmpCount=m_BmpPicker->GetBitmapCount();
		for (i=0;i<bmpCount;i++)
		{		
			strFace.Format(_T("{%d}"),i);
			index=-1;
			while (1)
			{
				index=strText.Find(strFace,index+1);
				if (index!=-1)
				{
					temp=index;
					for (j=0;j<lineCount;j++)
					{
						if (temp<vecLine[j])
							break;
						else
							temp--;
					}
					//SetSel(n_index+temp,n_index+temp+strFace.GetLength());				
					//InsertBitmap(HBITMAP(*m_BmpPicker->GetBitmapAt(i)),i);
					FaceNode.nPos=temp+n_index;
					FaceNode.nAtFace=i;
					FaceNode.strFace=strFace;
					FaceNode.nLength=strFace.GetLength();
					vecFace.push_back(FaceNode);
				} 
				else
					break;
			}
		}
		sort(vecFace.begin(), vecFace.end(), less_than);
		faceCount=vecFace.size();
		temp=0;
		for (i=0;i<faceCount;i++)
		{
			SetSel(vecFace[i].nPos-temp,vecFace[i].nPos+vecFace[i].nLength-temp);
			InsertBitmap(HBITMAP(*m_BmpPicker->GetBitmapAt(vecFace[i].nAtFace)),vecFace[i].nAtFace);
			temp+=vecFace[i].nLength-1;
		}
	}

	vecLine.clear();
	vecFace.clear();
}
/************************************************************************/
/* 按键的处理消息                                              */
/************************************************************************/
void CTPRichEditCtrlEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_type==CHATLOG)
	{			
		SendMessage(WM_KEYDOWN,VK_TAB,0);    //当做为聊天记录框的时候，要被输入的时候，发送TAB键把焦点移到聊天编辑框
		SendMessage(WM_KEYUP,VK_TAB,0);     //所以需要让聊天编辑框的TAB键顺序在聊天记录框之后，按Ctrl+D设置TAB键顺序
		return;
	}
	switch (nChar)
	{
	case 0x16:OnPaste();
		break;return;
	}

	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
}

/************************************************************************/
/* 初始化控件的一些设置                                        */
/************************************************************************/
void CTPRichEditCtrlEx::PreSubclassWindow()
{
	
	SetTargetDevice(NULL,0);	//设置自动换行	
	/*SendMessage(EM_SETTEXTMODE, TM_PLAINTEXT|TM_MULTILEVELUNDO|TM_SINGLECODEPAGE, 0);*/
	SendMessage(EM_SETLANGOPTIONS,0,0);		//关闭RichEdit默认开启的IMF_AUTOFONT和IMF_DUALFONT
	SendMessage(EM_AUTOURLDETECT, TRUE, 0);	//开启自动网址检测
	SetEventMask(GetEventMask() | ENM_LINK) ; 
	SetOptions(ECOOP_OR, ECO_AUTOVSCROLL);
	LimitText(0xffffffff);
	CTPRichEditCtrl::PreSubclassWindow();
}
/************************************************************************/
/* 做为聊天记录框的时候，显示发送者的名称、时间                */
/************************************************************************/
void CTPRichEditCtrlEx::AddMessageUser(CString strName,COLORREF crColor)
{
	SendMessage(EM_SETSEL,-2, -1); //这行一定要，目的是将光标停到最后
	strName = _T(" ") + strName;
	//设置即将要显示的字体颜色

	CHARFORMAT cf;

	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE ;
	cf.dwEffects &=~CFM_BOLD;
	cf.dwEffects &=~CFM_ITALIC;
	cf.dwEffects &=~CFM_UNDERLINE;
	cf.yHeight=180;
	_stprintf_s(cf.szFaceName,32,_T("宋体"));
	cf.crTextColor = crColor;

	SetWordCharFormat(cf);

	//恢愎缩进为0

	PARAFORMAT2 pf2;
	ZeroMemory(&pf2, sizeof(PARAFORMAT2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = 0;
	SetParaFormat(pf2);

	CTime ct=CTime::GetCurrentTime();
	SetSel(-1, -1);
	ReplaceSel(strName+ct.Format(_T(" %H:%M:%S"))+_T("\r\n"));

	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//将滚动条滚到最后
}
/************************************************************************/
/* 做为聊天记录框时，显示发送者的文本内容                      */
/*
 *	strText	聊天内容
 *	crFontName 字体名称
 *	crFontSize 字体大小
 *	bIsBold 是否黑体
 *	bIsItalic 是否斜体
 *	bIsUnderline 是否下划线
 *	crColor	字体颜色
 *	m_BmpPicker 表情拾取器指针
 */
/************************************************************************/
void CTPRichEditCtrlEx::AddMessgaeText(CString strText,CString crFontName,UINT crFontSize,BOOL bIsBold,BOOL bIsItalic,BOOL bIsUnderline,COLORREF crColor,CBmpPicker* m_BmpPicker)
{
	SendMessage(EM_SETSEL,-2, -1);

	CHARFORMAT cf;

	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE ;
	if (bIsBold)
		cf.dwEffects|=CFE_BOLD;
	if (bIsItalic)
		cf.dwEffects |=CFE_ITALIC;
	if (bIsUnderline)
		cf.dwEffects |=CFE_UNDERLINE;
	cf.yHeight =crFontSize*20; 
	_stprintf_s(cf.szFaceName,32,crFontName);
	cf.crTextColor = crColor;
	SetWordCharFormat(cf);

	//设置缩进为200

	PARAFORMAT2 pf2;
	ZeroMemory(&pf2, sizeof(PARAFORMAT2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = 200;
	SetParaFormat(pf2);

	/*SetSel(-1, -1);
	ReplaceSel(strText+_T("\r\n"));*/
	DecodeRichMessage(strText,m_BmpPicker);
	PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CTPRichEditCtrlEx::OnPaint()
{
 	CPaintDC dc(this); 
	CRect rtClient;
 	GetClientRect(&rtClient);
 	dc.FillSolidRect(rtClient,RGB(233,233,233));
}

DWORD CALLBACK SaveToRTFFileCallBack(DWORD dwCookie, LPBYTE pbBuffer, LONG cb,LONG *pcb)
{
	((CFile*)(LONGLONG)(dwCookie))->Write(pbBuffer,cb);
	return 0;
}

void CTPRichEditCtrlEx::SaveToRTFFile(CString sFilePath ,CString sFileName,BOOL bDelete)
{
	CFile file;
	if (bDelete)
	{
		DeleteFile(sFilePath+sFileName);
	}
	if (file.Open(sFilePath+sFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		//file.SeekToEnd();
		EDITSTREAM EditStream;
		EditStream.pfnCallback  = SaveToRTFFileCallBack;
		EditStream.dwCookie		= (DWORD)(LONGLONG)(&file);
		StreamOut(SF_RTF,EditStream);
		file.Close();
	}
	else
	{
		return ;
	}	

}

DWORD CALLBACK ReadFromRTFFileCallBack(DWORD dwCookie, LPBYTE pbBuffer, LONG cb, LONG *pcb)
{
	CFile *pFIle = (CFile *)dwCookie;
	*pcb = pFIle->Read(pbBuffer,cb);
	return 0 ;
}
void CTPRichEditCtrlEx::ReadFromRTFFile(CString sFilePath, CString sFileName,double filePos)
{
	CFile file;
	if (PathFileExists(sFilePath +sFileName))
	{
		if (file.Open(sFilePath+sFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
		{
			ULONGLONG iLength = file.GetLength();
			iLength = (ULONGLONG)(iLength * filePos);
			file.Seek(iLength,CFile::begin);
			EDITSTREAM EditStream;
			EditStream.pfnCallback  = ReadFromRTFFileCallBack;
			EditStream.dwCookie		= (DWORD)(LONGLONG)(&file);
			StreamIn(SF_RTF,EditStream);
			file.Close();
			PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
		}
		else
		{
			return ;
		}	
	}
}

BOOL CTPRichEditCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	  if (pMsg->message == WM_KEYDOWN)
	  {
		  if (pMsg->wParam == VK_RETURN && TP_IsCtrlKeyDown())
		  {
			   if (IsWindowEnabled())
			   {
				   CHARFORMAT cf;
				   GetDefaultCharFormat(cf);
				   AddMessgaeText(_T(""),cf.szFaceName,cf.yHeight/20,(BOOL)(cf.dwEffects & CFE_BOLD),(BOOL)(cf.dwEffects & CFE_ITALIC),(BOOL)(cf.dwEffects & CFE_UNDERLINE),cf.crTextColor,0);
			   }
			   return TRUE;
		  }
	  }
	  return FALSE;
}
void     CTPRichEditCtrlEx::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_WND,&wndClass);		
		wndClass.lpszClassName = WC_RIECHEDITEX;		
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_RIECHEDITEX,AfxGetInstanceHandle());
}
