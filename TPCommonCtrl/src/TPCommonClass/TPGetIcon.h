#pragma once
#include <AfxTempl.h>

struct TPFileInterface;
typedef struct stu_TPGetIconScale
{		
	DWORD    eVideoStandard;
	DWORD	 dwPicturePos;		  //图片的位置
	POINT    ptChipTopLeft;       //裁切左上角
	POINT    ptChipRightBottom;   //裁切右上角
	BOOL	 b1609;				  //是否按1609处理 // LI HUI LI 2009-03-25

	double   dScaleX;             //X缩放
	double   dScaleY;             //Y缩放
	float    dCenterX;           //缩放中心点
	float    dCenterY;           //缩放中心点
}TPGetIconScale;
typedef struct tag_TPGetIconItem
{
	BOOL            bFail;        //shibai
	BOOL            bCancle;       //被取取消
	BOOL            bDelete;       //退出的时候 需要删除 
	BOOL            bNew;          //新建的项目

	BOOL            bIsRead;       //当前正在读	
	CString         sName;         //文件名
	CString         sKeyName;
	BYTE           *pBuffer;       //内存
	CTPFileMap     *pMapFile;      //文件
	SYSTEMTIME      timeUser;      //最后使用时间
	__int64         iFrame;        //当前帧数
	CSize           szBuffer;      //数据大小

	TPGetIconScale  *pScale;
	struct _DrawWnd
	{	
		BOOL           *bRedrawFlag;   //重画标志
		HWND            hWnd;          //重画窗口
		CRect           rtDraw;      //重画区域	
		CRect           rtValidateRect;
		_DrawWnd()
		{	
			bRedrawFlag    = NULL;
			hWnd           = NULL;
			rtDraw         = CRect(0,0,0,0);
			rtValidateRect = CRect(0,0,0,0);
		}		
	} ;
	CArray<_DrawWnd,_DrawWnd &> arrWnd;

	BOOL           *bRedrawFlag;   //重画标志
	HWND            hWnd;          //重画窗口
	CRect           rtDraw;        //重画区域	
	CRect           rtValidateRect;        //重画区域	
	
	CSize             szFile;       //原来文件的大小
	TPFileInterface  *pFileInterface; 

	void           *hTPEffect; 
	__int64         lClipInFrame;
	__int64         lClipOutFrame;

	tag_TPGetIconItem()
	{
		bFail        = FALSE;
		bCancle      = FALSE;
		bDelete      = FALSE;
		bNew         = TRUE;
		bIsRead      = TRUE;		
	    sName        = _L("");
		sKeyName     = _L("");
	    pBuffer      = NULL;
		pMapFile     = NULL;
		GetLocalTime(&timeUser);
	    iFrame       = 0;
	    szBuffer   = CSize(0,0);
	    bRedrawFlag  = NULL;
	    hWnd         = NULL;
		rtDraw       = CRect(0,0,0,0);
		rtValidateRect = CRect(0,0,0,0);

		szFile         = CSize(0,0);
		pFileInterface = NULL;

		hTPEffect      = NULL; 
		lClipInFrame   = -1;
		lClipOutFrame  = -1;
		pScale         = NULL;

	};
	void Invalidate()
	{
		CMutex stuLock(FALSE,_T("GetIcon"));
		stuLock.Lock();
		for(INT k=0;k<this->arrWnd.GetSize();k++)
		{
			if(this->arrWnd[k].hWnd!=NULL&&::IsWindow(this->arrWnd[k].hWnd)) 
			{				
				if(this->arrWnd[k].bRedrawFlag)   *(this->arrWnd[k].bRedrawFlag) +=1;
				if(this->arrWnd[k].rtValidateRect.Width()<=0) ::InvalidateRect(this->arrWnd[k].hWnd,NULL,TRUE);
				else                                          ::InvalidateRect(this->arrWnd[k].hWnd,&this->arrWnd[k].rtValidateRect,TRUE);
			}
		}
		this->arrWnd.RemoveAll();
		stuLock.Unlock();
	}
	BOOL IsDraw(int iWidth)
	{
		for(INT k=0;k<this->arrWnd.GetSize();k++) if(this->arrWnd[k].rtDraw.Width()>iWidth) return TRUE;
		return FALSE;		
	}
	void AddWnd(tag_TPGetIconItem *pStruct)
	{
		CMutex stuLock(FALSE,_T("GetIcon"));
		stuLock.Lock();
		if(pStruct ->hWnd)
		{
			INT k = 0;
			for(k=0;k<this->arrWnd.GetSize();k++)
			{
				if(this->arrWnd[k].hWnd == pStruct->hWnd)						
				{
					this->arrWnd[k].rtValidateRect |= pStruct->rtValidateRect;
					break;
				}
			}
			if(k>=this->arrWnd.GetSize())
			{
				TPGetIconItem::_DrawWnd stuWnd;
				stuWnd.bRedrawFlag          = pStruct->bRedrawFlag;
				stuWnd.hWnd                 = pStruct->hWnd;
				stuWnd.rtValidateRect       = pStruct->rtValidateRect;
				this->arrWnd.Add(stuWnd);
			}				
		}
		stuLock.Unlock();
	}
	~tag_TPGetIconItem()
	{
		if(pMapFile)
		{
			pMapFile->Release();
			delete pMapFile;
			pMapFile = NULL;
		}
		pBuffer    = NULL;
		if(pScale) delete pScale;		
	}
} TPGetIconItem;
typedef CArray <TPGetIconItem *,TPGetIconItem *&> TPGetIconItemArray;

class AFX_EXT_CLASS CTPGetIcon  
{
public:
	CTPGetIcon();
	virtual ~CTPGetIcon();
public:
	int               m_iArrayLength;
	static void      *m_pFileInterface;
	static void      *m_pTimeLineInterface;
	static void      *m_pEffectInterface;
public:
	CMapStringToPtr     m_aData;
	CWinThread         *m_pThread;
	BOOL                m_bCancel;
	DWORD               m_dwTickCount;
	HANDLE              m_hGetEvent;
	CMutex              m_hMutex; 
	BOOL                m_bConntinue;

	static void       (*GetGraphScale)(LPBYTE pSrc,CSize &szBuffer,LPBYTE pDest,TPGetIconScale *pScale);

public:
	static void         SetEffectInterface(void *pEffectInterface);
	static void         SetFileInterface(void *pFileInterface);
	static void         SetTimeLineInterface(void *pTimeLineInterface);	
	

	BOOL   GetIcon(TPGetIconItem * pStruct,BOOL bForce =FALSE);
	BOOL   GetDefaultIcon(int iState,TPGetIconItem * pStruct);

//	BOOL   GetIcon(CString sFileName,__int64 lPos,LPBYTE &pByte,CSize &szBuffer);
//	BOOL   GetIcon(CString sFileName,__int64 lPos,LPBYTE &pByte,CSize &szBuffer,DWORD &dwFileType);

	void   ReleaseAll(); 

	void   ReLoadIcon(CString sFileName,CString sKeyName,int iFrame);
	void   ReLoadAll();

	void   StopThread();
private:
	void   ClearData();
	void   DeleteData();
};
